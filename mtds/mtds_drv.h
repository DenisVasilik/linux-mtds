#ifndef MTDS_DRV_H
#define MTDS_DRV_H

#include <linux/ioctl.h>

struct mtds_data
{
    bool slave;
    bool isReady;
    uint8_t bSnd;
    uint8_t bRcv;
};

#define IOC_INIT _IOWR('Z', 1, struct mtds_data *)
#define IOC_ENABLE_SLAVE _IOWR('Z', 2, struct mtds_data *)
#define IOC_READY _IOWR('Z', 3, struct mtds_data *)
#define IOC_WRITE _IOWR('Z', 4, struct mtds_data *)

#endif
