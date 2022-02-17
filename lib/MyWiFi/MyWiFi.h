#ifndef MyWiFi_h
#define MyWiFi_h
#endif

#include <Arduino.h>

class MyWiFi {
  public:
    MyWiFi();

    MyWiFi(String ssid, String password);

    MyWiFi(String ssid, String password, uint8_t addr_ssid, uint8_t addr_password);

    ~MyWiFi();  // Detructor no parameter

    String getSsid();

    String getPassword();

    uint8_t getAddrOfSsid();

    uint8_t getAddrOfPassword();

    int8_t getRssi();

    void setSsid(String ssid);

    void setPassword(String password);

    void setAddrOfSsid(uint8_t addr_ssid);
    
    void setAddrOfPassword(uint8_t addr_password);

    void setRssi(int32_t rssi);

    String toString();

  private:
    String ssid;
    String password;

    uint8_t addr_ssid;       
    uint8_t addr_password;

    // int32_t rssi;  // Recevied Strength Signal IndicatorS
};  