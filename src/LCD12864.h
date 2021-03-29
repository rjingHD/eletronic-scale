#ifndef _LCD12864_H_
#define _LCD12864_H_

#include "msp430f5529.h"

#define LCD12864_RS_H   P4DIR |= BIT1;  P4OUT |= BIT1;
#define LCD12864_RS_L   P4DIR |= BIT1;  P4OUT &= ~BIT1;
#define LCD12864_RW_H   P4DIR |= BIT2;  P4OUT |= BIT2;
#define LCD12864_RW_L   P4DIR |= BIT2;  P4OUT &= ~BIT2;
#define LCD12864_E_H    P6DIR |= BIT5;  P6OUT |= BIT5;
#define LCD12864_E_L    P6DIR |= BIT5;  P6OUT &= ~BIT5;

extern void LCD12864_Init();
extern void LCD12864_Write(unsigned char type,unsigned char dat);
extern void LCD12864_Delay_us(unsigned int us);
extern void LCD12864_Series_Write(unsigned char x, unsigned char y, unsigned char *series);
extern void LCD12864_lDigtal_Write(unsigned char x, unsigned char y, signed long int num);
extern void LCD12864_fDigtal_Write(unsigned char x, unsigned char y, double num, unsigned char decimal);
extern void LCD12864_Clear_All();

#endif
