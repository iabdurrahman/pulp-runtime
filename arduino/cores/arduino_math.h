#ifndef arduino_math_h
#define arduino_math_h

#include <math.h>

#define constrain(x, a, b) ((x)<(a)? (a) : (x)>(b)? (b) : (x))
#define max(x, y) ((x)>(y)? (x) : (y))
#define min(x, y) ((x)<(y)? (x) : (y))
#define sq(x) ((x)*(x))

long map (long value, long fromLow, long fromHigh, long toLow, long toHigh);

/*abs() : already defined in math.h*/
/*pow() : already defined in math.h*/
/*sqrt() : already defined in math.h*/

#endif


