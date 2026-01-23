#ifndef wire_h
#define wire_h

/*This library inherits from stream functions.
so, send() and receive() been replaced with read() and write()
This library implementation use a 32 byte buffer*/

#include <inttypes.h>
#include <stdbool.h>
#include "stream.h" //-> parent library for wire

#define buffer 32

#define WIRE_HAS_TIMEOUT // macro if setWireTimeout(), getWireTimeoutFlag(), and clearWireTimeout() are available

class TwoWire : public stream {
public:
    void begin(void);
    void begin(uint8_t address);
    void end(void);
    int requestFrom(int address, int quantity);
    int requestFrom(int address, int quantity, bool stop);
    void beginTransmission(int address);
    int endTransmission(void);
    size_t write(uint8_t value);
    size_t write(string);
    size_t write(uint8_t data, uint8_t length);
    int available(void);
    int read(void); // inherit from stream utility class
    void setClock(uint32_t frequency);
    void onReceive(void (*function)(int));
    void onRequest(void(*function)(int););
    void setWireTimeout(uint32_t timeout, bool reset_on_timeout);
    void setWireTimeout(void); //using default timeout
    void clearWireTimeoutFlag(void);
    bool getWireTimeoutFlag(void);
}

extern TwoWire Wire;

#endif