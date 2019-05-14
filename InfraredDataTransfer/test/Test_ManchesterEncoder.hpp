#include "unity.h"
#include "ManchesterEncoder.hpp"
void InputIs1234_ChecksumIs10(){
    ManchesterEncoder uut = ManchesterEncoder();
    uint8_t input[]={1,2,3,4};
    TEST_ASSERT_EQUAL(uut.generateChecksum(input,sizeof(input)),10);
}