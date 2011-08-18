/*
 * STRIKE-DEVICES.NET Design
 * =========================
 * Copyright 2011 by Michael Pohl
 */

/* cookie handling */

function createCookie(name, value, days) {
	if (days) {
		var date = new Date();
		date.setTime(date.getTime()+(days*24*60*60*1000));
		var expires = "; expires="+date.toGMTString();
	}
	else var expires = "";
	document.cookie = name+"="+value+expires+"; path=/";
}

function readCookie(name) {
	var nameEQ = name + "=";
	var ca = document.cookie.split(';');
	for(var i=0;i < ca.length;i++) {
		var c = ca[i];
		while (c.charAt(0)==' ') c = c.substring(1,c.length);
		if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
	}
	return null;
}

function eraseCookie(name) {
	createCookie(name,"",-1);
}

/* version switcher */

var viewportWidth = $(window).width();
var viewportHeight = $(window).height();

function orientationChange() {
  var temp = viewportWidth;
  viewportWidth = viewportHeight;
  viewportHeight = temp;
}

function initViewportWidth() {
  var deviceWidth = $(window).width();
  
  if(viewportWidth == deviceWidth)
    viewportWidth = null;
}

var version = readCookie("version");

if(version == null) {
  if(screen.width <= 480)
    version = "mobile";
  else
    version = "normal";
  
  createCookie("version", version, 1);
}

function activateSwitcher(id, switchFunction) {
  var switcher = $("#" + id);
  var text = switcher.text();
  switcher.replaceWith('<a href="" id="' + id + '" class="inactive-version">' +  text + '</a>');
  switcher = $("#" + id);
  switcher.click(switchFunction);
}

function set2normal() {
  if(viewportWidth != null)
    $("#viewport").attr("content", "width=" + viewportWidth);
    
  activateSwitcher("switch2mobile", switch2mobile);
  activateSwitcher("switch2print", switch2print);
}

function set2mobile() {
  if(viewportWidth != null)
    $("#viewport").attr("content", "width=device-width");
    
  var menus = $(".menu, #primary, #secondary");
  menus.hide();
  $("#stylesheet_mobile").attr("rel", "stylesheet");
  $("#stylesheet_mobile").removeAttr("media");
  menus.show();
  activateSwitcher("switch2normal", switch2normal);
  activateSwitcher("switch2print", switch2print);
}

function set2print() {  
  if(viewportWidth != null)
    $("#viewport").attr("content", "width=" + viewportWidth);
    
  $("#stylesheet_print").attr("rel", "stylesheet");
  $("#stylesheet_print").removeAttr("media");
  activateSwitcher("switch2mobile", switch2mobile);
  activateSwitcher("switch2normal", switch2normal);
  
  var previousVersion = readCookie("previous-version");
  
  if(previousVersion == null)
      eraseCookie("version");
  else {
      createCookie("version", previousVersion);
      eraseCookie("previous-version");
  }
}

function initViewControls() {
  $("#stylesheet_mobile").removeAttr("rel");
  $("#stylesheet_print").removeAttr("rel");

  if(version == "mobile")
    set2mobile();
  else if(version == "print")
    set2print();
  else
    set2normal();

  baseSize = parseFloat($("body").css("font-size"));
  
  if(relativeSize != 0)
    applySize();

  $("#view-controls").show();
}

function switch2mobile() {
  createCookie("version", "mobile", 1);
}

function switch2normal() {
  createCookie("version", "normal", 1);
}

function switch2print() {
  createCookie("previous-version", version, 1);
  createCookie("version", "print", 1);
}


/* size controls */

var baseSize = 0;
var relativeSize = readCookie("relativeSize");

if(relativeSize == null)
  relativeSize = 0;
else
  relativeSize = parseInt(relativeSize);

function applySize() {
  var currentSizeViewer = $("#size-current");
  
  currentSizeViewer.children("span").text((relativeSize>0?"+":"") + relativeSize);

  if(relativeSize == 0)
    currentSizeViewer.hide();
  else
    currentSizeViewer.show();
    
  var fontSize = 100 + relativeSize;
  $("body").css("font-size", fontSize + "%");
}

function sizeAdjust(delta) {
  if(delta == 0)
    relativeSize = 0;
  else
    relativeSize += delta;
  
  createCookie("relativeSize", relativeSize, 1);
  applySize();
}

function sizeDecrease() {
  sizeAdjust(-5);
}

function sizeReset() {
  sizeAdjust(0);
}

function sizeIncrease() {
  sizeAdjust(+5);
}


/* dynamic navigation menu */

function markCurrentRootNav() {
  var li = $(".menu").children("ul").children(".current_page_ancestor");
  
  if(!li.children("ul").hasClass("navExpanded"))
    li.children("a").addClass("currentNav");
}

