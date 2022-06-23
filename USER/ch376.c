#include "main.h"
#include "ch376.h"


static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   

//static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);
void CH376_CS1(void){	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);}
void CH376_CS0(void){	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);}
/*******************************************************************************
* 描      述      : 延时指定微秒时间,根据单片机主频调整,不精确.
*******************************************************************************/
void mDelayuS( u8 us )
{
	while ( us -- );
}

/*******************************************************************************
* 描      述      : 延时指定毫秒时间,根据单片机主频调整,不精确
*******************************************************************************/
void mDelaymS( u8 ms )
{
	while ( ms -- ) 
	{
		mDelayuS( 250 );
		mDelayuS( 250 );
		mDelayuS( 250 );
		mDelayuS( 250 );
	}
}

/*******************************************************************************
* 描      述      : 向CH376写  命令.
* 输      入      : UINT8 mCmd:要发送的命令.
*******************************************************************************/
void xWriteCH376Cmd(u8 mCmd)
{
//	SPI_CH376_SCS_HIGH();    /* 防止之前未通过xEndCH376Cmd禁止SPI片选 */
	CH376_CS1();
	delay_us(20);
/* 对于双向I/O引脚模拟SPI接口,那么必须确保已经设置SPI_SCS,SPI_SCK,SPI_SDI为输出
*  方向,SPI_SDO为输入方向 */
	CH376_CS0();/* SPI片选有效 */
//	SPI_CH376_SCS_LOW();      /* SPI片选有效 */
	SPIx_ReadWriteByte(mCmd);  /* 发出命令码 */
	delay_us(1700); 
	/* 延时1.5uS确保读写周期大于1.5uS,或者用上面一行的状态查询代替 */
	 
}


/*******************************************************************************
* 描      述      : 向CH376写   数据.
* 输      入      : UINT8 mData:
*                   要发送的数据.
*******************************************************************************/
void xWriteCH376Data(u8 mData)
{
	SPIx_ReadWriteByte(mData);
	delay_us(1);  /* 确保读写周期大于0.6uS */
}

/*******************************************************************************
* 函  数  名      : xReadCH376Data
* 描      述      : 从CH376读数据.
*******************************************************************************/
u8 xReadCH376Data(void)
{
	u8 i;
	delay_us(10);
	i = SPIx_ReadWriteByte( 0xFF );
	return( i );
}

/*******************************************************************************
* 函  数  名      : xEndCH376Cmd   结束命令.
*******************************************************************************/

void xEndCH376Cmd(void)
{
	CH376_CS1();/* 禁止SPI片选 */
}

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPIx_ReadWriteByte(u8 TxData)
{		
	uint8_t rxdata=00;
	HAL_SPI_TransmitReceive(&hspi1,&TxData,&rxdata,1,3);
	return rxdata;		    
}

/*******************************************************************************
* 描      述      : 查询CH376中断(INT#低电平).
* 返      回      : FALSE:无中断.       TRUE:有中断.
*******************************************************************************/
u8 Query376Interrupt(void)
{
	u8 i;
	i = (HAL_GPIO_ReadPin(CH376_INT_GPIO_Port,CH376_INT_Pin) ? FALSE : TRUE ); 	/* 如果连接了CH376的中断引脚则直接查询中断引脚 */
	return( i ); 
}


/*******************************************************************************
* 描      述      : 初始化CH376.
* 返      回      : FALSE:无中断.  TRUE:有中断.
*******************************************************************************/
u8 Init_CH376(void)
{
	u8 res;
	delay_ms(600);
	
//	SPI_CH376_SCS_HIGH();/* 禁止SPI片选 */
	CH376_CS1();/* 禁止SPI片选 */
	CH376_CS1();
	
	xWriteCH376Cmd( CMD11_CHECK_EXIST );
	xWriteCH376Data(0x55);
	res = xReadCH376Data();
	xEndCH376Cmd();
	if (res != 0xAA) 		
	{
		return(ERR_USB_UNKNOWN);  /* 通讯接口不正常,可能原因有:接口连接异常,其它设备影响(片选不唯一),串口波特率,一直在复位,晶振不工作 */
	}
//	else{
//		return(CH376OK);
//	}
	xWriteCH376Cmd( CMD11_SET_USB_MODE ); /* 设备USB工作模式 */
	xWriteCH376Data( 0x06 );
	delay_us(20);
	res = xReadCH376Data();
	xEndCH376Cmd();
	
	if ( res == CMD_RET_SUCCESS )  //RES=51  命令操作成功
	{
	    return( USB_INT_SUCCESS ); //USB事务或者传输操作成功 
	}
	else 
	{
	    return( ERR_USB_UNKNOWN );/* 设置模式错误 */
	}
}

///**
//  * @brief  等待超时回调函数
//  * @param  None.
//  * @retval None.
//  */
//static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
//{
//  /* 等待超时后的处理,输出错误信息 */
//	FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
//  return 0;
//}

