#ifndef ITDB02_H
#define ITDB02_H
#include "TimesNewRoman.hpp"
#include "Arduino.h"
#include <stdint.h>
#include "Color.hpp"
#include "Lines.hpp"
struct HLine{
    char string[21];
    uint8_t current_length=0;
};
class ITDB02
{
private:
    void WriteData(unsigned int data);
    void WriteCommand(unsigned int command);
    void SleepOut();
    uint16_t CurrentRow=0;
    uint16_t CurrentCol=0;
    Lines* Screen;
    uint8_t maxLines=12;
    uint8_t activeLines=0;
    int characterWidth(char);
    int characterHeigth(char);
    Line* getNextString(char* string, uint16_t maxLineLength, uint16_t startPos);
    void addLineToScreen(Line* lineToAdd);
    void drawScreen();
    void moveScreenLinesUp();
    void drawLine(uint16_t lineNmbr);
    Lines* splitString(char* string, uint16_t length);
public:
    ITDB02();
    ~ITDB02();
    void legacyWriteString(char* string, uint16_t length);
    void DisplayOff();
    void DisplayOn();
    void MemoryAccessControl(uint8_t parameter);
    void InterfacePixelFormat(uint8_t parameter);
    void WritePixel(uint8_t Red, uint8_t Blue, uint8_t Green);
    void SetColumnAddress(uint16_t Start, uint16_t End);
    void SetPageAddress(uint16_t Start, uint16_t End);
    void MemoryWrite();
    void VerticalScroll(uint16_t TFA, uint16_t VSA, uint16_t BFA);
    void VerticalScrollStartAdress(uint16_t VSP);
    void FillRectangle(uint16_t StartX, uint16_t StartY, uint16_t Width, uint16_t Height, Color rgb);
    void drawASCII(ASCII* character,uint16_t StartX, uint16_t StartY);
    void drawString(char* string, uint16_t length);

};
#endif