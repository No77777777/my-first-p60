/**
  @file IWCDMA.h

  @brief  This is the header file that describes an interface to a component

*/
/*
  Copyright (c) 2013 Qualcomm Technologies, Inc.
          All Rights Reserved.
    Qualcomm Confidential and Proprietary
*/

/*===========================================================================
                      EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/wcdma.mpss/8.4.0/api/IWCDMA.h#3 $
  $DateTime: 2020/01/24 03:56:36 $


  when        who     what, where, why
  ---------   ---     ------------------------------------------------------------
12/15/16     ha       Made changes for WRRC supports priority based reselection towards GERAN
11/21/17     sp       Made changes to provide an API which returns WCDMA release version
02/24/17     rn       Doubling WCDMA_SIGNAL_INFO functions based on sub id
04/12/16     kv       FR 32607: Web Boosting to improve latency by initiating TVM so that NW can move UE to DCH
03/24/16     ac       Fix function name in IWCDMA_wcdma_qual_estimation_change_report_state macro
02/04/16     nr       Made changes to expose rrc_find_rab_for_rb_in_est_rabs_sub API
12/21/15     ar       Introduce interface to check if WFW App can be disabled
10/02/15     ac       FR 26288: DS downlink quality measurement reporting
09/29/15     nr       Made changes inorder for TC to make use sub specific WRRC api
08/27/15     sg       Made changes to introduce a wrapper to NAS to indicate UE HSUPA category
07/27/15     sp       Made changes to cleanup unused voice interfaces.
07/22/15     kv       FR 22377: RAVE-API
05/26/15     sr       Initial checkin for FR 26957 - TxAGC logging
05/20/15     sks      Compilation error fix for W+W check in.
04/29/15     scv      FR 25842: RLC RTT & Queuing Delay Computation
04/13/15     geg      Fixed a copy paste srror
04/10/15      cc      Remove an unused API
04/08/15      sa      Made changes to support FR25951_SingleVoiceSession
04/02/15     geg      Added new API for FR 16887: Display of Partial PLMN Search Results during Manual Search 
03/19/15      ac      FR 25733: UL/DL throughput reporting 
03/10/15     sas      Added api interface fun to provide acqdb cell list info to LTE.
02/28/15     rsr      W+W Phase 2 Checkin.
02/19/15     ks       Add wl1_x2w_srch_init_g2w_crit_sect
02/19/15     geg      Added new API for FR 23226: WRRC changes for inclusion of AS ID as a new argument to functions 
02/19/15     sas      Made changes to add a new API which takes as_id as agrument and calls rrcdormancy_check_if_nw_support_fast_dormancy() 
05/28/14     geg      Mainlined feature FEATURE_WCDMA_GET_RAT_MEAS_INFO
04/28/14      sp      Made changes to add rrc_vocoder_control_enabled_sub for DUAL SIM cases
04/14/14      cc      Create an API for RF to call idle W2X search cfg to WFW in FTM mode
01/15/14     geg      Remove extra single space from IWCDMA.h which causes compilation error
01/09/14     geg      Segment Loading - WCDMA interface update on Bolt1.x. 
12/30/13     geg      Support new FEATURE_DUAL_SIM on DIME3.0. 
12/30/13     geg      Clean up WCDMA private API from interface inclusion chain for Feature Segment Loading DIME 3.0
01/08/14      geg     Segment Loading - WCDMA interface updates for DIME3.0 
10/17/13     hzhi    Support new FEATURE_DUAL_SIM on DIME3.0. 
 10/09/13     geg     Clean up WCDMA private API from interface inclusion chain for Feature Segment Loading DIME 3.0
 10/02/13     gp      Add dummy dpcch indication for ul cfg dpcch cmd
 10/01/2013   geg     Update W Interface on DIME 3.0 for segment loading. 
 07/24/13     jd      Changes for Dual band Dual carrier
 2013/05/10   hzhi    Added file hearder according to qualcomm standards.
 2013/01/01   ngeng   Created for segment loading.

===========================================================================*/
#ifndef _IWCDMA_H_
#define _IWCDMA_H_

#include "mcfg_seg_load.h"

/*======================================================================================
  editted by hzhi on 09/23/2013 to update WCDMA interface on DIME3.0 for segment loading. 
  ======================================================================================*/
#include "wcdma_common_api.h"
#include "l1lsmif.h"
#include "l1task.h"
#include "rrcintif.h"
#include "rrccmd.h"

#include "cmapi.h"//added on 02/25/2014 for MCS customer request.
#include "ds3g_ext.h"
/*=====================================================================================*/

/*  The component that is being used provides typedefs for pointers to each
    of its exported functions. 
 */

typedef boolean (*fn_l1_cgps_register_event_cb) (l1_cgps_event_cb_type event_cb_fp);
typedef boolean (*fn_wl1_cgps_register_event_cb) (l1_cgps_event_cb_type event_cb_fp, sys_modem_as_id_e_type wsub_id);
typedef l1_ext_cmd_type* (* fn_l1_get_cmd_buf)(void);
typedef boolean (*fn_l1_is_sub_chan_avail_for_asc) 
(
 uint8 asc_idx
 );

typedef void (*fn_l1_put_cmd)
(
 l1_ext_cmd_type* cmd_ptr
 );

typedef void (*fn_wl1_put_cmd)
(
  l1_ext_cmd_type* cmd_ptr,
  sys_modem_as_id_e_type wsub_id
);

typedef l2_dl_cmd_type * (*fn_l2_dl_get_cmd_buf)(void);

typedef void (*fn_l2_dl_put_cmd)(l2_dl_cmd_type *cmd_ptr);

typedef l2_ul_cmd_type * (*fn_l2_ul_get_cmd_buf)(void);

typedef void (*fn_l2_ul_put_cmd)(l2_ul_cmd_type  *cmd_ptr);

typedef boolean (*fn_mdsp_sync_iq_sample_capture_cmd)
(
  uint32 first_trig,          /* flag to do initialization */
  uint32 last_trig,           /* flag to do de-initialization */
  uint32 seq_id,/* sequence id of command sent to common FW */
  uint8 capt_buf_id,/* capture buffer id for use by common FW */
  uint8 res_buf_id,/* results buffer id for use by common FW */
  pre_cb_fn pre_cb,           /* the pre-command callback function */
  post_cb_fn post_cb          /* the post-commadn callback function */
);

typedef boolean (*fn_mdsp_sync_send_iq_cap_cmd)
(
  
  uint32 rxlmBufferId,         /* RxLm Buffer Id */
  uint32 chain,                /* Rx Chain to start the capture on */
  uint32 startTime,            /* Time to begin the capture */
  uint16 carrier,              /* Carrier information */
  uint16 numSamples,           /* Number of samples to capture */
  pre_cb_fn_old pre_cb,            /* pre-command callback function */
  post_cb_fn_old post_cb           /* post-command callback function */
);

typedef boolean (*fn_mdsp_sync_send_mod_timing_cmd)
(
  
  boolean slam,             /* slam or slew to the channel timing */
  uint8 phch,               /* which hw channel contains the downlink physical channel that the uplink
                               channel whose timing relation is being set derives it's timing from */
  uint32 txPnPositioncx8,   /* Absolute Tx PN position (DL DPCH timing + T0) */
  uint8 tti,                /* the value of the TTI counter must be set to when this command takes
                               effect */

  int8 dl_ref_offset,       /* DL reference offset */
  post_cb_fn post_cb,       /* the post callback function, may be NULL */
  pre_cb_fn pre_cb          /* the pre callback function, may be NULL */
);

typedef boolean (*fn_mdsp_sync_send_rf_init_cmd)
(
  uint32 event_id,
  uint32 offset,
  pre_cb_fn pre_cb,          /* pre-command callback function */
  post_cb_fn post_cb         /* post-command callback function */
);

typedef boolean (*fn_mdsp_sync_send_rxagc_cmd)
(
  void *agc_cfg,                                  /* AGC command config parameters */ 
  pre_cb_fn_old pre_cb,                               /* pre-command callback function */
  post_cb_fn_old post_cb                              /* post-command callback function */
);

typedef boolean (*fn_mdsp_sync_send_txlm_cfg_cmd)
(
  boolean mode_enable,        /*TxLM mode 1: enable, 0:disable*/
  uint32 buf_idx,             /* TxLM buffer index */
  pre_cb_fn_old pre_cb,           /* the pre-command callback function */
  post_cb_fn_old post_cb          /* the post-commadn callback function */
);

typedef boolean (*fn_mdsp_sync_send_ul_dpcch_cfg_cmd)
(
  
  boolean ena,                /* enable or disable DPCCH processing */
  boolean dummy_dpcch,        /* Real or dummy DPCCH cmd */
  uint16 slot_format,         /* slot format to use */
  uint16 uncoded_tfci,        /* the uncoded TFCI bits */
  wlulcm_wait_ScchOrderSec_type waitforscchorder,     /* Waitforscchorder */
  boolean disableseccarrier,    /* Disable secondary carrier */
  pre_cb_fn pre_cb,           /* the pre-command callback function */
  post_cb_fn post_cb          /* the post-commadn callback function */
);

typedef void (*fn_mdsp_send_ftm_idle_mode_w2x_cfg_cmd)
(
  /* the rxlm buffer index used to tune back the primary Rx chain to WCDMA at
     the end of an idle w2x search */
  uint32 rxlm_home_buf_idx,
  /* the rxlm buffer index used to tune back the diversity Rx chain to WCDMA at
     the end of an idle w2x search */
  uint32 rxlm_home_rxd_buf_idx,
  /* determines if the search is w2l or w2g */
  mdsp_sync_w2x_idle_cmd_mode_enum_type mode,
  /* pre-command callback function */
  pre_cb_fn pre_cb,
  /* post-command callback function */
  post_cb_fn post_cb
);

