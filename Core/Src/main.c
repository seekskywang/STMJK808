/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define	SPI_FLASH_CS_0    HAL_GPIO_WritePin(SPI3CS_GPIO_Port,SPI3CS_Pin,GPIO_PIN_RESET)
#define SPI_FLASH_CS_1    HAL_GPIO_WritePin(SPI3CS_GPIO_Port,SPI3CS_Pin,GPIO_PIN_SET)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */
struct SParameter SYSPAR={0};
u8 KeyNum=0,KeyFlag=1;
u32 testdata=0x5589;
struct Tes_RUN Test;
struct CalPara LinearCoeff={1,1};
int16_t Tenir;
u16 testcount;
float temperature=0;
//int16_t TempOffset[8]={0};
int16_t DispTemp[8];
u8 usbstatus = UNKNOWN;
uint32_t ADC_Value=0;
u16 bat_vm,battm;
u8 fileflag;
u8 saveok;
u8 usbsendbuf[0x40];
u8 *crec;
u8 *csend;
uint16_t readcrc;
const u8 speedset[3]={10,5,1};
uint32_t ucountold;
uint32_t ucount;
u8 oldkey=0,key_trg=0;
u8 newkey=0;
u8 sdstatus;
u8 testres;
u8 teststatus;
u32 sdsize;
u8 inqbuf[36];
INQUIRY_DATA inqdata;
// 设置时间戳计数的基准日期
RTC_DateTypeDef DateBase = {
    .Year = 22,
    .Month = 05,
    .Date = 01,
    .WeekDay = RTC_WEEKDAY_SUNDAY,
};

extern unsigned char USB_Recive_Buffer[64];
extern u8 UsbHidReceiveComplete;
extern USBD_HandleTypeDef hUsbDeviceFS;
extern HAL_StatusTypeDef  RTC_WriteTimeCounter(RTC_HandleTypeDef *hrtc, uint32_t TimeCounter);
extern uint32_t RTC_ReadTimeCounter(RTC_HandleTypeDef *hrtc);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI3_Init(void);
static void MX_FSMC_Init(void);
static void MX_TIM3_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM2_Init(void);
static void MX_SPI2_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
extern uint8_t USBD_CUSTOM_HID_SendReport(USBD_HandleTypeDef *pdev,
                                   uint8_t *report,
                                   uint16_t len);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void DelayMs(u32 nMs)
{
	
	u32 i;
	for(; nMs >0; nMs--)
	{
		for(i=10301;i>0;i--);
	}
}

void DelayUs(u32 delay_us)
{
	u8 i;
	for(;delay_us>0;delay_us--)
	{
		for(i=10;i>0;i--);
	}
}

uint16_t CRC16(uint8_t *puchMsg, uint8_t Len)
{
	uint8_t t, m,n,p;
	uint8_t uchCRCHi=0xFF; /* 高CRC字节初始化*/ 
	uint8_t uchCRCLo =0xFF; /* 低CRC 字节初始化*/ 
	for(t=0;t<Len;t++)
	{	
		uchCRCLo=uchCRCLo^puchMsg[t];
		for(n=0;n<8;n++)
		{
			m=uchCRCLo&1;p=uchCRCHi&1;uchCRCHi>>=1;
			uchCRCLo>>=1;

			if(p)
			{
				uchCRCLo|=0x80;
			}
			if(m)	
			{
				uchCRCHi=uchCRCHi^0xa0;
				uchCRCLo=uchCRCLo^1;
			}
		}
	}
	return (uchCRCHi<<8|uchCRCLo);
}

