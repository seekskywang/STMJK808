#include "main.h"
int16_t CurrentTemp[8];
//#define MAXCHN	16

typedef struct {  // 热电偶查询表的描述信息
  int32_t idx0wd; // 表起始对应温度
  int32_t wdmin;  // 量程最小温度
  int32_t wdmax;  // 量程最大温度
} TcTblDesc;

typedef struct { // 热电阻查询表的描述信息
  int32_t ptn;   // 表元素个数
  int32_t wdmin; // 量程最小温度
  int32_t wdmax; // 量程最大温度
} PtTblDesc;
const PtTblDesc PT100_DESC  = {1001, -200000, 800000};
const PtTblDesc PT1000_DESC = {1001, -200000, 800000};
extern const PtTblDesc PT100_DESC;
extern const PtTblDesc PT1000_DESC;

extern const TcTblDesc J_TC_DESC;
extern const TcTblDesc K_TC_DESC;
extern const TcTblDesc E_TC_DESC;
extern const TcTblDesc T_TC_DESC;
extern const TcTblDesc R_TC_DESC;
extern const TcTblDesc S_TC_DESC;
extern const TcTblDesc B_TC_DESC;
extern const TcTblDesc N_TC_DESC;

extern const int32_t PT100_TBL[];
extern const int32_t PT1000_TBL[];

extern const int32_t K_TC_TABLE[];
extern const int32_t J_TC_TABLE[];
extern const int32_t T_TC_TABLE[];
extern const int32_t E_TC_TABLE[];
extern const int32_t R_TC_TABLE[];
extern const int32_t S_TC_TABLE[];
extern const int32_t B_TC_TABLE[];
extern const int32_t N_TC_TABLE[];

extern int32_t __pt_meas(const PtTblDesc *pd, const int32_t *pttbl, int32_t ad);
extern int32_t __tc_meas(const TcTblDesc *pd, const int32_t *tctbl, int32_t ad, int32_t bt);
extern	u32 channel_read(uint8_t chn,uint8_t type);
extern	void tmp0(void);
uint8_t cal_stepx;
extern	uint8_t left_right[MAXCHN];
int32_t pt100_50ohm_adbuf[MAXCHN][32];
int32_t pt100_300ohm_adbuf[MAXCHN][32];
int32_t pt1000_500ohm_adbuf[MAXCHN][32];
int32_t pt1000_3000ohm_adbuf[MAXCHN][32];
int32_t tj_n50mv_adbuf[32];
int32_t tj_50mv_adbuf[32];
static  int32_t pt100_50ohm_adaver[MAXCHN];
static  int32_t pt100_300ohm_adaver[MAXCHN];
//static int32_t l3pt1000_500ohm_adaver[MAXCHN];
//static int32_t l3pt1000_3000ohm_adaver[MAXCHN];
static int32_t tj_n50mv_adaver;
static int32_t tj_50mv_adaver;

u8 cal_tims=0;
uint8_t flag_cal;

uint16_t batt,bat_v;
int32_t	tmp_emp[3]={0};
union	UU	enir_temp;
uint32_t pt_ad1[16],pt_ad2[16];//,pc_ad1[16],pc_ad2[16];
uint8_t tempOVER;
int64_t div64(int64_t num, int32_t div) {
  int64_t quot,qt = 0;
  uint64_t numer,divv,wh, wl, q, r;
  int flag;
  if ((num >= 0) && (div >= 0)) { flag  = 0;
    numer = num;
    divv = div;  
  } else if ((num >= 0) && (div < 0)) {	flag  = 1;
    numer = num;
    divv = 0u - (uint32_t)div;    
  } else if ((num < 0) && (div >= 0)) {	flag  = 2;
    numer = UINT64_C(0) - (uint64_t)num;
    divv = div;    
  } else { flag  = 3;
    numer = UINT64_C(0) - (uint64_t)num;
    divv = 0u - (uint32_t)div;   
  }
  while (numer > UINT32_MAX) {
    wh = numer >> 32;
    wl = (uint32_t)numer;
    q  = UINT32_MAX / divv;
    r  = UINT32_MAX - q * divv;
    qt += (uint64_t)wh * q;
    numer = (uint64_t)wh * (r + 1) + wl;
  }
  q = (uint32_t)numer / divv;//  r = (uint32_t)numer - q * divv;
  qt += q;
  if (flag == 0)     quot = qt;
  else if (flag == 1) quot = -qt;
  else if (flag == 2) quot = -qt;
  else     quot = qt;
  return quot;
}