typedef void (*fn_rlc_dl_deregister_srvc_handler)
(
  rlc_dl_deregister_srvc_type *de_reg_ptr      /* Ptr to WM registration Info   */
);
 
typedef void (*fn_rlc_dl_register_srvc_handler)
(
  rlc_dl_register_srvc_type *reg_ptr      /* Ptr to WM registration Info   */
);

typedef void (*fn_rlc_ul_deregister_srvc_handler)
(
  rlc_ul_deregister_srvc_type *de_reg_ptr /* Ptr to WM registration info   */
);
 
typedef void (*fn_rlc_ul_register_srvc_handler)
(
  rlc_ul_register_srvc_type *reg_ptr      /* Ptr to WM registration info   */
);

typedef void (*fn_rrc_delete_classmark_change_handover_info) (void);

typedef rrc_rab_id_type (*fn_rrc_find_rab_for_rb_in_est_rabs)
(
  uint8 rb_id
);

typedef rrc_rab_id_type (*fn_rrc_find_rab_for_rb_in_est_rabs_sub)
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint8 rb_id
);

typedef void (*fn_rrc_free_cmd_buf)(rrc_cmd_type *cmd_buf);

typedef void (*fn_rrc_get_classmark_change_handover_info)
(
  byte ** msg_ptr,    /* The command to be sent to UTRAN */
  uint8 * msg_len,    /* Length of the message           */
  boolean irat_ho_info_compressed
);
typedef rrc_cmd_type * ( *fn_rrc_get_cm_cmd_buf) (void);

typedef rrc_data_rate_info_type (*fn_rrc_get_data_rate_for_ds) (void);

typedef rrc_cmd_type * ( *fn_rrc_get_l1_cmd_buf)  ( void );

typedef rrc_cmd_type * ( *fn_rrc_get_lsm_cmd_buf) ( void );

typedef rrc_cmd_type * ( *fn_rrc_get_mm_cmd_buf)  ( void );

typedef rrc_rr_cmd_type * ( *fn_rrc_get_rr_cmd_buf) ( void );

typedef rrc_cmd_type * ( * fn_rrc_get_tc_cmd_buf) ( void );

typedef boolean (*fn_rrc_is_arfcn_valid)
(
  uint32 UARFCN,
  PLMN_id_T plmn_id
);

typedef boolean (*fn_rrc_is_wcdma_active) (void);

typedef void *  (*fn_rrc_malloc_for_external_cmd)
(
size_t size
);

typedef void (*fn_rrc_put_cm_cmd)
(
  rrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
);

typedef void (*fn_rrc_put_lsm_cmd)
(
  rrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
);

typedef void (*fn_rrc_put_mm_cmd)
(
  rrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
);

typedef void (*fn_rrc_put_rr_cmd)
(
  rrc_rr_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
);

typedef void (*fn_rrc_put_tc_cmd)
(
  rrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
);

typedef void (*fn_rrc_register_rrc_codec_ho_info_cb)
(
  RRC_CODEC_HO_INFO_CB_FUNC_TYPE *cb_ptr
);

typedef boolean (*fn_rrc_register_serving_cell_change_ind_cb)
(
  RRC_SERVING_CELL_CHANGE_IND_CB_FUNC_TYPE* cb_ptr
);

typedef void (*fn_rrc_reset_mcm_irat_resel_act_cause) (void);

typedef rrc_status_e_type (*fn_rrc_return_cs_data_call_parms)
(    
  uint32                  rab_id,
  rrc_cs_data_parms_type *cs_data_parms_ptr
);

typedef rrc_rab_search_e_type (*fn_rrc_return_lc_info_for_rab)
(
  rrc_user_plane_lc_info_type *ptr
);

typedef rrc_rr_cell_bar_info_e_type (*fn_rrc_rr_check_cell_barred)
(
  uint16 freq, uint16 scr_code
);

typedef void (*fn_rrc_set_ue_in_deep_sleep) (boolean value);

typedef void (*fn_rrc_update_band_preference)
(
  sys_band_mask_type band_pref_mask
);

typedef boolean (*fn_rrcgps_register_cgps_event_cb) ( rrcgps_cgps_event_cb_type event_cb_fp );

typedef boolean (*fn_rrcgps_register_cgps_ue_pos_capability_cb)
(
  rrcgps_cgps_ue_pos_capability_cb_type ue_cap_cb_fp
);

typedef boolean (*fn_rrcmeas_get_cell_plmn_info)
(
  rrcmeas_plmn_cell_info_struct_type* plmn_cell_info_ptr
);

typedef boolean (*fn_rrcscr_get_signalling_connection_status)
(
  rrc_cn_domain_identity_e_type cn_domain_id                   
);

typedef void (*fn_rxd_overide_nv_settings)(uint8 overide_nv_val);

typedef boolean (*fn_rxd_set_active_rx_chain) (boolean chain0, boolean chain1);

typedef uint8 (*fn_seq_get_cfn) (void);

typedef wcdma_signal_info_struct_type * (* fn_wcdma_get_signal_info) (sys_modem_as_id_e_type wsub_id);

typedef  wcdma_l2_dl_cmd_type  * (* fn_wcdma_l2_dl_get_cmd_buf) (void);

typedef  void  (*fn_wcdma_l2_dl_put_cmd)
(
  /* Pointer to the command buffer */
  wcdma_l2_dl_cmd_type   *cmdptr

);

typedef  wcdma_l2_ul_cmd_type * (*fn_wcdma_l2_ul_get_cmd_buf)(void);

typedef  void (*fn_wcdma_l2_ul_put_cmd)
(
  /* Pointer to the command buffer */
  wcdma_l2_ul_cmd_type   *cmd_ptr
);

typedef void (*fn_rlc_query_traffic_time_metrics) 
( 
  uint8 sub_ID, uint8 rlc_id, delay_metric_type delay_type, rlc_traffic_time_metrics* rlc_ul_traffic_metrics 
);

typedef void (*fn_rlc_reset_traffic_time_metrics) 
( 
  uint8 sub_ID, uint8 rlc_id 
);

typedef int16 (*fn_wcdma_query_tx_power_dbm)(void);

typedef sys_band_mask_type (*fn_wcdma_rrc_get_band_from_dl_uarfcn)
(
  uint16 uarfcn,
  sys_plmn_id_s_type camped_plmn_id,
  boolean *is_band_valid
);

typedef boolean (*fn_wl1_check_state_for_delayed_gstmr_int) (void); 

/*=====================================================================================
  added by hzhi on 09/23/2013 to update WCDMA interface on DIME3.0 for segment loading. 
  =====================================================================================*/
typedef boolean 
  (*fn_mdsp_sync_send_et_txlm_cfg_cmd)
    (
      boolean mode_enable,
      uint32 buf_idx,
      pre_cb_fn pre_cb,
      post_cb_fn post_cb
    );

typedef boolean 
  (*fn_mdsp_sync_send_rxlm_dyn_update_cmd)
    (
      uint8 rxlmBufferId[],
      uint32 rxlmDynGroupMask,
      uint32 rxAntennaMask,
      pre_cb_fn pre_cb,
      post_cb_fn post_cb
    );

typedef boolean 
  (*fn_mdsp_sync_send_sample_server_cmd)
    (
      boolean freeze,
      boolean unfreeze,
      boolean chain0,
      boolean chain1,
      boolean rfPwrUpdateFlag,
      uint16 actionTime,
      pre_cb_fn pre_cb,
      post_cb_fn post_cb
    );

typedef void 
  (*fn_rrc_gstk_get_utran_nmr_req)
    (
      uint8 num_cells, gstk_nmr_utran_qualifier_enum_type report_type, 
      uint32 *ref_id,
      sys_modem_as_id_e_type as_id
    );

typedef void 
  (*fn_wcdma_get_modem_stats_info)
    (
      wcdma_UI_event_info_type *wcdma_info
    );

typedef void 
  (*fn_wcdma_set_modem_stats_mask)
    (
      uint16 bit_mask
    );

typedef void 
  (*fn_wmcpmdrv_rf_ftm_mode_req)
    (
      boolean request
    );

typedef boolean (*fn_wcdma_start_capture_scan_data)
(
    void *wcdma_scan_data, 
    uint16  wcdma_scan_data_mem_size
);

typedef boolean (*fn_wcdma_stop_capture_scan_data)(void);

/*
 added by hzhi on 10/17/2013 to support new FEATURE_DUAL_SIM on DIME3.0. 
 */
typedef uint8 (*fn_rrc_return_wcdma_rrc_version)(void);

typedef void (*fn_rrc_delete_classmark_change_handover_info_sub_id)(sys_modem_as_id_e_type as_id);

typedef void (*fn_rrc_get_classmark_change_handover_info_sub_id)
(
  byte ** msg_ptr,  /* The command to be sent to UTRAN */
  uint8 * msg_len,  /* Length of the message */
  boolean irat_ho_info_compressed,
  sys_modem_as_id_e_type as_id
);

typedef boolean (*fn_rrc_is_arfcn_valid_sub_id)
(
  uint32 UARFCN,
  PLMN_id_T plmn_id,
  sys_modem_as_id_e_type as_id
);

