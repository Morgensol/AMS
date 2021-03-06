#include "ManchesterEncoder.hpp"

ManchesterEncoder::ManchesterEncoder()
{

}
ManchesterEncoder::~ManchesterEncoder()
{

}
uint16_t ManchesterEncoder::EncodeBit(uint8_t data)
{
    uint16_t output=0, outputCounter=0 ; 
    for (uint8_t i = 0; i < 8; i++)
    {
        if (data&(1<<i))
        {
            outputCounter++;
            output|= 1<<outputCounter++;
        }
        else
        {
            output|= 1<<outputCounter++;
            outputCounter++;
        }
        
        
    }
    return output;
}
void ManchesterEncoder::EncodeData(uint8_t* data, uint16_t* EncodedData,uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
        EncodedData[i]=EncodeBit(data[i]);
    }
}

uint32_t ManchesterEncoder::generateChecksum(uint8_t* data,uint32_t length)
{
    uint32_t retData=0;
    for (size_t i = 0; i < length; i++)
    {
        retData+=data[i];
    }
    return retData;
}