#include "stdincludes.h"
#include "p18f4620_config.h"
#include "lcd.h"
#include "interrupt.h"


// Function Declarations
void mcu_init();

// Global Variables
char lcdString[20];


void main() {

    mcu_init();
    lcd_initDisplay();

    strcpypgm2ram(lcdString, (char*)"Merry Christmas");
    lcd_writeLine(lcdString,1);
    strcpypgm2ram(lcdString, (char*)"Ho Ho Ho");
    lcd_writeLine(lcdString,2);
    strcpypgm2ram(lcdString, (char*)"Teseting ");
    lcd_writeLine(lcdString,3);
    strcpypgm2ram(lcdString, (char*)"    Beats Per Min");
    lcd_writeLine(lcdString,4);

    while(1);    
}


void mcu_init()
{
    // Internal osc. 8 MHz
    OSCCON = 0b01110000;
}