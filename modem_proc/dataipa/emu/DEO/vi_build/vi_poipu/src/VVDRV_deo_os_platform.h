#ifndef __VVDRV_DEO_OS_PLATFORM_H__
#define __VVDRV_DEO_OS_PLATFORM_H__
/**---------------------------------------------------------------------------
 * @file  VVDRV_deo_os_platform.h
 *
 * @brief This file is a generic wrapper which for each proc will include the 
 *        correct swi file with the definitions and register names of the 
 *        requested timers as defined in the msmhwioreg.h definitions.
 *
 * @note  This is generic structure, adapted for future changes. In case of
 *        new requirments supporting new or not suitable procs, just add a
 *        new platform file and add a compilation condition.
 *
 * @note  For different timer options please check the options defined in 
 *        the included VVDRV_deo_os_platform_<proc>.h file
 *
 * @note  This file should be included by deo_time_os (VI_BUILD) file only. 
 *
 * Copyright (c) 2014-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *----------------------------------------------------------------------------*/

#if (defined(COMPILING_SSC_SDC_PROC) || defined(COMPILING_SSC_PROC))
#include "VVDRV_deo_os_platform_sdc.h"
#else /* !COMPILING_SSC_SDC_PROC */
#include "VVDRV_deo_os_platform_sys.h"
#endif /* COMPILING_SSC_SDC_PROC */

#endif /* __VVDRV_DEO_OS_PLATFORM_H__ */
