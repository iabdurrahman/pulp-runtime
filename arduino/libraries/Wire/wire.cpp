#include <stdint.h>
#include <stdbool.h>

#include "wire.h"

void Wire::begin(void) {

}

void Wire::begin(uint8_t address) {

}

void Wire::end(void) {

}

int Wire::requestFrom(int address, int quantity) {

}

int Wire::requestFrom(int address, int quantity, bool stop) {

}

void Wire::beginTransmission(int address) {

}

int Wire::endTransmission(void) {

}

size_t WIre::write(uint8_t value) {

}

size_t Wire::write(string) {

}

size_t Wire::write(uint8_t data, uint8_t length) {

}

int Wire::available(void) {

}

int Wire::read(void) {

}

void Wire::setClock(uint32_t frequency) {

}

void Wire::onReceive(void (*function)(int)) {

}

void Wire::onRequest(void (*function)(int)) {

}

void Wire::setWireTimeout(uint32_t timeout, bool reset_on_timeout) {

}

void Wire::setWireTimeout(void) {

}

void Wire::clearWireTimeoutFlag(void) {

}

bool Wire::getWireTimeoutFlag(void) {
    
}