void UsbDataHandle(void)
{
	u8 i;
//	u8 j;
	uint16_t sendcrc;
	u8 creclen;
	u8 csendlen;

//	u16 voltage;//电压
//	u16 current;
//	u32 power;  //功率
//	u16 frequancy;
//	u16 PF;//功率因数
	
	if(USB_Recive_Buffer[0] == 0x01)
	{
		if(USB_Recive_Buffer[1] == 0x03)//读数据
		{
			free(crec);
			free(csend);
			if(USB_Recive_Buffer[2] == 0 && USB_Recive_Buffer[3] == 0)
			{
				readcrc = USB_Recive_Buffer[4] << 8|USB_Recive_Buffer[5];
				creclen = 4;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//初始化，每个元素都为零
				for(i = 0;i < 4;i++)
				{
					crec[i] = USB_Recive_Buffer[i];
				}
			}else{
				readcrc = USB_Recive_Buffer[6] << 8|USB_Recive_Buffer[7];
				creclen = 6;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//初始化，每个元素都为零
				for(i = 0;i < 6;i++)
				{
					crec[i] = USB_Recive_Buffer[i];
				}				
			}				
//			crcwatch = CRC16(crec,creclen);
			if(CRC16(crec,creclen) == readcrc)//CRC校验
			{
				
				if(USB_Recive_Buffer[2] == 0 && USB_Recive_Buffer[3] == 0)//读实时数据
				{
								
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = USB_Recive_Buffer[2];
					usbsendbuf[3] = USB_Recive_Buffer[3];
					usbsendbuf[4] = 0x00;
					if(SYSPAR.version == 0)
					{
						csendlen = 22;	
						usbsendbuf[5] = 0x08;
					}else{
						csendlen = 14;	
						usbsendbuf[5] = 0x04;
					}
//					usbsendbuf[6] = USB_Recive_Buffer[5]*2;
					
//					if(usbsendbuf[5]<= 16)
//					{
						for(i = 0; i < usbsendbuf[5]; i++)
						{
//							if(i<8)
//							{
							if(SYSPAR.version == 0)
							{
								if(CurrentTemp[i] == 0x7fff)
								{
									usbsendbuf[6+i*2] = 0x7F;
									usbsendbuf[7+i*2] = 0xFF;
								}else{
									usbsendbuf[6+i*2] = (u8)(DispTemp[i]>> 8);
									usbsendbuf[7+i*2] = (u8)(DispTemp[i]);
								}
							}else{
								if(CurrentTemp[i*2] == 0x7fff)
								{
									usbsendbuf[6+i*2] = 0x7F;
									usbsendbuf[7+i*2] = 0xFF;
								}else{
									usbsendbuf[6+i*2] = (u8)(DispTemp[i]>> 8);
									usbsendbuf[7+i*2] = (u8)(DispTemp[i]);
								}
							}
//							}else{
//								usbsendbuf[6+i*2] = 0xFF;
//								usbsendbuf[7+i*2] = 0xFF;
//							}

						}
						for(i = 0;i < csendlen; i++)
						{
							csend[i] = usbsendbuf[i];
						}
						sendcrc = CRC16(csend,csendlen);
						usbsendbuf[6+(usbsendbuf[5])*2] = (u8)(sendcrc >> 8);
						usbsendbuf[7+(usbsendbuf[5])*2] = (u8)(sendcrc);

						USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbsendbuf, 64);//数据回显						
//					
				}else if(USB_Recive_Buffer[2] == 0xC0 && USB_Recive_Buffer[3] == 0x00){//读取时间
					
					csendlen = 15;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);		
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度

//					usbreadtime = 1;
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = USB_Recive_Buffer[2];
					usbsendbuf[3] = USB_Recive_Buffer[3];
					usbsendbuf[4] = USB_Recive_Buffer[4];
					usbsendbuf[5] = USB_Recive_Buffer[5];
					usbsendbuf[6] = 0x08;
					usbsendbuf[7] = (u8)((2000+GetDate.Year)>>8);
					usbsendbuf[8] = (u8)(GetDate.Year);
					usbsendbuf[9] = (u8)(GetDate.Month);
					usbsendbuf[10] = (u8)(GetDate.Date);
					usbsendbuf[11] = (u8)(GetTime.Hours);
					usbsendbuf[12] = (u8)(GetTime.Minutes);
					usbsendbuf[13] = (u8)(GetTime.Seconds);
					usbsendbuf[14] = 0;
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[15] = (u8)(sendcrc >> 8);
					usbsendbuf[16] = (u8)(sendcrc);
					for(i = 17; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					
					DelayUs(200);
					USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbsendbuf, 64);//数据回显
				}else if(USB_Recive_Buffer[2] == 0xC0 && USB_Recive_Buffer[3] == 0x20){//读取传感器类型
					
					csendlen = 47;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = USB_Recive_Buffer[2];
					usbsendbuf[3] = USB_Recive_Buffer[3];
					usbsendbuf[4] = USB_Recive_Buffer[4];
					usbsendbuf[5] = USB_Recive_Buffer[5];
					usbsendbuf[6] = 0x28;
					for(i=0;i<40;i++)
					{
						if(i < 8)
						{
							usbsendbuf[i+7]=SYSPAR.SensorType[0];
						}else{
							usbsendbuf[i+7] = 0;
						}
					}

//					usbsendbuf[8] = TCTYPE;
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[47] = (u8)(sendcrc >> 8);
					usbsendbuf[48] = (u8)(sendcrc);
//					for(i = 11; i < 64 ; i++)
//					{
//						usbsendbuf[i] = 0;
//					}
					
					USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbsendbuf, 64);//数据回显
				}else if(USB_Recive_Buffer[2] == 0xC0 && USB_Recive_Buffer[3] == 0x10){//读取仪器状态
					
					csendlen = 9;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = USB_Recive_Buffer[2];
					usbsendbuf[3] = USB_Recive_Buffer[3];
					usbsendbuf[4] = USB_Recive_Buffer[4];
					usbsendbuf[5] = USB_Recive_Buffer[5];
					usbsendbuf[6] = 0x02;
					usbsendbuf[7] = 0x00;
					usbsendbuf[8] = 1;
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[9] = (u8)(sendcrc >> 8);
					usbsendbuf[10] = (u8)(sendcrc);
					for(i = 11; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbsendbuf, 64);//数据回显
				}else if(USB_Recive_Buffer[2] == 0x01 && USB_Recive_Buffer[3] == 0x01){//读取上下限
					
					csendlen = 11;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = USB_Recive_Buffer[2];
					usbsendbuf[3] = USB_Recive_Buffer[3];
					usbsendbuf[4] = USB_Recive_Buffer[4];
					usbsendbuf[5] = USB_Recive_Buffer[5];
					usbsendbuf[6] = 0x04;
					usbsendbuf[7] = (u8)((int)(SYSPAR.upper) >> 8);
					usbsendbuf[8] = (u8)((int)(SYSPAR.upper));
					usbsendbuf[9] = (u8)((int)(SYSPAR.lower));
					usbsendbuf[10] = (u8)((int)(SYSPAR.lower));
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[11] = (u8)(sendcrc >> 8);
					usbsendbuf[12] = (u8)(sendcrc);
					for(i = 13; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbsendbuf, 64);//数据回显
				}else if(USB_Recive_Buffer[2] == 0x02 && USB_Recive_Buffer[3] == 0x58){//读取单位
					free(csend);
					csendlen = 9;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = USB_Recive_Buffer[2];
					usbsendbuf[3] = USB_Recive_Buffer[3];
					usbsendbuf[4] = USB_Recive_Buffer[4];
					usbsendbuf[5] = USB_Recive_Buffer[5];
					usbsendbuf[6] = 0x02;
					usbsendbuf[7] = 0x00;
					if(SYSPAR.unit==0)
						usbsendbuf[8] = 5;
					else if(SYSPAR.unit==1)
						usbsendbuf[8] = 1;
					else if(SYSPAR.unit==2)
						usbsendbuf[8] = 4;
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[9] = (u8)(sendcrc >> 8);
					usbsendbuf[10] = (u8)(sendcrc);
					for(i = 11; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbsendbuf, 64);//数据回显
				}else if(USB_Recive_Buffer[2] == 0xEE && USB_Recive_Buffer[3] == 0xEE){//检测连接状态
					
					csendlen = 18;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = USB_Recive_Buffer[2];
					usbsendbuf[3] = USB_Recive_Buffer[3];
					usbsendbuf[4] = USB_Recive_Buffer[4];
					usbsendbuf[5] = USB_Recive_Buffer[5];
					usbsendbuf[6] = 0x4A;
					usbsendbuf[7] = 0x4B;
					usbsendbuf[8] = 0x38;
					usbsendbuf[9] = 0x30;
					if(SYSPAR.version == 0)
						usbsendbuf[10] = 0x38;
					else
						usbsendbuf[10] = 0x34;		
					usbsendbuf[11] = 0x00;
					usbsendbuf[12] = 0x00;
					usbsendbuf[13] = 0x08;
					usbsendbuf[14] = 0x00;
					usbsendbuf[15] = 0x00;
					usbsendbuf[16] = 0x00;
					usbsendbuf[17] = 0x00;

					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[18] = (u8)(sendcrc >> 8);
					usbsendbuf[19] = (u8)(sendcrc);
					for(i = 20; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbsendbuf, 64);//数据回显
					
				}else if(USB_Recive_Buffer[2] == 0x80 && USB_Recive_Buffer[3] == 0x10){//读取电参数
					
					csendlen = 19;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = USB_Recive_Buffer[2];
					usbsendbuf[3] = USB_Recive_Buffer[3];
					usbsendbuf[4] = USB_Recive_Buffer[4];
					usbsendbuf[5] = USB_Recive_Buffer[5];
					usbsendbuf[6] = 0x0C;
//					usbsendbuf[7] = (u8)(voltage >> 8);
//					usbsendbuf[8] = (u8)voltage;
//					usbsendbuf[9] = (u8)(current >> 8);
//					usbsendbuf[10] = (u8)current;
//					usbsendbuf[11] = (u8)(power >> 24);
//					usbsendbuf[12] = (u8)(power >> 16);
//					usbsendbuf[13] = (u8)(power >> 8);
//					usbsendbuf[14] = (u8)power;
//					usbsendbuf[15] = (u8)(frequancy >> 8);
//					usbsendbuf[16] = (u8)frequancy;
//					usbsendbuf[17] = (u8)(PF >> 8);
//					usbsendbuf[18] = (u8)PF;
					usbsendbuf[7] = 0xFF;
					usbsendbuf[8] = 0xFF;
					usbsendbuf[9] = 0xFF;
					usbsendbuf[10] = 0xFF;
					usbsendbuf[11] = 0xFF;
					usbsendbuf[12] = 0xFF;
					usbsendbuf[13] = 0xFF;
					usbsendbuf[14] = 0xFF;
					usbsendbuf[15] = 0xFF;
					usbsendbuf[16] = 0xFF;
					usbsendbuf[17] = 0xFF;
					usbsendbuf[18] = 0xFF;
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[19] = (u8)(sendcrc >> 8);
					usbsendbuf[20] = (u8)(sendcrc);
					
					for(i = 21; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					
					USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbsendbuf, 64);//数据回显
				}
			}
			
		}else if(USB_Recive_Buffer[1] == 0x10){
			free(crec);
			free(csend);
			if(USB_Recive_Buffer[2] == 0xC0 && USB_Recive_Buffer[3] == 0x00)//设置时间
			{
				readcrc = USB_Recive_Buffer[15] << 8|USB_Recive_Buffer[16];
				creclen = 15;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//初始化，每个元素都为零
//				crec[0] = 0x01;
//				crec[1] = 0x10;
//				crec[2] = USB_Recive_Buffer[2];
//				crec[3] = USB_Recive_Buffer[3];
//				crec[4] = USB_Recive_Buffer[4];
//				crec[5] = USB_Recive_Buffer[5];
//				crec[6] = USB_Recive_Buffer[6];
//				crec[7] = USB_Recive_Buffer[7];
//				crec[8] = USB_Recive_Buffer[8];
//				crec[9] = USB_Recive_Buffer[9];
//				crec[10] = USB_Recive_Buffer[10];
//				crec[11] = USB_Recive_Buffer[11];
//				crec[12] = USB_Recive_Buffer[12];
//				crec[13] = USB_Recive_Buffer[13];
//				crec[14] = USB_Recive_Buffer[14];
				for(i = 0; i < creclen;i ++)
				{
					crec[i] = USB_Recive_Buffer[i];
				}
				
//				crcwatch = CRC16(crec,creclen);
//				if(CRC16(crec,creclen) == readcrc)
//				{
//					csendlen = 6;
//					csend = (u8*)malloc(sizeof(u8) * csendlen);				
//					memset(csend, 0, csendlen);//初始化，每个元素都为零
//					
//					usbsendbuf[0] = 0x01;
//					usbsendbuf[1] = 0x10;
//					usbsendbuf[2] = 0xC0;
//					usbsendbuf[3] = 0x00;
//					usbsendbuf[4] = 0x00;
//					usbsendbuf[5] = 0x00;
//					
//					RTC_DateTypeDef RTC_DateStructure;
//					RTC_TimeTypeDef RTC_TimeStructure;
//					
//					YEAR = USB_Recive_Buffer[8];
//					MONTH = USB_Recive_Buffer[9];
//					DATE = USB_Recive_Buffer[10];
//					HOURS =USB_Recive_Buffer[11];
//					MINUTES = USB_Recive_Buffer[12];
//					SECONDS = USB_Recive_Buffer[13];										
//					
//					RTC_DateStructure.RTC_Date = DATE;
//					RTC_DateStructure.RTC_Month = MONTH;         
//					RTC_DateStructure.RTC_Year = YEAR;					
//					RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
//					RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
//					
//					RTC_TimeStructure.RTC_H12 = RTC_H12_AMorPM;
//					RTC_TimeStructure.RTC_Hours = HOURS;        
//					RTC_TimeStructure.RTC_Minutes = MINUTES;      
//					RTC_TimeStructure.RTC_Seconds = SECONDS;      
//					RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
//					RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
//					
//					for(i = 0;i < csendlen; i++)
//					{
//						csend[i] = usbsendbuf[i];
//					}
//					sendcrc = CRC16(csend,csendlen);
//					usbsendbuf[6] = (u8)(sendcrc >> 8);
//					usbsendbuf[7] = (u8)(sendcrc);
//					
//					for(i = 8; i < 64 ; i++)
//					{
//						usbsendbuf[i] = 0;
//					}
//					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显
//				}
			}else if(USB_Recive_Buffer[2] == 0x01){//设置上下限
				readcrc = USB_Recive_Buffer[11] << 8|USB_Recive_Buffer[12];
				creclen = 11;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//初始化，每个元素都为零
//				crec[0] = 0x01;
//				crec[1] = 0x10;
//				crec[2] = USB_Recive_Buffer[2];
//				crec[3] = USB_Recive_Buffer[3];
//				crec[4] = USB_Recive_Buffer[4];
//				crec[5] = USB_Recive_Buffer[5];
//				crec[6] = USB_Recive_Buffer[6];
//				crec[7] = USB_Recive_Buffer[7];
//				crec[8] = USB_Recive_Buffer[8];
//				crec[9] = USB_Recive_Buffer[9];
//				crec[10] = USB_Recive_Buffer[10];
				for(i = 0; i < creclen;i ++)
				{
					crec[i] = USB_Recive_Buffer[i];
				}
//				crcwatch = CRC16(crec,creclen);
				if(CRC16(crec,creclen) == readcrc)
				{
					csendlen = 6;
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x10;
					usbsendbuf[2] = 0x01;
					usbsendbuf[3] = 0x01;
					usbsendbuf[4] = 0x00;
					usbsendbuf[5] = 0x02;
					
					SYSPAR.upper = (USB_Recive_Buffer[7] << 8 | USB_Recive_Buffer[8]);
					SYSPAR.lower = (USB_Recive_Buffer[9] << 8 | USB_Recive_Buffer[10]);
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[6] = (u8)(sendcrc >> 8);
					usbsendbuf[7] = (u8)(sendcrc);
					
					for(i = 8; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbsendbuf, 64);//数据回显
				}
			}else if(USB_Recive_Buffer[2] == 0x02 && USB_Recive_Buffer[3] == 0x58){//设置单位
				readcrc = USB_Recive_Buffer[9] << 8|USB_Recive_Buffer[10];
				creclen = 9;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//初始化，每个元素都为零
//				crec[0] = 0x01;
//				crec[1] = 0x10;
//				crec[2] = USB_Recive_Buffer[2];
//				crec[3] = USB_Recive_Buffer[3];
//				crec[4] = USB_Recive_Buffer[4];
//				crec[5] = USB_Recive_Buffer[5];
//				crec[6] = USB_Recive_Buffer[6];
//				crec[7] = USB_Recive_Buffer[7];
//				crec[8] = USB_Recive_Buffer[8];
				for(i = 0; i < creclen;i ++)
				{
					crec[i] = USB_Recive_Buffer[i];
				}
				
//				crcwatch = CRC16(crec,creclen);
				if(CRC16(crec,creclen) == readcrc)
				{
					csendlen = 6;
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x10;
					usbsendbuf[2] = 0x02;
					usbsendbuf[3] = 0x58;
					usbsendbuf[4] = 0x00;
					usbsendbuf[5] = 0x01;
					
					SYSPAR.unit = USB_Recive_Buffer[8];
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[6] = (u8)(sendcrc >> 8);
					usbsendbuf[7] = (u8)(sendcrc);
					
					for(i = 8; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbsendbuf, 64);//数据回显
				}
			}
		}
	}
}

