#include "WiFIUDP.h"

WiFiUDP::WiFiUDP(){
    
}

uint8_t WiFiUDP::begin(int port){
    if (){
        return 0;
    }
    else if (){
        return 1;
    }
}

// inherits from the Stream
int WiFiUDP::available(void){
    if (){
        // If parsePacket hasn't been called yet
        return 0;
    }

}

int WiFiUDP::beginPacket(const char *hostName, int port){

}

int WiFiUDP::beginPacket(uint32_t hostIp, int port){

}

int WiFiUDP::endPacket(void){
    if (){
        return 0;
    }
    else if (){
        return 1;
    }
}

size_t WiFiUDP::write(uint8_t byte){

}

size_t WiFiUDP::write(const uint8_t *buffer, size_t size){

}

int WiFiUDP::parsePacket(void){

    if (){
        return 0;
    }
}

int WiFiUDP::peek(void){
    if (){
        
        return -1;
    }

    // b is the next byte or character
    return b;
}

int WiFiUDP::read(void){
    if (){
        // if no (more) internal buffer is available (to be read)
        return -1;
    }

    // b is the characters in the internal buffer
    return b;
}

int WiFiUDP::read(char *buffer, int len){

    // size is the size of the buffer
    return size;
}

void WiFiUDP:flush(void){

}

void WiFiUDP::stop(void){
    
}

uint32_t WiFiUDP::remoteIP(void){
    // Return the 4 bytes IP address of the host who sent the current incoming packet
}

uint16_t WiFiUDP::remotePort(void){
    // Returns the port of the host who sent the current incoming packet
}
