#include <reg52.h>
#include <intrins.h>
#include <Ctype.h>
sbit key=P2^0;
sbit cs1=P1^1;
sbit reset=P1^0;
sbit rs=P3^0;
sbit sclk=P3^1;
sbit sid=P3^2;
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
uchar code ascii_table_8x16[95][16];
uchar code ascii_table_5x8[95][5];
uchar code bmp1[];
uchar code cheng1[]={
//-- 文字: 成 --
//-- 宋体23; 此字体下对应的点阵为 ：宽 X 高 =31x31 --
//-- 高度不是8的倍数 现调整为:宽 X 高=32x32 --
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,
0xFC,0xFC,0x88,0x00,0x00,0x1C,0x78,0xF0,0xE0,0x00,0x80,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0xFF,0x83,0x83,0x83,0x83,0x83,0x83,0xC3,0xC3,0x03,0x1F,
0xFF,0xFF,0x83,0x03,0x03,0x03,0xC3,0xF3,0xF3,0x63,0x03,0x03,0x00,0x00,0x00,0x00,
0x00,0x00,0xFC,0xFF,0x3F,0x00,0x80,0x00,0x00,0x80,0xFF,0xFF,0x03,0x00,0x00,0x03,
0x9F,0xFF,0xF8,0xF8,0xBE,0x1F,0x07,0x01,0x00,0x00,0xE0,0x20,0x00,0x00,0x20,0x38,
0x1F,0x07,0x01,0x00,0x00,0x01,0x01,0x07,0x07,0x23,0x31,0x18,0x0C,0x0E,0x07,0x03,
0x01,0x01,0x01,0x03,0x07,0x0F,0x0E,0x1C,0x1F,0x3F,0x30,0x00,0x00,0x00,0x00,0x00};

uchar code zhuang1[]={
//--文字：状 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高 =16x16 --
0x08,0x30,0x00,0xFF,0x20,0x20,0x20,0x20,0xFF,0x20,0xE1,0x26,0x2C,0x20,0x20,0x00,
0x04,0x02,0x01,0xFF,0x40,0x20,0x18,0x07,0x00,0x00,0x03,0x0C,0x30,0x60,0x20,0x00};

uchar code tai1[]={
//--文字：态 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高 =16x16 --
0x00,0x04,0x04,0x04,0x84,0x44,0x34,0x4F,0x94,0x24,0x44,0x84,0x84,0x04,0x00,0x00,
0x00,0x60,0x39,0x01,0x00,0x3C,0x40,0x42,0x4C,0x40,0x40,0x70,0x04,0x09,0x31,0x00};

uchar code shi1[]={
//--文字：使 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高 =16x16 --
0x40,0x20,0xF0,0x1C,0x07,0xF2,0x94,0x94,0x94,0xFF,0x94,0x94,0x94,0xF4,0x04,0x00,
0x00,0x00,0x7F,0x00,0x40,0x41,0x22,0x14,0x0C,0x13,0x10,0x30,0x20,0x61,0x20,0x00};
uchar code yong1[]={
//--文字：用 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高 =16x16 --
0x00,0x00,0x00,0xFE,0x22,0x22,0x22,0x22,0xFE,0x22,0x22,0x22,0x22,0xFE,0x00,0x00,
0x80,0x40,0x30,0x0F,0x02,0x02,0x02,0x02,0xFF,0x02,0x02,0x42,0x82,0x7F,0x00,0x00};
uchar code mao_hao[]={
//-- 文字：(冒号) --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高=8x16 --
0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00};

char code num0[]={
//-- 文字：0 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高=8x16 --
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00
};
char code num1[]={
//-- 文字：1 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高=8x16 --
0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00
};
char code num2[]={
//-- 文字：2 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高=8x16 --
0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00
};
char code num3[]={
//-- 文字：3 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高=8x16 --
0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00
};
char code num4[]={
//-- 文字：4 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高=8x16 --
0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00
};
char code num5[]={
//-- 文字：5 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高=8x16 --
0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00
};
char code num6[]={
//-- 文字：6 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高=8x16 --
0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00
};
char code num7[]={
//-- 文字：7 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高=8x16 --
0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00
};
char code num8[]={
//-- 文字：8 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高=8x16 --
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00
};
char code num9[]={
//-- 文字：9 --
//-- 宋体12; 此字体下对应的点阵为 ：宽 X 高=8x16 --
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00
};

	//写指令到LCD模块
	void transfer_command(int data1)
	{
		char i;
		cs1=0;
		rs=0;
		for(i=0;i<8;i++)
		{
			sclk=0;
			if(data1&0x80) sid=1;
			else sid=0;
			sclk=1;
			data1=data1<<=1;
		}
		cs1=1;
	}
	
	//写数据到LCD模块
void transfer_data(int data1)
{
	char i;
	cs1=0;
	rs=1;
	for(i=0;i<8;i++)
	{
		sclk=0;
		if(data1&0x80) sid=1;
		else sid=0;
		sclk=1;
		data1=data1<<=1;
	}
	cs1=1;
}

//延时
void delay(int i)
{
	int j,k;
	for(j=0;j<i;j++)
	for(k=0;k<110;k++);
}

void waitkey()
{
	repeat:
	if(key==1)goto repeat;
	else delay(400);
}

//LCD模块初始化
void initial_lcd()
{
	reset=0; //低电平复位
	delay(100);
	reset=1; //复位完毕
	delay(100);
	transfer_command(0xe2); //软复位
	delay(500);
	transfer_command(0x2f); //打开内部升压
	delay(500);
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
void clear_screen()
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

void display_graphic_192x64(uchar *dp)
{
	uchar i,j;
	for(i=0;i<9;i++)
	{
		lcd_address(1+i,1);
		for(j=0;j<192;j++)
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
	uint i=0,j,k,n;
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			for(n=0;n<2;n++)
			{
				lcd_address(page+n,column);
				for(k=0;k<8;k++)
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
	uchar i=0,j,k,data1;
	while(text[i]>0x00)
	{
			if((text[i]>=0x20)&&(text[i]<=0x7e))
			{
					j=text[i]-0x20;
					lcd_address(page,column);
					for(k=0;k<5;k++)
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
	uint i=0,j,k;
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<0x7e))
		{
			j=text[i]-0x20;
			lcd_address(page,column);
			for(k=0;k<5;k++)
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