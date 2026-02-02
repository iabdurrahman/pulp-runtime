// #include <stdio.h>
#include <stdint.h>

#include "pulp.h"
#include "SPI.h"

static spim_t *spim; //defined in pulp.h, spi section 
static spim_conf_t conf; //defined in pulp.h, spi section

SPIClass SPI;

SPIClass::current_type SPIClass::current = {1000000, MSBFIRST, SPI_MODE0};

void SPIClass::begin(void) {
    spim_conf_init(&conf);
    conf.id = 0;
    conf.cs = 0;
    conf.max_baudrate = 1000000;
    conf.polarity = 0;
    conf.phase = 0;

    spim = spim_open(&conf);
}

void SPIClass::beginTransaction(SPISettings s) {
    current.clock = s.clock;
    current.bitOrder = s.bitOrder;
    current.dataMode = s.dataMode;

    if(!spim) return;

    spim->max_baudrate = s.clock;
    int div = spi_get_div(spim->max_baudrate);
    spim->div = div;

    spim->polarity = (s.dataMode & 0x02)? 1 : (s.dataMode & 0x03)? 1 : 0;
    spim->phase = (s.dataMode & 0x01)? 1 : (s.dataMode & 0x03)? 1 : 0;

    spim->cfg = SPI_CMD_CFG(div, spim->polarity, spim->phase);
}

void SPIClass::endTransaction(void) {

}

void SPIClass::end(void) {
    if(spim) {
        spim_close(spim);
        spim = nullptr;
    }
}

void SPIClass::setBitOrder(uint8_t bitOrder) {
    current.bitOrder = bitOrder;
}

void SPIClass::setClockDivider(uint8_t divider) {
    current.clock = divider;
}

void SPIClass::setDataMode(uint8_t dataMode) {
    current.dataMode = dataMode;
}

void SPIClass::transfer(void *val) {
    if (!spim) return;
    return spim_transfer(spim, val, val, 8, SPIM_CS_KEEP);
}

void SPIClass::transfer16(void *val16) {
    
}

void SPIClass::transfer(void *buffer, size_t size) {
    if (!spim) return;
    return spim_transfer(spim, buffer, buffer, size*8, SPIM_CS_KEEP);
}

void SPIClass::usingInterrupt(int interruptNumber) {

}