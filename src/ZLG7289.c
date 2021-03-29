/***************************ZLG7289.c*********************************
**
**
**
**
**
**
**
**********************************************************************/

#include "ZLG7289.h"
//#include "system.h"
/*函数名 ：ZLG7289_Init()
 *输入：无
 *输出：无
 *描述：初始化ZLG7289
 *备注：无
*/

void ZLG7289_Init()
{
    ZLG7289_CS_H;
    ZLG7289_CLK_L;
    ZLG7289_DATA_H;
    ZLG7289_KEY_OUT_MODE;        //设置为P1^2为输入，上拉
    ZLG7289_Write_Cmd(0xA4);    //写入复位指令
}
/*函数名 ：ZLG7289_Write_Cmd(unsigned char cmd)
 *输入：cmd = 0xA0         右移指令
        cmd = 0xA1         左移指令
        cmd = 0xA2         循环右移指令
        cmd = 0xA3         循环左移指令
        cmd = 0xA4         复位指令
        cmd = 0xBF         测试指令
 *输出：无
 *描述：向ZLG7289写入指令
 *备注：无
*/
void ZLG7289_Write_Cmd(unsigned char cmd)
{
    unsigned char i;
    ZLG7289_CS_L;
    for(i=0;i<8;i++)
    {
        if(cmd & 0x80)
        {
            ZLG7289_DATA_H;
        }
        else
        {
            ZLG7289_DATA_L;
        }
        cmd <<= 1;
        ZLG7289_CLK_H;
        delay_us(8);
        ZLG7289_CLK_L;
        delay_us(8);
    }
    ZLG7289_CS_H;
}
/*函数名 ：ZLG7289_Write_Cmd(unsigned char cmd)
 *输入：cmd = 0b1000 0a2a1a0       
        dat = 0bDPxxx d3d2d1d0          下载数据且按方式0编码
        cmd = 0b1100 1a2a1a0       
        dat = 0bDPxxx d3d2d1d0          下载数据且按方式1编码
        cmd = 0b1001 0a2a1a0       
        dat = 0bDPABC DEFG              下载数据但不译码
        cmd = 0x88     
        dat = 0bd7d6d5d4 d3d2d1d0       闪烁控制
        cmd = 0xE0     
        dat = 0bxxd5d4 d3d2d1d0         段点亮指令
        cmd = 0x15     
        dat = 0bd7d6d5d4 d3d2d1d0       读键盘数据指令（不掉用）
 *输出：无
 *描述：向ZLG7289写入带数据的指令
 *备注：读键盘数据指令此函数无法调用。
        读键盘数据中dat为输出数据，即ZLG7289向单片机输出。
*/
void ZLG7289_Write_CmdDat(unsigned char cmd,unsigned char dat)
{
    unsigned char i;
    ZLG7289_CS_L;
    delay_us(50);
    for(i=0;i<8;i++)                            //写入指令
    {
        if(cmd & 0x80)
        {
            ZLG7289_DATA_H;
        }
        else
        {
            ZLG7289_DATA_L;
        }
        cmd <<= 1;
        ZLG7289_CLK_H;
        delay_us(8);
        ZLG7289_CLK_L;
        delay_us(8);
    }
    delay_us(25);  
    for(i=0;i<8;i++)                            //写入数据
    {
        if(dat & 0x80)
        {
            ZLG7289_DATA_H;
        }
        else
        {
            ZLG7289_DATA_L;
        }
        dat <<= 1;
        ZLG7289_CLK_H;
        delay_us(8);
        ZLG7289_CLK_L;
        delay_us(8);
    }   
    ZLG7289_CS_H;
}
/*函数名 ：ZLG7289_ReadKey(unsigned char cmd)
 *输入：cmd = 0x15         读键盘输出指令
 *输出：键盘代码的值
 *描述：向ZLG7289写入0x15,输出键盘代码的值
 *备注：无
*/
unsigned char ZLG7289_ReadKey(unsigned char cmd)
{
    unsigned char i;
    unsigned char key;
    ZLG7289_CS_L;
    delay_us(50);
    for(i=0;i<8;i++)                    //固定写入0x15，以读取键盘代码值
    {
        if(cmd & 0x80)
        {
            ZLG7289_DATA_H;
        }
        else
        {
            ZLG7289_DATA_L;
        }
        cmd <<= 1;
        ZLG7289_CLK_H;
        delay_us(8);
        ZLG7289_CLK_L;
        delay_us(8);
    }
    delay_us(25); 
    ZLG7289_DATA_OUT_MODE;
    ZLG7289_CLK_H;
    delay_us(8);
    for(i=0;i<8;i++)                            //读取键盘代码值
    {
        key <<= 1;
        if(ZLG7289_DATA_OUT)
        {
            key |= 0x01;
        }
        else
        {
            key &= ~0x01;
        }
        ZLG7289_CLK_L;
        delay_us(8);
        ZLG7289_CLK_H;
        delay_us(8);
    }  
    ZLG7289_DATA_IN_MODE;
    ZLG7289_CLK_L;
    ZLG7289_CS_H;   
    switch (key)
    {  
      case 0:
            key =1;
            break;
      case 1:
            key = 4;
            break;
      case 2:
            key =7;
            break;
      case 3:
            key =10;//撤销键
            break;
      case 8:
            key = 2;
            break;
      case 9:
            key = 5;
            break;
      case 10:
            key = 8;
            break;
      case 11:
            key = 0;
            break;
      case 16:
            key = 3;
            break; 
      case 17:
            key = 6;
            break;
      case 18:
            key = 9;
            break;      
      case 19:
            key = 11;//确定
            break;       
      case 24:
            key = 12;//设置单价
            break;
      case 25:
            key = 13;//称毛皮
            break;
      case 26:
            key = 14;//去掉毛皮
            break;
      case 27:
            key = 15;//重置清零
            break; 
      default:
            key =80;
            break;
    }
    
            
            
    return key;
}
/*函数名 ：ZLG7289_Delay_us(unsigned int us)
 *输入：us   延迟时间          us范围为(0,65535)
 *输出：无
 *描述：延迟一定时间
 *备注：不精确延迟
*/
void ZLG7289_Delay_us(unsigned int us)
{
    unsigned int i;
    for(i=0;i<us;i++);
}

