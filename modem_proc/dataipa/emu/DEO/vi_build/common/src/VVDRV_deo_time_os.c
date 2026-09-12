/**-----------------------------------------------------------------------------
 * @file  VVDRV_deo_time_os.c
 *
 * @brief Implements the VI_BUILD OS-specific time service functions. Three
 *        different implementations are provided, depending on user-specified
 *        compile flags, as follows:
 *        1. DEO_TIME_OS_CLK_NONE           Empty implementation. 
 *
 *        2. DEO_TIME_OS_CLK_SLEEP_CLK      Implementation using SLEEP_TIMETICK
 *                                          clock (32768Hz). Provides accuracy
 *                                          better than 1 millisecond. If no
 *                                          flag is specified this is the
 *                                          default implementation
 *
 *        3. DEO_TIME_OS_CLK_QTIMER         Implementation using QTimer counter
 *                                          (usually 19.2MHz). Provides accuracy
 *                                          better than microsecond, depending
 *                                          on the HW capabilities
 *
 * Copyright (c) 2014-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *----------------------------------------------------------------------------*/

#if !defined(DEO_TIME_OS_CLK_NONE)

/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/

#include "VVDRV_deo_time_os.h"         /* API declarations                    */
#include "VVDRV_deo_os.h"              /* deo_os_memset                       */
#include "VVDRV_deo_debug.h"           /* DEO debug service                   */
#include <msmhwio.h>                   /* HWIO_IN                             */
#include "VVDRV_deo_os_platform.h"     /* Relevant timer definitions          */
#include DEO_TIME_OS_HWIO_FILE         /* msmhwioreg file required for timers */

/*------------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *----------------------------------------------------------------------------*/

/* Protect from missing platform definition.
 * Must define one of ASIC, RUMI, VELOCE
 */
#if (!defined(ASIC) && !defined(RUMI) && !defined(VELOCE))
#error Missing platform definition, must define one of ASIC,RUMI,VELOCE
#endif /* (!ASIC && !RUMI && !VELOCE) */

/* Protect from multiple contradicting platform definitions.
 * Define one and only one of ASIC, RUMI, VELOCE
 */
#if ((defined(ASIC) && defined(RUMI))   || \
     (defined(ASIC) && defined(VELOCE)) || \
     (defined(RUMI) && defined(VELOCE)))
#error Multiple platform definitions, define only one of ASIC,RUMI,VELOCE
#endif /* (ASIC && RUMI) || (ASIC && VELOCE) || (RUMI && VELOCE) */

/* If no compile flag specified, default to SLEEP_TIMETICK timer */
#if (!defined(DEO_TIME_OS_CLK_SLEEP_CLK) && !defined(DEO_TIME_OS_CLK_QTIMER))
#define DEO_TIME_OS_CLK_SLEEP_CLK
#endif /* !DEO_TIME_OS_CLK_SLEEP_CLK && !DEO_TIME_OS_CLK_QTIMER */

/* Protect from contradicting clock definitions */
#if ( defined(DEO_TIME_OS_CLK_QTIMER) &&  defined(DEO_TIME_OS_CLK_SLEEP_CLK) || \
     !defined(DEO_TIME_OS_CLK_QTIMER) && !defined(DEO_TIME_OS_CLK_SLEEP_CLK))
#error Timer definitions mismatch
#endif /*  (DEO_TIME_OS_CLK_QTIMER && DEO_TIME_OS_CLK_SLEEP_CLK) ||
        *  (!DEO_TIME_OS_CLK_QTIMER && !DEO_TIME_OS_CLK_SLEEP_CLK) 
        */

/** Maximal number of tries to detect if the clock is ticking */
#define DEO_TIME_CLOCK_TICK_MAX_TRIES   100000

/** Calculates ceil(a/b) when a and b are integers */
#define DEO_TIME_DIV_CEIL(a, b)         (((a) + (b) - 1) / (b))

/** Calculates round(a/b) when a and b are integers */
#define DEO_TIME_DIV_ROUND(a, b)        (((a) + ((b)/2))/(b))

