/**---------------------------------------------------------------------------
 * @file  VVDRV_deo_interrupt_os.c
 *
 * @brief implements the vi build specific interrupt functions
 *
 * Copyright (c) 2011-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/************************************************************************/
/*                           INCLUDES                                   */
/************************************************************************/

#include "VVDRV_deo_common.h"        /**< DEO_SUCCESS and DEO_FAIL            */
#include "VVDRV_deo_interrupt_os.h"  /**< API declarations                    */

/* DEO_IC_QGIC (APPS IC) */
#if defined (COMPILING_APPS_PROC)
#ifndef DEO_IC_QGIC
#define DEO_IC_QGIC
#endif /* DEO_IC_QGIC */
#endif /* COMPILING_APPS_PROC */

/* DEO_IC_NVIC (RPM IC) */
#if defined (COMPILING_RPM_PROC)
#ifndef DEO_IC_NVIC
#define DEO_IC_NVIC
#endif /* DEO_IC_NVIC */
#endif /* COMPILING_RPM_PROC */

/* DEO_IC_L2VIC (QDSP6 IC)
 * If you get warning "No definition of interrupt controller." its possibly
 * because you are compiling on Q6 that is not defined here.
 */
#if (defined (COMPILING_Q6SS_PROC)  || \
     defined (COMPILING_LPASS_PROC) || \
     defined (COMPILING_MSS_PROC))
#ifndef DEO_IC_L2VIC
#define DEO_IC_L2VIC
#endif /* DEO_IC_L2VIC */
#endif /* COMPILING_Q6SS_PROC || COMPILING_LPASS_PROC || COMPILING_MSS_PROC */

/* DEO_IC_GE (Generic Emulation IC)
 * Generic Emulation on Windows specified by WIN32 compile flag
 */
#if defined(WIN32)
#error "\nTarget file compiled on windows - use windows version of this file\n"
#endif /* WIN32 */

/* Check that at least one IC is defined
 * DEO_IC_NVIC, DEO_IC_L2VIC, DEO_IC_QGIC, DEO_IC_GE
 */
#if (!(defined(DEO_IC_QGIC) || defined(DEO_IC_NVIC) || defined(DEO_IC_L2VIC)))
#error "\nNo definition of interrupt controller\n"
#endif /* !(DEO_IC_NVIC || DEO_IC_QGIC || DEO_IC_L2VIC) */

/*
 * Check that no more than one IC is defined.
 * DEO_IC_NVIC, DEO_IC_L2VIC, DEO_IC_QGIC, DEO_IC_GE
 */
#if ((defined(DEO_IC_NVIC)  && defined(DEO_IC_QGIC) ) || \
     (defined(DEO_IC_L2VIC) && defined(DEO_IC_QGIC) ) || \
     (defined(DEO_IC_NVIC)  && defined(DEO_IC_L2VIC)))
#error "\nMultiple interrupts controller definitions\n"
#endif /* Multiple IC definitions */

/* Now get the needed IC driver header file */
#if defined(DEO_IC_QGIC)
#include "VVDRV_qgic.h"
#elif defined(DEO_IC_NVIC)
#include "VVDRV_nvic.h"
#elif defined(DEO_IC_L2VIC)
#include "VVDRV_qdsp6_Init.h"
#include "VVDRV_qdsp6_IntHandler.h"
#include "VVDRV_qdsp6_L2vicInt.h"
#endif /* DEO_IC_QGIC */

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

/** @brief The number of bits in uint32 */
#define DEO_UINT32_NUM_BITS (32)

/************************************************************************/
/*                            EXTERNS                                   */
/************************************************************************/

/* Contains interrupt data */
extern deo_interrupt_t g_deo_interrupt_array[DEO_INT_MAX_ISR_ATTACH];

/************************************************************************/
/*                         FUNCTIONS                                    */
/************************************************************************/

/***************************************************************************/
/* Common Functions                                                        */
/*                                                                         */
/* This section of the code contains functions that are common to both     */
/* QGIC, NVIC and L2VIC                                                    */
/***************************************************************************/

