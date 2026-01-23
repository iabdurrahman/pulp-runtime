#include "arduino_math.h"

long map(long value, long fromlow, long fromHigh, long toLow, long toHigh) {
    return (value - fromlow) * (toHigh - toLow) / (fromHigh - fromlow) + toLow;
}

