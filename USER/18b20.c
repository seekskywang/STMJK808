#include "main.h"

extern union	UU enir_temp;

void DS18B20_Mode_Out_PP(void)
{
    GPIO_InitTypeDef myGPIO_InitStruct;
    myGPIO_InitStruct.Pin = temperature_Pin;
    myGPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    myGPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(temperature_GPIO_Port, &myGPIO_InitStruct);
}

void DS18B20_Mode_IPU(void)
{
    GPIO_InitTypeDef myGPIO_InitStruct;
    myGPIO_InitStruct.Pin = temperature_Pin;
    myGPIO_InitStruct.Pull = GPIO_PULLUP;
    myGPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    myGPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(temperature_GPIO_Port, &myGPIO_InitStruct);
}

void DS18B20_Rst(void)
{
	/* 主机设置为推挽输出 */
	DS18B20_Mode_Out_PP();
	
	DS18B20_DQ_OUT_0;
	/* 主机至少产生480us的低电平复位信号 */
	//Delay_us(750);
	delay_us(750);
	/* 主机在产生复位信号后，需将总线拉高 */
	//DS18B20_DATA_OUT(HIGH);
	DS18B20_DQ_OUT_1;
	/*从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲*/
	delay_us(15);
}

/*
 * 检测从机给主机返回的存在脉冲
 * 0：成功
 * 1：失败
 */
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	
	/* 主机设置为上拉输入 */
	DS18B20_Mode_IPU();
	
	/* 等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号 
	 * 如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
	 */
	while( DS18B20_DQ_IN&& pulse_time<100 )
	{
		pulse_time++;
		delay_us(1);
	}	
	/* 经过100us后，存在脉冲都还没有到来*/
	if( pulse_time >=100 )
		return 1;
	else
		pulse_time = 0;
	
	/* 存在脉冲到来，且存在的时间不能超过240us */
	while( !DS18B20_DQ_IN&& pulse_time<240 )
	{
		pulse_time++;
		delay_us(1);
	}	
	if( pulse_time >=240 )
		return 1;
	else
		return 0;
}


/*
 * 从DS18B20读取一个bit
 */
uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	/* 读0和读1的时间至少要大于60us */	
	DS18B20_Mode_Out_PP();
	/* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
	DS18B20_DQ_OUT_0;
	delay_us(10);
	
	/* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
	DS18B20_Mode_IPU();
	//Delay_us(2);
	
	if( DS18B20_DQ_IN == SET )
		dat = 1;
	else
		dat = 0;
	
	/* 这个延时参数请参考时序图 */
	delay_us(45);
	
	return dat;
}

/*
 * 从DS18B20读一个字节，低位先行
 */
uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

/*
 * 写一个字节到DS18B20，低位先行
 */
void DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* 写0和写1的时间至少要大于60us */
		if (testb)
		{			
			DS18B20_DQ_OUT_0;
			/* 1us < 这个延时 < 15us */
			delay_us(8);
			
			DS18B20_DQ_OUT_1;
			delay_us(58);
		}		
		else
		{			
			DS18B20_DQ_OUT_0;
			/* 60us < Tx 0 < 120us */
			delay_us(70);
			
			DS18B20_DQ_OUT_1;			
			/* 1us < Trec(恢复时间) < 无穷大*/
			delay_us(2);
		}
	}
}

void DS18B20_Start(void)
{
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);		/* 跳过 ROM */
	DS18B20_Write_Byte(0X44);		/* 开始转换 */
}

uint8_t DS18B20_Init(void)
{
	//DS18B20_GPIO_Config();
	void DS18B20_Mode_Out_PP();
	DS18B20_DQ_OUT_1;
	DS18B20_Rst();
	
	return DS18B20_Presence();
}

/*
 * 存储的温度是16 位的带符号扩展的二进制补码形式
 * 当工作在12位分辨率时，其中5个符号位，7个整数位，4个小数位
 *
 *         |---------整数----------|-----小数 分辨率 1/(2^4)=0.0625----|
 * 低字节  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----符号位：0->正  1->负-------|-----------整数-----------|
 * 高字节  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * 温度 = 符号位 + 整数 + 小数*0.0625
 */
float DS18B20_Get_Temp(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);				/* 跳过 ROM */
	DS18B20_Write_Byte(0X44);				/* 开始转换 */
	
	DS18B20_Rst();
  DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);				/* 跳过 ROM */
  DS18B20_Write_Byte(0XBE);				/* 读温度值 */
	
	tplsb = DS18B20_Read_Byte();		 
	tpmsb = DS18B20_Read_Byte(); 
	
	//s_tem = tpmsb<<8;
	//s_tem = s_tem | tplsb;

//	if( s_tem < 0 )		/* 负温度 */
//		f_tem = (~s_tem+1) * 0.0625;	
	//else
	//	f_tem = s_tem * 0.0625;
	
		s_tem = tpmsb<<8;
		s_tem = s_tem + tplsb;
		if(( s_tem&0xF800)==0xF800  )		/* 负温度 */
		{
		f_tem = (~s_tem+1) * (-0.0625);	
		}
	else
	{
		f_tem = s_tem * 0.0625;
	
	}
	return f_tem; 	
}

void SET820(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);}
void CLR820(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);}

void SET820IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);
}

u8 reset820(void)
{u8 by;
	   SET820();
	   CLR820();
	   DelayUs(455);//500
	   SET820();
	   DelayUs(45);
	   SET820IN();
	   by=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7);
	   if(!by)
	   DelayUs(430);
	   return(by);
}

