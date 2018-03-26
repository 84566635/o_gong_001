#define TM1628_H_LOCAL
#include "TM1628.h"
//#include "define.h"
//#include "DISPLAY.h"

extern unsigned char Key[5];//存储按键值
unsigned char Band_Switch_Key[5];//存储按键值
unsigned char TM1628_buf[18]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char TM1628_buf2[18]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x00,0x00,0x00,0x00,0x00,0x00};

extern void delay_us(int usneed);


extern struct SYS_EVEN Sys_Even ;

#define Delay_uS(a)  delay_us(a)
                                  // 0   1    2    3   4    5    6    7     8   9    A     B    C   D     E   F    R   
const unsigned char TAB_duan[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0xf9,0x3b,0x79,0x79,0x73,0x31,0xb9,0x39,0x23,0x19,0xe9,0x1b,0xf1,0x11,0x33,0xab,0x10};
 struct DIGI_SHOW SysDigiShow = {8,8,8,8,88,88,0};

 /***************************************************************/
/* 显示程序 5位，可以加到7位       */
/*前4个是数码管，第五位管工作指示灯*/
/***************************************************************/
#
void TM1628_display()
{
    STB_H;        //片选，置高无效
    TM1628_buf[0]&=0x80;
    TM1628_buf[2]&=0x80;
    TM1628_buf[6]&=0x80;
    TM1628_buf[12]&=0x80;
    TM1628_buf[0]  = TAB_duan[SysDigiShow.fist];
    TM1628_buf[2]  = TAB_duan[SysDigiShow.second];
    TM1628_buf[6]  = TAB_duan[SysDigiShow.third];
    TM1628_buf[12] = TAB_duan[SysDigiShow.fourth];
    Command(0x03);//显示模式
    Command(0x40);//地址自动加一

    STB_L;           //片选，0有效

    Send_8bit(0xc0);//地址0开始
    Send_8bit(TM1628_buf[0]);//1
    Send_8bit(TM1628_buf[1]);
    Send_8bit(TM1628_buf[2]);//2
    Send_8bit(TM1628_buf[3]);
    Send_8bit(TM1628_buf[4]);//:
    Send_8bit(TM1628_buf[5]);
    Send_8bit(TM1628_buf[6]);//3
    STB_H;        //片选，置高无效
    STB_L;           //片选，0有效
    Send_8bit(0xCC);
    Send_8bit(TM1628_buf[12]);//4
    Send_8bit(TM1628_buf[13]);


   // Send_8bit(dat5);  //0x01-led1;0x02-led2;0x04-led3;0x08-led4
    //Send_8bit(0xff);  //led工作灯总开关


    STB_H;        //片选，置高无效
   // Command(0x88);  //显示开
}
  



void Flash_LED_DP_F(int status)
{
    Command(0x03);//显示模式
    Command(0x40);//地址自动加一
    if(status==0)
    {
      TM1628_buf[4]&=0xfc;
    }
    else
    {
      TM1628_buf[4]|=0x03;
    }
    STB_L;           //片选，0有效

    Send_8bit(0xc4);//地址0开始
    Send_8bit(TM1628_buf[4]);//:
    STB_H;           //片选，0有效
    //Send_8bit(TM1628_buf[4]);//:
}
   

//void Change_Digishow(int mini,int second)
//{
//  //SysDigiShow.fist    =  fisrt;
//  //SysDigiShow.second =  second;
//  //SysDigiShow.third   =  third;
//  //SysDigiShow.fourth  =  fourth;
//  
//  SysDigiShow.time_mini   = mini;
//  SysDigiShow.time_second = second;
//  SysDigiShow.fist    =  SysDigiShow.time_mini/10;
//  SysDigiShow.second  =  SysDigiShow.time_mini%10;
//  SysDigiShow.third   =  SysDigiShow.time_second/10;
//  SysDigiShow.fourth  =  SysDigiShow.time_second%10;
//  
//  Sys_Even.Digi_DisPlay_Change = 1;
//}
//向TM1628发送8位数据，从低位开始
void Send_8bit(unsigned char data)
{
 unsigned char i;
 for(i=0;i<8;i++)
     {	
        CLK_L;
        Delay_uS(10);
	   if(data&0x01)
	   {	
           DIO_H;
	   }
	   else
	   {
	    DIO_L;
	   }; 
	 Delay_uS(10);
	 CLK_H;
         Delay_uS(10);
         data>>=1;	
     };

}





