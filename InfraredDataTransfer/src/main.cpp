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
  // Serial.setTimeout(10000);
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
	// // Draw red parts of danish flag
	// display.FillRectangle(0,140,100,100,31,0,0);
	// display.FillRectangle(0,0,100,100,31,0,0);
	// display.FillRectangle(140,0,320-140,100,31,0,0);
	// display.FillRectangle(140,140,320-140,100,31,0,0);
  // uint32_t fwidth=0;
  char c[]={"Okay her er en histore om en dreng der hedder quang, han er 7 aar bor i tailand han staar tidligt op om morgenen og gaar meget sent i seng, for quang har nok at se til skoent han bare er en dreng, hver morgen kl kvart i 5 gaar quang alene ned\n"};
  // for (size_t i = 0; i < 26; i++)
  // {
  //   display.drawASCII(TimesNewRomanFont['a'+i],fwidth,0);
  //   fwidth+=TimesNewRomanFont['a'+i]->width;
  //   snprintf(c,100,"%c width = %u\n\r",'a'+i,TimesNewRomanFont['a'+i]->width);
  //   Serial.write(c);
    
  //   usleep(100);
  // }
  // display.drawASCII(TimesNewRomanFont['a'],0,0);
  // display.drawASCII(TimesNewRomanFont['b'],0,311);
  // display.drawASCII(TimesNewRomanFont['c'],220,0);
  // display.drawASCII(TimesNewRomanFont['d'],220,311);
  display.drawString(c,sizeof(c));
   display.drawString(c,sizeof(c));
   display.drawString(c,sizeof(c));
  // display.scrollText();
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
  // put your main code here, to run repeatedly:
  //   #ifdef SENDER
  //   BaseTimer* a = new CTCTimer(1,38000);
  //   IRTransmitter transmitter = IRTransmitter(new ManchesterEncoder(),a);
  //   uint8_t data[1024];
  //   uint32_t length=0;
  //   #endif
  //   #ifdef RECEIVER
  //   IRReceiver reciever = IRReceiver(0, true);
  //   #endif
  // while(1){
  //   #ifdef SENDER
  //   Serial.write("Enter Message and press enter to send: \n\r");
  //   do{
  //     length = Serial.readBytesUntil('\n',data,1024);
  //   }
  //   while(length==0);
  //   Serial.write("Sending...\n\r");
  //   transmitter.sendData(data,length);
  //   Serial.write("Data send!\n\r");
  //   #endif
  //   #ifdef RECEIVER
  //   reciever.Receive();
  //   #endif
  // }
  
}