/**
  @file wcdma_interface.c

  @brief  
*/
/*
  Copyright (c) 2013 - 2016 Qualcomm Technologies, Inc.
          All Rights Reserved.
    Qualcomm Confidential and Proprietary
*/

/*===========================================================================
                      EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/wcdma.mpss/8.4.0/common/src/wcdma_interface.c#2 $
  $DateTime: 2020/01/22 08:56:27 $


  when        who     what, where, why
  ---------   ---     ------------------------------------------------------------
12/15/16     ha       Made changes for WRRC supports priority based reselection towards GERAN
07/03/16      sp      Made changes to provide an API which returns WCDMA release version
04/12/16      kv      FR 32607: Web Boosting to improve latency by initiating TVM so that NW can move UE to DCH
02/04/16      nr      Made changes to expose rrc_find_rab_for_rb_in_est_rabs_sub API
01/05/16      ms      Fixed the entries in wcdma_mmcp_vtbl
12/21/15      ar      Add interface to check if WFW APP can be disabled
12/24/15      svh     Removed CTCH SS APIs.
12/15/15      om      FSL - fix WCDMA interface entry which causes compilation error
11/03/15      rn      Added changes for removal of HARDCODE_SUB_ID. 
10/02/15      ac      FR 26288: DS downlink quality measurement reporting
09/29/15      nr      Made changes inorder for TC to make use of sub specific WRRC api
07/27/15      sp      Made changes to cleanup unused voice interfaces.
07/22/15      kv      FR 22377: RAVE-API
06/20/15      ac      Remove unneeded THOR featurization around rlc_query_traffic_time_metrics/rlc_reset_traffic_time_metrics
06/17/15      ac      Added MSIM version of MMCP CTCH APIs
05/26/15      sr      Initial checkin for FR 26957 - TxAGC logging
04/29/15      scv     FR 25842: RLC RTT & Queuing Delay Computation
04/10/15      cc      Remove an unused API
04/08/15      sa      Made changes to support FR25951_SingleVoiceSession
04/02/15      geg     Added new API for FR 16887: Display of Partial PLMN Search Results during Manual Search 
03/19/15      ac      FR 25733: UL/DL throughput reporting 
03/13/15      aa      Made changes to clean up QCHAT specific APIs
03/10/15      sas     Added api interface fun to provide acqdb cell list info to LTE.
02/28/15      rsr     W+W Phase 2 Checkin.
02/19/15      ks      Add wl1_x2w_srch_init_g2w_crit_sect
02/19/15      geg     Added new API for FR 23226: WRRC changes for inclusion of AS ID as a new argument to functions 
02/19/15      sas     Made changes to add a new API which takes as_id as agrument and calls rrcdormancy_check_if_nw_support_fast_dormancy() 
05/28/14      geg     Mainlined feature FEATURE_WCDMA_GET_RAT_MEAS_INFO
04/28/14      sp      Made changes to add rrc_vocoder_control_enabled_sub for DUAL SIM cases
04/23/14      geg     Fix a compilation error that occured after enabling segment loading feature
04/14/14      cc      Create an API for RF to call idle W2X search cfg to WFW in FTM mode
01/08/14      geg     Segment Loading - WCDMA interface updates for DIME3.0 
11/26/13      geg     Segment loading FSL: add dummy task 
10/17/13      hzhi    Support new FEATURE_DUAL_SIM on DIME3.0. 
10/09/13      geg     Clean up WCDMA private API from interface inclusion chain for Feature Segment Loading DIME 3.0
2013/10/01    geg     Updated WCDMA interface for segment loading on DIME 3.0.
2013/05/10    hzhi    Added file hearder according to qualcomm standards.
2013/01/01    ngeng   Created for segment loading.

===========================================================================*/

#include "comdef.h"
#include "customer.h"
#include "target.h"
#include "sys_type.h"
#include "sys.h"

