#include "wifi_network.h"

WiFiClass WiFi;

int WiFiClass::begin(const char* ssid) {

}

int WiFiClass::begin(const char* ssid, const char* pass) {

}

int WiFiClass::begin(const char* ssid, int keyIndex, const char* key) {

}

void WiFiClass::disconnect(void) {

}

void WiFiClass::config(IPAddress ip) {

}

void WiFiClass::config(IPAddress ip, IPAddress dns) {

}

void WiFiClass::config(IPAddress ip, IPAddress dns, IPAddress gateaway) {

}

void WiFiClass::config(IPAddress ip, IPAddress dns, IPAddress gateaway, IPAddress subnet) {

}

void WiFiClass::setDNS(IPAddress dns_server1) {

}

void WiFiClass::setDNS(IPAddress dns_server1, IPAddress dns_server2) {

}

string WiFiClass::SSID(void) {

}

string WiFiClass::SSID(int wifiAccessPoint) {

}

byte WiFiClass::BSSID(uint8_t* bssid) {

}

long WiFiClass::RSSI(void) {

}

long WiFiClass::RSSI(const char* wifiAccessPoint) {

}

byte WiFiClass::encryptionType(void) {

}

byte WiFiClass::encryptionType(const char* wifiAccessPoint) {

}

byte WiFiClass::scanNetworks(void) {

}

int WiFiClass::status(void) {

}

int WiFiClass::getSocket(void) {

}

byte WiFiClass::macAddress(const char* mac) {

}