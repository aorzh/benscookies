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
 *
 *
 * If I add node__product to theme_hook_suggestions Then I can add a template file 'node--product.tpl.php'
 * @param string $vars
 */
function bens_cookies_process_node(&$vars) {
   // krumo($vars['node']);
   // die();
    // If some node type then...
    switch ($vars['type']){
        case "product_display":
            $vars['theme_hook_suggestions'][] = 'node__product';
            break;
        case "page":
            $vars['theme_hook_suggestions'][] = 'node__page';
            break;
    }

}

/**
 * Create templates for regions
 * @param type $vars
 */
function bens_cookies_alpha_preprocess_region(&$vars) {
    $menu_object = menu_get_object();
    if (isset($menu_object->type) && $vars['region'] == 'content') {
        $vars['theme_hook_suggestions'][] = 'region__content__'.$menu_object->type;
        $vars['attributes_array']['class'][] = 'region-content-'.$menu_object->type;
    }
    //dpm($menu_object);
}