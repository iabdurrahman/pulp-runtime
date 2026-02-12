/**
    * this file is implementation of Arduino Math API
    * https://docs.arduino.cc/language-reference/en/functions/math/
*/

#ifndef arduino_math_h
#define arduino_math_h



#include <math.h> /* provide standard math functions : abs(), pow(), and sqrt() */


/**
    * macro to constrain a number 'X' to be within a range (a (min), b (max))
    * if X is less than a, it returns a. If X is greater than b, it returns b
    * https://docs.arduino.cc/language-reference/en/functions/math/constrain/
*/
#define constrain(x, a, b) ((x)<(a)? (a) : (x)>(b)? (b) : (x))

/**
    * macro to find the larger of two numbers (x and y)
    * returns x if x is larger than y, otherwise returns y
    * https://docs.arduino.cc/language-reference/en/functions/math/max/ 
*/
#define max(x, y) ((x)>(y)? (x) : (y))

/**
    * macro to find the smaller of two numbers (x and y)
    * returns x if x is smaller than y, otherwise returns y
    * https://docs.arduino.cc/language-reference/en/functions/math/min/
*/
#define min(x, y) ((x)<(y)? (x) : (y))

/**
    * macro to calculate the square of a number
    * the number multiplied by itself
    * https://docs.arduino.cc/language-reference/en/functions/math/sq/
*/
#define sq(x) ((x)*(x))

#ifdef __cplusplus
extern "C" {
#endif
/**
    * method map
    * re-maps a number from one range to another
    * value of fromLow would get mapped to toLow, a value of fromHigh to toHigh
    * @param value The number to map.
    * @param fromLow The lower bound of the value's current range.
    * @param fromHigh The upper bound of the value's current range.
    * @param toLow The lower bound of the value's target range.
    * @param toHigh The upper bound of the value's target range.
    * @return The mapped value.
    * https://docs.arduino.cc/language-reference/en/functions/math/map/
*/
long map (long value, long fromLow, long fromHigh, long toLow, long toHigh);

#ifdef __cplusplus
}
#endif

#endif


