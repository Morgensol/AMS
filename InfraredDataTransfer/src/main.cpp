#include <Arduino.h>
#include "CTCTimer.hpp"
#include <string.h>
#include "IRTransmitter.hpp"
#include "myDelay.hpp"
#include "ManchesterDecoder.hpp"
#include "IRReceiver.hpp"
#define SENDER
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(10000);
}

void loop() {
  // put your main code here, to run repeatedly:
    #ifdef SENDER
    BaseTimer* a = new CTCTimer(1,38000);
    IRTransmitter transmitter = IRTransmitter(new ManchesterEncoder(),a);
    uint8_t data[1024];
    uint32_t length=0;
    #elif RECIEVER
    IRReceiver reciever = IRReceiver(0, true);
    #endif
  while(1){
    #ifdef SENDER
    Serial.write("Enter Message and press enter to send: \n\r");
    do{
      length = Serial.readBytesUntil('\n',data,1024);
    }
    while(length==0);
    Serial.write("Sending...\n\r");
    transmitter.sendData(data,length);
    Serial.write("Data send!\n\r");
    #elif RECIEVER
    reciever.Receive();
    #endif
  }
  
}