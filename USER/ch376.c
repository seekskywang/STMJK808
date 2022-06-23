#include "main.h"
#include "ch376.h"


static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   

//static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);
void CH376_CS1(void){	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);}
void CH376_CS0(void){	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);}
/*******************************************************************************
* ��      ��      : ��ʱָ��΢��ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ.
*******************************************************************************/
void mDelayuS( u8 us )
{
	while ( us -- );
}

/*******************************************************************************
* ��      ��      : ��ʱָ������ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
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
* ��      ��      : ��CH376д  ����.
* ��      ��      : UINT8 mCmd:Ҫ���͵�����.
*******************************************************************************/
void xWriteCH376Cmd(u8 mCmd)
{
//	SPI_CH376_SCS_HIGH();    /* ��ֹ֮ǰδͨ��xEndCH376Cmd��ֹSPIƬѡ */
	CH376_CS1();
	delay_us(20);
/* ����˫��I/O����ģ��SPI�ӿ�,��ô����ȷ���Ѿ�����SPI_SCS,SPI_SCK,SPI_SDIΪ���
*  ����,SPI_SDOΪ���뷽�� */
	CH376_CS0();/* SPIƬѡ��Ч */
//	SPI_CH376_SCS_LOW();      /* SPIƬѡ��Ч */
	SPIx_ReadWriteByte(mCmd);  /* ���������� */
	delay_us(1700); 
	/* ��ʱ1.5uSȷ����д���ڴ���1.5uS,����������һ�е�״̬��ѯ���� */
	 
}


/*******************************************************************************
* ��      ��      : ��CH376д   ����.
* ��      ��      : UINT8 mData:
*                   Ҫ���͵�����.
*******************************************************************************/
void xWriteCH376Data(u8 mData)
{
	SPIx_ReadWriteByte(mData);
	delay_us(1);  /* ȷ����д���ڴ���0.6uS */
}

/*******************************************************************************
* ��  ��  ��      : xReadCH376Data
* ��      ��      : ��CH376������.
*******************************************************************************/
u8 xReadCH376Data(void)
{
	u8 i;
	delay_us(10);
	i = SPIx_ReadWriteByte( 0xFF );
	return( i );
}

/*******************************************************************************
* ��  ��  ��      : xEndCH376Cmd   ��������.
*******************************************************************************/

void xEndCH376Cmd(void)
{
	CH376_CS1();/* ��ֹSPIƬѡ */
}

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPIx_ReadWriteByte(u8 TxData)
{		
	uint8_t rxdata=00;
	HAL_SPI_TransmitReceive(&hspi1,&TxData,&rxdata,1,3);
	return rxdata;		    
}

/*******************************************************************************
* ��      ��      : ��ѯCH376�ж�(INT#�͵�ƽ).
* ��      ��      : FALSE:���ж�.       TRUE:���ж�.
*******************************************************************************/
u8 Query376Interrupt(void)
{
	u8 i;
	i = (HAL_GPIO_ReadPin(CH376_INT_GPIO_Port,CH376_INT_Pin) ? FALSE : TRUE ); 	/* ���������CH376���ж�������ֱ�Ӳ�ѯ�ж����� */
	return( i ); 
}


/*******************************************************************************
* ��      ��      : ��ʼ��CH376.
* ��      ��      : FALSE:���ж�.  TRUE:���ж�.
*******************************************************************************/
u8 Init_CH376(void)
{
	u8 res;
	delay_ms(600);
	
//	SPI_CH376_SCS_HIGH();/* ��ֹSPIƬѡ */
	CH376_CS1();/* ��ֹSPIƬѡ */
	CH376_CS1();
	
	xWriteCH376Cmd( CMD11_CHECK_EXIST );
	xWriteCH376Data(0x55);
	res = xReadCH376Data();
	xEndCH376Cmd();
	if (res != 0xAA) 		
	{
		return(ERR_USB_UNKNOWN);  /* ͨѶ�ӿڲ�����,����ԭ����:�ӿ������쳣,�����豸Ӱ��(Ƭѡ��Ψһ),���ڲ�����,һֱ�ڸ�λ,���񲻹��� */
	}
//	else{
//		return(CH376OK);
//	}
	xWriteCH376Cmd( CMD11_SET_USB_MODE ); /* �豸USB����ģʽ */
	xWriteCH376Data( 0x06 );
	delay_us(20);
	res = xReadCH376Data();
	xEndCH376Cmd();
	
	if ( res == CMD_RET_SUCCESS )  //RES=51  ��������ɹ�
	{
	    return( USB_INT_SUCCESS ); //USB������ߴ�������ɹ� 
	}
	else 
	{
	    return( ERR_USB_UNKNOWN );/* ����ģʽ���� */
	}
}

///**
//  * @brief  �ȴ���ʱ�ص�����
//  * @param  None.
//  * @retval None.
//  */
//static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
//{
//  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
//	FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
//  return 0;
//}

