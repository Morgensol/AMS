#include <stdint.h>

class BaseEncoder
{
protected:
    virtual uint16_t EncodeBit(uint8_t data); 
public:
    virtual void EncodeData(uint8_t* data, uint16_t* EncodedData,uint32_t length);
    ~BaseEncoder();
};

