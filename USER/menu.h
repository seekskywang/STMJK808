#ifndef __MENU_H
#define __MENU_H

#define KEY_F1		0X01
#define KEY_F2		0X14
#define KEY_F3		0X03
#define KEY_F4		0X04
#define KEY_F5		0X05
#define KEY_MEAS	0X08
#define KEY_SETP	0X0C
#define KEY_SYST	0X0D
#define KEY_BRT 	0X0F
#define KEY_UP 		0X06
#define KEY_DOWN	0X0E
#define KEY_LEFT	0X09
#define KEY_RIGT	0X0B
#define KEY_ACC		0X07
#define KEY_OFF		0X02
//#define   SCREEN_HIGH	272
//#define   SCREEN_WIDTH  480

#define   SCREEN_HIGH	240
#define   SCREEN_WIDTH  320

#define    DOTCOLOR		 RGB888TORGB565(0x82e6e6)	 //��������ɫ	   bee6fb
#define    FILLBLOCK     RGB888TORGB565(0x0071ce)	  //�����ɫ
#define    WIRE			 RGB888TORGB565(0x0033a1)	  //�ߵ���ɫ
#define    FONTCOLOR     RGB888TORGB565(0xf0f032)	  //������ɫ��ɫf0f032	    0xffe0
#define    BABYBLUE		 RGB888TORGB565(0x00dcdc)	   //����ɫ	  4bc1e1
#define    BABYYELLOW    RGB888TORGB565(0xf0f032)   //����ɫ

#define    BUTTONCOLOR     RGB888TORGB565(0x000000)	  //��
#define    TOPCOLOR     	 RGB888TORGB565(0x000EE00)	  //��
#define    DATACOLOR     	 RGB888TORGB565(0xFFFFFF)	  //��
#define		 TCALCOLOR			 RGB888TORGB565(0xFF0000)//��


#define   GOLDYELLOW     RGB888TORGB565(0xeaa01e)

#define    RGB888_RED    0x00ff0000
#define    RGB888_GREEN  0x0000ff00
#define    RGB888_BLUE   0x000000ff	 

#define    XLENGHT      380    //դ��X�᳤��	   ����ͼ���� LENGHT
#define    YLENGHT		200	//դ��Y�᳤��	   	   ����ͼ����
#define	   SPOINTX		99		 //	 ���X����     ����ͼ����  63
#define    SPOINTY		4		 //	 ���Y����	   ����ͼ����
#define    ENDPOX		479		 //	 ������X����   ����ͼ����  463
#define	   ENDPOY		204		 //	 ������Y����   ����ͼ����
#define    CUR_X0		114		 //���߲�������x����
#define    CUR_Y0		34	
#define    LEFT         112		 //�����߲����е�ͨ����������112����
//#define    UP_SYS		13		 //�豸�������������ƶ�40����
//#define    CLOCTION		114		//���߲��������˵���ߵ�Xλ��
//#define    CRLOCTI		391		//���߲����ұߵ�ֵ��X��λ��
#define    SLOCT   		104		//�豸�������X���λ��
#define    SRLOCT   	360		//�豸�����ұ�X���λ��
#define    SENSORPY      46      //��ʾ����������Ӣ���ַ���Y��λ��

#define    HSTARTX      53 	   //��ʷ����X���������
#define    HSTARTY		 8	   //��ʷ����Y���������
#define    HENDX		453	   //��ʷ����X�Ľ�������
#define    HENDY		208	   //��ʷ����Y�Ľ�������

#define    HXLENGH		400		//��ʷ����X�ĳ���
#define    HYLENGH		200		//��ʷ����Y�ĳ���

#define    GRAY         RGB888TORGB565(0x8a8d8e)	   //��ɫ
#define    DATE_NUM   	0
#define    SYSM_YPOSTI 	24		//ϵͳ��ϢY��λ��

#define    SYSKEY		123654

#define   SETCOL1      			80
#define   SETCOL2      			240
#define   SETROW1      			30
#define   SETROWOFFSET      22

#define   SYSROW1     			22
#define   SYSROWOFFSET      18
//===========
#define   UP_LINE_OFFSET           210
#define   DOWN_LINE_OFFSET         212
#define   UP_LUM_OFFSET            31

#define   F1XCOR1      		2
#define   F1XCOR2      		65
#define   F2XCOR1      		2+70
#define   F2XCOR2      		65+70
#define   F3XCOR1      		2+70*2
#define   F3XCOR2      		65+70*2
#define   F4XCOR1      		2+70*3
#define   F4XCOR2      		65+70*3
#define   F5XCOR1      		2+70*4
#define   F5XCOR2      		318
#define   FYCOR1      		UP_LINE_OFFSET+2
#define   FYCOR2      		UP_LINE_OFFSET+UP_LUM_OFFSET-1

