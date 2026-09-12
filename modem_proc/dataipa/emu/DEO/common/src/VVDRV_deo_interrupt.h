#ifndef __VVDRV_DEO_INTERRUPT_H__
#define __VVDRV_DEO_INTERRUPT_H__
/**---------------------------------------------------------------------------
 * @file  VVDRV_deo_interrupt.h
 *
 * @brief declares the interface to DEO interrupt services
 *
 *  This API provides interfaces to register (attach), unregister (detach), 
 *  enable and disable interrupts.
 *  Each interrupt source can have only one ISR, but a single ISR can be
 *  attached to multiple interrupt sources
 *
 * Copyright (c) 2011-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/************************************************************************/
/*                           INCLUDES                                   */
/************************************************************************/

#include "VVDRV_deo_common.h"    /* DEO_SUCCESS, DEO_ERROR, uint32 and more  */

/************************************************************************/
/*                         MACROS & DEFINITIONS                         */
/************************************************************************/

/**
 * @brief   Maximal number of ISRs that can be attached simultaneously
 */
#ifndef DEO_INT_MAX_ISR_ATTACH
#if defined(DEO_DRIVER_CONCURRENT_EXECUTION)
#define DEO_INT_MAX_ISR_ATTACH  64
#else /* DEO_DRIVER_CONCURRENT_EXECUTION */
#define DEO_INT_MAX_ISR_ATTACH  20
#endif /* DEO_DRIVER_CONCURRENT_EXECUTION */
#endif /* DEO_INT_MAX_ISR_ATTACH */

/**
 * @brief   Used as invalid interrupt source number - must not match any
 *          valid interrupt line number on any interrupt controller
 */
#define DEO_INT_INVALID_IRQ_NUM (0xFFFFFFFFU)

/**
 * @brief   Used when targeting a specific CPU core is not supported by the 
 *          interrupt controllers. For example, NVIC and L6VIC don't support
 *          targeting CPU cores and this MACRO should be for NVIC and L6VIC.
 */
#define DEO_INT_TARGETS_NOT_APPLICABLE (0x0U)

/** 
 * @brief   Used for targeting only the current CPU core when the 
 *          interrupt controller supports targeting a specific CPU core.
 *          This define can be used with combination of other defines such as
 *          #DEO_INT_TARGETS_CPU_ID() by using bitwise or "|" operation.
 *          For NVIC/L2VIC (doesn't support targeting cpu), this will behave
 *          same as #DEO_INT_TARGETS_NOT_APPLICABLE.
 *          For QGIC (support targeting cpu) this will target the current cpu
 *          core and using it will be same as using #DEO_INT_TARGETS_CPU_ID() 
 *          with current cpu id.
 *           
 */
#define DEO_INT_TARGETS_SELF_CPU (0x20000000U)

/** 
 * @brief   Used for targeting a specific CPU core when the 
 *          interrupt controller supports targeting a specific CPU core.
 *          This define can be used with combination of other defines such as
 *          #DEO_INT_TARGETS_SELF_CPU by using bitwise or "|"  operation.
 *          If the target core is the current core #DEO_INT_TARGETS_SELF_CPU 
 *          may be used instead.
 *          For example, QGIC supports targeting CPU cores and this define may
 *          be used for QGIC. 
 */
#define DEO_INT_TARGETS_CPU_ID(x) (1 << x)

/** @brief   Used for trigger method - when IRQ trigger is level  */
#define DEO_INT_TRIGGER_LEVEL (0x0U)

/** @brief   Used for trigger method - when IRQ trigger is edge */
#define DEO_INT_TRIGGER_EDGE (0x1U)

/**
 * @brief   Used when secure level is not applicable for the interrupt 
 *          controller. This MACRO must be used for NVIC and L6VIC.
 */
#define DEO_INT_SECURE_LEVEL_NOT_APPLICABLE (0x0U)

/** 
 * @brief   Used for secure level - non secured interrupt for supported 
 *          interrupt controllers, 
 *          This MACRO can be used for QGIC non secure interrupt.
 */
#define DEO_INT_SECURE_LEVEL_NON_SECURE (0x1U)

/** @brief   Used for secure level - secured interrupt */
#define DEO_INT_SECURE_LEVEL_SECURE (0x2U)

/** @brief   Used for secure level - secure group 0 */
#define DEO_INT_SECURE_LEVEL_SECURE_GROUP0 (0x3U)

/************************************************************************/
/*                         TYPEDEFS                                     */
/************************************************************************/

/**
 * @brief   This struct contains data for initializing the underlying interrupt
 *          controller. It is used as parameter to two function: 
 *          #VVDRV_deo_init() and #VVDRV_deo_interrupt_init() and in both 
 *          VI-build and GE versions of DEO.
 */
typedef struct deo_interrupt_controller_init_s
{
    /** @brief  Virtual base-address of the interrupt controller
     *          This is required only when running on core emulation platform
     */
    void    *base_address;
    
} deo_interrupt_controller_init_t;

/**
 * @brief   Defines an alias representing the DEO interrupt priority.
 */
typedef uint32 deo_interrupt_priority_t;

/**
 * @brief   Defines an alias representing the targets CPU cores for the 
 *          interrupt. See #DEO_INT_TARGETS_NOT_APPLICABLE,
 *          #DEO_INT_TARGETS_SELF_CPU and #DEO_INT_TARGETS_CPU_ID() for 
 *          further details.
 */