typedef rrc_rr_cell_bar_info_e_type (*fn_rrc_rr_check_cell_barred_sub_id)
(
  uint16 freq, uint16 scr_code, 
  sys_modem_as_id_e_type as_id
);

typedef void (*fn_rrc_set_ue_in_deep_sleep_sub_id)( boolean value,sys_modem_as_id_e_type as_id);

typedef void (*fn_rrc_update_band_preference_sub_id)
(
  sys_band_mask_type band_pref_mask,  /* band mask */
  sys_modem_as_id_e_type  as_id
);

typedef boolean (*fn_wl1trm_can_gsm_disable_fw_app_wcdma)(void);

typedef boolean (*fn_wl1trm_allow_wcdma_fw_app_disable)(sys_modem_as_id_e_type as_id);

typedef uint8 (*fn_wcdma_rrc_get_wcsg_lfp_info)
(
   const source_cell_info_type *l_macro_ptr, 
   source_cell_info_type *w_csg_cells_ptr
);

/*
 *  added on 01/07/2014
 */
typedef boolean 
  (*fn_wl1_eul_stats)
    (
      wl1_eul_stats_struct_type *eul_stats
    );

typedef wl1_ul_modem_tx_power_enum_type 
  (*fn_wl1_query_modem_tx_power)
    (
      void
    );

typedef boolean 
  (*fn_wcdma_convert_uarfcn)
    (
      uint16 source_uarfcn,
      sys_sband_e_type band_num,
      uint16 *target_uarfcn,
      sys_plmn_id_s_type camped_plmn_id
    );

typedef errno_enum_type 
  (*fn_wcdma_derive_freq_from_uarfcn)
    (
      uint16 uarfcn,
      sys_plmn_id_s_type camped_plmn_id,
      uint16 *freq
    );

/*
 * added on 02/25/2014 for MCS customer request. 
 */
typedef void 
  (*fn_wcdma_get_rat_meas_info)
    (
      cmapi_wcdma_signal_info_struct_type* wcdma_signal_info,
      sys_modem_as_id_e_type wsub_id
    );

typedef boolean 
  (*fn_wcdma_is_rat_meas_info_ready)
    (
      sys_modem_as_id_e_type wsub_id
    );

typedef boolean (*fn_rrc_check_if_nw_support_fd)
(    
  sys_modem_as_id_e_type wrrc_as_id
);
typedef uint8 (*fn_rrc_ext_get_wcdma_hsupa_category)
(
  sys_modem_as_id_e_type wrrc_as_id
);

typedef void (*fn_rrc_put_mm_cmd_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
);

typedef void (*fn_rrc_put_tc_cmd_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
);

typedef void (*fn_rrc_put_lsm_cmd_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
);

typedef void (*fn_rrc_put_cm_cmd_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
);

typedef rrc_rab_search_e_type (*fn_rrc_return_lc_info_for_rab_sub)
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_user_plane_lc_info_type *ptr
);

typedef rrc_status_e_type (*fn_rrc_return_cs_data_call_parms_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  uint32                  rab_id,
  rrc_cs_data_parms_type *cs_data_parms_ptr
);

typedef boolean (*fn_rrc_is_wcdma_active_sub)
(sys_modem_as_id_e_type wrrc_as_id);

typedef rrc_hsdpa_status_enum_type (*fn_rrc_get_hsdpa_status_sub)
(
  sys_modem_as_id_e_type wrrc_as_id
);

typedef boolean (*fn_rrcmeas_get_cell_plmn_info_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrcmeas_plmn_cell_info_struct_type* plmn_cell_info_ptr
);

typedef boolean (*fn_rrcgps_register_cgps_event_cb_sub)
( sys_modem_as_id_e_type wrrc_as_id, 
  rrcgps_cgps_event_cb_type event_cb_fp
);

typedef boolean (*fn_rrcgps_register_cgps_ue_pos_capability_cb_sub)
( sys_modem_as_id_e_type wrrc_as_id, 
  rrcgps_cgps_ue_pos_capability_cb_type ue_cap_cb_fp
);

typedef uint8 (*fn_rrc_return_wcdma_rrc_version_sub)
( sys_modem_as_id_e_type wrrc_as_id );

typedef void (*fn_rrc_register_rrc_codec_ho_info_cb_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  RRC_CODEC_HO_INFO_CB_FUNC_TYPE *cb_ptr
);

typedef void (*fn_rrc_deregister_rrc_codec_ho_info_cb_sub)
(sys_modem_as_id_e_type wrrc_as_id);

typedef void (*fn_wcdma_set_modem_stats_mask_sub)
( sys_modem_as_id_e_type wrrc_as_id, 
  uint16 bit_mask
);

typedef void (*fn_wcdma_get_modem_stats_info_sub)
( sys_modem_as_id_e_type wrrc_as_id, 
  wcdma_UI_event_info_type *wcdma_info
);

typedef boolean (*fn_rrc_register_serving_cell_change_ind_cb_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  RRC_SERVING_CELL_CHANGE_IND_CB_FUNC_TYPE* cb_ptr
);

typedef boolean (*fn_rrc_deregister_serving_cell_change_ind_cb_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  RRC_SERVING_CELL_CHANGE_IND_CB_FUNC_TYPE *cb_ptr
);

typedef void (*fn_rrc_set_ue_in_deep_sleep_sub)
(sys_modem_as_id_e_type wrrc_as_id, boolean value);

typedef void (*fn_rrc_reset_mcm_irat_resel_act_cause_sub)
(
  sys_modem_as_id_e_type wrrc_as_id
);

typedef void (*fn_rrc_put_rr_cmd_sub_id)
(
  rrc_rr_cmd_type *cmd_buf,  /* The command buffer to be put in the queue */
  sys_modem_as_id_e_type as_id
);

typedef sys_band_mask_type (*fn_wcdma_rrc_get_band_from_dl_uarfcn_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  uint16 uarfcn,
  sys_plmn_id_s_type camped_plmn_id,
  boolean *is_band_valid
);

typedef uint8 (*fn_wcdma_rrc_get_wcsg_lfp_info_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  const source_cell_info_type *l_macro_ptr, 
  source_cell_info_type *w_csg_cells_ptr
);

typedef errno_enum_type (*fn_wcdma_derive_freq_from_uarfcn_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  uint16 uarfcn,
  sys_plmn_id_s_type camped_plmn_id,
  uint16 *freq
);

typedef boolean (*fn_wcdma_convert_uarfcn_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  uint16 source_uarfcn,
  sys_sband_e_type band_num,
  uint16 *target_uarfcn,
  sys_plmn_id_s_type camped_plmn_id
);

typedef boolean (*fn_wcdma_mfbi_is_supported_sub)
(sys_modem_as_id_e_type wrrc_as_id);

typedef boolean (*fn_rrcscr_get_signalling_connection_status_sub)
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cn_domain_identity_e_type cn_domain_id
);

/*
 * added on 03/10/2015 to provide acqdb info to LTE. 
 */
typedef void (*fn_wcdma_rrc_get_acqdb_cell_list)
(
  sys_modem_as_id_e_type wrrc_as_id,
  wcdma_acqdb_cell_list  *wcdma_acqdb_cell_list_ptr
);

/*
 * added on 04/02/2015 to provide API to GRR and LTE RRC. 
 */
typedef boolean (*fn_wrrc_get_inter_rat_bplmn_prtl_result)
(
  sys_detailed_plmn_list_s_type *available_plmn_list
);

typedef boolean (*fn_wrrc_get_inter_rat_bplmn_prtl_result_sub_id)
(
  sys_detailed_plmn_list_s_type *available_plmn_list, 
  sys_modem_as_id_e_type as_id
);


/*
 * added on 03/19/2015 for DS customer request
 */
typedef void
  (*fn_wl1_start_tput_data_reporting_ind)
    (
      void
    );


typedef void
  (*fn_wl1_stop_tput_data_reporting_ind)
    (
      void
    );

typedef boolean
  (*fn_wl1_get_hsdpa_stats)
    (
      wl1_hsdpa_stats_struct_type*
    );

typedef uint32 (*fn_wcdma_ext_audio_api)
(
  uint32 cmd_id,
  void* params,
  uint32 size
);

/*
 * added on 05/27/2015 for TxAGC logging. 
 */
typedef void
   (*fn_wl1_config_modem_power_stats_tx)
     (
       boolean start_stop
     );

typedef void
   (*fn_wl1_get_modem_power_stats)
    (
      uint32 tx_power_bin[DS3G_TX_POWER_BIN_MAX]
    );

typedef void (*fn_wcdma_l2_start_uplink_rate_calculations)
(
  sys_modem_as_id_e_type  as_id,
  boolean                 enable,
  uint32                  interval
);

/* 
 * added on 10/2/2015 for DS downlink quality reporting
 */ 
typedef uint16 (*fn_wcdma_qual_estimation_set_report_period)
  (
    uint16 report_period,        /* Requested reporting period in ms*/
    sys_modem_as_id_e_type as_id /* AS ID for this request */
  );

typedef boolean (*fn_wcdma_qual_estimation_change_report_state)
  (
     wcdma_qual_estimation_report_state state,  /* Requested reporting period in ms*/
     sys_modem_as_id_e_type as_id               /* AS ID for this request */
  );

/*
 * added on 01/07/2016 for DS to trigger TVM forcibly
 */
 typedef void (*fn_wcdma_rlc_ds_trigger_tvm_forcibly)
   (
     sys_modem_as_id_e_type    sub_id,         /* SUB ID for this request */
     rlc_ds_tvm_trigger_reason reason          /* Reason to trigger TVM  */
   );

