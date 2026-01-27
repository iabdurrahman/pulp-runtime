#include "IPAddress.h"

IPAddress::IPAddress() 
{
    bytes[0] = 0;
    bytes[1] = 0;
    bytes[2] = 0;
    bytes[3] = 0;
}

IPAddress::IPAddress(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) 
{
    bytes[0] = byte1;
    bytes[1] = byte2;
    bytes[2] = byte3;
    bytes[3] = byte4;
}

IPAddress IPAddress::localIP() {

}

IPAddress IPAddress::subnet() {

}

IPAddress IPAddress::gatewayIP() {
    
}