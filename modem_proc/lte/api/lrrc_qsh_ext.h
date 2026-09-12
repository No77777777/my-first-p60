/*!
  @file
  lrrc_qsh_ext.h

  @brief
  This file contains definitions for the Common type and constants required
  by different modules within LTE RRC

*/

/*=============================================================================

  Copyright (c) 2015 QUALCOMM Technologies Incorporated. All Rights Reserved

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

=============================================================================*/

/*=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order. 

$Header: //components/rel/lte.mpss/6.6.1/api/lrrc_qsh_ext.h#1 $

when       who           what, where, why
--------   ---           ------------------------------------------------------------
02/06/15   mdhanapa      initial version
=============================================================================*/

#ifndef LRRC_QSH_EXT_H
#define LRRC_QSH_EXT_H


/*=============================================================================

                   INCLUDE FILES

=============================================================================*/
#include <qsh.h>


#define LRRC_QSH_MAJOR_VER 0
#define LRRC_QSH_MINOR_VER 1


typedef enum
{  
    LRRC_QSH_DUMP_TAG0 = 0, /*lrrc_qsh_dump_tag_mini_crash_dump_s*/  
    LRRC_QSH_DUMP_TAG_MAX
} lrrc_qsh_dump_tag_e;

typedef enum
{  
  LRRC_QSH_EVENT_RLF_ANY = 0,  // RLF 
  LRRC_QSH_EVENT_RLF_ML1, // RLF due to PHY
  LRRC_QSH_EVENT_RLF_FC, // RLF due to Flow Control
  LRRC_QSH_EVENT_RLF_L2 ,  // RLF due to L2
  LRRC_QSH_EVENT_RLF_MAC_RACH,  // RLF due to RACH issues
  LRRC_QSH_EVENT_RLF_RLC_MAX_RETX , // RLF due to RLC MAX Reset
  LRRC_QSH_EVENT_RLF_RRC,  // RLF due to RRC internal procedures
  LRRC_QSH_EVENT_RLF_SIB_READ_ERROR ,  // RLF due to SIB read error
  LRRC_QSH_EVENT_RLF_HO_FAIL,  // RLF due to HO Failure
  LRRC_QSH_EVENT_CALL_START,  // Call start (1st DRB getting added)
  LRRC_QSH_EVENT_CALL_END,  // Call end (RRC Connection tear down)
  LRRC_QSH_EVENT_L2T_PSHO,  //  L2T PSHO
  LRRC_QSH_EVENT_L2W_PSHO,  //  L2W PSHO
  LRRC_QSH_EVENT_CELL_RESEL_FAILURE, // Cell reselection failure
  LRRC_QSH_EVENT_OOS_IND,  // OOS Indication from PHY
  LRRC_QSH_EVENT_VOLTE_CALL_START,  // VoLTE Call start (RoHC enabled)
  LRRC_QSH_EVENT_VOLTE_CALL_END,  // VoLTE Call end (RoHC disabled)
  LRRC_QSH_EVENT_L2L_RESEL_BEGIN,  // L2L resel begin
  LRRC_QSH_EVENT_IRAT_TO_LTE_FROM_TDS_HO_FAIL, // Inter RAT HO failure to LTE from TDS
  LRRC_QSH_EVENT_IRAT_TO_LTE_FROM_W_HO_FAIL, //// Inter RAT HO failure to LTE from W
  LRRC_QSH_EVENT_IRAT_FROM_LTE_TO_G_HO_FAIL, // Inter RAT HO failure from LTE to G
  LRRC_QSH_EVENT_IRAT_FROM_LTE_TO_1X_HO_FAIL, // Inter RAT HO failure from LTE to 1X
  LRRC_QSH_EVENT_IRAT_FROM_LTE_TO_TDS_HO_FAIL, // Inter RAT HO failure from LTE to TDS
  LRRC_QSH_EVENT_IRAT_FROM_LTE_TO_W_HO_FAIL, // Inter RAT HO failure from LTE to W
  LRRC_QSH_EVENT_IFREQ_HO_FAIL, // Inter freq HO failure
  LRRC_QSH_EVENT_SIB_READ_ERROR, // SIB Decode failure
  LRRC_QSH_EVENT_T311_TMRI, // t311 timer expiry 
  LRRC_QSH_EVENT_RRC_INACTIVE, // RRC Inactive state
  LRRC_QSH_EVENT_RRC_IDLE_NOT_CAMPED, // RRC Idle Not camped state
  LRRC_QSH_EVENT_RRC_IDLE_CAMPED, // RRC Idle Camped state
  LRRC_QSH_EVENT_RRC_CONNECTING, // RRC Connecting state
  LRRC_QSH_EVENT_RRC_CONNECTED, // RRC Connected state
  LRRC_QSH_EVENT_RRC_SUSPENDED, // RRC Suspended state
  LRRC_QSH_EVENT_RRC_IRAT_TO_LTE_STARTED, // RRC IRAT state
  LRRC_QSH_EVENT_RRC_CLOSING, // RRC Closing state
  LRRC_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE, // Common event for any failure case  
  LRRC_QSH_EVENT_PLMN_SRCH_TIME_OUT,
  LRRC_QSH_EVENT_CONN_ABORT_T300_EXPIRY,
  LRRC_QSH_EVENT_MAX_EVENT
}lrrc_qsh_event_e;


typedef enum
{  
    LRRC_QSH_METRIC_PERIODIC_STATS = 0,  /*lrrc_qsh_metric_periodic_stats_s*/
    LRRC_QSH_METRIC_PERIODIC_CSFB,  /*lrrc_qsh_metric_periodic_csfb_s*/ 
    LRRC_QSH_METRIC_PERIODIC_HO,  /*lrrc_qsh_metric_periodic_ho_s*/
    LRRC_QSH_METRIC_PERIODIC_RESEL,  /*lrrc_qsh_metric_periodic_resel_s*/
    LRRC_QSH_METRIC_TIMER,  /*lrrc_qsh_metric_tmr_s*/
    LRRC_QSH_METRIC_OTA,  /*lrrc_qsh_metric_ota_s*/
    LRRC_QSH_METRIC_MSG,  /*lrrc_qsh_metric_msg_s*/
    LRRC_QSH_METRIC_RRC_STATE,  /*lrrc_qsh_metric_rrc_state_s*/
    LRRC_QSH_METRIC_CONN_END_INFO, /* lrrc_qsh_metric_conn_end_info_s*/
    LRRC_QSH_METRIC_MAX
} lrrc_qsh_metric_e;


#define LTE_RRC_INFO_MAX_SI_LENGTH 96
typedef PACK(struct)
{
  /*! Number of Tx antennas detected */
  uint8                      num_tx_antennas;

  /*! Downlink bandwidth - lte_bandwidth_e */
  uint8            dl_bandwidth;

} lte_l1_mib_debug_info_param_s;

typedef PACK(struct)
{
  /*!< Value tag */
  uint32 value_tag;

  /*!< MIB */
  lte_l1_mib_debug_info_param_s mib;
  /*!< Encoded SIB1 */
  /*!< Size of encoded SI in bytes */
  uint32 encoded_si_size;
  /*!< Encoded SI */
  uint8 encoded_si[LTE_RRC_INFO_MAX_SI_LENGTH];

  /*!< Index corresponding to band chosen in multiBandInfoList */
  uint8 mfbi_index;
} lte_rrc_sib_active_cell_info_s;