void VVDRV_deo_interrupt_detach_os(uint32 int_source)
{
    (void)int_source;   /* Unused */
    /* Do nothing, as no support provided by underlying drivers */
}

/**
 * @brief       Search for ISR according to an interrupt line number, 
 *              if found the ISR will be called
 *
 * @param[in]   eInterrupt     Interrupt line number
 */
static void deo_interrupt_isr_wrapper_common(uint32 eInterrupt)
{
    deo_interrupt_t *interrupt_element = NULL;

    /* ISR can't use mutex, because this could result with a deadlock if 
       interrupt is triggered while attaching ISR */

    DEO_ARR_FOREACH(interrupt_element, g_deo_interrupt_array)
    {
        /* The "interrupt" struct pointer and ISR function pointer are
         * not NULL after interrupt registration so no further checks performed
         */
        if (interrupt_element->int_source == eInterrupt)
        {
            interrupt_element->isr(eInterrupt, interrupt_element->user_data);
            break;
        }
    }
}

/***************************************************************************/
/* QGIC                                                                    */
/*                                                                         */
/* This section of the code contains QGIC-related functions                */
/***************************************************************************/

#ifdef DEO_IC_QGIC

/**
 * @brief Convert secure level from from DEO defintions to GIC defintions
 *
 * @param[in] sec_level - deo security level @see deo_interrupt_secure_t
 *
 * @return the matching GIC secure type
 */
static HAL_qgic_SecureType deo_get_gic_secure_type
(
   deo_interrupt_secure_t sec_level
)
{
    if (DEO_INT_SECURE_LEVEL_NON_SECURE == sec_level)
    {
        return HAL_QGIC_NON_SECURE;
    }
    
    return HAL_QGIC_SECURE;
}

/**
 * @brief Convert trigger type from from DEO defintions to GIC defintions
 *
 * @param[in] trigger - deo trigger type see #deo_interrupt_trigger_t
 *
 * @return the matching GIC targets
 */
static HAL_qgic_TriggerType deo_get_gic_trigger(deo_interrupt_trigger_t trigger)
{
    return (trigger == DEO_INT_TRIGGER_EDGE) ? 
           HAL_QGIC_TRIGGER_EDGE : HAL_QGIC_TRIGGER_LEVEL;
}

/**
 * @brief Convert targets from from DEO defintions to GIC defintions
 *
 * @param[in] targets - deo targets see #deo_interrupt_targets_t
 *
 * @return the matching GIC targets
 */
static uint32 deo_get_gic_targets(deo_interrupt_targets_t targets)
{
    uint32 cpu_targets = 0x0;
    uint32 i = 0x0;
    uint32 gic_targets = 0x0;

    if (0x0 != (DEO_INT_TARGETS_SELF_CPU & targets))
    {
        gic_targets |= HAL_QGIC_TARGET_CPU(0);
    }

    /* Clearing attribute bits for the CPU ids scanning */
    cpu_targets = targets & ~(DEO_INT_TARGETS_SELF_CPU);

    for (i = 0; i < DEO_UINT32_NUM_BITS; ++i)
    {
        if (0x0 != (cpu_targets & (0x1 << i)))
        {
            gic_targets |= HAL_QGIC_TARGET_CPU(i);
        }
    }

    return gic_targets;
}

static void deo_interrupt_isr_wrapper(HAL_qgic_InterruptType eInterrupt)
{
    /* Check "Active" and "Pending" interrupt status.
     * The QGIC driver handles both "Active" and "Pending" states.
     * - Both states might be set before this driver ISR is called:
     *   - In some cases "Active" is set but not "Pending", e.g. this might
     *     happen for edge interrupt or SGI type interrupts.
     *   - In some cases "Pending" is set but not "Active".
     *   Therefore, both states should be checked.
     * - "Active" state is cleared by the QGIC driver.
     * - "Pending" state is cleared by this driver.
     * 
     * For further details the following document:
     * "Qualcomm Generic Interrupt Controller 2 HDD" 80-VK784-1, Rev. C
     *  January 10, 2013, section 1.2.3
     * http://qctweb1.qualcomm.com/prj/qct/coredev/intctrl/rtp/docs/qgic2/gicv2m/qgic2_HDD.pdf
     */
    if ((TRUE == VVDRV_qgic_isPending(eInterrupt)) ||
        (TRUE == VVDRV_qgic_isActive(eInterrupt))  )
    {
        /* Find and call relevant ISR */
        deo_interrupt_isr_wrapper_common(eInterrupt);
    }
}

