/*
 * TFTdriver.c
 *
 * Created: 14-02-2019 09:33:09
 *  Author: frederik
 */ 

#include "Arduino.h"
#include "ITDB02.hpp"
#include <stdint.h>

// Data port definitions:
#define DATA_PORT_HIGH PORTA
#define DATA_PORT_LOW  PORTC

// Control port definitions:
#define WR_PORT PORTG
#define WR_BIT 2
#define WR_DDR DDRG
#define DC_PORT PORTD
#define DC_BIT  7  //"DC" signal is at the shield called RS
#define DC_DDR DDRD
#define CS_PORT PORTG
#define CS_BIT  1
#define CS_DDR DDRG
#define RST_PORT PORTG
#define RST_BIT 0
#define RST_DDR DDRG
#define HORIZONTAL_MAX 320
#define VERTICAL_MAX 240

// LOCAL FUNCTIONS /////////////////////////////////////////////////////////////

// ILI 9341 data sheet, page 238
void ITDB02::WriteCommand(unsigned int command)
{
	DATA_PORT_LOW = (command);	
	DC_PORT &= ~(1<<DC_BIT);
	CS_PORT &= ~(1<<CS_BIT);
	WR_PORT &= ~(1<<WR_BIT);
	_NOP();
	WR_PORT |= (1<<WR_BIT);
	CS_PORT |= (1<<CS_BIT);
	_NOP();
}

// ILI 9341 data sheet, page 238
void ITDB02::WriteData(unsigned int data)
{
	DATA_PORT_HIGH = (data>>8);
	DATA_PORT_LOW = (data);
	DC_PORT |= (1<<DC_BIT);
	CS_PORT &= ~(1<<CS_BIT);
	WR_PORT &= ~(1<<WR_BIT);
	_NOP();
	WR_PORT |= (1<<WR_BIT);
	CS_PORT |= (1<<CS_BIT);
	_NOP();
}
ITDB02::ITDB02()
{
    	WR_DDR |= (1<<WR_BIT);
	CS_DDR |= (1<<CS_BIT);
	RST_DDR |= (1<<RST_BIT);
	DC_DDR |= (1<<DC_BIT);
	DDRA = 0xFF,
	DDRC = 0xFF;
	_delay_ms(1000);
	RST_PORT &= ~(1<<RST_BIT);
	_delay_ms(500);
	RST_PORT |= (1<<RST_BIT);
	_delay_ms(130);
	
	MemoryAccessControl(0b00001000);
	_delay_ms(100);
	InterfacePixelFormat(0b00000101);
	_delay_ms(200);
	SleepOut();
	_delay_ms(100);
    CurrentCol=0;
    CurrentRow=0;
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
}

void ITDB02::MemoryAccessControl(unsigned char parameter)
{
	WriteCommand(0b00110110);
	WriteData(parameter);
}

void ITDB02::InterfacePixelFormat(unsigned char parameter)
{
	WriteCommand(0b00111010);
	WriteData(parameter);
}

void ITDB02::MemoryWrite()
{
	WriteCommand(0b00101100);
}

// Red 0-31, Green 0-63, Blue 0-31
void ITDB02::WritePixel(unsigned char Red, unsigned char Green, unsigned char Blue)
{
	if((Red < 32) && (Green < 64) && (Blue <32))
	{
		WriteData((unsigned int)Red<<11 | (unsigned int)Green<<5 | (unsigned int)Blue);		
	}
}

// Set Column Address (0-239), Start > End
void ITDB02::SetColumnAddress(unsigned int Start, unsigned int End)
{
	WriteCommand(0b00101010);
	WriteData(Start>>8);
	WriteData(Start);
	WriteData(End>>8);
	WriteData(End);
}

// Set Page Address (0-319), Start > End
void ITDB02::SetPageAddress(unsigned int Start, unsigned int End)
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
void ITDB02::FillRectangle(unsigned int StartX, unsigned int StartY, unsigned int Width, unsigned int Height, unsigned char Red, unsigned char Green, unsigned char Blue)
{
	SetColumnAddress(StartY, StartY+Height-1);
	SetPageAddress(StartX, StartX+Width-1);
	MemoryWrite();
		
	for(uint32_t t = 0; t < (uint32_t)Width * (uint32_t)Height; t++)
	{
		WritePixel(Red, Green, Blue);
	}
}
void ITDB02::drawBMP(uint8_t* BMP)
{
	uint16_t Width,Height;
	Height=128;
	Width=128;
	SetColumnAddress(0, Height-1);
	SetPageAddress(0,Width-1);
	for(uint32_t t = 0; t < (uint32_t)Width * (uint32_t)Height; t++)
	{
		WritePixel(BMP[t]&0x1f, (BMP[t]&0xe0)+(BMP[t+1]&0x03), BMP[t+1]&0xfc);
	}
}
void ITDB02::drawASCII(ASCII* character,unsigned int StartX, unsigned int StartY)
{
    SetColumnAddress(StartY, StartY+character->height-1);
	SetPageAddress(StartX, StartX+character->width-1);
	MemoryWrite();
    for (size_t colomn = 0; colomn < character->width; colomn++)
    {  
        for (size_t pixel = 0; pixel < character->height; pixel++)
        {
            if ((character->map[pixel]&(1<<((character->width-1)-colomn))))
            {
                WritePixel(0,0,0);
            }
            else
            {
                WritePixel(31,63,31);
            }     
        }
        
    }
    
    
}
void ITDB02::drawString(char* string, uint8_t length)
{
    
    for (size_t i = 0; i < length; i++)
    {
        if(((CurrentCol+TimesNewRomanFont[(uint8_t)string[i]]->width) > HORIZONTAL_MAX))
        {
            CurrentRow+=19;
            CurrentCol=0;
            drawASCII(TimesNewRomanFont[(uint8_t)string[i]],CurrentCol,CurrentRow);
            CurrentCol=TimesNewRomanFont[(uint8_t)string[i]]->width;
        }
        else if(string[i]=='\n')
        {
            if((CurrentRow+19)>VERTICAL_MAX)
            {
                CurrentRow=0;
            }
            else
            {
                CurrentRow+=19;
                CurrentCol=0;
            }
            
        }
        else
        {
            drawASCII(TimesNewRomanFont[(uint8_t)string[i]],CurrentCol,CurrentRow);
            CurrentCol+=TimesNewRomanFont[(uint8_t)string[i]]->width;
        }
        

    }
    
}