typedef PACK(struct)
{
  /*!< Cause received in get_sibs req - lte_rrc_get_sibs_cause_e */
  uint8 cause;
  /**************************************************************************/

  /*!< Bitmask specifying sibs required based on UE capability and controller 
       state */
  uint32 sibs_required_bitmask;

  /*!< Bitmask specifying sibs to be acquired by this event. Reasons for 
       difference from sibs_required_mask:
        - Not all SIBs may be scheduled by SIB1
        - Event may be for reading only MIB & SIB1 */
  uint32 sibs_to_acquire_bitmask;
  /*!< Bitmask specifying sibs received so far regardless of they are current or 
       not (compare with sibs_rcvd_curr). This indicates SIBs that are either 
       current, or those that we are not sure of. Those that are stale are stale 
       are removed from this bitmask */
  uint32 sibs_rcvd_bitmask;

  /**************************************************************************/

  /*!< Bitmask indicating what SIBs sib_updated_indi needs to be sent for */
  uint32 sib_update_needed_bitmask;

  /**************************************************************************/

  /*!< Modification period window length (in SFN) */
  uint16 mod_period_window_length;
  /*!< SFN where current modification window starts */
  uint16 curr_window_start;
  /*!< SFN where current modification window ends */
  uint16 curr_window_end;

  /** SIB bitmask used in current modification period. It
   *  defines the SIBs that L1 must receive, and is applied
   *  immediately. While SIB collection is active, it always
   *  supersedes the mask sent in any previous requests. */
  uint32        sib_schd_req_curr_mask;

/** SIB bitmask to be used in the next modification period,
   *  i.e. after the specified boundary SFN. Before that SFN,
   *  the curr_mod_prd_sib_mask will be active. At the boundary
   *  SFN, L1 will immediately switch to the
   *  next_mod_prd_sib_mask. If the boundary SFN is set to
   *  LTE_CPHY_NULL_SFN, the next_mod_prd_sib_mask is ignored. */
  uint32        sib_schd_req_next_mask;

  /**************************************************************************/

  /*!< SFN after which SIBs are to be read. This is the SFN of the start of the 
       next modification period in case of si_update and next_mod_prd_value_tag_check. 
       This field is NULL otherwise. */
  uint16 next_mod_prd_sfn;
  
  /*!< Enum indicating which timer is currently running for the event
    lte_rrc_sib_event_tmr_e*/
  uint8 running_tmr;

  /** Last SFN when reading a SIB
  */
  uint16 last_read_sib_sfn;

} lte_rrc_sib_active_event_s;

typedef PACK(struct)
{
  /* dd_ptr_vars */
  /*!< Events db */
  lte_rrc_sib_active_event_s active_event;

  /* sd_ptr vars */
  /*!< SIB db */
  lte_rrc_sib_active_cell_info_s active_cell_sib_db;
 
  /*!< Boolean indicating whether ETWS is supported */
  boolean etws_supported:1;
  /*!< Boolean indicating whether CMAS is supported */
  boolean cmas_supported:1;
} lte_rrc_sib_info_s;
/* SIB Debug Info - END */

typedef PACK(struct)
{
  /* dd_ptr var */
  uint8                             num_lte_bands; /*!< Num LTE bands */

  boolean                           continue_search_after_cphy_bplmn_stop:1; /*!< continue search beyond stop */

  boolean                           irat_hplmn_found:1; /*!< IRAT HPLMN found */

  boolean                           pend_search_abort_till_mode_change_cnfi:1; /*!< pending flag */

  boolean                           ml1_revoke_cnfi_received:1; /*!< pending flag */

  boolean                           ml1_revoked:1; /*!< ML1 Revoked by other RRC module */

  boolean                           ml1_scheduled_search:1; /*!< ML1 scheduling PLMN search time slots */

  boolean                           rats_reordered:1; /*!< RATs are reordered */
  
  boolean                           nas_search_aborted:1; /*!< NAS search aborted */

  boolean                           stop_event_logged:1; /*!< Stop event logged earlier */

  boolean                           csg_search:1; /* CSG Search */

  boolean                           skip_searches_in_same_slot:1; /* Flag to skip subsequent L2X search attempts in same time slot */

} lte_rrc_plmn_search_info_s;
/* PLMN Search Debug Info - END */

typedef PACK(struct)
{
  /* Misc data */
  boolean is_trm_reserved:1; 
  uint8 reserve_reason; //lte_rrc_misc_trm_reserve_reason_e
  uint8 reserve_rat_type; //lte_rat_e

  /*! Current UE mode - sys_ue_mode_e_type */
  uint8              ue_mode;
  boolean            is_ue_mode_substate_srlte:1;

  /* MH data */
  /*!< Flag to indicate if last DL message received is being processed - if yes, 
       DLMs received will be added to pending queue until processing is
       complete */
  boolean dlm_processing_active:1;

  /*!< Number of messages cnf is yet to be received for. Basically the number 
       of mu_ids used */
  uint8 num_cnf_due;

  /*Pendq Data */
  uint8 num_items_in_pendq; 
} lte_rrc_misc_info_s;
/* MISC Debug Info - END */


typedef PACK(struct)
{

  /*! Bitmask which indicates which meas objects have been touched (add/modify/delete) */
  uint32 meas_obj_valid_bitmask;

  /*! Bitmask which indicates which meas ids have been touched (add/modify/delete) */
  uint32 meas_id_valid_bitmask;

  /*! Bitmask which indicates which report configs have been touched (add/modify/delete) */
  uint32 report_cfg_valid_bitmask;

  /*! Bitmask which indicates Qty config signaled across all RATs - lte_rrc_meas_qty_cfg_e */
  uint8 other_bitmask;

  /*! Number of frequency layers in idle mode - this could be the same as what is provided
      in SIBs or can be after pruning. */
  uint8  idle_meas_freq_layer_cnt;

  /*! Flag to indicate if T321 is running */
  boolean t321_running:1;

  /*! Flag to indicate if T320 is running */
  uint8 t320_running;

  /*! Flag to indicate if T325 is running */
  boolean t325_running:1;

  /*! Boolean to indicate if Depri all*/
  boolean depri_all:1;
  
} lte_rrc_meas_info_s;
/* MEAS Debug Info - END */


typedef PACK(struct)
{
  /*! Procedure ID of the initiating module lte_rrc_proc_e*/
  uint8 proc; 
  /*! Field to indicate the configuration type - lte_rrc_cfg_type_e*/
  uint8  cfg_type;
  /* Request_Response UMIDs */
  uint32 cmd_buffer_req_umid;
  uint32 cmd_buffer_rsp_umid;  
} lte_rrc_llc_info_s;
/* LLC Debug Info - END */


