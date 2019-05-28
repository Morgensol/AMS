#include <Arduino.h>
#include <unity.h>

#include "test_ManchesterEncoder.hpp"
#include "test_IRTransmitter.hpp"
void setup(){
    UNITY_BEGIN();
}
void loop(){
    RUN_TEST(InputIs1234_ChecksumIs10);
    _delay_ms(500);
    RUN_TEST(InputIs1_Outputs1InManchesterCode);
    _delay_ms(500);
    UNITY_END();
}