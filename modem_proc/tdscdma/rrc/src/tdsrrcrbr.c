/*===========================================================================
      R R C   R A D I O   B E A R E R   R E L E A S E   M O D U L E 

DESCRIPTION

  This module performs the Radio Bearer Release Procedure. This
  module processes the Radio Bearer Release Message and releases the 
  radio bearers given by the message. 
  
  The Radio Bearer Release Message may release both signalling radio bearers
  and radio bearers that are mapped to radio access bearers. 
  It may set also re-configure previously established radio bearers.
  The procedure may also initiate a hard-handover if the Radio Bearer
  Release Message indicates a frequency change or a frame offset change.
  
  
EXTERNALIZED FUNCTIONS

  tdsrrcrbr_procedure_event_handler   Event handler for this procedure
  tdsrrcrbr_init_procedure            Power-up initialization of the procedure

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Procedure must be initialized at power-up by calling tdsrrcrbr_init_procedure. 
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrbr.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------
03/14/12   as      Added code to support feature NB/WB handover support
06/16/11   bj     Added code to handle receive R9 RadioBearRlease Message.
03/10/11   bj     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/18/10   ysh     Frequency info save. 3GPP-8.6.6.1(3.4.2)
                                  Save Primary freq when enter Non-CELL_DCH state.
11/11/10   yzh     (3GPP: 8.6.6.27)Del tdsrrcllcpcie_initialise_sync_a_post_veri_info()
                    when IE Downlink information common for all radio links(FDD->TDD).
11/10/10   bj       Deleted IE "RNC support for change of UE capability".(3GPP: 8.6.2.3)
10/30/10   ysh     IE"Downlink information for each radio link" info save.  3GPP-8.6.6.4(3.2.13).
10/29/10   zwj     Added code to handle receive R4 RadioBearRlease Message.

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
#include "tdsrrcasn1util.h"
#include "rabmtask.h"
#include "rex.h"
#include "tdsrrcasn1.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcccm.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrcdt.h"
#include "tdsrrccu.h"
#include "tdsrrccui.h"
#include "tdsrrcintif.h"
#include "tdsrrclcm.h"
#include "tdsrrcllc.h"
#include "tdsrrcllcoc.h"
#include "tdsrrcmm_v.h"
#include "tdsrrcrbcommon.h"
#include "tdsrrcrbr.h"
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

#include "rrcmnif.h"
#include "tdsrrclbt.h"

#include "tdsrrcmisc.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsrrcdormancy.h"

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

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
/* We need to introduce a delay before transmitting
on a new configuration if the base-station cannot have
the old link and new link up at the same time. */
#define TDSRRCRBR_TEMP_TIMER_SIG 0x2000
rex_sigs_type tdsrrcrbr_sig;
rex_timer_type tdsrrcrbr_temp_timer;

#ifndef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
/* Structure to hold information about the RBs to be released.             */
typedef struct
{
  boolean is_valid;     /* indicates if there are RBs that need to be 
                           released/affected. If this is FALSE, the two
                           items below are not used                       */
  uint32 num_rbs;       /* Number of RBs released by the message          */
  tdsrrc_RB_Identity rb_id[TDS_MAX_RB]; /* Hold the ids of the RBs that are
                           released/affected by the RBR message           */
}tdsrrcrbr_rb_info_type;

typedef struct {
  tdsrrc_RB_Identity rb_id; /* Hold the ids of the RBs */
  tdsrrc_rb_cfg_e_type rb_config;
  uint16 ul_cfg_mask;
  uint16 dl_cfg_mask;
}tdsrrcrbr_rb_id_action;

typedef struct {
  boolean is_valid;
  uint32 num_rbs;       /* Number of RBs tobe configured by the message          */
  tdsrrcrbr_rb_id_action rb_info[TDS_MAX_RB];
}tdsrrcrbr_rb_reconfig_info_type;

/* Structure to store all internal data related to RBR procedure           */
typedef struct
{
  tdsrrcrb_substate_e_type curr_substate;  /* Store the current substate     */
  tdsrrcrb_transaction_id_type tr_ids;      /* Transaction ids for RBR msg    */
  tdsrrc_state_e_type state_indicated_by_msg; /* RRC State given by RBR msg   */
  tdsrrcrb_msg_failure_cause_type status;   /* Status of message validation   */
  tdsrrcrbr_rb_info_type rbs_tobe_released; /* Information on the RBs to be
                                            released                       */
  tdsrrcrbr_rb_reconfig_info_type rbs_tobe_reconfigured; /* Information on the RBs
                                             to be reconfigured              */
  tdsrrcrbr_rb_info_type rbs_tobe_affected; /* Information on the RBs to be
                                            affected                       */
  tdsrrcrb_rab_info_per_msg_type rabs;      /* Hold info about each rab in one
                                         RBR message till they're processed.
                                         This is used when RABs need to be
                                         reconfigured.                     */
  boolean need_to_release_cn_domain;     /* TRUE if a signalling connection
                                         for a CN domain is to be released */
  rrc_cn_domain_identity_e_type cn_to_release;   /* The domain to be released      */

  boolean    new_crnti_valid;            /* Indicates if a valid C-RNTI was recd 
                                          * in the message transitioning from CELL_DCH
                                          * to CELL_FACH
                                          */
  uint16     new_crnti;
  boolean    directed_cell_info; /* Indicates if a valid C-RNTI was recd */
  tdsrrc_int_u_rnti_type          new_urnti;
  boolean                      new_urnti_valid;          /* Indicates if a valid U-RNTI was recd */ 
  boolean                      rbr_for_srns_relocation;  /* Indicates SRNS reloc involved with 
                                                            this msg                             */

  boolean                       hho_ciph_status;
  boolean                       cn_info_saved;           /*flag to indicate that CN info is saved */
  mm_cmd_type *                 mm_cmd_ptr;              /*pointer to hold mm_cmd with cn info */

  boolean                       trans_to_dch_ciph_config_needed;
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /* Indicates if Fre-redirection is to be triggered in FACH->PCH case */
  boolean initiate_cell_selection_trans_from_fach;
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/


  boolean high_mobility_ind;

}tdsrrcrbr_int_data_type;
#endif/*FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE*/
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrcrbr_int_data_type tdsrrc_rbr;

boolean tdsrrcrbr_send_initiate_cu_flag = FALSE;
boolean tdsrrcrbr_delete_crnti          = FALSE;

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */
/*===========================================================================

FUNCTION   tdsrrcrbr_mark_cn_doamin_to_be_closed

DESCRIPTION

  This function processes the received SCRI in the RB release message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_mark_cn_doamin_to_be_closed
(
  tdsrrc_CN_DomainIdentity signallingConnectionRelIndication
)
{
  switch(signallingConnectionRelIndication)
  {
    case tdsrrc_CN_DomainIdentity_cs_domain:
      if(tdsrrcdt_is_cs_domain_open())
      {
        /* Need to release a CN domain */
        tdsrrc_rbr.need_to_release_cn_domain = TRUE; 
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CS doamin NOT open");
      }
      /* Copy the domain that is to be released */
      tdsrrc_rbr.cn_to_release = RRC_CS_DOMAIN_CN_ID;
      break;
     
    case tdsrrc_CN_DomainIdentity_ps_domain:
      if(tdsrrcdt_is_ps_domain_open())
      {
        /* Need to release a CN domain */
        tdsrrc_rbr.need_to_release_cn_domain = TRUE; 
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PS doamin NOT open");
      }
      /* Copy the domain that is to be released */
      tdsrrc_rbr.cn_to_release = RRC_PS_DOMAIN_CN_ID;
      break;
      
    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"This should not happen");
      break;
  }
}
/*===========================================================================

FUNCTION   RRCRBR_INDICATE_LAYER1_IF_CONFIG_MSG_RELEASE_VOICE_RAB

DESCRIPTION

  If radio bearer release message indicates UE to release voice RAB then this
  function will indicate layer1 about the same by calling some feature specific
  call back function provided by layer1.
  If there is no change in the voice RAB status then this function will not do anything.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_indicate_layer1_if_config_msg_release_voice_rab(void)
{
  uint32 rb_index, count; /* counter */
  uint32 num_rb, temp_num_rb;
  uint32 rb_id_list[MAX_RB_PER_RAB];

  if ((TRUE == tdsrrc_rbr.rbs_tobe_released.is_valid) && 
       (TRUE == tdsrrc_get_rb_id_list_associated_with_voice_rab(&num_rb, rb_id_list)))
  {
    temp_num_rb = num_rb;
    for (rb_index=0; rb_index<tdsrrc_rbr.rbs_tobe_released.num_rbs; rb_index++)
    {
      for (count=0; count<num_rb; count++)
      {
        if (rb_id_list[count] == tdsrrc_rbr.rbs_tobe_released.rb_id[rb_index])
        {
          --temp_num_rb;
          break;
        }
      }
    }

    /* if num_rb is 0 at this point then it means voice RAB is going to be released */
    if (0 == temp_num_rb)
    {
      tdsrrc_indicate_layer1_next_config_will_release_voice_rab();
    }
  }
}

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
boolean tdsrrcrbr_is_srns_in_progress
(
  void 
)
{
  return tdsrrc_rbr.rbr_for_srns_relocation;
}
/*===========================================================================

FUNCTION   RRCRBR_GET_NEW_CRNTI_VALID()

DESCRIPTION
 This function returns if reconfig msg has valid CRNTI


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrbr_get_new_crnti_valid
(
  void
)
{
  return tdsrrc_rbr.new_crnti_valid;
}
 /*===========================================================================

FUNCTION   RRCRBR_GET_CURRENT_SUB_STATE()

DESCRIPTION
 This function is used to return current substate


        
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrcrb_substate_e_type current_substate.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_substate_e_type tdsrrcrbr_get_current_sub_state( )
{
  return tdsrrc_rbr.curr_substate;
}

/*===========================================================================

FUNCTION tdsrrcrbr_append_start_list_to_rbr_complete
DESCRIPTION
  This function appends start list to tdsrrc_rbr message for SRNS relocation and
  also when TM bearers are present during non-dch to dch rrc-state
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcrbr_append_start_list_to_rbr_complete
(
tdsrrc_UL_DCCH_Message *msg_ptr               /* Pointer to Uplink DCCH Msg */
)
{
  /*start list is included in ul_counter_synchronization_info. So enable this IE in the
  bitmask */
  TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.radioBearerReleaseComplete,
  tdsrrc_RadioBearerReleaseComplete,ul_CounterSynchronisationInfo);

  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.radioBearerReleaseComplete.
  	ul_CounterSynchronisationInfo);
 
  if (SUCCESS != tdsrrcsmc_append_start_list(&msg_ptr->message.u.radioBearerReleaseComplete.ul_CounterSynchronisationInfo.startList,
                           TDSRRCSMC_EXTRACT_AND_APPEND_START))
  {
    /*should not come here */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBR: Could not append Start list");
  }
}
/*===========================================================================
FUNCTION   SEND_RBR_FAILURE_MSG

DESCRIPTION

  This function sends the Radio Bearer Release Failure message
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
static void tdsrrcrbr_send_rbr_failure_message
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
  rlc_lc_id_type ul_lc_id;      /* Logical Channel Id for the message  */ 
  tdsrrcsend_status_e_type status; /* Status from send chain module       */

  /* Allocate memory for this message. Note that this buffer is released
  by the SEND CHAIN */
  ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc(
                                         sizeof(tdsrrc_UL_DCCH_Message));
  if (ptr == NULL)
  {
    ERR_FATAL("No memory for RBR Failure msg!",0,0,0);
  }
  /* Reset SVTHHO in case of failure */
  tdsrrcsmc_reset_start_value_to_transmit_cs_hho(FALSE);

  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_radioBearerReleaseFailure; /* RBR Failure msg */

  /* Set the bit-mask - neither rrc_potentiallySuccesfulBearerList_present
  nor rrc_RadioBearerReconfigurationFailure_nonCriticalExtensions_present
  are included */
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReleaseFailure);

  /* Transaction Id - First check the rejected_transacion id. If
  it's not invalid, this message is being rejected because there
  is already another message accepted but not processed.
  If the rejected_transaction id is invalid, we are rejecting the
  accepted message, so use the accepted transaction id.

  NOTE: Don't change the order, it's important to check rejected
  transaction first. */
  if ( tdsrrc_rbr.tr_ids.rejected_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.radioBearerReleaseFailure.rrc_TransactionIdentifier =
    tdsrrc_rbr.tr_ids.rejected_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_rbr.tr_ids.rejected_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  }
  else if ( tdsrrc_rbr.tr_ids.accepted_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.radioBearerReleaseFailure.rrc_TransactionIdentifier =
    tdsrrc_rbr.tr_ids.accepted_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_rbr.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  }
  else
  {
    /* Don't know the transaction id for this message.( one reason
    for this could be that the message had a critical
    extenstion). Since it's unknown, we should set it to zero. */
    ptr->message.u.radioBearerReleaseFailure.rrc_TransactionIdentifier = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Transaction id invalid for RBR Failure message!");
  }

  switch ( failure_cause )
  {
  case TDSRRCRB_MSG_UNSUPPORTED_CONFIG:
    ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
    break;

  case TDSRRCRB_PHY_CHAN_FAILURE:
    ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_physicalChannelFailure;
    break;

  case TDSRRCRB_SIMULTANEOUS_RECONFIG:
    ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_incompatibleSimultaneousReconfiguration;
    break;

  case TDSRRCRB_MSG_PROTOCOL_ERROR:
    ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_protocolError;

	ptr->message.u.radioBearerReleaseFailure.failureCause.u.protocolError=
	  	rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation);

    if(ptr->message.u.radioBearerReleaseFailure.failureCause.u.protocolError == NULL)
    {
      ERR_FATAL("Memory allocation failure",0,0,0);
    }
    
    /* When there is a Protocol Error, we also have to indicate
    the protocol error cause */
    ptr->message.u.radioBearerReleaseFailure.failureCause.u.protocolError->diagnosticsType.t =
    T_tdsrrc_ProtocolErrorInformation_diagnosticsType_type1;

	ptr->message.u.radioBearerReleaseFailure.failureCause.u.protocolError->
		diagnosticsType.u.type1 = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation_type1);

    if(ptr->message.u.radioBearerReleaseFailure.failureCause.u.protocolError->
		    diagnosticsType.u.type1 == NULL)
    {
      ERR_FATAL("Memory allocation failure",0,0,0);
    }
    
    /* Copy the given protocol error cause */
    ptr->message.u.radioBearerReleaseFailure.failureCause.u.protocolError->
      diagnosticsType.u.type1->protocolErrorCause = prot_err_cause;
    /*log the protocol error*/
    tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                            (uint8)TDSRRC_PROCEDURE_RBR,
                            (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                            tdsrrc_log_map_protocol_cause(prot_err_cause)
                            );
    break;

  case TDSRRCRB_MSG_INVALID_CONFIG:
    ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
    break;

  case TDSRRCRB_MSG_INCOMPLETE_CONFIG:
    ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
    break;
  
  case TDSRRCRB_CELL_UPDATE_OCCURED:
     ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_cellUpdateOccurred;
    break;

  default:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RBR Unknown Failure message");
    ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
    break;
  }

  /* Get the RLC logical channel id for this msg from Logical Channel Manager */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH,
                                         DCCH_AM_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA);

  /* If the logical channel id is not invalid, send the message to the
  send chain module */
  if (ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending RBR Failure msg to send-chain Cause:%d",ptr->message.u.radioBearerReleaseFailure.failureCause.t);

    /* Need to wait for L2 ack for all states because RRC needs to trigger include reconfig status indicator
    if RLC acknowledgement is not yet received.
    As per 8.3.1.3
    1> if the IE "Cell update cause" is set to "radio link failure" and the UE has submitted a reconfiguration response
    message to RLC and the UE has not received the RLC acknowledgement for the response message:
    2> include the IE "Reconfiguration Status Indicator" and set it to TRUE.*/
    if( tdsrrc_rbr.status.failure_status == TDSRRCRB_PHY_CHAN_FAILURE)
    {
      tdsrrc_update_reconfig_waiting_for_l2ack_dch(TDSRRC_PROCEDURE_RBR);
      status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBR,  /* Procedure is RBR    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_REQUIRED  /* ACK is required */
                                  );
    }
    else
    {
    status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBR,  /* Procedure is RBR    */
                                 (void *)ptr,        /* Message pointer     */
                                 ul_lc_id,           /* RLC id              */
                                 TDSRRCSEND_L2ACK_NOT_REQUIRED  /* No need for ACK */
                                );
    }

    if ( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Send chain failed for RBR failure msg: error cause %d",status);
    }
  }
  else
  {
    /* Unexpected failure. ERR for now. May need to ERR_FATAL here */
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't send UL DCCH msg! invalid lc id: %d",ul_lc_id ); 
    /* It's important to free the memory since we didn't send the
    message. */
    tdsrrcasn1_free_pdu(tdsrrc_UL_DCCH_Message_PDU,ptr);
  }
}/* end function tdsrrcrbr_send_rbr_failure_message */

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
/*===========================================================================
FUNCTION   tdsrrcrbr_send_lifetime_validation_failure

DESCRIPTION

  This function sends the Radio Bearer Release Failure message
  on Uplink DCCH. The failure cause must be given to this
  function. 

        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
static void tdsrrcrbr_send_lifetime_validation_failure
(
tdsrrcrb_msg_status_e_type failure_cause  /* Failure Cause - should be a subset
                                        of the failure causes specified by
                                        the spec. */

)
{
  tdsrrc_UL_DCCH_Message *ptr;     /* Pointer to uplink DCCH message type */
  rlc_lc_id_type ul_lc_id;      /* Logical Channel Id for the message  */ 
  tdsrrcsend_status_e_type status; /* Status from send chain module       */

  /* Allocate memory for this message. Note that this buffer is released
  by the SEND CHAIN */
  ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc(
                                         sizeof(tdsrrc_UL_DCCH_Message));
  if (ptr == NULL)
  {
    ERR_FATAL("No memory for RBR Failure msg!",0,0,0);
  }

  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_radioBearerReleaseFailure; /* RBR Failure msg */

  /* Set the bit-mask - neither rrc_potentiallySuccesfulBearerList_present
  nor rrc_RadioBearerReconfigurationFailure_nonCriticalExtensions_present
  are included */
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReleaseFailure);

  /* Transaction Id - First check the rejected_transacion id. If
  it's not invalid, this message is being rejected because there
  is already another message accepted but not processed.
  If the rejected_transaction id is invalid, we are rejecting the
  accepted message, so use the accepted transaction id.

  NOTE: Don't change the order, it's important to check rejected
  transaction first. */
  if ( tdsrrc_rbr.tr_ids.rejected_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.radioBearerReleaseFailure.rrc_TransactionIdentifier =
    tdsrrc_rbr.tr_ids.rejected_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_rbr.tr_ids.rejected_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  }
  else if ( tdsrrc_rbr.tr_ids.accepted_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.radioBearerReleaseFailure.rrc_TransactionIdentifier =
    tdsrrc_rbr.tr_ids.accepted_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_rbr.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  }
  else
  {
    /* Don't know the transaction id for this message.( one reason
    for this could be that the message had a critical
    extenstion). Since it's unknown, we should set it to zero. */
    ptr->message.u.radioBearerReleaseFailure.rrc_TransactionIdentifier = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Transaction id invalid for RBR Failure message!");
  }

  switch ( failure_cause )
  {
  case TDSRRCRB_MSG_UNSUPPORTED_CONFIG:
    ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
    break;

  case TDSRRCRB_PHY_CHAN_FAILURE:
    ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_physicalChannelFailure;
    break;

  case TDSRRCRB_SIMULTANEOUS_RECONFIG:
    ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_incompatibleSimultaneousReconfiguration;
    break;

  case TDSRRCRB_MSG_INVALID_CONFIG:
    ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
    break;

  case TDSRRCRB_MSG_INCOMPLETE_CONFIG:
    ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
    break;
  
  case TDSRRCRB_CELL_UPDATE_OCCURED:
     ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_cellUpdateOccurred;
    break;

  default:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RBR Unknown Failure message");
    ptr->message.u.radioBearerReleaseFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
    break;
  }

  /* Get the RLC logical channel id for this msg from Logical Channel Manager */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH,
                                         DCCH_AM_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA);

  /* If the logical channel id is not invalid, send the message to the
  send chain module */
  if (ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending RBR Failure msg to send-chain Cause:%d",ptr->message.u.radioBearerReleaseFailure.failureCause.t);

    
    status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBR,  /* Procedure is RBR    */
                                 (void *)ptr,        /* Message pointer     */
                                 ul_lc_id,           /* RLC id              */
                                 TDSRRCSEND_L2ACK_NOT_REQUIRED  /* No need for ACK */
                                );

    if ( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Send chain failed for RBR failure msg: error cause %d",status);
    }
  }
  else
  {
    /* Unexpected failure. ERR for now. May need to ERR_FATAL here */
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't send UL DCCH msg! invalid lc id: %d",ul_lc_id ); 
    /* It's important to free the memory since we didn't send the
    message. */
    tdsrrcasn1_free_pdu(tdsrrc_UL_DCCH_Message_PDU,ptr);
  }
}/* end function tdsrrcrbr_send_lifetime_validation_failure */

#endif /*FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION*/

/*===========================================================================

FUNCTION   RRCRBR_BUILD_RBR_COMPLETE_MSG

DESCRIPTION

  This function builds the Radio Bearer Release Complete message to be sent
  on Uplink DCCH. 
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
void tdsrrcrbr_build_rbr_complete_message
(
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id,  /* Transaction Identifier to be sent in Uplink */
  tdsrrc_UL_DCCH_Message *ptr,                      /* Uplink DCCH message pointer */
  boolean              srns_reloc,               /* Indicates whether SRNS reloc is involved */ 
  boolean              cipher_update_required    /* Indicates whether TM ciphering is present */
)
{
  /* Identify as RBR Complete msg */
  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_radioBearerReleaseComplete; 

  /* Fill up the message contents. The bit_mask is set to 0 since none
  of the optional IE's are included for now. */
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReleaseComplete);


  if (srns_reloc == TRUE)
  {
    tdsrrcrbr_append_start_list_to_rbr_complete(ptr);
  }
  if((cipher_update_required == TRUE) && (TRUE == tdsrrcsmc_tm_ciph_active()))
  {
    if(! (TDSRRC_MSG_COMMON_BITMASK_IE(ptr->message.u.radioBearerReleaseComplete,
           tdsrrc_RadioBearerReleaseComplete,ul_CounterSynchronisationInfo)))
    {
      tdsrrcrbr_append_start_list_to_rbr_complete(ptr);
    }
    //Set the bitmask for count_c_act_time and copy the same..
   TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->message.u.radioBearerReleaseComplete,
      tdsrrc_RadioBearerReleaseComplete,count_C_ActivationTime);
	
    ptr->message.u.radioBearerReleaseComplete.count_C_ActivationTime =
      (tdsrrc_ActivationTime)tdsrrcsmc_step_2_cipher_config_for_hho();
  }

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
  #ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
    if(tdsrrcsib_is_sib11_11bis_12_read_deferred(TDSDEFERRED_ANY_ALLOWED_SIB))
    {
      ptr->message.u.radioBearerReleaseComplete.m.laterNonCriticalExtensionsPresent=1;
 
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReleaseComplete.laterNonCriticalExtensions);
      
      ptr->message.u.radioBearerReleaseComplete.laterNonCriticalExtensions.
	  	m.v770NonCriticalExtensionsPresent=1;
      
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReleaseComplete.laterNonCriticalExtensions.
	  	v770NonCriticalExtensions);
 
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReleaseComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        radioBearerReleaseComplete_v770ext);
      
      ptr->message.u.radioBearerReleaseComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        radioBearerReleaseComplete_v770ext.m.deferredMeasurementControlReadingPresent=1;
      
      ptr->message.u.radioBearerReleaseComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        radioBearerReleaseComplete_v770ext.deferredMeasurementControlReading = 
        tdsrrc_RadioBearerReleaseComplete_v770ext_IEs_deferredMeasurementControlReading_true_;
    }
#endif
  }

  ptr->message.u.radioBearerReleaseComplete.rrc_TransactionIdentifier =
  tdstransaction_id;
  /* We no longer need to keep this id. Make rejected_transaction id
  invalid again */
  //rbr.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;

}/* end function tdsrrcrbr_build_rbr_complete_message */


/*===========================================================================

FUNCTION   SEND_RBR_COMPLETE_MSG

DESCRIPTION

  This function sends the Radio Bearer Release Complete message
  on Uplink DCCH. 
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
static void tdsrrcrbr_send_rbr_complete_message( boolean proc_wait_for_l2_ack )
{

  tdsrrc_UL_DCCH_Message *ptr;     /* Pointer to uplink DCCH message type */
  rlc_lc_id_type ul_lc_id;      /* Logical Channel Id for the message  */ 
  tdsrrcsend_status_e_type status; /* Status from send chain module       */

  /* Allocate memory for this message. Note that this buffer is released
  by the SEND CHAIN */
  ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc(
                                         sizeof(tdsrrc_UL_DCCH_Message));
  if (ptr == NULL)
  {
    ERR_FATAL("No memory for RBR Complete msg!",0,0,0);
  }

  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_radioBearerReleaseComplete; /* RBR Complete msg */

  /* Fill up the message contents. The bit_mask is set to 0 since none
  of the optional IE's are included for now. */
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReleaseComplete); 

  if (tdsrrc_rbr.rbr_for_srns_relocation == TRUE)
  {
    tdsrrcrbr_append_start_list_to_rbr_complete(ptr);
  }
  else
  {
    if (tdsrrcllc_get_rlc_size_change_status() 
      && ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH || 
             tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
          || (tdsrrc_get_rlc_size_change_status_for_procedure_id(TDSRRC_PROCEDURE_RBR) == TRUE)))
    {
      // Check if next state is CELL+PCH, and OC API returns RLC size change, then include counter sync
      if (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH || 
          tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
      {
        /* Ask SMC to do start value calculation for PS domain */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RLCSIZEINDICATED Invoking SMC extract func");
        (void)tdsrrcsmc_extract_return_hfn_per_domain_and_update_start_flag(RRC_PS_DOMAIN_CN_ID);
      }
      else if (tdsrrc_get_rlc_size_change_status_for_procedure_id(TDSRRC_PROCEDURE_RBR) == TRUE)
      {
        tdsrrc_set_rlc_size_change_status(TDSRRC_PROCEDURE_NONE, 
                                       FALSE, TDSRRC_INVALID_RB_ID ,
                                       TDSRLC_RE_ESTABLISH_NONE);
      }

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsize change append start value");
      /*start list is included in ul_counter_synchronization_info. So enable this IE in the
      bitmask */
      TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->message.u.radioBearerReleaseComplete,
        tdsrrc_RadioBearerReleaseComplete,ul_CounterSynchronisationInfo);

	 TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReleaseComplete.ul_CounterSynchronisationInfo);

      
     /* call SMC API to append the start list */
     if (SUCCESS != tdsrrcsmc_append_start_list(&ptr->message.u.radioBearerReleaseComplete.ul_CounterSynchronisationInfo.startList,
                           TDSRRCSMC_EXTRACT_AND_APPEND_START))
     {
       /*should not come here */
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBR: Could not append Start list");
     }
    }
  }

  //if this is case of IFHHO and not SRNS relocation(i.e. ul counter sync is not present)
  //then also send START_List and set count c act time and configure smc for Step_2

  if ( ((tdsrrc_rbr.hho_ciph_status == TRUE) || (tdsrrc_rbr.trans_to_dch_ciph_config_needed == TRUE)) &&
       (TRUE == tdsrrcsmc_tm_ciph_active()))
  {
    if(! (TDSRRC_MSG_COMMON_BITMASK_IE(ptr->message.u.radioBearerReleaseComplete,
           tdsrrc_RadioBearerReleaseComplete,ul_CounterSynchronisationInfo)))
    {
      tdsrrcrbr_append_start_list_to_rbr_complete(ptr);
    }
  
    //Set the bitmask for count_c_act_time and copy the same..
    TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->message.u.radioBearerReleaseComplete,
      tdsrrc_RadioBearerReleaseComplete,count_C_ActivationTime);
    
    ptr->message.u.radioBearerReleaseComplete.count_C_ActivationTime =
      (tdsrrc_ActivationTime)tdsrrcsmc_step_2_cipher_config_for_hho();
  
    //Send step 2 config for HHO.. only one of these flags will be active, reset both of them
    //here
    if(tdsrrc_rbr.hho_ciph_status)
    {
      tdsrrc_rbr.hho_ciph_status =  FALSE;      
    }
    else if(tdsrrc_rbr.trans_to_dch_ciph_config_needed)
    {
      tdsrrc_rbr.trans_to_dch_ciph_config_needed = FALSE;      
    }
  }

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
    if(tdsrrcsib_is_sib11_11bis_12_read_deferred(TDSDEFERRED_ANY_ALLOWED_SIB))
    {
      ptr->message.u.radioBearerReleaseComplete.m.laterNonCriticalExtensionsPresent=1;

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReleaseComplete.
	  	laterNonCriticalExtensions);
       
      ptr->message.u.radioBearerReleaseComplete.laterNonCriticalExtensions.m.
	  	v770NonCriticalExtensionsPresent=1;
       
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReleaseComplete.laterNonCriticalExtensions.
	  	v770NonCriticalExtensions);

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerReleaseComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        radioBearerReleaseComplete_v770ext);
       
      ptr->message.u.radioBearerReleaseComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        radioBearerReleaseComplete_v770ext.m.deferredMeasurementControlReadingPresent=1;
       
      ptr->message.u.radioBearerReleaseComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        radioBearerReleaseComplete_v770ext.deferredMeasurementControlReading = 
        tdsrrc_RadioBearerReleaseComplete_v770ext_IEs_deferredMeasurementControlReading_true_;
    }
#endif
  }

  /* Transaction Id - use the accepted transaction id if known. Else
  use zero. Since this is a RBR Complete message, we don't check 
  rejected transaction ids. */
  if ( tdsrrc_rbr.tr_ids.accepted_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.radioBearerReleaseComplete.rrc_TransactionIdentifier =
    tdsrrc_rbr.tr_ids.accepted_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again unless proc is waiting for L2ACK*/
    if (!proc_wait_for_l2_ack)
    {
      tdsrrc_rbr.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
    }
  }
  else
  {
    /* Don't know the transaction id for this message.( one reason
    for this could be that the message had a critical
    extenstion). Since it's unknown, we should set it to zero. */
    ptr->message.u.radioBearerReleaseComplete.rrc_TransactionIdentifier = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Transaction id invalid for RBR Complete message!");
  }

  /* For now, there is nothing else to include in this message. We
  can give it to the send chain. */

  /* Get the RLC logical channel id for this msg from Logical Channel Manager */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH,
                                         DCCH_AM_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA);

  /* If the logical channel id is not invalid, send the message to the
  send chain module */
  if (ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RBR Complete message to send-chain");
    if (proc_wait_for_l2_ack) 
    {
      status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBR,  /* Procedure is RBR    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_REQUIRED  /* ACK is required */
                                   );
    }
    else
    {
      status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBR,  /* Procedure is RBR    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_NOT_REQUIRED  /* ACK is required */
                                   );
    }

    if ( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Send chain failed for RBR complete msg: error cause %d",status);
    }
    else /* Send is successful */
    {
    }
  }
  else
  {
    /* Unexpected failure. ERR for now. May need to ERR_FATAL here */
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't send UL DCCH msg! invalid lc id: %d",ul_lc_id ); 
    /* It's important to free the memory since we didn't send the
    message. */
    tdsrrcasn1_free_pdu(tdsrrc_UL_DCCH_Message_PDU, ptr);
  }
  tdsrrclog_send_rb_rate_info_packet();
  if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
  {
    if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
    {      
      (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
    }
    else
    {
      (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
    }
    tdsrrc_rbr.need_to_release_cn_domain = FALSE;
  }
}/* end function tdsrrcrbr_send_rbr_complete_message */


/*===========================================================================

FUNCTION tdsrrcrbr_is_pccpch_info_present

DESCRIPTION
  This function checks whether cell parameter id info is present in
  the message or not.

DEPENDENCIES
  None.

RETURN VALUE
              Boolean.
                 TRUE  : cell parameter id info is present in the message
                 FALSE : cell parameter id info is not present in the message

===========================================================================*/
static boolean  tdsrrcrbr_is_pccpch_info_present
(
  tdsrrc_RadioBearerRelease  *msg_ptr,
  tdsrrc_msg_ver_enum_type   msg_version
)
{

  /*this function is used to find whether directed cell info is present or not during Cell_DCH to Cell_FACH 
   * state transition. It is safe to assume that the rl list will have only one element.
   */
   
#ifdef FEATURE_TDSCDMA_REL9
  if(msg_version == TDSMSG_REL9)
  {
    tdsrrc_RadioBearerRelease_r9_IEs *r9_rb_rel_msg_ptr  =
        &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
        u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.radioBearerRelease_r9;
    
    if (r9_rb_rel_msg_ptr->m.dl_InformationPerRL_ListPresent)
    {
      if(r9_rb_rel_msg_ptr->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd)
      { 
        if (r9_rb_rel_msg_ptr->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBR message %d",
                  r9_rb_rel_msg_ptr->dl_InformationPerRL_List.
                  elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }
  else
#endif /* FEATURE_TDSCDMA_REL9 */
#ifdef FEATURE_TDSCDMA_REL8
  if(msg_version == TDSMSG_REL8)
  {
    tdsrrc_RadioBearerRelease_r8_IEs *r8_rb_rel_msg_ptr  =
        &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
        u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerRelease_r8;
    
    if (r8_rb_rel_msg_ptr->m.dl_InformationPerRL_ListPresent)
    {
      if(r8_rb_rel_msg_ptr->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd)
      { 
        if (r8_rb_rel_msg_ptr->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBR message %d",
                  r8_rb_rel_msg_ptr->dl_InformationPerRL_List.
                  elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }
  else
#endif /*FEATURE_TDSCDMA_REL8*/
  if(msg_version == TDSMSG_REL7)
  {
    tdsrrc_RadioBearerRelease_r7_IEs *r7_rb_rel_msg_ptr  =
        &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions
        .u.criticalExtensions.u.r7.radioBearerRelease_r7;
    
    if (r7_rb_rel_msg_ptr->m.dl_InformationPerRL_ListPresent)
    {
      if(r7_rb_rel_msg_ptr->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_tdd)
      { 
        if (r7_rb_rel_msg_ptr->dl_InformationPerRL_List.
                elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBR message %d",
                 r7_rb_rel_msg_ptr->dl_InformationPerRL_List.
               elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }
  else
  if(msg_version == TDSMSG_REL6)
  {
    tdsrrc_RadioBearerRelease_r6_IEs *r6_rb_rel_msg_ptr  =
      &msg_ptr->u.later_than_r3.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.r6.radioBearerRelease_r6;

    if (r6_rb_rel_msg_ptr->m.dl_InformationPerRL_ListPresent)
    {
      if(r6_rb_rel_msg_ptr->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
         T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_tdd)
      { 
        if (r6_rb_rel_msg_ptr->dl_InformationPerRL_List.
              elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBR message %d",
           r6_rb_rel_msg_ptr->dl_InformationPerRL_List.
           elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }
  else
  if(msg_version == TDSMSG_REL5)
  {
    tdsrrc_RadioBearerRelease_r5_IEs *r5_rb_rel_msg_ptr  =
      &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.radioBearerRelease_r5;

    if (r5_rb_rel_msg_ptr->m.dl_InformationPerRL_ListPresent)
    {
      if(r5_rb_rel_msg_ptr->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
           T_tdsrrc_DL_InformationPerRL_r5_modeSpecificInfo_tdd)
      { 
        if (r5_rb_rel_msg_ptr->dl_InformationPerRL_List.
                    elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBR message %d",
             r5_rb_rel_msg_ptr->dl_InformationPerRL_List.
             elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
            return TRUE;
        }
      }
    }
  }
  else
  {
    tdsrrc_RadioBearerRelease_r4_IEs *r4_rb_rel_msg_ptr  =
         &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerRelease_r4;

    if (r4_rb_rel_msg_ptr->m.dl_InformationPerRL_ListPresent)
    {
      if(r4_rb_rel_msg_ptr->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
           T_tdsrrc_DL_InformationPerRL_r4_modeSpecificInfo_tdd)
      { 
        if (r4_rb_rel_msg_ptr->dl_InformationPerRL_List.
                elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBR message %d",
                 r4_rb_rel_msg_ptr->dl_InformationPerRL_List.
                 elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
  
}  /* tdsrrcrbr_is_pccpch_info_present */


/*===========================================================================

FUNCTION   RRCRBR_VALIDATE_SRNS_RELOCATION_INFO()

DESCRIPTION

  This function checks whether SRNS relocation info is present or not
        
DEPENDENCIES

  None.
 
RETURN VALUE

  Boolean. TRUE : SRNS relocation info is present
           FALSE : SRNS relocation info is not present

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrbr_validate_srns_relocation_info
(
tdsrrc_RadioBearerRelease *msg_ptr      /* Pointer to the RB Setup message */
,tdsrrc_msg_ver_enum_type msg_version
)
{
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Validate if RBR is for srns relocation");
  if (TRUE == tdsrrc_is_fresh_in_reconfiguration_message())
  {
    return TRUE;
  }
#endif
   
  if (msg_version == TDSMSG_REL5)
  {
    if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.
		u.criticalExtensions.u.r5.radioBearerRelease_r5,
      tdsrrc_RadioBearerRelease_r5_IEs,dl_CounterSynchronisationInfo))
    {
      return TRUE;
    } 
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                 msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.radioBearerRelease_r5,
                                               new_U_RNTI))
    { /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.
             radioBearerRelease_r5.new_U_RNTI)) == FALSE)
      {
        return TRUE;
      }
    }
#endif
    else
    {
      return FALSE;
    }
  }
  if (msg_version == TDSMSG_REL6)
  {
    if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u. 
         criticalExtensions.u.r6.radioBearerRelease_r6,
         tdsrrc_RadioBearerRelease_r6_IEs,dl_CounterSynchronisationInfo)) 
    { 
      return TRUE; 
    } 
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
         msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
           criticalExtensions.u.r6.radioBearerRelease_r6, new_U_RNTI))
    {
      /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
           criticalExtensions.u.r6.radioBearerRelease_r6.new_U_RNTI)) == FALSE)
      {
        return TRUE;
      }
    }
#endif
    else
    {
      return FALSE;
    }
  }

  if (msg_version == TDSMSG_REL7)
  {
    if (TDSRRCRBR_R7_MSG_IE_PRESENT((&msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u. 
         criticalExtensions.u.r7.radioBearerRelease_r7),tdsrrc_RadioBearerRelease_r7_IEs,
         dl_CounterSynchronisationInfo)) 
    { 
      return TRUE; 
    } 
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
         msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
           criticalExtensions.u.r7.radioBearerRelease_r7, new_U_RNTI))
    {
      /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
           criticalExtensions.u.r7.radioBearerRelease_r7.new_U_RNTI)) == FALSE)
      {
        return TRUE;
      }
    }
#endif
    else
    {
      return FALSE;
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
  if (msg_version == TDSMSG_REL8)
  {
    if(TDSRRCRB_R8_MSG_IE_PRESENT((&msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerRelease_r8),tdsrrc_RadioBearerRelease_r8_IEs,
        dl_CounterSynchronisationInfo))
    { 
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: dl_CounterSynchronisationInfo present");    
      return TRUE; 
    }
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
             msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
               criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerRelease_r8, new_U_RNTI))
    {
      /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
               criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerRelease_r8.new_U_RNTI)) == FALSE)
      {
        return TRUE;
      }
    }
#endif
    else
    {
      return FALSE;
    }
  }
#endif /* FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
  if (msg_version == TDSMSG_REL9)
  {
    if(TDSRRCRB_R9_MSG_IE_PRESENT((&msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.radioBearerRelease_r9),tdsrrc_RadioBearerRelease_r9_IEs,
        dl_CounterSynchronisationInfo))
    { 
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: dl_CounterSynchronisationInfo present");    
      return TRUE; 
    }
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
             msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
              criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.radioBearerRelease_r9, new_U_RNTI))
    {
      /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
            criticalExtensions.u.criticalExtensions.u.criticalExtensions.
                                               u.criticalExtensions.u.r9.radioBearerRelease_r9.new_U_RNTI)) == FALSE)
      {
        return TRUE;
      }
    }
#endif
    else
    {
      return FALSE;
    }
  }
#endif /* FEATURE_TDSCDMA_REL9*/
  else if (msg_version == TDSMSG_REL4)
  {
  if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerRelease_r4,
    tdsrrc_RadioBearerRelease_r4_IEs,dl_CounterSynchronisationInfo))

  {
     return TRUE;
  }
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerRelease_r4, new_U_RNTI))
       {
          /*check if SRNC-ID changed*/
          if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.
                                                   u.r4.radioBearerRelease_r4.new_U_RNTI)) == FALSE)
          {
            return TRUE;
          }
        }
#endif
  else 
  {
    return FALSE;
  }
}

  return FALSE;
}

 /*===========================================================================
FUNCTION   tdsrrcrbr_configure_rab_change()

DESCRIPTION
 This function re-configures RABs and other lower layer parameters for VT -> Voice 
 and Voice -> VT fallback for RB-Release OTA msg. Also, if SCUDIF is not happening, then
 this function checks/configures for vocoder/rate change for Voice RAB. 
        
DEPENDENCIES
  None.
 
RETURN VALUE
  TRUE or FALSE

SIDE EFFECTS
  None.
===========================================================================*/
static boolean tdsrrcrbr_configure_rab_change
(
  uint8 rab_idx
)
{
#ifdef FEATURE_SCUDIF_ENHANCEMENT
  #error code not present
#endif/* FEATURE_SCUDIF_ENHANCEMENT*/
  mm_cmd_type *mm_ptr = NULL;   /* To send the MM_SYNC_IND command */

  if(rab_idx >= TDSRRCRB_MAX_RAB_PER_MSG)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RAB idx passed %d",rab_idx);
    return FALSE;
  }
#ifdef FEATURE_SCUDIF_ENHANCEMENT
  #error code not present
#endif/* FEATURE_SCUDIF_ENHANCEMENT*/
  {

    if(tdsrrc_rbr.rabs.rabs_in_msg[rab_idx].rab_type == TDSRRCRB_CS_VOICE_RAB)
    {
      tdsrrcrb_update_amr_cctrch_id_for_reconfig(
      tdsrrc_rbr.rabs.rabs_in_msg[rab_idx].nas_Synchronisation_Indicator
      );

    }
    if ((mm_ptr = mm_tdsrrc_get_cmd_buf(RRC_SYNC_IND)) == NULL)
    {
      ERR_FATAL("Can't send MM cmd: 0x%x  Out of memory!",RRC_SYNC_IND,0,0);
    }
    mm_ptr->cmd.rrc_sync_ind.cn_domain_id = RRC_CS_DOMAIN_CN_ID;
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
    mm_ptr->cmd.rrc_sync_ind.rab_info.rab_id =  tdsrrc_rbr.rabs.rabs_in_msg[rab_idx].rab_id;
  
    if(tdsrrc_rbr.rabs.rabs_in_msg[rab_idx].rab_type == TDSRRCRB_CS_VOICE_RAB)
    {
      mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_CS_VOICE_CALL;
    }
    else if(tdsrrc_rbr.rabs.rabs_in_msg[rab_idx].rab_type == TDSRRCRB_CS_DATA_RAB)
    {
      mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_CS_DATA_CALL;        
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Something wrong with RAB Type");      
      mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_INVALID_RAB_ID;
    }
    mm_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_NONE;
    if(mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type == RRCMN_CS_VOICE_CALL)
    {
      /* Fill in the codec type used in RRC_SYNC_IND*/
      if(tdsrrc_rbr.rabs.rabs_in_msg[rab_idx].nas_Synchronisation_Indicator == TDSCDMA_IVOCODER_ID_AMRWB)
      {
        mm_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_WB_AMR;
      }
      else
      {
        mm_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_NB_AMR;
      }
    }
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Codec %d in RRC_SYNC_IND",mm_ptr->cmd.rrc_sync_ind.rab_info.codec);   

#ifdef FEATURE_UMTS_VOICE_CIPHERING_IND
    #error code not present
#endif

#ifdef FEATURE_DUAL_SIM
    mm_ptr->cmd.rrc_sync_ind.as_id = tdsrrc_get_as_id();
#endif
    /* Put the command on MM queue */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sending RRC_SYNC_IND to MM for RAB type %d RAB-id %d",mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type,mm_ptr->cmd.rrc_sync_ind.rab_info.rab_id);
  
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(mm_ptr);
    }
    mm_put_cmd(mm_ptr);
  }
#ifdef FEATURE_SCUDIF_ENHANCEMENT
  #error code not present
#else
  return FALSE;
#endif
}

/*===========================================================================

FUNCTION   tdsrrcrbr_update_rab_info_reconfiglist

DESCRIPTION Updates the RAB information received in the RAB_InformationReconfigList IE in 
                      RB Release message.
                      It updates the NAS Syncronisation Indicator field received in the RB Release message.
                      In case the NAS Syncronisation Indicator IE is not present , then a default value of 
                      current codec  used is updated.



DEPENDENCIES
  None.

RETURN VALUE
 None

SIDE EFFECTS
  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrcrbr_update_rab_info_reconfiglist
(
  tdsrrc_RAB_InformationReconfigList *rab_list_ptr
)
{
  uint32 rab_index=0;  
  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();

  rrc_rab_id_type rab_id;           /* To store RAB id in internal RRC format */
  rrcmn_rab_status_T rab_type = RRCMN_INVALID_RAB_ID;     /* To store the status returned by MN layer. This
                                    indicates what type of RAB is received. This is
                                    used only if the RAB is for the CS_domain */
                                    
  if(rab_list_ptr != NULL)
  {
    for (rab_index=0; (rab_list_ptr->n > rab_index) && (rab_index < TDSRRCRB_MAX_RAB_PER_MSG); rab_index++)
    {
      rab_id = 
          tdsrrcrb_translate_gsm_map_rab_id((rab_list_ptr->elem[rab_index].rab_Identity.u.gsm_MAP_RAB_Identity));
                                            
  
      /* Store this info locally so that ESTABLISHED_RABS can
      be updated later, when these RABs are established */
      tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_id_is_gsm_map = TRUE;
      tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_id = rab_id;
      
      /* Check if this RAB is CS Domain or PS Domain */  
      if(rab_list_ptr->elem[rab_index].cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
      {
        /* Check if it is in loopback test */
        if ( tdsrrclbt_is_lb_on() == TRUE )
        {
          /* Set Rab type to Data */
          TDSRRC_MSG0(MSG_LEGACY_LOW,"Loopback Test");
          tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
        }
        else
        {
           /* For CS RABs, Call MN layer to verify this RAB */
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
          rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
          rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif
          tdsrrc_rbr.rabs.rabs_in_msg[rab_index].cn_domain =RRC_CS_DOMAIN_CN_ID;

          TDSRRC_MSG3(MSG_LEGACY_HIGH,"For RAB-Id %d, RAB type from NAS = %d, RAB type from EST_RABs = %d",rab_id, rab_type,
            tdsrrc_get_rab_type_for_given_rab_id(tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_id));

          if( rab_type == RRCMN_CS_VOICE_CALL )
          {
            tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_VOICE_RAB;
          }
          else
          {
            tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
          }
          
#ifdef FEATURE_SCUDIF_ENHANCEMENT
          #error code not present
#else
          if(rab_type == RRCMN_CS_VOICE_CALL)
#endif
          {
            nas_sync_ind =  tdsrrc_translate_nas_sync_indicator(&(rab_list_ptr->elem[rab_index].nas_Synchronisation_Indicator));
            if( nas_sync_ind == TDSCDMA_IVOCODER_ID_NONE)
            {
              tdsrrc_rbr.status.failure_status= TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported configuration .. codec not supported %d",nas_sync_ind );
              return FAILURE;
            }
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
            tdsrrc_set_ordered_codec_status(TRUE);
#endif
          }
        }   
      }
      else
      {
        tdsrrc_rbr.rabs.rabs_in_msg[rab_index].cn_domain =RRC_PS_DOMAIN_CN_ID;
        tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_PS_DATA_RAB;
      }
      tdsrrc_rbr.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator = nas_sync_ind;
     
    } 
  }
  
  tdsrrc_rbr.rabs.num_rabs_in_msg = rab_index;
  tdsrrc_set_ordered_codec_mode(nas_sync_ind);
  return SUCCESS;
}



/*===========================================================================

FUNCTION   VALIDATE_RBRELEASE_R7

DESCRIPTION

  This function validates the received Rel 7 RB Release Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Release Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_validate_rbrelease_r7
(
tdsrrc_RadioBearerRelease *msg_ptr      /* Pointer to the RB Release message */
)
{
  tdsrrc_RadioBearerRelease_r7_IEs* rb_release_ptr = NULL;
  
  /* Assume the message is valid for now */
  tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_VALID;
  
 
  rb_release_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
    u.criticalExtensions.u.r7.radioBearerRelease_r7;
  
  
  /* Check the RRC State indicatior given by the message */
  switch (rb_release_ptr->rrc_StateIndicator )
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;
    
    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;
    
    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;
    
    default:
      /* No other states are supported right now. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported state in RB Release Message");
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;
  }
  
  if ( tdsrrc_rbr.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
    return;
  }

  /* Validate DL HSPDSCH */
  if ( TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,dl_HSPDSCH_Information))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSPDSCH Info present");
    /* Check if network does not try to initiate HSDPA with invalid state */
    if (tdsrrc_hsdpa_initial_validation(rb_release_ptr->rrc_StateIndicator, 
                                      TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
                                        rrc_DL_HSPDSCH_Information_r7_modeSpecificInfo_fdd) ) == FAILURE)
    {
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }
  
  /* Check if there are new RABs to be reconfigured
  This is not supported now. */
  /*if (rb_release_ptr->bit_mask & rrc_RadioBearerRelease_r6_IEs_rab_InformationReconfigList_present)
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RAB Info to reconfigure present in RBR Message");
  } */

  if ( rb_release_ptr->rb_InformationReleaseList.n == 0 )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBInfoReleaselist is absent in RBR Message");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
    tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
    return;
  }

  /*if ( rb_release_ptr->bit_mask & rrc_RadioBearerRelease_r5_IEs_rb_InformationAffectedList_present )
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RB Info Affected list present in RBR Message");
  }*/


  if(TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,rb_InformationReconfigList))
  {
    /* Save the linked list head pointer */
    if (FAILURE == tdsrrcllc_validate_rb_info_reconfig_list_r7(
        &rb_release_ptr->rb_InformationReconfigList))
    {
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }

    if(TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,rab_InformationReconfigList))
    {
      if(tdsrrcrbr_update_rab_info_reconfiglist(&rb_release_ptr->rab_InformationReconfigList)
         == FAILURE)
      {
        tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        return;
      }
    }
    else
    {
      /* Set the Ordered codec mode with the current active codec */
      tdsrrc_set_ordered_codec_mode(tdsrrcdata_codec_info.rrc_active_codec);
      tdsrrc_rbr.rabs.num_rabs_in_msg =0;
    }

  }
  
  /* Check if RB with PDCP Information list is present. This
  is not supported in the first release. This may be supported
  later */

  
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if (TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,dl_CounterSynchronisationInfo))
#else
    if (tdsrrcrbr_validate_srns_relocation_info(msg_ptr, TDSMSG_REL7))
#endif
  {
    /*check whether the SRNS relocation is within Cell_DCH or not.
    SRNS relocation information can come in reconfiguration messages
    only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
      ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbr.state_indicated_by_msg);
      tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has 
    PDCP info list. If it is true then this configuration is unsupported */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rb_release_ptr->dl_CounterSynchronisationInfo,
         rb_WithPDCP_InfoList)) ||
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_release_ptr->dl_CounterSynchronisationInfo,
         tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
      tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation. 
         Set the flag that indicates that this procedure
         involves SRNS relocation */
      tdsrrc_rbr.rbr_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */

      if ( TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,new_U_RNTI))
      {
        tdsrrc_rbr.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(rb_release_ptr->new_U_RNTI),
          &tdsrrc_rbr.new_urnti);
      }
    }
  } /* dl counter sync info */

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  if((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    
    if((TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,dl_CommonInformation)) 
        && (TDSRRC_MSG_COMMON_BITMASK_IE(rb_release_ptr->dl_CommonInformation, 
        tdsrrc_DL_CommonInformation_r7,dl_dpchInfoCommon)) &&
       (rb_release_ptr->dl_CommonInformation.dl_dpchInfoCommon.t == 
        T_tdsrrc_DL_CommonInformation_r7_dl_dpchInfoCommon_dl_DPCH_InfoCommon))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");

        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon,
              tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
             &rb_release_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
             &mac_d_hfn_l);

        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Process Ciphering Config for HHO failed");
        }
        tdsrrc_rbr.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }
  

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
    (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
    /* If UTRAN DRX coefficient is not present then it is invalid configuration */
    
    if (!( TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_rbr.state_indicated_by_msg);
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    if (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      /*should we do it after getting l2ack for response??*/
     
      if (!( TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,ura_Identity)))
        
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
        tdsrrccu_clear_ura_id ();
      }
      else
      {
        /*URA ID present in the message set ura_identity variable with this value */
        if (rb_release_ptr->ura_Identity.numbits == 16)
        {
          tdsrrccu_update_ura_id (&(rb_release_ptr->ura_Identity));
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rb_release_ptr->ura_Identity.numbits);
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
          tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
          return;
        }
        
      }
      
    }
  }
 
  /* Reject if any MBMS info is present */
  if (TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,mbms_PL_ServiceRestrictInfo))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MBMS info not supported in RBR");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
  }

}/* end function tdsrrcrbr_validate_rbrelease_r7 */




/*===========================================================================

FUNCTION   VALIDATE_RBRELEASE_R6

DESCRIPTION

  This function validates the received Rel 5 RB Release Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Release Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_validate_rbrelease_r6
(
tdsrrc_RadioBearerRelease *msg_ptr      /* Pointer to the RB Release message */
)
{
  tdsrrc_RadioBearerRelease_r6_IEs* rb_release_ptr = NULL;
  
  /* Assume the message is valid for now */
  tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_VALID;
  
  
  rb_release_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
    u.criticalExtensions.u.r6.radioBearerRelease_r6;
  
  
  /* Check the RRC State indicatior given by the message */
  switch (rb_release_ptr->rrc_StateIndicator )
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;
    
    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;
    
    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;
    
    default:
      /* No other states are supported right now. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported state in RB Release Message");
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;
  }
  
  if ( tdsrrc_rbr.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
    return;
  }

  /* Validate DL HSPDSCH */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r6_IEs,dl_HSPDSCH_Information))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSPDSCH Info present");
    /* Check if network does not try to initiate HSDPA with invalid state */
    if (tdsrrc_hsdpa_initial_validation_r6(rb_release_ptr->rrc_StateIndicator, 
                                        &rb_release_ptr->dl_HSPDSCH_Information) == FAILURE)
    {
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }
  
  /* Check if there are new RABs to be reconfigured
  This is not supported now. */
  /*if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r6_IEs,rab_InformationReconfigList))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RAB Info to reconfigure present in RBR Message");
  } */

  if ( rb_release_ptr->rb_InformationReleaseList.n == 0 )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBInfoReleaselist is absent in RBR Message");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
    tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
    return;
  }

  /*if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, rrc_RadioBearerRelease_r5_IEs_rb_InformationAffectedList) )
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RB Info Affected list present in RBR Message");
  }*/


  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
               tdsrrc_RadioBearerRelease_r6_IEs,rb_InformationReconfigList))
  {
    /* Save the linked list head pointer */
    if (FAILURE == tdsrrcllc_validate_rb_info_reconfig_list_r6(
        &rb_release_ptr->rb_InformationReconfigList))
    {
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r6_IEs,rab_InformationReconfigList))
    {
      if(tdsrrcrbr_update_rab_info_reconfiglist(&rb_release_ptr->rab_InformationReconfigList)
         == FAILURE)
      {
        tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        return;
      }
    }
    else
    {
      /* Set the Ordered codec mode with the current active codec */
      tdsrrc_set_ordered_codec_mode(tdsrrcdata_codec_info.rrc_active_codec);
      tdsrrc_rbr.rabs.num_rabs_in_msg =0;
    }

  }
  
  /* Check if RB with PDCP Information list is present. This
  is not supported in the first release. This may be supported
  later */
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
    tdsrrc_RadioBearerRelease_r6_IEs,dl_CounterSynchronisationInfo))
#else
    if (tdsrrcrbr_validate_srns_relocation_info(msg_ptr, TDSMSG_REL6))
#endif

  {
    /*check whether the SRNS relocation is within Cell_DCH or not.
    SRNS relocation information can come in reconfiguration messages
    only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
      ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbr.state_indicated_by_msg);
      tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has 
    PDCP info list. If it is true then this configuration is unsupported */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rb_release_ptr->dl_CounterSynchronisationInfo,
         rb_WithPDCP_InfoList)) ||
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_release_ptr->dl_CounterSynchronisationInfo,
         tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
      tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation. 
         Set the flag that indicates that this procedure
         involves SRNS relocation */
      tdsrrc_rbr.rbr_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
          tdsrrc_RadioBearerRelease_r6_IEs,new_U_RNTI))
      {
        tdsrrc_rbr.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(rb_release_ptr->new_U_RNTI),
          &tdsrrc_rbr.new_urnti);
      }
    }
  } /* dl counter sync info */

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  if((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
        tdsrrc_RadioBearerRelease_r6_IEs,dl_CommonInformation)) && 
       (TDSRRC_MSG_COMMON_BITMASK_IE(rb_release_ptr->dl_CommonInformation,
        tdsrrc_DL_CommonInformation_r6,dl_dpchInfoCommon)) &&
       (rb_release_ptr->dl_CommonInformation.dl_dpchInfoCommon.t == 
        T_tdsrrc_DL_CommonInformation_r6_dl_dpchInfoCommon_dl_DPCH_InfoCommon))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");

        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr->dl_CommonInformation.dl_dpchInfoCommon.
             u.dl_DPCH_InfoCommon,tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
             &rb_release_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
             &mac_d_hfn_l);

        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Process Ciphering Config for HHO failed");
        }
        tdsrrc_rbr.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }
  

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
    (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
    /* If UTRAN DRX coefficient is not present then it is invalid configuration */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
      tdsrrc_RadioBearerRelease_r6_IEs,utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_rbr.state_indicated_by_msg);
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    if (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      /*should we do it after getting l2ack for response??*/
      if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
        tdsrrc_RadioBearerRelease_r6_IEs,ura_Identity)))
        
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
        tdsrrccu_clear_ura_id ();
      }
      else
      {
        /*URA ID present in the message set ura_identity variable with this value */
        if (rb_release_ptr->ura_Identity.numbits == 16)
        {
          tdsrrccu_update_ura_id (&(rb_release_ptr->ura_Identity));
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rb_release_ptr->ura_Identity.numbits);
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
          tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
          return;
        }
        
      }
      
    }
  }
  /* Reject if any MBMS info is present */
  if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
      tdsrrc_RadioBearerRelease_r6_IEs,mbms_PL_ServiceRestrictInfo)) ||
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
      tdsrrc_RadioBearerRelease_r6_IEs,mbms_PL_ServiceRestrictInfo)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MBMS info not supported in RBR");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
  }


}/* end function tdsrrcrbr_validate_rbrelease_r6 */


/*===========================================================================

FUNCTION   VALIDATE_RBRELEASE_R5

DESCRIPTION

  This function validates the received Rel 5 RB Release Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Release Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_validate_rbrelease_r5
(
tdsrrc_RadioBearerRelease *msg_ptr      /* Pointer to the RB Release message */
)
{
  tdsrrc_RadioBearerRelease_r5_IEs* rb_release_ptr = NULL;
  
  /* Assume the message is valid for now */
  tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_VALID;
  
  
  rb_release_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
    u.r5.radioBearerRelease_r5;
  
  
  /* Check the RRC State indicatior given by the message */
  switch ( rb_release_ptr->rrc_StateIndicator )
  {
  case tdsrrc_RRC_StateIndicator_cell_DCH:
    tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
    break;
    
  case tdsrrc_RRC_StateIndicator_cell_FACH:
    tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
    break;
  case tdsrrc_RRC_StateIndicator_cell_PCH:
    tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
    break;
    
  case tdsrrc_RRC_StateIndicator_ura_PCH:
    tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
    break;
    
  default:
    /* No other states are supported right now. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported state in RB Release Message");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    break;
  }
  
  if ( tdsrrc_rbr.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
    return;
  }

  /* Validate DL HSPDSCH */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r5_IEs,dl_HSPDSCH_Information))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSPDSCH Info present");

    /* Check if network does not try to initiate HSDPA with invalid state */
    if (tdsrrc_hsdpa_initial_validation(rb_release_ptr->rrc_StateIndicator, 
                TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
                   rrc_DL_HSPDSCH_Information_modeSpecificInfo_fdd ) ) == FAILURE)
    {
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  /* Check if there are new RABs to be reconfigured
  This is not supported now. */
 
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r5_IEs,rab_InformationReconfigList))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RAB Info to reconfigure present in RBR Message");
    if(tdsrrcrbr_update_rab_info_reconfiglist(&rb_release_ptr->rab_InformationReconfigList)
       == FAILURE)
    {
      return;
    }
  }
  else
  {
    tdsrrc_rbr.rabs.num_rabs_in_msg =0;
    /* Set the Ordered codec mode with the current active codec */
    tdsrrc_set_ordered_codec_mode(tdsrrcdata_codec_info.rrc_active_codec);
  }
  if ( rb_release_ptr->rb_InformationReleaseList.n == 0 )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBInfoReleaselist is absent in RBR Message");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
    tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
    return;
  }

  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r5_IEs,rb_InformationAffectedList) )
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RB Info Affected list present in RBR Message");
  }


  
  /* Check if RB with PDCP Information list is present. This
  is not supported in the first release. This may be supported
  later */

#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
    tdsrrc_RadioBearerRelease_r5_IEs,dl_CounterSynchronisationInfo))
#else
    if (tdsrrcrbr_validate_srns_relocation_info(msg_ptr, TDSMSG_REL5))
#endif

  {
    /*check whether the SRNS relocation is within Cell_DCH or not.
    SRNS relocation information can come in reconfiguration messages
    only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
      ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbr.state_indicated_by_msg);
      tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has 
    PDCP info list. If it is true then this configuration is unsupported */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rb_release_ptr->dl_CounterSynchronisationInfo,
          rb_WithPDCP_InfoList)) ||
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_release_ptr->dl_CounterSynchronisationInfo,
         tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
      tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
    /* The procedure involves Lossy SRNS relocation. 
    Set the flag that indicates that this procedure
      involves SRNS relocation */
      tdsrrc_rbr.rbr_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
        tdsrrc_RadioBearerRelease_r5_IEs,new_U_RNTI))
      {
        tdsrrc_rbr.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(rb_release_ptr->new_U_RNTI),
          &tdsrrc_rbr.new_urnti);
      }
    }

    
  } /* dl counter sync info */

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  if((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
        tdsrrc_RadioBearerRelease_r5_IEs,dl_CommonInformation)) && 
       (TDSRRC_MSG_COMMON_BITMASK_IE(rb_release_ptr->dl_CommonInformation,
        tdsrrc_DL_CommonInformation_r5,dl_DPCH_InfoCommon)))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");
        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             ( TDSRRC_MSG_COMMON_BITMASK_IE(rb_release_ptr->dl_CommonInformation.dl_DPCH_InfoCommon,
             tdsrrc_DL_DPCH_InfoCommon_r4,mac_d_HFN_initial_value)),
             &rb_release_ptr->dl_CommonInformation.dl_DPCH_InfoCommon.mac_d_HFN_initial_value,
             &mac_d_hfn_l);
        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Process Ciphering Config for HHO failed");
        }
        tdsrrc_rbr.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }
  

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
    (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
    /* If UTRAN DRX coefficient is not present then it is invalid configuration */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
      tdsrrc_RadioBearerRelease_r5_IEs,utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_rbr.state_indicated_by_msg);
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    if (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      /*should we do it after getting l2ack for response??*/
      if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
        tdsrrc_RadioBearerRelease_r5_IEs,ura_Identity)))
        
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
        tdsrrccu_clear_ura_id ();
      }
      else
      {
        /*URA ID present in the message set ura_identity variable with this value */
        if (rb_release_ptr->ura_Identity.numbits == 16)
        {
          tdsrrccu_update_ura_id (&(rb_release_ptr->ura_Identity));
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rb_release_ptr->ura_Identity.numbits);
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
          tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
          return;
        }
        
      }
      
    }
  }


}/* end function tdsrrcrbr_validate_rbrelease_r5 */





/*===========================================================================

FUNCTION   RRCRBR_SEND_CHANNEL_CONFIG_REQ

DESCRIPTION

  This function sends the TDSRRC_CHANNEL_CONFIG_REQ command to LLC
  to release the specified RBs. If signalling Radio 
  bearers are affected and included in the Radio Bearer
  Release Message, they will also be sent in the same command.
        
DEPENDENCIES

  TDSORDERED_CONFIG must be set before calling this function.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_send_channel_config_req
(
  void
)
{
  tdsrrc_cmd_type *ptr;          /* To hold the Command                       */
  uint32 rb_count=0;          /* To count the number of RBs in the command */
  uint32 rb_index=0;          /* To index the array of RBs in the command */
  uint8 return_value=0;
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

    /* First fill up the Radio Bearers that need to be released.
    For this we need to know how many RBs need to be released.
    */
    
    if( tdsrrc_rbr.rbs_tobe_released.is_valid == TRUE )
    {
      for(rb_count=0; ((rb_count<tdsrrc_rbr.rbs_tobe_released.num_rbs)&&(rb_index < TDS_MAX_RB)); rb_count++)
      {
        return_value = tdsrrcllc_check_if_rbid_present_in_chan_config_within_index(
                  &ptr->cmd.chan_config_req, tdsrrc_rbr.rbs_tobe_released.rb_id[rb_count],rb_index);
        /* Fill up the RB id and indicate that it needs to be released */
        if(TDS_MAX_RB == return_value)
        {
          ptr->cmd.chan_config_req.rb[rb_index].rb_id = 
            tdsrrc_rbr.rbs_tobe_released.rb_id[rb_count];
          ptr->cmd.chan_config_req.rb[rb_index].rb_config = TDSRELEASE_RB;
          rb_index++;
        }
        else
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH," Ignoring Duplicate RB ID % d in RBR at index %d ",(uint16)tdsrrc_rbr.rbs_tobe_released.rb_id[rb_count],rb_index);
        }
      }
    }/* end if tdsrrc_rbr.rbs_tobe_released.is_valid  == TRUE */

#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
    if ((TDSCHECK_RLC_ALL_RB_REEST_ENABLE)&&
         ((tdsrrc_get_state() == TDSRRC_STATE_CELL_PCH) ||(tdsrrc_get_state() == TDSRRC_STATE_URA_PCH) )&& 
          ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) || 
           (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No need configure any RB if already in PCH state");
    }
    else
    {
#endif 

      if(TRUE == tdsrrc_rbr.rbs_tobe_reconfigured.is_valid)
      {
        for(rb_count=0; ((rb_count<tdsrrc_rbr.rbs_tobe_reconfigured.num_rbs)&&(rb_index < TDS_MAX_RB)); rb_count++)
        {
          return_value = tdsrrcllc_check_if_rbid_present_in_chan_config_within_index(
                    &ptr->cmd.chan_config_req, (uint16)tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[rb_count].rb_id,rb_index);
          if(TDS_MAX_RB == return_value)
          {
            /* Fill up the RB id and indicate that it needs to be released */
            ptr->cmd.chan_config_req.rb[rb_index].rb_id = 
              (uint16)tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[rb_count].rb_id;
            ptr->cmd.chan_config_req.rb[rb_index].rb_config = TDSRECONFIG_RB_OTA;
            ptr->cmd.chan_config_req.rb[rb_index].ul_cfg_mask = ptr->cmd.chan_config_req.rb[rb_index].dl_cfg_mask = TDSMODIFY_RB_CFG;
            rb_index++;
          }
          else
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH," Ignoring Duplicate RB ID % d in RBR at index %d ",(uint16)tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[rb_count].rb_id,rb_index);
          }
        }
      }/* end if tdsrrc_rbr.rbs_tobe_reconfigured.is_valid  == TRUE */

      if( tdsrrc_rbr.rbs_tobe_affected.is_valid == TRUE )
      {
        for(rb_count=0; ((rb_count<tdsrrc_rbr.rbs_tobe_affected.num_rbs)&&(rb_index < TDS_MAX_RB)); rb_count++)
        {
          return_value = tdsrrcllc_check_if_rbid_present_in_chan_config_within_index(
                    &ptr->cmd.chan_config_req, (uint16)tdsrrc_rbr.rbs_tobe_affected.rb_id[rb_count],rb_index);
          if(TDS_MAX_RB == return_value)
          {
            /* Fill up the RB id and indicate that it needs to be released */
            ptr->cmd.chan_config_req.rb[rb_index].rb_id = 
              tdsrrc_rbr.rbs_tobe_affected.rb_id[rb_count];
            ptr->cmd.chan_config_req.rb[rb_index].rb_config = TDSRECONFIG_RB_OTA;
            ptr->cmd.chan_config_req.rb[rb_index].ul_cfg_mask = ptr->cmd.chan_config_req.rb[rb_index].dl_cfg_mask = TDSMODIFY_RB_CFG;
            rb_index++;
          }
          else
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH," Ignoring Duplicate RB ID % d in RBR at index %d ",(uint16)tdsrrc_rbr.rbs_tobe_affected.rb_id[rb_count],rb_index);
          }
        }
      }/* end if tdsrrc_rbr.rbs_tobe_released.is_valid  == TRUE */
    }


    /* Check if state change is required */
    if( (tdsrrc_get_state()) == (tdsrrc_rbr.state_indicated_by_msg) )
    {
      ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
    }
    else
    {
      /* The message indicates that state change is required.
      Indicate to LLC what the next state should be. */
      ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
      ptr->cmd.chan_config_req.next_state = tdsrrc_rbr.state_indicated_by_msg;

      if ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
          ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) || 
           (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))&&
           (rb_index < (TDS_MAX_RB - 1)))
      {
        /* UE is directly transitioning to Cell_PCH state from Cell_FACH state so
        include release of CCCH and establishment of PCCH in the same channel config
        request */
        ptr->cmd.chan_config_req.rb[rb_index].rb_id       = CCCH_RADIO_BEARER_ID;
        ptr->cmd.chan_config_req.rb[rb_index++].rb_config = TDSRELEASE_RB;
        ptr->cmd.chan_config_req.rb[rb_index].rb_id       = PCCH_RADIO_BEARER_ID;
        ptr->cmd.chan_config_req.rb[rb_index++].rb_config = TDSESTABLISH_RB;
      }
    }
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Indicated state : %d,OC usage :%d .",tdsrrc_rbr.state_indicated_by_msg,tdstransition_config.toc_usage);
    
    #ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
    if (TDSCHECK_RLC_ALL_RB_REEST_ENABLE)
    {
    if (((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH) && (tdstransition_config.toc_usage 
        == TDSTOC_FOR_DCH_AND_FACH)) || ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
        (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)))
    {
        for( i =0;(i<rb_index) && (rb_index < TDS_MAX_RB);i++)
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
        if ((um_rrc_srb_exist == FALSE) && (rb_index < TDS_MAX_RB))
        {
      ptr->cmd.chan_config_req.rb[rb_index].rb_id = DCCH_UM_RADIO_BEARER_ID;
      ptr->cmd.chan_config_req.rb[rb_index++].rb_config = TDSRECONFIG_RB;
        }
        if ((am_rrc_srb_exist == FALSE) && (rb_index < TDS_MAX_RB))
        {
      ptr->cmd.chan_config_req.rb[rb_index].rb_id = DCCH_AM_RADIO_BEARER_ID;
      ptr->cmd.chan_config_req.rb[rb_index++].rb_config = TDSRECONFIG_RB;
        }
        if ((am_high_priority_srb_exist == FALSE) && (rb_index < TDS_MAX_RB))
        {
      ptr->cmd.chan_config_req.rb[rb_index].rb_id = DCCH_DT_HIGH_PRI_RADIO_BEARER_ID;
      ptr->cmd.chan_config_req.rb[rb_index++].rb_config = TDSRECONFIG_RB;
        }
        if ((am_low_priority_srb_exist == FALSE) && (rb_index < TDS_MAX_RB))
        {
      /* check whether RB4 was present or not */
      ul_rlc_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH, 
                                   DCCH_DT_LOW_PRI_RADIO_BEARER_ID,
                                   UE_MODE_ACKNOWLEDGED_DATA);

      if(ul_rlc_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"No RB4 in RB mapping");
      }
      else 
      {
        ptr->cmd.chan_config_req.rb[rb_index].rb_id = DCCH_DT_LOW_PRI_RADIO_BEARER_ID;
        ptr->cmd.chan_config_req.rb[rb_index++].rb_config = TDSRECONFIG_RB;
      }

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      if((tdsrrclcm_get_rb_id_if_dl_lc_type_exists(UE_LOGCHAN_TM_DCCH_DL, &srb5_rb_id) == TDSRRCLCM_SUCCESS) &&
	 (rb_index < TDS_MAX_RB))
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
    of established and reconfigured RBs */
    ptr->cmd.chan_config_req.num_rb_to_config = (uint16)rb_index; 
    if(rb_index < TDS_MAX_RB)
    {
      tdsrrc_fill_rb_id_chan_req_from_standalone_db(&rb_index, &ptr->cmd.chan_config_req);
    }

    if((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)&&(rb_index < TDS_MAX_RB))
    {
      /* Fill those RBs in chan_config_req for which RLC PDU size has changed and 
      that are already not present in chan_config_req*/
      tdsrrc_fill_rb_id_chan_req_from_rb_list_mapped_to_dch(&rb_index, &ptr->cmd.chan_config_req);
    }
  
    /*If state transition is Cell_DCH->Cell_FACH or vice-verca, add all the AM 
    PS RBs in chan_config_req*/	
    if((((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH) && (tdstransition_config.toc_usage 
        == TDSTOC_FOR_DCH_AND_FACH)) || ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
        (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH))
        #ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
          ||((TDSCHECK_RLC_ALL_RB_REEST_ENABLE)&&(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
          (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH))
        #endif
          )&&(rb_index < TDS_MAX_RB))
    {
      tdsrrc_fill_rb_id_chan_req_from_est_rabs(&rb_index, &ptr->cmd.chan_config_req);
    }
 
    /* Indicate the id of the procedure sending the command */
    ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RBR;

    /* Indicate that we need a confirmation for this command */
    ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"CHANNEL_CONFIG_REQ RBs:%d, curr_st:%d, next_st:%d", 
             rb_index,tdsrrc_get_state(),tdsrrc_rbr.state_indicated_by_msg);
    /* We are not required to fill any other data for this command.
    Now, send the command */
    tdsrrc_put_int_cmd( ptr );

  }
  else
  {
    /* Out of memory!!  */
    ERR_FATAL("Can't send channel config! No memory!",0,0,0);
  }
}/* end function tdsrrcrbr_send_channel_config_req */


/*===========================================================================

FUNCTION  RRCRBR_CLEAR_PROCEDURE

DESCRIPTION

  This function clears all the internal variables in
  the RBR procedure and brings them to their default
  values.
    
DEPENDENCIES

  The Substate of the procedure should be changed
  to initial substate before calling this function.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_clear_procedure( void )
{
  uint32 count;

  /* Initialize the transaction id's for this procedure.
  Use a number that can't be an actual received transaction
  id. */
  tdsrrc_rbr.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  tdsrrc_rbr.tr_ids.rejected_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  /* Clear the C-RNTI valid flag */
  tdsrrc_rbr.new_crnti_valid    = FALSE;
  tdsrrc_rbr.directed_cell_info = FALSE;

  /* Reset Initiate Cell Update Flag */
  tdsrrcrbr_send_initiate_cu_flag = FALSE;
  tdsrrcrbr_delete_crnti          = FALSE;

  tdsrrc_rbr.rbr_for_srns_relocation = FALSE;
  tdsrrc_rbr.new_urnti_valid = FALSE;
  /* free the memory allocate for mm_cmd_buf
  if it is not free or in case of unsuccessful events
  before sending this cmd to mm */
  if (tdsrrc_rbr.cn_info_saved != FALSE)
  {
    /* free the memory allocated for mm cmd */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Free MM cmd buffer %x",tdsrrc_rbr.mm_cmd_ptr);
    mm_free_cmd_buf(tdsrrc_rbr.mm_cmd_ptr);
    /* set cn_info_saved to false */
    tdsrrc_rbr.cn_info_saved = FALSE;
  }

  /* Initialize the RABS per message structure */
  for ( count=0; count < TDSRRCRB_MAX_RAB_PER_MSG; count++ )
  {
    tdsrrc_rbr.rabs.rabs_in_msg[count].num_rbs_for_rab = 0;
    tdsrrc_rbr.rabs.rabs_in_msg[count].rab_type = TDSRRCRB_UNKNOWN_RAB;
    tdsrrc_rbr.rabs.chan_config_needed[count] = FALSE;
    tdsrrc_rbr.rabs.rabs_in_msg[count].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_NONE;

  }
  tdsrrc_rbr.rabs.num_rabs_in_msg = 0;

  /* Indicate that there are no signalling connections
  to be released. */
  tdsrrc_rbr.need_to_release_cn_domain = FALSE;

  /* Indicate that there are no RBs to be released. This
  is the default value. */
  tdsrrc_rbr.rbs_tobe_released.is_valid = FALSE;
  tdsrrc_rbr.rbs_tobe_reconfigured.is_valid = FALSE;
  tdsrrc_rbr.rbs_tobe_affected.is_valid = FALSE;
  tdsrrc_rbr.trans_to_dch_ciph_config_needed = FALSE;
  
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  tdsrrc_rbr.initiate_cell_selection_trans_from_fach = FALSE;     
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

  tdsrrc_rbr.high_mobility_ind = FALSE;
  tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
  tdsrrc_set_ordered_codec_status(FALSE);
#endif

}


/*===========================================================================

FUNCTION   RRCRBR_PROCESS_SUCCESSFUL_PROC_COMPLETION

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
static void tdsrrcrbr_process_successful_proc_completion
(
 boolean indicate_rlc
)
{
  /* send the nas cn info if stored */
  if (tdsrrc_rbr.cn_info_saved != FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_SERVICE_IND_FROM_OTA_MSG due to CN info to MM");
    if(tdsrrc_rbr.mm_cmd_ptr->cmd.hdr.message_id == (byte) RRC_SERVICE_IND_FROM_OTA_MSG )
    {
      /* update the cell access information in the service info structure */
      tdsrrcccm_update_service_info(tdsrrc_rbr.mm_cmd_ptr);
    }
    /* Put the command on MM queue */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(tdsrrc_rbr.mm_cmd_ptr);
    }
    mm_put_cmd(tdsrrc_rbr.mm_cmd_ptr);
    /* clear the flag */
    tdsrrc_rbr.cn_info_saved = FALSE;
  }
  /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
  configurations */
  tdsrrcllc_clear_ordered_config();  

  if (((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) || 
       (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH )) &&
      (indicate_rlc == TRUE))
  {
    /* register with RLC for uplink data indication */
    tdsrrcrb_register_with_rlc_for_ul_data_ind (TDSRRC_PROCEDURE_RBR);

  }

  if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
  {
    if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
    {      
      (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
    }
    else
    {
      (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
    }
    tdsrrc_rbr.need_to_release_cn_domain = FALSE;
  }

    /* send the nas cn info if stored */
  if (tdsrrc_rbr.cn_info_saved != FALSE)
  {
    /* Put the command on MM queue */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RRC_SERVICE_IND_FROM_OTA_MSG to MM");
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(tdsrrc_rbr.mm_cmd_ptr);
    }
    mm_put_cmd(tdsrrc_rbr.mm_cmd_ptr);
    /* clear the flag */
    tdsrrc_rbr.cn_info_saved = FALSE;
  }


  /* Go to the next sub-state in the state machine. That's
  the initial substate since we've sent the RB RC complete
  message */
  tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

  /* At this point we don't need to remember any of the
  Radio Bearer information since it's stored in ESTABLISHED_RABS.
  Clear our internal variables */
  tdsrrcrbr_clear_procedure();
  if (tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_RB_PROC_COMPL_IND) 
  {
    tdsrrcrb_send_rb_proc_compl_ind();
  }

}    /* tdsrrcrbr_process_successful_proc_completion */

/*===========================================================================

FUNCTION   tdsrrcrbr_process_high_mobility_ind_in_non_critical_ext

DESCRIPTION

  This function will process High Mobility Ind received in Non-critical ext
        
DEPENDENCIES
  None.

RETURN VALUE
None

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_process_high_mobility_ind_in_non_critical_ext
(
  tdsrrc_RadioBearerRelease *msg_ptr,
  tdsrrc_msg_ver_enum_type msg_version
)
{
  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
    if(msg_version == TDSMSG_REL5)
    {
      if((msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.m.v690NonCriticalExtensionsPresent) &
        (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v690NonCriticalExtensions.m.
        v770NonCriticalExtensionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v690NonCriticalExtensions.v770NonCriticalExtensions.
        radioBearerRelease_v770ext.m.ueMobilityStateIndicatorPresent))
      {
        if(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v690NonCriticalExtensions.v770NonCriticalExtensions.
            radioBearerRelease_v770ext.ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in RBR.");
          tdsrrc_rbr.high_mobility_ind = TRUE;
        }
      }
    }
    else if(msg_version == TDSMSG_REL6)
    {
     if((msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.m.v6b0NonCriticalExtensionsPresent) &&
         (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.v6b0NonCriticalExtensions.m.
         v770NonCriticalExtensionsPresent) &&
         (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.v6b0NonCriticalExtensions.v770NonCriticalExtensions.
         radioBearerRelease_v770ext.m.ueMobilityStateIndicatorPresent))
      {
        if(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.v6b0NonCriticalExtensions.
           v770NonCriticalExtensions.radioBearerRelease_v770ext.ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in RBR.");
          tdsrrc_rbr.high_mobility_ind = TRUE;
        }
      }
    }
    else if(msg_version == TDSMSG_REL4)
    {
      if((msg_ptr->u.later_than_r3.criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent) &&
           (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.m.
                 v590NonCriticalExtensionsPresent) &&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
                 m.v690NonCriticalExtensionsPresent) &&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
               v690NonCriticalExtensions.m.v770NonCriticalExtensionsPresent) &&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
                 v590NonCriticalExtensions.v690NonCriticalExtensions.
                   v770NonCriticalExtensions.radioBearerRelease_v770ext.m.ueMobilityStateIndicatorPresent))
      {
        if(msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
            v590NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
                radioBearerRelease_v770ext.ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in RBR.");
          tdsrrc_rbr.high_mobility_ind = TRUE;
        }
      }
    }  
  }
}

/*===========================================================================

FUNCTION   RRCRBR_PROCESS_RBRELEASE_R6

DESCRIPTION

  This function processes the received Rel 6 RB Release Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Release Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to release the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Release Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_process_rbrelease_r6
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_RB_InformationReleaseList  *rb_info_rel_list_ptr;
  tdsrrc_RB_InformationAffectedList_r6 *rb_info_affected_list_ptr;
   tdsrrc_RB_InformationReconfig_r6 *rb_info_reconfig_ptr;

  tdsrrc_RadioBearerRelease *msg_ptr;  /* Pointer to the RB Release message        */
  tdsrrcrb_directed_cell_info_type directed_cell_info; /* directed cell info struct */

  tdsrrcllc_oc_status_e_type oc_status;/* To hold tdsordered_config status          */
  boolean ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                    occured while setting tdsordered_config. */
  boolean rb_rel_srch = FALSE; 
  boolean rab_found = FALSE; 
  tdsrrc_state_e_type current_state;       /* current RRC State  */
  uint32 rab_count, rb_count, localrb_count;
  /* Local C_RNTI value */
  tdsrrc_int_c_rnti_type              c_rnti;
  tdsrrc_int_u_rnti_type              u_rnti;
  tdsrrcrb_rnti_update_info_type      rnti_update_info;   /* to hold rnti update info */

  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL6;


  tdsrrcsmc_srns_ip_check_status_e_type    ip_check_status = TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;


  tdsrrc_DL_DCCH_Message *dcch_msg_ptr ;     /* Pointer to the RB Release message */

  /* Store parameter IEs */
  tdsrrc_RadioBearerRelease_r6_IEs* rb_release_ptr = NULL;
  uint32 local_rb_idx=0;
  
  tdsrrcrb_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Release message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerRelease);


  /* Store Msg Ptr in a local variable */
  rb_release_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
    u.criticalExtensions.u.r6.radioBearerRelease_r6;



  if (TRUE == tdsrrcrbr_validate_srns_relocation_info(msg_ptr, msg_version))
  {
    ip_check_status = tdsrrcsmc_check_integrity_for_srns_relocation(cmd_ptr, msg_version);
  
    switch (ip_check_status)
    {
      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS:
        /*continue further processing with the message */
        break;

      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE:
        /*Discard the message and continue with ongoing procedure as if this
        message was not received */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IP check failed. Discarding RBS msg");
        break;

      case TDSRRCSMC_SRNS_SECURITY_INFO_INVALID:
      case TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION:
        if (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier
                       == tdsrrc_rbr.tr_ids.rejected_transaction)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid security config-rejected tr-id%d match!",tdsrrc_rbr.tr_ids.rejected_transaction);
          /* Nothing else to do, just ignore the message and return */
        } 
        else
        {
          /* send a reconfig failure messge with invalid
          configuration and 
          continue the same substate with any ongoing procedure */
          temp_failure_cause.failure_status = tdsrrc_rbr.status.failure_status ;
          temp_rejected_transaction = tdsrrc_rbr.tr_ids.rejected_transaction;
          
          tdsrrc_rbr.tr_ids.rejected_transaction = 
            (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
          if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
          {
            tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          }
          else
          {
            tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
          }
          /*Check whether Cell Update is going on */
          if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
               (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
          {
            if(tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbr subsstate(%d) Send failure with %d cause",tdsrrc_rbr.curr_substate,tdsrrc_rbr.status.failure_status);
              tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                       tdsrrc_rbr.status.prot_err_cause );
              
             tdsrrc_rbr.status.failure_status  = temp_failure_cause.failure_status;
             tdsrrc_rbr.tr_ids.rejected_transaction = temp_rejected_transaction ;
             
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
              (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
              /* Wait until Cell update procedure is completed */
              tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
            }
          }
          else
          {
            tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                            tdsrrc_rbr.status.prot_err_cause );
          }
        }
        /*do not clear the procedure or change the tdsrrc_rbe substate because
        some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
        break;

      default:
        /*should not come here */
        break;
    }
    if (ip_check_status != TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS)
    {
      /*do not clear the procedure or change the tdsrrc_rbr substate because
      some other tdsrrc_rbe procedure might be in progress. Just return from
      here */
      return;
    }
  }


  if ((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
      (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
      (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incomatible simultaneous reconfig! Rejecting RBR message");
     if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
            == tdsrrc_rbr.tr_ids.accepted_transaction )
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate RBR message - accepted tr-id matched!");
       /* Nothing else to do, just return */
     } 
     else if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                    == tdsrrc_rbr.tr_ids.rejected_transaction )
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate RBR message - rejected tr-id matched!");
       /* Nothing else to do, just return */
     } 
     else
     {
       /* We have received a RBR message with a new transaction id.
       This message should be in our rejected transaction
       list */
       
       temp_failure_cause.failure_status = tdsrrc_rbr.status.failure_status ;
       temp_rejected_transaction = tdsrrc_rbr.tr_ids.rejected_transaction;
          
       tdsrrc_rbr.tr_ids.rejected_transaction = 
         (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
       tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;

       /*Check whether Cell Update is going on */
       if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
            (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
       {
         if(tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
         {
           TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbr subsstate(%d) Send failure with %d cause",tdsrrc_rbr.curr_substate,tdsrrc_rbr.status.failure_status);
           tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                     tdsrrc_rbr.status.prot_err_cause );
           
           tdsrrc_rbr.status.failure_status  = temp_failure_cause.failure_status;
           tdsrrc_rbr.tr_ids.rejected_transaction = temp_rejected_transaction ;
           
         }
         else
         {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
           (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
           /* Wait until Cell update procedure is completed */
           tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
         }
       }
       else
       {
         tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                         tdsrrc_rbr.status.prot_err_cause );
         /*continue the same substate */ 
       }
     }
     return;
  }

  /* We have received a RB release message with a new transaction id.
  Accept the transaction Id */
  tdsrrc_rbr.tr_ids.accepted_transaction =
     msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rb_release_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rb_release_ptr->m.activationTimePresent,
         rb_release_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBR SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbr_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rb_release_ptr->m.activationTimePresent = 0;
    }
  }

#endif

  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  tdsrrcrbr_validate_rbrelease_r6(msg_ptr);

  if ( tdsrrc_rbr.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBR Message validation failed");

    /* Send the RB Release Failure Message */
    tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                              tdsrrc_rbr.status.prot_err_cause
                            );

    tdsrrcrbr_clear_procedure();
    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
  }
  /* If the message is valid, proceed further */
  else
  {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    boolean                  additional_srb;
    uecomdef_logchan_e_type  dl_sdu_type = UE_LOGCHAN_NONE;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing RBR Message");
    tdsrrcrbr_process_high_mobility_ind_in_non_critical_ext(msg_ptr,TDSMSG_REL6);
    
    /*store the rb ids tobe released */
    tdsrrc_rbr.rbs_tobe_released.is_valid = TRUE;
    rb_info_rel_list_ptr = &rb_release_ptr->rb_InformationReleaseList;
    localrb_count = 0;
	local_rb_idx =0;
    while ( rb_info_rel_list_ptr->n >  local_rb_idx)
    {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      additional_srb = FALSE;

      if((tdsrrclcm_get_dl_lc_type(tdsrrclcm_get_dl_rlc_lc_id(rb_info_rel_list_ptr->elem[local_rb_idx]), &dl_sdu_type)) == TDSRRCLCM_SUCCESS)
      {
        if(dl_sdu_type == UE_LOGCHAN_TM_DCCH_DL)
        {
          additional_srb = TRUE;
        }
      }
      /* check whether rb id is signalling rbid or does not exist in 
       * established rabs 
       */
      if (additional_srb == FALSE)
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
      { 
        /*check whether rb id is signalling rbid or does not exist in established
        rabs */
        if( rb_info_rel_list_ptr->elem[local_rb_idx] < DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
        {
          /* RB Message was invalid. */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RB id:%d in RBR Message",rb_info_rel_list_ptr->elem[local_rb_idx]);
          /* Send the RB Release Failure Message */
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                    tdsrrc_rbr.status.prot_err_cause
                                  );
  
          tdsrrcrbr_clear_procedure();
          /* Make sure we're back in the initial sub-state */
          tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
          return;
  
        }
        else if( rb_info_rel_list_ptr->elem[local_rb_idx] == DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
        {
          if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND != tdsrrclcm_check_dl_rlc_lc_id(
                                           UE_LOGCHAN_DCCH,
                                           DCCH_DT_LOW_PRI_RADIO_BEARER_ID,
                                           UE_MODE_ACKNOWLEDGED_DATA))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Release SRB4");
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignore Non-exitent rb_id = %d in tdsrrc_rbr msg",rb_info_rel_list_ptr->elem[local_rb_idx]);
			local_rb_idx++;
            continue;
          }
        }
        else if (!tdsrrc_find_rb_in_est_rabs(rb_info_rel_list_ptr->elem[local_rb_idx]))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignore Non-exitent rb_id =%d in tdsrrc_rbr msg",rb_info_rel_list_ptr->elem[local_rb_idx]);
          local_rb_idx++;
          continue;
        }
      }

      tdsrrc_rbr.rbs_tobe_released.rb_id[localrb_count] = rb_info_rel_list_ptr->elem[local_rb_idx];
      localrb_count++;
      local_rb_idx++;
    }
    tdsrrc_rbr.rbs_tobe_released.num_rbs = localrb_count;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Number ofRBs tobe released:%d",localrb_count);

    /*store the rb ids tobe reconfigured */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
          tdsrrc_RadioBearerRelease_r6_IEs,rb_InformationReconfigList))
    {
      tdsrrc_rbr.rbs_tobe_reconfigured.is_valid = TRUE;
            localrb_count = 0;
      while ( rb_release_ptr->rb_InformationReconfigList.n >localrb_count )
      {
      
 	   rb_info_reconfig_ptr = 
			  &rb_release_ptr->rb_InformationReconfigList.elem[localrb_count];
        /* Fill up the RB id and indicate that it needs to be reconfigured */
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_id= 
          rb_info_reconfig_ptr->rb_Identity;
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_config = TDSRECONFIG_RB;
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask = tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask = 0;
        if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
			tdsrrc_RB_InformationReconfig_r6,rlc_Info))&&
            (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,
            tdsrrc_RLC_Info_r6,ul_RLC_Mode)))
        {
          tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask = TDSMODIFY_RB_CFG;
        }
        if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
			tdsrrc_RB_InformationReconfig_r6,rlc_Info))&&
            (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,
            tdsrrc_RLC_Info_r6,dl_RLC_Mode)))
        {
          tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
        }
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
			tdsrrc_RB_InformationReconfig_r6,rb_StopContinue))
        {
          /* over-ride rb_config with STOP or continue */
          if (tdsrrc_RB_StopContinue_stopRB == rb_info_reconfig_ptr->rb_StopContinue)
          {
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask |= TDSSTOP_RB_CFG;
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask |= TDSSTOP_RB_CFG;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"rb-reconfig-info rb %d to STOP rb_count %d",tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_id,localrb_count);
          }
          else if (tdsrrc_RB_StopContinue_continueRB == rb_info_reconfig_ptr->rb_StopContinue)
          {
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask |= TDSCONTINUE_RB_CFG;
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask |= TDSCONTINUE_RB_CFG;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"rb-reconfig-info rb %d to CONTINUE rb_count %d",tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_id,localrb_count);
            
          }
        }

        localrb_count++;
        /* Go to the next reconfigured RB */
      }

      tdsrrc_rbr.rbs_tobe_reconfigured.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Number ofRBs tobe reconfigured:%d",localrb_count);
    }

    /*store the rb ids tobe affected */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
          tdsrrc_RadioBearerRelease_r6_IEs,rb_InformationAffectedList))
    {
      tdsrrc_rbr.rbs_tobe_affected.is_valid = TRUE;
      rb_info_affected_list_ptr = 
      &rb_release_ptr->rb_InformationAffectedList;
      localrb_count = 0;
      while ( rb_info_affected_list_ptr->n > localrb_count)
      {
        /*check whether rb id is signalling rbid or does not exist in established
        rabs */
        if(( rb_info_affected_list_ptr->elem[localrb_count].rb_Identity > DCCH_DT_LOW_PRI_RADIO_BEARER_ID)&&
           (! tdsrrc_find_rb_in_est_rabs(rb_info_affected_list_ptr->elem[localrb_count].rb_Identity)))
        {
          /* RB Message was invalid. */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RB id:%d in RBR Message",rb_info_affected_list_ptr->elem[localrb_count].rb_Identity);
          /* Send the RB Release Failure Message */
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                    tdsrrc_rbr.status.prot_err_cause
                                  );

          tdsrrcrbr_clear_procedure();
          /* Make sure we're back in the initial sub-state */
          tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
          return;

        }
        tdsrrc_rbr.rbs_tobe_affected.rb_id[localrb_count] = 
        rb_info_affected_list_ptr->elem[localrb_count].rb_Identity;
        localrb_count++;
      }
      tdsrrc_rbr.rbs_tobe_affected.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Number ofRBs tobe affected:%d",localrb_count);
    }


    /* If there is a signalling connection to be released,
    or if there are individual RBs to be released, we
    will be releasing some RBs. */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
        tdsrrc_RadioBearerRelease_r6_IEs,signallingConnectionRelIndication))
    {
      tdsrrcrbr_mark_cn_doamin_to_be_closed(rb_release_ptr->signallingConnectionRelIndication);
    
      for ( rab_count = 0; rab_count< MAX_RAB_TO_SETUP; rab_count++ )
      {
        if ( (tdsrrc_est_rabs.rabs[rab_count].cn_domain == tdsrrc_rbr.cn_to_release) &&
             (tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0) )
        {
          rab_found = TRUE;
          for ( rb_count=0; rb_count<tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab; rb_count++ )
          {
            rb_rel_srch = FALSE;
            for ( localrb_count=0; localrb_count<tdsrrc_rbr.rbs_tobe_released.num_rbs; localrb_count++ )
            {
              if ( tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id == 
                   tdsrrc_rbr.rbs_tobe_released.rb_id[localrb_count] )
              {
                rb_rel_srch = TRUE;
                break;
              }
            }
            if ( rb_rel_srch == FALSE ) // est rabs has an rb_id not in the rb release list
            {
              /* send failure message */
              tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
              tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                        tdsrrc_rbr.status.prot_err_cause);
              /* Go back to initial substate */
              tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
              /* Then clear all local variables that may
              need to be cleared */
              tdsrrcrbr_clear_procedure();
              return;
            }
          }/* rb_count loop */       
        } /*end of if loop */
      }/* rab_count loop */
      /*if none of the rabs in the establishedrabs have rb information*/
      if ( rab_found==FALSE )
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB lookup in est_rabs for cn:%d failed",tdsrrc_rbr.cn_to_release );
        /* send failure message */
        tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause);
        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbr_clear_procedure();
        return;

      }
    }



    /* Check if there is CN information present. If so, we need
    to send the information to the Cell Change manager */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r6_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      tdsrrc_rbr.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(
        &(rb_release_ptr->cn_InformationInfo) );
      if (tdsrrc_rbr.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbr.cn_info_saved = TRUE;
      }
    }

    /* Indicate layer1 if rb release message is going to release voice RAB */
    tdsrrcrbr_indicate_layer1_if_config_msg_release_voice_rab();

    tdsrrcrb_update_mac_rab_status(TDSRRC_PROCEDURE_RBR, RRC_CS_DOMAIN_CN_ID,
                                &rb_release_ptr->rb_InformationReleaseList,
                                FALSE);

    /* Set TDSORDERED_CONFIG so that LLC can process the
    necessary parameters. */
    oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBR,
                                           tdsrrc_rbr.state_indicated_by_msg,
                                           tdsrrc_DL_DCCH_Message_PDU,
                                           (void *)dcch_msg_ptr
                                         );

    switch ( oc_status )
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
        /* Populate the local variables to indicate if Re-direction info is present */
        directed_cell_info.freq_info_present = FALSE;
        directed_cell_info.cpid_present = FALSE; 
        if((current_state==TDSRRC_STATE_CELL_FACH) && 
               ((tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
               ||(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
               ||(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
          if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
                 tdsrrc_RadioBearerRelease_r6_IEs,frequencyInfo)) &&
                (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->frequencyInfo.modeSpecificInfo,
                rrc_FrequencyInfo_modeSpecificInfo_tdd)))
          {
            directed_cell_info.freq_info_present = TRUE;
            directed_cell_info.uarfcn_dl  = rb_release_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
          }
          
          if (tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL6))
          {
            if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r6_IEs,dl_InformationPerRL_List)) &&
              (rb_release_ptr->dl_InformationPerRL_List.n != 0))
            {
              
              directed_cell_info.cpid_present = TRUE;
              directed_cell_info.cell_param_id = rb_release_ptr->dl_InformationPerRL_List.elem[0].
                modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
            }
          }   
           /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
                 FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
          if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
            (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
            if(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
            {
              tdsrrcrbr_send_initiate_cu_flag = TRUE;
              tdsrrcrbr_delete_crnti          = TRUE;
            }
            else
            {
              tdsrrc_rbr.directed_cell_info = TRUE;
            }
          }  
        }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
        if (
            (current_state==tdsrrc_rbr.state_indicated_by_msg) ||
            ((current_state==TDSRRC_STATE_CELL_FACH) && 
             (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH))
            )
        {
          /* CELL_DCH->CELL_DCH or CELL_FACH->CELL_FACH or 
          CELL_FACH->CELL_DCH case. We can release the
          Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
          /*check whether any frequency info is present in the message
          and transition is CELL_FACH->CELL_FACH. In this case
          initiate a cell update after channel config is successfully completed */
          if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
             (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
          {
 #ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
             /*call the function that evaluates the directed cell info and set the flags accordingly */
            if ((tdsrrcrb_validate_directed_cell_info (directed_cell_info)== FALSE)
                &&(tdsrrcrbr_send_initiate_cu_flag == FALSE))
            {
              if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
                 tdsrrc_RadioBearerRelease_r6_IEs,new_C_RNTI))
              {
                tdsrrc_rbr.new_crnti_valid = TRUE;
                tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, 
                                    &c_rnti);
                tdsrrc_rbr.new_crnti = (uint16)c_rnti;
              }
              else
              {
                tdsrrc_rbr.new_crnti_valid = FALSE;
              }

              if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
              {
                if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
                {      
                  (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
                }
                else
                {
                  (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
                }
                tdsrrc_rbr.need_to_release_cn_domain = FALSE;
              }
              tdsrrcllcoc_update_toc_with_oc_from_fach();
              /*Initiate the cell selection*/
              tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR,
                                                     TDSRRC_TRANSITION_FROM_FACH,
                                                     tdsrrc_rbr.state_indicated_by_msg);
  
  
           
              
              tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
              /* exit from the switch case */
              break;
     
            }
     #else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
     
            if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
                tdsrrc_RadioBearerRelease_r6_IEs,frequencyInfo)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->frequencyInfo.modeSpecificInfo,
               rrc_FrequencyInfo_modeSpecificInfo_tdd)))
            {
              directed_cell_info.freq_info_present = TRUE;
              directed_cell_info.uarfcn_dl  = rb_release_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
            }
            else
            {
              directed_cell_info.freq_info_present = FALSE;
            }
            if (tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL6))
            {
              if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r6_IEs,dl_InformationPerRL_List)) &&
                (rb_release_ptr->dl_InformationPerRL_List.n != 0))
              {
                
                directed_cell_info.cpid_present = TRUE;
                directed_cell_info.cell_param_id = rb_release_ptr->dl_InformationPerRL_List.elem[0].
                  modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
              }
            }
            else
            {
              directed_cell_info.cpid_present = FALSE;
            }
            /*call the function that evaluates the directed cell info and set the flags accordingly */
            if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
            {
              /*directed cell info matched. Cell update is not required */
              tdsrrcrbr_send_initiate_cu_flag = FALSE;
            }
            else
            {
              /*directed cell info do not match. Cell update is required */
              tdsrrcrbr_send_initiate_cu_flag = TRUE;
              tdsrrcrbr_delete_crnti          = TRUE;
            }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
          } /* if CELL_FACH->CELL_FACH reconfiguration */

          /* Call the function that creates and sends a channel
            config request to LLC. */
          tdsrrcrbr_send_channel_config_req();

          /* Change the substate since we're waiting for the
             confirmation from LLC for the channel configs */
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
        {
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
             tdsrrc_RadioBearerRelease_r6_IEs,new_C_RNTI))
          {
            tdsrrc_rbr.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, 
                                &c_rnti);
            tdsrrc_rbr.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_rbr.new_crnti_valid = FALSE;
          }

          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR,
                                                  TDSRRC_TRANSITION_FROM_DCH,
                                                  TDSRRC_STATE_CELL_FACH);
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

          /* Start T_305 [periodic cell update] timer */
          tdsrrccu_start_t_305_timer();		  
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                 (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
        {
          /*store the directed cell info */
           tdsrrc_rbr.directed_cell_info = tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL6);

           /*Send the tdsrrc_rbr complete msg on old configuration */
           tdsrrcrbr_send_rbr_complete_message(TRUE);
           /*wait for L2ack */
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
           tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
           tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
        {
          /*Send the RBR complete msg on old configuration */
          tdsrrcrbr_send_rbr_complete_message(TRUE);
          /*wait for L2ack */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
          tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_FACH) && 
                ((tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
                 (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if ((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE)
            && (tdsrrc_rbr.directed_cell_info == FALSE))
          {
            if(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
            {
              tdsrrc_rbr.directed_cell_info = tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL6);
            }
            tdsrrc_rbr.initiate_cell_selection_trans_from_fach  = TRUE;
          }
    
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

          /* check whether CPICH info is present or not. Store this
          info  */
         // tdsrrc_rbr.directed_cell_info = rrcrbr_is_pcpich_info_present(msg_ptr);
          /*Send the RRCTCTC complete msg on old configuration */
          /* check whether new C-RNTI was present in the message. In that case
          we have to configure MAC with new C-RNTI before sending the response 
          message */
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
             tdsrrc_RadioBearerRelease_r6_IEs,new_C_RNTI))
          {
            tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, 
                                &c_rnti);
            /* Send a RNTI Update Request */
            rnti_update_info.crnti_action    = TDSRRC_RNTI_UPDATE;
            rnti_update_info.crnti           = (uint16)c_rnti;
            /* check whether URNTI is also present in the message */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
             tdsrrc_RadioBearerRelease_r6_IEs,new_U_RNTI))
            {
              tdsrrc_translate_urnti(&rb_release_ptr->new_U_RNTI, 
                                  &u_rnti);
              rnti_update_info.urnti_action    = TDSRRC_RNTI_UPDATE; 
              rnti_update_info.urnti           = u_rnti; 
            }
            else
            {
              rnti_update_info.urnti_action    = TDSRRC_RNTI_NOCHANGE; 
            }
            rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBR;
            rnti_update_info.rlc_re_establish_srb = FALSE;
            rnti_update_info.rlc_re_establish_rab = FALSE;
            rnti_update_info.cnf_reqd             = TRUE;
            tdsrrcrb_send_rnti_update_req(rnti_update_info);
            /* wait for the RNTI update confirm before sending the response
            message */
            tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
          }
          else 
          {
            tdsrrcrbr_send_rbr_complete_message(TRUE);
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
            /*wait for L2ack */
            tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
            tdsrrcllc_print_trans_chnl_rate();
          }
        }
        break;

      case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
        tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
        tdsrrc_rbr.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
		
      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_rbr.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_NOT_SET_OTHER:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
        /* When LLC sends this, there is a serious problem
        and we'll go back to disconnected state. No need
        to send the failure message here, we can just
        go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      default:

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
        /* Unknown error, just go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
    }/* end switch */

    if ( ordered_config_err_occured == TRUE )
    {
        /* No need to continue. Clear the procedure
        variables and return. */
        tdsrrcrbr_clear_procedure();
        return;
    }


    if(tdsrrc_rbr.rbr_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }

      
  }/* end if - valid rb message */
}

/*===========================================================================

FUNCTION   tdsrrcrbr_check_and_get_primary_plmn_id

DESCRIPTION

  This function will check whether primary plmn id is present 
  in r3,r5 path of RBR message. If present then it will return 
  primary plmn id.
        
DEPENDENCIES
  None.

RETURN VALUE
TRUE : IF Primary PLMN I.D is present in RBR message in r3, r5 path
FFALSE: Otherwise

SIDE EFFECTS

  None.

===========================================================================*/
static boolean tdsrrcrbr_check_and_get_primary_plmn_id
(
  /* Pointer to the RBR message */
  tdsrrc_RadioBearerRelease *msg_ptr,
  /*This will copy primary plmn i.d if present in OTA msg*/
  tdsrrc_PLMN_Identity *primary_plmn_Identity ,
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
      /*Check to see if primary PLMN I.D is present in RBR message ext
       * tdsrrc_RadioBearerRelease-v690ext which is present in 
       * v690NonCriticalExtensions in r99 path
       */
      if((msg_ptr->u.r3.m.v3a0NonCriticalExtensionsPresent)&&
         (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.m.
              v690NonCriticalExtensionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
                 v590NonCriticalExtensions.v690NonCriticalExtensions.radioBearerRelease_v690ext.m.primary_plmn_IdentityPresent))

      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:RBR msg contains primary_plmn_Id in r99 path");
        primary_plmn_Id_present = TRUE;
        *primary_plmn_Identity = msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
                 v590NonCriticalExtensions.v690NonCriticalExtensions.radioBearerRelease_v690ext.primary_plmn_Identity;
      }
      break;
    
    case TDSMSG_REL5:
      /*Check to see if primary PLMN I.D is present in RBR message ext
       * tdsrrc_RadioBearerRelease-v690ext which is present in 
       * v690NonCriticalExtensions in r5 path
       */
      if((msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.m.v690NonCriticalExtensionsPresent)&&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.
           v690NonCriticalExtensions.radioBearerRelease_v690ext.m.primary_plmn_IdentityPresent)
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:RBR msg contains primary_plmn_Id in r5 path");
        primary_plmn_Id_present = TRUE;
        *primary_plmn_Identity = msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.
           v690NonCriticalExtensions.radioBearerRelease_v690ext.primary_plmn_Identity;
      }
      break;
   
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"NWS: Msg version = %d,Not supported",msg_version);
      break;
  }
  return primary_plmn_Id_present;
}
/*===========================================================================

FUNCTION   RRCRBR_PROCESS_RBRELEASE_R5

DESCRIPTION

  This function processes the received Rel 5 RB Release Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Release Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to release the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Release Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_process_rbrelease_r5
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_RB_InformationReleaseList  *rb_info_rel_list_ptr;
  tdsrrc_RB_InformationAffectedList_r5 *rb_info_affected_list_ptr;
  tdsrrc_RadioBearerRelease *msg_ptr;  /* Pointer to the RB Release message        */
  tdsrrcrb_directed_cell_info_type directed_cell_info; /* directed cell info struct */

  tdsrrcllc_oc_status_e_type oc_status;/* To hold tdsordered_config status          */
  boolean ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                    occured while setting tdsordered_config. */
  boolean rb_rel_srch = FALSE; 
  boolean rab_found = FALSE; 
  tdsrrc_state_e_type current_state;       /* current RRC State  */
  uint32 rab_count, rb_count, localrb_count;
  /* Local C_RNTI value */
  tdsrrc_int_c_rnti_type              c_rnti;
  tdsrrc_int_u_rnti_type              u_rnti;
  tdsrrcrb_rnti_update_info_type      rnti_update_info;   /* to hold rnti update info */
  uint32 local_idx=0;
  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL5;


  tdsrrcsmc_srns_ip_check_status_e_type    ip_check_status = TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;


  tdsrrc_DL_DCCH_Message *dcch_msg_ptr ;     /* Pointer to the RB Release message */

  /* Store parameter IEs */
  tdsrrc_RadioBearerRelease_r5_IEs* rb_release_ptr = NULL;

   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  boolean primary_plmn_Id_present = FALSE;
   /*Variable to hold primary plmn i.d if present in OTA msg*/
  tdsrrc_PLMN_Identity primary_plmn_Identity;
  tdsrrcrb_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;


  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Release message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerRelease);

  if ((msg_ptr->u.later_than_r3.criticalExtensions.t == 
       T_tdsrrc_RadioBearerRelease_criticalExtensions_5_criticalExtensions) &&
      (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.t == 
       T_tdsrrc_RadioBearerRelease_criticalExtensions_5_r5))
  {
    /* Store Msg Ptr in a local variable */
    rb_release_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
                  u.r5.radioBearerRelease_r5;

    TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:rbreleaser5 rcvd");
  } 
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported RB Release t");
    temp_failure_cause.failure_status = tdsrrc_rbr.status.failure_status ;

    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;

    /*Check whether Cell Update is going on */
    if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
         (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
    {
      if(tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbr subsstate(%d) Send failure with %d cause",tdsrrc_rbr.curr_substate,tdsrrc_rbr.status.failure_status);
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                 tdsrrc_rbr.status.prot_err_cause );
        tdsrrc_rbr.status.failure_status = temp_failure_cause.failure_status;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
        (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
        /* Wait until Cell update procedure is completed */
        tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      }
    }
    else
    {
      tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                      tdsrrc_rbr.status.prot_err_cause );
      /*continue the same substate */ 
    }
    return;
  }

  if (TRUE == tdsrrcrbr_validate_srns_relocation_info(msg_ptr, msg_version))
  {
    ip_check_status = tdsrrcsmc_check_integrity_for_srns_relocation(cmd_ptr, msg_version);
  
    switch (ip_check_status)
    {
      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS:
        /*continue further processing with the message */
        break;

      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE:
        /*Discard the message and continue with ongoing procedure as if this
        message was not received */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IP check failed. Discarding RBS msg");
        break;

      case TDSRRCSMC_SRNS_SECURITY_INFO_INVALID:
      case TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION:
        if (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier
                       == tdsrrc_rbr.tr_ids.rejected_transaction)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid security config-rejected tr-id%d match!",tdsrrc_rbr.tr_ids.rejected_transaction);
          /* Nothing else to do, just ignore the message and return */
        } 
        else
        {
          /* send a reconfig failure messge with invalid
          configuration and 
          continue the same substate with any ongoing procedure */
          temp_failure_cause.failure_status = tdsrrc_rbr.status.failure_status ;
          temp_rejected_transaction = tdsrrc_rbr.tr_ids.rejected_transaction;
          
          tdsrrc_rbr.tr_ids.rejected_transaction = 
            (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
          if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
          {
            tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          }
          else
          {
            tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
          }
          /*Check whether Cell Update is going on */
          if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
               (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
          {
            if(tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbr subsstate(%d) Send failure with %d cause",tdsrrc_rbr.curr_substate,tdsrrc_rbr.status.failure_status);
              tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                       tdsrrc_rbr.status.prot_err_cause );
              tdsrrc_rbr.status.failure_status = temp_failure_cause.failure_status;
              tdsrrc_rbr.tr_ids.rejected_transaction = temp_rejected_transaction;
              
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
              (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
              /* Wait until Cell update procedure is completed */
              tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
            }
          }
          else
          {
            tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                            tdsrrc_rbr.status.prot_err_cause );
          }
        }
        /*do not clear the procedure or change the tdsrrc_rbe substate because
        some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
        break;

      default:
        /*should not come here */
        break;
    }
    if (ip_check_status != TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS)
    {
      /*do not clear the procedure or change the tdsrrc_rbr substate because
      some other tdsrrc_rbe procedure might be in progress. Just return from
      here */
      return;
    }
  }


  if ((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
      (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
      (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incomatible simultaneous reconfig! Rejecting RBR message");
     if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
            == tdsrrc_rbr.tr_ids.accepted_transaction )
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate RBR message - accepted tr-id matched!");
       /* Nothing else to do, just return */
     } 
     else if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                    == tdsrrc_rbr.tr_ids.rejected_transaction )
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate RBR message - rejected tr-id matched!");
       /* Nothing else to do, just return */
     } 
     else
     {
       /* We have received a RBR message with a new transaction id.
       This message should be in our rejected transaction
       list */
       tdsrrc_rbr.tr_ids.rejected_transaction = 
         (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
       tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;

       /*Check whether Cell Update is going on */
       if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
            (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
         if (SUCCESS != tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR))
         {
           TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBR failed to register with CU");
         }
         /* Wait until Cell update procedure is completed */
         tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
       }
       else
       {
         tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                         tdsrrc_rbr.status.prot_err_cause );
         /*continue the same substate */ 
       }
     }
     return;
  }

  /* We have received a RB release message with a new transaction id.
  Accept the transaction Id */
  tdsrrc_rbr.tr_ids.accepted_transaction =
     msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rb_release_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rb_release_ptr->m.activationTimePresent,
         rb_release_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBR SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbr_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rb_release_ptr->m.activationTimePresent = 0;
    }
  }

#endif

  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  tdsrrcrbr_validate_rbrelease_r5(msg_ptr);

  if ( tdsrrc_rbr.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBR Message validation failed");

    /* Send the RB Release Failure Message */
    tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                              tdsrrc_rbr.status.prot_err_cause
                            );

    tdsrrcrbr_clear_procedure();
    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
  }
  /* If the message is valid, proceed further */
  else
  {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    boolean                  additional_srb;
    uecomdef_logchan_e_type  dl_sdu_type = UE_LOGCHAN_NONE;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing RBR Message");

    tdsrrcrbr_process_high_mobility_ind_in_non_critical_ext(msg_ptr,TDSMSG_REL5);
    
    /*store the rb ids tobe released */
    tdsrrc_rbr.rbs_tobe_released.is_valid = TRUE;
    rb_info_rel_list_ptr = &rb_release_ptr->rb_InformationReleaseList;
    localrb_count = 0;
	local_idx =0;
    while ( rb_info_rel_list_ptr->n  > local_idx)
    {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      additional_srb = FALSE;

      if((tdsrrclcm_get_dl_lc_type(tdsrrclcm_get_dl_rlc_lc_id(rb_info_rel_list_ptr->elem[local_idx]), &dl_sdu_type)) == TDSRRCLCM_SUCCESS)
      {
        if(dl_sdu_type == UE_LOGCHAN_TM_DCCH_DL)
        {
          additional_srb = TRUE;
        }
      }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

      /*check whether rb id is signalling rbid or does not exist in established
      rabs */
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      if (additional_srb == FALSE)
      { 
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
        if((rb_info_rel_list_ptr->elem[local_idx] < DCCH_DT_LOW_PRI_RADIO_BEARER_ID))
        {
          /* RB Message was invalid. */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RB id:%d in RBR Message",rb_info_rel_list_ptr->elem[local_idx]);
          /* Send the RB Release Failure Message */
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                    tdsrrc_rbr.status.prot_err_cause
                                  );

          tdsrrcrbr_clear_procedure();
          /* Make sure we're back in the initial sub-state */
          tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
          return;

        }
        else if( rb_info_rel_list_ptr->elem[local_idx] == DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
        {
          if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND != tdsrrclcm_check_dl_rlc_lc_id(
                                           UE_LOGCHAN_DCCH,
                                           DCCH_DT_LOW_PRI_RADIO_BEARER_ID,
                                           UE_MODE_ACKNOWLEDGED_DATA))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Release SRB4");
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignore Non-exitent rb_id = %d in tdsrrc_rbr msg",rb_info_rel_list_ptr->elem[local_idx]);
            local_idx++;
            continue;
          }
        }
        else if(!tdsrrc_find_rb_in_est_rabs(rb_info_rel_list_ptr->elem[local_idx]))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignore Non-exitent rb_id in tdsrrc_rbr msg",rb_info_rel_list_ptr->elem[local_idx]);
          local_idx++;
          continue;
        }
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

      tdsrrc_rbr.rbs_tobe_released.rb_id[localrb_count] = rb_info_rel_list_ptr->elem[local_idx];
      localrb_count++;
      local_idx++;
    }
    tdsrrc_rbr.rbs_tobe_released.num_rbs = localrb_count;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Number ofRBs tobe released:%d",localrb_count);

    /*store the rb ids tobe affected */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
          tdsrrc_RadioBearerRelease_r5_IEs,rb_InformationAffectedList))
    {
      tdsrrc_rbr.rbs_tobe_affected.is_valid = TRUE;
      rb_info_affected_list_ptr = 
      &rb_release_ptr->rb_InformationAffectedList;
      localrb_count = 0;
      while ( rb_info_affected_list_ptr->n > localrb_count )
      {
        /*check whether rb id is signalling rbid or does not exist in established
        rabs */
        if(( rb_info_affected_list_ptr->elem[localrb_count].rb_Identity > DCCH_DT_LOW_PRI_RADIO_BEARER_ID)&&
           (! tdsrrc_find_rb_in_est_rabs(rb_info_affected_list_ptr->elem[localrb_count].rb_Identity)))
        {
          /* RB Message was invalid. */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RB id:%d in RBR Message",rb_info_affected_list_ptr->elem[localrb_count].rb_Identity);
          /* Send the RB Release Failure Message */
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                    tdsrrc_rbr.status.prot_err_cause
                                  );

          tdsrrcrbr_clear_procedure();
          /* Make sure we're back in the initial sub-state */
          tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
          return;

        }
        tdsrrc_rbr.rbs_tobe_affected.rb_id[localrb_count] = 
        rb_info_affected_list_ptr->elem[localrb_count].rb_Identity;
        localrb_count++;
      }
      tdsrrc_rbr.rbs_tobe_affected.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Number ofRBs tobe affected:%d",localrb_count);
    }


    /* If there is a signalling connection to be released,
    or if there are individual RBs to be released, we
    will be releasing some RBs. */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
        tdsrrc_RadioBearerRelease_r5_IEs,signallingConnectionRelIndication))
    {
      tdsrrcrbr_mark_cn_doamin_to_be_closed(rb_release_ptr->signallingConnectionRelIndication);
  
      for ( rab_count = 0; rab_count< MAX_RAB_TO_SETUP; rab_count++ )
      {
        if ( (tdsrrc_est_rabs.rabs[rab_count].cn_domain == tdsrrc_rbr.cn_to_release) &&
             (tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0) )
        {
          rab_found = TRUE;
          for ( rb_count=0; rb_count<tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab; rb_count++ )
          {
            rb_rel_srch = FALSE;
            for ( localrb_count=0; localrb_count<tdsrrc_rbr.rbs_tobe_released.num_rbs; localrb_count++ )
            {
              if ( tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id == 
                   tdsrrc_rbr.rbs_tobe_released.rb_id[localrb_count] )
              {
                rb_rel_srch = TRUE;
                break;
              }
            }
            if ( rb_rel_srch == FALSE ) // est rabs has an rb_id not in the rb release list
            {
              /* send failure message */
              tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
              tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                        tdsrrc_rbr.status.prot_err_cause);
              /* Go back to initial substate */
              tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
              /* Then clear all local variables that may
              need to be cleared */
              tdsrrcrbr_clear_procedure();
              return;
            }
          }/* rb_count loop */       
        } /*end of if loop */
      }/* rab_count loop */
      /*if none of the rabs in the establishedrabs have rb information*/
      if ( rab_found==FALSE )
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB lookup in est_rabs for cn:%d failed",tdsrrc_rbr.cn_to_release );
        /* send failure message */
        tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause);
        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbr_clear_procedure();
        return;

      }
    }

    /* Check if there is CN information present. If so, we need
    to send the information to the Cell Change manager */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r5_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
    
      /*Check to see if primary PLMN I.D is present in RBR message ext
       * tdsrrc_RadioBearerRelease-v690ext which is present in 
       * v690NonCriticalExtensions
       */
       primary_plmn_Id_present = tdsrrcrbr_check_and_get_primary_plmn_id(msg_ptr,
                                                                  &primary_plmn_Identity,TDSMSG_REL5);
    
      tdsrrc_rbr.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info(
                     &(rb_release_ptr->cn_InformationInfo) 
                     ,primary_plmn_Id_present
                     ,&primary_plmn_Identity
        );
      if (tdsrrc_rbr.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbr.cn_info_saved = TRUE;
      }
    }

    /* Indicate layer1 if rb release message is going to release voice RAB */
    tdsrrcrbr_indicate_layer1_if_config_msg_release_voice_rab();

    tdsrrcrb_update_mac_rab_status(TDSRRC_PROCEDURE_RBR, RRC_CS_DOMAIN_CN_ID,
                                &rb_release_ptr->rb_InformationReleaseList,
                                FALSE);

    /* Set TDSORDERED_CONFIG so that LLC can process the
    necessary parameters. */
    oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBR,
                                           tdsrrc_rbr.state_indicated_by_msg,
                                           tdsrrc_DL_DCCH_Message_PDU,
                                           (void *)dcch_msg_ptr
                                         );

    switch ( oc_status )
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
       /* Populate the local variables to indicate if Re-direction info is present */
        directed_cell_info.freq_info_present = FALSE;
        directed_cell_info.cpid_present = FALSE; 
        if((current_state==TDSRRC_STATE_CELL_FACH) && 
               ((tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
               ||(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
               ||(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
          if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
                  tdsrrc_RadioBearerRelease_r5_IEs,frequencyInfo)) &&
                 (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->frequencyInfo.modeSpecificInfo,
                 rrc_FrequencyInfo_modeSpecificInfo_tdd)))
          {
            directed_cell_info.freq_info_present = TRUE;
            directed_cell_info.uarfcn_dl  = rb_release_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
          }
         
          if (tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL5))
          {
            if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r5_IEs,dl_InformationPerRL_List)) &&
              (rb_release_ptr->dl_InformationPerRL_List.n != 0))
            {
              
              directed_cell_info.cpid_present = TRUE;
              directed_cell_info.cell_param_id = rb_release_ptr->dl_InformationPerRL_List.elem[0].
                modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
            }
          }      
          /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
                 FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
          if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
            (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
            if(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
            {
              tdsrrcrbr_send_initiate_cu_flag = TRUE;
              tdsrrcrbr_delete_crnti          = TRUE;
            }
            else
            {
              tdsrrc_rbr.directed_cell_info = TRUE;
            }
          }  
        }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
        if (
            (current_state==tdsrrc_rbr.state_indicated_by_msg) ||
            ((current_state==TDSRRC_STATE_CELL_FACH) && 
             (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH))
            )
        {
          /* CELL_DCH->CELL_DCH or CELL_FACH->CELL_FACH or 
          CELL_FACH->CELL_DCH case. We can release the
          Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
          /*check whether any frequency info is present in the message
          and transition is CELL_FACH->CELL_FACH. In this case
          initiate a cell update after channel config is successfully completed */
          if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
             (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
          {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
             /*call the function that evaluates the directed cell info and set the flags accordingly */
            if ((tdsrrcrbr_send_initiate_cu_flag == FALSE) &&
                (tdsrrcrb_validate_directed_cell_info (directed_cell_info)== FALSE))
            {
              if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
              tdsrrc_RadioBearerRelease_r5_IEs,new_C_RNTI))
              {
                tdsrrc_rbr.new_crnti_valid = TRUE;
                tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, 
                                    &c_rnti);
                tdsrrc_rbr.new_crnti = (uint16)c_rnti;
              }
              else
              {
                tdsrrc_rbr.new_crnti_valid = FALSE;
              }
   
              if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
              {
                if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
                {      
                  (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
                }
                else
                {
                  (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
                }
                tdsrrc_rbr.need_to_release_cn_domain = FALSE;
              }
              tdsrrcllcoc_update_toc_with_oc_from_fach();
              /*Initiate the cell selection*/
              tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR,
                                                       TDSRRC_TRANSITION_FROM_FACH,
                                                       tdsrrc_rbr.state_indicated_by_msg);

              tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

              /* exit from the switch case */
              break;
     
            }
     #else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
            if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
                tdsrrc_RadioBearerRelease_r5_IEs,frequencyInfo)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->frequencyInfo.modeSpecificInfo,
               rrc_FrequencyInfo_modeSpecificInfo_tdd)))
            {
              directed_cell_info.freq_info_present = TRUE;
              directed_cell_info.uarfcn_dl  = rb_release_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
            }
            else
            {
              directed_cell_info.freq_info_present = FALSE;
            }
            if (tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL5))
            {
              if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r5_IEs,dl_InformationPerRL_List)) &&
                (rb_release_ptr->dl_InformationPerRL_List.n != 0))
              {
                
                directed_cell_info.cpid_present = TRUE;
                directed_cell_info.cell_param_id = rb_release_ptr->dl_InformationPerRL_List.elem[0].
                  modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
              }
            }
            else
            {
              directed_cell_info.cpid_present = FALSE;
            }
            /*call the function that evaluates the directed cell info and set the flags accordingly */
            if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
            {
              /*directed cell info matched. Cell update is not required */
              tdsrrcrbr_send_initiate_cu_flag = FALSE;
            }
            else
            {
              /*directed cell info do not match. Cell update is required */
              tdsrrcrbr_send_initiate_cu_flag = TRUE;
              tdsrrcrbr_delete_crnti          = TRUE;
            }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
          } /* if CELL_FACH->CELL_FACH reconfiguration */

          /* Call the function that creates and sends a channel
            config request to LLC. */
          tdsrrcrbr_send_channel_config_req();

          /* Change the substate since we're waiting for the
             confirmation from LLC for the channel configs */
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
        {
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
             tdsrrc_RadioBearerRelease_r5_IEs,new_C_RNTI))
          {
            tdsrrc_rbr.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, 
                                &c_rnti);
            tdsrrc_rbr.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_rbr.new_crnti_valid = FALSE;
          }

          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR,
                                                  TDSRRC_TRANSITION_FROM_DCH,
                                                  tdsrrc_rbr.state_indicated_by_msg);
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

          /* Start T_305 [periodic cell update] timer */
          tdsrrccu_start_t_305_timer();

        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                 (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
        {
          /*store the directed cell info */
           tdsrrc_rbr.directed_cell_info = tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL5);

           /*Send the tdsrrc_rbr complete msg on old configuration */
           tdsrrcrbr_send_rbr_complete_message(TRUE);
           /*wait for L2ack */
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
           tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
           tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
        {
          /*Send the RBR complete msg on old configuration */
          tdsrrcrbr_send_rbr_complete_message(TRUE);
          /*wait for L2ack */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
          tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_FACH) && 
                ((tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
                 (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if ((tdsrrc_rbr.directed_cell_info == FALSE)
            && (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
          {
            if(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
            {
              tdsrrc_rbr.directed_cell_info = tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL5);
            }

            tdsrrc_rbr.initiate_cell_selection_trans_from_fach  = TRUE;
          }
    
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

          /* check whether CPICH info is present or not. Store this
          info  */
         // tdsrrc_rbr.directed_cell_info = rrcrbr_is_pcpich_info_present(msg_ptr);
          /*Send the RRCTCTC complete msg on old configuration */
          /* check whether new C-RNTI was present in the message. In that case
          we have to configure MAC with new C-RNTI before sending the response 
          message */
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
             tdsrrc_RadioBearerRelease_r5_IEs,new_C_RNTI))
          {
            tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, 
                                &c_rnti);
            /* Send a RNTI Update Request */
            rnti_update_info.crnti_action    = TDSRRC_RNTI_UPDATE;
            rnti_update_info.crnti           = (uint16)c_rnti;
            /* check whether URNTI is also present in the message */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
             tdsrrc_RadioBearerRelease_r5_IEs,new_U_RNTI))
            {
              tdsrrc_translate_urnti(&rb_release_ptr->new_U_RNTI, 
                                  &u_rnti);
              rnti_update_info.urnti_action    = TDSRRC_RNTI_UPDATE; 
              rnti_update_info.urnti           = u_rnti; 
            }
            else
            {
              rnti_update_info.urnti_action    = TDSRRC_RNTI_NOCHANGE; 
            }
            rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBR;
            rnti_update_info.rlc_re_establish_srb = FALSE;
            rnti_update_info.rlc_re_establish_rab = FALSE;
            rnti_update_info.cnf_reqd             = TRUE;
            tdsrrcrb_send_rnti_update_req(rnti_update_info);
            /* wait for the RNTI update confirm before sending the response
            message */
            tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
          }
          else 
          {
            tdsrrcrbr_send_rbr_complete_message(TRUE);
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
            /*wait for L2ack */
            tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
            tdsrrcllc_print_trans_chnl_rate();
          }
        }
        break;

      case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
        tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
        tdsrrc_rbr.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_rbr.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_NOT_SET_OTHER:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
        /* When LLC sends this, there is a serious problem
        and we'll go back to disconnected state. No need
        to send the failure message here, we can just
        go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      default:

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
        /* Unknown error, just go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
    }/* end switch */

    if ( ordered_config_err_occured == TRUE )
    {
        /* No need to continue. Clear the procedure
        variables and return. */
        tdsrrcrbr_clear_procedure();
        return;
    }


    if(tdsrrc_rbr.rbr_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }

      
  }/* end if - valid rb message */
}

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/*===========================================================================

FUNCTION   RRCRBR_PROCESS_NEW_CELL_IND

DESCRIPTION

  This function processes the received new cell ind command.
  <more later>.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_process_new_cell_ind
(
  tdsrrc_new_cell_ind_type *ptr /* Pointer to the State Change Ind cmd */
)
{
     
  if(tdsrrccsp_is_selected_cell_utran_directed()&&
      (tdsrrc_rbr.new_crnti_valid))  
  {
    /* No need to initiate cell update as the C-RNTI is available
     * and UE is camped on the directed cell.
     */
    tdsrrcrbr_send_initiate_cu_flag = FALSE;
  }
  else
  {
    tdsrrcrbr_send_initiate_cu_flag = TRUE;
    tdsrrcrbr_delete_crnti          = TRUE;
  }
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
    /* Call the function that creates and sends the  channel
    config information to be requested from LLC. */
  tdsrrcrbr_send_channel_config_req();
  
  
    /* Change the substate since we're waiting for the
    confirmation from LLC for the channel configs */
    tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
}

#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

/*===========================================================================

FUNCTION   RRCRBR_PROCESS_RBRELEASE_R7

DESCRIPTION

  This function processes the received Rel 6 RB Release Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Release Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to release the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Release Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_process_rbrelease_r7
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_RB_InformationReleaseList  *rb_info_rel_list_ptr;
  tdsrrc_RB_InformationAffectedList_r7 *rb_info_affected_list_ptr;
   tdsrrc_RB_InformationReconfig_r7 *rb_info_reconfig_ptr;
  tdsrrc_RadioBearerRelease *msg_ptr;  /* Pointer to the RB Release message        */
  tdsrrcrb_directed_cell_info_type directed_cell_info; /* directed cell info struct */

  tdsrrcllc_oc_status_e_type oc_status;/* To hold tdsordered_config status          */
  boolean ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                    occured while setting tdsordered_config. */
  boolean rb_rel_srch = FALSE; 
  boolean rab_found = FALSE; 
  tdsrrc_state_e_type current_state;       /* current RRC State  */
  uint32 rab_count, rb_count, localrb_count,local_idx = 0;
  /* Local C_RNTI value */
  tdsrrc_int_c_rnti_type              c_rnti;
  tdsrrc_int_u_rnti_type              u_rnti;
  tdsrrcrb_rnti_update_info_type      rnti_update_info;   /* to hold rnti update info */

  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL7;


  tdsrrcsmc_srns_ip_check_status_e_type    ip_check_status = TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;


  tdsrrc_DL_DCCH_Message *dcch_msg_ptr ;     /* Pointer to the RB Release message */

  /* Store parameter IEs */
  tdsrrc_RadioBearerRelease_r7_IEs* rb_release_ptr = NULL;
  
  tdsrrcrb_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;


  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Release message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerRelease);


  /* Store Msg Ptr in a local variable */
  rb_release_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
    u.criticalExtensions.u.criticalExtensions.u.r7.radioBearerRelease_r7;



  if (TRUE == tdsrrcrbr_validate_srns_relocation_info(msg_ptr, msg_version))
  {
    ip_check_status = tdsrrcsmc_check_integrity_for_srns_relocation(cmd_ptr, msg_version);
  
    switch (ip_check_status)
    {
      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS:
        /*continue further processing with the message */
        break;

      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE:
        /*Discard the message and continue with ongoing procedure as if this
        message was not received */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IP check failed. Discarding RBS msg");
        break;

      case TDSRRCSMC_SRNS_SECURITY_INFO_INVALID:
      case TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION:
        if (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier
                       == tdsrrc_rbr.tr_ids.rejected_transaction)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid security config-rejected tr-id%d match!",tdsrrc_rbr.tr_ids.rejected_transaction);
          /* Nothing else to do, just ignore the message and return */
        } 
        else
        {
          /* send a reconfig failure messge with invalid
          configuration and 
          continue the same substate with any ongoing procedure */
          temp_failure_cause.failure_status = tdsrrc_rbr.status.failure_status ;
          temp_rejected_transaction = tdsrrc_rbr.tr_ids.rejected_transaction;
          
          tdsrrc_rbr.tr_ids.rejected_transaction = 
            (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
          if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
          {
            tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          }
          else
          {
            tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
          }
          /*Check whether Cell Update is going on */
          if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
               (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
          {
            if(tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbr subsstate(%d) Send failure with %d cause",tdsrrc_rbr.curr_substate,tdsrrc_rbr.status.failure_status);
              tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                       tdsrrc_rbr.status.prot_err_cause );
              tdsrrc_rbr.status.failure_status  = temp_failure_cause.failure_status;
              tdsrrc_rbr.tr_ids.rejected_transaction = temp_rejected_transaction ;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
              (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
              /* Wait until Cell update procedure is completed */
              tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
            }
          }
          else
          {
            tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                            tdsrrc_rbr.status.prot_err_cause );
          }
        }
        /*do not clear the procedure or change the tdsrrc_rbe substate because
        some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
        break;

      default:
        /*should not come here */
        break;
    }
    if (ip_check_status != TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS)
    {
      /*do not clear the procedure or change the tdsrrc_rbr substate because
      some other tdsrrc_rbe procedure might be in progress. Just return from
      here */
      return;
    }
  }


  if ((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
      (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
      (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incomatible simultaneous reconfig! Rejecting RBR message");
     if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
            == tdsrrc_rbr.tr_ids.accepted_transaction )
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate RBR message - accepted tr-id matched!");
       /* Nothing else to do, just return */
     } 
     else if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                    == tdsrrc_rbr.tr_ids.rejected_transaction )
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate RBR message - rejected tr-id matched!");
       /* Nothing else to do, just return */
     } 
     else
     {
       /* We have received a RBR message with a new transaction id.
       This message should be in our rejected transaction
       list */
       temp_failure_cause.failure_status = tdsrrc_rbr.status.failure_status ;
       temp_rejected_transaction = tdsrrc_rbr.tr_ids.rejected_transaction;
       
       tdsrrc_rbr.tr_ids.rejected_transaction = 
         (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
       tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;

       /*Check whether Cell Update is going on */
       if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
            (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
       {
         if(tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
         {
           TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbr subsstate(%d) Send failure with %d cause",tdsrrc_rbr.curr_substate,tdsrrc_rbr.status.failure_status);
           tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                    tdsrrc_rbr.status.prot_err_cause );
           
           tdsrrc_rbr.status.failure_status  = temp_failure_cause.failure_status;
           tdsrrc_rbr.tr_ids.rejected_transaction = temp_rejected_transaction;
         }
         else
         {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
           (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
           /* Wait until Cell update procedure is completed */
           tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
         }
       }
       else
       {
         tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                         tdsrrc_rbr.status.prot_err_cause );
         /*continue the same substate */ 
       }
     }
     return;
  }

  /* We have received a RB release message with a new transaction id.
  Accept the transaction Id */
  tdsrrc_rbr.tr_ids.accepted_transaction =
     msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rb_release_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rb_release_ptr->m.activationTimePresent,
         rb_release_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBR SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbr_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rb_release_ptr->m.activationTimePresent = 0;
    }
  }

#endif

  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  tdsrrcrbr_validate_rbrelease_r7(msg_ptr);

  if ( tdsrrc_rbr.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBR Message validation failed");

    /* Send the RB Release Failure Message */
    tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                              tdsrrc_rbr.status.prot_err_cause
                            );

    tdsrrcrbr_clear_procedure();
    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
  }
  /* If the message is valid, proceed further */
  else
  {

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing RBR Message");

    if(TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,ueMobilityStateIndicator))
    {
      if(rb_release_ptr->ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in RBR.");
        tdsrrc_rbr.high_mobility_ind = TRUE;
      }
    }
    /*store the rb ids tobe released */
    tdsrrc_rbr.rbs_tobe_released.is_valid = TRUE;
    rb_info_rel_list_ptr = &rb_release_ptr->rb_InformationReleaseList;
    localrb_count = 0;
    local_idx=0;
    while ( rb_info_rel_list_ptr->n > local_idx)
    {
      /*check whether rb id is signalling rbid or does not exist in established
      rabs */
      if( rb_info_rel_list_ptr->elem[local_idx] < DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
        /* RB Message was invalid. */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RB id:%d in RBR Message",rb_info_rel_list_ptr->elem[local_idx]);
        /* Send the RB Release Failure Message */
        tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        tdsrrcrbr_clear_procedure();
        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
        return;

      }
      else if( rb_info_rel_list_ptr->elem[local_idx] == DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
        if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND != tdsrrclcm_check_dl_rlc_lc_id(
                                         UE_LOGCHAN_DCCH,
                                         DCCH_DT_LOW_PRI_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Release SRB4");
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignore Non-exitent rb_id = %d in tdsrrc_rbr msg",rb_info_rel_list_ptr->elem[local_idx]);
		      local_idx++;
		      continue;
        }
      }
      else if (!tdsrrc_find_rb_in_est_rabs(rb_info_rel_list_ptr->elem[local_idx]))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignore Non-exitent rb_id =%d in tdsrrc_rbr msg",rb_info_rel_list_ptr->elem[local_idx]);
        local_idx++;
        continue;
      }

      tdsrrc_rbr.rbs_tobe_released.rb_id[localrb_count] = rb_info_rel_list_ptr->elem[local_idx];
      localrb_count++;
      local_idx++;
    }
    tdsrrc_rbr.rbs_tobe_released.num_rbs = localrb_count;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Number ofRBs tobe released:%d",localrb_count);

    /*store the rb ids tobe reconfigured */
    
    if (TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,rb_InformationReconfigList))
    {
      tdsrrc_rbr.rbs_tobe_reconfigured.is_valid = TRUE;
            localrb_count = 0;
      while ( rb_release_ptr->rb_InformationReconfigList.n > localrb_count )
      {
      
	   rb_info_reconfig_ptr = 
			  &rb_release_ptr->rb_InformationReconfigList.elem[localrb_count];
        /* Fill up the RB id and indicate that it needs to be reconfigured */
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_id= 
          rb_info_reconfig_ptr->rb_Identity;
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_config = TDSRECONFIG_RB;
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask = tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask = 0;
        if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,tdsrrc_RB_InformationReconfig_r7,rlc_Info))&&
            (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode)))
        {
          tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask = TDSMODIFY_RB_CFG;
        }
        if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,tdsrrc_RB_InformationReconfig_r7,rlc_Info))&&
            (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)))
        {
          tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
        }
 
        if (  TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
			tdsrrc_RB_InformationReconfig_r7,rb_StopContinue))
        {
          /* over-ride rb_config with STOP or continue */
          if (tdsrrc_RB_StopContinue_stopRB == rb_info_reconfig_ptr->rb_StopContinue)
          {
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask |= TDSSTOP_RB_CFG;
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask |= TDSSTOP_RB_CFG;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"rb-reconfig-info rb %d to STOP rb_count %d",tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_id,localrb_count);
          }
          else if (tdsrrc_RB_StopContinue_continueRB == rb_info_reconfig_ptr->rb_StopContinue)
          {
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask |= TDSCONTINUE_RB_CFG;
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask |= TDSCONTINUE_RB_CFG;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"rb-reconfig-info rb %d to CONTINUE rb_count %d",tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_id,localrb_count);
            
          }
        }

        localrb_count++;
        /* Go to the next reconfigured RB */
      }

      tdsrrc_rbr.rbs_tobe_reconfigured.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Number ofRBs tobe reconfigured:%d",localrb_count);
    }

    /*store the rb ids tobe affected */
   
    if ( TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,rb_InformationAffectedList))
    {
      tdsrrc_rbr.rbs_tobe_affected.is_valid = TRUE;
      rb_info_affected_list_ptr = 
      &rb_release_ptr->rb_InformationAffectedList;
      localrb_count = 0;
      while ( rb_info_affected_list_ptr->n > localrb_count )
      {
        /*check whether rb id is signalling rbid or does not exist in established
        rabs */
        if(( rb_info_affected_list_ptr->elem[localrb_count].rb_Identity > DCCH_DT_LOW_PRI_RADIO_BEARER_ID)&&
           (! tdsrrc_find_rb_in_est_rabs(rb_info_affected_list_ptr->elem[localrb_count].rb_Identity)))
        {
          /* RB Message was invalid. */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RB id:%d in RBR Message",rb_info_affected_list_ptr->elem[localrb_count].rb_Identity);
          /* Send the RB Release Failure Message */
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                    tdsrrc_rbr.status.prot_err_cause
                                  );

          tdsrrcrbr_clear_procedure();
          /* Make sure we're back in the initial sub-state */
          tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
          return;

        }
        tdsrrc_rbr.rbs_tobe_affected.rb_id[localrb_count] = 
        rb_info_affected_list_ptr->elem[localrb_count].rb_Identity;
        localrb_count++;
        
      }
      tdsrrc_rbr.rbs_tobe_affected.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Number ofRBs tobe affected:%d",localrb_count);
    }


    /* If there is a signalling connection to be released,
    or if there are individual RBs to be released, we
    will be releasing some RBs. */
     
    if (TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,signallingConnectionRelIndication))
    {
      tdsrrcrbr_mark_cn_doamin_to_be_closed(rb_release_ptr->signallingConnectionRelIndication);
  
      for ( rab_count = 0; rab_count< MAX_RAB_TO_SETUP; rab_count++ )
      {
        if ( (tdsrrc_est_rabs.rabs[rab_count].cn_domain == tdsrrc_rbr.cn_to_release) &&
             (tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0) )
        {
          rab_found = TRUE;
          for ( rb_count=0; rb_count<tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab; rb_count++ )
          {
            rb_rel_srch = FALSE;
            for ( localrb_count=0; localrb_count<tdsrrc_rbr.rbs_tobe_released.num_rbs; localrb_count++ )
            {
              if ( tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id == 
                   tdsrrc_rbr.rbs_tobe_released.rb_id[localrb_count] )
              {
                rb_rel_srch = TRUE;
                break;
              }
            }
            if ( rb_rel_srch == FALSE ) // est rabs has an rb_id not in the rb release list
            {
              /* send failure message */
              tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
              tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                        tdsrrc_rbr.status.prot_err_cause);
              /* Go back to initial substate */
              tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
              /* Then clear all local variables that may
              need to be cleared */
              tdsrrcrbr_clear_procedure();
              return;
            }
          }/* rb_count loop */       
        } /*end of if loop */
      }/* rab_count loop */
      /*if none of the rabs in the establishedrabs have rb information*/
      if ( rab_found==FALSE )
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB lookup in est_rabs for cn:%d failed",tdsrrc_rbr.cn_to_release );
        /* send failure message */
        tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause);
        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbr_clear_procedure();
        return;

      }
    }

    /* Check if there is CN information present. If so, we need
    to send the information to the Cell Change manager */
    if (TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      tdsrrc_rbr.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(
        &(rb_release_ptr->cn_InformationInfo) );
      if (tdsrrc_rbr.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbr.cn_info_saved = TRUE;
      }
    }

    /* Indicate layer1 if rb release message is going to release voice RAB */
    tdsrrcrbr_indicate_layer1_if_config_msg_release_voice_rab();

    tdsrrcrb_update_mac_rab_status(TDSRRC_PROCEDURE_RBR, RRC_CS_DOMAIN_CN_ID,
                                &rb_release_ptr->rb_InformationReleaseList,
                                FALSE);

    /* Set TDSORDERED_CONFIG so that LLC can process the
    necessary parameters. */
    oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBR,
                                           tdsrrc_rbr.state_indicated_by_msg,
                                           tdsrrc_DL_DCCH_Message_PDU,
                                           (void *)dcch_msg_ptr
                                         );

    switch ( oc_status )
    {
      case TDSORDERED_CONFIG_SET:
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
        if(tdsrrcsib_is_current_event_sib_read_in_dch())
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Force Init GPS ");
           tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
        }
#endif
        current_state = tdsrrc_get_state(); 
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
       /* Populate the local variables to indicate if Re-direction info is present */
        directed_cell_info.freq_info_present = FALSE;
        directed_cell_info.cpid_present = FALSE; 
        if((current_state==TDSRRC_STATE_CELL_FACH) && 
               ((tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
               ||(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
               ||(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
          if((TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,frequencyInfo))
             &&(TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->frequencyInfo.modeSpecificInfo,
             rrc_FrequencyInfo_modeSpecificInfo_tdd)))
          {
            directed_cell_info.freq_info_present = TRUE;
            directed_cell_info.uarfcn_dl  = rb_release_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
          }
         
          if (tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL7))
          {
            if ((TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,dl_InformationPerRL_List))
                && (rb_release_ptr->dl_InformationPerRL_List.n != 0))
            {
              
              directed_cell_info.cpid_present = TRUE;
              directed_cell_info.cell_param_id = rb_release_ptr->dl_InformationPerRL_List.elem[0].
                modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
            }
          }      
          /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
                 FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
          if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
            (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
            if(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
            {
              tdsrrcrbr_send_initiate_cu_flag = TRUE;
              tdsrrcrbr_delete_crnti          = TRUE;
            }
            else
            {
              tdsrrc_rbr.directed_cell_info = TRUE;
            }
          }  
        }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

        if (
            (current_state==tdsrrc_rbr.state_indicated_by_msg) ||
            ((current_state==TDSRRC_STATE_CELL_FACH) && 
             (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH))
            )
        {
          /* CELL_DCH->CELL_DCH or CELL_FACH->CELL_FACH or 
          CELL_FACH->CELL_DCH case. We can release the
          Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
          /*check whether any frequency info is present in the message
          and transition is CELL_FACH->CELL_FACH. In this case
          initiate a cell update after channel config is successfully completed */
          if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
             (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
          {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
             /*call the function that evaluates the directed cell info and set the flags accordingly */
            if ((tdsrrcrbr_send_initiate_cu_flag == FALSE) &&
                (tdsrrcrb_validate_directed_cell_info (directed_cell_info)== FALSE))
            {
              if(TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,new_C_RNTI))
              {
                tdsrrc_rbr.new_crnti_valid = TRUE;
                tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, 
                                    &c_rnti);
                tdsrrc_rbr.new_crnti = (uint16)c_rnti;
              }
              else
              {
                tdsrrc_rbr.new_crnti_valid = FALSE;
              }
   
              if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
              {
                if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
                {      
                  (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
                }
                else
                {
                  (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
                }
                tdsrrc_rbr.need_to_release_cn_domain = FALSE;
              }
              tdsrrcllcoc_update_toc_with_oc_from_fach();
              /*Initiate the cell selection*/
              tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR,
                                                       TDSRRC_TRANSITION_FROM_FACH,
                                                       tdsrrc_rbr.state_indicated_by_msg);

              tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

              /* exit from the switch case */
              break;
     
            }
     #else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/       
            if((TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,frequencyInfo))
                &&(TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->frequencyInfo.modeSpecificInfo,
                rrc_FrequencyInfo_modeSpecificInfo_tdd)))
            {
              directed_cell_info.freq_info_present = TRUE;
              directed_cell_info.uarfcn_dl  = rb_release_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
            }
            else
            {
              directed_cell_info.freq_info_present = FALSE;
            }
            if (tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL7))
            {

              if ((TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,dl_InformationPerRL_List))
                &&  (rb_release_ptr->dl_InformationPerRL_List.n != 0))
              {
                
                directed_cell_info.cpid_present = TRUE;
                directed_cell_info.cell_param_id = rb_release_ptr->dl_InformationPerRL_List.elem[0].
                  modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
              }
            }
            else
            {
              directed_cell_info.cpid_present = FALSE;
            }
            /*call the function that evaluates the directed cell info and set the flags accordingly */
            if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
            {
              /*directed cell info matched. Cell update is not required */
              tdsrrcrbr_send_initiate_cu_flag = FALSE;
            }
            else
            {
              /*directed cell info do not match. Cell update is required */
              tdsrrcrbr_send_initiate_cu_flag = TRUE;
              tdsrrcrbr_delete_crnti          = TRUE;
            }
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
          } /* if CELL_FACH->CELL_FACH reconfiguration */

          /* Call the function that creates and sends a channel
            config request to LLC. */
          tdsrrcrbr_send_channel_config_req();

          /* Change the substate since we're waiting for the
             confirmation from LLC for the channel configs */
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
        {
            
          if(TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,new_C_RNTI))
          {
            tdsrrc_rbr.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, 
                                &c_rnti);
            tdsrrc_rbr.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_rbr.new_crnti_valid = FALSE;
          }

          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR,
                                                  TDSRRC_TRANSITION_FROM_DCH,
                                                  TDSRRC_STATE_CELL_FACH);
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

          /* Start T_305 [periodic cell update] timer */
          tdsrrccu_start_t_305_timer();		  
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                 (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
        {
          /*store the directed cell info */
           tdsrrc_rbr.directed_cell_info = tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL7);

           /*Send the tdsrrc_rbr complete msg on old configuration */
           tdsrrcrbr_send_rbr_complete_message(TRUE);
           /*wait for L2ack */
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
           tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
           tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
        {
          /*Send the RBR complete msg on old configuration */
          tdsrrcrbr_send_rbr_complete_message(TRUE);
          /*wait for L2ack */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
          tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_FACH) && 
                ((tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
                 (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
          /* check whether CPICH info is present or not. Store this
          info  */
         // tdsrrc_rbr.directed_cell_info = rrcrbr_is_pcpich_info_present(msg_ptr,TDSMSG_REL7);
          /*Send the RRCTCTC complete msg on old configuration */
          /* check whether new C-RNTI was present in the message. In that case
          we have to configure MAC with new C-RNTI before sending the response 
          message */
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if ((tdsrrc_rbr.directed_cell_info == FALSE) &&
            (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
          {
            if(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
            {
              tdsrrc_rbr.directed_cell_info = tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL7);
            }
            tdsrrc_rbr.initiate_cell_selection_trans_from_fach  = TRUE;
          }
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
     
          if(TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,new_C_RNTI))
          {
            tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, 
                                &c_rnti);
            /* Send a RNTI Update Request */
            rnti_update_info.crnti_action    = TDSRRC_RNTI_UPDATE;
            rnti_update_info.crnti           = (uint16)c_rnti;
            /* check whether URNTI is also present in the message */
            
            if (TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,new_U_RNTI))
            {
              tdsrrc_translate_urnti(&rb_release_ptr->new_U_RNTI, 
                                  &u_rnti);
              rnti_update_info.urnti_action    = TDSRRC_RNTI_UPDATE; 
              rnti_update_info.urnti           = u_rnti; 
            }
            else
            {
              rnti_update_info.urnti_action    = TDSRRC_RNTI_NOCHANGE; 
            }
            rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBR;
            rnti_update_info.rlc_re_establish_srb = FALSE;
            rnti_update_info.rlc_re_establish_rab = FALSE;
            rnti_update_info.cnf_reqd             = TRUE;
            tdsrrcrb_send_rnti_update_req(rnti_update_info);
            /* wait for the RNTI update confirm before sending the response
            message */
            tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
          }
          else 
          {
            tdsrrcrbr_send_rbr_complete_message(TRUE);
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
            /*wait for L2ack */
            tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
            tdsrrcllc_print_trans_chnl_rate();
          }
        }
        break;

      case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
        tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
        tdsrrc_rbr.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_rbr.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
      
      case TDSORDERED_CONFIG_NOT_SET_OTHER:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
        /* When LLC sends this, there is a serious problem
        and we'll go back to disconnected state. No need
        to send the failure message here, we can just
        go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      default:

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
        /* Unknown error, just go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
    }/* end switch */

    if ( ordered_config_err_occured == TRUE )
    {
        /* No need to continue. Clear the procedure
        variables and return. */
        tdsrrcrbr_clear_procedure();
        return;
    }


    if(tdsrrc_rbr.rbr_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }

      
  }/* end if - valid rb message */
}

#ifdef FEATURE_TDSCDMA_REL8

/*===========================================================================

FUNCTION   tdsrrcrbr_update_rab_info_reconfiglist_r8

DESCRIPTION Updates the RAB information received in the RAB_InformationReconfigList IE in 
                      RB Release message.
                      It updates the NAS Syncronisation Indicator field received in the RB Release message.
                      In case the NAS Syncronisation Indicator IE is not present , then a default value of 
                      current codec  used is updated.



DEPENDENCIES
  None.

RETURN VALUE
 None

SIDE EFFECTS
  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrcrbr_update_rab_info_reconfiglist_r8
(
  struct tdsrrc_RAB_InformationReconfigList_r8 *rab_list_ptr
)
{
  uint32 rab_index=0;  
  rrc_rab_id_type rab_id;           /* To store RAB id in internal RRC format */
  rrcmn_rab_status_T rab_type = RRCMN_INVALID_RAB_ID;     

  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();
                                    
  if(rab_list_ptr != NULL)
  {
    for (rab_index=0; (rab_list_ptr->n > rab_index) && (rab_index < TDSRRCRB_MAX_RAB_PER_MSG); rab_index++)
    {
      rab_id = tdsrrcrb_translate_gsm_map_rab_id((rab_list_ptr->elem[rab_index].rab_Identity.u.gsm_MAP_RAB_Identity));

      /*RRC_GKG: Here we can add the validation for RAB-Id from EST_RABs*/
      tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_id_is_gsm_map = TRUE;
      tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_id = rab_id;
      
      /* Check if this RAB is CS Domain or PS Domain */  
      if(rab_list_ptr->elem[rab_index].cn_DomainIdentity == RRC_CS_DOMAIN_CN_ID )
      {
        /* Check if it is in loopback test */
        if (tdsrrclbt_is_lb_on() == TRUE )
        {
          /* Set Rab type to Data */
          TDSRRC_MSG0(MSG_LEGACY_LOW,"Loopback Test");
          tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
        }
        else
        {
           /* For CS RABs, Call MN layer to verify this RAB */
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
           rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
           rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif
          tdsrrc_rbr.rabs.rabs_in_msg[rab_index].cn_domain =RRC_CS_DOMAIN_CN_ID;

          TDSRRC_MSG3(MSG_LEGACY_HIGH,"For RAB-Id %d, RAB type from NAS = %d, RAB type from EST_RABs = %d",rab_id, rab_type,
            tdsrrc_get_rab_type_for_given_rab_id(tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_id));

          if( rab_type == RRCMN_CS_VOICE_CALL )
          {
            tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_VOICE_RAB;
          }
          else
          {
            tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
          }

#ifdef FEATURE_SCUDIF_ENHANCEMENT
          #error code not present
#else
          if(rab_type == RRCMN_CS_VOICE_CALL)
#endif
          {
            nas_sync_ind =  tdsrrc_translate_nas_sync_indicator(&(rab_list_ptr->elem[rab_index].nas_Synchronisation_Indicator));
            if( nas_sync_ind == TDSCDMA_IVOCODER_ID_NONE)
            {
              tdsrrc_rbr.status.failure_status= TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Unsupported CODEC type %d",nas_sync_ind );
              return FAILURE;
            }
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
            tdsrrc_set_ordered_codec_status(TRUE);
#endif
          }
        }   
      }
      else
      {
        tdsrrc_rbr.rabs.rabs_in_msg[rab_index].cn_domain =RRC_PS_DOMAIN_CN_ID;
        tdsrrc_rbr.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_PS_DATA_RAB;
      }
      tdsrrc_rbr.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator = nas_sync_ind;
    } 
  }
  
  tdsrrc_rbr.rabs.num_rabs_in_msg = rab_index;

  tdsrrc_set_ordered_codec_mode(nas_sync_ind);

  return SUCCESS;
}

/*===========================================================================
FUNCTION   VALIDATE_RBRELEASE_R8

DESCRIPTION
  This function validates the received Rel 8 RB Release Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Release Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbr_validate_rbrelease_r8
(
tdsrrc_RadioBearerRelease *msg_ptr      /* Pointer to the RB Release message */
)
{
  tdsrrc_RadioBearerRelease_r8_IEs* rb_release_ptr = NULL;
  tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_VALID;

  rb_release_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
    u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerRelease_r8;
  
  /* Check the RRC State indicatior given by the message */
  switch (rb_release_ptr->rrc_StateIndicator )
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;
    
    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;
    
    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;
    
    default:
      /* No other states are supported right now. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: Unsupported state in RB Release Message");
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;
  }
  
  if ( tdsrrc_rbr.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG )
  {
    return;
  }

  /* Validate DL HSPDSCH */
  if ( TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,dl_HSPDSCH_Information))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSPDSCH Info present");
    /* Check if network does not try to initiate HSDPA with invalid state */
    if (tdsrrc_hsdpa_initial_validation(rb_release_ptr->rrc_StateIndicator, 
                                      TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
                                        rrc_DL_HSPDSCH_Information_r8_modeSpecificInfo_fdd) ) == FAILURE)
    {
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  if(rb_release_ptr->rb_InformationReleaseList.n == 0 )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: rb_InformationReleaseList is absent!");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
    tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
    return;
  }

  if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,rb_InformationReconfigList))
  {
    /* Save the linked list head pointer */
    if (FAILURE == tdsrrcllc_validate_rb_info_reconfig_list_r8(&rb_release_ptr->rb_InformationReconfigList))
    {
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,rab_InformationReconfigList))
    {
      if(tdsrrcrbr_update_rab_info_reconfiglist_r8(&rb_release_ptr->rab_InformationReconfigList) == FAILURE)
      {
        tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        return;
      }
    }
    else
    {
      /* Set the Ordered codec mode with the current active codec */
      tdsrrc_set_ordered_codec_mode(tdsrrcdata_codec_info.rrc_active_codec);
      tdsrrc_rbr.rabs.num_rabs_in_msg =0;
    }
  }
  

#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,
    dl_CounterSynchronisationInfo))
#else
    if (tdsrrcrbr_validate_srns_relocation_info(msg_ptr, TDSMSG_REL8))
#endif

  {
    /*check whether the SRNS relocation is within Cell_DCH or not.
    SRNS relocation information can come in reconfiguration messages
    only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
      ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"REL8: SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbr.state_indicated_by_msg);
      tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has 
    PDCP info list. If it is true then this configuration is unsupported */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rb_release_ptr->dl_CounterSynchronisationInfo,
		rb_WithPDCP_InfoList)) ||
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_release_ptr->dl_CounterSynchronisationInfo,
        tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: PDCP info in DL counter sync info not supported");
      tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation. Set the flag that indicates that this procedure
         involves SRNS relocation */
      tdsrrc_rbr.rbr_for_srns_relocation = TRUE;

      /* store if new U-RNTI is present in the message */
      if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,new_U_RNTI))
      {
        tdsrrc_rbr.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(rb_release_ptr->new_U_RNTI), &tdsrrc_rbr.new_urnti);
      }
    }
  } /* dl counter sync info */

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch */
  if((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,dl_CommonInformation)) 
      && (TDSRRC_MSG_COMMON_BITMASK_IE(rb_release_ptr->dl_CommonInformation,
      tdsrrc_DL_CommonInformation_r8,dl_dpchInfoCommon)) &&
      ( rb_release_ptr->dl_CommonInformation.dl_dpchInfoCommon.t ==
      T_tdsrrc_DL_CommonInformation_r8_dl_dpchInfoCommon_dl_DPCH_InfoCommon))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: To-DCH-Step-1 config activated");

        loc_ret_val = tdsrrcrb_get_mac_d_hfn(( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon,
			  tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
             &rb_release_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
             &mac_d_hfn_l);

        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: get_mac_d_hfn failed");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Process Ciphering Config for HHO failed");
        }
        tdsrrc_rbr.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: To-DCH-Step-1 not needed");
      }
    }
  }

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||(tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
    /* If UTRAN DRX coefficient is not present then it is invalid configuration */
    if (!( TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,
      utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8: UTRAN DRX Coef not preset,State:%d",tdsrrc_rbr.state_indicated_by_msg);
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    if (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      if (!( TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,ura_Identity)))  
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: URA ID not present");
        tdsrrccu_clear_ura_id ();
      }
      else
      {
        /*URA ID present in the message set ura_identity variable with this value */
        if (rb_release_ptr->ura_Identity.numbits == 16)
        {
          tdsrrccu_update_ura_id (&(rb_release_ptr->ura_Identity));
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rb_release_ptr->ura_Identity.numbits);
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
          tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
          return;
        }
      }
    }
  }

  /* Reject if any MBMS info is present */
  if (TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,mbms_PL_ServiceRestrictInfo))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MBMS info not supported in RBR");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
  }

  return;
}

/*===========================================================================
FUNCTION   tdsrrcrbr_process_after_oc_config_set_r8

DESCRIPTION
  This function processes the R8 RBR OTA after OC is set by this OTA. So if we call this function, it means
  that TDSORDERED_CONFIG_SET is returned while setting OC by RBR OTA.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbr_process_after_oc_config_set_r8
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr,
  tdsrrc_RadioBearerRelease_r8_IEs* rb_release_ptr
)
{
  tdsrrc_state_e_type                           current_state;       
  tdsrrcrb_directed_cell_info_type         directed_cell_info; 
  tdsrrc_RadioBearerRelease *              msg_ptr;  
  tdsrrc_int_c_rnti_type                        c_rnti;
  tdsrrc_int_u_rnti_type                        u_rnti;
  tdsrrcrb_rnti_update_info_type           rnti_update_info;   

  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerRelease);
  current_state = tdsrrc_get_state(); 

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
  if(tdsrrcsib_is_current_event_sib_read_in_dch())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: Force Init GPS ");
    tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
  }
#endif /*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /* Populate the local variables to indicate if Re-direction info is present */
  directed_cell_info.freq_info_present = FALSE;
  directed_cell_info.cpid_present = FALSE; 
  if((current_state==TDSRRC_STATE_CELL_FACH) && ((tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
    ||(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
  {
    if((TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,frequencyInfo))
      &&(TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->frequencyInfo.modeSpecificInfo,
      rrc_FrequencyInfo_modeSpecificInfo_tdd)))
    {
      directed_cell_info.freq_info_present = TRUE;
      directed_cell_info.uarfcn_dl  = rb_release_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
    }
   
    if(tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL8))
    {
      if((TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,
        dl_InformationPerRL_List)) && (rb_release_ptr->dl_InformationPerRL_List.n != 0))
      {
        directed_cell_info.cpid_present = TRUE;
        directed_cell_info.cell_param_id = rb_release_ptr->dl_InformationPerRL_List.elem[0].
                 modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
      }
    }      
    /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
            FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
    if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
      (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
      if(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
      {
        tdsrrcrbr_send_initiate_cu_flag = TRUE;
        tdsrrcrbr_delete_crnti          = TRUE;
      }
      else
      {
        tdsrrc_rbr.directed_cell_info = TRUE;
      }
    } 
  }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
  
  if((current_state==tdsrrc_rbr.state_indicated_by_msg) ||((current_state==TDSRRC_STATE_CELL_FACH) && 
    (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH)))
  {
    if((current_state==TDSRRC_STATE_CELL_FACH) && (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
    {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
       /*call the function that evaluates the directed cell info and set the flags accordingly */
      if ((tdsrrcrbr_send_initiate_cu_flag == FALSE) &&
          (tdsrrcrb_validate_directed_cell_info (directed_cell_info)== FALSE))
      {
        if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,new_C_RNTI))
        {
          tdsrrc_rbr.new_crnti_valid = TRUE;
          tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, &c_rnti);
          tdsrrc_rbr.new_crnti = (uint16)c_rnti;
        }
        else
        {
          tdsrrc_rbr.new_crnti_valid = FALSE;
        }
  
        if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
        {
          if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
          {      
            (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
          }
          else
          {
            (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
          }
          tdsrrc_rbr.need_to_release_cn_domain = FALSE;
        }
        tdsrrcllcoc_update_toc_with_oc_from_fach();
        /*Initiate the cell selection*/
        tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR, TDSRRC_TRANSITION_FROM_FACH,
                                                 tdsrrc_rbr.state_indicated_by_msg);
  
        tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
        return;
        }

  #else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/  
  
      if((TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,frequencyInfo))
        && (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->frequencyInfo.modeSpecificInfo,
        rrc_FrequencyInfo_modeSpecificInfo_tdd)))
      {
        directed_cell_info.freq_info_present = TRUE;
        directed_cell_info.uarfcn_dl  = rb_release_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
      }
      else
      {
        directed_cell_info.freq_info_present = FALSE;
      }
      if (tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL8))
      {
        if((TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,
          dl_InformationPerRL_List))
          && (rb_release_ptr->dl_InformationPerRL_List.n != 0))
        {
          directed_cell_info.cpid_present = TRUE;
          directed_cell_info.cell_param_id = rb_release_ptr->dl_InformationPerRL_List.elem[0].
            modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
        }
      }
      else
      {
        directed_cell_info.cpid_present = FALSE;
      }
      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
      {
        /*directed cell info matched. Cell update is not required */
        tdsrrcrbr_send_initiate_cu_flag = FALSE;
      }
      else
      {
        /*directed cell info do not match. Cell update is required */
        tdsrrcrbr_send_initiate_cu_flag = TRUE;
        tdsrrcrbr_delete_crnti          = TRUE;
      }
  #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
    } 
  
    /* Call the function that creates and sends a channel config request to LLC. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
    tdsrrcrbr_send_channel_config_req();
  
    /* Change the substate since we're waiting for the confirmation from LLC for the channel configs */
    tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
  }

  else if((current_state==TDSRRC_STATE_CELL_DCH) && (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
  {
      
    if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,new_C_RNTI))
    {
      tdsrrc_rbr.new_crnti_valid = TRUE;
      tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, &c_rnti);
      tdsrrc_rbr.new_crnti = (uint16)c_rnti;
    }
    else
    {
      tdsrrc_rbr.new_crnti_valid = FALSE;
    }
  
    /*Initiate the cell selection*/
    tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR, TDSRRC_TRANSITION_FROM_DCH,
                                            TDSRRC_STATE_CELL_FACH);
    tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
  
    /* Start T_305 [periodic cell update] timer */
    tdsrrccu_start_t_305_timer();
  }

  else if((current_state==TDSRRC_STATE_CELL_DCH) && (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
  {
    /*store the directed cell info */
     tdsrrc_rbr.directed_cell_info = tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL8);
  
     /*Send the tdsrrc_rbr complete msg on old configuration */
     tdsrrcrbr_send_rbr_complete_message(TRUE);
     /*wait for L2ack */
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: Waiting for L2 ACK for RBR Complete msg");
     tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
     tdsrrcllc_print_trans_chnl_rate();
  }

  else if((current_state==TDSRRC_STATE_CELL_DCH) && (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
  {
    /*Send the RBR complete msg on old configuration */
    tdsrrcrbr_send_rbr_complete_message(TRUE);
    /*wait for L2ack */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: Waiting for L2 ACK for RBR Complete msg");
    tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    tdsrrcllc_print_trans_chnl_rate();
  }

  else if((current_state==TDSRRC_STATE_CELL_FACH) && ((tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
    (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
  {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
    /*call the function that evaluates the directed cell info and set the flags accordingly */
    if((tdsrrc_rbr.directed_cell_info == FALSE) &&
      (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
    {
      if(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
      {
        tdsrrc_rbr.directed_cell_info = tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL8);
      }
      tdsrrc_rbr.initiate_cell_selection_trans_from_fach  = TRUE;
    }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
  
    if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,new_C_RNTI))
    {
      tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, &c_rnti);
      /* Send a RNTI Update Request */
      rnti_update_info.crnti_action    = TDSRRC_RNTI_UPDATE;
      rnti_update_info.crnti           = (uint16)c_rnti;
      /* check whether URNTI is also present in the message */
      if (TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,new_U_RNTI))
      {
        tdsrrc_translate_urnti(&rb_release_ptr->new_U_RNTI, &u_rnti);
        rnti_update_info.urnti_action    = TDSRRC_RNTI_UPDATE; 
        rnti_update_info.urnti           = u_rnti; 
      }
      else
      {
        rnti_update_info.urnti_action    = TDSRRC_RNTI_NOCHANGE; 
      }
      rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBR;
      rnti_update_info.rlc_re_establish_srb = FALSE;
      rnti_update_info.rlc_re_establish_rab = FALSE;
      rnti_update_info.cnf_reqd             = TRUE;
      tdsrrcrb_send_rnti_update_req(rnti_update_info);
      /* wait for the RNTI update confirm before sending the response message */
      tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
    }
    else 
    {
      tdsrrcrbr_send_rbr_complete_message(TRUE);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: Waiting for L2 ACK for RBR Complete msg");
      tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
      tdsrrcllc_print_trans_chnl_rate();
    }
  }
  return;  
}

/*===========================================================================
FUNCTION   RRCRBR_PROCESS_RBRELEASE_R8

DESCRIPTION
  This function processes the received Rel 8 RB Release Message. It first validates the message. 
  If the message has an invalid or unsupported configuration, a Radio Bearer Release Failure message 
  will be sent to the UTRAN. If the message is valid, it will be processed as follows -

    TDSORDERED_CONFIG will be set by calling LLC. 

    If TDSORDERED_CONFIG could be set without any conflicts, a TDSRRC_CHANNEL_CONFIG_REQ will be 
    sent to LLC to release the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Release Failure message will be sent to UTRAN.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbr_process_rbrelease_r8
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_RB_InformationReleaseList  *rb_info_rel_list_ptr;
  tdsrrc_RB_InformationAffectedList_r8 *rb_info_affected_list_ptr;
  tdsrrc_RB_InformationReconfig_r8 *rb_info_reconfig_ptr;
  tdsrrc_RadioBearerRelease *msg_ptr;  /* Pointer to the RB Release message        */

  tdsrrcllc_oc_status_e_type oc_status;/* To hold tdsordered_config status */
  /* Used to indicate if an error occured while setting tdsordered_config. */
  boolean ordered_config_err_occured = FALSE; 
  boolean rb_rel_srch = FALSE; 
  boolean rab_found = FALSE; 

  uint32 rab_count, rb_count, localrb_count,local_idx=0;
  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL8;
  tdsrrcsmc_srns_ip_check_status_e_type    ip_check_status = TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr ;     /* Pointer to the RB Release message */
  tdsrrc_RadioBearerRelease_r8_IEs* rb_release_ptr = NULL;
  
  tdsrrcrb_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Release message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerRelease);

  /* Store Msg Ptr in a local variable */
  rb_release_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
    u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerRelease_r8;

  if(TRUE == tdsrrcrbr_validate_srns_relocation_info(msg_ptr, msg_version))
  {
    ip_check_status = tdsrrcsmc_check_integrity_for_srns_relocation(cmd_ptr, msg_version);

    switch (ip_check_status)
    {
      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS:
        /*continue further processing with the message */
        break;

      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE:
        /*Discard the message and continue with ongoing procedure as if this
        message was not received */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: IP check failed. Discarding RBS msg");
        break;

      case TDSRRCSMC_SRNS_SECURITY_INFO_INVALID:
      case TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION:
        if (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier
                       == tdsrrc_rbr.tr_ids.rejected_transaction)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid security config-rejected tr-id%d match!",tdsrrc_rbr.tr_ids.rejected_transaction);
          /* Nothing else to do, just ignore the message and return */
        } 
        else
        {
          /* send a reconfig failure messge with invalid
          configuration and 
          continue the same substate with any ongoing procedure */
          temp_failure_cause.failure_status = tdsrrc_rbr.status.failure_status ;
          temp_rejected_transaction = tdsrrc_rbr.tr_ids.rejected_transaction;
          
          tdsrrc_rbr.tr_ids.rejected_transaction = 
            (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
          if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
          {
            tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          }
          else
          {
            tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
          }
          /*Check whether Cell Update is going on */
          if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
               (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
          {
            if(tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbr subsstate(%d) Send failure with %d cause",tdsrrc_rbr.curr_substate,tdsrrc_rbr.status.failure_status);
              tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                       tdsrrc_rbr.status.prot_err_cause );
              tdsrrc_rbr.status.failure_status  = temp_failure_cause.failure_status;
              tdsrrc_rbr.tr_ids.rejected_transaction = temp_rejected_transaction;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
              (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
              /* Wait until Cell update procedure is completed */
              tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
            }
          }
          else
          {
            tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                            tdsrrc_rbr.status.prot_err_cause );
          }
        }
        /*do not clear the procedure or change the tdsrrc_rbe substate because
        some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
        break;

      default:
        /*should not come here */
        break;
    }
    if (ip_check_status != TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS)
    {
      /*do not clear the procedure or change the tdsrrc_rbr substate because
      some other tdsrrc_rbe procedure might be in progress. Just return from
      here */
      return;
    }
  }

  if((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
    (tdsrrcccm_is_reselection_in_progress() == TRUE) ||
    (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incomatible simultaneous reconfig! Rejecting RBR message");
     if((msg_ptr->u.later_than_r3.rrc_TransactionIdentifier) 
            == tdsrrc_rbr.tr_ids.accepted_transaction )
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate RBR message - accepted tr-id matched!");
       /* Nothing else to do, just return */
     } 
     else if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                    == tdsrrc_rbr.tr_ids.rejected_transaction )
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate RBR message - rejected tr-id matched!");
       /* Nothing else to do, just return */
     } 
     else
     {
       /* We have received a RBR message with a new transaction id.
       This message should be in our rejected transaction
       list */
       temp_failure_cause.failure_status = tdsrrc_rbr.status.failure_status;
       temp_rejected_transaction = tdsrrc_rbr.tr_ids.rejected_transaction;
          
       tdsrrc_rbr.tr_ids.rejected_transaction = (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
       tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;

       /*Check whether Cell Update is going on */
       if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
            (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
       {
         if(tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
         {
           TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbr subsstate(%d) Send failure with %d cause",tdsrrc_rbr.curr_substate,tdsrrc_rbr.status.failure_status);
           tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                    tdsrrc_rbr.status.prot_err_cause );
           
           tdsrrc_rbr.status.failure_status  = temp_failure_cause.failure_status;
           tdsrrc_rbr.tr_ids.rejected_transaction = temp_rejected_transaction;
         }
         else
         {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
           (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
           /* Wait until Cell update procedure is completed */
           tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
         }
       }
       else
       {
         tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                         tdsrrc_rbr.status.prot_err_cause );
         /*continue the same substate */ 
       }
     }
     return;
  }

  /* We have received a RB release message with a new transaction id.
  Accept the transaction Id */
  tdsrrc_rbr.tr_ids.accepted_transaction = msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rb_release_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rb_release_ptr->m.activationTimePresent,
         rb_release_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBR SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbr_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rb_release_ptr->m.activationTimePresent = 0;
    }
  }

#endif

  /* Validate the Message to make sure there is no invalid or incompatible configuration 
  given by the message */
  tdsrrcrbr_validate_rbrelease_r8(msg_ptr);

  if(tdsrrc_rbr.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: RBR Message validation failed");

    /* Send the RB Release Failure Message */
    tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status, tdsrrc_rbr.status.prot_err_cause);

    tdsrrcrbr_clear_procedure();
    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
  }
  /* If the message is valid, proceed further */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: Processing RBR Message");

    if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,ueMobilityStateIndicator))
    {
      if(rb_release_ptr->ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in RBR.");
        tdsrrc_rbr.high_mobility_ind = TRUE;
      }
    }
    /*store the rb ids tobe released */
    tdsrrc_rbr.rbs_tobe_released.is_valid = TRUE;
    rb_info_rel_list_ptr = &rb_release_ptr->rb_InformationReleaseList;
    localrb_count = 0;
	local_idx=0;
    while (rb_info_rel_list_ptr->n > local_idx)
    {
      /*check whether rb id is signalling rbid or does not exist in established
      rabs */
      if( rb_info_rel_list_ptr->elem[local_idx] < DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
        /* RB Message was invalid. */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Invalid RB id:%d in RBR Message",rb_info_rel_list_ptr->elem[local_idx]);
        /* Send the RB Release Failure Message */
        tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status, tdsrrc_rbr.status.prot_err_cause);

        tdsrrcrbr_clear_procedure();
        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
        return;
      }
      else if( rb_info_rel_list_ptr->elem[local_idx] == DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
        if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND != tdsrrclcm_check_dl_rlc_lc_id(
                                         UE_LOGCHAN_DCCH,
                                         DCCH_DT_LOW_PRI_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Release SRB4");
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rel8:Ignore Non-exitent rb_id = %d in tdsrrc_rbr msg",rb_info_rel_list_ptr->elem[local_idx]);
          local_idx++;
          continue;
        }
      }
      else if (!tdsrrc_find_rb_in_est_rabs(rb_info_rel_list_ptr->elem[local_idx]))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Ignore Non-exitent rb_id =%d in tdsrrc_rbr msg",rb_info_rel_list_ptr->elem[local_idx]);
        local_idx++;
        continue;
      }

      tdsrrc_rbr.rbs_tobe_released.rb_id[localrb_count] = rb_info_rel_list_ptr->elem[local_idx];
      localrb_count++;
	  local_idx++;
    }

    tdsrrc_rbr.rbs_tobe_released.num_rbs = localrb_count;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Number of RBs to be released:%d",localrb_count);

    if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,rb_InformationReconfigList))
    {
      tdsrrc_rbr.rbs_tobe_reconfigured.is_valid = TRUE;
      
      localrb_count = 0;
      while (rb_release_ptr->rb_InformationReconfigList.n > localrb_count )
      {
        rb_info_reconfig_ptr = &rb_release_ptr->rb_InformationReconfigList.elem[localrb_count];
        /* Fill up the RB id and indicate that it needs to be reconfigured */
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_id= 
          rb_info_reconfig_ptr->rb_Identity;
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_config = TDSRECONFIG_RB;

        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask = tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask = 0;
        if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
			tdsrrc_RB_InformationReconfig_r8,rlc_Info))&&
          (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,
          tdsrrc_RLC_Info_r7,ul_RLC_Mode)))
        {
          tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask = TDSMODIFY_RB_CFG;
        }
        if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
			tdsrrc_RB_InformationReconfig_r8,rlc_Info))&&
          (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,
          tdsrrc_RLC_Info_r7,dl_RLC_Mode)))
        {
          tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
        }
 
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
			tdsrrc_RB_InformationReconfig_r8,rb_StopContinue))
        {
          /* over-ride rb_config with STOP or continue */
          if (tdsrrc_RB_StopContinue_stopRB == rb_info_reconfig_ptr->rb_StopContinue)
          {
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask |= TDSSTOP_RB_CFG;
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask |= TDSSTOP_RB_CFG;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL8: RB-Id %d to STOP rb_count %d",tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_id,localrb_count);
          }
          else if (tdsrrc_RB_StopContinue_continueRB == rb_info_reconfig_ptr->rb_StopContinue)
          {
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask |= TDSCONTINUE_RB_CFG;
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask |= TDSCONTINUE_RB_CFG;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL8: RB-Id %d to CONTINUE rb_count %d",tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_id,localrb_count);
          }
        }

        localrb_count++;
        /* Go to the next reconfigured RB */
      
      }

      tdsrrc_rbr.rbs_tobe_reconfigured.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Number of RBs to be reconfigured:%d",localrb_count);
    }

    /*store the rb ids tobe affected */
    if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,rb_InformationAffectedList))
    {
      tdsrrc_rbr.rbs_tobe_affected.is_valid = TRUE;
      rb_info_affected_list_ptr = 
      &rb_release_ptr->rb_InformationAffectedList;
      localrb_count = 0;
      while ( rb_info_affected_list_ptr->n > localrb_count)
      {
        /*check whether rb id is signalling rbid or does not exist in established
        rabs */
        if((rb_info_affected_list_ptr->elem[localrb_count].rb_Identity > DCCH_DT_LOW_PRI_RADIO_BEARER_ID) &&
          (!tdsrrc_find_rb_in_est_rabs((rb_info_affected_list_ptr->elem[localrb_count].rb_Identity))))
        {
          /* RB Message was invalid. */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Invalid RB id:%d in RBR Message",rb_info_affected_list_ptr->elem[localrb_count].rb_Identity);
          /* Send the RB Release Failure Message */
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status, tdsrrc_rbr.status.prot_err_cause);

          tdsrrcrbr_clear_procedure();
          /* Make sure we're back in the initial sub-state */
          tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
          return;
        }
        tdsrrc_rbr.rbs_tobe_affected.rb_id[localrb_count] = 
        rb_info_affected_list_ptr->elem[localrb_count].rb_Identity;
        localrb_count++;
      }
      tdsrrc_rbr.rbs_tobe_affected.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Number of RBs tobe affected:%d",localrb_count);
    }

    /* If there is a signalling connection to be released,
    or if there are individual RBs to be released, we
    will be releasing some RBs. */
    if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,
      signallingConnectionRelIndication))
    {
      /* Need to release a CN domain */
      tdsrrc_rbr.need_to_release_cn_domain = TRUE; 
      /* Copy the domain that is to be released */
      tdsrrc_rbr.cn_to_release = rb_release_ptr->signallingConnectionRelIndication;

      for(rab_count = 0; rab_count< MAX_RAB_TO_SETUP; rab_count++)
      {
        if((tdsrrc_est_rabs.rabs[rab_count].cn_domain == tdsrrc_rbr.cn_to_release) &&
          (tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0))
        {
          rab_found = TRUE;
          for ( rb_count=0; rb_count<tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab; rb_count++ )
          {
            rb_rel_srch = FALSE;
            for(localrb_count=0; localrb_count<tdsrrc_rbr.rbs_tobe_released.num_rbs; localrb_count++)
            {
              if(tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id == 
                  tdsrrc_rbr.rbs_tobe_released.rb_id[localrb_count])
              {
                rb_rel_srch = TRUE;
                break;
              }
            }
            if(rb_rel_srch == FALSE) // est rabs has an rb_id not in the rb release list
            {
              /* send failure message */
              tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
              tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status, tdsrrc_rbr.status.prot_err_cause);
              /* Go back to initial substate */
              tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
              /* Then clear all local variables that may need to be cleared */
              tdsrrcrbr_clear_procedure();
              return;
            }
          }/* rb_count loop */       
        } /*end of if loop */
      }/* rab_count loop */

      /*if none of the rabs in the establishedrabs have rb information*/
      if(rab_found==FALSE )
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: RB lookup in EST_RABS for cn:%d failed",tdsrrc_rbr.cn_to_release );
        /* send failure message */
        tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status, tdsrrc_rbr.status.prot_err_cause);
        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbr_clear_procedure();
        return;
      }
    }

    /* Check if there is CN information present. If so, we need to send the information to the Cell Change manager */
    if (TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: Saving CN info for NAS");
      tdsrrc_rbr.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(
        &(rb_release_ptr->cn_InformationInfo) );
      if (tdsrrc_rbr.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd!");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbr.cn_info_saved = TRUE;
      }
    }

    /* Indicate layer1 if rb release message is going to release voice RAB */
    tdsrrcrbr_indicate_layer1_if_config_msg_release_voice_rab();

    tdsrrcrb_update_mac_rab_status(TDSRRC_PROCEDURE_RBR, RRC_CS_DOMAIN_CN_ID,
                                &rb_release_ptr->rb_InformationReleaseList, FALSE);

    /* Set TDSORDERED_CONFIG so that LLC can process the necessary parameters. */
    oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBR, tdsrrc_rbr.state_indicated_by_msg,
                      tdsrrc_DL_DCCH_Message_PDU, (void *)dcch_msg_ptr);

    switch ( oc_status )
    {
      case TDSORDERED_CONFIG_SET:
        tdsrrcrbr_process_after_oc_config_set_r8(dcch_msg_ptr, rb_release_ptr);
        break;

      case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
        tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
        tdsrrc_rbr.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_rbr.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
      
      case TDSORDERED_CONFIG_NOT_SET_OTHER:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
        /* When LLC sends this, there is a serious problem
        and we'll go back to disconnected state. No need
        to send the failure message here, we can just
        go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      default:

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
        /* Unknown error, just go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
    }/* end switch */

    if ( ordered_config_err_occured == TRUE )
    {
        /* No need to continue. Clear the procedure
        variables and return. */
        tdsrrcrbr_clear_procedure();
        return;
    }

    if(tdsrrc_rbr.rbr_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }
  }/* end if - valid rb message */
  return;
}

#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
/*===========================================================================
FUNCTION   VALIDATE_RBRELEASE_R9

DESCRIPTION
  This function validates the received Rel 8 RB Release Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Release Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbr_validate_rbrelease_r9
(
tdsrrc_RadioBearerRelease *msg_ptr      /* Pointer to the RB Release message */
)
{
  tdsrrc_RadioBearerRelease_r9_IEs* rb_release_ptr = NULL;
  tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_VALID;

  rb_release_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
    u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.radioBearerRelease_r9;
  
  /* Check the RRC State indicatior given by the message */
  switch (rb_release_ptr->rrc_StateIndicator )
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;
    
    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;
    
    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;
    
    default:
      /* No other states are supported right now. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: Unsupported state in RB Release Message");
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;
  }
  
  if ( tdsrrc_rbr.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG )
  {
    return;
  }
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
      u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9,
      radioBearerRelease_r9_add_ext))
     
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: radioBearerRelease_r9_add_extPresent not yet supported");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    return;
  } 

  /* Validate DL HSPDSCH */
  if ( TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,dl_HSPDSCH_Information))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSPDSCH Info present");
    /* Check if network does not try to initiate HSDPA with invalid state */
    if (tdsrrc_hsdpa_initial_validation(rb_release_ptr->rrc_StateIndicator, 
                                      TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
                                        rrc_DL_HSPDSCH_Information_r9_modeSpecificInfo_fdd) ) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"hsdpa initial validate failed");
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  if(rb_release_ptr->rb_InformationReleaseList.n == 0 )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: rb_InformationReleaseList is absent!");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
    tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
    return;
  }

  if(TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,rb_InformationReconfigList))
  {
    /* Save the linked list head pointer */
    if (FAILURE == tdsrrcllc_validate_rb_info_reconfig_list_r8(&rb_release_ptr->rb_InformationReconfigList))
    {
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    if(TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,rab_InformationReconfigList))
    {
      if(tdsrrcrbr_update_rab_info_reconfiglist_r8(&rb_release_ptr->rab_InformationReconfigList) == FAILURE)
      {
        tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        return;
      }
    }
    else
    {
      /* Set the Ordered codec mode with the current active codec */
      tdsrrc_set_ordered_codec_mode(tdsrrcdata_codec_info.rrc_active_codec);
      tdsrrc_rbr.rabs.num_rabs_in_msg =0;
    }
  }
  
  
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if(TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,
    dl_CounterSynchronisationInfo))
#else
    if (tdsrrcrbr_validate_srns_relocation_info(msg_ptr, TDSMSG_REL9))
#endif

  {
    /*check whether the SRNS relocation is within Cell_DCH or not.
    SRNS relocation information can come in reconfiguration messages
    only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
      ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"REL9: SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbr.state_indicated_by_msg);
      tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has 
    PDCP info list. If it is true then this configuration is unsupported */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rb_release_ptr->dl_CounterSynchronisationInfo,
		rb_WithPDCP_InfoList)) ||
        (TDSRRC_MSG_COMMON_BITMASK_IE(rb_release_ptr->dl_CounterSynchronisationInfo,
        tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: PDCP info in DL counter sync info not supported");
      tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation. Set the flag that indicates that this procedure
         involves SRNS relocation */
      tdsrrc_rbr.rbr_for_srns_relocation = TRUE;

      /* store if new U-RNTI is present in the message */
      if(TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,new_U_RNTI))
      {
        tdsrrc_rbr.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(rb_release_ptr->new_U_RNTI), &tdsrrc_rbr.new_urnti);
      }
    }
  } /* dl counter sync info */

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch */
  if((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,dl_CommonInformation)) 
      && (TDSRRC_MSG_COMMON_BITMASK_IE(rb_release_ptr->dl_CommonInformation,
      tdsrrc_DL_CommonInformation_r8,dl_dpchInfoCommon)) &&
      ( rb_release_ptr->dl_CommonInformation.dl_dpchInfoCommon.t ==
      T_tdsrrc_DL_CommonInformation_r8_dl_dpchInfoCommon_dl_DPCH_InfoCommon))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: To-DCH-Step-1 config activated");

        loc_ret_val = tdsrrcrb_get_mac_d_hfn(( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon,
			  tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
             &rb_release_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
             &mac_d_hfn_l);

        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: get_mac_d_hfn failed");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:Process Ciphering Config for HHO failed");
        }
        tdsrrc_rbr.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: To-DCH-Step-1 not needed");
      }
    }
  }

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||(tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
    /* If UTRAN DRX coefficient is not present then it is invalid configuration */
    if (!( TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,
      utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL9: UTRAN DRX Coef not preset,State:%d",tdsrrc_rbr.state_indicated_by_msg);
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    if (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      if (!( TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,ura_Identity)))  
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: URA ID not present");
        tdsrrccu_clear_ura_id ();
      }
      else
      {
        /*URA ID present in the message set ura_identity variable with this value */
        if (rb_release_ptr->ura_Identity.numbits == 16)
        {
          tdsrrccu_update_ura_id (&(rb_release_ptr->ura_Identity));
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rb_release_ptr->ura_Identity.numbits);
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
          tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
          return;
        }
      }
    }
  }

  
  /* Reject if any MBMS info is present */
  if (TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,mbms_PL_ServiceRestrictInfo))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MBMS info not supported in RBR");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
  }
  
  if (TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,mbms_RB_ListReleasedToChangeTransferMode))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MBMS info not supported in RBR");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
  }

  return;
}
/*===========================================================================
FUNCTION   tdsrrcrbr_process_after_oc_config_set_r9

DESCRIPTION
  This function processes the R8 RBR OTA after OC is set by this OTA. So if we call this function, it means
  that TDSORDERED_CONFIG_SET is returned while setting OC by RBR OTA.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbr_process_after_oc_config_set_r9
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr,
  tdsrrc_RadioBearerRelease_r9_IEs* rb_release_ptr
)
{
  tdsrrc_state_e_type                           current_state;       
  tdsrrcrb_directed_cell_info_type         directed_cell_info; 
  tdsrrc_RadioBearerRelease *              msg_ptr;  
  tdsrrc_int_c_rnti_type                        c_rnti;
  tdsrrc_int_u_rnti_type                        u_rnti;
  tdsrrcrb_rnti_update_info_type           rnti_update_info;   

  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerRelease);
  current_state = tdsrrc_get_state(); 

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
  if(tdsrrcsib_is_current_event_sib_read_in_dch())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: Force Init GPS ");
    tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
  }
#endif /*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /* Populate the local variables to indicate if Re-direction info is present */
  directed_cell_info.freq_info_present = FALSE;
  directed_cell_info.cpid_present = FALSE; 
  if((current_state==TDSRRC_STATE_CELL_FACH) && ((tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
    ||(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
  {
    if((TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,frequencyInfo))
      &&(TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->frequencyInfo.modeSpecificInfo,
      rrc_FrequencyInfo_modeSpecificInfo_tdd)))
    {
      directed_cell_info.freq_info_present = TRUE;
      directed_cell_info.uarfcn_dl  = rb_release_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
    }
   
    if(tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL9))
    {
      if((TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,
        dl_InformationPerRL_List)) && (rb_release_ptr->dl_InformationPerRL_List.n != 0))
      {
        directed_cell_info.cpid_present = TRUE;
        directed_cell_info.cell_param_id = rb_release_ptr->dl_InformationPerRL_List.elem[0].
                 modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
      }
    }      
    /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
            FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
    if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
      (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
      if(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
      {
        tdsrrcrbr_send_initiate_cu_flag = TRUE;
        tdsrrcrbr_delete_crnti          = TRUE;
      }
      else
      {
        tdsrrc_rbr.directed_cell_info = TRUE;
      }
    } 
  }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
  
  if((current_state==tdsrrc_rbr.state_indicated_by_msg) ||((current_state==TDSRRC_STATE_CELL_FACH) && 
    (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH)))
  {
    if((current_state==TDSRRC_STATE_CELL_FACH) && (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
    {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
       /*call the function that evaluates the directed cell info and set the flags accordingly */
      if ((tdsrrcrbr_send_initiate_cu_flag == FALSE) &&
          (tdsrrcrb_validate_directed_cell_info (directed_cell_info)== FALSE))
      {
        if(TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,new_C_RNTI))
        {
          tdsrrc_rbr.new_crnti_valid = TRUE;
          tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, &c_rnti);
          tdsrrc_rbr.new_crnti = (uint16)c_rnti;
        }
        else
        {
          tdsrrc_rbr.new_crnti_valid = FALSE;
        }
  
        if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
        {
          if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
          {      
            (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
          }
          else
          {
            (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
          }
          tdsrrc_rbr.need_to_release_cn_domain = FALSE;
        }
        tdsrrcllcoc_update_toc_with_oc_from_fach();
        /*Initiate the cell selection*/
        tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR, TDSRRC_TRANSITION_FROM_FACH,
                                                 tdsrrc_rbr.state_indicated_by_msg);
  
        tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
        return;
        }

  #else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/  
  
      if((TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,frequencyInfo))
        && (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->frequencyInfo.modeSpecificInfo,
        rrc_FrequencyInfo_modeSpecificInfo_tdd)))
      {
        directed_cell_info.freq_info_present = TRUE;
        directed_cell_info.uarfcn_dl  = rb_release_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
      }
      else
      {
        directed_cell_info.freq_info_present = FALSE;
      }
      if (tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL9))
      {
        if((TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,
          dl_InformationPerRL_List))
          && (rb_release_ptr->dl_InformationPerRL_List.n != 0))
        {
          directed_cell_info.cpid_present = TRUE;
          directed_cell_info.cell_param_id = rb_release_ptr->dl_InformationPerRL_List.elem[0].
            modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
        }
      }
      else
      {
        directed_cell_info.cpid_present = FALSE;
      }
      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
      {
        /*directed cell info matched. Cell update is not required */
        tdsrrcrbr_send_initiate_cu_flag = FALSE;
      }
      else
      {
        /*directed cell info do not match. Cell update is required */
        tdsrrcrbr_send_initiate_cu_flag = TRUE;
        tdsrrcrbr_delete_crnti          = TRUE;
      }
  #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
    } 
  
    /* Call the function that creates and sends a channel config request to LLC. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
    tdsrrcrbr_send_channel_config_req();
  
    /* Change the substate since we're waiting for the confirmation from LLC for the channel configs */
    tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
  }

  else if((current_state==TDSRRC_STATE_CELL_DCH) && (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
  {
      
    if(TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,new_C_RNTI))
    {
      tdsrrc_rbr.new_crnti_valid = TRUE;
      tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, &c_rnti);
      tdsrrc_rbr.new_crnti = (uint16)c_rnti;
    }
    else
    {
      tdsrrc_rbr.new_crnti_valid = FALSE;
    }
  
    if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
    {
      if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
      {      
        (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
      }
      else
      {
        (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
      }
      tdsrrc_rbr.need_to_release_cn_domain = FALSE;
    }
  
    /*Initiate the cell selection*/
    tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR, TDSRRC_TRANSITION_FROM_DCH,
                                            TDSRRC_STATE_CELL_FACH);
    tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
  
    /* Start T_305 [periodic cell update] timer */
    tdsrrccu_start_t_305_timer();
  }

  else if((current_state==TDSRRC_STATE_CELL_DCH) && (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
  {
    /*store the directed cell info */
     tdsrrc_rbr.directed_cell_info = tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL9);
  
     /*Send the tdsrrc_rbr complete msg on old configuration */
     tdsrrcrbr_send_rbr_complete_message(TRUE);
     /*wait for L2ack */
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: Waiting for L2 ACK for RBR Complete msg");
     tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
     tdsrrcllc_print_trans_chnl_rate();
  }

  else if((current_state==TDSRRC_STATE_CELL_DCH) && (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
  {
    /*Send the RBR complete msg on old configuration */
    tdsrrcrbr_send_rbr_complete_message(TRUE);
    /*wait for L2ack */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: Waiting for L2 ACK for RBR Complete msg");
    tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    tdsrrcllc_print_trans_chnl_rate();
  }

  else if((current_state==TDSRRC_STATE_CELL_FACH) && ((tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
    (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
  {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
    /*call the function that evaluates the directed cell info and set the flags accordingly */
    if((tdsrrc_rbr.directed_cell_info == FALSE) &&
      (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
    {
      if(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
      {
        tdsrrc_rbr.directed_cell_info = tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL9);
      }
      tdsrrc_rbr.initiate_cell_selection_trans_from_fach  = TRUE;
    }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
  
    if(TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,new_C_RNTI))
    {
      tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, &c_rnti);
      /* Send a RNTI Update Request */
      rnti_update_info.crnti_action    = TDSRRC_RNTI_UPDATE;
      rnti_update_info.crnti           = (uint16)c_rnti;
      /* check whether URNTI is also present in the message */
      if (TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,new_U_RNTI))
      {
        tdsrrc_translate_urnti(&rb_release_ptr->new_U_RNTI, &u_rnti);
        rnti_update_info.urnti_action    = TDSRRC_RNTI_UPDATE; 
        rnti_update_info.urnti           = u_rnti; 
      }
      else
      {
        rnti_update_info.urnti_action    = TDSRRC_RNTI_NOCHANGE; 
      }
      rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBR;
      rnti_update_info.rlc_re_establish_srb = FALSE;
      rnti_update_info.rlc_re_establish_rab = FALSE;
      rnti_update_info.cnf_reqd             = TRUE;
      tdsrrcrb_send_rnti_update_req(rnti_update_info);
      /* wait for the RNTI update confirm before sending the response message */
      tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
    }
    else 
    {
      tdsrrcrbr_send_rbr_complete_message(TRUE);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: Waiting for L2 ACK for RBR Complete msg");
      tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
      tdsrrcllc_print_trans_chnl_rate();
    }
  }
  return;  
}

/*===========================================================================
FUNCTION   RRCRBR_PROCESS_RBRELEASE_R9

DESCRIPTION
  This function processes the received Rel 8 RB Release Message. It first validates the message. 
  If the message has an invalid or unsupported configuration, a Radio Bearer Release Failure message 
  will be sent to the UTRAN. If the message is valid, it will be processed as follows -

    TDSORDERED_CONFIG will be set by calling LLC. 

    If TDSORDERED_CONFIG could be set without any conflicts, a TDSRRC_CHANNEL_CONFIG_REQ will be 
    sent to LLC to release the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Release Failure message will be sent to UTRAN.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbr_process_rbrelease_r9
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_RB_InformationReleaseList  *rb_info_rel_list_ptr;
  tdsrrc_RB_InformationAffectedList_r8 *rb_info_affected_list_ptr;
  tdsrrc_RB_InformationReconfig_r8 *rb_info_reconfig_ptr;
  tdsrrc_RadioBearerRelease *msg_ptr;  /* Pointer to the RB Release message        */

  tdsrrcllc_oc_status_e_type oc_status;/* To hold tdsordered_config status */
  /* Used to indicate if an error occured while setting tdsordered_config. */
  boolean ordered_config_err_occured = FALSE; 
  boolean rb_rel_srch = FALSE; 
  boolean rab_found = FALSE; 

  uint32 rab_count, rb_count, localrb_count,local_idx=0;
  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL9;
  tdsrrcsmc_srns_ip_check_status_e_type    ip_check_status = TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr ;     /* Pointer to the RB Release message */
  tdsrrc_RadioBearerRelease_r9_IEs* rb_release_ptr = NULL;

  tdsrrcrb_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Release message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerRelease);

  /* Store Msg Ptr in a local variable */
  rb_release_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
    u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.radioBearerRelease_r9;

  if(TRUE == tdsrrcrbr_validate_srns_relocation_info(msg_ptr, msg_version))
  {
    ip_check_status = tdsrrcsmc_check_integrity_for_srns_relocation(cmd_ptr, msg_version);

    switch (ip_check_status)
    {
      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS:
        /*continue further processing with the message */
        break;

      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE:
        /*Discard the message and continue with ongoing procedure as if this
        message was not received */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: IP check failed. Discarding RBS msg");
        break;

      case TDSRRCSMC_SRNS_SECURITY_INFO_INVALID:
      case TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION:
        if (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier
                       == tdsrrc_rbr.tr_ids.rejected_transaction)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid security config-rejected tr-id%d match!",tdsrrc_rbr.tr_ids.rejected_transaction);
          /* Nothing else to do, just ignore the message and return */
        } 
        else
        {
          /* send a reconfig failure messge with invalid
          configuration and 
          continue the same substate with any ongoing procedure */

          temp_failure_cause.failure_status = tdsrrc_rbr.status.failure_status;
          temp_rejected_transaction = tdsrrc_rbr.tr_ids.rejected_transaction;
          
          tdsrrc_rbr.tr_ids.rejected_transaction = 
            (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
          if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
          {
            tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          }
          else
          {
            tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
          }
          /*Check whether Cell Update is going on */
          if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
               (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
          {
            if(tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbr subsstate(%d) Send failure with %d cause",tdsrrc_rbr.curr_substate,tdsrrc_rbr.status.failure_status);
              tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                       tdsrrc_rbr.status.prot_err_cause );
              tdsrrc_rbr.status.failure_status  = temp_failure_cause.failure_status;
              tdsrrc_rbr.tr_ids.rejected_transaction = temp_rejected_transaction;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
              (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
              /* Wait until Cell update procedure is completed */
              tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
            }
          }
          else
          {
            tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                            tdsrrc_rbr.status.prot_err_cause );
          }
        }
        /*do not clear the procedure or change the tdsrrc_rbe substate because
        some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
        break;

      default:
        /*should not come here */
        break;
    }
    if (ip_check_status != TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS)
    {
      /*do not clear the procedure or change the tdsrrc_rbr substate because
      some other tdsrrc_rbe procedure might be in progress. Just return from
      here */
      return;
    }
  }

  if((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
    (tdsrrcccm_is_reselection_in_progress() == TRUE) ||
    (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incomatible simultaneous reconfig! Rejecting RBR message");
     if((msg_ptr->u.later_than_r3.rrc_TransactionIdentifier) 
            == tdsrrc_rbr.tr_ids.accepted_transaction )
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate RBR message - accepted tr-id matched!");
       /* Nothing else to do, just return */
     } 
     else if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                    == tdsrrc_rbr.tr_ids.rejected_transaction )
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate RBR message - rejected tr-id matched!");
       /* Nothing else to do, just return */
     } 
     else
     {
       /* We have received a RBR message with a new transaction id.
       This message should be in our rejected transaction
       list */
       temp_failure_cause.failure_status = tdsrrc_rbr.status.failure_status;
       temp_rejected_transaction = tdsrrc_rbr.tr_ids.rejected_transaction;
       
       tdsrrc_rbr.tr_ids.rejected_transaction = (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
       tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;

       /*Check whether Cell Update is going on */
       if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
            (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
       {
         if(tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
         {
           TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbr subsstate(%d) Send failure with %d cause",tdsrrc_rbr.curr_substate,tdsrrc_rbr.status.failure_status);
           tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                    tdsrrc_rbr.status.prot_err_cause );
           
           tdsrrc_rbr.status.failure_status  = temp_failure_cause.failure_status;
           tdsrrc_rbr.tr_ids.rejected_transaction = temp_rejected_transaction;
         }
         else
         {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
           (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
           /* Wait until Cell update procedure is completed */
           tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
         }
       }
       else
       {
         tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                         tdsrrc_rbr.status.prot_err_cause );
         /*continue the same substate */ 
       }
     }
     return;
  }

  /* We have received a RB release message with a new transaction id.
  Accept the transaction Id */
  tdsrrc_rbr.tr_ids.accepted_transaction = msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rb_release_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rb_release_ptr->m.activationTimePresent,
         rb_release_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBR SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbr_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rb_release_ptr->m.activationTimePresent = 0;
    }
  }

#endif

  /* Validate the Message to make sure there is no invalid or incompatible configuration 
  given by the message */
  tdsrrcrbr_validate_rbrelease_r9(msg_ptr);

  if(tdsrrc_rbr.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: RBR Message validation failed");

    /* Send the RB Release Failure Message */
    tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status, tdsrrc_rbr.status.prot_err_cause);

    tdsrrcrbr_clear_procedure();
    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
  }
  /* If the message is valid, proceed further */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: Processing RBR Message");

    if(TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,ueMobilityStateIndicator))
    {
      if(rb_release_ptr->ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in RBR.");
        tdsrrc_rbr.high_mobility_ind = TRUE;
      }
    }
    /*store the rb ids tobe released */
    tdsrrc_rbr.rbs_tobe_released.is_valid = TRUE;
    rb_info_rel_list_ptr = &rb_release_ptr->rb_InformationReleaseList;
    localrb_count = 0;
	local_idx=0;
    while (rb_info_rel_list_ptr->n > local_idx)
    {
      /*check whether rb id is signalling rbid or does not exist in established
      rabs */
      if( rb_info_rel_list_ptr->elem[local_idx] < DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
        /* RB Message was invalid. */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9: Invalid RB id:%d in RBR Message",rb_info_rel_list_ptr->elem[local_idx]);
        /* Send the RB Release Failure Message */
        tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status, tdsrrc_rbr.status.prot_err_cause);

        tdsrrcrbr_clear_procedure();
        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
        return;
      }
      else if( rb_info_rel_list_ptr->elem[local_idx] == DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
        if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND != tdsrrclcm_check_dl_rlc_lc_id(
                                         UE_LOGCHAN_DCCH,
                                         DCCH_DT_LOW_PRI_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Release SRB4");
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rel9:Ignore Non-exitent rb_id = %d in tdsrrc_rbr msg",rb_info_rel_list_ptr->elem[local_idx]);
          local_idx++;
          continue;
        }
      }
      else if (!tdsrrc_find_rb_in_est_rabs(rb_info_rel_list_ptr->elem[local_idx]))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9: Ignore Non-exitent rb_id =%d in tdsrrc_rbr msg",rb_info_rel_list_ptr->elem[local_idx]);
        local_idx++;
        continue;
      }

      tdsrrc_rbr.rbs_tobe_released.rb_id[localrb_count] = rb_info_rel_list_ptr->elem[local_idx];
      localrb_count++;
	  local_idx++;
    }

    tdsrrc_rbr.rbs_tobe_released.num_rbs = localrb_count;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9: Number of RBs to be released:%d",localrb_count);

    if(TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,rb_InformationReconfigList))
    {
      tdsrrc_rbr.rbs_tobe_reconfigured.is_valid = TRUE;
      
      localrb_count = 0;
      while (rb_release_ptr->rb_InformationReconfigList.n > localrb_count )
      {
        rb_info_reconfig_ptr = &rb_release_ptr->rb_InformationReconfigList.elem[localrb_count];
        /* Fill up the RB id and indicate that it needs to be reconfigured */
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_id= 
          rb_info_reconfig_ptr->rb_Identity;
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_config = TDSRECONFIG_RB;

        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask = tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask = 0;
        if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
			tdsrrc_RB_InformationReconfig_r8,rlc_Info))&&
          (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,
          tdsrrc_RLC_Info_r7,ul_RLC_Mode)))
        {
          tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask = TDSMODIFY_RB_CFG;
        }
        if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
			tdsrrc_RB_InformationReconfig_r8,rlc_Info))&&
          (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->rlc_Info,
          tdsrrc_RLC_Info_r7,dl_RLC_Mode)))
        {
          tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
        }
 
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
			tdsrrc_RB_InformationReconfig_r8,rb_StopContinue))
        {
          /* over-ride rb_config with STOP or continue */
          if (tdsrrc_RB_StopContinue_stopRB == rb_info_reconfig_ptr->rb_StopContinue)
          {
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask |= TDSSTOP_RB_CFG;
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask |= TDSSTOP_RB_CFG;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL9: RB-Id %d to STOP rb_count %d",tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_id,localrb_count);
          }
          else if (tdsrrc_RB_StopContinue_continueRB == rb_info_reconfig_ptr->rb_StopContinue)
          {
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].ul_cfg_mask |= TDSCONTINUE_RB_CFG;
            tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].dl_cfg_mask |= TDSCONTINUE_RB_CFG;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL9: RB-Id %d to CONTINUE rb_count %d",tdsrrc_rbr.rbs_tobe_reconfigured.rb_info[localrb_count].rb_id,localrb_count);
          }
        }

        localrb_count++;
        /* Go to the next reconfigured RB */
      
      }

      tdsrrc_rbr.rbs_tobe_reconfigured.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9: Number of RBs to be reconfigured:%d",localrb_count);
    }

    /*store the rb ids tobe affected */
    if(TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,rb_InformationAffectedList))
    {
      tdsrrc_rbr.rbs_tobe_affected.is_valid = TRUE;
      rb_info_affected_list_ptr = 
      &rb_release_ptr->rb_InformationAffectedList;
      localrb_count = 0;
      while ( rb_info_affected_list_ptr->n > localrb_count)
      {
        /*check whether rb id is signalling rbid or does not exist in established
        rabs */
        if((rb_info_affected_list_ptr->elem[localrb_count].rb_Identity > DCCH_DT_LOW_PRI_RADIO_BEARER_ID) &&
          (!tdsrrc_find_rb_in_est_rabs((rb_info_affected_list_ptr->elem[localrb_count].rb_Identity))))
        {
          /* RB Message was invalid. */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9: Invalid RB id:%d in RBR Message",rb_info_affected_list_ptr->elem[localrb_count].rb_Identity);
          /* Send the RB Release Failure Message */
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status, tdsrrc_rbr.status.prot_err_cause);

          tdsrrcrbr_clear_procedure();
          /* Make sure we're back in the initial sub-state */
          tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
          return;
        }
        tdsrrc_rbr.rbs_tobe_affected.rb_id[localrb_count] = 
        rb_info_affected_list_ptr->elem[localrb_count].rb_Identity;
        localrb_count++;
      }
      tdsrrc_rbr.rbs_tobe_affected.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9: Number of RBs tobe affected:%d",localrb_count);
    }

    /* If there is a signalling connection to be released,
    or if there are individual RBs to be released, we
    will be releasing some RBs. */
    if(TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,
      signallingConnectionRelIndication))
    {
      /* Need to release a CN domain */
      tdsrrcrbr_mark_cn_doamin_to_be_closed(rb_release_ptr->signallingConnectionRelIndication);

      for(rab_count = 0; rab_count< MAX_RAB_TO_SETUP; rab_count++)
      {
        if((tdsrrc_est_rabs.rabs[rab_count].cn_domain == tdsrrc_rbr.cn_to_release) &&
          (tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0))
        {
          rab_found = TRUE;
          for ( rb_count=0; rb_count<tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab; rb_count++ )
          {
            rb_rel_srch = FALSE;
            for(localrb_count=0; localrb_count<tdsrrc_rbr.rbs_tobe_released.num_rbs; localrb_count++)
            {
              if(tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id == 
                  tdsrrc_rbr.rbs_tobe_released.rb_id[localrb_count])
              {
                rb_rel_srch = TRUE;
                break;
              }
            }
            if(rb_rel_srch == FALSE) // est rabs has an rb_id not in the rb release list
            {
              /* send failure message */
              tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
              tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status, tdsrrc_rbr.status.prot_err_cause);
              /* Go back to initial substate */
              tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
              /* Then clear all local variables that may need to be cleared */
              tdsrrcrbr_clear_procedure();
              return;
            }
          }/* rb_count loop */       
        } /*end of if loop */
      }/* rab_count loop */

      /*if none of the rabs in the establishedrabs have rb information*/
      if(rab_found==FALSE )
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9: RB lookup in EST_RABS for cn:%d failed",tdsrrc_rbr.cn_to_release );
        /* send failure message */
        tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status, tdsrrc_rbr.status.prot_err_cause);
        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbr_clear_procedure();
        return;
      }
    }

    /* Check if there is CN information present. If so, we need to send the information to the Cell Change manager */
    if (TDSRRCRB_R9_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r9_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: Saving CN info for NAS");
      tdsrrc_rbr.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(
        &(rb_release_ptr->cn_InformationInfo) );
      if (tdsrrc_rbr.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd!");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbr.cn_info_saved = TRUE;
      }
    }

    /* Indicate layer1 if rb release message is going to release voice RAB */
    tdsrrcrbr_indicate_layer1_if_config_msg_release_voice_rab();

    tdsrrcrb_update_mac_rab_status(TDSRRC_PROCEDURE_RBR, RRC_CS_DOMAIN_CN_ID,
                                &rb_release_ptr->rb_InformationReleaseList, FALSE);

    /* Set TDSORDERED_CONFIG so that LLC can process the necessary parameters. */
    oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBR, tdsrrc_rbr.state_indicated_by_msg,
                      tdsrrc_DL_DCCH_Message_PDU, (void *)dcch_msg_ptr);

    switch ( oc_status )
    {
      case TDSORDERED_CONFIG_SET:
        tdsrrcrbr_process_after_oc_config_set_r9(dcch_msg_ptr, rb_release_ptr);
        break;

      case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
        tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
        tdsrrc_rbr.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_rbr.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
      
      case TDSORDERED_CONFIG_NOT_SET_OTHER:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
        /* When LLC sends this, there is a serious problem
        and we'll go back to disconnected state. No need
        to send the failure message here, we can just
        go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      default:

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
        /* Unknown error, just go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
    }/* end switch */

    if ( ordered_config_err_occured == TRUE )
    {
        /* No need to continue. Clear the procedure
        variables and return. */
        tdsrrcrbr_clear_procedure();
        return;
    }

    if(tdsrrc_rbr.rbr_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }
  }/* end if - valid rb message */
  return;
}

#endif /*FEATURE_TDSCDMA_REL9*/

/*===========================================================================

FUNCTION   RRCRBR_PROCESS_RBRELEASE_MESSAGE

DESCRIPTION

  This function processes the received RB Release Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Release Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to release the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Release Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_process_rbrelease_message
(
  tdsrrc_cmd_type *cmd_ptr
  
)
{

  tdsrrc_RadioBearerRelease *msg_ptr;  /* Pointer to the RB Release message        */
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr ;     /* Pointer to the RB Release message */

  /* A Downlink SDU has been received from RLC. This
  SDU should contain a Radio Bearer Release Message. Otherwise
  we would not have received this command. Copy the 
  received message pointer. */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* If ASN.1 decoding failed, the received pointer would be null.
  So we know we got a RB Release Message (by peeking into
  the encoded bit stream), but we were not able to decode it.
  Send a RB release failure message with the appropriate
  cause. */
  if ( dcch_msg_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"ASN decode failed for RBR msg");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
    tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
    tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                              tdsrrc_rbr.status.prot_err_cause
                            );

    /* Then change to initial substate */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbr_clear_procedure();

    /* Nothing else to do here */
    return;
  }

  /* Make sure it's the Radio Bearer Release message
  although we would not have gotten here unless it's
  the right message */
  if ( dcch_msg_ptr->message.t != T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Incorrect message given to RBR proc %d",dcch_msg_ptr->message.t);
    return;
  }
  
  if(TRUE == tdsrrccsp_reselection_in_progress())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Abort cell reselection as it's conflict with RBR");
    tdsrrccsp_abort_cell_reselection();
  }

  /* Get the pointer to the actual RB Release message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerRelease);

  /* The first thing to do is check the transaction id.
  If the transaction id is the same as the accepted
  or rejected transaction ids, we don't process
  this message. It's a duplicate and will just be
  dropped. But we also don't change the current
  sub-state since we're in the middle of
  processing a previous message. Note that
  the transaction id is included only if critical
  extensions are not present. */
  if (msg_ptr->t == T_tdsrrc_RadioBearerRelease_r3)
  {
    tdsrrc_print_supported_asn1_rel_version();

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported RB Rel t:%d",msg_ptr->t);

    if ( (msg_ptr->u.r3.radioBearerRelease_r3.rrc_TransactionIdentifier)
                   ==  tdsrrc_rbr.tr_ids.rejected_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBR msg! rejected tr-id%d match!",tdsrrc_rbr.tr_ids.rejected_transaction);
      /* Nothing else to do, just ignore the message and return */
    } 
    else
    {
      /* We have received a RBR message with a new transaction id.
      This message should be in our rejected transaction
      list */
      tdsrrc_rbr.tr_ids.rejected_transaction = 
        (msg_ptr->u.r3.radioBearerRelease_r3.rrc_TransactionIdentifier);
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
      tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;
      /*Check whether Cell Update is going on */
      if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
           (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
        if (SUCCESS != tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR))
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBR failed to register with CU");
        }
        /* Wait until Cell update procedure is completed */
        tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
      }
      else
      {
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause );
        /*continue the same substate */ 
      }
    }
    return;
  }
  
  if ((SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL4)))
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:rbreleaser4");
    tdsrrcrbr_process_rbrelease_r4(cmd_ptr);
    return;
  }
  else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
       (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL5))) 
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:rbreleaser5");
    tdsrrcrbr_process_rbrelease_r5(cmd_ptr);
    return;
  }
  else if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6) &&
       (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL6)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL:rbreleaser6");
    tdsrrcrbr_process_rbrelease_r6(cmd_ptr);
    return;
  }
  else if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7) &&
       (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL7)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL:rbreleaser7");
    tdsrrcrbr_process_rbrelease_r7(cmd_ptr);
    return;
  }
#ifdef FEATURE_TDSCDMA_REL8
  else if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8) &&
       (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL8)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Received RB Releases.");
    tdsrrcrbr_process_rbrelease_r8(cmd_ptr);
    return;
  }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  else if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9) &&
       (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL9)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:Received RB Releases.");
    tdsrrcrbr_process_rbrelease_r9(cmd_ptr);
    return;
  }
#endif /* FEATURE_TDSCDMA_REL9 */

  else
  {
    tdsrrc_print_supported_asn1_rel_version();

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported RB Rel t:%d",msg_ptr->t);

    if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                   ==  tdsrrc_rbr.tr_ids.rejected_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBR msg! rejected tr-id%d match!",tdsrrc_rbr.tr_ids.rejected_transaction);
      /* Nothing else to do, just ignore the message and return */
    } 
    else
    {
      /* We have received a RBR message with a new transaction id.
      This message should be in our rejected transaction
      list */
      tdsrrc_rbr.tr_ids.rejected_transaction = 
        (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
      tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;
      /*Check whether Cell Update is going on */
      if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
           (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
      {
        if(tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbr.curr_substate,tdsrrc_rbr.status.failure_status);
          tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                    tdsrrc_rbr.status.prot_err_cause );
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_VALID;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
          (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
          /* Wait until Cell update procedure is completed */
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        }
      }
      else
      {
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause );
        /*continue the same substate */ 
      }
    }
    return;
  }
}/* end function tdsrrcrbr_process_rbrelease_message */


/*===========================================================================

FUNCTION   RRCRBR_PROCESS_RNTI_UPDATE_CNF

DESCRIPTION

  This function processes RNTI_UPDATE_CNF
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_process_rnti_update_cnf
(
void
)
{
  tdsrrc_state_e_type curr_state;

  curr_state = tdsrrc_get_state();

  if ((curr_state == TDSRRC_STATE_CELL_FACH) && 
      ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
       (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)))
  {
   /*send the response message now */
    tdsrrcrbr_send_rbr_complete_message(TRUE);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
    /*wait for L2ack */
    tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    tdsrrcllc_print_trans_chnl_rate();
  }
  else
  {
    /* We should not get here*/
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unexpected RNTI Update cnf,RBR state %d",tdsrrc_rbr.curr_substate);
  }
}

/*===========================================================================

FUNCTION   RRCRBR_PROCESS_CHANNEL_CONFIG_CNF

DESCRIPTION

  This function processes the received Channel Config Confirmation
  from LLC. If the required Radio-bearers have been released succesfully,
  and if any of the radio-access-bearers have to be released because
  all the radio-bearers mapped to those RABs have been released, this
  information needs to be given to the NAS entity for that RAB.
  This is done by sending an RRC_SYNC_IND to MM.
  
  If a signalling connection has been released, this procedure has
  to notify the RRC Connection Establishment procedure since the
  RRC connection establishment procedure keeps track of all the
  signalling connections established for NAS.
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_process_channel_config_cnf
(
tdsrrc_channel_config_cnf_type *ptr  /* Pointer to the Channel config cnf cmd */
)
{
  tdsrrcrb_rnti_update_info_type rnti_update_info;   /* to hold rnti update info */
  mm_cmd_type *mm_ptr;   /* To send the MM_SYNC_IND command */
  tdsrrc_cmd_type *cmd; /* To send a LLC command if channel config failed */
  rabm_cmd_type *rabm_ptr; /* To send the RABMAS_RAB_RELEASE_IND  command */
  tdsrrc_rb_remove_result_type rab_result; /* To hold result
                         of removing RBs from ESTABLISHED_RABS */
  uint8 count;          /* Local counter */
  tdsrrc_CellUpdateCause       cu_cause;  /*To store CU cause from CU procedure*/
  tdsrrc_state_e_type curr_state;
#ifdef FEATURE_SCUDIF_ENHANCEMENT
  #error code not present
#endif
  boolean amr_configured = FALSE;
  boolean cs_rab_released = FALSE;
  tdsrrc_rlc_re_est_for_srns_rb_e_type rb_type; /*rb type tobe reestablished */
  uint32 current_mode = tdsrrc_get_current_codec_mode();


  /* Check if the channel configuration was succesful. */
  if( ptr->chan_cfg_status != TDSRRCLLC_CHAN_CFG_SUCCESS )
  {
     /*
        Since channel config comfirm came with a failure, hence revert the max count c value
        in the temporary variable to the original value
    */
    tdsrrcsmc_revert_max_count_c();
     
    /* Channel config failure, so initialise the ordered RB-Mapping database*/
    tdsrrcllc_init_ordered_hanging_rb_mapping_info();

    /*Here we initialize tdsrlc_size_change_in_progress DB so that if it was set by RBR, 
    it is ready for next procedure*/
    tdsrrc_initialize_rlc_size_change();

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CHAN_CONFIG_CNF with Failure");
    tdsrrc_set_ordered_codec_mode(current_mode);
    /* Roll back tdsmac_rab_status */
    tdsrrcrb_update_mac_rab_status(TDSRRC_PROCEDURE_RBR, RRC_CS_DOMAIN_CN_ID,NULL, TRUE);
    
    tdsrrc_rbr.status.failure_status = TDSRRCRB_PHY_CHAN_FAILURE;
    if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_CFG_CANCELLED)
    {
      /* cell update is already in progress as this is for pending config
       * cancellation
       */
      #ifdef FEATURE_TDSCDMA_RCR_CANCEL_RECFG
      if ( TRUE == tdsrrcrcr_is_proc_active())
      {
        /* Reset TDSORDERED_CONFIG - UE can now accept other ordered
        configurations */
        if (TDSOC_NOT_SET == tdsrrcllc_reset_ordered_config(TDSRRC_PROCEDURE_RBR
                                                      ,TRUE
                                                      ))
        {
          /* Then clear all local variables that may
          need to be cleared */
          tdsrrcrbr_clear_procedure();
          /* Make sure we're back in the initial sub-state */
          tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
        }
      }
      else
      #endif
      {
        tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      }
      return; 
    }
    
    /* validation failure happened.  Send RB failure msg with invalid config */
    if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_OTHER)
    {
      
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      if(tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG)
      {
        tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBR, TDSRRC_UNRECOVERABLE_ERROR, FALSE);
        return; 
      }

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RBR failure msg with invalid config");
      tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                tdsrrc_rbr.status.prot_err_cause
                                );
      /* Reset TDSORDERED_CONFIG - UE can now accept other ordered
      configurations */
      if (TDSOC_NOT_SET == tdsrrcllc_reset_ordered_config(TDSRRC_PROCEDURE_RBR
                                                    ,TRUE
                                                    ))
      {
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbr_clear_procedure();
        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to reset the OC");
      }
      /*During FACH->DCH transition, if T305 got expired, we need to start T305 with zero timeout.*/	  
      if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH && tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
      {
        if((tdsrrctmr_get_remaining_time(TDSRRCTMR_T_305_TIMER) == 0)
            &&  (tdsrrc_convert_t305_to_ms(TDSRRC_GET_T305()) != TDSRRCTMR_INFINITY_VAL ))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Chan_config_req failed for FACH->DCH. So Starting T305 with zero timeout.");        
          tdsrrctmr_start_timer(TDSRRCTMR_T_305_TIMER, 0);
        }
      }	  
      return;
    } /* if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_OTHER) */

    /* Check if PHY_CHAN_FAILURE happened in FACH->DCH Transition.  If so, initiate a cell update
    request, and tdsrrc_rbr substate should be TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF */ 
    if (tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH && tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PHYCHANFAIL FACH->DCH.  Initiate CU");

      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrc_rbr.status.failure_status = TDSRRCRB_PHY_CHAN_FAILURE;
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBR, TDSRRC_PHY_CHAN_FAILURE,TRUE);
      /* Wait for CELL UPDATE to get a valid C_RNTI */
      tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      tdsrrcrbr_send_initiate_cu_flag = FALSE;
      tdsrrcrbr_delete_crnti          = FALSE;
      return;
    }
    /* Check if PHY_CHAN_FAILURE happended within CELL_DCH.  For now, initatiate a cell update request
       with cause RL Failure.  RBR substate should be TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF*/
    else if (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH && tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
    {
      tdsrrc_rbr.status.failure_status = TDSRRCRB_PHY_CHAN_FAILURE;
      if(tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG)
      {
        tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBR, TDSRRC_RADIO_LINK_FAILURE, FALSE);
        return; 
      }
      if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_PHY_CHL)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBRC failed.  Going back to old config success.");
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                  );
        /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
        configurations */
        tdsrrcllc_clear_ordered_config(); 
      }
      else if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_BACK_TO_OLD_CONFIG)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBRC failed. Going back to old config failed. Initiate CU");
        /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */

        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBR, TDSRRC_RADIO_LINK_FAILURE,TRUE);
        /* Wait for CELL UPDATE to get a valid C_RNTI */
        tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        tdsrrcrbr_send_initiate_cu_flag = FALSE;
        tdsrrcrbr_delete_crnti          = FALSE;
        return;
      }
    }
    else if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
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
        cmd->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RBR;
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
      tdsrrc_rbr.status.failure_status = TDSRRCRB_PHY_CHAN_FAILURE;
      tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                              tdsrrc_rbr.status.prot_err_cause
                            );
    }
    /* Go back to initial substate */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
    /* At this point we don't need to remember any of the
    Radio Bearer information since it's stored in ESTABLISHED_RABS.
    Clear our internal variables */
    tdsrrcrbr_clear_procedure();

    return;
  }
 
  if((tdsordered_config.set_status == TDSOC_SET_FOR_DCH_FACH_TRANS) || 
        (tdsordered_config.set_status == TDSOC_SET_FOR_DCH_CELL_PCH_TRANS) ||
        (tdsordered_config.set_status == TDSOC_SET_FOR_DCH_URA_PCH_TRANS))
  {
    if(tdsrrc_rbr.high_mobility_ind)
    {
      tdsrrcrb_send_upd_ota_parms_ind(TDSRRC_PROCEDURE_RBR, TDSL1_CPHY_HIGH_MOBILITY_INCL);
    }
  }

  /*For FACH->DCH, if CHAN_CONFIG_REQ is succeeded, we need to stop T305*/  
  if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH && tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Chan_config succeeded for FACH->DCH. So Stopping T305");
    tdsrrctmr_stop_timer(TDSRRCTMR_T_305_TIMER);
  }    

    /*Notify the Cell_Id to registered entities*/
  if(tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
  {
    tdsrrc_notify_cell_id_in_dch();
  }	

  /* Copy the Ordered RB Mapping into the current RB-Mapping database*/
  tdsrrcllc_copy_ordered_hanging_rb_mapping_to_current();


  if(ptr->inter_freq_hho_status == TRUE)
  {
    tdsrrc_rbr.hho_ciph_status = TRUE;
  }
  else
  {
    tdsrrc_rbr.hho_ciph_status = FALSE;  
  }

  /*If any RBs need to be released, they need to be updated in tdsrrc_est_rabs as well.
  After releasing the RBs, if it results in releasing the RAB, NAS needs to be notified.*/
  if( tdsrrc_rbr.rbs_tobe_released.is_valid == TRUE )
  {
    /*Loop through all the RBs that needs to be released.*/
    for( count=0; count < tdsrrc_rbr.rbs_tobe_released.num_rbs; count++ )
    {

      /* Call the function to remove each RB */
      tdsrrc_remove_rb_from_established_rabs( (tdsrrc_rbr.rbs_tobe_released.rb_id[count]),
                                           &(rab_result));

      /* Now check if a RAB was removed because of this RB */
      if ( rab_result.result == TDSRRC_RB_AND_RAB_REMOVED )
      {
        /* Since a RAB was removed, we should send the indication to the appropriate NAS entity. For CS
        RABs send the command to MM layer and for PS RABs send it to RABM layer. */

        if ( rab_result.cn_domain == RRC_CS_DOMAIN_CN_ID )
        {
          cs_rab_released = TRUE;
          if ((mm_ptr = mm_tdsrrc_get_cmd_buf(RRC_SYNC_IND)) == NULL)
          {
            ERR_FATAL("Can't send MM cmd: 0x%x  Out of memory!",RRC_SYNC_IND,0,0);
          }
#ifdef FEATURE_DUAL_SIM
          mm_ptr->cmd.rrc_sync_ind.as_id = tdsrrc_get_as_id();
#endif
          mm_ptr->cmd.rrc_sync_ind.cn_domain_id = RRC_CS_DOMAIN_CN_ID;
          mm_ptr->cmd.rrc_sync_ind.cause = RRC_RAB_ESTABLISHED;
          mm_ptr->cmd.rrc_sync_ind.rab_id_present = TRUE;
          mm_ptr->cmd.rrc_sync_ind.rab_info.action = RAB_RELEASED;
          mm_ptr->cmd.rrc_sync_ind.rab_info.rab_id = rab_result.rab_id;


          if(rab_result.rab_type == TDSRRCRB_CS_VOICE_RAB)
          {
            mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_CS_VOICE_CALL;
          }
          else if(rab_result.rab_type == TDSRRCRB_CS_DATA_RAB)
          {
            mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_CS_DATA_CALL;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Something wrong with RAB Type");
            mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_INVALID_RAB_ID;
          }
          /* Fill in the codec type used in RRC_SYNC_IND*/
          if(rab_result.rab_type == TDSRRCRB_CS_VOICE_RAB)
          {
            if(tdsrrc_get_current_codec_mode() == TDSCDMA_IVOCODER_ID_AMRWB )
            {
              mm_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_WB_AMR;
            }
            else
            {
              mm_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_NB_AMR;
            }
          }
          else
          {
            mm_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_NONE;
          }

#ifdef FEATURE_UMTS_VOICE_CIPHERING_IND
          #error code not present
#endif
          /* Put the command on MM queue */
          /* Put the command on MM queue */
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sending RRC_SYNC_IND to MM for RAB type %d RAB-id %d",mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type,mm_ptr->cmd.rrc_sync_ind.rab_info.rab_id);
          if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
          {
           tdscfa_mm_put_cmd(mm_ptr);
          }
          mm_put_cmd(mm_ptr);
        }/* end if cs domain */
        else if ( rab_result.cn_domain == RRC_PS_DOMAIN_CN_ID )
        {
          if ((rabm_ptr = rabm_get_cmd_buf()) == NULL)
          {
            ERR_FATAL("Can't send RABM cmd: 0x%x Out of memory!",RABMAS_RAB_RELEASE_IND,0,0);
          }
          rabm_ptr->header.cmd_id = RABMAS_RAB_RELEASE_IND;
          rabm_ptr->header.message_set = MS_RABM_RRC;
          rabm_ptr->cmd.rrc_release_ind.rabid = 
          (rabid_T)(rab_result.rab_id);
#ifdef FEATURE_DUAL_SIM
          rabm_ptr->cmd.rrc_release_ind.as_id = tdsrrc_get_as_id();
#endif
          /* Put the command on RABM queue */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RABMAS_RAB_RELEASE_IND to RABM");
          if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
          {
            tdscfa_rabm_put_cmd(rabm_ptr);
          }
          rabm_put_cmd(rabm_ptr);
        }/* end if ps domain */
      }/* end if RB_AND_RAB_REMOVED */

    }/* end for */
  }/* end if tdsrrc_rbr.rbs.need_to_release_rbs == TRUE */

  /* Since channel config confirm has been received with a success, hence update the value of 
  maximum Count C in the permanent variable from the temporary one */
  tdsrrcsmc_update_count_c_released_rbs();


  /*Go through all the RABs that came through RBR OTA. Usually, these should come
  either for SCUDIF or Codec change for Voice. But UTRAN can give RABs in RBR without
  these scenarios as well, just for sync purpose. For each of the RAB, NAS needs to be notified*/
  for( count=0; count < tdsrrc_rbr.rabs.num_rabs_in_msg; count++ )
  {
    if (tdsrrc_rbr.rabs.rabs_in_msg[count].cn_domain == RRC_CS_DOMAIN_CN_ID )
    {
#ifdef FEATURE_SCUDIF_ENHANCEMENT
#error code not present
#else /*FEATURE_SCUDIF_ENHANCEMENT*/
      (void)tdsrrcrbr_configure_rab_change(count);
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/
      amr_configured = TRUE;
    }
    else if ( tdsrrc_rbr.rabs.rabs_in_msg[count].cn_domain == RRC_PS_DOMAIN_CN_ID)
    {
      if ((rabm_ptr = rabm_get_cmd_buf()) == NULL)
      {
        ERR_FATAL("Can't send RABM cmd: 0x%x Out of memory!",RABMAS_RAB_ESTABLISH_IND,0,0);
      }
      rabm_ptr->header.cmd_id = RABMAS_RAB_ESTABLISH_IND;
      rabm_ptr->header.message_set = MS_RABM_RRC;
      rabm_ptr->cmd.rrc_establish_ind.rabid = 
      (rabid_T)(tdsrrc_rbr.rabs.rabs_in_msg[count].rab_id);

#ifdef FEATURE_DUAL_SIM
      rabm_ptr->cmd.rrc_establish_ind.as_id = tdsrrc_get_as_id();
#endif

      /* Put the command on RABM queue */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RABMAS_RAB_ESTABLISH_IND to RABM");
      if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
      {
        tdscfa_rabm_put_cmd(rabm_ptr);
      }
      rabm_put_cmd(rabm_ptr);
    }/* end if ps domain */   
  }


  /*Here we handle SCUDIF without any RAB Reconfig IE in RBR OTA*/
#ifdef FEATURE_SCUDIF_ENHANCEMENT
  #error code not present
#endif/*FEATURE_SCUDIF_ENHANCEMENT*/


  /*Here we handle the case of AMR Rate change without any RAB Reconfig IE in RBR OTA */
  if((cs_rab_released == FALSE) && (amr_configured == FALSE)
#ifdef FEATURE_SCUDIF_ENHANCEMENT
    #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/
    )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TV_TRRC: Configuring TDS Voice Adapter with mode %d from RBR.",current_mode);
    tdsrrcrb_update_amr_cctrch_id_for_reconfig(current_mode);
  }


  /* Check for CELL FACH before sending the tdsrrc_rbr complete message */
  curr_state=tdsrrc_get_state();
  if ((TDSRRC_STATE_CELL_FACH == curr_state) && 
      ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH) ||
       (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
       (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)))
  {
    if (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) 
    {
      /* this is the case of CELL_FACH->CELL_PCH state transition.
      In current implementation we do initiate cell selection. Instead
      we treat current cell as strongest cell */
      if (tdsrrc_rbr.directed_cell_info)
      {
        /* check whether current camped scr code is same as the one
        sent in the message. If not then initiate cell update procedure */
        if(!tdsrrccsp_is_selected_cell_utran_directed())
        {
          /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
          tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBR,
                                              TDSRRC_CELL_RESELECTION,
                                              FALSE);
        }
        /* No need to send ul data registration to rlc. This will be taken care
        by cellupdate procedure */
        tdsrrcrbr_process_successful_proc_completion(FALSE);
      }
      else
      {
        /* send drx info to L1. Since this is CELL_FACH->CELL_PCH state transition
        RB procedure is responsible for sending the DRX info to lower layer*/
        tdsrrcrb_send_l1_drx_req (TDSRRC_PROCEDURE_RBR);

        tdsrrcrbr_process_successful_proc_completion (TRUE);
      }
      /* return from here */
      return;
    }
    else if (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      /* send drx info to L1 */
      tdsrrcrb_send_l1_drx_req (TDSRRC_PROCEDURE_RBR);
      /* check whether URA update is required */
      if ( tdsrrccu_is_ura_update_reqd() == TRUE)
      {
        tdsrrcrb_send_ura_update_initiate_req( TDSRRC_PROCEDURE_RBR,
                                            TDSRRC_URA_RESELECTION,
                                            FALSE);
      }
      /* no need to wait for ura update completion 
      declare that proc is successfully completed */
      tdsrrcrbr_process_successful_proc_completion (TRUE);
      /* return from here */
      return;
    }
    if(tdsrrcrbr_send_initiate_cu_flag
       || (tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_RB_PROC_COMPL_IND) 
       )
    {
      if (tdsrrcrbr_delete_crnti)
      {
        /* Send a RNTI Update Request */
        rnti_update_info.crnti_action         = TDSRRC_RNTI_DELETE;
        rnti_update_info.urnti_action         = TDSRRC_RNTI_NOCHANGE;
        rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBR;
        rnti_update_info.rlc_re_establish_srb = FALSE;
        rnti_update_info.rlc_re_establish_rab = FALSE;
        rnti_update_info.cnf_reqd             = FALSE;

        tdsrrcrb_send_rnti_update_req(rnti_update_info);
      }
      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBR,
                                          TDSRRC_CELL_RESELECTION,
                                          TRUE);
      /* Wait for CELL UPDATE to get a valid C_RNTI */
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_VALID;
      tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      tdsrrcrbr_send_initiate_cu_flag = FALSE;
      tdsrrcrbr_delete_crnti          = FALSE;
      return;
    }
    /*Following case will occur when RLC RESET happens on SRB2 during DCH->FACH Transition*/
    else if((tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG) &&
      (tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) == TDSRRCCU_STARTED) && 
      (cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError) && (tdsrrccu_get_rb_id_unrec_error() == 
      DCCH_AM_RADIO_BEARER_ID))
    {
      if(tdsrrc_rbr.new_crnti_valid)
      {
        /* Send a RNTI Update Request */
        rnti_update_info.crnti_action         = TDSRRC_RNTI_UPDATE;
        rnti_update_info.urnti_action         = TDSRRC_RNTI_NOCHANGE;
        rnti_update_info.crnti                = tdsrrc_rbr.new_crnti;
        rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBR;
        rnti_update_info.rlc_re_establish_srb = FALSE;
        rnti_update_info.rlc_re_establish_rab = FALSE;
        rnti_update_info.cnf_reqd             = FALSE;
  
        tdsrrcrb_send_rnti_update_req(rnti_update_info);
        /* Send a Radio Bearer Setup Complete later */      
      }

      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBR,
                                          TDSRRC_UNRECOVERABLE_ERROR,
                                          TRUE);

      /* Wait for CELL UPDATE to get a valid C_RNTI */
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_VALID;
      tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;

      return;                                          
    }    
    /* Coming here implies the "tdsrrcrbr_send_initiate_cu_flag" is FALSE.
     * Check if Valid CRNTI is stored in RBR (for DCH->FACH case).
     * If so, send a RNTI Update Req to LLC and NOT wait for
     * Cell Update Complete Confirm. Cell Update will NOT be 
     * initiated when there is a valid C-RNTI and tdsrrcrbr_send_initiate_cu_flag
     * is FALSE.
     */
    else if(tdsrrc_rbr.new_crnti_valid)
    {
      /* Send a RNTI Update Request */
      rnti_update_info.crnti_action         = TDSRRC_RNTI_UPDATE;
      rnti_update_info.urnti_action         = TDSRRC_RNTI_NOCHANGE;
      rnti_update_info.crnti                = tdsrrc_rbr.new_crnti;
      rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBR;
      rnti_update_info.rlc_re_establish_srb = FALSE;
      rnti_update_info.rlc_re_establish_rab = FALSE;
      rnti_update_info.cnf_reqd             = FALSE;

      tdsrrcrb_send_rnti_update_req(rnti_update_info);
    }

    /* Query for valid C-RNTI */
    else if (TDSC_RNTI_NOT_VALID == tdsrrcllc_get_current_crnti_status() )
    {
      /* Wait for CELL UPDATE to get a valid C_RNTI. Register for CU complete cnf. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No Valid C_RNTI! Register for CU CNF");
      if (SUCCESS != tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBR failed to register with CU");
      }
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_VALID;
      tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      return;
    }    
  } /* if current state is CELL_FACH */
  else if (TDSRRC_STATE_CELL_PCH == curr_state)
  {
    if(tdsrrcrbr_send_initiate_cu_flag)
    {
      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBR,
                                          TDSRRC_CELL_RESELECTION,
                                          FALSE);
      tdsrrcrbr_send_initiate_cu_flag = FALSE;
      /* No need to send ul data registration to rlc. This will be taken care
      by cellupdate procedure */
      tdsrrcrbr_process_successful_proc_completion(FALSE);

      return;
    }
    else
    {
      /*Perform the final steps for successful completion of the
      procedure */
      tdsrrcrbr_process_successful_proc_completion(TRUE);
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
        tdsrrcrb_send_ura_update_initiate_req( TDSRRC_PROCEDURE_RBR,
                                            TDSRRC_URA_RESELECTION,
                                            FALSE);
      }
    }
    /* No need to send ul data registration to rlc. This will be taken care
    by cellupdate procedure */
    tdsrrcrbr_process_successful_proc_completion(TRUE);

    return;
  }

  /*check whether this reconfiguration involves SRNS relocation */
  if (tdsrrc_rbr.rbr_for_srns_relocation == TRUE)
  {
//  if (TDSRRC_STATE_CELL_DCH == curr_state)
  {

    /* Re-establish SRB2 before sending the response message */
    rb_type  = TDSRRC_RE_EST_RB_2;
    tdsrrcrb_send_rlc_re_est_req_to_smc(TDSRRC_PROCEDURE_RBR, rb_type, TRUE);

    tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_REESTABLISH_SRB2;
  }
  }
  else /*normal reconfiguration without SRNS relocation */
  {   
    /* Send the Radio Bearer Release Complete message */
    if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
        (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

    {
      tdsrrcrbr_send_rbr_complete_message(TRUE);
    }
    else
    {
         /* Need to wait for L2 ack for all states because RRC needs to trigger include reconfig status indicator
    if RLC acknowledgement is not yet received.
    As per 8.3.1.3
    1> if the IE "Cell update cause" is set to "radio link failure" and the UE has submitted a reconfiguration response
    message to RLC and the UE has not received the RLC acknowledgement for the response message:
    2> include the IE "Reconfiguration Status Indicator" and set it to TRUE.*/
      tdsrrc_update_reconfig_waiting_for_l2ack_dch(TDSRRC_PROCEDURE_RBR);
      tdsrrcrbr_send_rbr_complete_message(TRUE);

      if((tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG)
        && (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH))
      {
        if(tdsrrccu_check_rb_in_estab_rab())
        {
          /* IF CU is waiting to handle RLF/RLC unrecoverable error, then send complete
           * and delay for 50ms so to make sure Complete goes out to NW on old config
           */   
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delaying %d ms",TDSRRCRB_PND_CFG_DELAY);
          (void) rex_clr_sigs(rex_self(), TDSRRCRBR_TEMP_TIMER_SIG);
          (void) rex_set_timer(&tdsrrcrbr_temp_timer, TDSRRCRB_PND_CFG_DELAY); 
          tdsrrcrbr_sig = rex_wait(TDSRRCRBR_TEMP_TIMER_SIG);
          TDSRRC_MSG1(MSG_LEGACY_LOW,"RRCRBR signal:%d",tdsrrcrbr_sig);
          (void) rex_clr_sigs(rex_self(), TDSRRCRBR_TEMP_TIMER_SIG);
          tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBR, TDSRRC_UNRECOVERABLE_ERROR, FALSE);
          
        }
      }
      /*Following case handles the scenario when RF Failure or RLC unrecoverable error occured
        during DCH -> FACH or FACH -> FACH transiton.*/
      else if((tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG) &&
        (tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) == TDSRRCCU_STARTED))
      {
        if((cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError) && (tdsrrccu_get_rb_id_unrec_error() != DCCH_AM_RADIO_BEARER_ID))
        {
          /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
          tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBR,
                                              TDSRRC_UNRECOVERABLE_ERROR,
                                              TRUE);
        }
        else if(cu_cause == tdsrrc_CellUpdateCause_radiolinkFailure)
        {
          /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
          tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBR,
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
    if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
        (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");

      /* Change the substate to the next substate - this is
      where we wait for L2 ack from RLC for sending the RBR
      complete message. */
      tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    }
    else
    {
      
      if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
      {
        if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
        {      
          (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
        }
        else
        {
          (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
        }
        tdsrrc_rbr.need_to_release_cn_domain = FALSE;
      }


      /* send the nas cn info if stored */
      if (tdsrrc_rbr.cn_info_saved != FALSE)
      {
        /* Put the command on MM queue */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RRC_SERVICE_IND_FROM_OTA_MSG to MM");
        if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
        {
          tdscfa_mm_put_cmd(tdsrrc_rbr.mm_cmd_ptr);
        }
        mm_put_cmd(tdsrrc_rbr.mm_cmd_ptr);
        /* clear the flag */
        tdsrrc_rbr.cn_info_saved = FALSE;
      }

      /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
      configurations */
      tdsrrcllc_clear_ordered_config();  

      /* Go to the next sub-state in the state machine. That's
      the initial substate since we've sent the RBR complete
      message */
      tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

      /* At this point we don't need to remember any of the
      Radio Bearer information since it's stored in ESTABLISHED_RABS.
      Clear our internal variables */
      tdsrrcrbr_clear_procedure();
    }
  }
}

/*===========================================================================

FUNCTION   RRCRBR_PROCESS_RLC_REESTABLISHMENT_CNF

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
static void tdsrrcrbr_process_rlc_reestablishment_cnf
(
void
)
{
  tdsrrcrb_rnti_update_info_type rnti_update_info; /* structure to be used to request RNTI update */

  /* send rnti update request if URNTI was present in the rebsetup message */
  if (tdsrrc_rbr.new_urnti_valid == TRUE)
  {
    /* Send the RNTI update request to rrcllc to update with new-URNTI */
    /* Send a RNTI Update Request */
    rnti_update_info.crnti_action         = TDSRRC_RNTI_NOCHANGE;
    rnti_update_info.urnti_action         = TDSRRC_RNTI_UPDATE;
    rnti_update_info.urnti                = tdsrrc_rbr.new_urnti;
    rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBR;
    rnti_update_info.rlc_re_establish_srb = FALSE;
    rnti_update_info.rlc_re_establish_rab = FALSE;
    rnti_update_info.cnf_reqd             = FALSE;

    tdsrrcrb_send_rnti_update_req(rnti_update_info);
  }

  /* Send the reconfiguration complete message */

  tdsrrcrbr_send_rbr_complete_message(TRUE);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");

  /* Change the substate to the next substate - this is
     where we wait for L2 ack from RLC for sending the RB Reconfig
     complete message. */
  tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
  tdsrrcllc_print_trans_chnl_rate();
  
}

/*===========================================================================

FUNCTION   RRCRBR_PROCESS_STATE_CHANGE_IND

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
static void tdsrrcrbr_process_state_change_ind
(
  tdsrrc_state_change_ind_type *ptr /* Pointer to the State Change Ind cmd */
)
{
  tdsrrc_CellUpdateCause cu_cause;
  /* Check if we're going to disconnected state */
  if( ptr->new_state == TDSRRC_STATE_DISCONNECTED )
  {
    /* Then change to initial substate */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbr_clear_procedure();
  }

  else if (ptr->new_state == TDSRRC_STATE_CELL_FACH )
  {
    if(ptr->previous_state == TDSRRC_STATE_CELL_DCH)
    {
      if (((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
          (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)) &&
           (tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_L2_ACK))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"state change ind rbr_st:%d",tdsrrc_rbr.curr_substate); 
        /* Then change to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbr_clear_procedure();
      }
      else if (tdsrrc_rbr.status.failure_status == TDSRRCRB_MSG_VALID)
      {
        /* if cell update procedure is active with cause OOS, wait 
         * for the CU complete notification to send chan_config_req to
         * LLC 
         */
        if ((tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause)== TDSRRCCU_STARTED) &&
            (cu_cause == tdsrrc_CellUpdateCause_re_enteredServiceArea) &&
            (tdsrrcllc_get_toc_usage() ==TDSTOC_FOR_OOS))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Complete tdsrrc_rbr msg first.  Indicate compl to CU once done");
        }

        TDSRRC_MSG0(MSG_LEGACY_MED,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
        /* Call the function that creates and sends the  channel
           config information to be requested from LLC. */
        tdsrrcrbr_send_channel_config_req();
        /* Change the substate since we're waiting for the
           confirmation from LLC for the channel configs */
        tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;

        if((tdsrrccsp_is_selected_cell_utran_directed()) &&
           (tdsrrc_rbr.new_crnti_valid))
        {
          /* No need to initiate cell update as the C-RNTI is available
           * and UE is camped on the directed cell.
           */
          tdsrrcrbr_send_initiate_cu_flag = FALSE;
        }
        else
        {
          tdsrrcrbr_send_initiate_cu_flag = TRUE;
          tdsrrcrbr_delete_crnti = TRUE;
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
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received State change to CELL_FACH,no action");

    }
  }
  else if (ptr->new_state == TDSRRC_STATE_CELL_PCH || 
           ptr->new_state == TDSRRC_STATE_URA_PCH)
  {
    if(ptr->previous_state == TDSRRC_STATE_CELL_DCH)
    {
      if (ptr->new_state == TDSRRC_STATE_CELL_PCH) 
      {
        if (tdsrrc_rbr.directed_cell_info)
        {
          if (tdsrrccsp_is_selected_cell_utran_directed()) 
          {
            /* No need to initiate cell update UE is camped on 
            the directed cell. */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Directed cell match,No CU reqd");
            tdsrrcrbr_send_initiate_cu_flag = FALSE;
          }
          else
          {
            tdsrrcrbr_send_initiate_cu_flag = TRUE;
            tdsrrcrbr_delete_crnti          = TRUE;
          }
        }
        else
        {
          /* we have to initiate cell update incase cpich info is not present
          in the message */
          tdsrrcrbr_send_initiate_cu_flag = TRUE;
          tdsrrcrbr_delete_crnti          = TRUE;
        }
      }
      else
      {
        /* URA_PCH case */
        tdsrrcrbr_send_initiate_cu_flag = FALSE;
      }

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
      /* Call the function that creates and sends the  channel
      config information to be requested from LLC. */
      tdsrrcrbr_send_channel_config_req();

      /* Change the substate since we're waiting for the
         confirmation from LLC for the channel configs */
      tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
    }
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
    else if ((ptr->previous_state == TDSRRC_STATE_CELL_FACH) &&
     ( tdsrrc_rbr.initiate_cell_selection_trans_from_fach == TRUE))
    {
      tdsrrc_rbr.initiate_cell_selection_trans_from_fach = FALSE;
     
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
       /* Call the function that creates and sends a channel
          config request to LLC. */
      if (tdsrrc_rbr.directed_cell_info)
      {
        if(tdsrrccsp_is_selected_cell_utran_directed())
        {
           /* No need to initiate cell update as the C-RNTI is available
            * and UE is camped on the directed cell.
            */
          tdsrrcrbr_send_initiate_cu_flag = FALSE;
        }
        else
        {
          tdsrrcrbr_send_initiate_cu_flag = TRUE;
          tdsrrcrbr_delete_crnti          = TRUE;
        }
      }
      else
      {
        tdsrrcrbr_send_initiate_cu_flag = TRUE;
        tdsrrcrbr_delete_crnti          = TRUE;
      }
      tdsrrcrbr_send_channel_config_req();
       /* Change the substate since we're waiting for the
          confirmation from LLC for the channel configs */
      tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
    }
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Recvd state change %d to %d RBR state %d, No action",
               ptr->previous_state, ptr->new_state, tdsrrc_rbr.curr_substate);
    }

    /* IF CU is waiting for RB proc to complete, no need to initate CU through RB procedure. Reset flag. */
    if (tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_RB_PROC_COMPL_IND) 
    {
      tdsrrcrbr_send_initiate_cu_flag = FALSE;
    }

  }
  else
  {
    /* We should not get here since we haven't registered for
    other state changes */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Unsupported state change %d to %d",ptr->previous_state,ptr->new_state);
  }
}




/*===========================================================================

FUNCTION   RRCRBR_PROCESS_L2_ACK_IND

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
static void tdsrrcrbr_process_l2_ack_ind
(
tdsrrc_downlink_l2ack_ind_type *ptr /* Pointer to the L2 Ack Ind cmd */
)
{
  tdsrrc_cmd_type     *cmd; /* To send a LLC command if L2 tx failed */
  tdsrrc_state_e_type current_state;       /* current RRC State  */
  tdsrrc_rlc_re_est_for_srns_rb_e_type rb_type; /*rb type tobe reestablished */
  tdsrrc_CellUpdateCause       cu_cause;
  uint8                    rejected_tr_id = TDSRRCRB_INVALID_TRANSACTION_ID;

  if ( ptr->status == SUCCESS )
  {
    current_state = tdsrrc_get_state(); 

    /* Back up accepted transaction ID */
    rejected_tr_id = tdsrrc_rbr.tr_ids.accepted_transaction;
    
    /* clear transaction id*/
    tdsrrc_rbr.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
      
    if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
        (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
    {
      if(current_state==TDSRRC_STATE_CELL_DCH) 
      {
        if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
        {
          if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
          {      
            (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
          }
          else
          {
            (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
          }
          tdsrrc_rbr.need_to_release_cn_domain = FALSE;
        }
       
        /*L2 Ack for the RBR Complete message is received on the old configuration
        Now initiate a cell selection */
        tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR,
                                                TDSRRC_TRANSITION_FROM_DCH,
                                                tdsrrc_rbr.state_indicated_by_msg);
        /*wait for the state_change indication after the cell selection
        is successfully completed */
        tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

        /* Start T_305 [periodic cell update] timer */
        tdsrrccu_start_t_305_timer();

      }
      else if(current_state==TDSRRC_STATE_CELL_FACH) 
      {
        if (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) &&
            (cu_cause == tdsrrc_CellUpdateCause_re_enteredServiceArea || cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError ||
             cu_cause == tdsrrc_CellUpdateCause_radiolinkFailure))
        {
          /* This is a case where cell update is in progress while RBR transitions 
             UE to Cell_PCH or URA_PCH. Though L2 Ack is gotten, send a PCR failure 
             later when CU procedure gets Cell update confirm. If we were to act on 
             L2 Ack, RRC moves to Cell_PCH/URA_PCH where cell update confirm message
             is not handled (unless CU registers on RBR's behalf) & UE would stuck 
             in Cell_PCH forever [in the absence of reselections].
          */
          if(SUCCESS != tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBR procedure failed to register with CU");
          }
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Registered for Cell Update procedure completion: cu_cause = %d",cu_cause);
          tdsrrc_rbr.status.failure_status = TDSRRCRB_PHY_CHAN_FAILURE;
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;

          /* Restore the accepted transaction ID for sending failure later */
          tdsrrc_rbr.tr_ids.rejected_transaction = rejected_tr_id;
        }
        else
        {
 #ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          if((tdsrrc_rbr.initiate_cell_selection_trans_from_fach == TRUE )
            && (tdsrrccsp_check_initiate_cell_selection_handling() == TRUE))
          {
           
            /*Initiate the cell selection*/
            tdsrrcllcoc_update_toc_with_oc_from_fach();
             
            tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR,
                                                    TDSRRC_TRANSITION_FROM_FACH,
                                                    tdsrrc_rbr.state_indicated_by_msg);
 
 
             
             
            tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
          }
          else
#endif
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
            tdsrrc_rbr.initiate_cell_selection_trans_from_fach = FALSE;
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

            /* Call the function that creates and sends a channel
                config request to LLC. */
            tdsrrcrbr_send_channel_config_req();
            /* Change the substate since we're waiting for the
                confirmation from LLC for the channel configs */
            tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
          }
        }
      }
      else
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid combination of rrcstate%d and rrcrbr substate%d",current_state,tdsrrc_rbr.curr_substate);
      }

      /*return from here */
      return;
    }

    /* if SRNS relocation is involved with this message then re
    establish the RBs other than RB2 here */
    if (tdsrrc_rbr.rbr_for_srns_relocation)
    {
      if (TDSRRC_STATE_CELL_DCH == tdsrrc_get_state())
      {
        /* Initiate the re-establishment of remaining RBs before 
        RBR procedure goes back to initial substate. 
        No need to wait for the confirmation */
        rb_type  = TDSRRC_RE_EST_NON_RB_2_UM_AM;
        tdsrrcrb_send_rlc_re_est_req_to_smc(TDSRRC_PROCEDURE_RBR, rb_type, FALSE);
        /* send the nas cn info if stored */
        if (tdsrrc_rbr.cn_info_saved != FALSE)
        {
          /* Put the command on MM queue */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RRC_SERVICE_IND_FROM_OTA_MSG to MM");
          if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
          {
            tdscfa_mm_put_cmd(tdsrrc_rbr.mm_cmd_ptr);
          }
          mm_put_cmd(tdsrrc_rbr.mm_cmd_ptr);
          /* clear the flag */
          tdsrrc_rbr.cn_info_saved = FALSE;
        }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"SRNS reloc flag is ON in rrc state %d",tdsrrc_get_state());
      }
    }

  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"L2 ack failed for tx of RBR Complete msg");

    /* Here make sure that CU is not active.  If CU is active with OOS area handling
      then NACK coming from RLC shall be ingnored as RBR failure with cause
      CU complete should be sent to NW */
    if (TDSRRC_STATE_CELL_FACH == tdsrrc_get_state() &&
        TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) && 
        (cu_cause == tdsrrc_CellUpdateCause_re_enteredServiceArea || cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError ||
         cu_cause == tdsrrc_CellUpdateCause_radiolinkFailure))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NACK being ingnored as RBR failure msg will be sent");
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
      (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Registered for Cell Update procedure completion: cu_cause = %d",cu_cause);
      return;
    }

    /* clear transaction id */
    tdsrrc_rbr.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;

    if (tdsrrc_rbr.rbr_for_srns_relocation)
    {
      /* l2ack failed. If reconfiguration involves SARNS relocation then
      go to idle. Otherwise just complete the procedure */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"L2ack failed SRNSreloc:%d,next_st:%d",tdsrrc_rbr.rbr_for_srns_relocation,tdsrrc_rbr.state_indicated_by_msg);
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
        cmd->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RBR;
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

  /* Note OC is set at this point. No need to clear it
  if the next state is cell_pch or ura_pch and
  l2 ack failied because cu procedure will take care of it */
  if (((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)) &&
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
      if(tdsrrcllc_reset_ordered_config(TDSRRC_PROCEDURE_RBR
                                     ,TRUE
                                     ) != TDSOC_NOT_SET)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdsrrcllc_reset_ordered_config FAILED ");
      }
    }
  }
  else
  {
    if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
    {
      if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
      {      
        (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
      }
      else
      {
        (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
      }
      tdsrrc_rbr.need_to_release_cn_domain = FALSE;
    }
    /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
    configurations */
    tdsrrcllc_clear_ordered_config();  
  }

#if defined(FEATURE_3GPP_FAST_DORMANCY)
  if(tdsrrc_rbr.rbr_for_srns_relocation == TRUE)
  {
    tdsrrcdormancy_update_dormancy_for_srns_hho_cco(TDSRRC_PROCEDURE_RBR);
  }
#endif  

  /* Go to the next sub-state in the state machine. That's
  the initial substate since we've sent the RBR complete
  message */
  tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

  /* At this point we don't need to remember any of the
  Radio Bearer information since it's stored in ESTABLISHED_RABS.
  Clear our internal variables */
  tdsrrcrbr_clear_procedure();
}


/*===========================================================================

FUNCTION   RRCRBR_INITIAL_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBR procedure is in the TDSRRCRB_INITIAL substate. This function
  looks at the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbr_initial_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the received command's id */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {

    case TDSRRC_DOWNLINK_SDU_IND:
      
      /* A Downlink SDU has been received from RLC. This
      SDU should contain a Radio Bearer Release Message. Otherwise
      we would not have received this command. Copy the 
      received message pointer. */
      /* Process the RB Release message */
      tdsrrcrbr_process_rbrelease_message( cmd_ptr );
      break;

    default:

      /* No other events except Downlink SDU is expected in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
      break;
  }
}

/*===========================================================================

FUNCTION   tdsrrcrbr_other_wfcucnf_substate_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the RBR procedure is in the TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate
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
static void tdsrrcrbr_other_wfcucnf_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_STATE_CHANGE_IND:
      /* Indication that RRC's state has changed */
      tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;
    
    default:    
      /* No other events expected in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
      break;
  }
}

/*===========================================================================

FUNCTION   RRCRBR_DCH_WFSRB2REESTABLISH_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_REESTABLISH_SRB2 substate
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
static void tdsrrcrbr_dch_wfsrb2reestablish_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_RE_EST_RLC_FOR_SRNS_CNF:

    /* Confirmation from SMC of RLC re-establishment confirm */
    tdsrrcrbr_process_rlc_reestablishment_cnf( );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Reconfig Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Reconfig message */
    tdsrrcrbr_process_rbrelease_message( cmd_ptr );

    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }
}

/*===========================================================================

FUNCTION   tdsrrcrbr_other_wfcscnf_substate_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the RBR procedure is in the TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF substate
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
static void tdsrrcrbr_other_wfcscnf_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_STATE_CHANGE_IND:
      /* Indication that RRC's state has changed */
      tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;
    
    default:    
      /* No other events expected in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
      break;
  }
}


/*===========================================================================

FUNCTION   RRCRBR_DCJ_WFCUCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the tdsrrc_rbr procedure is in the TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate
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

static void tdsrrcrbr_dch_wfcucnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    if (tdsrrc_rbr.status.failure_status == TDSRRCRB_SIMULTANEOUS_RECONFIG)
    {
      /* Send the RB Release Failure Message */
      tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                tdsrrc_rbr.status.prot_err_cause
                              );
      /* Then clear all local variables that may
      need to be cleared */
      tdsrrcrbr_clear_procedure();

      /* Make sure we're back in the initial sub-state */
      tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
    }
    else if (tdsrrc_rbr.status.failure_status == TDSRRCRB_MSG_VALID)
    {
      /* Send the Radio Bearer Release Complete message */
      if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
          (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

      {
        tdsrrcrbr_send_rbr_complete_message(TRUE);
      }
      else
      {
        tdsrrcrbr_send_rbr_complete_message(FALSE);
      }

      tdsrrcllc_print_trans_chnl_rate();
      /*if next state is Cell_PCH or URA_PCH then we have to 
      treat the procedure as successful only after getting L2ack
      for the response message */
      if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
          (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");

        /* Change the substate to the next substate - this is
        where we wait for L2 ack from RLC for sending the RBR
        complete message. */
        tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
      }
      else
      {
        if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
        {
          if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
          {      
            (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
          }
          else
          {
            (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
          }
          tdsrrc_rbr.need_to_release_cn_domain = FALSE;
        }

        /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
        configurations */
        tdsrrcllc_clear_ordered_config();  

        /* send the nas cn info if stored */
        if (tdsrrc_rbr.cn_info_saved != FALSE)
        {
          /* Put the command on MM queue */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RRC_SERVICE_IND_FROM_OTA_MSG to MM");        
          if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
          {
            tdscfa_mm_put_cmd(tdsrrc_rbr.mm_cmd_ptr);
          }
          mm_put_cmd(tdsrrc_rbr.mm_cmd_ptr);
          /* clear the flag */
          tdsrrc_rbr.cn_info_saved = FALSE;
        }

        /* Go to the next sub-state in the state machine. That's
        the initial substate since we've sent the RBR complete
        message */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* At this point we don't need to remember any of the
        Radio Bearer information since it's stored in ESTABLISHED_RABS.
        Clear our internal variables */
        tdsrrcrbr_clear_procedure();
      }

    }
    /*  In the case of Phy_Chan_Failure, RB setup failure must be send
    and oc should be reset. */
    else if(tdsrrc_rbr.status.failure_status == TDSRRCRB_PHY_CHAN_FAILURE)
    {

      /* Send the RB Setup Failure Message */
      tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                tdsrrc_rbr.status.prot_err_cause
                              );
      /* Then clear all local variables that may
      need to be cleared */
      tdsrrcrbr_clear_procedure();
      /* Make sure we're back in the initial sub-state */
      tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure status %d not handled",tdsrrc_rbr.status.failure_status);
    }

    break;

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbr_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Setup Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Release message */
    tdsrrcrbr_process_rbrelease_message( cmd_ptr );
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring tdsrrc_rbr event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }

}



/*===========================================================================

FUNCTION   RRCrbr_FACH_WFCUCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the tdsrrc_rbr procedure is in the TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate
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

static void tdsrrcrbr_fach_wfcucnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    if (tdsrrc_rbr.status.failure_status == TDSRRCRB_SIMULTANEOUS_RECONFIG)
    {
      /* Send the RB Release Failure Message */
      tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                tdsrrc_rbr.status.prot_err_cause
                              );
      /* Then clear all local variables that may
      need to be cleared */
      tdsrrcrbr_clear_procedure();

      /* Make sure we're back in the initial sub-state */
      tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
    }
    else if (tdsrrc_rbr.status.failure_status == TDSRRCRB_MSG_VALID)
    {
      /* Send the Radio Bearer Release Complete message */
      if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
          (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
      {
        tdsrrcrbr_send_rbr_complete_message(TRUE);
      }
      else
      {
        tdsrrcrbr_send_rbr_complete_message(FALSE);
      }
      tdsrrcllc_print_trans_chnl_rate();
      /* if next state is Cell_PCH or URA_PCH then we have to 
         treat the procedure as successful only after getting L2ack
         for the response message */
      if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
          (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
        
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
        
        /* Change the substate to the next substate - this is
           where we wait for L2 ack from RLC for sending the RBR
           complete message. */
        tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
      }
      else
      {
        if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
        {
          if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
          {      
            (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
          }
          else
          {
            (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
          }
          tdsrrc_rbr.need_to_release_cn_domain = FALSE;
        }
        
        /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
           configurations */
        tdsrrcllc_clear_ordered_config();  

        /* send the nas cn info if stored */
        if (tdsrrc_rbr.cn_info_saved != FALSE)
        {
          /* Put the command on MM queue */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RRC_SERVICE_IND_FROM_OTA_MSG to MM");
          if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
          {
             tdscfa_mm_put_cmd(tdsrrc_rbr.mm_cmd_ptr);
          }
          mm_put_cmd(tdsrrc_rbr.mm_cmd_ptr);
          /* clear the flag */
          tdsrrc_rbr.cn_info_saved = FALSE;
        }

        /* Go to the next sub-state in the state machine. That's
           the initial substate since we've sent the RBR complete
           message */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* At this point we don't need to remember any of the
           Radio Bearer information since it's stored in ESTABLISHED_RABS.
           Clear our internal variables */
        tdsrrcrbr_clear_procedure();
      }
    }
    /*  In the case of Phy_Chan_Failure, RB setup failure must be send
    and oc should be reset. */
    else if(tdsrrc_rbr.status.failure_status == TDSRRCRB_PHY_CHAN_FAILURE)
    {

      /* Send the RB Setup Failure Message */
      tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                tdsrrc_rbr.status.prot_err_cause
                              );
      /* Then clear all local variables that may
      need to be cleared */
      tdsrrcrbr_clear_procedure();
      /* Make sure we're back in the initial sub-state */
      tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure status %d not handled",tdsrrc_rbr.status.failure_status);
    }

    break;

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbr_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Setup Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Release message */
    tdsrrcrbr_process_rbrelease_message( cmd_ptr );
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring tdsrrc_rbr event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }

}

/*===========================================================================

FUNCTION   RRCRBR_DCH_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBR procedure is in the TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate
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
static void tdsrrcrbr_dch_wfchcnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbr_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Release Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Release message */
    tdsrrcrbr_process_rbrelease_message( cmd_ptr );
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }
}


/*===========================================================================

FUNCTION   RRCRBR_FACH_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBR procedure is in the TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate
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
static void tdsrrcrbr_fach_wfchcnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbr_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Release Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Release message */
    tdsrrcrbr_process_rbrelease_message( cmd_ptr );
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }
}



/*===========================================================================

FUNCTION   RRCRBR_OTHER_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBR procedure is in the TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate
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
static void tdsrrcrbr_other_wfchcnf_substate_event_handler
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
    /* In this state this procedure should no longer be active. */

    /* Then change to initial substate */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbr_clear_procedure();
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring downlink sdu since UE not in DCH or FACH");
    /* In this state this procedure should no longer be active. */

    /* Then change to initial substate */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbr_clear_procedure();
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }
}



/*===========================================================================

FUNCTION   RRCRBR_DCH_WFL2ACK_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBR procedure is in the TDSRRCRB_WAIT_FOR_L2_ACK substate
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
static void tdsrrcrbr_dch_wfl2ack_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_DOWNLINK_L2ACK_IND:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbr_process_l2_ack_ind( &(cmd_ptr->cmd.downlink_l2ack_ind) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Release Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Release message */
    tdsrrcrbr_process_rbrelease_message( cmd_ptr );
    break;


  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    tdsrrc_rbr.status.failure_status = TDSRRCRB_CELL_UPDATE_OCCURED;
    /* Send the RBR Reconfig Failure Message */
    tdsrrcrbr_send_rbr_failure_message(tdsrrc_rbr.status.failure_status,
                              tdsrrc_rbr.status.prot_err_cause
                             );
    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbr_clear_procedure();

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }
}


/*===========================================================================

FUNCTION   RRCRBR_FACH_WFL2ACK_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBR procedure is in the TDSRRCRB_WAIT_FOR_L2_ACK substate
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
static void tdsrrcrbr_fach_wfl2ack_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_DOWNLINK_L2ACK_IND:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbr_process_l2_ack_ind( &(cmd_ptr->cmd.downlink_l2ack_ind) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Release Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Release message */
    tdsrrcrbr_process_rbrelease_message( cmd_ptr );
    break;

  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    tdsrrc_rbr.status.failure_status = TDSRRCRB_CELL_UPDATE_OCCURED;
    /* Send the RBR Reconfig Failure Message */
    tdsrrcrbr_send_rbr_failure_message(tdsrrc_rbr.status.failure_status,
                              tdsrrc_rbr.status.prot_err_cause
                             );
    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbr_clear_procedure();

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }
}



/*===========================================================================

FUNCTION   RRCRBR_OTHER_WFL2ACK_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBR procedure is in the TDSRRCRB_WAIT_FOR_L2_ACK substate
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
static void tdsrrcrbr_other_wfl2ack_substate_event_handler
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
    /* In this state this procedure should no longer be active. */

    /* Then change to initial substate */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbr_clear_procedure();
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC.*/
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring downlink SDU since UE not in DCH or FACH");
    /* In this state this procedure should no longer be active. */

    /* Then change to initial substate */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbr_clear_procedure();
    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }
}




/*===========================================================================

FUNCTION   RRCrbr_FACH_WFCSCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the tdsrrc_rbr procedure is in the TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF substate
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

static void tdsrrcrbr_fach_wfcscnf_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
   /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

    case TDSRRC_STATE_CHANGE_IND:
     /* Indication that RRC's state has changed */
      tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
    case TDSRRC_NEW_CELL_IND:
      tdsrrcrbr_process_new_cell_ind( &(cmd_ptr->cmd.new_cell_ind) );
      break;

    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      if(tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH)
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
          tdsrrcrbr_send_channel_config_req();
          /* Also reset the tdsrrcrbr_send_initiate_cu_flag to false as cell update
           * will not be needed since CU and CUCNF for oos took care 
           * of that
           */
          tdsrrcrbr_send_initiate_cu_flag = FALSE;
          /* Change the substate since we're waiting for the
          confirmation from LLC for the channel configs */
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
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
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
      break;
   }


}


/*===========================================================================

FUNCTION   RRCRBR_FACH_WFCRNTIUPDATECNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBR procedure is in the TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF substate
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

static void tdsrrcrbr_fach_wfcrntiupdatecnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_STATE_CHANGE_IND:
   /* Indication that RRC's state has changed */
    tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
   
    break;


  case TDSRRC_RNTI_UPDATE_CNF:
    if (cmd_ptr->cmd.rnti_update_cnf.status == SUCCESS) 
    {
      tdsrrcrbr_process_rnti_update_cnf();
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RNTI update failed.  Send RBR failure");
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_VALID;
      
      tdsrrcrbr_send_rbr_failure_message(tdsrrc_rbr.status.failure_status,
                               tdsrrc_rbr.status.prot_err_cause);
      
      /* Then clear all local variables that may
         need to be cleared */
      tdsrrcrbr_clear_procedure();

      /* Make sure we're back in the initial sub-state */
      tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
    }
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }

}

/*===========================================================================

FUNCTION   RRCRBR_PCH_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBR procedure is in the TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate
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
static void tdsrrcrbr_pch_wfchcnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{


  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbr_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:
  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }
}

/*===========================================================================

FUNCTION   RRCRBR_PCH_WFCUCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBR procedure is in the TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate
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
static void tdsrrcrbr_pch_wfcucnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    tdsrrcrbr_process_successful_proc_completion(FALSE);
    break;

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbr_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:
    /* Indication that RRC's state has changed */
    tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

 

    case TDSRRC_DOWNLINK_SDU_IND:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unexpected DOWNLINK_SDU_IND in substate %d",tdsrrc_rbr.curr_substate);
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }

}

/*===========================================================================

FUNCTION   RRCRBR_PCH_WF_CSCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBR procedure is in the TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF substate
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

static void tdsrrcrbr_pch_wfcscnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_STATE_CHANGE_IND:
   /* Indication that RRC's state has changed */
    tdsrrcrbr_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
   
    break;


  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBR event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }

}


/*===========================================================================

FUNCTION   RRCRBR_CELL_DCH_EVENT_HANDLER

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
static void tdsrrcrbr_cell_dch_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  if ((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_L2ACK_IND) &&
      (tdsrrc_rbr.curr_substate != TDSRRCRB_WAIT_FOR_L2_ACK))
  {
    /* We could get L2ack for a previously sent message while we are already
    processing the next message. So l2ack could be received any any of the following
    substates. If RBR substate does not indicate that we are not waiting for l2ack
    then just print the status */
    if (cmd_ptr->cmd.downlink_l2ack_ind.status == SUCCESS)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Recd L2ack for MUI:%d, Lc_Id:%d, rbr_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui, 
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_rbr.curr_substate);
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2ack failed for MUI:%d, Lc_Id:%d, rbr_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui, 
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_rbr.curr_substate);
    }
    /* return from here so that procedure can continue with existing process */
    return;
  }
  /* Switch on the current substate */
  switch ( tdsrrc_rbr.curr_substate )
  {
  case TDSRRCRB_INITIAL:

    /* Call the event handler for TDSRRCRB_INITIAL substate */
    tdsrrcrbr_initial_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:

    /* Call the event handler for TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate */
    tdsrrcrbr_dch_wfchcnf_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_L2_ACK:

    /* Call the event handler for TDSRRCRB_WAIT_FOR_L2_ACK substate */
    tdsrrcrbr_dch_wfl2ack_substate_event_handler( cmd_ptr );
    break;

    case TDSRRCRB_WAIT_FOR_REESTABLISH_SRB2:
      tdsrrcrbr_dch_wfsrb2reestablish_substate_event_handler(cmd_ptr);
      break;

    case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
      /* Call the event handler for TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate */
      tdsrrcrbr_dch_wfcucnf_event_handler( cmd_ptr);
      break;

  default:

    /* No other substates processed for this RRC state    */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Command %x not processed in CELL_DCH for substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;

  }
}



/*===========================================================================

FUNCTION   RRCRBR_CELL_FACH_EVENT_HANDLER

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
static void tdsrrcrbr_cell_fach_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  if ((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_L2ACK_IND) &&
      (tdsrrc_rbr.curr_substate != TDSRRCRB_WAIT_FOR_L2_ACK))
  {
    /* We could get L2ack for a previously sent message while we are already
    processing the next message. So l2ack could be received any any of the following
    substates. If RBR substate does not indicate that we are not waiting for l2ack
    then just print the status */
    if (cmd_ptr->cmd.downlink_l2ack_ind.status == SUCCESS)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Recd L2ack for MUI:%d, Lc_Id:%d, rbr_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui, 
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_rbr.curr_substate);
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2ack failed for MUI:%d, Lc_Id:%d, rbr_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui, 
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_rbr.curr_substate);
    }
    /* return from here so that procedure can continue with existing process */
    return;
  }
  /* Switch on the current substate */
  switch ( tdsrrc_rbr.curr_substate )
  {
  case TDSRRCRB_INITIAL:
    /* Call the event handler for TDSRRCRB_INITIAL substate */
    /* NOTE: Here we need to check if a cell-update procedure
    has been initialized or not. For now, just call the event handler */
    tdsrrcrbr_initial_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
    /* Call the event handler for TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate */
    tdsrrcrbr_fach_wfcucnf_event_handler( cmd_ptr);
    break;

  case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:
    /* Call the event handler for TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate */
    tdsrrcrbr_fach_wfchcnf_substate_event_handler( cmd_ptr );
    break;

  case  TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF:
    /* Call the event handler for TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate */
    tdsrrcrbr_fach_wfcscnf_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF:
    /* Call the event handler for TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF substate */
    tdsrrcrbr_fach_wfcrntiupdatecnf_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_L2_ACK:
    /* Call the event handler for TDSRRCRB_WAIT_FOR_L2_ACK substate */
    tdsrrcrbr_fach_wfl2ack_substate_event_handler( cmd_ptr );
    break;

  default:
    /* No other substates processed for this RRC state    */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Command %x not processed in CELL_FACH for substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
    break;
  }
}

/*===========================================================================

FUNCTION   RRCRBR_PCH_EVENT_HANDLER

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
static void tdsrrcrbr_pch_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the current substate */
  switch ( tdsrrc_rbr.curr_substate )
  {
  case TDSRRCRB_INITIAL:

    /* Call the event handler for TDSRRCRB_INITIAL substate */
    tdsrrcrbr_initial_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
    /* Call the event handler for TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate */
    tdsrrcrbr_pch_wfcucnf_event_handler( cmd_ptr);
    break;

  case TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF:

    tdsrrcrbr_pch_wfcscnf_event_handler( cmd_ptr);
    break;

  case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:

    /* Call the event handler for TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate */
    tdsrrcrbr_pch_wfchcnf_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_L2_ACK:

    /* The only way we could be in pch and rb state is "wait_for_l2_ack is if
       oos happened while waiting for l2 ack and cell update confirm takes UE to pch
       In this case, send failure msg of tdsrrc_rbrc with cause cell update occured 
    */
    tdsrrc_rbr.status.failure_status = TDSRRCRB_CELL_UPDATE_OCCURED;

    tdsrrcrbr_send_rbr_failure_message(tdsrrc_rbr.status.failure_status,
                              tdsrrc_rbr.status.prot_err_cause);
    /* Then clear all local variables that may
       need to be cleared */
    tdsrrcrbr_clear_procedure();

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
    break;

  default:

    /* No other substates processed for this RRC state    */
    TDSRRC_MSG3(MSG_LEGACY_MED,"Command %x not processed in substate%d rrc_state%d", 
            cmd_ptr->cmd_hdr.cmd_id, tdsrrc_rbr.curr_substate, tdsrrc_get_state());
    break;

  }
}




/*===========================================================================

FUNCTION   RRCRBR_OTHER_STATE_EVENT_HANDLER

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
static void tdsrrcrbr_other_state_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the current substate */
  switch ( tdsrrc_rbr.curr_substate )
  {
  
    case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:
      /* Call the event handler for WAIT_FOR_CHAN_CFG_CNF substate */
      tdsrrcrbr_other_wfchcnf_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCRB_WAIT_FOR_L2_ACK:
      /* Call the event handler for WAIT_FOR_L2_ACK substate */
      tdsrrcrbr_other_wfl2ack_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
      tdsrrcrbr_other_wfcucnf_substate_event_handler( cmd_ptr);
      break;

    case  TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF:
      /* Call the event handler for TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate */
      tdsrrcrbr_other_wfcscnf_substate_event_handler( cmd_ptr );
      break;

    default:
      /* No other substates processed for this RRC state    */
      TDSRRC_MSG2(MSG_LEGACY_MED,"Command %x not processed in IDLE or PCH states for substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbr.curr_substate);
      break;
  }
}


/*===========================================================================

FUNCTION RRCRBR_PROCEDURE_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  Radio Bearer Release procedure. This function is called by the
  dispatcher whenever an event is received for this module. This function
  checks the current RRC state and calls the event handler for
  that state. 
  
  Note that the Radio Bearer Release procedure can be initiated
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
void tdsrrcrbr_procedure_event_handler
(
tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  /* Get the current RRC State */
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  switch (rrc_state)
  {
  
  case TDSRRC_STATE_CELL_FACH:    
    /* If it's CELL_FACH state, call the event handler
    for CELL_FACH state */
    tdsrrcrbr_cell_fach_event_handler(cmd_ptr);
    break;

  case TDSRRC_STATE_CELL_DCH:    
    /* If it's CELL_DCH state, call the event handler
    for CELL_DCH state */
    tdsrrcrbr_cell_dch_event_handler(cmd_ptr);
    break;

    case TDSRRC_STATE_URA_PCH:
    case TDSRRC_STATE_CELL_PCH:
      /* If it's CELL_DCH state, call the event handler
      for CELL_PCH state */
      tdsrrcrbr_pch_event_handler(cmd_ptr);
      break;

  case TDSRRC_STATE_CONNECTING:   
  case TDSRRC_STATE_DISCONNECTED: 
    /* If it's any other state call the event handler
    for other states. The processing of events in
    other RRC states is identical for this procedure */
    tdsrrcrbr_other_state_event_handler(cmd_ptr);
    break;

  default:  
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC State: %d",rrc_state);
    break;
  }  
}/* end function tdsrrcrbr_procedure_event_handler */



/*===========================================================================

FUNCTION  RRCRBR_INIT_PROCEDURE

DESCRIPTION

  This function initializes the RBR procedure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbr_init_procedure( void )
{
  uint32 count;
  tdsrrcrbr_send_initiate_cu_flag = FALSE;


  /* Initialize the transaction id's for this procedure.
  Use a number that can't be an actual received transaction
  id. */
  tdsrrc_rbr.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  tdsrrc_rbr.tr_ids.rejected_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;

  tdsrrc_rbr.rbr_for_srns_relocation = FALSE;
  tdsrrc_rbr.new_urnti_valid = FALSE;

  tdsrrc_rbr.hho_ciph_status = FALSE;

  tdsrrc_rbr.trans_to_dch_ciph_config_needed = FALSE;
  /* Initialize the current substate */
  tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

  /* set cn_info_saved to false */
  tdsrrc_rbr.cn_info_saved = FALSE;

  /* Initialize CRNTI valid to FALSE */
  tdsrrc_rbr.new_crnti_valid    = FALSE;
  tdsrrc_rbr.directed_cell_info = FALSE;

  /* Initialize the RABS per message structure.
  For Radio Bearer Relase, these structures are not
  expected to be used unless there are RABs to
  be reconfigured given by the message */
  for ( count=0; count < TDSRRCRB_MAX_RAB_PER_MSG; count++ )
  {
    tdsrrc_rbr.rabs.rabs_in_msg[count].rab_type = TDSRRCRB_UNKNOWN_RAB;    
    tdsrrc_rbr.rabs.rabs_in_msg[count].num_rbs_for_rab = 0;
    tdsrrc_rbr.rabs.chan_config_needed[count] = FALSE;
    tdsrrc_rbr.rabs.rabs_in_msg[count].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_NONE;

  }
  tdsrrc_rbr.rabs.num_rabs_in_msg = 0;

  /* Indicate that there are no signalling connections
  to be released. */
  tdsrrc_rbr.need_to_release_cn_domain = FALSE;

  /* Indicate that there are no RBs to be released/reconfigured/affected. This
  is the default value. */
  tdsrrc_rbr.rbs_tobe_released.is_valid = FALSE;
  tdsrrc_rbr.rbs_tobe_reconfigured.is_valid = FALSE;
  tdsrrc_rbr.rbs_tobe_affected.is_valid = FALSE;

  tdsrrc_rbr.high_mobility_ind = FALSE;


  /* Register for state change notification -
  We want to be notified when the state changes
  from CELL_DCH to CELL_FACH and from any state
  to disconnected state. */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBR,  /* Procedure name */
                             TDSRRC_STATE_CELL_DCH, /* From State     */
                             TDSRRC_STATE_CELL_FACH /* To State       */
                           );

  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBR,  /* Procedure name */
                             TDSRRC_STATE_WILDCARD, /* From State     */
                             TDSRRC_STATE_DISCONNECTED /* To State    */
                           );
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBR,  /* Procedure name */
                             TDSRRC_STATE_CELL_DCH, /* From State     */
                             TDSRRC_STATE_CELL_PCH /* To State       */
                           );
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBR,  /* Procedure name */
                             TDSRRC_STATE_CELL_DCH, /* From State     */
                             TDSRRC_STATE_URA_PCH /* To State       */
                           );

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU

  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBR,  /* Procedure name */
                             TDSRRC_STATE_CELL_FACH, /* From State     */
                             TDSRRC_STATE_CELL_PCH /* To State       */
                           );

  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBR,  /* Procedure name */
                             TDSRRC_STATE_CELL_FACH, /* From State     */
                             TDSRRC_STATE_URA_PCH /* To State       */
                           );
  tdsrrc_rbr.initiate_cell_selection_trans_from_fach = FALSE;     
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

  /* Define a timer used to delay 5 seconds before
  sending the RB Release Complete message */
  rex_def_timer(&tdsrrcrbr_temp_timer, rex_self(), TDSRRCRBR_TEMP_TIMER_SIG);
}

/*===========================================================================

FUNCTION   RRCRBR_PROCESS_RBRELEASE_R4

DESCRIPTION

  This function processes the received Rel 4 RB Release Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Release Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to release the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Release Failure message
    will be sent to UTRAN.

    The function is copy from tdsrrcrbr_process_rbrelease_r5
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbr_process_rbrelease_r4
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_RB_InformationReleaseList  *rb_info_rel_list_ptr;
  tdsrrc_RB_InformationAffectedList *rb_info_affected_list_ptr;
  tdsrrc_RadioBearerRelease *msg_ptr;  /* Pointer to the RB Release message        */
  tdsrrcrb_directed_cell_info_type directed_cell_info; /* directed cell info struct */

  tdsrrcllc_oc_status_e_type oc_status;/* To hold tdsordered_config status          */
  boolean ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                    occured while setting tdsordered_config. */
  boolean rb_rel_srch = FALSE; 
  boolean rab_found = FALSE; 
  tdsrrc_state_e_type current_state;       /* current RRC State  */
  uint32 rab_count, rb_count, localrb_count;
  /* Local C_RNTI value */
  tdsrrc_int_c_rnti_type              c_rnti;
  tdsrrc_int_u_rnti_type              u_rnti;
  tdsrrcrb_rnti_update_info_type      rnti_update_info;   /* to hold rnti update info */
  uint32 local_idx=0;
  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL4;


  tdsrrcsmc_srns_ip_check_status_e_type    ip_check_status = TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;


  tdsrrc_DL_DCCH_Message *dcch_msg_ptr ;     /* Pointer to the RB Release message */

  /* Store parameter IEs */
  tdsrrc_RadioBearerRelease_r4_IEs* rb_release_ptr = NULL;


   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  boolean primary_plmn_Id_present = FALSE;
   /*Variable to hold primary plmn i.d if present in OTA msg*/
  tdsrrc_PLMN_Identity primary_plmn_Identity;
  tdsrrcrb_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;


  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Release message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerRelease);

  if (msg_ptr->u.later_than_r3.criticalExtensions.t == 
       T_tdsrrc_RadioBearerRelease_criticalExtensions_6_r4)
  {
    /* Store Msg Ptr in a local variable */
    rb_release_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerRelease_r4;

    TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:rbreleaser4 rcvd");
  } 
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported RB Release t");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;

    /*Check whether Cell Update is going on */
    if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
         (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
      if (SUCCESS != tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBR failed to register with CU");
      }
      /* Wait until Cell update procedure is completed */
      tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
    }
    else
    {
      tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                      tdsrrc_rbr.status.prot_err_cause );
      /*continue the same substate */ 
    }
    return;
  }

  if (TRUE == tdsrrcrbr_validate_srns_relocation_info(msg_ptr, msg_version))
  {
    ip_check_status = tdsrrcsmc_check_integrity_for_srns_relocation(cmd_ptr, msg_version);
  
    switch (ip_check_status)
    {
      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS:
        /*continue further processing with the message */
        break;

      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE:
        /*Discard the message and continue with ongoing procedure as if this
        message was not received */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IP check failed. Discarding RBS msg");
        break;

      case TDSRRCSMC_SRNS_SECURITY_INFO_INVALID:
      case TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION:
        if (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier
                       == tdsrrc_rbr.tr_ids.rejected_transaction)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid security config-rejected tr-id%d match!",tdsrrc_rbr.tr_ids.rejected_transaction);
          /* Nothing else to do, just ignore the message and return */
        } 
        else
        {
          /* send a reconfig failure messge with invalid
          configuration and 
          continue the same substate with any ongoing procedure */
          temp_failure_cause.failure_status = tdsrrc_rbr.status.failure_status ;
          temp_rejected_transaction= tdsrrc_rbr.tr_ids.rejected_transaction;
          
          tdsrrc_rbr.tr_ids.rejected_transaction = 
            (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
          if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
          {
            tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          }
          else
          {
            tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
          }
          /*Check whether Cell Update is going on */
          if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
               (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
          {
            if(tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbr.curr_substate,tdsrrc_rbr.status.failure_status);
              tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                       tdsrrc_rbr.status.prot_err_cause );
              
              tdsrrc_rbr.status.failure_status = temp_failure_cause.failure_status;

              tdsrrc_rbr.tr_ids.rejected_transaction = temp_rejected_transaction;
            }
            else
            {
		      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
              (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
              /* Wait until Cell update procedure is completed */
              tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
            }

          }
          else
          {
            tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                            tdsrrc_rbr.status.prot_err_cause );
          }
        }
        /*do not clear the procedure or change the tdsrrc_rbe substate because
        some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
        break;

      default:
        /*should not come here */
        break;
    }
    if (ip_check_status != TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS)
    {
      /*do not clear the procedure or change the tdsrrc_rbr substate because
      some other tdsrrc_rbe procedure might be in progress. Just return from
      here */
      return;
    }
  }


  if ((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
      (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
      (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incomatible simultaneous reconfig! Rejecting RBR message");
     if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
            == tdsrrc_rbr.tr_ids.accepted_transaction )
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate RBR message - accepted tr-id matched!");
       /* Nothing else to do, just return */
     } 
     else if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                    == tdsrrc_rbr.tr_ids.rejected_transaction )
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate RBR message - rejected tr-id matched!");
       /* Nothing else to do, just return */
     } 
     else
     {
       /* We have received a RBR message with a new transaction id.
       This message should be in our rejected transaction
       list */
       temp_failure_cause.failure_status = tdsrrc_rbr.status.failure_status ;
       temp_rejected_transaction= tdsrrc_rbr.tr_ids.rejected_transaction;
       
       tdsrrc_rbr.tr_ids.rejected_transaction = 
         (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
       tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;

       /*Check whether Cell Update is going on */
       if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
            (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
       {
         if(tdsrrc_rbr.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
         {
           TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbrc subsstate(%d) Send failure with %d cause",tdsrrc_rbr.curr_substate,tdsrrc_rbr.status.failure_status);
           tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                     tdsrrc_rbr.status.prot_err_cause );
           
           tdsrrc_rbr.status.failure_status = temp_failure_cause.failure_status;
           tdsrrc_rbr.tr_ids.rejected_transaction = temp_rejected_transaction;
         }
         else
         {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
           (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBR);
           /* Wait until Cell update procedure is completed */
           tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
         }

       }
       else
       {
         tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                         tdsrrc_rbr.status.prot_err_cause );
         /*continue the same substate */ 
       }
     }
     return;
  }

  /* We have received a RB release message with a new transaction id.
  Accept the transaction Id */
  tdsrrc_rbr.tr_ids.accepted_transaction =
     msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;


#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rb_release_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rb_release_ptr->m.activationTimePresent,
         rb_release_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBR SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbr_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rb_release_ptr->m.activationTimePresent = 0;
    }
  }

#endif


  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  tdsrrcrbr_validate_rbrelease_r4(msg_ptr);

  if ( tdsrrc_rbr.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBR Message validation failed");

    /* Send the RB Release Failure Message */
    tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                              tdsrrc_rbr.status.prot_err_cause
                            );

    tdsrrcrbr_clear_procedure();
    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
  }
  /* If the message is valid, proceed further */
  else
  {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    boolean                  additional_srb;
    uecomdef_logchan_e_type  dl_sdu_type = UE_LOGCHAN_NONE;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing RBR Message");

    tdsrrcrbr_process_high_mobility_ind_in_non_critical_ext(msg_ptr,TDSMSG_REL4);
    
    /*store the rb ids tobe released */
    tdsrrc_rbr.rbs_tobe_released.is_valid = TRUE;
    rb_info_rel_list_ptr = &rb_release_ptr->rb_InformationReleaseList;
    localrb_count = 0;
	   local_idx =0;
    while ( rb_info_rel_list_ptr->n  > local_idx)
    {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      additional_srb = FALSE;

      if((tdsrrclcm_get_dl_lc_type(tdsrrclcm_get_dl_rlc_lc_id(rb_info_rel_list_ptr->elem[local_idx]), &dl_sdu_type)) == TDSRRCLCM_SUCCESS)
      {
        if(dl_sdu_type == UE_LOGCHAN_TM_DCCH_DL)
        {
          additional_srb = TRUE;
        }
      }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

      /*check whether rb id is signalling rbid or does not exist in established
      rabs */
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      if (additional_srb == FALSE)
      { 
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
        if((rb_info_rel_list_ptr->elem[local_idx] < DCCH_DT_LOW_PRI_RADIO_BEARER_ID))
        {
          /* RB Message was invalid. */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RB id:%d in RBR Message",rb_info_rel_list_ptr->elem[local_idx]);
          /* Send the RB Release Failure Message */
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                    tdsrrc_rbr.status.prot_err_cause
                                  );

          tdsrrcrbr_clear_procedure();
          /* Make sure we're back in the initial sub-state */
          tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
          return;

        }
        else if( rb_info_rel_list_ptr->elem[local_idx] == DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
        {
          if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND != tdsrrclcm_check_dl_rlc_lc_id(
                                           UE_LOGCHAN_DCCH,
                                           DCCH_DT_LOW_PRI_RADIO_BEARER_ID,
                                           UE_MODE_ACKNOWLEDGED_DATA))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Release SRB4");
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignore Non-exitent rb_id = %d in tdsrrc_rbr msg",rb_info_rel_list_ptr->elem[local_idx]);
            local_idx++;
            continue;
          }
        }
        else if(!tdsrrc_find_rb_in_est_rabs(rb_info_rel_list_ptr->elem[local_idx]))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignore Non-exitent rb_id in tdsrrc_rbr msg",rb_info_rel_list_ptr->elem[local_idx]);
          local_idx++;
          continue;
        }
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

      tdsrrc_rbr.rbs_tobe_released.rb_id[localrb_count] = rb_info_rel_list_ptr->elem[local_idx];
      localrb_count++;
      local_idx++;
    }
    tdsrrc_rbr.rbs_tobe_released.num_rbs = localrb_count;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Number ofRBs tobe released:%d",localrb_count);

    /*store the rb ids tobe affected */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
          tdsrrc_RadioBearerRelease_r4_IEs,rb_InformationAffectedList))
    {
      tdsrrc_rbr.rbs_tobe_affected.is_valid = TRUE;
      rb_info_affected_list_ptr = 
      &rb_release_ptr->rb_InformationAffectedList;
      localrb_count = 0;
      while ( rb_info_affected_list_ptr->n > localrb_count )
      {
        /*check whether rb id is signalling rbid or does not exist in established
        rabs */
        if(( rb_info_affected_list_ptr->elem[localrb_count].rb_Identity > DCCH_DT_LOW_PRI_RADIO_BEARER_ID)&&
           (! tdsrrc_find_rb_in_est_rabs(rb_info_affected_list_ptr->elem[localrb_count].rb_Identity)))
        {
          /* RB Message was invalid. */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RB id:%d in RBR Message",rb_info_affected_list_ptr->elem[localrb_count].rb_Identity);
          /* Send the RB Release Failure Message */
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                    tdsrrc_rbr.status.prot_err_cause
                                  );

          tdsrrcrbr_clear_procedure();
          /* Make sure we're back in the initial sub-state */
          tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
          return;

        }
        tdsrrc_rbr.rbs_tobe_affected.rb_id[localrb_count] = 
        rb_info_affected_list_ptr->elem[localrb_count].rb_Identity;
        localrb_count++;
      }
      tdsrrc_rbr.rbs_tobe_affected.num_rbs= localrb_count;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Number ofRBs tobe affected:%d",localrb_count);
    }


    /* If there is a signalling connection to be released,
    or if there are individual RBs to be released, we
    will be releasing some RBs. */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
        tdsrrc_RadioBearerRelease_r4_IEs,signallingConnectionRelIndication))
    {
      tdsrrcrbr_mark_cn_doamin_to_be_closed(rb_release_ptr->signallingConnectionRelIndication);
  
      for ( rab_count = 0; rab_count< MAX_RAB_TO_SETUP; rab_count++ )
      {
        if ( (tdsrrc_est_rabs.rabs[rab_count].cn_domain == tdsrrc_rbr.cn_to_release) &&
             (tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0) )
        {
          rab_found = TRUE;
          for ( rb_count=0; rb_count<tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab; rb_count++ )
          {
            rb_rel_srch = FALSE;
            for ( localrb_count=0; localrb_count<tdsrrc_rbr.rbs_tobe_released.num_rbs; localrb_count++ )
            {
              if ( tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id == 
                   tdsrrc_rbr.rbs_tobe_released.rb_id[localrb_count] )
              {
                rb_rel_srch = TRUE;
                break;
              }
            }
            if ( rb_rel_srch == FALSE ) // est rabs has an rb_id not in the rb release list
            {
              /* send failure message */
              tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
              tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                        tdsrrc_rbr.status.prot_err_cause);
              /* Go back to initial substate */
              tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
              /* Then clear all local variables that may
                          need to be cleared */
              tdsrrcrbr_clear_procedure();
              return;
            }
          }/* rb_count loop */       
        } /*end of if loop */
      }/* rab_count loop */
      /*if none of the rabs in the establishedrabs have rb information*/
      if ( rab_found==FALSE )
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB lookup in est_rabs for cn:%d failed",tdsrrc_rbr.cn_to_release );
        /* send failure message */
        tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause);
        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbr_clear_procedure();
        return;

      }
    }

    /* Check if there is CN information present. If so, we need
       send the information to the Cell Change manager */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r4_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
    
      /*Check to see if primary PLMN I.D is present in RBR message ext
       * tdsrrc_RadioBearerRelease-v690ext which is present in 
       * v690NonCriticalExtensions
       */
       primary_plmn_Id_present = tdsrrcrbr_check_and_get_primary_plmn_id(msg_ptr,
                                                                  &primary_plmn_Identity,TDSMSG_REL4);
    
      tdsrrc_rbr.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info(
                     &(rb_release_ptr->cn_InformationInfo) 
                     ,primary_plmn_Id_present
                     ,&primary_plmn_Identity
        );
      if (tdsrrc_rbr.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbr.cn_info_saved = TRUE;
      }
    }

    /* Indicate layer1 if rb release message is going to release voice RAB */
    tdsrrcrbr_indicate_layer1_if_config_msg_release_voice_rab();

    tdsrrcrb_update_mac_rab_status(TDSRRC_PROCEDURE_RBR, RRC_CS_DOMAIN_CN_ID,
                                &rb_release_ptr->rb_InformationReleaseList,
                                FALSE);

    /* Set TDSORDERED_CONFIG so that LLC can process the
    necessary parameters. */
    oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBR,
                                           tdsrrc_rbr.state_indicated_by_msg,
                                           tdsrrc_DL_DCCH_Message_PDU,
                                           (void *)dcch_msg_ptr
                                         );

    switch ( oc_status )
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
       /* Populate the local variables to indicate if Re-direction info is present */
        directed_cell_info.freq_info_present = FALSE;
        directed_cell_info.cpid_present = FALSE; 
        if((current_state==TDSRRC_STATE_CELL_FACH) && 
               ((tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
               ||(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
               ||(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
          if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
                  tdsrrc_RadioBearerRelease_r4_IEs,frequencyInfo)) &&
                 (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->frequencyInfo.modeSpecificInfo,
                 rrc_FrequencyInfo_modeSpecificInfo_tdd)))
          {
            directed_cell_info.freq_info_present = TRUE;
            directed_cell_info.uarfcn_dl  = rb_release_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
          }
         
          if (tdsrrcrbr_is_pccpch_info_present(msg_ptr, TDSMSG_REL4))
          {
            directed_cell_info.cpid_present = TRUE;
            directed_cell_info.cell_param_id = rb_release_ptr->dl_InformationPerRL_List.elem[0].
                modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          }      
          /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
                 FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
          if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
            (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
            if(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
            {
              tdsrrcrbr_send_initiate_cu_flag = TRUE;
              tdsrrcrbr_delete_crnti          = TRUE;
            }
            else
            {
              tdsrrc_rbr.directed_cell_info = TRUE;
            }
          }  
        }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
        if (
            (current_state==tdsrrc_rbr.state_indicated_by_msg) ||
            ((current_state==TDSRRC_STATE_CELL_FACH) && 
             (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH))
            )
        {
          /* CELL_DCH->CELL_DCH or CELL_FACH->CELL_FACH or 
          CELL_FACH->CELL_DCH case. We can release the
          Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
          /*check whether any frequency info is present in the message
          and transition is CELL_FACH->CELL_FACH. In this case
          initiate a cell update after channel config is successfully completed */
          if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
             (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
          {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
             /*call the function that evaluates the directed cell info and set the flags accordingly */
            if ((tdsrrcrbr_send_initiate_cu_flag == FALSE) &&
                (tdsrrcrb_validate_directed_cell_info (directed_cell_info)== FALSE))
            {
              if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
              tdsrrc_RadioBearerRelease_r4_IEs,new_C_RNTI))
              {
                tdsrrc_rbr.new_crnti_valid = TRUE;
                tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, 
                                    &c_rnti);
                tdsrrc_rbr.new_crnti = (uint16)c_rnti;
              }
              else
              {
                tdsrrc_rbr.new_crnti_valid = FALSE;
              }
   
              if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
              {
                if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
                {      
                  (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
                }
                else
                {
                  (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
                }
                tdsrrc_rbr.need_to_release_cn_domain = FALSE;
              }
              tdsrrcllcoc_update_toc_with_oc_from_fach();
              /*Initiate the cell selection*/
              tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR,
                                                       TDSRRC_TRANSITION_FROM_FACH,
                                                       tdsrrc_rbr.state_indicated_by_msg);

              tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

              /* exit from the switch case */
              break;
     
            }
     #else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
            if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
                tdsrrc_RadioBearerRelease_r4_IEs,frequencyInfo)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_release_ptr->frequencyInfo.modeSpecificInfo,
               rrc_FrequencyInfo_modeSpecificInfo_tdd)))
            {
              directed_cell_info.freq_info_present = TRUE;
              directed_cell_info.uarfcn_dl  = rb_release_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
            }
            else
            {
              directed_cell_info.freq_info_present = FALSE;
            }
            if (tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL4))
            {
              if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r4_IEs,dl_InformationPerRL_List)) &&
                (rb_release_ptr->dl_InformationPerRL_List.n != 0))
              {
                
                directed_cell_info.cpid_present = TRUE;
                directed_cell_info.cell_param_id = rb_release_ptr->dl_InformationPerRL_List.elem[0].
                  modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
              }
            }
            else
            {
              directed_cell_info.cpid_present = FALSE;
            }
            /*call the function that evaluates the directed cell info and set the flags accordingly */
            if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
            {
              /*directed cell info matched. Cell update is not required */
              tdsrrcrbr_send_initiate_cu_flag = FALSE;
            }
            else
            {
              /*directed cell info do not match. Cell update is required */
              tdsrrcrbr_send_initiate_cu_flag = TRUE;
              tdsrrcrbr_delete_crnti          = TRUE;
            }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
          } /* if CELL_FACH->CELL_FACH reconfiguration */

          /* Call the function that creates and sends a channel
            config request to LLC. */
          tdsrrcrbr_send_channel_config_req();

          /* Change the substate since we're waiting for the
             confirmation from LLC for the channel configs */
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
        {
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
             tdsrrc_RadioBearerRelease_r4_IEs,new_C_RNTI))
          {
            tdsrrc_rbr.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, 
                                &c_rnti);
            tdsrrc_rbr.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_rbr.new_crnti_valid = FALSE;
          }

          if (tdsrrc_rbr.need_to_release_cn_domain == TRUE)
          {
            if(tdsrrc_rbr.cn_to_release == RRC_CS_DOMAIN_CN_ID)
            {      
              (void)tdsrrcscr_release_signalling_connection(RRC_CS_DOMAIN_CN_ID);
            }
            else
            {
              (void)tdsrrcscr_release_signalling_connection(RRC_PS_DOMAIN_CN_ID);      
            }
            tdsrrc_rbr.need_to_release_cn_domain = FALSE;
          }

          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_RBR,
                                                  TDSRRC_TRANSITION_FROM_DCH,
                                                  tdsrrc_rbr.state_indicated_by_msg);
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

          /* Start T_305 [periodic cell update] timer */
          tdsrrccu_start_t_305_timer();

        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                 (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
        {
          /*store the directed cell info */
           tdsrrc_rbr.directed_cell_info = tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL4);

           /*Send the tdsrrc_rbr complete msg on old configuration */
           tdsrrcrbr_send_rbr_complete_message(TRUE);
           /*wait for L2ack */
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
           tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
           tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
        {
          /*Send the RBR complete msg on old configuration */
          tdsrrcrbr_send_rbr_complete_message(TRUE);
          /*wait for L2ack */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
          tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
          tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_FACH) && 
                ((tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
                 (tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if ((tdsrrc_rbr.directed_cell_info == FALSE)
            && (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
          {
            if(tdsrrc_rbr.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
            {
              tdsrrc_rbr.directed_cell_info = tdsrrcrbr_is_pccpch_info_present(msg_ptr,TDSMSG_REL4);
            }

            tdsrrc_rbr.initiate_cell_selection_trans_from_fach  = TRUE;
          }
    
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

          /* check whether CPICH info is present or not. Store this
          info  */
         // tdsrrc_rbr.directed_cell_info = rrcrbr_is_pcpich_info_present(msg_ptr);
          /*Send the RRCTCTC complete msg on old configuration */
          /* check whether new C-RNTI was present in the message. In that case
          we have to configure MAC with new C-RNTI before sending the response 
          message */
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
             tdsrrc_RadioBearerRelease_r4_IEs,new_C_RNTI))
          {
            tdsrrc_translate_crnti(&rb_release_ptr->new_C_RNTI, 
                                &c_rnti);
            /* Send a RNTI Update Request */
            rnti_update_info.crnti_action    = TDSRRC_RNTI_UPDATE;
            rnti_update_info.crnti           = (uint16)c_rnti;
            /* check whether URNTI is also present in the message */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
             tdsrrc_RadioBearerRelease_r4_IEs,new_U_RNTI))
            {
              tdsrrc_translate_urnti(&rb_release_ptr->new_U_RNTI, 
                                  &u_rnti);
              rnti_update_info.urnti_action    = TDSRRC_RNTI_UPDATE; 
              rnti_update_info.urnti           = u_rnti; 
            }
            else
            {
              rnti_update_info.urnti_action    = TDSRRC_RNTI_NOCHANGE; 
            }
            rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBR;
            rnti_update_info.rlc_re_establish_srb = FALSE;
            rnti_update_info.rlc_re_establish_rab = FALSE;
            rnti_update_info.cnf_reqd             = TRUE;
            tdsrrcrb_send_rnti_update_req(rnti_update_info);
            /* wait for the RNTI update confirm before sending the response
            message */
            tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
          }
          else 
          {
            tdsrrcrbr_send_rbr_complete_message(TRUE);
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBR Complete msg");
            /*wait for L2ack */
            tdsrrc_rbr.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
            tdsrrcllc_print_trans_chnl_rate();
          }
        }
        break;

      case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
        tdsrrc_rbr.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
        tdsrrc_rbr.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_rbr.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
        /* Send the RB Release Failure Message */
        tdsrrcrbr_send_rbr_failure_message( tdsrrc_rbr.status.failure_status,
                                  tdsrrc_rbr.status.prot_err_cause
                                );

        /* Go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_NOT_SET_OTHER:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
        /* When LLC sends this, there is a serious problem
        and we'll go back to disconnected state. No need
        to send the failure message here, we can just
        go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      default:

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
        /* Unknown error, just go back to initial substate */
        tdsrrc_rbr.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
    }/* end switch */

    if ( ordered_config_err_occured == TRUE )
    {
        /* No need to continue. Clear the procedure
        variables and return. */
        tdsrrcrbr_clear_procedure();
        return;
    }


    if(tdsrrc_rbr.rbr_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }

      
  }/* end if - valid rb message */
}
/*===========================================================================

FUNCTION   VALIDATE_RBRELEASE_R4

DESCRIPTION

  This function validates the received Rel 5 RB Release Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Release Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.

  The function is copy from tdsrrcrbr_validate_rbrelease_r5
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbr_validate_rbrelease_r4
(
tdsrrc_RadioBearerRelease *msg_ptr      /* Pointer to the RB Release message */
)
{
  tdsrrc_RadioBearerRelease_r4_IEs* rb_release_ptr = NULL;
  
  /* Assume the message is valid for now */
  tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_VALID;
  
  
  rb_release_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerRelease_r4;
  
  
  /* Check the RRC State indicatior given by the message */
  switch ( rb_release_ptr->rrc_StateIndicator )
  {
  case tdsrrc_RRC_StateIndicator_cell_DCH:
    tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
    break;
    
  case tdsrrc_RRC_StateIndicator_cell_FACH:
    tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
    break;
  case tdsrrc_RRC_StateIndicator_cell_PCH:
    tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
    break;
    
  case tdsrrc_RRC_StateIndicator_ura_PCH:
    tdsrrc_rbr.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
    break;
    
  default:
    /* No other states are supported right now. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported state in RB Release Message");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    break;
  }
  
  if ( tdsrrc_rbr.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
    return;
  }

  /* Check if there are new RABs to be reconfigured
  This is not supported now. */
 
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r4_IEs,rab_InformationReconfigList))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RAB Info to reconfigure present in RBR Message");
    if(tdsrrcrbr_update_rab_info_reconfiglist(&rb_release_ptr->rab_InformationReconfigList)
       == FAILURE)
    {
      return;
    }
  }
  else
  {
    tdsrrc_rbr.rabs.num_rabs_in_msg =0;
    /* Set the Ordered codec mode with the current active codec */
    tdsrrc_set_ordered_codec_mode(tdsrrcdata_codec_info.rrc_active_codec);
  }
  if ( rb_release_ptr->rb_InformationReleaseList.n == 0 )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBInfoReleaselist is absent in RBR Message");
    tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
    tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
    return;
  }

  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, tdsrrc_RadioBearerRelease_r4_IEs,rb_InformationAffectedList) )
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RB Info Affected list present in RBR Message");
  }


  
  /* Check if RB with PDCP Information list is present. This
  is not supported in the first release. This may be supported
  later */
  
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
    tdsrrc_RadioBearerRelease_r4_IEs,dl_CounterSynchronisationInfo))
#else
    if (tdsrrcrbr_validate_srns_relocation_info(msg_ptr, TDSMSG_REL4))
#endif

  {
    /*check whether the SRNS relocation is within Cell_DCH or not.
    SRNS relocation information can come in reconfiguration messages
    only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
      ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbr.state_indicated_by_msg);
      tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has 
    PDCP info list. If it is true then this configuration is unsupported */
    /* Copy handle from R3 begin */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rb_release_ptr->dl_CounterSynchronisationInfo,
          rB_WithPDCP_InfoList))
    /* Copy handle from R3 end */
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
      tdsrrc_rbr.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
    /* The procedure involves Lossy SRNS relocation. 
    Set the flag that indicates that this procedure
      involves SRNS relocation */
      tdsrrc_rbr.rbr_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
        tdsrrc_RadioBearerRelease_r4_IEs,new_U_RNTI))
      {
        tdsrrc_rbr.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(rb_release_ptr->new_U_RNTI),
          &tdsrrc_rbr.new_urnti);
      }
    }

    
  } /* dl counter sync info */

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  if((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,
        tdsrrc_RadioBearerRelease_r4_IEs,dl_CommonInformation)) && 
       (TDSRRC_MSG_COMMON_BITMASK_IE(rb_release_ptr->dl_CommonInformation,
        tdsrrc_DL_CommonInformation_r4,dl_DPCH_InfoCommon)))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      /* mac_d_HFN absent in R3 */
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");
        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             ( TDSRRC_MSG_COMMON_BITMASK_IE(rb_release_ptr->dl_CommonInformation.dl_DPCH_InfoCommon,
             tdsrrc_DL_DPCH_InfoCommon_r4,mac_d_HFN_initial_value)),
             &rb_release_ptr->dl_CommonInformation.dl_DPCH_InfoCommon.mac_d_HFN_initial_value,
             &mac_d_hfn_l);
        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Process Ciphering Config for HHO failed");
        }
        tdsrrc_rbr.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }
  

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
    (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
    /* If UTRAN DRX coefficient is not present then it is invalid configuration */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
      tdsrrc_RadioBearerRelease_r4_IEs,utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_rbr.state_indicated_by_msg);
      tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    if (tdsrrc_rbr.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      /*should we do it after getting l2ack for response??*/
      if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr, 
        tdsrrc_RadioBearerRelease_r4_IEs,ura_Identity)))
        
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
        tdsrrccu_clear_ura_id ();
      }
      else
      {
        /*URA ID present in the message set ura_identity variable with this value */
        if (rb_release_ptr->ura_Identity.numbits == 16)
        {
          tdsrrccu_update_ura_id (&(rb_release_ptr->ura_Identity));
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rb_release_ptr->ura_Identity.numbits);
          tdsrrc_rbr.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
          tdsrrc_rbr.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
          return;
        }
        
      }
      
    }
  }


}/* end function tdsrrcrbr_validate_rbrelease_r4 */


