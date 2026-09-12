
#ifndef WRRC_QSH_EXT_H
#define WRRC_QSH_EXT_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            WRRC QSH EXT . H                 

GENERAL DESCRIPTION
   Contains declarations for function which are QUALCOMM proprietary 
   and may or may not be shipped as source code.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:$
  $Header: 

when       who     what, where, why
--------   ---     ---------------------------------------------------------
12/30/16   sp    Added state change event
08/17/16   sp     Made changes to update CU cause to QSH
06/27/16   sp     Added support to report general failure events to QSH
05/30/16   vs     Changes to add LOCK_WAIT_TIMEOUT event as a part of DTF
05/26/16   ha     Made changes to report Inter freq HO failure to QSH
05/23/16   vs     Changes to add SIB_READ_FAILURE event as a part of DTF
01/12/16   nr     Made changes to report failure events to QSH
11/25/15   sp     Made changes to use uint32 for cell-id instead of uint16
10/26/15   sp     Made changes to log connection end information and multi rab status
10/19/15   sp     Fixed compilation warning due to incorrect commenting
10/15/15   sp     Made changes to add resel failure, rrc conn rel events
09/10/15   sp     Made changes to log configuration info to QSH structures in inter task commands.
07/03/15   sg     Made changes to add PSC to timers and counters metric
06/30/15   sp     Fixed compilation errors with QSH feature enabled
03/20/15   sp     Made changes to convert ota metric msg_id to enum and include t314 logging
03/10/15   ad     Made changes to log important parms from OTAs
02/06/14   sg     Initial version for wrrc_qsh_ext.h

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include <string.h>
#include "trm.h"
#include "qsh.h"
#include "sys.h"

#define WRRC_QSH_MAJOR_VER 0
#define WRRC_QSH_MINOR_VER 1

/* Below data structures are defined for mini dump */

#define RRC_CRASH_DEBUG_OPT_MM_CMD_CNT 10
#define RRC_CRASH_DEBUG_OPT_IRAT_RRC_CMD_CNT 3
#define RRC_CRASH_DEBUG_OPT_INTERNAL_CMD_CNT 25
#define RRC_CRASH_DEBUG_OPT_L1_RRC_CMD_CNT 10
#define RRC_CRASH_DEBUG_OPT_LLC_CMD_CNT 8
#define RRC_CRASH_DEBUG_OPT_OTA_SIZE 1000
#define WRRC_QSH_CELL_UPDATE_CAUSE_NONE 0xFF


typedef enum
{
  WRRC_QSH_DUMP_TAG_CONN_MODE_GLOBALS = 0, /* wrrc_qsh_dump_tag_conn_mode_globals*/
  WRRC_QSH_DUMP_TAG_CSP_CCM_SIB_RCE_DEBUG,/*wrrc_qsh_dump_tag_csp_ccm_sib_rce*/
  WRRC_QSH_DUMP_TAG_MCM_DEBUG,/*wrrc_qsh_dump_tag_mcm*/
  WRRC_QSH_DUMP_TAG_LLC_DEBUG,/*wrrc_qsh_dump_tag_llc_debug*/
  WRRC_QSH_DUMP_TAG_CMD_OTAS_DEBUG,/* */  
  WRRC_QSH_DUMP_TAG_NV_VALUES,/*wrrc_qsh_dump_tag_nv_values*/
  WRRC_QSH_DUMP_TAG_CU_DEBUG,/*wrrc_qsh_dump_tag_cu*/
  WRRC_QSH_DUMP_TAG_RB_PROC_DEBUG,/*wrrc_qsh_dump_tag_rb_proc*/
  WRRC_QSH_DUMP_TAG_MISC_PROC_DEBUG,/*wrrc_qsh_dump_tag_misc_proc*/
  WRRC_QSH_DUMP_TAG_CCTRCH_DEBUG,/*wrrc_qsh_dump_tag_cctrch_debug*/
  WRRC_QSH_DUMP_TAG_MAX
}wrrc_qsh_dump_tag_e;

typedef PACK(struct)
{
 uint16 timeticks; 
 uint16 config;
 uint32 cmd_id;
}rrc_debug_opt_cmd_type;

typedef PACK(struct)
{
  uint16 timeticks;
  uint16 config;
  uint32 cmd_id;/*rrc_to_mm_sink_e_type*/
}rrc_to_mm_debug_opt_cmd_type;

typedef PACK(struct)
{
  uint16 timeticks;
  uint16 config;
  uint32 cmd_id;/*rrc_rr_cmd_e_type*/
}rrc_rr_debug_opt_cmd_type;


typedef PACK(struct)
{
  uint16 timeticks;
  uint16 config;
  uint32 cmd_id;/*l1_cmd_enum_type*/
}rrc_to_l1_opt_cmd_type;


typedef PACK(struct)
{
  uint16 timeticks;
  uint16 config;
  uint32 cmd_id;
}lte_cmds_opt_type;


typedef PACK(struct)
{
  uint16 timeticks;
  uint16 config;
  uint32 l1_llc_cmd;
}rrc_llc_cmd_opt_type;

typedef PACK(struct)
{
  rrc_debug_opt_cmd_type mm_to_rrc_debug_cmds[RRC_CRASH_DEBUG_OPT_MM_CMD_CNT];
  rrc_to_mm_debug_opt_cmd_type rrc_to_mm_debug_cmds[RRC_CRASH_DEBUG_OPT_MM_CMD_CNT];
}rrc_crash_debug_opt_mm_cmd_type;

typedef PACK(struct)
{
  rrc_debug_opt_cmd_type rr_to_rrc_debug_cmds[RRC_CRASH_DEBUG_OPT_IRAT_RRC_CMD_CNT];
  rrc_rr_debug_opt_cmd_type rrc_to_rr_debug_cmds[RRC_CRASH_DEBUG_OPT_IRAT_RRC_CMD_CNT];
}rrc_crash_debug_opt_rr_cmd_type;

typedef PACK(struct)
{
  rrc_debug_opt_cmd_type int_cmd[RRC_CRASH_DEBUG_OPT_INTERNAL_CMD_CNT];
}rrc_crash_debug_opt_int_cmd_type;

typedef PACK(struct)
{
  rrc_debug_opt_cmd_type l1_to_rrc_debug_cmds[RRC_CRASH_DEBUG_OPT_L1_RRC_CMD_CNT];
  rrc_to_l1_opt_cmd_type rrc_to_l1_debug_cmds[RRC_CRASH_DEBUG_OPT_L1_RRC_CMD_CNT];
}rrc_crash_debug_opt_l1_cmd_type;

typedef PACK(struct)
{
  lte_cmds_opt_type lte_to_rrc_debug_cmds[RRC_CRASH_DEBUG_OPT_IRAT_RRC_CMD_CNT]; 
  lte_cmds_opt_type rrc_to_lte_debug_cmds[RRC_CRASH_DEBUG_OPT_IRAT_RRC_CMD_CNT];
}rrc_crash_debug_opt_lte_cmd_type;

typedef PACK(struct)
{
  rrc_llc_cmd_opt_type rrc_llc_cmds[RRC_CRASH_DEBUG_OPT_LLC_CMD_CNT];
}rrc_crash_debug_opt_llc_cmd_type;

typedef PACK(struct)
{
  rrc_crash_debug_opt_mm_cmd_type rrc_crash_debug_mm_cmds;
  rrc_crash_debug_opt_rr_cmd_type rrc_crash_debug_rr_cmds;
  rrc_crash_debug_opt_int_cmd_type rrc_crash_debug_int_cmds;
  rrc_crash_debug_opt_l1_cmd_type rrc_crash_debug_l1_cmds;
  rrc_crash_debug_opt_lte_cmd_type rrc_crash_debug_lte_cmds;
  rrc_crash_debug_opt_llc_cmd_type rrc_crash_debug_llc_cmds;  
}rrc_crash_debug_opt_info_type;

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  uint8 rrc_mode;/*rrc_mode_e_type*/
  uint8 rrc_curr_state; /*rrc_state_e_type  */
  uint8 rrc_qsh_config_ptr_in_use; /* rrcllc_config_e_type */
  uint8 rrc_oc_state;  /*rrc_state_e_type  */
  uint8 rrc_cc_state;  /*rrc_state_e_type  */
  uint8 rrc_oc_set_status; /* rrcllc_oc_set_status_e_type */
  uint8 rrc_oc_set_proc; /* rrc_proc_e_type */
  uint8 rrc_oc_process_state; /* rrcllc_oc_process_state_e_type */
  uint8 toc_usage;/* rrcllc_toc_usage_e_type */
  uint8 hsdpa_action; /* hsdpa_action_enum_type */
  uint8 is_sib5_change_in_progress; /* rrcllc_sib5_change_type */
  uint8 oc_rnti_valid; /*mac_rnti_valid_e_type */
  uint8 oc_hrnti_status; /* rrc_hsdpa_hrnti_enum_type */
  uint8 oc_efach_hrnti_status; /*efach_hrnti_type_enum */
  uint8 oc_hs_action_in_e_fach;  /*hsdpa_action_enum_type */
  uint8 cc_rnti_valid; /*mac_rnti_valid_e_type */
  uint8 cc_hrnti_status; /* rrc_hsdpa_hrnti_enum_type */
  uint8 cc_efach_hrnti_status; /*efach_hrnti_type_enum */
  uint8 cc_hs_action_in_e_fach;  /*hsdpa_action_enum_type */
  uint8 dest_freq; /*rrc_freq_type*/  
  uint8 ctch_status; /* rrcllc_ctch_status_e_type */
  uint8 oc_l1_hs_rach_req_mask;  
  uint8 cc_l1_hs_rach_req_mask;
  uint8 sec_eul_is_hho:1 ;
  uint8 dest_psc_present:1 ; 
  uint8 hs_rach_to_eul_trans:1 ;
  uint8 common_edch_transmission:1 ;
  uint8 hspa_rnti_stored_cell_pch:1 ;
  uint8 cpc_dtx_drx_status:1 ;
  uint8 cpc_drx_status:1 ;
  uint8 e_dch_transmission:1 ;
  uint8 sec_e_dch_transmission:1 ;
  uint8 hs_status_in_e_fach:1 ;
  uint8 hsdpa_status:1 ;
  uint8 revert_old_config:1 ;
}wrrc_qsh_dump_tag_conn_mode_globals; /* 25 bytes */

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  uint16 wcdma_nv_hsupa_category;
  uint16 rrc_feature_nv_item_value;
  uint16 rrc_dsds_concurrency;  
  uint16 rrc_dsda_concurrency;
  uint8 rrc_nv_rel_indicator_debug;
  uint8 nv_hsdpa_category_debug;
  uint8 rrc_default_cfg_in_cell_fach_nv_debug;
  uint8 wcdma_look_for_concurrency_nv_debug :1 ;
  uint8 feature_dch_fach_opt_enabled_debug :1 ;
}wrrc_qsh_dump_tag_nv_values; /* 12 bytes */

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  uint32 num_stored_cells_sib_db;
  uint32 active_cell_index;
  uint8 last_mm_primitive; /* rrc_csp_mm_primitive_e_type */
  uint8 curr_type_acq; /* rrc_csp_acq_e_type */
  uint8 is_valid_nw_sharing;
  uint8 csfb_extended_status; /* rrccsp_csfb_status_e_type */
  uint8 interrat_type; /* rrc_csp_interrat_rat_e_type */
  uint8 bplmn_interrat_type; /* rrc_csp_interrat_rat_e_type */
  uint8 bplmn_suspend_proc; /* rrc_proc_e_type */
  uint8 bplmn_suspend_cause; /* rrc_bplmn_suspend_cause_e_type */
  uint8 rrc_csp_substate; /* rrc_csp_substate_e_type */	
  uint8 curr_scan;/* rrc_csp_scan_e_type */
  uint8 pending_type;/* rrc_csp_pending_e_type */
  uint8 cell_camp_status ;/* rrc_new_cell_status_e_type */
  uint8 camping_status;/* rrc_ccm_cell_camping_status_valid_e_type */
  uint8 rrc_ccm_substate;/* rrc_ccm_substate_e_type */
  uint8 srv_bcch_action;/* sib_bcch_action_e_type */
  uint8 ngh_bcch_action;/* sib_bcch_action_e_type */
  uint8 rrcrce_substate;/* rrcrce_substate_e_type */
  uint8 rrc_establishment_cause; /* rrc_establish_cause_e_type */
  uint8 rrc_v_300;
  uint8 rrc_release_cause; /* rrc_rel_cause_e_type */
  uint8 rrcrce_mm_abort_cause;/* rrc_abort_cause_e_type */
  uint8 init_cell_sel_ind_during_wtointerrat:1 ;
  uint8 bplmn_guard_srch_tmr_expired:1 ;
  uint8 bplmn_srch_tmr_expired:1 ;
  uint8 rrc_csfb_call_status:1 ;
  uint8 rrcrce_send_abort_cnf :1 ;
  uint8 rrcrce_abort_ind_sent:1 ;
  uint8 update_sibs_event_pending:1 ;
  uint8 event0_active:1 ;
  uint8 event1_active:1 ;
  uint8 sib_db_empty :1;
  uint8 ue_in_deep_sleep :1;
  uint8 acq_db_empty :1;
  uint8 bplmn_new_srch :1;
  uint8 bplmn_srch_cnf :1;
  uint8 do_lfs_scan :1;
  uint8 stop_pending_for_wtog_abort :1;
  uint8 allow_lte_resel :1;
  uint8 bplmn_data_saved :1;
  uint8 ue_in_conn_oos :1;
  uint8 dch_to_fach_pch_tmr_expd :1;
}wrrc_qsh_dump_tag_csp_ccm_sib_rce;/* 32 bytes */


typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  uint64 cphy_setup_req_mask[5];	/* last 5 cphy setup req masks */
  uint32 rrc_current_cmd_id;/* Current cmd being processed - rrc_cmd_e_type */
  uint8 current_cmd_engine; /* rrcllc_cmd_engine_e_type */
  uint8 chan_config_reason; /* rrcllc_chan_cfg_reason_e_type */
  uint8 int_cmd_q_cnt; 
  uint8 llc_cmd_q_cnt; 
  uint8 mm_cmd_q_cnt; 
  uint8 l1_cmd_q_cnt; 
  uint8 rrcllc_current_substate;
  uint8 procedure;  
  uint8 next_rrc_state; 
  uint8 cmd_under_process_valid :1 ;
}wrrc_qsh_dump_tag_llc_debug; /* 56 bytes */


typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  uint8 qsh_rrccu_substate; /* rrccu_substate_e_type */  
  uint8 rrccu_cell_update_cause; /* rrc_CellUpdateCause */
  uint8 rrccu_ura_update_cause; /* rrc_URA_UpdateCause */
  uint8 rrccu_state_indicator; /* rrc_RRC_StateIndicator */
  uint8 rrccu_resp_msg;/* rrccu_resp_msg_e_type */  
  uint8 rrccu_tran_to_discon_counter;  
  uint8 rrccu_buffered_cmd_mask;
  uint8 rrccu_reestablish_srb:1 ;
  uint8 rrccu_reestablish_rab:1 ;
}wrrc_qsh_dump_tag_cu; /* 8 bytes */


typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  uint8 rrc_rbe_substate; /* rrcrb_substate_e_type */
  uint8 rrc_rbrc_substate; /* rrcrb_substate_e_type */
  uint8 rrc_pcr_substate; /* rrcrb_substate_e_type*/
  uint8 rrc_tcr_substate; /* rrcrb_substate_e_type */
  uint8 rrc_rbr_substate; /* rrcrb_substate_e_type */
  uint8 num_of_ps_rabs;
  uint8 num_of_cs_rabs;
  uint8 call_type_mask;
  uint8 rrcrbe_send_initiate_cu_flag:1 ;
  uint8 rrcrbrc_send_initiate_cu_flag:1 ;
  uint8 rrcpcrc_send_initiate_cu_flag:1 ;
  uint8 rrctcrc_send_initiate_cu_flag:1 ;
  uint8 rrcrbr_send_initiate_cu_flag:1 ;  
}wrrc_qsh_dump_tag_rb_proc; /* 9 bytes */

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  uint32 rrcmcm_cmd_store_mm_cmdid;/* rrc_cmd_e_type */
  uint32 rrcmcm_cmd_store_interat_resel_rr_cmdid;/* rrc_rr_cmd_e_type */
  uint32 rrcmcm_cmd_store_lte_irat_cmdid;/* rrc_cmd_e_type */
  uint8 rrcmcm_state;/* rrcmcm_state_e_type */  
  uint8 rrcmcm_cmd_store_abort_flag;/* rrcmcm_abort_flag_e_type */
  uint8 rrcmcm_procedure_id;/* rrcmcm_abort_flag_e_type Destination procedure for change mode confirmation */
  uint8 rrcmcm_stop_cause;/* rrc_stop_cause_e_type */
  uint8 qsh_rrcwrm_unlock_type; /* trm_unlock_event_enum_t */
  uint8 wcdma_device_mode; /* sys_modem_device_mode_e_type */
  uint8 rrc_dual_mode_enabled :1;
  uint8 rrc_lte_mode_enabled :1;
  uint8 rrc_is_ue_in_sglte_mode :1;
  uint8 stop_cause :1;
  uint8 rrcmcm_send_stop_cnf_to_mm_flag :1;
  uint8 rrcmcm_est_req_saved_flag :1;
  uint8 wcdma_is_ue_in_dual_standby :1;
  uint8 wcdma_is_ue_in_sxlte_mode :1;
}wrrc_qsh_dump_tag_mcm; /* 20 bytes */

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  uint32 rrcmeas_meas_id_bitmask;
  uint32 config_id; /* default_config_id in handover message */
  uint8 misc_globals[150];
  uint8 rrcmeas_current_substate;/* rrcmeas_substate_e_type */
  uint8 rrcidt_substate;
  uint8 rrcudt_substate;
  uint8 rrcscrr_substate;
  uint8 rrciho_state; 
  uint8 rrc_ho_config_type;
  uint8 rrccho_substate; 
  uint8 status;
  uint8 failure_reason;
  uint8 rrcueci_substate;
  uint8 asu_substate;
  uint8 umi_substate;
  uint8 rrcsmc_substate;/* rrcsmc_substate_e_type */
  uint8 rrcrcr_substate; /* rrcrcr_substate_e_type */
  uint8 rrc_test_mode; /*byte*/
  uint8 lbt_substate;
  uint8 couec_substate; /*rrccouec.substate */
  uint8 rrc_dl_amr_mode;
  uint8 ho_from_gsm_eutra:1 ; /* TRUE- GSM, FALSE, EUTRA */
  uint8 rrcdt_int_data:1 ;
  uint8 umi_for_srns_relocation:1 ;
  uint8 rrciho_abort:1;
}wrrc_qsh_dump_tag_misc_proc; /*180 bytes */

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  uint8 used_cctrch_bm;
  uint8 to_be_freed_cctrch_bm;
  uint8	pccpch_is_up:1 ;
  uint8	n_pccpch_is_up:1 ;
  uint8	sccpch_is_up:1 ;
  uint8	dl_dpch_is_up:1 ;
  uint8	fach_included:1 ;
  uint8	pch_included:1 ;
  uint8	pch_included_for_sccpch_with_ctch:1 ;
  uint8	sccpch_with_ctch_is_up:1 ;
}wrrc_qsh_dump_tag_cctrch_debug;/* 3 bytes */

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  rrc_crash_debug_opt_info_type rrc_crash_debug_info_opt; /* 680 bytes */
  /* This OTA in crashdebugging with help in finding the configuration to all the layers in AS and is very helpful in finding the scenari of crash easily */
  uint8 ota[RRC_CRASH_DEBUG_OPT_OTA_SIZE]; /* 1000 */ 
}wrrc_qsh_dump_tag_cmds_otas_debug; /* 1680 bytes */


