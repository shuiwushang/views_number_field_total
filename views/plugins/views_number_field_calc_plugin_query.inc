<?php

class views_number_field_calc_plugin_query extends views_plugin_query_default {

  function get_aggregation_info() {
    $result = parent::get_aggregation_info();
    foreach ($result as $type => $info) {
      if (isset($info['handler']['filter'])) {
        $result[$type]['handler']['filter'] = 'views_number_field_calc_filter_group_by_numeric';
      }
    }
    return $result;
  }

}