#ifdef FEATURE_SEGMENT_LOADING
/* for wcdma_interface.c */
#include "wl1drxmanager.h"
#include "drx.h"
#include "wl1m.h"
#include "l1msetup.h"
#include "rrcccm.h"
#include "rrcscmgr.h"
#include "rrclcm.h"
#include "wcdma_rrc_api.h"
#include "wl1sleep.h"
#include "rrcrrif.h"
#include "rrccmd.h"
#include "rrcdata.h"
#include "rrclsmif.h"
#include "rrcmmif.h"
#include "IWCDMA.h"
#include "IWCDMA_ext.h"
#include "IWCDMA_v2.h"
#include "wsrch.h"
#include "wl1x2wsrchapi.h"
#include "rrccu.h"	
#include "rrccui.h"	
#include "rrctmr.h"
#include "wcdma_ext_api.h"
iWCDMA_vtbl  wcdma_vtbl = 
{
  NULL,
  NULL,
  NULL,
  l1_cgps_register_event_cb,
  wl1_cgps_register_event_cb,
  l1_get_cmd_buf,
  l1_is_sub_chan_avail_for_asc,
  l1_put_cmd,
  wl1_put_cmd,
  l2_dl_get_cmd_buf,
  l2_dl_put_cmd,
  l2_ul_get_cmd_buf,
  l2_ul_put_cmd,
  mdsp_sync_iq_sample_capture_cmd,
  mdsp_sync_send_iq_cap_cmd,
  mdsp_sync_send_mod_timing_cmd,
  mdsp_sync_send_rf_init_cmd,
  mdsp_sync_send_rxagc_cmd,
  mdsp_sync_send_txlm_cfg_cmd,
  mdsp_sync_send_ul_dpcch_cfg_cmd,
  mdsp_send_ftm_idle_mode_w2x_cfg_cmd,
  rlc_dl_deregister_srvc_handler,
  rlc_dl_register_srvc_handler,
  rlc_ul_deregister_srvc_handler,
  rlc_ul_register_srvc_handler,
  rrc_delete_classmark_change_handover_info,
  rrc_find_rab_for_rb_in_est_rabs,
  rrc_find_rab_for_rb_in_est_rabs_sub,
  rrc_get_classmark_change_handover_info,
  rrc_get_cm_cmd_buf,
  rrc_get_data_rate_for_ds,
  rrc_get_l1_cmd_buf,
  rrc_get_lsm_cmd_buf,
  rrc_get_mm_cmd_buf,
  rrc_get_rr_cmd_buf,
  rrc_get_tc_cmd_buf,
  rrc_is_arfcn_valid,
  rrc_is_wcdma_active,
  rrc_malloc_for_external_cmd,
  rrc_put_cm_cmd,
  rrc_put_lsm_cmd,
  rrc_put_mm_cmd,
  rrc_put_rr_cmd,
  rrc_put_tc_cmd,
  rrc_register_rrc_codec_ho_info_cb,
  rrc_register_serving_cell_change_ind_cb,
  rrc_reset_mcm_irat_resel_act_cause,
  rrc_return_cs_data_call_parms,
  rrc_return_lc_info_for_rab,
  rrc_rr_check_cell_barred,
  rrc_set_ue_in_deep_sleep,
  rrc_update_band_preference,
  rrcgps_register_cgps_event_cb,
  rrcgps_register_cgps_ue_pos_capability_cb,
  rrcmeas_get_cell_plmn_info,
  rrcscr_get_signalling_connection_status,
  rxd_overide_nv_settings,
  rxd_set_active_rx_chain,
  seq_get_cfn,
  wcdma_get_signal_info,
  wcdma_l2_dl_get_cmd_buf,
  wcdma_l2_dl_put_cmd,
  wcdma_l2_ul_get_cmd_buf,
  wcdma_l2_ul_put_cmd,
  rlc_query_traffic_time_metrics,
  rlc_reset_traffic_time_metrics,
  wcdma_query_tx_power_dbm,
  wcdma_rrc_get_band_from_dl_uarfcn,
  NULL,
/*======================================================================================
  added by hzhi on 09/23/2013 to update WCDMA interface on DIME3.0 for segment loading. 
  ======================================================================================*/
  mdsp_sync_send_et_txlm_cfg_cmd,
  mdsp_sync_send_rxlm_dyn_update_cmd,
  mdsp_sync_send_sample_server_cmd,
  rrc_gstk_get_utran_nmr_req,
  wcdma_get_modem_stats_info,
  wcdma_set_modem_stats_mask,
  wmcpmdrv_rf_ftm_mode_req, 
  wcdma_start_capture_scan_data,
  wcdma_stop_capture_scan_data,
/*
 added by hzhi on 10/17/2013 to support new FEATURE_DUAL_SIM on DIME3.0.
 */
  rrc_return_wcdma_rrc_version,
#ifdef FEATURE_DUAL_SIM
  rrc_delete_classmark_change_handover_info_sub_id,
  rrc_get_classmark_change_handover_info_sub_id,
  rrc_is_arfcn_valid_sub_id,
  rrc_rr_check_cell_barred_sub_id,
  rrc_set_ue_in_deep_sleep_sub_id,
#else /*FEATURE_DUAL_SIM*/
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
#endif/*FEATURE_DUAL_SIM*/
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  rrc_update_band_preference_sub_id,
  wl1trm_can_gsm_disable_fw_app_wcdma,
#else /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
  NULL,
  NULL,
#endif/* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
  wl1trm_allow_wcdma_fw_app_disable,
  wcdma_rrc_get_wcsg_lfp_info,
/*
 * added on 01/07/2014
 */
  wl1_eul_stats,
  wl1_query_modem_tx_power,
  wcdma_convert_uarfcn,
  wcdma_derive_freq_from_uarfcn,
/*
 * added on 02/25/2014 for MCS customer request. 
 */
  wcdma_get_rat_meas_info,
  wcdma_is_rat_meas_info_ready,
/*
 * added on 02/19/2015
 */
    rrc_check_if_nw_support_fd,
  	rrc_ext_get_wcdma_hsupa_category,

    rrc_put_mm_cmd_sub,
    rrc_put_tc_cmd_sub,
    rrc_put_lsm_cmd_sub,
    rrc_put_cm_cmd_sub,
    rrc_return_lc_info_for_rab_sub,
    rrc_return_cs_data_call_parms_sub,
    rrc_is_wcdma_active_sub,
    rrc_get_hsdpa_status_sub,
    rrcmeas_get_cell_plmn_info_sub,
    rrcgps_register_cgps_event_cb_sub,
    rrcgps_register_cgps_ue_pos_capability_cb_sub,
    rrc_return_wcdma_rrc_version_sub,
    rrc_register_rrc_codec_ho_info_cb_sub,
    rrc_deregister_rrc_codec_ho_info_cb_sub,
    wcdma_set_modem_stats_mask_sub,
    wcdma_get_modem_stats_info_sub,
    rrc_register_serving_cell_change_ind_cb_sub,
    rrc_deregister_serving_cell_change_ind_cb_sub,
    rrc_set_ue_in_deep_sleep_sub,
    rrc_reset_mcm_irat_resel_act_cause_sub,
    rrc_put_rr_cmd_sub_id,
    wcdma_rrc_get_band_from_dl_uarfcn_sub,
    wcdma_rrc_get_wcsg_lfp_info_sub,
    wcdma_derive_freq_from_uarfcn_sub,
    wcdma_convert_uarfcn_sub,
    wcdma_mfbi_is_supported_sub,
    rrcscr_get_signalling_connection_status_sub,
/*
 * added on 03/10/2015 to provide acqdb info to LTE. 
 */
  wcdma_rrc_get_acqdb_cell_list,

  wrrc_get_inter_rat_bplmn_prtl_result,
  wrrc_get_inter_rat_bplmn_prtl_result_sub_id,
  rrc_check_mcc_cnfgd_in_fallback_nv,

/*
 * added on 03/19/2015 
 */
    wl1_start_tput_data_reporting_ind,
    wl1_stop_tput_data_reporting_ind,
    wl1_get_hsdpa_stats,
    wcdma_ext_audio_api,

/*======================================================================================
  added by sramanna on 05/27/2015 to update WCDMA interface for TxAGC logging
  ======================================================================================*/
    wl1_config_modem_power_stats_tx,
    wl1_get_modem_power_stats,

/*
 * added on 07/22/2015 
 */
 #ifdef FEATURE_DATA_RAVE_SUPPORT 
    wcdma_l2_start_uplink_rate_calculations,
 #else
    NULL,
 #endif

/*
 * added on 10/2/2015 for DS downlink quality measurement
 */
  wcdma_qual_estimation_set_report_period,
  wcdma_qual_estimation_change_report_state,
/*
 * added on 01/07/2016 for DS to forcibly trigger TVM
 */
#ifdef FEATURE_WCDMA_DS_WEB_BOOST  
  wcdma_mac_ds_trigger_tvm_forcibly, 
#else
  NULL,
#endif

  wcdma_rrc_get_3gpp_release_version_sub,
/*Added on 11/30/16 WRRC supports priority based reselection towards GERAN */
   rrc_rr_absolute_priority_support
/*====================================== end of edit ====================================*/
};

