#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "pulp.h"
#include "wire.h"

static i2c_t *i2c;
static i2c_dev_t dev;

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxIndex = 0;
uint8_t TwoWire::rxLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txIndex = 0;
uint8_t TwoWire::txLength = 0;

uint8_t TwoWire::transmitting = 0;

TwoWire Wire;

void TwoWire::begin(void) {
    i2c_dev_init(&dev);

    dev.id = 0;
    dev.cs = 0xD0;
    dev.max_baudrate = 50000;

    i2c = i2c_open(&dev);

    rxIndex = 0;
    rxLength = 0;

    txIndex = 0;
    txLength = 0;
}

void TwoWire::begin(uint8_t address) {
    begin();
}

void TwoWire::end(void) {
    if(i2c){
        i2c_close(i2c);
        i2c = nullptr;
    }
}

int TwoWire::requestFrom(int address, int quantity) {
    return requestFrom((int)address, (int)quantity, (bool)true);
}

int TwoWire::requestFrom(int address, int quantity, bool stop) {
    if (i2c == nullptr) return 0;

    if (quantity > BUFFER_LENGTH) quantity = BUFFER_LENGTH;
    
    i2c->cs = address;
    i2c_read(i2c, rxBuffer, quantity, !stop);
    
    rxIndex = 0;
    rxLength = quantity;

    return quantity;
}

void TwoWire::beginTransmission(int address) {
    // indicate that we are transmitting
    transmitting = 1;
    // set address of targeted slave
    txAddress = address;
    // reset tx buffer iterator vars
    txIndex = 0;
    txLength = 0;
}

int TwoWire::endTransmission(void) {
    if (i2c){
        i2c->cs = txAddress;
        i2c_write(i2c, txBuffer, txLength);
        txIndex = 0;
        txLength = 0;
    }
    // reset tx buffer iterator vars
    
    // indicate that we are done transmitting
    transmitting = 0;
    return 0;
}

size_t TwoWire::write(uint8_t value) {
    if(transmitting){
    // in master transmitter mode
        // don't bother if buffer is full
        if(txLength >= BUFFER_LENGTH){
        return 0;
        }
        // put byte in tx buffer
        txBuffer[txIndex] = value;
        ++txIndex;
        // update amount in buffer   
        txLength = txIndex;
    }else{
    // in slave send mode
        // reply to master

        if (i2c) i2c_write(i2c, &value, 1);
    }
    return 1;    
}

size_t TwoWire::write(const char *str) {
    return -1;
}

size_t TwoWire::write(const String &myString) {
    return -1;
}

size_t TwoWire::write(const uint8_t *data, size_t length) {
    if(transmitting){
    // in master transmitter mode
        for(size_t i = 0; i < length; ++i){
        write(data[i]);
        }
    }else{
    // in slave send mode
        // reply to master
        if (i2c) i2c_write(i2c, (unsigned char*)data, length);
    }
    return length;
}

int TwoWire::available(void) {
    return rxLength - rxIndex;
}

int TwoWire::read(void) {
    int value = -1;
  
    // get each successive byte on each call
    if(rxIndex < rxLength){
        value = rxBuffer[rxIndex];
        ++rxIndex;
    }

    return value;
}

void TwoWire::setClock(uint32_t frequency) {

}

void TwoWire::onReceive(void (*function)(int)) {

}

void TwoWire::onRequest(void (*function)(int)) {

}

void TwoWire::setWireTimeout(uint32_t timeout, bool reset_on_timeout) {

}

void TwoWire::setWireTimeout(void) {

}

void TwoWire::clearWireTimeoutFlag(void) {

}

bool TwoWire::getWireTimeoutFlag(void) {
    return -1;
}