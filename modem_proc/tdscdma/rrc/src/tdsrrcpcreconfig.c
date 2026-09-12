/*===========================================================================
              R R C PHYSICAL CHANNEL RECONFIGURATION  M O D U L E 

DESCRIPTION

  This module performs the Physical Channel Reconfiguration Procedure. This
  module processes the Physical Channel Reconfiguration Message and sets up 
  the reconfigurations given by the message. 
  
  
EXTERNALIZED FUNCTIONS

  tdsrrcpcrc_procedure_event_handler   Event handler for this procedure
  tdsrrcpcrc_init_procedure            Power-up initialization of the procedure

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Procedure must be initialized at power-up by calling tdsrrcpcrc_init_procedure. 
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcpcreconfig.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/16/11   bj       Added code to handle receive R9 PhyscialChannelReconfig Message.
03/10/11   bj       Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/18/10   ysh     Frequency info save. 3GPP-8.6.6.1(3.4.2)
                                  Save Primary freq when enter Non-CELL_DCH state.
11/11/10   yzh     (3GPP: 8.6.6.27)Del tdsrrcllcpcie_initialise_sync_a_post_veri_info()
                    when IE Downlink information common for all radio links(FDD->TDD).
11/10/10   bj       Deleted IE "RNC support for change of UE capability".(3GPP: 8.6.2.3)
11/05/10   yzh      (3GPP: 8.6.6.15)Delete DPCH Compressed mode info(FDD only, under FEATURE_CM_SUPPORTED)(3.2.20).
10/30/10   ysh     IE"Downlink information for each radio link" info save.  3GPP-8.6.6.4(3.2.13).
10/29/10   zwj     Added code to handle receive R4 PhyscialChannelReconfig Message.
10/22/10   ysh     When Physical channel reconfiguration, compressed mode runtime error, reconfig failure,
                        this function is FDD only, RRC_COMPRESSED_MODE_IND should delete.3GPP-8.2.11.2(3.2.7)

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
#include "rex.h"
#include "tdsrrcasn1.h"
#include "tdsrrcasn1util.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrcintif.h"
#include "tdsrrcccm.h"  
#include "tdsrrccu.h"
#include "tdsrrccui.h"
#include "tdsrrclcm.h"
#include "tdsrrcllc.h"
#include "tdsrrcmm_v.h"
#include "tdsrrcrbcommon.h"
#include "tdsrrcpcreconfig.h"
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
#include "tdsrrclogging.h"

#include "tdsrrcmisc.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsrrcdormancy.h"

#include "tdsrrcllcoc.h"
#include "tdsCFAParser_i.h"
#include "tdsrrcnv.h"
#include "tdsrrcqsh.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
#ifndef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
/* Enumerated type of status of validating an PCRC message                   */
typedef enum
{
  TDSRRCPC_MSG_VALID,              /* Message is valid                        */
  TDSRRCPC_MSG_UNSUPPORTED_CONFIG, /* Message has un-supported configuration  */
  TDSRRCPC_PHY_CHAN_FAILURE,       /* Physical channel setup failed           */
  TDSRRCPC_SIMULTANEOUS_RECONFIG,  /* Incompatible simultaneous reconfig      */
  TDSRRCPC_MSG_PROTOCOL_ERROR,     /* Message has protocol error              */
  TDSRRCPC_MSG_INVALID_CONFIG,     /* Message has invalid configuration       */
  TDSRRCPC_MSG_INCOMPLETE_CONFIG,   /* Message has incomplete configuration    */
  TDSRRCPC_CELL_UPDATE_OCCURED     /* CELL update due to OOS occured          */
}tdsrrcpc_msg_status_e_type;

typedef struct
{
  tdsrrcpc_msg_status_e_type failure_status; /* Failure cause                 */
  tdsrrc_ProtocolErrorCause  prot_err_cause; /* Protocol Error cause          */
}tdsrrcpc_msg_failure_cause_type;
#endif/*FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE*/

#define TDSRRCPC_INVALID_TRANSACTION_ID  10

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
/* We need to introduce a delay before transmitting
on a new configuration if the base-station cannot have
the old link and new link up at the same time. */
#define TDSRRCPCRC_TEMP_TIMER_SIG 0x4001
rex_sigs_type tdsrrcpcrc_sig;
rex_timer_type tdsrrcpcrc_temp_timer;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
#ifndef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
/* Structure to store all internal data related to PCRC procedure           */
typedef struct
{
  tdsrrcrb_substate_e_type   curr_substate; /* Store the current substate     */
  tdsrrcrb_transaction_id_type tr_ids;        /* Transaction ids for RR msg    */
  tdsrrc_state_e_type          state_indicated_by_msg;/* RRC State given by RR msg     */
  tdsrrcpc_msg_failure_cause_type status;     /* Status of message validation   */
  boolean                   new_crnti_valid; /* Indicates if a valid C-RNTI was recd 
                                            * in the message transitioning from CELL_DCH
                                            * to CELL_FACH */
  uint16                    new_crnti;
  boolean                   directed_cell_info; /* Indicates if cell info was recd */
 
  tdsrrc_int_u_rnti_type          new_urnti;
  boolean                      new_urnti_valid;          /* Indicates if a valid U-RNTI was recd */ 
  boolean                      pcrc_for_srns_relocation;  /* Indicates SRNS reloc involved with 
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

}tdsrrcpcrc_int_data_type;
#endif/*FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE*/
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrcpcrc_int_data_type tdsrrc_pcrc;

boolean tdsrrcpcrc_send_initiate_cu_flag = FALSE;
boolean tdsrrcpcrc_delete_crnti = FALSE;


/* =======================================================================
**                            Function Definitions
** ======================================================================= */
/*===========================================================================

FUNCTION   RRCPCRC_IS_SRNS_IN_PROGRESS()

DESCRIPTION
 This function returns if SRNS is in progress with current reconfig procedure


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcpcrc_is_srns_in_progress
(
  void
)
{
  return tdsrrc_pcrc.pcrc_for_srns_relocation;
}
/*===========================================================================

FUNCTION   RRCPCRC_GET_NEW_CRNTI_VALID()

DESCRIPTION
 This function returns if reconfig msg has valid CRNTI


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcpcrc_get_new_crnti_valid
(
  void
)
{
  return tdsrrc_pcrc.new_crnti_valid;
}
/*===========================================================================

FUNCTION   RRCPCRC_GET_CURRENT_SUB_STATE()

DESCRIPTION
 This function is used to return current substate


        
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrcrb_substate_e_type current_substate.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_substate_e_type tdsrrcpcrc_get_current_sub_state( )
{
  return tdsrrc_pcrc.curr_substate;
}

/*===========================================================================

FUNCTION tdsrrcpcrc_append_start_list_to_pcrc_complete

DESCRIPTION
  This function appends start list to tdsrrc_pcrc message for SRNS relocation and
  also when TM bearers are present during non-dch to dch rrc-state
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
static void tdsrrcpcrc_append_start_list_to_pcrc_complete
(
tdsrrc_UL_DCCH_Message *msg_ptr               /* Pointer to Uplink DCCH Msg */
)
{
  /*start list is included in ul_counter_synchronization_info. So enable this IE in the
  bitmask */
  TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.physicalChannelReconfigurationComplete,
    tdsrrc_PhysicalChannelReconfigurationComplete,ul_CounterSynchronisationInfo); 
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.physicalChannelReconfigurationComplete.ul_CounterSynchronisationInfo);

 
  /* call SMC API to append the start list */
  if (SUCCESS != tdsrrcsmc_append_start_list(&msg_ptr->message.u.physicalChannelReconfigurationComplete.
                                          ul_CounterSynchronisationInfo.startList,
                                          TDSRRCSMC_EXTRACT_AND_APPEND_START))
  {
    /*should not come here */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not append Start list");
  } 
}

/*===========================================================================

FUNCTION tdsrrcpcrc_cpid_is_pccpch_info_present

DESCRIPTION
  This function checks whether cell parameter ID info is present in
  the message or not.

DEPENDENCIES
  None.

RETURN VALUE
              Boolean.
                 TRUE  : cell parameter ID info is present in the message
                 FALSE : cell parameter ID info is not present in the message

===========================================================================*/
static boolean  tdsrrcpcrc_cpid_is_pccpch_info_present
(
  tdsrrc_PhysicalChannelReconfiguration *msg_ptr,
  tdsrrc_msg_ver_enum_type msg_version
)
{
  /*this function is used to find whether directed cell info is present or not during Cell_DCH to Cell_FACH 
   * state transition. It is safe to assume that the rl list will have only one element.
   */

  if (TDSMSG_REL5 == msg_version)
  {
    tdsrrc_PhysicalChannelReconfiguration_r5_IEs *pcreconfig_ptr_5 = 
        &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5;

    if (pcreconfig_ptr_5->m.dl_InformationPerRL_ListPresent)
    {
      if(pcreconfig_ptr_5->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
           T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_tdd)
      { 
        if (pcreconfig_ptr_5->dl_InformationPerRL_List.
            elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in PCRC message %d",
                 pcreconfig_ptr_5->dl_InformationPerRL_List.
                 elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }
  else
  if (TDSMSG_REL6 == msg_version)
  {
    tdsrrc_PhysicalChannelReconfiguration_r6_IEs *pcreconfig_ptr_6 = 
       &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.r6.physicalChannelReconfiguration_r6;

    if (pcreconfig_ptr_6->m.dl_InformationPerRL_ListPresent)
    {
      if(pcreconfig_ptr_6->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
           T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_tdd)
      { 
        if (pcreconfig_ptr_6->dl_InformationPerRL_List.
             elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in PCRC message %d",
                pcreconfig_ptr_6->dl_InformationPerRL_List.
                elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }
  else
  if (TDSMSG_REL7 == msg_version)
  {
    tdsrrc_PhysicalChannelReconfiguration_r7_IEs *pcreconfig_ptr_7 = 
         &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.r7.physicalChannelReconfiguration_r7;

    if (pcreconfig_ptr_7->m.dl_InformationPerRL_ListPresent)
    {
      if(pcreconfig_ptr_7->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
       T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_tdd)
      { 
        if (pcreconfig_ptr_7->dl_InformationPerRL_List.
            elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in PCRC message %d",
                pcreconfig_ptr_7->dl_InformationPerRL_List.
                elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }
  else
#ifdef FEATURE_TDSCDMA_REL8
  if (TDSMSG_REL8 == msg_version)
  {
    tdsrrc_PhysicalChannelReconfiguration_r8_IEs *pcreconfig_ptr_8 = &msg_ptr->u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.physicalChannelReconfiguration_r8;

    if (pcreconfig_ptr_8->m.dl_InformationPerRL_ListPresent)
    {
      if(pcreconfig_ptr_8->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd)
      { 
        if (pcreconfig_ptr_8->dl_InformationPerRL_List.
              elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in PCRC message %d",
                      pcreconfig_ptr_8->dl_InformationPerRL_List.elem[0].modeSpecificInfo
                      .u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }
  else
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  if (TDSMSG_REL9 == msg_version)
  {
    tdsrrc_PhysicalChannelReconfiguration_r9_IEs *pcreconfig_ptr_9 = &msg_ptr->u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
       u.criticalExtensions.u.r9.physicalChannelReconfiguration_r9;

    if (pcreconfig_ptr_9->m.dl_InformationPerRL_ListPresent)
    {
      if(pcreconfig_ptr_9->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd)
      { 
        if (pcreconfig_ptr_9->dl_InformationPerRL_List.
              elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in PCRC message %d",
                      pcreconfig_ptr_9->dl_InformationPerRL_List.elem[0].modeSpecificInfo
                      .u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }
  else
#endif /* FEATURE_TDSCDMA_REL9 */
  if (TDSMSG_REL4 == msg_version)
  {
    tdsrrc_PhysicalChannelReconfiguration_r4_IEs *pcreconfig_ptr_4 = 
          &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.
           physicalChannelReconfiguration_r4;

    if (pcreconfig_ptr_4->m.dl_InformationPerRL_ListPresent)
    {
      if (pcreconfig_ptr_4->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t == 
          T_tdsrrc_DL_InformationPerRL_r4_modeSpecificInfo_tdd)
      { 
        if (pcreconfig_ptr_4->dl_InformationPerRL_List.
              elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in PCRC message %d",
                   pcreconfig_ptr_4->dl_InformationPerRL_List.
                   elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }

  return FALSE;

}  /* tdsrrcpcrc_cpid_is_pccpch_info_present */


/*===========================================================================

FUNCTION   SEND_PCRC_FAILURE_MSG

DESCRIPTION

  This function sends the Physical Channel Reconfiguration Failure message
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
static void tdsrrcpcrc_send_pcrc_failure_message
(
  tdsrrcpc_msg_status_e_type failure_cause,  /* Failure Cause - should be a subset
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
    ERR_FATAL("No memory for PCRC Reconfig Failure msg!",0,0,0);
  }
  
  /* Reset SVTHHO in case of failure */
  tdsrrcsmc_reset_start_value_to_transmit_cs_hho(FALSE);
  TDSRRC_RESET_MSG_IE_PRESENT_PTR(ptr); /* Integrity Check info not present */
  
  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_physicalChannelReconfigurationFailure; 
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.physicalChannelReconfigurationFailure);
  /* Unlike other RBControl failure messages the transactionId in PhChFailure
     is optional */
  TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->message.u.physicalChannelReconfigurationFailure,
        tdsrrc_PhysicalChannelReconfigurationFailure,rrc_TransactionIdentifier);
  
  /* Transaction Id - First check the rejected_transacion id. If
  it's not invalid, this message is being rejected because there
  is already another message accepted but not processed.
  If the rejected_transaction id is invalid, we are rejecting the
  accepted message, so use the accepted transaction id.

  NOTE: Don't change the order, it's important to check rejected
  transaction first. */
  if( tdsrrc_pcrc.tr_ids.rejected_transaction != TDSRRCPC_INVALID_TRANSACTION_ID )
  {
    
    ptr->message.u.physicalChannelReconfigurationFailure.rrc_TransactionIdentifier =
      tdsrrc_pcrc.tr_ids.rejected_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_pcrc.tr_ids.rejected_transaction = TDSRRCPC_INVALID_TRANSACTION_ID;
  }
  else if( tdsrrc_pcrc.tr_ids.accepted_transaction != TDSRRCPC_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.physicalChannelReconfigurationFailure.rrc_TransactionIdentifier =
      tdsrrc_pcrc.tr_ids.accepted_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_pcrc.tr_ids.accepted_transaction = TDSRRCPC_INVALID_TRANSACTION_ID;
  }
  else
  {
    /* Don't know the transaction id for this message.( one reason
    for this could be that the message had a critical
    extenstion). Since it's unknown, we should set it to zero. */
    ptr->message.u.physicalChannelReconfigurationFailure.rrc_TransactionIdentifier= 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Transaction id invalid for PCRC Reconfig Failure message!");
  }

  switch( failure_cause )
  {
    case TDSRRCPC_MSG_UNSUPPORTED_CONFIG:
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      break;

    case TDSRRCPC_PHY_CHAN_FAILURE:
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_physicalChannelFailure;
      break;

    case TDSRRCPC_SIMULTANEOUS_RECONFIG:
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_incompatibleSimultaneousReconfiguration;
      break;

    case TDSRRCPC_MSG_PROTOCOL_ERROR:
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_protocolError;

	  ptr->message.u.physicalChannelReconfigurationFailure.failureCause.u.protocolError=
	  	rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation);
	   if(ptr->message.u.physicalChannelReconfigurationFailure.failureCause.u.protocolError == NULL)
      {
        ERR_FATAL("Memory allocation failure",0,0,0);
      }  
	
      /* When there is a Protocol Error, we also have to indicate
      the protocol error cause */
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.u.
          protocolError->diagnosticsType.t  =
          T_tdsrrc_ProtocolErrorInformation_diagnosticsType_type1;
     
	 ptr->message.u.physicalChannelReconfigurationFailure.failureCause.u.protocolError->
		 diagnosticsType.u.type1 = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation_type1);
	 
	   if(ptr->message.u.physicalChannelReconfigurationFailure.failureCause.u.protocolError->
  		    diagnosticsType.u.type1 == NULL)
      {
        ERR_FATAL("Memory allocation failure",0,0,0);
      } 
      /* Copy the given protocol error cause */
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.u.
          protocolError->diagnosticsType.u.type1->protocolErrorCause = 
          prot_err_cause;
      /* log the protocol error */
      tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                              (uint8)TDSRRC_PROCEDURE_PCR,
                              (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                              tdsrrc_log_map_protocol_cause(prot_err_cause)
                              );
      break;

    case TDSRRCPC_MSG_INVALID_CONFIG:
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      break;

    case TDSRRCPC_MSG_INCOMPLETE_CONFIG:
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      break;

    case TDSRRCPC_CELL_UPDATE_OCCURED:
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_cellUpdateOccurred;
    break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending PCRC Unknown Failure message");
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      break;
  }

  /* Get the RLC logical channel id for this msg from Logical Channel Manager ??*/
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH,
                                         DCCH_AM_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA);

  /* If the logical channel id is not invalid, send the message to the
  send chain module */
  if(ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending PCRC Failure msg to send-chain Cause:%d",ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t);
    /* Need to wait for L2 ack for all states because RRC needs to trigger include reconfig status indicator
    if RLC acknowledgement is not yet received.
    As per 8.3.1.3
    1> if the IE "Cell update cause" is set to "radio link failure" and the UE has submitted a reconfiguration response
    message to RLC and the UE has not received the RLC acknowledgement for the response message:
    2> include the IE "Reconfiguration Status Indicator" and set it to TRUE.*/
     if( tdsrrc_pcrc.status.failure_status == TDSRRCPC_PHY_CHAN_FAILURE)
    {
      tdsrrc_update_reconfig_waiting_for_l2ack_dch(TDSRRC_PROCEDURE_PCR);
    status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_PCR,  /* Procedure is PCR    */
                                 (void *)ptr,        /* Message pointer     */
                                 ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_REQUIRED  /* ACK is required */
                                );
     }
    else
    {
      status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_PCR,  /* Procedure is PCR    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_NOT_REQUIRED  /* No need for ACK */
                                  );
    }
    if( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Send chain failed for PCRC failure msg: error cause %d",status);
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
}/* end function tdsrrcpcrc_send_pcrc_failure_message */


#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
/*===========================================================================

FUNCTION   tdsrrcpcrc_send_lifetime_validation_failure

DESCRIPTION

  This function sends the Physical Channel Reconfiguration Failure message
  on Uplink DCCH. The failure cause must be given to this
  function.
 
  It should be triggered before accept of the message, and no PCRC internal
  status 

        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
static void tdsrrcpcrc_send_lifetime_validation_failure
(
  tdsrrcpc_msg_status_e_type failure_cause  /* Failure Cause - should be a subset
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
    ERR_FATAL("No memory for PCRC Reconfig Failure msg!",0,0,0);
  }
   
  TDSRRC_RESET_MSG_IE_PRESENT_PTR(ptr); /* Integrity Check info not present */
  
  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_physicalChannelReconfigurationFailure; 
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.physicalChannelReconfigurationFailure);
  /* Unlike other RBControl failure messages the transactionId in PhChFailure
     is optional */
  TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->message.u.physicalChannelReconfigurationFailure,
        tdsrrc_PhysicalChannelReconfigurationFailure,rrc_TransactionIdentifier);
  
  if( tdsrrc_pcrc.tr_ids.rejected_transaction != TDSRRCPC_INVALID_TRANSACTION_ID )
  {
    
    ptr->message.u.physicalChannelReconfigurationFailure.rrc_TransactionIdentifier =
      tdsrrc_pcrc.tr_ids.rejected_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_pcrc.tr_ids.rejected_transaction = TDSRRCPC_INVALID_TRANSACTION_ID;
  }
  else if( tdsrrc_pcrc.tr_ids.accepted_transaction != TDSRRCPC_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.physicalChannelReconfigurationFailure.rrc_TransactionIdentifier =
      tdsrrc_pcrc.tr_ids.accepted_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_pcrc.tr_ids.accepted_transaction = TDSRRCPC_INVALID_TRANSACTION_ID;
  }
  else
  {
    /* Don't know the transaction id for this message.( one reason
    for this could be that the message had a critical
    extenstion). Since it's unknown, we should set it to zero. */
    ptr->message.u.physicalChannelReconfigurationFailure.rrc_TransactionIdentifier= 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Transaction id invalid for PCRC Reconfig Failure message!");
  }

  /*Only leave selected causes, currently, we should put physical failure*/
  switch( failure_cause )
  {
    case TDSRRCPC_MSG_UNSUPPORTED_CONFIG:
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      break;

    case TDSRRCPC_PHY_CHAN_FAILURE:
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_physicalChannelFailure;
      break;

    case TDSRRCPC_SIMULTANEOUS_RECONFIG:
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_incompatibleSimultaneousReconfiguration;
      break;

    case TDSRRCPC_MSG_INVALID_CONFIG:
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      break;

    case TDSRRCPC_MSG_INCOMPLETE_CONFIG:
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
      break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending PCRC Unknown Failure message");
      ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t =
        T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      break;
  }

  /* Get the RLC logical channel id for this msg from Logical Channel Manager ??*/
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH,
                                         DCCH_AM_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA);

  /* If the logical channel id is not invalid, send the message to the
  send chain module */
  if(ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SDU Lifetime Validation Failure: sending PCRC Failure with Cause:%d",ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t);

    /*No need to wait for L2ACK*/
    status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_PCR,  /* Procedure is PCR    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_NOT_REQUIRED  /* No need for ACK */
                                  );

    if( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Send chain failed for PCRC failure msg: error cause %d",status);
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
}/* end function tdsrrcpcrc_send_pcrc_failure_message */


#endif /*FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION*/

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/*===========================================================================

FUNCTION   SEND_PCRC_COMPLETE_MSG

DESCRIPTION

  This function sends the Physical Channel Reconfiguration Complete message
  on Uplink DCCH. 
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
static void tdsrrcpcrc_send_pcrc_complete_message( boolean proc_wait_for_l2_ack)
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
    ERR_FATAL("No memory for PCRC Complete msg!",0,0,0);
  }
  
  TDSRRC_RESET_MSG_IE_PRESENT_PTR(ptr); /* Integrity Check info not present */
  /* PCRC Complete msg */
  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_physicalChannelReconfigurationComplete; 

  /* Fill up the message contents. The bit_mask is set to 0 since none
  of the optional IE's are included for now. */
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.physicalChannelReconfigurationComplete);

  if (tdsrrc_pcrc.pcrc_for_srns_relocation == TRUE)
  {
    tdsrrcpcrc_append_start_list_to_pcrc_complete(ptr);
  }

  //if this is case of IFHHO and not SRNS relocation(i.e. ul counter sync is not present)
  //then also send START_List and set count c act time and configure smc for Step_2

  if ( ((tdsrrc_pcrc.hho_ciph_status == TRUE) || (tdsrrc_pcrc.trans_to_dch_ciph_config_needed == TRUE)) &&
       (TRUE == tdsrrcsmc_tm_ciph_active()))
  {
    if(! (TDSRRC_MSG_COMMON_BITMASK_IE(ptr->message.u.physicalChannelReconfigurationComplete,
           tdsrrc_PhysicalChannelReconfigurationComplete,ul_CounterSynchronisationInfo)))
    {
      tdsrrcpcrc_append_start_list_to_pcrc_complete(ptr);
    }
  
    //Set the bitmask for count_c_act_time and copy the same..
    TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->message.u.physicalChannelReconfigurationComplete,
      tdsrrc_PhysicalChannelReconfigurationComplete,count_C_ActivationTime);
    
    ptr->message.u.physicalChannelReconfigurationComplete.count_C_ActivationTime =
      (tdsrrc_ActivationTime)tdsrrcsmc_step_2_cipher_config_for_hho();
  
    //Send step 2 config for HHO.. only one of these flags will be active, reset both of them
    //here
    if(tdsrrc_pcrc.hho_ciph_status)
    {
      tdsrrc_pcrc.hho_ciph_status =  FALSE;      
    }
    else if(tdsrrc_pcrc.trans_to_dch_ciph_config_needed)
    {
      tdsrrc_pcrc.trans_to_dch_ciph_config_needed = FALSE;      
    }
  }

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
    if(tdsrrcsib_is_sib11_11bis_12_read_deferred(TDSDEFERRED_ANY_ALLOWED_SIB))
    {
      ptr->message.u.physicalChannelReconfigurationComplete.m.laterNonCriticalExtensionsPresent=1;
 
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.physicalChannelReconfigurationComplete.
	  	laterNonCriticalExtensions);
 
      ptr->message.u.physicalChannelReconfigurationComplete.laterNonCriticalExtensions.m.
	  	v770NonCriticalExtensionsPresent=1;
 
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.physicalChannelReconfigurationComplete.
	  	laterNonCriticalExtensions.v770NonCriticalExtensions);
      
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.physicalChannelReconfigurationComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        physicalChannelReconfigurationComplete_v770ext);
 
      ptr->message.u.physicalChannelReconfigurationComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        physicalChannelReconfigurationComplete_v770ext.m.deferredMeasurementControlReadingPresent =1;
 
      ptr->message.u.physicalChannelReconfigurationComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        physicalChannelReconfigurationComplete_v770ext.deferredMeasurementControlReading = 
        tdsrrc_PhysicalChannelReconfigurationComplete_v770ext_IEs_deferredMeasurementControlReading_true_;
    }
#endif
  }


  /* PCRC Complete msg */
  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_physicalChannelReconfigurationComplete; 

  /* Transaction Id - use the accepted transaction id if known. Else
  use zero. Since this is a PCRC Complete message, we don't check 
  rejected transaction ids. */
  if( tdsrrc_pcrc.tr_ids.accepted_transaction != TDSRRCPC_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.physicalChannelReconfigurationComplete.rrc_TransactionIdentifier=
      tdsrrc_pcrc.tr_ids.accepted_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again unless next state is TDSRRCRB_WAIT_FOR_L2_ACK */
    if (!proc_wait_for_l2_ack)
    {
      tdsrrc_pcrc.tr_ids.accepted_transaction = TDSRRCPC_INVALID_TRANSACTION_ID;
    }
  }
  else
  {
    /* Don't know the transaction id for this message.( one reason
    for this could be that the message had a critical
    extenstion). Since it's unknown, we should set it to zero. */
    ptr->message.u.physicalChannelReconfigurationComplete.rrc_TransactionIdentifier = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Transaction id invalid for PCRC Complete message!");
  }

  /* For now, there is nothing else to include in this message. We
  can give it to the send chain. */

  /* Get the RLC logical channel id for this msg from Logical Channel Manager ??*/
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH,
                                         DCCH_AM_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA);

  /* If the logical channel id is not invalid, send the message to the
  send chain module */
  if(ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RPCRC Complete Message to send-chain");
    if (proc_wait_for_l2_ack) 
    {
      status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_PCR,  /* Procedure is PCR    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_REQUIRED  /* ACK is required */
                                   );
    }
    else
    {
      status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_PCR,  /* Procedure is PCR    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_NOT_REQUIRED  /* ACK is required */
                                   );
    }

    if( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Send chain failed for PCRC complete msg: error cause %d",status);
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
}/* end function tdsrrcpcrc_send_pcrc_complete_message */


/*===========================================================================

FUNCTION   RRCPCRC_VALIDATE_SRNS_RELOCATION_INFO()

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
static boolean tdsrrcpcrc_validate_srns_relocation_info
(
  tdsrrc_PhysicalChannelReconfiguration *msg_ptr,    /* Pointer to the pcReconfigMessage */
  tdsrrc_msg_ver_enum_type msg_version
)
{
   /*check whether dl_counter_sync info is present or not . Do not check for
   the contents of DL_counter_sync_info. First the integrity check has tobe made
   with new integrity protection mode info in the message. Later we can check
   the dl_counter_sync_info contents and send a failure if PDCP info is present */
  if (msg_version == TDSMSG_REL5)
  {
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5,
      tdsrrc_PhysicalChannelReconfiguration_r5_IEs,dl_CounterSynchronisationInfo))
    {
      return TRUE;
    } 
#else
    return (tdsrrcpcrc_validate_srns_relocation_info_new(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5
                                                   .physicalChannelReconfiguration_r5), TDSMSG_REL5));
#endif
  }
  else
  if (msg_version == TDSMSG_REL6)
  {
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.r6.physicalChannelReconfiguration_r6,
        tdsrrc_PhysicalChannelReconfiguration_r6_IEs,dl_CounterSynchronisationInfo))
    {
      return TRUE;
    } 
#else
    return (tdsrrcpcrc_validate_srns_relocation_info_new(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
                     criticalExtensions.u.r6.physicalChannelReconfiguration_r6), TDSMSG_REL6));
#endif
  }
  else
  if (msg_version == TDSMSG_REL7)
  {
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.r7.physicalChannelReconfiguration_r7,
        tdsrrc_PhysicalChannelReconfiguration_r7_IEs,dl_CounterSynchronisationInfo))
    {
      return TRUE;
    } 
#else
    return (tdsrrcpcrc_validate_srns_relocation_info_new(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.r7.physicalChannelReconfiguration_r7), TDSMSG_REL7));
