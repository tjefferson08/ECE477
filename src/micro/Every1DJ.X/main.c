/* 
 * File:   main.c
 * Author: team2
 *
 * Created on March 24, 2014, 12:23 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f87j11.h>
#include <spi.h>
#include <string.h>
#include "lcd.h"
#include "led_driver.h"
#include "adc.h"
#include "RPI.h"
#include "metaData.h"

#pragma config WDTEN = OFF //Watchdog Timer
#pragma config XINST = ON //Extended Instruction Set
#pragma config FOSC = INTOSCPLL // Oscillator Selection bits (INTOSC with PLL enabled, port function on RA6 and RA7)

#define EXIT_SUCCESS 0

// GLOBALS
char song[20];
char artist[20];
char album[20];
char year[20];

unsigned int count = 0;
unsigned int metaData = 1;
unsigned int motorCount = 0;
unsigned int changeCount = 0;
unsigned int bpm = 200;
unsigned int displayPage;
unsigned int dataDone;
unsigned int command = 1;
unsigned int motorState = 0;

unsigned int timer4Max = 200;
unsigned int timer4Count = 0;
unsigned int adcResult = 0;

// prototypes
void high_isr (void);

// Redirect all high priority INTs to high_isr() for
// decoding/manual prioritization
#pragma code isr=0x08
#pragma interrupt isr

// Handle all interrupts here, send off to appropriate
// subhandlers
void isr (void)
{
    // MOTOR ISR handle
 /*   if (PIR1bits.TMR2IF) {
        motorCount++;
        if  (motorCount == bpm) {
            changeCount++;
            if (changeCount == 400) {
                //if (bpm > 79) {
                    bpm = 200;
                //}
                //else {
                //    bpm = 80;
                //}
                changeCount = 0;
            }
            motorCount = 0;
        sendMotorControl(motorState);
        motorState = (motorState + 1) % 4;
        }
    } */

    if (PIR3bits.TMR4IF) {
        PIR3bits.TMR4IF = 0;
        //Turn off interrupt while in function
        timer4Count++;
        if(timer4Count==timer4Max) {
            PIE3bits.TMR4IE = 0; //Disable Timer4 = PR4
            timer4Count = 0;
            adcResult = adc();
            sendLEDData(adcResult);
        }
    }

    // pushbutton 0 interrupt PLAY/PAUSE SONG
    if (INTCONbits.INT0IF) {

        //Command code for Play Pause
        command = 3;
       INTCONbits.INT0IF = 0;
    }

    // pushbutton 1 interrupt NEXT SONG
    else if (INTCON3bits.INT1IF) {
        //Command code for Next Song
        command = 2;
        INTCON3bits.INT1IF = 0;
    }

    //pushbutton 2 interrupt NEXT LCD PAGE
    else if (INTCON3bits.INT2IF) {
        if (displayPage) {
            lcdWrite(album, year, strlen(album), strlen(year), displayPage);
            displayPage = 0;
        }
        else {
            lcdWrite(song, artist, strlen(song), strlen(artist), displayPage);
            displayPage = 1;
        }
    }

    // pushbutton 3 interrupt CHANGE LED
    else if (INTCON3bits.INT3IF) {

    }

    else if (PIR1bits.SSP1IF) {
        receiveSPI(command);
        command = 1;
        if (dataDone) {
        //    lcdWrite(song, artist, strlen(song), strlen(artist));
            displayPage = 1; 
            dataDone = 0;
        }
    }
}
#pragma code /* return to the default code section */


int k = 9;
int ip = 0;
//char song_name[] = "PARADISE";
//char artist[] = "COLDPLAY";

void main(void) {
    
    INTCONbits.GIE = 1; //General Interrupt enable
    INTCONbits.PEIE = 1;

    
    //sendData(0xAA);

    //lcdWrite(song_name, artist, 8, 8, 0);
    //Timer 2 Output postscale bits - 1:1 scaler

    initializeADC();
    //initializeMotor();
   // sendMotorControl();
    //initializePushButtons();
    //initializeRPI();
    // REGS for pushbutton setup

    while(1) {
        TRISHbits.RH2 = 0x00; // RHS is output
        PORTHbits.RH2 = 1; // set heartbeat LED
        for (ip=0;ip<20;ip++) {
            Delay100TCYx(255);
        }
        PORTHbits.RH2 = 0;
        for (ip=0;ip<20;ip++) {
            Delay100TCYx(255);
        }
    }
}



