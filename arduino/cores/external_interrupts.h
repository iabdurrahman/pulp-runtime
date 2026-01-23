void attachInterrupt(digitalPinToInterrupt(pin), void(*userFunc)(void), int mode);
void detachInterrupt(digitalPinToInterrupt(pin));
int digitalPinToInterrupt(int pin);