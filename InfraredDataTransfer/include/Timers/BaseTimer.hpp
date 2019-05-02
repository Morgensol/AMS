#include <stdint.h>
enum PWM_COMMAND{
    ON,
    OFF
};
class BaseTimer
{
    protected:

    public:
    int ActiveTimer;
    uint32_t runningFreq;
    int prescale;
    int* possiblePrescalers;
    int WGM0,WGM1,WGM2,WGM3;
    int COMnX0,COMnX1;
    int CSn0,CSn1,CSn2;
    uint32_t OCRn;
    virtual void turnPwm(PWM_COMMAND onOff);
    virtual void setPwmTo(int pwm);
    virtual int calculatePrescale(int freq);
};