/*===========================================================================
  R R C   R A D I O   B E A R E R   R E C O N F I G   M O D U L E 

DESCRIPTION

  This module performs the Radio Bearer Reconfiguration Procedure. This
  module processes the Radio Bearer Reconfig Message and appropriately
  reconfigures the radio bearers given by the message. 
  
  The Radio Bearer Reconfig Message may reconfig both signalling radio bearers
  and radio bearers that are mapped to radio access bearers. 
  It may set also re-configure previously established radio bearers.
  The procedure may also initiate a hard-handover if the Radio Bearer
  Reconfig Message indicates a frequency change.
  
  
EXTERNALIZED FUNCTIONS

  tdsrrcrbrc_procedure_event_handler   Event handler for this procedure
  tdsrrcrbrc_init_procedure            Power-up initialization of the procedure

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Procedure must be initialized at power-up by calling tdsrrcrbrc_init_procedure. 
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrbreconfig.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/14/12   as      Added code to support feature NB/WB handover support
11/05/01   ysh     delete check IE "rb_InformationReconfigList " in R4 msg. Jira504.
08/09/11   mkh     Reverting the changes of naming the api made earlier to fix 
                   compile issue on 9x15
08/09/11   mkh     Compile fix on 9x15 due to enabling of FEATURE_TDSCDMA_VOC_AMR_WB
                   Removing prev changes of naming. Will revert in next revision.
06/16/11   bj      Added code to handle receive R9 RadioBearreconfigMessage.
03/10/11   bj      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/18/10   ysh     Frequency info save. 3GPP-8.6.6.1(3.4.2)
                                  Save Primary freq when enter Non-CELL_DCH state.
11/11/10   yzh     (3GPP: 8.6.6.27)Del tdsrrcllcpcie_initialise_sync_a_post_veri_info()
                   when IE Downlink information common for all radio links(FDD->TDD).
11/10/10   bj       Deleted IE "RNC support for change of UE capability".(3GPP: 8.6.2.3)
10/30/10   ysh     IE"Downlink information for each radio link" info save.  3GPP-8.6.6.4(3.2.13).
10/29/10   zwj    Added code to handle receive R4 RadioBearreconfigMessage.

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
#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "rabmtask.h"
#include "rex.h"
#include "tdsrrcasn1.h"
#include "tdsrrcasn1util.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcdata.h"
#include "tdsrrcintif.h"
#include "tdsrrcccm.h"
#include "tdsrrccu.h"
#include "tdsrrccui.h"
#include "tdsrrclbt.h"
#include "tdsrrclcm.h"
#include "tdsrrcllc.h"
#include "tdsrrcmm_v.h"
#include "rrcmnif.h"
#include "tdsrrcrbcommon.h"
#include "tdsrrcrbreconfig.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcsend.h"
#include "tdsrrccspi.h"
#include "tdsrrccsp.h"
#include "tdsrrctmr.h"
#if(defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) || \
  defined(FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING))
#include "tdsrrcsibproc.h"
#endif
#include "tdsrrcsmc.h"
#include "tdsrrclog.h"

#include "tdsrrciho.h"
#include "tdsrrcmisc.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsrrcdormancy.h"

#include "tdsrrcllcoc.h"
#include "tdsCFAParser_i.h"
#include "tdsrrcnv.h"
#include "tdsrrclogging.h"
#include "tdsrrcqsh.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* Enumerated type of the different substates for this procedure           */
#if 0
typedef enum
{
  RRCRBRC_INITIAL,                   /* Initial Substate                    */
  RRCRBRC_WAIT_FOR_CELL_UPDATE_CNF,  /* Wait for cell update                */
  RRCRBRC_WAIT_FOR_CELL_SELECTION_CNF,  /* wait for cell selection      */
  RRCRBRC_WAIT_FOR_CHAN_CFG_CNF,     /* Waiting for chan cfg cnf from LLC   */                      
  RRCRBRC_WAIT_FOR_CRNTI_UPDATE_CNF,     /* Waiting for CRNTI update cnf   */                      
  RRCRBRC_WAIT_FOR_L2_ACK,           /* Waiting for Layer 2 ack for uplink
                                    message transmission                   */
  RRCRBRC_WAIT_FOR_REESTABLISH_SRB2, /*waiting for re-establishment of srb2 */
  RRCRBRC_MAX_SUBSTATES
}rrcrbrc_substate_e_type;
#endif

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
/* We need to introduce a delay before transmitting
on a new configuration if the base-station cannot have
the old link and new link up at the same time. */
  #define TDSRRCRBRC_TEMP_TIMER_SIG 0x4001
rex_sigs_type tdsrrcrbrc_sig;
rex_timer_type tdsrrcrbrc_temp_timer;

#ifndef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
typedef enum
{
  TDSRB_STOP_CONTINUE_ABSENT=0,
  TDSRB_STOP,
  TDSRB_CONTINUE
}tdsrrcrbrc_rlc_stop_cont_e_type;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
typedef struct
{
  boolean is_valid;     /* indicates if there are RBs that need to be 
                           released/affected. If this is FALSE, the two
                           items below are not used                       */
  uint32 num_rbs;       /* Number of RBs released by the message          */
  tdsrrc_RB_Identity rb_id[TDS_MAX_RB]; /* Hold the ids of the RBs that are
                           released/affected by the RBR message           */
  /* Indicate if for this ie, stop or continue is specified. */
  tdsrrcrbrc_rlc_stop_cont_e_type  rb_stop_continue[TDS_MAX_RB]; 
  uint16 ul_cfg_mask[TDS_MAX_RB];
  uint16 dl_cfg_mask[TDS_MAX_RB];
}tdsrrcrbrc_rb_info_type;



/* Structure to store all internal data related to RBRC procedure           */
typedef struct
{
  tdsrrcrb_substate_e_type         curr_substate;  /* Store the current substate     */
  tdsrrcrb_transaction_id_type     tr_ids;      /* Transaction ids for RR msg    */
  tdsrrc_state_e_type              state_indicated_by_msg; /* RRC State given by RR msg     */
  tdsrrcrb_msg_failure_cause_type  status;   /* Status of message validation   */
  tdsrrcrbrc_rb_info_type          rbs_tobe_reconfigured;/* RBs to be reconfigured*/
  tdsrrcrbrc_rb_info_type          rbs_tobe_affected; /* RBs to be affected       */
  tdsrrcrb_rab_info_per_msg_type   rabs;      /* Hold info about each rab in one
                                         RB RC message till they're processed*/
  boolean                       directed_cell_info; /* Indicates if a valid C-RNTI was recd */
  boolean                       new_crnti_valid; /* Indicates if a valid C-RNTI was recd 
                                                  * in the message transitioning from CELL_DCH
                                                  * to CELL_FACH */
  uint16                        new_crnti;
  tdsrrc_int_u_rnti_type           new_urnti;
  boolean                       rbrc_for_srns_relocation;
  boolean                       cn_info_saved;
  mm_cmd_type *                 mm_cmd_ptr;
#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER    
  boolean                       srns_reallocation_trigger;
#endif
  boolean                       hho_ciph_status;

  boolean                       trans_to_dch_ciph_config_needed;
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
 /* Indicates if Fre-redirection is to be triggered in FACH->PCH case */
  boolean initiate_cell_selection_trans_from_fach;
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

  boolean high_mobility_ind;

}tdsrrcrbrc_int_data_type;
#endif
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrcrbrc_int_data_type tdsrrc_rbrc;


boolean tdsrrcrbrc_send_initiate_cu_flag = FALSE;
boolean tdsrrcrbrc_delete_crnti = FALSE;

#ifdef FEATURE_UMTS_PDCP
extern boolean tdsrrc_pdcp_enabled;
#endif /* FEATURE_UMTS_PDCP*/
#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER  
extern tdsrrc_ccm_cell_camping_info_type   tdsrrc_ccm_cell_camping_info;
#endif
/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */




/* =======================================================================
**                            Function Definitions
** ======================================================================= */
#ifdef FEATURE_UMTS_PDCP
/*===========================================================================

FUNCTION   RRCRBRC_IS_SRNS_IN_PROGRESS()

DESCRIPTION
 This function returns if SRNS is in progress with current reconfig procedure


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrbrc_is_srns_in_progress
(
  void
)
{
  return tdsrrc_rbrc.rbrc_for_srns_relocation;
}
#endif
/*===========================================================================

FUNCTION   RRCRBRC_GET_NEW_CRNTI_VALID()

DESCRIPTION
 This function returns if reconfig msg has valid CRNTI


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrbrc_get_new_crnti_valid
(
  void
)
{
  return tdsrrc_rbrc.new_crnti_valid;
}
 /*===========================================================================

FUNCTION   RRCRBRC_GET_CURRENT_SUB_STATE()

DESCRIPTION
 This function is used to return current substate


        
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrcrb_substate_e_type current_substate.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_substate_e_type tdsrrcrbrc_get_current_sub_state( )
{
  return tdsrrc_rbrc.curr_substate;
}

/*===========================================================================

FUNCTION   tdsrrcrbrc_configure_rab_change()

DESCRIPTION
 This function re-configures RABs and other lower layer parameters for VT -> Voice 
 and Voice -> VT fallback for RB-Reconfig OTA msg. Also, if SCUDIF is not happening, then
 this function checks/configures for vocoder/rate change for Voice RAB. 
        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_configure_rab_change(void)
{
  uint8 rab_index =0;

#ifdef FEATURE_SCUDIF_ENHANCEMENT
  #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/

  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"No. of RABs in RBRC %d",tdsrrc_rbrc.rabs.num_rabs_in_msg);
#ifdef FEATURE_SCUDIF_ENHANCEMENT
  #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/
  {
    for(rab_index =0;rab_index <tdsrrc_rbrc.rabs.num_rabs_in_msg; rab_index++)
    {
      if(tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB)
      {
        nas_sync_ind = tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator;
        break;
      }
    }
    
    
    tdsrrcrb_update_amr_cctrch_id_for_reconfig(nas_sync_ind);
  }
}


/*===========================================================================

FUNCTION tdsrrcrbrc_append_start_list_to_rbrc_complete

DESCRIPTION
  This function appends start list to tdsrrc_rbrc message for SRNS relocation and
  also when TM bearers are present during non-dch to dch rrc-state
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcrbrc_append_start_list_to_rbrc_complete
(
tdsrrc_UL_DCCH_Message *msg_ptr               /* Pointer to Uplink DCCH Msg */
)
{
  /*start list is included in ul_counter_synchronization_info. So enable this IE in the
  bitmask */
  TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.radioBearerReconfigurationComplete,
    tdsrrc_RadioBearerReconfigurationComplete,ul_CounterSynchronisationInfo); 
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.radioBearerReconfigurationComplete.ul_CounterSynchronisationInfo);
  /* Allocate memory for START List */
  /* call SMC API to append the start list */
  if (SUCCESS != tdsrrcsmc_append_start_list(&msg_ptr->message.u.radioBearerReconfigurationComplete.ul_CounterSynchronisationInfo.startList,
                                          TDSRRCSMC_EXTRACT_AND_APPEND_START))
  {
    /*should not come here */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not append Start list");
  } 
}
/*===========================================================================

FUNCTION   SEND_RB RC_FAILURE_MSG

DESCRIPTION

  This function sends the Radio Bearer Reconfiguration Failure message
  on Uplink DCCH. The failure cause must be given to this
  function. If the failure cause is "Protocol Error" the
  protocol error cause must also be given to this function.

        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
static void tdsrrcrbrc_send_rbrc_failure_message
(
tdsrrcrb_msg_status_e_type failure_cause,  /* Failure Cause - should be a subset
                                        of the failure causes specified by
                                        the spec. */
tdsrrc_ProtocolErrorCause  prot_err_cause  /* Protocol Error cause. This is checked
                                        only if the Failure cause is set to 
                                        "Protocol Error". */
)
{

  tdsrrc_UL_DCCH_Message *ptr;     /* Pointer to uplink DCCH message type */
  rlc_lc_id_type ul_lc_id; /* Logical Channel Id for the message  */
  tdsrrcsend_status_e_type status; /* Status from send chain module       */

  /* Allocate memory for this message. Note that this buffer is released
  by the SEND CHAIN */
  ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc( 
                                          sizeof(tdsrrc_UL_DCCH_Message));
  if (ptr == NULL)
  {
    ERR_FATAL("No memory for RB Reconfig Failure msg!",0,0,0);
  }

  /* Reset SVTHHO in case of failure */
  tdsrrcsmc_reset_start_value_to_transmit_cs_hho(FALSE);
  TDSRRC_RESET_MSG_IE_PRESENT_PTR(ptr); /* Integrity Check info not present */
  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_radioBearerReconfigurationFailure; /* RR Failure msg */

  /* Set the bit-mask -  ?? */
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReconfigurationFailure);

  /* Transaction Id - First check the rejected_transacion id. If
  it's not invalid, this message is being rejected because there
  is already another message accepted but not processed.
  If the rejected_transaction id is invalid, we are rejecting the
  accepted message, so use the accepted transaction id.

  NOTE: Don't change the order, it's important to check rejected
  transaction first. */
  if ( tdsrrc_rbrc.tr_ids.rejected_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {

    ptr->message.u.radioBearerReconfigurationFailure.rrc_TransactionIdentifier =
    tdsrrc_rbrc.tr_ids.rejected_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_rbrc.tr_ids.rejected_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  } 
  else if ( tdsrrc_rbrc.tr_ids.accepted_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.radioBearerReconfigurationFailure.rrc_TransactionIdentifier =
    tdsrrc_rbrc.tr_ids.accepted_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_rbrc.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  } 
  else
  {
    /* Don't know the transaction id for this message.( one reason
    for this could be that the message had a critical
    extenstion). Since it's unknown, we should set it to zero. */
    ptr->message.u.radioBearerReconfigurationFailure.rrc_TransactionIdentifier= 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalide Transaction id for RBRF message!");
  }

  switch ( failure_cause )
  {
  case TDSRRCRB_MSG_UNSUPPORTED_CONFIG:
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
    break;

  case TDSRRCRB_PHY_CHAN_FAILURE:
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_physicalChannelFailure;
    break;

  case TDSRRCRB_SIMULTANEOUS_RECONFIG:
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_incompatibleSimultaneousReconfiguration;
    break;

  case TDSRRCRB_MSG_PROTOCOL_ERROR:
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_protocolError;

    /* When there is a Protocol Error, we also have to indicate
    the protocol error cause */
     ptr->message.u.radioBearerReconfigurationFailure.failureCause.u.protocolError=
	  	rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation);
    
    if(ptr->message.u.radioBearerReconfigurationFailure.failureCause.u.protocolError == NULL)
    {
      ERR_FATAL("Memory allocation failure",0,0,0);
    }

    ptr->message.u.radioBearerReconfigurationFailure.failureCause.u.protocolError->
      diagnosticsType.t = T_tdsrrc_ProtocolErrorInformation_diagnosticsType_type1;

   ptr->message.u.radioBearerReconfigurationFailure.failureCause.u.protocolError->
    diagnosticsType.u.type1 = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation_type1);
   
    if(ptr->message.u.radioBearerReconfigurationFailure.failureCause.u.protocolError->
        diagnosticsType.u.type1 == NULL)
    {
      ERR_FATAL("Memory allocation failure",0,0,0);
    }
    
    /* Copy the given protocol error cause */
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.u.protocolError->
      diagnosticsType.u.type1->protocolErrorCause = prot_err_cause;
    /*log the protocol error*/
    tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                            (uint8)TDSRRC_PROCEDURE_RBRC,
                            (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                            tdsrrc_log_map_protocol_cause(prot_err_cause)
                            );
    break;

  case TDSRRCRB_MSG_INVALID_CONFIG:
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
    break;

  case TDSRRCRB_MSG_INCOMPLETE_CONFIG:
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
    break;

  case TDSRRCRB_CELL_UPDATE_OCCURED:
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_cellUpdateOccurred;
    break;

  default:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RBRC Unknown Failure message");
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
    break;
  }

  /* Get the RLC logical channel id for this msg from Logical Channel Manager ??*/
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH,
                                         DCCH_AM_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA);

  /* If the logical channel id is not invalid, send the message to the
  send chain module */
  if (ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending RBRC Failure msg to send-chain Cause:%d",ptr->message.u.radioBearerReconfigurationFailure.failureCause.t);
    
    /* Need to wait for L2 ack for all states because RRC needs to trigger include reconfig status indicator
    if RLC acknowledgement is not yet received.
    As per 8.3.1.3
    1> if the IE "Cell update cause" is set to "radio link failure" and the UE has submitted a reconfiguration response
    message to RLC and the UE has not received the RLC acknowledgement for the response message:
    2> include the IE "Reconfiguration Status Indicator" and set it to TRUE.*/
    if(tdsrrc_rbrc.status.failure_status == TDSRRCRB_PHY_CHAN_FAILURE)
    {
      tdsrrc_update_reconfig_waiting_for_l2ack_dch(TDSRRC_PROCEDURE_RBRC);
    status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBRC,  /* Procedure is RBRC    */
                                 (void *)ptr,        /* Message pointer     */
                                 ul_lc_id,           /* RLC id              */
                                 TDSRRCSEND_L2ACK_REQUIRED  /* ACK is required */
                                );
    }
    else
    {
      status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBRC,  /* Procedure is RBRC    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_NOT_REQUIRED  /* No need for ACK */
                                  );
    }
    if ( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Send chain failed for RB RC failure msg: error cause %d",status);
    }
  } 
  else
  {
    /* Unexpected failure. ERR for now. May need to ERR_FATAL here */
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't send UL DCCH msg! Invalid lc id: %d",ul_lc_id );
    /* It's important to free the memory since we didn't send the
    message. */
    tdsrrcasn1_free_pdu(tdsrrc_UL_DCCH_Message_PDU, ptr);
  }
}/* end function tdsrrcrbrc_send_rbrc_failure_message */

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION

/*===========================================================================

FUNCTION   tdsrrcrbrc_send_lifetime_validation_failure

DESCRIPTION

  This function sends the Radio Bearer Reconfiguration Failure message
  on Uplink DCCH. The failure cause must be given to this
  function. 

        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
static void tdsrrcrbrc_send_lifetime_validation_failure
(
tdsrrcrb_msg_status_e_type failure_cause  /* Failure Cause - should be a subset
                                        of the failure causes specified by
                                        the spec. */

)
{

  tdsrrc_UL_DCCH_Message *ptr;     /* Pointer to uplink DCCH message type */
  rlc_lc_id_type ul_lc_id; /* Logical Channel Id for the message  */
  tdsrrcsend_status_e_type status; /* Status from send chain module       */

  /* Allocate memory for this message. Note that this buffer is released
  by the SEND CHAIN */
  ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc( 
                                          sizeof(tdsrrc_UL_DCCH_Message));
  if (ptr == NULL)
  {
    ERR_FATAL("No memory for RB Reconfig Failure msg!",0,0,0);
  }

  TDSRRC_RESET_MSG_IE_PRESENT_PTR(ptr); /* Integrity Check info not present */
  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_radioBearerReconfigurationFailure; /* RR Failure msg */

  /* Set the bit-mask -  ?? */
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReconfigurationFailure);


  if ( tdsrrc_rbrc.tr_ids.rejected_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {

    ptr->message.u.radioBearerReconfigurationFailure.rrc_TransactionIdentifier =
    tdsrrc_rbrc.tr_ids.rejected_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_rbrc.tr_ids.rejected_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  } 
  else if ( tdsrrc_rbrc.tr_ids.accepted_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.radioBearerReconfigurationFailure.rrc_TransactionIdentifier =
    tdsrrc_rbrc.tr_ids.accepted_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_rbrc.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  } 
  else
  {
    /* Don't know the transaction id for this message.( one reason
    for this could be that the message had a critical
    extenstion). Since it's unknown, we should set it to zero. */
    ptr->message.u.radioBearerReconfigurationFailure.rrc_TransactionIdentifier= 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalide Transaction id for RBRF message!");
  }

  switch ( failure_cause )
  {
  case TDSRRCRB_MSG_UNSUPPORTED_CONFIG:
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
    break;

  case TDSRRCRB_PHY_CHAN_FAILURE:
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_physicalChannelFailure;
    break;

  case TDSRRCRB_SIMULTANEOUS_RECONFIG:
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_incompatibleSimultaneousReconfiguration;
    break;

  case TDSRRCRB_MSG_INVALID_CONFIG:
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
    break;

  case TDSRRCRB_MSG_INCOMPLETE_CONFIG:
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
    break;

  case TDSRRCRB_CELL_UPDATE_OCCURED:
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_cellUpdateOccurred;
    break;

  default:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RBRC Unknown Failure message");
    ptr->message.u.radioBearerReconfigurationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
    break;
  }

  /* Get the RLC logical channel id for this msg from Logical Channel Manager ??*/
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH,
                                         DCCH_AM_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA);

  /* If the logical channel id is not invalid, send the message to the
  send chain module */
  if (ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending RBRC Failure msg to send-chain Cause:%d",ptr->message.u.radioBearerReconfigurationFailure.failureCause.t);
      
    status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBRC,  /* Procedure is RBRC    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_NOT_REQUIRED  /* No need for ACK */
                                  );
    
    if ( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Send chain failed for RB RC failure msg: error cause %d",status);
    }
  } 
  else
  {
    /* Unexpected failure. ERR for now. May need to ERR_FATAL here */
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't send UL DCCH msg! Invalid lc id: %d",ul_lc_id );
    /* It's important to free the memory since we didn't send the
    message. */
    tdsrrcasn1_free_pdu(tdsrrc_UL_DCCH_Message_PDU, ptr);
  }
}/* end function tdsrrcrbrc_send_lifetime_validation_failure */

#endif /*FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION*/


/*===========================================================================

FUNCTION   SEND_RBRC_COMPLETE_MSG

DESCRIPTION

  This function sends the Radio Bearer Reconfig Complete message
  on Uplink DCCH. 
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
static void tdsrrcrbrc_send_rbrc_complete_message( boolean proc_wait_for_l2_ack )
{

  tdsrrc_UL_DCCH_Message *ptr;     /* Pointer to uplink DCCH message type */
  rlc_lc_id_type ul_lc_id; /* Logical Channel Id for the message  */
  tdsrrcsend_status_e_type status; /* Status from send chain module       */

  /* Allocate memory for this message. Note that this buffer is released
  by the SEND CHAIN */
  ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc( 
                                          sizeof(tdsrrc_UL_DCCH_Message));
  if (ptr == NULL)
  {
    ERR_FATAL("No memory for RB RC Complete msg!",0,0,0);
  }

  TDSRRC_RESET_MSG_IE_PRESENT_PTR(ptr); /* Integrity Check info not present */
  /* RB RC Complete msg */
  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_radioBearerReconfigurationComplete; 

  /* Fill up the message contents. The bit_mask is set to 0 since none
  of the optional IE's are included for now. */
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReconfigurationComplete);

  if (tdsrrc_rbrc.rbrc_for_srns_relocation == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Appends start list to tdsrrc_rbrc message for SRNS relocation");
    tdsrrcrbrc_append_start_list_to_rbrc_complete(ptr);
  }
  else
  {
    if (tdsrrcllc_get_rlc_size_change_status() 
      && ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH || 
             tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
          || (tdsrrc_get_rlc_size_change_status_for_procedure_id(TDSRRC_PROCEDURE_RBRC) == TRUE)))
    {
      // Check if next state is CELL+PCH, and OC API returns RLC size change, then include counter sync
      if (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH || 
          tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
      {
        /* Ask SMC to do start value calculation for PS domain */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RLCSIZEINDICATED Invoking SMC extract func");
        (void)tdsrrcsmc_extract_return_hfn_per_domain_and_update_start_flag(RRC_PS_DOMAIN_CN_ID);
        
        
      }
      else if (tdsrrc_get_rlc_size_change_status_for_procedure_id(TDSRRC_PROCEDURE_RBRC) == TRUE)
      {
        tdsrrc_set_rlc_size_change_status(TDSRRC_PROCEDURE_NONE, 
                                       FALSE, TDSRRC_INVALID_RB_ID ,
                                       TDSRLC_RE_ESTABLISH_NONE);
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsize change append start value");
        /*start list is included in ul_counter_synchronization_info. So enable this IE in the
        bitmask */
      TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->message.u.radioBearerReconfigurationComplete,
        tdsrrc_RadioBearerReconfigurationComplete,ul_CounterSynchronisationInfo); 
      
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.
	  	radioBearerReconfigurationComplete.ul_CounterSynchronisationInfo);
     
      /* call SMC API to append the start list */
      if (SUCCESS != tdsrrcsmc_append_start_list(
        &ptr->message.u.radioBearerReconfigurationComplete.ul_CounterSynchronisationInfo.startList,
        TDSRRCSMC_APPEND_START))
      {
        /*should not come here */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not append Start list");
      }
    }
  } 
  

  //if this is case of IFHHO and not SRNS relocation(i.e. ul counter sync is not present)
  //then also send START_List and set count c act time and configure smc for Step_2

  if ( ((tdsrrc_rbrc.hho_ciph_status == TRUE) || (tdsrrc_rbrc.trans_to_dch_ciph_config_needed == TRUE)) &&
       (TRUE == tdsrrcsmc_tm_ciph_active()))
  {
    if(! (TDSRRC_MSG_COMMON_BITMASK_IE(ptr->message.u.radioBearerReconfigurationComplete,
           tdsrrc_RadioBearerReconfigurationComplete,ul_CounterSynchronisationInfo)))
    {
      tdsrrcrbrc_append_start_list_to_rbrc_complete(ptr);
    }
  
    //Set the bitmask for count_c_act_time and copy the same..
    TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->message.u.radioBearerReconfigurationComplete,
      tdsrrc_RadioBearerReconfigurationComplete,count_C_ActivationTime);
    
    ptr->message.u.radioBearerReconfigurationComplete.count_C_ActivationTime =
      (tdsrrc_ActivationTime)tdsrrcsmc_step_2_cipher_config_for_hho();
  
    //Send step 2 config for HHO.. only one of these flags will be active, reset both of them
    //here
    if(tdsrrc_rbrc.hho_ciph_status)
    {
      tdsrrc_rbrc.hho_ciph_status =  FALSE;      
    }
    else if(tdsrrc_rbrc.trans_to_dch_ciph_config_needed)
    {
      tdsrrc_rbrc.trans_to_dch_ciph_config_needed = FALSE;      
    }
  }

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
 #ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
    if(tdsrrcsib_is_sib11_11bis_12_read_deferred(TDSDEFERRED_ANY_ALLOWED_SIB))
    {
      ptr->message.u.radioBearerReconfigurationComplete.m.
	  	laterNonCriticalExtensionsPresent=1;
 
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReconfigurationComplete.laterNonCriticalExtensions);
 
      ptr->message.u.radioBearerReconfigurationComplete.
	  	laterNonCriticalExtensions.m.v770NonCriticalExtensionsPresent=1;
 
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReconfigurationComplete.laterNonCriticalExtensions.
	  	v770NonCriticalExtensions);
 
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReconfigurationComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        radioBearerReconfigurationComplete_v770ext);
 
      ptr->message.u.radioBearerReconfigurationComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        radioBearerReconfigurationComplete_v770ext.m.deferredMeasurementControlReadingPresent=1;
      
      ptr->message.u.radioBearerReconfigurationComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        radioBearerReconfigurationComplete_v770ext.deferredMeasurementControlReading = 
        tdsrrc_RadioBearerReconfigurationComplete_v770ext_IEs_deferredMeasurementControlReading_true_;
    }
#endif
  }

  /* Transaction Id - use the accepted transaction id if known. Else
  use zero. Since this is a RB RC Complete message, we don't check 
  rejected transaction ids. */
  if ( tdsrrc_rbrc.tr_ids.accepted_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.radioBearerReconfigurationComplete.rrc_TransactionIdentifier=
    tdsrrc_rbrc.tr_ids.accepted_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again unless UE is waiting for TDSRRCRB_WAIT_FOR_L2_ACK*/
    if (!proc_wait_for_l2_ack)
    {
      tdsrrc_rbrc.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
    }
  } else
  {
    /* Don't know the transaction id for this message.( one reason
    for this could be that the message had a critical
    extenstion). Since it's unknown, we should set it to zero. */
    ptr->message.u.radioBearerReconfigurationComplete.rrc_TransactionIdentifier = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Transaction id invalid for RB RC Complete message!");
  }

  /* For now, there is nothing else to include in this message. We
  can give it to the send chain. */

  /* Get the RLC logical channel id for this msg from Logical Channel Manager ??*/
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH,
                                         DCCH_AM_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA);

  /* If the logical channel id is not invalid, send the message to the
  send chain module */
  if (ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RB Reconfig Complete Message to send-chain");
    if (proc_wait_for_l2_ack) 
    {
      status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBRC,  /* Procedure is RBRC    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_REQUIRED  /* ACK is required */
                                   );
    }
    else
    {
      status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBRC,  /* Procedure is RBRC    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_NOT_REQUIRED  /* ACK is not required */
                                   );
    }

    if ( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Send chain failed for RB RC complete msg: error cause %d",status);
    }
  } else
  {
    /* Unexpected failure. ERR for now. May need to ERR_FATAL here */
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't send UL DCCH msg! invalid lc id: %d",ul_lc_id ); 
    /* It's important to free the memory since we didn't send the
    message. */
    tdsrrcasn1_free_pdu(tdsrrc_UL_DCCH_Message_PDU, ptr);
  }

  tdsrrclog_send_rb_rate_info_packet();

}/* end function tdsrrcrbrc_send_rbrc_complete_message */

/*===========================================================================

FUNCTION   RRCRBRC_SEND_INDICATION_TO_UPPER_LAYERS

DESCRIPTION

  This function sends the approprate indications to MM or RABM layers
  depending on the contents of radiobearerreconfiguration message that
  has been stored locally.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_send_indication_to_upper_layers
(
  void
)
{
  mm_cmd_type   *mm_ptr;     /* To send the MM_SYNC_IND command */
  rabm_cmd_type *rabm_ptr; /* To send the RABMAS_RAB_ESTABLISH_IND command */
  uint32         rab_index=0; /* Used if there is more than 1 RAB in the message */
  uint32         rb_index=0; /* Used if there is more than 1 RAB in the message */
  uint32         local_rb_index=0; /* Used if there is more than 1 RAB in the message */
  boolean        rab_search = FALSE;

  /* send the nas cn info if stored */
  if (tdsrrc_rbrc.cn_info_saved != FALSE)
  {
    /* Put the command on MM queue */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RRC_SERVICE_IND_FROM_OTA_MSG to MM");
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(tdsrrc_rbrc.mm_cmd_ptr);
    }
    mm_put_cmd(tdsrrc_rbrc.mm_cmd_ptr);
    /* clear the flag */
    tdsrrc_rbrc.cn_info_saved = FALSE;
#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER      
    /*clear srns trigger flag*/
	 tdsrrc_rbrc.srns_reallocation_trigger = FALSE;    
#endif    
  }

  for(rb_index = 0; rb_index <tdsrrc_rbrc.rabs.num_rabs_in_msg; rb_index++)
  {
    if ((tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB) ||
        (tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_DATA_RAB) )
    {
      /* It's a CS RAB. Send the RRC_SYNC_IND primitive to MM. */
      if ((mm_ptr = mm_tdsrrc_get_cmd_buf(RRC_SYNC_IND)) == NULL)
      {
        ERR_FATAL("Can't send MM cmd: 0x%x  Out of memory!",RRC_SYNC_IND,0,0);
      }
#ifdef FEATURE_DUAL_SIM
      mm_ptr->cmd.rrc_sync_ind.as_id = tdsrrc_get_as_id();
#endif
      mm_ptr->cmd.rrc_sync_ind.cn_domain_id = RRC_CS_DOMAIN_CN_ID;
  
      /* If MS is in loopback test mode, set casue to Loopback */
      if ( tdsrrclbt_is_lb_on() == TRUE )
      {
        mm_ptr->cmd.rrc_sync_ind.cause = RRC_LOOPBACK_ESTABLISHED;
      }
      else
      {
        mm_ptr->cmd.rrc_sync_ind.cause = RRC_RAB_ESTABLISHED;

      }
      mm_ptr->cmd.rrc_sync_ind.rab_id_present = TRUE;
      mm_ptr->cmd.rrc_sync_ind.rab_info.action = RAB_ESTABLISHED;
      mm_ptr->cmd.rrc_sync_ind.rab_info.rab_id = 
      tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_id;
   
     /*By this time, rab type should have been updated in tdsrrc_rbrc*/
      if(tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB)
      {
        mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_CS_VOICE_CALL;
      }
      else if(tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_DATA_RAB)
      {
        mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_CS_DATA_CALL;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Something wrong with RAB Type");      
        mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_INVALID_RAB_ID;
      }
      mm_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_NONE;
      if(tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB)
      {
        if(tdsrrc_get_current_codec_mode() == TDSCDMA_IVOCODER_ID_AMRWB )
        {
          mm_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_WB_AMR;
        }
        else
        {
          mm_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_NB_AMR;
        }
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Codec %d in RRC_SYNC_IND",mm_ptr->cmd.rrc_sync_ind.rab_info.codec);
      }
      /* Put the command on MM queue */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sending RRC_SYNC_IND to MM for RAB type %d RAB-id %d",mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type,mm_ptr->cmd.rrc_sync_ind.rab_info.rab_id);
  
      if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
      {
        tdscfa_mm_put_cmd(mm_ptr);
      }
      mm_put_cmd(mm_ptr);
    }
    else if ( tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_PS_DATA_RAB )
    {
    
      if(tdsrrcscrr_is_cn_domain_open(RRC_PS_DOMAIN_CN_ID) == TRUE)
      {
        /* It's a PS RAB. Send the RABMAS_RAB_ESTABLISH_IND primitive to RABM. */
        if ((rabm_ptr = rabm_get_cmd_buf()) == NULL)
        {
          ERR_FATAL("Can't send RABM cmd: 0x%x  Out of memory!",RABMAS_RAB_ESTABLISH_IND,0,0);
        }
     
        rabm_ptr->header.cmd_id      = RABMAS_RAB_ESTABLISH_IND;
        rabm_ptr->header.message_set = (uint16)MS_RABM_RRC;
        rabm_ptr->cmd.rrc_establish_ind.rabid = 
        (rabid_T)(tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_id);
#ifdef FEATURE_DUAL_SIM
        rabm_ptr->cmd.rrc_establish_ind.as_id = tdsrrc_get_as_id();
#endif
  
        /* Put the command on RABM queue */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending RAB_ESTABLISH_IND to RABM RABid%d",tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_id);
        if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
        {
          tdscfa_rabm_put_cmd(rabm_ptr);
        }
        rabm_put_cmd(rabm_ptr);
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Skip sending RAB_ESTABLISH_IND as PS domain not open");
      }
    }
  }
  /* Indicate rabm whether or not the rab info to modify is present in the
  message */
  if (tdsrrc_check_rabs_associated_with_cn_domain(RRC_PS_DOMAIN_CN_ID))
  {
    for (rab_index = 0; rab_index< MAX_RAB_TO_SETUP; rab_index++)
    {
      if ( (tdsrrc_est_rabs.rabs[rab_index].cn_domain == RRC_PS_DOMAIN_CN_ID) &&
           (tdsrrc_est_rabs.rabs[rab_index].num_rbs_for_rab > 0) )
      {
        /* found a valid RAB for PS domain. Now check whether any of the RB of this RAB were 
        in the rb info reconfig list or rb info affected list.*/
        for (rb_index=0; rb_index<tdsrrc_est_rabs.rabs[rab_index].num_rbs_for_rab; rb_index++)
        {
          if (tdsrrc_rbrc.rbs_tobe_reconfigured.num_rbs>0)
          {
            for (local_rb_index =0; local_rb_index<tdsrrc_rbrc.rbs_tobe_reconfigured.num_rbs; local_rb_index++)
            {              
              if (tdsrrc_rbrc.rbs_tobe_reconfigured.rb_id[local_rb_index] == 
                  tdsrrc_est_rabs.rabs[rab_index].rb_for_rab[rb_index].rb_id)
              {
                rab_search = TRUE;
                break;
              }
            }
            
          }       
          if (!rab_search)
          {
            if (tdsrrc_rbrc.rbs_tobe_affected.num_rbs>0)
            {
              for (local_rb_index=0; local_rb_index<tdsrrc_rbrc.rbs_tobe_affected.num_rbs; local_rb_index++)
              {              
                if (tdsrrc_rbrc.rbs_tobe_affected.rb_id[local_rb_index] == 
                    tdsrrc_est_rabs.rabs[rab_index].rb_for_rab[rb_index].rb_id)
                {
                  rab_search = TRUE;
                  break;
                }
              }
            }       
          }
          /* if any one RB matches with the RBs associated with RAB then no need to
          continue looking for other RBs. break from here otherwise continue */
          if (rab_search)
          {
            break;
          }
        } /*for rb_index loop */
      }/* if ps_domain match found in established rab */
      /* if rab is found then send the indication to RABM
      and continue with remaining RABs */
      if (rab_search == TRUE)
      {
        if(tdsrrcscrr_is_cn_domain_open(RRC_PS_DOMAIN_CN_ID) == TRUE)
        {
          
          /*there is a PS domain RAB so send a notification to RABM */
          if ((rabm_ptr = rabm_get_cmd_buf()) == NULL)
          {
            ERR_FATAL("Can't send RABM cmd: 0x%x  Out of memory!",RABMAS_RAB_ESTABLISH_IND,0,0);
          }
          rabm_ptr->header.cmd_id      = RABMAS_RAB_ESTABLISH_IND;
          rabm_ptr->header.message_set = MS_RABM_RRC;
          rabm_ptr->cmd.rrc_establish_ind.rabid = (rabid_T)(tdsrrc_est_rabs.rabs[rab_index].rab_id); 
#ifdef FEATURE_DUAL_SIM
          rabm_ptr->cmd.rrc_establish_ind.as_id = tdsrrc_get_as_id();
#endif
          /* Put the command on RABM queue */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Send RAB_ESTABLISH_IND RABid:%d",tdsrrc_est_rabs.rabs[rab_index].rab_id);
          if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
          {
            tdscfa_rabm_put_cmd(rabm_ptr);
          }
          rabm_put_cmd(rabm_ptr);
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Skip sending RAB_ESTABLISH_IND as PS domain not open");
        }

        rab_search=FALSE;
      }
    }/*end of for*/
  }/*if there is any ps rab */


} /*rrcrbrc_send_indication_to_upper_layers */

/*===========================================================================

FUNCTION   tdsrrcrbrc_update_nas_sync_ind

DESCRIPTION Updates the NAS Syncronisation Indicator field received in the RB Reconfig message.
            In case the NAS Syncronisation Indicator IE is not present , then a default value of 
            current active  codec is used.

DEPENDENCIES
  None.

RETURN VALUE
 None

SIDE EFFECTS
  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrcrbrc_update_nas_sync_ind
(
   uint32 rab_index,
   tdsrrc_NAS_Synchronisation_Indicator *nas_Synchronisation_Indicator 
)
{
  tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator = 
  tdsrrc_translate_nas_sync_indicator(nas_Synchronisation_Indicator); 
   
  if( tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator == TDSCDMA_IVOCODER_ID_NONE)
  {
    tdsrrc_rbrc.status.failure_status= TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported Codec Configuration %d",tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator);

    return FAILURE;
  }
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
  tdsrrc_set_ordered_codec_status(TRUE);
#endif
  tdsrrc_set_ordered_codec_mode(tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator);
  return SUCCESS;
}

/*===========================================================================

FUNCTION tdsrrcrbrc_is_pccpch_info_present

DESCRIPTION
  This function checks whether cell parameter id info is present in
  the message or not.

DEPENDENCIES
  None.

RETURN VALUE
Boolean. TRUE  : cell parameter id info is present in the message
                 FALSE : cell parameter id info is not present in the message
===========================================================================*/
static boolean  tdsrrcrbrc_is_pccpch_info_present
(
  tdsrrc_RadioBearerReconfiguration  *msg_ptr
  ,
  tdsrrc_msg_ver_enum_type                   msg_version
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
    tdsrrc_RadioBearerReconfiguration_r9_IEs *rbrc_ptr_r9 = &msg_ptr->u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9
      .radioBearerReconfiguration_r9;  

    if (rbrc_ptr_r9->m.dl_InformationPerRL_ListPresent)
    {
      if(rbrc_ptr_r9->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd)
      { 
        if (rbrc_ptr_r9->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBRC message %d",
                  rbrc_ptr_r9->dl_InformationPerRL_List.
                  elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          result = TRUE;
        }
      }
    }
  }
  else
#endif/* FEATURE_TDSCDMA_REL9 */
#ifdef FEATURE_TDSCDMA_REL8
  if (msg_version == TDSMSG_REL8)
  {
    tdsrrc_RadioBearerReconfiguration_r8_IEs *rbrc_ptr_r8 = &msg_ptr->u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8
      .radioBearerReconfiguration_r8;  

    if (rbrc_ptr_r8->m.dl_InformationPerRL_ListPresent)
    {
      if(rbrc_ptr_r8->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd)
      { 
        if (rbrc_ptr_r8->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBRC message %d",
                  rbrc_ptr_r8->dl_InformationPerRL_List.
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
    tdsrrc_RadioBearerReconfiguration_r7_IEs *rbrc_ptr_r7 = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
               criticalExtensions.u.r7.radioBearerReconfiguration_r7;

    if (rbrc_ptr_r7->m.dl_InformationPerRL_ListPresent)
    {
      if(rbrc_ptr_r7->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_tdd)
      { 
        if (rbrc_ptr_r7->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBRC message %d",
                  rbrc_ptr_r7->dl_InformationPerRL_List.
                  elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          result = TRUE;
        }
      }
    }
  }
  else

  if (msg_version == TDSMSG_REL6)
  {
    tdsrrc_RadioBearerReconfiguration_r6_IEs *rbrc_ptr_r6 = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
               criticalExtensions.u.r6.radioBearerReconfiguration_r6;

    if (rbrc_ptr_r6->m.dl_InformationPerRL_ListPresent)
    {
      if(rbrc_ptr_r6->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_tdd)
      { 
        if (rbrc_ptr_r6->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBRC message %d",
                  rbrc_ptr_r6->dl_InformationPerRL_List.
                  elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          result = TRUE;
        }
      }
    }
  }
  else
  if (msg_version == TDSMSG_REL5)
  {
    tdsrrc_RadioBearerReconfiguration_r5_IEs *rbrc_ptr_r5 = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
               r5.radioBearerReconfiguration_r5;

    if (rbrc_ptr_r5->m.dl_InformationPerRL_ListPresent)
    {
      if(rbrc_ptr_r5->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r5_modeSpecificInfo_tdd)
      { 
        if (rbrc_ptr_r5->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBRC message %d",
                  rbrc_ptr_r5->dl_InformationPerRL_List.
                  elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          result = TRUE;
        }
      }
    }
  }
  else
  {
    tdsrrc_RadioBearerReconfiguration_r4_IEs *rbrc_ptr_r4 = &msg_ptr->u.later_than_r3.
              criticalExtensions.u.r4.radioBearerReconfiguration_r4;

    if (rbrc_ptr_r4->m.dl_InformationPerRL_ListPresent)
    {
      if(rbrc_ptr_r4->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r4_modeSpecificInfo_tdd)
      { 
        if (rbrc_ptr_r4->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBRC message %d",
                  rbrc_ptr_r4->dl_InformationPerRL_List.
                  elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          result = TRUE;
        }
      }
    }
  }

  return result;
}  /* tdsrrcrbrc_is_pccpch_info_present */


/*===========================================================================

FUNCTION   VALIDATE_RB_ID

DESCRIPTION

  This function validates the rb_ids in the received RB Reconfig Message.
  If signalling RB id is present it returns TDSRRC_RB_PRESENT. For user plane RB ids
  it checks whether the rb to be reconfigured or affected is present in the 
  established rbs. If not this function returns TDSRRC_RB_NOT_PRESENT.

DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrc_rb_search_e_type 
          TDSRRC_RB_PRESENT     :rb-id exists in the ESTABLISHED_RAB variable or
                              it is a signalling RB.
          TDSRRC_RB_NOT_PRESENT :rb-id is not a signalling RB and it also
                              does not exist in the ESTABLISHED_RAB variable.

SIDE EFFECTS

  None.

===========================================================================*/
static tdsrrc_rb_search_e_type tdsrrcrbrc_validate_rb_id(tdsrrc_RB_Identity rb_id)
{
  tdsrrc_rb_search_e_type rb_srch = TDSRRC_RB_NOT_PRESENT;
  /*check whether it is signalling rbid */
  /*Do not include rb-id0 in the check because it is for CCCH */
  if ((rb_id > CCCH_RADIO_BEARER_ID) && (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID))
  {
    rb_srch = TDSRRC_RB_PRESENT;
  }
  else
  {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    if(tdsrrcllc_has_config_srb5(TDSCURRENT_CONFIG, rb_id) == TRUE)
    {
      rb_srch = TDSRRC_RB_PRESENT;
    }
    else
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
    {

      /*it must be user plane rb. Check whether this RB exists in established_rab db */
      rb_srch = tdsrrc_find_rb_in_est_rabs (rb_id);
    }
  }
  
  return rb_srch;
}

/*===========================================================================

FUNCTION   RRCRBRC_VALIDATE_SRNS_RELOCATION_INFO()

DESCRIPTION

  This function checks whether SRNS relocation info is present or not
  and also checks whether the current state and state indicated by the message
  are both Cell_DCH or not 
        
DEPENDENCIES

  None.
 
RETURN VALUE

  Boolean. TRUE : SRNS relocation info is present
           FALSE : SRNS relocation info is not present

SIDE EFFECTS

  None.

===========================================================================*/
static boolean tdsrrcrbrc_validate_srns_relocation_info
(
  tdsrrc_RadioBearerReconfiguration *msg_ptr, /* Pointer to the RB RC message */
  tdsrrc_msg_ver_enum_type msg_version
)
{
  boolean result = FALSE;

#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Validate if RBRC is for srns relocation");
  if (TRUE == tdsrrc_is_fresh_in_reconfiguration_message())
  {
    return TRUE;
  }
#endif

  if (msg_version == TDSMSG_REL5)
  {
    if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
         u.r5.radioBearerReconfiguration_r5,tdsrrc_RadioBearerReconfiguration_r5_IEs,new_U_RNTI))
    {
      result = TRUE;
    }
  }
  else if (msg_version == TDSMSG_REL6)
  {
    if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
         criticalExtensions.u.r6.radioBearerReconfiguration_r6,tdsrrc_RadioBearerReconfiguration_r6_IEs,
         new_U_RNTI))
    { 
      result = TRUE; 
    }
  }
  else if (msg_version == TDSMSG_REL7)
  {
    if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
         criticalExtensions.u.r7.radioBearerReconfiguration_r7,
         tdsrrc_RadioBearerReconfiguration_r7_IEs,new_U_RNTI))
    { 
      result = TRUE; 
    }
  }
/*RRC_GKG: Find out why RBE and RBR dedices based on DL Sync Info but RBRC dedices based on URNTI*/
#ifdef FEATURE_TDSCDMA_REL8
  else if (msg_version == TDSMSG_REL8)
  {
    if(TDSRRCRB_R8_MSG_IE_PRESENT((&msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerReconfiguration_r8),
        tdsrrc_RadioBearerReconfiguration_r8_IEs, new_U_RNTI))
    { 
      result = TRUE; 
    }
  }
#endif /* FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
  else if (msg_version == TDSMSG_REL9)
  {
    if(TDSRRCRB_R9_MSG_IE_PRESENT((&msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.radioBearerReconfiguration_r9),
        tdsrrc_RadioBearerReconfiguration_r9_IEs, new_U_RNTI))
    { 
      result = TRUE; 
    }
  }
#endif /* FEATURE_TDSCDMA_REL9*/
  else
  {
    if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4,
          tdsrrc_RadioBearerReconfiguration_r4_IEs,new_U_RNTI))
    {
      /*check whether the SRNS relocation is within Cell_DCH or not */
      result = TRUE;
    }
  }

  return result; 
}


/*===========================================================================

FUNCTION   VALIDATE_RBRC_MESSAGE_R5

DESCRIPTION

  This function validates the received RB Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Reconfig Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_validate_rbrc_message_r5
(
  tdsrrc_RadioBearerReconfiguration *msg_ptr /* Pointer to the RB RC message */
)
{
  tdsrrc_RadioBearerReconfiguration_r5_IEs *rbrc_ptr = NULL;

  tdsrrc_RAB_InformationReconfigList *rab_list_ptr = NULL;  
#ifdef FEATURE_UMTS_PDCP
  tdsrrc_RB_InformationReconfig_r5 *rb_info_ptr = NULL;
  tdsrrc_HeaderCompressionInfo_r4 *pdcpHeaderCompressionInfo = NULL;  
#endif /* FEATURE_UMTS_PDCP*/
  rrc_rab_id_type rab_id;           /* To store RAB id in internal RRC format */
  rrcmn_rab_status_T rab_type;      /* To store the status returned by MN layer. This
                                    indicates what type of RAB is received. This is 
                                    used only if the RAB is for the CS_domain */
  tdsrrc_rab_search_e_type tdsrab_status; /* To check if RAB is present in ESTABLISHED_RABS */
  uint32 rab_index=0;               /* Used as an index when storing established
                                    RABs locally. Only when we have more than
                                    one RAB per message, this will be incremented */

  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();


  /* Assume the message is valid for now */
  tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_VALID;

  rbrc_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
             r5.radioBearerReconfiguration_r5;

  /* Check the RRC State indicatior given by the message and 
     store it in the tdsrrc_rbrc internal memory
   */
  switch (rbrc_ptr->rrc_StateIndicator)
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;

    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;

    default:
      /* No other states are supported right now. */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported state %d in RBRC R5 Msg",rbrc_ptr->rrc_StateIndicator);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;
  }

  if (tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG)
  {
    return;
  }

  /* Validate DL HSPDSCH */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r5_IEs,dl_HSPDSCH_Information))
  {
    /* Check if network does not try to initiate HSDPA with invalid state */
    if( tdsrrc_hsdpa_initial_validation(rbrc_ptr->rrc_StateIndicator, 
      TDSRRC_CHECK_COMMON_MSG_TYPE(rbrc_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
      rrc_DL_HSPDSCH_Information_modeSpecificInfo_fdd))
                                                     == FAILURE)  
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"INVALID DL_HSPDSCH_Information in RBRC");
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  /* Not support to the pre-config option. */
  if (!(TDSRRC_CHECK_COMMON_MSG_TYPE(rbrc_ptr->specificationMode,
      rrc_RadioBearerReconfiguration_r5_IEs_specificationMode_complete)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Non-support preconfig t in RBRC");
    tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    return;
  }

  

  /* Obtain U_RNTI if present for SRNS relocation.  */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,
  	tdsrrc_RadioBearerReconfiguration_r5_IEs,new_U_RNTI))
  {
    /* check whether the SRNS relocation is within Cell_DCH or not.
       SRNS relocation information can come in reconfiguration messages
       only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbrc.state_indicated_by_msg);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* message with valid SNRS relocation info */
    else
    {
      /* Set the flag that indicates that this procedure
         involves SRNS relocation */
      tdsrrc_rbrc.rbrc_for_srns_relocation = TRUE;
      /*store the new U-RNTI */
      tdsrrc_translate_urnti (&(rbrc_ptr->new_U_RNTI), &tdsrrc_rbrc.new_urnti);
    }
  }

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  tdsrrc_rbrc.trans_to_dch_ciph_config_needed = FALSE;
  if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    /* check to see if dl-dpch-common info is present while going to dch */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r5_IEs,dl_CommonInformation)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rbrc_ptr->dl_CommonInformation,
         tdsrrc_DL_CommonInformation_r5,dl_DPCH_InfoCommon)))
    {
      /* check if ciphering is active in mac for cs domain, if it is then,
        start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      */
      if (tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");
        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             (TDSRRC_MSG_COMMON_BITMASK_IE(rbrc_ptr->dl_CommonInformation.dl_DPCH_InfoCommon,
             tdsrrc_DL_DPCH_InfoCommon_r4,mac_d_HFN_initial_value)),
             &rbrc_ptr->dl_CommonInformation.dl_DPCH_InfoCommon.mac_d_HFN_initial_value,
             &mac_d_hfn_l);
        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }	 
        if (tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing Ciphering Config for HHO failed");
        }
        tdsrrc_rbrc.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }

  /*Here we process all the RABs in RAB Info Reconfig List*/    
  tdsrrc_rbrc.rabs.num_rabs_in_msg =0;
  if (rbrc_ptr->specificationMode.u.complete.m.rab_InformationReconfigListPresent)
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"RAB Info Reconfig in R5 RBRC Msg");

    rab_list_ptr = &rbrc_ptr->specificationMode.u.complete.rab_InformationReconfigList;

    for (rab_index = 0; rab_list_ptr->n > rab_index && rab_index < TDSRRCRB_MAX_RAB_PER_MSG; rab_index++)
    {
      /* Check if the RAB id is GSM_MAP type. We don't support any
         other type */
      if (rab_list_ptr->elem[rab_index].rab_Identity.t != T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RABid t%d is not GSM_MAP!",rab_list_ptr->elem[rab_index].rab_Identity.t);
        tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        return;
      }

      /* GSM_MAP RAB Id. Check if it's valid by calling the appropriate NAS entity. 
         First convert it to internal format. */
      rab_id = tdsrrcrb_translate_gsm_map_rab_id((rab_list_ptr->elem[rab_index].rab_Identity.u.gsm_MAP_RAB_Identity));

      /* Check if the RAB id is already in ESTABLISHED_RABS */
      tdsrab_status = tdsrrc_find_rab_in_established_rabs(rab_id);
      if ( tdsrab_status != TDSRRC_RAB_FOUND )
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB ID 0X%x not in ESTABLISHED_RABS",rab_id);
        tdsrrc_rbrc.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        return;
      }

      /* Store this info locally so that ESTABLISHED_RABS can
         be updated later, when these RABs are established */
      tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_id_is_gsm_map = TRUE;
      tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_id = rab_id;

      /* Check if this RAB is CS Domain or PS Domain */
      if ( rab_list_ptr->elem[rab_index].cn_DomainIdentity  == tdsrrc_CN_DomainIdentity_cs_domain )
      {
        /* Store the domain identity. This will be later added to
        ESTABLISHED_RABS */
        tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].cn_domain = RRC_CS_DOMAIN_CN_ID;
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS RABId %d to be setup",rab_id);
        /* Check if it is in loopback test */
        if ( tdsrrclbt_is_lb_on() == TRUE )
        {
          /* Set Rab type to Data */
          TDSRRC_MSG0(MSG_LEGACY_LOW,"Loopback Test");
          tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
        }
        else
        {
          /* For CS RABs, Call MN layer to verify this RAB */
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
          rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
          rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif

          TDSRRC_MSG3(MSG_LEGACY_HIGH,"For RAB-Id %d, RAB type from NAS = %d, RAB type from EST_RABs = %d",rab_id, rab_type,
            tdsrrc_get_rab_type_for_given_rab_id(tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_id));

          switch ( rab_type )
          {
            case RRCMN_CS_DATA_CALL:
              tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
              break;

            case RRCMN_CS_VOICE_CALL:
              tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_VOICE_RAB;

#ifdef FEATURE_SCUDIF_ENHANCEMENT
              #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/
              break;

            default:
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"CS RAB validation failed!");
              tdsrrc_rbrc.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
              break;

          } /* end switch */

          /*As a general approach we should use/update the NAS Sync Indicator even for CS DATA*/
#ifdef FEATURE_SCUDIF_ENHANCEMENT
          #error code not present
#else
          if(tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB)
#endif
          {
            if(tdsrrcrbrc_update_nas_sync_ind(rab_index,&(rab_list_ptr->elem[rab_index].nas_Synchronisation_Indicator))
                == FAILURE)
            {
              return;
            }
            nas_sync_ind = tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator;              
          }
        }

        if ( (tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_INVALID_CONFIG) ||
             (tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG)
           )
        {
          /* No need to continue */
          return;
        }
      }
      /* Else check for PS Domain */
      else if ( rab_list_ptr->elem[rab_index].cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain )
      {
        /* Store the domain identity. This will be later added to
           ESTABLISHED_RABS 
         */
        tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].cn_domain = RRC_PS_DOMAIN_CN_ID;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"PS RABId %d to be setup",rab_id);
        tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_PS_DATA_RAB;
      }
      rab_index++;
      tdsrrc_rbrc.rabs.num_rabs_in_msg ++;
    } // for each RAB info
  } //RAB info IE present
  tdsrrc_set_ordered_codec_mode(nas_sync_ind);
  if (rbrc_ptr->specificationMode.u.complete.m.rb_PDCPContextRelocationListPresent)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP Context Relocation not supported");
    tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    return;
  }

#ifdef FEATURE_UMTS_PDCP
 if (tdsrrc_pdcp_enabled)
 {
   /* Validate all PDCP info if present */
   if (rbrc_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent )
   {
     uint32 local_rb_idx =0;
	 
	 while (rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList.n > local_rb_idx) 
	 {
	   	 rb_info_ptr = &rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList.elem[local_rb_idx];
	   if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_ptr,tdsrrc_RB_InformationReconfig_r5,pdcp_SN_Info))
	   {
		 TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP SN info not suported");
		 tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
		 return;
	   }
	   if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_ptr,tdsrrc_RB_InformationReconfig_r5,pdcp_Info))
	   {
		 if (tdsrrc_get_rab_type_for_given_rb_id(rb_info_ptr->rb_Identity) != TDSRRCRB_PS_DATA_RAB)
		 {
		   TDSRRC_MSG1(MSG_LEGACY_HIGH,"NW incld PDCP info for non-ps rb id %d: Invalid",rb_info_ptr->rb_Identity);
		   tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
		   return;
		 }
         /* check whether lossless SRNS relocation flag is set or   
          * and header compression algo is present in the message 
          */
		 if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info.pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) && 
			 (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_info_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,rrc_LosslessSRNS_RelocSupport_supported)))
		 {
		   if (rb_info_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_absent)
		   {
			 TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP pdu header is absent.  Invalid config");
			 tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
			 return;
		   }
		   
		 } /* if rrc_PDCP_Info_r4_losslessSRNS_RelocSupport_present */

		 if (rb_info_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) 
		 {
		   if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info.pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) && 
			   (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_info_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,rrc_LosslessSRNS_RelocSupport_notSupported)))
		   {
			 if (!(TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info.pdcp_Info,
			 	tdsrrc_PDCP_Info_r4,headerCompressionInfoList)))
			 {
			   TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP header compress info list not present");
			   tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
			   return;
			 }
		   }
		 }

		 if (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info.pdcp_Info,
			 	tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) 
		 {
		   uint32 local_idx=0;
		   while (rb_info_ptr->pdcp_Info.pdcp_Info.headerCompressionInfoList.n >   local_idx) 
		   {
		     pdcpHeaderCompressionInfo = &rb_info_ptr->pdcp_Info.pdcp_Info.headerCompressionInfoList.elem[local_idx];
			 if (TDSRRC_CHECK_COMMON_MSG_TYPE(pdcpHeaderCompressionInfo->algorithmSpecificInfo,
				 rrc_AlgorithmSpecificInfo_r4_rfc2507_Info))
			 {
			   /* if the UE capability "Maximum header compression context space", is exceeded with this configuration send
                * failure with cause "Invalid Config"
                */
			   if ( 
				   (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER > IPHC_MAX_HC_CONTEXT_SPACE))
			   {
				 TDSRRC_MSG1(MSG_LEGACY_HIGH,"Max header comprss context space exceeded : %d",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER);
				 tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
				 return;
			   }
			 }
			 else if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.t == 
					  T_tdsrrc_AlgorithmSpecificInfo_r4_rfc3095_Info)
			 {
               /* 	the chosen MAX_CID shall not be greater than the value "Maximum number of ROHC context sessions" as 
                *  indicated in the IE "PDCP Capability"
                */
			   if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,ul_RFC3095))
			   {
				 if ((pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)) 
				 {
				   TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID);
				   tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
				   return;
				 }
			   }
			   if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,dl_RFC3095))
			   {
				 if ((pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)) 
				 {
				   TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID);
				   tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
				   return;
				 }
			   }
			 }/* else if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.t == rrc_rfc3095_Info)*/

			 local_idx++;
		   } /*  while (pdcpHeaderCompressionInfoList) */
		 } /*  if (rb_info_ptr->pdcp_Info.pdcp_Info.bit_mask & rrc_PDCP_Info_r4_headerCompressionInfoList_present) */

	   } /* if (rb_info_ptr->bit_mask & rrc_RB_InformationReconfig_r5_pdcp_Info_present) */
	   local_rb_idx++;
	 } /* while (rb_list_ptr) */
     
   } /* if (rbrc_ptr->specificationMode.u.complete.bit_mask & rrc_complete_rb_InformationReconfigList_present) */
 }
#endif /* FEATURE_UMTS_PDCP */

  /*Cell_PCH/URA_PCH specific validations */
 if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
     (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
 {
   /* If UTRAN DRX coefficient is not present then it is invalid configuration */
   if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,
         tdsrrc_RadioBearerReconfiguration_r5_IEs,utran_DRX_CycleLengthCoeff)))
   {
     TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_rbrc.state_indicated_by_msg);
     tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
     return;
   }
   if (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
   {
     /*should we do it after getting l2ack for response??*/
     if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r5_IEs,ura_Identity)))
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
       tdsrrccu_clear_ura_id ();
     }
     else
     {
       /*URA ID present in the message set ura_identity variable with this value */
       if (rbrc_ptr->ura_Identity.numbits == 16)
       {
         tdsrrccu_update_ura_id (&(rbrc_ptr->ura_Identity));
       }
       else
       {
         TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rbrc_ptr->ura_Identity.numbits);
         tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
         tdsrrc_rbrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
         return;
       }
     }
   }
 } // if TDSRRC_STATE_CELL_PCH or TDSRRC_STATE_URA_PCH
 
}/* end function tdsrrcrbrc_validate_rbrc_message_r5 */

/*===========================================================================

FUNCTION   VALIDATE_RBRC_MESSAGE_R6

DESCRIPTION

  This function validates the received RB Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Reconfig Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_validate_rbrc_message_r6
(
  tdsrrc_RadioBearerReconfiguration_r6_IEs *rbrc_ptr /* Pointer to the RB RC message */
)
{
  tdsrrc_RAB_InformationReconfigList *rab_list_ptr = NULL;  

  rrc_rab_id_type rab_id;           /* To store RAB id in internal RRC format */
  rrcmn_rab_status_T rab_type;      /* To store the status returned by MN layer. This
                                    indicates what type of RAB is received. This is 
                                    used only if the RAB is for the CS_domain */
  tdsrrc_rab_search_e_type tdsrab_status; /* To check if RAB is present in ESTABLISHED_RABS */
  uint32 rab_index=0;               /* Used as an index when storing established
                                    RABs locally. Only when we have more than
                                    one RAB per message, this will be incremented */
  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();

  /* Assume the message is valid for now */
  tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_VALID;


  /* Check the RRC State indicatior given by the message and 
     store it in the tdsrrc_rbrc internal memory
   */
  switch (rbrc_ptr->rrc_StateIndicator)
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;

    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;

    default:
      /* No other states are supported right now. */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported state %d in RBRC R5 Msg",rbrc_ptr->rrc_StateIndicator);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;
  }

  if (tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG)
  {
    return;
  }

  /* Validate DL HSPDSCH */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r6_IEs,dl_HSPDSCH_Information))
  {
    /* Check if network does not try to initiate HSDPA with invalid state */
    if (tdsrrc_hsdpa_initial_validation_r6(rbrc_ptr->rrc_StateIndicator, 
                                     &rbrc_ptr->dl_HSPDSCH_Information) == FAILURE)
    {
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  /* Not support to the pre-config option. */
  if (rbrc_ptr->specificationMode.t != 
      T_tdsrrc_RadioBearerReconfiguration_r6_IEs_specificationMode_complete)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Non-support preconfig t in RBRC");
    tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    return;
  }

  

  /* Obtain U_RNTI if present for SRNS relocation.  */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,
  	tdsrrc_RadioBearerReconfiguration_r6_IEs,new_U_RNTI))
  {
    /* check whether the SRNS relocation is within Cell_DCH or not.
       SRNS relocation information can come in reconfiguration messages
       only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbrc.state_indicated_by_msg);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* message with valid SNRS relocation info */
    else
    {
      /* Set the flag that indicates that this procedure
         involves SRNS relocation */
      tdsrrc_rbrc.rbrc_for_srns_relocation = TRUE;
      /*store the new U-RNTI */
      tdsrrc_translate_urnti (&(rbrc_ptr->new_U_RNTI), &tdsrrc_rbrc.new_urnti);
    }
  }

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  tdsrrc_rbrc.trans_to_dch_ciph_config_needed = FALSE;
  if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    /* check to see if dl-dpch-common info is present while going to dch */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,
         tdsrrc_RadioBearerReconfiguration_r6_IEs,dl_CommonInformation)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rbrc_ptr->dl_CommonInformation,
         tdsrrc_DL_CommonInformation_r6,dl_dpchInfoCommon)))
    {
      if (rbrc_ptr->dl_CommonInformation.dl_dpchInfoCommon.t == 
          T_tdsrrc_DL_CommonInformation_r6_dl_dpchInfoCommon_dl_DPCH_InfoCommon)
      {
        /* check if ciphering is active in mac for cs domain, if it is then,
          start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
        */
        if (tdsrrcsmc_tm_ciph_active() == TRUE)
        {
          uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
          uecomdef_status_e_type loc_ret_val;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");
          loc_ret_val = tdsrrcrb_get_mac_d_hfn(
               ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon,
               tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
               &rbrc_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
               &mac_d_hfn_l);
          if(loc_ret_val == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
          }
          if (tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing Ciphering Config for HHO failed");
          }
          tdsrrc_rbrc.trans_to_dch_ciph_config_needed = TRUE;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
        }
      }
    }
  }

  if (rbrc_ptr->specificationMode.u.complete.m.rab_InformationReconfigListPresent)
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"RAB Info Reconfig in R6 RBRC Msg");

    rab_list_ptr = &rbrc_ptr->specificationMode.u.complete.rab_InformationReconfigList;

    for (rab_index = 0; rab_list_ptr->n > rab_index && rab_index < TDSRRCRB_MAX_RAB_PER_MSG; rab_index++)
    {
      /* Check if the RAB id is GSM_MAP type. We don't support any
         other type */
      if (rab_list_ptr->elem[rab_index].rab_Identity.t != T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RABid t%d is not GSM_MAP!",rab_list_ptr->elem[rab_index].rab_Identity.t);
        tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        return;
      }

      /* GSM_MAP RAB Id. Check if it's valid by calling the appropriate NAS entity. 
         First convert it to internal format. */
      rab_id = tdsrrcrb_translate_gsm_map_rab_id((rab_list_ptr->elem[rab_index].rab_Identity.u.gsm_MAP_RAB_Identity));

      /* Check if the RAB id is already in ESTABLISHED_RABS */
      tdsrab_status = tdsrrc_find_rab_in_established_rabs(rab_id);
      if ( tdsrab_status != TDSRRC_RAB_FOUND )
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB ID 0X%x not in ESTABLISHED_RABS",rab_id);
        tdsrrc_rbrc.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        return;
      }

      /* Store this info locally so that ESTABLISHED_RABS can
         be updated later, when these RABs are established */
      tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_id_is_gsm_map = TRUE;
      tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_id = rab_id;

      /* Check if this RAB is CS Domain or PS Domain */
      if ( rab_list_ptr->elem[rab_index].cn_DomainIdentity  == tdsrrc_CN_DomainIdentity_cs_domain )
      {
        /* Store the domain identity. This will be later added to
        ESTABLISHED_RABS */
        tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].cn_domain = RRC_CS_DOMAIN_CN_ID;

        /* Check if it is in loopback test */
        if ( tdsrrclbt_is_lb_on() == TRUE )
        {
          /* Set Rab type to Data */
          TDSRRC_MSG0(MSG_LEGACY_LOW,"Loopback Test");
          tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
        }
        else
        {
          /* For CS RABs, Call MN layer to verify this RAB */
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
          rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
          rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif

          TDSRRC_MSG3(MSG_LEGACY_HIGH,"For RAB-Id %d, RAB type from NAS = %d, RAB type from EST_RABs = %d",rab_id, rab_type,
            tdsrrc_get_rab_type_for_given_rab_id(tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_id));

          switch ( rab_type )
          {
            case RRCMN_CS_DATA_CALL:
              tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
              break;

            case RRCMN_CS_VOICE_CALL:
              tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_VOICE_RAB;

#ifdef FEATURE_SCUDIF_ENHANCEMENT
              #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/

              break;

            default:
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"CS RAB validation failed!");
              tdsrrc_rbrc.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
              break;

          } /* end switch */
          /*As a general approach we should use/update the NAS Sync Indicator even for CS DATA*/
#ifdef FEATURE_SCUDIF_ENHANCEMENT
          #error code not present
#else
          if(tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB)
#endif
          {
              if(tdsrrcrbrc_update_nas_sync_ind(rab_index,
                &(rab_list_ptr->elem[rab_index].nas_Synchronisation_Indicator))
                 == FAILURE)
              {
                return;
              }
              nas_sync_ind =  tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator;
          }
  
        }

        if ( (tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_INVALID_CONFIG) ||
             (tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG)
           )
        {
          /* No need to continue */
          return;
        }
      }
      /* Else check for PS Domain */
      else if ( rab_list_ptr->elem[rab_index].cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain )
      {
        /* Store the domain identity. This will be later added to
           ESTABLISHED_RABS 
         */
        tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].cn_domain = RRC_PS_DOMAIN_CN_ID;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"PS RABId %d to be setup",rab_id);
        tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_PS_DATA_RAB;
      }
      
      tdsrrc_rbrc.rabs.num_rabs_in_msg ++;
    } // for each RAB info
  } //RAB info IE present
  tdsrrc_set_ordered_codec_mode(nas_sync_ind);


  if((T_tdsrrc_RadioBearerReconfiguration_r6_IEs_specificationMode_complete ==
     (rbrc_ptr->specificationMode.t) &&
      rbrc_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent))
  {
    /* Save the linked list head pointer */
    if (FAILURE == tdsrrcllc_validate_rb_info_reconfig_list_r6(
        &rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList))
    {
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
    /* If UTRAN DRX coefficient is not present then it is invalid configuration */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,
          tdsrrc_RadioBearerReconfiguration_r6_IEs,utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_rbrc.state_indicated_by_msg);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    if (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      /*should we do it after getting l2ack for response??*/
      if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,
	  	tdsrrc_RadioBearerReconfiguration_r6_IEs,ura_Identity)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
        tdsrrccu_clear_ura_id ();
      }
      else
      {
        /*URA ID present in the message set ura_identity variable with this value */
        if (rbrc_ptr->ura_Identity.numbits == 16)
        {
          tdsrrccu_update_ura_id (&(rbrc_ptr->ura_Identity));
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rbrc_ptr->ura_Identity.numbits);
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
          tdsrrc_rbrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
          return;
        }
      }
    }
  } // if TDSRRC_STATE_CELL_PCH or TDSRRC_STATE_URA_PCH

}/* end function tdsrrcrbrc_validate_rbrc_message_r6 */

/*===========================================================================

FUNCTION   VALIDATE_RBRC_MESSAGE_R7

DESCRIPTION

  This function validates the received RB Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Reconfig Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_validate_rbrc_message_r7
(
  tdsrrc_RadioBearerReconfiguration_r7_IEs *rbrc_ptr /* Pointer to the RB RC message */
)
{
  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();

  /* Assume the message is valid for now */
  tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_VALID;


  /* Check the RRC State indicatior given by the message and 
     store it in the tdsrrc_rbrc internal memory
   */
  switch (rbrc_ptr->rrc_StateIndicator)
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;

    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;

    default:
      /* No other states are supported right now. */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported state %d in RBRC R7 Msg",rbrc_ptr->rrc_StateIndicator);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;
  }

  if (tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG)
  {
    return;
  }

  /* Validate DL HSPDSCH */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r7_IEs,dl_HSPDSCH_Information))
  {
    /* Check if network does not try to initiate HSDPA with invalid state */
    if (tdsrrc_hsdpa_initial_validation(rbrc_ptr->rrc_StateIndicator, 
                                    TDSRRC_CHECK_COMMON_MSG_TYPE(rbrc_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
                                     rrc_DL_HSPDSCH_Information_r7_modeSpecificInfo_fdd)) == FAILURE)
    {
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  /* Not support to the pre-config option. */
  if (rbrc_ptr->specificationMode.t != 
      T_tdsrrc_RadioBearerReconfiguration_r7_IEs_specificationMode_complete)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Non-support preconfig t in RBRC");
    tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    return;
  }

  

  /* Obtain U_RNTI if present for SRNS relocation.  */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r7_IEs,new_U_RNTI))
  {
    /* check whether the SRNS relocation is within Cell_DCH or not.
       SRNS relocation information can come in reconfiguration messages
       only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbrc.state_indicated_by_msg);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* message with valid SNRS relocation info */
    else
    {
      /* Set the flag that indicates that this procedure
         involves SRNS relocation */
      tdsrrc_rbrc.rbrc_for_srns_relocation = TRUE;
      /*store the new U-RNTI */
      tdsrrc_translate_urnti (&(rbrc_ptr->new_U_RNTI), &tdsrrc_rbrc.new_urnti);
    }
  }

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  tdsrrc_rbrc.trans_to_dch_ciph_config_needed = FALSE;
  if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    /* check to see if dl-dpch-common info is present while going to dch */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,  
         tdsrrc_RadioBearerReconfiguration_r7_IEs,dl_CommonInformation)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rbrc_ptr->dl_CommonInformation,
         tdsrrc_DL_CommonInformation_r7,dl_dpchInfoCommon)))
    {
      if (rbrc_ptr->dl_CommonInformation.dl_dpchInfoCommon.t == 
          T_tdsrrc_DL_CommonInformation_r7_dl_dpchInfoCommon_dl_DPCH_InfoCommon)
      {
        /* check if ciphering is active in mac for cs domain, if it is then,
          start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
        */
        if (tdsrrcsmc_tm_ciph_active() == TRUE)
        {
          uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
          uecomdef_status_e_type loc_ret_val;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");
          loc_ret_val = tdsrrcrb_get_mac_d_hfn(
               (TDSRRC_MSG_COMMON_BITMASK_IE_PTR( 
               rbrc_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon,
               tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
               &rbrc_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
               &mac_d_hfn_l);
          if(loc_ret_val == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
          }
          if (tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing Ciphering Config for HHO failed");
          }
          tdsrrc_rbrc.trans_to_dch_ciph_config_needed = TRUE;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
        }
      }
    }
  }

  if (rbrc_ptr->specificationMode.u.complete.m.rab_InformationReconfigListPresent)
  {

    tdsrrc_rbrc.status.failure_status = tdsrrc_update_rab_info_reconfig_param(
      &rbrc_ptr->specificationMode.u.complete.rab_InformationReconfigList, 
      &tdsrrc_rbrc.rabs,
      &nas_sync_ind
      );
                                           
   if( tdsrrc_rbrc.status.failure_status != TDSRRCRB_MSG_VALID) 
   {
      return;
    }
    // for each RAB info
  } //RAB info IE present
  tdsrrc_set_ordered_codec_mode(nas_sync_ind);
 

  if((T_tdsrrc_RadioBearerReconfiguration_r7_IEs_specificationMode_complete ==
     (rbrc_ptr->specificationMode.t) &&
      rbrc_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent))
  {
    /* Save the linked list head pointer */
    if (FAILURE == tdsrrcllc_validate_rb_info_reconfig_list_r7(
        &rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList))
    {
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
    /* If UTRAN DRX coefficient is not present then it is invalid configuration */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,
          tdsrrc_RadioBearerReconfiguration_r7_IEs,utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_rbrc.state_indicated_by_msg);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    if (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      /*should we do it after getting l2ack for response??*/
      if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r7_IEs,ura_Identity)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
        tdsrrccu_clear_ura_id ();
      }
      else
      {
        /*URA ID present in the message set ura_identity variable with this value */
        if (rbrc_ptr->ura_Identity.numbits == 16)
        {
          tdsrrccu_update_ura_id (&(rbrc_ptr->ura_Identity));
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rbrc_ptr->ura_Identity.numbits);
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
          tdsrrc_rbrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
          return;
        }
      }
    }
  } // if TDSRRC_STATE_CELL_PCH or TDSRRC_STATE_URA_PCH

}/* end function tdsrrcrbrc_validate_rbrc_message_r7 */


/*===========================================================================

FUNCTION   RRCRBRC_SEND_CHANNEL_CONFIG_REQ

DESCRIPTION

  This function sends the TDSRRC_CHANNEL_CONFIG_REQ command to LLC
  to reconfig the necessary RBs associated with a RAB. If signalling
  Radio bearers are affected and included in the Radio Bearer
  Reconfig Message, they will also be sent in the same command.
  
  Currently the messege is not expected to have Signalling
  Radio Bearers for reconfig. If these are added in the future,
  they will also need to be sent in the same command.
        
DEPENDENCIES

  TDSORDERED_CONFIG must be set before calling this function.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_send_channel_config_req
(
  void
)
{

  tdsrrc_cmd_type *ptr;          /* To hold the Command                       */
  uint32 rb_count;            /* To count the number of RBs in the command */
  uint32 rb_index=0;          /* Index for the RB list */
  #ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
  uint8 i = 0;
  boolean um_rrc_srb_exist = FALSE;
  boolean am_rrc_srb_exist = FALSE;
  boolean am_high_priority_srb_exist = FALSE;
  boolean am_low_priority_srb_exist = FALSE;
  rlc_lc_id_type ul_rlc_lc_id = TDSRRCLCM_RLC_LC_ID_NOT_FOUND;
  #endif
  #ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
  #ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrc_RB_Identity srb5_rb_id;
  #endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
  #endif

  /* Get the command buffer */
  if((ptr = tdsrrc_get_int_cmd_buf())!=NULL)
  {
    /* Fill in the command id  */
    ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;

    /* First fill up the Radio Bearers that need to be reconfigured.
    For this we need to know how many RBs need to be reconfigured.
    */
    
    if( tdsrrc_rbrc.rbs_tobe_reconfigured.is_valid == TRUE )
    {
      for(rb_count=0; rb_count<tdsrrc_rbrc.rbs_tobe_reconfigured.num_rbs; rb_count++)
      {
            /* Fill up the RB id and indicate that it needs to be reconfigured */
            ptr->cmd.chan_config_req.rb[rb_index].rb_id = 
              tdsrrc_rbrc.rbs_tobe_reconfigured.rb_id[rb_count];

         ptr->cmd.chan_config_req.rb[rb_index].rb_config = TDSRECONFIG_RB;
         ptr->cmd.chan_config_req.rb[rb_index].rb_config = TDSRECONFIG_RB_OTA;
         ptr->cmd.chan_config_req.rb[rb_index].ul_cfg_mask = tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[rb_count];
         ptr->cmd.chan_config_req.rb[rb_index].dl_cfg_mask = tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[rb_count];
         rb_index++;
         
      }
    }/* end if tdsrrc_rbrc.rbs_tobe_reconfigured.is_valid  == TRUE */
    if( tdsrrc_rbrc.rbs_tobe_affected.is_valid == TRUE )
    {
      for(rb_count=0; rb_count<tdsrrc_rbrc.rbs_tobe_affected.num_rbs; rb_count++)
      {
            /* Fill up the RB id and indicate that it needs to be affected */
            ptr->cmd.chan_config_req.rb[rb_index].rb_id = 
              tdsrrc_rbrc.rbs_tobe_affected.rb_id[rb_count];
            ptr->cmd.chan_config_req.rb[rb_index].rb_config = TDSRECONFIG_RB_OTA;
            ptr->cmd.chan_config_req.rb[rb_index].ul_cfg_mask = ptr->cmd.chan_config_req.rb[rb_index].dl_cfg_mask = TDSMODIFY_RB_CFG;
            rb_index++;
      }
    }/* end if tdsrrc_rbrc.rbs_tobe_affected.is_valid  == TRUE */



    /* Check if state change is required */
    if( (tdsrrc_get_state()) == (tdsrrc_rbrc.state_indicated_by_msg) )
    {
      ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
    }
    else
    {
      /* The message indicates that state change is required.
      Indicate to LLC what the next state should be. */
      ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
      ptr->cmd.chan_config_req.next_state                = tdsrrc_rbrc.state_indicated_by_msg;

      if ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
          ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) || 
           (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)))
      {
        /* UE is directly transitioning to Cell_PCH state from Cell_FACH state so
        include release of CCCH and establishment of PCCH in the same channel config
        request */
        ptr->cmd.chan_config_req.rb[rb_index].rb_id       = CCCH_RADIO_BEARER_ID;
        ptr->cmd.chan_config_req.rb[rb_index++].rb_config = TDSRELEASE_RB;
        {
        ptr->cmd.chan_config_req.rb[rb_index].rb_id       = PCCH_RADIO_BEARER_ID;
        ptr->cmd.chan_config_req.rb[rb_index++].rb_config = TDSESTABLISH_RB;
        }
      }
    }
    #ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
    if (TDSCHECK_RLC_ALL_RB_REEST_ENABLE)
    {
    if (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH)
    {
        for( i =0;i<rb_index;i++)
        {
          if (ptr->cmd.chan_config_req.rb[i].rb_id == DCCH_UM_RADIO_BEARER_ID)
          {
            um_rrc_srb_exist = TRUE;
            continue;
          }
          else if (ptr->cmd.chan_config_req.rb[i].rb_id == DCCH_AM_RADIO_BEARER_ID)
          {
            am_rrc_srb_exist = TRUE;
            continue;
          }
          else if (ptr->cmd.chan_config_req.rb[i].rb_id == DCCH_DT_HIGH_PRI_RADIO_BEARER_ID)
          {
            am_high_priority_srb_exist = TRUE;
            continue;
          }
          else if (ptr->cmd.chan_config_req.rb[i].rb_id == DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
          {
            am_low_priority_srb_exist = TRUE;
            continue;
          }
        }
          /*reconfig signalling RBs */
        if (um_rrc_srb_exist == FALSE)
        {
      ptr->cmd.chan_config_req.rb[rb_index].rb_id = DCCH_UM_RADIO_BEARER_ID;
      ptr->cmd.chan_config_req.rb[rb_index++].rb_config = TDSRECONFIG_RB;
        }
        if (am_rrc_srb_exist == FALSE)
        {
      ptr->cmd.chan_config_req.rb[rb_index].rb_id = DCCH_AM_RADIO_BEARER_ID;
      ptr->cmd.chan_config_req.rb[rb_index++].rb_config = TDSRECONFIG_RB;
        }
        if (am_high_priority_srb_exist == FALSE)
        {
      ptr->cmd.chan_config_req.rb[rb_index].rb_id = DCCH_DT_HIGH_PRI_RADIO_BEARER_ID;
      ptr->cmd.chan_config_req.rb[rb_index++].rb_config = TDSRECONFIG_RB;
        }
        if (am_low_priority_srb_exist == FALSE)
        {
      /* check whether RB4 was present or not */
      ul_rlc_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH, 
                                   DCCH_DT_LOW_PRI_RADIO_BEARER_ID,
                                   UE_MODE_ACKNOWLEDGED_DATA);

      if(ul_rlc_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"No RB4 in CELL_FACH");
      }
      else 
      {
        ptr->cmd.chan_config_req.rb[rb_index].rb_id = DCCH_DT_LOW_PRI_RADIO_BEARER_ID;
        ptr->cmd.chan_config_req.rb[rb_index++].rb_config = TDSRECONFIG_RB;
      }

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      if(tdsrrclcm_get_rb_id_if_dl_lc_type_exists(UE_LOGCHAN_TM_DCCH_DL, &srb5_rb_id) == TDSRRCLCM_SUCCESS)
      {
        ptr->cmd.chan_config_req.rb[rb_index].rb_id       = srb5_rb_id;
        ptr->cmd.chan_config_req.rb[rb_index++].rb_config = TDSRECONFIG_RB;
      }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
    }
      }
    }
    #endif

    /* Number of RBs to config. Note rec_rb_count is the total
    of reconfigured and affected RBs */
    ptr->cmd.chan_config_req.num_rb_to_config = (uint16)rb_index;
    if(rb_index < TDS_MAX_RB)
    {
      tdsrrc_fill_rb_id_chan_req_from_standalone_db(&rb_index, &ptr->cmd.chan_config_req);
    }

    if(tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
    {
      /* Fill those RBs in chan_config_req for which RLC PDU size has changed and 
      that are already not present in chan_config_req*/
      tdsrrc_fill_rb_id_chan_req_from_rb_list_mapped_to_dch(&rb_index, &ptr->cmd.chan_config_req);
    }
  
    /*If state transition is Cell_DCH->Cell_FACH or vice-verca, add all the AM PS RBs
     in chan_config_req*/	
    if(((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH) && (tdstransition_config.toc_usage 
        == TDSTOC_FOR_DCH_AND_FACH)) || ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
        (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH))
        #ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
        ||((TDSCHECK_RLC_ALL_RB_REEST_ENABLE)&&(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
        (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH))
        #endif
        )
    {
      tdsrrc_fill_rb_id_chan_req_from_est_rabs(&rb_index, &ptr->cmd.chan_config_req);
    }

    /* Indicate that we need a confirmation for this command */
    ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

    /* Indicate the id of the procedure sending the command */
    ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RBRC;


    /* We are not required to fill any other data for this command.
    Now, send the command */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Sending CHANNEL_CONFIG_REQ RBs:%d, curr_st:%d, next_st:%d", 
               rb_index, tdsrrc_get_state(), tdsrrc_rbrc.state_indicated_by_msg);
    tdsrrc_put_int_cmd( ptr );

  }
  else
  {
    /* Out of memory!!  */
    ERR_FATAL("Can't send channel config! No memory!",0,0,0);
  }

}/* end function tdsrrcrbrc_send_channel_config_req */


/*===========================================================================

FUNCTION  RRCRBRC_CLEAR_PROCEDURE

DESCRIPTION

  This function clears all the internal variables in
  the RBRC procedure and brings them to their default
  values.
    
DEPENDENCIES

  The Substate of the procedure should be changed
  to initial substate before calling this function.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_clear_procedure( void )
{
  uint32 rab_index;
  /* Initialize the transaction id's for this procedure.
  Use a number that can't be an actual received transaction
  id. */
  tdsrrc_rbrc.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  tdsrrc_rbrc.tr_ids.rejected_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;

  tdsrrc_rbrc.new_crnti_valid       = FALSE;
  tdsrrc_rbrc.directed_cell_info    = FALSE;
  tdsrrcrbrc_send_initiate_cu_flag = FALSE;
  tdsrrcrbrc_delete_crnti          = FALSE;
  /* Indicate that there are no RBs to be reconfigured and
  no rbs tobe affected. This is the default value. */
  tdsrrc_rbrc.rbs_tobe_reconfigured.is_valid = FALSE;
  tdsrrc_rbrc.rbs_tobe_affected.is_valid     = FALSE;

  if (tdsrrc_rbrc.cn_info_saved != FALSE)
  {
    /* free the memory allocated for mm cmd */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Free MM cmd buffer %x",tdsrrc_rbrc.mm_cmd_ptr);
    mm_free_cmd_buf(tdsrrc_rbrc.mm_cmd_ptr);
    /* set cn_info_saved to false */
    tdsrrc_rbrc.cn_info_saved = FALSE;
#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER      
    /*send SRNS reallocation ind to nas with success*/
    if(tdsrrc_rbrc.srns_reallocation_trigger == TRUE)
    {
	 tdsrrc_rbrc.srns_reallocation_trigger = FALSE;
    }
#endif    
  }

  tdsrrc_rbrc.rbrc_for_srns_relocation = FALSE;

  /*clear the rab information */
  for (rab_index=0; rab_index<TDSRRCRB_MAX_RAB_PER_MSG; rab_index++)
  {
    tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].num_rbs_for_rab = 0;
    tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_UNKNOWN_RAB;    
    tdsrrc_rbrc.rabs.chan_config_needed[rab_index] = FALSE;
    tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_NONE;
  }
  tdsrrc_rbrc.rabs.num_rabs_in_msg = 0;

  tdsrrc_rbrc.trans_to_dch_ciph_config_needed = FALSE;

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  tdsrrc_rbrc.initiate_cell_selection_trans_from_fach = FALSE;     
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

  tdsrrc_rbrc.high_mobility_ind = FALSE;

  tdsrrc_set_ordered_codec_mode(tdsrrc_get_ordered_codec_mode());
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
  tdsrrc_set_ordered_codec_status(FALSE);
#endif


}



/*===========================================================================

FUNCTION   RRCRBRC_PROCESS_SUCCESSFUL_PROC_COMPLETION

DESCRIPTION

  This function does the final steps when the procedure is
  about to complete successfully.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_process_successful_proc_completion
(
 boolean indicate_rlc
)
{
  /* send the nas cn info if stored */
  if (tdsrrc_rbrc.cn_info_saved != FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_SERVICE_IND_FROM_OTA_MSG due to CN info to MM");
    if(tdsrrc_rbrc.mm_cmd_ptr->cmd.hdr.message_id == (byte) RRC_SERVICE_IND_FROM_OTA_MSG )
    {
      /* Update the cell access information in the service info structure*/
      tdsrrcccm_update_service_info(tdsrrc_rbrc.mm_cmd_ptr);
    }
    /* Put the command on MM queue */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(tdsrrc_rbrc.mm_cmd_ptr);
    }
    mm_put_cmd(tdsrrc_rbrc.mm_cmd_ptr);
    /* clear the flag */
    tdsrrc_rbrc.cn_info_saved = FALSE;
#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER      
    /*send SRNS reallocation ind to nas with success*/
    if(tdsrrc_rbrc.srns_reallocation_trigger == TRUE)
    {
	 tdsrrc_rbrc.srns_reallocation_trigger = FALSE;
    }
#endif    
  }
  /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
  configurations */
  tdsrrcllc_clear_ordered_config();  

  if (((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) || 
       (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH )) &&
      (indicate_rlc == TRUE))
  {
    /* register with RLC for uplink data indication */
    tdsrrcrb_register_with_rlc_for_ul_data_ind (TDSRRC_PROCEDURE_RBRC);
  }

  /* Go to the next sub-state in the state machine. That's
  the initial substate since we've sent the RB RC complete
  message */
  tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

  /* At this point we don't need to remember any of the
  Radio Bearer information since it's stored in ESTABLISHED_RABS.
  Clear our internal variables */
  tdsrrcrbrc_clear_procedure();

  if (tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_RB_PROC_COMPL_IND) 
  {
    tdsrrcrb_send_rb_proc_compl_ind();
  }
}

/*===========================================================================

FUNCTION   tdsrrcrbrc_check_and_send_failure_for_simul_reconfig

DESCRIPTION

  This function check whether received reconfig message results in simultaneous
  reconfig or not. If yes, then it sends a failure message to NW and returns FAILURE.
  If no, then it updates the accepted transaction id and return SUCCESS
          
DEPENDENCIES

  None.

REUSABILITY: R99/R5/R6

RETURN VALUE

  SUCCESS or FAILURE

SIDE EFFECTS

  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrcrbrc_check_and_send_failure_for_simul_reconfig
(
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id
)
{
  tdsrrcrb_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;
  
  if ((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
      (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
      (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incomatible simultaneous reconfig! Sending RBRC failure");
    if ( tdstransaction_id == tdsrrc_rbrc.tr_ids.accepted_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBRC msg - accepted tr-id%d match!",tdsrrc_rbrc.tr_ids.accepted_transaction);
      /* Nothing else to do, just return */
    }
    else if ( tdstransaction_id == tdsrrc_rbrc.tr_ids.rejected_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBRC msg - rejected tr-id%d match!",tdsrrc_rbrc.tr_ids.rejected_transaction);
      /* Nothing else to do, just return */
    }
    else
    {
      /* We have received a RB RC message with a new transaction id.
      This message should be in our rejected transaction list */
      temp_failure_cause.failure_status = tdsrrc_rbrc.status.failure_status;
      temp_rejected_transaction = tdsrrc_rbrc.tr_ids.rejected_transaction;
      
      tdsrrc_rbrc.tr_ids.rejected_transaction = tdstransaction_id;
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;

      /*Check whether Cell Update is going on */
      if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
           (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
      {
        if(tdsrrc_rbrc.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbrc subsstate(%d) Send failure with %d cause",TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF,tdsrrc_rbrc.status.failure_status);
          tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                   tdsrrc_rbrc.status.prot_err_cause );
          tdsrrc_rbrc.status.failure_status = temp_failure_cause.failure_status;
          tdsrrc_rbrc.tr_ids.rejected_transaction = temp_rejected_transaction ;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
          (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBRC);
          /* Wait until Cell update procedure is completed */
          tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
        }
      }
      else
      {
        tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                   tdsrrc_rbrc.status.prot_err_cause );
        /*continue the same substate */ 
      }
    }
    return FAILURE;
  }

  /* We have received a RB RC message with a new transaction id.
   Accept the transaction */
  tdsrrc_rbrc.tr_ids.accepted_transaction = tdstransaction_id;

  return(SUCCESS);
}

/*===========================================================================

FUNCTION   tdsrrcrbrc_validate_and_check_integrity_for_srns_relocation

DESCRIPTION

  This function validates whether received tdsrrc_rbrc message is due to SRNS relocation,
  if yes, then it performs the integrity check on the received message because
  integrity check was skipped for message which triggers SRNS relocation.
          
DEPENDENCIES

  None.

REUSABILITY: R99/R5/R6

RETURN VALUE

  SUCCESS or FAILURE

SIDE EFFECTS

  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrcrbrc_validate_and_check_integrity_for_srns_relocation
(
  tdsrrc_cmd_type *cmd_ptr,
  /* Pointer to the RB Reconfig message        */
  tdsrrc_RadioBearerReconfiguration *msg_ptr,
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id,  
  tdsrrc_msg_ver_enum_type msg_version
)
{
  tdsrrcsmc_srns_ip_check_status_e_type ip_check_status = 
                                                 TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS;

  /* Check whether this reconfig message has SRNS 
  relocation info. If this is true then inform SMC so
  that it uses the integrity protection info present in the
  message to do integrity check for this message */

  if (TRUE == tdsrrcrbrc_validate_srns_relocation_info(msg_ptr, msg_version))
  {
    ip_check_status = tdsrrcsmc_check_integrity_for_srns_relocation(cmd_ptr
                                                                 , msg_version
                                                                 );
    switch (ip_check_status)
    {
      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS:
        /*continue further processing with the message */
        break;

      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE:
        /*Discard the message and continue with ongoing procedure as if this
        message was not received */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IP check failed. Discarding RBRC msg");
        break;

      case TDSRRCSMC_SRNS_SECURITY_INFO_INVALID:
      case TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION:
        if (tdstransaction_id == tdsrrc_rbrc.tr_ids.rejected_transaction )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBRC msg - rejected tr-id%d match!",tdsrrc_rbrc.tr_ids.rejected_transaction);
          /* Nothing else to do, just ignore the message and return */
        } 
        else
        {
          /* send a reconfig failure messge with invalid
          configuration and 
          continue the same substate with any ongoing procedure */
          tdsrrc_rbrc.tr_ids.rejected_transaction = tdstransaction_id;
          if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
          {
            tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          }
          else
          {
            tdsrrc_rbrc.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
          }
          /*Check whether Cell Update is going on */
          if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
               (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
            (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBRC);
            /* Wait until Cell update procedure is completed */
            tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
          }
          else
          {
            tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                            tdsrrc_rbrc.status.prot_err_cause );
          }
        }
        /*do not clear the procedure or change the tdsrrc_rbrc substate because
        some other tdsrrc_rbrc procedure might be in progress. Just return from
        here */
        break;

      default:
        /*should not come here */
        break;

    }
  }

  if (ip_check_status != TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS)
  {
    return FAILURE;
  }
  else
  {
    return SUCCESS;
  }
}
#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================
FUNCTION   tdsrrcrbrc_process_after_oc_config_set_r8

DESCRIPTION
  This function processes the R8 RBRC OTA after OC is set by this OTA. So if we call this function, it means
  that TDSORDERED_CONFIG_SET is returned while setting OC by RBRC OTA.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbrc_process_after_oc_config_set_r8
(
  tdsrrc_DL_DCCH_Message * dcch_msg_ptr, 
  tdsrrc_RadioBearerReconfiguration_r8_IEs  *rbrc_ptr
)
{
  tdsrrc_state_e_type                         current_state;       /* current RRC State  */
  tdsrrcrb_directed_cell_info_type        directed_cell_info; /* directed cell info struct */
  tdsrrc_RadioBearerReconfiguration   *msg_ptr; /* Pointer to the RB Reconfig message        */
  tdsrrc_int_c_rnti_type                      c_rnti;
  tdsrrc_int_u_rnti_type                      u_rnti;
  tdsrrcrb_rnti_update_info_type         rnti_update_info;   /* to hold rnti update info */

  /* Get the pointer to the actual RB Reconfig message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerReconfiguration);

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
  if(tdsrrcsib_is_current_event_sib_read_in_dch())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Force Init GPS ");
    tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
  }
#endif /*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/

  current_state = tdsrrc_get_state(); 

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  directed_cell_info.freq_info_present = FALSE;
  directed_cell_info.cpid_present = FALSE;  

  /* Populate the local variables to indicate if Re-direction info is present */
  if((current_state==TDSRRC_STATE_CELL_FACH) && 
    ((tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
    ||(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
    ||(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
  {
    if((TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,frequencyInfo)) &&
      (rbrc_ptr->frequencyInfo.modeSpecificInfo.t == T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd))
    {
      directed_cell_info.freq_info_present = TRUE;
      directed_cell_info.uarfcn_dl  = rbrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
    }
  
    if(tdsrrcrbrc_is_pccpch_info_present(msg_ptr, TDSMSG_REL8))
    {
      directed_cell_info.cpid_present = TRUE;
      directed_cell_info.cell_param_id = rbrc_ptr->dl_InformationPerRL_List.elem[0].
                  modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
    }
    /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
          FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
    if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
      (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
      if(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
      {
        tdsrrcrbrc_send_initiate_cu_flag = TRUE;
        tdsrrcrbrc_delete_crnti          = TRUE;
      }
      else
      {
        tdsrrc_rbrc.directed_cell_info = TRUE;
      }
    }  
  }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

  /*DCH->DCH, FACH->FACH, FACH->DCH*/
  if ((current_state==tdsrrc_rbrc.state_indicated_by_msg) ||((current_state==TDSRRC_STATE_CELL_FACH) && 
    (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH)))
  {
    /*check whether any frequency info is present in the message and transition is 
    CELL_FACH->CELL_FACH. In this case initiate a cell update after channel config is 
    successfully completed */
    if ((current_state==TDSRRC_STATE_CELL_FACH)&& (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
    {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU

      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if((tdsrrcrbrc_send_initiate_cu_flag == FALSE) && 
        (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
      {
        if(TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,new_C_RNTI))
        {
          tdsrrc_rbrc.new_crnti_valid = TRUE;
          tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
          tdsrrc_rbrc.new_crnti = (uint16)c_rnti;
        }
        else
        {
          tdsrrc_rbrc.new_crnti_valid = FALSE;
        }
        /* Now copy OC to TOC */
        tdsrrcllcoc_update_toc_with_oc_from_fach();
        /*Initiate the cell selection*/
        tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
                    TDSRRC_TRANSITION_FROM_FACH,tdsrrc_rbrc.state_indicated_by_msg);

        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

        /*We can return from here. Because FACH->FACH is done.*/
        return;
      }

#else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

      if ((TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,
        frequencyInfo)) && (rbrc_ptr->frequencyInfo.modeSpecificInfo.t == 
         T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd))
      {
        directed_cell_info.freq_info_present = TRUE;
        directed_cell_info.uarfcn_dl         = 
        rbrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd.uarfcn_Nt;
      }
      else
      {
        directed_cell_info.freq_info_present = FALSE;
      }
      if (tdsrrcrbrc_is_pccpch_info_present(msg_ptr, TDSMSG_REL8))
      {
        directed_cell_info.cpid_present = TRUE;
        directed_cell_info.cell_param_id = rbrc_ptr->dl_InformationPerRL_List.elem[0].
                                    modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
      }
      else
      {
        directed_cell_info.cpid_present = FALSE;
      }

      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
      {
        /*directed cell info matched. Cell update is not required */
        tdsrrcrbrc_send_initiate_cu_flag = FALSE;
      }
      else
      {
        /*directed cell info do not match. Cell update is required */
        tdsrrcrbrc_send_initiate_cu_flag = TRUE;
        tdsrrcrbrc_delete_crnti          = TRUE;
      }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
    } /* if CELL_FACH->CELL_FACH reconfiguration */

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");

    /* Call the function that creates and sends a channel config request to LLC. */
    tdsrrcrbrc_send_channel_config_req();
    
    /* Change the substate since we're waiting for the confirmation from LLC for the 
    channel configs */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
  }

  /*DCH -> FACH*/
  else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
    (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
  {
    if(TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,new_C_RNTI))
    {
      tdsrrc_rbrc.new_crnti_valid = TRUE;
      tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
      tdsrrc_rbrc.new_crnti = (uint16)c_rnti;
    }
    else
    {
      tdsrrc_rbrc.new_crnti_valid = FALSE;
    }

    /*Initiate the cell selection*/
    tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
              TDSRRC_TRANSITION_FROM_DCH, TDSRRC_STATE_CELL_FACH);

    tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
  
    /* Start T_305 [periodic cell update] timer */
    tdsrrccu_start_t_305_timer();
  }

  /*DCH -> Cell_PCH*/
  else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
    (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
  {
    tdsrrc_rbrc.directed_cell_info = tdsrrcrbrc_is_pccpch_info_present(msg_ptr, TDSMSG_REL8);
    /*Send the RRCRBRC complete msg on old configuration */
    tdsrrcrbrc_send_rbrc_complete_message(TRUE);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
    /*wait for L2ack */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    tdsrrcllc_print_trans_chnl_rate();
  }

  /*DCH -> URA_PCH*/
  else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
    (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
  {
    /* no need to check directed cell info in this case */
    /*Send the RRCRBRC complete msg on old configuration */
    tdsrrcrbrc_send_rbrc_complete_message(TRUE);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
    /*wait for L2ack */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    tdsrrcllc_print_trans_chnl_rate();
  }

  /*FACH -> PCH*/
  else if ((current_state==TDSRRC_STATE_CELL_FACH) && 
    ((tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
    (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
  {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /*call the function that evaluates the directed cell info and set the flags accordingly */
    if((tdsrrc_rbrc.directed_cell_info == FALSE) && 
      (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
    {
      if(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
      {
        tdsrrc_rbrc.directed_cell_info = tdsrrcrbrc_is_pccpch_info_present(msg_ptr, TDSMSG_REL8);
      }
      tdsrrc_rbrc.initiate_cell_selection_trans_from_fach = TRUE;
    }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
  
    /*Send the RRCRBRC complete msg on old configuration */
    /* check whether new C-RNTI was present in the message. In that case we have to configure 
    MAC with new C-RNTI before sending the response message */
    if(TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,new_C_RNTI))
    {
      tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
      rnti_update_info.crnti_action = TDSRRC_RNTI_UPDATE;
      rnti_update_info.crnti        = (uint16)c_rnti;

      /* check whether URNTI is also present in the message */
      if(TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,new_U_RNTI))
      {
        tdsrrc_translate_urnti(&rbrc_ptr->new_U_RNTI, &u_rnti);
        rnti_update_info.urnti_action = TDSRRC_RNTI_UPDATE; 
        rnti_update_info.urnti        = u_rnti; 
      }
      else
      {
        rnti_update_info.urnti_action = TDSRRC_RNTI_NOCHANGE; 
      }

      rnti_update_info.procedure                = TDSRRC_PROCEDURE_RBRC;
      rnti_update_info.rlc_re_establish_srb  = FALSE;
      rnti_update_info.rlc_re_establish_rab  = FALSE;
      rnti_update_info.cnf_reqd                  = TRUE;

      /* Send a RNTI Update Request */
      tdsrrcrb_send_rnti_update_req(rnti_update_info);

      /* wait for the RNTI update confirm before sending the response message */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
    }
    else
    {
      tdsrrcrbrc_send_rbrc_complete_message(TRUE);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
      /*wait for L2ack */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
      tdsrrcllc_print_trans_chnl_rate();
    }
  }
  return;
}

/*===========================================================================
FUNCTION   tdsrrcrbrc_process_rb_reconfig_list_r8

DESCRIPTION
  This function proceses RB Reconfig List for R8 RBRC OTA.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbrc_process_rb_reconfig_list_r8
(
  tdsrrc_RB_InformationReconfigList_r8  *rb_info_reconfig_list_r8_ptr 
)
{
  uint8  localrb_count = 0;
  tdsrrcrbrc_rlc_stop_cont_e_type  rlc_stop_cont_e_value = TDSRB_STOP_CONTINUE_ABSENT;  
  tdsrrc_RB_InformationReconfig_r8 * rb_info_reconfig_ptr=NULL;
  /*store the rb ids tobe released */
  tdsrrc_rbrc.rbs_tobe_reconfigured.is_valid = TRUE;

  while (rb_info_reconfig_list_r8_ptr->n > localrb_count)
  {
      // Initialize the enum to this value.
      rlc_stop_cont_e_value = TDSRB_STOP_CONTINUE_ABSENT;
      rb_info_reconfig_ptr = &rb_info_reconfig_list_r8_ptr->elem[localrb_count];
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr, 
          tdsrrc_RB_InformationReconfig_r8,rb_StopContinue))
      {
        if (rb_info_reconfig_ptr->rb_StopContinue == tdsrrc_RB_StopContinue_stopRB)
        {
          rlc_stop_cont_e_value = TDSRB_STOP;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Stop recvd for RB %d",(uint16)(rb_info_reconfig_ptr->rb_Identity));
        }
        else if (rb_info_reconfig_ptr->rb_StopContinue == tdsrrc_RB_StopContinue_continueRB)
        {
          rlc_stop_cont_e_value = TDSRB_CONTINUE;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Continue recvd for RB %d",(uint16)(rb_info_reconfig_ptr->rb_Identity));
        }
      }

      tdsrrc_rbrc.rbs_tobe_reconfigured.rb_id[localrb_count] = 
          rb_info_reconfig_ptr->rb_Identity;

      tdsrrc_rbrc.rbs_tobe_reconfigured.rb_stop_continue[localrb_count] = rlc_stop_cont_e_value;

      tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] = tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] = 0;

      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,tdsrrc_RB_InformationReconfig_r8,rlc_Info))&&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode)))
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSMODIFY_RB_CFG;
      }
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,tdsrrc_RB_InformationReconfig_r8,rlc_Info))&&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)))
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSMODIFY_RB_CFG;
      }
      if(rlc_stop_cont_e_value == TDSRB_STOP)
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSSTOP_RB_CFG;
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSSTOP_RB_CFG;
      }
      else if(rlc_stop_cont_e_value == TDSRB_CONTINUE)
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSCONTINUE_RB_CFG;
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSCONTINUE_RB_CFG;
      }

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"REl8: RB count %d, rb_id %d, rlc_stop_cont %d", localrb_count,
               (uint16)(rb_info_reconfig_ptr->rb_Identity), rlc_stop_cont_e_value);

      localrb_count++;

  }

  tdsrrc_rbrc.rbs_tobe_reconfigured.num_rbs = localrb_count;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Num of RBs tobe reconfigured:%d",localrb_count);
}

/*===========================================================================
FUNCTION   RRCRBRC_UPDATE_ORDERED_CONFIG_R8

DESCRIPTION
  This function processes the received Channel Config Confirmation
  from LLC. If the required Radio-bearers have been reconfig succesfully,
  and if any of the radio-bearers map to radio-access-bearers, this
  information needs to be given to the NAS entity for that RAB.
  
  Currently Circuit-Switched voice and Circuit-Switched data RABs are 
  supported. If a voice RAB has been assigned by the UTRAN, this function
  gets the RLC ids and the AMR mode information from LLC and sends this
  information to voice services. If a CS Data RAB has been assigned, the
  RLC ids for those RBs are sent to the data services entity. Then this
  function sends the RRC_SYNC_IND primitive to MM.
          
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbrc_update_ordered_config_r8
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr,      /* Pointer to the RB Reconfig message */
  tdsrrc_msg_ver_enum_type                   msg_version
)
{
  tdsrrcllc_oc_status_e_type                oc_status;/* To hold tdsordered_config status  */
  /* Used to indicate if an error occured while setting tdsordered_config. */
  boolean                                      ordered_config_err_occured = FALSE; 

  /* We have to reconfigure RBs. First set TDSORDERED_CONFIG so that LLC can process the 
  necessary parameters. */
  oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBRC,
                     tdsrrc_rbrc.state_indicated_by_msg, tdsrrc_DL_DCCH_Message_PDU,
                     (void *)dcch_msg_ptr);
  
  switch (oc_status)
  {
    case TDSORDERED_CONFIG_SET:
      tdsrrcrbrc_process_after_oc_config_set_r8(dcch_msg_ptr,
          &dcch_msg_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.
          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.
          radioBearerReconfiguration_r8);
      break;

    case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
      /* Send the RB Release Failure Message */
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause );

      /* Go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
      tdsrrc_rbrc.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      /* Send the RB Release Failure Message */
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause );

      /* Go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;
      
      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

       TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
       tdsrrc_rbrc.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
       /* Send the RB Release Failure Message */
       tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause
                               );
   
       /* Go back to initial substate */
       tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
   
       /* Indicate that an error occured */
       ordered_config_err_occured = TRUE;
       break;
        
    case TDSORDERED_CONFIG_NOT_SET_OTHER:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
      /* When LLC sends this, there is a serious problem
      and we'll go back to disconnected state. No need
      to send the failure message here, we can just
      go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    default:

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
      /* Unknown error, just go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;
  }/* end switch */

  if (ordered_config_err_occured == TRUE)
  {
    /* No need to continue. Clear the procedure variables and return. */
    tdsrrcrbrc_clear_procedure();
    return;
  }
  else
  {
    if (tdsrrc_rbrc.rbrc_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }
  }
}



/*===========================================================================
FUNCTION   VALIDATE_RBRC_MESSAGE_R8

DESCRIPTION
  This function validates the received RB Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Reconfig Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbrc_validate_rbrc_message_r8
(
  tdsrrc_RadioBearerReconfiguration_r8_IEs *rbrc_ptr /* Pointer to the R8 RB RC message */
)
{
  /*For CS RABs (if present) the codec type will be updated in tdsrrc_ota_codec_mode_status
  even if RAB-InfoReconfig List isn't present*/
  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();

  /* Assume the message is valid for now */
  tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_VALID;

  /* Check the RRC State indicatior given by the message and 
     store it in the tdsrrc_rbrc internal memory */
  switch (rbrc_ptr->rrc_StateIndicator)
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;

    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;

    default:
      /* No other states are supported right now. */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported state %d in RBRC R8 Msg",rbrc_ptr->rrc_StateIndicator);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;
  }

  if (tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG)
  {
    return;
  }

  /* Validate DL HSPDSCH */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,dl_HSPDSCH_Information))
  {
    /* Check if network does not try to initiate HSDPA with invalid state */
    if (tdsrrc_hsdpa_initial_validation(rbrc_ptr->rrc_StateIndicator, 
                                    TDSRRC_CHECK_COMMON_MSG_TYPE(rbrc_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
                                     rrc_DL_HSPDSCH_Information_r8_modeSpecificInfo_fdd)) == FAILURE)
    {
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  /*RRC_GKG: Find out whether any Reconfig OTA support Pre-Reconfig*/
  /* Not support to the pre-config option. */
  if (rbrc_ptr->specificationMode.t != 
      T_tdsrrc_RadioBearerReconfiguration_r8_IEs_specificationMode_complete)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: Non-support preconfig t in RBRC");
    tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    return;
  }

  /* Obtain U_RNTI if present for SRNS relocation.  */
  if(TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,new_U_RNTI))
  {
    /* check whether the SRNS relocation is within Cell_DCH or not.
       SRNS relocation information can come in reconfiguration messages
       only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"REL8: SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbrc.state_indicated_by_msg);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* message with valid SNRS relocation info */
    else
    {
      /* Set the flag that indicates that this procedure involves SRNS relocation */
      tdsrrc_rbrc.rbrc_for_srns_relocation = TRUE;

      /*store the new U-RNTI */
      tdsrrc_translate_urnti(&(rbrc_ptr->new_U_RNTI), &tdsrrc_rbrc.new_urnti);
    }
  }

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch */
  tdsrrc_rbrc.trans_to_dch_ciph_config_needed = FALSE;
  /*RRC_GKG: Even though, any inconsistency exists in Security parameters, we don't fail the OTA.
  Check whether it's as per Spec.*/
  if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    /* check to see if dl-dpch-common info is present while going to dch */
    if ((TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,dl_CommonInformation)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rbrc_ptr->dl_CommonInformation,
         tdsrrc_DL_CommonInformation_r8,dl_dpchInfoCommon)))
    {
      if (rbrc_ptr->dl_CommonInformation.dl_dpchInfoCommon.t == 
          T_tdsrrc_DL_CommonInformation_r8_dl_dpchInfoCommon_dl_DPCH_InfoCommon)
      {
        /* check if ciphering is active in mac for cs domain, if it is then,
          start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
        */
        if (tdsrrcsmc_tm_ciph_active() == TRUE)
        {
          uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
          uecomdef_status_e_type loc_ret_val;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: To-DCH-Step-1 config activated");

          loc_ret_val = tdsrrcrb_get_mac_d_hfn((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon,
		  	   tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
               &rbrc_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
               &mac_d_hfn_l);

          if(loc_ret_val == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
          }
          if (tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing Ciphering Config for HHO failed");
          }
          tdsrrc_rbrc.trans_to_dch_ciph_config_needed = TRUE;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: To-DCH-Step-1 not needed");
        }
      }
    }
  }

  /*Check the RABs in the List.
  1. Make sure that the given RABs are in EST_RABs.
  2. Update the NAS-Sync Ind for CS RABs, if present.*/
  if (rbrc_ptr->specificationMode.u.complete.m.rab_InformationReconfigListPresent)
  {
    tdsrrc_rbrc.status.failure_status = tdsrrc_update_rab_info_reconfig_param_r8(
    &rbrc_ptr->specificationMode.u.complete.rab_InformationReconfigList, &tdsrrc_rbrc.rabs 
                                             ,&nas_sync_ind
                                           );
   if( tdsrrc_rbrc.status.failure_status != TDSRRCRB_MSG_VALID) 
   {
      return;
    }
  }

  tdsrrc_set_ordered_codec_mode(nas_sync_ind);

  /*Do the validations for RB-Reconfig List
  1. The RB-Id should be present in EST_RABs
  2. If RB is being STOP/CONTINUE, then it can't be SRB1/2 and TM URBs
  3. Check for PDCP Info SRNS relocation etc.*/
  if((T_tdsrrc_RadioBearerReconfiguration_r8_IEs_specificationMode_complete ==
     (rbrc_ptr->specificationMode.t) &&
      rbrc_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent))
  {
    /* Save the linked list head pointer */
    if (FAILURE == tdsrrcllc_validate_rb_info_reconfig_list_r8(
        &rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList))
    {
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  /*Cell_PCH/URA_PCH specific validations - 
     1. UTRAN DRX Cycle Length Coeff has to be present.
     2.  If state is URA_PCH, and URA-ID is present then check for it's length
  */
  if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
    /* If UTRAN DRX coefficient is not present then it is invalid configuration */
    if (!(TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,
      utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8: UTRAN DRX Coef not preset,State:%d",tdsrrc_rbrc.state_indicated_by_msg);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    if (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      /*If UE state is Cell_FACH then set the flag tdsrrccu_ura_update_required if more than one URA-IDs are
      present in SIB2*/
      if (!(TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,ura_Identity)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: URA-ID not present for URA_PCH state.");
        tdsrrccu_clear_ura_id();
      }
      else
      {
        /*URA ID present in the message set ura_identity variable with this value */
        if (rbrc_ptr->ura_Identity.numbits == 16)
        {
          tdsrrccu_update_ura_id (&(rbrc_ptr->ura_Identity));
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8: Invalid URA-ID length%d",rbrc_ptr->ura_Identity.numbits);
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
          tdsrrc_rbrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
          return;
        }
      }
    }
  } // if TDSRRC_STATE_CELL_PCH or TDSRRC_STATE_URA_PCH

}/* end function tdsrrcrbrc_validate_rbrc_message_r7 */

/*===========================================================================
FUNCTION   tdsrrcrbrc_process_rbrc_message_r8

DESCRIPTION
  This function processes the received RB Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Reconfig Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Reconfig Failure message
    will be sent to UTRAN.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbrc_process_rbrc_message_r8
(
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id,
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr  ,
  tdsrrc_RadioBearerReconfiguration_r8_IEs *rbrc_ptr
)
{
  tdsrrc_CN_InformationInfo_r6         *cn_info_ptr = NULL; 
  uint32                                       localrb_count;
  boolean                                     is_valid_rbid = TRUE;
  uint16                                       invalid_rbid = 0xff;
  tdsrrc_msg_ver_enum_type                   msg_version = TDSMSG_REL8;
  tdsrrc_RB_InformationAffectedList_r8  *rb_info_affected_list_r8_ptr = NULL;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Recvd RBRC Msg sub_st%d",tdsrrc_rbrc.curr_substate);

  if (FAILURE == tdsrrcrbrc_check_and_send_failure_for_simul_reconfig(tdstransaction_id))
  {
    return;
  }

  if(NULL == rbrc_ptr)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"rbrc_ptr is NULL.");
    return;
  }

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rbrc_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rbrc_ptr->m.activationTimePresent,
         rbrc_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBRC SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbrc_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rbrc_ptr->m.activationTimePresent = 0;
    }
  }

#endif

  /* Validate the Message to make sure there is no invalid or incompatible configuration given by the message */
  tdsrrcrbrc_validate_rbrc_message_r8( rbrc_ptr );


  if ( tdsrrc_rbrc.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. Send the RB Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message(tdsrrc_rbrc.status.failure_status, tdsrrc_rbrc.status.prot_err_cause);

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    tdsrrcrbrc_clear_procedure();
    return;
  }

  if(TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,cn_InformationInfo))
  {
    cn_info_ptr = &(rbrc_ptr->cn_InformationInfo);

    /* CN info presents. */
    if (cn_info_ptr != NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: Saving CN info for NAS");
      tdsrrc_rbrc.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(cn_info_ptr);
      if (tdsrrc_rbrc.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbrc.cn_info_saved = TRUE;
      }
    }
  }

  if((TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,
    ueMobilityStateIndicator)) && (rbrc_ptr->ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: High Mobility Ind Set to TRUE in RBRC.");
    tdsrrc_rbrc.high_mobility_ind = TRUE;
  }


  /* Process madatory RB information to reconfigure */
  if ((rbrc_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent) &&
      rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList.n != 0 )
  {
    /*Processing of TN-ReconfigList will not fail because we have already validated it.*/
    tdsrrcrbrc_process_rb_reconfig_list_r8(&rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList);
  }

  /* Continue further if there are no invalid Rbids in recnfig list */
  /*Store the rb ids tobe affected */
  if (rbrc_ptr->specificationMode.u.complete.m.rb_InformationAffectedListPresent)
  {
    tdsrrc_rbrc.rbs_tobe_affected.is_valid = TRUE;

    rb_info_affected_list_r8_ptr = &rbrc_ptr->specificationMode.u.complete.rb_InformationAffectedList;
    localrb_count = 0;
    while ( rb_info_affected_list_r8_ptr->n > localrb_count )
    {
      /*first check whether this rb-id exists in established_rab variable or not */
      if (! tdsrrcrbrc_validate_rb_id (rb_info_affected_list_r8_ptr->elem[localrb_count].rb_Identity))
      {
        /*rab-id does not exist in established_rab*/
        tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        is_valid_rbid = FALSE;
        invalid_rbid = (uint16)(rb_info_affected_list_r8_ptr->elem[localrb_count].rb_Identity);
        break;
      }
      tdsrrc_rbrc.rbs_tobe_affected.rb_id[localrb_count] = rb_info_affected_list_r8_ptr->elem[localrb_count].rb_Identity;
      localrb_count++;
    }
    tdsrrc_rbrc.rbs_tobe_affected.num_rbs= localrb_count;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Num of RBs to be affected:%d",localrb_count);
  }

  if (!is_valid_rbid)
  {
    /*one of the rab-id does not exist in established_rab*/
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8: RB-id %d is not in EST_RABs",invalid_rbid);

    /* Send the RB Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status, tdsrrc_rbrc.status.prot_err_cause);

    /* Make sure we're back in the initial sub-state */
    tdsrrcrbrc_clear_procedure();
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    return;
  }

  /*Update the OS with the OTA Content.*/
  tdsrrcrbrc_update_ordered_config_r8 (dcch_msg_ptr, msg_version);
  return;
} /* tdsrrcrbrc_process_rbrc_message_r8 */


#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
/*===========================================================================
FUNCTION   tdsrrcrbrc_process_after_oc_config_set_r9

DESCRIPTION
  This function processes the R8 RBRC OTA after OC is set by this OTA. So if we call this function, it means
  that TDSORDERED_CONFIG_SET is returned while setting OC by RBRC OTA.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbrc_process_after_oc_config_set_r9
(
  tdsrrc_DL_DCCH_Message * dcch_msg_ptr, 
  tdsrrc_RadioBearerReconfiguration_r9_IEs  *rbrc_ptr
)
{
  tdsrrc_state_e_type                         current_state;       /* current RRC State  */
  tdsrrcrb_directed_cell_info_type        directed_cell_info; /* directed cell info struct */
  tdsrrc_RadioBearerReconfiguration   *msg_ptr; /* Pointer to the RB Reconfig message        */
  tdsrrc_int_c_rnti_type                      c_rnti;
  tdsrrc_int_u_rnti_type                      u_rnti;
  tdsrrcrb_rnti_update_info_type         rnti_update_info;   /* to hold rnti update info */

  /* Get the pointer to the actual RB Reconfig message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerReconfiguration);

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
  if(tdsrrcsib_is_current_event_sib_read_in_dch())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Force Init GPS ");
    tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
  }
#endif /*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/

  current_state = tdsrrc_get_state(); 

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  directed_cell_info.freq_info_present = FALSE;
  directed_cell_info.cpid_present = FALSE;  

  /* Populate the local variables to indicate if Re-direction info is present */
  if((current_state==TDSRRC_STATE_CELL_FACH) && 
    ((tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
    ||(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
    ||(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
  {
    if((TDSRRCRB_R9_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r9_IEs,frequencyInfo)) &&
      (rbrc_ptr->frequencyInfo.modeSpecificInfo.t == T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd))
    {
      directed_cell_info.freq_info_present = TRUE;
      directed_cell_info.uarfcn_dl  = rbrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
    }
  
    if(tdsrrcrbrc_is_pccpch_info_present(msg_ptr, TDSMSG_REL9))
    {
      directed_cell_info.cpid_present = TRUE;
      directed_cell_info.cell_param_id = rbrc_ptr->dl_InformationPerRL_List.elem[0].
                  modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
    }
    /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
          FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
    if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
      (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
      if(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
      {
        tdsrrcrbrc_send_initiate_cu_flag = TRUE;
        tdsrrcrbrc_delete_crnti          = TRUE;
      }
      else
      {
        tdsrrc_rbrc.directed_cell_info = TRUE;
      }
    }  
  }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

  /*DCH->DCH, FACH->FACH, FACH->DCH*/
  if ((current_state==tdsrrc_rbrc.state_indicated_by_msg) ||((current_state==TDSRRC_STATE_CELL_FACH) && 
    (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH)))
  {
    /*check whether any frequency info is present in the message and transition is 
    CELL_FACH->CELL_FACH. In this case initiate a cell update after channel config is 
    successfully completed */
    if ((current_state==TDSRRC_STATE_CELL_FACH)&& (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
    {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU

      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if((tdsrrcrbrc_send_initiate_cu_flag == FALSE) && 
        (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
      {
        if(TDSRRCRB_R9_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r9_IEs,new_C_RNTI))
        {
          tdsrrc_rbrc.new_crnti_valid = TRUE;
          tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
          tdsrrc_rbrc.new_crnti = (uint16)c_rnti;
        }
        else
        {
          tdsrrc_rbrc.new_crnti_valid = FALSE;
        }
        /* Now copy OC to TOC */
        tdsrrcllcoc_update_toc_with_oc_from_fach();
        /*Initiate the cell selection*/
        tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
                    TDSRRC_TRANSITION_FROM_FACH,tdsrrc_rbrc.state_indicated_by_msg);

        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

        /*We can return from here. Because FACH->FACH is done.*/
        return;
      }

#else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

      if ((TDSRRCRB_R9_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r9_IEs,
        frequencyInfo)) && (rbrc_ptr->frequencyInfo.modeSpecificInfo.t == 
         T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd))
      {
        directed_cell_info.freq_info_present = TRUE;
        directed_cell_info.uarfcn_dl         = 
        rbrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd.uarfcn_Nt;
      }
      else
      {
        directed_cell_info.freq_info_present = FALSE;
      }
      if (tdsrrcrbrc_is_pccpch_info_present(msg_ptr, TDSMSG_REL9))
      {
        directed_cell_info.cpid_present = TRUE;
        directed_cell_info.cell_param_id = rbrc_ptr->dl_InformationPerRL_List.elem[0].
                                    modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
      }
      else
      {
        directed_cell_info.cpid_present = FALSE;
      }

      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
      {
        /*directed cell info matched. Cell update is not required */
        tdsrrcrbrc_send_initiate_cu_flag = FALSE;
      }
      else
      {
        /*directed cell info do not match. Cell update is required */
        tdsrrcrbrc_send_initiate_cu_flag = TRUE;
        tdsrrcrbrc_delete_crnti          = TRUE;
      }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
    } /* if CELL_FACH->CELL_FACH reconfiguration */

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");

    /* Call the function that creates and sends a channel config request to LLC. */
    tdsrrcrbrc_send_channel_config_req();
    
    /* Change the substate since we're waiting for the confirmation from LLC for the 
    channel configs */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
  }

  /*DCH -> FACH*/
  else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
    (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
  {
    if(TDSRRCRB_R9_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r9_IEs,new_C_RNTI))
    {
      tdsrrc_rbrc.new_crnti_valid = TRUE;
      tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
      tdsrrc_rbrc.new_crnti = (uint16)c_rnti;
    }
    else
    {
      tdsrrc_rbrc.new_crnti_valid = FALSE;
    }

    /*Initiate the cell selection*/
    tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
              TDSRRC_TRANSITION_FROM_DCH, TDSRRC_STATE_CELL_FACH);

    tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
  
    /* Start T_305 [periodic cell update] timer */
    tdsrrccu_start_t_305_timer();
  }

  /*DCH -> Cell_PCH*/
  else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
    (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
  {
    tdsrrc_rbrc.directed_cell_info = tdsrrcrbrc_is_pccpch_info_present(msg_ptr, TDSMSG_REL9);
    /*Send the RRCRBRC complete msg on old configuration */
    tdsrrcrbrc_send_rbrc_complete_message(TRUE);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
    /*wait for L2ack */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    tdsrrcllc_print_trans_chnl_rate();
  }

  /*DCH -> URA_PCH*/
  else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
    (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
  {
    /* no need to check directed cell info in this case */
    /*Send the RRCRBRC complete msg on old configuration */
    tdsrrcrbrc_send_rbrc_complete_message(TRUE);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
    /*wait for L2ack */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    tdsrrcllc_print_trans_chnl_rate();
  }

  /*FACH -> PCH*/
  else if ((current_state==TDSRRC_STATE_CELL_FACH) && 
    ((tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
    (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
  {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /*call the function that evaluates the directed cell info and set the flags accordingly */
    if((tdsrrc_rbrc.directed_cell_info == FALSE) && 
      (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
    {
      if(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
      {
        tdsrrc_rbrc.directed_cell_info = tdsrrcrbrc_is_pccpch_info_present(msg_ptr, TDSMSG_REL9);
      }
      tdsrrc_rbrc.initiate_cell_selection_trans_from_fach = TRUE;
    }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
  
    /*Send the RRCRBRC complete msg on old configuration */
    /* check whether new C-RNTI was present in the message. In that case we have to configure 
    MAC with new C-RNTI before sending the response message */
    if(TDSRRCRB_R9_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r9_IEs,new_C_RNTI))
    {
      tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
      rnti_update_info.crnti_action = TDSRRC_RNTI_UPDATE;
      rnti_update_info.crnti        = (uint16)c_rnti;

      /* check whether URNTI is also present in the message */
      if(TDSRRCRB_R9_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r9_IEs,new_U_RNTI))
      {
        tdsrrc_translate_urnti(&rbrc_ptr->new_U_RNTI, &u_rnti);
        rnti_update_info.urnti_action = TDSRRC_RNTI_UPDATE; 
        rnti_update_info.urnti        = u_rnti; 
      }
      else
      {
        rnti_update_info.urnti_action = TDSRRC_RNTI_NOCHANGE; 
      }

      rnti_update_info.procedure                = TDSRRC_PROCEDURE_RBRC;
      rnti_update_info.rlc_re_establish_srb  = FALSE;
      rnti_update_info.rlc_re_establish_rab  = FALSE;
      rnti_update_info.cnf_reqd                  = TRUE;

      /* Send a RNTI Update Request */
      tdsrrcrb_send_rnti_update_req(rnti_update_info);

      /* wait for the RNTI update confirm before sending the response message */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
    }
    else
    {
      tdsrrcrbrc_send_rbrc_complete_message(TRUE);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
      /*wait for L2ack */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
      tdsrrcllc_print_trans_chnl_rate();
    }
  }
  return;
}

/*===========================================================================
FUNCTION   RRCRBRC_UPDATE_ORDERED_CONFIG_R9

DESCRIPTION
  This function processes the received Channel Config Confirmation
  from LLC. If the required Radio-bearers have been reconfig succesfully,
  and if any of the radio-bearers map to radio-access-bearers, this
  information needs to be given to the NAS entity for that RAB.
  
  Currently Circuit-Switched voice and Circuit-Switched data RABs are 
  supported. If a voice RAB has been assigned by the UTRAN, this function
  gets the RLC ids and the AMR mode information from LLC and sends this
  information to voice services. If a CS Data RAB has been assigned, the
  RLC ids for those RBs are sent to the data services entity. Then this
  function sends the RRC_SYNC_IND primitive to MM.
          
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbrc_update_ordered_config_r9
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr,      /* Pointer to the RB Reconfig message */
  tdsrrc_msg_ver_enum_type                   msg_version
)
{
  tdsrrcllc_oc_status_e_type                oc_status;/* To hold tdsordered_config status  */
  /* Used to indicate if an error occured while setting tdsordered_config. */
  boolean                                      ordered_config_err_occured = FALSE; 

  /* We have to reconfigure RBs. First set TDSORDERED_CONFIG so that LLC can process the 
  necessary parameters. */
  oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBRC,
                     tdsrrc_rbrc.state_indicated_by_msg, tdsrrc_DL_DCCH_Message_PDU,
                     (void *)dcch_msg_ptr);
  
  switch (oc_status)
  {
    case TDSORDERED_CONFIG_SET:
      tdsrrcrbrc_process_after_oc_config_set_r9(dcch_msg_ptr,
          &dcch_msg_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.
          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.
          radioBearerReconfiguration_r9);
      break;

    case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
      /* Send the RB Release Failure Message */
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause );

      /* Go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
      tdsrrc_rbrc.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      /* Send the RB Release Failure Message */
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause );

      /* Go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;
      
      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

       TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
       tdsrrc_rbrc.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
       /* Send the RB Release Failure Message */
       tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause
                               );
   
       /* Go back to initial substate */
       tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
   
       /* Indicate that an error occured */
       ordered_config_err_occured = TRUE;
       break;
        
    case TDSORDERED_CONFIG_NOT_SET_OTHER:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
      /* When LLC sends this, there is a serious problem
      and we'll go back to disconnected state. No need
      to send the failure message here, we can just
      go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    default:

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
      /* Unknown error, just go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;
  }/* end switch */

  if (ordered_config_err_occured == TRUE)
  {
    /* No need to continue. Clear the procedure variables and return. */
    tdsrrcrbrc_clear_procedure();
    return;
  }
  else
  {
    if (tdsrrc_rbrc.rbrc_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }
  }
}

/*===========================================================================
FUNCTION   VALIDATE_RBRC_MESSAGE_R9

DESCRIPTION
  This function validates the received RB Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Reconfig Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbrc_validate_rbrc_message_r9
(
  tdsrrc_RadioBearerReconfiguration_r9_IEs *rbrc_ptr /* Pointer to the R8 RB RC message */
)
{
  /*For CS RABs (if present) the codec type will be updated in tdsrrc_ota_codec_mode_status
  even if RAB-InfoReconfig List isn't present*/
  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();

  /* Assume the message is valid for now */
  tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_VALID;

  /* Check the RRC State indicatior given by the message and 
     store it in the tdsrrc_rbrc internal memory */
  switch (rbrc_ptr->rrc_StateIndicator)
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;

    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;

    default:
      /* No other states are supported right now. */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported state %d in RBRC R9 Msg",rbrc_ptr->rrc_StateIndicator);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;
  }

  if (tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG)
  {
    return;
  }
  /* Validate DL HSPDSCH */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r9_IEs,dl_HSPDSCH_Information))
  {
    /* Check if network does not try to initiate HSDPA with invalid state */
    if (tdsrrc_hsdpa_initial_validation(rbrc_ptr->rrc_StateIndicator, 
                                    TDSRRC_CHECK_COMMON_MSG_TYPE(rbrc_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
                                     rrc_DL_HSPDSCH_Information_r9_modeSpecificInfo_fdd)) == FAILURE)
    {
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  /*RRC_GKG: Find out whether any Reconfig OTA support Pre-Reconfig*/
  /* Not support to the pre-config option. */
  if (rbrc_ptr->specificationMode.t != 
      T_tdsrrc_RadioBearerReconfiguration_r9_IEs_specificationMode_complete)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: Non-support preconfig t in RBRC");
    tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    return;
  }

  /* Obtain U_RNTI if present for SRNS relocation.  */
  if(TDSRRCRB_R9_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r9_IEs,new_U_RNTI))
  {
    /* check whether the SRNS relocation is within Cell_DCH or not.
       SRNS relocation information can come in reconfiguration messages
       only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"REL9: SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbrc.state_indicated_by_msg);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* message with valid SNRS relocation info */
    else
    {
      /* Set the flag that indicates that this procedure involves SRNS relocation */
      tdsrrc_rbrc.rbrc_for_srns_relocation = TRUE;

      /*store the new U-RNTI */
      tdsrrc_translate_urnti(&(rbrc_ptr->new_U_RNTI), &tdsrrc_rbrc.new_urnti);
    }
  }

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch */
  tdsrrc_rbrc.trans_to_dch_ciph_config_needed = FALSE;
  /*RRC_GKG: Even though, any inconsistency exists in Security parameters, we don't fail the OTA.
  Check whether it's as per Spec.*/
  if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    /* check to see if dl-dpch-common info is present while going to dch */
    if ((TDSRRCRB_R9_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r9_IEs,dl_CommonInformation)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rbrc_ptr->dl_CommonInformation,
         tdsrrc_DL_CommonInformation_r8,dl_dpchInfoCommon)))
    {
      if (rbrc_ptr->dl_CommonInformation.dl_dpchInfoCommon.t == 
          T_tdsrrc_DL_CommonInformation_r8_dl_dpchInfoCommon_dl_DPCH_InfoCommon)
      {
        /* check if ciphering is active in mac for cs domain, if it is then,
          start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
        */
        if (tdsrrcsmc_tm_ciph_active() == TRUE)
        {
          uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
          uecomdef_status_e_type loc_ret_val;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: To-DCH-Step-1 config activated");

          loc_ret_val = tdsrrcrb_get_mac_d_hfn((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon,
		  	   tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
               &rbrc_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
               &mac_d_hfn_l);

          if(loc_ret_val == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
          }
          if (tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing Ciphering Config for HHO failed");
          }
          tdsrrc_rbrc.trans_to_dch_ciph_config_needed = TRUE;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: To-DCH-Step-1 not needed");
        }
      }
    }
  }

  /*Check the RABs in the List.
  1. Make sure that the given RABs are in EST_RABs.
  2. Update the NAS-Sync Ind for CS RABs, if present.*/
  if (rbrc_ptr->specificationMode.u.complete.m.rab_InformationReconfigListPresent)
  {
    tdsrrc_rbrc.status.failure_status = tdsrrc_update_rab_info_reconfig_param_r8(
      &rbrc_ptr->specificationMode.u.complete.rab_InformationReconfigList, &tdsrrc_rbrc.rabs, &nas_sync_ind);
   if( tdsrrc_rbrc.status.failure_status != TDSRRCRB_MSG_VALID) 
   {
      return;
    }
  }

  tdsrrc_set_ordered_codec_mode(nas_sync_ind);

  /*Do the validations for RB-Reconfig List
  1. The RB-Id should be present in EST_RABs
  2. If RB is being STOP/CONTINUE, then it can't be SRB1/2 and TM URBs
  3. Check for PDCP Info SRNS relocation etc.*/
  if((T_tdsrrc_RadioBearerReconfiguration_r9_IEs_specificationMode_complete ==
     (rbrc_ptr->specificationMode.t) &&
      rbrc_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent))
  {
    /* Save the linked list head pointer */
    if (FAILURE == tdsrrcllc_validate_rb_info_reconfig_list_r8(
        &rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList))
    {
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  /*Cell_PCH/URA_PCH specific validations - 
     1. UTRAN DRX Cycle Length Coeff has to be present.
     2.  If state is URA_PCH, and URA-ID is present then check for it's length
  */
  if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
    /* If UTRAN DRX coefficient is not present then it is invalid configuration */
    if (!(TDSRRCRB_R9_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r9_IEs,
      utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL9: UTRAN DRX Coef not preset,State:%d",tdsrrc_rbrc.state_indicated_by_msg);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    if (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      /*If UE state is Cell_FACH then set the flag tdsrrccu_ura_update_required if more than one URA-IDs are
      present in SIB2*/
      if (!(TDSRRCRB_R9_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r9_IEs,ura_Identity)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: URA-ID not present for URA_PCH state.");
        tdsrrccu_clear_ura_id();
      }
      else
      {
        /*URA ID present in the message set ura_identity variable with this value */
        if (rbrc_ptr->ura_Identity.numbits == 16)
        {
          tdsrrccu_update_ura_id (&(rbrc_ptr->ura_Identity));
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL9: Invalid URA-ID length%d",rbrc_ptr->ura_Identity.numbits);
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
          tdsrrc_rbrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
          return;
        }
      }
    }
  } // if TDSRRC_STATE_CELL_PCH or TDSRRC_STATE_URA_PCH

}/* end function tdsrrcrbrc_validate_rbrc_message_r7 */

/*===========================================================================
FUNCTION   tdsrrcrbrc_process_rbrc_message_r9

DESCRIPTION
  This function processes the received RB Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Reconfig Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Reconfig Failure message
    will be sent to UTRAN.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbrc_process_rbrc_message_r9
(
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id,
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr  ,
  tdsrrc_RadioBearerReconfiguration_r9_IEs *rbrc_ptr
)
{
  tdsrrc_CN_InformationInfo_r6         *cn_info_ptr = NULL; 
  uint32                                       localrb_count;
  boolean                                     is_valid_rbid = TRUE;
  uint16                                       invalid_rbid = 0xff;
  tdsrrc_msg_ver_enum_type                   msg_version = TDSMSG_REL9;
  tdsrrc_RB_InformationAffectedList_r8  *rb_info_affected_list_r8_ptr = NULL;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9: Recvd RBRC Msg sub_st%d",tdsrrc_rbrc.curr_substate);

  if (FAILURE == tdsrrcrbrc_check_and_send_failure_for_simul_reconfig(tdstransaction_id))
  {
    return;
  }
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(dcch_msg_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
      u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9,
      radioBearerReconfiguration_r9_add_ext))
     
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: radioBearerReconfiguration_r9_add_extPresent not yet supported");
    tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    return;
  } 

  if(NULL == rbrc_ptr)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"rbrc_ptr is NULL.");
    return;
  }
    
#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rbrc_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rbrc_ptr->m.activationTimePresent,
         rbrc_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBRC SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbrc_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rbrc_ptr->m.activationTimePresent = 0;
    }
  }

#endif

  /* Validate the Message to make sure there is no invalid or incompatible configuration given by the message */
  //This code is to satify KW error only.
    tdsrrcrbrc_validate_rbrc_message_r9( rbrc_ptr );

  if ( tdsrrc_rbrc.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. Send the RB Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message(tdsrrc_rbrc.status.failure_status, tdsrrc_rbrc.status.prot_err_cause);

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    tdsrrcrbrc_clear_procedure();
    return;
  }

  if(TDSRRCRB_R9_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r9_IEs,cn_InformationInfo))
  {
    cn_info_ptr = &(rbrc_ptr->cn_InformationInfo);

    /* CN info presents. */
    if (cn_info_ptr != NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: Saving CN info for NAS");
      tdsrrc_rbrc.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(cn_info_ptr);
      if (tdsrrc_rbrc.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbrc.cn_info_saved = TRUE;
      }
    }
  }

  if((TDSRRCRB_R9_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r9_IEs,
    ueMobilityStateIndicator)) && (rbrc_ptr->ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: High Mobility Ind Set to TRUE in RBRC.");
    tdsrrc_rbrc.high_mobility_ind = TRUE;
  }


  /* Process madatory RB information to reconfigure */
  if ((rbrc_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent) &&
      rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList.n != 0 )
  {
    /*Processing of TN-ReconfigList will not fail because we have already validated it.*/
    tdsrrcrbrc_process_rb_reconfig_list_r8(&rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList);
  }

  /* Continue further if there are no invalid Rbids in recnfig list */
  /*Store the rb ids tobe affected */
  if (rbrc_ptr->specificationMode.u.complete.m.rb_InformationAffectedListPresent)
  {
    tdsrrc_rbrc.rbs_tobe_affected.is_valid = TRUE;

    rb_info_affected_list_r8_ptr = &rbrc_ptr->specificationMode.u.complete.rb_InformationAffectedList;
    localrb_count = 0;
    while ( rb_info_affected_list_r8_ptr->n > localrb_count )
    {
      /*first check whether this rb-id exists in established_rab variable or not */
      if (! tdsrrcrbrc_validate_rb_id (rb_info_affected_list_r8_ptr->elem[localrb_count].rb_Identity))
      {
        /*rab-id does not exist in established_rab*/
        tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        is_valid_rbid = FALSE;
        invalid_rbid = (uint16)(rb_info_affected_list_r8_ptr->elem[localrb_count].rb_Identity);
        break;
      }
      tdsrrc_rbrc.rbs_tobe_affected.rb_id[localrb_count] = rb_info_affected_list_r8_ptr->elem[localrb_count].rb_Identity;
      localrb_count++;
    }
    tdsrrc_rbrc.rbs_tobe_affected.num_rbs= localrb_count;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9: Num of RBs to be affected:%d",localrb_count);
  }

  if (!is_valid_rbid)
  {
    /*one of the rab-id does not exist in established_rab*/
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL9: RB-id %d is not in EST_RABs",invalid_rbid);

    /* Send the RB Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status, tdsrrc_rbrc.status.prot_err_cause);

    /* Make sure we're back in the initial sub-state */
    tdsrrcrbrc_clear_procedure();
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    return;
  }

  /*Update the OS with the OTA Content.*/
  tdsrrcrbrc_update_ordered_config_r9 (dcch_msg_ptr, msg_version);
  return;
} /* tdsrrcrbrc_process_rbrc_message_r7 */


#endif /* FEATURE_TDSCDMA_REL9 */

/*===========================================================================

FUNCTION   RRCRBRC_UPDATE_ORDERED_CONFIG_R5

DESCRIPTION

  This function processes the received Channel Config Confirmation
  from LLC. If the required Radio-bearers have been reconfig succesfully,
  and if any of the radio-bearers map to radio-access-bearers, this
  information needs to be given to the NAS entity for that RAB.
  
  Currently Circuit-Switched voice and Circuit-Switched data RABs are 
  supported. If a voice RAB has been assigned by the UTRAN, this function
  gets the RLC ids and the AMR mode information from LLC and sends this
  information to voice services. If a CS Data RAB has been assigned, the
  RLC ids for those RBs are sent to the data services entity. Then this
  function sends the RRC_SYNC_IND primitive to MM.
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_update_ordered_config_r5
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr,      /* Pointer to the RB Reconfig message */
  tdsrrc_msg_ver_enum_type                   msg_version
)
{
  tdsrrc_state_e_type                       current_state;       /* current RRC State  */
  tdsrrcllc_oc_status_e_type                oc_status;/* To hold tdsordered_config status          */
  boolean                                ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                                                       occured while setting tdsordered_config. */
  tdsrrc_int_c_rnti_type                    c_rnti;
  tdsrrc_int_u_rnti_type                    u_rnti;
  tdsrrcrb_rnti_update_info_type            rnti_update_info;   /* to hold rnti update info */
  tdsrrc_RadioBearerReconfiguration         *msg_ptr; /* Pointer to the RB Reconfig message        */
  tdsrrcrb_directed_cell_info_type          directed_cell_info; /* directed cell info struct */
  boolean                                crnti_present = FALSE;
  ASN1BitStr32                           *crnti_ptr = NULL;
  tdsrrc_RadioBearerReconfiguration_r5_IEs  *rbrc_ptr = NULL;

  /* We have to reconfigure RBs. First set TDSORDERED_CONFIG
    so that LLC can process the necessary parameters. */
  /* Get the pointer to the actual RB Reconfig message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerReconfiguration);
  oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBRC,
                                         tdsrrc_rbrc.state_indicated_by_msg,
                                         tdsrrc_DL_DCCH_Message_PDU,
                                         (void *)dcch_msg_ptr
                                       );
  
  rbrc_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
             r5.radioBearerReconfiguration_r5;
  switch (oc_status)
  {
    case TDSORDERED_CONFIG_SET:
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
      if((tdsrrcsib_is_current_event_sib_read_in_dch()) &&
         (TDSRRC_PROCEDURE_GPS == tdsrrcsib_get_proc_id_for_sib_read_in_dch()))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Force Init GPS ");
        tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
      }


#endif 

      current_state = tdsrrc_get_state(); 
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
     
      directed_cell_info.freq_info_present = FALSE;
      directed_cell_info.cpid_present = FALSE;
     
      /* Populate the local variables to indicate if Re-direction info is present */
      if((current_state==TDSRRC_STATE_CELL_FACH) && 
            ((tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
            ||(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
            ||(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)
            ))
      {
        if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r5_IEs,frequencyInfo)) &&
               (rbrc_ptr->frequencyInfo.modeSpecificInfo.t == 
                T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
              )
        {
          directed_cell_info.freq_info_present = TRUE;
          directed_cell_info.uarfcn_dl =  rbrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
        }
           
 
        if (tdsrrcrbrc_is_pccpch_info_present(msg_ptr, msg_version))
        {
          directed_cell_info.cpid_present = TRUE;
          directed_cell_info.cell_param_id = rbrc_ptr->dl_InformationPerRL_List.elem[0].
                       modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
        }
        /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
              FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
        if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
          (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
          if(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
          {
            tdsrrcrbrc_send_initiate_cu_flag = TRUE;
            tdsrrcrbrc_delete_crnti          = TRUE;
          }
          else
          {
            tdsrrc_rbrc.directed_cell_info = TRUE;
          }
        }             
      }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
      if ((current_state==tdsrrc_rbrc.state_indicated_by_msg) ||
          ((current_state==TDSRRC_STATE_CELL_FACH) && 
           (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH)))
      {
        /* CELL_DCH->CELL_DCH or CELL_FACH->CELL_FACH case. We can release the
        Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
        
        /*check whether any frequency info is present in the message
        and transition is CELL_FACH->CELL_FACH. In this case
        initiate a cell update after channel config is successfully completed */
        if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
            (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
        {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if((tdsrrcrbrc_send_initiate_cu_flag == FALSE) && 
            (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r5_IEs,new_C_RNTI))
            {
              tdsrrc_rbrc.new_crnti_valid = TRUE;
              tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
              tdsrrc_rbrc.new_crnti = (uint16)c_rnti;
            }
            else
            {
              tdsrrc_rbrc.new_crnti_valid = FALSE;
            }
    
            tdsrrcllcoc_update_toc_with_oc_from_fach();
            /*Initiate the cell selection*/
            tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
                                                       TDSRRC_TRANSITION_FROM_FACH,
                                                       tdsrrc_rbrc.state_indicated_by_msg);
    
    
                
                
            tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
    
  
            /* exit from the switch case */
            break;
          }
    
 #else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
          if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r5_IEs,frequencyInfo)) &&
              (rbrc_ptr->frequencyInfo.modeSpecificInfo.t == 
               T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
             )
          {
            directed_cell_info.freq_info_present = TRUE;
            directed_cell_info.uarfcn_dl         = 
            rbrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
          }
          else
          {
            directed_cell_info.freq_info_present = FALSE;
          }


          if (tdsrrcrbrc_is_pccpch_info_present(msg_ptr, msg_version))
          {
            directed_cell_info.cpid_present = TRUE;
            directed_cell_info.cell_param_id = rbrc_ptr->dl_InformationPerRL_List.elem[0].
                             modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          }
          else
          {
            directed_cell_info.cpid_present = FALSE;
          }

          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
          {
            /*directed cell info matched. Cell update is not required */
            tdsrrcrbrc_send_initiate_cu_flag = FALSE;
          }
          else
          {
            /*directed cell info do not match. Cell update is required */
            tdsrrcrbrc_send_initiate_cu_flag = TRUE;
            tdsrrcrbrc_delete_crnti          = TRUE;
          }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
        } /* if CELL_FACH->CELL_FACH reconfiguration */
        /* Call the function that creates and sends a channel
          config request to LLC. */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
        tdsrrcrbrc_send_channel_config_req();

        /* Change the substate since we're waiting for the
           confirmation from LLC for the channel configs */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
      }
      else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
               (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
      {

        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r5_IEs,new_C_RNTI))
        {
          tdsrrc_rbrc.new_crnti_valid = TRUE;
          tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
          tdsrrc_rbrc.new_crnti = (uint16)c_rnti;
        }
        else
        {
          tdsrrc_rbrc.new_crnti_valid = FALSE;
        }

        /*Initiate the cell selection*/
        tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
                                                TDSRRC_TRANSITION_FROM_DCH,
                                                tdsrrc_rbrc.state_indicated_by_msg);
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

        /* Start T_305 [periodic cell update] timer */
        tdsrrccu_start_t_305_timer();

      }
      else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
               (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
      {
        tdsrrc_rbrc.directed_cell_info = tdsrrcrbrc_is_pccpch_info_present(msg_ptr, msg_version);
        /*Send the RRCRBRC complete msg on old configuration */
        tdsrrcrbrc_send_rbrc_complete_message(TRUE);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
        /*wait for L2ack */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;

        tdsrrcllc_print_trans_chnl_rate();
      }
      else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
               (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
      {
        /* no need to check directed cell info in this case */
        /*Send the RRCRBRC complete msg on old configuration */
        tdsrrcrbrc_send_rbrc_complete_message(TRUE);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
        /*wait for L2ack */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;

        tdsrrcllc_print_trans_chnl_rate();
      }
      else if ((current_state==TDSRRC_STATE_CELL_FACH) && 
               ((tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
                (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
      {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
        /*call the function that evaluates the directed cell info and set the flags accordingly */
        if((tdsrrc_rbrc.directed_cell_info == FALSE) && 
          (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
        {
            
          if(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
          {
            tdsrrc_rbrc.directed_cell_info = tdsrrcrbrc_is_pccpch_info_present(msg_ptr, msg_version);
          }


          tdsrrc_rbrc.initiate_cell_selection_trans_from_fach = TRUE;
        }
    
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

        /*Send the RRCRBRC complete msg on old configuration */
        /* check whether new C-RNTI was present in the message. In that case
        we have to configure MAC with new C-RNTI before sending the response 
        message */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r5_IEs,new_C_RNTI))
        {
          crnti_present = TRUE;
          crnti_ptr = &rbrc_ptr->new_C_RNTI;
        }

        if (crnti_present)
        {
          tdsrrc_translate_crnti(crnti_ptr, &c_rnti);
          /* Send a RNTI Update Request */
          rnti_update_info.crnti_action = TDSRRC_RNTI_UPDATE;
          rnti_update_info.crnti        = (uint16)c_rnti;
          /* check whether URNTI is also present in the message */
          if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r5_IEs,new_U_RNTI))
          {
            tdsrrc_translate_urnti(&rbrc_ptr->new_U_RNTI, &u_rnti);
            rnti_update_info.urnti_action = TDSRRC_RNTI_UPDATE; 
            rnti_update_info.urnti        = u_rnti; 
          }
          else
          {
            rnti_update_info.urnti_action = TDSRRC_RNTI_NOCHANGE; 
          }
          rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBRC;
          rnti_update_info.rlc_re_establish_srb = FALSE;
          rnti_update_info.rlc_re_establish_rab = FALSE;
          rnti_update_info.cnf_reqd             = TRUE;
          tdsrrcrb_send_rnti_update_req(rnti_update_info);
          /* wait for the RNTI update confirm before sending the response
          message */
          tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
        }
        else
        {
          tdsrrcrbrc_send_rbrc_complete_message(TRUE);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
          /*wait for L2ack */
          tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
          tdsrrcllc_print_trans_chnl_rate();
        }
      }
      break;

    case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
      /* Send the RB Release Failure Message */
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause );

      /* Go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
      tdsrrc_rbrc.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      /* Send the RB Release Failure Message */
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause );

      /* Go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_rbrc.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                  tdsrrc_rbrc.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

    case TDSORDERED_CONFIG_NOT_SET_OTHER:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
      /* When LLC sends this, there is a serious problem
      and we'll go back to disconnected state. No need
      to send the failure message here, we can just
      go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    default:

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
      /* Unknown error, just go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;
  }/* end switch */

  if ( ordered_config_err_occured == TRUE )
  {
    /* No need to continue. Clear the procedure
    variables and return. */
    tdsrrcrbrc_clear_procedure();
    return;
  }
  else
  {
    if (tdsrrc_rbrc.rbrc_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }
  }
}
/*===========================================================================

FUNCTION   tdsrrcrbrc_check_and_get_primary_plmn_id

DESCRIPTION

  This function will check whether primary plmn id is present 
  in r3,r5 path of RBRC message. If present then it will return 
  primary plmn id.
        
DEPENDENCIES
  None.

RETURN VALUE
TRUE : IF Primary PLMN I.D is present in RBRC message in r3,r5 path
FFALSE: Otherwise

SIDE EFFECTS

  None.

===========================================================================*/
static boolean tdsrrcrbrc_check_and_get_primary_plmn_id
(
  /* Pointer to the RBRC message */
  tdsrrc_RadioBearerReconfiguration *msg_ptr,
  /*This will copy primary plmn i.d if present in OTA msg*/
  tdsrrc_PLMN_Identity *primary_plmn_Identity,
  /*This will indicate OTA msg version*/
  tdsrrc_msg_ver_enum_type msg_version
)
{
  boolean primary_plmn_Id_present = FALSE;

  if(msg_ptr == NULL)
  {
    /*msg ptr is NULL so return from here*/
    return FALSE;
  }
  switch(msg_version)
  {
    case TDSMSG_REL4:
      /*Check to see if primary PLMN I.D is present in RBRC message ext
       * tdsrrc_RadioBearerReconfiguration-v690ext which is present in 
       * v690NonCriticalExtensions in r4 path
       */
      if((msg_ptr->u.later_than_r3.criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.m.
              v5d0NonCriticalExtenstionsPresent)&&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v5d0NonCriticalExtenstions.m.v690NonCriticalExtensionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
             v5d0NonCriticalExtenstions.v690NonCriticalExtensions.radioBearerReconfiguration_v690ext.m.primary_plmn_IdentityPresent)
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:RBRC msg contains primary_plmn_Id in r99 path");
        primary_plmn_Id_present = TRUE;
        *primary_plmn_Identity = msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
		           v5d0NonCriticalExtenstions.v690NonCriticalExtensions.radioBearerReconfiguration_v690ext.primary_plmn_Identity;
      }
      break;
    
    case TDSMSG_REL5:
      /*Check to see if primary PLMN I.D is present in RBRC message ext
       * tdsrrc_RadioBearerReconfiguration-v690ext which is present in 
       * v690NonCriticalExtensions in r5 path
       */
      if((msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.m.v5d0NonCriticalExtenstionsPresent)&&
         (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.
           v5d0NonCriticalExtenstions.m.v690NonCriticalExtensionsPresent)&&
         (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.
           v5d0NonCriticalExtenstions.v690NonCriticalExtensions.
           radioBearerReconfiguration_v690ext.m.primary_plmn_IdentityPresent)
       )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:RBRC msg contains primary_plmn_Id in r5 path");
        primary_plmn_Id_present = TRUE;
        *primary_plmn_Identity =msg_ptr->u.later_than_r3.criticalExtensions.u.
            criticalExtensions.u.r5.v5d0NonCriticalExtenstions.v690NonCriticalExtensions.
           radioBearerReconfiguration_v690ext.primary_plmn_Identity;
      }
      break;
   
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"NWS: Msg version = %d,Not supported",msg_version);
      break;
  }
  
  return primary_plmn_Id_present;
}
/*===========================================================================

FUNCTION   tdsrrcrbrc_process_high_mobility_ind_in_non_critical_ext

DESCRIPTION

  This function will process High Mobility Ind received in Non-critical ext
        
DEPENDENCIES
  None.

RETURN VALUE
None

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_process_high_mobility_ind_in_non_critical_ext
(
  tdsrrc_RadioBearerReconfiguration *msg_ptr,
  tdsrrc_msg_ver_enum_type msg_version
)
{
  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
    if(msg_version == TDSMSG_REL5)
    {
      if((msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.m.v5d0NonCriticalExtenstionsPresent) &&
      (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v5d0NonCriticalExtenstions.m.v690NonCriticalExtensionsPresent) &&
      (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v5d0NonCriticalExtenstions.v690NonCriticalExtensions.m.
      v770NonCriticalExtensionsPresent) &&
      (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v5d0NonCriticalExtenstions.v690NonCriticalExtensions.
      v770NonCriticalExtensions.radioBearerReconfiguration_v770ext.m.ueMobilityStateIndicatorPresent))
      {
        if(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v5d0NonCriticalExtenstions.v690NonCriticalExtensions.
            v770NonCriticalExtensions.radioBearerReconfiguration_v770ext.ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in RBRC.");
          tdsrrc_rbrc.high_mobility_ind = TRUE;
        }
      }  
    }
    else if(msg_version == TDSMSG_REL6)
    {
      if((msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.v6b0NonCriticalExtensions.m.
          v6f0NonCriticalExtensionsPresent) &&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.v6b0NonCriticalExtensions.
          v6f0NonCriticalExtensions.m.v770NonCriticalExtensionsPresent) &&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.v6b0NonCriticalExtensions.
          v6f0NonCriticalExtensions.v770NonCriticalExtensions.radioBearerReconfiguration_v770ext.m.
           ueMobilityStateIndicatorPresent))
      {
        if(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.v6b0NonCriticalExtensions.
            v6f0NonCriticalExtensions.v770NonCriticalExtensions.radioBearerReconfiguration_v770ext.ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in RBRC.");
          tdsrrc_rbrc.high_mobility_ind = TRUE;
        }
      }
    }
    else if(msg_version == TDSMSG_REL4)
    {

      if((msg_ptr->u.later_than_r3.criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent) &&
           (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.m.
                 v590NonCriticalExtensionsPresent) &&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
                       m.v5d0NonCriticalExtenstionsPresent) &&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
                v5d0NonCriticalExtenstions.m.v690NonCriticalExtensionsPresent) &&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
              v5d0NonCriticalExtenstions.v690NonCriticalExtensions.m.v770NonCriticalExtensionsPresent) &&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
                 v590NonCriticalExtensions.v5d0NonCriticalExtenstions.v690NonCriticalExtensions.
                   v770NonCriticalExtensions.radioBearerReconfiguration_v770ext.m.ueMobilityStateIndicatorPresent))
      {
        if(msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
                 v590NonCriticalExtensions.v5d0NonCriticalExtenstions.v690NonCriticalExtensions.
                  v770NonCriticalExtensions.radioBearerReconfiguration_v770ext.ueMobilityStateIndicator == 
                   tdsrrc_High_MobilityDetected_high_MobilityDetected)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in RBRC.");
          tdsrrc_rbrc.high_mobility_ind = TRUE;
        }
      }
    }  
  }
}

/*===========================================================================

FUNCTION   PROCESS_RBRC_MESSAGE_R5

DESCRIPTION

  This function processes the received RB Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Reconfig Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Reconfig Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_process_rbrc_message_r5
(
  tdsrrc_cmd_type *cmd_ptr
)
{

  tdsrrc_DL_DCCH_Message                    *dcch_msg_ptr;      /* Pointer to the RB Reconfig message */
  tdsrrc_RadioBearerReconfiguration         *msg_ptr; /* Pointer to the RB Reconfig message        */
  tdsrrc_CN_InformationInfo                 *cn_info_ptr = NULL; 
  uint32                                  localrb_count;
  tdsrrcrbrc_rlc_stop_cont_e_type            rlc_stop_cont_e_value = TDSRB_STOP_CONTINUE_ABSENT;
  boolean                                 is_valid_rbid = TRUE;
  tdsrrc_RB_Identity invalid_rbid = 0xff;
  rlc_lc_id_type                          lc_id;
  uecomdef_logch_mode_e_type              lc_mode;

  tdsrrc_msg_ver_enum_type                   msg_version = TDSMSG_REL5;
  tdsrrc_RadioBearerReconfiguration_r5_IEs  *rbrc_ptr = NULL;
  tdsrrc_RB_InformationReconfig_r5  *rb_info_reconfig_ptr = NULL;
  tdsrrc_RB_InformationAffectedList_r5  *rb_info_affected_list_r5_ptr = NULL;

   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  boolean primary_plmn_Id_present = FALSE;
   /*Variable to hold primary plmn i.d if present in OTA msg*/
  tdsrrc_PLMN_Identity primary_plmn_Identity;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Recvd RBRC R5 Msg sub_st%d",tdsrrc_rbrc.curr_substate);
  
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Reconfig message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerReconfiguration);

  if (FAILURE == 
      tdsrrcrbrc_check_and_send_failure_for_simul_reconfig(
      msg_ptr->u.later_than_r3.rrc_TransactionIdentifier))
  {
    return;
  }
  /* Store Msg Ptr in a local variable */
  rbrc_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                        r5.radioBearerReconfiguration_r5;

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rbrc_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rbrc_ptr->m.activationTimePresent,
         rbrc_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBRC SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbrc_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rbrc_ptr->m.activationTimePresent = 0;
    }
  }

#endif

  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  tdsrrcrbrc_validate_rbrc_message_r5( msg_ptr );

  if ( tdsrrc_rbrc.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    /* Send the RB Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                               tdsrrc_rbrc.status.prot_err_cause
                             );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    tdsrrcrbrc_clear_procedure();
    return;
  }

  /* If the message is valid, proceed further */
  /* Check if there is CN information present. If so, we need
  to send the information to the Cell Change manager */
  
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,
  	tdsrrc_RadioBearerReconfiguration_r5_IEs,cn_InformationInfo))
  {
    cn_info_ptr = &(rbrc_ptr->cn_InformationInfo);
  }

  /* CN info presents. */
  if (cn_info_ptr != NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
    
      /*Check to see if primary PLMN I.D is present in RBRC message ext
       * tdsrrc_RadioBearerReconfiguration-v690ext which is present in 
       * v690NonCriticalExtensions
       */
       primary_plmn_Id_present = tdsrrcrbrc_check_and_get_primary_plmn_id(msg_ptr,
                                                                  &primary_plmn_Identity,TDSMSG_REL5);
    
    tdsrrc_rbrc.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info(
                     cn_info_ptr
                     ,primary_plmn_Id_present
                     ,&primary_plmn_Identity
                );
    if (tdsrrc_rbrc.mm_cmd_ptr == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
    }
    else
    {
      /* set cn_info_saved flag */
      tdsrrc_rbrc.cn_info_saved = TRUE;
    }
  }

  tdsrrcrbrc_process_high_mobility_ind_in_non_critical_ext(msg_ptr,TDSMSG_REL5);
 
  /* Process madatory RB information to reconfigure */
  /* Process madatory RB information to reconfigure */
  if ((rbrc_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent) &&
      (rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList.n != 0 ))
  {
    /*store the rb ids tobe released */
    tdsrrc_rbrc.rbs_tobe_reconfigured.is_valid = TRUE;
    
    localrb_count = 0;
    while (rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList.n >  localrb_count)
    {
      rb_info_reconfig_ptr = &rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList.elem[localrb_count];
      /*First check whether this rb-id exists in established_rab variable or not */
      if (! tdsrrcrbrc_validate_rb_id((rb_info_reconfig_ptr->rb_Identity)))
      {
        /*rab-id does not exist in established_rab*/
        tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        is_valid_rbid = FALSE;
        invalid_rbid = rb_info_reconfig_ptr->rb_Identity;
        /*Quit the loop*/
        break;
      }

      // Initialize the enum to this value.
      rlc_stop_cont_e_value = TDSRB_STOP_CONTINUE_ABSENT;

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
          tdsrrc_RB_InformationReconfig_r5,rb_StopContinue))
      {
        if ((uint16)(rb_info_reconfig_ptr->rb_Identity) <= 2)
        {
          /*rb-id does not exist in established_rab*/
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          is_valid_rbid = FALSE;
          invalid_rbid = rb_info_reconfig_ptr->rb_Identity;
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"RB ID %d can't be stopped/continued",invalid_rbid);
          /*Quit the loop*/
          break;
        }

        lc_id = tdsrrclcm_get_ul_rlc_lc_id((rb_info_reconfig_ptr->rb_Identity));

        if (lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
        {
          /*rb-id does not exist in established_rab*/
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          is_valid_rbid = FALSE;
          invalid_rbid = rb_info_reconfig_ptr->rb_Identity;
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"No corresponding LC id for the RB %d",invalid_rbid);
          /*Quit the loop*/
          break;
        }

        if (tdsrrclcm_get_ul_rlc_mode(lc_id, &lc_mode) == TDSRRCLCM_FAILURE)
        {
          /*rb-id does not exist in established_rab*/
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          is_valid_rbid = FALSE;
          invalid_rbid = rb_info_reconfig_ptr->rb_Identity;
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid lc_mode for RB %d",invalid_rbid);
          /*Quit the loop*/
          break;
        }

        if (rb_info_reconfig_ptr->rb_StopContinue == tdsrrc_RB_StopContinue_stopRB)
        {
          if (lc_mode == UE_MODE_TRANSPARENT)
          {
            // In case of transparent mode RB's just print an error message if stop/continue
            // is specified. Do not reject the rbreconfig message because of this.
            invalid_rbid = rb_info_reconfig_ptr->rb_Identity;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"TM RB %d can't be stopped/contd",invalid_rbid);
          }
          else
          {
            rlc_stop_cont_e_value = TDSRB_STOP;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Stop recvd for RB %d",(uint16)(rb_info_reconfig_ptr->rb_Identity));
          }

        }
        else if (rb_info_reconfig_ptr->rb_StopContinue == tdsrrc_RB_StopContinue_continueRB)
        {
          if (lc_mode == UE_MODE_TRANSPARENT)
          {
            // In case of transparent mode RB's just print an error message if stop/continue
            // is specified. Do not reject the rbreconfig message because of this.
            invalid_rbid = rb_info_reconfig_ptr->rb_Identity;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"TM RB %d can't be stopped/contd",invalid_rbid);
          }
          else
          {
            rlc_stop_cont_e_value = TDSRB_CONTINUE;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Continue recvd for RB %d",(uint16)(rb_info_reconfig_ptr->rb_Identity));
          }
        }
      }

      tdsrrc_rbrc.rbs_tobe_reconfigured.rb_id[localrb_count] = 
          rb_info_reconfig_ptr->rb_Identity;
      tdsrrc_rbrc.rbs_tobe_reconfigured.rb_stop_continue[localrb_count] = rlc_stop_cont_e_value;
      tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] = tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] = 0;
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
	  	tdsrrc_RB_InformationReconfig_r5,rlc_Info))&&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,
        tdsrrc_RLC_Info_r5,ul_RLC_Mode)))
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSMODIFY_RB_CFG;
      }
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
	  	tdsrrc_RB_InformationReconfig_r5,rlc_Info))&&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,
        tdsrrc_RLC_Info_r5,dl_RLC_Mode)))
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSMODIFY_RB_CFG;
      }
      if(rlc_stop_cont_e_value == TDSRB_STOP)
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSSTOP_RB_CFG;
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSSTOP_RB_CFG;
      }
      else if(rlc_stop_cont_e_value == TDSRB_CONTINUE)
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSCONTINUE_RB_CFG;
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSCONTINUE_RB_CFG;
      }

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"LocalRBCnt %d, rb_id %d, rlc_stop_cont %d", localrb_count,
               (uint16)(rb_info_reconfig_ptr->rb_Identity), rlc_stop_cont_e_value);
      localrb_count++;
      ;
    }
    tdsrrc_rbrc.rbs_tobe_reconfigured.num_rbs = localrb_count;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Num of RBs tobe reconfigured:%d",localrb_count);
  }

  /* Continue further if there are no invalid Rbids in recnfig list */
  if (is_valid_rbid)
  {
    /*Store the rb ids tobe affected */
    if (rbrc_ptr->specificationMode.u.complete.m.rb_InformationAffectedListPresent)
    {
      tdsrrc_rbrc.rbs_tobe_affected.is_valid = TRUE;
      rb_info_affected_list_r5_ptr = 
        &rbrc_ptr->specificationMode.u.complete.rb_InformationAffectedList;
      localrb_count = 0;
      while ( rb_info_affected_list_r5_ptr->n >localrb_count)
      {
        /*first check whether this rb-id exists in established_rab variable or not */
        if (! tdsrrcrbrc_validate_rb_id ((rb_info_affected_list_r5_ptr->elem[localrb_count].rb_Identity)))
        {
          /*rab-id does not exist in established_rab*/
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          is_valid_rbid = FALSE;
          invalid_rbid = (rb_info_affected_list_r5_ptr->elem[localrb_count].rb_Identity);
          /*one of the rab-id does not exist in established_rab*/
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"rb-id %d is not in established_rab",invalid_rbid);
          break;
        }
        tdsrrc_rbrc.rbs_tobe_affected.rb_id[localrb_count] = 
          rb_info_affected_list_r5_ptr->elem[localrb_count].rb_Identity;
        localrb_count++;
      }
      tdsrrc_rbrc.rbs_tobe_affected.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Num of RBs tobe affected:%d",localrb_count);
    }
  }

  if (!is_valid_rbid)
  {
    /*one of the rab-id does not exist in established_rab*/
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"rb-id %d is not in established_rab",invalid_rbid);
    /* Send the RB Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                               tdsrrc_rbrc.status.prot_err_cause
                             );
    /* Make sure we're back in the initial sub-state */
    tdsrrcrbrc_clear_procedure();
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    return;
  }
  /* For Cell_FACH or Cell_DCH continue with the ordered config setting */
  tdsrrcrbrc_update_ordered_config_r5 (dcch_msg_ptr, msg_version);
}/* end function tdsrrcrbrc_process_rbrc_message_r5 */



/*===========================================================================

FUNCTION   RRCRBRC_UPDATE_ORDERED_CONFIG_R6

DESCRIPTION

  This function processes the received Channel Config Confirmation
  from LLC. If the required Radio-bearers have been reconfig succesfully,
  and if any of the radio-bearers map to radio-access-bearers, this
  information needs to be given to the NAS entity for that RAB.
  
  Currently Circuit-Switched voice and Circuit-Switched data RABs are 
  supported. If a voice RAB has been assigned by the UTRAN, this function
  gets the RLC ids and the AMR mode information from LLC and sends this
  information to voice services. If a CS Data RAB has been assigned, the
  RLC ids for those RBs are sent to the data services entity. Then this
  function sends the RRC_SYNC_IND primitive to MM.
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_update_ordered_config_r6
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr,      /* Pointer to the RB Reconfig message */
  tdsrrc_msg_ver_enum_type                   msg_version
)
{
  tdsrrc_state_e_type                       current_state;       /* current RRC State  */
  tdsrrcllc_oc_status_e_type                oc_status;/* To hold tdsordered_config status          */
  boolean                                ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                                                       occured while setting tdsordered_config. */
  tdsrrc_int_c_rnti_type                    c_rnti;
  tdsrrc_int_u_rnti_type                    u_rnti;
  tdsrrcrb_rnti_update_info_type            rnti_update_info;   /* to hold rnti update info */
  tdsrrc_RadioBearerReconfiguration         *msg_ptr; /* Pointer to the RB Reconfig message        */
  tdsrrcrb_directed_cell_info_type          directed_cell_info; /* directed cell info struct */
  boolean                                crnti_present = FALSE;
  ASN1BitStr32                           *crnti_ptr = NULL;
  tdsrrc_RadioBearerReconfiguration_r6_IEs  *rbrc_ptr = NULL;

  /* We have to reconfigure RBs. First set TDSORDERED_CONFIG
    so that LLC can process the necessary parameters. */
  /* Get the pointer to the actual RB Reconfig message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerReconfiguration);
  oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBRC,
                                         tdsrrc_rbrc.state_indicated_by_msg,
                                         tdsrrc_DL_DCCH_Message_PDU,
                                         (void *)dcch_msg_ptr
                                       );
  
  rbrc_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
             criticalExtensions.u.r6.radioBearerReconfiguration_r6;
  switch (oc_status)
  {
    case TDSORDERED_CONFIG_SET:
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
      if((tdsrrcsib_is_current_event_sib_read_in_dch()) &&
         (TDSRRC_PROCEDURE_GPS == tdsrrcsib_get_proc_id_for_sib_read_in_dch()))
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Force Init GPS ");
         tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
       }


#endif /* FEATURE_TDSCDMA_GPS_CELL_ID_READING */
      current_state = tdsrrc_get_state(); 
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      directed_cell_info.freq_info_present = FALSE;
      directed_cell_info.cpid_present = FALSE;  
      /* Populate the local variables to indicate if Re-direction info is present */
      if((current_state==TDSRRC_STATE_CELL_FACH) && 
            ((tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
            ||(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
            || (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
      {
        if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r6_IEs,frequencyInfo)) &&
               (rbrc_ptr->frequencyInfo.modeSpecificInfo.t == 
                T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
              )
        {
          directed_cell_info.freq_info_present = TRUE;
          directed_cell_info.uarfcn_dl         = 
          rbrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
        }
        
 
 
        if (tdsrrcrbrc_is_pccpch_info_present(msg_ptr, msg_version))
        {
          directed_cell_info.cpid_present = TRUE;
          directed_cell_info.cell_param_id = rbrc_ptr->dl_InformationPerRL_List.elem[0].
                            modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
        }
        /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
             FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
        if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
          (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
          if(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
          {
            tdsrrcrbrc_send_initiate_cu_flag = TRUE;
            tdsrrcrbrc_delete_crnti          = TRUE;
          }
          else
          {
            tdsrrc_rbrc.directed_cell_info = TRUE;
          }
        }   
      }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/  
      if ((current_state==tdsrrc_rbrc.state_indicated_by_msg) ||
          ((current_state==TDSRRC_STATE_CELL_FACH) && 
           (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH)))
      {
        /* CELL_DCH->CELL_DCH or CELL_FACH->CELL_FACH case. We can release the
        Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");

        /*check whether any frequency info is present in the message
        and transition is CELL_FACH->CELL_FACH. In this case
        initiate a cell update after channel config is successfully completed */
        if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
            (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
        {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if((tdsrrcrbrc_send_initiate_cu_flag == FALSE) && 
            (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r6_IEs,
				                         new_C_RNTI))
            {
              tdsrrc_rbrc.new_crnti_valid = TRUE;
              tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
              tdsrrc_rbrc.new_crnti = (uint16)c_rnti;
            }
            else
            {
              tdsrrc_rbrc.new_crnti_valid = FALSE;
            }
    
            /* Now copy OC to TOC */
            tdsrrcllcoc_update_toc_with_oc_from_fach();
            /*Initiate the cell selection*/
            tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
                                                       TDSRRC_TRANSITION_FROM_FACH,
                                                       tdsrrc_rbrc.state_indicated_by_msg);
    
    
                
                
            tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
         
      
            /* exit from the switch case */
            break;
          }
 #else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
          if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r6_IEs,
		  	frequencyInfo)) &&
              (rbrc_ptr->frequencyInfo.modeSpecificInfo.t == 
               T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
             )
          {
            directed_cell_info.freq_info_present = TRUE;
            directed_cell_info.uarfcn_dl         = 
            rbrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
          }
          else
          {
            directed_cell_info.freq_info_present = FALSE;
          }


          if (tdsrrcrbrc_is_pccpch_info_present(msg_ptr, msg_version))
          {
            directed_cell_info.cpid_present = TRUE;
            directed_cell_info.cell_param_id = rbrc_ptr->dl_InformationPerRL_List.elem[0].
                               modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          }
          else
          {
            directed_cell_info.cpid_present = FALSE;
          }

          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
          {
            /*directed cell info matched. Cell update is not required */
            tdsrrcrbrc_send_initiate_cu_flag = FALSE;
          }
          else
          {
            /*directed cell info do not match. Cell update is required */
            tdsrrcrbrc_send_initiate_cu_flag = TRUE;
            tdsrrcrbrc_delete_crnti          = TRUE;
          }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
        } /* if CELL_FACH->CELL_FACH reconfiguration */
        /* Call the function that creates and sends a channel
          config request to LLC. */
        tdsrrcrbrc_send_channel_config_req();

        /* Change the substate since we're waiting for the
           confirmation from LLC for the channel configs */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
      }
      else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
               (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
      {

        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,
			tdsrrc_RadioBearerReconfiguration_r6_IEs,new_C_RNTI))
        {
          tdsrrc_rbrc.new_crnti_valid = TRUE;
          tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
          tdsrrc_rbrc.new_crnti = (uint16)c_rnti;
        }
        else
        {
          tdsrrc_rbrc.new_crnti_valid = FALSE;
        }

        /*Initiate the cell selection*/
        tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
                                                TDSRRC_TRANSITION_FROM_DCH,
                                                TDSRRC_STATE_CELL_FACH);
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

        /* Start T_305 [periodic cell update] timer */
        tdsrrccu_start_t_305_timer();		
      }
      else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
               (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
      {
        tdsrrc_rbrc.directed_cell_info = tdsrrcrbrc_is_pccpch_info_present(msg_ptr, msg_version);
        /*Send the RRCRBRC complete msg on old configuration */
        tdsrrcrbrc_send_rbrc_complete_message(TRUE);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
        /*wait for L2ack */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;

        tdsrrcllc_print_trans_chnl_rate();
      }
      else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
               (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
      {
        /* no need to check directed cell info in this case */
        /*Send the RRCRBRC complete msg on old configuration */
        tdsrrcrbrc_send_rbrc_complete_message(TRUE);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
        /*wait for L2ack */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;

        tdsrrcllc_print_trans_chnl_rate();
      }
      else if ((current_state==TDSRRC_STATE_CELL_FACH) && 
               ((tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
                (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
      {

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
        /*call the function that evaluates the directed cell info and set the flags accordingly */
        if((tdsrrc_rbrc.directed_cell_info == FALSE) && 
          (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
        {
            
          if(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
          {
           tdsrrc_rbrc.directed_cell_info = tdsrrcrbrc_is_pccpch_info_present(msg_ptr, msg_version);
          }
          tdsrrc_rbrc.initiate_cell_selection_trans_from_fach = TRUE;
 
        }
    
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

        /*Send the RRCRBRC complete msg on old configuration */
        /* check whether new C-RNTI was present in the message. In that case
        we have to configure MAC with new C-RNTI before sending the response 
        message */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r6_IEs,
			new_C_RNTI))
        {
          crnti_present = TRUE;
          crnti_ptr = &rbrc_ptr->new_C_RNTI;
        }

        if (crnti_present)
        {
          tdsrrc_translate_crnti(crnti_ptr, &c_rnti);
          /* Send a RNTI Update Request */
          rnti_update_info.crnti_action = TDSRRC_RNTI_UPDATE;
          rnti_update_info.crnti        = (uint16)c_rnti;
          /* check whether URNTI is also present in the message */
          if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r6_IEs,
		  	new_U_RNTI))
          {
            tdsrrc_translate_urnti(&rbrc_ptr->new_U_RNTI, &u_rnti);
            rnti_update_info.urnti_action = TDSRRC_RNTI_UPDATE; 
            rnti_update_info.urnti        = u_rnti; 
          }
          else
          {
            rnti_update_info.urnti_action = TDSRRC_RNTI_NOCHANGE; 
          }
          rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBRC;
          rnti_update_info.rlc_re_establish_srb = FALSE;
          rnti_update_info.rlc_re_establish_rab = FALSE;
          rnti_update_info.cnf_reqd             = TRUE;
          tdsrrcrb_send_rnti_update_req(rnti_update_info);
          /* wait for the RNTI update confirm before sending the response
          message */
          tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
        }
        else
        {
          tdsrrcrbrc_send_rbrc_complete_message(TRUE);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
          /*wait for L2ack */
          tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
          tdsrrcllc_print_trans_chnl_rate();
        }
      }
      break;

    case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
      /* Send the RB Release Failure Message */
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause );

      /* Go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
      tdsrrc_rbrc.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      /* Send the RB Release Failure Message */
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause );

      /* Go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_rbrc.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                  tdsrrc_rbrc.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

    case TDSORDERED_CONFIG_NOT_SET_OTHER:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
      /* When LLC sends this, there is a serious problem
      and we'll go back to disconnected state. No need
      to send the failure message here, we can just
      go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    default:

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
      /* Unknown error, just go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;
  }/* end switch */

  if ( ordered_config_err_occured == TRUE )
  {
    /* No need to continue. Clear the procedure
    variables and return. */
    tdsrrcrbrc_clear_procedure();
    return;
  }
  else
  {
    if (tdsrrc_rbrc.rbrc_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }
  }
}

/*===========================================================================

FUNCTION   tdsrrcrbrc_process_rbrc_message_r6

DESCRIPTION

  This function processes the received RB Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Reconfig Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Reconfig Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_process_rbrc_message_r6
(
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id,

  tdsrrc_DL_DCCH_Message *dcch_msg_ptr  ,
  /* Pointer to the RB Reconfig message        */
  tdsrrc_RadioBearerReconfiguration_r6_IEs *rbrc_ptr
)
{
  tdsrrc_CN_InformationInfo_r6              *cn_info_ptr = NULL; 
  tdsrrc_RadioBearerReconfiguration         *msg_ptr; /* Pointer to the RB Reconfig message        */
  uint32                                  localrb_count;
  tdsrrcrbrc_rlc_stop_cont_e_type            rlc_stop_cont_e_value = TDSRB_STOP_CONTINUE_ABSENT;
  boolean                                 is_valid_rbid = TRUE;
  uint16                                  invalid_rbid = 0xff;
  rlc_lc_id_type                          lc_id;
  uecomdef_logch_mode_e_type              lc_mode;

  tdsrrc_msg_ver_enum_type                   msg_version = TDSMSG_REL6;
  tdsrrc_RB_InformationReconfig_r6  *rb_info_reconfig_r6_ptr = NULL;
  tdsrrc_RB_InformationAffectedList_r6  *rb_info_affected_list_r6_ptr = NULL;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCEUL: Recvd RBRC R6 Msg sub_st%d",tdsrrc_rbrc.curr_substate);

   

  /* Get the pointer to the actual RB Reconfig message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerReconfiguration);
  if (FAILURE == 
    tdsrrcrbrc_check_and_send_failure_for_simul_reconfig(tdstransaction_id))
  {
    return;
  }

  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  if(rbrc_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"rbrc_ptr is NULL.");
    return;
  }
  else
  {
#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rbrc_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rbrc_ptr->m.activationTimePresent,
         rbrc_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBRC SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbrc_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rbrc_ptr->m.activationTimePresent = 0;
    }
  }

#endif
    tdsrrcrbrc_validate_rbrc_message_r6( rbrc_ptr );
  }

  if ( tdsrrc_rbrc.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    /* Send the RB Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                               tdsrrc_rbrc.status.prot_err_cause
                             );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    tdsrrcrbrc_clear_procedure();
    return;
  }

  if (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
  	criticalExtensions.u.r6.m.radioBearerReconfiguration_r6_add_extPresent)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: radioBearerReconfiguration_r6_add_ext not yet supported");
  } 
  
  if (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.r6.m.v6b0NonCriticalExtensionsPresent)
  {
  
    tdsrrcrbrc_process_high_mobility_ind_in_non_critical_ext(msg_ptr,TDSMSG_REL6);
  
  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,
  	tdsrrc_RadioBearerReconfiguration_r6_IEs,cn_InformationInfo))
  {
    cn_info_ptr = &(rbrc_ptr->cn_InformationInfo);

    /* CN info presents. */
    if (cn_info_ptr != NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      tdsrrc_rbrc.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(cn_info_ptr);
      if (tdsrrc_rbrc.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbrc.cn_info_saved = TRUE;
      }
    }
  }


  /* Process madatory RB information to reconfigure */
  /* Process madatory RB information to reconfigure */
  if ((rbrc_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent) &&
      rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList.n != 0 )
  {
    /*store the rb ids tobe released */
    tdsrrc_rbrc.rbs_tobe_reconfigured.is_valid = TRUE;
    
    localrb_count = 0;
    while (rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList.n > localrb_count)
    {
      // Initialize the enum to this value.
      rb_info_reconfig_r6_ptr = &rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList.elem[localrb_count];
      rlc_stop_cont_e_value = TDSRB_STOP_CONTINUE_ABSENT;

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_r6_ptr,
          tdsrrc_RB_InformationReconfig_r6,rb_StopContinue))
      {
        lc_id = tdsrrclcm_get_ul_rlc_lc_id((rb_info_reconfig_r6_ptr->rb_Identity));

        (void) tdsrrclcm_get_ul_rlc_mode(lc_id, &lc_mode);

        if (rb_info_reconfig_r6_ptr->rb_StopContinue == tdsrrc_RB_StopContinue_stopRB)
        {
          rlc_stop_cont_e_value = TDSRB_STOP;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Stop recvd for RB %d",(uint16)(rb_info_reconfig_r6_ptr->rb_Identity));
        }
        else if (rb_info_reconfig_r6_ptr->rb_StopContinue == tdsrrc_RB_StopContinue_continueRB)
        {
          rlc_stop_cont_e_value = TDSRB_CONTINUE;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Continue recvd for RB %d",(uint16)(rb_info_reconfig_r6_ptr->rb_Identity));
        }
      }

      tdsrrc_rbrc.rbs_tobe_reconfigured.rb_id[localrb_count] = 
          rb_info_reconfig_r6_ptr->rb_Identity;
      tdsrrc_rbrc.rbs_tobe_reconfigured.rb_stop_continue[localrb_count] = rlc_stop_cont_e_value;

      tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] = tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] = 0;
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_r6_ptr,
	  	tdsrrc_RB_InformationReconfig_r6,rlc_Info))&&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_r6_ptr->rlc_Info,
        tdsrrc_RLC_Info_r6,ul_RLC_Mode)))
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSMODIFY_RB_CFG;
      }
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_r6_ptr,
	  	tdsrrc_RB_InformationReconfig_r6,rlc_Info))&&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_r6_ptr->rlc_Info,
        tdsrrc_RLC_Info_r6,dl_RLC_Mode)))
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSMODIFY_RB_CFG;
      }
      if(rlc_stop_cont_e_value == TDSRB_STOP)
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSSTOP_RB_CFG;
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSSTOP_RB_CFG;
      }
      else if(rlc_stop_cont_e_value == TDSRB_CONTINUE)
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSCONTINUE_RB_CFG;
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSCONTINUE_RB_CFG;
      }

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"LocalRBCnt %d, rb_id %d, rlc_stop_cont %d", localrb_count,
               (uint16)(rb_info_reconfig_r6_ptr->rb_Identity), rlc_stop_cont_e_value);
      localrb_count++;
      
    }
    tdsrrc_rbrc.rbs_tobe_reconfigured.num_rbs = localrb_count;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Num of RBs tobe reconfigured:%d",localrb_count);
  }

  /* Continue further if there are no invalid Rbids in recnfig list */
  if (is_valid_rbid)
  {
    /*Store the rb ids tobe affected */
    if (rbrc_ptr->specificationMode.u.complete.m.rb_InformationAffectedListPresent)
    {
      tdsrrc_rbrc.rbs_tobe_affected.is_valid = TRUE;
      rb_info_affected_list_r6_ptr = 
        &rbrc_ptr->specificationMode.u.complete.rb_InformationAffectedList;
      localrb_count = 0;
      while ( rb_info_affected_list_r6_ptr->n > localrb_count )
      {
        /*first check whether this rb-id exists in established_rab variable or not */
        if (! tdsrrcrbrc_validate_rb_id ((rb_info_affected_list_r6_ptr->elem[localrb_count].rb_Identity)))
        {
          /*rab-id does not exist in established_rab*/
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          is_valid_rbid = FALSE;
          invalid_rbid = (uint16)(rb_info_affected_list_r6_ptr->elem[localrb_count].rb_Identity);
          /*one of the rab-id does not exist in established_rab*/
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"rb-id %d is not in established_rab",invalid_rbid);
          break;
        }
        tdsrrc_rbrc.rbs_tobe_affected.rb_id[localrb_count] = 
          rb_info_affected_list_r6_ptr->elem[localrb_count].rb_Identity;
        localrb_count++;
      }
      tdsrrc_rbrc.rbs_tobe_affected.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Num of RBs tobe affected:%d",localrb_count);
    }
  }

  if (!is_valid_rbid)
  {
    /*one of the rab-id does not exist in established_rab*/
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"rb-id %d is not in established_rab",invalid_rbid);
    /* Send the RB Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                               tdsrrc_rbrc.status.prot_err_cause
                             );
    /* Make sure we're back in the initial sub-state */
    tdsrrcrbrc_clear_procedure();
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    return;
  }
  /* For Cell_FACH or Cell_DCH continue with the ordered config setting */
  tdsrrcrbrc_update_ordered_config_r6 (dcch_msg_ptr, msg_version);
} /* tdsrrcrbrc_process_rbrc_message_r6 */

 

/*===========================================================================

FUNCTION   RRCRBRC_UPDATE_ORDERED_CONFIG_R7

DESCRIPTION

  This function processes the received Channel Config Confirmation
  from LLC. If the required Radio-bearers have been reconfig succesfully,
  and if any of the radio-bearers map to radio-access-bearers, this
  information needs to be given to the NAS entity for that RAB.
  
  Currently Circuit-Switched voice and Circuit-Switched data RABs are 
  supported. If a voice RAB has been assigned by the UTRAN, this function
  gets the RLC ids and the AMR mode information from LLC and sends this
  information to voice services. If a CS Data RAB has been assigned, the
  RLC ids for those RBs are sent to the data services entity. Then this
  function sends the RRC_SYNC_IND primitive to MM.
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_update_ordered_config_r7
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr,      /* Pointer to the RB Reconfig message */
  tdsrrc_msg_ver_enum_type                   msg_version
)
{
  tdsrrc_state_e_type                       current_state;       /* current RRC State  */
  tdsrrcllc_oc_status_e_type                oc_status;/* To hold tdsordered_config status          */
  boolean                                ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                                                       occured while setting tdsordered_config. */
  tdsrrc_int_c_rnti_type                    c_rnti;
  tdsrrc_int_u_rnti_type                    u_rnti;
  tdsrrcrb_rnti_update_info_type            rnti_update_info;   /* to hold rnti update info */
  tdsrrc_RadioBearerReconfiguration         *msg_ptr; /* Pointer to the RB Reconfig message        */
  tdsrrcrb_directed_cell_info_type          directed_cell_info; /* directed cell info struct */
  boolean                                crnti_present = FALSE;
  ASN1BitStr32                           *crnti_ptr = NULL;
  tdsrrc_RadioBearerReconfiguration_r7_IEs  *rbrc_ptr = NULL;

  /* We have to reconfigure RBs. First set TDSORDERED_CONFIG
    so that LLC can process the necessary parameters. */
  /* Get the pointer to the actual RB Reconfig message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerReconfiguration);
  oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBRC,
                                         tdsrrc_rbrc.state_indicated_by_msg,
                                         tdsrrc_DL_DCCH_Message_PDU,
                                         (void *)dcch_msg_ptr
                                       );
  
  rbrc_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
             criticalExtensions.u.r7.radioBearerReconfiguration_r7;
  switch (oc_status)
  {
    case TDSORDERED_CONFIG_SET:
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
     if(tdsrrcsib_is_current_event_sib_read_in_dch())
     {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Force Init GPS ");
      tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
     }
#endif
    current_state = tdsrrc_get_state(); 
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU

    directed_cell_info.freq_info_present = FALSE;
    directed_cell_info.cpid_present = FALSE;  
    /* Populate the local variables to indicate if Re-direction info is present */
    if((current_state==TDSRRC_STATE_CELL_FACH) && 
       ((tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
        || (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
        || (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
    {
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r7_IEs,frequencyInfo)) &&
         (rbrc_ptr->frequencyInfo.modeSpecificInfo.t ==
             T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd))
      {
        directed_cell_info.freq_info_present = TRUE;
        directed_cell_info.uarfcn_dl  = rbrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
      }
       
      if (tdsrrcrbrc_is_pccpch_info_present(msg_ptr, TDSMSG_REL7 ))
      {
        directed_cell_info.cpid_present = TRUE;
        directed_cell_info.cell_param_id = rbrc_ptr->dl_InformationPerRL_List.elem[0].
                                  modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
      }
      /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
          FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
      if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
        (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
        if(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
        {
          tdsrrcrbrc_send_initiate_cu_flag = TRUE;
          tdsrrcrbrc_delete_crnti          = TRUE;
        }
        else
        {
          tdsrrc_rbrc.directed_cell_info = TRUE;
        }
      }      
    }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
    if ((current_state==tdsrrc_rbrc.state_indicated_by_msg) ||
        ((current_state==TDSRRC_STATE_CELL_FACH) && 
         (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH)))
    {
      /* CELL_DCH->CELL_DCH or CELL_FACH->CELL_FACH case. We can release the
      Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
  
      /*check whether any frequency info is present in the message
      and transition is CELL_FACH->CELL_FACH. In this case
      initiate a cell update after channel config is successfully completed */
      if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
          (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
      {
  #ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
        /*call the function that evaluates the directed cell info and set the flags accordingly */
        if((tdsrrcrbrc_send_initiate_cu_flag == FALSE) && 
          (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r7_IEs,new_C_RNTI))
          {
            tdsrrc_rbrc.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
            tdsrrc_rbrc.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_rbrc.new_crnti_valid = FALSE;
          }
  
          /* Now copy OC to TOC */
          tdsrrcllcoc_update_toc_with_oc_from_fach();
          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
                                                     TDSRRC_TRANSITION_FROM_FACH,
                                                     tdsrrc_rbrc.state_indicated_by_msg);
  
  
              
              
          tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
       
    
          /* exit from the switch case */
          break;
        }
  #else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
        if (((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r7_IEs,frequencyInfo))) &&
            (rbrc_ptr->frequencyInfo.modeSpecificInfo.t == 
             T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
           )
        {
          directed_cell_info.freq_info_present = TRUE;
          directed_cell_info.uarfcn_dl         = 
          rbrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
        }
        else
        {
          directed_cell_info.freq_info_present = FALSE;
        }
  
  
        if (tdsrrcrbrc_is_pccpch_info_present(msg_ptr, msg_version))
        {
          directed_cell_info.cpid_present = TRUE;
          directed_cell_info.cell_param_id = rbrc_ptr->dl_InformationPerRL_List->elem[0].
                          modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
        }
        else
        {
          directed_cell_info.cpid_present = FALSE;
        }
  
        /*call the function that evaluates the directed cell info and set the flags accordingly */
        if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
        {
          /*directed cell info matched. Cell update is not required */
          tdsrrcrbrc_send_initiate_cu_flag = FALSE;
        }
        else
        {
          /*directed cell info do not match. Cell update is required */
          tdsrrcrbrc_send_initiate_cu_flag = TRUE;
          tdsrrcrbrc_delete_crnti          = TRUE;
        }
  #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
      } /* if CELL_FACH->CELL_FACH reconfiguration */
      /* Call the function that creates and sends a channel
        config request to LLC. */
      tdsrrcrbrc_send_channel_config_req();
  
      /* Change the substate since we're waiting for the
         confirmation from LLC for the channel configs */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
    }
    else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
               (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
    {
   
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r7_IEs,new_C_RNTI))
      {
        tdsrrc_rbrc.new_crnti_valid = TRUE;
        tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
        tdsrrc_rbrc.new_crnti = (uint16)c_rnti;
      }
      else
      {
        tdsrrc_rbrc.new_crnti_valid = FALSE;
      }
   
      /*Initiate the cell selection*/
      tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
                                              TDSRRC_TRANSITION_FROM_DCH,
                                              TDSRRC_STATE_CELL_FACH);
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
   
      /* Start T_305 [periodic cell update] timer */
      tdsrrccu_start_t_305_timer();		
    }
    else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
             (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
    {
      tdsrrc_rbrc.directed_cell_info = tdsrrcrbrc_is_pccpch_info_present(msg_ptr, msg_version);
      /*Send the RRCRBRC complete msg on old configuration */
      tdsrrcrbrc_send_rbrc_complete_message(TRUE);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
      /*wait for L2ack */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
  
      tdsrrcllc_print_trans_chnl_rate();
    }
    else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
             (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
    {
      /* no need to check directed cell info in this case */
      /*Send the RRCRBRC complete msg on old configuration */
      tdsrrcrbrc_send_rbrc_complete_message(TRUE);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
      /*wait for L2ack */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
  
      tdsrrcllc_print_trans_chnl_rate();
    }
    else if ((current_state==TDSRRC_STATE_CELL_FACH) && 
             ((tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
              (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
    {
  #ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if((tdsrrc_rbrc.directed_cell_info == FALSE) && 
        (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
      {
          
        if(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
        {
         tdsrrc_rbrc.directed_cell_info = tdsrrcrbrc_is_pccpch_info_present(msg_ptr, msg_version);
        }
        tdsrrc_rbrc.initiate_cell_selection_trans_from_fach = TRUE;
  
      }
  
  #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
  
      /*Send the RRCRBRC complete msg on old configuration */
      /* check whether new C-RNTI was present in the message. In that case
      we have to configure MAC with new C-RNTI before sending the response 
      message */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r7_IEs,new_C_RNTI))
      {
        crnti_present = TRUE;
        crnti_ptr = &rbrc_ptr->new_C_RNTI;
      }
  
      if (crnti_present)
      {
        tdsrrc_translate_crnti(crnti_ptr, &c_rnti);
        /* Send a RNTI Update Request */
        rnti_update_info.crnti_action = TDSRRC_RNTI_UPDATE;
        rnti_update_info.crnti        = (uint16)c_rnti;
        /* check whether URNTI is also present in the message */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r7_IEs,new_U_RNTI))
        {
          tdsrrc_translate_urnti(&rbrc_ptr->new_U_RNTI, &u_rnti);
          rnti_update_info.urnti_action = TDSRRC_RNTI_UPDATE; 
          rnti_update_info.urnti        = u_rnti; 
        }
        else
        {
          rnti_update_info.urnti_action = TDSRRC_RNTI_NOCHANGE; 
        }
        rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBRC;
        rnti_update_info.rlc_re_establish_srb = FALSE;
        rnti_update_info.rlc_re_establish_rab = FALSE;
        rnti_update_info.cnf_reqd             = TRUE;
        tdsrrcrb_send_rnti_update_req(rnti_update_info);
        /* wait for the RNTI update confirm before sending the response
        message */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
      }
      else
      {
        tdsrrcrbrc_send_rbrc_complete_message(TRUE);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
        /*wait for L2ack */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
        tdsrrcllc_print_trans_chnl_rate();
      }
    }
    break;

    case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
      /* Send the RB Release Failure Message */
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause );

      /* Go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
      tdsrrc_rbrc.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      /* Send the RB Release Failure Message */
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause );

      /* Go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;
      
      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

       TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
       tdsrrc_rbrc.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
       /* Send the RB Release Failure Message */
       tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause
                               );
   
       /* Go back to initial substate */
       tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
   
       /* Indicate that an error occured */
       ordered_config_err_occured = TRUE;
       break;
        
    case TDSORDERED_CONFIG_NOT_SET_OTHER:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
      /* When LLC sends this, there is a serious problem
      and we'll go back to disconnected state. No need
      to send the failure message here, we can just
      go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    default:

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
      /* Unknown error, just go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;
  }/* end switch */

  if ( ordered_config_err_occured == TRUE )
  {
    /* No need to continue. Clear the procedure
    variables and return. */
    tdsrrcrbrc_clear_procedure();
    return;
  }
  else
  {
    if (tdsrrc_rbrc.rbrc_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }
  }
}

/*===========================================================================

FUNCTION   tdsrrcrbrc_process_rbrc_message_r7

DESCRIPTION

  This function processes the received RB Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Reconfig Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Reconfig Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_process_rbrc_message_r7
(
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id,

  tdsrrc_DL_DCCH_Message *dcch_msg_ptr  ,
  /* Pointer to the RB Reconfig message        */
  tdsrrc_RadioBearerReconfiguration_r7_IEs *rbrc_ptr
)
{
  tdsrrc_CN_InformationInfo_r6              *cn_info_ptr = NULL; 
  uint32                                  localrb_count;
  tdsrrcrbrc_rlc_stop_cont_e_type            rlc_stop_cont_e_value = TDSRB_STOP_CONTINUE_ABSENT;
  boolean                                 is_valid_rbid = TRUE;
  uint16                                  invalid_rbid = 0xff;
  rlc_lc_id_type                          lc_id;
  uecomdef_logch_mode_e_type              lc_mode;
  tdsrrc_RadioBearerReconfiguration         *msg_ptr; /* Pointer to the RB Reconfig message        */
  tdsrrc_msg_ver_enum_type                   msg_version = TDSMSG_REL7;
  tdsrrc_RB_InformationReconfig_r7  *rb_info_reconfig_r7_ptr = NULL;
  tdsrrc_RB_InformationAffectedList_r7  *rb_info_affected_list_r7_ptr = NULL;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCEUL: Recvd RBRC r7 Msg sub_st%d",tdsrrc_rbrc.curr_substate);
    /* Get the pointer to the actual RB Reconfig message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerReconfiguration);
  if (FAILURE == 
    tdsrrcrbrc_check_and_send_failure_for_simul_reconfig(tdstransaction_id))
  {
    return;
  }

  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  if(rbrc_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"rbrc_ptr is NULL.");
    return;
  }
  else
  {

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rbrc_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rbrc_ptr->m.activationTimePresent,
         rbrc_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBRC SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbrc_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rbrc_ptr->m.activationTimePresent = 0;
    }
  }

#endif
    tdsrrcrbrc_validate_rbrc_message_r7( rbrc_ptr );
  }


  if ( tdsrrc_rbrc.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    /* Send the RB Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                               tdsrrc_rbrc.status.prot_err_cause
                             );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    tdsrrcrbrc_clear_procedure();
    return;
  }

  if ((msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.r7.m.v780NonCriticalExtensionsPresent)
      && (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.r7.v780NonCriticalExtensions.m.v790NonCriticalExtensionsPresent))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: rrc_v790NonCriticalExtensions_present not yet supported");
  } 
        
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r7_IEs,cn_InformationInfo))
  {
    cn_info_ptr = &(rbrc_ptr->cn_InformationInfo);

    /* CN info presents. */
    if (cn_info_ptr != NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      tdsrrc_rbrc.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(cn_info_ptr);
      if (tdsrrc_rbrc.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbrc.cn_info_saved = TRUE;
      }
    }
  }



  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r7_IEs,ueMobilityStateIndicator)) &&
      (rbrc_ptr->ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in RBRC.");
    tdsrrc_rbrc.high_mobility_ind = TRUE;
  }

  /* Process madatory RB information to reconfigure */
  /* Process madatory RB information to reconfigure */
  if ((rbrc_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent) &&
      rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList.n != 0 )
  {
    /*store the rb ids tobe released */
    tdsrrc_rbrc.rbs_tobe_reconfigured.is_valid = TRUE;
    
    localrb_count = 0;
    while (rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList.n > localrb_count)
    {
      rb_info_reconfig_r7_ptr = &rbrc_ptr->specificationMode.u.complete.rb_InformationReconfigList.elem[localrb_count];
      // Initialize the enum to this value.
      rlc_stop_cont_e_value = TDSRB_STOP_CONTINUE_ABSENT;

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_r7_ptr, 
          tdsrrc_RB_InformationReconfig_r7,rb_StopContinue))
      {
        lc_id = tdsrrclcm_get_ul_rlc_lc_id(rb_info_reconfig_r7_ptr->rb_Identity);

        (void) tdsrrclcm_get_ul_rlc_mode(lc_id, &lc_mode);

        if (rb_info_reconfig_r7_ptr->rb_StopContinue == tdsrrc_RB_StopContinue_stopRB)
        {
          rlc_stop_cont_e_value = TDSRB_STOP;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Stop recvd for RB %d",(uint16)(rb_info_reconfig_r7_ptr->rb_Identity));
        }
        else if (rb_info_reconfig_r7_ptr->rb_StopContinue == tdsrrc_RB_StopContinue_continueRB)
        {
          rlc_stop_cont_e_value = TDSRB_CONTINUE;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Continue recvd for RB %d",(uint16)(rb_info_reconfig_r7_ptr->rb_Identity));
        }
      }

      tdsrrc_rbrc.rbs_tobe_reconfigured.rb_id[localrb_count] = 
          rb_info_reconfig_r7_ptr->rb_Identity;
      tdsrrc_rbrc.rbs_tobe_reconfigured.rb_stop_continue[localrb_count] = rlc_stop_cont_e_value;

      tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] = tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] = 0;
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_r7_ptr,
	  	tdsrrc_RB_InformationReconfig_r7,rlc_Info))&&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_r7_ptr->rlc_Info,
        tdsrrc_RLC_Info_r7,ul_RLC_Mode)))
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSMODIFY_RB_CFG;
      }
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_r7_ptr,
	  	tdsrrc_RB_InformationReconfig_r7,rlc_Info))&&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_r7_ptr->rlc_Info,
        tdsrrc_RLC_Info_r7,dl_RLC_Mode)))
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSMODIFY_RB_CFG;
      }
      if(rlc_stop_cont_e_value == TDSRB_STOP)
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSSTOP_RB_CFG;
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSSTOP_RB_CFG;
      }
      else if(rlc_stop_cont_e_value == TDSRB_CONTINUE)
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSCONTINUE_RB_CFG;
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSCONTINUE_RB_CFG;
      }

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"LocalRBCnt %d, rb_id %d, rlc_stop_cont %d", localrb_count,
               (uint16)(rb_info_reconfig_r7_ptr->rb_Identity), rlc_stop_cont_e_value);
      localrb_count++;
    }
    tdsrrc_rbrc.rbs_tobe_reconfigured.num_rbs = localrb_count;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Num of RBs tobe reconfigured:%d",localrb_count);
  }

  /* Continue further if there are no invalid Rbids in recnfig list */
  if (is_valid_rbid)
  {
    /*Store the rb ids tobe affected */
    if (rbrc_ptr->specificationMode.u.complete.m.rb_InformationAffectedListPresent)
    {
      tdsrrc_rbrc.rbs_tobe_affected.is_valid = TRUE;
      rb_info_affected_list_r7_ptr = 
        &rbrc_ptr->specificationMode.u.complete.rb_InformationAffectedList;
      localrb_count = 0;
      while ( rb_info_affected_list_r7_ptr->n > localrb_count )
      {
        /*first check whether this rb-id exists in established_rab variable or not */
        if (! tdsrrcrbrc_validate_rb_id ((rb_info_affected_list_r7_ptr->elem[localrb_count].rb_Identity)))
        {
          /*rab-id does not exist in established_rab*/
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          is_valid_rbid = FALSE;
          invalid_rbid = (uint16)(rb_info_affected_list_r7_ptr->elem[localrb_count].rb_Identity);
          /*one of the rab-id does not exist in established_rab*/
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"rb-id %d is not in established_rab",invalid_rbid);
          break;
        }
        tdsrrc_rbrc.rbs_tobe_affected.rb_id[localrb_count] = 
          rb_info_affected_list_r7_ptr->elem[localrb_count].rb_Identity;
        localrb_count++;
      }
      tdsrrc_rbrc.rbs_tobe_affected.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Num of RBs tobe affected:%d",localrb_count);
    }
  }

  if (!is_valid_rbid)
  {
    /*one of the rab-id does not exist in established_rab*/
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"rb-id %d is not in established_rab",invalid_rbid);
    /* Send the RB Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                               tdsrrc_rbrc.status.prot_err_cause
                             );
    /* Make sure we're back in the initial sub-state */
    tdsrrcrbrc_clear_procedure();
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    return;
  }
  /* For Cell_FACH or Cell_DCH continue with the ordered config setting */
  tdsrrcrbrc_update_ordered_config_r7 (dcch_msg_ptr, msg_version);
} /* tdsrrcrbrc_process_rbrc_message_r7 */

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/*===========================================================================

FUNCTION   RRCRBRC_PROCESS_NEW_CELL_IND

DESCRIPTION

  This function processes the received new cell ind command.
   It handles it in CELL FACH state.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_process_new_cell_ind
(
  tdsrrc_new_cell_ind_type *ptr /* Pointer to the State Change Ind cmd */
)
{
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Process NEW_CELL_IND ,Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
  /* If the UE has camped on directed cell then do not send a cell update,
    * else send a CELL update with cause "cell Re-selection 
    */
  
  if(tdsrrccsp_is_selected_cell_utran_directed()&&
      (tdsrrc_rbrc.new_crnti_valid))    
  {
    /* No need to initiate cell update as the C-RNTI is available
     * and UE is camped on the directed cell.
     */
    tdsrrcrbrc_send_initiate_cu_flag = FALSE;
  }
  else
  {
    tdsrrcrbrc_send_initiate_cu_flag = TRUE;
    tdsrrcrbrc_delete_crnti          = TRUE;
  }
  
  /* Change the substate since we're waiting for the
   confirmation from LLC for the channel configs */
  tdsrrcrbrc_send_channel_config_req();
  tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;

}

#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
/*===========================================================================

FUNCTION   tdsrrcrbrc_send_rbrc_failure_unsupported_config

DESCRIPTION

  This function sends the rbreconfig-failure messsage with cause unsupported
  configuration because UE dont support the extension provided by the network.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_send_rbrc_failure_unsupported_config
(
  /* transaction id of RB Reconfig message */
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id  
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Un supported revision of RBRC msg");

  /* The first thing to do is check the transaction id.
  If the transaction id is the same as the accepted
  or rejected transaction ids, we don't process
  this message. It's a duplicate and will just be
  dropped. But we also don't change the current
  sub-state since we're in the middle of
  processing a previous message. Note that
  the transaction id is included only if critical
  extensions are not present. */
  if ( tdstransaction_id == tdsrrc_rbrc.tr_ids.rejected_transaction )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBRC msg - rejected tr-id%d match!",tdsrrc_rbrc.tr_ids.rejected_transaction);
    /* Nothing else to do, just ignore the message and return */
  } 
  else
  {
    tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    tdsrrc_rbrc.tr_ids.rejected_transaction = tdstransaction_id;

    /*Check whether Cell Update is going on */
    if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
         (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
      (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBRC);
      /* Wait until Cell update procedure is completed */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
    }
    else
    {
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause );
    }
  }
}
/*===========================================================================

FUNCTION   PROCESS_RBRC_MESSAGE

DESCRIPTION

  This function processes the received RB Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Reconfig Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Reconfig Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_process_rbrc_message
(
  tdsrrc_cmd_type *cmd_ptr
)
{

  tdsrrc_DL_DCCH_Message                    *dcch_msg_ptr;      /* Pointer to the RB Reconfig message */
  tdsrrc_RadioBearerReconfiguration         *msg_ptr; /* Pointer to the RB Reconfig message        */
#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER  
  /*add for srns reallocation check*/
  rrc_lai_type  lai;
#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS  
  rrc_rai_type rai;
#endif
  boolean trigger_srns_reallocation_op = FALSE;
  /*add for srns reallocation check end*/
#endif  
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd. downlink_sdu_ind.decoded_msg;

  /* If ASN.1 decoding failed, the received pointer would be null.
  So we know we got a RB Reconfig Message (by peeking into
  the encoded bit stream), but we were not able to decode it.
  Send a RB reconfig failure message with the appropriate
  cause. */
  if ( dcch_msg_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"ASN decode failed for RB Reconfig msg");
    tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
    tdsrrc_rbrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
    tdsrrc_rbrc.tr_ids.rejected_transaction = 0;
    tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                               tdsrrc_rbrc.status.prot_err_cause
                             );
    

    /* Nothing else to do here 
    continue with the same substate because some other reconfiguration
    message might be ongoing*/
    return;
  }

  /* Make sure it's the Radio Bearer Reconfig message
  although we would not have gotten here unless it's
  the right message */
  if ( dcch_msg_ptr->message.t != T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Incorrect message given to RBRC proc %d",dcch_msg_ptr->message.t);
    return;
  }   

  if(TRUE == tdsrrccsp_reselection_in_progress())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Abort cell reselection as it's conflict with RBRC");
    tdsrrccsp_abort_cell_reselection();
  }

  /* Get the pointer to the actual RB Reconfig message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerReconfiguration);

  /* Unsupport rel99 message */
  if (msg_ptr->t == T_tdsrrc_RadioBearerReconfiguration_r3)
  {
    tdsrrc_print_supported_asn1_rel_version();

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Configuration rel99 not supported.  Send RB reconfig failure");

    tdsrrcrbrc_send_rbrc_failure_unsupported_config(
        msg_ptr->u.r3.radioBearerReconfiguration_r3.rrc_TransactionIdentifier);      
    return;
  }
  /* This RBRC msg is Rel4/Rel5/Rel6/Rel7/Rel8 version. */
  if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration, TDSMSG_REL4))
  {
    /* check for SRNS relocation scenario */
    if (FAILURE == tdsrrcrbrc_validate_and_check_integrity_for_srns_relocation(
          cmd_ptr, msg_ptr, msg_ptr->u.later_than_r3.rrc_TransactionIdentifier, 
          TDSMSG_REL4))
    {
      /*do not clear the procedure or change the tdsrrc_rbr substate because
       some other rbreconfig procedure might be in progress. Just return from
       here */
      return;
    }   
    tdsrrcrbrc_process_rbrc_message_r4(cmd_ptr);
  }
  else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
      (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration, TDSMSG_REL5)))
  {
    /* check for SRNS relocation scenario */
    if (FAILURE == tdsrrcrbrc_validate_and_check_integrity_for_srns_relocation(
          cmd_ptr, msg_ptr, msg_ptr->u.later_than_r3.rrc_TransactionIdentifier, 
          TDSMSG_REL5))
    {
      /*do not clear the procedure or change the tdsrrc_rbr substate because
         some other rbreconfig procedure might be in progress. Just return from
         here */
      return;
    }
    tdsrrcrbrc_process_rbrc_message_r5(cmd_ptr);
  }
  else if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6) &&
      (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration, TDSMSG_REL6)))
  {
    /* check for SRNS relocation scenario */
    if (FAILURE == tdsrrcrbrc_validate_and_check_integrity_for_srns_relocation(
        cmd_ptr, msg_ptr, msg_ptr->u.later_than_r3.rrc_TransactionIdentifier, 
        TDSMSG_REL6))
    {
      /* do not clear the procedure or change the tdsrrc_rbr substate because
   some other rbreconfig procedure might be in progress. Just return from
   here */
      return;
    }

    tdsrrcrbrc_process_rbrc_message_r6(
      msg_ptr->u.later_than_r3.rrc_TransactionIdentifier,
      (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg,
      &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.r6.radioBearerReconfiguration_r6
    );
  }
  else if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7) &&
      (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration, TDSMSG_REL7)))
  {
    /* check for SRNS relocation scenario */
    if (FAILURE == tdsrrcrbrc_validate_and_check_integrity_for_srns_relocation(
        cmd_ptr, msg_ptr, msg_ptr->u.later_than_r3.rrc_TransactionIdentifier, 
        TDSMSG_REL7))
    {
      /* do not clear the procedure or change the tdsrrc_rbr substate because
    some other rbreconfig procedure might be in progress. Just return from
    here */
      return;
    }

    tdsrrcrbrc_process_rbrc_message_r7(
      msg_ptr->u.later_than_r3.rrc_TransactionIdentifier,
      (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg,
      &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.r7.radioBearerReconfiguration_r7
    );
  }
#ifdef FEATURE_TDSCDMA_REL8
  /* Now check whether R8 RBRC OTA is given or not*/
  else if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8) &&
      (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration, TDSMSG_REL8)))
  {
    /* check for SRNS relocation scenario */
    if (FAILURE == tdsrrcrbrc_validate_and_check_integrity_for_srns_relocation(
        cmd_ptr, msg_ptr, msg_ptr->u.later_than_r3.rrc_TransactionIdentifier, TDSMSG_REL8))
    {
      /* do not clear the procedure or change the tdsrrc_rbr substate because
   some other rbreconfig procedure might be in progress. Just return from
   here */
      return;
    }
    tdsrrcrbrc_process_rbrc_message_r8(msg_ptr->u.later_than_r3.rrc_TransactionIdentifier,
      (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg,
      &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerReconfiguration_r8);
  }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  /* Now check whether R9 RBRC OTA is given or not*/
  else if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9) &&
      (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration, TDSMSG_REL9)))
  {
    /* check for SRNS relocation scenario */
    if (FAILURE == tdsrrcrbrc_validate_and_check_integrity_for_srns_relocation(
        cmd_ptr, msg_ptr, msg_ptr->u.later_than_r3.rrc_TransactionIdentifier, TDSMSG_REL9))
    {
      /* do not clear the procedure or change the tdsrrc_rbr substate because
   some other rbreconfig procedure might be in progress. Just return from
   here */
      return;
    }
    tdsrrcrbrc_process_rbrc_message_r9(msg_ptr->u.later_than_r3.rrc_TransactionIdentifier,
      (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg,
      &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
      u.r9.radioBearerReconfiguration_r9);
  }
#endif /*  FEATURE_TDSCDMA_REL9 */

  else
  {
    tdsrrc_print_supported_asn1_rel_version();

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Configuration not supported.  Send RB reconfig failure");

    tdsrrcrbrc_send_rbrc_failure_unsupported_config(
        msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);      
  }

#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER  
  /*Check if RBRC include SRNS reallocation and whether there's LAC/RAC change
      Use tdsrrc_rbrc.cn_info_saved to check, if SRNS reallocation change and CN info is sent, \
      current rrc state and target rrc state are both DCH, we will set it to true
  */
  if((TRUE == tdsrrc_rbrc.cn_info_saved )&&(TDSRRC_STATE_CELL_DCH == tdsrrc_rbrc.state_indicated_by_msg)
  	&&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
	/*compare PLMN id in ccm and ota directly, there's exception in networking sharing, which will store in CCM directly, but will no happend in tds*/
	if(0 != memcmp(&(tdsrrc_rbrc.mm_cmd_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id),
		    &(tdsrrc_ccm_cell_camping_info.curr_cell.plmn_id),sizeof(rrc_plmn_identity_type)))
	{
		trigger_srns_reallocation_op = TRUE;
	}

	/*if no PLMN in ota cn info, we will set mcc to 0xFF, so need to set op to false to continue check LAC/RAC*/
	if((0xff == tdsrrc_rbrc.mm_cmd_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0])
		&& (0xff == tdsrrc_rbrc.mm_cmd_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1])
		&& (0xff == tdsrrc_rbrc.mm_cmd_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]))
	{
	       trigger_srns_reallocation_op = FALSE;
	}
  
  	/*if we have ota lac/rac compare with stored ota lac/rac*/
       if(TRUE == tdslac_in_ota_present_bak)
   	{
   		if(((tdslac_in_ota[0] != tdslac_in_ota_bak[0])||(tdslac_in_ota[1] != tdslac_in_ota_bak[1]))&&(TRUE == tdslac_in_ota_present))
		{
			trigger_srns_reallocation_op = TRUE;
		}
   	}
#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS	   
	else if(TRUE == tdsrac_in_ota_present_bak)
	{
		if((tdsrac_in_ota[0] != tdsrac_in_ota_bak[0])&&(TRUE == tdsrac_in_ota_present))
		{
			trigger_srns_reallocation_op = TRUE;
		}
	}
#endif	
	else  /*compare with cell lac /rac if no stored ota lac/rac */
	{
		if ((SUCCESS == tdsrrc_get_lac_id(&lai))&&(TRUE == tdslac_in_ota_present))
		{
			if((tdslac_in_ota[0] != lai.lac[0]) || (tdslac_in_ota[1] != lai.lac[1]))
			{
				trigger_srns_reallocation_op = TRUE;
			}
		}
#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
		if((SUCCESS == tdsrrc_get_rac_id(&rai))&&(TRUE == tdsrac_in_ota_present))
		{
			if(tdsrac_in_ota[0] != rai.rac[0])
			{
				trigger_srns_reallocation_op = TRUE;
			}
		}
#endif		
	}

	if(TRUE == trigger_srns_reallocation_op)
	{
	   /*set srns trigger flag*/
	   tdsrrc_rbrc.srns_reallocation_trigger = TRUE;
	}
  }
#endif  
  /* return - because all processing are done inside the function */
  return;
}


/*===========================================================================

FUNCTION   RRCRBRC_PROCESS_RNTI_UPDATE_CNF

DESCRIPTION

  This function processes RNTI_UPDATE_CNF
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_process_rnti_update_cnf
(
void
)
{
  tdsrrc_state_e_type curr_state;

  curr_state = tdsrrc_get_state();

  if ((curr_state == TDSRRC_STATE_CELL_FACH) && 
      ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
       (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)))
  {
   /*send the response message now */
    tdsrrcrbrc_send_rbrc_complete_message(TRUE);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
    /*wait for L2ack */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    tdsrrcllc_print_trans_chnl_rate();
  }
  else
  {
    /* We should not get here*/
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unexpected RNTI Update cnf,tdsrrc_rbrc state %d",tdsrrc_rbrc.curr_substate);
  }
}

/*===========================================================================

FUNCTION   RRCRBRC_PROCESS_CHANNEL_CONFIG_CNF

DESCRIPTION

  This function processes the received Channel Config Confirmation
  from LLC. If the required Radio-bearers have been reconfig succesfully,
  and if any of the radio-bearers map to radio-access-bearers, this
  information needs to be given to the NAS entity for that RAB.
  
  Currently Circuit-Switched voice and Circuit-Switched data RABs are 
  supported. If a voice RAB has been assigned by the UTRAN, this function
  gets the RLC ids and the AMR mode information from LLC and sends this
  information to voice services. If a CS Data RAB has been assigned, the
  RLC ids for those RBs are sent to the data services entity. Then this
  function sends the RRC_SYNC_IND primitive to MM.
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_process_channel_config_cnf
(
tdsrrc_channel_config_cnf_type *ptr  /* Pointer to the Channel config cnf cmd */
)
{
  tdsrrc_state_e_type            curr_state;
  tdsrrcrb_rnti_update_info_type rnti_update_info;   /* to hold rnti update info */
  tdsrrc_cmd_type *cmd; /* To send a LLC command if channel config failed */
  tdsrrc_CellUpdateCause       cu_cause; /*To store CU cause from CU procedure*/
  tdsrrc_rlc_re_est_for_srns_rb_e_type rb_type; /*rb type tobe reestablished */

  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();

  /* Check if the channel configuration was succesful. */
  if ( ptr->chan_cfg_status != TDSRRCLLC_CHAN_CFG_SUCCESS )
  {
    /* Channel config failure, so initialise the ordered RB-Mapping database*/
    tdsrrcllc_init_ordered_hanging_rb_mapping_info();

    /*Here we initialize tdsrlc_size_change_in_progress DB so that if it was set by RBRC, 
    it is ready for next procedure*/
    tdsrrc_initialize_rlc_size_change();

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CHAN_CONFIG_CNF with Failure");
    tdsrrc_set_ordered_codec_mode(nas_sync_ind);
    tdsrrc_rbrc.status.failure_status = TDSRRCRB_PHY_CHAN_FAILURE;
    if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_CFG_CANCELLED)
    {
      #ifdef FEATURE_TDSCDMA_RCR_CANCEL_RECFG
      if (TRUE == tdsrrcrcr_is_proc_active())
      {
        if (TDSOC_NOT_SET == tdsrrcllc_reset_ordered_config(TDSRRC_PROCEDURE_RBRC
                                                      ,TRUE
                                                      ))
        {
          /* Then clear all local variables that may
          need to be cleared */
          tdsrrcrbrc_clear_procedure();
          /* Make sure we're back in the initial sub-state */
          tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
        }
      }
      else
      #endif
      {
         /* cell update is already in progress as this is for pending config
         * cancellation
         */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Channel Config cancel waiting for CU complete");
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      }

      return; 
    }
    
    /* validation failure happened.  Send RB failure msg with invalid config */
    if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_OTHER)
    {
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      if(tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG)
      {
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBRC, TDSRRC_UNRECOVERABLE_ERROR, FALSE);
        return; 
      }

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RBRC failure msg with invalid config");
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause
                                 );
      /* Reset TDSORDERED_CONFIG - UE can now accept other ordered
      configurations */
      if (TDSOC_NOT_SET == tdsrrcllc_reset_ordered_config(TDSRRC_PROCEDURE_RBRC
                                                    ,TRUE
                                                    ))
      {
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbrc_clear_procedure();
        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to reset the OC");
      }
      /*During FACH->DCH transition, if T305 got expired, we need to start T305 with zero timeout.*/	  
      if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH && tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
      {
        if((tdsrrctmr_get_remaining_time(TDSRRCTMR_T_305_TIMER) == 0)
            && (tdsrrc_convert_t305_to_ms(TDSRRC_GET_T305()) != TDSRRCTMR_INFINITY_VAL ))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Chan_config_req failed for FACH->DCH. So Starting T305 with zero timeout.");        
          tdsrrctmr_start_timer(TDSRRCTMR_T_305_TIMER, 0);
        }
      }
      return;
    } /* if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_OTHER) */

    /* Check if PHY_CHAN_FAILURE happened in FACH->DCH Transition.  If so, initiate a cell update
    request, and tdsrrc_rbrc substate should be TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF */ 
    if (tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH && tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PHYCHANFAIL FACH->DCH.  Initiate CU");

      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_PHY_CHAN_FAILURE;
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBRC, TDSRRC_PHY_CHAN_FAILURE,TRUE);
      /* Wait for CELL UPDATE to get a valid C_RNTI */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      tdsrrcrbrc_send_initiate_cu_flag = FALSE;
      tdsrrcrbrc_delete_crnti          = FALSE;
      return;
    }
    /* Check if PHY_CHAN_FAILURE happended within CELL_DCH.  For now, initatiate a cell update request
       with cause RL Failure.  RBRC substate should be TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF*/
    else if (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH && tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
    {
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_PHY_CHAN_FAILURE;

      if(tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG)
      {
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBRC, TDSRRC_RADIO_LINK_FAILURE, FALSE);
        return; 
      }

      if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_PHY_CHL)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBreconfig failed,going back to old cfg success");
        tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                   tdsrrc_rbrc.status.prot_err_cause
                                   );
        /* Go back to initial substate */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbrc_clear_procedure();
        /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
        configurations */
        tdsrrcllc_clear_ordered_config(); 
      }
      else if(ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_BACK_TO_OLD_CONFIG)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBreconfig failed.  Old config failed.  Initiate CU");
        /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBRC, TDSRRC_RADIO_LINK_FAILURE,TRUE);
        /* Wait for CELL UPDATE to get a valid C_RNTI */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        tdsrrcrbrc_send_initiate_cu_flag = FALSE;
        tdsrrcrbrc_delete_crnti          = FALSE;
      }
    }
    else
    {
      if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
          (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
      {
        /* go to idle */
        if ( (cmd=tdsrrc_get_int_cmd_buf())!=NULL)
        { 
          cmd->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;
          /* Need to change state */
          cmd->cmd.chan_config_req.rrc_state_change_required = TRUE;
          /* Next state is disconnected state */
          cmd->cmd.chan_config_req.next_state = TDSRRC_STATE_DISCONNECTED;
          /* We don't need a confirm for this command */
          cmd->cmd.chan_config_req.rrc_channel_config_cnf_required = FALSE;
          /* Procedure name */
          cmd->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RBRC;
          /* No need to fill any other parameter in this command.
          When LLC sees the next state is Disconnected State, it
          will tear down all the Radio Bearers. */
          tdsrrc_put_int_cmd( cmd );
        } 
        else
        {
          ERR_FATAL("No memory for chan config cmd",0,0,0);
        }

      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Channel Config Failed!");
        tdsrrc_rbrc.status.failure_status = TDSRRCRB_PHY_CHAN_FAILURE;
        tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                   tdsrrc_rbrc.status.prot_err_cause
                                   );
      }
      /* Go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
      /* Then clear all local variables that may
      need to be cleared */
      tdsrrcrbrc_clear_procedure();
    }
    return;
  }


  if((tdsordered_config.set_status == TDSOC_SET_FOR_DCH_FACH_TRANS) || 
      (tdsordered_config.set_status == TDSOC_SET_FOR_DCH_CELL_PCH_TRANS)||
      (tdsordered_config.set_status == TDSOC_SET_FOR_DCH_URA_PCH_TRANS))	
  {
    if(tdsrrc_rbrc.high_mobility_ind == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending High Mobility");
      tdsrrcrb_send_upd_ota_parms_ind(TDSRRC_PROCEDURE_RBRC, TDSL1_CPHY_HIGH_MOBILITY_INCL);
    }
  }

  /*For FACH->DCH, if CHAN_CONFIG_REQ is succeeded, we need to stop T305*/  
  if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH && tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Chan_config succeeded for FACH->DCH. So Stopping T305");
    tdsrrctmr_stop_timer(TDSRRCTMR_T_305_TIMER);
  }  

   /*Notify the Cell_Id to registered entities*/
  if(tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
  {
    tdsrrc_notify_cell_id_in_dch();
  }	
     
  /* Copy the Ordered RB Mapping into the current RB-Mapping database*/
  tdsrrcllc_copy_ordered_hanging_rb_mapping_to_current();


  /*Here, we check and then configure for SCUDIF. Otherwise, we check for codec/rate
  change for AMR RAB. Notice that the SCUDIF/Rate change can happen with or without
  RAB Reconfig Info IE in RBRC OTA */
  tdsrrcrbrc_configure_rab_change();
     

  if(ptr->inter_freq_hho_status == TRUE)
  {
    tdsrrc_rbrc.hho_ciph_status = TRUE;
  }
  else
  {
    tdsrrc_rbrc.hho_ciph_status = FALSE;  
  }

  curr_state=tdsrrc_get_state();
  if ((TDSRRC_STATE_CELL_FACH == curr_state) && 
      ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH) ||
       (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
       (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)))
  {
    if (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) 
    {
      /* this is the case of CELL_FACH->CELL_PCH state transition.
      In current implementation we do initiate cell selection. Instead
      we treat current cell as strongest cell */
      if (tdsrrc_rbrc.directed_cell_info)
      {
        /* check whether current camped scr code is same as the one
        sent in the message. If not then initiate cell update procedure */
        if(!tdsrrccsp_is_selected_cell_utran_directed())
        {
          /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
          tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBRC,
                                              TDSRRC_CELL_RESELECTION,
                                              FALSE);
        }
        /* No need to send ul data registration to rlc. This will be taken care
        by cellupdate procedure */
        tdsrrcrbrc_process_successful_proc_completion(FALSE);
      }
      else
      {
        /* send drx info to L1. Since this is CELL_FACH->CELL_PCH state transition
        RB procedure is responsible for sending the DRX info to lower layer*/
        tdsrrcrb_send_l1_drx_req (TDSRRC_PROCEDURE_RBRC);

        tdsrrcrbrc_process_successful_proc_completion (TRUE);
      }
      /* return from here */
      return;
    }
    else if (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      /* send drx info to L1 */
      tdsrrcrb_send_l1_drx_req (TDSRRC_PROCEDURE_RBRC);
      /* check whether URA update is required */
      if ( tdsrrccu_is_ura_update_reqd() == TRUE)
      {
        tdsrrcrb_send_ura_update_initiate_req( TDSRRC_PROCEDURE_RBRC,
                                            TDSRRC_URA_RESELECTION,
                                            FALSE);
      }
      /* no need to wait for ura update completion 
      declare that proc is successfully completed */
      tdsrrcrbrc_process_successful_proc_completion (TRUE);
      /* return from here */
      return;
    }
    if(tdsrrcrbrc_send_initiate_cu_flag
       || (tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_RB_PROC_COMPL_IND) 
       )
    {
      if (tdsrrcrbrc_delete_crnti)
      {
        /* Send a RNTI Update Request */
        rnti_update_info.crnti_action         = TDSRRC_RNTI_DELETE;
        rnti_update_info.urnti_action         = TDSRRC_RNTI_NOCHANGE;
        rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBRC;
        rnti_update_info.rlc_re_establish_srb = FALSE;
        rnti_update_info.rlc_re_establish_rab = FALSE;
        rnti_update_info.cnf_reqd             = FALSE;

        tdsrrcrb_send_rnti_update_req(rnti_update_info);
      }
      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBRC,
                                          TDSRRC_CELL_RESELECTION,
                                          TRUE);
      /* Wait for CELL UPDATE to get a valid C_RNTI */
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_VALID;
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      tdsrrcrbrc_send_initiate_cu_flag = FALSE;
      tdsrrcrbrc_delete_crnti          = FALSE;
      return;
    }
    /*Following case will occur when RLC RESET happens on SRB2 during DCH->FACH Transition*/
    else if((tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG) &&
      (tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) == TDSRRCCU_STARTED) && 
      (cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError) && (tdsrrccu_get_rb_id_unrec_error() == 
      DCCH_AM_RADIO_BEARER_ID))
    {
      if(tdsrrc_rbrc.new_crnti_valid)
      {
        /* Send a RNTI Update Request */
        rnti_update_info.crnti_action         = TDSRRC_RNTI_UPDATE;
        rnti_update_info.urnti_action         = TDSRRC_RNTI_NOCHANGE;
        rnti_update_info.crnti                = tdsrrc_rbrc.new_crnti;
        rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBRC;
        rnti_update_info.rlc_re_establish_srb = FALSE;
        rnti_update_info.rlc_re_establish_rab = FALSE;
        rnti_update_info.cnf_reqd             = FALSE;
  
        tdsrrcrb_send_rnti_update_req(rnti_update_info);
        /* Send a Radio Bearer Setup Complete later */      
      }

      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBRC,
                                          TDSRRC_UNRECOVERABLE_ERROR,
                                          TRUE);    

      /* Wait for CELL UPDATE to get a valid C_RNTI */
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_VALID;
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;

      return;                                          
    }
    /* Coming here implies the "tdsrrcrbrc_send_initiate_cu_flag" is FALSE.
     * Check if Valid CRNTI is stored in RBRC (for DCH->FACH case).
     * If so, send a RNTI Update Req to LLC and NOT wait for
     * Cell Update Complete Confirm. Cell Update will NOT be 
     * initiated when there is a valid C-RNTI and tdsrrcrbrc_send_initiate_cu_flag
     * is FALSE.
     */
    else if(tdsrrc_rbrc.new_crnti_valid)
    {
      /* Send a RNTI Update Request */
      rnti_update_info.crnti_action         = TDSRRC_RNTI_UPDATE;
      rnti_update_info.urnti_action         = TDSRRC_RNTI_NOCHANGE;
      rnti_update_info.crnti                = tdsrrc_rbrc.new_crnti;
      rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBRC;
      rnti_update_info.rlc_re_establish_srb = FALSE;
      rnti_update_info.rlc_re_establish_rab = FALSE;
      rnti_update_info.cnf_reqd             = FALSE;

      tdsrrcrb_send_rnti_update_req(rnti_update_info);
      /* Send a Radio Bearer Setup Complete later */      
    }
      /* Query for valid C-RNTI */
    else if (TDSC_RNTI_NOT_VALID == tdsrrcllc_get_current_crnti_status() )
    {
      /* Wait for CELL UPDATE to get a valid C_RNTI. Register for CU complete cnf. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No Valid C_RNTI! Register for CU CNF");
      (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBRC);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_VALID;
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      return;
    }

  } /* if current state is CELL_FACH */
  else if (TDSRRC_STATE_CELL_PCH == curr_state)
  {
    if(tdsrrcrbrc_send_initiate_cu_flag)
    {
      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBRC,
                                          TDSRRC_CELL_RESELECTION,
                                          FALSE);
      tdsrrcrbrc_send_initiate_cu_flag = FALSE;
      /* No need to send ul data registration to rlc. This will be taken care
      by cellupdate procedure */
      tdsrrcrbrc_process_successful_proc_completion(FALSE);
      return;
    }
    else
    {
      /*Perform the final steps for successful completion of the
      procedure */
      tdsrrcrbrc_process_successful_proc_completion(TRUE);
      return;
    }
  }
  else if(TDSRRC_STATE_URA_PCH == curr_state)
  {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU    
    /* check whether current camped scr code is same as the one
    sent in the message. If not then initiate cell update procedure */
    if (!((tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause)== TDSRRCCU_STARTED) &&
        (cu_cause == tdsrrc_CellUpdateCause_re_enteredServiceArea) &&
       ( (tdsrrcllc_get_toc_usage() ==TDSTOC_FOR_OOS)||
       (tdsrrcllc_get_toc_usage() ==TDSTOC_FOR_OOS_WITHOUT_DCH_INFO)||
       (tdsrrcllc_get_toc_usage() ==TDSTOC_FOR_OOS_WITH_DCH_INFO))))
#endif
    {
      /* check whether URA update is required */
      if ( tdsrrccu_is_ura_update_reqd() == TRUE)
      {
        tdsrrcrb_send_ura_update_initiate_req( TDSRRC_PROCEDURE_RBRC,
                                            TDSRRC_URA_RESELECTION,
                                            FALSE);
      }
    }
    /* No need to send ul data registration to rlc. This will be taken care
    by cellupdate procedure */
    tdsrrcrbrc_process_successful_proc_completion(TRUE);
    return;
  }


  /*check whether this reconfiguration involves SRNS relocation */
  if (tdsrrc_rbrc.rbrc_for_srns_relocation == TRUE)
  {
    if (TDSRRC_STATE_CELL_DCH == curr_state)
    {
  
      /* Re-establish SRB2 before sending the response message */
      rb_type  = TDSRRC_RE_EST_RB_2;
      tdsrrcrb_send_rlc_re_est_req_to_smc(TDSRRC_PROCEDURE_RBRC, rb_type, TRUE);
  
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_REESTABLISH_SRB2;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"SRNS not supp. in state %d",curr_state);
    }
  }
  else /*normal reconfiguration without SRNS relocation */
  {   
    /* Send the Radio Bearer Reconfig Complete message */
    if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
        (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

    {
      tdsrrcrbrc_send_rbrc_complete_message(TRUE);
    }
    else
    {
      /* Need to wait for L2 ack for all states because RRC needs to trigger include reconfig status indicator
      if RLC acknowledgement is not yet received.
      As per 8.3.1.3
      1> if the IE "Cell update cause" is set to "radio link failure" and the UE has submitted a reconfiguration response
      message to RLC and the UE has not received the RLC acknowledgement for the response message:
      2> include the IE "Reconfiguration Status Indicator" and set it to TRUE.*/
        tdsrrc_update_reconfig_waiting_for_l2ack_dch(TDSRRC_PROCEDURE_RBRC);
        tdsrrcrbrc_send_rbrc_complete_message(TRUE);
      /* IF CU is waiting to handle RLF/RLC unrecoverable error, then send complete
       * and delay for 50ms so to make sure Complete goes out to NW on old config
       */
      if((tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG)
        && (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH))
      {
        /* wait for some time before initiating cell update */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delaying by %d ms ",TDSRRCRB_PND_CFG_DELAY);
        (void) rex_clr_sigs(rex_self(), TDSRRCRBRC_TEMP_TIMER_SIG);
        (void) rex_set_timer(&tdsrrcrbrc_temp_timer, TDSRRCRB_PND_CFG_DELAY);
    
        tdsrrcrbrc_sig = rex_wait(TDSRRCRBRC_TEMP_TIMER_SIG);
        TDSRRC_MSG1(MSG_LEGACY_LOW,"RRCRBRC signal:%d",tdsrrcrbrc_sig);
        (void) rex_clr_sigs(rex_self(), TDSRRCRBRC_TEMP_TIMER_SIG);
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBRC, TDSRRC_UNRECOVERABLE_ERROR, FALSE);
      } 
      /*Following case handles the scenario when RF Failure or RLC unrecoverable error occured
        during DCH -> FACH or FACH -> FACH transiton.*/
      else if((tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG) &&
        (tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) == TDSRRCCU_STARTED))
      {
        if((cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError) && (tdsrrccu_get_rb_id_unrec_error() != DCCH_AM_RADIO_BEARER_ID))
        {
          /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
          tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBRC,
                                              TDSRRC_UNRECOVERABLE_ERROR,
                                              TRUE);
        }
        else if(cu_cause == tdsrrc_CellUpdateCause_radiolinkFailure)
        {
          /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
          tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBRC,
                                              TDSRRC_RADIO_LINK_FAILURE,
                                              TRUE);
        }
      }
    }
    if (tdsrrcllc_get_ordered_config_status() != TDSOC_SET_FOR_DCH_FACH_TRANS)
    {
      /* prints rate for each transport channel */
      tdsrrcllc_print_trans_chnl_rate();
    }

    /*if next state is Cell_PCH or URA_PCH then we have to 
    treat the procedure as successful only after getting L2ack
    for the response message */
    if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
        (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

    {
      /* Change the substate to the next substate - this is
      where we wait for L2 ack from RLC for sending the RB RC
      complete message. */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    }
    else
    {
      /* clear the ordered config here
      treat the procedure as successful. */
      tdsrrcllc_clear_ordered_config();  

      /*inform upper layers if necessary */
      tdsrrcrbrc_send_indication_to_upper_layers();

      /* Go to the next sub-state in the state machine. That's
      the initial substate since we've sent the RB RC complete
      message */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* At this point we don't need to remember any of the
      Radio Bearer information since it's stored in ESTABLISHED_RABS.
      Clear our internal variables */
      tdsrrcrbrc_clear_procedure();
    }
  }
} /* tdsrrcrbrc_process_channel_config_cnf */


/*===========================================================================

FUNCTION   RRCRBRC_PROCESS_RLC_REESTABLISHMENT_CNF

DESCRIPTION

  This function processes the received rlc reestablishment cnf from SMC
  <more later>.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_process_rlc_reestablishment_cnf
(
void
)
{
  tdsrrcrb_rnti_update_info_type rnti_update_info; /* structure to be used to request RNTI update */

  /* Send the RNTI update request to rrcllc to update with new-URNTI */
  /* Send a RNTI Update Request */
  rnti_update_info.crnti_action         = TDSRRC_RNTI_NOCHANGE;
  rnti_update_info.urnti_action         = TDSRRC_RNTI_UPDATE;
  rnti_update_info.urnti                = tdsrrc_rbrc.new_urnti;
  rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBRC;
  rnti_update_info.rlc_re_establish_srb = FALSE;
  rnti_update_info.rlc_re_establish_rab = FALSE;
  rnti_update_info.cnf_reqd             = FALSE;

  tdsrrcrb_send_rnti_update_req(rnti_update_info);

  /* Send the reconfiguration complete message */

  tdsrrcrbrc_send_rbrc_complete_message(TRUE);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");

  /* Change the substate to the next substate - this is
     where we wait for L2 ack from RLC for sending the RB Reconfig
     complete message. */
  tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
  tdsrrcllc_print_trans_chnl_rate();
  
}

/*===========================================================================

FUNCTION   RRCRBRC_PROCESS_STATE_CHANGE_IND

DESCRIPTION

  This function processes the received State Change indication command.
  <more later>.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_process_state_change_ind
(
tdsrrc_state_change_ind_type *ptr /* Pointer to the State Change Ind cmd */
)
{
  tdsrrc_CellUpdateCause       cu_cause;
  /* Check if we're going to disconnected state */
  if ( ptr->new_state == TDSRRC_STATE_DISCONNECTED )
  {
    /* Then change to initial substate */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbrc_clear_procedure();
  }
  else if (ptr->new_state == TDSRRC_STATE_CELL_FACH )
  {
    if(ptr->previous_state == TDSRRC_STATE_CELL_DCH)
    {
      if (((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
          (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)) &&
           (tdsrrc_rbrc.curr_substate == TDSRRCRB_WAIT_FOR_L2_ACK))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"state change ind rbrc_st:%d",tdsrrc_rbrc.curr_substate); 
        /* Then change to initial substate */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

        /* Then clear all local variables that may
        need to be cleared */
        /* Note OC is set at this point. No need to clear it
        because cu procedure will take care of it */
        tdsrrcrbrc_clear_procedure();
      }
      else if (tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_VALID)
      {
        /* if cell update procedure is active with cause OOS, wait 
         * for the CU complete notification to send chan_config_req to
         * LLC 
         */
        if ((tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause)== TDSRRCCU_STARTED) &&
            (cu_cause == tdsrrc_CellUpdateCause_re_enteredServiceArea) &&
            (tdsrrcllc_get_toc_usage() ==TDSTOC_FOR_OOS))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Complete tdsrrc_rbrc msg first.  Indicate compl to CU once done");
        }
        /* Call the function that creates and sends the  channel
           config information to be requested from LLC. */
        tdsrrcrbrc_send_channel_config_req();

        /* Change the substate since we're waiting for the
           confirmation from LLC for the channel configs */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;

        if((tdsrrccsp_is_selected_cell_utran_directed()) &&
           (tdsrrc_rbrc.new_crnti_valid))
        {
           /* No need to initiate cell update as the C-RNTI is available
            * and UE is camped on the directed cell.
            */
          tdsrrcrbrc_send_initiate_cu_flag = FALSE;
        }
        else
        {
          tdsrrcrbrc_send_initiate_cu_flag = TRUE;
          tdsrrcrbrc_delete_crnti = TRUE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring STATE_CHANGE_IND");
      }

    }
    else
    {
      /* Currently there is no action when we go to cell_fach.
      This is not expected for release 1 since UE won't support
      transition to cell_fach. */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Recvd state change %d to %d tdsrrc_rbrc state %d, No action",
                ptr->previous_state, ptr->new_state, tdsrrc_rbrc.curr_substate);
    }
  }
  else if (ptr->new_state == TDSRRC_STATE_CELL_PCH || 
           ptr->new_state == TDSRRC_STATE_URA_PCH)
  {
    if(ptr->previous_state == TDSRRC_STATE_CELL_DCH)
    {

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
      /* Call the function that creates and sends the  channel
         config information to be requested from LLC. */
      tdsrrcrbrc_send_channel_config_req();
  
      /* Change the substate since we're waiting for the
         confirmation from LLC for the channel configs */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
    
      if (ptr->new_state == TDSRRC_STATE_CELL_PCH)
      {
        if (tdsrrc_rbrc.directed_cell_info)
        {
          if (tdsrrccsp_is_selected_cell_utran_directed()) 
          {
            /* No need to initiate cell update UE is camped on 
            the directed cell. */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Directed cell match,No CU reqd");
            tdsrrcrbrc_send_initiate_cu_flag = FALSE;
          }
          else
          {
            tdsrrcrbrc_send_initiate_cu_flag = TRUE;
            tdsrrcrbrc_delete_crnti          = TRUE;
          }
        }
        else
        {
          /* we have to initiate cell update incase cpich info is not present
          in the message */
          tdsrrcrbrc_send_initiate_cu_flag = TRUE;
          tdsrrcrbrc_delete_crnti          = TRUE;
        }
      }
      else
      {
        /* URA_PCH case */
        tdsrrcrbrc_send_initiate_cu_flag = FALSE;
      }
    }
 #ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
    /* Handle the case of FACH->PCH with Freq Redirection */
    else if( (ptr->previous_state == TDSRRC_STATE_CELL_FACH) && 
        (tdsrrc_rbrc.initiate_cell_selection_trans_from_fach == TRUE ))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
      /* Call the function that creates and sends a channel
         config request to LLC. */
      tdsrrc_rbrc.initiate_cell_selection_trans_from_fach = FALSE;
      if (ptr->new_state == TDSRRC_STATE_CELL_PCH)
      {
        if (tdsrrc_rbrc.directed_cell_info)
        {
          if(tdsrrccsp_is_selected_cell_utran_directed())
          {
            /* No need to initiate cell update as the C-RNTI is available
              * and UE is camped on the directed cell.
              */
            tdsrrcrbrc_send_initiate_cu_flag = FALSE;
          }
          else
          {
            tdsrrcrbrc_send_initiate_cu_flag = TRUE;
            tdsrrcrbrc_delete_crnti          = TRUE;
          }
        }
        else
        {
          /* we have to initiate cell update incase cpich info is not present
          in the message */
          tdsrrcrbrc_send_initiate_cu_flag = TRUE;
          tdsrrcrbrc_delete_crnti          = TRUE;
        }
      }
      else
      {
        /* No need of sending Cell update in case of URA PCH  */
        tdsrrcrbrc_send_initiate_cu_flag = FALSE;
      }
      
      tdsrrcrbrc_send_channel_config_req();
      /* Change the substate since we're waiting for the
         confirmation from LLC for the channel configs */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
    }
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Recvd state change %d to %d tdsrrc_rbrc state %d, No action",
               ptr->previous_state, ptr->new_state, tdsrrc_rbrc.curr_substate);
    }
    /* IF CU is waiting for RB proc to complete, no need to initate CU through RB procedure. Reset flag. */
    if (tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_RB_PROC_COMPL_IND) 
    {
      tdsrrcrbrc_send_initiate_cu_flag = FALSE;
    }
  } 
  else
  {
    /* We should not get here since we haven't registered for
    other state changes */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Unsupported state change %d to %d tdsrrc_rbrc state %d", ptr->previous_state,
             ptr->new_state, tdsrrc_rbrc.curr_substate);
  }
}

/*===========================================================================

FUNCTION   RRCRBRC_PROCESS_L2_ACK_IND

DESCRIPTION

  This function processes the received Layer 2 ACK indication.
  If the L2 ACK indicates that the RBR Complete message has
  been succesfully transmitted, the function clears TDSORDERED_CONFIG
  and goes back to the initial state.
  
  If the L2 ack indicates failure, it means that there is
  a serious failure in the air interface, so the procedure 
  sends a command to LLC to release all Radio Bearers and go
  to disconnected state.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_process_l2_ack_ind
(
tdsrrc_downlink_l2ack_ind_type *ptr /* Pointer to the L2 Ack Ind cmd */
)
{
  tdsrrc_cmd_type *cmd; /* To send a LLC command if L2 tx failed */
  tdsrrc_rlc_re_est_for_srns_rb_e_type rb_type;      /*rb type tobe reestablished */
  tdsrrc_state_e_type                  current_state;       /* current RRC State  */
  tdsrrc_CellUpdateCause       cu_cause;
  uint8                    rejected_tr_id = TDSRRCRB_INVALID_TRANSACTION_ID;

  if ( ptr->status == SUCCESS )
  {
    current_state = tdsrrc_get_state(); 

    /* Back up accepted transaction ID */
    rejected_tr_id = tdsrrc_rbrc.tr_ids.accepted_transaction;

    /* clean up trans id*/
    tdsrrc_rbrc.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;

    if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
        (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
    {

      if(current_state==TDSRRC_STATE_CELL_DCH) 
      {
        /*L2 Ack for the RBRC Complete message is received on the old configuration
        Now initiate a cell selection */
        tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
                                                TDSRRC_TRANSITION_FROM_DCH,
                                                tdsrrc_rbrc.state_indicated_by_msg);
        /*wait for the state_change indication after the cell selection
        is successfully completed */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

        /* Start T_305 [periodic cell update] timer */
        tdsrrccu_start_t_305_timer();

      }
      else if(current_state==TDSRRC_STATE_CELL_FACH) 
      {
        if (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) &&
            (cu_cause == tdsrrc_CellUpdateCause_re_enteredServiceArea || cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError ||
             cu_cause == tdsrrc_CellUpdateCause_radiolinkFailure))
        {
          /* This is a case where cell update is in progress while RBRC transitions 
             UE to Cell_PCH or URA_PCH. Though L2 Ack is gotten, send a PCR failure 
             later when CU procedure gets Cell update confirm. If we were to act on 
             L2 Ack, RRC moves to Cell_PCH/URA_PCH where cell update confirm message
             is not handled (unless CU registers on RBRC's behalf) & UE would stuck 
             in Cell_PCH forever [in the absence of reselections].
          */
          if(SUCCESS != tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBRC))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBRC procedure failed to register with CU");
          }

          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Registered for Cell Update procedure completion: cu_cause = %d",cu_cause);
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_PHY_CHAN_FAILURE;
          tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;

          /* Restore the accepted transaction ID for sending failure later */
          tdsrrc_rbrc.tr_ids.rejected_transaction = rejected_tr_id;
        }
        else
        {

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          /* In case of FACH ->PCH transition is Freq Re-direction 
           * info is present then trigger Cell Selection 
           */
          if((tdsrrc_rbrc.initiate_cell_selection_trans_from_fach == TRUE )
            &&(tdsrrccsp_check_initiate_cell_selection_handling() == TRUE))
          {
            /* Now copy OC to TOC */
            tdsrrcllcoc_update_toc_with_oc_from_fach();
            
            /*Initiate the cell selection*/
            tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
                                                   TDSRRC_TRANSITION_FROM_FACH,
                                                   tdsrrc_rbrc.state_indicated_by_msg);

        
            
            tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
          }
          else
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
             tdsrrc_rbrc.initiate_cell_selection_trans_from_fach = FALSE;
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

            /* Call the function that creates and sends a channel
               config request to LLC. */
            tdsrrcrbrc_send_channel_config_req();
            /* Change the substate since we're waiting for the
               confirmation from LLC for the channel configs */
            tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
          }
        }
      }
      else
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid combination of rrcstate%d and rrcrbrc substate%d",current_state,tdsrrc_rbrc.curr_substate);
      }
      
      /*return from here */
      return;
    }
    /* Control comes here when the procedure does not involve Cell_PCH or URA_PCH */
    if (tdsrrc_rbrc.rbrc_for_srns_relocation)
    {
      if (TDSRRC_STATE_CELL_DCH == tdsrrc_get_state())
      {
        /* Initiate the re-establishment of remaining RBs before 
        RBRC procedure goes back to initial substate. 
        No need to wait for the confirmation */
        rb_type  = TDSRRC_RE_EST_NON_RB_2_UM_AM;
        tdsrrcrb_send_rlc_re_est_req_to_smc(TDSRRC_PROCEDURE_RBRC, rb_type, FALSE);

        /*inform upper layers if necessary */
        tdsrrcrbrc_send_indication_to_upper_layers();
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"SRNS reloc flag is ON in rrc state %d",tdsrrc_get_state());
      }
    }
  } 
  
  else
  {
    /* l2ack failed. If reconfiguration involves SARNS relocation then
    go to idle. Otherwise just complete the procedure */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"L2 ack failed for RBRC Complete");

    /* Here make sure that CU is not active.  If CU is active with OOS area handling
       then NACK coming from RLC shall be ingnored as RBRC failure with cause
       CU complete should be sent to NW */
    if (TDSRRC_STATE_CELL_FACH == tdsrrc_get_state() &&
        tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) == TDSRRCCU_STARTED && 
        (cu_cause == tdsrrc_CellUpdateCause_re_enteredServiceArea || cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError ||
         cu_cause == tdsrrc_CellUpdateCause_radiolinkFailure))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NACK being ingnored as TCRC failure msg will be sent");
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
      (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBRC);
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Registered for Cell Update procedure completion: cu_cause = %d",cu_cause);
      return;
    }

    tdsrrc_rbrc.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
    if ((tdsrrc_rbrc.rbrc_for_srns_relocation) ||
        (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
        (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"L2ack failed SRNSreloc:%d,next_st:%d",tdsrrc_rbrc.rbrc_for_srns_relocation,tdsrrc_rbrc.state_indicated_by_msg);
      if ( (cmd=tdsrrc_get_int_cmd_buf())!=NULL)
      { 
        cmd->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;
        /* Need to change state */
        cmd->cmd.chan_config_req.rrc_state_change_required = TRUE;
        /* Next state is disconnected state */
        cmd->cmd.chan_config_req.next_state = TDSRRC_STATE_DISCONNECTED;
        /* We don't need a confirm for this command */
        cmd->cmd.chan_config_req.rrc_channel_config_cnf_required = FALSE;
        /* Procedure name */
        cmd->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RBRC;
        /* No need to fill any other parameter in this command.
        When LLC sees the next state is Disconnected State, it
        will tear down all the Radio Bearers. */
        tdsrrc_put_int_cmd( cmd );
      } 
      else
      {
        ERR_FATAL("No memory for chan config cmd",0,0,0);
      }
    }
  }

  /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
  configurations */
  /* Note OC is set at this point. No need to clear it
  if the next state is cell_pch or ura_pch and
  l2 ack failied because cu procedure will take care of it */
  if (((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)) &&
      (ptr->status != SUCCESS))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2 ack failed for PCH st");
    if (TDSOC_SET_FOR_DCH_FACH_TRANS == tdsrrcllc_get_ordered_config_status())
    {
      /* oc is already cleared by CU and reset for dch->fach transition 
      in this case do not clear oc */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"OC is set by cu");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"reset OC");
      if(tdsrrcllc_reset_ordered_config(TDSRRC_PROCEDURE_RBRC
                                     ,TRUE
                                     ) != TDSOC_NOT_SET)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCLLC reset ordered config failed");
      }
    }
  }
  else
  {
    tdsrrcllc_clear_ordered_config();  
  }

#if defined(FEATURE_3GPP_FAST_DORMANCY)
  if (tdsrrc_rbrc.rbrc_for_srns_relocation == TRUE)
  {
    tdsrrcdormancy_update_dormancy_for_srns_hho_cco(TDSRRC_PROCEDURE_RBRC);
  }
#endif 

  /* Go to the next sub-state in the state machine. That's
  the initial substate since we've sent the RB RC complete
  message */
  tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

  /* At this point we don't need to remember any of the
  Radio Bearer information since it's stored in ESTABLISHED_RABS.
  Clear our internal variables */
  tdsrrcrbrc_clear_procedure();
}


/*===========================================================================

FUNCTION   RRCRBRC_INITIAL_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_INITIAL substate. This function
  looks at the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_initial_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Reconfiguration Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */

    /* Process the RB Reconfig message */
    tdsrrcrbrc_process_rbrc_message( cmd_ptr );

    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RB RC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }
}



/*===========================================================================

FUNCTION   RRCRBRC_DCH_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate
  when the UE is in CELL_DCH state. This function looks at
  the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_dch_wfchcnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbrc_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Reconfig Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */

    /* Process the RB Reconfig message */

    tdsrrcrbrc_process_rbrc_message( cmd_ptr );

    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }
}


/*===========================================================================

FUNCTION   RRCRBRC_PCH_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate
  when the UE is in CELL_PCH state. This function looks at
  the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_pch_wfchcnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{


  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbrc_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:
  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }
}

/*===========================================================================

FUNCTION   tdsrrcrbrc_other_wfcucnf_substate_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate
  and the UE is in a state other than CELL_FACH or CELL_DCH. 
  This function looks at the received event and does the 
  appropriate processing of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_other_wfcucnf_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_STATE_CHANGE_IND:
      /* Indication that RRC's state has changed */
      tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;
    
    default:    
      /* No other events expected in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
      break;
  }
}

/*===========================================================================

FUNCTION   tdsrrcrbrc_other_wfcscnf_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF substate
  and the UE is in a state other than CELL_FACH or CELL_DCH. 
  This function looks at the received event and does the 
  appropriate processing of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_other_wfcscnf_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_STATE_CHANGE_IND:
      /* Indication that RRC's state has changed */
      tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;
    
    default:    
      /* No other events expected in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
      break;
  }
}

/*===========================================================================

FUNCTION   RRCRBRC_DCH_WFCUCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate
  and the UE is in CELL_DCH state. This function looks at 
  the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

static void tdsrrcrbrc_dch_wfcucnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      if(tdsrrc_rbrc.status.failure_status == TDSRRCRB_SIMULTANEOUS_RECONFIG)
      {
        /* Send the RB Reconfig Failure Message */
        tdsrrcrbrc_send_rbrc_failure_message(tdsrrc_rbrc.status.failure_status,
                                  tdsrrc_rbrc.status.prot_err_cause
                                );
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbrc_clear_procedure();

        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
      }
      else if(tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_VALID)
      {
        /* Send the RB Reconfig Complete message */
        if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
            (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

        {
          tdsrrcrbrc_send_rbrc_complete_message(TRUE);
        }
        else
        {
          tdsrrcrbrc_send_rbrc_complete_message(FALSE);
        }
        /*inform upper layers if necessary */
        tdsrrcrbrc_send_indication_to_upper_layers();

        tdsrrcllc_print_trans_chnl_rate();
        /*if next state is Cell_PCH or URA_PCH then we have to 
        treat the procedure as successful only after getting L2ack
        for the response message */
        if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
            (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

        {
          /* Change the substate to the next substate - this is
          where we wait for L2 ack from RLC for sending the RB RC
          complete message. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
          tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
        }
        else
        {
          /* clear the ordered config here
          treat the procedure as successful. */
          tdsrrcllc_clear_ordered_config();  

          /* send the nas cn info if stored */
          if (tdsrrc_rbrc.cn_info_saved != FALSE)
          {
            /* Put the command on MM queue */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RRC_SERVICE_IND_FROM_OTA_MSG to MM");
            if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
            {
              tdscfa_mm_put_cmd(tdsrrc_rbrc.mm_cmd_ptr);
            }
            mm_put_cmd(tdsrrc_rbrc.mm_cmd_ptr);
            /* clear the flag */
            tdsrrc_rbrc.cn_info_saved = FALSE;
#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER              
           /*send SRNS reallocation ind to nas with success*/
 	     if(tdsrrc_rbrc.srns_reallocation_trigger == TRUE)
 	     {
 		 tdsrrc_rbrc.srns_reallocation_trigger = FALSE;
 	     }
#endif         
          }

          /* Go to the next sub-state in the state machine. That's
          the initial substate since we've sent the RB RC complete
          message */
          tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

          /* At this point we don't need to remember any of the
          Radio Bearer information since it's stored in ESTABLISHED_RABS.
          Clear our internal variables */
          tdsrrcrbrc_clear_procedure();
        }
      }
      /*  In the case of Phy_Chan_Failure, RB setup failure must be send
      and oc should be reset. */
      else if(tdsrrc_rbrc.status.failure_status == TDSRRCRB_PHY_CHAN_FAILURE)
      {
        
        /* Send the RB Setup Failure Message */
        tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                  tdsrrc_rbrc.status.prot_err_cause
                                );

        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbrc_clear_procedure();
        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure status %d not handled",tdsrrc_rbrc.status.failure_status);
      }

    break;

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbrc_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Setup Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Reconfig message */
    tdsrrcrbrc_process_rbrc_message( cmd_ptr );
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }

}



/*===========================================================================

FUNCTION   RRCRBRC_FACH_WFCUCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate
  and the UE is in CELL_FACH state. This function looks at 
  the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

static void tdsrrcrbrc_fach_wfcucnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      if(tdsrrc_rbrc.status.failure_status == TDSRRCRB_SIMULTANEOUS_RECONFIG)
      {
        /* Send the RB Reconfig Failure Message */
        tdsrrcrbrc_send_rbrc_failure_message(tdsrrc_rbrc.status.failure_status,
                                  tdsrrc_rbrc.status.prot_err_cause
                                );
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbrc_clear_procedure();

        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
      }
      else if(tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_VALID)
      {
        if ((tdsrrcllc_get_toc_usage() == TDSTOC_FOR_OOS))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TOC usage TOC_FOR_OOS cu cause ");
					
          /* Call the function that creates and sends the  channel
             config information to be requested from LLC. */
          tdsrrcrbrc_send_channel_config_req();
  
          /* Change the substate since we're waiting for the
             confirmation from LLC for the channel configs */
          tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
          return;
        }

        /* Send the RB Reconfig Complete message */
        if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
            (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

        {
          tdsrrcrbrc_send_rbrc_complete_message(TRUE);
        }
        else
        {
          tdsrrcrbrc_send_rbrc_complete_message(FALSE);
        }
        /*inform upper layers if necessary */
        tdsrrcrbrc_send_indication_to_upper_layers();

        tdsrrcllc_print_trans_chnl_rate();
        /*if next state is Cell_PCH or URA_PCH then we have to 
        treat the procedure as successful only after getting L2ack
        for the response message */
        if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
            (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
        {
          /* Change the substate to the next substate - this is
          where we wait for L2 ack from RLC for sending the RB RC
          complete message. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
          tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
        }
        else
        {
          /* clear the ordered config here
          treat the procedure as successful. */
          tdsrrcllc_clear_ordered_config();  

          /* send the nas cn info if stored */
          if (tdsrrc_rbrc.cn_info_saved != FALSE)
          {
            /* Put the command on MM queue */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RRC_SERVICE_IND_FROM_OTA_MSG to MM");
            if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
            {
              tdscfa_mm_put_cmd(tdsrrc_rbrc.mm_cmd_ptr);
            }
            mm_put_cmd(tdsrrc_rbrc.mm_cmd_ptr);
            /* clear the flag */
            tdsrrc_rbrc.cn_info_saved = FALSE;
#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER              
	    /*send SRNS reallocation ind to nas with success*/
	    if(tdsrrc_rbrc.srns_reallocation_trigger == TRUE)
	    {
		 tdsrrc_rbrc.srns_reallocation_trigger = FALSE;
	    }
#endif        
          }

          /* Go to the next sub-state in the state machine. That's
          the initial substate since we've sent the RB RC complete
          message */
          tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

          /* At this point we don't need to remember any of the
          Radio Bearer information since it's stored in ESTABLISHED_RABS.
          Clear our internal variables */
          tdsrrcrbrc_clear_procedure();
        }
      }
      /*  In the case of Phy_Chan_Failure, RB setup failure must be send
      and oc should be reset. */
      else if(tdsrrc_rbrc.status.failure_status == TDSRRCRB_PHY_CHAN_FAILURE)
      {
        
        /* Send the RB Setup Failure Message */
        tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                  tdsrrc_rbrc.status.prot_err_cause
                                );

        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbrc_clear_procedure();
        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure status %d not handled",tdsrrc_rbrc.status.failure_status);
      }

    break;

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbrc_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Setup Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Reconfig message */
    tdsrrcrbrc_process_rbrc_message( cmd_ptr );
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }

}

/*===========================================================================

FUNCTION   rrcrbrc_FACH_WF_CSCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate
  and the UE is in CELL_FACH state. This function looks at 
  the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

static void tdsrrcrbrc_fach_wfcscnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_STATE_CHANGE_IND:
   /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:
    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Setup Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RBRC Setup message */
    tdsrrcrbrc_process_rbrc_message( cmd_ptr );
    break;
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU

  case TDSRRC_NEW_CELL_IND:
    tdsrrcrbrc_process_new_cell_ind( &(cmd_ptr->cmd.new_cell_ind) );
    break;
   /*In case of FACH->FACH OOS, CCM will not send a NEW_CELL_IND to procedure,
   * But CU will be informed, and CU will inform the procedure after the Cell
   * Update Procedure is complete*/
  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    if(tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH)
    {
      /* if OOS was detected during trans from DCH, cell update
         procedure has completed.  send channel config req to 
         complete the reconfiguration procedure
      */
      if ((tdsrrcllc_get_toc_usage() ==TDSTOC_FOR_OOS) 
          || (tdsrrcllc_get_toc_usage() ==TDSTOC_FOR_OOS_WITHOUT_DCH_INFO)
          ||(tdsrrcllc_get_toc_usage() ==TDSTOC_FOR_OOS_WITH_DCH_INFO)
          )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
        /* Call the function that creates and sends the  channel
        config information to be requested from LLC. */
        tdsrrcrbrc_send_channel_config_req();
        /* Also reset the tdsrrcrbrc_send_initiate_cu_flag as cell update
         * will not be needed since CU and CUCNF for oos took care 
         * of that
         */
        tdsrrcrbrc_send_initiate_cu_flag = FALSE;
  
        /* Change the substate since we're waiting for the
        confirmation from LLC for the channel configs */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Expected Cell Update Cnf command in Wait for Cell Selection state only in case of OOS");
      }
    }    
    else
    {
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"Expected Cell Update Cnf in Cell Selection cnf state only in FACH->FACh case");
    }
    break;
#endif
    default:
      /* No other events expected in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
      break;
  }

}

/*===========================================================================

FUNCTION   RRCRBRC_FACH_WFCRNTIUPDATECNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF substate
  and the UE is in CELL_FACH state. This function looks at 
  the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

static void tdsrrcrbrc_fach_wfcrntiupdatecnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_STATE_CHANGE_IND:
   /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
   
    break;


  case TDSRRC_RNTI_UPDATE_CNF:
    if (cmd_ptr->cmd.rnti_update_cnf.status == SUCCESS) 
    {
      tdsrrcrbrc_process_rnti_update_cnf();
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RNTI_UPDATE failed.  Send RBRC failure");
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_VALID;
      
      tdsrrcrbrc_send_rbrc_failure_message(tdsrrc_rbrc.status.failure_status,
                                tdsrrc_rbrc.status.prot_err_cause);
      
      /* Then clear all local variables that may
         need to be cleared */
      tdsrrcrbrc_clear_procedure();

      /* Make sure we're back in the initial sub-state */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    }
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }

}

/*===========================================================================

FUNCTION   RRCRBRC_PCH_WFCUCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate
  and the UE is in CELL_PCH state. This function looks at 
  the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_pch_wfcucnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    tdsrrcrbrc_process_successful_proc_completion(FALSE);
    break;

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbrc_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:
    /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

 

    case TDSRRC_DOWNLINK_SDU_IND:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Unexpected DOWNLINK_SDU_IND in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }

}

/*===========================================================================

FUNCTION   RRCRBRC_PCH_WF_CSCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF substate
  and the UE is in CELL_PCH state. This function looks at 
  the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

static void tdsrrcrbrc_pch_wfcscnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_STATE_CHANGE_IND:
   /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
   
    break;


  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }

}

/*===========================================================================

FUNCTION   RRCRBRC_FACH_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate
  and the UE is in CELL_FACH state. This function looks at 
  the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_fach_wfchcnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbrc_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Reconfig Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Reconfig message */
     tdsrrcrbrc_process_rbrc_message( cmd_ptr );

    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }
}



/*===========================================================================

FUNCTION   RRCRBRC_OTHER_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate
  and the UE is in any other state except CELL_DCH and CELL_FACH. 
  This function looks at the received event and does the appropriate
  processing of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_other_wfchcnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring chan config cnf since UE not in DCH or FACH");
    /* In this state this procedure should no longer be active.*/

    /* Then change to initial substate */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbrc_clear_procedure();
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring downlink sdu since UE not in DCH or FACH");
    /* In this state this procedure should no longer be active.*/

    /* Then change to initial substate */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbrc_clear_procedure();
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RB RC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }
}


/*===========================================================================

FUNCTION   RRCRBRC_DCH_WFSRB2REESTABLISH_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_REESTABLISH_SRB2 substate
  and the UE is in CELL_DCH state. This function looks at the 
  received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_dch_wfsrb2reestablish_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_RE_EST_RLC_FOR_SRNS_CNF:

    /* Confirmation from SMC of RLC re-establishment confirm */
    tdsrrcrbrc_process_rlc_reestablishment_cnf( );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Reconfig Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Reconfig message */
    tdsrrcrbrc_process_rbrc_message( cmd_ptr );

    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }
}


/*===========================================================================

FUNCTION   RRCRBRC_DCH_WFL2ACK_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_L2_ACK substate
  and the UE is in CELL_DCH state. This function looks at the 
  received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_dch_wfl2ack_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_DOWNLINK_L2ACK_IND:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbrc_process_l2_ack_ind( &(cmd_ptr->cmd.downlink_l2ack_ind) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Reconfig Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Reconfig message */
    tdsrrcrbrc_process_rbrc_message( cmd_ptr );

    break;
  
  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    tdsrrc_rbrc.status.failure_status = TDSRRCRB_CELL_UPDATE_OCCURED;
    /* Send the PC Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message(tdsrrc_rbrc.status.failure_status,
                              tdsrrc_rbrc.status.prot_err_cause
                              );

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbrc_clear_procedure();

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }
}


/*===========================================================================

FUNCTION   RRCRBRC_FACH_WFL2ACK_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_L2_ACK substate
  and the UE is in CELL_FACH state. This function looks at the 
  received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_fach_wfl2ack_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{


  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_DOWNLINK_L2ACK_IND:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbrc_process_l2_ack_ind( &(cmd_ptr->cmd.downlink_l2ack_ind) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Reconfig Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Reconfig message */
    tdsrrcrbrc_process_rbrc_message( cmd_ptr );

    break;

  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    tdsrrc_rbrc.status.failure_status = TDSRRCRB_CELL_UPDATE_OCCURED;
    /* Send the PC Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message(tdsrrc_rbrc.status.failure_status,
                              tdsrrc_rbrc.status.prot_err_cause
                              );

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbrc_clear_procedure();

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }
}



/*===========================================================================

FUNCTION   RRCRBRC_OTHER_WFL2ACK_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBRC procedure is in the TDSRRCRB_WAIT_FOR_L2_ACK substate
  and the UE is in any other state except CELL_DCH and CELL_FACH. 
  This function looks at the received event and does the appropriate
  processing of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_other_wfl2ack_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_DOWNLINK_L2ACK_IND:

    /* Confirmation from LLC for the channel configurations */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring L2 ack since UE not in DCH or FACH");
    /* In this state this procedure should no longer be active.*/

    /* Then change to initial substate */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbrc_clear_procedure();
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC.*/
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring downlink SDU since UE not in DCH or FACH");
    /* In this state this procedure should no longer be active.*/

    /* Then change to initial substate */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbrc_clear_procedure();
    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RB RC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }
}





/*===========================================================================

FUNCTION   RRCRBRC_CELL_DCH_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when RRC
  is in CELL_DCH state. This module checks the current sub-state of
  this procedure and calls the appropriate sub-state function to
  process the received event.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_cell_dch_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  if ((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_L2ACK_IND) &&
      (tdsrrc_rbrc.curr_substate != TDSRRCRB_WAIT_FOR_L2_ACK))
  {
    /* We could get L2ack for a previously sent message while we are already
    processing the next message. So l2ack could be received any any of the following
    substates. If tdsrrc_rbrc substate does not indicate that we are not waiting for l2ack
    then just print the status */
    if (cmd_ptr->cmd.downlink_l2ack_ind.status == SUCCESS)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Recd L2ack for MUI:%d, Lc_Id:%d, rbrc_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui, 
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_rbrc.curr_substate);
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2ack failed for MUI:%d, Lc_Id:%d, rbrc_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui, 
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_rbrc.curr_substate);
    }
    /* return from here so that procedure can continue with existing process */
    return;
  }
  /* Switch on the current substate */
  switch ( tdsrrc_rbrc.curr_substate )
  {
  case TDSRRCRB_INITIAL:

    /* Call the event handler for TDSRRCRB_INITIAL substate */
    tdsrrcrbrc_initial_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:

    /* Call the event handler for TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate */
    tdsrrcrbrc_dch_wfchcnf_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_L2_ACK:

    /* Call the event handler for TDSRRCRB_WAIT_FOR_L2_ACK substate */
    tdsrrcrbrc_dch_wfl2ack_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_REESTABLISH_SRB2:

    /* Call the event handler for TDSRRCRB_WAIT_FOR_REESTABLISH_SRB2 substate */
    tdsrrcrbrc_dch_wfsrb2reestablish_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
    /* Call the event handler for TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate */
    tdsrrcrbrc_dch_wfcucnf_event_handler( cmd_ptr);
    break;

  default:

    /* No other substates processed for this RRC state    */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Command %x not processed in CELL_DCH for substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;

  }
}



/*===========================================================================

FUNCTION   RRCRBRC_CELL_FACH_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when RRC
  is in CELL_FACH state. This module checks the current sub-state of
  this procedure and calls the appropriate sub-state function to
  process the received event.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_cell_fach_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  if ((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_L2ACK_IND) &&
      (tdsrrc_rbrc.curr_substate != TDSRRCRB_WAIT_FOR_L2_ACK))
  {
    /* We could get L2ack for a previously sent message while we are already
    processing the next message. So l2ack could be received any any of the following
    substates. If tdsrrc_rbrc substate does not indicate that we are not waiting for l2ack
    then just print the status */
    if (cmd_ptr->cmd.downlink_l2ack_ind.status == SUCCESS)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Recd L2ack for MUI:%d, Lc_Id:%d, rbrc_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui, 
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_rbrc.curr_substate);
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2ack failed for MUI:%d, Lc_Id:%d, rbrc_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui, 
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_rbrc.curr_substate);
    }
    /* return from here so that procedure can continue with existing process */
    return;
  }
  /* Switch on the current substate */
  switch ( tdsrrc_rbrc.curr_substate )
  {
  case TDSRRCRB_INITIAL:

    /* Call the event handler for TDSRRCRB_INITIAL substate */
    /* NOTE: Here we need to check if a cell-update procedure
    has been initialized or not. For now, just call the event handler */
    tdsrrcrbrc_initial_substate_event_handler( cmd_ptr );
    break;
   
  case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
    /* Call the event handler for TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate */
    tdsrrcrbrc_fach_wfcucnf_event_handler( cmd_ptr);
    break;

  case TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF:

    tdsrrcrbrc_fach_wfcscnf_event_handler( cmd_ptr);
    break;

  case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:

    /* Call the event handler for TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate */
    tdsrrcrbrc_fach_wfchcnf_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF:

      /* Call the event handler for TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF substate */
      tdsrrcrbrc_fach_wfcrntiupdatecnf_substate_event_handler( cmd_ptr );
      break;

  case TDSRRCRB_WAIT_FOR_L2_ACK:

    /* Call the event handler for TDSRRCRB_WAIT_FOR_L2_ACK substate */
    tdsrrcrbrc_fach_wfl2ack_substate_event_handler( cmd_ptr );
    break;

  default:

    /* No other substates processed for this RRC state    */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Command %x not processed in CELL_FACH for substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }
}


/*===========================================================================

FUNCTION   RRCRBRC_PCH_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when RRC
  is in CELL_PCH/URA_PCH state. This module checks the current sub-state of
  this procedure and calls the appropriate sub-state function to
  process the received event.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_pch_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the current substate */
  switch ( tdsrrc_rbrc.curr_substate )
  {
  case TDSRRCRB_INITIAL:

    /* Call the event handler for TDSRRCRB_INITIAL substate */
    tdsrrcrbrc_initial_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
    /* Call the event handler for TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate */
    tdsrrcrbrc_pch_wfcucnf_event_handler( cmd_ptr);
    break;

  case TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF:

    tdsrrcrbrc_pch_wfcscnf_event_handler( cmd_ptr);
    break;

  case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:

    /* Call the event handler for TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate */
    tdsrrcrbrc_pch_wfchcnf_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_L2_ACK:

    /* The only way we could be in pch and rb state is "wait_for_l2_ack is if
       oos happened while waiting for l2 ack and cell update confirm takes UE to pch
       In this case, send failure msg of tdsrrc_rbrc with cause cell update occured 
     */
    tdsrrc_rbrc.status.failure_status = TDSRRCRB_CELL_UPDATE_OCCURED;

    tdsrrcrbrc_send_rbrc_failure_message(tdsrrc_rbrc.status.failure_status,
                              tdsrrc_rbrc.status.prot_err_cause);

    /* Then clear all local variables that may
       need to be cleared */
    tdsrrcrbrc_clear_procedure();

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    break;

  default:

    /* No other substates processed for this RRC state    */
    TDSRRC_MSG3(MSG_LEGACY_MED,"Command %x not processed in substate%d rrc_state%d", 
            cmd_ptr->cmd_hdr.cmd_id, tdsrrc_rbrc.curr_substate, tdsrrc_get_state());
    break;

  }
}

/*===========================================================================

FUNCTION   RRCRBRC_OTHER_STATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when RRC
  is in any other state except CELL_FACH and CELL_DCH. This module 
  checks the current sub-state of this procedure and calls the
  appropriate sub-state function to process the received event.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbrc_other_state_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the current substate */
  switch ( tdsrrc_rbrc.curr_substate )
  {
  
    case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:
  
      /* Call the event handler for WAIT_FOR_CHAN_CFG_CNF substate */
      tdsrrcrbrc_other_wfchcnf_substate_event_handler( cmd_ptr );
      break;
  
    case TDSRRCRB_WAIT_FOR_L2_ACK:
  
      /* Call the event handler for WAIT_FOR_L2_ACK substate */
      tdsrrcrbrc_other_wfl2ack_substate_event_handler( cmd_ptr );
      break;
  
    case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
      tdsrrcrbrc_other_wfcucnf_substate_event_handler( cmd_ptr);
      break;

    case TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF:
      tdsrrcrbrc_other_wfcscnf_event_handler( cmd_ptr);
      break;

  default:

    /* No other substates processed for this RRC state    */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Command %x not processed in IDLE or PCH states for substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbrc.curr_substate);
    break;
  }
}

/*===========================================================================

FUNCTION RRCRBRC_PROCEDURE_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  Radio Bearer Reconfiguration procedure. This function is called by the
  dispatcher whenever an event is received for this module. This function
  checks the current RRC state and calls the event handler for
  that state. 
  
  Note that the Radio Bearer Reconfiguration procedure can be initiated
  only in the CELL_FACH and CELL_DCH states. However, while the
  procedure is active, the RRC state may change to any of the other
  states. Hence all RRC states are checked by this module.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
void tdsrrcrbrc_procedure_event_handler
(
tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{

  /* Get the current RRC State */
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  switch (rrc_state)
  {
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rcevd rbrc message in rrc_state %d",rrc_state);
  case TDSRRC_STATE_CELL_FACH:    
    /* If it's CELL_FACH state, call the event handler
    for CELL_FACH state */
    tdsrrcrbrc_cell_fach_event_handler(cmd_ptr);
    break;

  case TDSRRC_STATE_CELL_DCH:    
    /* If it's CELL_DCH state, call the event handler
    for CELL_DCH state */
    tdsrrcrbrc_cell_dch_event_handler(cmd_ptr);
    break;

    case TDSRRC_STATE_URA_PCH:
    case TDSRRC_STATE_CELL_PCH:
      /* If it's CELL_DCH state, call the event handler
      for CELL_PCH state */
      tdsrrcrbrc_pch_event_handler(cmd_ptr);
      break;

    case TDSRRC_STATE_CONNECTING:   
    case TDSRRC_STATE_DISCONNECTED: 
    /* If it's any other state call the event handler
    for other states. The processing of events in
    other RRC states is identical for this procedure */
    tdsrrcrbrc_other_state_event_handler(cmd_ptr);
    break;

  default:  
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC State: %d",rrc_state);
    break;
  }  
}/* end function tdsrrcrbrc_procedure_event_handler */


/*===========================================================================

FUNCTION tdsrrcrbrc_build_rbrc_complete_message

DESCRIPTION
  This function builds the RBRC Complete Message.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcrbrc_build_rbrc_complete_message
(
tdsrrc_RRC_TransactionIdentifier trans_id,     /* Transaction Id */
tdsrrc_UL_DCCH_Message *msg_ptr,               /* Pointer to Uplink DCCH Msg */
boolean              srns_reloc,            /* Indicates whether SRNS reloc is involved */ 
boolean              cipher_update_required /* Indicates whether TM ciphering is present */
)
{                                         

  /* Select the message as RBRC Complete message */
  msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_radioBearerReconfigurationComplete;

  /* Initialize the bit mask not to include the optional fields */
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.radioBearerReconfigurationComplete);

  if (srns_reloc == TRUE)
  {
    tdsrrcrbrc_append_start_list_to_rbrc_complete(msg_ptr);
  }

  if((cipher_update_required == TRUE) && (TRUE == tdsrrcsmc_tm_ciph_active()))
  {
    if(! (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.radioBearerReconfigurationComplete,
           tdsrrc_RadioBearerReconfigurationComplete,ul_CounterSynchronisationInfo)))
    {
      tdsrrcrbrc_append_start_list_to_rbrc_complete(msg_ptr);
    }
  
    //Set the bitmask for count_c_act_time and copy the same..
    TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.radioBearerReconfigurationComplete,
      tdsrrc_RadioBearerReconfigurationComplete,count_C_ActivationTime);
    
    msg_ptr->message.u.radioBearerReconfigurationComplete.count_C_ActivationTime =
      (tdsrrc_ActivationTime)tdsrrcsmc_step_2_cipher_config_for_hho();
  
    //Send step 2 config for HHO
    tdsrrc_rbrc.hho_ciph_status = FALSE;      
  }
  
  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
 #ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
    if(tdsrrcsib_is_sib11_11bis_12_read_deferred(TDSDEFERRED_ANY_ALLOWED_SIB))
    {
      msg_ptr->message.u.radioBearerReconfigurationComplete.m.laterNonCriticalExtensionsPresent=1;
 
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.radioBearerReconfigurationComplete.laterNonCriticalExtensions);
 
      msg_ptr->message.u.radioBearerReconfigurationComplete.
	  	laterNonCriticalExtensions.m.v770NonCriticalExtensionsPresent=1;
 
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.radioBearerReconfigurationComplete.
	  	laterNonCriticalExtensions.v770NonCriticalExtensions);
 
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.radioBearerReconfigurationComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        radioBearerReconfigurationComplete_v770ext);
 
      msg_ptr->message.u.radioBearerReconfigurationComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        radioBearerReconfigurationComplete_v770ext.m.deferredMeasurementControlReadingPresent=1;
      
      msg_ptr->message.u.radioBearerReconfigurationComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        radioBearerReconfigurationComplete_v770ext.deferredMeasurementControlReading = 
        tdsrrc_RadioBearerReconfigurationComplete_v770ext_IEs_deferredMeasurementControlReading_true_;
    }
#endif
  }
  /* Initialize the Transaction Id */
  msg_ptr->message.u.radioBearerReconfigurationComplete.
  rrc_TransactionIdentifier = trans_id;

} /* tdsrrcrbrc_build_rbrc_complete_message */




/*===========================================================================

FUNCTION  RRCRBRC_INIT_PROCEDURE

DESCRIPTION

  This function initializes the RBRC procedure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbrc_init_procedure( void )
{
  uint32 count;

  tdsrrcrbrc_send_initiate_cu_flag = FALSE;

  /* Initialize the transaction id's for this procedure.
  Use a number that can't be an actual received transaction
  id. */
  tdsrrc_rbrc.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  tdsrrc_rbrc.tr_ids.rejected_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;

  /* Initialize the current substate */
  tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
 
  /* Initialize New CRNTI valid to FALSE in the begining */
  tdsrrc_rbrc.new_crnti_valid = FALSE;
  tdsrrc_rbrc.directed_cell_info = FALSE;

  /* set cn_info_saved to false */
  tdsrrc_rbrc.cn_info_saved = FALSE;

  /* Indicate that there are no RBs to be released and
  no rbs tobe affected. This is the default value. */
  tdsrrc_rbrc.rbs_tobe_reconfigured.is_valid = FALSE;
  tdsrrc_rbrc.rbs_tobe_affected.is_valid     = FALSE;

  /*clear the SRNS relocation flag */
  tdsrrc_rbrc.rbrc_for_srns_relocation = FALSE;

  tdsrrc_rbrc.hho_ciph_status = FALSE;

  /* Initialize the RABS per message structure */
  for ( count=0; count < TDSRRCRB_MAX_RAB_PER_MSG; count++ )
  {
    tdsrrc_rbrc.rabs.rabs_in_msg[count].num_rbs_for_rab = 0;
    tdsrrc_rbrc.rabs.rabs_in_msg[count].rab_type = TDSRRCRB_UNKNOWN_RAB;    
    tdsrrc_rbrc.rabs.chan_config_needed[count] = FALSE;
    tdsrrc_rbrc.rabs.rabs_in_msg[count].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_NONE;

  }
  tdsrrc_rbrc.rabs.num_rabs_in_msg = 0;

  tdsrrc_rbrc.high_mobility_ind = FALSE;

  /* Register for state change notification -
  We want to be notified when the state changes
  from CELL_DCH to CELL_FACH and from any state
  to disconnected state. */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBRC,  /* Procedure name */
                             TDSRRC_STATE_CELL_DCH, /* From State     */
                             TDSRRC_STATE_CELL_FACH /* To State       */
                           );
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBRC,  /* Procedure name */
                             TDSRRC_STATE_CELL_DCH, /* From State     */
                             TDSRRC_STATE_CELL_PCH /* To State       */
                           );
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBRC,  /* Procedure name */
                             TDSRRC_STATE_CELL_DCH, /* From State     */
                             TDSRRC_STATE_URA_PCH /* To State       */
                           );
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBRC,  /* Procedure name */
                             TDSRRC_STATE_WILDCARD, /* From State     */
                             TDSRRC_STATE_DISCONNECTED /* To State    */
                           );
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU

  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBRC,  /* Procedure name */
                             TDSRRC_STATE_CELL_FACH, /* From State     */
                             TDSRRC_STATE_CELL_PCH /* To State       */
                           );

  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBRC,  /* Procedure name */
                             TDSRRC_STATE_CELL_FACH, /* From State     */
                             TDSRRC_STATE_URA_PCH /* To State       */
                           );
  tdsrrc_rbrc.initiate_cell_selection_trans_from_fach = FALSE;     
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/


  /* Define a timer used to delay 5 seconds before
  sending the RB Reconfig Complete message */
  rex_def_timer(&tdsrrcrbrc_temp_timer, rex_self(), TDSRRCRBRC_TEMP_TIMER_SIG);
}


// #endif
/*===========================================================================

FUNCTION   PROCESS_RBRC_MESSAGE_R4

DESCRIPTION

  This function processes the received RB Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Reconfig Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Reconfig Failure message
    will be sent to UTRAN.

    The function is copy from tdsrrcrbrc_process_rbrc_message_r5,but handle RB is from tdsrrcrbrc_process_rbrc_message
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbrc_process_rbrc_message_r4
(
  tdsrrc_cmd_type *cmd_ptr
)
{

  tdsrrc_DL_DCCH_Message                    *dcch_msg_ptr;      /* Pointer to the RB Reconfig message */
  tdsrrc_RadioBearerReconfiguration         *msg_ptr; /* Pointer to the RB Reconfig message        */
  tdsrrc_CN_InformationInfo                 *cn_info_ptr = NULL; 
  uint32                                  localrb_count;
  tdsrrcrbrc_rlc_stop_cont_e_type            rlc_stop_cont_e_value = TDSRB_STOP_CONTINUE_ABSENT;
  boolean                                 is_valid_rbid = TRUE;
  tdsrrc_RB_Identity invalid_rbid = 0xff;
  rlc_lc_id_type                          lc_id;
  uecomdef_logch_mode_e_type              lc_mode;

  tdsrrc_msg_ver_enum_type                   msg_version = TDSMSG_REL4;
  tdsrrc_RadioBearerReconfiguration_r4_IEs  *rbrc_ptr = NULL;
  tdsrrc_RB_InformationReconfig_r4  *rb_info_reconfig_ptr = NULL;
  tdsrrc_RB_InformationAffectedList  *rb_info_affected_list_r4_ptr = NULL;

   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  boolean primary_plmn_Id_present = FALSE;
   /*Variable to hold primary plmn i.d if present in OTA msg*/
  tdsrrc_PLMN_Identity primary_plmn_Identity;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Recvd RBRC R4 Msg sub_st%d",tdsrrc_rbrc.curr_substate);
  
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Reconfig message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerReconfiguration);

  if (FAILURE == 
      tdsrrcrbrc_check_and_send_failure_for_simul_reconfig(
      msg_ptr->u.later_than_r3.rrc_TransactionIdentifier))
  {
    return;
  }

/* Store Msg Ptr in a local variable */
  rbrc_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4;

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rbrc_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rbrc_ptr->m.activationTimePresent,
         rbrc_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBRC SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbrc_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rbrc_ptr->m.activationTimePresent = 0;
    }
  }

#endif


  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  tdsrrcrbrc_validate_rbrc_message_r4( msg_ptr );

  if ( tdsrrc_rbrc.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    /* Send the RB Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                               tdsrrc_rbrc.status.prot_err_cause
                             );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    tdsrrcrbrc_clear_procedure();
    return;
  }

  /* If the message is valid, proceed further */
  /* Check if there is CN information present. If so, we need
  to send the information to the Cell Change manager */
  
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,
  	tdsrrc_RadioBearerReconfiguration_r4_IEs,cn_InformationInfo))
  {
    cn_info_ptr = &(rbrc_ptr->cn_InformationInfo);
  }

  /* CN info presents. */
  if (cn_info_ptr != NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
    
      /*Check to see if primary PLMN I.D is present in RBRC message ext
       * tdsrrc_RadioBearerReconfiguration-v690ext which is present in 
       * v690NonCriticalExtensions
       */
       primary_plmn_Id_present = tdsrrcrbrc_check_and_get_primary_plmn_id(msg_ptr,
                                                                  &primary_plmn_Identity,TDSMSG_REL5);
    
    tdsrrc_rbrc.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info(
                     cn_info_ptr
                     ,primary_plmn_Id_present
                     ,&primary_plmn_Identity
                );
    if (tdsrrc_rbrc.mm_cmd_ptr == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
    }
    else
    {
      /* set cn_info_saved flag */
      tdsrrc_rbrc.cn_info_saved = TRUE;
    }
  }

  tdsrrcrbrc_process_high_mobility_ind_in_non_critical_ext(msg_ptr,TDSMSG_REL4); 

  /* Copy handle from R3 begin */
  /* Process madatory RB information to reconfigure */
  if ( msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4.
         rb_InformationReconfigList.n != 0 )
  {
    /*store the rb ids tobe released */
    tdsrrc_rbrc.rbs_tobe_reconfigured.is_valid = TRUE;

    localrb_count = 0;
    while (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4
    	              .rb_InformationReconfigList.n > localrb_count)
    {
       rb_info_reconfig_ptr = 
       &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4.
                    rb_InformationReconfigList.elem[localrb_count];
      /*First check whether this rb-id exists in established_rab variable or not */
      if (! tdsrrcrbrc_validate_rb_id(rb_info_reconfig_ptr->rb_Identity))
      {
        /*rab-id does not exist in established_rab*/
        tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        is_valid_rbid = FALSE;
        invalid_rbid = rb_info_reconfig_ptr->rb_Identity;
        /*one of the rab-id does not exist in established_rab*/
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"rb-id %d is not in established_rab",invalid_rbid);
        /*Quit the loop*/
        break;
      }
      
      // Initialize the enum to this value.
      rlc_stop_cont_e_value = TDSRB_STOP_CONTINUE_ABSENT;

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
               tdsrrc_RB_InformationReconfig,rb_StopContinue))
      {
        if ((rb_info_reconfig_ptr->rb_Identity) <= 2)
        {
          /*rb-id does not exist in established_rab*/
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          is_valid_rbid = FALSE;
          invalid_rbid = rb_info_reconfig_ptr->rb_Identity;
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"RB ID %d can't be stopped/continued",invalid_rbid);
          /*Quit the loop*/
          break;
        }
        
        lc_id = tdsrrclcm_get_ul_rlc_lc_id(rb_info_reconfig_ptr->rb_Identity);

        if (lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
        {
          /*rb-id does not exist in established_rab*/
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          is_valid_rbid = FALSE;
          invalid_rbid = rb_info_reconfig_ptr->rb_Identity;
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"No corresponding LC id for the RB %d",invalid_rbid);
          /*Quit the loop*/
          break;
        }
        
        if (tdsrrclcm_get_ul_rlc_mode(lc_id, &lc_mode) == TDSRRCLCM_FAILURE)
        {
          /*rb-id does not exist in established_rab*/
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          is_valid_rbid = FALSE;
          invalid_rbid = rb_info_reconfig_ptr->rb_Identity;
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid lc_mode for RB %d",invalid_rbid);
          /*Quit the loop*/
          break;
        }

        if (rb_info_reconfig_ptr->rb_StopContinue == tdsrrc_RB_StopContinue_stopRB)
        {
          if (lc_mode == UE_MODE_TRANSPARENT)
          {
            // In case of transparent mode RB's just print an error message if stop/continue
            // is specified. Do not reject the rbreconfig message because of this.
            invalid_rbid = rb_info_reconfig_ptr->rb_Identity;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Transparent mode RB %d can't be stopped/contd",invalid_rbid);
          }
          else
          {
            rlc_stop_cont_e_value = TDSRB_STOP;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Stop recvd for RB %d",rb_info_reconfig_ptr->rb_Identity);
          }
          
        }
        else if (rb_info_reconfig_ptr->rb_StopContinue == tdsrrc_RB_StopContinue_continueRB)
        {
          if (lc_mode == UE_MODE_TRANSPARENT)
          {
            // In case of transparent mode RB's just print an error message if stop/continue
            // is specified. Do not reject the rbreconfig message because of this.
            invalid_rbid = rb_info_reconfig_ptr->rb_Identity;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Transparent mode RB %d can't be stopped/contd",invalid_rbid);
          }
          else
          {
            rlc_stop_cont_e_value = TDSRB_CONTINUE;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Continue recvd for RB %d",rb_info_reconfig_ptr->rb_Identity);
          }
        }
      }
      
      tdsrrc_rbrc.rbs_tobe_reconfigured.rb_id[localrb_count] = 
          rb_info_reconfig_ptr->rb_Identity;
      tdsrrc_rbrc.rbs_tobe_reconfigured.rb_stop_continue[localrb_count] = rlc_stop_cont_e_value;

      tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] = tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] = 0;
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
            tdsrrc_RB_InformationReconfig,rlc_Info))&&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,
        tdsrrc_RLC_Info,ul_RLC_Mode)))
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSMODIFY_RB_CFG;
      }
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
             tdsrrc_RB_InformationReconfig,rlc_Info))&&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,
        tdsrrc_RLC_Info,dl_RLC_Mode)))
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSMODIFY_RB_CFG;
      }
      if(rlc_stop_cont_e_value == TDSRB_STOP)
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSSTOP_RB_CFG;
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSSTOP_RB_CFG;
      }
      else if(rlc_stop_cont_e_value == TDSRB_CONTINUE)
      {
        tdsrrc_rbrc.rbs_tobe_reconfigured.ul_cfg_mask[localrb_count] |= TDSCONTINUE_RB_CFG;
        tdsrrc_rbrc.rbs_tobe_reconfigured.dl_cfg_mask[localrb_count] |= TDSCONTINUE_RB_CFG;
      }
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"LocalRBCnt %d, rb_id %d, rlc_stop_cont %d", localrb_count,
               (rb_info_reconfig_ptr->rb_Identity), rlc_stop_cont_e_value);
      localrb_count++;
    }
    tdsrrc_rbrc.rbs_tobe_reconfigured.num_rbs = localrb_count;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Num of RBs tobe reconfigured:%d",localrb_count);
  }
  /* Copy handle from R3 end */
  
  /* Continue further if there are no invalid Rbids in recnfig list */
  if (is_valid_rbid)
  {
    /*Store the rb ids tobe affected */
    /*lint -e514*/
    if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4,
       tdsrrc_RadioBearerReconfiguration_r4_IEs,rb_InformationAffectedList)) 
        
      /*lint +e514*/
    {
      tdsrrc_rbrc.rbs_tobe_affected.is_valid = TRUE;
      rb_info_affected_list_r4_ptr = 
      &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4.rb_InformationAffectedList;
      localrb_count = 0;
      while ( rb_info_affected_list_r4_ptr->n > localrb_count )
      {
        /*first check whether this rb-id exists in established_rab variable or not */
        if (! tdsrrcrbrc_validate_rb_id (rb_info_affected_list_r4_ptr->elem[localrb_count].rb_Identity))
        {
          /*rab-id does not exist in established_rab*/
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          is_valid_rbid = FALSE;
          invalid_rbid = rb_info_affected_list_r4_ptr->elem[localrb_count].rb_Identity;
          /*one of the rab-id does not exist in established_rab*/
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"rb-id %d is not in established_rab",invalid_rbid);
          break;
        }
        tdsrrc_rbrc.rbs_tobe_affected.rb_id[localrb_count] = 
          rb_info_affected_list_r4_ptr->elem[localrb_count].rb_Identity;
        localrb_count++;
      }
      tdsrrc_rbrc.rbs_tobe_affected.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Num of RBs tobe affected:%d",localrb_count);
    }
  }

  if (!is_valid_rbid)
  {
    /*one of the rab-id does not exist in established_rab*/
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"rb-id %d is not in established_rab",invalid_rbid);
    /* Send the RB Reconfig Failure Message */
    tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                               tdsrrc_rbrc.status.prot_err_cause
                             );
    /* Make sure we're back in the initial sub-state */
    tdsrrcrbrc_clear_procedure();
    tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;
    return;
  }
  /* For Cell_FACH or Cell_DCH continue with the ordered config setting */
  tdsrrcrbrc_update_ordered_config_r4 (dcch_msg_ptr, msg_version);
}/* end function tdsrrcrbrc_process_rbrc_message_r5 */

/*===========================================================================

FUNCTION   RRCRBRC_UPDATE_ORDERED_CONFIG_R4

DESCRIPTION

  This function processes the received Channel Config Confirmation
  from LLC. If the required Radio-bearers have been reconfig succesfully,
  and if any of the radio-bearers map to radio-access-bearers, this
  information needs to be given to the NAS entity for that RAB.
  
  Currently Circuit-Switched voice and Circuit-Switched data RABs are 
  supported. If a voice RAB has been assigned by the UTRAN, this function
  gets the RLC ids and the AMR mode information from LLC and sends this
  information to voice services. If a CS Data RAB has been assigned, the
  RLC ids for those RBs are sent to the data services entity. Then this
  function sends the RRC_SYNC_IND primitive to MM.

  The function is copy from tdsrrcrbrc_update_ordered_config_r5
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbrc_update_ordered_config_r4
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr,      /* Pointer to the RB Reconfig message */
  tdsrrc_msg_ver_enum_type                   msg_version
)
{
  tdsrrc_state_e_type                       current_state;       /* current RRC State  */
  tdsrrcllc_oc_status_e_type                oc_status;/* To hold tdsordered_config status          */
  boolean                                ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                                                       occured while setting tdsordered_config. */
  tdsrrc_int_c_rnti_type                    c_rnti;
  tdsrrc_int_u_rnti_type                    u_rnti;
  tdsrrcrb_rnti_update_info_type            rnti_update_info;   /* to hold rnti update info */
  tdsrrc_RadioBearerReconfiguration         *msg_ptr; /* Pointer to the RB Reconfig message        */
  tdsrrcrb_directed_cell_info_type          directed_cell_info; /* directed cell info struct */
  boolean                                crnti_present = FALSE;
  ASN1BitStr32                           *crnti_ptr = NULL;
  tdsrrc_RadioBearerReconfiguration_r4_IEs  *rbrc_ptr = NULL;

  /* We have to reconfigure RBs. First set TDSORDERED_CONFIG
    so that LLC can process the necessary parameters. */
  /* Get the pointer to the actual RB Reconfig message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerReconfiguration);
  oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBRC,
                                         tdsrrc_rbrc.state_indicated_by_msg,
                                         tdsrrc_DL_DCCH_Message_PDU,
                                         (void *)dcch_msg_ptr
                                       );
  
  rbrc_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4;
  switch (oc_status)
  {
    case TDSORDERED_CONFIG_SET:
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
      if((tdsrrcsib_is_current_event_sib_read_in_dch()) &&
         (TDSRRC_PROCEDURE_GPS == tdsrrcsib_get_proc_id_for_sib_read_in_dch()))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Force Init GPS ");
        tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
      }


#endif 

      current_state = tdsrrc_get_state(); 
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
     
      directed_cell_info.freq_info_present = FALSE;
      directed_cell_info.cpid_present = FALSE;
     
      /* Populate the local variables to indicate if Re-direction info is present */
      if((current_state==TDSRRC_STATE_CELL_FACH) && 
            ((tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
            ||(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
            ||(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)
            ))
      {
        if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r4_IEs,frequencyInfo)) &&
               (rbrc_ptr->frequencyInfo.modeSpecificInfo.t == 
                T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
              )
        {
          directed_cell_info.freq_info_present = TRUE;
          directed_cell_info.uarfcn_dl =  rbrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
        }
           
 
        if (tdsrrcrbrc_is_pccpch_info_present(msg_ptr, TDSMSG_REL4))
        {
          directed_cell_info.cpid_present = TRUE;
          directed_cell_info.cell_param_id = rbrc_ptr->dl_InformationPerRL_List.elem[0].
               modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
        }
        /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
              FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
        if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
          (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
          if(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
          {
            tdsrrcrbrc_send_initiate_cu_flag = TRUE;
            tdsrrcrbrc_delete_crnti          = TRUE;
          }
          else
          {
            tdsrrc_rbrc.directed_cell_info = TRUE;
          }
        }             
      }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
      if ((current_state==tdsrrc_rbrc.state_indicated_by_msg) ||
          ((current_state==TDSRRC_STATE_CELL_FACH) && 
           (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH)))
      {
        /* CELL_DCH->CELL_DCH or CELL_FACH->CELL_FACH case. We can release the
        Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
        
        /*check whether any frequency info is present in the message
        and transition is CELL_FACH->CELL_FACH. In this case
        initiate a cell update after channel config is successfully completed */
        if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
            (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
        {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if((tdsrrcrbrc_send_initiate_cu_flag == FALSE) && 
            (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r4_IEs,new_C_RNTI))
            {
              tdsrrc_rbrc.new_crnti_valid = TRUE;
              tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
              tdsrrc_rbrc.new_crnti = (uint16)c_rnti;
            }
            else
            {
              tdsrrc_rbrc.new_crnti_valid = FALSE;
            }
    
            tdsrrcllcoc_update_toc_with_oc_from_fach();
            /*Initiate the cell selection*/
            tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
                                                       TDSRRC_TRANSITION_FROM_FACH,
                                                       tdsrrc_rbrc.state_indicated_by_msg);
    
    
                
                
            tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
    
  
            /* exit from the switch case */
            break;
          }
    
#else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
          if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r4_IEs,frequencyInfo)) &&
              (rbrc_ptr->frequencyInfo.modeSpecificInfo.t == 
               T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd)
             )
          {
            directed_cell_info.freq_info_present = TRUE;
            directed_cell_info.uarfcn_dl         = 
            rbrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
          }
          else
          {
            directed_cell_info.freq_info_present = FALSE;
          }


          if (tdsrrcrbrc_is_pccpch_info_present(msg_ptr, TDSMSG_REL4))
          {
            directed_cell_info.cpid_present = TRUE;
            directed_cell_info.cell_param_id = rbrc_ptr->dl_InformationPerRL_List.elem[0].
                   modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          }
          else
          {
            directed_cell_info.cpid_present = FALSE;
          }

          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
          {
            /*directed cell info matched. Cell update is not required */
            tdsrrcrbrc_send_initiate_cu_flag = FALSE;
          }
          else
          {
            /*directed cell info do not match. Cell update is required */
            tdsrrcrbrc_send_initiate_cu_flag = TRUE;
            tdsrrcrbrc_delete_crnti          = TRUE;
          }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
        } /* if CELL_FACH->CELL_FACH reconfiguration */
        /* Call the function that creates and sends a channel
          config request to LLC. */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
        tdsrrcrbrc_send_channel_config_req();

        /* Change the substate since we're waiting for the
           confirmation from LLC for the channel configs */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
      }
      else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
               (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
      {

        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r4_IEs,new_C_RNTI))
        {
          tdsrrc_rbrc.new_crnti_valid = TRUE;
          tdsrrc_translate_crnti(&rbrc_ptr->new_C_RNTI, &c_rnti);
          tdsrrc_rbrc.new_crnti = (uint16)c_rnti;
        }
        else
        {
          tdsrrc_rbrc.new_crnti_valid = FALSE;
        }

        /*Initiate the cell selection*/
        tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBRC,
                                                TDSRRC_TRANSITION_FROM_DCH,
                                                tdsrrc_rbrc.state_indicated_by_msg);
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

        /* Start T_305 [periodic cell update] timer */
        tdsrrccu_start_t_305_timer();

      }
      else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
               (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
      {
        tdsrrc_rbrc.directed_cell_info = tdsrrcrbrc_is_pccpch_info_present(msg_ptr, msg_version);
        /*Send the RRCRBRC complete msg on old configuration */
        tdsrrcrbrc_send_rbrc_complete_message(TRUE);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
        /*wait for L2ack */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;

        tdsrrcllc_print_trans_chnl_rate();
      }
      else if ((current_state==TDSRRC_STATE_CELL_DCH) && 
               (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
      {
        /* no need to check directed cell info in this case */
        /*Send the RRCRBRC complete msg on old configuration */
        tdsrrcrbrc_send_rbrc_complete_message(TRUE);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
        /*wait for L2ack */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;

        tdsrrcllc_print_trans_chnl_rate();
      }
      else if ((current_state==TDSRRC_STATE_CELL_FACH) && 
               ((tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
                (tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
      {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
        /*call the function that evaluates the directed cell info and set the flags accordingly */
        if((tdsrrc_rbrc.directed_cell_info == FALSE) && 
          (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
        {
            
          if(tdsrrc_rbrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
          {
            tdsrrc_rbrc.directed_cell_info = tdsrrcrbrc_is_pccpch_info_present(msg_ptr, msg_version);
          }


          tdsrrc_rbrc.initiate_cell_selection_trans_from_fach = TRUE;
        }
    
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

        /*Send the RRCRBRC complete msg on old configuration */
        /* check whether new C-RNTI was present in the message. In that case
        we have to configure MAC with new C-RNTI before sending the response 
        message */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r4_IEs,new_C_RNTI))
        {
          crnti_present = TRUE;
          crnti_ptr = &rbrc_ptr->new_C_RNTI;
        }

        if (crnti_present)
        {
          tdsrrc_translate_crnti(crnti_ptr, &c_rnti);
          /* Send a RNTI Update Request */
          rnti_update_info.crnti_action = TDSRRC_RNTI_UPDATE;
          rnti_update_info.crnti        = (uint16)c_rnti;
          /* check whether URNTI is also present in the message */
          if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r4_IEs,new_U_RNTI))
          {
            tdsrrc_translate_urnti(&rbrc_ptr->new_U_RNTI, &u_rnti);
            rnti_update_info.urnti_action = TDSRRC_RNTI_UPDATE; 
            rnti_update_info.urnti        = u_rnti; 
          }
          else
          {
            rnti_update_info.urnti_action = TDSRRC_RNTI_NOCHANGE; 
          }
          rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBRC;
          rnti_update_info.rlc_re_establish_srb = FALSE;
          rnti_update_info.rlc_re_establish_rab = FALSE;
          rnti_update_info.cnf_reqd             = TRUE;
          tdsrrcrb_send_rnti_update_req(rnti_update_info);
          /* wait for the RNTI update confirm before sending the response
          message */
          tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
        }
        else
        {
          tdsrrcrbrc_send_rbrc_complete_message(TRUE);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBRC Complete msg");
          /*wait for L2ack */
          tdsrrc_rbrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
          tdsrrcllc_print_trans_chnl_rate();
        }
      }
      break;

    case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
      /* Send the RB Release Failure Message */
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause );

      /* Go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
      tdsrrc_rbrc.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      /* Send the RB Release Failure Message */
      tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                 tdsrrc_rbrc.status.prot_err_cause );

      /* Go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_rbrc.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbrc_send_rbrc_failure_message( tdsrrc_rbrc.status.failure_status,
                                  tdsrrc_rbrc.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

    case TDSORDERED_CONFIG_NOT_SET_OTHER:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
      /* When LLC sends this, there is a serious problem
      and we'll go back to disconnected state. No need
      to send the failure message here, we can just
      go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;

    default:

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
      /* Unknown error, just go back to initial substate */
      tdsrrc_rbrc.curr_substate = TDSRRCRB_INITIAL;

      /* Indicate that an error occured */
      ordered_config_err_occured = TRUE;
      break;
  }/* end switch */

  if ( ordered_config_err_occured == TRUE )
  {
    /* No need to continue. Clear the procedure
    variables and return. */
    tdsrrcrbrc_clear_procedure();
    return;
  }
  else
  {
    if (tdsrrc_rbrc.rbrc_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }
  }
}

/*===========================================================================

FUNCTION   VALIDATE_RBRC_MESSAGE_R4

DESCRIPTION

  This function validates the received RB Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Reconfig Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.

  The function is copy from R5,but handle RB is from R3
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.s

===========================================================================*/
void tdsrrcrbrc_validate_rbrc_message_r4
(
  tdsrrc_RadioBearerReconfiguration *msg_ptr /* Pointer to the RB RC message */
)
{
  tdsrrc_RadioBearerReconfiguration_r4_IEs *rbrc_ptr = NULL;

#ifdef FEATURE_UMTS_PDCP
  tdsrrc_RB_InformationReconfig_r4 *rb_info_ptr = NULL;
  tdsrrc_HeaderCompressionInfo_r4 *pdcpHeaderCompressionInfo = NULL;  
#endif /* FEATURE_UMTS_PDCP*/
  rrc_rab_id_type rab_id;           /* To store RAB id in internal RRC format */
  rrcmn_rab_status_T rab_type;      /* To store the status returned by MN layer. This
                                    indicates what type of RAB is received. This is 
                                    used only if the RAB is for the CS_domain */
  tdsrrc_rab_search_e_type tdsrab_status; /* To check if RAB is present in ESTABLISHED_RABS */
  uint32 rab_index=0;               /* Used as an index when storing established
                                    RABs locally. Only when we have more than
                                    one RAB per message, this will be incremented */
  tdsrrc_RAB_InformationReconfigList *rab_list_ptr = NULL;  
  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();


  /* Assume the message is valid for now */
  tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_VALID;

  rbrc_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4;

  /* Check the RRC State indicatior given by the message and 
     store it in the tdsrrc_rbrc internal memory
   */
  switch (rbrc_ptr->rrc_StateIndicator)
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;

    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_rbrc.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;

    default:
      /* No other states are supported right now. */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported state %d in RBRC R5 Msg",rbrc_ptr->rrc_StateIndicator);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;
  }

  if (tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG)
  {
    return;
  }

  /* Check if RB Information Affected list is present. This
  is not supported in the first release. This may be supported
  later */
  if ( TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4,
        tdsrrc_RadioBearerReconfiguration_r4_IEs,rb_InformationAffectedList)
     )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB Info Affected list present in RB RC Message");
  }

  /* Obtain U_RNTI if present for SRNS relocation.  */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,
   tdsrrc_RadioBearerReconfiguration_r4_IEs,new_U_RNTI))
  {
    /* check whether the SRNS relocation is within Cell_DCH or not.
       SRNS relocation information can come in reconfiguration messages
       only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbrc.state_indicated_by_msg);
      tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* message with valid SNRS relocation info */
    else
    {
      /* Set the flag that indicates that this procedure
         involves SRNS relocation */
      tdsrrc_rbrc.rbrc_for_srns_relocation = TRUE;
      /*store the new U-RNTI */
      tdsrrc_translate_urnti (&(rbrc_ptr->new_U_RNTI), &tdsrrc_rbrc.new_urnti);
    }
  }

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  tdsrrc_rbrc.trans_to_dch_ciph_config_needed = FALSE;
  if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    /* check to see if dl-dpch-common info is present while going to dch */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r4_IEs,dl_CommonInformation)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rbrc_ptr->dl_CommonInformation,
         tdsrrc_DL_CommonInformation_r4,dl_DPCH_InfoCommon)))
    {
      /* check if ciphering is active in mac for cs domain, if it is then,
        start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      */
      if (tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");
        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             (TDSRRC_MSG_COMMON_BITMASK_IE(rbrc_ptr->dl_CommonInformation.dl_DPCH_InfoCommon,
             tdsrrc_DL_DPCH_InfoCommon_r4,mac_d_HFN_initial_value)),
             &rbrc_ptr->dl_CommonInformation.dl_DPCH_InfoCommon.mac_d_HFN_initial_value,
             &mac_d_hfn_l);
        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }	 
        if (tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing Ciphering Config for HHO failed");
        }
        tdsrrc_rbrc.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }
  
  /*The processing of RAB Info Reconfig is done differently based on feature WB-AMR.
  When feature FEATURE_TDSCDMA_VOC_AMR_WB is enabled, we process all the RABs but in case of
  no feature, we process only the first RAB. Right now, there's in fact no need to process more
  than one RABs in the same OTA.*/
  /*Here we process all the RABs in RAB Info Reconfig List*/    
  /* copy handle from R3 begin */
  /*********************************/    
  /*********************************/  
    if ( TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4,
         tdsrrc_RadioBearerReconfiguration_r4_IEs,rab_InformationReconfigList)
       )
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RAB Info Reconfig in RB RC Message");
        
      if (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4.rab_InformationReconfigList.n != 0)
      {
        rab_list_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4.rab_InformationReconfigList;   
       
        for (rab_index=0; (rab_list_ptr->n > rab_index) && (rab_index < TDSRRCRB_MAX_RAB_PER_MSG); rab_index++)
        {
          /* Check if the RAB id is GSM_MAP type. We don't support any
              other type */
          if ( rab_list_ptr->elem[rab_index].rab_Identity.t != T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity  )
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RABid t%d is not GSM_MAP!",rab_list_ptr->elem[rab_index].rab_Identity.t);
            tdsrrc_rbrc.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
            return;
          }
      
          /* GSM_MAP RAB Id. Check if it's valid by calling the
          appropriate NAS entity. First convert it to internal
          format. */
          rab_id = tdsrrcrb_translate_gsm_map_rab_id(
                                                 (rab_list_ptr->elem[rab_index].rab_Identity.u.gsm_MAP_RAB_Identity)
                                                 );
      
          /* Check if the RAB id is already in ESTABLISHED_RABS */
          tdsrab_status = tdsrrc_find_rab_in_established_rabs (rab_id);
          if ( tdsrab_status != TDSRRC_RAB_FOUND )
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB ID 0X%x not in ESTABLISHED_RABS",rab_id);
            tdsrrc_rbrc.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
            return;
          }
      
          /* Store this info locally so that ESTABLISHED_RABS can
          be updated later, when these RABs are established */
          tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_id_is_gsm_map = TRUE;
          tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_id = rab_id;
      
           /* Check if this RAB is CS Domain or PS Domain */
          if ( rab_list_ptr->elem[rab_index].cn_DomainIdentity  == tdsrrc_CN_DomainIdentity_cs_domain )
          {
            /* Store the domain identity. This will be later added to
            ESTABLISHED_RABS */
            tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].cn_domain = RRC_CS_DOMAIN_CN_ID;
      
            /* Check if it is in loopback test */
            if ( tdsrrclbt_is_lb_on() == TRUE )
            {
              /* Set Rab type to Data */
              TDSRRC_MSG0(MSG_LEGACY_LOW,"Loopback Test");
              tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
            }
            else
            {
              /* For CS RABs, Call MN layer to verify this RAB */
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
              rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
              rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif
  
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"For RAB-Id %d, RAB type from NAS = %d, RAB type from EST_RABs = %d",rab_id, rab_type,
                tdsrrc_get_rab_type_for_given_rab_id(tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_id));
      
              switch ( rab_type )
              {
                case RRCMN_CS_DATA_CALL:
                  tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
                  break;
      
                case RRCMN_CS_VOICE_CALL:
                  tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_VOICE_RAB;
  
#ifdef FEATURE_SCUDIF_ENHANCEMENT
                   #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/
  
                  break;
      
                default:
                  TDSRRC_MSG0(MSG_LEGACY_ERROR,"CS RAB validation failed!");
                  tdsrrc_rbrc.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
                  break;
      
              } /* end switch */
  
              /*As a general approach we should use/update the NAS Sync Indicator even for CS DATA*/
#ifdef FEATURE_SCUDIF_ENHANCEMENT
              #error code not present
#else
              if(tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB)
#endif /* FEATURE_SCUDIF_ENHANCEMENT */
              {
                if(tdsrrcrbrc_update_nas_sync_ind(rab_index,&(rab_list_ptr->elem[rab_index].nas_Synchronisation_Indicator))
                    == FAILURE)
                {
                  return;
                }
                nas_sync_ind = tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator;              
              }
            }
      
            if ( (tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_INVALID_CONFIG) ||
                 (tdsrrc_rbrc.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG)
               )
            {
              /* No need to continue */
              return;
            }
          }
          /* Else check for PS Domain */
          else if ( rab_list_ptr->elem[rab_index].cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain )
          {
            /* Store the domain identity. This will be later added to
              ESTABLISHED_RABS */
            tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].cn_domain = RRC_PS_DOMAIN_CN_ID;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"PS RABId %d to be setup",rab_id);
              tdsrrc_rbrc.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_PS_DATA_RAB;
          }   
        }     
      }
    }/* if rrc_RadioBearerReconfiguration_r4_IEs_rab_InformationReconfigList_present is present*/
    tdsrrc_rbrc.rabs.num_rabs_in_msg = rab_index;
    tdsrrc_set_ordered_codec_mode(nas_sync_ind);
  
  /*********************************/   
  /*********************************/  
  /* Copy handle from R3 end */

#ifdef FEATURE_UMTS_PDCP
 if (tdsrrc_pdcp_enabled)
 {
  if (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4.rb_InformationReconfigList.n != 0)
  {
      
    uint32 local_rb_idx =0;
    while (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4.
              rb_InformationReconfigList.n > local_rb_idx) 
    {
      rb_info_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4.rb_InformationReconfigList.elem[local_rb_idx]; 
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_ptr,tdsrrc_RB_InformationReconfig,pdcp_SN_Info))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP SN info not supported");
        tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        return; 
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_ptr,tdsrrc_RB_InformationReconfig,pdcp_Info))
      {
        if (tdsrrc_get_rab_type_for_given_rb_id(rb_info_ptr->rb_Identity) != TDSRRCRB_PS_DATA_RAB)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"NW incld PDCP info for non-ps rb id %d: Invalid",rb_info_ptr->rb_Identity);
          tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          return;
        }
        
        /* check whether lossless SRNS relocation flag is set or   
         * and header compression algo is present in the message 
         */
        if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info.pdcp_Info,tdsrrc_PDCP_Info,losslessSRNS_RelocSupport)) && 
            (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_info_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,rrc_LosslessSRNS_RelocSupport_supported)))
        {
          if (rb_info_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_absent)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP pdu header is absent.  Invalid config");
            tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
            return;
          }   
        } /* if rrc_PDCP_Info_r4_losslessSRNS_RelocSupport_present */
  
        if (rb_info_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) 
        {
          if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info.pdcp_Info,tdsrrc_PDCP_Info,losslessSRNS_RelocSupport)) && 
              (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_info_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,rrc_LosslessSRNS_RelocSupport_notSupported)))
          {
            if (!(TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info.pdcp_Info,tdsrrc_PDCP_Info,headerCompressionInfoList)) 
                ) 
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP header compress info list not present");
              tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
              return;
            }
          }
        }
        
        if (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info.pdcp_Info,tdsrrc_PDCP_Info,headerCompressionInfoList)) 
        {
          uint32 local_idx=0;
          while (rb_info_ptr->pdcp_Info.pdcp_Info.headerCompressionInfoList.n > local_idx) 
          {
            pdcpHeaderCompressionInfo = &rb_info_ptr->pdcp_Info.pdcp_Info.headerCompressionInfoList.elem[local_idx];
            if (TDSRRC_CHECK_COMMON_MSG_TYPE(pdcpHeaderCompressionInfo->algorithmSpecificInfo,
                rrc_AlgorithmSpecificInfo_rfc2507_Info))
            {
          /* if the UE capability "Maximum header compression context space", is exceeded with this configuration send
           * failure with cause "Invalid Config"
               */
              if ( 
                  (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER > IPHC_MAX_HC_CONTEXT_SPACE))
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Max header comprss context space exceeded : %d",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER);
                tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                return;
              }
            }
            local_idx++;
          } /*  while (pdcpHeaderCompressionInfoList) */
        } /*  if (rb_info_ptr->pdcp_Info.pdcp_Info.bit_mask & rrc_PDCP_Info_r4_headerCompressionInfoList_present) */
  
      } /* if (rb_info_ptr->bit_mask & rrc_RB_InformationReconfig_r5_pdcp_Info_present) */
      local_rb_idx++;
    } /* while (rb_list_ptr) */
  } /* if (rbrc_ptr->specificationMode.u.complete.bit_mask & rrc_complete_rb_InformationReconfigList_present) */
 }
#endif /* FEATURE_UMTS_PDCP */

  /*Cell_PCH/URA_PCH specific validations */
 if ((tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
     (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
 {
   /* If UTRAN DRX coefficient is not present then it is invalid configuration */
   if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,
         tdsrrc_RadioBearerReconfiguration_r4_IEs,utran_DRX_CycleLengthCoeff)))
   {
     TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_rbrc.state_indicated_by_msg);
     tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
     return;
   }
   if (tdsrrc_rbrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
   {
     /*should we do it after getting l2ack for response??*/
     if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r4_IEs,ura_Identity)))
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
       tdsrrccu_clear_ura_id ();
     }
     else
     {
       /*URA ID present in the message set ura_identity variable with this value */
       if (rbrc_ptr->ura_Identity.numbits == 16)
       {
         tdsrrccu_update_ura_id (&(rbrc_ptr->ura_Identity));
       }
       else
       {
         TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rbrc_ptr->ura_Identity.numbits);
         tdsrrc_rbrc.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
         tdsrrc_rbrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
         return;
       }
     }
   }
 } // if TDSRRC_STATE_CELL_PCH or TDSRRC_STATE_URA_PCH
 
}/* end function tdsrrcrbrc_validate_rbrc_message_r4 */


