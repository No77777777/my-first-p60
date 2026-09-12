#ifndef __VVDRV_DEO_INIT_H__
#define __VVDRV_DEO_INIT_H__
/**-----------------------------------------------------------------------------
 * @file  VVDRV_deo_init.h
 *
 * @brief Implements DEO common init APIs
 *
 * Copyright (c) 2010-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/

#include "VVDRV_deo_mempool.h"           /* deo_mempool_args_t */

/*------------------------------------------------------------------------------
 * Type Declarations
 *----------------------------------------------------------------------------*/
/**
 * @brief   DEO configuration structure
 */
typedef struct deo_cfg_s
{
    /**
     * Input parameter.
     * On Windows only, is set to TRUE, DEO will automatically initialize the 
     * DMA memory pool to PCIe DMA memory.
     * If set to FALSE DEO DMA pool will not be initialized (Windows and 
     * VI-BUILD).
     */
    boolean            init_dma_mempool;

    /**
     * Output parameter.
     * Windows only. If init_dma_mempool = TRUE, then it will hold the 
     * memory pool arguments filled automatically
     */
    deo_mempool_args_t mempool_cfg;

    /** 
     * Output parameter.
     * Windows only. Return RPB info for later use 
     */
    void*              pcie_cfg;

    /** 
     * Input parameter.
     * Arguments for SNOWBUSH translate registers 
     */
    void*              pcie_translate_cfg;

    /** 
     * Input parameter.
     * Arguments to initialize the interrupt controller 
     */
    void*              interrupt_controller_cfg;
} deo_cfg_t;

/*------------------------------------------------------------------------------
 * Function Definitions
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief   Initializes DEO services.
 *
 * The function initializes all DEO services, as specified by the input.
 *  1. OS layer: Refer to the relevant platform version of deo_os.h for more 
 *     information
 *  2. Memory pool service (mempool and DMA pool):Refer to deo_mempool.h 
 *     for more information.
 *  3. Interrupts service: Refer to deo_interrups.h for information.
 *  4. Time service: Refer to deo_time.h for information.
 *
 * @param [in,out]      cfg      If non-null, deo configuration data structure  
 *
 * @return      #DEO_SUCCESS if successful, otherwise failed
 */
int32 VVDRV_deo_init(deo_cfg_t *cfg);

/**
 * @brief   Un-initializes all DEO services that were initialized by 
 *          #VVDRV_deo_init()
 */
void VVDRV_deo_uninit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __VVDRV_DEO_INIT_H__ */