typedef enum
{
  /* Proc 0 is equivalent to no inter-RAT procedure in progress */
  LTE_RRC_IRAT_INFONO_PROC = 0,

  /*! From LTE Redirection procedure */
  LTE_RRC_IRAT_INFOFROM_LTE_REDIR_PROC,     

  /*! From LTE Internal Redirection procedure */
  LTE_RRC_IRAT_INFOFROM_LTE_INTERNAL_REDIR_PROC,  

  /*! To LTE Redirection procedure */
  LTE_RRC_IRAT_INFOTO_LTE_REDIR_PROC, 

  /*! From LTE Reselection procedure */
  LTE_RRC_IRAT_INFOFROM_LTE_RESEL_PROC,

  /*! From LTE Reverse Time Transfer (CDMA) procedure */
  LTE_RRC_IRAT_INFOFROM_LTE_RTT_PROC,

  /*! To LTE Reselection procedure */
  LTE_RRC_IRAT_INFOTO_LTE_RESEL_PROC,

  /*! From LTE CCO/NACC procedure - Applicable to LTE -> GERAN only */
  LTE_RRC_IRAT_INFOFROM_LTE_CCO_NACC_PROC,

  /*! To LTE PCCO procedure - Applicable to GERAN -> LTE only */
  LTE_RRC_IRAT_INFOTO_LTE_PCCO_PROC,

  /*! From LTE PLMN Search - Applicable to LTE -> GERAN/WCDMA only */
  LTE_RRC_IRAT_INFOFROM_LTE_PLMN_SEARCH_PROC,

  /*! To LTE PLMN Search - Applicable to GERAN/WCDMA -> LTE only*/
  LTE_RRC_IRAT_INFOTO_LTE_PLMN_SEARCH_PROC,

  /*! From LTE handover procedure */
  LTE_RRC_IRAT_INFOFROM_LTE_HANDOVER_PROC,

  /*! To LTE handover procedure */
  LTE_RRC_IRAT_INFOTO_LTE_HANDOVER_PROC,

  /*! From LTE 1x ECSFB procedure */
  LTE_RRC_IRAT_INFOFROM_LTE_1XECSFB_PROC,

  /*! From LTE 1x SRVCC procedure */
  LTE_RRC_IRAT_INFOFROM_LTE_1XSRVCC_PROC,

  /*! From LTE CGI Request procedure */
  LTE_RRC_IRAT_INFOFROM_LTE_CGI_REQUEST_PROC,

}lte_rrc_irat_proc_info_e;

typedef enum
{
  LTE_RRC_RAT_TYPE_LTE,            /*!< 0 */
  LTE_RRC_RAT_TYPE_GERAN,          /*!< 1 */
  LTE_RRC_RAT_TYPE_UTRA_FDD,       /*!< 2 */
  LTE_RRC_RAT_TYPE_UTRA_TDD,       /*!< 3 */
  LTE_RRC_RAT_TYPE_CDMA2000_HRPD,  /*!< 4 */
  LTE_RRC_RAT_TYPE_CDMA2000_1xRTT, /*!< 5 */
  LTE_RRC_RAT_TYPE_RAT_MAX
} lte_rrc_irat_rat_type_info_e;

typedef PACK(struct)
{
  lte_rrc_irat_proc_info_e                 proc;
  uint8     abort_cause; //lte_rrc_irat_abort_cause_type_e
  uint8        abort_proc; //lte_rrc_proc_e
  /*Applicable only to 
       T: lte_rrc_irat_to_tds_mode_e
      W: //lte_rrc_irat_to_w_mode_e
      1x: //lte_rrc_proc_e*/
  uint8            mode;
  // Applicable only to W
  uint8 resel_failed_info; //wcdma_rrc_lte_resel_failed_rsp_type
  boolean                             csfb_call_started:1;
  boolean                             csfb_call_ended:1;
  // Applicable only to G and DO
  uint8 resume_cause; //lte_rrc_to_DO_resume_cause_type_e
  // Applicable only to G
  uint8 deactivate_cause; //lte_rrc_to_G_deactivate_cco_cause_type_e
  uint8 fail_cause; //lte_rrc_irat_from_lte_failure_cause_type_e
  uint8 failure_phase; //lte_rrc_irat_cco_nacc_failure_phase_type_e
  boolean      override_bar_time:1; 
  boolean      sent_release_trm_req:1;
  // Applicable only to DO
  boolean                       pre_reg_status:1;
  // Applicable only to 1X
  uint8                               csfb_enabled:1;
} lte_rrc_irat_to_x_mgr_info_s;

typedef PACK(union)
{
  lte_rrc_irat_proc_info_e                     proc;

  // Applicable only to W and T
  uint64                                  redir_wait_tmr_val;
  int64                                   rem_wait_time;
  uint8              mode; 
  uint8              next_mode;
  uint8   psho_fail_cause;
  uint32      blind_redir_time_value;
  // Applicable only to G, 1x and DO
  uint8  deact_cause;
   // Applicable only to G
  uint32        redir_guard_time_value;
  uint64        redir_time_value;
  boolean       timer_set_externally:1;
  boolean       list_srch_only:1;
  // Applicable only to 1x and DO
  lte_rrc_irat_proc_info_e  proc_type_for_abort_during_deact;
} lte_rrc_irat_from_x_mgr_info_s;


typedef PACK(union)
{
    lte_rrc_irat_to_x_mgr_info_s to_x_info;
    lte_rrc_irat_from_x_mgr_info_s from_x_info;
} lte_rrc_irat_proc_debug_info_s;

typedef PACK(struct) 
{
  lte_rrc_irat_rat_type_info_e   rat_type;
  lte_rrc_irat_proc_debug_info_s rat_info;
} lte_rrc_irat_debug_info_s;
/* IRAT Debug Info - END */


typedef PACK(struct) 
{
  /*!< Table to keep track of UL transactions state from NAS - lte_rrc_ul_msg_fail_cause_e */
  uint8 lte_rrc_blocked_state_cause;
} lte_rrc_dt_info_s;
/* DT Debug Info - END */


typedef PACK(struct) 
{
  uint8 conn_rel_reason; /*!< Connection Release reason - lte_rrc_conn_rel_reason_e */
  boolean load_balancing_tau_is_reqd:1; /*!< Is TAU needed to be done by upper
                                           layers */
  boolean cs_FallbackHighPriority:1; /*!< Is Connection release associated with redirection 
                                        and cs Fall back high priority */
  boolean cell_barring_is_reqd:1; /*!< Is cell barring required by CSP */
  boolean redir_info_is_present:1; /*!< Is redirection info present? */
  boolean ml1_status:1; /*!< ML1 State - lte_rrc_ml1_status_type_e */
  boolean is_acq_needed:1; /*!< Connection Release to do ACQ */

} lte_rrc_crp_info_s;
/* CRP Debug Info - END */


typedef PACK(struct) 
{
  
  /* Camped statud - lte_rrc_camped_status_e */
  uint8 camped_status;
  
  /*! DL Center frequency - lte_earfcn_t */
  uint32 dl_earfcn;          

  /*! Band - sys_sband_lte_e_type*/
  uint8 band;            

  /*! System Bandwidth - lte_bandwidth_e */
  uint8 bandwidth;

  /*! Physical cell id of last camped cell - lte_phy_cell_id_t */
  uint16 phy_cell_id;   

  /*! SIB db index of last camped cell */
  uint8 sib_db_key;

  uint8 resel_in_progress:1;

  uint8 cfg_cnf_is_outstanding:1;

  uint8 deinit_is_reqd:1;

} lte_rrc_csp_info_s;
/* CSP Debug Info - END */


typedef PACK(struct) 
{

  /*!< Confirmation flags */
  boolean ml1_start_cnf_is_pending:1;
  boolean mac_start_cnf_is_pending:1;
  boolean rlcdl_start_cnf_is_pending:1;
  boolean rlcul_start_cnf_is_pending:1;
  boolean pdcpdl_start_cnf_is_pending:1;
  boolean pdcpul_start_cnf_is_pending:1;
  boolean ml1_stop_cnf_is_pending:1;
  boolean mac_stop_cnf_is_pending:1;
  boolean rlcdl_stop_cnf_is_pending:1;
  boolean rlcul_stop_cnf_is_pending:1;
  boolean pdcpdl_stop_cnf_is_pending:1;
  boolean pdcpul_stop_cnf_is_pending:1;
  /*! Status, if RF is available or not - lte_rrc_rf_status_e */
  uint8              rf_status;   
  uint8   device_mode; /*!< Device capability - sys_modem_device_mode_e_type */
  uint8  standby_mode; /*!< Number of active SIMs - sys_modem_dual_standby_pref_e_type */

} lte_rrc_ctlr_info_s;
/* CONTROLLER Debug Info - END */