#endif
  }
  else
#ifdef FEATURE_TDSCDMA_REL8
  if (msg_version == TDSMSG_REL8)
  {
     tdsrrc_PhysicalChannelReconfiguration_r8_IEs *pcreconfig_ptr = &msg_ptr->u.later_than_r3.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.
        physicalChannelReconfiguration_r8;  
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    if(pcreconfig_ptr->m.dl_CounterSynchronisationInfoPresent)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL Counter Sync Info is present in R8 PCRC message.");
      return TRUE;
    } 
#else
    return (tdsrrcpcrc_validate_srns_relocation_info_new(pcreconfig_ptr, TDSMSG_REL8));
#endif
  }
  else
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  if (msg_version == TDSMSG_REL9)
  {
     tdsrrc_PhysicalChannelReconfiguration_r9_IEs *pcreconfig_ptr = &msg_ptr->u.later_than_r3.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.r9.physicalChannelReconfiguration_r9;

#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    if(pcreconfig_ptr->m.dl_CounterSynchronisationInfoPresent)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL Counter Sync Info is present in R9 PCRC message.");
      return TRUE;
    } 
#else
    return (tdsrrcpcrc_validate_srns_relocation_info_new(pcreconfig_ptr, TDSMSG_REL9));
#endif
  }
  else
#endif
  if (msg_version == TDSMSG_REL4)  
  {
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.r4.physicalChannelReconfiguration_r4,
       tdsrrc_PhysicalChannelReconfiguration_r4_IEs,dl_CounterSynchronisationInfo))
  {
     return TRUE;
  }
#else
    return (tdsrrcpcrc_validate_srns_relocation_info_new(&(msg_ptr->u.later_than_r3.criticalExtensions.u.r4.physicalChannelReconfiguration_r4), TDSMSG_REL4));
#endif
  }

  return FALSE;
}




/*===========================================================================

FUNCTION   VALIDATE_PCRC_MESSAGE

DESCRIPTION

  This function validates the received PC Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a PCRC Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_validate_pcrc_r5
(
  tdsrrc_PhysicalChannelReconfiguration_r5_IEs *msg_ptr /* Pointer to the PCRC message */
)
{
 
  uecomdef_status_e_type status;
    /* Assume the message is valid for now */
  tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_VALID;

  /* Check the RRC State indicatior given by the message */
  switch( msg_ptr->rrc_StateIndicator )
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;

    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;


    default:
      /* No other states are supported right now. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported state in PC Reconfig Message");
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
      break;
  }

  if( tdsrrc_pcrc.status.failure_status == TDSRRCPC_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
     return;
  }

  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr,tdsrrc_PhysicalChannelReconfiguration_r5_IEs,dl_HSPDSCH_Information))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSPDSCH Info present");
    
    /* Check if network does not try to initiate HSDPA with invalid state */
    status = tdsrrc_hsdpa_initial_validation(msg_ptr->rrc_StateIndicator, 
      TDSRRC_CHECK_COMMON_MSG_TYPE(msg_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
      rrc_DL_HSPDSCH_Information_modeSpecificInfo_fdd));
    if (status == FAILURE)
    {
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_INVALID_CONFIG;
      return;
    }
  }
  
   /* Check if RB with PDCP Information list is present. This
  is not supported in the first release. This may be supported
  later */                                    
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
        tdsrrc_PhysicalChannelReconfiguration_r5_IEs,dl_CounterSynchronisationInfo)) 
#else
  if(tdsrrcpcrc_validate_srns_relocation_info_new(msg_ptr, TDSMSG_REL5))
#endif
  {

    /*check whether the SRNS relocation is within Cell_DCH or not.
     SRNS relocation information can come in reconfiguration messages
     only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"PCRC : SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_pcrc.state_indicated_by_msg);
      tdsrrc_pcrc.status.failure_status=TDSRRCPC_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has 
    PDCP info list. If it is true then this configuration is unsupported */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->dl_CounterSynchronisationInfo,
		rb_WithPDCP_InfoList)) ||
        (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->dl_CounterSynchronisationInfo,
         tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
      tdsrrc_pcrc.status.failure_status=TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation. 
        Set the flag that indicates that this procedure
      involves SRNS relocation */
      tdsrrc_pcrc.pcrc_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, tdsrrc_PhysicalChannelReconfiguration_r5_IEs,new_U_RNTI))
      {
        tdsrrc_pcrc.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(msg_ptr->new_U_RNTI),
                              &tdsrrc_pcrc.new_urnti);
      }
    }
  }

    /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  tdsrrc_pcrc.trans_to_dch_ciph_config_needed = FALSE;
  if((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr,
        tdsrrc_PhysicalChannelReconfiguration_r5_IEs,dl_CommonInformation)) && 
       (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->dl_CommonInformation,
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
             (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->dl_CommonInformation.dl_DPCH_InfoCommon,
             tdsrrc_DL_DPCH_InfoCommon_r4,mac_d_HFN_initial_value)),
             &msg_ptr->dl_CommonInformation.dl_DPCH_InfoCommon.mac_d_HFN_initial_value,
             &mac_d_hfn_l);

        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed ");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdsrrcsmc_process_cipher_config_for_hho failed");
        }
        tdsrrc_pcrc.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
     /* If UTRAN DRX coefficient is not present then it is invalid configuration */
     if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
         tdsrrc_PhysicalChannelReconfiguration_r5_IEs,utran_DRX_CycleLengthCoeff)))
     {
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_pcrc.state_indicated_by_msg);
       tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_INVALID_CONFIG;
       return;
     }
     if (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
     {
       /*should we do it after getting l2ack for response??*/
       if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, tdsrrc_PhysicalChannelReconfiguration_r5_IEs,ura_Identity)))
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
         tdsrrccu_clear_ura_id ();
       }
       else
       {
         /*URA ID present in the message set ura_identity variable with this value */
         if (msg_ptr->ura_Identity.numbits == 16)
         {
           tdsrrccu_update_ura_id (&(msg_ptr->ura_Identity));
         }
         else
         {
           TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",msg_ptr->ura_Identity.numbits);
           tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_PROTOCOL_ERROR;
           tdsrrc_pcrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
           return;
         }

       }
           
     }
  }
}/* end function tdsrrcpcrc_validate_pcrc_r5 */



/*===========================================================================

FUNCTION   tdsrrcpcrc_validate_pcrc_r6

DESCRIPTION

  This function validates the received PC Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a PCRC Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_validate_pcrc_r6
(
  tdsrrc_PhysicalChannelReconfiguration_r6_IEs *msg_ptr /* Pointer to the PCRC message */
)
{
 
  uecomdef_status_e_type status;
    /* Assume the message is valid for now */
  tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_VALID;

  /* Check the RRC State indicatior given by the message */
  switch( msg_ptr->rrc_StateIndicator )
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;

    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;


    default:
      /* No other states are supported right now. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported state in PC Reconfig Message");
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
      break;
  }

  if( tdsrrc_pcrc.status.failure_status == TDSRRCPC_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
     return;
  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
      tdsrrc_PhysicalChannelReconfiguration_r6_IEs,dl_HSPDSCH_Information))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSPDSCH Info present");
    
    /* Check if network does not try to initiate HSDPA with invalid state */
    status = tdsrrc_hsdpa_initial_validation_r6(msg_ptr->rrc_StateIndicator, 
      &msg_ptr->dl_HSPDSCH_Information);
    
    if (status == FAILURE)
    {
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_INVALID_CONFIG;
      return;
    }
  }
  
   /* Check if RB with PDCP Information list is present. This
  is not supported in the first release. This may be supported
  later */                                    
  
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
        tdsrrc_PhysicalChannelReconfiguration_r6_IEs,dl_CounterSynchronisationInfo)) 
#else
  if(tdsrrcpcrc_validate_srns_relocation_info_new(msg_ptr, TDSMSG_REL6))
#endif

  {

    /*check whether the SRNS relocation is within Cell_DCH or not.
     SRNS relocation information can come in reconfiguration messages
     only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"PCRC : SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_pcrc.state_indicated_by_msg);
      tdsrrc_pcrc.status.failure_status=TDSRRCPC_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has 
    PDCP info list. If it is true then this configuration is unsupported */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->dl_CounterSynchronisationInfo,
		rb_WithPDCP_InfoList)) ||
        (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->dl_CounterSynchronisationInfo,
         tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
      tdsrrc_pcrc.status.failure_status=TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation. 
        Set the flag that indicates that this procedure
      involves SRNS relocation */
      tdsrrc_pcrc.pcrc_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, tdsrrc_PhysicalChannelReconfiguration_r6_IEs,new_U_RNTI))
      {
        tdsrrc_pcrc.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(msg_ptr->new_U_RNTI),
                              &tdsrrc_pcrc.new_urnti);
      }
    }
  }

    /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  tdsrrc_pcrc.trans_to_dch_ciph_config_needed = FALSE;
  if((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr,
        tdsrrc_PhysicalChannelReconfiguration_r6_IEs,dl_CommonInformation)) && 
       (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->dl_CommonInformation,tdsrrc_DL_CommonInformation_r6,
       dl_dpchInfoCommon)) &&
       (T_tdsrrc_DL_CommonInformation_r6_dl_dpchInfoCommon_dl_DPCH_InfoCommon == 
         msg_ptr->dl_CommonInformation.dl_dpchInfoCommon.t))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");
        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(
             msg_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon,tdsrrc_DL_DPCH_InfoCommon_r6,
               mac_d_HFN_initial_value)),
              &msg_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
              &mac_d_hfn_l);
        if (loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"SMC process ciphering config for HHO failed");
        }
        tdsrrc_pcrc.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
     /* If UTRAN DRX coefficient is not present then it is invalid configuration */
     if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
         tdsrrc_PhysicalChannelReconfiguration_r6_IEs,utran_DRX_CycleLengthCoeff)))
     {
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_pcrc.state_indicated_by_msg);
       tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_INVALID_CONFIG;
       return;
     }
     if (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
     {
       /*should we do it after getting l2ack for response??*/
       if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, tdsrrc_PhysicalChannelReconfiguration_r6_IEs,ura_Identity)))
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
         tdsrrccu_clear_ura_id ();
       }
       else
       {
         /*URA ID present in the message set ura_identity variable with this value */
         if (msg_ptr->ura_Identity.numbits == 16)
         {
           tdsrrccu_update_ura_id (&(msg_ptr->ura_Identity));
         }
         else
         {
           TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",msg_ptr->ura_Identity.numbits);
           tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_PROTOCOL_ERROR;
           tdsrrc_pcrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
           return;
         }

       }
           
     }
  }
}/* end tdsrrcpcrc_validate_pcrc_r6() */


/*===========================================================================

FUNCTION   tdsrrcpcrc_validate_pcrc_r7

DESCRIPTION

  This function validates the received PC Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a PCRC Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_validate_pcrc_r7
(
  tdsrrc_PhysicalChannelReconfiguration_r7_IEs *msg_ptr /* Pointer to the PCRC message */
)
{
 
  uecomdef_status_e_type status;
    /* Assume the message is valid for now */
  tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_VALID;

  /* Check the RRC State indicatior given by the message */
  switch( msg_ptr->rrc_StateIndicator )
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;

    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;


    default:
      /* No other states are supported right now. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported state in PC Reconfig Message");
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
      break;
  }

  if( tdsrrc_pcrc.status.failure_status == TDSRRCPC_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
     return;
  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
      tdsrrc_PhysicalChannelReconfiguration_r7_IEs,dl_HSPDSCH_Information))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSPDSCH Info present");
    
    /* Check if network does not try to initiate HSDPA with invalid state */
    status = tdsrrc_hsdpa_initial_validation(msg_ptr->rrc_StateIndicator, 
          TDSRRC_CHECK_COMMON_MSG_TYPE(msg_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
          rrc_DL_HSPDSCH_Information_r7_modeSpecificInfo_fdd));
    
    if (status == FAILURE)
    {
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_INVALID_CONFIG;
      return;
    }
  }
  
   /* Check if RB with PDCP Information list is present. This
  is not supported in the first release. This may be supported
  later */                                    

#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
        tdsrrc_PhysicalChannelReconfiguration_r7_IEs,dl_CounterSynchronisationInfo)) 
#else
  if(tdsrrcpcrc_validate_srns_relocation_info_new(msg_ptr, TDSMSG_REL7))
#endif

  {

    /*check whether the SRNS relocation is within Cell_DCH or not.
     SRNS relocation information can come in reconfiguration messages
     only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"PCRC : SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_pcrc.state_indicated_by_msg);
      tdsrrc_pcrc.status.failure_status=TDSRRCPC_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has 
    PDCP info list. If it is true then this configuration is unsupported */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->dl_CounterSynchronisationInfo,
		rb_WithPDCP_InfoList)) ||
        (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->dl_CounterSynchronisationInfo,
         tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
      tdsrrc_pcrc.status.failure_status=TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation. 
        Set the flag that indicates that this procedure
      involves SRNS relocation */
      tdsrrc_pcrc.pcrc_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, tdsrrc_PhysicalChannelReconfiguration_r7_IEs,new_U_RNTI))
      {
        tdsrrc_pcrc.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(msg_ptr->new_U_RNTI),
                              &tdsrrc_pcrc.new_urnti);
      }
    }
  }

    /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  tdsrrc_pcrc.trans_to_dch_ciph_config_needed = FALSE;
  if((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr,
        tdsrrc_PhysicalChannelReconfiguration_r7_IEs,dl_CommonInformation)) && 
       (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->dl_CommonInformation,
       tdsrrc_DL_CommonInformation_r7,dl_dpchInfoCommon)) &&
       (T_tdsrrc_DL_CommonInformation_r7_dl_dpchInfoCommon_dl_DPCH_InfoCommon == 
         msg_ptr->dl_CommonInformation.dl_dpchInfoCommon.t))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");
        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon,tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
              &msg_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
              &mac_d_hfn_l);
        if (loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"SMC process ciphering config for HHO failed");
        }
        tdsrrc_pcrc.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
     /* If UTRAN DRX coefficient is not present then it is invalid configuration */
     if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
         tdsrrc_PhysicalChannelReconfiguration_r7_IEs,utran_DRX_CycleLengthCoeff)))
     {
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_pcrc.state_indicated_by_msg);
       tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_INVALID_CONFIG;
       return;
     }
     if (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
     {
       /*should we do it after getting l2ack for response??*/
       if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, tdsrrc_PhysicalChannelReconfiguration_r7_IEs,ura_Identity)))
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
         tdsrrccu_clear_ura_id ();
       }
       else
       {
         /*URA ID present in the message set ura_identity variable with this value */
         if (msg_ptr->ura_Identity.numbits == 16)
         {
           tdsrrccu_update_ura_id (&(msg_ptr->ura_Identity));
         }
         else
         {
           TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",msg_ptr->ura_Identity.numbits);
           tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_PROTOCOL_ERROR;
           tdsrrc_pcrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
           return;
         }

       }
           
     }
  }
}/* end tdsrrcpcrc_validate_pcrc_r7() */


/*===========================================================================

FUNCTION   tdsrrcpcrc_check_and_send_failure_for_simul_reconfig

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
static uecomdef_status_e_type tdsrrcpcrc_check_and_send_failure_for_simul_reconfig
(
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id
)
{

  tdsrrcpc_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;
  
  if ((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
      (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
     (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incomatible simultaneous reconfig! Rejecting PCRC message");
    if (tdstransaction_id == tdsrrc_pcrc.tr_ids.accepted_transaction )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate PC RC message - accepted tr-id matched!");
      /* Nothing else to do, just return */
    } 
    else if (tdstransaction_id == tdsrrc_pcrc.tr_ids.rejected_transaction )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate PC RC message - rejected tr-id matched!");
      /* Nothing else to do, just return */
    } 
    else
    {
      /* We have received a RB RC message with a new transaction id.
      This message should be in our rejected transaction list */
      
      temp_failure_cause.failure_status = tdsrrc_pcrc.status.failure_status;
      temp_rejected_transaction = tdsrrc_pcrc.tr_ids.rejected_transaction;
      
      tdsrrc_pcrc.tr_ids.rejected_transaction = tdstransaction_id;
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_SIMULTANEOUS_RECONFIG;
 
      /*Check whether Cell Update is going on */
      if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
           (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
      {
        if(tdsrrc_pcrc.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_pcrc subsstate(%d) Send failure with %d cause",TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF,tdsrrc_pcrc.status.failure_status);
          tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                   tdsrrc_pcrc.status.prot_err_cause );
          
          tdsrrc_pcrc.status.failure_status = temp_failure_cause.failure_status;
          tdsrrc_pcrc.tr_ids.rejected_transaction = temp_rejected_transaction;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
          if (SUCCESS != tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_PCR))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCR failed to register with CU");
          }
          /* Wait until Cell update procedure is completed */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
        }
      }
      else
      {
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                        tdsrrc_pcrc.status.prot_err_cause );
        /*continue the same substate */ 
      }
    }
    return (FAILURE);
  }


 /* We have received a Physical Channel Reconfig message with a new transaction id.
 Accept the new transaction ID  */
  tdsrrc_pcrc.tr_ids.accepted_transaction = tdstransaction_id;

  return (SUCCESS);
}


/*===========================================================================

FUNCTION   RRCPCRC_SEND_CHANNEL_CONFIG_REQ

DESCRIPTION

  This function sends the TDSRRC_CHANNEL_CONFIG_REQ command to LLC
        
DEPENDENCIES

  TDSORDERED_CONFIG must be set before calling this function.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_send_channel_config_req
(
  void
)
{
  tdsrrc_cmd_type *ptr;          /* To hold the Command                       */
  uint32 rb_count=0;      /* To count RBs to be reconfigured           */
  #ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
  rlc_lc_id_type ul_rlc_lc_id = TDSRRCLCM_RLC_LC_ID_NOT_FOUND;
  #endif
  #ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrc_RB_Identity srb5_rb_id;
  #endif
    
  /* Get the command buffer */
  if((ptr = tdsrrc_get_int_cmd_buf())!=NULL)
  {
    /* Fill in the command id  */
    ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;


    /* Check if state change is required */
    if( (tdsrrc_get_state()) == (tdsrrc_pcrc.state_indicated_by_msg) )
    {
      ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
    }
    else
    {
      /* The message indicates that state change is required.
      Indicate to LLC what the next state should be. */
      ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
      ptr->cmd.chan_config_req.next_state = tdsrrc_pcrc.state_indicated_by_msg;

      if ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
          ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) || 
           (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)))
      {
        /* UE is directly transitioning to Cell_PCH state from Cell_FACH state so
        include release of CCCH and establishment of PCCH in the same channel config
        request */
        ptr->cmd.chan_config_req.rb[rb_count].rb_id       = CCCH_RADIO_BEARER_ID;
        ptr->cmd.chan_config_req.rb[rb_count++].rb_config = TDSRELEASE_RB;
        ptr->cmd.chan_config_req.rb[rb_count].rb_id       = PCCH_RADIO_BEARER_ID;
        ptr->cmd.chan_config_req.rb[rb_count++].rb_config = TDSESTABLISH_RB;
      }
    }
    #ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
    if (TDSCHECK_RLC_ALL_RB_REEST_ENABLE)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Indicated state : %d,OC usage :%d .",tdsrrc_pcrc.state_indicated_by_msg,tdstransition_config.toc_usage);
    if (((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH) && (tdstransition_config.toc_usage 
          == TDSTOC_FOR_DCH_AND_FACH)) || ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
          (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)))
    {
          /*reconfig signalling RBs */
      ptr->cmd.chan_config_req.rb[rb_count].rb_id = DCCH_UM_RADIO_BEARER_ID;
      ptr->cmd.chan_config_req.rb[rb_count++].rb_config = TDSRECONFIG_RB;
      ptr->cmd.chan_config_req.rb[rb_count].rb_id = DCCH_AM_RADIO_BEARER_ID;
      ptr->cmd.chan_config_req.rb[rb_count++].rb_config = TDSRECONFIG_RB;
      ptr->cmd.chan_config_req.rb[rb_count].rb_id = DCCH_DT_HIGH_PRI_RADIO_BEARER_ID;
      ptr->cmd.chan_config_req.rb[rb_count++].rb_config = TDSRECONFIG_RB;

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
        ptr->cmd.chan_config_req.rb[rb_count].rb_id = DCCH_DT_LOW_PRI_RADIO_BEARER_ID;
        ptr->cmd.chan_config_req.rb[rb_count++].rb_config = TDSRECONFIG_RB;
      }

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      if(tdsrrclcm_get_rb_id_if_dl_lc_type_exists(UE_LOGCHAN_TM_DCCH_DL, &srb5_rb_id) == TDSRRCLCM_SUCCESS)
      {
        ptr->cmd.chan_config_req.rb[rb_count].rb_id       = srb5_rb_id;
        ptr->cmd.chan_config_req.rb[rb_count++].rb_config = TDSRECONFIG_RB;
      }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
    }
    }
    #endif

    ptr->cmd.chan_config_req.num_rb_to_config = (int) rb_count; 
    /* Indicate the id of the procedure sending the command */
    ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_PCR;

    /* Indicate that we need a confirmation for this command */
    ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

    /*If state transition is Cell_DCH->Cell_FACH or vice-verca, 
      add all the existing AM PS RBs in chan_config_req*/	
    if(((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH) && (tdstransition_config.toc_usage 
          == TDSTOC_FOR_DCH_AND_FACH)) || ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
          (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH))
          #ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
          ||((TDSCHECK_RLC_ALL_RB_REEST_ENABLE)&&(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
          (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH))
          #endif
          )
    {
      tdsrrc_fill_rb_id_chan_req_from_est_rabs(&rb_count, &ptr->cmd.chan_config_req);
    }

    /* We are not required to fill any other data for this command.
    Now, send the command */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"CHANNEL_CONFIG_REQ RBs %d",rb_count);
    tdsrrc_put_int_cmd( ptr );

    
  }
  else
  {
    /* Out of memory!!  */
    ERR_FATAL("Can't send channel config! No memory!",0,0,0);
  }
}/* end function tdsrrcpcrc_send_channel_config_req */


/*===========================================================================

FUNCTION  RRCPCRC_CLEAR_PROCEDURE

DESCRIPTION

  This function clears all the internal variables in
  the PCRC procedure and brings them to their default
  values.
    
DEPENDENCIES

  The Substate of the procedure should be changed
  to initial substate before calling this function.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_clear_procedure( void )
{
  /* Initialize the transaction id's for this procedure.
  Use a number that can't be an actual received transaction
  id. */
  tdsrrc_pcrc.tr_ids.accepted_transaction = TDSRRCPC_INVALID_TRANSACTION_ID;
  tdsrrc_pcrc.tr_ids.rejected_transaction = TDSRRCPC_INVALID_TRANSACTION_ID;

  /* Initialize the current substate */
  tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

   /* Clear the C-RNTI valid flag */
  tdsrrc_pcrc.new_crnti_valid    = FALSE;
  tdsrrc_pcrc.directed_cell_info = FALSE;

  tdsrrc_pcrc.pcrc_for_srns_relocation = FALSE;
  tdsrrc_pcrc.new_urnti_valid = FALSE;

  /* free the memory allocate for mm_cmd_buf
  if it is not free or in case of unsuccessful events
  before sending this cmd to mm */
  if (tdsrrc_pcrc.cn_info_saved != FALSE)
  {
    /* free the memory allocated for mm cmd */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Free MM cmd buffer %x",tdsrrc_pcrc.mm_cmd_ptr);
    mm_free_cmd_buf(tdsrrc_pcrc.mm_cmd_ptr);
    /* set cn_info_saved to false */
    tdsrrc_pcrc.cn_info_saved = FALSE;
  }

  /* Reset Initiate Cell Update Flag */
  tdsrrcpcrc_send_initiate_cu_flag = FALSE;
  tdsrrcpcrc_delete_crnti = FALSE;

  //Set hho_ciph_status to false
  tdsrrc_pcrc.hho_ciph_status = FALSE;      

  tdsrrc_pcrc.trans_to_dch_ciph_config_needed = FALSE;
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  tdsrrc_pcrc.initiate_cell_selection_trans_from_fach = FALSE;     
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/


  tdsrrc_pcrc.high_mobility_ind = FALSE;

}


/*===========================================================================

FUNCTION   RRCPCRC_PROCESS_SUCCESSFUL_PROC_COMPLETION

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
static void tdsrrcpcrc_process_successful_proc_completion
(
 boolean indicate_rlc
)
{
  /* send the nas cn info if stored */
  if (tdsrrc_pcrc.cn_info_saved != FALSE)
  {
    /* Put the command on MM queue */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_SERVICE_IND_FROM_OTA_MSG due to CN info to MM");

    /* clear the flag */
    if(tdsrrc_pcrc.mm_cmd_ptr->cmd.hdr.message_id == (byte) RRC_SERVICE_IND_FROM_OTA_MSG )
    {
       tdsrrcccm_update_service_info(tdsrrc_pcrc.mm_cmd_ptr);
    }
    
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(tdsrrc_pcrc.mm_cmd_ptr);
    }
    mm_put_cmd(tdsrrc_pcrc.mm_cmd_ptr);
    tdsrrc_pcrc.cn_info_saved = FALSE;
  }
  /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
  configurations */
  tdsrrcllc_clear_ordered_config();  

  if (((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) || 
       (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH )) &&
      (indicate_rlc == TRUE))
  {
    /* register with RLC for uplink data indication */
    tdsrrcrb_register_with_rlc_for_ul_data_ind (TDSRRC_PROCEDURE_PCR);

  }

  /* Go to the next sub-state in the state machine. That's
  the initial substate since we've sent the RB RC complete
  message */
  tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

  /* At this point we don't need to remember any of the
  Radio Bearer information since it's stored in ESTABLISHED_RABS.
  Clear our internal variables */
  tdsrrcpcrc_clear_procedure();

  if (tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_RB_PROC_COMPL_IND) 
  {
    tdsrrcrb_send_rb_proc_compl_ind();
  }
  
}    /* tdsrrcpcrc_process_successful_proc_completion */
/*===========================================================================

FUNCTION   tdsrrcpcrc_check_and_get_primary_plmn_id

DESCRIPTION

  This function will check whether primary plmn id is present 
  in r3,r5 path of PCR message. If present then it will return 
  primary plmn id.
        
DEPENDENCIES
  None.

RETURN VALUE
TRUE : IF Primary PLMN I.D is present in PCR message in r3,r5 path
FFALSE: Otherwise

SIDE EFFECTS

  None.

===========================================================================*/
static boolean tdsrrcpcrc_check_and_get_primary_plmn_id
(
  /* Pointer to the PCR message */
  tdsrrc_PhysicalChannelReconfiguration *msg_ptr,
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
  /*Check to see if primary PLMN I.D is present in PCR message ext
   * physicalChannelReconfiguration-v690ext which is present in 
   * v690NonCriticalExtensions
   */
  switch(msg_version)
  {
    case TDSMSG_REL4:
      if((msg_ptr->u.later_than_r3.criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.m.v590NonCriticalExtenstionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtenstions.m.
              v690NonCriticalExtensionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
                 v590NonCriticalExtenstions.v690NonCriticalExtensions.physicalChannelReconfiguration_v690ext.
                 m.primary_plmn_IdentityPresent)
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:PCR msg contains primary_plmn_Id in r4 path");
        primary_plmn_Id_present = TRUE;
        *primary_plmn_Identity = msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
                 v590NonCriticalExtenstions.v690NonCriticalExtensions.physicalChannelReconfiguration_v690ext.primary_plmn_Identity;
      }
      break;
  
    case TDSMSG_REL5:
      if((msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.m.v690NonCriticalExtensionsPresent)
         &&
         (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
           u.r5.v690NonCriticalExtensions.physicalChannelReconfiguration_v690ext.m.primary_plmn_IdentityPresent)
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:PCR msg contains primary_plmn_Id in r5 path");
        primary_plmn_Id_present = TRUE;
        *primary_plmn_Identity = msg_ptr->u.later_than_r3.criticalExtensions.u.
              criticalExtensions.u.r5.v690NonCriticalExtensions.
              physicalChannelReconfiguration_v690ext.primary_plmn_Identity;
      }
      break;
  
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"NWS: Msg version = %d,Not supported",msg_version);
      break;
  }
  return primary_plmn_Id_present;
}



