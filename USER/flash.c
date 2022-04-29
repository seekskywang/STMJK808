#include "main.h"

#define   EEPROM_SYS_PARA_ADDR  (0x08000000 + 251 * 1024) 
#define   EEPROM_CAL_PARA_ADDR  (0x08000000 + 253 * 1024)
FLASH_EraseInitTypeDef ferase;
u16 savesize;
//void MemRdHalfWord(u32 addr,u16 *data,u16 num)
//{
//	u16 i,*p;
//	p = (u16 *)addr;
//	for(i=0;i<num;i++) *(data++) = *(p++);
//}

//void MemWrHalfWord(u32 addr,u16 *data,u16 num)
//{
//  FLASH_Status temp_stat;
//	u16 i=0;
//	if (num>1024) return ;
//	addr &= 0xfffffffe;//地址半字对齐

//	FLASH_Unlock();
//	while(i<num)
//	{	temp_stat = FLASH_ProgramHalfWord(addr,*data++);
//		if(temp_stat != FLASH_COMPLETE)
//		  while(FLASH_GetStatus() == FLASH_BUSY);
//	  
//		addr += 2;
//		i++;
//	}
//  FLASH_Lock();
//}

////保存校准参数			 
//void SaveDevPara(struct  CalPara Para)
//{
//	u8 *data,buf[386];
//	u16 *p,i;//,num;
//	FLASH_Status temp_stat;
//	data = (u8 *) &Para;
////	num=sizeof(Para)+2;
//	buf[0]=0x5a;
//	buf[1]=0xa5;
//	for(i=2;i<386;i++) buf[i] = *(data++);
//	p=(u16 *)buf;
//		FLASH_Unlock();
//		temp_stat = FLASH_ErasePage(EEPROM_CAL_PARA_ADDR);
//	 	if(temp_stat != FLASH_COMPLETE)
//	 	{
//		 	while(FLASH_GetStatus() == FLASH_BUSY);
//	 	}
//		FLASH_Lock();
//	MemWrHalfWord(EEPROM_CAL_PARA_ADDR,p,193);
//}
////读取校准参数
//void RdDevPara(struct CalPara *DevcPara)
//{
//	u16 *p,*data;
//	data = (u16 *)DevcPara;
//	p=(u16 *)EEPROM_CAL_PARA_ADDR;
//	if(*p != 0xa55a)	return;

//	MemRdHalfWord(EEPROM_CAL_PARA_ADDR+2,data,192);
//}

/// 初始化FLASH
void FLASH_Init(void)
{
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    HAL_FLASH_Lock();
}


void MemRdHalfWord(u32 addr,u16 *data,u16 num)
{
	u16 i,*p;
	p = (u16 *)addr;
	for(i=0;i<num;i++) *(data++) = *(p++);
}


//保存系统参数
void SaveSysPara(struct SParameter SysPara)
{
	u8 *data;
	u32 i;
	u8 buf[72];
	u16 *p;
	u32 addr=EEPROM_SYS_PARA_ADDR;
	
	uint32_t PageError = 0;
	
	data = (u8 *)&SysPara;
	for(i=0;i<74;i++) 
	{
		buf[i] = *(data++);
	}
	p=(u16 *)buf;
	ferase.TypeErase = FLASH_TYPEERASE_PAGES;
  ferase.PageAddress = EEPROM_SYS_PARA_ADDR;
	ferase.NbPages=1;
	
	HAL_FLASH_Unlock();
//	FLASH_PageErase(EEPROM_SYS_PARA_ADDR);
	HAL_FLASHEx_Erase(&ferase, &PageError);
	for(i=0;i<37;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addr,*p++);
		addr+=2;
	}
	HAL_FLASH_Lock();	
}

//保存校准参数
void SaveDevPara(struct  CalPara Para)
{
	u8 *data;
	u32 i;
	u8 buf[192];
	u16 *p;
	u32 addr=EEPROM_CAL_PARA_ADDR;
	
	uint32_t PageError = 0;
	
	data = (u8 *)&LinearCoeff;
	for(i=0;i<192;i++) 
	{
		buf[i] = *(data++);
	}
	p=(u16 *)buf;
	ferase.TypeErase = FLASH_TYPEERASE_PAGES;
  ferase.PageAddress = EEPROM_CAL_PARA_ADDR;
	ferase.NbPages=1;
	
	HAL_FLASH_Unlock();
//	FLASH_PageErase(EEPROM_SYS_PARA_ADDR);
	HAL_FLASHEx_Erase(&ferase, &PageError);
	for(i=0;i<96;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addr,*p++);
		addr+=2;
	}
	HAL_FLASH_Lock();	
}
//读取系统参数
void RdSysPara(struct SParameter *SysPara)
{
	u16 /**p,*/*data;
	data = (u16 *)SysPara;
//	p=(u16 *)EEPROM_SYS_PARA_ADDR;
//	if(*p != 0xa55a) return;
	savesize=sizeof(LinearCoeff);
	MemRdHalfWord(EEPROM_SYS_PARA_ADDR,data,37);
}

//读取校准参数
void RdDevPara(struct  CalPara *Para)
{
	u16 /**p,*/*data;
	data = (u16 *)Para;
//	p=(u16 *)EEPROM_CAL_PARA_ADDR;
//	if(*p != 0xa55a) return;
	MemRdHalfWord(EEPROM_CAL_PARA_ADDR,data,96);
}

////读取系统参数
//void RdSysPara(struct SParameter *SysPara)
//{
//	u16 i;
//	u16 *data;
//	u32 addr=EEPROM_SYS_PARA_ADDR;
//	data = (u16 *)SysPara;
//	for(i=0;i<sizeof(SYSPAR)/2;i++)
//	{
//		*data++ = *(__IO uint16_t *)addr;
//		addr+=2;
//	}
//}


