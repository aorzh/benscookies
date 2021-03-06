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
 * @global type $user
 * @param type $form
 * @param type $form_state
 * @param type $checkout_pane
 * @param type $order
 * @return boolean|string
 */
function delivery_pane_checkout_form($form, $form_state, $checkout_pane, $order) {
    ctools_include('ajax');
    ctools_ajax_command_reload();
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
        return FALSE;
    }

    if (!empty($shortcode) && is_string($shortcode)) {
        foreach ($london_codes as $c) {
            if (0 === stripos($shortcode, $c)) {
                $options = array(
                    'Daytime (9-6pm)' => 'Daytime (9-6pm)',
                    'Lunchtime (11am - 1pm)' => 'Lunchtime (11am - 1pm)',
                    'Early afternoon (1pm - 3pm)' => 'Early afternoon (1pm - 3pm)',
                    'Late afternoon (3pm - 5pm)' => 'Late afternoon (3pm - 5pm)',
                );

                $checkout_form['delivery_pane_field_display']['#prefix'] = '<div id="delivery-wrapper">';
                $checkout_form['delivery_pane_field_display']['#suffix'] = '</div>';

                $checkout_form['delivery_pane_field_display'] = array(
                    '#type' => 'date_popup',
                    '#default_value' => '',
                    '#size' => 21,
                    //'#maxlength' => 100,
                    '#date_flexible' => 0,
                    '#date_timezone' => date_default_timezone(),
                    '#date_year_range' => (date('Y')) . ':+0',
                    '#date_format' => 'd M y',
                    '#required' => TRUE,
                    '#datepicker_options' => array(
                        'minDate' => 0,
                        'changeMonth' => FALSE,
                        'changeYear' => FALSE,
                        'firstDay' => 1,
                    ),
                    '#ajax' => array(
                        'callback' => 'delivery_form_refresh',
                        'wrapper' => 'test-form-wrapper',
                    ),
                );
                $checkout_form['delivery_pane_field_display']['#attached']['js'] = array(
                    drupal_get_path('module', 'delivery') . '/js/london_date.js',
                );

                $hours = date('H');
                $date_now = variable_get('date_now');
                if ($date_now == TRUE) {
                    //Create custom options

                    if ($hours >= '17') {
                        $options = array(
                            'Lunchtime (11am - 1pm)' => 'Lunchtime (11am - 1pm)',
                            'Early afternoon (1pm - 3pm)' => 'Early afternoon (1pm - 3pm)',
                            'Late afternoon (3pm - 5pm)' => 'Late afternoon (3pm - 5pm)',
                        );
                    } elseif ($hours >= '9' && $hours < '18') {
                        $options = array(
                            'Daytime (9-6pm)' => 'Daytime (9-6pm)',
                            'Early afternoon (1pm - 3pm)' => 'Early afternoon (1pm - 3pm)',
                            'Late afternoon (3pm - 5pm)' => 'Late afternoon (3pm - 5pm)',
                        );
                    } elseif ($hours >= '15' && $hours < '18') {
                        $options = array(
                            'Daytime (9-6pm)' => 'Daytime (9-6pm)',
                            'Lunchtime (11am - 1pm)' => 'Lunchtime (11am - 1pm)',
                            'Early afternoon (1pm - 3pm)' => 'Early afternoon (1pm - 3pm)',
                        );
                    }


                    $checkout_form['delivery_date'] = array(
                        '#type' => 'select',
                        '#title' => t('Time slots'),
                        '#options' => $options,
                        '#disabled' => FALSE,
                    );
                }

                $checkout_form['delivery_date'] = array(
                    '#type' => 'select',
                    '#title' => t('Time slots'),
                    '#default_value' => '',
                    '#options' => $options,
                    '#disabled' => FALSE,
                );
                return $checkout_form;
            }
        }
        if (0 !== stripos($shortcode, $c)) {

            $checkout_form['delivery_pane_field_display']['#prefix'] = '<div id="delivery-wrapper">';
            $checkout_form['delivery_pane_field_display']['#suffix'] = '</div>';

            $checkout_form['delivery_pane_field_display_1'] = array(
                '#type' => 'date_popup',
                '#default_value' => '',
                '#size' => 21,
                //'#maxlength' => 100,
                '#date_flexible' => 0,
                //'#date_timezone' => date_default_timezone(),
                '#date_year_range' => (date('Y')) . ':+0',
                '#date_format' => 'd M y',
                '#required' => TRUE,
                '#datepicker_options' => array(
                    'minDate' => 1,
                    'changeMonth' => FALSE,
                    'changeYear' => FALSE,
                    'firstDay' => 1,
                ),
                '#prefix' => '<div id="datepicker-schedule">',
                '#suffix' => '</div>',
            );
            $checkout_form['delivery_pane_field_display_1']['#attached']['js'] = array(
                drupal_get_path('module', 'delivery') . '/js/date.js',
            );
            return $checkout_form;
        }
    }

    return false;
}

/**
 * Ajax callback
 * @param type $form
 * @param type $form_state
 * @return type
 */
function delivery_form_refresh($form, $form_state) {

    $choosed_date = $form_state['values']['delivery']['delivery_pane_field_display'];
    $current_date = date('Y-m-d');

    variable_del('date_now');

    if ($choosed_date == $current_date) {
        variable_set('date_now', TRUE);
    } else {
        variable_set('date_now', FALSE);
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
    $content = array();
    $data = $order->data['delivery_pane_field_display'];

    $content[] = array(
        '#type' => 'item',
        '#title' => t('Date'),
        '#markup' => date('d M y, l', strtotime($data)),
    );
    if (isset($order->data['delivery_pane_field_time_slot']) && $order->data['delivery_pane_field_time_slot'] != NULL) {
        $content[] = array(
            '#type' => 'item',
            '#title' => t('Time slot'),
            '#markup' => $order->data['delivery_pane_field_time_slot'],
        );
    }
    return drupal_render($content);
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return boolean
 */
function delivery_pane_checkout_form_validate($form, $form_state) {
    //do something for validation
    return TRUE;
}