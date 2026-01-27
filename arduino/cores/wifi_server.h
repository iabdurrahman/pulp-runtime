#ifndef wifi_server_h
#define wifi_server_h

class WiFiServer {
public :
    WiFiServer(int port);

    void begin(void);
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
};

extern WiFiServer server;

#endif