void Brightness(void)
{
	switch(SYSPAR.brtness)
	{
		case 0:
		{
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 50);
		}break;
		case 1:
		{
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 60);
		}break;
		case 2:
		{
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 70);
		}break;
		case 3:
		{
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 80);
		}break;
		case 4:
		{
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 99);
		}break;
		
		default:break;
	}
	
}



u8 ScanKey(void)
{
	u8 key=0;
//	 keyout(0x0);
	 DelayUs(5);
	 if(!HAL_GPIO_ReadPin(K1_GPIO_Port,K1_Pin))
	 {
			key = 1;
	 }else if(!HAL_GPIO_ReadPin(K20_GPIO_Port,K20_Pin)){
		 key = 20;
	 }else if(!HAL_GPIO_ReadPin(K2_GPIO_Port,K2_Pin)){
			 key = 2;
	 }else if(!HAL_GPIO_ReadPin(K3_GPIO_Port,K3_Pin)){
		 key = 3;
	 }else if(!HAL_GPIO_ReadPin(K4_GPIO_Port,K4_Pin)){
		 key = 4;
	 }else if(!HAL_GPIO_ReadPin(K5_GPIO_Port,K5_Pin)){
		 key = 5;
	 }else if(!HAL_GPIO_ReadPin(K6_GPIO_Port,K6_Pin)){
		 key = 6;
	 }else if(!HAL_GPIO_ReadPin(K7_GPIO_Port,K7_Pin)){
		 key = 7;
	 }else if(!HAL_GPIO_ReadPin(K8_GPIO_Port,K8_Pin)){
		 key = 8;
	 }else if(!HAL_GPIO_ReadPin(K9_GPIO_Port,K9_Pin)){
		 key = 9;
	 }else if(!HAL_GPIO_ReadPin(K10_GPIO_Port,K10_Pin)){
		 key = 10;
	 }else if(!HAL_GPIO_ReadPin(K11_GPIO_Port,K11_Pin)){
		 key = 11;
	 }else if(!HAL_GPIO_ReadPin(K12_GPIO_Port,K12_Pin)){
		 key = 12;
	 }else if(!HAL_GPIO_ReadPin(K13_GPIO_Port,K13_Pin)){
		 key = 13;
	 }else if(!HAL_GPIO_ReadPin(K14_GPIO_Port,K14_Pin)){
		 key = 14;
	 }else if(!HAL_GPIO_ReadPin(K15_GPIO_Port,K15_Pin)){
		 key = 15;
	 }
	return key;
}

