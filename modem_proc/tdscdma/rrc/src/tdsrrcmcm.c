/*===========================================================================
         R R C   M O D E C O N T R O L   M O D U L E 

DESCRIPTION

  This module performs the actions related to the RRC mode. It 
  maintanences the RRC mode related information. It handles the TDS mode.
  activation and deactivation. It also informs the other procedures when 
  the mode change occurs. This module is used for dual-mode only.
  
  
EXTERNALIZED FUNCTIONS

  tdsrrcmcm_event_handler            Event handler for this module
  tdsrrcmcm_is_process_required      Filtering an incomming command
  rrcmcm_register_mode_change     Register mode change notification
  rrcmcm_get_mode_and_preference  Retrieve the current mode and the mode pref

INITIALIZATION AND SEQUENCING REQUIREMENTS

   
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcmcm.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   hx      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"
#include "err.h"
#include "msg.h"
#include "memheap.h"
#include "tdsrrctmr.h"
#include "tdsrrcsend.h"
#include "tdsrrccmd_v.h"
#include "tdsl1rrcif.h"
#include "tdsrrccmd_l2.h"
#include "tdsrrccmd.h"
#include "tdsrrcdata.h"
#include "tdsrrcintif.h"
#include "tdsrrclcm.h"
#include "tdsrrcllc.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcccm.h"
#include "tdsrrccspi.h"
#include "tdsrrccsp.h"
#include "tdsrrcsmc.h"
#include "tdsrrciho.h"
#include "tdsrrcmeas.h"
#include "tdsl1mtask.h"
#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "sys.h"
/*#include "tdsrrcrrif.h"*/
#include "tdsrrc_rr_if.h"
#include "rr_tdsrrc_if.h"

#include "tdsrrcmcm.h"
#include "tdsrrcrce.h"
#include "tdsrrciho.h"
#include "tdsrrccho.h"
/*#include "rr_rrc_if.h"*/
#include "tdsrrc_rr_if.h"
#include "rr_tdsrrc_if.h"
#include "tdsrrcmisc.h" 

#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsCFAParser_i.h"
#include "tdsrrclogging.h"
#include "tdsrrcnv.h"

#ifdef FEATURE_WCDMA_TO_TDSCDMA
#include "tdsrrc_rrc_if.h"
#endif
#include "tdsl1trm.h"
#include "tdsCFAParser.h"
#include "tdsrrcllcoc.h"

#include "tdsrrcqsh.h"
#include "tdsrrcsibdb.h"

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
rrc_plmn_list_req_type *tdsrrc_svd_plmn_req_ptr = NULL;

/* RAT Mode                                                                */
tdsrrc_mode_e_type tdsrrc_mode = TDSRRC_MODE_INACTIVE;

/*set activation  cause of TDS */
tdsrrcmcm_activation_cause tdsrrcmcm_act_cause = TDSRRCMCM_ACT_CAUSE_NONE;

/*set last activation cause of TDS */
tdsrrcmcm_activation_cause tdsrrcmcm_last_act_cause = TDSRRCMCM_ACT_CAUSE_NONE;

/* Mode Preference                                                         */
boolean tdsrrc_dual_mode_enabled = TRUE;
#ifdef FEATURE_CMCC_SECURE_CALL
/* is secure call enabled*/
boolean tdsrrc_is_secure_call_enabled = FALSE;
#endif
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
boolean tdsrrc_lte_mode_enabled = FALSE;
#endif
extern tdsrrc_ds_tuneaway_status_change_ind_type tds_ds_tuneaway_status_change_ind;

extern boolean tdsrrc_t2t_redir_due_to_rls;
extern boolean tdsrrc_t2t_redir_due_to_rej;

extern boolean tdsrrcmcm_sent_to_llc_tds_lower_req;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
/* MCM state */
tdsrrcmcm_state_e_type tdsrrcmcm_state = TDSRRCMCM_INITIAL;

/* MCM Mode Notification Table */
tdsrrcmcm_mode_change_table_type tdsrrcmcm_notifications[TDSRRCMCM_MAX_NOTIFICATIONS];

/* Mode change notification counter */
uint16 tdsrrcmcm_notification_counter = 0;

/* Struct to hold the commands which have deferred processing */
tdsrrcmcm_cmd_store_type tdsrrcmcm_cmd_store;

/* Destination procedure for change mode confirmation */
tdsrrc_proc_e_type tdsprocedure_id;

/* The reason for stopping TDS mode of operation */
rrc_stop_cause_e_type tdsstop_cause;

/*Save EST_REQ type.. This is needed because in STANDBY mode we will save
  the EST_REQ type and only later on going to ACTIVE mode send it to rce.
  If we go to INACTIVE mode contents are deleted*/
rrc_est_req_type tdsrrcmcm_est_req;

/*Flag to indicate if EST_REQ is saved or not*/
boolean tdsrrcmcm_est_req_saved_flag;

/* Flag to indicate if we need to send stop_cnf to MM (stop issued by MM) */
boolean tdsrrcmcm_send_stop_cnf_to_mm_flag;

/* Global HSDPA HSUPA Indicator */
extern sys_hs_ind_e_type tdshsdpa_hsupa_ind;

/* TRUE means timer out.
FALSE means timer is running*/
boolean tdsrrc_reestablish_timer_out = FALSE;

/* 160ms the quality below the Qout */
#define OUT_OF_SYNC_TIME_THRESHOLD 160
/* 16000ms is the original design. currently only in DCH state consider the T313+reestablish timer.
   And some NW will release the UE if no signal at uplink for about 16s*/
#define LTA_REESTABLISH_TIME 16000
#define LTA_MAX_TIME (0xFFFFFFFF)
#define LTA_DURATION_TO_INVALID_SIB_DB (300000)

  tdsl1_common_fail_enum_type tdsrrcmcm_wl1_failure_cause = TDSL1_FAILED_NORMAL;

  /* Boolean to indicate TRM exchange is a success upon receiving the request from LTE
   . Mainly used to determine if the lock needs to be exchanged with intermediate client 
     for redirection/HO failure cases. If TRUE the lock will be exchanged with the IRAT client */
  boolean tdsrrc_tdrm_exchange_success = FALSE;
  /* Use global to record the reason of synced priority for easily debug */
  trm_reason_enum_t tdsrrc_synced_reason;


#ifdef FEATURE_3GPP_CSFB_TDS
extern boolean tdsrrc_csfb_call_status;
#endif
extern sys_proc_type_e_type tdsrrc_proc_type;

typedef enum
{
  TDSINIT_ALL,
  TDSINIT_STATE_VARS_ONLY,
  TDSINIT_CMD_STORE_ONLY
} tdsrrcmcm_init_e_type;

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================

FUNCTION   RRCMCM_SEND_CHANGE_MODE_REQ

DESCRIPTION

  This function sends a TDSRRC_CHANGE_MODE_REQ to MCM

          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_change_mode_req( tdsrrc_mode_e_type mode, tdsrrc_proc_e_type proc_id )
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_CHANGE_MODE_REQ",0,0,0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANGE_MODE_REQ;

  /* Fill in procedure id    */
  cmd_ptr->cmd.change_mode_req.procedure = proc_id;

  /* Fill in the requested mode */
  cmd_ptr->cmd.change_mode_req.mode = mode;
  
  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_CHANGE_MODE_REQ sent to MCM");

  return;
} /* end of tdsrrciho_send_change_mode_req */

/*===========================================================================

FUNCTION  RRCMCM_SET_ESTABLISHMENT_CAUSE

DESCRIPTION

  This function sets the global RRC establishment cause appropriately 
  based on the last INTER-RAT GTOW procedure. No change otherwise.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_set_establishment_cause(void)
{
  /* Update est cause with inter-RAT reselection or inter-RAT CCO
   * only when the est cause is Registration.
   
   * Note: this is done to handle the case where UE reselects from
   * GtoW and before registration could be performed on W, an 
   * emergency call is attempted. If the check for registration 
   * is not done, then UE will incorrectly update the est cause
   * to Inter-RAT Resel/CCO and the emergency call cause will be lost
   */
  if(tdsrrc_establishment_cause == RRC_EST_REGISTRATION)
  {
    if(tdsrrcmcm_last_act_cause == TDSRRCMCM_INTERRAT_CELL_RESELECTION)
    {
      tdsrrc_establishment_cause = RRC_EST_INTER_RAT_CELL_RESELECT;
    }
    else if(tdsrrccho_is_g2td_pcco_in_progress())
    {
      tdsrrc_establishment_cause = RRC_EST_INTER_RAT_CELL_CHANGE_ORDER;
    }
    else
    {
    // Add more cases here
    }
  }



} /* tdsrrcmcm_set_establishment_cause */


/*===========================================================================

FUNCTION  RRCMCM_INIT_DATA

DESCRIPTION

  This function clears all the module variables and puts the initial
  values wherever needed.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_init_data( tdsrrcmcm_init_e_type init_type )
{                   
  if( (init_type == TDSINIT_ALL) || (init_type == TDSINIT_STATE_VARS_ONLY) )
  {
    /* Reset variables */
    tdsrrcmcm_state = TDSRRCMCM_INITIAL;
    tdsstop_cause = RRC_RAT_CHANGE;
    tdsrrcmcm_est_req_saved_flag = FALSE;
    tdsrrcmcm_send_stop_cnf_to_mm_flag = FALSE;
  }
  
  if( (init_type == TDSINIT_ALL) || (init_type == TDSINIT_CMD_STORE_ONLY) )
  {
    /* init cmd_store */
    memset(&tdsrrcmcm_cmd_store,0,sizeof(tdsrrcmcm_cmd_store));
    tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_RR_ABORT_NONE;
    tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_NONE;
    tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = TDSRRC_MODE_MAX;
  }
  
  return;
}

/*===========================================================================

FUNCTION  tdsrrcmcm_save_est_req

DESCRIPTION

  This function saves est_req if received in STANDBY state
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_save_est_req
(
tdsrrc_cmd_type *cmd_ptr
)
{
  /*copy the contents of est_req from the command locally and set saved flag
    to true*/
  memscpy(&tdsrrcmcm_est_req, sizeof(rrc_est_req_type),&cmd_ptr->cmd.rrc_est_req, sizeof(rrc_est_req_type));
  tdsrrcmcm_est_req_saved_flag = TRUE;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saved  TDSRRC_EST_REQ");
}
/*===========================================================================

FUNCTION  tdsrrcmcm_process_est_req

DESCRIPTION

  This function processes est req if received in STANDBY state..
  If EST_REQ is saved in STANDBY state then it is sent when
  mode changes to ACTIVE.
  If EST_REQ is saved in STANDBY state then it is deleted when
  mode changes to INACTIVE
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_process_est_req
(
tdsrrc_mode_e_type rrc_est_mode
)
{
  tdsrrc_cmd_type est_req_cmd;


  if(rrc_est_mode == TDSRRC_MODE_ACTIVE)
  {
    /*set cmd id to EST_REQ*/
    est_req_cmd.cmd_hdr.cmd_id = TDSRRC_EST_REQ;

    /*copy contents of est req command earlier saved*/
    memscpy(&est_req_cmd.cmd.rrc_est_req, sizeof(rrc_est_req_type),&tdsrrcmcm_est_req, sizeof(rrc_est_req_type));
    
    /*call rrcrce procedure as it would have been called if not for STANDBY state*/
    tdsrrcrce_procedure_event_handler( &est_req_cmd );
  }

  if((rrc_est_mode == TDSRRC_MODE_ACTIVE) || (rrc_est_mode == TDSRRC_MODE_INACTIVE))
  {
    /*whether rrc mode was ACTIVE or INACTIVE this flag needs to be reset*/
    tdsrrcmcm_est_req_saved_flag = FALSE;
  }
}

#ifdef FEATURE_WRLF_SYSTEM_SEL
/*===========================================================================

FUNCTION  tdsrrcmcm_send_gtow_bplmn_search_cnf_failed

DESCRIPTION

  This function sends GTOW BPLMN search CNF to RR as failure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_gtow_bplmn_search_cnf_failed
(
  void
)
{
  tdsrrc_interrat_plmn_srch_cnf_type interrat_plmn_srch_cnf;

  memset(&interrat_plmn_srch_cnf,0,sizeof(tdsrrc_interrat_plmn_srch_cnf_type));

  interrat_plmn_srch_cnf.available_plmn_list.length = 0;
  interrat_plmn_srch_cnf.srch_done = TRUE;
  interrat_plmn_srch_cnf.hplmn_found = FALSE;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN: Sending Srch Cnf to RR");
  tdsrr_send_rrc_message_to_rr_internal((void *)(&interrat_plmn_srch_cnf), TDSRRC_INTERRAT_PLMN_SRCH_CNF,
                                    sizeof(tdsrrc_interrat_plmn_srch_cnf_type));

}
/*===========================================================================

FUNCTION  tdsrrcmcm_send_gtow_reselection_reject

DESCRIPTION

  This function sends GTOW reselection reject to RR.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_gtow_reselection_reject
(
  void
)
{
  tdsrrc_interrat_reselect_rej_type rrc_interrat_reselect_rej;

  memset(&rrc_interrat_reselect_rej,0,sizeof(tdsrrc_interrat_reselect_rej_type));

  rrc_interrat_reselect_rej.reject_reason    = TDSRRC_UMTS_INAVALID_STATE;
   /* Time ( in Sec ) when the same cell should be considered */
  rrc_interrat_reselect_rej.time_to_reselect = 1280;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"wcdma_time_to_reselect is set to %d secs",rrc_interrat_reselect_rej.time_to_reselect);

  TDSRRC_DEBUG_INCREMENT_STATS(G2TD_CEL_RESEL_FAILURE);
  TDSRRC_DEBUG_INCREMENT_STATS(G2TD_CEL_RESEL_FAILURE_FAILED_OTHER_CAUSE);
  /* Send the RRC_INTERRAT_RESELECT_REJ message to RR */
  tdsrr_send_rrc_message_to_rr_internal((void *)(&(rrc_interrat_reselect_rej)), TDSRRC_INTERRAT_RESELECTION_REJ,
          sizeof(tdsrrc_interrat_reselect_rej_type));

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_INTERRAT_RESELECT_REJ sent to GSM RR");

}

#ifdef FEATURE_LTE_TO_TDSCDMA
/*===========================================================================

FUNCTION  rrcmcm_send_ltow_bplmn_search_cnf_failed

DESCRIPTION

  This function sends LTOW BPLMN search CNF to LTE-RRC as failure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_l2td_bplmn_search_cnf_failed
(
  void
)
{
  lte_irat_plmn_srch_rsp_s plmn_srch_rsp;

  memset(&plmn_srch_rsp,0,sizeof(lte_irat_plmn_srch_rsp_s));
 
  plmn_srch_rsp.srch_done   = TRUE;
  plmn_srch_rsp.hplmn_found = tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.hplmn_found;
  plmn_srch_rsp.available_plmn_list.length = 0;
  plmn_srch_rsp.detected_frequency_list.num_freqs = 0;
  msgr_init_hdr(&plmn_srch_rsp.msg_hdr,MSGR_TDSCDMA_RRC,TDSCDMA_RRC_LTE_PLMN_SRCH_RSP);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  msgr_set_hdr_variant(&plmn_srch_rsp.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
  msgr_set_hdr_inst(&plmn_srch_rsp.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
  msgr_send(&plmn_srch_rsp.msg_hdr, sizeof(lte_irat_plmn_srch_rsp_s));

  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_send_message_to_lte_rrc(&plmn_srch_rsp.msg_hdr, sizeof(lte_irat_plmn_srch_rsp_s));
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD:Sent TDSCDMA_RRC_LTE_PLMN_SRCH_RSP to LTE RRC");
}
/*===========================================================================

FUNCTION  rrccsp_send_ltow_reselection_reject

DESCRIPTION

  This function sends LTOW reselection CNF to LTE-RRC as failure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccsp_send_ltow_reselection_reject
(
  void
)
{
  tds_rrc_lte_resel_failed_rsp_type resel_failed_rsp;

  memset(&resel_failed_rsp,0,sizeof(tds_rrc_lte_resel_failed_rsp_type));
  
  resel_failed_rsp.failure_cause = TDSCDMA_RRC_NONE; /* ZZTBE: should change to valid reason */
  resel_failed_rsp.time_to_reselect = 0;
  msgr_init_hdr(&resel_failed_rsp.msg_hdr,MSGR_TDSCDMA_RRC,TDSCDMA_RRC_LTE_RESEL_FAILED_RSP);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  msgr_set_hdr_variant(&resel_failed_rsp.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
  msgr_set_hdr_inst(&resel_failed_rsp.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
  msgr_send(&resel_failed_rsp.msg_hdr, sizeof(tds_rrc_lte_resel_failed_rsp_type));

  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_send_message_to_lte_rrc(&resel_failed_rsp.msg_hdr, sizeof(tds_rrc_lte_resel_failed_rsp_type));
  }
  TDSRRC_DEBUG_INCREMENT_STATS(L2TD_CEL_RESEL_FAILURE);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: TDSCDMA_RRC_LTE_RESEL_FAILED_RSP sent to LTE RRC,cause %d",resel_failed_rsp.failure_cause);
}
#endif
#endif  /* FEATURE_WRLF_SYSTEM_SEL */
/*===========================================================================

FUNCTION  RRCMCM_IS_PROCESS_REQUIRED

DESCRIPTION

  This function returns True if the incoming command is required a further
  process based on the RRC mode. Otherwise, the False is returned.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_is_process_required( tdsrrc_cmd_type *cmd_ptr )
{                   
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type sub_id,curr_sub_id;
#endif
  boolean return_status = FALSE;

  tdsrrc_rr_cmd_type *rr_cmd = (tdsrrc_rr_cmd_type *)cmd_ptr;

#ifdef FEATURE_SGLTE
  tdsl1_meas_mode_enum_type l1_meas_mode;
#endif
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_BARRING_INFO_IND)
  {
    return TRUE;
  }

#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_SUBSCRIPTION_ID_IND)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_SUBSCRIPTION_ID_IND received from MM and handle it");
    return TRUE;
  }
#endif

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_LAI_REJECT_LIST_IND 
    )
  {
    return TRUE;
  }
#endif

  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_PSEUDO_LTE_LIST_UPDATE_IND)
  {
    return TRUE;
  }
#ifdef FEATURE_TDSCDMA_FATAL_RECOVERY 
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_CPHY_HW_ERROR_IND)
  {
    return TRUE;
  }
#endif

  /*Support TDSRRC_CPHY_ERROR_IND regardless RRC mode*/
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_CPHY_ERROR_IND)
  {
    return TRUE;
  }
  
#ifdef FEATURE_QSH_EVENT_METRIC
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_QSH_CB_IND)
  {
    return TRUE;
  }
#endif
  if ( TDSRRC_PROCEDURE_IND == cmd_ptr->cmd_hdr.cmd_id)
  {
    return TRUE;
  }

  /*stop abort resel to grr timer*/
  if(TDSRR_INTERRAT_RESELECTION_ABORT_CNF == cmd_ptr->cmd_hdr.cmd_id )
  {
     if(tdsrrctmr_get_remaining_time(TDSRRCTMR_ABORT_RESEL_REQ_TO_GRR_TIMER) > 0)
     {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Stop TDSRRCTMR_ABORT_RESEL_REQ_TO_GRR_TIMER.");
        tdsrrctmr_stop_timer(TDSRRCTMR_ABORT_RESEL_REQ_TO_GRR_TIMER);
     }
  }

  if(TDSRRCTMR_ABORT_RESEL_REQ_TO_GRR_TIMER_EXPIRED_IND == cmd_ptr->cmd_hdr.cmd_id )  
  {
    ERR_FATAL("GRR didn't reply abort_resel_req in 3 seconds",0,0,0);
  }

#ifdef FEATURE_MODEM_CONFIG_REFRESH
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MCFG_REFRESH_IND)
  {
    return TRUE;
  }
#endif
#ifdef FEATURE_CMCC_SECURE_CALL  
  if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_SECURE_CALL_IND)
  {
    return TRUE;
  }
#endif
  if ((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DS_STATUS_CHANGE_IND)
/* TDSRRC_MODE_PREF_CHANGE_IND is retired after FEATURE_DUAL_WCDMA is introduced */
#ifndef FEATURE_DUAL_WCDMA
      || (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MODE_PREF_CHANGE_IND)
#endif
     )
  {
#ifdef FEATURE_TRIPLE_SIM
    return TRUE;
#else
#ifdef FEATURE_TDSCDMA_DSDA
    return TRUE;
#else
    if(tdsrrc_mode == TDSRRC_MODE_INACTIVE)
    {
      return TRUE;
    }
    return FALSE;
#endif
#endif
  }
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DS_TUNEAWAY_STATUS_CHANGE_IND)
  {
    return TRUE;
  }
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MULTIMODE_SUBS_CHGD_IND)
  {
    return TRUE;
  }

#ifdef FEATURE_3GPP_CSFB_TDS
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_CSFB_CALL_STATUS_IND)
  {
    /*Process the CSFB status Ind from NAS in all modes and states*/
    return TRUE;
  }
#endif

  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_UE_MODE_IND)
  {
#ifdef FEATURE_SGLTE
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    sub_id = cmd_ptr->cmd.rrc_ue_mode_ind.as_id;
    if((sub_id < SYS_MODEM_AS_ID_1) || (sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
    {
      TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC: invalid sub_id %d", sub_id);
      return FALSE;
    }
    curr_sub_id = tdsrrc_get_as_id();      
#endif   
    l1_meas_mode = ( (cmd_ptr->cmd.rrc_ue_mode_ind.ue_mode == SYS_UE_MODE_SGLTE_HOME) ||
                     (cmd_ptr->cmd.rrc_ue_mode_ind.ue_mode == SYS_UE_MODE_SGLTE_TEST_ONLY) ) ?
                   TDSL1_MEAS_MODE_SGLTE : TDSL1_MEAS_MODE_NORMAL;
	
	  if( ((l1_meas_mode==TDSL1_MEAS_MODE_SGLTE)&&(tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_SGLTE)) ||
		  ((l1_meas_mode==TDSL1_MEAS_MODE_NORMAL)&&(tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_NORMAL))
      )
    {
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
        tdsrrc_set_identity_info_to_related_sub_params(curr_sub_id);
      tdsrrc_set_identity_info_to_global_params(sub_id);
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"rcvd TDSRRC_UE_MODE_IND sub_id %d with mode %d, curr UE mode %d,no handle",sub_id,cmd_ptr->cmd.rrc_ue_mode_ind.ue_mode,tdsrrc_csp_int_data.ue_mode);
  	  /* update global to keep in-sync with NAS */
  	  tdsrrc_csp_int_data.ue_mode = cmd_ptr->cmd.rrc_ue_mode_ind.ue_mode;
      tdsrrc_set_identity_info_to_related_sub_params(sub_id);
      tdsrrc_set_identity_info_to_global_params(curr_sub_id);
#else
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"rcvd TDSRRC_UE_MODE_IND with mode %d, curr UE mode %d,no handle",cmd_ptr->cmd.rrc_ue_mode_ind.ue_mode,tdsrrc_csp_int_data.ue_mode);
      /* update glable to keep in-sync with NAS */
      tdsrrc_csp_int_data.ue_mode = cmd_ptr->cmd.rrc_ue_mode_ind.ue_mode;
#endif
  	  return FALSE;
    }
    if( ((tdsrrc_mode != TDSRRC_MODE_INACTIVE) || (tdsrrcmcm_state == TDSRRCMCM_WAIT_START_CNF)) &&
		!((tdsrrc_mode == TDSRRC_MODE_STANDBY) && (tdsrrccho_is_td2g_cco_active())) /* we may have replied stop cnf to NAS during CCO */
	  )
    {
      ERR_FATAL("rcvd TDSRRC_UE_MODE_IND when tds is not stopped! tdsrrc_mode: %d, tdsrrcmcm_state: %d, cco_active: %d",
	  	        tdsrrc_mode,tdsrrcmcm_state,tdsrrccho_is_td2g_cco_active());
    }
    /*Process the SGLTE ue mode ind from NAS when L1 is not started or suspended for t2g cco, so the new ue mode can be sent to L1 by start_tds_mode_req/resume_req*/
    return TRUE;
#else
    return FALSE;
#endif
  }

  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MSGR_CMD)
  {
    switch(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id)
    {
      case TDSCDMA_RRC_GPS_POS_CELL_INFO_REQ:
      case TDSCDMA_RRC_GET_CONFIG_REQ:
      case TDSCDMA_RRC_SET_CONFIG_REQ:
      case TDSCDMA_RRC_QMI_DRX_LEVEL_REQ:
        /*Process the MSGR cmd in all modes and states*/
        return TRUE;

      default:
        break;
    }
  }

#ifdef FEATURE_WRLF_SYSTEM_SEL
  if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
  {
    if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_INTERRAT_RESELECTION_REQ)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF:UE in connected mode OOS. Cannot handle G2TD resel req.");
      tdsrrcmcm_send_gtow_reselection_reject();
      return FALSE;
    }
    else if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_INTERRAT_PLMN_SRCH_REQ)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF:UE in connected mode OOS. Cannot handle G2TD PLMN list req.");
      tdsrrcmcm_send_gtow_bplmn_search_cnf_failed();
      return FALSE;
    }
    else if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_PLMN_LIST_REQ)
    {
      if(tdsrrccsp_is_suitable_srch_ended())
      {
        /*Agreement with NAS: In COOS, MPLMN Search request will only be sent after NAS has indicated RRC_ABORT_REQ
         *Thus save MPLMN request for now untill, RRC Connection is aborted.
         */
        tdsrrc_svd_plmn_req_ptr = (rrc_plmn_list_req_type *)tdsrrc_malloc(sizeof(rrc_plmn_list_req_type));
        memscpy(tdsrrc_svd_plmn_req_ptr, sizeof(rrc_plmn_list_req_type), 
            &cmd_ptr->cmd.plmn_list_req, sizeof(rrc_plmn_list_req_type));
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF:MPLMN in COOS: Hard abort in progress. Save PLMN list req.");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF:UE in connected mode OOS. Cannot handle PLMN list req.");
        tdsrrccsp_send_mm_plmn_list_cnf_as_aborted(cmd_ptr->cmd.plmn_list_req.trans_id);
      }
      return FALSE;
    }
	else if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_BPLMN_SEARCH_ABORT_REQ)&&
            (tdsrrc_svd_plmn_req_ptr != NULL)) 
    {
      tdsrrccsp_send_mm_plmn_list_cnf_as_aborted(tdsrrc_svd_plmn_req_ptr->trans_id);
      tdsrrc_free(tdsrrc_svd_plmn_req_ptr);
      tdsrrc_svd_plmn_req_ptr = NULL;
      return FALSE;
    }
#ifdef FEATURE_LTE_TO_TDSCDMA
    else if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MSGR_CMD)
    {
      msgr_umid_type msgr_id = cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id;

      if(msgr_id == TDSCDMA_RRC_LTE_RESEL_REQ)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF:UE in connected mode OOS. Cannot handle L2TD resel req.");
        tdsrrccsp_send_ltow_reselection_reject();
        return FALSE;
      }
      else if(msgr_id == TDSCDMA_RRC_LTE_PLMN_SRCH_REQ)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF:UE in connected mode OOS. Cannot handle L2TD PLMN list req.");
        tdsrrcmcm_send_l2td_bplmn_search_cnf_failed();
        return FALSE;
      }
    }
#endif
  }
#endif

  /*After we have received a TDSRRC_MODE_CHANGE_REQ we should not receive any
    other Inter_Layer command.. Print a message if any commands are received,
    this will help debug if race conditions are seen
   */
  if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_MODE_CHANGE)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cmd %x rcvd after Mode_Change Rcvd",cmd_ptr->cmd_hdr.cmd_id);
  }

#ifdef FEATURE_TDSCDMA_TO_LTE
  if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MSGR_CMD) &&
    (cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == LTE_RRC_EUTRA_CAPABILITIES_UTRA_TDD_RSP))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received EUTRA cap rsp in rrc mode %d.",tdsrrc_mode);
    return TRUE;
  }
