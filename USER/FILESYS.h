/* name ������ָ���ļ���, ���԰�����Ŀ¼��, ��������·���ָ���, �ܳ��Ȳ�����1+8+1+3+1�ֽ� */
/* PathName ������ָȫ·���Ķ��ļ���, ������Ŀ¼�����༶��Ŀ¼��·���ָ������ļ���/Ŀ¼�� */
/* LongName ������ָ���ļ���, ��UNICODEС��˳�����, ������0�ֽڽ���, ʹ�ó��ļ����ӳ�������ȶ���ȫ�ֻ�����GlobalBuf, ���Ȳ�С��64�ֽ�, �����������ӳ����� */

/* ���� NO_DEFAULT_CH376_INT ���ڽ�ֹĬ�ϵ�Wait376Interrupt�ӳ���,��ֹ��,Ӧ�ó���������ж���һ��ͬ���ӳ��� */
/* ���� DEF_INT_TIMEOUT ��������Ĭ�ϵ�Wait376Interrupt�ӳ����еĵȴ��жϵĳ�ʱʱ��/ѭ������ֵ, 0�򲻼�鳬ʱ��һֱ�ȴ� */
/* ���� EN_DIR_CREATE �����ṩ�½��༶��Ŀ¼���ӳ���,Ĭ���ǲ��ṩ */
/* ���� EN_DISK_QUERY �����ṩ����������ѯ��ʣ��ռ��ѯ���ӳ���,Ĭ���ṩ */
/* ���� EN_SECTOR_ACCESS �����ṩ������Ϊ��λ��д�ļ����ӳ���,Ĭ���ǲ��ṩ */
/* ���� EN_LONG_NAME �����ṩ֧�ֳ��ļ������ӳ���,Ĭ���ǲ��ṩ */

#ifndef	__CH376_FS_H__
#define __CH376_FS_H__

#include "ch376.h"

#define	STRUCT_OFFSET( s, m )	( (u8)( & ((s *)0) -> m ) )							/* �����ȡ�ṹ��Ա���ƫ�Ƶ�ַ�ĺ� */
#ifdef	EN_LONG_NAME
#ifndef	LONG_NAME_BUF_LEN
#define	LONG_NAME_BUF_LEN	( LONG_NAME_PER_DIR * 20 )									/* ���ж���ĳ��ļ�������������,��СֵΪLONG_NAME_PER_DIR*1 */
#endif
#endif

u8	CH376ReadBlock( u8* buf );  													/* �ӵ�ǰ�����˵�Ľ��ջ�������ȡ���ݿ�,���س��� */

u8	CH376WriteReqBlock( u8* buf );  												/* ���ڲ�ָ��������д����������ݿ�,���س��� */

void	CH376WriteHostBlock( u8* buf, u8 len );  									/* ��USB�����˵�ķ��ͻ�����д�����ݿ� */

void	CH376WriteOfsBlock( u8* buf, u8 ofs, u8 len );  						/* ���ڲ�������ָ��ƫ�Ƶ�ַд�����ݿ� */

void	CH376SetFileName( u8* name );  												/* ���ý�Ҫ�������ļ����ļ��� */

u32	CH376Read32bitDat( void );  													/* ��CH376оƬ��ȡ32λ�����ݲ��������� */

u8	CH376ReadVar8( u8 var );  													/* ��CH376оƬ�ڲ���8λ���� */

void	CH376WriteVar8( u8 var, u8 dat );  										/* дCH376оƬ�ڲ���8λ���� */

u32	CH376ReadVar32( u8 var );  													/* ��CH376оƬ�ڲ���32λ���� */

void	CH376WriteVar32( u8 var, u32 dat );  										/* дCH376оƬ�ڲ���32λ���� */

void	CH376EndDirInfo( void );  														/* �ڵ���CH376DirInfoRead��ȡFAT_DIR_INFO�ṹ֮��Ӧ��֪ͨCH376���� */