// 线性校准, x' = x * A0 / 10000 + A1	PT10s LinearCoeff
int32_t linear_calib(int32_t x,  int32_t k, int32_t b) {
  int64_t x64 = x;
  x64 *= (int64_t)(k);
 x = (int32_t)(div64(x64,10000));
  x += b;
  return x;
}
// 冒泡排序
void bubble_sort(int32_t *buf, int32_t len) {
	int i,j;
	int32_t tmp;
  for ( i= 0; i < len - 1; i++) {
    for ( j = 0; j < len - 1 - i; j++) {
      if (buf[j] > buf[j + 1]) {
        tmp = buf[j];
        buf[j]      = buf[j + 1];
        buf[j + 1]  = tmp;
      }
    }
  }
}


int32_t simple_k(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
	int32_t A0;
	int64_t t64;
	t64 = ((int64_t)(y2 - y1)) * 10000;
	if((x2 - x1)==0)
	A0=10000;
	else
	A0  = (int32_t)div64(t64, x2 - x1);
	return A0;
}
int32_t simple_t(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
	int32_t A1;
	int64_t t64;
	t64   = (int64_t)x1 * (int64_t)y2 - (int64_t)x2 * (int64_t)y1;
	if((x1 - x2)==0)
	A1=0;
	else
	A1    = (int32_t)div64(t64, x1 - x2);
	return A1;
}

static void calib_doit(void) {
	int32_t A0, A1;
	u8 ch=0;
  for ( ch= 0; ch < MAXCHN ; ch++)
   {
	A0=simple_k(pt100_50ohm_adaver[ch], STD_AD_PT100_50, pt100_300ohm_adaver[ch],STD_AD_PT100_300);
	A1=simple_t(pt100_50ohm_adaver[ch], STD_AD_PT100_50, pt100_300ohm_adaver[ch],STD_AD_PT100_300);
	LinearCoeff.Pt100[ch].Ks = A0;
    LinearCoeff.Pt100[ch].Bt = A1;
//   simple_polyfit(l3pt1000_500ohm_adaver[ch], STD_AD_PT1000_500, l3pt1000_3000ohm_adaver[ch],
//                  STD_AD_PT1000_3000, &A0, &A1);
//	A0=simple_k(l3pt1000_500ohm_adaver[ch], STD_AD_PT1000_500, l3pt1000_3000ohm_adaver[ch],STD_AD_PT1000_3000);
//	A1=simple_t(l3pt1000_500ohm_adaver[ch], STD_AD_PT1000_500, l3pt1000_3000ohm_adaver[ch],STD_AD_PT1000_3000);
	LinearCoeff.Pt1000[ch].Ks = A0;
    LinearCoeff.Pt1000[ch].Bt = A1;
  }
	A0=simple_k(tj_n50mv_adaver, STD_AD_TJ_N50_MV, tj_50mv_adaver, STD_AD_TJ_50_MV);
	A1=simple_t(tj_n50mv_adaver, STD_AD_TJ_N50_MV, tj_50mv_adaver, STD_AD_TJ_50_MV);
//  simple_polyfit(tj_n50mv_adaver, STD_AD_TJ_N50_MV, tj_50mv_adaver, STD_AD_TJ_50_MV, &A0, &A1);
  for ( ch = 0; ch < MAXCHN; ch++) {
    LinearCoeff.JKT_[ch].Ks = A0;
    LinearCoeff.JKT_[ch].Bt = A1;
  }
}

