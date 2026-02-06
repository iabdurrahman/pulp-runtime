#include "variables.h"
#include "String.hpp"

#include "wifi_network.h"

WiFiClass WiFi;

int WiFiClass::begin(const char* ssid) {
    return -1;
}

int WiFiClass::begin(const char* ssid, const char* pass) {
    return -1;
}

int WiFiClass::begin(const char* ssid, int keyIndex, const char* key) {
    return -1;
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

// String WiFiClass::SSID(void) {
//     String SSID;
//     return SSID;
// }

// String WiFiClass::SSID(int wifiAccessPoint) {
//     String SSID;
//     return SSID;
// }

byte WiFiClass::BSSID(uint8_t* bssid) {
    return -1;
}

long WiFiClass::RSSI(void) {
    return -1;
}

long WiFiClass::RSSI(const char* wifiAccessPoint) {
    return -1;
}

byte WiFiClass::encryptionType(void) {
    return -1;
}

byte WiFiClass::encryptionType(const char* wifiAccessPoint) {
    return -1;
}

byte WiFiClass::scanNetworks(void) {
    return -1;
}

int WiFiClass::status(void) {
    return -1;
}

int WiFiClass::getSocket(void) {
    return -1;
}

byte WiFiClass::macAddress(const char* mac) {
    return -1;
}