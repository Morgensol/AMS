#ifndef CTCTIMER_H
#define CTCTIMER_H
#include <Arduino.h>
#include "BaseTimer.hpp"
#include <stdio.h>

class CTCTimer : public BaseTimer
{
private:
    int calculatePrescale(uint32_t desiredFrequency,int timer);
public:
    CTCTimer(int timer, uint32_t freq);
    ~CTCTimer();
    void togglePwm(PWM_COMMAND onOff);
    uint32_t getRunningFreq();
};
#endif