/* Below data structures are defined for metrics */
/* WRRC doesnt maintain any arrays. QSH takes care of maintaining the arrays for each of the metrics */
/* WRRC Just logs the events whenever they are triggered */
typedef enum{
  WRRC_NON_HHO,
  WRRC_TIMING_INIT,
  WRRC_TIMING_MAINTAINED,
  WRRC_REDIRECTION,
  WRRC_HHO_INVALID
}wrrc_hho_type_e;

typedef enum
{
  WRRC_QSH_METRIC_RESEL_CEL_SEL, /* wrrc_qsh_metric_resel_cel_sel_s */
  WRRC_QSH_METRIC_RLF_OOS, /* wrrc_qsh_metric_rlf_oos_s */
  WRRC_QSH_METRIC_HO, /* wrrc_qsh_metric_ho_s */
  WRRC_QSH_METRIC_ASET, /* wrrc_qsh_metric_aset_s */
  WRRC_QSH_METRIC_SERVING, /* wrrc_qsh_metric_serving_cell_s */
  WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS,
  WRRC_QSH_METRIC_OTA,/* wrrc_qsh_metric_ota_s */
  WRRC_QSH_METRIC_MM_TO_RRC_CMD,/* wrrc_qsh_metric_mm_to_rrc_cmd_s */
  WRRC_QSH_METRIC_RRC_TO_MM_CMD,/* wrrc_qsh_metric_rrc_to_mm_cmd_s */
  WRRC_QSH_METRIC_RR_TO_RRC_CMD,/* wrrc_qsh_metric_rr_to_rrc_cmd_s */
  WRRC_QSH_METRIC_RRC_TO_RR_CMD,/* wrrc_qsh_metric_rrc_to_rr_cmd_s */
  WRRC_QSH_METRIC_RRC_TO_L1_CMD,/* wrrc_qsh_metric_rrc_to_l1_cmd_s */
  WRRC_QSH_METRIC_L1_TO_RRC_CMD,/* wrrc_qsh_metric_l1_to_rrc_cmd_s */
  WRRC_QSH_METRIC_RRC_TO_LTE_CMD,/* wrrc_qsh_metric_rrc_to_lte_cmd_s */
  WRRC_QSH_METRIC_LTE_TO_RRC_CMD,/* wrrc_qsh_metric_lte_to_rrc_cmd_s */
  WRRC_QSH_METRIC_INTERNAL_CMD,/* wrrc_qsh_metric_internal_cmd_s */
  WRRC_QSH_METRIC_LLC_CMD,/* wrrc_qsh_metric_llc_cmd_s */
  WRRC_QSH_METRIC_RRC_STATE,/* wrrc_qsh_metric_rrc_state_history */
  WRRC_QSH_METRIC_MEAS_ASU_OTA,/*wrrc_qsh_metric_meas_asu_ota_s */
  WRRC_QSH_METRIC_CONN_END_INFO,/*wrrc_qsh_metric_call_end_info_s_type*/
  WRRC_QSH_METRIC_MULTI_RAB_STATUS,/*wrrc_qsh_metric_multi_rab_status_s_type*/
  WRRC_QSH_METRIC_CELL_UPDATE_CAUSE,/*wrrc_qsh_metric_cell_update_cause_s_type*/
  WRRC_QSH_METRIC_MAX
}wrrc_qsh_metric_e;

typedef struct{
  qsh_metric_hdr_s hdr; 
  uint8 num_u_to_l_resel;
  uint8 num_u_to_u_resel;
  uint8 num_u_to_g_resel;
  uint8 num_resel_fail;  
  uint8 num_cel_sel_fail;  
}wrrc_qsh_metric_resel_cel_sel_s;

typedef struct{
  qsh_metric_hdr_s hdr; 
  uint8 num_rlf;
  uint8 num_oos;
  uint8 num_con_reject;
}wrrc_qsh_metric_rlf_oos_s;

typedef struct {
  qsh_metric_hdr_s hdr; 
  uint8 num_u_to_l_ho;
  uint8 num_u_to_g_ho;
  uint8 num_inter_rat_ho_fail;/* only where W is originating RAT we log the failure, eg W->G*/ 
  uint8 num_srvcc;
  uint8 num_csfb;
  uint8 num_u_to_u_hho;
  uint8 num_hho_fail;
}wrrc_qsh_metric_ho_s; 

typedef struct {
  qsh_metric_hdr_s hdr; 
  uint8 num_rl;
}wrrc_qsh_metric_aset_s;
/* Non_HSUPA_Capable_Cell-0,HSUPA_Capable_Cell-1, HSUPA_Curently_Configured-2, DCHSUPA_Currently_Configured-3 */
typedef enum
{
  NON_HSDPA_CAPABLE,
  HSDPA_CAPABLE,
  HSDPA_UNKNOWN
}wrrc_hsdpa_cell_capability_enum_type;

typedef enum
{
  NON_HSUPA_CAPABLE,
  HSUPA_CAPABLE,
  HSUPA_UNKNOWN 
}wrrc_hsupa_cell_capability_enum_type;

/* Transition cnf and channel config cnf is the trigger to log these */
typedef struct {
  qsh_metric_hdr_s hdr;
  uint32 cell_id;
  boolean csg_indication;
  uint16 drx_cycle;/* OTA or sib configured */   
  uint8 rrc_state;
  uint16 uarfcn;
  uint16 psc;
  wrrc_hsdpa_cell_capability_enum_type hsdpa_cell_capability;/* Need to define an enum */
  boolean hsdpa_call_status; /* TRUE if HSDPA is configured */
  wrrc_hsupa_cell_capability_enum_type hsupa_cell_capability;/* Need to define an enum */
  boolean hsupa_call_status;
  boolean fdpch_status;  
  boolean comp_mode_status;
  int8 ul_interference;
  boolean cpc_dtx_status;
  boolean cpc_drx_status;
}wrrc_qsh_metric_serving_cell_s;

typedef struct {
  qsh_metric_hdr_s hdr;
  uint8 t313; 
  uint8 t300;
  uint8 t302;
  uint8 n313;
  uint8 n315;
  uint8 n300;
  uint8 n302;
  uint8 t314;
  uint16 psc;
}wrrc_qsh_metric_tmr_and_const_s;

typedef struct{
  uint8 num_rb_add;
  uint8 num_rb_rel;
  uint8 num_rb_recfg;
  uint8 oc_set_status; /* or state indicator */
  uint8 hho_type; /*non-hho, timing init, timing maintained, redirection. For non-dch otas, if freq info is included save this as redirection*/
  uint8 mapping_combo_srb; /* save it for SRB2 */
  uint8 mapping_combo_drb; /* save it only for 1st PS RB */
  uint8 srns_relocation_invloved:1;
  uint8 rlc_size_type :1; /* fixed or flexible for 1st PS RB	*/
  uint8 one_sided_re_est :1;
  uint8 cpc_dtx_drx_status :1;
  uint8 edch_transmission :1; /* even though mapping is provided, if some other edch info is missn this is false, so it may be important to save this */
  uint8 sec_edch_transmission :1;
  uint8 hsdpa_status :1; 
  uint8 dchsdpa_status :1;
}rrc_overall_ota_info_s;

typedef struct{
  rrc_overall_ota_info_s overall_ota;
  uint8 cap_requests;
}rrc_consetup_ota_parm_s;

typedef struct{
  rrc_overall_ota_info_s overall_ota;
  uint8 reserved;
}rrc_rb_recfg_ota_parm_s;

typedef struct{
  uint8 domain;
  uint8 int_algo;
  uint8 ciph_algo;
  uint8 sec_capability;
}rrc_smc_ota_parms_s;

typedef struct{
  rrc_overall_ota_info_s overall_ota;
  uint8 srb_re_est_ind :1;
  uint8 drb_re_est_ind :1;
  uint8 new_crnti_present :1;
  uint8 new_urnti_present :1;
}rrc_cuc_ota_param_s;

typedef struct{
  uint8 cell_update_cause;
  uint8 est_cause;
  uint8 rsi;
  uint8 rue_on_srb; /* RLC unrecoverable error */
  uint8 rue_on_drb; 
  uint8 security_revert_status;
  uint8 failure_cause;
  uint8 t314_expired:1;
  uint8 t315_expired:1;
}rrc_cu_ota_parms_s;

typedef struct{
  uint8 count_c_activation;
  uint8 failure_cause;/* only for failure messages */
  uint8 ul_integrity_info_present:1;
  uint8 ul_counter_sync_info:1;
}rrc_ul_ota_parms_s;

typedef struct{
  boolean new_crnti_valid; /*Indicates if a new crnti is recvd */
  boolean new_urnti_valid; /* Indicates if a valid U-RNTI was recd */ 
  boolean umi_for_srns_relocation;  
  boolean new_hrnti_valid;   
  uint8 timer_const_rcvd;
}rrc_umi_ota_parms_s;

typedef struct{
  rrc_overall_ota_info_s overall_ota;
  uint8 reserved;
}rrc_ho_to_utran_parm_s;

typedef struct
{
  uint32 fail_cause; /* DCS1800 or PCS 1900 */
}rrc_log_rrc_ho_from_utran_fail_ota;

typedef struct
{
  uint8 cn_id; /* Redirected RAT */
}rrc_log_rrc_scrr_ota;

typedef struct
{
  uint8 mcf_fail_cause; /* Measurement control failure cause */
}rrc_log_rrc_mcf_ota;

typedef struct
{
  uint8 cco_fail_cause; /* Cell change order failure cause */
}rrc_log_rrc_cco_fail_ota;

typedef struct
{
  ARFCN_T    BCCH_ARFCN;   /* indicates the Target GSM Cell BCCH ARFCN */
  uint8      BSIC_NCC;  /* indicates the Target GSM Cell BSIC */
  uint8      BSIC_BCC;
}rrc_log_rrc_cco_ota;


typedef struct
{
  uint8 lte_container_length; /* LTE capability container size */
}rrc_log_rrc_conn_setup_compl_ota;

typedef struct
{
  uint8    sys_info_mod:1; /* Sys info modification present */
  uint8    page_record_match:1; /* Page record match found */
  uint8    cn_id:1;  /* Domain id */
  uint8    paging_cause; /* Paging cause */
}rrc_log_pg_ota;


typedef struct
{
  uint8 conn_rej_cause; /* RRC connection reject cause */
  uint8 wait_time; /* RRC conn reject wait time */
  uint8 redir_info_present:1; /* Redirection info present */
  uint8 redir_rat; /* Redirected RAT */
}rrc_log_rrc_conn_rej_ota;

typedef struct
{
  uint8 conn_rel_cause; /* RRC connection release cause */
  uint8 redir_info_present:1; /* Redirection info present */
  uint8 redir_rat; /* Redirected RAT */
}rrc_log_rrc_conn_rel_ota;

typedef struct
{
  uint8 cn_id;
  uint8 est_cause;  /* Connection establishment cause */
  uint8 as_rel_indicator; /* UE release indicator */
}rrc_log_rrc_conn_req_ota;


typedef struct
{
  uint8 cn_id; /* Domain id */
  uint8 est_cause; /* IDT establishment cause */
}rrc_log_rrc_idt_ota;

typedef struct
{
  uint8 cn_id; /* Redirected RAT */
  uint8 scri_cause; /* SCRI cause */
}rrc_log_rrc_scri_ota;

typedef struct
{
  uint8 freq_band:1; /* DCS1800 or PCS 1900 */
  uint8 ho_msg_type; /* Single GSM message or Message list */
}rrc_log_rrc_ho_gsm_ota;


typedef union {
  rrc_consetup_ota_parm_s con_setup;
  rrc_rb_recfg_ota_parm_s rb_recfg;
  rrc_smc_ota_parms_s smc_params;
  rrc_cuc_ota_param_s cuc_params;
  rrc_cu_ota_parms_s cu_params;
  rrc_ul_ota_parms_s ul_ota_parms; /* recfg cplt and rbsetup complete */
  rrc_umi_ota_parms_s umi_ota_params;
  rrc_ho_to_utran_parm_s ho_to_utran_params;
  rrc_log_rrc_ho_gsm_ota ho_to_gsm;
  rrc_log_rrc_ho_from_utran_fail_ota ho_from_utran_fail_params;
  rrc_log_rrc_scrr_ota sig_conn_rel;
  rrc_log_rrc_scri_ota sig_conn_rel_ind;
  rrc_log_rrc_idt_ota idt_params;
  rrc_log_rrc_cco_fail_ota cco_fail_params;
  rrc_log_rrc_cco_ota cco_params;
  rrc_log_rrc_conn_setup_compl_ota conn_setup_compl_params;
  rrc_log_rrc_conn_req_ota conn_req_ota;
  rrc_log_rrc_conn_rel_ota conn_rel_ota;
  rrc_log_rrc_conn_rej_ota conn_rej_ota;
  rrc_log_pg_ota paging_type1;
  rrc_log_pg_ota paging_type2;
}rrc_ota_param_u;

