#include "interrupt.h"

void intrp_CheckInterrupt()
{

}

#pragma code ADC_INT=0x08 //At interrupt, code jumps here
void intrp_InterruptHappened ()
{   
    _asm
    GOTO intrp_CheckInterrupt
     _endasm
    //not enought space in memory
}