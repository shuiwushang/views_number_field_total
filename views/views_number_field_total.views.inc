<?php

/**
 * Implements Hook_views_data_alter
 */
function views_number_field_total_views_data_alter(&$data) {
  $data['views'][VIEWS_NUMBER_FIELD_TOTAL_FIELD] = array(
    'title' => t('Number Field Total'),
    'help' => t('Number Field Total.'),
    'field' => array(
      'handler' => 'views_number_field_total_handler',
      'click sortable' => TRUE,
    ),
    // Information for accepting a nid as a filter
    'filter' => array(
      'handler' => 'views_number_field_total_handler_filter_numeric',
    ),
    // Information for sorting on a nid.
    'sort' => array(
      'handler' => 'views_number_field_total_handler_filter_numeric_handler_sort',
    ),
  );
}

function views_number_field_total_views_plugins_alter(&$result) {
  $path = drupal_get_path('module', 'views_number_field_total');
  $result['query']['views_query']['path'] =  $path;
  $result['query']['views_query']['handler'] =  'views_number_field_total_plugin_query';
  $result['query']['views_query']['parent'] =  'views_plugin_query_default';
}