#include "main.h"
#define DOWM_BIT_9

u8 RxNum=0,commbus;

u16 length_rx,crcx,crcr;
//extern u16 DATA_address;
//extern u8 flag_485,buf_rx[],t485;

//void DE485_T(void){	GPIO_SetBits(GPIOB, GPIO_Pin_7);}
//void DE485_R(void){	GPIO_ResetBits(GPIOB, GPIO_Pin_7);}
////extern void flash_autosave(void);

//void Uart5Init(u32 bound)
//{
//	GPIO_InitTypeDef  USART_GPIO_InitStruct;
//	USART_InitTypeDef MUSART_InitStruct;
//	NVIC_InitTypeDef NVIC_InitStruct;
//	USART_DeInit(UART5);
////��ʼ��ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
//	RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5,ENABLE);
////��ʼ��IO����
//	USART_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;		//���츴�����
//	USART_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
//	USART_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
//	GPIO_Init(GPIOC,&USART_GPIO_InitStruct);
//	
//	USART_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
//	//USART_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	USART_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
//	GPIO_Init(GPIOD,&USART_GPIO_InitStruct);
//	
////��ʼ��USART
//	MUSART_InitStruct.USART_BaudRate = bound;	//38400���������ã������ɱ��빦�������
//	MUSART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ�����ݿ�����
//	MUSART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;		
//	MUSART_InitStruct.USART_Parity = USART_Parity_No;
///*	#ifdef DOWM_BIT_9
//	MUSART_InitStruct.USART_StopBits = USART_StopBits_2;
//  #else
//	MUSART_InitStruct.USART_StopBits = USART_StopBits_1;
//  #endif						
//	MUSART_InitStruct.USART_WordLength = USART_WordLength_8b;
//*/

// #ifdef DOWM_BIT_9
//	MUSART_InitStruct.USART_WordLength = USART_WordLength_9b;
//  #else
//	MUSART_InitStruct.USART_WordLength = USART_WordLength_8b;
//  #endif
//	MUSART_InitStruct.USART_StopBits = USART_StopBits_1;					
////	MUSART_InitStruct.USART_WordLength = USART_WordLength_8b;		
//	USART_Init(UART5,&MUSART_InitStruct);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//���ȼ�����Ϊ1 λ�����ȼ�  3λ�����ȼ�
//	
//	NVIC_InitStruct.NVIC_IRQChannel = 53;//USART5_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStruct);								//��ʼ���ж�
//	
//	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);					//�����ж�
//	USART_ClearFlag(UART5, USART_FLAG_TC);
//	USART_Cmd(UART5,ENABLE);		
//}

u16 CheckCode(u8 * pSendBuf, u16 n)//���У����
{
  u16 wCrc = 0xffff,i;
  u8 j;
  for(i=0;i<n;i++)
  {
    wCrc ^= (u16)(pSendBuf[i]);
    for(j=0; j<8; j++)
    {
      if(wCrc & 1)
      {
        wCrc >>= 1;
        wCrc ^= 0xA001;
      }
      else
        wCrc >>= 1;
    }
  }
  return wCrc;
}	

//void UART5_IRQHandler(void)								//USART5�жϷ�����
//{	u8 Tmp_rx;
//	static u8 Total_Len=0;
//	if(USART_GetITStatus(UART5,USART_IT_RXNE)!=RESET)	//�����˽����ж�
//	{  Tmp_rx = USART_ReceiveData(UART5);				//��һ���ܽ���־λ���
//	
//	//rx_isr();						// ����Ҫ��������ж�
//		if(!flag_485)	RxNum++;
////	 USART_SendData(UART5,RxNum); while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==0);
//		switch (RxNum)					
//		{	case 1://	bepp=5;			//�ж��Ƿ���֡ͷ
//				if(Tmp_rx==1)	   buf_rx[0]=Tmp_rx; 
//				 else			   RxNum=0;
//				 t485=0;
//			break;

//			case 2:	//bepp=5;			//command �ж����ݳ���	03		
//				if(Tmp_rx==3)	   		{commbus=buf_rx[1]=3;Total_Len=8;}
//			//	else if(Tmp_rx==0x66)	commbus=buf_rx[1]=0x66;
//				else if(Tmp_rx==0x10)	commbus=buf_rx[1]=0x10;
//				else	RxNum=0;
//			break;
//			default:buf_rx[RxNum-1]=Tmp_rx;
//			   if(commbus==0x10&&RxNum==7)
//			   Total_Len=Tmp_rx+9;
//			   if(RxNum == Total_Len)
//        {	   flag_485=1;RxNum=0;
//			   }
//			break;
//		}
//	}
//	USART_ClearITPendingBit(UART5,USART_IT_RXNE);
//}
//extern void Uart4Send(u8 buf[],u8 end);
extern u16 battm;
extern u8 tst_type[];
extern	void Uart4Send(u8 buf[],u8 end);


