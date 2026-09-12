/*===========================================================================
R R C  C E L L  C H A N G E   O R D E R   P R O C E D U R E

DESCRIPTION
    This header file contains implementation for the RRC Cell Change Order
    Procedure.



EXTERNALIZED FUNCTIONS

    tdsrrccho_init_procedure
        This function initializes everything that the RRC Cell Change Order procedure
         needs to have initialized.

    tdsrrccho_procedure_event_handler
        This procedure is the event handler for the RRC Cell Change Order procedure.
        The following events are handled by this procedure:


INITIALIZATION AND SEQUENCING REQUIREMENTS
    None.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                                                EDIT HISTORY FOR MODULE

    This section contains comments describing changes made to the module.
    Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrccho.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/24/14   hx      Merge in W CR for handling abort request during CCO 
02/04/14   hx      Ignoring CCO from Utran if there is L2ACK pending for abort or SCRI-PS 
11/02/12   HX      Add EVENT for CCO from UTRAN Start and End. 
08/20/12   hx      Merged to M9615ACEHWTAAM4100184.1
03/15/12   hx      Merged to VU_MODEM_WCDMA_NIKEL_3_0_MHCI.00.00.00.00.08.04
03/10/11   hx      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
12/14/10   hx      Change two ERR_FATAL to ERR in tdsrrccho_send_cell_change_order_failure_msg.
12/10/10   hx      Move process of tdsrrc_protocol_error_reject == TRUE from 
                   tdsrrccho_validate_and_update_info_from_cco_from_utran_msg to
                   tdsrrccho_process_cell_change_order_from_utran_msg, to avoid reference
                   to NULL pointer.

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdsrrclcm.h"               /* Logical Channel Manager */
#include "tdsrrcsend.h"              /* For sending a signalling message */
#include "msg.h"
#include "err.h"
#include "tdsrrcasn1.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrccu.h"
#include "tdsrrcmcm.h"
#include "tdsrrctmr.h"
#include "tdsrrcccm.h"
#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "tdsrrcrce.h"
#include "tdsl1const.h"
#include "tdsrrcsmc.h"
#include "tdsrrcllc.h"
#include "tdsrrcdt.h"
#include "tdsrrccsp.h"
#include "tdsrrccspi.h"
#include "tdsrrccmd.h"

/*#include "rr_rrc_if.h"*/
#include "tdsrrc_rr_if.h"
#include "rr_tdsrrc_if.h"

#include "mm_umts.h"
#include "tdsrrccho.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif
#include "tdsrrcasn1util.h"

#include "tdsrrcwrm.h"
#include "tdsCFAParser.h"

#if defined(FEATURE_3GPP_FAST_DORMANCY)
#include "tdsrrcdormancy.h"
#endif

#include "tdsCFAParser_i.h"
#include "tdsrrcnv.h"
#include "tdsrrclogging.h"

#ifdef FEATURE_TDSCDMA_DEDICATED_PRI_INFO
#include "tdsrrcmeas.h"
#endif
/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* Enumerated type of the different substates for this procedure           */
typedef enum
{
  TDSRRCCHO_INITIAL,                             /* Initial Substate                    */
  TDSRRCCHO_WAIT_STANDBY_CNF,                    /* Waiting for mode change comfirmation
                                                 from MCM (to Standby)               */
  TDSRRCCHO_WAIT_CELL_CHANGE_ORDER_CNF,
  /* Waiting for Cell Change Order
     comfirmation from MM       */

  TDSRRCCHO_WAIT_CELL_CHANGE_ORDER_ABORT_CNF,
  /* Waiting for Cell change order
     abort confirmation from RR */

  TDSRRCCHO_WAIT_ACTIVE_CNF,                     /* Waiting for mode change comfirmation
                                                 from MCM (to Active)                */
  TDSRRCCHO_WAIT_CHANNEL_CONFIG_CNF,             /* Waiting for revert back to UTRAN
                                                 configuration comfirmation from LLC */
  TDSRRCCHO_CELL_UPDATE_COMPLETE_CNF,
  /* Waiting for cell update comfirmation
    from CU                         */
  TDSRRCCHO_WAIT_IRAT_IN_PROGRESS_CNF,           /* Waiting for Mode change confirm from MCM */
  TDSRRCCHO_WAIT_CAMP_CNF,                       /* Waiting for CAMP CNF from CSP */
  TDSRRCCHO_WAIT_ACTIVATION_RSP,                 /* Waiting for ACTIVATION_RSP from MM */
  TDSRRCCHO_WAIT_CONN_SETUP_IND,                 /* Waiting for CONN IND from RCE */
  TDSRRCCHO_WAIT_INACTIVE_CNF                    /* Waiting for INACTIVE_CNF from MCM */
} tdsrrccho_state_e_type;

/* Cause for Activation Indication */
typedef enum
{
  TDSRRCCHO_ACT_CAUSE_NONE,
  RRCCHO_INTERRAT_CC_ORDER_G2TD,
  RRCCHO_INTERRAT_CC_ORDER_TD2G_FAILED
} tdsrrccho_activation_cause_e_type;

/* HX: TBD. Need update/replace with tdsrr_interrat_cc_order_fail_reason_type */
/* when ICD got updated */ 
/* EVENT CCO END failure causes*/
typedef enum
{
  TDSRRCCHO_COMMAND_MSG_INVALID,
  TDSRRCCHO_CONFIGURATION_NOT_SUPPORTED,
  TDSRRCCHO_PHYSICAL_CHANNEL_FAILURE
} tdsrrccho_event_cco_end_failure_e_type;

/* Procedure status */
typedef struct
{
  tdsrrc_RRC_TransactionIdentifier transaction_id;
  tdsrr_interrat_cc_order_req_type cell_change_order_req;
  tdsrr_interrat_cc_order_abort_req_type cell_change_abort_req;
  boolean act_time_present;
  uint32 activation_time;
  uint32 failure_cause;
  tdsrrc_ProtocolErrorCause protocol_error;
  boolean cco_in_progress;                //flag to keep track whether wtog is active
  boolean cco_send_ind_to_csp;            //flag to remember to send csp ind
  tdsrr_frequence_band_e_type ota_freq_band_ind; //This stores the OTA one, as the cell_change_order_req.BCCH_ARFCN.band is calced by RR.
} tdsrrccho_procedure_info_type;

typedef struct
{
  boolean                           cc_order_in_progress;   /* GTOW CC order in progress */
  /*scr_code has been changed to CPID, waiting for RR's confirm*/
  tdsrrc_cpid_type                 cpid;               /* Scrambling code */
  tdsrrc_freq_type                     freq;                   /* UARFCN */
  rrc_plmn_identity_type            requested_plmn;         /* PLMN */
  sys_network_selection_mode_e_type network_select_mode;    /* Network Select Mode*/
  uint32                            t3174;                  /* PCCO timer */

  tdsrr_interrat_cc_order_status_e_type status;                /* Status of PCCO */
  tdsrrc_interrat_reject_reason_T failure_reason;              /* Failure cause */

} rrccho_g2td_info_type;

/* Declare a local object of type g2td_info*/
static rrccho_g2td_info_type tdsrrc_g2td_cho_info;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

#ifdef FEATURE_RRC_DELAY_CELL_CHANGE_ORDER
/* We need to introduce a delay before suspending the TDS side and moving
 * to GSM
 */
#define TDSRRCCHO_TEMP_TIMER_SIG 0x4002
rex_sigs_type tdsrrccho_sig;
rex_timer_type tdsrrccho_temp_timer;
#endif /* FEATURE_RRC_DELAY_CELL_CHANGE_ORDER */

/* RRCCHO Substate */
tdsrrccho_state_e_type tdsrrccho_substate = TDSRRCCHO_INITIAL;

/* Procedure  statue */
tdsrrccho_procedure_info_type tdsrrccho_proc_info;

/* Field to remember the Activation cause */
tdsrrccho_activation_cause_e_type tdsrrccho_act_cause = TDSRRCCHO_ACT_CAUSE_NONE;

extern boolean tdsrrc_tdrm_exchange_success;

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================

FUNCTION   tdsrrccho_initialize_internal_data

DESCRIPTION

  Initizlize internal data for WtoG and GtoW CCO

DEPENDENCIES

  None.

RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccho_initialize_internal_data
(
  void
)
{
  //set cho state to initial
  tdsrrccho_substate = TDSRRCCHO_INITIAL;

  //reset wtog cco flags
  tdsrrccho_proc_info.cco_in_progress = FALSE;
  tdsrrccho_proc_info.cco_send_ind_to_csp = FALSE;
  tdsrrccho_proc_info.act_time_present = FALSE;
  tdsrrccho_proc_info.failure_cause = T_tdsrrc_InterRAT_ChangeFailureCause_unspecified;

  //reset gtow cco flags
  tdsrrc_g2td_cho_info.cc_order_in_progress = FALSE;
  tdsrrc_g2td_cho_info.failure_reason = TDSRRC_UMTS_NONE;

  //reset GERAN sysinfo
  tdsrrccho_proc_info.cell_change_order_req.geran_sysinfo_choice = TDSRR_GERAN_INFO_TYPE_NONE;
}

/*===========================================================================

FUNCTION   tdsrrccho_send_csp_tds_resume_ind

DESCRIPTION

  Send resume-ind to CSP, if CSP queried CHO was active.
  CSP queries this only when CU asks CSP to move to Disconnected substate.

DEPENDENCIES

  None.

RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccho_send_csp_tds_resume_ind
(
  void
)
{
  tdsrrc_cmd_type *cmd_ptr;   //pointer to send internal command to csp

  if(tdsrrccho_proc_info.cco_send_ind_to_csp == TRUE)
  {

    /* Get a RRC internal command buffer */
    if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
    {
      ERR_FATAL("No CMD buf RRC_CSP_TDS_RESUMED_IND",0,0,0);
    }

    cmd_ptr->cmd_hdr.cmd_id = RRC_CSP_TDS_RESUMED_IND;

    /* Put the command on the internal queue */
    tdsrrc_put_int_cmd( cmd_ptr );
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_CSP_TDS_RESUMED_IND sent to CSP");
  }
}

/*===========================================================================

FUNCTION   RRCCHO_SEND_CHANGE_MODE_REQ

DESCRIPTION

  This function sends a TDSRRC_CHANGE_MODE_REQ to MCM to request to change the
  RRC mode to Standby.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccho_send_change_mode_req( tdsrrc_mode_e_type mode )
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
  cmd_ptr->cmd.change_mode_req.procedure = TDSRRC_PROCEDURE_CHO;

  /* Fill in the requested mode */
  cmd_ptr->cmd.change_mode_req.mode = mode;

  /* Fill reason for change mode req */
  cmd_ptr->cmd.change_mode_req.reason = TDSINTERRAT_CCO;

  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_CHANGE_MODE_REQ sent to MCM,requested mode:%d",mode);

  return;
} /* tdsrrccho_send_change_mode_req */

/*===========================================================================

FUNCTION   tdsrrccho_is_td2g_cco_active

DESCRIPTION

  This function returns true/false based on whether WtoG PCCO is active
  or not.

DEPENDENCIES

  None.

RETURN VALUE

  TRUE/FALSE depending on whether WtoG CellChangeOrder was in active or not

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrccho_is_td2g_cco_active
(
  void
)
{
  if(tdsrrccho_proc_info.cco_in_progress == TRUE)
  {
    return TRUE;
  }
  return FALSE;
}
/*===========================================================================

FUNCTION   tdsrrccho_is_td2g_cco_in_progress

DESCRIPTION

  This function returns true/false based on whether WtoG CCO is in progress
  or not.

DEPENDENCIES

  None.

RETURN VALUE

  TRUE/FALSE depending on whether WtoG CellChangeOrder was in progress or not.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrccho_is_td2g_cco_in_progress
(
  void
)
{
  /* If we are waiting for a CU cnf,GSM side is already done.
     So we can let the UE go to idle  */
  if( tdsrrccho_substate == TDSRRCCHO_CELL_UPDATE_COMPLETE_CNF )
  {
    tdsrrccho_proc_info.cco_send_ind_to_csp = FALSE;
    return FALSE;
  }
  else
  {
    if(tdsrrccho_proc_info.cco_in_progress == TRUE)
    {
      tdsrrccho_proc_info.cco_send_ind_to_csp = TRUE;
      return TRUE;
    }
    else
    {
      tdsrrccho_proc_info.cco_send_ind_to_csp = FALSE;
      return FALSE;
    }
  }
}

