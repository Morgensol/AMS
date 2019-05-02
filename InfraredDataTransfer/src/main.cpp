#include <Arduino.h>
#include "Timers/CTCTimer.hpp"
#include <string.h>
void setup() {
  // put your setup code here, to run once:
// TCCR1B  = 0b00001001;
// TCCR1A  = 0b01000000;
// OCR1A    = 665;
  // 	TCCR1B = (1 << WGM12); // Configure timer 1 for CTC mode 
	// TCCR1A = (1 << COM1A0); // Enable timer 1 Compare Output channel A in toggle mode 
	// OCR1A   = 124; // Set CTC compare value to 1KHz at 16MHz AVR clock, with a prescaler of 64 
	// TCCR1B |= ((1 << CS10) | (1 << CS11)); // Start timer at Fcpu/64 
  
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  CTCTimer a = CTCTimer(1,17000);
    char text[100];
    snprintf(text,100,"CMP %lu, freq %lu, prescale %i ",a.OCRn, a.runningFreq, a.prescale);
  // char bits[100];
  // snprintf(bits,100,"WGM0 = %i, WGM1 = %i, WGM2 = %i,WGM3 = %i, Com0 = %i, Com1 = %i, CS0 = %i, CS1 = %i, CS2 = %i",a.WGM0,a.WGM1,a.WGM2,a.WGM3,a.COMnX0,a.COMnX1,a.CSn0,a.CSn1,a.CSn2);
  while(1){
    /* code */
    a.togglePwm(ON);
    delay(5000);
    a.togglePwm(OFF);
    delay(5000);
    digitalWrite(11,0);
    Serial.write(text);
  }
  
}