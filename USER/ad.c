#include "main.h"
#include "ad.h"
#define ADC1_DR_Address ((uint32_t)0x4001244C)
uint8_t left_right[MAXCHN];
u32 check;

void dly7124(void)
{	u8 u;
for(u=0;u<6;u++)		  u=u=u;
}
void SpiTxByte(u8 dat) 
{ 
	HAL_SPI_Transmit(&hspi2,&dat,1,3);
}

u8 SpiRxByte(void)
{	
	u8 tmp=0;
	HAL_SPI_Receive(&hspi2,&tmp,1,3);
	return tmp;
}

static void ad7124_regw(uint8_t reg, uint32_t dat) {
  switch (reg) {
  case REG_CONTROL:
  case REG_IOCTRL2:
  case REG_CHANNEL0:
  case REG_CONFIG0:
    AD_CS0();
    dat &= 0xFFFFu;
    SpiTxByte(reg);
    SpiTxByte(dat >> 8);
    SpiTxByte(dat & 0xFF);
    AD_CS1();
    return;

  case REG_IOCTRL1:
  case REG_ERR_EN:
  case REG_FILTER0:
  case REG_OFFSET0:
  case REG_FULLCB0:
    AD_CS0();
    dat &= 0xFFFFFFu;
    SpiTxByte(reg);
    SpiTxByte((dat & 0xFF0000u) >> 16);
    SpiTxByte((dat & 0xFF00u) >> 8);
    SpiTxByte((dat & 0xFFu));
    AD_CS1();
    return;
  }
}

static uint32_t ad7124_regr(uint8_t reg) {
  uint32_t dat = 0;
   AD_CS0();
  switch (reg) {
  case REG_STATUS:
  case REG_ID:
  case REG_MCLKCNT:
    SpiTxByte(reg | 0x40);
    dat = SpiRxByte();
    break;

  case REG_CONTROL:
  case REG_IOCTRL2:
  case REG_CHANNEL0:
  case REG_CONFIG0:
    SpiTxByte(reg | 0x40);
    dat = SpiRxByte();
    dat <<= 8;
    dat |= SpiRxByte();
    break;

  case REG_DATA:
  case REG_IOCTRL1:
  case REG_ERROR:
  case REG_ERR_EN:
  case REG_FILTER0:
  case REG_OFFSET0:
  case REG_FULLCB0:
    SpiTxByte(reg | 0x40);
    dat = SpiRxByte();
    dat <<= 8;
    dat |= SpiRxByte();
    dat <<= 8;
    dat |= SpiRxByte();
    break;

//  default: break;
  }	AD_CS1();
  return dat;
}

void gdtest(void)
{
	// 检查SPI通讯是否正常
	ad7124_regw( REG_ERR_EN, 0x180058u);
  check = ad7124_regr(REG_ERR_EN);
}


