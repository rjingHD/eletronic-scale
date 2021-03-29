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
/*������ ��LCD12864_Init()
 *���룺��
 *�������
 *��������ʼ��LCD12864
 *��ע����
*/
void LCD12864_Init()
{
    LCD12864_Write(0,0x30);
    LCD12864_Write(0,0x01);
    LCD12864_Delay_us(2000);
    LCD12864_Write(0,0x06);
    LCD12864_Write(0,0x0C);
}
/*������ ��LCD12864_Write(unsigned char type,unsigned char dat)
 *���룺type = 0   дָ��
        type = 1   д����
 *�������
 *��������LCD12864д��ָ��/����
 *��ע����
*/
void LCD12864_Write(unsigned char type,unsigned char dat)
{
    unsigned char cmd,dat1,dat2;
    unsigned char i = 0;
    LCD12864_RS_H;
    LCD12864_E_L;
    cmd = (0xF8 | (type << 1));                         //������д�룬�ֱ�Ϊ��ʼ�ֽڣ�����λ������λ
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
/*������ ��LCD12864_Delay_us(unsigned int us)
 *���룺us   �ӳ�ʱ��          us��ΧΪ(0,65535)
 *�������
 *�������ӳ�һ��ʱ��
 *��ע������ȷ�ӳ�
*/
void LCD12864_Delay_us(unsigned int us)
{
    unsigned int i;
    for(i=0;i<us;i++);
}
/*������ ��LCD1284_Series_Write(unsigned char position, unsigned char *series)
 *���룺x   �����ʼ��λ��
        y   �����ʼ��λ��
        *series    �����ַ���      
 *�������
 *������д���ַ�����Ҳ����д�뺺��
 *��ע����
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
	LCD12864_Write(0,position);                      //�趨���λ��
    for(;*series != 0;series++)
       LCD12864_Write(1,*series);
}
/*������ ��LCD12864_lDigtal_Write(unsigned char position, signed long int num)
 *���룺x   �����ʼ��λ��
        y   �����ʼ��λ��
        num        Ҫ��ʾ������ num��ΧΪ��-2147483648��2147483647��      
 *�������
 *��������ʾsigned long int������
 *��ע����
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
	LCD12864_Write(0,position);                     //�趨���λ��
    for(j=0;str[j] != 0;j++)
       LCD12864_Write(1,str[j]);
}
/*������ ��LCD12864_fDigtal_Write(unsigned char position, double num, unsigned char decimal)
 *���룺x   �����ʼ��λ��
        y   �����ʼ��λ��
        num        Ҫ��ʾ������ 
        decimal    ��Ҫ������С��λ��    ��num*10^decimal���ķ�ΧΪ��-2147483648��2147483647��
 *�������
 *��������ʾdouble������
 *��ע����
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

/*������ ��LCD12864_Clear_All()
 *���룺��     
 *�������
 *���������������Ļ
 *��ע����
*/
void LCD12864_Clear_All()
{
    LCD12864_Write(0,0x30);
    LCD12864_Write(0,0x01);
    LCD12864_Delay_us(2000);
}

