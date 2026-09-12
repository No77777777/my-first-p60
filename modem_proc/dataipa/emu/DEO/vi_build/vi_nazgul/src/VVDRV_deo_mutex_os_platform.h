#ifndef __VVDRV_DEO_MUTEX_OS_PLATFORM_H__
#define __VVDRV_DEO_MUTEX_OS_PLATFORM_H__
/**---------------------------------------------------------------------------
 * @file  VVDRV_deo_mutex_os_platform.h
 *
 * @brief Declares the DEO mutex interface specific for target
 *
 * Copyright (c) 2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Include Files
 *---------------------------------------------------------------------------*/
#include "comdef.h"

/*-----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *---------------------------------------------------------------------------*/
#if defined (COMPILING_APPS_PROC)

/** @brief enables mutex */
#define DEO_MUTEX_SUPPORTED

/**@brief inlcude file which implement the mutex */
#if !defined(DEO_MUTEX_H_INCLUDE_FILE)
   #define DEO_MUTEX_H_INCLUDE_FILE "apps_proc_common.h"
#endif /* DEO_MUTEX_H_INCLUDE_FILE */

/** @brief region which is used for the lock */
#define DEO_MUTEX_REGION __attribute__ ((section ("lock"), aligned(16)))

#else  /* !COMPILING_APPS_PROC */

/** @brief region which is used for the lock, empty for non-supported procs */
#define DEO_MUTEX_REGION

#endif /* COMPILING_APPS_PROC */


/*-----------------------------------------------------------------------------
 * Type Declarations
 *---------------------------------------------------------------------------*/
/** @brief the mutex lock type */
typedef uint32 deo_mutex_lock_t;


#endif /* __VVDRV_DEO_MUTEX_OS_PLATFORM_H__ */
