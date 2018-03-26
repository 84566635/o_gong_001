
#define HT1621_LOCAL
#include "HT1621.h"
//#include "delay.h"
//#include "define.h"
#include "bsp.h"
#include "sys.h"

#define uchar unsigned char
#define uint unsigned int


#define BUZZER USED

#ifndef  BUZZER 
#define BUZZER NOUSED
#endif

#define Delay_uS(a) delay_us(a)

#define Delay_mS(a) delay_ms(a)

//extern uchar LCD_buffer[31];

uchar LCD_buffer[31];

//void GPIO_IN_OUT_SET(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,char in_out_sel)
//{
//	GPIOx->CRL = 
//}



 //-----------------------------------------------------------------------------------------
 //函数名称：Ht1621Wr_Data() 
 //功 能：写数据函数,cnt为传送数据位数,数据传送为低位在前 
//-----------------------------------------------------------------------------------------
void Ht1621Wr_Data(uchar Data,uchar cnt) 
 { 
 uchar i; 
 for (i=0;i<cnt;i++)
   { 
   WR_1621_L;   //HT1621_WR=0; 
   Delay_uS(10);
   if((Data & 0x80)==0x80)
     {
     DATA_1621_H; //HT1621_DAT=1;
     }
     else
     {
     DATA_1621_L; //HT1621_DAT=0;
     };
   Delay_uS(10);
   WR_1621_H;   //HT1621_WR=1; 
   Data<<=1;    
   Delay_uS(10);
   }; 
 }





//-----------------------------------------------------------------------------------------
//函数名称：void Ht1621WrOneData(uchar Addr,uchar Data) 
//功能描述: HT1621在指定地址写入数据函数 
//参数说明：Addr为写入初始地址，Data为写入数据 
//说 明：因为HT1621的数据位4位，所以实际写入数据为参数的后4位 
//-----------------------------------------------------------------------------------------
void Ht1621WrOneData(uchar Addr,uchar Data) 
 { 
 CS_1621_L;                   //HT1621_CS=0; 
 Ht1621Wr_Data(WRITE_MODE,3); //写入数据标志101 
 Ht1621Wr_Data(Addr<<2,6);    //写入地址数据 
 Ht1621Wr_Data(Data<<4,4);    //写入数据 
 CS_1621_H;                   //HT1621_CS=1; 
 } 





//-----------------------------------------------------------------------------------------
//函数名称：void Ht1621WrAllData() 
//功能描述: HT1621连续写入方式函数 
//说 明：HT1621的数据位4位，此处每次数据为8位，写入数据总数按8位计算 
//-----------------------------------------------------------------------------------------
void Ht1621WrAllData(unsigned char Dis_Status) 
 { 
 uchar i; 
 CS_1621_L;                //HT1621_CS=0; 
 Ht1621Wr_Data(0xA0,3);    //写入数据标志101 
 Ht1621Wr_Data(0x00<<2,6); //写入地址数据 
 for (i=0;i<16;i++) 
 { 
  Ht1621Wr_Data(Dis_Status,8);   //写入数据 
  } 
  CS_1621_H;               //HT1621_CS=1; 
 } 





//-----------------------------------------------------------------------------------------
//函数名称：void Ht1621WrCmd(uchar Cmd) 
//功能描述: HT1621命令写入函数 
//参数说明：Cmd为写入命令数据 
//说 明：写入命令标识位100 
//-----------------------------------------------------------------------------------------
 void Ht1621WrCmd(uchar Cmd) 
 { 
 CS_1621_L;                //HT1621_CS=0; 
 Delay_uS(200);
 Ht1621Wr_Data(COMMAND_MODE,4);    //写入命令标志100 
 Ht1621Wr_Data(Cmd,8);     //写入命令数据 
 CS_1621_H;                //HT1621_CS=1; 
 Delay_uS(200);
 } 



//-----------------------------------------------------------------------------------------
//函数名称：void Ht1621_Init(void) 
//功能描述: HT1621初始化 
//说 明：初始化后，液晶屏所有字段均显示 
//-----------------------------------------------------------------------------------------
void Ht1621_Init()
 {
 WR_1621_O;
 DATA_1621_O;
 CS_1621_O;
 CS_1621_H;                //HT1621_CS=1; 
 WR_1621_H;                //HT1621_WR=1; 
 DATA_1621_H;              //HT1621_DAT=1; 
 Delay_mS(1);            //延时使LCD工作电压稳定 
 Ht1621WrCmd(BIAS);        
 Ht1621WrCmd(RC256);       //使用内部振荡器  
 Ht1621WrCmd(SYSEN); 
 Ht1621WrCmd(LCDON);
 Ht1621WrCmd(TONEOFF);
 }
      
      
      




//**************************
//功能：连续写入1621
//入参：MemAddress:内存映射地址 , *p:数据指针, i:数据数量
//回参：无
//日期：
//**************************
void S_Write_1621(uchar MemAddress,uchar *p,uchar i)
 { 
 uchar s; 
 CS_1621_L;                      //HT1621_CS=0; 
 Ht1621Wr_Data(WRITE_MODE,3);    //写入数据标志101 
 Ht1621Wr_Data(MemAddress,6);    //写入地址数据 
 for (s=0;s<i;s++) 
 { 
  Ht1621Wr_Data(*(p+s)<<4,4);    //写入数据  *(p+i) = p[i]
  } 
  CS_1621_H;                     //HT1621_CS=1; 
 } 



//****************************************
//功能：在LCD上显示ram的内容
//入参：Sec：秒
//  
//回参：无
//日期：
//****************************************
void Refresh_LCD(void)
{

S_Write_1621(0,LCD_buffer,31);
}

void Clear_LCD(void)
{
  char i;
	for(i=0;i<31;i++)
	{
		LCD_buffer[i] = 0;
	}
}


void Buzzer(int mS)
{
#if BUZZER == USED
 Ht1621WrCmd(TONEON);
#endif
 Delay_mS(mS);
 Ht1621WrCmd(TONEOFF);
}




 void TEST( void )
 {
 uchar i=0x1d;
 Ht1621_Init(); //上电初始化LCD 
 Delay_uS(250);  //延时一段时间 
 Ht1621WrAllData(0xff); 
 for(i=0;i<31;i++)
 {
 Ht1621WrOneData(i,0);//9
 };
 }

