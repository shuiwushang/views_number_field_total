<?php

/**
 * @Time       Nov 29, 2012 4:52:26 PM
 * @Filename   views_number_field_total_plugin_query
 * @Encoding   UTF-8
 * @author zhouitpro zhouitpro@gmail.com
 */

class views_number_field_total_plugin_query extends views_plugin_query_default {
  
  function get_aggregation_info() {
    $result = parent::get_aggregation_info();
    foreach($result as $type=>$info) {
      if(isset($info['handler']['filter'])) {
        $result[$type]['handler']['filter'] = 'views_number_field_total_filter_group_by_numeric';
      }
    }
    return $result;
  }
  
}
