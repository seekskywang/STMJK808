#include "main.h"

static int32_t rtouch_ad_x[RTOUCH_MEAS_N];
static int32_t rtouch_ad_y[RTOUCH_MEAS_N];

TouchTypeDef TouchData;         //定义用来存储读取到的数据
//static PosTypeDef TouchAdj;     //定义一阵数据用来保存校正因数
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
* Description    : 采样物理坐标值
* Input          : cmd：选择要读取是X轴还是Y轴的命令
* Output         : None
* Return         : 读取到的物理坐标值
****************************************************************************/

uint16_t TOUCH_ReadData(uint8_t cmd)
{
    uint8_t i, j;
    uint16_t readValue[TOUCH_READ_TIMES], value;
    uint32_t totalValue;

    /* 读取TOUCH_READ_TIMES次触摸值 */
    for(i=0; i<TOUCH_READ_TIMES; i++)
    {   /* 打开片选 */
				T_CS_LOW;
				xpt2046_write8(cmd);
				delay_us(MEAS_DELAY);
				readValue[i] = xpt2046_write8(0xFF);
				readValue[i] <<= 8;
				readValue[i] |= xpt2046_write8(0xFF);
				T_CS_HIGH;
				/* 将数据处理，读取到的AD值的只有12位，最低三位无用 */
        readValue[i] >>= 3;
				delay_us(INTV_DELAY);
    }

    /* 滤波处理 */
    /* 首先从大到小排序 */
    for(i=0; i<(TOUCH_READ_TIMES - 1); i++)
    {
        for(j=i+1; j<TOUCH_READ_TIMES; j++)
        {
            /* 采样值从大到小排序排序 */
            if(readValue[i] < readValue[j])
            {
                value = readValue[i];
								readValue[i] = readValue[j];
								readValue[j] = value;
            }   
        }       
    }
   
    /* 去掉最大值，去掉最小值，求平均值 */
    j = TOUCH_READ_TIMES - 1;
    totalValue = 0;
    for(i=1; i<j; i++)     //求y的全部值
    {
        totalValue += readValue[i];
    }
    value = totalValue / (TOUCH_READ_TIMES - 2);
      
    return value;
}

/****************************************************************************
* Function Name  : TOUCH_ReadXY
* Description    : 读取触摸屏的X轴Y轴的物理坐标值
* Input          : *xValue：保存读取到X轴物理坐标值的地址
*                * *yValue：保存读取到Y轴物理坐标值的地址
* Output         : None
* Return         : 0：读取成功；0xFF：读取失败
****************************************************************************/

uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue)
{   
    uint16_t xValue1, yValue1, xValue2, yValue2;

    xValue1 = TOUCH_ReadData(TOUCH_X_CMD);
    yValue1 = TOUCH_ReadData(TOUCH_Y_CMD);
    xValue2 = TOUCH_ReadData(TOUCH_X_CMD);
    yValue2 = TOUCH_ReadData(TOUCH_Y_CMD);
    
    /* 查看两个点之间的只采样值差距 */
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

    /* 判断采样差值是否在可控范围内 */
	if((*xValue > TOUCH_MAX+0) || (*yValue > TOUCH_MAX+0))  
	{
		return 0xFF;
	}

	/* 求平均值 */
	*xValue = (xValue1 + xValue2) / 2;
	*yValue = (yValue1 + yValue2) / 2;

//    /* 判断得到的值，是否在取值范围之内 */
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
* Description    : 检测屏幕是否校正，没有的话进行校正，将校正值放置到FLASH中
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TOUCH_Adjust(void)
{
    uint16_t xPot[4], yPot[4];
    float xFactor, yFactor;
	
		DRAW_TOUCH_SIGN(3);
    /* 读取第一个点 */
    while(TOUCH_IN)
		{
			
		}
    delay_ms(200);
		TOUCH_ReadXY(&xPot[0],&yPot[0]);
		while(!TOUCH_IN)
		{
			
		}
		
		DRAW_TOUCH_SIGN(4);
		/* 读取第二个点 */
    while(TOUCH_IN)
		{ 
			
		}
    delay_ms(200);
		TOUCH_ReadXY(&xPot[1],&yPot[1]);
		while(!TOUCH_IN)
		{
			
		}
		
		DRAW_TOUCH_SIGN(1);
		/* 读取第三个点 */
    while(TOUCH_IN)
		{
			
		}
    delay_ms(200);
		TOUCH_ReadXY(&xPot[2],&yPot[2]);
		while(!TOUCH_IN)
		{
			
		}
		
		DRAW_TOUCH_SIGN(2);
		/* 读取第四个点 */
    while(TOUCH_IN)
		{
			
		}
    delay_ms(200);
		TOUCH_ReadXY(&xPot[3],&yPot[3]);
    while(!TOUCH_IN)
		{
			
		}
    
//    /* 处理读取到的四个点的数据，整合成对角的两个点 */
//    px[0] = (xPot[0] + xPot[1]) / 2;
//    py[0] = (yPot[0] + yPot[2]) / 2;
//    px[1] = (xPot[3] + xPot[2]) / 2;
//    py[1] = (yPot[3] + yPot[1]) / 2;

//    /* 求出比例因数 */
//    xFactor = (float)LCD_ADJ_X / (px[1] - px[0]);
//    yFactor = (float)LCD_ADJ_Y / (py[1] - py[0]);  
//    
//    /* 求出偏移量 */
//    TouchAdj.xOffset = (int16_t)LCD_ADJX_MAX - ((float)px[1] * xFactor);
//    TouchAdj.yOffset = (int16_t)LCD_ADJY_MAX - ((float)py[1] * yFactor);

    
    
		xFactor=(float)(SCREEN_WIDTH-40)/(xPot[1]-xPot[0]);//得到xfac		 
		SYSPAR.xOffset=(SCREEN_WIDTH-xFactor*(xPot[1]+xPot[0]))/2;//得到xoff
				
		yFactor=(float)(SCREEN_HIGH-40)/(yPot[2]-yPot[0]);//得到yfac
		SYSPAR.yOffset=(SCREEN_HIGH-yFactor*(yPot[2]+yPot[0]))/2;//得到yoff  
					
		/* 将比例因数进行数据处理，然后保存 */
    SYSPAR.xFactor = xFactor ;
    SYSPAR.yFactor = yFactor ;
		//SaveSysPara(SYSPAR);
//    TouchAdj.posState = 1;
//    FLASH_WriteData(&TouchAdj.posState, TOUCH_ADJ_ADDR, sizeof(TouchAdj));            
}

u8 TOUCH_SCAN(void)
{
	if(!TOUCH_IN)
	{
		if(touchflag == 0 && SYSPAR.touch==0)
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
						case 1:
						{
							SYSPAR.TempOffset[0]+=10;
						}break;
						case 2:
						{
							SYSPAR.TempOffset[1]+=10;
						}break;
						case 3:
						{
							SYSPAR.TempOffset[2]+=10;
						}break;
						case 4:
						{
							SYSPAR.TempOffset[3]+=10;
						}break;
						case 5:
						{
							SYSPAR.TempOffset[4]+=10;
						}break;
						case 6:
						{
							SYSPAR.TempOffset[5]+=10;
						}break;
						case 7:
						{
							SYSPAR.TempOffset[6]+=10;
						}break;
						case 8:
						{
							SYSPAR.TempOffset[7]+=10;
						}break;
						default:break;
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
							//SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.speed=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
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
							//SaveSysPara(SYSPAR);
						}break;
						case 6:
						{
							SYSPAR.alarm=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 7:
						{
							SYSPAR.saveset=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							SYSPAR.interval+=10;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
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
							//SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.brtness=0;
							Brightness();
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							if(buttonpage == 0)
							{
								DateBuf=GetDate;
								DateBuf.Year++;
								moveflag = 1;
							}else{
								DateBuf=GetDate;
								DateBuf.Date++;
								moveflag = 1;
							}
							SaveTime();
						}break;
						case 6:
						{
							if(buttonpage == 0)
							{
								TimeBuf=GetTime;
								TimeBuf.Hours++;
								moveflag = 1;
							}else{
								TimeBuf=GetTime;
								TimeBuf.Seconds++;
								moveflag = 1;
							}
							SaveTime();
						}break;
						case 7:
						{
							SYSPAR.offsave=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							SYSPAR.touch=0;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
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
						case 1:
						{
							SYSPAR.TempOffset[0]-=10;
						}break;
						case 2:
						{
							SYSPAR.TempOffset[1]-=10;
						}break;
						case 3:
						{
							SYSPAR.TempOffset[2]-=10;
						}break;
						case 4:
						{
							SYSPAR.TempOffset[3]-=10;
						}break;
						case 5:
						{
							SYSPAR.TempOffset[4]-=10;
						}break;
						case 6:
						{
							SYSPAR.TempOffset[5]-=10;
						}break;
						case 7:
						{
							SYSPAR.TempOffset[6]-=10;
						}break;
						case 8:
						{
							SYSPAR.TempOffset[7]-=10;
						}break;
						default:break;
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
							//SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.speed=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							SYSPAR.unit=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 6:
						{
							SYSPAR.alarm=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 7:
						{
							SYSPAR.saveset=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							if(SYSPAR.interval<=10)
							{
								SYSPAR.interval=1;
							}else{
								SYSPAR.interval-=10;
							}
							moveflag = 1;
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
							//SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.brtness=1;
							Brightness();
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							if(buttonpage == 0)
							{
								DateBuf=GetDate;
								DateBuf.Year--;
								moveflag = 1;
							}else{
								DateBuf=GetDate;
								DateBuf.Date--;
								moveflag = 1;
							}
							SaveTime();
						}break;
						case 6:
						{
							if(buttonpage == 0)
							{
								TimeBuf=GetTime;
								TimeBuf.Hours--;
								moveflag = 1;
							}else{
								TimeBuf=GetTime;
								TimeBuf.Seconds--;
								moveflag = 1;
							}
							SaveTime();
						}break;
						case 7:
						{
							SYSPAR.offsave=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							SYSPAR.touch=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
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
						case 1:
						{
							SYSPAR.TempOffset[0]+=1;
						}break;
						case 2:
						{
							SYSPAR.TempOffset[1]+=1;
						}break;
						case 3:
						{
							SYSPAR.TempOffset[2]+=1;
						}break;
						case 4:
						{
							SYSPAR.TempOffset[3]+=1;
						}break;
						case 5:
						{
							SYSPAR.TempOffset[4]+=1;
						}break;
						case 6:
						{
							SYSPAR.TempOffset[5]+=1;
						}break;
						case 7:
						{
							SYSPAR.TempOffset[6]+=1;
						}break;
						case 8:
						{
							SYSPAR.TempOffset[7]+=1;
						}break;
						default:break;
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
							//SaveSysPara(SYSPAR);
						}break;
						case 2:
						{
							SYSPAR.speed=2;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							SYSPAR.unit=2;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							SYSPAR.interval+=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
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
							DISP_INPUT();
						}break;
//						case 1:
//						{
//							SYSPAR.language=1;
//							displayflag = 1;
//							//SaveSysPara(SYSPAR);
//						}break;
						case 2:
						{
							SYSPAR.brtness=2;
							Brightness();
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=2;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=2;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							DateBuf=GetDate;
							DateBuf.Month++;
							moveflag = 1;
							SaveTime();
						}break;
						case 6:
						{
							TimeBuf=GetTime;
							TimeBuf.Minutes++;
							moveflag = 1;
							SaveTime();
						}break;
						case 7:
						{
							SYSPAR.offsave=2;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
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
						case 1:
						{
							SYSPAR.TempOffset[0]-=1;
						}break;
						case 2:
						{
							SYSPAR.TempOffset[1]-=1;
						}break;
						case 3:
						{
							SYSPAR.TempOffset[2]-=1;
						}break;
						case 4:
						{
							SYSPAR.TempOffset[3]-=1;
						}break;
						case 5:
						{
							SYSPAR.TempOffset[4]-=1;
						}break;
						case 6:
						{
							SYSPAR.TempOffset[5]-=1;
						}break;
						case 7:
						{
							SYSPAR.TempOffset[6]-=1;
						}break;
						case 8:
						{
							SYSPAR.TempOffset[7]-=1;
						}break;
						default:break;
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
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							SYSPAR.interval-=1;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
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
							//SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=3;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=3;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 5:
						{
							DateBuf=GetDate;
							DateBuf.Month--;
							moveflag = 1;
							SaveTime();
						}break;
						case 6:
						{
							TimeBuf=GetTime;
							TimeBuf.Minutes--;
							moveflag = 1;
							SaveTime();
						}break;
						case 7:
						{
							SYSPAR.offsave=3;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 8:
						{
							TOUCH_RESET();
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
					if(itempos == 0)
					{
						memset((void *)SYSPAR.TempOffset,0,sizeof(SYSPAR.TempOffset));
					}else{
						SYSPAR.TempOffset[itempos-1]=0;
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
							//SaveSysPara(SYSPAR);
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
							//SaveSysPara(SYSPAR);
						}break;
						case 3:
						{
							SYSPAR.dimtime=4;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
						}break;
						case 4:
						{
							SYSPAR.autooff=4;
							moveflag = 1;
							//SaveSysPara(SYSPAR);
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
		SYSPARCOMP();
	}else if(inputflag == 1)//数字输入框
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
			switch(pageflag)
			{
				case PAGE_SETP:
				{
					if(itempos == 3)
					{
						INPUT_CONFIRM(&SYSPAR.upper);
					}else if(itempos == 4){
						INPUT_CONFIRM(&SYSPAR.lower);
					}
					SYSPARCOMP();
				}break;
				case PAGE_SYST:
				{
					if(itempos == 0)
					{
						if(!strcmp(inputbuf,"1316"))
						{
							inputflag=0;
							memset(inputbuf,0,sizeof(inputbuf));
							pageflag = PAGE_CAL;
							displayflag = 1;
						}
					}
				}break;
				case PAGE_CAL:
				{
					SN_CONFIRM();
				}break;
			}
			
		}else if(y > ZEROYCOR1 && y < ZEROYCOR2 && x > MINUSXCOR1 && x < MINUSXCOR2){
			DISP_NUM('-');
		}else if(y > ZEROYCOR1 && y < ZEROYCOR2 && x > DOTXCOR1 && x < DOTXCOR2){
			DISP_NUM('.');
		}else if(y > ZEROYCOR1 && y < ZEROYCOR2 && x > ZEROXCOR1 && x < ZEROXCOR2){
			DISP_NUM('0');
		}else if(y > EXITYCOR1 && y < EXITYCOR2 && x > EXITXCOR1 && x < EXITXCOR2){
			INPUT_CANCEL();
		}
	}else{
		switch(pageflag)
		{
			case PAGE_MEAS:
			{
				if(x > CHODDXCOR1 && x < CHODDXCOR2)//1357
				{
					if(y > CHROW1YCOR1 && y < CHROW1YCOR2)//1
					{
						itempos = 1;
					}else if(y > CHROW2YCOR1 && y < CHROW2YCOR2){//3
						itempos = 3;
					}else if(y > CHROW3YCOR1 && y < CHROW3YCOR2){//5
						itempos = 5;
					}else if(y > CHROW4YCOR1 && y < CHROW4YCOR2){//6
						itempos = 7;
					}
					moveflag = 1;
				}else if(x > CHEVENXCOR1 && x < CHEVENXCOR2){//2468
					if(y > CHROW1YCOR1 && y < CHROW1YCOR2)//2
					{
						itempos = 2;
					}else if(y > CHROW2YCOR1 && y < CHROW2YCOR2){//4
						itempos = 4;
					}else if(y > CHROW3YCOR1 && y < CHROW3YCOR2){//5
						itempos = 6;
					}else if(y > CHROW4YCOR1 && y < CHROW4YCOR2){//8
						itempos = 8;
					}
					moveflag = 1;
				}
			}break;
			case PAGE_SETP:
			{
				if(x > SETODDXCOR1 && x < SETODDXCOR2)//1234
				{
					if(y > SETROW1YCOR1 && y < SETROW1YCOR2)//1
					{
						itempos = 1;
					}else if(y > SETROW2YCOR1 && y < SETROW2YCOR2){//2
						itempos = 2;
					}else if(y > SETROW3YCOR1 && y < SETROW3YCOR2){//3
						itempos = 3;
					}else if(y > SETROW4YCOR1 && y < SETROW4YCOR2){//4
						itempos = 4;
					}
					moveflag = 1;
				}else if(x > SETEVENXCOR1 && x < SETEVENXCOR2){//5678
					if(y > SETROW1YCOR1 && y < SETROW1YCOR2)//5
					{
						itempos = 5;
					}else if(y > SETROW2YCOR1 && y < SETROW2YCOR2){//6
						itempos = 6;
					}else if(y > SETROW3YCOR1 && y < SETROW3YCOR2){//7
						itempos = 7;
					}else if(y > SETROW4YCOR1 && y < SETROW4YCOR2){//8
						itempos = 8;
					}
					moveflag = 1;
				}
			}break;
			case PAGE_SYST:
			{
				if(x > SETODDXCOR1 && x < SETODDXCOR2)//1234
				{
					if(y > SYSROW1YCOR1 && y < SYSROW1YCOR2)//1
					{
						itempos = 1;
					}else if(y > SYSROW2YCOR1 && y < SYSROW2YCOR2){//2
						itempos = 2;
					}else if(y > SYSROW3YCOR1 && y < SYSROW3YCOR2){//3
						itempos = 3;
					}else if(y > SYSROW4YCOR1 && y < SYSROW4YCOR2){//4
						itempos = 4;
					}
					moveflag = 1;
				}else if(x > SETEVENXCOR1 && x < SETEVENXCOR2){//5678
					if(y > SYSROW1YCOR1 && y < SYSROW1YCOR2)//5
					{
						itempos = 5;
					}else if(y > SYSROW2YCOR1 && y < SYSROW2YCOR2){//6
						itempos = 6;
					}else if(y > SYSROW3YCOR1 && y < SYSROW3YCOR2){//7
						itempos = 7;
					}else if(y > SYSROW4YCOR1 && y < SYSROW4YCOR2){//8
						itempos = 8;
					}
					moveflag = 1;
				}
			}break;
		}
	}
	
}
