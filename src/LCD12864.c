/***************************LCD12864.c*********************************
**
**
**
**
**
**
**
**********************************************************************/
#include "LCD12864.h"
/*函数名 ：LCD12864_Init()
 *输入：无
 *输出：无
 *描述：初始化LCD12864
 *备注：无
*/
void LCD12864_Init()
{
    LCD12864_Write(0,0x30);
    LCD12864_Write(0,0x01);
    LCD12864_Delay_us(2000);
    LCD12864_Write(0,0x06);
    LCD12864_Write(0,0x0C);
}
/*函数名 ：LCD12864_Write(unsigned char type,unsigned char dat)
 *输入：type = 0   写指令
        type = 1   写数据
 *输出：无
 *描述：向LCD12864写入指令/数据
 *备注：无
*/
void LCD12864_Write(unsigned char type,unsigned char dat)
{
    unsigned char cmd,dat1,dat2;
    unsigned char i = 0;
    LCD12864_RS_H;
    LCD12864_E_L;
    cmd = (0xF8 | (type << 1));                         //分三次写入，分别为起始字节，高四位，低四位
    for(i=0;i<8;i++)
    {
        if(cmd & 0x80)  
        {
          LCD12864_RW_H;
        }        
        else
        {
            LCD12864_RW_L;
        }
        LCD12864_E_H;
        cmd <<= 1;
        LCD12864_E_L;
    }
    dat1 = dat & 0xF0;
    for(i=0;i<8;i++)
    {
        if(dat1 & 0x80)  
        {
            LCD12864_RW_H;
        }
        else            
        {
          LCD12864_RW_L;
        }
         LCD12864_E_H;
        dat1 <<= 1;
        LCD12864_E_L;
    }
    dat2 = (dat<<4) & 0xF0;
    for(i=0;i<8;i++)
    {
        if(dat2 & 0x80)  
        { 
            LCD12864_RW_H;
        }
        else  
        {
            LCD12864_RW_L;
        }
        LCD12864_E_H;
        dat2 <<= 1;
        LCD12864_E_L;
    }
    LCD12864_RS_L;
    LCD12864_E_L;   
    LCD12864_Delay_us(72);
}
/*函数名 ：LCD12864_Delay_us(unsigned int us)
 *输入：us   延迟时间          us范围为(0,65535)
 *输出：无
 *描述：延迟一定时间
 *备注：不精确延迟
*/
void LCD12864_Delay_us(unsigned int us)
{
    unsigned int i;
    for(i=0;i<us;i++);
}
/*函数名 ：LCD1284_Series_Write(unsigned char position, unsigned char *series)
 *输入：x   光标起始行位置
        y   光标起始列位置
        *series    输入字符串      
 *输出：无
 *描述：写入字符串，也可以写入汉字
 *备注：无
*/
void LCD12864_Series_Write(unsigned char x, unsigned char y, unsigned char *series)
{
	unsigned char position;
        if(x == 1)
        	x = 0x80;
	else if(x == 2)
	        x = 0x90;
	else if(x == 3)
	        x = 0x88;
	else if(x == 4)
		    x = 0x98;
	position = x + y;
	LCD12864_Write(0,position);                      //设定光标位置
    for(;*series != 0;series++)
       LCD12864_Write(1,*series);
}
/*函数名 ：LCD12864_lDigtal_Write(unsigned char position, signed long int num)
 *输入：x   光标起始行位置
        y   光标起始列位置
        num        要显示的数字 num范围为（-2147483648，2147483647）      
 *输出：无
 *描述：显示signed long int型数字
 *备注：无
*/
void LCD12864_lDigtal_Write(unsigned char x, unsigned char y, signed long int num)
{
  
    unsigned char i = 0,j = 0;
    unsigned char buf[11],str[11];  
    unsigned char position;
    if(num < 0)
    {
        str[j++] = '-';
        num = -num;
    }
    do{
        buf[i++] = num % 10;
        num /= 10;
    }while(num != 0);
    for(;i > 0;)
      str[j++] = buf[--i] + '0';
    str[j++] = '\0';
        
        if(x == 1)
        	x = 0x80;
	else if(x == 2)
	        x = 0x90;
	else if(x == 3)
	        x = 0x88;
	else if(x == 4)
		    x = 0x98;
	position = x + y;
	LCD12864_Write(0,position);                     //设定光标位置
    for(j=0;str[j] != 0;j++)
       LCD12864_Write(1,str[j]);
}
/*函数名 ：LCD12864_fDigtal_Write(unsigned char position, double num, unsigned char decimal)
 *输入：x   光标起始行位置
        y   光标起始列位置
        num        要显示的数字 
        decimal    想要保留的小数位数    （num*10^decimal）的范围为（-2147483648，2147483647）
 *输出：无
 *描述：显示double型数字
 *备注：无
*/
void LCD12864_fDigtal_Write(unsigned char x, unsigned char y, double num, unsigned char decimal)
{
  
    unsigned char i = 0,j = 0;
    unsigned char buf[11],str[11];  
    signed long int integer = 0;
    integer = (signed long int)num;
    i = decimal + 1; 

    if(num < 0)
    {
        str[j++] = '-';
        num = -num;
    }
    buf[decimal] = '.';
    do{
      num *= 10;
      buf[--decimal] = (unsigned long int)num % 10 + '0';
    }while(decimal != 0);
    
    
    do{
        buf[i++] = integer % 10 + '0';
        integer /= 10;
    }while(integer != 0);
    
    for(;i > 0;)
      str[j++] = buf[--i];
    str[j] = '\0';
    
    LCD12864_Series_Write(x,y,str);
}

/*函数名 ：LCD12864_Clear_All()
 *输入：无     
 *输出：无
 *描述：清除整个屏幕
 *备注：无
*/
void LCD12864_Clear_All()
{
    LCD12864_Write(0,0x30);
    LCD12864_Write(0,0x01);
    LCD12864_Delay_us(2000);
}

