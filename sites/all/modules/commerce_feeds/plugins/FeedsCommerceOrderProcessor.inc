<?php

/**
 * @file
 * Class definition of FeedsCommerceOrderProcessor.
 */

/**
 * Creates orders from feed items.
 */
class FeedsCommerceOrderProcessor extends FeedsProcessor {

  /**
   * Define entity type.
   */
  public function entityType() {
    return 'commerce_order';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Commerce Orders');
    return $info;
  }

  /**
   * Creates a new order in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $type = $this->config['order_type'];
    $uid = 0;
    $status = NULL;
    $order = commerce_order_new($uid);
    $order->uid = $this->config['author'];
    $order->log = 'Created by FeedsCommerceOrderProcessor';
    return $order;
  }

  /**
   * Loads an existing order.
   */
  protected function entityLoad(FeedsSource $source, $order_id) {
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $order = commerce_order_load($order_id);
      $order->log = 'Updated by FeedsCommerceOrderProcessor';
      return $order;
    }
    return NULL;
  }

  /**
   * Save a order.
   */
  public function entitySave($order) {
    if ($order->order_status) {
      $order->status = $order->order_status
        ? $order->order_status
        : NULL;
    }

    // Load a user by mail and grab the uid.
    if (empty($order->uid) && $order->mail) {
      $order->uid = ($account = user_load_by_mail($order->mail))
        ? $account->uid
        : 0;
    }

    // Load the product by SKU or Product_id.
    if ($order->sku) {
      $product = commerce_product_load_by_sku($order->sku);
    }
    elseif ($order->product_id) {
      $product = commerce_product_load($order->product_id);
    }

    // Update the line item if the product is already attached to the order.
    if (!$order->is_new && ($line_item_id = $this->existingLineItemProductId($order, $product))) {
      $line_item = commerce_line_item_load($line_item_id);
      $clone = clone $line_item;

      // Updating the quantity.
      if ($order->quantity != $line_item->quantity) {
        $line_item->quantity = $order->quantity;
      }

      if ($clone != $line_item) {
        commerce_line_item_save($line_item);
      }
    }
    // Add a new line item if the product is not yet in the order.
    else {
      $line_item = commerce_product_line_item_new($product, $order->quantity, $order->order_id);
      // Save the line item to get its ID.
      commerce_line_item_save($line_item);
      // Add the line item to the order using the wrapper.
      $order_wrapper = entity_metadata_wrapper($this->entityType(), $order);
      $order_wrapper->commerce_line_items[] = $line_item;

      // If not already set, attach the customer profile to the order.
      if (empty($order->commerce_customer_billing)) {
        $this->addCustomerBillingProfileToOrder($order);
      }
      commerce_order_save($order);
    }
  }

  /*
   * Attach a customer profile to an order.
   */
  private function addCustomerBillingProfileToOrder($order) {
    $order_wrapper = entity_metadata_wrapper($this->entityType(), $order);

    // @TODO: access to that data using a better approach.
    $customer_profile_value = reset($order->commerce_customer_address[LANGUAGE_NONE]);

    // Tried to get a profil_id by comparing address informations from the order.
    if (!empty($order->commerce_customer_address)) {
      // Use EntityFieldQuery to compare for address field informations.
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'commerce_customer_profile')
        ->entityCondition('bundle', 'billing');

      // Grab only the default structure values from the address informations.
      $addressfield_values = array_intersect_key($customer_profile_value, addressfield_data_property_info());
      // Added field condition to the query if specific address information has
      // been filled.
      foreach($addressfield_values as $key => $value) {
        $query->fieldCondition('commerce_customer_address', $key, $value);
      }
      $result = $query->execute();

      // Return the profil_id if a customer profile match with all the address
      // field informations contained in the order.
      if (!empty($result)) {
        $profile = reset($result['commerce_customer_profile']);
        $order_wrapper->commerce_customer_billing->set($profile->profile_id);
        return;
      }
    }

    // If no customer profile has been found, create a new one with informations
    // contained in the address field.
    $customer_profile = commerce_customer_profile_new('billing', $order->uid);

    // Added addressfield default value to the address informations contained
    // in the order.
    $customer_profile_value += addressfield_default_values();
    $profil_wrapper = entity_metadata_wrapper('commerce_customer_profile', $customer_profile);
    $profil_wrapper->commerce_customer_address = $customer_profile_value;
    commerce_customer_profile_save($customer_profile);

    // Saved the profile_id in the order.
    $order_wrapper->commerce_customer_billing->set($customer_profile->profile_id);
    return;
  }

  /**
   * Delete a series of orders.
   */
  protected function entityDeleteMultiple($order_ids) {
    commerce_order_delete_multiple($order_ids);
    // @TODO: should we also delete profiles created ?
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $order_types = commerce_order_type_get_name();
    $customer_profile_types = commerce_customer_profile_type_get_name();
    return array(
      'order_type' => key($order_types),
      'customer_profile_type' => key($customer_profile_types),
      'author' => 0,
      'status' => '',
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    unset($form['input_format']);

    $order_types = commerce_order_type_get_name();
    array_walk($order_types, 'check_plain');
    $form['order_type'] = array(
      '#type' => 'select',
      '#title' => t('Order type'),
      '#description' => t('Select the order type for the order to be created.'),
      '#options' => $order_types,
      '#default_value' => $this->config['order_type'],
    );

    $customer_profile_types = commerce_customer_profile_type_get_name();
    array_walk($customer_profile_types, 'check_plain');
    $form['customer_profile_type'] = array(
      '#type' => 'select',
      '#title' => t('Customer profile type'),
      '#description' => t('Select the customer profile type.'),
      '#options' => $customer_profile_types,
      '#default_value' => $this->config['customer_profile_type'],
    );

    $form['update_existing']['#options'] = array(
      FEEDS_SKIP_EXISTING   => 'Do not update existing orders',
      FEEDS_UPDATE_EXISTING => 'Update existing orders',
    );

    return $form;
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();
    $targets += array(
      // Order infos.
      'order_id' => array(
        'name' => t('Order: ID'),
        'description' => t('The order identifier.'),
      ),
      'created' => array(
        'name' => t('Order: Created'),
        'description' => t('Unix timestamp when the order was created.'),
      ),
      'order_status' => array(
        'name' => t('Order: Status'),
        'description' => t('Status of the order.'),
      ),
      'customer_profile_id' => array(
        'name' => t('Order: Customer Profile Id'),
        'description' => t('The Id of the customer profile.'),
      ),
      // Customer infos.
      'uid' => array(
        'name' => t('User: ID'),
        'description' => t('Drupal user ID that owns the order.'),
      ),
      'mail' => array(
        'name' => t('User: Mail'),
        'description' => t('Address associated with the order.'),
      ),
      'username' => array(
        'name' => t('User: Name'),
        'description' => t('The customer\'s name.'),
      ),
      // Product infos.
      'sku' => array(
        'name' => t('Product: SKU'),
        'description' => t('The product identifier.'),
      ),
      'quantity' => array(
        'name' => t('Product: Quantity'),
        'description' => t('The purchased quantitiy of the product.'),
      ),
    );


    // Let's implement hook_feeds_processor_targets() to convert address feeds
    // informations into an address field.
    try {
      self::loadMappers();
      $entity_customer_profile_type = 'commerce_customer_profile';
      drupal_alter('feeds_processor_targets', $targets, $entity_customer_profile_type, $this->config['customer_profile_type']);
    } catch (Exception $e) {
      // Do nothing.
    }

    // Let implementers of hook_feeds_processor_targets() add their targets.
    try {
      self::loadMappers();
      drupal_alter('feeds_processor_targets', $targets, $this->entityType(), $this->config['order_type']);
    } catch (Exception $e) {
      // Do nothing.
    }

    return $targets;
  }

  /**
   * Get the order_id of an existing order if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($order_id = parent::existingEntityId($source, $result)) {
      return $order_id;
    }

    $item = $result->current_item;
    if (!empty($item['order_id']) || $item['mail'] || $item['created']) {
      $query = db_select('commerce_order')
        ->fields('commerce_order', array('order_id'));

      if (!empty($item['order_id'])) {
        $query->condition('order_id', $item['order_id']);
      }
      if ($item['mail']) {
        $query->condition('mail', trim($item['mail']));
      }
      if ($item['created']) {
        $query->condition('created', trim($item['created']));
      }

      $order_id = $query->execute()->fetchfield();
      if ($order_id) {
        return $order_id;
      }
    }
    return 0;
  }

  /**
   * Validate the commerce_order entity.
   */
  protected function entityValidate($order) {
    if (empty($order->sku) && empty($order->product_id)) {
      throw new FeedsValidationException(t('Required product SKU or product ID is missing.'));
    }
    else if (!empty($order->sku) && !commerce_product_validate_sku($order->sku)) {
      throw new FeedsValidationException(t('Product SKU ("@sku") is invalid.', array('@sku' => $order->sku)));
    }
    else if (!empty($order->product_id) && !commerce_product_load($order->product_id)) {
      throw new FeedsValidationException(t('Product ID ("@product_id") is invalid.', array('@product_id' => $order->product_id)));
    }

    global $statuses;
    if (empty($statuses)) {
      $statuses = commerce_order_statuses();
    }
    if (!$statuses[$order->order_status]) {
      throw new FeedsValidationException(t('Order status ("@status") is invalid.', array('@status' => $order->order_status)));
    }
  }

  /**
   * Get line_item_id if a product is available in a commerce line item of an order.
   */
  private function existingLineItemProductId($order, $product) {
    foreach (entity_metadata_wrapper($this->entityType(), $order->order_id)->commerce_line_items as $delta => $line_item_wrapper) {
      if (in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
        if ($product->product_id == $line_item_wrapper->commerce_product->raw()) {
          return $line_item_wrapper->line_item_id->value();
        }
      }
    }
    return NULL;
  }
}