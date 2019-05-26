#include <Arduino.h>
#include <avr/interrupt.h>
#include "CTCTimer.hpp"

struct IRReturnData{
    uint32_t length;
    uint8_t* streng;
};
class IRReceiver{
private:
    CTCTimer* _timer =NULL;
    uint32_t readBuffer(uint8_t* output, uint32_t index);
    uint32_t calculateOriginal(uint8_t* array, uint32_t nmbrOfBytes);
public:
    IRReceiver();
    ~IRReceiver();
    void setupInterrupt();
    IRReturnData Receive();
};