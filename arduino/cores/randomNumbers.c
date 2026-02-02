#include "stdlib.h"
#include <math.h>

long random(long max) {
    if (max == 0) {
        return 0;
    }
    return random() % max
}

long random(long min, long max) {
    if (min >= max) {
        return min;
    }
    long diff = max - min;
    return random(diff) + min;
}

void randomSeed(unsigned long seed) {
    if (seed != 0) {
        random(seed);
    }
}