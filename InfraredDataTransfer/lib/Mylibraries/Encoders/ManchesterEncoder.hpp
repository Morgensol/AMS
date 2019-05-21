#ifndef MANCHESTERENCODER_H
#define MANCHESTERENCODER_H
#include "BaseEncoder.hpp"
#include <stdio.h>
class ManchesterEncoder: public BaseEncoder
{
private:
    /* data */
    
public:
    ManchesterEncoder(/* args */);
    uint16_t EncodeBit(uint8_t data);
    void EncodeData(uint8_t* data, uint16_t* EncodedData,uint32_t length);
    virtual uint32_t generateChecksum(uint8_t* data,uint32_t length);
    ~ManchesterEncoder();
};
#endif