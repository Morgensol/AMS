#include <stdint.h>

class Line
{
private:
    char* characters;
    uint16_t length;
public:
    Line(char* c, uint16_t l){characters=c,length=l;}
    Line(uint16_t l = 1)
    {
        characters = new char[l];
        length=l;
        for (uint16_t i = 0; i < l; i++)
            characters[i]=' ';        
        
    }
    ~Line()
    {
        delete[] characters;
    }
    void resize(uint16_t l)
    {
        delete[] characters;
        characters = new char[l];
        length=l;
        for (uint16_t i = 0; i < l; i++)
            characters[i]=' '; 
    }
    void addChar(char c, uint16_t i)
    {
        if(i<length)
            characters[i]=c;
    }
	void operator=(const Line l)
	{
		uint16_t smallest = (this->length < l.length) ? this->length : l.length;
		for (uint16_t i = 0; i < smallest; i++)
			this->characters[i] = l.characters[i];
		this->length = smallest;
	}
    uint16_t getLength()
    {
        return length;
    }
    char getChar(uint16_t pos)
    {
        return characters[pos];
    }
    char* getArray()
    {
        return characters;
    }
};
