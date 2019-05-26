#include "ITDB02.hpp"
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
	_NOP();                     //wait 60ns
	WR_PORT |= (1<<WR_BIT);
	CS_PORT |= (1<<CS_BIT);
	_NOP();                     //Wait 60ns
}

// ILI 9341 data sheet, page 238
void ITDB02::WriteData(uint16_t data)
{
	DATA_PORT_HIGH = (data>>8);
	DATA_PORT_LOW = (data);
	DC_PORT |= (1<<DC_BIT);
	CS_PORT &= ~(1<<CS_BIT);
	WR_PORT &= ~(1<<WR_BIT);
	_NOP();                     //Wait 60ns
	WR_PORT |= (1<<WR_BIT);
	CS_PORT |= (1<<CS_BIT);
	_NOP();                     //Wait 60ns
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
    FillRectangle(0,0,320,240,Color(WHITE)); //Make a white background
    DisplayOn();
    Screen = new Lines(80,maxLines);
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
void ITDB02::WritePixel(Color rgb)
{
	WriteData((uint16_t)rgb.getRed()<<11 | (uint16_t)rgb.getGreen()<<5 | (uint16_t)rgb.getBlue());		
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
void ITDB02::FillRectangle(uint16_t StartX, uint16_t StartY, uint16_t Width, uint16_t Height, Color rgb)
{
	SetColumnAddress(StartY, StartY+Height-1);
	SetPageAddress(StartX, StartX+Width-1);
	MemoryWrite();
		
	for(uint32_t t = 0; t < (uint32_t)Width * (uint32_t)Height; t++)
	{
		WritePixel(rgb);
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
                WritePixel(Color(BLACK)); //black
            }
            else
            {
                WritePixel(Color(WHITE)); //white
            }     
        }
    } 
}

void ITDB02::drawString(char* string, uint16_t length)
{
    Lines* newLines = splitString(string,length);
    for (uint8_t lineNmbr = 0; lineNmbr < newLines->getMaxLines(); lineNmbr++)      
        addLineToScreen(newLines->getLine(lineNmbr));
    drawScreen();
    delete newLines;
}

Lines* ITDB02::splitString(char* string, uint16_t length)
{
    uint8_t numberOfLines = 0; 
    uint16_t pixelsToDraw=0;
    uint16_t pixelsToDrawCheck=0;
    uint16_t charsToDraw[12]={0};
    uint8_t charsCounter=0;
    for (uint16_t i = 0; i < length; i++)
    {
        pixelsToDraw+=TimesNewRomanFont[(uint8_t)string[i]]->width;
        pixelsToDrawCheck+=TimesNewRomanFont[(uint8_t)string[i]]->width;
        charsToDraw[charsCounter]++;
        if ((pixelsToDrawCheck+TimesNewRomanFont[(uint8_t)string[i+1]]->width) > HORIZONTAL_MAX)
        {
            charsCounter++;
            pixelsToDrawCheck=0;
        }
        
    }
    numberOfLines = (pixelsToDraw%HORIZONTAL_MAX)==0?pixelsToDraw/HORIZONTAL_MAX:(pixelsToDraw/HORIZONTAL_MAX)+1;
    Lines* returnObj= new Lines(80, numberOfLines);
    uint16_t startPos = 0;
    for ( uint16_t i=0; i < numberOfLines; i++)
    {
        returnObj->addLine(i, getNextString(string, charsToDraw[i], startPos));
         startPos +=charsToDraw[i];
    }
    
    return returnObj;
}

void ITDB02::addLineToScreen(Line* lineToAdd)
{
    if (activeLines+1 == Screen->getMaxLines())
        moveScreenLinesUp();
    Screen->addLine(activeLines,lineToAdd);
    activeLines += activeLines+1 >= Screen->getMaxLines() ? 0 : 1;
}

Line* ITDB02::getNextString(char* string, uint16_t maxLineLength, uint16_t startPos)
{
    Line* bufferLine = new Line(maxLineLength);
    for (uint16_t i = 0; i < maxLineLength; i++)
        bufferLine->addChar(string[i + startPos],i);
    return bufferLine;
}

void ITDB02::drawScreen()
{
    CurrentRow=0;
    for (uint16_t i = 0; i < Screen->getMaxLines(); i++)
    {
        CurrentCol = 0;
        drawLine(i);
        CurrentRow+=characterHeigth('A');  
    }
}

void ITDB02::drawLine(uint16_t lineNmbr)
{
    Line* tempLine = Screen->getLine(lineNmbr);

    for (uint8_t i = 0; i < tempLine->getLength(); i++)
    {
        drawASCII(TimesNewRomanFont[(uint8_t)tempLine->getChar(i)],CurrentRow,CurrentCol);
        CurrentCol+=characterWidth(tempLine->getChar(i));
    }
    delete tempLine;
}

void ITDB02::moveScreenLinesUp()
{
    for (uint8_t i = 0; i < Screen->getMaxLines()-1; i++)
        Screen->addLine(i,Screen->getLine(i+1));
    
}


int ITDB02::characterWidth(char character)
{
    return TimesNewRomanFont[(uint8_t)character]->width;
}

int ITDB02::characterHeigth(char character)
{
    return TimesNewRomanFont[(uint8_t)character]->height;
}