u32	CH376GetFileSize( void );  														/* ��ȡ��ǰ�ļ����� */

u8	CH376GetDiskStatus( void );                                                     /* ��ȡ���̺��ļ�ϵͳ�Ĺ���״̬ */

u8	CH376GetIntStatus( void );                                                      /* ��ȡ�ж�״̬��ȡ���ж����� */

#ifndef	NO_DEFAULT_CH376_INT
u8	Wait376Interrupt( void );                                                       /* �ȴ�CH376�ж�(INT#�͵�ƽ)�������ж�״̬��, ��ʱ�򷵻�ERR_USB_UNKNOWN */
#endif

u8	CH376SendCmdWaitInt( u8 mCmd );                                              /* �����������,�ȴ��ж� */

u8	CH376SendCmdDatWaitInt( u8 mCmd, u8 mDat );                               /* �����������һ�ֽ����ݺ�,�ȴ��ж� */

u8	CH376DiskReqSense( void );                                                      /* ���USB�洢������ */

u8	CH376DiskConnect( void );                                                       /* ���U���Ƿ�����,��֧��SD�� */

u8	CH376DiskMount( void );                                                         /* ��ʼ�����̲����Դ����Ƿ���� */

u8	CH376FileOpen( u8* name );                                                   /* �ڸ�Ŀ¼���ߵ�ǰĿ¼�´��ļ�����Ŀ¼(�ļ���) */

u8	CH376FileCreate( u8* name );                                                 /* �ڸ�Ŀ¼���ߵ�ǰĿ¼���½��ļ�,����ļ��Ѿ�������ô��ɾ�� */

u8	CH376DirCreate( u8* name );                                                  /* �ڸ�Ŀ¼���½�Ŀ¼(�ļ���)����,���Ŀ¼�Ѿ�������ôֱ�Ӵ� */

u8	CH376SeparatePath( u8* path );                                               /* ��·���з�������һ���ļ�������Ŀ¼(�ļ���)��,�������һ���ļ�������Ŀ¼�����ֽ�ƫ�� */

u8	CH376FileOpenDir( u8* PathName, u8 StopName );                            /* �򿪶༶Ŀ¼�µ��ļ�����Ŀ¼���ϼ�Ŀ¼,֧�ֶ༶Ŀ¼·��,֧��·���ָ���,·�����Ȳ�����255���ַ� */
/* StopName ָ�����һ���ļ�������Ŀ¼�� */

u8	CH376FileOpenPath( u8* PathName );                                           /* �򿪶༶Ŀ¼�µ��ļ�����Ŀ¼(�ļ���),֧�ֶ༶Ŀ¼·��,֧��·���ָ���,·�����Ȳ�����255���ַ� */

u8	CH376FileCreatePath( u8* PathName );                                         /* �½��༶Ŀ¼�µ��ļ�,֧�ֶ༶Ŀ¼·��,֧��·���ָ���,·�����Ȳ�����255���ַ� */

#ifdef	EN_DIR_CREATE
u8	CH376DirCreatePath( u8* PathName );                                          /* �½��༶Ŀ¼�µ�Ŀ¼(�ļ���)����,֧�ֶ༶Ŀ¼·��,֧��·���ָ���,·�����Ȳ�����255���ַ� */
#endif

u8	CH376FileErase( u8* PathName );                                              /* ɾ���ļ�,����Ѿ�����ֱ��ɾ��,��������ļ����ȴ���ɾ��,֧�ֶ༶Ŀ¼·�� */

u8	CH376FileClose( u8 UpdateSz );                                               /* �رյ�ǰ�Ѿ��򿪵��ļ�����Ŀ¼(�ļ���) */

u8	CH376DirInfoRead( void );                                                       /* ��ȡ��ǰ�ļ���Ŀ¼��Ϣ */

u8	CH376DirInfoSave( void );                                                       /* �����ļ���Ŀ¼��Ϣ */