/*===========================================================================

FUNCTION   tdsrrcpcrc_process_high_mobility_ind_in_non_critical_ext

DESCRIPTION

  This function will process High Mobility Ind received in Non-critical ext
        
DEPENDENCIES
  None.

RETURN VALUE
None

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_process_high_mobility_ind_in_non_critical_ext
(
  tdsrrc_PhysicalChannelReconfiguration *msg_ptr,
  tdsrrc_msg_ver_enum_type msg_version
)
{
  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
    if(msg_version == TDSMSG_REL5)
    {
     if((msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.m.v690NonCriticalExtensionsPresent) &&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v690NonCriticalExtensions.m.
          v770NonCriticalExtensionsPresent) &&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v690NonCriticalExtensions.v770NonCriticalExtensions.
          physicalChannelReconfiguration_v770ext.m.ueMobilityStateIndicatorPresent))
      {
        if(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v690NonCriticalExtensions.v770NonCriticalExtensions.
            physicalChannelReconfiguration_v770ext.ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in PCRC.");
          tdsrrc_pcrc.high_mobility_ind = TRUE;
        }
      }
    }
    else if(msg_version == TDSMSG_REL6)
    {
     if(( msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.v6b0NonCriticalExtensions.m.
           v770NonCriticalExtensionsPresent) &&
              (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.v6b0NonCriticalExtensions.v770NonCriticalExtensions.
              physicalChannelReconfiguration_v770ext.m.ueMobilityStateIndicatorPresent))
     {
       if(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.v6b0NonCriticalExtensions.
           v770NonCriticalExtensions.physicalChannelReconfiguration_v770ext.ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected)
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in PCRC.");
         tdsrrc_pcrc.high_mobility_ind = TRUE;
       }
     }     
    }
    else if(msg_version == TDSMSG_REL4)
    {
      
      if((msg_ptr->u.later_than_r3.criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent) &&
              (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.m.v590NonCriticalExtenstionsPresent) &&
              (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtenstions.m.
                    v690NonCriticalExtensionsPresent) &&
              (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtenstions.
                    v690NonCriticalExtensions.m.v770NonCriticalExtensionsPresent) &&
              (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
                       v590NonCriticalExtenstions.v690NonCriticalExtensions.v770NonCriticalExtensions.
                          physicalChannelReconfiguration_v770ext.m.ueMobilityStateIndicatorPresent))
       {
         if(msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
             v590NonCriticalExtenstions.v690NonCriticalExtensions.v770NonCriticalExtensions.
            physicalChannelReconfiguration_v770ext.ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected) 
         {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in PCRC.");
           tdsrrc_pcrc.high_mobility_ind = TRUE;
         }
      }
    }  
  }
}

/*===========================================================================

FUNCTION   PROCESS_PCRC_R5

DESCRIPTION

  This function processes the received Rel 5 PC Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a PCRC Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to reconfigure the Physical Channel given by the message.
    
    If TDSORDERED_CONFIG could not be set, a PCRC Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_process_pcrc_r5
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_PhysicalChannelReconfiguration_r5_IEs* pcrc_ptr = NULL;

  tdsrrc_DL_DCCH_Message  *dcch_msg_ptr = NULL; /* Pointer to the RB Setup message */
  tdsrrc_PhysicalChannelReconfiguration *msg_ptr = NULL;		/* Pointer to the RB Setup message        */

  tdsrrcllc_oc_status_e_type             oc_status;/* To hold tdsordered_config status   */
  tdsrrc_state_e_type                    current_state; /* current RRC State  */
  tdsrrc_int_c_rnti_type                 c_rnti;       /* Local C_RNTI value */
  tdsrrc_int_u_rnti_type                 u_rnti;
  tdsrrcrb_directed_cell_info_type       directed_cell_info; /* directed cell info struct */

  tdsrrcrb_rnti_update_info_type rnti_update_info;   /* to hold rnti update info */

  boolean       ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                    occured while setting tdsordered_config. */

   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  boolean primary_plmn_Id_present = FALSE;
   /*Variable to hold primary plmn i.d if present in OTA msg*/
  tdsrrc_PLMN_Identity primary_plmn_Identity;


  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  msg_ptr = &dcch_msg_ptr->message.u.physicalChannelReconfiguration;

  if (FAILURE == 
      tdsrrcpcrc_check_and_send_failure_for_simul_reconfig(
        msg_ptr->u.later_than_r3.rrc_TransactionIdentifier))
  {
    return;
  }

  /* Store Msg Ptr in a local variable */
  pcrc_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.
    criticalExtensions.u.r5.physicalChannelReconfiguration_r5;

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/
  if ((pcrc_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         pcrc_ptr->m.activationTimePresent,
         pcrc_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC SDU Lifetime validation failed");
      tdsrrcpcrc_send_lifetime_validation_failure(TDSRRCPC_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force PCRC Activation time to NOW");
      pcrc_ptr->m.activationTimePresent = 0;
    }

  }

#endif /*FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION*/

  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  tdsrrcpcrc_validate_pcrc_r5( pcrc_ptr );

  if( tdsrrc_pcrc.status.failure_status != TDSRRCPC_MSG_VALID )
  {
    /*  Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC Message validation failed");

    /* Send the PC Reconfig Failure Message */
    tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                              tdsrrc_pcrc.status.prot_err_cause
                            );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
  }
  /* If the message is valid, proceed further */
  else
  {

    TDSRRC_MSG0(MSG_LEGACY_LOW,"Processing PCRC Message");

    /* Check if there is CN information present. If so, we need
    to send the information to the Cell Change manager */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr,tdsrrc_PhysicalChannelReconfiguration_r5_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      /*Check to see if primary PLMN I.D is present in PCR message ext
       * physicalChannelReconfiguration-v690ext which is present in 
       * v690NonCriticalExtensions
       */
       primary_plmn_Id_present = tdsrrcpcrc_check_and_get_primary_plmn_id(msg_ptr,
                                                                  &primary_plmn_Identity,TDSMSG_REL5);
      tdsrrc_pcrc.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info(
                        &(pcrc_ptr->cn_InformationInfo)
                        ,primary_plmn_Id_present
                        ,&primary_plmn_Identity
                        );
 
      if (tdsrrc_pcrc.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_pcrc.cn_info_saved = TRUE;
      }
    }
    tdsrrcpcrc_process_high_mobility_ind_in_non_critical_ext(msg_ptr,TDSMSG_REL5);
   
    /* Need to send reconfig IE info. First set TDSORDERED_CONFIG
    so that LLC can process the necessary parameters. */
    oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_PCR,
                                           tdsrrc_pcrc.state_indicated_by_msg,
                                           tdsrrc_DL_DCCH_Message_PDU,
                                           (void *)dcch_msg_ptr );

    switch( oc_status )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"oc_status is %d ",oc_status);
      case TDSORDERED_CONFIG_SET:
        current_state = tdsrrc_get_state(); 
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
        if((tdsrrcsib_is_current_event_sib_read_in_dch()) &&
           (TDSRRC_PROCEDURE_GPS == tdsrrcsib_get_proc_id_for_sib_read_in_dch()))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Force Init GPS ");
          tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
        }


#endif 

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
        /* Populate the local variables to indicate if Re-direction info is present */
        directed_cell_info.freq_info_present = FALSE;
        directed_cell_info.cpid_present = FALSE;
        if((current_state==TDSRRC_STATE_CELL_FACH) && 
                 ((tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
                 ||(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
                 ||(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
           
          if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
              tdsrrc_PhysicalChannelReconfiguration_r5_IEs,frequencyInfo)) &&
             (TDSRRC_CHECK_COMMON_MSG_TYPE(pcrc_ptr->frequencyInfo.
              modeSpecificInfo,rrc_FrequencyInfo_modeSpecificInfo_tdd)))           
          {
            directed_cell_info.freq_info_present = TRUE;
            directed_cell_info.uarfcn_dl = pcrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
          }
          
          if (tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL5))
          {
            directed_cell_info.cpid_present = TRUE;
            directed_cell_info.cell_param_id = pcrc_ptr->dl_InformationPerRL_List.elem[0].
                                      modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          }
          /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
          FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
          if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
             (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
            if(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
            {
              tdsrrcpcrc_send_initiate_cu_flag = TRUE;
              tdsrrcpcrc_delete_crnti          = TRUE;
            }
            else
            {
              tdsrrc_pcrc.directed_cell_info = TRUE;
            }
          }          
        }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
        if (
            (current_state==tdsrrc_pcrc.state_indicated_by_msg)||
            ((current_state==TDSRRC_STATE_CELL_FACH)&& 
             (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH))
           )
        {
          /*  CellDCH<->CellDCH or CellFACH<->CellFACH or CellFACH->CellDCH 
          transition. Ordered Config has been set. We can setup the
          Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");

          /*check whether any frequency info is present in the message
          and transition is CELL_FACH->CELL_FACH. In this case
          initiate a cell update after channel config is successfully completed */
          if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
             (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
          {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
     
            /*call the function that evaluates the directed cell info and set the flags accordingly */
            if ((tdsrrcpcrc_send_initiate_cu_flag == FALSE)&&
                (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
            {
    
              if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r5_IEs,new_C_RNTI))
              {
                tdsrrc_pcrc.new_crnti_valid = TRUE;
                tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                                    &c_rnti);
                tdsrrc_pcrc.new_crnti = (uint16)c_rnti;
              }
              else
              {
                tdsrrc_pcrc.new_crnti_valid = FALSE;
              }
              /* Now copy OC to TOC */
              tdsrrcllcoc_update_toc_with_oc_from_fach();
              /*Initiate the cell selection*/
              tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR,
                                                       TDSRRC_TRANSITION_FROM_FACH,
                                                       tdsrrc_pcrc.state_indicated_by_msg);
    
        
              tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
     
              /* exit from the switch case */
              break;
            }
    
 #else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
            if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
                tdsrrc_PhysicalChannelReconfiguration_r5_IEs,frequencyInfo)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(pcrc_ptr->frequencyInfo.
                modeSpecificInfo,rrc_FrequencyInfo_modeSpecificInfo_tdd)))           
            {
              directed_cell_info.freq_info_present = TRUE;
              directed_cell_info.uarfcn_dl = pcrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
            }
            else
            {
              directed_cell_info.freq_info_present = FALSE;
            }
            if (tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL5))
            {
              directed_cell_info.cpid_present = TRUE;
              directed_cell_info.cell_param_id = pcrc_ptr->dl_InformationPerRL_Listelem[0].
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
              tdsrrcpcrc_send_initiate_cu_flag = FALSE;
            }
            else
            {
              /*directed cell info do not match. Cell update is required */
              tdsrrcpcrc_send_initiate_cu_flag = TRUE;
              tdsrrcpcrc_delete_crnti          = TRUE;
            }
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
          } /* if CELL_FACH->CELL_FACH reconfiguration */

          /* Call the function that creates and sends a channel
          config request to LLC. */
          tdsrrcpcrc_send_channel_config_req( );
             /* Change the substate since we're waiting for the
             confirmation from LLC for the channel configs */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
        {
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r5_IEs,new_C_RNTI))
          {
            tdsrrc_pcrc.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                                &c_rnti);
            tdsrrc_pcrc.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_pcrc.new_crnti_valid = FALSE;
          }


          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR,
                                                  TDSRRC_TRANSITION_FROM_DCH,
                                                  tdsrrc_pcrc.state_indicated_by_msg);
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

          /* Start T_305 [periodic cell update] timer */
          tdsrrccu_start_t_305_timer();

        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
        {
          tdsrrc_pcrc.directed_cell_info = tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL5);
         /*Send the PCRC complete msg on old configuration */
         tdsrrcpcrc_send_pcrc_complete_message(TRUE);
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
         /*wait for L2ack */
         tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
         /* prints rate for each transport channel */
         tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
        {
          /*Send the PCRC complete msg on old configuration */
          tdsrrcpcrc_send_pcrc_complete_message(TRUE);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
          /*wait for L2ack */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
          /* prints rate for each transport channel */
          tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_FACH) && 
                ((tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
                 (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if ((tdsrrc_pcrc.directed_cell_info == FALSE) &&
            (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
          {
            if(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
            {
              tdsrrc_pcrc.directed_cell_info = tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL5);
            }
          
            tdsrrc_pcrc.initiate_cell_selection_trans_from_fach = TRUE;
          }
          
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

          /* check whether new C-RNTI was present in the message. In that case
          we have to configure MAC with new C-RNTI before sending the response 
          message */
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
             tdsrrc_PhysicalChannelReconfiguration_r5_IEs,new_C_RNTI))
          {
            tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                                &c_rnti);
            /* Send a RNTI Update Request */
            rnti_update_info.crnti_action    = TDSRRC_RNTI_UPDATE;
            rnti_update_info.crnti           = (uint16)c_rnti;
            /* check whether URNTI is also present in the message */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr,
             tdsrrc_PhysicalChannelReconfiguration_r5_IEs,new_U_RNTI))
            {
              tdsrrc_translate_urnti(&pcrc_ptr->new_U_RNTI, 
                                  &u_rnti);
              rnti_update_info.urnti_action = TDSRRC_RNTI_UPDATE; 
              rnti_update_info.urnti        = u_rnti; 
            }
            else
            {
              rnti_update_info.urnti_action = TDSRRC_RNTI_NOCHANGE; 
            }
            rnti_update_info.procedure            = TDSRRC_PROCEDURE_PCR;
            rnti_update_info.rlc_re_establish_srb = FALSE;
            rnti_update_info.rlc_re_establish_rab = FALSE;
            rnti_update_info.cnf_reqd             = TRUE;

            tdsrrcrb_send_rnti_update_req(rnti_update_info);
            tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
          }
          else 
          {
            /* There is no new C-RNTI So send the response on the old configuration
            */
            tdsrrcpcrc_send_pcrc_complete_message(TRUE);
            /*wait for L2ack */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
            tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
            /* prints rate for each transport channel */
            tdsrrcllc_print_trans_chnl_rate();
          }
        }
        break;

      case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
        tdsrrc_pcrc.status.failure_status = TDSRRCPC_SIMULTANEOUS_RECONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
        tdsrrc_pcrc.status.failure_status =TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_pcrc.status.failure_status =TDSRRCPC_MSG_INVALID_CONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
		
      case TDSORDERED_CONFIG_NOT_SET_OTHER:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
        /* When LLC sends this, there is a serious problem
        and we'll go back to disconnected state. No need
        to send the failure message here, we can just
        go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      default:

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
        /* Unknown error, just go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
    }/* end switch */

    if( ordered_config_err_occured == TRUE )
    {
      /* No need to continue. Clear the procedure
      variables and return. */
      tdsrrcpcrc_clear_procedure();
      return;
    }

    if(tdsrrc_pcrc.pcrc_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }
  }
}




/*===========================================================================

FUNCTION   PROCESS_PCRC_R6

DESCRIPTION

  This function processes the received Rel 6 PC Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a PCRC Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to reconfigure the Physical Channel given by the message.
    
    If TDSORDERED_CONFIG could not be set, a PCRC Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_process_pcrc_r6
(
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id,

  tdsrrc_DL_DCCH_Message *dcch_msg_ptr  ,
  /* Pointer to the RB Reconfig message        */
  tdsrrc_PhysicalChannelReconfiguration_r6_IEs *pcrc_ptr
)
{
  tdsrrc_PhysicalChannelReconfiguration *msg_ptr = NULL;		/* Pointer to the RB Setup message        */

  tdsrrcllc_oc_status_e_type             oc_status;/* To hold tdsordered_config status   */
  tdsrrc_state_e_type                    current_state; /* current RRC State  */
  tdsrrc_int_c_rnti_type                 c_rnti;       /* Local C_RNTI value */
  tdsrrc_int_u_rnti_type                 u_rnti;
  tdsrrcrb_directed_cell_info_type       directed_cell_info; /* directed cell info struct */

  tdsrrcrb_rnti_update_info_type rnti_update_info;   /* to hold rnti update info */

  boolean       ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                    occured while setting tdsordered_config. */

  msg_ptr = &dcch_msg_ptr->message.u.physicalChannelReconfiguration;

  if (FAILURE == 
      tdsrrcpcrc_check_and_send_failure_for_simul_reconfig(
        msg_ptr->u.later_than_r3.rrc_TransactionIdentifier))
  {
    return;
  }

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/
  if ((pcrc_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         pcrc_ptr->m.activationTimePresent,
         pcrc_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC SDU Lifetime validation failed");
      tdsrrcpcrc_send_lifetime_validation_failure(TDSRRCPC_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force PCRC Activation time to NOW");
      pcrc_ptr->m.activationTimePresent = 0;
    }
  }

#endif /*FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION*/

  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  tdsrrcpcrc_validate_pcrc_r6( pcrc_ptr );

  if( tdsrrc_pcrc.status.failure_status != TDSRRCPC_MSG_VALID )
  {
    /*  Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC Message validation failed");

    /* Send the PC Reconfig Failure Message */
    tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                              tdsrrc_pcrc.status.prot_err_cause
                            );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
  }
  /* If the message is valid, proceed further */
  else
  {

    TDSRRC_MSG0(MSG_LEGACY_LOW,"Processing PCRC Message");

    /* Check if there is CN information present. If so, we need
    to send the information to the Cell Change manager */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r6_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      tdsrrc_pcrc.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(
                        &(pcrc_ptr->cn_InformationInfo));
 
      if (tdsrrc_pcrc.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_pcrc.cn_info_saved = TRUE;
      }
    }

    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.r6,
        physicalChannelReconfiguration_r6_add_ext))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: physicalChannelReconfiguration_r6_add_ext not yet supported");
    } 

    if (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.r6.m.v6b0NonCriticalExtensionsPresent)
    {
 
      tdsrrcpcrc_process_high_mobility_ind_in_non_critical_ext(msg_ptr,TDSMSG_REL6);
         
    }
        
    /* Need to send reconfig IE info. First set TDSORDERED_CONFIG
    so that LLC can process the necessary parameters. */
    oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_PCR,
                                           tdsrrc_pcrc.state_indicated_by_msg,
                                           tdsrrc_DL_DCCH_Message_PDU,
                                           (void *)dcch_msg_ptr );

    switch( oc_status )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"oc_status is %d ",oc_status);
      case TDSORDERED_CONFIG_SET:
        current_state = tdsrrc_get_state(); 
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
        if((tdsrrcsib_is_current_event_sib_read_in_dch()) &&
           (TDSRRC_PROCEDURE_GPS == tdsrrcsib_get_proc_id_for_sib_read_in_dch()))
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Force Init GPS ");
           tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
        }


#endif 
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
       /* Populate the local variables to indicate if Re-direction info is present */
        directed_cell_info.freq_info_present = FALSE;
        directed_cell_info.cpid_present = FALSE;
       
        if((current_state==TDSRRC_STATE_CELL_FACH) && 
               ((tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
               ||(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
               ||(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
          
          if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
                    tdsrrc_PhysicalChannelReconfiguration_r6_IEs,frequencyInfo)) &&
                   (TDSRRC_CHECK_COMMON_MSG_TYPE(pcrc_ptr->frequencyInfo.
                    modeSpecificInfo,rrc_FrequencyInfo_modeSpecificInfo_tdd)))           
          {
            directed_cell_info.freq_info_present = TRUE;
            directed_cell_info.uarfcn_dl = pcrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
          }
          
          if (tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL6))
          {
            directed_cell_info.cpid_present = TRUE;
            directed_cell_info.cell_param_id = pcrc_ptr->dl_InformationPerRL_List.elem[0].
                                      modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          }     
          /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
           FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
          if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
             (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
            if(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
            {
              tdsrrcpcrc_send_initiate_cu_flag = TRUE;
              tdsrrcpcrc_delete_crnti          = TRUE;
            }
            else
            {
              tdsrrc_pcrc.directed_cell_info = TRUE;
            }
          }
        }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

        if (
             (current_state==tdsrrc_pcrc.state_indicated_by_msg)||
             ((current_state==TDSRRC_STATE_CELL_FACH)&& 
              (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH))
            )
        {
          /*  CellDCH<->CellDCH or CellFACH<->CellFACH or CellFACH->CellDCH 
          transition. Ordered Config has been set. We can setup the
          Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");

          /*check whether any frequency info is present in the message
          and transition is CELL_FACH->CELL_FACH. In this case
          initiate a cell update after channel config is successfully completed */
          if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
             (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
          {
   #ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
     
            /*call the function that evaluates the directed cell info and set the flags accordingly */
            if ((tdsrrcpcrc_send_initiate_cu_flag == FALSE)&&
                (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
            {
              if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r6_IEs,new_C_RNTI))
              {
                tdsrrc_pcrc.new_crnti_valid = TRUE;
                tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                                    &c_rnti);
                tdsrrc_pcrc.new_crnti = (uint16)c_rnti;
              }
              else
              {
                tdsrrc_pcrc.new_crnti_valid = FALSE;
              }
   
              tdsrrcllcoc_update_toc_with_oc_from_fach();
                 /*Initiate the cell selection*/
              tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR,
                                                      TDSRRC_TRANSITION_FROM_FACH,
                                                      tdsrrc_pcrc.state_indicated_by_msg);
   
   
            
            
              tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
     
               /* exit from the switch case */
              break;
            }
    
   #else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
            if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
                tdsrrc_PhysicalChannelReconfiguration_r6_IEs,frequencyInfo)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(pcrc_ptr->frequencyInfo.
                modeSpecificInfo,rrc_FrequencyInfo_modeSpecificInfo_tdd)))           
            {
              directed_cell_info.freq_info_present = TRUE;
              directed_cell_info.uarfcn_dl = pcrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
            }
            else
            {
              directed_cell_info.freq_info_present = FALSE;
            }
            if (tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL6))
            {
              directed_cell_info.cpid_present = TRUE;
              directed_cell_info.cell_param_id = pcrc_ptr->dl_InformationPerRL_List.elem[0].
                                        modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
            }
            else
            {
              directed_cell_info.cpid_present = FALSE;
            }
            /*call the function that evaluates the directed cell info and set the flags accordingly */
            if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
            {
              /*directed cell info matched. Cell update is not required */
              tdsrrcpcrc_send_initiate_cu_flag = FALSE;
            }
            else
            {
              /*directed cell info do not match. Cell update is required */
              tdsrrcpcrc_send_initiate_cu_flag = TRUE;
              tdsrrcpcrc_delete_crnti          = TRUE;
            }
   #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
          } /* if CELL_FACH->CELL_FACH reconfiguration */

          /* Call the function that creates and sends a channel
          config request to LLC. */
          tdsrrcpcrc_send_channel_config_req( );
             /* Change the substate since we're waiting for the
             confirmation from LLC for the channel configs */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
        {
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r6_IEs,new_C_RNTI))
          {
            tdsrrc_pcrc.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                                &c_rnti);
            tdsrrc_pcrc.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_pcrc.new_crnti_valid = FALSE;
          }

          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR,
                                                  TDSRRC_TRANSITION_FROM_DCH,
                                                  TDSRRC_STATE_CELL_FACH);
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

          /* Start T_305 [periodic cell update] timer */
          tdsrrccu_start_t_305_timer();		  
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
        {
          tdsrrc_pcrc.directed_cell_info = tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL6);
         /*Send the PCRC complete msg on old configuration */
         tdsrrcpcrc_send_pcrc_complete_message(TRUE);
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
         /*wait for L2ack */
         tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
         /* prints rate for each transport channel */
         tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
        {
          /*Send the PCRC complete msg on old configuration */
          tdsrrcpcrc_send_pcrc_complete_message(TRUE);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
          /*wait for L2ack */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
          /* prints rate for each transport channel */
          tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_FACH) && 
                ((tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
                 (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if ((tdsrrc_pcrc.directed_cell_info == FALSE) &&
            (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
          {
            if(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
            {
              tdsrrc_pcrc.directed_cell_info = tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL6);
            }
       
            tdsrrc_pcrc.initiate_cell_selection_trans_from_fach = TRUE;
          }

 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

          /* check whether new C-RNTI was present in the message. In that case
          we have to configure MAC with new C-RNTI before sending the response 
          message */
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
             tdsrrc_PhysicalChannelReconfiguration_r6_IEs,new_C_RNTI))
          {
            tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                                &c_rnti);
            /* Send a RNTI Update Request */
            rnti_update_info.crnti_action    = TDSRRC_RNTI_UPDATE;
            rnti_update_info.crnti           = (uint16)c_rnti;
            /* check whether URNTI is also present in the message */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr,
             tdsrrc_PhysicalChannelReconfiguration_r6_IEs,new_U_RNTI))
            {
              tdsrrc_translate_urnti(&pcrc_ptr->new_U_RNTI, 
                                  &u_rnti);
              rnti_update_info.urnti_action = TDSRRC_RNTI_UPDATE; 
              rnti_update_info.urnti        = u_rnti; 
            }
            else
            {
              rnti_update_info.urnti_action = TDSRRC_RNTI_NOCHANGE; 
            }
            rnti_update_info.procedure            = TDSRRC_PROCEDURE_PCR;
            rnti_update_info.rlc_re_establish_srb = FALSE;
            rnti_update_info.rlc_re_establish_rab = FALSE;
            rnti_update_info.cnf_reqd             = TRUE;

            tdsrrcrb_send_rnti_update_req(rnti_update_info);
            tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
          }
          else 
          {
            /* There is no new C-RNTI So send the response on the old configuration
            */
            tdsrrcpcrc_send_pcrc_complete_message(TRUE);
            /*wait for L2ack */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
            tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
            /* prints rate for each transport channel */
            tdsrrcllc_print_trans_chnl_rate();
          }
        }
        break;

      case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
        tdsrrc_pcrc.status.failure_status = TDSRRCPC_SIMULTANEOUS_RECONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
        tdsrrc_pcrc.status.failure_status =TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_pcrc.status.failure_status =TDSRRCPC_MSG_INVALID_CONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
		
      case TDSORDERED_CONFIG_NOT_SET_OTHER:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
        /* When LLC sends this, there is a serious problem
        and we'll go back to disconnected state. No need
        to send the failure message here, we can just
        go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      default:

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
        /* Unknown error, just go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
    }/* end switch */

    if( ordered_config_err_occured == TRUE )
    {
      /* No need to continue. Clear the procedure
      variables and return. */
      tdsrrcpcrc_clear_procedure();
      return;
    }

    if(tdsrrc_pcrc.pcrc_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }
  }
}



/*===========================================================================

FUNCTION   PROCESS_PCRC_R7

DESCRIPTION

  This function processes the received Rel 6 PC Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a PCRC Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to reconfigure the Physical Channel given by the message.
    
    If TDSORDERED_CONFIG could not be set, a PCRC Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_process_pcrc_r7
(
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id,

  tdsrrc_DL_DCCH_Message *dcch_msg_ptr  ,
  /* Pointer to the RB Reconfig message        */
  tdsrrc_PhysicalChannelReconfiguration_r7_IEs *pcrc_ptr
)
{
  tdsrrc_PhysicalChannelReconfiguration *msg_ptr = NULL;		/* Pointer to the RB Setup message        */

  tdsrrcllc_oc_status_e_type             oc_status;/* To hold tdsordered_config status   */
  tdsrrc_state_e_type                    current_state; /* current RRC State  */
  tdsrrc_int_c_rnti_type                 c_rnti;       /* Local C_RNTI value */
  tdsrrc_int_u_rnti_type                 u_rnti;
  tdsrrcrb_directed_cell_info_type       directed_cell_info; /* directed cell info struct */

  tdsrrcrb_rnti_update_info_type rnti_update_info;   /* to hold rnti update info */

  boolean       ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                    occured while setting tdsordered_config. */

  msg_ptr = &dcch_msg_ptr->message.u.physicalChannelReconfiguration;
TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received PCRC msg rel 7,substate %d",tdsrrc_pcrc.curr_substate);
  if (FAILURE == 
      tdsrrcpcrc_check_and_send_failure_for_simul_reconfig(
        msg_ptr->u.later_than_r3.rrc_TransactionIdentifier))
  {
    return;
  }

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/
  if ((pcrc_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         pcrc_ptr->m.activationTimePresent,
         pcrc_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC SDU Lifetime validation failed");
      tdsrrcpcrc_send_lifetime_validation_failure(TDSRRCPC_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force PCRC Activation time to NOW");
      pcrc_ptr->m.activationTimePresent = 0;
    }
  }

#endif /*FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION*/

  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  tdsrrcpcrc_validate_pcrc_r7( pcrc_ptr );

  if( tdsrrc_pcrc.status.failure_status != TDSRRCPC_MSG_VALID )
  {
    /*  Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC Message validation failed");

    /* Send the PC Reconfig Failure Message */
    tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                              tdsrrc_pcrc.status.prot_err_cause
                            );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
  }
  /* If the message is valid, proceed further */
  else
  {

    TDSRRC_MSG0(MSG_LEGACY_LOW,"Processing PCRC Message");

    /* Check if there is CN information present. If so, we need
    to send the information to the Cell Change manager */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r7_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      tdsrrc_pcrc.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(
                        &(pcrc_ptr->cn_InformationInfo));
 
      if (tdsrrc_pcrc.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_pcrc.cn_info_saved = TRUE;
      }
    }
      
 
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r7_IEs,ueMobilityStateIndicator))
    {
      if(pcrc_ptr->ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in PCRC.");
        tdsrrc_pcrc.high_mobility_ind = TRUE;
      }
    }


    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7,
        physicalChannelReconfiguration_r7_add_ext))
       
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: rrc_physicalChannelReconfiguration_r7_add_ext_present not yet supported");
    } 

    /* Need to send reconfig IE info. First set TDSORDERED_CONFIG
    so that LLC can process the necessary parameters. */
    oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_PCR,
                                           tdsrrc_pcrc.state_indicated_by_msg,
                                           tdsrrc_DL_DCCH_Message_PDU,
                                           (void *)dcch_msg_ptr );

    switch( oc_status )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"oc_status is %d ",oc_status);
      case TDSORDERED_CONFIG_SET:
        current_state = tdsrrc_get_state(); 
        
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
        if(tdsrrcsib_is_current_event_sib_read_in_dch())
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Force Init GPS ");
          tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
        }