typedef enum {
	WRRC_UL_CCCH_MESSAGETYPE_CELLUPDATE,
	WRRC_UL_CCCH_MESSAGETYPE_RRCCONNECTIONREQUEST,
	WRRC_UL_CCCH_MESSAGETYPE_URAUPDATE,
	WRRC_DL_CCCH_MESSAGETYPE_CELLUPDATECONFIRM,
	WRRC_DL_CCCH_MESSAGETYPE_RRCCONNECTIONREJECT,
	WRRC_DL_CCCH_MESSAGETYPE_RRCCONNECTIONRELEASE,
	WRRC_DL_CCCH_MESSAGETYPE_RRCCONNECTIONSETUP,
	WRRC_DL_CCCH_MESSAGETYPE_URAUPDATECONFIRM,
	WRRC_UL_DCCH_MESSAGETYPE_ACTIVESETUPDATECOMPLETE,
	WRRC_UL_DCCH_MESSAGETYPE_ACTIVESETUPDATEFAILURE,
	WRRC_UL_DCCH_MESSAGETYPE_CELLCHANGEORDERFROMUTRANFAILURE,
	WRRC_UL_DCCH_MESSAGETYPE_COUNTERCHECKRESPONSE,
	WRRC_UL_DCCH_MESSAGETYPE_HANDOVERTOUTRANCOMPLETE,
	WRRC_UL_DCCH_MESSAGETYPE_INITIALDIRECTTRANSFER,
	WRRC_UL_DCCH_MESSAGETYPE_HANDOVERFROMUTRANFAILURE,
	WRRC_UL_DCCH_MESSAGETYPE_MEASUREMENTCONTROLFAILURE,
	WRRC_UL_DCCH_MESSAGETYPE_MEASUREMENTREPORT,
	WRRC_UL_DCCH_MESSAGETYPE_PHYSICALCHANNELRECONFIGURATIONCOMPLETE,
	WRRC_UL_DCCH_MESSAGETYPE_PHYSICALCHANNELRECONFIGURATIONFAILURE,
	WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRECONFIGURATIONCOMPLETE,
	WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRECONFIGURATIONFAILURE,
	WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRELEASECOMPLETE,
	WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRELEASEFAILURE,
	WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERSETUPCOMPLETE,
	WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERSETUPFAILURE,
	WRRC_UL_DCCH_MESSAGETYPE_RRCCONNECTIONRELEASECOMPLETE,
	WRRC_UL_DCCH_MESSAGETYPE_RRCCONNECTIONSETUPCOMPLETE,
	WRRC_UL_DCCH_MESSAGETYPE_RRCSTATUS,
	WRRC_UL_DCCH_MESSAGETYPE_SECURITYMODECOMPLETE,
	WRRC_UL_DCCH_MESSAGETYPE_SECURITYMODEFAILURE,
	WRRC_UL_DCCH_MESSAGETYPE_SIGNALLINGCONNECTIONRELEASEINDICATION,
	WRRC_UL_DCCH_MESSAGETYPE_TRANSPORTCHANNELRECONFIGURATIONCOMPLETE,
	WRRC_UL_DCCH_MESSAGETYPE_TRANSPORTCHANNELRECONFIGURATIONFAILURE,
	WRRC_UL_DCCH_MESSAGETYPE_TRANSPORTFORMATCOMBINATIONCONTROLFAILURE,
	WRRC_UL_DCCH_MESSAGETYPE_UECAPABILITYINFORMATION,
	WRRC_UL_DCCH_MESSAGETYPE_UPLINKDIRECTTRANSFER,
	WRRC_UL_DCCH_MESSAGETYPE_UTRANMOBILITYINFORMATIONCONFIRM,
	WRRC_UL_DCCH_MESSAGETYPE_UTRANMOBILITYINFORMATIONFAILURE,
	WRRC_UL_DCCH_MESSAGETYPE_MBMSMODIFICATIONREQUEST,
	WRRC_UL_DCCH_MESSAGETYPE_UL_DCCH_MESSAGETYPE_EXT,
	WRRC_DL_DCCH_MESSAGETYPE_ACTIVESETUPDATE,
	WRRC_DL_DCCH_MESSAGETYPE_ASSISTANCEDATADELIVERY,
	WRRC_DL_DCCH_MESSAGETYPE_CELLCHANGEORDERFROMUTRAN,
	WRRC_DL_DCCH_MESSAGETYPE_CELLUPDATECONFIRM,
	WRRC_DL_DCCH_MESSAGETYPE_COUNTERCHECK,
	WRRC_DL_DCCH_MESSAGETYPE_DOWNLINKDIRECTTRANSFER,
	WRRC_DL_DCCH_MESSAGETYPE_HANDOVERFROMUTRANCOMMAND_GSM,
	WRRC_DL_DCCH_MESSAGETYPE_HANDOVERFROMUTRANCOMMAND_CDMA2000,
	WRRC_DL_DCCH_MESSAGETYPE_MEASUREMENTCONTROL,
	WRRC_DL_DCCH_MESSAGETYPE_PAGINGTYPE2,
	WRRC_DL_DCCH_MESSAGETYPE_PHYSICALCHANNELRECONFIGURATION,
	WRRC_DL_DCCH_MESSAGETYPE_PHYSICALSHAREDCHANNELALLOCATION,
	WRRC_DL_DCCH_MESSAGETYPE_RADIOBEARERRECONFIGURATION,
	WRRC_DL_DCCH_MESSAGETYPE_RADIOBEARERRELEASE,
	WRRC_DL_DCCH_MESSAGETYPE_RADIOBEARERSETUP,
	WRRC_DL_DCCH_MESSAGETYPE_RRCCONNECTIONRELEASE,
	WRRC_DL_DCCH_MESSAGETYPE_SECURITYMODECOMMAND,
	WRRC_DL_DCCH_MESSAGETYPE_SIGNALLINGCONNECTIONRELEASE,
	WRRC_DL_DCCH_MESSAGETYPE_TRANSPORTCHANNELRECONFIGURATION,
	WRRC_DL_DCCH_MESSAGETYPE_TRANSPORTFORMATCOMBINATIONCONTROL,
	WRRC_DL_DCCH_MESSAGETYPE_UECAPABILITYENQUIRY,
	WRRC_DL_DCCH_MESSAGETYPE_UECAPABILITYINFORMATIONCONFIRM,
	WRRC_DL_DCCH_MESSAGETYPE_UPLINKPHYSICALCHANNELCONTROL,
	WRRC_DL_DCCH_MESSAGETYPE_URAUPDATECONFIRM,
	WRRC_DL_DCCH_MESSAGETYPE_UTRANMOBILITYINFORMATION,
	WRRC_DL_DCCH_MESSAGETYPE_HANDOVERFROMUTRANCOMMAND_GERANIU,
	WRRC_DL_DCCH_MESSAGETYPE_MBMSMODIFIEDSERVICESINFORMATION,
	WRRC_DL_DCCH_MESSAGETYPE_ETWSPRIMARYNOTIFICATIONWITHSECURITY,
	WRRC_DL_DCCH_MESSAGETYPE_HANDOVERFROMUTRANCOMMAND_EUTRA,
	WRRC_DL_DCCH_MESSAGETYPE_UEINFORMATIONREQUEST,
	WRRC_DL_DCCH_MESSAGETYPE_LOGGINGMEASUREMENTCONFIGURATION,
	WRRC_BCCH_FACH_MESSAGETYPE_SYSTEMINFORMATION,
	WRRC_BCCH_FACH_MESSAGETYPE_SYSTEMINFORMATIONCHANGEINDICATION,
	WRRC_PCCH_MESSAGETYPE_PAGINGTYPE1,
	WRRC_DL_DCCH_MESSAGETYPE_HANDOVERTOUTRAN,
	WRRC_MESSAGETYPE_INVALID
}wrrc_qsh_ota_msg_id;
typedef struct {
  qsh_metric_hdr_s hdr;
  wrrc_qsh_ota_msg_id msg_id;
  rrc_ota_param_u ota_info;
}wrrc_qsh_metric_ota_s;

typedef struct
{
  uint8 meas_id;  /* Measurement identity */
  uint8 meas_type; /* Intra/inter/irat/TVM/POS/Internal */ 
  uint8 meas_rpt_type; /* Event or periodic or additional */
  uint8 meas_event_id_bmask; /* Bitmask indicating the event */
}rrc_log_rrc_mcm_ota;

typedef struct
{
  uint8 meas_id;  /* Measurement identity */
  uint8 meas_type; /* Intra/inter/irat/TVM/quality */ 
  uint8 meas_rpt_type; /* Event or periodic or additional */
  uint8 event_id; /* Bitmask indicating event */
  uint8 num_cells_rpt; /* Number of cell reported */
}rrc_log_rrc_mrm_ota;

typedef struct{
  rrc_overall_ota_info_s overall_ota;
  uint8 num_rls_added;
  uint8 num_rls_removed;	
}rrc_dl_asu_ota_parms_s;

typedef struct{
  uint32 trans_id;
  uint8 failure_cause;/* only for failure messages */
  uint8 ul_integrity_info_present:1;
}rrc_ul_asu_ota_parms_s;
	
typedef union{
  rrc_dl_asu_ota_parms_s asu_info;
  rrc_ul_asu_ota_parms_s ul_asu_info;
  rrc_log_rrc_mcf_ota mcf_params;
  rrc_log_rrc_mrm_ota mrm_ota;
  rrc_log_rrc_mcm_ota mcm_ota;
}rrc_asu_meas_ota_param_u;

typedef struct {
  qsh_metric_hdr_s hdr;
  wrrc_qsh_ota_msg_id msg_id;
  rrc_asu_meas_ota_param_u asu_meas_info;
}wrrc_qsh_metric_meas_asu_ota_s;

typedef union
{
/* Connection req to be added */
rrc_cu_ota_parms_s cu_params;
rrc_log_rrc_mcf_ota mcf_params;
rrc_ul_asu_ota_parms_s ul_asu_info;
rrc_log_rrc_cco_fail_ota cco_fail_params;
rrc_ul_ota_parms_s ul_ota_parms; 
rrc_log_rrc_ho_from_utran_fail_ota ho_from_utran_fail_params;
}rrc_qsh_ul_ota_u;
typedef enum {
RRC_OTA_INVALID,
RRC_OTA_UL_FAILURE,
RRC_OTA_UL_CPLT,
RRC_OTA_CU,
RRC_OTA_MEAS,
RRC_OTA_ASU,
RRC_OTA_CHO,
RRC_OTA_IHO,
RRC_OTA_MAX
}rrc_qsh_ul_ota_type_e;

typedef struct {
rrc_qsh_ul_ota_type_e ul_ota_type;
uint8 msg_id;  
rrc_qsh_ul_ota_u ul_ota;
}rrc_qsh_ul_ota_param_s;

typedef uint32 qsh_gen_u32_config_type;

typedef struct
{
  /*
    RRC_EST_ORIG_CONV_CALL - 0
    RRC_EST_ORIG_STREAM_CALL -1
    RRC_EST_ORIG_INTERACT_CALL - 2
    RRC_EST_ORIG_BACKGND_CALL - 3
    RRC_EST_ORIG_SUBSCRIBED_TRF_CALL - 4
    RRC_EST_TERM_CONV_CALL - 5
    RRC_EST_TERM_STREAM_CALL - 6
    RRC_EST_TERM_INTERACT_CALL - 7
    RRC_EST_TERM_BACKGND_CALL - 8
    RRC_EST_EMERGENCY_CALL - 9
    RRC_EST_INTER_RAT_CELL_RESELECT -10
    RRC_EST_INTER_RAT_CELL_CHANGE_ORDER - 11
    RRC_EST_REGISTRATION - 12
    RRC_EST_DETACH - 13
    RRC_EST_HI_PRI_SIGNALLING - 14
    RRC_EST_LOW_PRI_SIGNALLING - 15
    RRC_EST_CALL_RE_ESTABLISH -16
    RRC_EST_TERM_HI_SIGNALLING -17
    RRC_EST_TERM_LOW_SIGNALLING -18
    RRC_EST_TERM_UNKNOWN - 19 
  */
  uint8       cause;
  /*CN Domain Identity 0- CS,1-PS*/
  boolean     cn_domain_id;
  /*
    Call Type RRC_CALL_TYPE_SPEECH - 0
    RRC_CALL_TYPE_VIDEO - 1
    RRC_CALL_TYPE_OTHERS - 2
    RRC_CALL_TYPE_NONE - 3 
  */
  uint8  call_type;
}qsh_rrc_est_req_type;

typedef struct
{
  /*
    RRC_NORMAL_ACQ - 0
    RRC_MO_CS_CALL - 1
    RRC_MO_PS_CALL - 2
    RRC_MO_CS_ECALL - 3
  */
  uint8 service_req_cause;
}qsh_rrc_service_req_type;

typedef struct
{
  /* 0 - CS, 1- PS */
  boolean     cn_domain_id;
  /* 
    est_cause QSH_RRC_EST_ORIG_CONV_CALL - 0
    QSH_RRC_EST_ORIG_STREAM_CALL 1,
    QSH_RRC_EST_ORIG_INTERACT_CALL 2
    QSH_RRC_EST_ORIG_BACKGND_CALL 3
    QSH_RRC_EST_ORIG_SUBSCRIBED_TRF_CALL 4
    QSH_RRC_EST_TERM_CONV_CALL 5
    QSH_RRC_EST_TERM_STREAM_CALL 6
    QSH_RRC_EST_TERM_INTERACT_CALL 7
    QSH_RRC_EST_TERM_BACKGND_CALL 8
    QSH_RRC_EST_EMERGENCY_CALL 9
    QSH_RRC_EST_INTER_RAT_CELL_RESELECT 10
    QSH_RRC_EST_INTER_RAT_CELL_CHANGE_ORDER 11
    QSH_RRC_EST_REGISTRATION 12
    QSH_RRC_EST_DETACH 13
    QSH_RRC_EST_HI_PRI_SIGNALLING 14
    QSH_RRC_EST_LOW_PRI_SIGNALLING 15
    QSH_RRC_EST_CALL_RE_ESTABLISH 16
    QSH_RRC_EST_TERM_HI_SIGNALLING 17
    QSH_RRC_EST_TERM_LOW_SIGNALLING 18
    QSH_RRC_EST_TERM_UNKNOWN 19
  */
  uint8 est_cause;
  /*
    Call Type RRC_CALL_TYPE_SPEECH - 0
    RRC_CALL_TYPE_VIDEO - 1
    RRC_CALL_TYPE_OTHERS - 2
    RRC_CALL_TYPE_NONE - 3 
  */
  uint8  call_type;
}qsh_rrc_open_session_req_type;


typedef struct
{
  boolean delay_sig_con_release;
  boolean auth_failed_bar_cells;
  boolean hard_abort;
  /* 
   RRC_UE_INITIATED_DORMANCY - 0
   RRC_UE_HIGHER_PRIO_CALL - 1
   RRC_DUAL_STANDBY_PS_ABORT - 2
   RRC_OTHERS - 3
  */
  uint8 abort_cause;
}qsh_rrc_abort_req_type;

typedef struct 
{
  boolean dual_standby_mode;

  /* dual_standby_mode_info
    SYS_MODEM_DS_PREF_NONE - Internal use, range checking - 0
    SYS_MODEM_DS_PREF_SINGLE_STANDBY - Phone is in Single Standby mode - 1
    SYS_MODEM_DS_PREF_DUAL_STANDBY - Phone is in Dual Standby mode - 2
    SYS_MODEM_DS_PREF_NO_CHANGE - Do not change the Dual Standby preference - 3
    SYS_MODEM_DS_PREF_DUAL_STANDBY_NO_TUNEAWAY - Phone is in Dual Standby mode with no tune away - 4
    SYS_MODEM_DS_PREF_AUTO - Standby mode is determined by the number of subscriptions - 5
    SYS_MODEM_DS_PREF_AUTO_NO_TUNEAWAY - Standby mode is determined by the number of subscriptions With no tuneaway - 6
    SYS_MODEM_DS_PREF_TRIPLE_STANDBY - TSTS mode - 7
    SYS_MODEM_DS_PREF_TRIPLE_STANDBY_NO_TUNEAWAY - 8
    SYS_MODEM_DS_PREF_NO_STANDBY_PREF - None of th stacks will be active - 9
    SYS_MODEM_DS_PREF_MAX - Internal use, range checking - 10
  */
  uint8  dual_standby_mode_info;
  /* device_mode
    SYS_MODEM_DEVICE_MODE_NONE - Internal use, range checking - 255
    SYS_MODEM_DEVICE_MODE_SINGLE_SIM - Device is single sim - 0
    SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY - Device is dual sim dual standy - 1
    SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE - Device is dual sim dual active - 2
    SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY - Device is Triple sim Triple standy - 3
    SYS_MODEM_DEVICE_MODE_MAX - Internal use for range checking - 4
  */
  uint8   device_mode;
}qsh_rrc_ds_status_change_ind_type;

typedef union{
  qsh_rrc_est_req_type           rrc_est_req; /* RRC_EST_REQ */
  qsh_rrc_service_req_type       rrc_service_req; /* RRC_SERVICE_REQ */
  qsh_rrc_open_session_req_type  open_sess_req; /* RRC_OPEN_SESSION_REQ */
  qsh_rrc_abort_req_type         rrc_abort_req; /* RRC_ABORT_REQ */
  qsh_rrc_ds_status_change_ind_type  rrc_ds_status_change_ind; /* RRC_DS_STATUS_CHANGE_IND */
  qsh_gen_u32_config_type gen_config;
}mm_rrc_u_type;

typedef enum
{

  QSH_RRC_MM_CMD_BASE = 0x0000,  /* MM commands start here */

  /* 0x0001*/
  QSH_RRC_SERVICE_REQ,

  /* 0x0002 */
  QSH_RRC_EST_REQ,

  /* 0x0003 */
  QSH_RRC_DATA_REQ,

  /* 0x0004 */
  QSH_RRC_OPEN_SESSION_REQ,

  /* 0x0005 */
  QSH_RRC_CLOSE_SESSION_REQ,

  /* 0x0006 */
  QSH_RRC_ABORT_REQ,

  /* 0x0007 */
  QSH_RRC_PLMN_LIST_REQ,

  /* 0x0008 */
  QSH_RRC_ACT_REQ,

  /* 0x0009 */
  QSH_RRC_DEACT_REQ,

  /* 0x000A */
  QSH_RRC_STOP_WCDMA_MODE_REQ,

  /* 0x000B */
  QSH_RRC_FORBIDDEN_LAI_LIST_UPDATE_REQ,

  /* 0x000C */
  QSH_RRC_INVALIDATE_SIM_DATA_REQ,

  /* 0x000D */
  QSH_RRC_SIM_INSERTED_REQ,

  /* 0x000E */
  QSH_RRC_SIM_UPDATE_REQ,

  /* 0x000F */
  QSH_RRC_ACTIVATION_RSP,

  /* 0x0010 */
  QSH_RRC_CHANGE_MODE_IND,

  /* 0x0011 */
  QSH_RRC_MODE_CHANGE_REQ,

  /* 0x0012 */
  QSH_RRC_EQ_PLMN_LIST_CHANGE_IND,

  /* 0x0013 */
  QSH_RRC_NW_SEL_MODE_RESET_IND,

  /* 0x0014 */
  QSH_RRC_BPLMN_SEARCH_ABORT_REQ,    /* Added for WTOW/WTOG BPLMN SEARCH ABORT */

  /* 0x0015 */
  QSH_RRC_EXIT_FROM_PWR_SAVE,

  /* 0x0016 */
  QSH_RRC_DS_STATUS_CHANGE_IND,

  /* 0x0017 */
  QSH_RRC_FAST_DORMANCY_REQ,       /*Added for fast dormancy request trigger*/

  /* 0x0018 */
  QSH_RRC_DS_TUNEAWAY_STATUS_CHANGE_IND,

  /* 0x0019 */
  QSH_RRC_CSFB_CALL_STATUS_IND, /*CSFB call status indication*/

  /* 0x001A */
  QSH_RRC_LTE_RESELECTION_ALLOWED_STATUS_IND,

  /* 0x001B */
  QSH_RRC_LAI_REJECT_LIST_IND,

  /* 0x001C */
  QSH_RRC_TUNEAWAY_ALLOWED_IND,

  /* 0x001D */
  QSH_RRC_SET_DRX_IND,

  /* 0x001E */
  QSH_RRC_CSG_WHITE_LIST_UPDATE_IND,

  /*Used by TDS */
  /* 0x001F */
  QSH_RRC_UE_MODE_IND,

  /* 0x0020*/
  QSH_RRC_MODE_PREF_CHANGE_IND,

  /* 0x0021*/
  QSH_RRC_MULTIMODE_SUBS_CHGD_IND,

  /* 0x0022*/
  QSH_RRC_PROCEDURE_IND,

}qsh_mm_rrc_cmd_e_type;

typedef struct {
  qsh_metric_hdr_s hdr;
  qsh_mm_rrc_cmd_e_type msg_id;       
  mm_rrc_u_type config; 
}wrrc_qsh_metric_mm_to_rrc_cmd_s;

typedef struct
{
  /* rel_cause
    RRC_REL_CAUSE_NORMAL - 0
    RRC_REL_CAUSE_UNSPEC - 1
    RRC_REL_CAUSE_PRE_EMPTIVE - 2
    RRC_REL_CAUSE_CONGESTION - 3
    RRC_REL_CAUSE_RE_ESTABLISH_REJECT - 4
    RRC_REL_USER_INACTIVITY - 5
    RRC_REL_DEEP_FADE - 6
    RRC_DIRECTED_SIGNALLING_REESTABLISHMENT - 7
    RRC_REL_CAUSE_OOS - 8
    RRC_UE_INIT_DORMANCY_UE_IN_OOS - 9
    RRC_UE_INIT_DORMANCY_ABORT_SUCCESS - 10
    RRC_REL_DUAL_STANDBY_PS_ABORT_SUCCESS - 11
    RRC_CONN_REL_CAUSE_MAX - 12
  */
  uint8    rel_cause;
}qsh_rrc_rel_ind_type;

