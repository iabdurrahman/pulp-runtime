#ifndef wifi_network_h
#define wifi_network_h

#include "IPAddress.h"

// WL_CONNECTED: assigned when connected to a WiFi network; 
// WL_NO_SHIELD: assigned when no WiFi shield is present; 
// WL_IDLE_STATUS: it is a temporary status assigned when WiFi.begin() is called and remains active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a connection is established (resulting in WL_CONNECTED); 
// WL_NO_SSID_AVAIL: assigned when no SSID are available; 
// WL_SCAN_COMPLETED: assigned when the scan networks is completed; 
// WL_CONNECT_FAILED: assigned when the connection fails for all the attempts; 
// WL_CONNECTION_LOST: assigned when the connection is lost; 
// WL_DISCONNECTED: assigned when disconnected from a network;

class WiFiClass : public IPAddress {
public :
    int begin(const char* ssid);
    int begin(const char* ssid, const char* pass);
    int begin(const char* ssid, int keyIndex, const char* key);
    void disconnect(void);
    void config(IPAddress ip);
    void config(IPAddress ip, IPAddress dns);
    void config(IPAddress ip, IPAddress dns, IPAddress gateaway);
    void config(IPAddress ip, IPAddress dns, IPAddress gateaway, IPAddress subnet);
    void setDNS(IPAddress dns_server1);
    void setDNS(IPAddress dns_server1, IPAddress dns_server2);
    string SSID(void);
    string SSID(int wifiAccessPoint);
    byte BSSID(uint8_t* bssid);
    long RSSI(void);
    long RSSI(const char* wifiAccessPoint);
    byte encryptionType(void);
    byte encryptionType(const char* wifiAccessPoint); //TKIP (WPA) = 2 WEP = 5 CCMP (WPA) = 4 NONE = 7 AUTO = 8
    byte scanNetworks(void);
    int status(void);
    int getSocket(void);
    byte macAddress(const char* mac);
};

extern WiFiClass WiFi;

#endif