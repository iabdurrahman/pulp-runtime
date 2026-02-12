#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "pulp.h"

#include "serial.h"

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

}

void Serials::end(void) {
    uart_close(_uart_id);
    _opened = false;
}

int Serials::read() {
    uint8_t byte;
    uart_read(_uart_id, &byte, 1);
    return byte;
}

size_t Serials::write(uint8_t data) {
    uart_write(_uart_id, &data, 1);
    return 1;
}

size_t Serials::write(const uint8_t *data, size_t length) {
    uart_write(_uart_id, data, length);
    return length;
}

int Serials::available(void) {
    int periph = ARCHI_UDMA_UART_ID(_uart_id);
    int ret = plp_uart_rx_busy(periph);
    if (ret > 0 ) {
        return ret;
    } else {
        return -1;
    }
}

int Serials::peek(void) {
    return -1;
}

void Serials::flush(void) {

}
