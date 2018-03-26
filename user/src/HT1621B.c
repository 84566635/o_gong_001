

#define u8 unsigned char 

/*LED 字模结构*/
typedef struct
{
    char mChar;
    u8 mModal;
}LED_MODAL_DEFINE;

typedef struct
{
	u8 Addr;
	u8 Ddat;
} LED_ICON_DEFINE;
#define ht1621_BIAS		0x52
#define ht1621_SYSEN	0x02
#define ht1621_LCDOFF	0x04
#define ht1621_LCDON	0x06
#define ht1621_RC256	0X30
#define ht1621_SYSDIS	0X00
#define ht1621_WDTDIS	0X0A
#define ht1621_BLON		0x12
#define ht1621_BLOFF	0x10

#undef	LED_WIDTH
#define LED_WIDTH	6

#define PIN_HT1621_CS		ht_cs
#define PIN_HT1621_WR		ht_wr
#define PIN_HT1621_DAT	ht_data

#define HT1621_SET_CS(value) 	GPIOEX_BitWrite(PIN_HT1621_CS,value)
#define HT1621_SET_WR(value) 	GPIOEX_BitWrite(PIN_HT1621_WR,value)
#define HT1621_SET_DAT(value) GPIOEX_BitWrite(PIN_HT1621_DAT,value)

/*常用的LED字模定义,需要扩充的自己增加数组*/
const LED_MODAL_DEFINE LED_MODAL[]={
    {' ',0x00}  ,
    {'0',0xf5}  ,
    {'1',0x05}  ,
    {'2',0xb6}  ,
    {'3',0x97}  ,
    {'4',0x47}  ,
    {'5',0xd3}  ,
    {'6',0xf3}  ,
    {'7',0x85}  ,
    {'8',0xf7}  ,
    {'9',0xd7}  ,
    {'A',0xe7}  ,
    {'b',0x73}  ,
    {'C',0xf0}  ,
    {'E',0xf2}  ,
    {'F',0xe2}  ,
    {'H',0x67}  ,
    {'L',0x70}  ,
    {'n',0xe5}  ,
    {'U',0x75}  ,
    {'O',0xf5}  ,
    {'-',0x02}  ,
    {'I',0x60}  ,
    {'q',0xc7}  ,
    {'d',0x37}  ,
    {'S',0Xd3}  ,
    {'y',0x57}  ,
    {'t',0x72}	,
    {'r',0x22}  ,
		{'j',0x15}  ,
		{'P',0xe6}  ,
};

const LED_ICON_DEFINE ICON_DEFINE[] = 
{
	{0,0x0},
	{0,0x8},
	{2,0x8},
	{4,0x8},
	{6,0x8},
	{8,0x8},
	{14,0x8},
	{10,0x40},
	{10,0x20},
	{10,0x4},
	{10,0x2},
	{12,0x40},
	{12,0x2},
	{12,0x4},
	{10,0x80},
	{10,0x8},
	{12,0x20},
	{12,0x80},
	{12,0x8},
};

static char led_content[LED_WIDTH*2+1];         //显示内容 考虑小数点的问题,内容长度为LED宽度×2
static u8 led_len;	
u8 led_disp_mem[16];
u8 led_prev_disp_mem[16];
static _eCM led_prev_icon[2];
static char _l_prev_blink[6];
extern char _l_blink_state[6];

/*取字符对应的字模 
参数 ：iValue 需要取模的字符 区分大小写
*/
static u8 LED_GetModal(char iValue)
{
    u8 i;
    for (i=0;i<sizeof(LED_MODAL)/sizeof(LED_MODAL[0]);i++)
		{
        if (iValue == LED_MODAL[i].mChar) 
				{
            return LED_MODAL[i].mModal;
        }
    }
    return 0;
}

