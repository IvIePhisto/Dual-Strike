<!DOCTYPE html >
<html xmlns="http://www.w3.org/1999/xhtml" <?php language_attributes(); ?>>
<head>
<meta http-equiv="content-type" content="<?php bloginfo('html_type'); ?>; charset=<?php bloginfo('charset'); ?>" />
<meta http-equiv="X-UA-Compatible" content="IE=Edge">
<title><?php get_title(); ?></title>
<link rel="stylesheet" type="text/css" href="<?php bloginfo('stylesheet_url'); ?>" />
<?php
if (is_singular())
  wp_enqueue_script('comment-reply');

wp_head();
?> 
<link rel="alternate" type="application/rss+xml" href="<?php bloginfo('rss2_url'); ?>" title="<?php printf( __( '%s latest posts', 'your-theme' ), wp_specialchars( get_bloginfo('name'), 1 ) ); ?>" />
<link rel="alternate" type="application/rss+xml" href="<?php bloginfo('comments_rss2_url') ?>" title="<?php printf( __( '%s latest comments', 'your-theme' ), wp_specialchars( get_bloginfo('name'), 1 ) ); ?>" />
<link rel="pingback" href="<?php bloginfo('pingback_url'); ?>" />

<link rel="stylesheet" href="blade_runner_font/stylesheet.css" type="text/css"/>
<link rel="stylesheet" href="css/default.css" type="text/css"/>
<link rel="stylesheet" href="css/mobile.css" type="text/css" media="screen and (max-device-width: 480px)" id="stylesheet_mobile"/>
<script src="js/jquery-1.4.4.min.js" type="text/javascript"></script>
<script src="js/default.js" type="text/javascript"></script>
<meta name="viewport" content="width=device-width; initial-scale=1" id="viewport"/>
<script type="text/javascript">initViewportWidth();</script>
</head>

<body id="body">
<div id="header"><div id="main-title"><a href="<?php bloginfo('url'); ?>/" rel="home"><?php bloginfo('name'); ?></a></div>

<div id="navigation-container">
<?php wp_page_menu( 'sort_column=menu_order' ); ?>

<?php if ( is_sidebar_active('primary_widget_area') ) : ?>
  <div id="primary" class="widget-area">
   <ul class="xoxo">
    <?php dynamic_sidebar('primary_widget_area'); ?>
   </ul>
  </div><!– #primary .widget-area –>
<?php endif; ?>  
 
<?php if ( is_sidebar_active('secondary_widget_area') ) : ?>
  <div id="secondary" class="widget-area">
   <ul class="xoxo">
    <?php dynamic_sidebar('secondary_widget_area'); ?>
   </ul>
  </div><!– #secondary .widget-area –>
<?php endif; ?>

<ul id="navigation">
    <li class="nav-item nav-item-main">
        <div class="nav-label nav-current"><span>Dual Strike</span></div>
        <ul>
            <li class="nav-item"><div class="nav-label"><a href="">Products</a></div></li>
            <li class="nav-item"><div class="nav-label"><a href="">Software &amp; Documentation</a><div></li>
        </ul>
    </li>
    <li class="nav-item nav-item-main">
      <div class="nav-label"><a href="">light Modding</a></div>
      <ul class="nav-inactive">
        <li class="nav-item">
          <div class="nav-label"><a href="">Sparky</a></div>
          <ul class="nav-inactive">
            <li class="nav-item">
              <div class="nav-label"><a href="">Jr.</a></div>
            </li>
            <li class="nav-item">
              <div class="nav-label"><a href="">PWM</a></div>
            </li>
          </ul>
        </li>
        <li class="nav-item">
          <div class="nav-label"><a href="">arc-Eyes</a></div>
        </li>
      </ul>
    </li>
</ul>
</div>

<div id="content-container">
<div id="content">
  <div id="javascript-warning" class="warning">
    <h2>JavaScript Disabled</h2>
    <p>This webpage uses JavaScript. Please enable it or use a browser that supports it to get full functionality.</p>
  </div>

  <?php global $wp_query; $total_pages = $wp_query->max_num_pages; if ($total_pages > 1) { ?>
  <div id="nav-above" class="navigation">
    <div class="nav-previous"><?php next_posts_link(__('<span class="meta-nav">&laquo;</span> Older posts', 'your-theme')) ?></div>
    <div class="nav-next"><?php previous_posts_link(__('Newer posts <span class="meta-nav">&raquo;</span>', 'your-theme')) ?></div>
  </div>
  <?php } ?>

  <?php while (have_posts()): the_post(); ?>
  <div id="post-<?php the_ID(); ?>" <?php post_class(); ?>>
    <h2 class="entry-title"><a href="<?php the_permalink(); ?>" rel="bookmark"><?php the_title(); ?></a></h2>
    <div class="entry-meta">
      <span class="meta-prep meta-prep-author"><?php _e('By ', 'your-theme'); ?></span>
      <span class="author vcard"><a class="url fn n" href="<?php echo get_author_link(false, $authordata->ID, $authordata->user_nicename); ?>" title="<?php printf( __('View all posts by %s', 'your-theme'), $authordata->display_name); ?>"><?php the_author(); ?></a></span>
      <span class="meta-sep"> | </span>
      <span class="meta-prep meta-prep-entry-date"><?php _e('Published ', 'your-theme'); ?></span>
      <span class="entry-date"><abbr class="published" title="<?php the_time('Y-m-d\TH:i:sO') ?>"><?php the_time(get_option('date_format')); ?></abbr></span>
      <?php edit_post_link( __('Edit', 'your-theme'), "<span class=\"meta-sep\">|</span>\n\t\t\t\t\t\t<span class=\"edit-link\">", "</span>\n\t\t\t\t\t" ) ?>
    </div>
 
    <div class="entry-content">
      <?php the_content( __('Continue reading <span class="meta-nav">&raquo;</span>', 'your-theme')); ?>
      <?php wp_link_pages('before=<div class="page-link">' . __('Pages:', 'your-theme') . '&after=</div>') ?>
    </div>
 
    <div class="entry-utility">
      <span class="cat-links"><span class="entry-utility-prep entry-utility-prep-cat-links"><?php _e('Posted in ', 'your-theme'); ?></span><?php echo get_the_category_list(', '); ?></span>
      <span class="meta-sep"> | </span>
      <?php the_tags('<span class="tag-links"><span class="entry-utility-prep entry-utility-prep-tag-links">' . __('Tagged ', 'your-theme') . '</span>', ", ", "</span>\n\t\t\t\t\t\t<span class=\"meta-sep\">|</span>\n") ?>
      <span class="comments-link"><?php comments_popup_link( __('Leave a comment', 'your-theme'), __('1 Comment', 'your-theme'), __('% Comments', 'your-theme')) ?></span>
      <?php edit_post_link( __('Edit', 'your-theme'), "<span class=\"meta-sep\">|</span>\n\t\t\t\t\t\t<span class=\"edit-link\">", "</span>\n\t\t\t\t\t\n") ?>
    </div>
  </div>
  <?php endwhile; ?>
  
  <?php global $wp_query; $total_pages = $wp_query->max_num_pages; if ($total_pages > 1) { ?>
  <div id="nav-below" class="navigation">
    <div class="nav-previous"><?php next_posts_link(__('<span class="meta-nav">&laquo;</span> Older posts', 'your-theme')) ?></div>
    <div class="nav-next"><?php previous_posts_link(__('Newer posts <span class="meta-nav">&raquo;</span>', 'your-theme')) ?></div>
  </div>
  <?php } ?>
</div>

<div id="footer">
<span>© Michael Pohl</span>
</div>
</body>
</html>