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
	/* ��������Ϊ������� */
	DS18B20_Mode_Out_PP();
	
	DS18B20_DQ_OUT_0;
	/* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
	//Delay_us(750);
	delay_us(750);
	/* �����ڲ�����λ�źź��轫�������� */
	//DS18B20_DATA_OUT(HIGH);
	DS18B20_DQ_OUT_1;
	/*�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������*/
	delay_us(15);
}

/*
 * ���ӻ����������صĴ�������
 * 0���ɹ�
 * 1��ʧ��
 */
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	
	/* ��������Ϊ�������� */
	DS18B20_Mode_IPU();
	
	/* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
	 * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
	 */
	while( DS18B20_DQ_IN&& pulse_time<100 )
	{
		pulse_time++;
		delay_us(1);
	}	
	/* ����100us�󣬴������嶼��û�е���*/
	if( pulse_time >=100 )
		return 1;
	else
		pulse_time = 0;
	
	/* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
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
 * ��DS18B20��ȡһ��bit
 */
uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	/* ��0�Ͷ�1��ʱ������Ҫ����60us */	
	DS18B20_Mode_Out_PP();
	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	DS18B20_DQ_OUT_0;
	delay_us(10);
	
	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	DS18B20_Mode_IPU();
	//Delay_us(2);
	
	if( DS18B20_DQ_IN == SET )
		dat = 1;
	else
		dat = 0;
	
	/* �����ʱ������ο�ʱ��ͼ */
	delay_us(45);
	
	return dat;
}

/*
 * ��DS18B20��һ���ֽڣ���λ����
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
 * дһ���ֽڵ�DS18B20����λ����
 */
void DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* д0��д1��ʱ������Ҫ����60us */
		if (testb)
		{			
			DS18B20_DQ_OUT_0;
			/* 1us < �����ʱ < 15us */
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
			/* 1us < Trec(�ָ�ʱ��) < �����*/
			delay_us(2);
		}
	}
}

void DS18B20_Start(void)
{
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);		/* ���� ROM */
	DS18B20_Write_Byte(0X44);		/* ��ʼת�� */
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
 * �洢���¶���16 λ�Ĵ�������չ�Ķ����Ʋ�����ʽ
 * ��������12λ�ֱ���ʱ������5������λ��7������λ��4��С��λ
 *
 *         |---------����----------|-----С�� �ֱ��� 1/(2^4)=0.0625----|
 * ���ֽ�  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----����λ��0->��  1->��-------|-----------����-----------|
 * ���ֽ�  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * �¶� = ����λ + ���� + С��*0.0625
 */
float DS18B20_Get_Temp(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
	DS18B20_Write_Byte(0X44);				/* ��ʼת�� */
	
	DS18B20_Rst();
  DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
  DS18B20_Write_Byte(0XBE);				/* ���¶�ֵ */
	
	tplsb = DS18B20_Read_Byte();		 
	tpmsb = DS18B20_Read_Byte(); 
	
	//s_tem = tpmsb<<8;
	//s_tem = s_tem | tplsb;

//	if( s_tem < 0 )		/* ���¶� */
//		f_tem = (~s_tem+1) * 0.0625;	
	//else
	//	f_tem = s_tem * 0.0625;
	
		s_tem = tpmsb<<8;
		s_tem = s_tem + tplsb;
		if(( s_tem&0xF800)==0xF800  )		/* ���¶� */
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
	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
	DS18B20_Write_Byte(0X44);				/* ��ʼת�� */
	
	DS18B20_Rst();
  DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
  DS18B20_Write_Byte(0XBE);				/* ���¶�ֵ */
	
	
	enir_temp.bx[0]=DS18B20_Read_Byte();
	enir_temp.bx[1]=DS18B20_Read_Byte();
	enir_temp.adx*=5;
	enir_temp.adx>>=3;

}

////******�������ܣ���ʼ��ADCһ��ͨ��,�������ĸ�ͨ���ɺ�����Get_myAdc()ȷ��
////******�����������
////******�����������
//void MYAD_Init(void)
//{	GPIO_InitTypeDef GPIO_Initstruct;
//	ADC_InitTypeDef AD_InitStruct;
//	//����ADCת���ķ�Ƶ
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);			//APB2����Ϊ72MHZ��������8��Ƶ��Ϊ9MHZ��ADCת��ʱ�Ӳ��ܳ���14MHZ
//	//ʹ�������ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1,ENABLE);
//	//�������ţ�������ADCʱ��

//	//CCCC:MOS_R--PC3;
//	GPIO_Initstruct.GPIO_Pin =  GPIO_Pin_0;
//	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOC,&GPIO_Initstruct);
//	
//	ADC_DeInit(ADC1);		//��ADC1ͨ���������ó�Ϊȱʡֵ
//	
//	AD_InitStruct.ADC_Mode = ADC_Mode_Independent;	//AD����ģʽ�����ڶ���ģʽ
//	AD_InitStruct.ADC_NbrOfChannel = 1;				//���й��������ͨ����
//	AD_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	//�����Ҷ���
//	AD_InitStruct.ADC_ScanConvMode = DISABLE;		//����ת�������ڵ�ͨ��ģʽ
//	AD_InitStruct.ADC_ContinuousConvMode = DISABLE;	//ADת�������ڵ���ת��ģʽ
//	AD_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת������������������ⲿ��������
//	ADC_Init(ADC1,&AD_InitStruct);
//	
//	ADC_Cmd(ADC1,ENABLE);		
//	
//	ADC_ResetCalibration(ADC1);			//ʹ�ܸ�λУ׼
//	while(ADC_GetCalibrationStatus(ADC1));	//�ȴ���λУ׼�Ľ���
//	ADC_StartCalibration(ADC1);					//����ADC1��У׼
//	while(ADC_GetCalibrationStatus(ADC1));	//�ȴ���λУ׼�Ľ���
//}
////*******�������ܣ���ȡAD�����Ľ��
////*******�����������
////*******���������16���Ƶ�ת�����
//u16 Get_myAdc(u8 ch)
//{	float value;
//	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_1Cycles5);	//�ڹ�������ͨ��������ͨ�������У�����ʱ��1us
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);					//���������ADת��
//	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));		//�ȴ�ת������
//	value=ADC_GetConversionValue(ADC1);
//	//value=value*3300/4095;
//	return value;						//�������һ��ADC1����ת���Ľ��
//}
