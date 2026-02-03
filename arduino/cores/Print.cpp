#include <Print.h>

size_t Print::write(const uint8_t *data, size_t length){
    size_t n = 0;
    while (length--) {
        if (write(*data++)) n++;
        else break;
    }
    return n;
}

// Parameters val allowed any data type. The missing implemented data types are bool, boolean, byte, short, word, float and string().

size_t Print::print(char val){
    return write(val);  // Print as it is
}

size_t Print::print(const char *val){
    return write(val);  // Print as it is
}

size_t Print::print(unsigned char val){
    return print((unsigned long) val, DEC);
}

size_t Print::print(int val){
    return print((long) val, DEC);
}

size_t Print::print(unsigned int val){
    return print((unsigned long) val, DEC);
}

size_t Print::print(const __FlashStringHelper *val){
    const char *p = (const char *)val;
    size_t n = 0;
    while (*p) {
        if(write(p++))n++;
        else break;
    }
    return n;
}

size_t Print::print(unsigned char val, int format){
    return print((unsigned long) val, format);
}

size_t Print::print(int val, int format){
    return print((long) val, format);
}

size_t Print::print(unsigned int val, int format){
    return print((unsigned long) val, format);
}

size_t Print::print(long val, int format){
    if (format == 0) {
        return write(val);
    } else if (format == 10) {
        if (val < 0) {
            int t = print('-');
            val = -val;
            return printNumber(val, 10) + t;
        }
        return printNumber(val, 10);
    } else {
        return printNumber(val, format);
    }
}

size_t Print::print(unsigned long val, int format){
    if (format == 0) return write(val);
    else return printNumber(val, format);
}

size_t Print::println(void)
{
  return write("\r\n");
}

size_t Print::print(double val, uint8_t format){
    return printFloat(val, format);
}

size_t Print::println(unsigned char val){
    return println((unsigned long) val, DEC);
}

size_t Print::println(int val){
    return println((long) val, DEC);
}

size_t Print::println(unsigned int val){
    return println((unsigned long) val, DEC);
}

size_t Print::println(unsigned char val, int format){
    return println((unsigned long) val, format);
}

size_t Print::println(int val, int format){
    return println((long) val, format);
}

size_t Print::println(unsigned int val, int format){
    return println((unsigned long) val, format);
}

size_t Print::println(long val, int format){
    size_t n = print(val, format);
    n += println();
    return n;
}

size_t Print::println(unsigned long val, int format){
    size_t n = print(val, format);
    n += println();
    return n;
}

size_t Print::println(double val, int format){
    size_t n = print(val, format);
    n += println();
    return n;
}

size_t Print::println(const __FlashStringHelper *val){
    size_t n = print(val);
    n += println();
    return n;
}

// Private Methods /////////////////////////////////////////////////////////////

size_t Print::printNumber(unsigned long n, uint8_t base)
{
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  return write(str);
}

static inline bool _isnan(double x)
{
  return x != x;
}

static inline bool _isinf(double x)
{
  return !_isnan(x) && _isnan (x - x);
}

size_t Print::printFloat(double number, uint8_t digits) 
{ 
  size_t n = 0;
  
  if (_isnan(number)) return print("nan");
  if (_isinf(number)) return print("inf");
  if (number > 4294967040.0) return print ("ovf");  // constant determined empirically
  if (number <-4294967040.0) return print ("ovf");  // constant determined empirically
  
  // Handle negative numbers
  if (number < 0.0)
  {
     n += print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    n += print('.'); 
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)(remainder);
    n += print(toPrint);
    remainder -= toPrint; 
  } 
  
  return n;
}
