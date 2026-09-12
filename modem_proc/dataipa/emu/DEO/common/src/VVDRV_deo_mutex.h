#ifndef __VVDRV_DEO_MUTEX_H__
#define __VVDRV_DEO_MUTEX_H__
/**---------------------------------------------------------------------------
 * @file  VVDRV_deo_mutex.h
 *
 * @brief Declares the DEO mutex interface
 *
 * Copyright (c) 2010-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Include Files
 *---------------------------------------------------------------------------*/
#include "VVDRV_deo_mutex_os_platform.h"


/*-----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *---------------------------------------------------------------------------*/
/** 
 * @brief Users must declare a global mutex using this macro, this macro
 *        will add additional attributes related to specific targets.
 * @note  Users should call #VVDRV_deo_mutex_create() before using the mutex.
 * @example DEO_MUTEX_DECLARE(static s_my_mutex)
 *          VVDRV_deo_mutex_create(&s_my_mutex, NULL)
 *          VVDRV_deo_mutex_lock(&s_my_mutex)
 *          VVDRV_deo_mutex_unlock(&s_my_mutex)
 *          VVDRV_deo_mutex_destroy(&s_my_mutex)
 */
#define DEO_MUTEX_DECLARE(mutex) DEO_MUTEX_REGION deo_mutex_t mutex

/** @brief create the mutex as critical section */
#define DEO_MUTEX_ATTR_CRITICAL_SECTION 0x0


/*-----------------------------------------------------------------------------
 * Type Declarations
 *---------------------------------------------------------------------------*/
/**
 * @brief this holds the mutex attribute settings
 */
typedef uint32 deo_mutex_attr_t;

/**
 * @brief mutex variable, it must be declared in global section using
 *        #DEO_MUTEX_DECLARE
 */
typedef struct deo_mutex 
{
   /** @brief variable which is used for the lock */
   deo_mutex_lock_t lock;
   
   /** @brief default is #DEO_MUTEX_ATTR_CRITICAL_SECTION */
   deo_mutex_attr_t attr;  
} deo_mutex_t;


/*------------------------------------------------------------------------------
 * Externs
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 * Global Data Definitions
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 * Static Variable Definitions
 *----------------------------------------------------------------------------*/

 
/*------------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 * Global Function Definitions
 *----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DEO mutex initiate
 * @warn function not thread safe and should be called from non-concurrent code
 * @return #DEO_SUCCESS on success, other on failure 
 */
int32 VVDRV_deo_mutex_init(void);

/**
 * @brief runtime creation of the mutex
 * @param [out] mutex is the mutex which will be created
 * @param [in] attr can specifiy param, when not specified, aka: used as NULL,
 *             default #DEO_MUTEX_ATTR_CRITICAL_SECTION will be used
 * @warn function not thread safe and should be called from non-concurrent code
 * @return #DEO_SUCCESS on success, other on failure 
 */
int32 VVDRV_deo_mutex_create(deo_mutex_t *mutex, deo_mutex_attr_t attr);

/**
 * @brief DEO mutex lock
 * @param [in] mutex is the mutex created by #VVDRV_deo_mutex_create()
 * @note this code may be blocking depends on the attributes
 * @return #DEO_SUCCESS on success, other on failure 
 */
int32 VVDRV_deo_mutex_lock(deo_mutex_t *mutex);

/**
 * @brief DEO mutex unlock. 
 * @return #DEO_SUCCESS on success, other on failure 
 * @param [in] mutex is the mutex created by #VVDRV_deo_mutex_create()
 * @note mutex should be the mutex which is locked by #VVDRV_deo_mutex_lock()
 * @return #DEO_SUCCESS on success, other on failure
 */
int32 VVDRV_deo_mutex_unlock(deo_mutex_t *mutex);

/**
 * @brief DEO mutex destory
 * @param [in] mutex is the mutex created by #VVDRV_deo_mutex_create()
 * @warn function not thread safe and should be called from non-concurrent code
 * @return #DEO_SUCCESS on success, other on failure 
 */
int32 VVDRV_deo_mutex_destroy(deo_mutex_t *mutex);

/**
 * @brief DEO mutex unitinialization
 * @warn function not thread safe and should be called from non-concurrent code
 */
void VVDRV_deo_mutex_uninit(void);

#ifdef __cplusplus
}
#endif

#endif /* __VVDRV_DEO_MUTEX_H__ */
