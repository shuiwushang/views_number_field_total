<?php

class views_number_field_total_handler_filter_numeric extends views_handler_filter_numeric{
  
  function query() {
   
    $this->view->_build('field');
    
    $info = $this->operators();

    $selected_field = $this->options['selected_field'];
    $sql_code = $this->options['sql_code'];

    if(isset($this->query->fields[$selected_field]) && $this->field_list($selected_field)) { 
      $field = $this->query->fields[$selected_field]['field'];
      if (!empty($info[$this->operator]['method'])) {
        if(trim($sql_code) != '') {
          $field = preg_replace('~\$field~', $field, $sql_code);
        }
        $this->{$info[$this->operator]['method']}($field);
      }
    }
  }
  
  function option_definition() {
    $options = parent::option_definition();
    $options['selected_field'] = array('default' => '');
    $options['sql_code'] = array('default' => '');
    return $options;
  }
  
  function field_list($index = NULL) {
    
    $this->view->init_handlers();      
    $this->view->init_query();
    $this->view->_build('field');
    
    $fields = array();
    foreach($this->query->fields as $name=>$field) {
      if(views_number_field_detection($name)) {
        $fields[$field['alias']] = $name;
      }
    }
    
    if($index !== NULL) {
      return isset($fields[$index]);
    }
    
    return $fields;
  }
  
  function options_form(&$form, &$form_state) {
    $form['selected_field'] = array(
      '#type' => 'select',
      '#title' => t('Select Field'),
      '#options' => $this->field_list(),
      '#default_value' => $this->options['selected_field'],
      '#size' => 1,
    );
    $form['sql_code'] = array(
      '#type' => 'textarea',
      '#title' => t('Sql Code'),
      '#default_value' => $this->options['sql_code'],
      '#description' => '$field is global',
      '#size' => 1,
    );
    parent::options_form($form, $form_state);
  }
  
  function op_simple($field) {
    $field_sql = implode($this->operator, array('('.$field.')', $this->value['value']));
    $this->query->add_where_expression($this->options['group'], $field_sql, array());
  }
  
  function op_between($field) {
    $field_sql = '('. $field .') BETWEEN '. $this->value['min'] .' AND '. $this->value['max'];
    $this->query->add_where_expression($this->options['group'], $field_sql, array());
    }

  function op_regex($field) {
    $field_sql = '('. $field .') RLIKE '. $this->value['value'];
    $this->query->add_where_expression($this->options['group'], $field_sql, array());
  }
  
}


/* End of file number_field_total_handler_filter_numeric.inc */   
