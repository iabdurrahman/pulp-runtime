#ifndef arduino_time_h
#define arduino_time_h

#include <stdint.h>

void delay(unsigned long ms);

void delayMicroseconds(unsigned int us);

unsigned long millis(void);

unsigned long micros(void);

#endif