typedef wcdma_rrc_release_version_e_type (*fn_wcdma_rrc_get_3gpp_release_version_sub)
(
  sys_modem_as_id_e_type wrrc_as_id
);

/*Added on 11/2/16 for Absolute priority support */
typedef boolean (*fn_rrc_rr_absolute_priority_support)
(
  sys_modem_as_id_e_type wrrc_as_id
);
/*Added on 07/10/17 for L+L page collision full scope FR */
typedef boolean (*fn_rrc_check_mcc_cnfgd_in_fallback_nv)
(
  uint16 camped_mcc,
  sys_modem_as_id_e_type wrrc_as_id
);
/*==================================== end of edit ====================================*/

/*  The component then provides a vtable beginning with add_ref, release,
    and qi, followed by the functions it exports.
 */
typedef struct
{
  add_ref_fn  add_ref;
  release_fn  release;
  qi_fn       qi;
  fn_l1_cgps_register_event_cb l1_cgps_register_event_cb;
  fn_wl1_cgps_register_event_cb wl1_cgps_register_event_cb;
  fn_l1_get_cmd_buf l1_get_cmd_buf;
  fn_l1_is_sub_chan_avail_for_asc l1_is_sub_chan_avail_for_asc;
  fn_l1_put_cmd l1_put_cmd;
  fn_wl1_put_cmd wl1_put_cmd;
  fn_l2_dl_get_cmd_buf l2_dl_get_cmd_buf;
  fn_l2_dl_put_cmd l2_dl_put_cmd;
  fn_l2_ul_get_cmd_buf l2_ul_get_cmd_buf;
  fn_l2_ul_put_cmd l2_ul_put_cmd;
  fn_mdsp_sync_iq_sample_capture_cmd mdsp_sync_iq_sample_capture_cmd;
  fn_mdsp_sync_send_iq_cap_cmd mdsp_sync_send_iq_cap_cmd;
  fn_mdsp_sync_send_mod_timing_cmd mdsp_sync_send_mod_timing_cmd;
  fn_mdsp_sync_send_rf_init_cmd mdsp_sync_send_rf_init_cmd;
  fn_mdsp_sync_send_rxagc_cmd mdsp_sync_send_rxagc_cmd;
  fn_mdsp_sync_send_txlm_cfg_cmd mdsp_sync_send_txlm_cfg_cmd;
  fn_mdsp_sync_send_ul_dpcch_cfg_cmd mdsp_sync_send_ul_dpcch_cfg_cmd;
  fn_mdsp_send_ftm_idle_mode_w2x_cfg_cmd mdsp_send_ftm_idle_mode_w2x_cfg_cmd;
  fn_rlc_dl_deregister_srvc_handler rlc_dl_deregister_srvc_handler;
  fn_rlc_dl_register_srvc_handler rlc_dl_register_srvc_handler;
  fn_rlc_ul_deregister_srvc_handler rlc_ul_deregister_srvc_handler;
  fn_rlc_ul_register_srvc_handler rlc_ul_register_srvc_handler;
  fn_rrc_delete_classmark_change_handover_info rrc_delete_classmark_change_handover_info;
  fn_rrc_find_rab_for_rb_in_est_rabs rrc_find_rab_for_rb_in_est_rabs;
  fn_rrc_find_rab_for_rb_in_est_rabs_sub rrc_find_rab_for_rb_in_est_rabs_sub;
  fn_rrc_get_classmark_change_handover_info rrc_get_classmark_change_handover_info;
  fn_rrc_get_cm_cmd_buf rrc_get_cm_cmd_buf;
  fn_rrc_get_data_rate_for_ds rrc_get_data_rate_for_ds;
  fn_rrc_get_l1_cmd_buf rrc_get_l1_cmd_buf;
  fn_rrc_get_lsm_cmd_buf rrc_get_lsm_cmd_buf;
  fn_rrc_get_mm_cmd_buf rrc_get_mm_cmd_buf;
  fn_rrc_get_rr_cmd_buf rrc_get_rr_cmd_buf;
  fn_rrc_get_tc_cmd_buf rrc_get_tc_cmd_buf;
  fn_rrc_is_arfcn_valid rrc_is_arfcn_valid;
  fn_rrc_is_wcdma_active rrc_is_wcdma_active;
  fn_rrc_malloc_for_external_cmd rrc_malloc_for_external_cmd;
  fn_rrc_put_cm_cmd rrc_put_cm_cmd;
  fn_rrc_put_lsm_cmd rrc_put_lsm_cmd;
  fn_rrc_put_mm_cmd rrc_put_mm_cmd;
  fn_rrc_put_rr_cmd rrc_put_rr_cmd;
  fn_rrc_put_tc_cmd rrc_put_tc_cmd;
  fn_rrc_register_rrc_codec_ho_info_cb rrc_register_rrc_codec_ho_info_cb;
  fn_rrc_register_serving_cell_change_ind_cb rrc_register_serving_cell_change_ind_cb;
  fn_rrc_reset_mcm_irat_resel_act_cause rrc_reset_mcm_irat_resel_act_cause;
  fn_rrc_return_cs_data_call_parms rrc_return_cs_data_call_parms;
  fn_rrc_return_lc_info_for_rab rrc_return_lc_info_for_rab;
  fn_rrc_rr_check_cell_barred rrc_rr_check_cell_barred;
  fn_rrc_set_ue_in_deep_sleep rrc_set_ue_in_deep_sleep;
  fn_rrc_update_band_preference rrc_update_band_preference;
  fn_rrcgps_register_cgps_event_cb rrcgps_register_cgps_event_cb;
  fn_rrcgps_register_cgps_ue_pos_capability_cb rrcgps_register_cgps_ue_pos_capability_cb;
  fn_rrcmeas_get_cell_plmn_info rrcmeas_get_cell_plmn_info;
  fn_rrcscr_get_signalling_connection_status rrcscr_get_signalling_connection_status;
  fn_rxd_overide_nv_settings rxd_overide_nv_settings;
  fn_rxd_set_active_rx_chain rxd_set_active_rx_chain;
  fn_seq_get_cfn seq_get_cfn;
  fn_wcdma_get_signal_info wcdma_get_signal_info;
  fn_wcdma_l2_dl_get_cmd_buf wcdma_l2_dl_get_cmd_buf;
  fn_wcdma_l2_dl_put_cmd wcdma_l2_dl_put_cmd;
  fn_wcdma_l2_ul_get_cmd_buf wcdma_l2_ul_get_cmd_buf;
  fn_wcdma_l2_ul_put_cmd wcdma_l2_ul_put_cmd;
  fn_rlc_query_traffic_time_metrics rlc_query_traffic_time_metrics;
  fn_rlc_reset_traffic_time_metrics rlc_reset_traffic_time_metrics;
  fn_wcdma_query_tx_power_dbm wcdma_query_tx_power_dbm;
  fn_wcdma_rrc_get_band_from_dl_uarfcn wcdma_rrc_get_band_from_dl_uarfcn;
  fn_wl1_check_state_for_delayed_gstmr_int wl1_check_state_for_delayed_gstmr_int;
/*======================================================================================
  added by hzhi on 09/23/2013 to update WCDMA interface on DIME3.0 for segment loading. 
  ======================================================================================*/
  fn_mdsp_sync_send_et_txlm_cfg_cmd mdsp_sync_send_et_txlm_cfg_cmd;
  fn_mdsp_sync_send_rxlm_dyn_update_cmd mdsp_sync_send_rxlm_dyn_update_cmd;
  fn_mdsp_sync_send_sample_server_cmd mdsp_sync_send_sample_server_cmd;
  fn_rrc_gstk_get_utran_nmr_req rrc_gstk_get_utran_nmr_req;
  fn_wcdma_get_modem_stats_info wcdma_get_modem_stats_info;
  fn_wcdma_set_modem_stats_mask wcdma_set_modem_stats_mask;
  fn_wmcpmdrv_rf_ftm_mode_req wmcpmdrv_rf_ftm_mode_req;
  fn_wcdma_start_capture_scan_data wcdma_start_capture_scan_data;
  fn_wcdma_stop_capture_scan_data wcdma_stop_capture_scan_data;
/*
 added by hzhi on 10/17/2013 to support new FEATURE_DUAL_SIM on DIME3.0. 
 */
  fn_rrc_return_wcdma_rrc_version rrc_return_wcdma_rrc_version;
  fn_rrc_delete_classmark_change_handover_info_sub_id rrc_delete_classmark_change_handover_info_sub_id;
  fn_rrc_get_classmark_change_handover_info_sub_id rrc_get_classmark_change_handover_info_sub_id;
  fn_rrc_is_arfcn_valid_sub_id rrc_is_arfcn_valid_sub_id;
  fn_rrc_rr_check_cell_barred_sub_id rrc_rr_check_cell_barred_sub_id;
  fn_rrc_set_ue_in_deep_sleep_sub_id rrc_set_ue_in_deep_sleep_sub_id;
  fn_rrc_update_band_preference_sub_id rrc_update_band_preference_sub_id;
  fn_wl1trm_can_gsm_disable_fw_app_wcdma wl1trm_can_gsm_disable_fw_app_wcdma;
  fn_wl1trm_allow_wcdma_fw_app_disable wl1trm_allow_wcdma_fw_app_disable;
  fn_wcdma_rrc_get_wcsg_lfp_info wcdma_rrc_get_wcsg_lfp_info;
/*
 * added on 01/07/2014
 */
  fn_wl1_eul_stats wl1_eul_stats;
  fn_wl1_query_modem_tx_power wl1_query_modem_tx_power;
  fn_wcdma_convert_uarfcn wcdma_convert_uarfcn;
  fn_wcdma_derive_freq_from_uarfcn wcdma_derive_freq_from_uarfcn;

/*
 * added on 02/25/2014 for MCS customer request. 
 */
  fn_wcdma_get_rat_meas_info wcdma_get_rat_meas_info;
  fn_wcdma_is_rat_meas_info_ready wcdma_is_rat_meas_info_ready;
/*
 * added on 02/19/2015
 */
  fn_rrc_check_if_nw_support_fd rrc_check_if_nw_support_fd;
  fn_rrc_ext_get_wcdma_hsupa_category rrc_ext_get_wcdma_hsupa_category;

  fn_rrc_put_mm_cmd_sub rrc_put_mm_cmd_sub;
  fn_rrc_put_tc_cmd_sub rrc_put_tc_cmd_sub;
  fn_rrc_put_lsm_cmd_sub rrc_put_lsm_cmd_sub;
  fn_rrc_put_cm_cmd_sub rrc_put_cm_cmd_sub;
  fn_rrc_return_lc_info_for_rab_sub rrc_return_lc_info_for_rab_sub;
  fn_rrc_return_cs_data_call_parms_sub rrc_return_cs_data_call_parms_sub;
  fn_rrc_is_wcdma_active_sub rrc_is_wcdma_active_sub;
  fn_rrc_get_hsdpa_status_sub rrc_get_hsdpa_status_sub;
  fn_rrcmeas_get_cell_plmn_info_sub rrcmeas_get_cell_plmn_info_sub;
  fn_rrcgps_register_cgps_event_cb_sub rrcgps_register_cgps_event_cb_sub;
  fn_rrcgps_register_cgps_ue_pos_capability_cb_sub rrcgps_register_cgps_ue_pos_capability_cb_sub;
  fn_rrc_return_wcdma_rrc_version_sub rrc_return_wcdma_rrc_version_sub;
  fn_rrc_register_rrc_codec_ho_info_cb_sub rrc_register_rrc_codec_ho_info_cb_sub;
  fn_rrc_deregister_rrc_codec_ho_info_cb_sub rrc_deregister_rrc_codec_ho_info_cb_sub;
  fn_wcdma_set_modem_stats_mask_sub wcdma_set_modem_stats_mask_sub;
  fn_wcdma_get_modem_stats_info_sub wcdma_get_modem_stats_info_sub;
  fn_rrc_register_serving_cell_change_ind_cb_sub rrc_register_serving_cell_change_ind_cb_sub;
  fn_rrc_deregister_serving_cell_change_ind_cb_sub rrc_deregister_serving_cell_change_ind_cb_sub;
  fn_rrc_set_ue_in_deep_sleep_sub rrc_set_ue_in_deep_sleep_sub;
  fn_rrc_reset_mcm_irat_resel_act_cause_sub rrc_reset_mcm_irat_resel_act_cause_sub;
  fn_rrc_put_rr_cmd_sub_id rrc_put_rr_cmd_sub_id;
  fn_wcdma_rrc_get_band_from_dl_uarfcn_sub wcdma_rrc_get_band_from_dl_uarfcn_sub;
  fn_wcdma_rrc_get_wcsg_lfp_info_sub wcdma_rrc_get_wcsg_lfp_info_sub;
  fn_wcdma_derive_freq_from_uarfcn_sub wcdma_derive_freq_from_uarfcn_sub;
  fn_wcdma_convert_uarfcn_sub wcdma_convert_uarfcn_sub;
  fn_wcdma_mfbi_is_supported_sub wcdma_mfbi_is_supported_sub;
  fn_rrcscr_get_signalling_connection_status_sub rrcscr_get_signalling_connection_status_sub;
/*
 * added on 03/10/2015 to provide acqdb info to LTE. 
 */
  fn_wcdma_rrc_get_acqdb_cell_list wcdma_rrc_get_acqdb_cell_list;

  fn_wrrc_get_inter_rat_bplmn_prtl_result wrrc_get_inter_rat_bplmn_prtl_result;
  fn_wrrc_get_inter_rat_bplmn_prtl_result_sub_id wrrc_get_inter_rat_bplmn_prtl_result_sub_id;

  fn_rrc_check_mcc_cnfgd_in_fallback_nv rrc_check_mcc_cnfgd_in_fallback_nv;

/*
 * added on 03/19/2015 for DS customer request
 */
  fn_wl1_start_tput_data_reporting_ind wl1_start_tput_data_reporting_ind;
  fn_wl1_stop_tput_data_reporting_ind wl1_stop_tput_data_reporting_ind;
  fn_wl1_get_hsdpa_stats wl1_get_hsdpa_stats;
  fn_wcdma_ext_audio_api wcdma_ext_audio_api;

/*======================================================================================
  added by sramanna on 05/27/2015 to update WCDMA interface for TxAGC logging
  ======================================================================================*/
  fn_wl1_config_modem_power_stats_tx wl1_config_modem_power_stats_tx;
  fn_wl1_get_modem_power_stats wl1_get_modem_power_stats;

/*
 * added on 07/22/2015 for DS API to estimate UL rate provided by MAC
 */
  fn_wcdma_l2_start_uplink_rate_calculations wcdma_l2_start_uplink_rate_calculations;

/*
 * added on 10/2/2015 for DS downlink quality measurement
 */
  fn_wcdma_qual_estimation_set_report_period wcdma_qual_estimation_set_report_period;
  fn_wcdma_qual_estimation_change_report_state wcdma_qual_estimation_change_report_state;

/*
 * added on 01/07/2016 for DS to trigger TVM forcibly
 */  
  fn_wcdma_rlc_ds_trigger_tvm_forcibly wcdma_rlc_ds_trigger_tvm_forcibly;
  fn_wcdma_rrc_get_3gpp_release_version_sub wcdma_rrc_get_3gpp_release_version_sub;

 /* Added on 12/02/2016 for external Absolute priority support
 */
  fn_rrc_rr_absolute_priority_support rrc_rr_absolute_priority_support;

/*====================================== end of edit ====================================*/
} iWCDMA_vtbl;



