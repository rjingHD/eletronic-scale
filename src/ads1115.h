/*
 * ads1115.h
 *
 *  Created on: 2014-8-31
 *      Author: Administrator
 */

#ifndef ADS1115_H_
#define ADS1115_H_
#include "msp430.h"
#define uchar unsigned char
#define uint unsigned int
#define SCL_H P2OUT |= BIT4
#define SCL_L P2OUT &= ~BIT4
#define SDA_H P3OUT |= BIT5
#define SDA_L P3OUT &= ~BIT5
#define SDA_in  P3DIR &= ~BIT5  //SDA�ĳ�����ģʽ
#define SDA_out P3DIR |= BIT5  //SDA������ģʽ
#define SDA_val P3IN&BIT5//SDA��λֵ
#define address 0x90//��ַ
#define config  0x01//ѡ��Ĵ���
#define conversion 0x00
#define Lsiwei  0xE3//�����ٶȣ��Ƚ���ģʽ������
#define  channel_0      0                //��ʾѡ��ͨ��0,���ٶ�
#define  channel_1      1                //��ʾѡ��ͨ��1
#define  channel_2      2                //��ʾѡ��ͨ��2
#define  channel_3      3                //��ʾѡ��ͨ��2
#define TRUE    1
#define FALSE   0
#define CPU_F ((double)8000000)
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))  //��ʱmS
void delay(void) ;
void start(void) ;
void stop(void) ;
void mack(void) ;
void mnack(void) ;
void ack() ;
void write1(void) ;
void write0(void) ;
void write1byte(uchar wdata) ;
uchar read1byte(void) ;
void Confige1115(uchar channel) ;
uint read1115();
uint  go_1115(uchar temp);
void ads1115_init(void);
#endif /* ADS1115_H_ */
