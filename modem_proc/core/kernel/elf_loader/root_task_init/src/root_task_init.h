/*=============================================================================
  Copyright (c) 2008 Qualcomm Technologies Incorporated.
           All Rights Reserved.
 * root_task_init.h
=============================================================================*/
#ifndef __ROOT_TASK_INIT_H__
#define __ROOT_TASK_INIT_H__

#include "msmhwiobase.h"
#include "HALhwio.h"


#define	RMB09_COMMAND_NOT_SET											   0x00

//Timer values
#define RMB09_POLL_EXPIRY_TIME											   2000000 //in us, max wait time before ramfs_detect_task is terminated
#define RMB09_POLL_PERIOD												   500 //in us, time period at which RMB09 register is polled
#define RAMFS_DETECT_TASK_SLEEP_PERIOD									   500 //in us, time period ramfs_detect_task is suspended

#define MPSS_RMB_REG_BASE                                                  (MSS_TOP_BASE      + 0x00180000)
#define MPSS_RMB_REG_BASE_PHYS                                             (MSS_TOP_BASE_PHYS + 0x00180000)

#define HWIO_MSS_RELAY_MSG_BUFFER_09_ADDR                                  (MPSS_RMB_REG_BASE      + 0x00000024)
#define HWIO_MSS_RELAY_MSG_BUFFER_09_PHYS                                  (MPSS_RMB_REG_BASE_PHYS + 0x00000024)
#define HWIO_MSS_RELAY_MSG_BUFFER_09_RMSK                                  0xffffffff
#define HWIO_MSS_RELAY_MSG_BUFFER_09_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_BUFFER_09_ADDR, HWIO_MSS_RELAY_MSG_BUFFER_09_RMSK)
#define HWIO_MSS_RELAY_MSG_BUFFER_09_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_BUFFER_09_ADDR, m)
#define HWIO_MSS_RELAY_MSG_BUFFER_09_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_BUFFER_09_ADDR,v)
#define HWIO_MSS_RELAY_MSG_BUFFER_09_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_BUFFER_09_ADDR,m,v,HWIO_MSS_RELAY_MSG_BUFFER_09_IN)
#define HWIO_MSS_RELAY_MSG_BUFFER_09_MSS_RELAY_MSG_BUFFER_09_BMSK          0xffffffff
#define HWIO_MSS_RELAY_MSG_BUFFER_09_MSS_RELAY_MSG_BUFFER_09_SHFT                 0x0


#endif
