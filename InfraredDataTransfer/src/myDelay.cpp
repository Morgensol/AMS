#include "myDelay.hpp"
void usleep(uint32_t us)
{
    for (size_t i = 0; i < us; i++)
    {
        _delay_us(1);
    }
    
}
void msleep(uint32_t ms)
{
    for (size_t i = 0; i < ms; i++)
    {
        _delay_ms(1);
    }
}