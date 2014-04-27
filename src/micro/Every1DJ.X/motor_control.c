#include <stdio.h>
#include <p18cxxx.h>
#include <stdlib.h>
#include <delays.h>

#define A PORTEbits.RE2
#define B PORTEbits.RE3
#define C PORTEbits.RE4
#define D PORTEbits.RE5
#define DIR PORTHbits.RH0
#define OE PORTJbits.RJ1
#define EN1 PORTDbits.RD1
#define EN2 PORTDbits.RD2



void initializeMotor() {
    TRISEbits.RE2 = 0; //A output
    TRISEbits.RE3 = 0; //B output
    TRISEbits.RE4 = 0; //C output
    TRISEbits.RE5 = 0; //D output
    TRISHbits.RH0 = 0; //DIR output
    TRISJbits.RJ1 = 0; //OE output
    TRISDbits.RD1 = 0; //EN1 output
    TRISDbits.RD2 = 0; //EN2 output
    PR2 = 0x80; //Period

    T2CONbits.T2OUTPS0 = 0;
    T2CONbits.T2OUTPS1 = 0;
    T2CONbits.T2OUTPS2 = 0;
    T2CONbits.T2OUTPS3 = 0;
    T2CONbits.T2CKPS = 0x01; //Prescale is 4
    T2CONbits.TMR2ON = 1;  //Turn on Timer
    PR2 = 0xFF; //Period
    PIE1bits.SSP1IE = 1; //MSSP1 interrupt enable?????
    PIE1bits.TMR2IE = 1; //TMR2 PR2 Match interrupt enable bit

    //Timer 2 interrupt flag
    //Must be cleared in software
    PIR1bits.TMR2IF = 0; //Should be 0
    DIR = 1;
    OE = 0;


    // Motor enables
    EN1 = 1;
    EN2 = 1;
}
void sendMotorControl(int state) {
    
    //clear timer2 interrupt
    PIR1bits.TMR2IF = 0;

    switch(state) {
        case(0):
            A = 1;
            B = 0;
            C = 0;
            D = 1;
            break;            
        case(1):
            A = 0;
            B = 1;
            C = 0;
            D = 1;
            break;
        case(2):
            A = 0;
            B = 1;
            C = 1;
            D = 0;
            break;
        case(3):
            A = 1;
            B = 0;
            C = 1;
            D = 0;
            break;
    }

}
