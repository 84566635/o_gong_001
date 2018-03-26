#ifndef IIC_H
#define IIC_H
#include"stm32f10x_conf.h"
#include"sys.h"
#include"stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "iic.h"
//#include "thecode_main.h"
//wp引脚为PA0在使用
#define bool char
#define TRUE 1
#define FALSE 0
#define 	 SDA       GPIO_Pin_7
#define    SDAGPIO   GPIOB
#define    SCL       GPIO_Pin_6
#define    SCLGPIO   GPIOB
#define 	 WP        GPIO_Pin_0
#define    WPGPIO    GPIOB
#define    iic_wp_low()    WPGPIO->BRR = WP//GPIO_ResetBits(WPGPIO,WP) 
#define    iic_wp_high()   WPGPIO->BSRR = WP//GPIO_SetBits(WPGPIO,WP)
#define    iic_sda_low()   SDAGPIO->BRR = SDA//GPIO_ResetBits(SDAGPIO,SDA) 
#define    iic_sda_high()  SDAGPIO->BSRR = SDA//GPIO_SetBits(SDAGPIO,SDA)
#define    iic_scl_low()   SCLGPIO->BRR = SCL//GPIO_ResetBits(SCLGPIO,SCL)
#define    iic_scl_high()  SCLGPIO->BSRR = SCL//GPIO_SetBits(SCLGPIO,SCL)  
#define    iic_sda_read()  ((SDAGPIO->IDR & SDA) != 0)?1:0//GPIO_ReadInputDataBit(SDAGPIO,SDA)
#define writedecadd 0xd0
#define readdecadd  0xd1

#ifdef MODEL_IIC
#define IIC_EXTERN_DEF
#else
#define IIC_EXTERN_DEF extern 
#endif

IIC_EXTERN_DEF bool EE_WriteBytes(unsigned short romadd,unsigned char *iic_writedata,int num);
IIC_EXTERN_DEF bool EE_ReadBytes(unsigned short romadd,unsigned char *iic_readdata,int num);
IIC_EXTERN_DEF void iic_init(void);
#endif