int32 VVDRV_deo_interrupt_attach_os(const deo_interrupt_t *pInterrupt)
{
    VVDRV_qgic_intInfo sInt_info = { 0x0 }; 
     
    /* Interrupt target cpu core and secure level are not required by QGIC, 
       NOT_APPLICABLE is is not valid input and not allowed */
    if ((NULL == pInterrupt) ||
        (DEO_INT_TARGETS_NOT_APPLICABLE == pInterrupt->int_targets) ||
        (DEO_INT_SECURE_LEVEL_NOT_APPLICABLE == pInterrupt->int_sec_level))
    {
        return DEO_ERROR;
    }
    
    sInt_info.eInterrupt = (HAL_qgic_InterruptType)pInterrupt->int_source;
    sInt_info.nPriority  = (HAL_qgic_PriorityType)pInterrupt->int_priority;
    sInt_info.eSecure    = deo_get_gic_secure_type(pInterrupt->int_sec_level);
    sInt_info.eTrigger   = deo_get_gic_trigger(pInterrupt->int_trigger);
    sInt_info.nTargets   = deo_get_gic_targets(pInterrupt->int_targets);
    sInt_info.pfnIsr     = deo_interrupt_isr_wrapper;

    VVDRV_qgic_installIsr(&sInt_info);

    return DEO_SUCCESS;
}

int32 VVDRV_deo_interrupt_enable_os(uint32 int_source)
{
    if (FALSE == VVDRV_qgic_isEnabled(int_source))
    {
        VVDRV_qgic_enable(int_source);
    }

    return DEO_SUCCESS;
}

int32 VVDRV_deo_interrupt_disable_os(uint32 int_source)
{
    if (TRUE == VVDRV_qgic_isEnabled(int_source))
    {
        VVDRV_qgic_disable(int_source);
    }

    return DEO_SUCCESS;
}

void VVDRV_deo_interrupt_clear_os(uint32 int_source)
{
    VVDRV_qgic_clearPending(int_source);
}

int32 VVDRV_deo_interrupt_init_os(const deo_interrupt_controller_init_t *config)
{   
    char *qgicVer = NULL;
    
    (void)config; /* Unused */

    VVDRV_qgic_init(&qgicVer);  /* QGIC version not needed */

    return DEO_SUCCESS;
}

/***************************************************************************/
/* NVIC                                                                    */
/*                                                                         */
/* This section of the code contains NVIC-related functions                */
/***************************************************************************/

#elif defined(DEO_IC_NVIC)

static void deo_interrupt_isr_wrapper(void)
{
    /* Find the active IRQ number */
    uint32 eInterrupt = VVDRV_nvic_GetActiveNumber();

    /* Find and call relevant ISR */
    deo_interrupt_isr_wrapper_common(eInterrupt);
}

int32 VVDRV_deo_interrupt_attach_os(const deo_interrupt_t *pInterrupt)
{
    VVDRV_nvic_IntInfo nvicInfo = { 0x0 };

    /* Interrupt target cpu core and secure level are not supported by NVIC, 
       NOT_APPLICABLE is the only settings which are allowed */
    if ((NULL == pInterrupt) ||
        ((DEO_INT_TARGETS_NOT_APPLICABLE != pInterrupt->int_targets) &&
         (DEO_INT_TARGETS_SELF_CPU != pInterrupt->int_targets)) ||
        (DEO_INT_SECURE_LEVEL_NOT_APPLICABLE != pInterrupt->int_sec_level))
    {
        return DEO_ERROR;
    }
        
    nvicInfo.eInterrupt = (VVDRV_nvic_InterruptType)pInterrupt->int_source;
    nvicInfo.eTrigger   = (pInterrupt->int_trigger == DEO_INT_TRIGGER_EDGE) ? 
                              NVIC_EDGE : NVIC_LEVEL;
    nvicInfo.ePolarity  = NVIC_HIGH;
    nvicInfo.nPriority  = (VVDRV_nvic_PriorityType)pInterrupt->int_priority;
    nvicInfo.pfnIsr     = deo_interrupt_isr_wrapper;

    VVDRV_nvic_InstallIsr(&nvicInfo);

    return DEO_SUCCESS;
}

