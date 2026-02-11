/**
    * this file is implementation of Arduino WiFi IP Address API
    * https://docs.arduino.cc/language-reference/en/functions/wifi/ipaddress/
*/

#ifndef IP_Address_h
#define IP_Address_h

#include <stdint.h> /* provides standard fixed-width integer*/

/**
    * class IPAddress
    * this class stores and manipulates 4-byte IP Addresses 
    */
class IPAddress {
private :
    uint8_t bytes[4]; /* internal buffer to hold 4-byte IP Address */

public :
    /**
        * default constructor
        * initializes IP 0.0.0.0
        */
    IPAddress(void);
    
    /**
        * parameterized constructor
        * @param byte1 first octet of the IP Address
        * @param byte2 second octet of the IP Address
        * @param byte3 third octet of the IP Address
        * @param byte4 fourth octet of the IP Address
        */
    IPAddress(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);

    ~IPAddress(void);

    /**
        * method localIP
        * gets the WiFi shield's IP Address
        * @return The IP Address of the shield
        * https://docs.arduino.cc/language-reference/en/functions/wifi/ipaddress/#ipaddresslocalip
        */
    IPAddress localIP();

    /**
        * method subnet
        * gets the WiFi shield's subnet mask
        * @return The subnet mask of the shield
        * https://docs.arduino.cc/language-reference/en/functions/wifi/ipaddress/#ipaddresssubnetmask
        */
    IPAddress subnet();

    /**
        * method gatewayIP
        * gets the WiFi shield's gateway IP Address
        * @return an array containing the shield's gateway IP Address
        * https://docs.arduino.cc/language-reference/en/functions/wifi/ipaddress/#ipaddressgatewayip
        */
    IPAddress gatewayIP();
};

#endif