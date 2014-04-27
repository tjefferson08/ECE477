#include <stdio.h>
#include <stdlib.h>
#include <p18cxxx.h>
#include <spi.h>
#include "metaData.h"

void initializeRPI(void){
    // Slave
    OpenSPI1(SLV_SSOFF, MODE_00, SMPEND);
    PIR1bits.SSP1IF = 0;
    PIE1bits.SSP1IE = 1; //SPI1 interrupt enabled
    SSP1BUF = 0x04;
}

void receiveSPI(int command) {
    int spiCount = 0;
    int dataType = 0;

    //Received heartbeat from micro
    if (SSP1BUF == 0xAA) {
        PIR1bits.SSP1IF = 0;

        //Control signal for Next Song
        if (command == 2) {
            SSP1BUF = 0x02;
        }
        //Control signal for Play Pause
        else if (command == 3) {
            SSP1BUF = 0x03;
        }
        //Generic responce to RPI Command = 1
        else {
            SSP1BUF = 0x01;
        }
        return;
    }

    //Start receiving next song from RPI
    else if (SSP1BUF == 0xBB) {
        PIE1bits.SSP1IE = 0; //SPI1 interrupt disabled
        PIR1bits.SSP1IF = 0;
        for (dataType=0;dataType<4;dataType++) {
            for(spiCount=0;spiCount<20;spiCount++)
            {
                SSP1BUF = 0x01;
                while(!PIR1bits.SSP1IF);
                PIR1bits.SSP1IF = 0;
                switch(dataType) {
                    case(0):
                        song[spiCount] =  SSP1BUF;
                        break;
                    case(1):
                        artist[spiCount] =  SSP1BUF;
                        break;
                    case(2):
                        album[spiCount] =  SSP1BUF;
                        break;
                    case(3):
                        year[spiCount] =  SSP1BUF;
                    default: break;
                }
            }
        }
        dataDone = 1; 
        PIE1bits.SSP1IE = 1; //SPI1 interrupt enabled
    }
}