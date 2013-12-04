<?php
/**
 * @file
 * Default XML Schema template for Order FTP Export.
 * 
 * Available variables:
 * - order: the commerce order object.
 * - line_items: array of commerce line item objects indexed by line_item_id.
 * - products: array of commerce products objects indexed by product_id.
 * - profiles: array of commerce profile objects indexed by profile_id.
 * - user: user object (who placed the order).
 */
$profile_id = $order->commerce_customer_billing['und'][0]['profile_id'];
$profile_address = $profiles[$profile_id]->commerce_customer_address['und'][0];
$wrapper = entity_metadata_wrapper('commerce_order', $order);
$total = $wrapper->commerce_order_total->amount->value();
$currency_code = $wrapper->commerce_order_total->currency_code->value();
$profile = commerce_customer_profile_load($profile_id);

$shipping = $wrapper->commerce_customer_shipping->commerce_customer_address->value();
$billing = $wrapper->commerce_customer_billing->commerce_customer_address->value();
$email_b = $wrapper->commerce_customer_billing->field_recipient_s_email_address->value();
$email_s = $wrapper->commerce_customer_shipping->field_recipient_s_email_address->value();


drupal_add_http_header('Content-Type', 'text/xml; charset=utf8');
?>
<Order>
    <OrderTotal>
        <Amount><?php print commerce_currency_amount_to_decimal($total, $currency_code); ?></Amount>
        <CurrencyCode><?php print $currency_code ?></CurrencyCode>
    </OrderTotal>
    <SalesChannel>DRUPAL</SalesChannel>
    <ShippingAddress>
        <Phone></Phone>
        <PostalCode><?php print $shipping['postal_code']; ?></PostalCode>
        <Name><?php print $shipping['name_line']; ?></Name>
        <CountryCode><?php print $shipping['country']; ?></CountryCode>
        <StateOrRegion></StateOrRegion>
        <AddressLine1><?php print $shipping['thoroughfare']; ?></AddressLine1>
        <City><?php print $shipping['locality']; ?></City>
        <Email><?php print $email_s; ?></Email>
    </ShippingAddress>
    <BillingAddress>
        <Phone></Phone>
        <PostalCode><?php print $billing['postal_code']; ?></PostalCode>
        <Name><?php print $billing['name_line']; ?></Name>
        <CountryCode><?php print $billing['country']; ?></CountryCode>
        <StateOrRegion></StateOrRegion>
        <AddressLine1><?php print $billing['thoroughfare']; ?></AddressLine1>
        <City><?php print $billing['locality']; ?></City>
        <Email><?php print $email_b; ?></Email>
    </BillingAddress>
    <BuyerEmail><?php print $order->mail; ?></BuyerEmail>
    <OrderStatus><?php print $order->status; ?></OrderStatus>
    <LastUpdateDate><?php print date('c', $order->changed); ?></LastUpdateDate>
    <PurchaseDate><?php print date('c', $order->created); ?></PurchaseDate>
    <OrderId><?php print $order->order_id; ?></OrderId>
    <DeliveryInstruction>
        <Date>
            <?php print $order->data['delivery_pane_field_display']?>
        </Date>
        <TimeSlot>
            <?php
                if(isset($order->data['delivery_pane_field_time_slot'])){
                    print $order->data['delivery_pane_field_time_slot'];
                }
            ?>
        </TimeSlot>
    </DeliveryInstruction>
    <Carrier>
        Best Way - <?php
        if (trim(strtoupper($shipping['locality'])) == 'LONDON') {
            print 'London Delivery';
        } else {
            print 'UK Mainland Delivery';
        }
        ?>
    </Carrier>
    <GiftInfo>
        <GiftCard>
            <?php
                if(isset($order->data['giftcard_cards'])){
                    print $order->data['giftcard_cards'];
                }
            ?>
        </GiftCard>
        <GiftText>
            <?php
                if(isset($order->data['giftcard_pane_field_display'])){
                    print $order->data['giftcard_pane_field_display'];
                }
            ?>
        </GiftText>
    </GiftInfo>

    <?php
    foreach ($order->commerce_line_items['und'] as $key => $line_item) {
        $item = commerce_line_item_load($line_item['line_item_id']);
        if ($item->type == 'shipping') {
            $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $item);
            $total = $line_item_wrapper->commerce_total->amount->value();
            $currency_code = $line_item_wrapper->commerce_total->currency_code->value();
            ?>

            <ShippingPrice>
                <Amount><?php print $total; ?></Amount>
                <CurrencyCode><?php print $currency_code; ?></CurrencyCode>
            </ShippingPrice>
        <?php }
    } ?>

    <OrderItems>
        <?php
        foreach ($order->commerce_line_items['und'] as $key => $line_item) {
            $item = commerce_line_item_load($line_item['line_item_id']);
            if ($item->type != 'shipping') {
                $wrap = entity_metadata_wrapper('commerce_line_item', $item);
                $cook = json_decode($wrap->field_cookie_selector_1->value());
                $lineitemname = db_select('commerce_product', 'cp')
                        ->fields('cp', array('title'))
                        ->condition('cp.sku', $item->line_item_label, '=')
                        ->execute()
                        ->fetchField();
                $item_wrapper = entity_metadata_wrapper('commerce_line_item', $item);
                $unit = $item_wrapper->commerce_unit_price->amount->value();
                ?>
                <OrderItem>
                    <OrderItemId><?php print $item->line_item_id; ?></OrderItemId>
                    <QuantityOrdered><?php print round($item->quantity); ?></QuantityOrdered>
                    <SellerSKU><?php print $item->line_item_label; ?></SellerSKU>
                    <?php
                    if (isset($item->data['context'])) {
                        $node = node_load($item->data['context']['entity']['entity_id']);
                    }
                    ?>
                    <Title><?php
                        if (isset($node->title)) {
                            print $node->title;
                        }
                        ?></Title>
                    <Cookies>                
                        <?php
                        if ($cook != NULL) {

                            foreach ($cook as $key => $value) {
                                if ($value->count != 0) {
                                    $coockie_names = db_select('node', 'n')
                                            ->fields('n', array('title'))
                                            ->condition('nid', $value->vid, '=')
                                            ->execute()
                                            ->fetchField();
                                    $coockie_sku = db_select('field_data_field_sku ', 'fs')
                                            ->fields('fs', array('field_sku_value'))
                                            ->condition('entity_id', $value->vid, '=')
                                            ->execute()
                                            ->fetchField();
                                    $coockie_name = str_replace("&", "&amp;", $coockie_names);
                                    ?>
                                    <Cookie>
                                        <SellerSKU><?php print $coockie_sku; ?></SellerSKU>			
                                        <Title><?php print $coockie_name; ?></Title>
                                        <QuantityOrdered><?php print $value->count; ?></QuantityOrdered>	
                                    </Cookie>											
                                <?php
                                }
                            }
                        }
                        ?>                                         
                    </Cookies>                
                    <ShippingTax>
                        <Amount></Amount>
                        <CurrencyCode><?php print $currency_code ?></CurrencyCode>
                    </ShippingTax>
                    <ShippingPrice>
                        <Amount></Amount>
                        <CurrencyCode><?php print $currency_code ?></CurrencyCode>
                    </ShippingPrice>
                    <ItemTax>
                        <Amount></Amount>
                        <CurrencyCode><?php print $currency_code ?></CurrencyCode>
                    </ItemTax>
                    <ItemPrice>
                        <Amount><?php print commerce_currency_amount_to_decimal($unit, $currency_code); ?></Amount>
                        <CurrencyCode><?php print $currency_code ?></CurrencyCode>
                    </ItemPrice>
                    <QuantityShipped><?php print round($item->quantity); ?></QuantityShipped>
                    <ShippingDiscount>
                        <Amount></Amount>
                        <CurrencyCode><?php print $currency_code ?></CurrencyCode>
                    </ShippingDiscount>
                </OrderItem>
    <?php }
} ?>
    </OrderItems>
</Order>
<?php drupal_exit(); ?>