#endif
#ifdef FEATURE_LTE_TO_TDSCDMA
#ifdef FEATURE_TDSCDMA_SUPPORT_LTE_RLF_REPORT_ENHANCEMENT
if((TDSRRC_MSGR_CMD == cmd_ptr->cmd_hdr.cmd_id)
    &&(LTE_RRC_UTRA_SERVING_CELL_INFO_REQ == cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id))
{
    if(TDSRRC_MODE_ACTIVE == tdsrrc_mode)
    {
        return TRUE;    
    }
    else
    {
        /*in non-active state, input FALSE to send failed response directly*/
        tdsrrccsp_send_lte_utra_cell_info_rsp(FALSE);
        return FALSE;
    }
}
#endif
#endif

  /* Check the command identifier based the RRC mode */
  switch (tdsrrc_mode)
  {
    case TDSRRC_MODE_INACTIVE:
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cmd received in Inactive: %x",cmd_ptr->cmd_hdr.cmd_id);

      /* If RRC is in inactive mode */
      switch (cmd_ptr->cmd_hdr.cmd_id)
      {
        /* Only the following commands are processed */
        case TDSRRC_SERVICE_REQ:
        case RRC_CPHY_START_TDS_MODE_CNF:
        case TDSRRC_ABORT_REQ:
        case TDSRRC_DEACT_REQ:
        case TDSRRC_MODE_CHANGE_REQ:
        case TDSRRC_LOWER_LAYER_DEACT_REQ:
        case TDSRRC_CPHY_DEACTIVATE_CNF:
        case TDSRRC_CRLC_RELEASE_ALL_CNF:
        case TDSRRC_LOWER_LAYER_DEACT_CNF:
        case TDSRRC_INVALIDATE_SIM_DATA_REQ:
        case TDSRRC_SIM_INSERTED_REQ:
        case TDSRRC_SIM_UPDATE_REQ:
        case RRC_STOP_TDS_MODE_REQ:
        case TDSRRC_FORBIDDEN_LAI_LIST_UPDATE_REQ:
        case TDSRRC_EQ_PLMN_LIST_CHANGE_IND:
        case TDSRRC_MODE_CHANGE_IND:
/*lint -e408*/
        case TDSRRC_INTERRAT_PLMN_SRCH_REQ:
        case TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ:
        case TDSRRC_BPLMN_FREQ_SCAN_TIMER_EXPIRED_IND:

        case TDSRRC_BPLMN_SEARCH_ABORT_REQ:

/*lint +e408*/
        case TDSRRC_CHANGE_MODE_CNF:
/*lint -e408*/
        case TDSRRC_INTERRAT_RESELECTION_REQ:
        case TDSRRC_INTERRAT_RESELECTION_ABORT_REQ:

#ifdef FEATURE_INTERRAT_HANDOVER_G2TD
        case TDSRRC_INTERRAT_HANDOVER_REQ:
        case TDSRRC_INTERRAT_HANDOVER_ABORT_REQ:
        /* case TDSRRC_CHANGE_MODE_REQ: */
        case TDSRRC_HO_CIPH_CNF:
#endif
        case TDSRRC_INTERRAT_CC_ORDER_REQ:
/* Take out from above feature */
        case TDSRRC_CHANGE_MODE_REQ:
/*lint +e408*/
        case TDSRRC_PLMN_LIST_REQ:
        case TDSRRC_CHANGE_MODE_IND:
        case TDSRRC_NEW_STATE_IND:
        case TDSRRC_STATE_CHANGE_IND:
        case TDSRRC_FREQ_SCAN_TIMER_EXPIRED_IND:
        case TDSRRC_DEEP_SLEEP_NO_SVC_TIMER_EXPIRED_IND:
        case TDSRRC_IRAT_ABORT_IND:
        case TDSRRC_SIX_HRS_SIB_TIMER_UPDATE_IND:
#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
        case TDSRRC_DED_PRI_VALDITY_TIMER_EXPIRED_IND:
#endif
#ifdef FEATURE_TDSCDMA_DEDICATED_PRI_INFO
	case TDSRRC_DEDICATED_PRIORITIES_REQ:
        case TDSRRC_CLEAR_DEDICATED_PRIORITIES_IND:
#endif
        case TDSRRC_SIB_CHANGE_GUARD_TIMER_EXPIRED_IND:
        case TDSRRC_DA_BACKOFF_TIMER_EXPIRED_IND:
        /*RR will send TDSRR_CIPHER_SYNC command to RRC when 
         *RRC is inactive, so return success here so that this command 
         *can be processed in dispatcher*/
/*lint -e408*/
        case TDSRR_CIPHER_SYNC:
/*lint +e408*/
/*lint -e408*/

        case TDSRRC_INTERRAT_REDIRECT_REQ:
        case TDSRRC_INTERRAT_REDIRECT_ABORT_REQ:
        case TDSRR_STOP_GSM_CNF:
#ifdef FEATURE_WCDMA_TO_TDSCDMA
        case RRC_TDSRRC_STOP_WCDMA_CNF:
#endif
/*lint +e408*/
        {
          /* The command needs to be processed */
          return_status = TRUE;
          break;
        }

/*lint -e408*/
        case TDSRRC_STOP_TDS_REQ:
          return_status = FALSE;
          break;
/*lint +e408*/

        case TDSRRC_LLC_CMD_REQ:
          if((cmd_ptr->cmd.rrc_llc_req.cmd_dest == TDSLAYER2_MAC_UL) &&
             (cmd_ptr->cmd.rrc_llc_req.l2_ul_cmd.cmd_id == TDSCMAC_UL_TRAFFIC_VOLUME_CONFIG_REQ) &&
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
             (cmd_ptr->cmd.rrc_llc_req.l2_ul_req != NULL ) &&
             (cmd_ptr->cmd.rrc_llc_req.l2_ul_req->mac_ul_traffic_vol_config->meas_cmd == TDSMAC_TRAFFIC_MEAS_DELETE_ALL)&&
             (cmd_ptr->cmd.rrc_llc_req.l2_ul_req->mac_ul_traffic_vol_config->config_conf_ind == FALSE))
#else/* FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/
             (cmd_ptr->cmd.rrc_llc_req.l2_ul_req.mac_ul_traffic_vol_config->meas_cmd == TDSMAC_TRAFFIC_MEAS_DELETE_ALL)&&
             (cmd_ptr->cmd.rrc_llc_req.l2_ul_req.mac_ul_traffic_vol_config->config_conf_ind == FALSE))
#endif
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Process the Delete All TVM measurements request in Inactive mode");
            return_status = TRUE;
          }
          else
          {
            return_status = FALSE;
          }
          break;

#ifdef FEATURE_LTE_TO_TDSCDMA
        case TDSRRC_MSGR_CMD:
          switch(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id)
          {
            case TDSCDMA_RRC_LTE_RESEL_REQ:
            case TDSCDMA_RRC_LTE_ABORT_RESEL_REQ:
            case TDSCDMA_RRC_LTE_PLMN_SRCH_REQ:
            case TDSCDMA_RRC_LTE_ABORT_PLMN_SRCH_REQ:
            case TDSCDMA_RRC_LTE_REDIR_REQ:
            case TDSCDMA_RRC_LTE_ABORT_REDIR_REQ:
            case TDSCDMA_RRC_LTE_UTRA_CAPABILITIES_REQ:
            case TDSCDMA_RRC_LTE_GET_DEDICATED_PRI_REQ:
            case TDSCDMA_RRC_LTE_CLEAR_DEDICATED_PRI_IND:
            case TDSCDMA_RRC_LTE_PSHO_REQ:
            case TDSCDMA_RRC_LTE_ABORT_PSHO_REQ:
            case TDSCDMA_RRC_LTE_GET_CGI_REQ:
            case TDSCDMA_RRC_LTE_ABORT_CGI_REQ:
            case TDSCDMA_RRC_LTE_GET_PLMN_PRTL_RESULTS_REQ:
#ifdef FEATURE_TDSCDMA_SUPPORT_LTE_DEPRIORITIZATION
            case TDSCDMA_RRC_LTE_DEPRI_FREQ_REQ:
#endif
              return_status = TRUE;
              break;
            default :
              return_status = FALSE;
              break;
          }
          break;
#endif

#ifdef FEATURE_WRLF_SYSTEM_SEL
         case TDSRRC_INITIATE_CELL_SELECTION_IND:
         case TDSRRC_CHANNEL_CONFIG_REQ:
         case TDSRRC_CHANNEL_CONFIG_CNF:
         case TDSRRC_T314_EXPIRED_IND:
         case TDSRRC_T315_EXPIRED_IND:
         case TDSRRC_T305_EXPIRED_IND:
         case TDSRRC_T307_EXPIRED_IND:
         case TDSRRC_T316_EXPIRED_IND:
         case TDSRRC_T317_EXPIRED_IND:
         case TDSRRC_CONN_MODE_OOS_TIMER_EXPIRED_IND:
         case TDSRRC_RRC_CON_DELAY_TIMER_EXPIRED_IND:
         case TDSRRC_SIG_CONN_RELEASE_REQ:
           if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
           {
             return_status = TRUE;
           }
           else
           {
             return_status = FALSE;
           }
          break;
#endif

        /* Return False for other commands */
        default:
        { 
          return_status = FALSE;
          break;
        }
      }
      break;
    }

    case TDSRRC_MODE_STANDBY:
      /*if EST_REQ is received save it for future use when we transition
        to ACTIVE substate*/
      switch (cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_EST_REQ:
          /* Check if WTOG BPLMN is currently active.If yes,then send an 
             IRAT_ABORT_IND to CSP procedure to abort the bplmn search */
          if((RRC_CSP_TD2G_BPLMN_AUTOMATIC_SCAN == tdsrrccsp_get_curr_scan())
              || (RRC_CSP_TD2G_BPLMN_MANUAL_SCAN == tdsrrccsp_get_curr_scan())
              || ((TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN == tdsrrccsp_get_curr_scan())
              && (cmd_ptr->cmd.rrc_est_req.cause == RRC_EST_DETACH)))
          {
            tdsrrcmcm_send_irat_abort_ind(); 
          }
          /* If TDS RRC receive a CS call est req when UE at CSFB mode ,
             RRC can abort the redirection/reslection to LTE. Because if UE 
             redirection/reselection to LTE, UE may fall back to TDS/GSM.*/
          /* if the feature  FEATURE_SGLTE not defined, UE is assumed as CSFB UE.
             if defined, get the ue mode from NAS information.*/
          else if((RRC_CS_DOMAIN_CN_ID == cmd_ptr->cmd.rrc_est_req.cn_domain_id)
                  &&((RRC_EST_ORIG_CONV_CALL == cmd_ptr->cmd.rrc_est_req.cause)
                  ||(RRC_EST_TERM_CONV_CALL == cmd_ptr->cmd.rrc_est_req.cause))
                  &&((TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN == tdsrrccsp_get_curr_scan()
                  ||(RRC_CSP_TDS_TO_GSM_CELL_RESELECTION_SCAN == tdsrrccsp_get_curr_scan())))
                  &&(TDSRRC_CSP_INTERRAT_LTE == tdsrrccsp_get_curr_interrat_type())
                  #ifdef FEATURE_SGLTE 
                  &&((SYS_UE_MODE_CSFB == tdsrrccsp_get_absolute_ue_mode())
                  ||(SYS_UE_MODE_CSFB_ONLY == tdsrrccsp_get_absolute_ue_mode()))
                  #endif
                  )
          {
            tdsrrcmcm_send_irat_abort_ind(); 
          }

          tdsrrcmcm_save_est_req(cmd_ptr);
          return_status = FALSE;
          break;
        
        case TDSRRC_ABORT_REQ:
          /* If an abort is received when an est_req is already pending,
           * clear the pending flag and return abort_cnf to mm.No
           * further processing is reqd on this abort,so return false
           */  
          if (tdsrrcmcm_est_req_saved_flag == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Abort Req in Standby mode with pending Est_req.Clearing Est_Req");
            tdsrrcmcm_est_req_saved_flag = FALSE;
            (void)tdsrrcrce_send_rrc_abort_cnf();
            return_status = FALSE;
          }
          else
          {
            /* Abort IRAT activity when ABORT REQ is received from NAS */
            if((TDSRRC_PROCEDURE_CSP == tdsprocedure_id ) &&
               ((RRC_CSP_TDS_TO_GSM_CELL_RESELECTION_SCAN == tdsrrccsp_get_curr_scan()) ||
               (RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN == tdsrrccsp_get_curr_scan())))
            {
             tdsrrcmcm_send_irat_abort_ind();
            }
            return_status = TRUE;
          }
          break;
          
        case TDSRRC_CMAC_UL_TRAFFIC_IND:
        case TDSRRC_MEASUREMENT_IND:
        /*Ignore TVM/GPS meas report when we are in standby and CCO is in progress*/
          if(!tdsrrccho_is_td2g_cco_active())
          {
             return_status = TRUE;
          }

          break;

        default:
          /*set status to true as all other events are honoured in this
            substate*/
          return_status = TRUE;
          break;
      }
      break;

    case TDSRRC_MODE_ACTIVE:
    case TDSRRC_MODE_INTERRAT_IN_PROGRESS:
    {
      /* Process all commands in the active mode */
      return_status = TRUE;
      break;
    }
    default:
    {
      switch (cmd_ptr->cmd_hdr.cmd_id)
      {
        /* Only the following commands are processed */
        case TDSRRC_FREQ_SCAN_TIMER_EXPIRED_IND:
        case TDSRRC_BPLMN_FREQ_SCAN_TIMER_EXPIRED_IND:
        case TDSRRC_SIX_HRS_SIB_TIMER_UPDATE_IND:
        case TDSRRC_SIB_CHANGE_GUARD_TIMER_EXPIRED_IND:
        /*RR will send TDSRR_CIPHER_SYNC command to RRC when 
         *RRC is inactive, so return success here so that this command 
         *can be processed in dispatcher*/
/*lint -e408*/
         case TDSRR_CIPHER_SYNC:
/*lint +e408*/
          return_status = TRUE;
          break;

        default :
          /* RRC Mode is not supported */
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unsupported RRC Mode: %d",tdsrrc_mode);
          return_status = FALSE;
          break;
      }
    }
  }

  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_STOP_TDS_REQ)
  {
    rr_cmd = (tdsrrc_rr_cmd_type *)cmd_ptr;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rxed TDSRRC_STOP_TDS_REQ from RR ,tdsstop_cause %d",rr_cmd->cmd.tdsrrc_rr_stop_tds_req.stop_req_cause);
    /*Only perform STOP_TDS_REQ from GRR when AS ID match.  If AS ID not match or TDS does not receive AS ID from NAS, 
	  *just send back STOP_TDS_CNF with as id set to the one brought by GRR
	  */
    if((tdsrrc_mode == TDSRRC_MODE_INACTIVE) && (rr_cmd->cmd.tdsrrc_rr_stop_tds_req.stop_req_cause == TDSRRC_POWER_OFF)
      && tdsrrc_is_sim_id_valid && (tdsrrc_sim_id_recvd == rr_cmd->cmd_hdr.as_id))
    {
      tdsrrc_csp_curr_select_data.num_available_freq = 0;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rxed TDSRRC_STOP_TDS_REQ in Inactive state,clear avail freq list.");
      tdsrrccsp_write_nv_acq_info();
      tdsl1_clear_barred_cells();
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"No processing of TDSRRC_STOP_TDS_REQ cmd from RR, tdsrrc_mode=%d,tdsstop_cause=%d",tdsrrc_mode,rr_cmd->cmd.tdsrrc_rr_stop_tds_req.stop_req_cause);
    }

    tdsrrccsp_send_rrc_rr_stop_tds_cnf(rr_cmd->cmd.tdsrrc_rr_stop_tds_req.stop_req_cause,rr_cmd->cmd_hdr.as_id); 
    return_status = FALSE;           
  }

#if defined (FEATURE_TDSCDMA_TO_WCDMA) && defined (FEATURE_WCDMA_TO_TDSCDMA)
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_RRC_STOP_TDS_REQ)
  {
    tdsrrc_rrc_cmd_type * wrrc_cmd_ptr = (tdsrrc_rrc_cmd_type *) cmd_ptr;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rxed TDSRRC_RRC_STOP_TDS_REQ from WRRC ,tdsstop_cause %d",wrrc_cmd_ptr->cmd.tdsrrc_rrc_stop_tds_req.stop_req_cause);
    if((tdsrrc_mode == TDSRRC_MODE_INACTIVE) && (wrrc_cmd_ptr->cmd.tdsrrc_rrc_stop_tds_req.stop_req_cause == POWER_OFF))
    {
      tdsrrc_csp_curr_select_data.num_available_freq = 0;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rxed TDSRRC_RRC_STOP_TDS_REQ in Inactive state,clear avail freq list.");
      tdsrrccsp_write_nv_acq_info();
      tdsl1_clear_barred_cells();
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"No processing of TDSRRC_RRC_STOP_TDS_REQ cmd from WRRC, tdsrrc_mode=%d,tdsstop_cause=%d",tdsrrc_mode,wrrc_cmd_ptr->cmd.tdsrrc_rrc_stop_tds_req.stop_req_cause);
    }
    
    tdsrrccsp_send_wrrc_stop_tds_cnf(wrrc_cmd_ptr->cmd.tdsrrc_rrc_stop_tds_req.stop_req_cause); 
    return_status = FALSE;           
  }
#endif /* #if defined (FEATURE_TDSCDMA_TO_WCDMA) && defined (FEATURE_WCDMA_TO_TDSCDMA) */

  if(return_status == FALSE)
  {
    //delete embedded pointer allocated for MEASUREMENT_REQ
    if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_LLC_CMD_REQ)
    {
      if(cmd_ptr->cmd.rrc_llc_req.cmd_dest == TDSLAYER1_PHY)
      {
        if(cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id == TDSL1_CPHY_MEASUREMENT_REQ)
        {
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
          if((cmd_ptr->cmd.rrc_llc_req.l1_req != NULL ) && 
            (cmd_ptr->cmd.rrc_llc_req.l1_req->meas.meas_ptr))
#else/* FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/
          if(cmd_ptr->cmd.rrc_llc_req.l1_req.meas.meas_ptr)
#endif/* FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/
          {
            tdsrrc_free_embedded_buf(cmd_ptr, NULL);
          }
        }
#ifdef FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT
        else if(cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id == TDSL1_CPHY_CELL_BAR_REQ)
        {
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
          if((cmd_ptr->cmd.rrc_llc_req.l1_req != NULL ) && 
            (cmd_ptr->cmd.rrc_llc_req.l1_req->cell_bar_req.forbidden_earfch_cell_list))
#else/* FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/
          if(cmd_ptr->cmd.rrc_llc_req.l1_req.cell_bar_req.forbidden_earfch_cell_list)
#endif/* FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/
          {
            tdsrrc_free_embedded_buf(cmd_ptr, NULL);
          }
        }
#endif
      }
      //delete embedded pointer allocated to TVM
      else if(cmd_ptr->cmd.rrc_llc_req.cmd_dest == TDSLAYER2_MAC_UL)
      {
        if(cmd_ptr->cmd.rrc_llc_req.l2_ul_cmd.cmd_id == TDSCMAC_UL_TRAFFIC_VOLUME_CONFIG_REQ)
        {
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
          if((cmd_ptr->cmd.rrc_llc_req.l2_ul_req != NULL ) &&
            (cmd_ptr->cmd.rrc_llc_req.l2_ul_req->mac_ul_traffic_vol_config))
#else/* FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/
          if(cmd_ptr->cmd.rrc_llc_req.l2_ul_req.mac_ul_traffic_vol_config)
#endif/* FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/
          {
            tdsrrc_free_embedded_buf(cmd_ptr, NULL);
          }
        }
      }
      else
      {
        // Add other commands requiring embedded pointers to be freed here
      }
    }
  }

  return (return_status);
} /* end of tdsrrcmcm_is_process_required */


/*===========================================================================

FUNCTION   RRCMCM_GET_SUBSTATE

DESCRIPTION

  This function retrieves the RRC MCM SUBSTATE.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  Returns tdsrrcmcm_state in tdsrrcmcm_state_e_type

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcmcm_state_e_type tdsrrcmcm_get_substate( void )
{
  /* return the current RRC mode */
  return (tdsrrcmcm_state);
}
/*===========================================================================

FUNCTION   RRCMCM_GET_RRC_MODE

DESCRIPTION

  This function retrieves the RRC current mode.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  Returns tdsrrc_mode in tdsrrc_mode_e_type

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_mode_e_type tdsrrcmcm_get_rrc_mode( void )
{
  /* return the current RRC mode */
  return (tdsrrc_mode);
}
/*===========================================================================

FUNCTION   RRCMCM_UPDATE_RRC_MODE_TO_ACTIVE

DESCRIPTION

  This function sets the current RRC mode to active.Currently called 
  by CHO procedure after the GTOW PCCO is successful.
  
  Only IRAT_IN_PROGRESS to ACTIVE is allowed
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_update_rrc_mode_to_active()
{
  if(TDSRRC_MODE_INTERRAT_IN_PROGRESS == tdsrrc_mode)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Mode updated to Active from IRAT_IN_PROGRESS");
    tdsrrc_mode = TDSRRC_MODE_ACTIVE;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid request for tdsrrc_mode update to active.Current tdsrrc_mode is %d",tdsrrc_mode);
  }
}
/*===========================================================================

FUNCTION   RRCMCM_IS_DUALMODE_ENABLED

DESCRIPTION

  This function returns a boolean to indicate if inter-RAT operation is
  enabled.

DEPENDENCIES

  None.

RETURN VALUE

  Returns a boolean value.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_is_dualmode_enabled(void)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Dual mode status %d",tdsrrc_dual_mode_enabled);
  return (tdsrrc_dual_mode_enabled);
}
#ifdef FEATURE_CMCC_SECURE_CALL
/*===========================================================================

FUNCTION  TDSRRCMCM_IS_SECURE_CALL_ENABLED

DESCRIPTION

  This function returns True if tdsrrc_is_secure_call_enabled, otherwise,
  it returns False.

DEPENDENCIES

  None.

RETURN VALUE

  Return  tdsrrc_is_secure_call_enabled in boolean.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_is_secure_call_enabled(void)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Secure call: status is %d (0: disable,1: enable)",tdsrrc_is_secure_call_enabled);
  return (tdsrrc_is_secure_call_enabled);
}
/*===========================================================================

FUNCTION  TDSRRCMCM_UPDATE_SECURE_CALL_STATUS

DESCRIPTION

  This function Update tdsrrc_is_secure_call_enabled.

DEPENDENCIES

  None.

RETURN VALUE

  Update tdsrrc_is_secure_call_enabled.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_update_secure_call_status(boolean is_secure_call_disabled)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Secure call: update to: %d (0: disable,1: enable)",is_secure_call_disabled);
  tdsrrc_is_secure_call_enabled = is_secure_call_disabled;
}
#endif
/*===========================================================================

FUNCTION  RRCMCM_IS_TDS_ONLY_MODE

DESCRIPTION

  This function returns True if only TDSCDMA mode is enabled, otherwise,
  it returns False.

DEPENDENCIES

  None.

RETURN VALUE

  Return tdsrrcmcm_is_tds_only_mode status in boolean.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_is_tds_only_mode(void)
{
   boolean lte_mode_enabled = FALSE;
   boolean ret = FALSE;

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
   lte_mode_enabled = tdsrrc_lte_mode_enabled;
#endif

   ret = !(tdsrrc_dual_mode_enabled || lte_mode_enabled);

   TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDS only mode: %d, G: %d, LTE: %d", 
               ret,
               tdsrrc_dual_mode_enabled, 
               lte_mode_enabled);

   return ret;
}

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)

/*===========================================================================

FUNCTION  RRCMCM_IS_LTE_MODE_ENABLED

DESCRIPTION

  This function returns True if LTE mode is enabled, otherwise FALSE

DEPENDENCIES

  None.

RETURN VALUE

  Return rrc_dual_mode_enabled status in boolean.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_is_lte_mode_enabled
(
  void
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"LTE_IRAT: LTE mode enabled status %d ",tdsrrc_lte_mode_enabled);
  return tdsrrc_lte_mode_enabled;
}

/*===========================================================================

FUNCTION  rrcmcm_update_lte_mode_enabled_status

DESCRIPTION

  This function updates the status of the flag whether if LTE mode is
  enabled or not.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_update_lte_mode_enabled_status
(
  boolean status
)
{
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"LTE_IRAT: LTE mode enabled status old:%d,new:%d ",tdsrrc_lte_mode_enabled,status);
  tdsrrc_lte_mode_enabled = status;
}
#endif

#ifdef FEATURE_RAT_PRIORITY_LIST
/*===========================================================================

FUNCTION   RRCMCM_UPDATE_MODE_CAPABILITY

DESCRIPTION

  This function updates GSM, LTE mode capability from NAS message
DEPENDENCIES

  None.

RETURN VALUE

  Returns a boolean value.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_update_mode_capability
(
  sys_rat_pri_list_info_s_type *rat_pri_list_info_ptr
)
{
  uint8 rat_idx = 0;

  tdsrrc_dual_mode_enabled = FALSE;
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
  tdsrrc_lte_mode_enabled = FALSE;
#endif

  for(rat_idx = 0; rat_idx < rat_pri_list_info_ptr->num_items && rat_idx < SYS_MAX_ACQ_PREF_RAT_LIST_NUM; rat_idx++)
  {
    if(rat_pri_list_info_ptr->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_GSM)
    {
      tdsrrc_dual_mode_enabled = TRUE;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IRAT: Dual mode/GSM mode enabled");
    }
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
    if(rat_pri_list_info_ptr->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_LTE)
    {
      tdsrrc_lte_mode_enabled = TRUE;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LTE_IRAT: LTE mode enabled");
    }
#endif
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"IRAT: GSM mode enabled: %d",tdsrrc_dual_mode_enabled);
}

/*===========================================================================

FUNCTION   RRCMCM_UPDATE_MODE_CAPABILITY_FROM_SERVICE_REQ

DESCRIPTION

  This function updates GSM, LTE mode capability from TDSRRC_SERVICE_REQ.
DEPENDENCIES

  None.

RETURN VALUE

  Returns a boolean value.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_update_mode_capability_from_service_req
(
  sys_rat_pri_list_info_s_type *rat_pri_list_info_ptr
)
{
  uint8 rat_idx = 0;
  tdsrrc_dual_mode_enabled = FALSE;
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
  tdsrrc_lte_mode_enabled = FALSE;
#endif

  for(rat_idx = 0; rat_idx < rat_pri_list_info_ptr->num_items && rat_idx < SYS_MAX_ACQ_PREF_RAT_LIST_NUM; rat_idx++)
  {
    if(rat_pri_list_info_ptr->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_GSM)
    {
      if(rat_pri_list_info_ptr->priority_list_info[rat_idx].bst_rat_acq_required)
      {
        tdsrrc_dual_mode_enabled = TRUE;
      }
    }
    /* Update LTE mode at the start of service search itself */
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
    else if(rat_pri_list_info_ptr->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_LTE)
    {
      tdsrrc_lte_mode_enabled = TRUE;
    }
#endif
  }
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"GSM mode enabled status %d ,LTE mode enabled status %d",tdsrrc_dual_mode_enabled,tdsrrc_lte_mode_enabled);
#else
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"GSM mode enabled status %d ",tdsrrc_dual_mode_enabled);
#endif
}


/*===========================================================================

FUNCTION  RRCMCM_IS_MODE_ENABLED_IN_RAT_PRI_LIST

DESCRIPTION

  This function returns True if mode passed as argument is enabled in RAT PRI LIST, otherwise FALSE

DEPENDENCIES

  None.

RETURN VALUE

  Return TRUE is LTE mode is enabled in RAT PRI LIST

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_is_mode_enabled_in_rat_pri_list
(
  sys_rat_pri_list_info_s_type *rat_pri_list_info_ptr,
  sys_sys_mode_e_type mode
)
{
  uint8 rat_idx = 0;
  boolean mode_enabled = FALSE;
  for(rat_idx = 0; rat_idx < rat_pri_list_info_ptr->num_items && rat_idx < SYS_MAX_ACQ_PREF_RAT_LIST_NUM; rat_idx++)
  {
    if(rat_pri_list_info_ptr->priority_list_info[rat_idx].acq_sys_mode == mode)
    {
      mode_enabled = TRUE;
      break;
    }
  }
  return mode_enabled;
}
#endif



/*===========================================================================

FUNCTION  RRCMCM_REGISTER_FOR_MODE_CHANGE

DESCRIPTION

  This function registers a notification when a RRC mode change occurs for a
  given condition. The from_mode is the mode from which the transition occurs 
  abd the to_mode is the mode to which the tansition occurs.
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_register_for_mode_change(
  tdsrrc_proc_e_type procedure,
  tdsrrc_mode_e_type from_mode,
  tdsrrc_mode_e_type to_mode
)
{
  /* Verify the procedure identifier */
  if (procedure >= TDSMAX_NUM_OF_PROCEDURES) 
  {
    /* Invalid procedure identifier */
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Procedure ID: %d",procedure);
  }
  else
  {
    /* Check if the list is full */
    if (tdsrrcmcm_notification_counter < (uint16)TDSRRCMCM_MAX_NOTIFICATIONS) 
    {
      /* Store the request into the mode change notification list */
      tdsrrcmcm_notifications[tdsrrcmcm_notification_counter].procedure = procedure;
      tdsrrcmcm_notifications[tdsrrcmcm_notification_counter].from_mode = from_mode;
      tdsrrcmcm_notifications[tdsrrcmcm_notification_counter].to_mode = to_mode;
      tdsrrcmcm_notification_counter++;
    }
    else
    {
      /* the list is full */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"The mode change list is full");
    }
  }
  return;
}

