<?php

/**
 * implements hook_help()
 * Megjeleniti a module help informaciokat
 * @param unknown_type $path
 * @param unknown_type $arg
 * @return string
 */
function fzguitar_help($path,$arg){
  $output = '';

  switch($path){
    case "admin/help#fzguitar":
      $output = '<h3>' .t('About'). '</h3>';
      $path = drupal_get_path('module', 'fzguitar') . '/fzguitar.info';
      $info = drupal_parse_info_file($path);
      $output .= 'Author: <a href="http://www.fzolee.hu">Zoltan Fabian</a><br/>';
      $output .= t('version').': '.$info['version'].'<br/>';
      $output .= t('compiled').': '.date('Y.m.d',$info['datestamp']);
      $output .="<p>".t("FZ Guitar shows a neck of guitar and frets.")."</p>";
      break;
    case "admin/help/fzguitar":
      break;
  }
  return $output;
}

/**
 *	implements hook_menu implementalasa
 */
function fzguitar_menu() {
  $items = array();
  //
  $items['admin/config/fz/fzguitar'] = array(
      'title' => t('FZ Guitar admin'),
      'description' => t('FZ Guitar settings control'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('fzguitar_settings'),
      'access arguments' => array('administer administration pages'),
      'weight' => 1
  );
  //Basic page
  $items['admin/config/fz/fzguitar/settings'] = array(
      'title' => t('Default'),
      'description' => t('FZ Guitar settings control default page'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('fzguitar_settings'),
      'access arguments' => array('administer administration pages'),
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => 2
  );

  //
  $items['admin/config/fz/fzguitar/pitch'] = array(
      'title' => t('Pitches'),
      'description' => t('Editing table of instruments and pitch'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('fzguitar_pitch'),
      'access arguments' => array('administer administration pages'),
      'type' => MENU_LOCAL_TASK,
      'weight' => 3
  );

  $items['admin/config/fz/fzguitar/pitch/record/load'] = array(
      'title' => t('Pitches'),
      'page arguments' => array('fzguitar_pitch_record_load'),
      'access arguments' => array('administer administration pages'),
      'type' =>MENU_CALLBACK
  );
  //
  $items['admin/config/fz/fzguitar/chords'] = array(
      'title' => t('Chords'),
      'description' => t('FZ Guitar,editing table of chords'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('fzguitar_chord'),
      'access arguments' => array('administer administration pages'),
      'type' => MENU_LOCAL_TASK,
      'weight' => 4
  );

  $items['admin/config/fz/fzguitar/chords/record/load'] = array(
      'title' => t('Pitches'),
      'page arguments' => array('fzguitar_chords_record_load'),
      'access arguments' => array('administer administration pages'),
      'type' =>MENU_CALLBACK
  );

  return $items;
}

function fzguitar_callback($pass =''){
  return true;
}
/**
 * hook_filter_info() implementation
 * Leirja, hogy milyen a szuro
 */
function fzguitar_filter_info(){
  $filters['fzguitar_filter'] =array(
      'title' => t('Guitar neck filter'),
      'description' => t('Substitutes a special image for the fzguitar tabs.'),
      'cache' => TRUE,
      'process callback' => 'fzguitar_substitute',
      'tips callback' => 'fzguitar_filter_tips',
  );
  return $filters;
}

/**
 * Admin interface for FZ Guitar
 */
function fzguitar_settings(){
  variable_set('menu_rebuild_needed', TRUE);
  $form = array();

  $form['fzguitar_chord'] = array(
      '#type'=>'checkbox',
      '#title' => t('Write out the name of chord'),
      '#default_value' => variable_get('fzguitar_chord',true),
      '#description' => t("Write out the name of chord to theneck of guitar")
  );
  $form['fzguitar_noteaut'] = array(
      '#type'=>'checkbox',
      '#title' => t('Calculate automatic the pitch on the strings and write out the neck'),
      '#default_value' => variable_get('fzguitar_noteaut',true),
      '#description' => t("Automatically write out the name of note to the neck of guitar")
  );
  $form['fzguitar_finger'] = array(
      '#type'=>'checkbox',
      '#title' => t('Write out the number of finger (1,2,3,4) or not'),
      '#default_value' => variable_get('fzguitar_finger',true),
      '#description' => t("Write out the number of finger or not")
  );
  $form['fzguitar_picking'] = array(
      '#type'=>'checkbox',
      '#title' => t('Write out the direction of picking: Up or down'),
      '#default_value' => variable_get('fzguitar_picking',true),
      '#description' => t("Write out the direction of picking: Up or downr")
  );

  //
  drupal_add_css('misc/farbtastic/farbtastic.css');
  drupal_add_js('misc/farbtastic/farbtastic.js');
  drupal_add_js(drupal_get_path('module','fzguitar').'/fzguitar.js');

  $form['fzguitar_rgb1'] = array(
      '#type'=>'textfield',
      '#title' => t('Set the default write color'),
      '#default_value' => variable_get('fzguitar_rgb1','#000080'),
      '#description' => t("With this color write out everything default to the neck")."<div id='color_rgb1'></div>",
  );

  $form['fzguitar_rgb2'] = array(
      '#type'=>'textfield',
      '#title' => t('Set the second write color'),
      '#default_value' => variable_get('fzguitar_rgb2','#800000'),
      '#description' => t("With this color write out the special things to the neck")."<div id='color_rgb2'></div><script>var fzguitar_admin_settings = true;</script>"
  );

  $form = system_settings_form($form);
  return $form;
}
