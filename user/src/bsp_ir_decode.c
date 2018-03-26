/*
*********************************************************************************************************
*
*	ģ������ : ����ң�ؽ���������ģ��
*	�ļ����� : bsp_ir_decode.c
*	��    �� : V1.0
*	˵    �� : ����ң�ؽ��յĺ����ź�����CPU�� PA8/TIM1_CH1.  ����������ʹ��TIM1_CH1ͨ�������벶������
*				Э�����롣
*
*	�޸ļ�¼ :
*		�汾��  ����         ����     ˵��
*		V1.0    2016-07-07   ������  ��ʽ����
*
*
*********************************************************************************************************
*/

#include "bsp.h"

#define IR_REPEAT_SEND_EN		1	/* ����ʹ�� */
#define IR_REPEAT_FILTER		10	/* ң����108ms ��������������, ��������1��������ط� */

/* ����GPIO�˿� */
#define RCC_IRD		RCC_APB2Periph_GPIOA
#define PORT_IRD	GPIOA
#define PIN_IRD		GPIO_Pin_8

void TIM_PWMIConfig34(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct);

IRD_T g_tIR;

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitIRD
*	����˵��: ����STM32��GPIO,���ں���ң��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitIRD(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_IRD, ENABLE);

	/* ����DQΪ�������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */
	GPIO_InitStructure.GPIO_Pin = PIN_IRD;
	GPIO_Init(PORT_IRD, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: IRD_StartWork
*	����˵��: ����TIM����ʼ����
*	��    ��: ��
*	�� �� ֵ: ��
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
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			/* ÿ�����䶼����1�β����¼� */
	TIM_ICInitStructure.TIM_ICFilter = 0x7;
	
	TIM_ICInit(TIM1, &TIM_ICInitStructure);
	
	/* ���÷�ƵΪ 1680/2�� ���������ֵ�ĵ�λ������ 10us, ��������Ƚ� */
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
*	�� �� ��: IRD_StopWork
*	����˵��: ֹͣ�������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IRD_StopWork(void)
{
	TIM_Cmd(TIM1, DISABLE);
	
	TIM_ITConfig(TIM1, TIM_IT_CC1, DISABLE);	
}

/*
*********************************************************************************************************
*	�� �� ��: IRD_DecodeNec
*	����˵��: ����NEC�����ʽʵʱ����
*	��    ��: _width �����ȣ���λ 10us
*	�� �� ֵ: ��
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
	
	/* NEC ��ʽ ��5�Σ�
		1��������  9ms�� + 4.5ms��
		2����8λ��ַ��  0=1.125ms  1=2.25ms    bit0�ȴ�
		3����8λ��ַ��  0=1.125ms  1=2.25ms
		4��8λ����      0=1.125ms  1=2.25ms
		5��8Ϊ���뷴��  0=1.125ms  1=2.25ms
	*/
// //width_lenth[width_conut] = _width;
// 	width_conut++;
loop1:	
	switch (g_tIR.Status)
	{
		case 0:			/* 929 �ȴ���������ź�  7ms - 11ms */
			if ((_width > 1300) && (_width < 1400))
			{
				g_tIR.Status = 1;
				s_Byte = 0;
				s_Bit = 0;
			}
			break;

// 		case 1:			/* 413 �ж���������ź�  3ms - 6ms */
// 			if ((_width > 313) && (_width < 600))	/* ������ 4.5ms */
// 			{
// 				g_tIR.Status = 2;
// 			}
// 			else if ((_width > 150) && (_width < 250))	/* 2.25ms */
// 			{
// 				#ifdef IR_REPEAT_SEND_EN				
// 					if (g_tIR.RepeatCount >= IR_REPEAT_FILTER)
// 					{
// 						bsp_PutKey(g_tIR.RxBuf[2] + IR_KEY_STRAT);	/* ������ */
// 					}
// 					else
// 					{
// 						g_tIR.RepeatCount++;
// 					}
// 				#endif
// 				g_tIR.Status = 0;	/* ��λ����״̬ */
// 			}
// 			else
// 			{
// 				/* �쳣���� */
// 				g_tIR.Status = 0;	/* ��λ����״̬ */
// 			}
// 			break;
		
// 		case 2:			/* �͵�ƽ�ڼ� 0.56ms */
// 			if ((_width > 10) && (_width < 100))
// 			{		
// 				g_tIR.Status = 3;
// 				s_LowWidth = _width;	/* ����͵�ƽ��� */
// 			}
// 			else	/* �쳣���� */
// 			{
// 				/* �쳣���� */
// 				g_tIR.Status = 0;	/* ��λ������״̬ */	
// 				goto loop1;		/* �����ж�ͬ���ź� */
// 			}
// 			break;

		case 1:			/* 85+25, 64+157 ��ʼ��������32bit */						
			TotalWitdh =  _width;
			/* 0�Ŀ��Ϊ1.125ms��1�Ŀ��Ϊ2.25ms */				
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
				/* �쳣���� */
				g_tIR.Status = 0;	/* ��λ������״̬ */	
				goto loop1;		/* �����ж�ͬ���ź� */
			}
			
			s_Bit++;
			if (s_Bit == 8)	/* ����8λ */
			{
				g_tIR.RxBuf[0] = s_Byte;
				s_Byte = 0;
			}
			else if (s_Bit == 16)	/* ����16λ */
			{
				g_tIR.RxBuf[1] = s_Byte;
				s_Byte = 0;
			}
			else if (s_Bit == 24)	/* ����24λ */
			{
				g_tIR.RxBuf[2] = s_Byte;
				s_Byte = 0;
			}
			else if (s_Bit == 32)	/* ����32λ */
			{
				g_tIR.RxBuf[3] = s_Byte;
								
				if (g_tIR.RxBuf[2] + g_tIR.RxBuf[3] == 255)	/* ���У�� */
				{
					bsp_PutKey(g_tIR.RxBuf[2] + IR_KEY_STRAT);	/* ����ֵ����KEY FIFO */
					
					g_tIR.RepeatCount = 0;	/* �ط������� */										
				}
				
				g_tIR.Status = 0;	/* �ȴ���һ����� */
				break;
			}
			g_tIR.Status = 1;	/* ������һ��bit */
			break;						
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TIM1_IRQHandler
*	����˵��: TIM1�жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
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

		NowCapture = TIM_GetCapture1(TIM1);	/* ��ȡ����ļ�����ֵ��������ֵ��0-65535ѭ������ */
    //NowCapture_temp[NowCapture_temp_count++] = NowCapture;
		if (NowCapture >= g_tIR.LastCapture)
		{
			Width = NowCapture - g_tIR.LastCapture;
		}
		else if (NowCapture < g_tIR.LastCapture)	/* �������ִ���󲢷�ת */
		{
			Width = ((0xFFFF - g_tIR.LastCapture) + NowCapture);
		}			
		
		if ((g_tIR.Status == 0) && (g_tIR.LastCapture == 0))
		{
			g_tIR.LastCapture = NowCapture;
			return;
		}
				
		g_tIR.LastCapture = NowCapture;	/* ���浱ǰ�������������´μ����ֵ */
		
		IRD_DecodeNec(Width);		/* ���� */		
	}
}


/***************************** wml *********************************/