#endif
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
        /* Populate the local variables to indicate if Re-direction info is present */
        directed_cell_info.freq_info_present = FALSE;
        directed_cell_info.cpid_present = FALSE;
        if((current_state==TDSRRC_STATE_CELL_FACH) && 
                 ((tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
                 ||(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
                 ||(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
           
          if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
              tdsrrc_PhysicalChannelReconfiguration_r7_IEs,frequencyInfo)) &&
             (TDSRRC_CHECK_COMMON_MSG_TYPE(pcrc_ptr->frequencyInfo.
              modeSpecificInfo,rrc_FrequencyInfo_modeSpecificInfo_tdd)))           
          {
            directed_cell_info.freq_info_present = TRUE;
            directed_cell_info.uarfcn_dl = pcrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
          }
          
          if (tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL7))
          {
            directed_cell_info.cpid_present = TRUE;
            directed_cell_info.cell_param_id = pcrc_ptr->dl_InformationPerRL_List.elem[0].
                                      modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
          }
          /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
          FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
          if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
            (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
            if(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
            {
              tdsrrcpcrc_send_initiate_cu_flag = TRUE;
              tdsrrcpcrc_delete_crnti          = TRUE;
            }
            else
            {
              tdsrrc_pcrc.directed_cell_info = TRUE;
            }
          }          
        }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
        if (
            (current_state==tdsrrc_pcrc.state_indicated_by_msg)||
            ((current_state==TDSRRC_STATE_CELL_FACH)&& 
             (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH))
           )
        {
          /*  CellDCH<->CellDCH or CellFACH<->CellFACH or CellFACH->CellDCH 
          transition. Ordered Config has been set. We can setup the
          Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");

          /*check whether any frequency info is present in the message
          and transition is CELL_FACH->CELL_FACH. In this case
          initiate a cell update after channel config is successfully completed */
          if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
             (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
          {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
     
            /*call the function that evaluates the directed cell info and set the flags accordingly */
            if ((tdsrrcpcrc_send_initiate_cu_flag == FALSE)&&
                (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
            {
    
              if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r7_IEs,new_C_RNTI))
              {
                tdsrrc_pcrc.new_crnti_valid = TRUE;
                tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                                    &c_rnti);
                tdsrrc_pcrc.new_crnti = (uint16)c_rnti;
              }
              else
              {
                tdsrrc_pcrc.new_crnti_valid = FALSE;
              }
              /* Now copy OC to TOC */
              tdsrrcllcoc_update_toc_with_oc_from_fach();
              /*Initiate the cell selection*/
              tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR,
                                                       TDSRRC_TRANSITION_FROM_FACH,
                                                       tdsrrc_pcrc.state_indicated_by_msg);
    
        
              tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
     
              /* exit from the switch case */
              break;
            }
    
 #else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
            if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
                tdsrrc_PhysicalChannelReconfiguration_r7_IEs,frequencyInfo)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(pcrc_ptr->frequencyInfo.
                modeSpecificInfo,rrc_FrequencyInfo_modeSpecificInfo_tdd))  )         
            {
              directed_cell_info.freq_info_present = TRUE;
              directed_cell_info.uarfcn_dl = pcrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
            }
            else
            {
              directed_cell_info.freq_info_present = FALSE;
            }
            if (tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL7))
            {
              directed_cell_info.cpid_present = TRUE;
              directed_cell_info.cell_param_id = pcrc_ptr->dl_InformationPerRL_List.elem[0].
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
              tdsrrcpcrc_send_initiate_cu_flag = FALSE;
            }
            else
            {
              /*directed cell info do not match. Cell update is required */
              tdsrrcpcrc_send_initiate_cu_flag = TRUE;
              tdsrrcpcrc_delete_crnti          = TRUE;
            }
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
          } /* if CELL_FACH->CELL_FACH reconfiguration */

          /* Call the function that creates and sends a channel
          config request to LLC. */
          tdsrrcpcrc_send_channel_config_req( );
             /* Change the substate since we're waiting for the
             confirmation from LLC for the channel configs */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
        {
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r7_IEs,new_C_RNTI))
          {
            tdsrrc_pcrc.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                                &c_rnti);
            tdsrrc_pcrc.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_pcrc.new_crnti_valid = FALSE;
          }

          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR,
                                                  TDSRRC_TRANSITION_FROM_DCH,
                                                  TDSRRC_STATE_CELL_FACH);
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

          /* Start T_305 [periodic cell update] timer */
          tdsrrccu_start_t_305_timer();		  
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
        {
          tdsrrc_pcrc.directed_cell_info = tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL7);
         /*Send the PCRC complete msg on old configuration */
         tdsrrcpcrc_send_pcrc_complete_message(TRUE);
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
         /*wait for L2ack */
         tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
         /* prints rate for each transport channel */
         tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
        {
          /*Send the PCRC complete msg on old configuration */
          tdsrrcpcrc_send_pcrc_complete_message(TRUE);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
          /*wait for L2ack */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
          /* prints rate for each transport channel */
          tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_FACH) && 
                ((tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
                 (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if ((tdsrrc_pcrc.directed_cell_info == FALSE) &&
            (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
          {
            if(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
            {
              tdsrrc_pcrc.directed_cell_info = tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL7);
            }
          
            tdsrrc_pcrc.initiate_cell_selection_trans_from_fach = TRUE;
          }
          
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

          /* check whether new C-RNTI was present in the message. In that case
          we have to configure MAC with new C-RNTI before sending the response 
          message */
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
             tdsrrc_PhysicalChannelReconfiguration_r7_IEs,new_C_RNTI))
          {
            tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                                &c_rnti);
            /* Send a RNTI Update Request */
            rnti_update_info.crnti_action    = TDSRRC_RNTI_UPDATE;
            rnti_update_info.crnti           = (uint16)c_rnti;
            /* check whether URNTI is also present in the message */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr,
             tdsrrc_PhysicalChannelReconfiguration_r7_IEs,new_U_RNTI))
            {
              tdsrrc_translate_urnti(&pcrc_ptr->new_U_RNTI, 
                                  &u_rnti);
              rnti_update_info.urnti_action = TDSRRC_RNTI_UPDATE; 
              rnti_update_info.urnti        = u_rnti; 
            }
            else
            {
              rnti_update_info.urnti_action = TDSRRC_RNTI_NOCHANGE; 
            }
            rnti_update_info.procedure            = TDSRRC_PROCEDURE_PCR;
            rnti_update_info.rlc_re_establish_srb = FALSE;
            rnti_update_info.rlc_re_establish_rab = FALSE;
            rnti_update_info.cnf_reqd             = TRUE;

            tdsrrcrb_send_rnti_update_req(rnti_update_info);
            tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
          }
          else 
          {
            /* There is no new C-RNTI So send the response on the old configuration
            */
            tdsrrcpcrc_send_pcrc_complete_message(TRUE);
            /*wait for L2ack */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
            tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
            /* prints rate for each transport channel */
            tdsrrcllc_print_trans_chnl_rate();
          }
        }
        break;

      case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
        tdsrrc_pcrc.status.failure_status = TDSRRCPC_SIMULTANEOUS_RECONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
        tdsrrc_pcrc.status.failure_status =TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_pcrc.status.failure_status =TDSRRCPC_MSG_INVALID_CONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
      
      case TDSORDERED_CONFIG_NOT_SET_OTHER:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
        /* When LLC sends this, there is a serious problem
        and we'll go back to disconnected state. No need
        to send the failure message here, we can just
        go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      default:

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
        /* Unknown error, just go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
    }/* end switch */

    if( ordered_config_err_occured == TRUE )
    {
      /* No need to continue. Clear the procedure
      variables and return. */
      tdsrrcpcrc_clear_procedure();
      return;
    }

    if(tdsrrc_pcrc.pcrc_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }
  }
}

#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================
FUNCTION   tdsrrcpcrc_process_after_oc_config_set_for_fach_to_fach_r8

DESCRIPTION
  This function processes the R8 PCRC OTA after OC is set by this OTA and When FACH to FACH
  state transition is triggered by the OTA.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcpcrc_process_after_oc_config_set_for_fach_to_fach_r8
(
  tdsrrc_PhysicalChannelReconfiguration * msg_ptr,
  tdsrrcrb_directed_cell_info_type  directed_cell_info
)
{
  tdsrrc_int_c_rnti_type                 c_rnti;           /* Local C_RNTI value */
  tdsrrc_PhysicalChannelReconfiguration_r8_IEs *pcrc_ptr = NULL;

  pcrc_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
          criticalExtensions.u.criticalExtensions.u.r8.physicalChannelReconfiguration_r8;

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU

  /*call the function that evaluates the directed cell info and set the flags accordingly */
  if ((tdsrrcpcrc_send_initiate_cu_flag == FALSE)&&
    (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
  {
    if(TDSRRCRB_R8_MSG_IE_PRESENT(pcrc_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,new_C_RNTI))
    {
      tdsrrc_pcrc.new_crnti_valid = TRUE;
      tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, &c_rnti);
      tdsrrc_pcrc.new_crnti = (uint16)c_rnti;
    }
    else
    {
      tdsrrc_pcrc.new_crnti_valid = FALSE;
    }

    /* Now copy OC to TOC */
    tdsrrcllcoc_update_toc_with_oc_from_fach();
    /*Initiate the cell selection*/
    tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR, 
                   TDSRRC_TRANSITION_FROM_FACH, tdsrrc_pcrc.state_indicated_by_msg);
  
    tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
  }

#else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
    /*check whether any frequency info is present in the message
    and transition is CELL_FACH->CELL_FACH. In this case
    initiate a cell update after channel config is successfully completed */
  if((TDSRRCRB_R8_MSG_IE_PRESENT(pcrc_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,frequencyInfo)) &&
    (TDSRRC_CHECK_COMMON_MSG_TYPE(pcrc_ptr->frequencyInfo.modeSpecificInfo,
    rrc_FrequencyInfo_modeSpecificInfo_tdd)) )          
  {
    directed_cell_info.freq_info_present = TRUE;
    directed_cell_info.uarfcn_dl = pcrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
  }
  else
  {
    directed_cell_info.freq_info_present = FALSE;
  }
  if (tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL8))
  {
    directed_cell_info.cpid_present = TRUE;
    directed_cell_info.cell_param_id = pcrc_ptr->dl_InformationPerRL_List.elem[0].
                              modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
  }
  else
  {
    directed_cell_info.cpid_present = FALSE;
  }
  /*call the function that evaluates the directed cell info and set the flags accordingly */
  if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
  {
    /*directed cell info matched. Cell update is not required */
    tdsrrcpcrc_send_initiate_cu_flag = FALSE;
  }
  else
  {
    /*directed cell info do not match. Cell update is required */
    tdsrrcpcrc_send_initiate_cu_flag = TRUE;
    tdsrrcpcrc_delete_crnti          = TRUE;
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");

  /* Call the function that creates and sends a channel config request to LLC. */
  tdsrrcpcrc_send_channel_config_req( );

  /* Change the substate since we're waiting for the confirmation from LLC for the channel configs */
  tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;

#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

  return;
}

/*===========================================================================
FUNCTION   tdsrrcpcrc_process_after_oc_config_set_r8

DESCRIPTION
  This function processes the R8 PCRC OTA after OC is set by this OTA. So if we call this function, it means
  that TDSORDERED_CONFIG_SET is returned while setting OC by PCRC OTA.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcpcrc_process_after_oc_config_set_r8
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr,
  tdsrrc_PhysicalChannelReconfiguration_r8_IEs *pcrc_ptr  
)
{
  tdsrrc_int_c_rnti_type                 c_rnti;           /* Local C_RNTI value */
  tdsrrc_int_u_rnti_type                 u_rnti;           /* Local U_RNTI value*/
  tdsrrc_state_e_type                    current_state;              /* current RRC State  */  
  tdsrrcrb_directed_cell_info_type  directed_cell_info;        /* directed cell info struct */
  tdsrrcrb_rnti_update_info_type    rnti_update_info;             /* to hold rnti update info */  
  tdsrrc_PhysicalChannelReconfiguration *msg_ptr = NULL;  /* Pointer to the RB Setup message */

  current_state = tdsrrc_get_state(); 

  msg_ptr = &dcch_msg_ptr->message.u.physicalChannelReconfiguration;
  
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
  if(tdsrrcsib_is_current_event_sib_read_in_dch())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Force Init GPS ");
    tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
  }
#endif /*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /* Populate the local variables to indicate if Re-direction info is present */
  directed_cell_info.freq_info_present = FALSE;
  directed_cell_info.cpid_present = FALSE;

  if((current_state==TDSRRC_STATE_CELL_FACH) && 
      ((tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
      ||(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
      ||(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
  {
    if((TDSRRCRB_R8_MSG_IE_PRESENT(pcrc_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,frequencyInfo)) &&
      (TDSRRC_CHECK_COMMON_MSG_TYPE(pcrc_ptr->frequencyInfo.modeSpecificInfo,
      rrc_FrequencyInfo_modeSpecificInfo_tdd)))           
    {
      directed_cell_info.freq_info_present = TRUE;
      directed_cell_info.uarfcn_dl = pcrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
    }
    if (tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL8))
    {
      directed_cell_info.cpid_present = TRUE;
      directed_cell_info.cell_param_id = pcrc_ptr->dl_InformationPerRL_List.elem[0].
                              modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
    }
    /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
    FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
    if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
       (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
      if(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
      {
        tdsrrcpcrc_send_initiate_cu_flag = TRUE;
        tdsrrcpcrc_delete_crnti          = TRUE;
      }
      else
      {
        tdsrrc_pcrc.directed_cell_info = TRUE;
      }
    }
  }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

  /* CellDCH<->CellDCH or CellFACH<->CellFACH or CellFACH->CellDCH transition. */
  if((current_state==tdsrrc_pcrc.state_indicated_by_msg)||((current_state==TDSRRC_STATE_CELL_FACH) && 
    (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH)))
  {
    if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
     (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
    {
      tdsrrcpcrc_process_after_oc_config_set_for_fach_to_fach_r8(msg_ptr, directed_cell_info);
      return;
    }/* if CELL_FACH->CELL_FACH reconfiguration */

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");

    /* Call the function that creates and sends a channel config request to LLC. */
    tdsrrcpcrc_send_channel_config_req( );

    /* Change the substate since we're waiting for the confirmation from LLC for the channel configs */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
  }

  /*Cell_DCH -> Cell_FACH*/
  else if((current_state==TDSRRC_STATE_CELL_DCH) && 
    (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
  {
    if(TDSRRCRB_R8_MSG_IE_PRESENT(pcrc_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,new_C_RNTI))
    {
      tdsrrc_pcrc.new_crnti_valid = TRUE;
      tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, &c_rnti);
      tdsrrc_pcrc.new_crnti = (uint16)c_rnti;
    }
    else
    {
      tdsrrc_pcrc.new_crnti_valid = FALSE;
    }

    /*Initiate the cell selection*/
    tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR,
                  TDSRRC_TRANSITION_FROM_DCH, TDSRRC_STATE_CELL_FACH);
    tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
    
    /* Start T_305 [periodic cell update] timer */
    tdsrrccu_start_t_305_timer();
  }

  /*Cell_DCH -> Cell_PCH*/
  else if((current_state==TDSRRC_STATE_CELL_DCH) && 
      (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
  {
    tdsrrc_pcrc.directed_cell_info = tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL8);

    /*Send the PCRC complete msg on old configuration */
    tdsrrcpcrc_send_pcrc_complete_message(TRUE);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");

    /*wait for L2ack */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    /* prints rate for each transport channel */
    tdsrrcllc_print_trans_chnl_rate();
  }

  /*Cell_DCH -> URA_PCH*/
  else if((current_state==TDSRRC_STATE_CELL_DCH) && 
      (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
  {
    /*Send the PCRC complete msg on old configuration */
    tdsrrcpcrc_send_pcrc_complete_message(TRUE);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
    /*wait for L2ack */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    /* prints rate for each transport channel */
    tdsrrcllc_print_trans_chnl_rate();
  }

  /*Cell_FACH -> Cell_PCH or Cell_FACH ->URA_PCH*/
  else if((current_state==TDSRRC_STATE_CELL_FACH) && 
      ((tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
  {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
    /*call the function that evaluates the directed cell info and set the flags accordingly */
    if((tdsrrc_pcrc.directed_cell_info == FALSE) &&
      (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
    {
      if(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
      {
        tdsrrc_pcrc.directed_cell_info = tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL8);
      }
      tdsrrc_pcrc.initiate_cell_selection_trans_from_fach = TRUE;
    }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
  
    /* check whether new C-RNTI was present in the message. In that case
    we have to configure MAC with new C-RNTI before sending the response message */
    if(TDSRRCRB_R8_MSG_IE_PRESENT(pcrc_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,new_C_RNTI))
    {
      tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, &c_rnti);

      /* Send a RNTI Update Request */
      rnti_update_info.crnti_action    = TDSRRC_RNTI_UPDATE;
      rnti_update_info.crnti           = (uint16)c_rnti;

      /* check whether URNTI is also present in the message */
      if(TDSRRCRB_R8_MSG_IE_PRESENT(pcrc_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,new_U_RNTI))
      {
        tdsrrc_translate_urnti(&pcrc_ptr->new_U_RNTI, &u_rnti);
        rnti_update_info.urnti_action = TDSRRC_RNTI_UPDATE; 
        rnti_update_info.urnti        = u_rnti; 
      }
      else
      {
        rnti_update_info.urnti_action = TDSRRC_RNTI_NOCHANGE; 
      }
      rnti_update_info.procedure            = TDSRRC_PROCEDURE_PCR;
      rnti_update_info.rlc_re_establish_srb = FALSE;
      rnti_update_info.rlc_re_establish_rab = FALSE;
      rnti_update_info.cnf_reqd             = TRUE;

      tdsrrcrb_send_rnti_update_req(rnti_update_info);
      tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
    }
    else 
    {
      /* There is no new C-RNTI So send the response on the old configuration */
      tdsrrcpcrc_send_pcrc_complete_message(TRUE);
      /*wait for L2ack */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
      tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
      /* prints rate for each transport channel */
      tdsrrcllc_print_trans_chnl_rate();
    }
  }
  return;
}

/*===========================================================================
FUNCTION   tdsrrcpcrc_validate_pcrc_r8

DESCRIPTION
  This function validates the received PC Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a PCRC Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcpcrc_validate_pcrc_r8
(
  tdsrrc_PhysicalChannelReconfiguration_r8_IEs *msg_ptr /* Pointer to the PCRC message */
)
{
  uecomdef_status_e_type status = FAILURE;
  /* Assume the message is valid for now */
  tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_VALID;

  /* Check the RRC State indicatior given by the message */
  switch( msg_ptr->rrc_StateIndicator )
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;

    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;

    default:
      /* No other states are supported right now. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported state in PC Reconfig Message");
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
      break;
  }

  if( tdsrrc_pcrc.status.failure_status == TDSRRCPC_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
     return;
  }

  /*RRC_GKG: We can put a check for HSDPA, HSUPA for TDD and return failure.*/
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
      tdsrrc_PhysicalChannelReconfiguration_r8_IEs,dl_HSPDSCH_Information))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSPDSCH Info present");
    
    /* Check if network does not try to initiate HSDPA with invalid state */
    status = tdsrrc_hsdpa_initial_validation(msg_ptr->rrc_StateIndicator, 
          TDSRRC_CHECK_COMMON_MSG_TYPE(msg_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
          rrc_DL_HSPDSCH_Information_r8_modeSpecificInfo_fdd));
    
    if (status == FAILURE)
    {
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_INVALID_CONFIG;
      return;
    }
  }

   /* Check if RB with PDCP Information list is present. This
  is not supported in the first release. This may be supported
  later */                                    
  
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if(TDSRRCRB_R8_MSG_IE_PRESENT(msg_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,
    dl_CounterSynchronisationInfo))
#else
  if(tdsrrcpcrc_validate_srns_relocation_info_new(msg_ptr, TDSMSG_REL8))
#endif

  {
    /*check whether the SRNS relocation is within Cell_DCH or not.
     SRNS relocation information can come in reconfiguration messages
     only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"PCRC : SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_pcrc.state_indicated_by_msg);
      tdsrrc_pcrc.status.failure_status=TDSRRCPC_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has 
    PDCP info list. If it is true then this configuration is unsupported */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->dl_CounterSynchronisationInfo,
		rb_WithPDCP_InfoList)) ||
        (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->dl_CounterSynchronisationInfo,
         tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
      tdsrrc_pcrc.status.failure_status=TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation. Set the flag that indicates that this procedure
      involves SRNS relocation */
      tdsrrc_pcrc.pcrc_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */
      if(TDSRRCRB_R8_MSG_IE_PRESENT(msg_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,
        new_U_RNTI))
      {
        tdsrrc_pcrc.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(msg_ptr->new_U_RNTI), &tdsrrc_pcrc.new_urnti);
      }
    }
  }

    /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  tdsrrc_pcrc.trans_to_dch_ciph_config_needed = FALSE;

  if((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
    (tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRCRB_R8_MSG_IE_PRESENT(msg_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,
      dl_CommonInformation)) && (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->dl_CommonInformation,
      tdsrrc_DL_CommonInformation_r8,dl_dpchInfoCommon)) && (T_tdsrrc_DL_CommonInformation_r8_dl_dpchInfoCommon_dl_DPCH_InfoCommon
      == msg_ptr->dl_CommonInformation.dl_dpchInfoCommon.t))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");

        loc_ret_val = tdsrrcrb_get_mac_d_hfn(( 
          TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon,
          tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
          &msg_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
          &mac_d_hfn_l);

        if (loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"SMC process ciphering config for HHO failed");
        }
        tdsrrc_pcrc.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
    /* If UTRAN DRX coefficient is not present then it is invalid configuration */
    if (!(TDSRRCRBR_R7_MSG_IE_PRESENT(msg_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,
      utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_pcrc.state_indicated_by_msg);
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_INVALID_CONFIG;
      return;
    }

    if (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      /*should we do it after getting l2ack for response??*/
      if (!(msg_ptr->m.ura_IdentityPresent))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
        tdsrrccu_clear_ura_id ();
      }
      else
      {
        /*URA ID present in the message set ura_identity variable with this value */
        if (msg_ptr->ura_Identity.numbits == 16)
        {
          tdsrrccu_update_ura_id (&(msg_ptr->ura_Identity));
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",msg_ptr->ura_Identity.numbits);
          tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_PROTOCOL_ERROR;
          tdsrrc_pcrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
          return;
        }
      }
    }
  }
}/* end tdsrrcpcrc_validate_pcrc_r7() */

/*===========================================================================
FUNCTION   PROCESS_PCRC_R8

DESCRIPTION
  This function processes the received Rel 8 PC Reconfig Message. It first validates the message. 
  If the message has an invalid or unsupported configuration, a PCRC Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -

    TDSORDERED_CONFIG will be set by calling LLC. 

    If TDSORDERED_CONFIG could be set without any conflicts, a TDSRRC_CHANNEL_CONFIG_REQ 
    will be sent to LLC to reconfigure the Physical Channel given by the message.
    
    If TDSORDERED_CONFIG could not be set, a PCRC Failure message
    will be sent to UTRAN.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcpcrc_process_pcrc_r8
(
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id,
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr  ,
  tdsrrc_PhysicalChannelReconfiguration_r8_IEs *pcrc_ptr   /* Pointer to the R8 PC Reconfig message */
)
{
  tdsrrc_PhysicalChannelReconfiguration *msg_ptr = NULL;  /* Pointer to the RB Setup message */
  tdsrrcllc_oc_status_e_type          oc_status;                     /* To hold tdsordered_config status   */

  /* Used to indicate if an error occured while setting tdsordered_config. */
  boolean       ordered_config_err_occured = FALSE; 
  
  msg_ptr = &dcch_msg_ptr->message.u.physicalChannelReconfiguration;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Received PC-Reconfig OTA,PCRC-substate %d",tdsrrc_pcrc.curr_substate);

  if (FAILURE == tdsrrcpcrc_check_and_send_failure_for_simul_reconfig(
        msg_ptr->u.later_than_r3.rrc_TransactionIdentifier))
  {
    return;
  }

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/
  if ((pcrc_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         pcrc_ptr->m.activationTimePresent,
         pcrc_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC SDU Lifetime validation failed");
      tdsrrcpcrc_send_lifetime_validation_failure(TDSRRCPC_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force PCRC Activation time to NOW");
      pcrc_ptr->m.activationTimePresent = 0;
    }
  }

#endif /*FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION*/

  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  tdsrrcpcrc_validate_pcrc_r8( pcrc_ptr );

  if( tdsrrc_pcrc.status.failure_status != TDSRRCPC_MSG_VALID )
  {
    /*  Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: PCRC Message validation failed");

    /* Send the PC Reconfig Failure Message */
    tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status, tdsrrc_pcrc.status.prot_err_cause);

    /* Make sure we're back in the initial sub-state */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
  }
  /* If the message is valid, proceed further */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"Processing PCRC Message");

    /* Check if there is CN information present. If so, we need to send the information to the 
    Cell Change manager */
    if(TDSRRCRB_R8_MSG_IE_PRESENT(pcrc_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,
      cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      tdsrrc_pcrc.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(
          &(pcrc_ptr->cn_InformationInfo));
 
      if (tdsrrc_pcrc.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_pcrc.cn_info_saved = TRUE;
      }
    }

    /*Check whether Mobility State Indicator is present in OTA, if yes, then store it PCRC global.
    Later we will inform L1 about this indicator.*/
    if(TDSRRCRB_R8_MSG_IE_PRESENT(pcrc_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,
      ueMobilityStateIndicator))
    {
      if(pcrc_ptr->ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in PCRC.");
        tdsrrc_pcrc.high_mobility_ind = TRUE;
      }
    }
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8,
        physicalChannelReconfiguration_r8_add_ext))
       
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: rrc_physicalChannelReconfiguration_r8_add_ext_present not yet supported");
    } 

    /* Need to send reconfig IE info. First set TDSORDERED_CONFIG
    so that LLC can process the necessary parameters. */
    oc_status = tdsrrcllc_set_ordered_config(TDSRRC_PROCEDURE_PCR, tdsrrc_pcrc.state_indicated_by_msg, 
                                                            tdsrrc_DL_DCCH_Message_PDU,(void *)dcch_msg_ptr );
    switch(oc_status)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"oc_status is %d ",oc_status);
      case TDSORDERED_CONFIG_SET:
        tdsrrcpcrc_process_after_oc_config_set_r8(dcch_msg_ptr,pcrc_ptr);
        break;

      case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
        tdsrrc_pcrc.status.failure_status = TDSRRCPC_SIMULTANEOUS_RECONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
        tdsrrc_pcrc.status.failure_status =TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_pcrc.status.failure_status =TDSRRCPC_MSG_INVALID_CONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
      
      case TDSORDERED_CONFIG_NOT_SET_OTHER:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
        /* When LLC sends this, there is a serious problem
        and we'll go back to disconnected state. No need
        to send the failure message here, we can just
        go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      default:

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
        /* Unknown error, just go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
    }/* end switch */

    if(ordered_config_err_occured == TRUE)
    {
      /* No need to continue. Clear the procedure variables and return. */
      tdsrrcpcrc_clear_procedure();
      return;
    }
    else
    {
      if(tdsrrc_pcrc.pcrc_for_srns_relocation == TRUE)
      {
        tdsrrcsmc_commit_fresh_for_srns_relocation();
      }
    }
  }
}


#endif /*FEATURE_TDSCDMA_REL8*/

#ifdef FEATURE_TDSCDMA_REL9
/*===========================================================================

FUNCTION   tdsrrcpcrc_validate_pcrc_r9

DESCRIPTION

  This function validates the received PC Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a PCRC Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_validate_pcrc_r9
(
  tdsrrc_PhysicalChannelReconfiguration_r9_IEs *msg_ptr /* Pointer to the PCRC message */
)
{
 
  uecomdef_status_e_type status;
    /* Assume the message is valid for now */
  tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_VALID;

  /* Check the RRC State indicatior given by the message */
  switch( msg_ptr->rrc_StateIndicator )
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;

    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;


    default:
      /* No other states are supported right now. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported state in PC Reconfig Message");
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
      break;
  }

  if( tdsrrc_pcrc.status.failure_status == TDSRRCPC_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
     return;
  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
      tdsrrc_PhysicalChannelReconfiguration_r9_IEs,dl_HSPDSCH_Information))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSPDSCH Info present");
    
    /* Check if network does not try to initiate HSDPA with invalid state */
    status = tdsrrc_hsdpa_initial_validation(msg_ptr->rrc_StateIndicator, 
          TDSRRC_CHECK_COMMON_MSG_TYPE(msg_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
          rrc_DL_HSPDSCH_Information_r9_modeSpecificInfo_fdd));
    
    if (status == FAILURE)
    {
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_INVALID_CONFIG;
      return;
    }
  }
  
   /* Check if RB with PDCP Information list is present. This
  is not supported in the first release. This may be supported
  later */                                    
  
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
        tdsrrc_PhysicalChannelReconfiguration_r9_IEs,dl_CounterSynchronisationInfo)) 
