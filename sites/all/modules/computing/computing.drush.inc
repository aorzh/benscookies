<?php

/**
 * @file
 *   Drupal Hybrid Computing drush interface.
 *   To use this, please install Drush at http://drupal.org/project/drush
 */

/**
 * Implements hook_drush_command().
 */
function computing_drush_command() {
  $items = array();

  $items['computing-create'] = array(
    'description' => "Create a computing record directly from drush",
    //'aliases' => array('ca'),
    'drupal dependencies' => array('computing'),
    'arguments' => array(
      'app'         => 'App name of this command.',
      'command'     => 'Please write down the command. Remember to enclose spaces in double quotes.',
      'description' => 'Description of this command. Enclose spaces in double quotes.',
      'fields'      => 'Vertical bar separated key-value pairs. Eg. field1=value1|field2=value2.'
    ),
    'options' => array(
      'json' => 'Fields is in json rather than the key-value pairs'
    ),
    //'callback' => 'computing_create_record',
    //'callback arguments' => array('default'),
    'examples' => array(
      'drush computing-add your_app your_command your_description "field1=value1|field1=value2"' =>
            "Create 'your_command'. Remember to enclose spaces into double quotes."
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  $items['computing-purge'] = array(
    'description' => 'Remove all commands where status is not null.',
    //'drupal dependencies' => array('computing'),
    //'aliases' => array('cpurge'),
    'options' => array(
      'force' => 'Controls whether to purge all command records or finished ones.'
    ),
    'examples' => array(
      'drush computing-purge' => 'Purge all finished commands.',
      'drush computing-purge --force' => 'Purge all commands.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
  );

  $items['computing-list'] = array(
    'description' => 'List computing records.',
    //'drupal dependencies' => array('computing'),
    //'aliases' => array('cpurge'),
    'options' => array(
      'all' => 'Controls whether to list all records, or records not processed yet..'
    ),
    'examples' => array(
      'drush computing-list' => 'List active records not getting processed.',
      'drush computing-list --all' => 'List all commands.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
  );

  $items['computing-call'] = array(
    'description' => 'Call any Drupal/PHP functions and print results as json for further process. All parameters must be josn.',
    'hidden' => TRUE,
    //'examples' => array(
    //  'drush computing-call "node_load 1"' => 'Display node 1 data in json',
    //),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  // copied some code from php-eval in core.drush.inc.
  $items['computing-eval'] = array(
    'description' => 'Call any Drupal/PHP snippet and print results as json for further process.',
    'hidden' => TRUE,
    'arguments' => array(
      'code' => 'PHP code, or "-" to read code from STDIN',
    ),
    'required-arguments' => TRUE,
    'examples' => array(
      'drush computing-eval "$nid=1; return node_load($nid);"' => 'Display node 1 data in json',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}



/**
 * Implementation of hook_drush_help().
 */
function computing_drush_help($section) {
  switch ($section) {
    case 'drush:computing-create':
      return dt("Create a computing record directly from drush.");
    case 'drush:computing-purge':
      return dt('Remove all command records.');
    case 'drush:computing-list':
      return dt('List computing records.');
    case 'meta:computing:title':
      return dt('Hybrid computing');
    case 'meta:computing:summary':
      return dt('Helps you run hybrid computing scripts in non-PHP code');
  }
}



function drush_computing_create($app, $command, $description = NULL, $fields = NULL) {
  if (drush_get_option('json', NULL) && !empty($fields)) {
    $fields_json = $fields;
    $fields = drush_json_decode($fields_json);
    if (!is_array($fields)) {
      drush_set_error('JSON_PARSE_ERROR', "Cannot parse fields json: $fields_json");
    }
  }
  else if (!empty($fields)) {
    // attention: this might have escape characters issue.
    $pairs = explode('|', $fields);
    $fields = array();
    foreach ($pairs as $pair) {
      $key_value = explode('=', $pair);
      $fields[$key_value[0]] = $key_value[1];
    }
  }
  else {
    $fields = array();
  }

  //var_dump($options);
  $id = computing_create_record($app, $command, $description, $fields);
  drush_print("Created command with ID: ". $id);
  drush_print_pipe($id);
}



function drush_computing_purge() {
  $force = drush_get_option('force', NULL);
  if (!$force) {
    $n = db_query("DELETE FROM {computing_record} WHERE status IS NOT NULL")->rowCount();
  }
  else {
    $n = db_query("DELETE FROM {computing_record}")->rowCount();
  }
  drush_print("Purge computing_record table successful. $n records purged.");
}



function drush_computing_list() {
  // FIXME: not working yet.
  $sql = drush_get_option('all', NULL)
    ? 'SELECT * FROM {computing_record};'
    : 'SELECT id, app, command, description, created FROM {computing_record} WHERE status IS NULL;';
  $result = drush_invoke_process('@self', 'sql-query', array($sql), array('--db-prefix'));
  //$result = drush_dispatch('sql-query', array($sql, '--db-prefix'));
  //drush_print_r(drush_backend_get_result());
}



function drush_computing_call() {
  //drush_print(func_num_args());
  // looks like it's fine to use drush_get_arguments(), but I'll use func_get_args() instead..
  $arguments = func_get_args();

  $func_name = array_shift($arguments);
  if (!function_exists($func_name)) {
    error_log("Function '$func_name' does not exist.");
    exit(-1);
  }

  // note: each argument is
  $result = @call_user_func_array($func_name, array_map('drush_json_decode', $arguments));
  //var_dump($result);
  //var_dump($result === NULL);

  $json = drush_json_encode($result);
  // $json = drush_format($result, NULL, 'json')
  drush_print("JSON output: \n{$json}");
  // see [#1697890], a simple work around is to add a whitespace.
  drush_print_pipe($json .' ');
}



function drush_computing_eval($code) {
  // read STDIN followed the example of drush_core_php_script()
  $php = $code == '-' ? stream_get_contents(STDIN) : $code;
  // todo: might need to encode/decode here.
  $result = eval($php);

  //drush_print(serialize($result));
  $json = drush_json_encode($result);
  drush_print("JSON output: \n$json");
  drush_print_pipe($json .' ');

  // the returned value would be saved in $results['object']
  return $result;
}
