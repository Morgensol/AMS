#ifndef BASEDECODER_H
#define BASEDECODER_H
#include <stdint.h>
class BaseDecoder
{
protected:
    virtual uint8_t DecodeBit(uint16_t data);
public:
    virtual void DecodeData(uint16_t* input, uint8_t* output,uint32_t length);
};
#endif

