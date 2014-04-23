/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <stdio.h>
#include <p18cxxx.h>
#include <adc.h>
#include <stdlib.h>
#include <delays.h>

#define NUMSAMPLES 200
#define BEATTHRESH 0x0100



    

void initializeADC() {
     //Register Initializations
    ADCON0 = 0;
    ADCON1 = 0;
    ANCON0 = 0;
   // TRISFbits.RF1 = 1;
    ADCON0bits.VCFG1 = 0; //AVss
    ADCON0bits.VCFG0 = 0; //AVdd
    ADCON0bits.ADON = 1;
    ADCON0bits.CHS = 0x03; //AN3
    ADCON1bits.ADFM = 1; //Right justified
    ADCON1bits.ADCAL = 1; // calibrate first
    ADCON1bits.ADCS = 0x06; //Fosc/4
    ADCON1bits.ACQT = 0x06; //16 TAD
    ANCON0bits.PCFG3 = 0;
    //ANCON1bits.PCFG13 = 0; //AN13 port configuration bits - Analog Channel
    ADRESH = 0;
    ADRESL = 0;

    //Timer4 initilization
    PIE3bits.TMR4IE = 1; //Enable Timer4 = PR4
    PR4 = 0xAA; //Timer 4 Period

    //Timer4 Register
    T4CONbits.T4OUTPS = 0x04; //1:4 Postscaler
    T4CONbits.TMR4ON = 1; //Turn Timer 4 ON
    T4CONbits.T4CKPS = 0x01; //Prescaler is 4

    //Must be cleared in software
    PIR3bits.TMR4IF = 0;

}

int adc() {

    unsigned int i = 0;
    unsigned int p = 0;
    unsigned int j = 0;
    unsigned int num_beats = 0;
    unsigned int result_high;
    unsigned int result_low;
    unsigned int result = 0;
    
    //Must be cleared in software
    PIR3bits.TMR4IF = 0;
    
    // dummy sample to do calibration
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO);
    ADCON1bits.ADCAL = 0;

    for (i=0; i<NUMSAMPLES; i++) {
        ADCON0bits.GO = 1;
        while (ADCON0bits.GO);
        result_high = ADRESH;
        result_low = ADRESL;
        result = result_low << 2;
        if ( result <= BEATTHRESH ) {
            num_beats++;
        }
        PIR1bits.ADIF = 0;
    }


    PIE3bits.TMR4IE = 1; //Enable Timer4 = PR4
    return (num_beats);

    TRISHbits.RH0 = 0x00; // RHS is output
    PORTHbits.RH0 = 1; // set heartbeat LED

    num_beats *= 2;
    for (j=0;j<4;j++) {      
        if (num_beats > 40) num_beats=39;
        for (p=0;p<(40-num_beats);p++) {
            Delay100TCYx(255);
        }
        PORTHbits.RH0 = 0;
        for (p=0;p<(40-num_beats);p++) {
            Delay100TCYx(255);
        }
        PORTHbits.RH0 = 1; // set heartbeat LED
    }

    return 1;
}