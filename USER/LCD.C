
//#include "stm32f10x_lib.h"
#include "main.h"
//#include "img.h"

extern u8 brightness; //亮度
extern u8 contrast;   //对比度
extern u8 saturation; //饱和度
u16 para1,para2,para3;
//-------------------------------------------------------------------------------------------------
void LCD_delayms(u8 t)
{
	 u32 tmx;
	 u8 i;
	for(i=0; i<t; i++)
	{for(tmx=10301;tmx>0;tmx--);}//DelayMs(1000);
}

u16 RGB888TORGB565(u32 color)	   //RGB888转RGB565
{
   u16 temp=0;
   u8 RED=0,GREEN=0,BLUE=0;
   // 获取RGB单色，并截取高位 
   RED=(color&RGB888_RED)>>19;
   GREEN=(color&RGB888_GREEN)>>10;
   BLUE=(color&RGB888_BLUE)>>3;

  // 连接 
   temp=(RED<<11)+(GREEN<<5)+BLUE ;
   return temp;

}

/**************************************************************************************
函数功能：设置写入数据的目标区域
输入：    区域的左上角和右下角坐标
输出：	  无
**************************************************************************************/
void LcdSetArea(u16 x1, u16 y1, u16 x2, u16 y2)
{
//	u16 temp;
/*	x1=320-x1;
	x2=320-x2;
	y1=240-y1;
	y2=240-y2;
	if(x1>x2)
	{	temp=x1;
		x1=x2;
		x2=temp;	
	}
	if(y1>y2)
	{	temp=y1;
		y1=y2;
		y2=temp;
	}  	*/
	WriteCommand(0x2A);	//设置开始列地址,结束列地址
  	WriteData((x1)>>8);
	WriteData(x1);
	WriteData((x2)>>8);
	WriteData(x2);

	WriteCommand(0x2B);	//设置开始行地址,结束行地址
	WriteData((y1)>>8);
	WriteData(y1);
	WriteData((y2)>>8);
	WriteData(y2);
}

/**************************************************************************************
函数功能：清除整个LCD屏幕
输入：    目标颜色
输出：	  无
**************************************************************************************/
void LcdClear(u16 color)
{
	register u32 size;

//	LcdSetArea(0,0,480,272);
	LcdSetArea(0,0,320,240);
	WriteCommand(0x2C);
	
	size = LCDBUFSIZE;
	while(size--)
		WriteData(color);
}

