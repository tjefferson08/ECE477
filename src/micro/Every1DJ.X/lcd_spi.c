/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <stdio.h>
#include <p18cxxx.h>
#include <spi.h>
#include <stdlib.h>
#include <delays.h>

//#include "system.h"        /* System funct/params, like osc/peripheral config */
//#include "user.h"          /* User funct/params, such as InitApp */
#include <xlcd.h>

#define CS PORTDbits.RD1
#define DC PORTDbits.RD4
#define RES PORTDbits.RD3
#define BRIGHTNESS 0xBF
 
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

char write_out;
char msg_code;
char val[38];
int i = 0;


void setGPIO(char value){
  //PIR1bits.SSP1IF = 1;
  PIR1bits.SSP1IF = 0;
  CS=0; // we are about to initiate transmission
  SSP1BUF = value;
  while(!PIR1bits.SSP1IF);
  PIR1bits.SSP1IF = 0;
  //WriteSPI1(value);    // set value
  CS=1; // we are ending the transmission
}


void lcdCommand(char command){
  //setGPIO(0x00); // RS=0, E=0
  Delay10TCYx(0);
  setGPIO(command); // send data
  Delay10TCYx(0);
  //    setGPIO(0x40); // RS=0, E=1
  //    Delay10TCYx(0);
  //    setGPIO(0x00); // RS=0, E=0
  //    Delay10TCYx(0);
}

