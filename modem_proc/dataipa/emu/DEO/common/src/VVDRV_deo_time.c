/**-----------------------------------------------------------------------------
 * @file  VVDRV_deo_time.c
 *
 * @brief Implements DEO time service
 *
 * Copyright (c) 2011-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/

#include "VVDRV_deo_debug.h"      /* deo debug service   */
#include "VVDRV_deo_time.h"       /* deo_time_t          */

#if !defined(DEO_TIME_OS_CLK_NONE)
#include "VVDRV_deo_time_os.h"    /* OS layer API        */
#endif /* !DEO_TIME_OS_CLK_NONE */

/*------------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Static Variable Definitions
 *----------------------------------------------------------------------------*/
 #if !defined(DEO_TIME_OS_CLK_NONE)

/** Flag to indicates if #VVDRV_deo_time_init() was successfully called */
static boolean deo_time_is_init = FALSE;

#endif /* !DEO_TIME_OS_CLK_NONE */

/*------------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *----------------------------------------------------------------------------*/

#if !defined(DEO_TIME_OS_CLK_NONE)

/**
 * @brief   Checks if a given timer is corrupted or not, via a "magic" pattern
 *
 * @param   timer   handler to a timer
 *
 * @return  DEO_SUCCESS if successful, otherwise failed
 */
static int32 deo_time_timer_is_valid(const deo_time_timer_t *timer)
{
    if (NULL == timer)
    {
        DEO_DBG_E(DEO_TIME, ("NULL timer handle\n"));
        return DEO_ERROR;
    }

    return DEO_SUCCESS;
}

#endif /* !DEO_TIME_OS_CLK_NONE */

/*------------------------------------------------------------------------------
 * Externalized Function Definitions
 *----------------------------------------------------------------------------*/
#if !defined(DEO_TIME_OS_CLK_NONE)

int32 VVDRV_deo_time_init(void)
{
    if (TRUE == deo_time_is_init)
    {
        return DEO_SUCCESS;
    }

    /* Init the OS layer */
    if (DEO_SUCCESS != VVDRV_deo_time_os_init())
    {
        DEO_DBG_E(DEO_TIME, ("DEO time OS-layer init failed\n"));
        return DEO_ERROR;
    }

    deo_time_is_init = TRUE;

    return DEO_SUCCESS;
}

void VVDRV_deo_time_uninit(void)
{
    if (TRUE == deo_time_is_init)
    {
        if (DEO_SUCCESS != VVDRV_deo_time_os_uninit())
        {
            DEO_DBG_E(DEO_TIME, ("DEO time un-init failed\n"));
        }

        deo_time_is_init = FALSE;
    }
}

int32 VVDRV_deo_time_sleep_ms(uint64 sleep_ms)
{
    return VVDRV_deo_time_sleep_us(sleep_ms * 1000U);
}

int32 VVDRV_deo_time_sleep_us(uint64 sleep_us)
{
    deo_time_timer_t timer;
    uint64 elapsed_us = 0;

    if (TRUE != deo_time_is_init)
    {
        DEO_DBG_E(DEO_TIME, ("DEO time not initialized\n"));
    }

    if (DEO_SUCCESS != VVDRV_deo_time_timer_start(&timer))
    {
        return DEO_ERROR;
    }

    while (elapsed_us < sleep_us)
    {
        if (DEO_SUCCESS != 
               VVDRV_deo_time_timer_get_elapsed_us(&timer, &elapsed_us))
        {
            return DEO_ERROR;
        }
    }

    return DEO_SUCCESS;
}

int32 VVDRV_deo_time_timer_get_elapsed_ms
(
    const deo_time_timer_t *timer,
    uint64                 *elapsed_ms
)
{
    /* Get elapsed ticks (in elapsed_ms param). Note that param checking and
     * error messages are handled by #VVDRV_deo_time_timer_get_elapsed_ticks()
     */
    if (DEO_SUCCESS != 
           VVDRV_deo_time_timer_get_elapsed_ticks(timer, elapsed_ms))
    {
        return DEO_ERROR;
    }

    return VVDRV_deo_time_os_ticks_to_ms(*elapsed_ms, elapsed_ms);
}