/*===========================================================================

FUNCTION   tdsrrccho_send_pcco_cnf

DESCRIPTION

  This function sends the TDSRRC_INTERRAT_CC_ORDER_CNF to RR with appropriate
  status(Success/Failure) and failure cause

DEPENDENCIES

  None.

RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/

static void tdsrrccho_send_pcco_cnf(void)
{
  /* Cnf command */
  tdsrrc_interrat_cc_order_cnf_type cc_order_cnf;

  memset(&cc_order_cnf,0,sizeof(tdsrrc_interrat_cc_order_cnf_type));

  /* Populate the command with status cause and failure reason(set to NONE in case of Success) */
  cc_order_cnf.status = tdsrrc_g2td_cho_info.status;
  cc_order_cnf.failure_reason = tdsrrc_g2td_cho_info.failure_reason;

  /* Send the TDSRRC_INTERRAT_CC_ORDER_CNF message to RR */  
  tdsrr_send_rrc_message_to_rr_internal((void *)(&(cc_order_cnf)), TDSRRC_INTERRAT_CC_ORDER_CNF,
                            sizeof(tdsrrc_interrat_cc_order_cnf_type)); 

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDSRRC_INTERRAT_CC_ORDER_CNF sent to RR.Status:%d,Fail_reason:%d",cc_order_cnf.status,cc_order_cnf.failure_reason);

  /*Locked when get pcco to utran message in tdsrrccho_process_pcco_to_utran_command*/
  tdsrrctmr_stop_timer(TDSRRCTMR_WRM_LOCK_TIMER);
  tdsrrcwrm_release_lock();
}

/*===========================================================================

FUNCTION   tdsrrccho_send_tl1_stop_ind

DESCRIPTION

  This function sends the TDSRR_INTERRAT_TL1_STOP_IND to RR

DEPENDENCIES

  None.

RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/

static void tdsrrccho_send_tl1_stop_ind(void)
{
  /* TL1 stop ind command */
  tdsrr_interrat_tl1_stop_ind_type tl1_stop_ind;

  memset(&tl1_stop_ind,0,sizeof(tdsrr_interrat_tl1_stop_ind_type));

  tl1_stop_ind.cause = TDSRR_TL1_STOP_IND_T2G_CCO;
  
  /* Send the TDSRR_INTERRAT_TL1_STOP_IND message to RR */  
  tdsrr_send_rrc_message_to_rr_internal((void *)(&(tl1_stop_ind)), TDSRR_INTERRAT_TL1_STOP_IND,
                            sizeof(tdsrr_interrat_tl1_stop_ind_type));

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRR_INTERRAT_TL1_STOP_IND sent to RR.Cause:%d",tl1_stop_ind.cause);

}


/*===========================================================================

FUNCTION   tdsrrccho_event_log_cco_from_utran_end

DESCRIPTION

    Event Logging for CCO from UTRAN END.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void  tdsrrccho_event_log_cco_from_utran_end
(
   uecomdef_status_e_type result,
   tdsrrccho_event_cco_end_failure_e_type failure_reason
)
{
  /* declare an event log type for cco end*/
  tdsrrclog_inter_rat_cco_end_event_type end_event_log;  

  if(result == SUCCESS)
  {
    end_event_log.tdscdma_cco_result = 1;
    end_event_log.tdscdma_cco_failure = 0xFF;
  }
  else
  {
    end_event_log.tdscdma_cco_result = 0;
    end_event_log.tdscdma_cco_failure = (uint8)failure_reason;
  }

  event_report_payload(EVENT_TDSCDMA_RRC_INTER_RAT_CCO_END,
                       sizeof(end_event_log),
                       (void *)&end_event_log);

  if(result != SUCCESS)
  {
    TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
  	                           TDS_SELF_HOSTING_MAJOR_CALL_RETENTION_EXCEPTION,
  	                           (TDS_SELF_HOSTING_MINOR_RRC_INTER_RAT_CCO_FAIL_BASE+end_event_log.tdscdma_cco_failure));
  }
}/*tdsrrciho_event_log_handover_from_utran_end*/


/*===========================================================================

FUNCTION tdsrrccho_process_cell_change_order_cnf

DESCRIPTION
    This function process the Cell Chnage Order Cnf received from the GSM

DEPENDENCIES
    None.

RETURN VALUE
    None

===========================================================================*/

static void tdsrrccho_process_cell_change_order_cnf
(
  tdsrr_interrat_cc_order_cnf_type cc_order_cnf  /* Cell Change Order Cnf */
)
{
  mm_cmd_type *mm_cmd_ptr = NULL;
  tdsrrc_out_of_lock_wait_e_type wrm_lock_status;
  boolean irat_ex_success;
  boolean irat_tx_ex_success;
  switch(cc_order_cnf.status)
  {
    case TDSCCO_SUCCESS:
    case TDSCCO_ABORTED_DUE_TO_HIGH_PRI_REQ:

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received TDSRR_INTERRAT_CC_ORDER_CNF from RR with status: %d",cc_order_cnf.status);
	  /* Change mode to Inactive */
      tdsrrccho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
      tdsrrccho_substate = TDSRRCCHO_WAIT_INACTIVE_CNF;
      
#if defined(FEATURE_3GPP_FAST_DORMANCY)
      /* Abort Dormancy if CCO proceed to successful processing. */
      tdsrrcdormancy_update_dormancy_for_srns_hho_cco(TDSRRC_PROCEDURE_CHO);
#endif      
      /*Report EVENT for CCO end*/
      tdsrrccho_event_log_cco_from_utran_end(SUCCESS, 0xFF);
      break;

    case TDSCCO_FAILURE_BEFORE_ACT_IND:

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received TDSCCO_FAILURE_BEFORE_ACT_IND from RR and th failure reason is %d ",cc_order_cnf.failure_reason);


      tdsrrccho_map_cell_change_order_failure_reason(cc_order_cnf.failure_reason);
      tdsrrc_tdrm_exchange_success = FALSE;

      if(tdsrrc_trm_exchange(&irat_ex_success, &irat_tx_ex_success))
      {
        tdscfa_dsds_add_event(TDSRRC_DSDS_EXCHANGE_LOCK);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TD2G: TDRM exchange success for T2G CCO cnf failure_before_ACT_ind");
      }

      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSIM: R&N after TRM_exchange, exchange flag: (%d, %d)",irat_ex_success, irat_tx_ex_success);
        tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_INTERRAT_IN_MS,TRUE);
        wrm_lock_status = tdsrrcwrm_acquire_wrm_lock(TRM_BG_TRAFFIC,100,TRUE);
        tdsrrcwrm_stop_lock_wait_timer();
        if(wrm_lock_status != TDSRRC_WRM_LOCK_ACQUIRED)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: RRC cannot acquire lock before resume L1 on T2G CCO cnf failure_before_ACT_ind, wrm_lock_status: %d", wrm_lock_status);
        }
      }
      tdsrrccho_send_change_mode_req(TDSRRC_MODE_ACTIVE);
      tdsrrccho_substate = TDSRRCCHO_WAIT_ACTIVE_CNF;
      /*Report EVENT for CCO end*/
      tdsrrccho_event_log_cco_from_utran_end(FAILURE, TDSRRCCHO_PHYSICAL_CHANNEL_FAILURE);
      break;

    case TDSCCO_FAILURE_AFTER_ACT_IND:

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received TDSCCO_FAILURE_AFTER_ACT_IND from RR and the failure reasonis %d",cc_order_cnf.failure_reason);
      /* After CCO abort FR, for AFTER_ACT_IND , there is no GSM cell barred reason. becasue after act ind means the failure happened after 
         GSM receive teh RR_ACT_RSP.*/
      /*And the only one reason should be T_tdsrrc_InterRAT_ChangeFailureCause_physicalChannelFailure per previous
        design.*/
      tdsrrccho_map_cell_change_order_failure_reason(cc_order_cnf.failure_reason);

      /* Send activation ind to nas*/
      if((mm_cmd_ptr = mm_tdsrrc_get_cmd_buf(RRC_ACTIVATION_IND)) == NULL)
      {
        ERR_FATAL("No MM buffer for RRC_ACTIVATION_IND",0,0,0);
      }
      mm_cmd_ptr->cmd.rrc_activation_ind.status = MM_AS_WTOG_CC_ORDER_FAILED;

#ifdef FEATURE_DUAL_SIM
      mm_cmd_ptr->cmd.rrc_activation_ind.as_id = tdsrrc_get_as_id();
#endif

      /* Put the command on MM queue */
      if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
      {
        tdscfa_mm_put_cmd(mm_cmd_ptr);
      }
      mm_put_cmd(mm_cmd_ptr);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_ACTIVATION_IND with status MM_AS_WTOG_CC_ORDER_FAILED sent to MM");

      /*update the cause for activation indication*/
      tdsrrccho_act_cause = RRCCHO_INTERRAT_CC_ORDER_TD2G_FAILED;
      tdsrrcmcm_update_tds_activation_cause(RRCMCM_INTERRAT_CC_ORDER_TD2G_FAILED);
      tdsrrccho_substate = TDSRRCCHO_WAIT_ACTIVATION_RSP;

      /*Report EVENT for CCO end*/
      tdsrrccho_event_log_cco_from_utran_end(FAILURE, TDSRRCCHO_PHYSICAL_CHANNEL_FAILURE);
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid status in cc_order_cnf command from RR:%d",cc_order_cnf.status);
      break;
  }
}

/*===========================================================================

FUNCTION tdsrrccho_map_cell_change_order_failure_reason

DESCRIPTION
    This function map the CCO failure reason.

DEPENDENCIES
    None.

RETURN VALUE
    Void

===========================================================================*/

void tdsrrccho_map_cell_change_order_failure_reason
(
  tdsrr_interrat_cc_order_fail_reason_type failure_reason    
)
{

    switch(failure_reason)
    {
      case TDSRR_GSM_CCO_OTHER_REASON:
        tdsrrccho_proc_info.failure_cause =
          T_tdsrrc_InterRAT_ChangeFailureCause_unspecified;
        break;

      case TDSRR_GSM_CCO_CELL_BARRED:
        tdsrrccsp_send_cphy_cell_bar_req(TDSRRC_CSP_GSM_CELL_BARRED, 320);
        tdsrrccho_proc_info.failure_cause =
          T_tdsrrc_InterRAT_ChangeFailureCause_configurationUnacceptable;
        break;

      case TDSRR_GSM_CCO_CELL_FORBIDDEN:
	  case TDSRR_GSM_CCO_BAND_NOT_SUPPORTED:
        tdsrrccho_proc_info.failure_cause =
          T_tdsrrc_InterRAT_ChangeFailureCause_configurationUnacceptable;
        break;

      case TDSRR_GSM_CCO_BSIC_NOT_FOUND:
      case TDSRR_GSM_CCO_BCCH_NOT_FOUND:
      case TDSRR_GSM_CCO_BCCH_DECODE_FAIL:
        tdsrrccho_proc_info.failure_cause =
          T_tdsrrc_InterRAT_ChangeFailureCause_physicalChannelFailure;
        break;

      case TDSRR_GSM_CCO_T309_EXPIRED:
	  case TDSRR_GSM_CCO_RA_FAILURE:
        tdsrrccho_proc_info.failure_cause =
          T_tdsrrc_InterRAT_ChangeFailureCause_physicalChannelFailure;
        break;

      case TDSRR_GSM_CCO_CELL_LOW_C1:
        tdsrrccho_proc_info.failure_cause =
          T_tdsrrc_InterRAT_ChangeFailureCause_physicalChannelFailure;
        break;

      case TDSRR_GSM_CCO_INVALID_STATE:
        tdsrrccho_proc_info.failure_cause =
          T_tdsrrc_InterRAT_ChangeFailureCause_unspecified;
        break;

      default:
        ERR_FATAL("Invalid failure cause",0,0,0);
        break;
    }

}/* tdsrrccho_process_cell_change_order_reject */
/*===========================================================================

FUNCTION tdsrrccho_validate_and_update_info_from_cco_from_utran_msg

DESCRIPTION
    This function validates the Cell Chnage Order from UTRAN message and
    updates suitable failure cause. Also updates Cell Change Order Request
    information from the message.

DEPENDENCIES
    None.

RETURN VALUE
    FALSE: if validation fails. Other wise TRUE.

===========================================================================*/

