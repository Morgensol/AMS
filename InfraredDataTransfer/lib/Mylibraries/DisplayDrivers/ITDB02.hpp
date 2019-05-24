
#include "TimesNewRoman.hpp"

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
    uint16_t CurrentRow;
    uint16_t CurrentCol;
    struct HLine DrawnLines[12];

    void handleEndLine();
    int characterWidth(char);
    int characterHeigth(char);
    bool tooManyCharacters(char);
    bool tooManyLines(char);
    void resetCurser(void);
    void newLine(char);
    void resetLine(void);
    void resetLines(void);
    void addNewCharacter(char, int);
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
    void VerticalScroll(uint16_t TFA, uint16_t VSA, uint16_t BFA);
    void VerticalScrollStartAdress(uint16_t VSP);
    void FillRectangle(uint16_t StartX, uint16_t StartY, uint16_t Width, uint16_t Height, uint8_t Red, uint8_t Green, uint8_t Blue);
    void drawASCII(ASCII* character,uint16_t StartX, uint16_t StartY);
    void drawString(char* string, uint16_t length);
    void scrollText();
};
