<?php

/**
 * @file
 * This file is empty by default because the base theme chain (Alpha & Omega) provides
 * all the basic functionality. However, in case you wish to customize the output that Drupal
 * generates through Alpha & Omega this file is a good place to do so.
 *
 * Alpha comes with a neat solution for keeping this file as clean as possible while the code
 * for your subtheme grows. Please read the README.txt in the /preprocess and /process subfolders
 * for more information on this topic.
 */

/**
 * Preprocess variables for html.tpl.php
 *
 * @see system_elements()
 * @see html.tpl.php
 */
function omega_kickstart_preprocess_html(&$variables) {
  // Add conditional stylesheets for IE
  $theme_path = drupal_get_path('theme', 'omega_kickstart');
  drupal_add_css($theme_path . '/css/ie-lte-8.css', array('group' => CSS_THEME, 'weight' => 20, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css($theme_path . '/css/ie-lte-7.css', array('group' => CSS_THEME, 'weight' => 21, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
  
  
  
  /*  User redirect if order price more than 100 gbp */
  
  global $user;
  $userRoles = $user;
  $uri = request_uri();
  
  if (strpos($uri, '/checkout') !== FALSE){
	$uid = $user->uid;  
    $order_id = commerce_cart_order_id($uid);
    $order = commerce_order_load($order_id);
    $wrapper = entity_metadata_wrapper('commerce_order', $order);
	$total = $wrapper->commerce_order_total->amount->value();
	$currency_code = $wrapper->commerce_order_total->currency_code->value();
	
	$decimal =  commerce_currency_amount_to_decimal($total, $currency_code);
    print_r($decimal);
    
    if($decimal >= 100){
		drupal_goto('webform/large-order-size');
	}
 
    
  }
  //print'<pre>';
  //print_r($userRoles);
  //print $userRoles->roles['3'];
  //print'<pre>';

}








/**
 * Implements hook_css_alter().
 *
 * TODO: Remove the function when patch will be applied to omega.
 * http://drupal.org/node/1784780
 */
function omega_kickstart_css_alter(&$css) {
  global $language;
  if ($language->direction == LANGUAGE_RTL) {
    foreach ($css as $key => $item) {
      if (!isset($item['basename']) || strpos($key, '-rtl.css') !== FALSE) {
        continue;
      }
      $css[$key]['basename'] = 'RTL::' .  $item['basename'];
    }
  }
}

/**
 * Preprocess field.
 */
function omega_kickstart_preprocess_field(&$variables) {
  $element = $variables['element'];
  if ($element['#entity_type'] != 'node' || $element['#field_name'] != 'title_field') {
    return;
  }
  $variables['theme_hook_suggestions'][] = 'field__fences_h2__node';
}

/**
 * Override the submitted variable.
 */
function omega_kickstart_preprocess_node(&$variables) {
  $variables['submitted'] = $variables['date'] . ' - ' . $variables['name'];
  if ($variables['type'] == 'blog_post') {
    $variables['submitted'] = t('By') . ' ' . $variables['name'] . ', ' . $variables['date'];
  }
}

///////////////////////////



 

