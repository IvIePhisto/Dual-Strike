function attachPie() {
  PIE.attach(this);
}

$(function() {
    $("#header").each(attachPie);
    
    if(version != "print") {
        $("#primary").each(attachPie);
        $("#secondary").each(attachPie);
        $("#content").each(attachPie);
        $(".menu").children("ul").children("li").children("a").each(attachPie);
        $(".widget-container").each(attachPie);
        $("#primary a").each(attachPie);
        $("#secondary a").each(attachPie);
        $("#searchform div").each(attachPie);
    }
});