/****************
 *���ƣ�lopper.h
 *���ܣ�ն��������ͷ�ļ���Ӧ�ó������������Ӧ�������ļ���
 *���ߣ�LINX
 *ʱ�䣺2012.12.14�½��ļ�
 ********************/

#ifndef _LOPPER_H_
#define _LOPPER_H_

//�������
#define LOPPER_IOC_MAGIC 't'

#define LOPPER_ON _IOW(LOPPER_IOC_MAGIC,1,int)     //����ߵ�ƽ���̵�����
#define LOPPER_OFF _IOW(LOPPER_IOC_MAGIC,2,int)     //����͵�ƽ���̵�����

#define LOPPER_IOC_MAXNR 2

#endif //_LOPPER_H_
