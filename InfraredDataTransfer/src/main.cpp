#include <Arduino.h>
#include "PWM.h"
void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(52,INPUT);
  PWM(1,CTC,10000);

  

  TCCR0A=0b01000010;
  TCCR0B=0b00000010;
  OCR0A=99;
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  while(1){
    /* code */
    if(digitalRead(52)==HIGH)
    {
        Serial.write("Detected");
    }
    
  }
  
}