typedef PACK(struct) 
{

  /*! CRE Cause - lte_rrc_cre_cause_e */
  uint8 cre_cause; 

  /*! Reason Reestablishment did not complete, if applicable - lte_rrc_cre_failure_cause_e */
  uint8 cre_failure_cause;

  /*! Boolean to indicate if conn release was received in middle of suspending RBs */
  boolean conn_rel_during_suspend:1; 

  /*Boolean to indicate RLF during CLOSING state*/
  boolean rlf_in_closing_state:1;
  
} lte_rrc_cre_info_s;
/* CRE Debug Info - END */


/* CEP Debug Info */
typedef PACK(struct) 
{
  /* dd_ptr var */
  uint8 est_cause; /*!< Establishment cause from the most recent 
                                     ConnEstReq */
  boolean rel_cfg_sent:1; /*!< Release cfg req sent*/
 
  boolean t300_timer_is_running:1;/*!< Is T300 timer running ?*/

  boolean resel_timer_is_running:1;/*!< Is the reselection timer running ?*/ 

  boolean resel_started; /*!State variable to keep track if reselection started*/

  boolean rach_in_progress:1;/*!< Is RACH in progress ?*/

  uint8 rach_abort_reason; /*!< Reason for aborting RACH, if any - lte_rrc_cep_rach_abort_reason_e */

  uint8 conn_est_status; /*!< Connection Establishment status, if applicable - lte_rrc_conn_est_status_e */
  
  boolean revoke_requested:1;/*!< This flag is set to TRUE if a 
                                revoke request has been sent by CEP*/ 

  boolean conn_abort_in_progress:1;/*!< Is CONN_ABORT in progress ?*/

} lte_rrc_cep_info_s;
/* CEP Debug Info - END */


typedef PACK(struct) 
{
  /*! sd_ptr var */
  uint8   rel_ver;
  uint8   ue_cat;
  uint8   ca_enable:1;
  uint8   max_cc_supported:3; /* Max value is 5 */
  uint32  fgi;
  uint32  tdd_fgi;
  uint32  fgi_r9;
  uint32  tdd_fgi_r9;
  uint32  fgi_r10;
  uint32  tdd_fgi_r10;
  boolean lte_supported:1;
  boolean cdma_1x_supported:1;
  boolean cdma_do_supported:1;
  boolean gsm_supported:1;
  boolean wcdma_supported:1;
  boolean tds_supported:1;
  uint64  lte_band_pref;
  uint64  cdma_1x_band_pref;
  uint64  cdma_do_band_pref;
  uint64  gsm_band_pref;
  uint64  wcdma_band_pref;
  uint64  tds_band_pref;
    
} lte_rrc_capability_info_s;
/* CAP Debug Info - END */


/*!< Size of the dispatch lookup hash table */
#define LTE_RRC_NUM_SM  31

/*!< Size of the QSH STM Tracebuffer */
#define LTE_RRC_QSH_STM_TRACEBUFFER_SIZE  64

typedef PACK(struct) 
{
  uint8 sm_name; /*!< State machine instance name */
  uint32 input_name; /*!< UMID */
  uint8 state; /*!< State machine Current state name */
} lte_rrc_stm_tracebuffer_info_s;

typedef PACK(struct) 
{
  /*! Transition trace buffer for on-target debugging purposes */
  lte_rrc_stm_tracebuffer_info_s trans_buffer[LTE_RRC_QSH_STM_TRACEBUFFER_SIZE];
} lte_rrc_stm_trans_info_s;

/* STM Debug Info */
typedef PACK(struct) 
{
  /* Same order in which they are activated in lte_rrc_dispatcher_activate_sms.
    index-0 being controller and index-30 being misc*/
  uint8 stm_states[LTE_RRC_NUM_SM]; 

  lte_rrc_stm_trans_info_s lte_rrc_stm;
  
} lte_rrc_stm_info_s;
/* STM Debug Info - END */


typedef PACK(struct) 
{
  lte_rrc_stm_info_s           stm_info;
  lte_rrc_capability_info_s    cap_info;
  lte_rrc_cep_info_s           cep_info;
  lte_rrc_ctlr_info_s          ctlr_info;
  lte_rrc_cre_info_s           cre_info;
  lte_rrc_csp_info_s           csp_info;
  lte_rrc_crp_info_s           crp_info;
  lte_rrc_dt_info_s            dt_info;
  lte_rrc_irat_debug_info_s    irat_info; 
  lte_rrc_llc_info_s           llc_info;   
  lte_rrc_meas_info_s          meas_info; 
  /* Contains debug info from misc, mh, pendq */
  lte_rrc_misc_info_s          misc_info; 
  lte_rrc_plmn_search_info_s   plmn_search_info;  
  lte_rrc_sib_info_s           sib_info;
} lte_rrc_debug_info_s; 

typedef PACK(struct)
{
 qsh_dump_tag_hdr_s hdr;
 lte_rrc_debug_info_s  tag_value;
} lrrc_qsh_dump_tag_mini_crash_dump_s;  


typedef enum
{  
    LTE_RRC_LOG_L_TO_L_RESEL = 0,
    LTE_RRC_LOG_L_TO_U_RESEL = 1,
    LTE_RRC_LOG_L_TO_G_RESEL = 2,
    LTE_RRC_LOG_RESEL_FAIL = 3,
    LTE_RRC_LOG_RESEL_MAX
} lte_rrc_log_resel_e;

typedef struct
{
  
  /*! Number of L<->L reselection attempts*/
  uint8 l_to_l_resel;
  
  /*! Number of L->U reselection attempts*/
  uint8 l_to_u_resel;
  
  /*! Number of L->G reselection attempts */
  uint8 l_to_g_resel;
  
  /*! Number of reselection failure attempts (LTE and IRAT)*/
  uint8 resel_failure_count; 
} lte_rrc_resel_s;  /*! To be logged after 100s*/

typedef enum
{  
    LTE_RRC_LOG_L_TO_L_HO = 0,
    LTE_RRC_LOG_L_TO_U_HO = 1,
    LTE_RRC_LOG_L_TO_G_CCO = 2,
    LTE_RRC_LOG_HO_FAIL = 3,
    LTE_RRC_LOG_HO_MAX
} lte_rrc_log_ho_e;


typedef struct
{
  
  /*! Number of L<->L HO attempts*/
  uint8 l_to_l_ho;
  
  /*! Number of L->U HO attempts*/
  uint8 l_to_u_ho;
  
  /*! Number of L->G CCO attempts*/
  uint8 l_to_g_cco;
  
  /*! Number of HO failure attempts (LTE and IRAT)*/
  uint8 ho_failure_cnt; 
} lte_rrc_ho_s;  /*! To be logged after 100s*/

typedef enum
{  
    LTE_RRC_LOG_L_TO_U_CSFB = 0,
    LTE_RRC_LOG_L_TO_G_CSFB = 1,
    LTE_RRC_LOG_CSFB_MAX
} lte_rrc_log_csfb_e;


typedef struct
{ 
  /*! Number of L->U CSFB attempts*/
  uint8 l_to_u_csfb;
  
  /*! Number of L->G CSFB attempts */
  uint8 l_to_g_csfb;
   
} lte_rrc_csfb_s;  /*! To be logged after 100s*/

