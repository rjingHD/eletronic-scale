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
/*������ ��ZLG7289_Init()
 *���룺��
 *�������
 *��������ʼ��ZLG7289
 *��ע����
*/

void ZLG7289_Init()
{
    ZLG7289_CS_H;
    ZLG7289_CLK_L;
    ZLG7289_DATA_H;
    ZLG7289_KEY_OUT_MODE;        //����ΪP1^2Ϊ���룬����
    ZLG7289_Write_Cmd(0xA4);    //д�븴λָ��
}
/*������ ��ZLG7289_Write_Cmd(unsigned char cmd)
 *���룺cmd = 0xA0         ����ָ��
        cmd = 0xA1         ����ָ��
        cmd = 0xA2         ѭ������ָ��
        cmd = 0xA3         ѭ������ָ��
        cmd = 0xA4         ��λָ��
        cmd = 0xBF         ����ָ��
 *�������
 *��������ZLG7289д��ָ��
 *��ע����
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
/*������ ��ZLG7289_Write_Cmd(unsigned char cmd)
 *���룺cmd = 0b1000 0a2a1a0       
        dat = 0bDPxxx d3d2d1d0          ���������Ұ���ʽ0����
        cmd = 0b1100 1a2a1a0       
        dat = 0bDPxxx d3d2d1d0          ���������Ұ���ʽ1����
        cmd = 0b1001 0a2a1a0       
        dat = 0bDPABC DEFG              �������ݵ�������
        cmd = 0x88     
        dat = 0bd7d6d5d4 d3d2d1d0       ��˸����
        cmd = 0xE0     
        dat = 0bxxd5d4 d3d2d1d0         �ε���ָ��
        cmd = 0x15     
        dat = 0bd7d6d5d4 d3d2d1d0       ����������ָ������ã�
 *�������
 *��������ZLG7289д������ݵ�ָ��
 *��ע������������ָ��˺����޷����á�
        ������������datΪ������ݣ���ZLG7289��Ƭ�������
*/
void ZLG7289_Write_CmdDat(unsigned char cmd,unsigned char dat)
{
    unsigned char i;
    ZLG7289_CS_L;
    delay_us(50);
    for(i=0;i<8;i++)                            //д��ָ��
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
    for(i=0;i<8;i++)                            //д������
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
/*������ ��ZLG7289_ReadKey(unsigned char cmd)
 *���룺cmd = 0x15         ���������ָ��
 *��������̴����ֵ
 *��������ZLG7289д��0x15,������̴����ֵ
 *��ע����
*/
unsigned char ZLG7289_ReadKey(unsigned char cmd)
{
    unsigned char i;
    unsigned char key;
    ZLG7289_CS_L;
    delay_us(50);
    for(i=0;i<8;i++)                    //�̶�д��0x15���Զ�ȡ���̴���ֵ
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
    for(i=0;i<8;i++)                            //��ȡ���̴���ֵ
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
            key =10;//������
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
            key = 11;//ȷ��
            break;       
      case 24:
            key = 12;//���õ���
            break;
      case 25:
            key = 13;//��ëƤ
            break;
      case 26:
            key = 14;//ȥ��ëƤ
            break;
      case 27:
            key = 15;//��������
            break; 
      default:
            key =80;
            break;
    }
    
            
            
    return key;
}
/*������ ��ZLG7289_Delay_us(unsigned int us)
 *���룺us   �ӳ�ʱ��          us��ΧΪ(0,65535)
 *�������
 *�������ӳ�һ��ʱ��
 *��ע������ȷ�ӳ�
*/
void ZLG7289_Delay_us(unsigned int us)
{
    unsigned int i;
    for(i=0;i<us;i++);
}

