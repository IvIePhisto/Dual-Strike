<?php
/**
 * Modifications for Strike-Devices.net by Michael Pohl 2011.
 */
 
/**
 * The Header for our theme.
 *
 * Displays all of the <head> section and everything up till <div id="main">
 *
 * @package WordPress
 * @subpackage Twenty_Ten
 * @since Twenty Ten 1.0
 */
?><!DOCTYPE html>
<html <?php language_attributes(); ?>>
<head>
<meta charset="<?php bloginfo( 'charset' ); ?>" />
<meta http-equiv="X-UA-Compatible" content="IE=Edge">
<title><?php
	/*
	 * Print the <title> tag based on what is being viewed.
	 */
	global $page, $paged;

	wp_title( '|', true, 'right' );

	// Add the blog name.
	bloginfo( 'name' );

	// Add the blog description for the home/front page.
	$site_description = get_bloginfo( 'description', 'display' );
	if ( $site_description && ( is_home() || is_front_page() ) )
		echo " | $site_description";

	// Add a page number if necessary:
	if ( $paged >= 2 || $page >= 2 )
		echo ' | ' . sprintf( __( 'Page %s', 'twentyten' ), max( $paged, $page ) );

	?></title>
<link rel="profile" href="http://gmpg.org/xfn/11" />
<link rel="stylesheet" type="text/css" media="all" href="<?php bloginfo( 'stylesheet_url' ); ?>" />
<link rel="pingback" href="<?php bloginfo( 'pingback_url' ); ?>" />
<link rel="stylesheet" href="<?php bloginfo('template_directory'); ?>/strike-devices.net/blade_runner_font/stylesheet.css" type="text/css"/>
<link rel="stylesheet" href="<?php bloginfo('template_directory'); ?>/strike-devices.net/mobile.css" type="text/css" media="handheld or screen and (max-device-width: 480px)" id="stylesheet_mobile"/>
<link rel="stylesheet" href="<?php bloginfo('template_directory'); ?>/strike-devices.net/print.css" type="text/css" media="print" id="stylesheet_print"/>
<style type="text/css" media="print">
#footer {
  display: none;
}
</style>
<script src="<?php bloginfo('template_directory'); ?>/strike-devices.net/jquery-1.5.1.min.js" type="text/javascript"></script>
<script type="text/javascript">
var navLabelExpand = "<?php esc_attr_e( 'expand', 'twentyten' ); ?>";
var navLabelFold = "<?php esc_attr_e( 'fold', 'twentyten' ); ?>";
</script>
<script src="<?php bloginfo('template_directory'); ?>/strike-devices.net/script.js" type="text/javascript"></script>
<meta name="viewport" content="width=device-width, initial-scale=1" id="viewport"/>
<script type="text/javascript">initViewportWidth();</script>
<!--[if lte IE 8]>
<script type="text/javascript" src="<?php bloginfo('template_directory'); ?>/strike-devices.net/css3pie-1.0beta3.js"></script>
<script type="text/javascript" src="<?php bloginfo('template_directory'); ?>/strike-devices.net/ie_lte_8.js"></script>
<![endif]-->
<?php
	/* We add some JavaScript to pages with the comment form
	 * to support sites with threaded comments (when in use).
	 */
	if ( is_singular() && get_option( 'thread_comments' ) )
		wp_enqueue_script( 'comment-reply' );

	/* Always have wp_head() just before the closing </head>
	 * tag of your theme, or you will break many plugins, which
	 * generally use this hook to add elements to <head> such
	 * as styles, scripts, and meta tags.
	 */
	wp_head();
?>
</head>

<body <?php body_class(); ?>>
<div id="wrapper" class="hfeed">
	<div id="header">
		<div id="masthead">
			<div id="branding" role="banner">
				<?php $heading_tag = ( is_home() || is_front_page() ) ? 'h1' : 'div'; ?>
				<<?php echo $heading_tag; ?> id="site-title">
					<span>
						<a href="<?php echo home_url( '/' ); ?>" title="<?php echo esc_attr( get_bloginfo( 'name', 'display' ) ); ?>" rel="home"><?php bloginfo( 'name' ); ?></a>
					</span>
				</<?php echo $heading_tag; ?>>
				<div id="site-description"><?php bloginfo( 'description' ); ?></div>

				<?php
					// Check if this is a post or page, if it has a thumbnail, and if it's a big one
					if ( is_singular() &&
							has_post_thumbnail( $post->ID ) &&
							( /* $src, $width, $height */ $image = wp_get_attachment_image_src( get_post_thumbnail_id( $post->ID ), 'post-thumbnail' ) ) &&
							$image[1] >= HEADER_IMAGE_WIDTH ) :
						// Houston, we have a new header image!
						echo get_the_post_thumbnail( $post->ID, 'post-thumbnail' );
					elseif ( get_header_image() ) : ?>
						<img src="<?php header_image(); ?>" width="<?php echo HEADER_IMAGE_WIDTH; ?>" height="<?php echo HEADER_IMAGE_HEIGHT; ?>" alt="" />
					<?php endif; ?>
			</div><!-- #branding -->

			<div id="access" role="navigation">
			  <span id="navigationToggle" style="display:none">
			    <button id="showNavigation" style="display:none" onclick="showNavigation()"><?php _e( 'show navigation', 'twentyten' ); ?></button>
			    <button id="hideNavigation" onclick="hideNavigation()"><?php _e( 'hide navigation', 'twentyten' ); ?></button>
			  </span>
			  <span id="navigationContent">
			  <?php /*  Allow screen readers / text browsers to skip the navigation menu and get right to the good stuff */ ?>
				<div class="skip-link screen-reader-text"><a href="#content" title="<?php esc_attr_e( 'Skip to content', 'twentyten' ); ?>"><?php _e( 'Skip to content', 'twentyten' ); ?></a></div>
				<?php /* Our navigation menu.  If one isn't filled out, wp_nav_menu falls back to wp_page_menu.  The menu assiged to the primary position is the one used.  If none is assigned, the menu with the lowest ID is used.  */ ?>
				<?php wp_nav_menu( array( 'container_class' => 'menu-header', 'theme_location' => 'primary' ) ); ?>
			  </span>
			</div><!-- #access -->
		</div><!-- #masthead -->
        <div id="header-clearer" style="clear: both"> </div>
	</div><!-- #header -->

	<div id="main">
