<?php

class views_number_field_total_handler extends views_handler_field_numeric {

  function query() {
    
    //This line is needed for the aggregation settidgs in the query
    $params = $this->options['group_type'] != 'group' ? array('function' => $this->options['group_type']) : array();
    
    $fields = array(
      'field1' => $this->options['field1'],
      'field2' => $this->options['field2']
    );
    
    $custom_fields = array(
      'field1' => $this->options['custom_number_field1'],
      'field2' => $this->options['custom_number_field2']
    );
    
    foreach($custom_fields as $key=>$field) {
      if(!empty($field) && is_numeric($field)) {
        $fields[$key] = $field;
      }
    }
    
    $operation = $this->options['operation'];
    
    if(!$this->formula_options_list($operation)) {
      $operation = '+';
    }
    
    $fieldsql = array();
    
    static $save_field = array();

    foreach($fields as $key=>$field) {
      
      if(is_numeric($field)) {
        $fieldsql[$key] = $field;
        continue;
      }
      
      //split field to array
      $field_array  = explode('###', $field);
      $field = $field_array[0];
      $field_group = isset($field_array[1]) ? $field_array[1] : FALSE;
      unset($field_array);

      if(!isset($this->view->field[$field])) continue;
      
      $field_info = $this->view->field[$field];
      
      if($field_info->table == 'views' && views_number_field_detection($field)) {
        if(isset($this->query->fields[$field])) {
          $fieldsql[$key] = $this->_string_brackets($this->query->fields[$field]['field']);
        }
        continue;
      }
      $field_info->set_relationship();
      //Force the tables needed for the fields in the query to be added.
      // In particular, the joins for any fields.
      $field_info->ensure_my_table();
      
      //get Table
      $table_string = (!empty($field_info->table_alias) && $field_info->table_alias !== 'unknown') ? $field_info->table_alias: $field_info->table;
      
      $field_string = ($field_group !== FALSE) ? $field_info->field .'_'. $field_group : $field_info->real_field;
      
      $fieldsql[$key] = implode('.', array($table_string,$field_string));
    }
    
    if (isset($fieldsql['field1']) && isset($fieldsql['field2'])) {
      $formula = implode($operation, $fieldsql);
      //TODO: dynamically create a field alias

      $field_key = implode('', $fields);
      $field_key = preg_replace('~[\.\+\-\*]*~', '', $field_key);
      
      //Generates the largest number to field alias
      $field_alias = VIEWS_NUMBER_FIELD_TOTAL_FIELD;
      
      //Prevent duplicate execution
      if(!isset($save_field[$field_key])){
        $save_field[$field_key] = TRUE;
        $i = 1;
        do{
          if(isset($this->query->fields[$field_alias])) {
            $field_alias = VIEWS_NUMBER_FIELD_TOTAL_FIELD .'_'. $i;
          }else {
            break;
          }
          $i++;
        }while(isset($this->query->fields[$field_alias]));
      }
      
      //add field
      $this->field_alias = $this->view->query->add_field(NULL, $formula, $field_alias, $params);
      $this->add_additional_fields($formula);
    }
  }
  
  /**
   * @access public
   * @param String $name
   * @return String
   */
  function _string_brackets($name) {
    return '(' .$name. ')';
  }
  