/*===========================================================================

FUNCTION RRCMCM_MAP_IRAT_REASON

DESCRIPTION

  This function maps the internal change mode request reason to
  the appropriate Suspend/Resume reason for L1 command
          
DEPENDENCIES

  None.
 
RETURN VALUE

  Suspend/Resume reason

SIDE EFFECTS

  None.

===========================================================================*/
static tdsl1_suspend_resume_cause_enum_type tdsrrcmcm_map_irat_reason
( 
  tdsrrc_change_mode_reason_e_type reason 
)
{
  /* Fill in the reason for suspension */
  switch(reason)
  {
    case TDSINTERRAT_REASON_OTHER:
    return TDSL1_IRAT_REASON_OTHER;
  case TDSINTERRAT_RESELECTION:
    return TDSL1_IRAT_RESELECTION;
  case TDSINTERRAT_REDIRECTION:
    return TDSL1_IRAT_REDIRECTION;
  case TDSINTERRAT_OOS:
    return TDSL1_IRAT_OOS;
  case TDSINTERRAT_BPLMN:
    return TDSL1_IRAT_BPLMN;
  case TDSINTERRAT_IHO:
    return TDSL1_IRAT_IHO;
  case TDSINTERRAT_CCO:
    return TDSL1_IRAT_CCO;
  case TDSINTERRAT_DSIM_TUNEAWAY:
    return TDSL1_IRAT_DUAL_SIM_TUNEAWAY;
  case TDSINTERRAT_TX_NEEDED:
    return TDSL1_IRAT_TX_NEEDED;
  case TDSINTERRAT_REASON_NONE:
    return TDSL1_IRAT_NONE;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid IRAT reason %d in change mode request",reason); 
    return TDSL1_IRAT_NONE;
  }
}

/*===========================================================================

FUNCTION   RRCMCM_SEND_START_TDSCDMA_REQ

DESCRIPTION

  This function sends a TDSL1_CPHY_START_TDSCDMA_MODE_REQ to L1 to initiate the 
  TDS mode activation. 
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_start_tdscdma_req( void )
{
  tdsl1_ext_cmd_type *l1_cmd_ptr;


  /*Register L1-L3 command call back functions*/
  /*No de-registration needed*/
  if(tdsl1_register_upper_layer_controller(TDSL1_CONTROLLER_RRC,
    &tdsrrc_get_l1_cmd_buf, 
    &tdsrrc_put_l1_cmd,
    &tdsrrc_malloc_for_external_cmd ) == FALSE)
  {
    ERR_FATAL("Could not register L1 call back function for L1-L3 commands",0,0,0);
  }

  /* Get a L1 command buffer */
  if ((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending CPHY_START_WCEMA_MODE_REQ",0,0,0);
  }

  /* Fill in the Command Header */
  l1_cmd_ptr->cmd_hdr.cmd_id = TDSL1_CPHY_START_TDSCDMA_MODE_REQ;

  l1_cmd_ptr->cmd_hdr.act_time_type = TDSL1_ACTIVATION_TIME_NONE;
  l1_cmd_ptr->cmd_hdr.act_time      = 0;
  //l1_cmd_ptr->cmd.start.mdsp_image = TDSL1_MDSP_IMAGE_WCDMA_GSM;

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined (FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
  /*set the L1 start tds mode cause for irat timing case, to prevent L1 reset stmr*/
  if(tdsrrc_is_x2td_irat_hho_timing())
  {
    l1_cmd_ptr->cmd.start.cause = TDSL1_START_CAUSE_IRAT_TIMING;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC->L1 start cause: TDSL1_START_CAUSE_IRAT_TIMING");
  }
  else
#endif
  if (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_INTERRAT_PLMN_SRCH_REQ)
  {
    l1_cmd_ptr->cmd.start.cause = TDSL1_START_CAUSE_BPLMN_G2T;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC->L1 start cause: TDSL1_START_CAUSE_BPLMN_G2T");
  }
  else
#ifdef FEATURE_LTE_TO_TDSCDMA
  if (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_LTE_PLMN_SRCH_REQ)
  {
    l1_cmd_ptr->cmd.start.cause = TDSL1_START_CAUSE_BPLMN_L2T;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC->L1 start cause: TDSL1_START_CAUSE_BPLMN_L2T");
  }
  else
#endif
  {
    l1_cmd_ptr->cmd.start.cause = TDSL1_START_CAUSE_OTHER;
	TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC->L1 start cause: TDSL1_START_CAUSE_OTHER");
  }

#ifdef FEATURE_SGLTE
    l1_cmd_ptr->cmd.start.meas_mode = tdsrrc_get_ue_mode();
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SGLTE: pass meas mode %d in CPHY_START_TDSCDMA_MODE_REQ",l1_cmd_ptr->cmd.start.meas_mode);
#endif

#ifdef FEATURE_DUAL_SIM
  l1_cmd_ptr->cmd_hdr.sys_modem_as_id = tdsrrc_get_as_id();
#endif

  /* Now send the primitive to L1 */
  tdsl1_put_cmd(l1_cmd_ptr);
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSL1_CPHY_START_TDSCDMA_MODE_REQ sent to L1");

  return;
} /* end of tdsrrcmcm_send_start_tdscdma_req */


/*===========================================================================

FUNCTION   tdsrrcmcm_send_suspend_tds_mode_req

DESCRIPTION

  This function sends a TDSL1_CPHY_SUSPEND_TDSCDMA_MODE_REQ to L1 to initiate the 
  TDS mode deactivation. 
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_suspend_tds_mode_req
( 
  tdsrrc_change_mode_reason_e_type irat_reason 
)
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_LLC_CMD_REQ",0,0,0);
  }
  else
  {
    tdsl1_req_cmd * llc_cmd_l1_req = NULL;
    tdsrrc_get_pointer_l1_req_value(&cmd_ptr->cmd.rrc_llc_req.l1_req, &llc_cmd_l1_req);
    /* Fill in the LLC Command Header */
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_LLC_CMD_REQ;
    cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_MCM;
    cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;

    /* Fill in L1 Command */
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_SUSPEND_TDSCDMA_MODE_REQ;

    if(tdsprocedure_id == TDSRRC_PROCEDURE_IHO)
    {
      /* Get the activation time for TtoG handover.. We will suspend TDS mode
        only after this activation time*/
      if( tdsrrciho_get_activation_time_for_td2g_ho(&(cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time)) 
        == SUCCESS)
      {
        cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = TDSL1_ACTIVATION_TIME_CFN;
        cmd_ptr->cmd.rrc_llc_req.l1_cmd.sdu_time      = tdsrrc_cfn_before_decode[tdsprocedure_id]; 
      }
      else
      {
        cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = TDSL1_ACTIVATION_TIME_NONE;
      }
    }
    else if(tdsprocedure_id == TDSRRC_PROCEDURE_CHO)
    {
      /* Get the activation time for TtoG Cell Change Order. We will suspend TDS mode
      only after this activation time*/
      if( tdsrrccho_get_activation_time_for_td2g_cho(&(cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time)) 
        == SUCCESS)
      {
        cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = TDSL1_ACTIVATION_TIME_CFN;
        cmd_ptr->cmd.rrc_llc_req.l1_cmd.sdu_time      = tdsrrc_cfn_before_decode[tdsprocedure_id]; 
      }
      else
      {
        cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = TDSL1_ACTIVATION_TIME_NONE;
      }
    }
    else
    {
      /* Set the activation time to NONE */
      cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = TDSL1_ACTIVATION_TIME_NONE;
    }

    /* Set this flag to FALSE in case another RAT fails camping */
    tdsrrc_csp_int_data.rlf_skip_first_svc_req = FALSE;
    
    /* Fill in the irat reason for suspension */
    llc_cmd_l1_req->suspend_req.cause = tdsrrcmcm_map_irat_reason(irat_reason);
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IRAT_reason in Suspend Req is %d",llc_cmd_l1_req->suspend_req.cause);

    /* Fill in the target rat info for suspend */
	llc_cmd_l1_req->suspend_req.rat = TDSL1_INTERRAT_MAX;
	
    if(tdsprocedure_id == TDSRRC_PROCEDURE_IHO)
    {
      if (tdsprocedure_info.handover_type == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM)
      {
        llc_cmd_l1_req->suspend_req.rat = TDSL1_INTERRAT_GSM;
      }
	  else if(tdsprocedure_info.handover_type == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA)
	  {
	    llc_cmd_l1_req->suspend_req.rat = TDSL1_INTERRAT_LTE;
	  }	  
    }
	else if(tdsprocedure_id == TDSRRC_PROCEDURE_CHO)
	{
	  llc_cmd_l1_req->suspend_req.rat = TDSL1_INTERRAT_GSM;
	}
	else if(tdsprocedure_id == TDSRRC_PROCEDURE_CSP)
	{ 
	  switch (tdsrrc_csp_int_data.curr_scan)
	  {
        case RRC_CSP_TD2G_BPLMN_AUTOMATIC_SCAN:
        case RRC_CSP_TD2G_BPLMN_MANUAL_SCAN:
		case TDSRRC_CSP_PLMN_LIST_SCAN:
#ifdef FEATURE_TDSCDMA_TO_LTE
          if (tdsrrc_csp_int_data.bplmn_interrat_type == TDSRRC_CSP_INTERRAT_LTE)
	      {
		    llc_cmd_l1_req->suspend_req.rat = TDSL1_INTERRAT_LTE;
		  }
		  else
#endif
		  {
		    llc_cmd_l1_req->suspend_req.rat = TDSL1_INTERRAT_GSM;
		  }
		  break;

        case RRC_CSP_TDS_TO_GSM_CELL_RESELECTION_SCAN:
#ifdef FEATURE_TDSCDMA_TO_LTE
          if (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE)
          {
            llc_cmd_l1_req->suspend_req.rat = TDSL1_INTERRAT_LTE;
          }
		  else
#endif
          {
            llc_cmd_l1_req->suspend_req.rat = TDSL1_INTERRAT_GSM;
          }
		  break;

		case TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_IDLE_STATE_SCAN:
        case TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_CONNECTING_STATE_SCAN:
        case TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN:
          if(tdsrrcmcm_is_dualmode_enabled())
          {
		    llc_cmd_l1_req->suspend_req.rat = TDSL1_INTERRAT_GSM;
          }
		  break;

		case TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN:
		  if(tdsrrc_csp_int_data.saved_rrc_cmd.cmd.redirect_req.choice == TDSRRC_INTER_SYSTEM_GSM)
		  {
		    llc_cmd_l1_req->suspend_req.rat = TDSL1_INTERRAT_GSM;
		  }
		  else if ( (tdsrrc_csp_int_data.saved_rrc_cmd.cmd.redirect_req.choice == TDSRRC_INTER_SYSTEM_LTE)
#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
                   || (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE)
#endif
		          )
		  {
		    llc_cmd_l1_req->suspend_req.rat = TDSL1_INTERRAT_LTE;
		  }
		  break;

	    default:
		  llc_cmd_l1_req->suspend_req.rat = TDSL1_INTERRAT_MAX;
		  break;
	  }
	}
	TDSRRC_MSG3(MSG_LEGACY_HIGH,"target RAT in Suspend Req is %d, dest proc %d, curr_scan type %d",
		        llc_cmd_l1_req->suspend_req.rat,tdsprocedure_id,tdsrrc_csp_int_data.curr_scan);
  }

  /* Now send the primitive to L1 */
  tdsrrc_put_int_cmd(cmd_ptr);
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSL1_CPHY_SUSPEND_TDSCDMA_MODE_REQ sent to L1");

  return;
} /* end of send_suspend_tds_mode_req */



/*===========================================================================

FUNCTION   tdsrrcmcm_send_resume_tds_mode_req

DESCRIPTION

  This function sends a TDSL1_CPHY_RESUME_TDSCDMA_MODE_REQ to L1 to initiate the 
  TDS mode activation through TDSRRC_LLC_CMD_REQ. 
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_resume_tds_mode_req
(
  tdsrrc_change_mode_reason_e_type irat_reason 
)
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_LLC_CMD_REQ",0,0,0);
  }
  else
  {
    tdsl1_req_cmd * llc_cmd_l1_req = NULL;
    tdsrrc_get_pointer_l1_req_value(&cmd_ptr->cmd.rrc_llc_req.l1_req, &llc_cmd_l1_req);
   

    /* Fill in the LLC Command Header */
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_LLC_CMD_REQ;
    cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_MCM;
    cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;

    /* Fill in L1 Command */
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_RESUME_TDSCDMA_MODE_REQ;
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = TDSL1_ACTIVATION_TIME_NONE;

    /* Fill in the irat reason for suspension */
    llc_cmd_l1_req->resume_req.cause = tdsrrcmcm_map_irat_reason(irat_reason);
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IRAT_reason in Resume Req is %d",llc_cmd_l1_req->resume_req.cause);
#ifdef FEATURE_SGLTE
    /* Pass current meas mode to L1, since it may have changed when L1 is SUSPENDED during T2G CCO */
    llc_cmd_l1_req->resume_req.meas_mode = tdsrrc_get_ue_mode();
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SGLTE: pass meas mode %d in CPHY_RESUME_TDSCDMA_MODE_REQ",llc_cmd_l1_req->resume_req.meas_mode);
#endif
  }

  /* Now send the primitive to L1 */
  tdsrrc_put_int_cmd(cmd_ptr);
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSL1_CPHY_RESUME_TDSCDMA_MODE_REQ sent to L1");

  return;
} /* end of tdsrrcmcm_send_resume_tds_mode_req */


/*===========================================================================

FUNCTION   tdsrrcmcm_send_stop_tds_cnf

DESCRIPTION

  This function sends a RRC_STOP_TDS_MODE_CNF to MM to initiate the 
  TDS mode has been deactivated. 
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_stop_tds_cnf(boolean append_rel_ind)
{
  mm_cmd_type *mm_buf_ptr;

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_STOP_TDSCDMA_MODE_CNF)) == NULL)
  {
    ERR_FATAL("No MM buffer for RRC_STOP_TDS_MODE_CNF",0,0,0);
  }

#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_stop_wcdma_mode_cnf.as_id = tdsrrc_get_as_id();
#endif

  if((append_rel_ind) 
#ifdef FEATURE_WRLF_SYSTEM_SEL
      && (!TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
#endif
    )
  {
    /* Retrieve START values from SMC */
    (void) tdsrrcsmc_append_hfns_to_rel_ind(&(mm_buf_ptr->cmd.rrc_stop_wcdma_mode_cnf.start_cs[0]),
                                  &(mm_buf_ptr->cmd.rrc_stop_wcdma_mode_cnf.start_ps[0]));
  }

  /* Put the command on MM queue */
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_mm_put_cmd(mm_buf_ptr);
  }
  mm_put_cmd(mm_buf_ptr);
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_STOP_TDS_MODE_CNF sent to MM");

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting tdshsdpa_hsupa_ind");
    tdshsdpa_hsupa_ind = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
  }

  tdsrrcwrm_stop_lock_wait_timer();
  tdsrrcwrm_release_lock();

#ifdef FEATURE_3GPP_CSFB_TDS
  tdsrrc_csfb_call_status = FALSE;
#endif

  return;
} /* end of tdsrrcmcm_send_stop_tds_cnf */



/*===========================================================================

FUNCTION   RRCMCM_SEND_SERVICE_CNF_WITH_FAILURE

DESCRIPTION

  This function sends a RRC_SERVICE_CNF to MM to with the 
  proper failure cause. 
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_service_cnf_with_failure( void )
{
  mm_cmd_type *mm_buf_ptr;

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_SERVICE_CNF)) == NULL)
  {
    ERR_FATAL("No MM buffer for RRC_SERVICE_CNF",0,0,0);
  }

#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_service_cnf.as_id = tdsrrc_get_as_id();
#endif

  /*if act_cause is cell change order for gtow, set that cause*/
  if(tdsrrcmcm_act_cause == RRCMCM_INTERRAT_CC_ORDER_G2TD)
  {
    mm_buf_ptr->cmd.rrc_service_cnf.status = MM_AS_INTERRAT_CC_ORDER;
  }
  /* Check current RRC state */
  else if (tdsrrcmcm_act_cause == RRCMCM_INTERRAT_HANDOVER_G2TD) 
  {
    /* Set status to MM_AS_INTERRAT_HANDOVER */
    mm_buf_ptr->cmd.rrc_service_cnf.status = MM_AS_INTERRAT_HANDOVER;
  }
  else if(tdsrrcmcm_act_cause == RRCMCM_INTERRAT_CC_ORDER_TD2G_FAILED)
  {
    /* Set status to MM_AS_WTOG_CC_ORDER_FAILED */
    mm_buf_ptr->cmd.rrc_service_cnf.status  = MM_AS_WTOG_CC_ORDER_FAILED;
  }
  else
  {
    /* Set status to MM_AS_INTERRAT_RESELECTION */
    mm_buf_ptr->cmd.rrc_service_cnf.status = MM_AS_INTERRAT_RESELECTION;
  }

  mm_buf_ptr->cmd.rrc_service_cnf.is_cell_id_valid = FALSE;

  /* Put the command on MM queue */
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_mm_put_cmd(mm_buf_ptr);
  }
  mm_put_cmd(mm_buf_ptr);
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_SERVICE_CNF sent to MM");

  return;
} /* end of tdsrrcmcm_send_service_cnf_with_failure */


/*===========================================================================

FUNCTION   RRCMCM_SEND_PLMN_LIST_CNF_WITH_FAILURE

DESCRIPTION

  This function sends a RRC_PLMN_LIST_CNF to MM to with the
  proper failure cause.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_send_plmn_list_cnf_with_failure
( 
  sys_network_selection_mode_e_type network_select_mode , 
  uint8 trans_id
#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
  , sys_service_search_e_type svc_srch_bgnd
#endif
)
{
  mm_cmd_type *mm_buf_ptr;

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_PLMN_LIST_CNF)) == NULL)
  {
    ERR_FATAL("No MM buffer for RRC_PLMN_LIST_CNF",0,0,0);
  }

#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.as_id = tdsrrc_get_as_id();
#endif

  /*set the number of plmns to 0 in the plmn list*/
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.num_plmns = 0;

  /* Set the status */
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.status = MM_AS_ABORTED;

  /* Set the network select mode */
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.network_select_mode = network_select_mode;
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.trans_id = trans_id;

#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.service_search = svc_srch_bgnd;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"BSS: svc_srch_bgnd %d sent in plmn_list_cnf",svc_srch_bgnd);
#endif

  /* Put the command on MM queue */
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_mm_put_cmd(mm_buf_ptr);
  }
  mm_put_cmd(mm_buf_ptr);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC_PLMN_LIST_CNF sent to MM with trans_id = %d",mm_buf_ptr->cmd.rrc_plmn_list_cnf.trans_id);

  return;
} /* end of tdsrrcmcm_send_plmn_list_cnf_with_failure */

/*===========================================================================

FUNCTION   RRCMCM_SEND_STOP_LOWER_LAYER_REQ

DESCRIPTION

  This function sends a TDSRRC_STOP_LOWER_LAYER_CNF to RRC/LLC to initiate the 
  TDS mode deactivation in the lower layers. 
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_stop_lower_layer_req( boolean due_to_stop_mode )
{
  tdsrrc_cmd_type *cmd_ptr;

  /* For rrc connection release or rrc connection reject with redirection info End Event*/
  tdsrrclog_tdscdma_to_tdscdma_redir_end_event_type td2td_redir_event;
  if ((TRUE == tdsrrc_t2t_redir_due_to_rls) || (TRUE == tdsrrc_t2t_redir_due_to_rej))
  {
    tdsrrc_t2t_redir_due_to_rls = FALSE;
    tdsrrc_t2t_redir_due_to_rej = FALSE;
    td2td_redir_event.status = TDTOTD_FAILURE;
    event_report_payload(EVENT_TDSCDMA_TO_TDSCDMA_REDIRECTION_END, sizeof(td2td_redir_event),
                          (void *)&td2td_redir_event);
  }

  /* Check if WTOG abort is in progress. If so, wait for RR to respond*/
  if ( (tdsrrc_mode == TDSRRC_MODE_STANDBY  ) &&
       (tdsrrccsp_check_td2g_abort_in_progress(tdsstop_cause)) 
     )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"W2G Abort in Progress. Holding Stop till further");
    return;
  }

#ifdef FEATURE_WRLF_SYSTEM_SEL
  /* If stop is received from NAS, when SIB read is in progress for WRLF, release BCCH.*/
  if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
  {
    tdsrrccsp_check_and_teardown_sbcch_during_wrlf();
  }
#endif
  tdsrrccsp_check_and_teardown_sbcch_during_bplmn();

  tdsrrccsp_cleanup_bplmn_state();

  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN)
  {
    tdsrrccsp_send_mm_plmn_list_cnf_as_aborted(tdsrrc_csp_int_data.bplmn_trans_id);
  }

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_STOP_LOWER_LAYER_REQ",0,0,0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_STOP_LOWER_LAYER_REQ;

  /* Fill in procedure id    */
  cmd_ptr->cmd.stop_lower_layer_req.procedure = TDSRRC_PROCEDURE_MCM;

  /* Set the cause field */
  if (tdsstop_cause == RRC_DEEP_SLEEP)
  {
    cmd_ptr->cmd.stop_lower_layer_req.cause = TDSL1_DEEP_SLEEP;

    /* Set the UE in deep sleep variable in CSP only if it's due to 
     * STOP_TDS_MODE_REQ and not for TDSRRC_MODE_CHANGE_REQ
     */
    if(due_to_stop_mode)
    {
      tdsrrccsp_set_ue_in_deep_sleep();
    }
  }
  else
  {
    cmd_ptr->cmd.stop_lower_layer_req.cause = TDSL1_RAT_CHANGE;
  }
  
  /* Set this flag to FALSE in case another RAT fails camping */
  tdsrrc_csp_int_data.rlf_skip_first_svc_req = FALSE;

  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_STOP_LOWER_LAYER_REQ to LLC:%d",tdsstop_cause);
  
  /* MCM sent stop lower layer req*/
  tdsrrcmcm_sent_to_llc_tds_lower_req = TRUE;
 
  if((tdsrrcllc_return_current_substate() == TDSLLC_WAIT_CPHY_ACQ_CNF)||
     (tdsrrcllc_return_current_substate() == TDSLLC_WAIT_CPHY_FREQ_SCAN_CNF))
  {
    /* Abort L1 ACQ/FREQ_SCAN if it want to stop TDS and ACQ_REQ/FREQ_SCAN_REQ has been sent to TL1*/
    tdsl1m_rrc_abort_l1_curr_activity();
  }
  else if ((tdsrrccsp_get_csp_substate() == TDSRRCCSP_CELL_SEL_WT_ACQ) ||
           (tdsrrccsp_get_csp_substate() == TDSRRCCSP_CELL_SEL_WT_ACQ_PENDING) ||
           (tdsrrccsp_get_csp_substate() == TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF) ||
           (tdsrrccsp_get_csp_substate() == TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF_PENDING)
          )
  {
    /* If CSP has sent ACQ_REQ/FREQ_SCAN_REQ but queued in LLC, set pending flag;  LLC will send abot sig after sending ACQ_REQ/FREQ_SCAN_REQ to TL1*/
    tdsrrccsp_set_abort_curr_activity_pending_flag();
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Abort sig DO NOT sent to L1 and waiting LLC send ACQ_REQ/FREQ_SCAN_REQ to L1");  
  }

  return;
} /* end of tdsrrcmcm_send_stop_lower_layer_req */


/*===========================================================================

FUNCTION   RRCMCM_SEND_MODE_CHANGE_IND

DESCRIPTION

  This function sends a TDSRRC_MODE_CHANGE_IND to the other RRC procedures that
  have registerred for the notification. 
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_mode_change_ind(
  tdsrrc_proc_e_type procedure,
  tdsrrc_mode_e_type from_mode,
  tdsrrc_mode_e_type to_mode 
)
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_MODE_CHANGE_IND",0,0,0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_MODE_CHANGE_IND;

  /* Fill in procedure id that will receive this indication */
  cmd_ptr->cmd.mode_change_ind.procedure = procedure;
  cmd_ptr->cmd.mode_change_ind.from_mode = from_mode;
  cmd_ptr->cmd.mode_change_ind.to_mode = to_mode;

  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_MODE_CHANGE_IND sent to %d",procedure);

  return;
} /* end of tdsrrcmcm_send_mode_change_ind */


/*===========================================================================

FUNCTION   RRCMCM_SEND_CHANGE_MODE_CNF

DESCRIPTION

  This function sends a TDSRRC_CHANGE_MODE_CNF to the other RRC procedure that
  initiates the mode change. And also processes EST_REQ if saved in STANDBY
  mode.
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_change_mode_cnf(boolean status)
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_CHANGE_MODE_CNF",0,0,0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANGE_MODE_CNF;

  /* Fill in procedure id that will receive this indication */
  cmd_ptr->cmd.change_mode_cnf.procedure = tdsprocedure_id;
  cmd_ptr->cmd.change_mode_cnf.status = status;
  if(tdsrrcmcm_state == TDSRRCMCM_WAIT_STOP_CNF_FOR_MODE_CHANGE)
  {
    cmd_ptr->cmd.change_mode_cnf.mode = TDSRRC_MODE_CHANGE;
  }
  else
  {
    cmd_ptr->cmd.change_mode_cnf.mode = tdsrrc_mode;
  }

  cmd_ptr->cmd.change_mode_cnf.fail_type = tdsrrcmcm_wl1_failure_cause;

  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_CHANGE_MODE_CNF sent to %d",tdsprocedure_id);

  /*process EST_REQ if present.. doing this check here itself as this
    will not be present 99% of time.. this check is for a race condition*/
  if(tdsrrcmcm_est_req_saved_flag == TRUE)
  {
    tdsrrcmcm_process_est_req(tdsrrc_mode);
  }
  if((tdsrrc_mode == TDSRRC_MODE_STANDBY) 
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
        &&
  	/* T2L resel will do trm exchange */
  	(!((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TDS_TO_GSM_CELL_RESELECTION_SCAN)&&
  	 (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE))) &&
  	/* T2L redir will do trm exchange */
  	(!((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN)&&
  	 ((tdsrrc_csp_int_data.saved_rrc_cmd.cmd.redirect_req.choice == TDSRRC_INTER_SYSTEM_LTE)
#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
      || (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE)
#endif
     )))
#endif

     /* If FEATURE_TDSCDMA_DSDS_ENH is defined, we can't release lock for T2G irat case*/
    /* T2X HO will do trm exchange */
    &&(!tdsrrciho_is_iho_td2g_active()) &&
    /* T2G CCO will do trm exchange */
    (!tdsrrccho_is_td2g_cco_active())
    &&
    /* T2G resel will do trm exchange */
    (!((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TDS_TO_GSM_CELL_RESELECTION_SCAN)&&
     (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_GSM))) &&
    /* T2G redir will do trm exchange */
    (!((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN)&&
     (tdsrrc_csp_int_data.saved_rrc_cmd.cmd.redirect_req.choice == TDSRRC_INTER_SYSTEM_GSM))) &&
    /* including fake T2G redir during OOS search */
    (!((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)&&
     (!tdsrrc_csp_int_data.init_cell_sel_ind_during_wtointerrat))) &&
    /* including fake T2G redir during DCH->FACH/PCH cell selection */
    (!(((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN) ||
         (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN) ||
         (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN)) &&
     (!tdsrrc_csp_int_data.dch_to_fach_pch_tmr_expd))) &&
    /* including fake T2G redir during FACH->FACH/PCH cell selection */
    (!(((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN) ||
         (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN) ||
         (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN)) &&
     (!tdsrrc_csp_int_data.fach_to_fach_pch_tmr_expd)))
  	)
  {
    tdsrrcwrm_release_lock();
  }
  return;
} /* end of tdsrrcmcm_send_change_mode_cnf */


/*===========================================================================

FUNCTION   RRCMCM_SEND_IRAT_ABORT_IND

DESCRIPTION

  This function sends a TDSRRC_IRAT_ABORT_IND to the corresponsing
  Interrat procedure to abort the ongoing Interrat operation 
  inorder to service a higher priority request from MM
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_send_irat_abort_ind(void)
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_IRAT_ABORT_IND",0,0,0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_IRAT_ABORT_IND;

  /* Fill in procedure id that will receive this indication */
  cmd_ptr->cmd.rrc_irat_abort_ind.procedure = tdsprocedure_id;
  
  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_IRAT_ABORT_IND sent to %d",tdsprocedure_id);

  return;
} /* end of tdsrrcmcm_send_irat_abort_ind */

