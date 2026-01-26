#ifndef MOUSE_H
#define MOUSE_H

class Mouseclass {
public:
    void begin(void);
    void click(char button);
    void end(void);
    void move(signed char xVal, signed char yVal, signed char wheel)
    void press(char button = MOUSE_LEFT);
    void release(char button = MOUSE_LEFT);
    bool isPressed(char button = MOUSE_LEFT);

private:

};

extern MouseClass Mouse;

#endif