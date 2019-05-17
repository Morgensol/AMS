#include <unity.h>
#include "ManchesterDecoder.hpp"
void inputIsEncoded_OutputIs1()
{
    uint16_t input[1]={0b0101010101011001};
    uint8_t output[1];
    ManchesterDecoder uut = ManchesterDecoder();
    uut.DecodeData(input,output,1);
    TEST_ASSERT_EQUAL(0b00000010,output[0]);
}
void asdasd()
{
    uint16_t input[1]={0b1010101010101010};
    uint8_t output[1];
    ManchesterDecoder uut = ManchesterDecoder();
    uut.DecodeData(input,output,1);
    TEST_ASSERT_EQUAL(0b11111111,output[0]);
}