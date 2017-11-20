// for windows
// var SERVER_URL = "http://localhost:8080/"
// for Pi
var SERVER_URL = "http://192.168.1.88:8080/"

/* use setTimeout to update status, otherwise several API calls may occurs simultaneously when it takes time in server side (especially for camera) */
sendStatus()
// setInterval(sendStatus, 200)
// setInterval(function() {
//     document.getElementById("img-camera").src = "static/00.jpg?" + (new Date() - 0);
//     console.log("aa");
// }, 500);

function sendStatus(num, onoff) {
    callApi(
        SERVER_URL + "setDeviceStatus",
        {
            "led_0":document.getElementById("checkbox-led-0").checked,
            "led_1":document.getElementById("checkbox-led-1").checked,
            "color_r":document.getElementById("range-led-r").value,
            "color_g":document.getElementById("range-led-g").value,
            "color_b":document.getElementById("range-led-b").value,
            "speaker":document.getElementById("range-speaker").value,
            "motor":document.getElementById("range-motor").value,
            // "servo":document.getElementById("range-servo").value,
            "text":document.getElementById("text-oled").value,
        },
        function (o) {
            getStatus()
        });
}

function getStatus() {
    callApi(
        SERVER_URL + "getDeviceStatus",
        {"prm1":"aaa", "prm2":"bbb"},
        function (o) {
            // console.log(o.responseText);
            var retJson = eval('new Object(' + o.responseText + ')');
            // console.log(retJson.ret)
            // document.getElementById("checkbox-led-0").checked = retJson.led_0 == "0" ? false : true;
            // document.getElementById("checkbox-led-1").checked = retJson.led_1 == "0" ? false : true;
            // document.getElementById("range-led-r").value = retJson.led_r
            // document.getElementById("range-led-g").value = retJson.led_g
            // document.getElementById("range-led-b").value = retJson.led_b
            // document.getElementById("range-speaker").value = retJson.speaker
            // document.getElementById("range-motor").value = retJson.motor
            // document.getElementById("range-servo").value = retJson.servo
            // document.getElementById("text-oled").value = retJson.oled_text

            if(retJson.btn_0 == "0"){
                document.getElementById("button-0").classList.remove('btn-primary');
                document.getElementById("button-0").classList.add('btn-secondary');
            } else {
                document.getElementById("button-0").classList.add('btn-primary');
                document.getElementById("button-0").classList.remove('btn-secondary');
            }
            if(retJson.btn_1 == "0"){
                document.getElementById("button-1").classList.remove('btn-primary');
                document.getElementById("button-1").classList.add('btn-secondary');
            } else {
                document.getElementById("button-1").classList.add('btn-primary');
                document.getElementById("button-1").classList.remove('btn-secondary');
            }
            document.getElementById("text-accel-x").textContent = Number(retJson.accel_x).toFixed(3);
            document.getElementById("text-accel-y").textContent = Number(retJson.accel_y).toFixed(3);
            document.getElementById("text-accel-z").textContent = Number(retJson.accel_z).toFixed(3);
            // document.getElementById("text-accel-tap").textContent = retJson.tap;

            document.getElementById("img-camera").src = SERVER_URL + "static/00.jpg?" + (new Date()).getTime();
            // document.getElementById("img-camera").src = "static/00.jpg";

            setTimeout(sendStatus, 10)
        });
}


function callApiTest() {
    console.log("callApiTest")
    callApi(
        SERVER_URL + "postTestJson",
        {"prm1":"aaa", "prm2":"bbb"},
        function (o) {
            console.log(o.responseText);
            var retJson = eval('new Object(' + o.responseText + ')');
            console.log(retJson.ret)
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
