/**---------------------------------------------------------------------------
 * @file  VVDRV_deo_interrupt.c
 *
 * @brief implements the interface to DEO interrupt services
 *
 * Copyright (c) 2010-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/************************************************************************/
/*                           INCLUDES                                   */
/************************************************************************/

#include "VVDRV_deo_debug.h"        /* deo debugging functions                */
#include "VVDRV_deo_mutex.h"        /* deo mutex                              */
#include "VVDRV_deo_interrupt.h"    /* deo interrupt handling API             */
#include "VVDRV_deo_interrupt_os.h" /* deo interrupt handling OS specific API */

/************************************************************************/
/*                         MACROS & DEFINITIONS                         */
/************************************************************************/

/**
 * @brief           Array elements iterator. Iteration starts element
 *                  at index zero
 *
 * @param[in, out]  ptr     Pointer that iterates array elements. Type should
 *                          be pointer to single array element
 * @param[in, out]  arr     Array of iterated elements. Most specify the array
 *                          itself, not pointer to array
 */
#ifndef DEO_ARR_FOREACH
#define DEO_ARR_FOREACH(ptr, arr) \
                for ((ptr) = (arr); (ptr) < ((arr) + ARR_SIZE(arr)); ++(ptr))
#endif /* DEO_ARR_FOREACH */

/************************************************************************/
/*                           DATA                                       */
/************************************************************************/

/** Array that holds the interrupts data */
volatile deo_interrupt_t g_deo_interrupt_array[DEO_INT_MAX_ISR_ATTACH] = {0};

/** Flag to indicates if deo_interrupt_init() was successfully called */
static boolean deo_interrupt_was_init = FALSE;

/** @brief mutex lock which is used in the interrupt module */
DEO_MUTEX_DECLARE(g_deo_int_mutex);

/************************************************************************/
/*                      STATIC FUNCTIONS                                */
/************************************************************************/
/**
 * @brief Detach interrupt element from a critical section protected code
 * @warn Must be called from a mutex locked sections
 * @param [in] interrupt_element the interrupt element to detach
 * @return #DEO_SUCCESS if successful otherwise #DEO_ERROR.
 */
static int32 deo_interrupt_detach_aux
(
    volatile deo_interrupt_t *interrupt_element
)
{
    uint32 int_source;

    if (NULL == interrupt_element)
    {
       DEO_DBG_E(DEO_INT, ("NULL interrupt element pointer\n"));
       return DEO_ERROR;
    }

    /* Save the interrupt source before setting it as invalid */
    int_source = interrupt_element->int_source;

    /* Disable the interrupt */
    VVDRV_deo_interrupt_disable(int_source);

    /* Remove interrupt data from the list, mark slot free
       Assuming the following write is atomic, otherwise TBD */
    interrupt_element->int_source = DEO_INT_INVALID_IRQ_NUM;

    /* Mark that was found */
    VVDRV_deo_interrupt_detach_os(int_source);

    return DEO_SUCCESS;
}

/************************************************************************/
/*                         FUNCTIONS                                    */
/************************************************************************/

