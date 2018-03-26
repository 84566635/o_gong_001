/***********************************************************************************************
************************************************************************************************/

#ifndef TM1628_H  //防止重复包含
#define TM1628_H  //防止重复包含

#include"stm32f10x.h"
#include<stdio.h>
#include "bsp.h"
#include "main.h"

// 函数声明
void Light_ON(unsigned char add,unsigned char Data);
void Light_OFF(unsigned char add,unsigned char Data);
void Send_8bit(unsigned char data);
void Command(unsigned char com);
void Init_TM1628(unsigned char LED_Status);

void Light2_ON(unsigned char add,unsigned char Data);
void Light2_OFF(unsigned char add,unsigned char Data);
void Command2(unsigned char com);
void Init_Bot_TM1628(unsigned char LED_Status);
#ifdef TM1628_H_LOCAL
#define TM1628_EXTERN_DECLAR
#else
 #define TM1628_EXTERN_DECLAR extern 
#endif
// 外部引用
extern unsigned char TM1628_buf[18];
extern unsigned char TM1628_buf2[18];

void TM1628_Read_Key() ;

////////////IO设置
//DIO....PC7  
//CLK....PC6
//STB....PC5  


#define DIO_R   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)//GPIO_ReadInputPin(GPIOC, GPIO_PIN_7)
#define CLK_R   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)//GPIO_ReadInputPin(GPIOC, GPIO_PIN_6)
#define STB_R   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)//GPIO_ReadInputPin(GPIOC, GPIO_PIN_5)

#define DIO_O   //GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_FAST)
#define CLK_O   //GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_FAST) 
#define STB_O   //GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST)
 
#define DIO_I   //GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT)
#define CLK_I   //GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT)
#define STB_I   //GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT)

#define DIO_H   GPIOB->BSRR = GPIO_Pin_7//GPIO_WriteHigh(GPIOC, GPIO_PIN_7)
#define CLK_H   GPIOB->BSRR = GPIO_Pin_6//GPIO_WriteHigh(GPIOC, GPIO_PIN_6)
#define STB_H   GPIOB->BSRR = GPIO_Pin_4//GPIO_WriteHigh(GPIOC, GPIO_PIN_5)

#define DIO_L   GPIOB->BRR = GPIO_Pin_7//GPIO_WriteLow(GPIOC, GPIO_PIN_7) 
#define CLK_L   GPIOB->BRR = GPIO_Pin_6//GPIO_WriteLow(GPIOC, GPIO_PIN_6)
#define STB_L   GPIOB->BRR = GPIO_Pin_4//GPIO_WriteLow(GPIOC, GPIO_PIN_5)



#define STB2_R   //GPIO_ReadInputPin(GPIOD, GPIO_PIN_0)


#define STB2_O   //GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_FAST)
 

#define STB2_I   //GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_IN_PU_NO_IT)


#define STB2_H   //GPIO_WriteHigh(GPIOD, GPIO_PIN_0)

#define STB2_L   //GPIO_WriteLow(GPIOD, GPIO_PIN_0)


////////////显段设定

#define		DISSEGCM413	0x00
#define		DISSEGCM512	0x01
#define		DISSEGCM611	0x02
#define		DISSEGCM710	0x03

////////////显示地址

#define		DIS_ADDR00	0xC0
#define		DIS_ADDR01	0xC1
#define		DIS_ADDR02	0xC2
#define		DIS_ADDR03	0xC3
#define		DIS_ADDR04	0xC4
#define		DIS_ADDR05	0xC5
#define		DIS_ADDR06	0xC6
#define		DIS_ADDR07	0xC7
#define		DIS_ADDR08	0xC8
#define		DIS_ADDR09	0xC9
#define		DIS_ADDR10	0xCA
#define		DIS_ADDR11	0xCB
#define		DIS_ADDR12	0xCC
#define		DIS_ADDR13	0xCD

////////命令
#define         ADDR_AUTOUP     0x40    // 地址自动加1模式
#define         ADDR_FIXED      0x44    // 地址固定模式
#define         GET_KEY         0x42    // 读取按键

////////显数命令
#define		DIS_FIXED	0x00		//写数据到显示寄存器

////////显亮度
#define		DIS_RIGH11	0x80		//设置脉冲宽度为 1/16
#define		DIS_RIGH12	0x81		//设置脉冲宽度为 2/16
#define		DIS_RIGH14	0x82		//设置脉冲宽度为 4/16
#define		DIS_RIGH110	0x83		//设置脉冲宽度为10/16
#define		DIS_RIGH111	0x84		//设置脉冲宽度为11/16
#define		DIS_RIGH112	0x85		//设置脉冲宽度为12/16
#define		DIS_RIGH113	0x86		//设置脉冲宽度为13/16
#define		DIS_RIGH114	0x87		//设置脉冲宽度为14/16

///////显示开关
#define         DIS_ON          0x88            //开启  
#define         DIS_OFF         0x80            //关闭

#define		SCAN_KEY_NUM	4		//扫描按键数..最大为5
//////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


 
#define LED1_NUM 13

struct DIGI_SHOW
{
int fist;   //10mS
int second;   //10mS
int third;   //10mS
int fourth;   //10mS
int time_mini;
int time_second;
int showtemp;
};


TM1628_EXTERN_DECLAR struct DIGI_SHOW SysDigiShow; 
//////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



#endif