#if defined(DEO_TIME_OS_CLK_SLEEP_CLK)
#if !defined(DEO_TIME_OS_VI_BUILD_SLEEP_TIMETICK_FREQ)
/** Default frequency of SLEEP_TIMETICK clock which is constant and equal to: 
 *  32768[ticks/sec]  
 */
#define DEO_TIME_OS_VI_BUILD_SLEEP_TIMETICK_FREQ          32768
#endif /* DEO_TIME_OS_VI_BUILD_SLEEP_TIMETICK_FREQ */
#endif /* DEO_TIME_OS_CLK_SLEEP_CLK */

#if defined(DEO_TIME_OS_CLK_QTIMER)
/** Scaling factor for clock frequency to eliminate overflow. Warning: do not
 *  change this value without carefully reviewing related code, otherwise
 *  reduced accuracy or calculation overflows might occur. For explanation
 *  about selected value, see #VVDRV_deo_time_os_ticks_to_us()
 *  and #VVDRV_deo_time_os_us_to_ticks()
 */
#define DEO_TIME_OS_FREQ_SCALE      1000
#endif /* DEO_TIME_OS_CLK_QTIMER */

/*------------------------------------------------------------------------------
 * Type Declarations
 *----------------------------------------------------------------------------*/

/** Contains OS time-related information and other information needed for
 *  timer abstraction
 */
typedef struct deo_time_os_info_s
{
    uint32 frequency;           /**< Clock frequency, in Hz Also ticks/second */

#if defined(DEO_TIME_OS_CLK_SLEEP_CLK)
    uint32 start_count;         /**< Start tick count from native API */
#endif /* DEO_TIME_OS_CLK_SLEEP_CLK */

#if defined(DEO_TIME_OS_CLK_QTIMER)
    uint32 freq_scaled;     /**< Clock freq divided by DEO_TIME_OS_FREQ_SCALE */
    boolean need_uninit_global_timer;  /**< TRUE if global timer need un-init */
    boolean need_uninit_core_timer;      /**< TRUE if core timer need un-init */
#endif /* DEO_TIME_OS_CLK_QTIMER */

} deo_time_os_info_t;

/*------------------------------------------------------------------------------
 * Static Variable Definitions
 *----------------------------------------------------------------------------*/

/** Holds time OS-layer related data */
static deo_time_os_info_t   deo_time_os_gInfo = {0};

/*------------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *----------------------------------------------------------------------------*/

/*------------------ Sleep timer -  32KHz ------------------------------------*/
#if defined(DEO_TIME_OS_CLK_SLEEP_CLK)

#if defined(ASIC)
/**
 * @brief        ASIC version. Gets the current tick count from Sleep clock
 *
 * @param[out]   count    ticks count as retrieved from the sleep clock
 */
static void deo_time_os_get_sleep_clock_count(uint32 *count) /* DEO_TIME_OS_CLK_SLEEP_CLK & ASIC */
{
    volatile uint32 count1, count2;

    /* Reading the counter value once may not return an accurate value if the
     * counter is in the processing of counting to the next value, and several
     * bits are changing.  Instead, the counter is to be repeatedly read until
     * a consistent value is read.
     */

    do
    {
        count1 = HWIO_IN(DEO_TIME_OS_VI_BUILD_SLEEP_TIMETICK_COUNT);
        count2 = HWIO_IN(DEO_TIME_OS_VI_BUILD_SLEEP_TIMETICK_COUNT);
    } while (count1 != count2);

    /* Counter returned the same value twice in a row so should be stable */
   *count = count2;
}

#else /* defined(RUMI) || defined(VELOCE) */

/**
 * @brief        RUMI version. Gets the current tick count from Sleep clock
 *
 * @param[out]   count    ticks count as retrieved from the sleep clock
 */
