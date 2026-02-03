#ifndef serial_h
#define serial_h

#include <stddef.h>
#include <stdbool.h>
#include "stream.h"

class Serials : public Stream {
protected :
    bool _opened; //for if(Serial)
    int _uart_id;
public :
    Serials(int uart_id = 0);
    int availableForWrite(void);
    void begin(unsigned long baud = 115200);
    void begin(unsigned long baud = 115200, int config);
    void end(void);
    int read(void) override;
    size_t write(uint8_t data) override;
    int available(void) override;
    int peek(void) override;
    void flush(void) override;
};

extern Serials Serial;

#endif