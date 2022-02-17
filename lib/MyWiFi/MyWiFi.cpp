#include <Arduino.h>
#include <WiFi.h>

#include "MyWiFi.h"

MyWiFi::MyWiFi() {}

MyWiFi::~MyWiFi() {}

MyWiFi::MyWiFi(String ssid, String password) {
  this->ssid = ssid;
  this->password = password;
}

MyWiFi::MyWiFi(String ssid, String password, uint8_t addr_ssid, uint8_t addr_password) {
  this->ssid = ssid;
  this->password = password;
  this->addr_ssid = addr_ssid;
  this->addr_password = addr_password;
}

String MyWiFi::getSsid() {
  return this->ssid;
}

String MyWiFi::getPassword() {
  return this->password;
}

uint8_t MyWiFi::getAddrOfSsid() {
  return this->addr_ssid;
}

uint8_t MyWiFi::getAddrOfPassword() {
  return this->addr_password;
}

// int8_t MyWiFi::getRssi() {
//   return this->rssi;
// }

void MyWiFi::setSsid(String ssid) {
  this->ssid = ssid;
}

void MyWiFi::setPassword(String password) {
  this->password = password;
}

void MyWiFi::setAddrOfSsid(uint8_t addr_ssid) {
  this->addr_ssid = addr_ssid;
}

void MyWiFi::setAddrOfPassword(uint8_t addr_password) {
  this->addr_password = addr_password;
}

// void MyWiFi::setRssi(int32_t rssi) {
//   this->rssi = rssi;
// }

String MyWiFi::toString() {
  return "[" + String(this->addr_ssid) + "] = \"" + this->ssid + "\"\n" +
          "[" + String(this->addr_password) + "] = \"" + this->password + "\"\n";
}