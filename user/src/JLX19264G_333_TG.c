
#define	THE_LCD
//#define DISPLAY_LOCAL
#include "stm32f10x.h"
#include "iic.h"
#include "lcd.h"
#include "stdint.h"
#include "display.h"
// sbit key=P2^0;
// sbit cs1=P1^1;
// sbit reset=P1^0;
// sbit rs=P3^0;
// sbit sclk=P3^1;
// sbit sid=P3^2;


#define 	 SDA       GPIO_Pin_7
#define    SDAGPIO   GPIOB
#define    SCL       GPIO_Pin_6
#define    SCLGPIO   GPIOB
#define 	 CS        GPIO_Pin_3
#define    CSGPIO    GPIOB
#define 	 RS        GPIO_Pin_5
#define    RSGPIO    GPIOB
#define 	 RST        GPIO_Pin_4
#define    RSTGPIO    GPIOB


#define    iic_cs_low()    CSGPIO->BRR = CS//GPIO_ResetBits(WPGPIO,WP) 
#define    iic_cs_high()   CSGPIO->BSRR = CS//GPIO_SetBits(WPGPIO,WP)

#define    iic_rs_low()    RSGPIO->BRR = RS//GPIO_ResetBits(WPGPIO,WP) 
#define    iic_rs_high()   RSGPIO->BSRR = RS//GPIO_SetBits(WPGPIO,WP)

#define    iic_rst_low()    RSTGPIO->BRR = RST//GPIO_ResetBits(WPGPIO,WP) 
#define    iic_rst_high()   RSTGPIO->BSRR = RST//GPIO_SetBits(WPGPIO,WP)

#define    iic_sda_low()   SDAGPIO->BRR = SDA//GPIO_ResetBits(SDAGPIO,SDA) 
#define    iic_sda_high()  SDAGPIO->BSRR = SDA//GPIO_SetBits(SDAGPIO,SDA)

#define    iic_scl_low()   SCLGPIO->BRR = SCL//GPIO_ResetBits(SCLGPIO,SCL)
#define    iic_scl_high()  SCLGPIO->BSRR = SCL//GPIO_SetBits(SCLGPIO,SCL)  
#define    iic_sda_read()  ((SDAGPIO->IDR & SDA) != 0)?1:0//GPIO_ReadInputDataBit(SDAGPIO,SDA)

#define LCD_RST_EN            RSTGPIO->BRR  = RST  //  PB4
#define LCD_RST_DISABLE       RSTGPIO->BSRR = RST  //  PB4
#define LCD_RS_LOW            RSGPIO->BRR   = RS  //  PB5
#define LCD_RS_HIGH           RSGPIO->BSRR  = RS  //  PB5


#define IIC_DELAY_TIME 30
#define IIC_DELAY_TIME_END 3//结束条件要多5us
#define WAIT_ACK_TIME 8

// uchar const ascii_table_8x16[95][16];
// uchar const ascii_table_5x8[95][5];
//uchar const bmp1[];



