#include "Arduino.h"
#include "ITDB02.hpp"
#include <stdint.h>

// Data port definitions:
#define DATA_PORT_HIGH PORTA
#define DATA_PORT_LOW  PORTC

// Interface definitions
#define WR_PORT PORTG
#define WR_BIT 2
#define WR_DDR DDRG
#define DC_PORT PORTD
#define DC_BIT  7  //AKA RS
#define DC_DDR DDRD
#define CS_PORT PORTG
#define CS_BIT  1
#define CS_DDR DDRG
#define RST_PORT PORTG
#define RST_BIT 0
#define RST_DDR DDRG
#define HORIZONTAL_MAX 320u
#define VERTICAL_MAX 240u

// ILI 9341 data sheet, page 238
void ITDB02::WriteCommand(uint16_t command)
{
	DATA_PORT_LOW = command;	
	DC_PORT &= ~(1<<DC_BIT);
	CS_PORT &= ~(1<<CS_BIT);
	WR_PORT &= ~(1<<WR_BIT);
	_NOP(); //wait 60ns
	WR_PORT |= (1<<WR_BIT);
	CS_PORT |= (1<<CS_BIT);
	_NOP(); //Wait 60ns
}

// ILI 9341 data sheet, page 238
void ITDB02::WriteData(uint16_t data)
{
	DATA_PORT_HIGH = (data>>8);
	DATA_PORT_LOW = (data);
	DC_PORT |= (1<<DC_BIT);
	CS_PORT &= ~(1<<CS_BIT);
	WR_PORT &= ~(1<<WR_BIT);
	_NOP(); //Wait 60ns
	WR_PORT |= (1<<WR_BIT);
	CS_PORT |= (1<<CS_BIT);
	_NOP(); // Wait 60ns
}
ITDB02::ITDB02()
{
    WR_DDR |= (1<<WR_BIT);
	CS_DDR |= (1<<CS_BIT);
	RST_DDR |= (1<<RST_BIT);
	DC_DDR |= (1<<DC_BIT);
	DDRA = 0xFF,
	DDRC = 0xFF;
	RST_PORT &= ~(1<<RST_BIT);
    _delay_ms(500);
	RST_PORT |= (1<<RST_BIT);
	_delay_ms(130); //Give it time to think before sending new commands
    //Specifies the interface for pixels and data

    SleepOut();
	MemoryAccessControl(0b00001000);
	InterfacePixelFormat(0b00000101);
    CurrentCol=0;
    CurrentRow=0;
    FillRectangle(0,0,320,240,31,63,31); //Make a white background
    DisplayOn(); //Turn On display
}

void ITDB02::DisplayOff()
{
	WriteCommand(0b00101000);
}

void ITDB02::DisplayOn()
{
	WriteCommand(0b00101001);
}

void ITDB02::SleepOut()
{
	WriteCommand(0b00010001);
    _delay_ms(5); //Wait 5ms after (p. 100 in datasheet)
}

void ITDB02::MemoryAccessControl(uint8_t parameter)
{
	WriteCommand(0b00110110);
	WriteData(parameter);
}

void ITDB02::InterfacePixelFormat(uint8_t parameter)
{
	WriteCommand(0b00111010);
	WriteData(parameter);
}

void ITDB02::MemoryWrite()
{
	WriteCommand(0b00101100);
}

// Red 0-31, Green 0-63, Blue 0-31
void ITDB02::WritePixel(uint8_t Red, uint8_t Green, uint8_t Blue)
{
	if((Red < 32) && (Green < 64) && (Blue <32))
	{
		WriteData((uint16_t)Red<<11 | (uint16_t)Green<<5 | (uint16_t)Blue);		
	}
}

// Set Column Address (0-239), Start > End
void ITDB02::SetColumnAddress(uint16_t Start, uint16_t End)
{
	WriteCommand(0b00101010);
	WriteData(Start>>8);
	WriteData(Start);
	WriteData(End>>8);
	WriteData(End);
}

// Set Page Address (0-319), Start > End
void ITDB02::SetPageAddress(uint16_t Start, uint16_t End)
{
	WriteCommand(0b00101011);
	WriteData(Start>>8);
	WriteData(Start);
	WriteData(End>>8);
	WriteData(End);
}

