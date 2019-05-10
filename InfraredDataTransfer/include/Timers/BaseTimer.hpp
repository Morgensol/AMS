#include <stdint.h>
enum PWM_COMMAND{
    ON,
    OFF
};
class BaseTimer
{
    protected:
    int ActiveTimer;
    uint32_t runningFreq;
    int prescale;
    int* possiblePrescalers;
    int WGM0,WGM1,WGM2,WGM3;
    int COMnX0,COMnX1;
    int CSn0,CSn1,CSn2;
    uint32_t OCRn;
    virtual int calculatePrescale(uint32_t desiredFrequency,int timer);
    public:
    virtual void togglePwm(PWM_COMMAND onOff);
    virtual uint32_t getRunningFreq();
};
