<?php

function computing_views_data() {
  $data = array();
  $data['computing_record'] = array(
    'table' => array(
      'group' => t('Hybrid Computing'),
      'help' => t('All issued command records of the Drupal Hybrid Computing module.'),
      'base' => array(
        'field' => 'id',
        'title' => t('Hybrid Computing'),
        'help' =>  t('All issued command records of the Drupal Hybrid Computing module.'),
      ),
    ),

    'id' => array(
      'title' => t('ID'),
      'help' => t('Command record ID in the queue.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),

    'app' => array(
      'title' => t('Application'),
      'help' => t('Application that issued the command.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),

    'command' => array(
      'title' => t('Command'),
      'help' => t('Command issued.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),

    'description' => array(
      'title' => t('Description'),
      'help' => t('Description of this command.'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),

    // if you need field/sort/filter for uid, use the relationship first.
    'uid' => array(
      'title' => t('Uid'),
      'help' => t('User ID who issued the command.'),
      //'field' => array(
      //  'handler' => 'views_handler_field',
      //  'click sortable' => TRUE,
      //),
      //'sort' => array(
      //  'handler' => 'views_handler_sort',
      //),
      //'filter' => array(
      //  'handler' => 'views_handler_filter_user_name',
      //),
      'relationship' => array(
        'base' => 'users',
        'base field' => 'uid',
        'handler' => 'views_handler_relationship',
        'label' => t('User'),
      ),
    ),

    'nid' => array(
      'title' => t('Nid'),
      'help' => t('Node ID associated with the command.'),
      'relationship' => array(
        'base' => 'node',
        'base field' => 'nid',
        'handler' => 'views_handler_relationship',
        'label' => t('Node'),
      ),
    ),

    'created' => array(
      'title' => t('Created'),
      'help' => t('The timestamp of this command was created.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
    ),

    'input' => array(
      'title' => t('Input'),
      'help' => t('Input of the command; usually a serialized string.'),
      'field' => array(
        'handler' => 'views_handler_field_serialized',
      ),
    ),

    'output' => array(
      'title' => t('Output'),
      'help' => t('Output of the command; usually a serialized string.'),
      'field' => array(
        'handler' => 'views_handler_field_serialized',
      ),
    ),

    'inputjson' => array(
      'title' => t('Input JSON'),
      'help' => t('Input of the command in JSON.'),
      'field' => array(
        // TODO: show formatted json.
        'handler' => 'views_handler_field_computing_json',
      ),
    ),

    'outputjson' => array(
      'title' => t('Output JSON'),
      'help' => t('Output of the command in JSON'),
      'field' => array(
        'handler' => 'views_handler_field_computing_json',
      ),
    ),

    'id1' => array(
      'title' => t('Parameter id1'),
      'help' => t('Parameter id1'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),

    'id2' => array(
      'title' => t('Parameter id2'),
      'help' => t('Parameter id2'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),

    'id3' => array(
      'title' => t('Parameter id3'),
      'help' => t('Parameter id3'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),

    'id4' => array(
      'title' => t('Parameter id4'),
      'help' => t('Parameter id4'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),

    'number1' => array(
      'title' => t('Parameter number1'),
      'help' => t('Parameter number1'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),

    'number2' => array(
      'title' => t('Parameter number2'),
      'help' => t('Parameter number2'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),

    'number3' => array(
      'title' => t('Parameter number3'),
      'help' => t('Parameter number3'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),

    'number4' => array(
      'title' => t('Parameter number4'),
      'help' => t('Parameter number4'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),

    'string1' => array(
      'title' => t('Parameter string1'),
      'help' => t('Parameter string1'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),

    'string2' => array(
      'title' => t('Parameter string2'),
      'help' => t('Parameter string2'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),

    'string3' => array(
      'title' => t('Parameter string3'),
      'help' => t('Parameter string3'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),

    'string4' => array(
      'title' => t('Parameter string4'),
      'help' => t('Parameter string4'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),

    /*'dependency' => array(
      'title' => t('Dependency ID'),
      'help' => t('Dependent command ID, which should be executed before this command.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),*/

    'status' => array(
      'title' => t('Status'),
      'help' => t('Status of the command execution.'),
      'field' => array(
        'handler' => 'views_handler_field_computing_status',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_computing_status',
      ),
    ),

    // TODO: should show form items (clickable button etc), rather than a 4-char string.
    'control' => array(
      'title' => t('Control'),
      'help' => t('Control instructions of the command execution.'),
      'field' => array(
        'handler' => 'views_handler_field_computing_control',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_computing_control',
      ),
    ),

    'message' => array(
      'title' => t('Message'),
      'help' => t('Output message of this command.'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),

    'weight' => array(
      'title' => t('Weight'),
      'help' => t('Weight of this command; used for prioritize.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),

    'start' => array(
      'title' => t('Start timestamp'),
      'help' => t('The timestamp of this command starts to get executed.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
    ),

    'end' => array(
      'title' => t('End timestamp'),
      'help' => t('The timestamp of this command gets finished.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
    ),

    'updated' => array(
      'title' => t('Updated timestamp'),
      'help' => t('Some long-time processing command might periodically set this timestamp'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
    ),

    'progress' => array(
      'title' => t('Progress'),
      'help' => t('Progress of the execution, a 0 to 1 percentage number.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),

    // TODO/FIXME: add time spend computed field based on start and end.

  );
  return $data;
}


class views_handler_field_computing_status extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    $status_code = computing_code('status', TRUE);
    return isset($status_code[$value]) ? $status_code[$value] : $value;
  }
}


class views_handler_filter_computing_status extends views_handler_filter_in_operator {
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }
    $this->value_options = computing_code('status', TRUE);
  }
}

class views_handler_field_computing_control extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    $control_code = computing_code('control', TRUE);
    return isset($control_code[$value]) ? $control_code[$value] : $value;
  }
}


class views_handler_filter_computing_control extends views_handler_filter_in_operator {
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }
    $this->value_options = computing_code('control', TRUE);
  }
}


class views_handler_field_computing_json extends views_handler_field {
  // doesn't do anything for now.
  // TODO: look at views_handler_field_serialized or markup.
}