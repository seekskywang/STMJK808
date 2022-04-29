#include "main.h"

static int32_t rtouch_ad_x[RTOUCH_MEAS_N];
static int32_t rtouch_ad_y[RTOUCH_MEAS_N];

TouchTypeDef TouchData;         //���������洢��ȡ��������
//static PosTypeDef TouchAdj;     //����һ��������������У������
u8 touchflag;
static char SPI3_ReadWriteByte(uint8_t txdata)
{
	uint8_t rxdata=00;
	HAL_SPI_TransmitReceive(&hspi3,&txdata,&rxdata,1,3);
	return rxdata;
}

uint8_t xpt2046_write8(uint8_t dat) {
	return SPI3_ReadWriteByte(dat);
//	return Touch_Get_ADC(dat);
}

void testtouch(void)
{
	 xpt2046_read(&rtouch_ad_x[0], &rtouch_ad_y[0]);
}

void xpt2046_read(int32_t *x, int32_t *y) {
  uint16_t ad;
  T_CS_LOW;
  xpt2046_write8(CMD_READ_Y);
  delay_us(MEAS_DELAY);
  ad = xpt2046_write8(0xFF);
  ad <<= 8;
  ad |= xpt2046_write8(0xFF);
  T_CS_HIGH;
  *y = ((int32_t) ad >> 3) & 0x0FFF;
  delay_us(INTV_DELAY);

  T_CS_LOW;
  xpt2046_write8(CMD_READ_X);
  delay_us(MEAS_DELAY);
  ad = xpt2046_write8(0xFF);
  ad <<= 8;
  ad |= xpt2046_write8(0xFF);
  T_CS_HIGH;
  *x = ((int32_t) ad >> 3) & 0x0FFF;
  delay_us(INTV_DELAY);
}

static void DRAW_TOUCH_SIGN(u8 step)
{
	LcdClear(DATACOLOR);
	switch(step)
	{
		case 1:
		{
			LcdPrintHorz(20,20,10,TCALCOLOR);
			LcdPrintVert(25,15,10,TCALCOLOR);	
		}break;
		case 2:
		{
			LcdPrintHorz(300,20,10,TCALCOLOR);
			LcdPrintVert(305,15,10,TCALCOLOR);
		}break;
		case 3:
		{
			LcdPrintHorz(20,220,10,TCALCOLOR);
			LcdPrintVert(25,215,10,TCALCOLOR);
		}break;
		case 4:
		{
			LcdPrintHorz(300,220,10,TCALCOLOR);
			LcdPrintVert(305,215,10,TCALCOLOR);	
		}break;
	}
	
}
/****************************************************************************
* Function Name  : TOUCH_ReadData
* Description    : ������������ֵ
* Input          : cmd��ѡ��Ҫ��ȡ��X�ỹ��Y�������
* Output         : None
* Return         : ��ȡ������������ֵ
****************************************************************************/

uint16_t TOUCH_ReadData(uint8_t cmd)
{
    uint8_t i, j;
    uint16_t readValue[TOUCH_READ_TIMES], value;
    uint32_t totalValue;

    /* ��ȡTOUCH_READ_TIMES�δ���ֵ */
    for(i=0; i<TOUCH_READ_TIMES; i++)
    {   /* ��Ƭѡ */
				T_CS_LOW;
				xpt2046_write8(cmd);
				delay_us(MEAS_DELAY);
				readValue[i] = xpt2046_write8(0xFF);
				readValue[i] <<= 8;
				readValue[i] |= xpt2046_write8(0xFF);
				T_CS_HIGH;
				/* �����ݴ�����ȡ����ADֵ��ֻ��12λ�������λ���� */
        readValue[i] >>= 3;
				delay_us(INTV_DELAY);
    }

    /* �˲����� */
    /* ���ȴӴ�С���� */
    for(i=0; i<(TOUCH_READ_TIMES - 1); i++)
    {
        for(j=i+1; j<TOUCH_READ_TIMES; j++)
        {
            /* ����ֵ�Ӵ�С�������� */
            if(readValue[i] < readValue[j])
            {
                value = readValue[i];
								readValue[i] = readValue[j];
								readValue[j] = value;
            }   
        }       
    }
   
    /* ȥ�����ֵ��ȥ����Сֵ����ƽ��ֵ */
    j = TOUCH_READ_TIMES - 1;
    totalValue = 0;
    for(i=1; i<j; i++)     //��y��ȫ��ֵ
    {
        totalValue += readValue[i];
    }
    value = totalValue / (TOUCH_READ_TIMES - 2);
      
    return value;
}

/****************************************************************************
* Function Name  : TOUCH_ReadXY
* Description    : ��ȡ��������X��Y�����������ֵ
* Input          : *xValue�������ȡ��X����������ֵ�ĵ�ַ
*                * *yValue�������ȡ��Y����������ֵ�ĵ�ַ
* Output         : None
* Return         : 0����ȡ�ɹ���0xFF����ȡʧ��
****************************************************************************/

uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue)
{   
    uint16_t xValue1, yValue1, xValue2, yValue2;

    xValue1 = TOUCH_ReadData(TOUCH_X_CMD);
    yValue1 = TOUCH_ReadData(TOUCH_Y_CMD);
    xValue2 = TOUCH_ReadData(TOUCH_X_CMD);
    yValue2 = TOUCH_ReadData(TOUCH_Y_CMD);
    
    /* �鿴������֮���ֻ����ֵ��� */
    if(xValue1 > xValue2)
    {
        *xValue = xValue1 - xValue2;
    }
    else
    {
        *xValue = xValue2 - xValue1;
    }

    if(yValue1 > yValue2)
    {
        *yValue = yValue1 - yValue2;
    }
    else
    {
        *yValue = yValue2 - yValue1;
    }

    /* �жϲ�����ֵ�Ƿ��ڿɿط�Χ�� */
	if((*xValue > TOUCH_MAX+0) || (*yValue > TOUCH_MAX+0))  
	{
		return 0xFF;
	}

	/* ��ƽ��ֵ */
	*xValue = (xValue1 + xValue2) / 2;
	*yValue = (yValue1 + yValue2) / 2;

//    /* �жϵõ���ֵ���Ƿ���ȡֵ��Χ֮�� */
//    if((*xValue > TOUCH_X_MAX+0) || (*xValue < TOUCH_X_MIN) 
//       || (*yValue > TOUCH_Y_MAX+0) || (*yValue < TOUCH_Y_MIN))
//    {                   
//        return 0xFF;
//    }
	TouchData.lcdx = TouchData.x * SYSPAR.xFactor + SYSPAR.xOffset;
	TouchData.lcdy = TouchData.y * SYSPAR.yFactor + SYSPAR.yOffset;
	if(TouchData.lcdx > 320)
	{
		return 0xFF;
	}
    return 0; 
}



/****************************************************************************
* Function Name  : TOUCH_Adjust
* Description    : �����Ļ�Ƿ�У����û�еĻ�����У������У��ֵ���õ�FLASH��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TOUCH_Adjust(void)
{
    uint16_t xPot[4], yPot[4];
    float xFactor, yFactor;
	
		DRAW_TOUCH_SIGN(3);
    /* ��ȡ��һ���� */
    while(TOUCH_IN)
		{
			
		}
    delay_ms(200);
		TOUCH_ReadXY(&xPot[0],&yPot[0]);
		while(!TOUCH_IN)
		{
			
		}
		
		DRAW_TOUCH_SIGN(4);
		/* ��ȡ�ڶ����� */
    while(TOUCH_IN)
		{ 
			
		}
    delay_ms(200);
		TOUCH_ReadXY(&xPot[1],&yPot[1]);
		while(!TOUCH_IN)
		{
			
		}
		
		DRAW_TOUCH_SIGN(1);
		/* ��ȡ�������� */
    while(TOUCH_IN)
		{
			
		}
    delay_ms(200);
		TOUCH_ReadXY(&xPot[2],&yPot[2]);
		while(!TOUCH_IN)
		{
			
		}
		
		DRAW_TOUCH_SIGN(2);
		/* ��ȡ���ĸ��� */
    while(TOUCH_IN)
		{
			
		}
    delay_ms(200);
		TOUCH_ReadXY(&xPot[3],&yPot[3]);
    while(!TOUCH_IN)
		{
			
		}
    
//    /* �����ȡ�����ĸ�������ݣ����ϳɶԽǵ������� */
//    px[0] = (xPot[0] + xPot[1]) / 2;
//    py[0] = (yPot[0] + yPot[2]) / 2;
//    px[1] = (xPot[3] + xPot[2]) / 2;
//    py[1] = (yPot[3] + yPot[1]) / 2;

//    /* ����������� */
//    xFactor = (float)LCD_ADJ_X / (px[1] - px[0]);
//    yFactor = (float)LCD_ADJ_Y / (py[1] - py[0]);  
//    
//    /* ���ƫ���� */
//    TouchAdj.xOffset = (int16_t)LCD_ADJX_MAX - ((float)px[1] * xFactor);
//    TouchAdj.yOffset = (int16_t)LCD_ADJY_MAX - ((float)py[1] * yFactor);

    
    
		xFactor=(float)(SCREEN_WIDTH-40)/(xPot[1]-xPot[0]);//�õ�xfac		 
		SYSPAR.xOffset=(SCREEN_WIDTH-xFactor*(xPot[1]+xPot[0]))/2;//�õ�xoff
				
		yFactor=(float)(SCREEN_HIGH-40)/(yPot[2]-yPot[0]);//�õ�yfac
		SYSPAR.yOffset=(SCREEN_HIGH-yFactor*(yPot[2]+yPot[0]))/2;//�õ�yoff  
					
		/* �����������������ݴ���Ȼ�󱣴� */
    SYSPAR.xFactor = xFactor ;
    SYSPAR.yFactor = yFactor ;
		SaveSysPara(SYSPAR);