void ad7124_init(void){
		u8 i;
		AD_CS0();
		for(i=0;i<8;i++) SpiTxByte(0xff);
		AD_CS1();

		reg_control.all        = 0; // 清空CONTROL
//		reg_control.clk_sel    = 0; // 内部614.4kHz时钟, CLK引脚不提供内部时钟
  reg_control.mode       = 4; // 空闲模式
  reg_control.power_mode = 2; // 全功耗模式
  reg_control.ref_en     = 1; // 内部基准电压使能
  reg_control.ncs_en     = 1; // 使能CS控制
  ad7124_regw(REG_CONTROL, reg_control.all);
		ad7124_regw(REG_CONTROL,0x390);		//reg=1
}
void ad7124_reset(void){
		u8 i;
		AD_CS0();
		for(i=0;i<8;i++) SpiTxByte(0xff);
		AD_CS1();
		DelayUs(500);
//		ad7124_regw(REG_CONTROL,0x390);		  //reg=1
		reg_control.all  = 0; // 清空CONTROL
		reg_control.ncs_en     = 1; // 使能CS控制
		reg_control.ref_en     = 1; // 内部基准电压使能
		reg_control.power_mode = 2; // 全功耗模式
		reg_control.mode       = 4; // 空闲模式
//  reg_control.clk_sel    = 0; // 内部614.4kHz时钟, CLK引脚不提供内部时钟
  ad7124_regw(REG_CONTROL, reg_control.all); 
		DelayUs(500);
}
// cfg: 通道配置参数
// fs : 通道采样速度
// pga: 通道增益
void ad7124_config(const Ad7124Config *cfg,uint8_t pga,uint16_t fs){
	reg_ioctrl1.all      = 0; // 清空IO_CONTROL1
	reg_ioctrl1.iout0    = cfg->iout0;
	reg_ioctrl1.iout1    = cfg->iout1;
	reg_ioctrl1.iout0_ch = cfg->iout0_ch;
	reg_ioctrl1.iout1_ch = cfg->iout1_ch;
	ad7124_regw(REG_IOCTRL1, reg_ioctrl1.all); //reg=3
	ad7124_regw(REG_IOCTRL2, cfg->vbias);	   //reg=4

	reg_channel0.all    = 0;// 清空CHANNEL0
	reg_channel0.enable = 1; // 使能
//	reg_channel0.setup  = 0; // 选择0号配置组
	reg_channel0.ainp   = cfg->ainp;
	reg_channel0.ainm   = cfg->ainm;
	ad7124_regw(REG_CHANNEL0, reg_channel0.all);   //reg=9

	reg_config0.all      = 0; // 清空CONFIG0
	reg_config0.bipolar  = cfg->bipolar;
//	reg_config0.burnout  = 0; // 检测电流源关闭
	reg_config0.ref_bufp = 1; // REFP缓冲开
	reg_config0.ref_bufm = 1; // REFN缓冲开
	reg_config0.ain_bufp = 1; // AINP缓冲开
	reg_config0.ain_bufm = 1; // AINN缓冲开
	reg_config0.ref_sel  = cfg->ref;
	reg_config0.pga      = pga;
	ad7124_regw(REG_CONFIG0, reg_config0.all);	//reg=0x19

	reg_filter0.all          = 0; // 清空FILTER0
//	reg_filter0.filter       = 0; // Sin4滤波
//	reg_filter0.rej60        = 0; // 50Hz 60Hz双抑制
//	reg_filter0.post_filter  = 0; // 后置滤波器
	reg_filter0.single_cycle = 1; // 单周期转换
	reg_filter0.fs           = fs;
	ad7124_regw(REG_FILTER0, reg_filter0.all);	//reg=0x21
}
// 执行单次转化,超时设定为1000ms
// 转换结束后会自动进入待机模式
// 随后程序关断所有激励和偏置输出
int32_t ad7124_convert(void) {
	int i;
	int32_t adx;
	reg_control.mode = 1; // 单次转换

	ad7124_regw(REG_CONTROL, reg_control.all);
//	DelayUs(20000);
//  ad7124_regw(REG_CONTROL, 0x304);			  //reg=1
//   TTON0();
	for ( i = 1; i < 10000; i++) {
	DelayUs(200);//ad7124_sleep(100);

	reg_status.all = ad7124_regr(REG_STATUS);  //reg=0
	if (reg_status.nrdy == 0) break;
  }

	adx = ad7124_regr(REG_DATA);				//reg=2
//   TTOFF1();
	ad7124_regw(REG_IOCTRL1, 0); // 清空IO_CONTROL1  reg=3
	ad7124_regw(REG_IOCTRL2, 0); // 清空IO_CONTROL2  reg=4

	if (reg_status.nrdy) return ADVAL_INVALID;
	if (adx == 0) return ADVAL_INVALID;
	if (adx == 0xFFFFFF) return ADVAL_INVALID;
	return adx;
}
void ac30_channel_off(void){
/*	USART_ITConfig(UART5,USART_IT_RXNE,DISABLE);					//开启中断
	USART_ClearFlag(UART5, USART_FLAG_TC);
	USART_Cmd(UART5,DISABLE);  */
//SETD1();SETD1();SETD2();SETD2();
//CLRD1();CLRD1();CLRD2();CLRD2();
	CH1OFF();
	CH2OFF();
	CH3OFF();
	CH4OFF();
	CH5OFF();
	CH6OFF();
	CH7OFF();
	CH8OFF();
/*	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);					//开启中断
	USART_ClearFlag(UART5, USART_FLAG_TC);
	USART_Cmd(UART5,ENABLE); */
}

