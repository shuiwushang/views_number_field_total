<?php

class views_number_field_total_filter_group_by_numeric extends views_handler_filter_group_by_numeric {

  function query() {

    if (views_number_field_detection($this->real_field)) {
      $info = $this->operators();

      $field = $this->options['selected_field'];
      $sql_code = $this->options['sql_code'];
      if (!empty($field) && !empty($info[$this->operator]['method'])) {
        if (trim($sql_code) != '') {
          $field = preg_replace('~\$field~', $field, $sql_code);
        }
        $this->{$info[$this->operator]['method']}($field);
      }
    }
    else {
      parent::query();
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    if (views_number_field_detection($this->real_field)) {
      $options['selected_field'] = array('default' => '');
      $options['sql_code'] = array('default' => '');
    }
    return $options;
  }

  function field_list($index = NULL) {
    $this->view->init_handlers();
    $this->view->init_query();
    $this->view->_build('field');

    $fields = array();
    foreach ($this->query->fields as $name => $field) {
      if (views_number_field_detection($name)) {
        $fields[$field['alias']] = $name;
      }
    }

    if ($index !== NULL) {
      return isset($fields[$index]);
    }
    return $fields;
  }

  function options_form(&$form, &$form_state) {
    if (views_number_field_detection($this->real_field)) {
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
    }
    parent::options_form($form, $form_state);
  }

}