void sendLetter(char letter){
  int i = 0;
  char charBUF[8];
  switch(letter) {
    charBUF[5] = charBUF[6] = charBUF[7] = 0x00; // true for all characters (spacing)
    //0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
  case '0':
    charBUF[0] = 0x3e;
    charBUF[1] = 0x51;
    charBUF[2] = 0x49;
    charBUF[3] = 0x45;
    charBUF[4] = 0x3e;
    break;
    //0x00, 0x42, 0x7F, 0x40, 0x00,// 1
  case '1':
    charBUF[0] = 0x00;
    charBUF[1] = 0x42;
    charBUF[2] = 0x7f;
    charBUF[3] = 0x40;
    charBUF[4] = 0x00;
    break;
    //0x42, 0x61, 0x51, 0x49, 0x46,// 2
  case '2':
    charBUF[0] = 0x42;
    charBUF[1] = 0x61;
    charBUF[2] = 0x51;
    charBUF[3] = 0x49;
    charBUF[4] = 0x46;
    break;
    //0x21, 0x41, 0x45, 0x4B, 0x31,// 3
  case '3':
    charBUF[0] = 0x21;
    charBUF[1] = 0x41;
    charBUF[2] = 0x45;
    charBUF[3] = 0x4b;
    charBUF[4] = 0x31;
    break;
    //0x18, 0x14, 0x12, 0x7F, 0x10,// 4
  case '4':
    charBUF[0] = 0x18;
    charBUF[1] = 0x14;
    charBUF[2] = 0x12;
    charBUF[3] = 0x7f;
    charBUF[4] = 0x10;
    break;
    //0x27, 0x45, 0x45, 0x45, 0x39,// 5
  case '5':
    charBUF[0] = 0x27;
    charBUF[1] = 0x45;
    charBUF[2] = 0x45;
    charBUF[3] = 0x45;
    charBUF[4] = 0x39;
    break;
    //0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
  case '6':
    charBUF[0] = 0x3c;
    charBUF[1] = 0x4a;
    charBUF[2] = 0x49;
    charBUF[3] = 0x49;
    charBUF[4] = 0x30;
    break;
    //0x01, 0x71, 0x09, 0x05, 0x03,// 7
  case '7':
    charBUF[0] = 0x01;
    charBUF[1] = 0x71;
    charBUF[2] = 0x09;
    charBUF[3] = 0x05;
    charBUF[4] = 0x03;
    break;
    //0x36, 0x49, 0x49, 0x49, 0x36,// 8
  case '8':
    charBUF[0] = 0x36;
    charBUF[1] = 0x49;
    charBUF[2] = 0x49;
    charBUF[3] = 0x49;
    charBUF[4] = 0x36;
    break;
    //0x06, 0x49, 0x49, 0x29, 0x1E,// 9
  case '9':
    charBUF[0] = 0x06;
    charBUF[1] = 0x49;
    charBUF[2] = 0x49;
    charBUF[3] = 0x29;
    charBUF[4] = 0x1e;
    break;
  case 'A':
    charBUF[0] = 0x7e;
    charBUF[1] = 0x11;
    charBUF[2] = 0x11;
    charBUF[3] = 0x11;
    charBUF[4] = 0x7e;
    break;
  case 'B':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x49;
    charBUF[2] = 0x49;
    charBUF[3] = 0x49;
    charBUF[4] = 0x36;
    break;
  case 'C':
    charBUF[0] = 0x3e;
    charBUF[1] = 0x41;
    charBUF[2] = 0x41;
    charBUF[3] = 0x41;
    charBUF[4] = 0x22;
    break;
  case 'D':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x41;
    charBUF[2] = 0x41;
    charBUF[3] = 0x22;
    charBUF[4] = 0x1c;
    break;
  //  $7f,$49,$49,$49,$41, '* E */
  case 'E':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x49;
    charBUF[2] = 0x49;
    charBUF[3] = 0x49;
    charBUF[4] = 0x41;
    break;
  //  $7f,$09,$09,$01,$01, '* F */
  case 'F':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x01;
    charBUF[4] = 0x01;
    break;
  //  0x3E, 0x41, 0x41, 0x51, 0x32,// G
  case 'G':
    charBUF[0] = 0x3e;
    charBUF[1] = 0x41;
    charBUF[2] = 0x41;
    charBUF[3] = 0x51;
    charBUF[4] = 0x32;
    break;
  //  $7f,$08,$08,$08,$7f, '* H */
  case 'H':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x08;
    charBUF[2] = 0x08;
    charBUF[3] = 0x08;
    charBUF[4] = 0x7f;
    break;
  //  $00,$41,$7f,$41,$00, '* I */
  case 'I':
    charBUF[0] = 0x00;
    charBUF[1] = 0x41;
    charBUF[2] = 0x7f;
    charBUF[3] = 0x41;
    charBUF[4] = 0x00;
    break;
  //  $20,$40,$41,$3f,$01, '* J */
  case 'J':
    charBUF[0] = 0x20;
    charBUF[1] = 0x40;
    charBUF[2] = 0x41;
    charBUF[3] = 0x3f;
    charBUF[4] = 0x01;
    break;
  //  $7f,$08,$14,$22,$41, '* K */
  case 'K':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x08;
    charBUF[2] = 0x14;
    charBUF[3] = 0x22;
    charBUF[4] = 0x41;
    break;
  //  $7f,$40,$40,$40,$40, '* L */
  case 'L':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x40;
    charBUF[2] = 0x40;
    charBUF[3] = 0x40;
    charBUF[4] = 0x40;
    break;
  //  0x7F, 0x02, 0x04, 0x02, 0x7F,// M
  case 'M':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x02;
    charBUF[2] = 0x04;
    charBUF[3] = 0x02;
    charBUF[4] = 0x7f;
    break;
  //  $7f,$04,$08,$10,$7f, '* N */
  case 'N':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x04;
    charBUF[2] = 0x08;
    charBUF[3] = 0x10;
    charBUF[4] = 0x7f;
    break;
  //  $3e,$41,$41,$41,$3e, '* O */
  case 'O':
    charBUF[0] = 0x3e;
    charBUF[1] = 0x41;
    charBUF[2] = 0x41;
    charBUF[3] = 0x41;
    charBUF[4] = 0x3e;
    break;
  //  $7f,$09,$09,$09,$06, '* P */
  case 'P':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x09;
    charBUF[4] = 0x06;
    break;
  //  $3e,$41,$51,$21,$5e, '* Q */
  case 'Q':
    charBUF[0] = 0x3e;
    charBUF[1] = 0x41;
    charBUF[2] = 0x51;
    charBUF[3] = 0x21;
    charBUF[4] = 0x5e;
    break;
  // 7F, 0x09, 0x19, 0x29, 0x46, // R
  case 'R':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x19;
    charBUF[3] = 0x29;
    charBUF[4] = 0x46;
    break;
  //0x46, 0x49, 0x49, 0x49, 0x31,// S
  case 'S':
    charBUF[0] = 0x46;
    charBUF[1] = 0x49;
    charBUF[2] = 0x49;
    charBUF[3] = 0x49;
    charBUF[4] = 0x31;
    break;
  //0x01, 0x01, 0x7F, 0x01, 0x01,// T
  case 'T':
    charBUF[0] = 0x01;
    charBUF[1] = 0x01;
    charBUF[2] = 0x7f;
    charBUF[3] = 0x01;
    charBUF[4] = 0x01;
    break;
 //0x3F, 0x40, 0x40, 0x40, 0x3F,// U
  case 'U':
    charBUF[0] = 0x3f;
    charBUF[1] = 0x40;
    charBUF[2] = 0x40;
    charBUF[3] = 0x40;
    charBUF[4] = 0x3f;
    break;
  //0x1F, 0x20, 0x40, 0x20, 0x1F,// V
  case 'V':
    charBUF[0] = 0x1f;
    charBUF[1] = 0x20;
    charBUF[2] = 0x40;
    charBUF[3] = 0x20;
    charBUF[4] = 0x1f;
    break;
    // 0x7F, 0x20, 0x18, 0x20, 0x7F,// W
  case 'W':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x20;
    charBUF[2] = 0x18;
    charBUF[3] = 0x20;
    charBUF[4] = 0x7f;
    break;
    //0x63, 0x14, 0x08, 0x14, 0x63,// X
  case 'X':
    charBUF[0] = 0x63;
    charBUF[1] = 0x14;
    charBUF[2] = 0x08;
    charBUF[3] = 0x14;
    charBUF[4] = 0x63;
    break;
    //0x03, 0x04, 0x78, 0x04, 0x03,// Y
  case 'Y':
    charBUF[0] = 0x03;
    charBUF[1] = 0x04;
    charBUF[2] = 0x78;
    charBUF[3] = 0x04;
    charBUF[4] = 0x03;
    break;
    // 0x61, 0x51, 0x49, 0x45, 0x43,// Z
  case 'Z':
    charBUF[0] = 0x61;
    charBUF[1] = 0x51;
    charBUF[2] = 0x49;
    charBUF[3] = 0x45;
    charBUF[4] = 0x43;
    break;
    //0x00, 0x00, 0x5F, 0x00, 0x00,// !
  case '!':
    charBUF[0] = 0x00;
    charBUF[1] = 0x00;
    charBUF[2] = 0x5f;
    charBUF[3] = 0x00;
    charBUF[4] = 0x00;
    break;
    //0x00, 0x07, 0x00, 0x07, 0x00,// "
  case '"':
    charBUF[0] = 0x00;
    charBUF[1] = 0x07;
    charBUF[2] = 0x00;
    charBUF[3] = 0x07;
    charBUF[4] = 0x00;
    break;
    //0x14, 0x7F, 0x14, 0x7F, 0x14,// #
  case '#':
    charBUF[0] = 0x14;
    charBUF[1] = 0x7f;
    charBUF[2] = 0x14;
    charBUF[3] = 0x7f;
    charBUF[4] = 0x14;
    break;
    //0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
  case '$':
    charBUF[0] = 0x24;
    charBUF[1] = 0x2a;
    charBUF[2] = 0x7f;
    charBUF[3] = 0x2a;
    charBUF[4] = 0x12;
    break;
    //0x23, 0x13, 0x08, 0x64, 0x62,// %
  case '%':
    charBUF[0] = 0x23;
    charBUF[1] = 0x13;
    charBUF[2] = 0x08;
    charBUF[3] = 0x64;
    charBUF[4] = 0x62;
    break;
    //0x36, 0x49, 0x55, 0x22, 0x50,// &
  case '&':
    charBUF[0] = 0x36;
    charBUF[1] = 0x49;
    charBUF[2] = 0x55;
    charBUF[3] = 0x22;
    charBUF[4] = 0x50;
    break;
    //0x00, 0x05, 0x03, 0x00, 0x00,// '
  case '\'':
    charBUF[0] = 0x00;
    charBUF[1] = 0x00;
    charBUF[2] = 0x5f;
    charBUF[3] = 0x00;
    charBUF[4] = 0x00;
    break;
    //0x00, 0x1C, 0x22, 0x41, 0x00,// (
  case '(':
    charBUF[0] = 0x00;
    charBUF[1] = 0x1c;
    charBUF[2] = 0x22;
    charBUF[3] = 0x41;
    charBUF[4] = 0x00;
    break;
    //0x00, 0x41, 0x22, 0x1C, 0x00,// )
  case ')':
    charBUF[0] = 0x00;
    charBUF[1] = 0x41;
    charBUF[2] = 0x22;
    charBUF[3] = 0x1c;
    charBUF[4] = 0x00;
    break;
    //0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
  case '*':
    charBUF[0] = 0x08;
    charBUF[1] = 0x2a;
    charBUF[2] = 0x1c;
    charBUF[3] = 0x2a;
    charBUF[4] = 0x08;
    break;
    //0x08, 0x08, 0x3E, 0x08, 0x08,// +
  case '+':
    charBUF[0] = 0x08;
    charBUF[1] = 0x08;
    charBUF[2] = 0x3e;
    charBUF[3] = 0x08;
    charBUF[4] = 0x08;
    break;
    //0x00, 0x50, 0x30, 0x00, 0x00,// ,
  case ',':
    charBUF[0] = 0x00;
    charBUF[1] = 0x50;
    charBUF[2] = 0x30;
    charBUF[3] = 0x00;
    charBUF[4] = 0x00;
    break;
    //0x08, 0x08, 0x08, 0x08, 0x08,// -
  case '-':
    charBUF[0] = 0x08;
    charBUF[1] = 0x08;
    charBUF[2] = 0x08;
    charBUF[3] = 0x08;
    charBUF[4] = 0x08;
    break;
    //0x00, 0x60, 0x60, 0x00, 0x00,// .
  case '.':
    charBUF[0] = 0x00;
    charBUF[1] = 0x60;
    charBUF[2] = 0x60;
    charBUF[3] = 0x00;
    charBUF[4] = 0x00;
    break;
    //0x20, 0x10, 0x08, 0x04, 0x02,// /
  case '/':
    charBUF[0] = 0x20;
    charBUF[1] = 0x10;
    charBUF[2] = 0x08;
    charBUF[3] = 0x04;
    charBUF[4] = 0x02;
    break;
    //0x00, 0x00, 0x7F, 0x41, 0x41,// [
  case '[':
    charBUF[0] = 0x00;
    charBUF[1] = 0x00;
    charBUF[2] = 0x7f;
    charBUF[3] = 0x41;
    charBUF[4] = 0x41;
    break;
    // 0x41, 0x41, 0x7F, 0x00, 0x00,// ]
  case ']':
    charBUF[0] = 0x41;
    charBUF[1] = 0x41;
    charBUF[2] = 0x7f;
    charBUF[3] = 0x00;
    charBUF[4] = 0x00;
    break;
    //0x04, 0x02, 0x01, 0x02, 0x04,// ^
  case '^':
    charBUF[0] = 0x04;
    charBUF[1] = 0x02;
    charBUF[2] = 0x01;
    charBUF[3] = 0x02;
    charBUF[4] = 0x04;
    break;
    //0x40, 0x40, 0x40, 0x40, 0x40,// _
  case '_':
    charBUF[0] = 0x40;
    charBUF[1] = 0x40;
    charBUF[2] = 0x40;
    charBUF[3] = 0x40;
    charBUF[4] = 0x40;
    break;
    //0x00, 0x01, 0x02, 0x04, 0x00,// `
  case '`':
    charBUF[0] = 0x00;
    charBUF[1] = 0x01;
    charBUF[2] = 0x02;
    charBUF[3] = 0x04;
    charBUF[4] = 0x00;
    break;
    //0x00, 0x36, 0x36, 0x00, 0x00,// :
  case ':':
    charBUF[0] = 0x00;
    charBUF[1] = 0x36;
    charBUF[2] = 0x36;
    charBUF[3] = 0x00;
    charBUF[4] = 0x00;
    break;
    //0x00, 0x56, 0x36, 0x00, 0x00,// ;
  case ';':
    charBUF[0] = 0x00;
    charBUF[1] = 0x56;
    charBUF[2] = 0x36;
    charBUF[3] = 0x00;
    charBUF[4] = 0x00;
    break;
    //0x00, 0x08, 0x14, 0x22, 0x41,// <
  case '<':
    charBUF[0] = 0x00;
    charBUF[1] = 0x08;
    charBUF[2] = 0x14;
    charBUF[3] = 0x22;
    charBUF[4] = 0x41;
    break;
    //0x14, 0x14, 0x14, 0x14, 0x14,// =
  case '=':
    charBUF[0] = 0x14;
    charBUF[1] = 0x14;
    charBUF[2] = 0x14;
    charBUF[3] = 0x14;
    charBUF[4] = 0x14;
    break;
    //0x41, 0x22, 0x14, 0x08, 0x00,// >
  case '>':
    charBUF[0] = 0x41;
    charBUF[1] = 0x22;
    charBUF[2] = 0x14;
    charBUF[3] = 0x08;
    charBUF[4] = 0x00;
    break;
    //0x02, 0x01, 0x51, 0x09, 0x06,// ?
  case '?':
    charBUF[0] = 0x02;
    charBUF[1] = 0x01;
    charBUF[2] = 0x51;
    charBUF[3] = 0x09;
    charBUF[4] = 0x06;
    break;
    //0x32, 0x49, 0x79, 0x41, 0x3E,// @
  case '@':
    charBUF[0] = 0x32;
    charBUF[1] = 0x49;
    charBUF[2] = 0x79;
    charBUF[3] = 0x41;
    charBUF[4] = 0x3e;
    break;
    //0x00, 0x08, 0x36, 0x41, 0x00,// {
  case '{':
    charBUF[0] = 0x00;
    charBUF[1] = 0x08;
    charBUF[2] = 0x36;
    charBUF[3] = 0x41;
    charBUF[4] = 0x00;
    break;
    //0x00, 0x00, 0x7F, 0x00, 0x00,// |
  case '|':
    charBUF[0] = 0x00;
    charBUF[1] = 0x00;
    charBUF[2] = 0x7f;
    charBUF[3] = 0x00;
    charBUF[4] = 0x00;
    break;
    //0x00, 0x41, 0x36, 0x08, 0x00,// }
  case '}':
    charBUF[0] = 0x00;
    charBUF[1] = 0x41;
    charBUF[2] = 0x36;
    charBUF[3] = 0x08;
    charBUF[4] = 0x00;
    break;
  case ' ':
    charBUF[0] = 0x00;
    charBUF[1] = 0x00;
    charBUF[2] = 0x00;
    charBUF[3] = 0x00;
    charBUF[4] = 0x00;
    break;

    // still need qrstuvqxyz

  // i think this will be a square?
  default:
    charBUF[0] = 0x7f;
    charBUF[1] = 0x41;
    charBUF[2] = 0x41;
    charBUF[3] = 0x41;
    charBUF[4] = 0x7f;
  }
  for (i=0;i<8;i++) {
    lcdCommand(charBUF[i]);
  }
}