//u8 reset820(void)
//{u8 by;
//	   SET820();
//		 DelayUs(50);
//	   CLR820();
//	   DelayUs(500);//500
//	   SET820();
//	   DelayUs(45);
//	   SET820IN();
//		 by=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7);
//		 if(by)
//		 {
//		 }
//		 SET820();
//	   
////	   by=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7);
////	   if(!by)
////	   DelayUs(430);
////	   return(by);
//}

void wr_com(u8 com)
{   u8 y;
   for(y=0;y<8;y++)
   {SET820();  	
	DelayUs(5);
	CLR820();
	DelayUs(7);
	if(com&1)	 SET820();
	else		 CLR820();
	DelayUs(18);
	com>>=1;
   }
   CLR820();
}

//void wr_com(u8 com)
//{   u8 y;
//   for(y=0;y<8;y++)
//   {
//	   CLR820();
//	   DelayUs(3);
//	   if(com&1)	 
//		   SET820();
//	   com>>=1;
//	   DelayUs(60);
//		 SET820();
//		 DelayUs(2);
//   }
//}

u8 read820(void)
{u8 tmp,i;
	 for(i=0;i<8;i++)
	 {tmp>>=1;
	  CLR820();
	  DelayUs(5);
	  SET820();
	  SET820IN();
		DelayUs(7);
	 if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7))
	 tmp=tmp|0x80;
	  DelayUs(18);
	  SET820();
	 }
	return(tmp);
} 

//u8 read820(void)
//{u8 tmp,i;
//	 for(i=0;i<8;i++)
//	 {tmp>>=1;
//	  CLR820();
//	  DelayUs(5);
//	  SET820();
//	  SET820IN();
//		DelayUs(7);
//	 if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7))
//	 tmp=tmp|0x80;
//	  DelayUs(18);
////	  SET820();
//	 }
//	return(tmp);
//} 

u16 ReadTemperature(void)
{
    float TPdat_f;
    u16 TPdat;    
    u8 TPH;                           //?????????
    u8 TPL;                           //?????????

    reset820();                //????
    wr_com(0xCC);        //??ROM??
    wr_com(0x44);        //??????
		HAL_Delay(700);
//    while (!DQ);                    //??????
    reset820();                //????
    wr_com(0xCC);        //??ROM??
    wr_com(0xBE);        //????????
    TPL = read820();       //??????
    TPH = read820();       //??????
    TPdat=TPH*256+TPL;
    if(TPdat>61440)
    {
       TPdat=(~TPdat)+1;
    }else{ }
    TPdat_f=TPdat*0.0625;
    TPdat=TPdat_f*10;
		
		
    return TPdat;
}

extern uint8_t outad[]; 
void tmp0(void)
{	  
	
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);				/* 跳过 ROM */
	DS18B20_Write_Byte(0X44);				/* 开始转换 */
	
	DS18B20_Rst();
  DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);				/* 跳过 ROM */
  DS18B20_Write_Byte(0XBE);				/* 读温度值 */
	
	
	enir_temp.bx[0]=DS18B20_Read_Byte();
	enir_temp.bx[1]=DS18B20_Read_Byte();
	enir_temp.adx*=5;
	enir_temp.adx>>=3;

}

////******函数功能：初始化ADC一个通道,具体是哪个通道由函数：Get_myAdc()确定
////******输入参数：无
////******输出参数：无
//void MYAD_Init(void)
//{	GPIO_InitTypeDef GPIO_Initstruct;
//	ADC_InitTypeDef AD_InitStruct;
//	//配置ADC转换的分频
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);			//APB2总线为72MHZ，这里用8分频成为9MHZ，ADC转换时钟不能超过14MHZ
//	//使能外设的时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1,ENABLE);
//	//配置引脚，并开启ADC时钟

//	//CCCC:MOS_R--PC3;
//	GPIO_Initstruct.GPIO_Pin =  GPIO_Pin_0;
//	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOC,&GPIO_Initstruct);
//	
//	ADC_DeInit(ADC1);		//将ADC1通道参数设置成为缺省值
//	
//	AD_InitStruct.ADC_Mode = ADC_Mode_Independent;	//AD采样模式工作在独立模式
//	AD_InitStruct.ADC_NbrOfChannel = 1;				//进行规则采样的通道数
//	AD_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	//数据右对齐
//	AD_InitStruct.ADC_ScanConvMode = DISABLE;		//单次转换工作在单通道模式
//	AD_InitStruct.ADC_ContinuousConvMode = DISABLE;	//AD转换工作在单次转换模式
//	AD_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件启动而不是外部触发启动
//	ADC_Init(ADC1,&AD_InitStruct);
//	
//	ADC_Cmd(ADC1,ENABLE);		
//	
//	ADC_ResetCalibration(ADC1);			//使能复位校准
//	while(ADC_GetCalibrationStatus(ADC1));	//等待复位校准的结束
//	ADC_StartCalibration(ADC1);					//开启ADC1的校准
//	while(ADC_GetCalibrationStatus(ADC1));	//等待复位校准的结束
//}
////*******函数功能：获取AD采样的结果
////*******输入参数：无
////*******输出参数：16进制的转换结果
//u16 Get_myAdc(u8 ch)
//{	float value;
//	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_1Cycles5);	//在规则组中通道中设置通道，序列，采样时间1us
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);					//用软件启动AD转换
//	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));		//等待转换结束
//	value=ADC_GetConversionValue(ADC1);
//	//value=value*3300/4095;
//	return value;						//返回最近一次ADC1规则转换的结果
//}