// Fills rectangle with specified color
// (StartX,StartY) = Upper left corner. X horizontal (0-319) , Y vertical (0-239).
// Height (1-240) is vertical. Width (1-320) is horizontal.
// R-G-B = 5-6-5 bits.
void ITDB02::FillRectangle(uint16_t StartX, uint16_t StartY, uint16_t Width, uint16_t Height, uint8_t Red, uint8_t Green, uint8_t Blue)
{
	SetColumnAddress(StartY, StartY+Height-1);
	SetPageAddress(StartX, StartX+Width-1);
	MemoryWrite();
		
	for(uint32_t t = 0; t < (uint32_t)Width * (uint32_t)Height; t++)
	{
		WritePixel(Red, Green, Blue);
	}
}
void ITDB02::drawASCII(ASCII* character,uint16_t StartX, uint16_t StartY)
{
    SetColumnAddress(StartX, StartX+character->height-1);
	SetPageAddress(StartY, StartY+character->width-1);
	MemoryWrite();
    //Drawing the first bit of each array, then the second and so forth
    for (size_t column = 0; column < character->width; column++)
    {  
        for (size_t pixel = 0; pixel < character->height; pixel++)
        {
            if ((character->map[pixel]&(1<<((character->width-1)-column))))
            {
                WritePixel(0,0,0); //black
            }
            else
            {
                WritePixel(31,63,31); //white
            }     
        }
        
    }
    
    
}

void ITDB02::drawString(char* string, uint16_t length)
{
    for (size_t i = 0; i < length-1; i++)   //Why -1?
    {
        
        if(string[i]=='\n')
            handleEndLine();

        else if(tooManyCharacters(string[i]))
        {
            if(tooManyLines(string[i]))
                resetCurser();
            else
                newLine(string[i]); //The way we calculate a new line, if a more complicated program is written, will need to be calculated from the largest character in that line
            
            addNewCharacter(string[i], i);
            CurrentCol += characterWidth(string[i]);
        } 
        else
        {
            addNewCharacter(string[i], i);
            CurrentCol += characterWidth(string[i]);
        }
    }
}

void ITDB02::handleEndLine()
{
    if(tooManyLines('A'))
        resetCurser();
    else
        newLine('A');
}

int ITDB02::characterWidth(char character)
{
    return TimesNewRomanFont[(uint8_t)character]->width;
}

int ITDB02::characterHeigth(char character)
{
    return TimesNewRomanFont[(uint8_t)character]->height;
}

bool ITDB02::tooManyCharacters(char character)
{
    return CurrentCol+characterWidth(character) >= HORIZONTAL_MAX;
}

bool ITDB02::tooManyLines(char character)
{
    return CurrentRow+characterHeigth(character) > (VERTICAL_MAX-19);
}

void ITDB02::resetCurser()
{
    resetLines();
    resetLine();
}

void ITDB02::newLine(char character)
{
    CurrentRow += characterHeigth(character);
    resetLine();
}

void ITDB02::resetLines()
{
    CurrentRow=0;
}

void ITDB02::resetLine()
{
    CurrentCol=0;
}

void ITDB02::addNewCharacter(char character, int index)
{
    drawASCII(TimesNewRomanFont[(uint8_t)character],CurrentRow,CurrentCol);
    DrawnLines[CurrentRow/19].string[index] = character;
    DrawnLines[CurrentRow/19].current_length++;
}

void ITDB02::scrollText()
{
    FillRectangle(0,0,320,240,31,63,31);
    CurrentCol=0;
    CurrentRow=0;
    for (size_t lines = 0; lines < 12; lines++)
    {
        for (size_t character = 0; character < DrawnLines[lines].current_length; character++)
        {
            drawASCII(TimesNewRomanFont[(uint8_t)DrawnLines[lines].string[character]],CurrentRow,CurrentCol);
            CurrentCol+=TimesNewRomanFont[(uint8_t)DrawnLines[lines].string[character]]->width;
        }
        
    }
    
}