u8	CH376ByteLocate( u32 offset );                                               /* ���ֽ�Ϊ��λ�ƶ���ǰ�ļ�ָ�� */

u8	CH376ByteRead( u8* buf, u16 ReqCount, u16* RealCount );                /* ���ֽ�Ϊ��λ�ӵ�ǰλ�ö�ȡ���ݿ� */

u8	CH376ByteWrite( u8* buf, u16 ReqCount, u16* RealCount );               /* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */

u8	CH376DiskCapacity( u32* DiskCap );                                           /* ��ѯ������������,������ */

u8   CH376DiskQuery( u32* DiskFre );                                              /* ��ѯ����ʣ��ռ���Ϣ,������ */

u8	CH376SecLocate( u32 offset );                                                /* ������Ϊ��λ�ƶ���ǰ�ļ�ָ�� */

#ifdef	EN_SECTOR_ACCESS

u8	CH376DiskReadSec( u8* buf, u32 iLbaStart, u8 iSectorCount );           /* ��U�̶�ȡ������������ݿ鵽������,��֧��SD�� */

u8	CH376DiskWriteSec( u8* buf, u32 iLbaStart, u8 iSectorCount );          /* ���������еĶ�����������ݿ�д��U��,��֧��SD�� */

u8	CH376SecRead( u8* buf, u8 ReqCount, u8* RealCount );                   /* ������Ϊ��λ�ӵ�ǰλ�ö�ȡ���ݿ�,��֧��SD�� */

u8	CH376SecWrite( u8* buf, u8 ReqCount, u8* RealCount );                  /* ������Ϊ��λ�ڵ�ǰλ��д�����ݿ�,��֧��SD�� */

#endif

#ifdef	EN_LONG_NAME

u8	CH376LongNameWrite( u8* buf, u16 ReqCount );                              /* ���ļ���ר�õ��ֽ�д�ӳ��� */

u8	CH376CheckNameSum( u8* DirName );                                            /* ���㳤�ļ����Ķ��ļ��������,����Ϊ��С����ָ����Ĺ̶�11�ֽڸ�ʽ */

u8	CH376LocateInUpDir( u8* PathName );                                          /* ���ϼ�Ŀ¼(�ļ���)���ƶ��ļ�ָ�뵽��ǰ�ļ�Ŀ¼��Ϣ���ڵ����� */
/* ����,˳�㽫��ǰ�ļ�Ŀ¼��Ϣ���ڵ�������ǰһ��������LBA��ַд��CH376�ڲ�VAR_FAT_DIR_LBA����(Ϊ�˷����ռ����ļ���ʱ��ǰ����,����Ҫ���ƶ�һ��) */
/* ʹ����ȫ�ֻ�����GlobalBuf��ǰ12���ֽ� */

u8	CH376GetLongName( u8* PathName, u8* LongName );                           /* �ɶ��ļ�������Ŀ¼(�ļ���)�������Ӧ�ĳ��ļ��� */
/* ��Ҫ������ļ���������·��PathName,��Ҫ�ṩ���������ճ��ļ���LongName(��UNICODEС�˱���,��˫0����) */
/* ʹ����ȫ�ֻ�����GlobalBuf��ǰ34���ֽ�,sizeof(GlobalBuf)>=sizeof(FAT_DIR_INFO)+2 */

u8	CH376CreateLongName( u8* PathName, u8* LongName );                        /* �½����г��ļ������ļ�,�ر��ļ��󷵻�,LongName����·��������RAM�� */
/* ��Ҫ������ļ���������·��PathName(�����Ȳο�FAT�淶�ɳ��ļ������в���),��Ҫ������UNICODEС�˱������˫0�����ĳ��ļ���LongName */
/* ʹ����ȫ�ֻ�����GlobalBuf��ǰ64���ֽ�,sizeof(GlobalBuf)>=sizeof(FAT_DIR_INFO)*2 */

#endif

#endif
