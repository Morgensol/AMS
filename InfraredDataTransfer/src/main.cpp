#include <Arduino.h>
#include "Timers/CTCTimer.hpp"
#include <string.h>
#include "IR_Parts/IRTransmitter.hpp"
#include "myDelay.hpp"
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
 BaseTimer* a = new CTCTimer(1,1000);
  uint8_t data[1]={1};
  IRTransmitter b = IRTransmitter(new ManchesterEncoder(),a);
  char c[100];

  while(1){
    b.sendData(data,1);
    _delay_ms(500);
    
    Serial.write(c);
  }
  
}