iWCDMA_mmcp_vtbl  wcdma_mmcp_vtbl = 
{
  NULL,
  NULL,
  NULL,
  dl_dec_deregister_ctch_pdu_decode_error_cb_msim,
  dl_dec_register_ctch_pdu_decode_error_cb_msim
};

iWCDMA_rftech_vtbl  wcdma_rftech_vtbl = 
{
  NULL,
  NULL,
  NULL
};

#ifdef FEATURE_QCHAT
iWCDMA_qchat_vtbl  wcdma_qchat_vtbl = 
{
  NULL,
  NULL,
  NULL,
  drx_manager_wake_from_sleep,
  drx_sleep_abort,
  l1m_setup_next_cmd,
  rrc_ccm_get_serv_plmn_cellid,
  rrc_get_int_cmd_buf,
  rrc_get_state,
  rrc_put_int_cmd,
#ifdef FEATURE_HSPA_CALL_STATUS_IND
  rrc_send_hspa_call_status_info,
#else /*FEATURE_HSPA_CALL_STATUS_IND*/
  NULL,
#endif /* FEATURE_HSPA_CALL_STATUS_IND */
  rrccu_clear_procedure,
  rrccu_convert_t302_to_ms,
  rrccu_return_cu_substate,
  rrccu_send_cell_update_initiate_req,
  rrccu_send_cell_update_msg,
  rrccu_set_cu_substate,
  rrclcm_check_ul_rlc_lc_id,
  rrctmr_get_remaining_time,
  rrctmr_start_timer,
  wl1_send_msg_to_controller,
  sleep_abort,
  l1_free_cmd_buf
};
#endif