#else
  if(tdsrrcpcrc_validate_srns_relocation_info_new(msg_ptr, TDSMSG_REL9))
#endif

  {

    /*check whether the SRNS relocation is within Cell_DCH or not.
     SRNS relocation information can come in reconfiguration messages
     only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"PCRC : SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_pcrc.state_indicated_by_msg);
      tdsrrc_pcrc.status.failure_status=TDSRRCPC_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has 
    PDCP info list. If it is true then this configuration is unsupported */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->dl_CounterSynchronisationInfo,
         rb_WithPDCP_InfoList)) ||
        (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->dl_CounterSynchronisationInfo,
         tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
      tdsrrc_pcrc.status.failure_status=TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation. 
        Set the flag that indicates that this procedure
      involves SRNS relocation */
      tdsrrc_pcrc.pcrc_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, tdsrrc_PhysicalChannelReconfiguration_r9_IEs,new_U_RNTI))
      {
        tdsrrc_pcrc.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(msg_ptr->new_U_RNTI),
                              &tdsrrc_pcrc.new_urnti);
      }
    }
  }

    /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  tdsrrc_pcrc.trans_to_dch_ciph_config_needed = FALSE;
  if((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr,
        tdsrrc_PhysicalChannelReconfiguration_r9_IEs,dl_CommonInformation)) && 
       (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->dl_CommonInformation,
       tdsrrc_DL_CommonInformation_r8,dl_dpchInfoCommon)) &&
       (T_tdsrrc_DL_CommonInformation_r8_dl_dpchInfoCommon_dl_DPCH_InfoCommon == 
         msg_ptr->dl_CommonInformation.dl_dpchInfoCommon.t))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");
        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon,tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
              &msg_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
              &mac_d_hfn_l);
        if (loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"SMC process ciphering config for HHO failed");
        }
        tdsrrc_pcrc.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
     /* If UTRAN DRX coefficient is not present then it is invalid configuration */
     if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
         tdsrrc_PhysicalChannelReconfiguration_r9_IEs,utran_DRX_CycleLengthCoeff)))
     {
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_pcrc.state_indicated_by_msg);
       tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_INVALID_CONFIG;
       return;
     }
     if (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
     {
       /*should we do it after getting l2ack for response??*/
       if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, tdsrrc_PhysicalChannelReconfiguration_r9_IEs,ura_Identity)))
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
         tdsrrccu_clear_ura_id ();
       }
       else
       {
         /*URA ID present in the message set ura_identity variable with this value */
         if (msg_ptr->ura_Identity.numbits == 16)
         {
           tdsrrccu_update_ura_id (&(msg_ptr->ura_Identity));
         }
         else
         {
           TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",msg_ptr->ura_Identity.numbits);
           tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_PROTOCOL_ERROR;
           tdsrrc_pcrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
           return;
         }

       }
           
     }
  }
}/* end tdsrrcpcrc_validate_pcrc_r9() */

/*===========================================================================
FUNCTION   tdsrrcpcrc_process_after_oc_config_set_r9

DESCRIPTION
  This function processes the R8 PCRC OTA after OC is set by this OTA. So if we call this function, it means
  that TDSORDERED_CONFIG_SET is returned while setting OC by PCRC OTA.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcpcrc_process_after_oc_config_set_r9
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr,
  tdsrrc_PhysicalChannelReconfiguration_r9_IEs *pcrc_ptr  
)
{
  tdsrrc_int_c_rnti_type                 c_rnti;           /* Local C_RNTI value */
  tdsrrc_int_u_rnti_type                 u_rnti;           /* Local U_RNTI value*/
  tdsrrc_state_e_type                    current_state;              /* current RRC State  */  
  tdsrrcrb_directed_cell_info_type  directed_cell_info;        /* directed cell info struct */
  tdsrrcrb_rnti_update_info_type    rnti_update_info;             /* to hold rnti update info */  
  tdsrrc_PhysicalChannelReconfiguration *msg_ptr = NULL;  /* Pointer to the RB Setup message */

  current_state = tdsrrc_get_state(); 

  msg_ptr = &dcch_msg_ptr->message.u.physicalChannelReconfiguration;
  
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
  if(tdsrrcsib_is_current_event_sib_read_in_dch())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Force Init GPS ");
    tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
  }
#endif
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /* Populate the local variables to indicate if Re-direction info is present */
  directed_cell_info.freq_info_present = FALSE;
  directed_cell_info.cpid_present = FALSE;
  if((current_state==TDSRRC_STATE_CELL_FACH) && 
           ((tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
           ||(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
           ||(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
  {
     
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
        tdsrrc_PhysicalChannelReconfiguration_r9_IEs,frequencyInfo)) &&
       (TDSRRC_CHECK_COMMON_MSG_TYPE(pcrc_ptr->frequencyInfo.
        modeSpecificInfo,rrc_FrequencyInfo_modeSpecificInfo_tdd)))           
    {
      directed_cell_info.freq_info_present = TRUE;
      directed_cell_info.uarfcn_dl = pcrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
    }
    
    if (tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL9))
    {
      directed_cell_info.cpid_present = TRUE;
      directed_cell_info.cell_param_id = pcrc_ptr->dl_InformationPerRL_List.elem[0].
                                modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
    }
    /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
    FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
    if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
      (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
      if(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
      {
        tdsrrcpcrc_send_initiate_cu_flag = TRUE;
        tdsrrcpcrc_delete_crnti          = TRUE;
      }
      else
      {
        tdsrrc_pcrc.directed_cell_info = TRUE;
      }
    }          
  }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
  if (
      (current_state==tdsrrc_pcrc.state_indicated_by_msg)||
      ((current_state==TDSRRC_STATE_CELL_FACH)&& 
       (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH))
     )
  {
    /*  CellDCH<->CellDCH or CellFACH<->CellFACH or CellFACH->CellDCH 
    transition. Ordered Config has been set. We can setup the
    Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");

    /*check whether any frequency info is present in the message
    and transition is CELL_FACH->CELL_FACH. In this case
    initiate a cell update after channel config is successfully completed */
    if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
       (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
    {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU

      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if ((tdsrrcpcrc_send_initiate_cu_flag == FALSE)&&
          (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
      {

        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r7_IEs,new_C_RNTI))
        {
          tdsrrc_pcrc.new_crnti_valid = TRUE;
          tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                              &c_rnti);
          tdsrrc_pcrc.new_crnti = (uint16)c_rnti;
        }
        else
        {
          tdsrrc_pcrc.new_crnti_valid = FALSE;
        }
        /* Now copy OC to TOC */
        tdsrrcllcoc_update_toc_with_oc_from_fach();
        /*Initiate the cell selection*/
        tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR,
                                                 TDSRRC_TRANSITION_FROM_FACH,
                                                 tdsrrc_pcrc.state_indicated_by_msg);

  
        tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
        return;
      }

#else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
          tdsrrc_PhysicalChannelReconfiguration_r9_IEs,frequencyInfo)) &&
         (TDSRRC_CHECK_COMMON_MSG_TYPE(pcrc_ptr->frequencyInfo.
          modeSpecificInfo,rrc_FrequencyInfo_modeSpecificInfo_tdd))  )         
      {
        directed_cell_info.freq_info_present = TRUE;
        directed_cell_info.uarfcn_dl = pcrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
      }
      else
      {
        directed_cell_info.freq_info_present = FALSE;
      }
      if (tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL9))
      {
        directed_cell_info.cpid_present = TRUE;
        directed_cell_info.cell_param_id = pcrc_ptr->dl_InformationPerRL_List.elem[0].
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
        tdsrrcpcrc_send_initiate_cu_flag = FALSE;
      }
      else
      {
        /*directed cell info do not match. Cell update is required */
        tdsrrcpcrc_send_initiate_cu_flag = TRUE;
        tdsrrcpcrc_delete_crnti          = TRUE;
      }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
    } /* if CELL_FACH->CELL_FACH reconfiguration */
  
    /* Call the function that creates and sends a channel
            config request to LLC. */
    tdsrrcpcrc_send_channel_config_req( );
       /* Change the substate since we're waiting for the
       confirmation from LLC for the channel configs */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
  }
  else if((current_state==TDSRRC_STATE_CELL_DCH) && 
          (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
  {
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r9_IEs,new_C_RNTI))
    {
      tdsrrc_pcrc.new_crnti_valid = TRUE;
      tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                          &c_rnti);
      tdsrrc_pcrc.new_crnti = (uint16)c_rnti;
    }
    else
    {
      tdsrrc_pcrc.new_crnti_valid = FALSE;
    }

    /*Initiate the cell selection*/
    tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR,
                                            TDSRRC_TRANSITION_FROM_DCH,
                                            TDSRRC_STATE_CELL_FACH);
    tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

    /* Start T_305 [periodic cell update] timer */
    tdsrrccu_start_t_305_timer();     
  }
  else if((current_state==TDSRRC_STATE_CELL_DCH) && 
          (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
  {
    tdsrrc_pcrc.directed_cell_info = tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL9);
   /*Send the PCRC complete msg on old configuration */
   tdsrrcpcrc_send_pcrc_complete_message(TRUE);
   TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
   /*wait for L2ack */
   tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
   /* prints rate for each transport channel */
   tdsrrcllc_print_trans_chnl_rate();
  }
  else if((current_state==TDSRRC_STATE_CELL_DCH) && 
          (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
  {
    /*Send the PCRC complete msg on old configuration */
    tdsrrcpcrc_send_pcrc_complete_message(TRUE);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
    /*wait for L2ack */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    /* prints rate for each transport channel */
    tdsrrcllc_print_trans_chnl_rate();
  }
  else if((current_state==TDSRRC_STATE_CELL_FACH) && 
          ((tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
           (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
  {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if ((tdsrrc_pcrc.directed_cell_info == FALSE) &&
        (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
      {
        if(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
        {
          tdsrrc_pcrc.directed_cell_info = tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL9);
        }
      
        tdsrrc_pcrc.initiate_cell_selection_trans_from_fach = TRUE;
      }
      
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

    /* check whether new C-RNTI was present in the message. In that case
    we have to configure MAC with new C-RNTI before sending the response 
    message */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
       tdsrrc_PhysicalChannelReconfiguration_r9_IEs,new_C_RNTI))
    {
      tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                          &c_rnti);
      /* Send a RNTI Update Request */
      rnti_update_info.crnti_action    = TDSRRC_RNTI_UPDATE;
      rnti_update_info.crnti           = (uint16)c_rnti;
      /* check whether URNTI is also present in the message */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr,
       tdsrrc_PhysicalChannelReconfiguration_r9_IEs,new_U_RNTI))
      {
        tdsrrc_translate_urnti(&pcrc_ptr->new_U_RNTI, 
                            &u_rnti);
        rnti_update_info.urnti_action = TDSRRC_RNTI_UPDATE; 
        rnti_update_info.urnti        = u_rnti; 
      }
      else
      {
        rnti_update_info.urnti_action = TDSRRC_RNTI_NOCHANGE; 
      }
      rnti_update_info.procedure            = TDSRRC_PROCEDURE_PCR;
      rnti_update_info.rlc_re_establish_srb = FALSE;
      rnti_update_info.rlc_re_establish_rab = FALSE;
      rnti_update_info.cnf_reqd             = TRUE;

      tdsrrcrb_send_rnti_update_req(rnti_update_info);
      tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
    }
    else 
    {
      /* There is no new C-RNTI So send the response on the old configuration
      */
      tdsrrcpcrc_send_pcrc_complete_message(TRUE);
      /*wait for L2ack */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
      tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
      /* prints rate for each transport channel */
      tdsrrcllc_print_trans_chnl_rate();
    }
  }
  return;
}

/*===========================================================================
FUNCTION   PROCESS_PCRC_R9

DESCRIPTION
  This function processes the received Rel 9 PC Reconfig Message. It first validates the message. 
  If the message has an invalid or unsupported configuration, a PCRC Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -

    TDSORDERED_CONFIG will be set by calling LLC. 

    If TDSORDERED_CONFIG could be set without any conflicts, a TDSRRC_CHANNEL_CONFIG_REQ 
    will be sent to LLC to reconfigure the Physical Channel given by the message.
    
    If TDSORDERED_CONFIG could not be set, a PCRC Failure message
    will be sent to UTRAN.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcpcrc_process_pcrc_r9
(
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id,

  tdsrrc_DL_DCCH_Message *dcch_msg_ptr  ,
  /* Pointer to the RB Reconfig message        */
  tdsrrc_PhysicalChannelReconfiguration_r9_IEs *pcrc_ptr
)
{
  tdsrrc_PhysicalChannelReconfiguration *msg_ptr = NULL;    /* Pointer to the RB Setup message        */

  tdsrrcllc_oc_status_e_type             oc_status;/* To hold tdsordered_config status   */

  boolean       ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                    occured while setting tdsordered_config. */

  msg_ptr = &dcch_msg_ptr->message.u.physicalChannelReconfiguration;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received PCRC msg rel 9,substate %d",tdsrrc_pcrc.curr_substate);
  
  if (FAILURE == 
      tdsrrcpcrc_check_and_send_failure_for_simul_reconfig(
        msg_ptr->u.later_than_r3.rrc_TransactionIdentifier))
  {
    return;
  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
         u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9,
      physicalChannelReconfiguration_r9_add_ext))
     
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: rrc_physicalChannelReconfiguration_r9_add_ext_present not yet supported");
    tdsrrc_pcrc.status.failure_status=TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
    /* Send the PC Reconfig Failure Message */
    tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                              tdsrrc_pcrc.status.prot_err_cause
                            );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
    return;
  } 

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/
  if ((pcrc_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         pcrc_ptr->m.activationTimePresent,
         pcrc_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC SDU Lifetime validation failed");
      tdsrrcpcrc_send_lifetime_validation_failure(TDSRRCPC_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force PCRC Activation time to NOW");
      pcrc_ptr->m.activationTimePresent = 0;
    }
  }

#endif /*FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION*/

  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  tdsrrcpcrc_validate_pcrc_r9( pcrc_ptr );

  if( tdsrrc_pcrc.status.failure_status != TDSRRCPC_MSG_VALID )
  {
    /*  Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC Message validation failed");

    /* Send the PC Reconfig Failure Message */
    tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                              tdsrrc_pcrc.status.prot_err_cause
                            );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
  }
  /* If the message is valid, proceed further */
  else
  {

    TDSRRC_MSG0(MSG_LEGACY_LOW,"Processing PCRC Message");

    /* Check if there is CN information present. If so, we need
    to send the information to the Cell Change manager */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r9_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      tdsrrc_pcrc.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(
                        &(pcrc_ptr->cn_InformationInfo));
 
      if (tdsrrc_pcrc.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_pcrc.cn_info_saved = TRUE;
      }
    }
      
 
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r9_IEs,ueMobilityStateIndicator))
    {
      if(pcrc_ptr->ueMobilityStateIndicator == tdsrrc_High_MobilityDetected_high_MobilityDetected)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind Set to TRUE in PCRC.");
        tdsrrc_pcrc.high_mobility_ind = TRUE;
      }
    }

    /* Need to send reconfig IE info. First set TDSORDERED_CONFIG
          so that LLC can process the necessary parameters. */
    oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_PCR,
                                           tdsrrc_pcrc.state_indicated_by_msg,
                                           tdsrrc_DL_DCCH_Message_PDU,
                                           (void *)dcch_msg_ptr );

    switch( oc_status )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"oc_status is %d ",oc_status);
      case TDSORDERED_CONFIG_SET:
        tdsrrcpcrc_process_after_oc_config_set_r9(dcch_msg_ptr,pcrc_ptr);
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_pcrc.status.failure_status =TDSRRCPC_MSG_INVALID_CONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
      
      case TDSORDERED_CONFIG_NOT_SET_OTHER:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
        /* When LLC sends this, there is a serious problem
        and we'll go back to disconnected state. No need
        to send the failure message here, we can just
        go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      default:

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
        /* Unknown error, just go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
    }/* end switch */

    if( ordered_config_err_occured == TRUE )
    {
      /* No need to continue. Clear the procedure
      variables and return. */
      tdsrrcpcrc_clear_procedure();
      return;
    }

    if(tdsrrc_pcrc.pcrc_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }
  }
}
#endif /* FEATURE_TDSCDMA_REL9 */

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/*===========================================================================

FUNCTION   RRCRPRC_PROCESS_NEW_CELL_IND

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
static void tdsrrcpcrc_process_new_cell_ind
(
  tdsrrc_new_cell_ind_type *ptr /* Pointer to the State Change Ind cmd */
)
{
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
  /* If the UE has camped on directed cell then do not send a cell update,
    * else send a CELL update with cause "cell Re-selection 
    "*/
  if(tdsrrccsp_is_selected_cell_utran_directed()&&
      (tdsrrc_pcrc.new_crnti_valid)) 
  {
     /* No need to initiate cell update as the C-RNTI is available
      * and UE is camped on the directed cell.
      */
    tdsrrcpcrc_send_initiate_cu_flag = FALSE;
  }
  else
  {
    tdsrrcpcrc_send_initiate_cu_flag = TRUE;
    tdsrrcpcrc_delete_crnti = TRUE;
  }
     
  tdsrrcpcrc_send_channel_config_req();
  
  /* Change the substate since we're waiting for the
     confirmation from LLC for the channel configs */
  tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
 
}
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

/*===========================================================================

FUNCTION   tdsrrcpcrc_validate_and_check_integrity_for_srns_relocation

DESCRIPTION

  This function validates whether received tdsrrc_pcrc message is due to SRNS relocation,
  if yes, then it performs the integrity check on the received message because
  integrity check was skipped for message which triggers SRNS relocation.
          
DEPENDENCIES

  None.

REUSABILITY: R99/R5/R6/R7/R8

RETURN VALUE

  SUCCESS or FAILURE

SIDE EFFECTS

  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrcpcrc_validate_and_check_integrity_for_srns_relocation
(
  tdsrrc_cmd_type *cmd_ptr,
  /* Pointer to the PC Reconfig message        */
  tdsrrc_PhysicalChannelReconfiguration *msg_ptr,
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


  if (TRUE == tdsrrcpcrc_validate_srns_relocation_info(msg_ptr, msg_version))
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
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IP check failed. Discarding RBS msg");
        break;

      case TDSRRCSMC_SRNS_SECURITY_INFO_INVALID:
      case TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION:
        if (tdstransaction_id == tdsrrc_pcrc.tr_ids.rejected_transaction)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid security info - rejected tr-id%d match!",tdsrrc_pcrc.tr_ids.rejected_transaction);
          /* Nothing else to do, just ignore the message and return */
        } 
        else
        {
          /* send a reconfig failure messge with invalid configuration and 
          continue the same substate with any ongoing procedure */
          tdsrrc_pcrc.tr_ids.rejected_transaction = tdstransaction_id;

          if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
          {
            tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_INVALID_CONFIG;
          }
          else
          {
            tdsrrc_pcrc.status.failure_status = TDSRRCPC_SIMULTANEOUS_RECONFIG;
          }
          /*Check whether Cell Update is going on */
          if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
               (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
            (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_PCR);
            /* Wait until Cell update procedure is completed */
            tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
          }
          else
          {
            tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                            tdsrrc_pcrc.status.prot_err_cause );
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
  }

  if (ip_check_status != TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS)
  {
    /*do not clear the procedure or change the tdsrrc_pcrc substate because
    some other tdsrrc_pcrc procedure might be in progress. Just return from
    here */
    return FAILURE;
  }
  else
  {
    return SUCCESS;
  }  
}

/*===========================================================================

FUNCTION   tdsrrcpcrc_send_pcrc_failure_unsupported_config

DESCRIPTION

  This function sends the pcreconfig-failure messsage with cause unsupported
  configuration because UE dont support the extension provided by the network.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_send_pcrc_failure_unsupported_config
(
  /* transaction id of RB Reconfig message */
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id  
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported PC Reconfig t");
  if (tdstransaction_id == tdsrrc_pcrc.tr_ids.rejected_transaction )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate PCRC msg! rejected tr-id%d match!",tdsrrc_pcrc.tr_ids.rejected_transaction);
    /* Nothing else to do, just ignore the message and return */
  }
  else
  {
    /* We have received a tdsrrc_pcrc message with a new transaction id.
    This message should be in our rejected transaction  
    list */
    tdsrrc_pcrc.tr_ids.rejected_transaction = tdstransaction_id;
    tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_PROTOCOL_ERROR;
    tdsrrc_pcrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;
    /*Check whether Cell Update is going on */
    if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
         (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
      if (SUCCESS != tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_PCR))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCR failed to register with CU");
      }
      /* Wait until Cell update procedure is completed */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF; 
    }
    else
    {
      tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                 tdsrrc_pcrc.status.prot_err_cause );
      /*continue the same substate */ 
    }
  }
  return;
}



/*===========================================================================

FUNCTION   PROCESS_PCRC_MESSAGE

DESCRIPTION

  This function processes the received PC Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a PCRC Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to reconfigure the Physical Channel given by the message.
    
    If TDSORDERED_CONFIG could not be set, a PCRC Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_process_pcrc_message
(
  tdsrrc_cmd_type *cmd_ptr
  
)
{

  tdsrrc_PhysicalChannelReconfiguration *msg_ptr;  /* Pointer to the PC Reconfig message        */
  tdsrrc_DL_DCCH_Message         *dcch_msg_ptr ;  /* Pointer to the PC Reconfig message */


  /* A Downlink SDU has been received from RLC. This
  SDU should contain a PC Reconfiguration Message. Otherwise
  we would not have received this command. Copy the 
  received message pointer. */
  
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received PCRC msg,substate %d",tdsrrc_pcrc.curr_substate);

  /* If ASN.1 decoding failed, the received pointer would be null.
  So we know we got a PCRC Message (by peeking into
  the encoded bit stream), but we were not able to decode it.
  Send a PC Reconfig failure message with the appropriate
  cause. and continue in same substate*/
  if( dcch_msg_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"ASN decode failed for PCRC msg");
    tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_PROTOCOL_ERROR;
    tdsrrc_pcrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
    tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                              tdsrrc_pcrc.status.prot_err_cause
                            );
    
    /* Nothing else to do here */
    return;
  }


  /* Make sure it's the PCRC message
  although we would not have gotten here unless it's
  the right message */ 
  if( dcch_msg_ptr->message.t != T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Incorrect message given to PCR proc %d",dcch_msg_ptr->message.t);
    return;
  }

  if(TRUE == tdsrrccsp_reselection_in_progress())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Abort cell reselection as it's conflict with pcreconfig");
    tdsrrccsp_abort_cell_reselection();
  }

  /* Get the pointer to the actual PC Reconfig message */
  msg_ptr = &(dcch_msg_ptr->message.u.physicalChannelReconfiguration);

  /* Unsupport rel99 message */
  if (msg_ptr->t == T_tdsrrc_PhysicalChannelReconfiguration_r3)
  {
    tdsrrc_print_supported_asn1_rel_version();

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Configuration rel99 not supported.  Send PC reconfig failure");

    tdsrrcpcrc_send_pcrc_failure_unsupported_config(
        msg_ptr->u.r3.physicalChannelReconfiguration_r3.rrc_TransactionIdentifier);      
    return;
  }
  
  if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL4))
  {
    /* check for SRNS relocation scenario */
    if (FAILURE == tdsrrcpcrc_validate_and_check_integrity_for_srns_relocation(
        cmd_ptr, msg_ptr, msg_ptr->u.later_than_r3.rrc_TransactionIdentifier, 
        TDSMSG_REL4))
    {
      /*do not clear the procedure or change the tdsrrc_rbr substate because
     some other rbreconfig procedure might be in progress. Just return from
     here */
      return;
    }
    tdsrrcpcrc_process_pcrc_r4(cmd_ptr);
  }
  else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
      (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL5)))
  {
    /* check for SRNS relocation scenario */
    if (FAILURE == tdsrrcpcrc_validate_and_check_integrity_for_srns_relocation(
        cmd_ptr, msg_ptr, msg_ptr->u.later_than_r3.rrc_TransactionIdentifier, 
        TDSMSG_REL5))
    {
      /*do not clear the procedure or change the tdsrrc_rbr substate because
       some other rbreconfig procedure might be in progress. Just return from
       here */
      return;
    }

    tdsrrcpcrc_process_pcrc_r5(cmd_ptr);
  }
  else if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6) &&
      (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL6)))
  {
    /* check for SRNS relocation scenario */
    if (FAILURE == tdsrrcpcrc_validate_and_check_integrity_for_srns_relocation(
        cmd_ptr, msg_ptr, msg_ptr->u.later_than_r3.rrc_TransactionIdentifier, 
        TDSMSG_REL6))
    {
      /* do not clear the procedure or change the tdsrrc_rbr substate because
      some other rbreconfig procedure might be in progress. Just return from
      here */
      return;
    }

    tdsrrcpcrc_process_pcrc_r6(
      msg_ptr->u.later_than_r3.rrc_TransactionIdentifier,
      (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg,
      &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.r6.physicalChannelReconfiguration_r6
    );
  }
  else if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7) &&
         (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL7)))
  {
    /* check for SRNS relocation scenario */
    if (FAILURE == tdsrrcpcrc_validate_and_check_integrity_for_srns_relocation(
        cmd_ptr, msg_ptr, msg_ptr->u.later_than_r3.rrc_TransactionIdentifier, 
        TDSMSG_REL7))
    {
      /* do not clear the procedure or change the tdsrrc_rbr substate because
      some other rbreconfig procedure might be in progress. Just return from
      here */
      return;
    }

    tdsrrcpcrc_process_pcrc_r7(
      msg_ptr->u.later_than_r3.rrc_TransactionIdentifier,
      (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg,
      &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.r7.physicalChannelReconfiguration_r7
    );
  }
#ifdef FEATURE_TDSCDMA_REL8
  /*Now we check whether R8 PCRC is present or not. */
  else if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8) &&
         (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL8)))
  {
    /* check for SRNS relocation scenario */
    if (FAILURE == tdsrrcpcrc_validate_and_check_integrity_for_srns_relocation(
      cmd_ptr, msg_ptr, msg_ptr->u.later_than_r3.rrc_TransactionIdentifier, TDSMSG_REL8))
    {
      /* do not clear the procedure or change the tdsrrc_rbr substate because
      some other rbreconfig procedure might be in progress. Just return from
      here */
      return;
    }
    /*Now process the R8 PCRC OTA and then return.*/
    tdsrrcpcrc_process_pcrc_r8(msg_ptr->u.later_than_r3.rrc_TransactionIdentifier,
      (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg,
      &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.r8.physicalChannelReconfiguration_r8);
  }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  /*Now we check whether R9 PCRC is present or not. */
  else if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9) &&
         (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL9)))
  {
    /* check for SRNS relocation scenario */
    if (FAILURE == tdsrrcpcrc_validate_and_check_integrity_for_srns_relocation(
      cmd_ptr, msg_ptr, msg_ptr->u.later_than_r3.rrc_TransactionIdentifier, TDSMSG_REL9))
    {
      /* do not clear the procedure or change the tdsrrc_rbr substate because
      some other rbreconfig procedure might be in progress. Just return from
      here */
      return;
    }
    /*Now process the R8 PCRC OTA and then return.*/
    tdsrrcpcrc_process_pcrc_r9(msg_ptr->u.later_than_r3.rrc_TransactionIdentifier,
      (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg,
      &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.physicalChannelReconfiguration_r9);
  }
#endif /* FEATURE_TDSCDMA_REL9 */

  else
  {
    tdsrrc_print_supported_asn1_rel_version();

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Configuration not supported.  Send PC reconfig failure");

    tdsrrcpcrc_send_pcrc_failure_unsupported_config(
        msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);      
  }

  /* return - because all r4, r5, r6, r7 and r8 processing are done inside the function */
  return;

}/* end function tdsrrcpcrc_process_pcrc_message */

 /*===========================================================================

FUNCTION   RRCPCRC_PROCESS_RNTI_UPDATE_CNF

DESCRIPTION

  This function processes RNTI_UPDATE_CNF
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_process_rnti_update_cnf
(
void
)
{
  tdsrrc_state_e_type curr_state;

  curr_state = tdsrrc_get_state();

  if ((curr_state == TDSRRC_STATE_CELL_FACH) && 
      ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
       (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)))
  {
   /*send the response message now */
    tdsrrcpcrc_send_pcrc_complete_message(TRUE);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
    /*wait for L2ack */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    /* prints rate for each transport channel */
    tdsrrcllc_print_trans_chnl_rate();
  }
  else
  {
    /* We should not get here*/
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unexpected RNTI Update cnf,tdsrrc_pcrc state %d",tdsrrc_pcrc.curr_substate);
  }
}