//U盘检测
u8 udisk_scan(void)
{
	static u8 res;
	
//	u8 i;
//	char str[64];
	if(usbstatus != UNCONNECTED)
	{
		res = CH376DiskConnect( );
		if( res != USB_INT_SUCCESS )/* 检查U盘是否连接,等待U盘插入,对于SD卡,可以由单片机直接查询SD卡座的插拔状态引脚 */
		{  
//			DrawUdisk1();
			LcdFillRec(259-10,6,267-10,19,BUTTONCOLOR);
			usbstatus = UNCONNECTED;
			fileflag = 0;
			saveok=0;
			return NO_CONNECTION;
		}
	}
//	Delay(200);
	if(usbstatus != CONNECTED)
	{		
		res = CH376DiskMount( );
		if(res == USB_INT_SUCCESS)
		{
//			DrawUdisk2();
			DISP_USB();
			usbstatus = CONNECTED;
			return UDISK_READY;
		}

	}
	return UDISK_NOTREADY;
}

u8 sd_scan(void)
{
	static u8 res;
	
//	u8 i;
//	char str[64];
	if(usbstatus != UNCONNECTED)
	{
		res = HAL_GPIO_ReadPin(SD_CON_GPIO_Port,SD_CON_Pin);
		if( res == 1)/* 检查U盘是否连接,等待U盘插入,对于SD卡,可以由单片机直接查询SD卡座的插拔状态引脚 */
		{  
//			DrawUdisk1();
			LcdFillRec(289,6,297,19,BUTTONCOLOR);
			usbstatus = UNCONNECTED;
			fileflag = 0;
			saveok=0;
			return NO_CONNECTION;
		}
	}
//	Delay(200);
	if(usbstatus != CONNECTED)
	{		
		res = HAL_GPIO_ReadPin(SD_CON_GPIO_Port,SD_CON_Pin);
		if(res == 0)
		{
//			DrawUdisk2();
			DISP_SD();
			usbstatus = CONNECTED;
			return UDISK_READY;
		}

	}
	return UDISK_NOTREADY;
}