int32 VVDRV_deo_interrupt_init(const deo_interrupt_controller_init_t *config)
{
    volatile deo_interrupt_t *interrupt_element = NULL;

    if (NULL == config)
    {
        DEO_DBG_E(DEO_INT, ("DEO interrupt init with NULL config pointer\n"));
        return DEO_ERROR;
    }

    if (TRUE == deo_interrupt_was_init)
    {
        return DEO_SUCCESS;
    }

    /* Initialize a mutex */
    if (DEO_SUCCESS != VVDRV_deo_mutex_init())
    {
        DEO_DBG_F(DEO_INT, ("DEO mutex initialization failure\n"));
        return DEO_ERROR;
    }

    if (DEO_SUCCESS != VVDRV_deo_mutex_create(&g_deo_int_mutex,
                                              DEO_MUTEX_ATTR_CRITICAL_SECTION))
    {
        DEO_DBG_F(DEO_INT, ("DEO mutex creation failure\n"));
        return DEO_ERROR;
    }

    /* Lock mutex to protect interrupt OS layer init and interrupt Queue */
    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&g_deo_int_mutex))
    {
        DEO_DBG_F(DEO_INT, ("DEO mutex lock failure\n"));
        return DEO_ERROR;
    }

    /* Need to recheck after lock */
    if (TRUE == deo_interrupt_was_init)
    {
        DEO_DBG_F(DEO_INT, ("DEO interrupt already initialized\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&g_deo_int_mutex))
        {
            DEO_DBG_E(DEO_INT, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Initialize the interrupt OS layer */
    if (DEO_SUCCESS != VVDRV_deo_interrupt_init_os(config))
    {
        DEO_DBG_F(DEO_INT, ("Could not initialize interrupt OS layer\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&g_deo_int_mutex))
        {
            DEO_DBG_E(DEO_INT, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Mark interrupt data array as empty */
    memset((void *)g_deo_interrupt_array, 0, sizeof(g_deo_interrupt_array));
    DEO_ARR_FOREACH(interrupt_element, g_deo_interrupt_array)
    {
        interrupt_element->int_source = DEO_INT_INVALID_IRQ_NUM;
    }

    deo_interrupt_was_init = TRUE;

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&g_deo_int_mutex))
    {
        DEO_DBG_F(DEO_INT, ("DEO mutex unlock failure\n"));
        return DEO_ERROR;
    }

    return DEO_SUCCESS;
}

void VVDRV_deo_interrupt_uninit(void)
{
    volatile deo_interrupt_t *interrupt_element = NULL;

    if (FALSE == deo_interrupt_was_init)
    {
        return; /* Not initialized, nothing to cleanup */
    }

    /* Detach all interrupts which are still attached */
    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&g_deo_int_mutex))
    {
        DEO_DBG_F(DEO_INT, ("DEO mutex lock failure\n"));
        return; /* Could not acquire lock */
    }

    DEO_ARR_FOREACH(interrupt_element, g_deo_interrupt_array)
    {
        if (DEO_INT_INVALID_IRQ_NUM != interrupt_element->int_source)
        {
           if (DEO_SUCCESS != deo_interrupt_detach_aux(interrupt_element))
           {
               DEO_DBG_E(DEO_INT, ("Failed detaching interrupt\n"));
           }
        }
    }

    deo_interrupt_was_init = FALSE;    /* Mark service as not initialized */

    /* Unlock mutex */
    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&g_deo_int_mutex))
    {
        DEO_DBG_E(DEO_INT, ("DEO mutex unlock failure\n"));
    }

    if (DEO_SUCCESS != VVDRV_deo_interrupt_uninit_os())
    {
        DEO_DBG_E(DEO_INT, ("DEO OS uninit failure\n"));
    }

    /* Destroy mutex */
    if (DEO_SUCCESS != VVDRV_deo_mutex_destroy(&g_deo_int_mutex))
    {
        DEO_DBG_E(DEO_INT, ("DEO mutex unlock failure\n"));
    }
}

int32 VVDRV_deo_interrupt_attach(const deo_interrupt_t *pInterrupt)
{
    volatile deo_interrupt_t *new_interrupt;
    volatile deo_interrupt_t *interrupt_element;

    if (FALSE == deo_interrupt_was_init)
    {
        DEO_DBG_E(DEO_INT, ("DEO interrupt not initialized\n"));
        return DEO_ERROR;
    }

    /* Check arguments */
    if ((NULL == pInterrupt) ||
        (NULL == pInterrupt->isr) ||
        (DEO_INT_INVALID_IRQ_NUM == pInterrupt->int_source))
    {
        DEO_DBG_E(DEO_INT, ("Invalid IRQ attach argument\n"));
        return DEO_ERROR;
    }

    /*  Disable the interrupt before attaching it */
    if (DEO_SUCCESS != VVDRV_deo_interrupt_disable_os(pInterrupt->int_source))
    {
        DEO_DBG_E(DEO_INT, ("Cannot disable interrupt source %u\n",
                            pInterrupt->int_source));
        return DEO_ERROR;
    }

    /* Scan the interrupt data:
     * Check that no previous attachment was made to same int_source
     * Find an empty slot for new interrupt data
     */
    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&g_deo_int_mutex))
    {
       DEO_DBG_F(DEO_INT, ("DEO mutex lock failure\n"));
       return DEO_ERROR;
    }

    new_interrupt = NULL;
    DEO_ARR_FOREACH(interrupt_element, g_deo_interrupt_array)
    {
        uint32 int_source = interrupt_element->int_source;
        if (int_source == pInterrupt->int_source)
        {
            new_interrupt = interrupt_element;
            DEO_DBG_W(DEO_INT,
                      ("Warning: trying to attach interrupt source that is "
                       "already attached - detaching the previous\n"));
            break;
        }
        else if ((NULL == new_interrupt) &&
                 (DEO_INT_INVALID_IRQ_NUM == int_source))
        {
            new_interrupt = interrupt_element;
        }
    }

    if (NULL == new_interrupt)
    {
        DEO_DBG_E(DEO_INT, ("Failed allocation of new interrupt data\n"));
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&g_deo_int_mutex))
        {
            DEO_DBG_E(DEO_INT, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }
    else if (new_interrupt->int_source == pInterrupt->int_source)
    {
        /* Need to remove the old registration */
        if (DEO_SUCCESS != deo_interrupt_detach_aux(new_interrupt))
        {
            DEO_DBG_E(DEO_INT, ("Failed detaching interrupt\n"));
            if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&g_deo_int_mutex))
            {
                DEO_DBG_E(DEO_INT, ("DEO mutex unlock failure\n"));
            }
            return DEO_ERROR;
        }
    }

    /* Now new_interrupt points to an available slot */

    /* Handle OS-specific operations */
    if (DEO_SUCCESS != VVDRV_deo_interrupt_attach_os(pInterrupt))
    {
        DEO_DBG_E(DEO_INT, ("Cannot attach interrupt source %u\n",
                            pInterrupt->int_source));
        new_interrupt = NULL;
        if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&g_deo_int_mutex))
        {
            DEO_DBG_E(DEO_INT, ("DEO mutex unlock failure\n"));
        }
        return DEO_ERROR;
    }

    /* Set the new interrupt data */
    new_interrupt->isr           = pInterrupt->isr;
    new_interrupt->user_data     = pInterrupt->user_data;
    new_interrupt->int_priority  = pInterrupt->int_priority;
    new_interrupt->int_trigger   = pInterrupt->int_trigger;
    new_interrupt->int_targets   = pInterrupt->int_targets;
    new_interrupt->int_sec_level = pInterrupt->int_sec_level;
    new_interrupt->int_source    = pInterrupt->int_source;

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&g_deo_int_mutex))
    {
        DEO_DBG_F(DEO_INT, ("DEO mutex unlock failure\n"));
        return DEO_ERROR;
    }

    return DEO_SUCCESS;
}

