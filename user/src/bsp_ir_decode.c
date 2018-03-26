/*
*********************************************************************************************************
*
*	模块名称 : 红外遥控接收器驱动模块
*	文件名称 : bsp_ir_decode.c
*	版    本 : V1.0
*	说    明 : 红外遥控接收的红外信号送入CPU的 PA8/TIM1_CH1.  本驱动程序使用TIM1_CH1通道的输入捕获功能来
*				协助解码。
*
*	修改记录 :
*		版本号  日期         作者     说明
*		V1.0    2016-07-07   吴铭林  正式发布
*
*
*********************************************************************************************************
*/

#include "bsp.h"

#define IR_REPEAT_SEND_EN		1	/* 连发使能 */
#define IR_REPEAT_FILTER		10	/* 遥控器108ms 发持续按下脉冲, 连续按下1秒后启动重发 */

/* 定义GPIO端口 */
#define RCC_IRD		RCC_APB2Periph_GPIOA
#define PORT_IRD	GPIOA
#define PIN_IRD		GPIO_Pin_8

void TIM_PWMIConfig34(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct);

IRD_T g_tIR;

/*
*********************************************************************************************************
*	函 数 名: bsp_InitIRD
*	功能说明: 配置STM32的GPIO,用于红外遥控器解码
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitIRD(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_IRD, ENABLE);

	/* 配置DQ为输入引脚 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */
	GPIO_InitStructure.GPIO_Pin = PIN_IRD;
	GPIO_Init(PORT_IRD, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: IRD_StartWork
*	功能说明: 配置TIM，开始解码
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void IRD_StartWork(void)
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBase;
//  TIM_OCInitTypeDef  TIM_OCInitStructure;	
	TIM_DeInit(TIM1);
	
	TIM_TimeBase.TIM_Period=65535;
	TIM_TimeBase.TIM_Prescaler=639;
	TIM_TimeBase.TIM_ClockDivision = TIM_CKD_DIV1;  
	TIM_TimeBase.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBase);
	
	/* TIM1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

// 	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //???PWM??1  
//     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
//     TIM_OCInitStructure.TIM_Pulse = 50;                                       //?????,???????????,??????  
//     TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //?????????CCR1?????  
//     TIM_OC1Init(TIM1, &TIM_OCInitStructure);                                    //????1      
//     TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);


	/* TIM1 chennel1 configuration : PA.8 */
  bsp_InitIRD();

	/* Enable the TIM1 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			/* 每次跳变都产生1次捕获事件 */
	TIM_ICInitStructure.TIM_ICFilter = 0x7;
	
	TIM_ICInit(TIM1, &TIM_ICInitStructure);
	
	/* 设置分频为 1680/2， 捕获计数器值的单位正好是 10us, 方便脉宽比较 */
  //TIM_PrescalerConfig(TIM1, 639, TIM_PSCReloadMode_Immediate);
	
	/* TIM enable counter */
	TIM_Cmd(TIM1, ENABLE);

	/* Enable the CC3 Interrupt Request */
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
	//TIM_CtrlPWMOutputs(TIM1,ENABLE);
	g_tIR.LastCapture = 0;	
	g_tIR.Status = 0;
}

