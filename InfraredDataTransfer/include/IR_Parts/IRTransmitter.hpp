#include "Encoders/ManchesterEncoder.hpp"
#include "Timers/CTCTimer.hpp"
class IRTransmitter
{
private:
    BaseEncoder* _encoder=NULL;
    BaseTimer* _timer=NULL;
    void sendByte(uint8_t bit);
public:
    IRTransmitter(BaseEncoder* encoder, BaseTimer* timer);
    ~IRTransmitter();
    void sendData(uint8_t* Data,uint32_t length);
};