typedef struct
{
  /* 0 - CS, 1 - PS */
  boolean   cn_domain_id;
  /* Cause  
    RRC_RAB_ESTABLISHED 0
    RRC_CIPHERING_ESTABLISHED 1
    RRC_INTEGRITY_ESTABLISHED 2
    RRC_INTEGRITY_CIPHERING_ESTABLISHED 3
    RRC_LOOPBACK_ESTABLISHED 4
    RRC_HANDOVER 5
    RRC_CIPHERING_CHANGE 6 
  */
  uint8   cause;
  /* NAS should look at this field only if the CN domain is CS. Since the requirement is for voice call.
   * RRC will not set this field properly for PS call currently.
   * ciphering_enabled = TRUE :- indicate ciphering is enabled for CS call
   * ciphering_enabled = FALSE :- indicate ciphering is disabled for CS call
   */
  boolean ciphering_enabled; 
}qsh_rrc_sync_ind_type;

typedef struct
{
  /*
    MM_AS_NO_SERVICE        - 0
    MM_AS_SERVICE_AVAILABLE - 1
  */
  boolean       service_status;

  /* 
    Service confirmation status 
    MM_AS_SUCCESS - 0
    MM_AS_CONNECTED -1
    MM_AS_INTERRAT_RESELECTION - 2
    MM_AS_INTERRAT_HANDOVER - 3
    MM_AS_INTERRAT_CC_ORDER - 4
    MM_AS_WTOG_INTERRAT_CC_ORDER - 5
    MM_AS_WTOG_CC_ORDER_FAILED - 6
    MM_AS_GTOW_CC_ORDER - 7
    MM_AS_GTOW_CC_ORDER_FAILED - 8 
    MM_AS_INTERRAT_REDIRECTION - 9
    MM_AS_ABORTED -10
    MM_AS_GTOW_REDIRECTION - 11
    MM_AS_INTERRAT_PS_HANDOVER - 12
    MM_AS_LTOW_SRVCC_CS_PS_HANDOVER - 13
    MM_AS_LTOW_SRVCC_CS_HANDOVER - 14
    MM_AS_LTOG_SRVCC_CS_HANDOVER - 15
    MM_AS_LTOG_CCO - 16
    MM_AS_SGLTE_INTER_RAT_RESELECTION - 17
    MM_AS_SGLTE_INTER_RAT_REDIRECTION - 18
    MM_AS_LTOG_SGLTE_SRVCC_CS_HANDOVER - 19
    MM_AS_SEARCH_PARTIAL - 20
  */
  uint8              status;


  /* This variable is used to inform NAS whether W
   * search was completed or not in the alloted time.
   * TRUE  => Search Done.
   * FALSE => Otherwise.
   * The value of this variable is valid only when uoos_use_timer
   * in RRC_SERVICE_REQ was set to TRUE and when 
   * available PLMN list is empty.
   */
  boolean                     wcdma_search_done;

  /* This var has been added to inform NAS (using var ue_oos in RRC_SERVICE_CNF) 
   * that service is available. NAS looks at this var, ue_oos, to determine if 
   * limited service request should be sent to RRC or not.
   * Notice that with UOOS, SD always sends Automatic Service Request and it 
   * sends Limited Service Request only when NAS informs SD that Limited Service 
   * is available.
   */
  boolean                     ue_oos;

}qsh_rrc_service_cnf_type;

typedef struct
{
  /*
    MM_AS_NO_SERVICE        - 0
    MM_AS_SERVICE_AVAILABLE - 1
  */
  boolean       service_status;
  
  /* Added to indicate if NO_SERVICE is because of no TRM resource 
    SYS_ACQ_STATUS_SUCCESS - 0 
    SYS_ACQ_STATUS_NO_RESOURCE - 1
  */
  boolean  acq_status;

  /* Added to indicate if NO SERVICE is due to RLF or not. 
    SYS_NO_SVC_CAUSE_NONE - 255 -FOR INTERNAL USE ONLY
    SYS_NO_SVC_CAUSE_RLF - 0 -Radio link failure cause
    SYS_NO_SVC_CAUSE_NORMAL - 1 -Normal cause
    SYS_NO_SVC_CAUSE_IRAT_TO_LTE - 2 -pseudo no service returned by NAS due to reselection to LTE from 3GPP2 RAT
    SYS_NO_SVC_CAUSE_MAX - 3 -FOR INTERNAL USE ONLY 
  */
  uint8  no_service_cause;

}qsh_rrc_service_ind_type;

typedef struct
{
  /* 0 - CS,1 - PS */
  boolean   cn_domain_id;
  /* paging_cause
    RRC_PAGE_CAUSE_TERM_CONV_CALL- 0
    RRC_PAGE_CAUSE_TERM_STREAM_CALL- 1
    RRC_PAGE_CAUSE_TERM_INTERACT_CALL- 2
    RRC_PAGE_CAUSE_TERM_BACKGND_CALL- 3
    RRC_PAGE_CAUSE_SMS - 4
    RRC_PAGE_CAUSE_HIGH_PRI_SIGNALLING - 5
    RRC_PAGE_CAUSE_LOW_PRI_SIGNALLING - 6
    RRC_PAGE_CAUSE_UNKNOWN- 7
  */
  uint8         paging_cause;
}qsh_rrc_page_ind_type;

typedef union
{
  qsh_rrc_rel_ind_type rrc_rel_ind; /* QSH_RRC_REL_IND */ 
  qsh_rrc_sync_ind_type rrc_sync_ind; /* QSH_RRC_SYNC_IND */
  qsh_rrc_service_cnf_type rrc_service_cnf; /* QSH_RRC_SERVICE_CNF */
  qsh_rrc_service_ind_type rrc_service_ind; /* QSH_RRC_SERVICE_IND */
  qsh_rrc_page_ind_type rrc_page_ind; /* QSH_RRC_PAGE_IND */
  qsh_gen_u32_config_type gen_config; 
}rrc_mm_u_type;

typedef enum
{
   QSH_RRC_EST_CNF = 100,
   QSH_RRC_REL_IND,
   QSH_RRC_SYNC_IND,
   QSH_RRC_SERVICE_CNF,
   QSH_RRC_SERVICE_IND,
   QSH_RRC_SERVICE_IND_FROM_OTA_MSG,
   QSH_RRC_PLMN_LIST_CNF,
   QSH_RRC_ABORT_IND,
   QSH_RRC_PAGE_IND,
   QSH_RRC_DATA_IND,
   QSH_RRC_OPEN_SESSION_CNF,
   QSH_RRC_CLOSE_SESSION_IND,
   QSH_RRC_CLOSE_SESSION_CNF,
   QSH_RRC_ABORT_CNF,
   QSH_RRC_DEACT_CNF,
   QSH_RRC_MODE_CHANGE_CNF,
   QSH_RRC_STOP_WCDMA_MODE_CNF,
   QSH_RRC_STOP_TDSCDMA_MODE_CNF = 116,
   QSH_RRC_ACTIVATION_IND,
   QSH_RRC_INTERRAT_CELL_CHANGE_IND,
   QSH_RRC_HSDPA_HSUPA_CELL_IND,
   QSH_RRC_HSPA_CALL_STATUS_IND,
   QSH_RRC_CONNECTED_MODE_SERVICE_STATUS_IND,
   QSH_RRC_FAST_DORMANCY_CNF,
   /*
    * MM Internal cmd same as RR_DATA_IND
    * RRC DATA IND will be transformed into MM_DATA_IND
    */
   QSH_MM_DATA_IND,
   /* RRC sends this primitive after WtoG CCO is successful */
   QSH_RRC_WTOG_CCO_COMPLETE_IND, 
   QSH_RRC_MBMS_CONN_REQ_IND,
   QSH_RRC_SUITABLE_SEARCH_END_IND,
   QSH_RRC_DISABLE_BPLMN_IND,
   QSH_RRC_SRVCC_HANDOVER_FAILURE_IND,
   QSH_RRC_PCH_STATE_IND,
 
   QSH_RRC_MM_SINK_CMD_MAX

}qsh_rrc_mm_cmd_e_type;

typedef struct {
  qsh_metric_hdr_s hdr;
  qsh_rrc_mm_cmd_e_type msg_id;
  rrc_mm_u_type config; 
}wrrc_qsh_metric_rrc_to_mm_cmd_s;

typedef struct
{
  /* Handover status
    RRC_RR_SUCCESS 0 
    RRC_RR_FAILURE 1
    RRC_RR_MAX_STATUS 2
  */
  uint8 handover_status;

  /* Handover failure cause: this field is only valid if handover status is failure
    RRC_RR_INVALID_STATE 0
    RRC_RR_GSM_PROTOCOL_ERROR 1
    RRC_RR_INVALID_CONFIGURATION 2
    RRC_RR_PHYSICAL_CHANNEL_FAILURE 3
    RRC_RR_MAX_FAILURE_CAUSE 4
    RRC_RR_CIPHER_KEY_NOT_AVAILABLE 5
    RRC_RR_UNEXPECTED_FAILURE 6 -Cause to indicate MDSP halt
  */
  uint8 failure_cause;
} qsh_rr_interrat_handover_cnf_type;

typedef struct
{
 
  /* failure_reason
    RR_RRC_REDIRECT_FAIL_OTHER_REASON 0 
    RR_RRC_REDIRECT_FAIL_WAIT_TIMER_EXPIRY 1
    RR_RRC_REDIRECT_FAIL_NO_SUITABLE_CELL_FOUND 2
    RR_RRC_REDIRECT_FAIL_MAX 3 
  */
  uint8 failure_reason;

  /* uoos_gsm_search_done - This variable is used by RR to inform RRC whether GSM
   * search was completed or not in the alloted time.
   * TRUE  => Search Done.
   * FALSE => Otherwise.
   * The value of this variable is valid only when uoos_gsm_use_timer
   * in RR_INTERRAT_REDIRECT_REQ is set to TRUE 
   */
  boolean uoos_gsm_search_done;

} qsh_rr_interrat_redirect_rej_type;

typedef struct
{

  /* Cipher Enable Indicator */
  boolean cipher_enabled;
  /* Indicates how a PLMN is to be selected. 
    SYS_NETWORK_SELECTION_MODE_NONE - 255 -FOR INTERNAL USE ONLY
    SYS_NETWORK_SELECTION_MODE_AUTOMATIC - 0
    SYS_NETWORK_SELECTION_MODE_MANUAL - 1
    SYS_NETWORK_SELECTION_MODE_LIMITED - 2
    SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION - 3
    SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITIO - 4
    SYS_NETWORK_SELECTION_MODE_MANUAL_CSG - 5
    SYS_NETWORK_SELECTION_MODE_AUTOMATIC_CAMP_ONLY - 6 -This is same as Automatic Mode but no registration or OTA message 
      -allowed to sent to n/w. Internal to CM - NAS interface 
    SYS_NETWORK_SELECTION_MODE_MANUAL_CAMP_ONLY - 7
    SYS_NETWORK_SELECTION_MODE_MAX    -FOR INTERNAL USE ONLY
  */
  uint8 network_select_mode;
  /* Indicates CSFB call back Status */
  boolean csfb_call_status;
} qsh_rrc_interrat_handover_req_type;


typedef union {
  qsh_rr_interrat_handover_cnf_type interrat_handover_cnf; /*QSH_RR_INTERRAT_HANDOVER_CNF*/
  qsh_rr_interrat_redirect_rej_type interrat_redirect_rej; /*QSH_RR_INTERRAT_REDIRECT_REJ*/
  qsh_rrc_interrat_handover_req_type interrat_handover_req; /*QSH_RRC_INTERRAT_HANDOVER_REQ*/
  qsh_gen_u32_config_type gen_config;
}rr_rrc_u_type;

typedef enum
{
    QSH_RR_TO_RRC_BASE = 0x0000,
    /* Add new RR to RRC commands below */
    /**********************************************************************/
  
    /* WCDMA to GSM inter-RAT handover confirmation from RR to RRC */
    QSH_RR_INTERRAT_HANDOVER_CNF,
  
    /* WCDMA to GSM inter-RAT handover abort confirmation from RR to RRC */
    QSH_RR_INTERRAT_HANDOVER_ABORT_CNF,
  
    /* WCDMA to GSM inter-RAT Reselection Reject from RR to RRC */
    QSH_RR_INTERRAT_RESELECTION_REJ,
  
    /* WCDMA to GSM inter-RAT CC Order Reject from RR to RRC */
    QSH_RR_INTERRAT_CC_ORDER_REJ,
  
    /* GSM to WCDMA inter-RAT Reselection Request from RR to RRC */
    QSH_RRC_INTERRAT_RESELECTION_REQ,
  
    /* GSM to WCDMA inter-RAT CC Order Request from RR to RRC */
    QSH_RRC_INTERRAT_CC_ORDER_REQ,
  
    /* GSM to WCDMA inter-RAT CC Order Abort Request from RR to RRC */
    QSH_RRC_INTERRAT_CC_ORDER_ABORT_REQ,
  
    /* GSM to WCDMA inter-RAT Reselection Request from RR to RRC */
    QSH_RRC_INTERRAT_HANDOVER_REQ,
    
    /* GSM to WCDMA inter-RAT Reselection Abort Request from RR to RRC */
    QSH_RRC_INTERRAT_HANDOVER_ABORT_REQ,
  
    /* GSM to WCDMA NAS Transfer indication */
    QSH_RRC_NAS_DATA_IND,
  
    /* Background 3G PLMN Search request from RR to RRC */
    QSH_RRC_INTERRAT_PLMN_SRCH_REQ,
  
    /* Background 3G PLMN Search Abort Request from RR to RRC */
    QSH_RRC_INTERRAT_PLMN_SRCH_ABORT_REQ,
  
    /* WCDMA to GSM Inter-RAT Redirection Reject from RR to RRC */
    QSH_RR_INTERRAT_REDIRECT_REJ,
  
    /* WCDMA to GSM Inter-RAT Redirection abort confirm from RR to RRC */
    QSH_RR_INTERRAT_REDIRECT_ABORT_CNF,
  
    /* This command is sent by RR to RRC to abort an on-going GTOW reselection. */
    QSH_RRC_INTERRAT_RESELECTION_ABORT_REQ,
  
    /* This command is sent by RR to RRC after the abort request has been 
     * successfully processed. 
     * Note: The WTOG reselection (Idle) abort cannot fail.
     */
    QSH_RR_INTERRAT_RESELECTION_ABORT_CNF,
  
    /* WCDMA to GSM inter-RAT CC Order Cnf from RR to RRC*/
    QSH_RR_INTERRAT_CC_ORDER_CNF,
  
    /* WCDMA to GSM inter-RAT CC Order Abort Cnf from RR to RRC*/
    QSH_RR_INTERRAT_CC_ORDER_ABORT_CNF,
  
    /* Background 2G PLMN Search Cnf from RR to RRC */
    QSH_RR_INTERRAT_PLMN_SRCH_CNF,
  
    /* Background 2G PLMN Search Abort Cnf from RR to RRC */
    QSH_RR_INTERRAT_PLMN_SRCH_ABORT_CNF
  
    /* Sent from GSM to WCDMA when RR receives ciphering mode 
     * command message which will start ciphering on G for CS domain
     */
    ,QSH_RR_CIPHER_SYNC_IND
  
    /* RR->RRC request to initiate a reselection to GAN procedure. */
    , QSH_RRC_RESELECT_TO_GAN_REQ
  
    /* RR->RRC request for RRC serving cell system information. */
    , QSH_RRC_GAN_CELL_INFO_REQ
    /* RR->RRC request to initiate a reselection from GAN procedure.
    */
    , QSH_RRC_RESELECT_FROM_GAN_REQ
    /*RR->RRC request to go to IDLE_DETACHED_MODE
    */
    ,QSH_RRC_GO_TO_IDLE_DETACHED_REQ
    /*RR->RRC request to wake up WCDMA
    */
    ,QSH_RRC_GAN_WAKE_UP_REQ
    /*RR->RRC request to camp on specific PLMN
    */
    ,QSH_RRC_GAN_ACT_REQ
    ,QSH_RRC_IMR_FROM_GAN_REQ
    ,QSH_RRC_CANCEL_HANDOVER_TO_GAN_REQ 
  
    , QSH_RRC_INTERRAT_REDIRECT_REQ
  
    , QSH_RRC_INTERRAT_REDIRECT_ABORT_REQ
    /*RR->RRC request to stop WCDMA
    */
    , QSH_RRC_RR_STOP_WCDMA_REQ
    /*RR->RRC response to RR_RRC_STOP_GSM_REQ
    */
    ,QSH_RR_RRC_STOP_GSM_CNF
  
    /* RR->RRC Request for dedicated priorities. */
    ,QSH_RRC_DEDICATED_PRIORITIES_REQ
  
    /* RR->RRC Response to request for dedicated priorities. */
    ,QSH_RR_DEDICATED_PRIORITIES_RSP
  
    /* RR->RRC Indicate that RR should clear it's dedicated priorities. */
    ,QSH_RRC_CLEAR_DEDICATED_PRIORITIES_IND

    /* RR->RRC Indicates that GSM has received Stop after sending CCO failure ind to RRC */
    ,QSH_RRC_GRR_INTERRAT_PROC_ABORT_IND

}qsh_rr_rrc_cmd_e_type;

typedef struct {
  qsh_metric_hdr_s hdr;
  qsh_rr_rrc_cmd_e_type msg_id;
  rr_rrc_u_type config; 
}wrrc_qsh_metric_rr_to_rrc_cmd_s;

