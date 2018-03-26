#define TM1628_H_LOCAL
#include "TM1628.h"
//#include "define.h"
//#include "DISPLAY.h"

extern unsigned char Key[5];//�洢����ֵ
unsigned char Band_Switch_Key[5];//�洢����ֵ
unsigned char TM1628_buf[18]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char TM1628_buf2[18]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x00,0x00,0x00,0x00,0x00,0x00};

extern void delay_us(int usneed);


extern struct SYS_EVEN Sys_Even ;

#define Delay_uS(a)  delay_us(a)
                                  // 0   1    2    3   4    5    6    7     8   9    A     B    C   D     E   F    R   
const unsigned char TAB_duan[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0xf9,0x3b,0x79,0x79,0x73,0x31,0xb9,0x39,0x23,0x19,0xe9,0x1b,0xf1,0x11,0x33,0xab,0x10};
 struct DIGI_SHOW SysDigiShow = {8,8,8,8,88,88,0};

 /***************************************************************/
/* ��ʾ���� 5λ�����Լӵ�7λ       */
/*ǰ4��������ܣ�����λ�ܹ���ָʾ��*/
/***************************************************************/
#
void TM1628_display()
{
    STB_H;        //Ƭѡ���ø���Ч
    TM1628_buf[0]&=0x80;
    TM1628_buf[2]&=0x80;
    TM1628_buf[6]&=0x80;
    TM1628_buf[12]&=0x80;
    TM1628_buf[0]  = TAB_duan[SysDigiShow.fist];
    TM1628_buf[2]  = TAB_duan[SysDigiShow.second];
    TM1628_buf[6]  = TAB_duan[SysDigiShow.third];
    TM1628_buf[12] = TAB_duan[SysDigiShow.fourth];
    Command(0x03);//��ʾģʽ
    Command(0x40);//��ַ�Զ���һ

    STB_L;           //Ƭѡ��0��Ч

    Send_8bit(0xc0);//��ַ0��ʼ
    Send_8bit(TM1628_buf[0]);//1
    Send_8bit(TM1628_buf[1]);
    Send_8bit(TM1628_buf[2]);//2
    Send_8bit(TM1628_buf[3]);
    Send_8bit(TM1628_buf[4]);//:
    Send_8bit(TM1628_buf[5]);
    Send_8bit(TM1628_buf[6]);//3
    STB_H;        //Ƭѡ���ø���Ч
    STB_L;           //Ƭѡ��0��Ч
    Send_8bit(0xCC);
    Send_8bit(TM1628_buf[12]);//4
    Send_8bit(TM1628_buf[13]);


   // Send_8bit(dat5);  //0x01-led1;0x02-led2;0x04-led3;0x08-led4
    //Send_8bit(0xff);  //led�������ܿ���


    STB_H;        //Ƭѡ���ø���Ч
   // Command(0x88);  //��ʾ��
}
  



void Flash_LED_DP_F(int status)
{
    Command(0x03);//��ʾģʽ
    Command(0x40);//��ַ�Զ���һ
    if(status==0)
    {
      TM1628_buf[4]&=0xfc;
    }
    else
    {
      TM1628_buf[4]|=0x03;
    }
    STB_L;           //Ƭѡ��0��Ч

    Send_8bit(0xc4);//��ַ0��ʼ
    Send_8bit(TM1628_buf[4]);//:
    STB_H;           //Ƭѡ��0��Ч
    //Send_8bit(TM1628_buf[4]);//:
}
   

//void Change_Digishow(int mini,int second)
//{
//  //SysDigiShow.fist    =  fisrt;
//  //SysDigiShow.second =  second;
//  //SysDigiShow.third   =  third;
//  //SysDigiShow.fourth  =  fourth;
//  
//  SysDigiShow.time_mini   = mini;
//  SysDigiShow.time_second = second;
//  SysDigiShow.fist    =  SysDigiShow.time_mini/10;
//  SysDigiShow.second  =  SysDigiShow.time_mini%10;
//  SysDigiShow.third   =  SysDigiShow.time_second/10;
//  SysDigiShow.fourth  =  SysDigiShow.time_second%10;
//  
//  Sys_Even.Digi_DisPlay_Change = 1;
//}
//��TM1628����8λ���ݣ��ӵ�λ��ʼ
void Send_8bit(unsigned char data)
{
 unsigned char i;
 for(i=0;i<8;i++)
     {	
        CLK_L;
        Delay_uS(10);
	   if(data&0x01)
	   {	
           DIO_H;
	   }
	   else
	   {
	    DIO_L;
	   }; 
	 Delay_uS(10);
	 CLK_H;
         Delay_uS(10);
         data>>=1;	
     };

}





