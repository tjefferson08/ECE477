/* 
 * File:   lcd_functions.c
 * Author: team2
 *
 * Created on March 8, 2014, 3:24 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
/*void oled_Command(unsigned char Data)
{
GPIO_Write(GPIOB, Data);//6800 mode
GPIO_ResetBits(GPIOC, RS);
GPIO_ResetBits(GPIOC, CS1);
GPIO_ResetBits(GPIOC, RW);
GPIO_SetBits(GPIOC, E1);
OLED_uDelay(100);
GPIO_ResetBits(GPIOC, E1);
OLED_uDelay(100);
GPIO_SetBits(GPIOC, RW);
GPIO_SetBits(GPIOC, CS1);
GPIO_SetBits(GPIOC, RS);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

void oled_Data(unsigned char Data)
{
GPIO_Write(GPIOB, Data);//6800 mode
GPIO_SetBits(GPIOC, RS);
GPIO_ResetBits(GPIOC, CS1);
GPIO_ResetBits(GPIOC, RW);
GPIO_SetBits(GPIOC, E1);
OLED_uDelay(100);
GPIO_ResetBits(GPIOC, E1);
OLED_uDelay(100);
GPIO_SetBits(GPIOC, RW);
GPIO_SetBits(GPIOC, CS1);
GPIO_SetBits(GPIOC, RS);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

void Set_Start_Column_12832(unsigned char d)
{
	oled_Command(0x00+d%16);		// Set Lower Column Start Address for Page Addressing Mode
						//   Default => 0x00
	oled_Command(0x10+d/16);		// Set Higher Column Start Address for Page Addressing Mode
						//   Default => 0x10
}
//--------------------------------------------------------------------------

void Set_Addressing_Mode_12832(unsigned char d)
{
	oled_Command(0x20);			// Set Memory Addressing Mode
	oled_Command(d);			//   Default => 0x02
						//     0x00 => Horizontal Addressing Mode
						//     0x01 => Vertical Addressing Mode
						//     0x02 => Page Addressing Mode
}
//--------------------------------------------------------------------------

void Set_Column_Address_12832(unsigned char a, unsigned char b)
{
	oled_Command(0x21);			// Set Column Address
	oled_Command(a);			//   Default => 0x00 (Column Start Address)
	oled_Command(b);			//   Default => 0x83 (Column End Address)
}
//--------------------------------------------------------------------------

void Set_Page_Address_12832(unsigned char a, unsigned char b)
{
	oled_Command(0x22);			// Set Page Address
	oled_Command(a);			//   Default => 0x00 (Page Start Address)
	oled_Command(b);			//   Default => 0x07 (Page End Address)
}
//--------------------------------------------------------------------------

void Set_Start_Line_12832(unsigned char d)
{
	oled_Command(0x40|d);			// Set Display Start Line
						//   Default => 0x40 (0x00)
}
//--------------------------------------------------------------------------

void Set_Contrast_Control_12832(unsigned char d)
{
	oled_Command(0x81);			// Set Contrast Control for Bank 0
	oled_Command(d);			//   Default => 0x80
}
//--------------------------------------------------------------------------

void Set_Area_Brightness_12832(unsigned char d)
{
	oled_Command(0x82);			// Set Brightness for Area Color Banks
	oled_Command(d);			//   Default => 0x80
}
//--------------------------------------------------------------------------

void Set_Segment_Remap_12832(unsigned char d)
{
	oled_Command(0xA0|d);			// Set Segment Re-Map
						//   Default => 0xA0
						//     0xA0 (0x00) => Column Address 0 Mapped to SEG0
						//     0xA1 (0x01) => Column Address 0 Mapped to SEG131
}
//--------------------------------------------------------------------------

void Set_Entire_Display_12832(unsigned char d)
{
	oled_Command(0xA4|d);			// Set Entire Display On / Off
						//   Default => 0xA4
						//     0xA4 (0x00) => Normal Display
						//     0xA5 (0x01) => Entire Display On
}
//--------------------------------------------------------------------------

void Set_Inverse_Display_12832(unsigned char d)
{
	oled_Command(0xA6|d);			// Set Inverse Display On/Off
						//   Default => 0xA6
						//     0xA6 (0x00) => Normal Display
						//     0xA7 (0x01) => Inverse Display On
}
//--------------------------------------------------------------------------

void Set_Multiplex_Ratio_12832(unsigned char d)
{
	oled_Command(0xA8);			// Set Multiplex Ratio
	oled_Command(d);			//   Default => 0x3F (1/64 Duty)
}
//--------------------------------------------------------------------------

void Set_Dim_Mode_12832(unsigned char a, unsigned char b)
{
	oled_Command(0xAB);			// Set Dim Mode Configuration
	oled_Command(0X00);			//           => (Dummy Write for First Parameter)
	oled_Command(a);			//   Default => 0x80 (Contrast Control for Bank 0)
	oled_Command(b);			//   Default => 0x80 (Brightness for Area Color Banks)
	oled_Command(0xAC);			// Set Display On in Dim Mode
}
//--------------------------------------------------------------------------

void Set_Master_Config_12832(unsigned char d)
{
	oled_Command(0xAD);			// Set Master Configuration
	oled_Command(0x8E|d);			//   Default => 0x8E
						//     0x8E (0x00) => Select External VCC Supply
						//     0x8F (0x01) => Select Internal DC/DC Voltage Converter
}
//--------------------------------------------------------------------------

void Set_Display_On_Off_12832(unsigned char d)
{
	oled_Command(0xAE|d);			// Set Display On/Off
						//   Default => 0xAE
						//     0xAE (0x00) => Display Off
						//     0xAF (0x01) => Display On
}
//--------------------------------------------------------------------------

void Set_Start_Page_12832(unsigned char d)
{
	oled_Command(0xB0|d);			// Set Page Start Address for Page Addressing Mode
						//   Default => 0xB0 (0x00)
}
//--------------------------------------------------------------------------

void Set_Common_Remap_12832(unsigned char d)
{
	oled_Command(0xC0|d);			// Set COM Output Scan Direction
						//   Default => 0xC0
						//     0xC0 (0x00) => Scan from COM0 to 63
						//     0xC8 (0x08) => Scan from COM63 to 0
}
//--------------------------------------------------------------------------

void Set_Display_Offset_12832(unsigned char d)
{
	oled_Command(0xD3);			// Set Display Offset
	oled_Command(d);			//   Default => 0x00
}
//--------------------------------------------------------------------------

void Set_Display_Clock_12832(unsigned char d)
{
	oled_Command(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
	oled_Command(d);			//   Default => 0x70
						//     D[3:0] => Display Clock Divider
						//     D[7:4] => Oscillator Frequency
}
//--------------------------------------------------------------------------

void Set_Area_Color_12832(unsigned char d)
{
	oled_Command(0xD8);			// Set Area Color Mode On/Off & Low Power Display Mode
	oled_Command(d);			//   Default => 0x00 (Monochrome Mode & Normal Power Display Mode)
}
//--------------------------------------------------------------------------

void Set_Precharge_Period_12832(unsigned char d)
{
	oled_Command(0xD9);			// Set Pre-Charge Period
	oled_Command(d);			//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
						//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
						//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}
//--------------------------------------------------------------------------

void Set_Common_Config_12832(unsigned char d)
{
	oled_Command(0xDA);			// Set COM Pins Hardware Configuration
	oled_Command(0x02|d);			//   Default => 0x12 (0x10)
						//     Alternative COM Pin Configuration
						//     Disable COM Left/Right Re-Map
}
//--------------------------------------------------------------------------

void Set_VCOMH_12832(unsigned char d)
{
	oled_Command(0xDB);			// Set VCOMH Deselect Level
	oled_Command(d);			//   Default => 0x34 (0.77*VCC)
}
//--------------------------------------------------------------------------

void Set_Read_Modify_Write_12832(unsigned char d)
{
	oled_Command(0xE0|d);			// Set Read Modify Write Mode
						//   Default => 0xE0
						//     0xE0 (0x00) => Enter Read Modify Write
						//     0xEE (0x0E) => Exit Read Modify Write
}
//--------------------------------------------------------------------------

void Set_NOP_12832()
{
	oled_Command(0xE3);			// Command for No Operation
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_RAM_12832(unsigned char Data)
{
unsigned char i,j;

	for(i=0;i<8;i++)
	{
		Set_Start_Page_12832(i);
		Set_Start_Column_12832(0x00);

		for(j=0;j<132;j++)
		{
			oled_Data(Data);
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)
//
//    a: Start Page
//    b: End Page
//    c: Start Column
//    d: Total Columns
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block_12832(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
unsigned char i,j;

	for(i=a;i<(b+1);i++)
	{
		Set_Start_Page_12832(i);
		Set_Start_Column_12832(c);

		for(j=0;j<d;j++)
		{
			oled_Data(Data);
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Checkboard (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Checkerboard_12832()
{
unsigned char i,j;

	for(i=0;i<8;i++)
	{
		Set_Start_Page_12832(i);
		Set_Start_Column_12832(0x00);

		for(j=0;j<66;j++)
		{
			oled_Data(0x55);
			oled_Data(0xaa);
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Frame (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Frame_12832()
{
unsigned char i,j;
int Max_Column = 128;

	Set_Start_Page_12832(0x00);
	Set_Start_Column_12832(XLevel);

	for(i=0;i<Max_Column;i++)
	{
		oled_Data(0x01);
	}

	Set_Start_Page_12832(0x03);
	Set_Start_Column_12832(XLevel);

	for(i=0;i<Max_Column;i++)
	{
		oled_Data(0x80);
	}

	for(i=0;i<4;i++)
	{
		Set_Start_Page_12832(i);

		for(j=0;j<Max_Column;j+=(Max_Column-1))
		{
			Set_Start_Column_12832(XLevel+j);

			oled_Data(0xFF);
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Character (5x7)
//
//    a: Database
//    b: Ascii
//    c: Start Page
//    d: Start Column
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Font57_12832(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
unsigned char *Src_Pointer=0;
unsigned char i;

	switch(a)
	{
		case 1:
			Src_Pointer=&Ascii_1[(b-1)][0];
			break;
		case 2:
			//Src_Pointer=&Ascii_2[(b-1)][0];
			break;
	}
	Set_Start_Page_12832(c);
	Set_Start_Column_12832(d);

	for(i=0;i<5;i++)
	{
		oled_Data(*Src_Pointer);
		Src_Pointer++;
	}
	oled_Data(0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show String
//
//    a: Database
//    b: Start Page
//    c: Start Column
//    * Must write "0" in the end...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_String_12832(unsigned char a, unsigned char *Data_Pointer, unsigned char b, unsigned char c)
{
unsigned char *Src_Pointer;

	Src_Pointer=Data_Pointer;
	Show_Font57_12832(1,96,b,c);			// No-Break Space
						//   Must be written first before the string start...

	while(1)
	{
		Show_Font57_12832(a,*Src_Pointer,b,c);
		Src_Pointer++;
		c+=6;
		if(*Src_Pointer == 0) break;
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern (Partial or Full Screen)
//
//    a: Start Page
//    b: End Page
//    c: Start Column
//    d: Total Columns
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Pattern_12832(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
unsigned char *Src_Pointer;
unsigned char i,j;

	Src_Pointer=Data_Pointer;
	for(i=a;i<(b+1);i++)
	{
		Set_Start_Page_12832(i);
		Set_Start_Column_12832(c);

		for(j=0;j<d;j++)
		{
			oled_Data(*Src_Pointer);
			Src_Pointer++;
		}
	}
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Bank Color & Look Up Table Setting (Partial Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_LUT_12832(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	oled_Command(0x91);			// Define Look Up Table of Area Color
	oled_Command(a);			//   Define Bank 0 Pulse Width
	oled_Command(b);			//   Define Color A Pulse Width
	oled_Command(c);			//   Define Color B Pulse Width
	oled_Command(d);			//   Define Color C Pulse Width
}


void Set_Bank_Color_12832()
{
	oled_Command(0x92);			// Define Area Color for Bank 1~16 (Page 0)
	oled_Command(0x00);			//   Define Bank 1~4 as Color A
	oled_Command(0x55);			//   Define Bank 5~8 as Color B
	oled_Command(0xAA);			//   Define Bank 9~12 as Color C
	oled_Command(0xFF);			//   Define Bank 13~16 as Color D

	oled_Command(0x93);			// Define Area Color for Bank 17~32 (Page 1)
	oled_Command(0xFF);			//   Define Bank 17~32 as Color D
	oled_Command(0xFF);
	oled_Command(0xFF);
	oled_Command(0xFF);
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Initialization
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED_Init_12832()
{
unsigned char i;
int Max_Column = 128;
int Max_Row = 32;
int Brightness = 0xBF;
int XLevelL = 0x04;
int XLevelH = 0x10;

	GPIO_ResetBits(GPIOC, RES);
	for(i=0;i<200;i++)
	{
		OLED_uDelay(200);
	}
	GPIO_SetBits(GPIOC, RES);

	Set_Display_On_Off_12832(0x00);		// Display Off (0x00/0x01)
	Set_Display_Clock_12832(0x10);		// Set Clock as 160 Frames/Sec
	Set_Multiplex_Ratio_12832(0x1F);		// 1/32 Duty (0x0F~0x3F)
	Set_Display_Offset_12832(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
	Set_Start_Line_12832(0x00);			// Set Mapping RAM Display Start Line (0x00~0x3F)
	Set_Master_Config_12832(0x00);		// Disable Embedded DC/DC Converter (0x00/0x01)
	Set_Area_Color_12832(0x05);			// Set Monochrome & Low Power Save Mode
	Set_Addressing_Mode_12832(0x02);		// Set Page Addressing Mode (0x00/0x01/0x02)
	Set_Segment_Remap_12832(0x01);		// Set SEG/Column Mapping (0x00/0x01)
	Set_Common_Remap_12832(0x08);			// Set COM/Row Scan Direction (0x00/0x08)
	Set_Common_Config_12832(0x10);		// Set Alternative Configuration (0x00/0x10)
	Set_LUT_12832(0x3F,0x3F,0x3F,0x3F);		// Define All Banks Pulse Width as 64 Clocks
	Set_Contrast_Control_12832(Brightness);	// Set SEG Output Current
	Set_Area_Brightness_12832(Brightness);	// Set Brightness for Area Color Banks
	Set_Precharge_Period_12832(0xD2);		// Set Pre-Charge as 13 Clocks & Discharge as 2 Clock
	Set_VCOMH_12832(0x08);			// Set VCOM Deselect Level
	Set_Entire_Display_12832(0x00);		// Disable Entire Display On (0x00/0x01)
	Set_Inverse_Display_12832(0x00);		// Disable Inverse Display On (0x00/0x01)

	Fill_RAM_12832(0x00);				// Clear Screen

	Set_Display_On_Off_12832(0x01);		// Display On (0x00/0x01)
}
//--------------------------------------------------------------------------
//  Demonstration
//--------------------------------------------------------------------------

int oled_12832(void)
{

OLED_Init_12832();

Set_Start_Page_12832(3); //move cursor to start of page 3
Set_Start_Column_12832(0);
int counter = 0;
for(i=0;i<128;i++){
	oled_Data(Buff[counter]);//send first byte of bitmap
	counter+=4;//point to the next byte of page 3 data
	}
Set_Start_Page_12832(2); //move cursor to start of page 2
Set_Start_Column_12832(0);
int counter = 1; //point to first byte of page 2 in the bitmap data
for(i=0;i<128;i++){
	oled_Data(Buff[counter]);
	counter+=4;
	}
Set_Start_Page_12832(1); //move cursor to start of page 1
Set_Start_Column_12832(0);
counter = 2;
for(i=0;i<128;i++){
	oled_Data(Buff[counter]);
	counter+=4;
	}
Set_Start_Page_12832(0); //move cursor to start of page 0
Set_Start_Column_12832(0);
counter = 3;
for(i=0;i<128;i++){
	oled_Data(Buff[counter]);
	counter+=4;
	}

return 1;
} */

