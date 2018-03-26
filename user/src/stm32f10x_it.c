/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "interrupt_init.h"


#include "stm32f10x.h"
#include "main.h"


uint8_t usart_CO2_rec_temp[CO2_DATA_NUM] ={0};
uint8_t usart_rec_count = 0,flag_getCO2_value = 0,flag_getPM_value = 0;
extern vu8 PM25DMABuffer[PM_DATA_NUM];//test
char usart2_count;



	
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
		//DisAllPWMout();
//		Update_DutyCycle(200);
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
// void SysTick_Handler(void)
// {
// }

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/*****************usart2 rx****************/
void DMA1_Channel6_IRQHandler(void)
{
	
	flag_getPM_value = 1;
	//PM_PW_DISABLE;
	USART_Cmd(USART2,DISABLE);
	DMA_ClearITPendingBit(DMA1_IT_TC6);
	
}


/************************/
// void USART1_IRQHandler(void)
// {
//  
//  if((USART_GetITStatus(USART1, USART_IT_RXNE)!= RESET ))
//  {
// 	 if(flag_getCO2_value == 0)
// 	 {
// 		 if(usart_rec_count<8)
// 		 {
// 			 usart_CO2_rec_temp[usart_rec_count] = (uint8_t)(USART_ReceiveData(USART1));
// 			 usart_rec_count++;
// 		 }
// 		 else if(usart_rec_count==8)
// 		 {
// 			 usart_rec_count = 0;
// 			 usart_CO2_rec_temp[8] = (uint8_t)USART_ReceiveData(USART1);
// 			 flag_getCO2_value = 1;
// 		 }
// 	 }
// 	 USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//  }
// }
///////////////////////////////////////////////
void USART1_IRQHandler(void)
{
 
 if((USART_GetITStatus(USART1, USART_IT_RXNE)!= RESET ))
 {
	 if(flag_getCO2_value == 0)
	 {
		// if(usart_rec_count<8)
		 {
			 usart_CO2_rec_temp[usart_rec_count] = (uint8_t)(USART_ReceiveData(USART1));
			 usart_rec_count++;
		 }
		 TIM2->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
		 TIM2->CNT = 0;
		 TIM2->CR1 |= TIM_CR1_CEN;
// 		 else if(usart_rec_count==8)
// 		 {
// 			 usart_rec_count = 0;
// 			 usart_CO2_rec_temp[8] = (uint8_t)USART_ReceiveData(USART1);
// 			 flag_getCO2_value = 1;
// 		 }
	 }
	 USART_ClearITPendingBit(USART1, USART_IT_RXNE);
 }
}

void TIM2_IRQHandler(void)
{		
   if(TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET)
	 {
	 		 usart_rec_count = 0;
			 flag_getCO2_value = 1;
		   TIM2->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
		   TIM2->CNT = 0;
		   TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
   }
}

//////////////////////////////////////////////////
 void USART2_IRQHandler(void)
 {
  
  if((USART_GetITStatus(USART2, USART_IT_RXNE)!= RESET ))
  {

		if (usart2_count<16)
		{
			usart2_count++;
    }
		else
		{
			usart2_count = 0;
    }
		PM25DMABuffer[usart2_count] = USART_ReceiveData(USART2);
 	 USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
 }


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
 
 
}


void TIM1_BRK_IRQHandler(void)
{
 if(TIM_GetITStatus(TIM1, TIM_IT_Break)!= RESET)
 {


	  TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
 }
}

void TIM1_UP_IRQHandler(void)
{
 if(TIM_GetITStatus(TIM1, TIM_IT_Update)!= RESET);
 TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}