/*===========================================================================

FUNCTION   RRCMCM_UPDATE_MODE_AND_PREFERENCE

DESCRIPTION

  This function updates the current RRC mode amd the RAT preference based on
  the new mode and RAT preference information. 
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_update_mode(
  tdsrrc_mode_e_type new_mode
)
{
  int i;
  

  /* Check if the new mode is different from the current mode */
  if (tdsrrc_mode != new_mode) 
  {
    /* Check through the mode change notification table to send 
    TDSRRC_MODE_CHANGE_IND to the registerred procedures */
    for (i = 0; i < tdsrrcmcm_notification_counter; i++) 
    {
      /* If the registerred condition is met, then send the indication */
      if ( ((tdsrrcmcm_notifications[i].from_mode == tdsrrc_mode) ||
        (tdsrrcmcm_notifications[i].from_mode == TDSRRC_MODE_WILDCARD)) &&
           ((tdsrrcmcm_notifications[i].to_mode == new_mode)  ||
        (tdsrrcmcm_notifications[i].to_mode == TDSRRC_MODE_WILDCARD)))
      {
        tdsrrcmcm_send_mode_change_ind(tdsrrcmcm_notifications[i].procedure,
            tdsrrcmcm_notifications[i].from_mode,
            tdsrrcmcm_notifications[i].to_mode);
      }
    }    
  }
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDS MODE CHANGED(%d): %d => %d", 
            (tdsrrc_mode != new_mode),
            tdsrrc_mode, 
            new_mode);

  /* update the RRC Mode and RAT Preference */
  tdsrrc_mode = new_mode;

#ifdef FEATURE_TDSCDMA_RRC_REPORT_STATE_MODE_TO_MCS
  tdsrrc_send_rrc_mode_and_state_to_mcs();
#endif

  if(new_mode == TDSRRC_MODE_INACTIVE)
  {
    /* Release the lock only for non CSFB cases and lock is not acquired by exchange for X2T irat */
    /* Otherwise, lock needs to be exchanged hence no release here */
    if(!(tdsrrc_tdrm_exchange_success
#ifdef FEATURE_3GPP_CSFB_TDS
	 || (tdsrrc_csfb_call_status && ((tdsrrccsp_get_curr_scan() == RRC_CSP_GSM_TO_TDS_PSHO_SCAN)||(tdsrrccsp_get_curr_scan() == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)))
#endif
	))
    {
      tdsrrcwrm_release_lock();
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: CSFB call status is set or x2t irat failed with lock exchanged. Do not release the lock");
    }
  }
  return;
} /* end of tdsrrcmcm_update_mode */


/*===========================================================================

FUNCTION   PROCESS_CHANGE_MODE_REQ

DESCRIPTION

  This function processes the TDSRRC_CHANGE_MODE_REQ message received from the
  other procedures and initiates the corresponding actions. The mode it is going
  to change to can only be STANDBY, ACTIVE or INACTIVE.
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_process_change_mode_req(
  tdsrrc_change_mode_req_type change_mode_req
)
{

  boolean due_to_stop_mode = FALSE;
  mm_cmd_type *mm_cmd_ptr;  
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  uint32 reest_timer_length = 0;
  boolean reest_timer_used = FALSE;
  boolean is_abort = FALSE;



  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rvd TDSRRC_CHANGE_MODE_REQ from proc:%d",change_mode_req.procedure);
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Current RRC Mode:%d,Requested mode:%d",tdsrrc_mode,change_mode_req.mode);

  /* Save the destination procedure for confirmation */
  tdsprocedure_id = change_mode_req.procedure;

  /* Check what is the mode it is going to change to */
  switch (change_mode_req.mode)
  {
    /* Standby mode */
    case TDSRRC_MODE_STANDBY:
      /* Chech for the current RRC mode */
      switch(tdsrrc_mode)
      {
        /* Current RRC mode is in Active */
        case TDSRRC_MODE_ACTIVE:
          /* send the TDSL1_CPHY_SUSPEND_TDSCDMA_MODE_REQ to L1 */
          tdsrrcmcm_send_suspend_tds_mode_req(change_mode_req.reason);  

          /* Change the current mode to Standby */
          tdsrrcmcm_update_mode(TDSRRC_MODE_STANDBY);

          tdsrrcmcm_state = TDSRRCMCM_WAIT_SUSPEND_CNF;
          break;
        /* Current RRC Mode is Standby */
        case TDSRRC_MODE_STANDBY:
          /* Send Mode confirmation right away */ 
          tdsrrcmcm_send_change_mode_cnf(TRUE);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC already in Standby");
          break;
        /* Current RRC Mode is TDSRRC_MODE_INTERRAT_IN_PROGRESS */
        case TDSRRC_MODE_INTERRAT_IN_PROGRESS:
          /* Error Fatal bcos this scenario should never happen. WtoG trigger cannot
             happen when GtoW is in progress */
          ERR_FATAL("Invalid change mode req.Curr mode IRAT_IN_PROGRESS, Req mode STANDBY",0,0,0);
          break;
        /* Others */
        default:
          /* Invald mode change request */
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Mode change invald from %d to Standby",tdsrrc_mode);
          break;
      }
      break;

    /* Active mode */
    case TDSRRC_MODE_ACTIVE:
      /* Check for the current RRC mode */
      switch(tdsrrc_mode)
      {
        case TDSRRC_MODE_STANDBY:
            if((rrc_state == TDSRRC_STATE_CELL_DCH) || 
               (rrc_state == TDSRRC_STATE_CELL_FACH) || 
               (rrc_state == TDSRRC_STATE_CONNECTING) ||
               ((TDSRRC_STATE_DISCONNECTED == rrc_state) && 
                ((TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF == tdsrrcrce_get_rce_sub_state())||
                 (TDSRRCRCE_WAIT_FOR_SIB7_CURRENT == tdsrrcrce_get_rce_sub_state())))
              )
            {
              uint32 ta_duration;
              uecomdef_status_e_type status;
              if((tdsrrccsp_get_curr_scan() == TDSRRC_CSP_UNLOCK_BY_SCAN) && (TDSRRC_STATE_DISCONNECTED != rrc_state))
              {
#ifdef FEATURE_SGLTE
                uint32 lock_wait_time_in_ms;
#endif
                uint32 stop_time_of_lta;
                uint32 start_time_of_lta = tdsrrccsp_get_system_time();					
                /*Send the tune away start event for QXDM logging*/
                event_report(EVENT_WCDMA_RRC_DSIM_TUNEAWAY_START);
                /* 16000ms is the original design. currently only in DCH state consider the T313+reestablish timer. */
                if(rrc_state == TDSRRC_STATE_CELL_DCH)
                {
                  /*If the LTA interval > max {16sec, 160ms+(N313-1)*10ms+T313+min(T315, (N302+1)*T302)}:  
                      Enter to idle directly. 
                    Else (Legacy)
                      Try to in sync*/
                  reest_timer_length = MAX(LTA_REESTABLISH_TIME,(OUT_OF_SYNC_TIME_THRESHOLD+(tdsrrcllc_translate_n313(TDSRRC_GET_N313())-1)*10)+
                                        (TDSRRC_GET_T313())*1000+MIN(tdsrrccu_get_reestablish_timer(),((TDSRRC_GET_N302())+1)*tdsrrccu_convert_t302_to_ms(TDSRRC_GET_T302())));
                  
                  tdsrrc_reestablish_timer_out = FALSE;
                  reest_timer_used = TRUE;
                }
                else 
                {
                  reest_timer_length = LTA_REESTABLISH_TIME;
                }
                tdsrrctmr_start_timer(TDSRRCTMR_GET_TA_DURATION_TIMER, reest_timer_length );
#ifdef FEATURE_SGLTE
                
                if(tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_SGLTE)
                {
                  if (rrc_state == TDSRRC_STATE_CELL_DCH)
                  {
                    lock_wait_time_in_ms = (uint32)tdscurrent_config_ptr->l1_dl_chan_parms.sync_det.T313*1000 +
                                           (uint32)tdscurrent_config_ptr->l1_dl_chan_parms.sync_det.N313*5;
                    TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSIM_SGLTE: in dch start lock timer for t313[%d]*1000+5ms[subframe]*n313[%d] = %dms before rpt no resource to nas",
                                tdscurrent_config_ptr->l1_dl_chan_parms.sync_det.T313, 
                                tdscurrent_config_ptr->l1_dl_chan_parms.sync_det.N313, lock_wait_time_in_ms);
                    tdsrrctmr_start_timer(TDSRRCTMR_WRM_LOCK_TIMER, lock_wait_time_in_ms);
                    tdsrrc_csp_int_data.wrm_lock_tmr_started = TRUE;
                  }
                  else if (rrc_state == TDSRRC_STATE_CELL_FACH)
                  {
                    lock_wait_time_in_ms = 4000;
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_SGLTE: in fach start lock timer for 4000ms before rpt no resource to nas");
                    tdsrrctmr_start_timer(TDSRRCTMR_WRM_LOCK_TIMER, lock_wait_time_in_ms);
                    tdsrrc_csp_int_data.wrm_lock_tmr_started = TRUE;
                  }
                }
#endif
                tdsrrc_reset_oos_due_to_no_resource_flag();
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA 
                tdsrrc_send_rlc_ta_status(TDSRLC_LTA_START_IND, TDSRLC_TA_SUCC_MAX_ENUM);
#endif                
                status = tdsrrcwrm_block_wait_to_acquire_lock_for_time(150, TRM_BG_TRAFFIC, &is_abort);
#ifdef FEATURE_SGLTE
                tdsrrctmr_stop_timer(TDSRRCTMR_WRM_LOCK_TIMER);
                tdsrrc_csp_int_data.wrm_lock_tmr_started = FALSE;
#endif
                if(status == FAILURE)
                {
                  /* Send a change mode cnf with failure */
                  tdsrrcmcm_wl1_failure_cause = TDSL1_FAILED_NO_TRM_LOCK;
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA                   
                  tdsrrc_send_rlc_ta_status(TDSRLC_LTA_STOP_IND, TDSRLC_FAIL_IND);
#endif                  
                  if(tdsrrc_get_oos_due_to_no_resource_flag_status()==FALSE)
                  {
                    tdsrrc_csp_int_data.ue_in_conn_oos = TRUE;
                    tdsrrc_reestablish_timer_out = FALSE;/*RRC will go to idle due to abort_req */					
					tdsrrcmcm_send_resume_tds_mode_req(change_mode_req.reason);
					tdsrrcmcm_state = TDSRRCMCM_WAIT_RESUME_CNF;
                    return;
                  }
#ifdef FEATURE_SGLTE
                  else
                  {
                    if (rrc_state == TDSRRC_STATE_CELL_DCH)
                    {
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_SGLTE: lock timer for t313+0.005[subframe]*n313 s expired in dch,try to resume L1");
                    }
                    else if (rrc_state == TDSRRC_STATE_CELL_FACH)
                    {
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_SGLTE: lock timer for 4 s expired in fach,try to resume L1");
                    }
					if (((rrc_state == TDSRRC_STATE_CELL_FACH)|| (rrc_state == TDSRRC_STATE_CELL_DCH) || (rrc_state == TDSRRC_STATE_CONNECTING))&& (is_abort == TRUE))
                    {
					 /* Send the TDSL1_CPHY_RESUME_TDSCDMA_MODE_REQ to L1 with reason "TDSINTERRAT_TX_NEEDED"
                                      * which will indicate TL1 to request RxTx lock for resume TDS for RRC cannot get Tx lock */
                      tdsrrcmcm_send_resume_tds_mode_req(TDSINTERRAT_TX_NEEDED);
                      tdsrrcmcm_state = TDSRRCMCM_WAIT_RESUME_CNF;
                      return;
                    }
                  }
#endif
                }

                /* Get the TA duration and pass it to L1 for optimizing data call resume time */
                ta_duration = reest_timer_length - ((uint32)tdsrrctmr_get_remaining_time(TDSRRCTMR_GET_TA_DURATION_TIMER));
                /* Timer expired . Length is T313+T314/T315 . RRC no need send the CU. Set this flag let CSP know.*/
                if ((tdsrrctmr_get_remaining_time(TDSRRCTMR_GET_TA_DURATION_TIMER) == 0)&&(reest_timer_used))
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM:PS reestablish timer out");
                  tdsrrc_reestablish_timer_out = TRUE;
                }
                tdsl1dsds_fill_tuneaway_duration(ta_duration/10);
                tdsrrctmr_stop_timer(TDSRRCTMR_GET_TA_DURATION_TIMER);

                stop_time_of_lta = tdsrrccsp_get_system_time();
                /*Invalid SIB DB if LTA duration is greater than 5 min*/
                if(stop_time_of_lta < start_time_of_lta)
                {
                  if((( LTA_MAX_TIME + 1 + stop_time_of_lta - start_time_of_lta)&LTA_MAX_TIME)
                      > LTA_DURATION_TO_INVALID_SIB_DB)
                  {
                    tdsrrcsibdb_invalid_sibdb_for_quick_cell_resel();
                  }
                }
                else
                {
                  if((stop_time_of_lta - start_time_of_lta)> LTA_DURATION_TO_INVALID_SIB_DB)
                  {
                    tdsrrcsibdb_invalid_sibdb_for_quick_cell_resel();
                  }
                }
                /*Send the tune away stop event for QXDM logging*/
                event_report(EVENT_WCDMA_RRC_DSIM_TUNEAWAY_STOP);
                
              }
              else

              if(tdsrrcwrm_acquire_lock_for_resuming_on_tds() == FAILURE)
              {
                /* Updating the RRC mode to ACTIVE because CHANGE_MODE_CNF will
                 * be handled in CSP only if RRC is active. Further actions as to what
                 * to be done will be taken care by CSP */
                //tdsrrcmcm_wl1_failure_cause = TDSL1_FAILED_NO_TRM_LOCK;
                //tdsrrcmcm_update_mode(TDSRRC_MODE_ACTIVE);
				/* If saved EST_REQ exist, send ABORT_IND to MM;  Or else it will be handle at tdsrrcmcm_send_change_mode_cnf()
				 * But TL1 is still under suspend state then;
				 */
                if(tdsrrcmcm_est_req_saved_flag == TRUE)
                {
                  tdsrrcmcm_est_req_saved_flag = FALSE;
            
                  /* Allocate memory for RRC_ABORT_IND command to MM.. */
                  if( (mm_cmd_ptr = mm_tdsrrc_get_cmd_buf(RRC_ABORT_IND)) != NULL)
                  {
                    /* Fill in parameters for RRC_ABORT_IND command */
                    mm_cmd_ptr->cmd.rrc_abort_ind.abort_cause = RRC_INTERNAL_ERROR;
              
                    /* Update the Max HFNs values to be stored in SIM or USIM */ 
                    (void) tdsrrcsmc_append_hfns_to_rel_ind(mm_cmd_ptr->cmd.rrc_abort_ind.hfn_cs,
                    mm_cmd_ptr->cmd.rrc_abort_cnf.hfn_ps);
              
#ifdef FEATURE_DUAL_SIM
                    mm_cmd_ptr->cmd.rrc_abort_ind.as_id = tdsrrc_get_as_id();
#endif
                    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Sent RRC_ABORT_IND to MM with cause: %d",RRC_CONNECTION_NOT_AVAILABLE);
              
                    /* Sends the command to MM */
                    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
                    {
                      tdscfa_mm_put_cmd(mm_cmd_ptr);
                    }
                    mm_put_cmd(mm_cmd_ptr);
                  }
                  else   /* RRC Task failed to allocate memory */
                  {
                    ERR_FATAL("Unable to allocate memory for cmd: %d",RRC_ABORT_IND,0,0);
                  } 
                }
                else
                {
                  /* Force init RCE if resume TDS fail; Or else, RCE may hang at connecting state
                   * due to CSP not start cell selection prceodure and TL1 always in suspend state
                   */
                  tdsrrcrce_force_initialize(TRUE);
                }
                /* Send the TDSL1_CPHY_RESUME_TDSCDMA_MODE_REQ to L1 with reason "TDSINTERRAT_TX_NEEDED"
                 * which will indicate TL1 to request RxTx lock for resume TDS for RRC cannot get Tx lock
                 */
                tdsrrcmcm_send_resume_tds_mode_req(TDSINTERRAT_TX_NEEDED);
                tdsrrcmcm_state = TDSRRCMCM_WAIT_RESUME_CNF;
                return;
              }
            }
          /* Send the TDSL1_CPHY_RESUME_TDSCDMA_MODE_REQ to L1 */
          tdsrrcmcm_send_resume_tds_mode_req(change_mode_req.reason);
          tdsrrcmcm_state = TDSRRCMCM_WAIT_RESUME_CNF;
          break;
        case TDSRRC_MODE_ACTIVE:
          /* Send Mode confirmation right away */
          tdsrrcmcm_send_change_mode_cnf(TRUE);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC already in Active");
          break;   
        default:
          /* Invald mode change request */
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Mode change invald from %d to Active",tdsrrc_mode);
          break;
      }           
      break;

    /* Interrat in Progress mode */
    case TDSRRC_MODE_INTERRAT_IN_PROGRESS:
      /* Check for the current RRC mode */
      if (tdsrrc_mode == TDSRRC_MODE_INACTIVE) 
      {
        tdsrrc_init_dynamic_ptrs();
        /*Update the substate and mode before posting the req to L1 as L1 will start processing before we updating the substate*/
        tdsrrcmcm_update_mode(TDSRRC_MODE_INTERRAT_IN_PROGRESS);
        tdsrrcmcm_state = TDSRRCMCM_WAIT_START_CNF;
        /* Start TDS mode */
        tdsrrcmcm_send_start_tdscdma_req();
      }
      else
      {
        /* Invald mode change request */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Mode change invald from %d to INP",tdsrrc_mode);
      }
      break;

    /* Inactive mode */
    case TDSRRC_MODE_INACTIVE:

      /* In general we do not process a mode change to Inactive if the current 
         mode is Standby.But in our Spec compliant WTOG Cell Change Order implementation,
         for the successful case,to stop TDS,CHO proc is going to send a 
         Change_mode_request to Inactive instead of MM sending a Stop_Wcdma_Req.
         RRC Mode would be standby in this scenario.Hence make an exception and handle
         this request if it is from CHO */
      if((change_mode_req.procedure == TDSRRC_PROCEDURE_CHO) && (tdsrrc_mode == TDSRRC_MODE_STANDBY))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Honoring Mode Change Req to Inactive from Standby for proc CHO"); 
        due_to_stop_mode = TRUE;
        tdsstop_cause = RRC_RAT_CHANGE;

        /* send stop tds request */
        tdsrrcmcm_send_stop_lower_layer_req(due_to_stop_mode);
        tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF;
      }
      /* Check the current RRC mode */
      else if (tdsrrc_mode == TDSRRC_MODE_INTERRAT_IN_PROGRESS) 
      {
        due_to_stop_mode = TRUE;
        /* send stop tds request */
        tdsrrcmcm_send_stop_lower_layer_req(due_to_stop_mode);
        tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF;
      }
      else if((change_mode_req.procedure == TDSRRC_PROCEDURE_CSP) && (tdsrrc_mode == TDSRRC_MODE_ACTIVE))
      {
        tdsstop_cause = RRC_RAT_CHANGE;
        due_to_stop_mode = TRUE;
        /* send stop tds request */
        tdsrrcmcm_send_stop_lower_layer_req(due_to_stop_mode);
        tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF;
      }
      else
      {
        /* Invald mode change request */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Mode change invald from %d to Inactive",tdsrrc_mode);
      }      
      break;

    case TDSRRC_MODE_CHANGE:
      if(tdsrrc_mode == TDSRRC_MODE_INACTIVE)
      {
        tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF_FOR_MODE_CHANGE;
        tdsrrcmcm_send_change_mode_cnf(TRUE);
        tdsrrcmcm_send_mode_change_ind(TDSRRC_PROCEDURE_IHO, 
                                    TDSRRC_MODE_CHANGE,
                                    TDSRRC_MODE_INACTIVE);
        tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);
        tdsrrcmcm_init_data(TDSINIT_ALL);
      }
      else
      {
        /* send stop tds request */
        /* For a mode change set the cause to be sent to L1 as DEEP SLEEP */
        tdsstop_cause = RRC_DEEP_SLEEP;
        due_to_stop_mode = FALSE;
        tdsrrcmcm_send_stop_lower_layer_req(due_to_stop_mode);
        tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF_FOR_MODE_CHANGE;
      }
      break;
    default:
      /* invalid the RRC mode to change to */
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid the RRC Mode Transition: %d",change_mode_req.mode);
      break;
  }
  return;
} /* end of tdsrrcmcm_process_change_mode_req */


#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
/*===========================================================================

FUNCTION   tdsrrcmcm_send_lte_interrat_plmn_rsp_with_failure

DESCRIPTION

  This function sends interrat PLMN search RSP as failure

DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_lte_interrat_plmn_rsp_with_failure(void)
{
  lte_irat_plmn_srch_rsp_s plmn_srch_rsp;

  memset(&plmn_srch_rsp,0,sizeof(lte_irat_plmn_srch_rsp_s));

  plmn_srch_rsp.available_plmn_list.length = 0;
  plmn_srch_rsp.srch_done = FALSE;
  plmn_srch_rsp.hplmn_found = FALSE;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Send TDSCDMA_RRC_LTE_PLMN_SRCH_RSP to LTE RRC");
  msgr_init_hdr(&plmn_srch_rsp.msg_hdr,MSGR_TDSCDMA_RRC,TDSCDMA_RRC_LTE_PLMN_SRCH_RSP);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  msgr_set_hdr_variant(&plmn_srch_rsp.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
  msgr_set_hdr_inst(&plmn_srch_rsp.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
  msgr_send(&plmn_srch_rsp.msg_hdr, sizeof(lte_irat_plmn_srch_rsp_s));

  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_send_message_to_lte_rrc(&plmn_srch_rsp.msg_hdr, sizeof(lte_irat_plmn_srch_rsp_s));
  }
}
#endif

/*===========================================================================

FUNCTION   tdsrrcmcm_send_interrat_plmn_cnf_with_failure

DESCRIPTION

  This function sends interrat PLMN search CNF as failure

DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_send_interrat_plmn_cnf_with_failure(void)
{
  tdsrrc_interrat_plmn_srch_cnf_type interrat_plmn_srch_cnf;

  memset(&interrat_plmn_srch_cnf,0,sizeof(tdsrrc_interrat_plmn_srch_cnf_type));
 
  interrat_plmn_srch_cnf.available_plmn_list.length = 0;
  interrat_plmn_srch_cnf.srch_done = FALSE;
  interrat_plmn_srch_cnf.hplmn_found = FALSE;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Send TDSRRC_INTERRAT_PLMN_SRCH_CNF");
  tdsrr_send_rrc_message_to_rr_internal((void *)(&interrat_plmn_srch_cnf), TDSRRC_INTERRAT_PLMN_SRCH_CNF,
                                    sizeof(tdsrrc_interrat_plmn_srch_cnf_type));
}
/*===========================================================================

FUNCTION   tdsrrcmcm_handle_start_tds_failure_for_dual_sim

DESCRIPTION

  This function handles start tds failure due to NO LOCK from L1.

DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_handle_start_tds_failure_for_dual_sim(void)
{

  boolean free_dynamic_ptrs = TRUE;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: L1 unable to get the lock for START_TDS_MODE_CNF");
  /*Check if abort flags are set to TRUE,
     >if Abort flag is set to TRUE, no need to send another STOP_TDS_REQ to L1, just behave as if STOP_CNF from L1 is received
     >If No Abort Flags, check the cmd flags for mode_change_req, stop_wcdma_mode_req
     >>send the start tds req again to WL1
  */
  /*TBD: Check for ABORT Flags needed here, this is for interRAT Hold on*/
  if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_MODE_CHANGE)
  {
    tdsprocedure_id = TDSRRC_PROCEDURE_CSP;
    tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF_FOR_MODE_CHANGE;
    tdsrrcmcm_send_change_mode_cnf(TRUE);
    tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);
    tdsrrcmcm_init_data(TDSINIT_ALL);
  }
  else if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == RRCMCM_CMD_STOP_TDS_REQ)
  {
    tdsrrcmcm_send_stop_cnf_to_mm_flag = TRUE;
    /* Clear RRC change mode requests,if any */
    tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = TDSRRC_MODE_MAX;
  
    /*TBD: check if mode_change_ind needs to be sent or not*/
    tdsrrcmcm_send_mode_change_ind(TDSRRC_PROCEDURE_CSP, tdsrrc_mode, 
                                TDSRRC_MODE_INACTIVE);
    tdsrrcmcm_send_mode_change_ind(TDSRRC_PROCEDURE_CCM, tdsrrc_mode,
                                TDSRRC_MODE_INACTIVE);
    tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);
    /* Send a Stop TDS Mode Confirmation to MM */
    tdsrrcmcm_send_stop_tds_cnf(TRUE);
    tdsrrcmcm_init_data(TDSINIT_ALL);
  }
  else if((tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_RR_INTERRAT_PLMN_ABORT_REQ)
           ||(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_RR_INTERRAT_RESELECTION_ABORT_REQ)
           ||(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_RR_INTERRAT_REDIRECT_ABORT_REQ)
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
           ||(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_PLMN_SRCH_REQ)
           ||(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_REDIR_REQ)
           ||(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_RESEL_REQ)
           ||(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_CGI_REQ)
#endif
         )
  {
    if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_RR_INTERRAT_PLMN_ABORT_REQ)
    {
      tdsrrccsp_send_bplmn_abort_cnf_to_rr();
    }
    else if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_RR_INTERRAT_RESELECTION_ABORT_REQ)
    {
      tdsrrccsp_send_interrat_reselection_abort_cnf();
    }
    else if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_RR_INTERRAT_REDIRECT_ABORT_REQ)
    {
      tdsrrccsp_send_g2td_redirection_abort_cnf();
    }
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
    else if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_PLMN_SRCH_REQ)
    {
      tdsrrccsp_send_lte_interrat_plmn_srch_abort_rsp();
    }
    else if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_REDIR_REQ)
    {
      tdsrrccsp_send_lte_interrat_redirection_abort_rsp();
    }
    else if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_RESEL_REQ)
    {
      tdsrrccsp_send_lte_interrat_reselection_abort_rsp();
    }
    else if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_CGI_REQ)
    {
      tdsrrccsp_send_lte_interrat_cgi_abort_rsp();
    }
#endif

    tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);
    /*TBD: Need to take care of SERVICE_REQ or LIST_REQ if pending*/
    tdsrrcmcm_init_data(TDSINIT_ALL);
  }
  /* If L1 fails to get TRM lock for start TDS in case of inter-RAT,
     send failure to RR immediately. 
  */
  else if(tdsrrc_mode == TDSRRC_MODE_INTERRAT_IN_PROGRESS)
  {
    if(tdsrrcmcm_cmd_store.rrcmcm_interrat_reselection_req_cmd.cmd_hdr.cmd_id== TDSRRC_INTERRAT_RESELECTION_REQ)
    {
      /* Send the Reselection Rej to GSM RR */
      tdsrrccsp_send_interrat_reselection_reject();
    }
    else if(tdsrrcmcm_cmd_store.rrcmcm_interrat_reselection_req_cmd.cmd_hdr.cmd_id == TDSRRC_INTERRAT_REDIRECT_REQ)
    {
      /* Send the Redirection Rej to GSM RR */
      tdsrrccsp_send_g2td_redirection_reject();
    }
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
    else if(tdsrrcmcm_cmd_store.rrcmcm_lte_irat_cmd.cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == TDSCDMA_RRC_LTE_RESEL_REQ)
    {
      tdsrrccsp_send_lte_interrat_reselection_failed_rsp();
    }
    else if(tdsrrcmcm_cmd_store.rrcmcm_lte_irat_cmd.cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == TDSCDMA_RRC_LTE_REDIR_REQ)
    {
      tdsrrccsp_send_lte_interrat_redirection_failed_rsp();
    }
    else if(tdsrrcmcm_cmd_store.rrcmcm_lte_irat_cmd.cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == TDSCDMA_RRC_LTE_GET_CGI_REQ)
    {
      tdsrrccsp_send_lte_interrat_cgi_rsp();
    }
#endif
    tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);
    tdsrrcmcm_init_data(TDSINIT_ALL);
  }
  else if(tdsrrcmcm_cmd_store.rrcmcm_interrat_reselection_req_cmd.cmd_hdr.cmd_id == TDSRRC_INTERRAT_PLMN_SRCH_REQ)
  {
    tdsrrcmcm_send_interrat_plmn_cnf_with_failure();
    tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);
    tdsrrcmcm_init_data(TDSINIT_ALL);
  }
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
  else if(tdsrrcmcm_cmd_store.rrcmcm_lte_irat_cmd.cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == TDSCDMA_RRC_LTE_PLMN_SRCH_REQ)
  {
    tdsrrcmcm_send_lte_interrat_plmn_rsp_with_failure();
    tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);
    tdsrrcmcm_init_data(TDSINIT_ALL);
  }
#endif
  else
  {
    /* Send a Start TDS mode request to L1 */
    tdsrrcmcm_send_start_tdscdma_req();
    tdsrrcmcm_state = TDSRRCMCM_WAIT_START_CNF;
    free_dynamic_ptrs = FALSE;
  }

  if(free_dynamic_ptrs
#ifdef FEATURE_WRLF_SYSTEM_SEL
     && (!TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
#endif
    )
    {
    tdsrrc_free_dynamic_ptrs();
    }
}


