/****************
 *���ƣ�stepper.h
 *���ܣ������������ͷ�ļ���Ӧ�ó������������Ӧ�������ļ���
 *���ߣ�feiniao
 *ʱ�䣺2011.12.15�½��ļ�
 ********************/

#ifndef _STEPPER_H_
#define _STEPPER_H_

//�������
#define STEPPER_IOC_MAGIC 's'

//��������
//#define STEPPER_IOCSTOP _IOW(STEPPER_IOC_MAGIC,0,int)    //ֹͣ
#define STEPPER_IOCFWD _IOW(STEPPER_IOC_MAGIC,1,int) //ǰ��
#define STEPPER_IOCBACK _IOW(STEPPER_IOC_MAGIC,2,int)    //����
#define STEPPER_IOCSTA _IOW(STEPPER_IOC_MAGIC,3,int)     //�ص���ʼλ��

#define STEPPER_IOC_MAXNR 3

#endif //_STEPPER_H_