/*===========================================================================

FUNCTION   RRCPCRC_PROCESS_CHANNEL_CONFIG_CNF

DESCRIPTION

  This function processes the received Channel Config Confirmation
  from LLC. 
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_process_channel_config_cnf
(
  tdsrrc_channel_config_cnf_type *ptr  /* Pointer to the Channel config cnf cmd */
)
{
  tdsrrc_state_e_type            curr_state;
  tdsrrcrb_rnti_update_info_type rnti_update_info;   /* to hold rnti update info */
  tdsrrc_cmd_type *cmd; /* To send a LLC command if channel config failed */
  tdsrrc_CellUpdateCause       cu_cause;  /*To store CU cause from CU procedure*/

  tdsrrc_rlc_re_est_for_srns_rb_e_type rb_type; /*rb type tobe reestablished */

  tdsrrcrb_update_amr_cctrch_id_for_reconfig(tdsrrcdata_codec_info.rrc_active_codec);

  /* Check if the channel configuration was succesful. */
  if( ptr->chan_cfg_status != TDSRRCLLC_CHAN_CFG_SUCCESS )
  {

    /*Here we initialize tdsrlc_size_change_in_progress DB so that if it was set by PCRC, 
    it is ready for next procedure*/
    tdsrrc_initialize_rlc_size_change();

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CHAN_CONFIG_CNF with Failure");
    tdsrrc_pcrc.status.failure_status = TDSRRCPC_PHY_CHAN_FAILURE;
    if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_CFG_CANCELLED)
    {
      #ifdef FEATURE_TDSCDMA_RCR_CANCEL_RECFG
      if ( TRUE == tdsrrcrcr_is_proc_active())
      {
        /* Reset TDSORDERED_CONFIG - UE can now accept other ordered
        configurations */
        if (TDSOC_NOT_SET == tdsrrcllc_reset_ordered_config(TDSRRC_PROCEDURE_PCR
                                                      ,TRUE
                                                      ))
        {
          /* Then clear all local variables that may
          need to be cleared */
          tdsrrcpcrc_clear_procedure();
          /* Make sure we're back in the initial sub-state */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        }
      }
      else
      #endif
      {
        /* cell update is already in progress as this is for pending config
        * cancellation
        */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CU active.  Wait for CU to complete");
        tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      }

      return; 
    }
    
    /* validation failure happened.  Send RB failure msg with invalid config */
    if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_OTHER)
    {
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_INVALID_CONFIG;

      if(tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CU active.  Initiate CU and wait for CU complete");
        tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_PCR, TDSRRC_UNRECOVERABLE_ERROR, FALSE);
        return; 
      }

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending PCRC failure msg with invalid config");
      tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                tdsrrc_pcrc.status.prot_err_cause
                                );
      /* Reset TDSORDERED_CONFIG - UE can now accept other ordered
      configurations */
      if (TDSOC_NOT_SET == tdsrrcllc_reset_ordered_config(TDSRRC_PROCEDURE_PCR
                                                    ,TRUE
                                                    ))
      {
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcpcrc_clear_procedure();
        /* Make sure we're back in the initial sub-state */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to reset the OC");
      }
      /*During FACH->DCH transition, if T305 got expired, we need to start T305 with zero timeout.*/
      if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH && tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
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
    request, and tdsrrc_pcrc substate should be TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF */ 
    if (tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH && tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PHYCHANFAIL FACH->DCH.  Initiate CU");

      tdsrrc_pcrc.status.failure_status = TDSRRCPC_PHY_CHAN_FAILURE;
      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_PCR, TDSRRC_PHY_CHAN_FAILURE,TRUE);
      /* Wait for CELL UPDATE to get a valid C_RNTI */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      tdsrrcpcrc_send_initiate_cu_flag = FALSE;
      tdsrrcpcrc_delete_crnti = FALSE;
      
    }
    /* Check if PHY_CHAN_FAILURE happended within CELL_DCH.  For now, initatiate a cell update request
       with cause RL Failure.  RBRC substate should be TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF*/
    else if (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH && tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
    {
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_PHY_CHAN_FAILURE;
      if(tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CU active.  Initiate CU and wait for CU to complete");
        tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_PCR, TDSRRC_RADIO_LINK_FAILURE, FALSE);
        return; 
      }
      if(ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_PHY_CHL)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PCRC failed,going back to old cfg success");
        
        
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                   tdsrrc_pcrc.status.prot_err_cause
                                   );
        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcpcrc_clear_procedure();
        /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
        configurations */
        tdsrrcllc_clear_ordered_config(); 
      }
      else if(ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_BACK_TO_OLD_CONFIG)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PCRC failed. Going back to old config failed.  Initiate CU");

        /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_PCR, TDSRRC_RADIO_LINK_FAILURE,TRUE);
        /* Wait for CELL UPDATE to get a valid C_RNTI */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        tdsrrcpcrc_send_initiate_cu_flag = FALSE;
        tdsrrcpcrc_delete_crnti = FALSE;
      }
    }
    else
    {
      if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
          (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
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
          cmd->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_PCR;
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
        tdsrrc_pcrc.status.failure_status = TDSRRCPC_PHY_CHAN_FAILURE;
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                   tdsrrc_pcrc.status.prot_err_cause
                                   );
      }
      /* Go back to initial substate */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
      /* At this point we don't need to remember any of the
      Radio Bearer information since it's stored in ESTABLISHED_RABS.
      Clear our internal variables */
      tdsrrcpcrc_clear_procedure();
    }
    return;
  }

  if((tdsordered_config.set_status == TDSOC_SET_FOR_DCH_FACH_TRANS) || 
      (tdsordered_config.set_status == TDSOC_SET_FOR_DCH_CELL_PCH_TRANS) ||
      (tdsordered_config.set_status == TDSOC_SET_FOR_DCH_URA_PCH_TRANS))	
  {
    if(tdsrrc_pcrc.high_mobility_ind == TRUE)
    {
      tdsrrcrb_send_upd_ota_parms_ind(TDSRRC_PROCEDURE_PCR, TDSL1_CPHY_HIGH_MOBILITY_INCL);
    }
  }  

  /*For FACH->DCH, if CHAN_CONFIG_REQ is succeeded, we need to stop T305*/  
  if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH && tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Chan_config succeeded for FACH->DCH. So Stopping T305");
    tdsrrctmr_stop_timer(TDSRRCTMR_T_305_TIMER);
  }

    /*Notify the Cell_Id to registered entities*/
  if(tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH)
  {
    tdsrrc_notify_cell_id_in_dch();
  }	

    /* Check for CELL FACH before sending the rbs complete message */
  curr_state=tdsrrc_get_state();
  if ((TDSRRC_STATE_CELL_FACH == curr_state) && 
      ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH) ||
       (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
       (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)))
  {
    if (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) 
    {
      /* this is the case of CELL_FACH->CELL_PCH state transition.
      In current implementation we do initiate cell selection. Instead
      we treat current cell as strongest cell */
      if (tdsrrc_pcrc.directed_cell_info)
      {
        /* check whether current camped scr code is same as the one
        sent in the message. If not then initiate cell update procedure */
        if(!tdsrrccsp_is_selected_cell_utran_directed())
        {
          /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
          tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_PCR,
                                              TDSRRC_CELL_RESELECTION,
                                              FALSE);
        }
      /* No need to send ul data registration to rlc. This will be taken care
      by cellupdate procedure */
        tdsrrcpcrc_process_successful_proc_completion(FALSE);
      }
      else
      {
        /* send drx info to L1. Since this is CELL_FACH->CELL_PCH state transition
        RB procedure is responsible for sending the DRX info to lower layer*/
        tdsrrcrb_send_l1_drx_req (TDSRRC_PROCEDURE_PCR);

        tdsrrcpcrc_process_successful_proc_completion (TRUE);
      }
      /* return from here */
      return;
    }
    else if (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
    {
      /* send drx info to L1 */
      tdsrrcrb_send_l1_drx_req (TDSRRC_PROCEDURE_PCR);
      /* check whether URA update is required */
      if ( tdsrrccu_is_ura_update_reqd() == TRUE)
      {
        tdsrrcrb_send_ura_update_initiate_req( TDSRRC_PROCEDURE_PCR,
                                            TDSRRC_URA_RESELECTION,
                                            FALSE);
      }
      /* no need to wait for ura update completion 
      declare that proc is successfully completed */
      tdsrrcpcrc_process_successful_proc_completion (TRUE);
      /* return from here */
      return;
    }
    if(tdsrrcpcrc_send_initiate_cu_flag
       || (tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_RB_PROC_COMPL_IND) 
       )
    {
      if (tdsrrcpcrc_delete_crnti)
      {
        /* Send a RNTI Update Request */
        rnti_update_info.crnti_action         = TDSRRC_RNTI_DELETE;
        rnti_update_info.urnti_action         = TDSRRC_RNTI_NOCHANGE;
        rnti_update_info.procedure            = TDSRRC_PROCEDURE_PCR;
        rnti_update_info.rlc_re_establish_srb = FALSE;
        rnti_update_info.rlc_re_establish_rab = FALSE;
        rnti_update_info.cnf_reqd             = FALSE;

        tdsrrcrb_send_rnti_update_req(rnti_update_info);
      }
      /* this is the case of DCH->FACH transition */
      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_PCR,
                                          TDSRRC_CELL_RESELECTION,
                                          TRUE);
      /* Wait for CELL UPDATE to get a valid C_RNTI */
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_VALID;
      tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      tdsrrcpcrc_send_initiate_cu_flag = FALSE;
      tdsrrcpcrc_delete_crnti = FALSE;
      return;
    }
    /*Following case will occur when RLC RESET happens on SRB2 during DCH->FACH Transition*/
    else if((tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG) &&
      (tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) == TDSRRCCU_STARTED) && 
      (cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError) && (tdsrrccu_get_rb_id_unrec_error() == DCCH_AM_RADIO_BEARER_ID))
    {
      if(tdsrrc_pcrc.new_crnti_valid)
      {
        /* Send a RNTI Update Request */
        rnti_update_info.crnti_action         = TDSRRC_RNTI_UPDATE;
        rnti_update_info.urnti_action         = TDSRRC_RNTI_NOCHANGE;
        rnti_update_info.crnti                = tdsrrc_pcrc.new_crnti;
        rnti_update_info.procedure            = TDSRRC_PROCEDURE_PCR;
        rnti_update_info.rlc_re_establish_srb = FALSE;
        rnti_update_info.rlc_re_establish_rab = FALSE;
        rnti_update_info.cnf_reqd             = FALSE;
  
        tdsrrcrb_send_rnti_update_req(rnti_update_info);
        /* Send a Radio Bearer Setup Complete later */      
      }

      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_PCR,
                                          TDSRRC_UNRECOVERABLE_ERROR,
                                          TRUE);

      /* Wait for CELL UPDATE to get a valid C_RNTI */
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_VALID;
      tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;

      return;                                          
    }
    
    /* Coming here implies the "tdsrrcpcrc_send_initiate_cu_flag" is FALSE.
     * Check if Valid CRNTI is stored in PCRC (for DCH->FACH case).
     * If so, send a RNTI Update Req to LLC and NOT wait for
     * Cell Update Complete Confirm. Cell Update will NOT be 
     * initiated when there is a valid C-RNTI and tdsrrcpcrc_send_initiate_cu_flag
     * is FALSE.
     */
    else if(tdsrrc_pcrc.new_crnti_valid)
    {
      /* Send a RNTI Update Request */
      rnti_update_info.crnti_action         = TDSRRC_RNTI_UPDATE;
      rnti_update_info.urnti_action         = TDSRRC_RNTI_NOCHANGE;
      rnti_update_info.crnti                = tdsrrc_pcrc.new_crnti;
      rnti_update_info.procedure            = TDSRRC_PROCEDURE_PCR;
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
      if (SUCCESS != tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_PCR))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCR failed to register with CU");
      }
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_VALID;
      tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      return;
    } 
  } /* if current_state is CELL_FACH */
  else if (TDSRRC_STATE_CELL_PCH == curr_state)
  {
    if(tdsrrcpcrc_send_initiate_cu_flag)
    {
      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_PCR,
                                          TDSRRC_CELL_RESELECTION,
                                          FALSE);
      tdsrrcpcrc_send_initiate_cu_flag = FALSE;
      /* No need to send ul data registration to rlc. This will be taken care
      by cellupdate procedure */
      tdsrrcpcrc_process_successful_proc_completion(FALSE);

      return;
    }
    else
    {
      /*Perform the final steps for successful completion of the
      procedure */
      tdsrrcpcrc_process_successful_proc_completion(TRUE);

      /* return from here */
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
        tdsrrcrb_send_ura_update_initiate_req( TDSRRC_PROCEDURE_PCR,
                                          TDSRRC_URA_RESELECTION,
                                          FALSE);
      }
    }
    /* send ul data registration to rlc. */
    tdsrrcpcrc_process_successful_proc_completion(TRUE);

    /* return from here */
    return;
  }

  if(ptr->inter_freq_hho_status == TRUE)
  {
    tdsrrc_pcrc.hho_ciph_status = TRUE;
  }
  else
  {
    tdsrrc_pcrc.hho_ciph_status = FALSE;  
  }
  
  /*check whether this reconfiguration involves SRNS relocation */
  if (tdsrrc_pcrc.pcrc_for_srns_relocation == TRUE)
  {
    if (TDSRRC_STATE_CELL_DCH == curr_state)
    {

      /* Re-establish SRB2 before sending the response message */
      rb_type  = TDSRRC_RE_EST_RB_2;
      tdsrrcrb_send_rlc_re_est_req_to_smc(TDSRRC_PROCEDURE_PCR, rb_type, TRUE);
  
      tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_REESTABLISH_SRB2;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"SRNS not supp. in state %d",curr_state);
    }
  }
  else /*normal reconfiguration without SRNS relocation */
  {   
    /* Send the Radio Bearer Reconfig Complete message */

    if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
        (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

    {
      tdsrrcpcrc_send_pcrc_complete_message(TRUE);
    }
    else
    {
    /* Need to wait for L2 ack for all states because RRC needs to trigger include reconfig status indicator
    if RLC acknowledgement is not yet received.
    As per 8.3.1.3
    1> if the IE "Cell update cause" is set to "radio link failure" and the UE has submitted a reconfiguration response
    message to RLC and the UE has not received the RLC acknowledgement for the response message:
    2> include the IE "Reconfiguration Status Indicator" and set it to TRUE.*/
        tdsrrc_update_reconfig_waiting_for_l2ack_dch(TDSRRC_PROCEDURE_PCR);
        tdsrrcpcrc_send_pcrc_complete_message(TRUE);
      if((tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG)
        && (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH))
      { 
        /* wait for some time before initiating cell update */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delaying by %d ms ",TDSRRCRB_PND_CFG_DELAY);
        (void) rex_clr_sigs(rex_self(), TDSRRCPCRC_TEMP_TIMER_SIG);
        (void) rex_set_timer(&tdsrrcpcrc_temp_timer, TDSRRCRB_PND_CFG_DELAY);  

        tdsrrcpcrc_sig = rex_wait(TDSRRCPCRC_TEMP_TIMER_SIG);
        TDSRRC_MSG1(MSG_LEGACY_LOW,"RRCPCRC signal:%d",tdsrrcpcrc_sig);
        (void) rex_clr_sigs(rex_self(), TDSRRCPCRC_TEMP_TIMER_SIG);
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_PCR, TDSRRC_UNRECOVERABLE_ERROR, FALSE);   
      } 
      else if((tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG) &&
      (tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) == TDSRRCCU_STARTED))
      {
        if((cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError) && (tdsrrccu_get_rb_id_unrec_error() != DCCH_AM_RADIO_BEARER_ID))
        {
          /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
          tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_PCR,
                                              TDSRRC_UNRECOVERABLE_ERROR,
                                              TRUE);
        }
        else if(cu_cause == tdsrrc_CellUpdateCause_radiolinkFailure)
        {
          /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
          tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_PCR,
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
    if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
        (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

    {
      /* Change the substate to the next substate - this is
      where we wait for L2 ack from RLC for sending the RB RC
      complete message. */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    }
    else
    {
      /* clear the ordered config here
      treat the procedure as successful. */
      tdsrrcllc_clear_ordered_config();  

      /* send the nas cn info if stored */
      if (tdsrrc_pcrc.cn_info_saved != FALSE)
      {
        /* Put the command on MM queue */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RRC_SERVICE_IND_FROM_OTA_MSG to MM");
        if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
        {
          tdscfa_mm_put_cmd(tdsrrc_pcrc.mm_cmd_ptr);
        }
        mm_put_cmd(tdsrrc_pcrc.mm_cmd_ptr);
        /* clear the flag */
        tdsrrc_pcrc.cn_info_saved = FALSE;
      }
      /* Go to the next sub-state in the state machine. That's
      the initial substate since we've sent the RB RC complete
      message */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

      /* At this point we don't need to remember any of the
      Radio Bearer information since it's stored in ESTABLISHED_RABS.
      Clear our internal variables */
      tdsrrcpcrc_clear_procedure();
    }
  
  }
}

/*===========================================================================

FUNCTION   RRCPCRC_PROCESS_RLC_REESTABLISHMENT_CNF

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
static void tdsrrcpcrc_process_rlc_reestablishment_cnf
(
void
)
{
  tdsrrcrb_rnti_update_info_type rnti_update_info; /* structure to be used to request RNTI update */

  /* send rnti update request if URNTI was present in the rebsetup message */
  if (tdsrrc_pcrc.new_urnti_valid == TRUE)
  {
    /* Send the RNTI update request to rrcllc to update with new-URNTI */
    /* Send a RNTI Update Request */
    rnti_update_info.crnti_action         = TDSRRC_RNTI_NOCHANGE;
    rnti_update_info.urnti_action         = TDSRRC_RNTI_UPDATE;
    rnti_update_info.urnti                = tdsrrc_pcrc.new_urnti;
    rnti_update_info.procedure            = TDSRRC_PROCEDURE_PCR;
    rnti_update_info.rlc_re_establish_srb = FALSE;
    rnti_update_info.rlc_re_establish_rab = FALSE;
    rnti_update_info.cnf_reqd             = FALSE;

    tdsrrcrb_send_rnti_update_req(rnti_update_info);
  }

  /* Send the reconfiguration complete message */

  tdsrrcpcrc_send_pcrc_complete_message(TRUE);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");

  /* Change the substate to the next substate - this is
     where we wait for L2 ack from RLC for sending the RB Reconfig
     complete message. */
  tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
  /* prints rate for each transport channel */
  tdsrrcllc_print_trans_chnl_rate();
  
}

/*===========================================================================

FUNCTION   RRCPCRC_PROCESS_STATE_CHANGE_IND

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
static void tdsrrcpcrc_process_state_change_ind
(
  tdsrrc_state_change_ind_type *ptr /* Pointer to the State Change Ind cmd */
)
{
  tdsrrc_CellUpdateCause cu_cause;
  /* Check if we're going to disconnected state */
  if( ptr->new_state == TDSRRC_STATE_DISCONNECTED )
  {
    /* Then change to initial substate */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcpcrc_clear_procedure();
  }

  else if(ptr->new_state == TDSRRC_STATE_CELL_FACH )
  {
    if(ptr->previous_state == TDSRRC_STATE_CELL_DCH)
    {
      if (((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
          (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)) &&
           (tdsrrc_pcrc.curr_substate == TDSRRCRB_WAIT_FOR_L2_ACK))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"state change ind pcrc_st:%d",tdsrrc_pcrc.curr_substate); 
        /* Then change to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcpcrc_clear_procedure();
      }
      else if(tdsrrc_pcrc.status.failure_status == TDSRRCPC_MSG_VALID)
      {
        /* if cell update procedure is active with cause OOS, wait 
         * for the CU complete notification to send chan_config_req to
         * LLC 
         */
        if ((tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause)== TDSRRCCU_STARTED) &&
            (cu_cause == tdsrrc_CellUpdateCause_re_enteredServiceArea) && 
            (tdsrrcllc_get_toc_usage() == TDSTOC_FOR_OOS)) 
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Complete tdsrrc_pcrc msg first.  Indicate compl to CU once done");
        }

        TDSRRC_MSG0(MSG_LEGACY_MED,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
        /* Call the function that creates and sends the  channel
           config information to be requested from LLC. */
        tdsrrcpcrc_send_channel_config_req();
        /* Change the substate since we're waiting for the
           confirmation from LLC for the channel configs */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;

        if((tdsrrccsp_is_selected_cell_utran_directed()) &&
           (tdsrrc_pcrc.new_crnti_valid))
        {
          /* No need to initiate cell update as the C-RNTI is available
           * and UE is camped on the directed cell.
           */
          tdsrrcpcrc_send_initiate_cu_flag=FALSE;
        }
        else
        {
          tdsrrcpcrc_send_initiate_cu_flag=TRUE;
          tdsrrcpcrc_delete_crnti = TRUE;
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
        if (tdsrrc_pcrc.directed_cell_info )
        {
          if (tdsrrccsp_is_selected_cell_utran_directed()) 
          {
            /* No need to initiate cell update UE is camped on 
            the directed cell. */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Directed cell match,No CU reqd");
            tdsrrcpcrc_send_initiate_cu_flag = FALSE;
          }
          else
          {
            tdsrrcpcrc_send_initiate_cu_flag = TRUE;
            tdsrrcpcrc_delete_crnti = TRUE;
          }
        }
        else
        {
          /* we have to initiate cell update incase cpich info is not present
          in the message */
          tdsrrcpcrc_send_initiate_cu_flag = TRUE;
          tdsrrcpcrc_delete_crnti = TRUE;
        }
      }
      else
      {
        /* URA_PCH case */
        tdsrrcpcrc_send_initiate_cu_flag = FALSE;
      }

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
      /* Call the function that creates and sends the  channel
         config information to be requested from LLC. */
      tdsrrcpcrc_send_channel_config_req();
  
      /* Change the substate since we're waiting for the
         confirmation from LLC for the channel configs */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
    }
 #ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
    else if ((ptr->previous_state == TDSRRC_STATE_CELL_FACH) &&
    (tdsrrc_pcrc.initiate_cell_selection_trans_from_fach == TRUE))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
      /* Call the function that creates and sends a channel
      config request to LLC. */
      tdsrrc_pcrc.initiate_cell_selection_trans_from_fach = FALSE;
      if (tdsrrc_pcrc.directed_cell_info )
      {
        if(tdsrrccsp_is_selected_cell_utran_directed())
        {
          /* No need to initiate cell update as the C-RNTI is available
           * and UE is camped on the directed cell.
           */
          tdsrrcpcrc_send_initiate_cu_flag = FALSE;
        }
        else
        {
          tdsrrcpcrc_send_initiate_cu_flag = TRUE;
          tdsrrcpcrc_delete_crnti = TRUE;
        }
      }
      else
      {
        tdsrrcpcrc_send_initiate_cu_flag = TRUE;
        tdsrrcpcrc_delete_crnti = TRUE;
      }
      tdsrrcpcrc_send_channel_config_req();
      /* Change the substate since we're waiting for the
      confirmation from LLC for the channel configs */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
    }
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Recvd state change %d to %d tdsrrc_pcrc state %d, No action",
                ptr->previous_state, ptr->new_state, tdsrrc_pcrc.curr_substate);
    }

    /* IF CU is waiting for RB proc to complete, no need to initate CU through RB procedure. Reset flag. */
    if (tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_RB_PROC_COMPL_IND) 
    {
      tdsrrcpcrc_send_initiate_cu_flag = FALSE;
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

FUNCTION   RRCPCRC_PROCESS_L2_ACK_IND

DESCRIPTION

  This function processes the received Layer 2 ACK indication.
  If the L2 ACK indicates that the PCRC complete message has
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
static void tdsrrcpcrc_process_l2_ack_ind
(
  tdsrrc_downlink_l2ack_ind_type *ptr /* Pointer to the L2 Ack Ind cmd */
)
{
  tdsrrc_cmd_type *cmd; /* To send a LLC command if L2 tx failed */
  tdsrrc_state_e_type                       current_state;       /* current RRC State  */

  tdsrrc_rlc_re_est_for_srns_rb_e_type rb_type; /*rb type tobe reestablished */
  tdsrrc_CellUpdateCause       cu_cause;
  uint8                    rejected_tr_id = TDSRRCPC_INVALID_TRANSACTION_ID;

  if( ptr->status == SUCCESS )
  {
    current_state = tdsrrc_get_state();

    /* Back up accepted transaction ID */
    rejected_tr_id = tdsrrc_pcrc.tr_ids.accepted_transaction;
    
    /* Clear the transaction ID */
    tdsrrc_pcrc.tr_ids.accepted_transaction = TDSRRCPC_INVALID_TRANSACTION_ID;
      
    if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
        (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
    {
      if(current_state==TDSRRC_STATE_CELL_DCH) 
      {
        /*L2 Ack for the PCRC Complete message is received on the old configuration
          Now initiate a cell selection */
        tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR,
                                                TDSRRC_TRANSITION_FROM_DCH,
                                                tdsrrc_pcrc.state_indicated_by_msg);

        /*wait for the state_change indication after the cell selection
        is successfully completed */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

        /* Start T_305 [periodic cell update] timer */
        tdsrrccu_start_t_305_timer();

      }
      else if(current_state==TDSRRC_STATE_CELL_FACH) 
      {
        if (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) &&
            (cu_cause == tdsrrc_CellUpdateCause_re_enteredServiceArea || cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError ||
             cu_cause == tdsrrc_CellUpdateCause_radiolinkFailure))
        {
          /* This is a case where cell update is in progress while PCR transitions 
             UE to Cell_PCH or URA_PCH. Though L2 Ack is gotten, send a PCR failure 
             later when CU procedure gets Cell update confirm. If we were to act on 
             L2 Ack, RRC moves to Cell_PCH/URA_PCH where cell update confirm message
             is not handled (unless CU registers on PCR's behalf) & UE would stuck 
             in Cell_PCH forever [in the absence of reselections].
          */
          if(SUCCESS != tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_PCR))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCR procedure failed to register with CU");
          }
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Registered for Cell Update procedure completion: cu_cause = %d",cu_cause);

          tdsrrc_pcrc.status.failure_status = TDSRRCPC_PHY_CHAN_FAILURE;
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;

          /* Restore the accepted transaction ID for sending failure later */
          tdsrrc_pcrc.tr_ids.rejected_transaction = rejected_tr_id;
        }
        else
        {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          if((tdsrrc_pcrc.initiate_cell_selection_trans_from_fach == TRUE )
            &&(tdsrrccsp_check_initiate_cell_selection_handling() == TRUE))
          {
            /*Initiate the cell selection*/
            tdsrrcllcoc_update_toc_with_oc_from_fach();   
             
            tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR,
                                                    TDSRRC_TRANSITION_FROM_FACH,
                                                    tdsrrc_pcrc.state_indicated_by_msg);
 
 
             
            tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
          }
          else
#endif
          {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
            tdsrrc_pcrc.initiate_cell_selection_trans_from_fach = FALSE;
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/     
            /* Call the function that creates and sends a channel
               config request to LLC. */
            tdsrrcpcrc_send_channel_config_req();
            /* Change the substate since we're waiting for the
               confirmation from LLC for the channel configs */
            tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
          }
        }
      }
      else
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid combination of rrcstate%d and rrcpcrc substate%d",current_state,tdsrrc_pcrc.curr_substate);
      }

      /*return from here */
      return;
    }
    /* if SRNS relocation is involved with this message then re
    establish the RBs other than RB2 here */
    if (tdsrrc_pcrc.pcrc_for_srns_relocation)
    {
      if (TDSRRC_STATE_CELL_DCH == tdsrrc_get_state())
      {
        /* Initiate the re-establishment of remaining RBs before 
        PCRC procedure goes back to initial substate. 
        No need to wait for the confirmation */
        rb_type  = TDSRRC_RE_EST_NON_RB_2_UM_AM;
        tdsrrcrb_send_rlc_re_est_req_to_smc(TDSRRC_PROCEDURE_PCR, rb_type, FALSE);

        /* send the nas cn info if stored */
        if (tdsrrc_pcrc.cn_info_saved != FALSE)
        {
          /* Put the command on MM queue */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RRC_SERVICE_IND_FROM_OTA_MSG to MM");
          if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
          {
            tdscfa_mm_put_cmd(tdsrrc_pcrc.mm_cmd_ptr);
          }
          mm_put_cmd(tdsrrc_pcrc.mm_cmd_ptr);
          /* clear the flag */
          tdsrrc_pcrc.cn_info_saved = FALSE;
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
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2 ack - transmission failed for PCRC Complete msg");
    
    /* Here make sure that CU is not active.  If CU is active with OOS area handling
       then NACK coming from RLC shall be ingnored as TCRC failure with cause
       CU complete should be sent to NW */
    if (TDSRRC_STATE_CELL_FACH == tdsrrc_get_state() &&
        TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) &&
        (cu_cause == tdsrrc_CellUpdateCause_re_enteredServiceArea || cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError ||
         cu_cause == tdsrrc_CellUpdateCause_radiolinkFailure))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NACK being ingnored as PCRC failure msg will be sent");
      (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_PCR);
       /* Wait until Cell update procedure is completed */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Registered for Cell Update procedure completion: cu_cause = %d",cu_cause);
      return;
    }

    /* Clear the transaction ID */
    tdsrrc_pcrc.tr_ids.accepted_transaction = TDSRRCPC_INVALID_TRANSACTION_ID;

    if (tdsrrc_pcrc.pcrc_for_srns_relocation) 
    {
      /* l2ack failed. If reconfiguration involves SARNS relocation then
      go to idle. Otherwise just complete the procedure */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"L2ack failed SRNSreloc%d,next_st:%d",tdsrrc_pcrc.pcrc_for_srns_relocation,tdsrrc_pcrc.state_indicated_by_msg);
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
        cmd->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_PCR;
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
  if (((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)) &&
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
      if(tdsrrcllc_reset_ordered_config(TDSRRC_PROCEDURE_PCR
                                     ,TRUE
                                     ) != TDSOC_NOT_SET)
      {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"OC not reset ");
      }
    }
  }
  else
  {
    /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
    configurations */
    tdsrrcllc_clear_ordered_config();  
  }
  
