/**
  @file tdscdma_interface.c

  @brief  
*/
/*
  Copyright (c) 2013 QUALCOMM Technologies Incorporated.
          All Rights Reserved.
    Qualcomm Confidential and Proprietary
*/

/*===========================================================================
                      EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdscdma_interface.c#1 $
  $DateTime: 2019/08/19 10:51:23 $


  when        who     what, where, why
  ---------   ---     ------------------------------------------------------------
 2015/06/03   lhu     update vtable for TX pwr statistics for Data Service
 2013/09/20   hzhi    Updated vtable for segment loading on DIME3.0. 
 2013/05/10   hzhi    Added file hearder according to qualcomm standards.
 2013/01/01   ngeng   Created for segment loading.

===========================================================================*/
/*
#define TL1_ARFCN_T ARFCN_T
*/

#include "comdef.h"
#include "customer.h"
#include "target.h"
#include "sys_type.h"
#include "sys.h"

#ifdef FEATURE_SEGMENT_LOADING

//#include "tds_rrc_api.h"
#include "ITDSCDMA.h"
//#define TL1_ARFCN_T ARFCN_T
#include "tdscdma_variation.h"

iTDSCDMA_vtbl tdscdma_vtbl = 
{
		NULL,
		NULL,
		NULL,
		rrc_is_tds_active,
#ifdef FEATURE_DUAL_WCDMA
    rrc_is_tds_active_sub,
#else
    NULL,
#endif
		tds_rrc_get_band_from_dl_uarfcn,
		tds_rrc_rat_is_supported,
		tdsl2_dl_service_register,
#ifdef FEATURE_DUAL_WCDMA
    tdsl2_dl_service_register_sub,
#else
    NULL,
#endif

		tdsl2_dl_submit_datamodem_msg,
#ifdef FEATURE_DUAL_WCDMA
    tdsl2_dl_submit_datamodem_msg_sub,
#else
    NULL,
#endif
		tdsl2_ul_service_register,
#ifdef FEATURE_DUAL_WCDMA
    tdsl2_ul_service_register_sub,
#else
        NULL,
#endif
		tdsl2_ul_submit_datamodem_msg,
#ifdef FEATURE_DUAL_WCDMA
    tdsl2_ul_submit_datamodem_msg_sub,
#else
    NULL,
#endif
		tdsrrc_check_cell_barred,
		tdsrrc_delete_classmark_change_handover_info,
		tdsrrc_find_rab_for_rb_in_est_rabs,
#ifdef FEATURE_DUAL_WCDMA
        tdsrrc_find_rab_for_rb_in_est_rabs_sub,
#else
        NULL,
#endif
		tdsrrc_get_classmark_change_handover_info,
		tdsrrc_get_hsdpa_status,
#ifdef FEATURE_DUAL_WCDMA
    tdsrrc_get_hsdpa_status_sub,
#else
    NULL,
#endif
		tdsrrc_get_hsupa_status,
#ifdef FEATURE_DUAL_WCDMA
    tdsrrc_get_hsupa_status_sub,
#else
      NULL,
#endif
		tdsrrc_get_mm_cmd_buf,
		tdsrrc_get_rr_cmd_buf,
		tdsrrc_get_tc_cmd_buf,
		tdsrrc_is_arfcn_valid,
		tdsrrc_malloc_for_external_cmd,
		tdsrrc_put_mm_cmd,
		tdsrrc_put_rr_cmd,
		tdsrrc_put_tc_cmd,
		tdsrrc_register_serving_cell_change_ind_cb,
		tdsrrc_return_cs_data_call_parms,
#ifdef FEATURE_DUAL_WCDMA
    tdsrrc_return_cs_data_call_parms_sub,
#else
    NULL,
#endif

		tdsrrc_return_lc_info_for_rab,
#ifdef FEATURE_DUAL_WCDMA
    tdsrrc_return_lc_info_for_rab_sub,
#else
      NULL,
#endif

		tdsrrc_set_ue_in_deep_sleep,
		tdsrrc_update_band_preference,
        NULL,
        NULL,
#ifdef FEATURE_CGPS
		tdsrrcmeas_get_cell_plmn_info,
#else
        NULL,
#endif
		tdsrxd_set_rx_chain_status,
    tdssrch_get_freq_syncdlRssi_tx_pwr_info,
/*=========================================================
  added by hzhi on 05/17/2013 to update vtbl for DIME2.0
  =========================================================*/
#ifndef TEST_FRAMEWORK
    tdscdmadiag_change_bler_rate,
#endif
    tdsrrc_register_rrc_codec_ho_info_cb,
/*====================================================================
  added by hzhi on 09/20/2013 for DIME3.0 M8974AAAAANAAM200349.
  ====================================================================*/
#ifdef FEATURE_TDSCDMA_FTM_NS_NEW
  tdsl1_api_send_acq_cmd,
  tdsl1_api_send_setup_cmd,
  tdsl1_api_send_start_cmd,
  tdsl1_api_send_stop_cmd,
#else /*FEATURE_TDSCDMA_FTM_NS_NEW*/
    NULL,
    NULL,
    NULL,
    NULL,
#endif/*FEATURE_TDSCDMA_FTM_NS_NEW*/
    tdsl1_api_send_timetag_cmd,
/*==============================================================
  added on 01/07/2014
  ==============================================================*/
    tdsrrc_get_ul_tx_power_range,
    tdsrrc_get_ul_rate_info,
#ifdef FEATURE_DUAL_WCDMA
    tdsrrc_get_ul_rate_info_sub,
#else
    NULL,
#endif

/*
 * added on 01/08/2014 for Bolt 1.1
 */
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
    tds_rrc_derive_freq_from_uarfcn,
#else
    NULL,
#endif /*FEATURE_TDSCDMA_TO_LTE || FEATURE_LTE_TO_TDSCDMA*/
    
/*
 * added on 01/31/2014 needed by CR 607232
 */
    tdssrch_get_timeslot_based_rxagc_info,
    tdssrch_cm_get_serving_cell_signal__info,
    tdssrch_cm_get_serving_cell_signal_instance_info,
    tdssrch_get_timeslot_based_txagc_info,
    tdsrrc_get_ul_data_rate_for_rab,
    NULL,
    tds_ext_audio_api,

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
    tdsrrc_get_inter_rat_bplmn_prtl_result,
#else
    NULL,
#endif

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
    tdsrrc_get_inter_rat_bplmn_prtl_result_sub_id,
#else
    NULL,
#endif

    tdsrrc_is_arfcn_valid_sub_id,

    tdsrrc_set_ue_in_deep_sleep_sub_id,

    tdsrrc_check_cell_barred_sub_id,

    tdsrrc_get_classmark_change_handover_info_sub_id,

    tdsrrc_delete_classmark_change_handover_info_sub_id,

    tdsrrc_update_band_preference_sub_id,

	tdsl1ulcfg_config_modem_power_stats_tx,
	
	tdsl1ulcfg_get_modem_power_stats_tx,
    tdsrrc_get_3gpp_release_version_ext,

    /*TRRC supports priority based reselection towards GERAN */
    tdsrrc_rr_absolute_priority_support,
/*================== end update vtbl ======================*/
};