void VVDRV_deo_interrupt_detach(uint32 int_source)
{
    volatile deo_interrupt_t *interrupt_element;

    if (FALSE == deo_interrupt_was_init)
    {
        DEO_DBG_E(DEO_INT, ("DEO interrupt not initialized\n"));
        return;  /* Error */
    }

    /* Check arguments */
    if (DEO_INT_INVALID_IRQ_NUM == int_source)
    {
        DEO_DBG_E(DEO_INT, ("Invalid IRQ detach argument\n"));
        return;  /* Error */
    }

    /* Find the interrupt and free it */
    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&g_deo_int_mutex))
    {
       DEO_DBG_F(DEO_INT, ("DEO mutex lock failure\n"));
       return;
    }

    DEO_ARR_FOREACH(interrupt_element, g_deo_interrupt_array)
    {
        if (interrupt_element->int_source == int_source)
        {
            if (DEO_SUCCESS != deo_interrupt_detach_aux(interrupt_element))
            {
                DEO_DBG_E(DEO_INT, ("DEO interrupt detach failure\n"));
                break;
            }

            interrupt_element = NULL;
            break;
        }
    }

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&g_deo_int_mutex))
    {
        DEO_DBG_E(DEO_INT, ("DEO mutex unlock failure\n"));
    }
}

