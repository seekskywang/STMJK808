
#ifndef __LCD_H__
#define __LCD_H__
//-----------------------------------------------------------------------------------------------
#define __AT070TN83__

#ifdef	__AT070TN83__
	
	#define LCD_COMM_ADD		*((u16 *)0X60000000)
	#define LCD_DATA_ADD		*((u16 *)0X60020000)
	
	#define WriteCommand(cmd)	{LCD_COMM_ADD = cmd;}
	#define WriteData(data)		{LCD_DATA_ADD = data;}
	#define ReadData(data)		{data = LCD_DATA_ADD;}
	
	#define LCDBUFSIZE	240*320
	
	#define    DOTCOLOR		 RGB888TORGB565(0x82e6e6)	 //点和外框颜色	   bee6fb
#define    FILLBLOCK     RGB888TORGB565(0x0071ce)	  //填充颜色
#define    WIRE			 RGB888TORGB565(0x0033a1)	  //线的颜色
#define    FONTCOLOR     RGB888TORGB565(0xf0f032)	  //字体颜色黄色f0f032	    0xffe0
#define    BABYBLUE		 RGB888TORGB565(0x00dcdc)	   //淡蓝色	  4bc1e1
#define    BABYYELLOW    RGB888TORGB565(0xf0f032)   //淡黄色

#define    BUTTONCOLOR     RGB888TORGB565(0x000000)	  //黑
#define    TOPCOLOR     	 RGB888TORGB565(0x000EE00)	  //绿
#define    DATACOLOR     	 RGB888TORGB565(0xFFFFFF)	  //白
#define		 TCALCOLOR			 RGB888TORGB565(0xFF0000)//红
#define		 BLUE1					 RGB888TORGB565(0x0033ff)
#define		 ORANGE					 RGB888TORGB565(0xEE7621)
#define   GOLDYELLOW     RGB888TORGB565(0xeaa01e)

#define    RGB888_RED    0x00ff0000
#define    RGB888_GREEN  0x0000ff00
#define    RGB888_BLUE   0x000000ff	 
//	#define OpenEL()	 GPIO_SetBits(GPIOB, GPIO_Pin_8)
//	#define CloseEL()	 GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	
//	#define DISP_HOR_RESOLUTION		240//482
//	#define DISP_VER_RESOLUTION		320//272
//	#define DISP_HOR_PULSE_WIDTH	1
//	#define DISP_HOR_BACK_PORCH		45
//	#define DISP_HOR_FRONT_PORCH	45
//	#define DISP_VER_PULSE_WIDTH	1
//	#define DISP_VER_BACK_PORCH		10	
//	#define DISP_VER_FRONT_PORCH	10
//#define HT  531
//#define HPS 43
//#define LPS 8
//#define HPW 1

//#define VT  292
//#define VPS 12
//#define FPS 8
//#define VPW 1	
//	#define HT (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)
//	#define HPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)
//	#define VT (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_FRONT_PORCH+DISP_VER_RESOLUTION)
	#define VSP (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH)
	
//	#define Up2Down2Left2Right  0x01
//	#define Down2Up2Right2Left	0x02
//	#define Up2Down2Right2Left	0x00
//	#define Down2Up2Left2Right	0x03
#endif
extern u16 para1,para2,para3;

extern void screen_init(void);
extern void LcdClear(u16 color);
extern u16 ReadId(void);
extern u16 RGB888TORGB565(u32 color);

extern void LcdSetArea(u16 x1, u16 y1, u16 x2, u16 y2);
extern void LcdPrintDot(u16 x, u16 y, u16 color);
extern void LcdPrintHorz(u16 x, u16 y, u16 width, u16 color);
extern void LcdPrintVert(u16 x, u16 y, u16 height, u16 color);
extern void LcdPrintLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
extern void LcdPrintRec(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
extern void LcdPrintRecDot(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
extern void LcdFillRec(u16 x1, u16 y1,u16 x2, u16 y2, u16 color);
extern void LcdFillArea(u16 x1, u16 y1, u16 x2, u16 y2,u16 *color);
extern void LcdReadRec(u16 x1, u16 y1, u16 x2, u16 y2,u16 *color);
extern void LcdClear(u16 color);
extern void LcdPrintEn(u8 code, u16 x, u16 y, u16 color,u16 bcolor);
extern void LcdPrintHz(u16 code,u16 x,u16 y,u16 color,u16 bcolor);
extern void Lcd_Str32(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void Lcd_Str16(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void Lcd_Str15(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void Lcd_Str10(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void LcdPrintEn12x24(u8 code, u16 x, u16 y, u16 color,u16 bcolor);
extern void LcdPrintStr24(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void LcdPrintStr7(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void TFT_Draw_Point(u16 x,u8 y,u16 color);
//extern void LcdPrintf(u16 x,u16 y,u16 color,u16 bcolor,u8 *fmt,...);
//extern void LcdPrint16bitBmp(u8 *bmpbit,u16 x,u16 y,u16 widht,u16 height);
//extern void LcdPrintOnebitBmp(const u8 *bmpbit,u16 x,u16 y,u16 width,u16 height,u16 color,u16 bcolor);
//extern void LcdPrintStr22(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void LcdPrintStr324(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void LcdPrintStr17(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void LcdPrintStr14(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void LcdPrintStr13_24(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void LcdPrintStr15(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);

extern void LcdPrintStr54(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void LcdPrintStr16_2(u8 *str,u16 x,u16 y,u16 color,u16 bcolor);
//extern u16 ReadData(void);

#endif