static boolean tdsrrccho_validate_and_update_info_from_cco_from_utran_msg
(
  tdsrrc_CellChangeOrderFromUTRAN *msg_ptr    /* Pointer to Cell Change Order */
)
{
  tdsrr_frequence_band_e_type   frequency_band;
  sys_band_T gsm_band;

/* move up to tdsrrccho_process_cell_change_order_from_utran_msg
   to protect a NULL ptr reference    
  if(tdsrrc_protocol_error_reject == TRUE)
  {
    tdsrrccho_proc_info.transaction_id = 0;
    tdsrrccho_proc_info.failure_cause =
        T_tdsrrc_InterRAT_ChangeFailureCause_protocolError;
    tdsrrccho_proc_info.protocol_error = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"ASN.1 decoding failed");
    return(FALSE);

  }
*/
  //if ordered-config is already set for any other transition, we should not trigger
  //a CCO to G as we may in the middle doing other important configurations
  //Return an error with
  if ((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET))
  {
    /* set transaction id */
    tdsrrccho_proc_info.transaction_id =
      msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs.rrc_TransactionIdentifier;
    tdsrrccho_proc_info.failure_cause =
        T_tdsrrc_InterRAT_ChangeFailureCause_protocolError;
    tdsrrccho_proc_info.protocol_error = tdsrrc_ProtocolErrorCause_messageNotCompatibleWithReceiverState;

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Simultaneous Reconfig.");
    return(FALSE);
  }

  if(msg_ptr->t == T_tdsrrc_CellChangeOrderFromUTRAN_later_than_r3)

  {
    tdsrrccho_proc_info.transaction_id = 0;
    tdsrrccho_proc_info.failure_cause =
    T_tdsrrc_InterRAT_ChangeFailureCause_protocolError;
    tdsrrccho_proc_info.protocol_error = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Critical extensions are not supported");
    return(FALSE);
  }

  if(msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs.
       interRAT_TargetCellDescription.technologySpecificInfo.t !=
       T_tdsrrc_InterRAT_TargetCellDescription_technologySpecificInfo_gsm)
  {
    /* set transaction id */
    tdsrrccho_proc_info.transaction_id =
      msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs.rrc_TransactionIdentifier;
    tdsrrccho_proc_info.failure_cause =
        T_tdsrrc_InterRAT_ChangeFailureCause_unspecified;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unsupported RAT is rcvd");
    return(FALSE);
  }

  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs,
       rab_InformationList))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RAB Info list is ignored");

  }

  /* set transaction id */
  tdsrrccho_proc_info.transaction_id =
      msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs.rrc_TransactionIdentifier;

  /* Extract the ARFCN and Freq band */
  tdsrrccho_proc_info.cell_change_order_req.BCCH_ARFCN.num = (uint16)
    msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs.interRAT_TargetCellDescription.
    technologySpecificInfo.u.gsm->bcch_ARFCN;

  if(msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs.interRAT_TargetCellDescription.
      technologySpecificInfo.u.gsm->frequency_band == tdsrrc_Frequency_Band_dcs1800BandUsed)
  {
    frequency_band = TDSRR_DCS_1800;
  }
  else
  {
    frequency_band = TDSRR_PCS_1900;
  }

  /* Check to see if this band is supported */
  if( (gsm_band = tdsrr_get_arfcn_band_internal(tdsrrccho_proc_info.cell_change_order_req.BCCH_ARFCN.num,
                                        frequency_band)) == INVALID_BAND)
  {
    /* set transaction id */
    tdsrrccho_proc_info.transaction_id = msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs.rrc_TransactionIdentifier;
    tdsrrccho_proc_info.failure_cause = T_tdsrrc_InterRAT_ChangeFailureCause_physicalChannelFailure;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unsupported GSM band in CellChangeOrderFromUTRAN Command");
    return(FALSE);
  }

  /* Fill in the band value */
  tdsrrccho_proc_info.cell_change_order_req.BCCH_ARFCN.band = gsm_band;
  tdsrrccho_proc_info.ota_freq_band_ind = frequency_band;

  tdsrrccho_proc_info.cell_change_order_req.BSIC_BCC = (uint8)
      msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs.
      interRAT_TargetCellDescription.technologySpecificInfo.u.gsm->bsic.bcc;

  tdsrrccho_proc_info.cell_change_order_req.BSIC_NCC = (uint8)
      msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs.
      interRAT_TargetCellDescription.technologySpecificInfo.u.gsm->bsic.ncc;

  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs.
       interRAT_TargetCellDescription.technologySpecificInfo.u.gsm,
       ncMode))
  {
    tdsrrccho_proc_info.cell_change_order_req.NC_Mode_present = TRUE;

    /*No Need to check for length of NC mode since it is fixed */
    switch(*(msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs.
        interRAT_TargetCellDescription.technologySpecificInfo.u.gsm->ncMode.data))
    {
      case 0:
        tdsrrccho_proc_info.cell_change_order_req.NC_Mode = TDSRR_NC_MODE_0;
        break;
      case 4:
        tdsrrccho_proc_info.cell_change_order_req.NC_Mode = TDSRR_NC_MODE_1;
        break;
      case 2:
        tdsrrccho_proc_info.cell_change_order_req.NC_Mode = TDSRR_NC_MODE_2;
        break;
      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid NC Mode");
        break;
    }
  }
  else
  {
    tdsrrccho_proc_info.cell_change_order_req.NC_Mode_present = FALSE;
  }

  tdsrrccho_proc_info.cell_change_order_req.T309_value = TDSRRC_GET_T309();

  if((tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH) &&
     (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs,
        tdsrrc_CellChangeOrderFromUTRAN_r3_IEs,activationTime))
     )
  {
    /* Set the activation time if present in CELL_DCH state */
    tdsrrccho_proc_info.act_time_present = TRUE;
    tdsrrccho_proc_info.activation_time =
    msg_ptr->u.r3.cellChangeOrderFromUTRAN_IEs.activationTime;
  }
  else
  {
    /* Activation time is not present. For CELL_FACH it should not be set by UTRAN */
    tdsrrccho_proc_info.act_time_present = FALSE;
  }

  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
    /* Check to see of non critical extensions are present */
    if(msg_ptr->u.r3.m.laterNonCriticalExtensionsPresent)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"r3_LaterNonCriticalExtensions present");

      if(msg_ptr->u.r3.laterNonCriticalExtensions.m.v590NonCriticalExtensionsPresent)
      {
        /* Have a local type to avoid long indirections */
        tdsrrc_CellChangeOrderFromUTRAN_v590ext_IEs* cco_v590ext_ie;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"v590NonCriticalExtensions present");

        cco_v590ext_ie = &(msg_ptr->u.r3.laterNonCriticalExtensions.v590NonCriticalExtensions.
                       cellChangeOrderFromUTRAN_v590ext);

        /* Check to see if NACC info is present */
        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cco_v590ext_ie,
            tdsrrc_CellChangeOrderFromUTRAN_v590ext_IEs,geran_SystemInfoType))
        {
 #ifdef FEATURE_TD2G_NACC
          tdsrrc_GERAN_SystemInformation * local_geran_SI;
          uint8 block_count = 0;

          if(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state())
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"rrc_geran_SystemInfoType is present");
            if(cco_v590ext_ie->geran_SystemInfoType.t == T_tdsrrc_CellChangeOrderFromUTRAN_v590ext_IEs_geran_SystemInfoType_sI)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Geran SI present in WTOG CCO message");

              /* Assign the SI to the local ptr so that while looping we dont tamper the links in the list */
              local_geran_SI = &cco_v590ext_ie->geran_SystemInfoType.u.sI;

              /* Set the SI type in the CCO req ommand */
              tdsrrccho_proc_info.cell_change_order_req.geran_sysinfo_choice = TDSRR_GERAN_INFO_TYPE_SI;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Geran PSI present in WTOG CCO message");

              /* Assign the SI to the local ptr so that while looping we dont tamper the links in the list */
              local_geran_SI = &cco_v590ext_ie->geran_SystemInfoType.u.pSI;

              /* Set the SI type in the CCO req ommand */
              tdsrrccho_proc_info.cell_change_order_req.geran_sysinfo_choice = TDSRR_GERAN_INFO_TYPE_PSI;
            }

            /* Extract the SI blocks and populate the cco_req */
            while(local_geran_SI->n > block_count)
            {
               memscpy(tdsrrccho_proc_info.cell_change_order_req.geran_SI[block_count].msg_string,
                     TDSRR_MAX_SI_BLOCK_LENGTH,
                     local_geran_SI->elem[block_count].data,
                     local_geran_SI->elem[block_count].numocts);

              tdsrrccho_proc_info.cell_change_order_req.geran_SI[block_count].msg_length =
                local_geran_SI->elem[block_count].numocts;

              block_count++;
            }

            /* Populate the number of blocks */
            tdsrrccho_proc_info.cell_change_order_req.num_blocks = block_count;

            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Geran Info populated.Type %d,num_blocks %d",tdsrrccho_proc_info.cell_change_order_req.geran_sysinfo_choice,tdsrrccho_proc_info.cell_change_order_req.num_blocks);
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring NACC since UE is in non-DCH state");
          }
#else
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOG NACC feature not supported");
#endif
        }
      }
    }
    else
    {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"r3 later non critical extensions not present");
    }
  }
  return(TRUE);
}/* tdsrrccho_validate_and_update_info_from_cco_from_utran_msg */
/*===========================================================================

FUNCTION   tdsrrccho_send_cell_change_order_req

DESCRIPTION

  This function sends a TDSRR_INTERRAT_CC_ORDER_REQ to GSM RR to initiate the
  Cell Change Order from UTRAN.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccho_send_cell_change_order_req( void )
{

  tdsrrccsp_fill_info_for_rr( &(tdsrrccho_proc_info.cell_change_order_req.network_select_mode));

#ifdef FEATURE_RAT_PRIORITY_LIST
  tdsrrccsp_get_rat_pri_list_info(&tdsrrccho_proc_info.cell_change_order_req.rat_pri_list_info);
#endif

#ifdef FEATURE_TDSCDMA_DEDICATED_PRI_INFO
/*copying the dedicated priority info from global structure(rrc_dedicated_priority_info) to rr_interrat_cc_order_req_type*/
  memset(&tdsrrccho_proc_info.cell_change_order_req.dedicated_priority_info,0,sizeof(lte_irat_dedicated_priority_info_s));
  tdsrrcmeas_populate_dedicated_priority_info_for_irat_req(&tdsrrccho_proc_info.cell_change_order_req.dedicated_priority_info);
#endif

  /* Send the TDSRR_INTERRAT_CC_ORDER_REQ message to RR */
  tdsrr_send_rrc_message_to_rr_internal((void *)(&(tdsrrccho_proc_info.cell_change_order_req)),
                             TDSRR_INTERRAT_CC_ORDER_REQ,
                            sizeof(tdsrr_interrat_cc_order_req_type));

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRR_INTERRAT_CC_ORDER_REQ sent to GSM RR");

  return;

} /* tdsrrccho_send_cell_change_order_req */



/*===========================================================================

FUNCTION   TDSRRCCHO_SEND_CELL_CHANGE_ORDER_ABORT_REQ

DESCRIPTION

  This function sends a RR_INTERRAT_CC_ORDER_ABORT_REQ to GSM RR to abort the
  Cell Change to GSM.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccho_send_cell_change_order_abort_req( void )
{
  tdsrr_send_rrc_message_to_rr_internal((void *)(&tdsrrccho_proc_info.cell_change_abort_req),
                                               TDSRR_INTERRAT_CC_ORDER_ABORT_REQ,
                                               sizeof(tdsrr_interrat_cc_order_abort_req_type));
	
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRR_INTERRAT_CC_ORDER_ABORT_REQ sent to GSM RR");
  return;
}



/*===========================================================================

FUNCTION   tdsrrccho_send_channel_config_req_to_llc

DESCRIPTION

  This function sends a TDSRRC_CHANNEL_CONFIG_REQ to the other RRC LLC to revert
  physical channels to previous UTRAN configuration in TDS L1.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccho_send_channel_config_req_to_llc(void)
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_CHANNEL_CONFIG_REQ",0,0,0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;

  /* Fill in procedure id */
  cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_CHO;

  /* Indicate that state change is not required */
  cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;

  /* Indicate that no confirmation is required from LLC */
  cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_CHANNEL_CONFIG_REQ sent to LLC");

  return;
} /* tdsrrccho_send_channel_config_req_to_llc */
/*===========================================================================

FUNCTION tdsrrccho_send_cell_change_order_failure_msg

DESCRIPTION
    This function builds the Cell Change Order from UTRAN Failure and
    sends the same in the uplink.

DEPENDENCIES
    None.

RETURN VALUE
    None

===========================================================================*/

