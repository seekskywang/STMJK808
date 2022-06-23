#ifndef __TOUCH_H
#define __TOUCH_H

/* ������������ */
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
/* ����У���������� */
#define LCD_ADJX_MIN (10)                      //��ȡ�ĸ������СXֵ
#define LCD_ADJX_MAX (SCREEN_WIDTH-1 - LCD_ADJX_MIN) //��ȡ�ĸ�������Xֵ
#define LCD_ADJY_MIN (10)                      //��ȡ�ĸ������СYֵ
#define LCD_ADJY_MAX (SCREEN_HIGH-1 - LCD_ADJY_MIN) //��ȡ�ĸ�������Yֵ

#define LCD_ADJ_X (LCD_ADJX_MAX - LCD_ADJY_MIN)//��ȡ����Ŀ��
#define LCD_ADJ_Y (LCD_ADJY_MAX - LCD_ADJY_MIN)//��ȡ����ĸ߶�

#define TOUCH_MAX        30    //Ԥ�ڲ�ֵ
#define TOUCH_READ_TIMES 5     //һ�ζ�ȡ����ֵ�Ĵ���
#define TOUCH_Y_CMD      0xD0  //��ȡX������
#define TOUCH_X_CMD      0x90  //��ȡY������
void xpt2046_read(int32_t *x, int32_t *y);
void testtouch(void);
uint16_t TOUCH_ReadData(uint8_t cmd);
uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue);
extern TouchTypeDef TouchData;
void TOUCH_Adjust(void);
u8 TOUCH_SCAN(void);
void TOUCH_HANDLE(u16 x,u16 y);
#endif