typedef enum
{
  /* RRC to RR command base. Note: starting from 0 */
  QSH_RRC_TO_RR_BASE = 0x0000,
  /* Add RRC to RR command below */

  /* WCDMA to GSM inter-RAT handover request from RRC to RR */
  QSH_RR_INTERRAT_HANDOVER_REQ,

  /* WCDMA to GSM inter-RAT handover abort request from RRC to RR */
  QSH_RR_INTERRAT_HANDOVER_ABORT_REQ,

  /* NAS Signaling data indication from RRC to RR */
  QSH_RR_NAS_DATA_IND,

  /* WCDMA to GSM inter-RAT Reselection Request from RRC to RR */
  QSH_RR_INTERRAT_RESELECTION_REQ,

  /* GSM to WCDMA inter-RAT Reselection Reject from RRC to RR */
  QSH_RRC_INTERRAT_RESELECTION_REJ,

  /* WCDMA to GSM inter-RAT CC Order Request from RRC to RR */
  QSH_RR_INTERRAT_CC_ORDER_REQ,

  /*WCDMA to GSM Handover confirmation status*/
  QSH_RRC_INTERRAT_HANDOVER_CNF,

  /*WCDMA to GSM Handover Abort confirmation status*/
  QSH_RRC_INTERRAT_HANDOVER_ABORT_CNF,

  /* GSM to WCDMA inter-RAT CC Order Reject from RR to RRC */
  QSH_RRC_INTERRAT_CC_ORDER_REJ,

  /* Background 3G PLMN Search Cnf from RRC to RR */
  QSH_RRC_INTERRAT_PLMN_SRCH_CNF,
  
  /* Background 3G PLMN Search Abort Cnf from RRC to RR */
  QSH_RRC_INTERRAT_PLMN_SRCH_ABORT_CNF,

  /* WCDMA to GSM service redirection request from RRC to RR */
  QSH_RR_INTERRAT_REDIRECT_REQ,

  /* WCDMA to GSM service redirection abort request from RRC to RR */
  QSH_RR_INTERRAT_REDIRECT_ABORT_REQ,

  /* This command is sent by RRC to RR to abort an on-going WTOG reselection */
  QSH_RR_INTERRAT_RESELECTION_ABORT_REQ,

  /* This command is sent by RRC to RR after the abort request has been 
   * successfully processed. 
   * Note: The GTOW reselection (Idle) abort cannot fail.
   */
  QSH_RRC_INTERRAT_RESELECTION_ABORT_CNF,

  /* WCDMA to GSM inter-RAT CC Order Abort Request from RRC to RR */
  QSH_RR_INTERRAT_CC_ORDER_ABORT_REQ,

  /* GSM to WCDMA inter-RAT CC Order Cnf from RRC to RR*/
  QSH_RRC_INTERRAT_CC_ORDER_CNF,

  /* Background 2G PLMN Search request from RRC to RR */
  QSH_RR_INTERRAT_PLMN_SRCH_REQ,

  /* Background 2G PLMN Search Abort Request from RRC to RR */
  QSH_RR_INTERRAT_PLMN_SRCH_ABORT_REQ,
    
  /* Background 2G PLMN Search Suspend Indication from RRC to RR */
  QSH_RR_INTERRAT_PLMN_SRCH_SUSPEND_IND,

  /* RRC->RR indication that RRC has received a service request. */
  QSH_RR_GAN_ACT_IND,

  /* RRC->RR autonomous indication of RRC state. */
  QSH_RR_GAN_MODE_IND,

  /* RRC->RR response to QSH_RRC_RESELECT_TO_GAN_REQ indicating successful
  completion of reselection to GAN. */
  QSH_RRC_RESELECT_TO_GAN_CNF,

  /* RRC->RR response to QSH_RRC_RESELECT_TO_GAN_REQ indicating temporary
  failure of reselection to GAN, which should be retried by RR at some
  point in the future. */
  QSH_RRC_RESELECT_TO_GAN_REJ,

  /* RRC->RR response to QSH_RRC_GAN_CELL_INFO_REQ. */
  QSH_RRC_GAN_CELL_INFO_CNF,
  /*RRC->RR response to QSH_RRC_RESLECT_FROM_GAN_REQ, indicating failure
    of reselection from GAN request  
  */
  QSH_RRC_RESELECT_FROM_GAN_REJ,
  /*RRC->RR Response to QSH_RRC_GAN_ACT_REQ, indicating failure of
    QSH_RRC_GAN_ACT_REQ
  */
  QSH_RRC_GAN_ACT_REJ,
  QSH_RRC_INTERRAT_REDIRECT_REJ,

  QSH_RRC_INTERRAT_REDIRECT_ABORT_CNF,
  /*RRC->RR Response to QSH_RRC_RR_STOP_WCDMA_REQ
  */
  QSH_RRC_RR_STOP_WCDMA_CNF,
  /*RRC->RR Request from RRC to Stop GSM
  */
  QSH_RR_RRC_STOP_GSM_REQ,

  /* RRC->RR Request for dedicated priorities. */
  QSH_RR_DEDICATED_PRIORITIES_REQ,

  /* RRC->RR Response to request for dedicated priorities. */
  QSH_RRC_DEDICATED_PRIORITIES_RSP,

  /* RRC->RR Indicate that RR should clear it's dedicated priorities. */
  QSH_RR_CLEAR_DEDICATED_PRIORITIES_IND,

  /* RRC->RR Indicate that new keys are activated at W*/
  QSH_RRC_CIPHER_SYNC_IND

}qsh_rrc_rr_cmd_e_type;


typedef struct {
  qsh_metric_hdr_s hdr;
  qsh_rrc_rr_cmd_e_type msg_id;
  qsh_gen_u32_config_type config; 
}wrrc_qsh_metric_rrc_to_rr_cmd_s;

typedef struct
{
  /* step
    L1_FREQ_RAW_SCAN - 0                    - 2 MHz step for raw scan 
    L1_FREQ_FINE_SCAN - 1                   - 200 KHz step for fine scan 
    L1_FREQ_ADDITIONAL_CHANNEL_SCAN - 2     - PCS, 850, 800 MHz additional channel scan
    L1_FREQ_LIST_SCAN - 3                   - When UE is in deep sleep RRC will peform list agc scan
                                              instead of ACQ DB scan
  */ 
  uint8  step;
  uint8  band;
  /* srch
    L1_SERVICE_SRCH_TYPE - 0
    L1_MPLMN_LIST_SRCH_TYPE - 1
    L1_ABPLMN_SRCH_TYPE - 2
    L1_BPLMN_SRCH_TYPE - 3 
  */
  uint8  srch;/* Indicates the search requested; service search, manual plmn or background plmn */
} qsh_l1_freq_scan_cmd_type;

typedef struct
{
    uint16 freq;
  /* acq_type
    L1_WCDMA_FREQ_ONLY - 0     - Use only frequency for acquisition
    L1_WCDMA_FREQ_SCR - 1      - Use frequency and scrambling code
    L1_WCDMA_FREQ_SCR_POS - 2  - Use frequency, scrambling code, pn position
  */
  uint8  acq_type;
  /*
    L1_WCDMA_MICRO             - Micro Acquisition Mode 0
    L1_WCDMA_FULL              - Full Acquisition Mode 1
    L1_WCDMA_REACQ             - Re-acquisition Mode 2
    L1_WCDMA_INTER_F_HO_ACQ    - Inter freq HO ACQ Mode 3
    L1_WCDMA_INTER_S_HO_ACQ    - Intersystem HO ACQ Mode 4
    L1_WCDMA_INTER_S_RESEL_ACQ - Intersystem Reselection ACQ Mode 5
    L1_WCDMA_INTER_S_RESUME    - Intersystem Resume Mode 6
    L1_WCDMA_ACQ_DET_LIST      - Return list of srambling codes detected 7
    L1_WCDMA_INTER_F_REDIRECTION_ACQ 8
    L1_WCDMA_ACQ_MODE_MAX 9
  */
  uint8 acq_mode;
}qsh_l1_acq_cmd_type;

typedef struct
{

  /* a bit mask that indicates which of the sub-primitives are included
     in the CPHY_SETUP_REQ
  */
    uint32 req_mask;
}qsh_l1_setup_cmd_type;

typedef struct
{
  /* Time for which cell is barred */
  uint16 t_barred;

  /* This is used to inform L1 if the GSM cell is barred, or WCDMA 
    cell is barred, or WCDMA frequency is barred, or the active set is barred 
    L1_WCDMA_NOT_BARRED - 0
    L1_GSM_CELL_BARRED - 1
    L1_WCDMA_PSC_BARRED - 2
    L1_WCDMA_FREQ_BARRED - 3
    L1_WCDMA_ASET_BARRED - 4
    L1_EUTRA_CELL_BARRED - 5
    L1_EUTRA_FREQ_BARRED - 6
    L1_WCDMA_FREQ_UNBARRED - 7
  */
  uint8 l1_cell_bar_info;
}qsh_l1_cell_bar_cmd_type;

typedef union
{
  /* Commands from RRC */
  qsh_l1_freq_scan_cmd_type           freq_scan;/*QSH_CPHY_FREQ_SCAN_REQ*/
  qsh_l1_acq_cmd_type                 acq;/*QSH_CPHY_ACQ_REQ*/
  qsh_l1_setup_cmd_type               setup;/*QSH_CPHY_SETUP_REQ*/
  qsh_l1_setup_cmd_type               cell_trans;/*QSH_CPHY_CELL_TRANSITION_REQ*/
  qsh_l1_cell_bar_cmd_type            cell_bar;/*QSH_CPHY_CELL_BAR_REQ*/
  qsh_gen_u32_config_type             gen_config;
} rrc_l1_u_type;

typedef enum
{
  /* Commands from RRC */
  QSH_CPHY_CAPABILITY_REQ           =   0x0,
  QSH_CPHY_IDLE_REQ                 =   0x1,
  QSH_CPHY_DEACTIVATE_REQ           =   0x2,
  QSH_CPHY_STOP_WCDMA_MODE_REQ      =   0x3,
  QSH_CPHY_START_WCDMA_MODE_REQ     =   0x4,
  QSH_CPHY_SUSPEND_WCDMA_MODE_REQ   =   0x5,
  QSH_CPHY_RESUME_WCDMA_MODE_REQ    =   0x6,
  QSH_CPHY_FREQ_SCAN_REQ            =   0x7,
  QSH_CPHY_ACQ_REQ                  =   0x8,
  QSH_CPHY_CELL_PARM_REQ            =   0x9,
  QSH_CPHY_ASET_UPDATE_REQ          =   0xA,
  QSH_CPHY_HARD_HANDOFF_REQ         =   0xB,
  QSH_CPHY_BCH_SCHEDULE_REQ         =   0xC,
  QSH_CPHY_SETUP_REQ                =   0xD,
  QSH_CPHY_DRX_REQ                  =   0xE,
  QSH_CPHY_CIPHERING_KEY_REQ        =   0xF,
  QSH_CPHY_MEASUREMENT_REQ          =   0x10,
  QSH_CPHY_CELL_SELECTION_REQ       =   0x11,
  QSH_CPHY_IMMEDIATE_MEAS_REQ       =   0x12,
  QSH_CPHY_INTEGRITY_KEY_REQ        =   0x13,
  QSH_CPHY_INTEGRITY_KEY_MAUC_REQ   =   0x14,
  QSH_CPHY_IMMEDIATE_RESELECTION_REQ=   0x15,
  QSH_CPHY_CELL_RESELECTION_RSP     =   0x16,
  QSH_CPHY_CELL_TRANSITION_REQ      =   0x17,
  QSH_CPHY_ACT_TIME_CANCEL_REQ      =   0x18,

  /* Commands from MAC */
  QSH_CPHY_UL_TX_CONTROL_REQ        =   0x19,
  QSH_CPHY_MAC_UL_CONFIG_CNF        =   0x1A,
  QSH_CPHY_MAC_DL_CONFIG_CNF        =   0x1B,
  QSH_CPHY_MAC_DL_CONFIG_AT_L1_FAIL_CNF =  0x1C,

  /* Commands from LSM (Location Services Manager) */
  QSH_CPHY_LSM_TIMETAG_REQ          =   0x1D,
  QSH_CPHY_LSM_CELL_MEAS_REQ        =   0x1E,

  /* Commands from BMC (Broadcast Multicast Control) */
  QSH_CPHY_BMC_SCHEDULING_REQ       =   0x1F,
  QSH_CPHY_BMC_SKIP_REQ             =   0x20,
  QSH_CPHY_BMC_DEACTIVATE_REQ       =   0x21,
  QSH_CPHY_CELL_BAR_REQ             =   0x22,
  QSH_CPHY_UPD_DL_SYNC_PARMS        =   0x23,

  /* Commands for SIBS */
  QSH_CPHY_SIB_SCHED_CHANGE_IND     =   0x24,

  QSH_CPHY_GO_TO_SLEEP_IND          =   0x25,

  /* MBMS related command and indication */
  /* ----------------------------------- */

  /* MCCH DRX request */
  QSH_CPHY_MBMS_MCCH_DRX_REQ         =  0x26,
  /* MBMS IDLE request */
  QSH_CPHY_MBMS_IDLE_REQ             =  0x27,
  /* MBMS Preferred layer (FLC) request */
  QSH_CPHY_MBMS_PL_PARM_REQ          =  0x28,

  QSH_CPHY_UPD_OTA_PARMS_IND         = 0x29,
  /* Commands from QChat */
  QSH_CPHY_QCHAT_START_IMM_ORIG      =  0x2A,

  /*Nudging RRC means telling RRC to bring up PCCPCH_S, so we can reestablish
   *timing of the network
   */
  QSH_CPHY_NUDGE_FOR_SFN_MISS_MATCH_RSP = 0x2B,

  /*RRC request L1 to provide info for modem stat*/
  QSH_CPHY_GET_MODEM_STATISTICS_REQ   =   0x2C,

  QSH_CPHY_RSSI_REQ                   =   0x2D,

  QSH_CPHY_DATA_FOR_UI_DISP_REQ       =   0x2E,

  QSH_CPHY_CHECK_SLEEP_RSP            =   0x2F,

  QSH_CPHY_CHECK_S_CRIT_REQ           =   0x30,

  QSH_CPHY_NMR_INFO_REQ               =   0x31,
  
  QSH_CPHY_ADDITIONAL_MEAS_DATA_REQ   =   0x32,

  QSH_CPHY_GET_IMM_MEAS_REQ           =   0x33,

#ifdef FEATURE_DUAL_SIM
 /*Dual SIM Commands*/
  QSH_CPHY_DS_STATUS_CHANGE_IND       =   0x34,
  
  QSH_CPHY_RECOVER_WCDMA_REQ          =   0x35,

#ifdef FEATURE_QTA
  QSH_CPHY_START_QTA_REQ              =   0x36,
  
  QSH_CPHY_STOP_QTA_REQ               =   0x37,
#endif

  QSH_CPHY_DRX_TIMER_EXPIRY_IND       =   0x38,
#endif
/*cmd is added so as to start BPLMN in the immediate 
  sleep cycleit will take WL1 to PCH and then immediately 
  to PCH sleep so that WL1 will call Can l1 function for 
  RRC to start BPMLN
*/
  QSH_CPHY_BPLMN_SLEEP_ABORT_IND      =   0x39,
  /* Number of External Commands. Should be the last element.*/
  QSH_CPHY_NUM_EXT_REQ_CMDS           =   0x3A
}qsh_rrc_l1_cmd_e_type;

typedef struct {
  qsh_metric_hdr_s hdr;
  qsh_rrc_l1_cmd_e_type msg_id;
  rrc_l1_u_type config; 
}wrrc_qsh_metric_rrc_to_l1_cmd_s;


typedef struct
{
  boolean              status;/* Indicates if the acquisition succeeds. */
  /*  QSH_WL1_FAILED_NORMAL -0
  QSH_WL1_FAILED_NO_WRM_LOCK - 1
  QSH_WL1_FAILED_BAND_MISMATCH -2
  QSH_WL1_FAILED_INVALID_MAX - 3*/
  uint8 fail_type;/* Indicates the type of acq failure. */
  uint16               freq;
}qsh_l1_acq_cnf_type;

typedef struct
{
  /* success - 1 , fail - 0 */
  boolean status;
  
  /*Indicate the status type validation failures or 
    setup failures or setup success
    QSH_L1_SETUP_STATUS_VALIDATION_FAIL -  0 
    QSH_L1_SETUP_STATUS_SETUP_FAIL - 1
    #ifdef FEATURE_DUAL_SIM - 2
    QSH_L1_SETUP_STATUS_WRM_LOCK_FAIL - 3
    #endif
    QSH_L1_SETUP_STATUS_SUCCESS  - 4 
  */
  uint8 status_type;

  /* Indicates hs channel status*/
  boolean hs_chan_status;

  /*Combined status of the Sec EDL and EUL operations; 
    Takes a value of FALSE if Sec E_DL or E_UL operation fails; Else, TRUE*/
  boolean eul_chan_status;
} qsh_l1_setup_cnf_type;


typedef struct
{
  /* 
    Error Indication 
    QSH_L1_INVALID_CMD_IND        -Invalid command in current state
    QSH_L1_PARM_ERR_IND           -Invalid parameters in command 
    QSH_L1_UNABLE_PROC_CMD_IND    -Unable to process command
    QSH_L1_FREQ_ERR_IND           -Invalid frequency selection
    QSH_L1_HARDWARE_FAILURE_IND   -Hardware failure in L1
    QSH_L1_FIRMWARE_FAILURE_IND   -Firmware failure in L1
    QSH_L1_LAST_ERR_IND           -Last error indication
  */
  uint8 err;

  qsh_rrc_l1_cmd_e_type   cmd_id;
    /* command id for which failure is being sent */
} qsh_l1_error_ind_type;


typedef struct
{
  /*Log freq if resel ind is for wcdma cell */
  uint16 freq;
  /*
    QSH_L1_RESEL_WCDMA - 0
    QSH_L1_RESEL_GSM - 1
    QSH_L1_RESEL_IS2000 - 2
    QSH_L1_RESEL_LTE - 3 
  */
  uint8       cell;
  /* success - 0, failure - 1*/
  boolean     status;  
}qsh_l1_cell_reselec_ind_type;

typedef struct
{
  uint8  meas_id;/* Measurement id: described in 10.3.7.48 range 1...16 */

  /* Report criteria
    L1_PERIODIC_RPT - 0
    L1_EVENT_TRIGGER_RPT - 1
    L1_NO_RPT - 2
    L1_NO_CHANGE - 3
    L1_INVALID_RPT_MODE - 4
    L1_RPT_MODE_NOT_PRESENT - 5
  */
  uint8 rpt_crit;
  /* measurement type
    L1_MEAS_TYPE_INVALID - 0
    L1_INTRA_FREQ - 1
    L1_INTER_FREQ - 2
    L1_INTER_SYS - 3
    L1_LCS - 4
    L1_TRAF_VOL - 5 
    L1_QUALITY - 6 
    L1_UE_INTERNAL - 7
    L1_INTER_SYS_EUTRA - 8
    L1_MEAS_TYPE_MAX - 9
  */
  uint8  meas_type;
}qsh_l1_measurement_ind_type;


