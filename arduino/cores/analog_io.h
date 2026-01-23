#include <stdint.h>

int analogRead(int pin);
void analogReadResolution(int bits);
void analogReference(uint8_t type);
void analogWrite(int pin, int value);
void analogWriteResolution(int bits);