int32 VVDRV_deo_time_timer_get_elapsed_us
(
    const deo_time_timer_t *timer,
    uint64                 *elapsed_us
)
{
    /* Get elapsed ticks (in elapsed_us param). Note that param checking and
     * error messages are handled by #VVDRV_deo_time_timer_get_elapsed_ticks()
     */
    if (DEO_SUCCESS != 
           VVDRV_deo_time_timer_get_elapsed_ticks(timer, elapsed_us))
    {
        return DEO_ERROR;
    }

    return VVDRV_deo_time_os_ticks_to_us(*elapsed_us, elapsed_us);
}

int32 VVDRV_deo_time_timer_get_elapsed_ticks
(
    const deo_time_timer_t *timer,
    uint64                 *elapsed_ticks
)
{
    uint64 current_ticks;

    if (TRUE != deo_time_is_init)
    {
        DEO_DBG_E(DEO_TIME, ("DEO time not initialized\n"));
        return DEO_ERROR;
    }

    if (DEO_SUCCESS != deo_time_timer_is_valid(timer))
    {
        DEO_DBG_E(DEO_TIME, ("DEO time invalid timer\n"));
        return DEO_ERROR;
    }

    if (NULL == elapsed_ticks)
    {
        DEO_DBG_E(DEO_TIME, ("DEO time NULL param\n"));
        return DEO_ERROR;
    }

    if (DEO_SUCCESS != VVDRV_deo_time_os_get_clock_count(&current_ticks))
    {
        DEO_DBG_E(DEO_TIME, ("DEO time cannot get the current ticks\n"));
        return DEO_ERROR;
    }

    *elapsed_ticks = current_ticks - *timer;

    return DEO_SUCCESS;
}

int32 VVDRV_deo_time_get_clock_frequency(uint32 *frequency)
{
    if (TRUE != deo_time_is_init)
    {
        DEO_DBG_E(DEO_TIME, ("DEO time not initialized\n"));
        return DEO_ERROR;
    }

    if (NULL == frequency)
    {
        DEO_DBG_E(DEO_TIME, ("DEO time NULL param\n"));
        return DEO_ERROR;
    }

    /* Get the clock frequency */
    return VVDRV_deo_time_os_get_clock_frequency(frequency);
}

int32 VVDRV_deo_time_timer_start(deo_time_timer_t *timer)
{
    uint64 current_ticks;

    if (TRUE != deo_time_is_init)
    {
        DEO_DBG_E(DEO_TIME, ("DEO time not initialized\n"));
        return DEO_ERROR;
    }

    if (NULL == timer)
    {
        DEO_DBG_E(DEO_TIME, ("DEO time NULL param\n"));
        return DEO_ERROR;
    }

    /* Get the current ticks count */
    if (DEO_SUCCESS != VVDRV_deo_time_os_get_clock_count(&current_ticks))
    {
        DEO_DBG_E(DEO_TIME, ("DEO time cannot get current ticks\n"));
        return DEO_ERROR;
    }

    /* Set the timer */
    *timer = current_ticks;

    return DEO_SUCCESS;
}

int32 VVDRV_deo_time_get_time_ms(uint64 *current_time_ms)
{
    /* Get time ticks (in current_time_ms param). Note that param checking and
     * error messages are handled by #VVDRV_deo_time_get_time_ticks()
     */
    if (DEO_SUCCESS != VVDRV_deo_time_get_time_ticks(current_time_ms))
    {
        return DEO_ERROR;
    }

    /* Get current microseconds from current ticks */
    return VVDRV_deo_time_os_ticks_to_us(*current_time_ms, current_time_ms);
}

int32 VVDRV_deo_time_get_time_us(uint64 *current_time_us)
{
    /* Get time ticks (in current_time_us param). Note that param checking and
     * error messages are handled by #VVDRV_deo_time_get_time_ticks()
     */
    if (DEO_SUCCESS != VVDRV_deo_time_get_time_ticks(current_time_us))
    {
        return DEO_ERROR;
    }

    /* Get current microseconds from current ticks */
    return VVDRV_deo_time_os_ticks_to_us(*current_time_us, current_time_us);
}

