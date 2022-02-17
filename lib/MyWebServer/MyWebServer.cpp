#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>  // This file is required to send request to web browser
#include <WiFiClient.h> // It handles all HTTP protocols

#include "MyWebServer.h"
#include "MyWiFi.h"
#include "MyWiFiManager.h"

#include "WebServerPage.h"

WebServer server(80);

String nodeMCU32sID = "123456789";
String ssid_connect = "";
String password_connect = "";

const String NULL_STRING = "";

MyWebServerClass::MyWebServerClass()
{
}

MyWebServerClass::MyWebServerClass(String ssid_ap, String password_ap)
{
    this->ssid_ap = ssid_ap;
    this->password_ap = password_ap;
}

MyWebServerClass::~MyWebServerClass() {}

/*
  24/01/2021: Eror: no instance of overloaded function "WebServer::on" matches the argument list -- argument types are: (const char [2], void ()) -- object type is: WebServer
                    invalid use of non-static member function 
                    at server.on() function
              -> change non-static function to static function
  Note: Need to declare WiFi mode in main.cpp
*/

void MyWebServerClass::handleClient()
{
    server.handleClient();
}

void MyWebServerClass::handleClient(unsigned long timeout) {
    unsigned long t = millis();
    while(1) {
        MyWebServer.handleClient();
        
        if(millis() - t > timeout) {
            break;
        }
    }
}

/**
 * Note: call server.handleClient() method onLoop or interrupt to retain webserver worker
 * @param ssid – Pointer to the SSID (max 63 char).
   @param passphrase – (for WPA2 min 8 char, for open use NULL)
   @param max_connection – Max simultaneous connected clients, 1 - 4
 */
void MyWebServerClass::startSoftAP(String ssid_ap, String password_ap, int max_connection) {
    // char ssid_ap_char[] = "YourSsidOfAcessPoint";
    // char password_ap_char[] = "YourPasswordOfAcessPoint";

    // ssid_ap.toCharArray(ssid_ap_char, ssid_ap.length() + 1); // '\0' in end of array char
    // password_ap.toCharArray(password_ap_char, password_ap.length() + 1);

    WiFi.softAP(ssid_ap.c_str(), password_ap.c_str(), 1, 0, max_connection);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
}

void MyWebServerClass::startWebServer(void handleRelay1ON(), void handleRelay1OFF(),
                                        void handleRelay2ON(), void handleRelay2OFF(),
                                        void handleRelay3ON(), void handleRelay3OFF(),
                                        void handleRelay4ON(), void handleRelay4OFF(),
                                        void handleRelayStatus(), void handleSensorParameter())
{
    server.on("/", handleRoot); // Call the 'handleRoot' function when a client requests URI "/"

    server.on("/relay1ON", handleRelay1ON);
    server.on("/relay1OFF", handleRelay1OFF);

    server.on("/relay2ON", handleRelay2ON);
    server.on("/relay2OFF", handleRelay2OFF);

    server.on("/relay3ON", handleRelay3ON);
    server.on("/relay3OFF", handleRelay3OFF);

    server.on("/relay4ON", handleRelay4ON);
    server.on("/relay4OFF", handleRelay4OFF);

    server.on("/getAllStatus", handleRelayStatus);

    server.on("/getSensorParameter", handleSensorParameter);

    server.on("/scanWiFi", handleWiFiScanner);

    server.on("/connectToWiFi", handleWiFiConnection); // Call the 'handleConnect' function when a POST request is made to URI "/connect"
    
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void MyWebServerClass::stop()
{
    server.stop();
    WiFi.softAPdisconnect();
}

void MyWebServerClass::handleRoot()
{
    Serial.println("GET /");

    String homePage = FPSTR(webserverPage); // Convert String to const char* and save it into SRAM
    server.send(200, "text/html", homePage);
}

void MyWebServerClass::handleWiFiScanner()
{
  Serial.println("GET /scan");
  
  String ssidJson;

  MyWiFi mwifiscanner[50];

  MyWiFiManager.scanNetwork(mwifiscanner);
  // int size = sizeof(mywifiscanner)/sizeof(mywifiscanner[0]);   // test to get size of object array
  
    ssidJson = "["; 
    for(int i = 0; i < 50; i++) {
        if(mwifiscanner[i].getSsid() != NULL) {
            ssidJson PROGMEM = ssidJson + "\"" + mwifiscanner[i].getSsid() + "\"" + ", ";
        }
        else {
            // Clear " and ,
            ssidJson.remove(ssidJson.length()-2, 2);
            ssidJson += "]"; 
            break;
        }
    }
    Serial.println(ssidJson);

    server.send(200, "application/json", ssidJson);
}

void MyWebServerClass::handleWiFiConnection()
{
    Serial.println("GET /connect");

    String id = server.arg("id");
    String ssidWeb = server.arg("ssid");
    String passwordWeb = server.arg("password");

    Serial.println(ssidWeb);
    Serial.println(passwordWeb);

    Serial.print("Length of SSID: ");
    Serial.println(ssidWeb.length());
    Serial.print("Length of password: ");
    Serial.println(passwordWeb.length());

    if (id == nodeMCU32sID)
    {
        //TODO: Right ID
        Serial.println("GET:");
        Serial.println(ssidWeb);
        Serial.println(passwordWeb);

        if(MyWiFiManager.connectToNetwork(ssidWeb, passwordWeb)) {
            Serial.println('\n');
            Serial.println("Connection established!");
            Serial.print("IP address:\t");
            Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer

            // Send wifi status notification and connected IP to Web
            String wifiStatus = "Success!";
            String connectedIP = String(WiFi.localIP());

            Serial.print("Connected IP: ");
            Serial.println(connectedIP);

            String json = "{\"NOTIFICATION\": \""+ wifiStatus +"\"," +
                            "\"IP\": \""+ connectedIP +"\"}";

            server.send(200, "application/json", json);

            // Save WiFi info in EEPROM
            MyWiFi mWiFi;
            mWiFi.setSsid(ssidWeb);
            mWiFi.setPassword(passwordWeb);
            mWiFi.setAddrOfSsid(10);
            mWiFi.setAddrOfPassword(50);
            MyWiFiManager.saveWiFiToEEPROM(mWiFi);

            // // Reset module
            // delay(50);
            ESP.restart();
        }
        else {
            ssid_connect = "";

            Serial.print("Change ssid to string:\"");
            Serial.print(ssid_connect);
            Serial.println("\"");

            String wifiStatus = "Failure!!!";

            String json = "{\"NOTIFICATION\": \""+ wifiStatus +"\"," +
                            "\"IP\": \""+ "" +"\"}";

            server.send(200, "application/json", json);
        }
    }
    else {
        //TODO: fail ID
        String wifiStatus = "Check ID again!!!";

        String json = "{\"NOTIFICATION\": \""+ wifiStatus +"\"," +
                            "\"IP\": \""+ "" +"\"}";

        server.send(200, "application/json", json);
    }
}

void MyWebServerClass::handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void MyWebServerClass::sendJsonResponse(String json) {
    server.send(200, "application/json", json);
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_MyWebServer)
MyWebServerClass MyWebServer;
#endif