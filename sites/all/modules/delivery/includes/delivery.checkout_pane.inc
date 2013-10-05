<?php     

/**
 * Refresh countries
 * @param type $commands
 * @param type $form
 * @param type $form_state
 */
function delivery_addressfield_standard_widget_refresh_alter(&$commands, $form, $form_state) {
    
    // The target element is one element below the triggering country selector.
    $array_parents = $form_state['triggering_element']['#array_parents'];
    array_pop($array_parents);

    // Iterate over the form parents to find the element.
    $element = $form;
    foreach ($array_parents as $name) {

        $element = &$element[$name];
        if (!empty($element['#addressfield'])) {
            break;
        }
    }

    $chosen_country = $element['country']['#default_value'];
    $countries = variable_get('delivery_pane_countries', FALSE);
    if (is_array($countries) && isset($countries[$chosen_country])) {
        ctools_include('ajax');
        variable_set('shipping_country', $chosen_country);
        //$commands[] = ctools_ajax_command_reload();
        $commands[] = ajax_command_invoke('#edit-delivery', 'html', array(render(delivery_pane_checkout_form())));
        $commands[] = ajax_command_css('#edit-delivery', array('display' => 'block'));
    } else {
        variable_set('shipping_country', $chosen_country);
         $commands[] = ajax_command_css('#edit-delivery', array('display' => 'none'));
    }

}

/**
 * Custom checkout pane.
 * 
 * Function name should consist of <pane key>_checkout_form.
 */
function delivery_pane_checkout_form($form, $form_state, $checkout_pane, $order) {
    global $user;
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $shipping = $order_wrapper->commerce_customer_shipping->commerce_customer_address->value();
    $country = $shipping['country'];
    $shortcode = $shipping['postal_code'];
    $order_id = commerce_cart_order_id($uid = $user->uid);

    $london_codes = array(
        'E1', 'W1', 'SE1', 'SW1', 'WC1', 'WC2', 'EC1', 'EC2', 'EC3', 'EC4', 'N1',
        'NW1', 'W2', 'SW3', 'SW7', 'NW8', 'W8', 'W9', 'W11', 'SE11', 'SE17', 'SW5', 'SW8',
        'SW10', 'N7', 'NW5', 'NW6', 'W10', 'W12', 'W14', 'SW6', 'SW9', 'SW11', 'N5', 'N6',
        'N19', 'NW3',
    );

    if ($country != 'GB') {
        
    }

    if (in_array($shortcode, $london_codes)) {

        $options = array(
            '1' => 'Daytime (9-6pm)',
            '2' => 'Lunchtime (11am - 1pm)',
            '3' => 'Early afternoon (1pm - 3pm)',
            '4' => 'Late afternoon (3pm - 5pm)',
        );

        $checkout_form['delivery_pane_field_display']['#prefix'] = '<div id="delivery-wrapper">';
        $checkout_form['delivery_pane_field_display']['#suffix'] = '</div>';

        $checkout_form['delivery_pane_field_display'] = array(
            '#type' => 'date_popup',
            '#default_value' => '',
            '#size' => 21,
            '#maxlength' => 100,
            '#date_flexible' => 0,
            '#date_timezone' => date_default_timezone(),
            '#date_year_range' => (date('Y')) . ':+0',
            '#date_format' => 'D d M Y',
            '#required' => TRUE,
            '#datepicker_options' => array(
                'minDate' => 0,
                'changeMonth' => FALSE,
                'changeYear' => FALSE,
                'firstDay' => 1,
            ),
            '#prefix' => '<div id="datepicker-schedule">',
            '#suffix' => '</div>',
            '#ajax' => array(
                'callback' => 'delivery_form_refresh',
                'wrapper' => 'test-form-wrapper',
            ),
        );

        $oid = db_select('commerce_delivery', 'cd')
                ->fields('cd', array('order_id'))
                ->execute()
                ->fetchCol();
        if ($oid == NULL) {
            $checkout_form['delivery date'] = array(
                '#type' => 'select',
                '#title' => t('Time slots'),
                '#options' => $options,
                '#disabled' => FALSE,
            );
        }
    } else {
        $lasttime = '15:00';
        
        $checkout_form['delivery_pane_field_display']['#prefix'] = '<div id="delivery-wrapper">';
        $checkout_form['delivery_pane_field_display']['#suffix'] = '</div>';

        $checkout_form['delivery_pane_field_display_1'] = array(
            '#type' => 'date_popup',
            '#default_value' => '',
            '#size' => 21,
            '#maxlength' => 100,
            '#date_flexible' => 0,
            '#date_timezone' => date_default_timezone(),
            '#date_year_range' => (date('Y')) . ':+0',
            '#date_format' => 'D d M Y',
            '#required' => TRUE,
            '#datepicker_options' => array(
                'minDate' => 1,
                'changeMonth' => FALSE,
                'changeYear' => FALSE,
                'firstDay' => 1,
            ),
            '#prefix' => '<div id="datepicker-schedule">',
            '#suffix' => '</div>',
            '#ajax' => array(
                'callback' => 'delivery_form_refresh',
                'wrapper' => 'test-form-wrapper',
            ),
        );
        $checkout_form['delivery_pane_field_display_1']['#attached']['js'] = array(
            drupal_get_path('module', 'delivery') . '/js/date.js',
        );
    }

    $checkout_form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Submit',
        '#submit' => array('delivery_form_submit'),
        '#ajax' => array('callback' => 'delivery_pane_checkout_ajax_callback'),
    );
    return $checkout_form;
}

