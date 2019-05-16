#pragma once
#include "unity.h"
#include "ManchesterEncoder.hpp"
void InputIs1234_ChecksumIs10()
{
    //Arange
    ManchesterEncoder uut = ManchesterEncoder();
    uint8_t input[]={1,2,3,4};
    //Act
    uint32_t output = uut.generateChecksum(input,sizeof(input));
    //Assert
    TEST_ASSERT_EQUAL(10,output);
}
void InputIs1_Outputs1InManchesterCode()
{
    //Arange
    ManchesterEncoder uut = ManchesterEncoder();
    uint8_t input[]={1};
    uint16_t testOutput[sizeof(input)];
    //Act
    uut.EncodeData(input,testOutput,sizeof(input));
    //Assert
    TEST_ASSERT_EQUAL(0b0101010101010110,testOutput[0]);
}