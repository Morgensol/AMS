#include <Arduino.h>
#include <unity.h>

#include "Test_ManchesterEncoder.hpp"

void test_asd(){
    TEST_ASSERT_EQUAL(1,1);
}

void setup(){
    UNITY_BEGIN();
}
void loop(){
    RUN_TEST(test_asd);
    RUN_TEST(newtestasd);
    UNITY_END();
}