static void JKT_CAL_PROCESS(void)
{
	int32_t A0, A1;
	u8 ch=0;
	
	A0=simple_k(tj_n50mv_adaver, STD_AD_TJ_N50_MV, tj_50mv_adaver, STD_AD_TJ_50_MV);
	A1=simple_t(tj_n50mv_adaver, STD_AD_TJ_N50_MV, tj_50mv_adaver, STD_AD_TJ_50_MV);
//  simple_polyfit(tj_n50mv_adaver, STD_AD_TJ_N50_MV, tj_50mv_adaver, STD_AD_TJ_50_MV, &A0, &A1);
  for ( ch = 0; ch < MAXCHN; ch++) {
    LinearCoeff.JKT_[ch].Ks = A0;
    LinearCoeff.JKT_[ch].Bt = A1;
  }
}
static void dwin_print_n50_mv(void) 
{
	int32_t sum = 0;
	u8 i;
  bubble_sort(tj_n50mv_adbuf, 32);
  for (i = 10; i < 18; i++) sum += tj_n50mv_adbuf[i];
  sum>>=3;
  tj_n50mv_adaver = (sum-0x800000)*4;
}

static void dwin_print_50_mv(void) 
{
  int32_t sum = 0;
  u8 i;
  bubble_sort(tj_50mv_adbuf, 32);
  for ( i= 10; i < 18; i++) sum += tj_50mv_adbuf[i];
	sum>>=3;
  tj_50mv_adaver = (sum-0x800000)*4;
}


void proc_send_dw(int ch) 
{


    switch (flag_cal) 
		{	
//		 case PREPARE_50_OHM: // 2 已就绪
//	if(flag_cal==10)	{	flag_cal=5;					 //ready next ad
// if(cal_tims)   dwin_progress(ch,cal_tims);
//		DelayMs(150);
//	   		 }
//		if(cal_stepx==0xA0){DelayMs(200);
//	  dwin_print_pt100_50();
//	   DelayMs(250);
//	  dwin_print_text(TXT_CALIB_MENU[3]);//	"连接300欧姆电阻, 然后点击NEXT",   ++calib_step
//	  cal_stepx++;
//	  flag_cal=0;	  }
//      break;

//    case PREPARE_300_OHM: // 3 已就绪
// 		if(flag_cal==10){	flag_cal=5;
//	if(cal_tims)  dwin_progress(ch,cal_tims);
//		DelayMs(150);
//      	 }
//		 if(cal_stepx==0xA0){DelayMs(200);
//	  dwin_print_pt100_300();
//	  DelayMs(250);
//	  cal_stepx++;
//	  flag_cal=0;
//	  dwin_print_text(TXT_CALIB_MENU[6]);//6	"拔出所有跳线, 然后点击NEXT",++calib_step
//	  	  }
//	  break;


    case PREPARE_N50_MV: // 7 已就绪
		{
		  dwin_print_n50_mv();
		  cal_stepx++;
	    flag_cal=0;
	  }break;

    case PREPARE_50_MV: // 8 已就绪
		{
			dwin_print_50_mv();
			cal_stepx++;
			flag_cal=0;
	  }break;

    case CALIB_SAVE: // 12 已就绪
		  flag_cal=0;
		  calib_doit();
		  SaveDevPara(LinearCoeff);
    break;

    case CALIB_FINISH: // 13 已就绪
			flag_cal=0;
			Test.f_run=RUN_t;
    break;	
	}
}


//void rd_cal_ad(u8 ch)
//{   int32_t adi,adx;
//	 if(cal_stepx==2||cal_stepx==3) adi = channel_read(ch,PT100);
//	 if(cal_stepx==8||cal_stepx==7) adi = channel_read(1,TYPE_K);

//	 if(left_right[ch])	 	 pt_ad1[ch]=adi;
//	 else				 	 pt_ad2[ch]=adi;
//	 
//	 if(left_right[ch]==0&&flag_cal==5)
//	 { adx=pt_ad1[ch]/2+pt_ad2[ch]/2;
//	 if(cal_stepx==2)pt100_50ohm_adbuf[ch][cal_tims]=adx;
//	 if(cal_stepx==3)pt100_300ohm_adbuf[ch][cal_tims]=adx;
////	 if(cal_stepx==4)pt1000_500ohm_adbuf[ch][cal_tims]=adx;
////	 if(cal_stepx==5)pt1000_3000ohm_adbuf[ch][cal_tims]=adx;
//	 if(cal_stepx==7)tj_n50mv_adbuf[cal_tims]=adi;
//	 if(cal_stepx==8)tj_50mv_adbuf[cal_tims]=adi;