/*  The component provides a macro to get the pointer to its vtable from
    a pointer to an interface (defined in dynload.h).
 */
#define WCDMA_vtbl(pi)   ((iWCDMA_vtbl *) ((pi)->pVtable))

#define IWCDMA_l1_cgps_register_event_cb(pi,event_cb_fp) (WCDMA_vtbl(pi))->l1_cgps_register_event_cb(event_cb_fp)

#define IWCDMA_wl1_cgps_register_event_cb(pi,event_cb_fp,wsub_id) (WCDMA_vtbl(pi))->wl1_cgps_register_event_cb(event_cb_fp, wsub_id)

#define IWCDMA_l1_get_cmd_buf(pi)                  (WCDMA_vtbl(pi))->l1_get_cmd_buf()

#define IWCDMA_l1_is_sub_chan_avail_for_asc(pi,asc_idx) (WCDMA_vtbl(pi))->l1_is_sub_chan_avail_for_asc(asc_idx)

#define IWCDMA_l1_put_cmd(pi,cmd_ptr)               (WCDMA_vtbl(pi))->l1_put_cmd(cmd_ptr)

#define IWCDMA_wl1_put_cmd(pi,cmd_ptr,wsub_id)      (WCDMA_vtbl(pi))->wl1_put_cmd(cmd_ptr, wsub_id)

#define IWCDMA_l2_dl_get_cmd_buf(pi)                (WCDMA_vtbl(pi))->l2_dl_get_cmd_buf()

#define IWCDMA_l2_dl_put_cmd(pi,cmd_ptr)            (WCDMA_vtbl(pi))->l2_dl_put_cmd(cmd_ptr)

#define IWCDMA_l2_ul_get_cmd_buf(pi)                (WCDMA_vtbl(pi))->l2_ul_get_cmd_buf()

#define IWCDMA_l2_ul_put_cmd(pi,cmd_ptr)            (WCDMA_vtbl(pi))->l2_ul_put_cmd(cmd_ptr)

#define IWCDMA_mdsp_sync_iq_sample_capture_cmd(pi,buf_idx,wb_chain_index,txc_chain_index,Num_samples,first_trig,last_trig,do_processing,pre_cb,post_cb) \
        (WCDMA_vtbl(pi))->mdsp_sync_iq_sample_capture_cmd(buf_idx,wb_chain_index,txc_chain_index,Num_samples,first_trig,last_trig,do_processing,pre_cb,post_cb)

#define IWCDMA_mdsp_sync_send_iq_cap_cmd(pi,rxlmBufferId,chain,startTime,carrier,numSamples,pre_cb,post_cb) \
        (WCDMA_vtbl(pi))->mdsp_sync_send_iq_cap_cmd(rxlmBufferId,chain,startTime,carrier,numSamples,pre_cb,post_cb)

#define IWCDMA_mdsp_sync_send_mod_timing_cmd(pi,slam,phch,txPnPositioncx8,tti,dl_ref_offset,post_cb,pre_cb) \
        (WCDMA_vtbl(pi))->mdsp_sync_send_mod_timing_cmd(slam,phch,txPnPositioncx8,tti,dl_ref_offset,post_cb,pre_cb)

