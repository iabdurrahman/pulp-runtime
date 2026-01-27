#ifndef wifi_client_h
#define wifi_client_h

#include <stdbool.h>
#include "IPAddress.h"
#include "stream.h"

class WiFiClient : public IPAddress, public Stream
{
public :
    bool connected(void);
    bool connect(IPAddress ip, int port);
    bool connect(String URL, int port);
    byte write(byte data);
    byte write(char data);
    byte print(char data);
    byte print(byte data);
    byte print(int data);
    byte print(long data);
    byte print(String data);
    byte print(char data, int BASE); //DEC for decimal (base 10), OCT for octal (base 8), HEX for hexadecimal (base 16).
    byte print(byte data, int BASE);
    byte print(int data, int BASE);
    byte print(long data, int BASE);
    byte print(String data, int BASE);
    byte println(void);
    byte println(char data);
    byte println(byte data);
    byte println(int data);
    byte println(long data);
    byte println(String data);
    byte println(char data, int BASE); //DEC for decimal (base 10), OCT for octal (base 8), HEX for hexadecimal (base 16).
    byte println(byte data, int BASE);
    byte println(int data, int BASE);
    byte println(long data, int BASE);
    byte println(String data, int BASE);
    void stop(void);
};

extern WiFiClient client;

#endif