//    TouchAdj.posState = 1;
//    FLASH_WriteData(&TouchAdj.posState, TOUCH_ADJ_ADDR, sizeof(TouchAdj));            
}

u8 TOUCH_SCAN(void)
{
	if(!TOUCH_IN)
	{
		if(touchflag == 0)
		{
			touchflag=1;
			if(TOUCH_ReadXY(&TouchData.x, &TouchData.y) == 0XFF)
			{
				return 0;
			}else{
				BZZUER();
				return 1;
			}
		}else{
			return 0;
		}
	}else{
		touchflag=0;
	}
	return 0;
}

void TOUCH_HANDLE(u16 x,u16 y)
{

	if(y > FYCOR1 && y < FYCOR2 && inputflag == 0)
	{
		if(x > F1XCOR1 && x < F1XCOR2)//F1
		{
			switch(pageflag)
			{
				case PAGE_MEAS:
				{
					switch(itempos)
					{
						case 0:
						{
							pageflag = PAGE_SETP;
							displayflag = 1;
						}break;
					}
				}break;
				case PAGE_SETP:
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
				case PAGE_SYST:
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
			}
			
		}else if(x > F2XCOR1 && x < F2XCOR2){//F2
			switch(pageflag)
			{
				case PAGE_MEAS:
				{
					switch(itempos)
					{
						case 0:
						{
							pageflag = PAGE_SYST;
							displayflag = 1;
						}break;
					}	
					
				}break;
				case PAGE_SETP:
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
				case PAGE_SYST:
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
			}
		}else if(x > F3XCOR1 && x < F3XCOR2){//F3
			switch(pageflag)
			{
				case PAGE_MEAS:
				{
					switch(itempos)
					{
						
					}	
					
				}break;
				case PAGE_SETP:
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
				case PAGE_SYST:
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
			}
		}else if(x > F4XCOR1 && x < F4XCOR2){//F4
			switch(pageflag)
			{
				case PAGE_MEAS:
				{
					switch(itempos)
					{
						
					}	
					
				}break;
				case PAGE_SETP:
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
				case PAGE_SYST:
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
			}
		}else if(x > F5XCOR1 && x < F5XCOR2){//F4
			switch(pageflag)
			{
				case PAGE_MEAS:
				{
					switch(itempos)
					{
						
					}	
					
				}break;
				case PAGE_SETP:
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
				case PAGE_SYST:
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
			}
		}
	}else if(inputflag == 1)//���������
	{
		if(y > NUMYCOR1 && y < NUMYCOR2)
		{
			if(x > NUM1XCOR1 && x < NUM1XCOR2)
			{
				DISP_NUM('1');
			}else if(x > NUM2XCOR1 && x < NUM2XCOR2){
				DISP_NUM('2');
			}else if(x > NUM3XCOR1 && x < NUM3XCOR2){
				DISP_NUM('3');
			}else if(x > NUM4XCOR1 && x < NUM4XCOR2){
				DISP_NUM('4');
			}else if(x > NUM5XCOR1 && x < NUM5XCOR2){
				DISP_NUM('5');
			}else if(x > NUM6XCOR1 && x < NUM6XCOR2){
				DISP_NUM('6');
			}else if(x > NUM7XCOR1 && x < NUM7XCOR2){
				DISP_NUM('7');
			}else if(x > NUM8XCOR1 && x < NUM8XCOR2){
				DISP_NUM('8');
			}else if(x > NUM9XCOR1 && x < NUM9XCOR2){
				DISP_NUM('9');
			}
		}else if(y > DELYCOR1 && y < DELYCOR2){
			if(x > DELXCOR1 && x < DELXCOR2)
			{
				DEL_NUM();
			}
		}else if(y > CONYCOR1 && y < CONYCOR2 && x > CONXCOR1 && x < CONXCOR2){
			if(itempos == 3)
			{
				INPUT_CONFIRM(&SYSPAR.upper);
			}else if(itempos == 4){
				INPUT_CONFIRM(&SYSPAR.lower);
			}
		}else if(y > ZEROYCOR1 && y < ZEROYCOR2 && x > MINUSXCOR1 && x < MINUSXCOR2){
			DISP_NUM('-');
		}else if(y > ZEROYCOR1 && y < ZEROYCOR2 && x > DOTXCOR1 && x < DOTXCOR2){
			DISP_NUM('.');
		}else if(y > ZEROYCOR1 && y < ZEROYCOR2 && x > ZEROXCOR1 && x < ZEROXCOR2){
			DISP_NUM('0');
		}
	}
	
}