#define IWDCDMA_mdsp_sync_send_rf_init_cmd(pi,event_id,offset,pre_cb,post_cb) \
        (WCDMA_vtbl(pi))->mdsp_sync_send_rf_init_cmd(event_id,offset,pre_cb,post_cb)

#define IWCDMA_mdsp_sync_send_rxagc_cmd(pi,configure_agc0_carr0,configure_agc0_carr1, \
        configure_agc1_carr0,configure_agc1_carr1buf_idx,buf_idx_rxd,pre_cb,post_cb)   \
        (WCDMA_vtbl(pi))->mdsp_sync_send_rxagc_cmd(configure_agc0_carr0,configure_agc0_carr1, \
        configure_agc1_carr0,configure_agc1_carr1buf_idx,buf_idx_rxd,pre_cb,post_cb)                                                      

#define IWCDMA_mdsp_sync_send_txlm_cfg_cmd(pi,mode_enable,buf_idx,pre_cb,post_cb) \
        (WCDMA_vtbl(pi))->mdsp_sync_send_txlm_cfg_cmd(mode_enable,buf_idx,pre_cb,post_cb)

#define IWCDMA_mdsp_sync_send_ul_dpcch_cfg_cmd(pi,ena,slot_format,uncoded_tfci,pre_cb,post_cb) \
        (WCDMA_vtbl(pi))->mdsp_sync_send_ul_dpcch_cfg_cmd(ena,slot_format,uncoded_tfci,pre_cb,post_cb)

#define IWCDMA_mdsp_send_ftm_idle_mode_w2x_cfg_cmd(pi,rxlm_home_buf_idx,rxlm_home_rxd_buf_idx,mode,pre_cb,post_cb) \
        (WCDMA_vtbl(pi))->mdsp_send_ftm_cm_init_cmd(rxlm_home_buf_idx,rxlm_home_rxd_buf_idx,mode,pre_cb,post_cb)

#define IWCDMA_rlc_dl_deregister_srvc_handler(pi,de_reg_ptr)  (WCDMA_vtbl(pi))->rlc_dl_deregister_srvc_handler(de_reg_ptr)

#define IWCDMA_rlc_dl_register_srvc_handler(pi,reg_ptr) (WCDMA_vtbl(pi))->rlc_dl_register_srvc_handler(reg_ptr)

#define IWCDMA_rlc_ul_deregister_srvc_handler(pi,de_reg_ptr) (WCDMA_vtbl(pi))->rlc_ul_deregister_srvc_handler(de_reg_ptr)

#define IWCDMA_rlc_ul_register_srvc_handler(pi,reg_ptr) (WCDMA_vtbl(pi))->rlc_ul_register_srvc_handler(reg_ptr)

#define IWCDMA_rrc_delete_classmark_change_handover_info(pi) (WCDMA_vtbl(pi))->rrc_delete_classmark_change_handover_info() 

#define IWCDMA_rrc_find_rab_for_rb_in_est_rabs(pi,rb_id)    (WCDMA_vtbl(pi))->rrc_find_rab_for_rb_in_est_rabs(rb_id)

#define IWCDMA_rrc_find_rab_for_rb_in_est_rabs_sub(pi,wsub_id,rb_id)    (WCDMA_vtbl(pi))->rrc_find_rab_for_rb_in_est_rabs_sub(wsub_id,rb_id)

#define IWCDMA_rrc_get_classmark_change_handover_info(pi,msg_ptr,msg_len,irat_ho_info_compressed) \
        (WCDMA_vtbl(pi))->rrc_get_classmark_change_handover_info(msg_ptr,msg_len,irat_ho_info_compressed)

#define IWCDMA_rrc_get_cm_cmd_buf(pi)  (WCDMA_vtbl(pi))-> rrc_get_cm_cmd_buf()

#define IWCDMA_rrc_get_data_rate_for_ds(pi)  (WCDMA_vtbl(pi))->rrc_get_data_rate_for_ds()

#define IWCDMA_rrc_get_l1_cmd_buf(pi)   (WCDMA_vtbl(pi))->rrc_get_l1_cmd_buf()

#define IWCDMA_rrc_get_lsm_cmd_buf(pi)   (WCDMA_vtbl(pi))->rrc_get_lsm_cmd_buf()
#define IWCDMA_rrc_get_mm_cmd_buf(pi)     (WCDMA_vtbl(pi))->rrc_get_mm_cmd_buf()
#define IWCDMA_rrc_get_rr_cmd_buf(pi)    (WCDMA_vtbl(pi))->rrc_get_rr_cmd_buf()
#define IWCDMA_rrc_get_tc_cmd_buf(pi)    (WCDMA_vtbl(pi))->rrc_get_tc_cmd_buf()
#define IWCDMA_rrc_is_arfcn_valid(pi,UARFCN,plmn_id) (WCDMA_vtbl(pi))->rrc_is_arfcn_valid(UARFCN,plmn_id)
#define IWCDMA_rrc_is_wcdma_active(pi)    (WCDMA_vtbl(pi))->rrc_is_wcdma_active()
#define IWCDMA_rrc_malloc_for_external_cmd(pi,size) (WCDMA_vtbl(pi))->rrc_malloc_for_external_cmd(size)
#define IWCDMA_rrc_put_cm_cmd(pi,cmd_buf) (WCDMA_vtbl(pi))->rrc_put_cm_cmd(cmd_buf)
#define IWCDMA_rrc_put_lsm_cmd(pi,cmd_buf) (WCDMA_vtbl(pi))->rrc_put_lsm_cmd(cmd_buf)
#define IWCDMA_rrc_put_mm_cmd(pi,cmd_buf)  (WCDMA_vtbl(pi))->rrc_put_mm_cmd(cmd_buf)
#define IWCDMA_rrc_put_rr_cmd(pi,cmd_buf)  (WCDMA_vtbl(pi))->rrc_put_rr_cmd(cmd_buf)
#define IWCDMA_rrc_put_tc_cmd(pi,cmd_buf)  (WCDMA_vtbl(pi))->rrc_put_tc_cmd(cmd_buf)
#define IWCDMA_rrc_register_rrc_codec_ho_info_cb(pi,cb_ptr)  (WCDMA_vtbl(pi))->rrc_register_rrc_codec_ho_info_cb(cb_ptr)
#define IWCDMA_rrc_register_serving_cell_change_ind_cb(pi,cb_ptr) (WCDMA_vtbl(pi))->rrc_register_serving_cell_change_ind_cb(cb_ptr)
#define IWCDMA_rrc_reset_mcm_irat_resel_act_cause(pi) (WCDMA_vtbl(pi))->rrc_reset_mcm_irat_resel_act_cause()
#define IWCDMA_rrc_return_cs_data_call_parms(pi,rab_id,cs_data_parms_ptr) (WCDMA_vtbl(pi))->rrc_return_cs_data_call_parms(rab_id,cs_data_parms_ptr)
#define IWCDMA_rrc_return_lc_info_for_rab(pi,ptr) (WCDMA_vtbl(pi))->rrc_return_lc_info_for_rab(ptr)
#define IWCDMA_rrc_rr_check_cell_barred(pi,freq,scr_code) (WCDMA_vtbl(pi))->rrc_rr_check_cell_barred(freq,scr_code)
#define IWCDMA_rrc_set_ue_in_deep_sleep(pi,value)   (WCDMA_vtbl(pi))->rrc_set_ue_in_deep_sleep(value)
#define IWCDMA_rrc_update_band_preference(pi,band_pref_mask) (WCDMA_vtbl(pi))->rrc_update_band_preference(band_pref_mask)
#define IWCDMA_rrcgps_register_cgps_event_cb(pi,event_cb_fp) (WCDMA_vtbl(pi))->rrcgps_register_cgps_event_cb(event_cb_fp)
#define IWCDMA_rrcgps_register_cgps_ue_pos_capability_cb(pi,ue_cap_cb_fp) (WCDMA_vtbl(pi))->rrcgps_register_cgps_ue_pos_capability_cb(ue_cap_cb_fp)
#define IWCDMA_rrcmeas_get_cell_plmn_info(pi,plmn_cell_info_ptr)  (WCDMA_vtbl(pi))->rrcmeas_get_cell_plmn_info(plmn_cell_info_ptr)
#define IWCDMA_rrcscr_get_signalling_connection_status(pi,cn_domain_id) (WCDMA_vtbl(pi))->rrcscr_get_signalling_connection_status(cn_domain_id)
#define IWCDMA_rxd_overide_nv_settings(pi,overide_nv_val)  (WCDMA_vtbl(pi))->rxd_overide_nv_settings(overide_nv_val)
#define IWCDMA_rxd_set_active_rx_chain(pi,chain0,chain1)   (WCDMA_vtbl(pi))->rxd_set_active_rx_chain(chain0,chain1)
#define IWCDMA_seq_get_cfn(pi) (WCDMA_vtbl(pi))->seq_get_cfn()
#define IWCDMA_wcdma_get_signal_info(pi) (WCDMA_vtbl(pi))->wcdma_get_signal_info(as_id)
#define IWCDMA_wcdma_l2_dl_get_cmd_buf(pi) (WCDMA_vtbl(pi))->wcdma_l2_dl_get_cmd_buf()
#define IWCDMA_wcdma_l2_dl_put_cmd(pi,cmdptr) (WCDMA_vtbl(pi))->wcdma_l2_dl_put_cmd(cmdptr)
#define IWCDMA_wcdma_l2_ul_get_cmd_buf(pi) (WCDMA_vtbl(pi))->wcdma_l2_ul_get_cmd_buf()
#define IWCDMA_wcdma_l2_ul_put_cmd(pi,cmd_ptr) (WCDMA_vtbl(pi))->wcdma_l2_ul_put_cmd(cmd_ptr)
#define IWCDMA_rlc_query_traffic_time_metrics(pi,sub_ID, rlc_id, delay_type, rlc_ul_traffic_metrics) (WCDMA_vtbl(pi))->rlc_query_traffic_time_metrics( sub_ID, rlc_id, delay_type, rlc_ul_traffic_metrics )
#define IWCDMA_rlc_reset_traffic_time_metrics(pi,sub_ID, rlc_id) (WCDMA_vtbl(pi))->rlc_reset_traffic_time_metrics( sub_ID, rlc_id)
#define IWCDMA_wcdma_query_tx_power_dbm(pi)  (WCDMA_vtbl(pi))->wcdma_query_tx_power_dbm()
#define IWCDMA_wcdma_rrc_get_band_from_dl_uarfcn(pi,uarfcn,camped_plmn_id,is_band_valid) (WCDMA_vtbl(pi))->wcdma_rrc_get_band_from_dl_uarfcn(uarfcn,camped_plmn_id,is_band_valid)
#define IWCDMA_wl1_check_state_for_delayed_gstmr_int(pi) (WCDMA_vtbl(pi))->wl1_check_state_for_delayed_gstmr_int()

