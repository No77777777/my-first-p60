#ifndef __VVDRV_DEO_INTERRUPT_OS_H__
#define __VVDRV_DEO_INTERRUPT_OS_H__
/**---------------------------------------------------------------------------
 * @file    VVDRV_deo_interrupt_os.h
 *
 * @brief   DEO interrupt controller OS abstraction APIs. 
 *          These are internal APIs that should not be called directly by users
 *          and could change without any notice
 *
 * Copyright (c) 2010-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/******************************************************************************/
/*                              INCLUDES                                      */
/******************************************************************************/

#include "VVDRV_deo_common.h"    /* DEO_SUCCESS, DEO_ERROR, uint32 and more  */
#include "VVDRV_deo_interrupt.h" /* deo_interrupt_t                          */

/******************************************************************************/
/*                              FUNCTIONS                                     */
/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief       Initialize interrupt controller with base virtual address of
 *              the interrupt controller. 
 *
 * @param[in]   config          Interrupt controller configuration.
 *                              See #deo_interrupt_controller_init_t for
 *                              further details.
 *
 * @return      #DEO_SUCCESS if successful otherwise #DEO_ERROR.
 */
int32 VVDRV_deo_interrupt_init_os(const deo_interrupt_controller_init_t *config);

/**
 * @brief       Attach an interrupt to be handled by the interrupt controller
 *
 * @param[in]   pInterrupt      Interrupt data struct
 *
 * @return      #DEO_SUCCESS if successful otherwise #DEO_ERROR
 */
int32 VVDRV_deo_interrupt_attach_os(const deo_interrupt_t *pInterrupt);

/**
 * @brief       Detach an interrupt from the interrupt controller
 *
 * @param[in]   int_source      The interrupt source
 */
void VVDRV_deo_interrupt_detach_os(uint32 int_source);

/**
 * @brief       Enable an interrupt line at the controller level
 *
 * @param[in]   int_source      The interrupt source
 *
 * @return      #DEO_SUCCESS if successful otherwise #DEO_ERROR
 */
int32 VVDRV_deo_interrupt_enable_os(uint32 int_source);

/**
 * @brief       Disable an interrupt line at the controller level
 *      
 * @param[in]   int_source      The interrupt source
 *
 * @return      #DEO_SUCCESS if successful otherwise #DEO_ERROR
 */
int32 VVDRV_deo_interrupt_disable_os(uint32 int_source);

/**
 * @brief       Clear pending status for an interrupt line at the controller
 *              level
 *
 * @param[in]   int_source  The interrupt source.
 */
void VVDRV_deo_interrupt_clear_os(uint32 int_source);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __VVDRV_DEO_INTERRUPT_OS_H__ */
