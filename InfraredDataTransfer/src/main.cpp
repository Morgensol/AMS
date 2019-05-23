#include <Arduino.h>
#include "CTCTimer.hpp"
#include <string.h>
#include "IRTransmitter.hpp"
#include "myDelay.hpp"
#include "ManchesterDecoder.hpp"
#include "IRReceiver.hpp"
#include "ITDB02.hpp"
#define TEST
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
    #endif

    #ifdef RECIEVER
    IRReceiver reciever = IRReceiver(0, true);
    #endif

    #ifdef TEST
// Initialize the display
  ITDB02 display =ITDB02();
  //initFont();
	// All pixels white (background)
	display.FillRectangle(0,0,320,240,31,63,31);
	// // Draw red parts of danish flag
	// display.FillRectangle(0,140,100,100,31,0,0);
	// display.FillRectangle(0,0,100,100,31,0,0);
	// display.FillRectangle(140,0,320-140,100,31,0,0);
	// display.FillRectangle(140,140,320-140,100,31,0,0);
  display.DisplayOn();
  uint32_t fwidth=0;
  char c[100];
  c[0]='H';
  c[1]='E';
  c[2]='S';
  c[3]='J';
  c[4]='M';
  c[5]='E';
  c[6]='D';
  c[7]='A';
  c[8]='\n';
  c[9]='E';
  // for (size_t i = 0; i < 26; i++)
  // {
  //   display.drawASCII(TimesNewRomanFont['A'+i],fwidth,0);
  //   fwidth+=TimesNewRomanFont['A'+i]->width;
  //   snprintf(c,100,"%c width = %u\n\r",'A'+i,TimesNewRomanFont['A'+i]->width);
  //   Serial.write(c);
    
  //   usleep(100);
  // }
  display.drawString(c,10);
  
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
    msleep(150+length);
    #endif

    #ifdef RECIEVER
    reciever.Receive();
    #endif

    #ifdef TEST
    // char c[100];
    // snprintf(c,100,"width:%u length:%u \n\r",TimesNewRomanFont['s']->width,TimesNewRomanFont[' ']->height);
    // Serial.write(c);
    msleep(1000);
    #endif
  }
  
}