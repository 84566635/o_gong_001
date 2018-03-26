#include"stm32f10x_conf.h"
#include"stdio.h"
#include"stm32f10x_gpio.h"
#include "sys.h"
vu32 test_time;
void sys(void)
{
		volatile ErrorStatus HSEStartUpStatus; 
		RCC_DeInit();

	  RCC_HSEConfig(RCC_HSE_OFF);
 		HSEStartUpStatus = RCC_WaitForHSEStartUp(); 
	  RCC_HSICmd(ENABLE);
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
	  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
		FLASH_SetLatency(FLASH_Latency_2);
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);
		while(RCC_GetSYSCLKSource() != 0x08);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//DMA
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//模拟spi
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
 		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//使能定时器
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//使能定时器
 		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		
		PWR_BackupAccessCmd(ENABLE);//????RTC ??????
    RCC_LSEConfig(RCC_LSE_OFF);//?????????????? ?,PC13 PC14 PC15 ??????IO??
    PWR_BackupAccessCmd(DISABLE);//?????????
}                    


void delay_us(int usneed)
{
	int i,j;
	 	for (i=0;i<usneed;i++)
 	    for(j=0;j<1;j++);//72MHZ is 10284 times,becaus,1/72MHZ = 13.89,1000/13.89=71994,72/7 = 10;
}

void delay_ms(int msneed)
{
 	int i,j;

 	for (i=0;i<msneed;i++)
 	    for(j=0;j<10284;j++);//72MHZ is 10284 times,becaus,1/72MHZ = 13.89,1000000/13.89=71994,71994/7 = 10284;
	
}
