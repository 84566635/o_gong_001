/*
*********************************************************************************************************
*
*	模块名称 : 红外遥控接收器驱动模块
*	文件名称 : bsp_ir_decode.h
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_IR_DECODE_H
#define _BSP_IR_DECODE_H


#include "stdint.h"
/* 定义红外遥控器按键代码, 和bsp_key.h 的物理按键代码统一编码 */
//typedef enum
//{
//	IR_KEY_STRAT 	= 0x80,
//	IR_KEY_POWER 	= IR_KEY_STRAT,//
//	IR_KEY_MENU 	= IR_KEY_STRAT + 0x02, //
//	IR_KEY_PLUS 	= IR_KEY_STRAT + 0x04,//
//	IR_KEY_UP 		= IR_KEY_STRAT + 0x05,//
//	IR_KEY_RETURN	= IR_KEY_STRAT + 0x06,
//	IR_KEY_LEFT		= IR_KEY_STRAT + 0x08,
//	IR_KEY_OK		  = IR_KEY_STRAT + 0x09,
//	IR_KEY_RIGHT	= IR_KEY_STRAT + 0x0A,
//	IR_KEY_SUB		= IR_KEY_STRAT + 0x0C,
//	IR_KEY_DOWN		= IR_KEY_STRAT + 0x0D,
//	IR_KEY_0		= IR_KEY_STRAT + 0x0E,
//	IR_KEY_1		= IR_KEY_STRAT + 0x10,
//	IR_KEY_2		= IR_KEY_STRAT + 0x11,
//	IR_KEY_3		= IR_KEY_STRAT + 0x12,
//	IR_KEY_4		= IR_KEY_STRAT + 0x14,
//	IR_KEY_5		= IR_KEY_STRAT + 0x15,
//	IR_KEY_6		= IR_KEY_STRAT + 0x16,
//	IR_KEY_7		= IR_KEY_STRAT + 0x18,
//	IR_KEY_8		= IR_KEY_STRAT + 0x19,
//	IR_KEY_9		= IR_KEY_STRAT + 0x1A,	
//}IR_KEY_E;
//0xc6 0xc4 0xc0 0xc3  0x89 0x87
#define IR_KEY_STRAT     0x80
#define IR_KEY_POWER     0xC6
#define IR_KEY_MODE      0xC4
#define IR_KEY_SPEED     0xC0
#define IR_KEY_RECYCLE   0xC3
#define IR_KEY_ANION     0x89
#define IR_KEY_TIME      0x87
typedef struct
{
	uint16_t LastCapture;
	uint8_t Status;
	uint8_t RxBuf[4];
	uint8_t RepeatCount;
}IRD_T;

void bsp_InitIRD(void);
void IRD_StartWork(void);
void IRD_StopWork(void);

extern IRD_T g_tIR;

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
