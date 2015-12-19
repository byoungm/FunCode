#ifndef _LCD_H_
#define	_LCD_H_

#include "stdincludes.h"


// Public Funtions
void lcd_initDisplay();
void lcd_displayOff();
void lcd_displayOn();
void lcd_clearDisplay();
void lcd_writeLine(char *text, UINT8 lineNumber);
void lcd_delayInUS(UINT16 time);

#endif	// _LCD_H_
