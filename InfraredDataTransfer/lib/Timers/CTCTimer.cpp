#include "CTCTimer.hpp"

CTCTimer::CTCTimer(int timer, uint32_t freq)
{
    int setupA,setupB;
    runningFreq = freq;
    ActiveTimer=timer;
    if (timer==0) {
        possiblePrescalers = new int[5]{1,8,64,256,1024};
        prescale    = calculatePrescale(freq,timer);
        WGM0    = 0;
        WGM1    = 1;
        WGM2    = 0;
        WGM3    = 0;
        COMnX0  = 1;
        COMnX1  = 0;
        setupA  = (WGM0<<WGM00) + (WGM1<<WGM01) + (COMnX0<<COM0A0) + (COMnX1<<COM0A1);
        setupB  = (WGM2<<WGM02) + (CSn0<<CS00) + (CSn1<<CS01) + (CSn2<< CS02);
    }
    else if(timer==2){
        possiblePrescalers = new int[7]{1,8,32,64,128,256,1024};
        prescale    = calculatePrescale(freq,timer);
        WGM0    = 0;
        WGM1    = 1;
        WGM2    = 0;
        WGM3    = 0;
        COMnX0  = 1;
        COMnX1  = 0;
        setupA  = (WGM0<<WGM20) + (WGM1<<WGM21) + (COMnX0<<COM2A0) + (COMnX1<<COM2A1);
        setupB  = (WGM2<<WGM22) + (CSn0<<CS20) + (CSn1<<CS21) + (CSn2<< CS22);
    }
    else{
        possiblePrescalers = new int[5]{1,8,64,256,1024};
        prescale    = calculatePrescale(freq,timer);
        WGM0    = 0;
        WGM1    = 0;
        WGM2    = 1;
        WGM3    = 0;
        COMnX0  = 1;
        COMnX1  = 0;
        setupA  = (WGM0<<WGM10) + (WGM1<<WGM11) + (COMnX0<<COM1A0) + (COMnX1<<COM1A1);
        setupB  = (WGM2<<WGM12) + (WGM3<<WGM13) + (CSn0<<CS00) + (CSn1<<CS01) + (CSn2<<CS02);
    }
    
    switch (timer)
    {
        case 0:
            pinMode(13, OUTPUT);
            TCCR0A  |= setupA;
            TCCR0B  |= setupB;
            OCR0A    = OCRn;
            break;
        case 1:
            pinMode(11, OUTPUT);
            TCCR1A  = setupA;
            TCCR1B  = setupB;
            OCR1A    = OCRn;
            break;
        case 2:
            pinMode(10, OUTPUT);
            TCCR2A  |= setupA;
            TCCR2B  |= setupB;
            OCR2A    = OCRn;
            break;
        case 3:
            pinMode(5, OUTPUT);
            TCCR3A  |= setupA;
            TCCR3B  |= setupB;
            OCR3A    = OCRn;
            break;
        case 4:
            pinMode(6, OUTPUT);
            TCCR4A  |= setupA;
            TCCR4B  |= setupB;
            OCR4A    = OCRn;
            break;
        case 5:
            pinMode(38, OUTPUT);
            TCCR5A  |= setupA;
            TCCR5B  |= setupB;
            OCR5A    = OCRn;
            break;    
        default:
            break;

    }
}

int CTCTimer::calculatePrescale(uint32_t desiredFrequency,int timer)
{
    uint32_t OCR=0;
    if(timer==0 or timer==2){
        for(size_t i = 0; i < (timer==2)?7:5 ; i++)
        {
            OCR=(F_CPU/(desiredFrequency*2*possiblePrescalers[i]))-1;
            if((OCR<0xff) & (OCR>0)){
                OCRn    = OCR;
                CSn0     = (0b00000001) & (i+1);
                CSn1     = (0b00000010) & (i+1);
                CSn2     = (0b00000100) & (i+1);
                return possiblePrescalers[i];
            }
        }
    }
    else{
        for(size_t i = 0; i < 5 ; i++)
        {
            OCR=(F_CPU/(desiredFrequency*2*possiblePrescalers[i]))-1;
            if((OCR<0xffff) & (OCR>0)){
                OCRn    = OCR;
                CSn0     = (0b00000001) & (i+1);
                CSn1     = (0b00000010) & (i+1);
                CSn2     = (0b00000100) & (i+1);
                return possiblePrescalers[i];
            }
        }        
    }
    return 0;
}
void CTCTimer::togglePwm(PWM_COMMAND onOff){
    switch (ActiveTimer)
    {
        case 0:
            if (onOff==OFF)
            {
                TCCR0A&=~((1<<COM0A1)|(1<<COM0A0));
                digitalWrite(13,LOW);
            }
            else{
                TCCR0B|=(1<<COM0A0);
            }
            
            break;
        case 1:
            if (onOff==OFF){
                TCCR1A&= ~((1<<COM1A1)|(1<<COM1A0));
                digitalWrite(11,LOW);
            }
            else{
                TCCR1A|=(1<<COM1A0);
            }
            break;
        case 2:
            if (onOff==OFF){
                TCCR2A&=~((1<<COM2A1)|(1<<COM2A0));
                digitalWrite(10,LOW);
            }
            else{
                TCCR2A|=(1<<COM2A0);
            }
            break;
        case 3:
            if (onOff==OFF){
                TCCR3A&=~((1<<COM3A1)|(1<<COM3A0));
                digitalWrite(5,LOW);
            }
            else{
                TCCR3A|=(1<<COM3A0);
            }
            break;
        case 4:
            if (onOff==OFF){
                TCCR4A&=~((1<<COM4A1)|(1<<COM4A0));
                digitalWrite(6,LOW);
            }
            else{
                TCCR4A|=(1<<COM4A0);
            }
            break;
        case 5:
            if (onOff==OFF){
                TCCR5A&=~((1<<COM5A1)|(1<<COM5A0));
                digitalWrite(38,LOW);
            }
            else{
                TCCR5A|=(1<<COM5A0);
            }
            break;
        default:
            break;
    }
}
uint32_t CTCTimer::getRunningFreq()
{
    return runningFreq;
}
CTCTimer::~CTCTimer()
{
}