/*将内容写入显示设备
参数: bGlint  需要闪烁的部分是否显示
*/
void LED_DisplayToDevice()
{
	u8 i = 0;
	u8 pos = 0;
	u8 value = 0;
	u8 mode[LED_WIDTH];
	u8 dispMemAddr[] = {0,2,4,6,8,14};
	memset(mode,0,sizeof(mode));
	for (i=0;i<led_len;) 
	{
		//取字模
		value=LED_GetModal(led_content[i++]);
		//小数点和前面的字符结合成一个字符
		if (led_content[i]=='.') 
		{
			i++;
		}
//		if (_l_blink_state[i] == 0)
	//		value = LED_GetModal(' ');

		mode[pos++]=value;
		if (pos>LED_WIDTH-1)
			break;
	}

	//
	for (i=0; i<pos; i++)
	{
		if (_l_blink_state[i] == 0)
			led_disp_mem[dispMemAddr[i]] = 0;
		else
			led_disp_mem[dispMemAddr[i]] = mode[i];
	}
	//
}
void WrCmd(u8 Cmd)
{
	u8 data, i, CmdByte = 0x80;
	
	HT1621_SET_CS(0);
	for (i=0; i<4; i++)
	{
		if (CmdByte&0x80)
			HT1621_SET_DAT(1);
		else
			HT1621_SET_DAT(0);
		HT1621_SET_WR(0);
		DelayUS(4);
		HT1621_SET_WR(1);
		CmdByte <<= 1;
	}
	
	for (i=0; i<8; i++)
	{
		if (Cmd&0x80)
			HT1621_SET_DAT(1);
		else
			HT1621_SET_DAT(0);
		HT1621_SET_WR(0);
		DelayUS(4);
		HT1621_SET_WR(1);
		DelayUS(4);
		Cmd <<= 1;
	}
	
	HT1621_SET_CS(1);
}

void WrAddData(u8 Add, u8 Ddat)
{
	u8 i, CmdByte = 0xA0;
	
	//
	led_disp_mem[Add] |= Ddat;
	//

	HT1621_SET_CS(0);
	for (i=0; i<3; i++)
	{
		if (CmdByte&0x80)
			HT1621_SET_DAT(1);
		else
			HT1621_SET_DAT(0);
		HT1621_SET_WR(0);
		DelayUS(4);
		HT1621_SET_WR(1);
		CmdByte <<= 1;		
	}
	
	Add <<= 2;
	for (i=0; i<6; i++)
	{
		if (Add&0x80)
			HT1621_SET_DAT(1);
		else
			HT1621_SET_DAT(0);
		HT1621_SET_WR(0);
		DelayUS(4);
		HT1621_SET_WR(1);
		Add <<= 1;
	}
	
	for (i=0; i<8; i++)
	{
		if (Ddat&0x80)
			HT1621_SET_DAT(1);
		else
			HT1621_SET_DAT(0);
		HT1621_SET_WR(0);
		DelayUS(4);
		HT1621_SET_WR(1);
		Ddat <<= 1;
	}
	
	HT1621_SET_CS(1);
}

void HT1621Cls(void)
{
	u8 i;
	for (i=0; i<16; i+=2)
	{
		WrAddData(i, 0x00);
	}
	
	memset(led_disp_mem, 0, sizeof (led_disp_mem));
}

void HT1621AllOn(void)
{
	u8 i;

	for (i=0; i<16; i+=2)
	{
		WrAddData(i, 0xff);
	}
}

void HT1621_Init()
{
	_eCM cm[2] = {CM_bph,CM_gw};
	GPIOEX_InitIOPin(PIN_HT1621_CS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIOEX_InitIOPin(PIN_HT1621_WR, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIOEX_InitIOPin(PIN_HT1621_DAT, GPIO_MODE_OUT_PP_LOW_FAST);
	
	GPIOEX_InitIOPin(PIN_RUNLED, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIOEX_BitWrite(PIN_RUNLED,0);
	
	WrCmd(ht1621_BIAS);
	WrCmd(ht1621_RC256);
	WrCmd(ht1621_SYSEN);
	WrCmd(ht1621_LCDON);
	
	HT1621Cls();
	//
	HC164_Init();
	//
}

void HT1621_ReInit()
{
		GPIOEX_InitIOPin(PIN_HT1621_CS, GPIO_MODE_OUT_PP_LOW_FAST);
		GPIOEX_InitIOPin(PIN_HT1621_WR, GPIO_MODE_OUT_PP_LOW_FAST);
		GPIOEX_InitIOPin(PIN_HT1621_DAT, GPIO_MODE_OUT_PP_LOW_FAST);
		WrCmd(ht1621_BIAS);
		WrCmd(ht1621_RC256);
		WrCmd(ht1621_SYSEN);
		WrCmd(ht1621_LCDON);
}

void HT1621_BackLight(u8 nStat)
{
	if (nStat)
	{
		WrCmd(ht1621_BLON);
	}
	else
	{
		WrCmd(ht1621_BLOFF);
	}
}