typedef struct
{
   uint16 meas_id;/* Measurement Identity Number */
   boolean status;/* Status (Valid?), true or false */
}  qsh_l1_meas_ctrl_cnf_type;



typedef union{
  qsh_l1_acq_cnf_type                 acq_cnf;/*RRC_CPHY_ACQ_CNF*/
  qsh_l1_setup_cnf_type               setup_cnf;/*RRC_CPHY_SETUP_CNF*/
  qsh_l1_error_ind_type               err_ind;/*RRC_CPHY_ERROR_IND*/
  qsh_l1_cell_reselec_ind_type        cell_rsel_ind;/*RRC_CPHY_CELL_RESELECTION_IND*/
  qsh_l1_cell_reselec_ind_type        cell_transition_cnf;/*RRC_CPHY_CELL_TRANSITION_CNF*/
  qsh_l1_measurement_ind_type         meas_ind;/*RRC_CPHY_MEASUREMENT_IND*/
  qsh_l1_meas_ctrl_cnf_type           meas_ctrl_cnf;/*RRC_CPHY_MEAS_CTRL_CNF*/
  qsh_gen_u32_config_type             gen_config;
}l1_rrc_u_type;

typedef enum
{
  QSH_RRC_L1_CMD_BASE = 0x0000,  /* L1 commands start here */

  /* 0x0001 */
  QSH_RRC_CPHY_ACQ_CNF,

  /* 0x0002 */
  QSH_RRC_CPHY_SETUP_CNF,

  /* 0x0003 */
  QSH_RRC_CPHY_ERROR_IND,

  /* 0x0004 */
  QSH_RRC_CPHY_IDLE_CNF,

  /* 0x0005 */
  QSH_RRC_CPHY_CELL_SELECTION_CNF,

  /* 0x0006 */
  QSH_RRC_CPHY_CELL_RESELECTION_IND,

  /* 0x0007 */
  QSH_RRC_CPHY_INTER_FREQ_BCH_CNF,

  /* 0x0008 */
  QSH_RRC_CPHY_MEASUREMENT_IND,

  /* 0x0009 */
  QSH_RRC_CPHY_CAPABILITY_CNF,

  /* 0x000A */
  QSH_RRC_CPHY_INTEGRITY_KEY_MAUC_CNF,

  /* 0x000B */
  QSH_RRC_CPHY_PHYCHAN_ESTABLISHED_IND,

  /* 0x000C */
  QSH_RRC_CPHY_IN_SYNC_IND,

  /* 0x000D */
  QSH_RRC_CPHY_OUT_OF_SYNC_IND,

  /* 0x000E */
  QSH_RRC_CPHY_RL_FAILURE_IND,

  /* 0x000F */
  QSH_RRC_CPHY_IMMEDIATE_MEAS_CNF,

  /* 0x0010 */
  QSH_RRC_CPHY_IMMEDIATE_RESELECTION_CNF,

  /* 0x0011 */
  QSH_RRC_CPHY_CELL_TRANSITION_CNF,

  /* 0x0012 */
  QSH_RRC_CPHY_FREQ_SCAN_CNF,

  /* 0x0013 */
  QSH_RRC_CPHY_DRX_CNF,

  /* 0x0014 */
  QSH_RRC_CPHY_OUT_OF_SERVICE_AREA_IND,

  /* 0x0015 */
  QSH_RRC_CPHY_DEACTIVATE_CNF,

  /* 0x0016 */
  QSH_RRC_CPHY_MEAS_CTRL_CNF,

  /* 0x0017 */
  QSH_RRC_CPHY_ACT_CANCEL_CNF,

  /* 0x0018 */
  QSH_RRC_CPHY_START_WCDMA_MODE_CNF,

  /* 0x0019 */
  QSH_RRC_CPHY_STOP_WCDMA_MODE_CNF,

  /* 0x001A */
  QSH_RRC_CPHY_SUSPEND_WCDMA_MODE_CNF,

  /* 0x001B */
  QSH_RRC_CPHY_RESUME_WCDMA_MODE_CNF,

  /* 0x001C */
  QSH_RRC_CPHY_COMPRESSED_MODE_IND,

  /* 0x001D */
  QSH_RRC_CPHY_OPERATIONAL_ERROR_IND,

  /* 0x001E */
  QSH_RRC_CPHY_DL_WEAK_IND,

  /* 0x00027 indicate RRC to initiate sending the measurement results 
  *on RACH in uplink by transitioning to eFACH state*/
  QSH_RRC_CPHY_EPCH_TO_EFACH_TRANSITION_IND,

  /*Nudging RRC means telling RRC to bring up PCCPCH_S, so we can reestablish
   *timing of the network
   */
  QSH_RRC_CPHY_NUDGE_FOR_SFN_MISS_MATCH_REQ,

  /* modem stat response from L1 to RRC */
  QSH_RRC_CPHY_MODEM_STATISTICS_RSP,

  /* MDSP halt indication */
  QSH_RRC_CPHY_MDSP_HALT_IND,
  /* WL1 recovery CNF*/
  QSH_RRC_CPHY_RECOVER_WCDMA_CNF,
  QSH_RRC_CPHY_ACTIVATION_TIMER_EXPIRY_IND,
  /* WL1 has started QTA*/
  QSH_RRC_CPHY_START_QTA_CNF,
  /* WL1 has come out of QTA*/
  QSH_RRC_CPHY_STOP_QTA_CNF
}qsh_l1_rrc_cmd_e_type;

typedef struct {
  qsh_metric_hdr_s hdr;
  qsh_l1_rrc_cmd_e_type msg_id;
  l1_rrc_u_type config; 
}wrrc_qsh_metric_l1_to_rrc_cmd_s;

typedef enum
{
  QSH_LTE_RRC_WCDMA_RESEL_REQ = 0x0280,
  QSH_LTE_RRC_WCDMA_REDIR_REQ = 0x0281,
  QSH_LTE_RRC_WCDMA_ABORT_REDIR_REQ = 0x0283,
  QSH_LTE_RRC_EUTRA_CAPABILITIES_REQ = 0x0288,
  QSH_LTE_RRC_WCDMA_PLMN_SRCH_REQ = 0x0289,
  QSH_LTE_RRC_WCDMA_ABORT_PLMN_SRCH_REQ = 0x028a,
  QSH_LTE_RRC_GET_DEDICATED_PRI_REQ = 0x028d,
  QSH_LTE_RRC_WCDMA_PSHO_REQ = 0x0292,
  QSH_LTE_RRC_WCDMA_ABORT_PSHO_REQ = 0x0293,
  
  QSH_WCDMA_RRC_LTE_RESEL_FAILED_RSP = 0x0301,
  QSH_WCDMA_RRC_LTE_ABORT_RESEL_RSP = 0x0302,
  QSH_WCDMA_RRC_LTE_REDIR_FAILED_RSP = 0x0303,
  QSH_WCDMA_RRC_LTE_ABORT_REDIR_RSP = 0x0304,
  QSH_WCDMA_RRC_LTE_PLMN_SRCH_RSP = 0x0305,
  QSH_WCDMA_RRC_LTE_ABORT_PLMN_SRCH_RSP = 0x0306,
  QSH_WCDMA_RRC_LTE_UTRA_CAPABILITIES_RSP = 0x0307,
  QSH_WCDMA_RRC_LTE_GET_DEDICATED_PRI_RSP = 0x0308,
  QSH_WCDMA_RRC_LTE_PSHO_RSP= 0x0309,
  QSH_WCDMA_RRC_LTE_ABORT_PSHO_RSP = 0x030a,
  QSH_WCDMA_RRC_LTE_GET_CGI_RSP = 0x030b,
  QSH_WCDMA_RRC_LTE_ABORT_CGI_RSP = 0x030c, 
  QSH_WCDMA_RRC_LTE_RESEL_RSP = 0x030d,

  QSH_LTE_RRC_WCDMA_RESEL_FAILED_RSP = 0x0381,
  QSH_LTE_RRC_WCDMA_REDIR_FAILED_RSP = 0x0382,
  QSH_LTE_RRC_WCDMA_ABORT_RESEL_RSP = 0x0383,
  QSH_LTE_RRC_WCDMA_ABORT_REDIR_RSP = 0x0384,
  QSH_LTE_RRC_EUTRA_CAPABILITIES_RSP = 0x0389,
  QSH_LTE_RRC_WCDMA_PLMN_SRCH_RSP = 0x038a,
  QSH_LTE_RRC_WCDMA_ABORT_PLMN_SRCH_RSP = 0x038b,
  QSH_LTE_RRC_GET_DEDICATED_PRI_RSP = 0x038e,
  QSH_LTE_RRC_WCDMA_PSHO_RSP = 0x0394,
  QSH_LTE_RRC_WCDMA_ABORT_PSHO_RSP = 0x0395,
  QSH_LTE_RRC_WCDMA_RESEL_RSP = 0x039e,

  QSH_WCDMA_RRC_LTE_CLEAR_DEDICATED_PRI_IND = 0x0401,
  QSH_WCDMA_RRC_LTE_PLMN_SRCH_SUSPEND_IND = 0x0402, 
  
  QSH_LTE_RRC_WCDMA_SUSPEND_PLMN_SRCH_IND = 0x0487 
}qsh_lte_rrc_cmd_e_type;

typedef struct {
  qsh_metric_hdr_s hdr;
  qsh_lte_rrc_cmd_e_type msg_id;
  /* U32 field is not added as we dont see importance in logging this field for RRC to LTE commands */
}wrrc_qsh_metric_rrc_to_lte_cmd_s;

typedef enum
{
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_UNKNOWN             = 0,
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_CELL_BARRED         = 1,
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_PLMN_MISMATCH       = 2, /* same as cell not suitable */
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_FORBIDDEN_TA        = 3,
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_ACQ_FAILED          = 4,
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_DL_WEAK             = 5,
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_CFG_FAILURE         = 6,
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_CELL_SELECT_FAILURE = 7, /* same as S_criteria fail */
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_SIB_READ_ERROR      = 8, /* DEPRICATED, DO NOT USE */
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_FREQ_BARRED         = 9,
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_FORBIDDEN_CSG_ID    = 10,
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_SIB_TIMEOUT_ERROR   = 11,
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_SIB_INVALID_ERROR   = 12,
  QSH_LTE_RRC_IRAT_TO_LTE_RESEL_FAILURE_CAUSE_DEACTIVATE_BY_NAS   = 13
}qsh_lte_rrc_irat_to_lte_failure_cause_type_e;


typedef struct{
  uint16 time_to_reselect;
  qsh_lte_rrc_irat_to_lte_failure_cause_type_e cause;
}qsh_lte_rrc_wcdma_resel_failed_rsp_s;

typedef struct{
   /*!< Indicates if the BPLMN search is done or not */
  boolean srch_done;

  /*!Set to TRUE for Partial Plmn results*/
  boolean partial_srch;

  /*!< Indicates if the highest priority PLMN was found or not */
  boolean hplmn_found;

  /*!< TRUE if the ASF search failed due to lack of FP info with the serving cell,
       FALSE otherwise. If TRUE, srch_done should also be TRUE */
  boolean no_fp_info;
}qsh_lte_irat_plmn_srch_rsp_s;

/*! @brief Handover status */
typedef enum
{
  QSH_LTE_RRC_IRAT_TO_LTE_PSHO_SUCCESS,
  QSH_LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE,
  QSH_LTE_RRC_IRAT_TO_LTE_PSHO_MAX_STATUS
} qsh_lte_rrc_irat_to_lte_psho_status_type_e;

/*! @brief Enumeration for Inter-RAT To-LTE PSHO Failure Causes
*/
typedef enum
{
  QSH_LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_NONE                 = 0, /* Failure cause is not applicable */
  QSH_LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_UNKNOWN              = 1, /* Failure cause is unknown */
  QSH_LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_INVALID_STATE        = 2, /* PSHO received during unexpected state */
  QSH_LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_PROTOCOL_ERROR       = 3, /* PSHO Container ASN1 decode failure */
  QSH_LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_INVALID_CONFIG       = 4, /* invalid configuration */
  QSH_LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_ACQ_FAILURE          = 5, /* acq failures */
  QSH_LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_FREQ_NOT_IMPLEMENTED = 6, /* unsupported freq */
  QSH_LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_RACH_FAILED          = 7  /* RACH failed */
} qsh_lte_rrc_irat_to_lte_psho_failure_cause_type_e;


typedef struct{
  qsh_lte_rrc_irat_to_lte_psho_status_type_e status;
  qsh_lte_rrc_irat_to_lte_psho_failure_cause_type_e fail_cause;
}qsh_lte_rrc_wcdma_psho_rsp_s;

typedef union{
  qsh_lte_rrc_wcdma_resel_failed_rsp_s resel_fail;
  qsh_lte_irat_plmn_srch_rsp_s plmn_srch_rsp;
  qsh_lte_rrc_wcdma_psho_rsp_s psho_rsp;
  qsh_gen_u32_config_type gen_conf;
}qsh_lte_rrc_config_u_type;

typedef struct {
  qsh_metric_hdr_s hdr;
  qsh_lte_rrc_cmd_e_type msg_id;
  qsh_lte_rrc_config_u_type config;
}wrrc_qsh_metric_lte_to_rrc_cmd_s;

