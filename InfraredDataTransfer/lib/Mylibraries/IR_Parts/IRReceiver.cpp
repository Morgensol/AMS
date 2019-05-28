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
        timestamp[counter++] = micros(); 

    if(counter ==3 and !trig)
    {
        unsigned long treT = timestamp[1]-timestamp[0];
        Time = timestamp[2]-timestamp[1];
        if((double)Time*3>(double)treT*0.80 && (double)treT*1.20>(double)Time*3){
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
                buff+=1<<buffcnt;
            
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
    trig=false;
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
uint32_t IRReceiver::calculateOriginal(uint8_t* array, uint32_t nmbrOfBytes)
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
    uint8_t index = 0;

    while(!trig); //Wait here untill the data sequence is started

    while (index<4 && trig) //Read checksum
        if(buffcnt == 8) index += readBuffer(checksum,index);

    index = 0;
    
    while (index<4 && trig) //Read length
        if(buffcnt == 8) index += readBuffer(length,index);

    index = 0;
    uint32_t arrayLen=calculateOriginal(length,4);

    IRReturnData ret = {.length = arrayLen, .streng = new uint8_t[arrayLen]};

    uint32_t Cmpchecksum =0;
    while(trig){
        if(buffcnt == 8)
        {
            readBuffer(ret.streng,index);
            Cmpchecksum += ret.streng[index];
            index++;
        }
        if(index == arrayLen){ //Once the length is reached, end reading
            trig = false;
            counter=0;
        }
    }

    TIMSK3 |= (1<<OCIE3A);  //Disable the timer that resets the reading process
    
    uint32_t arrayChecksum = calculateOriginal(checksum,4);

    if(arrayChecksum != Cmpchecksum) //Check if data is corrupt
    {
        Serial.write("Data corrupted");
        snprintf((char*)ret.streng,sizeof("Data corrupted"),"Data corrupted");
        ret.length=sizeof("Data corrupted");
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
