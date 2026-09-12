#ifndef __VVDRV_DEO_INTERRUPT_CONTROLLER_COPSS_H__
#define __VVDRV_DEO_INTERRUPT_CONTROLLER_COPSS_H__
/**---------------------------------------------------------------------------
 * @file  deo_interrupt_controller_copss.h
 *
 * @brief Declares the DEO interrupt controller interface
 *
 * Copyright (c) 2016-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/************************************************************************/
/*                            INCLUDES                                  */
/************************************************************************/

#include "VVDRV_deo_interrupt.h"          /* For deo_interrupt_t */

/************************************************************************/
/*                        TYPEDEFS & STRUCTS                            */
/************************************************************************/

/** @brief DEO interrupt controller driver function table */
typedef struct DEO_INT_CTRL_STRUCT
{
    /** @brief See #deo_interrupt_controller_init() */
    int32 (*init)(const deo_interrupt_controller_init_t *cfg);

    /** @brief See #deo_interrupt_controller_configure() */
    int32 (*configure)(const deo_interrupt_t *pInterrupt);

    /** @brief See #deo_interrupt_controller_enable() */
    int32 (*enable)(uint32 intNum);

    /** @brief See #deo_interrupt_controller_disable() */
    int32 (*disable)(uint32 intNum);

    /** @brief See #deo_interrupt_controller_clear() */
    int32 (*clear)(uint32 intNum);

    /** @brief See #deo_interrupt_controller_get_interrupt_source_status() */
    boolean (*get_source_status)(uint32 intNum);

    /** @brief See #deo_interrupt_controller_set_pending() */
    int32 (*set_pending)(const deo_interrupt_t *pInt, uint32 intCnt);
} DEO_INT_CTRL;

/************************************************************************/
/*                              FUNCTIONS                               */
/************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief           Load the function table of this interrupt controller
 *
 * @param[in, out]  funcTable   Pointer to function table to be filled
 *
 * @return          DEO_SUCCESS if successful otherwise DEO_ERROR
 */
int32 VVDRV_deo_interrupt_controller_load_copss(DEO_INT_CTRL *funcTable);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __VVDRV_DEO_INTERRUPT_CONTROLLER_COPSS_H__ */
