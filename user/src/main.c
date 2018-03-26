/*
  */

#define THE_MAIN

#include"stm32f10x_conf.h"
#include "sys.h"
#include "interrupt_init.h"
#include "stm32f10x_rcc.h"
#include<stdio.h>
#include "iic.h"
#include "stdint.h"
#include "main.h"
#include "lcd.h"
#include "bsp.h"
#include "display.h"
#include "HT1621.h"


/* Private -------------------------------------------------------------------*/
uint8_t get_CO2_CMD[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
/*--------------------------------------------------------------------------------------*/	
extern vu8 PM25DMABuffer[PM_DATA_NUM];
char flag_PM_PW = 0,flag_CO_PW = 0,shut_off_all_sensor = 0;
/*---------------------------------------------------------------------------------------*/
typedef	 union U16_S16
	{
		vu16 uBuf16;
		vs16 sBuf16;
  }BUF_U16_S16;
	
	BUF_U16_S16 gyro_zero_compansate;

	extern uchar LCD_buffer[31];
	extern void Clear_LCD(void);
	extern void Refresh_LCD(void);
/*---------------------------------------------------------------------------------------------*/
	
extern uint8_t usart_CO2_rec_temp[CO2_DATA_NUM];
extern uint8_t usart_rec_count ,flag_getCO2_value,flag_getPM_value;
	
	#define HIGH_CO2         2500
	#define MID_CO2          1400
	#define MID_LOW_CO2      750
	#define HIGH_PM2_5       150
	#define MID_PM2_5        100
	#define MID_LOW_PM2_5    60
	
	/**********************************************************/
	union U8_U16_SIGN
	{
		unsigned char  Buf8[2];
		vu16 Buf16;
  }Co2_value,PM2_5_concentration;
	
		union U8_U32_USIGN
	{
		unsigned char  Buf8[4];
		vu32 Buf32;
  }PM2_5_value;

unsigned char display_conc_PM[7] = {0},display_num_PM[12] = {0},display_CO2_conc[7] = {0};

char getCheckSum(char *packet)
{
	char i=0,checksum=0;
	for(i = 1;i<8;i++)
	{
		checksum += packet[i];
  }
	checksum = 0xff -checksum;
	checksum += 1;
	return checksum;
}
	
u8 Do_Xor(u8 *p,u8 length) //数组，异或操作数组结束长度 指针参数不支持inline关键字？？？
{
  u8 i,result;
  result = *p;
  for(i=1;i<length;i++)
  {
    result ^= *(++p);
  }
  return result;
}

void speak_one_time()
{
	SPEAKER_EN;//20161110 need del
	delay_ms(100);
	SPEAKER_DISABLE;
}


void clear_PM_rec()
{
	int i;
	for(i=0;i<PM_DATA_NUM;i++)
	{
		PM25DMABuffer[i]= 0;
	}
}
void decode_PM_data()
{
	int i,data_st_p;
	
	for(i=0;i<PM_DATA_NUM;i++)
	{
		if(PM25DMABuffer[i]==0x4D)
		{
			data_st_p = i;
		  break;
		}
	}
	if(data_st_p<(PM_DATA_NUM-17))
	{
		if(PM25DMABuffer[data_st_p+1] == 0x50)
		{
		  PM2_5_concentration.Buf8[1]=PM25DMABuffer[data_st_p+4];
			PM2_5_concentration.Buf8[0]=PM25DMABuffer[data_st_p+5];
			PM2_5_value.Buf8[3] = PM25DMABuffer[data_st_p+8];
			PM2_5_value.Buf8[2] = PM25DMABuffer[data_st_p+9];
			PM2_5_value.Buf8[1] = PM25DMABuffer[data_st_p+10];
			PM2_5_value.Buf8[0] = PM25DMABuffer[data_st_p+8];
		}
	}
}

void conv_CO2_conc_str(uint val)
{
	uint temp= 0;
	temp = val;

	display_CO2_conc[0] =  temp/10000 + 48;
	temp = temp%10000;
	display_CO2_conc[1] =  temp/1000 + 48;
	temp = temp%1000;
	display_CO2_conc[2] =  temp/100 + 48;
	temp = temp%100;
	display_CO2_conc[3] =  temp/10 + 48;
	display_CO2_conc[4] =  temp%10 + 48;
}

void conv_PM_num_str(uint val)
{
	uint temp= 0;
	temp = val;
	display_num_PM[0] =  temp/10000000 + 48;
	temp = temp%10000000;
	display_num_PM[1] =  temp/1000000 + 48;
	temp = temp%1000000;
	display_num_PM[2] =  temp/100000 + 48;
	temp = temp%100000;
	display_num_PM[3] =  temp/10000 + 48;
	temp = temp%10000;
	display_num_PM[4] =  temp/1000 + 48;
	temp = temp%1000;
	display_num_PM[5] =  temp/100 + 48;
	temp = temp%100;
	display_num_PM[6] =  temp/10 + 48;
	display_num_PM[7] =  temp% + 48;
}

void conv_PM_conc_str(uint val)
{
	uint temp= 0;
	temp = val;
	display_conc_PM[0] =  temp/10000 + 48;
	temp = temp%10000;
	display_conc_PM[1] =  temp/1000 + 48;
	temp = temp%1000;
	display_conc_PM[2] =  temp/100 + 48;
	temp = temp%100;
	display_conc_PM[3] =  temp/10 + 48;
	display_conc_PM[4] =  temp%10 + 48;
}

void delay_us_motor(int usneed)
{
	int i;
	for(i=0;i<usneed;i++)
	{
		GPIOC->ODR ^= GPIO_Pin_12;
		GPIOD->ODR ^= GPIO_Pin_2;
	}
}

static char speed_count_direct = 0;
static char step_motor_position = 0;
static char step_motor_moving_flag = 0;
void motor_step_move(int step_num,char direct)
{
	static int step_state = 0;
	unsigned int i =0;
	//M_EN1_DISABLE;
	//M_EN2_DISABLE;
	M_EN1_EN;
	M_EN2_EN;
	while(i<step_num)
	{
		step_motor_moving_flag = 1;
		switch(step_state)
		{
				case 0:
								M_IN1_LOW;
								M_IN2_HIGH;
								M_IN3_HIGH;
								M_IN4_HIGH;
								break;
				case 1:
								M_IN1_LOW;
								M_IN2_LOW;
								M_IN3_HIGH;
								M_IN4_HIGH;
								break;
				case 2:
								M_IN1_HIGH;
								M_IN2_LOW;
								M_IN3_HIGH;
								M_IN4_HIGH;
								break;
				case 3:
								M_IN1_HIGH;
								M_IN2_LOW;
								M_IN3_LOW;
								M_IN4_HIGH;
								break;
				case 4:
								M_IN1_HIGH;
								M_IN2_HIGH;
								M_IN3_LOW;
								M_IN4_HIGH;
								break;
				case 5:
								M_IN1_HIGH;
								M_IN2_HIGH;
								M_IN3_LOW;
								M_IN4_LOW;
								break;
				case 6:
								M_IN1_HIGH;
								M_IN2_HIGH;
								M_IN3_HIGH;
								M_IN4_LOW;
								break;
				case 7:
								M_IN1_LOW;
								M_IN2_HIGH;
								M_IN3_HIGH;
								M_IN4_LOW;
								break;
				default:
					      M_IN1_LOW;
								M_IN2_LOW;
								M_IN3_LOW;
								M_IN4_LOW;
				        break;
		}
		if(direct==0)
		{
			step_state ++;
			if(step_state>=8)
			{
				step_state = 0;
      }
    }
		else
		{
			if(step_state>0)
			{
			  step_state--;
			}
			else
			{
				step_state = 7;
      }
			
    }
		i++;
		//delay_us_motor(50);
		delay_us(5000);
	}
	step_motor_moving_flag = 0;
	M_EN1_DISABLE;
	M_EN2_DISABLE;
}

void start_motor(void)
{
	 bsp_StartTimer(2,3000);//led start question
	 TIM3->CCR2 = 12;//48;
	 while(bsp_CheckTimer(2)==0);	 
}

void step_motor_init(void)
{
	static int step_state_init = 0;
	M_EN1_EN;
	M_EN2_EN;
	while((GPIOA->IDR & GPIO_Pin_5)!=0)
	{
		switch(step_state_init)
		{
				case 0:
								M_IN1_LOW;
								M_IN2_HIGH;
								M_IN3_HIGH;
								M_IN4_HIGH;
								break;
				case 1:
								M_IN1_LOW;
								M_IN2_LOW;
								M_IN3_HIGH;
								M_IN4_HIGH;
								break;
				case 2:
								M_IN1_HIGH;
								M_IN2_LOW;
								M_IN3_HIGH;
								M_IN4_HIGH;
								break;
				case 3:
								M_IN1_HIGH;
								M_IN2_LOW;
								M_IN3_LOW;
								M_IN4_HIGH;
								break;
				case 4:
								M_IN1_HIGH;
								M_IN2_HIGH;
								M_IN3_LOW;
								M_IN4_HIGH;
								break;
				case 5:
								M_IN1_HIGH;
								M_IN2_HIGH;
								M_IN3_LOW;
								M_IN4_LOW;
								break;
				case 6:
								M_IN1_HIGH;
								M_IN2_HIGH;
								M_IN3_HIGH;
								M_IN4_LOW;
								break;
				case 7:
								M_IN1_LOW;
								M_IN2_HIGH;
								M_IN3_HIGH;
								M_IN4_LOW;
								break;
				default:
					      M_IN1_LOW;
								M_IN2_LOW;
								M_IN3_LOW;
								M_IN4_LOW;
				        break;
		}
		if(step_state_init>0)
		{
			step_state_init--;
		}
		else
		{
			step_state_init = 7;
		}
		delay_us(5000);
	}
	step_motor_position = 0;
	M_EN1_DISABLE;
	M_EN2_DISABLE;
}
void step_motor_out(void)
{
	
	switch(step_motor_position)
	{
			case 0:
				     motor_step_move(1700,0);
						 break;
			case 1:
				     motor_step_move(1400,0);
						 break;
			case 2:
				     
						 break;
			case 3:
				     
						 break;
			default:
						 break;
  }
	step_motor_position = 2;
}
void step_motor_in(void)
{
	switch(step_motor_position)
	{
			case 0:
				     motor_step_move(300,0);
						 break;
			case 1:
				     
						 break;
			case 2:
				     motor_step_move(1400,1);
						 break;
			case 3:
						 break;
			default:
						 break;			
  }
	step_motor_position = 1;
}
// void step_motor_out(void)
// {
// 	
// 	switch(step_motor_position)
// 	{
// 			case 0:
// 				     motor_step_move(1333,0);
// 						 break;
// 			case 1:
// 						 break;
// 			case 2:
// 				     motor_step_move(1333,1);
// 						 break;
// 			case 3:
// 				     motor_step_move(2666,1);
// 						 break;
// 			default:
// 						 break;
//   }
// 	step_motor_position = 1;
// }
// void step_motor_in(void)
// {
// 	switch(step_motor_position)
// 	{
// 			case 0:
// 				     motor_step_move(3999,0);
// 						 break;
// 			case 1:
// 				     motor_step_move(2666,0);
// 						 break;
// 			case 2:
// 				     motor_step_move(1333,0);
// 						 break;
// 			case 3:
// 						 break;
// 			default:
// 						 break;			
//   }
// 	step_motor_position = 2;
// }
// void  step_motor_stright(void)
// {
// 	switch(step_motor_position)
// 	{
// 			case 0:
// 				     motor_step_move(2666,0);
// 						 break;
// 			case 1:
// 				     motor_step_move(1333,0);
// 						 break;
// 			case 2:
// 						 break;
// 			case 3:
// 				     motor_step_move(1333,1);
// 						 break;
// 			default:
// 						 break;
//   }
// 	step_motor_position = 2;
// }
void get_co2_value(void)
{
	char i;
	for(i=0;i<9;i++)
	{ 
		while((USART1->SR&0X40)==0);
		USART1->DR = get_CO2_CMD[i];
  }
}

  static char last_recycle_shut_flag = 0; 
	static int  timing_count = 0;
	static char timing_flag = 0,anion_PW_state = 0;
	static int  timing_time_min_value = 0;
  static char mode_time_switch_flag = 0,work_0_1_mode_state = 0,work_mode = 0,manul_flag = 0,all_power_state = 0,display_chang_face_flag = 0, co2_pollute_flag = 0,PM2_5_pollute_flag = 0,power_on_init_time_flag = 0,pollute_flag_last = 0;
	static int mode_time_count = 0;
  static char start_delay_30_flag = 0;
void power_off(void)
{
	bsp_LedOff(1);
	bsp_LedOff(2);
	bsp_LedOff(3);
	bsp_LedOff(4);
	bsp_LedOff(5);
	bsp_LedOff(6);
	bsp_LedOff(7);
	bsp_LedOff(8);
	
	flag_CO_PW = 0;
	flag_PM_PW = 0;
	CO2_PW_DISABLE;
	TIM3->CCR2 = 0;
	DC_MOTOR_DISABLE;
//	clear_screen();
	Clear_LCD();
	clear_PM_rec();
	flag_getCO2_value = 0;
	flag_getPM_value = 0;

	timing_count = 0;
	timing_flag = 0;
	timing_time_min_value = 0;
	bsp_StopTimer(0);
	bsp_StopTimer(1);
	
	work_0_1_mode_state = 0;
	mode_time_switch_flag = 0;
	mode_time_count =0;	
	work_mode = 0;
	all_power_state = 0;
	//display_chang_face_flag = 0;
	co2_pollute_flag = 0;
	PM2_5_pollute_flag = 0 ;
	power_on_init_time_flag = 0;
	
	ANION_DISABLE;
	anion_PW_state = 0;
	pollute_flag_last = 0;
	LCD_buffer[7] &= 0x07;//display_graphic_tree_clear();
	start_delay_30_flag = 0;
	last_recycle_shut_flag = 0;
}


uint8_t keys_show[30]={0};
static uint8_t keys_show_count=0;

int main(void)
{
	
	
  static char dc_motor_speed_count = 0,air_dir_recycle_count=0,timing_state = 0;
	static char last_dc_motor_speed_count = 0;
	static char led_off_flag = 0;
	RCC_ClocksTypeDef check_clock;
  uint8_t ucKeyCode;
	char co2_crc_value = 0;
	char co2_decode_count;
  sys();
	bsp_Init();

	bsp_InitIRD();		/* 配置GPIO  */
  GPIO_Configuration();
	
	CO2_CALIB_DISABLE;
  ANION_DISABLE;
	LCD_buffer[7] &= 0x07;//display_graphic_tree_clear();
	
	PM_RST_DISABLE;
	PM_PW_DISABLE;
	CO2_PW_DISABLE;
	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, DISABLE); 
	//PM_PW_EN;
  TIM2_Configuration();
	USART_InitFun();
	NVIC_Configuration();
	IRD_StartWork();
	RCC_GetClocksFreq(&check_clock);
	bsp_InitLed();
	//iic_init();

	TIM3_Configuration();
	DC_MOTOR_DISABLE;
//	motor_step_move(20000,0);
  Ht1621_Init();
  M_EN1_DISABLE;
	M_EN2_DISABLE;
	clear_PM_rec();
	//display_graphic_33x64(display_S);
	if((GPIOA->IDR & GPIO_Pin_5)!=0)	
	{
		step_motor_init();//20161110 need del
	}	
	while (1)
	{
		bsp_Idle();		/* 空闲时执行的函数,比如喂狗. 在bsp.c中 */
		
		if(all_power_state==1)
		{
			
							if(bsp_CheckTimer(3)==1)
							{
								start_delay_30_flag = 1;
							}
			
							if(flag_getCO2_value==1&&shut_off_all_sensor==0&&start_delay_30_flag==1)
							{
                   // co2_crc_value = getCheckSum(&usart_CO2_rec_temp[1]);
								    
										if(0x86==usart_CO2_rec_temp[1]&&0xFF==usart_CO2_rec_temp[0])
										{
													Co2_value.Buf8[0] = usart_CO2_rec_temp[3];
													Co2_value.Buf8[1] = usart_CO2_rec_temp[2];

													if(Co2_value.Buf16 > HIGH_CO2)
													{
														  if(led_off_flag==0)
															{
															  co2_Purple_Led();
															}
														  co2_pollute_flag = 1;
														  LCD_buffer[7] &= 0x0E;//display_graphic_led_clear();
														  LCD_buffer[7] |= 0x02;
													}
													else if(Co2_value.Buf16 > MID_CO2)	
													{
														  if(led_off_flag==0)
															{
															   co2_Red_Led();
															}
														  co2_pollute_flag = 1;
														  LCD_buffer[7] &= 0x0E;//display_graphic_led_clear();
														  LCD_buffer[7] |= 0x02;
													}
													else if(Co2_value.Buf16 > MID_LOW_CO2)
													{
														  if(led_off_flag==0)
															{
															   co2_Yellow_Led();
															}
														  LCD_buffer[7] &= 0x0D;//display_graphic_led_clear();
														 // LCD_buffer[7] |= 0x01;
													}
													else 
													{
														  if(led_off_flag==0)
															{
															   co2_Green_Led();
															}
														  co2_pollute_flag = 0;
														 
														  LCD_buffer[7] &= 0x0D;//display_graphic_led_clear();
														 // LCD_buffer[7] |= 0x01;
													}
										}
										else
										{
											for(co2_decode_count = 0;co2_decode_count<11;co2_decode_count++)
											{
												if(usart_CO2_rec_temp[co2_decode_count]==0xFF)
													break;
                      }
											if(co2_decode_count<11&&usart_CO2_rec_temp[co2_decode_count]==0x86)
											{
												  Co2_value.Buf8[0] = usart_CO2_rec_temp[co2_decode_count+2];
													Co2_value.Buf8[1] = usart_CO2_rec_temp[co2_decode_count+1];
													if(Co2_value.Buf16 > HIGH_CO2)
													{
														  if(led_off_flag==0)
															{
															   co2_Purple_Led();
															}
														  co2_pollute_flag = 1;
														  LCD_buffer[7] &= 0x0E;//display_graphic_led_clear();
														  LCD_buffer[7] |= 0x02;
													}
													else if(Co2_value.Buf16 > MID_CO2)	
													{
														  if(led_off_flag==0)
															{
															  co2_Red_Led();
															}
														  co2_pollute_flag = 1;
														  LCD_buffer[7] &= 0x0E;//display_graphic_led_clear();
														  LCD_buffer[7] |= 0x02;
													}
													else if(Co2_value.Buf16 > MID_LOW_CO2)
													{
														  if(led_off_flag==0)
															{
															   co2_Yellow_Led();
															}
														  //LCD_buffer[7] &= 0x0D;//display_graphic_led_clear();
														  //LCD_buffer[7] |= 0x01;
													}
													else 
													{
														  if(led_off_flag==0)
															{
															  co2_Green_Led();
															}
														  co2_pollute_flag = 0;
														  LCD_buffer[7] &= 0x0D;//display_graphic_led_clear();
														  //LCD_buffer[7] |= 0x01;
													}
                      }
                    }
								flag_getCO2_value = 0;		
										
							}
// 							if(flag_getPM_value==0&&flag_PM_PW==1)
// 							{
// 								USART_Cmd(USART2,ENABLE);
// 							}
							
							if(flag_getPM_value==1&&shut_off_all_sensor==0&&start_delay_30_flag==1)
							{
										decode_PM_data();

										if(PM2_5_concentration.Buf16 > HIGH_PM2_5)
										{
											  if(led_off_flag==0)
												{
												  PM2_5_Purple_Led();
												}
											  PM2_5_pollute_flag = 1;
											  LCD_buffer[7] &= 0x0E;//display_graphic_led_clear();
											  LCD_buffer[7] |= 0x04;
										}
										else if(PM2_5_concentration.Buf16 > MID_PM2_5)	
										{
											  if(led_off_flag==0)
												{
												   PM2_5_Red_Led();
												}
											  PM2_5_pollute_flag = 1;
											  LCD_buffer[7] &= 0x0E;//display_graphic_led_clear();
											  LCD_buffer[7] |= 0x04;
										}
										else if(PM2_5_concentration.Buf16 > MID_LOW_PM2_5)
										{
											  if(led_off_flag==0)
												{
												  PM2_5_Yellow_Led();
												}
											  if(PM2_5_pollute_flag==1)
												{
													LCD_buffer[7] |= 0x04;
                        }
											  //LCD_buffer[7] &= 0x0B;//display_graphic_led_clear();
										}
										else
										{
											  if(led_off_flag==0)
												{
												   PM2_5_Green_Led();
												}
												if(last_recycle_shut_flag==0)
												{
													PM2_5_pollute_flag = 0;
													LCD_buffer[7] &= 0x0B;
												}
										}
										clear_PM_rec();
										flag_getPM_value = 0;
							}
							if(PM2_5_pollute_flag==0&&co2_pollute_flag==0&&shut_off_all_sensor==0)
							{
								LCD_buffer[7] |= 0x01;
							}
							if((LCD_buffer[7]&0x07)==0&&led_off_flag==0&&shut_off_all_sensor==0)
							{
								LCD_buffer[7] |= 0x01;
							}
							
							if(timing_flag==1)
							{
										if(bsp_CheckTimer(0))
										{
											timing_count++;
											power_on_init_time_flag = 1;
										}
										if(timing_count>=timing_time_min_value)
										{
											 power_off();
										}
							}
							
							if(mode_time_switch_flag)
							{
								  if((co2_pollute_flag == 0)&&(PM2_5_pollute_flag == 0))
									{
																if(bsp_CheckTimer(1))
																{
																	mode_time_count++;
																}
																if(work_mode==0)//auto
																{
																	switch(work_0_1_mode_state)
																	{
																		
																		 case 0:
																					 if(mode_time_count>=3||pollute_flag_last==1)
																					 {
																						  pollute_flag_last = 0;
																							work_0_1_mode_state = 1;
																							mode_time_count =0;	
																						  {
																							    TIM3->CCR2 = DC_MOTOR_MID_SPEED;//48;
																						  	 //display_graphic_speed_33x64(display_mid_speed);
																									LCD_buffer[1] |= 0x08;//led speed
																									LCD_buffer[2]  = 0x0C;
																									dc_motor_speed_count = 2;
                                              }
																							dc_motor_speed_count = 2;
																							
																						  //if(display_chang_face_flag==0)
																							{
																							   //display_graphic_win_dir_39x64(display_out_dir);
																								LCD_buffer[1] &= 0x08;
																								LCD_buffer[1] |= 0x02;
																							}
																	
																							{
																								ANION_EN;
																								anion_PW_state = 1;
																								LCD_buffer[7] |= 0x08;//display_graphic_tree_32x64(display_tree);
																							}
																							Refresh_LCD();
																							step_motor_out();
																							
																							//display_graphic_33x64(display_A);
																					 }
																					 break;
																		case 1:
																					 if(mode_time_count>=2||pollute_flag_last==1)
																					 {
																						 pollute_flag_last = 0;
																							work_0_1_mode_state = 2;
																							mode_time_count =0;	
																							TIM3->CCR2 = 0;
																							dc_motor_speed_count = 4;
																							{
																									//display_graphic_speed_33x64(display_no_speed);
																									LCD_buffer[1] |= 0x08;//led speed
																									LCD_buffer[2]  = 0x00;                                            
                                               }
																								
																						  //if(display_chang_face_flag==0)
																							{
																							   //display_graphic_win_dir_39x64(display_out_dir);
																								LCD_buffer[1] &= 0x08;
																								LCD_buffer[1] |= 0x02;
																							}

																							{
																								ANION_DISABLE;
																								anion_PW_state = 0;
																								LCD_buffer[7] &= 0x07;//display_graphic_tree_clear();
																							}
																							Refresh_LCD();
																							step_motor_out();
																							
																					 }
																					 break;
																		case 2:							     
																					 if(mode_time_count>=1||pollute_flag_last==1)
																					 {
																						  pollute_flag_last = 0;
																							work_0_1_mode_state = 0;
																							mode_time_count =0;	
                                              {		
                                                  start_motor();																								
																									TIM3->CCR2 = DC_MOTOR_MID_SPEED;//48;	
																								 //display_graphic_speed_33x64(display_mid_speed);
																									LCD_buffer[1] |= 0x08;//led speed
																									LCD_buffer[2]  = 0x0C;
																									dc_motor_speed_count = 2;
                                              }
																							
																						  //if(display_chang_face_flag==0)
																							{
																							   LCD_buffer[1] &= 0x08;//display_graphic_win_dir_39x56(display_in_dir);
                                                 LCD_buffer[1] |= 0x03;																						
																							}

																							{
																								ANION_EN;
																								anion_PW_state = 1;
																								LCD_buffer[7] |= 0x08;//display_graphic_tree_32x64(display_tree);
																							}

                                              Refresh_LCD();	
                                              step_motor_in();																							
																					 }										 
																					 break;
																	}
																	
																}
																else if(work_mode==1||work_mode==2)
																{
																		switch(work_0_1_mode_state)
																		{
																			case 0:										 
																						 if(mode_time_count>=3||pollute_flag_last==1)
																						 {
																							  pollute_flag_last = 0;
																								work_0_1_mode_state = 1;
																								mode_time_count =0;	
																								TIM3->CCR2 = DC_MOTOR_LOW_SPEED;//48;
																							  {
																									//display_graphic_speed_33x64(display_low_speed);
																									LCD_buffer[1] |= 0x08;//led speed
																									LCD_buffer[2]  = 0x08;                                            
                                                }
																								dc_motor_speed_count = 3;
																								
																								//if(display_chang_face_flag==0)
																								{
																									 //display_graphic_win_dir_39x64(display_out_dir);
																									LCD_buffer[1] &= 0x08;
																									LCD_buffer[1] |= 0x02;
																								}
																								
																								{
																									ANION_EN;
																									anion_PW_state = 1;
																									LCD_buffer[7] |= 0x08;//display_graphic_tree_32x64(display_tree);
																								}

																								Refresh_LCD();
																								step_motor_out();
																								//display_graphic_33x64(display_A);
																						 }										 
																						 break;
																			case 1:										 
																						 if(mode_time_count>=2||pollute_flag_last==1)
																						 {
																							  pollute_flag_last = 0;
																								work_0_1_mode_state = 2;
																								mode_time_count =0;	
																								TIM3->CCR2 = 0;
																							  {
																									//display_graphic_speed_33x64(display_no_speed);
																									LCD_buffer[1] |= 0x08;//led speed
																									LCD_buffer[2]  = 0x00;                                            
                                                }
																								dc_motor_speed_count = 4;
																								
																								//if(display_chang_face_flag==0)
																								{
																									 //display_graphic_win_dir_39x64(display_out_dir);
																									LCD_buffer[1] &= 0x08;
																									LCD_buffer[1] |= 0x02;
																								}

																								{
																									ANION_DISABLE;
																									anion_PW_state = 0;
																									LCD_buffer[7] &= 0x07;//display_graphic_tree_clear();
																								}
																								Refresh_LCD();
																								step_motor_out();
																						 }
																						 break;
																		case 2:							     
																						 if(mode_time_count>=1||pollute_flag_last==1)
																						 {
																							  pollute_flag_last = 0;
																								work_0_1_mode_state = 0;
																								mode_time_count =0;		
																							  start_motor();
																								TIM3->CCR2 = DC_MOTOR_LOW_SPEED;//48;
																							  {
																									//display_graphic_speed_33x64(display_low_speed);
																									LCD_buffer[1] |= 0x08;//led speed
																									LCD_buffer[2]  = 0x08;                                            
                                                }
																								dc_motor_speed_count = 3;
																								
																								//if(display_chang_face_flag==0)
																								{
																									 LCD_buffer[1] &= 0x08;//display_graphic_win_dir_39x56(display_in_dir);
																									 LCD_buffer[1] |= 0x03;																						
																								}	

																								{
																									ANION_EN;
																									anion_PW_state = 1;
																									LCD_buffer[7] |= 0x08;//display_graphic_tree_32x64(display_tree);
																								}

                                                Refresh_LCD();
                                                step_motor_in();																								
																						 }										 
																						 break;
																		}
																}
											}// if(PM2_5_pollute_flag = 0&&PM2_5_pollute_flag = 0)
											else 
											{
												if(shut_off_all_sensor==0)
												{
													pollute_flag_last = 1;
													if(co2_pollute_flag==1)
													{
														  
															
															 if(display_chang_face_flag==0)
															{
														     //display_graphic_win_dir_39x64(display_out_dir);
																 LCD_buffer[1] &= 0x08;
																 LCD_buffer[1] |= 0x02;
															}
															
															if(work_mode==0)
															{
														      TIM3->CCR2 = DC_MOTOR_HIGH_SPEED;//48;
																  //display_graphic_speed_33x64(display_high_speed);
																	LCD_buffer[1] |= 0x08;//led speed
																	LCD_buffer[2]  = 0x0E;
															}
															else
															{
																  TIM3->CCR2 = DC_MOTOR_MID_SPEED;//48;
																  //display_graphic_speed_33x64(display_mid_speed);
																	LCD_buffer[1] |= 0x08;//led speed
																	LCD_buffer[2]  = 0x0C;
                              }
															if(step_motor_position!=2)
															{
																Refresh_LCD();
																step_motor_out() ;
															}
													}
													else
													{
														  
//														// if(display_chang_face_flag==0)
															{
																 //display_graphic_win_dir_39x56(display_in_dir);
																 LCD_buffer[1] &= 0x08;
																 LCD_buffer[1] |= 0x03;
														     
															}
															
															if(work_mode==0)
															{
														      TIM3->CCR2 = DC_MOTOR_HIGH_SPEED;//48;
																  //display_graphic_speed_33x64(display_high_speed);
																	LCD_buffer[1] |= 0x08;//led speed
																	LCD_buffer[2]  = 0x0E;
															}
															else
															{
																  TIM3->CCR2 = DC_MOTOR_MID_SPEED;//48;
																  //display_graphic_speed_33x64(display_mid_speed);
																	LCD_buffer[1] |= 0x08;//led speed
																	LCD_buffer[2]  = 0x0C;
                              }
															if(step_motor_position!=1)
															{
																	Refresh_LCD();
																	step_motor_in();
															}
															
													}
                      }
										}//if(shut_off_all_sensor==0)
							}//if(mode_time_switch_flag)
							
							if(power_on_init_time_flag==1)
							{
								  flag_CO_PW = 1;
              }
	   }//if(all_power_state==1)
		/* 按键滤波和检测由后台systick中断服务程序实现，我们只需要调用bsp_GetKey读取键值即可。 */
		ucKeyCode = bsp_GetKey();	/* 读取键值, 无键按下时返回 KEY_NONE = 0 */
		#ifdef debug
// 		if(keys_show[keys_show_count]!=ucKeyCode&&ucKeyCode!=0)
// 		{
// 			if(keys_show_count<30)
// 			keys_show_count++;
// 			keys_show[keys_show_count] = ucKeyCode;//0xc6 0xc4 0xc0 0xc3  0x89 0x87
//     }
		#endif
		if (ucKeyCode != KEY_NONE)
		{
			switch (ucKeyCode)
			{

				case IR_KEY_POWER:  //power 0xC6  
				case KEY_DOWN_K1:			/* 主板K1键按下 */	
                            speak_one_time();					
				                    if(all_power_state==0x0000)
														{
																 bsp_LedOn(7);
                                 bsp_LedOn(8);															
															   //clear_screen();
															
															   pollute_flag_last = 0;
																 shut_off_all_sensor = 0;
																 //display_graphic_win_dir_39x64(display_in_dir);
															   LCD_buffer[1] &= 0x08;
																 LCD_buffer[1] |= 0x03;
																 //display_graphic_33x64(display_A);
															   LCD_buffer[0]  = 0x09;
																 mode_time_switch_flag = 1;
															   
															   //display_graphic_speed_33x64(display_mid_speed);
															   LCD_buffer[1] |= 0x08;//speed led
															   LCD_buffer[2]  = 0x0C;//speed led
																 bsp_StartAutoTimer(1,60000);
																// flag_CO_PW = 1;
																	CO2_PW_EN;
															    co2_Green_Led();
																	flag_PM_PW = 1;	
															    LCD_buffer[6] |= 0x08;//speed led
																  manul_flag = 0;
															    work_mode = 0;
															  
															   timing_flag = 0;
															   timing_state = 5;
																 //bsp_StartAutoTimer(0,60000);
															   //timing_time_min_value = 120;
																  //display_graphic_27x64_single(display_time_2);
																 LCD_buffer[3]  = 0x0F;//speed led
																 LCD_buffer[4]  = 0x0D;//speed led
																 LCD_buffer[5]  = 0x0F;//speed led
																 LCD_buffer[6] &= 0x08;//speed led
																 LCD_buffer[6] |= 0x05;//speed led
																 
// 																 LCD_buffer[3]  = 0x00;//speed led
// 																 LCD_buffer[4]  = 0x08;//speed led
// 																 LCD_buffer[5]  = 0x0D;//speed led
// 																 LCD_buffer[6] &= 0x08;//speed led
// 																 LCD_buffer[6] |= 0x03;//speed led
																 clear_PM_rec();
																 flag_getCO2_value = 0;
	                               flag_getPM_value = 0;
															
															    ANION_EN;
																	anion_PW_state = 1;
																	//display_graphic_tree_32x64(display_tree);
		                             	LCD_buffer[7] |= 0x08;																		
                                 
																 bsp_StartTimer(3,30000);//led start question
																 PM2_5_Green_Led();
																 co2_Green_Led();
																 
                                Refresh_LCD();															 
                                step_motor_in();																	
																all_power_state = 1;
																DC_MOTOR_EN;
																start_motor();
																 TIM3->CCR2 = DC_MOTOR_MID_SPEED;//48;
															   dc_motor_speed_count = 1;
																flag_CO_PW = 1;
																
														}
														else
														{
															  LCD_buffer[1] &= 0x08;
																LCD_buffer[1] |= 0x03;
															  Refresh_LCD();
															  step_motor_in();
																power_off();
																all_power_state = 0;
																//DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
														}
														
				                  	break;

				case KEY_DOWN_K2:			/* 主板K2键按下 */
				case IR_KEY_MODE://0xc4
					                  
														if(all_power_state==1&&step_motor_moving_flag==0&&last_recycle_shut_flag == 0)
														{	
																	
//       															{
// 																			flag_PM_PW = 1;
//                                       LCD_buffer[6] |= 0x08;//speed led																			
// 																	    flag_CO_PW = 1;
// 																	    CO2_PW_EN;
// 																		}
															
															    speak_one_time();
															
															    if(work_mode<3)
																	{
																		work_mode++;
																	}
																	else
																	{
																		work_mode = 0;
																	}
																	
																	if(manul_flag==1)
																	{
																		manul_flag = 0;
																		work_mode = 0;
																		last_recycle_shut_flag = 0;
																	}
																	switch(work_mode)
																	{
																		case 0:
                                           bsp_LedOn(7);
                                           
//																		       if(display_chang_face_flag==0)
																					 {
																		         //display_graphic_win_dir_39x64(display_out_dir);
																						 //LCD_buffer[1] &= 0x08;
																             //LCD_buffer[1] |= 0x01;
																						 LCD_buffer[1] &= 0x08;//display_graphic_win_dir_39x56(display_in_dir);
                                             LCD_buffer[1] |= 0x03;	
																		         //display_graphic_33x64(display_A);
																						 LCD_buffer[0]  = 0x09;
																						 //display_graphic_speed_33x64(display_mid_speed);
																						 LCD_buffer[1] |= 0x08;//led speed
																						 LCD_buffer[2]  = 0x0C;
																						 
																					 }
																					 Refresh_LCD();
																					 step_motor_in();
																					 
																					 mode_time_switch_flag = 1;
																		       TIM3->CCR2 = DC_MOTOR_MID_SPEED;//48;
																		       DC_MOTOR_EN;
																		       dc_motor_speed_count = 2;
																		       
																		       bsp_StartAutoTimer(1,60000);
                                           //flag_CO_PW = 1;
																					 CO2_PW_EN;
																					 flag_PM_PW = 1;																	
                                           manul_flag = 0;	       
																					 led_off_flag = 0;															
																					 break;
																		case 1:
																			     bsp_LedOn(7);
																			     
//																		       if(display_chang_face_flag==0)
																					 {
																		          //display_graphic_win_dir_39x64(display_out_dir);
																						// LCD_buffer[1] &= 0x08;
																            // LCD_buffer[1] |= 0x01;
																						 LCD_buffer[1] &= 0x08;//display_graphic_win_dir_39x56(display_in_dir);
                                             LCD_buffer[1] |= 0x03;	
																						  //display_graphic_speed_33x64(display_low_speed);
																						 LCD_buffer[1] |= 0x08;//led speed
																						 LCD_buffer[2]  = 0x08;
																						  //display_graphic_33x64(display_S);
																						 LCD_buffer[0] = 0x0A;
																					 }
																					 Refresh_LCD();
																					 step_motor_in();//step_motor_out();
																					 
																					 
																					 mode_time_switch_flag = 1;
																		       TIM3->CCR2 = DC_MOTOR_LOW_SPEED;//22;
																		       dc_motor_speed_count = 3;																		       
																		       bsp_StartAutoTimer(1,60000);	
                                           //flag_CO_PW = 1;
																					 CO2_PW_EN;
																					 flag_PM_PW = 1;																					 
																		       manul_flag = 0;
																		       DC_MOTOR_EN;
																		       led_off_flag = 0;
																			     																	
																					 break;
																		case 2:
																			     bsp_LedOff(7);
																		       bsp_LedOff(1);
																						 bsp_LedOff(2);
																						 bsp_LedOff(3);
																			       bsp_LedOff(4);
																						 bsp_LedOff(5);
																						 bsp_LedOff(6);
																		         led_off_flag = 1;
																		         bsp_LedOff(5);
																						 bsp_LedOff(6);
																					 break;
																		
																		case 3:
																			      bsp_LedOn(7);
                                            bsp_LedOn(8);
																		        led_off_flag = 0;
																					 break;
																		default:
																			     
																					 {
																		         //display_graphic_win_dir_39x64(display_out_dir);
																						 LCD_buffer[1] &= 0x08;
																             LCD_buffer[1] |= 0x02;
																		         //display_graphic_33x64(display_A);
																						 LCD_buffer[0]  = 0x09;
																						 //display_graphic_speed_33x64(display_mid_speed);
																						 LCD_buffer[1] |= 0x08;//led speed
																						 LCD_buffer[2]  = 0x0C;
																						 
																					 }
																					 Refresh_LCD();
																					 step_motor_out();
																					 
																					 
																					 mode_time_switch_flag = 1;
																		       TIM3->CCR2 = DC_MOTOR_MID_SPEED;//48;
																		       DC_MOTOR_EN;
																		       dc_motor_speed_count = 2;
																		       led_off_flag = 0;
																		       bsp_StartAutoTimer(1,60000);
                                           //flag_CO_PW = 1;
																					 CO2_PW_EN;
																					 flag_PM_PW = 1;																	
                                           manul_flag = 0;
																					 break;
																	}
																	work_0_1_mode_state = 0;
												          mode_time_count =0;	
																	shut_off_all_sensor = 0;
														}
														//display_string_5x8(1,1,1,"kk");
                            
														break;
				case IR_KEY_SPEED://dc motor speed
        case KEY_DOWN_K3:				
					                  
                            if(all_power_state==1&&shut_off_all_sensor == 0)
														{				
															    speak_one_time();
															
															    bsp_LedOn(7);
															    //if(speed_count_direct==0)
                                  if(dc_motor_speed_count<3&&speed_count_direct==0)
																	{	
                                    speed_count_direct = 0;																		
																		dc_motor_speed_count++;
																	}
																	else
																	{
																		
																		speed_count_direct = 1;
																		dc_motor_speed_count--;
																		if(dc_motor_speed_count==0)
																		{
																			speed_count_direct = 0;
                                    }
																	}	
                                  if(manul_flag==0)	
																	{
																		dc_motor_speed_count = 0;
																		air_dir_recycle_count = 0;//recycle need clear beacase shutdown display is 2
																		led_off_flag = 0;
																		speed_count_direct = 0;
																	}																		
																	/***************/
																	switch (dc_motor_speed_count)
																	{
																			case 3: 
																				      if(speed_count_direct == 0 )
																							{
																								speed_count_direct = 1;
                                              }
																							TIM3->CCR2 = DC_MOTOR_LOW_SPEED;//22
																			        DC_MOTOR_EN;
																							//display_graphic_speed_33x64(display_low_speed);
																			        LCD_buffer[1] |= 0x08;//led speed
																						  LCD_buffer[2]  = 0x08;
																							break;
																			case 2: 
																							TIM3->CCR2 = DC_MOTOR_MID_SPEED;//48;
																							//display_graphic_speed_33x64(display_mid_speed);
																			        LCD_buffer[1] |= 0x08;//led speed
																						  LCD_buffer[2]  = 0x0C;
																							break;
																			case 1: 
																							TIM3->CCR2 = DC_MOTOR_HIGH_SPEED;//78;
																							//display_graphic_speed_33x64(display_high_speed);
																			        LCD_buffer[1] |= 0x08;//led speed
																						  LCD_buffer[2]  = 0x0E;
																							break;
																			case 0: 
																							TIM3->CCR2 = DC_MOTOR_STRONG_SPEED;//120;
																							//display_graphic_speed_33x64(display_strong_speed);
																			        LCD_buffer[1] |= 0x08;//led speed
																						  LCD_buffer[2]  = 0x0F;
																							break;
																			case 4: 
																							TIM3->CCR2 = 0;
																							DC_MOTOR_DISABLE;
																							//display_graphic_speed_33x64(display_no_speed);
																			        LCD_buffer[1] &= 0x07;//led speed
																						  LCD_buffer[2]  = 0x00;
																							break;
																			default:
																							TIM3->CCR2 = 0;
																							DC_MOTOR_DISABLE;
																							//display_graphic_speed_33x64(display_no_speed);
																			        LCD_buffer[1] |= 0x08;//led speed
																						  LCD_buffer[2]  = 0x00;
																							break;
																	}
																	
																	 manul_flag = 1;
																	 LCD_buffer[0]  = 0x0C;//display manual
																	 mode_time_switch_flag = 0;
													  }
														
					                  break;
															
				case KEY_DOWN_K4:	//air detect power
        case IR_KEY_RECYCLE:			
					                  
					                  if(all_power_state==1&&manul_flag==1&&step_motor_moving_flag==0)
														{	
															    speak_one_time();
															    if(air_dir_recycle_count<2)
																	{
																		air_dir_recycle_count++;
																	}
																	else
																	{
																		air_dir_recycle_count = 0;
																	}	
																	last_dc_motor_speed_count = dc_motor_speed_count;													    																	
																	switch(air_dir_recycle_count)
																	{
																			case 0:
																				     
																				     if(last_recycle_shut_flag==1)
																						 {
																							 last_recycle_shut_flag = 0;
																							 
																							 ANION_EN;
																							 anion_PW_state = 1;
																							 LCD_buffer[7] |= 0x08;//display_graphic_tree_32x64(display_tree);
																							 
																							 start_delay_30_flag = 0;
																							 bsp_StartTimer(3,30000);//led start question
																							 PM2_5_Green_Led();
																							 co2_Green_Led();
																							 
                                             }
																			       shut_off_all_sensor = 0;
																					   PM2_5_Green_Led();
																						 flag_PM_PW = 1;
																						 //LCD_buffer[6] |= 0x08;//speed led
																						 flag_CO_PW = 1;
																			       //power_on_init_time_flag = 0;
																			       co2_Green_Led();
																						 CO2_PW_EN;
																			       
																						 {
																							 LCD_buffer[1] &= 0x08;//display_graphic_win_dir_39x56(display_in_dir);
																							 LCD_buffer[1] |= 0x03;
																						 }
																						 //display_graphic_speed_33x64(display_mid_speed);
																						 LCD_buffer[1] |= 0x08;//led speed
																						 LCD_buffer[2]  = 0x0C;
																						 dc_motor_speed_count = 2;
																						 Refresh_LCD();
																						 start_motor();
																						 TIM3->CCR2 = DC_MOTOR_MID_SPEED;//48;
																							
																						 step_motor_in();
																						 
																						
																						 break;
																			case 1:
																				      
																						 {
																							 //display_graphic_win_dir_39x64(display_out_dir);
																							 LCD_buffer[1] &= 0x08;
																							 LCD_buffer[1] |= 0x02;
																						 }
																						 Refresh_LCD();
																					   step_motor_out();
																				     shut_off_all_sensor = 0;																			       
																						 break;
																			case 2:
																				     
																						 	
// 																						 power_off();
// 																		         //flag_CO_PW = 1;
// 																			       
// 																             all_power_state = 0;
// 																			       bsp_LedOn(7);
																			       led_off_flag = 0;
																			       last_recycle_shut_flag = 1;
																			       shut_off_all_sensor = 1;
																			
																			       TIM3->CCR2 = 0;
																						 dc_motor_speed_count = 4;
																						 //display_graphic_speed_33x64(display_no_speed);
																						 LCD_buffer[1] |= 0x08;//led speed
																						 LCD_buffer[2]  = 0x00;
																			
																			       mode_time_switch_flag = 0;
																						 CO2_PW_DISABLE;
																						 flag_CO_PW = 0;
																			       power_on_init_time_flag = 0;
																			       bsp_LedOff(1);
																						 bsp_LedOff(2);
																						 bsp_LedOff(3);
																			       //LCD_buffer[6] &= 0x07;//speed led
																			       flag_PM_PW = 0;
                                             bsp_LedOff(4);
																						 bsp_LedOff(5);
																						 bsp_LedOff(6);
																			       
																			       ANION_DISABLE;
																		         anion_PW_state = 0;
// 																		        // LCD_buffer[7] &= 0x07;//display_graphic_tree_clear();
 																						 LCD_buffer[7] = 0x00;
																			      // Clear_LCD();
                                             {
																							 //display_graphic_win_dir_39x64(display_stright_dir);
																							 LCD_buffer[1] &= 0x08;
																							 LCD_buffer[1] |= 0x04;
																						 }
																						 Refresh_LCD();
																						 step_motor_out();																			
																						 break;
																			case 3:
																						 																		
																						 break;
																			default:
																				     CO2_PW_DISABLE;
																			       bsp_LedOff(1);
																						 bsp_LedOff(2);
																						 bsp_LedOff(3);
																			       bsp_LedOff(4);
																						 bsp_LedOff(5);
																						 bsp_LedOff(6);
																						 flag_PM_PW = 0;
																			       LCD_buffer[6] &= 0x07;//speed led
																						 flag_CO_PW = 0;
																						 break;
																			
																	}
                                  																
																	
													  }
														
														break;
														
				case KEY_DOWN_K5:	//anion power
        case IR_KEY_ANION:			
					                  
					                  if(all_power_state==1)
														{			
															    speak_one_time();
																	if(anion_PW_state==0)
																	{
																		ANION_EN;
																		anion_PW_state = 1;
																		LCD_buffer[7] |= 0x08;//display_graphic_tree_32x64(display_tree);
																	}
																	else
																	{
																		ANION_DISABLE;
																		anion_PW_state = 0;
																		LCD_buffer[7] &= 0x07;//display_graphic_tree_clear();
																	}
													  }
														
														break;
														
			  case KEY_DOWN_K6:	//TIME
        case IR_KEY_TIME:	//第6键		
					                  
					                  if(all_power_state==1)
														{	
															    speak_one_time();
															    if(timing_state<5)
																	{
																		timing_state++;
																	}
																	else
																	{
																		timing_state = 0;
																	}
																	
																	if(timing_state!=5)
																	{
																		timing_flag = 1;
																	}
																	else
																	{
																		timing_flag = 0;
																	}
																	bsp_StartAutoTimer(0,60000);
																	switch(timing_state)
																	{
																		case 0:
																					 timing_time_min_value = 120;
																		       //display_graphic_54x64_single_clear();
																		       //display_graphic_27x64_single(display_time_2);
																		       LCD_buffer[3]  = 0x00;//speed led
																					 LCD_buffer[4]  = 0x08;//speed led
																					 LCD_buffer[5]  = 0x0D;//speed led
																					 LCD_buffer[6] &= 0x08;//speed led
																					 LCD_buffer[6] |= 0x03;//speed led
																					 break;
																		case 1:
																					 timing_time_min_value = 240;
																	         //display_graphic_27x64_single(display_time_4);
																		       LCD_buffer[3]  = 0x00;//speed led
																					 LCD_buffer[4]  = 0x08;//speed led
																					 LCD_buffer[5]  = 0x02;//speed led
																					 LCD_buffer[6] &= 0x08;//speed led
																					 LCD_buffer[6] |= 0x07;//speed led
																					 break;
																		case 2:
																					 timing_time_min_value = 480;
																		       //display_graphic_27x64_single(display_time_8);
																		       LCD_buffer[3]  = 0x00;//speed led
																					 LCD_buffer[4]  = 0x08;//speed led
																					 LCD_buffer[5]  = 0x0F;//speed led
																					 LCD_buffer[6] &= 0x08;//speed led
																					 LCD_buffer[6] |= 0x07;//speed led
																					 break;
																		case 3:
																					 timing_time_min_value = 720;
																		       //display_graphic_27x64_1(display_time_1);
																		       //display_graphic_27x64_2(display_time_2);
																					 LCD_buffer[3]  = 0x00;//speed led
																					 LCD_buffer[4]  = 0x0D;//speed led
																					 LCD_buffer[5]  = 0x0D;//speed led
																					 LCD_buffer[6] &= 0x08;//speed led
																					 LCD_buffer[6] |= 0x03;//speed led
																					 break;
																		
																		case 4:
																					 timing_time_min_value = 1440;
																		       //display_graphic_27x64_1(display_time_2);
																		       //display_graphic_27x64_2(display_time_4);																	
																		       LCD_buffer[3]  = 0x0D;//speed led
																					 LCD_buffer[4]  = 0x0B;//speed led
																					 LCD_buffer[5]  = 0x02;//speed led
																					 LCD_buffer[6] &= 0x08;//speed led
																					 LCD_buffer[6] |= 0x07;//speed led
																					 break;
																		case 5:
//                                         //ff																	
																		       LCD_buffer[3]  = 0x0F;//speed led
																					 LCD_buffer[4]  = 0x0D;//speed led
																					 LCD_buffer[5]  = 0x0F;//speed led
																					 LCD_buffer[6] &= 0x08;//speed led
																					 LCD_buffer[6] |= 0x05;//speed led
																					 break;
																		default:
																					 timing_time_min_value = 60;
																					 break;
																	}
																	
													  }
														
														break;
				
				default:
														if (ucKeyCode >= IR_KEY_STRAT)
														{
														}
														break;
			}
			
			Refresh_LCD();
		}
		
	}
}
