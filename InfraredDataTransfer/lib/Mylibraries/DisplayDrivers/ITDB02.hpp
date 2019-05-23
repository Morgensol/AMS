
#include "TimesNewRoman.hpp"

class ITDB02
{
private:
    void WriteData(unsigned int data);
    void WriteCommand(unsigned int command);
    void SleepOut();
    uint8_t CurrentRow;
    uint8_t CurrentCol;
public:
    ITDB02();
    ~ITDB02();
    void DisplayOff();
    void DisplayOn();
    void MemoryAccessControl(unsigned char parameter);
    void InterfacePixelFormat(unsigned char parameter);
    void WritePixel(unsigned char Red, unsigned char Blue, unsigned char Green);
    void SetColumnAddress(unsigned int Start, unsigned int End);
    void SetPageAddress(unsigned int Start, unsigned int End);
    void MemoryWrite();
    void FillRectangle(unsigned int StartX, unsigned int StartY, unsigned int Width, 
                    unsigned int Height, unsigned char Red, unsigned char Green, unsigned char Blue);
    void drawBMP(uint8_t* BMP);
    void drawASCII(ASCII* character,unsigned int StartX, unsigned int StartY);
    void drawString(char* string, uint8_t length);
};
