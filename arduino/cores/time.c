#include "time.h"
#include "implem/delay.h"
#include "implem/tick.h"

void delay(unsigned long ms) {
    pos_delay_busy_ms((long) ms);
}

void delayMicroseconds(unsigned int us) {
    pos_delay_busy_us((long) us);
}

unsigned long millis(void) {
    return (unsigned long) pos_tick_get_counter_ms();
}

unsigned long micros(void) {
    return (unsigned long) pos_tick_get_counter_us();   
}