uint16_t bat_get_adc(){
    //开启ADC1
  HAL_ADC_Start(&hadc1);
    //等待ADC转换完成，超时为100ms
    HAL_ADC_PollForConversion(&hadc1,100);
    //判断ADC是否转换成功
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1),HAL_ADC_STATE_REG_EOC)){
         //读取值
       return HAL_ADC_GetValue(&hadc1);
    }
    return 0;
}
		
	
void  BZZUER(void)
{
 		BuzzerOn()
		delay_ms(10);	
		BuzzerOff()
}

void UDISK_SAVE(void)
{
	u8 buf[200];
	u8 i;
	static u8 filename[64];
	static u8 TarName[64];
	static u32 udcount;
	saveok=0;

	if(fileflag == 0)
	{
		fileflag = 1;
		udcount = 0;
		sprintf((char *)filename, "/%02d%02d%02d%02d.XLS",GetDate.Month
											,GetDate.Date
											,GetTime.Hours
											,GetTime.Minutes); /* 目标文件名 */
		strcpy((char *)TarName,(char *)filename);
		if(CH376FileCreatePath(TarName) != USB_INT_SUCCESS)//创建文件
		{
			return;
		}
		switch(SYSPAR.SensorType[0])
		{
			case TYPE_T:
			{
				sprintf((char *)buf,"热电偶类型:T-%d",TYPE_T);
			}break;
			case TYPE_K:
			{
				sprintf((char *)buf,"热电偶类型:K-%d",TYPE_K);
			}break;
			case TYPE_J:
			{
				sprintf((char *)buf,"热电偶类型:J-%d",TYPE_J);
			}break;
			case TYPE_N:
			{
				sprintf((char *)buf,"热电偶类型:N-%d",TYPE_N);
			}break;
			case TYPE_E:
			{
				sprintf((char *)buf,"热电偶类型:E-%d",TYPE_E);
			}break;
			case TYPE_S:
			{
				sprintf((char *)buf,"热电偶类型:S-%d",TYPE_S);
			}break;
			case TYPE_R:
			{
				sprintf((char *)buf,"热电偶类型:R-%d",TYPE_R);
			}break;
			case TYPE_B:
			{
				sprintf((char *)buf,"热电偶类型:B-%d",TYPE_B);
			}break;
			case PT100:
			{
				sprintf((char *)buf,"热电偶类型:PT100-%d",PT100);
			}break;
		}
		if(CH376ByteWrite(buf,strlen((const char *)buf), NULL ) != USB_INT_SUCCESS)//写入
		{
			return;
		}
		switch(SYSPAR.unit)
		{
			case 0:
			{
				sprintf((char *)buf,"\t单位:℃-%d\n",5);
			}break;
			case 1:
			{
				sprintf((char *)buf,"\t单位:℉-%d\n",1);
			}break;
			case 2:
			{
				sprintf((char *)buf,"\t单位:K-%d\n",4);
			}break;
		}
		if(CH376ByteWrite(buf,strlen((const char *)buf), NULL ) != USB_INT_SUCCESS)//写入
		{
			return;
		}
		if(CH376ByteLocate(0xFFFFFFFF) != USB_INT_SUCCESS)
		{
			return;
		}
		sprintf((char *)buf,"\t日期\t时间\t通道1\t通道2\t通道3\t通道4\t通道5\t通道6\t通道7\t通道8");
		if(CH376ByteWrite(buf,strlen((const char *)buf), NULL ) != USB_INT_SUCCESS)//写入
		{
			return;
		}
		if(CH376FileClose(TRUE) != USB_INT_SUCCESS)//关闭文件
		{
			return;
		}
	}
	delay_us(100);
	strcpy((char *)TarName,(char *)filename);
	if(CH376FileOpenPath(TarName) != USB_INT_SUCCESS)//打开文件
	{
		return;
	}
	if(CH376ByteLocate(0xFFFFFFFF) != USB_INT_SUCCESS)
	{
		return;
	}
	udcount++;
	sprintf((char *)buf, "\n%d\t20%02d-%02d-%02d\t%02d:%02d:%02d"
											,udcount
											,GetDate.Year
											,GetDate.Month
											,GetDate.Date
											,GetTime.Hours
											,GetTime.Minutes
											,GetTime.Seconds
											); /* 目标文件名 */
	if(CH376ByteWrite(buf,strlen((const char *)buf), NULL ) != USB_INT_SUCCESS)//写入
	{
		return;
	}
	if(SYSPAR.version == 0)//808
	{
		for(i=0;i<8;i++)
		{
			if(CurrentTemp[i] == 0x7fff)
			{
				strcpy((char *)buf,"\tN/A");
			}else{
				sprintf((char *)buf,"\t%.1f",((double)DispTemp[i])/10);
			}
			if(CH376ByteWrite(buf,strlen((const char *)buf), NULL ) != USB_INT_SUCCESS)//写入
			{
				return;
			}
		}
	}else{
		for(i=0;i<4;i++)
		{
			if(CurrentTemp[i*2] == 0x7fff)
			{
				strcpy((char *)buf,"\tN/A");
			}else{
				sprintf((char *)buf,"\t%.1f",((double)DispTemp[i])/10);
			}
			if(CH376ByteWrite(buf,strlen((const char *)buf), NULL ) != USB_INT_SUCCESS)//写入
			{
				return;
			}
		}
//		for(i=0;i<4;i++)
//		{
//			strcpy((char *)buf,"\tN/A");
//			if(CH376ByteWrite(buf,strlen((const char *)buf), NULL ) != USB_INT_SUCCESS)//写入
//			{
//				return;
//			}
//		}
	}
	if(CH376FileClose(TRUE) != USB_INT_SUCCESS)//关闭文件
	{
		return;
	}
	saveok=1;
}
 
