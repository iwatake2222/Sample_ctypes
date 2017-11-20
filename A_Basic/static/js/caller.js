// for windows
// var SERVER_URL = "http://localhost:8080/"
// for Pi
var SERVER_URL = "http://192.168.1.88:8080/"

setInterval(getButton, 200)

function setLed(num, onoff) {
    callApi(
        SERVER_URL + "setLed",
        {
            "num": num,
            "onoff": onoff
        },
        function (o) {
        });
}

function getButton() {
    callApi(
        SERVER_URL + "getButton",
        {"dummy":"dummy"},
        function (o) {
            console.log(o.responseText);
            var retJson = eval('new Object(' + o.responseText + ')');
            document.getElementById("checkbox-button-0").checked = retJson.btn_0 == "1";
            document.getElementById("checkbox-button-1").checked = retJson.btn_1 == "1";
        });
}

function callApi(url, jsonObj, callback) {
    var xhr = new XMLHttpRequest();
    xhr.open('POST', url);
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.setRequestHeader('Accept', 'application/json');

    xhr.onreadystatechange = (function(myxhr) {
        return function() {
            if (xhr.readyState == 4 && xhr.status == 200) {
                callback(myxhr);
            }
        }
    })(xhr);

    xhr.send(JSON.stringify(jsonObj));
}