static void tdsrrccho_send_cell_change_order_failure_msg
(
  void
)
{

  rlc_lc_id_type lc_id;                    /* Local variable to store
                                              RLC logical channel Id */
  tdsrrc_UL_DCCH_Message *msg = NULL;         /* Pointer to UL message */

  lc_id = tdsrrclcm_check_ul_rlc_lc_id(
            UE_LOGCHAN_DCCH,              /* Uplink Logical Channel Type. */
            DCCH_AM_RADIO_BEARER_ID,      /* Radio Bearer Id assoicated with
                                            the above Downlink Logical
                                            Channel type. */
            UE_MODE_ACKNOWLEDGED_DATA);   /* Mode of transmission for the RLC
                                            service entity. */
  if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND == lc_id)
  {/*Abuse of ERR_FATAL: lc_id may not be found after a CU procedrue*/
   /* ERR_FATAL("Can't find RLC ID for UL RB 2",0,0,0);*/
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Can't find RLC ID for UL RB 2");
  }
  else
  {
        /* Allocate a buffer for an uplink DCCH message. This buffer is released
        * by tdsrrcsend_send_ul_sdu().
        */
        msg = (tdsrrc_UL_DCCH_Message *)
              tdsrrc_malloc(sizeof (struct tdsrrc_UL_DCCH_Message));
        if (msg == NULL) {
            /* Error allocating memory for the uplink message. Print a message and
            * return failure.
            */
            ERR_FATAL("Failed to allocate memory for Proc",0,0,0);
        }

        msg->message.t = T_tdsrrc_UL_DCCH_MessageType_cellChangeOrderFromUTRANFailure;

        msg->message.u.cellChangeOrderFromUTRANFailure.t =
        T_tdsrrc_CellChangeOrderFromUTRANFailure_r3;

        /* Set the bit mask not to include non-critical extensions */
        TDSRRC_RESET_MSG_IE_PRESENT(msg->message.u.cellChangeOrderFromUTRANFailure.u.r3);

        /* Set the bit mask not to include Dummy IE */
        TDSRRC_RESET_MSG_IE_PRESENT(msg->message.u.cellChangeOrderFromUTRANFailure.u.r3.
                                    cellChangeOrderFromUTRANFailure_r3);


        /* set transaction id */
        msg->message.u.cellChangeOrderFromUTRANFailure.u.r3.
        cellChangeOrderFromUTRANFailure_r3.rrc_TransactionIdentifier =
        tdsrrccho_proc_info.transaction_id;

        /* Set the Failure cause */
        msg->message.u.cellChangeOrderFromUTRANFailure.u.r3.
        cellChangeOrderFromUTRANFailure_r3.interRAT_ChangeFailureCause.t =
        tdsrrccho_proc_info.failure_cause;

        if (tdsrrccho_proc_info.failure_cause ==
            T_tdsrrc_InterRAT_ChangeFailureCause_protocolError) {
            msg->message.u.cellChangeOrderFromUTRANFailure.u.r3.
            cellChangeOrderFromUTRANFailure_r3.interRAT_ChangeFailureCause.u.protocolError=
            rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation);

            if (msg->message.u.cellChangeOrderFromUTRANFailure.u.r3.
                cellChangeOrderFromUTRANFailure_r3.interRAT_ChangeFailureCause.u.protocolError == NULL) {
                ERR_FATAL("Memory allocation failure",0,0,0);
            }
            msg->message.u.cellChangeOrderFromUTRANFailure.u.r3.
            cellChangeOrderFromUTRANFailure_r3.interRAT_ChangeFailureCause.u.
            protocolError->diagnosticsType.t =
            T_tdsrrc_ProtocolErrorInformation_diagnosticsType_type1;

            msg->message.u.cellChangeOrderFromUTRANFailure.u.r3.
            cellChangeOrderFromUTRANFailure_r3.interRAT_ChangeFailureCause.u.protocolError->
            diagnosticsType.u.type1 = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation_type1);

            if (msg->message.u.cellChangeOrderFromUTRANFailure.u.r3.cellChangeOrderFromUTRANFailure_r3.
                interRAT_ChangeFailureCause.u.protocolError->diagnosticsType.u.type1 == NULL) {
                ERR_FATAL("Memory allocation failure",0,0,0);
            }
            /* Set the protocol error cause */
            msg->message.u.cellChangeOrderFromUTRANFailure.u.r3.
            cellChangeOrderFromUTRANFailure_r3.interRAT_ChangeFailureCause.u.
            protocolError->diagnosticsType.u.type1->protocolErrorCause =
            tdsrrccho_proc_info.protocol_error;


            /*log the protocol error*/
            tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                            (uint8)TDSRRC_PROCEDURE_CHO,
                            (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                            tdsrrc_log_map_protocol_cause(tdsrrccho_proc_info.protocol_error)
                            );

        }

        /* Send the signalling message */
        if (tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_CHO,
                                   msg,
                                   lc_id,
                                   TDSRRCSEND_L2ACK_NOT_REQUIRED
                                  ) == TDSRRCSEND_SUCCESS) 
        {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Change Order from UTRAN Failure is sent");
        } 
        else 
        {
            ERR_FATAL("failed to send Cell Change Order from UTRAN Failure",0,0,0);
            
        }
  }
   /* Reset the act_time_present flag */
  tdsrrccho_proc_info.act_time_present = FALSE;

}/* tdsrrccho_send_cell_change_order_failure_msg */


/*===========================================================================

FUNCTION   tdsrrccho_event_log_cco_from_utran_start

DESCRIPTION

    Event Logging for CCO from UTRAN command.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void  tdsrrccho_event_log_cco_from_utran_start(void)
{
  /* declare an event log type for cco start*/
  tdsrrclog_inter_rat_cco_start_event_type start_event_log;  

  if(tdsrrccho_proc_info.act_time_present)
  {
    start_event_log.activation = 1;
    start_event_log.activation_cfn = tdsrrccho_proc_info.activation_time;
  }
  else
  {
    start_event_log.activation = 0;
    start_event_log.activation_cfn = 0x00;
  }

  /*get current cfn from L1*/
  start_event_log.receive_cfn =tdsseq_get_cfn();
  start_event_log.BSIC_NCC = tdsrrccho_proc_info.cell_change_order_req.BSIC_NCC;
  start_event_log.BSIC_BCC = tdsrrccho_proc_info.cell_change_order_req.BSIC_BCC;

  start_event_log.Freq_Band = (uint8)tdsrrccho_proc_info.ota_freq_band_ind;

  start_event_log.BCCH_ARFCN = tdsrrccho_proc_info.cell_change_order_req.BCCH_ARFCN.num;

  event_report_payload(EVENT_TDSCDMA_RRC_INTER_RAT_CCO_START,
                       sizeof(start_event_log),
                       (void *)&start_event_log);
}/*tdsrrciho_event_log_handover_from_utran*/


/*===========================================================================

FUNCTION tdsrrccho_process_cell_change_order_from_utran_msg

DESCRIPTION
    This function checks whether TDSRRC_DOWNLINK_SDU_IND is for DCCH logical
    channel. If the message is received on DCCH logical channel and checks for
    Cell Chnage Order message and responds with Cell Change Order failure
    message

DEPENDENCIES
    None.

RETURN VALUE
    None

===========================================================================*/

static void tdsrrccho_process_cell_change_order_from_utran_msg
(
  tdsrrc_cmd_type *cmd_ptr                        /* Pointer to the RRC Command */
)
{

  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;  /* Pointer to DCCH message*/

  uint8 message_choice;                   /* Local varibale for message t*/

  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  /* Assert the receieved message is of type tdsrrc_DL_CCCH_Message */
  if(TDSRRC_DOWNLINK_SDU_IND == cmd_ptr->cmd_hdr.cmd_id)
  {
    if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_DCCH_Message_PDU)
    {

      message_choice =
      tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                   cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

      /*check if decoding was successful otherwise set error flag to true*/
      tdsrrc_protocol_error_reject = FALSE;

      if(cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL)
      {
        dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)
                       cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;
      } 
      else
      { /* ASN1 decoding failed */
        tdsrrc_protocol_error_reject = TRUE;
        tdsrrccho_proc_info.transaction_id = 0;
        tdsrrccho_proc_info.failure_cause =
            T_tdsrrc_InterRAT_ChangeFailureCause_protocolError;
        tdsrrccho_proc_info.protocol_error = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"ASN.1 decoding failed");
        
      }

      /* Assert the received message is Cell Change Order from UTRAN */
      if( message_choice == T_tdsrrc_DL_DCCH_MessageType_cellChangeOrderFromUTRAN )
      {
        boolean cs_open_status = FALSE;
        boolean ps_open_status = FALSE;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Change Order from UTRAN is rcvd");

        // UE can receive WtoG CHO only when PS-Signalling Connection is established.
        // If CS+PS is present, even then we should not process the CCO message.
        // According to 25.331 section 8.3.11.3, if the signaling connection status is 
        // not suitable to proceed with CCO, UE should just ignore the message and not
        // send a failure back to the network.
        ps_open_status = tdsrrcscr_get_signalling_connection_status(RRC_PS_DOMAIN_CN_ID);
        cs_open_status = tdsrrcscr_get_signalling_connection_status(RRC_CS_DOMAIN_CN_ID);

        // check based on foll. truth table condition met
        // PS  CS
        //     0   0  (ps_false error)
        //     0   1  (ps_false and also cs_true error )
        //     1   0  (ps_true and also cs_false, so don't error)
        //     1   1  (ps_true but cs_true error)
        if((ps_open_status == FALSE) || (cs_open_status == TRUE))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring CellChangeOrderFromUTRAN due to signaling connection incompatibility");
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sig conn status: PS %d,CS %d",ps_open_status,cs_open_status);
          return;
        }

        /*lint -save -e613 */
        if((tdsrrc_protocol_error_reject == TRUE)||(tdsrrccho_validate_and_update_info_from_cco_from_utran_msg(
           (tdsrrc_CellChangeOrderFromUTRAN *) &(dcch_msg_ptr->message.
           u.cellChangeOrderFromUTRAN)) == FALSE))
        {
          if(rrc_state == TDSRRC_STATE_CELL_FACH)
          {
            if(TDSRRCCU_NOT_COMPLETED ==
               tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_CHO) )
            {
              /* CELL UPDATE is in process of getting a valid C-RNTI. */
              tdsrrccho_substate = TDSRRCCHO_CELL_UPDATE_COMPLETE_CNF;
            }
            else
            {
              tdsrrccho_send_cell_change_order_failure_msg();
            }
          }
          else if(rrc_state == TDSRRC_STATE_CELL_DCH)
          {
            tdsrrccho_send_cell_change_order_failure_msg();
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"This should not happen");
          }

        }
        /*lint -restore */
        else if (tdsrrcmcm_is_dualmode_enabled() == FALSE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Dual-mode is not enabled");
          /* UE is not enabled for dual-mode, send failure to UTRAN */
          tdsrrccho_proc_info.failure_cause =
            T_tdsrrc_InterRAT_HO_FailureCause_configurationUnacceptable;

          if(TDSRRCCU_NOT_COMPLETED ==
               tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_CHO) )
          {
            /* CELL UPDATE is in process of getting a valid C-RNTI. */
            tdsrrccho_substate = TDSRRCCHO_CELL_UPDATE_COMPLETE_CNF;
          }
          else
          {
            tdsrrccho_send_cell_change_order_failure_msg();
          }
        }
        /*Check if the SCRI has been sent and RRC is in wait for L2ACK state  * 
          *dormancy triggered SCRI either via DT or Dormancy itself           *
          *soft-abort triggered by NAS                                        */
        else if((TRUE == tdsrrcrce_get_rrc_abort_cnf_status()) 
                || tdsrrcdormancy_is_dormancy_active())
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"There is either soft-abort or SCRI-PS pending on L2ACK");
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Reject IHO via physical channel failure");
          tdsrrccho_proc_info.failure_cause = T_tdsrrc_InterRAT_ChangeFailureCause_physicalChannelFailure;
          tdsrrccho_send_cell_change_order_failure_msg();
          
        }
        else
        {
          /* Request MCM to change the RRC mode from Active to Standby */
#ifdef FEATURE_RRC_DELAY_CELL_CHANGE_ORDER

          /* If the activation time is not present delay acting on the CHO message
           * by 60 ms
           */
          if(!tdsrrccho_proc_info.act_time_present)
          {
            /* Start a timer for 100 milli seconds, so that the
               base station can get RLC L2 Ack for Cell Change Order message. */

            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Delaying SUSPEND for CHO by 100 ms");
            (void) rex_clr_sigs(rex_self(), TDSRRCCHO_TEMP_TIMER_SIG);
            (void) rex_set_timer(&tdsrrccho_temp_timer, 100);

            tdsrrccho_sig = rex_wait(TDSRRCCHO_TEMP_TIMER_SIG);
            TDSRRC_MSG1(MSG_LEGACY_LOW,"tdsrrccho_sig %d",tdsrrccho_sig);
            (void) rex_clr_sigs(rex_self(), TDSRRCCHO_TEMP_TIMER_SIG);
          }

#endif /* FEATURE_RRC_DELAY_CELL_CHANGE_ORDER */

          /* Abort BPLMN if it is active before sending the cco req to GSM side*/
          if(TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
          {
            
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN active during WTOG CHO. Abort BPLMN procedure");

            tdsrrccsp_cleanup_bplmn_state();
          }

          tdsrrccho_event_log_cco_from_utran_start();
          tdsrrccho_send_change_mode_req(TDSRRC_MODE_STANDBY);
          tdsrrccho_substate = TDSRRCCHO_WAIT_STANDBY_CNF;
          tdsrrccho_proc_info.cco_in_progress = TRUE;
        }
      }
    } /* Assertion for SDU type */
    else
    {
      /* This should never happen */
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid SDU type is fwded to CHO Proc: %d",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Event: %d is forwarded to CHO Proc",cmd_ptr->cmd_hdr.cmd_id);
  }
}/* tdsrrccho_process_cell_change_order_from_utran_msg */
/*===========================================================================

FUNCTION tdsrrccho_process_cho_initial_substate

DESCRIPTION
  This function processes all events that are dispatched to Cell Change
  Order procedure in TDSRRCCHO_INITIAL substate of either CELL_FACH or
    CELL_DCH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrccho_process_cho_initial_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_DOWNLINK_SDU_IND:
      tdsrrccho_process_cell_change_order_from_utran_msg(cmd_ptr);
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccho_substate);
      break;
  }
} /* tdsrrccho_process_cho_initial_substate */
/*===========================================================================

FUNCTION tdsrrccho_process_cho_wait_standby_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to Cell Change
  Order procedure in TDSRRCCHO_WAIT_STANDBY_CNF substate of either CELL_FACH or
    CELL_DCH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrccho_process_cho_wait_standby_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{

  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* Change mode confirmation from the MCM */
    case TDSRRC_CHANGE_MODE_CNF:

      if(!tdrm_check_cli_state_is_in_lock(TDRM_CLIENT_RRC))
	  {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"DSIM: RRC does not have lock before sending T2G CCO req!");
	  }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Change priority to TRM_BG_TRAFFIC for T2G CCO");

      tdsrrc_wrm_change_priority(TRM_BG_TRAFFIC,TRUE);
      tdsrrc_trm_exchange_out(TDSRRC_CSP_INTERRAT_GSM);
      tdsrrcwrm_release_lock();

      /* Initiate Cell Change Order from UTRAN */
      tdsrrccho_send_cell_change_order_req();

      /* Change the procedure state to wait for Cell Change Order confirmation */
      tdsrrccho_substate = TDSRRCCHO_WAIT_CELL_CHANGE_ORDER_CNF;
      break;

    case TDSRRC_IRAT_ABORT_IND:
      tdsrrccho_send_change_mode_req(TDSRRC_MODE_ACTIVE);
      tdsrrccho_substate = TDSRRCCHO_WAIT_ACTIVE_CNF;
      break;

    default:
      /* No other events are normal in this
      substate */
      /*HX: TDSRRC_ABORT_REQ and TDSRRC_STATE_CHANGE_IND may need to be considered?*/
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected event %x in state %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccho_substate);
      break;
  }

} /* tdsrrccho_process_cho_wait_standby_cnf_substate */
/*===========================================================================

FUNCTION tdsrrccho_process_cho_wait_cell_change_order_cnf

DESCRIPTION
  This function processes all events that are dispatched to Cell Change
  Order procedure in TDSRRCCHO_WAIT_CELL_CHANGE_ORDER_CNF substate of
    either CELL_FACH or CELL_DCH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrccho_process_cho_wait_cell_change_order_cnf
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  tdsrrc_out_of_lock_wait_e_type wrm_lock_status;
  boolean irat_ex_success;
  boolean irat_tx_ex_success;

  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_IRAT_ABORT_IND)
  {
    tdsrrccho_send_cell_change_order_abort_req();
    tdsrrccho_substate = TDSRRCCHO_WAIT_CELL_CHANGE_ORDER_ABORT_CNF;
  }
  else
  {

      /* Switch on the received command. */
    switch( cmd_ptr->cmd_hdr.cmd_id )
    {
      /*lint -e408*/
      case TDSRR_INTERRAT_CC_ORDER_CNF:
        /*lint +e408*/
        /*lint -e740*/
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received RR_INTERRAT_CC_ORDER_CNF from RR");
        tdsrrccho_process_cell_change_order_cnf(((tdsrrc_rr_cmd_type *) cmd_ptr)->cmd.interrat_cc_order_cnf);
        /*lint +e740*/
        break;

      /*After CCO ABORT FR, there is no  TDSRR_INTERRAT_CC_ORDER_REJ between TDS and GSM*/
      /* Cell Change Order Reject from GSM */
      /*lint -e408*/
      case TDSRR_INTERRAT_CC_ORDER_REJ:
        /*lint +e408*/
        /* GSM Rejected Cell Change Order Request. So Continue in TDS */
        /*lint -e740*/
        tdsrrccho_map_cell_change_order_failure_reason(
          ((tdsrrc_rr_cmd_type *) cmd_ptr)->cmd.interrat_cc_order_rej.failure_reason);

        tdsrrc_tdrm_exchange_success = FALSE;
        if(tdsrrc_trm_exchange(&irat_ex_success, &irat_tx_ex_success))      
        {
          tdscfa_dsds_add_event(TDSRRC_DSDS_EXCHANGE_LOCK);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2G: TDRM exchange success for T2G CCO reject");
        }

        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSIM: R&N after TRM_exchange, exchange flag: (%d, %d)",irat_ex_success, irat_tx_ex_success);
          tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_INTERRAT_IN_MS,TRUE);
          wrm_lock_status = tdsrrcwrm_acquire_wrm_lock(TRM_BG_TRAFFIC,100,TRUE);
          tdsrrcwrm_stop_lock_wait_timer();
          if(wrm_lock_status != TDSRRC_WRM_LOCK_ACQUIRED)
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: RRC cannot acquire lock before resume L1 on T2G CCO reject, wrm_lock_status: %d", wrm_lock_status);
          }
        }
		
        /*lint +e740*/

        /* Request MCM to change the RRC mode from Standby to Active */
        tdsrrccho_send_change_mode_req(TDSRRC_MODE_ACTIVE);

        tdsrrccho_substate = TDSRRCCHO_WAIT_ACTIVE_CNF;
        break;

      default:
        /* No other events are normal in this
        substate */
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected event %x in state %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccho_substate);
        break;
    }

  }
} /* tdsrrccho_process_cho_wait_cell_change_order_cnf */


