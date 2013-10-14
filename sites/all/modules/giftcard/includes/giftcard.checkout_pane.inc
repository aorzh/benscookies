<?php

/**
 * 
 * @return type
 */
function giftcard_ajax_disable_select() {
    ctools_include('ajax');
    $commands = array();
    return array('#type' => 'ajax', '#commands' => $commands);
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
function giftcard_pane_checkout_form($form, $form_state, $checkout_pane, $order) {

    global $user;
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $shipping = $order_wrapper->commerce_customer_shipping->commerce_customer_address->value();
    $country = $shipping['country'];
    $shortcode = $shipping['postal_code'];
    $order_id = commerce_cart_order_id($uid = $user->uid);

    $checkout_form['giftcard_pane_field_display']['#prefix'] = '<div id="giftcard-wrapper">';
    $checkout_form['giftcard_pane_field_display']['#suffix'] = '</div>';

    $checkout_form['giftcard_pane_field_display'] = array(
        '#type' => 'textarea',
        '#title' => t('Include a Gift Message'),
    );
    $print_opt = array(
        '1' => 'Yes',
        '2' => 'No',
    );

    $checkout_form['giftcard_print'] = array(
        '#type' => 'radios',
        '#title' => t('Print your Message on a Greeting Card for 2?'),
        '#options' => $print_opt,
        '#ajax' => array(
            'callback' => 'giftcard_form_refresh',
            'wrapper' => 'test-form-wrapper',
        ),
    );
    $card_opt = array(
        'Birthday' => 'Birthday',
        'Congradulations' => 'Congradulations',
        'Thanks' => 'Thanks',
        'Thinking of You' => 'Thinking of You',
        "Ben's Logo" => "Ben's Logo",
    );
    $checkout_form['giftcard_cards'] = array(
        '#type' => 'select',
        '#options' => $card_opt,
        '#attributes' => array(
          'class' => array('hidden'),  
        ),
    );

    return $checkout_form;
}

/**
 * Ajax callback
 * @param type $form
 * @param type $form_state
 * @return type
 */
function giftcard_form_refresh($form, $form_state) {
    ctools_include('ajax');
    $commands = array();
    /*ob_start();
    krumo($form_state); 
    $data = ob_get_clean();
    $fp = fopen("/home/neo/www/cookies.loc/log.html", "w");
    fwrite($fp, $data);
    fclose($fp);*/
    
    if($form_state['values']['giftcard']['giftcard_print'] == '1'){
        $commands[] = ajax_command_invoke('#edit-giftcard-giftcard-cards', 'removeClass', array('hidden'));
    } else {
        $commands[] = ajax_command_invoke('#edit-giftcard-giftcard-cards', 'addClass', array('hidden'));
    }
    return array('#type' => 'ajax', '#commands' => $commands);
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
function giftcard_pane_review($form, $form_state, $checkout_pane, $order) {
    $content = array();

    $data_text = $order->data['giftcard_pane_field_display'];
    $data_card = $order->data['giftcard_cards'];
    $content[] = array(
        '#type' => 'item',
        '#title' => t('Gift Card text: '),
        '#markup' => $data_text,
    );
    if (isset($data_card) && $data_card != NULL) {
        $content[] = array(
            '#type' => 'item',
            '#title' => t('Gift Card: '),
            '#markup' => $data_card,
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
function giftcard_pane_checkout_form_validate($form, $form_state) {
    //do something for validation
    return TRUE;
}