typedef enum
{  
    LTE_RRC_LOG_RLF = 0,
    LTE_RRC_LOG_OOS = 1,
    LTE_RRC_LOG_CONN_REJ = 2,
    LTE_RRC_LOG_CELL_SEL_FAIL = 3,
    LTE_RRC_LOG_STATS_MAX
} lte_rrc_log_stats_e;

typedef struct
{ 
  /*! Number of RLF counts*/
  uint8 rlf_cnt;
  
  /*! Number of OOS counts*/
  uint8 oos_cnt;
  
  /*! Number of Connection reject counts*/
  uint8 conn_rej_cnt;
  
  /*! Number of Cell selection failure counts */
  uint8 cell_sel_failure_cnt;
   
} lte_rrc_stats_s;  /*! To be logged after 100s*/

/* RRC stats - END */

typedef struct
{
  /*! T301 in ms*/
  uint16 t301;
  
  /*! T304 in ms*/
  uint16 t304;
  
  /*! T310 enum lte_cphy_t310_e*/
  uint16 t310;
  
  /*! T311 in ms*/
  uint16 t311;
  
  /*! N310 enum lte_cphy_n310_e*/
  uint8 n310;
  
  /*! N311enum lte_cphy_n311_e*/
  uint8 n311;
   
} lte_rrc_log_tmr_s; 
/* RRC LOG TMR - END */

typedef struct
{
   uint8 est_status; /*!< Status -- lte_rrc_conn_est_status_e*/
   boolean barring_info_is_present:1; /*!< Is the barring timer info present ?*/
   uint8 call_type_bitmask;/*!< Bit mask indicating for which calls the barring 
                               info is valid */
} lte_rrc_log_conn_est_cnf_s; 

typedef struct
{
  uint8 status; /*!< Status -- lte_rrc_ul_data_cnf_status_e*/
} lte_rrc_log_ul_data_cnf_s; 

#define LTE_RRC_LOG_MAX_NUM_ACTIVE_EPS_BEARERS 11

typedef struct
{
  uint8 num_active_bearers; /*!< Number of active EPS bearers */
  uint8 active_eps_bearers[LTE_RRC_LOG_MAX_NUM_ACTIVE_EPS_BEARERS]; /*!< Active EPS bearers */
  uint8 num_added_bearers; /*!< Number of added EPS bearers */
  uint8 num_removed_bearers; /*!< Number of removed EPS bearers */
} lte_rrc_log_eps_ind_s; 

typedef struct 
{
  uint8 trm_priority; /*Indicates TRM priority(HIGH/LOW)*/
  uint8 trm_procedure; /*Indicates active TRM procedure*/
} lte_rrc_log_cphy_trm_prio_s; 

typedef struct
{
  /*! Device mode type -- sys_modem_device_mode_e_type*/
  int8  device_mode;
  /*!< Number of active SIMs -- sys_modem_dual_standby_pref_e_type*/
  uint8  standby_mode; 
  /*! UE mode type -- lte_cphy_ue_mode_type_e */
  uint8  ue_mode;
  /* Start cause for ML1 to decide TRM exchange -- lte_cphy_start_req_cause_e*/
  uint8    start_cause; 
  /* LTE earfcn for ML1 for TRM reservation */
  uint32    earfcn;
} lte_rrc_log_cphy_start_req_s;

typedef struct
{
  /*! Specific cause for stop -- lte_cphy_stop_req_cause_e*/
  uint8 stop_cause;
} lte_rrc_log_cphy_stop_req_s;

typedef struct
{
  /*! Suspend cause -- lte_cphy_suspend_resume_cause_e*/
  uint8   cause;
  /*! Reserve TRM for exchange*/
  boolean trm_irat_exchange:1;
} lte_rrc_log_cphy_suspend_req_s;

typedef struct
{
  /*! Resume cause: should match with suspend cause -- lte_cphy_suspend_resume_cause_e*/
  uint8    cause;
} lte_rrc_log_cphy_resume_req_s;

typedef struct
{
  uint8 rel_reason; /*!< Release reason -- lte_rrc_conn_rel_reason_e*/
  boolean load_balancing_tau_is_reqd:1; /*!< Is load balancing TAU needed */
} lte_rrc_log_conn_rel_s;

typedef struct
{
  /*! E-UTRA Band -- sys_sband_lte_e_type*/
  uint8 band;
  /*! EARFCN*/
  uint32 earfcn;
  /*! Physical cell ID */
  uint16 phy_cell_id;
} lte_rrc_log_cphy_cell_select_req_s;

typedef struct
{
  /*!< Service status -- lte_rrc_svc_status_e*/
  uint8 svc_status; 
  /* Division duplex type - None, FDD or TDD  */
  uint8 div_duplex;
  /* boolean flag to indicate whether force detach is required*/
  boolean detach_reqd:1;
} lte_rrc_log_svc_ind_s;

typedef struct
{
  boolean status:1; /*!< True = success, False = failure */
} lte_rrc_log_deact_cnf_s;


typedef struct
{
  uint8 status; /*!< Status -- lte_rrc_cco_nacc_status_e*/
} lte_rrc_log_cco_ind_s;

typedef struct
{
  /*! The cell id this configuration applies to or the source cell in case of
      handover */
  uint16 phy_cell_id;
  /*! The frequency the configuration applies to or the source cell in case of handover */
  uint32 dl_cell_frequency;
} lte_rrc_log_cphy_conn_rel_s;

typedef struct
{
  /*! The cell id this configuration applies to or the source cell in case of
      handover */
  uint16 phy_cell_id;
  /*! The frequency the configuration applies to or the source cell in case of handover */
  uint32 dl_cell_frequency;
  /*! Common Cfg Bitmask */
  uint16 mask;
} lte_rrc_log_cphy_common_cfg_req_s;

typedef struct
{
  /*! Identifies whether this command is specifying the default parameters
      or is a configuration update from the network. */
  boolean is_default_cfg:1;
  /*! Ded Cfg Bitmask */
  uint32 mask;
} lte_rrc_log_cphy_ded_cfg_req_s;

typedef struct
{
  /*! The cell id this configuration applies to or the source cell in case of
      handover */
  uint16 phy_cell_id;
  /*! The frequency the configuration applies to or the source cell in case of handover */
  uint32 dl_cell_frequency;
  /*! Common Cfg Bitmask */
  uint16 common_cfg_mask;
  /*! Ded Cfg Bitmask */
  uint32 ded_cfg_mask;
} lte_rrc_log_cphy_ho_cfg_req_s;

typedef struct
{
  /*! UM mode configured */
  boolean              um_mode_enabled:1;
} lte_rrc_log_cphy_drx_opt_req_s;

typedef struct
{
  /*! Requested Bitmask */
  uint16 req_mask;
  /*! Disabled Bitmask */
  uint16 disabled_mask;
} lte_rrc_log_cphy_idle_meas_req_s;

typedef struct
{
  /*! Bitmask which indicates which measurement objects are valid. Bit 0
      corresponds to meas object 1, ..., bit 31 corresponds to
      meas object 32. */
  uint32 meas_obj_valid_bitmask;

  /*! Bitmask which indicates which measurement ids are valid. Bit 0
      corresponds to meas ID 1, ..., bit 31 corresponds to meas ID 32. */
  uint32 meas_id_valid_bitmask;

  /*! Bitmask which indicates which report configs are valid. Bit 0
      corresponds to report config 1, ..., bit 31 corresponds to
      report config 32. */
  uint32 report_cfg_valid_bitmask;
  
  /*! Bitmask which indicates which other cfgs have been modified */
  uint32 other_mod_bitmask;
} lte_rrc_log_cphy_conn_meas_req_s;

