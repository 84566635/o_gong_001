#ifdef	INTERRUPT_INIT
#define	INTERRUPT_TYPE_EXT
#else
#define	INTERRUPT_TYPE_EXT extern
#endif

#ifndef INTERRUPT_INIT_H
#define INTERRUPT_INIT_H
#define TIM_USART_CLK  72000000 
#define TIM_UART_CLOCK 1000000
#define ADC1_DR_Address    ((u32)0x4001244C)

INTERRUPT_TYPE_EXT void TIM1_PWM_init(void);
INTERRUPT_TYPE_EXT void NVIC_Configuration(void);
INTERRUPT_TYPE_EXT void ADC_InitFun(void);
INTERRUPT_TYPE_EXT void USART_InitFun(void);
INTERRUPT_TYPE_EXT void DMA_InitFun(void);
//INTERRUPT_TYPE_EXT vu16 AD_Value[2];
INTERRUPT_TYPE_EXT void TIM2_Configuration(void);
//INTERRUPT_TYPE_EXT void SetT1PWM(vu16 Pulse);
INTERRUPT_TYPE_EXT void TIM4_Configuration(void);
INTERRUPT_TYPE_EXT void EXTI_PD_Config(void);
INTERRUPT_TYPE_EXT void SPI_InitFun(void);
INTERRUPT_TYPE_EXT void SPI1_DMA_Configuration( void );
INTERRUPT_TYPE_EXT void SPI2_DMA_Configuration( void );
INTERRUPT_TYPE_EXT void EXTI_PC_Config(void);
INTERRUPT_TYPE_EXT void GPIO_Configuration(void);
INTERRUPT_TYPE_EXT void TIM6_Configuration(void);
INTERRUPT_TYPE_EXT void TIM3_Configuration(void);

// INTERRUPT_TYPE_EXT void TimUsart_Configuration(void);
#endif