typedef uint32 deo_interrupt_targets_t;

/**
 * @brief   Defines an alias representing the DEO interrupt trigger.
 *          See #DEO_INT_TRIGGER_LEVEL and #DEO_INT_TRIGGER_EDGE for further
 *          details.
 */
typedef uint8 deo_interrupt_trigger_t;

/**
 * @brief   Defines an alias representing the DEO interrupt secure level.
 *          See #DEO_INT_SECURE_LEVEL_NOT_APPLICABLE, 
 *          #DEO_INT_SECURE_LEVEL_NON_SECURE, #DEO_INT_SECURE_LEVEL_SECURE
 *          and #DEO_INT_SECURE_LEVEL_SECURE_GROUP0 for further details. 
 */
typedef uint8 deo_interrupt_secure_t;

/**
 * @brief   Defines an alias representing the prototype of the interrupt 
 *          service routine.
 */
typedef void (*deo_isr)(uint32 int_source, void *user_data);

/**
 * @brief   DEO interrupt configuration structure
 */
typedef struct deo_interrupt_s
{
    /** @brief Interrupt service routine */
    deo_isr isr;
    
    /** @brief Argument passed to ISR */
    void *user_data;
    
    /** Interrupt source number */
    uint32 int_source;

    /** 
     * @brief Interrupt priority.
     *        See #deo_interrupt_priority_t for further details.
     */
    deo_interrupt_priority_t int_priority;

    /** 
     * @brief Interrupt target CPU cores.
     *        See #deo_interrupt_targets_t for further details.
     */
    deo_interrupt_targets_t int_targets;

    /** 
     * @brief Interrupt trigger method.
     *        See #deo_interrupt_trigger_t for further details.
     */
    deo_interrupt_trigger_t int_trigger;
        
    /** 
     * @brief Interrupt secure level.
     *        See #deo_interrupt_secure_t for further details.
     */
    deo_interrupt_secure_t int_sec_level; 

}  deo_interrupt_t;

/************************************************************************/
/*                         FUNCTIONS                                    */
/************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief       Initializes the DEO interrupts service
 *
 * @note        The function is called by #VVDRV_deo_init()
 * @note        The function should be called only once in a program
 *
 * @param[in]   config          Interrupt controller configuration.
 *                              See #VVDRV_deo_interrupt_controller_init_t for
 *                              further details.
 *
 * @return      #DEO_SUCCESS if successful otherwise #DEO_ERROR 
 */
int32 VVDRV_deo_interrupt_init(const deo_interrupt_controller_init_t *config);

/**
 * @brief       Clean-up and free resources of of DEO interrupt services
 */
void VVDRV_deo_interrupt_uninit(void);

/**
 * @brief       Attach an interrupt: register an ISR and configure the 
 *              interrupt according to the passed parameters.
 *              Each interrupt source can have only one interrupt structure
 *              so if a previous subscription for the same interrupt source 
 *              exists it will detached and the new ISR will be attached
 *
 * @note        After attaching a new ISR the interrupt is disabled
 * @note        #VVDRV_deo_interrupt_init() should be called prior to calling 
 *              this API
 *
 * @param[in]   pInterrupt   pointer to the interrupt attributes 
 *
 * @return      #DEO_SUCCESS if successful otherwise #DEO_ERROR 
 */
int32 VVDRV_deo_interrupt_attach(const deo_interrupt_t *pInterrupt);

/**
 * @brief       Detach an interrupt: disable the interrupt and remove the ISR
 *              from the interrupt source structure
 *
 * @note        #VVDRV_deo_interrupt_init() should be called prior to calling 
 *              this API
 *
 * @param[in]   int_source  The interrupt source
 */
void VVDRV_deo_interrupt_detach(uint32 int_source);

/**
 * @brief       Enable an interrupt source
 *
 * @note        #VVDRV_deo_interrupt_init() should be called prior to calling 
 *              this API
 *
 * @param[in]   int_source  The interrupt source
 *
 * @return      #DEO_SUCCESS if successful otherwise #DEO_ERROR  
 */
int32 VVDRV_deo_interrupt_enable(uint32 int_source);

/**
 * @brief       Enable all previously attached interrupt sources
 *
 * @note        #VVDRV_deo_interrupt_init() should be called prior to calling 
 *              this API
 *
 * @return      #DEO_SUCCESS if successful otherwise #DEO_ERROR
 */
int32 VVDRV_deo_interrupt_enable_all(void);

/**
 * @brief       Disable an interrupt source
 *
 * @note        #VVDRV_deo_interrupt_init() should be called prior to calling 
 *              this API
 *
 * @param[in]   int_source  The interrupt source
 */
void VVDRV_deo_interrupt_disable(uint32 int_source);

/**
 * @brief       Disable all previously attached interrupt sources
 *
 * @note        #VVDRV_deo_interrupt_init() should be called prior to calling 
 *              this API
 */
void VVDRV_deo_interrupt_disable_all(void);

/**
 * @brief       Clear an interrupt source
 *
 * @note        #VVDRV_deo_interrupt_init() should be called prior to calling 
 *              this API.
 *
 * @param[in]   int_source  The interrupt source
 */
void VVDRV_deo_interrupt_clear(uint32 int_source);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __VVDRV_DEO_INTERRUPT_H_ */
