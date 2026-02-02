#ifndef advanced_io_h
#define advanced_io_h

#include <stdint.h>

#define HIGH 1
#define LOW 0

#define MSBFIRST 1
#define LSBFIRST 0

#define GPIO_PWM 17
#define GPIO_PWM_MUX 2

void noTone(uint8_t pin);

unsigned long pulseIn(int pin, int value);
unsigned long pulseIn(int pin, int value, unsigned long timeout);

unsigned long pulseInLong(int pin, int value);
unsigned long pulseInLong(int pin, int value, unsigned long timeout);

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value);

void tone(uint8_t pin, unsigned int frequency, unsigned long duration); 

#endif