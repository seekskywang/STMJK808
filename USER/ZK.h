#ifndef __ZK_H
#define __ZK_H
//#include "stm32f10x_lib.h"
#include "main.h"
struct __ASC_ZK_8x16
{
	u8 code;
	u8 ENCODE[16];
};

struct __ASC_ZK_16x32
{
	u8 code;
	u8 ENCODE[64];
};
struct typFNT_GB16_16x16                 // 汉字字模数据结构
{
    u8 code[2];             // 汉字内码索引
    u8 Msk[32];                       // 点阵码数据
};

struct __ASC_ZK_12x24
{
	u8 code;
	u8 ENCODE[48];
};

struct typFNT_GB16_24x24                 // 汉字字模数据结构
{
       u8 code[2];               // 汉字内码索引
       u8 Msk[72];                       // 点阵码数据
};
struct typFNT_GB16_32x33                 // 汉字字模数据结构
{
       u8 code[2];               // 汉字内码索引
       u8 Msk[132];                       // 点阵码数据
};
/*struct typFNT_GB16_16x22
{
	u8 code[2]; 			  // ??????
	u8 Msk[44]; 					  // ?????

};
   */
struct typFNT_ZK8_8x22                 // ????????
{
       u8 code;               // ??????
       u8 ENCODE[22];                       // ?????
};

struct __ASC_ZK_4x7
{
	     u8 code;
		 u8 ENCODE[7]; 

};
struct __ASC_ZK_11x17
{
	u8 code;
	u8 ENCODE[30];
};
struct __ASC_ZK_11x15
{
	u8 code;
	u8 ENCODE[26];
};
struct __ASC_ZK_13x24
{
	u8 code;
	u8 ENCODE[48];
};
struct __ASC_ZK_10x15
{
	u8 code;
	u8 ENCODE[26];
};

struct __ASC_ZK_48x54
{
	u8 code;
	u8 ENCODE[324];
};

struct __ASC_ZK_8x16_2
{
	u8 code;
	u8 ENCODE[11];
};
struct __ASC_ZK_8x16_3
{
	u8 code;
	u8 ENCODE[7];
};

extern const struct __ASC_ZK_16x32  ZK_16x32[];
extern const struct __ASC_ZK_8x16  ZK_8x16[];
extern const struct __ASC_ZK_12x24 ZK_12x24[];
extern const struct typFNT_GB16_16x16 GB_16[];
//extern const struct typFNT_GB16_24x24 GB_24[];
//extern const struct typFNT_GB16_32x33 GB_324[];
//extern const struct typFNT_ZK8_8x22 ZK_8X22[];
//extern const struct typFNT_GB16_16x22 GB_16X22[];
extern const struct __ASC_ZK_4x7  ZK_4x7[];
extern const struct __ASC_ZK_11x17 ZK_11x17[];
extern const struct __ASC_ZK_11x15 ZK_11x15[];

extern const struct __ASC_ZK_13x24  ZK_13x24[];
extern const struct __ASC_ZK_10x15  ZK_10x15[];

extern const struct	__ASC_ZK_48x54  ZK_48x54[];
extern const struct __ASC_ZK_8x16_2  ZK_8x16_2[];
extern const struct __ASC_ZK_8x16_3  ZK_8x16_3[];
#endif
