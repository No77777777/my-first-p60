/*===========================================================================

                                  R R C R C E

GENERAL DESCRIPTION
 The RRCRCE module consists of RRC CONNECTION ESTABLISHMENT PROCEDURE 
 implementation. The 3GPP TS 25.331 RRC Protocol specification is used as
 basis for implementation.

EXTERNALIZED FUNCTIONS

  tdsrrcrce_init_procedure()
    This function will initialize the RRC Connection Establishment 
    procedure substate to TDSRRCRCE_INITIAL Substate. This will called in the
    RRCTASK modules along with other Init Procedures.

  tdsrrcrce_procedure_event_handler()

    This is the entry point for RRC Connection Establishment procedure. 
    All events for RCE come to this function from RRC Dispatcher. 
    Depending on the UE state of RRC Connection Procedure, it calls the
    corresponding state handler. These state handlers call substate 
    handlers depending on the the current substate of RRC Connection
    Establishment procedure.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrce.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/21/11   yuh    Merged LTE code
09/08/11   mkh     Events renamed to TDS
08/12/11   ysh   when judge cpid present, should judge cellParametersIDPresent, not dl_InformationPerRL_ListPresent.
06/16/11   bj    Added code to handle receive R9 RRCSetup message.
03/10/11   bj     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/18/10   ysh     Frequency info save. 3GPP-8.6.6.1(3.4.2)
                                  Save Primary freq when enter Non-CELL_DCH state.
11/17/10   bj      Made changes to set IE "UE RadioAccessCapability"(FDD->TDD).
11/11/10   yzh     (3GPP: 8.6.6.28a)Del Downlink F-DPCH info common for all radio links(FDD)
                    and (3GPP: 8.6.6.27)Del tdsrrcllcpcie_initialise_sync_a_post_veri_info() when IE
                    Downlink information common for all radio links(FDD->TDD).
11/10/10   bj      Deleted IE "RNC support for change of UE capability".(3GPP: 8.6.2.3)
11/10/10   bj      Added code to set IE "UE Specific Behaviour Information 1 idle".,this IE is used 
                     to indicate the N frequncy and multi-Carrier cacapability 
10/30/10   ysh     IE"Downlink information for each radio link" info save.  3GPP-8.6.6.4(3.2.13).
10/29/10   zwj     Added code to handle receive R4 RRCSetup Message.
10/26/10   bj      Made changes to set Call Type does not need to check ESTABLISHMENT_CAUSE.3GPP:8.1.3.3

===========================================================================*/



/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "customer.h"
#include "rex.h"
#include "err.h"
#include "tdsrrcasn1.h"
#include "tdsrrcrce.h"
#include "tdsrrcrcei.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrccmd_v.h"
#include "tdsrrccmd_l2.h"
#include "tdsl1rrcif.h"
#include "tdsrrccmd.h"
#include "tdsrrcccm.h"
#include "tdsrrclcm.h"
#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "tdsrrcsend.h"
#include "tdsrrcmeas.h"
#include "tdsrrcllc.h"
#include "tdsrrcsibdb.h"
#include "tdsrrcscmgr.h"
#include "tdsrrctmr.h"
#include "string.h"
#include "tdsrrcuece.h"
#include "tdsrrcueci.h"
#include "tdsrrcdt.h"
#include "bit.h"
#include "tdsrrcsmc.h"
#include "tdsrrcsibproc.h"
#include "tdsrrcmcm.h"
#include "tdsrrcnv.h"
#include "tdsrrclogging.h"
#include "event.h"
#include "tdscdmaselfhosting.h"
#include "tdsrrcrcr.h"
#include "tdsrrcasn1util.h"
#include "tdsrrccsp.h"
#include "tdsrrccspi.h"
#include "tdsrrccho.h"
#include "tdsrrcmisc.h"
#include "mm_as_types.h"
/*#include "rr_rrc_if.h"*/
#include "tdsrrc_rr_if.h"
#include "rr_tdsrrc_if.h"

#ifdef  FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
#include "tdsrrcmeasi.h"
#endif


#include "tdsrrcmisc.h"
#include "tdsrrccu.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsCFAParser_i.h"

#ifdef FEATURE_TDSCDMA_TO_LTE
#include "lte_rrc_ext_api.h"
#endif
#include "tdsl1trm.h"
#include "tdsrrcwrm.h"
#include "tdsrrccui.h"
#include "tdsrrcqsh.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

#ifdef FEATURE_UMTS_PDCP
extern boolean tdsrrc_pdcp_enabled;
#endif /* FEATURE_UMTS_PDCP */
/* Flag to indicate whether to wait for 40 seconds before sending 
   signalling connection release */

#ifdef FEATURE_3GPP_CSFB_TDS
extern boolean tdsrrc_csfb_call_status;
#endif

#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
#define TDSRRC_DEFAULT_DRX_CYCLE_MS 2560
#define TDSRRC_NUM_DRX_CYCLE_BEFORE_WTOL_FAST_REDIR 2
/* Min Guard timer value needs to be 3s. 2sec for SIB reading during DCH->PCH trans
   and 1sec for CU after PAGE reception. */
#define TDSRRC_GUARD_TIMER_FOR_PCH_TRANS_BEFORE_WTOL_FAST_REDIR 3000
tdsrrcrce_csfb_status_e_type tdsrrc_csfb_extended_status = CSFB_INVALID;
static boolean tdsrrc_crt_restarted_once = FALSE;
#endif
typedef struct rrcrce_default_config_info_s {
  tdsrrc_config_e_type   config_type;  
  struct tdsrrc_SysInfoType16 * sib16_ptr;      /*ptr to sysInfoType16       */
  /* Pointer to DL DCCH message */
  tdsrrc_DL_DCCH_Message     *dcch_msg_ptr;
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT 
  struct tdsrrc_DL_DCCH_Message * rbs_ptr;    /*ptr to R5 RBS for DCs*/
#endif /*FEATURE_TDSCDMA_SRB5_SUPPORT*/

  uint32              default_config_id;     /*default configuration ID   */
  boolean             def_config_ptr_present;/*flag to indicate that def
                                               config ptr has been expanded
                                               to SIB16 type.             */
} tdsrrcrce_default_config_info_type;

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
extern boolean tdsrrc_ue_pos_cap_pch ;
#endif

extern sys_proc_type_e_type tdsrrc_proc_type;

/*local ptr to keep the NAS EST_REQ*/
static tdsrrc_cmd_type *tdsrrcrce_temp_cmd_ptr = NULL;


boolean tdsrrc_t2t_redir_due_to_rej;            /* The flag is record RCJ with redirection info */

/*===========================================================================

      RRC CONNECTION ESTABLISHMENT PROCEDURE SUBSTATE Variable        

===========================================================================*/
/* RCE Substate variable initialized to RCE_INITIAL during the RRC TASK 
   startup. */              
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrcrce_substate_e_type  tdsrrcrce_substate;  


static uint32 tdsrrcrce_number_of_rbs;      /* Indicates number of RBs that are 
                                           received thru RRC CONNECTION
                                           SETUP message */
static tdsrrc_RRC_TransactionIdentifier tdsrrcrce_setup_transaction_identifier;
                                        /* This stores the transaction Id for
                                           RRC Connection Setup message */
static tdsrrc_RRC_TransactionIdentifier tdsrrcrce_reject_transaction_identifier;
                                        /* This stores the transaction Id for
                                           RRC Connection Reject message */
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrc_RRC_StateIndicator tdsrrcrce_state_indicator;
                                        /* This stores the State RRC State
                                           Indicator received thru RRC 
                                           Connection Setup Message */
static tdsrrc_WaitTime tdsrrcrce_wait_time;    /* This stores the Wait Time received
                                           through RRC Connection Reject
                                           message */
/*This variable is used in file scope only and set for global for debugging purpose only*/
rrc_abort_cause_e_type tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE; 
boolean tdsrrcrce_send_abort_cnf = FALSE;

static boolean tdsrrcrce_abort_ind_sent = FALSE;

static uint8 tdsrrcrce_num_retries_count = 0;
static boolean tdsrrcrce_sig_con_delay = FALSE;

static boolean tdsrrcrce_sig_conn_rel_req_for_both_domains = FALSE;

/* RCE procedure will fill the channel config request with the current value of the 
transaction id. Transaction ids will help RCE associate a channel config confirm
with a channel config request that it had sent */
static uint8  tdsrrcrce_chl_config_transaction_id = 0;

/* Stores the RRC Connection Setup message */
static tdsdl_ccch_msg_type tdsrrcrce_save_dl_ccch_msg;

/* Stores the information that redirection is via Connection
 * reject or Connection Setup */
static tdsrrcrce_conn_rsp_e_type tdsrrcrce_rsp_to_rrc_conn_req;

static tdsrrcrce_default_config_info_type tdsrrcrce_default_config_info;

/*CN Domain Identity*/
rrc_cn_domain_identity_e_type     tdsrrc_est_cn_domain_id;
/*Call Type*/
rrc_call_type_in_est_req_e_type  tdsrrc_est_call_type;

/* Routing Area ID */
rrc_rai_type         tdsrrc_est_req_for_rai;
LOCAL boolean        tdsrrcrce_lai_rai_mismatch = FALSE;

/*"Bit mask used to trim RCE message */
uint32 tdstrim_rce_msg_status = 0x00;

/* TRUE means get the L2 ACK for RRC Connection setup complete message. */
static boolean tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;

#define TDSRRC_RRC_SETUP_ALLOW_TA_TIMER_LENGTH 360
boolean tdsrrcrce_allow_ta_flag = FALSE;

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */
#define TDSGET_TRANS_ID() (++tdsrrcrce_chl_config_transaction_id % 255)



#define TDSRRCRCE_R6_CRITICAL_EXT_PRESENT \
  ((msg_ptr->message.u.rrcConnectionSetup.u.  \
    later_than_r3.criticalExtensions.t ==  \
    T_tdsrrc_RRCConnectionSetup_criticalExtensions_5_criticalExtensions) && \
    (msg_ptr->message.u.rrcConnectionSetup.u. \
     later_than_r3.criticalExtensions.u.criticalExtensions.t == \
     T_tdsrrc_RRCConnectionSetup_criticalExtensions_4_criticalExtensions) && \
    (msg_ptr->message.u.rrcConnectionSetup.u. \
     later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == \
     T_tdsrrc_RRCConnectionSetup_criticalExtensions_4_r6)) 

#define TDSRRCRCE_R7_CRITICAL_EXT_PRESENT \
  ((msg_ptr->message.u.rrcConnectionSetup.u.  \
    later_than_r3.criticalExtensions.t ==  \
    T_tdsrrc_RRCConnectionSetup_criticalExtensions_5_criticalExtensions) && \
    (msg_ptr->message.u.rrcConnectionSetup.u. \
     later_than_r3.criticalExtensions.u.criticalExtensions.t == \
     T_tdsrrc_RRCConnectionSetup_criticalExtensions_4_criticalExtensions) && \
    (msg_ptr->message.u.rrcConnectionSetup.u. \
     later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == \
     T_tdsrrc_RRCConnectionSetup_criticalExtensions_3_criticalExtensions) &&\
     (msg_ptr->message.u.rrcConnectionSetup.u. \
     later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == \
     T_tdsrrc_RRCConnectionSetup_criticalExtensions_3_r7)) 

#define TDSRRCTMR_WAIT_TIMER_FOR_T2G_RESEL_IN_MS   1000

/*===========================================================================
** -----------------------------------------------------------------------------------
**-- ----------NON-Demand paging section Srart--------------------------------------------
** -----------------------------------------------------------------------------------
  This section will have code section that will not be demand paged. Function which should be in this section are
  -RRC functions that are called by L1/L2 which are higher priority then RRC
  -RRC functions that are called in ISR context or RTOS timer call back
  -RRC functions that indirectly gets called by L1/L2 /ISR/timer call back   
  For example: Fun1() gets called by L1. Fun2() is called by Fun1(). Now both  Fun1() & Fun2() should be NON demand paged, 
  so both should be added in this section  
  Usage of pragma 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this ? 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */




/*===========================================================================
FUNCTION RRCRCE_IS_RCE_PROCEDURE_WAITING_FOR_BPLMN

DESCRIPTION
  This function looks into RRC Connection Establishment procedure's 
  sub-states and returns the status of RRC Connection Establishment
  Procedure.

DEPENDENCIES
  None

RETURN VALUE
  boolean: 
           TRUE  - if RCE is waiting for BPLMN search to be suspended/aborted.
           FALSE - otherwise
           
SIDE EFFECTS
  To be used only by CSP for BPLMN search as the logic is designed for CSP's requirement.
===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
boolean tdsrrcrce_is_rce_procedure_waiting_for_bplmn(void)
{
  tdsrrc_state_e_type curr_rrc_state = tdsrrc_get_state();

  if((curr_rrc_state == TDSRRC_STATE_DISCONNECTED)
     && (tdsrrcrce_substate != TDSRRCRCE_INITIAL))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSTOTDS RCE is active sst %d",tdsrrcrce_substate);
    /* RCE is active */
    return(TRUE);
  }
  else if((curr_rrc_state != TDSRRC_STATE_DISCONNECTED)
           && (tdsrrcrce_substate != TDSRRCRCE_FINAL))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSTOTDS RCE is active sst %d",tdsrrcrce_substate);
    /* RCE is active */
    return(TRUE);
  }
  else
  {
    return(FALSE);
  }
} /* tdsrrcrce_is_rce_procedure_waiting_for_bplmn */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/


/*===========================================================================

FUNCTION tdsrrcrce_is_call_pending

DESCRIPTION
  This function is called by CSP to check if the RCE is in
  MT call pending state


DEPENDENCIES
  None.

RETURN VALUE
  TRUE: MT call is pending.
  FALSE: MT call is not pending

===========================================================================*/
tdsrrcrce_call_pending_e_type tdsrrcrce_is_call_pending(void)
{
  tdsrrcrce_substate_e_type curr_substate = tdsrrcrce_get_rce_sub_state();
  //checking the RCE sub-state first since the est cause may not be reset correctly

  if ((curr_substate == TDSRRCRCE_WAIT_FOR_SIB7_CURRENT)||
      (curr_substate == TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF)||
      (curr_substate == TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP)||
      (curr_substate == TDSRRCRCE_WAIT_FOR_SRB_SETUP_CNF)||
      (curr_substate == TDSRRCRCE_WAIT_FOR_L2_ACK)||
      /*Following two state is for race condition*/
      (curr_substate == TDSRRCRCE_WAIT_NEW_CELL_IND)||
      (curr_substate == TDSRRCRCE_WAIT_NEW_CELL_IND_SIB7))
  {
    if ((tdsrrc_establishment_cause == RRC_EST_TERM_CONV_CALL)||
         (tdsrrc_establishment_cause == RRC_EST_TERM_STREAM_CALL)||
         (tdsrrc_establishment_cause == RRC_EST_TERM_INTERACT_CALL)||
         (tdsrrc_establishment_cause == RRC_EST_TERM_BACKGND_CALL)||
         (tdsrrc_establishment_cause == RRC_EST_TERM_HI_SIGNALLING)||
         (tdsrrc_establishment_cause == RRC_EST_TERM_LOW_SIGNALLING)||
         (tdsrrc_establishment_cause == RRC_EST_TERM_UNKNOWN))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDSRRCRCE is in MT pending, with est casue = %d,sub-state=%d.",tdsrrc_establishment_cause,curr_substate);
      return TDSRRCRCE_MT_CALL_PENDING;

    }
    else if ((tdsrrc_establishment_cause == RRC_EST_ORIG_CONV_CALL)||
         (tdsrrc_establishment_cause == RRC_EST_ORIG_STREAM_CALL)||
         (tdsrrc_establishment_cause == RRC_EST_ORIG_INTERACT_CALL)||
         (tdsrrc_establishment_cause == RRC_EST_ORIG_BACKGND_CALL))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDSRRCRCE is in MO pending, with est casue = %d,sub-state=%d.",tdsrrc_establishment_cause,curr_substate);
      return TDSRRCRCE_MO_CALL_PENDING;

    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDSRRCRCE is in other pending, with est casue = %d,sub-state=%d.",tdsrrc_establishment_cause,curr_substate);
      return TDSRRCRCE_OTHER_CALL_PENDING;
    }
    
  }
  return TDSRRCRCE_NONE_CALL_PENDING;
}

/*===========================================================================

FUNCTION tdsrrcrce_is_cs_call_pending

DESCRIPTION
  This function is called by CSP to check if the RCE is in
  CS call pending state


DEPENDENCIES
  None.

RETURN VALUE
  TRUE: CS call is pending.
  FALSE: CS call is not pending

===========================================================================*/
boolean tdsrrcrce_is_cs_call_pending(void)
{
  tdsrrcrce_substate_e_type curr_substate = tdsrrcrce_get_rce_sub_state();
  //checking the RCE sub-state first since the est cause may not be reset correctly

  /*If cell reselection and EST_REQ is pending or UE is waiting for UE camped on to handle EST_REQ, then check EST cause*/
  if (((TRUE == tdsrrccsp_reselection_in_progress()) &&
         (((curr_substate == TDSRRCRCE_WAIT_NEW_CELL_IND) && (tdsrrcrce_temp_cmd_ptr != NULL)) ||
         (curr_substate == TDSRRCRCE_WAIT_NEW_CELL_IND_SIB7))) ||
      (curr_substate == TDSRRCRCE_WAIT_FOR_UE_CAMP_ON))
  {
    if (((tdsrrc_establishment_cause == RRC_EST_TERM_CONV_CALL)||
        (tdsrrc_establishment_cause == RRC_EST_ORIG_CONV_CALL) ||
        (tdsrrc_establishment_cause == RRC_EST_EMERGENCY_CALL) ) &&
        (tdsrrc_est_cn_domain_id == RRC_CS_DOMAIN_CN_ID))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDSRRCRCE is in CS call pending, with est casue = %d,sub-state=%d.",tdsrrc_establishment_cause,curr_substate);
      return TRUE;
    } 
  }
  return FALSE;
}


/*===========================================================================

FUNCTION tdsrrcrce_delete_default_config_info

DESCRIPTION
  This function deletes the default configuration database used to maintain the
  status of RRC connection setup processing.

DEPENDENCIES
  None

RETURN VALUE
  uint8

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcrce_delete_default_config_info(void)
{
  if(tdsrrcrce_default_config_info.def_config_ptr_present == TRUE)
  {
    if((tdsrrcrce_default_config_info.default_config_id == TDSRRC_DEF_CONFIG_17) 
      || (tdsrrcrce_default_config_info.default_config_id == TDSRRC_DEF_CONFIG_23)
    )
    {
      /*free the msg_ptr previously allocated.. if failure, flag an error*/
      if(tdsrrcasn1_free_pdu((int)tdsrrc_DL_DCCH_Message_PDU, 
         tdsrrcrce_default_config_info.dcch_msg_ptr))
      { 
        /* This is a memory leak and it should never happen. */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful");    
      } 
      else
      { 
        /*clean the msg_ptr data structure*/
        tdsrrcrce_default_config_info.def_config_ptr_present = FALSE;
      } 
    }
    else
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    if (tdsrrcrce_default_config_info.rbs_ptr != NULL)
    {
      if(tdsrrcasn1_free_pdu((int)tdsrrc_DL_DCCH_Message_PDU, 
          tdsrrcrce_default_config_info.rbs_ptr))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful");  
      }
      else
      {
        /*clean the msg_ptr data structure*/
        tdsrrcrce_default_config_info.def_config_ptr_present = FALSE;
      }
    }
    else
#endif /*FEATURE_TDSCDMA_SRB5_SUPPORT*/
    {
      /*free the msg_ptr previously allocated.. if failure, flag an error*/
      if(tdsrrcasn1_free_pdu((int)tdsrrc_SysInfoType16_PDU, 
         tdsrrcrce_default_config_info.sib16_ptr))
      { 
        /* This is a memory leak and it should never happen. */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful");    
      } 
      else
      { 
        /*clean the msg_ptr data structure*/
        tdsrrcrce_default_config_info.def_config_ptr_present = FALSE;
      } 
    }
  }
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrcrce_default_config_info.rbs_ptr = NULL;
#endif /*FEATURE_TDSCDMA_SRB5_SUPPORT*/

  tdsrrcrce_default_config_info.sib16_ptr = NULL;

  tdsrrcrce_default_config_info.config_type =  TDSRRC_INVALID_CONFIG;
} /* tdsrrcrce_delete_default_config_info */

/*===========================================================================

FUNCTION tdsrrcrce_is_pccpch_info_present

DESCRIPTION
  This function checks whether cell parameter id info is present in
  the message or not.

DEPENDENCIES
  None.

RETURN VALUE
Boolean. TRUE  : cell parameter id info is present in the message
                 FALSE : cell parameter id info is not present in the message
===========================================================================*/
static boolean  tdsrrcrce_is_pccpch_info_present
(
  tdsrrc_RRCConnectionSetup          *msg_ptr,
  tdsrrc_msg_ver_enum_type            msg_version
)
{
  /* this function is used to find whether directed cell info is present
     or not during Cell_DCH to Cell_FACH state transition. It is safe to 
     assume that the rl list will have only one element.
   */
  boolean result = FALSE;

#ifdef FEATURE_TDSCDMA_REL9
  if (msg_version == TDSMSG_REL9)
  {
    tdsrrc_RRCConnectionSetup_r9_IEs *rce_ptr_r9 = &msg_ptr->u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.r9.rrcConnectionSetup_r9;

    if (rce_ptr_r9->m.dl_InformationPerRL_ListPresent)
    {
      if(rce_ptr_r9->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd)
      { 
        if (rce_ptr_r9->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RCE message %d",
                  rce_ptr_r9->dl_InformationPerRL_List.
                  elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          result = TRUE;
        }
      }
    }
  }
  else
#endif/*FEATURE_TDSCDMA_REL9*/
#ifdef FEATURE_TDSCDMA_REL8
  if (msg_version == TDSMSG_REL8)
  {
    tdsrrc_RRCConnectionSetup_r8_IEs *rce_ptr_r8 = &msg_ptr->u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8
      .rrcConnectionSetup_r8;

    if (rce_ptr_r8->m.dl_InformationPerRL_ListPresent)
    {
      if(rce_ptr_r8->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd)
      { 
        if (rce_ptr_r8->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RCE message %d",
                  rce_ptr_r8->dl_InformationPerRL_List.
                  elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          result = TRUE;
        }
      }
    }
  }
  else
#endif/*FEATURE_TDSCDMA_REL8*/
  if (msg_version == TDSMSG_REL7)
  {
    tdsrrc_RRCConnectionSetup_r7_IEs *rce_ptr_r7 = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
               criticalExtensions.u.r7.rrcConnectionSetup_r7;

    if (rce_ptr_r7->m.dl_InformationPerRL_ListPresent)
    {
      if(rce_ptr_r7->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_tdd)
      { 
        if (rce_ptr_r7->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RCE message %d",
                  rce_ptr_r7->dl_InformationPerRL_List.
                  elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          result = TRUE;
        }
      }
    }
  }
  else

  if (msg_version == TDSMSG_REL6)
  {
    tdsrrc_RRCConnectionSetup_r6_IEs *rce_ptr_r6 = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
               criticalExtensions.u.r6.rrcConnectionSetup_r6;

    if (rce_ptr_r6->m.dl_InformationPerRL_ListPresent)
    {
      if(rce_ptr_r6->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_tdd)
      { 
        if (rce_ptr_r6->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RCE message %d",
                  rce_ptr_r6->dl_InformationPerRL_List.
                  elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          result = TRUE;
        }
      }
    }
  }
  else
  if (msg_version == TDSMSG_REL5)
  {
    tdsrrc_RRCConnectionSetup_r5_IEs *rce_ptr_r5 = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
               r5.rrcConnectionSetup_r5;

    if (rce_ptr_r5->m.dl_InformationPerRL_ListPresent)
    {
      if(rce_ptr_r5->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r5_modeSpecificInfo_tdd)
      { 
        if (rce_ptr_r5->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RCE message %d",
                  rce_ptr_r5->dl_InformationPerRL_List.
                  elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          result = TRUE;
        }
      }
    }
  }
  else
  {
    tdsrrc_RRCConnectionSetup_r4_IEs *rce_ptr_r4 = &msg_ptr->u.later_than_r3.
              criticalExtensions.u.r4.rrcConnectionSetup_r4;

    if (rce_ptr_r4->m.dl_InformationPerRL_ListPresent)
    {
      if(rce_ptr_r4->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r4_modeSpecificInfo_tdd)
      { 
        if (rce_ptr_r4->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RCE message %d",
                  rce_ptr_r4->dl_InformationPerRL_List.
                  elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          result = TRUE;
        }
      }
    }
  }

  return result;
}  /* tdsrrcrbrc_is_pccpch_info_present */

/*===========================================================================

FUNCTION RRCRCE_SEND_CONN_SETUP_IND

DESCRIPTION
  This function builds TDSRRC_CONN_SETUP_IND command to post to CHO proc.The calling
  function should pass in the paramters to be sent in the command. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_send_conn_setup_ind(tdsrrc_conn_setup_ind_type conn_setup_ind_params);

/*===========================================================================

FUNCTION tdsrrcrce_init_procedure

DESCRIPTION
  This function will initialize the RRC Connection Establishment procedure
  substate to TDSRRCRCE_INITIAL Substate. This will called in the RRCTASK modules 
  along with other Init Procedures.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrcrce_init_procedure
(    
  void  
)
{
  /* Resets the RCE state machine */
  tdsrrcrce_substate = TDSRRCRCE_INITIAL; 
  /* The following code is added since de-registration mechanism is not place.
     It should be deleted once de-registration mechanism in place in State
     change manager */
  tdsrrcscmgr_register_for_scn(TDSRRC_PROCEDURE_RCE,
                            TDSRRC_STATE_WILDCARD,
                            TDSRRC_STATE_DISCONNECTED);

  /*Init the values*/
  tdsrrc_est_cn_domain_id = RRC_CS_DOMAIN_CN_ID;
  tdsrrc_est_call_type = RRC_CALL_TYPE_NONE;

  tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;

  /* For T2T redirection event */
  tdsrrc_t2t_redir_due_to_rej = FALSE;
  tdsrrcrce_allow_ta_flag = FALSE;

} /* tdsrrcrce_init_procedure */
#ifdef FEATURE_UE_INITIATED_DORMANCY
/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_abort_cnf_ue_init_dormancy

DESCRIPTION
  This function builds RRC_ABORT_CNF command to post to MM in response to 
  TDSRRC_ABORT_REQ. This is only for feature UE initiated dormancy feature
  This .TDSRRC_ABORT_REQ is kind of dummay cnf for NAS. if its sent then 
  RRC-NAS both will have RRC connection intact.This is used by RRC when 
  it receives abort_req for feature ue_init_dormancy & RRC is in OOS.
  The requirement here is that when application initiates close it can't wait 
  for longer time to receive response from modem. So If RRC is in OOS 
  return Immediately.
DEPENDENCIES
  None

RETURN VALUE
  Void
  
SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcrce_send_rrc_abort_cnf_ue_init_dormancy
(
  rrc_rel_cause_e_type abort_cnf_cause
)
{
  mm_cmd_type *cmd_ptr;                      /* Pointer to the MM Command */

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Send Abort Cnf in %d substate for ue init dormancy",tdsrrcrce_substate);
  /* Allocate memory for RRC_ABORT_CNF command */
  if( (cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_ABORT_CNF)) != NULL)
  {
    /* Have Zero value for HFNs as its dummy abort cnf to NAS*/ 
    memset(cmd_ptr->cmd.rrc_abort_cnf.hfn_cs, 0x00, RRC_MAX_HFN_LENGTH);
    memset(cmd_ptr->cmd.rrc_abort_cnf.hfn_ps, 0x00, RRC_MAX_HFN_LENGTH);

    cmd_ptr->cmd.rrc_abort_cnf.conn_rel_cause = abort_cnf_cause;

    cmd_ptr->cmd.rrc_abort_cnf.as_id = tdsrrc_get_as_id();

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sent RRC_ABORT_CNF to MM with rel_cause %d",cmd_ptr->cmd.rrc_abort_cnf.conn_rel_cause);

    /* Sends the command to MM */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(cmd_ptr);
    }
    mm_put_cmd(cmd_ptr);

  }    
  else   /* RRC Task failed to allocate memory */
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unable to allocate memory for cmd: %d",RRC_ABORT_CNF);
  }
}/*tdsrrcrce_send_rrc_abort_cnf */
#endif

/*===========================================================================

FUNCTION   TDSRRC_SEND_IRAT_ABORT_IND

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
void tdsrrc_send_irat_abort_ind(tdsrrc_proc_e_type tdsrrc_proc)
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
  cmd_ptr->cmd.rrc_irat_abort_ind.procedure = tdsrrc_proc;
  
  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_IRAT_ABORT_IND sent to %d",tdsrrc_proc);

  return;
}
/*===========================================================================

FUNCTION tdsrrcrce_procedure_event_handler

DESCRIPTION
  This is the entry point for RRC Connection Establishment procedure. All
  events for RCE come to this function from RRC Dispatcher. Depending on the
  UE state of RRC Connection Procedure, it calls the corresponding state 
  handler. These state handlers call substate handlers depending on the 
  the current substate of RRC Connection Establishment procedure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  TDSRRC_MSG2(MSG_LEGACY_MED,"Received RRCRCE event, rrc_st:%d,rrcrce_subst:%d",rrc_state,tdsrrcrce_substate);

#ifdef FEATURE_UE_INITIATED_DORMANCY
  /* If Abort is for UE initiated dormancy feature, check to see if UE is in OOS  */
  if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_ABORT_REQ)&&
     (cmd_ptr->cmd.rrc_abort_req.abort_cause == RRC_UE_INITIATED_DORMANCY)&&
     (rrc_state == TDSRRC_STATE_DISCONNECTED)
    )
  {
    /*UE does not have RRC connection so send abort cnf right here*/
    tdsrrcrce_send_rrc_abort_cnf_ue_init_dormancy(RRC_UE_INIT_DORMANCY_ABORT_SUCCESS);
    return;
  }
  else if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_ABORT_REQ)&&
     (cmd_ptr->cmd.rrc_abort_req.abort_cause == RRC_UE_INITIATED_DORMANCY)&&
     (TDSRRC_NOT_CAMPED_ON == tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_RCE))
    )
  {
    /* If Abort is for UE initiated dormancy feature, check to see if UE is in OOS  */
    /*UE is in OOS so send abort cnf right here*/
    tdsrrcrce_send_rrc_abort_cnf_ue_init_dormancy(RRC_UE_INIT_DORMANCY_UE_IN_OOS);
    return;
  }
#endif

/* Send release cause as PS_ABORT_SUCCESS only when connection has already been released*/
  if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_ABORT_REQ) &&
      (cmd_ptr->cmd.rrc_abort_req.abort_cause == RRC_DUAL_STANDBY_PS_ABORT))
  {
    if(rrc_state == TDSRRC_STATE_DISCONNECTED)
    {
      tdsrrc_release_cause = RRC_REL_DUAL_STANDBY_PS_ABORT_SUCCESS;
    }
    else
    {
      tdsrrc_release_cause = RRC_REL_CAUSE_NORMAL;
    }
  }
  if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_ABORT_REQ) && (tdsrrccho_is_td2g_cco_active() == TRUE))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"T2G CCO ABORT: Abort T2G CCO when receiving TDSRRC_ABORT_REQ");
    tdsrrc_send_irat_abort_ind(TDSRRC_PROCEDURE_CHO);
  }
  switch (rrc_state)
  {
    case TDSRRC_STATE_DISCONNECTED: 
      tdsrrcrce_process_disconnected_state(cmd_ptr);
      break;
    case TDSRRC_STATE_CONNECTING:   
      tdsrrcrce_process_connecting_state(cmd_ptr); 
      break;
    case TDSRRC_STATE_CELL_FACH:    
      tdsrrcrce_process_cell_fach_state(cmd_ptr); 
      break;
    case TDSRRC_STATE_CELL_DCH:     
      tdsrrcrce_process_cell_dch_state(cmd_ptr); 
      break;
    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:
      tdsrrcrce_process_cell_ura_pch_state(cmd_ptr);
      break;
    default:  
      ERR_FATAL("Event forwarded in invalid State: %d",rrc_state,0,0);
      break;
   }  

#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
  if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_STATE_CHANGE_IND) && (cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED)
      && (tdsrrc_csfb_extended_status != CSFB_INVALID))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB_HACK: Resetting tdsrrc_csfb_extended_status on state change to disconnected");
    tdsrrc_csfb_extended_status = CSFB_INVALID;
    tdsrrc_crt_restarted_once = FALSE;
  }
#endif

} /* tdsrrcrce_procedure_event_handler */

/*===========================================================================

FUNCTION tdsrrcrce_process_disconnected_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in Disconnected state of Idle mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_disconnected_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  rrc_abort_cause_e_type tdsrce_abort_ind_cause = RRC_MAX_ACCESS_FAILURE;
  if ((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_CPHY_OUT_OF_SERVICE_AREA_IND) 
     ||(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_OUT_OF_SERVICE_INTERNAL_IND)
	 )
  {
    event_report(EVENT_TDSCDMA_RRC_OUT_OF_SERVICE);
	TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
							   TDS_SELF_HOSTING_MAJOR_IDLE_EXCEPTION,
							   TDS_SELF_HOSTING_MINOR_RRC_OUT_OF_SERVICE);

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"OOS in RCE disconnected state.tdsrrcrce_substate %d",tdsrrcrce_substate);
    if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_OUT_OF_SERVICE_INTERNAL_IND)
    {
      tdsrce_abort_ind_cause = RRC_NO_RESOURCE_AVAILABLE;
    }
#ifdef FEATURE_INTERRAT_PCCO_G2TD
    /* If GTOTDS is in progress,send a conn_setup_ind with failure to CHO.
       CHO would deactivate TDS and pass control back to GSM.Do not send
       RRC_ABORT_IND to MM if GTOTDS is active */
    if(tdsrrccho_is_g2td_pcco_in_progress())
    { 
      tdsrrc_conn_setup_ind_type conn_setup_ind_params;

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"OOS in Disconnected state when PCCO is active.tdsrrcrce_substate %d",tdsrrcrce_substate);

      conn_setup_ind_params.status = FAILURE;
      conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;
      tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);

      /* reset the RCE substate */
      tdsrrcrce_substate = TDSRRCRCE_INITIAL;    
      tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
      return;
    }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

    if(tdsrrcrce_send_abort_cnf == TRUE)
    {
      if(TDSRRCRCE_SUCCESS == tdsrrcrce_send_rrc_abort_cnf() )
      { 
        tdsrrcrce_substate = TDSRRCRCE_INITIAL;
        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
      }
      else
      {
        ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
      }
    }
    else
    {
      if(tdsrrcrce_substate != TDSRRCRCE_INITIAL)
      {
        (void) tdsrrcrce_send_rrc_abort_ind(tdsrce_abort_ind_cause);
        tdsrrcrce_substate = TDSRRCRCE_INITIAL;
        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
      }
    }
    /* Set "set_l1_to_release_lock_when_drop_all" to indicate TL1 to release lock when drop all for internal OOS
	 * for OOS triggered by TL1, TL1 will always release lock when receive drop all, no need any indicator for it
	 */
    if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_OUT_OF_SERVICE_INTERNAL_IND)
    {
      tdsrrc_csp_int_data.set_l1_to_release_lock_when_drop_all = TRUE;
    }

    /* Initiate Cell Selection and remain in TDSRRCRCE_INITIAL substate */
    (void) tdsrrcrce_send_rrc_initiate_cell_selection_ind(TDSRRC_OUT_OF_SERVICE_AREA_IND,
                                                tdsrrc_get_state() );
    return;
  }

  switch (tdsrrcrce_substate)
  {
    case TDSRRCRCE_INITIAL: 
    // CR 352866 begin
    case TDSRRCRCE_WAIT_NEW_CELL_IND:
    // CR 352866 end
      tdsrrcrce_process_rce_initial_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_UE_CAMP_ON:
      tdsrrcrce_process_rce_wait_for_ue_camp_on_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_SIB7_CURRENT:
    // CR 352866 begin
    case TDSRRCRCE_WAIT_NEW_CELL_IND_SIB7:
    // CR 352866 end
      tdsrrcrce_process_rce_wait_for_sib7_current(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF: 
      tdsrrcrce_process_rce_wait_for_rb0_setup_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP: 
      tdsrrcrce_process_rce_wait_for_conn_setup_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_SRB_SETUP_CNF:
      tdsrrcrce_process_rce_wait_for_srb_setup_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_REDIRECT_CNF:
      tdsrrcrce_process_rce_wait_for_redirect_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_WAIT_TIMER_TO_EXPIRE:
      tdsrrcrce_process_rce_wait_for_wait_time_expire_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE:
      tdsrrcrce_process_rce_wait_for_rrc_timer_expire_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_CU_COMPLETE_CNF:
      tdsrrcrce_process_rce_wait_for_cu_complete_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCR_WAIT_FOR_SRB_REL_CNF:
      tdsrrcrce_process_rce_wait_for_srb_rel_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL:
      tdsrrcrce_process_rce_wait_for_sig_con_rel_compl_substate(cmd_ptr);
      break;
    case TDSRRCRCE_FINAL: 
      tdsrrcrce_process_rce_final_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_RB0_REL_CNF:
      tdsrrcrce_process_rce_wait_for_rb0_rel_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_RESELCT_COMPL:
      tdsrrcrce_process_rce_wait_for_reselct_compl_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_RB0_PHYCHAN_RESETUP_CNF:
      tdsrrcrce_process_rce_wait_for_rb0_phychan_resetup_cnf_substate(cmd_ptr);
      break;
    default:           
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid substate in DISCONNECTED State: %d",tdsrrcrce_substate);
      break;
  }  
}/* tdsrrcrce_process_disconnected_state */

/*===========================================================================

FUNCTION tdsrrcrce_process_connecting_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in Connecting state of Idle mode. The connecting 
  state is a transient state created to honor Paging Type 1 message during the
  RRC Connection Establishment procedure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_connecting_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  rrc_abort_cause_e_type tdsrce_abort_ind_cause = RRC_MAX_ACCESS_FAILURE;
  if ((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_CPHY_OUT_OF_SERVICE_AREA_IND)
   || (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_OUT_OF_SERVICE_INTERNAL_IND)
     )
  {
    event_report(EVENT_TDSCDMA_RRC_OUT_OF_SERVICE);
	TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
							   TDS_SELF_HOSTING_MAJOR_IDLE_EXCEPTION,
							   TDS_SELF_HOSTING_MINOR_RRC_OUT_OF_SERVICE);

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"OOS in Connecting state.tdsrrcrce_substate %d",tdsrrcrce_substate);
    if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_OUT_OF_SERVICE_INTERNAL_IND)
    {
      tdsrce_abort_ind_cause = RRC_NO_RESOURCE_AVAILABLE;
    }
#ifdef FEATURE_INTERRAT_PCCO_G2TD
    /* If GTOTDS is in progress,send a conn_setup_ind with failure to CHO
       CHO would deactivate TDS and pass control back to GSM */
    if(tdsrrccho_is_g2td_pcco_in_progress())
    { 
      tdsrrc_conn_setup_ind_type conn_setup_ind_params;

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"OOS in connecting state when PCCO is active.tdsrrcrce_substate %d",tdsrrcrce_substate);

      conn_setup_ind_params.status = FAILURE;
      conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;
      tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  

      /* Reset RCE susbstate */
      tdsrrcrce_substate = TDSRRCRCE_INITIAL;
      tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
      tdsrrcrce_allow_ta_flag = FALSE;
      return;
    }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

    /* Set "set_l1_to_release_lock_when_drop_all" to indicate TL1 to release lock when drop all for internal OOS
	 * for OOS triggered by TL1, TL1 will always release lock when receive drop all, no need any indicator for it
	 */
    if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_OUT_OF_SERVICE_INTERNAL_IND)
    {
      tdsrrc_csp_int_data.set_l1_to_release_lock_when_drop_all = TRUE;
    }

    (void)tdsrrcrce_send_rrc_channel_config_req_for_rb0(TDSRELEASE_RB, 
                                               TDSRRC_STATE_DISCONNECTED,
                                               FALSE);
    tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;
#ifdef FEATURE_WRLF_SYSTEM_SEL
    tdsrrc_set_connecting_oos_flag(TRUE);
#endif
    tdsrrcrce_mm_abort_cause = tdsrce_abort_ind_cause;
    return;
  }
  switch (tdsrrcrce_substate)
  {
    case TDSRRCRCE_WAIT_FOR_UE_CAMP_ON:
      tdsrrcrce_process_rce_wait_for_ue_camp_on_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP: 
      tdsrrcrce_process_rce_wait_for_conn_setup_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_SRB_SETUP_CNF:
      tdsrrcrce_process_rce_wait_for_srb_setup_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_REDIRECT_CNF:
      tdsrrcrce_process_rce_wait_for_redirect_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_WAIT_TIMER_TO_EXPIRE:
      tdsrrcrce_process_rce_wait_for_wait_time_expire_substate(cmd_ptr);
      break;
    case TDSRRCRCR_WAIT_FOR_SRB_REL_CNF:
      tdsrrcrce_process_rce_wait_for_srb_rel_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_RESELCT_COMPL:
      tdsrrcrce_process_rce_wait_for_reselct_compl_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_RB0_REL_CNF:
      tdsrrcrce_process_rce_wait_for_rb0_rel_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_RB0_PHYCHAN_RESETUP_CNF:
      tdsrrcrce_process_rce_wait_for_rb0_phychan_resetup_cnf_substate(cmd_ptr);
      break;
    default:     
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid substate in CONNECTING State: %d",tdsrrcrce_substate);
      break;
  }  
} /* tdsrrcrce_process_connecting_state*/
/*===========================================================================

FUNCTION tdsrrcrce_process_cell_fach_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in CELL_FACH state of Connected  mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_cell_fach_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  /* If network authentication check has failed then bar the serving cell */
  if(    (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_ABORT_REQ)
      && (cmd_ptr->cmd.rrc_abort_req.auth_failed_bar_cells == TRUE)
      && (tdsrrccsp_get_curr_scan() != TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
      && (tdsrrccsp_get_curr_scan() != TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN)
      && (tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_CSP) == TDSRRC_CAMPED_ON)
    )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"N/W Auth Failed. Rcvd Abort Req from MM in RRC State: %d. Cell bar req sent to L1.",tdsrrc_get_state());
    tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_CPID_BARRED, 1280);
  }
  switch (tdsrrcrce_substate)
  {
    case TDSRRCRCE_WAIT_FOR_CU_COMPLETE_CNF:
      tdsrrcrce_process_rce_wait_for_cu_complete_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCR_WAIT_FOR_SRB_REL_CNF:
      tdsrrcrce_process_rce_wait_for_srb_rel_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL:
      tdsrrcrce_process_rce_wait_for_sig_con_rel_compl_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE:
      tdsrrcrce_process_rce_wait_for_rrc_timer_expire_substate(cmd_ptr);
      break;
    case TDSRRCRCE_FINAL: 
      tdsrrcrce_process_rce_final_substate(cmd_ptr);
      break;
   default:     
     TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid substate in CELL_FACH state: %d",tdsrrcrce_substate);
     break;
  }  
} /* tdsrrcrce_process_cell_fach_state */
/*===========================================================================

FUNCTION tdsrrcrce_process_cell_dch_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in CELL_DCH state of Connected  mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_cell_dch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  /* If network authentication check has failed then bar the active set */
  if(    (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_ABORT_REQ)
      && (cmd_ptr->cmd.rrc_abort_req.auth_failed_bar_cells == TRUE)
      && (tdsrrccsp_get_curr_scan() != TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
      && (tdsrrccsp_get_curr_scan() != TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN)
      && (tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_CSP) == TDSRRC_CAMPED_ON)
    )
  { 
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"N/W Auth Failed. Rcvd Abort Req from MM in RRC State: %d. ASet bar req sent to L1.",tdsrrc_get_state());
    tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_ASET_BARRED, 1280);
  }
  switch (tdsrrcrce_substate)
  {
    
    case TDSRRCRCR_WAIT_FOR_SRB_REL_CNF:
      tdsrrcrce_process_rce_wait_for_srb_rel_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL:
      tdsrrcrce_process_rce_wait_for_sig_con_rel_compl_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE:
      tdsrrcrce_process_rce_wait_for_rrc_timer_expire_substate(cmd_ptr);
      break;
    case TDSRRCRCE_FINAL: 
      tdsrrcrce_process_rce_final_substate(cmd_ptr);
      break;
    default:     
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid substate in CELL_DCH State: %d",tdsrrcrce_substate);
      break;
  }  
} /* tdsrrcrce_process_cell_dch_state */

/*===========================================================================

FUNCTION tdsrrcrce_process_cell_ura_pch_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in CELL_PCH state or URA_PCH state of Connected
  mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_cell_ura_pch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  /* If network authentication check has failed then bar the serving cell */
  if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_ABORT_REQ)
      && (cmd_ptr->cmd.rrc_abort_req.auth_failed_bar_cells == TRUE)
      && (tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_CSP) == TDSRRC_CAMPED_ON))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"N/W Auth Failed. Rcvd Abort Req from MM in RRC State: %d. Cell bar req sent to L1.",tdsrrc_get_state());
    tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_CPID_BARRED, 1280);
  }
  switch (tdsrrcrce_substate)
  {
    case TDSRRCRCE_FINAL: 
      tdsrrcrce_process_rce_final_substate(cmd_ptr);
      break;
    case TDSRRCRCR_WAIT_FOR_SRB_REL_CNF:
      tdsrrcrce_process_rce_wait_for_srb_rel_cnf_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL:
      tdsrrcrce_process_rce_wait_for_sig_con_rel_compl_substate(cmd_ptr);
      break;
    case TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE:
      tdsrrcrce_process_rce_wait_for_rrc_timer_expire_substate(cmd_ptr);
      break;
    default:     
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid substate in PCH state: %d",tdsrrcrce_substate);
      break;
  }  
} /* tdsrrcrce_process_cell_ura_pch_state */

/*==========================================================================

FUNCTION tdsrrcrce_update_initial_ue_identity

DESCRIPTION
  This function updates contents of Initial UE Identity Choice and Initial
  UE Identity received from NAS TDSRRC_EST_REQ primitive.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_update_initial_ue_identity
(
  tdsrrc_InitialUE_Identity *rrc_initial_ue_identity_ext
                                             /* Initial UE Indentity to be 
                                                sent over the air */
)
{
  uint32 i;                                  /* Temporary varibale to update linked list members */
  
  switch (tdsrrc_initial_ue_identity_choice)
  {
    case RRC_IMSI_UE_ID:
      rrc_initial_ue_identity_ext->t = T_tdsrrc_InitialUE_Identity_imsi;

      rrc_initial_ue_identity_ext->u.imsi = 
        rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_IMSI_GSM_MAP);

      if(rrc_initial_ue_identity_ext->u.imsi == NULL)
      {
        ERR_FATAL("Failed to allocate memory for IMSI",0,0,0);
      }
         
      rrc_initial_ue_identity_ext->u.imsi->n = tdsrrc_initial_ue_identity.imsi.num_digits;

      ALLOC_ASN1ARRAY1(&tdsenc_ctxt, rrc_initial_ue_identity_ext->u.imsi, tdsrrc_Digit);
        
      if(rrc_initial_ue_identity_ext->u.imsi->elem == NULL)
      {
        ERR_FATAL("Failed to allocate memory for IMSI value",0,0,0);
      }

      for(i = 0; i < tdsrrc_initial_ue_identity.imsi.num_digits; i++) 
      {  
        /* Update the digit value */
        rrc_initial_ue_identity_ext->u.imsi->elem[i] = 
          (tdsrrc_Digit)tdsrrc_initial_ue_identity.imsi.value[i];
       
      }        
      break;

    case RRC_TMSI_LAI_UE_ID:
      rrc_initial_ue_identity_ext->t = T_tdsrrc_InitialUE_Identity_tmsi_and_LAI;
      rrc_initial_ue_identity_ext->u.tmsi_and_LAI =
        rtxMemAllocTypeZ(&tdsenc_ctxt, tdsrrc_TMSI_and_LAI_GSM_MAP );
      
      if(rrc_initial_ue_identity_ext->u.tmsi_and_LAI == NULL)
      {
        ERR_FATAL("Failed to allocate memory for TMSI",0,0,0);
      }
      
      /* Update the TMSI length in bits */
      rrc_initial_ue_identity_ext->u.tmsi_and_LAI->tmsi.numbits = 
          RRC_MAX_GSMMAP_TMSI_LENGTH * 8;

      for (i = 0; i < RRC_MAX_GSMMAP_TMSI_LENGTH; i++)
      {
        /* Update value for TMSI */
        *(rrc_initial_ue_identity_ext->u.tmsi_and_LAI->tmsi.data + i) =
          tdsrrc_initial_ue_identity.tmsi_lai.tmsi.value[i];
      }

      rrc_initial_ue_identity_ext->u.tmsi_and_LAI->lai.plmn_Identity.mcc.n =
        TDSRRCRCE_MCC_LENGTH;
     
      ALLOC_ASN1ARRAY1 (&tdsenc_ctxt, 
        &rrc_initial_ue_identity_ext->u.tmsi_and_LAI->lai.plmn_Identity.mcc, tdsrrc_Digit);
        
      if(rrc_initial_ue_identity_ext->u.tmsi_and_LAI->lai.plmn_Identity.mcc.elem == NULL)
      {
        ERR_FATAL("Failed to allocate memory for MCC",0,0,0);
      }
      
      for (i = 0; i < TDSRRCRCE_MCC_LENGTH ; i++)
      {
        /* Set value for MCC */
        rrc_initial_ue_identity_ext->u.tmsi_and_LAI->lai.plmn_Identity.mcc.elem[i] =
          tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.mcc[i];
      }

      rrc_initial_ue_identity_ext->u.tmsi_and_LAI->lai.plmn_Identity.mnc.n=
        tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.num_mnc_digits;
      
      ALLOC_ASN1ARRAY1 (&tdsenc_ctxt, 
        &rrc_initial_ue_identity_ext->u.tmsi_and_LAI->lai.plmn_Identity.mnc, tdsrrc_Digit);

      if(rrc_initial_ue_identity_ext->u.tmsi_and_LAI->lai.plmn_Identity.mnc.elem == NULL)
      {
        ERR_FATAL("Failed to allocate memory for MCC",0,0,0);
      } 
     
      /* Allocate memory for MNC */
      
      if(rrc_initial_ue_identity_ext->u.tmsi_and_LAI->lai.plmn_Identity.
         mnc.elem == NULL)
      {
        ERR_FATAL("Failed to allocate memory for MNC",0,0,0);
      }

      
      for (i = 0; i < tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.num_mnc_digits;
           i++)
      {
        /* Set Value for MNC */
        rrc_initial_ue_identity_ext->u.tmsi_and_LAI->lai.plmn_Identity.mnc.elem[i] =
          tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.mnc[i];
       
        
      }      
       
      
      /* Set the LAC length in bits */
      rrc_initial_ue_identity_ext->u.tmsi_and_LAI->lai.lac.numbits = 
        RRC_MAX_GSMMAP_LAC_LENGTH * 8;


      
      for (i = 0; i < RRC_MAX_GSMMAP_LAC_LENGTH; i++)
      {
        /* Update LAC value */
        *(rrc_initial_ue_identity_ext->u.tmsi_and_LAI->lai.lac.data + i ) = 
          tdsrrc_initial_ue_identity.tmsi_lai.lai.lac[i];
      } 
      
      break;

    case RRC_PTMSI_RAI_UE_ID:
      rrc_initial_ue_identity_ext->t = T_tdsrrc_InitialUE_Identity_p_TMSI_and_RAI;

       rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI =
        rtxMemAllocTypeZ(&tdsenc_ctxt, tdsrrc_P_TMSI_and_RAI_GSM_MAP );
      
      if(rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI == NULL)
      {
        ERR_FATAL("Failed to allocate memory for TMSI an rai",0,0,0);
      }
      
      /* Set PTMSI length in bits */
      rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->p_TMSI.numbits = 
        RRC_MAX_GSMMAP_PTMSI_LENGTH * 8;

      

      for (i = 0; i < RRC_MAX_GSMMAP_PTMSI_LENGTH; i++)
      {
        /*Update PTMSI value */
        *(rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->p_TMSI.data + i) =
          tdsrrc_initial_ue_identity.ptmsi_rai.ptmsi.value[i];
      }

       rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->rai.lai.plmn_Identity.mcc.n =
        TDSRRCRCE_MCC_LENGTH;
     
     ALLOC_ASN1ARRAY1 (&tdsenc_ctxt, 
        &rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->rai.lai.plmn_Identity.mcc, tdsrrc_Digit);

     if(rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->rai.lai.plmn_Identity.mcc.elem == NULL)
     {
       ERR_FATAL("No mem for mcc ",0,0,0);
     }
     
    
  
      for (i = 0; i < TDSRRCRCE_MCC_LENGTH ; i++)
      {
        /* Set value for MCC */
        rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->rai.lai.plmn_Identity.
          mcc.elem[i] = tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.mcc[i];          

      }
     rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->rai.lai.plmn_Identity.mnc.n=
        tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.num_mnc_digits;

      ALLOC_ASN1ARRAY1 (&tdsenc_ctxt, 
        &rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->rai.lai.plmn_Identity.mnc, tdsrrc_Digit);

       if(rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->rai.lai.plmn_Identity.mnc.elem == NULL)
      {
        ERR_FATAL("Failed to allocate memory for MNC",0,0,0);
      }
      

      
      for (i = 0; i < tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.num_mnc_digits;
           i++)
      {
        /* Set Value for MNC */
        rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->rai.lai.plmn_Identity.mnc.elem[i] =
          tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.mnc[i];
       
        
      }    
      
      /* Set the LAC length in bits */
      rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->rai.lai.lac.numbits = 
        RRC_MAX_GSMMAP_LAC_LENGTH * 8;

  

      
      for (i = 0; i < RRC_MAX_GSMMAP_LAC_LENGTH; i++)
      {
        /* Update LAC value */
        *(rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->rai.lai.lac.data  + i ) = 
          tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.lac[i];
      }  
      
      /* Set the RAC length in bits */
      rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->rai.rac.numbits = 
        RRC_MAX_GSMMAP_RAC_LENGTH * 8;



      for (i = 0; i < RRC_MAX_GSMMAP_RAC_LENGTH; i++)
      {
        /* Update the RAC value */
        *(rrc_initial_ue_identity_ext->u.p_TMSI_and_RAI->rai.rac.data + i ) = 
          tdsrrc_initial_ue_identity.ptmsi_rai.rai.rac[i];
      } 
      break;

    case RRC_IMEI_UE_ID:
      rrc_initial_ue_identity_ext->t = T_tdsrrc_InitialUE_Identity_imei;

      /* Allocate memory for IMEI */
      rrc_initial_ue_identity_ext->u.imei = 
          rtxMemAllocTypeZ(&tdsenc_ctxt, tdsrrc_IMEI);

      if(rrc_initial_ue_identity_ext->u.imei == NULL)
      {
        ERR_FATAL("Failed to allocate memory for IMEI",0,0,0);
      }
      
      rrc_initial_ue_identity_ext->u.imei->n = RRC_NUM_IMEI_DIGITS;
      ALLOC_ASN1ARRAY1 (&tdsenc_ctxt, 
        rrc_initial_ue_identity_ext->u.imei, tdsrrc_IMEI_Digit);
      if(rrc_initial_ue_identity_ext->u.imei->elem == NULL )
      {
        ERR_FATAL("No mem for IMEI elem",0,0,0);
      }
    
      for(i = 0; i < RRC_NUM_IMEI_DIGITS; i++) 
      {            
        
        /* Update the digit value */
        rrc_initial_ue_identity_ext->u.imei->elem[i] = 
          (tdsrrc_IMEI_Digit)tdsrrc_initial_ue_identity.imei.value[i];

       
      }       
      break;

    default:
      //This should never happen. Initial UE Identity is already validated
      ERR_FATAL("Invalid Initial UE Id t is rcvd: %d",tdsrrc_initial_ue_identity_choice,0,0);
      break;
  }
} /* tdsrrcrce_update_initial_ue_identity*/
/*==========================================================================

FUNCTION tdsrrcrce_update_establishment_cause

DESCRIPTION
  This function gets equivalent external RRC Connection Establishment cause
  for the internal RRC Connection Established cause received from the NAS 
  through TDSRRC_EST_REQ primitive.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
tdsrrc_EstablishmentCause tdsrrcrce_update_establishment_cause
(
  void
)
{
  tdsrrc_EstablishmentCause rrc_establishment_cause_ext;
                                      /* RRC Connection Establishment cause
                                         to be sent over the air */
  tdsrrc_translate_establishment_cause(tdsrrc_establishment_cause, 
                         &rrc_establishment_cause_ext);    
  return(rrc_establishment_cause_ext);
  
} /* tdsrrcrce_update_establishment_cause */
/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_initiate_cell_selection_ind

DESCRIPTION
  This function builds TDSRRC_INITIATE_CELL_SELECTION_IND command to post to
  CSP procedure with the cause specified.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it successfully sends 
  TDSRRC_INITIATE_CELL_SELECTION_IND command to CSP. Otherwise it returns 
  TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_initiate_cell_selection_ind
(
  tdsrrc_cell_sel_cause_e_type cause,           /* Cause to trigger Cell
                                                selection */
  tdsrrc_state_e_type dest_state                /* Destination State */
)
{
  tdsrrc_cmd_type *cmd_ptr;                    /* Pointer to the RRC Command */

  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MAJOR; 
                                             /* The status is initialized to
                                                Failure */
  /* Allocate memory for TDSRRC_INITIATE_CELL_SELECTION_IND command */
  if( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_INITIATE_CELL_SELECTION_IND;

    /* Fill in parameters for TDSRRC_INITIATE_CELL_SELECTION_IND command */
    cmd_ptr->cmd.initiate_cell_sel_ind.cause = cause;
    cmd_ptr->cmd.initiate_cell_sel_ind.proc  = TDSRRC_PROCEDURE_RCE;
#ifdef FEATURE_DISCONNECT_ON_UNSUPPORTED_IRAT_REDIRECTION
    if (cause == TDSRRC_TRANSITION_TO_DISCONNECTED &&
        dest_state== TDSRRC_STATE_MAX)
    {
      cmd_ptr->cmd.initiate_cell_sel_ind.dest_state = TDSRRC_STATE_DISCONNECTED;
      cmd_ptr->cmd.initiate_cell_sel_ind.tx_to_discon_reason = TDSRRC_TX_TO_DISCON_UNSUPPORTED_REDIRECTION;
    }
    else
#endif
    {
    cmd_ptr->cmd.initiate_cell_sel_ind.dest_state = dest_state;
    cmd_ptr->cmd.initiate_cell_sel_ind.tx_to_discon_reason = TDSRRC_TX_TO_DISCON_OTHER;
    }

    TDSRRC_MSG2(MSG_LEGACY_MED,"Sent internal cmd: %d with cause: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,cause);

    tdsrrc_put_int_cmd(cmd_ptr);          /* Sends the command to CSP */
    status = TDSRRCRCE_SUCCESS;           /* sets the status to success */
  }    
  else   /* RRC Task failed to allocate memory */
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unable to allocate memory for cmd: %d",TDSRRC_INITIATE_CELL_SELECTION_IND);
  } 
  tdsrrcrce_allow_ta_flag = FALSE;
  return (status);
}/*tdsrrcrce_send_rrc_initiate_cell_selection_ind */

/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_channel_config_req_for_rb0

DESCRIPTION
  This function builds TDSRRC_CHANNEL_CONFIG_REQ command and  requests RRC LLC
  to either setup or modify or release  Radio Bearer 0.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it successfully sends 
  TDSRRC_CHANNEL_CONFIG_REQ command to the RRC LLC. Otherwise it returns
  TDSRRCRCE_FAILURE_MAJOR

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_channel_config_req_for_rb0
(
               
  tdsrrc_rb_cfg_e_type action,                   /* Type of action RB 0 */
  tdsrrc_state_e_type next_state,                /* Next state to transition is
                                                 expected to take place */
  boolean confirm_required                    /* This indicates whether
                                                 TDSRRC_CHANNEL_CONFIG_CNF is 
                                                 required or not */
)
{
  tdsrrc_cmd_type *cmd_ptr;                     /* Pointer to the RRC Command */

  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MAJOR; 
                                             /* The status is initialized to
                                                Failure */  
  /* Allocates the buffer to RRC Internal command */
  if( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {     
    /* Fill in the command parameters for TDSRRC_CHANNEL_CONFIG_REQ */
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;
    cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RCE;
    cmd_ptr->cmd.chan_config_req.transaction_id = TDSGET_TRANS_ID();
    tdsrrcrce_chl_config_transaction_id = (uint8) cmd_ptr->cmd.chan_config_req.transaction_id;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Assigned transaction id = %d",tdsrrcrce_chl_config_transaction_id);

    /* Fill in parameters for TDSRRC_CHANNEL_CONFIG_REQ command */
    cmd_ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
    cmd_ptr->cmd.chan_config_req.next_state = next_state;
    cmd_ptr->cmd.chan_config_req.num_rb_to_config = 1;
    cmd_ptr->cmd.chan_config_req.rb[0].rb_id = CCCH_RADIO_BEARER_ID;
    cmd_ptr->cmd.chan_config_req.rb[0].rb_config = action;
    cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = 
      confirm_required;
  
    tdsrrc_put_int_cmd(cmd_ptr);                 /* sends the command to RRC
                                                 LLC. */
    status = TDSRRCRCE_SUCCESS;                  /* Status is initialized to
                                                 Success */           
  }
  else  /* RRC Task failed to allocate memory */
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unable to allocate memory for cmd: %d",TDSRRC_CHANNEL_CONFIG_REQ);
  }
  return (status);
}/*tdsrrcrce_send_rrc_channel_config_req_for_rb0 */

/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_est_cnf

DESCRIPTION
  This function builds RRC_EST_CNF command to post to MM with  
  success status of RRC Connection Establishment procedure.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it successfully sends 
  RRC_EST_CNF command to the MM. Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_est_cnf
(
  void
)
{
  mm_cmd_type *cmd_ptr;                      /* Pointer to the MM Command */

  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MAJOR; 
                                             /* The status is initialized to
                                                Failure */
  /* Allocate memory for RRC_EST_CNF command */
  if( (cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_EST_CNF)) != NULL)
  {
    /* No parameters to fill in for RRC_EST_CNF command */

    cmd_ptr->cmd.rrc_est_cnf.as_id = tdsrrc_get_as_id();

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent RRC_EST_CNF to MM");
 
    /* Sends the command to MM */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(cmd_ptr);
    }
    mm_put_cmd(cmd_ptr);
    status = TDSRRCRCE_SUCCESS;           /* sets the status to success */
  }    
  else   /* RRC Task failed to allocate memory */
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unable to allocate memory for cmd: %d",RRC_EST_CNF);
  }     
#ifdef FEATURE_3GPP_CSFB_TDS
  /*Connection is established, reset CSFB call status flag*/
  tdsrrc_csfb_call_status = FALSE;
  tdsrrc_csp_int_data.include_csfb_ind = FALSE;
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_csfb_call_status %d,include_csfb_ind %d",tdsrrc_csfb_call_status,tdsrrc_csp_int_data.include_csfb_ind);
#endif

#ifdef FEATURE_TDSCDMA_TO_LTE
  /* Reset flag which indicates pre-redirection info has to be included or not in connection request msg. */
  tdsrrccsp_reset_flag_for_excluding_pre_redir_info();
#endif
  return (status);
}/*tdsrrcrce_send_rrc_est_cnf */
/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_abort_ind

DESCRIPTION
  This function builds RRC_ABORT_IND command to post to MM with failure 
  status of RRC Connection Establishment procedure.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it successfully sends 
  RRC_ABORT_IND command to the MM. Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_abort_ind
(
  rrc_abort_cause_e_type cause               /* Cause for RRC Connection 
                                                Failure */
)
{
  mm_cmd_type *cmd_ptr;                      /* Pointer to the MM Command */

  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MAJOR; 
                                             /* The status is initialized to
                                                Failure */

  /* send abort cause as LAI/RAI changed to NAS, so that NAS can attempt silent redial 
     for a pending MO call after LU is complete. */
  if(tdsrrcrce_lai_rai_mismatch)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Changed abort cause from %d to %d",cause,RRC_ABORT_LAI_RAI_CHANGED);
    cause =  RRC_ABORT_LAI_RAI_CHANGED;
  }

  /* Allocate memory for RRC_ABORT_IND command */
  if( (cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_ABORT_IND)) != NULL)
  {
    /* Send abort indication with cause "no resource" if no resource flag is set
     * so that NAS will retry happens within 2 seconds, or else, NAS will retry long time latter*/
    if(TRUE == tdsrrc_get_oos_due_to_no_resource_flag_status())
    {
      cmd_ptr->cmd.rrc_abort_ind.abort_cause = RRC_NO_RESOURCE_AVAILABLE;
    }  
    else
    {
      cmd_ptr->cmd.rrc_abort_ind.abort_cause = cause;
    }

    cmd_ptr->cmd.rrc_abort_ind.as_id = tdsrrc_get_as_id();

    /* NAS should look into this value only when abort cause is RRC_ABORT_CONN_REJECT_WAIT_TMR */
    cmd_ptr->cmd.rrc_abort_ind.conn_rej_wait_timer = tdsrrcrce_wait_time;

    /* Update the Max HFNs values to be stored in SIM or USIM */ 
    (void) tdsrrcsmc_append_hfns_to_rel_ind(cmd_ptr->cmd.rrc_abort_ind.hfn_cs,
                                         cmd_ptr->cmd.rrc_abort_ind.hfn_ps);

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sent RRC_ABORT_IND to MM with cause: %d",cause);

    /* Sends the command to MM */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(cmd_ptr);
    }
    mm_put_cmd(cmd_ptr);
    status = TDSRRCRCE_SUCCESS;           /* sets the status to success */
    
    /* NAS requested to indicate the domain for which abort is sent with cause access class not allowed 
      * Release event still uses the old cause*/ 
    if((cause == RRC_AC_NOT_ALLOWED_CS_DOMAIN) || (cause == RRC_AC_NOT_ALLOWED_PS_DOMAIN))
    {
      tdsrrcrce_send_conn_rel_cause_event(RRC_ACCESS_CLASS_NOT_ALLOWED);
    }
    else
    {
      tdsrrcrce_send_conn_rel_cause_event(cause);
    }
  }    
  else   /* RRC Task failed to allocate memory */
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unable to allocate memory for cmd: %d",RRC_ABORT_IND);
  }     

  tdsrrcwrm_release_lock();
  tdsrrc_proc_type = SYS_PROC_TYPE_NONE; 

#ifdef FEATURE_3GPP_CSFB_TDS
  /*Connection is aborted, reset CSFB call status flag*/
  tdsrrc_csfb_call_status = FALSE;
  tdsrrc_csp_int_data.include_csfb_ind = FALSE;
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_csfb_call_status %d,include_csfb_ind %d",tdsrrc_csfb_call_status,tdsrrc_csp_int_data.include_csfb_ind);
#endif

#ifdef FEATURE_TDSCDMA_TO_LTE
  /* Reset flag which indicates pre-redirection info has to be included or not in connection request msg. */
  tdsrrccsp_reset_flag_for_excluding_pre_redir_info();
#endif

#ifdef FEATURE_QSH_EVENT_METRIC
/* report SYS_RLF_WEAK_SIGNAL_RANDOM_ACCESS_FAILURE for all the causes in enum rrc_abort_cause_e_type. */
  tdsrrcqsh_update_rlf_or_weak_signal_failure(SYS_RLF_WEAK_SIGNAL_RANDOM_ACCESS_FAILURE);
#endif  

  tdsrrc_release_cause = RRC_REL_CAUSE_UNSPEC;
  return (status);
}/*tdsrrcrce_send_rrc_abort_ind */
/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_abort_cnf

DESCRIPTION
  This function builds RRC_ABORT_CNF command to post to MM in response to 
  TDSRRC_ABORT_REQ.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it successfully sends 
  RRC_ABORT_CNF command to the MM. Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_abort_cnf
(
  void
)
{
  mm_cmd_type *cmd_ptr;                      /* Pointer to the MM Command */

  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MAJOR; 
                                             /* The status is initialized to
                                                Failure */
  tdsrrcrce_send_abort_cnf = FALSE;

  tdsrrcrce_sig_con_delay = FALSE;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Send Abort Cnf in %d substate",tdsrrcrce_substate);

  /*clear the TDSRRCTMR_SOFT_ABORT_GUARD_TIMER whenever the abort cnf is sent*/
  tdsrrctmr_stop_timer(TDSRRCTMR_SOFT_ABORT_GUARD_TIMER);


  /* Allocate memory for RRC_ABORT_CNF command */
  if( (cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_ABORT_CNF)) != NULL)
  {
    /* No paramters to fill in for RRC_ABORT_CNF command */

    cmd_ptr->cmd.rrc_abort_cnf.as_id = tdsrrc_get_as_id();

    /* Update the Max HFNs values to be stored in SIM or USIM */ 
    (void) tdsrrcsmc_append_hfns_to_rel_ind(cmd_ptr->cmd.rrc_abort_cnf.hfn_cs,
                                         cmd_ptr->cmd.rrc_abort_cnf.hfn_ps);

    cmd_ptr->cmd.rrc_abort_cnf.conn_rel_cause = tdsrrc_release_cause;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sent RRC_ABORT_CNF to MM with rel_cause %d",cmd_ptr->cmd.rrc_abort_cnf.conn_rel_cause);

    /* Sends the command to MM */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(cmd_ptr);
    }
    mm_put_cmd(cmd_ptr);

    status = TDSRRCRCE_SUCCESS;           /* sets the status to success */

    tdsrrcrce_send_conn_rel_cause_event((uint8)RRC_ABORT_CAUSE_TYPE_MAX + (uint8)tdsrrc_release_cause);   
  }    
  else   /* RRC Task failed to allocate memory */
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unable to allocate memory for cmd: %d",RRC_ABORT_CNF);
  }     
  tdsrrcwrm_release_lock();
  tdsrrc_proc_type = SYS_PROC_TYPE_NONE; 
#ifdef FEATURE_3GPP_CSFB_TDS
  /*Connection is aborted, reset CSFB call status flag*/
  tdsrrc_csfb_call_status = FALSE;
  tdsrrc_csp_int_data.include_csfb_ind = FALSE;
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_csfb_call_status %d,include_csfb_ind %d",tdsrrc_csfb_call_status,tdsrrc_csp_int_data.include_csfb_ind);
#endif

  /* Initialize release cause */
  tdsrrc_release_cause = RRC_REL_CAUSE_UNSPEC;

  return (status);
}/*tdsrrcrce_send_rrc_abort_cnf */

/*===========================================================================

FUNCTION RRCRCE_SEND_CONN_SETUP_IND

DESCRIPTION
  This function builds TDSRRC_CONN_SETUP_IND command to post to CHO proc.The calling
  function should pass in the paramters to be sent in the command. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_send_conn_setup_ind(tdsrrc_conn_setup_ind_type conn_setup_ind_params)
{
  tdsrrc_cmd_type *int_cmd_ptr = NULL;

  /* Allocates the buffer to RRC Internal command */
  if( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {     
    /* Fill in the command parameters for TDSRRC_CONN_SETUP_IND */
    int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CONN_SETUP_IND;
    int_cmd_ptr->cmd.rrc_conn_setup_ind.status = conn_setup_ind_params.status;
    int_cmd_ptr->cmd.rrc_conn_setup_ind.conn_failure_reason = 
      conn_setup_ind_params.conn_failure_reason;

    /* Sends conn_setup_ind to cho procedure */
    tdsrrc_put_int_cmd(int_cmd_ptr);     
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sending TDSRRC_CONN_SETUP_IND to CHO.Status:%d,Failure reason %d",int_cmd_ptr->cmd.rrc_conn_setup_ind.status,int_cmd_ptr->cmd.rrc_conn_setup_ind.conn_failure_reason);

  }
  else  /* RRC Task failed to allocate memory */
  {
    ERR_FATAL("Memory alloc failed for Internal Cmd: %d",TDSRRC_CONN_SETUP_IND,0,0);
  }
}
/*===========================================================================

FUNCTION tdsrrcrce_validate_rrc_est_req

DESCRIPTION
  This function checks whether TDSRRC_EST_REQ contains a valid Initial UE
  indentity.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it finds a valid
  Initial UE Identity in TDSRRC_EST_REQ command to the MM.
  Otherwise it returns TDSRRCRCE_FAILURE_MINOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_validate_rrc_est_req
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC Command */
)
{
  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MINOR; 
                                             /* The status is initialized to
                                                Failure */
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_EST_REQ)
  {
    switch(cmd_ptr->cmd.rrc_est_req.ue_id_choice)
    {
      case RRC_IMSI_UE_ID:
        if(cmd_ptr->cmd.rrc_est_req.initial_ue_id.imsi.num_digits < 6)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid IMSI length is sent");
        }
        else
        {
          status = TDSRRCRCE_SUCCESS;
        }
        break;
      case RRC_TMSI_LAI_UE_ID:
        if ((cmd_ptr->cmd.rrc_est_req.initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits == 3) &&
            (cmd_ptr->cmd.rrc_est_req.initial_ue_id.tmsi_lai.lai.plmn.mnc[2] > 9))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"MM Sent mnc as 3 digits and last digit %d",cmd_ptr->cmd.rrc_est_req.initial_ue_id.tmsi_lai.lai.plmn.mnc[2]);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set mnc to 2 digits");
          cmd_ptr->cmd.rrc_est_req.initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits = 2;
        }
        status = TDSRRCRCE_SUCCESS;
        break;

      case RRC_PTMSI_RAI_UE_ID:
        if ((cmd_ptr->cmd.rrc_est_req.initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits == 3) &&
            (cmd_ptr->cmd.rrc_est_req.initial_ue_id.ptmsi_rai.rai.lai.plmn.mnc[2] > 9))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"MM Sent mnc as 3 digits and last digit %d",cmd_ptr->cmd.rrc_est_req.initial_ue_id.ptmsi_rai.rai.lai.plmn.mnc[2]);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set mnc to 2 digits");
          cmd_ptr->cmd.rrc_est_req.initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits = 2;
        }
        status = TDSRRCRCE_SUCCESS;
        break;
    
    
      case RRC_IMEI_UE_ID:
        status = TDSRRCRCE_SUCCESS;
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Initial UE Identity: %d",cmd_ptr->cmd.rrc_est_req.ue_id_choice);
        break;
    }
  }
  return (status);

}/*tdsrrcrce_validate_rrc_est_req */
/*===========================================================================

FUNCTION tdsrrcrce_process_rce_initial_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in RCE_INITIAL substate of Disconnected State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_initial_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  rrc_abort_cause_e_type  cause = RRC_MAX_ACCESS_FAILURE;

  uint64 tdsrrcrce_wait_reselection_timer_remain = 0;

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_EST_REQ: 

      if(tdsrrccsp_get_csp_substate() == TDSRRCCSP_WT_SUSPEND_MODE_CHANGE_CNF
         && tdsrrcmcm_get_rrc_mode() == TDSRRC_MODE_ACTIVE)
      {
        /*process this race condition:
        1. CSP receives TDSRRC_CPHY_CELL_RESELECTION_IND(L1 command queue)
        2. CSP sends TDSRRC_CHANGE_MODE_REQ to MCM(internal command queue) 
        3. RRC receives TDSRRC_EST_REQ(mm command queue) before MCM processes TDSRRC_CHANGE_MODE_REQ which is in internal command queue) .*/    
        if(TDSRRCRCE_FAILURE_MAJOR == tdsrrcrce_send_rrc_abort_ind(RRC_FAILURE_UNSPECIFIED))      
        {     
          ERR_FATAL("Unable to post MM Command: %d",RRC_ABORT_IND,0,0);
        } 
        break;
      }   
      
      if (TRUE == tdsrrccsp_reselection_in_progress())
      {
        if (tdsrrcrce_substate == TDSRRCRCE_WAIT_NEW_CELL_IND)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"have received TDSRRC_EST_REQ before,omit this time");
          break;
        }

        /*Need to consider race condition and the est_req is a poped up one due to back to back CSP reselection*/
        if (tdsrrcrce_temp_cmd_ptr != NULL)
        {
          /*this is due to back to back reselection*/ 
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"This is due to back to back Cell Reselection,tdsrrcrce_temp_cmd_ptr=0x%X",tdsrrcrce_temp_cmd_ptr);
          /*The timer will be restart after return and still honor the previous 5s timer*/
        }
        else
        {
          
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Hold off TDSRRC_EST_REQ because of cell reselecting");
          tdsrrcrce_temp_cmd_ptr = (tdsrrc_cmd_type *) tdsrrc_get_mm_cmd_buf();
          memscpy(tdsrrcrce_temp_cmd_ptr, sizeof(tdsrrc_mm_cmd_type), cmd_ptr, sizeof(tdsrrc_mm_cmd_type));
        
          if(TDSRRCRCE_SUCCESS == tdsrrcrce_validate_rrc_est_req(cmd_ptr))
          {
            tdsrrc_establishment_cause = cmd_ptr->cmd.rrc_est_req.cause;
            tdsrrc_est_cn_domain_id = cmd_ptr->cmd.rrc_est_req.cn_domain_id;
            tdsrrc_proc_type = cmd_ptr->cmd.rrc_est_req.proc_type;
          }
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Start 5s timer in case cell reselect cannot finish in time,est_cause is %d ",cmd_ptr->cmd.rrc_est_req.cause);
          tdsrrctmr_start_timer(TDSRRCTMR_RCE_WAIT_RESELCT_TIMER, 
                            (unsigned long) TDSRRCRCE_TIMER_VALUE_FOR_WAIT_RESELCT);
        }
        
        /*Set to TDSRRCRCE_WAIT_NEW_CELL_IND whenever tdsrrcrce_temp_cmd_ptr is valid*/
        tdsrrcrce_substate = TDSRRCRCE_WAIT_NEW_CELL_IND;
        
        break;
      }
      
      
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rcvd TDSRRC_EST_REQ from MM with cause:%d",cmd_ptr->cmd.rrc_est_req.cause);

      tdsrrc_protocol_error_indicator = tdsrrc_ProtocolErrorIndicator_noError;

      /* Resets the V300 variable */
      tdsrrc_v_300 = 0;
   
      /* Initialize the Transaction Id for RRC CONNECTION SETUP to Invalid */
      tdsrrcrce_setup_transaction_identifier = TDSRRCRCE_INVALID_TRANSACTION_ID;
      
      /* Initialize the Transaction Id for RRC CONNECTION REJECT to Invalid */
      tdsrrcrce_reject_transaction_identifier = TDSRRCRCE_INVALID_TRANSACTION_ID;

      /* Initialize the Wait Time to zero seconds */
      tdsrrcrce_wait_time = 0;

      /* Initialize abort flag to FALSE */
      tdsrrcrce_send_abort_cnf = FALSE;

      tdsrrcrce_abort_ind_sent = FALSE;
      tdsrrcrce_lai_rai_mismatch = FALSE;

      /* Initialize release cause to unspecified */
      tdsrrc_release_cause = RRC_REL_CAUSE_UNSPEC;

      if(TDSRRCRCE_SUCCESS == tdsrrcrce_validate_rrc_est_req(cmd_ptr))
      {
        /*updates the establishment cause */
        tdsrrc_establishment_cause = cmd_ptr->cmd.rrc_est_req.cause;
        tdsrrc_proc_type = cmd_ptr->cmd.rrc_est_req.proc_type;
        /* Updates Initial UE Identity */
        tdsrrc_initial_ue_identity_choice = cmd_ptr->cmd.rrc_est_req.ue_id_choice;

        tdsrrc_initial_ue_identity = cmd_ptr->cmd.rrc_est_req.initial_ue_id;

        tdsrrc_est_cn_domain_id = cmd_ptr->cmd.rrc_est_req.cn_domain_id;

        tdsrrc_est_call_type = cmd_ptr->cmd.rrc_est_req.call_type;

        tdsrrc_est_req_for_rai = cmd_ptr->cmd.rrc_est_req.rai;

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_EST_REQ for cn_domain: %d",tdsrrc_est_cn_domain_id);
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAI PLMN: %d-%d ",TDSRRC_CSP_GET_INT_MCC(tdsrrc_est_req_for_rai.lai.plmn)
                                  ,TDSRRC_CSP_GET_INT_MNC(tdsrrc_est_req_for_rai.lai.plmn)
                                  ,0);
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"LAC: %x %x",tdsrrc_est_req_for_rai.lai.lac[0],tdsrrc_est_req_for_rai.lai.lac[1]);
        if(tdsrrc_est_cn_domain_id != RRC_CS_DOMAIN_CN_ID)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAC: %d",tdsrrc_est_req_for_rai.rac[0]);
        }

        tdsrrcrce_send_conn_req_cause_event(tdsrrc_establishment_cause);

        /* Function call to CSP to know whether UE is camped on  */
        if (TDSRRC_CAMPED_ON == tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_RCE))
        {
          if(TDSRRCRCE_SUCCESS == tdsrrcrce_validate_cell_access_status(
                                   tdsrrc_est_cn_domain_id,
                                   tdsrrc_establishment_cause,
                                   &cause)
            )
          {
            if(TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
            {
              /* Due to race condition because of "context-switching", 
               * RCE needs to update it's substate before putting 
               * TDSRRC_BPLMN_SUSPEND_REQ in the queue 
               */
              tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_UE_CAMP_ON;

              /* With the below feature defined we will suspend and resume
               * the BPLMN search for any establishment cause received 
               * from NAS
               */
              tdsrrccsp_send_bplmn_suspend_req(TDSRRC_PROCEDURE_RCE, TDSBPLMN_SUSPEND_REASON_REGISTRATION);

              /* Wait for NEW_CELL_IND in TDSRRCRCE_WAIT_FOR_UE_CAMP_ON substate */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE waiting for SUSPEND_CNF from CSP. BPLMN srch in prog");
            
              /* Starting this timer is NOT required as CSP on receiving TDSRRC_BPLMN_SUSPEND_REQ
               * will send TDSRRC_BPLMN_SUSPEND_CNF almost immediately. This timer is started only 
               * as a recovery mechanism in case CSP fails to send TDSRRC_BPLMN_SUSPEND_CNF to RCE
               */
              tdsrrctmr_start_timer(TDSRRCTMR_UE_TO_CAMP_ON_TIMER, 
                            (unsigned long) TDSRRCRCE_TIMER_VALUE_FOR_UE_TO_CAMP_ON);       
            }
            else
            {
              if(tdsrrcwrm_acquire_lock_for_est_req(tdsrrc_est_cn_domain_id,0xfffffff,TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_EST_REQ_IN_MS) == FAILURE)
              {
                /* Send RRC_ABORT_IND command to the MM */
                if(TDSRRCRCE_SUCCESS != 
                   tdsrrcrce_send_rrc_abort_ind(RRC_NO_RESOURCE_AVAILABLE) )      
                {
                 
                  ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
                } 
                return;
              }
               /* UE is camped on, then Check whether SIB7 is current */
              if(
                 (TRUE == tdsrrcsib_check_and_update_sib7_validity (TDSRRC_PROCEDURE_RCE) )
                )
              { 
                /* UE is camped on and SIB7 is current, then Setup RB0 */
                if( TDSRRCRCE_SUCCESS == 
                    tdsrrcrce_send_rrc_channel_config_req_for_rb0(TDSESTABLISH_RB,
                                                               TDSRRC_STATE_CONNECTING,
                                                               TRUE ))  
                {              
                  tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF;     
                }    
                else  /* Failed to send a internal command to RRC LLC*/
                {     
                  ERR_FATAL("Unable to send internal cmd to LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);         
                }
              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE waits for SIB7 to become current"); 
                tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_SIB7_CURRENT;
              }
            }
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH," Cause value filled in RRC_ABORT_IND cmd= %d ",cause);
             if(tdsrrc_est_cn_domain_id == RRC_CS_DOMAIN_CN_ID)
            {
              if(TDSRRCRCE_SUCCESS != tdsrrcrce_send_rrc_abort_ind(RRC_AC_NOT_ALLOWED_CS_DOMAIN) )
              {
                ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
              }
            }
            else
            {
              if(TDSRRCRCE_SUCCESS != tdsrrcrce_send_rrc_abort_ind(RRC_AC_NOT_ALLOWED_PS_DOMAIN) )
              {
                ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
              }
            }

            if(tdsrrc_establishment_cause == RRC_EST_EMERGENCY_CALL)
            {
              if(TDSRRCRCE_SUCCESS != 
                   tdsrrcrce_send_rrc_initiate_cell_selection_ind(TDSRRC_OUT_OF_SERVICE_AREA_IND,tdsrrc_get_state()))
              {
                ERR_FATAL("Unable to post an internal Command: %d",TDSRRC_OUT_OF_SERVICE_AREA_IND,0,0);
              }
              tdsrrccsp_set_est_req_for_emerg_call();
            }
          }
        }
        else /* UE is not camped on */
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE waiting for UE to camp on");
          tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_UE_CAMP_ON;

          /* start the timer for UE to camp on */
          tdsrrctmr_start_timer(TDSRRCTMR_UE_TO_CAMP_ON_TIMER, 
                        (unsigned long) TDSRRCRCE_TIMER_VALUE_FOR_UE_TO_CAMP_ON);       
        }
      }
      else /* Validation Failed */
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Validation failure in TDSRRC_EST_REQ");

#ifdef FEATURE_INTERRAT_PCCO_G2TD
        /* If GTOTDS is in progress,send a conn_setup_ind with failure to CHO
           CHO would deactivate TDS and pass control back to GSM.Do not send
           RRC_ABORT_IND to MM if GTOTDS is active */
        if(tdsrrccho_is_g2td_pcco_in_progress())
        { 
          tdsrrc_conn_setup_ind_type conn_setup_ind_params;
          conn_setup_ind_params.status = FAILURE;
          conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Validation failed in TDSRRC_EST_REQ when PCCO is active");
          tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  

          /* Reset RCE susbstate */
          tdsrrcrce_substate = TDSRRCRCE_INITIAL;
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          return;
        }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

        /* Send RRC_ABORT_IND command to the MM */
        if(TDSRRCRCE_SUCCESS != 
           tdsrrcrce_send_rrc_abort_ind(RRC_UNSUPPORTED_INITIAL_UE_ID) )      
        {
         
          ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
        } 

      }
      break;

    case TDSRRC_NEW_CELL_IND:   
      /* This Command will come from CSP in TDSRRCRCE_INITIAL substate if RRC
         is receives TDSRRC_ABORT_REQ from NAS while  waiting for UE
         to camp on and in that case RCE simply ignores it*/       
    case TDSRRC_CELL_RESELECTION_FAILURE_IND:
      // CR 352866 begin
      if (tdsrrcrce_substate == TDSRRCRCE_WAIT_NEW_CELL_IND)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE has pending TDSRRC_EST_REQ,cell reselction finished");
        if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_NEW_CELL_IND)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"cell reselect succeed, stop wait timer,resume RCE");
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"cell reselect fail, stop wait timer,resume RCE");
        }
        /*as SIB7 is still using the timer, keep the logic local*/
        tdsrrcrce_wait_reselection_timer_remain 
          = tdsrrctmr_get_remaining_time (TDSRRCTMR_RCE_WAIT_RESELCT_TIMER);

        tdsrrctmr_stop_timer(TDSRRCTMR_RCE_WAIT_RESELCT_TIMER);
        tdsrrcrce_substate = TDSRRCRCE_INITIAL;
        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        /*tdsrrcrce_temp_cmd_ptr should not be NULL for current sub_state, add check to satisfy PW 
          We still need set the sub_state to initial as above*/
        if(tdsrrcrce_temp_cmd_ptr != NULL)
        {
          tdsrrcrce_procedure_event_handler(tdsrrcrce_temp_cmd_ptr);
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"tdsrrcrce_temp_cmd_ptr is NULL in rce substate %d",tdsrrcrce_substate);
        }

        /*in case the est req has been rebuffered due to back to back reselection*/
        if((tdsrrcrce_temp_cmd_ptr != NULL) && (tdsrrcrce_substate != TDSRRCRCE_WAIT_NEW_CELL_IND))
        {
          tdsrrc_free_cmd_buf(tdsrrcrce_temp_cmd_ptr);
          tdsrrcrce_temp_cmd_ptr = NULL;
        }
        else
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR, "Back to back reselection: tdsrrcrce_temp_cmd_ptr: 0x%X, rce-substate: %d, remain timer: %d", 
            tdsrrcrce_temp_cmd_ptr, tdsrrcrce_substate, tdsrrcrce_wait_reselection_timer_remain);
          /*restart the timer as it is stopped*/
          tdsrrctmr_start_timer(TDSRRCTMR_RCE_WAIT_RESELCT_TIMER, 
                            tdsrrcrce_wait_reselection_timer_remain);

        }
        break;
      }
      // CR 352866 end
      break;
    // CR 352866 begin
    case TDSRRC_TIMER_TO_RCE_WAIT_RESELCT_EXPIRED_IND:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_TIMER_TO_RCE_WAIT_RESELCT_EXPIRED_IND in rce substate %d",tdsrrcrce_substate);
      if (tdsrrcrce_substate == TDSRRCRCE_WAIT_NEW_CELL_IND)
      {
        boolean is_resel_aborted = FALSE;

        if(TRUE == tdsrrccsp_reselection_in_progress())
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Abort cell reselection as wait reselect timer expired");
          is_resel_aborted = tdsrrccsp_abort_cell_reselection();
        }

        tdsrrcrce_substate = TDSRRCRCE_INITIAL;
        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;

        if(tdsrrcrce_temp_cmd_ptr != NULL)
        {
          if(is_resel_aborted)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Handle EST_REQ as wait reselect timer expired");
            tdsrrcrce_procedure_event_handler(tdsrrcrce_temp_cmd_ptr);
          }

          /* In case the est req has been rebuffered due to reselection can't be aborted immediately */
          if((tdsrrcrce_temp_cmd_ptr != NULL) && (tdsrrcrce_substate != TDSRRCRCE_WAIT_NEW_CELL_IND))
          {
            tdsrrc_free_cmd_buf(tdsrrcrce_temp_cmd_ptr);
            tdsrrcrce_temp_cmd_ptr = NULL;

            /* Send RRC_ABORT_IND command to the MM */
            if(TDSRRCRCE_SUCCESS != 
               tdsrrcrce_send_rrc_abort_ind(RRC_MAX_ACCESS_FAILURE) )
            {
              ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
            }

            tdsrrcrce_substate = TDSRRCRCE_INITIAL;        
            tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          }
          else
          {
            /* Now, don't send RRC_ABORT_IND to NAS, just abort reselection and handle EST_REQ */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_ABORT_IND is not sent to NAS! Abort reselection and handle EST_REQ");
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Ptr Wrong! tdsrrcrce_temp_cmd_ptr is NULL");
        }
      }
      break;
    // CR 352866 end

    case TDSRRC_STATE_CHANGE_IND:
      TDSRRC_MSG2(MSG_LEGACY_MED,"Unexpected Event: %d in the substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;

    case TDSRRC_ABORT_REQ:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ rcvd in %d substate",tdsrrcrce_substate);
      /* Send RRC_ABORT_CNF command to the MM */
      if(TDSRRCRCE_SUCCESS == 
         tdsrrcrce_send_rrc_abort_cnf() )      
      { 
        if(tdsrrcrce_temp_cmd_ptr != NULL)
        {
          tdsrrc_free_cmd_buf(tdsrrcrce_temp_cmd_ptr);
          tdsrrcrce_temp_cmd_ptr = NULL;
        }
        
        tdsrrctmr_stop_timer(TDSRRCTMR_RCE_WAIT_RESELCT_TIMER);

        /* At this time, none of the RBs are setup. Simply reset the RCE
         substate */
         tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */
         tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
      }    
      else
      {     
        ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
      } 
      break;

    default:     
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;
  }  
} /* tdsrrcrce_process_rce_initial_substate */

/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_sib7_current

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in TDSRRCRCE_WAIT_FOR_SIB7_CURRENT  substate of 
  Disconnected State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_sib7_current
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  rrc_abort_cause_e_type  cause = RRC_MAX_ACCESS_FAILURE;

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_SIB7_UPDATE_STATUS_IND: /* This command actually represents
                                        that SIB 7 is current */   
      if (cmd_ptr->cmd.sib7_update_status.status == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB7 Update Status Ind with failure");

#ifdef FEATURE_INTERRAT_PCCO_G2TD
        /* If GTOTDS is in progress,send a conn_setup_ind with failure to CHO.
           CHO would deactivate TDS and pass control back to GSM.Do not send
           RRC_ABORT_IND to MM if GTOTDS is active */
        if(tdsrrccho_is_g2td_pcco_in_progress())
        { 
          tdsrrc_conn_setup_ind_type conn_setup_ind_params;
          conn_setup_ind_params.status = FAILURE;
          conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB7 Update failure when PCCO is active");
          tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);
          
          /* The RCE Procedure ends */
          tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          break;
        }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */
        
        if(TDSRRCRCE_SUCCESS == tdsrrcrce_send_rrc_abort_ind(RRC_INTERNAL_ERROR) )      
        {
          /* The RCE Procedure ends */
          tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          break;
        }    
        else
        {     
          ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_IND,0,0);
        }    
      }

//if getting SIB7 IND correctly, abort RB0 setup until CSP finished
      // CR 352866 begin
      if (TRUE == tdsrrccsp_reselection_in_progress())
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"cell reselect breakes SIB7 reading");
        tdsrrcrce_substate = TDSRRCRCE_WAIT_NEW_CELL_IND_SIB7;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"start 5s timer for cell reselect can not finish");
        tdsrrctmr_start_timer(TDSRRCTMR_RCE_WAIT_RESELCT_TIMER, 
                            (unsigned long) TDSRRCRCE_TIMER_VALUE_FOR_WAIT_RESELCT);
        break;
      }
      // CR 352866 end

      /*If RRC is already holding the Lock, don't need to acquire lock again*/
      if(tdrm_check_cli_state_is_in_lock(TDRM_CLIENT_RRC))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Lock is already in RRC.");
      }
      else if(tdsrrcwrm_acquire_lock_for_est_req(tdsrrc_est_cn_domain_id,0xfffffff,TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_EST_REQ_IN_MS) == FAILURE)
      {
        /* Send RRC_ABORT_IND command to the MM */
        if(TDSRRCRCE_SUCCESS != 
            tdsrrcrce_send_rrc_abort_ind(RRC_NO_RESOURCE_AVAILABLE) )			
        {									 
          ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
        } 
        /* The RCE Procedure ends */
        tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */
        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        break;
      }

      /* Send the internal command to RRC LLC to establish RB 0 */
      if( TDSRRCRCE_SUCCESS ==
        tdsrrcrce_send_rrc_channel_config_req_for_rb0(TDSESTABLISH_RB,
        TDSRRC_STATE_CONNECTING,
        TRUE) )  
      {              
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF;     
      }    
      else  /* Failed to send a internal command to RRC LLC.*/   
      {  
        ERR_FATAL("Unable to send internal cmd to LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);         
      }
      break;

    case TDSRRC_ABORT_REQ:

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ rcvd in %d substate",tdsrrcrce_substate);

      /* Call SIB fn to forcefully initialize its active event if any */
      tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_SIB7_UPDATE);

      /* Send RRC_ABORT_CNF command to the MM */
      if(TDSRRCRCE_SUCCESS == 
         tdsrrcrce_send_rrc_abort_cnf() )      
      {        
        /* At this time, none of the RBs are setup. Simply reset the RCE
         substate */
         tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */
         tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
      }    
      else
      {     
        ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
      }    
      break;

    case TDSRRC_NEW_CELL_IND:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Got TDSRRC_NEW_CELL_IND in RCE sub statewait_for_sib7_current ");
    //Due to aborted RB0 Setup, we need resume here
    case TDSRRC_CELL_RESELECTION_FAILURE_IND:
      // CR 352866 begin
      if (tdsrrcrce_substate == TDSRRCRCE_WAIT_NEW_CELL_IND_SIB7)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE hold SIB7 receive, cell reselction finished,stop wait timer");
        if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_NEW_CELL_IND)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"cell reselect find new cell,");
        }
        tdsrrctmr_stop_timer(TDSRRCTMR_RCE_WAIT_RESELCT_TIMER);
        //when returned back to old cell
        if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_CELL_RESELECTION_FAILURE_IND)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"cell reselect returns to old cell,");
          /*checking SIB7 current is not needed as the sub-state was set when the old SIB7 is current */
          /*
            if(TRUE == tdsrrcsib_check_and_update_sib7_validity (TDSRRC_PROCEDURE_RCE) )
            { 
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Old cell SIB7 is current");
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Wait old cell's SIB7 become current");
              tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_SIB7_CURRENT;
              break;
            }
           */
        }

      }
      // CR 352866 end
      if(TDSRRCRCE_SUCCESS != tdsrrcrce_validate_cell_access_status(
                            tdsrrc_est_cn_domain_id,
                            tdsrrc_establishment_cause,
                            &cause)
        )
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH," Cause value filled in RRC_ABORT_IND cmd= %d,rrcrce sub state=%d",cause,tdsrrcrce_substate);
        if(tdsrrc_est_cn_domain_id == RRC_CS_DOMAIN_CN_ID)
        {
          if(TDSRRCRCE_SUCCESS != tdsrrcrce_send_rrc_abort_ind(RRC_AC_NOT_ALLOWED_CS_DOMAIN) )
          {
            ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
          }
        }
        else
        {
          if(TDSRRCRCE_SUCCESS != tdsrrcrce_send_rrc_abort_ind(RRC_AC_NOT_ALLOWED_PS_DOMAIN) )
          {
            ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
          }
        }

        tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */
        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Bring rce state to TDSRRCRCE_INITIAL since AC status is not suffice to go ahead with call setup ");
        break;
      }

      /*If RRC is already holding the Lock, don't need to acquire lock again*/
      if(tdrm_check_cli_state_is_in_lock(TDRM_CLIENT_RRC))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Lock is already in RRC.");
      }
      else if(tdsrrcwrm_acquire_lock_for_est_req(tdsrrc_est_cn_domain_id,0xfffffff,TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_EST_REQ_IN_MS) == FAILURE)
      {
        /* Send RRC_ABORT_IND command to the MM */
        if(TDSRRCRCE_SUCCESS != 
            tdsrrcrce_send_rrc_abort_ind(RRC_NO_RESOURCE_AVAILABLE) )     
        {                  
          ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
        } 
        /* The RCE Procedure ends */
        tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */
        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        break;
      }

      /* Send the internal command to RRC LLC to establish RB 0 */
      if( TDSRRCRCE_SUCCESS ==
        tdsrrcrce_send_rrc_channel_config_req_for_rb0(TDSESTABLISH_RB,
        TDSRRC_STATE_CONNECTING,
        TRUE) )  
      {              
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF;     
      }    
      else  /* Failed to send a internal command to RRC LLC.*/   
      {  
        ERR_FATAL("Unable to send internal cmd to LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);         
      }
      break;

    // CR 352866 begin
    case TDSRRC_TIMER_TO_RCE_WAIT_RESELCT_EXPIRED_IND:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_TIMER_TO_RCE_WAIT_RESELCT_EXPIRED_IND in rce substate %d",tdsrrcrce_substate);
      if (tdsrrcrce_substate == TDSRRCRCE_WAIT_NEW_CELL_IND_SIB7)
      {
        boolean is_resel_aborted = FALSE;

        /* Abort Reselection, Handle EST_REQ */
        if(TRUE == tdsrrccsp_reselection_in_progress())
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Abort cell reselection as wait reselect timer expired");
          is_resel_aborted = tdsrrccsp_abort_cell_reselection();
        }

        tdsrrcrce_substate = TDSRRCRCE_INITIAL;        
        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;

        if(tdsrrcrce_temp_cmd_ptr != NULL)
        {
          if(is_resel_aborted)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Handle EST_REQ as wait reselect timer expired");
            tdsrrcrce_procedure_event_handler(tdsrrcrce_temp_cmd_ptr);
          }

          /* In case the est req has been rebuffered due to reselection can't be aborted immediately */
          if((tdsrrcrce_temp_cmd_ptr != NULL) && (tdsrrcrce_substate != TDSRRCRCE_WAIT_NEW_CELL_IND))
          {
            tdsrrc_free_cmd_buf(tdsrrcrce_temp_cmd_ptr);
            tdsrrcrce_temp_cmd_ptr = NULL;

            /* Send RRC_ABORT_IND command to the MM */
            if(TDSRRCRCE_SUCCESS != 
               tdsrrcrce_send_rrc_abort_ind(RRC_MAX_ACCESS_FAILURE) )
            {
              ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
            }

            tdsrrcrce_substate = TDSRRCRCE_INITIAL;        
            tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          }
          else
          {
            /* Now, don't send RRC_ABORT_IND to NAS, just abort reselection and handle EST_REQ */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_ABORT_IND is not sent to NAS! Abort reselection and handle EST_REQ");
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Ptr Wrong! tdsrrcrce_temp_cmd_ptr is NULL");
        }
      }
      break;
    // CR 352866 end

    default:     
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;
  }  
} /* tdsrrcrce_process_rce_wait_for_sib7_current */


/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_ue_camp_on_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in RCE_WAIT_FOR_UE_CAMP_ON  substate of 
  Disconnected State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_ue_camp_on_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  rrc_abort_cause_e_type  cause = RRC_MAX_ACCESS_FAILURE;

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_NEW_CELL_IND:  /* This command actually represents UE Camp on 
                               Indication */ 
  
      /* Stop the timer to UE to camp on */  
      tdsrrctmr_stop_timer(TDSRRCTMR_UE_TO_CAMP_ON_TIMER);

      if(TDSRRCRCE_SUCCESS != tdsrrcrce_validate_cell_access_status(
                              tdsrrc_est_cn_domain_id,
                              tdsrrc_establishment_cause,
                              &cause)
        )
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH," Cause value filled in RRC_ABORT_IND cmd= %d,rrcrce sub state=%d",cause,tdsrrcrce_substate);
        if(TDSRRCRCE_SUCCESS != 
            tdsrrcrce_send_rrc_abort_ind(RRC_ACCESS_CLASS_NOT_ALLOWED) )
        {
          ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
        }
        tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */
        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Bring rce state to TDSRRCRCE_INITIAL since AC status is not suffice to go ahead with call setup ");
        return;
      }
      if(tdsrrcwrm_acquire_lock_for_est_req(tdsrrc_est_cn_domain_id,0xfffffff,TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_EST_REQ_IN_MS) == FAILURE)
      {
        /* Send RRC_ABORT_IND command to the MM */
        if(TDSRRCRCE_SUCCESS != 
           tdsrrcrce_send_rrc_abort_ind(RRC_NO_RESOURCE_AVAILABLE) )      
        {
         
          ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
        } 
        tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */
        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        return;
      }
      /* UE is camped on and SIB7 is current, then Setup RB0 */
      if( TDSRRCRCE_SUCCESS == 
          tdsrrcrce_send_rrc_channel_config_req_for_rb0(TDSESTABLISH_RB,
                                                       TDSRRC_STATE_CONNECTING,
                                                       TRUE ))  
      {    
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF; 
      }    
      else  /* Failed to send a internal command to RRC LLC*/
      {   
        ERR_FATAL("Unable to send internal cmd to LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);  
      }
      break;

    case TDSRRC_BPLMN_SUSPEND_CNF:

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSTOTDS: RCE rcvd BPLMN_SUSPEND_CNF from CSP with status %d",cmd_ptr->cmd.rrc_csp_bplmn_suspend_cnf.status);

      /* Stop the timer to UE to camp on */  
      tdsrrctmr_stop_timer(TDSRRCTMR_UE_TO_CAMP_ON_TIMER);

      if(cmd_ptr->cmd.rrc_csp_bplmn_suspend_cnf.status == SUCCESS)
      {
        if(tdsrrcwrm_acquire_lock_for_est_req(tdsrrc_est_cn_domain_id,0xfffffff,TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_EST_REQ_IN_MS) == FAILURE)
        {
          /* Send RRC_ABORT_IND command to the MM */
          if(TDSRRCRCE_SUCCESS != 
             tdsrrcrce_send_rrc_abort_ind(RRC_NO_RESOURCE_AVAILABLE) )      
          {
           
            ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
          } 
          tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          return;
        }
        /* UE is camped on, then Check whether SIB7 is current */
        if(TRUE == tdsrrcsib_check_and_update_sib7_validity (TDSRRC_PROCEDURE_RCE) )
        { 
          /* UE is camped on and SIB7 is current, then Setup RB0 */
          if( TDSRRCRCE_SUCCESS == 
              tdsrrcrce_send_rrc_channel_config_req_for_rb0(TDSESTABLISH_RB,
                                                         TDSRRC_STATE_CONNECTING,
                                                         TRUE ))  
          {              
            tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF;     
          }    
          else  /* Failed to send a internal command to RRC LLC*/
          {     
            ERR_FATAL("Unable to send internal cmd to LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);         
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE waits for SIB7 to become current"); 
          tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_SIB7_CURRENT;
        }    
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR CSP sent SUSPEND_CNF with status FAILURE");

        /* Send RRC_ABORT_IND command to the MM */
        if(TDSRRCRCE_SUCCESS == 
           tdsrrcrce_send_rrc_abort_ind(RRC_FAIL_TO_CAMP_ON))      
        {
          /* The RCE Procedure ends */
          tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        }    
        else
        {     
          ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_IND,0,0);
        }     
      }
      break;

    case TDSRRC_TIMER_TO_CAMP_ON_EXPIRED_IND:      

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_TIMER_TO_CAMP_ON_EXPIRED_IND in rce substate %d",tdsrrcrce_substate);

#ifdef FEATURE_INTERRAT_PCCO_G2TD
      /* If GTOTDS is in progress,send a conn_setup_ind with failure to CHO.
         CHO would deactivate TDS and pass control back to GSM.Do not send
         RRC_ABORT_IND to MM if GTOTDS is active */
      if(tdsrrccho_is_g2td_pcco_in_progress())
      { 
        tdsrrc_conn_setup_ind_type conn_setup_ind_params;
        conn_setup_ind_params.status = FAILURE;
        conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_TIMER_TO_CAMP_ON_EXPIRED_IND when PCCO active in rce substate %d",tdsrrcrce_substate);
        tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);

        /* The RCE Procedure ends */
        tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */
        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        break;
      }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */
      
      /* Send RRC_ABORT_IND command to the MM */
      if(TDSRRCRCE_SUCCESS == 
         tdsrrcrce_send_rrc_abort_ind(RRC_FAIL_TO_CAMP_ON) )      
      {
        /* The RCE Procedure ends */
        tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */
        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
      }    
      else
      {     
        ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_IND,0,0);
      }     
      break;

    case TDSRRC_ABORT_REQ:

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ rcvd in %d substate",tdsrrcrce_substate);

      /* Stop the timer for UE to camp on */  
      tdsrrctmr_stop_timer(TDSRRCTMR_UE_TO_CAMP_ON_TIMER);

     /* Send RRC_ABORT_CNF command to the MM */
      if(TDSRRCRCE_SUCCESS == 
         tdsrrcrce_send_rrc_abort_cnf() )      
      {        
        /* At this time, none of the RBs are setup. Simply reset the RCE
         substate */
         tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */
         tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
      }    
      else
      {     
        ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
      }    
      break;

    default:     
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;
  }  
} /* tdsrrcrce_process_rce_wait_for_ue_camp_on_substate */

#ifdef FEATURE_TDSCDMA_TO_LTE
/*===========================================================================

FUNCTION rrcrce_is_band_supported_for_eutra_neighbors

DESCRIPTION
  This function checks if the EUTRA bands corresponding to the EARFCN list provided
  in SIB19 is supported by UE.

DEPENDENCIES
  Calls LTE-RRC API lte_rrc_earfcn_is_supported().

RETURN VALUE
  FALSE: If none of the bands corresponding to EARFCN list in SIB19 is supported by UE.
  TRUE : Else.

SIDE EFFECTS
  None
===========================================================================*/
static boolean tdsrrcrce_is_band_supported_for_eutra_neighbors
(
  void
)
{
  tdsrrc_SysInfoType19 *sib19_ptr = NULL;

  /* If SIB 19 is not present or if SIB 19 is present but freq list is not provided, send 
      "Support of E-UTRA FDD" set to TRUE if UE supports LTE mode */
  boolean band_supported = TRUE;
  uint32 idx = 0;

#ifdef FEATURE_TDSCDMA_SIB19
  sib19_ptr = (tdsrrc_SysInfoType19 *)tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB19);
#endif
  if(sib19_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2L: SIB19 is NULL.");
  }
  else
  {
    if(sib19_ptr->m.eutra_FrequencyAndPriorityInfoListPresent)
    {
      tdsrrc_EUTRA_FrequencyAndPriorityInfoList *eutra_freq_pri_list_ptr = &(sib19_ptr->eutra_FrequencyAndPriorityInfoList);

      while((eutra_freq_pri_list_ptr->n > idx))
      {
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
        if(lte_rrc_earfcn_is_supported(tdsrrc_get_as_id(),(lte_earfcn_t)eutra_freq_pri_list_ptr->elem[idx].earfcn, tdsrrccsp_get_lte_band_pref()))
#else
        if(lte_rrc_earfcn_is_supported((lte_earfcn_t)eutra_freq_pri_list_ptr->elem[idx].earfcn, tdsrrccsp_get_lte_band_pref()))
#endif
        {
          break;
        }
        idx++;
      }
      /* If freq list is given in SIB 19, but none of the bands corresponding to the EUTRA freq are supported by UE
         (eutra_freq_pri_list_ptr traverses to the end), do not send pre-redirection info in RRC connection 
         request message to network */
      if(eutra_freq_pri_list_ptr->n == idx)
      {
        band_supported = FALSE;
      }
    }
  }
  return band_supported;
}
#endif

/*===========================================================================

FUNCTION tdsrrcrce_build_rrc_connection_request_msg

DESCRIPTION
  This function builds the RRC CONNECTION REQUEST message with all required
  parameters.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_build_rrc_connection_request_msg
(
  tdsrrc_UL_CCCH_Message *msg_ptr               /* Pointer to uplink CCCH message*/
)
{

#ifdef FEATURE_3GPP_CSFB_TDS
  tdsrrc_RRCConnectionRequest_v860NonCriticalExtensions *rce_v860_ext = NULL;

  tdsrrc_RRCConnectionRequest_v940NonCriticalExtensions *rce_v940_ext = NULL;
#endif

#ifdef FEATURE_TDSCDMA_TO_LTE
#ifdef LTE_BAND_NUM
  sys_lte_band_mask_e_type bitmask_zero; 
  sys_lte_band_mask_e_type supported_lte_band;
  supported_lte_band = tdsrrccsp_get_lte_band_pref();
  memset(&bitmask_zero, 0x00, sizeof(sys_lte_band_mask_e_type));
#endif
#endif

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"tdsrrc_nv_rel_indicator: %x",tdsrrc_nv_rel_indicator);

  /* Select the message as RRC CONNECTION REQUEST message */
  msg_ptr->message.t = T_tdsrrc_UL_CCCH_MessageType_rrcConnectionRequest;


  /* Set the connection establishment cause properly if there
   * was a GTOTDS reselection or GTOTDS PCCO
   */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Est Cause %d",tdsrrc_establishment_cause);
  tdsrrcmcm_set_establishment_cause();
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Est Cause after MCM update %d",tdsrrc_establishment_cause);

  /* To update the proc id as RCE so that first IDT can be sent without cause */
  tdsrrcidt_update_proc_id();

  /* Initializes the RRC Connection Establishment cause */
  msg_ptr->message.u.rrcConnectionRequest.establishmentCause = 
    tdsrrcrce_update_establishment_cause();

  /* Initializes the initial UE Id parameter */
  tdsrrcrce_update_initial_ue_identity(
    &msg_ptr->message.u.rrcConnectionRequest.initialUE_Identity);

  /* Initializes the Protocol Error Indicator */
  msg_ptr->message.u.rrcConnectionRequest.protocolErrorIndicator = 
    tdsrrc_protocol_error_indicator;

  /* Initialize all optional fields to ommitted */
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionRequest);

 /* Function call to Measurement Control & Report procedure. */
  if(TDSRRCMEAS_RACH_SUCCESSFUL == tdsrrcmeas_append_meas_rpt_on_rach(
     &msg_ptr->message.u.rrcConnectionRequest.measuredResultsOnRACH, TDSRRC_PROCEDURE_RCE, FALSE))
  {
  /* if Measurement Control & Reportprocedure returns RRC_MEAS_RACH_SUCCESFUL,
     then enable the  bit_mask field */
   TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.rrcConnectionRequest,
        tdsrrc_RRCConnectionRequest,measuredResultsOnRACH);
   
  }
  else
  {
  /*"Measured Results on RACH" IE is optional, if Measurement Control & Report
     procedure returns other than RRC_MEAS_RACH_SUCCESFUL, then disable the
     bit_mask field */
    TDSRRC_MSG0(MSG_LEGACY_MED,"Measured Results on RACH is not appended");
  } 

  TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.rrcConnectionRequest,
    v3d0NonCriticalExtensions);
  
  msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
    rRCConnectionRequest_v3d0ext.m.uESpecificBehaviourInformation1idlePresent = 1;
  
  /* Indicate that UE support N Frequency */
  msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
    rRCConnectionRequest_v3d0ext.uESpecificBehaviourInformation1idle.numbits= 4;
  msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
    rRCConnectionRequest_v3d0ext.uESpecificBehaviourInformation1idle.data[0]= 0;
  
  msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.m.v4b0NonCriticalExtensionsPresent =1;
  
  /* Indicate that we are a Release 4 UE */
  msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.
    rrcConnectionRequest_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_4;

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting REL 5 IN CON.REQ");

    /* Indicate that we are a Release 5 UE */
    msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.
      rrcConnectionRequest_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_5;

    /* Set vb0 non-critical extension present */
    msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
      v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent =1;

    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
      v4b0NonCriticalExtensions.v590NonCriticalExtensions);

    /* Indicate that we do not support pre-determined config */
    msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
      rrcConnectionRequest_v590ext.predefinedConfigStatusInfo = FALSE;
  }

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL: Setting REL 6 IN CON.REQ");
    /* Indicate that we are a Release 6 UE */
    msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.
      rrcConnectionRequest_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_6;
    
    msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
      v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.v690NonCriticalExtensionsPresent =1;

       TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions);

#if  defined (FEATURE_TDSCDMA_REL9)
      msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.m.v6b0NonCriticalExtensionsPresent = 1;
          
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.
              rrcConnectionRequest_v6b0ext);
          
          
      msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.m.v6e0NonCriticalExtensionsPresent =1;
          
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions);
          
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
        rrcConnectionRequest_v6e0ext);
   
#else

    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
      v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions);

#endif

    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
      v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
      rrcConnectionRequest_v690ext);

    msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
      v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
      rrcConnectionRequest_v690ext.m.ueCapabilityIndicationPresent =1;

#ifdef FEATURE_TDSCDMA_HSUPA
    if (TDSCHECK_WCDMA_OPTIONAL_FEATURE_HSUPA_SUPPORTED)
    {
      /* Indicate that UE supports for hsdch and edch*/
      msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
        rrcConnectionRequest_v690ext.ueCapabilityIndication = tdsrrc_RRCConnectionRequest_v690ext_IEs_ueCapabilityIndication_hsdch_edch;
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCEUL: optional feature EUL/HSUPA(0x%x->0x%x) disabled through NV item",tds_nv_optional_feature_list,TDSCHECK_WCDMA_OPTIONAL_FEATURE_HSUPA_SUPPORTED);

      /* Indicate that UE supports for hsdch only */
      msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
        rrcConnectionRequest_v690ext.ueCapabilityIndication = tdsrrc_RRCConnectionRequest_v690ext_IEs_ueCapabilityIndication_hsdch;
    }
#else
    /* Indicate that UE supports for hsdch only */
    msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
      v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
      rrcConnectionRequest_v690ext.ueCapabilityIndication = tdsrrc_RRCConnectionRequest_v690ext_IEs_ueCapabilityIndication_hsdch;

#endif

#if defined(FEATURE_TDSCDMA_REL9)

    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
    {
     TDSRRC_RESET_MSG_IE_PRESENT(
        msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions
        );

      msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
        m.v770NonCriticalExtensionsPresent =1;
  
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
        v770NonCriticalExtensions);

      TDSRRC_RESET_MSG_IE_PRESENT(
      msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
        v770NonCriticalExtensions.rrcConnectionRequest_v770ext);
  
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  
      msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
        v770NonCriticalExtensions.rrcConnectionRequest_v770ext.m.mac_ehsSupportPresent =1;
      msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
        v770NonCriticalExtensions.rrcConnectionRequest_v770ext.mac_ehsSupport = tdsrrc_RRCConnectionRequest_v770ext_IEs_mac_ehsSupport_true_;
            
   
#endif/*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
      if(tdsrrcwrm_get_tuneaway_status() == FALSE)
      {
#ifdef FEATURE_TDSCDMA_CPC_DTX
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Indicate UE support CPC-DTX operation on UL DPCCH");

      TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
        v770NonCriticalExtensions.rrcConnectionRequest_v770ext,
        tdsrrc_RRCConnectionRequest_v770ext_IEs,discontinuousDpcchTransmission);
      
      msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
        v770NonCriticalExtensions.rrcConnectionRequest_v770ext.discontinuousDpcchTransmission = tdsrrc_RRCConnectionRequest_v770ext_IEs_discontinuousDpcchTransmission_true_;
#endif/*FEATURE_TDSCDMA_CPC_DTX*/
      }
      if(tdsl1_get_mobility_info())
      {
#ifdef FEATURE_TDSCDMA_REL7_SPECRS      
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Detected");
       TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
          v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.rrcConnectionRequest_v770ext,
          tdsrrc_RRCConnectionRequest_v770ext_IEs,ueMobilityStateIndicator);  
        msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
          v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
          v770NonCriticalExtensions.rrcConnectionRequest_v770ext.ueMobilityStateIndicator = tdsrrc_High_MobilityDetected_high_MobilityDetected;
#else /*FEATURE_TDSCDMA_REL7_SPECRS*/
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"High Mobility Ind from L1 not Supported.");
#endif /*FEATURE_TDSCDMA_REL7_SPECRS*/
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility not Detected");
      }

    }
#endif/*FEATURE_TDSCDMA_REL9*/

    /* Include CN domain indicator in RCE message .
       *If CN domain is PS then no need to include Call type.
       *If CN domain is CS then need to include Call type according to the call being initiated
    */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"tdsrrc_est_cn_domain_id = %d,tdsrrc_establishment_cause =%d,tdsrrc_est_call_type = %d",
                                                                                                  tdsrrc_est_cn_domain_id,
                                                                                                  tdsrrc_establishment_cause,
                                                                                                  tdsrrc_est_call_type);
    switch (tdsrrc_est_cn_domain_id) 
    {
      case RRC_CS_DOMAIN_CN_ID:
        msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
          v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
          rrcConnectionRequest_v690ext.domainIndicator.t = T_tdsrrc_RRCConnectionRequest_v690ext_IEs_domainIndicator_cs_domain;

        switch(tdsrrc_est_call_type)
        {
          case RRC_CALL_TYPE_SPEECH:
            msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
              v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
              rrcConnectionRequest_v690ext.domainIndicator.u.cs_domain.csCallType = tdsrrc_RRCConnectionRequest_v690ext_IEs_csCallType_speech; 
            break;
    
          case RRC_CALL_TYPE_VIDEO:
            msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
              v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
              rrcConnectionRequest_v690ext.domainIndicator.u.cs_domain.csCallType = tdsrrc_RRCConnectionRequest_v690ext_IEs_csCallType_video; 
            break;
    
          case RRC_CALL_TYPE_OTHERS:
            msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
              v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
              rrcConnectionRequest_v690ext.domainIndicator.u.cs_domain.csCallType = tdsrrc_RRCConnectionRequest_v690ext_IEs_csCallType_other;
            break;
    
          default:
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Wrong est call type set by NAS = %d",tdsrrc_est_call_type);
            msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
               v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
               rrcConnectionRequest_v690ext.domainIndicator.u.cs_domain.csCallType = tdsrrc_RRCConnectionRequest_v690ext_IEs_csCallType_other; 
            break;
        }

        break;

      case RRC_PS_DOMAIN_CN_ID:
        msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
          v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
          rrcConnectionRequest_v690ext.domainIndicator.t = T_tdsrrc_RRCConnectionRequest_v690ext_IEs_domainIndicator_ps_domain;
        break;

      default :
        msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
          v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
          rrcConnectionRequest_v690ext.domainIndicator.t = T_tdsrrc_RRCConnectionRequest_v690ext_IEs_domainIndicator_ps_domain;
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CN domain I.D=%d",tdsrrc_est_cn_domain_id);
        break;
    }
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
    if(SUCCESS == tdsrrcifreq_append_meas_rpt_on_rach(&(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
        v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.rrcConnectionRequest_v690ext.
        measuredResultsOnRACHinterFreq),TDSRRC_PROCEDURE_RCE))
    {
      TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
          rrcConnectionRequest_v690ext,tdsrrc_RRCConnectionRequest_v690ext_IEs,measuredResultsOnRACHinterFreq);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Enabled Inter Frequency measurement results on RACH");
    }
#endif

  }
  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL: Setting REL 7 IN CON.REQ");
    /* Indicate that we are a Release 6 UE */
    msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.
      rrcConnectionRequest_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_7;
  }

#ifdef FEATURE_TDSCDMA_REL8
  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Setting AS Rel Indicator in RRC Conn Request.");
    /* Indicate that we are a Release 8 UE */
    msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.
      rrcConnectionRequest_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_8;

    /*As part of R7 feature, extension v770NonCriticalExtensions ha been added already. So we can assume that it's present here in R8.*/
    /*Here we add extension v7b0NonCriticalExtensions. Later we may need to add it earlier at R7 when E-FDPCH needs to be enabled.*/
    msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.m.
        v7b0NonCriticalExtensionsPresent =1;

   TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.v7b0NonCriticalExtensions);

    /*Here we add extension v7b0NonCriticalExtensions*/
    msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.v7b0NonCriticalExtensions.m.
        v860NonCriticalExtensionsPresent =1;

    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.v7b0NonCriticalExtensions.
        v860NonCriticalExtensions);

    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.v7b0NonCriticalExtensions.
        v860NonCriticalExtensions.rrcConnectionRequest_v860ext);

#ifdef FEATURE_TDS_DC_HSDPA
    if((TDSCHECK_WCDMA_OPTIONAL_FEATURE_DC_HSDPA_SUPPORTED)
#ifdef FEATURE_TDSCDMA_BAND_SPEC_DC_HSDPA     
       &&  (TRUE == tdsrrcueci_check_rfm_band_support_for_dual_carrier())
#endif
      )
    {
      msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
          v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.v7b0NonCriticalExtensions.
          v860NonCriticalExtensions.rrcConnectionRequest_v860ext.m.multiCellSupportPresent =1;

      msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
          v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.v7b0NonCriticalExtensions.
          v860NonCriticalExtensions.rrcConnectionRequest_v860ext.multiCellSupport = tdsrrc_RRCConnectionRequest_v860ext_IEs_multiCellSupport_true_;
    }
#endif /*FEATURE_TDS_DC_HSDPA*/

#ifdef FEATURE_TDSCDMA_TO_LTE
    if(tdsrrcmcm_is_lte_mode_enabled() &&
       tdsrrcrce_is_band_supported_for_eutra_neighbors() &&
       !tdsrrccsp_is_preredirection_info_to_be_excluded())
    {
      msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
          v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.v7b0NonCriticalExtensions.
          v860NonCriticalExtensions.rrcConnectionRequest_v860ext.m.pre_redirectionInfoPresent = 1;

      if(lte_rrc_lte_fdd_is_supported(tdsrrccsp_get_lte_band_pref()))
      {
        msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
            v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.v7b0NonCriticalExtensions.
            v860NonCriticalExtensions.rrcConnectionRequest_v860ext.pre_redirectionInfo.supportEUTRA_FDD = TRUE;
      }
      else
      {
        msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
            v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.v7b0NonCriticalExtensions.
            v860NonCriticalExtensions.rrcConnectionRequest_v860ext.pre_redirectionInfo.supportEUTRA_FDD = FALSE;
      }

      if(lte_rrc_lte_tdd_is_supported(tdsrrccsp_get_lte_band_pref()))
      {
        msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
            v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.v7b0NonCriticalExtensions.
            v860NonCriticalExtensions.rrcConnectionRequest_v860ext.pre_redirectionInfo.supportEUTRA_TDD = TRUE;
      }
      else
      {
        msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
            v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.v7b0NonCriticalExtensions.
            v860NonCriticalExtensions.rrcConnectionRequest_v860ext.pre_redirectionInfo.supportEUTRA_TDD = FALSE;
      }

    }
#endif
  }
#endif /* FEATURE_TDSCDMA_REL8 */

#ifdef FEATURE_TDSCDMA_REL9
  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:Setting AS Rel Indicator in RRC Conn Request.");
    /* Indicate that we are a Release 9 UE */
    msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.v4b0NonCriticalExtensions.
      rrcConnectionRequest_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_9;
  }
#endif /* FEATURE_TDSCDMA_REL9 */

#ifdef FEATURE_TDSCDMA_TO_LTE
#ifdef LTE_BAND_NUM
  if(tdsrrcmcm_is_lte_mode_enabled() &&
     !SYS_LTE_BAND_MASK_CHK_IF_EQUAL(&supported_lte_band, &bitmask_zero))
#else
  if(tdsrrcmcm_is_lte_mode_enabled() &&
      (tdsrrccsp_get_lte_band_pref() != 0))
#endif
  {
    tdsrrcueci_send_eutra_capabilitites_req_to_lte();
  }
#endif /* FEATURE_TDSCDMA_REL9 */

#ifdef FEATURE_3GPP_CSFB_TDS
  /* Setting the CSFB Indicator IE(spec CR5026) and the support for system information container IE(spec CR4321) */
  if(tdsrrc_csp_int_data.include_csfb_ind && (!TDSCHECK_OPTIONAL_FEATURE_IGNORE_CSFB_INDICATION))
  {
    rce_v860_ext = &msg_ptr->message.u.rrcConnectionRequest.v3d0NonCriticalExtensions.
      v4b0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.
      v770NonCriticalExtensions.v7b0NonCriticalExtensions.v860NonCriticalExtensions;

    rce_v860_ext->m.v7e0NonCriticalExtensionsPresent =1 ;

    rce_v860_ext->v7e0NonCriticalExtensions.m.v7g0NonCriticalExtensionsPresent =1 ;

    rce_v860_ext->v7e0NonCriticalExtensions.v7g0NonCriticalExtensions.m.v920NonCriticalExtensionsPresent =1 ;

    rce_v860_ext->v7e0NonCriticalExtensions.v7g0NonCriticalExtensions.
      v920NonCriticalExtensions.m.v940NonCriticalExtensionsPresent =1 ;

    rce_v940_ext = &rce_v860_ext->v7e0NonCriticalExtensions.v7g0NonCriticalExtensions.
      v920NonCriticalExtensions.v940NonCriticalExtensions;

    /* This gives the info if stored SI container is available with RRC or not */
    if(tdsrrc_SIB_container.num_cont > 0)
    {
      rce_v940_ext->rrcConnectionRequest_v940ext.m.systemInformationContainerStoredIndicatorPresent =1 ;

      rce_v940_ext->rrcConnectionRequest_v940ext.systemInformationContainerStoredIndicator = tdsrrc_RRCConnectionRequest_v940ext_IEs_systemInformationContainerStoredIndicator_true;
    }

    rce_v940_ext->m.va40NonCriticalExtensionsPresent =1 ;

    rce_v940_ext->va40NonCriticalExtensions.m.v9b0NonCriticalExtensionsPresent =1 ;

    rce_v940_ext->va40NonCriticalExtensions.v9b0NonCriticalExtensions.rrcConnectionRequest_v9b0ext.m.cSFBIndicationPresent =1;

    rce_v940_ext->va40NonCriticalExtensions.v9b0NonCriticalExtensions.
      rrcConnectionRequest_v9b0ext.cSFBIndication = tdsrrc_RRCConnectionRequest_v9b0ext_IEs_cSFBIndication_true;

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"CSFB: Setting CSFB Indication %d [0 : SET]",rce_v940_ext->va40NonCriticalExtensions.v9b0NonCriticalExtensions.
      rrcConnectionRequest_v9b0ext.cSFBIndication,0,0);
  }
#endif

} /* tdsrrcrce_build_rrc_connection_request_msg  */

/*===========================================================================

FUNCTION tdsrrcrce_get_t300_from_serv_cell_db

DESCRIPTION
  This function reads the serving cell DB and returns the T300 value from the
  UE Timers and Constants in idle mode field

DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type : Returns TDSRRCRCE_SUCCESS If it successful retrieves
  T300 from the SIB database. Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_get_t300_from_serv_cell_db
(
  tdsrrc_T_300 *rrcrce_t_300                    /* Pointer to T300 */ 
)
{
  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MAJOR; 
                                            /* Status is initialized to
                                               Failure */
  tdsrrc_SysInfoType1 *rrcrce_sib1;            /* Pointer to SIB 1 */

  /* Read SIB 1 from the SIB database */
  rrcrce_sib1 = (tdsrrc_SysInfoType1 *)
    tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB1);

  /* Check whether SIB 1 is a valid one */
  if (rrcrce_sib1 != NULL)
  {
    *rrcrce_t_300= rrcrce_sib1 ->ue_IdleTimersAndConstants.t_300;
    status = TDSRRCRCE_SUCCESS;
  }
  else
  {
    /* The UE should have camped and received a valid SIB1 before UE initiates
       RRC Connection Establishment procedure. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB Database does not have a valid SIB 1");
  }
  return (status);
} /* tdsrrcrce_get_t300_from_serv_cell_db */

/*===========================================================================

FUNCTION tdsrrcrce_get_n300_from_serv_cell_db

DESCRIPTION
  This function reads the serving cell DB and returns the N300 value from the
  UE Timers and Constants in idle mode field

DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type : Returns TDSRRCRCE_SUCCESS If it successful retrieves
  N300 from the SIB database. Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_get_n300_from_serv_cell_db
(
  tdsrrc_N_300 *rrcrce_n_300                     /* Pointer to N300 */  
)
{
  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MAJOR; 
                                            /* Status is initialized to
                                               Failure */
  tdsrrc_SysInfoType1 *rrcrce_sib1;            /* Pointer to SIB 1 */

  {
    /* Read SIB 1 from the SIB database */
    rrcrce_sib1 = (tdsrrc_SysInfoType1 *)
      tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB1);
  
    /* Check whether SIB 1 is a valid one */
    if (rrcrce_sib1 != NULL)
    {
      if((tdsrrc_check_L1_accumulated_rscp() > -850)
         ||(TRUE == tdsrrcmcm_is_dualmode_enabled()))
      {
        *rrcrce_n_300= rrcrce_sib1 ->ue_IdleTimersAndConstants.n_300;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"N300 from SIB is: %d",*rrcrce_n_300);
      }
      else
      {
        *rrcrce_n_300= (rrcrce_sib1 ->ue_IdleTimersAndConstants.n_300>3)? 7:rrcrce_sib1 ->ue_IdleTimersAndConstants.n_300*2;
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"As RSCP <= -85dbm, for T only or T+L mode,N300 is changed from%d  to: %d",rrcrce_sib1 ->ue_IdleTimersAndConstants.n_300,*rrcrce_n_300);  
      }
      status = TDSRRCRCE_SUCCESS;
    }
    else
    {
      /* The UE should have camped and received a valid SIB1 before UE initiates
         RRC Connection Establishment procedure. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB Database does not have a valid SIB 1");
    }
  }
  return (status);
} /* tdsrrcrce_get_n300_from_serv_cell_db */

/*===========================================================================

FUNCTION tdsrrcrce_convert_t300_to_ms

DESCRIPTION
  This function converts the tdsrrc_T_300 enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsrrcrce_convert_t300_to_ms
(
  tdsrrc_T_300 rrcrce_t_300                   /* enum for Timer T300 */
)
{
  uint32 timer_value;                       /* Timer Value in milli seconds */

  switch (rrcrce_t_300)
  {
    case tdsrrc_T_300_ms100:
      timer_value = 100;
      break;
    /* 200 milli seconds interval*/
    case tdsrrc_T_300_ms200:
      timer_value = 200;
      break;      
    case tdsrrc_T_300_ms400:
      timer_value = 400;
      break;
    case tdsrrc_T_300_ms600:
      timer_value = 600;
      break;
    case tdsrrc_T_300_ms800:
      timer_value = 800;
      break;
    case tdsrrc_T_300_ms1000:
      timer_value = 1000;
      break;
    case tdsrrc_T_300_ms1200:
      timer_value = 1200;
      break;
    case tdsrrc_T_300_ms1400:
      timer_value = 1400;
      break;
    case tdsrrc_T_300_ms1600:
      timer_value = 1600;
      break;
    case tdsrrc_T_300_ms1800:
      timer_value = 1800;
      break;
    case tdsrrc_T_300_ms2000:
      timer_value = 2000;
      break;
    /*  1000 milli seconds interval*/
    case tdsrrc_T_300_ms3000:
      timer_value = 3000;
      break;
    case tdsrrc_T_300_ms4000:
      timer_value = 4000;
      break;
    case tdsrrc_T_300_ms6000:
      timer_value = 6000;
      break;
    case tdsrrc_T_300_ms8000:
      timer_value = 8000;
      break;
    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown Timer value rcvd from SIB1");
      timer_value = 100;  /* Set lowest timer value as a default */
      break;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"T300 is %d ms",timer_value); 
  return (timer_value);
} /* tdsrrcrce_convert_t300_to_ms*/

   
/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_rb0_phychan_resetup_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in TDSRRCRCE_WAIT_FOR_RB0_PHYCHAN_RESETUP_CNF  substate of 
  Connecting State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_rb0_phychan_resetup_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  rrc_abort_cause_e_type  cause = RRC_MAX_ACCESS_FAILURE;

  switch(cmd_ptr->cmd_hdr.cmd_id) 
  {
    case TDSRRC_ABORT_REQ:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ rcvd in %d substate",tdsrrcrce_substate);
      tdsrrcrce_send_abort_cnf = TRUE;
      break;

    case TDSRRC_STATE_CHANGE_IND:
      /* Stop the timer T300 */
      tdsrrctmr_stop_timer(TDSRRCTMR_T_300_TIMER);

      if (cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED)
      {
        if (tdsrrcrce_send_abort_cnf == TRUE)
        {
          /* Send RRC_ABORT_CNF command to the MM */
          if(TDSRRCRCE_SUCCESS == 
            tdsrrcrce_send_rrc_abort_cnf() )      
          {        
          /* At this time, none of the RBs are setup. Simply reset the RCE
            substate */
            tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */
            tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;

            break;
          }    
          else
          {     
            ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
          }  
        }
        else
        {
          if(TDSRRCRCE_SUCCESS == 
            tdsrrcrce_send_rrc_abort_ind(RRC_FAILURE_UNSPECIFIED) )      
          {
            /* The RCE Procedure ends */
            tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */
            tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          }    
          else
          {     
            ERR_FATAL("Unable to post MM Command: %d",RRC_ABORT_IND,0,0);
          }
        }
      }

      break;

    case TDSRRC_CHANNEL_CONFIG_CNF:
      /* The RRC LLC should send first Confirmation before it informs the 
         State Change Manager to go to CONNECTING STATE */
      if(cmd_ptr->cmd.chan_config_cnf.procedure == TDSRRC_PROCEDURE_RCE)
      {

        if(cmd_ptr->cmd.chan_config_cnf.transaction_id == tdsrrcrce_chl_config_transaction_id)
        {
          if( TDSRRCLLC_CHAN_CFG_SUCCESS == cmd_ptr->cmd.chan_config_cnf.chan_cfg_status ) 
          {
            if(tdsrrcrce_send_abort_cnf)
            {
              /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
              if( TDSRRCRCE_SUCCESS == 
                  tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                  TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
              {    
                /* Wait for successful release of RB0 */
                tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;
              }    
              else    /* Failed to send a internal command to RRC LLC.*/
              {      
                ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
              }
            }
            else
            {
              if(TDSRRCRCE_SUCCESS == tdsrrcrce_validate_cell_access_status(
                                    tdsrrc_est_cn_domain_id,
                                    tdsrrc_establishment_cause,
                                    &cause)
                )
              {
                /* Sends the RRC Connection Request message to Send Chain  and This
                   function also starts the timer T300 */
                tdsrrcrce_send_rrc_connection_request_msg();
                /* The RCE procedure waits for the RRC Connection Setup Message */
                tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP;

                /* Increments the re-transmission counter for RRC Connection
                   Request Message */
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);

                tdsrrc_v_300++;  
              }
              else
              {
                /* Release the RB 0 and request the RRC LLC to go to Idle disconnected */
                if( TDSRRCRCE_SUCCESS == 
                   tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                  TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                {
                  /* The RCE Procedure ends */
                  tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; /* change the RCE substate */
                  if(cause  ==  RRC_ACCESS_CLASS_NOT_ALLOWED)
                  {
                    if(tdsrrc_est_cn_domain_id == RRC_CS_DOMAIN_CN_ID)
                    {
                      tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_CS_DOMAIN;
                    }
                    else
                    {
                      tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_PS_DOMAIN;
                    }
                  }
                  else
                  {
                    tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
                  }
                }
                else    /* Failed to send a internal command to RRC LLC.*/
                {
                  ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                }
              }
            }
          }
          else
          {
            event_report(EVENT_TDSCDMA_RRC_RB0_SETUP_FAILURE);
			TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
				                       TDS_SELF_HOSTING_MAJOR_CALL_SETUP_EXCEPTION,
				                       TDS_SELF_HOSTING_MINOR_RRC_RB0_SETUP_FAILURE);

            if(tdsrrcrce_send_abort_cnf)
            {
              if(TDSRRCRCE_SUCCESS == tdsrrcrce_send_rrc_abort_cnf() )
              { 
                /* Clear the ESTABLISHED_RABS while bringing down RRC Connection*/
                tdsrrc_clear_established_rabs();

                /* Reset Signalling Connections status */
                tdsrrcscr_reset_signalling_connection_status();

                tdsrrcrce_substate = TDSRRCRCE_INITIAL;
                tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;

              }
              else
              {
                ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
              }
            }
            else
            {
              /* Send RRC_ABORT_IND command to the MM */
              if(TDSRRCRCE_SUCCESS == 
                 tdsrrcrce_send_rrc_abort_ind(RRC_INTERNAL_ERROR) )      
              {
                /* The RCE Procedure ends */
                tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */
                tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
              }    
              else
              {     
                ERR_FATAL("Unable to post MM Command: %d",RRC_ABORT_IND,0,0);
              }    
            }
            /* Trigger Cell Selection */
            if(TDSRRCRCE_SUCCESS ==
               tdsrrcrce_send_rrc_initiate_cell_selection_ind(
               TDSRRC_CELL_SEL_RB0_SETUP_FAILURE, TDSRRC_STATE_DISCONNECTED) )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB0 setup failure");
            }
            else
            {
              ERR_FATAL("Failed to post int cmd: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
            }
          }
        }
        else
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring tr_id %d in chl config cnf. Expected tr_id = %d",cmd_ptr->cmd.chan_config_cnf.transaction_id,tdsrrcrce_chl_config_transaction_id);
        }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_CHANNEL_CONFIG_CNF's proc :%d",cmd_ptr->cmd.chan_config_cnf.procedure);
      }

      break;

    default:     
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;
   }  

} /* tdsrrcrce_process_rce_wait_for_rb0_phychan_resetup_cnf_substate  */


/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_rb0_setup_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF  substate of 
  Disconnected State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_rb0_setup_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  switch(cmd_ptr->cmd_hdr.cmd_id) 
  {
    case TDSRRC_ABORT_REQ:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ rcvd in %d substate",tdsrrcrce_substate);
      tdsrrcrce_send_abort_cnf = TRUE;
      break;

    case TDSRRC_CHANNEL_CONFIG_CNF:
      /* The RRC LLC should send first Confirmation before it informs the 
         State Change Manager to go to CONNECTING STATE */
      if(cmd_ptr->cmd.chan_config_cnf.procedure == TDSRRC_PROCEDURE_RCE)
      {
        if(cmd_ptr->cmd.chan_config_cnf.transaction_id == tdsrrcrce_chl_config_transaction_id)
        {
          if( TDSRRCLLC_CHAN_CFG_SUCCESS == cmd_ptr->cmd.chan_config_cnf.chan_cfg_status ) 
          {
            if(tdsrrcrce_send_abort_cnf)
            {
              /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
              if( TDSRRCRCE_SUCCESS == 
                  tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                  TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
              {    
                /* Wait for successful release of RB0 */
                tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;
              }    
              else    /* Failed to send a internal command to RRC LLC.*/
              {      
                ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
              }
            }
            else
            {
              /* Sends the RRC Connection Request message to Send Chain  and This
                 function also starts the timer T300 */
              tdsrrcrce_send_rrc_connection_request_msg();
              /* The RCE procedure waits for the RRC Connection Setup Message */
              tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP;

              /* Increments the re-transmission counter for RRC Connection
                 Request Message */
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);

              tdsrrc_v_300++;  
            }
          }
          else /* Channel Config for RB0 failure */
          {
            event_report(EVENT_TDSCDMA_RRC_RB0_SETUP_FAILURE);
			TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
				                       TDS_SELF_HOSTING_MAJOR_CALL_SETUP_EXCEPTION,
				                       TDS_SELF_HOSTING_MINOR_RRC_RB0_SETUP_FAILURE);

            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Channel Config for RB0 failure. tdsrrcrce_substate %d",tdsrrcrce_substate);

#ifdef FEATURE_INTERRAT_PCCO_G2TD
            if(tdsrrccho_is_g2td_pcco_in_progress())
            {
              tdsrrc_conn_setup_ind_type conn_setup_ind_params;
              conn_setup_ind_params.status = FAILURE;
              conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;
              
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Channel Config for RB0 failure when PCCO active.tdsrrcrce_substate %d",tdsrrcrce_substate);
              tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  
                  
              /* set rce_substate to initial */
              tdsrrcrce_substate = TDSRRCRCE_INITIAL;
              tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
              break;
            }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

            if(tdsrrcrce_send_abort_cnf)
            {
              if(TDSRRCRCE_SUCCESS == tdsrrcrce_send_rrc_abort_cnf() )
              { 
                /* Clear the ESTABLISHED_RABS while bringing down RRC Connection*/
                tdsrrc_clear_established_rabs();

                /* Reset Signalling Connections status */
                tdsrrcscr_reset_signalling_connection_status();

                tdsrrcrce_substate = TDSRRCRCE_INITIAL;
                tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;

              }
              else
              {
                ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
              }
            }
            else
            {
              /* Send RRC_ABORT_IND command to the MM */
              if(TDSRRCRCE_SUCCESS == 
                 tdsrrcrce_send_rrc_abort_ind(RRC_INTERNAL_ERROR) )      
              {
                /* The RCE Procedure ends */
                tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */
                tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
              }    
              else
              {     
                ERR_FATAL("Unable to post MM Command: %d",RRC_ABORT_IND,0,0);
              }    
            }

            /* Trigger Cell Selection */
            if(TDSRRCRCE_SUCCESS ==
               tdsrrcrce_send_rrc_initiate_cell_selection_ind(
               TDSRRC_CELL_SEL_RB0_SETUP_FAILURE, TDSRRC_STATE_DISCONNECTED) )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB0 setup failure");
            }
            else
            {
              ERR_FATAL("Failed to post int cmd: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
            }
          }
        }
        else
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring tr_id %d in chl config cnf. Expected tr_id = %d",cmd_ptr->cmd.chan_config_cnf.transaction_id,tdsrrcrce_chl_config_transaction_id);
        }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_CHANNEL_CONFIG_CNF's proc :%d",cmd_ptr->cmd.chan_config_cnf.procedure);
      }

      break;

    default:     
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;
   }  

} /* tdsrrcrce_process_rce_wait_for_rb0_setup_cnf_substate  */

/*===========================================================================

FUNCTION tdsrrcrce_compare_arrays

DESCRIPTION
  This function checks whether two arrays of specified length are equal or not

DEPENDENCIES
  This function converts type of arrays into charecter arrays before comparing.
  Hence length should be multiplied suitably if integer or any other types are
  compared.

RETURN VALUE
  TRUE if both are same. otherwise FALSE

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrcrce_compare_arrays
(
  void *str1,                               /* First array */
  void *str2,                               /* Second array */
  uint32 length                             /* Length of the array */
)
{  
  if(!memcmp((const char *) str1,(const char *) str2,length) )
  { 
    return (TRUE);
  }
  else
  {
    return(FALSE);
  }
} /* tdsrrcrce_compare_arrays */


/*===========================================================================

FUNCTION tdsrrcrce_verify_ue_identity

DESCRIPTION
  This function checks whether UE identity in the downlink CCCH message is 
  same as UE initial identity.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: This returns TDSRRCRCE_SUCCESS if the match occurs. 
  Otherwise it returns TDSRRCRCE_FAILURE_MINOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_verify_ue_identity
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to the received 
                                                command */
)
{
  uint32 i;                                  /* Temporary variables for 
                                                indexing */
  uint8 message_choice;                       /* Local variable for message
                                                type */ 
  uint16 initial_ue_id_choice_pos;           /* Local variable to store the
                                                position for Initial UE 
                                                Identity t.*/
  rrc_initial_ue_id_type initial_ue_id;      /* Pointer to internal format 
                                                Initial UE Indentity. */
  rrc_initial_ue_id_choice_e_type initial_ue_id_choice;

  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MINOR; 
                                             /* The status is initialized to
                                                Failure */ 
  
  if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type != tdsrrc_DL_CCCH_Message_PDU)
  {
    return (status);
  }
 
  message_choice = 
    tdsrrc_get_dl_ccch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                 cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

  /* Just the initialize the Initial UE Id Choice Position to an
     invalid position */
  initial_ue_id_choice_pos = 0;

  /* Initialize temporary variable to invalid Initial UE Id Choice */
  i = 0xff;
  
  switch (message_choice)
  {
    case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup:
      /* Calculate Initial UE Identity Choice position */

      /* Check whether Integrity Check info is present */
      if(0x00 == b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                           TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                           TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
      {
        /* Check if there are enough bits to peek into the message type */
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
            (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
             TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
             TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE +
             TDSNUM_OF_BITS_FOR_NON_CRITIAL_EXT_PRESENT +
             TDSNUM_OF_BITS_FOR_RRC_CONN_SETUP_OPTIONAL_FIELDS +
             TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE )
          )
        {
        
          initial_ue_id_choice_pos = 
            TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
            TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
            TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
            TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE +
            TDSNUM_OF_BITS_FOR_NON_CRITIAL_EXT_PRESENT +
            TDSNUM_OF_BITS_FOR_RRC_CONN_SETUP_OPTIONAL_FIELDS;
        }
        if(initial_ue_id_choice_pos != 0)
        {
        /*Peek into byte stream and get the Initial UE Id t */
          i =
            b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                      initial_ue_id_choice_pos,
                      TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE);
        }

      
      }
      /* The integrity Check Info is present, Skip 36 bits for message type */
      else if( 0x01 == b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                           TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                           TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
      {
        /* Check if there are enough bits to peek into the message type */
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
            (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
             TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
             TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
             TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE +
             TDSNUM_OF_BITS_FOR_NON_CRITIAL_EXT_PRESENT +
             TDSNUM_OF_BITS_FOR_RRC_CONN_SETUP_OPTIONAL_FIELDS +
             TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE )
          )
        {
      
          initial_ue_id_choice_pos = 
            TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
            TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
            TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
            TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
            TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE +
            TDSNUM_OF_BITS_FOR_NON_CRITIAL_EXT_PRESENT +
            TDSNUM_OF_BITS_FOR_RRC_CONN_SETUP_OPTIONAL_FIELDS;

        }
        if(initial_ue_id_choice_pos != 0)
        {
        /*Peek into byte stream and get the Initial UE Id t */
          i =
            b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                    initial_ue_id_choice_pos,
                    TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE);
        }

      }
      else
      {
   
        ERR_FATAL("Invalid Integrity Check Info Present",0,0,0);
      }    
      
      break;
  case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionRelease:

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"rrcconnectionrelease rcvd");
      
      break;

    case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionReject:
      /* Calculate Initial UE Identity Choice position */

      /* Check whether Integrity Check info is present */
      if(0x00 == b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                           TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                           TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
      {
        /* Check if there are enough bits to peek into the message type */
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
            (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
             TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
             TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE +
             TDSNUM_OF_BITS_FOR_NON_CRITIAL_EXT_PRESENT +
             TDSNUM_OF_BITS_FOR_RRC_CONN_REJECT_OPTIONAL_FIELDS +
             TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE )
          )
        {
        
          initial_ue_id_choice_pos = 
            TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
            TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
            TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
            TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE +
            TDSNUM_OF_BITS_FOR_NON_CRITIAL_EXT_PRESENT +
            TDSNUM_OF_BITS_FOR_RRC_CONN_REJECT_OPTIONAL_FIELDS;
        }
        if(initial_ue_id_choice_pos != 0)
        {
        /*Peek into byte stream and get the Initial UE Id t */
          i =
            b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                      initial_ue_id_choice_pos,
                      TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE);
        }

      
      }
      /* The integrity Check Info is present, Skip 36 bits for message type */
      else if( 0x01 == b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                           TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                           TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
      {
        /* Check if there are enough bits to peek into the message type */
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
            (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
             TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
             TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
             TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE +
             TDSNUM_OF_BITS_FOR_NON_CRITIAL_EXT_PRESENT +
             TDSNUM_OF_BITS_FOR_RRC_CONN_REJECT_OPTIONAL_FIELDS +
             TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE )
          )
        {
      
          initial_ue_id_choice_pos = 
            TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
            TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
            TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
            TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
            TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE +
            TDSNUM_OF_BITS_FOR_NON_CRITIAL_EXT_PRESENT +
            TDSNUM_OF_BITS_FOR_RRC_CONN_REJECT_OPTIONAL_FIELDS;

        }
        if(initial_ue_id_choice_pos != 0)
        {
        /*Peek into byte stream and get the Initial UE Id t */
          i =
            b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                    initial_ue_id_choice_pos,
                    TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE);
        }

      }
      else
      {
   
        ERR_FATAL("Invalid Integrity Check Info Present",0,0,0);
      }    
      
      break;

    default:
      /* This should never happen. Dispatcher should route only RRC 
         Connection Setup  and RRC Connection Reject messages to RRCRCE 
         proecdure */
      /*lint -e793*/
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"wrong message routed to RCE Proc: %d",message_choice);
      /*lint +e793*/
      break;

  }
  /*lint -save -e685 */
  if ((rrc_initial_ue_id_choice_e_type)i > RRC_IMEI_UE_ID)  
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Initial UE Id Choice rcvd: %d",i);
    return(status);
  }
  /*lint -restore */
  else /* Continue with peeking in to Initial UE Id */
  {
    /* Lint e64 */
    initial_ue_id_choice = (rrc_initial_ue_id_choice_e_type)i;

    switch(initial_ue_id_choice)
    {
      case RRC_IMSI_UE_ID:
        /*Peek into byte stream and get the IMSI variable length */
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
            (uint32) (initial_ue_id_choice_pos +
             TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
             TDSNUM_OF_BITS_FOR_VARIABLE_IMSI_LENGTH )
          )
        {
          i =
            b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                    (uint16) (initial_ue_id_choice_pos + 
                    TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE),
                    TDSNUM_OF_BITS_FOR_VARIABLE_IMSI_LENGTH);

          if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
              (uint32) (initial_ue_id_choice_pos +
               TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
               TDSNUM_OF_BITS_FOR_DIGIT * 
               (TDSNUM_OF_BITS_FOR_VARIABLE_IMSI_LENGTH + 
               TDSNUM_OF_BITS_FOR_FIXED_IMSI_LENGTH) )
            )
          {
             /* Update IMSI length */
            initial_ue_id.imsi.num_digits = 
              i + TDSNUM_OF_BITS_FOR_FIXED_IMSI_LENGTH;

            if(initial_ue_id.imsi.num_digits > RRC_MAX_GSMMAP_IMSI_DIGITS)
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid number of IMSI digits");
              return status;
            }

            if(initial_ue_id.imsi.num_digits == 
                 tdsrrc_initial_ue_identity.imsi.num_digits)
            {
              for( i = 0; i < initial_ue_id.imsi.num_digits; i++)
              {
                /*Peek into byte stream and get the IMSI values */
                initial_ue_id.imsi.value[i] = 
                  b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                            (uint16) (initial_ue_id_choice_pos + 
                            TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +
                            TDSNUM_OF_BITS_FOR_VARIABLE_IMSI_LENGTH +
                            (TDSNUM_OF_BITS_FOR_DIGIT * i) ),
                            TDSNUM_OF_BITS_FOR_DIGIT );
               
              }
            }
            /* Compare the Initial UE Id with the received one */
            if( (initial_ue_id_choice == tdsrrc_initial_ue_identity_choice) &&
                (initial_ue_id.imsi.num_digits == 
                 tdsrrc_initial_ue_identity.imsi.num_digits) &&
                (tdsrrcrce_compare_arrays(initial_ue_id.imsi.value,
                                  tdsrrc_initial_ue_identity.imsi.value,
                                  (initial_ue_id.imsi.num_digits * 
                                   sizeof(uint8)  ) ) )
               )
            {
              
               status = TDSRRCRCE_SUCCESS;
            }
          }
        }         
        break;

      case RRC_TMSI_LAI_UE_ID:
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
              (uint32) (initial_ue_id_choice_pos +
               TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
               TDSNUM_OF_BITS_FOR_TMSI +
               (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
               TDSNUM_OF_BITS_FOR_OPTIONAL_MNC)
              )
        {
          /* Peek into byte stream and identify number digits for MNC */
          initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits = 2 + 
             b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                      (uint16) (initial_ue_id_choice_pos + 
                      TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                      TDSNUM_OF_BITS_FOR_TMSI +
                      (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) ),
                      TDSNUM_OF_BITS_FOR_OPTIONAL_MNC);
          
          if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
              (uint32) (initial_ue_id_choice_pos +
               TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
               TDSNUM_OF_BITS_FOR_TMSI +
               (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
               TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
               (initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits * TDSNUM_OF_BITS_FOR_DIGIT) +
               RRC_MAX_GSMMAP_LAC_LENGTH * 8)
            ) 
          {
       
            for (i = 0; i < RRC_MAX_GSMMAP_TMSI_LENGTH; i ++)
            {
              /* Peek into byte stream and get the TMSI values */
              initial_ue_id.tmsi_lai.tmsi.value[i] = 
                b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                          (uint16) (initial_ue_id_choice_pos + 
                          TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                          (8 * i) ), 8 );
            }
          
            /* Peek into byte stream and get the MCC values */
            for (i = 0; i < RRC_MAX_MCC_DIGITS; i ++)
            {
              /* Peek into byte stream and get the TMSI values */
              initial_ue_id.tmsi_lai.lai.plmn.mcc[i] = 
                b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                          (uint16) (initial_ue_id_choice_pos + 
                          TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                          TDSNUM_OF_BITS_FOR_TMSI +
                          (TDSNUM_OF_BITS_FOR_DIGIT * i) ),
                          TDSNUM_OF_BITS_FOR_DIGIT);
            }
            
            if(initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits ==
                 tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.num_mnc_digits )
            {
              for (i = 0; ((i < initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits) && (i < RRC_MAX_MNC_DIGITS)); i++)
              {
                /* Peek into byte stream and get the MNC values */
                initial_ue_id.tmsi_lai.lai.plmn.mnc[i] = 
                  b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                            (uint16) (initial_ue_id_choice_pos + 
                            TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                            TDSNUM_OF_BITS_FOR_TMSI +
                            (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
                            TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
                            (TDSNUM_OF_BITS_FOR_DIGIT * i) ),
                            TDSNUM_OF_BITS_FOR_DIGIT);
              }
            
            
              /* Peek into byte stream and get LAC values */  
              for (i = 0; i < RRC_MAX_GSMMAP_LAC_LENGTH; i++ )
              {
                initial_ue_id.tmsi_lai.lai.lac[i] =
                  b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                        (uint16) (initial_ue_id_choice_pos + 
                        TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                        TDSNUM_OF_BITS_FOR_TMSI +
                        (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
                         TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
                        (TDSNUM_OF_BITS_FOR_DIGIT * 
                         initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits) + 
                        ( 8 * i) ),8 );
              }
            }
            
            if(tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.num_mnc_digits > RRC_MAX_MNC_DIGITS)
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid number of MNC digits");
              return status;
            }

            if( (initial_ue_id_choice == tdsrrc_initial_ue_identity_choice) &&
                (initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits ==
                 tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.num_mnc_digits ) &&
                tdsrrcrce_compare_arrays(initial_ue_id.tmsi_lai.lai.plmn.mcc,
                                      tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.mcc,
                                      RRC_MAX_MCC_DIGITS) &&                
                tdsrrcrce_compare_arrays(initial_ue_id.tmsi_lai.lai.plmn.mnc,
                                      tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.mnc,
                                      tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.num_mnc_digits) &&
                tdsrrcrce_compare_arrays(initial_ue_id.tmsi_lai.tmsi.value,
                                      tdsrrc_initial_ue_identity.tmsi_lai.tmsi.value,
                                      RRC_MAX_GSMMAP_TMSI_LENGTH) &&
                tdsrrcrce_compare_arrays(initial_ue_id.tmsi_lai.lai.lac,
                                      tdsrrc_initial_ue_identity.tmsi_lai.lai.lac,
                                      RRC_MAX_GSMMAP_LAC_LENGTH)
              )
            {
              
               status = TDSRRCRCE_SUCCESS;
            }
          }
        }       
        break;

      case RRC_PTMSI_RAI_UE_ID:
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
              (uint32) (initial_ue_id_choice_pos +
               TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
               TDSNUM_OF_BITS_FOR_PTMSI +
               (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
               TDSNUM_OF_BITS_FOR_OPTIONAL_MNC)
              )
        {
          /* Peek into byte stream and identify number digits for MNC */
          initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits = 2 + 
             b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                      (uint16) (initial_ue_id_choice_pos + 
                      TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                      TDSNUM_OF_BITS_FOR_PTMSI +
                      (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) ),
                      TDSNUM_OF_BITS_FOR_OPTIONAL_MNC);
          
          if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
              (uint32) (initial_ue_id_choice_pos +
               TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
               TDSNUM_OF_BITS_FOR_PTMSI +
               (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
               TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
               (initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits * TDSNUM_OF_BITS_FOR_DIGIT) +
               RRC_MAX_GSMMAP_LAC_LENGTH * 8 +
               RRC_MAX_GSMMAP_RAC_LENGTH * 8)
            ) 
          {
       
            for (i = 0; i < RRC_MAX_GSMMAP_PTMSI_LENGTH; i ++)
            {
              /* Peek into byte stream and get the TMSI values */
              initial_ue_id.ptmsi_rai.ptmsi.value[i] = 
                b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                          (uint16) (initial_ue_id_choice_pos + 
                          TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                          (8 * i) ), 8 );
            }
          
            /* Peek into byte stream and get the MCC values */
            for (i = 0; i < RRC_MAX_MCC_DIGITS; i ++)
            {
              /* Peek into byte stream and get the TMSI values */
              initial_ue_id.ptmsi_rai.rai.lai.plmn.mcc[i] = 
                b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                          (uint16) (initial_ue_id_choice_pos + 
                          TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                          TDSNUM_OF_BITS_FOR_PTMSI +
                          (TDSNUM_OF_BITS_FOR_DIGIT * i) ),
                          TDSNUM_OF_BITS_FOR_DIGIT);
            }
            if(initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits ==
                 tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.num_mnc_digits )
            {
              for (i = 0; ((i < initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits) && (i < RRC_MAX_MNC_DIGITS)); i++)
              {
                /* Peek into byte stream and get the MNC values */
                initial_ue_id.ptmsi_rai.rai.lai.plmn.mnc[i] = 
                  b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                            (uint16) (initial_ue_id_choice_pos + 
                            TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                            TDSNUM_OF_BITS_FOR_PTMSI +
                            (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
                            TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
                            (TDSNUM_OF_BITS_FOR_DIGIT * i) ),
                            TDSNUM_OF_BITS_FOR_DIGIT);
              }
            
             
              /* Peek into byte stream and get LAC values */  
              for (i = 0; i < RRC_MAX_GSMMAP_LAC_LENGTH; i++ )
              {
                initial_ue_id.ptmsi_rai.rai.lai.lac[i] =
                  b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                        (uint16) (initial_ue_id_choice_pos + 
                        TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                        TDSNUM_OF_BITS_FOR_TMSI +
                        (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
                         TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
                        (TDSNUM_OF_BITS_FOR_DIGIT * 
                         initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits) + 
                        ( 8 * i) ),8 );
              }
              /* Peek into byte stream and get RAC value */
              for (i = 0; i < RRC_MAX_GSMMAP_RAC_LENGTH; i++)
              {
                initial_ue_id.ptmsi_rai.rai.rac[i] =
                  b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                        (uint16) (initial_ue_id_choice_pos + 
                        TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                        TDSNUM_OF_BITS_FOR_TMSI +
                        (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
                         TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
                        (TDSNUM_OF_BITS_FOR_DIGIT * 
                         initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits) + 
                        ( 8 * RRC_MAX_GSMMAP_LAC_LENGTH) + (8 * i) ),8 );
              }
            }

            if(tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.num_mnc_digits > RRC_MAX_MNC_DIGITS)
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid number of MNC digits");
              return status;
            }

            if( (initial_ue_id_choice == tdsrrc_initial_ue_identity_choice) &&
                (initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits ==
                 tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.num_mnc_digits ) &&

                tdsrrcrce_compare_arrays(initial_ue_id.ptmsi_rai.rai.lai.plmn.mcc,
                                tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.mcc,
                                RRC_MAX_MCC_DIGITS)  &&
                tdsrrcrce_compare_arrays(initial_ue_id.ptmsi_rai.rai.lai.plmn.mnc,
                                tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.mnc,
                                tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.
                                num_mnc_digits)  &&
                tdsrrcrce_compare_arrays(initial_ue_id.ptmsi_rai.ptmsi.value,
                                      tdsrrc_initial_ue_identity.ptmsi_rai.ptmsi.value,
                                      RRC_MAX_GSMMAP_PTMSI_LENGTH) &&
                tdsrrcrce_compare_arrays(initial_ue_id.ptmsi_rai.rai.lai.lac,
                                      tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.lac,
                                      RRC_MAX_GSMMAP_LAC_LENGTH) &&
                tdsrrcrce_compare_arrays(initial_ue_id.ptmsi_rai.rai.rac,
                                      tdsrrc_initial_ue_identity.ptmsi_rai.rai.rac,
                                      RRC_MAX_GSMMAP_RAC_LENGTH) 
              )
            {
              
               status = TDSRRCRCE_SUCCESS;
            }
          }
        }       
        break;

      case RRC_IMEI_UE_ID:        
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
            (uint32)(initial_ue_id_choice_pos +
             TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
             (TDSNUM_OF_BITS_FOR_DIGIT * RRC_NUM_IMEI_DIGITS)  )
          )
        {
          for( i = 0; i < RRC_NUM_IMEI_DIGITS; i++)
          {
            /*Peek into byte stream and get the IMEI values */
            initial_ue_id.imei.value[i] = 
              b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                        (uint16) (initial_ue_id_choice_pos + 
                        TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +
                        (TDSNUM_OF_BITS_FOR_DIGIT * i) ),
                        TDSNUM_OF_BITS_FOR_DIGIT );
       
          }
          if( (initial_ue_id_choice == tdsrrc_initial_ue_identity_choice) &&
                (tdsrrcrce_compare_arrays(initial_ue_id.imei.value,
                                  tdsrrc_initial_ue_identity.imei.value,
                                  (RRC_NUM_IMEI_DIGITS *
                                   sizeof(uint8)  )   ) )
               )
            {
              
               status = TDSRRCRCE_SUCCESS;
            }
        }
        break;

      default:
        ERR_FATAL("This should never happen",0,0,0);
        break;
    }
  }

  return(status);
} /* tdsrrcrce_verify_ue_identity */

/*===========================================================================

FUNCTION rrcrce_verify_ue_identity_later_than_r3

DESCRIPTION
  This function checks whether UE identity in the downlink CCCH message is 
  same as UE initial identity.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: This returns TDSRRCRCE_SUCCESS if the match occurs. 
  Otherwise it returns TDSRRCRCE_FAILURE_MINOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_verify_ue_identity_later_than_r3
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to the received 
                                                command */
)
{
  uint32 i;                                  /* Temporary variables for 
                                                indexing */
  uint8 message_choice;                       /* Local variable for message
                                                type */ 
  uint16 initial_ue_id_choice_pos;           /* Local variable to store the
                                                position for Initial UE 
                                                Identity t.*/
  rrc_initial_ue_id_type initial_ue_id;      /* Pointer to internal format 
                                                Initial UE Indentity. */
  rrc_initial_ue_id_choice_e_type initial_ue_id_choice;

  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MINOR; 
                                             /* The status is initialized to
                                                Failure */ 
  
  if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type != tdsrrc_DL_CCCH_Message_PDU)
  {
    return (status);
  }
 
  message_choice = 
    tdsrrc_get_dl_ccch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                 cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

  /* Just the initialize the Initial UE Id Choice Position to an
     invalid position */
  initial_ue_id_choice_pos = 0;

  /* Initialize temporary variable to invalid Initial UE Id Choice */
  i = 0xff;
  
  switch (message_choice)
  {
    case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup:
      /* Calculate Initial UE Identity Choice position */

      /* Check whether Integrity Check info is present */
      if(0x00 == b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                           TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                           TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
      {
        /* Check if there are enough bits to peek into the message type */
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
            (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
             TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
             TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE +
             TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE )
          )
        {
        
          initial_ue_id_choice_pos = 
            TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
            TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
            TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
            TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE;
            
        }
        if(initial_ue_id_choice_pos != 0)
        {
        /*Peek into byte stream and get the Initial UE Id t */
          i =
            b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                      initial_ue_id_choice_pos,
                      TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE);
        }

      
      }
      /* The integrity Check Info is present, Skip 36 bits for message type */
      else if( 0x01 == b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                           TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                           TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
      {
        /* Check if there are enough bits to peek into the message type */
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
            (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
             TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
             TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
             TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE +
             TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE )
          )
        {
      
          initial_ue_id_choice_pos = 
            TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
            TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
            TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
            TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
            TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE;
            

        }
        if(initial_ue_id_choice_pos != 0)
        {
        /*Peek into byte stream and get the Initial UE Id t */
          i =
            b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                    initial_ue_id_choice_pos,
                    TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE);
        }

      }
      else
      {
   
        ERR_FATAL("Invalid Integrity Check Info Present ",0,0,0);
      }    
      
      break;


    case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionRelease:

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"rrcconnectionrelease rcvd");
      
      break;

    case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionReject:
      /* Calculate Initial UE Identity Choice position */

      /* Check whether Integrity Check info is present */
      if(0x00 == b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                           TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                           TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
      {
        /* Check if there are enough bits to peek into the message type */
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
            (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
             TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
             TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE +
             TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE )
          )
        {
        
          initial_ue_id_choice_pos = 
            TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
            TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
            TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
            TDSNUM_OF_BITS_FOR_RRC_CONN_REJECT_OPTIONAL_FIELDS;
        }
        if(initial_ue_id_choice_pos != 0)
        {
        /*Peek into byte stream and get the Initial UE Id t */
          i =
            b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                      initial_ue_id_choice_pos,
                      TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE);
        }

      
      }
      /* The integrity Check Info is present, Skip 36 bits for message type */
      else if( 0x01 == b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                           TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                           TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
      {
        /* Check if there are enough bits to peek into the message type */
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
            (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
             TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
             TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
             TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE +
             TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE )
          )
        {
      
          initial_ue_id_choice_pos = 
            TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
            TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
            TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
            TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE + 
            TDSNUM_OF_BITS_FOR_RRC_CONN_REJECT_OPTIONAL_FIELDS;

        }
        if(initial_ue_id_choice_pos != 0)
        {
        /*Peek into byte stream and get the Initial UE Id t */
          i =
            b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                    initial_ue_id_choice_pos,
                    TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE);
        }

      }
      else
      {
   
        ERR_FATAL("Invalid Integrity Check Info Present",0,0,0);
      }    
      
      break;

    default:
      /* This should never happen. Dispatcher should route only RRC 
         Connection Setup  and RRC Connection Reject messages to RRCRCE 
         proecdure */
      /*lint -e793*/
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"wrong message routed to RCE Proc: %d",message_choice);
      /*lint +e793*/
      break;

  }
  /*lint -save -e685 */
  if ((rrc_initial_ue_id_choice_e_type)i > RRC_IMEI_UE_ID)  
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Initial UE Id Choice rcvd: %d",i);
    return(status);
  }
  /*lint -restore */
  else /* Continue with peeking in to Initial UE Id */
  {
    /* Lint e64 */
    initial_ue_id_choice = (rrc_initial_ue_id_choice_e_type)i;

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PEEKING VALID");

    switch(initial_ue_id_choice)
    {
      case RRC_IMSI_UE_ID:
        /*Peek into byte stream and get the IMSI variable length */
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
            (uint32) (initial_ue_id_choice_pos +
             TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
             TDSNUM_OF_BITS_FOR_VARIABLE_IMSI_LENGTH )
          )
        {
          i =
            b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                    (uint16) (initial_ue_id_choice_pos + 
                    TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE),
                    TDSNUM_OF_BITS_FOR_VARIABLE_IMSI_LENGTH);

          if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
              (uint32) (initial_ue_id_choice_pos +
               TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
               TDSNUM_OF_BITS_FOR_DIGIT * 
               (TDSNUM_OF_BITS_FOR_VARIABLE_IMSI_LENGTH + 
               TDSNUM_OF_BITS_FOR_FIXED_IMSI_LENGTH) )
            )
          {
             /* Update IMSI length */
            initial_ue_id.imsi.num_digits = 
              i + TDSNUM_OF_BITS_FOR_FIXED_IMSI_LENGTH;

            if(initial_ue_id.imsi.num_digits > RRC_MAX_GSMMAP_IMSI_DIGITS)
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid number of IMSI digits");
              return status;
            }

            if(initial_ue_id.imsi.num_digits == 
                  tdsrrc_initial_ue_identity.imsi.num_digits)
            {
               for( i = 0; i < initial_ue_id.imsi.num_digits; i++)
               {
                 /*Peek into byte stream and get the IMSI values */
                 initial_ue_id.imsi.value[i] = 
                   b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                             (uint16) (initial_ue_id_choice_pos + 
                             TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +
                             TDSNUM_OF_BITS_FOR_VARIABLE_IMSI_LENGTH +
                             (TDSNUM_OF_BITS_FOR_DIGIT * i) ),
                             TDSNUM_OF_BITS_FOR_DIGIT );
                
               }
            }
            /* Compare the Initial UE Id with the received one */
            if( (initial_ue_id_choice == tdsrrc_initial_ue_identity_choice) &&
                (initial_ue_id.imsi.num_digits == 
                 tdsrrc_initial_ue_identity.imsi.num_digits) &&
                (tdsrrcrce_compare_arrays(initial_ue_id.imsi.value,
                                  tdsrrc_initial_ue_identity.imsi.value,
                                  (initial_ue_id.imsi.num_digits * 
                                   sizeof(uint8)  ) ) )
               )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"IMSI MATCHED");
              
               status = TDSRRCRCE_SUCCESS;
            }
          }
        }         
        break;

      case RRC_TMSI_LAI_UE_ID:
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
              (uint32) (initial_ue_id_choice_pos +
               TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
               TDSNUM_OF_BITS_FOR_TMSI +
               (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
               TDSNUM_OF_BITS_FOR_OPTIONAL_MNC)
              )
        {
          /* Peek into byte stream and identify number digits for MNC */
          initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits = 2 + 
             b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                      (uint16) (initial_ue_id_choice_pos + 
                      TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                      TDSNUM_OF_BITS_FOR_TMSI +
                      (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) ),
                      TDSNUM_OF_BITS_FOR_OPTIONAL_MNC);
          
          if(initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits > RRC_MAX_MNC_DIGITS)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid number of MNC digits");
            return status;
          }

          if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
              (uint32) (initial_ue_id_choice_pos +
               TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
               TDSNUM_OF_BITS_FOR_TMSI +
               (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
               TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
               (initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits * TDSNUM_OF_BITS_FOR_DIGIT) +
               RRC_MAX_GSMMAP_LAC_LENGTH * 8)
            ) 
          {
       
            for (i = 0; i < RRC_MAX_GSMMAP_TMSI_LENGTH; i ++)
            {
              /* Peek into byte stream and get the TMSI values */
              initial_ue_id.tmsi_lai.tmsi.value[i] = 
                b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                          (uint16) (initial_ue_id_choice_pos + 
                          TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                          (8 * i) ), 8 );
            }
          
            /* Peek into byte stream and get the MCC values */
            for (i = 0; i < RRC_MAX_MCC_DIGITS; i ++)
            {
              /* Peek into byte stream and get the TMSI values */
              initial_ue_id.tmsi_lai.lai.plmn.mcc[i] = 
                b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                          (uint16) (initial_ue_id_choice_pos + 
                          TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                          TDSNUM_OF_BITS_FOR_TMSI +
                          (TDSNUM_OF_BITS_FOR_DIGIT * i) ),
                          TDSNUM_OF_BITS_FOR_DIGIT);
            }
            if(initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits ==
                 tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.num_mnc_digits )
            {
              for (i = 0; i < initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits; i++)
              {
                /* Peek into byte stream and get the MNC values */
                initial_ue_id.tmsi_lai.lai.plmn.mnc[i] = 
                  b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                            (uint16) (initial_ue_id_choice_pos + 
                            TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                            TDSNUM_OF_BITS_FOR_TMSI +
                            (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
                            TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
                            (TDSNUM_OF_BITS_FOR_DIGIT * i) ),
                            TDSNUM_OF_BITS_FOR_DIGIT);
              }
            
             
              /* Peek into byte stream and get LAC values */  
              for (i = 0; i < RRC_MAX_GSMMAP_LAC_LENGTH; i++ )
              {
                initial_ue_id.tmsi_lai.lai.lac[i] =
                  b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                        (uint16) (initial_ue_id_choice_pos + 
                        TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                        TDSNUM_OF_BITS_FOR_TMSI +
                        (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
                         TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
                        (TDSNUM_OF_BITS_FOR_DIGIT * 
                         initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits) + 
                        ( 8 * i) ),8 );
              }
            }
            
            if(tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.num_mnc_digits > RRC_MAX_MNC_DIGITS)
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid number of MNC digits");
              return status;
            }

            if( (initial_ue_id_choice == tdsrrc_initial_ue_identity_choice) &&
                (initial_ue_id.tmsi_lai.lai.plmn.num_mnc_digits ==
                 tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.num_mnc_digits ) &&
                tdsrrcrce_compare_arrays(initial_ue_id.tmsi_lai.lai.plmn.mcc,
                                      tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.mcc,
                                      RRC_MAX_MCC_DIGITS) &&                
                tdsrrcrce_compare_arrays(initial_ue_id.tmsi_lai.lai.plmn.mnc,
                                      tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.mnc,
                                      tdsrrc_initial_ue_identity.tmsi_lai.lai.plmn.num_mnc_digits) &&
                tdsrrcrce_compare_arrays(initial_ue_id.tmsi_lai.tmsi.value,
                                      tdsrrc_initial_ue_identity.tmsi_lai.tmsi.value,
                                      RRC_MAX_GSMMAP_TMSI_LENGTH) &&
                tdsrrcrce_compare_arrays(initial_ue_id.tmsi_lai.lai.lac,
                                      tdsrrc_initial_ue_identity.tmsi_lai.lai.lac,
                                      RRC_MAX_GSMMAP_LAC_LENGTH)
              )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"TMSI MATCHED");
              
               status = TDSRRCRCE_SUCCESS;
            }
          }
        }       
        break;

      case RRC_PTMSI_RAI_UE_ID:
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
              (uint32) (initial_ue_id_choice_pos +
               TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
               TDSNUM_OF_BITS_FOR_PTMSI +
               (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
               TDSNUM_OF_BITS_FOR_OPTIONAL_MNC)
              )
        {
          /* Peek into byte stream and identify number digits for MNC */
          initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits = 2 + 
             b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                      (uint16) (initial_ue_id_choice_pos + 
                      TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                      TDSNUM_OF_BITS_FOR_PTMSI +
                      (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) ),
                      TDSNUM_OF_BITS_FOR_OPTIONAL_MNC);
          
          if(initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits > RRC_MAX_MNC_DIGITS)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid number of MNC digits");
            return status;
          }

          if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
              (uint32) (initial_ue_id_choice_pos +
               TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
               TDSNUM_OF_BITS_FOR_PTMSI +
               (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
               TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
               (initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits * TDSNUM_OF_BITS_FOR_DIGIT) +
               RRC_MAX_GSMMAP_LAC_LENGTH * 8 +
               RRC_MAX_GSMMAP_RAC_LENGTH * 8)
            ) 
          {
       
            for (i = 0; i < RRC_MAX_GSMMAP_PTMSI_LENGTH; i ++)
            {
              /* Peek into byte stream and get the TMSI values */
              initial_ue_id.ptmsi_rai.ptmsi.value[i] = 
                b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                          (uint16) (initial_ue_id_choice_pos + 
                          TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                          (8 * i) ), 8 );
            }
          
            /* Peek into byte stream and get the MCC values */
            for (i = 0; i < RRC_MAX_MCC_DIGITS; i ++)
            {
              /* Peek into byte stream and get the TMSI values */
              initial_ue_id.ptmsi_rai.rai.lai.plmn.mcc[i] = 
                b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                          (uint16) (initial_ue_id_choice_pos + 
                          TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                          TDSNUM_OF_BITS_FOR_PTMSI +
                          (TDSNUM_OF_BITS_FOR_DIGIT * i) ),
                          TDSNUM_OF_BITS_FOR_DIGIT);
            }
            if(initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits ==
                 tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.num_mnc_digits )
            {
              for (i = 0; i < initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits; i++)
              {
                /* Peek into byte stream and get the MNC values */
                initial_ue_id.ptmsi_rai.rai.lai.plmn.mnc[i] = 
                  b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                            (uint16) (initial_ue_id_choice_pos + 
                            TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                            TDSNUM_OF_BITS_FOR_PTMSI +
                            (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
                            TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
                            (TDSNUM_OF_BITS_FOR_DIGIT * i) ),
                            TDSNUM_OF_BITS_FOR_DIGIT);
              }
            
              /* Peek into byte stream and get LAC values */  
              for (i = 0; i < RRC_MAX_GSMMAP_LAC_LENGTH; i++ )
              {
                initial_ue_id.ptmsi_rai.rai.lai.lac[i] =
                  b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                        (uint16) (initial_ue_id_choice_pos + 
                        TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                        TDSNUM_OF_BITS_FOR_TMSI +
                        (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
                         TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
                        (TDSNUM_OF_BITS_FOR_DIGIT * 
                         initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits) + 
                        ( 8 * i) ),8 );
              }
              /* Peek into byte stream and get RAC value */
              for (i = 0; i < RRC_MAX_GSMMAP_RAC_LENGTH; i++)
              {
                initial_ue_id.ptmsi_rai.rai.rac[i] =
                  b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                        (uint16) (initial_ue_id_choice_pos + 
                        TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +                       
                        TDSNUM_OF_BITS_FOR_TMSI +
                        (TDSNUM_OF_BITS_FOR_DIGIT * RRC_MAX_MCC_DIGITS) +
                         TDSNUM_OF_BITS_FOR_OPTIONAL_MNC +
                        (TDSNUM_OF_BITS_FOR_DIGIT * 
                         initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits) + 
                        ( 8 * RRC_MAX_GSMMAP_LAC_LENGTH) + (8 * i) ),8 );
              }
            }
            
            if(tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.num_mnc_digits > RRC_MAX_MNC_DIGITS)
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid number of MNC digits");
              return status;
            }

            if( (initial_ue_id_choice == tdsrrc_initial_ue_identity_choice) &&
                (initial_ue_id.ptmsi_rai.rai.lai.plmn.num_mnc_digits ==
                 tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.num_mnc_digits ) &&

                tdsrrcrce_compare_arrays(initial_ue_id.ptmsi_rai.rai.lai.plmn.mcc,
                                tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.mcc,
                                RRC_MAX_MCC_DIGITS)  &&
                tdsrrcrce_compare_arrays(initial_ue_id.ptmsi_rai.rai.lai.plmn.mnc,
                                tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.mnc,
                                tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.plmn.
                                num_mnc_digits)  &&
                tdsrrcrce_compare_arrays(initial_ue_id.ptmsi_rai.ptmsi.value,
                                      tdsrrc_initial_ue_identity.ptmsi_rai.ptmsi.value,
                                      RRC_MAX_GSMMAP_PTMSI_LENGTH) &&
                tdsrrcrce_compare_arrays(initial_ue_id.ptmsi_rai.rai.lai.lac,
                                      tdsrrc_initial_ue_identity.ptmsi_rai.rai.lai.lac,
                                      RRC_MAX_GSMMAP_LAC_LENGTH) &&
                tdsrrcrce_compare_arrays(initial_ue_id.ptmsi_rai.rai.rac,
                                      tdsrrc_initial_ue_identity.ptmsi_rai.rai.rac,
                                      RRC_MAX_GSMMAP_RAC_LENGTH) 
              )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"PTMSI MATCHED");
              
               status = TDSRRCRCE_SUCCESS;
            }
          }
        }       
        break;

      case RRC_IMEI_UE_ID:        
        if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8) >= 
            (uint32)(initial_ue_id_choice_pos +
             TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE + 
             (TDSNUM_OF_BITS_FOR_DIGIT * RRC_NUM_IMEI_DIGITS)  )
          )
        {
          for( i = 0; i < RRC_NUM_IMEI_DIGITS; i++)
          {
            /*Peek into byte stream and get the IMEI values */
            initial_ue_id.imei.value[i] = 
              b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                        (uint16) (initial_ue_id_choice_pos + 
                        TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE +
                        (TDSNUM_OF_BITS_FOR_DIGIT * i) ),
                        TDSNUM_OF_BITS_FOR_DIGIT );
       
          }
          if( (initial_ue_id_choice == tdsrrc_initial_ue_identity_choice) &&
                (tdsrrcrce_compare_arrays(initial_ue_id.imei.value,
                                  tdsrrc_initial_ue_identity.imei.value,
                                  (RRC_NUM_IMEI_DIGITS *
                                   sizeof(uint8)  )   ) )
               )
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"IMEI MATCHED");
              
               status = TDSRRCRCE_SUCCESS;
            }
        }
        break;

      default:
        ERR_FATAL("This should never happen",0,0,0);
        break;
    }
  }
  if (status == TDSRRCRCE_SUCCESS)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"INITIAL UE ID MATCHED");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"INITIAL UE ID NOT MATCHED");
  }
  return(status);
} /* tdsrrcrce_verify_ue_identity_later_than_r3 */

/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_connection_request_msg

DESCRIPTION
  This function builds the RRC CONNECTION REQUEST message and calls the Send
  Chain to transmit the same. Even if for some reason connection request is not
  sent, then also T300 will be started so that sending connection request can be 
  retried after its expiry.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_send_rrc_connection_request_msg
(
  void
)
{
  tdsrrc_UL_CCCH_Message *msg_ptr;             /* Pointer to uplink CCCH message*/

  rlc_lc_id_type ul_lc_id;                  /* Logical Channel Id */
  
 
 /* Checks whether Logical Channel entity setup for CCCH logical
       Channel type */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_CCCH,
                                         CCCH_RADIO_BEARER_ID,                                           
                                         UE_MODE_TRANSPARENT);
  /* If RLC Logical Channel Id is already setup, the send RRC Connection
     Request message through SEND_CHAIN */
  if(ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    /* Allocate a buffer for an uplink CCCH Message. This buffer should be 
       released by tdsrrcsend_send_ul_sdu() function using OSS compiler functions */

    msg_ptr = (tdsrrc_UL_CCCH_Message *) tdsrrc_malloc ( 
                                      sizeof(struct tdsrrc_UL_CCCH_Message));
    if (msg_ptr != NULL)
    {
      tdsrrcrce_build_rrc_connection_request_msg(msg_ptr); 
      /* This places SDU into RLC watermark queue */
      if(TDSRRCSEND_SUCCESS != 
         tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RCE,msg_ptr,ul_lc_id,
         TDSRRCSEND_L2ACK_NOT_REQUIRED) )
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to transmit RRC Conn Req Msg,Wait for T300 to expire before retransmitting again.");
      }
    }
    else
    {
      ERR_FATAL("Memory alloc for RRC Conn Req failed",0,0,0);
    }
    TDSRRC_DEBUG_INCREMENT_STATS(TDSRRC_RCE_ATTEMP);
  }
  else
  { 
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Can't find RLC ID for UL RB 0,Wait for T300 to expire before retransmitting again.");
  } 
  tdsrrcrce_allow_ta_flag = FALSE;
} /* tdsrrcrce_send_rrc_connection_request_msg */

#if defined(FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT)
/*===========================================================================

FUNCTION tdsrrcrce_process_default_config_info

DESCRIPTION
  This function proecess default configuration info specified in
  RRC Connection setup msg
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type : It returns TDSRRCRCE_SUCCESS if default config id 
  1 or 2 is indicated by NW and next state is DCH. 

SIDE EFFECTS
  None
===========================================================================*/
static tdsrrcrce_status_e_type tdsrrcrce_process_default_config_info (uint32 config_id_received,
                                                         tdsrrc_RRC_StateIndicator next_rrc_state)
{

  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MINOR; 
  
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrcrce_default_config_info.rbs_ptr = NULL;
#endif /*FEATURE_TDSCDMA_SRB5_SUPPORT*/

  tdsrrcrce_default_config_info.sib16_ptr = NULL;
  
  if((config_id_received== TDSRRC_DEF_CONFIG_0) ||
     (config_id_received == TDSRRC_DEF_CONFIG_1) ||
     (config_id_received == TDSRRC_DEF_CONFIG_3) ||
     (config_id_received == TDSRRC_DEF_CONFIG_4) ||
     (config_id_received == TDSRRC_DEF_CONFIG_6) ||
     (config_id_received == TDSRRC_DEF_CONFIG_7) ||
     (config_id_received == TDSRRC_DEF_CONFIG_8) ||
     (config_id_received == TDSRRC_DEF_CONFIG_9) ||
     (config_id_received == TDSRRC_DEF_CONFIG_10)
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
     || (config_id_received == TDSRRC_DEF_CONFIG_11)
     || (config_id_received == TDSRRC_DEF_CONFIG_12)
#ifdef FEATURE_TDSCDMA_VOC_AMR_WB     
     ||(config_id_received == TDSRRC_DEF_CONFIG_13)
     ||(config_id_received == TDSRRC_DEF_CONFIG_14)
#endif /*FEATURE_TDSCDMA_VOC_AMR_WB*/     
#endif /*FEATURE_TDSCDMA_SRB5_SUPPORT*/
     ||(config_id_received == TDSRRC_DEF_CONFIG_15)
     ||(config_id_received == TDSRRC_DEF_CONFIG_16)
     ||(config_id_received == TDSRRC_DEF_CONFIG_18)
     ||(config_id_received == TDSRRC_DEF_CONFIG_19)     
     ||(config_id_received == TDSRRC_DEF_CONFIG_20) 
     ||(config_id_received == TDSRRC_DEF_CONFIG_21) 
     ||(config_id_received == TDSRRC_DEF_CONFIG_22)
/* 17,23 are related with UPA or DPA on SRB, now unsupport. */
//     ||(config_id_received == TDSRRC_DEF_CONFIG_17)
//     ||(config_id_received == TDSRRC_DEF_CONFIG_23)
     )
  { 
    /* Since the Transport channel type is DCH, other cases have to be rejected */
    if(next_rrc_state == tdsrrc_RRC_StateIndicator_cell_DCH)
    {

      if((config_id_received == TDSRRC_DEF_CONFIG_17)
        ||(config_id_received == TDSRRC_DEF_CONFIG_23)
      )
      {
        tdsrrcrce_default_config_info.dcch_msg_ptr = NULL;
        tdsrrcrce_default_config_info.dcch_msg_ptr = 
            (tdsrrc_DL_DCCH_Message *)tdsrrcasn1_decode_pdu( (void *)tdsdefault_config_info[config_id_received].
                                                   default_config_ptr, tdsrrc_DL_DCCH_Message_PDU,
                                                   tdsdefault_config_info[config_id_received].default_config_len
                                                   );
        if(tdsrrcrce_default_config_info.dcch_msg_ptr == NULL)
        {  
          ERR_FATAL("No memory reserved for rb_setup_r6_ptr ptr",0,0,0);
        } 
      }
      else
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT    
      if((config_id_received == TDSRRC_DEF_CONFIG_11) ||
         (config_id_received == TDSRRC_DEF_CONFIG_12) ||
         (config_id_received == TDSRRC_DEF_CONFIG_13) ||
         (config_id_received == TDSRRC_DEF_CONFIG_14))
      {
        tdsrrcrce_default_config_info.rbs_ptr = 
          (tdsrrc_DL_DCCH_Message *)tdsrrcasn1_decode_pdu((void *)tdsdefault_config_info[config_id_received].
                                                   default_config_ptr, tdsrrc_DL_DCCH_Message_PDU,
                                                   tdsdefault_config_info[config_id_received].default_config_len
                                                   );
        if(tdsrrcrce_default_config_info.rbs_ptr == NULL)
        {  
          ERR_FATAL("No memory reserved for RBS ptr",0,0,0);
        } 
      }
      else
#endif /*FEATURE_TDSCDMA_SRB5_SUPPORT*/     
      {
        tdsrrcrce_default_config_info.sib16_ptr = 
          (tdsrrc_SysInfoType16 *)tdsrrcasn1_decode_pdu( (void *)tdsdefault_config_info[config_id_received].
                                                   default_config_ptr, tdsrrc_SysInfoType16_PDU,
                                                   tdsdefault_config_info[config_id_received].default_config_len
                                                   );
  
        if(tdsrrcrce_default_config_info.sib16_ptr == NULL)
        {  
          ERR_FATAL("No memory reserved for SIB16 ptr",0,0,0);
        } 
      }
      tdsrrcrce_default_config_info.config_type = TDSRRC_DEFAULT_CONFIG;
      tdsrrcrce_default_config_info.def_config_ptr_present = TRUE;
      tdsrrcrce_default_config_info.default_config_id = config_id_received;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Default configuration ID: %d received",config_id_received);
      status = TDSRRCRCE_SUCCESS;
    } 
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RCS: Default config - %d with non-DCH target state",config_id_received);
    }
  }
  else
  { 
    /* Ideally we shouldn't be here - but being too cautious */
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid default configuration ID: %d received",config_id_received); 
  }

  return status;
}
#endif /*#FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT*/

/*===========================================================================

FUNCTION tdsrrcrce_validate_rrc_connection_setup_msg

DESCRIPTION
  This function validates whether RRC CONNECTION SETUP message contain min.
  RB info to setup RRC Connection.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type : It returns TDSRRCRCE_SUCCESS if it contains RB 1,RB 2,
  RB 3 and optionally RB 4 info. Otherwise it returns TDSRRCRCE_FAILURE_MINOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_validate_rrc_connection_setup_msg
(
  tdsrrc_DL_CCCH_Message *msg_ptr,              /* Pointer to Downlink CCCH Msg */
  uint32 dl_sdu_type,                         /* Downlink SDU type */
  tdsrrc_msg_ver_enum_type* msg_version_ptr
)
{
  tdsrrc_RRCConnectionSetup_r4_IEs* rrc_conn_ptr_r4 = NULL;

  tdsrrc_RRCConnectionSetup_r5_IEs* rrc_conn_ptr = NULL;

  tdsrrc_RRCConnectionSetup_r6_IEs* rrc_conn_ptr_r6 = NULL;
  tdsrrc_SRB_InformationSetupList2_r6 * rrcrce_srb_information_setup_r6_list_ptr= NULL;
#if defined(FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT)
  /*ptr to rb setup r6     */
  tdsrrc_RadioBearerSetup_r6_IEs * rb_setup_r6_ptr = NULL;
#endif /* FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT */
  tdsrrc_RRCConnectionSetup_r7_IEs* rrc_conn_ptr_r7 = NULL;

#ifdef FEATURE_TDSCDMA_REL8
  tdsrrc_RRCConnectionSetup_r8_IEs* rrc_conn_ptr_r8 = NULL;
#endif /* FEATURE_TDSCDMA_REL8 */

#ifdef FEATURE_TDSCDMA_REL9
    tdsrrc_RRCConnectionSetup_r9_IEs* rrc_conn_ptr_r9 = NULL;
#endif /* FEATURE_TDSCDMA_REL9 */

  tdsrrc_SRB_InformationSetupList2 * rrcrce_srb_information_setup_list_ptr = NULL;
                                             /* Local pointer to SRB 
                                                Information List*/

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  /*Local Pointer to store SRBs for DCs with SRB5*/
  tdsrrc_SRB_InformationSetupList_r5 * rrcrce_srb5_information_setup_list_ptr = NULL;
#endif /*FEATURE_TDSCDMA_SRB5_SUPPORT*/

  uint32 number_of_rbs = 0;                  /* Number SRBs received thru
                                                RRC Connection Setup Msg */
  tdsrrc_state_e_type next_state;               /* Local variable for state */
  tdsrrc_RRC_StateIndicator next_rrc_state ;
  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MINOR; 
  boolean dpch_info_present = FALSE;
  /* Set message version to rel 99 by default */
  *msg_version_ptr = TDSMSG_REL4;


  /*First Update msg_version_ptr with correct version of RRC Conn Setup.*/
  if (msg_ptr->message.u.rrcConnectionSetup.t ==
        T_tdsrrc_RRCConnectionSetup_r3)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:rrcconnectionr99 rcvd");    
    tdsrrc_print_supported_asn1_rel_version();
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Critical extensions are not supported");
     /* Status is already set to TDSRRCRCE_FAILURE_MINOR */
    return(status); 
  }
  else if(SUCCESS == tdsrrc_ccch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup, TDSMSG_REL4))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:rrcconnectionr4 rcvd");
    *msg_version_ptr = TDSMSG_REL4;
  }
  else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
       (SUCCESS == tdsrrc_ccch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup, TDSMSG_REL5)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:rrcconnectionr5 rcvd");
    *msg_version_ptr = TDSMSG_REL5;
  }
  else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6) &&
       (SUCCESS == tdsrrc_ccch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup, TDSMSG_REL6)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL:rrcconnectionr6 rcvd");
    *msg_version_ptr = TDSMSG_REL6;
  }
  else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7) &&
       (SUCCESS == tdsrrc_ccch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup, TDSMSG_REL7)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL:rrcconnectionr7 rcvd");
    *msg_version_ptr = TDSMSG_REL7;
  }
#ifdef FEATURE_TDSCDMA_REL8 
  else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8) &&
       (SUCCESS == tdsrrc_ccch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup, TDSMSG_REL8)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:RRC Conn Setup is rcvd");
    *msg_version_ptr = TDSMSG_REL8;
  }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
  else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9) &&
       (SUCCESS == tdsrrc_ccch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup, TDSMSG_REL9)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:RRC Conn Setup is rcvd");
    *msg_version_ptr = TDSMSG_REL9;
  }
#endif /*FEATURE_TDSCDMA_REL9*/
  else
  {
    tdsrrc_print_supported_asn1_rel_version();
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:RRC Connection Critical extensions not supported");
    /* Status is already set to TDSRRCRCE_FAILURE_MINOR */
    return(status); 
  }


  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
    memset((void*)&tdsrrcrce_default_config_info, 0, sizeof(tdsrrcrce_default_config_info));
    tdsrrcrce_default_config_info.config_type = TDSRRC_INVALID_CONFIG;
  }


  switch (*msg_version_ptr) 
  {
    case TDSMSG_REL4:
      /*  Validate Transaction Id */
      if (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier > 3)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Transaction Id is received: %d",msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier);
        /* Status is already set to TDSRRCRCE_FAILURE_MINOR */  
        return(status);
      }
      rrc_conn_ptr_r4 = &msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.r4.rrcConnectionSetup_r4;

      next_rrc_state = rrc_conn_ptr_r4->rrc_StateIndicator;

      rrcrce_srb_information_setup_list_ptr = 
           (tdsrrc_SRB_InformationSetupList2 * ) &rrc_conn_ptr_r4->srb_InformationSetupList ;

      dpch_info_present = (((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_conn_ptr_r4, 
                             tdsrrc_RRCConnectionSetup_r4_IEs,ul_ChannelRequirement)) && 
                            (TDSRRC_SET_COMMON_MSG_TYPE(rrc_conn_ptr_r4->ul_ChannelRequirement,
                             rrc_UL_ChannelRequirement_ul_DPCH_Info))) ||
                           ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_conn_ptr_r4,
                             tdsrrc_RRCConnectionSetup_r4_IEs,dl_InformationPerRL_List)) &&
                            (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_conn_ptr_r4->dl_InformationPerRL_List.elem[0],
                             tdsrrc_DL_InformationPerRL,dl_DPCH_InfoPerRL))));
      break;

    case TDSMSG_REL5:
      rrc_conn_ptr = &msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.r5.rrcConnectionSetup_r5;
    
      if (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier > 3)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Transaction Id is received: %d",msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier);
        /* Status is already set to TDSRRCRCE_FAILURE_MINOR */  
        return(status);
      }

      next_rrc_state = rrc_conn_ptr->rrc_StateIndicator;

      /* At present, we support complete & default configuration modes only */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_conn_ptr->specificationMode, 
          rrc_RRCConnectionSetup_r5_IEs_specificationMode_complete))
      {
        tdsrrcrce_default_config_info.config_type = TDSRRC_COMPLETE_CONFIG;
        rrcrce_srb_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList2 *)
        &rrc_conn_ptr->specificationMode.u.complete.srb_InformationSetupList;
      }
      else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_conn_ptr->specificationMode,
               rrc_RRCConnectionSetup_r5_IEs_specificationMode_preconfiguration))
      {
        if(rrc_conn_ptr->specificationMode.u.preconfiguration.preConfigMode.t == 
           T_tdsrrc_RRCConnectionSetup_r5_IEs_preConfigMode_defaultConfig)
        {
          /*The process is for TDD 
            And it is needed under the FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT*/
#ifdef FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT
          if(rrc_conn_ptr->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigMode == 
             tdsrrc_DefaultConfigMode_tdd)
          {
            uint32 config_id_received = TDSRRC_DEF_CONFIG_INVALID;

            config_id_received = rrc_conn_ptr->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity;

            if((config_id_received == TDSRRC_DEF_CONFIG_0) ||
               (config_id_received == TDSRRC_DEF_CONFIG_1) ||
               (config_id_received == TDSRRC_DEF_CONFIG_3) ||
               (config_id_received == TDSRRC_DEF_CONFIG_4) ||
               (config_id_received == TDSRRC_DEF_CONFIG_6) ||
               (config_id_received == TDSRRC_DEF_CONFIG_7) ||
               (config_id_received == TDSRRC_DEF_CONFIG_8) ||
               (config_id_received == TDSRRC_DEF_CONFIG_9) ||
               (config_id_received == TDSRRC_DEF_CONFIG_10)
#if defined(FEATURE_TDSCDMA_SRB5_SUPPORT)
               ||(config_id_received == TDSRRC_DEF_CONFIG_11)
#ifdef FEATURE_TDSCDMA_VOC_AMR_WB
               ||(config_id_received == TDSRRC_DEF_CONFIG_13)
#endif /*FEATURE_TDSCDMA_VOC_AMR_WB*/
#endif /*FEATURE_TDSCDMA_SRB5_SUPPORT */
               )
            { 
              /* Since the Transport channel type is DCH, other cases have to be rejected */
              if(rrc_conn_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
              {
#if defined(FEATURE_TDSCDMA_SRB5_SUPPORT)          
                if((config_id_received == TDSRRC_DEF_CONFIG_11) ||
                  (config_id_received == TDSRRC_DEF_CONFIG_13))
                {
                  tdsrrcrce_default_config_info.rbs_ptr = 
                    (tdsrrc_DL_DCCH_Message *)tdsrrcasn1_decode_pdu( (void *)tdsdefault_config_info[config_id_received].
                                                   default_config_ptr, tdsrrc_DL_DCCH_Message_PDU,
                                                   tdsdefault_config_info[config_id_received].default_config_len
                                                   );
                  if(tdsrrcrce_default_config_info.rbs_ptr == NULL)
                  {  
                    ERR_FATAL("No memory reserved for RBS ptr",0,0,0);
                  } 
                  rrcrce_srb5_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList_r5 *)
                    &tdsrrcrce_default_config_info.rbs_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.u.r5.radioBearerSetup_r5.srb_InformationSetupList;
                }
                else
#endif /*FEATURE_TDSCDMA_SRB5_SUPPORT */
                {
                  tdsrrcrce_default_config_info.sib16_ptr = (tdsrrc_SysInfoType16 *)
                      tdsrrcasn1_decode_pdu( (void *)tdsdefault_config_info[config_id_received].
                      default_config_ptr, tdsrrc_SysInfoType16_PDU,
                      tdsdefault_config_info[config_id_received].default_config_len);
  
                  if(tdsrrcrce_default_config_info.sib16_ptr == NULL)
                  {  
                    ERR_FATAL("No memory reserved for SIB16 ptr",0,0,0);
                  } 
                  rrcrce_srb_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList2 *)
                      &tdsrrcrce_default_config_info.sib16_ptr->preDefinedRadioConfiguration.predefinedRB_Configuration.srb_InformationList;
                }
                tdsrrcrce_default_config_info.config_type = TDSRRC_DEFAULT_CONFIG;
                tdsrrcrce_default_config_info.def_config_ptr_present = TRUE;
                tdsrrcrce_default_config_info.default_config_id = config_id_received;

                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Default configuration ID: %d received",config_id_received);
              } 
              else
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"RCS: Default config - %d with non-DCH target state",config_id_received);
                return (status);
              }
            }
            else
            { 
              /* Ideally we shouldn't be here - but being too cautious */
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid default configuration ID: %d received",config_id_received);
              return(status); 
            }

          }
          else
#endif /* FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT */
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Preconfiguration Mode for FDD received .. not supported");
            return(status); 
          }
        }
        else
        { 
          /* Predefined configuration presently not supported as of now */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RCS: Predefined configuration Mode not supported");
          return (status);
        }
      }
      else

      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RCS: Preconfiguration Mode not supported");
        return (status);
      }

      dpch_info_present = (((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_conn_ptr,
                             tdsrrc_RRCConnectionSetup_r5_IEs,ul_ChannelRequirement)) && 
                            (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_conn_ptr->ul_ChannelRequirement,
                             rrc_UL_ChannelRequirement_r4_ul_DPCH_Info))) ||
                           ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_conn_ptr,
                             tdsrrc_RRCConnectionSetup_r5_IEs,dl_InformationPerRL_List)) &&
                            (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_conn_ptr->dl_InformationPerRL_List.elem[0],
                             tdsrrc_DL_InformationPerRL_r5bis,dl_DPCH_InfoPerRL))));

      break;
    case TDSMSG_REL6:
      rrc_conn_ptr_r6 = &msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6;

      if (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier > 3)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Transaction Id is received: %d",msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier);
        /* Status is already set to TDSRRCRCE_FAILURE_MINOR */  
        return(status);
      }

      next_rrc_state = rrc_conn_ptr_r6->rrc_StateIndicator;

      /* At present, we support complete & default configuration modes only */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_conn_ptr_r6->specificationMode,
          rrc_RRCConnectionSetup_r6_IEs_specificationMode_complete))
      {
        tdsrrcrce_default_config_info.config_type = TDSRRC_COMPLETE_CONFIG;
        rrcrce_srb_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList2 *)
        &rrc_conn_ptr_r6->specificationMode.u.complete.srb_InformationSetupList;
      }
      else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_conn_ptr_r6->specificationMode,
               rrc_RRCConnectionSetup_r6_IEs_specificationMode_preconfiguration))
      {
        if( rrc_conn_ptr_r6->specificationMode.u.preconfiguration.preConfigMode.t == 
           T_tdsrrc_RRCConnectionSetup_r6_IEs_preConfigMode_defaultConfig)
        {
          /*The process is for TDD 
            And it is needed under the FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT*/
#ifdef FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT
          if(rrc_conn_ptr_r6->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigMode == 
             tdsrrc_DefaultConfigMode_tdd)
          {
            if (TDSRRCRCE_SUCCESS != tdsrrcrce_process_default_config_info( 
              rrc_conn_ptr_r6->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity,
              next_rrc_state))
            {
              return(status);
            }
         

            if((tdsrrcrce_default_config_info.default_config_id == TDSRRC_DEF_CONFIG_17)
             || (tdsrrcrce_default_config_info.default_config_id == TDSRRC_DEF_CONFIG_23)
            )
            {
              rb_setup_r6_ptr = tdsrrcrce_get_rb_setup_r6_ptr_for_dc();
              if(rb_setup_r6_ptr != NULL)
              {
                rrcrce_srb_information_setup_r6_list_ptr = (tdsrrc_SRB_InformationSetupList2_r6 *)
                  &rb_setup_r6_ptr->specificationMode.u.complete.srb_InformationSetupList;
              }
              else
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"Default config-17 decoded msg ptr NULL,ptr val=%d",rb_setup_r6_ptr);
                return(status);
              }
            }
            else
            {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT    
              uint32 config_id_received = TDSRRC_DEF_CONFIG_INVALID;
  
              config_id_received = rrc_conn_ptr_r6->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity;
              if((config_id_received == TDSRRC_DEF_CONFIG_11) ||
                 (config_id_received == TDSRRC_DEF_CONFIG_12) ||
                 (config_id_received == TDSRRC_DEF_CONFIG_13) ||
                 (config_id_received == TDSRRC_DEF_CONFIG_14))
              {
                rrcrce_srb5_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList_r5 *)
                    &tdsrrcrce_default_config_info.rbs_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.u.r5.radioBearerSetup_r5.srb_InformationSetupList;

                if(rrcrce_srb5_information_setup_list_ptr == NULL)
                {
                          TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC with SRB5 ptr is NULL");
                          return(status);
                }
              }
              else
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
              {
                rrcrce_srb_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList2 *)
                &tdsrrcrce_default_config_info.sib16_ptr->preDefinedRadioConfiguration.predefinedRB_Configuration.srb_InformationList;
              }
            }
          }
          else
#endif /* FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT */
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Preconfiguration Mode received .. not supported in FDD mode now");
            return(status); 
          }
        }
        else
        { 
          /* Predefined configuration presently not supported as of now */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RCS: Predefined configuration Mode not supported");
          return (status);
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RCS: Preconfiguration Mode not supported");
        return (status);
      }

      dpch_info_present = (((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_conn_ptr_r6,
                             tdsrrc_RRCConnectionSetup_r6_IEs,ul_DPCH_Info)) && 
                            (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_conn_ptr_r6->ul_DPCH_Info.modeSpecificInfo,
                             rrc_UL_DPCH_Info_r6_modeSpecificInfo_tdd))) ||
                           ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_conn_ptr_r6,
                             tdsrrc_RRCConnectionSetup_r6_IEs,dl_InformationPerRL_List)) &&
                            (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_conn_ptr_r6->dl_InformationPerRL_List.elem[0], 
                             tdsrrc_DL_InformationPerRL_r6,dl_dpchInfo))));

      break;
    case TDSMSG_REL7:
      rrc_conn_ptr_r7 = &msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7;

      if (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier > 3)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Transaction Id is received: %d",msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier);
        /* Status is already set to TDSRRCRCE_FAILURE_MINOR */  
        return(status);
      }

      next_rrc_state = rrc_conn_ptr_r7->rrc_StateIndicator;

  
      /* At present, we support complete & default configuration modes only */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_conn_ptr_r7->specificationMode,
          rrc_RRCConnectionSetup_r7_IEs_specificationMode_complete))
      {
        tdsrrcrce_default_config_info.config_type = TDSRRC_COMPLETE_CONFIG;
        rrcrce_srb_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList2 *)
        &rrc_conn_ptr_r7->specificationMode.u.complete.srb_InformationSetupList;
      }
      else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_conn_ptr_r7->specificationMode,
               rrc_RRCConnectionSetup_r7_IEs_specificationMode_preconfiguration))
      {
        if(rrc_conn_ptr_r7->specificationMode.u.preconfiguration.preConfigMode.t == 
           T_tdsrrc_RRCConnectionSetup_r7_IEs_preConfigMode_defaultConfig)
        {
          /*The process is still for FDD 
            And it is needed under the FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT*/
#ifdef FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT
          if(rrc_conn_ptr_r7->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigMode == 
             tdsrrc_DefaultConfigMode_tdd)
          {

            if (TDSRRCRCE_SUCCESS != tdsrrcrce_process_default_config_info( 
              rrc_conn_ptr_r7->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity,
              next_rrc_state))
            {
              return(status);
            }
            if((tdsrrcrce_default_config_info.default_config_id == TDSRRC_DEF_CONFIG_17)
             || (tdsrrcrce_default_config_info.default_config_id == TDSRRC_DEF_CONFIG_23)
            )
            {
              rb_setup_r6_ptr = tdsrrcrce_get_rb_setup_r6_ptr_for_dc();
              if(rb_setup_r6_ptr != NULL)
              {
                rrcrce_srb_information_setup_r6_list_ptr = (tdsrrc_SRB_InformationSetupList2_r6 *)
                  &rb_setup_r6_ptr->specificationMode.u.complete.srb_InformationSetupList;
              }
              else
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"Default config-17 decoded msg ptr NULL,ptr val=%d",rb_setup_r6_ptr);
                return(status);
              }
            }
            else
            {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT    
              uint32 config_id_received = TDSRRC_DEF_CONFIG_INVALID;
  
              config_id_received = rrc_conn_ptr_r7->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity;
              if((config_id_received == TDSRRC_DEF_CONFIG_11) ||
                 (config_id_received == TDSRRC_DEF_CONFIG_12) ||
                 (config_id_received == TDSRRC_DEF_CONFIG_13) ||
                 (config_id_received == TDSRRC_DEF_CONFIG_14))
              {
                rrcrce_srb5_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList_r5 *)
                    &tdsrrcrce_default_config_info.rbs_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.u.r5.radioBearerSetup_r5.srb_InformationSetupList;

                if(rrcrce_srb5_information_setup_list_ptr == NULL)
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC with SRB5 ptr is NULL");
                  return(status);
                }
              }
              else
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
              {
                /*lint -e740 */
                rrcrce_srb_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList2 *)&tdsrrcrce_default_config_info.sib16_ptr->preDefinedRadioConfiguration.predefinedRB_Configuration.srb_InformationList;
              }
            }
            /*lint +e740 */
          }
          else
#endif /* FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT */
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Preconfiguration Mode received .. not supported in FDD mode now");
            return(status); 
          }
        }
        else
        { 
          /* Predefined configuration presently not supported as of now */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RCS: Predefined configuration Mode not supported");
          return (status);
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RCS: Preconfiguration Mode not supported");
        return (status);
      }

      dpch_info_present = (((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_conn_ptr_r7, 
                             tdsrrc_RRCConnectionSetup_r7_IEs,ul_DPCH_Info)) && 
                            (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_conn_ptr_r7->ul_DPCH_Info.modeSpecificInfo,
                             rrc_UL_DPCH_Info_r7_modeSpecificInfo_tdd))) ||
                           ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_conn_ptr_r7,
                             tdsrrc_RRCConnectionSetup_r7_IEs,dl_InformationPerRL_List)) &&
                            (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_conn_ptr_r7->dl_InformationPerRL_List.elem[0],
                             tdsrrc_DL_InformationPerRL_r7,dl_dpchInfo))));

      break;

#ifdef FEATURE_TDSCDMA_REL8
    case TDSMSG_REL8:
      rrc_conn_ptr_r8 = &msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8;

      if (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier > 3)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Transaction Id is received: %d",msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier);
        /* Status is already set to TDSRRCRCE_FAILURE_MINOR */  
        return(status);
      }

      next_rrc_state = rrc_conn_ptr_r8->rrc_StateIndicator;

  

      /* At present, we support complete & default configuration modes only */
      if (rrc_conn_ptr_r8->specificationMode.t == 
          T_tdsrrc_RRCConnectionSetup_r8_IEs_specificationMode_complete)
      {
        tdsrrcrce_default_config_info.config_type = TDSRRC_COMPLETE_CONFIG;
        /*RRC_GKG: Make sure that we don't use the following SRB ptr for anything else because we are typecasting
        different releases of SRB Lists to R99 SRB List. It's fine so long as we use it for counting.*/
        rrcrce_srb_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList2 *)
        &rrc_conn_ptr_r8->specificationMode.u.complete.srb_InformationSetupList;
      }
      else if (rrc_conn_ptr_r8->specificationMode.t == 
               T_tdsrrc_RRCConnectionSetup_r8_IEs_specificationMode_preconfiguration)
      {
        
        if(rrc_conn_ptr_r8->specificationMode.u.preconfiguration.preConfigMode.t == 
           T_tdsrrc_RRCConnectionSetup_r8_IEs_preConfigMode_defaultConfig)

        {
          /*The process is for TDD 
            And it is needed under the FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT*/
#ifdef FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT
          if(rrc_conn_ptr_r8->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigMode == 
             tdsrrc_DefaultConfigMode_tdd)
          {
            if (TDSRRCRCE_SUCCESS != tdsrrcrce_process_default_config_info( 
              rrc_conn_ptr_r8->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity,
              next_rrc_state))
            {
              return(status);
            }
            if(tdsrrcrce_default_config_info.default_config_id == TDSRRC_DEF_CONFIG_17)
            {
              rb_setup_r6_ptr = tdsrrcrce_get_rb_setup_r6_ptr_for_dc();
              if(rb_setup_r6_ptr != NULL)
              {
                rrcrce_srb_information_setup_r6_list_ptr = (tdsrrc_SRB_InformationSetupList2_r6 *)
                  &rb_setup_r6_ptr->specificationMode.u.complete.srb_InformationSetupList;
              }
              else
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"Default config-17 decoded msg ptr NULL,ptr val=%d",rb_setup_r6_ptr);
                return(status);
              }
            }
            else
            {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT    
              uint32 config_id_received = TDSRRC_DEF_CONFIG_INVALID;
  
              config_id_received = rrc_conn_ptr_r8->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity;
              if((config_id_received == TDSRRC_DEF_CONFIG_11) ||
                 (config_id_received == TDSRRC_DEF_CONFIG_12) ||
                 (config_id_received == TDSRRC_DEF_CONFIG_13) ||
                 (config_id_received == TDSRRC_DEF_CONFIG_14))
              {
                rrcrce_srb5_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList_r5 *)
                    &tdsrrcrce_default_config_info.rbs_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.u.r5.radioBearerSetup_r5.srb_InformationSetupList;

                if(rrcrce_srb5_information_setup_list_ptr == NULL)
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC with SRB5 ptr is NULL");
                  return(status);
                }
              }
              else
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
              {
                /*lint -e740 */
                rrcrce_srb_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList2 *)&tdsrrcrce_default_config_info.sib16_ptr->preDefinedRadioConfiguration.predefinedRB_Configuration.srb_InformationList;
              }
            }
            /*lint +e740 */
          }
          else
#endif /* FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT */
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Preconfiguration Mode received .. not supported in FDD mode now");
            return(status); 
          }
        }
        else
        { 
          /* Predefined configuration presently not supported as of now */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RCS: Predefined configuration Mode not supported");
          return (status);
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RCS: Preconfiguration Mode not supported");
        return (status);
      }

      dpch_info_present = (((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_conn_ptr_r8,
                             tdsrrc_RRCConnectionSetup_r8_IEs,ul_DPCH_Info)) && 
                            (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_conn_ptr_r8->ul_DPCH_Info.modeSpecificInfo, 
                             rrc_UL_DPCH_Info_r7_modeSpecificInfo_tdd))) ||
                           ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_conn_ptr_r8,
                             tdsrrc_RRCConnectionSetup_r8_IEs,dl_InformationPerRL_List)) &&
                            (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_conn_ptr_r8->dl_InformationPerRL_List.elem[0],
                             tdsrrc_DL_InformationPerRL_r8,dl_dpchInfo))));

      break;
#endif /* FEATURE_TDSCDMA_REL8*/


#ifdef FEATURE_TDSCDMA_REL9
    case TDSMSG_REL9:
      rrc_conn_ptr_r9 = &msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.r9.rrcConnectionSetup_r9;

      if (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier > 3)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Transaction Id is received: %d",msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier);
        /* Status is already set to TDSRRCRCE_FAILURE_MINOR */  
        return(status);
      }
      
      if (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.
        u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
        u.r9.m.rrcConnectionSetup_r9_add_extPresent)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"rrcConnectionSetup_r9_add_ext present");
        /* Status is already set to TDSRRCRCE_FAILURE_MINOR */  
        return(status);
      }

      next_rrc_state = rrc_conn_ptr_r9->rrc_StateIndicator;

  

      /* At present, we support complete & default configuration modes only */
      if (rrc_conn_ptr_r9->specificationMode.t == 
          T_tdsrrc_RRCConnectionSetup_r9_IEs_specificationMode_complete)
      {
        tdsrrcrce_default_config_info.config_type = TDSRRC_COMPLETE_CONFIG;
        /*RRC_GKG: Make sure that we don't use the following SRB ptr for anything else because we are typecasting
        different releases of SRB Lists to R99 SRB List. It's fine so long as we use it for counting.*/
        rrcrce_srb_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList2 *)
        &rrc_conn_ptr_r9->specificationMode.u.complete.srb_InformationSetupList;
      }
      else if (rrc_conn_ptr_r9->specificationMode.t == 
               T_tdsrrc_RRCConnectionSetup_r9_IEs_specificationMode_preconfiguration)
      {
        
        if(rrc_conn_ptr_r9->specificationMode.u.preconfiguration.preConfigMode.t == 
           T_tdsrrc_RRCConnectionSetup_r9_IEs_preConfigMode_defaultConfig)

        {
          /*The process is for TDD 
            And it is needed under the FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT*/
#ifdef FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT
          if(rrc_conn_ptr_r9->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigMode == 
             tdsrrc_DefaultConfigMode_tdd)
          {
            if (TDSRRCRCE_SUCCESS != tdsrrcrce_process_default_config_info( 
              rrc_conn_ptr_r9->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity,
              next_rrc_state))
            {
              return(status);
            }
            if((tdsrrcrce_default_config_info.default_config_id == TDSRRC_DEF_CONFIG_17)
             || (tdsrrcrce_default_config_info.default_config_id == TDSRRC_DEF_CONFIG_23)
            )							
            {
              rb_setup_r6_ptr = tdsrrcrce_get_rb_setup_r6_ptr_for_dc();
              if(rb_setup_r6_ptr != NULL)
              {
                rrcrce_srb_information_setup_r6_list_ptr = (tdsrrc_SRB_InformationSetupList2_r6 *)
                  &rb_setup_r6_ptr->specificationMode.u.complete.srb_InformationSetupList;
              }
              else
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"Default config-17 decoded msg ptr NULL,ptr val=%d",rb_setup_r6_ptr);
                return(status);
              }
            }
            else
            {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT    
              uint32 config_id_received = TDSRRC_DEF_CONFIG_INVALID;
  
              config_id_received = rrc_conn_ptr_r9->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity;
              if((config_id_received == TDSRRC_DEF_CONFIG_11) ||
                 (config_id_received == TDSRRC_DEF_CONFIG_12) ||
                 (config_id_received == TDSRRC_DEF_CONFIG_13) ||
                 (config_id_received == TDSRRC_DEF_CONFIG_14))
              {
                rrcrce_srb5_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList_r5 *)
                    &tdsrrcrce_default_config_info.rbs_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.u.r5.radioBearerSetup_r5.srb_InformationSetupList;

                if(rrcrce_srb5_information_setup_list_ptr == NULL)
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC with SRB5 ptr is NULL");
                  return(status);
                }
              }
              else
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
              {
                /*lint -e740 */
                rrcrce_srb_information_setup_list_ptr = (tdsrrc_SRB_InformationSetupList2 *)&tdsrrcrce_default_config_info.sib16_ptr->preDefinedRadioConfiguration.predefinedRB_Configuration.srb_InformationList;
              }
            }
            /*lint +e740 */
          }
          else
#endif /* FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT */
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Preconfiguration Mode received .. not supported in FDD mode now");
            return(status); 
          }
        }
        else
        { 
          /* Predefined configuration presently not supported as of now */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RCS: Predefined configuration Mode not supported");
          return (status);
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RCS: Preconfiguration Mode not supported");
        return (status);
      }

      dpch_info_present = (((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_conn_ptr_r9,
                             tdsrrc_RRCConnectionSetup_r9_IEs,ul_DPCH_Info)) && 
                            (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_conn_ptr_r9->ul_DPCH_Info.modeSpecificInfo, 
                             rrc_UL_DPCH_Info_r7_modeSpecificInfo_tdd))) ||
                           ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_conn_ptr_r9,
                             tdsrrc_RRCConnectionSetup_r9_IEs,dl_InformationPerRL_List)) &&
                            (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_conn_ptr_r9->dl_InformationPerRL_List.elem[0],
                             tdsrrc_DL_InformationPerRL_r8,dl_dpchInfo))));

      break;
#endif /* FEATURE_TDSCDMA_REL9*/


    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"msg version not recognized");
      return status;
  } /* switch*/

  if((tdsrrcrce_default_config_info.config_type == TDSRRC_DEFAULT_CONFIG)&&
    ((tdsrrcrce_default_config_info.default_config_id == TDSRRC_DEF_CONFIG_17)
             || (tdsrrcrce_default_config_info.default_config_id == TDSRRC_DEF_CONFIG_23)
    ))
  {
    if(rrcrce_srb_information_setup_r6_list_ptr == NULL)
    { /*lint -e793 */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SRB Information Setup List IE not found");
      /*lint +e793*/
      return(status); 
    }
    number_of_rbs=rrcrce_srb_information_setup_r6_list_ptr->n;
     
  }
  else
  {
    if(rrcrce_srb_information_setup_list_ptr == NULL 
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
       && rrcrce_srb5_information_setup_list_ptr == NULL
#endif /*FEATURE_TDSCDMA_SRB5_SUPPORT*/
    )
    { /*lint -e793 */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SRB Information Setup List IE not found");
      /*lint +e793*/
      return(status); 
    }
    if(rrcrce_srb_information_setup_list_ptr != NULL)
    {
      number_of_rbs = rrcrce_srb_information_setup_list_ptr->n;
    }
    /* Checks whether sufficient SRB info is present to setup 
       RRC Connection */
    
  }
  
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  if((tdsrrcrce_default_config_info.def_config_ptr_present == TRUE) &&
      (tdsrrcrce_default_config_info.rbs_ptr != NULL))
  {
    if(rrcrce_srb5_information_setup_list_ptr != NULL)
    {
      number_of_rbs = rrcrce_srb5_information_setup_list_ptr->n;
    }
  }
#endif /*FEATURE_TDSCDMA_SRB5_SUPPORT*/

  tdsrrcrce_number_of_rbs = number_of_rbs;

  /* Check whether required number SRB Info is present or not */
  if( (number_of_rbs == 3) ||  (number_of_rbs == 4) || (number_of_rbs == 5))
  {
    /* Check whether State Inidcator matches with DPCH info present */
    switch(next_rrc_state)
    {
      case tdsrrc_RRC_StateIndicator_cell_DCH:
        /* Check whether DPCH info is present in the RRC CONNECTION SETUP
           message */
        if (dpch_info_present)
        {
          next_state = TDSRRC_STATE_CELL_DCH;

          /* Updates the TDSORDERED_CONFIG with RRC CONNECTION SETUP Message
          parameters */
          if(TDSORDERED_CONFIG_SET == tdsrrcllc_set_ordered_config(
                                   TDSRRC_PROCEDURE_RCE,
                                   next_state,                                         
                                   dl_sdu_type,
                                   msg_ptr) )
          {
            status = TDSRRCRCE_SUCCESS;  
          }
        }
        break;

      case tdsrrc_RRC_StateIndicator_cell_FACH:    
        /* Future validations can be added here */
        next_state = TDSRRC_STATE_CELL_FACH;

        /* Updates the TDSORDERED_CONFIG with RRC CONNECTION SETUP Message
        parameters */
        if(TDSORDERED_CONFIG_SET == tdsrrcllc_set_ordered_config(
                                 TDSRRC_PROCEDURE_RCE,
                                 next_state,                                         
                                 dl_sdu_type,
                                 msg_ptr) )
        { 
          status = TDSRRCRCE_SUCCESS;  
        }
        break;

      case tdsrrc_RRC_StateIndicator_cell_PCH:
      case tdsrrc_RRC_StateIndicator_ura_PCH: 
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC state Indicator is rcvd: %d",next_rrc_state);
        break;
    }
  }  
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"No. of SRBs are %d",number_of_rbs);
    return(status);
  }

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
    if(tdsrrcrce_default_config_info.config_type == TDSRRC_DEFAULT_CONFIG)
    {
      tdsrrcrce_delete_default_config_info();
    }
  }

  return(status);  
 
} /* tdsrrcrce_validate_rrc_connection_setup_msg */

#ifdef FEATURE_TDSCDMA_TO_LTE
/*===========================================================================
FUNCTION rrcrce_send_lte_redirection_req

DESCRIPTION
  This function builds the RRC_REDIRECT_REQ command with all parameters
  received through RRC CONNECTION REJECT message and sends the command to
  Cell Selection Procedure
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_send_lte_redirection_req
(
  tdsrrc_DL_CCCH_Message    *msg_ptr
)
{
  tdsrrc_cmd_type *cmd_ptr;                     /* Pointer to RRC Command */
  tdsrrc_EUTRA_TargetFreqInfoList *EUTRA_target_freq_info_ptr = NULL;
  tdsrrc_EUTRA_BlacklistedCellPerFreqList *blacklisted_cell_info_ptr = NULL;
  uint8 earfcn_count = 0;
  uint8 blacklisted_cells_count = 0;
  uint32 idx, idy;
  if( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Fill in the command parameters to RRC_REDIRECT_REQ Command */
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_REDIRECT_REQ;
    cmd_ptr->cmd.redirect_req.proc_id = TDSRRC_PROCEDURE_RCE;
    cmd_ptr->cmd.redirect_req.wait_time = tdsrrcrce_wait_time;
    cmd_ptr->cmd.redirect_req.cpid = 128; /* initialize the psc to an invalid value */

    cmd_ptr->cmd.redirect_req.choice = TDSRRC_INTER_SYSTEM_LTE;
    cmd_ptr->cmd.redirect_req.u.EUTRA_target_cell_info.earfcn_count = 0;

    EUTRA_target_freq_info_ptr = &(msg_ptr->message.u.rrcConnectionReject.u.r3.laterNonCriticalExtensions.v690NonCriticalExtensions.v6f0NonCriticalExtensions.
      v860NonCriticalExtensions.rrcConnectionReject_v860ext.redirectionInfo.u.interRATInfo->u.eutra->eutra_TargetFreqInfoList);
    idx = 0;
    while((EUTRA_target_freq_info_ptr->n > idx) &&
            (earfcn_count < LTE_RRC_IRAT_MAX_REDIR_FREQ))
    {
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
      if(!lte_rrc_earfcn_is_supported(tdsrrc_get_as_id(),(lte_earfcn_t)EUTRA_target_freq_info_ptr->elem[idx].dlEUTRACarrierFreq, tdsrrccsp_get_lte_band_pref()))
#else
      if(!lte_rrc_earfcn_is_supported((lte_earfcn_t)EUTRA_target_freq_info_ptr->elem[idx].dlEUTRACarrierFreq, tdsrrccsp_get_lte_band_pref()))
#endif
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSTOL: EARFCN %d not supported by UE",EUTRA_target_freq_info_ptr->elem[idx].dlEUTRACarrierFreq);
        idx++;
        continue;
      }
      cmd_ptr->cmd.redirect_req.u.EUTRA_target_cell_info.earfcn_info[earfcn_count].earfcn = (lte_earfcn_t)EUTRA_target_freq_info_ptr->elem[idx].dlEUTRACarrierFreq;

      /* Check if blacklisted cells also present for the current EARFCN.*/
      blacklisted_cells_count = 0;
      if(EUTRA_target_freq_info_ptr->elem[idx].m.eutraBlacklistedCellPerFreqListPresent)
      {
        idy = 0;
        blacklisted_cell_info_ptr = &(EUTRA_target_freq_info_ptr->elem[idx].eutraBlacklistedCellPerFreqList);
        while((blacklisted_cell_info_ptr->n > idy) &&
                 (blacklisted_cells_count < LTE_RRC_IRAT_MAX_EUTRA_CELLS_PER_FREQ))
        {
          cmd_ptr->cmd.redirect_req.u.EUTRA_target_cell_info.earfcn_info[earfcn_count].blacklisted_cells[blacklisted_cells_count++] = 
             blacklisted_cell_info_ptr->elem[idy].physicalCellIdentity;
          idy++;
        }
      }
      cmd_ptr->cmd.redirect_req.u.EUTRA_target_cell_info.earfcn_info[earfcn_count++].blacklisted_cells_count = blacklisted_cells_count;
      idx++;
    }
    cmd_ptr->cmd.redirect_req.u.EUTRA_target_cell_info.earfcn_count = earfcn_count;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSTOL: Num of EARFCN's in redirection info = %d",earfcn_count);

    if(earfcn_count == 0)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSTOL: No target EARFCN's supported. No redirection.");
      tdsrrc_free(cmd_ptr);

      tdsrrcrce_process_connection_rej_with_wait_time();
    }
    else
    {
      if(TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
      {
        tdsrrccsp_cleanup_bplmn_state();
      }

      /* sends the command to Cell Selection Procedure */
      tdsrrc_put_int_cmd(cmd_ptr);         
      /* Wait for redirect confirm from Cell Selection Procedure */
      tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_REDIRECT_CNF;
    }
  }
  else
  {
    ERR_FATAL("Unable to get internal cmd buffer for redirect_req: %d",0,0,0);
  }
}
#endif



/*===========================================================================

FUNCTION tdsrrcrce_handle_interrat_redirection_info

DESCRIPTION
  This function processes inter-RAT redirection info received
  in RRC Connection Reject message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcrce_handle_interrat_redirection_info
(
  tdsrrc_DL_CCCH_Message    *msg_ptr
#ifdef FEATURE_TDSCDMA_TO_LTE
  , boolean lte_redirection_info
#endif
)
{
  boolean mode_enabled = TRUE;

#ifdef FEATURE_INTERRAT_PCCO_G2TD             
  if(tdsrrccho_is_g2td_pcco_in_progress())
  {
    tdsrrc_conn_setup_ind_type conn_setup_ind_params;
    conn_setup_ind_params.status = FAILURE;
    conn_setup_ind_params.conn_failure_reason = TDSRRC_CONNECTION_REJECT;

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Connection Reject with Inter-RAT redirection info.PCCO is active");
    tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);               
    
    tdsrrcrce_substate = TDSRRCRCE_INITIAL;
    tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
    return;
  }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

#ifdef FEATURE_TDSCDMA_TO_LTE
  if(lte_redirection_info)
  {
    if(tdsrrcmcm_is_lte_mode_enabled())
    {
      tdsrrcrce_rsp_to_rrc_conn_req = TDSRRCRCE_CONNECTION_REJECT;
      tdsrrcrce_send_lte_redirection_req(msg_ptr);
    }
    else
    {
      mode_enabled = FALSE;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSTOL Redir: LTE mode not enabled");
    }
  }
  else
#endif
   {
    if(tdsrrcmcm_is_dualmode_enabled())
    {
      tdsrrcrce_rsp_to_rrc_conn_req = TDSRRCRCE_CONNECTION_REJECT;
      /* sends the command to Cell Selection Procedure */
      if(tdsrrcrce_send_redirection_req(msg_ptr, FALSE) == TDSRRCRCE_SUCCESS)          
      {     
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"iRAT Redirection Req sent to CSP");
        /* Wait for redirect confirm from Cell Selection Procedure */
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_REDIRECT_CNF;   
      }  
      else 
      {                  
        ERR_FATAL("Unable to post Internal Cmd to CSP: %d",TDSRRC_REDIRECT_REQ,0,0); 
      } 
    }
    else
    {
      mode_enabled = FALSE;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"GSM mode not enabled");
    }
  }

  if(!mode_enabled)
  {
    /* Start the timer for wait time. The wait time units are Secs */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"iRAT Redirection rcvd when Mode is not enabled. Wt tmr strted");
#ifdef FEATURE_DISCONNECT_ON_UNSUPPORTED_IRAT_REDIRECTION
    if ((tdsrrc_establishment_cause == RRC_EST_EMERGENCY_CALL) &&
         tdsrrccsp_check_gsm_bands_supported())                               
    {
      if( TDSRRCRCE_SUCCESS == 
             tdsrrcrce_send_rrc_initiate_cell_selection_ind(
             TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_MAX) )
      {                            
        /* The RCE Procedure ends */
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; 

        /* Set Abort Cause */
        tdsrrcrce_mm_abort_cause = RRC_UNSUPPORTED_REDIRECTION;
      }    
    }
    else
#endif
    {
      tdsrrcrce_process_connection_rej_with_wait_time();
    }
  }
}

/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_conn_setup_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in RRCRCE_WAIT_FOR_CONN_SETUP substate of 
  Connecting State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_conn_setup_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to the received 
                                                command */
)
{

  tdsrrc_DL_CCCH_Message *msg_ptr = NULL;       /* Pointer to  downlink CCCH
                                                message */ 
  tdsrrc_N_300 rrcrce_n_300;                    /* Local Varibale to store 
                                                N300 */ 
  tdsrrc_state_e_type next_state = TDSRRC_STATE_MAX; 
                                             /* Local variable to store 
                                                next state */ 

  uint8 message_choice;                      /* Local variable to store message
                                                t */

  rrc_abort_cause_e_type  cause = RRC_MAX_ACCESS_FAILURE;
  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL4;

  /* Initialize the pointers to NULL */
  tdsrrcrce_save_dl_ccch_msg.dl_sdu      = NULL;
  tdsrrcrce_save_dl_ccch_msg.decoded_msg = NULL; 
  
  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_STATE_CHANGE_IND:
      if((cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED) &&
         ( tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_STATE_CHANGE_IND received.tdsrrcrce_substate %d",tdsrrcrce_substate);

        /* Stop the timer T300 */
        tdsrrctmr_stop_timer(TDSRRCTMR_T_300_TIMER);
#ifdef FEATURE_INTERRAT_PCCO_G2TD
        if(tdsrrccho_is_g2td_pcco_in_progress())
        {
          tdsrrc_conn_setup_ind_type conn_setup_ind_params;
          conn_setup_ind_params.status = FAILURE;
          conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;

          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_STATE_CHANGE_IND received when PCCO is active.tdsrrcrce_substate %d",tdsrrcrce_substate);      
          tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  
       
          /* set rce_substate to initial */
          tdsrrcrce_substate = TDSRRCRCE_INITIAL;
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          break;
        }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

        /* Send RRC_ABORT_IND command to the MM */
        if(TDSRRCRCE_SUCCESS == 
           tdsrrcrce_send_rrc_abort_ind(RRC_FAILURE_UNSPECIFIED) )      
        {        
           /* Reset RCE Procedure and  Procedure ends */
           tdsrrcrce_substate = TDSRRCRCE_INITIAL; 
           tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        }    
        else
        {     
          ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_IND,0,0);
        }      
      }
      
      else
      {
        ERR_FATAL("RRCSCMGR posted an unregistered event",cmd_ptr->cmd.state_change_ind.new_state ,0,0);
      }
      break;

    case TDSRRC_ABORT_REQ: 
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ rcvd in %d substate",tdsrrcrce_substate);
      /* Stop the timer T300 */
        tdsrrctmr_stop_timer(TDSRRCTMR_T_300_TIMER);
      /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
      if( TDSRRCRCE_SUCCESS == 
          tdsrrcrce_send_rrc_initiate_cell_selection_ind(
          TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
      {                            
        /* Wait for successful release of RB0 */
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;  
        tdsrrcrce_send_abort_cnf = TRUE;
      }
      else    /* Failed to send a internal command to RRC LLC.*/
      {      
        ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
      }
      
      break;

    case TDSRRC_CMAC_STATUS_IND:
      {
           
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"CMAC Status Ind with status %d",((tdsrrc_mac_cmd_type *)cmd_ptr)->cmd.cmac_status_ind);
        /* Update RACH tx stats here */
        if(((tdsrrc_mac_cmd_type *)cmd_ptr)->cmd.cmac_status_ind == TDSMAX_RACH_CYCLES_REACHED)
        {
          /*MAC has hit MAX RACH transmission cycle, so update RACH tx parameters count here*/
          TDSRRC_DEBUG_INCREMENT_STATS(TDSRACH_RRC_RETRY_FAIL_DUE_TO_CMAC_STATUS_IND);
        }
        {
          /* Value in milliseconds. Default value is 1000. */
          tdsrrc_T_300 rrcrce_t_300 = tdsrrc_T_300_ms1000;                   /* Local parameter to store T300 */
          /* Get timer T300 from the SIB 1 database */
          if (TDSRRCRCE_SUCCESS !=  tdsrrcrce_get_t300_from_serv_cell_db(&rrcrce_t_300))
          {
            ERR_FATAL("T300 value is invalid",0,0,0);
          }
          /* Only start ALLOW_TA_TIMER when RACH success and T300 is longer than RRC_SETUP_ALLOW_TA_TIMER_LENGTH*/
          if (((tdsrrc_mac_cmd_type *)cmd_ptr)->cmd.cmac_status_ind == TDSRACH_TX_SUCCESS)
          {
            if (tdsrrcrce_convert_t300_to_ms(rrcrce_t_300) > TDSRRC_RRC_SETUP_ALLOW_TA_TIMER_LENGTH)
            {
              tdsrrctmr_start_timer(TDSRRCTMR_RCE_ALLOW_TA_TIMER, TDSRRC_RRC_SETUP_ALLOW_TA_TIMER_LENGTH);
            }
            tdsrrcrce_allow_ta_flag = FALSE;
          }
          else
          {
            //Always set "tdsrrcrce_allow_ta_flag" as TRUE to allow TA if RACH fail
            tdsrrcrce_allow_ta_flag = TRUE;         
          }
          /* start the timer T300 */
          tdsrrctmr_start_timer(TDSRRCTMR_T_300_TIMER, 
                             tdsrrcrce_convert_t300_to_ms(rrcrce_t_300));  
        }
      }
      break;
    case TDSRRC_RCE_TA_ALLOW_EXPIRED_IND:
      tdsrrcrce_allow_ta_flag = TRUE;
      break;
        
    case TDSRRC_T300_EXPIRED_IND:
    case TDSRRC_NEW_CELL_IND: 
      /* Check whether the receievd command is TDSRRC_NEW_CELL_IND */
      if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_NEW_CELL_IND)
      {
        /* Stop the timer T300 */
        tdsrrctmr_stop_timer(TDSRRCTMR_T_300_TIMER);
      }
      else
      {
        if(tdsrrctmr_get_remaining_time(TDSRRCTMR_T_300_TIMER) > 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Curr T300 time remaining > 0 Ignore expiry ind");
          break;
        }
      }
       /* The UE is already in CONNECTING State. The CCM should have reconfigured
          the  RB 0*/
      /* Gets N300 from the SIB 1 database */
      if( TDSRRCRCE_SUCCESS != 
          tdsrrcrce_get_n300_from_serv_cell_db(&rrcrce_n_300))
      {
        ERR_FATAL("N300 value is invalid",0,0,0);
      }
      
      if((TDSRRCRCE_SUCCESS == tdsrrcrce_validate_cell_access_status(
                      tdsrrc_est_cn_domain_id,tdsrrc_establishment_cause,&cause)) &&
        (tdsrrc_v_300 <= rrcrce_n_300))
      {        
        /* Resend RRC connection Request Message and this function restarts the
         * T300 Timer */
        tdsrrcrce_send_rrc_connection_request_msg();
        /* RRC Connection Request Message is successfully sent */
        /* Increments the re-transmission counter for RRC Connection Request
           Message */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);

        tdsrrc_v_300++;     
          
      }
      else /* Reached maximum allowed re-tries */
      { 
        tdsrrcrce_num_retries_count++;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"tdsrrcrce_num_retries_count = %d",tdsrrcrce_num_retries_count);

#ifdef FEATURE_TDSCDMA_TO_LTE
        /* In case this is the first attampt for connection request after redirection from LTE, need to reset */
        /* the flag to indicate pre-redirection info has to be included in following connection request msg. */
        tdsrrccsp_reset_flag_for_excluding_pre_redir_info();
#endif

#ifdef FEATURE_INTERRAT_PCCO_G2TD
        /* If GTOTDS PCCO is active,sent a conn_setup_ind with failure */
        if(tdsrrccho_is_g2td_pcco_in_progress())
        {
          tdsrrc_conn_setup_ind_type conn_setup_ind_params;
          conn_setup_ind_params.status = FAILURE;
          conn_setup_ind_params.conn_failure_reason = TDSRRC_MAX_RACH_ATTEMPTS;

          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Max RACH attempts reached when PCCO active.tdsrrcrce_num_retries_count = %d",tdsrrcrce_num_retries_count);
          tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);

          /* set rce_substate to initial */
          tdsrrcrce_substate = TDSRRCRCE_INITIAL;
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          break;
        }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

        /*RRC will check whether MSET exist other better frequency which RSCP is higher than bar_cell_rscp_threshold,
          if yes RCE will bar current serv cell bar_cell_n300_failure_timer*/
        if ((tdsrrcrce_nv_est_fail_bar_cell.bar_cell_enable) && (tdsrrcrce_is_good_freq_exist_in_mset()))
        {
          tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_CPID_BARRED,tdsrrcrce_nv_est_fail_bar_cell.bar_cell_n300_failure_timer);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Serv CELL Barred due to rrc est N300 failure");
        }

        /* Release the RB 0 and request the RRC LLC to go to Idle disconnected */
        if( TDSRRCRCE_SUCCESS == 
           tdsrrcrce_send_rrc_initiate_cell_selection_ind(
          TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
        {                            
          /* The RCE Procedure ends */
          tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; /* change the RCE substate */
          if(cause  ==  RRC_ACCESS_CLASS_NOT_ALLOWED)
          {
            if(tdsrrc_est_cn_domain_id == RRC_CS_DOMAIN_CN_ID)
            {
              tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_CS_DOMAIN;
            }
            else
            {
              tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_PS_DOMAIN;
            }
          }
          else
          {
            if(tdsrrc_check_L1_accumulated_rscp() <= -850)
            {
              tdsrrcrce_mm_abort_cause = RRC_FAILURE_DUE_TO_CONGESTION;
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Report RRC_FAILURE_DUE_TO_CONGESTION to NAS for Silent Redial as RSCP <= -85dbm");
            }
            else
            {
            tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Report RRC_MAX_ACCESS_FAILURE to NAS. Silent Redial disabled as RSCP is normal");
            }
          }

        }    
        else    /* Failed to send a internal command to RRC LLC.*/
        {      
          ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
        }    

      }
      break; 
             


    case TDSRRC_DOWNLINK_SDU_IND:

      tdsrrc_protocol_error_reject = FALSE;
            
      /* Assert the received message is of type tdsrrc_DL_CCCH_Message. Also 
         Check whether reselection is in progress */
      if( (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_CCCH_Message_PDU) &&
          (FALSE == tdsrrcccm_is_reselection_in_progress() ) 
        )
      {
        // CR 374310 begin
        if (TRUE == tdsrrccsp_reselection_in_progress())
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Discard RRC Connection Setup in RCE if it detects cell reselection is ongoing");
          break;
        }
        // CR 374310 end
        message_choice = 
          tdsrrc_get_dl_ccch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

        if(cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL)
        {
           /* Get the OTA message pointer */
          msg_ptr = (tdsrrc_DL_CCCH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;
        }
        else /* ASN1 decoding failed */
        {
          tdsrrc_protocol_error_reject = TRUE;
        }  

        switch( message_choice )
        {
          
          case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup: /* RRC CONNECTION SETUP message
                                                 is received. */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Connection Setup Msg is received");
            /* Reset the tdsrrcrce_num_retries_count */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting tdsrrcrce_num_retries_count to 0");
            tdsrrcrce_num_retries_count =0;
            tdsrrcrce_allow_ta_flag = FALSE;
            /* Stop the timer T300 */
            tdsrrctmr_stop_timer(TDSRRCTMR_T_300_TIMER);
            /* Check whether ASN1 decoding as resulted in  PROTOCOL_ERROR_REJECT */
            if(tdsrrc_protocol_error_reject == TRUE)
            {
#if(!defined(T_WINNT))
              /*log the protocol error*/
              tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                                      (uint8) TDSRRC_PROCEDURE_RCE,
                                      TDSRRCLOG_PROTOCOL_ERROR,
                                      TDSRRCLOG_ASN1_ENC_ERR
                                     );
#endif

              if( TDSRRCRCE_SUCCESS !=
                  tdsrrcrce_get_n300_from_serv_cell_db(&rrcrce_n_300))
              {
                ERR_FATAL("N300 value is invalid",0,0,0);
              }
              if(tdsrrc_v_300 <= rrcrce_n_300)
              {          
                tdsrrc_protocol_error_indicator = tdsrrc_ProtocolErrorIndicator_errorOccurred;
                
                /* Resend RRC Connection Request message and this function
                   restarts the T300 Timer*/
                tdsrrcrce_send_rrc_connection_request_msg();
                /* RRC Connection Request Message is successfully sent */
                /* Increment the re-transmission counter for RRC Connection
                   Request message */
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);

                tdsrrc_v_300++;
              }
              else       /* Max re-tries exceeded */
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Max retries exceeded.tdsrrcrce_substate %d",tdsrrcrce_substate);

#ifdef FEATURE_INTERRAT_PCCO_G2TD
                if(tdsrrccho_is_g2td_pcco_in_progress())
                {
                  tdsrrc_conn_setup_ind_type conn_setup_ind_params;
                  conn_setup_ind_params.status = FAILURE;
                  conn_setup_ind_params.conn_failure_reason = TDSRRC_MAX_RACH_ATTEMPTS;

                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Max RACH attempts reached when PCCO active.tdsrrcrce_num_retries_count = %d",tdsrrcrce_num_retries_count);
                  tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);
     
                  /* set rce_substate to initial */
                  tdsrrcrce_substate = TDSRRCRCE_INITIAL;
                  tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
                  break;
                }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

                /* Release the RB 0 and request the RRC LLC to go to 
                   Idle Disconnected */
                if( TDSRRCRCE_SUCCESS == 
                    tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                    TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                {
                  /* The RCE Procedure ends */
                  tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;  /* Change the RCE substate */

                  tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
                }  
                else    /* Failed to send a internal command to RRC LLC.*/
                {      
                  ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                }  

              }
            } 
            else /* ASN1 successfully decoded the message */
            {
              tdsrrc_RRC_StateIndicator  state_indicator_instructed = tdsrrc_RRC_StateIndicator_ura_PCH; /* Other than FACH */
              tdsrrc_msg_ver_enum_type   msg_release_version = TDSMSG_INVALID;
              uint32                  mode_received = T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd;
              boolean                 freq_info_present = FALSE;
              tdsrrc_UARFCN              dl_uarfcn = 0;
              boolean                 crnti_present = FALSE;
              boolean                 dl_info_per_rl_list_present = FALSE;
              if(tdsrrc_v_300 != TDSRRC_RACH_TX_STATS_ONE_TIME_RACH)
              {
                /*RACH tx Success full for UE message sent in UL, 
                 *so update RACH tx parameters count here*/
                TDSRRC_DEBUG_INCREMENT_STATS(TDSRACH_SUCCFUL_RRC_RETRY);
              }

#ifdef FEATURE_INTERRAT_PCCO_G2TD
              if(tdsrrccho_is_g2td_pcco_in_progress())
              { 
                tdsrrc_conn_setup_ind_type conn_setup_ind_params;
                conn_setup_ind_params.status = SUCCESS;
                conn_setup_ind_params.conn_failure_reason = TDSRRC_FAILURE_NONE;

                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Connection Setup received and successfully decoded.Informing CHO proc");
                tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  
              }                  
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

              /*lint -save -e613 */
              if(msg_ptr->message.u.rrcConnectionSetup.t == T_tdsrrc_RRCConnectionSetup_r3)
              {
                 /* do nothing for REL99 */              
              }
              else if(msg_ptr->message.u.rrcConnectionSetup.u.
                       later_than_r3.criticalExtensions.t == 
                       T_tdsrrc_RRCConnectionSetup_criticalExtensions_6_r4)
              {
                msg_release_version = TDSMSG_REL4;
                state_indicator_instructed = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.
                                     criticalExtensions.u.r4.rrcConnectionSetup_r4.rrc_StateIndicator;

                  if(state_indicator_instructed == tdsrrc_RRC_StateIndicator_cell_FACH)
                  {
                    TDSRRC_MSG3(MSG_LEGACY_HIGH,"R4 ConnSetUp for FACH rcvd with freq info.FreqInfoMask = 0x%x", 
                           msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.
                                                u.r4.rrcConnectionSetup_r4.m.frequencyInfoPresent,0,0);

                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.
                                               criticalExtensions.u.r4.rrcConnectionSetup_r4,
                                               tdsrrc_RRCConnectionSetup_r4_IEs,frequencyInfo))
                    { 
                      freq_info_present = TRUE;
                      mode_received = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                                        r4.rrcConnectionSetup_r4.frequencyInfo.modeSpecificInfo.t;
                      dl_uarfcn = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                                      r4.rrcConnectionSetup_r4.frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
                    } 

                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.
                                                    u.r4.rrcConnectionSetup_r4,
                                                    tdsrrc_RRCConnectionSetup_r4_IEs,new_c_RNTI))
                    {
                      crnti_present = TRUE;
                    }

                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.
                                                    u.r4.rrcConnectionSetup_r4,
                                                    tdsrrc_RRCConnectionSetup_r4_IEs,dl_InformationPerRL_List))
                    {
                      if(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.
                            u.r4.rrcConnectionSetup_r4.dl_InformationPerRL_List.elem[0].modeSpecificInfo.t
                            == T_tdsrrc_DL_InformationPerRL_r4_modeSpecificInfo_tdd)
                      { 
                        if (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.
                                 u.r4.rrcConnectionSetup_r4.dl_InformationPerRL_List.
                               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
                        {
                          TDSRRC_MSG0(MSG_LEGACY_LOW,"Cell Parameter ID in RCE message");
                          dl_info_per_rl_list_present = TRUE;
                        }
                      }
                    }
                 }
              } /* end of Rel-4 else if */
    
              else if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
              {
                if((msg_ptr->message.u.rrcConnectionSetup.u.
                       later_than_r3.criticalExtensions.t == 
                       T_tdsrrc_RRCConnectionSetup_criticalExtensions_5_criticalExtensions) &&
                      (msg_ptr->message.u.rrcConnectionSetup.u.
                       later_than_r3.criticalExtensions.u.criticalExtensions.t == 
                       T_tdsrrc_RRCConnectionSetup_criticalExtensions_5_r5))
                {
                  msg_release_version = TDSMSG_REL5;
                  state_indicator_instructed = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                                               criticalExtensions.u.r5.rrcConnectionSetup_r5.rrc_StateIndicator;

                  if(state_indicator_instructed == tdsrrc_RRC_StateIndicator_cell_FACH)
                  {
                    TDSRRC_MSG3(MSG_LEGACY_HIGH,"R5 ConnSetUp for FACH rcvd with freq info. FreqInfoMask = 0x%x", 
                           msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                                               criticalExtensions.u.r5.rrcConnectionSetup_r5.m.frequencyInfoPresent,0,0);

                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                                               criticalExtensions.u.r5.rrcConnectionSetup_r5,
                                               tdsrrc_RRCConnectionSetup_r5_IEs,frequencyInfo))
                    { 
                      freq_info_present = TRUE;
                      mode_received = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                                 criticalExtensions.u.r5.rrcConnectionSetup_r5.frequencyInfo.modeSpecificInfo.t;
                      dl_uarfcn = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                                 criticalExtensions.u.r5.rrcConnectionSetup_r5.frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
                    } 

                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                                               criticalExtensions.u.r5.rrcConnectionSetup_r5,
                                               tdsrrc_RRCConnectionSetup_r5_IEs,new_c_RNTI))
                    {
                      crnti_present = TRUE;
                    }

                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                                               criticalExtensions.u.r5.rrcConnectionSetup_r5,
                                               tdsrrc_RRCConnectionSetup_r5_IEs,dl_InformationPerRL_List))
                    {
                      if(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                           criticalExtensions.u.r5.rrcConnectionSetup_r5.dl_InformationPerRL_List.elem[0]
                           .modeSpecificInfo.t == T_tdsrrc_DL_InformationPerRL_r5_modeSpecificInfo_tdd)
                      { 
                        if (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                              criticalExtensions.u.r5.rrcConnectionSetup_r5.dl_InformationPerRL_List.
                               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
                        {
                          TDSRRC_MSG0(MSG_LEGACY_LOW,"Cell Parameter ID in RCE message");
                          dl_info_per_rl_list_present = TRUE;
                        }
                      }
                    }
                  }
                } /* end of Rel-5 else if */
              }
              if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
              {
                if (TDSRRCRCE_R6_CRITICAL_EXT_PRESENT)
                {
                  msg_release_version = TDSMSG_REL6;
                  state_indicator_instructed = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                                               criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6.rrc_StateIndicator;

                  if(state_indicator_instructed == tdsrrc_RRC_StateIndicator_cell_FACH)
                  {
                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6,
                       tdsrrc_RRCConnectionSetup_r6_IEs,frequencyInfo))
                    { 
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"R6 ConnSetUp for FACH rcvd. Freq info is present"); 
                      freq_info_present = TRUE;
                      mode_received = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                        criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6.frequencyInfo.modeSpecificInfo.t;
                      dl_uarfcn = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                        criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6.
                        frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
                    } 

                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6,
                       tdsrrc_RRCConnectionSetup_r6_IEs,new_c_RNTI))
                    {
                      crnti_present = TRUE;
                    }

                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6,
                       tdsrrc_RRCConnectionSetup_r6_IEs,dl_InformationPerRL_List))
                    {
                      if(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                           criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6.
                           dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
                        T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_tdd)
                      { 
                        if (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                              criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6
                              .dl_InformationPerRL_List.
                               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
                        {
                          TDSRRC_MSG0(MSG_LEGACY_LOW,"Cell Parameter ID in RCE message");
                          dl_info_per_rl_list_present = TRUE;
                        }
                      }
                    }
                  }
                } /* end of Rel-6 else if */
              }

              if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
              {
                if (TDSRRCRCE_R7_CRITICAL_EXT_PRESENT)
                {
                  msg_release_version = TDSMSG_REL7;
                  state_indicator_instructed = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                                               criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7.rrc_StateIndicator;

                  if(state_indicator_instructed == tdsrrc_RRC_StateIndicator_cell_FACH)
                  {
                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7,
                       tdsrrc_RRCConnectionSetup_r7_IEs,frequencyInfo))
                    { 
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"R7 ConnSetUp for FACH rcvd. Freq info is present"); 
                      freq_info_present = TRUE;
                      mode_received = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7.frequencyInfo.modeSpecificInfo.t;
                      dl_uarfcn = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7.
                        frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
                    } 

                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7,
                       tdsrrc_RRCConnectionSetup_r7_IEs,new_c_RNTI))
                    {
                      crnti_present = TRUE;
                    }

                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7,
                       tdsrrc_RRCConnectionSetup_r7_IEs,dl_InformationPerRL_List))
                    {
                      if(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7
                         .dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
                        T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_tdd)
                      { 
                        if (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                            criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7
                            .dl_InformationPerRL_List.
                               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
                        {
                          TDSRRC_MSG0(MSG_LEGACY_LOW,"Cell Parameter ID in RCE message");
                          dl_info_per_rl_list_present = TRUE;
                        }
                      }
                    }
                  }
                } /* end of Rel-6 else if */
              }
              /*RRC_GKG: We can use else-if here. Also we can correct it for R7 and R6 also, though no issues.*/
#ifdef FEATURE_TDSCDMA_REL8
              if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8)
              {
                if (TDSRRCRCE_R8_CRITICAL_EXT_PRESENT)
                {
                  msg_release_version = TDSMSG_REL8;
                  state_indicator_instructed = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                                               criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8.rrc_StateIndicator;

                  if(state_indicator_instructed == tdsrrc_RRC_StateIndicator_cell_FACH)
                  {
                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8,
                       tdsrrc_RRCConnectionSetup_r8_IEs,frequencyInfo))
                    { 
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"R8 ConnSetUp for FACH rcvd. Freq info is present"); 
                      freq_info_present = TRUE;
                      mode_received = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8.frequencyInfo.modeSpecificInfo.t;
                      dl_uarfcn = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8.frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
                    } 

                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8,
                       tdsrrc_RRCConnectionSetup_r8_IEs,new_c_RNTI))
                    {
                      crnti_present = TRUE;
                    }

                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8,
                       tdsrrc_RRCConnectionSetup_r8_IEs,dl_InformationPerRL_List))
                    {
                      if(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8.
                          dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
                        T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd)
                      { 
                        if (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                               criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8.
                              dl_InformationPerRL_List.
                               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
                        {
                          TDSRRC_MSG0(MSG_LEGACY_LOW,"Cell Parameter ID in RCE message");
                          dl_info_per_rl_list_present = TRUE;
                        }
                      }
                    }
                  }
                } /* end of Rel-8*/
              }
#endif /* FEATURE_TDSCDMA_REL8*/

#ifdef FEATURE_TDSCDMA_REL9
              if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9)
              {
                if (TDSRRCRCE_R9_CRITICAL_EXT_PRESENT)
                {
                  msg_release_version = TDSMSG_REL9;
                  state_indicator_instructed = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                                               criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.rrcConnectionSetup_r9.rrc_StateIndicator;
              
                  if(state_indicator_instructed == tdsrrc_RRC_StateIndicator_cell_FACH)
                  {
                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.rrcConnectionSetup_r9,
                       tdsrrc_RRCConnectionSetup_r9_IEs,frequencyInfo))
                    { 
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"R9 ConnSetUp for FACH rcvd. Freq info is present"); 
                      freq_info_present = TRUE;
                      mode_received = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.rrcConnectionSetup_r9.frequencyInfo.modeSpecificInfo.t;
                      dl_uarfcn = msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.rrcConnectionSetup_r9.frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
                    } 
              
                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.rrcConnectionSetup_r9,
                       tdsrrc_RRCConnectionSetup_r9_IEs,new_c_RNTI))
                    {
                      crnti_present = TRUE;
                    }
              
                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.rrcConnectionSetup_r9,
                       tdsrrc_RRCConnectionSetup_r9_IEs,dl_InformationPerRL_List))
                    {
                      if(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.rrcConnectionSetup_r9
                         .dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
                        T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd)
                      { 
                        if (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                             criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.rrcConnectionSetup_r9
                             .dl_InformationPerRL_List.
                               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
                        {
                          TDSRRC_MSG0(MSG_LEGACY_LOW,"Cell Parameter ID in RCE message");
                          dl_info_per_rl_list_present = TRUE;
                        }
                      }
                    }
                  }
                } /* end of Rel-9*/
              }
#endif /* FEATURE_TDSCDMA_REL9*/



              
              /* If connection setup is for FACH then check for the frequency info */
              if((state_indicator_instructed == tdsrrc_RRC_StateIndicator_cell_FACH) && ((msg_release_version == TDSMSG_REL4) ||
                 (msg_release_version == TDSMSG_REL5) || (msg_release_version == TDSMSG_REL6)
                 || (msg_release_version == TDSMSG_REL7)
#ifdef FEATURE_TDSCDMA_REL8
                 || (msg_release_version == TDSMSG_REL8)
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
                 || (msg_release_version == TDSMSG_REL9)
#endif /*FEATURE_TDSCDMA_REL9*/

                ))
              {
                /* Check whether frequency info is present */
                if(freq_info_present && (dl_uarfcn != tdsrrcccm_get_curr_camped_freq()))
                {
                  if(TDSRRCRCE_SUCCESS != tdsrrcrce_get_n300_from_serv_cell_db(&rrcrce_n_300))
                  { 
                    ERR_FATAL("N300 value is invalid",0,0,0);
                  } 

                  if( tdsrrc_v_300 <= rrcrce_n_300)
                  { 
                    if(mode_received == T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
                    { 
                      /* tdsrrc_validate_dl_arfcn->tdsrrc_validate_tds_frequency, old function also just hudge DL freq */
                      if(SUCCESS == tdsrrc_validate_tds_frequency(dl_uarfcn
                                                     )
                        )
                      { 
                        tdsrrcrce_wait_time = 1;
                        tdsrrcrce_rsp_to_rrc_conn_req = TDSRRCRCE_CONNECTION_SETUP;

                        /* Check if C-RNTI is present and RL info is present for CPID */
                        if(crnti_present && dl_info_per_rl_list_present)
                        { 
                          /* Save the encoded DL_CCCH message */
                          /* Allocated memory */
                          tdsrrcrce_save_dl_ccch_msg.dl_sdu = tdsrrc_malloc(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

                          /* Copy the contents received in the allocated memory */
                          memscpy(tdsrrcrce_save_dl_ccch_msg.dl_sdu, 
                                 cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length, 
                               cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                               cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

                          /* Save the length and type of SDU */
                          tdsrrcrce_save_dl_ccch_msg.dl_sdu_length = 
                                              cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length;

                          tdsrrcrce_save_dl_ccch_msg.dl_sdu_type = 
                                              cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type;

                          /* Send the command to Cell Selection Procedure */
                          if(tdsrrcrce_send_redirection_req(msg_ptr, 
                                   tdsrrcrce_is_pccpch_info_present(&msg_ptr->message.u.rrcConnectionSetup, msg_release_version))
                                     == TDSRRCRCE_SUCCESS)
                          { 
                            TDSRRC_MSG2(MSG_LEGACY_HIGH,"ConnSetUp inter-freq redirection from (%d,%d)",tdsrrcccm_get_curr_camped_freq(),tdsrrcccm_get_curr_camped_cpid());
                          }
                          else 
                          {                       
                            ERR_FATAL("Unable to post Internal Cmd to CSP: %d",TDSRRC_REDIRECT_REQ,0,0);
                          }  
                        } 
                        /* C-RNTI absent or CPID information not present */
                        else
                        { 
                          /* Sends the command to Cell Selection Procedure */
                          if(tdsrrcrce_send_redirection_req(msg_ptr, FALSE) == TDSRRCRCE_SUCCESS)          
                          { 
                            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RCE ConnSetUp: Inter-freq redirection req for freq %d sent to CSP",dl_uarfcn);
                          }   
                          else 
                          {                       
                            ERR_FATAL("Unable to post Internal Cmd to CSP: %d",TDSRRC_REDIRECT_REQ,0,0);
                          }  
                        } 
                        /* Wait for redirect confirm from Cell Selection Proc */
                        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_REDIRECT_CNF;   
                      } 
                      /* Unsupported frequency received. Send another Conn Req */
                      else
                      { 
                        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rcvd invalid Freq %d in redirection. Send RRC Conn Req again",dl_uarfcn);

                        /* RRC Connection Request will be sent on validation failure
                         * when tdsrrcrce_validate_rrc_connection_setup_msg() function
                         * is called.
                         */
                      } 
                    } 
                    /* Incorrect mode specific info */
                    else
                    { 
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ConnSetUp iFreq Redirection rcvd with unsupported mode_choice %d. Go to Idle",mode_received);

                      /* Since this redirection is unsupported, abandon the procedure 
                       * Release RB 0 and request RRC LLC to go to Idle disconnected 
                       */
                      /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
                      if(TDSRRCRCE_SUCCESS == 
                         tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                         TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                      {
                        /* The RCE Procedure ends */
                        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; 

                        /* Set Abort Cause */
                        tdsrrcrce_mm_abort_cause = RRC_UNSUPPORTED_REDIRECTION;
                      } 
                      /* Failed to send a internal command to RRC LLC.*/
                      else    
                      { 
                        ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                      }    
                      break;
                    } 
                  } 
                  /* tdsrrc_v_300 > rrcrce_n_300 */
                  else
                  {
                    TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_v_300 %d > rrcrce_n_300 %d. release_rb. Goto idle",tdsrrc_v_300,rrcrce_n_300);
                    /* Release RB 0 and request RRC LLC to go to 
                       Idle Disconnected */
                    if( TDSRRCRCE_SUCCESS == 
                       tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                       TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                    {
                      /* The RCE Procedure ends */
                      tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;  /* Change the RCE substate */

                      tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
                    }  
                    else    /* Failed to send a internal command to RRC LLC.*/
                    {      
                      ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                    }  

                  }
                }
                /* Frequency info absent or same as the camped frequency */
                else
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"ConnSetUp iFreq Redirection for the same freq or absent");
                }
              }/* End of "if" destination state is FACH */
              /*lint -restore */

              
              /* Validate the RRC Connection Setup message */
              if((tdsrrcrce_substate == TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP) 
                  && (TDSRRCRCE_SUCCESS != tdsrrcrce_validate_rrc_connection_setup_msg(msg_ptr,
                                         cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type, &msg_version)))
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"protocol error rej has been set to true");

                /* Unsuppored IE is received from the UTRAN */
                tdsrrc_protocol_error_reject = TRUE;

                tdsrrc_protocol_error_indicator = tdsrrc_ProtocolErrorIndicator_errorOccurred;

                /* Send RRC Connection Request again if V300 <= N 300 */
                if( TDSRRCRCE_SUCCESS !=
                  tdsrrcrce_get_n300_from_serv_cell_db(&rrcrce_n_300))
                {
                  ERR_FATAL("N300 value is invalid",0,0,0);
                }
                if(tdsrrc_v_300 <= rrcrce_n_300)
                {                       
                  /* Resend RRC Connection Request message and this function
                     restarts the T300 Timer*/
                  tdsrrcrce_send_rrc_connection_request_msg();
                  /* RRC Connection Request Message is successfully sent */
                  /* Increment the re-transmission counter for RRC Connection
                       Request message */
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);

                  tdsrrc_v_300++;
                }
                else       /* Max re-tries exceeded */
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Max retries exceeded.tdsrrcrce_substate %d",tdsrrcrce_substate);

#ifdef FEATURE_INTERRAT_PCCO_G2TD
                  /* If GTOTDS PCCO is active,sent a conn_setup_ind with failure */
                  if(tdsrrccho_is_g2td_pcco_in_progress())
                  {
                    tdsrrc_conn_setup_ind_type conn_setup_ind_params;
                    conn_setup_ind_params.status = FAILURE;
                    conn_setup_ind_params.conn_failure_reason = TDSRRC_MAX_RACH_ATTEMPTS;

                    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Max RACH attempts reached when PCCO active.tdsrrcrce_num_retries_count = %d",tdsrrcrce_num_retries_count);
                    tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);
     
                    /* set rce_substate to initial */
                    tdsrrcrce_substate = TDSRRCRCE_INITIAL;
                    tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
                    break;
                  }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

                  /* Release the RB 0 and request the RRC LLC to go to 
                     Idle Disconnected */
                  /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req ");    
                  if( TDSRRCRCE_SUCCESS == 
                     tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                     TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                  {
                    /* The RCE Procedure ends */
                    tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;  /* Change the RCE substate */

                    /* Set Abort Cause */
                    tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;

                  }  
                  else    /* Failed to send a internal command to RRC LLC.*/
                  {      
                    ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                  }  

                }  /* Max re-tries exceeded */
              }
              /* RRC Connection Setup is successfully validated */
              else if(tdsrrcrce_substate == TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC ConnSetUp msg has been validated");

                /* Update the required parameters */
                  
                  /* Initialize the Capability Update Requirement variable */
                tdsrrc_capability_update_requirement.
                  system_specific_cap_update_req_present = FALSE;
                
                tdsrrc_capability_update_requirement.
                  ue_radio_capability_update_requirement = FALSE;

                if (msg_version == TDSMSG_REL4)
                {
                  /* Update RCE Transaction Identifier */
                  /*lint -save -e613 */
                  tdsrrcrce_setup_transaction_identifier = msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier;
                  
                  /* Update RRC State Indicator */

                  tdsrrcrce_state_indicator = msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.r4.rrcConnectionSetup_r4.
                    rrc_StateIndicator;
                  /*lint -restore */

                  /* Save the RRC Capability Update Requirement to build RRC
                     CONNECTION SETUP COMPLETE message */

                  /*lint -save -e613 */
                  tdsrrcuece_copy_capability_update_requirement_r4(msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.r4.rrcConnectionSetup_r4.
                    capabilityUpdateRequirement);
                  /*lint -restore */
                }


                if (msg_version == TDSMSG_REL5)
                {
                  /* Update RCE Transaction Identifier */
                  /*lint -save -e613 */
                  tdsrrcrce_setup_transaction_identifier = msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier;
                  
                  /* Update RRC State Indicator */
                  
                  tdsrrcrce_state_indicator = msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.rrcConnectionSetup_r5.rrc_StateIndicator;
                  
                  /*lint -restore */
                  
                  /* Initialize the Capability Update Requirement variable */
                  
                  /* Save the RRC Capability Update Requirement to build RRC
                  CONNECTION SETUP COMPLETE message */
                  /*lint -save -e613 */
                  tdsrrcuece_copy_capability_update_requirement_later_than_r3(msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.rrcConnectionSetup_r5.capabilityUpdateRequirement);
                  /*lint -restore */

                }

                if ((msg_version == TDSMSG_REL6) && 
                    (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6))
                {
                  /* Update RCE Transaction Identifier */
                  /*lint -save -e613 */
                  tdsrrcrce_setup_transaction_identifier = msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier;
                  
                  /* Update RRC State Indicator */
                  
                  tdsrrcrce_state_indicator = msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6.rrc_StateIndicator;
                  
                  /*lint -restore */
                  
                  /* Save the RRC Capability Update Requirement to build RRC
                  CONNECTION SETUP COMPLETE message */
                  /*lint -save -e613 */
                  tdsrrcuece_copy_capability_update_requirement_later_than_r3(msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6.capabilityUpdateRequirement);
                  /*lint -restore */

                }

                if ((msg_version == TDSMSG_REL7) && 
                    (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7))
                {
                  /* Update RCE Transaction Identifier */
                  /*lint -save -e613 */
                  tdsrrcrce_setup_transaction_identifier = msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier;
                  
                  /* Update RRC State Indicator */
                  
                  tdsrrcrce_state_indicator = msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7.rrc_StateIndicator;
                  
                  /*lint -restore */
                  
                  /* Save the RRC Capability Update Requirement to build RRC
                  CONNECTION SETUP COMPLETE message */
                  /*lint -save -e613 */
                  tdsrrcuece_copy_capability_update_requirement_r7(&msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                    criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7.capabilityUpdateRequirement);
                  /*lint -restore */

                }


#ifdef FEATURE_TDSCDMA_REL8
                if ((msg_version == TDSMSG_REL8) && 
                    (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8))
                {
                  /* Update RCE Transaction Identifier */
                  /*lint -save -e613 */
                  tdsrrcrce_setup_transaction_identifier = msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier;
                  
                  /* Update RRC State Indicator */
                  
                  tdsrrcrce_state_indicator = msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8.rrc_StateIndicator;
                  
                  /*lint -restore */
                  
                  /* Save the RRC Capability Update Requirement to build RRC
                  CONNECTION SETUP COMPLETE message */
                  /*lint -save -e613 */
                  tdsrrcuece_copy_capability_update_requirement_r8(&msg_ptr->message.u.rrcConnectionSetup.u.
                  later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
                  criticalExtensions.u.r8.rrcConnectionSetup_r8.capabilityUpdateRequirement);
                  /*lint -restore */
                }

#endif /* FEATURE_TDSCDMA_REL8*/

#ifdef FEATURE_TDSCDMA_REL9
                if ((msg_version == TDSMSG_REL9) && 
                    (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9))
                {
                  /* Update RCE Transaction Identifier */
                  /*lint -save -e613 */
                  tdsrrcrce_setup_transaction_identifier = msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier;
                  
                  /* Update RRC State Indicator */
                  
                  tdsrrcrce_state_indicator = msg_ptr->message.u.
                    rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.rrcConnectionSetup_r9.rrc_StateIndicator;
                  
                  /*lint -restore */
                  
                  /* Save the RRC Capability Update Requirement to build RRC
                  CONNECTION SETUP COMPLETE message */
                  /*lint -save -e613 */
                  tdsrrcuece_copy_capability_update_requirement_r8(&msg_ptr->message.u.rrcConnectionSetup.u.
                  later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
                  criticalExtensions.u.criticalExtensions.u.r9.rrcConnectionSetup_r9.capabilityUpdateRequirement);
                  /*lint -restore */
                }
                
#endif /* FEATURE_TDSCDMA_REL9 */


                /* Update the next state */
                if(tdsrrcrce_state_indicator == tdsrrc_RRC_StateIndicator_cell_DCH)
                {
                  next_state = TDSRRC_STATE_CELL_DCH;
                }
                else if(tdsrrcrce_state_indicator == tdsrrc_RRC_StateIndicator_cell_FACH)
                {
                  next_state = TDSRRC_STATE_CELL_FACH;
                } 
                else
                {
                   /* This should never happen since RRC state is already validated */
                   ERR_FATAL("Invalid RRC state Indicator is received: %d",tdsrrcrce_state_indicator,0,0);
                }
                
                if( TDSRRCRCE_SUCCESS == 
                    tdsrrcrce_send_rrc_channel_config_req_for_srb(TDSESTABLISH_RB,
                                                               next_state,
                                                               TRUE) )  
                { 
                  tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_SRB_SETUP_CNF;     
                }    
                else  /* Failed to send a internal command to RRC LLC.*/
                {               
                  ERR_FATAL("Unable send internal cmd to LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
                } 

              } /* RRC Connection Setup is successfully validated */
              else
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"RCE waiting in sstate %d",tdsrrcrce_substate);
              }

            } /* ASN1 decoding is successful */
            break;     
 
          case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionReject: /* RRC CONNECTION REJECT is received */

            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Connection Reject Msg is received");
            /* Reset the tdsrrcrce_num_retries_count */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting tdsrrcrce_num_retries_count to 0");
            tdsrrcrce_num_retries_count =0;

#ifdef FEATURE_QSH_EVENT_METRIC
            TDSRRC_QSH_METRIC_UPDATE(TDSRRC_QSH_CONN_REJ);
#endif
            tdsrrcrce_allow_ta_flag = FALSE;

            /* Stop the timer T300 */
            tdsrrctmr_stop_timer(TDSRRCTMR_T_300_TIMER);
            if(tdsrrc_protocol_error_reject == TRUE) 
            /* ASN1 decoding resulted in PROTOCOL ERROR REJECT */
            {
#if(!defined(T_WINNT))
              /*log the protocol error*/
              tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                                      (uint8)TDSRRC_PROCEDURE_RCE,
                                      TDSRRCLOG_PROTOCOL_ERROR,
                                      TDSRRCLOG_ASN1_ENC_ERR
                                    );
#endif
              if( TDSRRCRCE_SUCCESS != 
                  tdsrrcrce_get_n300_from_serv_cell_db(&rrcrce_n_300))
              {
                ERR_FATAL("N300 value is invalid",0,0,0);
              }               
              if( tdsrrc_v_300 <= rrcrce_n_300)
              {            
                tdsrrc_protocol_error_indicator = tdsrrc_ProtocolErrorIndicator_errorOccurred;
                
                /* Re-send RRC CONNECTION REQUEST message over the air and 
                   this function restarts the Timer T300 */
                tdsrrcrce_send_rrc_connection_request_msg();
                /* RRC Connection Request Message is successfully sent */
                /* Increment the re-transmission counter for RRC Connection
                     Request message */
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);

                tdsrrc_v_300++;
              } 
              else   /* Reached maximum re-tries */
              { 
              
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"T300 Reached maximum re-tries");
#ifdef FEATURE_INTERRAT_PCCO_G2TD
                if(tdsrrccho_is_g2td_pcco_in_progress())
                {
                  tdsrrc_conn_setup_ind_type conn_setup_ind_params;
                  conn_setup_ind_params.status = FAILURE;
                  conn_setup_ind_params.conn_failure_reason = TDSRRC_MAX_RACH_ATTEMPTS;

                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Max RACH attempts reached when PCCO active.tdsrrcrce_num_retries_count = %d",tdsrrcrce_num_retries_count);
                  tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);
     
                  /* set rce_substate to initial */
                  tdsrrcrce_substate = TDSRRCRCE_INITIAL;
                  tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
                  break;
                }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

                /* Release the RB 0 and request the RRC LLC to go to Idle
                   disconnected */
                /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
                if( TDSRRCRCE_SUCCESS == 
                   tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                   TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                {                            
                  /* The RCE Procedure ends */
                  tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; 

                  /* Set Abort Cause */
                  tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
                }    
                else    /* Failed to send a internal command to RRC LLC.*/
                {      
                  ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                }          

              } 
            }  
            else  /* ASN1 decoding is successful */
            { 
              /* Update Transaction Id for RRC Connection Reject Msg */
              /*lint -save -e613 */
              tdsrrcrce_reject_transaction_identifier = msg_ptr->message.u.
                rrcConnectionReject.u.r3.rrcConnectionReject_r3.
                rrc_TransactionIdentifier;

              TDSRRC_MSG3(MSG_LEGACY_HIGH,"Wait Time is %d", msg_ptr->message.u.rrcConnectionReject.u.
                r3.rrcConnectionReject_r3.waitTime,0,0);

              /* Update the wait time */
              tdsrrcrce_wait_time = msg_ptr->message.u.rrcConnectionReject.u.r3.rrcConnectionReject_r3.waitTime;

              /* Validate Transaction Id and make sure critical externsions
                 are not selected */
              if( (tdsrrcrce_reject_transaction_identifier <= 3 ) &&
                   (msg_ptr->message.u.rrcConnectionReject.t ==
                   T_tdsrrc_RRCConnectionReject_r3 ) )
              {
                if(tdsrrcrce_wait_time == 0)
                {
#ifdef FEATURE_INTERRAT_PCCO_G2TD
                  if(tdsrrccho_is_g2td_pcco_in_progress())
                  {
                    tdsrrc_conn_setup_ind_type conn_setup_ind_params;
                    conn_setup_ind_params.status = FAILURE;
                    conn_setup_ind_params.conn_failure_reason = TDSRRC_CONNECTION_REJECT;

                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PCCO active.Connection Reject with wait time = 0");
                    tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  
     
                    /* set rce_substate to initial */
                    tdsrrcrce_substate = TDSRRCRCE_INITIAL;
                    tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
                    break;
                  }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Wt_tm=0. Send RELEASE_RB0; goto TDSRRC_STATE_DISCONNECTED");
                  
                  /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req ");    
                  if( TDSRRCRCE_SUCCESS == 
                     tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                     TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                  {                            
                    /* The RCE Procedure ends */
                    tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; 

                    /* Set Abort Cause appropriately. When the abort cause is 
                     * RRC_MAX_ACCESS_FAILURE then SR is not attempted. This has been
                     * done for GCF test case 6.1.2.6 (part 2).
                     */

/*
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting RRC->MM abort_cause to FAILURE_DUE_TO_CONG");
                    tdsrrcrce_mm_abort_cause = RRC_FAILURE_DUE_TO_CONGESTION;
*/
                    /*For TDS, 0NV control is required for for 6.1.2.6 
                     *Remove the SR as short term solution */
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting RRC->MM abort_cause to RRC_MAX_ACCESS_FAILURE");
                    tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;


                  }
                  else    /* Failed to send a internal command to RRC LLC.*/
                  {      
                    ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                  }   
                }
                else  /* Wait Time is non-zero */
                {
                  if( TDSRRCRCE_SUCCESS !=
                      tdsrrcrce_get_n300_from_serv_cell_db(&rrcrce_n_300))
                  {
                    ERR_FATAL("N300 value = %d is invalid",rrcrce_n_300,0,0);
                  }
                  if( tdsrrc_v_300 <= rrcrce_n_300)
                  {
                    /*Check whether redirection info is present */
                    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionReject.u.r3.
                           rrcConnectionReject_r3,tdsrrc_RRCConnectionReject_r3_IEs,redirectionInfo))
                    {
                      switch(msg_ptr->message.u.rrcConnectionReject.u.r3.
                             rrcConnectionReject_r3.redirectionInfo.t)
                      {
                        case T_tdsrrc_RedirectionInfo_frequencyInfo:   /* Redirection to new Freq */
                          if(msg_ptr->message.u.rrcConnectionReject.u.r3.
                             rrcConnectionReject_r3.redirectionInfo.u.frequencyInfo->
                             modeSpecificInfo.t ==
                                              T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
                          {
                            /* tdsrrc_validate_dl_arfcn->tdsrrc_validate_tds_frequency, old function also just hudge DL freq */
                            if(SUCCESS == tdsrrc_validate_tds_frequency(
                                              msg_ptr->message.u.rrcConnectionReject.u.r3.
                                              rrcConnectionReject_r3.redirectionInfo.u.frequencyInfo->
                                              modeSpecificInfo.u.tdd->uarfcn_Nt
                                              )
                              )
                            {
                              tdsrrcrce_rsp_to_rrc_conn_req = TDSRRCRCE_CONNECTION_REJECT;

                              /* Send the command to Cell Selection Procedure */
                              if(tdsrrcrce_send_redirection_req(msg_ptr, FALSE) == TDSRRCRCE_SUCCESS)          
                              {     
                                TDSRRC_MSG3(MSG_LEGACY_HIGH,"RCE: Inter-freq redirection req for freq %d to CSP", 
                                                    msg_ptr->message.u.rrcConnectionReject.u.r3.
                                                    rrcConnectionReject_r3.redirectionInfo.u.
                                                    frequencyInfo->modeSpecificInfo.u.tdd->uarfcn_Nt, 0, 0);
                                /* Wait for redirect confirm from Cell Selection Proc */
                                tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_REDIRECT_CNF;   
                              }  
                              else 
                              {                      
                                ERR_FATAL("Unable to post Internal Cmd to CSP: %d",TDSRRC_REDIRECT_REQ,0,0);
                              } 
                            }
                            /* Unsupported frequency received. Start wait timer */
                            else
                            {
                              TDSRRC_MSG3(MSG_LEGACY_HIGH,"iFreq Redirection rcvd with unsupported freq %d",
                                               msg_ptr->message.u.rrcConnectionReject.u.r3.
                                               rrcConnectionReject_r3.redirectionInfo.u.frequencyInfo->
                                               modeSpecificInfo.u.tdd->uarfcn_Nt,0,0);

                             tdsrrcrce_process_connection_rej_with_wait_time();
                            }
                          }
                          else
                          {
                            TDSRRC_MSG3(MSG_LEGACY_HIGH,"Received Unsupported Freq Info:%d in redirection",
                              msg_ptr->message.u.rrcConnectionReject.u.r3.
                              rrcConnectionReject_r3.redirectionInfo.u.frequencyInfo->
                              modeSpecificInfo.t,0,0);

                            /* Since this redirection is unsupported, abandon the procedure */
                            /* Release the RB 0 and request the RRC LLC to go to
                               Idle disconnected */
                            /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
                            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req ");    
                            if( TDSRRCRCE_SUCCESS == 
                                tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                                TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                            {                            
                              /* The RCE Procedure ends */
                              tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; 

                              /* Set Abort Cause */
                              tdsrrcrce_mm_abort_cause = RRC_UNSUPPORTED_REDIRECTION;
                            }    
                            else    /* Failed to send a internal command to RRC LLC.*/
                            {      
                              ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                            }    
                                                
                          }
                          break;

                      case T_tdsrrc_RedirectionInfo_interRATInfo: /* Redirection to new System */

 tdsrrcrce_handle_interrat_redirection_info(
                            msg_ptr
                                #ifdef FEATURE_TDSCDMA_TO_LTE
                                                    , FALSE
                                #endif
                            );
                          break;

                        default:  /* No redirection, resend after wait time expire */                      
                          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Received Unsupported redirection: %d",
                                   msg_ptr->message.u.rrcConnectionReject.u.r3.
                                   rrcConnectionReject_r3.redirectionInfo.t,
                                   0,0);

                          /* Since this redirection is unsupported, abandon the procedure */
                          /* Release the RB 0 and request the RRC LLC to go to
                             Idle disconnected */
                          /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
                          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
                          if( TDSRRCRCE_SUCCESS == 
                             tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                             TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                          {                            
                            /* The RCE Procedure ends */
                            tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; /* change the RCE substate */

                            /* Set Abort Cause */
                            tdsrrcrce_mm_abort_cause = RRC_UNSUPPORTED_REDIRECTION;  
                          }    
                          else    /* Failed to send a internal command to RRC LLC.*/
                          {      
                            ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                          }    
                              
                          break;
                      } /* Switch for redirection t */
                    }
#ifdef FEATURE_TDSCDMA_TO_LTE
                    else if((msg_ptr->message.u.rrcConnectionReject.u.r3.m.laterNonCriticalExtensionsPresent) &&
                              (msg_ptr->message.u.rrcConnectionReject.u.r3.laterNonCriticalExtensions.m.v690NonCriticalExtensionsPresent) &&
                              (msg_ptr->message.u.rrcConnectionReject.u.r3.laterNonCriticalExtensions.v690NonCriticalExtensions.m.v6f0NonCriticalExtensionsPresent) &&
                              (msg_ptr->message.u.rrcConnectionReject.u.r3.laterNonCriticalExtensions.v690NonCriticalExtensions.v6f0NonCriticalExtensions.m.v860NonCriticalExtensionsPresent) &&
                              (msg_ptr->message.u.rrcConnectionReject.u.r3.laterNonCriticalExtensions.v690NonCriticalExtensions.v6f0NonCriticalExtensions.
                                 v860NonCriticalExtensions.rrcConnectionReject_v860ext.m.redirectionInfoPresent) &&
                              (msg_ptr->message.u.rrcConnectionReject.u.r3.laterNonCriticalExtensions.v690NonCriticalExtensions.v6f0NonCriticalExtensions.
                                 v860NonCriticalExtensions.rrcConnectionReject_v860ext.redirectionInfo.t == T_tdsrrc_RedirectionInfo_v860ext_interRATInfo) &&
                              (msg_ptr->message.u.rrcConnectionReject.u.r3.laterNonCriticalExtensions.v690NonCriticalExtensions.v6f0NonCriticalExtensions.
                                 v860NonCriticalExtensions.rrcConnectionReject_v860ext.redirectionInfo.u.interRATInfo->t == T_tdsrrc_InterRATInfo_v860ext_eutra)
                            )
                    {
                      tdsrrcrce_handle_interrat_redirection_info(msg_ptr, TRUE);
                    }
#endif
                    else  /* Redirection info is not present */
                    {
                      tdsrrcrce_process_connection_rej_with_wait_time();
                    }
                  } /*if V300 <= N300 */
                  else   /* V300 > N300 Reached maximum re-tries */
                  {
#ifdef FEATURE_INTERRAT_PCCO_G2TD
                    /* If GTOTDS PCCO is active,sent a conn_setup_ind with failure */
                    if(tdsrrccho_is_g2td_pcco_in_progress())
                    {
                      tdsrrc_conn_setup_ind_type conn_setup_ind_params;
                      conn_setup_ind_params.status = FAILURE;
                      conn_setup_ind_params.conn_failure_reason = TDSRRC_MAX_RACH_ATTEMPTS;

                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Max RACH attempts though wait_timer non zero in Conn Reject.PCCO active");
                      tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);
                      
                      /* set rce_substate to initial */
                      tdsrrcrce_substate = TDSRRCRCE_INITIAL;
                      tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
                      break;
                    }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

                    /* Release the RB 0 and request the RRC LLC to go to Idle disconnected */
                    if( TDSRRCRCE_SUCCESS == 
                       tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                       TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                    {                            
                      /* The RCE Procedure ends */
                      tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;

                      /* Set Abort Cause */
                      tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;  
                    }    
                    else    /* Failed to send a internal command to RRC LLC.*/
                    {      
                      ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                    }   
                  }               
                }   /* Wait time is non-zero */
              } /* Valid Transaction Id  and critical extensions not selected */
              else /*Invalid Transaction Id is received */
              {
                 /* Set the Protocol Error Indicator to TRUE */
                 tdsrrc_protocol_error_indicator = tdsrrc_ProtocolErrorIndicator_errorOccurred;
              }
              /*lint -restore */
            } /* ASN1 decoding is successful */   
            break; 

          default:     
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid message is forwarded: %d",message_choice);
            break;
        } /* Switch for the message */

      }/* Checks for tdsrrc_DL_CCCH_Message PDU type */
      else /* Received other than tdsrrc_DL_CCCH_Message type */
      {
        if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type != tdsrrc_DL_CCCH_Message_PDU)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rcvd an invalid message type:%d",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
        }
        else /* Reselection is in progress */
        {
          /* Stop the timer T300 */
          tdsrrctmr_stop_timer(TDSRRCTMR_T_300_TIMER);
          /* Trash the message received on DL CCCH since reselection is in
             progress and wait for TDSRRC_NEW_CELL_IND from the CSP */
          tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RESELCT_COMPL;  
        }
        
      }
      break;

    default:     
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;
  }
}
/* tdsrrcrce_process_rce_wait_for_conn_setup_substate  */
/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_reselct_compl_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in TDSRRCRCE_WAIT_FOR_RESELCT_COMPL substate of 
  Connecting State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_reselct_compl_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to the received 
                                                command */
)
{
  rrc_abort_cause_e_type  cause = RRC_MAX_ACCESS_FAILURE;
  tdsrrc_N_300 rrcrce_n_300;                    /* Local Varibale to store 
                                                N300 */ 
  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
      
    case TDSRRC_STATE_CHANGE_IND:
      if((cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED) &&
         ( tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
      {
        /* Stop the timer for Physical Channel Timer */     
        tdsrrctmr_stop_timer(TDSRRCTMR_PHY_CHAN_FAILURE_TIMER);  

        /* Send RRC_ABORT_IND command to the MM */
        if(TDSRRCRCE_SUCCESS == 
           tdsrrcrce_send_rrc_abort_ind(RRC_FAILURE_UNSPECIFIED) )      
        {        
           /* Reset RCE Procedure and  Procedure ends */
           tdsrrcrce_substate = TDSRRCRCE_INITIAL; 
           tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        }    
        else
        {     
          ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_IND,0,0);
        }      

      }
      else
      {
        ERR_FATAL("RRCSCMGR posted an unregistered event",cmd_ptr->cmd.state_change_ind.new_state ,0,0);
      }
      break;

    case TDSRRC_ABORT_REQ: 

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ rcvd in %d substate",tdsrrcrce_substate);
      
      if (tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED)
      {
        /* Send RRC_ABORT_CNF command to the MM */
        if(TDSRRCRCE_SUCCESS == 
          tdsrrcrce_send_rrc_abort_cnf() )      
        {        
        /* At this time, none of the RBs are setup. Simply reset the RCE
          substate */
          
          tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          break;
        }
        
      }
      /* Stop the timer for Physical Channel Timer */     
      tdsrrctmr_stop_timer(TDSRRCTMR_PHY_CHAN_FAILURE_TIMER);  

      /* Sends TDSRRC_CHANNEL_CONFIG_REQ command to RRC LLC to release RB 0*/
     /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
      if( TDSRRCRCE_SUCCESS == 
         tdsrrcrce_send_rrc_initiate_cell_selection_ind(
         TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
      {                            
        /* Wait for successful release of RB0 */
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; //RRCRCR_WAIT_FOR_SRB_REL_CNF; 

        tdsrrcrce_send_abort_cnf = TRUE;
      
      }    
      else    /* Failed to send a internal command to RRC LLC.*/
      {      
        ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
      }
      
      break;

    case TDSRRC_NEW_CELL_IND: 
      
      if (tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED)
      {
        break;
      }

      /* Stop the timer for Physical Channel Timer */     
      tdsrrctmr_stop_timer(TDSRRCTMR_PHY_CHAN_FAILURE_TIMER);  

      /* The UE is already in CONNECTING State. The CCM should have reconfigured
          the  RB 0*/
      /* Gets N300 from the SIB 1 database */
      if( TDSRRCRCE_SUCCESS != 
          tdsrrcrce_get_n300_from_serv_cell_db(&rrcrce_n_300))
      {
        ERR_FATAL("N300 value is invalid",0,0,0);
      }
      if((TDSRRCRCE_SUCCESS == tdsrrcrce_validate_cell_access_status
                       (tdsrrc_est_cn_domain_id,tdsrrc_establishment_cause,&cause)) &&
         (tdsrrc_v_300 <= rrcrce_n_300))
      {        
        /* Resend RRC connection Request Message and this function restarts the
           T300 Timer */
        tdsrrcrce_send_rrc_connection_request_msg();
        /* RRC Connection Request Message is successfully sent */
        /* Increments the re-transmission counter for RRC Connection Request
             Message */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);

        /* The RCE procedure waits for the RRC Connection Setup Message */
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP;

        tdsrrc_v_300++;     
      }
      else /* Reached maximum allowed re-tries */
      { 
        tdsrrcrce_num_retries_count++;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"tdsrrcrce_num_retries_count = %d",tdsrrcrce_num_retries_count);

        /* Release the RB 0 and request the RRC LLC to go to Idle disconnected */
        /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req ");    
        if( TDSRRCRCE_SUCCESS == 
         tdsrrcrce_send_rrc_initiate_cell_selection_ind(
         TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
        {                            
          /* The RCE Procedure ends */
          tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; /* change the RCE substate */ 

          /* Set Abort Cause */
          if(cause  ==  RRC_ACCESS_CLASS_NOT_ALLOWED)
          {
            if(tdsrrc_est_cn_domain_id == RRC_CS_DOMAIN_CN_ID)
            {
              tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_CS_DOMAIN;
            }
            else
            {
              tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_PS_DOMAIN;
            }
          }
          else
          {
            tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
          }

        }    
        else    /* Failed to send a internal command to RRC LLC.*/
        {      
        ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
        }   
      }
      break; 
             
    case TDSRRC_DOWNLINK_SDU_IND:

      /* RRC Connection Setup or RRC Connection Reject may be received in this
         Substate because of cell-reselections. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Simultanous transactions not supported");
      break;
      
   
    case TDSRRC_PHY_CHAN_FAILURE_TIMER_EXPIRED_IND:

      /* 10 second timer has expired so take UE to idle */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PHY_CHAN_FAILURE TIMER EXPIRED. Taking UE To IDLE : ");
      
      /* The agreement is that CSP would tear down RB0 and RCE
         should not send channel config req to tear down RB0 */
      if( SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCE, 
                                                                 TDSRRC_TX_TO_DISCON_OTHER) )
      {
        /* Update the release cause */
        tdsrrc_release_cause = RRC_REL_DEEP_FADE;

        /* RRC_ABORT_IND will be sent tdsrrcrce_process_rce_wait_for_rb0_rel_cnf_substate 
        upon a state change indication */ 
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;
        tdsrrcrce_mm_abort_cause = RRC_FAILURE_UNSPECIFIED;

      }
      else
      {
        ERR_FATAL("Failed to go to Disconnected state",0,0,0);
      }
      break;

    default:     
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;
  }
}
/* tdsrrcrce_process_rce_wait_for_reselct_compl_substate  */


/*===========================================================================

FUNCTION tdsrrcrce_validate_and_handle_acc_class_restr_on_redirec_succ

DESCRIPTION
  This function validates the access class restriction on redirection success and initiates cell selection if
  access is not allowed.

DEPENDENCIES
  None

RETURN VALUE
  uecomdef_status_e_type
  SUCCESS if access class validation succeeds 
  FAILURE otherwise

SIDE EFFECTS
  None
===========================================================================*/
static uecomdef_status_e_type tdsrrcrce_validate_and_handle_acc_class_restr_on_redirec_succ
(
  void
)
{
  uecomdef_status_e_type status = SUCCESS;
  rrc_abort_cause_e_type  cause = RRC_MAX_ACCESS_FAILURE;

  if(TDSRRCRCE_SUCCESS != tdsrrcrce_validate_cell_access_status
                             (tdsrrc_est_cn_domain_id,tdsrrc_establishment_cause,&cause))
  {
    /*Access is not allowed on redirected frequency*/
#ifdef FEATURE_INTERRAT_PCCO_G2TD
    if(tdsrrccho_is_g2td_pcco_in_progress())
    {
      tdsrrc_conn_setup_ind_type conn_setup_ind_params;
      conn_setup_ind_params.status = FAILURE;
      conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Access not allowed when PCCO is active.tdsrrcrce_substate %d",tdsrrcrce_substate);      
      tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  

      /* set rce_substate to initial */
      tdsrrcrce_substate = TDSRRCRCE_INITIAL;
      tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
    }
    else
#endif /* FEATURE_INTERRAT_PCCO_G2TD */
    {
      /*
         Initiate Cell Selection Indication and wait for State change indication. 
         On state change indication send ABORT_IND to NAS with cause ACCESS CLASS not allowed
      */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP");    
      if( TDSRRCRCE_SUCCESS == 
           tdsrrcrce_send_rrc_initiate_cell_selection_ind(
           TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
      {
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;  /* Change the RCE substate */
        if(tdsrrc_est_cn_domain_id == RRC_CS_DOMAIN_CN_ID)
        {
          tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_CS_DOMAIN;
        }
        else
        {
          tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_PS_DOMAIN;
        }
      }
      else    /* Failed to send a internal command to RRC CSP.*/
      {      
        ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
      }  
    }
    status = FAILURE;
  }
  return status;
}

/*===========================================================================

FUNCTION  tdsrrcwrm_update_lock_after_redir_cnf

DESCRIPTION

  This function change or requests WRM for lock after redire cnf
  establishment.
    
DEPENDENCIES

  None.
 
RETURN VALUE

 FALSE - Acquire lock failure 

SIDE EFFECTS

  None.

===========================================================================*/
static boolean tdsrrcwrm_update_lock_after_redir_cnf(void)
{
  trm_reason_enum_t priority = tdsrrcwrm_proc_map_priority();
  /*If RRC is already holding the Lock, don't need to acquire lock again*/
  if(tdrm_check_cli_state_is_in_lock(TDRM_CLIENT_RRC))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Lock is already in RRC.");
    tdsrrc_wrm_change_priority(priority,FALSE);
    tdrm_retain_lock(TDRM_CLIENT_RRC);
  }
  else if(tdsrrcwrm_acquire_lock_for_est_req(tdsrrc_est_cn_domain_id,0xfffffff,TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_EST_REQ_IN_MS) == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");		
    if( TDSRRCRCE_SUCCESS == 
          tdsrrcrce_send_rrc_initiate_cell_selection_ind(TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
    {
      /* The RCE Procedure ends */
      tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;	/* Change the RCE substate */
		
      tdsrrcrce_mm_abort_cause = RRC_NO_RESOURCE_AVAILABLE;
    }  
    else		/* Failed to send a internal command to RRC LLC.*/
    { 		 
      ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
    } 

    return FALSE;
  }

  return TRUE;
}


/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_redirect_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in TDSRRCRCE_WAIT_FOR_REDIRECT_CNF substate of 
  Connecting State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_redirect_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to the received 
                                                command */
)
{
  tdsrrc_N_300 rrcrce_n_300;                    /* Local Varibale to store N300 */ 


  tdsrrc_cmd_type *rce_out_cmd_ptr;
  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL99;

  rrc_abort_cause_e_type  cause = RRC_MAX_ACCESS_FAILURE;

  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    
    case TDSRRC_DOWNLINK_SDU_IND:
      /* RRC Connection Setup or RRC Connection Reject may be received in this
         Substate because of cell-reselections. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Simultanous transactions not supported");
      break;
    
    case TDSRRC_STATE_CHANGE_IND:
      if((cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED) &&
         ( tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
      {
#ifdef FEATURE_INTERRAT_PCCO_G2TD
        if(tdsrrccho_is_g2td_pcco_in_progress())
        {
          tdsrrc_conn_setup_ind_type conn_setup_ind_params;
          conn_setup_ind_params.status = FAILURE;
          conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;

          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_STATE_CHANGE_IND received when PCCO is active.tdsrrcrce_substate %d",tdsrrcrce_substate);      
          tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  
       
          /* set rce_substate to initial */
          tdsrrcrce_substate = TDSRRCRCE_INITIAL;
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          break;
        }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

        /* Send RRC_ABORT_IND command to the MM */
        if(TDSRRCRCE_SUCCESS == 
           tdsrrcrce_send_rrc_abort_ind(RRC_FAILURE_UNSPECIFIED) )      
        {        
           /* Reset RCE Procedure and  Procedure ends */
           tdsrrcrce_substate = TDSRRCRCE_INITIAL; 
           tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        }    
        else
        {     
          ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_IND,0,0);
        }    
      }
      else
      {
        ERR_FATAL("RRCSCMGR posted an unregistered event %d",cmd_ptr->cmd.state_change_ind.new_state,0,0);
      }
      break;
    
    case TDSRRC_ABORT_REQ:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"ABORT_REQ rcvd in Redirect Wt Cnf. Send tx to disconnected to CSP");

      /* The agreement is that CSP would tear down RB0 and RCE
         should not send channel config req to tear down RB0 */
      if( SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCE, 
                                                                 TDSRRC_TX_TO_DISCON_OTHER) )
      {
        /* Update the release cause */
        tdsrrc_release_cause = RRC_REL_DEEP_FADE;
      
        tdsrrcrce_send_abort_cnf = TRUE;
      
        /* RRC_ABORT_CNF will be sent tdsrrcrce_process_rce_wait_for_rb0_rel_cnf_substate 
         * upon a state change indication */ 
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;
        tdsrrcrce_mm_abort_cause = RRC_FAILURE_UNSPECIFIED;
      }
      else
      {
        ERR_FATAL("Failed to go to Disconnected state",0,0,0);
      }
      break;

    case TDSRRC_REDIRECT_CNF:

      /* Redirection succeeded */
      if(cmd_ptr->cmd.redirect_cnf.status == TDSRRC_REDIRECT_SUCCESS)
      { 
        switch(cmd_ptr->cmd.redirect_cnf.cause)
        {
          case TDSRRC_CAMPED_ON_REDIRECTED_FREQ:

            /* Redirection success for inter-freq redirection via RRC Connection Reject */
            if(tdsrrcrce_rsp_to_rrc_conn_req == TDSRRCRCE_CONNECTION_REJECT)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE rcvd TDSRRC_REDIRECT_SUCCESS for Connection REJECT. Reset v300");

              /* Reset V300 counter */
              tdsrrc_v_300 = 0;
              /*Validate the access class on the redirected frequency, if access is allowed then send connection request*/
              if(SUCCESS == tdsrrcrce_validate_and_handle_acc_class_restr_on_redirec_succ())
              {
                if(FALSE == tdsrrcwrm_update_lock_after_redir_cnf())
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Update lock failure after redir_cnf");
                  break;
                }

                /* Resend RRC Connection Request message and this function
                             restarts the T300 Timer*/
                tdsrrcrce_send_rrc_connection_request_msg();
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);
                tdsrrc_v_300++;
                tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP;
              }
            }
            /* Redirection success for inter-freq redirection via RRC Connection Setup */
            else if(tdsrrcrce_rsp_to_rrc_conn_req == TDSRRCRCE_CONNECTION_SETUP)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE rcvd TDSRRC_REDIRECT_SUCCESS for Freq redirection in Conn SETUP msg");
              /*Validate the access class on the redirected frequency, if access is allowed then send connection request*/
              if(SUCCESS == tdsrrcrce_validate_and_handle_acc_class_restr_on_redirec_succ())
              {
              /* As per Spec, V300 counter is not reset */
              /* Compare new N300 with V300 and send RRC Connection Request accordingly */
              if(TDSRRCRCE_SUCCESS != tdsrrcrce_get_n300_from_serv_cell_db(&rrcrce_n_300))
              {
                ERR_FATAL("N300 value is invalid",rrcrce_n_300,0,0);
              }                    
              else if(tdsrrc_v_300 <= rrcrce_n_300)
              {
                if(FALSE == tdsrrcwrm_update_lock_after_redir_cnf())
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Update lock failure after redir_cnf");
                  break;
                }
              
                TDSRRC_MSG2(MSG_LEGACY_HIGH,"v300 (%d) <= n300 (%d). Send conn req again",tdsrrc_v_300,rrcrce_n_300);

                /* Resend RRC Connection Request message and this function
                   restarts the T300 Timer*/
                tdsrrcrce_send_rrc_connection_request_msg();
                /* RRC Connection Request Message is successfully sent */
                /* Increment the re-transmission counter for RRC Connection
                     Request message */
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);
                tdsrrc_v_300++;
                tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP;
              }
              else
              {
                TDSRRC_MSG2(MSG_LEGACY_HIGH,"v300 (%d) > n300 (%d) on the new cell. Go to idle",tdsrrc_v_300,rrcrce_n_300);    

                /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req %d");    
                if( TDSRRCRCE_SUCCESS == 
                   tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                   TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                {
                  /* The RCE Procedure ends */
                  tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;  /* Change the RCE substate */

                  tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
                }  
                else    /* Failed to send a internal command to RRC LLC.*/
                {      
                  ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                } 
              }
            }
            }
            
            /* Incorrect value for tdsrrcrce_rsp_to_rrc_conn_req - which should never be the case
             * as the var tdsrrcrce_rsp_to_rrc_conn_req is always set before sending redirection
             * request to CSP.
             */
            else
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR:RCE rcvd REDIRECT_SUCCESS when tdsrrcrce_rsp_to_rrc_conn_req %d. Trigger error recovery.",tdsrrcrce_rsp_to_rrc_conn_req);
#ifdef FEATURE_INTERRAT_PCCO_G2TD
              if(tdsrrccho_is_g2td_pcco_in_progress())
              {
                tdsrrc_conn_setup_ind_type conn_setup_ind_params;
                conn_setup_ind_params.status = FAILURE;
                conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;

                TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_STATE_CHANGE_IND received when PCCO is active.tdsrrcrce_substate %d",tdsrrcrce_substate);

                tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);

                /* set rce_substate to initial */
                tdsrrcrce_substate = TDSRRCRCE_INITIAL;
                tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
                break;
              }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

              /* Release RB 0 and request RRC LLC to go to Idle Disconnected */
              /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
              if( TDSRRCRCE_SUCCESS == 
                   tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                   TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
              {
                /* The RCE Procedure ends */
                tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;  /* Change the RCE substate */

                tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
              }  
              else    /* Failed to send a internal command to RRC LLC.*/
              {      
                ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
              } 
            }
            break;                                           

          case TDSRRC_CAMPED_ON_REDIRECTED_CELL:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE rcvd TDSRRC_REDIRECT_SUCCESS for CPID redirection in Conn SETUP msg");

            /* Redirection success for inter-freq redirection via RRC Connection Setup */
            if(tdsrrcrce_rsp_to_rrc_conn_req == TDSRRCRCE_CONNECTION_SETUP)
            {
              /* Decode the saved message */
              tdsrrcrce_save_dl_ccch_msg.decoded_msg = 
                     ((tdsrrc_DL_CCCH_Message *) tdsrrcasn1_decode_pdu((void *)tdsrrcrce_save_dl_ccch_msg.dl_sdu,
                                                                 (int)tdsrrcrce_save_dl_ccch_msg.dl_sdu_type,
                                                                 tdsrrcrce_save_dl_ccch_msg.dl_sdu_length
                                                                 ));
              /* If failed to decode then go to idle */
              if(tdsrrcrce_save_dl_ccch_msg.decoded_msg == NULL)
              {
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: failed to decode the pdu. Tx to discon");

                /* The agreement is that CSP would tear down RB0 and RCE
                   should not send channel config req to tear down RB0 */
                if( SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCE, 
                                                                           TDSRRC_TX_TO_DISCON_OTHER) )
                {
                  /* Update the release cause */
                  tdsrrc_release_cause = RRC_REL_CAUSE_UNSPEC;

                  /* RRC_ABORT_IND will be sent tdsrrcrce_process_rce_wait_for_rb0_rel_cnf_substate 
                   * upon a state change indication */ 
                  tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;
                  tdsrrcrce_mm_abort_cause = RRC_FAIL_TO_CAMP_ON;
                }
                else
                {
                  ERR_FATAL("Failed to go to Disconnected state",0,0,0);
                }
              }
              /* PDU was successfully decoded */
              else
              {
                /* Validate the RRC Connection Setup message */
                if(TDSRRCRCE_SUCCESS != tdsrrcrce_validate_rrc_connection_setup_msg(
                                                tdsrrcrce_save_dl_ccch_msg.decoded_msg,
                                                tdsrrcrce_save_dl_ccch_msg.dl_sdu_type, 
                                                &msg_version))
                {
                  TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: After CPID Redirection Connection Setup Message cld not be validated");
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"protocol error rej has been set to true");

                  /* Unsuppored IE is received from the UTRAN */
                  tdsrrc_protocol_error_reject = TRUE;
                  tdsrrc_protocol_error_indicator = tdsrrc_ProtocolErrorIndicator_errorOccurred;

                  /* Send RRC Connection Request again if V300 <= N 300 */
                  if( TDSRRCRCE_SUCCESS != tdsrrcrce_get_n300_from_serv_cell_db(&rrcrce_n_300))
                  {
                    ERR_FATAL("N300 value is invalid",0,0,0);
                  }
                  if(tdsrrc_v_300 <= rrcrce_n_300)
                  {                       
                    if(FALSE == tdsrrcwrm_update_lock_after_redir_cnf())
                    {
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Update lock failure after redir_cnf");
                      break;
                    }
                    tdsrrcrce_send_rrc_connection_request_msg();
                    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);
                    tdsrrc_v_300++;
                  }
                  /* Max re-tries exceeded */
                  else       
                  {
                    /* Release RB 0 and request RRC LLC to go to Idle Disconnected */
                    /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req %d");    
                    if( TDSRRCRCE_SUCCESS == 
                        tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                        TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                    {
                      /* The RCE Procedure ends */
                      tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;  /* Change the RCE substate */

                      tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
                    }  
                    else    /* Failed to send a internal command to RRC LLC.*/
                    {      
                      ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                    } 
                  }  /* Max re-tries exceeded */
                }
                /* RRC Connection Setup is successfully validated */
                else
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC ConnSetUp msg has been validated");

                  /* Reset V300 counter */
                  tdsrrc_v_300 = 0;

                  /* Update the required parameters */
                  if (msg_version == TDSMSG_REL4)
                  {
                    /* Update RCE Transaction Identifier */
                    /*lint -save -e613 */
                    tdsrrcrce_setup_transaction_identifier = tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier;

                    /* Update RRC State Indicator */
                    tdsrrcrce_state_indicator = tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.rrcConnectionSetup.u.
                          later_than_r3.criticalExtensions.u.r4.rrcConnectionSetup_r4.rrc_StateIndicator;
                    /*lint -restore */

                    /* Initialize the Capability Update Requirement variable */
                    tdsrrc_capability_update_requirement.system_specific_cap_update_req_present = FALSE;
                    tdsrrc_capability_update_requirement.ue_radio_capability_update_requirement = FALSE;

                    /* Save the RRC Capability Update Requirement to build RRC
                       CONNECTION SETUP COMPLETE message */
                    /*lint -save -e613 */
                    tdsrrcuece_copy_capability_update_requirement_r4(tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.r4.rrcConnectionSetup_r4.
                      capabilityUpdateRequirement);
                    /*lint -restore */
                  }

                  if (msg_version == TDSMSG_REL5)
                  {
                    /* Update RCE Transaction Identifier */
                    /*lint -save -e613 */
                    tdsrrcrce_setup_transaction_identifier = tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier;

                    /* Update RRC State Indicator */
                    tdsrrcrce_state_indicator = tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                      criticalExtensions.u.r5.rrcConnectionSetup_r5.rrc_StateIndicator;

                    /*lint -restore */                  
                    /* Initialize the Capability Update Requirement variable */
                    tdsrrc_capability_update_requirement.system_specific_cap_update_req_present = FALSE;
                    tdsrrc_capability_update_requirement.ue_radio_capability_update_requirement = FALSE;

                    /* Save the RRC Capability Update Requirement to build RRC
                    CONNECTION SETUP COMPLETE message */
                    /*lint -save -e613 */
                    tdsrrcuece_copy_capability_update_requirement_later_than_r3(tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                      criticalExtensions.u.r5.rrcConnectionSetup_r5.capabilityUpdateRequirement);
                    /*lint -restore */
                  }
                  if ((msg_version == TDSMSG_REL6)&& 
                    (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6))
                  {
                    /* Update RCE Transaction Identifier */
                    /*lint -save -e613 */
                    tdsrrcrce_setup_transaction_identifier = tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier;

                    /* Update RRC State Indicator */
                    tdsrrcrce_state_indicator = tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.criticalExtensions.u. criticalExtensions.u.
                      criticalExtensions.u.r6.rrcConnectionSetup_r6.rrc_StateIndicator;

                    /*lint -restore */                  
                    /* Initialize the Capability Update Requirement variable */
                    tdsrrc_capability_update_requirement.system_specific_cap_update_req_present = FALSE;
                    tdsrrc_capability_update_requirement.ue_radio_capability_update_requirement = FALSE;

                    /* Save the RRC Capability Update Requirement to build RRC
                    CONNECTION SETUP COMPLETE message */
                    /*lint -save -e613 */
                    tdsrrcuece_copy_capability_update_requirement_later_than_r3(tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                      criticalExtensions.u.r6.rrcConnectionSetup_r6.capabilityUpdateRequirement);
                    /*lint -restore */
                  }
                  if ((msg_version == TDSMSG_REL7)&& 
                    (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7))
                  {
                    /* Update RCE Transaction Identifier */
                    /*lint -save -e613 */
                    tdsrrcrce_setup_transaction_identifier = tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier;

                    /* Update RRC State Indicator */
                    tdsrrcrce_state_indicator = tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.criticalExtensions.u. criticalExtensions.u.
                      criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7.rrc_StateIndicator;

                    /*lint -restore */                  
                    /* Initialize the Capability Update Requirement variable */
                    tdsrrc_capability_update_requirement.system_specific_cap_update_req_present = FALSE;
                    tdsrrc_capability_update_requirement.ue_radio_capability_update_requirement = FALSE;

                    /* Save the RRC Capability Update Requirement to build RRC
                    CONNECTION SETUP COMPLETE message */
                    /*lint -save -e613 */
                    tdsrrcuece_copy_capability_update_requirement_r7(&tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                      criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7.capabilityUpdateRequirement);
                    /*lint -restore */
                  }
#ifdef FEATURE_TDSCDMA_REL8
                  if ((msg_version == TDSMSG_REL8)&& 
                    (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8))
                  {
                    /* Update RCE Transaction Identifier */
                    /*lint -save -e613 */
                    tdsrrcrce_setup_transaction_identifier = tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier;

                    /* Update RRC State Indicator */
                    tdsrrcrce_state_indicator = tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.criticalExtensions.u. criticalExtensions.u.
                      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8.rrc_StateIndicator;

                    /*lint -restore */                  
                    /* Initialize the Capability Update Requirement variable */
                    tdsrrc_capability_update_requirement.system_specific_cap_update_req_present = FALSE;
                    tdsrrc_capability_update_requirement.ue_radio_capability_update_requirement = FALSE;

                    /* Save the RRC Capability Update Requirement to build RRC
                    CONNECTION SETUP COMPLETE message */
                    /*lint -save -e613 */
                    tdsrrcuece_copy_capability_update_requirement_r8(&tdsrrcrce_save_dl_ccch_msg.decoded_msg->message.u.
                      rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8.capabilityUpdateRequirement);
                    /*lint -restore */
                  }
#endif /*FEATURE_TDSCDMA_REL8*/
                  if( TDSRRCRCE_SUCCESS == 
                      tdsrrcrce_send_rrc_channel_config_req_for_srb(TDSESTABLISH_RB, TDSRRC_STATE_CELL_FACH, TRUE) )  
                  { 
                    tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_SRB_SETUP_CNF;     
                  }    
                  else  /* Failed to send a internal command to RRC LLC.*/
                  {               
                    ERR_FATAL("Unable send internal cmd to LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
                  } 
                } /* RRC Conn Setup Successfully Validated */   
              } /* PDU successfully decoded */
            }
            /* Incorrect value for tdsrrcrce_rsp_to_rrc_conn_req - which should never be the case
             * as the var tdsrrcrce_rsp_to_rrc_conn_req is always set before sending redirection
             * request to CSP.
             */
            else
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR:RCE rcvd REDIRECT_SUCCESS for CPID when tdsrrcrce_rsp_to_rrc_conn_req %d. Recover",tdsrrcrce_rsp_to_rrc_conn_req);
              /* Release RB 0 and request RRC LLC to go to Idle Disconnected */
              /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
                if( TDSRRCRCE_SUCCESS == 
                   tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                   TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                {
                  /* The RCE Procedure ends */
                  tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;  /* Change the RCE substate */

                  tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
                }  
                else    /* Failed to send a internal command to RRC LLC.*/
                {      
                  ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                } 
            }
            break;

          default:
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Redirection success rcvd by RCE w/ incorrect cause %d",cmd_ptr->cmd.redirect_cnf.cause);
            break;
        }
      }
      /* Redirection has failed */
      else 
      {
        switch(cmd_ptr->cmd.redirect_cnf.cause)
        {
          /* UE camped, but not on redirected frequency */
          case TDSRRC_CAMPED_ON_DIFFERENT_FREQ:

            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RCE rcvd RDRCT_FAILURE cause %d. Checking N300 for the new cell",cmd_ptr->cmd.redirect_cnf.cause);

            if( TDSRRCRCE_SUCCESS !=
                tdsrrcrce_get_n300_from_serv_cell_db(&rrcrce_n_300))
            {
              ERR_FATAL("N300 value is invalid",rrcrce_n_300,0,0);
            }


            if((TDSRRCRCE_SUCCESS == tdsrrcrce_validate_cell_access_status
                           (tdsrrc_est_cn_domain_id,tdsrrc_establishment_cause,&cause))&&
               (tdsrrc_v_300 <= rrcrce_n_300))
            {
              if(FALSE == tdsrrcwrm_update_lock_after_redir_cnf())
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Update lock failure after redir_cnf");
                break;
              }

              TDSRRC_MSG2(MSG_LEGACY_HIGH,"v300 (%d) <= n300 (%d). Send conn req again",tdsrrc_v_300,rrcrce_n_300);

              /* Resend RRC Connection Request message and this function
                 restarts the T300 Timer*/
              tdsrrcrce_send_rrc_connection_request_msg();
              /* RRC Connection Request Message is successfully sent */
              /* Increment the re-transmission counter for RRC Connection
                   Request message */
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);
              tdsrrc_v_300++;
              tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP;
            }
            else
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"v300 (%d) > n300 (%d) on the new cell. Go to idle",tdsrrc_v_300,rrcrce_n_300);    

#ifdef FEATURE_INTERRAT_PCCO_G2TD
              if(tdsrrccho_is_g2td_pcco_in_progress())
              {
                tdsrrc_conn_setup_ind_type conn_setup_ind_params;
                conn_setup_ind_params.status = FAILURE;
                conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;

                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Max Retries when PCCO is active.tdsrrcrce_substate %d",tdsrrcrce_substate);      
                tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  
       
                /* set rce_substate to initial */
                tdsrrcrce_substate = TDSRRCRCE_INITIAL;
                tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
                break;
              }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

              /* Release RB 0 and request RRC LLC to go to Idle Disconnected */
              /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
              if( TDSRRCRCE_SUCCESS == 
                   tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                   TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
              {
                /* The RCE Procedure ends */
                tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;  /* Change the RCE substate */

                if(cause  ==  RRC_ACCESS_CLASS_NOT_ALLOWED)
                {
                  if(tdsrrc_est_cn_domain_id == RRC_CS_DOMAIN_CN_ID)
                  {
                    tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_CS_DOMAIN;
                  }
                  else
                  {
                    tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_PS_DOMAIN;
                  }
                }
                else
                {
                  tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
                }
              }  
              else    /* Failed to send a internal command to RRC LLC.*/
              {      
                ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
              } 
            }
            break;


          case TDSRRC_CAMPED_ON_ORIGINAL_FREQ:

            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RCE rcvd RDRCT_FAILURE cause %d. Sending RRC Conn Req again",cmd_ptr->cmd.redirect_cnf.cause);

            if(TDSRRCRCE_SUCCESS != tdsrrcrce_validate_cell_access_status
                      (tdsrrc_est_cn_domain_id,tdsrrc_establishment_cause,&cause))
            {
              /* Release RB 0 and request RRC LLC to go to Idle Disconnected */
              /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
              if( TDSRRCRCE_SUCCESS == 
                   tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                   TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
              {
                /* The RCE Procedure ends */
                tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;  /* Change the RCE substate */

                if(cause  ==  RRC_ACCESS_CLASS_NOT_ALLOWED)
                {
                  if(tdsrrc_est_cn_domain_id == RRC_CS_DOMAIN_CN_ID)
                  {
                    tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_CS_DOMAIN;
                  }
                  else
                  {
                    tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_PS_DOMAIN;
                  }
                }
                else
                {
                  tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
                }
              }
              else    /* Failed to send a internal command to RRC LLC.*/
              {
                ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
              }
            }
            else
            {
              if(FALSE == tdsrrcwrm_update_lock_after_redir_cnf())
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Update lock failure after redir_cnf");
                break;
              }
              /* Resend RRC Connection Request message and this function
                 restarts the T300 Timer*/
              tdsrrcrce_send_rrc_connection_request_msg();
              /* RRC Connection Request Message is successfully sent */
              /* Increment the re-transmission counter for RRC Connection
                   Request message */
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);
              tdsrrc_v_300++;
              tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP;
            }
            break;


          /* UE failed to camp */
          case TDSRRC_FAILED_TO_CAMP:

            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RCE rcvd RDRCT_FAILURE cause %d. Taking UE To IDLE.",cmd_ptr->cmd.redirect_cnf.cause);
#ifdef FEATURE_INTERRAT_PCCO_G2TD
            if(tdsrrccho_is_g2td_pcco_in_progress())
            {
              tdsrrc_conn_setup_ind_type conn_setup_ind_params;
              conn_setup_ind_params.status = FAILURE;
              conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;

              TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_FAILED_TO_CAMP received when PCCO is active.tdsrrcrce_substate %d",tdsrrcrce_substate);      
              tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  

              /* set rce_substate to initial */
              tdsrrcrce_substate = TDSRRCRCE_INITIAL;
              tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
              break;
            }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

            /* The agreement is that CSP would tear down RB0 and RCE
               should not send channel config req to tear down RB0 */
            if( SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCE, 
                                                                       TDSRRC_TX_TO_DISCON_OTHER) )
            {
              /* Update the release cause */
              tdsrrc_release_cause = RRC_REL_CAUSE_UNSPEC;
      
              /* RRC_ABORT_IND will be sent tdsrrcrce_process_rce_wait_for_rb0_rel_cnf_substate 
               * upon a state change indication */ 
              tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;
              tdsrrcrce_mm_abort_cause = RRC_FAIL_TO_CAMP_ON;
            }
            else
            {
              ERR_FATAL("Failed to go to Disconnected state",0,0,0);
            }
            break;


          /* CSP did not receive inter frequency redirect request in connecting state
           * or RCE sent wrong parameters to CSP or redirection is not supported. 
           * In this case RCE should start wait timer.
           */
          case TDSRRC_RCE_INCOMPATIBLE:
          case TDSRRC_RAT_REDIRECTION_NOT_SUPPORTED:
          case TDSRRC_FREQ_REDIRECTION_NOT_SUPPORTED:
          case TDSRRC_REDIRECTION_NOT_SUPPORTED:                    
            
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"RCE rcvd RDRCT_FAILURE cause %d. Strtng wt time %d",cmd_ptr->cmd.redirect_cnf.cause,tdsrrcrce_wait_time * 1000);

            tdsrrcrce_process_connection_rej_with_wait_time();
            break;

          case TDSRRC_REDIRECT_GSM_FAILURE:
            /* Send Channel config request to LLC asking it to re-setup
             * FACH channels
             */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Redirect cnf (GSM_FAILURE) from CSP");

            /* Allocates the buffer to RRC Internal command */
            if((rce_out_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
            {     
              /* Fill in the command parameters for TDSRRC_CHANNEL_CONFIG_REQ */
              rce_out_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;
              rce_out_cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RCE;
              rce_out_cmd_ptr->cmd.chan_config_req.transaction_id = TDSGET_TRANS_ID();
              tdsrrcrce_chl_config_transaction_id = (uint8) rce_out_cmd_ptr->cmd.chan_config_req.transaction_id;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Assigned transaction id = %d",tdsrrcrce_chl_config_transaction_id);

              /* Fill in parameters for TDSRRC_CHANNEL_CONFIG_REQ command */
              rce_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
              rce_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_CONNECTING;
              rce_out_cmd_ptr->cmd.chan_config_req.chan_config_reason 
                                                      = TDSRRCLLC_CHAN_CFG_REASON_PHYCHAN_RESETUP;
              rce_out_cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = 
                TRUE;

              rce_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 0;
              rce_out_cmd_ptr->cmd.chan_config_req.release_phychls  = FALSE;
              rce_out_cmd_ptr->cmd.chan_config_req.release_trchls   = FALSE;

              tdsrrc_put_int_cmd(rce_out_cmd_ptr);         
            }
            else  /* RRC Task failed to allocate memory */
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unable to allocate memory for cmd: %d",TDSRRC_CHANNEL_CONFIG_REQ);
            }

            tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_PHYCHAN_RESETUP_CNF;
            break;

          case TDSRRC_REDIRECT_RESUME_FAILURE:
            /* Trigger OOS handling in Connecting */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Redirect cnf (RESUME_FAILURE) from CSP");

            /* Trigger Cell Selection */
            if(TDSRRCRCE_SUCCESS ==
               tdsrrcrce_send_rrc_initiate_cell_selection_ind(
               TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDS resume failure");
            }
            else
            {
              ERR_FATAL("Failed to post int cmd: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
            }
            break;

          /* CSP sent wrong failure cause to RCE. Send abort indication to MM */
          default:

            TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR REDIRECT_FAILURE rcvd with incorrect cause %d",cmd_ptr->cmd.redirect_cnf.cause);

#ifdef FEATURE_INTERRAT_PCCO_G2TD
            if(tdsrrccho_is_g2td_pcco_in_progress())
            {
              tdsrrc_conn_setup_ind_type conn_setup_ind_params;
              conn_setup_ind_params.status = FAILURE;
              conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;

              TDSRRC_MSG1(MSG_LEGACY_HIGH,"REDIRECT_FAIL received when PCCO is active.tdsrrcrce_substate %d",tdsrrcrce_substate);      
              tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  

              /* set rce_substate to initial */
              tdsrrcrce_substate = TDSRRCRCE_INITIAL;
              tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
              break;
            }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

            /* The agreement is that CSP would tear down RB0 and RCE
             * should not send channel config req to tear down RB0 
             */
            if( SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCE, 
                                                                       TDSRRC_TX_TO_DISCON_OTHER) )
            {
              /* Update the release cause */
              tdsrrc_release_cause = RRC_REL_CAUSE_UNSPEC;
      
              /* RRC_ABORT_IND will be sent tdsrrcrce_process_rce_wait_for_rb0_rel_cnf_substate 
               * upon a state change indication */ 
              tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;
              tdsrrcrce_mm_abort_cause = RRC_FAILURE_UNSPECIFIED;
            }
            else
            {
              ERR_FATAL("Failed to go to Disconnected state",0,0,0);
            }
            break;
        }
      }
          
      break;

    case TDSRRC_NEW_CELL_IND:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring New Cell Ind received in TDSRRCRCE_WAIT_FOR_REDIRECT_CNF");
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;
  }  /* End Switch Statement */


  /* Since RCE receives NEW_CELL_IND before REDIRECT_CNF,
   * memory should not be freed in this case (if memory
   * was allocated)
   */
  if(tdsrrcrce_substate != TDSRRCRCE_WAIT_FOR_REDIRECT_CNF)
  {
    /* Free the memory allocated to the encoded string */
    if(tdsrrcrce_save_dl_ccch_msg.dl_sdu != NULL)
    {
      tdsrrc_free(tdsrrcrce_save_dl_ccch_msg.dl_sdu);
    }

    /* Free the memory allocated to the decoded message */
    if(tdsrrcrce_save_dl_ccch_msg.decoded_msg != NULL)
    {
      if(tdsrrcasn1_free_pdu((int)tdsrrcrce_save_dl_ccch_msg.dl_sdu_type, tdsrrcrce_save_dl_ccch_msg.decoded_msg))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Unable to free pdu for %d",tdsrrcrce_save_dl_ccch_msg.dl_sdu_type);
      }
    }
  }
}/* tdsrrcrce_process_rce_wait_for_redirect_cnf_substate*/

/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_wait_time_expire_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in TDSRRCRCE_WAIT_FOR_WAIT_TIMER_TO_EXPIRE substate of 
  Connecting State
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_wait_time_expire_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  rrc_abort_cause_e_type  cause = RRC_MAX_ACCESS_FAILURE;

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {   
    case TDSRRC_DOWNLINK_SDU_IND:
      /* RRC Connection Setup or RRC Connection Reject may be received in this
         Substate because of cell-reselections. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Simultanous transactions not supported");
      break;

    case TDSRRC_STATE_CHANGE_IND:
      /* Stop the timer for wait time */     
       tdsrrctmr_stop_timer(TDSRRCTMR_WAIT_TIME_TIMER);  
   
      if((cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED) &&
         ( tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
      {
 #ifdef FEATURE_INTERRAT_PCCO_G2TD
        if(tdsrrccho_is_g2td_pcco_in_progress())
        {
          tdsrrc_conn_setup_ind_type conn_setup_ind_params;
          conn_setup_ind_params.status = FAILURE;
          conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;

          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_STATE_CHANGE_IND received when PCCO is active.tdsrrcrce_substate %d",tdsrrcrce_substate);      
          tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  
       
          /* set rce_substate to initial */
          tdsrrcrce_substate = TDSRRCRCE_INITIAL;
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          break;
        }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

        /* Send RRC_ABORT_IND command to the MM */
        if(TDSRRCRCE_SUCCESS == 
           tdsrrcrce_send_rrc_abort_ind(RRC_FAILURE_UNSPECIFIED) )      
        {        
           /* Reset RCE Procedure and  Procedure ends */
           tdsrrcrce_substate = TDSRRCRCE_INITIAL; 
           tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        }    
        else
        {     
          ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_IND,0,0);
        }    
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCSCMGR posted an unregistered event");
      }
      break;

    case TDSRRC_ABORT_REQ:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ rcvd in %d substate",tdsrrcrce_substate);
      /* Stop the timer for wait time */     
      tdsrrctmr_stop_timer(TDSRRCTMR_WAIT_TIME_TIMER); 
    
      /* The agreement is that CSP would tear down RB0 and RCE
         should not send channel config req to tear down RB0 */
      if( SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCE, 
                                                                 TDSRRC_TX_TO_DISCON_OTHER) )
      {
        /* Update the release cause */
        tdsrrc_release_cause = RRC_REL_DEEP_FADE;
      
        tdsrrcrce_send_abort_cnf = TRUE;
                                   
        /* RRC_ABORT_CNF will be sent tdsrrcrce_process_rce_wait_for_rb0_rel_cnf_substate 
         * upon a state change indication */ 
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;
        tdsrrcrce_mm_abort_cause = RRC_FAILURE_UNSPECIFIED;
      }
      else
      {
        ERR_FATAL("Failed to go to Disconnected state",0,0,0);
      }
      break;

    case TDSRRC_WAIT_TIMER_EXPIRED_IND:
      /* Do not request lock to send RRCConn while CSP is waiting for response/abort_cnf for GRR due to
       * 1. Request lock may cause GSM cannot get lock for T2G resel
       * 2. After getting lock and resel rej/abort_cnf back, RRC will exchange lock which will put lock to exchange client
       *    lead to RRC lock request fail later
       */
      if (((tdsrrccsp_get_csp_substate() == TDSRRCCSP_WT_RESPONSE_FROM_RR) || (tdsrrccsp_get_csp_substate() == TDSRRCCSP_WT_RR_ABORT_CNF))
          && (tdsrrccsp_get_curr_scan() == RRC_CSP_TDS_TO_GSM_CELL_RESELECTION_SCAN)
         )
      {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Wait timer timeout while T2G reselection ongoing,start wait timer again");    
         tdsrrctmr_start_timer(TDSRRCTMR_WAIT_TIME_TIMER, TDSRRCTMR_WAIT_TIMER_FOR_T2G_RESEL_IN_MS); 
      }   
      else
      {
        if(tdsrrcwrm_acquire_lock_for_est_req(tdsrrc_est_cn_domain_id,0xfffffff,TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_EST_REQ_IN_MS) == FAILURE)
        {
          if( TDSRRCRCE_SUCCESS == 
              tdsrrcrce_send_rrc_initiate_cell_selection_ind(
              TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
          {
            /* The RCE Procedure ends */
            tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;  /* Change the RCE substate */

            if(cause  ==  RRC_ACCESS_CLASS_NOT_ALLOWED)
            {
              tdsrrcrce_mm_abort_cause = RRC_ACCESS_CLASS_NOT_ALLOWED;
            }
            else
            {
              tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
            }
          }
          else    /* Failed to send a internal command to RRC LLC.*/
          {
            ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
          }
          break;
        }
        if(TDSRRCRCE_SUCCESS != tdsrrcrce_validate_cell_access_status
                  (tdsrrc_est_cn_domain_id,tdsrrc_establishment_cause,&cause))
        {
          /* Release RB 0 and request RRC LLC to go to Idle Disconnected */
          /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req ");    
          if( TDSRRCRCE_SUCCESS == 
          tdsrrcrce_send_rrc_initiate_cell_selection_ind(
             TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
          {
            /* The RCE Procedure ends */
            tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF;  /* Change the RCE substate */

            if(cause  ==  RRC_ACCESS_CLASS_NOT_ALLOWED)
            {
              if(tdsrrc_est_cn_domain_id == RRC_CS_DOMAIN_CN_ID)
              {
                tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_CS_DOMAIN;
              }
              else
              {
                tdsrrcrce_mm_abort_cause = RRC_AC_NOT_ALLOWED_PS_DOMAIN;
              }
            }
            else
            {
              tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;
            }
          }
          else    /* Failed to send a internal command to RRC LLC.*/
          {
            ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
          }
        }
        else
        {
          /* Resend RRC connection Request Message and this function restarts the
             T300 Timer */
          tdsrrcrce_send_rrc_connection_request_msg();
          tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP; 
          /* Increments the re-transmission counter for RRC Connection 
               Request message */ 
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);
  
          tdsrrc_v_300++;     
        }
      }
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;
  }  
} /*  tdsrrcrce_process_rce_wait_for_wait_time_expire_substate */
/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_channel_config_req_for_srb

DESCRIPTION
  This function builds TDSRRC_CHANNEL_CONFIG_REQ command and  requests RRC LLC
  to either setup or modify or release  Signalling Radio Bearers 1 through 4.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it successfully sends 
  TDSRRC_CHANNEL_CONFIG_REQ command to the RRC LLC. Otherwise it returns
  TDSRRCRCE_FAILURE_MAJOR

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_channel_config_req_for_srb
(
               
  tdsrrc_rb_cfg_e_type action,                   /* Type of action on SRBs */
  tdsrrc_state_e_type next_state,                /* Next state to transition is
                                                 expected to take place */
  boolean confirm_required                    /* This indiactes whether
                                                 TDSRRC_CHANNEL_CONFIG_CNF is 
                                                 required or not */
)
{
  tdsrrc_cmd_type *cmd_ptr;                     /* Pointer to the RRC Command */

  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MAJOR; 
                                             /* The status is initialized to
                                                Failure */  
  uint32 count = 0;                          /* Local variable for count */
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrc_RB_Identity                rb_id;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

  /* Allocates the buffer to RRC Internal command */
  if( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {     
    /* Fill in the command parameters for TDSRRC_CHANNEL_CONFIG_REQ */
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;
    cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RCE;
    cmd_ptr->cmd.chan_config_req.transaction_id = TDSGET_TRANS_ID();
    tdsrrcrce_chl_config_transaction_id = (uint8) cmd_ptr->cmd.chan_config_req.transaction_id;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Assigned transaction id = %d",tdsrrcrce_chl_config_transaction_id);

    /* Fill in parameters for TDSRRC_CHANNEL_CONFIG_REQ command */
    cmd_ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
    cmd_ptr->cmd.chan_config_req.next_state = next_state;
    /* The tdsrrcrce_validate_rrc_connection_setup_msg function would
       have captured if number of rbs is equal other than 3 or 4 */
    cmd_ptr->cmd.chan_config_req.num_rb_to_config = (int32 ) 
      tdsrrcrce_number_of_rbs;


    cmd_ptr->cmd.chan_config_req.rb[count].rb_id = DCCH_UM_RADIO_BEARER_ID;
    cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = action;

    cmd_ptr->cmd.chan_config_req.rb[count].rb_id = DCCH_AM_RADIO_BEARER_ID;
    cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = action;

    cmd_ptr->cmd.chan_config_req.rb[count].rb_id = DCCH_DT_HIGH_PRI_RADIO_BEARER_ID;
    cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = action;

    cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = 
      confirm_required;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"No. of RBs in Chan config Req = %d",tdsrrcrce_number_of_rbs);
    
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    if(tdsordered_config_ptr->srb5_info.srb5_rb_exists  == TRUE)
    {
      if(tdsrrcrce_number_of_rbs == 5)
      {
        cmd_ptr->cmd.chan_config_req.rb[count].rb_id = tdsordered_config_ptr->srb5_info.rb_id;
        cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = action;
  
        cmd_ptr->cmd.chan_config_req.rb[count].rb_id = DCCH_DT_LOW_PRI_RADIO_BEARER_ID;
        cmd_ptr->cmd.chan_config_req.rb[count].rb_config = action;
      }
      else
      {
        cmd_ptr->cmd.chan_config_req.rb[count].rb_id = tdsordered_config_ptr->srb5_info.rb_id;
        cmd_ptr->cmd.chan_config_req.rb[count].rb_config = action;
      }
    }
    else 
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
    {
      if (tdsrrcrce_number_of_rbs == 4)
      {
        cmd_ptr->cmd.chan_config_req.rb[count].rb_id = DCCH_DT_LOW_PRI_RADIO_BEARER_ID;
        cmd_ptr->cmd.chan_config_req.rb[count].rb_config = action;
      }
    }
 
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    if(tdsrrclcm_get_rb_id_if_dl_lc_type_exists(UE_LOGCHAN_TM_DCCH_DL, &rb_id) == TDSRRCLCM_SUCCESS)
    {
      /* Though this a generic function for RCE, RCE can only
         release SRB#5 only during NAS abort cases */
      if(action == TDSRELEASE_RB)
      {
        cmd_ptr->cmd.chan_config_req.rb[++count].rb_id = rb_id;
        cmd_ptr->cmd.chan_config_req.rb[count].rb_config = action;
        cmd_ptr->cmd.chan_config_req.num_rb_to_config++;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RCE trying to do other than releasing SRB#5 ... something wrong");
        /* Anyway continue */
      }
    }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
 
    tdsrrc_put_int_cmd(cmd_ptr);                 /* sends the command to RRC
                                                 LLC. */
    status = TDSRRCRCE_SUCCESS;                  /* Status is initialized to
                                                 Success */           
  }
  else  /* RRC Task failed to allocate memory */
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Memory alloc failed for Internal Cmd:%d",TDSRRC_CHANNEL_CONFIG_REQ);
  }
  return (status);
}/*tdsrrcrce_send_rrc_channel_config_req_for_srb */
/*===========================================================================

FUNCTION tdsrrcrce_send_cell_update_initiate_req

DESCRIPTION
  This function send the cell update initiate request if UE does not have
  a valid C-RNTI in CELL_FACH state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcrce_send_cell_update_initiate_req
(
  tdsrrc_proc_e_type procedure
)
{
  tdsrrc_cmd_type *int_cmd_ptr;
  //This initiates a CELL Update procedure and enters the substate
  //RRCRCE_WAIT_CU_COMPLETE_CNF. 
  if( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {     
    /* Fill in the command parameters for TDSRRC_CELL_UPDATE_INITIATE_REQ */
    int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
    int_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_CELL_RESELECTION;
    int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = TRUE;
    int_cmd_ptr->cmd.initiate_cell_update.procedure = procedure;
        
    tdsrrc_put_int_cmd(int_cmd_ptr);             
  }
  else
  {
    ERR_FATAL("Unable to allocate memmory for: %d cmd",TDSRRC_CELL_UPDATE_INITIATE_REQ,0,0);
  }
}/*tdsrrcrce_send_cell_update_initiate_req*/
/*===========================================================================
FUNCTION tdsrrcrce_process_rce_wait_for_srb_setup_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in TDSRRCRCE_WAIT_FOR_SRB_SETUP_CNF sub state of 
  Connecting State
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_srb_setup_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{ 
 
  tdsrrc_N_300 rrcrce_n_300;                    /* Local Variable to store N300 */

  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_DOWNLINK_SDU_IND:
      /* RRC Connection Setup or RRC Connection Reject may be received in this
         Substate because of cell-reselections. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Simultanous transactions not supported");
      break;

    case TDSRRC_STATE_CHANGE_IND:
      if((cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED) &&
         ( tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
      {
        /* Send RRC_ABORT_IND command to the MM */
        if(TDSRRCRCE_SUCCESS == 
           tdsrrcrce_send_rrc_abort_ind(RRC_FAILURE_UNSPECIFIED) )      
        {        
           /* Reset RCE Procedure and  Procedure ends */
           tdsrrcrce_substate = TDSRRCRCE_INITIAL; 
           tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        }    
        else
        {     
          ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_IND,0,0);
        }    
      }
      else
      {
        ERR_FATAL("RRCSCMGR posted an unregistered event",cmd_ptr->cmd.state_change_ind.new_state,0,0);
      }
      break;

    case TDSRRC_ABORT_REQ:

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ rcvd in %d substate",tdsrrcrce_substate);
      /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req %d");    
      if( TDSRRCRCE_SUCCESS ==
          tdsrrcrce_send_rrc_initiate_cell_selection_ind(
         TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
      {     
        /* Wait for successful release of SRBs*/
        tdsrrcrce_substate = TDSRRCRCR_WAIT_FOR_SRB_REL_CNF;
        tdsrrcrce_send_abort_cnf = TRUE;
        tdsrrcrce_chl_config_transaction_id = 0;
      }    
      else   /* Failed to send a internal command to RRC LLC.*/
      {      
        ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
      } 
      break;

    case TDSRRC_NEW_CELL_IND:          
      /* The UE is already in CONNECTING State. The CCM should have reconfigured
          the  RB 0*/
      /* Gets N300 from the SIB 1 database */
      if( TDSRRCRCE_SUCCESS != 
          tdsrrcrce_get_n300_from_serv_cell_db(&rrcrce_n_300))
      {
        ERR_FATAL("N300 value is invalid",0,0,0);
      }
      if(tdsrrc_v_300 <= rrcrce_n_300)               
      {
        /* Resend RRC connection Request Message and this function restarts the
           T300 Timer */
        tdsrrcrce_send_rrc_connection_request_msg();
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP; 
        /* Increments the re-transmission counter for RRC Connection Request
             Message */ 
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);

        tdsrrc_v_300++;     
      }
      else /* Reached maximum allowed re-tries */
      { 
        /* Send RRC_ABORT_IND command to MM */
        if(TDSRRCRCE_SUCCESS == 
           tdsrrcrce_send_rrc_abort_ind(RRC_MAX_ACCESS_FAILURE) )      
        {
          /* The RCE Procedure ends */
          tdsrrcrce_substate = TDSRRCRCE_INITIAL;         /* reset the RCE substate */
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        }    
        else
        {  
          ERR_FATAL("Unable to post MM Cmd: %d",RRC_ABORT_IND,0,0);
        } 
      }     
      break;

    case TDSRRC_CHANNEL_CONFIG_CNF:
      if(cmd_ptr->cmd.chan_config_cnf.procedure == TDSRRC_PROCEDURE_RCE)
      {

        if(cmd_ptr->cmd.chan_config_cnf.transaction_id == tdsrrcrce_chl_config_transaction_id)
        {
          if( TDSRRCLLC_CHAN_CFG_SUCCESS == cmd_ptr->cmd.chan_config_cnf.chan_cfg_status ) 
          { 
            /* Copy the Ordered RB Mapping into the current RB-Mapping database*/
            tdsrrcllc_copy_ordered_hanging_rb_mapping_to_current();

    

            switch(tdsrrc_translate_state_indicator_to_state(tdsrrcrce_state_indicator))           
            {
              case TDSRRC_STATE_CELL_FACH:
                /* Query for valid C-RNTI */
                if (TDSC_RNTI_NOT_VALID == tdsrrcllc_get_current_crnti_status() )
                {
                  /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
                  tdsrrcrce_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RCE);
                  /* Wait for CELL UPDATE to get a valid C_RNTI */
                  tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_CU_COMPLETE_CNF;
                }
                else
                {
                  /* Send RRC Connection Setup Complete message */
                  if (TDSRRCRCE_SUCCESS != 
                    tdsrrcrce_send_rrc_connection_setup_complete_message())
                  {
                    ERR_FATAL("Unable to send RRC Connection Complete Msg",0,0,0);
                  }
                  else
                  {
                       /* Start the timer for L2 ACK */
                      /* tdsrrctmr_start_timer(TDSRRCTMR_RCE_L2ACK_TIMER, 
                      TDSRRCRCE_TIMER_VALUE_FOR_L2_ACK); */
  
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Conn Setup Compl Msg is sent"); 

                    tdsrrcllc_print_trans_chnl_rate();
                    /* Clear the Ordered Config */
                    tdsrrcllc_clear_ordered_config();
  
                    /* Update the necesary parameters here*/
                    tdsrrcsmc_reset_smc_ip_started_flag();
  
                    tdsrrccu_start_t_305_timer();

                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Delay sending RRC_EST_CNF to MM");
                    tdsrrcrce_substate = TDSRRCRCE_FINAL; /*We still go to final state, unless RCE needs process more command when waiting for L2ACK*/
                  }
                }
                break;

              case TDSRRC_STATE_CELL_DCH:
            
                /*Notify the Cell_Id to registered entities*/
                tdsrrc_notify_cell_id_in_dch();                    
                /* Send RRC Connection Setup Complete message */
                if (TDSRRCRCE_SUCCESS != 
                  tdsrrcrce_send_rrc_connection_setup_complete_message())
                {
                  ERR_FATAL("Unable to send RRC Connection Complete Msg",0,0,0);
                }
                else
                {
                /* Start the timer for L2 ACK 
                tdsrrctmr_start_timer(TDSRRCTMR_RCE_L2ACK_TIMER, 
                  TDSRRCRCE_TIMER_VALUE_FOR_L2_ACK); */

                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Conn Setup Compl Msg is sent");    
                  
                  tdsrrcllc_print_trans_chnl_rate();
                  /* Clear the Ordered Config */
                  tdsrrcllc_clear_ordered_config();

                  /* Update the necesary parameters here*/
                  tdsrrcsmc_reset_smc_ip_started_flag();

                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Delay sending RRC_EST_CNF to MM");
                  tdsrrcrce_substate = TDSRRCRCE_FINAL; /*We still go to final state, unless RCE needs process more command when waiting for L2ACK*/
                } 
                break;

              default:
                ERR_FATAL(" RRC entered into invalid state :%d",tdsrrcrce_state_indicator,0,0);
                break;
            }

          }
          else /* Physical Channel Setup Failure , Retry again */
          {  
            /* Channel config failure, so initialise the ordered RB-Mapping database*/
            tdsrrcllc_init_ordered_hanging_rb_mapping_info();

            (void)tdsrrcrce_get_n300_from_serv_cell_db(&rrcrce_n_300);
            if(tdsrrc_v_300 <= rrcrce_n_300)
            {  

              if (tdsrrcrce_state_indicator == tdsrrc_RRC_StateIndicator_cell_DCH && tdsrrc_get_state() == TDSRRC_STATE_CONNECTING)
              { 
                // ~~
                /* reset the tdsordered_config */
                if (TDSOC_NOT_SET != tdsrrcllc_reset_ordered_config(TDSRRC_PROCEDURE_RCE
                                                              ,TRUE
                                                              ))
                {
                  TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not Reset OC");
                  /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
                  if( TDSRRCRCE_SUCCESS ==
                      tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                      TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
                  { 
                    /* Change the RCE state machine */         
                    tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; 
                    /* Set Abort Cause */
                    tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;     
                  }    
                  else   /* Failed to send a internal command to RRC LLC.*/
                  {      
                    ERR_FATAL("Unable send internal cmd to CSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
                  } 
                }
                else
                {
                  /* start phy chan failure timer */
                  tdsrrctmr_start_timer(TDSRRCTMR_PHY_CHAN_FAILURE_TIMER, TDSRRCTMR_PHYCHANFAILURE_TIMER_IN_MS);     
                  /* Initiate Cell Selection and substate should be TDSRRCRCE_WAIT_FOR_RESELCT_COMPL */
                  if (TDSRRCRCE_SUCCESS== 
                      tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                      TDSRRC_OUT_OF_SERVICE_AREA_IND,tdsrrc_get_state()) )
                  {
                    tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RESELCT_COMPL;
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PhyChanSetupFail rcvd.Initiating cell sel");
                  }
                  else
                  {
                    ERR_FATAL("Unable to send cell selection Req Msg",0,0,0);
                  }
                }
              }
              else
              {
                tdsrrcrce_send_rrc_connection_request_msg();
                tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP; 
                /* Increments the re-transmission counter for RRC Connection
                   Request Message */
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Connection Request is sent: %d",tdsrrc_v_300);

                tdsrrc_v_300++;
              }
            }
            else  /* Max re-tries exceeded */
            {
              /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req %d");    
              if( TDSRRCRCE_SUCCESS ==
                  tdsrrcrce_send_rrc_initiate_cell_selection_ind(
                  TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
              { 
                /* Change the RCE state machine */         
                tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; 

                /* Set Abort Cause */
                tdsrrcrce_mm_abort_cause = RRC_MAX_ACCESS_FAILURE;     
              }    
              else   /* Failed to send a internal command to RRC LLC.*/
              {      
                ERR_FATAL("Unable send internal cmd to CSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
              } 
            }
          } /* Physical Channel Setup Failure */
        }
        else
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring tr_id %d in chl config cnf. Expected tr_id = %d",cmd_ptr->cmd.chan_config_cnf.transaction_id,tdsrrcrce_chl_config_transaction_id);
        }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_CHANNEL_CONFIG_CNF's proc: %d",cmd_ptr->cmd.chan_config_cnf.procedure);
      }
      break;
   
    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;

   }  
} /* tdsrrcrce_process_rce_wait_for_srb_setup_cnf_substate*/

/*===========================================================================
FUNCTION tdsrrcrce_process_rce_wait_for_cu_complete_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in RRCRCE_WAIT__FOR_CU_COMPLETE_CNF sub state of 
  CELL_FACH State
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_cu_complete_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{ 
  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_DOWNLINK_SDU_IND:
      /* RRC Connection Setup or RRC Connection Reject may be received in this
         Substate because of cell-reselections. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Simultanous transactions not supported");
      break;

    case TDSRRC_STATE_CHANGE_IND:
      if((cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED) &&
         ( tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
      {
        /* Send RRC_ABORT_IND command to the MM */
        if(TDSRRCRCE_SUCCESS == 
           tdsrrcrce_send_rrc_abort_ind(RRC_FAILURE_UNSPECIFIED) )      
        {        
           /* Reset RCE Procedure and  Procedure ends */
           tdsrrcrce_substate = TDSRRCRCE_INITIAL; 
           tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        }    
        else
        {     
          ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_IND,0,0);
        }    
      }
      else
      {
        ERR_FATAL("RRCSCMGR posted an unregistered event",cmd_ptr->cmd.state_change_ind.new_state,0,0);
      }
      break;

    case TDSRRC_ABORT_REQ:

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ rcvd in %d substate",tdsrrcrce_substate);
      /* Send command to LLC to release SRBs */
      /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
      if( TDSRRCRCE_SUCCESS ==
         tdsrrcrce_send_rrc_initiate_cell_selection_ind(
         TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
      {     
        
        /* Wait for successful release of SRBs */
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; //RRCRCR_WAIT_FOR_SRB_REL_CNF;    

        tdsrrcrce_send_abort_cnf = TRUE;
      
      }    
      else   /* Failed to send a internal command to RRC LLC.*/
      {      
        ERR_FATAL("Unable send internal cmd to CSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
      } 
      break;

    case TDSRRC_NEW_CELL_IND:          
      /* The UE is already in CELL_FACH State. So Ignore the command since Cell
         Update would have been initiated */
      TDSRRC_MSG2(MSG_LEGACY_MED,"Unexpected event:%d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate); 
      break;

    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      if(cmd_ptr->cmd.cu_complete_cnf.procedure == TDSRRC_PROCEDURE_RCE)
      {
        if (TDSRRCRCE_SUCCESS != 
          tdsrrcrce_send_rrc_connection_setup_complete_message())
        {
          ERR_FATAL("Unable to send RRC Connection Complete Msg",0,0,0);
        }
        else
        {
        /* Start the timer for L2 ACK 
        tdsrrctmr_start_timer(TDSRRCTMR_RCE_L2ACK_TIMER, 
          TDSRRCRCE_TIMER_VALUE_FOR_L2_ACK); */
          
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Conn Setup Compl Msg is sent");    
 
          tdsrrcllc_print_trans_chnl_rate();
          /* Clear the Ordered Config */
          tdsrrcllc_clear_ordered_config();
            
          /* Update the necesary parameters here*/
          tdsrrcsmc_reset_smc_ip_started_flag();
            
          tdsrrccu_start_t_305_timer(); 

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Delay sending RRC_EST_CNF to MM");
          tdsrrcrce_substate = TDSRRCRCE_FINAL; /*We still go to final state, unless RCE needs process more command when waiting for L2ACK*/ 
        } 
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_CELL_UPDATE_COMPLETE_CNF's proc: %d",cmd_ptr->cmd.cu_complete_cnf.procedure);
      }
      
      break;
   
    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;

   }  
} /* tdsrrcrce_process_rce_wait_for_cu_complete_cnf_substate*/

/*===========================================================================
FUNCTION tdsrrcrce_send_rrc_connection_setup_complete_message

DESCRIPTION
  This function builds the RRC CONNECTION SETUP COMPLETE message and calls
  the Send Chain to transmit the message.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type : It returns TDSRRCRCE_SUCCESS if it successfully builds 
  the RRC CONNECTION SETUP COMPLETE message and calls the Send Chain. 
  Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_connection_setup_complete_message
(
  void
)
{
  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MAJOR;
  tdsrrc_UL_DCCH_Message *msg_ptr;             /* Pointer to uplink DCCH message*/
  uint32 num_rat=0;
  rlc_lc_id_type ul_lc_id;                  /* Logical Channel Id */
  boolean ret_flag = FALSE;
  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL5; 

  boolean support_ho_to_gan = FALSE;

#ifdef FEATURE_TDSCDMA_TX_DIVERSITY_DL_FOR_MIMO
  boolean support_tx_diversity_mimo = FALSE;
#endif /*FEATURE_TDSCDMA_TX_DIVERSITY_DL_FOR_MIMO*/

#ifdef FEATURE_TDSCDMA_TX_DIVERSITY_DL_FOR_MIMO
  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
    support_tx_diversity_mimo = TRUE;
  }
#endif

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
    msg_version = TDSMSG_REL5;
  }
  else
  {
    msg_version = TDSMSG_REL4;
  }

  /* To satisfy Compiler/LINT to not generate warning */
  /*lint -e774*/
  if ((msg_version == TDSMSG_REL5) || (ret_flag == TRUE))
  {
    TDSRRC_MSG2(MSG_LEGACY_LOW,"Msg version is %d,Ret Flag %d",msg_version,ret_flag);
  }
  /*lint +e774*/

  /* Checks whether Logical Channel entity setup for DCCH logical
     Channel type */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH,
                                         DCCH_AM_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA);

  /* If RLC Logical Channel Id is already setup, the send RRC Connection
     Request message through SEND_CHAIN */
  if(ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    /* Allocate a buffer for an uplink DCCH Message. This buffer should be released
       by SEND_CHAIN using OSS compiler functions */
    msg_ptr = (tdsrrc_UL_DCCH_Message *) tdsrrc_malloc ( 
                                               sizeof(struct tdsrrc_UL_DCCH_Message));
    if (msg_ptr == NULL)
    {
      /* Lint will (Warning) 527: Unreachable. But changing ERR_FATAL
      to ERR would result in losing the line no in which this
      happened. So retain ERR_FATAL */
      ERR_FATAL("Malloc for RRC Conn Setup Comp Msg failed",0,0,0);
    }

    /* Initialize contents of Message Pointer */
    memset((void *)msg_ptr, 0, sizeof(tdsrrc_UL_DCCH_Message));

    /* Select the message as RRC CONNECTION SETUP COMPLETE message */
    msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_rrcConnectionSetupComplete;

    /* set the Transaction id */
    msg_ptr->message.u.rrcConnectionSetupComplete.rrc_TransactionIdentifier =
      tdsrrcrce_setup_transaction_identifier;
   
    /* Appends START list to the RRC CONNECTION SETUP COMPLETE message */
    (void)tdsrrcsmc_append_start_list(
            &msg_ptr->message.u.rrcConnectionSetupComplete.startList,
            TDSRRCSMC_APPEND_START);
 
    /* Initialize bit mask for RRC Connection Setup complete message */
    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete);
   
    if (tdsrrc_capability_update_requirement.
        ue_radio_capability_update_requirement == TRUE) 
    { 
      TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetupComplete,
        tdsrrc_RRCConnectionSetupComplete,ue_RadioAccessCapability);

      ret_flag = tdsrrcueci_append_ue_radio_access_capability(
                   &msg_ptr->message.u.rrcConnectionSetupComplete.ue_RadioAccessCapability,
                   FALSE,
                   TDSRRCUECI_CAP_UPDATE);

      // Set rrcConnectionSetupComplete.bit_mask to include v370NonCriticalExtensions
      TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetupComplete,
          tdsrrc_RRCConnectionSetupComplete,v370NonCriticalExtensions);

      // Initialize rrcConnectionSetupComplete_v370ext.bit_mask
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
          rrcConnectionSetupComplete_v370ext);


      // Set v370NonCriticalExtensions.bit_mask to include v380NonCriticalExtensions
      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.m.
      v380NonCriticalExtensionsPresent =1;
        
      // Set rrcConnectionSetupComplete_v380ext.bit_mask to include RadioAccessCapability_v380ext
      /*TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.v380NonCriticalExtensions.
      rrcConnectionSetupComplete_v380ext,ue_RadioAccessCapability_v380ext);*/
      
      // Set rx_tx_TimeDifferenceType2Capable to FALSE. Revisit this section when UE supports this.
      /*msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.v380NonCriticalExtensions.
      rrcConnectionSetupComplete_v380ext.ue_RadioAccessCapability_v380ext.
      ue_PositioningCapabilityExt_v380.rx_tx_TimeDifferenceType2Capable = FALSE; */
   
      // Initialize bit_mask of dl_PhysChCapabilityFDD_v380ext 
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.v380NonCriticalExtensions.
      rrcConnectionSetupComplete_v380ext.dl_PhysChCapabilityFDD_v380ext);

      // Initialize v380NonCriticalExtensions.bit_mask 
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions);
  

      /* Select v3a0NonCriticalExtensions on v380NonCriticalExtensions */
      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.m.v3a0NonCriticalExtensionsPresent =1;

      TDSRRC_RESET_MSG_IE_PRESENT( msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions);
      /* Select laterNonCriticalExtensions on v3a0NonCriticalExtensions */
      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.m.laterNonCriticalExtensionsPresent =1 ;

      /* Set laterNonCriticalExtensions bitmask to 0 */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions);

      /* Reset bit_mask for rrcConnectionSetupComplete_v3a0ext. */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.rrcConnectionSetupComplete_v3a0ext);

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
      if(tdsrrc_ue_pos_cap_pch)
      {
        TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.v380NonCriticalExtensions.
          v3a0NonCriticalExtensions.rrcConnectionSetupComplete_v3a0ext, ue_RadioAccessCapability_v3a0ext);
        msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.v380NonCriticalExtensions.
          v3a0NonCriticalExtensions.rrcConnectionSetupComplete_v3a0ext.ue_RadioAccessCapability_v3a0ext.ue_PositioningCapabilityExt_v3a0.
          validity_CellPCH_UraPCH = tdsrrc_UE_PositioningCapabilityExt_v3a0_validity_CellPCH_UraPCH_true_;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: PCH validity set");
      }
#endif

      /* Select v3g0NonCriticalExtensions on laterNonCriticalExtensions */
      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.m.
       v3g0NonCriticalExtensionsPresent =1 ;

      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions);

      /* No MP filed on laterNonCriticalExtensions */

      /* Select v4b0NonCriticalExtensions on v3g0NonCriticalExtensions */
      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.m.v4b0NonCriticalExtensionsPresent =1;

      /* Reset bit_mask of rrcConnectionSetupComplete_v3g0ext. */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.rrcConnectionSetupComplete_v3g0ext);


      /* Set v4b0ext bit_mask to rrc_ue_RadioAccessCapability_v4b0ext_present */
      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
        m.ue_RadioAccessCapability_v4b0extPresent =1;

      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
        ue_RadioAccessCapability_v4b0ext);

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.pdcp_Capability_r4_ext.supportForRfc3095.t
        = T_tdsrrc_PDCP_Capability_r4_ext_supportForRfc3095_notSupported;

#ifdef FEATURE_UMTS_PDCP
      if (tdsrrc_pdcp_enabled)
      { 
        msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
          ue_RadioAccessCapability_v4b0ext.pdcp_Capability_r4_ext.supportForRfc3095.t = 
          T_tdsrrc_PDCP_Capability_r4_ext_supportForRfc3095_supported;
        
        
        msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
          ue_RadioAccessCapability_v4b0ext.pdcp_Capability_r4_ext.supportForRfc3095.u.supported = 
          rtxMemAllocTypeZ(&tdsenc_ctxt, tdsrrc_PDCP_Capability_r4_ext_supported );
        
        msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
          ue_RadioAccessCapability_v4b0ext.pdcp_Capability_r4_ext.supportForRfc3095.u.supported->maxROHC_ContextSessions = 
          tdsrrc_MaxROHC_ContextSessions_r4_s4;
  
        
        msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
          ue_RadioAccessCapability_v4b0ext.pdcp_Capability_r4_ext.supportForRfc3095.u.
          supported->reverseCompressionDepth =0;
      }
#endif /* FEATURE_UMTS_PDCP */
      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.m.tdd_CapabilityExtPresent = 1;

      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.rf_Capability);

      /* set TDD RF capability */
      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.rf_Capability.m.tddRF_CapabilityPresent = 1;

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.rf_Capability.tddRF_Capability.ue_PowerClass = 2;

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.rf_Capability.tddRF_Capability.
      radioFrequencyBandTDDList = tdsrrc_RadioFrequencyBandTDDList_a;

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.rf_Capability.tddRF_Capability.
      chipRateCapability = tdsrrc_ChipRateCapability_mcps1_28;

      /* set physical channel capability */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR);

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.
      physicalChannelCapability_LCR.m.tdd128_PhysChCapabilityPresent = 1;

      /* DL physical channel capability */
      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
      tdd128_PhysChCapability.downlinkPhysChCapability.maxTS_PerSubFrame = 4;

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
      tdd128_PhysChCapability.downlinkPhysChCapability.maxPhysChPerFrame = 64;

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
      tdd128_PhysChCapability.downlinkPhysChCapability.minimumSF = tdsrrc_MinimumSF_DL_sf1;

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
      tdd128_PhysChCapability.downlinkPhysChCapability.supportOfPDSCH = FALSE;

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
      tdd128_PhysChCapability.downlinkPhysChCapability.maxPhysChPerTS = 16;

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
      tdd128_PhysChCapability.downlinkPhysChCapability.supportOf8PSK = FALSE;

      /* UL physical channel capability */
      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
      tdd128_PhysChCapability.uplinkPhysChCapability.maxTS_PerSubFrame = 5;

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
      tdd128_PhysChCapability.uplinkPhysChCapability.maxPhysChPerTimeslot = tdsrrc_MaxPhysChPerTimeslot_ts2;

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
      tdd128_PhysChCapability.uplinkPhysChCapability.minimumSF = tdsrrc_MinimumSF_UL_sf1;

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
      tdd128_PhysChCapability.uplinkPhysChCapability.supportOfPUSCH = FALSE;

      msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
      tdd128_PhysChCapability.uplinkPhysChCapability.supportOf8PSK = FALSE;

      /* Downlink TDD measurement are not supported   */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.measurementCapability_r4_ext.downlinkCompressedMode_LCR);

      /* Uplink TDD measurement are not supported   */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
      v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
      ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.measurementCapability_r4_ext.uplinkCompressedMode_LCR);

      if((msg_version == TDSMSG_REL5) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5))
      {

        if((support_ho_to_gan)
          || (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
          )
        {

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Populating r3_add_ext in RRCConnSetupComplete message");

          /* Set the bitmask for r3_add_ext */
         TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions,
           rrcConnectionSetupComplete_r3_add_ext);

    
          
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            rrcConnectionSetupComplete_r3_add_ext);

          if(support_ho_to_gan)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Populating r3_add_ext in  for GAN HO support");
            
            TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            rrcConnectionSetupComplete_r3_add_ext,
              tdsrrc_RRCConnectionSetupComplete_r3_add_ext_IEs,v680NonCriticalExtensions);
  
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            rrcConnectionSetupComplete_r3_add_ext.v680NonCriticalExtensions);
  
            TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            rrcConnectionSetupComplete_r3_add_ext.v680NonCriticalExtensions.
            rrcConnectionSetupComplete_v680ext.ue_RadioAccessCapability_v680ext.
            multiModeRAT_Capability_v680ext,supportOfHandoverToGAN);
  
            msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            rrcConnectionSetupComplete_r3_add_ext.v680NonCriticalExtensions.
            rrcConnectionSetupComplete_v680ext.ue_RadioAccessCapability_v680ext.
            multiModeRAT_Capability_v680ext.supportOfHandoverToGAN = 
            tdsrrc_MultiModeRAT_Capability_v680ext_supportOfHandoverToGAN_doesSupportHandoverToGAN;
          }

          if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
          {
            if(!(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              rrcConnectionSetupComplete_r3_add_ext.m.v680NonCriticalExtensionsPresent))
            {
              msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              rrcConnectionSetupComplete_r3_add_ext.m.v680NonCriticalExtensionsPresent =1;
    
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              rrcConnectionSetupComplete_r3_add_ext.v680NonCriticalExtensions);
  
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
              v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              rrcConnectionSetupComplete_r3_add_ext.v680NonCriticalExtensions.
              rrcConnectionSetupComplete_v680ext.ue_RadioAccessCapability_v680ext.
              multiModeRAT_Capability_v680ext);
            }
  

          }
        }
      }
      
      if ((msg_version == TDSMSG_REL5) &&
          (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE Cap REL5");

        /* DL physical channel capability */
        msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
        tdd128_PhysChCapability.downlinkPhysChCapability.maxTS_PerSubFrame = 5;

        msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
        v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.rrcConnectionSetupComplete_v4b0ext.
        ue_RadioAccessCapability_v4b0ext.tdd_CapabilityExt.physicalChannelCapability_LCR.
        tdd128_PhysChCapability.downlinkPhysChCapability.maxPhysChPerFrame = 80;
		
	    /* Select v590NonCriticalExtensions on v4b0NonCriticalExtensions */
        msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent =1;

        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
                  v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                  v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
                  rrcConnectionSetupComplete_v590ext);

        /* Select RadioAccessCapability_v590ext on rrcConnectionSetupComplete_v590ext */
        TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
          rrcConnectionSetupComplete_v590ext,
          tdsrrc_RRCConnectionSetupComplete_v590ext_IEs,
          ue_RadioAccessCapability_v590ext);

        /* No v590 noncritical extension */
        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
          v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions);

        if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
        {
          /* v590 noncritical extension present */
          msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.
            v590NonCriticalExtensions.m.v5c0NonCriticalExtensionsPresent =1;
  
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
            v5c0NonCriticalExtensions.rrcConnectionSetupComplete_v5c0ext);
  
          /* v5c0 noncritical extension present */
          msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
            v5c0NonCriticalExtensions.m.v690NonCriticalExtensionsPresent =1;
  
          /* No v690 noncritical extension */
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
            v5c0NonCriticalExtensions.v690NonCriticalExtensions);
  
          /* Indicate rrc_RRCConnectionSetupComplete_v690ext_IEs_ueCapabilityContainer_present 
             in rrcConnectionSetupComplete_v6xyext */
          TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
            v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
            v5c0NonCriticalExtensions.v690NonCriticalExtensions.rrcConnectionSetupComplete_v690ext,
            tdsrrc_RRCConnectionSetupComplete_v690ext_IEs,ueCapabilityContainer);
        if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
        {
 #ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
          if(tdsrrcsib_is_sib11_11bis_12_read_deferred(TDSDEFERRED_ANY_ALLOWED_SIB))
          {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB read:Appending Deferred meas read IE. in con. setup compl");
              msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
                 v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                 v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
                 v5c0NonCriticalExtensions.v690NonCriticalExtensions.m.v770NonCriticalExtensionsPresent=1;
  
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
                v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
                v5c0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions);
  
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
                v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
                v5c0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
                rrcConectionSetupComplete_v770ext);
  
              msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
                v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
                v5c0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
                rrcConectionSetupComplete_v770ext.m.deferredMeasurementControlReadingPresent=1;
  
              msg_ptr->message.u.rrcConnectionSetupComplete.v370NonCriticalExtensions.
                v380NonCriticalExtensions.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
                v5c0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
                rrcConectionSetupComplete_v770ext.deferredMeasurementControlReading = 
                tdsrrc_RRCConnectionSetupComplete_v770ext_IEs_deferredMeasurementControlReading_true_;
            }
 #endif
          }
        }

        /* Appends the Radio Access Capability to the RRC CONNECTION SETUP
           COMPLETE message */
        tdsrrcueci_append_ue_radio_access_capability_later_than_r3(&msg_ptr->message.u.rrcConnectionSetupComplete.
                                                                v370NonCriticalExtensions.v380NonCriticalExtensions.
                                                                v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                                                                v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.
                                                                v590NonCriticalExtensions.rrcConnectionSetupComplete_v590ext.
                                                                ue_RadioAccessCapability_v590ext,
                                                                &msg_ptr->message.u.rrcConnectionSetupComplete.
                                                                v370NonCriticalExtensions.v380NonCriticalExtensions.
                                                                v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                                                                v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.
                                                                v590NonCriticalExtensions.v5c0NonCriticalExtensions.
                                                                v690NonCriticalExtensions.rrcConnectionSetupComplete_v690ext.
                                                                ueCapabilityContainer,
                                                                TDSRRCUECI_CAP_UPDATE);
      }
    }
    if (tdsrrcmcm_is_dualmode_enabled())
    {
      if (tdsrrc_capability_update_requirement.
          system_specific_cap_update_req_present == TRUE) 
      /* For Single mode operation IE "UE system specific capability" will not
         be included even though UTRAN requests the same */
      {
        int count;
        num_rat=0;
        for(count = 0; count < tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.count && count < 16;count++)
        {
          if(tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[count] == TDSRRC_GSM_CAP)
          {
            num_rat = 1;
            break;
          }
        }
      
        if(num_rat !=0)
        {
          msg_ptr->message.u.rrcConnectionSetupComplete.ue_RATSpecificCapability.n= num_rat;
          ALLOC_ASN1ARRAY1(&tdsenc_ctxt,&msg_ptr->message.u.rrcConnectionSetupComplete.ue_RATSpecificCapability,tdsrrc_InterRAT_UE_RadioAccessCapability);
          if(msg_ptr->message.u.rrcConnectionSetupComplete.ue_RATSpecificCapability.elem == NULL)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed allocate memory ratspecCap");
            return (status);
          }

          TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.rrcConnectionSetupComplete, ue_RATSpecificCapability);
          
          tdsrrcueci_append_gsm_specific_capability(
            &msg_ptr->message.u.rrcConnectionSetupComplete.ue_RATSpecificCapability.elem[0]);
        }
      }   
    }

    if(tdsrrcrce_state_indicator == tdsrrc_RRC_StateIndicator_cell_DCH)
    {
      tdsrrc_update_reconfig_waiting_for_l2ack_dch(TDSRRC_PROCEDURE_RCE);
    }

    /* This places SDU into RLC watermark queue */
    if (TDSRRCSEND_SUCCESS == 
        tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RCE,msg_ptr,ul_lc_id,
        TDSRRCSEND_L2ACK_REQUIRED))
    {
      status = TDSRRCRCE_SUCCESS;

    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failed to xmit RRC conn setup Compl Msg");
    }
  }
  else
  { 
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Can't find RLC ID for UL RB 2: %d");
  } 
 
  return (status);            
} /* tdsrrcrce_send_rrc_connection_setup_complete_message */


/*===========================================================================
FUNCTION tdsrrcrce_send_redirection_req

DESCRIPTION
  This function builds the TDSRRC_REDIRECT_REQ command with all parameters
  received through RRC CONNECTION REJECT message and sends the command to
  Cell Selection Procedure
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type : Returns TDSRRCRCE_SUCCESS if it successfully sends
  TDSRRC_REDIRECT_REQ command. Otherwise returns TDSRRCRCE_FAILURE_MAJOR

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_redirection_req
(
  tdsrrc_DL_CCCH_Message    *msg_ptr,
  boolean                cpid_info_present
)
{
 
  tdsrrc_cmd_type *cmd_ptr;                     /* Pointer to RRC Command */
  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MAJOR; 
                                             /* The status is initialized to
                                                Failure */
  
  sys_band_T gsm_band;
  tdsrr_frequence_band_e_type freq_band;
  tdsrrc_GSM_TargetCellInfoList *GSM_target_cell_info_ptr = NULL;
  tdsrrclog_tdscdma_to_tdscdma_redir_start_event_type td2td_redir_event;
  
  /* Allocates the buffer to RRC Internal command */
  if( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {     
    /* Fill in the command parameters to TDSRRC_REDIRECT_REQ Command */
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_REDIRECT_REQ;
    cmd_ptr->cmd.redirect_req.proc_id = TDSRRC_PROCEDURE_RCE;
    cmd_ptr->cmd.redirect_req.wait_time = tdsrrcrce_wait_time;
    cmd_ptr->cmd.redirect_req.cpid = 128; /* initialize the cpid to an invalid value, TDD:0~127*/
    cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.size = 0;

    if(tdsrrcrce_rsp_to_rrc_conn_req == TDSRRCRCE_CONNECTION_REJECT)
    {
      switch(msg_ptr->message.u.rrcConnectionReject.u.
                 r3.rrcConnectionReject_r3.redirectionInfo.t)
      {
        case T_tdsrrc_RedirectionInfo_frequencyInfo: 
          cmd_ptr->cmd.redirect_req.choice= TDSRRC_FREQ_REDIRECTION;
      
          cmd_ptr->cmd.redirect_req.u.freq.uarfcn_DL = (uint16)
                                    msg_ptr->message.u.rrcConnectionReject.u.
                                    r3.rrcConnectionReject_r3.redirectionInfo.u.
                                    frequencyInfo->modeSpecificInfo.u.tdd->uarfcn_Nt;

          /* For rrc connection reject with redirection info Start Event*/
          tdsrrc_t2t_redir_due_to_rej = TRUE;
          td2td_redir_event.wait_timer_in_secs = tdsrrcrce_wait_time;
          td2td_redir_event.redir_reason = TD2TD_REDIR_REJECT;
          td2td_redir_event.freq = cmd_ptr->cmd.redirect_req.u.freq.uarfcn_DL;
          event_report_payload(EVENT_TDSCDMA_TO_TDSCDMA_REDIRECTION_START, sizeof(td2td_redir_event),
                                (void *)&td2td_redir_event);

          break;
    
        case T_tdsrrc_RedirectionInfo_interRATInfo:
          cmd_ptr->cmd.redirect_req.choice= TDSRRC_INTER_SYSTEM_GSM;

          if(msg_ptr->message.u.rrcConnectionReject.u.r3.m.laterNonCriticalExtensionsPresent)
          {
            if(msg_ptr->message.u.rrcConnectionReject.u.r3.laterNonCriticalExtensions.m.
                v690NonCriticalExtensionsPresent)
            {
              if(msg_ptr->message.u.rrcConnectionReject.u.r3.laterNonCriticalExtensions.
                v690NonCriticalExtensions.rrcConnectionReject_v690ext.m.redirectionInfo_v690extPresent)
              {
                 uint32 local_idx=0;
                 GSM_target_cell_info_ptr = &msg_ptr->message.u.rrcConnectionReject.u.r3.laterNonCriticalExtensions.
                    v690NonCriticalExtensions.rrcConnectionReject_v690ext.redirectionInfo_v690ext;

                 if(TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
                 {
                   tdsrrccsp_cleanup_bplmn_state();
                 }

                 while ((GSM_target_cell_info_ptr->n > local_idx) && 
                        (cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.size < TDS_MAX_RRC_RR_GSM_CELL_LIST_SIZE))
                 {
                      /* Let first check the type of the cell to find if we
                       * really need to give this cell to L1 */
                    switch(GSM_target_cell_info_ptr->elem[local_idx].frequency_band)
                    {
                       case tdsrrc_Frequency_Band_dcs1800BandUsed:
                         freq_band = TDSRR_DCS_1800;
                         break;
              
                       case tdsrrc_Frequency_Band_pcs1900BandUsed:
                         freq_band = TDSRR_PCS_1900;
                         break;
              
                       default:
                         TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unsupported Band %d rcvd",GSM_target_cell_info_ptr->elem[local_idx].frequency_band);
                         freq_band = TDSRR_MAX_BANDS;
                         break;
                    }
              
                    if((gsm_band
                            = tdsrr_get_arfcn_band_internal((uint16)GSM_target_cell_info_ptr->elem[local_idx].bcch_ARFCN, freq_band))
                         != INVALID_BAND)
                    {
                       TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding ARFCN %d and band %d to the list to be sent to RR",GSM_target_cell_info_ptr->elem[local_idx].bcch_ARFCN,freq_band);
              
                       /* Get bcch_ARFCN */
                       cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.arfcns[cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.size].num
                          = (uint16) GSM_target_cell_info_ptr->elem[local_idx].bcch_ARFCN;
              
                       cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.arfcns[cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.size].band
                          = gsm_band;
                    }
                    else
                    {
                        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Band %d for ARFCN %d is invalid. Not adding to list",(tdsrr_frequence_band_e_type)GSM_target_cell_info_ptr->elem[local_idx].frequency_band,(uint16) GSM_target_cell_info_ptr->elem[local_idx].bcch_ARFCN);
                    }
                    if(GSM_target_cell_info_ptr->elem[local_idx].m.bsicPresent)
                    {
                       cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.BSIC[cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.size].BSIC_NCC 
                              = GSM_target_cell_info_ptr->elem[local_idx].bsic.ncc;
                       cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.BSIC[cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.size++].BSIC_BCC 
                              = GSM_target_cell_info_ptr->elem[local_idx].bsic.bcc;
                    }
                    else
                    {
                       cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.BSIC[cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.size].BSIC_NCC 
                              = 0xFF;
                       cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.BSIC[cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.size++].BSIC_BCC 
                              = 0xFF;
                    }
                    local_idx++;
                 }   
              }
            }
          }
          break;

        default:     
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"This should never happen");
          break;
      }      
    }
    else if(tdsrrcrce_rsp_to_rrc_conn_req == TDSRRCRCE_CONNECTION_SETUP)
    {
      cmd_ptr->cmd.redirect_req.choice= TDSRRC_FREQ_REDIRECTION;

      if ((msg_ptr->message.u.rrcConnectionSetup.t == T_tdsrrc_RRCConnectionSetup_later_than_r3)&& 
          (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.t ==
           T_tdsrrc_RRCConnectionSetup_criticalExtensions_6_r4)) 
      {
        cmd_ptr->cmd.redirect_req.u.freq.uarfcn_DL = (uint16)
                                    msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                                    r4.rrcConnectionSetup_r4.frequencyInfo.modeSpecificInfo.
                                    u.tdd->uarfcn_Nt;
        if((cpid_info_present)
         && (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.r4
         .rrcConnectionSetup_r4.dl_InformationPerRL_List.n != 0))
        {
          cmd_ptr->cmd.redirect_req.cpid = (uint8)
              msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.r4.rrcConnectionSetup_r4.
              dl_InformationPerRL_List.elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
        } 
      }
      else
      {
        if ((msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.t == 
             T_tdsrrc_RRCConnectionSetup_criticalExtensions_5_criticalExtensions) && 
            (msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.t ==
             T_tdsrrc_RRCConnectionSetup_criticalExtensions_5_r5)) 
        {
          if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
          {
            /* Safely assume REL-5 as other cases would have been filtered by the time we are here */
            cmd_ptr->cmd.redirect_req.u.freq.uarfcn_DL = (uint16)
            msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
              criticalExtensions.u.r5.rrcConnectionSetup_r5.frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
            
            if((cpid_info_present) && 
               (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                criticalExtensions.u.r5.rrcConnectionSetup_r5,
                tdsrrc_RRCConnectionSetup_r5_IEs,dl_InformationPerRL_List)))
            {
              cmd_ptr->cmd.redirect_req.cpid = (uint8)
              msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                criticalExtensions.u.r5.rrcConnectionSetup_r5.
                dl_InformationPerRL_List.elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
            }
          } /* if R5 NV is set */
        } /* if R5 msg*/
        else if (TDSRRCRCE_R6_CRITICAL_EXT_PRESENT)
        { 
          if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
          {
            /* Safely assume REL-6 as other cases would have been filtered by the time we are here */
            cmd_ptr->cmd.redirect_req.u.freq.uarfcn_DL = (uint16)
            msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
              criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6.frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
            
            if((cpid_info_present) && 
               (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6, 
                tdsrrc_RRCConnectionSetup_r6_IEs,dl_InformationPerRL_List)))
            {
              cmd_ptr->cmd.redirect_req.cpid = (uint8)
              msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                criticalExtensions.u.criticalExtensions.u.r6.rrcConnectionSetup_r6.
                dl_InformationPerRL_List.elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
            }
          } /* if R6 NV is set */
        } /* if R6 msg*/
        else if (TDSRRCRCE_R7_CRITICAL_EXT_PRESENT)
        { 
          if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
          {
            /* Safely assume REL-6 as other cases would have been filtered by the time we are here */
            cmd_ptr->cmd.redirect_req.u.freq.uarfcn_DL = (uint16)
            msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
              criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7.frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
            
            if((cpid_info_present) && 
               (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7,
                tdsrrc_RRCConnectionSetup_r7_IEs,dl_InformationPerRL_List)))
            {
              cmd_ptr->cmd.redirect_req.cpid = (uint8)
              msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.rrcConnectionSetup_r7.
                dl_InformationPerRL_List.elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
            }
          } /* if R7 NV is set */
        } /* if R6 msg*/
#ifdef FEATURE_TDSCDMA_REL8 
        else if (TDSRRCRCE_R8_CRITICAL_EXT_PRESENT)
        { 
          if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8)
          {
            /* Safely assume REL-7 as other cases would have been filtered by the time we are here */
            cmd_ptr->cmd.redirect_req.u.freq.uarfcn_DL = (uint16)
            msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8.frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
            
            if((cpid_info_present) && (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8,
                       tdsrrc_RRCConnectionSetup_r8_IEs,dl_InformationPerRL_List)))
            {
              cmd_ptr->cmd.redirect_req.cpid = (uint8)
                  msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                  criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.rrcConnectionSetup_r8.
                  dl_InformationPerRL_List.elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
            }
          } /* if R8 NV is set */
        } /* if R8 msg*/
#endif /* FEATURE_TDSCDMA_REL8 */ 
#ifdef FEATURE_TDSCDMA_REL9 
        else if (TDSRRCRCE_R9_CRITICAL_EXT_PRESENT)
        { 
          if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9)
          {
            /* Safely assume REL-7 as other cases would have been filtered by the time we are here */
            cmd_ptr->cmd.redirect_req.u.freq.uarfcn_DL = (uint16)
            msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.rrcConnectionSetup_r9.frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
            
            if((cpid_info_present) && (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.rrcConnectionSetup_r9,
                       tdsrrc_RRCConnectionSetup_r9_IEs,dl_InformationPerRL_List)))
            {
              cmd_ptr->cmd.redirect_req.cpid = (uint8)
                  msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                  criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.rrcConnectionSetup_r9.
                  dl_InformationPerRL_List.elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
            }
          } /* if R9 NV is set */
        } /* if R9 msg*/
#endif /* FEATURE_TDSCDMA_REL9 */ 

      } /*else if later than R3 is selected */
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: incorrect enum type %d for connection rsp",tdsrrcrce_rsp_to_rrc_conn_req);
      return (status);
    }

    /* sends the command to Cell Selection Procedure */
    tdsrrc_put_int_cmd(cmd_ptr);         
    status = TDSRRCRCE_SUCCESS; /* Status is initialized to Success */           
  }

  /* RRC Task failed to allocate memory */
  else 
  { 
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unable to allocate memory for Cmd: %d",TDSRRC_REDIRECT_REQ);
  } 
  return (status);
   
} /* tdsrrcrce_send_redirection_req */

/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_rel_ind

DESCRIPTION
  This function builds RRC_REL_IND command to post to MM with the 
  RRC Connection Release cause.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it successfully sends 
  RRC_REL_IND command to the MM. Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_rel_ind
(
  void
)
{
  mm_cmd_type *cmd_ptr;                      /* Pointer to the RRC Command */

  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MAJOR; 
                                             /* The status is initialized to
                                                Failure */
  tdsrrcrce_sig_con_delay = FALSE;
  /* Allocate memory for RRC_REL_IND command */
  if( (cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_REL_IND)) != NULL)
  {
    cmd_ptr->cmd.rrc_rel_ind.as_id = tdsrrc_get_as_id();
    /* Fill in parameters fro RRC_REL_IND command */
    cmd_ptr->cmd.rrc_rel_ind.rel_cause = tdsrrc_release_cause;
    
    /* Update the Max HFNs values to be stored in SIM or USIM */ 
    (void) tdsrrcsmc_append_hfns_to_rel_ind(cmd_ptr->cmd.rrc_rel_ind.hfn_cs,
                                  cmd_ptr->cmd.rrc_rel_ind.hfn_ps);

    /* Sends the command to MM */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(cmd_ptr);
    }
    mm_put_cmd(cmd_ptr);
   
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC sends RRC_REL_IND to MM,Cause %d",tdsrrc_release_cause);

    status = TDSRRCRCE_SUCCESS;           /* sets the status to success */

    tdsrrcrce_send_conn_rel_cause_event((uint8)RRC_ABORT_CAUSE_TYPE_MAX + (uint8)tdsrrc_release_cause);
  }    
  else   /* RRC Task failed to allocate memory */
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unable to allocate memory for MM Cmd:%d",RRC_REL_IND);
  }     

  /* Do not release RRC's lock when T2X IRAT ongoing and wait for suspend TDS cnf. Due to RRC need to exchange lock
   * after suspend TDS success
   */
  if (!(((tdsrrccsp_get_curr_scan()==RRC_CSP_TDS_TO_GSM_CELL_RESELECTION_SCAN) ||
        (tdsrrccsp_get_curr_scan()==TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN))
       && (tdsrrccsp_get_csp_substate() == TDSRRCCSP_WT_SUSPEND_MODE_CHANGE_CNF)))
  {
  tdsrrcwrm_release_lock();
  }
  tdsrrc_proc_type = SYS_PROC_TYPE_NONE; 

  tdsrrc_send_rab_rel_ind_on_ps_release();

#ifdef FEATURE_WRLF_SYSTEM_SEL
  /* When releasing connection due to re-establishment timer expiry send suitable 
       search end ind to NAS (if splmn 30s timer has not expired), and disable BPLMN
       ind as FALSE to enable the periodic BPLMN searches*/
  if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
  {
    tdsrrccsp_check_n_send_suitable_srch_end_ind();
    tdsrrccsp_send_disable_bplmn_ind(FALSE);
  }
#endif

  /* Initialize release cause */
  tdsrrc_release_cause = RRC_REL_CAUSE_UNSPEC;

  return (status);
}/*tdsrrcrce_send_rrc_rel_ind */
/*===========================================================================
FUNCTION tdsrrcrce_process_rce_wait_for_srb_rel_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in TDSRRCRCR_WAIT_FOR_SRB_REL_CNF substate of any RRC 
  state.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_srb_rel_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  switch (cmd_ptr->cmd_hdr.cmd_id)
  {

    case TDSRRC_STATE_CHANGE_IND:
      if((cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED) &&
         ( tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
      {
        if ((TDSRRC_MODE_ACTIVE == tdsrrcmcm_get_rrc_mode())
#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
          /* For inter-RAT redirection due to connection release, handle STATE_CHANGE_IND
          * in STANDBY_MODE as RRC releases all channels before starting redirection search
          */
          || ((TDSRRC_MODE_STANDBY == tdsrrcmcm_get_rrc_mode())
             && (tdsrrccsp_get_curr_scan() == TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN))
#endif
#ifdef FEATURE_WRLF_SYSTEM_SEL
          || (TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS() && 
            (TDSRRC_MODE_INACTIVE == tdsrrcmcm_get_rrc_mode()))
#endif
           )
        {
          /* Check if abort is pending */
          if (tdsrrcrce_send_abort_cnf == TRUE) 
          {
            if(TDSRRCRCE_SUCCESS == tdsrrcrce_send_rrc_abort_cnf() )
            { 
              /* Clear the ESTABLISHED_RABS while bringing down RRC Connection*/
              tdsrrc_clear_established_rabs();
              
              /* Reset Signalling Connections status */
              tdsrrcscr_reset_signalling_connection_status();
              
              tdsrrcrce_substate = TDSRRCRCE_INITIAL;
              tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
              
            }
            else
            {
              ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
            }
          }
          else
          {
            if(TDSRRCRCE_SUCCESS == tdsrrcrce_send_rrc_rel_ind() )
            {
              /* Clear the ESTABLISHED_RABS while bringing down RRC Connection*/
              tdsrrc_clear_established_rabs();

              /* Reset Signalling Connections status */
              tdsrrcscr_reset_signalling_connection_status();

              tdsrrcrce_substate = TDSRRCRCE_INITIAL;
              tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;

              break;
            }
            else
            {     
              ERR_FATAL("Unable to post an MM Cmd: %d",RRC_REL_IND,0,0);
            }  
          }
        }
        else
        {

          /* Check if abort is pending, need to do so if there
             is race condition for receiving soft-abort from NAS*/
          if (tdsrrcrce_send_abort_cnf == TRUE) 
          {
            if(TDSRRCRCE_SUCCESS == tdsrrcrce_send_rrc_abort_cnf() )
            { 
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Force sending ABORT CNF in RRC state:%d",tdsrrcmcm_get_rrc_mode());
              
            }
            else
            {
              ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
            }
          }
          /* Don't send the REL_IND to MM */
          /* Clear the ESTABLISHED_RABS while bringing down RRC Connection*/
          tdsrrc_clear_established_rabs();

          /* Reset Signalling Connections status */
          tdsrrcscr_reset_signalling_connection_status();

          tdsrrcrce_substate = TDSRRCRCE_INITIAL;
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        }
      }
      else
      {
        ERR_FATAL("RRCSCMGR posted an unregistered event",0,0,0);
      }
      break;

    case TDSRRC_ABORT_REQ:
      /* Another TDSRRC_ABORT_REQ is received. Just set the flag again just in 
       * case it wasn't set before
       */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ in substate: %d,saved",tdsrrcrce_substate);
      tdsrrcrce_send_abort_cnf = TRUE;

      break;

    case TDSRRC_CHANNEL_CONFIG_CNF:

      if(cmd_ptr->cmd.chan_config_cnf.transaction_id == tdsrrcrce_chl_config_transaction_id)
      {
        if (tdsrrcrce_send_abort_cnf == TRUE) 
        {
          /* Send RRC_ABORT_CNF command to the MM */
          if(TDSRRCRCE_SUCCESS == 
            tdsrrcrce_send_rrc_abort_cnf() )      
          {
            /* Call this fn to make sure that Establised RABS contains invalid values */
            tdsrrc_clear_established_rabs();

            tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */
            tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
            /* Reset Signalling Connections status */
            tdsrrcscr_reset_signalling_connection_status();
          }    
          else
          {     
            ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
          }  
        }
        else
        {
          if(TDSRRCRCE_SUCCESS == tdsrrcrce_send_rrc_rel_ind() )
          {
            /* Clear the ESTABLISHED_RABS while bringing down RRC Connection*/
            tdsrrc_clear_established_rabs();

            /* Reset Signalling Connections status */
            tdsrrcscr_reset_signalling_connection_status();

            tdsrrcrce_substate = TDSRRCRCE_INITIAL;
            tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;

            break;
          }
          else
          {     
            ERR_FATAL("Unable to post an MM Cmd: %d",RRC_REL_IND,0,0);
          }
        }
      }
      else
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring tr_id %d in chl config cnf. Expected tr_id = %d",cmd_ptr->cmd.chan_config_cnf.transaction_id,tdsrrcrce_chl_config_transaction_id);
      }
      break;
      
    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;
  }  

} /* tdsrrcrce_process_rce_wait_for_srb_rel_cnf_substate */


/*===========================================================================
FUNCTION tdsrrcrce_process_rce_wait_for_rb0_rel_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in TDSRRCRCE_WAIT_FOR_RB0_REL_CNF substate of any RRC 
  state.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_rb0_rel_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  rrc_abort_cause_e_type  cause = RRC_MAX_ACCESS_FAILURE;

  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_ABORT_REQ:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ rcvd in %d substate",tdsrrcrce_substate);
      tdsrrcrce_send_abort_cnf = TRUE;
      break;

    case TDSRRC_STATE_CHANGE_IND:
      tdsrrctmr_stop_timer(TDSRRCTMR_WAIT_TIME_TIMER); 
      if (cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED)
      {
#ifdef FEATURE_INTERRAT_PCCO_G2TD
        if(tdsrrccho_is_g2td_pcco_in_progress())
        {
          tdsrrc_conn_setup_ind_type conn_setup_ind_params;
          conn_setup_ind_params.status = FAILURE;
          conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"State change ind when PCCO active in rb0_rel_cnf substate");
          tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  
                
          /* set rce_substate to initial */
          tdsrrcrce_substate = TDSRRCRCE_INITIAL;
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          break;
        }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */
        
        if (tdsrrcrce_send_abort_cnf == TRUE)
        {
          /* Send RRC_ABORT_CNF command to the MM */
          if(TDSRRCRCE_SUCCESS == tdsrrcrce_send_rrc_abort_cnf() )      
          {        
            /* At this time, none of the RBs are setup. Simply reset the RCE
               substate */
            tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */
            tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
            break;
          }    
          else
          {     
            ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
          }  
        }
        else
        {
          if(!tdsrrcrce_abort_ind_sent)
          {
            if(TDSRRCRCE_SUCCESS == tdsrrcrce_send_rrc_abort_ind(tdsrrcrce_mm_abort_cause) )      
            {
              /* The RCE Procedure ends */
              tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */
              tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
            }    
            else
            {     
              ERR_FATAL("Unable to post MM Command: %d",RRC_ABORT_IND,0,0);
            }
          }
          /* The RCE Procedure ends */
          tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */
          
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          tdsrrcrce_abort_ind_sent = FALSE;
        }
      }
      break;

    case TDSRRC_EST_REQ: 

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rcvd TDSRRC_EST_REQ from MM with cause:%d",cmd_ptr->cmd.rrc_est_req.cause);

      tdsrrc_protocol_error_indicator = tdsrrc_ProtocolErrorIndicator_noError;

      /* Resets the V300 variable */
      tdsrrc_v_300 = 0;
   
      /* Initialize the Transaction Id for RRC CONNECTION SETUP to Invalid */
      tdsrrcrce_setup_transaction_identifier = TDSRRCRCE_INVALID_TRANSACTION_ID;
      
      /* Initialize the Transaction Id for RRC CONNECTION REJECT to Invalid */
      tdsrrcrce_reject_transaction_identifier = TDSRRCRCE_INVALID_TRANSACTION_ID;

      /* Initialize the Wait Time to zero seconds */
      tdsrrcrce_wait_time = 0;

      /* Initialize abort flag to FALSE */
      tdsrrcrce_send_abort_cnf = FALSE;

      tdsrrcrce_abort_ind_sent = FALSE;

      /* Initialize release cause to unspecified */
      tdsrrc_release_cause = RRC_REL_CAUSE_UNSPEC;

      if(TDSRRCRCE_SUCCESS == tdsrrcrce_validate_rrc_est_req(cmd_ptr))
      {
        /*updates the establishment cause */
        tdsrrc_establishment_cause = cmd_ptr->cmd.rrc_est_req.cause;
        tdsrrc_proc_type = cmd_ptr->cmd.rrc_est_req.proc_type;
      
        /* Updates Initial UE Identity */
        tdsrrc_initial_ue_identity_choice = cmd_ptr->cmd.rrc_est_req.ue_id_choice;

        tdsrrc_initial_ue_identity = cmd_ptr->cmd.rrc_est_req.initial_ue_id;

        tdsrrc_est_cn_domain_id = cmd_ptr->cmd.rrc_est_req.cn_domain_id;

        tdsrrc_est_call_type = cmd_ptr->cmd.rrc_est_req.call_type;

        tdsrrc_est_req_for_rai = cmd_ptr->cmd.rrc_est_req.rai;

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_EST_REQ for cn_domain: %d",tdsrrc_est_cn_domain_id);
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAI PLMN: %d-%d ",TDSRRC_CSP_GET_INT_MCC(tdsrrc_est_req_for_rai.lai.plmn)
                                  ,TDSRRC_CSP_GET_INT_MNC(tdsrrc_est_req_for_rai.lai.plmn)
                                  ,0);
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"LAC: %x %x",tdsrrc_est_req_for_rai.lai.lac[0],tdsrrc_est_req_for_rai.lai.lac[1]);
        if(tdsrrc_est_cn_domain_id != RRC_CS_DOMAIN_CN_ID)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAC: %d",tdsrrc_est_req_for_rai.rac[0]);
        }

        /* Function call to CSP to know whether UE is camped on  */
        if (TDSRRC_CAMPED_ON == tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_RCE))
        {
          if(TDSRRCRCE_SUCCESS == tdsrrcrce_validate_cell_access_status(
                                   tdsrrc_est_cn_domain_id,
                                   tdsrrc_establishment_cause,
                                   &cause)
            )
          {
            if(TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
            {
              /* Due to race condition because of "context-switching", 
               * RCE needs to update it's substate before putting 
               * TDSRRC_BPLMN_SUSPEND_REQ in the queue 
               */
              tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_UE_CAMP_ON;

              /* With the below feature defined we will suspend and resume
               * the BPLMN search for any establishment cause received 
               * from NAS
               */
              tdsrrccsp_send_bplmn_suspend_req(TDSRRC_PROCEDURE_RCE, TDSBPLMN_SUSPEND_REASON_REGISTRATION);

              /* Wait for NEW_CELL_IND in TDSRRCRCE_WAIT_FOR_UE_CAMP_ON substate */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE waiting for SUSPEND_CNF from CSP. BPLMN srch in prog");
            
              /* Starting this timer is NOT required as CSP on receiving TDSRRC_BPLMN_SUSPEND_REQ
               * will send TDSRRC_BPLMN_SUSPEND_CNF almost immediately. This timer is started only 
               * as a recovery mechanism in case CSP fails to send TDSRRC_BPLMN_SUSPEND_CNF to RCE
               */
              tdsrrctmr_start_timer(TDSRRCTMR_UE_TO_CAMP_ON_TIMER, 
                            (unsigned long) TDSRRCRCE_TIMER_VALUE_FOR_UE_TO_CAMP_ON);       
            }
            else
            {
              if(tdsrrcwrm_acquire_lock_for_est_req(tdsrrc_est_cn_domain_id,0xfffffff,TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_EST_REQ_IN_MS) == FAILURE)
              {
                /* Send RRC_ABORT_IND command to the MM */
                if(TDSRRCRCE_SUCCESS != 
                   tdsrrcrce_send_rrc_abort_ind(RRC_NO_RESOURCE_AVAILABLE) )      
                {
                 
                  ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
                } 
                tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */

                tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
                return;
              }
               /* UE is camped on, then Check whether SIB7 is current */
              if(TRUE == tdsrrcsib_check_and_update_sib7_validity (TDSRRC_PROCEDURE_RCE) )
              { 
                /* UE is camped on and SIB7 is current, then Setup RB0 */
                if( TDSRRCRCE_SUCCESS == 
                    tdsrrcrce_send_rrc_channel_config_req_for_rb0(TDSESTABLISH_RB,
                                                               TDSRRC_STATE_CONNECTING,
                                                               TRUE ))  
                {              
                  tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF;     
                }    
                else  /* Failed to send a internal command to RRC LLC*/
                {     
                  ERR_FATAL("Unable to send internal cmd to LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);         
                }
              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE waits for SIB7 to become current"); 
                tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_SIB7_CURRENT;
              }
            }
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH," Cause value filled in RRC_ABORT_IND cmd= %d ",cause);
            if(tdsrrc_est_cn_domain_id == RRC_CS_DOMAIN_CN_ID)
            {
              if(TDSRRCRCE_SUCCESS != tdsrrcrce_send_rrc_abort_ind(RRC_AC_NOT_ALLOWED_CS_DOMAIN) )
              {
                ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
              }
            }
            else
            {
              if(TDSRRCRCE_SUCCESS != tdsrrcrce_send_rrc_abort_ind(RRC_AC_NOT_ALLOWED_PS_DOMAIN) )
              {
                ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
              }
            }

          }
        }
        else /* UE is not camped on */
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE waiting for UE to camp on");
          tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_UE_CAMP_ON;

          /* start the timer for UE to camp on */
          tdsrrctmr_start_timer(TDSRRCTMR_UE_TO_CAMP_ON_TIMER, 
                        (unsigned long) TDSRRCRCE_TIMER_VALUE_FOR_UE_TO_CAMP_ON);       
        }
      }
      else /* Validation Failed */
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Validation failure in TDSRRC_EST_REQ");

#ifdef FEATURE_INTERRAT_PCCO_G2TD
        /* If GTOTDS is in progress,send a conn_setup_ind with failure to CHO
           CHO would deactivate TDS and pass control back to GSM.Do not send
           RRC_ABORT_IND to MM if GTOTDS is active */
        if(tdsrrccho_is_g2td_pcco_in_progress())
        { 
          tdsrrc_conn_setup_ind_type conn_setup_ind_params;
          conn_setup_ind_params.status = FAILURE;
          conn_setup_ind_params.conn_failure_reason = TDSRRC_CONN_FAILURE_OTHER;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Validation failed in TDSRRC_EST_REQ when PCCO is active");
          tdsrrcrce_send_conn_setup_ind(conn_setup_ind_params);                  

          /* Reset RCE susbstate */
          tdsrrcrce_substate = TDSRRCRCE_INITIAL;
          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
          return;
        }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */

        /* Send RRC_ABORT_IND command to the MM */
        if(TDSRRCRCE_SUCCESS != 
           tdsrrcrce_send_rrc_abort_ind(RRC_UNSUPPORTED_INITIAL_UE_ID) )      
        {
         
          ERR_FATAL("Unable to post an MM Command: %d",RRC_ABORT_IND,0,0);
        } 
      }
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;
  }  
} /* tdsrrcrce_process_rce_wait_for_rb0_rel_cnf_substate */

/*===========================================================================
FUNCTION tdsrrcrce_process_rce_wait_for_sig_con_rel_compl_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL sub state either 
  CELL_FACH State or CELL_DCH state.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_sig_con_rel_compl_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{ 
  boolean initiate_cell_sel;

  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_DOWNLINK_SDU_IND:
      /* RRC Connection Setup or RRC Connection Reject may be received in this
         Substate because of cell-reselections. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Simultanous transactions not supported");
      break;

    case TDSRRC_STATE_CHANGE_IND:
      /* Send RRC_ABORT_CNF command to the MM since RCE already in this substate
         since it has received TDSRRC_ABORT_REQ */
      if (tdsrrcrce_send_abort_cnf == TRUE)
      {
        if(TDSRRCRCE_FAILURE_MAJOR == 
         tdsrrcrce_send_rrc_abort_cnf() )      
        {        
          ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
        }
      }
#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
      else if((tdsrrc_csfb_extended_status == CSFB_CS_RB_RELEASED) && 
        (cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED))
      {
        if(TDSRRCRCE_SUCCESS != tdsrrcrce_send_rrc_rel_ind())
        {
          ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
        }
      }
#endif
        
        tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */

        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;

        /* Clear the ESTABLISHED_RABS while bringing down RRC Connection*/
        tdsrrc_clear_established_rabs();

        /* Reset Signalling Connections status */
        tdsrrcscr_reset_signalling_connection_status();
        tdsrrcrce_sig_conn_rel_req_for_both_domains = FALSE;
      break;

    case TDSRRC_ABORT_REQ:
      /* Another TDSRRC_ABORT_REQ is received. Just set the flag again just in 
       * case it wasn't set before
       */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ in substate: %d,saved",tdsrrcrce_substate);
      tdsrrcrce_send_abort_cnf = TRUE;

      initiate_cell_sel = FALSE;
      if(cmd_ptr->cmd.rrc_abort_req.hard_abort == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received HARD_ABORT flag from NAS");
        initiate_cell_sel = TRUE;
      }

      if(cmd_ptr->cmd.rrc_abort_req.abort_cause == RRC_DUAL_STANDBY_PS_ABORT)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received PS Abort from NAS");
        initiate_cell_sel = TRUE;
      }
      if(initiate_cell_sel)
      {
        /* Send initiate cell sel command to CSP so that UE is taken to Idle */
        (void)tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCE, TDSRRC_TX_TO_DISCON_HARD_ABORT);

        /* Wait for successful release of SRBs */
        tdsrrcrce_substate = TDSRRCRCR_WAIT_FOR_SRB_REL_CNF;
        tdsrrcrce_send_abort_cnf = TRUE;
      }
      break;

    case TDSRRC_NEW_CELL_IND:          
      /* The UE is already in CELL_FACH State. So Ignore the command since Cell
         Update would have been initiated */
      TDSRRC_MSG2(MSG_LEGACY_MED,"Unexpected event:%d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate); 
      break;

    case TDSRRC_SIG_CONN_RELEASE_CNF:
      /* Release the Signalling Radio Bearer irrespective of the status */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Status rcvd from SCRR: %d",cmd_ptr->cmd.rrc_sig_conn_cnf.status);

#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
      if(tdsrrc_csfb_extended_status == CSFB_CS_RB_RELEASED)
      {
        /* start the timer for UE before it can release RRC Connection */
        tdsrrctmr_start_timer(TDSRRCTMR_RRC_CON_DELAY_TIMER, 
          (unsigned long) tdsrrc_fast_return_to_lte_after_csfb_timer_nv); 
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"CSFB_HACK:Started wait for RRC conn release timer of value :%d ms",tdsrrc_fast_return_to_lte_after_csfb_timer_nv); 

        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE;
        break;
      }
#endif
      if (tdsrrcrce_sig_conn_rel_req_for_both_domains == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SigConRelCnf rcvd for first domain");
        /* Remain in this sub-state waiting for the next one */
        
        /* Set the following bool to FALSE so that previous
        processing could be done again */
        tdsrrcrce_sig_conn_rel_req_for_both_domains = FALSE;
        break;
      }
      /* Got the L2 ACK. Now check if the flag was set to TRUE then now release
         RRC connection by sending channel config req */

      /*Remove the check for signal delay ind from NAS and add delay timer unconditionally*/
#if 0
      if (tdsrrcrce_sig_con_delay == TRUE)
      {
        /* Reset the flag */
        tdsrrcrce_sig_con_delay = FALSE;
        
        if(!tdsrrcrcr_is_proc_active())
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending req to tear down RRC Connection");
          /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */           
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req ");    
          if(TDSRRCRCE_SUCCESS == 
             tdsrrcrce_send_rrc_initiate_cell_selection_ind(
             TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
          {     
            /* Wait for successful release of SRBs */
            tdsrrcrce_substate = TDSRRCRCR_WAIT_FOR_SRB_REL_CNF;                        
          }    
          else   /* Failed to send a internal command to RRC LLC.*/
          {      
            ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCR is active. Do not send Init Cell Sel Ind to CSP.");
        }
        break;
      }
      else
      {
        /*This is our feild optimization  */
        /* start the timer for UE with zero value */
        tdsrrctmr_start_timer(TDSRRCTMR_RRC_CON_DELAY_TIMER, 
          (unsigned long) TDSRRCRCE_TIMER_VALUE_FOR_SCRI_HOLDOFF);
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Started RRC con timer with %d ms",TDSRRCRCE_TIMER_VALUE_FOR_SCRI_HOLDOFF); 
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE;
      }
#else
      /*This is the requested behaviour from testing to leave 3s timer*/
      /* start the timer for UE before it can release RRC Connection */
      tdsrrctmr_start_timer(TDSRRCTMR_RRC_CON_DELAY_TIMER, 
        (unsigned long) TDSRRCRCE_TIMER_VALUE_SMALL_FOR_RRC_CON_REL);    
        
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Started RRC con smallar delay timer :%d",TDSRRCRCE_TIMER_VALUE_SMALL_FOR_RRC_CON_REL); 
      tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE;
#endif  /*#if 0*/

      

      break;
   
    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;

   }  
} /* tdsrrcrce_process_rce_wait_for_sig_con_rel_compl_substate*/
/*===========================================================================
FUNCTION tdsrrcrce_process_rce_wait_for_rrc_timer_expire_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE sub state 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_rrc_timer_expire_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{ 
  tdsrrc_cmd_type *rrcrce_close_sess_cmd_ptr;
  
  uint8 cnt = 0;
#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
  uint32 current_drx_cycle_ms = 0;
  tdsrrcllc_oc_set_status_e_type oc_status;
  oc_status = tdsrrcllc_get_ordered_config_status();
#endif 
  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
  case TDSRRC_DOWNLINK_SDU_IND:
  /* RRC Connection Setup or RRC Connection Reject may be received in this
    Substate because of cell-reselections. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Simultanous transactions not supported");
    break;
    
  case TDSRRC_STATE_CHANGE_IND:
    /* stop the rrc wait timer */
    tdsrrctmr_stop_timer(TDSRRCTMR_RRC_CON_DELAY_TIMER);
    /* Send RRC_ABORT_CNF command to the MM since RCE already in this substate
    since it has received TDSRRC_ABORT_REQ */
    
    if(tdsrrcrce_send_abort_cnf)
    {    
      if(TDSRRCRCE_FAILURE_MAJOR == 
      tdsrrcrce_send_rrc_abort_cnf() )      
    {        
        ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
      }        
    }
#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
    else if((tdsrrc_csfb_extended_status == CSFB_CS_RB_RELEASED) && 
        (cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED))
    {
      if(TDSRRCRCE_SUCCESS != tdsrrcrce_send_rrc_rel_ind())
      {
        ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
      }
    }
#endif      
      tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */

      tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
      
      /* Clear the ESTABLISHED_RABS while bringing down RRC Connection*/
      tdsrrc_clear_established_rabs();
      
      /* Reset Signalling Connections status */
      tdsrrcscr_reset_signalling_connection_status();
      tdsrrcrce_sig_conn_rel_req_for_both_domains = FALSE;
    break;
    
  case TDSRRC_NEW_CELL_IND:          
  /* The UE is already in CELL_FACH State. So Ignore the command since Cell
    Update would have been initiated */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Unexpected event:%d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate); 
    break;
    
  case TDSRRC_ABORT_REQ:
  /* Another TDSRRC_ABORT_REQ is received. Just set the flag again just in 
  * case it wasn't set before
    */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ in substate: %d,saved",tdsrrcrce_substate);
    tdsrrcrce_send_abort_cnf = TRUE;

    if(cmd_ptr->cmd.rrc_abort_req.hard_abort == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received HARD_ABORT flag from NAS");

      /* Send initiate cell sel command to CSP so that UE is taken to Idle */
      (void)tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCE, TDSRRC_TX_TO_DISCON_HARD_ABORT);

      /* stop the timer for UE before it can release RRC Connection */
      tdsrrctmr_stop_timer(TDSRRCTMR_RRC_CON_DELAY_TIMER);
    
      /* Wait for successful release of SRBs */
      tdsrrcrce_substate = TDSRRCRCR_WAIT_FOR_SRB_REL_CNF;
      tdsrrcrce_send_abort_cnf = TRUE;
    break;
    }
    /* For PS abort, stop timer and send SCRI if not already sent, or release the connection */
    /*If not PS abort, break*/
    if(cmd_ptr->cmd.rrc_abort_req.abort_cause == RRC_DUAL_STANDBY_PS_ABORT)
    {
      tdsrrctmr_stop_timer(TDSRRCTMR_RRC_CON_DELAY_TIMER);
    }
    else
    {
    break;
    }
    
  case TDSRRC_RRC_CON_DELAY_TIMER_EXPIRED_IND:  /*lint !e616 */
    /* Release the Signalling Radio Bearer irrespective of the status */
    if (tdsrrcrcr_is_proc_active())
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCR Active. Wait for st change ind");
      break;
    }
#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
    if(tdsrrc_csfb_extended_status == CSFB_CS_RB_RELEASED)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB_HACK: Conn release timer expired.");

      if(tdsrrccho_is_td2g_cco_active())
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB_HACK:TDSTOG CCO Active. Do not initialte cell selection");
        break;
      }
#ifdef FEATURE_WRLF_SYSTEM_SEL
      /* If OOS search is in progress, restart the connection release timer till
               1. UE comes back to service.
               2. Re-establishment timers expire and RRC connection is released */
      if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
      {
        /* start the timer for UE before it can release RRC Connection */
        tdsrrctmr_start_timer(TDSRRCTMR_RRC_CON_DELAY_TIMER, 
          (unsigned long) tdsrrc_fast_return_to_lte_after_csfb_timer_nv); 
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"CSFB_HACK:Re-Started wait for RRC conn release timer of value:%d ms,due to OOS",tdsrrc_fast_return_to_lte_after_csfb_timer_nv); 

        break;
      }
#endif

      /* In URA-PCH if we close the connection locally without the first page, then there is a chance of out of sync.So we should run the timer atleast for 1 page*/
      if (((tdsrrc_get_state() == TDSRRC_STATE_CELL_PCH) || (tdsrrc_get_state() == TDSRRC_STATE_URA_PCH) 
          ||(TDSOC_SET_FOR_DCH_CELL_PCH_TRANS == oc_status) 
          ||(TDSOC_SET_FOR_DCH_URA_PCH_TRANS == oc_status)
          ||(TDSOC_SET_FOR_FACH_CELL_PCH_TRANS == oc_status)
          ||(TDSOC_SET_FOR_FACH_URA_PCH_TRANS == oc_status))
          &&(tdsrrc_crt_restarted_once == FALSE)
          )
      { 
        current_drx_cycle_ms = 10*(tdsrrcllc_get_drx_info());

        if(current_drx_cycle_ms == 0)
        {
          current_drx_cycle_ms = TDSRRC_DEFAULT_DRX_CYCLE_MS;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"CSFB_HACK:Using default DRX cycle lenght (%d in ms)",TDSRRC_DEFAULT_DRX_CYCLE_MS);
        }

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"CSFB_HACK:Check  Restart wait %d %d %d",
          tdsrrc_fast_return_to_lte_after_csfb_timer_nv,current_drx_cycle_ms,TDSRRC_GUARD_TIMER_FOR_PCH_TRANS_BEFORE_WTOL_FAST_REDIR); 

        /* Guard timer value is 3s. 2sec for SIB reading during DCH->PCH trans and 1sec for CU after PAGE reception. */
        if ((TDSRRC_NUM_DRX_CYCLE_BEFORE_WTOL_FAST_REDIR * current_drx_cycle_ms) + TDSRRC_GUARD_TIMER_FOR_PCH_TRANS_BEFORE_WTOL_FAST_REDIR 
                                        > tdsrrc_fast_return_to_lte_after_csfb_timer_nv ) 
        {
           tdsrrctmr_start_timer(TDSRRCTMR_RRC_CON_DELAY_TIMER, 
             (unsigned long) ( (2*current_drx_cycle_ms) + TDSRRC_GUARD_TIMER_FOR_PCH_TRANS_BEFORE_WTOL_FAST_REDIR - tdsrrc_fast_return_to_lte_after_csfb_timer_nv )); 
           tdsrrc_crt_restarted_once = TRUE;
        }
        break;
      }
    }
#endif
    if (tdsrrcrce_sig_con_delay == TRUE)
    {
      tdsrrcrce_sig_con_delay = FALSE;
      /* Now Send Sig Connection Release Req */
      if(!tdsrrcscr_get_signalling_connection_status(RRC_CS_DOMAIN_CN_ID) && 
        !tdsrrcscr_get_signalling_connection_status(RRC_PS_DOMAIN_CN_ID )                                     )
      {
        /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
        if( TDSRRCRCE_SUCCESS ==
            tdsrrcrce_send_rrc_initiate_cell_selection_ind(
           TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
        {     
          /* Wait for successful release of SRBs */
          tdsrrcrce_substate = TDSRRCRCR_WAIT_FOR_SRB_REL_CNF;
        }    
        else   /* Failed to send a internal command to RRC LLC.*/
        {      
          ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
        }
      }
      else
      {
        if(tdsrrcscr_get_signalling_connection_status(RRC_CS_DOMAIN_CN_ID) )
        {
          if (tdsrrcrcr_is_proc_active())
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCR Active. SigConnRelReq not sent");
            break;
          }
          
          /* Allocates the buffer to RRC Internal command */
          if( (rrcrce_close_sess_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
          { 
            rrcrce_close_sess_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIG_CONN_RELEASE_REQ;
            rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.cn_domain_id =
              RRC_CS_DOMAIN_CN_ID;
            rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.
              sig_rel_complete_cnf_required = TRUE;
#if defined(FEATURE_3GPP_FAST_DORMANCY)
             rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc = TDSRRC_PROCEDURE_RCE;
#endif
            /* sends the command to Signalling Connection Release Request Proc */
            tdsrrc_put_int_cmd(rrcrce_close_sess_cmd_ptr);  
            cnt++;
          }
          else
          {
            ERR_FATAL("Mem alloc Failed",0,0,0);
          }
          
        }
        if(tdsrrcscr_get_signalling_connection_status(RRC_PS_DOMAIN_CN_ID) )
        {
          if (tdsrrcrcr_is_proc_active())
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCR Active. SigConnRelReq not sent");
            break;
          }
          
          /* Allocates the buffer to RRC Internal command */
          if( (rrcrce_close_sess_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
          { 
            rrcrce_close_sess_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIG_CONN_RELEASE_REQ;
            rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.cn_domain_id =
              RRC_PS_DOMAIN_CN_ID;
            rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.
              sig_rel_complete_cnf_required = TRUE;
#if defined(FEATURE_3GPP_FAST_DORMANCY)
            rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc = TDSRRC_PROCEDURE_RCE;
#endif
            /* sends the command to Signalling Connection Release Request Proc */
            tdsrrc_put_int_cmd(rrcrce_close_sess_cmd_ptr);  
            cnt++;
          }
          else
          {
            ERR_FATAL("Mem alloc Failed",0,0,0);
          }
          
        }
        if (cnt == 2)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SigConRelease req for both domains");
          tdsrrcrce_sig_conn_rel_req_for_both_domains = TRUE;
        }
        
        /* Wait for signalling connection release to complete */
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL;
        
      }
      
      break;
      
    }
    /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
    if( TDSRRCRCE_SUCCESS ==
        tdsrrcrce_send_rrc_initiate_cell_selection_ind(
       TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
    {     
      /* Wait for successful release of SRBs */
      tdsrrcrce_substate = TDSRRCRCR_WAIT_FOR_SRB_REL_CNF;                        
      
    }    
    else   /* Failed to send a internal command to RRC LLC.*/
    {      
      ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
    }
    break;
    
    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;
      
   }  
} /* tdsrrcrce_process_rce_wait_for_rrc_timer_expire_substate*/
/*===========================================================================
FUNCTION tdsrrcrce_process_rce_final_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in TDSRRCRCE_FINAL substate of either CELL_FACH 
  state or CELL_DCH State or CELL_PCH state or URA_PCH state.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_final_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  tdsrrc_cmd_type *rrcrce_close_sess_cmd_ptr;       /* Pointer to command to
                                                TDSRRC_SIG_CONN_RELEASE_REQ */
  tdsrrc_CellUpdateCause       cu_cause;
  tdsrrccu_cell_update_started_status_e_type cu_start_status;
  boolean soft_to_hard_abort = FALSE;
  uint32 cnt = 0;
  uint32 con_delay_timer = 0;

  switch (cmd_ptr->cmd_hdr.cmd_id)
  {

    case TDSRRC_STATE_CHANGE_IND:
      if((cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED) &&
         ( tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
      {
        if((TDSRRC_MODE_ACTIVE == tdsrrcmcm_get_rrc_mode())
          /* For inter-RAT redirection due to connection release, handle STATE_CHANGE_IND
           * in STANDBY_MODE as RRC releases all channels before starting redirection search
           */
           || ((TDSRRC_MODE_STANDBY == tdsrrcmcm_get_rrc_mode())
               && (tdsrrccsp_get_curr_scan() == TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN))
#ifdef FEATURE_WRLF_SYSTEM_SEL
           || (TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS() && 
                  (TDSRRC_MODE_INACTIVE == tdsrrcmcm_get_rrc_mode()))
#endif
          )
        {
          /* Check if abort is pending */
          if (tdsrrcrce_send_abort_cnf == TRUE) 
          {
            if(TDSRRCRCE_SUCCESS == tdsrrcrce_send_rrc_abort_cnf() )
            { 
              /* Clear the ESTABLISHED_RABS while bringing down RRC Connection*/
              tdsrrc_clear_established_rabs();
              
              /* Reset Signalling Connections status */
              tdsrrcscr_reset_signalling_connection_status();
              
              tdsrrcrce_substate = TDSRRCRCE_INITIAL;

              tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
              
            }
            else
            {
              ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
            }
          }
          else
          {
            if(TDSRRCRCE_SUCCESS == tdsrrcrce_send_rrc_rel_ind() )
            {
              /* Clear the ESTABLISHED_RABS while bringing down RRC Connection*/
              tdsrrc_clear_established_rabs();

              /* Reset Signalling Connections status */
              tdsrrcscr_reset_signalling_connection_status();

              tdsrrcrce_substate = TDSRRCRCE_INITIAL;

              tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
            }
            else
            {     
              ERR_FATAL("Unable to post an MM Cmd: %d",RRC_REL_IND,0,0);
            }  
          }
        }
        else
        {
          /* Don't send the REL_IND to MM */
          /* Clear the ESTABLISHED_RABS while bringing down RRC Connection*/
          tdsrrc_clear_established_rabs();

          /* Reset Signalling Connections status */
          tdsrrcscr_reset_signalling_connection_status();

          tdsrrcrce_substate = TDSRRCRCE_INITIAL;

          tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        }
      }
      else
      {
        ERR_FATAL("RRCSCMGR posted an unregistered event",0,0,0);
      }
      break;

    case TDSRRC_ABORT_REQ:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ABORT_REQ rcvd in %d substate",tdsrrcrce_substate);
      if((!tdsrrcscr_get_signalling_connection_status(RRC_CS_DOMAIN_CN_ID) && 
        !tdsrrcscr_get_signalling_connection_status(RRC_PS_DOMAIN_CN_ID)) ||
         (cmd_ptr->cmd.rrc_abort_req.auth_failed_bar_cells))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"No sig conn rel required. auth_failed = %d",cmd_ptr->cmd.rrc_abort_req.auth_failed_bar_cells);
        /* Do not send command to LLC to release SRBs Instead send Initiate Cell Selection Indication */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Init Cell Sel Ind to CSP instead Channel Config Req");    
        if( TDSRRCRCE_SUCCESS ==
           tdsrrcrce_send_rrc_initiate_cell_selection_ind(
           TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
        {     
          /* Wait for successful release of SRBs */
          tdsrrcrce_substate = TDSRRCRCR_WAIT_FOR_SRB_REL_CNF;
          tdsrrcrce_send_abort_cnf = TRUE;
        }    
        else   /* Failed to send a internal command to RRC LLC.*/
        {      
          ERR_FATAL("Unable send internal cmd to CSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
        }
      }
      else
      {
        /*When receiving ABORT_REQ (soft ABORT) from NAS, RRC checks if all the following conditions are met:
         1.	TDS in DSDS mode;
         2.	AND RLF has been declared;
         3.	AND TDS does not have the lock;
         If yes,  it performs hard abort .
         Because when RRC get the lock in soft abort, RRC will send cu and scri, this delay the abort procedure.*/
        if ((tds_is_ue_in_dual_standby)&&(FALSE == (tdrm_check_cli_state_is_in_lock(TDRM_CLIENT_RRC))))
        {
          cu_start_status = tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause);
          
          if ( (TDSRRCCU_STARTED == cu_start_status) && (tdsrrc_CellUpdateCause_radiolinkFailure == cu_cause) )
          {
            soft_to_hard_abort = TRUE;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSDS:Change soft abort to hard abort");
          }
        }
		if((cmd_ptr->cmd.rrc_abort_req.hard_abort == TRUE)||(TRUE == soft_to_hard_abort))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received HARD_ABORT flag from NAS");

          /* Send initiate cell sel command to CSP so that UE is taken to Idle */
          (void)tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCE, TDSRRC_TX_TO_DISCON_HARD_ABORT);

          /* Wait for successful release of SRBs */
          tdsrrcrce_substate = TDSRRCRCR_WAIT_FOR_SRB_REL_CNF;
          tdsrrcrce_send_abort_cnf = TRUE;
          break;
        }

  
        if(tdsrrctmr_get_remaining_time(TDSRRCTMR_SOFT_ABORT_GUARD_TIMER)==0)
		{
			/*process Soft-Abort*/
			/*Start protection timer for cannot get SCRI sent*/
			tdsrrctmr_start_timer(TDSRRCTMR_SOFT_ABORT_GUARD_TIMER,
							   (unsigned long) TDSRRCRCE_SOFT_ABORT_GUARD_TIMER_VAL);
			TDSRRC_MSG1(MSG_LEGACY_HIGH,"Started RRC TDSRRCTMR_SOFT_ABORT_GUARD_TIMER: %d",TDSRRCRCE_SOFT_ABORT_GUARD_TIMER_VAL); 
		}
        else
		{
			TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC TDSRRCTMR_SOFT_ABORT_GUARD_TIMER has been started in LTA"); 
		}

        /* make sure the TDSRRCRCE_TIMER_VALUE_FOR_SCRI_HOLDOFF time gap reached before sending SigConRelInd OTA msg */
        if((TDSRRCRCE_SOFT_ABORT_GUARD_TIMER_VAL-tdsrrctmr_get_remaining_time(TDSRRCTMR_SOFT_ABORT_GUARD_TIMER))
			    < TDSRRCRCE_TIMER_VALUE_FOR_SCRI_HOLDOFF)
	{
	  con_delay_timer = TDSRRCRCE_TIMER_VALUE_FOR_SCRI_HOLDOFF-
	       (TDSRRCRCE_SOFT_ABORT_GUARD_TIMER_VAL-tdsrrctmr_get_remaining_time(TDSRRCTMR_SOFT_ABORT_GUARD_TIMER));
        }
     
        if(tdsrrcscr_get_signalling_connection_status(RRC_CS_DOMAIN_CN_ID) )
        {
          tdsrrcrce_send_abort_cnf = TRUE;
          if (tdsrrcrcr_is_proc_active())
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCR Active. SigConnRelReq not sent");
            break;
          }
          /* atleast one of the signalling connection was up */
          if (cmd_ptr->cmd.rrc_abort_req.delay_sig_con_release == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"NAS flag for RRCConnRel TRUE");
            tdsrrcrce_sig_con_delay = TRUE;
            /*Keep the original logic*/
#if 0

            /*This will set delay for some test box cannot handle SCRI*/
            /* start the timer for UE to camp on */
            tdsrrctmr_start_timer(TDSRRCTMR_RRC_CON_DELAY_TIMER, 
                               (unsigned long) TDSRRCRCE_TIMER_VALUE_FOR_SCRI_HOLDOFF);
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Started RRC con timer with %d ms",TDSRRCRCE_TIMER_VALUE_FOR_SCRI_HOLDOFF); 
#else
            /*This will set delay for some test box cannot handle SCRI*/
            /* start the timer for UE to camp on */
            if(con_delay_timer>0)
			{
			   tdsrrctmr_start_timer(TDSRRCTMR_RRC_CON_DELAY_TIMER,(unsigned long)con_delay_timer);
			   TDSRRC_MSG1(MSG_LEGACY_HIGH,"Started RRC con timer with %d ms",con_delay_timer); 

			   tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE;

			   break;
			}
            else
			{
                tdsrrcrce_sig_con_delay = FALSE;
				/* Allocates the buffer to RRC Internal command */
				if( (rrcrce_close_sess_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
				{ 
				  rrcrce_close_sess_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIG_CONN_RELEASE_REQ;
				  rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.cn_domain_id =
					RRC_CS_DOMAIN_CN_ID;
				  rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.
					sig_rel_complete_cnf_required = TRUE;
#if defined(FEATURE_3GPP_FAST_DORMANCY)
				  rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc = TDSRRC_PROCEDURE_RCE;
#endif
				  /* sends the command to Signalling Connection Release Request Proc */
				  tdsrrc_put_int_cmd(rrcrce_close_sess_cmd_ptr);  
				  cnt++;
				}
				else
				{
				  ERR_FATAL("Mem alloc Failed",0,0,0);
				}
			}
#endif
          }
          else
          {
            /* Allocates the buffer to RRC Internal command */
            if( (rrcrce_close_sess_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
            { 
              rrcrce_close_sess_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIG_CONN_RELEASE_REQ;
              rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.cn_domain_id =
                RRC_CS_DOMAIN_CN_ID;
              rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.
                sig_rel_complete_cnf_required = TRUE;
#if defined(FEATURE_3GPP_FAST_DORMANCY)
              rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc = TDSRRC_PROCEDURE_RCE;
#endif
              /* sends the command to Signalling Connection Release Request Proc */
              tdsrrc_put_int_cmd(rrcrce_close_sess_cmd_ptr);  
              cnt++;
            }
            else
            {
              ERR_FATAL("Mem alloc Failed",0,0,0);
            }
          }
        }
        if(tdsrrcscr_get_signalling_connection_status(RRC_PS_DOMAIN_CN_ID) )
        {
          tdsrrcrce_send_abort_cnf = TRUE;
          if (tdsrrcrcr_is_proc_active())
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCR Active. SigConnRelReq not sent");
            break;
          }
          /* atleast one of the signalling connection was up */
          if (cmd_ptr->cmd.rrc_abort_req.delay_sig_con_release == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"NAS flag for RRCConnRel TRUE");
            tdsrrcrce_sig_con_delay = TRUE;
             /*Keep the original logic*/
#if 0
            /*This will set delay for some test box cannot handle SCRI*/
            /* start the timer for UE to camp on */
            tdsrrctmr_start_timer(TDSRRCTMR_RRC_CON_DELAY_TIMER, 
                               (unsigned long) TDSRRCRCE_TIMER_VALUE_FOR_SCRI_HOLDOFF);
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Started RRC con timer with %d ms",TDSRRCRCE_TIMER_VALUE_FOR_SCRI_HOLDOFF); 
           
#else 
            /*This will set delay for some test box cannot handle SCRI*/
            /* start the timer for UE to camp on */
            if(con_delay_timer>0)
			{
			   tdsrrctmr_start_timer(TDSRRCTMR_RRC_CON_DELAY_TIMER,(unsigned long)con_delay_timer);
			   TDSRRC_MSG1(MSG_LEGACY_HIGH,"Started RRC con timer with %d ms",con_delay_timer); 

			   tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE;
			
			   break;
			}
			else
			{
			    tdsrrcrce_sig_con_delay = FALSE;
            /* Allocates the buffer to RRC Internal command */
				if( (rrcrce_close_sess_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
				{ 
				  rrcrce_close_sess_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIG_CONN_RELEASE_REQ;
				  rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.cn_domain_id =
				    RRC_PS_DOMAIN_CN_ID;
				  rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.
				    sig_rel_complete_cnf_required = TRUE;
#if defined(FEATURE_3GPP_FAST_DORMANCY)
				  rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc = TDSRRC_PROCEDURE_RCE;
#endif
				  /* sends the command to Signalling Connection Release Request Proc */
				  tdsrrc_put_int_cmd(rrcrce_close_sess_cmd_ptr);  
				  cnt++;
				}
				else
				{
				  ERR_FATAL("Mem alloc Failed",0,0,0);
				}
          	}
#endif
          }
          else
          {
            /* Allocates the buffer to RRC Internal command */
            if( (rrcrce_close_sess_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
            { 
              rrcrce_close_sess_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIG_CONN_RELEASE_REQ;
              rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.cn_domain_id =
                RRC_PS_DOMAIN_CN_ID;
              rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.
                sig_rel_complete_cnf_required = TRUE;
#if defined(FEATURE_3GPP_FAST_DORMANCY)
              rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc = TDSRRC_PROCEDURE_RCE;
#endif
              /* sends the command to Signalling Connection Release Request Proc */
              tdsrrc_put_int_cmd(rrcrce_close_sess_cmd_ptr);  
              cnt++;
            }
            else
            {
              ERR_FATAL("Mem alloc Failed",0,0,0);
            }
          }
        }
        if (cnt == 2)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SigConRelease req for both domains");
          tdsrrcrce_sig_conn_rel_req_for_both_domains = TRUE;
        }
        /* Wait for signalling connection release to complete */
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL;
        
        tdsrrcrce_send_abort_cnf = TRUE;
        if(cmd_ptr->cmd.rrc_abort_req.abort_cause == RRC_DUAL_STANDBY_PS_ABORT)
        {
          tdsrrcrce_sig_con_delay = TRUE;
        }
      }
      
      break; 

    case TDSRRC_DOWNLINK_L2ACK_IND:
     
       if(cmd_ptr->cmd.downlink_l2ack_ind.status == SUCCESS)
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2 ack for RRC ConnSetCompl rcvd");

         /* Send RRC_EST_CNF command to MM */
         if(TDSRRCRCE_SUCCESS == 
           tdsrrcrce_send_rrc_est_cnf() )      
         {
           /* The RCE Procedure goes to FINAL substate */
           tdsrrcrce_substate = TDSRRCRCE_FINAL;   
           
           /* Reset the tdsrrcrce_num_retries_count */
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting tdsrrcrce_num_retries_count to 0");
           tdsrrcrce_num_retries_count = 0;
           
           /* Initial release cause to Unspecified */
           tdsrrc_release_cause = RRC_REL_CAUSE_UNSPEC;

           /* Reset V300 */
           tdsrrc_v_300 = 0;
           TDSRRC_DEBUG_INCREMENT_STATS(TDSRRC_RCE_SUCCEED);
         }    
         else
         { 
           ERR_FATAL("Unable to post MM Cmd: %d",RRC_EST_CNF,0,0);
         }
         
         tdsrrcrce_l2_ack_rcv_for_rcsc = TRUE;
       }
       else
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failed to get L2 ack RRC ConnSetCompl");
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failed to send MM RRC_EST_CNF due to not receiving L2 ack for RRCConnSetCompl");
       }
      
       break;

#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
    case TDSRRC_INTERNAL_CSFB_ABORT_REQ:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CSFB_HACK:RRC_INTERNAL_CSFB_ABORT_REQ rcvd in %d substate",tdsrrcrce_substate);

      if(tdsrrcscr_get_signalling_connection_status(RRC_PS_DOMAIN_CN_ID))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB_HACK:PS domain open,send SCRI.");

        /* Allocates the buffer to RRC Internal command */
        if( (rrcrce_close_sess_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
        { 
          rrcrce_close_sess_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIG_CONN_RELEASE_REQ;
          rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.cn_domain_id =
            RRC_PS_DOMAIN_CN_ID;
          rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.
            sig_rel_complete_cnf_required = TRUE;
          rrcrce_close_sess_cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc = TDSRRC_PROCEDURE_RCE;
          /* sends the command to Signalling Connection Release Request Proc */
          tdsrrc_put_int_cmd(rrcrce_close_sess_cmd_ptr);  
        }
  
        /* Wait for signalling connection release to complete */
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB_HACK:PS domain not open.");

        /* start the timer for UE before it can release RRC Connection */
        tdsrrctmr_start_timer(TDSRRCTMR_RRC_CON_DELAY_TIMER, 
                (unsigned long) tdsrrc_fast_return_to_lte_after_csfb_timer_nv); 
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"CSFB_HACK:Started wait for RRC conn release timer of value :%d ms",tdsrrc_fast_return_to_lte_after_csfb_timer_nv); 
        
        tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE;
      }
      tdsrrcrce_sig_con_delay = FALSE;
      break;
#endif
    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrce_substate);
      break;
  }  

} /* tdsrrcrce_process_rce_final_substate */

/*===========================================================================
FUNCTION tdsrrcrce_get_rrc_connection_status

DESCRIPTION
  This function looks into RRC Connection Establishment provcedure's 
  sub-states and returns the status of RRC Connection.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_rrc_conn_status_e_type: Returns TDSRRCRCE_RRC_CONN_ESTABLISHED if RRC
  Connection Establishment procedure is in TDSRRCRCE_FINAL substate. Otherwise
  it returns TDSRRCRCE_RRC_CONN_NOT_ESTABLISHED.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_rrc_conn_status_e_type tdsrrcrce_get_rrc_connection_status
(
  void                  
)
{
  if((tdsrrcrce_substate != TDSRRCRCE_FINAL)
    &&(tdsrrcrce_substate != TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE))
  {
    /* RRC Connection is not established */
    return ( TDSRRCRCE_RRC_CONN_NOT_ESTABLISHED );
  }
  else
  {
    /* RRC Connection is established */
    return ( TDSRRCRCE_RRC_CONN_ESTABLISHED );
  }

} /* tdsrrcrce_get_rrc_connection_status */
/*===========================================================================
FUNCTION tdsrrcrce_is_rce_procedure_inactive

DESCRIPTION
  This function looks into RRC Connection Establishment provcedure's 
  sub-states and returns the status of RRC Connection Establishment
  Procedure.

DEPENDENCIES
  None

RETURN VALUE
  boolean: TRUE if RCE is still inactive. (Note - when RCE is waiting for 
           UE to camp, the function will still return TRUE) 
           FALSE when RCE is not in Initial substate and UE is camped on 
           a cell.

SIDE EFFECTS
  To be used only by CSP as the logic is designed for CSP's requirement.
===========================================================================*/
boolean  tdsrrcrce_is_rce_procedure_inactive(void)
{
  if ((tdsrrcrce_substate == TDSRRCRCE_INITIAL) ||
      (tdsrrcrce_substate == TDSRRCRCE_WAIT_FOR_UE_CAMP_ON))
  {
    /* RCE will not be considered "active" if in one
     * of these substates.
     */
    return(TRUE);
  }
  else
  {
    return(FALSE);
  }
} /*tdsrrcrce_is_rce_procedure_inactive */

/*===========================================================================
FUNCTION tdsrrcrce_change_substate_to_rrcrce_final

DESCRIPTION
  This function changes the substate of RRC Connection Establishment provcedure's 
  to TDSRRCRCE_FINAL

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  To be used only during G2W Handover and should not be used for any other 
  purpose.
===========================================================================*/
void tdsrrcrce_change_substate_to_rrcrce_final
(
  void
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Changing substate %d to Final",tdsrrcrce_substate);
  tdsrrcrce_substate = TDSRRCRCE_FINAL;

   tdsrrcrce_send_abort_cnf = FALSE;
   /* Reset the tdsrrcrce_num_retries_count */
   TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting tdsrrcrce_num_retries_count to 0");
   tdsrrcrce_num_retries_count =0;
}

/*===========================================================================
FUNCTION rrcrce_validate_access_class

DESCRIPTION
  This function validates access class of CSP

DEPENDENCIES
  None

RETURN VALUE
  rrcrce status

SIDE EFFECTS
  None
===========================================================================*/

tdsrrcrce_status_e_type tdsrrcrce_validate_cell_access_status
(
  rrc_cn_domain_identity_e_type   cn_domain_id,
  rrc_establish_cause_e_type   establishment_cause,
  rrc_abort_cause_e_type  *cause              /* cause */
)
{
  mm_as_cell_access_status_T cell_access_status;
  tdsrrcrce_status_e_type status = TDSRRCRCE_FAILURE_MINOR;

  /*Get the current cell access status */
  cell_access_status = tdsrrccsp_get_curr_cell_access_status(cn_domain_id);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell access status from CSP internal database=%d ",cell_access_status);
  
  switch (cell_access_status)
  {
    case MM_AS_ACCESS_ALL_CALLS:
      {
        status = TDSRRCRCE_SUCCESS;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell Access allowed:MM_AS_ACCESS_ALL_CALLS,status=%d",status);
      }
      break;

    case MM_AS_ACCESS_EMERGENCY_ONLY:
      if(establishment_cause == RRC_EST_EMERGENCY_CALL)
      {
        status = TDSRRCRCE_SUCCESS;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell Acces allowed:MM_AS_ACCESS_EMERGENCY_ONLY,status=%d",status);
      }
      else
      {
        *cause = RRC_ACCESS_CLASS_NOT_ALLOWED;
        status = TDSRRCRCE_FAILURE_MINOR;
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Cell Acces not allowed for regular voice calls status=%d,cause=%d ",status,*cause);
      }
      break;

    case MM_AS_ACCESS_NO_CALLS:
    {
      *cause = RRC_ACCESS_CLASS_NOT_ALLOWED;
      status = TDSRRCRCE_FAILURE_MINOR;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Cell Acces not allowed for any calls status=%d,cause=%d ",status,*cause);
    }
    break;

    case MM_AS_ACCESS_NORMAL_ONLY:
      if(establishment_cause != RRC_EST_EMERGENCY_CALL)
      {
        status = TDSRRCRCE_SUCCESS;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Acces allowed for regular voice calls");
      }
      else
      {
        *cause = RRC_ACCESS_CLASS_NOT_ALLOWED;
        status = TDSRRCRCE_FAILURE_MINOR;
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Cell Access not allowed for the calls other than normal/regular voice calls status=%d,cause=%d ",status,*cause);
      }
    break;
  }
  return status;
}

/*===========================================================================
FUNCTION tdsrrcrce_force_initialize

DESCRIPTION
  This function changes the substate of RRC Connection Establishment provcedure's 
  to TDSRRCRCE_INITIAL and sends the required Indication/Response to MM depending
  on RRCRCE substate. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  To be used only during Mode Change and when an Out of Service Area is indicated
  to MM while in IDLE DISCONNECTED state.
  It should not be used for any other purpose.
===========================================================================*/

void tdsrrcrce_force_initialize
(
  boolean initiate_cell_selection
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Force Initialize.Changing substate %d to Initial",tdsrrcrce_substate);
      tdsrrcrce_allow_ta_flag = FALSE;

#ifdef FEATURE_INTERRAT_PCCO_G2TD
  if(tdsrrccho_is_g2td_pcco_in_progress())
  {
    /* No need to send the CONN_SETUP_IND to CHO */
    /* set rce_substate to initial */
    tdsrrcrce_substate = TDSRRCRCE_INITIAL;

    tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;

    /* Reset the tdsrrcrce_num_retries_count */
    tdsrrcrce_num_retries_count = 0;

    tdsrrcrce_send_abort_cnf = FALSE;

    return;
  }
#endif /* FEATURE_INTERRAT_PCCO_G2TD */
  
  if(tdsrrcrce_substate == TDSRRCRCE_INITIAL)
  {
    tdsrrcrce_substate = TDSRRCRCE_INITIAL;

    tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
    /* Reset the tdsrrcrce_num_retries_count */
    tdsrrcrce_num_retries_count = 0;

    tdsrrcrce_send_abort_cnf = FALSE;
  }
  else if((tdsrrcrce_substate == TDSRRCRCE_WAIT_FOR_UE_CAMP_ON) ||
          (tdsrrcrce_substate == TDSRRCRCE_WAIT_FOR_SIB7_CURRENT))
  {
    if(tdsrrcrce_substate == TDSRRCRCE_WAIT_FOR_SIB7_CURRENT)
    {
      /* Call SIB fn to forcefully initialize its active event if any */
      tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_SIB7_UPDATE);
    }
    else
    {
      /* Stop the timer to UE to camp on */  
      tdsrrctmr_stop_timer(TDSRRCTMR_UE_TO_CAMP_ON_TIMER);
    }

    /* Send RRC_ABORT_IND command to the MM */
    if(TDSRRCRCE_SUCCESS == 
       tdsrrcrce_send_rrc_abort_ind(RRC_FAIL_TO_CAMP_ON) )      
    {
      /* The RCE Procedure ends */
      tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */

      tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
      /* Reset the tdsrrcrce_num_retries_count */
      tdsrrcrce_num_retries_count = 0;

      tdsrrcrce_send_abort_cnf = FALSE;
    }    
    else
    {     
      ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_IND,0,0);
    }       
  }
  else if (tdsrrcrce_substate == TDSRRCRCE_WAIT_FOR_RB0_REL_CNF)
  {
    if (tdsrrcrce_send_abort_cnf == TRUE)
    {
      /* Send RRC_ABORT_CNF command to the MM */
      if(TDSRRCRCE_SUCCESS == 
        tdsrrcrce_send_rrc_abort_cnf() )      
      {        
      /* At this time, none of the RBs are setup. Simply reset the RCE
        substate */
        tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */

        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        /* Reset the tdsrrcrce_num_retries_count */
        tdsrrcrce_num_retries_count = 0;
        
        return;
      }    
      else
      {     
        ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
      }  
    }
    else
    {
      /* Send RRC_ABORT_IND command to the MM */
      if(TDSRRCRCE_SUCCESS == 
        tdsrrcrce_send_rrc_abort_ind(RRC_FAILURE_UNSPECIFIED) )      
      {
        /* The RCE Procedure ends */
        tdsrrcrce_substate = TDSRRCRCE_INITIAL;    /* reset the RCE substate */

        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        /* Reset the tdsrrcrce_num_retries_count */
        tdsrrcrce_num_retries_count = 0;
        
        return;
      }    
      else
      {     
        ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_IND,0,0);
      }
    }
  }
  else if (tdsrrcrce_substate == TDSRRCRCR_WAIT_FOR_SRB_REL_CNF)
  {
    /* Send RRC_ABORT_CNF command to the MM */
    if(TDSRRCRCE_SUCCESS == 
      tdsrrcrce_send_rrc_abort_cnf() )      
    {       
    /* At this time, none of the RBs are setup. Simply reset the RCE
      substate */
      tdsrrcrce_substate = TDSRRCRCE_INITIAL; /* The RCE Procedure ends */

      tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
      /* Reset the tdsrrcrce_num_retries_count */
      tdsrrcrce_num_retries_count = 0;
      
      /* Call this fn to make sure that Establised RABS contains invalid values */
      tdsrrc_clear_established_rabs();
     
      return;
    }    
    else
    {     
      ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_CNF,0,0);
    }  
  }
  else if ((tdsrrcrce_substate == TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP) ||
           (tdsrrcrce_substate == TDSRRCRCE_WAIT_FOR_RESELCT_COMPL) ||
           (tdsrrcrce_substate == TDSRRCRCE_WAIT_FOR_REDIRECT_CNF) ||
           (tdsrrcrce_substate == TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF))
  {
    if(initiate_cell_selection)
    {
      /* Send RRC_ABORT_IND command to the MM */
      if(TDSRRCRCE_SUCCESS == 
         tdsrrcrce_send_rrc_abort_ind(RRC_FAIL_TO_CAMP_ON) )
      {
        /* Stop the timer to UE to camp on */  
        tdsrrctmr_stop_timer(TDSRRCTMR_T_300_TIMER);
        tdsrrcrce_abort_ind_sent = TRUE;
        /* Release the RB 0 and request the RRC LLC to go to Idle disconnected */
        if( TDSRRCRCE_SUCCESS == 
           tdsrrcrce_send_rrc_initiate_cell_selection_ind(
          TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED) )
        {
          /* The RCE Procedure ends */
          tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; /* change the RCE substate */
        }
        else    /* Failed to send a internal command to RRC LLC.*/
        {
          ERR_FATAL("Unable send internal cmd toCSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
        }
      }    
      else
      {
        ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_IND,0,0);
      }
    }
    else
    {
      /* Send RRC_ABORT_IND command to the MM */
      if(TDSRRCRCE_SUCCESS == 
         tdsrrcrce_send_rrc_abort_ind(RRC_MAX_ACCESS_FAILURE) )
      {
        /* Stop the timer to UE to camp on */  
        tdsrrctmr_stop_timer(TDSRRCTMR_T_300_TIMER);
        tdsrrcrce_substate = TDSRRCRCE_INITIAL;

        tdsrrcrce_l2_ack_rcv_for_rcsc = FALSE;
        /*Set tdsrrcrce_abort_ind_sent to FALSE as RCE is moving to initial substate*/
        tdsrrcrce_abort_ind_sent = FALSE;
      }    
      else
      {
        ERR_FATAL("Unable to post an MM Cmd: %d",RRC_ABORT_IND,0,0);
      }
    }
    /* Reset the tdsrrcrce_num_retries_count */
    tdsrrcrce_num_retries_count = 0;
    tdsrrcrce_send_abort_cnf = FALSE;
    tdsrrcrce_lai_rai_mismatch = FALSE;
  }    
}

/*===========================================================================

FUNCTION   tdsrrcrce_get_sib16_ptr

DESCRIPTION

  This function gets the Sib16 ptr either from the actual sib16 or the
  SIB16 version of Default Configuration
  
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrc_SysInfoType16 pointer

SIDE EFFECTS

  None.

===========================================================================*/
struct tdsrrc_SysInfoType16 * tdsrrcrce_get_sib16_ptr()
{
  struct tdsrrc_SysInfoType16 * sib16_ptr = NULL;

  if(tdsrrcrce_default_config_info.config_type == TDSRRC_DEFAULT_CONFIG)
  {
    sib16_ptr = tdsrrcrce_default_config_info.sib16_ptr;
  }

  return sib16_ptr;
}

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*===========================================================================

FUNCTION   tdsrrcrce_get_rbs_ptr

DESCRIPTION

  This function gets the RBS ptr either from the actual RBS or the
  RBS version of Default Configuration
  
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrc_DL_DCCH_Message pointer

SIDE EFFECTS

  None.

===========================================================================*/
struct tdsrrc_DL_DCCH_Message * tdsrrcrce_get_rbs_ptr(void)
{
  struct tdsrrc_DL_DCCH_Message * rbs_ptr = NULL;

  if(tdsrrcrce_default_config_info.config_type == TDSRRC_DEFAULT_CONFIG)
  {
    rbs_ptr = tdsrrcrce_default_config_info.rbs_ptr;
  }

  return rbs_ptr;
}
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

/*===========================================================================

FUNCTION   tdsrrcrce_get_rb_setup_r6_ptr_for_dc

DESCRIPTION

  This function gets the default config ptr from RB setup r6 I.E.
  
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrc_RadioBearerSetup_r6_IEs pointer

SIDE EFFECTS

  None.

===========================================================================*/
struct tdsrrc_RadioBearerSetup_r6_IEs * tdsrrcrce_get_rb_setup_r6_ptr_for_dc
(
  void
)
{
  tdsrrc_RadioBearerSetup_r6_IEs * rb_setup_r6_ptr = NULL;

  if(tdsrrcrce_default_config_info.config_type == TDSRRC_DEFAULT_CONFIG)
  {
    if(tdsrrcrce_default_config_info.dcch_msg_ptr != NULL)
    {
      rb_setup_r6_ptr = &(tdsrrcrce_default_config_info.dcch_msg_ptr->message.u.radioBearerSetup.
          u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.radioBearerSetup_r6);
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Default config ptr val = %x for HSPA",rb_setup_r6_ptr);
  
  return rb_setup_r6_ptr;
}
/*===========================================================================

FUNCTION   tdsrrcrce_is_dc_config_on_hspa

DESCRIPTION

  This function will indicate if DC will configure RB's/SRB's on HSPA channel's
  i.e. EUL in UL & HSDPA on DL. Currently only DC-17 does config SRB's on 
  HSPA channel's.
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE -If DC will configure RB's/SRB's on HSPA channel's
  FASLE - other wise
  
SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrce_is_dc_config_on_hspa
(
  void
)
{
  boolean is_dc_config_hspa = FALSE;

  /*DC-17 config's SRB's on HSPA channel's.*/
  if((tdsrrcrce_default_config_info.config_type == TDSRRC_DEFAULT_CONFIG)&&
    ((tdsrrcrce_default_config_info.default_config_id == TDSRRC_DEF_CONFIG_17)
             || (tdsrrcrce_default_config_info.default_config_id == TDSRRC_DEF_CONFIG_23)
    ))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DC I.D =%d config on HSPA channels",tdsrrcrce_default_config_info.default_config_id );
    is_dc_config_hspa = TRUE;
  }

  return is_dc_config_hspa;
}

/*===========================================================================
FUNCTION tdsrrcrce_get_rrc_est_cause

DESCRIPTION
  Returns establishment cause

DEPENDENCIES
  None

RETURN VALUE
  Returns establishment cause

SIDE EFFECTS
  None
===========================================================================*/
rrc_establish_cause_e_type tdsrrcrce_get_rrc_est_cause
(
  void
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Est Cause is %d",tdsrrc_establishment_cause);
  return tdsrrc_establishment_cause;      
} /* tdsrrcrce_get_rrc_est_cause */
#ifdef FEATURE_TDSCDMA_MTPL_THERMAL_BACKOFF 
/*===========================================================================
FUNCTION tdsrrcrce_set_rrc_est_cause_for_emergency_call

DESCRIPTION
  Sets the establishment cause to RRC_EST_EMERGENCY_CALL

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_set_rrc_est_cause_for_emergency_call
(
  void
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Est Cause is %d",tdsrrc_establishment_cause);
  tdsrrc_establishment_cause = RRC_EST_EMERGENCY_CALL;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Est Cause is %d",tdsrrc_establishment_cause);
} /* rrcrce_get_rrc_est_cause */
#endif
/*===========================================================================
FUNCTION tdsrrcrce_is_rce_procedure_inactive

DESCRIPTION
  This function looks into RRC Connection Establishment provcedure's 
  sub-states and returns the status of RRC Connection Establishment
  Procedure.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if RCE is in active. 
  FALSE: Otherwise 
SIDE EFFECTS
none
===========================================================================*/
boolean  tdsrrcrce_is_rce_procedure_active(void)
{
  if(tdsrrcrce_substate == TDSRRCRCE_INITIAL)
  {
    /* RCE will not be considered "active" if in one
     * of these substates.
     */
    return(FALSE);
  }
  else
  {
    return(TRUE);
  }
} /*tdsrrcrce_is_rce_procedure_active */
/*===========================================================================
FUNCTION tdsrrcrce_get_rce_sub_state()

DESCRIPTION
  This function will return current RRC Connection Establishment 
  procedure's sub state.
DEPENDENCIES
  None

RETURN VALUE
  return current rce sub state.
SIDE EFFECTS
none
===========================================================================*/
tdsrrcrce_substate_e_type  tdsrrcrce_get_rce_sub_state(void)
{
  return tdsrrcrce_substate;
} /*tdsrrcrce_get_rce_sub_state*/

/*===========================================================================
FUNCTION tdsrrcrce_check_for_rai_mismatch()

DESCRIPTION
  This function compares the camped on LAI/RAI with the one given in buffered EST_REQ to decide
  on whether to proceed with EST_REQ or reject it.
DEPENDENCIES
  None

RETURN VALUE
  TRUE: Mismatch and reject the buffered EST_REQ.
SIDE EFFECTS
none
===========================================================================*/
boolean tdsrrcrce_check_for_rai_mismatch(rrc_plmn_identity_type camped_plmn)
{
  rrc_rai_type    camped_on_rai;
  /* This stores the GSM-MAP NAS system info */
  rrc_gsm_map_nas_sys_info_type   nas_common_info;
  /* Pointer to SIB1 */
  tdsrrc_SysInfoType1 *sib1_ptr;
  unsigned char  *tdsdata_ptr;
  uint32         i;
  boolean compare_rai = FALSE;

  /* Fill up the NAS system info here */
  /* Let us first read SIB1 - we should have SIB1 for sure */
  sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB1);

  if( sib1_ptr == NULL )
  {
    // SIB1 for the camped cell is not present in the SIB Database.
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"SIB1 for MCC %d, MNC %d,is not present",TDSRRC_CSP_GET_INT_MCC(camped_plmn),TDSRRC_CSP_GET_INT_MNC(camped_plmn));
    return FALSE;
  } // SIB1 not present 

  /* The SIB1 is present */
  nas_common_info.length = (uint32)sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts;

  /* Check if the RRC MM interface structure array can hold this info */
  if(sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts > RRC_MAX_GSM_INFO_OCTET_COUNT)
  {
   TDSRRC_MSG1(MSG_LEGACY_ERROR,"Not enough space for Common NAS Sys Info (%d bytes)",sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts);
   return FALSE;
  }

  /* Fill in the Common NAS system info now */
  tdsdata_ptr = sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.data;
  for( i = 0; i < sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i ++)
  {
    nas_common_info.value[i] =  *tdsdata_ptr;
    tdsdata_ptr ++;
  }
 
  /* Get the LAI status from local utility */
  /* TBD: Call MM utility to initialize lai first. */
  if(mm_extract_lac_from_sib_info(&camped_on_rai.lai, &nas_common_info) == FALSE)
  {
   TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not extract LAI from NAS Common sys info");
   return FALSE;
  }

  /* Since MM doesn't know the PLMN from NAS system info, fill in the PLMN now */
  camped_on_rai.lai.plmn = camped_plmn;

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Camped on PLMN: %d-%d ",TDSRRC_CSP_GET_INT_MCC(camped_on_rai.lai.plmn)
                            ,TDSRRC_CSP_GET_INT_MNC(camped_on_rai.lai.plmn)
                            ,0);

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"LAC: %x %x",camped_on_rai.lai.lac[0],camped_on_rai.lai.lac[1]);

  if(tdsrrc_est_cn_domain_id != RRC_CS_DOMAIN_CN_ID)
  {
    compare_rai = TRUE;

    /* Get the RACs from NAS Domain Specific Info */
    if(tdsrrc_extract_rac_from_sib_info(&camped_on_rai, &sib1_ptr->cn_DomainSysInfoList) == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not extract RAC from NAS Domain Specific Info");
      return FALSE;
    }
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAC: %d",camped_on_rai.rac[0]);
  }

  if((camped_on_rai.lai.plmn.num_mnc_digits ==
       tdsrrc_est_req_for_rai.lai.plmn.num_mnc_digits ) &&

     tdsrrcrce_compare_arrays(camped_on_rai.lai.plmn.mcc,
                           tdsrrc_est_req_for_rai.lai.plmn.mcc,
                           RRC_MAX_MCC_DIGITS)  &&
     tdsrrcrce_compare_arrays(camped_on_rai.lai.plmn.mnc,
                           tdsrrc_est_req_for_rai.lai.plmn.mnc,
                           tdsrrc_est_req_for_rai.lai.plmn.
                           num_mnc_digits)  &&
     tdsrrcrce_compare_arrays(camped_on_rai.lai.lac,
                           tdsrrc_est_req_for_rai.lai.lac,
                           RRC_MAX_GSMMAP_LAC_LENGTH) 
    )
  {
    if(!compare_rai)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LAI MATCHED");
      return FALSE;
    }
    else if(tdsrrcrce_compare_arrays(camped_on_rai.rac,
                             tdsrrc_est_req_for_rai.rac,
                             RRC_MAX_GSMMAP_RAC_LENGTH))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RAI MATCHED");
      return FALSE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"LAI/RAI mismatch");
    tdsrrcrce_lai_rai_mismatch = TRUE;
  }
  return TRUE;
}

/*===========================================================================
FUNCTION    tdsrrcrce_send_conn_rel_cause_event 

DESCRIPTION
  This function will log the event named EVENT_TDSCDMA_RRC_CONN_REL_CAUSE to diag task.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_send_conn_rel_cause_event
(
  uint8 conn_rel_cause
)
{
  tdsrrclog_conn_rel_cause_event_type conn_rel_cause_event;
  int8 self_hosting_offset;

  switch(conn_rel_cause) 
  {
    case (uint8)RRC_FAIL_TO_CAMP_ON:
      conn_rel_cause_event.conn_rel_cause = 0;
    event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case (uint8)RRC_INTERNAL_ERROR:
      conn_rel_cause_event.conn_rel_cause = 1;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;
  
    case (uint8)RRC_UNSUPPORTED_REDIRECTION:
      conn_rel_cause_event.conn_rel_cause = 2;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case (uint8)RRC_MAX_ACCESS_FAILURE:
      conn_rel_cause_event.conn_rel_cause = 3;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);    
      break;

    case (uint8)RRC_FAILURE_DUE_TO_CONGESTION:
      conn_rel_cause_event.conn_rel_cause = 4;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case (uint8)RRC_FAILURE_UNSPECIFIED:
      conn_rel_cause_event.conn_rel_cause = 5;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case (uint8)RRC_UNSUPPORTED_INITIAL_UE_ID:
      conn_rel_cause_event.conn_rel_cause = 6;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case (uint8)RRC_ACCESS_CLASS_NOT_ALLOWED:
      conn_rel_cause_event.conn_rel_cause = 7;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case (uint8)RRC_CONNECTION_NOT_AVAILABLE:
      conn_rel_cause_event.conn_rel_cause = 8;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case (uint8)RRC_PCCO_FAILURE:
      conn_rel_cause_event.conn_rel_cause = 9;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case (uint8)RRC_NO_RESOURCE_AVAILABLE:
      conn_rel_cause_event.conn_rel_cause = 10;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case (uint8)RRC_ABORT_LAI_RAI_CHANGED:
      conn_rel_cause_event.conn_rel_cause = 11;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case ((uint8)RRC_ABORT_CAUSE_TYPE_MAX+(uint8)RRC_REL_CAUSE_NORMAL):
      conn_rel_cause_event.conn_rel_cause = 12;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
#ifdef FEATURE_QSH_EVENT_METRIC
      tdsrrcqsh_update_connection_release(TDSRRC_NORMAL_EVENT);
#endif 
      break;

    case ((uint8)RRC_ABORT_CAUSE_TYPE_MAX+(uint8)RRC_REL_CAUSE_UNSPEC):
      conn_rel_cause_event.conn_rel_cause = 13;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
#ifdef FEATURE_QSH_EVENT_METRIC
      tdsrrcqsh_update_connection_release(TDSRRC_UNSPECIFIED);
#endif       
      break;

    case ((uint8)RRC_ABORT_CAUSE_TYPE_MAX+(uint8)RRC_REL_CAUSE_PRE_EMPTIVE):
      conn_rel_cause_event.conn_rel_cause = 14;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
#ifdef FEATURE_QSH_EVENT_METRIC
      tdsrrcqsh_update_connection_release(TDSRRC_PRE_EMPTIVE_RELEASE);
#endif       
      break;

    case ((uint8)RRC_ABORT_CAUSE_TYPE_MAX+(uint8)RRC_REL_CAUSE_CONGESTION):
      conn_rel_cause_event.conn_rel_cause = 15;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
#ifdef FEATURE_QSH_EVENT_METRIC
      tdsrrcqsh_update_connection_release(TDSRRC_CONGESTION);
#endif       
      break;

    case ((uint8)RRC_ABORT_CAUSE_TYPE_MAX+(uint8)RRC_REL_CAUSE_RE_ESTABLISH_REJECT):
      conn_rel_cause_event.conn_rel_cause = 16;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
#ifdef FEATURE_QSH_EVENT_METRIC
      tdsrrcqsh_update_connection_release(TDSRRC_REESTABLISHMENT_REJECT);
#endif   
      break;

    case ((uint8)RRC_ABORT_CAUSE_TYPE_MAX+(uint8)RRC_REL_USER_INACTIVITY):
      conn_rel_cause_event.conn_rel_cause = 17;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
#ifdef FEATURE_QSH_EVENT_METRIC
      tdsrrcqsh_update_connection_release(TDSRRC_USER_INACTIVITY);
#endif 
      break;

    case ((uint8)RRC_ABORT_CAUSE_TYPE_MAX+(uint8)RRC_REL_DEEP_FADE):
      conn_rel_cause_event.conn_rel_cause = 18;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case ((uint8)RRC_ABORT_CAUSE_TYPE_MAX+(uint8)RRC_DIRECTED_SIGNALLING_REESTABLISHMENT):
      conn_rel_cause_event.conn_rel_cause = 19;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
#ifdef FEATURE_QSH_EVENT_METRIC
      tdsrrcqsh_update_connection_release(TDSRRC_DIRECTED_SIGNALING_CONNECTION_REESTABLISHMENT);
#endif 
      break;

    case ((uint8)RRC_ABORT_CAUSE_TYPE_MAX+(uint8)RRC_REL_CAUSE_OOS):
      conn_rel_cause_event.conn_rel_cause = 20;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case ((uint8)RRC_ABORT_CAUSE_TYPE_MAX+(uint8)RRC_UE_INIT_DORMANCY_UE_IN_OOS):
      conn_rel_cause_event.conn_rel_cause = 21;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case ((uint8)RRC_ABORT_CAUSE_TYPE_MAX+(uint8)RRC_UE_INIT_DORMANCY_ABORT_SUCCESS):
      conn_rel_cause_event.conn_rel_cause = 22;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    case ((uint8)RRC_ABORT_CAUSE_TYPE_MAX+(uint8)RRC_REL_DUAL_STANDBY_PS_ABORT_SUCCESS):
      conn_rel_cause_event.conn_rel_cause = 23;
      event_report_payload(EVENT_TDSCDMA_RRC_CONN_REL_CAUSE, sizeof(conn_rel_cause_event),
                       (void *)&conn_rel_cause_event);
      break;

    default:
	  conn_rel_cause_event.conn_rel_cause = 0xFF;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid release cause type %d",conn_rel_cause);
      break;
  }

	/* only mark failure related causes */
  self_hosting_offset = (conn_rel_cause_event.conn_rel_cause == 15) ? 0 :    /* 3 RRC_REL_CAUSE_CONGESTION */
	                    ((conn_rel_cause_event.conn_rel_cause == 16) ? 1 :  /* 4 RRC_REL_CAUSE_RE_ESTABLISH_REJECT */
                        ((conn_rel_cause_event.conn_rel_cause == 18) ? 2 :  /* 6 RRC_REL_DEEP_FADE */
						((conn_rel_cause_event.conn_rel_cause == 20) ? 3 :  /* 8 RRC_REL_CAUSE_OOS */
						  	-1)));
	if (self_hosting_offset > 0)
	{
	  TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
		                         TDS_SELF_HOSTING_MAJOR_CALL_RETENTION_EXCEPTION,
		                         (TDS_SELF_HOSTING_MINOR_RRC_CONN_REL_BASE + (uint8)self_hosting_offset));
	}
  }

/*===========================================================================
FUNCTION    tdsrrcrce_send_conn_req_cause_event 

DESCRIPTION
  This function will log the event named EVENT_WCDMA_CONN_REQ_CAUSE to diag task.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_send_conn_req_cause_event
(
  uint8 conn_req_cause
)
{
  tdsrrclog_conn_req_cause_event_type conn_req_cause_event;
  if(conn_req_cause <= RRC_EST_TERM_UNKNOWN)
  {
    conn_req_cause_event.conn_req_cause = conn_req_cause;
    event_report_payload(EVENT_TDSCDMA_RRC_CONN_REQ_CAUSE, sizeof(conn_req_cause_event),
                       (void *)&conn_req_cause_event);
  }
}

/*===========================================================================

FUNCTION tdsrrcrce_trim_rce_msg_ul_tm_tb_size

DESCRIPTION
  This procedure is the event handler for the RRC CONNECTION REQUEST procedure.


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

boolean tdsrrcrce_trim_rce_msg_ul_tm_tb_size
(
  tdsrrc_UL_CCCH_Message *msg_ptr,
  uint32 pdu_length
)
{
  boolean status = FALSE;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RCE trimming bit Mask val =%x",tdstrim_rce_msg_status);
  /*These If conditions should be in sequence, below is the spec section which indicates that
    *8.5.23 Measured results on RACH
    *1>for messages transmitted on CCCH, take care that the maximum allowed message size
    *    is not exceeded when forming the IE "Measured results on RACH", i.e. limit the number 
    *    of included neighbour cells or if required omit the IE "Measured results on RACH" altogether. 
    *   When limiting the number of included neighbouring cells, 
    *   the number of inter-frequency cells should be limited first i.e. inter-frequency cells should
    *   be omitted before limiting  the number of intra- frequency cells.
    */
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
  if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)&&
     ((tdstrim_rce_msg_status & TDSRRCRCE_REMOVED_INTERF_MEAS_RPT_ON_RACH) 
                       == TDSRRCRCE_TRIM_REQ_BIT_MASK_NOT_SET))
  {
    /*Call meas function which will remove some or all cell info for Inter Freq Measurment on RACH*/
    (void)tdsrrcifreq_adjust_ccch_message_length((void *)msg_ptr,pdu_length);
     /*This Bit mask is used to indicate some or all cell info for Inter Freq Measurment on RACH 
     is removed in rce message*/
     tdstrim_rce_msg_status |= TDSRRCRCE_REMOVED_INTERF_MEAS_RPT_ON_RACH;
     status = TRUE;
  }
  else
#endif
  if((tdstrim_rce_msg_status & TDSRRCRCE_REMOVED_MONITORED_CELL_FROM_MEAS_RPT_ON_RACH) 
                       == TDSRRCRCE_TRIM_REQ_BIT_MASK_NOT_SET)
  {
    tdsrrc_MeasuredResultsOnRACH * rrcmeas_rach_report_ptr = NULL;  /* pointer to struct of type tdsrrc_MeasuredResultsOnRACH */
    tdsrrc_MonitoredCellRACH_List * curr_monitcells_ptr = NULL;  /* pointer to tdsrrc_MeasuredResultsOnRACH used in freeing a node*/
    

    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionRequest,tdsrrc_RRCConnectionRequest,measuredResultsOnRACH))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"rrc_rrcConnectionRequest_measuredResultsOnRACH_present bit mask set");
      rrcmeas_rach_report_ptr = &msg_ptr->message.u.rrcConnectionRequest.measuredResultsOnRACH;
      if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrcmeas_rach_report_ptr,monitoredCells))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"rrc_monitoredCells_present bit mask set");
        /*Exclude monitored cell results in meas rpt on rach*/
        TDSRRC_RESET_MSG_IE_PRESENT_PTR(rrcmeas_rach_report_ptr);
        curr_monitcells_ptr = &rrcmeas_rach_report_ptr->monitoredCells;

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Number of monitored cell = %d",curr_monitcells_ptr->n);
        curr_monitcells_ptr->n = 0;
        rtxMemFreePtr(&tdsenc_ctxt,curr_monitcells_ptr->elem);
       
      }
    }
    /*Set REMOVE_MONITORED_CELL_FROM_MEAS_RPT_ON_RACH in tdstrim_cu_msg_status*/
    tdstrim_rce_msg_status |= TDSRRCRCE_REMOVED_MONITORED_CELL_FROM_MEAS_RPT_ON_RACH;
    status = TRUE;
  }
  else if((tdstrim_rce_msg_status & TDSRRCRCE_REMOVED_ENTIRE_MEAS_RPT_ON_RACH) 
                       == TDSRRCRCE_TRIM_REQ_BIT_MASK_NOT_SET)
  {
    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.rrcConnectionRequest,
		tdsrrc_RRCConnectionRequest,measuredResultsOnRACH))
    {
      /*Disable bit mask*/
      msg_ptr->message.u.rrcConnectionRequest.m.measuredResultsOnRACHPresent =0;
    }
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Removed measuredResultsOnRACH I.E from rce,bit mask ");
    tdstrim_rce_msg_status |= TDSRRCRCE_REMOVED_ENTIRE_MEAS_RPT_ON_RACH;
    status = TRUE;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"MAX possible trim on RCE done,lets discard this msg");
    status = FALSE;
  }
  return status;
}

/*===========================================================================

FUNCTION tdsrrcrce_reset_trim_bit_mask

DESCRIPTION
  This procedure Reset RRC CONNECTION REQUEST Trim bit mask.


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcrce_reset_trim_bit_mask
(
void
)
{
  /*Reset RRC CONNECTION REQUEST Trim bit mask*/
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE reset Trim bit mask ");
  tdstrim_rce_msg_status = tdstrim_rce_msg_status & TDSRRCRCE_TRIM_REQ_BIT_MASK_NOT_SET;
}
/*===========================================================================

FUNCTION tdsrrcrce_get_rcsc_status

DESCRIPTION
  This function get status whether ACK is received for RRC Connection setup 
  complete .


DEPENDENCIES
  None.

RETURN VALUE
  TRUE: ACK is received for RRC Connection setup complete.

===========================================================================*/
boolean tdsrrcrce_get_rcsc_status(void)
{
  return tdsrrcrce_l2_ack_rcv_for_rcsc;
}

#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
/*===========================================================================

FUNCTION tdsrrcrce_post_internal_csfb_abort_cmd

DESCRIPTION
  This function posts internal command to trigger RRC connection.
   abort when the CSFB CS call ends.


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcrce_post_internal_csfb_abort_cmd(void)
{
  tdsrrc_cmd_type *int_cmd_ptr;

  if (tdsrrcrcr_is_proc_active())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCR Active. Internal abort not sent");
    return;
  }

  if((int_cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_INTERNAL_CSFB_ABORT_REQ",0,0,0);
  }
  
  /* Fill in the command id  */
  int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_INTERNAL_CSFB_ABORT_REQ;
  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( int_cmd_ptr );
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Posted internal cmd TDSRRC_INTERNAL_CSFB_ABORT_REQ");
  tdsrrc_csfb_extended_status = CSFB_CS_RB_RELEASED;
}
#endif
#ifdef  FEATURE_TDSCDMA_SPECIFIC_SIB_READING
/*===========================================================================

FUNCTION tdsrrcrce_is_waiting_for_sib7_update

DESCRIPTION
   This function is used to check whether it is waiting for SIB7 udpate.


DEPENDENCIES
  TRUE : RRC is waiting for SIB7

RETURN VALUE
  None.

===========================================================================*/
boolean tdsrrcrce_is_waiting_for_sib7_update(void)
{
  if (tdsrrcrce_substate == TDSRRCRCE_WAIT_FOR_SIB7_CURRENT)
  {
    return TRUE;
  }
  return FALSE;
}

#endif
/*===========================================================================

FUNCTION tdsrrcrce_get_rrc_abort_cnf_status

DESCRIPTION
   This function is used to get the rrc_abort_cnf status.


DEPENDENCIES
  TRUE : rrc need to send the abort_cnf

RETURN VALUE
  None.

===========================================================================*/
boolean tdsrrcrce_get_rrc_abort_cnf_status(void)
{
  return tdsrrcrce_send_abort_cnf;
}
/*===========================================================================

FUNCTION tdsrrcrce_get_rce_ta_allow_status

DESCRIPTION
   This function is used to get the tdsrrcrce_allow_ta_flag status.


DEPENDENCIES
  TRUE : rrc allow the TA at connecting state.

RETURN VALUE
  None.

===========================================================================*/
boolean tdsrrcrce_get_rce_ta_allow_status(void)
{
  return tdsrrcrce_allow_ta_flag;
}

/*===========================================================================

FUNCTION tdsrrcrce_process_connection_rej_with_wait_time

DESCRIPTION
  This function processes connection rejection req with wait time:
  1)connection for registration,go to idle and let NAS try
  2)connection for ps signaling,change lock reason from ACCESS to BG_TRAFFIC

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_connection_rej_with_wait_time()
{
  /* Move to idle only for below EST cause because there is no NAS retry for other EST cause */ 
  if((tdsrrc_establishment_cause == RRC_EST_REGISTRATION) ||
    (tdsrrc_establishment_cause == RRC_EST_CALL_RE_ESTABLISH) ||
    (tdsrrc_establishment_cause == RRC_EST_INTER_RAT_CELL_RESELECT))
  {
    if( TDSRRCRCE_SUCCESS == 
        tdsrrcrce_send_rrc_initiate_cell_selection_ind(
        TDSRRC_TRANSITION_TO_DISCONNECTED, TDSRRC_STATE_DISCONNECTED))
    {                            
      /* The RCE Procedure ends */
      tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_RB0_REL_CNF; 

      /* Set Abort Cause */
      tdsrrcrce_mm_abort_cause = RRC_ABORT_CONN_REJECT_WAIT_TMR;
    }
    else
    {
      ERR_FATAL("Unable send internal cmd to CSP: %d",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
    }
  }
  else
  {
    tdsrrctmr_start_timer(TDSRRCTMR_WAIT_TIME_TIMER, tdsrrcrce_wait_time * 1000);  
    tdsrrcrce_substate = TDSRRCRCE_WAIT_FOR_WAIT_TIMER_TO_EXPIRE;
    /* Change the priority to BG_TRAFFIC to allow another sub to recover from OOS or perform CM activities*/
    if ((SYS_PROC_TYPE_PS_CALL_SIGNALING == tdsrrc_proc_type)||
       (SYS_PROC_TYPE_SMS == tdsrrc_proc_type)||
       (SYS_PROC_TYPE_TEST_LOOPBACK == tdsrrc_proc_type))
    {
      tdsrrc_wrm_change_priority(TRM_BG_TRAFFIC, FALSE);
      tdrm_retain_lock(TDRM_CLIENT_RRC);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM:Change priority to BG TRF during wait_time after conn rej");
    }
  }
}

/*===========================================================================

FUNCTION tdsrrcrce_is_good_freq_exist_in_mset

DESCRIPTION
  This function check whether exist good frequency in MSET


DEPENDENCIES
  None.

RETURN VALUE
  TURE: exist good freq rscp > bar_cell_rscp_threshold
  FALSE: no freq rscp > bar_cell_rscp_threshold

===========================================================================*/
boolean tdsrrcrce_is_good_freq_exist_in_mset(void)
{
  tdsl1_mset_list_type tdsl1_mset_list;

  memset(&tdsl1_mset_list,0,sizeof(tdsl1_mset_list_type));

  tdsl1_get_mset_list(&tdsl1_mset_list);

  for (int i = 0; i< tdsl1_mset_list.num_freq;i++)
  {
    if (tdsl1_mset_list.freq[i]!= tdsrrcccm_get_curr_camped_freq())
    {
      if (tdsl1_mset_list.rscp[i] > tdsrrcrce_nv_est_fail_bar_cell.bar_cell_rscp_threshold)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Exist better frequency:%d,RSCP:%d",tdsl1_mset_list.freq[i],tdsl1_mset_list.rscp[i]);
        return TRUE;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Doesn't Exist better frequency RSCP >%d dbm",tdsrrcrce_nv_est_fail_bar_cell.bar_cell_rscp_threshold);
        return FALSE;
      }
    }
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Only serv freq or NULL freq in MSET");
  return FALSE;
}