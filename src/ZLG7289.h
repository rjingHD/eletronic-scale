#ifndef _ZLG7289_H_
#define _ZLG7289_H_

#include "msp430f5529.h"

#define CPU_F ((double)1000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define ZLG7289_CS_H            P3DIR |= BIT7;P3OUT |= BIT7;
#define ZLG7289_CS_L            P3DIR |= BIT7;P3OUT &= ~BIT7;
#define ZLG7289_CLK_H           P4DIR |= BIT0;P4OUT |= BIT0;
#define ZLG7289_CLK_L           P4DIR |= BIT0;P4OUT &= ~BIT0;
#define ZLG7289_DATA_H          P4DIR |= BIT3;P4OUT |= BIT3;
#define ZLG7289_DATA_L          P4DIR |= BIT3;P4OUT &= ~BIT3;
#define ZLG7289_KEY_OUT         (P1IN & BIT2)

#define ZLG7289_KEY_OUT_MODE    P1DIR &= ~BIT2;P1REN |= BIT2;           //设置P1^2为输入，上拉。以作为判断用
#define ZLG7289_DATA_OUT_MODE   P4DIR &= ~BIT3;P4REN |= BIT3;
#define ZLG7289_DATA_IN_MODE    P4DIR |= BIT3;
#define ZLG7289_DATA_OUT        (P4IN & BIT3)

extern void ZLG7289_Init();
extern void ZLG7289_Write_Cmd(unsigned char cmd);
extern void ZLG7289_Write_CmdDat(unsigned char cmd,unsigned char dat);
extern unsigned char ZLG7289_ReadKey(unsigned char cmd);
extern void ZLG7289_Delay_us(unsigned int us);

#endif