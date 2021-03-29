/*
 * ads1115.c
 *已更正
 *  Created on: 2014-8-31
 *      Author: Administrator
 */
#include "ads1115.h"
void delay(void)
{
	uchar i;
 for(i = 0;i < 15;i++)
   _NOP();
}
/*******************************************
函数名称start
功    能完成IIC的起始条件操作
参    数无
返回值  无
********************************************/
void start(void)
{
 SCL_H;
 SDA_H;
 delay();
 SDA_L;
 delay();
 SCL_L;
}
/*******************************************
函数名称stop
功    能完成IIC的终止条件操作
参    数无
返回值  无
********************************************/
void stop(void)
{
 SDA_L;
 delay();
 SCL_H;;
 delay();
}

/*******************************************
函数名称mack
功    能完成IIC的主机应答操作
参    数无
返回值  无
********************************************/

void mack(void)
{
 SDA_L;
 _NOP(); _NOP();
 SCL_H;
 delay();
 SCL_L;
 _NOP();_NOP();
 SDA_H;
 delay();
}
/*******************************************
函数名称mnack
功    能完成IIC的主机无应答操作
参    数无
返回值  无
********************************************/
void mnack(void)
{
 SDA_H;
 _NOP(); _NOP();
 SCL_H;
 delay();
 SCL_L;
 _NOP(); _NOP();
 SDA_L;
 delay();
}
/**********检查应答信号函数******************/
/*如果返回值为1则证明有应答信号反之没有*/ /*******************************************
函数名称check
功    能检查从机的应答操作
参    数无
返回值  从机是否有应答1--有0--无
********************************************/
void ack()
{
  uchar i;
  i=0;
  SDA_in;
  SCL_H;
  delay();
  while((P2IN&BIT2)&&(i<250)) i++;
  SDA_out;
  SCL_L;
  delay();
}

/*******************************************
函数名称write1
功    能向IIC总线发送一个1
参    数无
返回值  无
********************************************/
void write1(void)
{
 SDA_H;
 delay();
 SCL_H;
 delay();
 SCL_L;
 delay();
}

/*******************************************
函数名称write0
功    能向IIC总线发送一个0
参    数无
返回值  无
********************************************/
void write0(void)
{
 SDA_L;
 delay();
 SCL_H;  delay();
 SCL_L;
 delay();
}

/*******************************************
函数名称write1byte
功    能向IIC总线发送一个字节的数据
参    数wdata--发送的数据
返回值  无
********************************************/
void write1byte(uchar wdata)
{
 uchar i;
 for(i = 8;i > 0;i--)
 {
  if(wdata & 0x80) write1();
  else        write0();
  wdata <<= 1;
 }
 SDA_H;
 _NOP();
}

/*******************************************
函数名称read1byte
功    能从IIC总线读取一个字节
参    数无
返回值  读取的数据
********************************************/
uchar read1byte(void)
{
 uchar  rdata = 0x00,i;
    uchar flag;
	 for(i = 0;i < 8;i++)
	 {
	  SDA_H;
	  SCL_H;
	  SDA_in;
	  flag = SDA_val;
	  rdata <<= 1;
	  if(flag)
		  rdata |= 0x01;
	   SDA_out;
		SCL_L;
	 }
return rdata;
}
void Confige1115(uchar channel)
{
  uchar a_channel;
  switch (channel)
  {
  case 0:a_channel=0xC2;break;
   case 1:a_channel=0xD2;break;
    case 2:a_channel=0xE2;break;
     case 3:a_channel=0xF2;break;
  default: break;
  }
  start();
  write1byte(address);
  ack();
   write1byte(config);
  ack();
   write1byte(a_channel);
  ack();
   write1byte(Lsiwei);
  ack();
  stop();
}

uint read1115()

{
  uchar result_l,result_h;
  uint result;
  start();
   write1byte(address);
  ack();
   write1byte(conversion);
  ack();
  stop();
  delay_ms(5);
  start();
  write1byte(address+1);
   ack();
  result_h=read1byte();
   mack();
  result_l=read1byte();
  ack();
  stop();
  result=result_h*256+result_l;
  return result;
}
uint  go_1115(uchar temp)
{
  uint result_gd;
  Confige1115(temp);
  delay_ms(5);
  result_gd=read1115();
  delay_ms(5);
  return result_gd;
}
void ads1115_init(void)
{
	  P2DIR|=BIT4;
	  P3DIR|=BIT5;
	  SCL_H;
	  SDA_H;
	  SDA_out;
}

