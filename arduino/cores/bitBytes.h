#ifndef bitBytes_h
#define bitBytes_h

#define bit(n) (1 << (n))

#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitWrite(value, n, bit) ((bit)? bitSet(value, n) : bitClear(value, n))

#define highByte(w) ((uint8_t) (((w) >> 8) & 0xFF))
#define lowByte(w) ((uint8_t) ((w) & 0xFF))

#endif