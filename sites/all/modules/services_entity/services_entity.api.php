<?php

/**
 * @file
 * Hooks provided by Services Entity for the definition of Resource Controllers.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Defines Services Entity resource plugins.
 *
 * @return
 *   An associative array with the following keys:
 *   - 'title': The display name of the controller.
 *	 - 'description': A short description of the controller.
 *   - 'class': The class name of the controller. This should be in a separate
 *     file included in the module .info file.
 */
function hook_services_entity_resource_info() {
  $result = array();

  $result['generic'] = array(
    'title' => 'Generic Entity Processor',
    'description' => 'Acts as a generic wrapper for entities. Data structures are exactly what they are in Drupal.',
    'class' => 'ServicesEntityResourceController',
  );

  return $result;
}
