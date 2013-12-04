<?php
/**
 * @file
 * commerce_feeds_example.feeds_importer_default.inc
 */

/**
 * Implementation of hook_feeds_importer_default().
 */
function commerce_feeds_example_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'product_importer';
  $feeds_importer->config = array(
    'name' => 'Product Importer',
    'description' => 'Grab books from CSV and turn them into products',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsCommerceProductProcessor',
      'config' => array(
        'product_type' => 'product',
        'author' => '1',
        'mappings' => array(
          0 => array(
            'source' => 'SKU',
            'target' => 'sku',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'Title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'Image',
            'target' => 'field_image',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'Price',
            'target' => 'commerce_price:amount',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'input_format' => NULL,
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => 1800,
    'expire_period' => 3600,
    'import_on_create' => TRUE,
    'process_in_background' => FALSE,
  );
  $export['product_importer'] = $feeds_importer;

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'product_reference_importer';
  $feeds_importer->config = array(
    'name' => 'Product Reference Importer',
    'description' => 'Pull the CSV feed in and make it into nodes',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'product_display',
        'expire' => '-1',
        'author' => '1',
        'mappings' => array(
          0 => array(
            'source' => 'SKU',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'Title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'SKU',
            'target' => 'field_product:sku',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'Description',
            'target' => 'body',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '1',
        'input_format' => 'full_html',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => 1800,
    'expire_period' => 3600,
    'import_on_create' => TRUE,
    'process_in_background' => FALSE,
  );
  $export['product_reference_importer'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'order_impoter';
  $feeds_importer->config = array(
    'name' => 'Order Impoter',
    'description' => 'Import oders from CSV. Need to run after the import product examples',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsCommerceOrderProcessor',
      'config' => array(
        'order_type' => 'commerce_order',
        'author' => 0,
        'status' => '',
        'mappings' => array(
          0 => array(
            'source' => 'created',
            'target' => 'created',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'status',
            'target' => 'order_status',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'sku',
            'target' => 'sku',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'quantity',
            'target' => 'quantity',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'name',
            'target' => 'username',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'mail',
            'target' => 'mail',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'country',
            'target' => 'commerce_customer_address:country',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'area',
            'target' => 'commerce_customer_address:administrative_area',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'locality',
            'target' => 'commerce_customer_address:locality',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'postal_code',
            'target' => 'commerce_customer_address:postal_code',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'street',
            'target' => 'commerce_customer_address:thoroughfare',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'name',
            'target' => 'commerce_customer_address:name_line',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => 1800,
    'expire_period' => 3600,
    'import_on_create' => TRUE,
    'process_in_background' => FALSE,
  );
  $export['order_importer'] = $feeds_importer;

  return $export;
}