  /**
   * @access public
   * @return array
   */
  function get_select_number_field() {
    $fields = array();
    
    $fields_array = $this->view->display_handler->get_handlers('field');
    //TODO: This part is very weak:
    //We need to find a proper way to scan data types for fields.
    foreach ($fields_array as $name => $field) {
      
      if($field instanceof views_handler_field_math) {
        continue;
      }
      
      if (get_class($field)  == 'views_handler_field_numeric') {
        $fields[$name] = $field->ui_name() . $this->_string_brackets($name);
        continue;
      }
     
      if($field instanceof views_handler_field_field) {
        if($field->field_info['type'] == 'number_integer') {
          $fields[$name] = $field->ui_name() . $this->_string_brackets($name);
          continue;
        }
      }

      if(get_class($field) == 'views_handler_field_field') {
        if(isset($field->field_info['columns']) && !empty($field->field_info['columns'])) {
          foreach($field->field_info['columns'] as $key=>$column) {
            if($column['type'] == 'int' OR $column['type'] == 'numeric') {
              $fields[$name . '###'. $key] = $field->ui_name() . $this->_string_brackets($key);
              continue;
            }
          } 
        }
      }
      if(views_number_field_detection($name)) {
        $fields[$name] = $field->ui_name() . $this->_string_brackets($name);
      }
    }
    
    $fields[VIEWS_NUMBER_FIELD_TOTAL_FIELD .'_custom_number'] = 'Custom number';
    
    return $fields;
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['field1'] = array('default' => 'none', 'translatable' => TRUE);
    $options['field2'] = array('default' => 'none', 'translatable' => TRUE);
    $options['custom_number_field1'] = array('default' => '', 'translatable' => TRUE);
    $options['custom_number_field2'] = array('default' => '', 'translatable' => TRUE);
    $options['operation'] = array('default' => 'none', 'translatable' => TRUE);
    $options['php_value'] = array('default' => '');
    return $options;
  }
  
  function formula_options_list($index = NULL) {
    
    static $options = array();
    
    if(empty($options)) {
      $options = array(
        '+' => t('+'),
        '-' => t('-'),
        '*' => t('x'),
        '/' => t('÷')
      );

      drupal_alter('views_field_formula', &$options);
    }
    
    if($index !== NULL) {
      return isset($options[$index]);
    }
    
    return $options;
  }
  
  function options_form(&$form, &$form_state) {
    $form['field1'] = array(
      '#type' => 'select',
      '#title' => t('field1'),
      '#options' => $this->get_select_number_field(),
      '#default_value' => $this->options['field1'],
      '#size' => 1,
    );
    
    $form['custom_number_field1'] = array(
       '#type' => 'textfield',
       '#title' => 'Custom Number',
       '#size' => 15,
       '#default_value' => $this->options['custom_number_field1'],
       '#states' => array(
         //Hide the modes when the content type value is .
         //'visible' => FALSE,
         'visible' => array(
          'select[name="options[field1]"]' => array('value' => VIEWS_NUMBER_FIELD_TOTAL_FIELD .'_custom_number'),
         ),
       ),
     );
    
    $form['operation'] = array(
      '#type' => 'select',
      '#title' => t('Operating'),
      '#options' => $this->formula_options_list(),
      '#default_value' => $this->options['operation'],
      '#description' => t('Operating.'),
    );
    
    $form['field2'] = array(
      '#type' => 'select',
      '#title' => t('field2'),
      '#options' => $this->get_select_number_field(),
      '#default_value' => $this->options['field2'],
      '#size' => 1,
    );
    
    $form['custom_number_field2'] = array(
       '#type' => 'textfield',
       '#title' => 'Custom Number',
       '#size' => 15,
       '#default_value' => $this->options['custom_number_field2'],
       '#states' => array(
         //Hide the modes when the content type value is .
         //'visible' => FALSE,
         'visible' => array(
          'select[name="options[field2]"]' => array('value' => VIEWS_NUMBER_FIELD_TOTAL_FIELD .'_custom_number'),
         ),
       ),
     );
    
    $form['php_value'] = array(
      '#type' => 'textarea',
      '#title' => t('Value Code'),
      '#default_value' => $this->options['php_value'],
      '#description' => t('Code to construct the value of this field. Do not use <?php ?> delimiters. $value is value'),
    );

    parent::options_form($form, $form_state);
  }
  
  
  function render($values) {
    $value = $values->{$this->field_alias};

    if(trim($this->options['php_value']) != '') { 
      $value = eval('return '. $this->options['php_value'] .';');
    }
    
    $values->{$this->field_alias} =  $value;
    return parent::render($values);
  }
}
