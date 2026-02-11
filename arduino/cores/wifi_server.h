/**
    * this file is implementation of Arduino WiFi Client API
    * https://docs.arduino.cc/language-reference/en/functions/wifi/server/
*/

#ifndef wifi_server_h
#define wifi_server_h

#include "variables.h"
#include "String.hpp"

#include "wifi_client.h" /* provide WiFiClient object for return data type available method */

/**
    * class WiFiServer
    */
class WiFiServer
{
public :
    /**
        * public class method
        */

    /**
        * default constructor
        */
    WiFiServer(void);

    /**
        * parameterized constructor
        * @param port The port to listen on
        * https://docs.arduino.cc/language-reference/en/functions/wifi/server/#wifiserver
        */
    WiFiServer(int port);

    /**
        * destructor 
        */
    ~WiFiServer(void);

    /**
        * method begin
        * tells the server to begin listening for incoming connections
        * https://docs.arduino.cc/language-reference/en/functions/wifi/server/#serverbegin
        */
    void begin(void);

    /**
        * method write
        * write data to all the clients connected to a server
        * @param data The value to write
        * @return The number of bytes written
        * https://docs.arduino.cc/language-reference/en/functions/wifi/server/#serverwrite
        */
    byte write(byte data);
    byte write(char data);

    /**
        * method print
        * print data to all the clients connected to a server
        * @param data The data to print
        * @param BASE The base in which to print numbers
        * @return The number of bytes written, though reading that number is optional
        * https://docs.arduino.cc/language-reference/en/functions/wifi/server/#serverprint
        */
    byte print(char data);
    byte print(byte data);
    byte print(int data);
    byte print(long data);
    byte print(String data);
    byte print(char data, int BASE);
    byte print(byte data, int BASE);
    byte print(int data, int BASE);
    byte print(long data, int BASE);
    byte print(String data, int BASE);

    /**
        * method println
        * prints data, followed by a newline, to all the clients connected to a server
        * @param data The data to print
        * @param BASE The base in which to print numbers
        * @return The number of bytes written, though reading that number is optional
        * https://docs.arduino.cc/language-reference/en/functions/wifi/server/#serverprintln
        */
    byte println(void);
    byte println(char data);
    byte println(byte data);
    byte println(int data);
    byte println(long data);
    byte println(String data);
    byte println(char data, int BASE);
    byte println(byte data, int BASE);
    byte println(int data, int BASE);
    byte println(long data, int BASE);
    byte println(String data, int BASE);

    /**
        * gets a client that is connected to the server and has data available for reading
        * the connection persists when the returned client object goes out of scope
        * closed by calling client.stop()
        * @return a client object (this object will evaluate to false in an if-statement if no Client has data available for reading)
        * https://docs.arduino.cc/language-reference/en/functions/wifi/server/#serveravailable
        */
    WiFiClient available(void);
};

/**
    * Pre-instantiated object for global access to the WiFiServer
    */
extern WiFiServer server;

#endif