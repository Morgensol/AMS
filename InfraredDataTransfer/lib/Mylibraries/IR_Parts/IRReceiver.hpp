#include <Arduino.h>
#include <avr/interrupt.h>
#include "CTCTimer.hpp"

struct IRReturnData{
    uint32_t length;
    uint8_t* streng;
};
class IRReceiver{
private:
int frequence = 0;
CTCTimer* _timer =NULL;
public:
IRReceiver(int inter, bool setup);
~IRReceiver();
void setupInterrupt(int inter);
int getFrequence();
void setFrequence(int val);
IRReturnData Receive();
};