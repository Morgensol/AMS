#include "ManchesterDecoder.hpp"
ManchesterDecoder::ManchesterDecoder(/* args */)
{
}

ManchesterDecoder::~ManchesterDecoder()
{
}
uint8_t ManchesterDecoder::DecodeBit(uint16_t data)
{
    uint8_t output=0, outputCounter=0 ;
    for (uint8_t i = 0; i < 16; i++)
    {
        if (((data&(1<<i))==0) and ((data&(1<<++i))!=0))
        {
            output|=1<<outputCounter;
        }
        outputCounter++;
        
    }
    return output;
}
void ManchesterDecoder::DecodeData(uint16_t* input, uint8_t* output,uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
        output[i]=DecodeBit(input[i]);
    }
}