int32 VVDRV_deo_interrupt_enable(uint32 int_source)
{
    if (FALSE == deo_interrupt_was_init)
    {
        DEO_DBG_E(DEO_INT, ("DEO interrupt not initialized\n"));
        return DEO_ERROR;
    }

    return VVDRV_deo_interrupt_enable_os(int_source);
}

int32 VVDRV_deo_interrupt_enable_all(void)
{
    volatile deo_interrupt_t *interrupt_element;
    int32 status;

    if (FALSE == deo_interrupt_was_init)
    {
        DEO_DBG_E(DEO_INT, ("DEO interrupt not initialized\n"));
        return DEO_ERROR;
    }

    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&g_deo_int_mutex))
    {
       DEO_DBG_F(DEO_INT, ("DEO mutex lock failure\n"));
       return DEO_ERROR;
    }

    status = DEO_SUCCESS;  /* If list is empty, operation successful */
    DEO_ARR_FOREACH(interrupt_element, g_deo_interrupt_array)
    {
        uint32 int_source = interrupt_element->int_source;
        if (DEO_INT_INVALID_IRQ_NUM != int_source)
        {
            /* Enable the interrupt */
            status = VVDRV_deo_interrupt_enable_os(int_source);
            if (DEO_SUCCESS != status)
            {
                DEO_DBG_E(DEO_INT, ("Error enabling interrupt source %u\n",
                                    int_source));
                /* Continue and try to enable the rest */
            }
        }
    }

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&g_deo_int_mutex))
    {
       DEO_DBG_F(DEO_INT, ("DEO mutex lock failure\n"));
       return DEO_ERROR;
    }

    return status;
}

void VVDRV_deo_interrupt_disable(uint32 int_source)
{
    if (FALSE == deo_interrupt_was_init)
    {
        DEO_DBG_E(DEO_INT, ("DEO interrupt not initialized\n"));
        return; /* Error */
    }

    if (DEO_SUCCESS != VVDRV_deo_interrupt_disable_os(int_source))
    {
        DEO_DBG_E(DEO_INT, ("Error disabling interrupt source %u\n",
                            int_source));
        return; /* Error */
    }

    return;     /* Success */
}

void VVDRV_deo_interrupt_disable_all(void)
{
    volatile deo_interrupt_t *interrupt_element;
    int32 status;

    if (FALSE == deo_interrupt_was_init)
    {
        DEO_DBG_E(DEO_INT, ("DEO interrupt not initialized\n"));
        return;  /* Error */
    }

    if (DEO_SUCCESS != VVDRV_deo_mutex_lock(&g_deo_int_mutex))
    {
       DEO_DBG_E(DEO_INT, ("DEO mutex lock failure\n"));
       return;  /* Error */
    }

    status = DEO_SUCCESS;  /* If list is empty, operation successful */
    DEO_ARR_FOREACH(interrupt_element, g_deo_interrupt_array)
    {
        uint32 int_source = interrupt_element->int_source;
        if (DEO_INT_INVALID_IRQ_NUM != int_source)
        {
            /* Disable the interrupt */
            status = VVDRV_deo_interrupt_disable_os(int_source);
            if (DEO_SUCCESS != status)
            {
                DEO_DBG_E(DEO_INT, ("Error disabling interrupt source %u\n",
                                    int_source));
                /* Continue and try to disable the rest */
            }
        }
    }

    if (DEO_SUCCESS != VVDRV_deo_mutex_unlock(&g_deo_int_mutex))
    {
        DEO_DBG_E(DEO_INT, ("DEO mutex unlock failure\n"));
        return;
    }

    /* Success */
}

void VVDRV_deo_interrupt_clear(uint32 int_source)
{
    if (FALSE == deo_interrupt_was_init)
    {
        DEO_DBG_E(DEO_INT, ("DEO interrupt not initialized\n"));
        return;     /* Error */
    }

    VVDRV_deo_interrupt_clear_os(int_source);
    /* Success*/
}

