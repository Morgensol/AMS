#include <Arduino.h>
#include <avr/interrupt.h>
#include "CTCTimer.hpp"

struct IRReturnData{
    uint32_t length;
    char* streng;
};
class IRReceiver{
private:
int frequence = 0;
CTCTimer* _timer =NULL;
uint32_t readBuffer(uint8_t* output, uint32_t index);
uint32_t calculateReadLength(uint8_t* array, uint32_t nmbrOfBytes);
public:
IRReceiver();
~IRReceiver();
void setupInterrupt();
IRReturnData Receive();
};