
#include "toroidalui.h"

#define BTN_UP    &PORTB, 3, 20, 0
#define BTN_DOWN  &PORTB, 4, 20, 0
#define BTN_LEFT  &PORTB, 5, 20, 0
#define BTN_RIGHT &PORTB, 6, 20, 0
#define BTN_ENTER &PORTB, 7, 20, 0

const float Pi = 3.1415926535;

// Lcd pinout settings
sbit LCD_RS at RE2_bit;
sbit LCD_EN at RE1_bit;
sbit LCD_D7 at RD7_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D4 at RD4_bit;

// Pin direction
sbit LCD_RS_Direction at TRISE2_bit;
sbit LCD_EN_Direction at TRISE1_bit;
sbit LCD_D7_Direction at TRISD7_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D4_Direction at TRISD4_bit;

float meterCounter;
unsigned int tickCounter = 0;
unsigned int secondCounter = 0;
unsigned int msecondCounter = 0;
char buff[50];

float radius = .3;
float circumference;

// Menu variables
int menu_i;
int menu_j;


void resetTimer()
{
    // 1 second value
    //TMR0H = 0xE1;
    //TMR0L = 0x7B;
    TMR0H = 0xFF;
    TMR0L = 0x05;

    INTCON.TMR0IF = 0;
}

void hud()
{
    sprintf(buff, "%ds %.2fkm/h               ", secondCounter, (3.6*tickCounter*circumference)/secondCounter);
    Lcd_Out(1, 1, buff);
    sprintf(buff, "%.2frpm %.2fm       ", (60*tickCounter)/(float)secondCounter, meterCounter);
    Lcd_Out(2, 1, buff);
}

void drawDisplay()
{
    tui_draw();

}


void interrupt()
{
    if(INTCON.INT0IF) {
        tickCounter++;
        meterCounter += circumference;
        INTCON.INT0IF = 0;
    }
    
    if(INTCON.TMR0IF) {
        resetTimer();
        msecondCounter++;
        if(msecondCounter == 1000) {
            secondCounter++;
            msecondCounter = 0;
        }
    }
}

void scanKeys() {
    if(Button(BTN_UP)) {
        tui_up();
    } else if(Button(BTN_DOWN)) {
        tui_down();
    }
}


void main() {
    circumference = 2*Pi*radius;

    ADCON0 = 0;
    ADCON1 = 0x0007;
    TRISD = 0;
    TRISE = 0;
    
    INTCON.GIE = 1; // Enables interrupts

    // Enabling tick detection
    INTCON.INT0IE = 1; // Enables INT0
    INTCON.INT0IF = 0; // Clears INT0
    
    // Enabling one second timer
    T0CON.TMR0ON = 1;  // Turns on Timer0
    T0CON.T08BIT = 0;  // Timer0 16bit
    T0CON.T0CS = 0;    // Internal clock
    T0CON.PSA = 0;     // Enables Timer0 prescaler
    T0CON.T0PS2 = 0;   // Prescaler 1:256
    T0CON.T0PS1 = 1;   // Prescaler 1:256
    T0CON.T0PS0 = 0;   // Prescaler 1:256
    INTCON.TMR0IE = 1; // Enables TMR0
    resetTimer();

    Lcd_Init();
    
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    
    tui_register(0, 0, hud);

    while(1) {

        
        scanKeys();
    
        drawDisplay();
        Delay_ms(100);
    }
}
