#include "stream.h"


int Stream::timedRead() {
    int c;
    unsigned long _startMillis = millis();
    do {
        c = read();
        if(c >= 0) return c;
    } while (millis() - _startMillis < _timeout);
    return -1;
}

int Stream::timedPeek() {
    int c;
    unsigned long _startMillis = millis();
    do {
        c = peek();
        if(c >= 0) return c;
    } while(millis() - _startMillis < _timeout);
    return -1;
}

int Stream::peekNextDigit(LookaheadMode lookahead, bool detectDecimal)
{
  int c;
  while (1) {
    c = timedPeek();

    if( c < 0 ||
        c == '-' ||
        (c >= '0' && c <= '9') ||
        (detectDecimal && c == '.')) return c;

    switch( lookahead ){
        case SKIP_NONE: return -1; // Fail code.
        case SKIP_WHITESPACE:
            switch( c ){
                case ' ':
                case '\t':
                case '\r':
                case '\n': break;
                default: return -1; // Fail code.
            }
        case SKIP_ALL:
            break;
    }
    read();  // discard non-numeric
  }
}

bool Stream::find(char *target) {
    return findUntil(target, NULL);
}

bool Stream::find(char *target, size_t length) {
    return findUntil(target, NULL);   
}

bool Stream::findUntil(char *target, char *terminator) {

    if (target == nullptr || target[0] == '\0') return true;

    size_t target_len = strlen(target);
    size_t terminator_len = (terminator != NULL) ? strlen(terminator) : 0;
    size_t index = 0;
    size_t term_idx = 0;

    while (true) {
        int c = timedRead();

        if (c == -1) return false;

        char current = (char) c;

        if (current == target[index]) {
            index++;
            if (index == target_len) return true;
        } else {
            index = (current == target[0]) ? 1 : 0;
        }

        if (terminator_len > 0) {
            if (current == terminator[term_idx]) {
                term_idx++;
                if (term_idx == terminator_len) return false;
            } else {
                term_idx = (current == terminator[0]) ? 1 : 0;
            }
        }
    }
}

size_t Stream::readBytes(char *buffer, size_t length) {
    size_t count = 0;
    while (count < length) {
        int c = timedRead();
        if(c < 0) break;
        buffer[count] = (char) c;
        count++;
    }
    return count;
}

size_t Stream::readBytes(byte *buffer, size_t length) {
    return readBytes((char *) buffer, length);
}

size_t Stream::readBytesUntil(char character, char *buffer, size_t length) {
    size_t count = 0;
    while (count < length) {
        int c = timedRead();
        if(c < 0) break;
        if(c == (int) character) break;
        buffer[count] = (char) c;
        count++;
    }

    return count;
}

size_t Stream::readBytesUntil(char character, byte *buffer, size_t length) {
    return readBytesUntil(character, (char *) buffer, length);
}

// String Stream::readString(void) {
//     String retval;
//     while(true) {
//         int c = timedRead();
//         if(c < 0) break;
//         retval += (char) c;
//     }
//     return retval;
// }

// String Stream::readStringUntil(char terminator) {
//     String retval;
//     while(true) {
//         int c = timedRead();
//         if (c < 0 || c == (int) terminator)break;
//         retval += (char) c;
//     }
//     return retval;
// }

long Stream::parseInt() {
    return parseInt(SKIP_ALL, '\0');
}

long Stream::parseInt(LookaheadMode lookahead) {
    return parseInt(lookahead, '\0');
}

long Stream::parseInt(LookaheadMode lookahead, char ignore) {
    bool isNegative = false;
    long value = 0;
    int c;  
    c = peekNextDigit(lookahead, false);
    // ignore non numeric leading characters
    if(c < 0)
      return 0; // zero returned if timeout 
    do{
      if(c == ignore)
        ; // ignore this character
      else if(c == '-')
        isNegative = true;
      else if(c >= '0' && c <= '9')        // is c a digit?
        value = value * 10 + c - '0';
      read();  // consume the character we got with peek
      c = timedPeek();
    }
    while( (c >= '0' && c <= '9') || c == ignore ); 
    if(isNegative)
      value = -value;
    return value;
}

float Stream::parseFloat() {
    return parseFloat(SKIP_ALL, '\0');
}

float Stream::parseFloat(LookaheadMode lookahead) {
    return parseFloat(lookahead, '\0');
}

float Stream::parseFloat(LookaheadMode lookahead, char ignore) {
    bool isNegative = false;
    bool isFraction = false;
    long value = 0;
    int c;
    float fraction = 1.0;

    c = peekNextDigit(lookahead, true);
      // ignore non numeric leading characters
    if(c < 0)
      return 0; // zero returned if timeout

    do{
      if(c == ignore)
        ; // ignore
      else if(c == '-')
        isNegative = true;
      else if (c == '.')
        isFraction = true;
      else if(c >= '0' && c <= '9')  {      // is c a digit?
        value = value * 10 + c - '0';
        if(isFraction)
           fraction *= 0.1;
      }
      read();  // consume the character we got with peek
      c = timedPeek();
    }
    while( (c >= '0' && c <= '9')  || (c == '.' && !isFraction) || c == ignore );

    if(isNegative)
      value = -value;
    if(isFraction)
      return value * fraction;
    else
      return value;
}

void Stream::setTimeout(unsigned long time) {
    _timeout = (time > 0) ? time : def_timeout;
}
