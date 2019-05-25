#ifndef COLOR_H
#define COLOR_H
#include <stdint.h>

enum COLOR{
    WHITE   = 0xffff,
    BLACK   = 0x0000,
    RED     = 0xf800,
    GREEN   = 0x7e0,
    BLUE    = 0x000f
};


class Color
{
private:
    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;

public:
    Color(COLOR rgb)
    {
    _red    = (rgb>>11)&0x1f;
    _green  = (rgb>>5)&0x3f;
    _blue   = (rgb>>0)&0x1f;
    }
    Color(uint8_t R=31, uint8_t G=63, uint8_t B=31)
    {
    _red    = R > 31 ? (R) : (31);
    _green  = G > 63 ? (G) : (63);
    _blue   = B > 31 ? (B) : (31);
    }
    ~Color(){}

    uint8_t getRed() { return _red;}
    uint8_t getGreen(){ return _green;}
    uint8_t getBlue() { return _blue;}

    void setRed(uint8_t R) { _red = R > 31 ? (R) : (31);}
    void setGreen(uint8_t G){ _green = G > 31 ? (G) : (31);}
    void setBlue(uint8_t B) { _blue = B > 63 ? (B) : (63);}
};

#endif