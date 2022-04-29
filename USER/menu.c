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


const char TESTDISP_BUTTON[2][5][6]={
	{"设置",
	"系统",
	"",
	"",
	""},
	{"SETUP",
	"SYSTEM",
	"",
	"",
	""}
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
	""},
	{"MEAS ",
	"SETUP ",
	"SYSTEM",
	"",
	""}
	
};

const char DATE_BUTTON1[2][5][8]={
	{"年+",
	"年-",
	"月+",
	"月-",
	"1/2"},
	{"YEAR+",
	"YEAR-",
	"MON+",
	"MON+",
	"1/2"}
	
};

const char DATE_BUTTON2[2][5][8]={
	{"日+",
	"日-",
	"",
	"",
	"1/2"},
	{"DAY+",
	"DAY-",
	"",
	"",
	"1/2"}
	
};

const char TIME_BUTTON1[2][5][8]={
	{"时+",
	"时-",
	"分+",
	"分-",
	"1/2"},
	{"HOUR+",
	"HOUR-",
	"MIN+",
	"MIN+",
	"1/2"}
	
};

const char TIME_BUTTON2[2][5][8]={
	{"秒+",
	"秒-",
	"",
	"",
	"1/2"},
	{"SEC+",
	"SEC-",
	"",
	"",
	"1/2"}
	
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

const char TCTYPE_ITEM[][5]={
	"TC-T",
	"TC-K",
	"TC-J",
	"TC-N",
	"1/2",
	"TC-E",
	"TC-S",
	"TC-R",
	"TC-B",
	"2/2",
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
	"EXIT"
};

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



void KEY_HANDLE(u8 key)
{
	BZZUER();
	switch(pageflag)
	{
		case PAGE_MEAS:
		{
			switch(key)
			{
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
							TempOffset[0]+=10;
						}break;
						case 2:
						{
							TempOffset[1]+=10;
						}break;
						case 3:
						{
							TempOffset[2]+=10;
						}break;
						case 4:
						{
							TempOffset[3]+=10;
						}break;
						case 5:
						{
							TempOffset[4]+=10;
						}break;
						case 6:
						{
							TempOffset[5]+=10;
						}break;
						case 7:
						{
							TempOffset[6]+=10;
						}break;
						case 8:
						{
							TempOffset[7]+=10;
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
							TempOffset[0]-=10;
						}break;
						case 2:
						{
							TempOffset[1]-=10;
						}break;
						case 3:
						{
							TempOffset[2]-=10;
						}break;
						case 4:
						{
							TempOffset[3]-=10;
						}break;
						case 5:
						{
							TempOffset[4]-=10;
						}break;
						case 6:
						{
							TempOffset[5]-=10;
						}break;
						case 7:
						{
							TempOffset[6]-=10;
						}break;
						case 8:
						{
							TempOffset[7]-=10;
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
							TempOffset[0]+=1;
						}break;
						case 2:
						{
							TempOffset[1]+=1;
						}break;
						case 3:
						{
							TempOffset[2]+=1;
						}break;
						case 4:
						{
							TempOffset[3]+=1;
						}break;
						case 5:
						{
							TempOffset[4]+=1;
						}break;
						case 6:
						{
							TempOffset[5]+=1;
						}break;
						case 7:
						{
							TempOffset[6]+=1;
						}break;
						case 8:
						{
							TempOffset[7]+=1;
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
							TempOffset[0]-=1;
						}break;
						case 2:
						{
							TempOffset[1]-=1;
						}break;
						case 3:
						{
							TempOffset[2]-=1;
						}break;
						case 4:
						{
							TempOffset[3]-=1;
						}break;
						case 5:
						{
							TempOffset[4]-=1;
						}break;
						case 6:
						{
							TempOffset[5]-=1;
						}break;
						case 7:
						{
							TempOffset[6]-=1;
						}break;
						case 8:
						{
							TempOffset[7]-=1;
						}break;
						default:break;
					}
				}break;
				case KEY_F5:
				{
					itempos=0;
					moveflag = 1;
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
					SaveSysPara(SYSPAR);
					Brightness();
				}break;
//				case KEY_ACC:
//				{
//					memset(&SYSPAR,0,sizeof(SYSPAR));
//				}break;
				default:break;
			}
		}break;
		case PAGE_SETP:
		{
			switch(key)
			{
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
							SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.speed=0;
							moveflag = 1;
							SaveSysPara(SYSPAR);
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
							SaveSysPara(SYSPAR);
						}break;
						case 6:
						{
							SYSPAR.alarm=0;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 7:
						{
							SYSPAR.saveset=0;
							moveflag = 1;
							SaveSysPara(SYSPAR);
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
							SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.speed=1;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							SYSPAR.unit=1;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 6:
						{
							SYSPAR.alarm=1;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 7:
						{
							SYSPAR.saveset=1;
							moveflag = 1;
							SaveSysPara(SYSPAR);
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
							SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.speed=2;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							SYSPAR.unit=2;
							moveflag = 1;
							SaveSysPara(SYSPAR);
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
							SaveSysPara(SYSPAR);
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
							SaveSysPara(SYSPAR);
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
					SaveSysPara(SYSPAR);
					Brightness();
				}break;
				default:break;
			}
		}break;
		case PAGE_SYST:
		{
			switch(key)
			{
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
							SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.brtness=0;
							Brightness();
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=0;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=0;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							if(buttonpage == 0)
							{
								GetDate.Year++;
								moveflag = 1;
							}else{
								GetDate.Date++;
								moveflag = 1;
							}
							HAL_RTC_SetDate(&hrtc, &GetDate, RTC_FORMAT_BIN);
						}break;
						case 6:
						{
							if(buttonpage == 0)
							{
								GetTime.Hours++;
								moveflag = 1;
							}else{
								GetTime.Seconds++;
								moveflag = 1;
							}
							HAL_RTC_SetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
						}break;
						case 7:
						{
							SYSPAR.offsave=0;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							SYSPAR.touch=0;
							moveflag = 1;
							SaveSysPara(SYSPAR);
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
							SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.brtness=1;
							Brightness();
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=1;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=1;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							if(buttonpage == 0)
							{
								GetDate.Year--;
								moveflag = 1;
							}else{
								GetDate.Date--;
								moveflag = 1;
							}
							HAL_RTC_SetDate(&hrtc, &GetDate, RTC_FORMAT_BIN);
						}break;
						case 6:
						{
							if(buttonpage == 0)
							{
								GetTime.Hours--;
								moveflag = 1;
							}else{
								GetTime.Seconds--;
								moveflag = 1;
							}
							HAL_RTC_SetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
						}break;
						case 7:
						{
							SYSPAR.offsave=1;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							SYSPAR.touch=1;
							moveflag = 1;
							SaveSysPara(SYSPAR);
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
							pageflag = PAGE_CAL;
							displayflag = 1;
						}break;
//						case 1:
//						{
//							SYSPAR.language=1;
//							displayflag = 1;
//							SaveSysPara(SYSPAR);
//						}break;
						case 2:
						{
							SYSPAR.brtness=2;
							Brightness();
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=2;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=2;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							GetDate.Month++;
							moveflag = 1;
							HAL_RTC_SetDate(&hrtc, &GetDate, RTC_FORMAT_BIN);
						}break;
						case 6:
						{
							GetTime.Minutes++;
							moveflag = 1;
							HAL_RTC_SetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
						}break;
						case 7:
						{
							SYSPAR.offsave=2;
							moveflag = 1;
							SaveSysPara(SYSPAR);
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
							SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=3;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=3;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							GetDate.Month--;
							moveflag = 1;
							HAL_RTC_SetDate(&hrtc, &GetDate, RTC_FORMAT_BIN);
						}break;
						case 6:
						{
							GetTime.Minutes--;
							moveflag = 1;
							HAL_RTC_SetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
						}break;
						case 7:
						{
							SYSPAR.offsave=3;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							
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
							SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=4;
							moveflag = 1;
							SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=4;
							moveflag = 1;
							SaveSysPara(SYSPAR);
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
					SaveSysPara(SYSPAR);
					Brightness();
				}break;
				default:break;
			}
		}break;
		case PAGE_CAL:
		{
			switch(key)
			{
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
						Lcd_Str16((u8 *)TESTDISP_BUTTON[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
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
						Lcd_Str16((u8 *)TCTYPE_ITEM[i+buttonpage*5],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
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
						Lcd_Str16((u8 *)CAL_BUTTON[SYSPAR.language][i],1+(i*70)+17,UP_LINE_OFFSET+7,BUTTONCOLOR,FILLBLOCK);
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
	SaveSysPara(SYSPAR);
}


void DISP_TEMP(void)//显示温度
{
	u8 i;
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
				DispTemp[i]=CurrentTemp[i]+TempOffset[i];
				Hex_Format(DispTemp[i],1,5,0);
				if(DispTemp[i] > SYSPAR.upper || DispTemp[i] < SYSPAR.lower)
				{
					if(i+1==itempos)
						LcdPrintStr24((u8 *)DispBuf,168+40,42+i/2*38,BUTTONCOLOR,TCALCOLOR);//2468);
					else
						LcdPrintStr24((u8 *)DispBuf,168+40,42+i/2*38,TCALCOLOR,BUTTONCOLOR);//2468);
				}else{
					if(i+1==itempos)
						LcdPrintStr24((u8 *)DispBuf,168+40,42+i/2*38,BUTTONCOLOR,TOPCOLOR);//2468);
					else
						LcdPrintStr24((u8 *)DispBuf,168+40,42+i/2*38,TOPCOLOR,BUTTONCOLOR);//2468);
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
				DispTemp[i]=CurrentTemp[i]+TempOffset[i];
				Hex_Format(DispTemp[i],1,5,0);
				if(DispTemp[i] > SYSPAR.upper || DispTemp[i] < SYSPAR.lower)
				{
					if(i+1==itempos)
						LcdPrintStr24((u8 *)DispBuf,8+40,42+i/2*38,BUTTONCOLOR,TCALCOLOR);//1357);
					else
						LcdPrintStr24((u8 *)DispBuf,8+40,42+i/2*38,TCALCOLOR,BUTTONCOLOR);//1357);
				}else{
					if(i+1==itempos)
						LcdPrintStr24((u8 *)DispBuf,8+40,42+i/2*38,BUTTONCOLOR,TOPCOLOR);//1357);
					else
						LcdPrintStr24((u8 *)DispBuf,8+40,42+i/2*38,TOPCOLOR,BUTTONCOLOR);//1357);
				}
				
			}
		}
	}
}


void DISP_TEST(void)//测量显示页面固定显示
{
	u8 i;
	itempos=0;
	LcdClear(BUTTONCOLOR);
	DISP_TOP(PAGE_MEAS);
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

void DISP_PAGE(void)
{
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
	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
		/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &GetDate, RTC_FORMAT_BIN);

	sprintf(timebuf,"%02d:%02d:%02d",GetTime.Hours,GetTime.Minutes,
	GetTime.Seconds);
	sprintf(datebuf,"20%02d-%02d-%02d",GetDate.Year,GetDate.Month,
	GetDate.Date);
	switch(pageflag)
	{
		
		case PAGE_MEAS:
		{
			
			Lcd_Str16((u8 *)timebuf,170,1,BABYYELLOW,BUTTONCOLOR);
			
			Lcd_Str16((u8 *)datebuf,180,192,BABYYELLOW,BUTTONCOLOR);
		}break;
		case PAGE_SYST:
		{
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

