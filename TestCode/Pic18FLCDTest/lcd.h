#ifndef _LCD_H_
#define	_LCD_H_

#include "stdincludes.h"


// Public Funtions
void lcd_initDisplay(void);
void lcd_displayOff(void);
void lcd_displayOn(void);
void lcd_clearDisplay(void);
void lcd_writeLine(char *text, UINT8 lineNumber);
void lcd_delayInUS(UINT16 time);

#endif	// _LCD_H_
