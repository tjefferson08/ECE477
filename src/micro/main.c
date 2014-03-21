/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <stdio.h>
#include <p18cxxx.h>
#include <spi.h>
#include <stdlib.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
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
  //  $7f,$09,$09,$09,$01, '* F */
  case 'F':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x09;
    charBUF[4] = 0x01;
    break;
  //  $3e,$41,$49,$49,$7a, '* G */
  case 'G':
    charBUF[0] = 0x3e;
    charBUF[1] = 0x41;
    charBUF[2] = 0x49;
    charBUF[3] = 0x49;
    charBUF[4] = 0x7a;
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
  //  $7f,$02,$0c,$02,$7f, '* M */
  case 'M':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x02;
    charBUF[2] = 0x0c;
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
  case 'Q':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x09;
    charBUF[4] = 0x06;
    break;
  case 'R':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x09;
    charBUF[4] = 0x06;
    break;
  case 'S':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x09;
    charBUF[4] = 0x06;
    break;
  case 'P':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x09;
    charBUF[4] = 0x06;
    break;
  case 'T':
    charBUF[0] = 0x40;
    charBUF[1] = 0x40;
    charBUF[2] = 0x7f;
    charBUF[3] = 0x40;
    charBUF[4] = 0x40;
    break;
  case 'U':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x09;
    charBUF[4] = 0x06;
    break;
  case 'V':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x09;
    charBUF[4] = 0x06;
    break;
  case 'W':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x09;
    charBUF[4] = 0x06;
    break;
  case 'X':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x09;
    charBUF[4] = 0x06;
    break;
  case 'Y':
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x09;
    charBUF[4] = 0x06;
    break;
  case 'Z':
    charBUF[0] = 0x;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x09;
    charBUF[4] = 0x06;
    break;
  case ',': // ????
    charBUF[0] = 0x7f;
    charBUF[1] = 0x09;
    charBUF[2] = 0x09;
    charBUF[3] = 0x09;
    charBUF[4] = 0x06;
    break;
  case '!':
    charBUF[0] = 0x00;
    charBUF[1] = 0x00;
    charBUF[2] = 0x7d;
    charBUF[3] = 0x00;
    charBUF[4] = 0x00;
    break;
  case '-':
    charBUF[0] = 0x00;
    charBUF[1] = 0x10;
    charBUF[2] = 0x10;
    charBUF[3] = 0x10;
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
//--------------------------------------------------------------------------

void Set_Addressing_Mode(unsigned char d)
{
  lcdCommand(0x20);			// Set Memory Addressing Mode
  lcdCommand(d);			//   Default => 0x02
  //     0x00 => Horizontal Addressing Mode
  //     0x01 => Vertical Addressing Mode
  //     0x02 => Page Addressing Mode
}
//--------------------------------------------------------------------------

void Set_Column_Address(unsigned char a, unsigned char b)
{
  lcdCommand(0x21);			// Set Column Address
  lcdCommand(a);			//   Default => 0x00 (Column Start Address)
  lcdCommand(b);			//   Default => 0x83 (Column End Address)
}
//--------------------------------------------------------------------------

void Set_Page_Address(unsigned char a, unsigned char b)
{
  lcdCommand(0x22);			// Set Page Address
  lcdCommand(a);			//   Default => 0x00 (Page Start Address)
  lcdCommand(b);			//   Default => 0x07 (Page End Address)
}
//--------------------------------------------------------------------------

void Set_Start_Line(unsigned char d)
{
  lcdCommand(0x40|d);			// Set Display Start Line
  //   Default => 0x40 (0x00)
}
//--------------------------------------------------------------------------

void Set_Contrast_Control(unsigned char d)
{
  lcdCommand(0x81);			// Set Contrast Control for Bank 0
  lcdCommand(d);			//   Default => 0x80
}
//--------------------------------------------------------------------------

void Set_Area_Brightness(unsigned char d)
{
  lcdCommand(0x82);			// Set Brightness for Area Color Banks
  lcdCommand(d);			//   Default => 0x80
}
//--------------------------------------------------------------------------

void Set_Segment_Remap(unsigned char d)
{
  lcdCommand(0xA0|d);			// Set Segment Re-Map
  //   Default => 0xA0
  //     0xA0 (0x00) => Column Address 0 Mapped to SEG0
  //     0xA1 (0x01) => Column Address 0 Mapped to SEG131
}
//--------------------------------------------------------------------------

void Set_Entire_Display(unsigned char d)
{
  lcdCommand(0xA4|d);			// Set Entire Display On / Off
  //   Default => 0xA4
  //     0xA4 (0x00) => Normal Display
  //     0xA5 (0x01) => Entire Display On
}
//--------------------------------------------------------------------------

void Set_Inverse_Display(unsigned char d)
{
  lcdCommand(0xA6|d);			// Set Inverse Display On/Off
  //   Default => 0xA6
  //     0xA6 (0x00) => Normal Display
  //     0xA7 (0x01) => Inverse Display On
}
//--------------------------------------------------------------------------