/*===========================================================================

FUNCTION   RRCMCM_PROCESS_MODE_CHANGE_REQ

DESCRIPTION

  This function processes the MODE_CHANGE_REQ message received from CSP
  procedure.  This will stop the TDS RAT and take it to FTM/GAMING
  mode.  
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_process_mode_change_req
(
  void
)
{
  tdsrrc_change_mode_req_type change_mode_req;
  
  /*CSP handles this primitive from MM.. The mode to take RRC to is
    TDSRRC_MODE_CHANGE.. set this in change_mode_req type and call the
    tdsrrcmcm_process_change_mode_req*/
  change_mode_req.procedure = TDSRRC_PROCEDURE_CSP;
  change_mode_req.mode = TDSRRC_MODE_CHANGE;
  /* For a mode change set the cause to be sent to L1 as DEEP SLEEP */
  tdsstop_cause = RRC_DEEP_SLEEP;

  tdsrrcmcm_process_change_mode_req(change_mode_req);
}

/*===========================================================================

FUNCTION   RRCMCM_INITIAL_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the MCM is in the TDSRRCMCM_INITIAL substate. This function
  looks at the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_initial_substate_event_handler(
  tdsrrc_cmd_type *cmd_ptr /* Received command                   */
)
{
  boolean due_to_stop_mode = FALSE;
  tdsrrc_state_e_type curr_rrc_state; 
  tdsrrc_out_of_lock_wait_e_type wrm_lock_status;
  boolean irat_ex_success; 
  boolean irat_tx_ex_success;
  tdsrrc_csp_band_class_type freq_band;
  #ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
  tds_rrc_lte_get_cgi_req_type *lte_cgi_req = NULL; 
  lte_irat_plmn_srch_req_s *plmn_list_req = NULL;
  tds_rrc_lte_redir_req_type *lte_redir_req = NULL;
  #endif

  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
/*lint -e408*/
/*lint -e740*/
   case TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ:
     switch(tdsrrc_mode)
     {
      case TDSRRC_MODE_INACTIVE:
      
        /* TDS is not up and CSP is not processing a PLMN search currently.Inform CSP to reset 
         all the internal databases related to search and send an abort confirm to RR  */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:Received TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ in Initial state/Inactive mode"); 
        tdsrrccsp_send_bplmn_abort_cnf_to_rr();
        break;
      
       case TDSRRC_MODE_INTERRAT_IN_PROGRESS:
    
         /* Forward the request to CSP because CSP is already processing a BPLMN search request  */
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:Received TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ in Initial state/Interrat-in-progress mode ");  
         tdsrrccsp_cell_selection_procedure( cmd_ptr );                
         break;
      
       default:

         /* This should never happen */
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:Received TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ in an Initial state/Invalid mode");  
         break;
     }
     break;

    case TDSRRC_INTERRAT_PLMN_SRCH_REQ:
      switch(tdsrrc_mode)
      {
        case TDSRRC_MODE_INACTIVE:
    
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:Received TDSRRC_INTERRAT_PLMN_SRCH_REQ in Initial state/Inactive mode");
      
          /* Process the request */
          tdsrrcmcm_cmd_store.rrcmcm_interrat_reselection_req_cmd = *((tdsrrc_rr_cmd_type *)cmd_ptr);     // Store the command
          tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_INTERRAT_PLMN_SRCH_REQ ;                    // Set the cmd flag       
          tdsrrc_init_dynamic_ptrs();
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
          tdsrrc_proc_type = SYS_PROC_TYPE_BPLMN_SEARCH;
#endif
          /*Update the substate and mode before posting the req to L1 as L1 will start processing before we updating the substate*/
          tdsrrcmcm_state = TDSRRCMCM_WAIT_START_CNF;                                                     // Set substate to TDSRRCMCM_WAIT_START_CNF
          tdsrrcmcm_send_start_tdscdma_req();
          break;

        default:

          /* We shouldnt be getting this request in any other mode other than Inactive 
          Do we send a cnf back with failure ? */  
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:Received TDSRRC_INTERRAT_PLMN_SRCH_REQ in an Initial state/Invalid mode");  
          break;  
      }
    break;
/*lint +e408*/
/*lint +e740*/

    case TDSRRC_MODE_CHANGE_REQ:
      /* Check the current RRC mode */
      switch(tdsrrc_mode)
      {
        case TDSRRC_MODE_STANDBY:
          /* Store the cmd */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_MODE_CHANGE_REQ in Standby Mode");
          (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type));
          tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_MODE_CHANGE_REQ;

          /* Abort the ongoing interrat procedure to process the service req */
          tdsrrcmcm_send_irat_abort_ind(); 
          break; 

        default:
          /* In all other cases pass the cmd to CSP */
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;
      }
      break;

    case TDSRRC_DEACT_REQ:
      /* Check the current RRC mode */
      if(tdsrrc_mode == TDSRRC_MODE_STANDBY)
      {
        /* Store the cmd - not really needed */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_DEACT_REQ in Standby Mode");
        (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type));
        tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_DEACT_REQ;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM is in standby ... Handling deact request");
      }
      tdsrrccsp_cell_selection_procedure(cmd_ptr );
      break; 

    case TDSRRC_SERVICE_REQ:
#ifdef FEATURE_TDSCDMA_SUPPORT_LTE_DEPRIORITIZATION
	  if ( (cmd_ptr->cmd.rrc_service_req.requested_plmn.plmn_type == RRC_GSM_MAP_SPECIFIC_PLMN_REQ) &&
	  	   (!tdsrrccsp_is_dummy_plmn_id(cmd_ptr->cmd.rrc_service_req.requested_plmn.plmn_id)) )
      {
        /* inform LRRC on NAS selected PLMN in case LTE deprioritization is active */
        tdsrrcmeas_send_lte_depri_requested_plmn_ind(cmd_ptr->cmd.rrc_service_req.requested_plmn.plmn_id);
      }
#endif

      /* Check current RRC mode */
      switch(tdsrrc_mode)
      {       
        /* RRC is in Active mode */
        case TDSRRC_MODE_ACTIVE:
#ifdef FEATURE_RAT_PRIORITY_LIST
          tdsrrcmcm_update_mode_capability_from_service_req(&cmd_ptr->cmd.rrc_service_req.rat_pri_list_info);
#else
          /* Set the dual-mode enable flag */
          tdsrrc_dual_mode_enabled = cmd_ptr->cmd.rrc_service_req.dual_mode;
#endif

          /* Invoke the initial cell selection */
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

          /* RRC is in Inactive Mode */
        case TDSRRC_MODE_INACTIVE:
#ifdef FEATURE_WRLF_SYSTEM_SEL
          if(!TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
#endif
          {
            tdsrrc_init_dynamic_ptrs();
          }
          
          /* Start TDS mode, store the TDSRRC_SERVICE_REQ command */
          (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type));
          /*Update the substate and mode before posting the req to L1 as L1 will start processing before we updating the substate*/
          tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_SERVICE_REQ;
          tdsrrcmcm_state = TDSRRCMCM_WAIT_START_CNF;
          /* Send a Start TDS mode request to L1 */
          tdsrrcmcm_send_start_tdscdma_req();
          break;
        
        case TDSRRC_MODE_STANDBY:
          /* Store the cmd */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_SERVICE_REQ in Standby Mode");
          (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type));
          tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_SERVICE_REQ;
             
          /* Abort the ongoing interrat procedure to process the service req */
          tdsrrcmcm_send_irat_abort_ind(); 
          break; 
 
        case TDSRRC_MODE_INTERRAT_IN_PROGRESS:        
          /*This can happen when a G->W reselection is in progress and the user has
           *changed mode from Auto->W only.We would not get an activation_rsp 
           *if we succeed.Hence treat this service req as an implicit activation_rsp
           */
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_SERVICE_REQ in Interrat-in-progress Mode,updating RRC Mode to Active");
#ifdef FEATURE_RAT_PRIORITY_LIST
           tdsrrcmcm_update_mode_capability_from_service_req(&cmd_ptr->cmd.rrc_service_req.rat_pri_list_info);
#else
           tdsrrc_dual_mode_enabled = cmd_ptr->cmd.rrc_service_req.dual_mode;
#endif
           tdsrrcmcm_update_mode(TDSRRC_MODE_ACTIVE);

           /* Forward the service request to CSP */
           tdsrrccsp_cell_selection_procedure(cmd_ptr);

           /* Init data */
           tdsrrcmcm_init_data(TDSINIT_ALL);
           break;

        /* Others */
        default:
          /* Reject TDSRRC_SERVICE_REQ during inter-RAT operation */
          tdsrrcmcm_send_service_cnf_with_failure();
          break;
      }
      break;    

    /* Mode change indication from MM */
    case TDSRRC_CHANGE_MODE_IND:
      {
        sys_band_mask_type tds_band_pref = 0;
        sys_band_mask_type gw_band_pref = 0;
        boolean prev_dual_mode_status = tdsrrc_dual_mode_enabled;
        boolean mode_status_changed = FALSE;

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
        boolean prev_lte_mode_status = tdsrrc_lte_mode_enabled;
#endif
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_CHANGE_MODE_IND is received from MM");


#ifdef FEATURE_RAT_PRIORITY_LIST
        tds_band_pref = tdsrrccsp_extract_tds_band_pref_from_rat_pri_list(
                                   &cmd_ptr->cmd.rrc_change_mode_ind.rat_pri_list_info);
        gw_band_pref = tdsrrccsp_extract_gw_band_pref_from_rat_pri_list(
                                   &cmd_ptr->cmd.rrc_change_mode_ind.rat_pri_list_info);
        tdsrrcmcm_update_mode_capability(&cmd_ptr->cmd.rrc_change_mode_ind.rat_pri_list_info);
        if(!prev_dual_mode_status && tdsrrc_dual_mode_enabled)  
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"GSM mode enabled gsm_mode %d",tdsrrc_dual_mode_enabled);
          mode_status_changed = TRUE;
        }
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
        if(prev_lte_mode_status != tdsrrc_lte_mode_enabled)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"LTE_IRAT: new lte_mode %d,prev lte_mode %d",tdsrrc_lte_mode_enabled,prev_lte_mode_status);
          mode_status_changed = TRUE;
        }
#endif

#else
        /* ZZTBD: FEATURE_RAT_PRIORITY_LIST must be defined.
           We no longer handle TDS and GERAN in a single bit mask set as below.
         */
        gw_band_pref = cmd_ptr->cmd.rrc_change_mode_ind.band_pref;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DUAL: Updated the DUAL mode flag"); 
        tdsrrc_dual_mode_enabled = cmd_ptr->cmd.rrc_change_mode_ind.dual_mode;
        if(!prev_dual_mode_status && tdsrrc_dual_mode_enabled)
        {
          mode_status_changed = TRUE;
        }
#endif

        if (TRUE == tdsrrccsp_check_if_3g_band_pref_changed(tds_band_pref))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"DUAL:Band preference is not different from what stored in CSP");
        }
        if (mode_status_changed) 
        {
          /* Set the dual-mode enable flag */
          tdsrr_set_band_pref_internal(gw_band_pref);

          /* Update TDS band */
          tdsrrc_update_band_preference(tds_band_pref);
          /* Update GW band */
          tdsrrc_csp_int_data.band_pref_gw = gw_band_pref;
#ifdef FEATURE_RAT_PRIORITY_LIST
          tdsrrccsp_update_rat_pri_list_info(&cmd_ptr->cmd.rrc_change_mode_ind.rat_pri_list_info);
#endif
          /* Update GSM with UE's band preference */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"DUAL: Updated rr and rrc band preferences");
          tdsrrcmcm_send_dual_mode_info_to_meas_ueci(cmd_ptr);
        }
      }

      break;

    /* RRC PLMN List request */
    case TDSRRC_PLMN_LIST_REQ:
#ifdef FEATURE_TDSCDMA_SCAN_SCOPE
	  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rx:TDSRRC_PLMN_LIST_REQ with Scan Scope %d",cmd_ptr->cmd.plmn_list_req.scan_scope);
	  tdsrrc_csp_int_data.bplmn_scan_scope = cmd_ptr->cmd.plmn_list_req.scan_scope;
#endif
      /* Check current RRC mode */
      switch(tdsrrc_mode)
      {
        /* RRC is in Active Mode */
        case TDSRRC_MODE_ACTIVE:
          /* Forward the command to CSP for processing */
          tdsrrccsp_cell_selection_procedure(cmd_ptr);
          break;

        /* RRC is in Inactive mode */
        case TDSRRC_MODE_INACTIVE:
          /* Store the command and initiate TDS activation procedure */
          (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type));
          tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_PLMN_LIST_REQ;

          tdsrrc_init_dynamic_ptrs();
          /*Update the substate and mode before posting the req to L1 as L1 will start processing before we updating the substate*/
          tdsrrcmcm_state = TDSRRCMCM_WAIT_START_CNF;
          /* Send a Start TDS mode request to L1 */
          tdsrrcmcm_send_start_tdscdma_req();
          break;

        case TDSRRC_MODE_STANDBY:

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_PLMN_LIST_REQ in Standby Mode");

          curr_rrc_state = tdsrrc_get_state();

          /* Do not honor PLMN_LIST_REQ in DCH/FACH states*/
          if((curr_rrc_state == TDSRRC_STATE_CELL_DCH) || (curr_rrc_state == TDSRRC_STATE_CELL_FACH)) 
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Failing PLMN_LIST_REQ bcos rrc state is DCH/FACH. rrc_state %d",curr_rrc_state);
            tdsrrcmcm_send_plmn_list_cnf_with_failure(cmd_ptr->cmd.plmn_list_req.network_select_mode,
                                                   cmd_ptr->cmd.plmn_list_req.trans_id
                                                   #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
                                                   ,cmd_ptr->cmd.plmn_list_req.service_search
                                                   #endif
                                                   );
          }
          /* Also fail the PLMN_LIST_REQ if UE is in PCH states and an OOS is in progress */
          else if( ((curr_rrc_state == TDSRRC_STATE_CELL_PCH) || (curr_rrc_state == TDSRRC_STATE_URA_PCH)) &&
                   (TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN == tdsrrccsp_get_curr_scan()))
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Failing PLMN_LIST_REQ in PCH state with OOS in progress: rrc_state %d",curr_rrc_state);
            tdsrrcmcm_send_plmn_list_cnf_with_failure(cmd_ptr->cmd.plmn_list_req.network_select_mode,
                                                   cmd_ptr->cmd.plmn_list_req.trans_id
                                                   #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
                                                   ,cmd_ptr->cmd.plmn_list_req.service_search
                                                   #endif
                                                   );    
          }
          /* Check if the PLMN Req is Automatic.If Yes,then give priority to the ongoing Interrat procedure
          and send a plmn cnf with failure to NAS.Make sure to populate the fields correctly */
          else if(cmd_ptr->cmd.plmn_list_req.network_select_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_PLMN_LIST_REQ(Automatic mode) in RRC Standby mode.Sending failure immediately");
            tdsrrcmcm_send_plmn_list_cnf_with_failure(cmd_ptr->cmd.plmn_list_req.network_select_mode,
                                                   cmd_ptr->cmd.plmn_list_req.trans_id
                                                   #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
                                                   ,cmd_ptr->cmd.plmn_list_req.service_search
                                                   #endif
                                                   );    
          }
          else if((curr_rrc_state == TDSRRC_STATE_CONNECTING) && (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN))
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Failing PLMN_LIST_REQ bcos rrc state is CONNECTING. rrc_state %d",curr_rrc_state);
            tdsrrcmcm_send_plmn_list_cnf_with_failure(cmd_ptr->cmd.plmn_list_req.network_select_mode,
                                                   cmd_ptr->cmd.plmn_list_req.trans_id
                                                   #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
                                                   ,cmd_ptr->cmd.plmn_list_req.service_search
                                                   #endif
                                                   );    
          }
          else
          {
          /* Buffer the command */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Buffering PLMN LIST REQ");
            (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type));
            tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_PLMN_LIST_REQ;

            /* Abort the ongoing interrat procedure to process the plmn list req */
            tdsrrcmcm_send_irat_abort_ind(); 
          }
          break; 

        case TDSRRC_MODE_INTERRAT_IN_PROGRESS:
          if(tdsrrccho_is_g2td_pcco_in_progress())
          {
            /*The check is added to avoid the below stuck up scneario
               >GTOW PCCO is in progress, 
               >RRC received EST_REQ, and RRC is processing it
               >RRC received LIST_REQ and mode is updated to Active
               >Connection establishment failure, CHO procedure sends mode change req to INACTIVE
               >MCM ignores the mode change request to Inactive from Active and CHO is stuck up
            */
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Failing PLMN_LIST_REQ in RRC mode %d because GTOW PCCO is in progress",tdsrrc_mode);
            tdsrrcmcm_send_plmn_list_cnf_with_failure(cmd_ptr->cmd.plmn_list_req.network_select_mode,
                                                   cmd_ptr->cmd.plmn_list_req.trans_id
                                                   #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
                                                   ,cmd_ptr->cmd.plmn_list_req.service_search
                                                   #endif
                                                   );    
          }
          else
          {
            /*This can happen when a G->W reselection is in progress and the user has
             *changed mode from Auto->W only.We would not get an activation_rsp 
             *if we succeed.Hence treat this plmn_list_req as an implicit activation_rsp
             */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_PLMN_LIST_REQ in Interrat-in-progress Mode,updating RRC Mode to Active");
            tdsrrcmcm_update_mode(TDSRRC_MODE_ACTIVE);

            /* Forward the service request to CSP */
            tdsrrccsp_cell_selection_procedure(cmd_ptr);

            /* Init data */
            tdsrrcmcm_init_data(TDSINIT_ALL);
          }
          break;

        /* Otherwise send a confirmation with a failure status */
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received TDSRRC_PLMN_LIST_REQ in invalid RRC mode: %d",tdsrrc_mode);
          tdsrrcmcm_send_plmn_list_cnf_with_failure(cmd_ptr->cmd.plmn_list_req.network_select_mode,
                                                 cmd_ptr->cmd.plmn_list_req.trans_id
                                                 #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
                                                 ,cmd_ptr->cmd.plmn_list_req.service_search
                                                 #endif
                                                 );    
          break;
      }
      break;

    case RRC_STOP_TDS_MODE_REQ:
      /* Extract the cause of stopping TDS mode of operation */
      tdsstop_cause = cmd_ptr->cmd.stop_wcdma_mode_req.cause;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received RRC_STOP_TDS_REQ with Cause %d",tdsstop_cause);

      /* Reset the flag which indicates service request or PLMN list request is saved */
      if((tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_SERVICE_REQ) ||
          (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_PLMN_LIST_REQ))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore any saved service req or PLMN list req when processing STOP_REQ from NAS.");
        tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_NONE;
      }

#ifdef FEATURE_OOSC_USER_ACTION
#ifdef FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE
      tdsrrccsp_check_and_send_uoosc_end_ind();
#endif
#endif
      /* If the stop cause is RRC_WTOG_CC_ORDER,send a stop-cnf to MM
         We would stop only after a a successful CCO Cnf from RR */
      if(tdsstop_cause == RRC_WTOG_CC_ORDER)
      {
        /* Return a Stop TDS Confirmation to MM */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received RRC_STOP_TDS_REQ with Cause RRC_WTOG_CC_ORDER in Initial substate,tdsrrc_mode:%d",tdsrrc_mode);

        /* Raise an assert if CCO is not in progress */
        if (!tdsrrccho_is_td2g_cco_active())
        {
          ERR_FATAL("TD2G CHO is not in progress but received stop with cause RRC_WTOG_CC_ORDER",0,0,0);
        }

        tdsrrcmcm_send_stop_tds_cnf(FALSE);
      }

      /* Check if RRC is Inactive mode */
      else if (tdsrrc_mode == TDSRRC_MODE_INACTIVE) 
      {
        /* Return a Stop TDS Confirmation to MM */
        tdsrrcmcm_send_stop_tds_cnf(TRUE);
      }
      else
      {
        due_to_stop_mode = TRUE;

          /* Send a Stop Lower Layer request to RRC/LLC */
          tdsrrcmcm_send_stop_lower_layer_req(due_to_stop_mode);

        /* Since MM has issued this stop commnad,send a cnf to mm after we stop tds */
        tdsrrcmcm_send_stop_cnf_to_mm_flag = TRUE;

        tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF;
      }
      break;

    case TDSRRC_CHANGE_MODE_REQ:
      /* Process the change mode request */
      tdsrrcmcm_process_change_mode_req( cmd_ptr->cmd.change_mode_req );
      break;

/*lint -e408*/
/*lint -e740*/
    case TDSRRC_INTERRAT_RESELECTION_REQ:
     
      switch(tdsrrc_mode)
      {
        case TDSRRC_MODE_INACTIVE:
          /* Start TDS mode process */
          tdsrrcmcm_update_mode(TDSRRC_MODE_INTERRAT_IN_PROGRESS);
          tdsprocedure_id = TDSRRC_PROCEDURE_CSP;
          tdsrrc_init_dynamic_ptrs();
          /*Update the substate and mode before posting the req to L1 as L1 will start processing before we updating the substate*/
          tdsrrcmcm_cmd_store.rrcmcm_interrat_reselection_req_cmd = *((tdsrrc_rr_cmd_type *)cmd_ptr);

          tdsrrc_proc_type = SYS_PROC_TYPE_IRAT_CELL_RESELECTION;
          if(tdsrrc_trm_exchange(&irat_ex_success, &irat_tx_ex_success))
          {
            tdscfa_dsds_add_event(TDSRRC_DSDS_EXCHANGE_LOCK);
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_G2TD: TDRM exchange success for resel");
            tdsrrc_synced_reason = tdrm_get_reason();
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: inherited priority %d from source RAT",tdsrrc_synced_reason);
            tdsrrc_tdrm_exchange_success = TRUE;
            /* Release TX for reselection */
            if(irat_tx_ex_success == TRUE)
            {
              tdrm_release_tx();
            }
          }

          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSIM: R&N after TRM_exchange, exchange flag: (%d, %d)",irat_ex_success, irat_tx_ex_success);
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: force inherited priority to TRM_CHANNEL_MAINTENANCE before source RAT change is ready");
            tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_INTERRAT_IN_MS,TRUE);
            wrm_lock_status = tdsrrcwrm_acquire_wrm_lock(TRM_CHANNEL_MAINTENANCE,100,TRUE);
            tdsrrcwrm_stop_lock_wait_timer();
            if (TDSRRC_ABORT_FROM_RR == wrm_lock_status)
            { 
              tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_RR_INTERRAT_RESELECTION_ABORT_REQ;
              tdsrrcmcm_handle_start_tds_failure_for_dual_sim();
              break;
            }
            else if(wrm_lock_status != TDSRRC_WRM_LOCK_ACQUIRED)
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: RRC cannot acquire lock before start L1 for G2T resel in DR-DSDS, wrm_lock_status: %d", wrm_lock_status);
            }
          }
          /* No more sync for X2T redirect and reselection*/
          //TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: sync TDRM priority reason to %d for IRAT",tdsrrc_synced_reason);
          //tdrm_sync_reason(tdsrrc_synced_reason);

          tdsrrcmcm_state = TDSRRCMCM_WAIT_START_CNF;
          tdsrrcmcm_send_start_tdscdma_req();
          break;

        default:
 
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rvd TDSRRC_INTERRAT_RESELECTION_REQ from RR in Invalid RRC mode");
          break;
      }
      break;

    case TDSRRC_INTERRAT_RESELECTION_ABORT_REQ:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rvd TDSRRC_INTERRAT_RESELECTION_ABORT_REQ from RR in Initial substate,tdsrrc_mode: %d",tdsrrc_mode);
       
      switch(tdsrrc_mode)
      {
        case TDSRRC_MODE_INTERRAT_IN_PROGRESS:
          /* Send the abort ind to csp to abort the interrat process */
          tdsrrcmcm_send_irat_abort_ind();
          break;

        case TDSRRC_MODE_INACTIVE:
          /* Inform CSP to send abort confirmation to rr */
          tdsrrccsp_send_interrat_reselection_abort_cnf();
          break;
        default:
          /* We should not be getting command in any other substate */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_INTERRAT_RESELECTION_ABORT_REQ in Invalid mode,tdsrrc_mode: %d",tdsrrc_mode);
          break;
      }
      break;
/*lint +e740*/
/*lint +e408*/

/*lint -e408*/
    case TDSRRC_INTERRAT_REDIRECT_REQ:
     
      switch(tdsrrc_mode)
      {
        case TDSRRC_MODE_INACTIVE:
          /* Start TDS mode process */
          tdsrrcmcm_update_mode(TDSRRC_MODE_INTERRAT_IN_PROGRESS);
          tdsprocedure_id = TDSRRC_PROCEDURE_CSP;
          tdsrrc_init_dynamic_ptrs();
          /*Update the substate and mode before posting the req to L1 as L1 will start processing before we updating the substate*/
          tdsrrcmcm_cmd_store.rrcmcm_interrat_reselection_req_cmd = *((tdsrrc_rr_cmd_type *)cmd_ptr);
          
          tdsrrc_proc_type =  SYS_PROC_TYPE_REDIRECTION;
          if(tdsrrc_trm_exchange(&irat_ex_success, &irat_tx_ex_success))
          {
            tdscfa_dsds_add_event(TDSRRC_DSDS_EXCHANGE_LOCK);
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_G2TD: TDRM exchange success for redirect");
            tdsrrc_synced_reason = tdrm_get_reason();
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: inherited priority %d from source RAT",tdsrrc_synced_reason);
            tdsrrc_tdrm_exchange_success = TRUE;
            /* Release TX for redirection */
            if(irat_tx_ex_success == TRUE)
            {
              tdrm_release_tx();
            }
          }

          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSIM: R&N after TRM_exchange, exchange flag: (%d, %d)",irat_ex_success, irat_tx_ex_success);
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: force inherited priority to TRM_ACQUISITION before source RAT change is ready");
            tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_INTERRAT_IN_MS,TRUE);
            wrm_lock_status = tdsrrcwrm_acquire_wrm_lock(TRM_ACQUISITION,100,TRUE);
            tdsrrcwrm_stop_lock_wait_timer();
            if (TDSRRC_ABORT_FROM_RR == wrm_lock_status)
            { 
              tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_RR_INTERRAT_REDIRECT_ABORT_REQ;
              tdsrrcmcm_handle_start_tds_failure_for_dual_sim();
              break;
            }
            else if(wrm_lock_status != TDSRRC_WRM_LOCK_ACQUIRED)
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: RRC cannot acquire lock before start L1 for G2T redir in DR-DSDS, wrm_lock_status: %d", wrm_lock_status);
            }
            else
            {
              tdrm_retain_lock(TDRM_CLIENT_RRC);
            }
          }
          /* No more sync for X2T redirect and reselection */
          //TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: sync TDRM priority reason to %d for IRAT",tdsrrc_synced_reason);
          //tdrm_sync_reason(tdsrrc_synced_reason);
          tdsrrcmcm_state = TDSRRCMCM_WAIT_START_CNF;
          tdsrrcmcm_send_start_tdscdma_req();
          break;

        default:
 
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rvd TDSRRC_INTERRAT_REDIRECT_REQ from RR in Invalid RRC mode");
          break;
      }
      break;

    case TDSRRC_INTERRAT_REDIRECT_ABORT_REQ:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rvd TDSRRC_INTERRAT_REDIRECT_ABORT_REQ from RR in Initial substate,tdsrrc_mode: %d",tdsrrc_mode);

      switch(tdsrrc_mode)
      {
        case TDSRRC_MODE_INTERRAT_IN_PROGRESS:
          /* Send the abort ind to csp to abort the interrat process */
          tdsrrcmcm_send_irat_abort_ind();
          break;

        case TDSRRC_MODE_INACTIVE:
          /* Inform CSP to send abort confirmation to rr */
          tdsrrccsp_send_g2td_redirection_abort_cnf();
          break;

        default:
          /* We should not be getting command in any other substate */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_INTERRAT_REDIRECT_ABORT_REQ in Invalid mode,tdsrrc_mode: %d",tdsrrc_mode);
          break;
      }
      break;
