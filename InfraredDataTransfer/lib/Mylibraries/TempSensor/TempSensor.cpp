#include "TempSensor.hpp"

volatile uint8_t counter = 0;
volatile uint8_t dataCounter = 0;
volatile bool start = false;
volatile unsigned long timestamp[3];
volatile unsigned long startTime;
volatile unsigned long endTime;
volatile uint8_t data[80];
volatile uint16_t RHi;
volatile uint8_t RHd;
volatile uint16_t Tempi;
volatile uint8_t Tempd;
volatile uint8_t Parity;

ISR(INT1_vect)
{
    if(start)
    {
        endTime = micros();
        data[dataCounter++] = endTime-startTime;
        startTime = endTime;
        if(dataCounter == 80)
        {
            start = false;
            dataCounter = 0;
            Serial.write("data is ready");
        }
    }

    if(!start)
    {
        timestamp[counter++]=micros();
    }

    if(counter == 3 and !start)
    {
        if((timestamp[1]-timestamp[0])+(timestamp[2]-timestamp[1]) > 140 && 180 > (timestamp[2]-timestamp[1])+(timestamp[1]-timestamp[0]))
        {
            startTime = timestamp[2];
            start = true;
            counter = 0;
        }
        else
        {
            Serial.write("failed");
            counter = 0;
        }
    }
}

TempSensor::TempSensor()
{
    setup();
}

TempSensor::~TempSensor()
{
}

void TempSensor::setup(){
    cli();
    EICRA = 0b00000100;
    EIMSK |= 0b00000010;
    sei();
}

char* TempSensor::getTemp(){
    cli();
    DDRD |= 1<<1;
    PORTD &=~(1<<1);
    _delay_us(1200);
    PORTD |= 1<<1;
    _delay_us(35);
    DDRD &=~(1<<1);
    sei();
    _delay_ms(10);
    uint8_t inserter = 0; 
    for (size_t i = 8; i > 0; i--)
    {
        if(data[inserter]+data[inserter+1] > 70 && 90 > data[inserter]+data[inserter+1]){
            RHi &=~(1<<(i-1));
        }
        if(data[inserter]+data[inserter+1] > 110 && 145 > data[inserter]+data[inserter+1]){
            RHi |= (1<<(i-1));
        }
        inserter = inserter+2;
    }
    for (size_t i = 8; i > 0; i--)
    {
        if(data[inserter]+data[inserter+1] > 70 && 90 > data[inserter]+data[inserter+1]){
            RHd &=~(1<<(i-1));
        }
        if(data[inserter]+data[inserter+1] > 110 && 145 > data[inserter]+data[inserter+1]){
            RHd |= (1<<(i-1));
        }
        inserter = inserter+2;
    }
    for (size_t i = 8; i > 0; i--)
    {
        if(data[inserter]+data[inserter+1] > 70 && 90 > data[inserter]+data[inserter+1]){
            Tempi &=~(1<<(i-1));
        }
        if(data[inserter]+data[inserter+1] > 110 && 145 > data[inserter]+data[inserter+1]){
            Tempi |= (1<<(i-1));
        }
        inserter = inserter+2;
    }
    for (size_t i = 8; i > 0; i--)
    {
        if(data[inserter]+data[inserter+1] > 70 && 90 > data[inserter]+data[inserter+1]){
            Tempd &=~(1<<(i-1));
        }
        if(data[inserter]+data[inserter+1] > 110 && 145 > data[inserter]+data[inserter+1]){
            Tempd |= (1<<(i-1));
        }
        inserter = inserter+2;
    }
    for (size_t i = 8; i > 0; i--)
    {
        if(data[inserter]+data[inserter+1] > 70 && 90 > data[inserter]+data[inserter+1]){
            Parity &=~(1<<(i-1));
        }
        if(data[inserter]+data[inserter+1] > 100 && 145 > data[inserter]+data[inserter+1]){
            Parity |= (1<<(i-1));
        }
        inserter = inserter+2;
    }
    uint16_t paritycheck = RHi+RHd+Tempi+Tempd;

    if(paritycheck > 255)
    {
        for (size_t i = 15; i > 7; i--)
        {
            paritycheck &=~(1<<(i));
        }
    }

    counter = 0;

    if(paritycheck == Parity)
    {
        Serial.write("Success\n\r");
        double accurateTemp;
        int16_t signedTemp;
        uint16_t Temp = ((Tempi<<8)+Tempd);
        if(Tempi & (1<<15))
        {
            Tempi &=~(1<<(15));
            signedTemp = (int16_t)Temp*(-1);
        }
        else
        {
            signedTemp = (int16_t)Temp;
        }
        accurateTemp = signedTemp*0.1;
        snprintf(message, 20, "Temperatur = %.1f C  ", accurateTemp);
        Serial.write(message);
        return message;
    }
    else
    {
        snprintf(message, 20, "Paritycheck failed");
        return message;
    }
}

