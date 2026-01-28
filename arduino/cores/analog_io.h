#include <stdint.h>

#ifndef GPIO_PWM
#define GPIO_PWM        17
#endif

#ifndef GPIO_PWM_MUX
#define GPIO_PWM_MUX    2
#endif

int analogRead(int pin);
void analogReadResolution(int bits);
void analogReference(uint8_t type);
void analogWrite(int pin, int value);
void analogWriteResolution(int bits);