/*==========================================================================================*/

void wcdma_interface_dtor(void  *pMe)
{
  if(!pMe){
    return;
  }
  rrc_free(pMe);
  
}


interface_t * wcdma_interface_ctor(void)
{
	interface_t *pi = (interface_t *) rrc_malloc(sizeof(interface_t));
  if(!pi)
  {
    return NULL;
  }
  ref_cnt_obj_init(pi, wcdma_interface_dtor);

  pi->pVtable = (iunknown_t *) &wcdma_vtbl;

  return pi;
}


void wcdma_interface_mmcp_dtor(void  *pMe)
{
  if(!pMe){
    return;
  }
  rrc_free(pMe);
}

interface_t * wcdma_interface_mmcp_ctor(void)
{
#ifndef FEATURE_UMTS_BMC
  return NULL;
#endif /* FEATURE_UMTS_BMC */
	interface_t *pi = (interface_t *) rrc_malloc(sizeof(interface_t));
  if(!pi)
  {
    return NULL;
  }
  ref_cnt_obj_init(pi, wcdma_interface_mmcp_dtor);

  pi->pVtable = (iunknown_t *) &wcdma_mmcp_vtbl;

  return pi;
}


void wcdma_interface_rftech_dtor(void  *pMe)
{
  if(!pMe){
    return;
  }
  rrc_free(pMe);
}

interface_t * wcdma_interface_rftech_ctor(void)
{
  interface_t *pi = (interface_t *) rrc_malloc(sizeof(interface_t));
  if(!pi)
  {
    return NULL;
  }
  ref_cnt_obj_init(pi, wcdma_interface_rftech_dtor);

  pi->pVtable = (iunknown_t *) &wcdma_rftech_vtbl;

  return pi;
}


void wcdma_interface_qchat_dtor(void  *pMe)
{
  if(!pMe){
    return;
  }
  rrc_free(pMe);
}

interface_t * wcdma_interface_qchat_ctor(void)
{
	interface_t *pi = (interface_t *) rrc_malloc(sizeof(interface_t));
  if(!pi)
  {
    return NULL;
  }
  ref_cnt_obj_init(pi, wcdma_interface_qchat_dtor);
#ifdef FEATURE_QCHAT
  pi->pVtable = (iunknown_t *) &wcdma_qchat_vtbl;
#endif
  return pi;
}

void wcdma_task_null(void) __attribute__((section(".task_text")));
void wcdma_task_null(void)
{
#ifndef FEATURE_MODEM_RCINIT
  rex_sigs_type      rex_signals_mask;  /* Mask of signals returned by rex */
  /* Send an ACK and wait for Task Start signal */
  tmc_task_start();
#else /*FEATURE_MODEM_RCINIT*/
  /* Send Handshake to RCINIT */
  rcinit_handshake_startup();
#endif /* FEATURE_MODEM_RCINIT */


  for ( ;; )
  { /* Never exit this loop ... */
#ifndef FEATURE_MODEM_RCINIT
    rex_signals_mask = rex_wait(
                                 TASK_OFFLINE_SIG   |
                                 TASK_STOP_SIG
                                 );

    if ((rex_signals_mask & TASK_STOP_SIG) != 0)
    {
      (void) rex_clr_sigs( rex_self(), TASK_STOP_SIG );
  //    (void) rex_set_sigs( &tmc_tcb, TMC_ACK_SIG );
    }

    if ((rex_signals_mask & TASK_OFFLINE_SIG) != 0)
    {
      (void) rex_clr_sigs( rex_self(), TASK_OFFLINE_SIG );
  //    (void) rex_set_sigs( &tmc_tcb, TMC_ACK_SIG );
    }

#else /* FEATURE_MODEM_RCINIT */
    rex_wait(
             TASK_OFFLINE_SIG   |
             TASK_STOP_SIG
            );

#endif /* FEATURE_MODEM_RCINIT */

  } /* end for */
}

#endif /* FEATURE_SEGMENT_LOADING */