void iic_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = SDA;
	GPIO_Init(SDAGPIO,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = SCL;
	GPIO_Init(SCLGPIO,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = CS;
	GPIO_Init(CSGPIO,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = RST;
	GPIO_Init(RSTGPIO,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = RS;
	GPIO_Init(RSGPIO,&GPIO_InitStruct);
	
	iic_cs_high();//cs1=1;
	iic_scl_high();//sclk=1;
	iic_rs_low();//rs=0;
}

// void iic_write8(unsigned char writedate)
// {
// 	int i;
// 	SDAGPIO->CRL &= 0x0FFFFFFF;
// 	SDAGPIO->CRL |= 0x30000000;	
// 	for(i=0;i<8;i++)
// 	{				
// 		if(0x80&writedate)
// 		{
// 				iic_sda_high();
// 		}	
//     else
// 		{
// 			  iic_sda_low();
// 		}			
// 		delay_us(IIC_DELAY_TIME);
// 		iic_scl_high();				
// 		delay_us(IIC_DELAY_TIME);
// 		iic_scl_low();
// 		writedate <<= 1;				
// 		//delay_us(IIC_DELAY_TIME);   		
// 	}
// 	//iic_scl_low();
// 	//delay_us(IIC_DELAY_TIME);
// }

	//写指令到LCD模块
	void transfer_command(int data1)
	{
		char i;
		iic_cs_low();//cs1=0;
		delay_us(IIC_DELAY_TIME);
		iic_rs_low();//rs=0;
		delay_us(IIC_DELAY_TIME);
		for(i=0;i<8;i++)
		{
			iic_scl_low();//sclk=0;
			if(data1&0x80) iic_sda_high();//sid=1;
			else iic_sda_low();//sid=0;
			
			delay_us(IIC_DELAY_TIME);
			iic_scl_high();//sclk=1;
			data1=data1<<=1;
			//if(i<8)
			delay_us(IIC_DELAY_TIME);
		}
		iic_cs_high();//cs1=1;
	}
	
	//写数据到LCD模块
void transfer_data(int data1)
{
	char i;
	iic_cs_low();//cs1=0;
	delay_us(IIC_DELAY_TIME);
	iic_rs_high();//rs=1;
	delay_us(IIC_DELAY_TIME);
	for(i=0;i<8;i++)
	{
		iic_scl_low();//sclk=0;
		if(data1&0x80) iic_sda_high();//sid=1;
		else iic_sda_low();//sid=0;
		
		delay_us(IIC_DELAY_TIME);
		iic_scl_high();//sclk=1;
		data1=data1<<=1;
		//if(i<8)
		delay_us(IIC_DELAY_TIME);
	}
	iic_cs_high();//cs1=1;
}

//延时
// void delay(int i)
// {
// 	int j,k;
// 	for(j=0;j<i;j++)
// 	for(k=0;k<110;k++);
// }

// void waitkey()
// {
// 	repeat:
// 	if(key==1)goto repeat;
// 	else delay(400);
// }

//LCD模块初始化
void initial_lcd(void)
{
	iic_rst_low();//reset=0; //低电平复位
	delay_us(1000);
	iic_rst_high();//reset=1; //复位完毕
	delay_us(1000);
	transfer_command(0xe2); //软复位
	delay_us(5000);
	transfer_command(0x2f); //打开内部升压
	delay_us(5000);
	transfer_command(0x81); //微调对比度
	transfer_command(0x45); //微调对比度的值，可设置范围：0x00~0xFF
	transfer_command(0xeb); //1/9 偏压比(bias)
	transfer_command(0xc4); //行扫描顺序：从上到下 oxc2
	transfer_command(0xa0); //列扫描顺序:从左到右
	transfer_command(0xaf); //开显示
}

void lcd_address(uchar page,uchar column)
{
	column=column-1; // 第一列1 在LCD驱动IC里是第0列
	page=page-1;
	transfer_command(0xb0+page); //设置页地址，每页是8行，一个画面的64行被分成8个页，第一页在lcd驱动IC里是第0页
	transfer_command(((column>>4)&0x0f)+0x10); //设置列地址的高4位
	transfer_command(column&0x0f); //设置列地址的低四位
}
//全屏清屏
void clear_screen(void)
{
	unsigned char i,j;
	for(i=0;i<9;i++)
	{
		lcd_address(1+i,1);
		for(j=0;j<192;j++)
		{
		   transfer_data(0x00);
		}
	}
}

void display_graphic_192x64(const uchar  *dp)
{
	int i,j;
	for(i=0;i<9;i++)
	{
		lcd_address(i+1,1);
		for(j=0;j<192;j++)
		{
			transfer_data(*dp);
			dp++;
		}
	}
}

// void display_graphic_33x64(const uchar  *dp)
// {
// 	int i,j;
// 	const uchar *p;
// 	p = dp+263;
// 	for(i=0;i<9;i++)
// 	{
// 		lcd_address(i+1,1);
// 		for(j=0;j<33;j++)
// 		{
// 			transfer_data(*dp);
// 			p--;
// 		}
// 	}
// }

void display_graphic_tree_32x64(const uchar  *dp)
{
	int i,j;
	for(i=0;i<9;i++)
	{
		lcd_address(i+2,1);
		for(j=0;j<32;j++)
		{
			transfer_data(*dp);
			dp++;
		}
	}
}
void display_graphic_tree_clear()
{
	int i,j;
	for(i=0;i<9;i++)
	{
		lcd_address(i+1,1);
		for(j=0;j<32;j++)
		{
			transfer_data(0);
		}
	}
}

void display_graphic_33x64(const uchar  *dp)
{
	int i,j;
	for(i=0;i<9;i++)
	{
		lcd_address(i+1,160);
		for(j=0;j<33;j++)
		{
			transfer_data(*dp);
			dp++;
		}
	}
}

void display_graphic_54x64_single_clear()
{
	int i,j;
	for(i=0;i<9;i++)
	{
		lcd_address(i+1,33);
		for(j=0;j<54;j++)
		{
			transfer_data(0);
		}
	}
}

void display_graphic_27x64_single(const uchar  *dp)
{
	int i,j;
	for(i=0;i<9;i++)
	{
		lcd_address(i+1,46);
		for(j=0;j<27;j++)
		{
			transfer_data(*dp);
			dp++;
		}
	}
}
void display_graphic_27x64_1(const uchar  *dp)
{
	int i,j;
	for(i=0;i<9;i++)
	{
		lcd_address(i+1,60);
		for(j=0;j<27;j++)
		{
			transfer_data(*dp);
			dp++;
		}
	}
}
void display_graphic_27x64_2(const uchar  *dp)
{
	int i,j;
	for(i=0;i<9;i++)
	{
		lcd_address(i+1,33);
		for(j=0;j<27;j++)
		{
			transfer_data(*dp);
			dp++;
		}
	}
}
void display_graphic_speed_33x64(const uchar  *dp)
{
	int i,j;
	for(i=0;i<9;i++)
	{
		lcd_address(i+1,88);
		for(j=0;j<33;j++)
		{
			transfer_data(*dp);
			dp++;
		}
	}
}

void display_graphic_win_dir_33x64(const uchar  *dp)
{
	int i,j;
	for(i=0;i<9;i++)
	{
		lcd_address(i+1,118);
		for(j=0;j<33;j++)
		{
			transfer_data(*dp);
			dp++;
		}
	}
}

void display_graphic_win_dir_39x56(const uchar  *dp)
{
	int i,j;
	for(i=0;i<8;i++)
	{
		lcd_address(i+2,118);
		for(j=0;j<39;j++)
		{
			transfer_data(*dp);
			dp++;
		}
	}
}

void display_graphic_win_dir_39x64(const uchar  *dp)
{
	int i,j;
	for(i=0;i<9;i++)
	{
		lcd_address(i+1,118);
		for(j=0;j<39;j++)
		{
			transfer_data(*dp);
			dp++;
		}
	}
}

void display_graphic_32x50(const uchar  *dp)
{
	int i,j;
	for(i=0;i<8;i++)
	{
		lcd_address(i+2,160);
		for(j=0;j<32;j++)
		{
			transfer_data(*dp);
			dp++;
		}
	}
}
//==================display a piture of 128*64 dots================
void full_display(uchar data_left,uchar data_right)
{
	int i,j;
	for(i=0;i<8;i++)
	{
		lcd_address(i+1,1);
		for(j=0;j<96;j++)
		{
			transfer_data(data_left);
			transfer_data(data_right);
		}
	}
}

//显示32X32点阵图像\汉字、生僻字或 32*32其他图像
void display_graphic_32x32(uchar page,uchar column,uchar *dp)
{
	uchar i,j;
	for(j=0;j<4;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<31;i++)
		{
			transfer_data(*dp); //写数据到LCD，每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}

//显示16X16点阵图像\汉字、生僻字或 16*16其他图像
void display_graphic_16x16(uchar page,uchar column,uchar *dp)
{
	uchar i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<16;i++)
		{
			transfer_data(*dp); //????LCD,?????8 ???????????1
			dp++;
		}
	}
}

////显示16X16点阵图像\ASCII,或 16*16其他图像
void display_graphic_8x16(uchar page,uchar column,uchar *dp)
{
	uchar i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<8;i++)
		{
			transfer_data(*dp); //????LCD,?????8 ???????????1
			dp++;
		}
	}
}

void display_string_8x16(uint page,uint column,uchar *text)
{
	uint i=0,j,col_conv;
	int k,n;
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			for(n=1;n>=0;n--)
			{
				col_conv = 186-column;
				lcd_address(page+n,col_conv);
				for(k=7;k>=0;k--)
				{
				transfer_data(ascii_table_8x16[j][k+8*n]);//??5x7?ASCII??LCD ?,y ??
				}
			}
			i++;
			column+=8;
		}
		else
		i++;
	}
}


void display_string_5x8(uint page,uint column,uchar reverse,uchar *text)
{
	uchar i=0,j,data1,col_conv;
	int k;
	while(text[i]>0x00)
	{
			if((text[i]>=0x20)&&(text[i]<=0x7e))
			{
					j=text[i]-0x20;
				  col_conv = 186 - column;
					lcd_address(page,col_conv);
					for(k=4;k>=0;k--)
					{
						if(reverse==1) data1=~ascii_table_5x8[j][k];
						else data1=ascii_table_5x8[j][k];
						transfer_data(data1);
					}
					if(reverse==1) transfer_data(0xff);
					else transfer_data(0x00);
					i++;
					column+=6;
			}
			else
			i++;
	}
}

void display_string_5x8_1(uint page,uint column,uchar *text)
{
	uint i=0,j;
	int k;
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<0x7e))
		{
			j=text[i]-0x20;
			lcd_address(page,column);
			for(k=5;k >=0;k--)
			{
				transfer_data(ascii_table_5x8[j][k]);//??5x7?ASCII??LCD?,y????,x ?
			}
			i++;
			column+=6;
		}
		else
		i++;
	}
}
