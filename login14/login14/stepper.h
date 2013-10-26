/****************
 *名称：stepper.h
 *功能：步进电机驱动头文件，应用程序和驱动程序都应包含此文件。
 *作者：feiniao
 *时间：2011.12.15新建文件
 ********************/

#ifndef _STEPPER_H_
#define _STEPPER_H_

//定义幻数
#define STEPPER_IOC_MAGIC 's'

//定义命令
//#define STEPPER_IOCSTOP _IOW(STEPPER_IOC_MAGIC,0,int)    //停止
#define STEPPER_IOCFWD _IOW(STEPPER_IOC_MAGIC,1,int) //前进
#define STEPPER_IOCBACK _IOW(STEPPER_IOC_MAGIC,2,int)    //后退
#define STEPPER_IOCSTA _IOW(STEPPER_IOC_MAGIC,3,int)     //回到起始位置

#define STEPPER_IOC_MAXNR 3

#endif //_STEPPER_H_
