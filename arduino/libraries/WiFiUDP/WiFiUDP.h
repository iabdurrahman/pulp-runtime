#ifndef WIFIUDP_H
#define WIFIUDP_H

#include <stdint.h>
#include <stream.h>

class WiFiUDP : public Stream {
private:

public:
    uint8_t begin(int port);
    int beginPacket(const char *hostName, int port);
    int beginPacket(uint32_t hostIp, int port);
    int endPacket(void);
    size_t write(uint8_t byte);
    size_t write(const uint8_t *buffer, size_t size);
    int parsePacket(void);
    int read(void);
    int read(char *buffer, int len);
    void stop(void);
    uint32_t remoteIP(void);
    uint16_t remotePort(void);

    virtual int available(void);
    virtual int peek(void);
    virtual void flush(void);

};

#endif