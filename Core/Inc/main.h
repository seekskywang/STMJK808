/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD.h"
#include "menu.h"
#include "ZK.h"
#include <stdio.h>
#include "touch.h"
//#include "ad.h"
#include "cal.h"
#include "delay.h"
#include "string.h"
#include "touch.h"
#include <stdlib.h>
#include "ch376.h"
#include "FILESYS.h"
//#include "usbd_customhid.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern u8 KeyNum,KeyFlag;
extern struct SParameter SYSPAR;
extern SPI_HandleTypeDef hspi3;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim3;
extern struct Tes_RUN  Test;
extern struct CalPara LinearCoeff;
extern int16_t CurrentTemp[8];
extern uint8_t tempOVER;
extern u8 calflag;
extern RTC_DateTypeDef GetDate;  //获取日期结构体
extern RTC_TimeTypeDef GetTime;   //获取时间结构体
extern RTC_HandleTypeDef hrtc;
extern u8 inputflag;
extern RTC_DateTypeDef DateBuf;  //获取日期结构体
extern RTC_TimeTypeDef TimeBuf;   //获取时间结构体
//extern int16_t TempOffset[8];
extern int16_t DispTemp[8];
extern u8 usaveflag;
extern char inputbuf[10];
extern u8 autooffflag;
extern union	UU	enir_temp;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void BZZUER(void);
u8 ScanKey(void);
void FLASH_Init(void);
void SaveSysPara(struct SParameter SysPara);
void SaveDevPara(struct  CalPara Para);
void RdSysPara(struct SParameter *SysPara);
void RdDevPara(struct CalPara *Para);
void SYSPARCOMP(void);
void SYSPARRST(void);
void DelayUs(u32 delay_us);
void Brightness(void);
void ad_pro(u8 ch,u8 typ);
void ad7124_init(void);
void proc_send_dw(int ch);
void cal_process(void);
void DISP_INPUT(void);
void DISP_NUM(char num);
void DEL_NUM(void);
void INPUT_CONFIRM(__packed int16_t *data);
void SN_CONFIRM(void);
void gdtest(void);
u16 Tab_bat(u16 bat);
u16 ReadTemperature(void);
unsigned char DS18B20_Init(void);			//初始化DS18B20
float DS18B20_Get_Temp(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(unsigned char dat);//写入一个字节
unsigned char DS18B20_Read_Byte(void);		//读出一个字节
unsigned char DS18B20_Read_Bit(void);		//读出一个位
unsigned char DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20  
void POWER_OFF(void);
void tmp0(void);
void TOUCH_RESET(void);
void RTC_Get_DateTimeCounter(RTC_DateTypeDef *sDate, RTC_TimeTypeDef *sTime);
void SaveTime(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define K20_Pin GPIO_PIN_4
#define K20_GPIO_Port GPIOE
#define K1_Pin GPIO_PIN_5
#define K1_GPIO_Port GPIOE
#define K8_Pin GPIO_PIN_6
#define K8_GPIO_Port GPIOE
#define CH2_Pin GPIO_PIN_1
#define CH2_GPIO_Port GPIOC
#define CH1_Pin GPIO_PIN_2
#define CH1_GPIO_Port GPIOC
#define CHARGE_Pin GPIO_PIN_3
#define CHARGE_GPIO_Port GPIOC
#define LCD_RST_Pin GPIO_PIN_2
#define LCD_RST_GPIO_Port GPIOA
#define CH376_INT_Pin GPIO_PIN_3
#define CH376_INT_GPIO_Port GPIOA
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define CH8_Pin GPIO_PIN_5
#define CH8_GPIO_Port GPIOC
#define CH7_Pin GPIO_PIN_0
#define CH7_GPIO_Port GPIOB
#define K12_Pin GPIO_PIN_1
#define K12_GPIO_Port GPIOB
#define BEEP_Pin GPIO_PIN_11
#define BEEP_GPIO_Port GPIOB
#define SPI2_CS_Pin GPIO_PIN_12
#define SPI2_CS_GPIO_Port GPIOB
#define CH6_Pin GPIO_PIN_12
#define CH6_GPIO_Port GPIOD
#define CH5_Pin GPIO_PIN_13
#define CH5_GPIO_Port GPIOD
#define K15_Pin GPIO_PIN_8
#define K15_GPIO_Port GPIOC
#define K13_Pin GPIO_PIN_9
#define K13_GPIO_Port GPIOC
#define K11_Pin GPIO_PIN_8
#define K11_GPIO_Port GPIOA
#define K7_Pin GPIO_PIN_9
#define K7_GPIO_Port GPIOA
#define PEN_Pin GPIO_PIN_10
#define PEN_GPIO_Port GPIOA
#define SPI3CS_Pin GPIO_PIN_15
#define SPI3CS_GPIO_Port GPIOA
#define K5_Pin GPIO_PIN_10
#define K5_GPIO_Port GPIOC
#define K9_Pin GPIO_PIN_11
#define K9_GPIO_Port GPIOC
#define K4_Pin GPIO_PIN_12
#define K4_GPIO_Port GPIOC
#define K3_Pin GPIO_PIN_2
#define K3_GPIO_Port GPIOD
#define K14_Pin GPIO_PIN_3
#define K14_GPIO_Port GPIOD
#define CH4_Pin GPIO_PIN_6
#define CH4_GPIO_Port GPIOD
#define K6_Pin GPIO_PIN_6
#define K6_GPIO_Port GPIOB
#define CH3_Pin GPIO_PIN_7
#define CH3_GPIO_Port GPIOB
#define K2_Pin GPIO_PIN_8
#define K2_GPIO_Port GPIOB
#define K10_Pin GPIO_PIN_9
#define K10_GPIO_Port GPIOB
#define SD_CON_Pin GPIO_PIN_1
#define SD_CON_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
#define temperature_Pin GPIO_PIN_7
#define temperature_GPIO_Port GPIOC

#define	RUN_t	1
#define	CAL_t	2

#define TYPE_T   1
#define TYPE_K   2
#define TYPE_J   3
#define TYPE_N   4
#define TYPE_E   5
#define TYPE_S   6
#define TYPE_R   7
#define TYPE_B   8
#define PT100   9
#define PT1000  10
#define TYPE_NC  99

#define PAGE_MEAS    1
#define PAGE_SETP    2
#define PAGE_SYST    3
#define PAGE_CAL     4

#define BuzzerOn()       {HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);}
#define BuzzerOff()      {HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);}

#define	DS18B20_DQ_OUT_1	 HAL_GPIO_WritePin(temperature_GPIO_Port, temperature_Pin, GPIO_PIN_SET)
#define	DS18B20_DQ_OUT_0 	 HAL_GPIO_WritePin(temperature_GPIO_Port, temperature_Pin, GPIO_PIN_RESET)
#define	DS18B20_DQ_IN  		 HAL_GPIO_ReadPin(temperature_GPIO_Port, temperature_Pin)  //数据端口	PC7

#define	CHARGE_STATUS  		 HAL_GPIO_ReadPin(CHARGE_GPIO_Port, CHARGE_Pin)

#define MAX_CHANNEL_NUM 8

/*USB状态*/
#define UNKNOWN		     2
#define UNCONNECTED		 0
#define CONNECTED		   1

//USB检测响应
#define NO_CONNECTION				0
#define UDISK_READY					1
#define UDISK_NOTREADY			2
__packed struct SParameter
{
	u8 Dia;
	u8 chnum;
	//测量设置
	u8 SensorType[MAX_CHANNEL_NUM];//热电偶类型
	u8 unit;//单位
	u8 speed;//速率
	u8 alarm;//讯响
	int16_t upper;//上限
	int16_t lower;//下限
	u8 saveset;//存储设置 
	u16 interval;//存储间隔
	//系统设置
	u8 language;//语言
	u8 brtness;//亮度
	u8 dimtime;//降低亮度时间
	u8 offsave;//文件
	u8 autooff;//自动关机
	u8 touch;//触摸屏
	u8 sn[10];//序列号
	//59+3+2
	int16_t xOffset;
  int16_t yOffset; 
  float xFactor;
  float yFactor;
	int16_t TempOffset[8];
	u8 version;//版本:0-808;1-804
	u8 jkflag;//logo
	//71+3+16+2+2
//	u8 id;
	
//	int16_t dis_up[MAX_CHANNEL_NUM];
//	int16_t dis_dw[MAX_CHANNEL_NUM];
//	u8 id;
//	u8 idx;
//	u8 idz;

};

__packed struct PT10s
{
	int32_t Ks;
	int32_t Bt;//64
};

//校准参数
__packed struct CalPara
{
	struct PT10s JKT_[MAX_CHANNEL_NUM];//128
//64
	struct PT10s Pt100[MAX_CHANNEL_NUM];//128
//256
	struct PT10s Pt1000[MAX_CHANNEL_NUM];//128
//384
};

__packed struct DateTime
{
	u8 Year;
	u8 Month;
	u8 Date;
	u8 Hour;
	u8 Minute;
	u8 Second;	
};

__packed struct Tes_RUN
{
	u8	ch;
	u8	f_ch;
	u8	typ;
	u8	ad_bat;
	u8	tmp820;
	u8	tmt;
	u8	f_run;
	u8	f485;
};

union	UU
	{int16_t	adx;
	u16	ax;
	u8	bx[2];
	};
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