//向TM1628发送命令
void Command(unsigned char com)
{
	STB_L;
	Send_8bit(com);
	if(com!=0x42)
	STB_H;
	Delay_uS(10);
}



//点亮add地址data的灯
void Light_ON(unsigned char add,unsigned char Data)
{
        STB_O;
        CLK_O;
        DIO_O;
	TM1628_buf[add]|=Data;   

        //以下条件编译默认 HT1628_DisMode == ADDR_FIXED
        //如定义ADDR_AUTOUP，每次执行本函数，1628会全部重刷buffer
        //如定义ADDR_FIXED，每次执行本函数，1628只会重刷一次add地址一次
        
        #ifndef HT1628_DisMode
        #define HT1628_DisMode ADDR_FIXED
        #endif
        
        #if HT1628_DisMode == ADDR_AUTOUP
        Command(DISSEGCM710);    //设置显示模式  7位10段模式
	Command(ADDR_AUTOUP);    //数据命令  地址自动加1
	STB_L;
	Send_8bit(DIS_ADDR00);
	Delay_uS(10);
	for(i=0;i<14;i++)
	{
	 Send_8bit(TM1628_buf[i]);
	}
        STB_H;
        #elif HT1628_DisMode == ADDR_FIXED
        Command(DISSEGCM710);    //设置显示模式  7位10段模式  
        Command(ADDR_FIXED);     //数据命令  固定地址
	STB_L;
	Send_8bit(0xC0|add);     //HT1628地址B7 B6固定为1
	Delay_uS(10);
	Send_8bit(TM1628_buf[add]);
        STB_H;
        #endif
	Delay_uS(10);
        Command(DIS_RIGH114|DIS_ON);//设置亮度	
}



//熄灭add地址data的灯
void Light_OFF(unsigned char add,unsigned char Data)
{
        STB_O;
        CLK_O;
        DIO_O;
	TM1628_buf[add]&=~Data;   

        //以下条件编译默认 HT1628_DisMode == ADDR_FIXED
        //如定义ADDR_AUTOUP，每次执行本函数，1628会全部重刷buffer
        //如定义ADDR_FIXED，每次执行本函数，1628只会重刷一次add地址一次
        
        #ifndef HT1628_DisMode
        #define HT1628_DisMode ADDR_FIXED
        #endif
        
        #if HT1628_DisMode == ADDR_AUTOUP
        Command(DISSEGCM710);    //设置显示模式  7位10段模式
	Command(ADDR_AUTOUP);    //数据命令  地址自动加1
	STB_L;
	Send_8bit(DIS_ADDR00);
	Delay_uS(10);
	for(i=0;i<14;i++)
	{
	 Send_8bit(TM1628_buf[i]);
	}
        STB_H;
        #elif HT1628_DisMode == ADDR_FIXED
        Command(DISSEGCM710);    //设置显示模式  7位10段模式  
        Command(ADDR_FIXED);     //数据命令  固定地址
	STB_L;
	Send_8bit(0xC0|add);     //HT1628地址B7 B6固定为1
	Delay_uS(10);
	Send_8bit(TM1628_buf[add]);
        STB_H;
        #endif
        
	Delay_uS(5);
	Command(DIS_RIGH114|DIS_ON);//亮度//显示控制    脉冲宽度10/16
	
}


//
void Init_TM1628(unsigned char LED_Status)
{	unsigned char i;
        STB_O;
        CLK_O;
        DIO_O;

	Command(DISSEGCM710); //设置显示模式 0x00 7位10段模式
	Command(ADDR_AUTOUP); //数据命令：地址自动加1
        STB_L;
	Send_8bit(DIS_ADDR00);//地址命令	
	Delay_uS(20);
	for(i=0;i<14;i++)
	{ 
          Send_8bit(LED_Status);
          TM1628_buf[i]=LED_Status;
	};
	STB_H;
	Delay_uS(20);
	Command(DIS_RIGH114|DIS_ON);//亮度//显示控制
};


//向TM1628发送命令
void Command2(unsigned char com)
{
	STB2_L;
	Send_8bit(com);
	if(com!=0x42)
	STB2_H;
	Delay_uS(10);
}

