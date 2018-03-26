#ifndef _HT1621_H_
#define _HT1621_H_

#ifdef HT1621_LOCAL
#define HT1621_EXTERN
#else
#define HT1621_EXTERN extern 
#endif

#include "stm32f10x.h"
//#include "delay.h"


//////////��������////////////

HT1621_EXTERN void Ht1621Wr_Data(unsigned char Data,unsigned char cnt) ;      

HT1621_EXTERN void Ht1621WrOneData(unsigned char Addr,unsigned char Data) ;

HT1621_EXTERN void Ht1621WrAllData(unsigned char Dis_Status) ;

HT1621_EXTERN void Ht1621WrCmd(unsigned char Cmd) ;

HT1621_EXTERN void Ht1621_Init(void); 

HT1621_EXTERN void S_Write_1621(unsigned char MemAddress,unsigned char *p,unsigned char i);

HT1621_EXTERN void Buzzer(int mS);

HT1621_EXTERN void TEST( void );



//IO����
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









//�����
#define COMMAND_MODE  0x80
/* 100  */
#define WRITE_MODE   0xA0
/* 101  */
#define READ_MODE  0xC0
/* 110  */
#define BIAS   0x52   
/* 1/3BIAS 1/4DUTY */
#define XTAL32 0x28   
/* ʹ���ⲿ���� */
#define RC256  0x30   
/*ʹ���ڲ�256KRC ����*/
#define SYSEN  0x02   
/*���񵴷�����*/
#define SYSDIS 0x00   
/*�ر��񵴷�����*/
#define LCDON  0x06   
/*��LCD*/
#define LCDOFF 0x04   
/*��ʾ�ر�*/
#define TONE4  0x64   
/*����BZ ���Ƶ��Ϊ4K*/
#define TONE2  0x60   
/*����BZ ���Ƶ��Ϊ2K*/
#define TONEON 0x12   
/*��BZ ��Ƶ���*/
#define TONEOFF 0x10  
/*�ر�BZ ��Ƶ���*/
#define CLRWDT 0x0E   
/*����WDT*/
#define F1     0xA0   
/*WDT����Ϊ1 �����*/
#define F2     0xA1   
/*WDT����Ϊ0.5�����*/
#define IRQEN  0x88  
/*IRQ �����ֹ*/
#define IRQDIS 0x80   
/*IRQ �������*/
#define WDTEN  0x07   
/*��WDT*/
#define WDTDIS 0x0a  
/*�ر�WDT*/
#define TIMERDIS 0x04 
/*�ر�ʱ�����*/



#endif