typedef struct
{
  /*! request type -- lte_cphy_bplmn_type_e*/
  uint8     req_type;

  /*! Priority of the PLMN search, if set to LTE_CPHY_BPLMN_PRIORITY_HIGH
    BPLMN search should be prioritised -- lte_cphy_bplmn_priority_e*/
  uint8     priority;
} lte_rrc_log_cphy_bplmn_start_req_s;

typedef enum
{  
    LTE_RRC_LOG_MSG_TYPE_CONN_EST_CNF = 0,
    LTE_RRC_LOG_MSG_TYPE_ML1_CANCEL_CONN_REQ = 1,
    LTE_RRC_LOG_MSG_TYPE_UL_DATA_CNF = 2,
    LTE_RRC_LOG_MSG_TYPE_DL_DATA_IND = 3,
    LTE_RRC_LOG_MSG_TYPE_EPS_UPDATE_IND = 4,
    LTE_RRC_LOG_MSG_TYPE_ML1_TRM_PRIO_REQ = 5,
    LTE_RRC_LOG_MSG_TYPE_ML1_START_REQ = 6,
    LTE_RRC_LOG_MSG_TYPE_ML1_STOP_REQ = 7,
    LTE_RRC_LOG_MSG_TYPE_ML1_SUSPEND_REQ = 8,
    LTE_RRC_LOG_MSG_TYPE_ML1_RESUME_REQ = 9,
    LTE_RRC_LOG_MSG_TYPE_CONN_REL_IND = 10,
    LTE_RRC_LOG_MSG_TYPE_ML1_CELL_UNBAR_REQ = 11,
    LTE_RRC_LOG_MSG_TYPE_ML1_BL_PCI_IND = 12,
    LTE_RRC_LOG_MSG_TYPE_ML1_CELL_SELECT_REQ = 13,
    LTE_RRC_LOG_MSG_TYPE_SERVICE_IND = 14,
    LTE_RRC_LOG_MSG_TYPE_DEACT_CNF = 15,
    LTE_RRC_LOG_MSG_TYPE_ML1_CELL_BAR_REQ = 16,
    LTE_RRC_LOG_MSG_TYPE_CCO_COMP_IND = 17,
    LTE_RRC_LOG_MSG_TYPE_ML1_REL_TRM_REQ = 18,
    LTE_RRC_LOG_MSG_TYPE_ML1_BAND_SCAN_REQ = 19,
    LTE_RRC_LOG_MSG_TYPE_ML1_SYS_SCAN_REQ = 20,
    LTE_RRC_LOG_MSG_TYPE_ML1_ACQ_REQ = 21,
    LTE_RRC_LOG_MSG_TYPE_ML1_CONN_REL_REQ = 22,
    LTE_RRC_LOG_MSG_TYPE_ML1_ABORT_REQ = 23,
    LTE_RRC_LOG_MSG_TYPE_ML1_COMMON_CFG_REQ = 24,
    LTE_RRC_LOG_MSG_TYPE_ML1_DED_CFG_REQ = 25,
    LTE_RRC_LOG_MSG_TYPE_ML1_HO_CFG_REQ = 26,
    LTE_RRC_LOG_MSG_TYPE_ML1_DRX_REQ = 27,
    LTE_RRC_LOG_MSG_TYPE_ML1_IDLE_MEAS_REQ = 28,
    LTE_RRC_LOG_MSG_TYPE_ML1_CONN_MEAS_REQ = 29,
    LTE_RRC_LOG_MSG_TYPE_ML1_SERV_CELL_MEAS_REQ = 30,
    LTE_RRC_LOG_MSG_TYPE_ML1_IDLE_DRX_CFG_REQ = 31,
    LTE_RRC_LOG_MSG_TYPE_ML1_BPLMN_START_REQ = 32,
    LTE_RRC_LOG_MSG_TYPE_ML1_BPLMN_CELL_REQ = 33,
    LTE_RRC_LOG_MSG_TYPE_ML1_BPLMN_SIB_IND = 34,
    LTE_RRC_LOG_MSG_TYPE_ML1_BPLMN_STOP_REQ = 35,
    LTE_RRC_LOG_MSG_TYPE_ML1_BPLMN_SUSPEND_REQ = 36,
    LTE_RRC_LOG_MSG_TYPE_ML1_BPLMN_SKIP_REQ = 37,
    LTE_RRC_LOG_MSG_TYPE_ML1_SIB_SCHD_REQ = 38,
    LTE_RRC_LOG_MSG_TYPE_ML1_TDD_CFG_REQ = 39,
    LTE_RRC_LOG_MSG_TYPE_ML1_POWER_UPDATE_REQ = 40,
    LTE_RRC_LOG_MSG_TYPE_MAX
} lte_rrc_log_msg_type_e;


typedef union
{
  lte_rrc_log_conn_est_cnf_s conn_est_cnf; /*! LTE_RRC_CONN_EST_CNF*/
  lte_rrc_log_ul_data_cnf_s ul_data_cnf; /*! LTE_RRC_UL_DATA_CNF*/
  lte_rrc_log_eps_ind_s eps_ind; /*!LTE_RRC_ACTIVE_EPS_BEARER_UPDATE_IND*/
  lte_rrc_log_cphy_trm_prio_s trm_prio_req; /*!LTE_CPHY_TRM_PRIORITY_REQ*/
  lte_rrc_log_cphy_start_req_s start_req; /*!LTE_CPHY_START_REQ*/
  lte_rrc_log_cphy_stop_req_s stop_req; /*!LTE_CPHY_STOP_REQ*/
  lte_rrc_log_cphy_suspend_req_s suspend_req; /*!LTE_CPHY_SUSPEND_REQ*/
  lte_rrc_log_cphy_resume_req_s resume_req; /*!LTE_CPHY_RESUME_REQ*/
  lte_rrc_log_conn_rel_s conn_rel_ind; /*!LTE_RRC_CONN_REL_IND*/
  lte_rrc_log_cphy_cell_select_req_s cell_select_req; /*!LTE_CPHY_CELL_SELECT_REQ*/
  lte_rrc_log_svc_ind_s svc_ind; /*!LTE_RRC_SERVICE_IND*/
  lte_rrc_log_deact_cnf_s deact_cnf; /*!LTE_RRC_DEACTIVATE_CNF*/
  lte_rrc_log_cco_ind_s cco_ind; /*!LTE_RRC_CCO_NACC_COMPLETED_IND*/
  lte_rrc_log_cphy_conn_rel_s conn_rel_req; /*!LTE_CPHY_CON_RELEASE_REQ*/
  lte_rrc_log_cphy_common_cfg_req_s common_cfg_req; /*!LTE_CPHY_COMMON_CFG_REQ*/
  lte_rrc_log_cphy_ded_cfg_req_s ded_cfg_req; /*!LTE_CPHY_DEDICATED_CFG_REQ*/
  lte_rrc_log_cphy_ho_cfg_req_s ho_cfg_req; /*!LTE_CPHY_HANDOVER_REQ*/
  lte_rrc_log_cphy_drx_opt_req_s drx_opt_req; /*!LTE_CPHY_DRX_OPT_REQ*/
  lte_rrc_log_cphy_idle_meas_req_s idle_meas_cfg_req; /*!LTE_CPHY_IDLE_MEAS_CFG_REQ*/
  lte_rrc_log_cphy_conn_meas_req_s conn_meas_cfg_req; /*!LTE_CPHY_CONN_MEAS_CFG_REQ*/
  lte_rrc_log_cphy_bplmn_start_req_s bplmn_start_req; /*!LTE_CPHY_BPLMN_START_REQ*/
}lte_rrc_log_umid_info_u;

