#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>

class Print {
    private:
    
    public:
        virtual size_t write(uint8_t) = 0; // Added as print is an abstract base class

        size_t write(byte value);
        size_t write(const char *string);
        size_t write(const uint8_t *data, size_t length);

        size_t print(char val);
        size_t print(const char *val);
        size_t print(unsigned char val);
        size_t print(int val);
        size_t print(unsigned int val);
        size_t print(const __FlashStringHelper *val);
        size_t print(unsigned char val, int format);
        size_t print(int val, int format);
        size_t print(unsigned int val, int format);
        size_t print(long val, int format = DEC);
        size_t print(unsigned long val, int format = DEC);
        size_t print(double val, int format = 2);
        size_t println(unsigned char val);
        size_t println(int val);
        size_t println(unsigned int val);
        size_t println(unsigned char val, int format);
        size_t println(int val, int format);
        size_t println(unsigned int val, int format);
        size_t println(long val, int format = DEC);
        size_t println(unsigned long val, int format = DEC);
        size_t println(double val, int format = 2);
        size_t print(const __FlashStringHelper *val);
}

#endif