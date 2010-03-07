<?php

/**
 * System settings form for Searchlight.
 */
function searchlight_settings_form($form_state) {
  $form = array();

  // Datasource views selection.
  // @TODO: Should this be by view_name:display rather trying to discover the
  // correct display later on?
  $form['searchlight_views'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Datasource views'),
    '#description' => t('Choose a view to use as the search configuration for each table.'),
  );
  $views = variable_get('searchlight_views', array());
  views_include('admin');
  foreach (views_fetch_base_tables() as $base_table => $info) {
    $options = drupal_map_assoc(array_keys(searchlight_get_usable_views($base_table, 'searchlight_datasource')));
    if (!empty($options)) {
      $options = array(0 => '<'. t('Disabled') .'>') + $options;
      $form['searchlight_views'][$base_table] = array(
        '#title' => $info['title'],
        '#description' => $info['description'],
        '#weight' => $info['weight'],
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => !empty($views[$base_table]) ? $views[$base_table] : 0,
      );
    }
  }

  // Backend selection.
  $form['backend'] = array(
    '#tree' => FALSE,
    '#type' => 'fieldset',
    '#title' => t('Backend'),
    '#description' => t('Choose a search backend to use with Searchlight.'),
  );
  $form['backend']['searchlight_backend'] = array(
    '#type' => 'select',
    '#options' => array(0 => '< '. t('Choose a backend') .' >'),
    '#default_value' => variable_get('searchlight_backend', 'sphinx'),
  );
  foreach (searchlight_registry('backend') as $key => $info) {
    $form['backend']['searchlight_backend']['#options'][$key] = $info['title'];
    $backend = searchlight_get_backend($key);
    $form['backend']["searchlight_backend_{$key}"] = $backend->settingsForm(variable_get("searchlight_backend_{$key}", array()));
    $form['backend']["searchlight_backend_{$key}"]['#tree'] = TRUE;
    $form['backend']["searchlight_backend_{$key}"]['#attributes'] = array(
      'class' => "searchlight-backend-{$key}",
    );
  }

  $form = system_settings_form($form);
  return $form;
}