void Set_Multiplex_Ratio(unsigned char d)
{
  lcdCommand(0xA8);			// Set Multiplex Ratio
  lcdCommand(d);			//   Default => 0x3F (1/64 Duty)
}
//--------------------------------------------------------------------------

void Set_Dim_Mode(unsigned char a, unsigned char b)
{
  lcdCommand(0xAB);			// Set Dim Mode Configuration
  lcdCommand(0X00);			//           => (Dummy Write for First Parameter)
  lcdCommand(a);			//   Default => 0x80 (Contrast Control for Bank 0)
  lcdCommand(b);			//   Default => 0x80 (Brightness for Area Color Banks)
  lcdCommand(0xAC);			// Set Display On in Dim Mode
}
//--------------------------------------------------------------------------

void Set_Master_Config(unsigned char d)
{
  lcdCommand(0xAD);			// Set Master Configuration
  lcdCommand(0x8E|d);			//   Default => 0x8E
  //     0x8E (0x00) => Select External VCC Supply
  //     0x8F (0x01) => Select Internal DC/DC Voltage Converter
}
//--------------------------------------------------------------------------

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
//--------------------------------------------------------------------------

void Set_Common_Remap(unsigned char d)
{
  lcdCommand(0xC0|d);			// Set COM Output Scan Direction
  //   Default => 0xC0
  //     0xC0 (0x00) => Scan from COM0 to 63
  //     0xC8 (0x08) => Scan from COM63 to 0
}
//--------------------------------------------------------------------------

void Set_Display_Offset(unsigned char d)
{
  lcdCommand(0xD3);			// Set Display Offset
  lcdCommand(d);			//   Default => 0x00
}
//--------------------------------------------------------------------------

