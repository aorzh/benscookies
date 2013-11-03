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
drupal_add_http_header('Content-Type', 'text/xml; charset=utf8');
?>
<PurchaseOrders>
    <?php foreach ($order as $orders) { ?>
        <OrderID><?php print $orders->order_id; ?></OrderID>
    <?php } ?>
</PurchaseOrders>    
<?php drupal_exit(); ?>