void lcdChar(unsigned char letter){
  // 0b10000000=0x80
  //setGPIO(0x80); // RS=1, we going to send data to be displayed
  Delay10TCYx(0); // let things settle down
  setGPIO(letter); // send display character
  // Now we need to toggle the enable pin (EN) for the display to take effect
  // setGPIO( 0xc0); // RS=1, EN=1
  // Delay10TCYx(0); // let things settle down, this time just needs to be long enough for the chip to detect it as high
  // setGPIO(0x00); // RS=0, EN=0 // this completes the enable pin toggle
  // Delay10TCYx(0);
}


void lcdGoTo(char pos){
  // add 0x80 to be able to use HD44780 position convention
  lcdCommand(0x80+pos);
}

void lcdWriteString(rom unsigned char *s){
  while(*s)
    sendLetter(*s++);
}

void Set_Start_Column(unsigned char d)
{
  lcdCommand(0x00+d%16);		// Set Lower Column Start Address for Page Addressing Mode
  //   Default => 0x00
  lcdCommand(0x10+d/16);		// Set Higher Column Start Address for Page Addressing Mode
  //   Default => 0x10
}


void Set_Display_On_Off(unsigned char d)
{
  lcdCommand(0xAE|d);			// Set Display On/Off
  //   Default => 0xAE
  //     0xAE (0x00) => Display Off
  //     0xAF (0x01) => Display On
}
//--------------------------------------------------------------------------