typedef enum
{

/*--------------------------------------------------------*/
/* Command Ids of Internal RRC commands                   */
/*--------------------------------------------------------*/

  QSH_RRC_INT_CMD_BASE = 0x0000,  /* Internal commands start here */

  /* 0x001: Command posted by LLC to indicate that RLC downlink SDU has been received */
  QSH_RRC_DOWNLINK_SDU_IND,

  /* 0x002: Command posted by LLC to indicate that RLC downlink L2 ACK has been received*/
  QSH_RRC_DOWNLINK_L2ACK_IND,

  /* 0x003: Command posted from any RRC module to LLC to request a reset of L1. In other
   * words, no physical channels are active, and L1 is waiting for an
   * CPHY_ACQ_REQ.
   */
  QSH_RRC_RESET_L1_REQ,

  /* 0x004: */
  QSH_RRC_RESET_L1_CNF,

  /* 0x005: Command to get specific SIBs from the serving cell or a neighbor cell. */
  QSH_RRC_GET_SPECIFIC_SIBS_REQ,

  /* 0x006: Command posted from SIB module to another RRC module to confirm a
   * QSH_RRC_GET_SPECIFIC_SIBS_REQ command.
   */
  QSH_RRC_GET_SPECIFIC_SIBS_CNF,

  /* 0x007: Command posted by timer when TIMER_TO_CAMP_ON is expired */
  QSH_RRC_TIMER_TO_CAMP_ON_EXPIRED_IND,

  /* 0x008: Command posted by timer when WAIT TIME Timer is expired */
  QSH_RRC_WAIT_TIMER_EXPIRED_IND,

  /* 0x009: Command used to indicate that Phy Chan Failure timer is expired */
  QSH_RRC_PHY_CHAN_FAILURE_TIMER_EXPIRED_IND,

  /* 0x00A: Command posted by timer when RRC Connection Delay Timer is expired */
  QSH_RRC_QSH_RRC_CON_DELAY_TIMER_EXPIRED_IND,

  /* 0x00B: Command posted by timer when Timer T300 is  expired */
  QSH_RRC_T300_EXPIRED_IND,

  /* 0x00C: Command posted by timer when Timer T308 is  expired */
  QSH_RRC_T308_EXPIRED_IND,

  /* 0x00D: Command posted by timer when Timer T304 is  expired */
  QSH_RRC_T304_EXPIRED_IND,

  /* 0x00E: Command posted by timer when Timer T302 is  expired */
  QSH_RRC_T302_EXPIRED_IND,

  /* 0x00F: Command posted by timer when Timer T305 is  expired */
  QSH_RRC_T305_EXPIRED_IND,

  /* 0x010: Command posted by timer when Timer T316 is  expired */
  QSH_RRC_T316_EXPIRED_IND,

  /* 0x011: Command posted by timer when Timer T317 is  expired */
  QSH_RRC_T317_EXPIRED_IND,

  /* 0x012: Command posted by timer when Timer T307 is  expired */
  QSH_RRC_T307_EXPIRED_IND,

  /* 0x013: Command posted by timer when Timer T314 is  expired */
  QSH_RRC_T314_EXPIRED_IND,

  /* 0x014: Command posted by timer when Timer T315 is  expired */
  QSH_RRC_T315_EXPIRED_IND,

  /* 0x015: Command posted by timer when RCE Timer for L2 ACK  is  expired */
  QSH_RRC_TIMER_FOR_RCE_L2ACK_EXPIRED_IND,

  /* 0x016: Command posted by timer when RCR Timer for L2 ACK  is  expired */
  QSH_RRC_TIMER_FOR_RCR_L2ACK_EXPIRED_IND,

  /* 0x017: Command posted by RCE to CSP with either frequency or System redirection */
  QSH_RRC_REDIRECT_REQ,

  /* 0x018: Command posted by CSP to RCE after CSP camped on to redirected frequency or
   * system
   */
  QSH_RRC_REDIRECT_CNF,

  /* 0x019: Command posted to State Change Manager when RRC state changes. */
  QSH_RRC_NEW_STATE_IND,

  /* 0x01A: Command poster by State Change Manager to other RRC procedures
   * to notify them of the state transitions.
   */
  QSH_RRC_STATE_CHANGE_IND,

  /* 0x01B: Command posted to Cell Change manager requesting a Cell Change */
  QSH_RRC_CELL_CHANGE_REQ,

  /* 0x01C: Command posted by Cell Change Manager in response to Cell Change Req.
   * This command is posted to the procedure that requested Cell Change.
   */
  QSH_RRC_CELL_CHANGE_CNF,

  /* 0x01D: Command posted by Cell Change manager to inform procedures of a change
   * in cell.
   */
  QSH_RRC_NEW_CELL_IND,

  /* 0x01E: Command posted from any RRC module to LLC to setup/reconfig/release
   * connected mode channels.
   */
  QSH_RRC_CHANNEL_CONFIG_REQ,

  /* 0x01F: Command posted from LLC to any RRC module to confirm a setup/reconfig/release
   * of connected mode channels.
   */
  QSH_RRC_CHANNEL_CONFIG_CNF,

  /* 0x020: Command used to request any one of a number of L1 or L2 commands as
   * specified in rrc_llc_cmd_type
   */
  QSH_RRC_LLC_CMD_REQ,

  /* 0x021: Command used to confirm any one of a number of L1 or L2 commands as
   * specified in rrc_llc_cmd_type
   */
  QSH_RRC_LLC_CMD_CNF,

  /* 0x022: Command used to confirm that CELL UPDATE procedure is completed. This will
   * be in response to QSH_RRC_CELL_UPDATE_INITAITE_REQ
   */
  QSH_RRC_CELL_UPDATE_COMPLETE_CNF,

  /* 0x023: Command used to initiate CELL UPDATE procedure. It includes the reason to for
   * CELL UPDATE Procedure initiation
   */
  QSH_RRC_CELL_UPDATE_INITIATE_REQ,

  /* 0x024: Command used to inform SIB procedure that SIBs modified. */
  QSH_RRC_UPDATE_SIBS_IND,

  /* 0x025: Command used to initiate UE Capability Information procedure */
  QSH_RRC_INITIATE_UE_CAPABILITY_REQ,

  /* 0x026: Command used by the RCE procedure to indicate CSP to exculde previous frequency
   * from the cell reselection for the specified Wait Time
   */
  QSH_RRC_DISABLE_PREV_FREQ_IND,

  /* 0x027: Command posted by cell selection procedure to measurement procedure
   * to read SIB11/12 and then inform L1 to start measurements
   */
  QSH_RRC_CELL_SELECTED_MEAS_REQ,

  /* 0x028: Command used to perform the periodically RSSI&BER report to the CM */
  QSH_RRC_TIMER_FOR_RSSI_BER_EXPIRED_IND,

  /* 0x029: Command used to trigger cell selection */
  QSH_RRC_INITIATE_CELL_SELECTION_IND,

  /* 0x02A: Command to request deactivation of lower layers */
  QSH_RRC_LOWER_LAYER_DEACT_REQ,

  /* 0x02B: Confirmation for lower layer deactivation */
  QSH_RRC_LOWER_LAYER_DEACT_CNF,

  /* 0x02C: Command used to inform the SIB procedure that the SIB wait time has
   * expired
   */
  QSH_RRC_SIB_WAIT_TIMER_EXPIRED_IND,

  /* 0x02D: Command posted by SIB procedure to other procedures when
   * a SIB in the serving cell has been modified by the UTRAN
   */
  QSH_RRC_SIB_CHANGE_IND,

  /* 0x02E: Command to RRC LLC update C-RNTI and/or U-RNTI */
  QSH_RRC_RNTI_UPDATE_REQ,

  /* 0x02F: Command to Confirm the updatation of C-RNTI and/or U-RNTI */
  QSH_RRC_RNTI_UPDATE_CNF,

  /* 0x030: Command to stop WCDMA mode in the lower layers */
  QSH_RRC_STOP_LOWER_LAYER_REQ,

  /* 0x031: Command to Confirm the stop of WCDMA mode */
  QSH_RRC_STOP_LOWER_LAYER_CNF,

  /* 0x032: Command to notify a RRC mode transition */
  QSH_RRC_MODE_CHANGE_IND,

  /* 0x033: Command to RRC LLC for cell reselection */
  QSH_RRC_CELL_RESELECTION_REQ,

  /* 0x034: Command to confirm the reselection request */
  QSH_RRC_CELL_RESELECTION_CNF,

  /* 0x035: Command to give the SIB7 update status. Posted by
   * the SIB procedure to other RRC procedures
   */
  QSH_RRC_SIB7_UPDATE_STATUS_IND,

  /* 0x036: Command to indicate that the SIB7 expiration
   * timer has expired.
   */
  QSH_RRC_SIB7_EXP_TIMER_EXPIRED_IND,

  /* 0x037: Command from LLC to indcate end of reconfig procedure,
   * and SMC can continue security configuration
   */
  QSH_RRC_RECONFIG_COMPLETE_IND,

  /* 0x038: Command from LLC to indcate configuration of lower layers
   * with ciphering information passed in ciph_config_req
   */
  QSH_RRC_CIPH_CONFIG_REQ,

  /* 0x039: Command from LLC to indcate configuration of lower layers
   * with ciphering information passed in ciph_config_req
   */
  QSH_RRC_CIPH_CONFIG_CNF,

  /* 0x03A: Command to be send from SMC to LLC to abort ongoing
   * ciphering configuraiton at RLC and revert to old set of keys.
   * NOTE: There is no CNF associated with this command.
   */
  QSH_RRC_ABORT_CIPH_CONFIG_REQ,

  /* 0x03B: Command from Timer to indicate that the timer to ignore
   * BCCH Modification info has expired
   */
  QSH_RRC_IGNORE_BCCH_MOD_TIMER_EXPIRED_IND,

  /* 0x03C: Command from Timer to indicate that the timer for cell
   * selection going from DCH->FACH has expired
   */
  QSH_RRC_DCH_FACH_CELL_SELECTION_TIMER_EXPIRED_IND,

  /* 0x03D: Command from Procedure to SCRR to release Signalling Connections */
  QSH_RRC_SIG_CONN_RELEASE_REQ,

  /* 0x03E: Command from SCRR to Procedure confirming that Signalling Connections
   * are released
   */
  QSH_RRC_SIG_CONN_RELEASE_CNF,

  /* 0x03F: */
  QSH_RRC_CHANGE_MODE_REQ,

  /* 0x040: */
  QSH_RRC_CHANGE_MODE_CNF,

  /* 0x041: */
  QSH_RRC_NAS_DATA_TRANSFER_IND,

  /* 0x042: */
  QSH_RRC_REVERT_TO_UTRAN_CONFIG_REQ,

  /* 0x043: */
  QSH_RRC_REVERT_TO_UTRAN_CONFIG_CNF,

  /* 0x044: */
  QSH_RRC_DELETE_MEAS_REQ,

  /* 0x045: */
  QSH_RRC_COMPRESSED_MODE_IND,

  /* 0x046: */
  QSH_RRC_CSP_ACQ_REQ,

  /* 0x047: */
  QSH_RRC_CSP_ACQ_CNF,

  /* 0x048: */
  QSH_RRC_HO_CIPH_REQ,

  /* 0x049: */
  QSH_RRC_HO_CIPH_CNF,

  /* 0x04A: Command from Timer to indicate that the timer for detection
   * of L1 deadlock condition has expired.
   */
  QSH_RRCTMR_L1_DEADLOCK_DETECT_TIMER_EXPIRED_IND,

  /* 0x04B: Command from Timer to indicate that the timer to delay ERR_FATAL
   * for CPHY_SETUP_CNF or CPHY_CELL_TRANSITION_CNF has expired
   */
  QSH_RRCTMR_DELAY_ERR_FATAL_TIMER_EXPIRED_IND,

  /* 0x04C: IND to inform UDT procedure that SMC has gotten L2 ACK and it is safe to
   * transmit Uplink Messages
   */
  QSH_RRC_SMC_DDT_L2_ACK_IND,

  /* 0x04D: Command from TMR to indicated that the timer for cell
   * selection from DCH to CELL_PCH or URA_PCH has expired
   */
  QSH_RRC_DCH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND,

  /* 0x04E: Req/CNF between Reconfig-Procedures and SMC to trigger re-establishment
   * of RLC radio bearers
   */
  QSH_RRC_RE_EST_RLC_FOR_SRNS_REQ,

  /* 0x04F: */
  QSH_RRC_RE_EST_RLC_FOR_SRNS_CNF,

  /* 0x050: Req/CNF between SMC-LLC to re-establish RLC radio bearers
   */
  QSH_RRC_LLC_RE_EST_RLC_REQ,

  /* 0x051: */
  QSH_RRC_LLC_RE_EST_RLC_CNF,

  /* 0x052: */
  QSH_RRC_ERR_FATAL_IND,

  /* 0x053: */
  QSH_RRC_T3174_EXPIRED_IND,

  /* 0x054: */
  QSH_RRC_CSP_CAMP_REQ,

  /* 0x055: */
  QSH_RRC_CSP_CAMP_CNF,

  /* 0x056: */
  QSH_RRC_CONN_SETUP_IND,

  /* PLEASE ADD ALL NEW Internal Commands HERE */

  /* 0x057: Command used to initiate URA UPDATE procedure. It includes the reason to for
   * URA UPDATE Procedure initiation
   */
  QSH_RRC_URA_UPDATE_INITIATE_REQ,

  /* 0x058: Command used to confirm that URA UPDATE procedure is completed. This will
   * be in response to QSH_RRC_URA_UPDATE_INITAITE_REQ
   */
  QSH_RRC_URA_UPDATE_COMPLETE_CNF,

  /* 0x059: Command used to indicate to CSP that full frequency scan can be initiated */
  QSH_RRC_FREQ_SCAN_TIMER_EXPIRED_IND,

  /* 0x05A: Command used to indicate that OOS timer is expired */
  QSH_RRC_OOS_TIMER_EXPIRED_IND,

  /* 0x05B: Command used to indicate that Deep Sleep no service timer has expired */
  QSH_RRC_DEEP_SLEEP_NO_SVC_TIMER_EXPIRED_IND,

  /* 0x05C: Command used to indicate to CSP that WCDMA has been resumed (by CHO) */
  QSH_RRC_CSP_WCDMA_RESUMED_IND,

  /* 0x05D: Command used to indicate that BPLMN 2G->3G search timer has expired */
  QSH_RRC_BPLMN_SRCH_TIMER_EXPIRED_IND,

  /* 0x05E: Command used to indicate that BPLMN 2G->3G full scan timer has expired */
  QSH_RRC_BPLMN_FREQ_SCAN_TIMER_EXPIRED_IND,

  /* 0x05F: Command used to indicate that the wait timer started for the last camped
   * frequency during inter frequency redirection has expired.
   */
  QSH_RRC_FREQ_REDIRECT_WAIT_TIMER_EXPIRED_IND,

  /* 0x060: Command used to abort interrat operation when a higher priority request
   * needs to be processed
   */
  QSH_RRC_IRAT_ABORT_IND,

  /* 0x061: Command used to indicate that the 30s specific PLMN search timer when
   * UE is OOS in Connected Mode has expired
   */
  QSH_RRC_CONN_MODE_OOS_TIMER_EXPIRED_IND,

  /* 0x062: Command used to indicate that the inactivity timer when
   * has expired
   */
   QSH_RRC_INACTIVITY_TIMER_EXPIRED_IND,

  /* 0x063: Command used for indicating the completion of IDT procedure for a particular domain
   * This is used by release procedure
   */
  QSH_RRC_IDT_COMPLETED_FOR_DOMAIN,

  /* 0x064: Command used to indicate that BPLMN 3G->3G search timer has expired */
  QSH_RRC_WTOW_BPLMN_DRX_SRCH_TIMER_EXPIRED_IND,

  /* 0x065: Command used to indicate that MAX BPLMN 3G->3G search timer allowed has expired */
  QSH_RRC_BPLMN_GUARD_SRCH_TIMER_EXPIRED_IND,

  /* 0x066: Command used to indicate that BPLMN 3G->3G,2G search can be started */
  QSH_RRC_BPLMN_SEARCH_RESUME_IND,

  /* 0x067: Command sent by other procedures to CSP to suspend the BPLMN search */
  QSH_RRC_BPLMN_SUSPEND_REQ,

  /* 0x068: Command sent by CSP to the procedure that sent suspend request for suspending BPLMN search */
  QSH_RRC_BPLMN_SUSPEND_CNF,

  /* 0x069: command sent from MCM to Meas and UECI on QSH_RRC_CHANGE_MODE_IND */
  QSH_RRC_MODE_STATUS_IND,

  /* 0x06A: Command used to indicate for updating six hrs sib timer*/
  QSH_RRC_SIX_HRS_SIB_TIMER_UPDATE_IND,

  /* 0x06B: Command used to indicate that Uniform OOS search timer has expired */
  QSH_RRC_UNIFORM_OOS_SEARCH_TIMER_EXPIRED_IND,

  /* 0x06C: Command used to put UE into deep sleep when UE is OOS in Connected Mode
   */
  QSH_RRC_CONN_MODE_DEEP_SLEEP_REQ,

  /* 0x06D: Command used to wakeup UE from deep sleep when UE is OOS in Connected Mode
   */
  QSH_RRC_CONN_MODE_DEEP_SLEEP_WAKEUP_REQ,

  /* 0x06E: Command used to indicate that the initial Tmins no deep sleep
   * timer has expired. The timer is started when UE goes OOS in Connected Mode.
   */
  QSH_RRC_CONN_MODE_OOS_INITIAL_NO_DS_TIMER_EXPIRED_IND,

  /* 0x06F: Command used to indicate that the deep sleep timer has expired and UE
   * should now wake up and search for service again
   */
  QSH_RRC_CONN_MODE_OOS_DS_TIMER_EXPIRED_IND,

  /* 0x070:  */
  QSH_RRC_RB_PROC_COMPL_IND,

  /* 0x071: Command used to indicate SIB change Gaurd timer expired*/
  QSH_RRC_SIB_CHANGE_GUARD_TIMER_EXPIRED_IND,

  /* 0x072: this command is sent by LLC to CSP after receiving START_CNF
   * from L1 in Connected Mode
   */
  QSH_RRC_CONN_MODE_DEEP_SLEEP_WAKEUP_CNF,

  /*0x073: Command used to indicate BCCH Modification timer expiry*/
  QSH_RRC_BCCH_MODIFICATION_TIMER_EXPIRED_IND,

  /* 0x074: Command used to indicate that  Manual PLMN search full scan timer has expired */
  QSH_RRC_MANUAL_PLMN_FREQ_SCAN_TIMER_EXPIRED_IND,

  /* 0x075: Command from timer proc to indicate that the timer for cell
   * selection going from FACH->FACH/PCH has expired
   */
  QSH_RRC_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND,

  /* 0x076: Command posted by RRC when GSTK queries RRC for
   * Network Measurement information
   */
  QSH_RRC_GSTK_UTRAN_NMR_INFO_REQ,

  /* 0x077: Command from timer proc to indicate that the timer for cell
   * selection on GtoW redirection has expired.
   */
  QSH_RRC_GTOW_REDIRECTION_WAIT_TIMER_EXPIRED_IND,

  /* 0x078: Command from timer proc to indicate the guard timer for 
   * abort W2G BPLMN has expired.
   */
  QSH_RRC_WTOG_ABORT_GUARD_TIMER_EXPIRED_IND,

  /* 0x079: Command from CSP proc to indicate MCM can go ahead with 
   * stop wcdma request
   */
  QSH_RRC_CONTINUE_STOP_MODE_REQ,

  /*0x07A: Command for expiry of timer T320. T320 is the wait timer for CU. 
        UTRAN may use this timer to queue the UE in Cell_PCH or URA_PCH state.*/
  QSH_RRC_T320_EXPIRED_IND,

  /*0x07B: Command for requesting "Change of UE Capability" thru COUEC proc*/
  QSH_RRC_COUEC_REQ,
  
  /*0x07C: Command for confirmation of "Change of UE Cap." request*/
  QSH_RRC_COUEC_CNF,

  /*0x07D: Command for expiry of COUEC timer*/
  QSH_RRC_TIMER_COUEC_EXPIRED_IND,

  /*0x07E : Command used to indicate T319 expiry*/
  QSH_RRC_T319_EXPIRED_IND,

  /*0x07F: Command used to indicate that Dedicated priority validity timer expired*/
  QSH_RRC_DED_PRI_VALDITY_TIMER_EXPIRED_IND,

  /*0x080: Command used to indicate that Dedicated priority info has changed*/
  QSH_RRC_DED_PRI_CHANGE_IND,

  QSH_RRC_WRM_PREEMPT_IND,

  QSH_RRC_NO_RESOURCE_AVAILABLE_FROM_L1,

  /*0x08B : Command used to indicate T323 expiry*/
  QSH_RRC_T323_EXPIRED_IND,

  /* Command used to indicate that EOOS full scan timer has expired */
  QSH_RRC_EOOS_FULL_SCAN_TIMER_EXPIRED_IND,

  /* Command used to indicate WRM Unlock_By timer expired */
  QSH_RRC_DUAL_SIM_WRM_UNLOCK_BY_TIMER_EXP_IND,

  QSH_RRC_QCHAT_FA_RRC_TIMER_EXPIRED_IND,

  QSH_RRC_IHO_OPEN_DOMAIN_IND,

  QSH_RRC_LTE_TO_WCDMA_SRCH_TIMER_EXPIRY_IND,

  QSH_RRC_INTERNAL_CSFB_ABORT_REQ,

  /* Command used to initiate RLC_UL_DATA_IND procedure */
  QSH_RRC_RLC_UL_DATA_IND,

  QSH_RRC_GTOW_REDIRECTION_LFS_TIMER_EXPIRED_IND,

  QSH_RRC_CSG_ASF_SRCH_TIMER_EXPIRY_IND,

  QSH_RRC_CU_TRANS_TO_DISCON_TIMER_EXPIRED,

  QSH_RRC_SIB_STOP_PARTITION_TMR_EXPIRED_IND,
 
  /*Timer to report partial PLMN LIST to NAS during Manual PLMN Search*/ 
  QSH_RRC_BPLMN_PARTIAL_REPORT_TIMER_EXPIRED_IND,

  QSH_RRC_TEN_MIN_SIB_TIMER_UPDATE_IND,

  QSH_RRC_NV_REFRESH_IND,

  QSH_RRC_QSH_PROCESS_CMD,

  QSH_RRC_GET_SIBDB_ENTRY_FOR_CELL,

  QSH_RRC_GET_SIBDB_ENTRY_FOR_CELL_CNF,

  QSH_RRC_ADD_ACQ_ENTRY,

  QSH_RRC_OUT_OF_SERVICE_INTERNAL_IND,

  QSH_RRC_TRM_PRIORITY_CHANGE_IND

}qsh_rrc_int_cmd_e_type;

