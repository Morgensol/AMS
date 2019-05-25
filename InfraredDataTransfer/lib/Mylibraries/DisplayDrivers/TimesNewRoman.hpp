#ifndef TIMESNEWROMAN_H
#define TIMESNEWROMAN_H

#include "stdint.h"
typedef uint16_t ASCII_Char[19];
struct ASCII{
    uint16_t* map;
    uint8_t width;
    uint8_t height;
};
extern struct ASCII* TimesNewRomanFont[400];
void setupTimesNewRoman();

#endif