/*
*********************************************************************************************************
*	函 数 名: IRD_StopWork
*	功能说明: 停止红外解码
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void IRD_StopWork(void)
{
	TIM_Cmd(TIM1, DISABLE);
	
	TIM_ITConfig(TIM1, TIM_IT_CC1, DISABLE);	
}

/*
*********************************************************************************************************
*	函 数 名: IRD_DecodeNec
*	功能说明: 按照NEC编码格式实时解码
*	形    参: _width 脉冲宽度，单位 10us
*	返 回 值: 无
*********************************************************************************************************
*/
//uint16_t width_lenth[50]= {0};
void IRD_DecodeNec(uint16_t _width)
{
	static uint16_t s_LowWidth;
	static uint8_t s_Byte;
	static uint8_t s_Bit;
	
//	static uint8_t width_conut = 0;
	uint16_t TotalWitdh;
	
	/* NEC 格式 （5段）
		1、引导码  9ms低 + 4.5ms高
		2、低8位地址码  0=1.125ms  1=2.25ms    bit0先传
		3、高8位地址码  0=1.125ms  1=2.25ms
		4、8位数据      0=1.125ms  1=2.25ms
		5、8为数码反码  0=1.125ms  1=2.25ms
	*/
// //width_lenth[width_conut] = _width;
// 	width_conut++;
loop1:	
	switch (g_tIR.Status)
	{
		case 0:			/* 929 等待引导码低信号  7ms - 11ms */
			if ((_width > 1300) && (_width < 1400))
			{
				g_tIR.Status = 1;
				s_Byte = 0;
				s_Bit = 0;
			}
			break;

// 		case 1:			/* 413 判断引导码高信号  3ms - 6ms */
// 			if ((_width > 313) && (_width < 600))	/* 引导码 4.5ms */
// 			{
// 				g_tIR.Status = 2;
// 			}
// 			else if ((_width > 150) && (_width < 250))	/* 2.25ms */
// 			{
// 				#ifdef IR_REPEAT_SEND_EN				
// 					if (g_tIR.RepeatCount >= IR_REPEAT_FILTER)
// 					{
// 						bsp_PutKey(g_tIR.RxBuf[2] + IR_KEY_STRAT);	/* 连发码 */
// 					}
// 					else
// 					{
// 						g_tIR.RepeatCount++;
// 					}
// 				#endif
// 				g_tIR.Status = 0;	/* 复位解码状态 */
// 			}
// 			else
// 			{
// 				/* 异常脉宽 */
// 				g_tIR.Status = 0;	/* 复位解码状态 */
// 			}
// 			break;
		
// 		case 2:			/* 低电平期间 0.56ms */
// 			if ((_width > 10) && (_width < 100))
// 			{		
// 				g_tIR.Status = 3;
// 				s_LowWidth = _width;	/* 保存低电平宽度 */
// 			}
// 			else	/* 异常脉宽 */
// 			{
// 				/* 异常脉宽 */
// 				g_tIR.Status = 0;	/* 复位解码器状态 */	
// 				goto loop1;		/* 继续判断同步信号 */
// 			}
// 			break;

		case 1:			/* 85+25, 64+157 开始连续解码32bit */						
			TotalWitdh =  _width;
			/* 0的宽度为1.125ms，1的宽度为2.25ms */				
			s_Byte >>= 1;
			if ((TotalWitdh > 92) && (TotalWitdh < 132))
			{
				;					/* bit = 0 */
			}
			else if ((TotalWitdh > 205) && (TotalWitdh < 245))
			{
				s_Byte += 0x80;		/* bit = 1 */
			}	
			else
			{
				/* 异常脉宽 */
				g_tIR.Status = 0;	/* 复位解码器状态 */	
				goto loop1;		/* 继续判断同步信号 */
			}
			
			s_Bit++;
			if (s_Bit == 8)	/* 收齐8位 */
			{
				g_tIR.RxBuf[0] = s_Byte;
				s_Byte = 0;
			}
			else if (s_Bit == 16)	/* 收齐16位 */
			{
				g_tIR.RxBuf[1] = s_Byte;
				s_Byte = 0;
			}
			else if (s_Bit == 24)	/* 收齐24位 */
			{
				g_tIR.RxBuf[2] = s_Byte;
				s_Byte = 0;
			}
			else if (s_Bit == 32)	/* 收齐32位 */
			{
				g_tIR.RxBuf[3] = s_Byte;
								
				if (g_tIR.RxBuf[2] + g_tIR.RxBuf[3] == 255)	/* 检查校验 */
				{
					bsp_PutKey(g_tIR.RxBuf[2] + IR_KEY_STRAT);	/* 将键值放入KEY FIFO */
					
					g_tIR.RepeatCount = 0;	/* 重发计数器 */										
				}
				
				g_tIR.Status = 0;	/* 等待下一组编码 */
				break;
			}
			g_tIR.Status = 1;	/* 继续下一个bit */
			break;						
	}
}

/*
*********************************************************************************************************
*	函 数 名: TIM1_IRQHandler
*	功能说明: TIM1中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
//uint16_t NowCapture_temp[50];
//static uint16_t NowCapture_temp_count=0;
void TIM1_CC_IRQHandler(void)
{
	uint16_t NowCapture;
	uint16_t Width;
	
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1))
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);

		NowCapture = TIM_GetCapture1(TIM1);	/* 读取捕获的计数器值，计数器值从0-65535循环计数 */
    //NowCapture_temp[NowCapture_temp_count++] = NowCapture;
		if (NowCapture >= g_tIR.LastCapture)
		{
			Width = NowCapture - g_tIR.LastCapture;
		}
		else if (NowCapture < g_tIR.LastCapture)	/* 计数器抵达最大并翻转 */
		{
			Width = ((0xFFFF - g_tIR.LastCapture) + NowCapture);
		}			
		
		if ((g_tIR.Status == 0) && (g_tIR.LastCapture == 0))
		{
			g_tIR.LastCapture = NowCapture;
			return;
		}
				
		g_tIR.LastCapture = NowCapture;	/* 保存当前计数器，用于下次计算差值 */
		
		IRD_DecodeNec(Width);		/* 解码 */		
	}
}


/***************************** wml *********************************/
