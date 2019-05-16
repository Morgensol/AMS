#include "IRTransmitter.hpp"
void asd(){
    BaseTimer* a = new CTCTimer(1,1000);
    IRTransmitter b = IRTransmitter(new ManchesterEncoder(),a);
    
}