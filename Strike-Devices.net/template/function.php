<?php
// Make theme available for translation
// Translations can be filed in the /languages/ directory
load_theme_textdomain( 'your-theme', TEMPLATEPATH . '/languages' );
 
$locale = get_locale();
$locale_file = TEMPLATEPATH . "/languages/$locale.php";
if ( is_readable($locale_file) )
 require_once($locale_file);
 
// Get the page number
function get_page_number() {
    if ( get_query_var('paged') ) {
        print ' | ' . __( 'Page ' , 'your-theme') . get_query_var('paged');
    }
}

function get_title() {
    if (is_single()) {
        single_post_title();
    }      
    elseif (is_home() || is_front_page()) {
        bloginfo('name');
        print ' | ';
        bloginfo('description');
        get_page_number();
    }
    elseif (is_page()) {
        single_post_title('');
    }
    elseif (is_search()) {
        bloginfo('name');
        print ' | Search results for ' . wp_specialchars($s);
        get_page_number();
    }
    elseif (is_404()) {
        bloginfo('name');
        print ' | Not Found';
    }
    else {
        bloginfo('name');
        wp_title('|');
        get_page_number();
    }
}

// Register widgetized areas
function theme_widgets_init() {
  // Area 1
  register_sidebar(array(
    'name' => 'Primary Widget Area',
    'id' => 'primary_widget_area',
    'before_widget' => '<li id="%1$s" class="widget-container %2$s">',
    'after_widget' => "</li>",
    'before_title' => '<h3 class="widget-title">',
    'after_title' => '</h3>',
  ) );
 
  // Area 2
  register_sidebar(array(
    'name' => 'Secondary Widget Area',
    'id' => 'secondary_widget_area',
    'before_widget' => '<li id="%1$s" class="widget-container %2$s">',
    'after_widget' => "</li>",
    'before_title' => '<h3 class="widget-title">',
    'after_title' => '</h3>',
  ));
}
 
add_action( 'init', 'theme_widgets_init' );

$preset_widgets = array(
  'primary_widget_area'  => array('search', 'pages', 'categories', 'archives'),
  'secondary_widget_area'  => array('links', 'meta'));

if (isset( $_GET['activated'])) {
 update_option('sidebars_widgets', $preset_widgets);
}
// update_option('sidebars_widgets', NULL);

// Check for static widgets in widget-ready areas
function is_sidebar_active($index) {
  global $wp_registered_sidebars;
  $widgetcolums = wp_get_sidebars_widgets();
   
  if ($widgetcolums[$index])
    return true;
  
  return false;
}
?>