void Set_Display_Clock(unsigned char d)
{
  lcdCommand(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
  lcdCommand(d);			//   Default => 0x70
  //     D[3:0] => Display Clock Divider
  //     D[7:4] => Oscillator Frequency
}
//--------------------------------------------------------------------------

void Set_Area_Color(unsigned char d)
{
  lcdCommand(0xD8);			// Set Area Color Mode On/Off & Low Power Display Mode
  lcdCommand(d);			//   Default => 0x00 (Monochrome Mode & Normal Power Display Mode)
}
//--------------------------------------------------------------------------

void Set_Precharge_Period(unsigned char d)
{
  lcdCommand(0xD9);			// Set Pre-Charge Period
  lcdCommand(d);			//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
  //     D[3:0] => Phase 1 Period in 1~15 Display Clocks
  //     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}
//--------------------------------------------------------------------------

void Set_Common_Config(unsigned char d)
{
  lcdCommand(0xDA);			// Set COM Pins Hardware Configuration
  lcdCommand(0x02|d);			//   Default => 0x12 (0x10)
  //     Alternative COM Pin Configuration
  //     Disable COM Left/Right Re-Map
}
//--------------------------------------------------------------------------

void Set_VCOMH(unsigned char d)
{
  lcdCommand(0xDB);			// Set VCOMH Deselect Level
  lcdCommand(d);			//   Default => 0x34 (0.77*VCC)
}
//--------------------------------------------------------------------------

void Set_Read_Modify_Write(unsigned char d)
{
  lcdCommand(0xE0|d);			// Set Read Modify Write Mode
  //   Default => 0xE0
  //     0xE0 (0x00) => Enter Read Modify Write
  //     0xEE (0x0E) => Exit Read Modify Write
}
//--------------------------------------------------------------------------

void Set_NOP()
{
  lcdCommand(0xE3);			// Command for No Operation
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Bank Color & Look Up Table Setting (Partial Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_LUT(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
  lcdCommand(0x91);			// Define Look Up Table of Area Color
  lcdCommand(a);			//   Define Bank 0 Pulse Width
  lcdCommand(b);			//   Define Color A Pulse Width
  lcdCommand(c);			//   Define Color B Pulse Width
  lcdCommand(d);			//   Define Color C Pulse Width
}


void Set_Bank_Color()
{
  lcdCommand(0x92);			// Define Area Color for Bank 1~16 (Page 0)
  lcdCommand(0x00);			//   Define Bank 1~4 as Color A
  lcdCommand(0x55);			//   Define Bank 5~8 as Color B
  lcdCommand(0xAA);			//   Define Bank 9~12 as Color C
  lcdCommand(0xFF);			//   Define Bank 13~16 as Color D

  lcdCommand(0x93);			// Define Area Color for Bank 17~32 (Page 1)
  lcdCommand(0xFF);			//   Define Bank 17~32 as Color D
  lcdCommand(0xFF);
  lcdCommand(0xFF);
  lcdCommand(0xFF);
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


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Initialization
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED_Init()
{
  unsigned char i;
  int Max_Column = 128;
  int Max_Row = 32;
  int Brightness = 0xBF;
  int XLevelL = 0x04;
  int XLevelH = 0x10;

  setGPIO(0x00);
  for(i=0;i<200;i++)
    {
      Delay10TCYx(0);
    }
  setGPIO(0x00);

  Set_Display_On_Off(0x00);		// Display Off (0x00/0x01)
  Set_Display_Clock(0x10);		// Set Clock as 160 Frames/Sec
  Set_Multiplex_Ratio(0x1F);		// 1/32 Duty (0x0F~0x3F)
  Set_Display_Offset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
  Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x3F)
  Set_Master_Config(0x00);		// Disable Embedded DC/DC Converter (0x00/0x01)
  Set_Area_Color(0x05);			// Set Monochrome & Low Power Save Mode
  Set_Addressing_Mode(0x02);		// Set Page Addressing Mode (0x00/0x01/0x02)
  Set_Segment_Remap(0x01);		// Set SEG/Column Mapping (0x00/0x01)
  Set_Common_Remap(0x08);			// Set COM/Row Scan Direction (0x00/0x08)
  Set_Common_Config(0x10);		// Set Alternative Configuration (0x00/0x10)
  Set_LUT(0x3F,0x3F,0x3F,0x3F);		// Define All Banks Pulse Width as 64 Clocks
  Set_Contrast_Control(BRIGHTNESS);	// Set SEG Output Current
  Set_Area_Brightness(Brightness);	// Set Brightness for Area Color Banks
  Set_Precharge_Period(0xD2);		// Set Pre-Charge as 13 Clocks & Discharge as 2 Clock
  Set_VCOMH(0x08);			// Set VCOM Deselect Level
  Set_Entire_Display(0x00);		// Disable Entire Display On (0x00/0x01)
  Set_Inverse_Display(0x00);		// Disable Inverse Display On (0x00/0x01)

  Fill_RAM(0x00);				// Clear Screen

  Set_Display_On_Off(0x01);		// Display On (0x00/0x01)
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

void main(void)
{
  // Master
  OpenSPI1(SPI_FOSC_64, MODE_10, SMPEND);
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

  //OLED_Init();

  /*Set_Display_On_Off(0x00); // Display Off (0x00/0x01) 
    Set_Display_Clock(0x10); // Set Clock as 160 Frames/Sec 
    Set_Multiplex_Ratio(0x1F); // 1/32 Duty (0x0F~0x3F) 
    Set_Display_Offset(0x00); // Shift Mapping RAM Counter (0x00~0x3F) 
    Set_Start_Line(0x00); // Set Mapping RAM Display Start Line (0x00~0x3F) 
    Set_Master_Config(0x00); // Disable Embedded DC/DC Converter (0x00/0x01) 
    Set_Area_Color(0x05); // Set Monochrome & Low Power Save Mode 
    Set_Addressing_Mode(0x02); // Set Page Addressing Mode (0x00/0x01/0x02) 
    Set_Segment_Remap(0x01); // Set SEG/Column Mapping (0x00/0x01) 
    Set_Common_Remap(0x08); //Set COM/Row Scan Direction (0x00/0x08) 
    Set_Common_Config(0x10); // Set Alternative Configuration (0x00/0x10) 
    Set_LUT(0x3F,0x3F,0x3F,0x3F); // Define All Banks Pulse Width as 64 Clocks 
    Set_Contrast_Control(BRIGHTNESS); // Set SEG Output Current 
    Set_Area_Brightness(BRIGHTNESS); // Set Brightness for Area Color Banks 
    Set_Precharge_Period(0xD2); // St Pre?Charge as 13 Clocks & Discharge as 2 Clock 
    Set_VCOMH(0x08); // Set VCOM Deselect Level 
    Set_Entire_Display(0x00); // Disable Entire Display On (0x00/0x01) 
    Set_Inverse_Display(0x00); // Disable Inverse Display On (0x00/0x01) */
  Set_Display_On_Off(0x01); // Display On (0x00/0x01) 
  //lcdCommand(0xA4); // RAM ENABLED
  Fill_RAM(0x00); // Clear Screen 

  DC = 0;
  Set_Start_Page(0);
  Set_Start_Column(0x00);
  DC = 1;
  //   lcdWriteString("AllAboutEE.com"); // using the string function
  //   lcdGoTo(0x40); // go to line two
  //38, 6e, c6, c6, fe, c6, c6, 00
  sendLetter('A');
  sendLetter('A');
  sendLetter('A');
  sendLetter('H');
  sendLetter('H');
  sendLetter('H');
  sendLetter('H');
  sendLetter('H');
  sendLetter('H');
  /*    lcdChar('P');
	lcdChar('I');
	lcdChar(' ');
	lcdChar('L');
	lcdChar('i');
	lcdChar('b');
	lcdChar('r');
	lcdChar('a');
	lcdChar('r');
	lcdChar('y');*/

  while(1){

  }
}