/*===========================================================================

FUNCTION tdsrrccho_process_cell_change_order_abort_cnf

DESCRIPTION
  This function processes the RR_INTERRAT_CC_ORDER_CNF received in 
  TDSRRCCHO_WAIT_CELL_CHANGE_ORDER_ABORT_CNF substate of either CELL_FACH 
  or CELL_DCH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrccho_process_cell_change_order_abort_cnf
(
  tdsrr_interrat_cc_order_abort_cnf_type cc_order_abort_cnf  /* Cell Change Order Cnf */
)
{
  tdsrrc_out_of_lock_wait_e_type wrm_lock_status;
  boolean irat_ex_success;
  boolean irat_tx_ex_success;

  switch(cc_order_abort_cnf.abort_status)
  {  
    case TDSRRC_RR_SUCCESS:
      /* Set CCO fail cause as "unspecified"*/
      tdsrrccho_proc_info.failure_cause = T_tdsrrc_InterRAT_ChangeFailureCause_unspecified;
      tdsrrc_tdrm_exchange_success = FALSE;
      if(tdsrrc_trm_exchange(&irat_ex_success,&irat_tx_ex_success))   
      {
        tdscfa_dsds_add_event(TDSRRC_DSDS_EXCHANGE_LOCK);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2G: TDRM exchange success for T2G CCO abort cnf with success");
      }

      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSIM: R&N after TRM_exchange, exchange flag: (%d, %d)",irat_ex_success, irat_tx_ex_success);
        tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_INTERRAT_IN_MS,TRUE);
        wrm_lock_status = tdsrrcwrm_acquire_wrm_lock(TRM_BG_TRAFFIC,100,TRUE);
        tdsrrcwrm_stop_lock_wait_timer();
        if(wrm_lock_status != TDSRRC_WRM_LOCK_ACQUIRED)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: RRC cannot acquire lock before resume L1 on T2G CCO abort cnf with failure_before_ACT_ind, wrm_lock_status: %d", wrm_lock_status);
        }
      }
      tdsrrccho_send_change_mode_req(TDSRRC_MODE_ACTIVE);
      tdsrrccho_substate = TDSRRCCHO_WAIT_ACTIVE_CNF;
      break;

    default:
      ERR_FATAL("Invalid status in cc_order_abort_cnf command from RR:%d",cc_order_abort_cnf.abort_status,0,0);
      break;
  }
}

/* tdsrrccho_process_cho_wait_cell_change_order_cnf */
/*===========================================================================

FUNCTION tdsrrccho_process_cho_wait_cell_change_order_abort_cnf

DESCRIPTION
  This function processes all events that are dispatched to Cell Change
  Order procedure in RRCCHO_WAIT_CELL_CHANGE_ORDER_ABORT_CNF substate 
  of either CELL_FACH or CELL_DCH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrccho_process_cho_wait_cell_change_order_abort_cnf
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
    /* Switch on the received command. */
  switch(cmd_ptr->cmd_hdr.cmd_id )
  {
    /*lint -e408*/
    case TDSRR_INTERRAT_CC_ORDER_ABORT_CNF:
      /*lint +e408*/
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRR_INTERRAT_CC_ORDER_ABORT_CNF from RR");
      /*lint -e740*/
      tdsrrccho_process_cell_change_order_abort_cnf(((tdsrrc_rr_cmd_type *) cmd_ptr)->cmd.interrat_cc_order_abort_cnf);
      /*lint +e740*/
      break;
    /* Cell Change Order Reject from GSM */
    /*lint -e408*/
    case TDSRR_INTERRAT_CC_ORDER_CNF:
    case TDSRR_INTERRAT_CC_ORDER_REJ:
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Waiting for CC_ORDER_ABORT_CNF,ignore %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccho_substate);
      break;

    default:
      /* No other events are normal in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected event %x in state %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccho_substate);
      break;
  }

} 


/*===========================================================================

FUNCTION tdsrrccho_process_cho_wait_active_cnf

DESCRIPTION
  This function processes all events that are dispatched to Cell Change
  Order procedure in TDSRRCCHO_WAIT_ACTIVE_CNF substate of either CELL_FACH or
    CELL_DCH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrccho_process_cho_wait_active_cnf
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  tdsrrc_cmd_type *int_cmd_ptr;

  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* Change mode confirmation from the MCM */
    case TDSRRC_CHANGE_MODE_CNF:
      if(tdsrrccho_proc_info.cco_send_ind_to_csp == TRUE)
      {
        //send csp resume-ind if needed
        tdsrrccho_send_csp_tds_resume_ind();
        tdsrrccho_initialize_internal_data();
      }
      else
      {
        if(cmd_ptr->cmd.change_mode_cnf.status == TRUE)
        {
          tdsrrccho_send_channel_config_req_to_llc();
          tdsrrccho_substate = TDSRRCCHO_WAIT_CHANNEL_CONFIG_CNF;
        }
        else /* change mode failure */
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx: TDSRRC_CHANGE_MODE_CNF (Failure) ");
          if(TDSRRC_STATE_CELL_FACH == tdsrrc_get_state())
          {
            if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_CHO) )
            {
              /* CELL UPDATE is in process of getting a valid C-RNTI. */
              tdsrrccho_substate = TDSRRCCHO_CELL_UPDATE_COMPLETE_CNF;
            }
            else //there is no cell update process currently active.
            {
              if ((int_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
              {
                int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
                int_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_PHY_CHAN_FAILURE;
                int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = TRUE;
                int_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_CHO;

                TDSRRC_MSG0(MSG_LEGACY_HIGH,"CELL_UPDATE_INITIATE_REQ sent by CHO on resume failure in CELL_FACH state");
                tdsrrc_put_int_cmd(int_cmd_ptr);

                tdsrrccho_substate = TDSRRCCHO_CELL_UPDATE_COMPLETE_CNF;
              }
              else
              {
                ERR_FATAL("No CMD buf CELL_UPDATE_INITIATE_REQ",0,0,0);
              }
            }
          }
          else //we are in cell_dch
          {
            if ((int_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
            {
              int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
              int_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_RADIO_LINK_FAILURE;
              int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = TRUE;
              int_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_CHO;

              TDSRRC_MSG0(MSG_LEGACY_HIGH,"CELL_UPDATE_INITIATE_REQ sent by CHO on resume failure in CELL_DCH state");
              tdsrrc_put_int_cmd(int_cmd_ptr);

              tdsrrccho_substate = TDSRRCCHO_CELL_UPDATE_COMPLETE_CNF;
            }
            else
            {
              ERR_FATAL("No CMD buf CELL_UPDATE_INITIATE_REQ",0,0,0);
            }
          }
        }
      }

    break;
    default:
      /* No other events are normal in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected event %x in state %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccho_substate);
      break;
  }
} /* tdsrrccho_process_cho_wait_active_cnf */
/*===========================================================================

FUNCTION tdsrrccho_process_cho_wait_channel_config_cnf

DESCRIPTION
  This function processes all events that are dispatched to Cell Change
  Order procedure in TDSRRCCHO_WAIT_CHANNEL_CONFIG_CNF substate of either
  CELL_FACH or CELL_DCH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrccho_process_cho_wait_channel_config_cnf
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  tdsrrc_cmd_type *int_cmd_ptr = NULL;

  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* Change mode confirmation from the MCM */
    case TDSRRC_CHANNEL_CONFIG_CNF:
      if(tdsrrccho_proc_info.cco_send_ind_to_csp == TRUE)
      {
        //send csp resume-ind if needed
        tdsrrccho_send_csp_tds_resume_ind();
        tdsrrccho_initialize_internal_data();
      }
      else
      {
        if (cmd_ptr->cmd.chan_config_cnf.chan_cfg_status == TDSRRCLLC_CHAN_CFG_SUCCESS)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx: TDSRRC_CHANNEL_CONFIG_CNF (Success) ");
          if(TDSRRC_STATE_CELL_FACH == tdsrrc_get_state())
          {
            /*  Send Cell Change Order Reject in the UL*/
            if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_CHO) )
            {
              /* CELL UPDATE is in process of getting a valid C-RNTI. */
              tdsrrccho_substate = TDSRRCCHO_CELL_UPDATE_COMPLETE_CNF;
            }
            else
            {
              tdsrrccho_send_cell_change_order_failure_msg();
              tdsrrccho_initialize_internal_data();
            }
          }
          else //we are in cell_dch
          {
            tdsrrccho_send_cell_change_order_failure_msg();
            tdsrrccho_initialize_internal_data();
          }
        }
        else //channel config failed.Try to do a cell update
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx: TDSRRC_CHANNEL_CONFIG_CNF (Failure) ");
          if(TDSRRC_STATE_CELL_FACH == tdsrrc_get_state())
          {
            if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_CHO) )
            {
              /* CELL UPDATE is in process of getting a valid C-RNTI. */
              tdsrrccho_substate = TDSRRCCHO_CELL_UPDATE_COMPLETE_CNF;
            }
            else //there is no cell update process currently active.
            {
              if ((int_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
              {
                int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
                int_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_PHY_CHAN_FAILURE;
                int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = TRUE;
                int_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_CHO;

                TDSRRC_MSG0(MSG_LEGACY_HIGH,"CELL_UPDATE_INITIATE_REQ sent by CHO on resume failure in CELL_FACH state");
                tdsrrc_put_int_cmd(int_cmd_ptr);

                tdsrrccho_substate = TDSRRCCHO_CELL_UPDATE_COMPLETE_CNF;
              }
              else
              {
                ERR_FATAL("No CMD buf CELL_UPDATE_INITIATE_REQ",0,0,0);
              }
            }
          }
          else //we are in cell_dch
          {
            if ((int_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
            {
              int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
              int_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_RADIO_LINK_FAILURE;
              int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = TRUE;
              int_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_CHO;

              TDSRRC_MSG0(MSG_LEGACY_HIGH,"CELL_UPDATE_INITIATE_REQ sent by CHO on resume failure in CELL_DCH state");
              tdsrrc_put_int_cmd(int_cmd_ptr);

              tdsrrccho_substate = TDSRRCCHO_CELL_UPDATE_COMPLETE_CNF;
            }
            else
            {
              ERR_FATAL("No CMD buf CELL_UPDATE_INITIATE_REQ",0,0,0);
            }
          }
        }
      }
     break;

    default:
      /* No other events are normal in this substate */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected event %x in state %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccho_substate);
      break;
  }
} /* tdsrrccho_process_cho_wait_channel_config_cnf */
/*===========================================================================

FUNCTION tdsrrccho_process_cho_cell_update_cnf

DESCRIPTION
  This function processes all events that are dispatched to Cell Change
  Order procedure in RRCCHO_CELL_UPDATE_CNF substate of either
    CELL_FACH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrccho_process_cho_cell_update_cnf
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  tdsrrc_cmd_type *int_cmd_ptr = NULL;
  tdsrrc_state_e_type rrc_state;

  rrc_state = tdsrrc_get_state();

  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* Cell Update Completed confirmation */
    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      if(tdsrrccho_proc_info.cco_send_ind_to_csp == TRUE)
      {
        //send csp resume-ind if needed
        tdsrrccho_send_csp_tds_resume_ind();
        tdsrrccho_initialize_internal_data();
      }
      else
      {
        if( (TDSRRC_STATE_CELL_PCH == rrc_state) || (TDSRRC_STATE_URA_PCH == rrc_state) )
        {
          // Initiate a cell update with cause TDSRRC_UL_DATA_TRANSMISSION
          if ((int_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
          {
            int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
            int_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_UL_DATA_TRANSMISSION;
            int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = TRUE;
            int_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_CHO;

            TDSRRC_MSG0(MSG_LEGACY_HIGH,"CELL_UPDATE_INITIATE_REQ sent by CHO on resume failure.Cause:TDSRRC_UL_DATA_TRANSMISSION ");
            tdsrrc_put_int_cmd(int_cmd_ptr);
          }
          else
          {
            ERR_FATAL("No CMD buf CELL_UPDATE_INITIATE_REQ",0,0,0);
          }
        }
        else  // We are in cell_dch/cell_fach
        {
          /* Send Cell Change Order Reject in the UL*/
          tdsrrccho_send_cell_change_order_failure_msg();
          tdsrrccho_initialize_internal_data();
        }
      }
      break;

    default:
      /* No other events are normal in this substate */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected event %x in state %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccho_substate);
      break;
  }

} /* tdsrrccho_process_cho_cell_update_cnf */

