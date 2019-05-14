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
    uint8_t packedData[length+8];
    AddChecksumAndLength(data,length,packedData);
    for (uint32_t i = 0; i < length; i++)
    {
        EncodedData[i]=EncodeBit(packedData[i]);
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
void ManchesterEncoder::AddChecksumAndLength(uint8_t* data, uint32_t length, uint8_t* returnData)
{
    uint32_t checksum=0;
    returnData[4]=length&0xff;
    returnData[5]=(length&0xff00)>>8;
    returnData[6]=(length&0xff0000)>>16;
    returnData[7]=(length&0xff000000)>>24;
    for (size_t i = 0; i < 4; i++)
    {
        checksum+=returnData[i+4];
    }
    
    for (size_t i = 0; i < length; i++)
    {
        checksum+=data[i];
        returnData[i+8]=data[i];
    }
    returnData[0]=checksum&0xff;
    returnData[1]=(checksum&0xff00)>>8;
    returnData[2]=(checksum&0xff0000)>>16;
    returnData[3]=(checksum&0xff000000)>>24;
}