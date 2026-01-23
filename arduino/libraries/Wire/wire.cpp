#include <stdint.h>
#include <stdbool.h>

#include "wire.h"

TwoWire Wire;

void TwoWire::begin(void) {

}

void TwoWire::begin(uint8_t address) {

}

void TwoWire::end(void) {

}

int TwoWire::requestFrom(int address, int quantity) {

}

int TwoWire::requestFrom(int address, int quantity, bool stop) {

}

void TwoWire::beginTransmission(int address) {

}

int TwoWire::endTransmission(void) {

}

size_t TwoWIre::write(uint8_t value) {

}

size_t TwoWire::write(string) {

}

size_t TwoWire::write(uint8_t data, uint8_t length) {

}

int TwoWire::available(void) {

}

int TwoWire::read(void) {

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

}