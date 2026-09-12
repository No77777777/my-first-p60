#ifndef RFLTE_CORE_H
#define RFLTE_CORE_H

/*! 
  @file
  rflte_core.h
 
 @brief
 This file is used to define an interface to all core layer APIs needed by
 the MC layer.
 */

/*==============================================================================

  Copyright (c) 2013 - 2020 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/rftech_lte.mpss/3.11/rf/core/inc/rflte_core.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
12/23/19   ssl     adding UE power class as input argument to rflte_core_update_sar() API
02/10/17   sk      Fix initialisation of sar,coex and other backoffs to make sure max power works for power class 2.
04/25/16   rp      MPR/AMPR CA NS NV override Support
03/11/16   am      Added rflte_core_get_sar_forced_ant()
12/07/15   sg      Make TxPL APIs handle based
11/23/15   br      Added fix for tuner rf dispatch snum crash
11/04/15   mah     Changes to enable handle bases TxPL SM.
11/04/15   mah     Handle based TXPL SM.
10/15/15   kab     Fix PHR backoff and Max allowed UE Tx power (in dB10 units)
09/15/15    pv     Added support to avoid SM lookup for TxLM index and use index from tune config.
09/02/15   kr      Added support for tx max pwr based EARFCN NV
08/28/15    sg     FR28508: Add ULCA Support for MTPL Backoff due to PA thermal
07/20/15   pkg     Changes to support FR 24616:Algorithm to mitigate VBatt
                   droop (down to 2.5V) via TX backoff.
08/18/14   kab     TxPL changes to support SGLTE Tx power backoff with Coex.
07/31/14   ndb     Added support for extended EARFCN for LTE
06/13/14   svi     Notch Application based on Tx AFC
09/12/13   svi     Added MC interfaces
09/05/13   cri     Initial check in

==============================================================================*/

#include "comdef.h"
#include "rfcom.h"
#include "rflte_mc.h"
#include "rflte_nv.h"
#include "rfc_card_lte.h"
#include "rfcommon_core_xpt.h"
#include "rflte_state.h"
#include "lm_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
  RFLTE_CORE_SGLTE_MTPL_UPDATE,
  RFLTE_CORE_WLAN_COEX_MTPL_UPDATE
}rflte_core_mtpl_type;

#define RF_MTPL_LOG_PACKET  0

extern rflte_core_txpl_sm_type rflte_core_txpl_sm[RFA_RF_LTE_MAX_CELL_SUPPORTED];
rflte_core_compute_mtpl_output_param_s rflte_core_txpl_dbg_s;

/*----------------------------------------------------------------------------*/
void rflte_core_enter_mode(void);
/*----------------------------------------------------------------------------*/
void rflte_core_exit_mode(uint32 carrier_index);
/*----------------------------------------------------------------------------*/
void rflte_core_sleep(uint32 carrier_index);
/*----------------------------------------------------------------------------*/
void rflte_core_tx_config(uint8 sub_id,
                          rflte_mc_carrier_info_type carrier_info,
                          rfcom_lte_band_type band,
                          rfcom_lte_bw_type bw,
                          rfc_lte_band_lpm_type lpm_status_info,
                          xpt_cfg_type xpt_cfg,
                          rfa_rf_lte_tdd_ul_dl_cfg_type tdd_ul_dl_cfg,
                          rfcom_lte_earfcn_type tx_channel,
                          boolean intra_band_ulca_active);
/*----------------------------------------------------------------------------*/
void rflte_core_tx_disable(uint8 sub_id,
                           rflte_mc_carrier_info_type carrier_info,
                           rfcom_lte_band_type band,
                           xpt_cfg_type xpt_cfg);

void rflte_core_disable_tuner_device_tx_handle_switch( rflte_mc_carrier_info_type          carrier_info,
                                                       lm_handle_type                      txlm_buffer_idx_ori,
                                                       lm_handle_type                      txlm_buffer_idx_new,
                                                       rfcom_lte_band_type                 band);
/*----------------------------------------------------------------------------*/
void rflte_core_update_rxagc_freq_comp(rfm_device_enum_type device,
                                       rfcom_lte_band_type band,
                                       rfcom_lte_earfcn_type rx_chan,
                                       rflte_nv_tbl_type* nv_tbl_ptr,
                                       boolean meas_nbr_flag,
                                       boolean intra_band_ca_flag,
                                       uint32 carrier_idx,
                                       boolean apply_ota_flag);
/*----------------------------------------------------------------------------*/
void rflte_core_update_sar(uint8 sub_id,
                           lm_handle_type txlm_buffer_idx,
                           uint32 carrier_idx,
                           rfcom_lte_earfcn_type tx_channel,
                           rflte_core_txpl_power_class_type ue_power_class);
/*----------------------------------------------------------------------------*/
boolean rflte_core_get_sar_forced_ant(uint8 ant);
/*----------------------------------------------------------------------------*/
void rflte_core_set_mtpl(uint8 sub_id,
                         rflte_core_mtpl_type mtpl_type,
                         uint32 carrier_idx,
                         lm_handle_type txlm_buffer_idx,
                         uint32 plimit_type_mask,
                         int16 sar_limit_dB10,
                         int16 coex_limit_dB10,
                         int16 wcn_limit_dB10,
                         int16 subframe_coex_limit_dB10,
                         int16 vbatt_limit_dB10,
                         int16 pa_thermal_limit_dB10);
/*----------------------------------------------------------------------------*/
void rflte_core_enter_critical_section(lm_handle_type  txlm_buffer_idx);
/*----------------------------------------------------------------------------*/
void rflte_core_leave_critical_section(lm_handle_type  txlm_buffer_idx);
/*----------------------------------------------------------------------------*/
void rflte_core_program_afc_notch_update(uint8 sub_id, uint32 cell_index );
/*----------------------------------------------------------------------------*/
void rflte_core_map_ca_ns_ampr_rb_alloc_info_from_nv(rfnv_ca_ns_ampr_backoff_static_data_combo_type *ca_ns_ampr_static_data_combo,
                                                                 uint8 ca_ns_index);
/*----------------------------------------------------------------------------*/
void rflte_core_map_ca_ns_mpr_rb_alloc_info_from_nv(rfnv_ca_ns_mpr_backoff_static_data_combo_type *ca_ns_mpr_static_data_combo,
                                                                uint8 ca_ns_index);

#ifdef __cplusplus
}
#endif

#endif /* RFLTE_CORE_H */
