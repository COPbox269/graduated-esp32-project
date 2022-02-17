#ifndef MyWiFiManagerManager_h
#define MyWiFiManager_h
#endif

#include <Arduino.h>

class MyWiFiManagerClass {
  public:
    MyWiFiManagerClass();

    ~MyWiFiManagerClass();  // Detructor no parameter

    String translateEncryptionType(wifi_auth_mode_t encryptionType);

    void scanNetwork(MyWiFi mywifiscanner[]); // doing...

    void saveWiFiToEEPROM(MyWiFi mywifi);

    MyWiFi readWiFiFromEEPROM(uint8_t addr_ssid, uint8_t addr_password);

    bool connectToNetwork(String ssid, String password); // doing...

    bool internetStatus = false;
};

extern MyWiFiManagerClass MyWiFiManager;
/* 
  Code below cause EROR when call in a funtion of other class
-> Fix by above code paragraph
*/
// #if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_MyWiFiManager)
// extern MyWiFiManagerClass MyWiFiManager;
// #endif