static void deo_time_os_get_sleep_clock_count(uint32 *count) /* DEO_TIME_OS_CLK_SLEEP_CLK & RUMI */
{
    /* Since when working on the RUMI the CPU runs very slow,
     * it is possible that it will get stuck on two consequent reads
     * So we perform only one read and return it value.
     */
   *count = HWIO_IN(DEO_TIME_OS_VI_BUILD_SLEEP_TIMETICK_COUNT);
}

#endif /* ASIC || RUMI */

#endif /* DEO_TIME_OS_CLK_SLEEP_CLK */

/*------------------ Performance timer -  19.2MHz ----------------------------*/
#if defined(DEO_TIME_OS_CLK_QTIMER)

/**
 * @brief        Initializes the performance clock HW and sets frequency
 */
static void deo_time_os_perf_clock_init(void)   /* DEO_TIME_OS_CLK_QTIMER */
{
    /* The following procedure is documented in:
     * MPM2 Core - Hardware Programming Guide - 80-NB684-1H Rev. C
     * Section: 2.5 Global counter
     *
     * https://projects.qualcomm.com/sites/IPCatalog/_layouts/DocIdRedir.aspx?ID=TTYVYFYYK6ED-1-42
     */

    deo_time_os_gInfo.need_uninit_global_timer = FALSE;
    deo_time_os_gInfo.need_uninit_core_timer   = FALSE;

    /* Check if global timer is already initialized */
    if (0x0 == in_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_ENABLE))
    {
        /* Global timer and core timer needs to be initialized */

        /* Set frequency: do not write the register to set frequency, since in
         * contradiction to HPG it seems that any value can be written, but
         * there is no effect on the real frequency.
         */

        /* Clear the counter low and high bits */
        out_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_LO, 0);
        out_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_HI, 0);

        /* Enable the global counter */
        out_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_ENABLE, 0x1);
        deo_time_os_gInfo.need_uninit_global_timer = TRUE;

#ifdef DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL
        /* Enable the core counter */
        out_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL, 0x03f);
        deo_time_os_gInfo.need_uninit_core_timer = TRUE;
#endif /* DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL */
    }
    else
    {
#ifdef DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL
        /* Check if core counter needs to be initialized */
        if (0x3f !=
                in_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL))
        {
            /* Enable the core counter */
            out_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL,
                      0x03f);
            deo_time_os_gInfo.need_uninit_core_timer = TRUE;
        }
#endif /* DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL */
    }

    /* Get clock frequency to be used later
     * The frequency should be read from HW, however, in some platforms
     * (e.g. emulation) the value read is not reflect the actual frequency.
     * So, if the user defines DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_FREQ
     * that value is used instead of the read value.
     */
#if defined(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_FREQ)
    /* Set the frequency to the user defined value */
    deo_time_os_gInfo.frequency = DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_FREQ;
#else /* !DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_FREQ */
    /* Read the frequency from HW */
    deo_time_os_gInfo.frequency =
            in_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_CONTROL);
#endif /* DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_FREQ */
}

#endif /* DEO_TIME_OS_CLK_QTIMER */

/*-------------------Common static functions ---------------------------------*/

/**
 * @brief   Sanity check function, checks if the clock is counting by doing the
 *          following:
 *              1. Read the clock start count
 *              2. Read the clock again and compare to start count
 *              3. If count has changed, clock is ticking
 *
 * @return  DEO_SUCCESS if successful, otherwise failed
 */
static int32 deo_time_os_is_clock_ticking(void)
{
    uint32 iter = 0;
    uint64 start_ticks = 0;
    uint64 current_ticks = 0;

    if (DEO_SUCCESS != VVDRV_deo_time_os_get_clock_count(&start_ticks))
    {
        return DEO_ERROR;
    }

    for (iter = 0 ; iter <= DEO_TIME_CLOCK_TICK_MAX_TRIES ; iter++)
    {
        /* Get the current tick count */
        if (DEO_SUCCESS != VVDRV_deo_time_os_get_clock_count(&current_ticks))
        {
            return DEO_ERROR;
        }

        if (current_ticks > start_ticks)
        {
            return DEO_SUCCESS;
        }

        /* The tick count must be monotonically increasing */
        DEO_ASSERT((current_ticks == start_ticks),
                                      ("not monotonically increasing timer\n"));
    }

    /* Timer did not change, failure */
    return DEO_ERROR;
}

