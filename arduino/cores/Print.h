#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>

#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN // Prevent warnings if BIN is previously defined
#undef BIN
#endif
#define BIN 2

class Print {
    private:
        size_t printNumber(unsigned long n, uint8_t base);
        size_t printFloat(double number, uint8_t digits);
    public:
        virtual size_t write(uint8_t) = 0; // Added as print is an abstract base class

        static inline size_t _strlen(const char *s) {
            size_t n = 0;
            while (*s++) n++;
            return n;
        }

        size_t write(const char *string){
            if (string == NULL) return 0;
            return write((const uint8_t *)string, _strlen(string));
        }
        virtual size_t write(const uint8_t *data, size_t length);
        size_t write(const char *data, size_t length) {
            return write((const uint8_t *)data, length);
        }

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
        size_t println(const __FlashStringHelper *val);
        size_t println(void);
};

#endif