int32 VVDRV_deo_interrupt_enable_os(uint32 int_source)
{
    if (FALSE == VVDRV_nvic_IsEnable(int_source))
    {
        VVDRV_nvic_Enable(int_source);
    }

    return DEO_SUCCESS;
}

int32 VVDRV_deo_interrupt_disable_os(uint32 int_source)
{
    if (TRUE == VVDRV_nvic_IsEnable(int_source) )
    {
        VVDRV_nvic_Disable(int_source);
    }

    return DEO_SUCCESS;
}

void VVDRV_deo_interrupt_clear_os(uint32 int_source)
{
    VVDRV_nvic_ClearPending(int_source);
}

int32 VVDRV_deo_interrupt_init_os(const deo_interrupt_controller_init_t *config)
{
    (void)config; /* Unused */
    
    VVDRV_nvic_Init();
    
    return DEO_SUCCESS;
}

/***************************************************************************/
/* L2VIC                                                                   */
/*                                                                         */
/* This section of the code contains L2VIC-related functions               */
/***************************************************************************/

#elif defined(DEO_IC_L2VIC)

static void deo_interrupt_isr_wrapper(uint32 eInterrupt)
{
    /* Find and call relevant ISR */
    deo_interrupt_isr_wrapper_common(eInterrupt);
}

int32 VVDRV_deo_interrupt_attach_os(const deo_interrupt_t *pInterrupt)
{
    VVDRV_qdsp6_q6l2vicTriggerType trigger;
    
    /* Interrupt target cpu core and secure level are not supported by NVIC, 
       NOT_APPLICABLE is the only settings which are allowed */
    if ((NULL == pInterrupt) ||
        (0x0 < pInterrupt->int_priority) ||
        ((DEO_INT_TARGETS_NOT_APPLICABLE != pInterrupt->int_targets) &&
        (DEO_INT_TARGETS_SELF_CPU != pInterrupt->int_targets)) ||
        (DEO_INT_SECURE_LEVEL_NOT_APPLICABLE != pInterrupt->int_sec_level))
    {
        return DEO_ERROR;
    }

    trigger = (pInterrupt->int_trigger == DEO_INT_TRIGGER_EDGE) ? 
                  Q6L2VIC_TRIGGER_EDGE : Q6L2VIC_TRIGGER_LEVEL;
    
    if (TRUE != VVDRV_qdsp6_q6l2vicConfigInt(pInterrupt->int_source, trigger))
    {
        return DEO_ERROR;
    }

    if (TRUE != VVDRV_qdsp6_q6l2vicInstallHandler(pInterrupt->int_source,
                                                  deo_interrupt_isr_wrapper))
    {
        return DEO_ERROR;
    }

    return DEO_SUCCESS;
}

int32 VVDRV_deo_interrupt_enable_os(uint32 int_source)
{
    VVDRV_qdsp6_q6l2vicIntEnable(int_source);

    return DEO_SUCCESS;
}

int32 VVDRV_deo_interrupt_disable_os(uint32 int_source)
{
    VVDRV_qdsp6_q6l2vicIntDisable(int_source);

    return DEO_SUCCESS;
}

void VVDRV_deo_interrupt_clear_os(uint32 int_source)
{
    VVDRV_qdsp6_q6l2vicIntClear(int_source);
}

int32 VVDRV_deo_interrupt_init_os(const deo_interrupt_controller_init_t *config)
{
    (void)config;       /* Unused */

    return DEO_SUCCESS;
}

#endif /* DEO_IC_NVIC | DEO_IC_QGIC | DEO_IC_L2VIC */
