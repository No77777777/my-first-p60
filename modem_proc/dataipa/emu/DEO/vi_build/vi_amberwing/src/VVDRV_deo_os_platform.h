#ifndef __VVDRV_DEO_OS_PLATFORM_H__
#define __VVDRV_DEO_OS_PLATFORM_H__
/**---------------------------------------------------------------------------
 * @file  VVDRV_deo_os_platform.h
 *
 * @brief Holds the register names of the requested timers as defined in the 
 *        msmhwioreg.h definitions.
 *        This file contains the following definitions (documented below):
 *        For MS accuracy - SLEEP_TIMETICK: 
 *          - DEO_TIME_OS_VI_BUILD_SLEEP_TIMETICK_FREQ
 *          - DEO_TIME_OS_VI_BUILD_SLEEP_TIMETICK_COUNT
 *        For US accuracy - Performance counters - global and core level:
 *          For Global counter:
 *          - DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_HI
 *          - DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_LO
 *          - DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_ENABLE
 *          - DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_CONTROL
 *          - DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_FREQ (optional)
 *          For core counter also define:
 *          - DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL
 *          - DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_HI
 *          - DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_LO
 *
 * @note    The QTimer has two modes of operation, reading the tick count from
 *          the global or the core counter:
 *          - If global counter is used, there is no need to define the core 
 *            timer defines
 *          - In case core counter is used, both global and core counters must 
 *            be defined
 *          Consult with the documentation below for more information. 
 *
 * @note  This file should be included by deo_time_os (VI_BUILD) file only. 
 *
 * Copyright (c) 2014-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *----------------------------------------------------------------------------*/

#ifndef DEO_TIME_OS_HWIO_FILE
/** HWIO Register file alias for the time OS layer */
#define DEO_TIME_OS_HWIO_FILE        "msmhwioreg_MPM.h"
#endif /* DEO_TIME_OS_HWIO_FILE */

/*----------------------------------------------------------------------------*/
/*      SLEEP_TIMETICK clock defines                                          */
/*----------------------------------------------------------------------------*/

/** 
 * @def     DEO_TIME_OS_VI_BUILD_SLEEP_TIMETICK_FREQ
 * 
 * @brief   If defined, overrides the default frequency of SLEEP_TIMETICK clock.
 *          Value is in Hz
 */
#if defined(RUMI)
/* RUMI Sleep timetick works at 4Mhz */
#define DEO_TIME_OS_VI_BUILD_SLEEP_TIMETICK_FREQ          4000000
#endif  /* RUMI */

/** 
 * @brief   Contains the name - as found in the msmhwioreg.h file - of
 *          the SLEEP_TIMETICK tick count value
 *          For example: 
 *              - MPM_SLEEP_TIMETICK_COUNT_VAL
 *              - MPM2_MPM_SLEEP_TIMETICK_COUNT_VAL
 */
#define DEO_TIME_OS_VI_BUILD_SLEEP_TIMETICK_COUNT   \
                                        MPM_SLEEP_TIMETICK_COUNT_VAL

/*----------------------------------------------------------------------------*/
/*      QTimer defines                                                        */
/*----------------------------------------------------------------------------*/

#if defined(RUMI)
/**
 * @def     DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_FREQ
 *
 * @brief   Allows the user to manually define the performance global counter
 *          frequency. Value is in Hz
 * @note    This should not used in the common case, since the frequency will
 *          be read from the HW. Use only if the HW will not report the actual
 *          frequency, e.g. when working on an emulation platform
 */
#define DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_FREQ        4000000
#endif  /* RUMI */
 
/** Define the QTimer global counter high word register address */
#define DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_HI       \
                                        HWIO_MPM_CONTROL_CNTCV_HI_ADDR

/** Define the QTimer global counter low word register address */
#define DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_LO       \
                                        HWIO_MPM_CONTROL_CNTCV_LO_ADDR

/** Define the QTimer global counter enable register address */
#define DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_ENABLE   \
                                        HWIO_MPM_CONTROL_CNTCR_ADDR

/** Define the QTimer global counter control register address */
#define DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_CONTROL  \
                                        HWIO_MPM_CONTROL_CNTFID0_ADDR

/**
 * @def     DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL
 *
 * @brief   Define the QTimer core control register address. If defined the
 *          core counter will be used instead of the global counter
 *          example:  HWIO_QTMR_AC_CNTACR_n_ADDR(0)
 *
 * @note    If this is defined also the following must be defined: 
 *          - DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_HI
 *          - DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_LO
 */

/**
 * @def     DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_HI
 *
 * @brief   Define the QTimer core counter high word register address
 *          example: HWIO_APCS_F0_QTMR_V1_CNTPCT_HI_ADDR
 *
 * @note    Used only if DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL
 *          is defined
 */
 
/**
 * @def     DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_LO
 *
 * @brief   Define the QTimer core counter low word register address
 *          example: HWIO_APCS_F0_QTMR_V1_CNTPCT_LO_ADDR
 *
 * @note    Used only if DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL
 *          is defined
 */

/* Protect from contradicting definitions */
#if defined(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL) &&        \
            (!defined(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_HI) ||   \
             !defined(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_LO))
#error Performance counter definitions mismatch
#endif /* DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL && 
          (!DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_HI ||
           !DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_LO)
        */

#endif /* __VVDRV_DEO_OS_PLATFORM_H__ */
