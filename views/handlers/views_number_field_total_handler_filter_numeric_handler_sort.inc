<?php

class views_number_field_total_handler_filter_numeric_handler_sort extends views_handler_sort {
    
  function query() {
    
    $field = $this->options['selected_field'];
    
    if(!empty($field) && $this->field_list($field)) { 
      $order = $this->options['order'];
      $this->query->orderby[] = array(
        'field' => $field,
        'direction' => strtoupper($order)
      );
    }
  }
  
  function option_definition() {
    $options = parent::option_definition();
    $options['selected_field'] = array('default' => '');
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
    parent::options_form($form, $form_state);
  }
}


/* End of file number_field_total_handler_filter_numeric.inc */   