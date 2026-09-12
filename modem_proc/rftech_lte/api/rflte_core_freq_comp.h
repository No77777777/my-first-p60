#ifndef RFLTE_CORE_FREQ_COMP_H
#define RFLTE_CORE_FREQ_COMP_H

/*! 
  @file
  rflte_core.h
 
 @brief
 This file is used to define an interface to all core layer APIs needed by
 the MC layer.
 */

/*==============================================================================

  Copyright (c) 2013 -2020 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/rftech_lte.mpss/3.11/api/rflte_core_freq_comp.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
10/30/15   mah      txlm handle fix for all api inside rflte_core_computer_p_cmax_update
10/30/15   mah     Change more api to txlm handle based 
10/30/15   mah     Handle based TXPL SM.
08/18/15    JJ     Added api to compute fbrx weighted avg freqcomp for stage3
08/18/15    JJ     Made rflte_core_tx_tune_state_update visible to other files
06/04/14    pv     Added support for FBRx RB Based Freq comp.
04/17/14   svi     PA static NV and code redesign
09/05/13   cri     Initial check in

==============================================================================*/

#include "comdef.h"

/* FBRx Related */
#include "rfcommon_fbrx_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*--------------------------------------------------------------------------*/
void rflte_core_clear_fbrx_freq_comp_offsets(uint32 carrier_idx, lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
void rflte_core_tx_tune_state_update(uint32 carrier_idx, lm_handle_type txlm_buffer_idx, boolean update);
/*--------------------------------------------------------------------------*/
void rflte_core_compute_freq_comp(uint8 sub_id,
                                  uint32 carrier_idx,
                                  uint8* rb_start,
                                  uint8* rb_block,
                                  int32* lin_vs_freq_val_p_in,
                                  int32* lin_vs_freq_val_p_out,
                                  int32* lin_vs_freq_val_legacy,
                                  boolean is_lin_vs_temp_vs_freq_in_use);

/*--------------------------------------------------------------------------*/
int16 rflte_core_compute_freq_comp_lim(uint8 sub_id,
                                       uint32 carrier_idx,
                                       uint8 phy_start_rb,
                                       uint8 phy_rb_block);

/*--------------------------------------------------------------------------*/
boolean rflte_core_compute_fbrx_rb_based_gain_comp(uint32 carrier_idx,
                                                   uint32 phy_start_rb,
                                                   uint32 phy_rb_block,
                                                   lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
void rflte_core_compute_weighted_avg_fcomp_ulca(uint8 sub_id,
                                                lm_handle_type pcell_txlm_handle,
                                                lm_handle_type scell_txlm_handle,
                                                boolean pcell_srs_enabled,
                                                boolean scell_srs_enabled,
                                                uint8   pcell_num_rb,
                                                uint8   scell_num_rb,
                                                uint8   pcell_sub_fn_num,
                                                uint8   scell_sub_fn_num,
                                                boolean *comb_slot_active
                                               );
/*----------------------------------------------------------------------------*/
void rflte_core_fbrx_compute_weighted_avg_fcomp_ulca
                                               (
                                                lm_handle_type pcell_txlm_handle,
                                                lm_handle_type scell_txlm_handle,
                                                uint8   pcell_num_rb,
                                                uint8   scell_num_rb,
                                                uint8   pcell_sub_fn_num,
                                                uint8   scell_sub_fn_num
                                               );

#ifdef __cplusplus
}
#endif

#endif /* RFLTE_CORE_H */
