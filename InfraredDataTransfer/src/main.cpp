#include <Arduino.h>
#include "CTCTimer.hpp"
#include <string.h>
#include "IRTransmitter.hpp"
#include "myDelay.hpp"
#include "ManchesterDecoder.hpp"
#include "IRReceiver.hpp"
#include "ITDB02.hpp"
#include "TempSensor.hpp"
#define RECIEVER
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(10000);
  setupTimesNewRoman();
}

void loop() {
  //Data setup
  
  #ifdef SENDER
  BaseTimer* a = new CTCTimer(1,38000);
  IRTransmitter transmitter = IRTransmitter(new ManchesterEncoder(),a);
  TempSensor temp =TempSensor();
  char data[1024];
  uint32_t length=0;
  #endif

  #ifdef RECIEVER
  IRReceiver reciever = IRReceiver(0, true);
  ITDB02 display= ITDB02();
  #endif
  // Loop to run infinetly
  while(1){
    #ifdef SENDER
    Serial.write("Enter Message and press enter to send: \n\r");
    do{
      length = Serial.readBytesUntil('\n',data,1024);
    }
    while(length==0);
    if(data[0]=='T' && data[1]=='E' && data[2]=='M' && data[3]=='P')
    {
      snprintf(data,20,"%s",temp.getTemp());
      length=20;
    }
    else if(data[0]=='H' && data[1]=='U' && data[2]=='M')
    {
      snprintf(data,20,"%s",temp.getHum());
      length=20;
    }
      Serial.write("Sending...\n\r");
      transmitter.sendData((uint8_t*)data,length);
      Serial.write("Data send!\n\r");
      msleep(150+length);
    #endif

    #ifdef RECIEVER
    IRReturnData ReadData= reciever.Receive();
    display.drawString((char*)"Recieved Message:\n",sizeof("Recieved Message:\n"));
    display.drawString(ReadData.streng,ReadData.length);
    delete[] ReadData.streng;
    #endif
  }
  
}