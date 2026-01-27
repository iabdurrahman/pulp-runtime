#ifndef stream_h
#define stream_h

#include <stdbool.h>
#include <stddef.h>
#include "print.h"

enum LookaheadMode {
    SKIP_ALL, //all characters other than digits or a minus sign are ignored when scanning the stream for an integer. This is the default mode.
    SKIP_NONE, //nothing is skipped, and the stream is not touched unless the first waiting character is valid.
    SKIP_WHITESPACE //only tabs, spaces, line feeds, and carriage returns are skipped.
};

class Stream : public Print {
public :
    int available(void);
    int read(void);
    void flush(void);
    bool find(char *target);
    bool find(char *target, size_t length);
    bool findUntil(char *target, char *terminator);
    int peek(void);
    size_t readBytes(char buffer, int length);
    size_t readBytes(byte buffer, int length);
    size_t readBytesUntil(char character, char buffer, int length);
    size_t readBytesUntil(char character, byte buffer, int length);
    string readString(void);
    string readStringUntil(char terminator);
    long parseInt();
    long parseInt(LookaheadMode lookahead);
    long parseInt(LookaheadMode lookahead, char ignore);
    float parseFloat();
    float parseFloat(LookaheadMode lookahead);
    float parseFloat(LookaheadMode lookahead, char ignore);
    void setTimeout(unsigned long time);
};

#endif