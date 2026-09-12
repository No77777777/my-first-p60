/*==============================================================================

  Copyright (c) 2012 - 2014 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.
    $Header: //components/rel/rfmodem_habanero.mpss/1.11/lm_habanero/inc/rflm_dtr_rx_bbrx_rsb_corr.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
06/30/15   lm      Initial revision
 
==============================================================================*/

#ifndef RFLM_DTR_RX_BBRX_RSB_CORR_H
#define RFLM_DTR_RX_BBRX_RSB_CORR_H

#include "comdef.h"

/*!
  @brief
  Number of bits in a BBRX_RSB fuse 
*/
#define BBRX_RSB_FUSE_SIZE 3
#define BBRX_RSB_FUSE_COMBINATIONS (1<<BBRX_RSB_FUSE_SIZE)
/*! Normalizing factor. Needed for BBRx RSSB correction */
#define RFCOMMON_MSM_GAIN_NORM 65536

/*----------------------------------------------------------------------------*/

typedef enum
{
  BBRX_MODE_0,
  BBRX_MODE_1,
  BBRX_MODE_2,
  BBRX_MODE_3,
  BBRX_MODE_MAX,
  BBRX_MODE_NUM = BBRX_MODE_MAX
} rfcommon_msm_bbrx_mode_type;

typedef struct
{
  struct bits_location_struct
  {
    uint32 reg_addr;
    uint32 reg_mask;
    uint8 reg_offset;
  } bits_location[BBRX_RSB_FUSE_SIZE];

  uint8 fuse_value;
} bbrx_rsb_fuse_reg_data;

typedef uint32 bbrx_fuse_to_gain_delta_value_type[BBRX_MODE_MAX][BBRX_RSB_FUSE_COMBINATIONS];

extern bbrx_rsb_fuse_reg_data bbrx_gain_reg_data[RXLM_CHAIN_MAX];
extern bbrx_rsb_fuse_reg_data bbrx_phase_reg_data[RXLM_CHAIN_MAX][BBRX_MODE_MAX];


uint32* rfcommon_msm_get_bbrx_fuse_to_gain_value_table(void);

bbrx_fuse_to_gain_delta_value_type *rfcommon_msm_get_bbrx_fuse_to_gain_delta_value_table(void);

/*Return gain_delta table for LTE 40Mhz*/
uint32* rfcommon_msm_get_bbrx_fuse_to_gain_delta_value_table_lte_40mhz(void);


#endif
