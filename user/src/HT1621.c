
#define HT1621_LOCAL
#include "HT1621.h"
//#include "delay.h"
//#include "define.h"
#include "bsp.h"
#include "sys.h"

#define uchar unsigned char
#define uint unsigned int


#define BUZZER USED

#ifndef  BUZZER 
#define BUZZER NOUSED
#endif

#define Delay_uS(a) delay_us(a)

#define Delay_mS(a) delay_ms(a)

//extern uchar LCD_buffer[31];

uchar LCD_buffer[31];

//void GPIO_IN_OUT_SET(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,char in_out_sel)
//{
//	GPIOx->CRL = 
//}



 //-----------------------------------------------------------------------------------------
 //�������ƣ�Ht1621Wr_Data() 
 //�� �ܣ�д���ݺ���,cntΪ��������λ��,���ݴ���Ϊ��λ��ǰ 
//-----------------------------------------------------------------------------------------
void Ht1621Wr_Data(uchar Data,uchar cnt) 
 { 
 uchar i; 
 for (i=0;i<cnt;i++)
   { 
   WR_1621_L;   //HT1621_WR=0; 
   Delay_uS(10);
   if((Data & 0x80)==0x80)
     {
     DATA_1621_H; //HT1621_DAT=1;
     }
     else
     {
     DATA_1621_L; //HT1621_DAT=0;
     };
   Delay_uS(10);
   WR_1621_H;   //HT1621_WR=1; 
   Data<<=1;    
   Delay_uS(10);
   }; 
 }





//-----------------------------------------------------------------------------------------
//�������ƣ�void Ht1621WrOneData(uchar Addr,uchar Data) 
//��������: HT1621��ָ����ַд�����ݺ��� 
//����˵����AddrΪд���ʼ��ַ��DataΪд������ 
//˵ ������ΪHT1621������λ4λ������ʵ��д������Ϊ�����ĺ�4λ 
//-----------------------------------------------------------------------------------------
void Ht1621WrOneData(uchar Addr,uchar Data) 
 { 
 CS_1621_L;                   //HT1621_CS=0; 
 Ht1621Wr_Data(WRITE_MODE,3); //д�����ݱ�־101 
 Ht1621Wr_Data(Addr<<2,6);    //д���ַ���� 
 Ht1621Wr_Data(Data<<4,4);    //д������ 
 CS_1621_H;                   //HT1621_CS=1; 
 } 





//-----------------------------------------------------------------------------------------
//�������ƣ�void Ht1621WrAllData() 
//��������: HT1621����д�뷽ʽ���� 
//˵ ����HT1621������λ4λ���˴�ÿ������Ϊ8λ��д������������8λ���� 
//-----------------------------------------------------------------------------------------
void Ht1621WrAllData(unsigned char Dis_Status) 
 { 
 uchar i; 
 CS_1621_L;                //HT1621_CS=0; 
 Ht1621Wr_Data(0xA0,3);    //д�����ݱ�־101 
 Ht1621Wr_Data(0x00<<2,6); //д���ַ���� 
 for (i=0;i<16;i++) 
 { 
  Ht1621Wr_Data(Dis_Status,8);   //д������ 
  } 
  CS_1621_H;               //HT1621_CS=1; 
 } 





//-----------------------------------------------------------------------------------------
//�������ƣ�void Ht1621WrCmd(uchar Cmd) 
//��������: HT1621����д�뺯�� 
//����˵����CmdΪд���������� 
//˵ ����д�������ʶλ100 
//-----------------------------------------------------------------------------------------
 void Ht1621WrCmd(uchar Cmd) 
 { 
 CS_1621_L;                //HT1621_CS=0; 
 Delay_uS(200);
 Ht1621Wr_Data(COMMAND_MODE,4);    //д�������־100 
 Ht1621Wr_Data(Cmd,8);     //д���������� 
 CS_1621_H;                //HT1621_CS=1; 
 Delay_uS(200);
 } 



//-----------------------------------------------------------------------------------------
//�������ƣ�void Ht1621_Init(void) 
//��������: HT1621��ʼ�� 
//˵ ������ʼ����Һ���������ֶξ���ʾ 
//-----------------------------------------------------------------------------------------
void Ht1621_Init()
 {
 WR_1621_O;
 DATA_1621_O;
 CS_1621_O;
 CS_1621_H;                //HT1621_CS=1; 
 WR_1621_H;                //HT1621_WR=1; 
 DATA_1621_H;              //HT1621_DAT=1; 
 Delay_mS(1);            //��ʱʹLCD������ѹ�ȶ� 
 Ht1621WrCmd(BIAS);        
 Ht1621WrCmd(RC256);       //ʹ���ڲ�����  
 Ht1621WrCmd(SYSEN); 
 Ht1621WrCmd(LCDON);
 Ht1621WrCmd(TONEOFF);
 }
      
      
      




//**************************
//���ܣ�����д��1621
//��Σ�MemAddress:�ڴ�ӳ���ַ , *p:����ָ��, i:��������
//�زΣ���
//���ڣ�
//**************************
void S_Write_1621(uchar MemAddress,uchar *p,uchar i)
 { 
 uchar s; 
 CS_1621_L;                      //HT1621_CS=0; 
 Ht1621Wr_Data(WRITE_MODE,3);    //д�����ݱ�־101 
 Ht1621Wr_Data(MemAddress,6);    //д���ַ���� 
 for (s=0;s<i;s++) 
 { 
  Ht1621Wr_Data(*(p+s)<<4,4);    //д������  *(p+i) = p[i]
  } 
  CS_1621_H;                     //HT1621_CS=1; 
 } 



//****************************************
//���ܣ���LCD����ʾram������
//��Σ�Sec����
//  
//�زΣ���
//���ڣ�
//****************************************
void Refresh_LCD(void)
{

S_Write_1621(0,LCD_buffer,31);
}

void Clear_LCD(void)
{
  char i;
	for(i=0;i<31;i++)
	{
		LCD_buffer[i] = 0;
	}
}


void Buzzer(int mS)
{
#if BUZZER == USED
 Ht1621WrCmd(TONEON);
#endif
 Delay_mS(mS);
 Ht1621WrCmd(TONEOFF);
}




 void TEST( void )
 {
 uchar i=0x1d;
 Ht1621_Init(); //�ϵ��ʼ��LCD 
 Delay_uS(250);  //��ʱһ��ʱ�� 
 Ht1621WrAllData(0xff); 
 for(i=0;i<31;i++)
 {
 Ht1621WrOneData(i,0);//9
 };
 }