//===========================================================================
//函数名: GetDayOfMonth
//功能  : 根据年月，获取当月的天数并返回
//参数  : uint8_t year, uint8_t month
//返回值: uint8_t
//===========================================================================
uint8_t GetDayOfMonth(uint8_t year, uint8_t month)
{
    uint8_t DayOfMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
 
    if (month == 2)
    {
        if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)))
            return 29;
        else
            return 28;
    }
    else
        return DayOfMonth[month - 1];
}

//===========================================================================
//函数名: GetDiffDate(uint32_t StartDate, uint32_t EndDate)
//功能  : 计算两个日期之间的天数并返回
//参数  : RTC_DateTypeDef StartDate, RTC_DateTypeDef EndDate
//返回值: uint32_t
//===========================================================================
uint32_t GetDiffDate(RTC_DateTypeDef StartDate, RTC_DateTypeDef EndDate)
{
    uint32_t DayDiff = 0; // 两个日期的天数差
 
    while (1)
    {
        if ((StartDate.Year == EndDate.Year) && (StartDate.Month == EndDate.Month))
        {
            DayDiff += EndDate.Date - StartDate.Date;
            break;
        }
        else
        {
            DayDiff += EndDate.Date;
            EndDate.Month--;
            if (EndDate.Month == 0)
            {
                EndDate.Month = 12;
                EndDate.Year--;
            }
            EndDate.Date = GetDayOfMonth(EndDate.Year, EndDate.Month);
        }
    }
 
    return DayDiff;
}
 
//===========================================================================
//函数名: CalculateDate
//功能  : 根据起始日期和天数，计算之后的日期并返回
//参数  : RTC_DateTypeDef FromDate, uint32_t DayDiff
//返回值: RTC_DateTypeDef
//===========================================================================
RTC_DateTypeDef CalculateDate(RTC_DateTypeDef FromDate, uint32_t DayDiff)
{
    uint16_t day = 0;
 
    while (1)
    {
        day = GetDayOfMonth(FromDate.Year, FromDate.Month);
        if (FromDate.Date + DayDiff > day)
        {
            DayDiff -= day - FromDate.Date;
            FromDate.Month++;
            FromDate.Date = 0;
 
            if (FromDate.Month == 13)
            {
                FromDate.Month = 1;
                FromDate.Year++;
            }
        }
        else
        {
            FromDate.Date += DayDiff;
            break;
        }
    }
 
    FromDate.WeekDay = DateBase.WeekDay + (DayDiff % 7);
    if (FromDate.WeekDay >= 7)
        FromDate.WeekDay -= 7;
 
    return FromDate;
}
 