/*lint +e408*/
    
    case TDSRRC_ACTIVATION_RSP:
      if((tdsrrcmcm_act_cause == TDSRRCMCM_INTERRAT_CELL_RESELECTION) ||
          (tdsrrcmcm_act_cause == TDSRRCMCM_INTERRAT_CELL_REDIRECTION))
      {
        /* Update the RRC mode to Active */
        tdsrrcmcm_update_mode(TDSRRC_MODE_ACTIVE);
  
        /* Forward the command to CSP for processing */
        tdsrrccsp_cell_selection_procedure(cmd_ptr);
      
        /* Set the last activation cause here */
        if(tdsrrcmcm_act_cause == TDSRRCMCM_INTERRAT_CELL_REDIRECTION)
        {
          tdsrrcmcm_last_act_cause = TDSRRCMCM_ACT_CAUSE_NONE;
        }
        else
        {
          tdsrrcmcm_last_act_cause = TDSRRCMCM_INTERRAT_CELL_RESELECTION;
        }

        /*set the activation cause to none again.  DT or CSP procedures initializes
        the activation cause after sending activation ind to tds*/
        tdsrrcmcm_act_cause = TDSRRCMCM_ACT_CAUSE_NONE;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"FWD TDSRRC_ACTIVATION_RSP to CSP");
      }
      else if( tdsrrcmcm_act_cause ==  RRCMCM_INTERRAT_HANDOVER_G2TD )
      {
#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
        tdsrrcmcm_send_mode_change_ind(TDSRRC_PROCEDURE_IHO, 
                                        tdsrrcmcm_get_rrc_mode(),
                                        TDSRRC_MODE_ACTIVE);

        /* Update the PLMN Id and ACQ type */
        tdsrrccsp_update_plmn_info_for_g2td_handover(&cmd_ptr->cmd.activation_rsp);
  
        /* Update the RRC mode to Active */
        tdsrrcmcm_update_mode(TDSRRC_MODE_ACTIVE);
        tdsrrc_tdrm_exchange_success = FALSE;
        /* report the TDS UARFCN that successful camp on after L2T HO */
        if(tdsrrciho_is_l2t_ho())
        {
          tdsrrccsp_send_lte_interrat_info_ind();
        }

#endif
        /* Set the last activation cause here */
        tdsrrcmcm_last_act_cause = RRCMCM_INTERRAT_HANDOVER_G2TD;
      }    
      else if( tdsrrcmcm_act_cause == RRCMCM_INTERRAT_CC_ORDER_G2TD)
      {
        /* Set the last activation cause */
        tdsrrcmcm_last_act_cause = RRCMCM_INTERRAT_CC_ORDER_G2TD;
        tdsrrccho_procedure_event_handler(cmd_ptr);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"FWD TDSRRC_ACTIVATION_RSP to CHO");

      }
      else if( tdsrrcmcm_act_cause == RRCMCM_INTERRAT_CC_ORDER_TD2G_FAILED)
      {
        /* Set the last activation cause */
        tdsrrcmcm_last_act_cause = RRCMCM_INTERRAT_CC_ORDER_TD2G_FAILED;
        tdsrrccho_procedure_event_handler(cmd_ptr);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"FWD TDSRRC_ACTIVATION_RSP to CHO");
      }
      
     /*set the activation cause to none again.  DT or CSP procedures initializes
        the activation cause after sending activation ind to tds*/
      tdsrrcmcm_act_cause = TDSRRCMCM_ACT_CAUSE_NONE;
      
      break;


#ifdef FEATURE_LTE_TO_TDSCDMA
    case TDSRRC_MSGR_CMD:
      {
        switch(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id)
        {
          case TDSCDMA_RRC_LTE_RESEL_REQ:
            switch(tdsrrc_mode)
            {
              case TDSRRC_MODE_INACTIVE:
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD:TDSCDMA_RRC_LTE_RESEL_REQ from LTE RRC, Initial Substate,rrc_mode %d",tdsrrc_mode);
                tdsrrc_init_dynamic_ptrs();
                tdsrrcmcm_update_mode(TDSRRC_MODE_INTERRAT_IN_PROGRESS);
                tdsprocedure_id = TDSRRC_PROCEDURE_CSP;
                tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_LTE_IRAT_REQ;
                tdsrrcmcm_cmd_store.rrcmcm_lte_irat_cmd = *cmd_ptr;
                
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
                tdsrrc_proc_type = SYS_PROC_TYPE_IRAT_CELL_RESELECTION;
                if(tdsrrc_trm_exchange(&irat_ex_success, &irat_tx_ex_success))
                {
                  tdscfa_dsds_add_event(TDSRRC_DSDS_EXCHANGE_LOCK);
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: TDRM exchange success for resel");
                  tdsrrc_synced_reason = tdrm_get_reason();
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: inherited priority %d from source RAT",tdsrrc_synced_reason);
                  tdsrrc_tdrm_exchange_success = TRUE;
                  /* Release TX for reselection */
                  if(irat_tx_ex_success == TRUE)
                  {
                    tdrm_release_tx();
                  }
                }

                {
                  TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSIM: R&N after TRM_exchange, exchange flag: (%d, %d)",irat_ex_success, irat_tx_ex_success);
                  tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_INTERRAT_IN_MS,TRUE);
                  wrm_lock_status = tdsrrcwrm_acquire_wrm_lock(TRM_CHANNEL_MAINTENANCE,100,TRUE);
                  tdsrrcwrm_stop_lock_wait_timer();
                  if (TDSRRC_ABORT_FROM_RR == wrm_lock_status)
                  { 
                    tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_LTE_ABORT_RESEL_REQ;
                    tdsrrcmcm_handle_start_tds_failure_for_dual_sim();
                    break;
                  }
                  else if(wrm_lock_status != TDSRRC_WRM_LOCK_ACQUIRED)
                  {
                    TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: RRC cannot acquire lock before start L1 for L2T resel in DR-DSDS, wrm_lock_status: %d", wrm_lock_status);
                  }
                }
                /* No more sync for X2T redirect and reselection */
                //TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: sync TDRM priority reason to %d for IRAT",tdsrrc_synced_reason);
                //tdrm_sync_reason(tdsrrc_synced_reason);
#endif
                tdsrrcmcm_state = TDSRRCMCM_WAIT_START_CNF;
                tdsrrcmcm_send_start_tdscdma_req();
                break;
              default:
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"LTOW:Rvd TDSCDMA_RRC_LTE_RESEL_REQ from LTE RRC in Invalid RRC mode");
                break;
            }
            break;

          case TDSCDMA_RRC_LTE_ABORT_RESEL_REQ:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: TDSCDMA_RRC_LTE_ABORT_RESEL_REQ from LTE RRC in Initial substate,rrc_mode: %d",tdsrrc_mode);
            switch(tdsrrc_mode)
            {
              case TDSRRC_MODE_INTERRAT_IN_PROGRESS:
                tdsrrcmcm_send_irat_abort_ind();
                break;

              case TDSRRC_MODE_INACTIVE:
                tdsrrccsp_send_lte_interrat_reselection_abort_rsp();
                break;

              default:
                /* We should not be getting command in any other substate */
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: TDSCDMA_RRC_LTE_ABORT_RESEL_REQ  in Invalid mode,rrc_mode: %d",tdsrrc_mode);
                break;
            }
            break;

          case TDSCDMA_RRC_LTE_PLMN_SRCH_REQ:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: TDSCDMA_RRC_LTE_PLMN_SRCH_REQ from LTE RRC in Initial substate,rrc_mode %d",tdsrrc_mode);
            switch(tdsrrc_mode)
            {
              case TDSRRC_MODE_INACTIVE:
                
                tdsrrc_init_dynamic_ptrs();
                tdsrrcmcm_cmd_store.rrcmcm_lte_irat_cmd = *cmd_ptr;
                tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_LTE_PLMN_SRCH_REQ ;
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
                plmn_list_req = &cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.tds_rrc_lte_plmn_srch_req;
#ifdef LTE_BAND_NUM              
                tdsrrc_csp_int_data.band_pref = plmn_list_req->band_pref.chgwt_band_cap;
#else
                tdsrrc_csp_int_data.band_pref = plmn_list_req->band_pref;
#endif
#endif
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
                tdsrrc_proc_type = SYS_PROC_TYPE_BPLMN_SEARCH;
#endif
                tdsrrcmcm_state = TDSRRCMCM_WAIT_START_CNF;
                tdsrrcmcm_send_start_tdscdma_req();
                break;
              default:
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"L2TD: TDSCDMA_RRC_LTE_PLMN_SRCH_REQ in invalid RRC mode %d",tdsrrc_mode);
                break;
            }
            break;


          case TDSCDMA_RRC_LTE_ABORT_PLMN_SRCH_REQ:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: TDSCDMA_RRC_LTE_ABORT_PLMN_SRCH_REQ from LTE RRC in Initial Substate,rrc_mode %d",tdsrrc_mode);
            switch(tdsrrc_mode)
            {
             case TDSRRC_MODE_INACTIVE:
               tdsrrccsp_send_lte_interrat_plmn_srch_abort_rsp();
               break;

              case TDSRRC_MODE_INTERRAT_IN_PROGRESS:
                tdsrrccsp_cell_selection_procedure( cmd_ptr );
                break;

              default:
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"L2TD::TDSCDMA_RRC_LTE_ABORT_PLMN_SRCH_REQ in an Initial state/Invalid mode %d",tdsrrc_mode);  
                break;
            }
            break;

          case TDSCDMA_RRC_LTE_REDIR_REQ:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: Rvd TDSCDMA_RRC_LTE_REDIR_REQ from LTE RRC in Initial substate,rrc_mode: %d",tdsrrc_mode);

            switch(tdsrrc_mode)
            {
              case TDSRRC_MODE_INACTIVE:

                /* Start TDS mode process */
                tdsrrcmcm_update_mode(TDSRRC_MODE_INTERRAT_IN_PROGRESS);
                tdsprocedure_id = TDSRRC_PROCEDURE_CSP;
                tdsrrc_init_dynamic_ptrs();
                /*Update the substate and mode before posting the req to L1 as L1 will start processing before we updating the substate*/
                tdsrrcmcm_cmd_store.rrcmcm_lte_irat_cmd = *cmd_ptr;
                tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_LTE_IRAT_REQ;

#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
                lte_redir_req = &cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.tds_rrc_lte_redir_req;
                tdsrrc_csp_int_data.band_pref = tdsrrccsp_extract_tds_band_pref_from_rat_pri_list(&(lte_redir_req->rat_pri_list_info));
                tdsrrc_proc_type =  SYS_PROC_TYPE_REDIRECTION;
                if(tdsrrc_trm_exchange(&irat_ex_success, &irat_tx_ex_success))
                {
                  tdscfa_dsds_add_event(TDSRRC_DSDS_EXCHANGE_LOCK);
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: TDRM exchange success for redir");
                  tdsrrc_synced_reason = tdrm_get_reason();
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: inherited priority %d from source RAT",tdsrrc_synced_reason);
                  tdsrrc_tdrm_exchange_success = TRUE;
                  /* Release TX for redirection */
                  if(!tdsrrc_csfb_call_status)
                  {
                    if(irat_tx_ex_success == TRUE)
                    {
                      tdrm_release_tx();
                    }             
                  }
                }
   
                {
                  trm_reason_enum_t tdsrrc_synced_reason_tmp;
                  /* source RAT reason not available, apply default */
                  if(tdsrrc_csfb_call_status)
                  {
                    tdsrrc_synced_reason_tmp = TRM_TRAFFIC;
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Change to TRM_TRAFFIC for CSFB");
                  }
                  else
                  {
                    tdsrrc_synced_reason_tmp = TRM_ACQUISITION;
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: hard code to TRM_ACQUISITION");
                  }
                  TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSIM: R&N after TRM_exchange, exchange flag: (%d, %d)",irat_ex_success, irat_tx_ex_success);
                  tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_INTERRAT_IN_MS,TRUE);
                  wrm_lock_status = tdsrrcwrm_acquire_wrm_lock(tdsrrc_synced_reason_tmp,100,TRUE);
                  tdsrrcwrm_stop_lock_wait_timer();
                  if (TDSRRC_ABORT_FROM_RR == wrm_lock_status)
                  { 
                    tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_LTE_ABORT_REDIR_REQ;
                    tdsrrcmcm_handle_start_tds_failure_for_dual_sim();
                    break;
                  }
                  else if(wrm_lock_status != TDSRRC_WRM_LOCK_ACQUIRED)
                  {
                    TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: RRC cannot acquire lock before start L1 for L2T redir in DR-DSDS, wrm_lock_status: %d", wrm_lock_status);
                  }
                }
                /* No more sync for X2T redirect and reselection */
                //TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: sync TDRM priority reason to %d for IRAT",tdsrrc_synced_reason);
                //tdrm_sync_reason(tdsrrc_synced_reason);
#endif
                tdsrrcmcm_state = TDSRRCMCM_WAIT_START_CNF;
                tdsrrcmcm_send_start_tdscdma_req();
                break;

              default:

                TDSRRC_MSG0(MSG_LEGACY_ERROR,"L2TD: Rvd TDSCDMA_RRC_LTE_REDIR_REQ from LTE RRC in Invalid RRC mode");
                break;
            }
            break;

          case TDSCDMA_RRC_LTE_ABORT_REDIR_REQ:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: Rvd TDSCDMA_RRC_LTE_ABORT_REDIR_REQ from LTE RRC in Initial substate,rrc_mode: %d",tdsrrc_mode);
      
            switch(tdsrrc_mode)
            {
              case TDSRRC_MODE_INTERRAT_IN_PROGRESS:
                /* Send the abort ind to csp to abort the interrat process */
                tdsrrcmcm_send_irat_abort_ind();
                break;

              default:
                /* Inform CSP to send abort confirmation to rr */
                tdsrrccsp_send_lte_interrat_redirection_abort_rsp();
                /* We should not be getting command in any other substate */
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"L2TD: TDSCDMA_RRC_LTE_ABORT_REDIR_REQ in Invalid mode,rrc_mode: %d",tdsrrc_mode);
                break;
            }
            break;

          case TDSCDMA_RRC_LTE_GET_CGI_REQ:
            switch(tdsrrc_mode)
            {
              case TDSRRC_MODE_INACTIVE:
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS

				lte_cgi_req =  &cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.tds_rrc_lte_cgi_req;
                freq_band = tdsrrc_get_frequency_band(lte_cgi_req->uarfcn);
                
                switch(freq_band)
                {
                  case RRC_CSP_TDS_A_BAND:
                    tdsrrc_csp_int_data.band_pref |= SYS_BAND_MASK_TDS_BANDA;
                    break;
                  case RRC_CSP_TDS_E_BAND:
                    tdsrrc_csp_int_data.band_pref |= SYS_BAND_MASK_TDS_BANDE;
                    break;
                  case RRC_CSP_TDS_F_LOWER_BAND:
                  case RRC_CSP_TDS_F_UPPER_BAND:
                    tdsrrc_csp_int_data.band_pref |= SYS_BAND_MASK_TDS_BANDF;
                    break;
                }
#endif
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD:TDSCDMA_RRC_LTE_GET_CGI_REQ from LTE RRC, Initial Substate,rrc_mode %d",tdsrrc_mode);
                tdsrrc_init_dynamic_ptrs();
                tdsrrcmcm_update_mode(TDSRRC_MODE_INTERRAT_IN_PROGRESS);
                tdsprocedure_id = TDSRRC_PROCEDURE_CSP;
                tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_LTE_IRAT_REQ;
                tdsrrcmcm_cmd_store.rrcmcm_lte_irat_cmd = *cmd_ptr;
                tdsrrcmcm_state = TDSRRCMCM_WAIT_START_CNF;
                tdsrrcmcm_send_start_tdscdma_req();
                break;
              default:
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"LTOW:Rvd TDSCDMA_RRC_LTE_GET_CGI_REQ from LTE RRC in Invalid RRC mode");
                break;
            }
            break;

          case TDSCDMA_RRC_LTE_ABORT_CGI_REQ:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: TDSCDMA_RRC_LTE_ABORT_CGI_REQ from LTE RRC in Initial substate,rrc_mode: %d",tdsrrc_mode);
            switch(tdsrrc_mode)
            {
              case TDSRRC_MODE_INTERRAT_IN_PROGRESS:
                tdsrrcmcm_send_irat_abort_ind();
                break;

              case TDSRRC_MODE_INACTIVE:
                tdsrrccsp_send_lte_interrat_cgi_abort_rsp();
                break;

              default:
                /* We should not be getting command in any other substate */
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: TDSCDMA_RRC_LTE_ABORT_CGI_REQ  in Invalid mode,rrc_mode: %d",tdsrrc_mode);
                break;
            }
            break;

          default :
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2TD: Invalid LTE RRC cmd %x in MCM initial substate and rrc_mode ",
                 cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id,tdsrrcmcm_state,tdsrrc_mode);
            break;
        }
      }
      break;
#endif

    default:

      /* No other events are normal in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring MCM event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcmcm_state);
      break;
  }
  return;
}



/*===========================================================================

FUNCTION   RRCMCM_WAIT_START_CNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the MCM procedure is in the TDSRRCMCM_WAIT_START_CNF substate.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_wait_start_cnf_substate_event_handler(
  tdsrrc_cmd_type *cmd_ptr /* Received command                   */
)
{
  boolean due_to_stop_mode = FALSE;
  
  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {

/*lint -e408*/
/*lint -e740*/
    case TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ:
      switch(tdsrrc_mode)
      {
        case TDSRRC_MODE_INACTIVE:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ in Wait-Start-cnf State/Inactive mode");

        /* Buffer this command and process it once you get the start confirmation */
          tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_RR_INTERRAT_PLMN_ABORT_REQ;
          //rrcmcm_cmd_store.rrcmcm_interrat_reselection_req_cmd = *((tdsrrc_rr_cmd_type *)cmd_ptr); 
          break;
      
        default:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ in Wait-Start-cnf State/Invalid mode");

          /* This should never happen */
          break;
      }
      break;

    case TDSRRC_INTERRAT_RESELECTION_ABORT_REQ:
      /* Buffer this command and process it once you get the start confirmation */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:TDSRRC_INTERRAT_RESELECTION_ABORT_REQ in Wait-Start-cnf State");
      tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_RR_INTERRAT_RESELECTION_ABORT_REQ;
      break;
/*lint +e408*/
/*lint +e740*/

/*lint -e408*/
    case TDSRRC_INTERRAT_REDIRECT_ABORT_REQ:
      /* Buffer this command and process it once you get the start confirmation */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:TDSRRC_INTERRAT_REDIRECT_ABORT_REQ in Wait-Start-cnf State");
      tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_RR_INTERRAT_REDIRECT_ABORT_REQ;
      break;
/*lint +e408*/
    /* Mode change request from MM */
    case TDSRRC_MODE_CHANGE_REQ:
      /* Forward the request to CSP */
      tdsrrccsp_cell_selection_procedure( cmd_ptr );
      break;

    /* Deact req from MM */
    case TDSRRC_DEACT_REQ:
      /* Forward the request to CSP */
      tdsrrccsp_cell_selection_procedure( cmd_ptr );
      break;

    case TDSRRC_SERVICE_REQ:
      if((tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_INTERRAT_PLMN_SRCH_REQ) &&
         (tdsrrcmcm_cmd_store.rrcmcm_abort_flag != TDSRRCMCM_RR_INTERRAT_PLMN_ABORT_REQ))
      { 
        /* RR is expected to abort the search. This should ideally never happen*/
        ERR_FATAL("Rx:TDSRRC_SERVICE_REQ when doing BPLMN search - This should never happen",0,0,0);
      }
      else
      {
        /* Overwrite the stored TDSRRC_SERVICE_REQ command */
        (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type));
        tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_SERVICE_REQ;
      }
      break;

    /* Mode change indication from MM */
    case TDSRRC_CHANGE_MODE_IND:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DUAL:No processing is done for the message TDSRRC_CHANGE_MODE_IND in mcm sstate=%d",tdsrrcmcm_state);
      break;

    case RRC_STOP_TDS_MODE_REQ:
      /* Extract the cause of stopping TDS mode of operation */
      tdsstop_cause = cmd_ptr->cmd.stop_wcdma_mode_req.cause;

      /* Set Command received flag */
      tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = RRCMCM_CMD_STOP_TDS_REQ;
      break;

    case TDSRRC_PLMN_LIST_REQ:
      if((tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_INTERRAT_PLMN_SRCH_REQ) &&
         (tdsrrcmcm_cmd_store.rrcmcm_abort_flag != TDSRRCMCM_RR_INTERRAT_PLMN_ABORT_REQ))
      { 
        /* RR is expected to abort the search. This should ideally never happen*/
        ERR_FATAL("Rx:TDSRRC_PLMN_LIST_REQ when doing BPLMN search - This should never happen",0,0,0);
      }
      else
      {
        /* if MCM is handling the revice req, abort it and send no service to NAS and handle plmn list req directly */
        if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_SERVICE_REQ)
        {
          /* Send SERVICE_CNF with no service */
          tdsrrccsp_send_mm_cmd(RRC_SERVICE_CNF,
                                   MM_AS_NO_SERVICE,
                                   &tdsrrc_csp_int_data.curr_sel_plmn,
                                   &tdsrrc_csp_int_data.curr_acq_entry);
        }
        /* Store the command */
        (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type));
        tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_PLMN_LIST_REQ;
      }
      break;

    case RRC_CPHY_START_TDS_MODE_CNF:
      // BPLMN search abort request
      if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_RR_INTERRAT_PLMN_ABORT_REQ)      
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing buffered RRC_INTERRAT_PLMN_ABORT_REQ after start cnf");
        /* stop tds */
        tdsrrcmcm_update_mode(TDSRRC_MODE_INTERRAT_IN_PROGRESS);
        tdsstop_cause = RRC_RAT_CHANGE;
        tdsrrcmcm_send_stop_lower_layer_req(due_to_stop_mode);

        /* Change the state to Wait for Stop Confirmation */
        tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF;    
      }
      
      else if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_RR_INTERRAT_RESELECTION_ABORT_REQ)      
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing buffered TDSRRC_INTERRAT_RESELECTION_ABORT_REQ after start cnf - Sending Stop cmd");

        /* Stop tds */
        tdsrrcmcm_update_mode(TDSRRC_MODE_INTERRAT_IN_PROGRESS);
        due_to_stop_mode = TRUE;
        tdsstop_cause = RRC_DEEP_SLEEP;
        tdsrrcmcm_send_stop_lower_layer_req(due_to_stop_mode);

        /* Change the state to Wait for Stop Confirmation */
        tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF;    
      }

      else if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_RR_INTERRAT_REDIRECT_ABORT_REQ)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing buffered TDSRRC_INTERRAT_REDIRECT_ABORT_REQ after start cnf - Sending Stop cmd");

        /* Stop tds */
        tdsrrcmcm_update_mode(TDSRRC_MODE_INTERRAT_IN_PROGRESS);
        due_to_stop_mode = TRUE;
        tdsstop_cause = RRC_DEEP_SLEEP;
        tdsrrcmcm_send_stop_lower_layer_req(due_to_stop_mode);

        /* Change the state to Wait for Stop Confirmation */
        tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF;    
      }

#ifdef FEATURE_LTE_TO_TDSCDMA
      else if((tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_RESEL_REQ) ||
              (tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_REDIR_REQ) ||
              (tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_CGI_REQ)    
              )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: Processing saved L2TD interrat Abort req after start cnf");
        tdsrrcmcm_update_mode(TDSRRC_MODE_INTERRAT_IN_PROGRESS);
        due_to_stop_mode = TRUE;
        tdsstop_cause = RRC_DEEP_SLEEP;
        tdsrrcmcm_send_stop_lower_layer_req(due_to_stop_mode);
        tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF;    
      }
      else if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_PLMN_SRCH_REQ)      
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: Processing TDSCDMA_RRC_ABORT_PLMN_SRCH_REQ after start cnf");
        tdsrrcmcm_update_mode(TDSRRC_MODE_INTERRAT_IN_PROGRESS);
        tdsstop_cause = RRC_RAT_CHANGE;
        tdsrrcmcm_send_stop_lower_layer_req(due_to_stop_mode);
        tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF;    
      }
      else if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_LTE_IRAT_REQ)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: Continue processing LTE->TDSCDMA request after start cnf");
        tdsrrccsp_cell_selection_procedure(&tdsrrcmcm_cmd_store.rrcmcm_lte_irat_cmd);
        tdsrrcmcm_init_data(TDSINIT_ALL);
        tdsrrctmr_start_timer( TDSRRCTMR_RSSI_BER_TIMER,
                               TDSRRCTMR_RSSI_BER_REPORT_TIME );
      }
      else if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_LTE_PLMN_SRCH_REQ)
      {
        tdsrrcmcm_update_mode(TDSRRC_MODE_INTERRAT_IN_PROGRESS);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: Continue processing TDSCDMA_RRC_LTE_PLMN_SRCH_REQ after start cnf"); 
        tdsrrccsp_cell_selection_procedure(&tdsrrcmcm_cmd_store.rrcmcm_lte_irat_cmd);
        tdsrrcmcm_init_data(TDSINIT_ALL);
      }
#endif

      // BPLMN search request triggered the wCDMA start 
      else if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_INTERRAT_PLMN_SRCH_REQ)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Continue processing RRC_INTERRAT_PLMN_REQ after start cnf"); 

        /* Update RRC mode */
        tdsrrcmcm_update_mode(TDSRRC_MODE_INTERRAT_IN_PROGRESS);

        /* Forward inter-RAT PLMN search request to CSP */
        /*lint -e740*/
        tdsrrccsp_cell_selection_procedure( (tdsrrc_cmd_type *)(&tdsrrcmcm_cmd_store.rrcmcm_interrat_reselection_req_cmd) );        
        /*lint +e740*/

        /* Go back to initial state */
        tdsrrcmcm_init_data(TDSINIT_ALL);
      }

      /*if mode_change_req is received earlier, then process that command and*/
      else if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_MODE_CHANGE)
      {
        /* Update RRC Mode and RAT preference, and
           send the mode change indication if needed */
        tdsrrcmcm_update_mode(TDSRRC_MODE_ACTIVE);

        tdsrrcmcm_process_mode_change_req();
      }      

      /* Check if a RRC_STOP_TDS_MODE_REQ is received */
      else if (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == RRCMCM_CMD_STOP_TDS_REQ) 
      {
        /* Update RRC Mode and RAT preference, and
           send the mode change indication if needed */
        tdsrrcmcm_update_mode(TDSRRC_MODE_ACTIVE);

        due_to_stop_mode = TRUE;

          /* Send a stop lower layer request to LLC */
          tdsrrcmcm_send_stop_lower_layer_req(due_to_stop_mode);
       
        /* Since MM has issued this stop commnad,send a cnf to mm after we stop tds */
        tdsrrcmcm_send_stop_cnf_to_mm_flag = TRUE;

    /* Clear RRC change mode requests,if any */
        tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = TDSRRC_MODE_MAX;

        /* Change the state to Wait for Stop Confirmation */
        tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF;
      }

      else if (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_SERVICE_REQ)
      {
        /* Update RRC Mode and RAT preference (2.0), and
           send the mode change indication if needed */
        tdsrrcmcm_update_mode(TDSRRC_MODE_ACTIVE);

#ifdef FEATURE_RAT_PRIORITY_LIST
        tdsrrcmcm_update_mode_capability_from_service_req(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd.cmd.rrc_service_req.rat_pri_list_info);
#else
        /* Set the dual-mode enable flag */
        tdsrrc_dual_mode_enabled = tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd.cmd.rrc_service_req.dual_mode;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting tdsrrc_dual_mode_enabled to %d",tdsrrc_dual_mode_enabled);
#endif

        /* Invoke the initial cell selection */
        tdsrrccsp_cell_selection_procedure((tdsrrc_cmd_type *) &tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd);

        /* Start the RSSI reporting timer */
        tdsrrctmr_start_timer( TDSRRCTMR_RSSI_BER_TIMER,
                            TDSRRCTMR_RSSI_BER_REPORT_TIME );

        tdsrrcmcm_init_data(TDSINIT_ALL);
      }

      else if (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_PLMN_LIST_REQ)
      {
        /* Update RRC Mode and RAT preference (2.0), and
           send the mode change indication if needed */
        tdsrrcmcm_update_mode(TDSRRC_MODE_ACTIVE);

        /* Invoke the initial cell selection */
        tdsrrccsp_cell_selection_procedure((tdsrrc_cmd_type *) &tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd);

        /* Start the RSSI reporting timer */
        tdsrrctmr_start_timer( TDSRRCTMR_RSSI_BER_TIMER,
                            TDSRRCTMR_RSSI_BER_REPORT_TIME );

        tdsrrcmcm_init_data(TDSINIT_ALL);

      }
      else 
      {

       /* Check if RRC is in INTERRAT_IN_PROGRESS mode */
        if (tdsrrc_mode == TDSRRC_MODE_INTERRAT_IN_PROGRESS) 
        {
          if (tdsprocedure_id == TDSRRC_PROCEDURE_CSP) 
          {
            /*lint -e740*/
            /* Forward inter-RAT cell selection request to CSP */
            tdsrrccsp_cell_selection_procedure( (tdsrrc_cmd_type *)(&tdsrrcmcm_cmd_store.rrcmcm_interrat_reselection_req_cmd) );
            /*lint +e740*/
          }
          else
          {
            /* Send RRC mode change confirmation */
            tdsrrcmcm_send_change_mode_cnf(TRUE);
          }
        }

        /* Check if there is a pending change mode req */
        else if(tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode != TDSRRC_MODE_MAX)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Pending Ch-Mode-Req to mode %d in MCM-Sub-State %d",tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode,tdsrrcmcm_state);

          //send change mode req and reset the cmd_store flag
          tdsrrcmcm_send_change_mode_req(tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode, 
                                      tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.proc);
          tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = TDSRRC_MODE_MAX;
          tdsrrcmcm_state = TDSRRCMCM_INITIAL;
        }
        else
        { 
          /* Update RRC Mode and RAT preference (2.0), and 
             send the mode change indication if needed */
          tdsrrcmcm_update_mode(TDSRRC_MODE_ACTIVE);

          if (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag != TDSRRCMCM_CMD_PLMN_LIST_REQ)
          {
#ifdef FEATURE_RAT_PRIORITY_LIST
            tdsrrcmcm_update_mode_capability_from_service_req(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd.cmd.rrc_service_req.rat_pri_list_info);
#else
            /* Set the dual-mode enable flag */
            tdsrrc_dual_mode_enabled = tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd.cmd.rrc_service_req.dual_mode;
#endif
          }

          /* Start the cell selection */
          tdsrrccsp_cell_selection_procedure( &tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd );
        }

        /* Start the RSSI reporting timer */
        tdsrrctmr_start_timer( TDSRRCTMR_RSSI_BER_TIMER,
                            TDSRRCTMR_RSSI_BER_REPORT_TIME );
        tdsrrcmcm_init_data(TDSINIT_ALL);
      }   
      break;

    case TDSRRC_CHANGE_MODE_REQ:
      /*we can receive a CHANGE_MODE_REQ at any state. Therefore
        save the cmd_flag to change_mode type, this can over-ride
        another other saved flag as we should be going to STOP as
        soon as we get this from MM, this will override all other
        commands*/
      if(cmd_ptr->cmd.change_mode_req.mode == TDSRRC_MODE_CHANGE)
      {
        tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_MODE_CHANGE;
      }
      else
      {
        /*  in cases when an rrc procedure does a change-mode-req to another substate
            save the change_mode_req_mode and honor it before honoring any other change
            requests when the current change_mode_req is honored
         */
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Buffering CHANGE_MODE_REQ.req_mode %d,req_proc %d,mcm_substate %d",
                 cmd_ptr->cmd.change_mode_req.mode,
                 cmd_ptr->cmd.change_mode_req.procedure,
                 tdsrrcmcm_state);
        tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = cmd_ptr->cmd.change_mode_req.mode;
        tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.proc = cmd_ptr->cmd.change_mode_req.procedure;
      }
      break;

