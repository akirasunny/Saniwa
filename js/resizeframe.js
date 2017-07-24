function addJQuery(callback) {
    var script = document.createElement("script");
    script.setAttribute("src", "//ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js");
    script.addEventListener('load', function() {
        var script = document.createElement("script");
        script.textContent = "window.JQ=jQuery.noConflict(true);(" + callback.toString() + ")();";
        document.body.appendChild(script);
    }, false);
    document.body.appendChild(script);
}

function resizeFrameMain() {
    //get page url
    if (JQ(location).attr('href') == "http://pc-play.games.dmm.com/play/tohken/"){
        setTimeout(function(){
            JQ('#dmm-ntgnavi-renew').hide();
            JQ(".dmm-ntgnavi").hide();
            JQ(document.body).css({ 'overflow': 'hidden' });
            JQ('#game_frame').css({'position': 'fixed','left': '0','top': '0','width': '100%','height': '100%','z-index': '1000000'});
        },5000);
    }else if(JQ(location).attr('hostname') == "osapi.dmm.com"){
        setTimeout(function(){
            JQ(document.body).css({ 'overflow': 'hidden' });
            JQ('#flash_object').css({'position': 'fixed','left': '0','top': '0','width': '100%','height': '100%','z-index': '1000000'});
        },5000);
    }
}

// load jQuery and execute the main function
addJQuery(resizeFrameMain);