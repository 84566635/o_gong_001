/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.c
*	��    �� : V1.0
*	˵    �� : ����LEDָʾ��
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-07-07  ������   ��ʽ����
*
*	Copyright (C), 2016-2020, ������ 15989485769
*
*********************************************************************************************************
*/

#include "bsp.h"

#define R_LED_CO2 1
#define B_LED_CO2 2
#define G_LED_CO2 3
#define R_LED_PM  4
#define G_LED_PM  5
#define B_LED_PM  6
#define POWER_LED 7
/*


	����û���LEDָʾ�Ƹ���С��4�������Խ������LEDȫ������Ϊ�͵�1��LEDһ��������Ӱ�������
*/

	/*
		     LED���߷��䣺
			LED1       : PE2 (�͵�ƽ�������ߵ�ƽϨ��)
			LED2       : PE3 (�͵�ƽ�������ߵ�ƽϨ��)
			LED3       : PE4 (�͵�ƽ�������ߵ�ƽϨ��)
			LED4       : PE5 (�͵�ƽ�������ߵ�ƽϨ��)
	*/
	#define RCC_ALL_LED 	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC	/* �����ڶ�Ӧ��RCCʱ�� */

	#define GPIO_PORT_LED1  GPIOA     //R_LED_CO2
	#define GPIO_PIN_LED1	GPIO_Pin_1

	#define GPIO_PORT_LED2  GPIOC    //G_LED_CO2
	#define GPIO_PIN_LED2	GPIO_Pin_14

	#define GPIO_PORT_LED3  GPIOC    //B_LED_CO2
	#define GPIO_PIN_LED3	GPIO_Pin_15

	#define GPIO_PORT_LED4  GPIOB    //R_LED_PM2.5
	#define GPIO_PIN_LED4	GPIO_Pin_8
	
	#define GPIO_PORT_LED5  GPIOB    //G_LED_PM2.5
	#define GPIO_PIN_LED5	GPIO_Pin_9
	
	#define GPIO_PORT_LED6  GPIOB    //B_LED_PM2.5
	#define GPIO_PIN_LED6	GPIO_Pin_2
	
	#define GPIO_PORT_LED7  GPIOC    //lcd_LED
	#define GPIO_PIN_LED7	  GPIO_Pin_9
	
	#define GPIO_PORT_LED8  GPIOA    //lcd_LED
	#define GPIO_PIN_LED8	  GPIO_Pin_11



/**********************************************************************/
void co2_Red_Led(void)
{
  bsp_LedOn(R_LED_CO2);
	bsp_LedOff(G_LED_CO2);
	bsp_LedOff(B_LED_CO2);	  
}

/**********************************************************************/

void co2_Green_Led(void)
{
  bsp_LedOff(R_LED_CO2);
	bsp_LedOn(G_LED_CO2);
	bsp_LedOff(B_LED_CO2);	  
}
void co2_Yellow_Led(void)
{
  bsp_LedOn(R_LED_CO2);
	bsp_LedOn(G_LED_CO2);
	bsp_LedOff(B_LED_CO2);	  
}
/**********************************************************************/

void co2_Purple_Led(void)
{
  bsp_LedOn(R_LED_CO2);
	bsp_LedOff(G_LED_CO2);
	bsp_LedOn(B_LED_CO2);	  
}


/**********************************************************************/
void PM2_5_Red_Led(void)
{
  bsp_LedOn(R_LED_PM);
	bsp_LedOff(G_LED_PM);
	bsp_LedOff(B_LED_PM);	  
}

/**********************************************************************/
void PM2_5_Green_Led(void)
{
  bsp_LedOff(R_LED_PM);
	bsp_LedOn(G_LED_PM);
	bsp_LedOff(B_LED_PM);	  
}
void PM2_5_Yellow_Led(void)
{
  bsp_LedOn(R_LED_PM);
	bsp_LedOn(G_LED_PM);
	bsp_LedOff(B_LED_PM);	  
}
/**********************************************************************/
void PM2_5_Purple_Led(void)
{
  bsp_LedOn(R_LED_PM);
	bsp_LedOff(G_LED_PM);
	bsp_LedOn(B_LED_PM);	  
}

