#include "main.h"


RTC_DateTypeDef GetDate;  //获取日期结构体
RTC_TimeTypeDef GetTime;   //获取时间结构体
u32 total=0;
u8	skipfalg=0;
u8 itempos=0;//光标位置
u8 pageflag=0;//页面标志
u8 buttonpage=0;//按钮页面
u8 displayflag=0;//页面刷新标志
u8 moveflag=0;//光标移动标志
struct DateTime SYSTIME={0};
//extern RTC_HandleTypeDef hrtc;
u8 calflag = 0;
uint8_t DispBuf[12];
u8 inputflag;
u8 bit_flag;
u8 dot_flag;
char inputbuf[10];
u8 powerondelay;
RTC_DateTypeDef DateBuf;  //获取日期结构体
RTC_TimeTypeDef TimeBuf;   //获取时间结构体

const char TESTDISP_BUTTON[2][5][6]={
	{"设置",
	"系统",
	"",
	"",
	"复位"},
	{"SETUP",
	"SYSTEM",
	"",
	"",
	"RST"}
};

const char SETUP_BUTTON[2][5][6]={
	{"测量",
	"系统",
	"",
	"",
	""},
	{"MEAS",
	"SYSTEM",
	"",
	"",
	""}
	
};

const char SYSSET_BUTTON[2][5][8]={
	{"测量",
	"设置",
	"校准",
	"",
	""},
	{"MEAS",
	"SETUP",
	"CAL  ",
	"",
	""}
	
};

const char CAL_BUTTON[2][5][8]={
	{"测量",
	"设置",
	"系统",
	"",
	"复位"},
	{"MEAS ",
	"SETUP ",
	"SYSTEM",
	"",
	"RST"}
	
};

const char DATE_BUTTON1[2][5][8]={
	{"年+",
	"年-",
	"月+",
	"月-",
	"NEXT"},
	{"YEAR+",
	"YEAR-",
	"MON+",
	"MON+",
	"NEXT"}
	
};

const char DATE_BUTTON2[2][5][8]={
	{"日+",
	"日-",
	"",
	"",
	"PREV"},
	{"DAY+",
	"DAY-",
	"",
	"",
	"PREV"}
	
};

const char TIME_BUTTON1[2][5][8]={
	{"时+",
	"时-",
	"分+",
	"分-",
	"NEXT"},
	{"HOUR+",
	"HOUR-",
	"MIN+",
	"MIN+",
	"NEXT"}
	
};

const char TIME_BUTTON2[2][5][8]={
	{"秒+",
	"秒-",
	"",
	"",
	"PREV"},
	{"SEC+",
	"SEC-",
	"",
	"",
	"PREV"}
	
};

const char TOP_CHAR[2][5][15]={
	
	{"<测量显示>",
	"<测量设置>",
	"<系统设置>",
	"<仪器校准>",
	""},
	{"<MEASURE>",
	"<SETUP>",
	"<SYSTEM SETUP>",
	"<CALIBTRATION>",
	""}
};

const char SETUP_ITEM[2][8][10]={
	{"型号",
	"单位",
	"速率",
	"讯响",
	"上限",
	"储存设置",
	"下限",
	"时间间隔"},
	{"MODEL",
	"UNIT",
	"RATE",
	"ALARM",
	"HIHG",
	"FAT SET",
	"LOW",
	"INTERVAL"}
	
};


const char SYS_ITEM[2][14][15]={
	{"型号",
	"热电偶型号",
	"仪器序列号",
	"硬件版本",
	"软件版本",
	"操作系统",
	"LANGUAGE",
	"亮度调节",
	"降低亮度",
	"自动关机",
	"日期",
	"时间",
	"文件",
	"触摸屏"},
	{"MODEL",
	"TC-MODEL",
	"SERIAL NO.",
	"HW VERSION",
	"SW VERSION",
	"OS",
	"LANGUAGE",
	"BRIGHTNESS",
	"DIM DISP",
	"APO",
	"DATE",
	"TIME",
	"FILE",
	"TOUCH"}
	
};

const char CAL_ITEM[][15]={
	"JKT_N50",
	"JKT_50",
	"PT100_50",
	"PT100_300",
};

const char CAL_DONE[][4]={
	"OK"
};

const char TCTYPE_ITEM[][5]={
	"TC-T",
	"TC-K",
	"TC-J",
	"TC-N",
	"NEXT",
	"TC-E",
	"TC-S",
	"TC-R",
	"TC-B",
	"PREV",
};

const char DISTYPE_ITEM[][5]={
	"TC-T",
	"TC-K",
	"TC-J",
	"TC-N",
	"TC-E",
	"TC-S",
	"TC-R",
	"TC-B",
};

const char UNIT_ITEM[][3]={
	"℃",
	"K ",
	"F ",
	"",
	"",
};

const char UNIT_DISP_ITEM[][6]={
	"(℃)",
	"(K) ",
	"(F) ",
	"",
	"",
};

const char RATE_ITEM[2][5][8]={
	{"慢速",
	"中速",
	"快速",
	"",
	""},
	{"SLOW  ",
	"MIDIUM",
	"FAST  ",
	"",
	""}
	
};

const char ALARM_ITEM[2][5][5]={
	{"打开",
	"关闭",
	"",
	"",
	""},
	{"ON ",
	"OFF",
	"",
	"",
	""}
	
};

const char INPUT_BUTTON[2][5][6]={
	{"输入",
	"",
	"",
	"",
	""},
	{"INPUT",
	"",
	"",
	"",
	""}
	
};

const char SAVE_ITEM[][5]={
	"USB",
	"SD ",
	"",
	"",
	"",

};

const char BUTTON_TIME[][5]={
	"+10",
	"-10",
	"+1",
	"-1",
	"",

};

const char SYSSET_ITEM[][20]={
	"Multi Temp Meter",
	"T,K,J,N,E,S,R,B",
	"REV A1.1",
	"REV A1.0",
	"REV V1.0 Build 2000",
};

const char LANGUAGE_ITEM[][10]={
	"中文",
	"ENGLISH",
	"",
	"",
	"",
};

const char BRTNESS_ITEM[][10]={
	"20% ",
	"40% ",
	"60% ",
	"80% ",
	"100%",
};

const char DIM_ITEM[2][5][10]={
	{
	"5分钟 ",
	"10分钟",
	"20分钟",
	"30分钟",
	"关闭  "},
	{
	"5min ",
	"10min",
	"20min",
	"30min",
	"OFF "},
};

const char FILE_ITEM[2][5][10]={
	{
	"自动保存",
	"忽略    ",
	"立即保存",
	"复位    ",
	""},
	{
	"AUTO  ",
	"IGNORE",
	"SAVE  ",
	"RESET ",
	""},
};

const char TOUCH_ITEM[2][5][10]={
	{
	"启用",
	"关闭",
	"校准",
	"复位",
	""},
	{
	"ENABLE ",
	"DISABLE",
	"CAL    ",
	"RESET  ",
	""},
};

const char SETCAL_ITEM[2][5][5]={
	{"校准",
	"",
	"",
	"",
	""},
	{"CAL",
	"",
	"",
	"",
	""}
	
};

const char INPUT_ITEM[2][5][6]={
	{"数值:",
	"删除",
	"确定",
	"取消",
	""},
	{"",
	"",
	"",
	"",
	""}
	
};

const char INPUT_EXT[][2]={
	"-",
	".",
	"0",
};

const char USEROFFSET_ITEM[][5]={
	"+1.0",
	"-1.0",
	"+0.1",
	"-0.1",
	"RST"
};

const char POWERON_ITEM[][32]={
	"JK808 MULTI CHANNEL TEMP.METER",
	"Initializing Channel 1...",
	"Initializing Channel 2...",
	"Initializing Channel 3...",
	"Initializing Channel 4...",
	"Initializing Channel 5...",
	"Initializing Channel 6...",
	"Initializing Channel 7...",
	"Initializing Channel 8...",
	"Done!"
};

void POWER_OFF(void)
{
	SaveSysPara(SYSPAR);//保存数据
	powerondelay=0;
	delay_ms(500);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);//关机
}

//==========================================================
//函数名称：Hex_Format
//函数功能：格式化整数值到DispBuf
//入口参数：整数，小数点，数值位数，显示零
//出口参数：无
//创建日期：2014.12.15
//修改日期：2015.01.14 13:12
//备注说明：dot=0-XXXXX XXXX.X XXX.XX XX.XXX X.XXXX-Dot=4
//注意事项：最多5位数
//==========================================================
void Hex_Format(int32_t dat , uint8_t Dot , uint8_t len , uint8_t dispzero)
{
	uint8_t i,j;
	u8 neg=0;
	
	if(dat<0)
		neg=1;
	if(len>6)len=10;//最大长度5
	if(len==0)len=1;
	if(Dot>5)Dot=5;//最大小数点4
	if(Dot>len)Dot=len;

	for(i=0;i<sizeof(DispBuf);i++)	//清空缓冲区
		DispBuf[i]=' ';

	if(neg==1)
	{
		DispBuf[0] = '-';
		for(i=len;i>1;i--)	//数制转换6  len
		{
			DispBuf[i-1] = abs(dat)%10 + '0';//转为ASCII
			dat/=10;
		}
	}else{
		for(i=len;i>0;i--)	//数制转换6  len
		{
			DispBuf[i-1] = dat%10 + '0';//转为ASCII
			dat/=10;
		}
	}
	
	if(Dot)//有小数点
	{
		j=len;
		i=Dot;//取小数点位置
		while(i--)
		{
			DispBuf[j]=DispBuf[j-1];
			j--;
		}
		DispBuf[j]='.';//加入小数点
	}
	else
	DispBuf[len]=' ';	

	if(dispzero==0)//不显示无效零
	{
		if(neg==1)
		{
			for(i=1;i<(len-1);i++)
			{
				if((DispBuf[i]=='0')&&(DispBuf[i+1]!='.'))
					DispBuf[i]=' ';
				else
					break;
			}
		}else{
			for(i=0;i<(len-1);i++)
			{
				if((DispBuf[i]=='0')&&(DispBuf[i+1]!='.'))
					DispBuf[i]=' ';
				else
					break;
			}
		}
	}
	//if(Dot)
	len++;
	for(i=len;i<sizeof(DispBuf);i++)//末尾补结束符'\0'
	{
		DispBuf[i]=0;
	}			
}

//触摸复位
void TOUCH_RESET(void)
{
	SYSPAR.xFactor=0.0922570005;
	SYSPAR.yFactor=0.0674763843;
	SYSPAR.xOffset=-24;
	SYSPAR.yOffset=-16;
}