char* TempSensor::getHum(){
    cli();
    DDRD |= 1<<1;
    PORTD &=~(1<<1);
    _delay_us(1200);
    PORTD |= 1<<1;
    _delay_us(35);
    DDRD &=~(1<<1);
    sei();
    _delay_ms(10);
    uint8_t inserter = 0; 
    for (size_t i = 8; i > 0; i--)
    {
        if(data[inserter]+data[inserter+1] > 70 && 90 > data[inserter]+data[inserter+1]){
            RHi &=~(1<<(i-1));
        }
        if(data[inserter]+data[inserter+1] > 110 && 145 > data[inserter]+data[inserter+1]){
            RHi |= (1<<(i-1));
        }
        inserter = inserter+2;
    }
    for (size_t i = 8; i > 0; i--)
    {
        if(data[inserter]+data[inserter+1] > 70 && 90 > data[inserter]+data[inserter+1]){
            RHd &=~(1<<(i-1));
        }
        if(data[inserter]+data[inserter+1] > 110 && 145 > data[inserter]+data[inserter+1]){
            RHd |= (1<<(i-1));
        }
        inserter = inserter+2;
    }
    for (size_t i = 8; i > 0; i--)
    {
        if(data[inserter]+data[inserter+1] > 70 && 90 > data[inserter]+data[inserter+1]){
            Tempi &=~(1<<(i-1));
        }
        if(data[inserter]+data[inserter+1] > 110 && 145 > data[inserter]+data[inserter+1]){
            Tempi |= (1<<(i-1));
        }
        inserter = inserter+2;
    }
    for (size_t i = 8; i > 0; i--)
    {
        if(data[inserter]+data[inserter+1] > 70 && 90 > data[inserter]+data[inserter+1]){
            Tempd &=~(1<<(i-1));
        }
        if(data[inserter]+data[inserter+1] > 110 && 145 > data[inserter]+data[inserter+1]){
            Tempd |= (1<<(i-1));
        }
        inserter = inserter+2;
    }
    for (size_t i = 8; i > 0; i--)
    {
        if(data[inserter]+data[inserter+1] > 70 && 90 > data[inserter]+data[inserter+1]){
            Parity &=~(1<<(i-1));
        }
        if(data[inserter]+data[inserter+1] > 100 && 145 > data[inserter]+data[inserter+1]){
            Parity |= (1<<(i-1));
        }
        inserter = inserter+2;
    }
    uint16_t paritycheck = RHi+RHd+Tempi+Tempd;

    if(paritycheck > 255)
    {
        for (size_t i = 15; i > 7; i--)
        {
            paritycheck &=~(1<<(i));
        }
    }

    counter = 0;

    if(paritycheck == Parity)
    {
        Serial.write("Success\n\r");
        double accurateRH;
        int16_t signedRH;
        uint16_t RH = ((RHi<<8)+RHd);
        signedRH = (int16_t)RH;
        accurateRH = signedRH*0.1;
        snprintf(message, 20, "Humidity = %.1f %%  ", accurateRH);
        Serial.write(message);
        return message;
    }
    else
    {
        snprintf(message, 20, "Paritycheck failed");
        return message;
    }
}

