#include "IRReceiver.hpp"
#include "avr/interrupt.h"

volatile bool trig = false;
unsigned long timestamp[3];
volatile uint8_t buffcnt = 0; 
uint8_t counter = 0;
unsigned long T = 0;
unsigned long current;
unsigned long previous;
volatile uint8_t buff = 0;

ISR(INT0_vect)
{
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
        }
        else{
            counter = 0;
            Serial.write('G');
        }
    }

    if(trig){
        current = micros();
        if((double)current-(double)previous > 0.6f*(double)T)
        {
            if((PIND&(1<<PIND0)))
            {
                buff+=1<<buffcnt;
            }
            previous = current;
            buffcnt++;
        }
    }
}

ISR(INT1_vect)
{
    Serial.write('A');
}

ISR(INT2_vect)
{
    Serial.write('A');
}

ISR(INT3_vect)
{
    Serial.write('A');
}

ISR(INT4_vect)
{
    Serial.write('A');
}

ISR(INT5_vect)
{
    Serial.write('A');
}

ISR(INT6_vect)
{
    Serial.write('A');
}

ISR(INT7_vect)
{
    Serial.write('A');
}

IRReceiver::IRReceiver(int inter, bool setup){
    if(setup){
    setupInterrupt(inter);
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



    char d[100];
    snprintf(d, 100, "Length %lu" ,arrayLen);
    Serial.write(d);

    for (size_t i = 0; i < 4; i++)
    {
        snprintf(d, 100, "length[%i] = 0x%x", i, length[i]);
        Serial.write(d);
    }

    uint8_t array[arrayLen];
    uint8_t index = 0;
    while(trig){
        if(buffcnt == 8)
        {
            array[index]=buff;
            index++;
            buffcnt = 0;
            buff = 0;
        }
        if(index == arrayLen){
            trig = false;
        }
    }

    for (size_t i = 0; i < arrayLen; i++)
    {
        snprintf(d, 100, "array[%i] = 0x%x", i, array[i]);
        Serial.write(d);
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
    else if(inter == 1){
        EICRA = 0b00000100;
        EIMSK |= 0b00000010;
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