/*======================================================================================
  added by hzhi on 09/23/2013 to update WCDMA interface on DIME3.0 for segment loading. 
  ======================================================================================*/
#define IWCDMA_mdsp_sync_send_et_txlm_cfg_cmd(pi,mode_enable,buf_idx,pre_cb,post_cb) (WCDMA_vtbl(pi))->mdsp_sync_send_et_txlm_cfg_cmd(mode_enable,buf_idx,pre_cb,post_cb)
#define IWCDMA_mdsp_sync_send_rxlm_dyn_update_cmd(pi,rxlmBufferId,rxlmDynGroupMask,rxAntennaMask,pre_cb,post_cb) (WCDMA_vtbl(pi))->mdsp_sync_send_rxlm_dyn_update_cmd(rxlmBufferId,rxlmDynGroupMask,rxAntennaMask,pre_cb,post_cb)
#define IWCDMA_mdsp_sync_send_sample_server_cmd(pi,freeze,unfreeze,chain0,chain1,rfPwrUpdateFlag,actionTime,pre_cb,post_cb) (WCDMA_vtbl(pi))->mdsp_sync_send_sample_server_cmd(freeze,unfreeze,chain0,chain1,rfPwrUpdateFlag,actionTime,pre_cb,post_cb)
#define IWCDMA_rrc_gstk_get_utran_nmr_req(pi,num_cells,report_type,ref_id,as_id) (WCDMA_vtbl(pi))->rrc_gstk_get_utran_nmr_req(num_cells,report_type,ref_id,as_id)
#define IWCDMA_wcdma_get_modem_stats_info(pi,wcdma_info) (WCDMA_vtbl(pi))->wcdma_get_modem_stats_info(wcdma_info)
#define IWCDMA_wcdma_set_modem_stats_mask(pi,bit_mask) (WCDMA_vtbl(pi))->wcdma_set_modem_stats_mask(bit_mask)
#define IWCDMA_wmcpmdrv_rf_ftm_mode_req(pi,request) (WCDMA_vtbl(pi))->wmcpmdrv_rf_ftm_mode_req(request)
#define IWCDMA_wcdma_start_capture_scan_data(pi,wcdma_scan_data,wcdma_scan_data_mem_size) (WCDMA_vtbl(pi))->wcdma_start_capture_scan_data(wcdma_scan_data,wcdma_scan_data_mem_size)
#define IWCDMA_wcdma_stop_capture_scan_data(pi) (WCDMA_vtbl(pi))->wcdma_stop_capture_scan_data()
/*
 added by hzhi on 10/17/2013 to support new FEATURE_DUAL_SIM on DIME3.0. 
 */
#define IWCDMA_rrc_return_wcdma_rrc_version(pi) (WCDMA_vtbl(pi))->rrc_return_wcdma_rrc_version()
#define IWCDMA_rrc_delete_classmark_change_handover_info_sub_id(pi,as_id) (WCDMA_vtbl(pi))->rrc_delete_classmark_change_handover_info_sub_id(as_id)
#define IWCDMA_rrc_get_classmark_change_handover_info_sub_id(pi,msg_ptr,msg_len,irat_ho_info_compressed,as_id) (WCDMA_vtbl(pi))->rrc_get_classmark_change_handover_info_sub_id(msg_ptr,msg_len,irat_ho_info_compressed,as_id)
#define IWCDMA_rrc_is_arfcn_valid_sub_id(pi,UARFCN,plmn_id,as_id) (WCDMA_vtbl(pi))->rrc_is_arfcn_valid_sub_id(UARFCN,plmn_id,as_id)
#define IWCDMA_rrc_rr_check_cell_barred_sub_id(pi,freq,scr_code,as_id) (WCDMA_vtbl(pi))->rrc_rr_check_cell_barred_sub_id(freq,scr_code,as_id)
#define IWCDMA_rrc_set_ue_in_deep_sleep_sub_id(pi,value,as_id) (WCDMA_vtbl(pi))->rrc_set_ue_in_deep_sleep_sub_id(value,as_id)
#define IWCDMA_rrc_update_band_preference_sub_id(pi,band_pref_mask,as_id) (WCDMA_vtbl(pi))->rrc_update_band_preference_sub_id(band_pref_mask,as_id)
#define IWCDMA_wl1trm_can_gsm_disable_fw_app_wcdma(pi) (WCDMA_vtbl(pi))->wl1trm_can_gsm_disable_fw_app_wcdma()
#define IWCDMA_wl1trm_allow_wcdma_fw_app_disable(pi, as_id) (WCDMA_vtbl(pi))->wl1trm_allow_wcdma_fw_app_disable(as_id)
#define IWCDMA_wcdma_rrc_get_wcsg_lfp_info(pi,l_macro_ptr,w_csg_cells_ptr) (WCDMA_vtbl(pi))->wcdma_rrc_get_wcsg_lfp_info(l_macro_ptr,w_csg_cells_ptr)

/*
 * added on 01/07/2014
 */
#define IWCDMA_wl1_eul_stats(pi, eul_stats) \
        (WCDMA_vtbl(pi))->wl1_eul_stats(eul_stats)
#define IWCDMA_wl1_query_modem_tx_power(pi) \
        (WCDMA_vtbl(pi))->wl1_query_modem_tx_power()
#define IWCDMA_wcdma_convert_uarfcn(pi, source_uarfcn, band_num, target_uarfcn, camped_plmn_id) \
        (WCDMA_vtbl(pi))->wcdma_convert_uarfcn(source_uarfcn, band_num, target_uarfcn, camped_plmn_id)
#define IWCDMA_wcdma_derive_freq_from_uarfcn(pi, uarfcn, camped_plmn_id, freq) \
        (WCDMA_vtbl(pi))->wcdma_derive_freq_from_uarfcn(uarfcn, camped_plmn_id, freq)

/*
 * added on 02/25/2014 for MCS customer request. 
 */
#define IWCDMA_wcdma_get_rat_meas_info(pi, wcdma_signal_info ,wsub_id) \
        (WCDMA_vtbl(pi))->wcdma_get_rat_meas_info(wcdma_signal_info ,wsub_id)
#define IWCDMA_wcdma_is_rat_meas_info_ready(pi,wsub_id) \
        (WCDMA_vtbl(pi))->wcdma_is_rat_meas_info_ready(wsub_id)
/* added on 02/25/2014 for MCS customer request. */
/* added on 02/19/2015*/
#define IWCDMA_rrc_check_if_nw_support_fd(pi,wrrc_as_id) (WCDMA_vtbl(pi))->rrc_check_if_nw_support_fd(wrrc_as_id)
#define IWCDMA_rrc_ext_get_wcdma_hsupa_category(pi,wrrc_as_id) (WCDMA_vtbl(pi))->rrc_ext_get_wcdma_hsupa_category(wrrc_as_id)

