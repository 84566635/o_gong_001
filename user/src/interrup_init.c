#define INTERRUPT_INIT
#include "stm32f10x_conf.h"    
#include "interrupt_init.h"
#include "main.h"
#define SPI1_DR_Addr ( (u32)0x4001300C )
#define SPI2_DR_Addr ( (u32)0x4000380C )
vu16 AD_Value[2];
vu8 PM25DMABuffer[PM_DATA_NUM];


void TIM2_Configuration(void) //64/2*2=64MHZ
{
		TIM_TimeBaseInitTypeDef TIM_TimeBase;                                     
		TIM_DeInit(TIM2);
		TIM_TimeBase.TIM_Period=(3000-1);
		TIM_TimeBase.TIM_Prescaler=63;
		TIM_TimeBase.TIM_ClockDivision = TIM_CKD_DIV1;  
		TIM_TimeBase.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM2,&TIM_TimeBase);
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		TIM_ClearFlag(TIM2,TIM_FLAG_Update);
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    //TIM_Cmd(TIM2,ENABLE);
	  //TIM2->CR1 |= TIM_CR1_CEN;
}

	void TIM3_Configuration(void)
{
    TIM_OCInitTypeDef  TIM_OCInitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBase;                                     
		TIM_DeInit(TIM3);
		TIM_TimeBase.TIM_Period=249;
		TIM_TimeBase.TIM_Prescaler=31;//36/144=0.25MHZ=4us//64/256 = 0.25MHZ=4us
		TIM_TimeBase.TIM_ClockDivision = TIM_CKD_DIV1;  
		TIM_TimeBase.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3,&TIM_TimeBase);
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM3, ENABLE);
	  TIM_CtrlPWMOutputs(TIM3,ENABLE);
	

				 
				 
	//TIM_Cmd(TIM3,ENABLE);
}

    

void USART_InitFun(void)
{
		/*	波特率为9600bps;
	*	8位数据长度;
	*	1个停止位，校验;
	*	禁用硬件流控制;
	*	禁止USART时钟;
	*	时钟极性低;
	*	在第1个边沿捕获数据
	*	最后一位数据的时钟脉冲不从 SCLK 输出； 
	*/
	/* 定义 USART 初始化结构体 USART_InitStructure */
  USART_InitTypeDef USART_InitStructure;
	/* 定义 USART 初始化结构体 USART_ClockInitStructure */
  USART_ClockInitTypeDef  USART_ClockInitStructure;
	DMA_InitTypeDef DMA_InitStructure; 

	
	USART_DeInit(USART1);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1 , &USART_InitStructure);

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1,ENABLE);
	

/****************************USART2***********************************/	
	USART_DeInit(USART2);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2 , &USART_InitStructure);
	//下面是时钟初始化
	

/* DMA1 Channel6 (triggered by USART2 Rx event) Config */ //?? STM32 datasheet 
	DMA_DeInit(DMA1_Channel6); 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR); 
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)PM25DMABuffer; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
	DMA_InitStructure.DMA_BufferSize = sizeof(PM25DMABuffer); 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
//	
	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE); 
	
		/* Enable USART2 DMA Rxrequest */ 
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE); 
	/* Enable DMA1 Channel6 */ 
	DMA_Cmd(DMA1_Channel6, ENABLE);
//	//USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
	
	
}

void NVIC_Configuration(void)

{

		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
		
 		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
 		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 		NVIC_Init(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

}

	void GPIO_Configuration(void)
	{
			GPIO_InitTypeDef GPIO_InitStruct;
		

		
		//HALL	
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;	
			GPIO_Init(GPIOA,&GPIO_InitStruct);
						

/**********************************************************************OUT************************/		
// 	   //PWM
		
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//< JTAG-DP Disabled and SW-DP Enabled *///PA15/JTDI  PB4/JNRST PB3/JTDO
		
// TIM3_CH2 PWM PA7			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;	
 			GPIO_Init(GPIOA,&GPIO_InitStruct);
			
			// PM_P_C  PA12			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;	
 			GPIO_Init(GPIOA,&GPIO_InitStruct);
			
			// CO_P_C  PA15			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;	
 			GPIO_Init(GPIOA,&GPIO_InitStruct);
			
			// ANION_C PC8			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;	
 			GPIO_Init(GPIOC,&GPIO_InitStruct);
			
			// PM_RST  PA6			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;	
 			GPIO_Init(GPIOA,&GPIO_InitStruct);
			
			// calib_CO2 PB14			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;	
 			GPIO_Init(GPIOB,&GPIO_InitStruct);
			
			// SPEAKER  PB12			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;	
 			GPIO_Init(GPIOB,&GPIO_InitStruct);
			
			// M_EN1 PC12			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;	
 			GPIO_Init(GPIOC,&GPIO_InitStruct);
			
			// M_EN2  PD2			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;	
 			GPIO_Init(GPIOD,&GPIO_InitStruct);
			
			// M_IN1 PC10			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;	
 			GPIO_Init(GPIOC,&GPIO_InitStruct);
			
			// M_IN2 PC11			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;	
 			GPIO_Init(GPIOC,&GPIO_InitStruct);
			
			// M_IN3  PA0			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;	
 			GPIO_Init(GPIOA,&GPIO_InitStruct);
			
			// M_IN4 PA4			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;	
 			GPIO_Init(GPIOA,&GPIO_InitStruct);
			
			// RST PB4/JNRST			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;	
 			GPIO_Init(GPIOB,&GPIO_InitStruct);
			
			// CD/RS PB5			
 			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;	
 			GPIO_Init(GPIOB,&GPIO_InitStruct);
			
	/*********************************************************************/		

		//AI输入		

//UART1
		/* Configure USART1 Tx (PA9) as alternate function push-pull */
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
	//usart 
		/* Configure USART1 Rx (PA10) as input floating */
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
//UART2
		/* Configure USART2 Tx (PA2) as alternate function push-pull */
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
	//usart 
		/* Configure USART2 Rx (PA3) as input floating */
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOA, &GPIO_InitStruct);

		
	}
