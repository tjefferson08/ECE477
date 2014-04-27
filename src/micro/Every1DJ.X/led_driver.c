#include <stdio.h>
#include <p18cxxx.h>
#include <spi.h>
#include <stdlib.h>
#include <delays.h>
#include <i2c.h>

#define SDA PORTDbits.RD5
#define SCL PORTDbits.RD6
#define OE PORTDbits.RD4

#define LCDADDR 0x1c

//Control registers
#define MODE1 0x00
#define MODE2 0x01
#define GRPPWM 0x1A
#define GRPFREQ 0x1B
#define LEDOUT0 0x1D
#define LEDOUT1 0x1E
#define LEDOUT2 0x1F
#define LEDOUT3 0x20
#define LEDOUT4 0x21
#define LEDOUT5 0x22


  unsigned int q;
  unsigned int y;

void sendDataLED(char value){
  PIR3bits.SSP2IF = 0;
  SSP2BUF = value;
  while(!PIR3bits.SSP2IF);
  PIR3bits.SSP2IF = 0;
  Delay10TCYx(0);
}

void controlRegister(char conReg) {
    sendDataLED(conReg);
}

void closeI2CLED() {

     //Clear interrupt flag
     PIR3bits.SSP2IF = 0;
     //Stop Enable bit
     SSP2CON2bits.PEN = 1;

     while(!PIR3bits.SSP2IF);
     //Clear interrupt flag
     PIR3bits.SSP2IF = 0;
}

void openI2CLED(char addr) {
     //Clear interrupt flag
     PIR3bits.SSP2IF = 0;
     //Start Enable Bit
     SSP2CON2bits.SEN = 1;
     while(!PIR3bits.SSP2IF);
     //Clear interrupt flag
     PIR3bits.SSP2IF = 0;
     Delay10TCYx(0);
     //Send address and Write bit
     SSP2BUF = (addr << 1);
     while(!PIR3bits.SSP2IF);
     //Clear interrupt flag
     PIR3bits.SSP2IF = 0;
     //Delay10TCYx(100);
     Delay10TCYx(0);
}

void blueLEDs(char brightness, char frequency) {
    
}

void redLEDs(char brightness, char frequency) {

}

void greenLEDs(char brightness, char frequency) {

}

void LEDON(int led, int color, int value) {
        openI2CLED(LCDADDR);
        sendDataLED(led);
        sendDataLED(value);
        closeI2CLED();
}

int sendLEDData(unsigned int adcResult) {
     SSP2CON1 = 0;
     SSP2CON2 = 0;
     SSP2STAT = 0;

     TRISDbits.RD6 = 1; //Enable SCL for output
     TRISDbits.RD5 = 1; //Enable SDA for output
     TRISDbits.RD3 = 0; //Enable OE
     SSP2CON1bits.SSPEN = 1; //Master Synchronous Serial Port Enable
     SSP2CON1bits.SSPM3 = 1; //Master Mode
     SSP2ADD = 0x63;
     //PIE3bits.SSP2IE = 1; //MSSP2 Interrupt Enable bit
     SSP2STATbits.SMP = 1;

     OE = 0;
     Delay10TCYx(0);
     OE = 0;
     
     //Restart Command
     openI2CLED(0x03);
     sendDataLED(0xa5);
     sendDataLED(0x5a);

     //Close and Open I2C with LCD address
     closeI2CLED();
     Delay10TCYx(0);

     openI2CLED(LCDADDR);
     //Control Register - Mode 1
     controlRegister(MODE1);
     //00000000 - Normal mode of operation
     sendDataLED(0x0F); //enable subaddress and all call routines

     openI2CLED(LCDADDR);
     //Control Register - Mode 1
     controlRegister(MODE2);
     //00000000 - Normal mode of operation
     sendDataLED(0x28);

     openI2CLED(LCDADDR);
     //Control Register - setup all LED's outputs for individual PWM control
     controlRegister(LEDOUT0);
     sendDataLED(0xFF);

      openI2CLED(LCDADDR);
     //Control Register
     controlRegister(LEDOUT1);
     sendDataLED(0xFF);

     openI2CLED(LCDADDR);
     //Control Register
     controlRegister(LEDOUT2);
     sendDataLED(0xFF);

     openI2CLED(LCDADDR);
     //Control Register
     controlRegister(LEDOUT3);
     sendDataLED(0xFF);

     openI2CLED(LCDADDR);
     //Control Register
     controlRegister(LEDOUT4);
     sendDataLED(0xFF);

     openI2CLED(LCDADDR);
     //Control Register
     controlRegister(LEDOUT5);
     sendDataLED(0xFF);

     //TURN ON LED
     LEDON(0x03, 0x00, 0x10);

     //TURN ON LED
     LEDON(0x07, 0x00, 0x08);

     openI2CLED(LCDADDR);
     //Control Register
     controlRegister(GRPPWM);

     if (adcResult > 0x10) {
         sendDataLED(0xFA);
     }
     else {
         sendDataLED(0xFA);
     }
     

     openI2CLED(LCDADDR);
     //Control Register
     controlRegister(GRPFREQ);
     if (adcResult > 0x10) {
        sendDataLED(0x06);
     }
     else {
         sendDataLED(0x40);
     }

     /*
     while(1) {
        //LED1 ON
        LEDON(0x03, 0x00, 0xff);
        for(y=0;y<40;y++) {
            Delay10TCYx(0);
        }
        //LED1 OFF
        LEDON(0x03, 0x00, 0x00);

        for(y=0;y<40;y++) {
            Delay10TCYx(0);
        }
        
     }*/

     //Control Register - Group Duty Cycle
     //controlRegister(0x1A);
     //10000000 - Group blinking
     //sendDataLED(0xAA);

     //Control Register - Group Frequency
     //controlRegister(0x1B);
     //10000000 - Group period
     //sendDataLED(0xAA);

     //Control Register - Chase control
     //controlRegister(0x1C);
     //00001100 - LED1 output enabled
     //sendDataLED(0x8F);

     //Control Register - LED Driver output state
     //controlRegister(0x1D);
     //00001100 - LED1 output enabled
     //sendDataLED(0x55);

     closeI2CLED();
     //Interrupt generated once stop condition is complete

}
