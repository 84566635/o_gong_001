/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.c
*	版    本 : V1.0
*	说    明 : 驱动LED指示灯
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-07-07  吴铭林   正式发布
*
*	Copyright (C), 2016-2020, 吴铭林 15989485769
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


	如果用户的LED指示灯个数小于4个，可以将多余的LED全部定义为和第1个LED一样，并不影响程序功能
*/

	/*
		     LED口线分配：
			LED1       : PE2 (低电平点亮，高电平熄灭)
			LED2       : PE3 (低电平点亮，高电平熄灭)
			LED3       : PE4 (低电平点亮，高电平熄灭)
			LED4       : PE5 (低电平点亮，高电平熄灭)
	*/
	#define RCC_ALL_LED 	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC	/* 按键口对应的RCC时钟 */

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
*	函 数 名: bsp_InitLed
*	功能说明: 配置LED指示灯相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_ALL_LED, ENABLE);

	/*
		配置所有的LED指示灯GPIO为推挽输出模式
		由于将GPIO设置为输出时，GPIO输出寄存器的值缺省是0，因此会驱动LED点亮.
		这是我不希望的，因此在改变GPIO为输出前，先关闭LED指示灯
	*/
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */

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
*	函 数 名: bsp_LedOn
*	功能说明: 点亮指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
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
*	函 数 名: bsp_LedOff
*	功能说明: 熄灭指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
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
*	函 数 名: bsp_LedToggle
*	功能说明: 翻转指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 按键代码
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
*	函 数 名: bsp_IsLedOn
*	功能说明: 判断LED指示灯是否已经点亮。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 1表示已经点亮，0表示未点亮
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

