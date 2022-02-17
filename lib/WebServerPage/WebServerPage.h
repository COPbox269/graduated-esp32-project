#include <Arduino.h>

const char webserverPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en" dir="ltr">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>NodeMCU-32s WebServer</title>

    <style>
      h2 {
        text-align: center;
        margin: 25px 0px 0px 0px;  /* top right bottom left */
        -webkit-text-stroke: 1px #f50707;
        -webkit-text-fill-color: #f50707;
      }

      h3 {
        text-align: center;
        margin: 25px 0px 30px 0px;
        color: navy;
        -webkit-text-stroke: 1px #403e03;
        -webkit-text-fill-color: #403e03;
      }

      label {
        font-weight: bold;
        font-size: 18px;
      }

      .homeContent {
        text-align: center;
      }

      .homeButton {
        width: 275px;
        height: 40px;
        margin-bottom: 0px;
        color: navy;
        font-weight: bold;
        font-size: 15px;
      }

      .panelContent {
        border: darkblue solid 2px;
        background: lightblue;
        padding: 5px;
        height: auto;
        width: 325px;
        border-radius: 20px;
        margin-left: auto;
        margin-right: auto;
      }

      .wifiContent {
        margin-left: 30px;
        margin-right: 30px;
        margin-top: 20px;
      }

      .labelWiFi {
        font-size: 18px;
        color: navy;
      }

      .inputWiFi {
        font-size: 18px;
        width: 250px;
        height: 25px;
        margin-top: 5px;
        margin-bottom: 5px;
      }

      .btnWiFi {
        margin: 20px 10px 20px 10px; /* top right bottom left*/
        width: 85px;
        height: 40px;
        border-radius: 5px;
        font-size: 15px;
        font-weight: bold;
        outline: none;
        text-align: center;
      }

      #btnScan {
        background: #1569C7;
        color: white;
      }

      #btnConnect {
        background: #23C48E;
        color: white;
      }

      #btnChangeAP {
        background: #1569C7;
        color: white;
        width: 110px;
      }

      .parameterContent {
        float: left;
        width: 162px;
        height: auto;
        font-size: 18px;
      }

      .labelParameter {
        color: navy;
      }

      .voltageContent {
        margin-left: 20px;
        margin-bottom: 10px;
        text-align: left;
      }

      .powerContent {
        margin-left: 20px;
        text-align:left;
      }

      .currentContent {
        text-align: right;
        margin-right: 20px;
        margin-bottom: 10px;
      }

      .energyContent {
        text-align: right;
        margin-right: 20px;
      }

      .parameterValue {
         color: black;
         font-weight: bold;
         font-size: 18px;
      }

      .controlPanel {
        margin-top: 150px;
        text-align: center;
      }

      .relayStatus {
        outline: none;
        margin: 0px 5px 15px 5px;
        width: 60px;
        height: 60px;
        border-radius: 50%;
        color:black;
        font-size:15px;
        font-weight: bold;
        background: #23C48E;
        border: solid 2px white;
      }

      .relayButton {
        height: 65px;
        width: 60px;
        margin: 15px 5px 15px 10px; /*top right bottom left*/
        background-color: #1569C7;
        border-radius: 10px;
        /* border: solid 2px white; */
        outline:none;
        color:white;
        font-size:15px;
        font-weight: bold;
      }

      .backHome {
        margin-top: 25px;
        margin-bottom: 20px;

      }

      #btnBackHome {
        height: 35px;
        width: 270px;
        margin: 10px 5px 10px 5px; /*top right bottom left*/
        background-color: white;
        border-radius: 5px;
        outline: none;
        color: #f50707;
        font-size: 15px;
        font-weight: bold;
      }
    </style>
  </head>
  <body>
    <div class="homeContent">
      <div class="stickyContent">
        <h2 id="titleContent"></h2>
        <br>
      </div>
      <br>

      <div id="homePage">
        <br>
        <input type="button" class="homeButton" id="btnSetUpWiFiPanel" value="Set Up WiFi" onclick="openSetUpWiFiPanel()">
        <br><br>
        <input type="button" class="homeButton" id="btnDevicePanel" value="Device Panel" onclick="openDevicePanel()">
        <br><br>
      </div>
    </div>

    <div class="panelContent" id="functionPanel">
      <div class="panel">
        <div id="setUpWiFiPanel">
          <div class="wifiContent">
            <label for="nodemcu32sID" class="labelWiFi">ID<br/></label>
            <input type="text" class="inputWiFi" id="nodemcu32sID" minlength="8"/>

            <label for="ssid" class="labelWiFi">SSID<br /></label>
            <input type="text" list="ssidOfWiFi" class="inputWiFi" id="ssid" maxlength="32"/>
            <datalist id="ssidOfWiFi">
            </datalist>

            <label for="password" class="labelWiFi">PASSWORD<br /></label>
            <input type="text" class="inputWiFi" id="password" maxlength="64" minlength="8"/>
          </div>

          <div style="text-align: center;">
            <input type="button" class="btnWiFi" id="btnScan" value="Scan" onclick="scanWiFi()">
            <input type="button" class="btnWiFi" id="btnConnect" value="Connect" onclick="connectToWiFi()">
          </div>
        </div>

        <div id="devicePanel">
          <div class="parameterPanel">
            <h3>PARAMETER PANEL</h3>

            <div class="parameterContent">
              <div class="voltageContent">
                <label for="voltage" class="labelParameter">VOTAGE</label>
                <div class="parameterValue" id="voltage">0</div>
              </div>
              <div class="powerContent">
                <label for="power" class="labelParameter">POWER</label>
                <div class="parameterValue" id="power">0</div>
              </div>
            </div>

            <div class="parameterContent">
              <div class="currentContent">
                <label for="current" class="labelParameter">CURRENT</label>
                <div class="parameterValue" id="current">0</div>
              </div>
              <div class="energyContent">
                <label for="energy" class="labelParameter">ENERGY</label>
                <div class="parameterValue" id="energy">0</div>
              </div>
            </div>
          </div>

          <div class="controlPanel">
            <h3>CONTROL PANEL</h3>

            <div id="relayStatus">
              <input id="relayStatus1" class="relayStatus" type="button" value="ON"/>
              <input id="relayStatus2" class="relayStatus" type="button" value="ON"/>
              <input id="relayStatus3" class="relayStatus" type="button" value="ON"/>
              <input id="relayStatus4" class="relayStatus" type="button" value="ON"/>
            </div>

            <div id="relayButton">
              <input class="relayButton" type="button" id="relayButton1" value="RL1" onclick="getRelayButton(0)">
              <input class="relayButton" type="button" id="relayButton2" value="RL2" onclick="getRelayButton(1)">
              <input class="relayButton" type="button" id="relayButton3" value="RL3" onclick="getRelayButton(2)">
              <input class="relayButton" type="button" id="relayButton4" value="RL4" onclick="getRelayButton(3)">
            </div>
          </div>
        </div>
      </div>

      <div style="text-align: center;">
        <input type="button" id="btnBackHome" value="Back Home" onclick="backHome()">
      </div>
    </div>

  <script type="text/javascript">

    // Initial instance variables
    const xhttpRelayStatus = new XMLHttpRequest();

    var intervalReadDevice;

    var relay1;
    var relay2;
    var relay3;
    var relay4;

    // IDEA: Initial home page loading
    window.onload = onWindowLoading();
    function onWindowLoading()
    {
      document.getElementById("titleContent").innerHTML = "NODEMCU-32S HOMEPAGE";
      document.getElementById("homePage").style.display = "block";
      document.getElementById("setUpWiFiPanel").style.display = "none";
      document.getElementById("devicePanel").style.display = "none";
      document.getElementById("functionPanel").style.display = "none";
    }

    function openSetUpWiFiPanel() {
      document.getElementById("titleContent").innerHTML = "SET UP WIFI";
      document.getElementById("homePage").style.display = "none";
      document.getElementById("functionPanel").style.display = "block";
      document.getElementById("setUpWiFiPanel").style.display = "block";
    }

    function openDevicePanel() {
      document.getElementById("titleContent").innerHTML = "DEVICE PANEL";
      document.getElementById("homePage").style.display = "none";
      document.getElementById("functionPanel").style.display = "block";
      document.getElementById("devicePanel").style.display = "block";

      // IDEA: Sync all relay status
      getAllRelayStatus();

      // IDEA: Sync all sensor parameter
      getAllParameter();

      // IDEA: Update relay status and parameter of device every 3 seconds
      intervalReadDevice = setInterval(getDevicePanel, 3000);
    }

    function alertFunc() {
      alert("Hello!");
    }

    // Send request to get all relay status
    function getAllRelayStatus() {
      xhttpRelayStatus.open("GET", "/getAllStatus", true);
      xhttpRelayStatus.onreadystatechange = handleRelayStatus;
      xhttpRelayStatus.send();
    }

    // IDEA: Recognize which relay button is pressed and send request 4 relay status: ON/OFF
    function getRelayButton(pressedPosition) {
      switch (pressedPosition) {
        case 0:
          var relayStatus1 = document.getElementById("relayStatus1").value;

            if(relayStatus1 == "ON") {
              xhttpRelayStatus.open("GET","/relay1OFF", true);
            }
            else {
              xhttpRelayStatus.open("GET","/relay1ON", true);
            }

            // must be put in case (switch)
            xhttpRelayStatus.onreadystatechange = handleRelayStatus;
            xhttpRelayStatus.send();
          break;
        case 1:
          var relayStatus2 = document.getElementById("relayStatus2").value;
          if(relayStatus2 == "ON") {
            xhttpRelayStatus.open("GET","/relay2OFF", true);
          }
          else {
            xhttpRelayStatus.open("GET","/relay2ON", true);
          }

          xhttpRelayStatus.onreadystatechange = handleRelayStatus;
          xhttpRelayStatus.send();
          break;
        case 2:
          var relayStatus3 = document.getElementById("relayStatus3").value;
          if(relayStatus3 == "ON") {
            xhttpRelayStatus.open("GET","/relay3OFF", true);
          }
          else {
            xhttpRelayStatus.open("GET","/relay3ON", true);
          }

          xhttpRelayStatus.onreadystatechange = handleRelayStatus;
          xhttpRelayStatus.send();
          break;
        case 3:
          var relayStatus4 = document.getElementById("relayStatus4").value;
          if(relayStatus4 == "ON") {
            xhttpRelayStatus.open("GET","/relay4OFF", true);
          }
          else {
            xhttpRelayStatus.open("GET","/relay4ON", true);
          }

          xhttpRelayStatus.onreadystatechange = handleRelayStatus;
          xhttpRelayStatus.send();
          break;
      }
    }

    // IDEA: Get relay status response
    function handleRelayStatus() {
      if(xhttpRelayStatus.readyState == 4 && xhttpRelayStatus.status == 200) {
        var relayStatus = xhttpRelayStatus.responseText;

        const obj = JSON.parse(relayStatus);
        relay1 = obj.RELAY1;
        relay2 = obj.RELAY2;
        relay3 = obj.RELAY3;
        relay4 = obj.RELAY4;

        updateRelayStatus(relay1, relay2, relay3, relay4);
      }
    }

    // IDEA: Display relay status on panel
    function updateRelayStatus(RELAY1, RELAY2, RELAY3, RELAY4) {
      if(RELAY1 == "ON") {
        document.getElementById("relayStatus1").value = "ON";
        document.getElementById("relayStatus1").style.background = "#23C48E";
      }
      else {
        document.getElementById("relayStatus1").value = "OFF";
        document.getElementById("relayStatus1").style.background = "#cc2e29";
      }

      if(RELAY2 == "ON") {
        document.getElementById("relayStatus2").value = "ON";
        document.getElementById("relayStatus2").style.background = "#23C48E";
      }
      else {
        document.getElementById("relayStatus2").value = "OFF";
        document.getElementById("relayStatus2").style.background = "#cc2e29";
      }

      if(RELAY3 == "ON") {
        document.getElementById("relayStatus3").value = "ON";
        document.getElementById("relayStatus3").style.background = "#23C48E";
      }
      else {
        document.getElementById("relayStatus3").value = "OFF";
        document.getElementById("relayStatus3").style.background = "#cc2e29";
      }

      if(RELAY4 == "ON") {
        document.getElementById("relayStatus4").value = "ON";
        document.getElementById("relayStatus4").style.background = "#23C48E";
      }
      else {
        document.getElementById("relayStatus4").value = "OFF";
        document.getElementById("relayStatus4").style.background = "#cc2e29";
      }
    }

    // IDEA: Set Up WiFi
    // Connect to WiFi
    function connectToWiFi() {
      if(!isEmpty(document.getElementById("nodemcu32sID"), "Please enter ID of NodeMCU32s!") &&
        !isEmpty(document.getElementById("ssid"), "Please enter ssid!") &&
        !isEmpty(document.getElementById("password"), "Please enter password!")) {

        var id = document.getElementById("nodemcu32sID").value;
        var ssid = document.getElementById("ssid").value;
        var password = document.getElementById("password").value;

        alert("Connect to " + '"' + ssid + '"');

        const xhttp = new XMLHttpRequest;
        xhttp.onreadystatechange = function() {
          if(xhttp.readyState == 4 && xhttp.status == 200) {
            var result = xhttp.responseText;

            const obj = JSON.parse(result);

            // Notify result of wifi connection
            alert(obj.NOTIFICATION);
          }
        };

        // Open URL parameter with Javascript
        // To use webserver.arg() in nodemcu32s code
        xhttp.open("GET","/connectToWiFi?id=" + id + "&ssid="+ssid+"&password="+password,true);
        xhttp.send();
      }
    }

    // Scan SSID of arround WiFi
    function scanWiFi() {
      document.getElementById("ssidOfWiFi").innerHTML = "";
      document.getElementById("ssid").innerHTML = "";

      const xhttp = new XMLHttpRequest;

      // Handle response
      xhttp.onreadystatechange = function() {
        if(xhttp.readyState == 4 && xhttp.status == 200) {
          var result = xhttp.responseText;

          // Json array
          const obj = JSON.parse(result);

          // Convert Json array to Javascipt array
          var ssidOption = "";
          obj.forEach((item, i) => {
            ssidOption += '<option value="'+ item +'" />';
          });

          // Display ssid array to datalist
          document.getElementById("ssidOfWiFi").innerHTML = ssidOption;
        }
      };
      xhttp.open("GET", "/scanWiFi", true);
      xhttp.send();
    }

    // IDEA: Check empty
    function isEmpty(element, message) {
      if(element.value.trim() == "") {
        alert(message);
        element.focus();
        return true;
      }
      else {
        return false;
      }
    }

    function getDevicePanel() {
      getAllRelayStatus();

      getAllParameter();
    }

    function getAllParameter() {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if(xhttp.readyState == 4 && xhttp.status == 200) {
            var parameter = xhttp.responseText;

            const obj = JSON.parse(parameter);

            document.getElementById("voltage").innerHTML = obj.VOLTAGE;
            document.getElementById("current").innerHTML = obj.CURRENT;
            document.getElementById("power").innerHTML = obj.POWER;
            document.getElementById("energy").innerHTML = obj.ENERGY;
          }
        };
        xhttp.open("GET","/getSensorParameter",true);
        xhttp.send();
    }

    // IDEA: Back Home Button
    function backHome() {
      onWindowLoading();
    }
  </script>
  </body>
</html>

)=====";