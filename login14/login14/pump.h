#ifndef _MEMDEV_H_
#define _MEMDEV_H_

#include <linux/ioctl.h>

/* 定义幻数 */
#define MEMDEV_IOC_MAGIC  'u'

/* 定义命令 */
#define MEMDEV1_ON   _IO(MEMDEV_IOC_MAGIC, 1)
#define MEMDEV1_OFF _IO(MEMDEV_IOC_MAGIC, 2)
#define MEMDEV2_ON   _IO(MEMDEV_IOC_MAGIC, 3)
#define MEMDEV2_OFF   _IO(MEMDEV_IOC_MAGIC, 4)
#define MEMDEV3_ON _IO(MEMDEV_IOC_MAGIC, 5)
#define MEMDEV3_OFF _IO(MEMDEV_IOC_MAGIC, 6)
#define MEMDEV4_ON _IO(MEMDEV_IOC_MAGIC, 7)
#define MEMDEV4_OFF _IO(MEMDEV_IOC_MAGIC, 8)

#define MEMDEV_IOC_MAXNR 8

#endif /* _MEMDEV_H_ */
