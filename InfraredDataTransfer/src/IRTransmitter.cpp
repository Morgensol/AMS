#include "IR_Parts/IRTransmitter.hpp"

IRTransmitter::IRTransmitter(BaseEncoder *encoder, BaseTimer *timer)
{
    _encoder=encoder;
    _timer=timer;
}
void IRTransmitter::sendByte(uint8_t byte) //LSB & Little-Endian
{
    for (size_t i = 0; i < 8; i++)
    {
        if(byte&(1<<i))
        {
            _timer->togglePwm(ON);
            delayMicroseconds(((1/_timer->getRunningFreq())*1000*1000)*10);
            _timer->togglePwm(OFF);
        }
        else
        {
            delayMicroseconds(((1/_timer->getRunningFreq())*1000*1000)*10);
        }
        
    }
    
}
void IRTransmitter::sendData(uint8_t* Data,uint32_t length)
{
    uint16_t encoded[length];
    _encoder->EncodeData(Data,encoded,length);
    for (size_t i = 0; i < length; i++)
    {
        sendByte(encoded[i]&0xFF);
        sendByte((encoded[i]&0xFF00)>>8);
    }
    
}