/*------------------------------------------------------------------------------
 * Externalized Function Definitions
 *----------------------------------------------------------------------------*/

int32 VVDRV_deo_time_os_get_clock_frequency(uint32 *frequency) /* !DEO_TIME_OS_CLK_NONE */
{
    *frequency = deo_time_os_gInfo.frequency;

    return DEO_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* DEO_TIME_OS_CLK_SLEEP_CLK - 32KHz TIMER                                    */
/*                                                                            */
/* This section of the code contains 32KHz TIMER-related functions            */
/*----------------------------------------------------------------------------*/
#if defined(DEO_TIME_OS_CLK_SLEEP_CLK)

int32 VVDRV_deo_time_os_init(void) /* DEO_TIME_OS_CLK_SLEEP_CLK */
{
    /* Clear the internal deo_time_os_gInfo data structure */
    deo_os_memset(&deo_time_os_gInfo, 0, sizeof(deo_time_os_gInfo));

    /* Save the frequency */
    deo_time_os_gInfo.frequency = DEO_TIME_OS_VI_BUILD_SLEEP_TIMETICK_FREQ;

    /* Get and store initial system count, must be done before any call
       to #VVDRV_deo_time_os_get_clock_count() which use the start_count */
    deo_time_os_get_sleep_clock_count(&deo_time_os_gInfo.start_count);
    
    /* Verify that clock is indeed ticking */
    if (DEO_SUCCESS != deo_time_os_is_clock_ticking())
    {
        DEO_DBG_F(DEO_TIME, ("Timer is not ticking\n"));
        return DEO_ERROR;
    }

    return DEO_SUCCESS;
}

/*----------------------------------------------------------------------------*/
int32 VVDRV_deo_time_os_uninit(void) /* DEO_TIME_OS_CLK_SLEEP_CLK */
{
    return DEO_SUCCESS;         /* Do nothing */
}

/*----------------------------------------------------------------------------*/
int32 VVDRV_deo_time_os_get_clock_count(uint64 *tick_count) /* DEO_TIME_OS_CLK_SLEEP_CLK */
{
    /* SLEEP_TIMETICK timer API provides 32-bit result, so with 32KHz frequency
     * it will wraparound ~1.5 days since power-up.
     * The following implementation removes changes the behavior to wraparound
     * ~1.5 days since timer initialization instead of power-up
     */
    uint32 current_count32;

    /* Read from HW */
    deo_time_os_get_sleep_clock_count(&current_count32);

    /* Update tick count with delta since timer initialization call.
     * Note: 2's complement
     */
    *tick_count = (uint64)(current_count32 - deo_time_os_gInfo.start_count);

    return DEO_SUCCESS;
}

/*----------------------------------------------------------------------------*/
int32 VVDRV_deo_time_os_ticks_to_ms(uint64 ticks, uint64 *ms) /* DEO_TIME_OS_CLK_SLEEP_CLK */
{
    /* Note that following calculation will overflow only when:
     * ticks >= (2^64 / 1000) =~ 17800 years since power-up
     * (for frequency = 32KHz)
     */

    *ms = (1000 * ticks) / (deo_time_os_gInfo.frequency);

    return DEO_SUCCESS;
}

/*----------------------------------------------------------------------------*/
int32 VVDRV_deo_time_os_ticks_to_us(uint64 ticks, uint64 *us) /* DEO_TIME_OS_CLK_SLEEP_CLK */
{
    /* Note that following calculation will overflow only when:
     * ticks >= (2^64 / 1000000) =~ 17.8 years since power-up
     * (for frequency = 32KHz)
     */

    *us = (1000000 * ticks) / (deo_time_os_gInfo.frequency);

    return DEO_SUCCESS;
}

/*----------------------------------------------------------------------------*/
int32 VVDRV_deo_time_os_us_to_ticks(uint64 us, uint64 *ticks) /* DEO_TIME_OS_CLK_SLEEP_CLK */
{
    /* Note: the general form is:
     *      *ticks = (us * frequency) / 1000000;
     * However this will always round down. For a slow clock when a tick is
     * longer than a microsecond, if we wait for the resulting amount of
     * ticks, we might wait less than the specified amount of microseconds,
     * thus waiting less than required. So we use a "ceiling" implementation:
     *      *ticks = DEO_TIME_DIV_CEIL(us * frequency), 1000000);
     *
     * Note: the following calculation will overflow only when:
     * (us * frequency) + 1000000 >= (2^64), which gives: us =~ 17.8 years
     * (for frequency = 32KHz)
     */

    *ticks = DEO_TIME_DIV_CEIL(us * (deo_time_os_gInfo.frequency), 1000000);

    return DEO_SUCCESS;
}

#endif  /* DEO_TIME_OS_CLK_SLEEP_CLK */

/*----------------------------------------------------------------------------*/
/* DEO_TIME_OS_CLK_QTIMER - 19.2 MHz TIMER                                    */
/*                                                                            */
/* This section of the code contains 19.2 MHz TIMER-related functions         */
/*----------------------------------------------------------------------------*/
#if defined(DEO_TIME_OS_CLK_QTIMER)

int32 VVDRV_deo_time_os_init(void) /* DEO_TIME_OS_CLK_QTIMER */
{
    /* Clear the internal deo_time_os_gInfo data structure */
    deo_os_memset(&deo_time_os_gInfo, 0, sizeof(deo_time_os_gInfo));

    /* Initialize the HW */
    deo_time_os_perf_clock_init();

    /* Sanity check the frequency */
    if (0 == deo_time_os_gInfo.frequency)
    {
        DEO_DBG_F(DEO_TIME, ("Zero clock frequency %u\n",
                             deo_time_os_gInfo.frequency));
        return DEO_ERROR;
    }

    /* Check if frequency scale used is optimal, warn otherwise */
    if (0 != (deo_time_os_gInfo.frequency % DEO_TIME_OS_FREQ_SCALE))
    {
        DEO_DBG_W(DEO_TIME, ("Frequency scaling used might reduce accuracy\n"));
    }

    /* Check that clock in indeed ticking */
    if (DEO_SUCCESS != deo_time_os_is_clock_ticking())
    {
        DEO_DBG_F(DEO_TIME, ("Timer is not ticking\n"));
        return DEO_ERROR;
    }

    /* Calculate scaled frequency to reduce calculation overhead later */
    deo_time_os_gInfo.freq_scaled =
                                DEO_TIME_DIV_ROUND(deo_time_os_gInfo.frequency,
                                                   DEO_TIME_OS_FREQ_SCALE);

    return DEO_SUCCESS;
}

/*----------------------------------------------------------------------------*/
int32 VVDRV_deo_time_os_uninit(void) /* DEO_TIME_OS_CLK_QTIMER */
{
#ifdef DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL
    /* If needed, disable the core counter */
    if (TRUE == deo_time_os_gInfo.need_uninit_core_timer)
    {
        out_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL, 0);
        deo_time_os_gInfo.need_uninit_core_timer = FALSE;
    }
#endif /* DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL */

    /* Disable the global counter */
    if (TRUE == deo_time_os_gInfo.need_uninit_global_timer)
    {
        out_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_ENABLE, 0);
        deo_time_os_gInfo.need_uninit_global_timer = FALSE;
    }

    return DEO_SUCCESS;
}

