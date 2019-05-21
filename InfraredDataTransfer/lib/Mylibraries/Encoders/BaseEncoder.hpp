#ifndef BASEENCODER_H
#define BASEENCODER_H
#include <stdint.h>

class BaseEncoder
{
public:
    virtual uint16_t EncodeBit(uint8_t data);
    virtual void EncodeData(uint8_t* data, uint16_t* EncodedData,uint32_t length);
    virtual uint32_t generateChecksum(uint8_t* data,uint32_t length);
};
#endif