#if defined(FEATURE_3GPP_FAST_DORMANCY)
  if(tdsrrc_pcrc.pcrc_for_srns_relocation == TRUE)
  {
    tdsrrcdormancy_update_dormancy_for_srns_hho_cco(TDSRRC_PROCEDURE_PCR);
  }
#endif
  
  /* Go to the next sub-state in the state machine. That's
  the initial substate since we've sent the PCRC complete
  message */
  tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

  /*  Clear our internal variables */
  tdsrrcpcrc_clear_procedure();
}


/*===========================================================================

FUNCTION   RRCPCRC_INITIAL_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_INITIAL substate. This function
  looks at the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_initial_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the received command's id */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {

    case TDSRRC_DOWNLINK_SDU_IND:
        /* Process the PC Reconfig message */
      tdsrrcpcrc_process_pcrc_message( cmd_ptr );
      
      break;


    default:

      /* No other events except Downlink SDU is expected in this
      substate */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
              cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
      break;
  }
}
/*===========================================================================

FUNCTION   RRCPCRC_DCH_WFSRB2REESTABLISH_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the TDSRRCRB_WAIT_FOR_REESTABLISH_SRB2 substate
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
static void tdsrrcpcrc_dch_wfsrb2reestablish_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_RE_EST_RLC_FOR_SRNS_CNF:

    /* Confirmation from SMC of RLC re-establishment confirm */
    tdsrrcpcrc_process_rlc_reestablishment_cnf( );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Reconfig Message. Otherwise
    we would not have received this command. Copy the 
    received message pointer. */
    /* Process the RB Reconfig message */
    tdsrrcpcrc_process_pcrc_message( cmd_ptr );

    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
            cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
    break;
  }
}

/*===========================================================================

FUNCTION   RRCPCRC_DCH_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate
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
static void tdsrrcpcrc_dch_wfchcnf_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the received command's id */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {

    case TDSRRC_CHANNEL_CONFIG_CNF:
      
      /* Confirmation from LLC for the channel configurations */
      tdsrrcpcrc_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
      break;

    case TDSRRC_STATE_CHANGE_IND:
      
      /* Indication that RRC's state has changed */
      tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;

    case TDSRRC_DOWNLINK_SDU_IND:
      
        /* Process the PC Reconfig message */
      tdsrrcpcrc_process_pcrc_message( cmd_ptr);
          
      break;

    default:

      /* No other events expected in this
      substate */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
              cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
      break;
  }
}


/*===========================================================================

FUNCTION   tdsrrcpcrc_other_wfcucnf_substate_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate
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
static void tdsrrcpcrc_other_wfcucnf_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_STATE_CHANGE_IND:
      /* Indication that RRC's state has changed */
      tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;
    
    default:    
      /* No other events expected in this
      substate */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
              cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
      break;
  }
}


/*===========================================================================

FUNCTION   RRCPCRC_DCH_WFCUCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate
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

static void tdsrrcpcrc_dch_wfcucnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      if(tdsrrc_pcrc.status.failure_status == TDSRRCPC_SIMULTANEOUS_RECONFIG)
      {
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message(tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause
                                );
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcpcrc_clear_procedure();

        /* Make sure we're back in the initial sub-state */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
      }
      else if(tdsrrc_pcrc.status.failure_status == TDSRRCPC_MSG_VALID)
      {
        /* Send the TC Reconfig Complete message */
        if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
            (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

        {
          tdsrrcpcrc_send_pcrc_complete_message(TRUE);
        }
        else
        {
          tdsrrcpcrc_send_pcrc_complete_message(FALSE);
        }
        /* prints rate for each transport channel */
        tdsrrcllc_print_trans_chnl_rate();
        /*if next state is Cell_PCH or URA_PCH then we have to 
        treat the procedure as successful only after getting L2ack
        for the response message */
        if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
            (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
          /* Change the substate to the next substate - this is
          where we wait for L2 ack from RLC for sending the RB RC
          complete message. */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
        }
        else
        {
          /* clear the ordered config here
          treat the procedure as successful. */
          tdsrrcllc_clear_ordered_config();  
          /* send the nas cn info if stored */
          if (tdsrrc_pcrc.cn_info_saved != FALSE)
          {
            /* Put the command on MM queue */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RRC_SERVICE_IND_FROM_OTA_MSG to MM");
            if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
            {
              tdscfa_mm_put_cmd(tdsrrc_pcrc.mm_cmd_ptr);
            }
            mm_put_cmd(tdsrrc_pcrc.mm_cmd_ptr);
            /* clear the flag */
            tdsrrc_pcrc.cn_info_saved = FALSE;
          }

          /* Go to the next sub-state in the state machine. That's
          the initial substate since we've sent the RB RC complete
          message */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

          /* At this point we don't need to remember any of the
          Radio Bearer information since it's stored in ESTABLISHED_RABS.
          Clear our internal variables */
          tdsrrcpcrc_clear_procedure();
        }
      }
      /*  In the case of Phy_Chan_Failure, RB setup failure must be send
          and oc should be reset. */
      else if(tdsrrc_pcrc.status.failure_status == TDSRRCPC_PHY_CHAN_FAILURE)
      {
        
        /* Send the RB Setup Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause
                                );

        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcpcrc_clear_procedure();
        /* Make sure we're back in the initial sub-state */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure status %d not handled",tdsrrc_pcrc.status.failure_status);
      }

    break;

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcpcrc_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* Process the PC Reconfig message */
    tdsrrcpcrc_process_pcrc_message( cmd_ptr );
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
            cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
    break;
  }

}


/*===========================================================================

FUNCTION   RRCPCRC_FACH_WFCUCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate
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

static void tdsrrcpcrc_fach_wfcucnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      if(tdsrrc_pcrc.status.failure_status == TDSRRCPC_SIMULTANEOUS_RECONFIG)
      {
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message(tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause
                                );
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcpcrc_clear_procedure();

        /* Make sure we're back in the initial sub-state */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
      }
      else if(tdsrrc_pcrc.status.failure_status == TDSRRCPC_MSG_VALID)
      {
        /* if OOS happens while waiting for cell update confirm then send  
         * channel config request again to reset TOC and to configure lower layers*/
        if ((tdsrrcllc_get_toc_usage() == TDSTOC_FOR_OOS))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TOC usage TOC_FOR_OOS sending channel config req ");

          /* Call the function that creates and sends the  channel
             config information to be requested from LLC. */
          tdsrrcpcrc_send_channel_config_req();
  
          /* Change the substate since we're waiting for the
             confirmation from LLC for the channel configs */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;	
          return;
        }
        /* Send the TC Reconfig Complete message */
        if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
            (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
          
        {
          tdsrrcpcrc_send_pcrc_complete_message(TRUE);
        }
        else
        {
          tdsrrcpcrc_send_pcrc_complete_message(FALSE);
        }
        /*if next state is Cell_PCH or URA_PCH then we have to 
          treat the procedure as successful only after getting L2ack
          for the response message */
        if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
            (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
          
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
          /* Change the substate to the next substate - this is
             where we wait for L2 ack from RLC for sending the RB RC
             complete message. */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
        }
        else
        {
          /* clear the ordered config here
             treat the procedure as successful. */
          tdsrrcllc_clear_ordered_config();  
          /* send the nas cn info if stored */
          if (tdsrrc_pcrc.cn_info_saved != FALSE)
          {
            /* Put the command on MM queue */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RRC_SERVICE_IND_FROM_OTA_MSG to MM");
            if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
            {
              tdscfa_mm_put_cmd(tdsrrc_pcrc.mm_cmd_ptr);
            }
            mm_put_cmd(tdsrrc_pcrc.mm_cmd_ptr);
            /* clear the flag */
            tdsrrc_pcrc.cn_info_saved = FALSE;
          }

          /* Go to the next sub-state in the state machine. That's
             the initial substate since we've sent the RB RC complete
             message */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
            
          /* At this point we don't need to remember any of the
             Radio Bearer information since it's stored in ESTABLISHED_RABS.
             Clear our internal variables */
          tdsrrcpcrc_clear_procedure();
        }
      }
      /*  In the case of Phy_Chan_Failure, RB setup failure must be send
          and oc should be reset. */
      else if(tdsrrc_pcrc.status.failure_status == TDSRRCPC_PHY_CHAN_FAILURE)
      {
        
        /* Send the RB Setup Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause
                                );


        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcpcrc_clear_procedure();
        /* Make sure we're back in the initial sub-state */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure status %d not handled",tdsrrc_pcrc.status.failure_status);
      }

    break;

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcpcrc_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* Process the PC Reconfig message */
    tdsrrcpcrc_process_pcrc_message( cmd_ptr );
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
            cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
    break;
  }

}
/*===========================================================================

FUNCTION   RRCPCRC_PCH_WF_CSCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF substate
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

static void tdsrrcpcrc_pch_wfcscnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_STATE_CHANGE_IND:
   /* Indication that RRC's state has changed */
    tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
   
    break;


  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
            cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
    break;
  }

}



/*===========================================================================

FUNCTION   RRCPCRC_PCH_WFCUCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate
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
static void tdsrrcpcrc_pch_wfcucnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    if(tdsrrc_pcrc.status.failure_status != TDSRRCPC_MSG_VALID)
    {
      /* Send the PC Reconfig Failure Message */
      tdsrrcpcrc_send_pcrc_failure_message(tdsrrc_pcrc.status.failure_status,
                                tdsrrc_pcrc.status.prot_err_cause
                              );
      /* Then clear all local variables that may
      need to be cleared */
      tdsrrcpcrc_clear_procedure();

      /* Make sure we're back in the initial sub-state */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
    }
    else 
    {
      /* Send the TC Reconfig Complete message */
      if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
          (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

      {
        tdsrrcpcrc_send_pcrc_complete_message(TRUE);
      }
      else
      {
        tdsrrcpcrc_send_pcrc_complete_message(FALSE);
      }
      /* prints rate for each transport channel */
      tdsrrcllc_print_trans_chnl_rate();
      /*if next state is Cell_PCH or URA_PCH then we have to 
      treat the procedure as successful only after getting L2ack
      for the response message */
      if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
          (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))

      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
        /* Change the substate to the next substate - this is
        where we wait for L2 ack from RLC for sending the RB RC
        complete message. */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
      }
      else
      {
        /* clear the ordered config here
        treat the procedure as successful. */
        tdsrrcllc_clear_ordered_config();  

        /* Go to the next sub-state in the state machine. That's
        the initial substate since we've sent the RB RC complete
        message */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

        /* At this point we don't need to remember any of the
        Radio Bearer information since it's stored in ESTABLISHED_RABS.
        Clear our internal variables */
        tdsrrcpcrc_clear_procedure();
      }
    }
    break;

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcpcrc_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:
    /* Indication that RRC's state has changed */
    tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

 

    case TDSRRC_DOWNLINK_SDU_IND:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Unexpected DOWNLINK_SDU_IND in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_pcrc.curr_substate);
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
            cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
    break;
  }

}

/*===========================================================================

FUNCTION   RRCPCRC_PCH_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate
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
static void tdsrrcpcrc_pch_wfchcnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{


  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
  
  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcpcrc_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:
  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
            cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
    break;
  }
}


/*===========================================================================

FUNCTION   RRCPCRC_FACH_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate
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
static void tdsrrcpcrc_fach_wfchcnf_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {

    case TDSRRC_CHANNEL_CONFIG_CNF:
      
      /* Confirmation from LLC for the channel configurations */
      tdsrrcpcrc_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
      break;

    case TDSRRC_STATE_CHANGE_IND:
      
      /* Indication that RRC's state has changed */
      tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;

    case TDSRRC_DOWNLINK_SDU_IND:
      
        /* Process the PC Reconfig message */
        // Reject - upen ? 
      tdsrrcpcrc_process_pcrc_message( cmd_ptr );
      
      break;

    default:

      /* No other events expected in this
      substate */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
              cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
      break;
  }
}



/*===========================================================================

FUNCTION   RRCPCRC_OTHER_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate
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
static void tdsrrcpcrc_other_wfchcnf_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the received command's id */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {

    case TDSRRC_CHANNEL_CONFIG_CNF:
      
      /* Confirmation from LLC for the channel configurations */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring chan config cnf since UE not in DCH or FACH");
      /* In this state this procedure should no longer be active. */

      /* Then change to initial substate */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

      /* Then clear all local variables that may
      need to be cleared */
      tdsrrcpcrc_clear_procedure();
      break;

    case TDSRRC_STATE_CHANGE_IND:
      
      /* Indication that RRC's state has changed */
      tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;

    case TDSRRC_DOWNLINK_SDU_IND:
      
      /* A Downlink SDU has been received from RLC. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring downlink sdu since UE not in DCH or FACH");
      /* In this state this procedure should no longer be active. */

      /* Then change to initial substate */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

      /* Then clear all local variables that may
      need to be cleared */
      tdsrrcpcrc_clear_procedure();
      break;

    default:

      /* No other events expected in this
      substate */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
              cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
      break;
  }
}



/*===========================================================================

FUNCTION   RRCPCRC_DCH_WFL2ACK_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_L2_ACK substate
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
static void tdsrrcpcrc_dch_wfl2ack_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {

    case TDSRRC_DOWNLINK_L2ACK_IND:
      
      /* Confirmation from LLC for the channel configurations */
      tdsrrcpcrc_process_l2_ack_ind( &(cmd_ptr->cmd.downlink_l2ack_ind) );
      break;

    case TDSRRC_STATE_CHANGE_IND:
      
      /* Indication that RRC's state has changed */
      tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;

    case TDSRRC_DOWNLINK_SDU_IND:
      
        /* Process the PC Reconfig message */
      tdsrrcpcrc_process_pcrc_message( cmd_ptr );
      
      break;

    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      /* The only reason this scenario could occur is if while in wait_for_l2_ack substate oos area
         happened. In this case, send failure msg with cause cell update occured 
      */
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_CELL_UPDATE_OCCURED;

      /* Send the PC Reconfig Failure Message */
      tdsrrcpcrc_send_pcrc_failure_message(tdsrrc_pcrc.status.failure_status,
                                tdsrrc_pcrc.status.prot_err_cause
                                );

      /* Then clear all local variables that may
      need to be cleared */
      tdsrrcpcrc_clear_procedure();

      /* Make sure we're back in the initial sub-state */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
      break;

    default:

      /* No other events except Downlink SDU is expected in this
      substate */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
              cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
      break;
  }
}


/*===========================================================================

FUNCTION   RRCPCRC_FACH_WFL2ACK_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_L2_ACK substate
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
static void tdsrrcpcrc_fach_wfl2ack_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  
  /* Switch on the received command's id */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {

    case TDSRRC_DOWNLINK_L2ACK_IND:
      
      /* Confirmation from LLC for the channel configurations */
      tdsrrcpcrc_process_l2_ack_ind( &(cmd_ptr->cmd.downlink_l2ack_ind) );
      break;

    case TDSRRC_STATE_CHANGE_IND:
      
      /* Indication that RRC's state has changed */
      tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;

    case TDSRRC_DOWNLINK_SDU_IND:
      
        /* Process the PC Reconfig message */
      tdsrrcpcrc_process_pcrc_message( cmd_ptr );
      
      break;
  
    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      /* The only reason this scenario could occur is if while in wait_for_l2_ack substate oos area
         happened. In this case, send failure msg with cause cell update occured 
      */
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_CELL_UPDATE_OCCURED;
      /* Send the PC Reconfig Failure Message */
      tdsrrcpcrc_send_pcrc_failure_message(tdsrrc_pcrc.status.failure_status,
                                tdsrrc_pcrc.status.prot_err_cause
                                );
      /* Then clear all local variables that may
      need to be cleared */
      tdsrrcpcrc_clear_procedure();

      /* Make sure we're back in the initial sub-state */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
      break;

    default:

      /* No other events except Downlink SDU is expected in this
      substate */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
              cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
      break;
  }
}



/*===========================================================================

FUNCTION   RRCPCRC_FACH_WFCSCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF substate
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
static void tdsrrcpcrc_fach_wfcscnf_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
   /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

    case TDSRRC_STATE_CHANGE_IND:
     /* Indication that RRC's state has changed */
      tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
    case TDSRRC_NEW_CELL_IND:
      tdsrrcpcrc_process_new_cell_ind( &(cmd_ptr->cmd.new_cell_ind) );
      break;

    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      if(tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_FACH)
      {
        /* if OOS was detected during trans from DCH, cell update
           procedure has completed.  send channel config req to 
           complete the reconfiguration procedure
          */
        if( (tdsrrcllc_get_toc_usage() ==TDSTOC_FOR_OOS) 
            || (tdsrrcllc_get_toc_usage() ==TDSTOC_FOR_OOS_WITHOUT_DCH_INFO)
            ||(tdsrrcllc_get_toc_usage() ==TDSTOC_FOR_OOS_WITH_DCH_INFO)
         )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
          /* Call the function that creates and sends the  channel
          config information to be requested from LLC. */
          tdsrrcpcrc_send_channel_config_req();
          /* Also reset the tdsrrcpcrc_send_initiate_cu_flag to false as cell update
           * will not be needed since CU and CUCNF for oos took care 
           * of that
           */
          tdsrrcpcrc_send_initiate_cu_flag = FALSE;
          /* Change the substate since we're waiting for the
          confirmation from LLC for the channel configs */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
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
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
              cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
      break;
   }


}
/*===========================================================================

FUNCTION   RRCPCRC_FACH_WFCRNTIUPDATECNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF substate
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

static void tdsrrcpcrc_fach_wfcrntiupdatecnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_STATE_CHANGE_IND:
   /* Indication that RRC's state has changed */
    tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
   
    break;


  case TDSRRC_RNTI_UPDATE_CNF:
    if (cmd_ptr->cmd.rnti_update_cnf.status == SUCCESS) 
    {
      tdsrrcpcrc_process_rnti_update_cnf();
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RNTI update failed.  Send PCRC failure msg");
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_SIMULTANEOUS_RECONFIG;
      /* Send the TCRC Failure Message */
      tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                 tdsrrc_pcrc.status.prot_err_cause );

      
      /* Then clear all local variables that may
         need to be cleared */
      tdsrrcpcrc_clear_procedure();

      /* Make sure we're back in the initial sub-state */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
    }
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command 0x%x ignored in substate%d rrc_state%d", 
            cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
    break;
  }

}  /* tdsrrcpcrc_fach_wfcrntiupdatecnf_substate_event_handler */

/*===========================================================================

FUNCTION   RRCPCRC_OTHER_WFL2ACK_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the PCRC procedure is in the TDSRRCRB_WAIT_FOR_L2_ACK substate
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
static void tdsrrcpcrc_other_wfl2ack_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  
  /* Switch on the received command's id */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {

    case TDSRRC_DOWNLINK_L2ACK_IND:
      
      /* Confirmation from LLC for the channel configurations */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring L2 ack since UE not in DCH or FACH");
      /* In this state this procedure should no longer be active. */

      /* Then change to initial substate */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

      /* Then clear all local variables that may
      need to be cleared */
      tdsrrcpcrc_clear_procedure();
      break;

    case TDSRRC_STATE_CHANGE_IND:
      
      /* Indication that RRC's state has changed */
      tdsrrcpcrc_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;

    case TDSRRC_DOWNLINK_SDU_IND:
      
      /* A Downlink SDU has been received from RLC.*/
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring downlink SDU since UE not in DCH or FACH");
      /* In this state this procedure should no longer be active. */

      /* Then change to initial substate */
      tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

      /* Then clear all local variables that may
      need to be cleared */
      tdsrrcpcrc_clear_procedure();
      break;

    default:

      /* No other events except Downlink SDU is expected in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring PCRC event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_pcrc.curr_substate);
      break;
  }
}





/*===========================================================================

FUNCTION   RRCPCRC_CELL_DCH_EVENT_HANDLER

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
static void tdsrrcpcrc_cell_dch_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  if ((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_L2ACK_IND) &&
      (tdsrrc_pcrc.curr_substate != TDSRRCRB_WAIT_FOR_L2_ACK))
  {
    /* We could get L2ack for a previously sent message while we are already
    processing the next message. So l2ack could be received any any of the following
    substates. If PCrc substate does not indicate that we are not waiting for l2ack
    then just print the status */
    if (cmd_ptr->cmd.downlink_l2ack_ind.status == SUCCESS)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Recd L2ack for MUI:%d, Lc_Id:%d, pcrc_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui, 
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_pcrc.curr_substate);
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2ack failed for MUI:%d, Lc_Id:%d, pcrc_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui, 
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_pcrc.curr_substate);
    }
    /* return from here so that procedure can continue with existing process */
    return;
  }
  /* Switch on the current substate */
  switch( tdsrrc_pcrc.curr_substate )
  {
    case TDSRRCRB_INITIAL:
     
      /* Call the event handler for TDSRRCRB_INITIAL substate */
      tdsrrcpcrc_initial_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:
     
      /* Call the event handler for TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate */
      tdsrrcpcrc_dch_wfchcnf_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCRB_WAIT_FOR_L2_ACK:
     
      /* Call the event handler for TDSRRCRB_WAIT_FOR_L2_ACK substate */
      tdsrrcpcrc_dch_wfl2ack_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCRB_WAIT_FOR_REESTABLISH_SRB2:
      tdsrrcpcrc_dch_wfsrb2reestablish_substate_event_handler(cmd_ptr);
      break;

    case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
      /* Call the event handler for TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate */
      tdsrrcpcrc_dch_wfcucnf_event_handler( cmd_ptr);
      break;

    default:

      /* No other substates processed for this RRC state    */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Command %x not processed in CELL_DCH for substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_pcrc.curr_substate);
      break;

  }
}

                                                                  

/*===========================================================================

FUNCTION   RRCPCRC_CELL_FACH_EVENT_HANDLER

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
static void tdsrrcpcrc_cell_fach_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  if ((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_L2ACK_IND) &&
      (tdsrrc_pcrc.curr_substate != TDSRRCRB_WAIT_FOR_L2_ACK))
  {
    /* We could get L2ack for a previously sent message while we are already
    processing the next message. So l2ack could be received any any of the following
    substates. If PCrc substate does not indicate that we are not waiting for l2ack
    then just print the status */
    if (cmd_ptr->cmd.downlink_l2ack_ind.status == SUCCESS)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Recd L2ack for MUI:%d, Lc_Id:%d, pcrc_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui, 
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_pcrc.curr_substate);
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2ack failed for MUI:%d, Lc_Id:%d, pcrc_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui, 
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_pcrc.curr_substate);
    }
    /* return from here so that procedure can continue with existing process */
    return;
  }
  /* Switch on the current substate */
  switch( tdsrrc_pcrc.curr_substate )
  {
    case TDSRRCRB_INITIAL:
      /* Call the event handler for TDSRRCRB_INITIAL substate */
      /* NOTE: Here we need to check if a cell-update procedure
      has been initialized or not. For now, just call the event handler */
      tdsrrcpcrc_initial_substate_event_handler( cmd_ptr );
      break;
   
    case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
      /* Call the event handler for TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate */
      tdsrrcpcrc_fach_wfcucnf_event_handler( cmd_ptr);
      break;

    case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:
      /* Call the event handler for TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate */
      tdsrrcpcrc_fach_wfchcnf_substate_event_handler( cmd_ptr );
      break;

  case  TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF:
      /* Call the event handler for TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate */
      tdsrrcpcrc_fach_wfcscnf_substate_event_handler( cmd_ptr );
      break;


    case TDSRRCRB_WAIT_FOR_L2_ACK:
      /* Call the event handler for TDSRRCRB_WAIT_FOR_L2_ACK substate */
      tdsrrcpcrc_fach_wfl2ack_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF:
        /* Call the event handler for TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF substate */
        tdsrrcpcrc_fach_wfcrntiupdatecnf_substate_event_handler( cmd_ptr );
        break;

    default:
      /* No other substates processed for this RRC state    */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Command %x not processed in CELL_FACH for substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_pcrc.curr_substate);
      break;
  }
}


/*===========================================================================

FUNCTION   RRCPCRC_PCH_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when RRC
  is in CELL_PCH state. This module checks the current sub-state of
  this procedure and calls the appropriate sub-state function to
  process the received event.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcpcrc_pch_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the current substate */
  switch ( tdsrrc_pcrc.curr_substate )
  {
  case TDSRRCRB_INITIAL:

    /* Call the event handler for TDSRRCRB_INITIAL substate */
    tdsrrcpcrc_initial_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
    /* Call the event handler for TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate */
    tdsrrcpcrc_pch_wfcucnf_event_handler( cmd_ptr);
    break;

  case TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF:

    tdsrrcpcrc_pch_wfcscnf_event_handler( cmd_ptr);
    break;

  case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:

    /* Call the event handler for TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF substate */
    tdsrrcpcrc_pch_wfchcnf_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_L2_ACK:

    /* The only way we could be in pch and rb state is "wait_for_l2_ack" is if
       oos happened while waiting for l2 ack and cell update confirm takes UE to pch
       In this case, send failure msg of tdsrrc_rbrc with cause cell update occured 
    */
    tdsrrc_pcrc.status.failure_status = TDSRRCPC_CELL_UPDATE_OCCURED;

    tdsrrcpcrc_send_pcrc_failure_message(tdsrrc_pcrc.status.failure_status,
                              tdsrrc_pcrc.status.prot_err_cause);

    /* Then clear all local variables that may
       need to be cleared */
    tdsrrcpcrc_clear_procedure();

    /* Make sure we're back in the initial sub-state */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
    break;


  default:

    /* No other substates processed for this RRC state    */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Command %x not processed in substate%d rrc_state%d", 
            cmd_ptr->cmd_hdr.cmd_id, tdsrrc_pcrc.curr_substate, tdsrrc_get_state());
    break;

  }
}


/*===========================================================================

FUNCTION   RRCPCRC_OTHER_STATE_EVENT_HANDLER

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
static void tdsrrcpcrc_other_state_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the current substate */
  switch( tdsrrc_pcrc.curr_substate )
  {
    
    case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:
     
      /* Call the event handler for WAIT_FOR_CHAN_CFG_CNF substate */
      tdsrrcpcrc_other_wfchcnf_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCRB_WAIT_FOR_L2_ACK:
     
      /* Call the event handler for WAIT_FOR_L2_ACK substate */
      tdsrrcpcrc_other_wfl2ack_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
      tdsrrcpcrc_other_wfcucnf_substate_event_handler( cmd_ptr);
      break;

    default:

      /* No other substates processed for this RRC state    */
      TDSRRC_MSG2(MSG_LEGACY_MED,"Command %x not processed in IDLE or PCH states for substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_pcrc.curr_substate);
      break;
  }
}



/*===========================================================================

FUNCTION RRCPCRC_PROCEDURE_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  PCRC procedure. This function is called by the
  dispatcher whenever an event is received for this module. This function
  checks the current RRC state and calls the event handler for
  that state. 
  
  Note that the PCRC procedure can be initiated
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
void tdsrrcpcrc_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  /* Get the current RRC State */
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

   /* Process State Change to Idle Disconnected in all substates */
  if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_STATE_CHANGE_IND) &&
     (cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED ))
  {
    /* Then change to initial substate */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcpcrc_clear_procedure();
  }
  switch (rrc_state)
  {

    case TDSRRC_STATE_CELL_FACH:    
      /* If it's CELL_FACH state, call the event handler
      for CELL_FACH state */
      tdsrrcpcrc_cell_fach_event_handler(cmd_ptr);
      break;

    case TDSRRC_STATE_CELL_DCH:    
      /* If it's CELL_DCH state, call the event handler
      for CELL_DCH state */
      tdsrrcpcrc_cell_dch_event_handler(cmd_ptr);
      break;

    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:
      /* If it's CELL_DCH state, call the event handler
      for CELL_PCH state */
      tdsrrcpcrc_pch_event_handler(cmd_ptr);
      break;

    case TDSRRC_STATE_CONNECTING:   
    case TDSRRC_STATE_DISCONNECTED: 
      /* If it's any other state call the event handler
      for other states. The processing of events in
      other RRC states is identical for this procedure */
      tdsrrcpcrc_other_state_event_handler(cmd_ptr);
      break;

    default:  
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC State: %d",rrc_state);
      break;
  }  
}/* end function tdsrrcpcrc_procedure_event_handler */