/*----------------------------------------------------------------------------*/
int32 VVDRV_deo_time_os_get_clock_count(uint64 *tick_count) /* DEO_TIME_OS_CLK_QTIMER */
{
    volatile uint32 hi1, hi2, low;

    do
    {

#ifdef DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL
        /* Read the core timer */
        hi1 = in_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_HI);
        low = in_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_LO);
        hi2 = in_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_HI);
#else
        /* Read the global timer */
        hi1 = in_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_HI);
        low = in_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_LO);
        hi2 = in_dword(DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_GLOBAL_HI);
#endif /* DEO_TIME_OS_VI_BUILD_QTIMER_COUNTER_CORE_CONTROL */

    } while (hi1 != hi2);

    /* Update the return value */
    *tick_count = ((((uint64)hi1) << 32) | ((uint64)low));

    return DEO_SUCCESS;
}

/*----------------------------------------------------------------------------*/
int32 VVDRV_deo_time_os_ticks_to_ms(uint64 ticks, uint64 *ms) /* DEO_TIME_OS_CLK_QTIMER */
{
    /* Note that following calculation will overflow only when:
     * ticks >= (2^64 / 1000) =~ 29 years since power-up
     * (for frequency = 20MHz)
     */

    *ms = (1000 * ticks) / (deo_time_os_gInfo.frequency);

    return DEO_SUCCESS;
}