/**********************************************************************/

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_ALL_LED, ENABLE);

	/*
		�������е�LEDָʾ��GPIOΪ�������ģʽ
		���ڽ�GPIO����Ϊ���ʱ��GPIO����Ĵ�����ֵȱʡ��0����˻�����LED����.
		�����Ҳ�ϣ���ģ�����ڸı�GPIOΪ���ǰ���ȹر�LEDָʾ��
	*/
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED1;
	GPIO_Init(GPIO_PORT_LED1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED2;
	GPIO_Init(GPIO_PORT_LED2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED3;
	GPIO_Init(GPIO_PORT_LED3, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED4;
	GPIO_Init(GPIO_PORT_LED4, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED5;
	GPIO_Init(GPIO_PORT_LED5, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED6;
	GPIO_Init(GPIO_PORT_LED6, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED7;
	GPIO_Init(GPIO_PORT_LED7, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED8;
	GPIO_Init(GPIO_PORT_LED8, &GPIO_InitStructure);
	
	
	bsp_LedOff(1);
	bsp_LedOff(2);
	bsp_LedOff(3);
	bsp_LedOff(4);
	bsp_LedOff(5);
	bsp_LedOff(6);
	bsp_LedOff(7);
	bsp_LedOff(8);


}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOn
*	����˵��: ����ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no)
{
	//_no--;

	if (_no == 1)
	{
		GPIO_PORT_LED1->BRR = GPIO_PIN_LED1;
	}
	else if (_no == 2)
	{
		GPIO_PORT_LED2->BRR = GPIO_PIN_LED2;
	}
	else if (_no == 3)
	{
		GPIO_PORT_LED3->BRR = GPIO_PIN_LED3;
	}
	else if (_no == 4)
	{
		GPIO_PORT_LED4->BRR = GPIO_PIN_LED4;
	}
	else if (_no == 5)
	{
		GPIO_PORT_LED5->BRR = GPIO_PIN_LED5;
	}
	else if (_no == 6)
	{
		GPIO_PORT_LED6->BRR = GPIO_PIN_LED6;
	}
	else if (_no == 7)
	{
		GPIO_PORT_LED7->BRR = GPIO_PIN_LED7;
	}
	else if (_no == 8)
	{
		GPIO_PORT_LED8->BRR = GPIO_PIN_LED8;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOff
*	����˵��: Ϩ��ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	//_no--;

	if (_no == 1)
	{
		GPIO_PORT_LED1->BSRR = GPIO_PIN_LED1;
	}
	else if (_no == 2)
	{
		GPIO_PORT_LED2->BSRR = GPIO_PIN_LED2;
	}
	else if (_no == 3)
	{
		GPIO_PORT_LED3->BSRR = GPIO_PIN_LED3;
	}
	else if (_no == 4)
	{
		GPIO_PORT_LED4->BSRR = GPIO_PIN_LED4;
	}
	else if (_no == 5)
	{
		GPIO_PORT_LED5->BSRR = GPIO_PIN_LED5;
	}
	else if (_no == 6)
	{
		GPIO_PORT_LED6->BSRR = GPIO_PIN_LED6;
	}
	else if (_no == 7)
	{
		GPIO_PORT_LED7->BSRR = GPIO_PIN_LED7;
	}
	else if (_no == 8)
	{
		GPIO_PORT_LED8->BSRR = GPIO_PIN_LED8;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	if (_no == 1)
	{
		GPIO_PORT_LED1->ODR ^= GPIO_PIN_LED1;
	}
	else if (_no == 2)
	{
		GPIO_PORT_LED2->ODR ^= GPIO_PIN_LED2;
	}
	else if (_no == 3)
	{
		GPIO_PORT_LED3->ODR ^= GPIO_PIN_LED3;
	}
	else if (_no == 4)
	{
		GPIO_PORT_LED4->ODR ^= GPIO_PIN_LED4;
	}
	else if (_no == 5)
	{
		GPIO_PORT_LED5->ODR ^= GPIO_PIN_LED5;
	}
	else if (_no == 6)
	{
		GPIO_PORT_LED6->ODR ^= GPIO_PIN_LED6;
	}
	else if (_no == 7)
	{
		GPIO_PORT_LED7->ODR ^= GPIO_PIN_LED7;
	}
	else if (_no == 8)
	{
		GPIO_PORT_LED8->ODR ^= GPIO_PIN_LED8;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_IsLedOn
*	����˵��: �ж�LEDָʾ���Ƿ��Ѿ�������
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: 1��ʾ�Ѿ�������0��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_IsLedOn(uint8_t _no)
{
	if (_no == 1)
	{
		if ((GPIO_PORT_LED1->ODR & GPIO_PIN_LED1) == 0)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 2)
	{
		if ((GPIO_PORT_LED2->ODR & GPIO_PIN_LED2) == 0)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 3)
	{
		if ((GPIO_PORT_LED3->ODR & GPIO_PIN_LED3) == 0)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 4)
	{
		if ((GPIO_PORT_LED4->ODR & GPIO_PIN_LED4) == 0)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 5)
	{
		if ((GPIO_PORT_LED5->ODR & GPIO_PIN_LED5) == 0)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 6)
	{
		if ((GPIO_PORT_LED6->ODR & GPIO_PIN_LED6) == 0)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 7)
	{
		if ((GPIO_PORT_LED7->ODR & GPIO_PIN_LED7) == 0)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 8)
	{
		if ((GPIO_PORT_LED8->ODR & GPIO_PIN_LED8) == 0)
		{
			return 1;
		}
		return 0;
	}

	return 0;
}

