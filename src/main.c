#include "msp430f5529.h"
#include "LCD12864.h"
#include "ZLG7289.h"
#include "ads1115.h"

#define cmda 0x15
#define  N_ 21

float value=0;

long sum=0;
int count=0;
int count1=0;
int count2=0;
long array[N_]={0,0,0,0,0,0,0,0,0,0};
long sum_2=0;
int flag=0;
float temp;
char flag_chaozhong= 0;  //
float value_basic = 0;
float value_delta = 0;
float weight1 = 0;
float voltage_store = 0;

unsigned char key_num = 0; //用来存放按键按下的键值
double weight_total = 0;    //用来存放传感器读取出来的数据
double weight_maopi = 0;   //用来存放毛皮数据
double weight = 0;          //用来存放实物重量
double price = 0;             //用来存放单价 
double price_total = 0;             //用来存放总价 
unsigned char table[6] = {'0','0','.','0','0',0};//用来存放转换成数组变量 
unsigned char state = 0;    //用来存放设置状态

unsigned char i = 0;
unsigned char a;

char weight_flag = 0;
// unsigned int weight_test = 0; //
//void delay_us(unsigned int k);
void Data_deteal(void);
void sort (void);
void Data_Init(void);

void main()
{
    WDTCTL = WDTPW + WDTHOLD;

    LCD12864_Init();
    ZLG7289_Init();
    ads1115_init();                       //1115初始化    
    Data_Init();
    
    P1REN |=BIT2;
    P1OUT |=BIT2;
    P1IES |=BIT2;
    P1IFG &=~BIT2;
    P1IE |=BIT2;
    

    LCD12864_Series_Write(1,0,"****************");
    LCD12864_Series_Write(2,2,"欢迎使用");
    LCD12864_Series_Write(3,0,"500 克精密电子秤");
    LCD12864_Series_Write(4,0,"****************");
    LCD12864_Delay_us(50000);                  //重量显示延迟时间  
    LCD12864_Delay_us(50000);
    LCD12864_Delay_us(50000);
    LCD12864_Delay_us(50000);
    LCD12864_Delay_us(50000);
    LCD12864_Delay_us(50000);
    LCD12864_Delay_us(50000);
    LCD12864_Delay_us(50000);
    LCD12864_Delay_us(50000);                  
    LCD12864_Delay_us(50000);
    

    LCD12864_Clear_All();
    
    Data_Init();
    LCD12864_Series_Write(1,0,"待机");     
    LCD12864_Series_Write(2,0,"重量:");
 //   LCD12864_Series_Write(2,0,"皮重:");
    LCD12864_Series_Write(3,0,"单价:");//////////////////////////////////////////
    LCD12864_Series_Write(4,0,"总价:");
    LCD12864_Series_Write(2,6,"g");
 //   LCD12864_Series_Write(2,6,"g");
    LCD12864_Series_Write(3,6,"元");
    LCD12864_Series_Write(4,6,"元");    
   
    
    while(1)
      {

      Data_deteal(); 
      __bis_SR_register(GIE);
      
      
      }
          
        
}
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
   

    key_num=ZLG7289_ReadKey( cmda );
    
   switch(key_num)
    {
      case 12:
        state=1;LCD12864_Series_Write(1,0,"设置单价  ");break;
        
      case 13: 
        state=2;LCD12864_Series_Write(1,0,"去皮/ 清零");break;
      case 14:  
        state=3;break;
      case 15:  LCD12864_Series_Write(1,0,"价格归零  ");
        state=4;LCD12864_Series_Write(1,0,"称重      ");break;
      default: LCD12864_Series_Write(1,0,"常规      ");
        break;
    }
    switch(state)
    {
      case 0:
        break;
      case 1:
        state=1;LCD12864_Series_Write(1,0,"设置单价  ");                       //
        if(key_num<=11)
        {
           if(key_num<=9)
           {
               if(i<4)
               {  
                  if(i<2)
                  {
                  table[i]=key_num+0x30;
                  i++;
                  }
                  else
                  {
                  table[i+1]=key_num+0x30;
                  i++;
                  if(i==4)
                  i=0;
                  }  
               }
           }
           if(key_num==10)
           {
              table[0]='0';
              table[1]='0';
              table[3]='0';
              table[4]='0';
           }
           if(key_num==11)
           {
           state=0;
           i=0;
           

           }  
        }
        price = 0;
        price = ((table[0]-0x30)*10+(table[1]-0x30)+((double)table[3]-0x30)/10+((double)table[4]-0x30)/100)/1000;
        
        break;
    case 2:

        Data_Init();
     //   price_total = 0;                                                 //
        
        state = 0;
        break;
    case 3:                                             
        price_total = 0; 
        LCD12864_fDigtal_Write(4,3,price_total,2);
        table[0]='0';
        table[1]='0';
        table[3]='0';
        table[4]='0'; 
        state = 0 ;
        break;
    case 4:                                          //称重
         weight_flag = 1;
         if (weight_flag == 1)
        {
              weight = weight1;
             // weight = weight - weight_maopi;
              price_total = price_total + price * weight;
              LCD12864_fDigtal_Write(2,3,weight,2);
              LCD12864_fDigtal_Write(4,3,price_total,2);
              LCD12864_Delay_us(50000);                  //重量显示延迟时间  
              LCD12864_Delay_us(50000);
              LCD12864_Delay_us(50000);
              LCD12864_Delay_us(50000);
              LCD12864_Delay_us(50000);
              LCD12864_Delay_us(50000);
              LCD12864_Delay_us(50000);
              LCD12864_Delay_us(50000);
              weight_flag = 0;
              
        }
        state = 0 ;
        break;
    default:
        break;
    }

   if (weight_flag == 1)                                                           //
      {                                                                            //
      weight = weight1;                                                            //
      price_total = price_total + price * weight;                                  //
      LCD12864_fDigtal_Write(1,3,weight,2);                                        //
      LCD12864_fDigtal_Write(4,3,price_total,2);                                   //
      LCD12864_Delay_us(50000);                  //重量显示延迟时间                //
      LCD12864_Delay_us(50000);                                                    //
      LCD12864_Delay_us(50000);                                                    //
      LCD12864_Delay_us(50000);                                                    //
      LCD12864_Delay_us(50000);                                                    //
      LCD12864_Delay_us(50000);                                                    //
      LCD12864_Delay_us(50000);                                                    //
      LCD12864_Delay_us(50000);                                                    //
      weight_flag = 0;                                                             //
      
      }
   
 //   LCD12864_lDigtal_Write(2,7,state);

    LCD12864_Series_Write(3,3,table);

    P1IFG &=~BIT2;
}