/*===========================================================================

FUNCTION  tdsrrccho_process_wait_activation_rsp_event_hdlr

DESCRIPTION

  This function handles the events received by this module when
  the procedure is Waiting for Activation Rsp from MM.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccho_process_wait_activation_rsp_event_hdlr( tdsrrc_cmd_type *cmd_ptr )
{
  tdsrrc_out_of_lock_wait_e_type wrm_lock_status;
  boolean irat_ex_success; 
  boolean irat_tx_ex_success;

  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_INTERRAT_CC_ORDER_ABORT_REQ)
  {
    tdsrrc_g2td_cho_info.status = TDSCCO_FAILURE_AFTER_ACT_IND;
    tdsrrc_g2td_cho_info.failure_reason = TDSRRC_UMTS_NONE;
    tdsrrccho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
    /* go to inactive substate so that we abort TDSCDMA*/
    tdsrrccho_substate = TDSRRCCHO_WAIT_INACTIVE_CNF;
  }
  else if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_GRR_INTERRAT_PROC_ABORT_IND)
  {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received RR_INTERRAT_PROC_ABORT_IND from RR,clear connection and deactive");
	 /* Change mode to Inactive */
     tdsrrccho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
     tdsrrccho_substate = TDSRRCCHO_WAIT_INACTIVE_CNF;
      
#if defined(FEATURE_3GPP_FAST_DORMANCY)
     /* Abort Dormancy if CCO proceed to successful processing. */
     tdsrrcdormancy_update_dormancy_for_srns_hho_cco(TDSRRC_PROCEDURE_CHO);
#endif      

  }
  else
  {
    /* Switch on the received command. */
    switch( cmd_ptr->cmd_hdr.cmd_id )
    {
      case TDSRRC_ACTIVATION_RSP:

        if(tdsrrccho_act_cause == RRCCHO_INTERRAT_CC_ORDER_TD2G_FAILED)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rxed: TDSRRC_ACTIVATION_RSP after WTOG_CC_ORDER Failure");

          /* Send a service ind to MM.MM should not send any RAU/EST_REQ */
          tdsrrccsp_send_backup_service_ind();
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_SERVICE_IND sent to MM");

          tdsrrc_tdrm_exchange_success = FALSE;
          if(tdsrrc_trm_exchange(&irat_ex_success, &irat_tx_ex_success))
          {
            tdscfa_dsds_add_event(TDSRRC_DSDS_EXCHANGE_LOCK);
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2G: TDRM exchange success for T2G CCO cnf failure_after_ACT_ind");
          }

          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSIM: R&N after TRM_exchange, exchange flag: (%d, %d)",irat_ex_success, irat_tx_ex_success);
            tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_INTERRAT_IN_MS,TRUE);
            wrm_lock_status = tdsrrcwrm_acquire_wrm_lock(TRM_BG_TRAFFIC,100,TRUE);
            tdsrrcwrm_stop_lock_wait_timer();
            if(wrm_lock_status != TDSRRC_WRM_LOCK_ACQUIRED)
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: RRC cannot acquire lock before resume L1 on T2G CCO cnf failure_after_ACT_ind, wrm_lock_status: %d", wrm_lock_status);
            }
          }
          /* Resume TDS mode */
          tdsrrccho_send_change_mode_req(TDSRRC_MODE_ACTIVE);

          /* In case of CCO failure send RABM_EST_IND to upper layes */
          tdsrrcrbe_send_rabm_est_ind_to_upper_layers();

          tdsrrccho_substate = TDSRRCCHO_WAIT_ACTIVE_CNF;
        }
        else if(tdsrrccho_act_cause == RRCCHO_INTERRAT_CC_ORDER_G2TD)
        {
          /* go to initial substate */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rxed: TDSRRC_ACTIVATION_RSP for GTOW_CC_ORDER");

          /* Send a service ind to MM.This would trigger RAU from MM */
          tdsrrccsp_send_service_ind();
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_SERVICE_IND sent to MM");

          /* In case of CCO failure send RABM_EST_IND to upper layes */
          tdsrrcrbe_send_rabm_est_ind_to_upper_layers();

          /* Wait for RRC_CONN_SETUP_IND */
          tdsrrccho_substate = TDSRRCCHO_WAIT_CONN_SETUP_IND;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rxed: TDSRRC_ACTIVATION_RSP for unknown cause");
          tdsrrccho_initialize_internal_data();
        }

        /* Reset the act_cause variable */
        tdsrrccho_act_cause = TDSRRCCHO_ACT_CAUSE_NONE;
        break;

      default:
        /* Unexpected command */
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected cmd %x in CHO state %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccho_substate);
        break;
    }
  }
} /* tdsrrccho_process_wait_activation_rsp_event_hdlr */

/*===========================================================================

FUNCTION tdsrrccho_process_cell_dch_state 
 
    Merged to FACH State 
==========================================================================*/

/*===========================================================================

FUNCTION tdsrrccho_process_cell_fach_state

DESCRIPTION
  This function processes all events that are dispatched to Cell Change
  Order from UTRAN procedure in either CELL_FACH state of Connected  mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrccho_process_cell_fach_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  switch (tdsrrccho_substate)
  {
    case TDSRRCCHO_INITIAL:
      tdsrrccho_process_cho_initial_substate(cmd_ptr);
      break;
    case TDSRRCCHO_WAIT_STANDBY_CNF:
      tdsrrccho_process_cho_wait_standby_cnf_substate(cmd_ptr);
      break;
    case TDSRRCCHO_WAIT_CELL_CHANGE_ORDER_CNF:
      tdsrrccho_process_cho_wait_cell_change_order_cnf(cmd_ptr);
      break;
    case TDSRRCCHO_WAIT_ACTIVE_CNF:
      tdsrrccho_process_cho_wait_active_cnf(cmd_ptr);
      break;
    case TDSRRCCHO_CELL_UPDATE_COMPLETE_CNF:
      tdsrrccho_process_cho_cell_update_cnf(cmd_ptr);
      break;
    case TDSRRCCHO_WAIT_CHANNEL_CONFIG_CNF:
      tdsrrccho_process_cho_wait_channel_config_cnf(cmd_ptr);
      break;
    case TDSRRCCHO_WAIT_ACTIVATION_RSP:
      tdsrrccho_process_wait_activation_rsp_event_hdlr(cmd_ptr);
      break;
    case TDSRRCCHO_WAIT_CELL_CHANGE_ORDER_ABORT_CNF:
      tdsrrccho_process_cho_wait_cell_change_order_abort_cnf(cmd_ptr);
    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid command in RRC State %d,sub-state: %d",rrc_state,tdsrrccho_substate);
      break;
  }
} /* tdsrrccho_process_cell_fach_state */


/*===========================================================================

FUNCTION   tdsrrccho_get_activation_time_for_td2g_cho

DESCRIPTION

  This function populates activation time for WtoG cell change order into the received
  ptr and also returns success/failure if activation is present or not.

DEPENDENCIES

  None.

RETURN VALUE

  SUCCESS/FAILURE depending if Activation Time was appended or not.

SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type tdsrrccho_get_activation_time_for_td2g_cho
(
uint16 * act_time
)
{
  /*if activation time was present in the message then send the same*/
  if(tdsrrccho_proc_info.act_time_present == TRUE)
  {
    *act_time = (uint16)(tdsrrccho_proc_info.activation_time);
    return SUCCESS;
  }
  else
  {
    return FAILURE;
  }
} /* tdsrrccho_get_activation_time_for_td2g_cho */


/*===========================================================================

FUNCTION   tdsrrccho_is_g2td_pcco_in_progress

DESCRIPTION

  This function returns true/false based on whether GtoW PCCO is in progress
  or not.

DEPENDENCIES

  None.

RETURN VALUE

  TRUE/FALSE depending if Activation Time was appended or not.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrccho_is_g2td_pcco_in_progress(void)
{
  return tdsrrc_g2td_cho_info.cc_order_in_progress;
}

/*===========================================================================

FUNCTION   tdsrrccho_process_wait_inactive_cnf_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the procedure is Waiting for Inactive Confirmation from MCM

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccho_process_wait_inactive_cnf_event_handler( tdsrrc_cmd_type *cmd_ptr )
{
  mm_cmd_type *mm_cmd_ptr = NULL;

  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_CHANGE_MODE_CNF:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx: TDSRRC_CHANGE_MODE_CNF(Inactive)");
      
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: cnf_mode=%d, td2g_prog=%d, g2td_prog=%d",
                                  cmd_ptr->cmd.change_mode_cnf.mode,
                                  tdsrrccho_proc_info.cco_in_progress, 
                                  tdsrrc_g2td_cho_info.cc_order_in_progress);

      if(cmd_ptr->cmd.change_mode_cnf.mode == TDSRRC_MODE_INACTIVE)
      {
#ifdef FEATURE_INTERRAT_PCCO_G2TD
        if(tdsrrc_g2td_cho_info.cc_order_in_progress)
        {
          /* Force initialise RCE so that RCE does not send an ABORT_IND to MM */
          tdsrrcrce_force_initialize(TRUE);

          /* Send PCCO Cnf.Failure cause and status(Failure) would already be set */
          tdsrrccho_send_pcco_cnf();
        }
