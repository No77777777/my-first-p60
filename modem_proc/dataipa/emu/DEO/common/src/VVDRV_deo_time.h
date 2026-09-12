#ifndef __VVDRV_DEO_TIME_H__
#define __VVDRV_DEO_TIME_H__
/**-----------------------------------------------------------------------------
 * @file  VVDRV_deo_time.h
 *
 * @brief Declares the interface to DEO time services
 *
 * @note  The user can select between 5 implementation options, with same APIs
 *        Each implementation option is defined by using one of the following
 *        definitions.
 *
 *  1. DEO_TIME_OS_CLK_NONE       - If defined time service will use an empty
 *                                  implementation and APIs will fail.
 *                                  If defined this will override all other 
 *                                  implementation options
 *
 *  2. DEO_TIME_OS_CLK_SLEEP_CLK  - Relevant to VI-BUILD version.
 *                                  If defined, time service will use the target
 *                                  sleep clock which usually provides a clock
 *                                  of 32KHz. If no VI-BUILD option is defined
 *                                  this will be set as default
 *
 *  3. DEO_TIME_OS_CLK_QTIMER     - Relevant to VI-BUILD version.
 *                                  If defined, time service will use the target
 *                                  QTimer which usually provide a clock of
 *                                  19.2MHz
 *
 *  4. DEO_TIME_OS_CLK_MULTIMEDIA - Relevant to Windows version.
 *                                  If defined, time service will use the
 *                                  Windows multimedia timer APIs which usually
 *                                  provides a 1KHz clock. If no Windows option
 *                                  is defined this will be set as default
 *
 *  5. DEO_TIME_OS_CLK_PERF_CNTR  - Relevant to Windows version.
 *                                  If defined, time service will use the
 *                                  Windows performance counters APIs which
 *                                  usually provides a clock of about 3MHz
 *
 * Copyright (c) 2011-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/

#include "VVDRV_deo_common.h"      /* DEO_SUCCESS, DEO_ERROR, uint32 and more */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*------------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Type Declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief   DEO time timer handle
 */
typedef uint64 deo_time_timer_t;

/*------------------------------------------------------------------------------
 * Function Definitions
 *----------------------------------------------------------------------------*/

/**
 * @brief       Initializes the DEO time service
 *
 * @note        The function is called by #VVDRV_deo_init()
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_init(void);

/**
 * @brief       Un-init the DEO time service
 *
 * @note        The function is called by #VVDRV_deo_uninit()
 */
void VVDRV_deo_time_uninit(void);

/**
 * @brief       Sleep at least a given amount of milliseconds. Function will
 *              return only after this time period
 *
 * @note        The sleep interval may be greater than specified due to the
 *              granularity of the clock and other system considerations
 *
 * @note        #VVDRV_deo_time_init() should be called prior to calling 
 *              this API
 *
 * @note        Maximal sleep interval is at least 10 years
 *
 * @param[in]   sleep_ms   Sleep time in milliseconds
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_sleep_ms(uint64 sleep_ms);

/**
 * @brief       Sleep at least a given amount of microseconds. Function will
 *              return only after this time period
 *
 * @note        The sleep interval may be greater than specified due to the
 *              granularity of the clock and other system considerations
 *
 * @note        #VVDRV_deo_time_init() should be called prior to calling 
 *              this API
 *
 * @note        Maximal sleep interval is at least 10 years
 *
 * @param[in]   sleep_us   Sleep time in microseconds
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_sleep_us(uint64 sleep_us);

/**
 * @brief       Initializes a timer and starts increasing its count
 *
 * @note        #VVDRV_deo_time_init() should be called prior to calling 
 *              this API
 *
 * @param[out]  timer   Timer handle
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_timer_start(deo_time_timer_t *timer);

/**
 * @brief       Returns the elapsed time (in milliseconds) since the last call
 *              to #VVDRV_deo_time_timer_start()
 *
 * @note        #VVDRV_deo_time_init() should be called prior to calling 
 *              this API
 *
 * @warning     If #VVDRV_deo_time_start_timer() was not called before calling 
 *              this API, the elapsed time returned is undefined
 *
 * @note        Rollover (wraparound) will not occur for at least 10 years
 *
 * @param[in]   timer          Timer handle
 * @param[out]  elapsed_ms     Elapsed time in milliseconds
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_timer_get_elapsed_ms
(
    const deo_time_timer_t *timer,
    uint64                 *elapsed_ms
);

/**
 * @brief       Returns the elapsed time (in microseconds) since the last call
 *              to #VVDRV_deo_time_timer_start()
 *
 * @note        #VVDRV_deo_time_init() should be called prior to calling 
 *              this API
 *
 * @warning     If #VVDRV_deo_time_start_timer() was not called before calling
 *              this API, the elapsed time returned is undefined
 *
 * @note        Rollover (wraparound) will not occur for at least 10 years
 *
 * @param[in]   timer          Timer handle
 * @param[out]  elapsed_us     Elapsed time in microseconds
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_timer_get_elapsed_us
(
    const deo_time_timer_t *timer,
    uint64                 *elapsed_us
);

/**
 * @brief       Returns the elapsed time (in clock ticks) since the last call
 *              to #VVDRV_deo_time_timer_start()
 *
 * @note        #VVDRV_deo_time_init() should be called prior to calling 
 *              this API
 *
 * @note        Use #VVDRV_deo_time_get_clock_frequency() to get the number
 *              of clock ticks per second
 *
 * @warning     If #VVDRV_deo_time_start_timer() was not called before calling
 *              this API, the elapsed time returned is undefined
 *
 * @note        Rollover (wraparound) will not occur for at least 10 years
 *
 * @param[in]   timer          Timer handle
 * @param[out]  elapsed_ticks  Elapsed time, in clock ticks
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_timer_get_elapsed_ticks
(
    const deo_time_timer_t *timer,
    uint64                 *elapsed_ticks
);

/**
 * @brief       Gets the clock frequency, which is also the number of clock
 *              ticks per second
 *
 * @note        #VVDRV_deo_time_init() should be called prior to calling 
 *              this API
 *
 * @param[out]  frequency      Clock frequency, in Hz (also ticks/second)
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_get_clock_frequency(uint32 *frequency);

/**
 * @brief       Gets the current time (in milliseconds) since reset.
 *
 * @note        #VVDRV_deo_time_init() should be called prior to calling 
 *              this API
 *
 * @warning     Rollover (wraparound) is based on the implementation of
 *              #VVDRV_deo_time_os_get_clock_count()
 *
 * @param[out]  current_time_ms Current time in milliseconds
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_get_time_ms(uint64 *current_time_ms);

/**
 * @brief       Gets the current time (in microseconds) since reset.
 *
 * @note        #VVDRV_deo_time_init() should be called prior to calling 
 *              this API
 *
 * @warning     Rollover (wraparound) will occure based on implementation of
 *              #VVDRV_deo_time_os_get_clock_count()
 *
 * @param[out]  time_us time in microseconds
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_get_time_us(uint64 *current_time_us);

/**
 * @brief       Gets the current time (in clock ticks) since reset.
 *
 * @note        #VVDRV_deo_time_init() should be called prior to calling 
 *              this API
 *
 * @warning     Rollover (wraparound) is based on the implementation of
 *              #VVDRV_deo_time_os_get_clock_count()
 *
 * @param[out]  current_time_ticks  Current time in clock ticks
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_time_get_time_ticks(uint64 *current_time_ticks);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __VVDRV_DEO_TIME_H__ */
