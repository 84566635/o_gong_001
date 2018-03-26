#ifndef	MAIN_H 
#define	MAIN_H 

#ifdef	THE_MAIN
#define	MAIN_TYPE_EXT
#else
#define	MAIN_TYPE_EXT extern
#endif
#include"stm32f10x.h"

#define DC_MOTOR_EN           TIM3->CCMR1 =0x6000; TIM_CtrlPWMOutputs(TIM3,ENABLE),TIM_Cmd(TIM3,ENABLE)
#define DC_MOTOR_DISABLE      TIM3->CCMR1 =0x4000; TIM_Cmd(TIM3,DISABLE)
#define PM_PW_EN              GPIOA->BSRR = GPIO_Pin_12  //  PA12
#define PM_PW_DISABLE         GPIOA->BRR  = GPIO_Pin_12  //  PA12
#define CO2_PW_EN             GPIOA->BSRR = GPIO_Pin_15  //  PA15
#define CO2_PW_DISABLE        GPIOA->BRR = GPIO_Pin_15  //  PA15


#define ANION_EN              GPIOC->BSRR = GPIO_Pin_8  //  PC8
#define ANION_DISABLE         GPIOC->BRR  = GPIO_Pin_8  //  PC8

#define PM_RST_EN             GPIOA->BRR  = GPIO_Pin_6  //  PA6
#define PM_RST_DISABLE        GPIOA->BSRR = GPIO_Pin_6  //  PA6

#define CO2_CALIB_EN          GPIOB->BRR  = GPIO_Pin_14  //  PB14
#define CO2_CALIB_DISABLE     GPIOB->BSRR = GPIO_Pin_14  //  PB14

#define SPEAKER_EN            GPIOB->BSRR  = GPIO_Pin_12  //  PB12
#define SPEAKER_DISABLE       GPIOB->BRR = GPIO_Pin_12  //  PB12

#define M_EN1_EN              GPIOC->BSRR = GPIO_Pin_12  //  PC12
#define M_EN1_DISABLE         GPIOC->BRR = GPIO_Pin_12  //  PC12
#define M_EN2_EN              GPIOD->BSRR = GPIO_Pin_2  //  PD2
#define M_EN2_DISABLE         GPIOD->BRR = GPIO_Pin_2  //  PD2
#define M_IN1_LOW             GPIOC->BRR = GPIO_Pin_10  //  PC10
#define M_IN1_HIGH            GPIOC->BSRR = GPIO_Pin_10  //  PC10
#define M_IN2_LOW             GPIOC->BRR = GPIO_Pin_11  //  PC11
#define M_IN2_HIGH            GPIOC->BSRR = GPIO_Pin_11  //  PC11
#define M_IN3_LOW             GPIOA->BRR = GPIO_Pin_0  //  PA0
#define M_IN3_HIGH            GPIOA->BSRR = GPIO_Pin_0  //  PA0
#define M_IN4_LOW             GPIOA->BRR = GPIO_Pin_4  //  PA4
#define M_IN4_HIGH            GPIOA->BSRR = GPIO_Pin_4  //  PA4

#define LCD_RST_EN            GPIOB->BSRR = GPIO_Pin_4  //  PB4
#define LCD_RST_DISABLE       GPIOB->BSRR = GPIO_Pin_4  //  PB4
#define LCD_RS_LOW            GPIOB->BSRR = GPIO_Pin_5  //  PB5
#define LCD_RS_HIGH           GPIOB->BSRR = GPIO_Pin_5  //  PB5

#define PM_DATA_NUM 35
#define CO2_DATA_NUM 20

#define DC_MOTOR_LOW_SPEED       3
#define DC_MOTOR_MID_SPEED       4
#define DC_MOTOR_HIGH_SPEED      9
#define DC_MOTOR_STRONG_SPEED    249

MAIN_TYPE_EXT	 	struct control_flags
		{
			unsigned char get_speed      :1;
			unsigned char uart_send      :1;
			unsigned char bat_low        :1;
			unsigned char down           :1;
			unsigned char over_speed     :1;
			unsigned char speed_gather   :1;
			unsigned char limittimeend   :1;
			unsigned char limittimestart :1;
			unsigned      Stop:1;
			unsigned  Wheel_Direction:1;
		}Ctr_Flag;
		
#endif


