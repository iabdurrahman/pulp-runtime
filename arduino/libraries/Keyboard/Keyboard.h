#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

class KeyboardClass{
public:
    KeyboardClass();
    void begin(const uint8_t* layout = KeyboardLayout_en_US);
    void end(void);
    size_t press(char key);
    size_t print(char character);
    size_t print(const char* characters);
    size_t println(void);
    size_t println(char character);
    size_t println(const char* characters);
    size_t release(char key);
    void releaseAll(void);
    size_t write(char character);

private:

};

extern KeyboardClass Keyboard;

#endif