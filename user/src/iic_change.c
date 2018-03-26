#define MODEL_IIC
#include"stm32f10x_conf.h"
#include"sys.h"
#include"stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "iic.h"
//#include "thecode_main.h"

#define IIC_DELAY_TIME 8
#define IIC_DELAY_TIME_END 13//结束条件要多5us
#define WAIT_ACK_TIME 18
void iic_write8(unsigned char writedate);

void iic_ack()
{
	uint8_t i = WAIT_ACK_TIME;
// 	GPIO_InitTypeDef GPIO_InitStruct;
// 	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
// 	GPIO_InitStruct.GPIO_Pin = SDA;
// 	GPIO_Init(SDAGPIO,&GPIO_InitStruct);
	SDAGPIO->CRL &= 0x0FFFFFFF;
	SDAGPIO->CRL |= 0x30000000;
	iic_sda_high();
	delay_us(IIC_DELAY_TIME);
	iic_scl_high();
	//delay_us(IIC_DELAY_TIME);
	
// 	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
// 	GPIO_InitStruct.GPIO_Pin = SDA;
// 	GPIO_Init(SDAGPIO,&GPIO_InitStruct);
	SDAGPIO->CRL &= 0x0FFFFFFF;
	SDAGPIO->CRL |= 0x80000000;
	while ((iic_sda_read()!= 0)&&(i >1))i--;
	iic_scl_low();
	//delay_us(IIC_DELAY_TIME);
	//iic_sda_high();
}

void iic_nack(uint8_t ack)
{
	
// 	GPIO_InitTypeDef GPIO_InitStruct;
// 	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStruct.GPIO_Pin = SDA;
// 	GPIO_Init(SDAGPIO,&GPIO_InitStruct);
	SDAGPIO->CRL &= 0x0FFFFFFF;
	SDAGPIO->CRL |= 0x30000000;
	
	if(ack)
	{	
	  iic_sda_high();
	}
	else
	{
		iic_sda_low();
	}
	delay_us(IIC_DELAY_TIME);
	iic_scl_high();
	delay_us(IIC_DELAY_TIME);
	iic_scl_low();
	//delay_us(IIC_DELAY_TIME);
	//iic_sda_low();
}

void iic_start()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStruct.GPIO_Pin = SDA;
	GPIO_Init(SDAGPIO,&GPIO_InitStruct);
	iic_sda_high();
	delay_us(IIC_DELAY_TIME);
	iic_scl_high();
	delay_us(IIC_DELAY_TIME);
	iic_sda_low();
	delay_us(IIC_DELAY_TIME);
	iic_scl_low();
	//delay_us(IIC_DELAY_TIME);
}

void iic_stop()
{
// 	GPIO_InitTypeDef GPIO_InitStruct;
// 	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
// 	GPIO_InitStruct.GPIO_Pin = SDA;
// 	GPIO_Init(SDAGPIO,&GPIO_InitStruct);
	SDAGPIO->CRL &= 0x0FFFFFFF;
	SDAGPIO->CRL |= 0x30000000;
	//iic_scl_low();
 	//delay_us(IIC_DELAY_TIME);
	iic_sda_low();
	delay_us(IIC_DELAY_TIME);
	iic_scl_high();
	delay_us(IIC_DELAY_TIME_END);
	iic_sda_high();
	delay_us(IIC_DELAY_TIME_END);
}

void iic_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = SDA;
	GPIO_Init(SDAGPIO,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = SCL;
	GPIO_Init(SCLGPIO,&GPIO_InitStruct);
}

unsigned char iic_read8(void)
{
	int i;
	unsigned char receive;
	//GPIO_InitTypeDef GPIO_InitStruct;
	//GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStruct.GPIO_Pin = SDA;
	//GPIO_Init(SDAGPIO,&GPIO_InitStruct);
	SDAGPIO->CRL &= 0x0FFFFFFF;
	SDAGPIO->CRL |= 0x80000000;
	receive = 0;
//iic_sda_high();
	
	for(i=0;i<8;i++)
	{
		delay_us(IIC_DELAY_TIME);
		iic_scl_high();
		receive <<= 1;
		if(iic_sda_read()!=0)
		{
			receive |= 0x01;
		}	
		delay_us(IIC_DELAY_TIME);	
		iic_scl_low();
    	
		
	}
	return receive;
}

void iic_write8(unsigned char writedate)
{
	int i;
// 	GPIO_InitTypeDef GPIO_InitStruct;
// 	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStruct.GPIO_Pin = SDA;
// 	GPIO_Init(SDAGPIO,&GPIO_InitStruct);
	SDAGPIO->CRL &= 0x0FFFFFFF;
	SDAGPIO->CRL |= 0x30000000;
	
	for(i=0;i<8;i++)
	{
		
		
		if(0x80&writedate)
		{
				iic_sda_high();
		}	
    else
		{
			  iic_sda_low();
		}			
		delay_us(IIC_DELAY_TIME);
		iic_scl_high();				
		delay_us(IIC_DELAY_TIME);
		iic_scl_low();
		writedate <<= 1;
		
		
		//delay_us(IIC_DELAY_TIME);
    
		
	}
	//iic_scl_low();
	//delay_us(IIC_DELAY_TIME);
}



bool EE_WriteBytes(unsigned short romadd,unsigned char * iic_writedata,int num)
{
	int i;
	unsigned char temp;
	temp = (unsigned char)((romadd>>7)&0x0E);
	iic_wp_low();
	iic_start();
	iic_write8((writedecadd|temp));
	iic_ack();
	iic_write8((uint8_t)romadd);
	iic_ack();
	for(i=0;i<num;i++)
	{
	  iic_write8(*iic_writedata);
		iic_ack();
		//delay_ms(5);
		delay_us(15);
		iic_writedata++;
	}
	iic_stop();
	delay_us(15);
	iic_wp_high();
	
	return TRUE;
}

bool EE_ReadBytes(unsigned short romadd,unsigned char *iic_readdata,int num)
{
	int i;
	unsigned char temp;
	temp = (unsigned char)((romadd>>7)&0x0E);
	iic_wp_low();
	iic_start();
	iic_write8((writedecadd|temp));
	iic_ack();
	iic_write8((unsigned char)romadd);
	iic_ack();
	iic_start();
	iic_write8((readdecadd|temp));
	iic_ack();
	for(i=0;i<num;i++)
	{
		*iic_readdata = iic_read8();
		iic_ack();
		iic_readdata++;
	}
	iic_nack(1);
	iic_stop();
	iic_wp_high();
	
	return TRUE;
}
