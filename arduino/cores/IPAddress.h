#ifndef IP_Address_h
#define IP_Address_h

#include <stdint.h>

class IPAddress {
private :
    uint8_t bytes[4];

public :
    IPAddress();
    
    IPAddress(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);

    IPAddress localIP();
    IPAddress subnet();
    IPAddress gatewayIP();
};

#endif