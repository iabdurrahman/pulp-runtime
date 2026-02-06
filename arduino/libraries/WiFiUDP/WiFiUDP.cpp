#include <stdint.h>
#include <stddef.h>

#include "WiFiUDP.h"

WiFiUDP::WiFiUDP(void){

}

WiFiUDP::~WiFiUDP(void){

}

uint8_t WiFiUDP::begin(int port){
    if (1){
        return 0;
    }
    else /*if (0)*/{
        return 1;
    }
}

// inherits from the Stream
int WiFiUDP::available(void){
    if (1){
        // If parsePacket hasn't been called yet
        return 0;
    }

}

int WiFiUDP::beginPacket(const char *hostName, int port){
    return -1;
}

int WiFiUDP::beginPacket(uint32_t hostIp, int port){
    return -1;
}

int WiFiUDP::endPacket(void){
    if (1){
        return 0;
    }
    else /*if (0)*/{
        return 1;
    }
}

size_t WiFiUDP::write(uint8_t byte){
    return -1;
}

size_t WiFiUDP::write(const uint8_t *buffer, size_t size){
    return -1;
}

int WiFiUDP::parsePacket(void){

    if (1){
        return 0;
    }
}

int WiFiUDP::peek(void){
    int b = 0;
    if (1){
        
        return -1;
    }

    // b is the next byte or character
    return b;
}

int WiFiUDP::read(void){
    int b = 0;
    if (1){
        // if no (more) internal buffer is available (to be read)
        return -1;
    }

    // b is the characters in the internal buffer
    return b;
}

int WiFiUDP::read(char *buffer, int len){
    int size = 0; // dummy
    // size is the size of the buffer
    return size;
}

void WiFiUDP::flush(void){

}

void WiFiUDP::stop(void){
    
}

uint32_t WiFiUDP::remoteIP(void){
    // Return the 4 bytes IP address of the host who sent the current incoming packet
    return -1;
}

uint16_t WiFiUDP::remotePort(void){
    // Returns the port of the host who sent the current incoming packet
    return -1;
}
