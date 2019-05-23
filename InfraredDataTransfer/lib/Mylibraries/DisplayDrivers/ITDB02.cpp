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
	RST_PORT |= (1<<RST_BIT);
	_delay_ms(130); //Give it time to think before sending new commands
    //Specifies the interface for pixels and data
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
    if(length==0)
        return;
    for (size_t i = 0; i < length-1; i++)
    {
        if(string[i]=='\n')
        {
            if((CurrentRow+19)>VERTICAL_MAX-19)
            {
                CurrentRow=0;
                CurrentCol=0;
            }
            else
            {
                CurrentRow+=19;
                CurrentCol=0;
            }
        }
        else if(((CurrentCol+TimesNewRomanFont[(uint8_t)string[i]]->width) > HORIZONTAL_MAX-1))
        {
            if((CurrentRow+TimesNewRomanFont[(uint8_t)string[i]]->height)>(VERTICAL_MAX-19))
            {
                CurrentRow=0;
                CurrentCol=0;
            }
            else
            {
                CurrentRow+=TimesNewRomanFont[(uint8_t)string[i]]->height;
                CurrentCol=0;
            }
            drawASCII(TimesNewRomanFont[(uint8_t)string[i]],CurrentRow,CurrentCol);
            CurrentCol=TimesNewRomanFont[(uint8_t)string[i]]->width;
        } 
        else
        {
            drawASCII(TimesNewRomanFont[(uint8_t)string[i]],CurrentRow,CurrentCol);
            CurrentCol+=TimesNewRomanFont[(uint8_t)string[i]]->width;
        }
        

    }
    
}
