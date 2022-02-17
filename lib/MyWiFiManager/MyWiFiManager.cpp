/*
  scan network, save to wifi class
*/

#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>

#include "MyWiFi.h"
#include "MyWiFiManager.h"

MyWiFiManagerClass::MyWiFiManagerClass()
{
}

MyWiFiManagerClass::~MyWiFiManagerClass() {}

/*Security of networks*/
String MyWiFiManagerClass::translateEncryptionType(wifi_auth_mode_t encryptionType)
{
    switch (encryptionType)
    {
    case (WIFI_AUTH_OPEN):
        return "Open";
    case (WIFI_AUTH_WEP):
        return "WEP";
    case (WIFI_AUTH_WPA_PSK):
        return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK):
        return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK):
        return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE):
        return "WPA2_ENTERPRISE";
    }
    return "";
}

void MyWiFiManagerClass::scanNetwork(MyWiFi mywifi[])
{
    int numberOfNetwork = WiFi.scanNetworks();

    Serial.print("Number of networks found: ");
    Serial.println(numberOfNetwork);

    if (numberOfNetwork <= 0)
    {
        return;
    }
    else
    {
        for (int i = 0; i < numberOfNetwork; i++)
        {
            mywifi[i].setSsid(WiFi.SSID(i));

            Serial.printf("SSID[%d]: ", i);
            Serial.println(WiFi.SSID(i));

            Serial.printf("RSSI[%d]: ", i);
            Serial.println(WiFi.RSSI(i));

            Serial.printf("Mac adrress[%d]: ", i);
            Serial.println(WiFi.BSSIDstr(i));

            Serial.print("Encryption type: ");
            String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
            Serial.println(encryptionTypeDescription);
            Serial.println("-----------------------");
        }
    }
}

/*
  ssid[0] : 100 - 132
  password[0] : 133 - 200

  ssid[1] : 200 - 232
  password[1] : 233 - 300

  ssid[2] : 300 - 332
  password[2] : 333 - 400
*/
void MyWiFiManagerClass::saveWiFiToEEPROM(MyWiFi mywifi)
{
    // default addr of SSID: 10 -> 42
    // default addr of PASSWORD: 50 -> 114

    String ssid = mywifi.getSsid();
    String password = mywifi.getPassword();

    uint8_t sizeOfSsid = ssid.length();
    uint8_t sizeOfPassword = password.length();

    uint8_t addr_ssid = mywifi.getAddrOfSsid();
    uint8_t addr_password = mywifi.getAddrOfPassword();

    Serial.printf("Save SSID(addr: %d): \"", addr_ssid);
    for (int i = 0; i < sizeOfSsid; i++)
    {
        EEPROM.writeChar(addr_ssid + i, ssid[i]);
        Serial.print(ssid[i]);
    }
    Serial.println("\"");
    EEPROM.writeChar(addr_ssid + sizeOfSsid, '\0'); // Add termination null character to string ssid
    EEPROM.commit();

    Serial.printf("Save PASSWORD(addr: %d): \"", addr_password);
    for (int i = 0; i < sizeOfPassword; i++)
    {
        EEPROM.writeChar(addr_password + i, password[i]);
        Serial.print(password[i]);
    }
    Serial.println("\"");
    EEPROM.writeChar(addr_password + sizeOfPassword, '\0'); // Add termination null character to string ssid
    EEPROM.commit();
}

MyWiFi MyWiFiManagerClass::readWiFiFromEEPROM(uint8_t addr_ssid, uint8_t addr_password)
{
    // default addr of SSID: 10 -> 42
    // default addr of PASSWORD: 50 -> 114
    MyWiFi mWiFi;

    String ssid;
    String password;

    for (int i = addr_ssid; i <= addr_ssid + 32; i++)
    {
        char c = (char)EEPROM.readChar(i);

        if (c != '\0')
        {
            ssid.concat(c);
        }
        else
        {
            break;
        }
    }

    // delay(10);

    for (int i = addr_password; i <= addr_password + 64; i++)
    {
        char c = (char)EEPROM.readChar(i);

        if (c != '\0')
        {
            password.concat(c);
        }
        else
        {
            break;
        }
    }

    mWiFi.setSsid(ssid);
    mWiFi.setPassword(password);
    mWiFi.setAddrOfSsid(addr_ssid);
    mWiFi.setAddrOfPassword(addr_password);

    return mWiFi;
}

// Test wifi not password
bool MyWiFiManagerClass::connectToNetwork(String ssid, String password)
{
    // Set auto reconnection
    WiFi.setAutoReconnect(false);
    delay(50);
    WiFi.setAutoConnect(false);
    delay(50);

    // Make sure device dont connect to any network
    WiFi.disconnect();
    delay(100);

    // Connect to the network
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.println("Connecting to ");
    Serial.print("SSID: \"");
    Serial.print(ssid);
    Serial.println("\"");
    Serial.print("Password: \"");
    Serial.print(password);
    Serial.println("\"");

    // Wait for the Wi-Fi to connect (10000ms = 10s)
    int count = 0;
    while (count < 50)
    {
        if(WiFi.status() == WL_CONNECTED) {
            Serial.println('\n');
            Serial.println("Connection established!");  
            Serial.print("IP address:\t");
            Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer

            return true;
        }

        delay(200);
        Serial.print('.');

        count++;
    }

    Serial.println("");
    Serial.println("Timed out");
    return false;
}

MyWiFiManagerClass MyWiFiManager;
/* 
  Code below cause EROR when call in a funtion of other class
-> Fix by above code paragraph
*/
// #if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_MyWiFiManager)
// MyWiFiManagerClass MyWiFiManager;
// #endif