//	 flag_cal=10;
//	 cal_tims++;
//	 }
//	 	 if(cal_tims>31){ cal_tims=0;
//	 	Test.ch++;
//	 if(cal_stepx==7||cal_stepx==8){
//	 	flag_cal=7;
//	 	cal_stepx=0xA0;
//	 	Test.ch=0;
//	 }

//	 if(Test.ch>SYSPAR.chnum-1){
//	 flag_cal=7;
//	 	cal_stepx=0xA0;
//	 	Test.ch=0;	 }
//	 		}
//}

void cal_process(void)
{
	u8 i;
	int32_t adi;
	if(calflag == 1)
	{
		for(i=0;i<32;i++)
		{
			adi = channel_read(0,TYPE_K);
			tj_n50mv_adbuf[i]=adi;
		}
		dwin_print_n50_mv();
		calflag = 0;
	}else if(calflag == 2){
		for(i=0;i<32;i++)
		{
			adi = channel_read(0,TYPE_K);
			tj_50mv_adbuf[i]=adi;
		}
		dwin_print_50_mv();
		JKT_CAL_PROCESS();
		SaveDevPara(LinearCoeff);
		calflag = 0;
		Test.f_run=RUN_t;
	}else if(calflag == 3){
		
	}else if(calflag == 4){
		
	}
}
 //华氏度：F=32+1.8×C
int16_t TempC2K(int16_t tempx)
{
  int32_t temp;
  int16_t f;
  temp = tempx *18;
  f = (int16_t)(temp/10 + 320);
  f++;
  return f;
}

int16_t TempK2C(int16_t tempx)
{
  int32_t temp;
  int16_t c;
  temp = 10 * (tempx-320);
  c = (int16_t)(temp /18);
  return c;
}