int32 VVDRV_deo_time_get_time_ticks(uint64 *current_time_ticks)
{
    if (TRUE != deo_time_is_init)
    {
        DEO_DBG_E(DEO_TIME, ("DEO time not initialized\n"));
        return DEO_ERROR;
    }

    if (NULL == current_time_ticks)
    {
        DEO_DBG_E(DEO_TIME, ("NULL param\n"));
        return DEO_ERROR;
    }

    /* Get the current ticks count */
    if (DEO_SUCCESS != VVDRV_deo_time_os_get_clock_count(current_time_ticks))
    {
        DEO_DBG_E(DEO_TIME, ("Cannot get current ticks\n"));
        return DEO_ERROR;
    }

    return DEO_SUCCESS;
}

#else /* DEO_TIME_OS_CLK_NONE */

int32 VVDRV_deo_time_init(void)
{
    return DEO_SUCCESS;     /* Not supported, DEO_TIME_OS_CLK_NONE defined */
}

void VVDRV_deo_time_uninit(void)
{
    /* Not supported, DEO_TIME_OS_CLK_NONE defined */
}

int32 VVDRV_deo_time_sleep_ms(uint64 sleep_ms)
{
    (void)sleep_ms;         /* Unused */

    return DEO_ERROR;       /* Not supported, DEO_TIME_OS_CLK_NONE defined */
}

int32 VVDRV_deo_time_sleep_us(uint64 sleep_us)
{
    (void)sleep_us;         /* Unused */

    return DEO_ERROR;       /* Not supported, DEO_TIME_OS_CLK_NONE defined */
}

int32 VVDRV_deo_time_timer_start(deo_time_timer_t *timer)
{
    (void)timer;            /* Unused */

    return DEO_ERROR;       /* Not supported, DEO_TIME_OS_CLK_NONE defined */
}

int32 VVDRV_deo_time_timer_get_elapsed_ms
(
    const deo_time_timer_t *timer,
    uint64                 *elapsed_ms
)
{
    (void)timer;            /* Unused */
    (void)elapsed_ms;       /* Unused */

    return DEO_ERROR;       /* Not supported, DEO_TIME_OS_CLK_NONE defined */
}

int32 VVDRV_deo_time_timer_get_elapsed_us
(
    const deo_time_timer_t *timer,
    uint64                 *elapsed_us
)
{
    (void)timer;            /* Unused */
    (void)elapsed_us;       /* Unused */

    return DEO_ERROR;       /* Not supported, DEO_TIME_OS_CLK_NONE defined */
}

int32 VVDRV_deo_time_timer_get_elapsed_ticks
(
    const deo_time_timer_t *timer,
    uint64                 *elapsed_ticks
)
{
    (void)timer;            /* Unused */
    (void)elapsed_ticks;    /* Unused */

    return DEO_ERROR;       /* Not supported, DEO_TIME_OS_CLK_NONE defined */
}

int32 VVDRV_deo_time_get_clock_frequency(uint32 *frequency)
{
    (void)frequency;        /* Unused */

    return DEO_ERROR;       /* Not supported, DEO_TIME_OS_CLK_NONE defined */
}

int32 VVDRV_deo_time_get_time_ms(uint64 *current_time_ms)
{
    (void)current_time_ms;  /* Unused */

    return DEO_ERROR;       /* Not supported, DEO_TIME_OS_CLK_NONE defined */
}

int32 VVDRV_deo_time_get_time_us(uint64 *current_time_us)
{
    (void)current_time_us;  /* Unused */

    return DEO_ERROR;       /* Not supported, DEO_TIME_OS_CLK_NONE defined */
}

int32 VVDRV_deo_time_get_time_ticks(uint64 *current_time_ticks)
{
    (void)current_time_ticks;  /* Unused */

    return DEO_ERROR;          /* Not supported, DEO_TIME_OS_CLK_NONE defined */
}

#endif /* !DEO_TIME_OS_CLK_NONE */
