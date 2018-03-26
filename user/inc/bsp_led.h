/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "stdint.h"

/* ���ⲿ���õĺ������� */
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
