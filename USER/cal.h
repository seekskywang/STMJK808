

#define MAXCHN 8
#define MAXBUF 32

#define PREPARE_STARTUP   0
#define PREPARE_JP12_ON   1
#define PREPARE_50_OHM    2
#define PREPARE_300_OHM   3
#define PREPARE_500_OHM   4
#define PREPARE_3000_OHM  5
#define PREPARE_JPALL_OFF 6
#define PREPARE_N50_MV    7
#define PREPARE_50_MV     8
//#define PREPARE_1000_MV   9
//#define PREPARE_0_V       10
//#define PREPARE_10_V      11
#define CALIB_SAVE        12
#define CALIB_FINISH      13

#define STD_AD_PT100_50    1720740
#define STD_AD_PT100_300   10324441
#define STD_AD_PT1000_500  2150925
#define STD_AD_PT1000_3000 12905551

#define STD_AD_TJ_N50_MV -21474836
#define STD_AD_TJ_50_MV  21474836
#define STD_AD_LV_N50_MV -5368709
#define STD_AD_LV_50_MV  5368709

#define STD_AD_HV_0V   0
#define STD_AD_HV_10_V 495868
extern	uint8_t flag_cal,flag_send,typp;
extern	uint8_t left_right[MAXCHN];
extern	void DelayMs(u32 nMs);
//const u8 TXT_CALIB_MENU[][60] = {
//	"从不显示的空字符串",                  // #define PREPARE_STARTUP   0
//	"插入JP1,JP2, 然后点击NEXT",           // #define PREPARE_JP12_ON   1
//	"连接50欧姆电阻, 然后点击NEXT",        // #define PREPARE_50_OHM    2
//	"连接300欧姆电阻, 然后点击NEXT",       // #define PREPARE_300_OHM   3
//	"连接500欧姆电阻, 然后点击NEXT",       // #define PREPARE_500_OHM   4
//	"连接3000欧姆电阻, 然后点击NEXT",      // #define PREPARE_3000_OHM  5
//	"拔出所有跳线, 然后点击NEXT",          // #define PREPARE_JPALL_OFF 6
//	"在LOW端接入-50mV电压, 然后点击NEXT",  // #define PREPARE_0_MV      7
//	"在LOW端接入50mV电压, 然后点击NEXT",   // #define PREPARE_50_MV     8
//	"在LOW端接入1000mV电压, 然后点击NEXT", // #define PREPARE_1000_MV   9
//	"在LOW端接入0mV电压, 然后点击NEXT",    // #define PREPARE_0_V       10
//	"在LOW端接入10V电压, 然后点击NEXT",    // #define PREPARE_10_V      11
//	"点击NEXT以保存校准数据",              // #define CALIB_SAVE        12
//	"校准完成,点击NEXT查看结果",           // #define CALIB_FINISH      13
//	"采集板已退出校准",
//};
//extern void simple_polyfit(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t *A0, int32_t *A1);
extern	int64_t div64(int64_t num, int32_t div);
