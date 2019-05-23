
#include "TimesNewRoman.hpp"

class ITDB02
{
private:
    void WriteData(unsigned int data);
    void WriteCommand(unsigned int command);
    void SleepOut();
    uint16_t CurrentRow;
    uint16_t CurrentCol;
public:
    ITDB02();
    ~ITDB02();
    void DisplayOff();
    void DisplayOn();
    void MemoryAccessControl(uint8_t parameter);
    void InterfacePixelFormat(uint8_t parameter);
    void WritePixel(uint8_t Red, uint8_t Blue, uint8_t Green);
    void SetColumnAddress(uint16_t Start, uint16_t End);
    void SetPageAddress(uint16_t Start, uint16_t End);
    void MemoryWrite();
    void FillRectangle(uint16_t StartX, uint16_t StartY, uint16_t Width, uint16_t Height, uint8_t Red, uint8_t Green, uint8_t Blue);
    void drawASCII(ASCII* character,uint16_t StartX, uint16_t StartY);
    void drawString(char* string, uint16_t length);
};