#endif

        if (tdsrrccho_proc_info.cco_in_progress)
        {		
	      /* Send CCO Completion Ind to MM */
          if((mm_cmd_ptr = mm_tdsrrc_get_cmd_buf(RRC_WTOG_CCO_COMPLETE_IND)) == NULL)
          {
            ERR_FATAL("No MM buffer for RRC_ACTIVATION_IND",0,0,0);
          }

#ifdef FEATURE_DUAL_SIM
          mm_cmd_ptr->cmd.rrc_wtog_cco_complete_ind.as_id = tdsrrc_get_as_id();
#endif
          if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
          {
            tdscfa_mm_put_cmd(mm_cmd_ptr);
          }
          mm_put_cmd(mm_cmd_ptr);

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_WTOG_CCO_COMPLETE_IND sent to MM");
          /* Send TDSRR_INTERRAT_TL1_STOP_IND to RR */
          tdsrrccho_send_tl1_stop_ind();
        }
        /* Reset state machine */
        tdsrrccho_initialize_internal_data();
      }
      break;

    default:
      /* Unexpected command */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected CMD %x in state %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccho_substate);
      break;
  }
} /* tdsrrccho_process_wait_inactive_cnf_event_handler */

/*===========================================================================

FUNCTION  tdsrrccho_process_wait_conn_setup_ind_event_hdlr

DESCRIPTION

  This function handles the events received by this module when
  the procedure is Waiting for Connection Indication from RCE

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccho_process_wait_conn_setup_ind_event_hdlr( tdsrrc_cmd_type *cmd_ptr )
{
  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_CONN_SETUP_IND:

      if(cmd_ptr->cmd.rrc_conn_setup_ind.status == SUCCESS)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rxed: TDSRRC_CONN_SETUP_IND with Success");

        /* Stop Timer T3174 */
        tdsrrctmr_stop_timer(TDSRRCTMR_T_3174_TIMER);

        /* Update RRC Mode to Active since the PCCO was successful */
        tdsrrcmcm_update_rrc_mode_to_active();

        /* Send the PCCO CNF to RR indicating Success */
        tdsrrc_g2td_cho_info.status = TDSCCO_SUCCESS;
        tdsrrc_g2td_cho_info.failure_reason = TDSRRC_UMTS_NONE;
        tdsrrccho_send_pcco_cnf();

        /* Initialize internal data & reset state machine */
        tdsrrccho_initialize_internal_data();
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rxed: TDSRRC_CONN_SETUP_IND with Failure");

         /* Stop Timer T3174 */
        tdsrrctmr_stop_timer(TDSRRCTMR_T_3174_TIMER);

        /* Set PCCO status and failure reason */
        tdsrrc_g2td_cho_info.status = TDSCCO_FAILURE_AFTER_ACT_IND;
        if(cmd_ptr->cmd.rrc_conn_setup_ind.conn_failure_reason == TDSRRC_MAX_RACH_ATTEMPTS)
        {
          tdsrrc_g2td_cho_info.failure_reason = TDSRRC_UMTS_MAX_RACH_ATTEMPTS;
        }
        else if(cmd_ptr->cmd.rrc_conn_setup_ind.conn_failure_reason == TDSRRC_CONNECTION_REJECT)
        {
          tdsrrc_g2td_cho_info.failure_reason = TDSRRC_UMTS_CONNECTION_REJECT;
        }
        else
        {
          /* Set a generic failure cause */
          tdsrrc_g2td_cho_info.failure_reason = TDSRRC_UMTS_CELL_CHANGE_FAILURE;
        }

        /* Change mode to Inactive */
        tdsrrccho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
        tdsrrccho_substate = TDSRRCCHO_WAIT_INACTIVE_CNF;
      }
      break;

    default:
      /* Unexpected command */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected cmd %x in CHO state %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccho_substate);
      break;
  }
} /* tdsrrccho_process_wait_conn_setup_ind_event_hdlr */

/*===========================================================================

FUNCTION  tdsrrccho_process_wait_camp_cnf_event_hdlr

DESCRIPTION

  This function handles the events received by this module when
  the procedure is Waiting for Camping confirmation from CSP

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccho_process_wait_camp_cnf_event_hdlr( tdsrrc_cmd_type *cmd_ptr )
{
#ifdef FEATURE_INTERRAT_PCCO_G2TD
  mm_cmd_type *mm_cmd_ptr = NULL;
#endif

  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_INTERRAT_CC_ORDER_ABORT_REQ)
  {
    tdsrrc_g2td_cho_info.status = TDSCCO_FAILURE_BEFORE_ACT_IND;
    tdsrrc_g2td_cho_info.failure_reason = TDSRRC_UMTS_NONE;
    tdsrrccho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
    /* go to inactive substate so that we abort WCDMA*/
    tdsrrccho_substate = TDSRRCCHO_WAIT_INACTIVE_CNF;
  }
  else
  {
    /* Switch on the received command. */
    switch( cmd_ptr->cmd_hdr.cmd_id )
    {
      case TDSRRC_CSP_CAMP_CNF:
  #ifdef FEATURE_INTERRAT_PCCO_G2TD
        if(cmd_ptr->cmd.rrc_csp_camp_cnf.camping_status == SUCCESS)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rxed: TDSRRC_CSP_CAMP_CNF(Success)");

          /* Send Activation Ind to MM since we have camped successfully */
          if((mm_cmd_ptr = mm_tdsrrc_get_cmd_buf(RRC_ACTIVATION_IND)) == NULL)
          {
            ERR_FATAL("No MM buffer for RRC_ACTIVATION_IND",0,0,0);
          }
          mm_cmd_ptr->cmd.rrc_activation_ind.status = MM_AS_GTOW_CC_ORDER;

  #ifdef FEATURE_DUAL_SIM
          mm_cmd_ptr->cmd.rrc_activation_ind.as_id = tdsrrc_get_as_id();
  #endif
          /* Put the command in MM queue */
          if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
          {
            tdscfa_mm_put_cmd(mm_cmd_ptr);
          }
          mm_put_cmd(mm_cmd_ptr);

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_ACTIVATION_IND sent to MM");

          /* Update the cause for activation indication */
          tdsrrccho_act_cause = RRCCHO_INTERRAT_CC_ORDER_G2TD;
          tdsrrcmcm_update_tds_activation_cause(RRCMCM_INTERRAT_CC_ORDER_G2TD);

          /* Wait for Activation Rsp from MM*/
          tdsrrccho_substate = TDSRRCCHO_WAIT_ACTIVATION_RSP;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rxed: TDSRRC_CSP_CAMP_CNF(Failure)");

          /* Stop Timer T3174 */
          tdsrrctmr_stop_timer(TDSRRCTMR_T_3174_TIMER);

          /* Set PCCO status and failure reason */
          tdsrrc_g2td_cho_info.status = TDSCCO_FAILURE_BEFORE_ACT_IND;
  #ifndef FEATURE_GSM_PLT
          tdsrrc_g2td_cho_info.failure_reason = cmd_ptr->cmd.rrc_csp_camp_cnf.failure_cause;
  #endif
          /* Change mode to Inactive */
          tdsrrccho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
          tdsrrccho_substate = TDSRRCCHO_WAIT_INACTIVE_CNF;
        }
  #endif
        break;

      default:
        /* Unexpected command */
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected cmd %x in CHO state %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccho_substate);
        break;
    }
  }
} /* tdsrrccho_process_wait_camp_cnf_event_hdlr */

/*===========================================================================

FUNCTION   tdsrrccho_send_camp_req

DESCRIPTION

  This function sends a request to CSP to camp on a TDS cell.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccho_send_camp_req(void)
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_CSP_CAMP_REQ",0,0,0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CSP_CAMP_REQ;

  /* Fill in the Frequency on which we want to acquire */
  cmd_ptr->cmd.rrc_csp_camp_req.freq = tdsrrc_g2td_cho_info.freq;

  /* Fill in the scrambling code */
  cmd_ptr->cmd.rrc_csp_camp_req.cpid = (uint16)(tdsrrc_g2td_cho_info.cpid); /*(uint16)(tdsrrc_g2td_cho_info.scr_code* 16);*/

  /* Fill in the PLMN Id */
  cmd_ptr->cmd.rrc_csp_camp_req.requested_plmn = tdsrrc_g2td_cho_info.requested_plmn;

  /* Fill the network select mode */
  cmd_ptr->cmd.rrc_csp_camp_req.network_select_mode = tdsrrc_g2td_cho_info.network_select_mode;

  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_CSP_CAMP_REQ sent to CSP");

}/*tdsrrccho_send_camp_req*/

/*===========================================================================

FUNCTION  tdsrrccho_process_wait_irat_in_progress_cnf_event_hdlr

DESCRIPTION

  This function handles the events received by this module when
  the procedure is Waiting for Interrat in Progress Cnf from MCM

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccho_process_wait_irat_in_progress_cnf_event_hdlr( tdsrrc_cmd_type *cmd_ptr )
{
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_INTERRAT_CC_ORDER_ABORT_REQ)
  {
    tdsrrc_g2td_cho_info.status = TDSCCO_FAILURE_BEFORE_ACT_IND;
    tdsrrc_g2td_cho_info.failure_reason = TDSRRC_UMTS_NONE;
    tdsrrccho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
    /* go to inactive substate so that we abort TDSCDMA*/
    tdsrrccho_substate = TDSRRCCHO_WAIT_INACTIVE_CNF;
  }
  else
  {

    /* Switch on the received command. */
    switch( cmd_ptr->cmd_hdr.cmd_id )
    {
      /* TDSRRC_CHANGE_MODE_CNF */
      case TDSRRC_CHANGE_MODE_CNF:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rx: TDSRRC_CHANGE_MODE_CNF(IRAT_PROG)");

        /* Request CSP to camp on the cell */
        tdsrrccho_send_camp_req();

        /* Set the procedure state to wait for Acquisition confirmation */
        tdsrrccho_substate = TDSRRCCHO_WAIT_CAMP_CNF;

        break;

      default:
        /* Unexpected command */
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected cmd %x in CHO state %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccho_substate);
        break;
    }
  }
} /* end of tdsrrccho_process_wait_irat_in_progress_cnf_event_hdlr */

/*===========================================================================

FUNCTION   RRCCHO_PROCESS_PCCO_TO_UTRAN_COMMAND

DESCRIPTION

  This function processes the Packet Cell Change Order to UTRAN request from
  GSM RR.Copies the contents into local structure and activates the TDS stack .

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrccho_process_pcco_to_utran_command
(
  tdsrrc_rr_cmd_type * cmd_ptr
)
{
  /* Copy the contents of the PCCO Request to local variables */
  tdsrrc_g2td_cho_info.requested_plmn = mmumts_convert_nas_plmn_id_to_rrc_plmn_id(cmd_ptr->cmd.interrat_cc_order_req.requested_PLMN);
  tdsrrc_g2td_cho_info.freq = cmd_ptr->cmd.interrat_cc_order_req.UARFCN;
  tdsrrc_g2td_cho_info.cpid = cmd_ptr->cmd.interrat_cc_order_req.cpid;
  tdsrrc_g2td_cho_info.network_select_mode = cmd_ptr->cmd.interrat_cc_order_req.network_select_mode;
  tdsrrc_g2td_cho_info.t3174 = cmd_ptr->cmd.interrat_cc_order_req.t3174;

#ifdef FEATURE_TDSCDMA_DEDICATED_PRI_INFO
  /*updating the dedicated priority info received in rrc_interrat_cc_order_req into global structure (rrc_dedicated_priority_info)*/
  tdsrrcmeas_update_dedicated_priority_info_from_irat_req(&cmd_ptr->cmd.interrat_cc_order_req.dedicated_priority_info,
                           mmumts_convert_nas_plmn_id_to_rrc_plmn_id(cmd_ptr->cmd.interrat_cc_order_req.requested_PLMN));
#endif
#ifdef FEATURE_RAT_PRIORITY_LIST
  tdsrrccsp_update_rat_pri_list_info(&cmd_ptr->cmd.interrat_cc_order_req.rat_pri_list_info);
