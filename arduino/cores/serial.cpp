#include <stdint.h>
#include <stdbool.h>

#include "serial.h"
#include "pulp.h"
#include "uart.h"

Serials Serial(0);

Serials::Serials(int uart_id) {
    _uart_id = uart_id;
    _opened = false;
}

int Serials::availableForWrite() {
    return -1;
}

void Serials::begin(unsigned long baud) {
    uart_open(_uart_id, baud);
    _opened = true;
}

void Serials::begin(unsigned long baud, int config) {
    return -1;
}

void Serials::end(void) {
    uart_close(_uart_id);
    _opened = false;
}

int Serials::read() {
    uint8_t byte;
    int ret = uart_read(_uart_id, &byte, 1);
    return byte;
}

size_t Serials::write(uint8_t data) {
    uart_write(_uart_id, &data, 1);
    return 1;
}

int Serials::available() {
    return -1;
}

