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
#include "comdef.h"     /* basic types */

 
/*-----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *---------------------------------------------------------------------------*/
/** mutex is not supported for this target */

/** @brief region which is used for the lock, empty for non-supported targets */
#define DEO_MUTEX_REGION


/*-----------------------------------------------------------------------------
 * Type Declarations
 *---------------------------------------------------------------------------*/
/** @brief the mutex lock type */
typedef uint32 deo_mutex_lock_t;


#endif /* __VVDRV_DEO_MUTEX_OS_PLATFORM_H__ */