void KEY_HANDLE(u8 key)
{
	BZZUER();
	switch(pageflag)
	{
		case PAGE_MEAS:
		{
			switch(key)
			{
				case KEY_OFF:
				{
					POWER_OFF();
				}break;
//				case KEY_MEAS:
//				{
//					pageflag = PAGE_MEAS;
//					displayflag = 1;
//				}break;
				case KEY_SETP:
				{
					pageflag = PAGE_SETP;
					displayflag = 1;
				}break;
				case KEY_SYST:
				{
					pageflag = PAGE_SYST;
					displayflag = 1;
				}break;
				case KEY_F1:
				{
					switch(itempos)
					{
						case 0:
						{
							pageflag = PAGE_SETP;
							displayflag = 1;
						}break;
						case 1:
						{
							SYSPAR.TempOffset[0]+=10;
						}break;
						case 2:
						{
							SYSPAR.TempOffset[1]+=10;
						}break;
						case 3:
						{
							SYSPAR.TempOffset[2]+=10;
						}break;
						case 4:
						{
							SYSPAR.TempOffset[3]+=10;
						}break;
						case 5:
						{
							SYSPAR.TempOffset[4]+=10;
						}break;
						case 6:
						{
							SYSPAR.TempOffset[5]+=10;
						}break;
						case 7:
						{
							SYSPAR.TempOffset[6]+=10;
						}break;
						case 8:
						{
							SYSPAR.TempOffset[7]+=10;
						}break;
						default:break;
					}
				}break;
				case KEY_F2:
				{
					switch(itempos)
					{
						case 0:
						{
							pageflag = PAGE_SYST;
							displayflag = 1;
						}break;
						case 1:
						{
							SYSPAR.TempOffset[0]-=10;
						}break;
						case 2:
						{
							SYSPAR.TempOffset[1]-=10;
						}break;
						case 3:
						{
							SYSPAR.TempOffset[2]-=10;
						}break;
						case 4:
						{
							SYSPAR.TempOffset[3]-=10;
						}break;
						case 5:
						{
							SYSPAR.TempOffset[4]-=10;
						}break;
						case 6:
						{
							SYSPAR.TempOffset[5]-=10;
						}break;
						case 7:
						{
							SYSPAR.TempOffset[6]-=10;
						}break;
						case 8:
						{
							SYSPAR.TempOffset[7]-=10;
						}break;
						default:break;
					}
				}break;
				case KEY_F3:
				{
					switch(itempos)
					{
						case 1:
						{
							SYSPAR.TempOffset[0]+=1;
						}break;
						case 2:
						{
							SYSPAR.TempOffset[1]+=1;
						}break;
						case 3:
						{
							SYSPAR.TempOffset[2]+=1;
						}break;
						case 4:
						{
							SYSPAR.TempOffset[3]+=1;
						}break;
						case 5:
						{
							SYSPAR.TempOffset[4]+=1;
						}break;
						case 6:
						{
							SYSPAR.TempOffset[5]+=1;
						}break;
						case 7:
						{
							SYSPAR.TempOffset[6]+=1;
						}break;
						case 8:
						{
							SYSPAR.TempOffset[7]+=1;
						}break;
						default:break;
					}
				}break;
				case KEY_F4:
				{
					switch(itempos)
					{
						case 1:
						{
							SYSPAR.TempOffset[0]-=1;
						}break;
						case 2:
						{
							SYSPAR.TempOffset[1]-=1;
						}break;
						case 3:
						{
							SYSPAR.TempOffset[2]-=1;
						}break;
						case 4:
						{
							SYSPAR.TempOffset[3]-=1;
						}break;
						case 5:
						{
							SYSPAR.TempOffset[4]-=1;
						}break;
						case 6:
						{
							SYSPAR.TempOffset[5]-=1;
						}break;
						case 7:
						{
							SYSPAR.TempOffset[6]-=1;
						}break;
						case 8:
						{
							SYSPAR.TempOffset[7]-=1;
						}break;
						default:break;
					}
				}break;
				case KEY_F5:
				{
					if(itempos == 0)
					{
						memset((void *)SYSPAR.TempOffset,0,sizeof(SYSPAR.TempOffset));
					}else{
						SYSPAR.TempOffset[itempos-1]=0;
					}
//					itempos=0;
//					moveflag = 1;
				}break;				
				case KEY_UP:
				{
					if(itempos>1)
					{
						itempos-=2;
					}else{
						itempos=0;
					}
					moveflag = 1;
				}break;	
				case KEY_DOWN:
				{
					if(itempos==0)
					{
						itempos++;
					}else{
						itempos+=2;
					}
					if(itempos>8)
					{
						itempos=8;
					}
					moveflag = 1;
				}break;
				case KEY_LEFT:
				{
					if(itempos>0)
					{
						itempos--;
					}
					moveflag = 1;
				}break;
				case KEY_RIGT:
				{
					if(itempos<8)
					{
						itempos++;
					}
					moveflag = 1;
				}break;
				case KEY_BRT:
				{
					if(SYSPAR.brtness < 4)
					{
						SYSPAR.brtness++;
					}else{
						SYSPAR.brtness=0;
					}
					//SaveSysPara(SYSPAR);
					Brightness();
				}break;
				case KEY_ACC:
				{
//					gdtest();
//					memset(&SYSPAR,0,sizeof(SYSPAR));
				}break;
				default:break;
			}
		}break;
		case PAGE_SETP:
		{
			switch(key)
			{
				case KEY_OFF:
				{
					POWER_OFF();
				}break;
				case KEY_MEAS:
				{
					pageflag = PAGE_MEAS;
					displayflag = 1;
				}break;
//				case KEY_SETP:
//				{
//					pageflag = PAGE_SETP;
//					displayflag = 1;
//				}break;
				case KEY_SYST:
				{
					pageflag = PAGE_SYST;
					displayflag = 1;
				}break;
				case KEY_F1:
				{
					switch(itempos)
					{
						case 0:
						{
							pageflag = PAGE_MEAS;
							displayflag = 1;
						}break;
						case 1:
						{
							SYSPAR.SensorType[0]=1+4*buttonpage;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.speed=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							DISP_INPUT();
						}break;
						case 4:
						{
							DISP_INPUT();
						}break;
						case 5:
						{
							SYSPAR.unit=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 6:
						{
							SYSPAR.alarm=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 7:
						{
							SYSPAR.saveset=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							SYSPAR.interval+=10;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						default:break;
					}
				}break;
				case KEY_F2:
				{
					switch(itempos)
					{
						case 0:
						{
							pageflag = PAGE_SYST;
							displayflag = 1;
						}break;
						case 1:
						{
							SYSPAR.SensorType[0]=2+4*buttonpage;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.speed=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							SYSPAR.unit=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 6:
						{
							SYSPAR.alarm=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 7:
						{
							SYSPAR.saveset=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							if(SYSPAR.interval<=10)
							{
								SYSPAR.interval=1;
							}else{
								SYSPAR.interval-=10;
							}
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						default:break;
					}
				}break;
				case KEY_F3:
				{
					switch(itempos)
					{
						case 1:
						{
							SYSPAR.SensorType[0]=3+4*buttonpage;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.speed=2;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							SYSPAR.unit=2;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							SYSPAR.interval+=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						default:break;
					}
				}break;
				case KEY_F4:
				{
					switch(itempos)
					{
						case 1:
						{
							SYSPAR.SensorType[0]=4+4*buttonpage;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							SYSPAR.interval-=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						default:break;
					}
				}break;
				case KEY_F5:
				{
					switch(itempos)
					{
						case 1:
						{
							buttonpage=!buttonpage;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						default:break;
					}
				}break;
				case KEY_UP:
				{
					if(itempos>0)
					{
						itempos--;
					}
					moveflag = 1;
				}break;	
				case KEY_DOWN:
				{
					if(itempos<8)
					{
						itempos++;
					}
					moveflag = 1;
				}break;
				case KEY_LEFT:
				{
					if(itempos>4)
					{
						itempos-=4;
					}
					moveflag = 1;
				}break;
				case KEY_RIGT:
				{
					if(itempos<5)
					{
						itempos+=4;
					}
					moveflag = 1;
				}break;
				case KEY_BRT:
				{
					if(SYSPAR.brtness < 4)
					{
						SYSPAR.brtness++;
					}else{
						SYSPAR.brtness=0;
					}
					//SaveSysPara(SYSPAR);
					Brightness();
				}break;

				default:break;
			}
			SYSPARCOMP();
		}break;
		case PAGE_SYST:
		{
			switch(key)
			{
				
				case KEY_OFF:
				{
					POWER_OFF();
				}break;
				case KEY_MEAS:
				{
					pageflag = PAGE_MEAS;
					displayflag = 1;
				}break;
				case KEY_SETP:
				{
					pageflag = PAGE_SETP;
					displayflag = 1;
				}break;
//				case KEY_SYST:
//				{
//					pageflag = PAGE_SYST;
//					displayflag = 1;
//				}break;
				case KEY_F1:
				{
					switch(itempos)
					{
						case 0:
						{
							pageflag = PAGE_MEAS;
							displayflag = 1;
						}break;
						case 1:
						{
							SYSPAR.language=0;
							displayflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.brtness=0;
							Brightness();
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							if(buttonpage == 0)
							{
//								DateBuf=GetDate;
								DateBuf.Year++;
								moveflag = 1;
							}else{
//								DateBuf=GetDate;
								DateBuf.Date++;
								moveflag = 1;
							}
							SaveTime();
						}break;
						case 6:
						{
							if(buttonpage == 0)
							{
//								TimeBuf=GetTime;
								TimeBuf.Hours++;
								if(TimeBuf.Hours>23)
									TimeBuf.Hours=0;
								moveflag = 1;
							}else{
//								TimeBuf=GetTime;
								TimeBuf.Seconds++;
								if(TimeBuf.Seconds>59)
									TimeBuf.Seconds=0;
								moveflag = 1;
							}
							SaveTime();
						}break;
						case 7:
						{
							SYSPAR.offsave=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							SYSPAR.touch=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						default:break;
					}
				}break;
				case KEY_F2:
				{
					switch(itempos)
					{
						case 0:
						{
							pageflag = PAGE_SETP;
							displayflag = 1;
						}break;
						case 1:
						{
							SYSPAR.language=1;
							displayflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.brtness=1;
							Brightness();
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							if(buttonpage == 0)
							{
//								DateBuf=GetDate;
								DateBuf.Year--;
								if(DateBuf.Year<22)
									DateBuf.Year=22;
								moveflag = 1;
							}else{
//								DateBuf=GetDate;
								DateBuf.Date--;
								if(DateBuf.Date<1)
									DateBuf.Date=1;
								moveflag = 1;
							}
							SaveTime();
						}break;
						case 6:
						{
							if(buttonpage == 0)
							{
//								TimeBuf=GetTime;
								TimeBuf.Hours--;
								if(TimeBuf.Hours>23)
									TimeBuf.Hours=0;
								moveflag = 1;
							}else{
//								TimeBuf=GetTime;
								TimeBuf.Seconds--;
								if(TimeBuf.Seconds>59)
									TimeBuf.Seconds=0;
								moveflag = 1;
							}
							SaveTime();
						}break;
						case 7:
						{
							SYSPAR.offsave=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							SYSPAR.touch=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						default:break;
					}
				}break;
				case KEY_F3:
				{
					switch(itempos)
					{
						case 0:
						{
							DISP_INPUT();
//							pageflag = PAGE_CAL;
//							displayflag = 1;
						}break;
//						case 1:
//						{
//							SYSPAR.language=1;
//							displayflag = 1;
//							//SaveSysPara(SYSPAR);
//						}break;
						case 2:
						{
							SYSPAR.brtness=2;
							Brightness();
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=2;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=2;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
//							DateBuf=GetDate;
							DateBuf.Month++;
							if(DateBuf.Month>12)
									DateBuf.Month=12;
							moveflag = 1;
							SaveTime();
						}break;
						case 6:
						{
//							TimeBuf=GetTime;
							TimeBuf.Minutes++;
							if(TimeBuf.Minutes>59)
									TimeBuf.Minutes=59;
							moveflag = 1;
							SaveTime();
						}break;
						case 7:
						{
							SYSPAR.offsave=2;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							TOUCH_Adjust();
						}break;
						default:break;
					}
				}break;
				case KEY_F4:
				{
					switch(itempos)
					{
						case 2:
						{
							SYSPAR.brtness=3;
							Brightness();
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=3;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=3;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
//							DateBuf=GetDate;
							DateBuf.Month--;
							if(DateBuf.Month<1)
									DateBuf.Month=0;
							moveflag = 1;
							SaveTime();
						}break;
						case 6:
						{
//							TimeBuf=GetTime;
							TimeBuf.Minutes--;
							if(TimeBuf.Minutes>59)
									TimeBuf.Minutes=0;
							moveflag = 1;
							SaveTime();
						}break;
						case 7:
						{
							SYSPAR.offsave=3;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							TOUCH_RESET();
						}break;
						default:break;
					}
				}break;
				case KEY_F5:
				{
					switch(itempos)
					{
						case 2:
						{
							SYSPAR.brtness=4;
							Brightness();
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=4;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=4;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							buttonpage=!buttonpage;
							moveflag = 1;
						}break;
						case 6:
						{
							buttonpage=!buttonpage;
							moveflag = 1;
						}break;
						
						default:break;
					}
				}break;
				case KEY_UP:
				{
					if(itempos>0)
					{
						itempos--;
					}
					moveflag = 1;
				}break;	
				case KEY_DOWN:
				{
					if(itempos<8)
					{
						itempos++;
					}
					moveflag = 1;
				}break;
				case KEY_LEFT:
				{
					if(itempos>4)
					{
						itempos-=4;
					}
					moveflag = 1;
				}break;
				case KEY_RIGT:
				{
					if(itempos<5)
					{
						itempos+=4;
					}
					moveflag = 1;
				}break;
				case KEY_BRT:
				{
					if(SYSPAR.brtness < 4)
					{
						SYSPAR.brtness++;
					}else{
						SYSPAR.brtness=0;
					}
					//SaveSysPara(SYSPAR);
					Brightness();
				}break;
				default:break;
			}
			SYSPARCOMP();
		}break;
		case PAGE_CAL:
		{
			switch(key)
			{
				case KEY_OFF:
				{
					POWER_OFF();
				}break;
				case KEY_MEAS:
				{
					pageflag = PAGE_MEAS;
					displayflag = 1;
				}break;
				case KEY_SETP:
				{
					pageflag = PAGE_SETP;
					displayflag = 1;
				}break;
				case KEY_ACC:
				{
					DISP_INPUT();
				}break;
				case KEY_F1:
				{
					switch(itempos)
					{
						case 0:
						{
							pageflag = PAGE_MEAS;
							displayflag = 1;
						}break;
						case 1:
						{
							Test.f_run=CAL_t;
							calflag=1;
						}break;
						case 2:
						{
							Test.f_run=CAL_t;
							calflag=2;
						}break;
						case 3:
						{
							Test.f_run=CAL_t;
							calflag=3;
						}break;
						case 4:
						{
							Test.f_run=CAL_t;
							calflag=4;
						}break;


						default:break;
					}
				}break;
				case KEY_F2:
				{
					switch(itempos)
					{
						case 0:
						{
							pageflag = PAGE_SETP;
							displayflag = 1;
						}break;


						default:break;
					}
				}break;
				case KEY_F3:
				{
					switch(itempos)
					{
						case 0:
						{
							pageflag = PAGE_SYST;
							displayflag = 1;
						}break;

						default:break;
					}
				}break;
				case KEY_F5:
				{
					switch(itempos)
					{
						case 0:
						{
							SYSPARRST();
						}break;

						default:break;
					}
				}break;
				case KEY_UP:
				{
					if(itempos>0)
					{
						itempos--;
					}
					moveflag = 1;
				}break;	
				case KEY_DOWN:
				{
					if(itempos<4)
					{
						itempos++;
					}
					moveflag = 1;
				}break;
				default:break;
			}
		}break;
		default:break;
	}
	KeyFlag=0;
}



void KEY_COLORBLOCK(u8 page)	  	   //按键显示 page-页面
{
  u8 i;
//  u16 color;
	for(i=0;i<5;i++)
	{		 
		LcdPrintHorz(1+(i*70),UP_LINE_OFFSET,67,DATACOLOR);		//横线
		LcdPrintVert(1+(i*70),UP_LINE_OFFSET,UP_LUM_OFFSET,DATACOLOR);	
		LcdFillRec(2+(i*70),UP_LINE_OFFSET+2,2+(i*70)+67-1-1,UP_LINE_OFFSET+UP_LUM_OFFSET-1,FILLBLOCK);
		switch(page)
		{
			case PAGE_MEAS:
			{
				switch(itempos)
				{
					case 0:
					{
						if(i<4)
							Lcd_Str16((u8 *)TESTDISP_BUTTON[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
						else
							Lcd_Str16((u8 *)TESTDISP_BUTTON[SYSPAR.language][i],1+(i*70)+17-15,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
					case 8:
					{
						if(i<4)
							Lcd_Str16((u8 *)USEROFFSET_ITEM[i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
						else
							Lcd_Str16((u8 *)USEROFFSET_ITEM[i],1+(i*70)+5,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}
					break;
					default:break;
				}
			}break;
			
			case PAGE_SETP:
			{
				switch(itempos)
				{
					case 0:
					{
						Lcd_Str16((u8 *)SETUP_BUTTON[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 1:
					{
						if(i == 4 && i == 9)
							Lcd_Str16((u8 *)TCTYPE_ITEM[i+buttonpage*5],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
						else
							Lcd_Str16((u8 *)TCTYPE_ITEM[i+buttonpage*5],1+(i*70)+17-15,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 2:
					{
						Lcd_Str16((u8 *)RATE_ITEM[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 3:
					{
						Lcd_Str16((u8 *)INPUT_BUTTON[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 4:
					{
						Lcd_Str16((u8 *)INPUT_BUTTON[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 5:
					{
						Lcd_Str16((u8 *)UNIT_ITEM[i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 6:
					{
						Lcd_Str16((u8 *)ALARM_ITEM[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 7:
					{
						Lcd_Str16((u8 *)SAVE_ITEM[i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 8:
					{
						Lcd_Str16((u8 *)BUTTON_TIME[i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					default:break;
				}
			}break;
			
			case PAGE_SYST:
			{
				switch(itempos)
				{
					case 0:
					{
						Lcd_Str16((u8 *)SYSSET_BUTTON[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 1:
					{
						Lcd_Str16((u8 *)LANGUAGE_ITEM[i],1+(i*70)+10,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 2:
					{
						Lcd_Str16((u8 *)BRTNESS_ITEM[i],1+(i*70)+2,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 3:
					{
						Lcd_Str16((u8 *)DIM_ITEM[SYSPAR.language][i],1+(i*70)+2,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 4:
					{
						Lcd_Str16((u8 *)DIM_ITEM[SYSPAR.language][i],1+(i*70)+2,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 5:
					{
						if(buttonpage == 0)
							Lcd_Str16((u8 *)DATE_BUTTON1[SYSPAR.language][i],1+(i*70)+2,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
						else
							Lcd_Str16((u8 *)DATE_BUTTON2[SYSPAR.language][i],1+(i*70)+2,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 6:
					{
						if(buttonpage == 0)
							Lcd_Str16((u8 *)TIME_BUTTON1[SYSPAR.language][i],1+(i*70)+2,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
						else
							Lcd_Str16((u8 *)TIME_BUTTON2[SYSPAR.language][i],1+(i*70)+2,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 7:
					{
						Lcd_Str16((u8 *)FILE_ITEM[SYSPAR.language][i],2+(i*70)+2,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 8:
					{
						Lcd_Str16((u8 *)TOUCH_ITEM[SYSPAR.language][i],17+(i*70)+2,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					default:break;
				}
			}break;
			case PAGE_CAL:
			{
				switch(itempos)
				{
					case 0:
					{
						if(i<4)
							Lcd_Str16((u8 *)CAL_BUTTON[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
						else
							Lcd_Str16((u8 *)CAL_BUTTON[SYSPAR.language][i],1+(i*70)+17-15,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 1:
					{
						Lcd_Str16((u8 *)SETCAL_ITEM[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 2:
					{
						Lcd_Str16((u8 *)SETCAL_ITEM[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 3:
					{
						Lcd_Str16((u8 *)SETCAL_ITEM[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					case 4:
					{
						Lcd_Str16((u8 *)SETCAL_ITEM[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
					}break;
					default:break;
				}
			}break;
			default:break;
		}
//		if(page == PAGE_MEAS)
//		{
//			
//		}else if(page == PAGE_SETP){
//			Lcd_Str16((u8 *)SETUP_BUTTON[i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
//		}else if(page == PAGE_SYST){
//			Lcd_Str16((u8 *)SYSSET_BUTTON[i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
//		}
	}	
}
static void DISP_TOP(u8 page)//显示页头
{
//	LcdFillRec(0,0,82,18,TOPCOLOR);
	Lcd_Str16((u8 *)TOP_CHAR[0][page-1],1,1,BUTTONCOLOR,TOPCOLOR);
}

static void DISP_CHNUM(void)//显示通道名称
{
	u8 i;
	char numchar[4];
	for(i=0;i<8;i++)
	{
		sprintf(numchar,"%02d:",i+1);
		if(i%2)
		{
			Lcd_Str16((u8 *)numchar,168,42+i/2*38,BABYYELLOW,BUTTONCOLOR);//2468
		}else{
			Lcd_Str16((u8 *)numchar,8,42+i/2*38,BABYYELLOW,BUTTONCOLOR);//1357
		}
	}
}
static void DRAW_BUTTON(u16 x,u16 y,u16 width,u16 height,u16 color)
{
//	LcdPrintHorz(1+(i*70),UP_LINE_OFFSET,67,DATACOLOR);		//横线
//		LcdPrintVert(1+(i*70),UP_LINE_OFFSET,UP_LUM_OFFSET,DATACOLOR);	
//		LcdFillRec(2+(i*70),UP_LINE_OFFSET+2,2+(i*70)+67-1-1,UP_LINE_OFFSET+UP_LUM_OFFSET-1,FILLBLOCK);
	
	
	LcdPrintHorz(x+1,y+1,width-1,DATACOLOR);		
		
	LcdPrintVert(x+1,y+1,height-1,DATACOLOR);	
	LcdPrintVert(x+2,y+1,height-1,DATACOLOR);	
	LcdPrintHorz(x,y,width,BUTTONCOLOR);		
	LcdPrintVert(x,y,height,BUTTONCOLOR);
	LcdPrintVert(x+width,y,height,BUTTONCOLOR);
	LcdPrintVert(x+width+1,y,height+1,BUTTONCOLOR);
	LcdPrintHorz(x,y+height,width+1,BUTTONCOLOR);
	LcdFillRec(x+3,y+2,x+width-1,y+height-1,color);
}

void DISP_INPUT(void)//显示数字输入框
{
	u8 i;
	char numchar[4];
	inputflag=1;
	bit_flag=1;
	LcdFillRec(0,130,320,240,DATACOLOR);
	
	
	Lcd_Str16((u8 *)INPUT_ITEM[SYSPAR.language][0],60,145,BUTTONCOLOR,DATACOLOR);
	LcdPrintRec(100,145-2,200,165-2,BUTTONCOLOR);
	
	DRAW_BUTTON(255,140,60,26,BLUE1);
	Lcd_Str16((u8 *)INPUT_ITEM[SYSPAR.language][1],280,145,DATACOLOR,BLUE1);
	DRAW_BUTTON(5,215-5,62,26,BLUE1);
	Lcd_Str16((u8 *)INPUT_ITEM[SYSPAR.language][2],20,220-5,DATACOLOR,BLUE1);
	DRAW_BUTTON(225,215-5,62,26,BLUE1);
	Lcd_Str16((u8 *)INPUT_ITEM[SYSPAR.language][3],240,220-5,DATACOLOR,BLUE1);
	
	
	LcdFillRec(7,170,313,170+32,BUTTONCOLOR);
	for(i=0;i<9;i++)
	{
		sprintf(numchar,"%d",i+1);
		DRAW_BUTTON(7+i*34,170,31,31,TCALCOLOR);
		Lcd_Str16((u8 *)numchar,7+12+i*34,170+7,DATACOLOR,TCALCOLOR);
	}
	for(i=0;i<3;i++)
	{
		DRAW_BUTTON(87+i*34,170+34,31,31,BLUE1);
		Lcd_Str16((u8 *)INPUT_EXT[i],87+12+i*34,170+34+7,DATACOLOR,BLUE1);
	}
	
}

void DISP_NUM(char num)
{
	if(bit_flag<10)
	{
		if(dot_flag != 0 && num == 0x2e)
		{
			
		}else{
			strcat(inputbuf,&num);
			Lcd_Str16((u8 *)&num,105+(bit_flag-1)*8,145,BUTTONCOLOR,DATACOLOR);
			if(dot_flag == 0 && num == 0x2e)
			{
				dot_flag = bit_flag;
			}
			bit_flag++;
		}
	}
}

void DEL_NUM(void)
{
	if(bit_flag > 1)
	{
		Lcd_Str16(" ",105+(bit_flag-2)*8,145,BUTTONCOLOR,DATACOLOR);
		bit_flag --;
		inputbuf[bit_flag-1] = '\0';
		if(bit_flag == dot_flag)
		{
			dot_flag = 0;
		}
	}
}

void INPUT_CONFIRM(__packed int16_t *data)
{
	u8 i;
	if(dot_flag==0)
	{
		*data=atoi(inputbuf)*10;
	}else{
		*data=atof(inputbuf)*10;
	}
	bit_flag=0;
	inputflag=0;
	dot_flag=0;
	memset(inputbuf,0,sizeof(inputbuf));
	LcdClear(BUTTONCOLOR);
	DISP_TOP(PAGE_SETP);
	
	for(i=0;i<8;i++)
	{
		if(i%2)
		{
			Lcd_Str16((u8 *)SETUP_ITEM[SYSPAR.language][i],168,30+i/2*22,FILLBLOCK,BUTTONCOLOR);//2468
		}else{
			Lcd_Str16((u8 *)SETUP_ITEM[SYSPAR.language][i],8,30+i/2*22,FILLBLOCK,BUTTONCOLOR);//1357
		}
	}
	KEY_COLORBLOCK(pageflag);
	DISP_PAGE_ITEM(pageflag,itempos);
	//SaveSysPara(SYSPAR);
}

void SN_CONFIRM(void)
{
//	u8 i;
	
	bit_flag=0;
	inputflag=0;
	dot_flag=0;
	strcpy((char *)SYSPAR.sn,inputbuf);
	memset(inputbuf,0,sizeof(inputbuf));
	LcdClear(BUTTONCOLOR);
	DISP_TOP(PAGE_CAL);
	KEY_COLORBLOCK(pageflag);
	DISP_PAGE_ITEM(pageflag,itempos);
	SaveSysPara(SYSPAR);
}

void INPUT_CANCEL(void)
{
	u8 i;
	bit_flag=0;
	inputflag=0;
	dot_flag=0;
	memset(inputbuf,0,sizeof(inputbuf));
	LcdClear(BUTTONCOLOR);
	if(pageflag == PAGE_SETP)
	{
		DISP_TOP(PAGE_SETP);
		for(i=0;i<8;i++)
		{
			if(i%2)
			{
				Lcd_Str16((u8 *)SETUP_ITEM[SYSPAR.language][i],168,30+i/2*22,FILLBLOCK,BUTTONCOLOR);//2468
			}else{
				Lcd_Str16((u8 *)SETUP_ITEM[SYSPAR.language][i],8,30+i/2*22,FILLBLOCK,BUTTONCOLOR);//1357
			}
		}
		KEY_COLORBLOCK(pageflag);
		DISP_PAGE_ITEM(pageflag,itempos);
	}else if(pageflag == PAGE_SYST){
		DISP_TOP(PAGE_SYST);
	
		for(i=0;i<10;i++)
		{
			Lcd_Str16((u8 *)SYS_ITEM[SYSPAR.language][i],8,22+i*18,FILLBLOCK,BUTTONCOLOR);
		}
		for(i=0;i<4;i++)
		{
			Lcd_Str16((u8 *)SYS_ITEM[SYSPAR.language][i+10],168,22+(6+i)*18,FILLBLOCK,BUTTONCOLOR);
		}
		for(i=0;i<5;i++)
		{
			if(i<2)
				Lcd_Str16((u8 *)SYSSET_ITEM[i],144,22+i*18,DATACOLOR,BUTTONCOLOR);
			else
				Lcd_Str16((u8 *)SYSSET_ITEM[i],144,22+(i+1)*18,DATACOLOR,BUTTONCOLOR);
		}
		DISP_PAGE_ITEM(pageflag,itempos);
	}
}

void DISP_TEMP(void)//显示温度
{
	u8 i;
	u8 compflag=0;
	for(i=0;i<8;i++)
	{
		if(i%2)
		{
			if(CurrentTemp[i] == 0x7fff)
			{
				if(i+1==itempos)
					LcdPrintStr24("------",168+40,42+i/2*38,BUTTONCOLOR,DATACOLOR);//2468);
				else
					LcdPrintStr24("------",168+40,42+i/2*38,DATACOLOR,BUTTONCOLOR);//2468);
			}else{
				DispTemp[i]=CurrentTemp[i]+SYSPAR.TempOffset[i];
				Hex_Format(DispTemp[i],1,5,0);
				if(DispTemp[i] > SYSPAR.upper || DispTemp[i] < SYSPAR.lower)
				{
					compflag=1;
					if(i+1==itempos)
					{
						LcdPrintStr24((u8 *)DispBuf,168+40,42+i/2*38,BUTTONCOLOR,TCALCOLOR);//2468);
					}
					else{
						LcdPrintStr24((u8 *)DispBuf,168+40,42+i/2*38,TCALCOLOR,BUTTONCOLOR);//2468);
						
					}
				}else{
					if(i+1==itempos)
					{
						LcdPrintStr24((u8 *)DispBuf,168+40,42+i/2*38,BUTTONCOLOR,TOPCOLOR);//2468);
					}
					else{
						LcdPrintStr24((u8 *)DispBuf,168+40,42+i/2*38,TOPCOLOR,BUTTONCOLOR);//2468);
						
					}
				}
			}
		}else{
			if(CurrentTemp[i] == 0x7fff)
			{
				if(i+1==itempos)
					LcdPrintStr24("------",8+40,42+i/2*38,BUTTONCOLOR,DATACOLOR);//1357);
				else
					LcdPrintStr24("------",8+40,42+i/2*38,DATACOLOR,BUTTONCOLOR);//1357);
			}else{
				DispTemp[i]=CurrentTemp[i]+SYSPAR.TempOffset[i];
				Hex_Format(DispTemp[i],1,5,0);
				if(DispTemp[i] > SYSPAR.upper || DispTemp[i] < SYSPAR.lower)
				{
					compflag=1;
					if(i+1==itempos)
					{
						LcdPrintStr24((u8 *)DispBuf,8+40,42+i/2*38,BUTTONCOLOR,TCALCOLOR);//1357);
					}
					else{
						LcdPrintStr24((u8 *)DispBuf,8+40,42+i/2*38,TCALCOLOR,BUTTONCOLOR);//1357);
					}
				}else{
					if(i+1==itempos)
					{
						LcdPrintStr24((u8 *)DispBuf,8+40,42+i/2*38,BUTTONCOLOR,TOPCOLOR);//1357);
					}
					else{
						LcdPrintStr24((u8 *)DispBuf,8+40,42+i/2*38,TOPCOLOR,BUTTONCOLOR);//1357);
					}
				}
				
			}
		}
	}
	if(compflag==1 && SYSPAR.alarm == 0)
	{
		BuzzerOn();
	}else{
		BuzzerOff();
	}
}

void DISP_ENVIROMENT_TEMP(void)//显示环境温度
{
	tmp0();
	Hex_Format(enir_temp.adx,1,5,0);
	Lcd_Str16((u8 *)DispBuf,100,2,TOPCOLOR,BUTTONCOLOR);
}
	
static void DISP_TCTYPE(void)
{
	Lcd_Str16((u8 *)DISTYPE_ITEM[SYSPAR.SensorType[0]-1],110-10-5,1,DATACOLOR,BUTTONCOLOR);
}

static void DISP_UNIT(void)
{
	Lcd_Str16("(",110+32-10-5,3,DATACOLOR,BUTTONCOLOR);
	Lcd_Str16(")",110+32-10+32-5,3,DATACOLOR,BUTTONCOLOR);
	Lcd_Str16((u8 *)UNIT_ITEM[SYSPAR.unit],110+32-10+16-5,1,DATACOLOR,BUTTONCOLOR);
}


void DISP_TEST(void)//测量显示页面固定显示
{
	u8 i;
	itempos=0;
	LcdClear(BUTTONCOLOR);
	DISP_TOP(PAGE_MEAS);
	DISP_TCTYPE();
	DISP_UNIT();
	DISP_CHNUM();
	for(i=0;i<5;i++)
	{	
		LcdPrintHorz(1,30+38*i,319,DATACOLOR);		//横线
		
	}
	for(i=0;i<3;i++)
	{
		LcdPrintVert(1+160*i,30,152,DATACOLOR);		//竖线
	}
}


	
void DISP_SETUP(void)//测量设置页面固定显示
{
	u8 i;
	itempos=0;
	LcdClear(BUTTONCOLOR);
	DISP_TOP(PAGE_SETP);
	
	for(i=0;i<8;i++)
	{
		if(i%2)
		{
			Lcd_Str16((u8 *)SETUP_ITEM[SYSPAR.language][i],168,30+i/2*22,FILLBLOCK,BUTTONCOLOR);//2468
		}else{
			Lcd_Str16((u8 *)SETUP_ITEM[SYSPAR.language][i],8,30+i/2*22,FILLBLOCK,BUTTONCOLOR);//1357
		}
	}

}

void DISP_SYS(void)//系统设置页面固定显示
{
	u8 i;
	itempos=0;
	LcdClear(BUTTONCOLOR);
	DISP_TOP(PAGE_SYST);
	
	for(i=0;i<10;i++)
	{
		Lcd_Str16((u8 *)SYS_ITEM[SYSPAR.language][i],8,22+i*18,FILLBLOCK,BUTTONCOLOR);
	}
	for(i=0;i<4;i++)
	{
		Lcd_Str16((u8 *)SYS_ITEM[SYSPAR.language][i+10],168,22+(6+i)*18,FILLBLOCK,BUTTONCOLOR);
	}
	for(i=0;i<5;i++)
	{
		if(i<2)
			Lcd_Str16((u8 *)SYSSET_ITEM[i],144,22+i*18,DATACOLOR,BUTTONCOLOR);
		else
			Lcd_Str16((u8 *)SYSSET_ITEM[i],144,22+(i+1)*18,DATACOLOR,BUTTONCOLOR);
	}
	Lcd_Str16((u8 *)SYSPAR.sn,144,22+2*18,DATACOLOR,BUTTONCOLOR);
	TimeBuf=GetTime;
	DateBuf=GetDate;
}

void DISP_CAL(void)//校准页面固定显示
{
	itempos=0;
	LcdClear(BUTTONCOLOR);
	DISP_TOP(PAGE_CAL);

}

//显示页面可选项
void DISP_PAGE_ITEM(u8 page,u8 pos)
{
//	u8 i;
	char itembuf[10];
	switch(page)
	{
		case PAGE_MEAS:
		{
			
			
			
//				for(i=0;i<8;i++)
//				{
//					if(i%2)
//					{
//						Lcd_Str16((u8 *)SETUP_ITEM[i],168,30+i/2*22,FILLBLOCK,BUTTONCOLOR);//2468
//					}else{
//						Lcd_Str16((u8 *)SETUP_ITEM[i],8,30+i/2*22,FILLBLOCK,BUTTONCOLOR);//1357
//					}
//				}

		}break;
		case PAGE_SETP:
		{
			if(itempos == 1)//型号
			{

				Lcd_Str16((u8 *)DISTYPE_ITEM[SYSPAR.SensorType[0]-1],SETCOL1,SETROW1,BUTTONCOLOR,BABYYELLOW);
			}
			else{
				Lcd_Str16((u8 *)DISTYPE_ITEM[SYSPAR.SensorType[0]-1],SETCOL1,SETROW1,BABYYELLOW,BUTTONCOLOR);
			}
			
			if(itempos == 2)//速率
				Lcd_Str16((u8 *)RATE_ITEM[SYSPAR.language][SYSPAR.speed],SETCOL1,SETROW1+SETROWOFFSET,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)RATE_ITEM[SYSPAR.language][SYSPAR.speed],SETCOL1,SETROW1+SETROWOFFSET,BABYYELLOW,BUTTONCOLOR);
			
			Hex_Format(SYSPAR.upper,1,5,0);
			if(itempos == 3)//上限
				Lcd_Str16((u8 *)DispBuf,SETCOL1,SETROW1+SETROWOFFSET*2,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)DispBuf,SETCOL1,SETROW1+SETROWOFFSET*2,BABYYELLOW,BUTTONCOLOR);
			
			Hex_Format(SYSPAR.lower,1,5,0);
			if(itempos == 4)//下限
				Lcd_Str16((u8 *)DispBuf,SETCOL1,SETROW1+SETROWOFFSET*3,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)DispBuf,SETCOL1,SETROW1+SETROWOFFSET*3,BABYYELLOW,BUTTONCOLOR);
			
			if(itempos == 5)//单位
				Lcd_Str16((u8 *)UNIT_ITEM[SYSPAR.unit],SETCOL2,SETROW1,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)UNIT_ITEM[SYSPAR.unit],SETCOL2,SETROW1,BABYYELLOW,BUTTONCOLOR);
			
			if(itempos == 6)//讯响
				Lcd_Str16((u8 *)ALARM_ITEM[SYSPAR.language][SYSPAR.alarm],SETCOL2,SETROW1+SETROWOFFSET,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)ALARM_ITEM[SYSPAR.language][SYSPAR.alarm],SETCOL2,SETROW1+SETROWOFFSET,BABYYELLOW,BUTTONCOLOR);
			
			if(itempos == 7)//储存设置
				Lcd_Str16((u8 *)SAVE_ITEM[SYSPAR.saveset],SETCOL2,SETROW1+SETROWOFFSET*2,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)SAVE_ITEM[SYSPAR.saveset],SETCOL2,SETROW1+SETROWOFFSET*2,BABYYELLOW,BUTTONCOLOR);
			
			sprintf(itembuf,"%04d",SYSPAR.interval);
			if(itempos == 8)//时间间隔
				Lcd_Str16((u8 *)itembuf,SETCOL2,SETROW1+SETROWOFFSET*3,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)itembuf,SETCOL2,SETROW1+SETROWOFFSET*3,BABYYELLOW,BUTTONCOLOR);
		}break;
		case PAGE_SYST:
		{
			if(itempos == 1)//语言
				Lcd_Str16((u8 *)LANGUAGE_ITEM[SYSPAR.language],SETCOL1,SYSROW1+SYSROWOFFSET*6,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)LANGUAGE_ITEM[SYSPAR.language],SETCOL1,SYSROW1+SYSROWOFFSET*6,BABYYELLOW,BUTTONCOLOR);
			
			if(itempos == 2)//亮度调节
				Lcd_Str16((u8 *)BRTNESS_ITEM[SYSPAR.brtness],SETCOL1,SYSROW1+SYSROWOFFSET*7,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)BRTNESS_ITEM[SYSPAR.brtness],SETCOL1,SYSROW1+SYSROWOFFSET*7,BABYYELLOW,BUTTONCOLOR);
			
			if(itempos == 3)//降低亮度
				Lcd_Str16((u8 *)DIM_ITEM[SYSPAR.language][SYSPAR.dimtime],SETCOL1,SYSROW1+SYSROWOFFSET*8,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)DIM_ITEM[SYSPAR.language][SYSPAR.dimtime],SETCOL1,SYSROW1+SYSROWOFFSET*8,BABYYELLOW,BUTTONCOLOR);
			
			if(itempos == 4)//自动关机
				Lcd_Str16((u8 *)DIM_ITEM[SYSPAR.language][SYSPAR.autooff],SETCOL1,SYSROW1+SYSROWOFFSET*9,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)DIM_ITEM[SYSPAR.language][SYSPAR.autooff],SETCOL1,SYSROW1+SYSROWOFFSET*9,BABYYELLOW,BUTTONCOLOR);
			
			if(itempos == 7)//文件
				Lcd_Str16((u8 *)FILE_ITEM[SYSPAR.language][SYSPAR.offsave],SETCOL2,SYSROW1+SYSROWOFFSET*8,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)FILE_ITEM[SYSPAR.language][SYSPAR.offsave],SETCOL2,SYSROW1+SYSROWOFFSET*8,BABYYELLOW,BUTTONCOLOR);
			
			if(itempos == 8)//触摸屏
				Lcd_Str16((u8 *)TOUCH_ITEM[SYSPAR.language][SYSPAR.touch],SETCOL2,SYSROW1+SYSROWOFFSET*9,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)TOUCH_ITEM[SYSPAR.language][SYSPAR.touch],SETCOL2,SYSROW1+SYSROWOFFSET*9,BABYYELLOW,BUTTONCOLOR);
		}break;
		case PAGE_CAL:
		{
			if(itempos == 1)//-50mV
			{
				Lcd_Str16((u8 *)CAL_ITEM[0],SETCOL1,SETROW1,BUTTONCOLOR,BABYYELLOW);
			}
			else{
				Lcd_Str16((u8 *)CAL_ITEM[0],SETCOL1,SETROW1,BABYYELLOW,BUTTONCOLOR);
			}
			
			if(itempos == 2)//50mV
				Lcd_Str16((u8 *)CAL_ITEM[1],SETCOL1,SETROW1+SETROWOFFSET,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)CAL_ITEM[1],SETCOL1,SETROW1+SETROWOFFSET,BABYYELLOW,BUTTONCOLOR);
			
			if(itempos == 3)//50o
				Lcd_Str16((u8 *)CAL_ITEM[2],SETCOL1,SETROW1+SETROWOFFSET*2,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)CAL_ITEM[2],SETCOL1,SETROW1+SETROWOFFSET*2,BABYYELLOW,BUTTONCOLOR);
			
			if(itempos == 4)//300o
				Lcd_Str16((u8 *)CAL_ITEM[3],SETCOL1,SETROW1+SETROWOFFSET*3,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)CAL_ITEM[3],SETCOL1,SETROW1+SETROWOFFSET*3,BABYYELLOW,BUTTONCOLOR);
		}break;
		default:break;
	}
	KEY_COLORBLOCK(page);
}

void DISP_CAL_PROCESS(u8 step)
{
	Lcd_Str16((u8 *)CAL_DONE[0],SETCOL1+60,SETROW1+(step-1)*SETROWOFFSET,TCALCOLOR,BUTTONCOLOR);
}

void DISP_PAGE(void)
{
	bit_flag=0;
	inputflag=0;
	dot_flag=0;
	switch(pageflag)
	{
		case PAGE_MEAS:
		{
			DISP_TEST();
		}break;
		case PAGE_SETP:
		{
			DISP_SETUP();
		}break;
		case PAGE_SYST:
		{
			DISP_SYS();
		}break;
		case PAGE_CAL:
		{
			DISP_CAL();
		}break;
		default:break;
	}
	KEY_COLORBLOCK(pageflag);
	DISP_PAGE_ITEM(pageflag,itempos);
}

void DISP_TIME(void)//显示时间
{
	char timebuf[10];
	char datebuf[10];
////	/* Get the RTC current Time */
//	HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
//		/* Get the RTC current Date */
//	HAL_RTC_GetDate(&hrtc, &GetDate, RTC_FORMAT_BIN);
	
	switch(pageflag)
	{
		
		case PAGE_MEAS:
		{
			RTC_Get_DateTimeCounter(&GetDate,&GetTime);
			sprintf(timebuf,"%02d:%02d:%02d",GetTime.Hours,GetTime.Minutes,
			GetTime.Seconds);
			sprintf(datebuf,"20%02d-%02d-%02d",GetDate.Year,GetDate.Month,
			GetDate.Date);
			Lcd_Str16((u8 *)timebuf,170,1,BABYYELLOW,BUTTONCOLOR);
			
			Lcd_Str16((u8 *)datebuf,180,192,BABYYELLOW,BUTTONCOLOR);
		}break;
		case PAGE_SYST:
		{
			RTC_Get_DateTimeCounter(&GetDate,&GetTime);
			sprintf(timebuf,"%02d:%02d:%02d",TimeBuf.Hours,TimeBuf.Minutes,
			TimeBuf.Seconds);
			sprintf(datebuf,"20%02d-%02d-%02d",DateBuf.Year,DateBuf.Month,
			DateBuf.Date);
			if(itempos == 5)//日期
				Lcd_Str16((u8 *)datebuf,SETCOL2,SYSROW1+SYSROWOFFSET*6,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)datebuf,SETCOL2,SYSROW1+SYSROWOFFSET*6,BABYYELLOW,BUTTONCOLOR);
			if(itempos == 6)//时间
				Lcd_Str16((u8 *)timebuf,SETCOL2,SYSROW1+SYSROWOFFSET*7,BUTTONCOLOR,BABYYELLOW);
			else
				Lcd_Str16((u8 *)timebuf,SETCOL2,SYSROW1+SYSROWOFFSET*7,BABYYELLOW,BUTTONCOLOR);
			
//			Lcd_Str16((u8 *)timebuf,170,1,BABYYELLOW,BUTTONCOLOR);
//			Lcd_Str16((u8 *)datebuf,180,192,BABYYELLOW,BUTTONCOLOR);
		}break;
		default:break;
	}
	
}

void DISP_BAT(u8 cap)
{
	LcdFillRec(300,2,310,20,BUTTONCOLOR);
	LcdFillRec(303,2,307,5,TOPCOLOR);
	LcdPrintRec(300,5,310,20,TOPCOLOR);
	LcdFillRec(301,6,309,19,TOPCOLOR);
	if(cap < 25)
	{
		LcdFillRec(301,6,309,15,BUTTONCOLOR);
	}else if(cap >=25 && cap < 50){
		LcdFillRec(301,6,309,12,BUTTONCOLOR);
	}else if(cap >=50 && cap < 75){
		LcdFillRec(301,6,309,9,BUTTONCOLOR);
	}else if(cap >=75){
		
	}
	
}

void DISP_CHARGE(void)//充电图标
{
	LcdFillRec(300,2,310,20,BUTTONCOLOR);
	LcdPrintVert(303,2,5,TOPCOLOR);
	LcdPrintVert(303+4,2,5,TOPCOLOR);
	LcdPrintHorz(300,7,11,TOPCOLOR);
	LcdFillRec(302,8,308,15,TOPCOLOR);
	LcdPrintHorz(303,16,5,TOPCOLOR);
	LcdPrintHorz(304,17,3,TOPCOLOR);
	LcdPrintVert(305,17,4,TOPCOLOR);
	
}

void DISP_USB(void)//U盘图标
{
	LcdFillRec(260-10,6,266-10,9,ORANGE);
	LcdFillRec(260+2-10,6+1,260+2-10,6+1,BUTTONCOLOR);
	LcdFillRec(266-2-10,6+1,266-2-10,6+1,BUTTONCOLOR);
	LcdFillRec(259-10,10,267-10,17,ORANGE);
	LcdPrintHorz(260-10,18,7,ORANGE);
	LcdPrintHorz(262-10,19,3,ORANGE);
//	LcdPrintHorz(263,20,2,TOPCOLOR);
}

void DISP_HID(void)//上位机通讯图标
{
	LcdFillRec(270,8,271,8+10,TOPCOLOR);
	LcdPrintHorz(268,8-1,6,TOPCOLOR);
	LcdPrintHorz(269,8-2,4,TOPCOLOR);
	LcdPrintHorz(270,8-3,2,TOPCOLOR);
	
	LcdFillRec(275,8-3,276,15,TOPCOLOR);
	LcdPrintHorz(275-2,15+1,6,TOPCOLOR);
	LcdPrintHorz(275-1,15+2,4,TOPCOLOR);
	LcdPrintHorz(275,15+3,2,TOPCOLOR);
	
}

void DrawLogo(u16 x,u16 y)
{
//	page_flag = 0xff;
//	LCD_Clear(LCD_COLOR_BLACK);	
	/*J*/
	LcdPrintHorz(x,y,41,ORANGE);
	LcdPrintHorz(x-1,y-1,44,ORANGE);
	LcdPrintHorz(x-1,y-2,46,ORANGE);
	LcdPrintHorz(x,y-3,46,ORANGE);
	LcdPrintHorz(x+2,y-4,46,ORANGE);
	LcdPrintHorz(x+35,y-5,14,ORANGE);
	LcdPrintHorz(x+35,y-6,15,ORANGE);
	LcdPrintHorz(x+35,y-7,15,ORANGE);
	LcdPrintHorz(x+36,y-8,14,ORANGE);
	LcdPrintHorz(x+36,y-9,15,ORANGE);
	LcdPrintHorz(x+37,y-10,14,ORANGE);
	LcdPrintHorz(x+37,y-11,14,ORANGE);
	LcdPrintHorz(x+37,y-12,14,ORANGE);
	LcdPrintHorz(x+38,y-13,14,ORANGE);
	LcdPrintHorz(x+37,y-14,15,ORANGE);
	LcdPrintHorz(x+38,y-15,14,ORANGE);
	LcdPrintHorz(x+38,y-16,14,ORANGE);
	LcdPrintHorz(x+38,y-17,15,ORANGE);
	LcdPrintHorz(x+39,y-18,14,ORANGE);
	LcdPrintHorz(x+39,y-19,14,ORANGE);
	LcdPrintHorz(x+39,y-20,15,ORANGE);
	LcdPrintHorz(x+40,y-21,14,ORANGE);
	LcdPrintHorz(x+40,y-22,14,ORANGE);
	LcdPrintHorz(x+40,y-23,14,ORANGE);
	LcdPrintHorz(x+40,y-24,14,ORANGE);
	LcdPrintHorz(x+41,y-25,13,ORANGE);
	LcdPrintHorz(x+41,y-26,14,ORANGE);
	LcdPrintHorz(x+41,y-27,14,ORANGE);
	LcdPrintHorz(x+42,y-28,14,ORANGE);
	LcdPrintHorz(x+42,y-29,14,ORANGE);
	LcdPrintHorz(x+42,y-30,14,ORANGE);
	LcdPrintHorz(x+42,y-31,15,ORANGE);
	LcdPrintHorz(x+42,y-32,15,ORANGE);
	LcdPrintHorz(x+43,y-33,13,ORANGE);
	LcdPrintHorz(x+43,y-34,14,ORANGE);
	LcdPrintHorz(x+44,y-35,13,ORANGE);
	LcdPrintHorz(x+44,y-36,14,ORANGE);
	LcdPrintHorz(x+44,y-37,14,ORANGE);
	LcdPrintHorz(x+44,y-38,15,ORANGE);
	LcdPrintHorz(x+44,y-39,15,ORANGE);
	LcdPrintHorz(x+45,y-40,14,ORANGE);
	LcdPrintHorz(x+45,y-41,14,ORANGE);
	LcdPrintHorz(x+45,y-42,15,ORANGE);
	LcdPrintHorz(x+45,y-43,15,ORANGE);
	LcdPrintHorz(x+46,y-44,14,ORANGE);
	LcdPrintHorz(x+46,y-45,14,ORANGE);
	LcdPrintHorz(x+46,y-46,15,ORANGE);
	LcdPrintHorz(x+47,y-47,14,ORANGE);
	LcdPrintHorz(x+47,y-48,14,ORANGE);
	LcdPrintHorz(x+47,y-49,15,ORANGE);
	LcdPrintHorz(x+47,y-50,15,ORANGE);
	LcdPrintHorz(x+48,y-51,14,ORANGE);
	LcdPrintHorz(x+48,y-52,14,ORANGE);
	LcdPrintHorz(x+48,y-53,14,ORANGE);
	LcdPrintHorz(x+49,y-54,13,ORANGE);
	LcdPrintHorz(x+49,y-55,14,ORANGE);
	
	/*i*/
	LcdFillRec(x+72,y-54,x+72+12,y-54+3,ORANGE);
	LcdPrintHorz(x+71,y-52,13,ORANGE);
	LcdFillRec(x+71,y-51,x+71+12,y-51+4,ORANGE);
	LcdFillRec(x+70,y-47,x+70+12,y-47+3,ORANGE);
	
	LcdPrintHorz(x+68,y-44+4,14,ORANGE);
	LcdPrintHorz(x+69,y-43+4,13,ORANGE);
	LcdPrintHorz(x+68,y-42+4,13,ORANGE);
	LcdPrintHorz(x+68,y-41+4,12,ORANGE);
	LcdFillRec(x+68-1,y-40+4,x+68-1+13,y-40+4+3,ORANGE);
	LcdPrintHorz(x+68-1,y-37+4,12,ORANGE);
	LcdFillRec(x+67-1,y-36+4,x+67-1+13,y-36+4+3,ORANGE);
	LcdPrintHorz(x+67-1,y-33+4,12,ORANGE);
	LcdFillRec(x+66-1,y-32+4,x+66-1+13,y-32+4+2,ORANGE);
	LcdPrintHorz(x+66-1,y-30+4,12,ORANGE);
	LcdFillRec(x+65-1,y-29+4,x+65-1+13,y-29+4+3,ORANGE);
	LcdPrintHorz(x+64-1,y-26+4,14,ORANGE);
	LcdFillRec(x+64-1,y-25+4,x+64-1+13,y-25+4+2,ORANGE);
	LcdFillRec(x+64-1,y-23+4,x+64-1+12,y-23+4+2,ORANGE);
	LcdPrintHorz(x+63-1,y-21+4,13,ORANGE);
	LcdFillRec(x+63-1,y-20+4,x+63-1+12,y-20+4+2,ORANGE);
	LcdFillRec(x+62-1,y-18+4,x+62-1+13,y-18+4+3,ORANGE);
	LcdPrintHorz(x+62-1,y-15+4,12,ORANGE);
	LcdFillRec(x+61-1,y-14+4,x+61-1+13,y-14+4+3,ORANGE);
	LcdFillRec(x+60-1,y-11+4,x+60-1+13,y-11+4+2,ORANGE);
	LcdFillRec(x+59-1,y-5,x+59-1+13,y-5+2,ORANGE);
	LcdPrintHorz(x+59-1,y-3,14,ORANGE);
	LcdFillRec(x+59-1,y-2,x+59-1+13,y-2+2,ORANGE);
	LcdPrintHorz(x+59-1,y,12,ORANGE);
	
	/*n*/
	LcdPrintHorz(x+88,y-40,23,ORANGE);
	LcdPrintHorz(x+85,y-39,27,ORANGE);
	LcdPrintHorz(x+84,y-38,30,ORANGE);
	LcdPrintHorz(x+83,y-37,32,ORANGE);
	LcdPrintHorz(x+83,y-36,3,ORANGE);
	LcdPrintHorz(x+104,y-36,12,ORANGE);
	LcdPrintHorz(x+83,y-35,1,ORANGE);
	LcdPrintHorz(x+104,y-35,12,ORANGE);
	LcdFillRec(x+104,y-34,x+104+12,y-34+3,ORANGE);
	LcdFillRec(x+103,y-31,x+103+13,y-31+3,ORANGE);
	LcdPrintHorz(x+102,y-28,13,ORANGE);
	LcdPrintHorz(x+103,y-27,12,ORANGE);
	LcdPrintHorz(x+102,y-26,13,ORANGE);
	LcdFillRec(x+101,y-25,x+101+13,y-25+2,ORANGE);
	LcdFillRec(x+101,y-23,x+101+12,y-23+2,ORANGE);
	LcdFillRec(x+100,y-21,x+100+13,y-21+3,ORANGE);
	LcdFillRec(x+99,y-18,x+99+13,y-18+3,ORANGE);
	LcdPrintHorz(x+99,y-15,12,ORANGE);
	LcdFillRec(x+98,y-14,x+98+13,y-14+3,ORANGE);
	LcdPrintHorz(x+98,y-11,12,ORANGE);
	LcdPrintHorz(x+97,y-10,13,ORANGE);
	LcdFillRec(x+97,y-9,x+97+12,y-9+4,ORANGE);
	LcdFillRec(x+96,y-5,x+96+12,y-5+3,ORANGE);
	LcdFillRec(x+95,y-2,x+95+13,y-2+3,ORANGE);
	
	/*k*/
	LcdPrintHorz(x+127,y-56,12,ORANGE);
	LcdFillRec(x+126,y-55,x+126+13,y-55+3,ORANGE);
	LcdFillRec(x+126,y-52,x+126+12,y-52+2,ORANGE);
	LcdFillRec(x+125,y-50,x+125+12,y-50+2,ORANGE);
	LcdPrintHorz(x+124,y-48,13,ORANGE);
	LcdPrintHorz(x+124,y-47,12,ORANGE);
	LcdPrintHorz(x+124,y-46,13,ORANGE);
	LcdPrintHorz(x+124,y-45,12,ORANGE);
	LcdFillRec(x+123,y-44,x+123+13,y-44+2,ORANGE);
	LcdFillRec(x+123,y-42,x+123+12,y-42+3,ORANGE);
	LcdPrintHorz(x+153,y-41,12,ORANGE);
	LcdPrintHorz(x+151,y-40,13,ORANGE);
	
	LcdPrintHorz(x+122,y-39,13,ORANGE);
	LcdPrintHorz(x+150,y-39,12,ORANGE);
	
	LcdFillRec(x+122,y-38,x+122+12,y-38+2,ORANGE);
	LcdPrintHorz(x+148,y-38,13,ORANGE);
	LcdPrintHorz(x+147,y-37,12,ORANGE);
	
	LcdFillRec(x+121,y-36,x+121+13,y-36+3,ORANGE);
	LcdPrintHorz(x+146,y-36,12,ORANGE);
	LcdPrintHorz(x+145,y-35,11,ORANGE);
	LcdPrintHorz(x+143,y-34,12,ORANGE);
	
	LcdPrintHorz(x+120,y-33,13,ORANGE);
	LcdPrintHorz(x+141,y-33,13,ORANGE);
	
	LcdPrintHorz(x+121,y-32,12,ORANGE);
	LcdPrintHorz(x+140,y-32,12,ORANGE);
	
	LcdPrintHorz(x+120,y-31,13,ORANGE);
	LcdPrintHorz(x+138,y-31,13,ORANGE);
	
	LcdPrintHorz(x+120,y-30,12,ORANGE);
	LcdPrintHorz(x+137,y-30,13,ORANGE);
	
	LcdPrintHorz(x+119,y-29,13,ORANGE);
	LcdPrintHorz(x+135,y-29,15,ORANGE);
	
	LcdPrintHorz(x+119,y-28,12,ORANGE);
	LcdPrintHorz(x+134,y-28,16,ORANGE);
	
	LcdPrintHorz(x+119,y-27,31,ORANGE);
	LcdPrintHorz(x+118,y-26,31,ORANGE);
	LcdPrintHorz(x+118,y-25,32,ORANGE);
	LcdPrintHorz(x+118,y-24,32,ORANGE);
	LcdPrintHorz(x+117,y-23,33,ORANGE);
	
	LcdPrintHorz(x+118,y-22,19,ORANGE);
	LcdPrintHorz(x+138,y-22,12,ORANGE);
	
	LcdPrintHorz(x+117,y-21,19,ORANGE);
	LcdPrintHorz(x+138,y-21,12,ORANGE);
	
	LcdPrintHorz(x+117,y-20,18,ORANGE);
	LcdPrintHorz(x+138,y-20,13,ORANGE);
	
	LcdPrintHorz(x+116,y-19,17,ORANGE);
	LcdPrintHorz(x+138,y-19,13,ORANGE);
	
	LcdPrintHorz(x+116,y-18,16,ORANGE);
	LcdPrintHorz(x+138,y-18,14,ORANGE);
	
	LcdPrintHorz(x+116,y-17,14,ORANGE);
	LcdPrintHorz(x+138,y-17,14,ORANGE);
	
	LcdPrintHorz(x+116,y-16,13,ORANGE);
	LcdPrintHorz(x+138,y-16,14,ORANGE);
	
	LcdFillRec(x+115,y-15,x+115+13,y-15+3,ORANGE);
	LcdPrintHorz(x+115,y-12,12,ORANGE);
	LcdFillRec(x+114,y-11,x+114+13,y-11+3,ORANGE);
	LcdPrintHorz(x+114,y-8,12,ORANGE);
	LcdFillRec(x+113,y-7,x+113+13,y-7+3,ORANGE);
	LcdPrintHorz(x+112,y-4,14,ORANGE);
	LcdFillRec(x+112,y-3,x+112+13,y-3+3,ORANGE);
	LcdPrintHorz(x+112,y,12,ORANGE);
	
	LcdFillRec(x+139,y-15,x+139+13,y-15+3,ORANGE);	
	LcdFillRec(x+139,y-12,x+139+14,y-12+2,ORANGE);
	LcdFillRec(x+140,y-10,x+140+13,y-10+3,ORANGE);
	LcdFillRec(x+140,y-7,x+140+14,y-7+3,ORANGE);
	LcdFillRec(x+140,y-4,x+140+15,y-4+2,ORANGE);
	LcdPrintHorz(x+141,y-2,14,ORANGE);
	LcdPrintHorz(x+143,y-1,12,ORANGE);
	LcdPrintHorz(x+145,y,10,ORANGE);
	
	/*o*/
	LcdPrintHorz(x+173,y-41,35,ORANGE);
	LcdPrintHorz(x+171,y-40,39,ORANGE);
	LcdPrintHorz(x+169,y-39,41,ORANGE);
	LcdPrintHorz(x+168,y-38,43,ORANGE);
	
	LcdPrintHorz(x+167,y-37,13,ORANGE);
	LcdPrintHorz(x+198,y-37,13,ORANGE);
	
	LcdPrintHorz(x+166,y-36,13,ORANGE);
	LcdPrintHorz(x+199,y-36,13,ORANGE);
	
	LcdPrintHorz(x+166,y-35,12,ORANGE);
	LcdPrintHorz(x+198,y-35,14,ORANGE);
	
	LcdPrintHorz(x+165,y-34,13,ORANGE);
	LcdPrintHorz(x+198,y-34,14,ORANGE);
	
	LcdPrintHorz(x+165,y-33,13,ORANGE);
	LcdPrintHorz(x+198,y-33,13,ORANGE);
	
	LcdPrintHorz(x+165,y-32,13,ORANGE);
	LcdPrintHorz(x+198,y-32,13,ORANGE);
	
	LcdFillRec(x+164,y-31,x+164+14,y-31+2,ORANGE);
	LcdFillRec(x+197,y-31,x+197+14,y-31+2,ORANGE);
	LcdPrintHorz(x+164,y-29,13,ORANGE);
	LcdPrintHorz(x+197,y-29,13,ORANGE);
	
	LcdFillRec(x+163,y-28,x+163+14,y-28+2,ORANGE);
	LcdFillRec(x+196,y-28,x+196+14,y-28+2,ORANGE);
	LcdPrintHorz(x+163,y-26,13,ORANGE);
	LcdPrintHorz(x+196,y-26,13,ORANGE);
	
	LcdPrintHorz(x+162,y-25,14,ORANGE);
	LcdPrintHorz(x+196,y-25,13,ORANGE);
	
	LcdPrintHorz(x+162,y-25,13,ORANGE);
	LcdPrintHorz(x+195,y-25,14,ORANGE);
	
	LcdPrintHorz(x+162,y-24,14,ORANGE);
	LcdPrintHorz(x+195,y-24,13,ORANGE);
	
	LcdPrintHorz(x+162,y-23,13,ORANGE);
	LcdPrintHorz(x+195,y-23,13,ORANGE);
	
	LcdPrintHorz(x+161,y-22,14,ORANGE);
	LcdPrintHorz(x+194,y-22,14,ORANGE);
	
	LcdPrintHorz(x+161,y-21,14,ORANGE);
	LcdPrintHorz(x+194,y-21,14,ORANGE);
	
	LcdPrintHorz(x+161,y-20,13,ORANGE);
	LcdPrintHorz(x+194,y-20,13,ORANGE);
	
	LcdPrintHorz(x+160,y-19,14,ORANGE);
	LcdPrintHorz(x+194,y-19,13,ORANGE);
	
	LcdPrintHorz(x+160,y-18,14,ORANGE);
	LcdPrintHorz(x+193,y-18,14,ORANGE);
	
	LcdPrintHorz(x+160,y-17,13,ORANGE);
	LcdPrintHorz(x+193,y-17,14,ORANGE);
	
	LcdPrintHorz(x+159,y-16,14,ORANGE);
	LcdPrintHorz(x+193,y-16,14,ORANGE);
	
	LcdPrintHorz(x+160,y-15,13,ORANGE);
	LcdPrintHorz(x+193,y-15,13,ORANGE);	
	
	LcdFillRec(x+159,y-14,x+159+13,y-14+4,ORANGE);
	LcdPrintHorz(x+192,y-14,14,ORANGE);	
	LcdFillRec(x+192,y-13,x+192+13,y-13+3,ORANGE);
	
	LcdFillRec(x+159,y-10,x+159+12,y-10+3,ORANGE);
	LcdFillRec(x+191,y-10,x+191+13,y-10+2,ORANGE);
	LcdPrintHorz(x+190,y-8,13,ORANGE);	
	
	LcdPrintHorz(x+159,y-7,14,ORANGE);
	LcdPrintHorz(x+188,y-7,14,ORANGE);
	
	LcdPrintHorz(x+159,y-6,43,ORANGE);
	LcdPrintHorz(x+160,y-5,41,ORANGE);
	LcdPrintHorz(x+161,y-4,39,ORANGE);
	LcdPrintHorz(x+162,y-3,36,ORANGE);
	LcdPrintHorz(x+163,y-2,34,ORANGE);
	LcdPrintHorz(x+164,y-1,32,ORANGE);
	
//	/*金*/
//	LcdPrintHorz(x+224,y-36,18,ORANGE);
//	LcdPrintHorz(x+223,y-35,22,ORANGE);
//	LcdPrintHorz(x+222,y-34,24,ORANGE);
//	LcdPrintHorz(x+221,y-33,25,ORANGE);
//	
//	LcdPrintHorz(x+221,y-32,9,ORANGE);
//	LcdPrintHorz(x+238,y-32,8,ORANGE);
//	
//	LcdPrintHorz(x+220,y-31,9,ORANGE);
//	LcdPrintHorz(x+238,y-31,9,ORANGE);
//	
//	LcdPrintHorz(x+219,y-30,9,ORANGE);
//	LcdPrintHorz(x+238,y-30,9,ORANGE);
//	
//	LcdPrintHorz(x+218,y-29,9,ORANGE);
//	LcdPrintHorz(x+239,y-29,8,ORANGE);
//	
//	LcdPrintHorz(x+218,y-28,8,ORANGE);
//	LcdPrintHorz(x+239,y-28,9,ORANGE);
//	
//	LcdPrintHorz(x+217,y-27,8,ORANGE);
//	LcdPrintHorz(x+226,y-27,22,ORANGE);
//	
//	LcdPrintHorz(x+215,y-26,33,ORANGE);
//	LcdPrintHorz(x+214,y-25,35,ORANGE);
//	
//	LcdPrintHorz(x+213,y-24,9,ORANGE);
//	LcdPrintHorz(x+227,y-24,8,ORANGE);
//	LcdPrintHorz(x+232,y-24,9,ORANGE);
//	
//	LcdPrintHorz(x+213,y-23,8,ORANGE);
//	LcdPrintHorz(x+227,y-23,9,ORANGE);
//	LcdPrintHorz(x+232,y-23,9,ORANGE);
//	
//	LcdPrintHorz(x+227,y-22,8,ORANGE);
//	LcdPrintHorz(x+226,y-21,9,ORANGE);
//	LcdPrintHorz(x+226,y-20,8,ORANGE);
//	
//	LcdPrintHorz(x+213,y-19,34,ORANGE);
//	LcdPrintHorz(x+212,y-18,35,ORANGE);
//	LcdPrintHorz(x+212,y-17,35,ORANGE);
//	
//	LcdPrintHorz(x+225,y-16,8,ORANGE);
//	
//	LcdPrintHorz(x+213,y-15,7,ORANGE);
//	LcdPrintHorz(x+225,y-15,8,ORANGE);
//	LcdPrintHorz(x+238,y-15,7,ORANGE);
//	
//	LcdPrintHorz(x+212,y-14,8,ORANGE);
//	LcdPrintHorz(x+225,y-14,8,ORANGE);
//	LcdPrintHorz(x+237,y-14,8,ORANGE);
//	
//	LcdPrintHorz(x+212,y-13,8,ORANGE);
//	LcdPrintHorz(x+224,y-13,9,ORANGE);
//	LcdPrintHorz(x+237,y-13,7,ORANGE);
//	
//	LcdPrintHorz(x+212,y-12,8,ORANGE);
//	LcdPrintHorz(x+224,y-12,8,ORANGE);
//	LcdPrintHorz(x+236,y-12,8,ORANGE);
//	
//	LcdPrintHorz(x+212,y-11,8,ORANGE);
//	LcdPrintHorz(x+223,y-11,9,ORANGE);
//	LcdPrintHorz(x+235,y-11,8,ORANGE);
//	
//	LcdPrintHorz(x+212,y-10,8,ORANGE);
//	LcdPrintHorz(x+223,y-10,9,ORANGE);
//	LcdPrintHorz(x+235,y-10,7,ORANGE);
//	
//	LcdPrintHorz(x+212,y-9,8,ORANGE);
//	LcdPrintHorz(x+223,y-9,8,ORANGE);
//	LcdPrintHorz(x+235,y-9,7,ORANGE);
//	
//	LcdPrintHorz(x+212,y-8,8,ORANGE);
//	LcdPrintHorz(x+222,y-8,9,ORANGE);
//	LcdPrintHorz(x+234,y-8,7,ORANGE);
//	
//	LcdPrintHorz(x+212,y-7,9,ORANGE);
//	LcdPrintHorz(x+222,y-7,19,ORANGE);

//	LcdPrintHorz(x+214,y-6,16,ORANGE);
//	LcdPrintHorz(x+231,y-6,8,ORANGE);
//	
//	LcdPrintHorz(x+217,y-5,4,ORANGE);
//	LcdPrintHorz(x+222,y-5,8,ORANGE);
//	LcdPrintHorz(x+232,y-5,4,ORANGE);
//	
//	LcdPrintHorz(x+222,y-4,8,ORANGE);
//	
//	LcdFillRec(x+206,y-3,x+206+39,y-3+2,ORANGE);
//	
//	/*科*/
//	LcdPrintHorz(x+269,y-38,7,ORANGE);
//	LcdPrintHorz(x+285,y-38,6,ORANGE);
//	
//	LcdPrintHorz(x+267,y-37,9,ORANGE);
//	LcdPrintHorz(x+285,y-37,6,ORANGE);
//	
//	LcdPrintHorz(x+265,y-36,11,ORANGE);
//	LcdPrintHorz(x+284,y-36,7,ORANGE);
//	
//	LcdPrintHorz(x+256,y-35,18,ORANGE);
//	LcdPrintHorz(x+275,y-35,7,ORANGE);
//	LcdPrintHorz(x+284,y-35,7,ORANGE);
//	
//	LcdPrintHorz(x+256,y-34,17,ORANGE);
//	LcdPrintHorz(x+275,y-34,7,ORANGE);
//	LcdPrintHorz(x+283,y-34,8,ORANGE);
//	
//	LcdPrintHorz(x+256,y-33,15,ORANGE);
//	LcdPrintHorz(x+274,y-33,7,ORANGE);
//	LcdPrintHorz(x+283,y-33,8,ORANGE);
//	
//	LcdPrintHorz(x+261,y-32,6,ORANGE);
//	LcdPrintHorz(x+274,y-32,7,ORANGE);
//	LcdPrintHorz(x+283,y-32,8,ORANGE);
//	
//	LcdPrintHorz(x+261,y-31,6,ORANGE);
//	LcdPrintHorz(x+274,y-31,7,ORANGE);
//	LcdPrintHorz(x+282,y-31,8,ORANGE);
//	
//	LcdPrintHorz(x+261,y-30,6,ORANGE);
//	LcdPrintHorz(x+274,y-30,7,ORANGE);
//	LcdPrintHorz(x+282,y-30,8,ORANGE);
//	
//	LcdPrintHorz(x+260,y-29,7,ORANGE);
//	LcdPrintHorz(x+274,y-29,7,ORANGE);
//	LcdPrintHorz(x+282,y-29,8,ORANGE);
//	
//	LcdPrintHorz(x+253,y-28,20,ORANGE);
//	LcdPrintHorz(x+275,y-28,6,ORANGE);
//	LcdPrintHorz(x+282,y-28,7,ORANGE);
//	
//	LcdPrintHorz(x+253,y-27,19,ORANGE);
//	LcdPrintHorz(x+274,y-27,7,ORANGE);
//	LcdPrintHorz(x+282,y-27,7,ORANGE);
//	
//	LcdPrintHorz(x+259,y-26,7,ORANGE);
//	LcdPrintHorz(x+274,y-26,7,ORANGE);
//	LcdPrintHorz(x+282,y-26,7,ORANGE);
//	
//	LcdPrintHorz(x+258,y-25,8,ORANGE);
//	LcdPrintHorz(x+274,y-25,14,ORANGE);

//	LcdPrintHorz(x+259,y-24,6,ORANGE);
//	LcdPrintHorz(x+277,y-24,11,ORANGE);
//	
//	LcdPrintHorz(x+253,y-23,4,ORANGE);
//	LcdPrintHorz(x+258,y-23,10,ORANGE);
//	LcdPrintHorz(x+271,y-23,5,ORANGE);
//	LcdPrintHorz(x+281,y-23,6,ORANGE);
//	
//	LcdPrintHorz(x+253,y-22,16,ORANGE);
//	LcdPrintHorz(x+270,y-22,7,ORANGE);
//	LcdPrintHorz(x+280,y-22,8,ORANGE);
//	
//	LcdPrintHorz(x+253,y-21,16,ORANGE);
//	LcdPrintHorz(x+270,y-21,6,ORANGE);
//	LcdPrintHorz(x+280,y-21,7,ORANGE);
//	
//	LcdPrintHorz(x+252,y-20,24,ORANGE);
//	LcdPrintHorz(x+280,y-20,7,ORANGE);
//	
//	LcdPrintHorz(x+252,y-19,4,ORANGE);
//	LcdPrintHorz(x+257,y-19,12,ORANGE);
//	LcdPrintHorz(x+270,y-19,6,ORANGE);
//	LcdPrintHorz(x+280,y-19,7,ORANGE);
//	
//	LcdPrintHorz(x+252,y-18,4,ORANGE);
//	LcdPrintHorz(x+257,y-18,12,ORANGE);
//	LcdPrintHorz(x+270,y-18,6,ORANGE);
//	LcdPrintHorz(x+279,y-18,7,ORANGE);
//	
//	LcdPrintHorz(x+251,y-17,18,ORANGE);
//	LcdPrintHorz(x+270,y-17,6,ORANGE);
//	LcdPrintHorz(x+279,y-17,7,ORANGE);
//		
//	LcdPrintHorz(x+251,y-16,12,ORANGE);
//	LcdPrintHorz(x+264,y-16,4,ORANGE);
//	LcdPrintHorz(x+269,y-16,7,ORANGE);
//	LcdPrintHorz(x+279,y-16,7,ORANGE);
//	
//	LcdPrintHorz(x+251,y-15,17,ORANGE);
//	LcdPrintHorz(x+269,y-15,7,ORANGE);
//	LcdPrintHorz(x+278,y-15,8,ORANGE);
//	
//	LcdPrintHorz(x+251,y-14,11,ORANGE);
//	LcdPrintHorz(x+263,y-14,5,ORANGE);
//	LcdPrintHorz(x+269,y-14,7,ORANGE);
//	LcdPrintHorz(x+278,y-14,7,ORANGE);
//	
//	LcdPrintHorz(x+250,y-13,17,ORANGE);
//	LcdPrintHorz(x+270,y-13,6,ORANGE);
//	LcdPrintHorz(x+278,y-13,7,ORANGE);
//	
//	LcdPrintHorz(x+250,y-12,17,ORANGE);
//	LcdPrintHorz(x+271,y-12,6,ORANGE);
//	LcdPrintHorz(x+278,y-12,7,ORANGE);
//	
//	LcdPrintHorz(x+250,y-11,11,ORANGE);
//	LcdPrintHorz(x+262,y-11,5,ORANGE);
//	LcdPrintHorz(x+273,y-11,12,ORANGE);
//	
//	LcdPrintHorz(x+249,y-10,18,ORANGE);
//	LcdPrintHorz(x+277,y-10,7,ORANGE);
//	
//	LcdPrintHorz(x+249,y-9,5,ORANGE);
//	LcdPrintHorz(x+255,y-9,12,ORANGE);
//	LcdPrintHorz(x+273,y-9,14,ORANGE);
//	
//	LcdPrintHorz(x+248,y-8,39,ORANGE);
//	
//	LcdPrintHorz(x+248,y-7,12,ORANGE);
//	LcdPrintHorz(x+260,y-7,22,ORANGE);
//	
//	LcdPrintHorz(x+248,y-6,12,ORANGE);
//	LcdPrintHorz(x+260,y-6,5,ORANGE);
//	LcdPrintHorz(x+275,y-6,7,ORANGE);
//	
//	LcdPrintHorz(x+248,y-5,18,ORANGE);
//	LcdPrintHorz(x+274,y-5,8,ORANGE);
//	
//	LcdPrintHorz(x+248,y-4,4,ORANGE);
//	LcdPrintHorz(x+253,y-4,7,ORANGE);
//	LcdPrintHorz(x+260,y-4,7,ORANGE);
//	LcdPrintHorz(x+274,y-4,8,ORANGE);
//	
//	LcdPrintHorz(x+247,y-3,4,ORANGE);
//	LcdPrintHorz(x+253,y-3,6,ORANGE);
//	LcdPrintHorz(x+260,y-3,5,ORANGE);
//	LcdPrintHorz(x+274,y-3,8,ORANGE);
//	
//	LcdPrintHorz(x+247,y-2,4,ORANGE);
//	LcdPrintHorz(x+253,y-2,7,ORANGE);
//	LcdPrintHorz(x+262,y-2,2,ORANGE);
//	LcdPrintHorz(x+273,y-2,9,ORANGE);
//	
//	LcdPrintHorz(x+247,y-1,1,ORANGE);
//	LcdPrintHorz(x+252,y-1,7,ORANGE);
//	LcdPrintHorz(x+273,y-1,8,ORANGE);
//	
//	LcdPrintHorz(x+273,y,2,ORANGE);
}

void DISP_POWERON(void)
{
	u8 i;
	LcdClear(BUTTONCOLOR);
	DrawLogo(50,150);
	delay_ms(800);
	LcdClear(BUTTONCOLOR);
	for(i=0;i<10;i++)
	{
		Lcd_Str16((u8 *)POWERON_ITEM[i],2,2+18*i,DATACOLOR,BUTTONCOLOR);
		DelayMs(200);
	}
}
