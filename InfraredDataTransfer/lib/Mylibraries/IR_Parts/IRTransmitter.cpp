#include "IRTransmitter.hpp"

IRTransmitter::IRTransmitter(BaseEncoder *encoder, BaseTimer *timer)
{
    _encoder=encoder;
    _timer=timer;
}
IRTransmitter::~IRTransmitter()
{

}
void IRTransmitter::sendByte(uint8_t byte) //LSB & Little-Endian
{
    for (size_t i = 0; i < 8; i++)
    {
        if(byte&(1<<i))
        {
            _timer->togglePwm(ON);
            usleep(((1000ul*1000ul)/_timer->getRunningFreq())*10);
            _timer->togglePwm(OFF);
        }
        else
        {
            usleep(((1000ul*1000ul)/_timer->getRunningFreq())*10);
        }
        
    }
    
}
void IRTransmitter::sendData(uint8_t* Data,uint32_t length)
{
    //First 8 bytes with checksum and length
    uint8_t ChecksumAndLength[8];

    //Output arrays for Data and checksum+length
    uint16_t encodedData[length];
    uint16_t encodedChecksumAndLength[8];

    //Adding Checksum to Array
    uint32_t checksum=_encoder->generateChecksum(Data,length);
    ChecksumAndLength[0]=checksum&0xff;
    ChecksumAndLength[1]=(checksum&0xff00)>>8;
    ChecksumAndLength[2]=(checksum&0xff0000)>>16;
    ChecksumAndLength[3]=(checksum&0xff000000)>>24;
    
    //Adding Length to Array
    ChecksumAndLength[4]=length&0xff;
    ChecksumAndLength[5]=(length&0xff00)>>8;
    ChecksumAndLength[6]=(length&0xff0000)>>16;
    ChecksumAndLength[7]=(length&0xff000000)>>24;
    _encoder->EncodeData(ChecksumAndLength,encodedChecksumAndLength,8);
    _encoder->EncodeData(Data,encodedData,length);
    //Initiating write
    sendByte(0b11100000);
    for (size_t i = 0; i < 8; i++)
    {
        sendByte((uint8_t)encodedChecksumAndLength[i]&0xFF);
        sendByte((uint8_t)((encodedChecksumAndLength[i]&0xFF00)>>8));
    }
    for (size_t i = 0; i < length; i++)
    {
        sendByte((uint8_t)encodedData[i]&0xFF);
        sendByte((uint8_t)((encodedData[i]&0xFF00)>>8));
    }
    
}