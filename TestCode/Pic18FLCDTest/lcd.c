#include "lcd.h"

// Bit mapping of Registers
#define E  PORTEbits.RE0
#define RW PORTEbits.RE1
#define RS PORTEbits.RE2
#define LCD_DATA_PORT PORTD

// Functions pg.24
// 
// DL=1 for 8bit mode
// D=1 display is on
// C=1 cursor is displayed
// B=1 cursor blinks

// Line star Addresses
#define LCD_LINE_START_1  0x00
#define LCD_LINE_START_2  0x40
#define LCD_LINE_START_3  0x14
#define LCD_LINE_START_4  0x54


// Private Function Protypes
void lcd_sendCommandPulseInUS(UINT16 pulseWidthInUS);
void lcd_delayInUS(UINT16 time);
void lcd_putCharacter(char c);
void lcd_setAddr(UINT16);
void lcd_putCharAtAddr(char c, UINT16 addr);



// ****************************
// Fuction Definations
// *****************************

void lcd_sendCommandPulseInUS(UINT16 pulseWidthInUS)
{
    E = 0;
    lcd_delayInUS(pulseWidthInUS);
    E = 1;
    lcd_delayInUS(pulseWidthInUS);
    E = 0;
}

void lcd_clearDisplay()
{
    RS = 0;
    RW = 0;
    LCD_DATA_PORT = 0x01;
    lcd_sendCommandPulseInUS(15000);
}

//delay in ms
void lcd_delayInUS(UINT16 time)
{
    time *= 2;
    while(time>0){
        Delay1TCY(); //8MHz, 4 inst per clock -> 500ns * 2 = 1us
        time--;
    }
}

void lcd_initDisplay()
{
    TRISD = 0x00;  // set to output output
    TRISE = 0x00;  // set to output
    ADCON1 = 0x0F;
    
    RS = 0;
    RW = 0;
    LCD_DATA_PORT = 0x38;//make 38 for two lines
    lcd_sendCommandPulseInUS(2000);
    lcd_displayOn();
    lcd_clearDisplay();

}

void lcd_displayOff()
{
    RS = 0;
    RW = 0;
    LCD_DATA_PORT = 0x08;//display off, cursor off, blinking off, DC
    lcd_sendCommandPulseInUS(40);
}

void lcd_displayOn()
{
    RS = 0;
    RW = 0;
    LCD_DATA_PORT = 0x0C;//display on, cursor off, blinking off, DC
    lcd_sendCommandPulseInUS(40);
}

void lcd_setAddr(UINT16 addr){
    RS = 0;
    RW = 0;
    LCD_DATA_PORT = 0x80 | (0x7F & addr);
    lcd_sendCommandPulseInUS(40);
}

void lcd_putCharacter(char c){
    
    RS = 1;  //data mode
    RW = 0;  //write
    LCD_DATA_PORT = 0xFF & c;
    lcd_sendCommandPulseInUS(40);
}

void lcd_writeLine(char *text, UINT8 lineNumber)
{
    UINT16 s = 0;
    UINT16 i = 0;
    
    switch (lineNumber)
    {
        case 1:
            lcd_setAddr( LCD_LINE_START_1 );
            break;
        case 2:
            lcd_setAddr( LCD_LINE_START_2 );
            break;
        case 3:
            lcd_setAddr( LCD_LINE_START_3 );
            break;
        case 4:
            lcd_setAddr( LCD_LINE_START_4 );
            break;
        default:
            lcd_setAddr( LCD_LINE_START_1 );
            break;
    }

    s = strlen(text);
    for(i=0;i<s;i++){
        lcd_putCharacter(text[i]);
    }
}

void lcd_putCharAtAddr(char c, UINT16 addr)
{
    lcd_setAddr( addr );
    lcd_putCharacter( c );
}

