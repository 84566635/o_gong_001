/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "stdint.h"

/* 供外部调用的函数声明 */
void bsp_InitLed(void);
void bsp_LedOn(uint8_t _no);
void bsp_LedOff(uint8_t _no);
void bsp_LedToggle(uint8_t _no);
uint8_t bsp_IsLedOn(uint8_t _no);
void co2_Red_Led(void);
void co2_Green_Led(void);
void co2_Yellow_Led(void);
void co2_Purple_Led(void);
void PM2_5_Red_Led(void);
void PM2_5_Green_Led(void);
void PM2_5_Yellow_Led(void);
void PM2_5_Purple_Led(void);
#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