/*----------------------------------------------------------------------------*/
int32 VVDRV_deo_time_os_ticks_to_us(uint64 ticks, uint64 *us) /* DEO_TIME_OS_CLK_QTIMER */
{
    /* Note that following general calculation will overflow only when:
     * ticks >= (2^64 / 1000000) =~ 11.1 days since power-up
     * (for frequency = 19.2MHz).
     *
     * The general form:
     *      *us = (1000000 * ticks) / deo_time_os_gInfo.frequency;
     *
     * To guarantee a longer period before overflow, we scale (divide) both
     * numerator and denominator by same constant DEO_TIME_OS_FREQ_SCALE.
     * For a chosen constant DEO_TIME_OS_FREQ_SCALE = 1000:
     *     1. It allows a suitable period (see below).
     *     2. It divides 1000000 with no remainder.
     *     3. It divides clocks like 20MHz or 19.2MHz with no reminder.
     * The following calculation will overflow only when:
     * ticks >= (2^64 / (1000000/DEO_TIME_OS_FREQ_SCALE)) =~ 29 years
     * since power-up (for frequency = 20MHz).
     */

    *us = ((1000000/DEO_TIME_OS_FREQ_SCALE) * ticks) /
                                            (deo_time_os_gInfo.freq_scaled);

    return DEO_SUCCESS;
}

/*----------------------------------------------------------------------------*/
int32 VVDRV_deo_time_os_us_to_ticks(uint64 us, uint64 *ticks) /* DEO_TIME_OS_CLK_QTIMER */
{
    /* Note: the general form is:
     *      *ticks = (us * frequency) / 1000000;
     * However this will always round down. For a slow clock when a tick is
     * longer than a microsecond, if we wait for the resulting amount of
     * ticks, we might wait less than the specified amount of microseconds,
     * thus waiting less than required. So we use a "ceiling" implementation:
     *      *ticks = DEO_TIME_DIV_CEIL(us * frequency), 1000000);
     *
     * To guarantee a longer period before overflow, we scale (divide) both
     * numerator and denominator by same constant DEO_TIME_OS_FREQ_SCALE.
     * For a chosen constant DEO_TIME_OS_FREQ_SCALE = 1000:
     *     1. It allows a suitable period (see below).
     *     2. It divides 1000000 with no remainder.
     *     3. It divides clocks like 20MHz or 19.2MHz with no reminder.
     *
     * Note: the following calculation will overflow only when:
     * (us * freq_scaled) + (1000000/DEO_TIME_OS_FREQ_SCALE) >= (2^64),
     *  which gives: us =~ 29 years (for frequency = 20MHz)
     */

    *ticks = DEO_TIME_DIV_CEIL(us * (deo_time_os_gInfo.freq_scaled),
                               (1000000/DEO_TIME_OS_FREQ_SCALE));

    return DEO_SUCCESS;
}
#endif  /* DEO_TIME_OS_CLK_QTIMER */

#endif /* !DEO_TIME_OS_CLK_NONE */