u16 ReadId(void)
{
	uint16_t temp = 0 ;
	WriteCommand(0xD3);   

     
    // 读取前面两个用不到的字节
    ReadData(para1);
    ReadData(para2);
     
    // 开始正式的读取数据
    ReadData(temp);
    // 将读取到的数据向左移动8位，腾出位置
    temp <<= 8;
    // 继续读取数据放到temp变量中
	ReadData(para3);
    temp |= para3;
     
    // 返回读取到的ID数据
    return temp;
}
void screen_init(void)
{
//	WriteCommand(0x01);//软复位
	delay_ms(120);
	WriteCommand(0x01);//软复位
	delay_ms(120);
//************* Start Initial Sequence **********//
    WriteCommand(0xCF);   /* 功耗控制B */
    WriteData(0x00);	
    WriteData(0xc1);    //01kaiqi MLG   03guanbi
    WriteData(0X30);

    WriteCommand(0xED);  /* 电源序列控制 */
    WriteData(0x64);  /* 软启动控制   */
    WriteData(0x03);  /* 电源序列控制 */
    WriteData(0X12);  /* 电源序列控制 */
    WriteData(0X81);  /* DDVDH增强模式*/

    WriteCommand(0xE8);  /* 驱动时序控制A*/
    WriteData(0x85);  /* 栅极驱动器的非重叠时序控制 */
    WriteData(0x10);  /* EQ时序控制 0X00 */
    WriteData(0x78);  /* 预充电时间控制  */

    WriteCommand(0xCB);  /* 功耗控制A */
    WriteData(0x39);
    WriteData(0x2C);
    WriteData(0x00);
    WriteData(0x34);  /* 内核电压控制 */
    WriteData(0x02);  /* DDVDH控制    */

    WriteCommand(0xF7);  /* 泵比控制     */
    WriteData(0x20);  /* 比率控制     */

    WriteCommand(0x3A);	//PIXEL FORMAT SET 18BIT P136
//***********xiugai*******************//
//write_dat(0x66);//fpga
    WriteData(0x55);//TST
//************************************//
    WriteCommand(0xEA);  /* 驱动时序控制B      */
    WriteData(0x00);  /* 栅极驱动器时序控制 */
    WriteData(0x00);

    WriteCommand(0xB1);  /* (正常模式/全色模式下)帧速率控制  */
    WriteData(0x00);  /* 内部时钟分频设置，00时表示不分频 */
    WriteData(0x1A);  /* RTNA设置，用于设置1H(行)的时间 //1b:70hz  1f:61hz  0X1D*/

//write_cmd(0xB4);  //display inversion control
//write_dat(0x00);

    WriteCommand(0xB6);  /* 显示功能设置// Display Function Control  P166 */
    WriteData(0x0A);  /* 设置在没显示区域的扫描格式，0A表示间隔扫描//NORMALLY BLACK isc:scan cycle */
//    WriteData(0xA2);  /* 设置源极、栅极驱动器的移动方向和扫描周期  0X02  */
	WriteData(0xc2);
	WriteData(0x27);

    WriteCommand(0xC0);  /* 功耗控制1//Power control  P180   */
    WriteData(0x22);  /* 设置GVDD电平//VRH[5:0]           */

    WriteCommand(0xC1);  /* 功耗控制2 //Power control  P181      */
    WriteData(0x13);  /* 设置用于升压电路的因子 //SAP[2:0];BT[3:0]  AVDD VGH VGL  */

    WriteCommand(0xC5);  /* VCOM控制1//VCM control   P182    */
    WriteData(0x46);  /* 设置VCOMH电压  //4.45v   */
    WriteData(0x3A);  /* 设置VCOML电压 //-1.05    */

    WriteCommand(0xC7);  /* VCOM控制2//VCM control2  P184    */
    WriteData(0XCA);  /* 设置VCOM偏移电压 //4:28  1:31    */

    WriteCommand(0x36);  /* 存储器访问控制 // Memory Access Control  P129  */
//***********xiugai*******************//
//write_dat(0x00);   //00:BGR TST
//    WriteData(0x08);  /* 该指令定义帧存储器的读写扫描方向  //08:RGB    */
		 WriteData(0xA8);  /* 该指令定义帧存储器的读写扫描方向  //08:RGB    */
//*************************************//
    WriteCommand(0xF2);  /* 使能3G // 3Gamma Function Disable  P206        */
    WriteData(0x00);  /* 01使能3G，00不使能   */


    WriteCommand(0x26);  /* 伽马设置//Gamma curve selected     */
    WriteData(0x01);  /* 选择伽马曲线1    */

    WriteCommand(0xE0);  /* 正极伽马校准//Set Gamma    */
    WriteData(0x0D);  //63
    WriteData(0x2A);//62
    WriteData(0x27);//61
    WriteData(0x0A);//59
    WriteData(0x0B);//57
    WriteData(0x05);//50
    WriteData(0x49);//43
    WriteData(0X79);//27/36
    WriteData(0x3D);//20
    WriteData(0x0D);//13
    WriteData(0x18);//6
    WriteData(0x0C);//4
    WriteData(0x10);//2
    WriteData(0x06);//1
    WriteData(0x00);//0

    WriteCommand(0XE1);  /* 退出睡眠模式负极伽马校准//Set Gamma  */
    WriteData(0x02);   //63
    WriteData(0x0B);//62
    WriteData(0x0D);//61
    WriteData(0x04);//59
    WriteData(0x11);//57
    WriteData(0x06);//50
    WriteData(0x18);//43
    WriteData(0x16);//27//36
    WriteData(0x2B);//20
    WriteData(0x03);//13
    WriteData(0x06);//6
    WriteData(0x07);//4
    WriteData(0x2A);//2
    WriteData(0x2E);//1
    WriteData(0x0F);   //0
		
//		write_cmd(0x53);//wr ctrl
//		WriteData(0x0c);

//		write_cmd(0x5e);//wr cabc
//		WriteData(0x0);

		WriteCommand(0x51);//wr linght
		WriteData(0x00);
		
	//	write_cmd(0x53);//wr ctrl
	//	write_dat(0x04);

//write_cmd(0x51);//wr linght
	//	write_dat(0xff);
    WriteCommand(0x11);  /* 退出睡眠模式//Exit Sleep   */
    delay_ms(120);
    WriteCommand(0x29);  /* 开显示//Display on   */
}

u16 fontbuf[256];

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
  WriteCommand(0X2A);
  WriteData(Xpos>>8);
	WriteData(Xpos&0XFF);         
	WriteCommand(0X2B);
	WriteData(Ypos>>8);
	WriteData(Ypos&0XFF);
} 

//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
        WriteCommand(0x2C);
}   


/**************************************************************************************
函数功能：画点
输入：    u16  x,y    	        点的坐标
	  u16  color		颜色
输出：	  无
**************************************************************************************/
void LcdPrintDot(u16 x, u16 y, u16 color)
{
//	LcdSetArea(240-x,272-y,240-x,272-y);
	//LcdSetArea(x,y,x,y);
	LcdSetArea(y,x,y,x);
	WriteCommand(0x2C);
	WriteData(color);
}
/**************************************************************************************
函数功能：画矩形方框
输入：    矩形的左上角和右下角坐标，和目标颜色
输出：	  无
**************************************************************************************/
void LcdPrintRecDot(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{	u8 i;
	for(i=0;i<(x2-x1)/3;i++)
	LcdPrintDot(x1+i*3, y1, color);
	for(i=0;i<(y2-y1)/3;i++)
	LcdPrintDot(x1, y1+i*3,  color);
	for(i=0;i<(x2-x1)/3;i++)
	LcdPrintDot(x1+i*3, y2,  color);
	for(i=0;i<(y2-y1)/3;i++)
	LcdPrintDot(x2, y1+i*3, color);
}
/**************************************************************************************
函数功能：画水平直线
输入：    WORD 	x    	        起点的横坐标
	  WORD  y  	        起点的纵坐标
	  WORD  width		直线长度
	  WORD  color		颜色
输出：	  无
**************************************************************************************/
void LcdPrintHorz(u16 x, u16 y, u16 width, u16 color)
{
	LcdSetArea(x,y,x+width-1,y);
	WriteCommand(0x2C);
	
	while(width--)
		WriteData(color);
}

/**************************************************************************************
函数功能：画垂直直线
输入：    u16  x    	        起点的横坐标
	  u16  y  	        起点的纵坐标
	  u16  height		直线高度
	  u16  color		颜色
输出：	  无
**************************************************************************************/
void LcdPrintVert(u16 x, u16 y, u16 height, u16 color)
{
	LcdSetArea(x,y,x,y+height-1);
	WriteCommand(0x2C);
	
        while(height--)
		WriteData(color);
}

/**************************************************************************************
函数功能：画斜线
输入：u16  x1    	    起点的横坐标
	  u16  y1  	        起点的纵坐标
	  u16  x2    	    终点的横坐标
	  u16  y2  	        终点纵的坐标
	  u16  color		颜色
输出：	  无
**************************************************************************************/
void Bre_Line(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
    int dx,dy,e;
    dx=x2-x1;
    dy=y2-y1;

    if(dx>=0)
    {
        if(dy >= 0) // dy>=0
        {
            if(dx>=dy) // 1/8 octant
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    LcdPrintDot(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}
                    x1+=1;
                    e+=dy;
                }
            }
            else        // 2/8 octant
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    LcdPrintDot(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else           // dy<0
        {
            dy=-dy;   // dy=abs(dy)

            if(dx>=dy) // 8/8 octant
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    LcdPrintDot(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}
                    x1+=1;
                    e+=dy;
                }
            }
            else        // 7/8 octant
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    LcdPrintDot(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}
                    y1-=1;
                    e+=dx;
                }
            }
        }
    }
    else //dx<0
    {
        dx=-dx;        //dx=abs(dx)
        if(dy >= 0) // dy>=0
        {
            if(dx>=dy) // 4/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    LcdPrintDot(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 3/8 octant
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    LcdPrintDot(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else           // dy<0
        {
            dy=-dy;   // dy=abs(dy)

            if(dx>=dy) // 5/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    LcdPrintDot(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 6/8 octant
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    LcdPrintDot(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}
                    y1-=1;
                    e+=dx;
                }
            }
        }
    }
}

