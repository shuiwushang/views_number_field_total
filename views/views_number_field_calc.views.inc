<?php

/**
 * Implements hook_views_data_alter().
 */
function views_number_field_calc_views_data_alter(&$data) {
  $data['views'][VIEWS_NUMBER_FIELD_CALC_FIELD] = array(
    'title' => t('Number field calculate'),
    'help' => t('Number field calculate.'),
    'field' => array(
      'handler' => 'views_number_field_calc_handler',
      'click sortable' => TRUE,
    ),

    // Information for accepting a nid as a filter.
    'filter' => array(
      'handler' => 'views_number_field_calc_handler_filter_numeric',
    ),
    // sorting.
    'sort' => array(
      'handler' => 'views_number_field_calc_handler_filter_numeric_handler_sort',
    ),
  );
}

/**
 *  Override default query class.
 * 
  * Implements hook_views_plugins_alter().
  */
function views_number_field_calc_views_plugins_alter(&$result) {
  $path = drupal_get_path('module', 'views_number_field_calc');
  $result['query']['views_query']['path'] = $path . 'views/plugins/';
  $result['query']['views_query']['file']  = 'views_number_field_calc_plugin_query.inc';
  $result['query']['views_query']['handler'] = 'views_number_field_calc_plugin_query';
  $result['query']['views_query']['parent'] = 'views_plugin_query_default';
}