//点亮add地址data的灯
void Light2_ON(unsigned char add,unsigned char Data)
{
        STB2_O;
        CLK_O;
        DIO_O;
	TM1628_buf2[add]|=Data;   

        //以下条件编译默认 HT1628_DisMode == ADDR_FIXED
        //如定义ADDR_AUTOUP，每次执行本函数，1628会全部重刷buffer
        //如定义ADDR_FIXED，每次执行本函数，1628只会重刷一次add地址一次
        
        #ifndef HT1628_DisMode
        #define HT1628_DisMode ADDR_FIXED
        #endif
        
        #if HT1628_DisMode == ADDR_AUTOUP
        Command2(DISSEGCM710);    //设置显示模式  7位10段模式
	Command2(ADDR_AUTOUP);    //数据命令  地址自动加1
	STB2_L;
	Send_8bit(DIS_ADDR00);
	Delay_uS(10);
	for(i=0;i<14;i++)
	{
	 Send_8bit(TM1628_buf2[i]);
	}
        STB2_H;
        #elif HT1628_DisMode == ADDR_FIXED
        Command2(DISSEGCM512);    //设置显示模式  7位10段模式  
        Command2(ADDR_FIXED);     //数据命令  固定地址
	STB2_L;
	Send_8bit(0xC0|add);     //HT1628地址B7 B6固定为1
	Delay_uS(10);
	Send_8bit(TM1628_buf2[add]);
        STB2_H;
        #endif
	Delay_uS(10);
        Command2(DIS_RIGH114|DIS_ON);//设置亮度	
}


void Light2_OFF(unsigned char add,unsigned char Data)
{
        STB2_O;
        CLK_O;
        DIO_O;
	TM1628_buf2[add]&=~Data;   

        //以下条件编译默认 HT1628_DisMode == ADDR_FIXED
        //如定义ADDR_AUTOUP，每次执行本函数，1628会全部重刷buffer
        //如定义ADDR_FIXED，每次执行本函数，1628只会重刷一次add地址一次
        
        #ifndef HT1628_DisMode
        #define HT1628_DisMode ADDR_FIXED
        #endif
        
        #if HT1628_DisMode == ADDR_AUTOUP
        Command2(DISSEGCM710);    //设置显示模式  7位10段模式
	Command2(ADDR_AUTOUP);    //数据命令  地址自动加1
	STB2_L;
	Send_8bit(DIS_ADDR00);
	Delay_uS(10);
	for(i=0;i<14;i++)
	{
	 Send_8bit(TM1628_buf2[i]);
	}
        STB2_H;
        #elif HT1628_DisMode == ADDR_FIXED
        Command2(DISSEGCM512);    //设置显示模式  7位10段模式  
        Command2(ADDR_FIXED);     //数据命令  固定地址
	STB2_L;
	Send_8bit(0xC0|add);     //HT1628地址B7 B6固定为1
	Delay_uS(10);
	Send_8bit(TM1628_buf2[add]);
        STB2_H;
        #endif
        
	Delay_uS(5);
	Command2(DIS_RIGH114|DIS_ON);//亮度//显示控制    脉冲宽度10/16
	
}


//
void Init_Bot_TM1628(unsigned char LED_Status)
{	unsigned char i;
        STB2_O;
        CLK_O;
        DIO_O;

	Command2(DISSEGCM512); //设置显示模式 0x00 7位10段模式
	Command2(ADDR_AUTOUP); //数据命令：地址自动加1
        STB2_L;
	Send_8bit(DIS_ADDR00);//地址命令	
	Delay_uS(20);
	for(i=0;i<14;i++)
	{ 
          Send_8bit(LED_Status);
          //TM1628_buf2[i]=LED_Status;
	};
	STB2_H;
	Delay_uS(20);
	Command2(DIS_RIGH114|DIS_ON);//亮度//显示控制
};

void TM1628_Read_Key()  
{ 
  unsigned char i,j;  
  STB2_O;
  CLK_O;
  DIO_O;
  Command2(0x42);  //读键盘命令  
  DIO_H;
  DIO_I;
  STB2_L;
  //DIO=1;      //将DIO置高   
  Delay_uS(10);
  for(j=0;j<5;j++) //连续读取5个字节   
  {
    for(i=0;i<8;i++)    // LED驱动控制专用电路   
      {        
        Band_Switch_Key[j]=Band_Switch_Key[j]>>1; 
        CLK_L;
        Delay_uS(10);
        CLK_H;    
        Delay_uS(10);
        if(DIO_R)          
          Band_Switch_Key[j]=Band_Switch_Key[j]|0X80;  
      }  
  }
  DIO_O;
  STB2_H; 
} 





