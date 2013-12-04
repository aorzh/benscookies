<?php
/**
 * This class is designed to create a very clean API that integrates with
 * the services and entity modules. We want to strip all "drupalisms" out
 * of the API. For example, there should be no [LANGUAGE_NONE][0][value] or
 * field_ in the API.
 *
 * It should be possible to create an API that is easily replicated on another
 * system.
 *
 * Much of this code is borrowed from restws module.
 */
class ServicesEntityResourceControllerClean extends ServicesEntityResourceController {

  public function create($entity_type, array $values) {
    $property_info = entity_get_all_property_info($entity_type);
    $values = $this->transform_values($entity_type, $property_info, $values);
    try {
      $wrapper = entity_property_values_create_entity($entity_type, $values);
    }
    catch (EntityMetadataWrapperException $e) {
      services_error($e->getMessage(), 406);
    }

    // Make sure that bundle information is present on entities that have
    // bundles.
    $entity_info = entity_get_info($entity_type);
    if (isset($entity_info['bundle keys'])) {
      foreach ($entity_info['bundle keys'] as $bundle_key) {
        if (!array_key_exists($bundle_key, $values)) {
          services_error('Missing bundle: ' . $bundle_key, 406);
        }
      }
    }

    $properties = $wrapper->getPropertyInfo();
    $diff = array_diff_key($values, $properties);
    if (!empty($diff)) {
      services_error('Unknown data properties: ' . implode(' ', array_keys($diff)) . '.', 406);
    }
    $wrapper->save();
    return $this->get_data($wrapper, '*');
  }

  public function retrieve($entity_type, $entity_id, $fields) {
    $entity = parent::retrieve($entity_type, $entity_id, '*');
    return $this->get_data(entity_metadata_wrapper($entity_type, $entity), $fields);
  }

  public function update($entity_type, $entity_id, array $values) {
    $property_info = entity_get_all_property_info($entity_type);
    $values = $this->transform_values($entity_type, $property_info, $values);
    try {
      $wrapper = entity_metadata_wrapper($entity_type, $entity_id);
      foreach ($values as $name => $value) {
        $wrapper->{$name}->set($value);
      }
    }
    catch (EntityMetadataWrapperException $e) {
      services_error($e->getMessage(), 406);
    }
    $wrapper->save();
    return $this->get_data($wrapper, '*');
  }

  public function index($entity_type, $fields, $parameters, $page, $pagesize, $sort, $direction) {
    $property_info = entity_get_all_property_info($entity_type);
    $parameters = $this->transform_values($entity_type, $property_info, $parameters);
    $sort = (isset($property_info['field_' . $sort]))?'field_' . $sort:$sort;

    // Call the parent method, which takes care of access control.
    $entities = parent::index($entity_type, '*', $parameters, $page, $pagesize, $sort, $direction);
    foreach($entities as $entity) {
      $return[] = $this->get_data(entity_metadata_wrapper($entity_type, $entity), $fields);
    }
    return $return;
  }

  /**
   * Implements ServicesResourceControllerInterface::field().
   */
  public function field($entity_type, $entity_id, $field_name, $fields = '*', $raw = FALSE) {
    $entity = entity_load_single($entity_type, $entity_id);
    if (!$entity) {
      services_error('Entity not found', 404);
    }

    $field_name = preg_replace('/^field_/', '', $field_name);

    // The metadata wrapper checks entity_access() on each entity in the field.
    $return = $this->get_data(entity_metadata_wrapper($entity_type, $entity), $field_name);
    return $return;
  }

  /**
   * Return the data structure for an entity stripped of all "drupalisms" such as
   * field_ and complex data arrays.
   *
   * @param type $wrapper
   * @return type
   */
  protected function get_data($wrapper, $fields = '*') {
    if ($fields != '*') {
      $fields_array = explode(',', $fields);
    }
    $data = array();
    $filtered = $this->property_access_filter($wrapper);
    foreach ($filtered as $name => $property) {
      // We don't want 'field_' at the beginning of fields. This is a drupalism and shouldn't be in the api.
      $name = preg_replace('/^field_/', '', $name);
      // If fields is set and it isn't one of them, go to the next.
      if ($fields != '*' && !in_array($name, $fields_array)) {
        continue;
      }
      try {
        if ($property instanceof EntityDrupalWrapper) {
          // For referenced entities only return the URI.
          if ($id = $property->getIdentifier()) {
            $data[$name] = $this->get_resource_reference($property->type(), $id);
          }
        }
        elseif ($property instanceof EntityValueWrapper) {
          $data[$name] = $property->value();
        }
        elseif ($property instanceof EntityListWrapper || $property instanceof EntityStructureWrapper) {
          $data[$name] = $this->get_data($property);
        }
      }
      catch (EntityMetadataWrapperException $e) {
        // A property causes problems - ignore that.
      }
    }
    // If bundle = entity_type, don't send it.
    if (method_exists($wrapper, 'entityInfo')) {
      $entity_info = $wrapper->entityInfo();
      if (isset($entity_info['bundle keys'])) {
        foreach ($entity_info['bundle keys'] as $bundle_key) {
          if (array_key_exists($bundle_key, $data) && $data[$bundle_key] == $wrapper->type()) {
            unset($data[$bundle_key]);
          }
        }
      }
    }
    return $data;
  }

  /**
   * Return a resource reference array.
   *
   * @param type $resource
   * @param type $id
   * @return type
   */
  protected function get_resource_reference($resource, $id) {
    $return = array(
      'uri' => services_resource_uri(array('entity_' . $resource, $id)),
      'id' => $id,
      'resource' => $resource,
    );
    if (module_exists('uuid') && entity_get_info($resource)) {
      $ids = entity_get_uuid_by_id($resource, array($id));
      if ($id = reset($ids)) {
        $return['uuid'] = $id;
      }
    }
    return $return;
  }

  /**
   * Filters out properties where view access is not allowed for the current user.
   *
   * @param EntityMetadataWrapper $wrapper
   *   EntityMetadataWrapper that should be checked.
   *
   * @return
   *   An array of properties where access is allowed, keyed by their property
   *   name.
   */
  protected function property_access_filter($wrapper) {
    $filtered = array();
    foreach ($wrapper as $name => $property) {
      if ($property->access('view')) {
        $filtered[$name] = $property;
      }
    }
    return $filtered;
  }

  /**
   * Checks for field_ prefix for each field and adds it if necessary.
   *
   * @param type $values
   * @return type
   */
  protected function transform_values($entity_type, $property_info, $values) {
    // Default the bundle to the entity_type. This allows eck types that don't use custom bundles to not have to specify the bundle.
    $entity_info = entity_get_info($entity_type);
    if (isset($entity_info['bundle keys'])) {
      foreach ($entity_info['bundle keys'] as $bundle_key) {
        if (!array_key_exists($bundle_key, $values)) {
          $values[$bundle_key] = $entity_type;
        }
      }
    }

    foreach($values as $key => $value) {
      // Handle Resource references so we can pass pack the object.
      if (is_array($value) && isset($value['id'])) {
        $values[$key] = $value['id'];
      }
      // Check if this is actually a field_ value
      if (isset($property_info['field_' . $key])) {
        $values['field_' . $key] = $values[$key];
        unset($values[$key]);
      }
    }
    return $values;
  }
}