typedef struct
{
   /*! UMID - lte_rrc_log_msg_type_e*/
   uint8 umid;
   
   /*! UMID Info*/
   lte_rrc_log_umid_info_u umid_info;
} lte_rrc_log_msg_s; 
/* RRC MSG - END */

typedef struct
{
  /*! DL BW*/
  uint8 dl_bw;
  
  /*! Num TX Antennas*/
  uint8 num_tx_antennas;
  
  /*! PHICH Duration*/
  uint8 phich_duration;
  
  /*! PHICH resource*/
  uint8 phich_resource;
} lte_rrc_log_ota_mib_s;

typedef struct
{
  /*! Tracking Area Code*/
  uint32 tac;
  
  /*! Cell Identity*/
  uint32 cell_identity;
  
  /*! Intra Freq Reselection*/
  boolean intra_freq_resel:1;
  
  /*! Cell barred*/
  boolean cell_barred:1;
  
  /*! Frequency Band Indicator*/
  uint8 freq_band_ind;
  
  /*! Value tag*/
  uint8 value_tag;
} lte_rrc_log_ota_sib1_s;


typedef struct
{
  /*! Message Identifier*/
  uint32 msg_id;
  
  /*! Serial number*/
  uint32 serial_num;
} lte_rrc_log_ota_sib10_s;

typedef struct
{
  /*! Message Identifier*/
  uint32 msg_id;
  
  /*! Serial number*/
  uint32 serial_num;
} lte_rrc_log_ota_cmas_sib_s;

typedef struct
{
  /*! SI Modification*/
  boolean si_mod:1;
  
  /*! ETWS Indication*/
  boolean etws_indi:1;
  
  /*! CMAS Indication*/
  boolean cmas_indi:1;
  
} lte_rrc_log_ota_page_s;

typedef struct
{
  /*! Wait Time in Mins*/
  uint8 wait_time;
  
  /*! Deprioritization IE present or not*/
  boolean depri_present:1;
  
  /*! Deprioritization Type -- lte_depri_type_e*/
  uint8 depri_type:1;
  
  /*! Deprioritization Timer value in Mins*/
  uint8 t325;
  
} lte_rrc_log_ota_conn_rej_s;

typedef struct
{
  /*! Connection release cause*/
  uint8 cause;
  
  /*! Redirection IE present or not*/
  boolean redir_present:1;
  
  /*! IdleModeMobilityControlInfo IE present or not*/
  boolean idle_mobility_present:1;
} lte_rrc_log_ota_conn_rel_s;

typedef struct
{
  /*! Ciphering Algorithm*/
  uint8 cipher_algo;
  
  /*! integrityProt Algorithm*/
  uint8 intg_algo;
} lte_rrc_log_ota_sec_s;

typedef struct
{
  /*! Reestablishment cause*/
  uint8 cre_cause;
  
  /*! C-RNTI*/
  uint32 crnti;
  
  /*! PCI*/
  uint16 pci;
  
  /*! Short MAC-I*/
  uint16 maci;
  
} lte_rrc_log_ota_cre_s;

typedef struct
{
  /*! Establishment cause*/
  uint8 cause;
  
  /*! MMEC*/
  uint32 mmec;
  
  /*! TMSI*/
  uint32 tmsi;
  
} lte_rrc_log_ota_cep_s;

typedef struct
{ 
  /*! UE Rel version*/
  uint8 ue_rel;
  
  /*! R8/9/10 FDD/TDD FGIs*/
  uint32  fgi;
  uint32  tdd_fgi;
  uint32  fgi_r9;
  uint32  tdd_fgi_r9;
  uint32  fgi_r10;
  uint32  tdd_fgi_r10;
  
} lte_rrc_log_ota_cap_s;

typedef struct
{ 
  /*! AC Barring for Emergency*/
  boolean ac_barring_emergency:1; 
  
  /*!Additional Spectrum emmission*/
  uint8 additional_spectrum_emmission;
  
} lte_rrc_log_ota_sib2_s;

typedef struct
{ 
  /*!Q-Hyst */
  uint8 q_hyst;
  
  /*!S Non Intra search */
  uint8 s_non_intra_search;
  
  /*!Serving cell resel priority */
  int16 cell_resel_priority;
  
  /*!Q-Rxlevmin*/
  int32 q_rxlevmin;  
} lte_rrc_log_ota_sib3_s;

typedef struct
{ 
  /*! Number of inter Freq neighbors*/
  uint8 num_inter_freqs;
  
} lte_rrc_log_ota_sib5_s;

typedef struct
{ 
  /*! Boolean to indicate whether UTRAN Mode is FDD or not*/
  boolean utran_is_fdd:1;
  
  /*! Number of UTRAN Freq neighbors*/
  uint8 num_utran_freqs;
  
} lte_rrc_log_ota_sib6_s;

typedef struct
{ 
  /*! Number of GSM ARFCNs*/
  uint8 num_gsm_freqs;
  
  /*! Number of GSM Frequency groups*/
  uint8 num_gsm_freq_grps;
  
} lte_rrc_log_ota_sib7_s;

typedef struct
{ 
  /*! DO Band Class List size*/
  uint8 do_band_class_list_size;
  
  /*! 1x Band Class List size*/
  uint8 onex_band_class_list_size;
  
  /*! DO Neighbor cell info list size*/
  uint8 do_neigh_cell_info_list_size;
  
  /*! 1x Neighbor cell info list size*/
  uint8 onex_neigh_cell_info_list_size;
  
} lte_rrc_log_ota_sib8_s;

typedef struct
{ 
  /*! Number of SRBs to be added*/
  uint8 num_srb_to_add;
  
  /*! Number of DRBs to be added*/
  uint8 num_drb_to_add;
  
  /*! Number of DRBs to be released*/
  uint8 num_drb_to_rel;
} lte_rrc_log_ota_rb_dlm_s;


typedef struct
{ 
  /*! Number of SRBs to be added*/
  uint8 num_srb_to_add;
  
  /*! Number of DRBs to be added*/
  uint8 num_drb_to_add;
  
  /*! Number of DRBs to be released*/
  uint8 num_drb_to_rel;

  /*! Meas Cfg Present*/
  boolean meas_cfg_present:1;
  
  /*!CA Cfg Present*/
  boolean ca_present:1;
  
  /* Mobility Present*/
  boolean ho_present:1;
  
} lte_rrc_log_ota_cfg_dlm_s;

typedef struct
{ 
  /*! Meas Id*/
  uint8 meas_id;
  
  /*! Meas Obj Id*/
  uint8 meas_obj_id;
  
  /*! Report Cfg Id*/
  uint8 rpt_cfg_id;

  /*! Report Type -- lte_cphy_meas_report_cfg_trig_type_e*/
  uint8 rpt_type;
  
} lte_rrc_log_ota_mr_s;

typedef struct
{ 
  /*! RLF Report present*/
  boolean rlf_rpt_present:1;
  
  /*! Log Meas Report present*/
  boolean log_meas_rpt_present:1;
  
  /*! Connection fail Report present*/
  boolean conn_fail_rpt_present:1;

} lte_rrc_log_ota_ul_dcch_cmp_s;