#define IWCDMA_rrc_put_mm_cmd_sub(pi,wrrc_as_id,cmd_buf) (WCDMA_vtbl(pi))->rrc_put_mm_cmd_sub(wrrc_as_id,cmd_buf)
#define IWCDMA_rrc_put_tc_cmd_sub(pi,wrrc_as_id,cmd_buf) (WCDMA_vtbl(pi))->rrc_put_tc_cmd_sub(wrrc_as_id,cmd_buf)
#define IWCDMA_rrc_put_lsm_cmd_sub(pi,wrrc_as_id,cmd_buf) (WCDMA_vtbl(pi))->rrc_put_lsm_cmd_sub(wrrc_as_id,cmd_buf)
#define IWCDMA_rrc_put_cm_cmd_sub(pi,wrrc_as_id,cmd_buf) (WCDMA_vtbl(pi))->rrc_put_cm_cmd_sub(wrrc_as_id,cmd_buf)
#define IWCDMA_rrc_return_lc_info_for_rab_sub(pi,wrrc_as_id,ptr) (WCDMA_vtbl(pi))->rrc_return_lc_info_for_rab_sub(wrrc_as_id,ptr)
#define IWCDMA_rrc_return_cs_data_call_parms_sub(pi,wrrc_as_id,rab_id,cs_data_parms_ptr) (WCDMA_vtbl(pi))->rrc_return_cs_data_call_parms_sub(wrrc_as_id,rab_id,cs_data_parms_ptr)
#define IWCDMA_rrc_is_wcdma_active_sub(pi,wrrc_as_id) (WCDMA_vtbl(pi))->rrc_is_wcdma_active_sub(wrrc_as_id)
#define IWCDMA_rrc_get_hsdpa_status_sub(pi,wrrc_as_id) (WCDMA_vtbl(pi))->rrc_get_hsdpa_status_sub(wrrc_as_id)
#define IWCDMA_rrcmeas_get_cell_plmn_info_sub(pi,wrrc_as_id,plmn_cell_info_ptr) (WCDMA_vtbl(pi))->rrcmeas_get_cell_plmn_info_sub(wrrc_as_id,plmn_cell_info_ptr)
#define IWCDMA_rrcgps_register_cgps_event_cb_sub(pi,wrrc_as_id,event_cb_fp) (WCDMA_vtbl(pi))->rrcgps_register_cgps_event_cb_sub(wrrc_as_id,event_cb_fp)
#define IWCDMA_rrcgps_register_cgps_ue_pos_capability_cb_sub(pi,wrrc_as_id,ue_cap_cb_fp) (WCDMA_vtbl(pi))->rrcgps_register_cgps_ue_pos_capability_cb_sub(wrrc_as_id,ue_cap_cb_fp)
#define IWCDMA_rrc_return_wcdma_rrc_version_sub(pi,wrrc_as_id) (WCDMA_vtbl(pi))->rrc_return_wcdma_rrc_version_sub(wrrc_as_id)
#define IWCDMA_rrc_register_rrc_codec_ho_info_cb_sub(pi,wrrc_as_id,cb_ptr) (WCDMA_vtbl(pi))->rrc_register_rrc_codec_ho_info_cb_sub(wrrc_as_id,cb_ptr)
#define IWCDMA_rrc_deregister_rrc_codec_ho_info_cb_sub(pi,wrrc_as_id) (WCDMA_vtbl(pi))->rrc_deregister_rrc_codec_ho_info_cb_sub(wrrc_as_id)
#define IWCDMA_wcdma_set_modem_stats_mask_sub(pi,wrrc_as_id,bit_mask) (WCDMA_vtbl(pi))->wcdma_set_modem_stats_mask_sub(wrrc_as_id,bit_mask)
#define IWCDMA_wcdma_get_modem_stats_info_sub(pi,rrc_as_id,wcdma_info) (WCDMA_vtbl(pi))->wcdma_get_modem_stats_info_sub(rrc_as_id,wcdma_info)
#define IWCDMA_rrc_register_serving_cell_change_ind_cb_sub(pi,wrrc_as_id,cb_ptr) (WCDMA_vtbl(pi))->rrc_register_serving_cell_change_ind_cb_sub(wrrc_as_id,cb_ptr)
#define IWCDMA_rrc_deregister_serving_cell_change_ind_cb_sub(pi,wrrc_as_id,cb_ptr) (WCDMA_vtbl(pi))->rrc_deregister_serving_cell_change_ind_cb_sub(wrrc_as_id,cb_ptr)
#define IWCDMA_rrc_set_ue_in_deep_sleep_sub(pi,wrrc_as_id,value) (WCDMA_vtbl(pi))->rrc_set_ue_in_deep_sleep_sub(wrrc_as_id,value)
#define IWCDMA_rrc_reset_mcm_irat_resel_act_cause_sub(pi,wrrc_as_id) (WCDMA_vtbl(pi))->rrc_reset_mcm_irat_resel_act_cause_sub(wrrc_as_id)
#define IWCDMA_rrc_put_rr_cmd_sub_id(pi,cmd_buf,as_id) (WCDMA_vtbl(pi))->rrc_put_rr_cmd_sub_id(cmd_buf,as_id)
#define IWCDMA_wcdma_rrc_get_band_from_dl_uarfcn_sub(pi,wrrc_as_id,uarfcn,camped_plmn_id,is_band_valid) (WCDMA_vtbl(pi))->wcdma_rrc_get_band_from_dl_uarfcn_sub(wrrc_as_id,uarfcn,camped_plmn_id,is_band_valid)
#define IWCDMA_wcdma_rrc_get_wcsg_lfp_info_sub(pi,wrrc_as_id,l_macro_ptr,w_csg_cells_ptr) (WCDMA_vtbl(pi))->wcdma_rrc_get_wcsg_lfp_info_sub(wrrc_as_id,l_macro_ptr,w_csg_cells_ptr)
#define IWCDMA_wcdma_derive_freq_from_uarfcn_sub(pi,wrrc_as_id,uarfcn,camped_plmn_id,freq) (WCDMA_vtbl(pi))->wcdma_derive_freq_from_uarfcn_sub(wrrc_as_id,uarfcn,camped_plmn_id,freq)
#define IWCDMA_wcdma_convert_uarfcn_sub(pi,wrrc_as_id,source_uarfcn,band_num,target_uarfcn,camped_plmn_id) (WCDMA_vtbl(pi))->wcdma_convert_uarfcn_sub(wrrc_as_id,source_uarfcn,band_num,target_uarfcn,camped_plmn_id)
#define IWCDMA_wcdma_mfbi_is_supported_sub(pi,wrrc_as_id) (WCDMA_vtbl(pi))->wcdma_mfbi_is_supported_sub(wrrc_as_id)
#define IWCDMA_rrcscr_get_signalling_connection_status_sub(pi,wrrc_as_id,cn_domain_id) (WCDMA_vtbl(pi))->rrcscr_get_signalling_connection_status_sub(wrrc_as_id,cn_domain_id)

#define IWCDMA_wcdma_rrc_get_acqdb_cell_list(pi,wrrc_as_id,wcdma_acqdb_cell_list_ptr) (WCDMA_vtbl(pi))->wcdma_rrc_get_acqdb_cell_list(wrrc_as_id,wcdma_acqdb_cell_list_ptr)

#define IWCDMA_wrrc_get_inter_rat_bplmn_prtl_result(pi,available_plmn_list) (WCDMA_vtbl(pi))->wrrc_get_inter_rat_bplmn_prtl_result(available_plmn_list)
#define IWCDMA_wrrc_get_inter_rat_bplmn_prtl_result_sub_id(pi,available_plmn_list,as_id) (WCDMA_vtbl(pi))->wrrc_get_inter_rat_bplmn_prtl_result_sub_id(available_plmn_list,as_id)
#define IWCDMA_wcdma_rrc_get_3gpp_release_version_sub(pi,as_id) (WCDMA_vtbl(pi))->wcdma_rrc_get_3gpp_release_version_sub(wrrc_as_id)
#define IWCDMA_rrc_check_mcc_cnfgd_in_fallback_nv(pi,camped_mcc,as_id) (WCDMA_vtbl(pi))->rrc_check_mcc_cnfgd_in_fallback_nv(camped_mcc,as_id)

/*
 * added on 03/19/2015 for DS customer request
 */
#define IWCDMA_wl1_start_tput_data_reporting_ind(pi) \
        (WCDMA_vtbl(pi))->wl1_start_tput_data_reporting_ind()
#define IWCDMA_wl1_stop_tput_data_reporting_ind(pi) \
        (WCDMA_vtbl(pi))->wl1_stop_tput_data_reporting_ind()
#define IWCDMA_wl1_get_hsdpa_stats(pi, hsdpa_stats) \
        (WCDMA_vtbl(pi))->wl1_get_hsdpa_stats(hsdpa_stats)
#define IWCDMA_wcdma_ext_audio_api(pi, cmd_id, param, size) \
        (WCDMA_vtbl(pi))->wcdma_ext_audio_api(cmd_id, param, size)

/*======================================================================================
  added by sramanna on 05/27/2015 to update WCDMA interface for TxAGC logging
  ======================================================================================*/
#define IWCDMA_wl1_config_modem_power_stats_tx(pi, start_stop) (WCDMA_vtbl(pi))->wl1_config_modem_power_stats_tx(start_stop)
#define IWCDMA_wl1_get_modem_power_stats(pi, tx_power_bin) (WCDMA_vtbl(pi))->wl1_get_modem_power_stats(tx_power_bin)

#define IWCDMA_wcdma_l2_start_uplink_rate_calculations(pi, as_id, enable, interval)\
        (WCDMA_vtbl(pi)->wcdma_l2_start_uplink_rate_calculations(as_id, enable, interval))

/*
 * added on 10/2/2015 for DS downlink quality measurement
 */
#define IWCDMA_wcdma_qual_estimation_set_report_period(pi, report_period, as_id) (WCDMA_vtbl(pi))->wcdma_qual_estimation_set_report_period(report_period, as_id)
#define IWCDMA_wcdma_qual_estimation_change_report_state(pi, state, as_id) (WCDMA_vtbl(pi))->wcdma_qual_estimation_change_report_state(state, as_id)

/*
 * added on 01/07/2016 for DS to trigger TVM forcibly
 */  
#define IWCDMA_wcmda_rlc_ds_trigger_tvm_forcibly(pi,sub_id,reason) \
        (WCDMA_vtbl(pi))->wcdma_mac_ds_trigger_tvm_forcibly(sub_id, reason)

/*
 * Added on 12/02/2016 for external Absolute priority support
 */
#define IWCDMA_rrc_rr_absolute_priority_support(pi, as_id) (WCDMA_vtbl(pi))->rrc_rr_absolute_priority_support(as_id)
/*====================================== end of edit ====================================*/

#endif /* _IWCDMA_H_ */