/**
 * Ajax callback
 */
function delivery_form_refresh($form, $form_state, $checkout_pane, $order) {
    $oid = db_select('commerce_delivery', 'cd')
            ->fields('cd', array('order_id'))
            ->execute()
            ->fetchCol();
    $time_slots = db_select('commerce_delivery', 'cd')
            ->fields('cd', array('time_slot'))
            ->execute()
            ->fetchCol();
    
    
    $order = $form_state['order'];
    $order_id = $order->order_id;
    $date = $form_state['values']['delivery']['delivery_pane_field_display'];
    
    

    if (!in_array($order_id, $oid)) {
        //delivery_form_alter();
        /*
        db_insert('commerce_delivery')
                ->fields(array('order_id' => $order_id, 'time_slot' => $time_slot))
                ->execute();*/
    } else {
        db_update('commerce_delivery')
                ->condition('order_id', $order_id)
                ->fields(array('order_id' => $order_id, 'time_slot' => $time_slot))
                ->execute();
    }
 
    return $form;
}

/**
 * Checkout pane callback: returns the cart contents review data for the
 *   Review checkout pane.
 * @param type $form
 * @param type $form_state
 * @param type $checkout_pane
 * @param type $order
 * @return type
 */
function delivery_pane_review($form, $form_state, $checkout_pane, $order) {

    
    if (is_array($countries) && isset($countries[$chosen_country])) {
        $content = array();
        $data = $order->data['delivery_pane_field_display'];
        $content[] = array(
            '#type' => 'item',
            '#title' => t(''),
            '#markup' => date('d F Y, l', strtotime($data)),
        );
        return drupal_render($content);   
    }
}

function  delivery_pane_checkout_form_validate($form, $form_state) {
    //do something for validation
    return TRUE;
}


/**
 * Ajax callback in response to a change of country in an address field.
 *
 * The only thing we have to do is to find the proper element to render.
 */
/*
function delivery_standard_widget_refresh($form, $form_state) {

  // The target element is one element below the triggering country selector.
  $array_parents = $form_state['triggering_element']['#array_parents'];
  array_pop($array_parents);

  // Iterate over the form parents to find the element.
  $element = $form;
  foreach ($array_parents as $name) {
    $element = &$element[$name];
    if (!empty($element['#addressfield'])) {
      break;
    }
  }

  // Return the address block, but remove the '_weight' element inserted
  // by the field API.
  unset($element['_weight']);
  return $element;
}*/
