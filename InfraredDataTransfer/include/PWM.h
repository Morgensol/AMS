#include <Arduino.h>

enum mode{
    NORMAL=0,
    PHASE_CORRECT,
    CTC,
    FAST_PWM
};
class PWM
{
private:
    int prescales[5]{1,8,64,256,1024};
    int prescaler;
    int OCRn;
    int RunningFreq;
public:
    PWM(int timer, mode type, int freq);
    ~PWM();
    void setPWM(int dutycycle);
    void CalculateAndSetPrescale();
};