void Data_deteal(void)
{   
    if(flag==0)
    {   for(count=0;count<N_;count++)
        {
          array[count]=go_1115(channel_3);
        }
    }
    else
    {

      for(count=0;count<N_;count++)
      {
          array[count] = go_1115(channel_3);
      }
    }
   
    if(count==N_)
    {   count=0;
        flag=1;
        sort(); 
        value =(array[12]+array[13]+array[14]+array[8]+array[9]+array[10]+array[11])/7;
        _NOP();

        value = (float)value/0x7fff*4.0960;  //将AD值转化为数组
 
        value_delta = value - value_basic;

        
        
        weight1 = value_delta * 318.524;                                                     //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 //       LCD12864_fDigtal_Write(2, 3, weight1,2 );  
//        if ((7.0> weight1)&&(weight1> 5.0))                     //6.0       5-7
//        {
//        weight1 = (weight1- 6.0)/2 + 6.0;
//        }
        
//        if ((43.0> weight1)&&(weight1> 40.0))                   //41.5     40-43
//        {
//        weight1 = (weight1- 41.5)/3 + 41.5;
//        }
        
        if ((55.0> weight1)&&(weight1> 45.0))                   //50.0     45-55

        {
        weight1 = (weight1- 50.0)/7 + 50.0;
        }
        
//        if ((62.0> weight1)&&(weight1> 58.0))                    //60.0    58-62
//        {
//        weight1 = (weight1- 60.0)/3 + 60.0;
//        }
        
        if ((104.0> weight1)&&(weight1> 100.0))                    //102.0    100-104
        {
        weight1 = (weight1- 102.0)/3 + 102.0;
        }
        
        if ((155.0> weight1)&&(weight1> 145.0))                   //150.0  145-155
        {
        weight1 = (weight1 - 150.0)/7 + 150.0;
        }
       
        if ((207.0> weight1)&&(weight1> 193.0))                   //200.0   193-207
        {
         weight1 = (weight1 - 200.0)/10 + 200.0;
        }
        
        if ((460.0> weight1)&&(weight1> 440.0))                   //450.0   440-460
        {
         weight1 = (weight1 - 450.0)/15 + 450.0; 
        }
         LCD12864_fDigtal_Write(2,3, weight1,2 );
        
        
        
        
 //       LCD12864_fDigtal_Write(2, 3, weight1,2 );

        if (weight1>500)
        {
          LCD12864_Series_Write(1,6,"超重");
          flag_chaozhong= 1 ;
        }
        else if (flag_chaozhong)
        {
          LCD12864_Series_Write(1,6,"    ");
          flag_chaozhong= 0; 
        }
          
    }
  

         
      
}


void sort (void)
{
    int i,j;
    int flag=1;
    long temp;
    while(flag)
    {  
        flag=0;
        for(i=0;i<N_-1;i++)
        {
            for(j=i+1;j<N_;j++)
            {   
                if(array[j]<array[j-1])
                {
                    flag=1;
                    temp = array[j];
                    array[j] = array[j-1];
                    array[j-1] = temp;
                }
                  
            }
        }
    }
}

void Data_Init(void)
{   if(flag==0)
    {   for(count=0;count<N_;count++)
        {
          array[count]=go_1115(channel_3);
        }
    }
    else
    {

      for(count=0;count<N_;count++)
      {
          array[count] = go_1115(channel_3);
      }
    }
   
    if(count==N_)
    {   count=0;
        flag=1;
        sort(); 
        value_basic =(array[12]+array[13]+array[14]+array[8]+array[9]+array[10]+array[11])/7;
        _NOP();
        value_basic = (float)value_basic/0x7fff*4.0960;  //将AD值转化为数组

    }
}