//===========================================================================
//函数名: RTC_Set_DateTimeCounter
//功能  : 设置RTC时间（将时间转化为时间戳保存到RTC中）
//参数  :
//返回值:
//===========================================================================
void RTC_Set_DateTimeCounter(RTC_DateTypeDef *sDate, RTC_TimeTypeDef *sTime)
{
    uint32_t timecounter = (uint32_t)sTime->Hours * 3600 + (uint32_t)sTime->Minutes * 60 + sTime->Seconds + GetDiffDate(DateBase, *sDate) * 3600 * 24;
    RTC_WriteTimeCounter(&hrtc, timecounter);
}


//===========================================================================
//函数名: RTC_Get_DateTimeCounter
//功能  : 获取RTC时间（获取存储在计数器中的时间戳，再将其转化为时间）
//参数  :
//返回值:
//===========================================================================
void RTC_Get_DateTimeCounter(RTC_DateTypeDef *sDate, RTC_TimeTypeDef *sTime)
{
    uint32_t timecounter = RTC_ReadTimeCounter(&hrtc);
    uint32_t SecOfToday = timecounter % (3600 * 24);
    uint32_t DiffDay = timecounter / (3600 * 24);
 
    sTime->Hours = SecOfToday / 3600;
    sTime->Minutes = SecOfToday % 3600 / 60;
    sTime->Seconds = SecOfToday % 60;
 
    *sDate = CalculateDate(DateBase, DiffDay);
}

void SaveTime(void)
{
//	DateBuf.Year=23;
//	DateBuf.Month=11;
//	DateBuf.Date=2;
//	TimeBuf.Hours=15;
//	TimeBuf.Minutes=55;
//	TimeBuf.Seconds=31;
	RTC_Set_DateTimeCounter(&DateBuf, &TimeBuf);
//  HAL_RTC_SetTime(&hrtc, &TimeBuf, RTC_FORMAT_BIN);
//	HAL_RTC_SetDate(&hrtc, &DateBuf, RTC_FORMAT_BIN);
//  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x0102);
}
/* USER CODE END 0 */


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
 {
  /* USER CODE BEGIN 1 */
	u8 ret,bat_c=0,usbdect=0,adcount=0,udiscount=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
 
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI3_Init();
  MX_FSMC_Init();
  MX_TIM3_Init();
//  MX_RTC_Init();
  MX_TIM2_Init();
  MX_SPI2_Init();
  MX_USB_DEVICE_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
	delay_init(48);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
	delay_ms(5);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	
	FLASH_Init();
  BuzzerOn() 	
  ad7124_init();	
//	delay_init(48);
	RdSysPara(&SYSPAR);
	RdDevPara(&LinearCoeff);
	SYSPARCOMP();
	ret=DS18B20_Init();
	delay_ms(200);
	BuzzerOff()
	MX_RTC_Init();
//	SYSPAR.saveset=1;
//	delay_ms(120);
	
//	while(ReadId() != 0x9341)
//	{
//		screen_init();
//		delay_ms(10);
//	}
	Init_CH376();
//  LcdClear(BABYYELLOW);
	pageflag = PAGE_MEAS;
	displayflag=1;
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	Brightness();
	screen_init();
	DISP_POWERON();
	DISP_TEST();
	delay_ms(100);
//	Savetest(testdata);
//	//SaveSysPara(SYSPAR);
	Test.f_run=RUN_t;
	KeyFlag=0;
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//		DISP_HID();
//		DISP_USB();
//		DISP_SD();
		newkey=ScanKey();
		if(newkey!=oldkey) 
			key_trg=1;
//		else 
//		{
		if(key_trg==1) 
		{
			KeyNum=newkey;
			if(KeyNum != 0)	
				KeyFlag=1;
		}
		key_trg=0;
//		}
		oldkey=newkey;
		
		DISP_TIME();
		if(displayflag == 1)
		{ 	
			DISP_PAGE();
			displayflag = 0;
		}
		if(moveflag == 1)
		{
			DISP_PAGE_ITEM(pageflag,itempos);
			moveflag= 0;
		}
		
		if(TOUCH_SCAN())
		{
			TOUCH_HANDLE(TouchData.lcdx,240-TouchData.lcdy);
		}	
		if(KeyFlag)
		{
			KEY_HANDLE(KeyNum);
		}
		if(Test.f_run==RUN_t && adcount > speedset[SYSPAR.speed])
		{
//			gdtest();
			ad_pro(Test.ch,SYSPAR.SensorType[0]);
			adcount=0;
		}else{
			adcount++;
		}
		if(pageflag == PAGE_MEAS)
		{
			DISP_TEMP();
		}else if(pageflag == PAGE_CAL){
			DISP_ENVIROMENT_TEMP();
		}

		if(Test.f_run==CAL_t)
		{ 	
	  	if(calflag != 0)
			{
				cal_process();
			}
	  }
		if(SYSPAR.saveset == 0)//U盘
		{
			if(usbdect==20)
			{
				udisk_scan();
				usbdect=0;
			}else{
				usbdect++;
			}
			
			if(usbstatus == CONNECTED)
			{
				ucount=RTC_ReadTimeCounter(&hrtc);
				if(ucount-ucountold >= SYSPAR.interval)
				{
					teststatus= CH376ReadBlock(inqbuf);
					UDISK_SAVE();
					ucountold=ucount;
				}
	//			usaveflag=0;
			}
				
			if(saveok == 1)
			{
				if(udiscount > 5)
				{
					DISP_USB();
				}else{
					LcdFillRec(259-10,6,267-10,19,BUTTONCOLOR);
				}
				if(udiscount > 10)
				{
					udiscount=0;
				}else{
					udiscount++;
				}
			}
		}else if(SYSPAR.saveset == 1){//SD卡
			sdstatus = HAL_GPIO_ReadPin(SD_CON_GPIO_Port,SD_CON_Pin);
			
//			teststatus=CH376DiskQuery(&sdsize);
//			teststatus=CH376FileOpen("test.txt");
			if(usbdect==20)
			{
				sd_scan();
				usbdect=0;
			}else{
				usbdect++;
			}
			if(usbstatus == CONNECTED)//识别到SD卡
			{
				testres = CH376DiskMount();
				ucount=RTC_ReadTimeCounter(&hrtc);
				if(ucount-ucountold >= SYSPAR.interval)
				{
					
//					teststatus= CH376ReadBlock(inqbuf);
//					teststatus = CH376FileCreate("TEST.TXT");
					UDISK_SAVE();
					ucountold=ucount;
				}
			}
			if(saveok == 1)
			{
				if(udiscount > 5)
				{
					DISP_SD();
				}else{
					LcdFillRec(289,6,297,19,BUTTONCOLOR);
				}
				if(udiscount > 10)
				{
					udiscount=0;
				}else{
					udiscount++;
				}
			}
		}
//		bat_vm=bat_get_adc();
		if(bat_c < 100)
		{
			ADC_Value+=bat_get_adc();
			bat_c++;
		}else{
			bat_c=0;
			ADC_Value/=100;
			battm=Tab_bat(ADC_Value);
			ADC_Value=0;
			if(battm>100)	
			battm=100;
//			if(oldcharge == CHARGE_STATUS)
//			{
//				
//			}else{
				if(CHARGE_STATUS)
				{
					DISP_CHARGE();
				}else{
					DISP_BAT(battm);
				}
//				oldcharge=CHARGE_STATUS;
//			}
		}
		
		if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED && pageflag == PAGE_MEAS)
		{
			DISP_HID();
		}else{
			LcdFillRec(268,5,279,18,BUTTONCOLOR);
		}
		if(UsbHidReceiveComplete)
		{
			UsbDataHandle();
			UsbHidReceiveComplete=0;
		}

		if(autooffflag==1)
		{
			POWER_OFF();
		}