#ifdef FEATURE_LTE_TO_TDSCDMA
     case TDSRRC_MSGR_CMD:
        switch(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id)
        {
          case TDSCDMA_RRC_LTE_ABORT_RESEL_REQ:
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"LTOW: TDSCDMA_RRC_LTE_ABORT_RESEL_REQ from LTE RRC in MCM state %d,RRC mode %d",tdsrrcmcm_state,tdsrrc_mode);
            tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_LTE_ABORT_RESEL_REQ;
            break;
          case TDSCDMA_RRC_LTE_ABORT_PLMN_SRCH_REQ:
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"LTOW: TDSCDMA_RRC_LTE_ABORT_PLMN_SRCH_REQ from LTE RRC in MCM state %d,RRC mode %d",tdsrrcmcm_state,tdsrrc_mode);
            tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_LTE_ABORT_PLMN_SRCH_REQ;
            break;
          case TDSCDMA_RRC_LTE_ABORT_REDIR_REQ:
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"LTOW: TDSCDMA_RRC_LTE_ABORT_REDIR_REQ from LTE RRC in MCM state %d,RRC mode %d",tdsrrcmcm_state,tdsrrc_mode);
            tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_LTE_ABORT_REDIR_REQ;
            break;
          case TDSCDMA_RRC_LTE_ABORT_CGI_REQ:
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"LTOW: TDSCDMA_RRC_LTE_ABORT_CGI_REQ from LTE RRC in MCM state %d,RRC mode %d",tdsrrcmcm_state,tdsrrc_mode);
            tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_LTE_ABORT_CGI_REQ;
            break;
          default :
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"LTOW: Invalid cmd %x from LTE RRC in MCM state %d, RRC mode %d",
                cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id,tdsrrcmcm_state,tdsrrc_mode);
            break;
        }
        break;
#endif

    default:

      /* No other events are normal in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Ignoring MCM event %x in state %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcmcm_state);
      break;
  }
  return;
}



/*===========================================================================

FUNCTION   RRCMCM_WAIT_STOP_CNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the procedure is in the Wait for Stop Confirmation substate.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_wait_stop_cnf_substate_event_handler(
  tdsrrc_cmd_type *cmd_ptr  /* Received command                   */
)
{

  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
/*lint -e408*/
    case TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ:
      switch(tdsrrc_mode)
      {
        case TDSRRC_MODE_INTERRAT_IN_PROGRESS:

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ in Wait-Stop-cnf State/Interrat-in-progress mode");

          /* Forward the abort request to CSP because CSP is already processing a BPLMN search request  */
          tdsrrccsp_cell_selection_procedure( cmd_ptr );                
          break;
      
        default:

          /* This should never happen */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ in Wait-Stop-cnf State/Invalid mode");
          break;
      }
      break;

    case TDSRRC_INTERRAT_RESELECTION_ABORT_REQ:
      /* Abort the ongoing interrat reselection procedure */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:TDSRRC_INTERRAT_RESELECTION_ABORT_REQ in Wait-Stop-cnf State");
      tdsrrcmcm_send_irat_abort_ind(); 
      break;
/*lint +e408*/

/*lint -e408*/
    case TDSRRC_INTERRAT_REDIRECT_ABORT_REQ:
      /* Abort the ongoing interrat reselection procedure */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:TDSRRC_INTERRAT_REDIRECT_ABORT_REQ in Wait-Stop-cnf State");
      tdsrrcmcm_send_irat_abort_ind(); 
      break;
/*lint +e408*/
    /* Mode change request from MM */
    case TDSRRC_MODE_CHANGE_REQ:
      /* Forward the request to CSP */
      tdsrrccsp_cell_selection_procedure( cmd_ptr );
      break;

    /* Deact req from MM */
    case TDSRRC_DEACT_REQ:
      /* Forward the request to CSP */
      tdsrrccsp_cell_selection_procedure( cmd_ptr );
      break;

    case TDSRRC_SERVICE_REQ:
    /* If BPLMN search is active and we get a service request,send a cnf with failure
     This should ideally never happen */
      if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_INTERRAT_PLMN_SRCH_REQ)
      { 
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:TDSRRC_SERVICE_REQ when doing BPLMN search - This should never happen");
        tdsrrcmcm_send_service_cnf_with_failure();
      }
      else
      {
        /* Store the TDSRRC_SERVICE_REQ command and set the command received flag */
        (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type));
        tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_SERVICE_REQ;
      }
      break;

     /* Mode change indication from MM */
   case TDSRRC_CHANGE_MODE_IND:
     TDSRRC_MSG1(MSG_LEGACY_HIGH,"DUAL:No processing is done for the message TDSRRC_CHANGE_MODE_IND in mcm sstate=%d",tdsrrcmcm_state);  
     break;

    case TDSRRC_PLMN_LIST_REQ:
      /* If BPLMN search is active and we get a PLMN list request,send a cnf with failure
      This should ideally never happen */
      if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_INTERRAT_PLMN_SRCH_REQ)
      { 
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx:TDSRRC_PLMN_LIST_REQ when doing BPLMN search - This should never happen");
        tdsrrcmcm_send_plmn_list_cnf_with_failure(cmd_ptr->cmd.plmn_list_req.network_select_mode,
                                               cmd_ptr->cmd.plmn_list_req.trans_id
                                               #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
                                               ,cmd_ptr->cmd.plmn_list_req.service_search
                                               #endif
                                               );    
      }
      else
      {
        /* Store the TDSRRC_PLMN_LIST_REQ command and set the command received flag */
        (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type));
        tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_PLMN_LIST_REQ;
      }
      break;

    case RRC_STOP_TDS_MODE_REQ:

      /* Reset the flag which indicates service request or PLMN list request is saved */
      if((tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_SERVICE_REQ) ||
          (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_PLMN_LIST_REQ))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore any service req or PLMN list req saved when processing STOP_REQ from NAS.");
        tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_NONE;
      }

      /* Though we are already processing a stop cmd,
         send a cnf to mm after we stop tds,cos MM would be expecting it */
      tdsrrcmcm_send_stop_cnf_to_mm_flag = TRUE;

      break;

    case TDSRRC_STOP_LOWER_LAYER_CNF:
#ifdef FEATURE_WRLF_SYSTEM_SEL
      /* Free tdsrrc_svd_plmn_req_ptr when stop TDS done*/
      if (tdsrrc_svd_plmn_req_ptr != NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Stop TDS done. Abort saved PLMN list req.");
        tdsrrccsp_send_mm_plmn_list_cnf_as_aborted(tdsrrc_svd_plmn_req_ptr->trans_id);
        tdsrrc_free(tdsrrc_svd_plmn_req_ptr);
        tdsrrc_svd_plmn_req_ptr = NULL;
      }

      if(!TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
#endif
      {
        tdsrrc_free_dynamic_ptrs();
      }

      /* process BPLMN abort message if its pending(recd in start-cnf state) */
      if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_RR_INTERRAT_PLMN_ABORT_REQ)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Continue processing RRC_INTERRAT_PLMN_ABORT_REQ after stop cnf");

        /* Inform CSP to cleanup and send abort confirmation to rr */
        tdsrrccsp_send_bplmn_abort_cnf_to_rr();      

        /* Set mode back to inactive*/ 
        tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);

        if((tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_SERVICE_REQ)
          || (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_PLMN_LIST_REQ))
        {
          /* Go back to initial state */
          tdsrrcmcm_init_data(TDSINIT_STATE_VARS_ONLY);
          tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_RR_ABORT_NONE;
          tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = TDSRRC_MODE_MAX;
        }
        else
        {
          /* Go back to initial state */
          tdsrrcmcm_init_data(TDSINIT_ALL);
        }
      }

      /* process reselection abort message if its pending(recd in start-cnf state) */
      else if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_RR_INTERRAT_RESELECTION_ABORT_REQ)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Continue processing TDSRRC_INTERRAT_RESELECTION_ABORT_REQ after stop cnf");

        /* Inform CSP to send abort confirmation to rr */
        tdsrrccsp_send_interrat_reselection_abort_cnf();

        /* Set mode back to inactive*/ 
        tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);

        if((tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_SERVICE_REQ)
          || (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_PLMN_LIST_REQ))
        {
          /* Go back to initial state */
          tdsrrcmcm_init_data(TDSINIT_STATE_VARS_ONLY);
          tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_RR_ABORT_NONE;
          tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = TDSRRC_MODE_MAX;
        }
        else
        {
          /* Go back to initial state */
          tdsrrcmcm_init_data(TDSINIT_ALL);
        }
      }

      else if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_RR_INTERRAT_REDIRECT_ABORT_REQ)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Continue processing TDSRRC_INTERRAT_REDIRECT_ABORT_REQ after stop cnf");

        /* Inform CSP to send abort confirmation to rr */
        tdsrrccsp_send_g2td_redirection_abort_cnf();

        /* Set mode back to inactive*/ 
        tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);

        if((tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_SERVICE_REQ)
          || (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_PLMN_LIST_REQ))
        {
          /* Go back to initial state */
          tdsrrcmcm_init_data(TDSINIT_STATE_VARS_ONLY);
          tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_RR_ABORT_NONE;
          tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = TDSRRC_MODE_MAX;
        }
        else
        {
          /* Go back to initial state */
          tdsrrcmcm_init_data(TDSINIT_ALL);
        }
      }
#ifdef FEATURE_LTE_TO_TDSCDMA
      else if((tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_RESEL_REQ) ||
                (tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_PLMN_SRCH_REQ) ||
                (tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_REDIR_REQ) ||
                (tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_CGI_REQ)
                ) 
      {
        if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_RESEL_REQ)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"LTOW: Continue processing TDSCDMA_RRC_LTE_ABORT_RESEL_REQ after STOP_CNF");
          tdsrrccsp_send_lte_interrat_reselection_abort_rsp();
        }
        else if(tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_REDIR_REQ)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"LTOW: Continue processing TDSCDMA_RRC_LTE_ABORT_REDIR_REQ after STOP_CNF");
          tdsrrccsp_send_lte_interrat_redirection_abort_rsp();
        }
        else if (tdsrrcmcm_cmd_store.rrcmcm_abort_flag == TDSRRCMCM_LTE_ABORT_PLMN_SRCH_REQ)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"LTOW: Continue processing TDSCDMA_RRC_LTE_ABORT_PLMN_SRCH_REQ after STOP_CNF");
          tdsrrccsp_send_lte_interrat_plmn_srch_abort_rsp();
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"LTOW: Continue processing TDSCDMA_RRC_LTE_ABORT_CGI_REQ after STOP_CNF");
          tdsrrccsp_send_lte_interrat_cgi_abort_rsp();
        }
        tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);
        if((tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_SERVICE_REQ)
          || (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_PLMN_LIST_REQ))
        {
          tdsrrcmcm_init_data(TDSINIT_STATE_VARS_ONLY);
          tdsrrcmcm_cmd_store.rrcmcm_abort_flag = TDSRRCMCM_RR_ABORT_NONE;
          tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = TDSRRC_MODE_MAX;
        }
        else
        {
          tdsrrcmcm_init_data(TDSINIT_ALL);
        }
      }
#endif
      /*if mode_change_req is received earlier, then process that command and*/
      if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_MODE_CHANGE)
      {
        tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);
        tdsrrcmcm_send_stop_tds_cnf(TRUE);
        tdsrrcmcm_process_mode_change_req();
      }

      /* Check if a TDSRRC_SERVICE_REQ or TDSRRC_PLMN_LIST_REQ is received */
      else if (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_SERVICE_REQ ||
               tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_PLMN_LIST_REQ ) 
      {
        /*send mode change cnf first, then mode change ind to csp/ccm*/
        tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);

        /* Send the mode change indication to CSP&CCM. NOTE: In the future,
           if registring method is implemented, the following will be
           removed */
        tdsrrcmcm_send_mode_change_ind(TDSRRC_PROCEDURE_CSP, tdsrrc_mode, 
                                    TDSRRC_MODE_INACTIVE);
        tdsrrcmcm_send_mode_change_ind(TDSRRC_PROCEDURE_CCM, tdsrrc_mode,
                                    TDSRRC_MODE_INACTIVE);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent Mode Change to CSP and CCM");

#ifdef FEATURE_WRLF_SYSTEM_SEL
        if(!TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
#endif
        {
          tdsrrc_init_dynamic_ptrs();
        }
        /*Update the substate and mode before posting the req to L1 as L1 will start processing before we updating the substate*/
        tdsrrcmcm_state = TDSRRCMCM_WAIT_START_CNF;
        /* Send a request to restart the TDS Mode */
        tdsrrcmcm_send_start_tdscdma_req();
      }

      else
      {
        /* Check for if RRC is in INTERRAT_IN_PROGRESS mode */
        if (tdsrrc_mode == TDSRRC_MODE_INTERRAT_IN_PROGRESS) 
        {
          
          tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);
          tdsrrcmcm_send_change_mode_cnf(TRUE);

          /* Send stop cnf to MM if needed */
          if(tdsrrcmcm_send_stop_cnf_to_mm_flag)
          {
            /* Send a Stop TDS Mode Confirmation to MM */
            tdsrrcmcm_send_stop_tds_cnf(TRUE);
          }

          /* Send the mode change indication to CSP&CCM. NOTE: In the future,
             if registring method is implemented, the following will be
             removed */
          tdsrrcmcm_send_mode_change_ind(TDSRRC_PROCEDURE_CSP, tdsrrc_mode, 
                                      TDSRRC_MODE_INACTIVE);
          tdsrrcmcm_send_mode_change_ind(TDSRRC_PROCEDURE_CCM, tdsrrc_mode,
                                      TDSRRC_MODE_INACTIVE);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent Mode Change to CSP and CCM");

        }
        else
        { 

		  /* Send the mode change indication to CSP&CCM. NOTE: In the future,
		   if registring method is implemented, the following will be
		   removed */
		  tdsrrcmcm_send_mode_change_ind(TDSRRC_PROCEDURE_CSP, tdsrrc_mode, 
									TDSRRC_MODE_INACTIVE);
		  tdsrrcmcm_send_mode_change_ind(TDSRRC_PROCEDURE_CCM, tdsrrc_mode,
									TDSRRC_MODE_INACTIVE);
		  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent Mode Change to CSP and CCM");

          /* Update the RRC mode and RAT preference, and send mode change
             indications if needed */
          tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);

          if(tdsrrcmcm_send_stop_cnf_to_mm_flag)
          {
            /* Send a Stop TDS Mode Confirmation to MM */
            tdsrrcmcm_send_stop_tds_cnf(TRUE);
			tdsrrc_reset_oos_due_to_no_resource_flag();
          }
        }

        /* Stop the RSSI reporting timer */
        tdsrrctmr_stop_timer(TDSRRCTMR_RSSI_BER_TIMER);
  
        tdsrrcmcm_init_data(TDSINIT_ALL);
      }
      break;

    case  TDSRRC_CONTINUE_STOP_MODE_REQ:
       tdsrrcmcm_send_stop_lower_layer_req(TRUE);
      break;
      
    
    case TDSRRC_CHANGE_MODE_REQ:
      /*we can receive a CHANGE_MODE_REQ at any state. Therefore
        save the cmd_flag to change_mode type, this can over-ride
        another other saved flag as we should be going to STOP as
        soon as we get this from MM, this will override all other
        commands*/
      if(cmd_ptr->cmd.change_mode_req.mode == TDSRRC_MODE_CHANGE)
      {
        tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_MODE_CHANGE;
      }
      break;

#ifdef FEATURE_LTE_TO_TDSCDMA
    case TDSRRC_MSGR_CMD:
      switch(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id)
      {
        case TDSCDMA_RRC_LTE_ABORT_RESEL_REQ:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"LTOW: TDSCDMA_RRC_LTE_ABORT_RESEL_REQ in wt_stop_cnf,rrc_mode %d",tdsrrc_mode);
          tdsrrcmcm_send_irat_abort_ind();
          break;
        case TDSCDMA_RRC_LTE_ABORT_PLMN_SRCH_REQ:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"LTOW: TDSCDMA_RRC_LTE_ABORT_PLMN_SRCH_REQ in wt_stop_cnf,rrc_mode %d",tdsrrc_mode);
          if(tdsrrc_mode == TDSRRC_MODE_INTERRAT_IN_PROGRESS)
          {
            tdsrrccsp_cell_selection_procedure(cmd_ptr);
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"LTOW: TDSCDMA_RRC_LTE_ABORT_PLMN_SRCH_REQ in wt_stop_cnf,Invalid rrc_mode %d",tdsrrc_mode);
          }
          break;
        case TDSCDMA_RRC_LTE_ABORT_REDIR_REQ:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"LTOW: TDSCDMA_RRC_LTE_ABORT_REDIR_REQ in wt_stop_cnf,rrc_mode %d",tdsrrc_mode);
          tdsrrcmcm_send_irat_abort_ind();
          break;
        case TDSCDMA_RRC_LTE_ABORT_CGI_REQ:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"LTOW: TDSCDMA_RRC_LTE_ABORT_CGI_REQ in wt_stop_cnf,rrc_mode %d",tdsrrc_mode);
          tdsrrcmcm_send_irat_abort_ind();
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"LTOW: Invalid LTE RRC cmd %x",cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id);
          break;
      }
      break;
#endif
    default:

      /* No other events are normal in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Ignoring MCM event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcmcm_state);
      break;
  }
  return;
}


/*===========================================================================

FUNCTION   RRCMCM_WAIT_STOP_CNF_FOR_MODE_CHANGE_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the procedure is in the Wait for Stop Confirmation for a Mode Change
  substate.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_wait_stop_cnf_for_mode_change_substate_event_handler(
  tdsrrc_cmd_type *cmd_ptr  /* Received command                   */
)
{
  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_STOP_LOWER_LAYER_CNF:
      tdsrrc_free_dynamic_ptrs();
#ifdef FEATURE_WRLF_SYSTEM_SEL
      /* Free tdsrrc_svd_plmn_req_ptr when stop TDS done*/
      if (tdsrrc_svd_plmn_req_ptr != NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Stop TDS done. Abort saved PLMN list req.");
        tdsrrccsp_send_mm_plmn_list_cnf_as_aborted(tdsrrc_svd_plmn_req_ptr->trans_id);
        tdsrrc_free(tdsrrc_svd_plmn_req_ptr);
        tdsrrc_svd_plmn_req_ptr = NULL;
      }

      if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
      {
        tdsrrc_csp_int_data.ue_in_conn_oos = FALSE;
      }
#endif

      tdsrrcmcm_send_change_mode_cnf(TRUE);
      tdsrrcmcm_send_mode_change_ind(TDSRRC_PROCEDURE_IHO, 
                                  TDSRRC_MODE_CHANGE,
                                  TDSRRC_MODE_INACTIVE);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Game Mode: stopped the TDSRRCTMR_RSSI_BER_TIMER");
      tdsrrctmr_stop_timer(TDSRRCTMR_RSSI_BER_TIMER);
      tdsrrcmcm_update_mode(TDSRRC_MODE_INACTIVE);      
      tdsrrcmcm_init_data(TDSINIT_ALL);

      break;

    case  TDSRRC_CONTINUE_STOP_MODE_REQ:
      tdsrrcmcm_send_stop_lower_layer_req(FALSE);
      break;

    default:

      /* No other events are normal in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Ignoring MCM event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcmcm_state);
      break;
  }
  return;
}


/*===========================================================================

FUNCTION   RRCMCM_WAIT_SUSPEND_CNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the procedure is in the Wait for SUSPEND Confirmation substate.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_wait_suspend_cnf_substate_event_handler(
  tdsrrc_cmd_type *cmd_ptr  /* Received command */
)
{
  boolean due_to_stop_mode = FALSE;
  tdsrrc_state_e_type curr_rrc_state;

  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* TDSRRC_LLC_CMD_CNF */
    case TDSRRC_LLC_CMD_CNF:
      
      /* If RRC_STOP_TDS_REQ was received act on it */
      if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == RRCMCM_CMD_STOP_TDS_REQ)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing buffered STOP_TDS_REQ on suspend_cnf substate");
        due_to_stop_mode = TRUE;
        
        /* Send a stop lower layer request to LLC */
        tdsrrcmcm_send_stop_lower_layer_req(due_to_stop_mode);

        /* Clear RRC change mode requests,if any */
        tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = TDSRRC_MODE_MAX;

        /* Change the state to Wait for Stop Confirmation */
        tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF;
      }

      /*if mode_change_req is received earlier, then process that command.. we
        are going to STOP TDS mode*/
      else if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_MODE_CHANGE)
      {
        tdsrrcmcm_process_mode_change_req();
      } 
      else if(tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode != TDSRRC_MODE_MAX)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Pending Ch-Mode-Req to mode %d in MCM-Sub-State %d",tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode,tdsrrcmcm_state);

        //send change mode req and reset the cmd_store flag
        tdsrrcmcm_send_change_mode_req(tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode, 
                                    tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.proc);
        tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = TDSRRC_MODE_MAX;
        tdsrrcmcm_state = TDSRRCMCM_INITIAL;
      }
      else if (cmd_ptr->cmd.rrc_llc_cnf.ll_cnf == TDSRRC_LL_CPHY_SUSPEND_CNF) 
      {
        /* Check for CPHY_SUSPEND_CNF */
        /* Check if suspend operation is successful */
        if (cmd_ptr->cmd.rrc_llc_cnf.cnf.suspend_cnf.status) 
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rvd CPHY_SUSPEND_TDS_MODE_CNF");
  
          /* Process buffered service req,if any */
          if( (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_SERVICE_REQ) ||
              (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_PLMN_LIST_REQ) ||
              (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_MODE_CHANGE_REQ)||
              (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_DEACT_REQ) )
          {
            tdsrrcwrm_release_lock();
            /* Send an abort indication to the interrat procedure in order to process the 
               Service_Req from MM */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending IRAT_ABORT_IND to process a buffered MM command");
            tdsrrcmcm_send_irat_abort_ind();

            /* Do not init cmd store because we need to process it later */
            tdsrrcmcm_init_data(TDSINIT_STATE_VARS_ONLY);
          }
          else if(tdsrrcmcm_est_req_saved_flag == TRUE)
          {
            /* Send change mode confirmation */
            tdsrrcmcm_send_change_mode_cnf(TRUE);
            
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Init data is not called so as to keep tdsrrcmcm_est_req_saved_flag to TRUE");
            /* Reset MCM state */
            tdsrrcmcm_state = TDSRRCMCM_INITIAL;
          }
          else
          {
            /* Send change mode confirmation */
            tdsrrcmcm_send_change_mode_cnf(TRUE);
            
            /* Reset MCM state */
            tdsrrcmcm_init_data(TDSINIT_ALL);
          }
        }
        else
        {
          /* If suspend failed, error fatal for now */
          ERR_FATAL("TDS L1 Suspend failed!",0,0,0);
        }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unexpected LLC Command: %x from LLC",cmd_ptr->cmd.rrc_llc_cnf.ll_cnf);
      }
      break;

    case TDSRRC_CHANGE_MODE_IND:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DUAL:TDSRRC_CHANGE_MODE_IND is not handled currently in RRC MCM WAIT SUSPEND CNF Handler");      
      break;  

    case TDSRRC_CHANGE_MODE_REQ:
      /*we can receive a CHANGE_MODE_REQ at any state. Therefore
        save the cmd_flag to change_mode type, this can over-ride
        another other saved flag as we should be going to STOP as
        soon as we get this from MM, this will override all other
        commands*/
      if(cmd_ptr->cmd.change_mode_req.mode == TDSRRC_MODE_CHANGE)
      {
        tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_MODE_CHANGE;
      }
      else
      {
        /*  in cases when an rrc procedure does a change-mode-req to another substate
            save the change_mode_req_mode and honor it before honoring any other change
            requests when the current change_mode_req is honored
         */
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Buffering CHANGE_MODE_REQ.req_mode %d,req_proc %d,mcm_substate %d",
                 cmd_ptr->cmd.change_mode_req.mode,
                 cmd_ptr->cmd.change_mode_req.procedure,
                 tdsrrcmcm_state);
        tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = cmd_ptr->cmd.change_mode_req.mode;
        tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.proc = cmd_ptr->cmd.change_mode_req.procedure;
      }
      break;

    /* MM commands that need to be honoured in wait_suspend_cnf substate*/
    case TDSRRC_SERVICE_REQ:
      /* Store the command and act on it after we resume TDS */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_SERVICE_REQ in wait_suspend_cnf substate,buffering cmd");
      (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type)); 
      tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_SERVICE_REQ;
      break;
  
  case TDSRRC_PLMN_LIST_REQ:

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_PLMN_LIST_REQ in suspend_cnf substate");

      curr_rrc_state = tdsrrc_get_state();

      /* Do not honor PLMN_LIST_REQ in DCH/FACH states */
      if((curr_rrc_state == TDSRRC_STATE_CELL_DCH) || (curr_rrc_state == TDSRRC_STATE_CELL_FACH))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Failing PLMN_LIST_REQ bcos rrc state is DCH/FACH. rrc_state %d",curr_rrc_state);
        tdsrrcmcm_send_plmn_list_cnf_with_failure(cmd_ptr->cmd.plmn_list_req.network_select_mode,
                                               cmd_ptr->cmd.plmn_list_req.trans_id
                                               #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
                                               ,cmd_ptr->cmd.plmn_list_req.service_search
                                               #endif
                                               );
      }
      /* Also fail the PLMN_LIST_REQ if UE is in PCH states and an OOS is in progress */
      else if( ((curr_rrc_state == TDSRRC_STATE_CELL_PCH) || (curr_rrc_state == TDSRRC_STATE_URA_PCH)) &&
               (TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN == tdsrrccsp_get_curr_scan()) )
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Failing PLMN_LIST_REQ in PCH state with OOS in progress: rrc_state %d",curr_rrc_state);
        tdsrrcmcm_send_plmn_list_cnf_with_failure(cmd_ptr->cmd.plmn_list_req.network_select_mode,
                                               cmd_ptr->cmd.plmn_list_req.trans_id
                                               #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
                                               ,cmd_ptr->cmd.plmn_list_req.service_search
                                               #endif
                                               );
      }
      /* Check if the PLMN Req is Automatic.If Yes,then give priority to the ongoing Interrat procedure
      and send a plmn cnf with failure to NAS.Make sure to populate the fields correctly */
      else if(cmd_ptr->cmd.plmn_list_req.network_select_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_PLMN_LIST_REQ(Automatic mode) in wait_suspend_cnf substate.Sending failure immediately");
        tdsrrcmcm_send_plmn_list_cnf_with_failure(cmd_ptr->cmd.plmn_list_req.network_select_mode,
                                               cmd_ptr->cmd.plmn_list_req.trans_id
                                               #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
                                               ,cmd_ptr->cmd.plmn_list_req.service_search
                                               #endif
                                               );    
      }
      else if((curr_rrc_state == TDSRRC_STATE_CONNECTING) &&( tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Failing PLMN_LIST_REQ bcos rrc state is CONNECTING. rrc_state %d",curr_rrc_state);
        tdsrrcmcm_send_plmn_list_cnf_with_failure(cmd_ptr->cmd.plmn_list_req.network_select_mode,
                                               cmd_ptr->cmd.plmn_list_req.trans_id
                                               #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
                                               ,cmd_ptr->cmd.plmn_list_req.service_search
                                               #endif
                                               );
      }
      else
      {
        /* Store the command and act on it after we resume TDS */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_PLMN_LIST_REQ in wait_suspend_cnf substate,buffering cmd");
        (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type)); 
        tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_PLMN_LIST_REQ;
      }
      break;

    case TDSRRC_MODE_CHANGE_REQ:
      /* Store the command and act on it after we resume TDS */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_MODE_CHANGE_REQ in wait_suspend_cnf substate,buffering cmd");
      (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type)); 
      tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_MODE_CHANGE_REQ;
      break;

    case TDSRRC_DEACT_REQ:
      /* Store the command and act on it after we resume TDS */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_DEACT_REQ in wait_suspend_cnf substate,buffering cmd");
      (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type)); 
      tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_DEACT_REQ;
      break;

    case RRC_STOP_TDS_MODE_REQ:
       /* This would happen only when user changes preference from Auto->G only 
       * Store the command and act on it after we get the suspend_cnf 
       */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received RRC_STOP_TDS_MODE_REQ in wait_suspend_cnf substate,buffering cmd"); 
      (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type)); 
      tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = RRCMCM_CMD_STOP_TDS_REQ;

      /* Set the stop cause */
      tdsstop_cause = cmd_ptr->cmd.stop_wcdma_mode_req.cause;
      
      /* Since MM has issued this stop commnad,send a cnf to mm after we stop tds */
      tdsrrcmcm_send_stop_cnf_to_mm_flag = TRUE;

      break;
