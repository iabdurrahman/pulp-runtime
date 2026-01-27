#ifndef serial_h
#define serial_h

#include <stddef.h>
#include <stdbool.h>
#include "stream.h"

class Serials : public Stream {
protected :
    bool opened; //for if(Serial)
public :
    int availableForWrite(void);
    void begin(unsigned long baud);
    void begin(unsigned long baud, int config);
    void end(void);
    // size_t write(val); available on print class 
    // size_t write(str); available on print class 
    // size_t write(buf, len); available on print class 
};

extern Serials Serial;

#endif