//��TM1628��������
void Command(unsigned char com)
{
	STB_L;
	Send_8bit(com);
	if(com!=0x42)
	STB_H;
	Delay_uS(10);
}



//����add��ַdata�ĵ�
void Light_ON(unsigned char add,unsigned char Data)
{
        STB_O;
        CLK_O;
        DIO_O;
	TM1628_buf[add]|=Data;   

        //������������Ĭ�� HT1628_DisMode == ADDR_FIXED
        //�綨��ADDR_AUTOUP��ÿ��ִ�б�������1628��ȫ����ˢbuffer
        //�綨��ADDR_FIXED��ÿ��ִ�б�������1628ֻ����ˢһ��add��ַһ��
        
        #ifndef HT1628_DisMode
        #define HT1628_DisMode ADDR_FIXED
        #endif
        
        #if HT1628_DisMode == ADDR_AUTOUP
        Command(DISSEGCM710);    //������ʾģʽ  7λ10��ģʽ
	Command(ADDR_AUTOUP);    //��������  ��ַ�Զ���1
	STB_L;
	Send_8bit(DIS_ADDR00);
	Delay_uS(10);
	for(i=0;i<14;i++)
	{
	 Send_8bit(TM1628_buf[i]);
	}
        STB_H;
        #elif HT1628_DisMode == ADDR_FIXED
        Command(DISSEGCM710);    //������ʾģʽ  7λ10��ģʽ  
        Command(ADDR_FIXED);     //��������  �̶���ַ
	STB_L;
	Send_8bit(0xC0|add);     //HT1628��ַB7 B6�̶�Ϊ1
	Delay_uS(10);
	Send_8bit(TM1628_buf[add]);
        STB_H;
        #endif
	Delay_uS(10);
        Command(DIS_RIGH114|DIS_ON);//��������	
}



//Ϩ��add��ַdata�ĵ�
void Light_OFF(unsigned char add,unsigned char Data)
{
        STB_O;
        CLK_O;
        DIO_O;
	TM1628_buf[add]&=~Data;   

        //������������Ĭ�� HT1628_DisMode == ADDR_FIXED
        //�綨��ADDR_AUTOUP��ÿ��ִ�б�������1628��ȫ����ˢbuffer
        //�綨��ADDR_FIXED��ÿ��ִ�б�������1628ֻ����ˢһ��add��ַһ��
        
        #ifndef HT1628_DisMode
        #define HT1628_DisMode ADDR_FIXED
        #endif
        
        #if HT1628_DisMode == ADDR_AUTOUP
        Command(DISSEGCM710);    //������ʾģʽ  7λ10��ģʽ
	Command(ADDR_AUTOUP);    //��������  ��ַ�Զ���1
	STB_L;
	Send_8bit(DIS_ADDR00);
	Delay_uS(10);
	for(i=0;i<14;i++)
	{
	 Send_8bit(TM1628_buf[i]);
	}
        STB_H;
        #elif HT1628_DisMode == ADDR_FIXED
        Command(DISSEGCM710);    //������ʾģʽ  7λ10��ģʽ  
        Command(ADDR_FIXED);     //��������  �̶���ַ
	STB_L;
	Send_8bit(0xC0|add);     //HT1628��ַB7 B6�̶�Ϊ1
	Delay_uS(10);
	Send_8bit(TM1628_buf[add]);
        STB_H;
        #endif
        
	Delay_uS(5);
	Command(DIS_RIGH114|DIS_ON);//����//��ʾ����    ������10/16
	
}


//
void Init_TM1628(unsigned char LED_Status)
{	unsigned char i;
        STB_O;
        CLK_O;
        DIO_O;

	Command(DISSEGCM710); //������ʾģʽ 0x00 7λ10��ģʽ
	Command(ADDR_AUTOUP); //���������ַ�Զ���1
        STB_L;
	Send_8bit(DIS_ADDR00);//��ַ����	
	Delay_uS(20);
	for(i=0;i<14;i++)
	{ 
          Send_8bit(LED_Status);
          TM1628_buf[i]=LED_Status;
	};
	STB_H;
	Delay_uS(20);
	Command(DIS_RIGH114|DIS_ON);//����//��ʾ����
};


//��TM1628��������
void Command2(unsigned char com)
{
	STB2_L;
	Send_8bit(com);
	if(com!=0x42)
	STB2_H;
	Delay_uS(10);
}