/*===========================================================================

FUNCTION tdsrrcpcrc_build_pcr_complete_message

DESCRIPTION
  This function builds the PCR Complete message.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcpcrc_build_pcr_complete_message
(
  tdsrrc_RRC_TransactionIdentifier trans_id,   /* Transaction Id */
  tdsrrc_UL_DCCH_Message *msg_ptr,             /* Pointer to Uplink DCCH Msg */
  boolean              srns_reloc,          /* Indicates whether SRNS reloc is involved */ 
  boolean            cipher_update_required /* Indicates whether TM ciphering is present */
)
{                                         

  /* Select the message as PCR Complete message */
  msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_physicalChannelReconfigurationComplete;

  /* Initialize the bit mask not to include the optional fields */
 TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.physicalChannelReconfigurationComplete);

  if (srns_reloc == TRUE)
  {
    tdsrrcpcrc_append_start_list_to_pcrc_complete(msg_ptr);
  }

  if((cipher_update_required == TRUE ) && (TRUE == tdsrrcsmc_tm_ciph_active()))
  {
    if(! (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.physicalChannelReconfigurationComplete,
           tdsrrc_PhysicalChannelReconfigurationComplete,ul_CounterSynchronisationInfo)))
    {
      tdsrrcpcrc_append_start_list_to_pcrc_complete(msg_ptr);
    }
  
    //Set the bitmask for count_c_act_time and copy the same..
    TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.physicalChannelReconfigurationComplete,
      tdsrrc_PhysicalChannelReconfigurationComplete,count_C_ActivationTime);
    
    msg_ptr->message.u.physicalChannelReconfigurationComplete.count_C_ActivationTime =
      (tdsrrc_ActivationTime)tdsrrcsmc_step_2_cipher_config_for_hho();  
  }

  
  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
    if(tdsrrcsib_is_sib11_11bis_12_read_deferred(TDSDEFERRED_ANY_ALLOWED_SIB))
    {
      msg_ptr->message.u.physicalChannelReconfigurationComplete.m.laterNonCriticalExtensionsPresent =1;
   
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.physicalChannelReconfigurationComplete.
	  	laterNonCriticalExtensions);
   
      msg_ptr->message.u.physicalChannelReconfigurationComplete.laterNonCriticalExtensions.m.
	  	v770NonCriticalExtensionsPresent=1;
   
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.physicalChannelReconfigurationComplete.laterNonCriticalExtensions.
	  	v770NonCriticalExtensions);
      
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.physicalChannelReconfigurationComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        physicalChannelReconfigurationComplete_v770ext);
   
      msg_ptr->message.u.physicalChannelReconfigurationComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        physicalChannelReconfigurationComplete_v770ext.m.deferredMeasurementControlReadingPresent =1;
   
      msg_ptr->message.u.physicalChannelReconfigurationComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
        physicalChannelReconfigurationComplete_v770ext.deferredMeasurementControlReading = 
        tdsrrc_PhysicalChannelReconfigurationComplete_v770ext_IEs_deferredMeasurementControlReading_true_;
    }
#endif
   }

  /* Initialize the Transaction Id */
  msg_ptr->message.u.physicalChannelReconfigurationComplete.
  rrc_TransactionIdentifier = trans_id;
} /* tdsrrcpcrc_build_pcr_complete_message */



/*===========================================================================

FUNCTION  RRCPCRC_INIT_PROCEDURE

DESCRIPTION

  This function initializes the PCRC procedure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcpcrc_init_procedure( void )
{
//  uint32 count;

  /* Initialize the transaction id's for this procedure.
  Use a number that can't be an actual received transaction
  id. */
  tdsrrc_pcrc.tr_ids.accepted_transaction = TDSRRCPC_INVALID_TRANSACTION_ID;
  tdsrrc_pcrc.tr_ids.rejected_transaction = TDSRRCPC_INVALID_TRANSACTION_ID;

  tdsrrc_pcrc.pcrc_for_srns_relocation = FALSE;
  tdsrrc_pcrc.new_urnti_valid = FALSE;


  tdsrrc_pcrc.hho_ciph_status = FALSE;

  /* set cn_info_saved to false */
  tdsrrc_pcrc.cn_info_saved = FALSE;

  /* Initialize the current substate */
  tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

  /* Clear the C-RNTI valid flag */
  tdsrrc_pcrc.new_crnti_valid    = FALSE;
  tdsrrc_pcrc.directed_cell_info = FALSE;

  /* Reset Initiate Cell Update Flag */
  tdsrrcpcrc_send_initiate_cu_flag = FALSE;


  tdsrrc_pcrc.high_mobility_ind = FALSE;

  /* Register for state change notification -
  We want to be notified when the state changes
  from CELL_DCH to CELL_FACH and from any state
  to disconnected state. */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_PCR,  /* Procedure name */
                             TDSRRC_STATE_CELL_DCH, /* From State     */
                             TDSRRC_STATE_CELL_FACH /* To State       */
                           );

  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_PCR,  /* Procedure name */
                             TDSRRC_STATE_WILDCARD, /* From State     */
                             TDSRRC_STATE_DISCONNECTED /* To State    */
                           );

  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_PCR,  /* Procedure name */
                             TDSRRC_STATE_CELL_DCH, /* From State     */
                             TDSRRC_STATE_CELL_PCH /* To State       */
                           );
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_PCR,  /* Procedure name */
                             TDSRRC_STATE_CELL_DCH, /* From State     */
                             TDSRRC_STATE_URA_PCH /* To State       */
                           );
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU

  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_PCR,  /* Procedure name */
                             TDSRRC_STATE_CELL_FACH, /* From State     */
                             TDSRRC_STATE_CELL_PCH /* To State       */
                           );

  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_PCR,  /* Procedure name */
                             TDSRRC_STATE_CELL_FACH, /* From State     */
                             TDSRRC_STATE_URA_PCH /* To State       */
                           );
  tdsrrc_pcrc.initiate_cell_selection_trans_from_fach = FALSE;     
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

  /* Define a timer used to delay 5 seconds before
  sending the PC Reconfig Complete message */
  rex_def_timer(&tdsrrcpcrc_temp_timer, rex_self(), TDSRRCPCRC_TEMP_TIMER_SIG);

}

// #endif


/*===========================================================================

FUNCTION   PROCESS_PCRC_R4

DESCRIPTION

  This function processes the received Rel 4 PC Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a PCRC Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to reconfigure the Physical Channel given by the message.
    
    If TDSORDERED_CONFIG could not be set, a PCRC Failure message
    will be sent to UTRAN.

    The function is copy from tdsrrcpcrc_process_pcrc_r4
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcpcrc_process_pcrc_r4
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_PhysicalChannelReconfiguration_r4_IEs* pcrc_ptr = NULL;

  tdsrrc_DL_DCCH_Message  *dcch_msg_ptr = NULL; /* Pointer to the RB Setup message */
  tdsrrc_PhysicalChannelReconfiguration *msg_ptr = NULL;		/* Pointer to the RB Setup message        */

  tdsrrcllc_oc_status_e_type             oc_status;/* To hold tdsordered_config status   */
  tdsrrc_state_e_type                    current_state; /* current RRC State  */
  tdsrrc_int_c_rnti_type                 c_rnti;       /* Local C_RNTI value */
  tdsrrc_int_u_rnti_type                 u_rnti;
  tdsrrcrb_directed_cell_info_type       directed_cell_info; /* directed cell info struct */

  tdsrrcrb_rnti_update_info_type rnti_update_info;   /* to hold rnti update info */

  boolean       ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                    occured while setting tdsordered_config. */

   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  boolean primary_plmn_Id_present = FALSE;
   /*Variable to hold primary plmn i.d if present in OTA msg*/
  tdsrrc_PLMN_Identity primary_plmn_Identity;

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  msg_ptr = &dcch_msg_ptr->message.u.physicalChannelReconfiguration;

  if (FAILURE == 
      tdsrrcpcrc_check_and_send_failure_for_simul_reconfig(
        msg_ptr->u.later_than_r3.rrc_TransactionIdentifier))
  {
    return;
  }

  /* Store Msg Ptr in a local variable */
  pcrc_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.physicalChannelReconfiguration_r4;

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/
  if ((pcrc_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         pcrc_ptr->m.activationTimePresent,
         pcrc_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC SDU Lifetime validation failed");
      tdsrrcpcrc_send_lifetime_validation_failure(TDSRRCPC_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force PCRC Activation time to NOW");
      pcrc_ptr->m.activationTimePresent = 0;
    }
  }

#endif /*FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION*/

  /* Validate the Message to make sure there is no
  invalid or incompatible configuration given by the
  message */
  tdsrrcpcrc_validate_pcrc_r4( pcrc_ptr );

  if( tdsrrc_pcrc.status.failure_status != TDSRRCPC_MSG_VALID )
  {
    /*  Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PCRC Message validation failed");

    /* Send the PC Reconfig Failure Message */
    tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                              tdsrrc_pcrc.status.prot_err_cause
                            );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
  }
  /* If the message is valid, proceed further */
  else
  {

    TDSRRC_MSG0(MSG_LEGACY_LOW,"Processing PCRC Message");

    /* Check if there is CN information present. If so, we need
    to send the information to the Cell Change manager */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr,tdsrrc_PhysicalChannelReconfiguration_r4_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      /*Check to see if primary PLMN I.D is present in PCR message ext
       * physicalChannelReconfiguration-v690ext which is present in 
       * v690NonCriticalExtensions
       */
       primary_plmn_Id_present = tdsrrcpcrc_check_and_get_primary_plmn_id(msg_ptr,
                                                                  &primary_plmn_Identity,TDSMSG_REL4);
      tdsrrc_pcrc.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info(
                        &(pcrc_ptr->cn_InformationInfo)
                        ,primary_plmn_Id_present
                        ,&primary_plmn_Identity
                        );
 
      if (tdsrrc_pcrc.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_pcrc.cn_info_saved = TRUE;
      }
    }
    tdsrrcpcrc_process_high_mobility_ind_in_non_critical_ext(msg_ptr,TDSMSG_REL4);
   
    /* Need to send reconfig IE info. First set TDSORDERED_CONFIG
    so that LLC can process the necessary parameters. */
    oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_PCR,
                                           tdsrrc_pcrc.state_indicated_by_msg,
                                           tdsrrc_DL_DCCH_Message_PDU,
                                           (void *)dcch_msg_ptr );

    switch( oc_status )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"oc_status is %d ",oc_status);
      case TDSORDERED_CONFIG_SET:
        current_state = tdsrrc_get_state(); 
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
        if((tdsrrcsib_is_current_event_sib_read_in_dch()) &&
           (TDSRRC_PROCEDURE_GPS == tdsrrcsib_get_proc_id_for_sib_read_in_dch()))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Force Init GPS ");
          tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
        }


#endif 

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
        /* Populate the local variables to indicate if Re-direction info is present */
        directed_cell_info.freq_info_present = FALSE;
        directed_cell_info.cpid_present = FALSE;
        if((current_state==TDSRRC_STATE_CELL_FACH) && 
                 ((tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
                 ||(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
                 ||(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
           
          if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
              tdsrrc_PhysicalChannelReconfiguration_r4_IEs,frequencyInfo)) &&
             (TDSRRC_CHECK_COMMON_MSG_TYPE(pcrc_ptr->frequencyInfo.
              modeSpecificInfo,rrc_FrequencyInfo_modeSpecificInfo_tdd)))           
          {
            directed_cell_info.freq_info_present = TRUE;
            directed_cell_info.uarfcn_dl = pcrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
          }
          
          if (tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL4))
          {
            directed_cell_info.cpid_present = TRUE;
            directed_cell_info.cell_param_id = pcrc_ptr->dl_InformationPerRL_List.elem[0].
                                      modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          }
          /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection (FACH->FACH or
          FACH -> PCH), treat it as un-supported configuration and send failure.*/ 
          if((tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE) && 
             (tdsrrccsp_check_initiate_cell_selection_handling() == FALSE))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
            if(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH)
            {
              tdsrrcpcrc_send_initiate_cu_flag = TRUE;
              tdsrrcpcrc_delete_crnti          = TRUE;
            }
            else
            {
              tdsrrc_pcrc.directed_cell_info = TRUE;
            }
          }          
        }
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
        if (
            (current_state==tdsrrc_pcrc.state_indicated_by_msg)||
            ((current_state==TDSRRC_STATE_CELL_FACH)&& 
             (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_DCH))
           )
        {
          /*  CellDCH<->CellDCH or CellFACH<->CellFACH or CellFACH->CellDCH 
          transition. Ordered Config has been set. We can setup the
          Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");

          /*check whether any frequency info is present in the message
          and transition is CELL_FACH->CELL_FACH. In this case
          initiate a cell update after channel config is successfully completed */
          if ((current_state==TDSRRC_STATE_CELL_FACH)&& 
             (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
          {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
     
            /*call the function that evaluates the directed cell info and set the flags accordingly */
            if ((tdsrrcpcrc_send_initiate_cu_flag == FALSE)&&
                (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
            {
    
              if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r4_IEs,new_C_RNTI))
              {
                tdsrrc_pcrc.new_crnti_valid = TRUE;
                tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                                    &c_rnti);
                tdsrrc_pcrc.new_crnti = (uint16)c_rnti;
              }
              else
              {
                tdsrrc_pcrc.new_crnti_valid = FALSE;
              }
              /* Now copy OC to TOC */
              tdsrrcllcoc_update_toc_with_oc_from_fach();
              /*Initiate the cell selection*/
              tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR,
                                                       TDSRRC_TRANSITION_FROM_FACH,
                                                       tdsrrc_pcrc.state_indicated_by_msg);
    
        
              tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
     
              /* exit from the switch case */
              break;
            }
    
 #else /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
            /* 1102 */
            if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
                tdsrrc_PhysicalChannelReconfiguration_r4_IEs,frequencyInfo)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(pcrc_ptr->frequencyInfo.
                modeSpecificInfo,rrc_FrequencyInfo_modeSpecificInfo_tdd)))           
            {
              directed_cell_info.freq_info_present = TRUE;
              directed_cell_info.uarfcn_dl = pcrc_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
            }
            else
            {
              directed_cell_info.freq_info_present = FALSE;
            }
            if (tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL4))
            {
              directed_cell_info.cpid_present = TRUE;
              directed_cell_info.cell_param_id = pcrc_ptr->dl_InformationPerRL_Listelem[0].
                                        modeSpecificInfo.u.tdd->u.tdd->cellParametersI;
            }
            else
            {
              directed_cell_info.cpid_present = FALSE;
            }
            /*call the function that evaluates the directed cell info and set the flags accordingly */
            if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
            {
              /*directed cell info matched. Cell update is not required */
              tdsrrcpcrc_send_initiate_cu_flag = FALSE;
            }
            else
            {
              /*directed cell info do not match. Cell update is required */
              tdsrrcpcrc_send_initiate_cu_flag = TRUE;
              tdsrrcpcrc_delete_crnti          = TRUE;
            }
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
          } /* if CELL_FACH->CELL_FACH reconfiguration */

          /* Call the function that creates and sends a channel
          config request to LLC. */
          tdsrrcpcrc_send_channel_config_req( );
             /* Change the substate since we're waiting for the
             confirmation from LLC for the channel configs */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_FACH))
        {
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, tdsrrc_PhysicalChannelReconfiguration_r4_IEs,new_C_RNTI))
          {
            tdsrrc_pcrc.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                                &c_rnti);
            tdsrrc_pcrc.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_pcrc.new_crnti_valid = FALSE;
          }


          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req( TDSRRC_PROCEDURE_PCR,
                                                  TDSRRC_TRANSITION_FROM_DCH,
                                                  tdsrrc_pcrc.state_indicated_by_msg);
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

          /* Start T_305 [periodic cell update] timer */
          tdsrrccu_start_t_305_timer();

        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH))
        {
          tdsrrc_pcrc.directed_cell_info = tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL4);
         /*Send the PCRC complete msg on old configuration */
         tdsrrcpcrc_send_pcrc_complete_message(TRUE);
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
         /*wait for L2ack */
         tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
         /* prints rate for each transport channel */
         tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_DCH) && 
                (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH))
        {
          /*Send the PCRC complete msg on old configuration */
          tdsrrcpcrc_send_pcrc_complete_message(TRUE);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
          /*wait for L2ack */
          tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
          /* prints rate for each transport channel */
          tdsrrcllc_print_trans_chnl_rate();
        }
        else if((current_state==TDSRRC_STATE_CELL_FACH) && 
                ((tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH) ||
                 (tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_URA_PCH)))
        {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
          /*call the function that evaluates the directed cell info and set the flags accordingly */
          if ((tdsrrc_pcrc.directed_cell_info == FALSE) &&
            (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE))
          {
            if(tdsrrc_pcrc.state_indicated_by_msg==TDSRRC_STATE_CELL_PCH)
            {
              tdsrrc_pcrc.directed_cell_info = tdsrrcpcrc_cpid_is_pccpch_info_present(msg_ptr, TDSMSG_REL4);
            }
          
            tdsrrc_pcrc.initiate_cell_selection_trans_from_fach = TRUE;
          }
          
 #endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

          /* check whether new C-RNTI was present in the message. In that case
          we have to configure MAC with new C-RNTI before sending the response 
          message */
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr, 
             tdsrrc_PhysicalChannelReconfiguration_r4_IEs,new_C_RNTI))
          {
            tdsrrc_translate_crnti(&pcrc_ptr->new_C_RNTI, 
                                &c_rnti);
            /* Send a RNTI Update Request */
            rnti_update_info.crnti_action    = TDSRRC_RNTI_UPDATE;
            rnti_update_info.crnti           = (uint16)c_rnti;
            /* check whether URNTI is also present in the message */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pcrc_ptr,
             tdsrrc_PhysicalChannelReconfiguration_r4_IEs,new_U_RNTI))
            {
              tdsrrc_translate_urnti(&pcrc_ptr->new_U_RNTI, 
                                  &u_rnti);
              rnti_update_info.urnti_action = TDSRRC_RNTI_UPDATE; 
              rnti_update_info.urnti        = u_rnti; 
            }
            else
            {
              rnti_update_info.urnti_action = TDSRRC_RNTI_NOCHANGE; 
            }
            rnti_update_info.procedure            = TDSRRC_PROCEDURE_PCR;
            rnti_update_info.rlc_re_establish_srb = FALSE;
            rnti_update_info.rlc_re_establish_rab = FALSE;
            rnti_update_info.cnf_reqd             = TRUE;

            tdsrrcrb_send_rnti_update_req(rnti_update_info);
            tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF;
          }
          else 
          {
            /* There is no new C-RNTI So send the response on the old configuration
            */
            tdsrrcpcrc_send_pcrc_complete_message(TRUE);
            /*wait for L2ack */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for PCRC Complete msg");
            tdsrrc_pcrc.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
            /* prints rate for each transport channel */
            tdsrrcllc_print_trans_chnl_rate();
          }
        }
        break;

      case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
        tdsrrc_pcrc.status.failure_status = TDSRRCPC_SIMULTANEOUS_RECONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;

        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
        tdsrrc_pcrc.status.failure_status =TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
        tdsrrc_pcrc.status.failure_status =TDSRRCPC_MSG_INVALID_CONFIG;
        /* Send the PC Reconfig Failure Message */
        tdsrrcpcrc_send_pcrc_failure_message( tdsrrc_pcrc.status.failure_status,
                                  tdsrrc_pcrc.status.prot_err_cause );

        /* Go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
		
      case TDSORDERED_CONFIG_NOT_SET_OTHER:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
        /* When LLC sends this, there is a serious problem
        and we'll go back to disconnected state. No need
        to send the failure message here, we can just
        go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;

      default:

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
        /* Unknown error, just go back to initial substate */
        tdsrrc_pcrc.curr_substate = TDSRRCRB_INITIAL;
        
        /* Indicate that an error occured */
        ordered_config_err_occured = TRUE;
        break;
    }/* end switch */

    if( ordered_config_err_occured == TRUE )
    {
      /* No need to continue. Clear the procedure
      variables and return. */
      tdsrrcpcrc_clear_procedure();
      return;
    }

    if(tdsrrc_pcrc.pcrc_for_srns_relocation == TRUE)
    {
      tdsrrcsmc_commit_fresh_for_srns_relocation();
    }
  }
}
/*===========================================================================

FUNCTION   VALIDATE_PCRC_R4

DESCRIPTION

  This function validates the received PC Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a PCRC Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.

  The function is copy from copy from tdsrrcpcrc_validate_pcrc_r5,delete HSDPA handle
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcpcrc_validate_pcrc_r4
(
  tdsrrc_PhysicalChannelReconfiguration_r4_IEs *msg_ptr /* Pointer to the PCRC message */
)
{
 
  /* Assume the message is valid for now */
  tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_VALID;

  /* Check the RRC State indicatior given by the message */
  switch( msg_ptr->rrc_StateIndicator )
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_FACH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_PCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_CELL_PCH;
      break;

    case tdsrrc_RRC_StateIndicator_ura_PCH:
      tdsrrc_pcrc.state_indicated_by_msg = TDSRRC_STATE_URA_PCH;
      break;


    default:
      /* No other states are supported right now. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported state in PC Reconfig Message");
      tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
      break;
  }

  if( tdsrrc_pcrc.status.failure_status == TDSRRCPC_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
     return;
  }

 
   /* Check if RB with PDCP Information list is present. This
  is not supported in the first release. This may be supported
  later */                                    

#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
        tdsrrc_PhysicalChannelReconfiguration_r4_IEs,dl_CounterSynchronisationInfo)) 
#else
  if(tdsrrcpcrc_validate_srns_relocation_info_new(msg_ptr, TDSMSG_REL4))
#endif
  {

    /*check whether the SRNS relocation is within Cell_DCH or not.
     SRNS relocation information can come in reconfiguration messages
     only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && 
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"PCRC : SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_pcrc.state_indicated_by_msg);
      tdsrrc_pcrc.status.failure_status=TDSRRCPC_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has 
    PDCP info list. If it is true then this configuration is unsupported */
    /* Copy handle from R3 begin */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->dl_CounterSynchronisationInfo,
        rB_WithPDCP_InfoList))
    /* Copy handle from R3 end */
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
      tdsrrc_pcrc.status.failure_status=TDSRRCPC_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation. 
        Set the flag that indicates that this procedure
      involves SRNS relocation */
      tdsrrc_pcrc.pcrc_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, tdsrrc_PhysicalChannelReconfiguration_r4_IEs,new_U_RNTI))
      {
        tdsrrc_pcrc.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(msg_ptr->new_U_RNTI),
                              &tdsrrc_pcrc.new_urnti);
      }
    }
  }

    /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  tdsrrc_pcrc.trans_to_dch_ciph_config_needed = FALSE;
  if((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_DCH) && ( tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr,
        tdsrrc_PhysicalChannelReconfiguration_r4_IEs,dl_CommonInformation)) && 
       (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->dl_CommonInformation,
        tdsrrc_DL_CommonInformation_r4,dl_DPCH_InfoCommon)))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");

        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->dl_CommonInformation.dl_DPCH_InfoCommon,
             tdsrrc_DL_DPCH_InfoCommon_r4,mac_d_HFN_initial_value)),
             &msg_ptr->dl_CommonInformation.dl_DPCH_InfoCommon.mac_d_HFN_initial_value,
             &mac_d_hfn_l);

        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed ");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdsrrcsmc_process_cipher_config_for_hho failed");
        }
        tdsrrc_pcrc.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }

  /*Cell_PCH/URA_PCH specific validations */
  if ((tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH))
  {
     /* If UTRAN DRX coefficient is not present then it is invalid configuration */
     if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, 
         tdsrrc_PhysicalChannelReconfiguration_r4_IEs,utran_DRX_CycleLengthCoeff)))
     {
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_pcrc.state_indicated_by_msg);
       tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_INVALID_CONFIG;
       return;
     }
     if (tdsrrc_pcrc.state_indicated_by_msg == TDSRRC_STATE_URA_PCH)
     {
       /*should we do it after getting l2ack for response??*/
       if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr, tdsrrc_PhysicalChannelReconfiguration_r4_IEs,ura_Identity)))
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
         tdsrrccu_clear_ura_id ();
       }
       else
       {
         /*URA ID present in the message set ura_identity variable with this value */
         if (msg_ptr->ura_Identity.numbits == 16)
         {
           tdsrrccu_update_ura_id (&(msg_ptr->ura_Identity));
         }
         else
         {
           TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",msg_ptr->ura_Identity.numbits);
           tdsrrc_pcrc.status.failure_status = TDSRRCPC_MSG_PROTOCOL_ERROR;
           tdsrrc_pcrc.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
           return;
         }

       }
           
     }
  }
}/* end function tdsrrcpcrc_validate_pcrc_r4 */

#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
/*===========================================================================

FUNCTION   TDSRRCCU_VALIDATE_SRNS_RELOCATION_INFO_NEW()

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
boolean tdsrrcpcrc_validate_srns_relocation_info_new
(
  void *msg_ptr,                              /* Pointer to Downlink Msg */
  tdsrrc_msg_ver_enum_type msg_version        /* Downlink message version */
)
{

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Validate if PCRC is for srns relocation");
  if (TRUE == tdsrrc_is_fresh_in_reconfiguration_message())
  {
    return TRUE;
  }

  if (msg_version == TDSMSG_REL5)
  {
    tdsrrc_PhysicalChannelReconfiguration_r5_IEs *msg_ptr_r5 = (tdsrrc_PhysicalChannelReconfiguration_r5_IEs *)msg_ptr;
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr_r5, 
        tdsrrc_PhysicalChannelReconfiguration_r5_IEs,dl_CounterSynchronisationInfo)) 
    {
      return TRUE;
    }
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(msg_ptr_r5, new_U_RNTI))
    { /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr_r5->new_U_RNTI)) == FALSE)
      {
        return TRUE;
      }
    }
    else
    {
      return FALSE;
    }
  }
  else if (msg_version == TDSMSG_REL6)
  {
    tdsrrc_PhysicalChannelReconfiguration_r6_IEs *msg_ptr_r6 = (tdsrrc_PhysicalChannelReconfiguration_r6_IEs *)msg_ptr;
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr_r6, 
        tdsrrc_PhysicalChannelReconfiguration_r6_IEs,dl_CounterSynchronisationInfo)) 
    {
      return TRUE;
    }
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(msg_ptr_r6, new_U_RNTI))
    { /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr_r6->new_U_RNTI)) == FALSE)
      {
        return TRUE;
      }
    }
    else
    {
      return FALSE;
    }
  }
  
  else if (msg_version == TDSMSG_REL7)
  {
    tdsrrc_PhysicalChannelReconfiguration_r7_IEs *msg_ptr_r7 = (tdsrrc_PhysicalChannelReconfiguration_r7_IEs *)msg_ptr;
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr_r7, 
        tdsrrc_PhysicalChannelReconfiguration_r7_IEs,dl_CounterSynchronisationInfo)) 
    {
      return TRUE;
    }
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(msg_ptr_r7, new_U_RNTI))
    { /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr_r7->new_U_RNTI)) == FALSE)
      {
        return TRUE;
      }
    }
    else
    {
      return FALSE;
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
  else if (msg_version == TDSMSG_REL8)
  {
    tdsrrc_PhysicalChannelReconfiguration_r8_IEs *msg_ptr_r8 = (tdsrrc_PhysicalChannelReconfiguration_r8_IEs *)msg_ptr;
    if(TDSRRCRB_R8_MSG_IE_PRESENT(msg_ptr_r8,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,
                       dl_CounterSynchronisationInfo))
    {
      return TRUE;
    }
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(msg_ptr_r8, new_U_RNTI))
    { /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr_r8->new_U_RNTI)) == FALSE)
      {
        return TRUE;
      }
    }
    else
    {
      return FALSE;
    }
  }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
    else if (msg_version == TDSMSG_REL9)
    {
     tdsrrc_PhysicalChannelReconfiguration_r9_IEs *msg_ptr_r9 = (tdsrrc_PhysicalChannelReconfiguration_r9_IEs *)msg_ptr;
     if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr_r9, 
            tdsrrc_PhysicalChannelReconfiguration_r9_IEs,dl_CounterSynchronisationInfo)) 
     {
       return TRUE;
     }
     else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(msg_ptr_r9, new_U_RNTI))
     { /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr_r9->new_U_RNTI)) == FALSE)
      {
        return TRUE;
      }
     }
     else
     {
      return FALSE;
     }
    }
#endif /*FEATURE_TDSCDMA_REL9*/

/*rest is for version == 4*/
    else if (msg_version == TDSMSG_REL4)
    {
      tdsrrc_PhysicalChannelReconfiguration_r4_IEs *msg_ptr_r4 = (tdsrrc_PhysicalChannelReconfiguration_r4_IEs *)msg_ptr;
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg_ptr_r4, 
           tdsrrc_PhysicalChannelReconfiguration_r4_IEs,dl_CounterSynchronisationInfo)) 
      {
        return TRUE;
      }
      else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(msg_ptr_r4, new_U_RNTI))
      { /*check if SRNC-ID changed*/
        if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr_r4->new_U_RNTI)) == FALSE)
        {
          return TRUE;
        }
      }
      else
      {
       return FALSE;
      }
    }
  return FALSE; 

}

#endif