void tdscdma_interface_dtor(void *pMe)
{
  if(!pMe){
    return;
  }
	tdsrrc_public_mem_free(pMe);
}



interface_t * tdscdma_interface_ctor(void)
{
	interface_t * pi = (interface_t *)tdsrrc_public_mem_malloc(sizeof(interface_t));
  if(!pi)
  {
    return NULL;
  }
	ref_cnt_obj_init(pi, tdscdma_interface_dtor);
	
	pi->pVtable = (iunknown_t *) &tdscdma_vtbl;

	return pi;
}

/*--------------------------------------------------------------------*
 * dummy function stub used when FEATURE_SEGMENT_LOADING is defined,  *
 * but TDS mode is unloaded.                                          *
 *--------------------------------------------------------------------*/
void tds_task_null(void) __attribute__((section(".task_text")));
void tds_task_null(void)
{
#ifndef FEATURE_MODEM_RCINIT
  /* Mask of signals returned by rex */
  uint32    rex_signals_mask;
  /* Send an ACK and wait for Task Start signal */
  tmc_task_start();
  for ( ;; )
  { /* Never exit this loop ... */
    rex_signals_mask = rex_wait(
                               TASK_OFFLINE_SIG   |
                               TASK_STOP_SIG
                               );
    if ((rex_signals_mask & TASK_STOP_SIG) != 0)
    {
      (void) rex_clr_sigs( rex_self(), TASK_STOP_SIG );
    }

    /*-------------------------------------------------------*/
    /* Check if offline command signal was set.  If set then */
    /* clear signal, send ack back to TMC, and proceed.       */
    /*-------------------------------------------------------*/

    if ((rex_signals_mask & TASK_OFFLINE_SIG) != 0)
    {
      (void) rex_clr_sigs( rex_self(), TASK_OFFLINE_SIG );
    }
  } /* end for */

#else /* FEATURE_MODEM_RCINIT */
  /* Send Handshake to RCINIT */
  rcinit_handshake_startup();
  for ( ;; )
  { /* Never exit this loop ... */
    rex_wait(
             TASK_OFFLINE_SIG   |
             TASK_STOP_SIG
             );
  } /* end for */
#endif /* FEATURE_MODEM_RCINIT */
}


#endif /* FEATURE_SEGMENT_LOADING */