//			CurrentTemp[2]=	 bat_vm;///100;
//			CurrentTemp[3]=	bat_vm%100;
		
//	HAL_Delay(20);
//	  SPI_Flash_ReadID();
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC
                              |RCC_PERIPHCLK_USB;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
	if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x0102)
  {
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
		sTime.Hours = 23;
		sTime.Minutes = 59;
		sTime.Seconds = 50;

//		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
//		{
//			Error_Handler();
//		}
		DateToUpdate.WeekDay = RTC_WEEKDAY_SUNDAY;
		DateToUpdate.Month = RTC_MONTH_MAY;
		DateToUpdate.Date = 1;
		DateToUpdate.Year = 23;

//		if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
//		{
//			Error_Handler();
//		}
  /* USER CODE BEGIN RTC_Init 2 */
		// 将日期和时间换算为时间戳保存到计数器中
    RTC_Set_DateTimeCounter(&DateToUpdate, &sTime);
 
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x0102);
	}
  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi3.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 479;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 100;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 479;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 100;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 99;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */
	
  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, CH2_Pin|CH1_Pin|CH8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SPI1_NSS_Pin|SPI3CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CH7_Pin|BEEP_Pin|SPI2_CS_Pin|CH3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, CH6_Pin|CH5_Pin|CH4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : K20_Pin K1_Pin K8_Pin */
  GPIO_InitStruct.Pin = K20_Pin|K1_Pin|K8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CH2_Pin CH1_Pin CH8_Pin */
  GPIO_InitStruct.Pin = CH2_Pin|CH1_Pin|CH8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : CHARGE_Pin K15_Pin K13_Pin K5_Pin
                           K9_Pin K4_Pin */
  GPIO_InitStruct.Pin = CHARGE_Pin|K15_Pin|K13_Pin|K5_Pin
                          |K9_Pin|K4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : SD_CON_Pin PEN_Pin */
  GPIO_InitStruct.Pin = SD_CON_Pin|PEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
  /*Configure GPIO pins : LCD_RST_Pin SPI3CS_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|SPI3CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : CH376_INT_Pin K11_Pin K7_Pin */
  GPIO_InitStruct.Pin = CH376_INT_Pin|K11_Pin|K7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI1_NSS_Pin */
  GPIO_InitStruct.Pin = SPI1_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPI1_NSS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CH7_Pin BEEP_Pin SPI2_CS_Pin CH3_Pin */
  GPIO_InitStruct.Pin = CH7_Pin|BEEP_Pin|SPI2_CS_Pin|CH3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : K12_Pin K6_Pin K2_Pin K10_Pin */
  GPIO_InitStruct.Pin = K12_Pin|K6_Pin|K2_Pin|K10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : CH6_Pin CH5_Pin CH4_Pin */
  GPIO_InitStruct.Pin = CH6_Pin|CH5_Pin|CH4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PEN_Pin */
  GPIO_InitStruct.Pin = PEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PEN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : K3_Pin K14_Pin */
  GPIO_InitStruct.Pin = K3_Pin|K14_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void)
{

  /* USER CODE BEGIN FSMC_Init 0 */

  /* USER CODE END FSMC_Init 0 */

  FSMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FSMC_Init 1 */

  /* USER CODE END FSMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 0;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 1;
  Timing.BusTurnAroundDuration = 0;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /** Disconnect NADV
  */

  __HAL_AFIO_FSMCNADV_DISCONNECTED();

  /* USER CODE BEGIN FSMC_Init 2 */

  /* USER CODE END FSMC_Init 2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
