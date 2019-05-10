#include "BaseEncoder.hpp"
#include <stdio.h>
class ManchesterEncoder: public BaseEncoder
{
private:
    /* data */
    uint16_t EncodeBit(uint8_t data);
public:
    ManchesterEncoder(/* args */);
    void EncodeData(uint8_t* data, uint16_t* EncodedData,uint32_t length);
    ~ManchesterEncoder();
};