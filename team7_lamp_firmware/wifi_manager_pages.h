#pragma once

const char* wm_rootPage = "<!DOCTYPE html> \
<title>WebLamp wifi setup</title> \
<link rel=\"stylesheet\" href=\"/style.css\"> \
<h1>WebLamp wifi setup</h1> \
<a href=\"/wifi\" class=\"nonline\"><div class=\"button\">Configure WiFi</div></a> \
<hr> \
<span class=\"subtext\">WebLamp by. Team7 wifi manager</span>";

const char* wm_wifiPage = "<!DOCTYPE html> \
<title>WebLamp wifi setup</title> \
<link rel=\"stylesheet\" href=\"/style.css\"> \
<h1>WebLamp wifi setup</h1> \
<ul> \
    {{wifilist}} \
</ul> \
<form action=\"/connect-confirm\"> \
<input class=\"input-text\" placeholder=\"SSID\" type=\"text\" id=\"ssid\" name=\"ssid\"> <br>\
<input class=\"input-text\" placeholder=\"password\" type=\"text\" id=\"password\" name=\"password\"> <br>\
<!--div class=\"checkbox-div\"><input type=\"checkbox\" id=\"scales\"><label class=\"checkbox-label unfocused-text\"> Show password</label> </div-->\
<div class=\"button\" onClick=\"this.parentNode.submit();\">Connect</div> \
<noscript><input type=\"submit\" value=\"Connect\"></noscript> \
</form> \
<hr> \
<span class=\"subtext\">WebLamp by. Team7 wifi manager</span> \
\
<script> \
    document.querySelectorAll(\".wifiname\").forEach(function (e) { \
        e.onclick = function () { \
            document.getElementById(\"ssid\").value = e.innerText; \
            document.getElementById(\"password\").focus(); \
        } \
    }) \
</script> ";

const char* wm_confirmPage = "<!DOCTYPE html> \
<title>WebLamp wifi setup</title> \
<link rel=\"stylesheet\" href=\"/style.css\"> \
<h1>WebLamp wifi setup</h1> \
<h3>Do you confirm this wifi data?</h1> \
<form action=\"/connect\"> \
<input class=\"input-text\" placeholder=\"SSID\" type=\"text\" id=\"ssid\" name=\"ssid\" value=\"{{ssid}}\" disabled> <br> \
<input class=\"input-text\" placeholder=\"password\" type=\"text\" id=\"password\" name=\"password\" value=\"{{password-safe}}\" disabled> <br>\
<a href=\"/connect?ssid={{ssid}}&password={{password}}\" class=\"nonline\"><div class=\"button\">Confirm</div></a> \
</form> \
<a href=\"/wifi\" class=\"nonline\"><div class=\"button\">Go back</div></a> \
";

const char* wm_connectPage = "<!DOCTYPE html> \
<title>WebLamp wifi setup</title> \
<style> \
{{integred-styles}} \
</style> \
<h1>WebLamp wifi setup</h1> \
<h3>Connecting is in progress, please wait</h1> \
<form action=\"/connect\"> \
<input class=\"input-text\" placeholder=\"SSID\" type=\"text\" id=\"ssid\" name=\"ssid\" value=\"{{ssid}}\" disabled> <br> \
<input class=\"input-text\" placeholder=\"password\" type=\"password\" id=\"password\" name=\"password\" value=\"{{password-safe}}\" disabled> <br>\
</form> \
";

const char* wm_stylePage = ".button \
{ \
    background-color: #2980B9; \
    cursor: pointer; \
    margin: auto; \
    margin-top: 1.5em; \
    color: white; \
    text-align: center; \
    max-width: 70%; \
    padding: 0.5em; \
    text-decoration: none; \
    font-size: 3em; \
    border-radius: 0.3em; \
    font-family: \"MS Sans Serif\", Geneva, sans-serif; \
} h1 { \
    font-size: 4em; \
    text-align: center; \
    color: white; \
    font-family: \"MS Sans Serif\", Geneva, sans-serif; \
} h3 { \
    font-size: 2em; \
    text-align: center; \
    color: white; \
    font-family: \"MS Sans Serif\", Geneva, sans-serif; \
} .wifiname { \
    color: #6060EE; \
    text-decoration: underline; \
    cursor: pointer; \
} a.nonline { \
    text-decoration: none; \
} hr { \
    margin-top: 6em; \
    color: #707070; \
} .subtext { \
    color: #707070; \
    font-size: 1em; \
} li { \
    margin: auto; \
    list-style-type: none; \
    text-align: center; \
    color: white; \
    font-size: 3em; \
    margin-top: 0.5em; \
} ol { \
    width: 100%; \
} .input-text { \
    display: block; \
    margin: auto; \
    font-size: 2.5em; \
    width: 60%; \
    padding: 0.2em; \
} .unfocused-text { \
    color: #909090; \
} .checkbox-label { \
    color: white; \
} .checkbox-div { \
    margin: auto; \
} body { \
    background: #272727; \
}" ;