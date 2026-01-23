#include "external_interrupts.h"

void attachInterrupt(digitalPinToInterrupt(pin), void(*userFunc)(void), int mode){
    
    switch (mode) {
        case LOW:   
            break;
        case CHANGE:
            break;
        case RISING:
            break;
        case FALLING:
            break;
        default:
            return;
    }

}

void detachInterrupt(digitalPinToInterrupt(pin)){

}

int digitalPinToInterrupt(int pin){
    if (){
        return pin;
    }
    else{
        return -1;
    }
}