extern  int16_t Tenir;
//extern void Uart4Send(u8 buf[],u8 end);
void ad_pro(u8 ch,u8 typ)
   {	static int32_t adi,adt,adf,ctemp,tmk;
		static u8 enir_t;//,c_tim[16]={0};
		adi = channel_read(ch,typ);
		if(tempOVER==0)
	{	if(left_right[ch])	 	 pt_ad1[ch]=adi;

	 else	pt_ad2[ch]=adi;
	 adi=(pt_ad1[ch]+pt_ad2[ch])/2;

tmk=Tenir*100;

//CurrentTemp[1]=	  Tenir;
		switch(typ){
//	 case TYPE_NC:tempOVER=1;
//	 break;
	 case PT100:

	 adt=linear_calib(adi,LinearCoeff.Pt100[ch].Ks,LinearCoeff.Pt100[ch].Bt );
	 ctemp=__pt_meas(&PT100_DESC, PT100_TBL, adt);
	 break;
	 
	 case TYPE_K:adt=(adi-0x800000)*4; //LinearCoeff.JKT_[ch].Ks=10000;LinearCoeff.JKT_[ch].Bt=0;
		 adf=linear_calib(adt, LinearCoeff.JKT_[ch].Ks,LinearCoeff.JKT_[ch].Bt);
	 ctemp=__tc_meas(&K_TC_DESC, K_TC_TABLE, adf,tmk);
	 break;
	 
	case TYPE_J:adt=(adi-0x800000)*4;
		adf=linear_calib(adt, LinearCoeff.JKT_[ch].Ks,LinearCoeff.JKT_[ch].Bt);
	 ctemp=__tc_meas(&J_TC_DESC, J_TC_TABLE, adf,tmk);
	 break;
	case TYPE_T:adt=(adi-0x800000)*4;
		adf=linear_calib(adt, LinearCoeff.JKT_[ch].Ks,LinearCoeff.JKT_[ch].Bt);
	 ctemp=__tc_meas(&T_TC_DESC, T_TC_TABLE, adf,tmk);
	 break;
	case TYPE_E:adt=(adi-0x800000)*4;
		 adf=linear_calib(adt, LinearCoeff.JKT_[ch].Ks,LinearCoeff.JKT_[ch].Bt);
	 ctemp=__tc_meas(&E_TC_DESC, E_TC_TABLE, adf,tmk);
	 break;
	case TYPE_R:adt=(adi-0x800000)*4;
		 adf=linear_calib(adt, LinearCoeff.JKT_[ch].Ks,LinearCoeff.JKT_[ch].Bt);
	 ctemp=__tc_meas(&R_TC_DESC, R_TC_TABLE, adf,tmk);
	 break;
	case TYPE_S:adt=(adi-0x800000)*4;
		 adf=linear_calib(adt, LinearCoeff.JKT_[ch].Ks,LinearCoeff.JKT_[ch].Bt);
	 ctemp=__tc_meas(&S_TC_DESC, S_TC_TABLE, adf,tmk);
	 break;
	case TYPE_B:adt=(adi-0x800000)*4;
		 adf=linear_calib(adt, LinearCoeff.JKT_[ch].Ks,LinearCoeff.JKT_[ch].Bt);
	 ctemp=__tc_meas(&B_TC_DESC, B_TC_TABLE, adf,tmk);
	 break;
	case TYPE_N:adt=(adi-0x800000)*4;
		 adf=linear_calib(adt, LinearCoeff.JKT_[ch].Ks,LinearCoeff.JKT_[ch].Bt);
	 ctemp=__tc_meas(&N_TC_DESC, N_TC_TABLE, adf,tmk);
	 break;	 
	 }		  }
//cont++;	 
//if(cont>1) 
{//  	cont=0;
//	if(ch==1)	Test.ch=5;
//	else

	Test.ch++;
	if(Test.ch>7)
		Test.ch=0;
//if(Test.ch==1)Test.ch=4;	if(ch!=4)	
	 if(tempOVER==1)
	 {CurrentTemp[ch]=0x7fff;}	 //
	 else
	 CurrentTemp[ch]=ctemp/100;
	 }
/*	 if(ctemp>(CurrentTemp[ch]+50)||CurrentTemp[ch]>(ctemp+50)){c_tim[ch]++; CurrentTemp[ch]++;}
	 else	{CurrentTemp[ch]=ctemp;c_tim[ch]=0;}
	 if(c_tim[ch]>1){c_tim[ch]=0;CurrentTemp[ch]=ctemp;}
*/
/*	  	 if(typ<PT100){
			  if(SYSPAR.chnum==8){
				   if(CurrentTemp[ch]>-2000){
					if(ch==0){		if(CurrentTemp[7]==-2000) CurrentTemp[0]+=15;			}
				   	else{		 if(CurrentTemp[ch-1]==-2000) CurrentTemp[ch]+=13;		   	 }
			  		}
					}
			  if(SYSPAR.chnum==16){
			  if(CurrentTemp[ch]>-2000){
					if(ch==0){		if(CurrentTemp[15]==-2000) CurrentTemp[0]+=15;			}
				   	else{		 if(CurrentTemp[ch-1]==-2000) CurrentTemp[ch]+=13;		   	 }
			  		}	
	 		}
	 } */
	 if(tempOVER==0)
	 {if(SYSPAR.Dia==4)	 {adf=TempC2K(CurrentTemp[ch]);CurrentTemp[ch]=adf;}//F

	 if(SYSPAR.Dia==1)	 {adf=2731+CurrentTemp[ch];CurrentTemp[ch]=adf;}//k
	 }

//t820;
		if(typ<PT100)	Test.tmp820++;
		else			Test.tmp820=0;

	   if(Test.ad_bat<18){Test.tmp820=1;}// Tenir=10;
		//   Test.ad_bat++;
		 //  if(Test.ad_bat>18)Test.ad_bat=20;
	if(Test.tmp820%15==1)//||Test.tmp820<5
		  {
		  tmp0();
/*	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);					//开启中断
	USART_ClearFlag(UART5, USART_FLAG_TC);
	USART_Cmd(UART5,ENABLE);
*/		  if(Test.tmt) Test.tmt=0;
		  else		Test.tmt=1;
//if(Test.tmt>6) Test.tmt=0;
		  if((Tenir<enir_temp.adx+30)&&enir_temp.adx<(Tenir+30))
		  {tmp_emp[Test.tmt]=enir_temp.adx;enir_t=0;}
			else		enir_t++;
			if(enir_t>2){ tmp_emp[Test.tmt]=enir_temp.adx;enir_t=0; }

		  Tenir=tmp_emp[0]/2+tmp_emp[1]/2;

		  if(Tenir>800)Tenir=800;		  
		  else if(Tenir<-300)Tenir=-300;
//		  CurrentTemp[4]=Tenir;
	}
}


