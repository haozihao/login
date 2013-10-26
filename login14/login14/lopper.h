/****************
 *名称：lopper.h
 *功能：斩波器驱动头文件，应用程序和驱动程序都应包含此文件。
 *作者：LINX
 *时间：2012.12.14新建文件
 ********************/

#ifndef _LOPPER_H_
#define _LOPPER_H_

//定义幻数
#define LOPPER_IOC_MAGIC 't'

#define LOPPER_ON _IOW(LOPPER_IOC_MAGIC,1,int)     //输出高电平，继电器开
#define LOPPER_OFF _IOW(LOPPER_IOC_MAGIC,2,int)     //输出低电平，继电器关

#define LOPPER_IOC_MAXNR 2

#endif //_LOPPER_H_