void ac30_channel_on(u8 chm)
{
/*	USART_ITConfig(UART5,USART_IT_RXNE,DISABLE);					//开启中断
	USART_ClearFlag(UART5, USART_FLAG_TC);
	USART_Cmd(UART5,DISABLE);*/
	switch(chm)
	 {
	   case 0: 
		  CH8OFF();  
			CH2OFF();
			CH3OFF();
			CH4OFF();
			CH5OFF();
			CH6OFF();
			CH7OFF();
		  CH1ON();
	   break;
	   case 1: 
		  CH1OFF();
			CH3OFF();
			CH4OFF();
			CH5OFF();
			CH6OFF();
			CH7OFF();
			CH8OFF();
		 CH2ON();
	   break;
	   case 2: 
			 CH2OFF();
		   CH1OFF();
			
			CH4OFF();
			CH5OFF();
			CH6OFF();
			CH7OFF();
			CH8OFF();
		  CH3ON();
	   break;
	   case 3: 
			 CH3OFF();
		    CH1OFF();
			CH2OFF();
			
			CH5OFF();
			CH6OFF();
			CH7OFF();
			CH8OFF();
		 CH4ON();
	   break;
	   case 4:
			 CH4OFF();
		    CH1OFF();
			CH2OFF();
			CH3OFF();
			
			CH6OFF();
			CH7OFF();
			CH8OFF();
		 CH5ON();
	   break;
	   case 5:
			 CH5OFF();
		    CH1OFF();
			CH2OFF();
			CH3OFF();
			CH4OFF();
			
			CH7OFF();
			CH8OFF();
		 CH6ON();
	   break;
	   case 6: 
			 CH6OFF();
		    CH1OFF();
			CH2OFF();
			CH3OFF();
			CH4OFF();
			CH5OFF();
			
			CH8OFF();
		 CH7ON();
	   break;
	   case 7: 
			 CH7OFF();
		    CH1OFF();
			CH2OFF();
			CH3OFF();
			CH4OFF();
			CH5OFF();
			CH6OFF();
			
			CH8ON();
	   break;
		 
//	   case 0: SETA1();SETB1();SETC1();break;
//	   case 1: CLRA1();SETB1();SETC1();break;
//	   case 2: SETA1();CLRB1();SETC1();break;
//	   case 3: CLRA1();CLRB1();SETC1();break;
//	   case 4: SETA1();SETB1();CLRC1();break;
//	   case 5: CLRA1();SETB1();CLRC1();break;
//	   case 6: SETA1();CLRB1();CLRC1();break;
//	   case 7: CLRA1();CLRB1();CLRC1();break;

//	   case 8: SETA2();CLRB2();CLRC2();break;
//	   case 9: CLRA2();CLRB2();CLRC2();break;
//	   case 10: SETA2();SETB2();CLRC2();break;
//	   case 11: CLRA2();SETB2();CLRC2();break;
//	   case 12: SETA2();CLRB2();SETC2();break; 
//	   case 13: CLRA2();CLRB2();SETC2();break;
//	   case 14: SETA2();SETB2();SETC2();break;
//	   case 15: CLRA2();SETB2();SETC2();break;
	 }
//   	if(chm>7)	{	CLRD1();CLRD1();SETD2();SETD2(); }
//	else		{	CLRD2();CLRD2();SETD1();SETD1(); }
/*	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);		//开启中断
	USART_ClearFlag(UART5, USART_FLAG_TC);
	USART_Cmd(UART5,ENABLE);*/
}
// 读取通道的AD转换值
u32 channel_read(uint8_t chn,uint8_t type) {
  uint16_t fs;  //g_eepcfg.period[chn]
  uint8_t pga  = 4;//4;//g_chnana[chn].pga;
  int32_t ad;
  const Ad7124Config *adcfg;
  if(Test.f_run==CAL_t)fs=AD7124_FS_TBL[5];
  else
  fs	= AD7124_FS_TBL[4];
  adcfg = &ADCFG_TCOUPLE;
	ac30_channel_on(chn);
//	GPIO_ResetBits(GPIOA, GPIO_Pin_11);//close power	port
	DelayUs(AC30_TIME_OPEN);
	DelayUs(AC30_TIME_OPEN);
	DelayUs(AC30_TIME_OPEN);
//	GPIO_SetBits(GPIOA, GPIO_Pin_11);//close power	port
	ad7124_reset();
	ad7124_config(adcfg, pga, fs);
	ad = ad7124_convert();
	if(ad==0x7fffffff)tempOVER=1;
	else
	{tempOVER=0;fs	= AD7124_FS_TBL[5];}

	if(Test.f_run==CAL_t)fs=AD7124_FS_TBL[5];
//  if(type==PT100||type==PT1000)fs	= AD7124_FS_TBL[5];
// else  	   fs	= AD7124_FS_TBL[5];
//sense on or off
/*	adcfg = &ADCFG_TCOUPLE;
	ac30_channel_on(chn);
	DelayUs(AC30_TIME_OPEN);
	ad7124_reset();
	ad7124_config(adcfg, pga, fs);
	ad = ad7124_convert();
   */
//1 2 3	4 5 6 7 8 9	    10	11	   16	   17
//T K J N E S R B pt100	0-v	4-20ma	18b20  am2305
  if(tempOVER==0) {
  switch (type) {
  case PT100:
  case PT1000:
    if (left_right[chn])    adcfg = &ADCFG_R_3PT;
	 else      adcfg = &ADCFG_L_3PT;
    break;

  case TYPE_R:
  case TYPE_S:
  case TYPE_B:
  case TYPE_K:
  case TYPE_E:
  case TYPE_T:
  case TYPE_J:
  case TYPE_N: pga  = 5;adcfg = &ADCFG_TCOUPLE; break;
  }
	ac30_channel_on(chn);
	DelayUs(AC30_TIME_OPEN);
	DelayUs(AC30_TIME_OPEN);
	DelayUs(AC30_TIME_OPEN);
	ad7124_reset();
	ad7124_config(adcfg, pga, fs);
	ad = ad7124_convert();
	ac30_channel_off();
	DelayUs(AC30_TIME_OPEN);
	DelayUs(AC30_TIME_OPEN);
	DelayUs(AC30_TIME_OPEN);
	left_right[chn] = 1 - left_right[chn];}

  return ad;

}

