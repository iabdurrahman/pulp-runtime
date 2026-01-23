// #include <stdio.h>
// #include <stdint.h>
// #include "pulp.h"

#include "SPI.h"

SPIClass SPI;

SPIClass::current_type SPIClass::current = {1000000, MSBFIRST, SPI_MODE0};

void SPIClass::begin(void) {

}

void SPIClass::beginTransaction(SPISettings s) {
    current.clock = s.clock;
    current.bitOrder = s.bitOrder;
    current.dataMode = s.dataMode;
}

void SPIClass::endTransaction(void) {

}

void SPIClass::end(void) {

}

void SPIClass::setBitOrder(uint8_t bitOrder) {
    current.bitOrder = bitOrder;
}

void SPIClass::setClockDivider(uint8_t divider) {
    
}

void SPIClass::setDataMode(uint8_t dataMode) {
    current.dataMode = dataMode;
}

void SPIClass::transfer(void *val) {

}

void SPIClass::transfer16(void *val16) {
    
}

void SPIClass::transfer(void *buffer, size_t size) {

}

void SPIClass::usingInterrupt(int interruptNumber) {

}