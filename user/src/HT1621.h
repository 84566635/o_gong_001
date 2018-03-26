#ifndef _HT1621_H_
#define _HT1621_H_

#ifdef HT1621_LOCAL
#define HT1621_EXTERN
#else
#define HT1621_EXTERN extern 
#endif

#include "stm32f10x.h"
//#include "delay.h"


//////////函数声明////////////

HT1621_EXTERN void Ht1621Wr_Data(unsigned char Data,unsigned char cnt) ;      

HT1621_EXTERN void Ht1621WrOneData(unsigned char Addr,unsigned char Data) ;

HT1621_EXTERN void Ht1621WrAllData(unsigned char Dis_Status) ;

HT1621_EXTERN void Ht1621WrCmd(unsigned char Cmd) ;

HT1621_EXTERN void Ht1621_Init(void); 

HT1621_EXTERN void S_Write_1621(unsigned char MemAddress,unsigned char *p,unsigned char i);

HT1621_EXTERN void Buzzer(int mS);

HT1621_EXTERN void TEST( void );



//IO设置
//WR....PB0  
//CS....PB1 
//DATA..PD2
//RD....NC

//#define GPIO_IN_OUT_SET() 

#define WR_1621_R   ((GPIOB->IDR & GPIO_PIN_6) != 0)?1:0//GPIO_ReadInputDataBit(SDAGPIO,SDA)//GPIO_ReadInputPin(GPIOB,GPIO_PIN_0)
#define CS_1621_R   ((GPIOB->IDR & GPIO_PIN_4) != 0)?1:0//GPIO_ReadInputDataBit(SDAGPIO,SDA)//GPIO_ReadInputPin(GPIOB,GPIO_PIN_1)
#define DATA_1621_R ((GPIOB->IDR & GPIO_PIN_7) != 0)?1:0//GPIO_ReadInputDataBit(SDAGPIO,SDA)//GPIO_ReadInputPin(GPIOB,GPIO_PIN_2)

#define WR_1621_O   GPIOB->CRL &=0xF0FFFFFF,GPIOB->CRL |=0x07000000//GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_FAST)
#define CS_1621_O   GPIOB->CRL &=0xFFF0FFFF,GPIOB->CRL |=0x00070000//GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_OUT_PP_HIGH_FAST)
#define DATA_1621_O GPIOB->CRL &=0x0FFFFFFF,GPIOB->CRL |=0x70000000//GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST)

#define WR_1621_I   GPIOB->CRL &=0xF0FFFFFF,GPIOB->CRL |=0x01000000,//GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_IN_PU_NO_IT)
#define CS_1621_I   GPIOB->CRL &=0xFFF0FFFF,GPIOB->CRL |=0x00010000,//GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_IN_PU_NO_IT)
#define DATA_1621_I GPIOB->CRL &=0x0FFFFFFF,GPIOB->CRL |=0x10000000,//GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT)

#define WR_1621_H   GPIOB->BSRR = GPIO_Pin_6//GPIO_WriteHigh(GPIOB, GPIO_PIN_0)
#define CS_1621_H   GPIOB->BSRR = GPIO_Pin_4//GPIO_WriteHigh(GPIOB, GPIO_PIN_1)
#define DATA_1621_H GPIOB->BSRR = GPIO_Pin_7//GPIO_WriteHigh(GPIOB, GPIO_PIN_2)

#define WR_1621_L   GPIOB->BRR = GPIO_Pin_6//GPIO_WriteLow(GPIOB, GPIO_PIN_0)
#define CS_1621_L   GPIOB->BRR = GPIO_Pin_4//GPIO_WriteLow(GPIOB, GPIO_PIN_1)
#define DATA_1621_L GPIOB->BRR = GPIO_Pin_7//GPIO_WriteLow(GPIOB, GPIO_PIN_2)









//命令定义
#define COMMAND_MODE  0x80
/* 100  */
#define WRITE_MODE   0xA0
/* 101  */
#define READ_MODE  0xC0
/* 110  */
#define BIAS   0x52   
/* 1/3BIAS 1/4DUTY */
#define XTAL32 0x28   
/* 使用外部晶振 */
#define RC256  0x30   
/*使用内部256KRC 振荡器*/
#define SYSEN  0x02   
/*打开振荡发生器*/
#define SYSDIS 0x00   
/*关闭振荡发生器*/
#define LCDON  0x06   
/*打开LCD*/
#define LCDOFF 0x04   
/*显示关闭*/
#define TONE4  0x64   
/*设置BZ 输出频率为4K*/
#define TONE2  0x60   
/*设置BZ 输出频率为2K*/
#define TONEON 0x12   
/*打开BZ 音频输出*/
#define TONEOFF 0x10  
/*关闭BZ 音频输出*/
#define CLRWDT 0x0E   
/*清零WDT*/
#define F1     0xA0   
/*WDT设置为1 秒溢出*/
#define F2     0xA1   
/*WDT设置为0.5秒溢出*/
#define IRQEN  0x88  
/*IRQ 输出禁止*/
#define IRQDIS 0x80   
/*IRQ 输出允许*/
#define WDTEN  0x07   
/*打开WDT*/
#define WDTDIS 0x0a  
/*关闭WDT*/
#define TIMERDIS 0x04 
/*关闭时基输出*/



#endif
