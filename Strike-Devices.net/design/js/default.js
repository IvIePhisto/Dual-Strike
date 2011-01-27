/* mobile/normal version switcher */

var viewportWidth = $(window).width();

function initViewportWidth() {
  var deviceWidth = $(window).width();
  
  if(viewportWidth == deviceWidth)
    viewportWidth = null;
}

function switch2mobile() {
  if(viewportWidth != null)
    $("#viewport").attr("content", "width=device-width");
    
  $("#navigation").hide();
  $("#stylesheet_mobile").attr("rel", "stylesheet");
  $("#navigation").show();
  $("#switch2mobile").hide();
  $("#switch2normal").show();
}

function switch2normal() {
  if(viewportWidth != null)
    $("#viewport").attr("content", "width=" + viewportWidth);
    
  $("#stylesheet_mobile").removeAttr("rel");
  $("#switch2normal").hide();
  $("#switch2mobile").show();
}

/* initialization on document load */
$(document).ready(function() {
  $("#javascript-warning").remove();
  $("#stylesheet_mobile").removeAttr("media");
  $("#footer").prepend(
    '<a id="switch2mobile" href="#body" class="version_switcher" style="display:none">switch to mobile version</span>' +
    '<a id="switch2normal" href="#body" class="version_switcher" style="display:none">switch to normal version</span>'
  );
  $("#switch2mobile").click(switch2mobile);
  $("#switch2normal").click(switch2normal);
  
  if(screen.width <= 480)
    switch2mobile();
  else
    switch2normal();

});

