#include "IRReceiver.hpp"
#include "avr/interrupt.h"

volatile bool trig = false;
volatile unsigned long timestamp[3];
volatile uint8_t buffcnt = 0; 
volatile uint8_t counter = 0;
volatile unsigned long Time = 0;
volatile unsigned long current;
volatile unsigned long previous;
volatile uint8_t buff = 0;

ISR(INT0_vect)
{
    TCNT3=0;
    if(!trig)
    {
        timestamp[counter++] = micros(); 
    }

    if(counter ==3 and !trig)
    {
        unsigned long treT = timestamp[1]-timestamp[0];
        Time = timestamp[2]-timestamp[1];
        if((double)Time*3>(double)treT*0.85 && (double)treT*1.15>(double)Time*3){
            trig = true; 
            Time = (Time+treT)>>1;
            previous = timestamp[2];
            counter = 0;
            TIMSK3 = 0;
        }
        else{
            counter = 0;
            Serial.write("Timing failed \n\r");
        }
    }

    if(trig){
        current = micros();
        if((double)current-(double)previous > 0.80f*(double)Time && 1.20f*(double)Time > (double)current-(double)previous)
        {
            if((PIND&(1<<PIND0)))
            {
                buff+=1<<buffcnt;
            }
            previous = current;
            buffcnt++;
        }
        else if((double)current-(double)previous > 2.0f*(double)Time)
        {
            trig = false;
            Serial.write("Data reading Failed \n\r");
        }
    }
}

ISR(TIMER3_COMPA_vect)
{
    counter = 0;
}

IRReceiver::IRReceiver(){
    cli();
    setupInterrupt();
    sei();
}

IRReceiver::~IRReceiver(){
    
}
uint32_t IRReceiver::readBuffer(uint8_t* output, uint32_t index)
{
    output[index]=buff;
    buff=0;
    buffcnt=0;
    return 1;
}
uint32_t IRReceiver::calculateReadLength(uint8_t* array, uint32_t nmbrOfBytes)
{
    uint32_t calculatedLength=0;
    for (uint32_t byte = 0; byte < nmbrOfBytes; byte++)
    {
        calculatedLength+=(uint32_t)array[byte]<<(byte*8);
    }
    return calculatedLength;
}
IRReturnData IRReceiver::Receive(){
    uint8_t checksum[4];
    uint8_t length[4];
    volatile uint8_t lengthCheck = 0;

    while(!trig); //Wait here untill the data sequence is started

    while (lengthCheck<4) //Read checksum
        if(buffcnt == 8) lengthCheck += readBuffer(checksum,lengthCheck);

    lengthCheck = 0;
    
    while (lengthCheck<4) //Read length
        if(buffcnt == 8) lengthCheck += readBuffer(length,lengthCheck);

    uint32_t arrayLen=calculateReadLength(length,4);

    IRReturnData ret = {.length = arrayLen, .streng = new volatile char[arrayLen]};
    
    volatile uint32_t index = 0;
    uint32_t Cmpchecksum =0;
    while(trig){
        if(buffcnt == 8)
        {
            ret.streng[index]=buff;
            Cmpchecksum += ret.streng[index];
            index++;
            buffcnt = 0;
            buff = 0;
        }
        if(index == arrayLen){
            trig = false;
            counter=0;
        }
    }
    TIMSK3 |= (1<<OCIE3A);

    uint32_t arrayChecksum=0;
    arrayChecksum=checksum[0]+((uint32_t)checksum[1]<<8)+((uint32_t)checksum[2]<<16)+((uint32_t)checksum[3]<<24);

    if(arrayChecksum != Cmpchecksum)
    {
        Serial.write("Data corrupted");
    }

    else{
        char d[100];
        cli();
        for (size_t i = 0; i < arrayLen; i++)
        {
            snprintf(d, 100, "%c" ,ret.streng[i]);
            Serial.write(d);
        }
        Serial.write("\n\r");

        sei();
    }
    return ret;
}
void IRReceiver::setupInterrupt(){
    EICRA = 0b00000001;
    EIMSK |= 0b00000001;
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3 = 3;
    OCR3A = 1561;
    TCCR3B |= (1<< WGM32);
    TCCR3B |= (1<<CS32) | (1<<CS30);
    TIMSK3 |= (1<<OCIE3A);
}