void Set_Start_Page(unsigned char d)
{
  lcdCommand(0xB0|d);			// Set Page Start Address for Page Addressing Mode
  //   Default => 0xB0 (0x00)
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_RAM(unsigned char Data)
{
  unsigned char i,j;

  for(i=0;i<8;i++)
    {
      DC =0;
      Set_Start_Page(i);
      Set_Start_Column(0x00);
      DC =1;
      for(j=0;j<132;j++)
	{
	  lcdCommand(Data);
	}
    }
}


void Checkerboard()
{
  unsigned char i,j;

  for(i=0;i<8;i++)
    {
      DC = 0;
      Set_Start_Page(i);
      Set_Start_Column(0x00);
      DC =1;
      for(j=0;j<66;j++)
	{
	  lcdCommand(0x55);
	  lcdCommand(0xaa);
	}
    }
}

int lcdWrite(char *stra, int len)
{
    int j = 0;
  // Master
  OpenSPI1(SPI_FOSC_64, MODE_00, SMPEND);
  TRISDbits.RD1=0; // our chip select pin needs to be an output so that we can toggle it
  TRISDbits.RD3=0; // our  pin needs to be an output so that we can toggle it
  TRISDbits.RD4=0; // our  pin needs to be an output so that we can toggle it


  RES = 0;
  for(i=0;i<200;i++)
    {
      Delay10TCYx(0);
    }
  RES = 1;
  CS = 1;
  DC = 0;

  Set_Display_On_Off(0x01); // Display On (0x00/0x01) 
  Fill_RAM(0x00); // Clear Screen 
  
  DC = 0;
  Set_Start_Page(0);
  Set_Start_Column(0x00);
  DC = 1;
  sendLetter('A');
  sendLetter('B');
  while (j<len) {
    sendLetter(stra[j]);
    j++;
  }

  return 1;
}
