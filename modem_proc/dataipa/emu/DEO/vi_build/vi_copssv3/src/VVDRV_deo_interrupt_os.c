/**---------------------------------------------------------------------------
 * @file  VVDRV_deo_interrupt_os.c
 *
 * @brief implements the vi build specific interrupt functions
 *
 * Copyright (c) 2016-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/************************************************************************/
/*                           INCLUDES                                   */
/************************************************************************/

#include "VVDRV_deo_common.h"        /**< DEO_SUCCESS and DEO_FAIL            */
#include "VVDRV_deo_interrupt_os.h"  /**< API declarations                    */

/* include files for the 2 underlying interrupt controllers */
#include "VVDRV_nvic.h"
#include "VVDRV_deo_interrupt_controller_copss.h"

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
/*                            EXTERNS                                   */
/************************************************************************/

/* Contains interrupt data */
extern deo_interrupt_t g_deo_interrupt_array[DEO_INT_MAX_ISR_ATTACH];

/************************************************************************/
/*                             DATA                                     */
/************************************************************************/

/**
 * @brief DEO COPSS interrupt controller function table
 */
static DEO_INT_CTRL deo_gIcFunc = {0};

/************************************************************************/
/*                         FUNCTIONS                                    */
/************************************************************************/

static void deo_interrupt_isr_wrapper(void)
{
    volatile deo_interrupt_t *interrupt_element; /* Interrupt element */

    /* Verify that the handler was not registered on other interrupt sources
     * of NVIC */
    if (VVDRV_nvic_GetActiveNumber() != COPSSV3_GE_WIC)
    {
        return;
    }
    
    /* Find the interrupts and call the callback functions */
    DEO_ARR_FOREACH(interrupt_element, g_deo_interrupt_array)
    {
        uint32 int_source = interrupt_element->int_source;
        if (DEO_INT_INVALID_IRQ_NUM == int_source)
        {
            continue;
        }

        if (TRUE == deo_gIcFunc.get_source_status(int_source))
        {
            /* Activate the callback function for current element */
            interrupt_element->isr(int_source, interrupt_element->user_data);
        }
    }
}

/*****************************************************************************/
void VVDRV_deo_interrupt_detach_os(uint32 int_source)
{
    (void)int_source;   /* Unused */
    /* Do nothing, as no support provided by underlying drivers */
}

/*****************************************************************************/
int32 VVDRV_deo_interrupt_attach_os(const deo_interrupt_t *pInterrupt)
{
    if ((NULL == pInterrupt) ||
        ((DEO_INT_TARGETS_NOT_APPLICABLE != pInterrupt->int_targets) &&
         (DEO_INT_TARGETS_SELF_CPU != pInterrupt->int_targets)) ||
        (DEO_INT_SECURE_LEVEL_NOT_APPLICABLE != pInterrupt->int_sec_level))
    {
        return DEO_ERROR;
    }
    
    return deo_gIcFunc.configure(pInterrupt);
}

/*****************************************************************************/
int32 VVDRV_deo_interrupt_enable_os(uint32 int_source)
{
    return deo_gIcFunc.enable(int_source);
}

/*****************************************************************************/
int32 VVDRV_deo_interrupt_disable_os(uint32 int_source)
{
    return deo_gIcFunc.disable(int_source);
}

/*****************************************************************************/
void VVDRV_deo_interrupt_clear_os(uint32 int_source)
{
    (void)deo_gIcFunc.clear(int_source);
}

/*****************************************************************************/
int32 VVDRV_deo_interrupt_init_os(const deo_interrupt_controller_init_t *config)
{   
    VVDRV_nvic_IntInfo nvicInfo = {0};
    deo_interrupt_controller_init_t newConfig = {0};
    
    (void)config; /* Unused */
    
    /* Generate a new config so as to have a non-const copy of 'config'.
     * By convention, user does not configure the interrupt controller base
     * address, so it should be done either in this layer or lower. The 
     * lower layer is a reused copy of a file from the Windows DEO branch
     * which I'd like to avoid changing. */
    newConfig.base_address = (void *)0xF81C0000;
    
    /* First, initialize the GE Interrupt Controller */
    if (DEO_SUCCESS != VVDRV_deo_interrupt_controller_load_copss(&deo_gIcFunc))
    {
        return DEO_ERROR;
    }
    if (DEO_SUCCESS != deo_gIcFunc.init(&newConfig))
    {
        return DEO_ERROR;
    }
    
    /* Then, initialize the CM3 NVIC */
    VVDRV_nvic_Init();
    
    /* Now install DEO's wrapper as the ISR for GE interrupts in NVIC */
    nvicInfo.eInterrupt = COPSSV3_GE_WIC;
    nvicInfo.eTrigger   = NVIC_LEVEL;
    nvicInfo.ePolarity  = NVIC_HIGH;
    nvicInfo.nPriority  = 0;
    nvicInfo.pfnIsr     = deo_interrupt_isr_wrapper;

    VVDRV_nvic_InstallIsr(&nvicInfo);

    return DEO_SUCCESS;
}

