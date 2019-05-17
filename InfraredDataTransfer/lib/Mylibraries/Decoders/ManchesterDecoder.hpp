#ifndef MANCHESTERDECODER_H
#define MANCHESTERDECODER_H
#include "BaseDecoder.hpp"

class ManchesterDecoder : BaseDecoder
{
private:
    uint8_t DecodeBit(uint16_t data);
public:
    void DecodeData(uint16_t* input, uint8_t* output,uint32_t length);
    ManchesterDecoder();
    ~ManchesterDecoder();
};
#endif