/*lint -e408*/
/*lint +e408*/
    default:
      /* Unexpected command */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected CMD: %x in State: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcmcm_state);
      break;
  }
  return;
} /* end of tdsrrcmcm_wait_suspend_cnf_substate_event_handler */


/*===========================================================================

FUNCTION   RRCMCM_WAIT_RESUME_CNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the procedure is in the Wait for RESUME Confirmation substate.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcmcm_wait_resume_cnf_substate_event_handler(
  tdsrrc_cmd_type *cmd_ptr  /* Received command */
)
{
  mm_cmd_type *mm_cmd_ptr;                    /* Pointer to the MM Command */
  boolean due_to_stop_mode = FALSE;
  
  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  { 
  case TDSRRC_LLC_CMD_CNF:
    /*if mode_change_req is received earlier, then process that command and*/
    if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_MODE_CHANGE)
    {
      tdsrrcmcm_process_mode_change_req();
    }  
    
    /* Check if a RRC_STOP_TDS_MODE_REQ is received */
    else if (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == RRCMCM_CMD_STOP_TDS_REQ) 
    {
    /* Update RRC Mode and RAT preference, and
      send the mode change indication if needed */
      tdsrrcmcm_update_mode(TDSRRC_MODE_ACTIVE);
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing buffered STOP_TDS_REQ on suspend_cnf substate");
      
      due_to_stop_mode = TRUE;
      
      /* Send a stop lower layer request to LLC */
      tdsrrcmcm_send_stop_lower_layer_req(due_to_stop_mode);
      
      /* Since MM has issued this stop commnad,send a cnf to mm after we stop tds */
      tdsrrcmcm_send_stop_cnf_to_mm_flag = TRUE;
      
      /* Clear RRC change mode requests,if any */
      tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = TDSRRC_MODE_MAX;
      
      /* Change the state to Wait for Stop Confirmation */
      tdsrrcmcm_state = TDSRRCMCM_WAIT_STOP_CNF;
    }

    else if(tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode != TDSRRC_MODE_MAX)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Pending Ch-Mode-Req to mode %d in MCM-Sub-State %d",tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode,tdsrrcmcm_state);
      
      //send change mode req and reset the cmd_store flag
      tdsrrcmcm_send_change_mode_req(tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode, 
        tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.proc);
      tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = TDSRRC_MODE_MAX;
      tdsrrcmcm_state = TDSRRCMCM_INITIAL;
    }
    /* Check for CPHY_RESUME_CNF */
    else if (cmd_ptr->cmd.rrc_llc_cnf.ll_cnf == TDSRRC_LL_CPHY_RESUME_CNF) 
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CPHY_RESUME_TDS_MODE_CNF: %d",cmd_ptr->cmd.rrc_llc_cnf.cnf.resume_cnf.status);
      {
        /* Change the RRC mode */
        tdsrrcmcm_update_mode(TDSRRC_MODE_ACTIVE);
      }
      /* Check the resume status */
      if (cmd_ptr->cmd.rrc_llc_cnf.cnf.resume_cnf.status == FALSE)
      {
        if(tdsrrccsp_send_llc_cmd(TDSRRC_RESET_L1_REQ) == FAILURE)
        {
          ERR_FATAL("RRC Unable to get cmd buf for RRC cmd 0x%x",TDSRRC_RESET_L1_REQ,0,0);
        }
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS resume failed with cause %d",cmd_ptr->cmd.rrc_llc_cnf.cnf.resume_cnf.fail_type);
        /* Update TA duration to invalid value. L1 triggers a subsequent OOS post self recovery by RRC */
        if((tdsrrccsp_get_curr_scan() == TDSRRC_CSP_UNLOCK_BY_SCAN) && ((tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH) || (tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)))
        {
          tdsl1dsds_fill_tuneaway_duration(0xFFFF);
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA           
          tdsrrc_send_rlc_ta_status(TDSRLC_LTA_STOP_IND, TDSRLC_FAIL_IND);
#endif          
        }

        if(cmd_ptr->cmd.rrc_llc_cnf.cnf.resume_cnf.fail_type == TDSL1_FAILED_NO_TRM_LOCK)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"DSIM: TDS resume failed as L1 failed to acquire lock.");
#ifdef FEATURE_SGLTE
          if(tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_SGLTE)
          {
            if ((tdsrrccsp_get_curr_scan() == TDSRRC_CSP_UNLOCK_BY_SCAN) &&
                (tdsrrc_get_oos_due_to_no_resource_flag_status()==TRUE) &&
                ((tdsrrc_get_state()==TDSRRC_STATE_CELL_DCH) ||
                (tdsrrc_get_state()==TDSRRC_STATE_CELL_FACH))
               )
            {
              boolean fake_l1_cmd = TRUE;

			  /* handle the same pending cmd as below, before sending fake rlf/oos ind */
              if((tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_SERVICE_REQ) ||
				(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_PLMN_LIST_REQ) ||
				(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_MODE_CHANGE_REQ) ||
				(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_DEACT_REQ)	)
              {
                if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_PLMN_LIST_REQ)
			    {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_SGLTE: upon resume cnf, failing pending PLMN_LIST_REQ as rrc is DCH/FACH,rrc_state %d",tdsrrc_get_state());
                  tdsrrcmcm_send_plmn_list_cnf_with_failure(tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd.cmd.plmn_list_req.network_select_mode,
                                                            tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd.cmd.plmn_list_req.trans_id
#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
                                                            ,tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd.cmd.plmn_list_req.service_search
#endif
                                                   );
				}
				else
                {
                  /* for service_req, mode_change_req, deact_req, do not send rlf/oos ind. Let legacy pending cmd handling below to do it */
                  fake_l1_cmd = FALSE;
				  tdsrrc_reset_oos_due_to_no_resource_flag();
                }
              }
              if (fake_l1_cmd)
              {
              tdsrrc_l1_cmd_type *l1_cmd_ptr;
              /* treat it as success, send drop_all to L1 to move it from suspended to idle, trigger rlf to rrc */
              tdsrrcmcm_send_change_mode_cnf(TRUE);
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_SGLTE: resume failed in rrc st %d,reset L1 and trigger rlf/oos in RRC",tdsrrc_get_state());
              /* send drop_all no cnf needed */
              tdsrrc_csp_int_data.csp_waiting_for_wl1_to_release_lock = FALSE;
              if(tdsrrccsp_send_llc_cmd(TDSRRC_RESET_L1_REQ) == FAILURE)
              {
                ERR_FATAL("RRC Unable to get cmd buf for RRC cmd 0x%x",TDSRRC_RESET_L1_REQ,0,0);
              }
              /* trigger rlf/oos */
              tdsrrcmcm_init_data(TDSINIT_ALL);
              /* Get a L1 command buffer */
              if((l1_cmd_ptr = tdsrrc_get_l1_cmd_buf()) == NULL)
              {
                ERR_FATAL("DSIM_SGLTE: No CMD buffer for RL_FAILURE_IND/OOS_IND",0,0,0);
              }
              if (tdsrrc_get_state()==TDSRRC_STATE_CELL_DCH)
              {
                l1_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CPHY_RL_FAILURE_IND;
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_SGLTE: fake TDSRRC_CPHY_RL_FAILURE_IND sent to RRC");
              }
              else if (tdsrrc_get_state()==TDSRRC_STATE_CELL_FACH)
              {
                l1_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CPHY_OUT_OF_SERVICE_AREA_IND;
                /* we need a regular oos scenario, don't set it to no_lock, otherwise it will be directed to csp */
                l1_cmd_ptr->cmd.oos_ind.oos_cause = TDSL1_FAILED_NORMAL;
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_SGLTE: fake TDSRRC_CPHY_OUT_OF_SERVICE_AREA_IND sent to RRC");
              }
              /* Put the command on the L1 cmd queue */
              tdsrrc_put_l1_cmd( l1_cmd_ptr );
              return;
            }
          }
          }
#endif
        }
        tdsrrcmcm_wl1_failure_cause = cmd_ptr->cmd.rrc_llc_cnf.cnf.resume_cnf.fail_type;

        /* If it is an inter-RAT cell reselection, send a command to CSP */
        if (tdsprocedure_id == TDSRRC_PROCEDURE_CSP)
        {
        /* reset the est_req_saved_flag to false. We don't want RCE procedure to start
        PCCPCH setup, right after a RESUME_CNF_FAILURE and even before CSP procedure 
          has had time to select a new cell.  Reset this flag and send ABORT_IND to MM */
          if(tdsrrcmcm_est_req_saved_flag == TRUE)
          {
            if(tdsrrccsp_get_curr_scan() == TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2L:WTOG: Process the buffered EST_REQ.");
              tdsrrcmcm_process_est_req(TDSRRC_MODE_ACTIVE);
            }
            else
            {
            tdsrrcmcm_est_req_saved_flag = FALSE;
            
            /* Allocate memory for RRC_ABORT_IND command to MM.. */
            if( (mm_cmd_ptr = mm_tdsrrc_get_cmd_buf(RRC_ABORT_IND)) != NULL)
            {
              /* Fill in parameters for RRC_ABORT_IND command */
              mm_cmd_ptr->cmd.rrc_abort_ind.abort_cause = RRC_INTERNAL_ERROR;
              
              /* Update the Max HFNs values to be stored in SIM or USIM */ 
              (void) tdsrrcsmc_append_hfns_to_rel_ind(mm_cmd_ptr->cmd.rrc_abort_ind.hfn_cs,
                mm_cmd_ptr->cmd.rrc_abort_cnf.hfn_ps);
              
#ifdef FEATURE_DUAL_SIM
              mm_cmd_ptr->cmd.rrc_abort_ind.as_id = tdsrrc_get_as_id();
#endif

              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Sent RRC_ABORT_IND to MM with cause: %d",RRC_CONNECTION_NOT_AVAILABLE);
              
              /* Sends the command to MM */
              if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
              {
                tdscfa_mm_put_cmd(mm_cmd_ptr);
              }
                mm_put_cmd(mm_cmd_ptr);
            }
            else   /* RRC Task failed to allocate memory */
            {
              ERR_FATAL("Unable to allocate memory for cmd: %d",RRC_ABORT_IND,0,0);
            } 
            
          }
          }
          
          /* Send change mode confirmation */
          tdsrrcmcm_send_change_mode_cnf(FALSE);
          
        }
        else if((tdsprocedure_id == TDSRRC_PROCEDURE_IHO) || (tdsprocedure_id == TDSRRC_PROCEDURE_CHO))
        {
          /* Send a change mode cnf with failure */
          tdsrrcmcm_send_change_mode_cnf(FALSE);
        }
        else
        {
          /* Currently W can be suspended only by CSP, IHO, and CHO procedures */
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: W resume for proc %d other than CSP, IHO,CHO",tdsprocedure_id);
          
          ERR_FATAL("Send resume cnf with failure to the calling proc %d",tdsprocedure_id,0,0);
        } 
      }
      /* Resume is successful */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resume Successful,reset oos_due_to_no_resource_flag");
        tdsrrc_reset_oos_due_to_no_resource_flag();
        /* IRAT reselection failed, if RRC is in connecting state, enter to idle state directly
         * Call force initalize here first which may send initiate_cell_sel_ind and then send change_mode_cnf
         * Or else, CSP may receive change_mode_cnf first which will lead to setup FACH channel
         */     
        if (tdsprocedure_id == TDSRRC_PROCEDURE_CSP && tdsrrccsp_get_curr_scan() == RRC_CSP_TDS_TO_GSM_CELL_RESELECTION_SCAN)
        {
          if (tdsrrc_get_state() == TDSRRC_STATE_CONNECTING)
          {
            tdsrrcrce_force_initialize(TRUE);
          }
        }
        tdsrrcmcm_send_change_mode_cnf(TRUE);
      }
      
      /* Process any pending request that you received in standby mode */
      if((tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_SERVICE_REQ) ||
        (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_PLMN_LIST_REQ) ||
        (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_MODE_CHANGE_REQ) ||
        (tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_DEACT_REQ)  )
      {
        /* If the buffered cmd is service req, update the dual mode flag before 
           forwarding cmd to CSP */
        if(tdsrrcmcm_cmd_store.rrcmcm_cmd_flag == TDSRRCMCM_CMD_SERVICE_REQ)
        {
#ifdef FEATURE_RAT_PRIORITY_LIST
          tdsrrcmcm_update_mode_capability_from_service_req(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd.cmd.rrc_service_req.rat_pri_list_info);
#else
          tdsrrc_dual_mode_enabled = tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd.cmd.rrc_service_req.dual_mode;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting tdsrrc_dual_mode_enabled to %d",tdsrrc_dual_mode_enabled);
#endif
        }

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Forwarding buffered MM request %d to CSP",tdsrrcmcm_cmd_store.rrcmcm_cmd_flag);
        
        /* Invoke the initial cell selection */
        tdsrrccsp_cell_selection_procedure((tdsrrc_cmd_type *) &tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd) ;
      }
      
      tdsrrcmcm_init_data(TDSINIT_ALL);
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unexpected LLC Command: %x from LLC",cmd_ptr->cmd.rrc_llc_cnf.ll_cnf);
    }
    break;  
    
    /* Mode change request from MM */
    case TDSRRC_MODE_CHANGE_REQ:
      /* Forward the request to CSP */
      tdsrrccsp_cell_selection_procedure( cmd_ptr );
      break;
      
      /* Deact req from MM */
    case TDSRRC_DEACT_REQ:
      /* Forward the request to CSP */
      tdsrrccsp_cell_selection_procedure( cmd_ptr );
      break;
      
      /* TDSRRC_SERVICE_REQ from MM */
    case TDSRRC_SERVICE_REQ:
      /* Store the command and act on it after we resume TDS */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_SERVICE_REQ in wait_resume_cnf substate,buffering cmd");
      (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type)); 
      tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_SERVICE_REQ;
      break;
      /* Mode change indication from MM */  
    case TDSRRC_CHANGE_MODE_IND:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DUAL:TDSRRC_CHANGE_MODE_IND is not handled currently in RRC MCM WAIT RESUME CNF Handler");      
      break;    
      
      /* TDSRRC_PLMN_LIST_REQ */
    case TDSRRC_PLMN_LIST_REQ:
      /* Store the command and act on it after we resume TDS */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_PLMN_LIST_REQ in wait_resume_cnf substate,buffering cmd");
      (void) memscpy(&tdsrrcmcm_cmd_store.rrcmcm_mm_req_cmd, sizeof(tdsrrc_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type)); 
      tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_PLMN_LIST_REQ;
      break;
      
    case TDSRRC_CHANGE_MODE_REQ:
    /*we can receive a CHANGE_MODE_REQ at any state. Therefore
    save the cmd_flag to change_mode type, this can over-ride
    another other saved flag as we should be going to STOP as
    soon as we get this from MM, this will override all other
      commands*/
      if(cmd_ptr->cmd.change_mode_req.mode == TDSRRC_MODE_CHANGE)
      {
        tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_MODE_CHANGE;
      }
      else
      {
      /*  in cases when an rrc procedure does a change-mode-req to another substate
      save the change_mode_req_mode and honor it before honoring any other change
      requests when the current change_mode_req is honored
        */
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Buffering CHANGE_MODE_REQ.req_mode %d,req_proc %d,mcm_substate %d",
          cmd_ptr->cmd.change_mode_req.mode,
          cmd_ptr->cmd.change_mode_req.procedure,
          tdsrrcmcm_state);
        tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = cmd_ptr->cmd.change_mode_req.mode;
        tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.proc = cmd_ptr->cmd.change_mode_req.procedure;
      }
      break;
      
    case RRC_STOP_TDS_MODE_REQ:
      /* Extract the cause of stopping TDS mode of operation */
      tdsstop_cause = cmd_ptr->cmd.stop_wcdma_mode_req.cause;
      
      /* Set Command received flag */
      tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = RRCMCM_CMD_STOP_TDS_REQ;
      break;
/*lint -e408*/
/*lint +e408*/
    default:
      /* Unexpected command */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected CMD: %x in State: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcmcm_state);
      break;
  }
  return;
} /* end of tdsrrcmcm_wait_resume_cnf_substate_event_handler */


/*===========================================================================

FUNCTION RRCMCM_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  Mode Control module. This function is called by the
  dispatcher whenever an event is received for this module.  
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
void tdsrrcmcm_event_handler(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  mm_cmd_type *mm_buf_ptr = NULL;
  tdsrrc_event_mcm_state_info_type mcm_state_event;
  tdsrrcmcm_state_e_type current_mcm_state = tdsrrcmcm_state;

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Event is received by MCM: %x at: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcmcm_state);

#ifdef FEATURE_DUAL_SIM
  /* Special handling for NAS cmd with mis-matched as-id*/
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case RRC_STOP_TDS_MODE_REQ:
      if(tdsrrc_sim_id_recvd != cmd_ptr->cmd.stop_wcdma_mode_req.as_id)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received STOP_TDS_MODE_REQ with AS ID mismatched, Send STOP_TDSCDMA_MODE_CNF to MM");
        /* Get a buffer from MM task */
        if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_STOP_TDSCDMA_MODE_CNF)) == NULL)
        {
          ERR_FATAL("No MM buffer for RRC_STOP_TDS_MODE_CNF",0,0,0);
        }     
        mm_buf_ptr->cmd.rrc_stop_wcdma_mode_cnf.as_id = cmd_ptr->cmd.stop_wcdma_mode_req.as_id;
 
        /* Put the command on MM queue */
        if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
        {
          tdscfa_mm_put_cmd(mm_buf_ptr);
        }
        mm_put_cmd(mm_buf_ptr);
        return;
      }
      break;
    case TDSRRC_DEACT_REQ:
    case TDSRRC_MODE_CHANGE_REQ:
      if(tdsrrc_sim_id_recvd != cmd_ptr->cmd.deact_req.as_id)
      {
        if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DEACT_REQ)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received DEACT_REQ with AS ID mismatched, Send RRC_DEACT_CNF to MM");
            /* SEND THE DEACT_CNF to MM */
          /* Get a buffer from MM task */
          if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_DEACT_CNF)) == NULL)
          {
            ERR_FATAL("No MM buffer for MM cmd 0x%x",RRC_DEACT_CNF,0,0);
          }
        }
        else if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MODE_CHANGE_REQ)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received MODE_CHANGE_REQ with AS ID mismatched,Send MODE_CHANGE_CNF to MM");
          
          /* SEND the RRC_MODE_CHANGE_CNF to MM */
          if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_MODE_CHANGE_CNF)) == NULL)
          {
            ERR_FATAL("No MM buffer for MM cmd 0x%x",RRC_MODE_CHANGE_CNF,0,0);
          }
        }

        /* Update the Max HFNs values to be stored in SIM or USIM */
        (void) tdsrrcsmc_append_hfns_to_rel_ind(mm_buf_ptr->cmd.rrc_deact_cnf.hfn_cs,
                                             mm_buf_ptr->cmd.rrc_deact_cnf.hfn_ps);
        
        mm_buf_ptr->cmd.rrc_deact_cnf.as_id = cmd_ptr->cmd.deact_req.as_id;

        /* Put the command on MM queue */
        if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
        {
          tdscfa_mm_put_cmd(mm_buf_ptr);
        }
        mm_put_cmd(mm_buf_ptr);
        return;
      }
      break;
      
     default:
      break;
  }
#endif
  
  /* Check the procedure substate and procedure the event accordingly */  
  switch( tdsrrcmcm_state )
  {
    case TDSRRCMCM_INITIAL:
      /* Call the event handler for Initial substate */
      tdsrrcmcm_initial_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCMCM_WAIT_START_CNF:
      /* Call the event handler for waiting for start cnf substate */
      tdsrrcmcm_wait_start_cnf_substate_event_handler( cmd_ptr );
      break;
  
    case TDSRRCMCM_WAIT_STOP_CNF:
      /* Call the event handler for wait for stop cnf substate */
      tdsrrcmcm_wait_stop_cnf_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCMCM_WAIT_SUSPEND_CNF:
      /* Call the event handler for wait for suspend substate */
      tdsrrcmcm_wait_suspend_cnf_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCMCM_WAIT_RESUME_CNF:
      /* Call the event handler for wait for resume substate */
      tdsrrcmcm_wait_resume_cnf_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCMCM_WAIT_STOP_CNF_FOR_MODE_CHANGE:
      /* Call the event handler for wait for stop cnf (Mode change) substate */
      tdsrrcmcm_wait_stop_cnf_for_mode_change_substate_event_handler( cmd_ptr );
      break;

    default:
      /* Invalid substate - should never happen. Clear
         all procedure vars */
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid MSM Substate %d",tdsrrcmcm_state);
      tdsrrcmcm_state = TDSRRCMCM_INITIAL;
      break;
  }/* end switch */

  mcm_state_event.mcm_sub_state = tdsrrcmcm_state;

  event_report_payload(EVENT_TDSCDMA_RRC_MCM_SUBSTATE, 
                       sizeof(tdsrrc_event_mcm_state_info_type), 
                       (void *)&mcm_state_event);
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDS MCM State changed(%d): %d => %d ",
           (current_mcm_state != tdsrrcmcm_state),
           current_mcm_state,
           mcm_state_event.mcm_sub_state);

  return;
}


/*===========================================================================

FUNCTION  tdsrrcmcm_update_tds_activation_cause

DESCRIPTION

  This function updates the cause of tds activation.  Activation could be
  due to either cell reselection or due to GtoW Handover
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_update_tds_activation_cause( tdsrrcmcm_activation_cause act_cause )
{
  /*update the activation cause*/
  tdsrrcmcm_act_cause = act_cause;

}/*tdsrrcmcm_update_tds_activation_cause*/
/*===========================================================================

FUNCTION  tdsrrcmcm_update_dual_mode_enabled_status

DESCRIPTION

  This function updates the status of the flag whether if dual mode is
  enabled or not.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_update_dual_mode_enabled_status( boolean status )
{
  
  /*update the dual_mode_enabled status*/
  tdsrrc_dual_mode_enabled = status;

}/*tdsrrcmcm_update_dual_mode_enabled_status*/




/*===========================================================================
FUNCTION tdsrrcmcm_get_rrc_est_cause

DESCRIPTION
  Returns establishment cause

DEPENDENCIES
  None

RETURN VALUE
  Returns establishment cause

SIDE EFFECTS
  None
===========================================================================*/
rrc_establish_cause_e_type tdsrrcmcm_get_rrc_est_cause
(
  void                  
)
{
  if(tdsrrcmcm_est_req_saved_flag)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Est Cause is %d",tdsrrcmcm_est_req.cause); 
    return tdsrrcmcm_est_req.cause;
  }
  else
  {
    return RRC_EST_TERM_UNKNOWN;
  }
} /* tdsrrcmcm_get_rrc_est_cause */

/*===========================================================================
FUNCTION rrcmcm_reset_last_act_cause

DESCRIPTION
  Resets activation cause

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcmcm_reset_last_act_cause
(
  void
)
{
  tdsrrcmcm_last_act_cause = TDSRRCMCM_ACT_CAUSE_NONE;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reset the last act rsp cause."); 
}

/*===========================================================================
FUNCTION tdsrrcmcm_send_dual_mode_info_to_meas_ueci

DESCRIPTION
  Returns None

DEPENDENCIES
  None

RETURN VALUE
  Returns establishment cause

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcmcm_send_dual_mode_info_to_meas_ueci
(
 tdsrrc_cmd_type *cmd_ptr                  
)
{
   tdsrrc_cmd_type *cmd_ptr_buff;
   
   /* Get a RRC internal command buffer */
   if((cmd_ptr_buff = tdsrrc_get_int_cmd_buf()) == NULL)
   {
     ERR_FATAL("No CMD buffer for TDSRRC_MODE_STATUS_IND",0,0,0);
    }

  /* Fill in the command id  */
  cmd_ptr_buff->cmd_hdr.cmd_id = TDSRRC_MODE_STATUS_IND;  

  /* Put the command in the internal queue */
  tdsrrc_put_int_cmd(cmd_ptr_buff );
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DUAL:TDSRRC_MODE_STATUS_IND is sent to dispatcher ");
} /* tdsrrcmcm_get_rrc_est_cause */

/*===========================================================================

FUNCTION  RRCMCM_INIT_PROCEDURE

DESCRIPTION

  This function initializes the Mode Contro module. This function should be
  only called once at power-on time, and it has to be the first module to
  be initialized.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_init_procedure( void )
{
  /* Initialize data for the module */
  tdsrrc_mode = TDSRRC_MODE_INACTIVE;
  tdsrrc_dual_mode_enabled = TRUE;
#ifdef FEATURE_CMCC_SECURE_CALL  
  tdsrrc_is_secure_call_enabled = FALSE;
#endif  
  tdsrrcmcm_notification_counter = 0;
  tdsrrcmcm_state = TDSRRCMCM_INITIAL;
  tdsrrcmcm_cmd_store.rrcmcm_cmd_flag = TDSRRCMCM_CMD_NONE;
  tdsrrcmcm_cmd_store.rrcmcm_change_mode_req.mode = TDSRRC_MODE_MAX;
  tdsrrcmcm_act_cause = TDSRRCMCM_ACT_CAUSE_NONE;
  tdsrrcmcm_last_act_cause = TDSRRCMCM_ACT_CAUSE_NONE;
  tdsrrcmcm_est_req_saved_flag=FALSE;


  return;
}

/*===========================================================================
FUNCTION tdsrrcmcm_set_reestablish_timer_out

DESCRIPTION
  Set tdsrrc_reestablish_timer_out

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcmcm_set_reestablish_timer_out(void)
{
  tdsrrc_reestablish_timer_out = TRUE;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set tdsrrc_reestablish_timer_out to TRUE."); 
}
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA 
/*===========================================================================

FUNCTION  TDSRRC_SEND_RLC_TA_STATUS

DESCRIPTION

  This function sends signal 'TDSRLC_UL_TA_IND' to RLC to indicate the START/STOP status of LTA/QTA 

DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_send_rlc_ta_status(tdsrlc_ta_ind_e_type tune_away_ind, tdsrlc_ta_success_e_type tune_away_succ)
{
  tdsl2_ul_cmd_type  *l2_ul_cmd_ptr;
  
  /* Fill in the MAC uplink parameters and send the primitive */
  if ((l2_ul_cmd_ptr = tdsl2_ul_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending TDSRLC_UL_TA_IND",0,0,0);
  }

  /* Fill in the Command Header */
  l2_ul_cmd_ptr->cmd_hdr.cmd_id = TDSRLC_UL_TA_IND;
  l2_ul_cmd_ptr->cmd_data.tune_away_info.tune_away_ind = tune_away_ind;
  l2_ul_cmd_ptr->cmd_data.tune_away_info.tune_away_succ = tune_away_succ;

	TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sending TDSRLC_UL_TA_IND  TA-Type:%d(0-LtaStart,1-LtaStop,2-QtaStart,3-QtaStop), TA-Status:%d(0-success,1-fail)",
              tune_away_ind,
              tune_away_succ); 

  tdsl2_ul_put_cmd(l2_ul_cmd_ptr);
}
#endif/*FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA */
