#ifndef __AD_H
#define __AD_H
#include "main.h"
#pragma anon_unions
//#define MAXCHN	8
#define AD7124_4BRUZ

#define ADVAL_INVALID 0x7FFFFFFF /* ��Ч��AD����ֵ */
/*const uint16_t  tab_res[]=	{0x718,//0xcd4,
			0x2710,	   //0.0c
			0x97f4};*/	//845.0c
#define REG_STATUS   0x00 /* !!! 8 bit R   RESET = 0x00     */
#define REG_CONTROL  0x01 /* !!! 16 bit RW  RESET = 0x0000   */
#define REG_DATA     0x02 /* !!! 24 bit R   RESET = 0x000000 */
#define REG_IOCTRL1  0x03 /* !!! 24 bit RW  RESET = 0x000000 */
#define REG_IOCTRL2  0x04 /* !!! 16 bit RW  RESET = 0x0000   */
#define REG_ID       0x05 /*      8 bit R   RESET = 0x04/06  */
#define REG_ERROR    0x06 /* !!! 24 bit R   RESET = 0x000000 */
#define REG_ERR_EN   0x07 /* !!! 24 bit RW  RESET = 0x000040 */
#define REG_MCLKCNT  0x08 /*      8 bit R   RESET = 0x00     */
#define REG_CHANNEL0 0x09 /* !!! 16 bit RW  RESET = 0x8001   */
#define REG_CONFIG0  0x19 /* !!! 16 bit RW  RESET = 0x0860   */
#define REG_FILTER0  0x21 /* !!! 24 bit RW  RESET = 0x060180 */
#define REG_OFFSET0  0x29 /* !!! 24 bit RW  RESET = 0x800000 */
#define REG_FULLCB0  0x31 /* !!! 24 bit RW  RESET = 0x5XXXXX */

void AD_CS1(void){	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);}
void AD_CS0(void){	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);}


#define AD7124_AIN0  0
#define AD7124_AIN1  1
#define AD7124_AIN2  2
#define AD7124_AIN3  3
#define AD7124_AIN4  4
#define AD7124_AIN5  5
#define AD7124_AIN6  6
#define AD7124_AIN7  7
#define AD7124_AIN8  8
#define AD7124_AIN9  9
#define AD7124_AIN10 10
#define AD7124_AIN11 11
#define AD7124_AIN12 12
#define AD7124_AIN13 13
#define AD7124_AIN14 14
#define AD7124_AIN15 15

#define AD7124_UNIPOLAR    0
#define AD7124_BIPOLAR     1
#define AD7124_BIASX       0
#define AD7124_IOOUT_AINX  0
#define AD7124_REFSEL_EXT1 0
#define AD7124_REFSEL_EXT2 1
#define AD7124_REFSEL_INT  2
#define AD7124_REFSEL_AVDD 3

#define AD7124_IOUT_CLOSE  0
#define AD7124_IOUT_50UA   1
#define AD7124_IOUT_100UA  2
#define AD7124_IOUT_250UA  3
#define AD7124_IOUT_500UA  4
#define AD7124_IOUT_750UA  5
#define AD7124_IOUT_1000UA 6

#ifdef AD7124_4BRUZ
#define AD7124_IOOUT_AIN0 0
#define AD7124_IOOUT_AIN1 1
#define AD7124_IOOUT_AIN2 4
#define AD7124_IOOUT_AIN3 5
#define AD7124_IOOUT_AIN4 10
#define AD7124_IOOUT_AIN5 11
#define AD7124_IOOUT_AIN6 14
#define AD7124_IOOUT_AIN7 15

#define AD7124_BIAS0 0x0001
#define AD7124_BIAS1 0x0002
#define AD7124_BIAS2 0x0010
#define AD7124_BIAS3 0x0020
#define AD7124_BIAS4 0x0400
#define AD7124_BIAS5 0x0800
#define AD7124_BIAS6 0x4000
#define AD7124_BIAS7 0x8000

const uint16_t AD7124_FS_TBL[] = {
    12,   // ����
    12,   // Ŀ��    1ms, ʵ��   2.5ms + ����
    12,   // Ŀ��    5ms, ʵ��   2.5ms + ����
    36,   // Ŀ��   10ms, ʵ��   7.5ms + ����
    80,   // Ŀ��   20ms, ʵ�� 16.67ms + ����
    216,  // Ŀ��   50ms, ʵ��    45ms + ����
    384,  // Ŀ��  100ms, ʵ��    80ms + ����
    864,  // Ŀ��  200ms, ʵ��   180ms + ����
    1920, // Ŀ��  500ms, ʵ��   400ms + ����
    1920, // Ŀ�� 1000ms, ʵ��   400ms + ����
    1920, // Ŀ�� 2000ms, ʵ��   400ms + ����
    1920, // Ŀ�� 5000ms, ʵ��   400ms + ����
};

typedef struct {    // Ad7124��ͨ��ģʽ����
  uint8_t ainp;     // AINP����ѡ��
  uint8_t ainm;     // AINM����ѡ��
  uint8_t bipolar;  // 1˫�� 0����
  uint8_t ref;      // 0: REF1  1:REF2  2:�ڲ���׼  3:AVdd
  uint8_t iout0;    // IOUT0��������ֵ
  uint8_t iout1;    // IOUT1��������ֵ
  uint8_t iout0_ch; // IOUT0ͨ��ѡ��
  uint8_t iout1_ch; // IOUT1ͨ��ѡ��
  uint16_t vbias;   // ƫ�õ�ѹ��ʩ�ӵ�ͨ��λͼ
} Ad7124Config;

//#define MAXBUF 32
typedef struct {              // ģ��ͨ��������ʱ���ݽṹ
  uint32_t buf_dat24[MAXBUF]; // DAT24�������
  uint32_t dat24;             // ͨ������DAT24���
  int32_t dat;                // ͨ������ԭʼ���
  int32_t ad;                 // ͨ������AD���
  uint8_t buf_total;          // ���������ݸ���
  uint8_t buf_ipos;           // ������λ��
  uint8_t buf_opos;           // ������λ��
  uint8_t pga;                // ͨ����ǰ����,��ָ����ʽ��ʾ
  int32_t dzwd;               // ��ǰͨ�������¶�
  uint32_t dzwd24;            // ��ǰͨ�������¶�DAT24
} ChnAnalog;

const Ad7124Config ADCFG_L_3PT = {AD7124_AIN1,        AD7124_AIN2,       AD7124_UNIPOLAR,
                                  AD7124_REFSEL_EXT1, AD7124_IOUT_100UA, AD7124_IOUT_100UA,
                                  AD7124_IOOUT_AIN0,  AD7124_IOOUT_AIN3, AD7124_BIASX};

const Ad7124Config ADCFG_R_3PT = {AD7124_AIN1,        AD7124_AIN2,       AD7124_UNIPOLAR,
                                  AD7124_REFSEL_EXT1, AD7124_IOUT_100UA, AD7124_IOUT_100UA,
                                  AD7124_IOOUT_AIN3,  AD7124_IOOUT_AIN0, AD7124_BIASX};

const Ad7124Config ADCFG_TCOUPLE = {AD7124_AIN1,       AD7124_AIN2,       AD7124_BIPOLAR,
                                    AD7124_REFSEL_INT, AD7124_IOUT_CLOSE, AD7124_IOUT_CLOSE,
                                    AD7124_IOOUT_AINX, AD7124_IOOUT_AINX, AD7124_BIAS3};

void	CH1OFF(void) {  HAL_GPIO_WritePin(CH1_GPIO_Port, CH1_Pin,GPIO_PIN_SET);}
void	CH1ON(void) {   HAL_GPIO_WritePin(CH1_GPIO_Port, CH1_Pin,GPIO_PIN_RESET);}

void	CH2OFF(void) {  HAL_GPIO_WritePin(CH2_GPIO_Port, CH2_Pin,GPIO_PIN_SET);}
void	CH2ON(void)  {   HAL_GPIO_WritePin(CH2_GPIO_Port, CH2_Pin,GPIO_PIN_RESET);}

void	CH3OFF(void){   HAL_GPIO_WritePin(CH3_GPIO_Port, CH3_Pin,GPIO_PIN_SET);}
void	CH3ON(void)	{   HAL_GPIO_WritePin(CH3_GPIO_Port, CH3_Pin,GPIO_PIN_RESET);}

void	CH4OFF(void) {   HAL_GPIO_WritePin(CH4_GPIO_Port, CH4_Pin,GPIO_PIN_SET);}
void	CH4ON(void) {   HAL_GPIO_WritePin(CH4_GPIO_Port, CH4_Pin,GPIO_PIN_RESET);}

void	CH5OFF(void) {   HAL_GPIO_WritePin(CH5_GPIO_Port, CH5_Pin,GPIO_PIN_SET);}
void	CH5ON(void) {   HAL_GPIO_WritePin(CH5_GPIO_Port, CH5_Pin,GPIO_PIN_RESET);}

void	CH6OFF(void) {   HAL_GPIO_WritePin(CH6_GPIO_Port, CH6_Pin,GPIO_PIN_SET);}
void	CH6ON(void) {   HAL_GPIO_WritePin(CH6_GPIO_Port, CH6_Pin,GPIO_PIN_RESET);}

void	CH7OFF(void) {   HAL_GPIO_WritePin(CH7_GPIO_Port, CH7_Pin,GPIO_PIN_SET);}
void	CH7ON(void) {   HAL_GPIO_WritePin(CH7_GPIO_Port, CH7_Pin,GPIO_PIN_RESET);}

void	CH8OFF(void) {   HAL_GPIO_WritePin(CH8_GPIO_Port, CH8_Pin,GPIO_PIN_SET);}
void	CH8ON(void) {   HAL_GPIO_WritePin(CH8_GPIO_Port, CH8_Pin,GPIO_PIN_RESET);}

#ifdef AD7124_4BRUZ
static union {
  uint32_t all;
  struct {
    uint32_t iout0_ch : 4;
    uint32_t iout1_ch : 4;
    uint32_t iout0 : 3;
    uint32_t iout1 : 3;
    uint32_t : 1;
    uint32_t pdsw : 1;
    uint32_t : 2;
    uint32_t gpio_ctrl1 : 1;
    uint32_t gpio_ctrl2 : 1;
    uint32_t : 2;
    uint32_t gpio_dat1 : 1;
    uint32_t gpio_dat2 : 1;
    uint32_t : 8;
  };
} reg_ioctrl1;
#endif

extern const Ad7124Config ADCFG_L_3PT;
extern const Ad7124Config ADCFG_R_3PT;
//extern const Ad7124Config ADCFG_2PT_4PT;
extern const Ad7124Config ADCFG_TCOUPLE;
//extern const Ad7124Config ADCFG_MVOLT;
//extern const Ad7124Config ADCFG_HVOLT;
//extern const Ad7124Config ADCFG_4TO20MA;
//extern const Ad7124Config ADCFG_20MVOLT;

static union {
  uint16_t all;
  struct {
    uint16_t pga : 3;
    uint16_t ref_sel : 2;
    uint16_t ain_bufm : 1;
    uint16_t ain_bufp : 1;
    uint16_t ref_bufm : 1;
    uint16_t ref_bufp : 1;
    uint16_t burnout : 2;
    uint16_t bipolar : 1;
    uint16_t : 4;
  };
} reg_config0;

static union {
  uint16_t all;
  struct {
    uint16_t ainm : 5;
    uint16_t ainp : 5;
    uint16_t : 2;
    uint16_t setup : 3;
    uint16_t enable : 1;
  };
} reg_channel0;

static union {
  uint32_t all;
  struct {
    uint32_t fs : 11;
    uint32_t : 5;

    uint32_t single_cycle : 1;
    uint32_t post_filter : 3;
    uint32_t rej60 : 1;
    uint32_t filter : 3;
    uint32_t : 8;
  };
} reg_filter0;

static union {
  uint8_t all;
  struct {
    uint8_t ch_active : 4;
    uint8_t por_flag : 1;
    uint8_t : 1;
    uint8_t error_flag : 1;
    uint8_t nrdy : 1;
  }; 
} reg_status;

static union {
  uint16_t all;
  struct {
    uint16_t clk_sel : 2;
    uint16_t mode : 4;
    uint16_t power_mode : 2;
    uint16_t ref_en : 1;
    uint16_t ncs_en : 1;
    uint16_t data_status : 1;
    uint16_t cont_read : 1;
    uint16_t nrdy_del : 1;
    uint16_t : 3;
  };
} reg_control;

#define AC30_TIME_OPEN 500
#define AC30_TIME_SHUT 990
ChnAnalog g_chnana[MAXCHN]; // ģ��ͨ��������ʱ����


static const uint16_t PERIOD_TBL[] = {5, 5, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000};

#endif
#endif
