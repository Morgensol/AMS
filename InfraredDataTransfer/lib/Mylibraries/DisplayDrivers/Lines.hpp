#include "Line.hpp"
class Lines
{
private:
    Line *savedLines;
    uint8_t maxLines;
public:
    void addLine(uint8_t spot, Line* lineToAdd)
    {        
		for (uint16_t i = 0; i < savedLines[spot].getLength(); i++)
		{
			if (i < lineToAdd->getLength())
				savedLines[spot].addChar(lineToAdd->getChar(i), i);
			else
				savedLines[spot].addChar(' ', i);
		}
        delete lineToAdd;
    }
    ~Lines()
    {
            delete[] savedLines;
    }
    Lines(uint16_t Length, uint16_t Height)
    {
        maxLines=Height;
        savedLines = new Line[Height]();
        for (uint16_t i = 0; i < Height; i++)
            savedLines[i].resize(Length);
        
    }
    Line* getLine(uint8_t pos)
    {
		Line *line = new Line(savedLines[pos].getLength());
		for (uint16_t i = 0; i < savedLines[pos].getLength(); i++)
			line->addChar(savedLines[pos].getChar(i), i);
		return line;
    }
    uint8_t getMaxLines()
    {
        return maxLines;
    }
};
