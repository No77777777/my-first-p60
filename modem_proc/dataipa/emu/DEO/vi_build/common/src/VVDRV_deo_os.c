/**---------------------------------------------------------------------------
 * @file  VVDRV_deo_os.c
 *
 * @brief Implements DEO VI build init functions
 *
 * Copyright (c) 2011-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/************************************************************************/
/*                           INCLUDES                                   */
/************************************************************************/

#include "VVDRV_deo_common.h"         /**< DEO_SUCCESS        */ 
#include "VVDRV_deo_os.h"             /**< API coherency      */ 

/************************************************************************/
/*                             FUNCTIONS                                */
/************************************************************************/

int32 VVDRV_deo_init_os(deo_cfg_t *cfg)
{
    (void)cfg;
    return DEO_SUCCESS;
}

void VVDRV_deo_uninit_os(void)
{
  /* Empty implementation */
}

int32 VVDRV_deo_interrupt_uninit_os(void)
{
    return DEO_SUCCESS;
}
