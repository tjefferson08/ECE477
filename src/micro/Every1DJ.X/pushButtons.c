#include <stdio.h>
#include <p18cxxx.h>
#include <stdlib.h>
#include <delays.h>

void initializePushButtons(void) {
   // RCONbits.IPEN = 0; // no interrupt prioritization
    //INTCONbits.PEIE = 0; // Peripheral Interrupt enable
    INTCONbits.RBIE = 0; // enable RB port change interrupt
    INTCONbits.INT0IE = 1; //INT0 external interrupt enable
    INTCON3bits.INT1IE = 1; // INT1 IE
    INTCON3bits.INT2IE = 1; // INT2 IE
    INTCON3bits.INT3IE = 1; // INT3 IE
    INTCONbits.RBIF = 0; // clear IFG initially
    INTCON2bits.RBPU = 1; // pullups on portB disabled
    INTCONbits.INT0IF = 0; // IFGs - clr in SW
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT2IF = 0;
    INTCON3bits.INT3IF = 0;
    INTCON2bits.INTEDG0 = 0; //interrupt on falling edge
    INTCON2bits.INTEDG1 = 0;
    INTCON2bits.INTEDG2 = 0;
    INTCON2bits.INTEDG3 = 0;
    TRISBbits.TRISB0 = 1; // RB0-3 inputs
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 1;
}