#endif

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"PCCO params: UARFCN %d,CPID %d,T3174 %d msec",
	          tdsrrc_g2td_cho_info.freq,
	          tdsrrc_g2td_cho_info.cpid,
            tdsrrc_g2td_cho_info.t3174);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"PCCO params contd: Network Select Mode is %d",tdsrrc_g2td_cho_info.network_select_mode);

  /* Set T3174 to atleast 5 secs */
  if(tdsrrc_g2td_cho_info.t3174 < 5000)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting T3174 to 5 secs");
    tdsrrc_g2td_cho_info.t3174 = 5000;
  }

  /* Acquire lock before starting TDS for GTOW CCO */
  if(tdsrrcwrm_acquire_lock_for_IHO_or_CCO(tdsrrc_g2td_cho_info.t3174, FALSE) != TDSRRC_WRM_LOCK_ACQUIRED)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Lock not acquired for CCO.");
    /* Set PCCO status and failure reason */
    tdsrrc_g2td_cho_info.status = TDSCCO_FAILURE_BEFORE_ACT_IND;
    tdsrrc_g2td_cho_info.failure_reason = TDSRRC_UMTS_INAVALID_STATE;

    /* Send PCCO Cnf.Failure cause and status(Failure) would already be set */
    tdsrrccho_send_pcco_cnf();
    /* Reset state machine */

    tdsrrccho_initialize_internal_data();
    return;
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Lock acquired for CCO.");
  /* Start timer T3174 with the remaining time of lock wait timer */
  tdsrrctmr_start_timer(TDSRRCTMR_T_3174_TIMER,tdsrrctmr_get_remaining_time(TDSRRCTMR_WRM_LOCK_TIMER));
  tdsrrctmr_stop_timer(TDSRRCTMR_WRM_LOCK_TIMER);


  /* Request MCM to change the RRC mode Inactive to Interrat in Progress */
  tdsrrccho_send_change_mode_req(TDSRRC_MODE_INTERRAT_IN_PROGRESS);

  /* Change the procedure state to wait for change mode confirmation */
  tdsrrccho_substate = TDSRRCCHO_WAIT_IRAT_IN_PROGRESS_CNF;
} /* tdsrrccho_process_pcco_to_utran_command */


/*===========================================================================

FUNCTION   RRCCHO_FILL_GSM_CELL_INFO_FOR_CELL_BAR

DESCRIPTION

  This function is called by CSP when WtoG CCO fails because the GSM cell is
  barred. This function populates the GSM cell bar structure with the arfcn
  and bsic values from tdsrrccho_proc_info global structure so that the GSM cell
  can be appropriately barred.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrccho_fill_gsm_cell_info_for_cell_bar
(
  W_ARFCN_T *gsm_arfcn,
  uint8     *gsm_ncc,
  uint8     *gsm_bcc
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Fn tdsrrccho_fill_gsm_cell_info_for_cell_bar called");
  *gsm_arfcn = tdsrrccho_proc_info.cell_change_order_req.BCCH_ARFCN;
  *gsm_ncc   = tdsrrccho_proc_info.cell_change_order_req.BSIC_NCC;
  *gsm_bcc   = tdsrrccho_proc_info.cell_change_order_req.BSIC_BCC;
} /* tdsrrccho_fill_gsm_cell_info_for_cell_bar */


/*===========================================================================

FUNCTION tdsrrccho_procedure_event_handler

DESCRIPTION
    This procedure is the event handler for the RRC Cell Change Order procedure.
    The following events are handled by this procedure.


DEPENDENCIES
    None.

RETURN VALUE
    None.

===========================================================================*/
void tdsrrccho_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr      /* Pointer to RRC command */
)
{
  tdsrrc_state_e_type rrc_state;
  mm_cmd_type *mm_cmd_ptr = NULL;
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_STATE_CHANGE_IND)
  {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRC State Change: CHO state=%d, td2g_prog=%d, g2td_prog=%d",
                                  tdsrrccho_substate,
                                  tdsrrccho_proc_info.cco_in_progress, 
                                  tdsrrc_g2td_cho_info.cc_order_in_progress);

#ifdef FEATURE_INTERRAT_PCCO_G2TD
    if(tdsrrc_g2td_cho_info.cc_order_in_progress)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_STATE_CHANGE_IND when GTOW PCCO in progress,cho_substate %d",tdsrrccho_substate);

      /* If substate is not Wait_Inactive_cnf,some other procedure has taken the UE to
         idle.Set the status and failure cause and send pcco_cnf */
      if(tdsrrccho_substate != TDSRRCCHO_WAIT_INACTIVE_CNF)
      {
        /* Stop Timer T3174 */
        tdsrrctmr_stop_timer(TDSRRCTMR_T_3174_TIMER);

        /* Set PCCO status and failure reason */
        if( (tdsrrccho_substate == TDSRRCCHO_WAIT_IRAT_IN_PROGRESS_CNF)||
            (tdsrrccho_substate == TDSRRCCHO_WAIT_CAMP_CNF) )
        {
          tdsrrc_g2td_cho_info.status = TDSCCO_FAILURE_BEFORE_ACT_IND;
        }
        else
        {
           tdsrrc_g2td_cho_info.status = TDSCCO_FAILURE_AFTER_ACT_IND;
        }

        /* We do not know the exaect reason for UE to Idle since CHO
           did not initiate it.Hence set a generic reason for PCCO failure */
        tdsrrc_g2td_cho_info.failure_reason = TDSRRC_UMTS_CELL_CHANGE_FAILURE;

        /* Send PCCO_CNF */
        tdsrrccho_send_pcco_cnf();
      }
      else
      {
        /* Else,the mode change to Inactive was initiated by CHO procedure.The status and failure
           cause would have been set already when the mode change was initiated.Just send
           the pcco_cnf */
        tdsrrccho_send_pcco_cnf();
      }
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"RRC State Change to IDLE in %d",tdsrrccho_substate);
    }
#endif

    if (tdsrrccho_proc_info.cco_in_progress)
    {
      if(tdsrrccho_substate == TDSRRCCHO_WAIT_INACTIVE_CNF)
      {
	    if((mm_cmd_ptr = mm_tdsrrc_get_cmd_buf(RRC_WTOG_CCO_COMPLETE_IND)) == NULL)
        {
          ERR_FATAL("No MM buffer for RRC_ACTIVATION_IND",0,0,0);
        }

#ifdef FEATURE_DUAL_SIM
        mm_cmd_ptr->cmd.rrc_wtog_cco_complete_ind.as_id = tdsrrc_get_as_id();
#endif
        if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
        {
          tdscfa_mm_put_cmd(mm_cmd_ptr);
        }
        mm_put_cmd(mm_cmd_ptr);

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_WTOG_CCO_COMPLETE_IND sent to MM");
        /* Send TDSRR_INTERRAT_TL1_STOP_IND to RR */
        tdsrrccho_send_tl1_stop_ind();
      }
    }

    /* Reset state machine */
    tdsrrccho_initialize_internal_data();
    tdsrrccho_act_cause = TDSRRCCHO_ACT_CAUSE_NONE;
  }
  else if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_T3174_EXPIRED_IND)
  {
#ifdef FEATURE_INTERRAT_PCCO_G2TD
    if(tdsrrc_g2td_cho_info.cc_order_in_progress)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_T3174_EXPIRED_IND in CHO sub-state %d",tdsrrccho_substate);

      /* If CHO is already waiting for an Inactive_Cnf,it means we are already in the
         process of deactivating TDS due to some other failure.Hence we can safely
         ignore the Timer expiry Ind.Hence process the expired ind only if CHO is not
         in wait_inactive_cnf substate */
      if(tdsrrccho_substate != TDSRRCCHO_WAIT_INACTIVE_CNF)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Stopping TDS in CHO sub-state %d",tdsrrccho_substate);

        /* Set PCCO status and failure reason */
        if( (tdsrrccho_substate == TDSRRCCHO_WAIT_IRAT_IN_PROGRESS_CNF)||
            (tdsrrccho_substate == TDSRRCCHO_WAIT_CAMP_CNF) )
        {
          tdsrrc_g2td_cho_info.status = TDSCCO_FAILURE_BEFORE_ACT_IND;
        }
        else
        {
           tdsrrc_g2td_cho_info.status = TDSCCO_FAILURE_AFTER_ACT_IND;
        }
        tdsrrc_g2td_cho_info.failure_reason = TDSRRC_UMTS_T3174_EXPIRED;

        /* Send Change Mode Request to Inactive */
        tdsrrccho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
        tdsrrccho_substate = TDSRRCCHO_WAIT_INACTIVE_CNF;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CHO already in the process of de-activating TDS.Hence,Timer Expiry not handled");
      }
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_T3174_EXPIRED_IND when GTOW PCCO is not active.Ignoring cmd.rrc_state: %d",tdsrrc_get_state());
    }
#endif
  }
  else
  {
    rrc_state = tdsrrc_get_state();

    switch(rrc_state)
    {
       /*using one handler to process both FACH and DCH*/
       case TDSRRC_STATE_CELL_FACH:
       case TDSRRC_STATE_CELL_DCH:
        tdsrrccho_process_cell_fach_state(cmd_ptr);
        break;


#ifdef FEATURE_INTERRAT_PCCO_G2TD

      case TDSRRC_STATE_DISCONNECTED:
      case TDSRRC_STATE_CONNECTING:
        switch(tdsrrccho_substate)
        {
          case TDSRRCCHO_INITIAL:
            switch(cmd_ptr->cmd_hdr.cmd_id)
            {
              /*lint -e408*/
              case TDSRRC_INTERRAT_CC_ORDER_REQ:
              /*lint +e408*/
                if(tdsrrc_g2td_cho_info.cc_order_in_progress)
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Dropping PCCO request from RR since PCCO is already active.");
                }
                else
                {
                  /* Received an Interrat PCCO Request from RR */
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received RRC_INTERRAT_CC_ORDER_REQ from RR");
                  /* Set gtow flag to True */
                  tdsrrc_g2td_cho_info.cc_order_in_progress = TRUE;

                  /* Process the GTOW PCCO request */
                  /*lint -e740*/
                  tdsrrccho_process_pcco_to_utran_command( (tdsrrc_rr_cmd_type *)cmd_ptr);
                  /*lint +e740*/
                }
                break;

              default:
                /* Unexpected event */
                TDSRRC_MSG3(MSG_LEGACY_HIGH,"Unexpected cmd %x in CHO: RRC st: %d, CHO st: %d",
				          cmd_ptr->cmd_hdr.cmd_id,rrc_state,tdsrrccho_substate);
                break;
            }
            break;

          case TDSRRCCHO_WAIT_IRAT_IN_PROGRESS_CNF:
            tdsrrccho_process_wait_irat_in_progress_cnf_event_hdlr(cmd_ptr);
            break;

          case TDSRRCCHO_WAIT_CAMP_CNF:
            tdsrrccho_process_wait_camp_cnf_event_hdlr(cmd_ptr);
            break;

          case TDSRRCCHO_WAIT_ACTIVATION_RSP:
            tdsrrccho_process_wait_activation_rsp_event_hdlr(cmd_ptr);
            break;

          case TDSRRCCHO_WAIT_CONN_SETUP_IND:
            tdsrrccho_process_wait_conn_setup_ind_event_hdlr(cmd_ptr);
            break;

          case TDSRRCCHO_WAIT_INACTIVE_CNF:
            tdsrrccho_process_wait_inactive_cnf_event_handler(cmd_ptr);
            break;

          default:
            /* Unexpected event */
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"Unexpected event to CHO. RRC st: %d,CHO st: %d",rrc_state,tdsrrccho_substate);
            break;
        }
        break;
#endif

      default:
        /* Unexpected event */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Event fwded to CHO in wrong RRC st: %d",rrc_state);
        break;
    }
  }

}   /* tdsrrccho_procedure_event_handler */

/*===========================================================================

FUNCTION tdsrrccho_init_procedure

DESCRIPTION
    This function initializes everything that the RRC Cell Change Order procedure
    needs to have initialized.

DEPENDENCIES
    None.

RETURN VALUE
    None.

===========================================================================*/

void tdsrrccho_init_procedure(void)
{

  /* Reset variables */
  tdsrrccho_initialize_internal_data();

    /* Register the state change from DCH to Idle */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_CHO,      /* Procedure name */
                             TDSRRC_STATE_CELL_DCH,     /* From State     */
                             TDSRRC_STATE_DISCONNECTED  /* To State       */
                           );
    /* Register the state change from FACH to Idle */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_CHO,      /* Procedure name */
                             TDSRRC_STATE_CELL_FACH,    /* From State     */
                             TDSRRC_STATE_DISCONNECTED  /* To State       */
                           );

  /* Register the state change from CONNECTING to Idle */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_CHO,      /* Procedure name */
                             TDSRRC_STATE_CONNECTING,    /* From State     */
                             TDSRRC_STATE_DISCONNECTED  /* To State       */
                           );

#ifdef FEATURE_RRC_DELAY_CELL_CHANGE_ORDER
  /* Define a timer used to delay 60ms before
  suspending TDS */
  rex_def_timer(&tdsrrccho_temp_timer, rex_self(), TDSRRCCHO_TEMP_TIMER_SIG);
#endif /* FEATURE_RRC_DELAY_CELL_CHANGE_ORDER */

}


