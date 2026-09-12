#ifndef __VVDRV_DEO_TIME_OS_H__
#define __VVDRV_DEO_TIME_OS_H__
/**---------------------------------------------------------------------------
 * @file    VVDRV_deo_time_os.h
 *
 * @brief   DEO time service, OS-layer APIs. 
 *          These API could change without any notice and should not be called
 *          by user code
 *
 * Copyright (c) 2014-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************/
/*                              INCLUDES                                      */
/******************************************************************************/

#include "VVDRV_deo_common.h"               /* DEO_SUCCESS, uint32 and uint64 */

/******************************************************************************/
/*                              FUNCTIONS                                     */
/******************************************************************************/

/**
 * @brief   Initializes the timer service OS layer
 *
 * @return  #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_os_init(void);

/**
 * @brief   Un-initializes the timer service OS layer
 *
 * @return  #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_os_uninit(void);

/**
 * @brief       Get the current clock tick count of the timer-service. The
 *              count is monotonically increasing. Rollover (Wraparound) is no
 *              less than 100 years from first initialization of the timer
 *              service
 *
 * @param[out]  tick_count  current tick count
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_os_get_clock_count(uint64 *tick_count);

/**
 * @brief       Gets the clock frequency, which is also the number of clock
 *              ticks per second
 *
 * @param[out]  frequency      Clock frequency, in Hz (also ticks/second)
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_os_get_clock_frequency(uint32 *frequency);

/**
 * @brief       Converts ticks into milliseconds units
 *
 * @param[in]   ticks   ticks count
 * @param[out]  ms      milliseconds count
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_os_ticks_to_ms(uint64 ticks, uint64 *ms);

/**
 * @brief       Converts ticks into microseconds units
 *
 * @param[in]   ticks   ticks count
 * @param[out]  us      microseconds count
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_os_ticks_to_us(uint64 ticks, uint64 *us);

/**
 * @brief       Converts ticks into microseconds units
 *
 * @note        The ceiling value will be returned in case that the number of 
 *              microseconds could no be represented by the clock frequency.
 *
 * @param[in]    us      microseconds count
 * @param[out]   ticks   ticks count
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_os_us_to_ticks(uint64 us, uint64 *ticks);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DEO_TIME_OS_H__ */