//����add��ַdata�ĵ�
void Light2_ON(unsigned char add,unsigned char Data)
{
        STB2_O;
        CLK_O;
        DIO_O;
	TM1628_buf2[add]|=Data;   

        //������������Ĭ�� HT1628_DisMode == ADDR_FIXED
        //�綨��ADDR_AUTOUP��ÿ��ִ�б�������1628��ȫ����ˢbuffer
        //�綨��ADDR_FIXED��ÿ��ִ�б�������1628ֻ����ˢһ��add��ַһ��
        
        #ifndef HT1628_DisMode
        #define HT1628_DisMode ADDR_FIXED
        #endif
        
        #if HT1628_DisMode == ADDR_AUTOUP
        Command2(DISSEGCM710);    //������ʾģʽ  7λ10��ģʽ
	Command2(ADDR_AUTOUP);    //��������  ��ַ�Զ���1
	STB2_L;
	Send_8bit(DIS_ADDR00);
	Delay_uS(10);
	for(i=0;i<14;i++)
	{
	 Send_8bit(TM1628_buf2[i]);
	}
        STB2_H;
        #elif HT1628_DisMode == ADDR_FIXED
        Command2(DISSEGCM512);    //������ʾģʽ  7λ10��ģʽ  
        Command2(ADDR_FIXED);     //��������  �̶���ַ
	STB2_L;
	Send_8bit(0xC0|add);     //HT1628��ַB7 B6�̶�Ϊ1
	Delay_uS(10);
	Send_8bit(TM1628_buf2[add]);
        STB2_H;
        #endif
	Delay_uS(10);
        Command2(DIS_RIGH114|DIS_ON);//��������	
}


void Light2_OFF(unsigned char add,unsigned char Data)
{
        STB2_O;
        CLK_O;
        DIO_O;
	TM1628_buf2[add]&=~Data;   

        //������������Ĭ�� HT1628_DisMode == ADDR_FIXED
        //�綨��ADDR_AUTOUP��ÿ��ִ�б�������1628��ȫ����ˢbuffer
        //�綨��ADDR_FIXED��ÿ��ִ�б�������1628ֻ����ˢһ��add��ַһ��
        
        #ifndef HT1628_DisMode
        #define HT1628_DisMode ADDR_FIXED
        #endif
        
        #if HT1628_DisMode == ADDR_AUTOUP
        Command2(DISSEGCM710);    //������ʾģʽ  7λ10��ģʽ
	Command2(ADDR_AUTOUP);    //��������  ��ַ�Զ���1
	STB2_L;
	Send_8bit(DIS_ADDR00);
	Delay_uS(10);
	for(i=0;i<14;i++)
	{
	 Send_8bit(TM1628_buf2[i]);
	}
        STB2_H;
        #elif HT1628_DisMode == ADDR_FIXED
        Command2(DISSEGCM512);    //������ʾģʽ  7λ10��ģʽ  
        Command2(ADDR_FIXED);     //��������  �̶���ַ
	STB2_L;
	Send_8bit(0xC0|add);     //HT1628��ַB7 B6�̶�Ϊ1
	Delay_uS(10);
	Send_8bit(TM1628_buf2[add]);
        STB2_H;
        #endif
        
	Delay_uS(5);
	Command2(DIS_RIGH114|DIS_ON);//����//��ʾ����    ������10/16
	
}


//
void Init_Bot_TM1628(unsigned char LED_Status)
{	unsigned char i;
        STB2_O;
        CLK_O;
        DIO_O;

	Command2(DISSEGCM512); //������ʾģʽ 0x00 7λ10��ģʽ
	Command2(ADDR_AUTOUP); //���������ַ�Զ���1
        STB2_L;
	Send_8bit(DIS_ADDR00);//��ַ����	
	Delay_uS(20);
	for(i=0;i<14;i++)
	{ 
          Send_8bit(LED_Status);
          //TM1628_buf2[i]=LED_Status;
	};
	STB2_H;
	Delay_uS(20);
	Command2(DIS_RIGH114|DIS_ON);//����//��ʾ����
};

void TM1628_Read_Key()  
{ 
  unsigned char i,j;  
  STB2_O;
  CLK_O;
  DIO_O;
  Command2(0x42);  //����������  
  DIO_H;
  DIO_I;
  STB2_L;
  //DIO=1;      //��DIO�ø�   
  Delay_uS(10);
  for(j=0;j<5;j++) //������ȡ5���ֽ�   
  {
    for(i=0;i<8;i++)    // LED��������ר�õ�·   
      {        
        Band_Switch_Key[j]=Band_Switch_Key[j]>>1; 
        CLK_L;
        Delay_uS(10);
        CLK_H;    
        Delay_uS(10);
        if(DIO_R)          
          Band_Switch_Key[j]=Band_Switch_Key[j]|0X80;  
      }  
  }
  DIO_O;
  STB2_H; 
} 