/**************************************************************************************
函数功能：画线
输入：u16  x1    	    起点的横坐标
	  u16  y1  	        起点的纵坐标
	  u16  x2    	    终点的横坐标
	  u16  y2  	        终点的纵坐标
	  u16  color		颜色
输出：	  无
**************************************************************************************/
void LcdPrintLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 length;
	if((x1==x2) || (y1==y2))
	{
	  	LcdSetArea(x1,y1,x2,y2);
		WriteCommand(0x2C);
		if(x1==x2)
		{
			if(y2>y1)		length=y2-y1+1;
			
			else	 		length=y1-y2+1;
		}
		else if(y1==y2)
		{
			if(x2>x1)		length=x2-x1+1;
			
			else	 		length=x1-x2+1;
		}
        while(length--)
		WriteData(color);
	}
	else
		Bre_Line(x1,y1,x2,y2,color);
}
								
/**************************************************************************************
函数功能：画矩形方框
输入：    矩形的左上角和右下角坐标，和目标颜色
输出：	  无
**************************************************************************************/
void LcdPrintRec(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{//	u8 i;
//	for(i=0;i*4<x2;i++)		   //x2 -
//	LcdPrintHorz(x1+i*4+1, y1,  2, color);
	LcdPrintHorz(x1, y1, x2-x1+1, color);
/*	LcdPrintHorz(x1, y1, 1, color);
	LcdPrintHorz(x1+4, y1+3, 1, color);
	LcdPrintHorz(x1+8, y1+5, 1, color);*/
	LcdPrintVert(x1, y1, y2-y1+1, color);
	LcdPrintHorz(x1, y2, x2-x1+1, color);
	LcdPrintVert(x2, y1, y2-y1+1, color);
}

/**************************************************************************************
函数功能：填充一个矩形区域
输入：    矩形的左上角和右下角坐标，和目标颜色
输出：	  无
**************************************************************************************/
/*void LcdFillRec(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	register u32 size;
	LcdSetArea(x1,y1,x2,y2);
	WriteCommand(0x2C);
	size = (u32)(y2-y1+1)*(x2-x1+1);
	while(size--)
		WriteData(color);
}*/
void LcdFillRec(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{		// register
	
//	u16 height,width;
//	u16 i,j;
//	width=x2-x1+1;                 //得到填充的宽度
//	height=y2-y1+1;                //高度
//	for(i=0;i<height;i++)
//	{
//		LCD_SetCursor(x1,y1+i);           //设置光标位置
//		LCD_WriteRAM_Prepare();     //开始写入GRAM
//		for(j=0;j<width;j++)
//			WriteData(color[i*height+j]);//写入数据
//	}        
				
	u32 size;
	LcdSetArea(x1,y1,x2,y2);
//	WriteCommand(0x2A);
//	WriteData(0);
//	WriteData(240-y1);

//	WriteCommand(0x2b);
//	WriteData(x1>>8);
//	WriteData(x1&0xff);
//LCD_SetCursor(x1,y1);           //设置光标位置
	
	WriteCommand(0x2c);
	WriteData(color);
	size = (u32)(y2-y1+1)*(x2-x1+1);
	while(size--)
	WriteData(color);
}
/**************************************************************************************
函数功能：填充一个矩形区域
输入：    矩形的左上角和右下角坐标，和目标颜色地址
输出：	  无
**************************************************************************************/
void LcdFillArea(u16 x1, u16 y1, u16 x2, u16 y2,u16 *color)
{
	u32 size;
	u16 *p;
	p=color;
	LcdSetArea(x1,y1,x2,y2);
	WriteCommand(0x2C);
	size = (y2-y1+1)*(x2-x1+1);
	while(size--)
		WriteData(*(p++));
}

/**************************************************************************************
函数功能：读取一个矩形区域
输入：    矩形的左上角和右下角坐标，和区域颜色
输出：	  无
**************************************************************************************/
void LcdReadRec(u16 x1, u16 y1, u16 x2, u16 y2,u16 *color)
{
  u32 size;
	
	LcdSetArea(x1,y1,x2,y2);
	WriteCommand(0x2E);
	
	size = (u32)(y2-y1+1)*(x2-x1+1);
	while(size--)
		ReadData(*(color++));
}


void TFT_Draw_Point(u16 x,u8 y,u16 color)
{
    WriteCommand(0X2A); 
    WriteData(0x00);
    WriteData(240-y);

    WriteCommand(0X2B); 
    WriteData(x>>8);
    WriteData(x&0xFF);

    WriteCommand(0x2C);//开始写
    WriteData(color);
}
/*void LcdClear(u16 x,u8 y,u16 color)
{
    WriteCommand(0X2A); 
    WriteData(0x00);
    WriteData(240-y);

    WriteCommand(0X2B); 
    WriteData(x>>8);
    WriteData(x&0xFF);

    WriteCommand(0x2C);//开始写
    WriteData(color);
}*/
/**************************************************************************************
函数功能：在LCD上打印一个24X32英文字符
输入：    code       字符内码
          x,y	     目标显示位置坐标
	  color	     字体颜色
	  bcolor     背景颜色
输出：	  无
**************************************************************************************/

void LcdPrintEn24x32(u8 code, u16 x, u16 y, u16 color,u16 bcolor)
{
	u8 ip=0,i,j,k;
  do
	{
		if(ZK_24x32[ip].code == code)
		{
			LcdSetArea(x,y,x+23,y+31);
			WriteCommand(0x2C);
			for(i=0;i<32;i++)
			{														
				for(j=0;j<3;j++)
				{
					for(k=0;k<8;k++)
					{
						if(ZK_24x32[ip].ENCODE[i]&(0x80>>j))
							 WriteData(color)//
						
						else
							 WriteData(bcolor)
					}
				}
			}
			return;
		}
		ip++;
	}
	while(ZK_24x32[ip].code);
}


/**************************************************************************************
函数功能：在LCD上打印一个8X16英文字符
输入：    code       字符内码
          x,y	     目标显示位置坐标
	  color	     字体颜色
	  bcolor     背景颜色
输出：	  无
**************************************************************************************/

void LcdPrintEn8x16(u8 code, u16 x, u16 y, u16 color,u16 bcolor)
{
	u8 ip=0,i,j;
  do
	{
		if(ZK_8x16[ip].code == code)
		{
			LcdSetArea(x,y,x+7,y+15);
			WriteCommand(0x2C);
			for(i=0;i<16;i++)
			{														
				for(j=0;j<8;j++)
				{
					if(ZK_8x16[ip].ENCODE[i]&(0x80>>j))
						 WriteData(color)//
					
					else
						 WriteData(bcolor)
				}
			}
			return;
		}
		ip++;
	}
	while(ZK_8x16[ip].code);
}


 /**************************************************************************************
函数功能：在LCD上打印一个8X16英文字符  单体
输入：    code       字符内码
          x,y	     目标显示位置坐标
	  color	     字体颜色
	  bcolor     背景颜色
输出：	  无
**************************************************************************************/

void LcdPrintEn8x16_2(u8 code, u16 x, u16 y, u16 color,u16 bcolor)
{
	u8 ip=0,i,j;
  do
	{
		if(ZK_8x16_2[ip].code == code)
		{
			LcdSetArea(x,y,x+7,y+15);
			WriteCommand(0x2C);
			for(i=0;i<11;i++)
			{														
				for(j=0;j<8;j++)
				{
					if(ZK_8x16_2[ip].ENCODE[i]&(0x80>>j))
						 WriteData(color)//
					
					else 
						 WriteData(bcolor)
				}
			}
			return;
		}
		ip++;
	}
	while(ZK_8x16_2[ip].code);
}
void LcdPrintEn8x16_3(u8 code, u16 x, u16 y, u16 color,u16 bcolor)
{
	u8 ip=0,i,j;
  do
	{
		if(ZK_8x16_3[ip].code == code)
		{
		//	LcdSetArea(x,y,x+7,y+14);
			LcdSetArea(x,y,x+7,y+14);
			WriteCommand(0x2C);
			for(i=0;i<7;i++)
			{	for(j=0;j<8;j++)
				{	if(ZK_8x16_3[ip].ENCODE[i]&(0x80>>j))
						 WriteData(color)
					else 
						 WriteData(bcolor)
				}
			}
			return;
		}
		ip++;
	}
	while(ZK_8x16_3[ip].code);
}
/**************************************************************************************
函数功能：在LCD上打印一个4X7英文字符
输入：    code       字符内码
          x,y	     目标显示位置坐标
	  color	     字体颜色
	  bcolor     背景颜色
输出：	  无
**************************************************************************************/

void LcdPrintEn4x7(u8 code, u16 x, u16 y, u16 color,u16 bcolor)
{
	u8 ip=0,i,j,k,num=0;
  do
	{
		if(ZK_4x7[ip].code == code)
		{
			LcdSetArea(x,y,x+3,y+6);
			WriteCommand(0x2C);
			for(i=0;i<7;i++)
			{														
				for(j=0;j<2;j++)
				{
					for(k=0;k<4;k++)
					{
						if(j*4+k==4) break;
						if(ZK_4x7[ip].ENCODE[num]&(0x80>>k))
			 				WriteData(color)
			 			
			 			else
			 				WriteData(bcolor)
					}
					num++;
					if(j*4+k==4) break;
				}
			}
			return;
		}
		ip++;
	}
	while(ZK_4x7[ip].code);
}
void LcdPrintEn11x15(u8 code, u16 x, u16 y, u16 color,u16 bcolor)
{
	u8 ip=0,i,j,k,num=0;
    do
	{
		if(ZK_11x15[ip].code == code)
		{
			LcdSetArea(x,y,x+10,y+16); //10
			WriteCommand(0x2C);
			for(i=0;i<13;i++)
			{														
				for(j=0;j<2;j++)
				{
					for(k=0;k<8;k++)
					{
						if(j*8+k==11) break;  //11
						if(ZK_11x15[ip].ENCODE[num]&(0x80>>k))
			 				WriteData(color)
			 			
			 			else
			 				WriteData(bcolor)
					}
					num++;
					if(j*8+k==11) break; //11
				}
			}
			return;
		}
		ip++;
	}
	while(ZK_11x15[ip].code);
}
 /**************************************************************************************
函数功能：在LCD上打印一个11X17英文字符
输入：    code       字符内码
          x,y	     目标显示位置坐标
	  color	     字体颜色
	  bcolor     背景颜色
输出：	  无
**************************************************************************************/
void LcdPrintEn11x17(u8 code, u16 x, u16 y, u16 color,u16 bcolor)
{
	u8 ip=0,i,j,k,num=0;
    do
	{
		if(ZK_11x17[ip].code == code)
		{
			LcdSetArea(x,y,x+10,y+16);
			WriteCommand(0x2C);
			for(i=0;i<15;i++)
			{														
				for(j=0;j<2;j++)
				{
					for(k=0;k<8;k++)
					{
						if(j*8+k==11) break;
						if(ZK_11x17[ip].ENCODE[num]&(0x80>>k))
			 				WriteData(color)
			 			
			 			else
			 				WriteData(bcolor)
					}
					num++;
					if(j*8+k==11) break;
				}
			}
			return;
		}
		ip++;
	}
	while(ZK_11x17[ip].code);
}

 /**************************************************************************************
函数功能：在LCD上打印一个13X24英文字符
输入：    code       字符内码
          x,y	     目标显示位置坐标
	  color	     字体颜色
	  bcolor     背景颜色
输出：	  无
**************************************************************************************/
void LcdPrintEn13x24(u8 code, u16 x, u16 y, u16 color,u16 bcolor)
{
	u8 ip=0,i,j,k,num=0;
    do
	{
		if(ZK_13x24[ip].code == code)
		{
			LcdSetArea(x,y,x+13,y+23);
			WriteCommand(0x2C);
			for(i=0;i<24;i++)
			{														
				for(j=0;j<2;j++)
				{
					for(k=0;k<8;k++)
					{
						if(j*8+k==14) break;
						if(ZK_13x24[ip].ENCODE[num]&(0x80>>k))
			 				WriteData(color)

			 			else			WriteData(bcolor)
					}
					num++;
					if(j*8+k==14) break;
				}
			}
			return;
		}
		ip++;
	}
	while(ZK_13x24[ip].code);
}

 /**************************************************************************************
函数功能：在LCD上打印一个10X15英文字符
输入：    code       字符内码
          x,y	     目标显示位置坐标
	  color	     字体颜色
	  bcolor     背景颜色
输出：	  无
**************************************************************************************/

void LcdPrintEn10x15(u8 code, u16 x, u16 y, u16 color,u16 bcolor)
{
	u8 ip=0,i,j,k,num=0;
    do
	{
		if(ZK_10x15[ip].code == code)
		{
			LcdSetArea(x,y,x+9,y+14);
			WriteCommand(0x2C);
			for(i=0;i<13;i++)
			{														
				for(j=0;j<2;j++)
				{
					for(k=0;k<8;k++)
					{
						if(j*8+k==10) break;
						if(ZK_10x15[ip].ENCODE[num]&(0x80>>k))
			 				WriteData(color)

			 			else
			 				WriteData(bcolor)
					}
					num++;
					if(j*8+k==10) break;
				}
			}
			return;
		}
		ip++;
	}
	while(ZK_10x15[ip].code);
}

/**************************************************************************************
函数功能：在LCD上打印一个12X24英文字符
输入：    code       字符内码
          x,y	     目标显示位置坐标
	  color	     字体颜色
	  bcolor     背景颜色
输出：	  无
**************************************************************************************/

void LcdPrintEn12x24(u8 code, u16 x, u16 y, u16 color,u16 bcolor)
{
	u8 ip=0,i,j,k,num=0;
    do
	{
		if(ZK_12x24[ip].code == code)
		{
			LcdSetArea(x,y,x+11,y+23);
			WriteCommand(0x2C);
			for(i=0;i<24;i++)
			{														
				for(j=0;j<2;j++)
				{
					for(k=0;k<8;k++)
					{
						if(j*8+k==12) break;
						if(ZK_12x24[ip].ENCODE[num]&(0x80>>k))
			 				WriteData(color)

			 			else
			 				WriteData(bcolor)
					}
					num++;
					if(j*8+k==12) break;
				}
			}
			return;
		}
		ip++;
	}
	while(ZK_12x24[ip].code);
}

/**************************************************************************************
函数功能：在LCD上打印一个48X54英文字符
输入：    code       字符内码
          x,y	     目标显示位置坐标
	  color	     字体颜色
	  bcolor     背景颜色
输出：	  无
**************************************************************************************/

void LcdPrintEn48x54(u8 code, u16 x, u16 y, u16 color,u16 bcolor)
{
	u8 i,j,k;
	u16	ip=0,num=0;
    do
	{
		if(ZK_48x54[ip].code == code)
		{
			LcdSetArea(x,y,x+45,y+53);
			WriteCommand(0x2C);
			for(i=0;i<54;i++)
			{														
				for(j=0;j<7;j++)
				{
					for(k=0;k<8;k++)
					{
						if(j*8+k==46) break;
						if(ZK_48x54[ip].ENCODE[num]&(0x80>>k))
			 				WriteData(color)

			 			else
			 				WriteData(bcolor)
					}
					num++;
					if(j*8+k==46) break;
				}
			}
			return;
		}
		ip++;
	}
	while(ZK_48x54[ip].code);
}

/**************************************************************************************
函数功能：在LCD上打印一个16X16汉字
输入：    code   字符内码
          x,y	 目标显示位置坐标
	  color	     字体颜色
	  bcolor     背景颜色
输出：	  无
**************************************************************************************/
void LcdPrintHz16x16(u16 code,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 ip=0,i,j;
	u16 fcode;
	do
	{if((GB_16[ip].code[0] == (code>>8)) && (GB_16[ip].code[1] == (code&0xff)))
		{
			LcdSetArea(x,y,x+15,y+15);
			WriteCommand(0x2C);
			for(i=0;i<32;i=i+2)
			{
				fcode = GB_16[ip].Msk[i]<<8|GB_16[ip].Msk[i+1];
				for(j=0;j<16;j++)
				{
					if(fcode&(0x8000>>j))
						WriteData(color)
					else 
						WriteData(bcolor)
				}
			}
			return;
		}
		ip++;
	}while(GB_16[ip].code[0] != 0 || GB_16[ip].code[1] != 0 );
}

/**************************************************************************************
函数功能：在LCD上打印一个24x24汉字
输入：    code       字符内码
          x,y	     目标显示位置坐标
	  color	     字体颜色
	  bcolor     背景颜色
输出：	  无
**************************************************************************************/
/*void LcdPrintHz24x24(u16 code,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 ip=0,i,j;
	do
	{
		if((GB_24[ip].code[0] == (code>>8)) && (GB_24[ip].code[1] == (code&0x00ff)))
		{
			LcdSetArea(x,y,x+23,y+23);	
			WriteCommand(0x2C);
			for(i=0;i<72;i++)
			{
				for(j=0;j<8;j++)
				{
					if(GB_24[ip].Msk[i]&(0x80>>j))
						WriteData(color)

					else
						WriteData(bcolor)
				}
			}
			return;
		}
		ip++;
	}while(GB_24[ip].code[0] != 0 || GB_24[ip].code[1] != 0);
}	  */
/**************************************************************************************
函数功能：在LCD上打印一个32x32汉字
输入：    code       字符内码
          x,y	     目标显示位置坐标
	  color	     字体颜色
	  bcolor     背景颜色
输出：	  无
**************************************************************************************/
// void LcdPrintHz32x32(u16 code,u16 x,u16 y,u16 color,u16 bcolor)
// {
// 	u8 ip=0,i,j;
// 	do
// 	{
// 		if((GB_324[ip].code[0] == (code>>8)) && (GB_324[ip].code[1] == (code&0x00ff)))
// 		{
// 			LcdSetArea(x,y,x+31,y+31);	
// 			WriteCommand(0x2C);
// 			for(i=0;i<132;i++)
// 			{
// 				for(j=0;j<8;j++)
// 				{
// 					if(GB_324[ip].Msk[i]&(0x80>>j))
// 						WriteData(color);

// 					else
// 						WriteData(bcolor);
// 				}
// 			}
// 			return;
// 		}
// 		ip++;
// 	}while(GB_324[ip].code[0] != 0 || GB_324[ip].code[1] != 0);
// }

/**************************************************************************************
功能描述: 	在屏幕显示一个字符串
输    入: 	char* str       字符串指针
                u16 x,y		目标显示位置坐标
		u16 color	字体颜色
		u16 bcolor	背景颜色
输    出: 	无
**************************************************************************************/
void Lcd_Str32(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i;
	u16 px;	
	i = 0;
	px = x;
//	px2=x+256;
	while(str[i])
	{
		if(str[i] > 0x7F)//是汉字
		{
			LcdPrintHz16x16((str[i]<<8)|str[i+1],px,y,color,bcolor);
			i += 2;
			px += 16;
		//	px2-=16;
		}
		else//非汉字
		{
			LcdPrintEn24x32(str[i],px, y, color,bcolor);
			i++;
			px += 24;
		}
	}
}

void Lcd_Str16(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i;
	u16 px;	
	i = 0;
	px = x;
//	px2=x+256;
	while(str[i])
	{
		if(str[i] > 0x7F)//是汉字
		{
			LcdPrintHz16x16((str[i]<<8)|str[i+1],px,y,color,bcolor);
			i += 2;
			px += 16;
		//	px2-=16;
		}
		else//非汉字
		{
			LcdPrintEn8x16(str[i],px, y, color,bcolor);
			i++;
			px += 8;
		}
	}
}
void Lcd_Str15(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i=0;
	u16 px;	
	px = x;
//	px2=x+256;
	while(str[i])
	{
		if(str[i] > 0x7F)//是汉字
		{
			LcdPrintHz16x16((str[i]<<8)|str[i+1],px,y,color,bcolor);
			i += 2;
			px += 16;
		//	px2-=16;
		}
		else//非汉字
		{
			LcdPrintEn8x16_2(str[i],px, y, color,bcolor);
			i++;
			px += 8;
		}
	}
}
void Lcd_Str10(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i=0;
	u16 px;	
	px = x;
//	px2=x+256;
	while(str[i])
	{
		if(str[i] > 0x7F)//是汉字
		{
			LcdPrintHz16x16((str[i]<<8)|str[i+1],px,y,color,bcolor);
			i += 2;
			px += 16;
		//	px2-=16;
		}
		else//非汉字
		{
			LcdPrintEn8x16_3(str[i],px, y, color,bcolor);
			i++;
			px += 4;
		}
	}
}
/**************************************************************************************
功能描述: 	在屏幕显示一个字符串	 (8x16的单体)
输    入: 	char* str       字符串指针
                u16 x,y		目标显示位置坐标
		u16 color	字体颜色
		u16 bcolor	背景颜色
输    出: 	无
**************************************************************************************/
void LcdPrintStr16_2(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i;
	u16 px;
	
	i = 0;
	px = x;
//	px2=x+256;
	while(str[i])
	{
		if(str[i] > 0x7F)//是汉字
		{
			LcdPrintHz16x16((str[i]<<8)|str[i+1],px,y,color,bcolor);
			i += 2;
			px += 16;
		//	px2-=16;
		}
		else//非汉字
		{
			LcdPrintEn8x16_2(str[i],px, y, color,bcolor);
			i++;
			px += 8;
		}
	}
}
/*void LcdPrintStr16_3(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i=0;
	u16 px;	
	px = x;
//	px2=x+256;
	while(str[i])
	{
		if(str[i] > 0x7F)//是汉字
		{
			LcdPrintHz16x16((str[i]<<8)|str[i+1],px,y,color,bcolor);
			i += 2;
			px += 16;
		//	px2-=16;
		}
		else//非汉字
		{
		//	LcdPrintEn8x16_3(str[i],px, y, color,bcolor);
			i++;
			px += 8;
		}
	}
}	 */
/**************************************************************************************
功能描述: 	在屏幕显示一个字符串  4x7
输    入: 	char* str       字符串指针
                u16 x,y		目标显示位置坐标
		u16 color	字体颜色
		u16 bcolor	背景颜色
输    出: 	无
**************************************************************************************/
void LcdPrintStr7(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i=0;
	u16 px;

	px = x;
	while(str[i])
	{
//		if(str[i] > 0x7F)//是汉字
//		{
//			LcdPrintHz16x16((str[i]<<8)|str[i+1],px,y,color,bcolor);
//			i += 2;
//			px += 16;
//		}
//		else//非汉字
//		{
			LcdPrintEn4x7(str[i],px, y, color,bcolor);
			i++;
			px += 5;
//		}
	}
}

/**************************************************************************************
功能描述: 	在屏幕显示一个11x17字符串
输    入: 	char* str       字符串指针
                u16 x,y		目标显示位置坐标
		u16 color	字体颜色
		u16 bcolor	背景颜色
输    出: 	无
**************************************************************************************/
void LcdPrintStr17(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i;
	u16 px;
	
	i = 0;
	px = x;
	while(str[i])
	{
//		if(str[i] > 0x7F)//是汉字
//		{
//			LcdPrintHz24x24((str[i]<<8)|str[i+1],px,y,color,bcolor);
//			i += 2;
//			px += 24;
//		}
//		else//非汉字
//		{
			LcdPrintEn11x17(str[i],px, y+1, color,bcolor);
			i++;
			px += 11;
//		}
	}
}

/**************************************************************************************
功能描述: 	在屏幕显示一个48x54字符串
输    入: 	char* str       字符串指针
                u16 x,y		目标显示位置坐标
		u16 color	字体颜色
		u16 bcolor	背景颜色
输    出: 	无
**************************************************************************************/
void LcdPrintStr54(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u16 i;
	u16 px;
	
	i = 0;
	px = x;
	while(str[i])
	{
//		if(str[i] > 0x7F)//是汉字
//		{
//			LcdPrintHz24x24((str[i]<<8)|str[i+1],px,y,color,bcolor);
//			i += 2;
//			px += 24;
//		}
//		else//非汉字
//		{
			LcdPrintEn48x54(str[i],px, y+1, color,bcolor);
			i++;
			px += 46;
//		}
	}
}  

/**************************************************************************************
功能描述: 	在屏幕显示一个13x24字符串
输    入: 	char* str       字符串指针
                u16 x,y		目标显示位置坐标
		u16 color	字体颜色
		u16 bcolor	背景颜色
输    出: 	无
**************************************************************************************/
void LcdPrintStr13_24(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i;
	u16 px;
	
	i = 0;
	px = x;
	while(str[i])
	{
//		if(str[i] > 0x7F)//是汉字
//		{
//			LcdPrintHz24x24((str[i]<<8)|str[i+1],px,y,color,bcolor);
//			i += 2;
//			px += 24;
//		}
//		else//非汉字
//		{
			LcdPrintEn13x24(str[i],px, y+1, color,bcolor);
			i++;
			px += 14;
//		}
	}
}
	  
/**************************************************************************************
功能描述: 	在屏幕显示一个10x15字符串
输    入: 	char* str       字符串指针
                u16 x,y		目标显示位置坐标
		u16 color	字体颜色
		u16 bcolor	背景颜色
输    出: 	无
**************************************************************************************/
void LcdPrintStr15(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i;
	u16 px;
	
	i = 0;
	px = x;
	while(str[i])
	{
//		if(str[i] > 0x7F)//是汉字
//		{
//			LcdPrintHz24x24((str[i]<<8)|str[i+1],px,y,color,bcolor);
//			i += 2;
//			px += 24;
//		}
//		else//非汉字
//		{
			LcdPrintEn10x15(str[i],px, y+1, color,bcolor);
			i++;
			px += 10;
//		}
	}
}
void LcdPrintStr14(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i;
	u16 px;
	
	i = 0;
	px = x;
	while(str[i])
	{
//		if(str[i] > 0x7F)//是汉字
//		{
//			LcdPrintHz24x24((str[i]<<8)|str[i+1],px,y,color,bcolor);
//			i += 2;
//			px += 24;
//		}
//		else//非汉字
//		{
			LcdPrintEn11x15(str[i],px, y+1, color,bcolor);
			i++;
			px += 9;
//		}
	}
}

/**************************************************************************************
功能描述: 	在屏幕显示一个24x24字符串
输    入: 	char* str       字符串指针
                u16 x,y		目标显示位置坐标
		u16 color	字体颜色
		u16 bcolor	背景颜色
输    出: 	无
**************************************************************************************/
void LcdPrintStr24(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i;
	u16 px;
	
	i = 0;
	px = x;
	while(str[i])
	{
		if(str[i] > 0x7F)//是汉字
		{
//			LcdPrintHz24x24((str[i]<<8)|str[i+1],px,y,color,bcolor);
			i += 2;
			px += 24;
		}
		else//非汉字
		{
			LcdPrintEn12x24(str[i],px, y+1, color,bcolor);
			i++;
			px += 12;
		}
	}
}	
/**************************************************************************************
功能描述: 	在屏幕显示一个24x24字符串
输    入: 	char* str       字符串指针
                u16 x,y		目标显示位置坐标
		u16 color	字体颜色
		u16 bcolor	背景颜色
输    出: 	无
**************************************************************************************/
/*void LcdPrintStr324(u8 *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i;
	u16 px;
	i = 0;
	px = x;
	while(str[i])
	{
		if(str[i] > 0x7F)//是汉字
		{
			LcdPrintHz24x24((str[i]<<8)|str[i+1],px,y,color,bcolor);
			i += 2;
			px += 32;
		}
		else//非汉字
		{
			LcdPrintEn12x24(str[i],px, y+1, color,bcolor);
			i++;
			px += 16;
		}
	}
}	   */
/**************************************************************************************
功能描述: 在屏幕按格式显示一个字符串
输    入:
输    出: 无
**************************************************************************************/
/*void LcdPrintf(u16 x,u16 y,u16 color,u16 bcolor,u8 *fmt,...)
{
    va_list ap;
    char str[64];

    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    Lcd_Str16(str,x,y,color,bcolor);
    va_end(ap);
}*/

/**************************************************************************************
功能描述: 在屏幕显示一 16bit bmp图形
输    入: const u8* bmpbit 	bmp图形数据数组,如果不是放在FLASH中的图片,请将const去掉
          u16 x,y		目标显示位置坐标
	  u16 width,height	图形的宽度和高度
输    出: 无
**************************************************************************************/
void LcdPrint16bitBmp(u8* bmpbit,u16 x,u16 y,u16 widht,u16 height)
{
      	u32 size,ip = 0;
      	
	LcdSetArea(x,y,x+widht-1,y+height-1);
	WriteCommand(0x2C);
	
	size = widht*height*2;
      	for(ip = 0; ip < size; ip += 2)
           	WriteData(bmpbit[ip]<<8|bmpbit[ip+1]);
}

/**************************************************************************************
功能描述: 在屏幕显示一 黑白 bmp图形
输    入: const u8* bmpbit 	bmp图形数据数组,如果不是放在FLASH中的图片,请将const去掉
          u16 x,y		目标显示位置坐标
	  u16 width,height	图形的宽度和高度
输    出: 无
**************************************************************************************/
void LcdPrintOnebitBmp(const u8* bmpbit,u16 x,u16 y,u16 width,u16 height,u16 color,u16 bcolor)
{
	u16 i,j,k;
	u32 ip=0;
	LcdSetArea(x,y,x+width-1,y+height-1);
	WriteCommand(0x2C);
	for(i=0;i<height;i++)
	{ 		for(j=0;j*8<width;j++)
		{			for(k=0;k<8;k++)
			{
				if(j*8+k==width) break;
				if(((bmpbit[ip]<<k)& 0x80) >0)
			 		WriteData(color)

			 	else
			 		WriteData(bcolor)
			}
			ip++;
			if(j*8+k==width) break;
		}
	}
}

/**************************************************************************************
功能描述: 在屏幕显示填充三角形图形
输    入:
          u16 x,y		目标显示位置坐标
输    出: 无
**************************************************************************************/
void LcdPrintTriangle(u16 x,u16 y,u16 line_color,u16 fill_color,u16 bg_color)
{
    u16 i,j;  	
	LcdSetArea(x,y,x+199,y+99);
	WriteCommand(0x2C);
	for(i=0;i<100;i++)
	{	for(j=0;j<200;j++)
		{//if(j==100-i || j==99-i || j==100+i || j==99+i)
			if(j<(100-i) || j>(100+i))
				WriteData(bg_color)	

			else if(j>(105-i) && j<(95+i) && i<96)
				 WriteData(fill_color)
			else
				 WriteData(line_color)
		}
	}
}