typedef struct {
  qsh_metric_hdr_s hdr;
  qsh_rrc_int_cmd_e_type msg_id;
  qsh_gen_u32_config_type config; 
}wrrc_qsh_metric_internal_cmd_s;

typedef enum
{
  /* Commands from RRC */
  QSH_LLC_CPHY_CAPABILITY_REQ           =   0x1000,
  QSH_LLC_CPHY_IDLE_REQ                 =   0x1001,
  QSH_LLC_CPHY_DEACTIVATE_REQ           =   0x1002,
  QSH_LLC_CPHY_STOP_WCDMA_MODE_REQ      =   0x1003,
  QSH_LLC_CPHY_START_WCDMA_MODE_REQ     =   0x1004,
  QSH_LLC_CPHY_SUSPEND_WCDMA_MODE_REQ   =   0x1005,
  QSH_LLC_CPHY_RESUME_WCDMA_MODE_REQ    =   0x1006,
  QSH_LLC_CPHY_FREQ_SCAN_REQ            =   0x1007,
  QSH_LLC_CPHY_ACQ_REQ                  =   0x1008,
  QSH_LLC_CPHY_CELL_PARM_REQ            =   0x1009,
  QSH_LLC_CPHY_ASET_UPDATE_REQ          =   0x100A,
  QSH_LLC_CPHY_HARD_HANDOFF_REQ         =   0x100B,
  QSH_LLC_CPHY_BCH_SCHEDULE_REQ         =   0x100C,
  QSH_LLC_CPHY_SETUP_REQ                =   0x100D,
  QSH_LLC_CPHY_DRX_REQ                  =   0x100E,
  QSH_LLC_CPHY_CIPHERING_KEY_REQ        =   0x100F,
  QSH_LLC_CPHY_MEASUREMENT_REQ          =   0x1010,
  QSH_LLC_CPHY_CELL_SELECTION_REQ       =   0x1011,
  QSH_LLC_CPHY_IMMEDIATE_MEAS_REQ       =   0x1012,
  QSH_LLC_CPHY_INTEGRITY_KEY_REQ        =   0x1013,
  QSH_LLC_CPHY_INTEGRITY_KEY_MAUC_REQ   =   0x1014,
  QSH_LLC_CPHY_IMMEDIATE_RESELECTION_REQ=   0x1015,
  QSH_LLC_CPHY_CELL_RESELECTION_RSP     =   0x1016,
  QSH_LLC_CPHY_CELL_TRANSITION_REQ      =   0x1017,
  QSH_LLC_CPHY_ACT_TIME_CANCEL_REQ      =   0x1018,

  /* Commands from MAC */
  QSH_LLC_CPHY_UL_TX_CONTROL_REQ        =   0x1019,
  QSH_LLC_CPHY_MAC_UL_CONFIG_CNF        =   0x101A,
  QSH_LLC_CPHY_MAC_DL_CONFIG_CNF        =   0x101B,
  QSH_LLC_CPHY_MAC_DL_CONFIG_AT_L1_FAIL_CNF =  0x101C,

  /* Commands from LSM (Location Services Manager) */
  QSH_LLC_CPHY_LSM_TIMETAG_REQ          =   0x101D,
  QSH_LLC_CPHY_LSM_CELL_MEAS_REQ        =   0x101E,

  /* Commands from BMC (Broadcast Multicast Control) */
  QSH_LLC_CPHY_BMC_SCHEDULING_REQ       =   0x101F,
  QSH_LLC_CPHY_BMC_SKIP_REQ             =   0x1020,
  QSH_LLC_CPHY_BMC_DEACTIVATE_REQ       =   0x1021,
  QSH_LLC_CPHY_CELL_BAR_REQ             =   0x1022,
  QSH_LLC_CPHY_UPD_DL_SYNC_PARMS        =   0x1023,

  /* Commands for SIBS */
  QSH_LLC_CPHY_SIB_SCHED_CHANGE_IND     =   0x1024,

  QSH_LLC_CPHY_GO_TO_SLEEP_IND          =   0x1025,

  /* MBMS related command and indication */
  /* ----------------------------------- */

  /* MCCH DRX request */
  QSH_LLC_CPHY_MBMS_MCCH_DRX_REQ         =  0x1026,
  /* MBMS IDLE request */
  QSH_LLC_CPHY_MBMS_IDLE_REQ             =  0x1027,
  /* MBMS Preferred layer (FLC) request */
  QSH_LLC_CPHY_MBMS_PL_PARM_REQ          =  0x1028,

  QSH_LLC_CPHY_UPD_OTA_PARMS_IND         = 0x1029,
  /* Commands from QChat */
  QSH_LLC_CPHY_QCHAT_START_IMM_ORIG      =  0x102A,

  /*Nudging RRC means telling RRC to bring up PCCPCH_S, so we can reestablish
   *timing of the network
   */
  QSH_LLC_CPHY_NUDGE_FOR_SFN_MISS_MATCH_RSP = 0x102B,

  /*RRC request L1 to provide info for modem stat*/
  QSH_LLC_CPHY_GET_MODEM_STATISTICS_REQ   =   0x102C,

  QSH_LLC_CPHY_RSSI_REQ                   =   0x102D,

  QSH_LLC_CPHY_DATA_FOR_UI_DISP_REQ       =   0x102E,

  QSH_LLC_CPHY_CHECK_SLEEP_RSP            =   0x102F,

  QSH_LLC_CPHY_CHECK_S_CRIT_REQ           =   0x1030,

  QSH_LLC_CPHY_NMR_INFO_REQ               =   0x1031,
  
  QSH_LLC_CPHY_ADDITIONAL_MEAS_DATA_REQ   =   0x1032,

  QSH_LLC_CPHY_GET_IMM_MEAS_REQ           =   0x1033,

#ifdef FEATURE_DUAL_SIM
 /*Dual SIM Commands*/
  QSH_LLC_CPHY_DS_STATUS_CHANGE_IND       =   0x1034,
  
  QSH_LLC_CPHY_RECOVER_WCDMA_REQ          =   0x1035,

#ifdef FEATURE_QTA
  QSH_LLC_CPHY_START_QTA_REQ              =   0x1036,
  
  QSH_LLC_CPHY_STOP_QTA_REQ               =   0x1037,
#endif

  QSH_LLC_CPHY_DRX_TIMER_EXPIRY_IND       =   0x1038,
#endif
/*cmd is added so as to start BPLMN in the immediate 
  sleep cycleit will take WL1 to PCH and then immediately 
  to PCH sleep so that WL1 will call Can l1 function for 
  RRC to start BPMLN
*/
  QSH_LLC_CPHY_BPLMN_SLEEP_ABORT_IND      =   0x1039,
  /* Number of External Commands. Should be the last element.*/
  QSH_LLC_CPHY_NUM_EXT_REQ_CMDS           =   0x103A,

/* L2 UL commands */

  QSH_LLC_CMAC_UL_CONFIG_REQ = 0x2064,         /* Configure UL MAC */

  /* Configure UL MAC traffic volume measurements*/
  QSH_LLC_CMAC_UL_TRAFFIC_VOLUME_CONFIG_REQ,

  /* RRC can request MAC to stop transmission on DTCH using this command*/
  QSH_LLC_CMAC_UL_BLOCK_RACH_TRAFFIC_REQ,

  QSH_LLC_CMAC_UL_TFC_SUBSET_CFG_REQ,

  QSH_LLC_CMAC_HS_UL_PROCESS_HDR_LOG_PKT_REQ,

  QSH_LLC_CMAC_HS_UL_PROCESS_ETFCI_STATUS_LOG_PKT_REQ,

  QSH_LLC_CMAC_HS_UL_PROCESS_MAC_I_IS_LOG_PKT_REQ,

  QSH_LLC_CMAC_HS_UL_PROCESS_MAC_HSRACH_TIMERS_LOG_PKT_REQ,

  QSH_LLC_CMAC_QSH_PROCESS_METRIC_COMMAND_REQ ,
  
  QSH_LLC_CRLC_UL_TM_CONFIG_REQ ,      /* Configure UL TM RLC */

  QSH_LLC_CRLC_UL_UM_CONFIG_REQ,      /* Configure UL UM RLC */

  QSH_LLC_CRLC_AM_CONFIG_REQ,         /* Configure AM RLC */

  QSH_LLC_CRLC_AM_RESET_REQ,          /* RESET AM RLC */

  QSH_LLC_CRLC_UL_CIPHER_CONFIG_REQ ,  /* Configure Ciphering on DL RLC*/

  QSH_LLC_CRLC_UL_RESUME_REQ,         /* Resume transmission on RLC channels*/

  QSH_LLC_CRLC_UL_HFN_REQ ,            /* Get the HFN on DL RLC*/

  QSH_LLC_CRLC_RELEASE_ALL_REQ,       /* Release all non-MBMS RLC entities.  */

  QSH_LLC_CRLC_RELEASE_ALL_WITH_MBMS_REQ, /* Release all RLC entities.  */

  QSH_LLC_CRLC_UL_REG_FOR_DATA_IND_REQ, /* Register incoming data indication. */

  QSH_LLC_CRLC_UL_UNREG_FOR_DATA_IND_REQ, /* Register incoming data indication. */

  QSH_LLC_RLC_REGISTER_AM_SDU_CONFIRM_CALLBACK_REQ, /* Request for SDU confirm    */
                                            /* callback function for RRC  */

  QSH_LLC_RLC_OPEN_RX_WIN,            /* Open the Receiving Window. */

  QSH_LLC_RLC_CLOSE_RX_WIN,           /* Close the Receiving Window.  */

  QSH_LLC_RLCI_UL_RESET_REQ,          /* RESET request for UL AM RLC  */

  QSH_LLC_RLCI_DL_CONFIG_CNF,         /* Confirm the completion of DL config  */

  QSH_LLC_RLCI_DL_RESET_DONE,         /* Confirm the completion of the DL RESET */

  QSH_LLC_RLC_UL_PURGE_WM_REQ ,            /* Command to purge the uplink WM */

  QSH_LLC_CRLC_UL_ABORT_CIPHER_CONFIG_REQ, /* Command to abort the new UL ciphering
                                      configuration                                */

  QSH_LLC_CRLC_UL_CONTINUE_REQ,    /* Continue transmission on Stopped RLC channels*/
  
  QSH_LLC_RLCI_UL_TX_AM_STATUS,             /* command to indicate UL to send status PDU */

  QSH_LLC_RLCI_UL_DUP_RESET_REQ,      /* command to indicate duplicate RESET reception on DL*/

  QSH_RLC_HANDLE_TM_TIMER_BASED_SDU_DISCARD_CB_IND,

  QSH_LLC_CRLC_QSH_PROCESS_METRIC_COMMAND_REQ,

  QSH_WCDMA_L2_UL_MAX_CMDS,              /* Number of Ul L2 Command types. must be last entry */


  /*L2 DL Commands */


  QSH_LLC_RLCI_DL_TX_AM_STATUS = 0x3064,     /* Copy nak_list_q (DL) to tx_nak_q (UL) */

  QSH_LLC_CMAC_DL_CONFIG_REQ,       /* Configure DL MAC */

  QSH_LLC_CRLC_DL_TM_CONFIG_REQ,    /* Configure DL TM RLC */

  QSH_LLC_CRLC_DL_UM_CONFIG_REQ,    /* Configure DL UM RLC */

  QSH_LLC_RLCI_DL_CONFIG_REQ,       /* Configure DL AM RLC */

  QSH_LLC_RLCI_DL_RESET_REQ,        /* RESET request for DL AM RLC  */

  QSH_LLC_RLCI_UL_RESET_DONE,       /* Confirm the RESET completion of UL AM RLC  */

  QSH_LLC_RLCI_DL_RELEASE_ALL_REQ,  /* Release All non-MBMS request to the Downlink */

  QSH_LLC_CRLC_DL_CONTINUE_REQ,      /*Continue the stopped RBs in UM mode*/

  QSH_LLC_CRLC_DL_CIPHER_CONFIG_REQ,/* Configure Cophering on DL RLC*/

  QSH_LLC_CRLC_DL_HFN_REQ,          /* Configure Cophering on DL RLC*/

  QSH_LLC_RLC_REGISTER_AM_SDU_DISCARD_CALLBACK_REQ, /* Request for registration of SDU  */
                                            /* discard callback function for    */
                                            /* RRC                              */
  QSH_LLC_RLC_PROCESS_MAC_HS_RESET_REQ,

  QSH_LLC_CRLC_DL_ABORT_CIPHER_CONFIG_REQ,          /* Request to abort the new DL
                                               ciphering configuration         */

  QSH_LLC_RLC_HANDLE_SDU_DISCARD_TIMEOUT_EVENT, /* Command to handle SDU discard timer */

  QSH_LLC_RLC_PDCP_CLEANUP_WM,

  QSH_WCDMA_L2_DL_MAX_CMDS            /* Number of DL L2 Command types. must be last entry */

}qsh_rrc_llc_cmd_e_type;


typedef struct {
  qsh_metric_hdr_s hdr;
  qsh_rrc_llc_cmd_e_type msg_id;
  qsh_gen_u32_config_type config; 
}wrrc_qsh_metric_llc_cmd_s;

typedef struct {
  qsh_metric_hdr_s hdr;
  uint8 rrc_state;
}wrrc_qsh_metric_rrc_state_history;

typedef enum
{
  WRRC_CONN_REL_CAUSE_NORMAL_EVENT, 
  WRRC_CONN_REL_CAUSE_UNSPECIFIED,
  WRRC_CONN_REL_CAUSE_PRE_EMPTIVE_RELEASE,
  WRRC_CONN_REL_CAUSE_CONGESTION,
  WRRC_CONN_REL_CAUSE_REESTABLISHMENT_REJECT,
  WRRC_CONN_REL_CAUSE_DRCTD_SIGNALING_CONN_REEST,
  WRRC_CONN_REL_CAUSE_USER_INACTIVITY,
  WRRC_CONN_REL_CAUSE_OTHER
}wrrc_conn_rel_cause_e_type;

typedef union
{
  wrrc_conn_rel_cause_e_type      conn_rel_cause;
  sys_rlf_weak_signal_e_type      rlf_cause;
  sys_handover_failure_e_type     ho_fail_type;
}wrrc_call_end_cause_u_type;

typedef struct{
  qsh_metric_hdr_s       hdr;
  sys_call_end_event_e_type   type;
  wrrc_call_end_cause_u_type   cause;
}wrrc_qsh_metric_call_end_info_s_type;

typedef struct{
  qsh_metric_hdr_s       hdr;
  sys_rab_status_e_type mrab_status;
}wrrc_qsh_metric_multi_rab_status_s_type;

typedef struct{
  qsh_metric_hdr_s   hdr;
  uint8              cu_cause;
}wrrc_qsh_metric_cell_update_cause_s_type;

typedef enum
{
  WRRC_QSH_EVENT_ACQ = 0,                   /* (FREQ) Acquisition start */
  WRRC_QSH_EVENT_FREQ_SCAN = 1,             /* (FREQ) Frequency scan start */
  WRRC_QSH_EVENT_ACTIVATON_WNDW_START = 2,  /* (FREQ) Activation timer is expired (for a non-zero activation timer value)*/
  WRRC_QSH_EVENT_WTOG_HO_REQ = 3,           /* (RARE) W to G Handover is started */
  WRRC_QSH_EVENT_CCO_START = 4,             /* (RARE) Cell change order started */
  WRRC_QSH_EVENT_GTOW_HO_REQ = 5,           /* (RARE) G to W handover is started */
  WRRC_QSH_EVENT_LTOW_HO_REQ = 6,           /* (RARE) L to W handover is started */
  WRRC_QSH_EVENT_WAIT_FOR_L2_ACK = 7,       /* (FREQ) UE is waiting for L2 ACK for PCH reconfiguration/SRNS relocation/SMC */
  WRRC_QSH_EVENT_SIB_READ_START = 8,        /* (FREQ) SIB read started */
  WRRC_QSH_EVENT_NON_IRAT_RESEL_START = 9,  /* (FREQ) Inter freq or intra freq reselection started */
  WRRC_QSH_EVENT_IRAT_RESEL_START = 10,     /* (FREQ) WtoG or WtoL resel started */
  WRRC_QSH_EVENT_DCH_CELL_PCH_TRANS = 11,   /* (RARE) DCH to PCH state transition triggered */
  WRRC_QSH_EVENT_DCH_FACH_TRANS = 12,       /* (FREQ) DCH to FACH state transition triggered */
  WRRC_QSH_EVENT_SUSPEND_START = 13,        /* (FREQ) WCDMA is suspended */
  WRRC_QSH_EVENT_RESUME_START = 14,         /* (FREQ) WCDMA is resumed */
  WRRC_QSH_EVENT_CELL_RESEL_FAILURE = 15,   /* (RARE) Cell reselection failure */
  WRRC_QSH_EVENT_CONN_REL_UNSPEC_CAUSE = 16,/* (RARE) Connection release unspecified cause */
  WRRC_QSH_EVENT_WTOL_HO_REQ = 17,          /* (RARE) W to L handover is started */
  WRRC_QSH_EVENT_W2X_IRAT_HO_FAILURE = 18,  /* (RARE) W to G/L/1x handover failure event*/
  WRRC_QSH_EVENT_OTA_MESSAGE_FAILURE = 19,  /* (RARE) Failure OTA message event */
  WRRC_QSH_EVENT_SIB_DECODE_FAILURE = 20,   /* (RARE) SIB read failure */
  WRRC_QSH_EVENT_INTER_FREQ_HO_FAILURE = 21,  /* (RARE) Inter frequency handover failure event*/
  WRRC_QSH_EVENT_LOCK_WAIT_TIMEOUT = 22,    /* (FREQ) WRM lock wait timer expiry*/
  WRRC_QSH_EVENT_RACH_FAILURE = 23,   
  WRRC_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE = 24, /* Reported for all failures */
  WRRC_QSH_EVENT_RRC_STATE_CHANGE = 31,    /*(FREQ)Reports RRC state change*/
  WRRC_QSH_EVENT_MAX                        /* (NA)   For Internal bound checking only */
}wrrc_qsh_event_e;

#endif /*WRRC_QSH_EXT_H*/
