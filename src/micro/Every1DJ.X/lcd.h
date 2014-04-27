/* 
 * File:   lcd.h
 * Author: team2
 *
 * Created on March 24, 2014, 12:34 PM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

int lcdWrite(char *str, char *strb, int lena, int lenb, int displayPage);


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

