#include <Arduino.h>
#include <avr/interrupt.h>

class IRReceiver{
private:
int frequence = 0;
public:
IRReceiver(int inter, bool setup);
~IRReceiver();
void setupInterrupt(int inter);
int getFrequence();
void setFrequence(int val);
void Receive();
};