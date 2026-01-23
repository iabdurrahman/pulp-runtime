#ifndef SPI_h
#define SPI_h

#include <stdint.h>

#define MSBFIRST 1
#define LSBFIRST 0

#define SPI_MODE0 0x00 // cpha & cpol = 0
#define SPI_MODE1 0x01 //cpha = 1
#define SPI_MODE2 0x02 //cpol = 1
#define SPI_MODE3 0x03 //cpha & cpol = 1

class SPISettings {
public:
    uint32_t clock;
    uint8_t bitOrder;
    uint8_t dataMode;

    SPISettings(uint32_t clk, uint8_t bitO, uint8_t mode) : clock(clk), bitOrder(bitO), dataMode(mode) {}
    SPISettings() : clock(1000000), bitOrder(MSBFIRST), dataMode(SPI_MODE0) {}
};

class SPIClass {
public:
    static void begin(void);
    static void beginTransaction(SPISettings settings);
    static void endTransaction(void);
    static void end(void);
    static void setBitOrder(uint8_t bitOrder);
    static void setClockDivider(uint8_t divider);
    static void setDataMode(uint8_t dataMode);
    static void transfer(void *val);
    static void transfer16(void *val16);
    static void transfer(void *buffer, size_t size);
    static void usingInterrupt(int interruptNumber);

private:
    struct current_type {
        uint32_t clock;
        uint8_t bitOrder;
        uint8_t dataMode;
    } static current;
};

extern SPIClass SPI;

#endif