#define   NUM1XCOR1      		7+3
#define   NUM1XCOR2      		7+30
#define   NUM2XCOR1      		7+3+34
#define   NUM2XCOR2      		7+30+34
#define   NUM3XCOR1      		7+3+34*2
#define   NUM3XCOR2      		7+30+34*2
#define   NUM4XCOR1      		7+3+34*3
#define   NUM4XCOR2      		7+30+34*3
#define   NUM5XCOR1      		7+3+34*4
#define   NUM5XCOR2      		7+30+34*4
#define   NUM6XCOR1      		7+3+34*5
#define   NUM6XCOR2      		7+30+34*5
#define   NUM7XCOR1      		7+3+34*6
#define   NUM7XCOR2      		7+30+34*6
#define   NUM8XCOR1      		7+3+34*7
#define   NUM8XCOR2      		7+30+34*7
#define   NUM9XCOR1      		7+3+34*8
#define   NUM9XCOR2      		7+30+34*8
#define   DELXCOR1      		255+3
#define   DELXCOR2      		255+59
#define   CONXCOR1      		5+3
#define   CONXCOR2      		5+61
#define   CANCELXCOR1       255+3
#define   CANCELXCOR2      	255+61
#define   MINUSXCOR1      	87+3
#define   MINUSXCOR2      	87+30
#define   DOTXCOR1       		87+3+34
#define   DOTXCOR2      		87+30+34
#define   ZEROXCOR1      		87+3+34*2
#define   ZEROXCOR2      		87+30+34*2
#define   EXITXCOR1      		225+2
#define   EXITXCOR2      		225+62-2
#define   CHODDXCOR1      	8+40
#define   CHODDXCOR2      	8+40+72
#define   CHEVENXCOR1      	168+40
#define   CHEVENXCOR2      	168+40+72
#define   SETODDXCOR1      	SETCOL1
#define   SETODDXCOR2      	SETCOL1+48
#define   SETEVENXCOR1      SETCOL2
#define   SETEVENXCOR2      SETCOL2+48

#define   NUMYCOR1      		170+2
#define   NUMYCOR2      		170+30
#define   DELYCOR1      		140+2
#define   DELYCOR2      		140+25
#define   CONYCOR1      		215-5+2
#define   CONYCOR2      		215-5+25
#define   ZEROYCOR1      		170+34+2
#define   ZEROYCOR2      		170+34+30
#define   EXITYCOR1      		215-5+2
#define   EXITYCOR2      		215-5+25
#define   CHROW1YCOR1      	42
#define   CHROW1YCOR2      	42+24
#define   CHROW2YCOR1      	42+38
#define   CHROW2YCOR2      	42+24+38
#define   CHROW3YCOR1      	42+38*2
#define   CHROW3YCOR2      	42+24+38*2
#define   CHROW4YCOR1      	42+38*3
#define   CHROW4YCOR2      	42+24+38*3
#define   SETROW1YCOR1      SETROW1
#define   SETROW1YCOR2      SETROW1+16
#define   SETROW2YCOR1      SETROW1+SETROWOFFSET
#define   SETROW2YCOR2      SETROW1+SETROWOFFSET+16
#define   SETROW3YCOR1      SETROW1+SETROWOFFSET*2
#define   SETROW3YCOR2      SETROW1+SETROWOFFSET*2+16
#define   SETROW4YCOR1      SETROW1+SETROWOFFSET*3
#define   SETROW4YCOR2      SETROW1+SETROWOFFSET*3+16
#define   SYSROW1YCOR1      SYSROW1+SYSROWOFFSET*6
#define   SYSROW1YCOR2      SYSROW1+SYSROWOFFSET*6+16
#define   SYSROW2YCOR1      SYSROW1+SYSROWOFFSET*7
#define   SYSROW2YCOR2      SYSROW1+SYSROWOFFSET*7+16
#define   SYSROW3YCOR1      SYSROW1+SYSROWOFFSET*8
#define   SYSROW3YCOR2      SYSROW1+SYSROWOFFSET*8+16
#define   SYSROW4YCOR1      SYSROW1+SYSROWOFFSET*9
#define   SYSROW4YCOR2      SYSROW1+SYSROWOFFSET*9+16

extern u8 itempos;//���λ��
extern u8 pageflag;//ҳ���־
extern u8 displayflag;//ҳ��ˢ�±�־
extern u8 moveflag;//����ƶ���־
extern u8 buttonpage;
extern u8 calflag;
	
void DISP_TEST(void);
void KEY_HANDLE(u8 key);
void DISP_PAGE_ITEM(u8 page,u8 pos);
void DISP_PAGE(void);
void DISP_TIME(void);
void DISP_TEMP(void);
void DISP_BAT(u8 cap);
void DISP_CHARGE(void);
void DISP_USB(void);
void INPUT_CANCEL(void);
void DISP_CAL_PROCESS(u8 step);
void DISP_ENVIROMENT_TEMP(void);//��ʾ�����¶�
void DISP_HID(void);
void DrawLogo(u16 x,u16 y);
void DISP_POWERON(void);

#endif
