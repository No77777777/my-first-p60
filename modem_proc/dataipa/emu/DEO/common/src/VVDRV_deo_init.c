/**---------------------------------------------------------------------------
 * @file   VVDRV_deo_init.c
 *
 * @brief Implements common init/un-init for DEO services
 *
 * Copyright (c) 2011-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/************************************************************************/
/*                           INCLUDES                                   */
/************************************************************************/

#include "VVDRV_deo_debug.h"          /* DEO debug definitions                */
#include "VVDRV_deo_interrupt.h"      /* DEO interrupt handeling API          */
#include "VVDRV_deo_os.h"             /* deo_init_os and deo_uninit_os        */
#include "VVDRV_deo_init.h"           /* DEO init API                         */
#include "VVDRV_deo_common.h"         /* DEO_SUCCESS and DEO_ERROR            */
#include "VVDRV_deo_time.h"           /* DEO timer service                    */
#include "VVDRV_deo_mutex.h"          /* DEO mutex service                    */

/************************************************************************/
/*                         FUNCTIONS                                    */
/************************************************************************/

int32 VVDRV_deo_init(deo_cfg_t *cfg)
{
    deo_interrupt_controller_init_t *ic_cfg = NULL;

    if (NULL == cfg)
    {
        DEO_DBG_E(DEO_INIT, ("NULL configuration pointer\n"));
        return DEO_ERROR;
    }

    /* Initalize the DEO mutex service */
    if (DEO_SUCCESS != VVDRV_deo_mutex_init())
    {
        DEO_DBG_F(DEO_INT, ("Failed to initialize OS layer\n"));
        return DEO_ERROR;
    }

    /* Initialize OS dependent services */
    if (DEO_SUCCESS != VVDRV_deo_init_os(cfg))
    {
        DEO_DBG_F(DEO_INT, ("Failed to initialize OS layer\n"));
        return DEO_ERROR;
    }

    /* Initialize the memory pool, if requested */
    if (TRUE == cfg->init_dma_mempool)
    {
        if (DEO_SUCCESS != VVDRV_deo_mem_init(&cfg->mempool_cfg))
        {
            DEO_DBG_E(DEO_INIT, ("Failed to initialize the memory pool\n"));
            return DEO_ERROR;
        }
    }

    /* Initialize DEO interrupt service, if requested */
    ic_cfg = (deo_interrupt_controller_init_t *)cfg->interrupt_controller_cfg;
    if (NULL != ic_cfg)
    {
        if (DEO_SUCCESS != VVDRV_deo_interrupt_init(ic_cfg))
        {
            DEO_DBG_F(DEO_INT, ("Failed to initialize interrupt service\n"));
            return DEO_ERROR;
        }
    }

    /* Initialize the DEO timer service */
    if (DEO_SUCCESS != VVDRV_deo_time_init())
    {
        DEO_DBG_F(DEO_INT, ("Failed to initialize timer service\n"));
        return DEO_ERROR;
    }

    return DEO_SUCCESS;
}

void VVDRV_deo_uninit(void)
{
    VVDRV_deo_mem_uninit();           /* Un-initialize the memory pools */
    VVDRV_deo_interrupt_uninit();     /* Un-initialize the interrupt service */
    VVDRV_deo_time_uninit();          /* Un-initialize the timer service */
    VVDRV_deo_uninit_os();            /* Un-initialize OS dependent services */
    VVDRV_deo_mutex_uninit();         /* Un-initialize the mutex service */
}
