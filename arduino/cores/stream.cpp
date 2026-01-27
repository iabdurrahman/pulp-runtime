#include "stream.h"

int Stream::available() {

}

int Stream::read() {

}

void Stream::flush() {

}

bool Stream::find(char *target) {

}

bool Stream::find(char *target, size_t length) {

}

bool Stream::findUntil(char *target, char *terminator) {

}

int Stream::peek(void) {

}

size_t Stream::readBytes(char buffer, int length) {

}

size_t Stream::readBytes(byte buffer, int length) {

}

size_t Stream::readBytesUntil(char character, char buffer, int length) {

}

size_t Stream::readBytesUntil(char character, byte buffer, int length) {

}

string Stream::readString(void) {

}

string Stream::readStringUntil(char terminator) {

}

long Stream::parseInt() {

}

long Stream::parseInt(LookaheadMode lookahead) {

}

long Stream::parseInt(LookaheadMode lookahead, char ignore) {

}

float Stream::parseFloat() {

}

float Stream::parseFloat(LookaheadMode lookahead) {

}

float Stream::parseFloat(LookaheadMode lookahead, char ignore) {

}

void Stream::setTimeout(unsigned long time) {

}
