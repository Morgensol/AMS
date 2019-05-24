#include "IRReceiver.hpp"
#include "avr/interrupt.h"

volatile bool trig = false;
volatile unsigned long timestamp[3];
volatile uint8_t buffcnt = 0; 
volatile uint8_t counter = 0;
volatile unsigned long T = 0;
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
        T = timestamp[2]-timestamp[1];
        // char c[100];
        // snprintf(c,100,"T: %lu treT: %lu timestamp1: %lu timestamp2: %lu timestamp3: %lu",T,treT, timestamp[0],timestamp[1], timestamp[2]);
        // Serial.write(c);
        if((double)T*3>(double)treT*0.85 && (double)treT*1.15>(double)T*3){
            trig = true; 
            T = (T+treT)>>1;
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
        if((double)current-(double)previous > 0.80f*(double)T && 1.20f*(double)T > (double)current-(double)previous)
        {
            if((PIND&(1<<PIND0)))
            {
                buff+=1<<buffcnt;
            }
            previous = current;
            buffcnt++;
        }
        else if((double)current-(double)previous > 2.0f*(double)T)
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

ISR(INT2_vect)
{
    Serial.write("Not configured to receive data \n\r");
}

ISR(INT3_vect)
{
    Serial.write("Not configured to receive data \n\r");
}

ISR(INT4_vect)
{
    Serial.write("Not configured to receive data \n\r");
}

ISR(INT5_vect)
{
    Serial.write("Not configured to receive data \n\r");
}

ISR(INT6_vect)
{
    Serial.write("Not configured to receive data \n\r");
}

ISR(INT7_vect)
{
    Serial.write("Not configured to receive data \n\r");
}

IRReceiver::IRReceiver(int inter, bool setup){
    if(setup){
        cli();
        setupInterrupt(inter);
        TCCR3A = 0;
        TCCR3B = 0;
        TCNT3 = 3;
        OCR3A = 1561;
        TCCR3B |= (1<< WGM32);
        TCCR3B |= (1<<CS32) | (1<<CS30);
        TIMSK3 |= (1<<OCIE3A);
        sei();
    }
}

IRReceiver::~IRReceiver(){
    
}

void IRReceiver::Receive(){
    while(!trig)
    { 
    }   
    uint8_t checksum[4];
    uint8_t length[4];
    volatile uint8_t lengthCheck = 0;
    while (lengthCheck<4)
    {
        if(buffcnt == 8){
            checksum[lengthCheck] = buff;
            lengthCheck++;
            buff = 0;
            buffcnt = 0;
        }
    }

    lengthCheck = 0;
    while (lengthCheck<4)
    {
        if(buffcnt == 8){
            length[lengthCheck] = buff;
            lengthCheck++;
            buff = 0;
            buffcnt = 0;
        }
    }

    uint32_t arrayLen=0;
    arrayLen=length[0]+((uint32_t)length[1]<<8)+((uint32_t)length[2]<<16)+((uint32_t)length[3]<<24);

    

    volatile uint8_t array[arrayLen];
    volatile uint32_t index = 0;
    uint32_t Cmpchecksum =0;
    while(trig){
        if(buffcnt == 8)
        {
            array[index]=buff;
            Cmpchecksum += array[index];
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
        snprintf(d, 100, "Length = %lu \n\r" ,arrayLen);
        Serial.write(d);
        snprintf(d, 100, "Checksum = %lu \n\r" ,arrayChecksum);
        Serial.write(d);
        snprintf(d, 100, "CmpChecksum = %lu \n\r" ,Cmpchecksum);
        Serial.write(d);
        cli();
        for (size_t i = 0; i < arrayLen; i++)
        {
            snprintf(d, 100, "%c" ,array[i]);
            Serial.write(d);
        }
        Serial.write("\n\r");
        sei();
    }
}

void IRReceiver::setFrequence(int val){
    frequence = val;
}

int IRReceiver::getFrequence(){
    return frequence;
}

void IRReceiver::setupInterrupt(int inter){
    if(inter == 0){
        EICRA = 0b00000001;
        EIMSK |= 0b00000001;
    }
    else if(inter == 2){
        EICRA = 0b00010000;
        EIMSK |= 0b00000100;
    }
    else if(inter == 3){
        EICRA = 0b01000000;
        EIMSK |= 0b00001000;
    }
    else if(inter == 4){
        EICRB = 0b00000001;
        EIMSK |= 0b00010000;
    }
    else if(inter == 5){
        EICRB = 0b00000100;
        EIMSK |= 0b00100000;
    }
    else if(inter == 6){
        EICRB = 0b00010000;
        EIMSK |= 0b01000000;
    }
    else if(inter == 7){
        EICRB = 0b01000000;
        EIMSK |= 0b10000000;
    }
}