typedef struct
{ 
  /*! Eutra Capability requested*/
  boolean eutra_cap_req:1;
  
  /*! Utra Capability requested*/
  boolean utra_cap_req:1;
  
  /*! GERAN PS Capability requested*/
  boolean gsm_ps_cap_req:1;

  /*! GERAN CS Capability requested*/
  boolean gsm_cs_cap_req:1;
  
  /*! CDMA Capability requested*/
  boolean cdma_cap_req:1;
} lte_rrc_log_ota_cap_req_s;
  

typedef enum
{  
    LTE_RRC_LOG_OTA_TYPE_MIB = 0,
    LTE_RRC_LOG_OTA_TYPE_SIB1 = 1,
    LTE_RRC_LOG_OTA_TYPE_SIB10 = 2,
    LTE_RRC_LOG_OTA_TYPE_SIB11 = 3,
    LTE_RRC_LOG_OTA_TYPE_SIB12 = 4,
    LTE_RRC_LOG_OTA_TYPE_PAGE = 5,
    LTE_RRC_LOG_OTA_TYPE_CONN_REJ = 6,
    LTE_RRC_LOG_OTA_TYPE_CONN_REL = 7,
    LTE_RRC_LOG_OTA_TYPE_SEC = 8,
    LTE_RRC_LOG_OTA_TYPE_REST_REQ = 9,
    LTE_RRC_LOG_OTA_TYPE_CONN_EST_REQ = 10,
    LTE_RRC_LOG_OTA_TYPE_CAP_RSP = 11,
    LTE_RRC_LOG_OTA_TYPE_SIB2 = 12,
    LTE_RRC_LOG_OTA_TYPE_SIB3 = 13,
    LTE_RRC_LOG_OTA_TYPE_SIB4 = 14,
    LTE_RRC_LOG_OTA_TYPE_SIB5 = 15,
    LTE_RRC_LOG_OTA_TYPE_SIB6 = 16,
    LTE_RRC_LOG_OTA_TYPE_SIB7 = 17,
    LTE_RRC_LOG_OTA_TYPE_SIB8 = 18,
    LTE_RRC_LOG_OTA_TYPE_REEST_DLM = 19,
    LTE_RRC_LOG_OTA_TYPE_CONN_STP_DLM = 20,
    LTE_RRC_LOG_OTA_TYPE_RECFG_DLM = 21,
    LTE_RRC_LOG_OTA_TYPE_MR = 22,
    LTE_RRC_LOG_OTA_TYPE_RECFG_CMP = 23,
    LTE_RRC_LOG_OTA_TYPE_REEST_CMP = 24,
    LTE_RRC_LOG_OTA_TYPE_CONN_SETUP_CMP = 25,
    LTE_RRC_LOG_OTA_TYPE_SMC = 26,
    LTE_RRC_LOG_OTA_TYPE_SMF = 27,
    LTE_RRC_LOG_OTA_TYPE_CAP_REQ = 28,
    LTE_RRC_LOG_OTA_TYPE_UEINFO = 29,
    LTE_RRC_LOG_OTA_TYPE_MAX
} lte_rrc_log_ota_type_e;

typedef union
{
 lte_rrc_log_ota_mib_s mib;                /*! MIB*/
 lte_rrc_log_ota_sib1_s sib1;                /*! SIB1*/
 lte_rrc_log_ota_sib10_s sib10;            /*! SIB10*/
 lte_rrc_log_ota_cmas_sib_s sib11;         /*! SIB11*/
 lte_rrc_log_ota_cmas_sib_s sib12;         /*! SIB12*/
 lte_rrc_log_ota_page_s page;              /*! Paging*/
 lte_rrc_log_ota_conn_rej_s conn_rej;      /*! Conn Reject*/
 lte_rrc_log_ota_conn_rel_s conn_rel;      /*! Conn Release*/
 lte_rrc_log_ota_sec_s security;           /*! Security*/
 lte_rrc_log_ota_cre_s cre_req;            /*! Reestablishment Req*/
 lte_rrc_log_ota_cep_s cep_req;            /*! Connection establishment Req*/
 lte_rrc_log_ota_cap_s cap_rsp;            /*! Capability response*/
 lte_rrc_log_ota_sib2_s sib2;                  /*! SIB2*/
 lte_rrc_log_ota_sib3_s sib3;                  /*! SIB3*/
 lte_rrc_log_ota_sib5_s sib5;                 /*! SIB5*/
 lte_rrc_log_ota_sib6_s sib6;                 /*!  SIB6*/
 lte_rrc_log_ota_sib7_s sib7;                /*! SIB7*/
 lte_rrc_log_ota_sib8_s sib8;                /*! SIB8*/
 lte_rrc_log_ota_rb_dlm_s cre_dlm;   /*! Reestablishment DLM*/
 lte_rrc_log_ota_rb_dlm_s cep_dlm; /*! Connection setup DLM*/
 lte_rrc_log_ota_cfg_dlm_s cfg_dlm;  /*! Reconfig DLM*/
 lte_rrc_log_ota_mr_s meas_rpt;        /*! Meas report*/
 lte_rrc_log_ota_ul_dcch_cmp_s cfg_cmp; /*! Reconfig Complete*/
 lte_rrc_log_ota_ul_dcch_cmp_s cre_cmp; /*! Reestablishment Complete*/
 lte_rrc_log_ota_ul_dcch_cmp_s cep_cmp; /*! Connection setup Complete*/
 lte_rrc_log_ota_cap_req_s cap_req; /*! UE Capability request*/
}lte_rrc_log_ota_data_u;

typedef struct
{
   /*! OTA Type -- lte_rrc_log_ota_type_e*/
   uint8 ota_type;
   
   /*! OTA Info*/
   lte_rrc_log_ota_data_u ota_data;
} lte_rrc_log_ota_s; 
/* RRC OTA - END */

typedef struct
{
  qsh_metric_hdr_s hdr;
  lte_rrc_log_ota_s ota_info;
}  lrrc_qsh_metric_ota_s;

typedef struct
{
  qsh_metric_hdr_s hdr;
  lte_rrc_log_msg_s msg_info;
}  lrrc_qsh_metric_msg_s;

typedef struct
{
  qsh_metric_hdr_s hdr;
  lte_rrc_log_tmr_s tmr_info;
}  lrrc_qsh_metric_tmr_s;

typedef struct
{
  qsh_metric_hdr_s hdr;
  lte_rrc_stats_s stats_info;
}  lrrc_qsh_metric_periodic_stats_s;

typedef struct
{
  qsh_metric_hdr_s hdr;
  lte_rrc_csfb_s csfb_info;
}  lrrc_qsh_metric_periodic_csfb_s;

typedef struct
{
  qsh_metric_hdr_s hdr;
  lte_rrc_ho_s ho_info;
}  lrrc_qsh_metric_periodic_ho_s;

typedef struct
{
  qsh_metric_hdr_s hdr;
  lte_rrc_resel_s resel_info;
}  lrrc_qsh_metric_periodic_resel_s;

typedef struct
{
  qsh_metric_hdr_s hdr;
  /*! Current state -- lte_rrc_log_ctrl_state_e*/
  uint8 rrc_state;
}  lrrc_qsh_metric_rrc_state_s;

typedef struct
{
  qsh_metric_hdr_s hdr;

  uint8 call_end_type;

  uint8 cause;

}lrrc_qsh_metric_conn_end_info_s;

#endif /* LRRC_QSH_EXT_H */

