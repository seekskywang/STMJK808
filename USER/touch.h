#ifndef __TOUCH_H
#define __TOUCH_H

/* 定义数据类型 */
typedef struct 
{
	uint16_t x;
	uint16_t y;
    uint16_t lcdx;
    uint16_t lcdy;
} TouchTypeDef;

typedef struct{
    uint8_t posState;   
    int16_t xOffset;
    int16_t yOffset; 
    float xFactor;
    float yFactor;
} PosTypeDef;

#define CMD_READ_Y    0x90
#define CMD_READ_X    0xD0

#define MEAS_DELAY    36
#define INTV_DELAY    36

#define RTOUCH_MEAS_N  16

#define T_CS_LOW       (HAL_GPIO_WritePin(SPI3CS_GPIO_Port,SPI3CS_Pin,GPIO_PIN_RESET))
#define T_CS_HIGH      (HAL_GPIO_WritePin(SPI3CS_GPIO_Port,SPI3CS_Pin,GPIO_PIN_SET))
#define	TOUCH_IN  			HAL_GPIO_ReadPin(PEN_GPIO_Port, PEN_Pin)
/* 触摸校正因数设置 */
#define LCD_ADJX_MIN (10)                      //读取四个点的最小X值
#define LCD_ADJX_MAX (SCREEN_WIDTH-1 - LCD_ADJX_MIN) //读取四个点的最大X值
#define LCD_ADJY_MIN (10)                      //读取四个点的最小Y值
#define LCD_ADJY_MAX (SCREEN_HIGH-1 - LCD_ADJY_MIN) //读取四个点的最大Y值

#define LCD_ADJ_X (LCD_ADJX_MAX - LCD_ADJY_MIN)//读取方框的宽度
#define LCD_ADJ_Y (LCD_ADJY_MAX - LCD_ADJY_MIN)//读取方框的高度

#define TOUCH_MAX        30    //预期差值
#define TOUCH_READ_TIMES 5     //一次读取触摸值的次数
#define TOUCH_Y_CMD      0xD0  //读取X轴命令
#define TOUCH_X_CMD      0x90  //读取Y轴命令
void xpt2046_read(int32_t *x, int32_t *y);
void testtouch(void);
uint16_t TOUCH_ReadData(uint8_t cmd);
uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue);
extern TouchTypeDef TouchData;
void TOUCH_Adjust(void);
u8 TOUCH_SCAN(void);
void TOUCH_HANDLE(u16 x,u16 y);
#endif

