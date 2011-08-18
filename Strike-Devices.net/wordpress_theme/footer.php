<?php
/**
 * The template for displaying the footer.
 *
 * Contains the closing of the id=main div and all content
 * after.  Calls sidebar-footer.php for bottom widgets.
 *
 * @package WordPress
 * @subpackage Twenty_Ten
 * @since Twenty Ten 1.0
 */
?>
	</div><!-- #main -->

	<div id="footer" role="contentinfo">
		<div id="colophon">
            <span id="view-controls" style="display:none">
                <span id="version-switcher">
                    <?php _e('Optimize for') ?>:
                    <span id="switch2normal"><?php _e('Desktop') ?></span> |
                    <span id="switch2mobile"><?php _e('Mobile') ?></span> |
                    <span id="switch2print"><?php _e('Print') ?></span>
                </span><!-- #version-switcher -->
                &#8211;
                <span id="size-control">
                    <?php _e('Zoom') ?><span id="size-current" style="display:none"> (<span>0</span> %)</span>:
                    <button id="size-decrease" title="<?php _e('decrease') ?>" onclick="sizeDecrease()">-</button> |
                    <button id="size-reset" title="<?php _e('reset') ?>" onclick="sizeReset()">0</button> |
                    <button id="size-increase" title="<?php _e('increase') ?>" onclick="sizeIncrease()">+</button>
                </span><!-- #size-control -->
            </span><!-- #view-controls -->
<?php
	/* A sidebar in the footer? Yep. You can can customize
	 * your footer with four columns of widgets.
	 */
	get_sidebar( 'footer' );
?>

			<div id="site-info">
				<a href="<?php echo home_url( '/' ) ?>" title="<?php echo esc_attr( get_bloginfo( 'name', 'display' ) ); ?>" rel="home">
					<?php bloginfo( 'name' ); ?>
				</a>
			</div><!-- #site-info -->

			<div id="site-generator">
				<?php do_action( 'twentyten_credits' ); ?>
				<a href="<?php echo esc_url( __('http://wordpress.org/', 'twentyten') ); ?>"
						title="<?php esc_attr_e('Semantic Personal Publishing Platform', 'twentyten'); ?>" rel="generator">
					<?php printf( __('Proudly powered by %s.', 'twentyten'), 'WordPress' ); ?>
				</a>
			</div><!-- #site-generator -->

		</div><!-- #colophon -->
	</div><!-- #footer -->

</div><!-- #wrapper -->

<?php
	/* Always have wp_footer() just before the closing </body>
	 * tag of your theme, or you will break many plugins, which
	 * generally use this hook to reference JavaScript files.
	 */

	wp_footer();
?>
</body>
</html>
