#include "PWM.h"
#define F_CPU 16000000
#include <stdio.h>
PWM::PWM(int timer, mode wantedMode, int freq)
{
    int wgm0=0, wgm1=0, wgm2=1, wgm3=0; //CTC
    int COMnA1=0, COMnA0=1;  // Toggle Compare
    RunningFreq=freq;
    CalculateAndSetPrescale();
    switch (timer)
    {
        case 1:
            TCCR1A|= wgm0 | wgm1<<1 | COMnA0<<6 | COMnA1<<7;
            TCCR1B|= wgm2<<3 | wgm3<<4;
            break;
        case 3:
            wgm0=0; wgm1=0; wgm2=0;
            break;
        case 4:
            break;
        case 5:
            wgm0=0; wgm1=0; wgm2=0;
            break;    
        default:
            wgm0=0;
            wgm1=0;
            wgm2=0;
            break;
    }
    
}
void PWM::CalculateAndSetPrescale()
{
    int OCR=0;
    for(size_t i = 0; i < 5 ; i++)
    {
        OCR=F_CPU/(RunningFreq*2*(1<<prescales[i]));
        if((OCR<256) & (OCR!=0))
        {
            OCRn=OCR;
            prescaler=prescales[i];
            TCCR1B|=i+1;
        }
    }
    OCR1A=OCRn;
}
PWM::~PWM()
{
}
void PWM::setPWM(int dutycycle)
{

}