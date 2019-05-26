#include "Arduino.h"
#include "avr/interrupt.h"
class TempSensor
{
private:
    char message[20];
public:
    TempSensor();
    ~TempSensor();
    void setup();
    void sendStartSequence();
    void getData();
    char* getTemp();
    char* getHum();
    bool checkParity();
};