function hasNavItemExpandedAncestor(children) {
  var parentChildren = children.parent("li").parent("ul");
  
  if(parentChildren.hasClass("navExpanded"))
    return true;
  else
    return false;
}

var positionIsInElement = false;

function isPositionInElement(x, y, element) {
  if(element.offset != undefined) {
    var offset = element.offset();
  
    if(offset != null) {
      var minX = offset.left;
      var minY = offset.top;
      var maxX = minX + element.outerWidth();
      var maxY = minY + element.outerHeight();

      if((x >= minX)
      && (x <= maxX)
      && (y >= minY)
      && (y <= maxY)) {
        positionIsInElement = true;
      }
    }
  }
}

function isPositionInElementOrDescendant(x, y, element) {
  positionIsInElement = false;
  isPositionInElement(x, y, element);
  
  if(positionIsInElement)
    return true;
  
  var descendants = element.find("*").each(function(index, element){
    isPositionInElement(x, y, $(this));
  });
  
  return positionIsInElement;
}

function changeExpansionState(children) {
  var button = children.parent().children("button");
  
  if(children.hasClass("navExpanded")) {
    children.removeClass("navExpanded");
    button.attr("title", navLabelExpand);
    button.html("+");
    children.hide();
  }
  else {
    children.addClass("navExpanded");
    button.attr("title", navLabelFold);
    button.html("-");
    children.show();
  }
}

function closeMenuOnLostFocus(event) {
  var x = event.pageX;
  var y = event.pageY;
  
  if(!isPositionInElementOrDescendant(x, y, $(".menu"))) {
    $(".navExpanded").each(function(index, element){
      element = $(element);
      changeExpansionState(element);
    });
    markCurrentRootNav();
  }
}

function toggleNavItem() {
  var button = $(this);
  var parent = button.parent();
  var link = parent.children("a");
  var oldChildren = $(".navExpanded");
  var children = parent.children("ul");
  var isExpanded = children.hasClass("navExpanded");

  if(oldChildren.index(children) != 0 && !hasNavItemExpandedAncestor(children)) {
    changeExpansionState(oldChildren);
  }
  
  if(link.hasClass("currentNav")) {
    children.find(".current_page_item")
    .parents(".current_page_ancestor")
    .children(".children")
    .each(function(){
      var descendantChild = $(this);
      
      if(!descendantChild.parent().children("a").hasClass("currentNav")) {
        changeExpansionState(descendantChild);
      }
    });

    link.removeClass("currentNav");
  }
  
  changeExpansionState(children);
  markCurrentRootNav();
}

function hideNavigation() {
  $("#hideNavigation").hide();
  $("#navigationContent").hide();
  $("#showNavigation").show();
}

function showNavigation() {
  $("#showNavigation").hide();
  $("#navigationContent").show();
  $("#hideNavigation").show();
}


/* touch support */

function setTouched() {
  $(this).removeClass("untouched");
  $(this).addClass("touched");
}

function setUntouched() {
  $(this).removeClass("touched");
  $(this).addClass("untouched");
}


/* initialization on document load */

function initMenu() {
  if(version == "normal") {
      var i = 0;
      var menu = $(".menu");
      
      menu.hide();
      var children = menu.find(".children");
      
      children.each(function(){
        var children = $(this);
        
        children.parent().children("a").after('<button title="' + navLabelExpand + '">+</button>');
        
        var button = children.parent().children("button");
        button.click(toggleNavItem);
      });

      markCurrentRootNav();
      $("body").click(closeMenuOnLostFocus);
      menu.show();
      
      children.each(function(){
        var child = $(this);
        child.css("display", "none");
      });
      
      menu.children("ul").children("li").children("ul").css("position", "absolute");
  }
  else if(version == "mobile") {
      hideNavigation();
      $("#navigationToggle").show();
  }
}

$(document).ready(function() {
  initViewControls();
  initMenu();
    
  window.scrollTo(0, 1);
    
  var touchTargets = $("#primary ul li a");
  touchTargets.bind("touchstart", setTouched);
  touchTargets.bind("touchend", setUntouched);
  touchTargets.bind("touchcancel", setUntouched);

  touchTargets = $("#secondary ul li a");
  touchTargets.bind("touchstart", setTouched);
  touchTargets.bind("touchend", setUntouched);
  touchTargets.bind("touchcancel", setUntouched);

  touchTargets = $(".menu ul li a");
  touchTargets.bind("touchstart", setTouched);
  touchTargets.bind("touchend", setUntouched);
  touchTargets.bind("touchcancel", setUntouched);

  touchTargets = $("#searchform input");
  touchTargets.bind("touchstart", setTouched);
  touchTargets.bind("touchend", setUntouched);
  touchTargets.bind("touchcancel", setUntouched);

  touchTargets = $("#view-controls button");
  touchTargets.bind("touchstart", setTouched);
  touchTargets.bind("touchend", setUntouched);
  touchTargets.bind("touchcancel", setUntouched);
});

