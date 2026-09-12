/*===========================================================================
  R R C   R A D I O   B E A R E R   E S T A B L I S H M E N T   M O D U L E 

DESCRIPTION

  This module performs the Radio Bearer Establishment Procedure. This
  module processes the Radio Bearer Setup Message and sets up the 
  radio bearers given by the message. 
  
  The Radio Bearer Setup Message may setup both signalling radio bearers
  and radio bearers that are mapped to radio access bearers. 
  It may set also re-configure previously established radio bearers.
  The procedure may also initiate a hard-handover if the Radio Bearer
  Setup Message indicates a frequency change or a frame offset change.
  
  
EXTERNALIZED FUNCTIONS

  tdsrrcrbe_procedure_event_handler   Event handler for this procedure
  tdsrrcrbe_init_procedure            Power-up initialization of the procedure

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Procedure must be initialized at power-up by calling tdsrrcrbe_init_procedure. 
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrbe.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/14/12   as      Added code to support feature NB/WB handover support
06/16/11   bj      Added code to handle receive R9 RadioBearSetup Message.
06/10/11   ttl     Change client id to MVS_CLIENT_TDSCDMA for ghdi_mvs_acquire()
03/10/11   bj      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/18/10   ysh     Frequency info save. 3GPP-8.6.6.1(3.4.2)
                   Save Primary freq when enter Non-CELL_DCH state.
11/11/10   yzh     (3GPP: 8.6.6.27)Del tdsrrcllcpcie_initialise_sync_a_post_veri_info()
                   when IE Downlink information common for all radio links(FDD->TDD).
11/10/10   bj      Deleted IE "RNC support for change of UE capability".(3GPP: 8.6.2.3)
10/30/10   ysh     IE"Downlink information for each radio link" info save.  3GPP-8.6.6.4(3.2.13).
10/29/10   zwj     Added code to handle receive R4 RadioBearSetup Message.

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

#include "tdsrrccmd_v.h"
#include "tdsrrcccm.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrccu.h"
#include "tdsrrccui.h"
#include "tdsrrcintif.h"
#include "tdsrrclcm.h"
#include "tdsrrcllc.h"
#include "tdsrrcmm_v.h"
#include "rrcmnif.h"
#include "tdsrrcrbcommon.h"
#include "tdsrrcrbe.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcsend.h"
#include "tdsrrclbt.h"
#include "tdsrrccspi.h"
#include "tdsrrccsp.h"
#if(defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) || \
  defined(FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING))
#include "tdsrrcsibproc.h"
#endif
#include "tdsrrcsmc.h"
#include "tdsrrclog.h"
#include "tdsrrcasn1util.h"

#include "tdsseq.h"
#include "tdsrrcsmc.h"

#include "tdsrrcllcoc.h"
#include "tdsrrctmr.h"

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

/* AMR 12.2 kbps speech */
#define TDSAMR_MODE_122_CLASS_A_BIT_SIZE  81
#define TDSAMR_MODE_122_CLASS_B_BIT_SIZE 103
#define TDSAMR_MODE_122_CLASS_C_BIT_SIZE  60

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
#define TDSRRCRBE_TEMP_TIMER_SIG 0x4000
rex_sigs_type tdsrrcrbe_sig;
rex_timer_type tdsrrcrbe_temp_timer;
  

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
#ifndef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
/* Structure to store all internal data related to RBE procedure           */
typedef struct
{
  tdsrrcrb_substate_e_type curr_substate;  /* Store the current substate     */
  tdsrrcrb_transaction_id_type tr_ids;      /* Transaction ids for RBS msg    */
  tdsrrc_state_e_type state_from_msg;       /* RRC State given by RBS msg     */
  tdsrrcrb_msg_failure_cause_type status;   /* Status of message validation   */
  tdsrrcrb_rab_info_per_msg_type  rabs;      /* Hold info about each rab in one
                                         RBS message till they're processed*/
  boolean                      new_crnti_valid;           /* Indicates if a valid C-RNTI was
                                                            recd in the message transitioning
                                                            from CELL_DCH to CELL_FACH       */
  uint16                       new_crnti;
  boolean                      directed_cell_info; /* Indicates if a valid C-RNTI was recd */
  tdsrrc_int_u_rnti_type          new_urnti;
  boolean                      new_urnti_valid;          /* Indicates if a valid U-RNTI was recd */
  boolean                      rbe_for_srns_relocation;  /* Indicates SRNS reloc involved with
                                                            this msg                             */
  boolean                       cn_info_saved;           /*flag to indicate that CN info is saved */
  mm_cmd_type *                 mm_cmd_ptr;              /*pointer to hold mm_cmd with cn info */
  boolean                       hho_ciph_status;
  boolean                       trans_to_dch_ciph_config_needed;


  boolean first_tm_rb_setup;
}tdsrrcrbe_int_data_type;
#endif/*FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE*/
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrcrbe_int_data_type tdsrrc_rbe;

tdsrrc_cmd_type tdsrrc_cmd;

boolean tdsrrcrbe_send_initiate_cu_flag = FALSE;
boolean tdsrrcrbe_delete_crnti          = FALSE;

uint8 tdsciphering_activation_cfn;

tdsmac_rab_status_e_type tdsmac_rab_status;
#ifdef FEATURE_UMTS_PDCP
extern boolean tdsrrc_pdcp_enabled;
#endif /* FEATURE_UMTS_PDCP*/

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */
/*===========================================================================

FUNCTION   RRCRBE_INDICATE_LAYER1_IF_RB_SETUP_CONFIGURE_VOICE_RAB

DESCRIPTION

  If radio bearer setup message indicates UE to setup voice RAB then this
  function will indicate layer1 about the same by calling some feature specific
  call back function provided by layer1.
  If received rbsetup message is not for setting up voice call then this function
  will not do anything.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_indicate_layer1_if_rb_setup_configure_voice_rab(void)
{
  uint32 rab_index;

  for (rab_index=0; rab_index<tdsrrc_rbe.rabs.num_rabs_in_msg; rab_index++)
  {
    if (TDSRRCRB_CS_VOICE_RAB == tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type)
    {
      tdsrrc_indicate_layer1_next_config_is_for_voice();
    }
  }
}
/*===========================================================================

FUNCTION   RRCRBE_IS_SRNS_IN_PROGRESS()

DESCRIPTION
 This function returns if SRNS is in progress with current reconfig procedure



DEPENDENCIES

  None.

RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrbe_is_srns_in_progress
(
  void
)
{
  return tdsrrc_rbe.rbe_for_srns_relocation;
}

/*===========================================================================

FUNCTION   RRCRBE_GET_NEW_CRNTI_VALID()

DESCRIPTION
 This function returns if reconfig msg has valid CRNTI


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrbe_get_new_crnti_valid
(
  void
)
{
  return tdsrrc_rbe.new_crnti_valid;
}
/*===========================================================================

FUNCTION   RRCRBE_GET_CURRENT_SUB_STATE()

DESCRIPTION
 This function is used to return current substate



DEPENDENCIES

  None.

RETURN VALUE

  tdsrrcrb_substate_e_type current_substate.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_substate_e_type tdsrrcrbe_get_current_sub_state( )
{
  return tdsrrc_rbe.curr_substate;
}

/*===========================================================================

FUNCTION tdsrrcrbe_is_pccpch_info_present

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
static boolean  tdsrrcrbe_is_pccpch_info_present
(
  tdsrrc_RadioBearerSetup *msg_ptr,
  tdsrrc_msg_ver_enum_type msg_version
)
{
  /*this function is used to find whether directed cell info is present or not during Cell_DCH to Cell_FACH 
   * state transition. It is safe to assume that the rl list will have only one element.
   */

  if (TDSMSG_REL5 == msg_version)
  {
    tdsrrc_RadioBearerSetup_r5_IEs *rbe_ptr_5 = 
        &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.radioBearerSetup_r5;

    if (rbe_ptr_5->m.dl_InformationPerRL_ListPresent)
    {
      if(rbe_ptr_5->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
           T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_tdd)
      { 
        if (rbe_ptr_5->dl_InformationPerRL_List.
            elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBE message %d",
                 rbe_ptr_5->dl_InformationPerRL_List.
                 elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }
  else
  if (TDSMSG_REL6 == msg_version)
  {
    tdsrrc_RadioBearerSetup_r6_IEs *rbe_ptr_6 = 
       &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.r6.radioBearerSetup_r6;

    if (rbe_ptr_6->m.dl_InformationPerRL_ListPresent)
    {
      if(rbe_ptr_6->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
           T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_tdd)
      { 
        if (rbe_ptr_6->dl_InformationPerRL_List.
             elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBE message %d",
                rbe_ptr_6->dl_InformationPerRL_List.
                elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }
  else
  if (TDSMSG_REL7 == msg_version)
  {
    tdsrrc_RadioBearerSetup_r7_IEs *rbe_ptr_7 = 
         &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.r7.radioBearerSetup_r7;

    if (rbe_ptr_7->m.dl_InformationPerRL_ListPresent)
    {
      if(rbe_ptr_7->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
       T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_tdd)
      { 
        if (rbe_ptr_7->dl_InformationPerRL_List.
            elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBE message %d",
                rbe_ptr_7->dl_InformationPerRL_List.
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
    tdsrrc_RadioBearerSetup_r8_IEs *rbe_ptr_8 = &msg_ptr->u.later_than_r3.criticalExtensions.u.
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerSetup_r8;

    if (rbe_ptr_8->m.dl_InformationPerRL_ListPresent)
    {
      if(rbe_ptr_8->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
        T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd)
      { 
        if (rbe_ptr_8->dl_InformationPerRL_List.
              elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBE message %d",
                      rbe_ptr_8->dl_InformationPerRL_List.elem[0].modeSpecificInfo
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
      tdsrrc_RadioBearerSetup_r9_IEs *rbe_ptr_9 = &msg_ptr->u.later_than_r3.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.radioBearerSetup_r9;
  
      if (rbe_ptr_9->m.dl_InformationPerRL_ListPresent)
      {
        if(rbe_ptr_9->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t ==
          T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd)
        { 
          if (rbe_ptr_9->dl_InformationPerRL_List.
                elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBE message %d",
                        rbe_ptr_9->dl_InformationPerRL_List.elem[0].modeSpecificInfo
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
    tdsrrc_RadioBearerSetup_r4_IEs *rbe_ptr_4 = 
          &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerSetup_r4;

    if (rbe_ptr_4->m.dl_InformationPerRL_ListPresent)
    {
      if (rbe_ptr_4->dl_InformationPerRL_List.elem[0].modeSpecificInfo.t == 
        T_tdsrrc_DL_InformationPerRL_r4_modeSpecificInfo_tdd)
      { 
        if (rbe_ptr_4->dl_InformationPerRL_List.
              elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell Parameter ID in RBE message %d",
                   rbe_ptr_4->dl_InformationPerRL_List.
                   elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID,0,0);
          return TRUE;
        }
      }
    }
  }

  return FALSE;

}  /* tdsrrcrbe_is_pccpch_info_present */
/*===========================================================================

FUNCTION   SEND_RBS_FAILURE_MSG

DESCRIPTION

  This function sends the Radio Bearer Setup Failure message
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
static void tdsrrcrbe_send_rbs_failure_message
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
  ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc(sizeof(tdsrrc_UL_DCCH_Message));
  if (ptr == NULL)
  {
    ERR_FATAL("No memory for RBS Failure msg!",0,0,0);
  }

  /* Reset SVTHHO in case of failure */
  tdsrrcsmc_reset_start_value_to_transmit_cs_hho(FALSE);
 ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_radioBearerSetupFailure; /* RBS Failure msg */

  /* Set the bit-mask - neither rrc_potentiallySuccesfulBearerList_present
  nor rrc_RadioBearerReconfigurationFailure_nonCriticalExtensions_present
  are included */
  memset(&ptr->message.u.radioBearerSetupFailure.m,0,
          sizeof(ptr->message.u.radioBearerSetupFailure.m));

  /* Transaction Id - First check the rejected_transacion id. If
  it's not invalid, this message is being rejected because there
  is already another message accepted but not processed.
  If the rejected_transaction id is invalid, we are rejecting the
  accepted message, so use the accepted transaction id.

  NOTE: Don't change the order, it's important to check rejected
  transaction first. */
  if ( tdsrrc_rbe.tr_ids.rejected_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier =
    tdsrrc_rbe.tr_ids.rejected_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_rbe.tr_ids.rejected_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  }
  else if ( tdsrrc_rbe.tr_ids.accepted_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier =
    tdsrrc_rbe.tr_ids.accepted_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_rbe.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  }
  else
  {
    /* Don't know the transaction id for this message.( one reason
    for this could be that the message had a critical
    extenstion). Since it's unknown, we should set it to zero. */
    ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Trid for RBS Failure msg!");
  }
  if (ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier > 3)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid TrId:%d from RBS db,assuming 0",ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier);
    ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier = 0;
  }

  switch ( failure_cause )
  {
  case TDSRRCRB_MSG_UNSUPPORTED_CONFIG:

    ptr->message.u.radioBearerSetupFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
    break;

  case TDSRRCRB_PHY_CHAN_FAILURE:

    ptr->message.u.radioBearerSetupFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_physicalChannelFailure;
    break;

  case TDSRRCRB_SIMULTANEOUS_RECONFIG:

    ptr->message.u.radioBearerSetupFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_incompatibleSimultaneousReconfiguration;
    break;

  case TDSRRCRB_MSG_PROTOCOL_ERROR:

    ptr->message.u.radioBearerSetupFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_protocolError;

    /* When there is a Protocol Error, we also have to indicate
    the protocol error cause */
    ptr->message.u.radioBearerSetupFailure.failureCause.u.protocolError=
        rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation);
    
    if(ptr->message.u.radioBearerSetupFailure.failureCause.u.protocolError == NULL)
    {
      ERR_FATAL("Memory allocation failure",0,0,0);
    }
    
    ptr->message.u.radioBearerSetupFailure.failureCause.u.protocolError->diagnosticsType.t  =
    T_tdsrrc_ProtocolErrorInformation_diagnosticsType_type1;

    ptr->message.u.radioBearerSetupFailure.failureCause.u.protocolError->
    diagnosticsType.u.type1 = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation_type1);

    if(ptr->message.u.radioBearerSetupFailure.failureCause.u.protocolError->
        diagnosticsType.u.type1 == NULL)
    {
      ERR_FATAL("Memory allocation failure",0,0,0);
    }

    /* Copy the given protocol error cause */
    ptr->message.u.radioBearerSetupFailure.failureCause.u.protocolError->
    diagnosticsType.u.type1->protocolErrorCause = prot_err_cause;
    /*log the protocol error*/
    tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                            (uint8)TDSRRC_PROCEDURE_RBE,
                            (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                            tdsrrc_log_map_protocol_cause(prot_err_cause)
                            );
    break;

  case TDSRRCRB_MSG_INVALID_CONFIG:

    ptr->message.u.radioBearerSetupFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
    break;

  case TDSRRCRB_MSG_INCOMPLETE_CONFIG:

    ptr->message.u.radioBearerSetupFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
    break;

  case TDSRRCRB_CELL_UPDATE_OCCURED:

    ptr->message.u.radioBearerSetupFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_cellUpdateOccurred;
    break;

  default:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RBE Unknown Failure message");
    ptr->message.u.radioBearerSetupFailure.failureCause.t =
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
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending RBS Failure msg to send-chain Cause:%d",ptr->message.u.radioBearerSetupFailure.failureCause.t);
    /* Need to wait for L2 ack for all states because RRC needs to trigger include reconfig status indicator
    if RLC acknowledgement is not yet received.
    As per 8.3.1.3
    1> if the IE "Cell update cause" is set to "radio link failure" and the UE has submitted a reconfiguration response
    message to RLC and the UE has not received the RLC acknowledgement for the response message:
    2> include the IE "Reconfiguration Status Indicator" and set it to TRUE.*/
     if(tdsrrc_rbe.status.failure_status == TDSRRCRB_PHY_CHAN_FAILURE)
    {
      tdsrrc_update_reconfig_waiting_for_l2ack_dch(TDSRRC_PROCEDURE_RBE);
      status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBE,  /* Procedure is RBE    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_REQUIRED  /* ACK is required */
                                  );
    }
    else
    {
    status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBE,  /* Procedure is RBE    */
                                 (void *)ptr,        /* Message pointer     */
                                 ul_lc_id,           /* RLC id              */
                                 TDSRRCSEND_L2ACK_NOT_REQUIRED  /* No need for ACK */
                                );
   }
    if ( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Send chain failed for RBS failure msg: error cause %d",status);
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
}/* end function tdsrrcrbe_send_rbs_failure_message */

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
/*===========================================================================

FUNCTION   tdsrrcrbe_send_lifetime_validation_failure

DESCRIPTION

  This function sends the Radio Bearer Setup Failure message
  on Uplink DCCH. The failure cause must be given to this
  function. 


DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_send_lifetime_validation_failure
(
tdsrrcrb_msg_status_e_type failure_cause
)
{

  tdsrrc_UL_DCCH_Message *ptr;     /* Pointer to uplink DCCH message type */
  rlc_lc_id_type ul_lc_id; /* Logical Channel Id for the message  */
  tdsrrcsend_status_e_type status; /* Status from send chain module       */

  /* Allocate memory for this message. Note that this buffer is released
  by the SEND CHAIN */
  ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc(sizeof(tdsrrc_UL_DCCH_Message));
  if (ptr == NULL)
  {
    ERR_FATAL("No memory for RBS Failure msg!",0,0,0);
  }

  
 ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_radioBearerSetupFailure; /* RBS Failure msg */

  /* Set the bit-mask - neither rrc_potentiallySuccesfulBearerList_present
  nor rrc_RadioBearerReconfigurationFailure_nonCriticalExtensions_present
  are included */
  memset(&ptr->message.u.radioBearerSetupFailure.m,0,
          sizeof(ptr->message.u.radioBearerSetupFailure.m));

 
  if ( tdsrrc_rbe.tr_ids.rejected_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier =
    tdsrrc_rbe.tr_ids.rejected_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_rbe.tr_ids.rejected_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  }
  else if ( tdsrrc_rbe.tr_ids.accepted_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {
    ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier =
    tdsrrc_rbe.tr_ids.accepted_transaction;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */
    tdsrrc_rbe.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  }
  else
  {
    /* Don't know the transaction id for this message.( one reason
    for this could be that the message had a critical
    extenstion). Since it's unknown, we should set it to zero. */
    ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Trid for RBS Failure msg!");
  }
  if (ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier > 3)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid TrId:%d from RBS db,assuming 0",ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier);
    ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier = 0;
  }

  switch ( failure_cause )
  {
  case TDSRRCRB_MSG_UNSUPPORTED_CONFIG:

    ptr->message.u.radioBearerSetupFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
    break;

  case TDSRRCRB_PHY_CHAN_FAILURE:

    ptr->message.u.radioBearerSetupFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_physicalChannelFailure;
    break;

  case TDSRRCRB_SIMULTANEOUS_RECONFIG:

    ptr->message.u.radioBearerSetupFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_incompatibleSimultaneousReconfiguration;
    break;

  case TDSRRCRB_MSG_INVALID_CONFIG:

    ptr->message.u.radioBearerSetupFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
    break;

  case TDSRRCRB_MSG_INCOMPLETE_CONFIG:

    ptr->message.u.radioBearerSetupFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;
    break;

  case TDSRRCRB_CELL_UPDATE_OCCURED:

    ptr->message.u.radioBearerSetupFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_cellUpdateOccurred;
    break;

  default:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RBE Unknown Failure message");
    ptr->message.u.radioBearerSetupFailure.failureCause.t =
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
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending RBS Failure msg to send-chain Cause:%d",ptr->message.u.radioBearerSetupFailure.failureCause.t);

    status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBE,  /* Procedure is RBE    */
                                 (void *)ptr,        /* Message pointer     */
                                 ul_lc_id,           /* RLC id              */
                                 TDSRRCSEND_L2ACK_NOT_REQUIRED  /* No need for ACK */
                                );

    if ( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Send chain failed for RBS failure msg: error cause %d",status);
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
}/* end function tdsrrcrbe_send_lifetime_validation_failure */

#endif /*FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION*/

/*===========================================================================

FUNCTION   tdsrrcrbe_append_start_list_in_ul_counter_sync_info

DESCRIPTION

  This function appends start list in ul counter sync info in rbs-complete msg.
  This takes care of the start value of a given domain which will be included
  in rbs-complete msg and hence it doesnt re-compute the start value for that
  domain.

DEPENDENCIES

  None.

RETURN VALUE

  SUCCESS or FAILURE

SIDE EFFECTS

  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrcrbe_append_start_list_in_ul_counter_sync_info
(
  tdsrrc_UL_DCCH_Message *ptr,
  byte start_value[TDSRRC_MAX_START_LENGTH], /* start_value should always have a valid value */
  tdsrrcsmc_append_start_e_type append_start_type
)
{
  /* Local ptr for start list traversal */
  tdsrrc_STARTList *start_list_ptr;

  if (TDSRRC_MSG_COMMON_BITMASK_IE(ptr->message.u.radioBearerSetupComplete,
    tdsrrc_RadioBearerSetupComplete,ul_CounterSynchronisationInfo))
  {
    /* ul-counter sync info already updated */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"ul-counter-sync-info already updated - ignore this call");
    return(SUCCESS);
  }
   TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->message.u.radioBearerSetupComplete,
    tdsrrc_RadioBearerSetupComplete,ul_CounterSynchronisationInfo); 

  memset(&ptr->message.u.radioBearerSetupComplete.ul_CounterSynchronisationInfo.m,0,
           sizeof(ptr->message.u.radioBearerSetupComplete.ul_CounterSynchronisationInfo.m));
 
            
 
  
 

  /* Assign local ptr to Start List */
  start_list_ptr = &ptr->message.u.radioBearerSetupComplete.ul_CounterSynchronisationInfo.startList;

  return tdsrrcsmc_append_start_list_with_already_computed_start(
           start_list_ptr,
           start_value,
           append_start_type);

}

/*===========================================================================

FUNCTION   SEND_RBS_COMPLETE_MSG

DESCRIPTION

  This function sends the Radio Bearer Setup Complete message
  on Uplink DCCH.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_send_rbs_complete_message( boolean proc_wait_for_l2_ack )
{
  tdsrrc_UL_DCCH_Message *ptr;     /* Pointer to uplink DCCH message type */
  rlc_lc_id_type ul_lc_id; /* Logical Channel Id for the message  */
  tdsrrcsend_status_e_type status; /* Status from send chain module       */
  boolean new_tm_rb_setup = FALSE; /* Boolean to keep track of new TM RB setup */
  /*to get start value if new UM/AM rb's are added*/
  byte start_value[TDSRRC_MAX_START_LENGTH] = {0};

  /* Allocate memory for this message. Note that this buffer is released
  by the SEND CHAIN */
  ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc(sizeof(tdsrrc_UL_DCCH_Message));
  if (ptr == NULL)
  {
    ERR_FATAL("No memory for RBS Complete msg!",0,0,0);
  }

  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_radioBearerSetupComplete; /* RBS Complete msg */

  /* Fill up the message contents. The bit_mask is set to 0 since none
  of the optional IE's are included for now. */
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerSetupComplete);

  /* make sure that dl-counter-sync info is not received in the rb-setup message */
  if (FALSE == tdsrrc_rbe.rbe_for_srns_relocation)
  {
    if (tdsrrcsmc_get_and_reset_start_value_to_transmit(start_value) == TRUE)
    {
      /*if ciphering is enabled and new um/am rb's are setup, then send
        START value for n/w to sync up*/
      ptr->message.u.radioBearerSetupComplete.m.start_ValuePresent = 1;

      ptr->message.u.radioBearerSetupComplete.start_Value.numbits =
        TDSRRC_START_VALUE_LENGTH;

      /*copy the contents of latest_start_list into start_list*/
      memscpy(ptr->message.u.radioBearerSetupComplete.start_Value.data,TDSRRC_MAX_START_LENGTH,
             start_value,
             TDSRRC_MAX_START_LENGTH);

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Reconfig START: 0x%x 0x%x 0x%x", start_value[0],
                                                 start_value[1],
                                                 start_value[2]);
    }
  }
  /* in case its srns relocation then we need to set the 
    start value because this function will not be called.
    If this function would have been called earlier then 
    this call to this function is nothing but a NOOP.
    */
  (void) tdsrrcsmc_get_and_reset_start_value_to_transmit(start_value);
  
  if (tdsrrc_rbe.rbe_for_srns_relocation == TRUE)
  {
    (void)tdsrrcrbe_append_start_list_in_ul_counter_sync_info
      (ptr, start_value, TDSRRCSMC_EXTRACT_AND_APPEND_START);
  }
  else
  {
    if (tdsrrcllc_get_rlc_size_change_status() 
      && ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH || 
             tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH)
          || (tdsrrc_get_rlc_size_change_status_for_procedure_id(TDSRRC_PROCEDURE_RBE) == TRUE)))
    {
      // Check if next state is CELL+PCH, and OC API returns RLC size change, then include counter sync
      if (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH || 
          tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH)
      {
        /* Ask SMC to do start value calculation for PS domain */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RLCSIZEINDICATED Invoking SMC extract func");
        (void)tdsrrcsmc_extract_return_hfn_per_domain_and_update_start_flag(RRC_PS_DOMAIN_CN_ID);
      }
      else if (tdsrrc_get_rlc_size_change_status_for_procedure_id(TDSRRC_PROCEDURE_RBE) == TRUE)
      {
        tdsrrc_set_rlc_size_change_status(TDSRRC_PROCEDURE_NONE, 
                                       FALSE, TDSRRC_INVALID_RB_ID ,
                                       TDSRLC_RE_ESTABLISH_NONE);
      }

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsize change append start value");
      (void)tdsrrcrbe_append_start_list_in_ul_counter_sync_info
        (ptr, start_value, TDSRRCSMC_APPEND_START);
    }
  }
  

  /* Transaction Id - use the accepted transaction id if known. Else
  use zero. Since this is a RBS Complete message, we don't check
  rejected transaction ids. */
  if ( tdsrrc_rbe.tr_ids.accepted_transaction != TDSRRCRB_INVALID_TRANSACTION_ID )
  {
    if (tdsrrc_rbe.tr_ids.accepted_transaction<4)
    {
      ptr->message.u.radioBearerSetupComplete.rrc_TransactionIdentifier =
      tdsrrc_rbe.tr_ids.accepted_transaction;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid TransactionId:%d in RBE db sending 0",tdsrrc_rbe.tr_ids.accepted_transaction);
      ptr->message.u.radioBearerSetupComplete.rrc_TransactionIdentifier = 0;
    }
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again unless rb substate is TDSRRCRB_WAIT_FOR_L2_ACK */
    if (!proc_wait_for_l2_ack)
    {
      tdsrrc_rbe.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
    }
  }
  else
  {
    /* Don't know the transaction id for this message.( one reason
    for this could be that the message had a critical
    extenstion). Since it's unknown, we should set it to zero. */
    ptr->message.u.radioBearerSetupComplete.rrc_TransactionIdentifier = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Trid for RBS Complete msg!");
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

    new_tm_rb_setup = tdsrrcsmc_is_new_tm_rb_setup();

    if( (tdsrrcsmc_is_ciphering_enabled() == TDSRRCSMC_SECURITY_STARTED) &&
        (new_tm_rb_setup == TRUE) )
    {
      /*if ciphering is enabled and new tm rb's are setup, then send
        ciphering activation time in rb_setup_complete message*/
     TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->message.u.radioBearerSetupComplete,
        tdsrrc_RadioBearerSetupComplete,count_C_ActivationTime);

      ptr->message.u.radioBearerSetupComplete.count_C_ActivationTime =
        tdsciphering_activation_cfn;

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ciphering activation CFN = %d",tdsciphering_activation_cfn);
    }

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"hho_ciph_status: %d, trans_to_dch_ciph_config_needed: %d, new_tm_rb_setup: %d",
      tdsrrc_rbe.hho_ciph_status, tdsrrc_rbe.trans_to_dch_ciph_config_needed, new_tm_rb_setup);

    //if this is case of IFHHO and not SRNS relocation(i.e. ul counter sync is not present)
    //then also send START_List and set count c act time and configure smc for Step_2
    if (((tdsrrc_rbe.hho_ciph_status == TRUE) || (tdsrrc_rbe.trans_to_dch_ciph_config_needed == TRUE)) &&
        ((TRUE == tdsrrcsmc_tm_ciph_active()) && (FALSE == new_tm_rb_setup)))
    {
      (void)tdsrrcrbe_append_start_list_in_ul_counter_sync_info(
        ptr, start_value, TDSRRCSMC_EXTRACT_AND_APPEND_START);

      //Set the bitmask for count_c_act_time and copy the same..
      TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->message.u.radioBearerSetupComplete,
        tdsrrc_RadioBearerSetupComplete,count_C_ActivationTime);
      
      ptr->message.u.radioBearerSetupComplete.count_C_ActivationTime =
        (tdsrrc_ActivationTime)tdsrrcsmc_step_2_cipher_config_for_hho();

      //Send step 2 config for HHO.. only one of these flags will be active, reset both of them
      //here
      if(tdsrrc_rbe.hho_ciph_status)
      {
        tdsrrc_rbe.hho_ciph_status =  FALSE;      
      }
      else if(tdsrrc_rbe.trans_to_dch_ciph_config_needed)
      {
        tdsrrc_rbe.trans_to_dch_ciph_config_needed = FALSE;      
      }
    }
    /* Check for HHO condition */
    if(TRUE == tdsrrc_rbe.hho_ciph_status)
    {
      /* Check if TM bearers were established using the RBE message. If so do not do a Step-1, Step2
         ciphering config. Need to include UL Counter Sync info, but do not do the start value calculation 
         for the CS domain since it already has been done as a prt of channel config req. 
         Use the value that is appended in the "Start Value" above. Extract the start value
         for PS domain alone */
      if(TRUE == new_tm_rb_setup)
      {
        (void)tdsrrcrbe_append_start_list_in_ul_counter_sync_info(
          ptr, start_value, TDSRRCSMC_EXTRACT_AND_APPEND_START);
      }
    }

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"TM_CIPH_STATUS : %d FIRST TM RB STATUS :%d",tdsrrcsmc_tm_ciph_active(),tdsrrc_rbe.first_tm_rb_setup);
    if((TRUE == tdsrrcsmc_tm_ciph_active()) && (tdsrrc_rbe.first_tm_rb_setup == TRUE))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"First TM RB setup. Step2 HHO");
      if(!(TDSRRC_MSG_COMMON_BITMASK_IE(ptr->message.u.radioBearerSetupComplete,
        tdsrrc_RadioBearerSetupComplete,count_C_ActivationTime)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Bitmask To Set Count C Act Time");
        //Set the bitmask for count_c_act_time and copy the same..
        TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->message.u.radioBearerSetupComplete,
               tdsrrc_RadioBearerSetupComplete,count_C_ActivationTime);
      }

      ptr->message.u.radioBearerSetupComplete.count_C_ActivationTime =
      (tdsrrc_ActivationTime)tdsrrcsmc_step_2_cipher_config_for_first_tm_rb_setup();

      //Send step 2 config for HHO.. only one of these flags will be active, reset both of them
      //here
      tdsrrc_rbe.hho_ciph_status =  FALSE;      
      tdsrrc_rbe.first_tm_rb_setup = FALSE;
    }

    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
    {
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
      if(tdsrrcsib_is_sib11_11bis_12_read_deferred(TDSDEFERRED_ANY_ALLOWED_SIB))
      {
        ptr->message.u.radioBearerSetupComplete.m.laterNonCriticalExtensionsPresent=1;
 
        TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerSetupComplete.laterNonCriticalExtensions);
 
        ptr->message.u.radioBearerSetupComplete.laterNonCriticalExtensions.
            m.v770NonCriticalExtensionsPresent=1;
 
        TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerSetupComplete.laterNonCriticalExtensions.
            v770NonCriticalExtensions);
 
        TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.radioBearerSetupComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
          radioBearerSetupComplete_v770ext);
 
        ptr->message.u.radioBearerSetupComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
          radioBearerSetupComplete_v770ext.m.deferredMeasurementControlReadingPresent=1;
        
        ptr->message.u.radioBearerSetupComplete.laterNonCriticalExtensions.v770NonCriticalExtensions.
          radioBearerSetupComplete_v770ext.deferredMeasurementControlReading = 
          tdsrrc_RadioBearerSetupComplete_v770ext_IEs_deferredMeasurementControlReading_true_;
      }
#endif
    }

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RBS Complete message to send-chain");
    if (proc_wait_for_l2_ack)
    {
      status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBE,  /* Procedure is RBE    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_REQUIRED  /* ACK is required */
                                   );
    }
    else
    {
      status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RBE,  /* Procedure is RBE    */
                                   (void *)ptr,        /* Message pointer     */
                                   ul_lc_id,           /* RLC id              */
                                   TDSRRCSEND_L2ACK_NOT_REQUIRED  /* ACK is NOT required */
                                   );
    }

    if ( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Send chain failed for RBS complete msg: error cause %d",status);
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

}/* end function tdsrrcrbe_send_rbs_complete_message */


/*===========================================================================

FUNCTION   RRCRBE_SEND_INDICATION_TO_UPPER_LAYERS

DESCRIPTION

  This function processes the received Layer 2 ACK indication.
  If the L2 ACK indicates that the RBS Complete message has
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
static void tdsrrcrbe_send_indication_to_upper_layers
(
  void
)
{
  mm_cmd_type *mm_ptr;     /* To send the MM_SYNC_IND command */
  rabm_cmd_type *rabm_ptr; /* To send the RABMAS_RAB_ESTABLISH_IND command */
  uint32 rab_index=0;    /* Used if there is more than 1 RAB in the message */
  /* send the nas cn info if stored */
  if (tdsrrc_rbe.cn_info_saved != FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_SERVICE_IND_FROM_OTA_MSG due to CN info to MM");
    if(tdsrrc_rbe.mm_cmd_ptr->cmd.hdr.message_id ==(byte) RRC_SERVICE_IND_FROM_OTA_MSG)
    {
      /* update the cell access information in service info */
      tdsrrcccm_update_service_info(tdsrrc_rbe.mm_cmd_ptr);
    }
    /* Put the command on MM queue */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(tdsrrc_rbe.mm_cmd_ptr);
    }
    mm_put_cmd(tdsrrc_rbe.mm_cmd_ptr);
    /* clear the flag */
    tdsrrc_rbe.cn_info_saved = FALSE;
  }

  /* Now we need to send primitives to the appropriate NAS entity.
    CS calls, it's to the MM layer and for PS calls it's to the RABM layer.*/
  for (rab_index=0; rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg; rab_index++)
  {
    if ( (tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB) ||
         (tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_DATA_RAB) )
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
      mm_ptr->cmd.rrc_sync_ind.rab_info.rab_id = tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id;

      if(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB)
      {
        mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_CS_VOICE_CALL;
      }
      else if(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_DATA_RAB)
      {
        mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_CS_DATA_CALL;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Something wrong with RAB Type");      
        mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_INVALID_RAB_ID;
      }
  
      /* Fill in the codec type used in RRC_SYNC_IND*/ 
      mm_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_NONE;
      if(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB)
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
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Codec %d in RRC_SYNC_IND",mm_ptr->cmd.rrc_sync_ind.rab_info.codec);

#ifdef FEATURE_UMTS_VOICE_CIPHERING_IND
      #error code not present
#endif /*FEATURE_UMTS_VOICE_CIPHERING_IND*/

      /* Put the command on MM queue */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sending RRC_SYNC_IND to MM for RAB type %d RAB-id %d",mm_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type,mm_ptr->cmd.rrc_sync_ind.rab_info.rab_id);
      if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
      {
        tdscfa_mm_put_cmd(mm_ptr);
      }
      mm_put_cmd(mm_ptr);
    }

    else if ( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_PS_DATA_RAB )
    {

      if(tdsrrcscrr_is_cn_domain_open(RRC_PS_DOMAIN_CN_ID) == TRUE)
      {
        /* It's a PS RAB. Send the RABMAS_RAB_ESTABLISH_IND primitive to RABM. */
        if ((rabm_ptr = rabm_get_cmd_buf()) == NULL)
        {
          ERR_FATAL("Can't send RABM cmd - out of memory!",0,0,0);
        }
        rabm_ptr->header.cmd_id = RABMAS_RAB_ESTABLISH_IND;
        rabm_ptr->header.message_set = MS_RABM_RRC;
        rabm_ptr->cmd.rrc_establish_ind.rabid = (rabid_T)(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id);
#ifdef FEATURE_DUAL_SIM
        rabm_ptr->cmd.rrc_establish_ind.as_id = tdsrrc_get_as_id();
#endif
        /* Put the command on RABM queue */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending RAB_ESTABLISH_IND to RABM RABid %d",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id);
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

} /*rrcrbe_send_indication_upper_layers */

/*===========================================================================

FUNCTION tdsrrcrbe_send_crnti_update_req

DESCRIPTION
  This function sends the RNTI Update request to LLC to update
  with new C-RNTI.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void  tdsrrcrbe_send_crnti_update_req(uint16  crnti)
{
  tdsrrc_cmd_type  *cmd_ptr;

  /* Allocates the buffer to RRC Internal command */
  if( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_RNTI_UPDATE_REQ;

    /* Fill parameters for TDSRRC_RNTI_UPDATE_REQ command */
    cmd_ptr->cmd.rnti_update_req.procedure = TDSRRC_PROCEDURE_RBE;

    /* Indicate that C-RNTI needs to be updated */
    cmd_ptr->cmd.rnti_update_req.crnti_action = TDSRRC_RNTI_UPDATE;
    cmd_ptr->cmd.rnti_update_req.crnti = crnti;

    /* Indicate that U-RNTI has not changed. Not till S RNS relocation
     * is supported.
     */
    cmd_ptr->cmd.rnti_update_req.urnti_action = TDSRRC_RNTI_NOCHANGE;

    cmd_ptr->cmd.rnti_update_req.rlc_re_establish_srb = FALSE;
    cmd_ptr->cmd.rnti_update_req.rlc_re_establish_rab = FALSE;

    /* Confirmation from LLC is not needed */
    cmd_ptr->cmd.rnti_update_req.cnf_required = FALSE;

    tdsrrc_put_int_cmd(cmd_ptr);       /* sends the command to RRC  LLC. */

  }
  else  /* RRC Task failed to allocate memory */
  {
    ERR_FATAL("Unable to allocate memory for cmd: %d",TDSRRC_RNTI_UPDATE_REQ,0,0);
  }
}  /* tdsrrcrbe_send_crnti_update_req */

/*===========================================================================

FUNCTION tdsrrcrbe_send_cell_update_initiate_req

DESCRIPTION
  This function send the cell update initiate request if UE does not have
  a valid C-RNTI in CELL_FACH state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void tdsrrcrbe_send_cell_update_initiate_req
(
tdsrrc_proc_e_type procedure
)
{
  tdsrrc_cmd_type *int_cmd_ptr;

  if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
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
}/*tdsrrcrbe_send_cell_update_initiate_req*/

/*===========================================================================

FUNCTION   RRCRBE_VALIDATE_SRNS_RELOCATION_INFO()

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
boolean tdsrrcrbe_validate_srns_relocation_info
(
tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
,tdsrrc_msg_ver_enum_type msg_version
)
{
   /*check whether dl_counter_sync info is present or not . Do not check for
   the contents of DL_counter_sync_info. First the integrity check has tobe made
   with new integrity protection mode info in the message. Later we can check
   the dl_counter_sync_info contents and send a failure if PDCP info is present */

#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Validate if RBE is for srns relocation");
  if (TRUE == tdsrrc_is_fresh_in_reconfiguration_message())
  {
    return TRUE;
  }
#endif
  if (msg_version == TDSMSG_REL5)
  {
    if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.radioBearerSetup_r5,
      tdsrrc_RadioBearerSetup_r5_IEs,dl_CounterSynchronisationInfo))
    {
      return TRUE;
    }
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                 msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.radioBearerSetup_r5,
                                               new_U_RNTI))
    { /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.
             radioBearerSetup_r5.new_U_RNTI)) == FALSE)
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
  else if (msg_version == TDSMSG_REL6)
  {
     if (TDSRRC_CHECK_COMMON_MSG_TYPE(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
       criticalExtensions.u.r6.radioBearerSetup_r6.specificationMode,
       rrc_RadioBearerSetup_r6_IEs_specificationMode_complete) &&
      (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
       criticalExtensions.u.r6.radioBearerSetup_r6.specificationMode.u.
       complete.m.dl_CounterSynchronisationInfoPresent))
     {
       return TRUE;
     }
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
         msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
           criticalExtensions.u.r6.radioBearerSetup_r6, new_U_RNTI))
    {
      /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
           criticalExtensions.u.r6.radioBearerSetup_r6.new_U_RNTI)) == FALSE)
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
  else if (msg_version == TDSMSG_REL7)
  {
     if ((TDSRRC_CHECK_COMMON_MSG_TYPE(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
       criticalExtensions.u.r7.radioBearerSetup_r7.specificationMode,
       rrc_RadioBearerSetup_r7_IEs_specificationMode_complete)) &&
      (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
       criticalExtensions.u.criticalExtensions.u.r7.radioBearerSetup_r7.specificationMode.u.complete.m. 
       dl_CounterSynchronisationInfoPresent))
     {
       return TRUE;
     }
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
         msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
           criticalExtensions.u.r7.radioBearerSetup_r7, new_U_RNTI))
    {
      /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
           criticalExtensions.u.r7.radioBearerSetup_r7.new_U_RNTI)) == FALSE)
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
  else if (msg_version == TDSMSG_REL8)
  {
    if (TDSRRCRB_R8_MSG_IE_PRESENT((&msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerSetup_r8),tdsrrc_RadioBearerSetup_r8_IEs,
        dl_CounterSynchronisationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:DL Sync Info is present");
      return TRUE;
    }
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
             msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
               criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerSetup_r8, new_U_RNTI))
    {
      /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
               criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerSetup_r8.new_U_RNTI)) == FALSE)
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
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
    else if (msg_version == TDSMSG_REL9)
    {
      if (TDSRRCRB_R9_MSG_IE_PRESENT((&msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
          criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.radioBearerSetup_r9),tdsrrc_RadioBearerSetup_r9_IEs,
          dl_CounterSynchronisationInfo))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:DL Sync Info is present");
        return TRUE;
      }
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
             msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
              criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.radioBearerSetup_r9, new_U_RNTI))
    {
      /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
            criticalExtensions.u.criticalExtensions.u.criticalExtensions.
           u.criticalExtensions.u.r9.radioBearerSetup_r9.new_U_RNTI)) == FALSE)
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
#endif /*FEATURE_TDSCDMA_REL9*/


/*rest is for version == 4*/
  else if (msg_version == TDSMSG_REL4)
  {
  if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerSetup_r4,
      tdsrrc_RadioBearerSetup_r4_IEs,dl_CounterSynchronisationInfo))
  {
    return TRUE;
  }
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerSetup_r4, new_U_RNTI))
     {
        /*check if SRNC-ID changed*/
        if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.
                                                 u.r4.radioBearerSetup_r4.new_U_RNTI)) == FALSE)
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

FUNCTION   tdsrrcrbe_is_first_tm_rb_setup

DESCRIPTION

  This function checks whether any of TM RBs have been already established or not.

DEPENDENCIES

  None.

RETURN VALUE

  TRUE : If a new TM RB is being set up and there's no existing TM RB in the RABs already established.
  FALSE : Otherwise

SIDE EFFECTS

  None.

===========================================================================*/
static boolean tdsrrcrbe_is_first_tm_rb_setup
(
  boolean new_tm_rb
)
{
  boolean status = TRUE;
  uint8 i=0, j=0, lc_id;
  tdsrrclcm_status_e_type rrclcm_status = TDSRRCLCM_FAILURE;
  uecomdef_logch_mode_e_type rx_mode;
  boolean dl_mode_tm = FALSE;
  
  if(new_tm_rb)
  {
    for(i=0;i<MAX_RAB_TO_SETUP;i++)
    {
      for(j=0;j<tdsrrc_est_rabs.rabs[i].num_rbs_for_rab;j++)
      {
        dl_mode_tm = FALSE;
        lc_id = tdsrrclcm_get_dl_rlc_lc_id(tdsrrc_est_rabs.rabs[i].rb_for_rab[j].rb_id);
        if( lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
        {
          rrclcm_status = tdsrrclcm_get_dl_rlc_mode(lc_id, &rx_mode);
          if(rrclcm_status == TDSRRCLCM_SUCCESS && rx_mode == UE_MODE_TRANSPARENT)
          {
            dl_mode_tm = TRUE;
          }
        }

        if(dl_mode_tm == TRUE)
        {
          lc_id = tdsrrclcm_get_ul_rlc_lc_id(tdsrrc_est_rabs.rabs[i].rb_for_rab[j].rb_id);
          if( lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
          {
            rrclcm_status = tdsrrclcm_get_ul_rlc_mode(lc_id, &rx_mode);
            if(rrclcm_status == TDSRRCLCM_SUCCESS && rx_mode == UE_MODE_TRANSPARENT)
            {
              status = FALSE;
              break;
            }
          }
        }
      }
      if(status == FALSE)
      {
        break;
      }
    }
  }
  else
  {
    status = FALSE;
  }

  return status;
}


/*===========================================================================

FUNCTION   VALIDATE_RBSETUP_R5

DESCRIPTION

  This function validates the received R5 RB Setup Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Setup Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_validate_rbsetup_r5
(
tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
)
{

  uecomdef_status_e_type status;

  tdsrrc_RadioBearerSetup_r5_IEs* rbsetup_ptr = NULL;

  tdsrrc_RAB_InformationSetupList_r5 *rab_list_ptr = NULL;

  tdsrrc_RB_InformationSetupList_r5 *rb_list_ptr = NULL;

#ifdef FEATURE_UMTS_PDCP

  tdsrrc_HeaderCompressionInfo_r4 *pdcpHeaderCompressionInfo = NULL;

#endif /* FEATURE_UMTS_PDCP*/


  boolean new_tm_rb = FALSE;
  rlc_lc_id_type local_lc_id;
  uecomdef_logch_mode_e_type rx_mode;
  tdsrrclcm_status_e_type result;
  boolean dl_tm_flag;

  rrc_rab_id_type rab_id;           /* To store RAB id in internal RRC format */
  rrcmn_rab_status_T rab_type = RRCMN_INVALID_RAB_ID;     /* To store the status returned by MN layer. This
                                    indicates what type of RAB is received. This is
                                    used only if the RAB is for the CS_domain */
  uint32 rab_index=0;               /* Used as an index when storing established
                                    RABs locally. Only when we have more than
                                    one RAB per message, this will be incremented */
  uint32 rb_index =0;                          
  boolean voice_rab_already_exists = FALSE;
 /* set the default nas_sync_ind to the current codec used*/
  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();
  /* Assume the message is valid for now */
  tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_VALID;


  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.radioBearerSetup_r5;
  switch (rbsetup_ptr->rrc_StateIndicator)
  {
  case tdsrrc_RRC_StateIndicator_cell_DCH:

    tdsrrc_rbe.state_from_msg = TDSRRC_STATE_CELL_DCH;
    break;

  case tdsrrc_RRC_StateIndicator_cell_FACH:

    tdsrrc_rbe.state_from_msg = TDSRRC_STATE_CELL_FACH;
    break;

  default: /* Just to satisfy LINT */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid RRC state from Msg");
    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    break;

  } /* End of switch */

  if ( tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
    return;
  }

  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r5_IEs,
    dl_HSPDSCH_Information))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSPDSCH Info present");

    /* Check if network does not try to initiate HSDPA with invalid state */

    status = tdsrrc_hsdpa_initial_validation(rbsetup_ptr->rrc_StateIndicator, 
      TDSRRC_CHECK_COMMON_MSG_TYPE(rbsetup_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
      rrc_DL_HSPDSCH_Information_modeSpecificInfo_fdd));


    if (status == FAILURE)
    {
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }
  rab_index =0;
  /* Check if there are new RABs to be established */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
    tdsrrc_RadioBearerSetup_r5_IEs,rab_InformationSetupList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS: RAB Info present in RBS Message");

    rab_list_ptr = &rbsetup_ptr->rab_InformationSetupList;

    /* Check if the RAB id is GSM_MAP type. We don't support any
    other type */
    if ( !(rab_list_ptr->elem[rab_index].rab_Info.rab_Identity.t ==
          T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB ID t%d is not GSM_MAP!",rab_list_ptr->elem[rab_index].rab_Info.rab_Identity.t);
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;
    }

    /* check for the unsupported PDCP info */

    do  /* going through the RAB info setup list */
    {
      rb_list_ptr = &rab_list_ptr->elem[rab_index].rb_InformationSetupList;
      rb_index= 0;
      do  /* going through the RB info setup list */
      {
        tdsrrc_RB_InformationSetup_r5 * rb_setup_info_ptr;
        rb_setup_info_ptr = &rb_list_ptr->elem[rb_index];
        if (tdsrrc_find_rb_in_est_rabs(rb_setup_info_ptr->rb_Identity) == TDSRRC_RB_PRESENT) 
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB id %d already exist in est rabs: Invalid config",rb_setup_info_ptr->rb_Identity);
          tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          return;
        }

        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_setup_info_ptr,
            tdsrrc_RB_InformationSetup_r5,pdcp_Info))
        {
#ifdef FEATURE_UMTS_PDCP
          if (tdsrrc_pdcp_enabled)
          {
           /* If the RAB is for cs domain, send RBS failure with cause
             *   invalid configuration
             */
            if (rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info invalid for CS rab");
              tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
              return;
            }
            /* check whether lossless SRNS relocation flag is set
             * and header compression algo is present in the message */
            if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_setup_info_ptr->pdcp_Info,
                tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
                (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_setup_info_ptr->pdcp_Info.losslessSRNS_RelocSupport,
                rrc_LosslessSRNS_RelocSupport_supported)))
            {
              if (rb_setup_info_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_absent)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP pdu header is absent.  Invalid config");
                tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                return;
              }

            } /* if rrc_PDCP_Info_r4_losslessSRNS_RelocSupport_present */
            if (rb_setup_info_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present)
            {
              if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_setup_info_ptr->pdcp_Info,
                  tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
                  (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_setup_info_ptr->pdcp_Info.losslessSRNS_RelocSupport,
                   rrc_LosslessSRNS_RelocSupport_notSupported)))
              {
                if (!(TDSRRC_MSG_COMMON_BITMASK_IE(rb_setup_info_ptr->pdcp_Info,
                    tdsrrc_PDCP_Info_r4,headerCompressionInfoList)))
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP header compress info list not present");
                  tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                  return;
                }
              }
            }

            if (TDSRRC_MSG_COMMON_BITMASK_IE(rb_setup_info_ptr->pdcp_Info,
                tdsrrc_PDCP_Info_r4,headerCompressionInfoList))
            {
              uint32 local_idx=0;
             
              while (rb_setup_info_ptr->pdcp_Info.headerCompressionInfoList.n > local_idx)
              {
                 pdcpHeaderCompressionInfo = &rb_setup_info_ptr->pdcp_Info.headerCompressionInfoList.elem[local_idx];
                if (TDSRRC_CHECK_COMMON_MSG_TYPE(pdcpHeaderCompressionInfo->algorithmSpecificInfo,
                    rrc_AlgorithmSpecificInfo_r4_rfc2507_Info))
                {
                  /* if the UE capability "Maximum header compression context space", is exceeded with this configuration send
                   * failure with cause "Invalid Config"
                   */
                  if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER > IPHC_MAX_HC_CONTEXT_SPACE)
                  {
                    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  max header %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER);
                    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                    return;
                  }
                }
                else if (TDSRRC_CHECK_COMMON_MSG_TYPE(pdcpHeaderCompressionInfo->algorithmSpecificInfo,
                         rrc_AlgorithmSpecificInfo_r4_rfc3095_Info))
                {
                  /*    the chosen MAX_CID shall not be greater than the value "Maximum number of ROHC context sessions" as 
                   *  indicated in the IE "PDCP Capability"
                   */
                  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.
                    u.rfc3095_Info,ul_RFC3095))
                  {
                    if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID);
                      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                      return;
                    }
                  }

                  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,
                    dl_RFC3095))
                  {
                    if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID);
                      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                      return;
                    }
                  }

                }  /* if (pdcpHeaderCompressionInfoList->value.algorithmSpecificInfo.t == rrc_rfc3095_Info)*/

                local_idx++;
              } /*  while (pdcpHeaderCompressionInfoList) */

            } /*  if (rb_list_ptr->value.pdcp_Info.bit_mask & rrc_PDCP_Info_r4_headerCompressionInfoList_present) */
          }
#endif
        } /* if (rb_list_ptr->value.bit_mask & rrc_RB_InformationSetup_r5_pdcp_Info_present) */

        if(new_tm_rb == FALSE)
        {
          dl_tm_flag = FALSE;
          
          if((TDSRRC_CHECK_COMMON_MSG_TYPE(rb_setup_info_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_r5_rlc_Info)) &&
          (((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_setup_info_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,ul_RLC_Mode)) &&
               (rb_setup_info_ptr->rlc_InfoChoice.u.rlc_Info->ul_RLC_Mode.t==
               T_tdsrrc_UL_RLC_Mode_ul_TM_RLC_Mode))
               &&
               ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_setup_info_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,dl_RLC_Mode)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_setup_info_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode,
                rrc_DL_RLC_Mode_r5_dl_TM_RLC_Mode)))))
          {
            new_tm_rb = TRUE;
          }
          else if(TDSRRC_CHECK_COMMON_MSG_TYPE(rb_setup_info_ptr->rlc_InfoChoice,
            rrc_RLC_InfoChoice_r5_same_as_RB))
          {
            local_lc_id = tdsrrclcm_get_dl_rlc_lc_id(rb_setup_info_ptr->rlc_InfoChoice.u.same_as_RB);
            if(local_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
            {
              result = tdsrrclcm_get_dl_rlc_mode(local_lc_id, &rx_mode);
              if(result == TDSRRCLCM_SUCCESS && rx_mode == UE_MODE_TRANSPARENT)
              {
                dl_tm_flag = TRUE;
              }
            }
  
            local_lc_id = tdsrrclcm_get_ul_rlc_lc_id(rb_setup_info_ptr->rlc_InfoChoice.u.same_as_RB);
            if(local_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
            {
              result = tdsrrclcm_get_ul_rlc_mode(local_lc_id, &rx_mode);
              if(dl_tm_flag == TRUE && result == TDSRRCLCM_SUCCESS && rx_mode == UE_MODE_TRANSPARENT)
              {
                new_tm_rb = TRUE;
              }
            }
          }
        }
        /*point to the next link */
        rb_index++;
      } while (rb_list_ptr->n > rb_index);
      /* Go to next RAB setup infomation  */
      rab_index++;
    } while (rab_list_ptr->n > rab_index);

  }
  else
  {
    /* There is no RAB to be setup in this message.
      No further validation is needed */
    tdsrrc_rbe.rabs.num_rabs_in_msg = 0;
  }
  if(new_tm_rb == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"One or more TM RBs being setup");
  }
  /*check for the SRNS relocation info */
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
      tdsrrc_RadioBearerSetup_r5_IEs,dl_CounterSynchronisationInfo))
#else
    if (tdsrrcrbe_validate_srns_relocation_info(msg_ptr, TDSMSG_REL5))
#endif

  {

    /*check whether the SRNS relocation is within Cell_DCH or not.
  SRNS relocation information can come in reconfiguration messages
    only when the state transition is Cell_DCH->Cell_DCH */

    if (!((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH) &&
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbe.state_from_msg);
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has
    PDCP info list. If it is true then this configuration is unsupported */

    if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rbsetup_ptr->dl_CounterSynchronisationInfo,
        rb_WithPDCP_InfoList)) ||
        (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CounterSynchronisationInfo,
         tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation.
      Set the flag that indicates that this procedure
        involves SRNS relocation */
      tdsrrc_rbe.rbe_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
          tdsrrc_RadioBearerSetup_r5_IEs,new_U_RNTI))
      {
        tdsrrc_rbe.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(rbsetup_ptr->new_U_RNTI),
                             &tdsrrc_rbe.new_urnti);
      }
    }

  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
    tdsrrc_RadioBearerSetup_r5_IEs,rab_InformationSetupList))
  {
    /* GSM_MAP RAB Id. Check if it's valid by calling the
    appropriate NAS entity. First convert it to internal
    format. */
    rab_index =0;
    rab_list_ptr = &rbsetup_ptr->rab_InformationSetupList;

    /* check whether all RABs in the rabinfo list belong to same CN domain id or not
       otherwise this is an invalid configuration */
    while (rab_list_ptr->n > (rab_index+1) )
    {
      if (rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity !=
          rab_list_ptr->elem[rab_index+1].rab_Info.cn_DomainIdentity)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Different CN domains in rab-list %d %d",rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity,rab_list_ptr->elem[rab_index+1].rab_Info.cn_DomainIdentity);
        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        return;
      }
      rab_index++;;
    }
  
    /* Restore the Ptr */
    rab_list_ptr = &rbsetup_ptr->rab_InformationSetupList;
  
  
    for (rab_index=0; (rab_list_ptr->n > rab_index) && (rab_index < TDSRRCRB_MAX_RAB_PER_MSG); rab_index++)
    {
      rab_id = tdsrrcrb_translate_gsm_map_rab_id((rab_list_ptr->elem[rab_index].rab_Info.
                                                rab_Identity.u.gsm_MAP_RAB_Identity));
  
      /* Store this info locally so that ESTABLISHED_RABS can
    be updated later, when these RABs are established */
      tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id_is_gsm_map = TRUE;
      tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id = rab_id;
      tdsrrc_rbe.rabs.rabs_in_msg[rab_index].re_est_timer =
      rab_list_ptr->elem[rab_index].rab_Info.re_EstablishmentTimer;
      /* Check if this RAB is CS Domain or PS Domain */
      if ( rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
      {
        /* Store the domain identity. This will be later added to
          ESTABLISHED_RABS */
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain = RRC_CS_DOMAIN_CN_ID;
  
        /* For CS RABs, Call MN layer to verify this RAB */
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
        rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
        rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"For RAB-Id %d, RAB type from NAS = %d, RAB type from EST_RABs = %d",rab_id, rab_type,
            tdsrrc_get_rab_type_for_given_rab_id(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id));
  
        switch ( rab_type )
        {
          case RRCMN_CS_DATA_CALL:
  
            TDSRRC_MSG1(MSG_LEGACY_LOW,"Received a CS Data RAB Id %d",rab_id);
            break;
  
          case RRCMN_CS_VOICE_CALL:

            {
              TDSRRC_MSG1(MSG_LEGACY_LOW,"Received a CS Voice RAB Id %d",rab_id);
              if (tdsrrc_find_if_voice_rab_exists_in_established_rabs( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id )
                == TRUE)
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Voice RAB Id - %d already exists",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id );
                /*"if" covers the case when third RB is being added for AMR call*/
                if(rab_list_ptr->elem[rab_index].rb_InformationSetupList.n == 1)
                {
                  /* Most likely network is trying to add third RB to switch from lower AMR rate to higher */
                  voice_rab_already_exists = TRUE;
                }
                else
                {
                  tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid configuration .. more than one RAB subflow is being added");
                  return;
                }
              }
              else
              {
                /* Whenever a new voice RAB is to be set up, at least two RBs need to be associated with that RAB.
                Therefore, here checks for presence of more than 1 RB. */
                if (rab_list_ptr->elem[rab_index].rb_InformationSetupList.n == 1)
                {
#ifdef FEATURE_SCUDIF_ENHANCEMENT
                  #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/

                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Not enough RBs mapped to voice RAB!");
                  tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
                }
              }
            }
  
            break;
  
          default:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS RAB validation failed! RABid %d",rab_id);
            break;
  
        } /* end switch */
  
        if ( (tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_INVALID_CONFIG) ||
           (tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG))
        {
          /* No need to continue */
          return;
        }

#ifdef FEATURE_SCUDIF_ENHANCEMENT
        #error code not present
#else
        if(rab_type == RRCMN_CS_VOICE_CALL )
#endif/*FEATURE_SCUDIF_ENHANCEMENT*/
        {
          /* If the NAS Sync Indicator IE is present in the OTA msg then , extract it and update the RBE database */
          if(rab_list_ptr->elem[rab_index].rab_Info.m.nas_Synchronisation_IndicatorPresent)
          {
            nas_sync_ind =  tdsrrc_translate_nas_sync_indicator(&(rab_list_ptr->elem[rab_index].rab_Info.nas_Synchronisation_Indicator));
            if( nas_sync_ind == TDSCDMA_IVOCODER_ID_NONE )
            {
              tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported  configuration .. codec not supported %d",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator );
              return;
            }
          }
          tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator = nas_sync_ind;    
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM   
          tdsrrc_set_ordered_codec_status(TRUE);
#endif
        }

      }
      /* Else check for PS Domain */
      else if ( rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain )
      {
        /* Store the domain identity. This will be later added to
          ESTABLISHED_RABS */
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain = RRC_PS_DOMAIN_CN_ID;
  
        /* For PS Domain, if it's a loopback, then no need for query,
        otherwise, call RABM module to verify the RAB */
        if((tdsrrclbt_is_lb_on() == TRUE) ||
#ifdef FEATURE_DUAL_WCDMA
           (rabm_rabid_validate_per_sub(tdsrrc_get_as_id(), RAB_ESTABLISH_QUERY, (uint8)rab_id) == TRUE)
#else
           (rabm_rabid_validate(RAB_ESTABLISH_QUERY, (uint8)rab_id) == TRUE)
#endif
        )

        {
          TDSRRC_MSG1(MSG_LEGACY_LOW,"PS RAB Id %d to be setup",rab_id);
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"PS RAB validation failed! RABid %d",rab_id);
          tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
          return;
        }
      }
  
      if(voice_rab_already_exists == TRUE)
      {
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_VOICE_RAB;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Updated RAB type as voice");
      }
      else
      {
        /* ASN1 structure 'tdsrrc_RAB_InformationSetup' is validated at this point,
            hence update rab_type in structure 'tdsrrc_rbe.rabs.rabs_in_msg[rab_index]'. */
        (void) tdsrrc_update_rab_type_in_rab_info_r5(
           &(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
                      u.r5.radioBearerSetup_r5.rab_InformationSetupList.elem[rab_index]),
           &(tdsrrc_rbe.rabs.rabs_in_msg[rab_index]));
      }
  
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
               tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id,
               tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain,
               tdsrrc_rbe.rabs.rabs_in_msg[rab_index].re_est_timer);
      /* set the ordered codec mode which is sent in the ordered config to MAC.
        * If the RBE msg is not for Voice RAB also then also the current codec value
        * is passed to MAC 
        */
      tdsrrc_set_ordered_codec_mode(nas_sync_ind);
      tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator = nas_sync_ind;
  
      /* Go to next RAB info  */
      
    }
    if (rab_list_ptr->n > rab_index)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Too many RABs in an RBSetupMessage");
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;    
    }
  }
  /* Store the number of the RAB info in the RBSetup message. */
  tdsrrc_rbe.rabs.num_rabs_in_msg = rab_index;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"%d RABs in an RBSetupMessage",rab_index);

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  tdsrrc_rbe.trans_to_dch_ciph_config_needed = FALSE;
  if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH) && 
      (tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH)
     )
  {
    /* check to see if dl-dpch-common info is present while going to dch */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
        tdsrrc_RadioBearerSetup_r5_IEs,dl_CommonInformation)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CommonInformation, 
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
             (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CommonInformation.dl_DPCH_InfoCommon,
             tdsrrc_DL_DPCH_InfoCommon_r4,mac_d_HFN_initial_value)),
             &rbsetup_ptr->dl_CommonInformation.dl_DPCH_InfoCommon.mac_d_HFN_initial_value,
             &mac_d_hfn_l);
        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }
        if (tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing Ciphering Config for HHO failed");
        }
        tdsrrc_rbe.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }
  else if((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)&&
      (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH))
  {
    tdsrrc_rbe.first_tm_rb_setup = tdsrrcrbe_is_first_tm_rb_setup(new_tm_rb);
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"First TM RB being set status : %d",tdsrrc_rbe.first_tm_rb_setup);
    if(tdsrrc_rbe.first_tm_rb_setup == TRUE)
    {
      /* check to see if dl-dpch-common info is present while going to dch */
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
         tdsrrc_RadioBearerSetup_r5_IEs,dl_CommonInformation)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CommonInformation,
           tdsrrc_DL_CommonInformation_r5,dl_DPCH_InfoCommon)))
      {
        /* check if ciphering is active in mac for cs domain, if it is then,
          start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
        */
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"First time TM RB Setup HHO Step-1 config activated");
        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CommonInformation.dl_DPCH_InfoCommon,
             tdsrrc_DL_DPCH_InfoCommon_r4,mac_d_HFN_initial_value)),
             &rbsetup_ptr->dl_CommonInformation.dl_DPCH_InfoCommon.mac_d_HFN_initial_value,
             &mac_d_hfn_l);
        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }
        if (tdsrrcsmc_process_cipher_config_for_first_tm_setup(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing Ciphering Config for First TM RB Setup failed");
          /*This step is reached only when the first step returns a failure,. i.e, if the ciphering is not active for CS domain.
              Hence setting tdsrrc_rbe.first_tm_rb_setup to FALSE so that step 2 is not done*/
          tdsrrc_rbe.first_tm_rb_setup = FALSE;
        }
      }
    }
  }
  /* If UTRAN DRX coefficient is not present then it is invalid configuration */
  if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_rbe.state_from_msg  == TDSRRC_STATE_URA_PCH))
  {

    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
          tdsrrc_RadioBearerSetup_r5_IEs,utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_rbe.state_from_msg);
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }
  if (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH)
  {
    /*should we do it after getting l2ack for response??*/
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
          tdsrrc_RadioBearerSetup_r5_IEs,ura_Identity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
      tdsrrccu_clear_ura_id ();
    }
    else
    {
      /*URA ID present in the message set ura_identity variable with this value */
      if (rbsetup_ptr->ura_Identity.numbits == 16)
      {
        tdsrrccu_update_ura_id (&(rbsetup_ptr->ura_Identity));
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rbsetup_ptr->ura_Identity.numbits);
        tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
        tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
        return;
      }
    }
  }


  return;

}/* end function tdsrrcrbe_validate_rbsetup_r5 */


/*===========================================================================

FUNCTION   VALIDATE_RBSETUP_R6

DESCRIPTION

  This function validates the received R6 RB Setup Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Setup Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_validate_rbsetup_r6
(
tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
)
{
  uecomdef_status_e_type status;

  tdsrrc_RadioBearerSetup_r6_IEs* rbsetup_ptr = NULL;

  tdsrrc_RAB_InformationSetupList_r6 *rab_list_ptr = NULL;

  tdsrrc_RAB_InformationReconfigList *rab_reconfig_list_ptr = NULL;

  tdsrrc_rab_search_e_type tdsrab_status; /* To check if RAB is present in ESTABLISHED_RABS */

  tdsrrc_RB_InformationSetup_r6 *rb_info_ptr = NULL;

#ifdef FEATURE_UMTS_PDCP

   tdsrrc_HeaderCompressionInfo_r4 *pdcpHeaderCompressionInfo = NULL;

#endif /* FEATURE_UMTS_PDCP*/


  rrc_rab_id_type rab_id;           /* To store RAB id in internal RRC format */
  rrcmn_rab_status_T rab_type = RRCMN_INVALID_RAB_ID;     /* To store the status returned by MN layer. This
                                    indicates what type of RAB is received. This is
                                    used only if the RAB is for the CS_domain */
  uint32 rab_index=0;               /* Used as an index when storing established
                                    RABs locally. Only when we have more than
                                    one RAB per message, this will be incremented */
  uint32 rb_index =0;
  boolean new_tm_rb = FALSE;
  rlc_lc_id_type local_lc_id;
  uecomdef_logch_mode_e_type rx_mode;
  tdsrrclcm_status_e_type result;
  boolean dl_tm_flag;

  boolean voice_rab_already_exists = FALSE;
  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();



  /* Assume the message is valid for now */
  tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_VALID;


  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
    u.r6.radioBearerSetup_r6;
  switch (rbsetup_ptr->rrc_StateIndicator)
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbe.state_from_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbe.state_from_msg = TDSRRC_STATE_CELL_FACH;
      break;

    default: /* Just to satisfy LINT */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid RRC state from Msg");
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;

  } /* End of switch */

  if ( tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
    return;
  }

  /*Reject RB setup with default configuration */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rbsetup_ptr->specificationMode,
      rrc_RadioBearerSetup_r6_IEs_specificationMode_dummy))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Default config not expected on rbsetup");
    tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
    return;
  }

  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r6_IEs,dl_HSPDSCH_Information) )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL:HSPDSCH Info present");

    /* Check if network does not try to initiate HSDPA with invalid state */
    status = tdsrrc_hsdpa_initial_validation_r6(rbsetup_ptr->rrc_StateIndicator,
                                             &rbsetup_ptr->dl_HSPDSCH_Information);

    if (status == FAILURE)
    {
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }
 

  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rbsetup_ptr->specificationMode,
      rrc_RadioBearerSetup_r6_IEs_specificationMode_complete))
  {
    /* Check if there are new RABs to be established */

    if (rbsetup_ptr->specificationMode.u.complete.m.rab_InformationSetupListPresent)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL: RAB Info present in RBS Message");
      rab_list_ptr = &rbsetup_ptr->specificationMode.u.complete.rab_InformationSetupList;
      rab_index =0;
      /* check for the unsupported PDCP info */
  
      while (rab_list_ptr->n > rab_index)  /* going through the RAB info setup list */
      {
        /* Check if the RAB id is GSM_MAP type. We don't support any
           other type */
        if ((rab_list_ptr->elem[rab_index].rab_Info.rab_Identity.t 
             != T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB ID t%d is not GSM_MAP!",rab_list_ptr->elem[rab_index].rab_Info.rab_Identity.t);
          tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
          return;
        }
  
        
        if (rab_list_ptr->n > (rab_index +1))
        {
          if (rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity !=
              rab_list_ptr->elem[rab_index+1].rab_Info.cn_DomainIdentity)
          {
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"Different CN domains in rab-list %d %d",rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity,rab_list_ptr->elem[rab_index+1].rab_Info.cn_DomainIdentity);
            tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
            return;
          }
        }
        /* Reject if MBMS info is included */
        if (rab_list_ptr->elem[rab_index].rab_Info.m.mbms_SessionIdentityPresent)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MBMS session Identity present. Not supported");
          tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
          return;
        }
        /* check for the unsupported PDCP info */
        
        rb_index =0;
        /* going through the RB info setup list */
        while (rab_list_ptr->elem[rab_index].rb_InformationSetupList.n > rb_index)
        {
          rb_info_ptr = &rab_list_ptr->elem[rab_index].rb_InformationSetupList.elem[rb_index];
          if (tdsrrc_find_rb_in_est_rabs(rb_info_ptr->rb_Identity) == TDSRRC_RB_PRESENT) 
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB id %d already exist in est rabs: Invalid config",rb_info_ptr->rb_Identity);
            tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
            return;
          }
  
          if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_ptr,
              tdsrrc_RB_InformationSetup_r6,pdcp_Info))
          {
  #ifdef FEATURE_UMTS_PDCP
            if (tdsrrc_pdcp_enabled)
            {
              /* If the RAB is for cs domain, send RBS failure with cause
                 *   invalid configuration
                 */
              if (rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == RRC_CS_DOMAIN_CN_ID)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info invalid for CS rab");
                tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                return;
              }
              /* check whether lossless SRNS relocation flag is set
               * and header compression algo is present in the message */
              if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
                  (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_info_ptr->pdcp_Info.losslessSRNS_RelocSupport,rrc_LosslessSRNS_RelocSupport_supported)))
              {
                if (rb_info_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_absent)
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP pdu header is absent.  Invalid config");
                  tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                  return;
                }
  
              } /* if rrc_PDCP_Info_r4_losslessSRNS_RelocSupport_present */
              if (rb_info_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present)
              {
                if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
                    (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_info_ptr->pdcp_Info.losslessSRNS_RelocSupport,
                    rrc_LosslessSRNS_RelocSupport_notSupported)))
                {
                  if (!(TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)))
                  {
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP header compress info list not present");
                    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                    return;
                  }
                }
              }
  
              if (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info, tdsrrc_PDCP_Info_r4,headerCompressionInfoList))
              {
                uint32 local_idx =0;
                while (rb_info_ptr->pdcp_Info.headerCompressionInfoList.n > local_idx)
                {
                  pdcpHeaderCompressionInfo = &rb_info_ptr->pdcp_Info.headerCompressionInfoList.elem[local_idx];
                  if (TDSRRC_CHECK_COMMON_MSG_TYPE(pdcpHeaderCompressionInfo->algorithmSpecificInfo,
                      rrc_AlgorithmSpecificInfo_r4_rfc2507_Info))
                  {
                    /* if the UE capability "Maximum header compression context space", is exceeded with this configuration send
                     * failure with cause "Invalid Config"
                     */
                    if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER > IPHC_MAX_HC_CONTEXT_SPACE)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  max header %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER);
                      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                      return;
                    }
                  }
                  else if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.t ==
                           T_tdsrrc_AlgorithmSpecificInfo_r4_rfc3095_Info)
                  {
                    /*  the chosen MAX_CID shall not be greater than the value "Maximum number of ROHC context sessions" as 
                     *  indicated in the IE "PDCP Capability"
                     */
                    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,ul_RFC3095))
                    {
                      if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
                      {
                        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID);
                        tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                        return;
                      }
                    }
  
                    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,dl_RFC3095))
                    {
                      if(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
                      {
                        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID);
                        tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                        return;
                      }
                    } 
                  }  /* if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.t == rrc_rfc3095_Info)*/ 
                  local_idx++;
                } 
  
              } /*  if (rb_info_ptr->pdcp_Info.bit_mask & rrc_PDCP_Info_r4_headerCompressionInfoList_present) */
            }
  #endif
          } /* if (rb_info_ptr->bit_mask & rrc_RB_InformationSetup_r6_pdcp_Info_present) */
  

          /* Reject if MBMS info is included */
          if ((rab_list_ptr->elem[rab_index].rab_Info.m.mbms_SessionIdentityPresent))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"MBMS session Identity present. Not supported");
            tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
            return;
          }

          if(new_tm_rb == FALSE)
          {
            dl_tm_flag = FALSE;
            if((TDSRRC_CHECK_COMMON_MSG_TYPE(rb_info_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_r6_rlc_Info)) &&
              (((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_ptr->rlc_InfoChoice.u.rlc_Info,
                 tdsrrc_RLC_Info_r6,ul_RLC_Mode))&&
                 (rb_info_ptr->rlc_InfoChoice.u.rlc_Info->ul_RLC_Mode.t == 
                 T_tdsrrc_UL_RLC_Mode_ul_TM_RLC_Mode))) &&
                 ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r6,dl_RLC_Mode))&&
                 (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_info_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode,
                 rrc_DL_RLC_Mode_r6_dl_TM_RLC_Mode))))
            {
              new_tm_rb = TRUE;
            }
            else if(TDSRRC_CHECK_COMMON_MSG_TYPE(rb_info_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_r6_same_as_RB))
            {
              local_lc_id = tdsrrclcm_get_dl_rlc_lc_id(rb_info_ptr->rlc_InfoChoice.u.same_as_RB);
              if(local_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
              {
                result = tdsrrclcm_get_dl_rlc_mode(local_lc_id, &rx_mode);
                if(result == TDSRRCLCM_SUCCESS && rx_mode == UE_MODE_TRANSPARENT)
                {
                  dl_tm_flag = TRUE;
                }
              }
    
              local_lc_id = tdsrrclcm_get_ul_rlc_lc_id(rb_info_ptr->rlc_InfoChoice.u.same_as_RB);
              if(local_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
              {
                result = tdsrrclcm_get_ul_rlc_mode(local_lc_id, &rx_mode);
                if(dl_tm_flag == TRUE && result == TDSRRCLCM_SUCCESS && rx_mode == UE_MODE_TRANSPARENT)
                {
                  new_tm_rb = TRUE;
                }
              }
            }
          }
          /*point to the next link */
          rb_index++;
        }
        /* Go to next RAB setup infomation  */
        rab_index++;
      } 
  
    }
    else
    {
      /* There is no RAB to be setup in this message.
        No further validation is needed */
      tdsrrc_rbe.rabs.num_rabs_in_msg = 0;
    }
    if(new_tm_rb == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"One or more TM RBs being setup");
    }
  

#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    if (rbsetup_ptr->specificationMode.u.complete.m.dl_CounterSynchronisationInfoPresent)
#else
    if (tdsrrcrbe_validate_srns_relocation_info(msg_ptr, TDSMSG_REL6))
#endif

    {
      /*check whether the SRNS relocation is within Cell_DCH or not.
        SRNS relocation information can come in reconfiguration messages
        only when the state transition is Cell_DCH->Cell_DCH */
      if (!((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH) &&
            ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbe.state_from_msg);
        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        return;
      }
     /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has
       PDCP info list. If it is true then this configuration is unsupported */
  
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rbsetup_ptr->specificationMode.u.complete.dl_CounterSynchronisationInfo,
           rb_WithPDCP_InfoList)) ||
          (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->specificationMode.u.complete.dl_CounterSynchronisationInfo,
           tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        return;
      }
      else
      {
        /* The procedure involves Lossy SRNS relocation.
        Set the flag that indicates that this procedure
        involves SRNS relocation */
        tdsrrc_rbe.rbe_for_srns_relocation = TRUE;
        /* store if new U-RNTI is present in the message */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
           tdsrrc_RadioBearerSetup_r6_IEs,new_U_RNTI))
        {
          tdsrrc_rbe.new_urnti_valid = TRUE;
          tdsrrc_translate_urnti (&(rbsetup_ptr->new_U_RNTI),
                               &tdsrrc_rbe.new_urnti);
        }
      }
    }
  
    /* Process RAB reconfig before RAB setup because we want to catch NW's which
       are trying to setup and reconfig RAB with the same message */
    if (rbsetup_ptr->specificationMode.u.complete.m.rab_InformationReconfigListPresent)
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RAB Info Reconfig in R6 RBS Msg");
  
      rab_reconfig_list_ptr = &rbsetup_ptr->specificationMode.u.complete.rab_InformationReconfigList;
  
      for (rab_index = 0;(rab_reconfig_list_ptr->n > rab_index) && (rab_index < TDSRRCRB_MAX_RAB_PER_MSG); rab_index++)
      {
        /* Check if the RAB id is GSM_MAP type. We don't support any
           other type */
        if (rab_reconfig_list_ptr->elem[rab_index].rab_Identity.t != T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RABid t%d is not GSM_MAP!",rab_reconfig_list_ptr->elem[rab_index].rab_Identity.t);
          tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
          return;
        }
  
        /* GSM_MAP RAB Id. Check if it's valid by calling the appropriate NAS entity. 
           First convert it to internal format. */
        rab_id = tdsrrcrb_translate_gsm_map_rab_id((rab_reconfig_list_ptr->elem[rab_index].rab_Identity.u.gsm_MAP_RAB_Identity));
  
        /* Check if the RAB id is already in ESTABLISHED_RABS */
        tdsrab_status = tdsrrc_find_rab_in_established_rabs(rab_id);
        if ( tdsrab_status != TDSRRC_RAB_FOUND )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB ID 0X%x not in ESTABLISHED_RABS",rab_id);
          tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
          return;
        }
  
        /* Store this info locally so that ESTABLISHED_RABS can
           be updated later, when these RABs are established */
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id_is_gsm_map = TRUE;
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id = rab_id;
  
        /* Check if this RAB is CS Domain or PS Domain */
        if ( rab_reconfig_list_ptr->elem[rab_index].cn_DomainIdentity  == tdsrrc_CN_DomainIdentity_cs_domain )
        {
          /* Store the domain identity. This will be later added to
          ESTABLISHED_RABS */
          tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain = RRC_CS_DOMAIN_CN_ID;
  
          /* Check if it is in loopback test */
          if ( tdsrrclbt_is_lb_on() == TRUE )
          {
            /* Set Rab type to Data */
            TDSRRC_MSG0(MSG_LEGACY_LOW,"Loopback Test");
            tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
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
                tdsrrc_get_rab_type_for_given_rab_id(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id));  

            switch ( rab_type )
            {
              case RRCMN_CS_DATA_CALL:
                tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
                break;
  
              case RRCMN_CS_VOICE_CALL:
                tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_VOICE_RAB;
                break;
  
              default:
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"CS RAB validation failed!");
                tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
                break;
  
            } /* end switch */
          }
  
          if ( (tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_INVALID_CONFIG) ||
               (tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG)
             )
          {
            /* No need to continue */
            return;
          }

#ifdef FEATURE_SCUDIF_ENHANCEMENT
          #error code not present
#else
          if(rab_type == RRCMN_CS_VOICE_CALL )
#endif/*FEATURE_SCUDIF_ENHANCEMENT*/
          {
            nas_sync_ind =   tdsrrc_translate_nas_sync_indicator(&(rab_reconfig_list_ptr->elem[rab_index].nas_Synchronisation_Indicator));
            if( nas_sync_ind == TDSCDMA_IVOCODER_ID_NONE )
            {
              tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
              return;
            }
            tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator = nas_sync_ind;             
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
            tdsrrc_set_ordered_codec_status(TRUE);
#endif
          }

        }
        /* Else check for PS Domain */
        else if ( rab_reconfig_list_ptr->elem[rab_index].cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain )
        {
          /* Store the domain identity. This will be later added to
             ESTABLISHED_RABS 
           */
          tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain = RRC_PS_DOMAIN_CN_ID;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"PS RABId %d to be setup",rab_id);
          tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_PS_DATA_RAB;
        }
       /* set the ordered codec mode which is sent in the ordered config to MAC.
        * If the RBE msg is not for Voice RAB also then also the current codec value
        * is passed to MAC 
        */
        tdsrrc_set_ordered_codec_mode(nas_sync_ind);
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator = nas_sync_ind;
  
        
      } // for each RAB info
      tdsrrc_rbe.rabs.num_rabs_in_msg = rab_index;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Total Reconfig RABs %d",tdsrrc_rbe.rabs.num_rabs_in_msg);  
    }

    if (rbsetup_ptr->specificationMode.u.complete.m.rab_InformationSetupListPresent)    
    {
      /* GSM_MAP RAB Id. Check if it's valid by calling the
      appropriate NAS entity. First convert it to internal
      format. */
      rab_list_ptr = &rbsetup_ptr->specificationMode.u.complete.rab_InformationSetupList;
    
  
  
      for (rab_index=tdsrrc_rbe.rabs.num_rabs_in_msg;(rab_list_ptr->n > rab_index) && (rab_index < TDSRRCRB_MAX_RAB_PER_MSG); rab_index++)
      {
        rab_id = tdsrrcrb_translate_gsm_map_rab_id((rab_list_ptr->elem[rab_index].rab_Info.
                                                  rab_Identity.u.gsm_MAP_RAB_Identity));
    
        /* Store this info locally so that ESTABLISHED_RABS can
        be updated later, when these RABs are established */
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id_is_gsm_map = TRUE;
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id = rab_id;
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].re_est_timer =
        rab_list_ptr->elem[rab_index].rab_Info.re_EstablishmentTimer;
        /* Check if this RAB is CS Domain or PS Domain */
        if ( rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == RRC_CS_DOMAIN_CN_ID )
        {
          /* Store the domain identity. This will be later added to
            ESTABLISHED_RABS */
          tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain = RRC_CS_DOMAIN_CN_ID;
    
          /* For CS RABs, Call MN layer to verify this RAB */
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
          rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
          rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif

          TDSRRC_MSG3(MSG_LEGACY_HIGH,"For RAB-Id %d, RAB type from NAS = %d, RAB type from EST_RABs = %d",rab_id, rab_type,
                tdsrrc_get_rab_type_for_given_rab_id(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id));  
    
          switch ( rab_type )
          {
            case RRCMN_CS_DATA_CALL:
    
              TDSRRC_MSG1(MSG_LEGACY_LOW,"Received a CS Data RAB Id %d",rab_id);
              break;
    
            case RRCMN_CS_VOICE_CALL:
    
              TDSRRC_MSG1(MSG_LEGACY_LOW,"Received a CS Voice RAB Id %d",rab_id);

              {
                if (tdsrrc_find_if_voice_rab_exists_in_established_rabs( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id )
                    == TRUE)
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Voice RAB Id - %d already exists",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id );
                  /*The condition of "if" covers the case when third RB is being added for AMR call*/
                  if(rab_list_ptr->elem[rab_index].rb_InformationSetupList.n == 1)
                  {
                    /* Most likely network is trying to add third RB to switch from lower AMR rate to higher */
                    voice_rab_already_exists = TRUE;
                  }
                  else
                  {
                    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid configuration .. more than one RAB subflow is being added");
                    return;
                  }
                }
                /*The given RAB isn't an existing Voice RAB.*/
                else
                {
                  /* Whenever a new voice RAB is to be set up, at least two RBs need to be associated with that RAB.
                  Therefore, first condition of "if" checks for presence of more than 1 RB. And the second condition
                  of "if" ensures that it's for CS over DCH.*/
                  if (rab_list_ptr->elem[rab_index].rb_InformationSetupList.n == 1)
                  {
#ifdef FEATURE_SCUDIF_ENHANCEMENT
                    #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/

                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Not enough RBs mapped to voice RAB!");
                    tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
                  }
                }
              }          
    
              break;
    
            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS RAB validation failed! RABid %d",rab_id);
              break;
    
          } /* end switch */
    
          if ( (tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_INVALID_CONFIG) ||
             (tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG))
          {
            /* No need to continue */
            return;
          }

#ifdef FEATURE_SCUDIF_ENHANCEMENT
          #error code not present
#else
          if(rab_type == RRCMN_CS_VOICE_CALL)
#endif/*FEATURE_SCUDIF_ENHANCEMENT*/
          {
            if(rab_list_ptr->elem[rab_index].rab_Info.m.nas_Synchronisation_IndicatorPresent)
            {
              nas_sync_ind =  tdsrrc_translate_nas_sync_indicator(
                                     &(rab_list_ptr->elem[rab_index].rab_Info.nas_Synchronisation_Indicator));
              if( nas_sync_ind == TDSCDMA_IVOCODER_ID_NONE )
              {
                tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported configuration .. codec not supported %d",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator );
                return;
              }          
            }
            tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator =nas_sync_ind;        
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
            tdsrrc_set_ordered_codec_status(TRUE);
#endif
          }

        }
        /* Else check for PS Domain */
        else if ( rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain )
        {
          /* Store the domain identity. This will be later added to
            ESTABLISHED_RABS */
          tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain = RRC_PS_DOMAIN_CN_ID;
    
          /* For PS Domain, if it's a loopback, then no need for query,
          otherwise, call RABM module to verify the RAB */
          if((tdsrrclbt_is_lb_on() == TRUE) ||
#ifdef FEATURE_DUAL_WCDMA
             (rabm_rabid_validate_per_sub(tdsrrc_get_as_id(), RAB_ESTABLISH_QUERY, (uint8)rab_id) == TRUE)
#else
             (rabm_rabid_validate(RAB_ESTABLISH_QUERY, (uint8)rab_id) == TRUE)
#endif
          )

          {
            TDSRRC_MSG1(MSG_LEGACY_LOW,"PS RAB Id %d to be setup",rab_id);
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"PS RAB validation failed! RABid %d",rab_id);
            tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
            return;
          }
        }
    
        if(voice_rab_already_exists == TRUE)
        {
          tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_VOICE_RAB;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Updated RAB type as voice");
        }
        else
        {
          /* ASN1 structure 'tdsrrc_RAB_InformationSetup' is validated at this point,
              hence update rab_type in structure 'tdsrrc_rbe.rabs.rabs_in_msg[rab_index]'. */
              /* change argument */
          (void) tdsrrc_update_rab_type_in_rab_info_r6(
             &(rbsetup_ptr->specificationMode.u.complete.rab_InformationSetupList.elem[rab_index]),
             &(tdsrrc_rbe.rabs.rabs_in_msg[rab_index]));
        }
    
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                 tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id,
                 tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain,
                 tdsrrc_rbe.rabs.rabs_in_msg[rab_index].re_est_timer);
        /* set the ordered codec mode which is sent in the ordered config to MAC.
          * If the RBE msg is not for Voice RAB also then also the current codec value
          * is passed to MAC 
          */
        tdsrrc_set_ordered_codec_mode(nas_sync_ind);
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator = nas_sync_ind;
    
        /* Go to next RAB info  */
       
      }
      if (rab_list_ptr->n > rab_index)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Too many RABs in an RBSetupMessage");
        tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        return;  
      }
    }
    tdsrrc_rbe.rabs.num_rabs_in_msg = rab_index;
  }

  /* Store the number of the RAB info in the RBSetup message. */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"%d RABs  in an RBSetupMessage",tdsrrc_rbe.rabs.num_rabs_in_msg);

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
  */
  tdsrrc_rbe.trans_to_dch_ciph_config_needed = FALSE;
  if((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH) && 
     (tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH)
    )
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
       tdsrrc_RadioBearerSetup_r6_IEs,dl_CommonInformation)) && 
       (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CommonInformation,tdsrrc_DL_CommonInformation_r6,
       dl_dpchInfoCommon)) &&
       (T_tdsrrc_DL_CommonInformation_r6_dl_dpchInfoCommon_dl_DPCH_InfoCommon == 
         rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.t))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 config activated");
        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon,tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
              &rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
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
        tdsrrc_rbe.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }
  else if((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)&&
      (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH))
  {
    tdsrrc_rbe.first_tm_rb_setup = tdsrrcrbe_is_first_tm_rb_setup(new_tm_rb);
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"First TM RB being set status : %d",tdsrrc_rbe.first_tm_rb_setup);
    if(tdsrrc_rbe.first_tm_rb_setup == TRUE)
    {
      //check to see if dl-dpch-common info is present while going to dch
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
         tdsrrc_RadioBearerSetup_r6_IEs,dl_CommonInformation)) && 
         (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CommonInformation,
         tdsrrc_DL_CommonInformation_r6,dl_dpchInfoCommon)) &&
         (T_tdsrrc_DL_CommonInformation_r6_dl_dpchInfoCommon_dl_DPCH_InfoCommon == 
          rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.t))
      {
        uecomdef_status_e_type loc_ret_val;
        //check if ciphering is active in mac for cs domain, if it is then,
        //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"First time TM RB Setup HHO Step-1 config activated");
        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon,tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
              &rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
              &mac_d_hfn_l);
        if (loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }
        if(tdsrrcsmc_process_cipher_config_for_first_tm_setup(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SMC process ciphering config for First TM RB Setup failed");
          /*This step is reached only when the first step returns a failure,. i.e, if the ciphering is not active for CS domain.
            Hence setting tdsrrc_rbe.first_tm_rb_setup to FALSE so that step 2 is not done*/
            tdsrrc_rbe.first_tm_rb_setup = FALSE;
        }

      }
    }
  }

  /* If UTRAN DRX coefficient is not present then it is invalid configuration */
  if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_rbe.state_from_msg  == TDSRRC_STATE_URA_PCH))
  {

    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
         tdsrrc_RadioBearerSetup_r6_IEs,utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_rbe.state_from_msg);
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  if((TDSRRC_CHECK_COMMON_MSG_TYPE(rbsetup_ptr->specificationMode,
    rrc_RadioBearerSetup_r6_IEs_specificationMode_complete))
      &&
      (rbsetup_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent))
  {
    /* Save the linked list head pointer */
    if (FAILURE == tdsrrcllc_validate_rb_info_reconfig_list_r6(
        &rbsetup_ptr->specificationMode.u.complete.rb_InformationReconfigList))
    {
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  if (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH)
  {
    /*should we do it after getting l2ack for response??*/
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
         tdsrrc_RadioBearerSetup_r6_IEs,ura_Identity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
      tdsrrccu_clear_ura_id ();
    }
    else
    {
      /*URA ID present in the message set ura_identity variable with this value */
      if (rbsetup_ptr->ura_Identity.numbits == 16)
      {
        tdsrrccu_update_ura_id (&(rbsetup_ptr->ura_Identity));
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rbsetup_ptr->ura_Identity.numbits);
        tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
        tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
        return;
      }
    }
  }


  return;

}/* end function tdsrrcrbe_validate_rbsetup_r6 */





/*===========================================================================

FUNCTION   RRCRBE_SEND_CHANNEL_CONFIG_REQ_R5

DESCRIPTION

  This function sends the TDSRRC_CHANNEL_CONFIG_REQ command to LLC
  to setup the necessary RBs associated with a RAB. If signalling
  Radio bearers are affected and included in the Radio Bearer
  Setup Message, they will also be sent in the same command.

  Currently the messege is not expected to have Signalling
  Radio Bearers for setup. If these are added in the future,
  they will also need to be sent in the same command.

DEPENDENCIES

  TDSORDERED_CONFIG must be set before calling this function.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_send_channel_config_req_r5
(
tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
)
{
  tdsrrc_cmd_type *out_cmd_ptr;  /* To hold the Command                       */
  uint32 rb_count=0;          /* To count the number of RBs in the command */
  uint8 rbe_rb_cnt = 0;

  uint32 rab_index=0;         /* Used when more than one RAB is received in a RBS message */
  uint32 srb_cnt=0;
  tdsrrc_RB_Identity rb_id=0;

  tdsrrc_RB_InformationSetupList_r5 *rb_ptr = NULL; /* Temporary pointer to RB info      */
  tdsrrc_RB_InformationAffectedList_r5*  rec_rb_ptr = NULL;  /* Pointer to RBs to be Reconfigured */

  tdsrrc_RAB_InformationSetupList_r5 *rab_info_ptr = NULL;
  tdsrrc_SRB_InformationSetupList_r5   *      srb_setup_ptr = NULL;
  tdsrrcrb_directed_cell_info_type        directed_cell_info; /* directed cell info struct */

  tdsrrc_RadioBearerSetup_r5_IEs* rbsetup_ptr = NULL;

  /* Fill in the command id  */
  tdsrrc_cmd.cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;

  /* Set RBSetup Ptr */
  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.radioBearerSetup_r5;


  /* First fill up the Radio Bearers that need to be setup.
  For this we need to know how many RBs need to be setup.
  If RABs need to be setup, check how many RBs are mapped
  to RABs. */

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r5_IEs,srb_InformationSetupList))
  {
    /* Get the SRB setup list */
    srb_setup_ptr = &rbsetup_ptr->srb_InformationSetupList;

    while ((srb_setup_ptr->n > srb_cnt) && (srb_cnt < UE_MAX_SRB_SETUP))
    {
      /* Check to see if the RB ID is there ... */
      if (srb_setup_ptr->elem[srb_cnt].m.rb_IdentityPresent)
      {
        rb_id = srb_setup_ptr->elem[srb_cnt].rb_Identity;
      }

      /*
      * ... if not, then start with the first SRB and get the next SRB ID
      * each time one is not included in the IE.
      */
      else
      {
        /* Get the next SRB, note this increments to 1 the first time */
        rb_id++;
      }

      /* Fill up the RB id and indicate that it needs to be established */
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rb_id;
      if(tdsrrclcm_get_dl_rlc_lc_id(rb_id) == TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
      {
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSESTABLISH_RB;
      }
      else
      {
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = 
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
      }

      rb_count++;
      srb_cnt++;


    } /* end SRB setup */

  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r5_IEs,rab_InformationSetupList))  
  {
    rab_info_ptr = &rbsetup_ptr->rab_InformationSetupList;
    for (rab_index=0; rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg && (rab_info_ptr->n > rab_index); rab_index++)
    {
      if ( tdsrrc_rbe.rabs.chan_config_needed[rab_index] == TRUE )
      {
        rb_ptr = &rab_info_ptr->elem[rab_index].rb_InformationSetupList;
  
        if ( rb_ptr->n == 0 )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No RBs present in msg!");
        } else
        {
          rbe_rb_cnt = 0;
          while ( rb_ptr->n > rbe_rb_cnt )
          {
            /* Fill up the RB id and indicate that it needs to be established */
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rb_ptr->elem[rbe_rb_cnt].rb_Identity;
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSESTABLISH_RB;
  
            /* Update the local structure so that we know which RB id's we've
            requested to be setup */
            tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rb_for_rab[rbe_rb_cnt].rb_id = rb_ptr->elem[rbe_rb_cnt].rb_Identity;
  
  
            /* Increment count of RBs */
            rb_count++;
            rbe_rb_cnt ++;
  
            /* Do a safety check on the count. We can't do more than 32
            configs in a command */
            if ( (rb_count >= TDS_MAX_RB) || (rbe_rb_cnt >= MAX_RB_PER_RAB))
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"MAX RB count exceeded!");
              break;
            }
  
            /* Go to the next RB */
          }/* end while */
          /* Save the number of RBs for this RAB */
          tdsrrc_rbe.rabs.rabs_in_msg[rab_index].num_rbs_for_rab = rbe_rb_cnt;
        }
      }/* end if rabs_setup_needed */
      
    }
  }

  /* Now check if the message contains an RB information to be
  affected list. If so, we need to reconfigure some RBs */
  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r5_IEs,rb_InformationAffectedList))
  {
  uint32 local_idx =0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB info affected list present");

    rec_rb_ptr = &rbsetup_ptr->rb_InformationAffectedList;

    if ( rec_rb_ptr->n == 0 )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No affected RBs in msg!");
    }
    else
    {
      while ( rec_rb_ptr->n > local_idx)
      {

        /* Fill up the RB id and indicate that it needs to be reconfigured */
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rec_rb_ptr->elem[local_idx].rb_Identity;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
        /* Increment count of RBs. This is done here since we
        need to use the next index that's not used already. */
        rb_count++;

        /* Go to the next affected RB */
       local_idx++;

        /* Do a safety check on the count. We can't do more than 32
        configs in a command */
        if ( rb_count >= TDS_MAX_RB )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MAX RB count exceeded!");
          break;
        }
      } /* end while */
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"No affected RB list in msg");
  }/* end if affected list present */

  /* Number of RBs to config. Note rb_count is the total
  of established and reconfigured RBs */
  tdsrrc_cmd.cmd.chan_config_req.num_rb_to_config = (int) rb_count;

  if(rb_count < TDS_MAX_RB)
  {
    tdsrrc_fill_rb_id_chan_req_from_standalone_db(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
  }

  if(tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)
  {
    /* Fill those RBs in chan_config_req for which RLC PDU size has changed and 
    that are already not present in chan_config_req*/
    if(rb_count < TDS_MAX_RB)
    {
    tdsrrc_fill_rb_id_chan_req_from_rb_list_mapped_to_dch(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
    }
  }

  /*If state transition is Cell_DCH->Cell_FACH or vice-verca, add all 
  the existing AM PS RBs in chan_config_req*/   
  if(((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_FACH) && (tdstransition_config.toc_usage == 
      TDSTOC_FOR_DCH_AND_FACH)) || ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && (tdsrrc_rbe.state_from_msg 
      == TDSRRC_STATE_CELL_DCH)))
  {
    tdsrrc_fill_rb_id_chan_req_from_est_rabs(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
  }

  /* Check if state change is required */
  if ( (tdsrrc_get_state()) == (tdsrrc_rbe.state_from_msg) )
  {
    tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required = FALSE;
  }
  else
  {
    /* The message indicates that state change is required.
    Indicate to LLC what the next state should be. */
    tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required = TRUE;
    tdsrrc_cmd.cmd.chan_config_req.next_state = tdsrrc_rbe.state_from_msg;
  }

  /* Indicate the id of the procedure sending the command */
  tdsrrc_cmd.cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RBE;

  /* Indicate that we need a confirmation for this command */
  tdsrrc_cmd.cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

  /* We are not required to fill any other data for this command.
   Now, send the command */
  if (((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
      (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
      (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
  {

    /*Initiate the cell selection*/
    tdsrrcrb_send_initiate_cell_selection_req(TDSRRC_PROCEDURE_RBE,
                                           TDSRRC_TRANSITION_FROM_DCH,
                                           tdsrrc_rbe.state_from_msg);

    tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

    /* Start T_305 [periodic cell update] timer */
    tdsrrccu_start_t_305_timer();

  } 
  else
  {
    /*check whether any frequency info is present in the message
    and transition is CELL_FACH->CELL_FACH. In this case
    initiate a cell update after channel config is successfully completed */
    if (((tdsrrc_get_state())==TDSRRC_STATE_CELL_FACH)&&
        (tdsrrc_rbe.state_from_msg==TDSRRC_STATE_CELL_FACH))
    {
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
           tdsrrc_RadioBearerSetup_r5_IEs,frequencyInfo)) &&
          (rbsetup_ptr->frequencyInfo.
           modeSpecificInfo.t == T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd))
      {
        directed_cell_info.freq_info_present = TRUE;
        directed_cell_info.uarfcn_dl = rbsetup_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
      }
      else
      {
        directed_cell_info.freq_info_present = FALSE;
      }
      if (tdsrrcrbe_is_pccpch_info_present(msg_ptr, TDSMSG_REL5))
      {
        directed_cell_info.cpid_present = TRUE;
        directed_cell_info.cell_param_id = rbsetup_ptr->dl_InformationPerRL_List.elem[0].
                                             modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
      }
      else
      {
        directed_cell_info.cpid_present = FALSE;
      }
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE)
      {
        /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection 
              (FACH->FACH ), treat it as un-supported configuration and send failure.*/ 
        if(tdsrrccsp_check_initiate_cell_selection_handling() == FALSE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
       
          tdsrrcrbe_send_initiate_cu_flag = TRUE;
          tdsrrcrbe_delete_crnti          = TRUE;
        }
        else
        {
          tdsrrc_int_c_rnti_type      c_rnti;       /* Local C_RNTI value */
          if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
                  tdsrrc_RadioBearerSetup_r5_IEs,new_C_RNTI))
          {
            tdsrrc_rbe.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI,
                                &c_rnti);
            tdsrrc_rbe.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_rbe.new_crnti_valid = FALSE;
          }
          tdsrrcllcoc_update_toc_with_oc_from_fach();
          
          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req(TDSRRC_PROCEDURE_RBE,
                                                  TDSRRC_TRANSITION_FROM_FACH,
                                                  tdsrrc_rbe.state_from_msg);
       
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
           
          return;
        }
      }

#else/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
      {
        /*directed cell info matched. Cell update is not required */
        tdsrrcrbe_send_initiate_cu_flag = FALSE;
      }
      else
      {
        /*directed cell info do not match. Cell update is required */
        tdsrrcrbe_send_initiate_cu_flag = TRUE;
        tdsrrcrbe_delete_crnti          = TRUE;
      }
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

    } /* if CELL_FACH->CELL_FACH reconfiguration */
    /* Get a Command buffer and copy the local command to the buffer
    * so as to enqueue it on the internal queue.
    */
    if ((out_cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL)
    {
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
      memscpy(out_cmd_ptr,sizeof(tdsrrc_cmd_int_type),&tdsrrc_cmd,sizeof(tdsrrc_cmd_int_type));
#else
      out_cmd_ptr->cmd = tdsrrc_cmd.cmd;
      out_cmd_ptr->cmd_hdr = tdsrrc_cmd.cmd_hdr;
#endif

      tdsrrc_put_int_cmd( out_cmd_ptr );
    }
    else
    {
      ERR_FATAL("Could not get internal cmd buf",0,0,0);
    }
  }

}/* end function tdsrrcrbe_send_channel_config_req_r5 */


/*===========================================================================

FUNCTION   RRCRBE_SEND_CHANNEL_CONFIG_REQ_R6

DESCRIPTION

  This function sends the TDSRRC_CHANNEL_CONFIG_REQ command to LLC
  to setup the necessary RBs associated with a RAB. If signalling
  Radio bearers are affected and included in the Radio Bearer
  Setup Message, they will also be sent in the same command.

  Currently the messege is not expected to have Signalling
  Radio Bearers for setup. If these are added in the future,
  they will also need to be sent in the same command.

DEPENDENCIES

  TDSORDERED_CONFIG must be set before calling this function.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_send_channel_config_req_r6
(
tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
)
{
  tdsrrc_cmd_type *out_cmd_ptr;  /* To hold the Command                       */
  uint32 rb_count=0;          /* To count the number of RBs in the command */
  uint8  rbe_rb_cnt = 0;

  uint32 rab_index=0;         /* Used when more than one RAB is received in a RBS message */
  uint32 srb_cnt=0;
  tdsrrc_RB_Identity rb_id=0;

  tdsrrc_RB_InformationSetupList_r6 *rb_ptr = NULL; /* Temporary pointer to RB info      */
  tdsrrc_RB_InformationAffectedList_r6 * affected_rb_ptr = NULL;  /* Pointer to RBs to be Reconfigured */
  tdsrrc_RB_InformationReconfigList_r6 * reconfig_rb_ptr = NULL;
  tdsrrc_RAB_InformationSetupList_r6 *rab_info_ptr = NULL;
  tdsrrc_SRB_InformationSetupList_r6       *  srb_setup_ptr = NULL;
  tdsrrcrb_directed_cell_info_type        directed_cell_info; /* directed cell info struct */

  tdsrrc_RadioBearerSetup_r6_IEs* rbsetup_ptr = NULL;


  /* Fill in the command id  */
  tdsrrc_cmd.cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;

  /* Set RBSetup Ptr */
  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
    criticalExtensions.u.r6.radioBearerSetup_r6;


  if (rbsetup_ptr->specificationMode.t ==
      T_tdsrrc_RadioBearerSetup_r6_IEs_specificationMode_complete)
  {
    /* First fill up the Radio Bearers that need to be setup.
    For this we need to know how many RBs need to be setup.
    If RABs need to be setup, check how many RBs are mapped
    to RABs. */
    if (rbsetup_ptr->specificationMode.u.complete.m.srb_InformationSetupListPresent)
    {
      /* Get the SRB setup list */
      srb_setup_ptr = &rbsetup_ptr->specificationMode.u.complete.srb_InformationSetupList;
  
      while ((srb_setup_ptr->n > srb_cnt) && (srb_cnt < UE_MAX_SRB_SETUP))
      {
        /* Check to see if the RB ID is there ... */
        if (srb_setup_ptr->elem[srb_cnt].m.rb_IdentityPresent)
        {
          rb_id = srb_setup_ptr->elem[srb_cnt].rb_Identity;
        }
  
        /*
        * ... if not, then start with the first SRB and get the next SRB ID
        * each time one is not included in the IE.
        */
        else
        {
          /* Get the next SRB, note this increments to 1 the first time */
          rb_id++;
        }
  
        /* Fill up the RB id and indicate that it needs to be established */
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rb_id;
        if(tdsrrclcm_get_dl_rlc_lc_id(rb_id) == TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSESTABLISH_RB;
        }
        else
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = 
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
        }
  
        rb_count++;
        srb_cnt++;
  

  
      } /* end SRB setup */
  
    }


    if (rbsetup_ptr->specificationMode.u.complete.m.rab_InformationSetupListPresent)    
    {
      rab_info_ptr = &rbsetup_ptr->specificationMode.u.complete.rab_InformationSetupList;

      for (rab_index=0; (rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg )&& (rab_info_ptr->n > rab_index); rab_index++)
      {
        if ( tdsrrc_rbe.rabs.chan_config_needed[rab_index] == TRUE )
        {
          rb_ptr = &rab_info_ptr->elem[rab_index].rb_InformationSetupList;
    
          if ( rb_ptr->n == 0 )
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"No RBs present in msg!");
          } else
          {
            rbe_rb_cnt = 0;
            while ( rb_ptr->n > rbe_rb_cnt )
            {
              /* Fill up the RB id and indicate that it needs to be established */
              tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rb_ptr->elem[rbe_rb_cnt].rb_Identity;
              tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSESTABLISH_RB;
    
              /* Update the local structure so that we know which RB id's we've
              requested to be setup */
              tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rb_for_rab[rbe_rb_cnt].rb_id = rb_ptr->elem[rbe_rb_cnt].rb_Identity;
    
  
              /* Increment count of RBs */
              rb_count++;
              rbe_rb_cnt ++;
    
              /* Do a safety check on the count. We can't do more than 32
              configs in a command */
              if ( (rb_count >= TDS_MAX_RB) || (rbe_rb_cnt >= MAX_RB_PER_RAB))
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"MAX RB count exceeded!");
                break;
              }
    
              /* Go to the next RB */
              
            }/* end while */
            /* Save the number of RBs for this RAB */
            tdsrrc_rbe.rabs.rabs_in_msg[rab_index].num_rbs_for_rab = rbe_rb_cnt;
          }
        }/* end if rabs_setup_needed */
      }
    }

    /* Now check if the message contains an RB information to be
       reconfig list. If so, we need to reconfigure some RBs */
    if ( rbsetup_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent)
    {
     uint32 local_idx=0;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB info reconfig list present");
  
      reconfig_rb_ptr = &rbsetup_ptr->specificationMode.u.complete.rb_InformationReconfigList;
  
      while ( reconfig_rb_ptr->n > local_idx )
      {
  
        /* Fill up the RB id and indicate that it needs to be reconfigured */
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = reconfig_rb_ptr->elem[local_idx].rb_Identity;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = 0;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = 0;
        if((reconfig_rb_ptr->elem[local_idx].m.rlc_InfoPresent)&&
           (reconfig_rb_ptr->elem[local_idx].rlc_Info.m.ul_RLC_ModePresent))
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = TDSMODIFY_RB_CFG;
        }
        if((reconfig_rb_ptr->elem[local_idx].m.rlc_InfoPresent)&&
           (reconfig_rb_ptr->elem[local_idx].rlc_Info.m.dl_RLC_ModePresent))
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
        }
        if (reconfig_rb_ptr->elem[local_idx].m.rb_StopContinuePresent )
        {
          /* over-ride rb_config with STOP or continue */
          if (tdsrrc_RB_StopContinue_stopRB == reconfig_rb_ptr->elem[local_idx].rb_StopContinue)
          {
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask |= TDSSTOP_RB_CFG;
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask |= TDSSTOP_RB_CFG;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"rb-reconfig-info rb %d to STOP rb_count %d",tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id,rb_count);
          }
          else if (tdsrrc_RB_StopContinue_continueRB == reconfig_rb_ptr->elem[local_idx].rb_StopContinue)
          {
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask |= TDSCONTINUE_RB_CFG;
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask |= TDSCONTINUE_RB_CFG;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"rb-reconfig-info rb %d to CONTINUE rb_count %d",tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id,rb_count);
            
          }
        }
  
        /* Increment count of RBs. This is done here since we
        need to use the next index that's not used already. */
        rb_count++;
  
        /* Go to the next affected RB */
        local_idx++;
  
        /* Do a safety check on the count. We can't do more than 32
        configs in a command */
        if ( rb_count >= TDS_MAX_RB )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MAX RB count exceeded!");
          break;
        }
      } /* end while */
    }
  
    /* Now check if the message contains an RB information to be
    affected list. If so, we need to reconfigure some RBs */
    if ( rbsetup_ptr->specificationMode.u.complete.m.rb_InformationAffectedListPresent)
    {
      uint32 local_idx=0;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB info affected list present");
  
      affected_rb_ptr = &rbsetup_ptr->specificationMode.u.complete.rb_InformationAffectedList;
  
      if ( affected_rb_ptr->n == 0 )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"No affected RBs in msg!");
      }
      else
      {
        while ( affected_rb_ptr->n > local_idx )
        {
  
          /* Fill up the RB id and indicate that it needs to be reconfigured */
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = affected_rb_ptr->elem[local_idx].rb_Identity;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
          /* Increment count of RBs. This is done here since we
          need to use the next index that's not used already. */
          rb_count++;
  
          /* Go to the next affected RB */
         local_idx++;
  
          /* Do a safety check on the count. We can't do more than 32
          configs in a command */
          if ( rb_count >= TDS_MAX_RB )
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"MAX RB count exceeded!");
            break;
          }
        } /* end while */
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"No affected RB list in msg");
    }/* end if affected list present */

  }
  else if( rbsetup_ptr->specificationMode.t == T_tdsrrc_RadioBearerSetup_r6_IEs_specificationMode_dummy)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"default config not expected in RB-Setup");
  }



  /* Number of RBs to config. Note rb_count is the total
  of established and reconfigured RBs */
  tdsrrc_cmd.cmd.chan_config_req.num_rb_to_config = (int) rb_count;

  if(rb_count < TDS_MAX_RB)
  {
    tdsrrc_fill_rb_id_chan_req_from_standalone_db(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
  }

  if(tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)
  {
     /* Fill those RBs in chan_config_req for which RLC PDU size has changed and 
     that are already not present in chan_config_req*/
    if(rb_count < TDS_MAX_RB)
    {
    tdsrrc_fill_rb_id_chan_req_from_rb_list_mapped_to_dch(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
    }
  }
 
  /*If state transition is Cell_DCH->Cell_FACH or vice-verca, add all the existing 
  AM PS RBs in chan_config_req*/    
  if(((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_FACH) && (tdstransition_config.toc_usage == 
      TDSTOC_FOR_DCH_AND_FACH)) || ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && (tdsrrc_rbe.state_from_msg 
      == TDSRRC_STATE_CELL_DCH)))
  {
    tdsrrc_fill_rb_id_chan_req_from_est_rabs(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
  }
  
  /* Check if state change is required */
  if ( (tdsrrc_get_state()) == (tdsrrc_rbe.state_from_msg) )
  {
    tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required = FALSE;
  }
  else
  {
    /* The message indicates that state change is required.
    Indicate to LLC what the next state should be. */
    tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required = TRUE;
    tdsrrc_cmd.cmd.chan_config_req.next_state = tdsrrc_rbe.state_from_msg;
  }

  /* Indicate the id of the procedure sending the command */
  tdsrrc_cmd.cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RBE;

  /* Indicate that we need a confirmation for this command */
  tdsrrc_cmd.cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

  /* We are not required to fill any other data for this command.
   Now, send the command */
  if (((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
      (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
      (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
  {

    /*Initiate the cell selection*/
    tdsrrcrb_send_initiate_cell_selection_req(TDSRRC_PROCEDURE_RBE,
                                           TDSRRC_TRANSITION_FROM_DCH,
                                           tdsrrc_rbe.state_from_msg);

    tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

    /* Start T_305 [periodic cell update] timer */
    tdsrrccu_start_t_305_timer();
  }
  else
  {
    /*check whether any frequency info is present in the message
    and transition is CELL_FACH->CELL_FACH. In this case
    initiate a cell update after channel config is successfully completed */
    if (((tdsrrc_get_state())==TDSRRC_STATE_CELL_FACH)&&
        (tdsrrc_rbe.state_from_msg==TDSRRC_STATE_CELL_FACH))
    {
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
          tdsrrc_RadioBearerSetup_r6_IEs,frequencyInfo)) &&
          (rbsetup_ptr->frequencyInfo.
           modeSpecificInfo.t == T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd))
      {
        directed_cell_info.freq_info_present = TRUE;
        directed_cell_info.uarfcn_dl = rbsetup_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
      }
      else
      {
        directed_cell_info.freq_info_present = FALSE;
      }
      if (tdsrrcrbe_is_pccpch_info_present(msg_ptr, TDSMSG_REL6))
      {
        directed_cell_info.cpid_present = TRUE;
        directed_cell_info.cell_param_id = rbsetup_ptr->dl_InformationPerRL_List.elem[0].
                                             modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
      }
      else
      {
        directed_cell_info.cpid_present = FALSE;
      }
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE)
      {
        /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection 
              (FACH->FACH ), treat it as un-supported configuration and send failure.*/ 
        if(tdsrrccsp_check_initiate_cell_selection_handling() == FALSE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
       
          tdsrrcrbe_send_initiate_cu_flag = TRUE;
          tdsrrcrbe_delete_crnti          = TRUE;
        }
        else
        {
          tdsrrc_int_c_rnti_type      c_rnti;       /* Local C_RNTI value */
          if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
                tdsrrc_RadioBearerSetup_r6_IEs,new_C_RNTI))
          {
            tdsrrc_rbe.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI,
                                &c_rnti);
            tdsrrc_rbe.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_rbe.new_crnti_valid = FALSE;
          }
          tdsrrcllcoc_update_toc_with_oc_from_fach();
          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req(TDSRRC_PROCEDURE_RBE,
                                                  TDSRRC_TRANSITION_FROM_FACH,
                                                  tdsrrc_rbe.state_from_msg);
       
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
                     
          return;
        }
      }
#else/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
      {
        /*directed cell info matched. Cell update is not required */
        tdsrrcrbe_send_initiate_cu_flag = FALSE;
      }
      else
      {
        /*directed cell info do not match. Cell update is required */
        tdsrrcrbe_send_initiate_cu_flag = TRUE;
        tdsrrcrbe_delete_crnti          = TRUE;
      }
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

    } /* if CELL_FACH->CELL_FACH reconfiguration */
    /* Get a Command buffer and copy the local command to the buffer
    * so as to enqueue it on the internal queue.
    */

    if ((out_cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL)
    {
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
      memscpy(out_cmd_ptr,sizeof(tdsrrc_cmd_int_type),&tdsrrc_cmd,sizeof(tdsrrc_cmd_int_type));
#else
      out_cmd_ptr->cmd = tdsrrc_cmd.cmd;
      out_cmd_ptr->cmd_hdr = tdsrrc_cmd.cmd_hdr;
#endif

      tdsrrc_put_int_cmd( out_cmd_ptr );
    }
    else
    {
      ERR_FATAL("Could not get internal cmd buf",0,0,0);
    }
  }
}/* end function tdsrrcrbe_send_channel_config_req_r6 */


/*===========================================================================

FUNCTION  RRCRBE_CLEAR_PROCEDURE

DESCRIPTION

  This function clears all the internal variables in
  the RBE procedure and brings them to their default
  values.

DEPENDENCIES

  The Substate of the procedure should be changed
  to initial substate before calling this function.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_clear_procedure( void )
{
  uint32 count;

  /* Initialize the transaction id's for this procedure.
  Use a number that can't be an actual received transaction
  id. */
  tdsrrc_rbe.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  tdsrrc_rbe.tr_ids.rejected_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;

  /* Clear the C-RNTI valid flag */
  tdsrrc_rbe.new_crnti_valid    = FALSE;
  tdsrrc_rbe.directed_cell_info = FALSE;

  tdsrrcrbe_send_initiate_cu_flag = FALSE;
  tdsrrcrbe_delete_crnti          = FALSE;

  tdsrrc_rbe.rbe_for_srns_relocation = FALSE;
  tdsrrc_rbe.new_urnti_valid = FALSE;

  /* free the memory allocate for mm_cmd_buf
  if it is not free or in case of unsuccessful events
  before sending this cmd to mm */
  if (tdsrrc_rbe.cn_info_saved != FALSE)
  {
    /* free the memory allocated for mm cmd */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Free MM cmd buffer %x",tdsrrc_rbe.mm_cmd_ptr);
    mm_free_cmd_buf(tdsrrc_rbe.mm_cmd_ptr);
    /* set cn_info_saved to false */
    tdsrrc_rbe.cn_info_saved = FALSE;
  }


  /* Initialize the RABS per message structure */
  for ( count=0; count < TDSRRCRB_MAX_RAB_PER_MSG; count++ )
  {
    tdsrrc_rbe.rabs.rabs_in_msg[count].num_rbs_for_rab = 0;
    tdsrrc_rbe.rabs.rabs_in_msg[count].rab_type= TDSRRCRB_UNKNOWN_RAB;
    tdsrrc_rbe.rabs.chan_config_needed[count] = FALSE;
    tdsrrc_rbe.rabs.rabs_in_msg[count].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_NONE;
  }
  tdsrrc_rbe.rabs.num_rabs_in_msg = 0;


  tdsrrc_set_ordered_codec_mode(tdsrrc_get_ordered_codec_mode());
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
  tdsrrc_set_ordered_codec_status(FALSE);
#endif

}
#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================
FUNCTION   tdsrrcrbe_prevalidate_rab_setup_list_r8

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static uecomdef_status_e_type tdsrrcrbe_prevalidate_rab_setup_list_r8
(
  tdsrrc_RAB_InformationSetupList_r8 *rab_list_ptr
)
{
  tdsrrc_RB_InformationSetup_r8 *rb_info_ptr = NULL;
  tdsrrc_RAB_InformationSetup_r8 * rab_info_ptr=NULL;
  uint32 rab_idx =0,rb_index=0;
  do  /*going through the RAB info setup list */
  {
    rab_info_ptr = &rab_list_ptr->elem[rab_idx];
    /*Reject if RAB Info Replace IE is present.*/
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rab_info_ptr,tdsrrc_RAB_InformationSetup_r8,rab_InfoReplace))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IE rab_InfoReplace not supported yet!");
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return FAILURE;  
    }

    /* Reject if MBMS info is included */
    if (TDSRRC_MSG_COMMON_BITMASK_IE(rab_info_ptr->rab_Info,tdsrrc_RAB_Info_r7,mbms_SessionIdentity))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IE mbms_SessionIdentity not supported yet!");
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return FAILURE;
    }

    /* Reject if MBMS info is included */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rab_info_ptr->rab_Info,mbms_ServiceIdentity))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IE mbms_ServiceIdentity not supported yet!");
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return FAILURE;
    }

    /*RRC_GKG: While supporting CSoHSPA, ensure to remove this check.*/
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rab_info_ptr,tdsrrc_RAB_InformationSetup_r8,cs_HSPA_Information))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IE cs_HSPA_Information not supported yet!");
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return FAILURE;  
    }

    /* Check if the RAB id is GSM_MAP type. We don't support any other type */
    if((rab_info_ptr->rab_Info.rab_Identity.t
        != T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:RAB ID t%d is not GSM_MAP!",rab_info_ptr->rab_Info.rab_Identity.t);
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return FAILURE;
    }

    /* check whether all RABs in the rabinfo list belong to same CN domain id or not
        otherwise this is an invalid configuration */
    if(rab_list_ptr->n > (rab_idx +1))
    {
      if(rab_info_ptr->rab_Info.cn_DomainIdentity !=
        rab_list_ptr->elem[rab_idx +1].rab_Info.cn_DomainIdentity)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"REL8:Different CN domains in RAB-list %d %d",rab_info_ptr->rab_Info.cn_DomainIdentity,rab_list_ptr->elem[rab_idx +1].rab_Info.cn_DomainIdentity);

        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        return FAILURE;
      }
    }

    /* check for the unsupported PDCP info */
    rb_index =0;
    do  /*going through the RB info setup list*/
    {
      
      rb_info_ptr = &rab_info_ptr->rb_InformationSetupList.elem[rb_index];
      if(tdsrrc_find_rb_in_est_rabs(rb_info_ptr->rb_Identity) == TDSRRC_RB_PRESENT) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:RB id %d already exist in est rabs: Invalid config",rb_info_ptr->rb_Identity);
        tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        return FAILURE;
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_ptr,tdsrrc_RB_InformationSetup_r8,pdcp_Info))
      {
#ifdef FEATURE_UMTS_PDCP
        if (tdsrrc_pdcp_enabled)
        {
          /*If the RAB is for cs domain, send RBS failure with cause invalid configuration*/
          if (rab_info_ptr->rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:PDCP info invalid for CS RAB");
            tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
            return FAILURE;
          }

          if(rab_info_ptr->rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
          {
            if(tdsrrcllc_validate_pdcp_info_in_rb_info_setup_list_r8(rb_info_ptr) == FALSE)
            {
              tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:PDCP Validation Failed!");
              return FAILURE;
            }
          }
        }
#endif /*FEATURE_UMTS_PDCP*/
      } 
      /*point to the next link */
      rb_index++;
    } while (rab_info_ptr->rb_InformationSetupList.n >  rb_index);

    /* Go to next RAB setup infomation  */
    rab_idx++;
  } while (rab_list_ptr->n > rab_idx);

  return SUCCESS;
}

/*===========================================================================
FUNCTION   tdsrrcrbe_validate_rab_setup_list_r8

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static uecomdef_status_e_type tdsrrcrbe_validate_rab_setup_list_r8
(
  tdsrrc_RAB_InformationSetupList_r8 * rab_list_ptr,
  uint32 no_of_rabs,
  uint32 nas_sync_ind
)
{
  rrc_rab_id_type rab_id;           /* To store RAB id in internal RRC format */
  
  /* To store the status returned by MN layer. This indicates what type of RAB is received. This is
  used only if the RAB is for the CS_domain */
  rrcmn_rab_status_T rab_type = RRCMN_INVALID_RAB_ID;     
  
  /* Used as an index when storing established RABs locally. Only when we have more than
  one RAB per message, this will be incremented */
  uint32 rab_index=0;               

  boolean voice_rab_already_exists = FALSE;
    
  for (rab_index= no_of_rabs; (rab_list_ptr->n > rab_index) && rab_index < TDSRRCRB_MAX_RAB_PER_MSG; rab_index++)
  {
    /* GSM_MAP RAB Id. First convert it to internal format. */      
    rab_id = tdsrrcrb_translate_gsm_map_rab_id((rab_list_ptr->elem[rab_index].rab_Info.
                                                  rab_Identity.u.gsm_MAP_RAB_Identity));
    
    /* Store this info locally so that ESTABLISHED_RABS can
    be updated later, when these RABs are established */
    tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id_is_gsm_map = TRUE;
    tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id = rab_id;
    tdsrrc_rbe.rabs.rabs_in_msg[rab_index].re_est_timer =
    rab_list_ptr->elem[rab_index].rab_Info.re_EstablishmentTimer;
    /* Check if this RAB is CS Domain or PS Domain */
    if ( rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
    {
      /* Store the domain identity. This will be later added to ESTABLISHED_RABS */
      tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain = RRC_CS_DOMAIN_CN_ID;
    
      /* For CS RABs, Call MN layer to verify this RAB */
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
      rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
      rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"For RAB-Id %d, RAB type from NAS = %d, RAB type from EST_RABs = %d",rab_id, rab_type,
        tdsrrc_get_rab_type_for_given_rab_id(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id));
    
      switch ( rab_type )
      {
        case RRCMN_CS_DATA_CALL:
    
          TDSRRC_MSG1(MSG_LEGACY_LOW,"Received a CS Data RAB Id %d",rab_id);
          break;
    
        case RRCMN_CS_VOICE_CALL:

          TDSRRC_MSG1(MSG_LEGACY_LOW,"Received a CS Voice RAB Id %d",rab_id);
          {
            if (tdsrrc_find_if_voice_rab_exists_in_established_rabs( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id)
                 == TRUE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:Voice RAB Id - %d already exists",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id );

              /*First condition of "if" covers the case when third RB is being added for AMR call*/
              /*Second condition of "if" covers the case when previously established Voice Call was of type CS over HSPA.*/
              if( rab_list_ptr->elem[rab_index].rb_InformationSetupList.n == 1)
              {
                voice_rab_already_exists = TRUE;
              }
              else
              {
#ifdef FEATURE_SCUDIF_ENHANCEMENT
                #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/              
                tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Invalid configuration .. more than one RAB subflow is being added");
                return FAILURE;
              }
            }
            /*The given RAB isn't an existing Voice RAB.*/
            else
            {
              /* Whenever a new voice RAB is to be set up, at least two RBs need to be associated with that RAB.
              Therefore, first condition of "if" checks for presence of more than 1 RB. And the second condition
              of "if" ensures that it's for CS over DCH.*/
              if ( rab_list_ptr->elem[rab_index].rb_InformationSetupList.n == 1)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Not enough RBs mapped to voice RAB!");
                tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
              }
            }
          }          
          break;
    
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:CS RAB validation failed! RABid %d",rab_id);
          break;
      } /* end switch */
    
      if ( (tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_INVALID_CONFIG) ||
         (tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG))
      {
        /* No need to continue */
        return FAILURE;
      }

#ifdef FEATURE_SCUDIF_ENHANCEMENT
      #error code not present
#else
      if(rab_type == RRCMN_CS_VOICE_CALL )
#endif/*FEATURE_SCUDIF_ENHANCEMENT*/
      {
        if(rab_list_ptr->elem[rab_index].rab_Info.m.nas_Synchronisation_IndicatorPresent)
        {
          nas_sync_ind =  tdsrrc_translate_nas_sync_indicator(
                                 &(rab_list_ptr->elem[rab_index].rab_Info.nas_Synchronisation_Indicator));
          if( nas_sync_ind == TDSCDMA_IVOCODER_ID_NONE )
          {
            tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:Unsupported configuration .. codec not supported %d",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator );
            return FAILURE;
          }          
        }
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator =nas_sync_ind;        
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
        tdsrrc_set_ordered_codec_status(TRUE);
#endif
      }
    }

    /* Else check for PS Domain */
    else if ( rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain )
    {
      /* Store the domain identity. This will be later added to ESTABLISHED_RABS */
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain = RRC_PS_DOMAIN_CN_ID;
    
      /* For PS Domain, if it's a loopback, then no need for query, otherwise, call RABM module to verify the RAB */
      if((tdsrrclbt_is_lb_on() == TRUE) ||
#ifdef FEATURE_DUAL_WCDMA
         (rabm_rabid_validate_per_sub(tdsrrc_get_as_id(), RAB_ESTABLISH_QUERY, (uint8)rab_id) == TRUE)
#else
         (rabm_rabid_validate(RAB_ESTABLISH_QUERY, (uint8)rab_id) == TRUE)
#endif
      )
      {
        TDSRRC_MSG1(MSG_LEGACY_LOW,"PS RAB Id %d to be setup",rab_id);
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8:PS RAB validation failed! RABid %d",rab_id);
        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        return FAILURE;
      }
    }
    
    if(voice_rab_already_exists == TRUE)
    {
      tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_VOICE_RAB;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Updated RAB type as voice");
    }
    else
    {
      /* ASN1 structure 'tdsrrc_RAB_InformationSetup' is validated at this point,
          hence update rab_type in structure 'tdsrrc_rbe.rabs.rabs_in_msg[rab_index]'. */
          /* change argument */
      (void) tdsrrc_update_rab_type_in_rab_info_r8(&(rab_list_ptr->elem[rab_index]),
         &(tdsrrc_rbe.rabs.rabs_in_msg[rab_index]));
    }
    
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"REL8:RAB_ID:%d,Domain:%d,re_est_tmr:%d",
             tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id,
             tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain,
             tdsrrc_rbe.rabs.rabs_in_msg[rab_index].re_est_timer);
    /* set the ordered codec mode which is sent in the ordered config to MAC.
      * If the RBE msg is not for Voice RAB also then also the current codec value
      * is passed to MAC 
      */
    tdsrrc_set_ordered_codec_mode(nas_sync_ind);
    tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator = nas_sync_ind;
    
    /* Go to next RAB info  */
  }
  if (rab_list_ptr->n > rab_index)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Too many RABs in an RBSetupMessage");
    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;

    return FAILURE;
  }
  
  tdsrrc_rbe.rabs.num_rabs_in_msg = rab_index;
  return SUCCESS;
}

/*===========================================================================
FUNCTION   RRCRBE_SEND_CHANNEL_CONFIG_REQ_R8

DESCRIPTION
  This function sends the TDSRRC_CHANNEL_CONFIG_REQ command to LLC
  to setup the necessary RBs associated with a RAB. If signalling
  Radio bearers are affected and included in the Radio Bearer
  Setup Message, they will also be sent in the same command.

  Currently the messege is not expected to have Signalling
  Radio Bearers for setup. If these are added in the future,
  they will also need to be sent in the same command.

DEPENDENCIES
  TDSORDERED_CONFIG must be set before calling this function.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbe_send_channel_config_req_r8
(
  tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
)
{
  tdsrrc_cmd_type *out_cmd_ptr;  /* To hold the Command                       */
  uint32 rb_count=0;          /* To count the number of RBs in the command */
  uint8  rbe_rb_cnt = 0;

  uint32 rab_index=0;         /* Used when more than one RAB is received in a RBS message */
  uint32 srb_cnt=0;
  tdsrrc_RB_Identity rb_id=0;

  tdsrrc_RB_InformationSetupList_r8 *rb_ptr = NULL; /* Temporary pointer to RB info      */
  tdsrrc_RB_InformationAffectedList_r8 *affected_rb_ptr = NULL;  /* Pointer to RBs to be Reconfigured */
  tdsrrc_RB_InformationReconfigList_r8 *reconfig_rb_ptr = NULL;
  tdsrrc_RAB_InformationSetupList_r8 *rab_info_ptr = NULL;
  tdsrrc_SRB_InformationSetupList_r8        * srb_setup_ptr = NULL;
  tdsrrcrb_directed_cell_info_type        directed_cell_info; /* directed cell info struct */

  tdsrrc_RadioBearerSetup_r8_IEs* rbsetup_ptr = NULL;

  /* Fill in the command id  */
  tdsrrc_cmd.cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;

  /* Set RBSetup Ptr */
  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
    criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerSetup_r8;


  /* First fill up the Radio Bearers that need to be setup. For this we need to know how many RBs need to be setup.
  If RABs need to be setup, check how many RBs are mapped to RABs. */
  if(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,srb_InformationSetupList))
  {
    /* Get the SRB setup list */
    srb_setup_ptr = &rbsetup_ptr->srb_InformationSetupList;

    while ((srb_setup_ptr->n > srb_cnt) && (srb_cnt < UE_MAX_SRB_SETUP))
    {
      /* Check to see if the RB ID is there ... */
      if (srb_setup_ptr->elem[srb_cnt].m.rb_IdentityPresent)
      {
        rb_id = srb_setup_ptr->elem[srb_cnt].rb_Identity;
      }
      /*If not, then start with the first SRB and get the next SRB ID each time one is not included in the IE.*/
      else
      {
        /* Get the next SRB, note this increments to 1 the first time */
        rb_id++;
      }
  
      /* Fill up the RB id and indicate that it needs to be established */
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rb_id;
      if(tdsrrclcm_get_dl_rlc_lc_id(rb_id) == TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
      {
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSESTABLISH_RB;
      }
      else
      {
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = 
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
      }
      rb_count++;
      srb_cnt++;


    } /* end SRB setup */
  }

  if(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,rab_InformationSetupList))
  {
    rab_info_ptr = &rbsetup_ptr->rab_InformationSetupList;
    for (rab_index=0; rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg && rab_info_ptr->n > rab_index; rab_index++)
    {
      if ( tdsrrc_rbe.rabs.chan_config_needed[rab_index] == TRUE)
      {
        rb_ptr = &rab_info_ptr->elem[rab_index].rb_InformationSetupList;

        if ( rb_ptr->n == 0 )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:No RBs present in msg!");
        }
        else
        {
          rbe_rb_cnt = 0;
          while ( rb_ptr->n > rbe_rb_cnt )
          {
            /* Fill up the RB id and indicate that it needs to be established */
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rb_ptr->elem[rbe_rb_cnt].rb_Identity;
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSESTABLISH_RB;
  
            /* Update the local structure so that we know which RB id's we've
                    requested to be setup */
            tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rb_for_rab[rbe_rb_cnt].rb_id = rb_ptr->elem[rbe_rb_cnt].rb_Identity;
  
            /* Increment count of RBs */
            rb_count++;
            rbe_rb_cnt ++;
  
            /* Do a safety check on the count. We can't do more than 32
                     configs in a command */
            if ( rb_count >= TDS_MAX_RB )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:MAX RB count exceeded!");
              break;
            }
            if ( rbe_rb_cnt >= MAX_RB_PER_RAB )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:MAX RBs per RAB count exceeded!");
              break;
            }
  
            /* Go to the next RB */
          }/* end while */
            /* Save the number of RBs for this RAB */
          tdsrrc_rbe.rabs.rabs_in_msg[rab_index].num_rbs_for_rab = rbe_rb_cnt;
        }
      }/* end if rabs_setup_needed */
    }
  }
  /* Now check if the message contains an RB information to be
     reconfig list. If so, we need to reconfigure some RBs */
  if(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,rb_InformationReconfigList))
  {
    uint32 local_idx=0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:RB info reconfig list present");
  
    reconfig_rb_ptr = &rbsetup_ptr->rb_InformationReconfigList;
  
    while ( reconfig_rb_ptr->n > local_idx )
    {
  
      /* Fill up the RB id and indicate that it needs to be reconfigured */
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = reconfig_rb_ptr->elem[local_idx].rb_Identity;
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = 0;
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = 0;
      if((reconfig_rb_ptr->elem[local_idx].m.rlc_InfoPresent)&&
         (reconfig_rb_ptr->elem[local_idx].rlc_Info.m.ul_RLC_ModePresent))
      {
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = TDSMODIFY_RB_CFG;
      }
      if((reconfig_rb_ptr->elem[local_idx].m.rlc_InfoPresent)&&
         (reconfig_rb_ptr->elem[local_idx].rlc_Info.m.dl_RLC_ModePresent))
      {
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
      }
  
      if (reconfig_rb_ptr->elem[local_idx].m.rb_StopContinuePresent)
      {
        /* over-ride rb_config with STOP or continue */
        if (tdsrrc_RB_StopContinue_stopRB == reconfig_rb_ptr->elem[local_idx].rb_StopContinue)
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask |= TDSSTOP_RB_CFG;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask |= TDSSTOP_RB_CFG;
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL8:rb-reconfig-info rb %d to STOP rb_count %d",tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id,rb_count);
        }
        else if (tdsrrc_RB_StopContinue_continueRB == reconfig_rb_ptr->elem[local_idx].rb_StopContinue)
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask |= TDSCONTINUE_RB_CFG;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask |= TDSCONTINUE_RB_CFG;
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL8:rb-reconfig-info rb %d to CONTINUE rb_count %d",tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id,rb_count);
          
        }
      }
  
      /* Increment count of RBs. This is done here since we
      need to use the next index that's not used already. */
      rb_count++;
  
      /* Go to the next affected RB */
      local_idx++;
  
      /* Do a safety check on the count. We can't do more than 32
      configs in a command */
      if ( rb_count >= TDS_MAX_RB )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:MAX RB count exceeded!");
        break;
      }
    } /* end while */
  }
  
  /* Now check if the message contains an RB information to be
   affected list. If so, we need to reconfigure some RBs */
  if(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,rb_InformationAffectedList))
  {
   uint32 local_idx=0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:RB info affected list present");
  
    affected_rb_ptr = &rbsetup_ptr->rb_InformationAffectedList;
  
    if ( affected_rb_ptr->n == 0 )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:No affected RBs in msg!");
    }
    else
    {
      while ( affected_rb_ptr->n > local_idx )
      {
  
        /* Fill up the RB id and indicate that it needs to be reconfigured */
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = affected_rb_ptr->elem[local_idx].rb_Identity;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
  
        /* Increment count of RBs. This is done here since we
        need to use the next index that's not used already. */
        rb_count++;
  
        /* Go to the next affected RB */
        local_idx++;
  
        /* Do a safety check on the count. We can't do more than 32
        configs in a command */
        if ( rb_count >= TDS_MAX_RB )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:MAX RB count exceeded!");
          break;
        }
      } /* end while */
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"No affected RB list in msg");
  }/* end if affected list present */


  /* Number of RBs to config. Note rb_count is the total
   of established and reconfigured RBs */
  tdsrrc_cmd.cmd.chan_config_req.num_rb_to_config = (int) rb_count;

  if(rb_count < TDS_MAX_RB)
  {
  tdsrrc_fill_rb_id_chan_req_from_standalone_db(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
  }

  if(tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)
  {
     /* Fill those RBs in chan_config_req for which RLC PDU size has changed and 
     that are already not present in chan_config_req*/
    if(rb_count < TDS_MAX_RB)
    {
    tdsrrc_fill_rb_id_chan_req_from_rb_list_mapped_to_dch(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
    }
  }
 
  /*If state transition is Cell_DCH->Cell_FACH or vice-verca, add all the existing 
  AM PS RBs in chan_config_req*/    
  if(((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_FACH) && (tdstransition_config.toc_usage == 
      TDSTOC_FOR_DCH_AND_FACH)) || ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && (tdsrrc_rbe.state_from_msg 
      == TDSRRC_STATE_CELL_DCH)))
  {
    tdsrrc_fill_rb_id_chan_req_from_est_rabs(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
  }
  /* Check if state change is required */
  if ( (tdsrrc_get_state()) == (tdsrrc_rbe.state_from_msg) )
  {
    tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required = FALSE;
  }
  else
  {
    /* The message indicates that state change is required.
    Indicate to LLC what the next state should be. */
    tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required = TRUE;
    tdsrrc_cmd.cmd.chan_config_req.next_state = tdsrrc_rbe.state_from_msg;
  }

  /* Indicate the id of the procedure sending the command */
  tdsrrc_cmd.cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RBE;

  /* Indicate that we need a confirmation for this command */
  tdsrrc_cmd.cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

  /* We are not required to fill any other data for this command.
   Now, send the command */
  if (((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
      (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
      (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
  {
    /*Initiate the cell selection*/
    tdsrrcrb_send_initiate_cell_selection_req(TDSRRC_PROCEDURE_RBE,
            TDSRRC_TRANSITION_FROM_DCH, tdsrrc_rbe.state_from_msg);

    tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

    /* Start T_305 [periodic cell update] timer */
    tdsrrccu_start_t_305_timer();
  }
  else
  {
    /*check whether any frequency info is present in the message
    and transition is CELL_FACH->CELL_FACH. In this case
    initiate a cell update after channel config is successfully completed */
    if (((tdsrrc_get_state())==TDSRRC_STATE_CELL_FACH)&&
        (tdsrrc_rbe.state_from_msg==TDSRRC_STATE_CELL_FACH))
    {
      if((TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,frequencyInfo)) &&
          (rbsetup_ptr->frequencyInfo.modeSpecificInfo.t == T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd))
      {
        directed_cell_info.freq_info_present = TRUE;
        directed_cell_info.uarfcn_dl = rbsetup_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
      }
      else
      {
        directed_cell_info.freq_info_present = FALSE;
      }
      if (tdsrrcrbe_is_pccpch_info_present(msg_ptr, TDSMSG_REL8))
      {
        directed_cell_info.cpid_present = TRUE;
        directed_cell_info.cell_param_id = rbsetup_ptr->dl_InformationPerRL_List.elem[0].
                                             modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
      }
      else
      {
        directed_cell_info.cpid_present = FALSE;
      }
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE)
      {
        /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection 
          FACH->FACH ), treat it as un-supported configuration and send failure.*/ 
        if(tdsrrccsp_check_initiate_cell_selection_handling() == FALSE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
       
          tdsrrcrbe_send_initiate_cu_flag = TRUE;
          tdsrrcrbe_delete_crnti          = TRUE;
        }
        else
        {
          tdsrrc_int_c_rnti_type      c_rnti;       /* Local C_RNTI value */
          if(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,new_C_RNTI))
          {
            tdsrrc_rbe.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI,
                                &c_rnti);
            tdsrrc_rbe.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_rbe.new_crnti_valid = FALSE;
          }
          tdsrrcllcoc_update_toc_with_oc_from_fach();
          
          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req(TDSRRC_PROCEDURE_RBE,
                                                  TDSRRC_TRANSITION_FROM_FACH,
                                                  tdsrrc_rbe.state_from_msg);
       
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
              
          return;
        }
      }

#else/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if (tdsrrcrb_validate_directed_cell_info(directed_cell_info))
      {
        /*directed cell info matched. Cell update is not required */
        tdsrrcrbe_send_initiate_cu_flag = FALSE;
      }
      else
      {
        /*directed cell info do not match. Cell update is required */
        tdsrrcrbe_send_initiate_cu_flag = TRUE;
        tdsrrcrbe_delete_crnti          = TRUE;
      }
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
    } /* if CELL_FACH->CELL_FACH reconfiguration */

    /* Get a Command buffer and copy the local command to the buffer so as to 
    enqueue it on the internal queue. */
    if ((out_cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL)
    {
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
      memscpy(out_cmd_ptr,sizeof(tdsrrc_cmd_int_type),&tdsrrc_cmd,sizeof(tdsrrc_cmd_int_type));
#else
      out_cmd_ptr->cmd_hdr = tdsrrc_cmd.cmd_hdr;
      out_cmd_ptr->cmd     = tdsrrc_cmd.cmd;
#endif
      tdsrrc_put_int_cmd( out_cmd_ptr );
    }
    else
    {
      ERR_FATAL("Could not get internal cmd buf",0,0,0);
    }
  }
}

/*===========================================================================
FUNCTION   VALIDATE_RBSETUP_R8

DESCRIPTION
  This function validates the received R8 RB Setup Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Setup Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbe_validate_rbsetup_r8
(
  tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
)
{
  tdsrrc_RadioBearerSetup_r8_IEs* rbsetup_ptr = NULL;

  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();

  /* Assume the message is valid for now */
  tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_VALID;

  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
    u.criticalExtensions.u.criticalExtensions.u.r8.radioBearerSetup_r8;

  switch (rbsetup_ptr->rrc_StateIndicator)
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbe.state_from_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbe.state_from_msg = TDSRRC_STATE_CELL_FACH;
      break;

    default: /* Just to satisfy LINT */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:Invalid RRC state from Msg");
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;

  } /* End of switch */

  if ( tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
    return;
  }

  /* Check if there are new RABs to be established */
  if(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,rab_InformationSetupList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:RAB Info Setup List present.");
    if(tdsrrcrbe_prevalidate_rab_setup_list_r8(&rbsetup_ptr->rab_InformationSetupList) == FAILURE)
    {
      return;
    }
  }
  else
  {
    /*There is no RAB to be setup in this message.*/
    tdsrrc_rbe.rabs.num_rabs_in_msg = 0;
  }  

#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if(rbsetup_ptr->m.dl_CounterSynchronisationInfoPresent)
#else
    if (tdsrrcrbe_validate_srns_relocation_info(msg_ptr, TDSMSG_REL8))
#endif

  {
    /*check whether the SRNS relocation is within Cell_DCH or not.
      SRNS relocation information can come in reconfiguration messages
      only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH) &&
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"REL8:SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbe.state_from_msg);
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }

    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has
       PDCP info list. If it is true then this configuration is unsupported */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rbsetup_ptr->dl_CounterSynchronisationInfo,
        rb_WithPDCP_InfoList)) ||
        (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CounterSynchronisationInfo, 
        tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:PDCP info in DL counter sync info not supported");
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation.
      Set the flag that indicates that this procedure
      involves SRNS relocation */
      tdsrrc_rbe.rbe_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */
      if (TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,new_U_RNTI))
      {
        tdsrrc_rbe.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(rbsetup_ptr->new_U_RNTI), &tdsrrc_rbe.new_urnti);
      }
    }
  }

  /* Process RAB reconfig before RAB setup because we want to catch NW's which
    are trying to setup and reconfig RAB with the same message */
  if(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,rab_InformationReconfigList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:RAB Info Reconfig List is present.");

    tdsrrc_rbe.status.failure_status = tdsrrc_update_rab_info_reconfig_param_r8(
       &rbsetup_ptr->rab_InformationReconfigList, &tdsrrc_rbe.rabs, &nas_sync_ind);
    if( tdsrrc_rbe.status.failure_status != TDSRRCRB_MSG_VALID) 
    {
      return;
    }
  }

  if(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,rab_InformationSetupList))
  {
    if(tdsrrcrbe_validate_rab_setup_list_r8(&rbsetup_ptr->rab_InformationSetupList, 
        tdsrrc_rbe.rabs.num_rabs_in_msg, nas_sync_ind) == FAILURE)
    {
      return;
    }
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:%d RABs  in an RBSetup",tdsrrc_rbe.rabs.num_rabs_in_msg);

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch */
  tdsrrc_rbe.trans_to_dch_ciph_config_needed = FALSE;

  if((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH) && (tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,dl_CommonInformation)) && 
       (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CommonInformation,
       tdsrrc_DL_CommonInformation_r8,dl_dpchInfoCommon)) &&
       (T_tdsrrc_DL_CommonInformation_r8_dl_dpchInfoCommon_dl_DPCH_InfoCommon
       == rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.t))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:To-DCH-Step-1 config activated");
        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon,tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
              &rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
              &mac_d_hfn_l);
        if (loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:get_mac_d_hfn failed");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:SMC process ciphering config for HHO failed");
        }
        tdsrrc_rbe.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:To-DCH-Step-1 not needed");
      }
    }
  }

  /* If UTRAN DRX coefficient is not present then it is invalid configuration */
  if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_rbe.state_from_msg  == TDSRRC_STATE_URA_PCH))
  {

    if (!(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8:UTRAN DRX Coef not preset,State:%d",tdsrrc_rbe.state_from_msg);
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  if(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,rb_InformationReconfigList))
  {
    /* Save the linked list head pointer */
    if (FAILURE == tdsrrcllc_validate_rb_info_reconfig_list_r8(&rbsetup_ptr->rb_InformationReconfigList))
    {
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  if (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH)
  {
    /*should we do it after getting l2ack for response??*/
    if (!(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,ura_Identity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:URA ID not present");
      tdsrrccu_clear_ura_id ();
    }
    else
    {
      /*URA ID present in the message set ura_identity variable with this value */
      if (rbsetup_ptr->ura_Identity.numbits == 16)
      {
        tdsrrccu_update_ura_id (&(rbsetup_ptr->ura_Identity));
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8:Invalid URA ID length%d",rbsetup_ptr->ura_Identity.numbits);
        tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
        tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
        return;
      }
    }
  }

  return;
}

/*===========================================================================
FUNCTION   PROCESS_RBSETUP_R8

DESCRIPTION
  This function processes the Rel 8 RB Setup Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Setup Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -

    TDSORDERED_CONFIG will be set by calling LLC.

    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.

    If TDSORDERED_CONFIG could not be set, a Radio Bearer Setup Failure message
    will be sent to UTRAN.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbe_process_rbsetup_r8
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_DL_DCCH_Message     *dcch_msg_ptr;   /* Pointer to the RB Setup message */
  tdsrrc_RadioBearerSetup *msg_ptr;    /* Pointer to the RB Setup message        */

  tdsrrc_rab_search_e_type tdsrab_status = TDSRRC_RAB_NOT_FOUND; /* To check if RAB is present in ESTABLISHED_RABS */
  tdsrrcllc_oc_status_e_type oc_status;/* To hold tdsordered_config status          */
  uint32 rab_index=0;               /* Used when more than 1 rab is present in
                                    a RBS message                             */
  tdsrrc_int_c_rnti_type      c_rnti;       /* Local C_RNTI value */
  boolean ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                                 occured while setting tdsordered_config. */
  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL8;

  tdsrrc_RadioBearerSetup_r8_IEs* rbsetup_ptr = NULL;

  tdsrrcsmc_srns_ip_check_status_e_type    ip_check_status = TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;
  tdsrrcrb_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;


  TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:RRC cur_substate %d",tdsrrc_rbe.curr_substate);

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Setup message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerSetup);

  /* Store Msg Ptr in a local variable */
  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
    u.r8.radioBearerSetup_r8;

  if (TRUE == tdsrrcrbe_validate_srns_relocation_info(msg_ptr, msg_version))
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
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:IP check failed. Discarding RBS msg");
      break;

    case TDSRRCSMC_SRNS_SECURITY_INFO_INVALID:
    case TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION:
      if (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier
          == tdsrrc_rbe.tr_ids.rejected_transaction)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:Duplicate RBE msg - rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
        /* Nothing else to do, just ignore the message and return */
      }
      else
      {
        /* send a reconfig failure messge with invalid
        configuration and
          continue the same substate with any ongoing procedure */
        temp_rejected_transaction = tdsrrc_rbe.tr_ids.rejected_transaction;
        temp_failure_cause.failure_status = tdsrrc_rbe.status.failure_status;
        
        tdsrrc_rbe.tr_ids.rejected_transaction =
        (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
        if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
        {
          tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        }
        else
        {
          tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
        }
        /*Check whether Cell Update is going on */
        if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
             (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
        {
          if(tdsrrc_rbe.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbe.curr_substate,tdsrrc_rbe.status.failure_status);
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                     tdsrrc_rbe.status.prot_err_cause );
            tdsrrc_rbe.tr_ids.rejected_transaction = temp_rejected_transaction;
            tdsrrc_rbe.status.failure_status = temp_failure_cause.failure_status;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Cell Update Started! Register for CU CNF");
            (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
            /* Wait until Cell update procedure is completed */
            tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
          }
        }
        else
        {
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause );
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
      /*do not clear the procedure or change the tdsrrc_rbe substate because
      some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
      return;
    }

  } /* End of validation of srns relocation */

  if ((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
      (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
      (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Incomatible simultaneous reconfig! Rejecting RBS message");
    if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
         == tdsrrc_rbe.tr_ids.accepted_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:Duplicate RBS msg! accepted tr-id%d match!",tdsrrc_rbe.tr_ids.accepted_transaction);
      /* Nothing else to do, just return */
    }
    else if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                == tdsrrc_rbe.tr_ids.rejected_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:Duplicate RBS msg! rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
      /* Nothing else to do, just return */
    }
    else
    {
      /* We have received a RBS message with a new transaction id.
      This message should be in our rejected transaction
        list */
      temp_rejected_transaction = tdsrrc_rbe.tr_ids.rejected_transaction;
      temp_failure_cause.failure_status = tdsrrc_rbe.status.failure_status;
      tdsrrc_rbe.tr_ids.rejected_transaction =
      (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
      tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;

      /*Check whether Cell Update is going on */
      if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
           (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
      {
        if(tdsrrc_rbe.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbe.curr_substate,tdsrrc_rbe.status.failure_status);
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                   tdsrrc_rbe.status.prot_err_cause );
          tdsrrc_rbe.tr_ids.rejected_transaction = temp_rejected_transaction;
          tdsrrc_rbe.status.failure_status = temp_failure_cause.failure_status;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Cell Update Started! Register for CU CNF");
          (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
          /* Wait until Cell update procedure is completed */
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        }
      }
      else
      {
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                  tdsrrc_rbe.status.prot_err_cause );
        /*continue the same substate */
      }
    }
    return;
  }

  /* We have received a RB setup message with a new transaction id.
  Accept the transaction Id */
  tdsrrc_rbe.tr_ids.accepted_transaction =
  msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;


#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rbsetup_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rbsetup_ptr->m.activationTimePresent,
         rbsetup_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBE SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbe_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rbsetup_ptr->m.activationTimePresent = 0;
    }
  }

#endif

  tdsrrcrbe_validate_rbsetup_r8(msg_ptr);

  if ( tdsrrc_rbe.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:RB Message validation failed");
    /* Send the RB Setup Failure Message */
    tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
    tdsrrcrbe_clear_procedure();
  }
  /* If the message is valid, proceed further */
  else
  {
    tdsrrcrbe_indicate_layer1_if_rb_setup_configure_voice_rab();

     /* If new rabs are setup, update mac rab status */
    if(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,rab_InformationSetupList))
    {
      tdsrrcrb_update_mac_rab_status(TDSRRC_PROCEDURE_RBE, tdsrrc_rbe.rabs.rabs_in_msg[0].cn_domain,NULL,FALSE);
    }

    /*calculate tdsciphering_activation_cfn only if ciphering is enabled*/
    if (tdsrrcsmc_is_ciphering_enabled() == TDSRRCSMC_SECURITY_STARTED)
    {
      if(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,activationTime))
      {
        /*256 is the CFN rollover length*/
        /*250 is used to activate ciphering after 250 10ms time interval,
        this should give sufficient time for n/w side and ue side to have
        configured ciphering on the rb's successfully*/
        tdsciphering_activation_cfn =
        (uint8) ((rbsetup_ptr->activationTime + 250) % 256);
        /*round it to 8 frame boundary */
        tdsciphering_activation_cfn = (uint8)((tdsciphering_activation_cfn/8)*8);

      }
      else
      {
        /*256 is the CFN rollover length*/
        /*250 is used to activate ciphering after 250 10ms time interval,
        this should give sufficient time for n/w side and ue side to have
        configured ciphering on the rb's successfully*/
        tdsciphering_activation_cfn = (uint8)((tdsseq_get_cfn() + 250) % 256);
        /*round it to 8 frame boundary */
        tdsciphering_activation_cfn = (uint8)((tdsciphering_activation_cfn/8)*8);
      }
    }

    /*call rrcsmc function to update ciphering activation cfn*/
    tdsrrcsmc_update_ciphering_activation_cfn(tdsciphering_activation_cfn);

    /* Check if there is CN information present. */
    if(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Saving CN info for NAS");
      tdsrrc_rbe.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(
        &(rbsetup_ptr->cn_InformationInfo) );
      if (tdsrrc_rbe.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbe.cn_info_saved = TRUE;
      }
    }


    /* Check if there are new RABs to be established or if there are
       affected RB or SRBs to be setup */

    if ((TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,rab_InformationSetupList)) ||
        (((TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,rb_InformationAffectedList)) &&
        (rbsetup_ptr->rb_InformationAffectedList.n != 0 )) ||
        ((TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,srb_InformationSetupList)) &&
         (rbsetup_ptr->srb_InformationSetupList.n != 0))))
    {
      /* Check if the RAB id is already in ESTABLISHED_RABS */
      for (rab_index=0; rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg; rab_index++)
      {
        tdsrab_status = tdsrrc_find_rab_in_established_rabs(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id );
        tdsrrc_rbe.rabs.chan_config_needed[rab_index] = TRUE;

        if ((tdsrab_status != TDSRRC_RAB_NOT_FOUND) &&
             (tdsrrc_find_if_voice_rab_exists_in_established_rabs( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id )
               == FALSE))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:RAB %d already exists in EST_RABS!",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id);
         /* send a failure message if the rab in the message  already
           exists in   established_rabs. Later add a check for the rbs
           associated with the rabs so that the failure will be sent only when
           any of the rb-ids match with the rb-ids in established_rab for that
           rab */
          tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
          /* Send the RB Setup Failure Message */
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause
                                    );

        /* Make sure we're back in the initial sub-state */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
          tdsrrcrbe_clear_procedure();
          return;
        }
      }

      /* First set TDSORDERED_CONFIG so that LLC can process the necessary parameters.
      This function will copy all RABs from the rxd msg to oc. */
      oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBE, tdsrrc_rbe.state_from_msg,
                                             tdsrrc_DL_DCCH_Message_PDU, (void *)dcch_msg_ptr);

      switch ( oc_status )
      {
        case TDSORDERED_CONFIG_SET:
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
          if(tdsrrcsib_is_current_event_sib_read_in_dch())
          {
             TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:RRCGPS: Force Init GPS ");
             tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
          }
#endif /*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/

          /* Ordered Config has been set. We can setup the
            Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
          tdsrrcrbe_send_channel_config_req_r8(msg_ptr);

          if(((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
              (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
              (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
          {
            if(TDSRRCRB_R8_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,new_C_RNTI))
            {
              tdsrrc_rbe.new_crnti_valid = TRUE;
              tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI, &c_rnti);
              tdsrrc_rbe.new_crnti = (uint16)c_rnti;
            }
            else
            {
              tdsrrc_rbe.new_crnti_valid = FALSE;
            }
            tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
          }
          else
          {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
            if(tdsrrc_rbe.curr_substate != TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF)
#endif/* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
            {
               /*Change the substate since we're waiting for the confirmation from LLC for the channel configs */
               tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
            }
          }
          break;

        case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:LLC returned simultaneous reconfig!");
          tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
          /* Send the RB Setup Failure Message */
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);

          /* Go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;

        case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:LLC returned unsupported config!");
          tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
          /* Send the RB Setup Failure Message */
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);

          /* Go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;
          
          case TDSORDERED_CONFIG_CONFIGURATION_INVALID:
 
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:LLC returned invalid config!");
           tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
           /* Send the RB Setup Failure Message */
           tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);
 
           /* Go back to initial substate */
           tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
 
           /* Indicate that an error occured */
           ordered_config_err_occured = TRUE;
           break;
        case TDSORDERED_CONFIG_NOT_SET_OTHER:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:LLC returned other failure!");
          /* When LLC sends this, there is a serious problem
          and we'll go back to disconnected state. No need
          to send the failure message here, we can just
          go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;

        default:

          TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:Unknown return value from LLC");
          /* Unknown error, just go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;
        }/* end switch */

        if ( ordered_config_err_occured == TRUE )
        {
          /* No need to continue. Clear the procedure
            variables and return. */
          tdsrrcrbe_clear_procedure();
          return;
        }

        if (tdsrrc_rbe.rbe_for_srns_relocation == TRUE)
        {
          tdsrrcsmc_commit_fresh_for_srns_relocation();
        }
    }
    else
    {
      /*If we are here, it means that RABs are not given in RB-Setup.
      Also, SRBs and RB Affected IEs are not given.*/
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:No RB IEs are given in the OTA.");
      tdsrrc_rbe.status.failure_status= TDSRRCRB_MSG_INVALID_CONFIG;
      tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_informationElementMissing;  

      /* Send the RB Setup Failure Message */
      tdsrrcrbe_send_rbs_failure_message(tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);

      /* Make sure we're back in the initial sub-state */
      tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
      tdsrrcrbe_clear_procedure();

      return;  
    }/* end if - RBs need to be reconfigured */
  } /* else if valid rb message */
}/* end function process_rbsetup_message_r6 */
#endif /*FEATURE_TDSCDMA_REL8*/


/*===========================================================================

FUNCTION   RRCRBE_SEND_CHANNEL_CONFIG_REQ_R7

DESCRIPTION

  This function sends the TDSRRC_CHANNEL_CONFIG_REQ command to LLC
  to setup the necessary RBs associated with a RAB. If signalling
  Radio bearers are affected and included in the Radio Bearer
  Setup Message, they will also be sent in the same command.

  Currently the messege is not expected to have Signalling
  Radio Bearers for setup. If these are added in the future,
  they will also need to be sent in the same command.

DEPENDENCIES

  TDSORDERED_CONFIG must be set before calling this function.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_send_channel_config_req_r7
(
tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
)
{
  tdsrrc_cmd_type *out_cmd_ptr;  /* To hold the Command                       */
  uint32 rb_count=0;          /* To count the number of RBs in the command */
  uint8  rbe_rb_cnt = 0;

  uint32 rab_index=0;         /* Used when more than one RAB is received in a RBS message */
  uint32 srb_cnt=0;
  tdsrrc_RB_Identity rb_id=0;

  tdsrrc_RB_InformationSetupList_r7 *rb_ptr = NULL; /* Temporary pointer to RB info      */
  tdsrrc_RB_InformationAffectedList_r7 *affected_rb_ptr = NULL;  /* Pointer to RBs to be Reconfigured */
  tdsrrc_RB_InformationReconfigList_r7 *reconfig_rb_ptr = NULL;
  tdsrrc_RAB_InformationSetupList_r7 *rab_info_ptr = NULL;
  tdsrrc_SRB_InformationSetupList_r7    *     srb_setup_ptr = NULL;
  tdsrrcrb_directed_cell_info_type        directed_cell_info; /* directed cell info struct */

  tdsrrc_RadioBearerSetup_r7_IEs* rbsetup_ptr = NULL;

  /* Fill in the command id  */
  tdsrrc_cmd.cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;

  /* Set RBSetup Ptr */
  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
    criticalExtensions.u.criticalExtensions.u.r7.radioBearerSetup_r7;


  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rbsetup_ptr->specificationMode,
      rrc_RadioBearerSetup_r7_IEs_specificationMode_complete))
  {
    /* First fill up the Radio Bearers that need to be setup.
    For this we need to know how many RBs need to be setup.
    If RABs need to be setup, check how many RBs are mapped
    to RABs. */
    if (rbsetup_ptr->specificationMode.u.complete.m.srb_InformationSetupListPresent)
    {
      /* Get the SRB setup list */
      srb_setup_ptr = &rbsetup_ptr->specificationMode.u.complete.srb_InformationSetupList;
  
      while ((srb_setup_ptr->n > srb_cnt) && (srb_cnt < UE_MAX_SRB_SETUP))
      {
        /* Check to see if the RB ID is there ... */
        if (srb_setup_ptr->elem[srb_cnt].m.rb_IdentityPresent)
        {
          rb_id = srb_setup_ptr->elem[srb_cnt].rb_Identity;
        }
  
        /*
        * ... if not, then start with the first SRB and get the next SRB ID
        * each time one is not included in the IE.
        */
        else
        {
          /* Get the next SRB, note this increments to 1 the first time */
          rb_id++;
        }
  
        /* Fill up the RB id and indicate that it needs to be established */
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rb_id;
        if(tdsrrclcm_get_dl_rlc_lc_id(rb_id) == TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSESTABLISH_RB;
        }
        else
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = 
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
        }
  
        rb_count++;
        srb_cnt++;
  

  
      } /* end SRB setup */
  
    }

    if (rbsetup_ptr->specificationMode.u.complete.m.rab_InformationSetupListPresent)    
    {
      rab_info_ptr = &rbsetup_ptr->specificationMode.u.complete.rab_InformationSetupList;
      for (rab_index=0; rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg && (rab_info_ptr->n > rab_index); rab_index++)
      {
        if ( tdsrrc_rbe.rabs.chan_config_needed[rab_index] == TRUE )
        {
          rb_ptr = &rab_info_ptr->elem[rab_index].rb_InformationSetupList;
    
          if ( rb_ptr->n == 0 )
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"No RBs present in msg!");
          } else
          {
            rbe_rb_cnt = 0;
            while ( rb_ptr->n > rbe_rb_cnt )
            {
              /* Fill up the RB id and indicate that it needs to be established */
              tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rb_ptr->elem[rbe_rb_cnt].rb_Identity;
              tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSESTABLISH_RB;
    
              /* Update the local structure so that we know which RB id's we've
              requested to be setup */
              tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rb_for_rab[rbe_rb_cnt].rb_id = rb_ptr->elem[rbe_rb_cnt].rb_Identity;
    
              /* Increment count of RBs */
              rb_count++;
              rbe_rb_cnt ++;
    
              /* Do a safety check on the count. We can't do more than 32
              configs in a command */
              if ( (rb_count >= TDS_MAX_RB) || (rbe_rb_cnt >= MAX_RB_PER_RAB) )
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"MAX RB count exceeded!");
                break;
              }
    
              /* Go to the next RB */
            }/* end while */
            /* Save the number of RBs for this RAB */
            tdsrrc_rbe.rabs.rabs_in_msg[rab_index].num_rbs_for_rab = rbe_rb_cnt;
          }
        }/* end if rabs_setup_needed */
      }
    }
    /* Now check if the message contains an RB information to be
       reconfig list. If so, we need to reconfigure some RBs */
    if ( rbsetup_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent)
    {
      uint32 local_idx=0;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB info reconfig list present");
  
      reconfig_rb_ptr = &rbsetup_ptr->specificationMode.u.complete.rb_InformationReconfigList;
  
      while ( reconfig_rb_ptr->n > local_idx )
      {
  
        /* Fill up the RB id and indicate that it needs to be reconfigured */
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = reconfig_rb_ptr->elem[local_idx].rb_Identity;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = 0;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = 0;
        if((reconfig_rb_ptr->elem[local_idx].m.rlc_InfoPresent)&&
           (reconfig_rb_ptr->elem[local_idx].rlc_Info.m.ul_RLC_ModePresent))
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = TDSMODIFY_RB_CFG;
        }
        if((reconfig_rb_ptr->elem[local_idx].m.rlc_InfoPresent)&&
           (reconfig_rb_ptr->elem[local_idx].rlc_Info.m.dl_RLC_ModePresent))
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
        }

        if (reconfig_rb_ptr->elem[local_idx].m.rb_StopContinuePresent)
        {
          /* over-ride rb_config with STOP or continue */
          if (tdsrrc_RB_StopContinue_stopRB == reconfig_rb_ptr->elem[local_idx].rb_StopContinue)
          {
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask |= TDSSTOP_RB_CFG;
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask |= TDSSTOP_RB_CFG;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"rb-reconfig-info rb %d to STOP rb_count %d",tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id,rb_count);
          }
          else if (tdsrrc_RB_StopContinue_continueRB == reconfig_rb_ptr->elem[local_idx].rb_StopContinue)
          {
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask |= TDSCONTINUE_RB_CFG;
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask |= TDSCONTINUE_RB_CFG;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"rb-reconfig-info rb %d to CONTINUE rb_count %d",tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id,rb_count);
            
          }
        }
  
        /* Increment count of RBs. This is done here since we
        need to use the next index that's not used already. */
        rb_count++;
  
        /* Go to the next affected RB */
        local_idx++;
  
        /* Do a safety check on the count. We can't do more than 32
        configs in a command */
        if ( rb_count >= TDS_MAX_RB )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MAX RB count exceeded!");
          break;
        }
      } /* end while */
    }
  
    /* Now check if the message contains an RB information to be
    affected list. If so, we need to reconfigure some RBs */
    if ( rbsetup_ptr->specificationMode.u.complete.m.rb_InformationAffectedListPresent)
    {
      uint32 local_idx=0;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB info affected list present");
  
      affected_rb_ptr = &rbsetup_ptr->specificationMode.u.complete.rb_InformationAffectedList;
  
      if ( affected_rb_ptr->n == 0 )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"No affected RBs in msg!");
      }
      else
      {
        while ( affected_rb_ptr->n > local_idx )
        {
  
          /* Fill up the RB id and indicate that it needs to be reconfigured */
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = affected_rb_ptr->elem[local_idx].rb_Identity;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;

  
          /* Increment count of RBs. This is done here since we
          need to use the next index that's not used already. */
          rb_count++;
  
          /* Go to the next affected RB */
          local_idx++;
  
          /* Do a safety check on the count. We can't do more than 32
          configs in a command */
          if ( rb_count >= TDS_MAX_RB )
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"MAX RB count exceeded!");
            break;
          }
        } /* end while */
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"No affected RB list in msg");
    }/* end if affected list present */

  }
  else if( rbsetup_ptr->specificationMode.t == T_tdsrrc_RadioBearerSetup_r6_IEs_specificationMode_dummy)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Preconfig mode not supported for RB-setup");
  }

  /* Number of RBs to config. Note rb_count is the total
  of established and reconfigured RBs */
  tdsrrc_cmd.cmd.chan_config_req.num_rb_to_config = (int) rb_count;

  if(rb_count < TDS_MAX_RB)
  {
    tdsrrc_fill_rb_id_chan_req_from_standalone_db(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
  }

  if(tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)
  {
     /* Fill those RBs in chan_config_req for which RLC PDU size has changed and 
     that are already not present in chan_config_req*/
    if(rb_count < TDS_MAX_RB)
    {
    tdsrrc_fill_rb_id_chan_req_from_rb_list_mapped_to_dch(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
    }
  }
 
  /*If state transition is Cell_DCH->Cell_FACH or vice-verca, add all the existing 
  AM PS RBs in chan_config_req*/    
  if(((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_FACH) && (tdstransition_config.toc_usage == 
      TDSTOC_FOR_DCH_AND_FACH)) || ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && (tdsrrc_rbe.state_from_msg 
      == TDSRRC_STATE_CELL_DCH)))
  {
    tdsrrc_fill_rb_id_chan_req_from_est_rabs(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
  }
  /* Check if state change is required */
  if ( (tdsrrc_get_state()) == (tdsrrc_rbe.state_from_msg) )
  {
    tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required = FALSE;
  }
  else
  {
    /* The message indicates that state change is required.
    Indicate to LLC what the next state should be. */
    tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required = TRUE;
    tdsrrc_cmd.cmd.chan_config_req.next_state = tdsrrc_rbe.state_from_msg;
  }

  /* Indicate the id of the procedure sending the command */
  tdsrrc_cmd.cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RBE;

  /* Indicate that we need a confirmation for this command */
  tdsrrc_cmd.cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

  /* We are not required to fill any other data for this command.
   Now, send the command */
  if (((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
      (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
      (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
  {

    /*Initiate the cell selection*/
    tdsrrcrb_send_initiate_cell_selection_req(TDSRRC_PROCEDURE_RBE,
                                           TDSRRC_TRANSITION_FROM_DCH,
                                           tdsrrc_rbe.state_from_msg);

    tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

    /* Start T_305 [periodic cell update] timer */
    tdsrrccu_start_t_305_timer();
  }
  else
  {
    /*check whether any frequency info is present in the message
    and transition is CELL_FACH->CELL_FACH. In this case
    initiate a cell update after channel config is successfully completed */
    if (((tdsrrc_get_state())==TDSRRC_STATE_CELL_FACH)&&
        (tdsrrc_rbe.state_from_msg==TDSRRC_STATE_CELL_FACH))
    {
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
           tdsrrc_RadioBearerSetup_r7_IEs,frequencyInfo)) &&
          (rbsetup_ptr->frequencyInfo.
           modeSpecificInfo.t == T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd))
      {
        directed_cell_info.freq_info_present = TRUE;
        directed_cell_info.uarfcn_dl = rbsetup_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
      }
      else
      {
        directed_cell_info.freq_info_present = FALSE;
      }
      if (tdsrrcrbe_is_pccpch_info_present(msg_ptr, TDSMSG_REL7))
      {
        directed_cell_info.cpid_present = TRUE;
        directed_cell_info.cell_param_id = rbsetup_ptr->dl_InformationPerRL_List.elem[0].
                                             modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
      }
      else
      {
        directed_cell_info.cpid_present = FALSE;
      }
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE)
      {
        /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection 
          FACH->FACH ), treat it as un-supported configuration and send failure.*/ 
        if(tdsrrccsp_check_initiate_cell_selection_handling() == FALSE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
       
          tdsrrcrbe_send_initiate_cu_flag = TRUE;
          tdsrrcrbe_delete_crnti          = TRUE;
        }
        else
        {
          tdsrrc_int_c_rnti_type      c_rnti;       /* Local C_RNTI value */
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r7_IEs,new_C_RNTI))
          {
            tdsrrc_rbe.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI,
                                &c_rnti);
            tdsrrc_rbe.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_rbe.new_crnti_valid = FALSE;
          }
          tdsrrcllcoc_update_toc_with_oc_from_fach();
          
          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req(TDSRRC_PROCEDURE_RBE,
                                                  TDSRRC_TRANSITION_FROM_FACH,
                                                  tdsrrc_rbe.state_from_msg);
       
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
              
          return;
        }
      }
#else/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
      {
        /*directed cell info matched. Cell update is not required */
        tdsrrcrbe_send_initiate_cu_flag = FALSE;
      }
      else
      {
        /*directed cell info do not match. Cell update is required */
        tdsrrcrbe_send_initiate_cu_flag = TRUE;
        tdsrrcrbe_delete_crnti          = TRUE;
      }
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
    } /* if CELL_FACH->CELL_FACH reconfiguration */
    /* Get a Command buffer and copy the local command to the buffer
    * so as to enqueue it on the internal queue.
    */

    if ((out_cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL)
    {
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
      memscpy(out_cmd_ptr,sizeof(tdsrrc_cmd_int_type),&tdsrrc_cmd,sizeof(tdsrrc_cmd_int_type));
#else
      out_cmd_ptr->cmd = tdsrrc_cmd.cmd;
      out_cmd_ptr->cmd_hdr = tdsrrc_cmd.cmd_hdr;
#endif

      tdsrrc_put_int_cmd( out_cmd_ptr );
    }
    else
    {
      ERR_FATAL("Could not get internal cmd buf",0,0,0);
    }
  }
}

/*===========================================================================
FUNCTION   tdsrrcrbe_validate_rab_setup_list_r7

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_status_e_type tdsrrcrbe_validate_rab_setup_list_r7
(
  tdsrrc_RAB_InformationSetupList_r7 * rab_list_ptr,
  uint32 no_of_rabs,
  uint32 nas_sync_ind
)
{
  rrc_rab_id_type rab_id;           /* To store RAB id in internal RRC format */
  
  /* To store the status returned by MN layer. This indicates what type of RAB is received. This is
  used only if the RAB is for the CS_domain */
  rrcmn_rab_status_T rab_type = RRCMN_INVALID_RAB_ID;     
  
  /* Used as an index when storing established RABs locally. Only when we have more than
  one RAB per message, this will be incremented */
  uint32 rab_index=0;               

  boolean voice_rab_already_exists = FALSE;

    
  for (rab_index= no_of_rabs; (rab_list_ptr->n > rab_index) && (rab_index < TDSRRCRB_MAX_RAB_PER_MSG); rab_index++)
  {
    /* GSM_MAP RAB Id. First convert it to internal format. */      
    rab_id = tdsrrcrb_translate_gsm_map_rab_id((rab_list_ptr->elem[rab_index].rab_Info.
                                                  rab_Identity.u.gsm_MAP_RAB_Identity));
    
    /* Store this info locally so that ESTABLISHED_RABS can
    be updated later, when these RABs are established */
    tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id_is_gsm_map = TRUE;
    tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id = rab_id;
    tdsrrc_rbe.rabs.rabs_in_msg[rab_index].re_est_timer =
    rab_list_ptr->elem[rab_index].rab_Info.re_EstablishmentTimer;
    /* Check if this RAB is CS Domain or PS Domain */
    if ( rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
    {
      /* Store the domain identity. This will be later added to ESTABLISHED_RABS */
      tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain = RRC_CS_DOMAIN_CN_ID;

      /* For CS RABs, Call MN layer to verify this RAB */
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
      rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
      rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif

      switch ( rab_type )
      {
        case RRCMN_CS_DATA_CALL:

          TDSRRC_MSG1(MSG_LEGACY_LOW,"Received a CS Data RAB Id %d",rab_id);
          break;

        case RRCMN_CS_VOICE_CALL:

          TDSRRC_MSG1(MSG_LEGACY_LOW,"Received a CS Voice RAB Id %d",rab_id);

          {
            if(TDSRRC_MSG_COMMON_BITMASK_IE(rab_list_ptr->elem[rab_index].rab_Info,
                tdsrrc_RAB_Info_r7,nas_Synchronisation_Indicator))
            {
              nas_sync_ind =  tdsrrc_translate_nas_sync_indicator(
                                     &(rab_list_ptr->elem[rab_index].rab_Info.nas_Synchronisation_Indicator));
              if( nas_sync_ind == TDSCDMA_IVOCODER_ID_NONE )
              {
                tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported codec %d",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator );
                return FAILURE;
              }          
            }

            tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator =nas_sync_ind;        
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
            tdsrrc_set_ordered_codec_status(TRUE);
#endif

            if (tdsrrc_find_if_voice_rab_exists_in_established_rabs( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id)
                 == TRUE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Voice RAB Id - %d already exists",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id );

              /*The condition of "if" covers the case when third RB is being added for AMR call*/
              if(rab_list_ptr->elem[rab_index].rb_InformationSetupList.n == 1)
              {
                voice_rab_already_exists = TRUE;
              }
              else
              {
                tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid configuration .. more than one RAB subflow is being added");
                return FAILURE;
              }
            }
            /*The given RAB isn't an existing Voice RAB.*/
            else
            {
              /* Whenever a new voice RAB is to be set up, at least two RBs need to be associated with that RAB.
              Therefore, the condition of "if" checks for presence of more than 1 RB.*/
              if (rab_list_ptr->elem[rab_index].rb_InformationSetupList.n == 1)
              {
#ifdef FEATURE_SCUDIF_ENHANCEMENT
                #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Not enough RBs mapped to voice RAB!");
                tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
              }
            }
          }          
        break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS RAB validation failed! RABid %d",rab_id);
          break;
      } /* end switch */

      if((tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_INVALID_CONFIG) ||
         (tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG))
      {
        /* No need to continue */
        return FAILURE;
      }
    }

    /* Else check for PS Domain */
    else if ( rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain )
    {
      /* Store the domain identity. This will be later added to ESTABLISHED_RABS */
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain = RRC_PS_DOMAIN_CN_ID;

      /* For PS Domain, if it's a loopback, then no need for query, otherwise, call RABM module to verify the RAB */
      if((tdsrrclbt_is_lb_on() == TRUE) ||
#ifdef FEATURE_DUAL_WCDMA
         (rabm_rabid_validate_per_sub(tdsrrc_get_as_id(), RAB_ESTABLISH_QUERY, (uint8)rab_id) == TRUE)
#else
         (rabm_rabid_validate(RAB_ESTABLISH_QUERY, (uint8)rab_id) == TRUE)
#endif
      )
      {
        TDSRRC_MSG1(MSG_LEGACY_LOW,"PS RAB Id %d to be setup",rab_id);
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"PS RAB validation failed! RABid %d",rab_id);
        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        return FAILURE;
      }
    }
    
    if(voice_rab_already_exists == TRUE)
    {
      tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_VOICE_RAB;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Updated RAB type as voice");
    }
    else
    {
      /* ASN1 structure 'tdsrrc_RAB_InformationSetup' is validated at this point,
      hence update rab_type in structure 'tdsrrc_rbe.rabs.rabs_in_msg[rab_index]'. */
      /* change argument */
      (void) tdsrrc_update_rab_type_in_rab_info_r7(&(rab_list_ptr->elem[rab_index]),
         &(tdsrrc_rbe.rabs.rabs_in_msg[rab_index]));
    }

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
             tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id,
             tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain,
             tdsrrc_rbe.rabs.rabs_in_msg[rab_index].re_est_timer);
    /* set the ordered codec mode which is sent in the ordered config to MAC.
      * If the RBE msg is not for Voice RAB also then also the current codec value
      * is passed to MAC 
      */
    tdsrrc_set_ordered_codec_mode(nas_sync_ind);
    tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator = nas_sync_ind;

    /* Go to next RAB info  */
    
  }
  if (rab_list_ptr->n > rab_index)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Too many RABs in an RBSetupMessage");
    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;

    return FAILURE;
  }
  
  tdsrrc_rbe.rabs.num_rabs_in_msg = rab_index;
  return SUCCESS;
}

/*===========================================================================
FUNCTION   tdsrrcrbe_prevalidate_rab_setup_list_r7

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_status_e_type tdsrrcrbe_prevalidate_rab_setup_list_r7
(
  tdsrrc_RAB_InformationSetupList_r7 *rab_list_ptr
)
{
   tdsrrc_RB_InformationSetup_r7 *rb_info_ptr = NULL;
  
#ifdef FEATURE_UMTS_PDCP
   tdsrrc_HeaderCompressionInfo_r4 *pdcpHeaderCompressionInfo = NULL;
#endif /* FEATURE_UMTS_PDCP*/
   uint32 rab_index=0,rb_index=0;
  do  /* going through the RAB info setup list */
  {
    /* Check if the RAB id is GSM_MAP type. We don't support any other type */
    if ((rab_list_ptr->elem[rab_index].rab_Info.rab_Identity.t
      != T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB ID t%d is not GSM_MAP!",rab_list_ptr->elem[rab_index].rab_Info.rab_Identity.t);
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return FAILURE;
    }

    /* check whether all RABs in the rabinfo list belong to same CN domain id or not
    otherwise this is an invalid configuration */
    if (rab_list_ptr->n > (rab_index+1))
    {
      if (rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity !=
          rab_list_ptr->elem[rab_index+1].rab_Info.cn_DomainIdentity)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Different CN domains in rab-list %d %d",rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity,rab_list_ptr->elem[rab_index+1].rab_Info.cn_DomainIdentity);
        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        return FAILURE;
      }
    }

    /* Reject if MBMS info is included */
    if ((rab_list_ptr->elem[rab_index].rab_Info.m.mbms_SessionIdentityPresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MBMS session Identity present. Not supported");
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return FAILURE;
    }

    /* Reject if MBMS info is included */
    if ((rab_list_ptr->elem[rab_index].rab_Info.m.mbms_ServiceIdentityPresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MBMS session Identity present. Not supported");
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return FAILURE;
    }

    /* check for the unsupported PDCP info */
    rb_index=0;
    do  /* going through the RB info setup list */
    {

        rb_info_ptr = &rab_list_ptr->elem[rab_index].rb_InformationSetupList.elem[rb_index];
      if (tdsrrc_find_rb_in_est_rabs(rb_info_ptr->rb_Identity) == TDSRRC_RB_PRESENT) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB id %d already exist in est rabs: Invalid config",rb_info_ptr->rb_Identity);
        tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        return FAILURE;
      }

      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_ptr,tdsrrc_RB_InformationSetup_r7,pdcp_Info))
      {
#ifdef FEATURE_UMTS_PDCP
        if (tdsrrc_pdcp_enabled)
        {
          /* If the RAB is for cs domain, send RBS failure with cause invalid configuration */
          if (rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info invalid for CS rab");
            tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
            return FAILURE;
          }

          if(rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
          {
            /* check whether lossless SRNS relocation flag is set
             * and header compression algo is present in the message */
            if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
                (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_info_ptr->pdcp_Info.losslessSRNS_RelocSupport,
                rrc_LosslessSRNS_RelocSupport_supported)))
            {
              if (rb_info_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_absent)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP pdu header is absent.  Invalid config");
                tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                return FAILURE;
              }
            } /* if rrc_PDCP_Info_r4_losslessSRNS_RelocSupport_present */

            if (rb_info_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present)
            {
              if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
                (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_info_ptr->pdcp_Info.losslessSRNS_RelocSupport,
                rrc_LosslessSRNS_RelocSupport_notSupported)))
              {
                if (!(TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)))
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP header compress info list not present");
                  tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                  return FAILURE;
                }
              }
            }

            if (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList))
            {
              uint32 local_idx=0;
              while (rb_info_ptr->pdcp_Info.headerCompressionInfoList.n > local_idx)
              {
                pdcpHeaderCompressionInfo = &rb_info_ptr->pdcp_Info.headerCompressionInfoList.elem[local_idx];
                if (TDSRRC_CHECK_COMMON_MSG_TYPE(pdcpHeaderCompressionInfo->algorithmSpecificInfo,
                    rrc_AlgorithmSpecificInfo_r4_rfc2507_Info))
                {
                  /* if the UE capability "Maximum header compression context space", is exceeded with this configuration send
                  * failure with cause "Invalid Config"  */
                  if ( 
                    (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER > IPHC_MAX_HC_CONTEXT_SPACE))
                  {
                    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  max header %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER);
                    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                    return FAILURE;
                  }
                }
                else if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.t == T_tdsrrc_AlgorithmSpecificInfo_r4_rfc3095_Info)
                {
                  /* the chosen MAX_CID shall not be greater than the value "Maximum number of ROHC context sessions" as 
                  *  indicated in the IE "PDCP Capability" */
                  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,
                    ul_RFC3095))
                  {
                    if ( (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS))
                    {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID);
                      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                      return FAILURE;
                    }
                  }

                  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,dl_RFC3095))
                  {
                    if ((pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS))
                    {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID);
                      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                      return FAILURE;
                    }
                  }
                }  /* if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.t == rrc_rfc3095_Info)*/

                local_idx++;
              } /*  while (pdcpHeaderCompressionInfoList) */
            } /*  if (rb_info_ptr->pdcp_Info.bit_mask & rrc_PDCP_Info_r4_headerCompressionInfoList_present) */
          }
        }
#endif /*FEATURE_UMTS_PDCP*/
      } /* if (rb_info_ptr->bit_mask & rrc_RB_InformationSetup_r6_pdcp_Info_present) */
      /*point to the next link */
      rb_index++;
    } while (rab_list_ptr->elem[rab_index].rb_InformationSetupList.n > rb_index);

    /* Go to next RAB setup infomation  */
    rab_index++;
  } while (rab_list_ptr->n > rab_index);

  return SUCCESS;
}

/*===========================================================================
FUNCTION   VALIDATE_RBSETUP_R7

DESCRIPTION
  This function validates the received R6 RB Setup Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Setup Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbe_validate_rbsetup_r7
(
  tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
)
{

  tdsrrc_RadioBearerSetup_r7_IEs* rbsetup_ptr = NULL;

  uecomdef_status_e_type status;

  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();

  /* Assume the message is valid for now */
  tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_VALID;

  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
    u.criticalExtensions.u.r7.radioBearerSetup_r7;

  switch (rbsetup_ptr->rrc_StateIndicator)
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbe.state_from_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbe.state_from_msg = TDSRRC_STATE_CELL_FACH;
      break;

    default: /* Just to satisfy LINT */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid RRC state from Msg");
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;

  } /* End of switch */

  if ( tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
    return;
  }
#if 0
   /*Yet to add the support ofr REL7 modes.*/
  /*Reject RB setup with default configuration */
  if (rbsetup_ptr->specificationMode.t &
      rrc_RadioBearerSetup_r7_IEs_specificationMode_preconfiguration)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Default config not supported on rbsetup");
    tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    return;
  }
#endif
  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r7_IEs,dl_HSPDSCH_Information))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL:HSPDSCH Info present");

    /* Check if network does not try to initiate HSDPA with invalid state */
    status = tdsrrc_hsdpa_initial_validation(rbsetup_ptr->rrc_StateIndicator,
             TDSRRC_CHECK_COMMON_MSG_TYPE(rbsetup_ptr->dl_HSPDSCH_Information.modeSpecificInfo,
                                rrc_DL_HSPDSCH_Information_r7_modeSpecificInfo_fdd));

    if (status == FAILURE)
    {
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rbsetup_ptr->specificationMode,
      rrc_RadioBearerSetup_r7_IEs_specificationMode_complete))
  {
    /* Check if there are new RABs to be established */
    if (rbsetup_ptr->specificationMode.u.complete.m.rab_InformationSetupListPresent)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL: RAB Info present in RBS Message");
      if(tdsrrcrbe_prevalidate_rab_setup_list_r7(&rbsetup_ptr->specificationMode.u.complete.
          rab_InformationSetupList) == FAILURE)
      {
        return;
      }
    }
    else
    {
      /* There is no RAB to be setup in this message.*/
      tdsrrc_rbe.rabs.num_rabs_in_msg = 0;
    }
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    if (rbsetup_ptr->specificationMode.u.complete.m.dl_CounterSynchronisationInfoPresent)
#else
    if (tdsrrcrbe_validate_srns_relocation_info(msg_ptr, TDSMSG_REL7))
#endif
       
    {
      /*check whether the SRNS relocation is within Cell_DCH or not.
        SRNS relocation information can come in reconfiguration messages
        only when the state transition is Cell_DCH->Cell_DCH */
      if (!((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH) &&
            ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbe.state_from_msg);
        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        return;
      }
     /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has
       PDCP info list. If it is true then this configuration is unsupported */
  
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rbsetup_ptr->specificationMode.u.complete.dl_CounterSynchronisationInfo,
           rb_WithPDCP_InfoList)) ||
          (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->specificationMode.u.complete.dl_CounterSynchronisationInfo,
           tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        return;
      }
      else
      {
        /* The procedure involves Lossy SRNS relocation.
        Set the flag that indicates that this procedure
        involves SRNS relocation */
        tdsrrc_rbe.rbe_for_srns_relocation = TRUE;
        /* store if new U-RNTI is present in the message */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
            tdsrrc_RadioBearerSetup_r7_IEs,new_U_RNTI))
        {
          tdsrrc_rbe.new_urnti_valid = TRUE;
          tdsrrc_translate_urnti (&(rbsetup_ptr->new_U_RNTI),
                               &tdsrrc_rbe.new_urnti);
        }
      }
    }

   /* Process RAB reconfig before RAB setup because we want to catch NW's which
       are trying to setup and reconfig RAB with the same message */
    if (rbsetup_ptr->specificationMode.u.complete.m.rab_InformationReconfigListPresent)
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RAB Info Reconfig in R6 RBS Msg");


      tdsrrc_rbe.status.failure_status = tdsrrc_update_rab_info_reconfig_param(
         &rbsetup_ptr->specificationMode.u.complete.rab_InformationReconfigList, &tdsrrc_rbe.rabs, &nas_sync_ind);
                                           
      if( tdsrrc_rbe.status.failure_status != TDSRRCRB_MSG_VALID) 
      {
        return;
      }
      
    }

    if (rbsetup_ptr->specificationMode.u.complete.m.rab_InformationSetupListPresent)
    {
      if(tdsrrcrbe_validate_rab_setup_list_r7(&rbsetup_ptr->specificationMode.u.complete.
          rab_InformationSetupList, tdsrrc_rbe.rabs.num_rabs_in_msg,nas_sync_ind) == FAILURE)
        {
          return;
        }
      }
    }

  /* Store the number of the RAB info in the RBSetup message. */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"%d RABs  in an RBSetupMessage",tdsrrc_rbe.rabs.num_rabs_in_msg);

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
  */
  tdsrrc_rbe.trans_to_dch_ciph_config_needed = FALSE;
  if((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH) && 
     (tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH)
    )
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
        tdsrrc_RadioBearerSetup_r7_IEs,dl_CommonInformation)) && 
       (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CommonInformation,
       tdsrrc_DL_CommonInformation_r7,dl_dpchInfoCommon)) &&
       ( rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.t ==
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
             (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(
              rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon,tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
              &rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
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
        tdsrrc_rbe.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }

  /* If UTRAN DRX coefficient is not present then it is invalid configuration */
  if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_rbe.state_from_msg  == TDSRRC_STATE_URA_PCH))
  {

    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
          tdsrrc_RadioBearerSetup_r7_IEs,utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_rbe.state_from_msg);
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  if((TDSRRC_CHECK_COMMON_MSG_TYPE(rbsetup_ptr->specificationMode,rrc_RadioBearerSetup_r7_IEs_specificationMode_complete)) &&
      rbsetup_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent)
  {
    /* Save the linked list head pointer */
    if (FAILURE == tdsrrcllc_validate_rb_info_reconfig_list_r7(
        &rbsetup_ptr->specificationMode.u.complete.rb_InformationReconfigList))
    {
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  if (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH)
  {
    /*should we do it after getting l2ack for response??*/
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
          tdsrrc_RadioBearerSetup_r7_IEs,ura_Identity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
      tdsrrccu_clear_ura_id ();
    }
    else
    {
      /*URA ID present in the message set ura_identity variable with this value */
      if (rbsetup_ptr->ura_Identity.numbits == 16)
      {
        tdsrrccu_update_ura_id (&(rbsetup_ptr->ura_Identity));
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rbsetup_ptr->ura_Identity.numbits);
        tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
        tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
        return;
      }
    }
  }
  return;

}

/*===========================================================================
FUNCTION   PROCESS_RBSETUP_R7

DESCRIPTION

  This function processes the Rel 6 RB Setup Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Setup Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -

    TDSORDERED_CONFIG will be set by calling LLC.

    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.

    If TDSORDERED_CONFIG could not be set, a Radio Bearer Setup Failure message
    will be sent to UTRAN.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_process_rbsetup_r7
(
tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_DL_DCCH_Message     *dcch_msg_ptr;   /* Pointer to the RB Setup message */
  tdsrrc_RadioBearerSetup *msg_ptr;    /* Pointer to the RB Setup message        */

  tdsrrc_rab_search_e_type tdsrab_status = TDSRRC_RAB_NOT_FOUND; /* To check if RAB is present in ESTABLISHED_RABS */
  tdsrrcllc_oc_status_e_type oc_status;/* To hold tdsordered_config status          */
  uint32 rab_index=0;               /* Used when more than 1 rab is present in
                                    a RBS message                             */
  tdsrrc_int_c_rnti_type      c_rnti;       /* Local C_RNTI value */
  boolean ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                                 occured while setting tdsordered_config. */
  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL7;

  tdsrrc_RadioBearerSetup_r7_IEs* rbsetup_ptr = NULL;

  tdsrrcsmc_srns_ip_check_status_e_type    ip_check_status = TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;
  tdsrrcrb_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;


  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCEUL:rbsetupr7 cur_substate %d",tdsrrc_rbe.curr_substate);

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Setup message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerSetup);

  /* Store Msg Ptr in a local variable */
  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
    u.r7.radioBearerSetup_r7;

  if (TRUE == tdsrrcrbe_validate_srns_relocation_info(msg_ptr, msg_version))
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
          == tdsrrc_rbe.tr_ids.rejected_transaction)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBE msg - rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
        /* Nothing else to do, just ignore the message and return */
      }
      else
      {
        /* send a reconfig failure messge with invalid
        configuration and
          continue the same substate with any ongoing procedure */
        temp_rejected_transaction = tdsrrc_rbe.tr_ids.rejected_transaction;
        temp_failure_cause.failure_status = tdsrrc_rbe.status.failure_status;
        
        tdsrrc_rbe.tr_ids.rejected_transaction =
        (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
        if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
        {
          tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        }
        else
        {
          tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
        }
        /*Check whether Cell Update is going on */
        if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
             (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
        {
          if(tdsrrc_rbe.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbe.curr_substate,tdsrrc_rbe.status.failure_status);
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                     tdsrrc_rbe.status.prot_err_cause );
            tdsrrc_rbe.tr_ids.rejected_transaction = temp_rejected_transaction;
            tdsrrc_rbe.status.failure_status = temp_failure_cause.failure_status;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
            (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
            /* Wait until Cell update procedure is completed */
            tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
          }
        }
        else
        {
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause );
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
      /*do not clear the procedure or change the tdsrrc_rbe substate because
      some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
      return;
    }

  } /* End of validation of srns relocation */

  if ((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
      (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
      (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incomatible simultaneous reconfig! Rejecting RBS message");
    if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
         == tdsrrc_rbe.tr_ids.accepted_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBS msg! accepted tr-id%d match!",tdsrrc_rbe.tr_ids.accepted_transaction);
      /* Nothing else to do, just return */
    }
    else if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                == tdsrrc_rbe.tr_ids.rejected_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBS msg! rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
      /* Nothing else to do, just return */
    }
    else
    {
      /* We have received a RBS message with a new transaction id.
      This message should be in our rejected transaction
        list */
      temp_rejected_transaction = tdsrrc_rbe.tr_ids.rejected_transaction;
      temp_failure_cause.failure_status = tdsrrc_rbe.status.failure_status;
      tdsrrc_rbe.tr_ids.rejected_transaction =
      (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
      tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;

      /*Check whether Cell Update is going on */
      if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
           (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
      {
        if(tdsrrc_rbe.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbe.curr_substate,tdsrrc_rbe.status.failure_status);
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                   tdsrrc_rbe.status.prot_err_cause );
          tdsrrc_rbe.tr_ids.rejected_transaction = temp_rejected_transaction;
          tdsrrc_rbe.status.failure_status = temp_failure_cause.failure_status;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
          (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
          /* Wait until Cell update procedure is completed */
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        }
      } 
      else
      {
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                  tdsrrc_rbe.status.prot_err_cause );
        /*continue the same substate */
      }
    }
    return;
  }

  /* We have received a RB setup message with a new transaction id.
  Accept the transaction Id */
  tdsrrc_rbe.tr_ids.accepted_transaction =
  msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rbsetup_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rbsetup_ptr->m.activationTimePresent,
         rbsetup_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBE SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbe_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rbsetup_ptr->m.activationTimePresent = 0;
    }
  }

#endif

  tdsrrcrbe_validate_rbsetup_r7(msg_ptr);

  if ( tdsrrc_rbe.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB Message validation failed");
    /* Send the RB Setup Failure Message */
    tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                              tdsrrc_rbe.status.prot_err_cause
                            );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
    tdsrrcrbe_clear_procedure();
  }
  /* If the message is valid, proceed further */
  else
  {
    tdsrrcrbe_indicate_layer1_if_rb_setup_configure_voice_rab();

    /*From lower layer's point of view, CS over HSPA call is same as PS call.
    So updating tdsmac_rab_status as PS Call for Cs over HSPA.*/

    if(rbsetup_ptr->specificationMode.u.complete.m.rab_InformationSetupListPresent)
    {
      tdsrrcrb_update_mac_rab_status(
        TDSRRC_PROCEDURE_RBE,
        tdsrrc_rbe.rabs.rabs_in_msg[0].cn_domain,
        NULL,
        FALSE);
    }


    /*calculate tdsciphering_activation_cfn only if ciphering is enabled*/
    if (tdsrrcsmc_is_ciphering_enabled() == TDSRRCSMC_SECURITY_STARTED)
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r7_IEs,activationTime))
      {
        /*256 is the CFN rollover length*/
        /*250 is used to activate ciphering after 250 10ms time interval,
        this should give sufficient time for n/w side and ue side to have
        configured ciphering on the rb's successfully*/
        tdsciphering_activation_cfn =
        (uint8) ((rbsetup_ptr->activationTime + 250) % 256);
        /*round it to 8 frame boundary */
        tdsciphering_activation_cfn = (uint8)((tdsciphering_activation_cfn/8)*8);

      }
      else
      {
        /*256 is the CFN rollover length*/
        /*250 is used to activate ciphering after 250 10ms time interval,
        this should give sufficient time for n/w side and ue side to have
        configured ciphering on the rb's successfully*/
        tdsciphering_activation_cfn = (uint8)((tdsseq_get_cfn() + 250) % 256);
        /*round it to 8 frame boundary */
        tdsciphering_activation_cfn = (uint8)((tdsciphering_activation_cfn/8)*8);
      }
    }

    /*call rrcsmc function to update ciphering activation cfn*/
    tdsrrcsmc_update_ciphering_activation_cfn(tdsciphering_activation_cfn);

    /* Check if there is CN information present. */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r7_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      tdsrrc_rbe.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(
        &(rbsetup_ptr->cn_InformationInfo) );
      if (tdsrrc_rbe.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbe.cn_info_saved = TRUE;
      }
    }

    /* Check if there are new RABs to be established or if there are
       affected RB or SRBs to be setup
     */

    if ((rbsetup_ptr->specificationMode.u.complete.m.rab_InformationSetupListPresent) ||
        (((rbsetup_ptr->specificationMode.u.complete.m.rb_InformationAffectedListPresent) &&
          (rbsetup_ptr->specificationMode.u.complete.rb_InformationAffectedList.n != 0 )) ||
        ((rbsetup_ptr->specificationMode.u.complete.m.srb_InformationSetupListPresent) &&
         (rbsetup_ptr->specificationMode.u.complete.srb_InformationSetupList.n != 0))))
    {
      /* Check if the RAB id is already in ESTABLISHED_RABS */
      for (rab_index=0; rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg; rab_index++)
      {
        tdsrab_status = tdsrrc_find_rab_in_established_rabs
          (tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id );
        tdsrrc_rbe.rabs.chan_config_needed[rab_index] = TRUE;
        /*Right now, we are not handling VT -> CSoHS Voice scenarios.*/
#ifdef FEATURE_SCUDIF_ENHANCEMENT
        #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/

        /*The following case handles the scenario when the RAB being established is present in EST RABs But it's not 
        CS VOICE RAB. In that case, RB-Setup Failure has to be sent*/
        if ((tdsrab_status != TDSRRC_RAB_NOT_FOUND) &&
             (tdsrrc_find_if_voice_rab_exists_in_established_rabs( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id )
               == FALSE))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB %d already exists in EST_RABS!",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id);
         /* send a failure message if the rab in the message  already
           exists in   established_rabs. Later add a check for the rbs
           associated with the rabs so that the failure will be sent only when
           any of the rb-ids match with the rb-ids in established_rab for that
           rab */
          tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
          /* Send the RB Setup Failure Message */
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause
                                    );

        /* Make sure we're back in the initial sub-state */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
          tdsrrcrbe_clear_procedure();
          return;
        }
      }

      /* We have to establish this RAB. First set TDSORDERED_CONFIG
      so that LLC can process the necessary parameters.
      This function will copy all RABs from the rxd msg to oc. */
      oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBE,
                                             tdsrrc_rbe.state_from_msg,
                                             tdsrrc_DL_DCCH_Message_PDU,
                                             (void *)dcch_msg_ptr
                                             );


      switch ( oc_status )
      {
        case TDSORDERED_CONFIG_SET:
  #ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
            if(tdsrrcsib_is_current_event_sib_read_in_dch())
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Force Init GPS ");
               tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
            }
  #endif

          /* Ordered Config has been set. We can setup the
            Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
          /* Call the function that creates and sends a channel
          config request to LLC. */
          tdsrrcrbe_send_channel_config_req_r7( msg_ptr );

          if (((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
              (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
              (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
                tdsrrc_RadioBearerSetup_r5_IEs,new_C_RNTI))
            {
              tdsrrc_rbe.new_crnti_valid = TRUE;
              tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI,
                                  &c_rnti);
              tdsrrc_rbe.new_crnti = (uint16)c_rnti;
            }
            else
            {
              tdsrrc_rbe.new_crnti_valid = FALSE;
            }
            tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
          }
          else
          {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
            if( tdsrrc_rbe.curr_substate != TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF )
#endif/* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
            {
               /* Change the substate since we're waiting for the
                 confirmation from LLC for the channel configs */
               tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
            }
          }
          break;

        case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
          tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
          /* Send the RB Setup Failure Message */
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause
                                  );

          /* Go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;

        case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
          tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
          /* Send the RB Setup Failure Message */
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause
                                  );

          /* Go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;
          
          case TDSORDERED_CONFIG_CONFIGURATION_INVALID:
 
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
           tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
           /* Send the RB Setup Failure Message */
           tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                     tdsrrc_rbe.status.prot_err_cause
                                   );
 
           /* Go back to initial substate */
           tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
 
           /* Indicate that an error occured */
           ordered_config_err_occured = TRUE;
           break;
        case TDSORDERED_CONFIG_NOT_SET_OTHER:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
          /* When LLC sends this, there is a serious problem
          and we'll go back to disconnected state. No need
          to send the failure message here, we can just
          go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;

        default:

          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
          /* Unknown error, just go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;
        }/* end switch */

        if ( ordered_config_err_occured == TRUE )
        {
          /* No need to continue. Clear the procedure
            variables and return. */
          tdsrrcrbe_clear_procedure();
          return;
        }

        if (tdsrrc_rbe.rbe_for_srns_relocation == TRUE)
        {
          tdsrrcsmc_commit_fresh_for_srns_relocation();
        }
    }
    else
    {
      /*If we are here, it means that RABs are not given in RB-Setup.
      Also, SRBs and RB Affected IEs are not given.*/

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"No RB IEs are given in the OTA.");
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
      tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_informationElementMissing;  

      /* Send the RB Setup Failure Message */
      tdsrrcrbe_send_rbs_failure_message(tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);
      /* Make sure we're back in the initial sub-state */
      tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
      tdsrrcrbe_clear_procedure();

      return;          
    }/* end if - RBs need to be reconfigured */
  } /* else if valid rb message */
}/* end function process_rbsetup_message_r6 */



/*===========================================================================

FUNCTION   PROCESS_RBSETUP_R6

DESCRIPTION

  This function processes the Rel 6 RB Setup Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Setup Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -

    TDSORDERED_CONFIG will be set by calling LLC.

    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.

    If TDSORDERED_CONFIG could not be set, a Radio Bearer Setup Failure message
    will be sent to UTRAN.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_process_rbsetup_r6
(
tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_DL_DCCH_Message     *dcch_msg_ptr;   /* Pointer to the RB Setup message */
  tdsrrc_RadioBearerSetup *msg_ptr;    /* Pointer to the RB Setup message        */

  tdsrrc_rab_search_e_type tdsrab_status = TDSRRC_RAB_NOT_FOUND; /* To check if RAB is present in ESTABLISHED_RABS */
  tdsrrcllc_oc_status_e_type oc_status;/* To hold tdsordered_config status          */
  uint32 rab_index=0;               /* Used when more than 1 rab is present in
                                    a RBS message                             */
  tdsrrc_int_c_rnti_type      c_rnti;       /* Local C_RNTI value */
  boolean ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                                 occured while setting tdsordered_config. */
  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL6;

  tdsrrc_RadioBearerSetup_r6_IEs* rbsetup_ptr = NULL;

  tdsrrcsmc_srns_ip_check_status_e_type    ip_check_status = TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;

  tdsrrcrb_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;


  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCEUL:rbsetupr6 cur_substate %d",tdsrrc_rbe.curr_substate);

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Setup message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerSetup);

  /* Store Msg Ptr in a local variable */
  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
    u.r6.radioBearerSetup_r6;

  if (TRUE == tdsrrcrbe_validate_srns_relocation_info(msg_ptr, msg_version))
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
          == tdsrrc_rbe.tr_ids.rejected_transaction)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBE msg - rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
        /* Nothing else to do, just ignore the message and return */
      }
      else
      {
        /* send a reconfig failure messge with invalid
        configuration and
          continue the same substate with any ongoing procedure */
        temp_rejected_transaction = tdsrrc_rbe.tr_ids.rejected_transaction;
        temp_failure_cause.failure_status = tdsrrc_rbe.status.failure_status;
        tdsrrc_rbe.tr_ids.rejected_transaction =
        (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
        if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
        {
          tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        }
        else
        {
          tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
        }
        /*Check whether Cell Update is going on */
        if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
             (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
        {
          if(tdsrrc_rbe.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbe.curr_substate,tdsrrc_rbe.status.failure_status);
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                     tdsrrc_rbe.status.prot_err_cause );
            tdsrrc_rbe.tr_ids.rejected_transaction = temp_rejected_transaction;
            tdsrrc_rbe.status.failure_status = temp_failure_cause.failure_status;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
            (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
            /* Wait until Cell update procedure is completed */
            tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
          }
        }
        else
        {
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause );
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
      /*do not clear the procedure or change the tdsrrc_rbe substate because
      some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
      return;
    }

  } /* End of validation of srns relocation */

  if ((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
      (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
      (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incomatible simultaneous reconfig! Rejecting RBS message");
    if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
         == tdsrrc_rbe.tr_ids.accepted_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBS msg! accepted tr-id%d match!",tdsrrc_rbe.tr_ids.accepted_transaction);
      /* Nothing else to do, just return */
    }
    else if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                == tdsrrc_rbe.tr_ids.rejected_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBS msg! rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
      /* Nothing else to do, just return */
    }
    else
    {
      /* We have received a RBS message with a new transaction id.
      This message should be in our rejected transaction
        list */
      temp_rejected_transaction = tdsrrc_rbe.tr_ids.rejected_transaction;
      temp_failure_cause.failure_status = tdsrrc_rbe.status.failure_status;
      
      tdsrrc_rbe.tr_ids.rejected_transaction =
      (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
      tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;

      /*Check whether Cell Update is going on */
      if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
           (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
      {
        if(tdsrrc_rbe.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbe.curr_substate,tdsrrc_rbe.status.failure_status);
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                   tdsrrc_rbe.status.prot_err_cause );
          tdsrrc_rbe.tr_ids.rejected_transaction = temp_rejected_transaction;
          tdsrrc_rbe.status.failure_status = temp_failure_cause.failure_status;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
          (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
          /* Wait until Cell update procedure is completed */
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        }
      }
      else
      {
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                  tdsrrc_rbe.status.prot_err_cause );
        /*continue the same substate */
      }
    }
    return;
  }

  /* We have received a RB setup message with a new transaction id.
  Accept the transaction Id */
  tdsrrc_rbe.tr_ids.accepted_transaction =
  msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;


#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rbsetup_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rbsetup_ptr->m.activationTimePresent,
         rbsetup_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBE SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbe_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rbsetup_ptr->m.activationTimePresent = 0;
    }
  }

#endif

  tdsrrcrbe_validate_rbsetup_r6(msg_ptr);

  if ( tdsrrc_rbe.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB Message validation failed");
    /* Send the RB Setup Failure Message */
    tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                              tdsrrc_rbe.status.prot_err_cause
                            );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
    tdsrrcrbe_clear_procedure();
  }
  /* If the message is valid, proceed further */
  else
  {
    tdsrrcrbe_indicate_layer1_if_rb_setup_configure_voice_rab();

     /* If new rabs are setup, update mac rab status */

    if(rbsetup_ptr->specificationMode.u.complete.m.rab_InformationSetupListPresent)
    {
      tdsrrcrb_update_mac_rab_status(
        TDSRRC_PROCEDURE_RBE,
        tdsrrc_rbe.rabs.rabs_in_msg[0].cn_domain,
        NULL,
        FALSE);
    }


    /*calculate tdsciphering_activation_cfn only if ciphering is enabled*/
    if (tdsrrcsmc_is_ciphering_enabled() == TDSRRCSMC_SECURITY_STARTED)
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r6_IEs,activationTime))
      {
        /*256 is the CFN rollover length*/
        /*250 is used to activate ciphering after 250 10ms time interval,
        this should give sufficient time for n/w side and ue side to have
        configured ciphering on the rb's successfully*/
        tdsciphering_activation_cfn =
        (uint8) ((rbsetup_ptr->activationTime + 250) % 256);
        /*round it to 8 frame boundary */
        tdsciphering_activation_cfn = (uint8)((tdsciphering_activation_cfn/8)*8);

      }
      else
      {
        /*256 is the CFN rollover length*/
        /*250 is used to activate ciphering after 250 10ms time interval,
        this should give sufficient time for n/w side and ue side to have
        configured ciphering on the rb's successfully*/
        tdsciphering_activation_cfn = (uint8)((tdsseq_get_cfn() + 250) % 256);
        /*round it to 8 frame boundary */
        tdsciphering_activation_cfn = (uint8)((tdsciphering_activation_cfn/8)*8);
      }
    }

    /*call rrcsmc function to update ciphering activation cfn*/
    tdsrrcsmc_update_ciphering_activation_cfn(tdsciphering_activation_cfn);

    /* Check if there is CN information present. */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r6_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      tdsrrc_rbe.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(
        &(rbsetup_ptr->cn_InformationInfo) );
      if (tdsrrc_rbe.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbe.cn_info_saved = TRUE;
      }
    }

    /* Check if there are new RABs to be established or if there are
       affected RB or SRBs to be setup
     */

    if ((rbsetup_ptr->specificationMode.u.complete.m.rab_InformationSetupListPresent) ||
        (((rbsetup_ptr->specificationMode.u.complete.m.rb_InformationAffectedListPresent) &&
          (rbsetup_ptr->specificationMode.u.complete.rb_InformationAffectedList.n != 0 )) ||
        ((rbsetup_ptr->specificationMode.u.complete.m.srb_InformationSetupListPresent) &&
         (rbsetup_ptr->specificationMode.u.complete.srb_InformationSetupList.n != 0))))
    {
      /* Check if the RAB id is already in ESTABLISHED_RABS */
      for (rab_index=0; rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg; rab_index++)
      {
        tdsrab_status = tdsrrc_find_rab_in_established_rabs
          (tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id );
        tdsrrc_rbe.rabs.chan_config_needed[rab_index] = TRUE;
#ifdef FEATURE_SCUDIF_ENHANCEMENT
        #error code not present
#endif

        if ((tdsrab_status != TDSRRC_RAB_NOT_FOUND) &&
             (tdsrrc_find_if_voice_rab_exists_in_established_rabs( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id )
               == FALSE)
            )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB %d already exists in EST_RABS!",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id);
         /* send a failure message if the rab in the message  already
           exists in   established_rabs. Later add a check for the rbs
           associated with the rabs so that the failure will be sent only when
           any of the rb-ids match with the rb-ids in established_rab for that
           rab */
          tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
          /* Send the RB Setup Failure Message */
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause
                                    );

        /* Make sure we're back in the initial sub-state */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
          tdsrrcrbe_clear_procedure();
          return;
        }
      }

      /* We have to establish this RAB. First set TDSORDERED_CONFIG
      so that LLC can process the necessary parameters.
      This function will copy all RABs from the rxd msg to oc. */
      oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBE,
                                             tdsrrc_rbe.state_from_msg,
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

#endif /* FEATURE_TDSCDMA_GPS_CELL_ID_READING */

          /* Ordered Config has been set. We can setup the
            Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
          /* Call the function that creates and sends a channel
          config request to LLC. */
          tdsrrcrbe_send_channel_config_req_r6( msg_ptr );

          if (((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
              (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
              (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
                tdsrrc_RadioBearerSetup_r5_IEs,new_C_RNTI))
            {
              tdsrrc_rbe.new_crnti_valid = TRUE;
              tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI,
                                  &c_rnti);
              tdsrrc_rbe.new_crnti = (uint16)c_rnti;
            }
            else
            {
              tdsrrc_rbe.new_crnti_valid = FALSE;
            }
            tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
          }
          else
          {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
           if( tdsrrc_rbe.curr_substate != TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF )
  #endif/* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
           {
             /* Change the substate since we're waiting for the
               confirmation from LLC for the channel configs */
             tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
           }
          }
          break;

        case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
          tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
          /* Send the RB Setup Failure Message */
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause
                                  );

          /* Go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;

        case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
          tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
          /* Send the RB Setup Failure Message */
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause
                                  );

          /* Go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;

    case TDSORDERED_CONFIG_CONFIGURATION_INVALID:

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
          tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
          /* Send the RB Setup Failure Message */
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause
                                  );

          /* Go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;

        case TDSORDERED_CONFIG_NOT_SET_OTHER:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
          /* When LLC sends this, there is a serious problem
          and we'll go back to disconnected state. No need
          to send the failure message here, we can just
          go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;

        default:

          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
          /* Unknown error, just go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;
        }/* end switch */

        if ( ordered_config_err_occured == TRUE )
        {
          /* No need to continue. Clear the procedure
            variables and return. */
          tdsrrcrbe_clear_procedure();
          return;
        }

        if (tdsrrc_rbe.rbe_for_srns_relocation == TRUE)
        {
          tdsrrcsmc_commit_fresh_for_srns_relocation();
        }
    }
    else
    {
      /*If we are here, it means that RABs are not given in RB-Setup.
      Also, SRBs and RB Affected IEs are not given.*/

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"No RB IEs are given in the OTA.");
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
      tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_informationElementMissing;  

      /* Send the RB Setup Failure Message */
      tdsrrcrbe_send_rbs_failure_message(tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);
      /* Make sure we're back in the initial sub-state */
      tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
      tdsrrcrbe_clear_procedure();

      return;          
    }/* end if - RBs need to be reconfigured */
  } /* else if valid rb message */
}/* end function process_rbsetup_message_r6 */

#ifdef FEATURE_TDSCDMA_REL9
/*===========================================================================
FUNCTION   RRCRBE_SEND_CHANNEL_CONFIG_REQ_R9

DESCRIPTION
  This function sends the TDSRRC_CHANNEL_CONFIG_REQ command to LLC
  to setup the necessary RBs associated with a RAB. If signalling
  Radio bearers are affected and included in the Radio Bearer
  Setup Message, they will also be sent in the same command.

  Currently the messege is not expected to have Signalling
  Radio Bearers for setup. If these are added in the future,
  they will also need to be sent in the same command.

DEPENDENCIES
  TDSORDERED_CONFIG must be set before calling this function.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbe_send_channel_config_req_r9
(
  tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
)
{
  tdsrrc_cmd_type *out_cmd_ptr;  /* To hold the Command                       */
  uint32 rb_count=0;          /* To count the number of RBs in the command */
  uint8  rbe_rb_cnt = 0;

  uint32 rab_index=0;         /* Used when more than one RAB is received in a RBS message */
  uint32 srb_cnt=0;
  tdsrrc_RB_Identity rb_id=0;

  tdsrrc_RB_InformationSetupList_r8 *rb_ptr = NULL; /* Temporary pointer to RB info      */
  tdsrrc_RB_InformationAffectedList_r8 *affected_rb_ptr = NULL;  /* Pointer to RBs to be Reconfigured */
  tdsrrc_RB_InformationReconfigList_r8 *reconfig_rb_ptr = NULL;
  tdsrrc_RAB_InformationSetupList_r8 *rab_info_ptr = NULL;
  tdsrrc_SRB_InformationSetupList_r8        * srb_setup_ptr = NULL;
  tdsrrcrb_directed_cell_info_type        directed_cell_info; /* directed cell info struct */

  tdsrrc_RadioBearerSetup_r9_IEs* rbsetup_ptr = NULL;

  /* Fill in the command id  */
  tdsrrc_cmd.cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;

  /* Set RBSetup Ptr */
  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
    criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.radioBearerSetup_r9;


  /* First fill up the Radio Bearers that need to be setup. For this we need to know how many RBs need to be setup.
  If RABs need to be setup, check how many RBs are mapped to RABs. */
  if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,srb_InformationSetupList))
  {
    /* Get the SRB setup list */
    srb_setup_ptr = &rbsetup_ptr->srb_InformationSetupList;

    while ((srb_setup_ptr->n > srb_cnt) && (srb_cnt < UE_MAX_SRB_SETUP))
    {
      /* Check to see if the RB ID is there ... */
      if (srb_setup_ptr->elem[srb_cnt].m.rb_IdentityPresent)
      {
        rb_id = srb_setup_ptr->elem[srb_cnt].rb_Identity;
      }
      /*If not, then start with the first SRB and get the next SRB ID each time one is not included in the IE.*/
      else
      {
        /* Get the next SRB, note this increments to 1 the first time */
        rb_id++;
      }
  
      /* Fill up the RB id and indicate that it needs to be established */
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rb_id;
      if(tdsrrclcm_get_dl_rlc_lc_id(rb_id) == TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
      {
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSESTABLISH_RB;
      }
      else
      {
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = 
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
      }
      rb_count++;
      srb_cnt++;


    } /* end SRB setup */
  }

  if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,rab_InformationSetupList))
  {
    rab_info_ptr = &rbsetup_ptr->rab_InformationSetupList;
    for (rab_index=0; rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg && rab_info_ptr->n > rab_index; rab_index++)
    {
      if ( tdsrrc_rbe.rabs.chan_config_needed[rab_index] == TRUE)
      {
        rb_ptr = &rab_info_ptr->elem[rab_index].rb_InformationSetupList;

        if ( rb_ptr->n == 0 )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:No RBs present in msg!");
        }
        else
        {
          rbe_rb_cnt = 0;
          while ( rb_ptr->n > rbe_rb_cnt )
          {
            /* Fill up the RB id and indicate that it needs to be established */
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rb_ptr->elem[rbe_rb_cnt].rb_Identity;
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSESTABLISH_RB;
  
            /* Update the local structure so that we know which RB id's we've
                    requested to be setup */
            tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rb_for_rab[rbe_rb_cnt].rb_id = rb_ptr->elem[rbe_rb_cnt].rb_Identity;
  
            /* Increment count of RBs */
            rb_count++;
            rbe_rb_cnt ++;
  
            /* Do a safety check on the count. We can't do more than 32
                     configs in a command */
            if ( rb_count >= TDS_MAX_RB )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:MAX RB count exceeded!");
              break;
            }
            if ( rbe_rb_cnt >= MAX_RB_PER_RAB )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:MAX RBs per RAB count exceeded!");
              break;
            }
  
            /* Go to the next RB */
          }/* end while */
            /* Save the number of RBs for this RAB */
          tdsrrc_rbe.rabs.rabs_in_msg[rab_index].num_rbs_for_rab = rbe_rb_cnt;
        }
      }/* end if rabs_setup_needed */
    }
  }
  /* Now check if the message contains an RB information to be
     reconfig list. If so, we need to reconfigure some RBs */
  if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,rb_InformationReconfigList))
  {
    uint32 local_idx=0;
    TDSRRC_MSG0(MSG_LEGACY_MED,"REL9:RB info reconfig list present");
  
    reconfig_rb_ptr = &rbsetup_ptr->rb_InformationReconfigList;
  
    while ( reconfig_rb_ptr->n > local_idx )
    {
  
      /* Fill up the RB id and indicate that it needs to be reconfigured */
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = reconfig_rb_ptr->elem[local_idx].rb_Identity;
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = 0;
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = 0;
      if((reconfig_rb_ptr->elem[local_idx].m.rlc_InfoPresent)&&
         (reconfig_rb_ptr->elem[local_idx].rlc_Info.m.ul_RLC_ModePresent))
      {
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = TDSMODIFY_RB_CFG;
      }
      if((reconfig_rb_ptr->elem[local_idx].m.rlc_InfoPresent)&&
         (reconfig_rb_ptr->elem[local_idx].rlc_Info.m.dl_RLC_ModePresent))
      {
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
      }
  
      if (reconfig_rb_ptr->elem[local_idx].m.rb_StopContinuePresent)
      {
        /* over-ride rb_config with STOP or continue */
        if (tdsrrc_RB_StopContinue_stopRB == reconfig_rb_ptr->elem[local_idx].rb_StopContinue)
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask |= TDSSTOP_RB_CFG;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask |= TDSSTOP_RB_CFG;
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL8:rb-reconfig-info rb %d to STOP rb_count %d",tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id,rb_count);
        }
        else if (tdsrrc_RB_StopContinue_continueRB == reconfig_rb_ptr->elem[local_idx].rb_StopContinue)
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask |= TDSCONTINUE_RB_CFG;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask |= TDSCONTINUE_RB_CFG;
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL9:rb-reconfig-info rb %d to CONTINUE rb_count %d",tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id,rb_count);
          
        }
      }
  
      /* Increment count of RBs. This is done here since we
      need to use the next index that's not used already. */
      rb_count++;
  
      /* Go to the next affected RB */
      local_idx++;
  
      /* Do a safety check on the count. We can't do more than 32
      configs in a command */
      if ( rb_count >= TDS_MAX_RB )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:MAX RB count exceeded!");
        break;
      }
    } /* end while */
  }
  
  /* Now check if the message contains an RB information to be
   affected list. If so, we need to reconfigure some RBs */
  if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,rb_InformationAffectedList))
  {
   uint32 local_idx=0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:RB info affected list present");
  
    affected_rb_ptr = &rbsetup_ptr->rb_InformationAffectedList;
  
    if ( affected_rb_ptr->n == 0 )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:No affected RBs in msg!");
    }
    else
    {
      while ( affected_rb_ptr->n > local_idx )
      {
  
        /* Fill up the RB id and indicate that it needs to be reconfigured */
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = affected_rb_ptr->elem[local_idx].rb_Identity;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
  
        /* Increment count of RBs. This is done here since we
        need to use the next index that's not used already. */
        rb_count++;
  
        /* Go to the next affected RB */
        local_idx++;
  
        /* Do a safety check on the count. We can't do more than 32
        configs in a command */
        if ( rb_count >= TDS_MAX_RB )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:MAX RB count exceeded!");
          break;
        }
      } /* end while */
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"No affected RB list in msg");
  }/* end if affected list present */


  /* Number of RBs to config. Note rb_count is the total
   of established and reconfigured RBs */
  tdsrrc_cmd.cmd.chan_config_req.num_rb_to_config = (int) rb_count;

  if(rb_count < TDS_MAX_RB)
  {
  tdsrrc_fill_rb_id_chan_req_from_standalone_db(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
  }

  if(tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)
  {
     /* Fill those RBs in chan_config_req for which RLC PDU size has changed and 
     that are already not present in chan_config_req*/
    if(rb_count < TDS_MAX_RB)
    {
    tdsrrc_fill_rb_id_chan_req_from_rb_list_mapped_to_dch(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
    }
  }
 
  /*If state transition is Cell_DCH->Cell_FACH or vice-verca, add all the existing 
  AM PS RBs in chan_config_req*/	
  if(((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_FACH) && (tdstransition_config.toc_usage == 
      TDSTOC_FOR_DCH_AND_FACH)) || ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && (tdsrrc_rbe.state_from_msg 
      == TDSRRC_STATE_CELL_DCH)))
  {
    tdsrrc_fill_rb_id_chan_req_from_est_rabs(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
  }
  /* Check if state change is required */
  if ( (tdsrrc_get_state()) == (tdsrrc_rbe.state_from_msg) )
  {
    tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required = FALSE;
  }
  else
  {
    /* The message indicates that state change is required.
    Indicate to LLC what the next state should be. */
    tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required = TRUE;
    tdsrrc_cmd.cmd.chan_config_req.next_state = tdsrrc_rbe.state_from_msg;
  }

  /* Indicate the id of the procedure sending the command */
  tdsrrc_cmd.cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RBE;

  /* Indicate that we need a confirmation for this command */
  tdsrrc_cmd.cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

  /* We are not required to fill any other data for this command.
   Now, send the command */
  if (((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
      (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
      (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
  {
    /*Initiate the cell selection*/
    tdsrrcrb_send_initiate_cell_selection_req(TDSRRC_PROCEDURE_RBE,
            TDSRRC_TRANSITION_FROM_DCH, tdsrrc_rbe.state_from_msg);

    tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

    /* Start T_305 [periodic cell update] timer */
    tdsrrccu_start_t_305_timer();
  }
  else
  {
    /*check whether any frequency info is present in the message
    and transition is CELL_FACH->CELL_FACH. In this case
    initiate a cell update after channel config is successfully completed */
    if (((tdsrrc_get_state())==TDSRRC_STATE_CELL_FACH)&&
        (tdsrrc_rbe.state_from_msg==TDSRRC_STATE_CELL_FACH))
    {
      if((TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,frequencyInfo)) &&
          (rbsetup_ptr->frequencyInfo.modeSpecificInfo.t == T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd))
      {
        directed_cell_info.freq_info_present = TRUE;
        directed_cell_info.uarfcn_dl = rbsetup_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
      }
      else
      {
        directed_cell_info.freq_info_present = FALSE;
      }
      if (tdsrrcrbe_is_pccpch_info_present(msg_ptr, TDSMSG_REL9))
      {
        directed_cell_info.cpid_present = TRUE;
        directed_cell_info.cell_param_id = rbsetup_ptr->dl_InformationPerRL_List.elem[0].
                                             modeSpecificInfo.u.tdd->u.tdd->cellParametersID;;
      }
      else
      {
        directed_cell_info.cpid_present = FALSE;
      }
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE)
      {
        /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection 
          FACH->FACH ), treat it as un-supported configuration and send failure.*/ 
        if(tdsrrccsp_check_initiate_cell_selection_handling() == FALSE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
       
          tdsrrcrbe_send_initiate_cu_flag = TRUE;
          tdsrrcrbe_delete_crnti          = TRUE;
        }
        else
        {
          tdsrrc_int_c_rnti_type      c_rnti;       /* Local C_RNTI value */
          if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,new_C_RNTI))
          {
            tdsrrc_rbe.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI,
                                &c_rnti);
            tdsrrc_rbe.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_rbe.new_crnti_valid = FALSE;
          }
          tdsrrcllcoc_update_toc_with_oc_from_fach();
          
          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req(TDSRRC_PROCEDURE_RBE,
                                                  TDSRRC_TRANSITION_FROM_FACH,
                                                  tdsrrc_rbe.state_from_msg);
       
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
              
          return;
        }
      }

#else/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if (tdsrrcrb_validate_directed_cell_info(directed_cell_info))
      {
        /*directed cell info matched. Cell update is not required */
        tdsrrcrbe_send_initiate_cu_flag = FALSE;
      }
      else
      {
        /*directed cell info do not match. Cell update is required */
        tdsrrcrbe_send_initiate_cu_flag = TRUE;
        tdsrrcrbe_delete_crnti          = TRUE;
      }
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
    } /* if CELL_FACH->CELL_FACH reconfiguration */

    /* Get a Command buffer and copy the local command to the buffer so as to 
    enqueue it on the internal queue. */
    if ((out_cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL)
    {
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
      memscpy(out_cmd_ptr,sizeof(tdsrrc_cmd_int_type),&tdsrrc_cmd,sizeof(tdsrrc_cmd_int_type));
#else
      out_cmd_ptr->cmd_hdr = tdsrrc_cmd.cmd_hdr;
      out_cmd_ptr->cmd     = tdsrrc_cmd.cmd;
#endif
      tdsrrc_put_int_cmd( out_cmd_ptr );
    }
    else
    {
      ERR_FATAL("Could not get internal cmd buf",0,0,0);
    }
  }
}

/*===========================================================================
FUNCTION   VALIDATE_RBSETUP_R9

DESCRIPTION
  This function validates the received R8 RB Setup Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Setup Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbe_validate_rbsetup_r9
(
  tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
)
{
  tdsrrc_RadioBearerSetup_r9_IEs* rbsetup_ptr = NULL;

  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();

  /* Assume the message is valid for now */
  tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_VALID;

  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
    u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.radioBearerSetup_r9;

  switch (rbsetup_ptr->rrc_StateIndicator)
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      tdsrrc_rbe.state_from_msg = TDSRRC_STATE_CELL_DCH;
      break;

    case tdsrrc_RRC_StateIndicator_cell_FACH:
      tdsrrc_rbe.state_from_msg = TDSRRC_STATE_CELL_FACH;
      break;

    default: /* Just to satisfy LINT */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:Invalid RRC state from Msg");
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      break;

  } /* End of switch */

  if ( tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
    return;
  }
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
         u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9,
      radioBearerSetup_r9_add_ext))
     
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: radioBearerSetup_r9_add_extPresent not yet supported");
    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    return;
  } 

  /* Check if there are new RABs to be established */
  if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,rab_InformationSetupList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:RAB Info Setup List present.");
    if(tdsrrcrbe_prevalidate_rab_setup_list_r8(&rbsetup_ptr->rab_InformationSetupList) == FAILURE)
    {
      return;
    }
  }
  else
  {
    /*There is no RAB to be setup in this message.*/
    tdsrrc_rbe.rabs.num_rabs_in_msg = 0;
  }  

#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,dl_CounterSynchronisationInfo))
#else
    if (tdsrrcrbe_validate_srns_relocation_info(msg_ptr, TDSMSG_REL9))
#endif

  {
    /*check whether the SRNS relocation is within Cell_DCH or not.
      SRNS relocation information can come in reconfiguration messages
      only when the state transition is Cell_DCH->Cell_DCH */
    if (!((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH) &&
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"REL9:SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbe.state_from_msg);
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }

    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has
       PDCP info list. If it is true then this configuration is unsupported */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rbsetup_ptr->dl_CounterSynchronisationInfo,
		rb_WithPDCP_InfoList)) ||
        (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CounterSynchronisationInfo, 
        tdsrrc_DL_CounterSynchronisationInfo_r5,rb_PDCPContextRelocationList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:PDCP info in DL counter sync info not supported");
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation.
      Set the flag that indicates that this procedure
      involves SRNS relocation */
      tdsrrc_rbe.rbe_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */
      if (TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,new_U_RNTI))
      {
        tdsrrc_rbe.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(rbsetup_ptr->new_U_RNTI), &tdsrrc_rbe.new_urnti);
      }
    }
  }

  /* Process RAB reconfig before RAB setup because we want to catch NW's which
    are trying to setup and reconfig RAB with the same message */
  if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,rab_InformationReconfigList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:RAB Info Reconfig List is present.");

    tdsrrc_rbe.status.failure_status = tdsrrc_update_rab_info_reconfig_param_r8(
      &rbsetup_ptr->rab_InformationReconfigList, &tdsrrc_rbe.rabs, &nas_sync_ind);
    if( tdsrrc_rbe.status.failure_status != TDSRRCRB_MSG_VALID) 
    {
      return;
    }
  }

  if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,rab_InformationSetupList))
  {
    if(tdsrrcrbe_validate_rab_setup_list_r8(&rbsetup_ptr->rab_InformationSetupList, 
        tdsrrc_rbe.rabs.num_rabs_in_msg, nas_sync_ind) == FAILURE)
    {
      return;
    }
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:%d RABs  in an RBSetup",tdsrrc_rbe.rabs.num_rabs_in_msg);

  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch */
  tdsrrc_rbe.trans_to_dch_ciph_config_needed = FALSE;

  if((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH) && (tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH))
  {
    //check to see if dl-dpch-common info is present while going to dch
    if((TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,dl_CommonInformation)) && 
       (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CommonInformation,
       tdsrrc_DL_CommonInformation_r8,dl_dpchInfoCommon)) &&
       (T_tdsrrc_DL_CommonInformation_r8_dl_dpchInfoCommon_dl_DPCH_InfoCommon
       == rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.t))
    {
      //check if ciphering is active in mac for cs domain, if it is then,
      //start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
      if(tdsrrcsmc_tm_ciph_active() == TRUE)
      {
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:To-DCH-Step-1 config activated");
        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon,tdsrrc_DL_DPCH_InfoCommon_r6,mac_d_HFN_initial_value)),
              &rbsetup_ptr->dl_CommonInformation.dl_dpchInfoCommon.u.
               dl_DPCH_InfoCommon->mac_d_HFN_initial_value,
              &mac_d_hfn_l);
        if (loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:get_mac_d_hfn failed");
        }
        if(tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:SMC process ciphering config for HHO failed");
        }
        tdsrrc_rbe.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:To-DCH-Step-1 not needed");
      }
    }
  }

  /* If UTRAN DRX coefficient is not present then it is invalid configuration */
  if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_rbe.state_from_msg  == TDSRRC_STATE_URA_PCH))
  {

    if (!(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL9:UTRAN DRX Coef not preset,State:%d",tdsrrc_rbe.state_from_msg);
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,rb_InformationReconfigList))
  {
    /* Save the linked list head pointer */
    if (FAILURE == tdsrrcllc_validate_rb_info_reconfig_list_r8(&rbsetup_ptr->rb_InformationReconfigList))
    {
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }

  if (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH)
  {
    /*should we do it after getting l2ack for response??*/
    if (!(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,ura_Identity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:URA ID not present");
      tdsrrccu_clear_ura_id ();
    }
    else
    {
      /*URA ID present in the message set ura_identity variable with this value */
      if (rbsetup_ptr->ura_Identity.numbits == 16)
      {
        tdsrrccu_update_ura_id (&(rbsetup_ptr->ura_Identity));
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL9:Invalid URA ID length%d",rbsetup_ptr->ura_Identity.numbits);
        tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
        tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
        return;
      }
    }
  }

  return;
}

/*===========================================================================
FUNCTION   PROCESS_RBSETUP_R9

DESCRIPTION
  This function processes the Rel 8 RB Setup Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Setup Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -

    TDSORDERED_CONFIG will be set by calling LLC.

    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.

    If TDSORDERED_CONFIG could not be set, a Radio Bearer Setup Failure message
    will be sent to UTRAN.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void tdsrrcrbe_process_rbsetup_r9
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_DL_DCCH_Message     *dcch_msg_ptr;   /* Pointer to the RB Setup message */
  tdsrrc_RadioBearerSetup *msg_ptr;    /* Pointer to the RB Setup message        */

  tdsrrc_rab_search_e_type tdsrab_status = TDSRRC_RAB_NOT_FOUND; /* To check if RAB is present in ESTABLISHED_RABS */
  tdsrrcllc_oc_status_e_type oc_status;/* To hold tdsordered_config status          */
  uint32 rab_index=0;               /* Used when more than 1 rab is present in
                                    a RBS message                             */
  tdsrrc_int_c_rnti_type      c_rnti;       /* Local C_RNTI value */
  boolean ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                                 occured while setting tdsordered_config. */
  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL9;

  tdsrrc_RadioBearerSetup_r9_IEs* rbsetup_ptr = NULL;

  tdsrrcsmc_srns_ip_check_status_e_type    ip_check_status = TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;
  
  tdsrrcrb_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;


  TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:RRC cur_substate %d",tdsrrc_rbe.curr_substate);

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Setup message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerSetup);

  /* Store Msg Ptr in a local variable */
  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
    u.criticalExtensions.u.r9.radioBearerSetup_r9;

  if (TRUE == tdsrrcrbe_validate_srns_relocation_info(msg_ptr, msg_version))
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
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:IP check failed. Discarding RBS msg");
      break;

    case TDSRRCSMC_SRNS_SECURITY_INFO_INVALID:
    case TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION:
      if (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier
          == tdsrrc_rbe.tr_ids.rejected_transaction)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:Duplicate RBE msg - rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
        /* Nothing else to do, just ignore the message and return */
      }
      else
      {
        /* send a reconfig failure messge with invalid
        configuration and
          continue the same substate with any ongoing procedure */
        temp_rejected_transaction = tdsrrc_rbe.tr_ids.rejected_transaction;
        temp_failure_cause.failure_status = tdsrrc_rbe.status.failure_status;
        
        tdsrrc_rbe.tr_ids.rejected_transaction =
        (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
        if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
        {
          tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        }
        else
        {
          tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
        }
        /*Check whether Cell Update is going on */
        if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
             (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
        {
          if(tdsrrc_rbe.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbe.curr_substate,tdsrrc_rbe.status.failure_status);
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                     tdsrrc_rbe.status.prot_err_cause );
            
            tdsrrc_rbe.tr_ids.rejected_transaction = temp_rejected_transaction;
            tdsrrc_rbe.status.failure_status = temp_failure_cause.failure_status;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:Cell Update Started! Register for CU CNF");
            (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
            /* Wait until Cell update procedure is completed */
            tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
          }
        }
        else
        {
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause );
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
      /*do not clear the procedure or change the tdsrrc_rbe substate because
      some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
      return;
    }

  } /* End of validation of srns relocation */

  if ((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
      (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
      (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:Incomatible simultaneous reconfig! Rejecting RBS message");
    if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
         == tdsrrc_rbe.tr_ids.accepted_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:Duplicate RBS msg! accepted tr-id%d match!",tdsrrc_rbe.tr_ids.accepted_transaction);
      /* Nothing else to do, just return */
    }
    else if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                == tdsrrc_rbe.tr_ids.rejected_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:Duplicate RBS msg! rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
      /* Nothing else to do, just return */
    }
    else
    {
      /* We have received a RBS message with a new transaction id.
      This message should be in our rejected transaction
        list */
      temp_rejected_transaction = tdsrrc_rbe.tr_ids.rejected_transaction;
      temp_failure_cause.failure_status = tdsrrc_rbe.status.failure_status;  
      tdsrrc_rbe.tr_ids.rejected_transaction =
      (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
      tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;

      /*Check whether Cell Update is going on */
      if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
           (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
      {
        if(tdsrrc_rbe.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbe.curr_substate,tdsrrc_rbe.status.failure_status);
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                   tdsrrc_rbe.status.prot_err_cause );
          tdsrrc_rbe.tr_ids.rejected_transaction = temp_rejected_transaction;
          tdsrrc_rbe.status.failure_status = temp_failure_cause.failure_status;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:Cell Update Started! Register for CU CNF");
          (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
          /* Wait until Cell update procedure is completed */
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
          
        }
      }
      else
      {
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                  tdsrrc_rbe.status.prot_err_cause );
        /*continue the same substate */
      }
    }
    return;
  }

  /* We have received a RB setup message with a new transaction id.
  Accept the transaction Id */
  tdsrrc_rbe.tr_ids.accepted_transaction =
  msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;


#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rbsetup_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rbsetup_ptr->m.activationTimePresent,
         rbsetup_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBE SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbe_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rbsetup_ptr->m.activationTimePresent = 0;
    }
  }

#endif


  tdsrrcrbe_validate_rbsetup_r9(msg_ptr);

  if ( tdsrrc_rbe.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:RB Message validation failed");
    /* Send the RB Setup Failure Message */
    tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
    tdsrrcrbe_clear_procedure();
  }
  /* If the message is valid, proceed further */
  else
  {
    tdsrrcrbe_indicate_layer1_if_rb_setup_configure_voice_rab();

     /* If new rabs are setup, update mac rab status */
    if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,rab_InformationSetupList))
    {
      tdsrrcrb_update_mac_rab_status(TDSRRC_PROCEDURE_RBE, tdsrrc_rbe.rabs.rabs_in_msg[0].cn_domain,NULL,FALSE);
    }

    /*calculate tdsciphering_activation_cfn only if ciphering is enabled*/
    if (tdsrrcsmc_is_ciphering_enabled() == TDSRRCSMC_SECURITY_STARTED)
    {
      if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,activationTime))
      {
        /*256 is the CFN rollover length*/
        /*250 is used to activate ciphering after 250 10ms time interval,
        this should give sufficient time for n/w side and ue side to have
        configured ciphering on the rb's successfully*/
        tdsciphering_activation_cfn =
        (uint8) ((rbsetup_ptr->activationTime + 250) % 256);
        /*round it to 8 frame boundary */
        tdsciphering_activation_cfn = (uint8)((tdsciphering_activation_cfn/8)*8);

      }
      else
      {
        /*256 is the CFN rollover length*/
        /*250 is used to activate ciphering after 250 10ms time interval,
        this should give sufficient time for n/w side and ue side to have
        configured ciphering on the rb's successfully*/
        tdsciphering_activation_cfn = (uint8)((tdsseq_get_cfn() + 250) % 256);
        /*round it to 8 frame boundary */
        tdsciphering_activation_cfn = (uint8)((tdsciphering_activation_cfn/8)*8);
      }
    }

    /*call rrcsmc function to update ciphering activation cfn*/
    tdsrrcsmc_update_ciphering_activation_cfn(tdsciphering_activation_cfn);

    /* Check if there is CN information present. */
    if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:Saving CN info for NAS");
      tdsrrc_rbe.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info_r6(
        &(rbsetup_ptr->cn_InformationInfo) );
      if (tdsrrc_rbe.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbe.cn_info_saved = TRUE;
      }
    }


    /* Check if there are new RABs to be established or if there are
       affected RB or SRBs to be setup */

    if ((TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,rab_InformationSetupList)) ||
        (((TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,rb_InformationAffectedList))  
          ) ||
        ((TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,srb_InformationSetupList)) 
         )))
    {
      /* Check if the RAB id is already in ESTABLISHED_RABS */
      for (rab_index=0; rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg; rab_index++)
      {
        tdsrab_status = tdsrrc_find_rab_in_established_rabs(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id );
        tdsrrc_rbe.rabs.chan_config_needed[rab_index] = TRUE;

        if ((tdsrab_status != TDSRRC_RAB_NOT_FOUND) &&
             (tdsrrc_find_if_voice_rab_exists_in_established_rabs( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id )
               == FALSE))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:RAB %d already exists in EST_RABS!",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id);
         /* send a failure message if the rab in the message  already
           exists in   established_rabs. Later add a check for the rbs
           associated with the rabs so that the failure will be sent only when
           any of the rb-ids match with the rb-ids in established_rab for that
           rab */
          tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
          /* Send the RB Setup Failure Message */
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause
                                    );

        /* Make sure we're back in the initial sub-state */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
          tdsrrcrbe_clear_procedure();
          return;
        }
      }

      /* First set TDSORDERED_CONFIG so that LLC can process the necessary parameters.
      This function will copy all RABs from the rxd msg to oc. */
      oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBE, tdsrrc_rbe.state_from_msg,
                                             tdsrrc_DL_DCCH_Message_PDU, (void *)dcch_msg_ptr);

      switch ( oc_status )
      {
        case TDSORDERED_CONFIG_SET:
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
          if(tdsrrcsib_is_current_event_sib_read_in_dch())
          {
             TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:RRCGPS: Force Init GPS ");
             tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH);
          }
#endif /*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/

          /* Ordered Config has been set. We can setup the
            Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */

          TDSRRC_MSG0(MSG_LEGACY_MED,"REL9:Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
          tdsrrcrbe_send_channel_config_req_r9(msg_ptr);

          if(((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
              (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
              (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
          {
            if(TDSRRCRB_R9_MSG_IE_PRESENT(rbsetup_ptr,tdsrrc_RadioBearerSetup_r9_IEs,new_C_RNTI))
            {
              tdsrrc_rbe.new_crnti_valid = TRUE;
              tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI, &c_rnti);
              tdsrrc_rbe.new_crnti = (uint16)c_rnti;
            }
            else
            {
              tdsrrc_rbe.new_crnti_valid = FALSE;
            }
            tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
          }
          else
          {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
            if(tdsrrc_rbe.curr_substate != TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF)
#endif/* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
            {
               /*Change the substate since we're waiting for the confirmation from LLC for the channel configs */
               tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
            }
          }
          break;

        case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:LLC returned simultaneous reconfig!");
          tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
          /* Send the RB Setup Failure Message */
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);

          /* Go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;

        case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:LLC returned unsupported config!");
          tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
          /* Send the RB Setup Failure Message */
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);

          /* Go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;
          
          case TDSORDERED_CONFIG_CONFIGURATION_INVALID:
 
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:LLC returned invalid config!");
           tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
           /* Send the RB Setup Failure Message */
           tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);
 
           /* Go back to initial substate */
           tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
 
           /* Indicate that an error occured */
           ordered_config_err_occured = TRUE;
           break;
        case TDSORDERED_CONFIG_NOT_SET_OTHER:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:LLC returned other failure!");
          /* When LLC sends this, there is a serious problem
          and we'll go back to disconnected state. No need
          to send the failure message here, we can just
          go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;

        default:

          TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:Unknown return value from LLC");
          /* Unknown error, just go back to initial substate */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* Indicate that an error occured */
          ordered_config_err_occured = TRUE;
          break;
        }/* end switch */

        if ( ordered_config_err_occured == TRUE )
        {
          /* No need to continue. Clear the procedure
            variables and return. */
          tdsrrcrbe_clear_procedure();
          return;
        }

        if (tdsrrc_rbe.rbe_for_srns_relocation == TRUE)
        {
          tdsrrcsmc_commit_fresh_for_srns_relocation();
        }
    }
    else
    {
      /*If we are here, it means that RABs are not given in RB-Setup.
      Also, SRBs and RB Affected IEs are not given.*/
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:No RB IEs are given in the OTA.");
      tdsrrc_rbe.status.failure_status= TDSRRCRB_MSG_INVALID_CONFIG;
      tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_informationElementMissing;  

      /* Send the RB Setup Failure Message */
      tdsrrcrbe_send_rbs_failure_message(tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);

      /* Make sure we're back in the initial sub-state */
      tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
      tdsrrcrbe_clear_procedure();

      return;  
    }/* end if - RBs need to be reconfigured */
  } /* else if valid rb message */
}/* end function process_rbsetup_message_r6 */

#endif /* FEATURE_TDSCDMA_REL9 */

/*===========================================================================

FUNCTION   tdsrrcrbe_check_and_get_primary_plmn_id

DESCRIPTION

  This function will check whether primary plmn id is present 
  in r3,r5 path of RBE message. If present then it will return 
  primary plmn id.
        
DEPENDENCIES
  None.

RETURN VALUE
TRUE : IF Primary PLMN I.D is present in RBE message in r3,r5 path
FFALSE: Otherwise

SIDE EFFECTS

  None.

===========================================================================*/
static boolean tdsrrcrbe_check_and_get_primary_plmn_id
(
  /* Pointer to the RBE message */
  tdsrrc_RadioBearerSetup *msg_ptr,
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
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBE message is NULL");
    return FALSE;
  }
  switch(msg_version)
  {
    case TDSMSG_REL4:
      /*Check to see if primary PLMN I.D is present in RBE message ext
       * tdsrrc_RadioBearerSetup-v690ext which is present in 
       * v690NonCriticalExtensions in r99 path
       */
      if((msg_ptr->u.later_than_r3.criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.m.
              v5d0NonCriticalExtenstionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.v5d0NonCriticalExtenstions.m.
              v690NonCriticalExtensionsPresent) &&
        (msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
                 v590NonCriticalExtensions.v5d0NonCriticalExtenstions.v690NonCriticalExtensions.radioBearerSetup_v690ext.m.primary_plmn_IdentityPresent))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:RBE msg contains primary_plmn_Id in r99 path");
        primary_plmn_Id_present = TRUE;
        *primary_plmn_Identity = msg_ptr->u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
                 v590NonCriticalExtensions.v5d0NonCriticalExtenstions.v690NonCriticalExtensions.radioBearerSetup_v690ext.primary_plmn_Identity;
      }
      break;
  
    case TDSMSG_REL5:
    /*Check to see if primary PLMN I.D is present in RBE message ext
     * tdsrrc_RadioBearerSetup-v690ext which is present in 
     * v690NonCriticalExtensions in r5 path
     */
    if((msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.m.v5d0NonCriticalExtenstionsPresent)&&
       (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.
         v5d0NonCriticalExtenstions.m.v690NonCriticalExtensionsPresent)&&
       (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.
         v5d0NonCriticalExtenstions.v690NonCriticalExtensions.
         radioBearerSetup_v690ext.m.primary_plmn_IdentityPresent)
      )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:RBE msg contains primary_plmn_Id in r5 path");
      primary_plmn_Id_present = TRUE;
      *primary_plmn_Identity = msg_ptr->u.later_than_r3.criticalExtensions.u.
             criticalExtensions.u.r5.v5d0NonCriticalExtenstions.v690NonCriticalExtensions.
             radioBearerSetup_v690ext.primary_plmn_Identity;
    }
    break;
    
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"NWS: Msg version = %d,Not supported",msg_version);
      break;
  }
  
  return primary_plmn_Id_present;
}
/*===========================================================================

FUNCTION   PROCESS_RBSETUP_R5

DESCRIPTION

  This function processes the Rel 5 RB Setup Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Setup Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -

    TDSORDERED_CONFIG will be set by calling LLC.

    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.

    If TDSORDERED_CONFIG could not be set, a Radio Bearer Setup Failure message
    will be sent to UTRAN.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_process_rbsetup_r5
(
tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_DL_DCCH_Message     *dcch_msg_ptr; /* Pointer to the RB Setup message */
  tdsrrc_RadioBearerSetup *msg_ptr;    /* Pointer to the RB Setup message        */

  tdsrrc_rab_search_e_type tdsrab_status = TDSRRC_RAB_NOT_FOUND; /* To check if RAB is present in ESTABLISHED_RABS */
  tdsrrcllc_oc_status_e_type oc_status;/* To hold tdsordered_config status          */
  uint32 rab_index=0;               /* Used when more than 1 rab is present in
                                    a RBS message                             */
  tdsrrc_int_c_rnti_type      c_rnti;       /* Local C_RNTI value */
  boolean ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                    occured while setting tdsordered_config. */

  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL5;

  tdsrrc_RadioBearerSetup_r5_IEs* rbsetup_ptr = NULL;

  tdsrrcsmc_srns_ip_check_status_e_type    ip_check_status = TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;

   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  boolean primary_plmn_Id_present = FALSE;
   /*Variable to hold primary plmn i.d if present in OTA msg*/
  tdsrrc_PLMN_Identity primary_plmn_Identity;
   
  tdsrrcrb_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;


  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:rbsetupr5 cur_substate %d",tdsrrc_rbe.curr_substate);

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Setup message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerSetup);


  /* The first thing to do is check if Rel 5 contents are present */
  if ((msg_ptr->u.later_than_r3.criticalExtensions.t ==
       T_tdsrrc_RadioBearerSetup_criticalExtensions_5_criticalExtensions) &&
      (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.t ==
       T_tdsrrc_RadioBearerSetup_criticalExtensions_5_r5))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:rbsetupr5 rcvd");

    /* Store Msg Ptr in a local variable */
    rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.
                  u.r5.radioBearerSetup_r5;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported RB Setup t:%d",msg_ptr->t);
    temp_failure_cause.failure_status = tdsrrc_rbe.status.failure_status;
    temp_failure_cause.prot_err_cause = tdsrrc_rbe.status.prot_err_cause;
    
    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
    tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;

    if ((msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
        == tdsrrc_rbe.tr_ids.rejected_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBS msg! rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
      /* Nothing else to do, just ignore the message and return */
    }
    else
    {
      temp_rejected_transaction = tdsrrc_rbe.tr_ids.rejected_transaction;
      tdsrrc_rbe.tr_ids.rejected_transaction = msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;

      /*Check whether Cell Update is going on */
      if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
         (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
      {
        if(tdsrrc_rbe.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbe.curr_substate,tdsrrc_rbe.status.failure_status);
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                   tdsrrc_rbe.status.prot_err_cause );
          tdsrrc_rbe.status.failure_status = temp_failure_cause.failure_status;
          tdsrrc_rbe.status.prot_err_cause = temp_failure_cause.prot_err_cause;
          tdsrrc_rbe.tr_ids.rejected_transaction = temp_rejected_transaction;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
          (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);

          /* Wait until Cell update procedure is completed */
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        }
      }
      else
      {
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                tdsrrc_rbe.status.prot_err_cause );
      }
    }

    return;
  }

  if (TRUE == tdsrrcrbe_validate_srns_relocation_info(msg_ptr, msg_version))
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
          == tdsrrc_rbe.tr_ids.rejected_transaction)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBE msg - rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
        /* Nothing else to do, just ignore the message and return */
      }
      else
      {
        /* send a reconfig failure messge with invalid
        configuration and
          continue the same substate with any ongoing procedure */
        temp_failure_cause.failure_status = tdsrrc_rbe.status.failure_status;
        temp_rejected_transaction = tdsrrc_rbe.tr_ids.rejected_transaction;
        
        tdsrrc_rbe.tr_ids.rejected_transaction =
        (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
        if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
        {
          tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
        }
        else
        {
          tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
        }
        /*Check whether Cell Update is going on */
        if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
             (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
        {
          if(tdsrrc_rbe.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbe.curr_substate,tdsrrc_rbe.status.failure_status);
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                     tdsrrc_rbe.status.prot_err_cause );
            tdsrrc_rbe.status.failure_status =  temp_failure_cause.failure_status;
            tdsrrc_rbe.tr_ids.rejected_transaction = temp_rejected_transaction;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
            (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
            /* Wait until Cell update procedure is completed */
            tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
          }
        }
        else
        {
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause );
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
      /*do not clear the procedure or change the tdsrrc_rbe substate because
      some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
      return;
    }

  } /* End of validation of srns relocation */

  if ((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
      (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
      (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incomatible simultaneous reconfig! Rejecting RBS message");
    if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
         == tdsrrc_rbe.tr_ids.accepted_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBS msg! accepted tr-id%d match!",tdsrrc_rbe.tr_ids.accepted_transaction);
      /* Nothing else to do, just return */
    }
    else if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                == tdsrrc_rbe.tr_ids.rejected_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBS msg! rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
      /* Nothing else to do, just return */
    }
    else
    {
      /* We have received a RBS message with a new transaction id.
      This message should be in our rejected transaction
        list */
      tdsrrc_rbe.tr_ids.rejected_transaction =
      (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
      tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;

      /*Check whether Cell Update is going on */
      if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
           (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
        (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
        /* Wait until Cell update procedure is completed */
        tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      } else
      {
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                  tdsrrc_rbe.status.prot_err_cause );
        /*continue the same substate */
      }
    }
    return;
  }

  /* We have received a RB setup message with a new transaction id.
  Accept the transaction Id */
  tdsrrc_rbe.tr_ids.accepted_transaction =
  msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rbsetup_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rbsetup_ptr->m.activationTimePresent,
         rbsetup_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBE SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbe_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rbsetup_ptr->m.activationTimePresent = 0;
    }
  }

#endif

  tdsrrcrbe_validate_rbsetup_r5(msg_ptr);

  if ( tdsrrc_rbe.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB Message validation failed");

    /* Send the RB Setup Failure Message */
    tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                              tdsrrc_rbe.status.prot_err_cause
                            );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
    tdsrrcrbe_clear_procedure();
  }
  /* If the message is valid, proceed further */
  else
  {
    tdsrrcrbe_indicate_layer1_if_rb_setup_configure_voice_rab();

    /* If new rabs are setup, update mac rab status */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
        tdsrrc_RadioBearerSetup_r5_IEs,rab_InformationSetupList))
    {
      tdsrrcrb_update_mac_rab_status(
        TDSRRC_PROCEDURE_RBE,
        tdsrrc_rbe.rabs.rabs_in_msg[0].cn_domain,
        NULL,
        FALSE);
    }



    /*calculate tdsciphering_activation_cfn only if ciphering is enabled*/
    if (tdsrrcsmc_is_ciphering_enabled() == TDSRRCSMC_SECURITY_STARTED)
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
        tdsrrc_RadioBearerSetup_r5_IEs,activationTime))
      {
        /*256 is the CFN rollover length*/
        /*250 is used to activate ciphering after 250 10ms time interval,
        this should give sufficient time for n/w side and ue side to have
        configured ciphering on the rb's successfully*/
        tdsciphering_activation_cfn =
        (uint8) ((rbsetup_ptr->activationTime + 250) % 256);
        /*round it to 8 frame boundary */
        tdsciphering_activation_cfn = (uint8)((tdsciphering_activation_cfn/8)*8);

      }
      else
      {
        /*256 is the CFN rollover length*/
        /*250 is used to activate ciphering after 250 10ms time interval,
        this should give sufficient time for n/w side and ue side to have
        configured ciphering on the rb's successfully*/
        tdsciphering_activation_cfn = (uint8)((tdsseq_get_cfn() + 250) % 256);
        /*round it to 8 frame boundary */
        tdsciphering_activation_cfn = (uint8)((tdsciphering_activation_cfn/8)*8);
      }
    }

    /*call rrcsmc function to update ciphering activation cfn*/
    tdsrrcsmc_update_ciphering_activation_cfn(tdsciphering_activation_cfn);

    /* Check if there is CN information present. */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r5_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      /*Check to see if primary PLMN I.D is present in RBE message ext
       * radioBearerSetup-v690ext which is present in 
       * v690NonCriticalExtensions
       */
       primary_plmn_Id_present = tdsrrcrbe_check_and_get_primary_plmn_id(msg_ptr,
                                                                  &primary_plmn_Identity,TDSMSG_REL5);
      tdsrrc_rbe.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info(
                     &(rbsetup_ptr->cn_InformationInfo) 
                     ,primary_plmn_Id_present
                     ,&primary_plmn_Identity
          );
      if (tdsrrc_rbe.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbe.cn_info_saved = TRUE;
      }

    }

    /* Check if there are new RABs to be established */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
        tdsrrc_RadioBearerSetup_r5_IEs,rab_InformationSetupList))
    {
      /* Check if the RAB id is already in ESTABLISHED_RABS */
      for (rab_index=0; rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg; rab_index++)
      {
        tdsrab_status = tdsrrc_find_rab_in_established_rabs
                     ( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id );
        tdsrrc_rbe.rabs.chan_config_needed[rab_index] = TRUE;
        if (tdsrab_status != TDSRRC_RAB_NOT_FOUND)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB %d already exists in EST_RABS!",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id);
          break;
        }
      }

#ifdef FEATURE_SCUDIF_ENHANCEMENT
      #error code not present
#endif

      if ( (tdsrab_status == TDSRRC_RAB_NOT_FOUND) || 
           (tdsrrc_find_if_voice_rab_exists_in_established_rabs( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id )
               == TRUE))
      {
        /* We have to establish this RAB. First set TDSORDERED_CONFIG
        so that LLC can process the necessary parameters.
          This function will copy all RABs from the rxd msg to oc. */
        oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBE,
                                               tdsrrc_rbe.state_from_msg,
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
            /* Ordered Config has been set. We can setup the
              Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
            /* Call the function that creates and sends a channel
            config request to LLC. */
            tdsrrcrbe_send_channel_config_req_r5( msg_ptr );
  
            if (((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
                (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
                (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
            {
              if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
                  tdsrrc_RadioBearerSetup_r5_IEs,new_C_RNTI))
              {
                tdsrrc_rbe.new_crnti_valid = TRUE;
                tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI,
                                    &c_rnti);
                tdsrrc_rbe.new_crnti = (uint16)c_rnti;
              }
              else
              {
                tdsrrc_rbe.new_crnti_valid = FALSE;
              }
              tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
            }
            else
            {
/* for "RRC_Issues_Management_QC" No.22 begin */
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
              if( tdsrrc_rbe.curr_substate != TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF )
#endif/* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
              {
              /* Change the substate since we're waiting for the
                confirmation from LLC for the channel configs */
                tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
              }
            }
/* for "RRC_Issues_Management_QC" No.22 end */
            break;
  
          case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
            tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
            /* Send the RB Setup Failure Message */
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                      tdsrrc_rbe.status.prot_err_cause
                                    );
  
            /* Go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
  
          case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
            tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
            /* Send the RB Setup Failure Message */
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                      tdsrrc_rbe.status.prot_err_cause
                                    );
  
            /* Go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;

       case TDSORDERED_CONFIG_CONFIGURATION_INVALID:
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
            tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
            /* Send the RB Setup Failure Message */
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                      tdsrrc_rbe.status.prot_err_cause
                                    );
  
            /* Go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
  
          case TDSORDERED_CONFIG_NOT_SET_OTHER:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
            /* When LLC sends this, there is a serious problem
            and we'll go back to disconnected state. No need
            to send the failure message here, we can just
            go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
  
          default:
  
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
            /* Unknown error, just go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
        }/* end switch */

        if ( ordered_config_err_occured == TRUE )
        {
          /* No need to continue. Clear the procedure
            variables and return. */
          tdsrrcrbe_clear_procedure();
          return;
        }

        if (tdsrrc_rbe.rbe_for_srns_relocation == TRUE)
        {
          tdsrrcsmc_commit_fresh_for_srns_relocation();
        }

      }
      else
      {
        /* send a failure message if the rab in the message  already
        exists in established_rabs. Later add a check for the rbs
        associated with the rabs so that the failure will be sent only when
        any of the rb-ids match with the rb-ids in established_rab for that
          rab */
        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        /* Send the RB Setup Failure Message */
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                  tdsrrc_rbe.status.prot_err_cause
                                );

        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
        tdsrrcrbe_clear_procedure();
        return;
      }
    }




    /* If RABs need to be setup, the RBs to be setup for
    that RAB and any other RBs to be re-configured as indicated
    by the message will be sent in the same channel config
    command. But if no new RBs have been setup, then we
    need to send a separate command for any RBs that
    need to be re-configured */
    if ( tdsrrc_rbe.rabs.chan_config_needed[0] == FALSE )
    {
      /* RABs are not being setup for this message.
      Even though we don't have to setup RABs, there may
      be signalling RBs to be reconfigured in the message. Check
        if there are any */

      if (((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r5_IEs,rb_InformationAffectedList)) &&
          (rbsetup_ptr->rb_InformationAffectedList.n != 0 )) ||
          ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r5_IEs,srb_InformationSetupList)) &&
          (rbsetup_ptr->srb_InformationSetupList.n != 0)))
      {

        /* We have to reconfigure these RBs. First set TDSORDERED_CONFIG
          so that LLC can process the necessary parameters. */
        oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBE,
                                               tdsrrc_rbe.state_from_msg,
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

#endif /* FEATURE_TDSCDMA_GPS_CELL_ID_READING */

            /* Ordered Config has been set. We can setup the
              Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
            TDSRRC_MSG0(MSG_LEGACY_LOW,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
            /* Call the function that creates and sends a channel
            config request to LLC. */
            tdsrrcrbe_send_channel_config_req_r5( msg_ptr );
  
            if (((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
                (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
                (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
            {
              if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
                  tdsrrc_RadioBearerSetup_r5_IEs,new_C_RNTI))
              {
                tdsrrc_rbe.new_crnti_valid = TRUE;
                tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI,
                                    &c_rnti);
                tdsrrc_rbe.new_crnti = (uint16)c_rnti;
              }
              else
              {
                tdsrrc_rbe.new_crnti_valid = FALSE;
              }
              tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
            }
            else
            {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
              if( tdsrrc_rbe.curr_substate != TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF )
  #endif/* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
              {
                /* Change the substate since we're waiting for the
                confirmation from LLC for the channel configs */
                tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
              }
            }
  
            break;
  
          case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
            tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
            /* Send the RB Setup Failure Message */
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                      tdsrrc_rbe.status.prot_err_cause
                                    );
  
            /* Go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
  
          case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
            tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
            /* Send the RB Setup Failure Message */
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                      tdsrrc_rbe.status.prot_err_cause
                                    );
  
            /* Go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
            
       case TDSORDERED_CONFIG_CONFIGURATION_INVALID:
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
            tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
            /* Send the RB Setup Failure Message */
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                      tdsrrc_rbe.status.prot_err_cause
                                    );
  
            /* Go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
  
  
          case TDSORDERED_CONFIG_NOT_SET_OTHER:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
            /* When LLC sends this, there is a serious problem
            and we'll go back to disconnected state. No need
            to send the failure message here, we can just
            go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
  
          default:
  
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
            /* Unknown error, just go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
        }/* end switch */

        if ( ordered_config_err_occured == TRUE )
        {
          /* No need to continue. Clear the procedure
            variables and return. */
          tdsrrcrbe_clear_procedure();
          return;
        }
      }
      else
      {
        /*If we are here, it means that RABs are not given in RB-Setup.
        Also, SRBs and RB Affected IEs are not given.*/

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No RB IEs are given in the OTA.");
        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_informationElementMissing;  

        /* Send the RB Setup Failure Message */
        tdsrrcrbe_send_rbs_failure_message(tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);
        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
        tdsrrcrbe_clear_procedure();

        return;          
      }/* end if - RBs need to be reconfigured */
    }/* end if - tdsrrc_rbe.rabs.chan_config_needed[rab_index] == FALSE */
  }/* end if - valid rb message */
}/* end function process_rbsetup_message_r5 */

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/*===========================================================================

FUNCTION   RRCRBE_PROCESS_NEW_CELL_IND

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
static void tdsrrcrbe_process_new_cell_ind
(
  tdsrrc_new_cell_ind_type *ptr /* Pointer to the State Change Ind cmd */
)
{
  tdsrrc_cmd_type  *out_cmd_ptr;
#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
  uint8 i = 0;
  uint32 rb_index=0; 
  boolean um_rrc_srb_exist = FALSE;
  boolean am_rrc_srb_exist = FALSE;
  boolean am_high_priority_srb_exist = FALSE;
  boolean am_low_priority_srb_exist = FALSE;
  rlc_lc_id_type ul_rlc_lc_id = TDSRRCLCM_RLC_LC_ID_NOT_FOUND;
#endif
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
  /* If the UE has camped on directed cell then do not send a cell update,
   * else send a CELL update with cause "cell Re-selection 
   */
  
  if(tdsrrccsp_is_selected_cell_utran_directed()&&
      (tdsrrc_rbe.new_crnti_valid)) 
  {
    /* No need to initiate cell update as the C-RNTI is available
     * and UE is camped on the directed cell.
     */
    tdsrrcrbe_send_initiate_cu_flag = FALSE;
  }
  else
  {
    tdsrrcrbe_send_initiate_cu_flag = TRUE;
    tdsrrcrbe_delete_crnti          = TRUE;
  }
      
  /* Call the function that creates and sends the  channel
     config information to be requested from LLC. */
    
  if ((out_cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL)
  {
#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
    /* In fach state, RRC should configure the rb list if it is not in the RBE message, because
        the RLC PDU size maybe changed at new cell*/
    if ((tdsrrc_cmd.cmd_hdr.cmd_id == TDSRRC_CHANNEL_CONFIG_REQ)&&TDSCHECK_RLC_ALL_RB_REEST_ENABLE)
    {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Add SRB after find the new cell");
        rb_index = tdsrrc_cmd.cmd.chan_config_req.num_rb_to_config;
        for( i =0;i<rb_index;i++)
        {
          if (tdsrrc_cmd.cmd.chan_config_req.rb[i].rb_id == DCCH_UM_RADIO_BEARER_ID)
          {
            um_rrc_srb_exist = TRUE;
            continue;
          }
          else if (tdsrrc_cmd.cmd.chan_config_req.rb[i].rb_id == DCCH_AM_RADIO_BEARER_ID)
          {
            am_rrc_srb_exist = TRUE;
            continue;
          }
          else if (tdsrrc_cmd.cmd.chan_config_req.rb[i].rb_id == DCCH_DT_HIGH_PRI_RADIO_BEARER_ID)
          {
            am_high_priority_srb_exist = TRUE;
            continue;
          }
          else if (tdsrrc_cmd.cmd.chan_config_req.rb[i].rb_id == DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
          {
            am_low_priority_srb_exist = TRUE;
            continue;
          }
        }
          /*reconfig signalling RBs */
        if ((um_rrc_srb_exist == FALSE)&&(rb_index < TDS_MAX_RB))
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_index].rb_id = DCCH_UM_RADIO_BEARER_ID;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_index++].rb_config = TDSRECONFIG_RB;
        }
        if ((am_rrc_srb_exist == FALSE)&&(rb_index < TDS_MAX_RB))
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_index].rb_id = DCCH_AM_RADIO_BEARER_ID;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_index++].rb_config = TDSRECONFIG_RB;
        }
        if ((am_high_priority_srb_exist == FALSE)&&(rb_index < TDS_MAX_RB))
        {
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_index].rb_id = DCCH_DT_HIGH_PRI_RADIO_BEARER_ID;
          tdsrrc_cmd.cmd.chan_config_req.rb[rb_index++].rb_config = TDSRECONFIG_RB;
        }
        if (am_low_priority_srb_exist == FALSE)
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
            if (rb_index < TDS_MAX_RB)
            {
              tdsrrc_cmd.cmd.chan_config_req.rb[rb_index].rb_id = DCCH_DT_LOW_PRI_RADIO_BEARER_ID;
              tdsrrc_cmd.cmd.chan_config_req.rb[rb_index++].rb_config = TDSRECONFIG_RB;
            }            
          }
        }
        tdsrrc_cmd.cmd.chan_config_req.num_rb_to_config = rb_index;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Num of RB in channel configure request %d",rb_index);
     }
#endif
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
    memscpy(out_cmd_ptr,sizeof(tdsrrc_cmd_int_type),&tdsrrc_cmd,sizeof(tdsrrc_cmd_int_type));
#else
    out_cmd_ptr->cmd = tdsrrc_cmd.cmd;
    out_cmd_ptr->cmd_hdr = tdsrrc_cmd.cmd_hdr;
#endif

    tdsrrc_put_int_cmd( out_cmd_ptr );
  }
  else
  {
    ERR_FATAL("Could not get internal cmd buf",0,0,0);
  }
  
  /* Change the substate since we're waiting for the
     confirmation from LLC for the channel configs */
  tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
  
}
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

/*===========================================================================

FUNCTION   tdsrrcrbe_send_rbsetup_failure_message_ext_not_comprehended

DESCRIPTION
  Function to send failure message because message extension is not comprehended.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_send_rbsetup_failure_message_ext_not_comprehended
(
  tdsrrc_RadioBearerSetup *msg_ptr
)
{
  if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                 == tdsrrc_rbe.tr_ids.rejected_transaction )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBS msg! rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
    /* Nothing else to do, just ignore the message and return */
  }
  else
  {

    /* We have received a RBS message with a new transaction id.
    This message should be in our rejected transaction
    list */
    tdsrrc_rbe.tr_ids.rejected_transaction =
      (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
    tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;
    /*Check whether Cell Update is going on */
    if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
         (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
      (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
      /* Wait until Cell update procedure is completed */
      tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
    }
    else
    {
      tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                      tdsrrc_rbe.status.prot_err_cause );
      /*continue the same substate */
    }
  }
}

/*===========================================================================

FUNCTION   PROCESS_RBSETUP_MESSAGE

DESCRIPTION

  This function processes the received RB Setup Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Setup Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -

    TDSORDERED_CONFIG will be set by calling LLC.

    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.

    If TDSORDERED_CONFIG could not be set, a Radio Bearer Setup Failure message
    will be sent to UTRAN.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_process_rbsetup_message
(
tdsrrc_cmd_type *cmd_ptr
)
{

  tdsrrc_DL_DCCH_Message     *dcch_msg_ptr; /* Pointer to the RB Setup message */
  tdsrrc_RadioBearerSetup *msg_ptr;    /* Pointer to the RB Setup message        */
  tdsrrcsmc_srns_ip_check_status_e_type    ip_check_status = TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;
  tdsrrc_rbe.first_tm_rb_setup = FALSE;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Recvd RB Setup Msg cur_substate %d",tdsrrc_rbe.curr_substate);


  if(TRUE == tdsrrccsp_reselection_in_progress())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Abort cell reselection as it's conflict with RB Setup");
    tdsrrccsp_abort_cell_reselection();
  }

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
                          downlink_sdu_ind.decoded_msg;
  /* If ASN.1 decoding failed, the received pointer would be null.
  So we know we got a RB Setup Message (by peeking into
  the encoded bit stream), but we were not able to decode it.
  Send a RB setup failure message with the appropriate
  cause. */
  if ( dcch_msg_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"ASN decode failed for RBS msg");
    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
    tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
    tdsrrc_rbe.tr_ids.rejected_transaction = 0;
    tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                              tdsrrc_rbe.status.prot_err_cause
                            );

    /* Nothing else to do here */
    /*do not change the current state. Some other RBsetup procedure might be
    on going. Maintain the current state and do not clear the procedure */
    return;
  }

  /* Make sure it's the Radio Bearer Setup message
  although we would not have gotten here unless it's
  the right message */
  if ( dcch_msg_ptr->message.t != T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Incorrect message given to RBE proc %d",dcch_msg_ptr->message.t);
    return;
  }

  /* Get the pointer to the actual RB Setup message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerSetup);

  /* The first thing to do is check the transaction id.
  If the transaction id is the same as the accepted
  or rejected transaction ids, we don't process
  this message. It's a duplicate and will just be
  dropped. But we also don't change the current
  sub-state since we're in the middle of
  processing a previous message. Note that
  the transaction id is included only if critical
  extensions are not present. */
  /* Unsupport rel99 message */
  if (msg_ptr->t == T_tdsrrc_RadioBearerSetup_r3)
  {
    if (msg_ptr->u.r3.radioBearerSetup_r3.rrc_TransactionIdentifier
                       == tdsrrc_rbe.tr_ids.rejected_transaction)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBE msg - rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
      /* Nothing else to do, just ignore the message and return */
    }
    else
    {
      /* send a reconfig failure messge with invalid
   configuration and
   continue the same substate with any ongoing procedure */
      tdsrrc_rbe.tr_ids.rejected_transaction =
        (msg_ptr->u.r3.radioBearerSetup_r3.rrc_TransactionIdentifier);
      if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
      {
        tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      }
      else
      {
        tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
      }
      /*Check whether Cell Update is going on */
      if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
           (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
      {
        if(tdsrrc_rbe.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbe.curr_substate,tdsrrc_rbe.status.failure_status);
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                    tdsrrc_rbe.status.prot_err_cause );
          tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_VALID;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
          (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
          /* Wait until Cell update procedure is completed */
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        }
      }
      else
      {
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                        tdsrrc_rbe.status.prot_err_cause );
      }
    }
        /*do not clear the procedure or change the tdsrrc_rbe substate because
        some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
    return;
  }
  
  if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL4))  
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:rbsetupr4 msg");
    tdsrrcrbe_process_rbsetup_r4(cmd_ptr);
    return;
  }
  else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
      (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL5)))
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:rbsetupr5 msg");
        tdsrrcrbe_process_rbsetup_r5(cmd_ptr);
        return;
  }
  /* Check if Rel 6 contents are present */
  else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6) &&
       (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL6)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL:rbsetupr6 msg");
    tdsrrcrbe_process_rbsetup_r6(cmd_ptr);
    return;
  }
  /* Check if Rel 7 contents are present */
  else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7) &&
       (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL7)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCREL7:rbsetupr7 msg");
    tdsrrcrbe_process_rbsetup_r7(cmd_ptr);
    return;
  }
#ifdef FEATURE_TDSCDMA_REL8
  /* Check if Rel 8 contents are present */
  else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8) &&
       (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL8)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Received RB Setup OTA.");
    tdsrrcrbe_process_rbsetup_r8(cmd_ptr);
    return;
  }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
    /* Check if Rel 9 contents are present */
    else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9) &&
         (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
           T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL9)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:Received RB Setup OTA.");
      tdsrrcrbe_process_rbsetup_r9(cmd_ptr);
      return;
    }
#endif /* FEATURE_TDSCDMA_REL9 */

  else
  {
    tdsrrc_print_supported_asn1_rel_version();
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Configuration not supported.  Send RB setup failure");
    tdsrrcrbe_send_rbsetup_failure_message_ext_not_comprehended(msg_ptr);
    
    /*do not change the current state. Some other RBsetup procedure might be
    on going. Maintain the current state and do not clear the procedure */
    return;
  }
}




/*===========================================================================

FUNCTION   RRCRBE_PROCESS_RNTI_UPDATE_CNF

DESCRIPTION

  This function processes RNTI_UPDATE_CNF

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_process_rnti_update_cnf
(
void
)
{
  tdsrrc_state_e_type curr_state;

  curr_state = tdsrrc_get_state();

  if ((curr_state == TDSRRC_STATE_CELL_FACH) &&
      ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
       (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH)))
  {
   /*send the response message now */
    tdsrrcrbe_send_rbs_complete_message(TRUE);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBE Complete msg");
    /*wait for L2ack */
    tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    /* prints rate for each transport channel */
    tdsrrcllc_print_trans_chnl_rate();
  }
  else
  {
    /* We should not get here*/
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unexpected RNTI Update cnf,RBE state %d",tdsrrc_rbe.curr_substate);
  }
}

/*===========================================================================

FUNCTION   RRCRBE_PROCESS_CHANNEL_CONFIG_CNF

DESCRIPTION

  This function processes the received Channel Config Confirmation
  from LLC. If the required Radio-bearers have been setup succesfully,
  and if any of the radio-bearers map to radio-access-bearers, this
  information needs to be given to the NAS entity for that RAB.

  If a voice RAB has been assigned by the UTRAN, this function
  gets the RLC ids and the AMR mode information from LLC and sends this
  information to voice services. But for CS and PS data calls,
  the RLC ids are not sent by this function. The data services
  entity calls the required function in rrcdata module to get
  the RLC ids whenever needed.

  This function sends the RRC_SYNC_IND primitive to MM for Circuit-Switched
  calls. This function sends the RABMAS_RAB_ESTABLISH_IND primitive to
  RABM for Packet-Switched calls.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_process_channel_config_cnf
(
tdsrrc_channel_config_cnf_type *ptr  /* Pointer to the Channel config cnf cmd */
)
{

  uint32 rab_index=0;    /* Used if there is more than 1 RAB in the
                         message */
  tdsrrcllc_amr_mapping_info_type amr_info = {0}; /* To store AMR mode
                         info incase a voice RAB has been setup */
  
  tdsrrc_state_e_type curr_state;
  tdsrrcrb_rnti_update_info_type rnti_update_info;   /* to hold rnti update info */
  tdsrrc_CellUpdateCause       cu_cause; /*To store CU cause from CU procedure*/

  tdsrrc_rlc_re_est_for_srns_rb_e_type rb_type; /*rb type tobe reestablished */
  boolean amr_rate_configured = FALSE;
  
  uint32 current_mode = tdsrrc_get_current_codec_mode();

#ifdef FEATURE_SCUDIF_ENHANCEMENT
  #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/

  amr_info.dl_amr_c_id = 0;

  /* Check if the channel configuration was succesful. */
  if ( ptr->chan_cfg_status != TDSRRCLLC_CHAN_CFG_SUCCESS )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CHAN_CONFIG_CNF with Failure");
    /* Channel config failure, so initialise the ordered RB-Mapping database*/
    tdsrrcllc_init_ordered_hanging_rb_mapping_info();

    /*Here we initialize tdsrlc_size_change_in_progress DB so that if it was set by RBE, 
    it is ready for next procedure*/
    tdsrrc_initialize_rlc_size_change();

    tdsrrc_set_ordered_codec_mode(current_mode);
    /* Roll back tdsmac_rab_status */
    tdsrrcrb_update_mac_rab_status(TDSRRC_PROCEDURE_RBE, RRC_CS_DOMAIN_CN_ID, NULL,TRUE);

    tdsrrc_rbe.status.failure_status = TDSRRCRB_PHY_CHAN_FAILURE;
    /* Channel config got Cancelled because of RL_FAILURE or RLC_UNRECOVERABLE
     * error.  Wait for CU procedure to complete before sending failure msg to NW
     */
    if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_CFG_CANCELLED)
    {
      /* cell update is already in progress as this is for pending config
       * cancellation
       */
       #ifdef FEATURE_TDSCDMA_RCR_CANCEL_RECFG
       if (TRUE == tdsrrcrcr_is_proc_active())
       {
          /* Reset TDSORDERED_CONFIG - UE can now accept other ordered
          configurations */
          if (TDSOC_NOT_SET == tdsrrcllc_reset_ordered_config(TDSRRC_PROCEDURE_RBE
                                                        ,TRUE
                                                        ))
          {
            /* Then clear all local variables that may
            need to be cleared */
            tdsrrcrbe_clear_procedure();
            /* Make sure we're back in the initial sub-state */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
          }
       }
       else
       #endif
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Channel Config cancel waiting for CU complete");
         tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
       }

      return;
    }
    /* validation failure happened.  Send RB failure msg with invalid config */
    if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_OTHER)
    {

      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      /* Check if CU is waiting to handle RLC unrecoverable error.
       * If so, wait for CU to complete before sending failure to NW.
       */
      if(tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG)
      {
        tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBE, TDSRRC_UNRECOVERABLE_ERROR, FALSE);
        return;
      }

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending RBE failure msg with invalid config");
      tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                tdsrrc_rbe.status.prot_err_cause
                                );
      /* Reset TDSORDERED_CONFIG - UE can now accept other ordered
      configurations */
      if (TDSOC_NOT_SET == tdsrrcllc_reset_ordered_config(TDSRRC_PROCEDURE_RBE
                                                    ,TRUE
                                                    ))
      {
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbe_clear_procedure();
        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to reset the OC");
      }
      /*During FACH->DCH transition, if T305 got expired, we need to start T305 with zero timeout.*/      
      if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH && tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)
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




    //anytime chan-config for tdsrrc_rbe fails,  we go to fach, hence call the API which SMC provides
    //to release TM bearers for CS domain.  This needs to be done as going to fach does not
    //go through regular channel-config and hence smc needs to know of this release of tm bearers
    //directly through rb procedure
    tdsrrcsmc_update_ciphering_config_info_for_signalling_release(RRC_CS_DOMAIN_CN_ID);

    /* Check if PHY_CHAN_FAILURE happened in FACH->DCH Transition.  If so, initiate a cell update
    request, and tdsrrc_rbe substate should be TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF */
    if (tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH && tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PHYCHANFAIL FACH->DCH.  Initiate CU");
      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBE, TDSRRC_PHY_CHAN_FAILURE,TRUE);
      /* Wait for CELL UPDATE to get a valid C_RNTI */
      tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      tdsrrcrbe_send_initiate_cu_flag = FALSE;
      tdsrrcrbe_delete_crnti          = FALSE;
    }
    /* Check if PHY_CHAN_FAILURE happended within CELL_DCH.  For now, initatiate a cell update request
       with cause RL Failure.  RBE substate should be TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF*/
    else if (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH && tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)
    {
      /* if cu is waiting to handle RL/RLC unrecoverable failure, wait for
       * Cell update to complete before sending failure.
      */
      if(tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG)
      {
        tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBE, TDSRRC_RADIO_LINK_FAILURE, FALSE);
        return;
      }

      if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_PHY_CHL)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBsetup failed,going back to old cfg success");
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                  tdsrrc_rbe.status.prot_err_cause
                                  );
        /* Go back to initial substate */
        tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbe_clear_procedure();
        tdsrrcllc_clear_ordered_config();
      }
      else if (ptr->chan_cfg_status == TDSRRCLLC_CHAN_CFG_FAILURE_BACK_TO_OLD_CONFIG)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBsetup failed. Old config failed.  Initiate CU");
        /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBE, TDSRRC_RADIO_LINK_FAILURE,TRUE);
        /* Wait for CELL UPDATE to get a valid C_RNTI */
        tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        tdsrrcrbe_send_initiate_cu_flag = FALSE;
        tdsrrcrbe_delete_crnti          = FALSE;
      }
    }
    else
    {
      tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                tdsrrc_rbe.status.prot_err_cause
                                );
      /* Go back to initial substate */
      tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
      /* Then clear all local variables that may
      need to be cleared */
      tdsrrcrbe_clear_procedure();
    }
    return;
  }
 
  
  /*For FACH->DCH, if CHAN_CONFIG_REQ is succeeded, we need to stop T305*/  
  if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH && tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Chan_config succeeded for FACH->DCH. So Stopping T305");
    tdsrrctmr_stop_timer(TDSRRCTMR_T_305_TIMER);
  }  

   /*Notify the Cell_Id to registered entities*/
  if(tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)
  {
    tdsrrc_notify_cell_id_in_dch();
  } 

  /* Copy the Ordered RB Mapping into the current RB-Mapping database*/
  tdsrrcllc_copy_ordered_hanging_rb_mapping_to_current();

  tdsrrc_rbe.hho_ciph_status = ptr->inter_freq_hho_status;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO Ciph Status set to %d from chan cfg cnf",tdsrrc_rbe.hho_ciph_status);
  /* Channel config was succesful. Check if any
  RABs were setup, or only Signalling RBs were setup */
  for (rab_index=0; rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg; rab_index++)
  {
    /*This flag is used to notify whether the RAB has been updated in tdsrrc_est_rabs.*/
    boolean existing_rab_updated = FALSE;

    if ( tdsrrc_rbe.rabs.chan_config_needed[rab_index] == TRUE )
    {
      /* Some RABs were setup. Set the default value again
      till we get the next message */
      tdsrrc_rbe.rabs.chan_config_needed[rab_index] = FALSE;

   /* Now we need to update the UE Variable ESTABLISHED_RABS.
      Note that we have only one RAB per message now. When
      more RABs are established, we need to do this for each
      RAB.Check if the RAB id is already in ESTABLISHED_RABS.
      It shouldn't be, but we have other RRC procedures running
      at the same time, so make a safety check here. */
      if ( (tdsrrc_find_rab_in_established_rabs( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id ))
           == TDSRRC_RAB_NOT_FOUND )
      {
        /* We can add this RAB. */
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Add RAB id %d, RAB type: %d(0:Voice, 2:PS) in EST_RAB, rab_index: %d",
                     tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id,
                     tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type, rab_index);
        tdsrrc_add_rab_to_established_rabs( &(tdsrrc_rbe.rabs.rabs_in_msg[rab_index]));


      }
      else
      {
        /*We are here. It implies that the RAB was already present in tdsrrc_est_rabs.*/
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RAB already in established RABs");
        if((tdsrrc_find_if_voice_rab_exists_in_established_rabs(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id) == TRUE)
           )

        {
          existing_rab_updated = TRUE;
          tdsrrc_update_rb_info_to_existing_rab(&tdsrrc_rbe.rabs.rabs_in_msg[rab_index]);
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"updated RB info in established RAB %d",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id );
        }    
        /*For VT -> Voice SCUDIF, we will come here.*/
#ifdef FEATURE_SCUDIF_ENHANCEMENT
#error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB Type %d in RBE DB",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type);
        }
      }

#ifdef FEATURE_SCUDIF_ENHANCEMENT
      #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/
      {
        /*By this time, RBs and RABs have been updated in tdsrrc_est_rabs.*/
        /* Now check what kind of RAB was setup */
        if ( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB )
        {
          uint8 rab_idx = 0;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Channel config succeeded for voice RAB");
          if((rab_idx = tdsrrc_get_idx_in_rrc_est_rabs_for_rab_id(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id)) 
                < MAX_RAB_TO_SETUP)
          {
            if((existing_rab_updated) && (tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator!=current_mode))
            {
              tdsrrc_set_current_codec_mode(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator);
              current_mode = tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator;
              tdsrrc_change_codec_configuration(rab_idx);
            }
            else
            {
              tdsrrc_set_current_codec_mode(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator);
              current_mode = tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator;
              if( current_mode == TDSCDMA_IVOCODER_ID_AMRWB)
              {
                amr_info.mode_type = TDSCDMA_IVOCODER_ID_AMRWB;
                (void) tdsrrcllc_get_amr_wb_mapping(&(amr_info));
              }
              else
              {
                amr_info.mode_type = TDSCDMA_IVOCODER_ID_AMR;
                /*Now TDS Voice Adapter needs to be configured. The structure amr_info will be filled by this function. */
                (void) tdsrrcllc_get_amr_mapping(&(amr_info));
              }
              tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_CODEC_MODE);
              tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_SCR_MODE);
              tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS);
              tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_START);
            }
          }

          amr_rate_configured = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TV_TRRC: Updating TDS Voice Adapter for voice RAB");
        }
        else if ( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_DATA_RAB )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Channel config succeeded for CS data RAB");
  
          /* For CS Data calls we don't give the rlc id's right now.
          The logical channel manager has the rlc ids and Data Services
          will call the function tdsrrc_return_lc_info_for_rab to get the
          LC ids. */
        }
        else if ( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type == TDSRRCRB_PS_DATA_RAB )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Channel config succeeded for PS data RAB");
  
          /* For PS calls we don't give the rlc id's right now.
          The logical channel manager has the rlc ids and Data Services
          will call the function tdsrrc_return_lc_info_for_rab to get the
          LC ids. */
        }
        else
        {
          /* Won't happen since we would have rejected the
          message earlier. But this is a safety check */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported RAB type: %d",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type);
        }
      }
    }/* end if - tdsrrc_rbe.rabs.chan_config_needed[rab_index] == TRUE */
  } /* for() */


#ifdef FEATURE_SCUDIF_ENHANCEMENT
  #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/

  /* Make sure TDS Voice Adapter gets updated with the TB sizes if reconfigured 
     while adding CS DATA RAB/PS RAB/SRBs */
  if(amr_rate_configured == FALSE)
  {
    tdsrrcrb_update_amr_cctrch_id_for_reconfig(current_mode);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Updated MVS when for already established voice RAB (probable rate change)");
  }


  /* Check for CELL FACH before sending the rbs complete message */
  curr_state = tdsrrc_get_state();
  if ((TDSRRC_STATE_CELL_FACH == curr_state) &&
      ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_FACH) ||
       (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
       (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH)))
  {
    if(tdsrrcrbe_send_initiate_cu_flag 
       || (tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_RB_PROC_COMPL_IND) 
       )
    {
      if (tdsrrcrbe_delete_crnti)
      {
        /* Send a RNTI Update Request */
        rnti_update_info.crnti_action         = TDSRRC_RNTI_DELETE;
        rnti_update_info.urnti_action         = TDSRRC_RNTI_NOCHANGE;
        rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBE;
        rnti_update_info.rlc_re_establish_srb = FALSE;
        rnti_update_info.rlc_re_establish_rab = FALSE;
        rnti_update_info.cnf_reqd             = FALSE;

        tdsrrcrb_send_rnti_update_req(rnti_update_info);
      }
      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrcrbe_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBE);
      /* Wait for CELL UPDATE to get a valid C_RNTI */
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_VALID;
      tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      tdsrrcrbe_send_initiate_cu_flag = FALSE;
      tdsrrcrbe_delete_crnti          = FALSE;
      return;
    }
    /*Following case will occur when RLC RESET happens on SRB2 during DCH->FACH Transition*/
    else if((tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG) &&
      (tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) == TDSRRCCU_STARTED) && 
      (cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError) && (tdsrrccu_get_rb_id_unrec_error() == 
      DCCH_AM_RADIO_BEARER_ID))
    {
      if(tdsrrc_rbe.new_crnti_valid)
      {
        /* Send a RNTI Update Request */
        rnti_update_info.crnti_action         = TDSRRC_RNTI_UPDATE;
        rnti_update_info.urnti_action         = TDSRRC_RNTI_NOCHANGE;
        rnti_update_info.crnti                = tdsrrc_rbe.new_crnti;
        rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBE;
        rnti_update_info.rlc_re_establish_srb = FALSE;
        rnti_update_info.rlc_re_establish_rab = FALSE;
        rnti_update_info.cnf_reqd             = FALSE;
  
        tdsrrcrb_send_rnti_update_req(rnti_update_info);
        /* Send a Radio Bearer Setup Complete later */      
      }

      /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
      tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBE,
                                          TDSRRC_UNRECOVERABLE_ERROR,
                                          TRUE);

      /* Wait for CELL UPDATE to get a valid C_RNTI */
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_VALID;
      tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;

      return;                                          
    }    
    /* Coming here implies the "tdsrrcrbe_send_initiate_cu_flag" is FALSE.
     * Check if Valid CRNTI is stored in RBE (for DCH->FACH case).
     * If so, send a RNTI Update Req to LLC and NOT wait for
     * Cell Update Complete Confirm. Cell Update will NOT be
     * initiated when there is a valid C-RNTI and send_initiate_cu_flag
     * is FALSE.
     */
    else if(tdsrrc_rbe.new_crnti_valid)
    {
      /* Send a RNTI Update Request */
      tdsrrcrbe_send_crnti_update_req(tdsrrc_rbe.new_crnti);
      /* Send a Radio Bearer Setup Complete later */
    }

    /* Query for valid C-RNTI */
    else if (TDSC_RNTI_NOT_VALID == tdsrrcllc_get_current_crnti_status() )
    {
      /* Wait for CELL UPDATE to get a valid C_RNTI. Register for CU complete cnf. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No Valid C_RNTI! Register for CU CNF");
      (void) tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_VALID;
      tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      return;
    }
  }


  /*check whether this reconfiguration involves SRNS relocation */
  if (tdsrrc_rbe.rbe_for_srns_relocation == TRUE)
  {
    if (TDSRRC_STATE_CELL_DCH == curr_state)
    {
      /* Re-establish SRB2 before sending the response message */
      rb_type  = TDSRRC_RE_EST_RB_2;
      tdsrrcrb_send_rlc_re_est_req_to_smc(TDSRRC_PROCEDURE_RBE, rb_type, TRUE);

      tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_REESTABLISH_SRB2;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"SRNS not supp. in state %d",curr_state);
    }
  }
  else /*normal reconfiguration without SRNS relocation */
  {
    /* Send the Radio Bearer Reconfig Complete message */
    if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
        (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH))
    {
      tdsrrcrbe_send_rbs_complete_message(TRUE);
    }
    else
    {
     /* Need to wait for L2 ack for all states because RRC needs to trigger include reconfig status indicator
     if RLC acknowledgement is not yet received.
     As per 8.3.1.3
     1> if the IE "Cell update cause" is set to "radio link failure" and the UE has submitted a reconfiguration response
     message to RLC and the UE has not received the RLC acknowledgement for the response message:
     2> include the IE "Reconfiguration Status Indicator" and set it to TRUE.*/
        tdsrrc_update_reconfig_waiting_for_l2ack_dch(TDSRRC_PROCEDURE_RBE);
        tdsrrcrbe_send_rbs_complete_message(TRUE);
      /* IF CU is waiting to handle RLF/RLC unrecoverable error, then send complete
       * and delay for 50ms so to make sure Complete goes out to NW on old config
       */
      if((tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG)
        && (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delaying by %d ms",TDSRRCRB_PND_CFG_DELAY);
        (void) rex_clr_sigs(rex_self(), TDSRRCRBE_TEMP_TIMER_SIG);
        (void) rex_set_timer(&tdsrrcrbe_temp_timer, TDSRRCRB_PND_CFG_DELAY);
        tdsrrcrbe_sig = rex_wait(TDSRRCRBE_TEMP_TIMER_SIG);
        TDSRRC_MSG1(MSG_LEGACY_LOW,"RRCRBE signal:%d",tdsrrcrbe_sig);
        (void) rex_clr_sigs(rex_self(), TDSRRCRBE_TEMP_TIMER_SIG);
        tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBE, TDSRRC_UNRECOVERABLE_ERROR, FALSE);
      }
      /*Following case handles the scenario when RF Failure or RLC unrecoverable error occured
        during DCH -> FACH or FACH -> FACH transiton.*/
      else if((tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_PENDING_CONFIG) &&
      (tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) == TDSRRCCU_STARTED))
      {
        if((cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError) && (tdsrrccu_get_rb_id_unrec_error() != DCCH_AM_RADIO_BEARER_ID))
        {
          /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
          tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBE,
                                              TDSRRC_UNRECOVERABLE_ERROR,
                                              TRUE);
        }
        else if(cu_cause == tdsrrc_CellUpdateCause_radiolinkFailure)
        {
          /* Send TDSRRC_CELL_UPDATE_INITIATE_REQ command to CELL UPDATE Proc */
          tdsrrcrb_send_cell_update_initiate_req(TDSRRC_PROCEDURE_RBE,
                                              TDSRRC_RADIO_LINK_FAILURE,
                                              TRUE);
        }
      }      
    }
    /* send rbs indication to upperlayers */
    tdsrrcrbe_send_indication_to_upper_layers();

    if (tdsrrcllc_get_ordered_config_status() != TDSOC_SET_FOR_DCH_FACH_TRANS)
    {
      /* prints rate for each transport channel */
      tdsrrcllc_print_trans_chnl_rate();
    }

    /*if next state is Cell_PCH or URA_PCH then we have to
    treat the procedure as successful only after getting L2ack
    for the response message */
    if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
        (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH))
    {
      /* Change the substate to the next substate - this is
      where we wait for L2 ack from RLC for sending the RB RC
      complete message. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBS Complete msg");
      tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
    }
    else
    {
      /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
      configurations */
      tdsrrcllc_clear_ordered_config();

      /* Go to the next sub-state in the state machine. That's
      the initial substate since we've sent the RBS complete
      message */
      tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

      /* At this point we don't need to remember any of the
      Radio Bearer information since it's stored in ESTABLISHED_RABS.
      Clear our internal variables */
      tdsrrcrbe_clear_procedure();
    }
  }

}
/*===========================================================================

FUNCTION   RRCRBE_PROCESS_RLC_REESTABLISHMENT_CNF

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
static void tdsrrcrbe_process_rlc_reestablishment_cnf
(
void
)
{
  tdsrrcrb_rnti_update_info_type rnti_update_info; /* structure to be used to request RNTI update */

  /* send rnti update request if URNTI was present in the rebsetup message */
  if (tdsrrc_rbe.new_urnti_valid == TRUE)
  {
    /* Send the RNTI update request to rrcllc to update with new-URNTI */
    /* Send a RNTI Update Request */
    rnti_update_info.crnti_action         = TDSRRC_RNTI_NOCHANGE;
    rnti_update_info.urnti_action         = TDSRRC_RNTI_UPDATE;
    rnti_update_info.urnti                = tdsrrc_rbe.new_urnti;
    rnti_update_info.procedure            = TDSRRC_PROCEDURE_RBE;
    rnti_update_info.rlc_re_establish_srb = FALSE;
    rnti_update_info.rlc_re_establish_rab = FALSE;
    rnti_update_info.cnf_reqd             = FALSE;

    tdsrrcrb_send_rnti_update_req(rnti_update_info);
  }

  /* Send the reconfiguration complete message */

  tdsrrcrbe_send_rbs_complete_message(TRUE);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBE Complete msg");

  /* Change the substate to the next substate - this is
     where we wait for L2 ack from RLC for sending the RB Reconfig
     complete message. */
  tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
  /* prints rate for each transport channel */
  tdsrrcllc_print_trans_chnl_rate();

}




/*===========================================================================

FUNCTION   RRCRBE_PROCESS_STATE_CHANGE_IND

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
static void tdsrrcrbe_process_state_change_ind
(
tdsrrc_state_change_ind_type *ptr /* Pointer to the State Change Ind cmd */
)
{
  tdsrrc_cmd_type  *out_cmd_ptr;
  tdsrrc_CellUpdateCause cu_cause;
  /* Check if we're going to disconnected state */
  if ( ptr->new_state == TDSRRC_STATE_DISCONNECTED )
  {
    /* Then change to initial substate */
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbe_clear_procedure();
  }

  else if (ptr->new_state == TDSRRC_STATE_CELL_FACH)
  {

    if(ptr->previous_state == TDSRRC_STATE_CELL_DCH)
    {
      if (tdsrrc_rbe.status.failure_status != TDSRRCRB_MSG_VALID)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring STATE_CHANGE_IND");
      }
      else
      {
        /* if cell update procedure is active with cause OOS, wait 
         * for the CU complete notification to send chan_config_req to
         * LLC 
         */
        if (tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause)== TDSRRCCU_STARTED &&
            cu_cause == tdsrrc_CellUpdateCause_re_enteredServiceArea) 
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Complete tdsrrc_rbe msg first.  Indicate compl to CU once done");
        }
        
        TDSRRC_MSG0(MSG_LEGACY_LOW,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
        /* Call the function that creates and sends the  channel
           config information to be requested from LLC. */
      
        if ((out_cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL)
        {
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
          memscpy(out_cmd_ptr,sizeof(tdsrrc_cmd_int_type),&tdsrrc_cmd,sizeof(tdsrrc_cmd_int_type));
#else
          out_cmd_ptr->cmd = tdsrrc_cmd.cmd;
          out_cmd_ptr->cmd_hdr = tdsrrc_cmd.cmd_hdr;
#endif

          tdsrrc_put_int_cmd( out_cmd_ptr );
        }
        else
        {
          ERR_FATAL("Could not get internal cmd buf",0,0,0);
        }
        
        /* Change the substate since we're waiting for the
           confirmation from LLC for the channel configs */
        tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
      
        if((tdsrrccsp_is_selected_cell_utran_directed()) &&
           (tdsrrc_rbe.new_crnti_valid))
        {
          /* No need to initiate cell update as the C-RNTI is available
          * and UE is camped on the directed cell.
          */
          tdsrrcrbe_send_initiate_cu_flag=FALSE;
        }
        else
        {
          tdsrrcrbe_send_initiate_cu_flag=TRUE;
          tdsrrcrbe_delete_crnti = TRUE;
        }
      }
    }
    else
    {

      /* Currently there is no action when we go to cell_fach.
         This is not expected for release 1 since UE won't support
         transition to cell_fach. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received State change to CELL_FACH,no action");

    }
    /* NOTE: When we do support this, we need to start timer
    T305 if it's not running. We need to stop DRX mode, and
    if C-RNTI is not sent in the message, send a command to
    cell-update procedure. */
  }
  else
  {
    /* We should not get here since we haven't registered for
    other state changes */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Unsupported state change %d to %d",ptr->previous_state,ptr->new_state);
  }
}





/*===========================================================================

FUNCTION   RRCRBE_PROCESS_L2_ACK_IND

DESCRIPTION

  This function processes the received Layer 2 ACK indication.
  If the L2 ACK indicates that the RBS Complete message has
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
static void tdsrrcrbe_process_l2_ack_ind
(
tdsrrc_downlink_l2ack_ind_type *ptr /* Pointer to the L2 Ack Ind cmd */
)
{
  tdsrrc_cmd_type *cmd; /* To send a LLC command if L2 tx failed */

  tdsrrc_rlc_re_est_for_srns_rb_e_type rb_type; /*rb type tobe reestablished */
  tdsrrc_CellUpdateCause       cu_cause;
  if ( ptr->status == SUCCESS )
  {
    /* clear trans id */
    tdsrrc_rbe.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;

    /* if SRNS relocation is involved with this message then re
    establish the RBs other than RB2 here */
    if (tdsrrc_rbe.rbe_for_srns_relocation)
    {
      if (TDSRRC_STATE_CELL_DCH == tdsrrc_get_state())
      {
        /* Initiate the re-establishment of remaining RBs before
        RBE procedure goes back to initial substate.
        No need to wait for the confirmation */
        rb_type  = TDSRRC_RE_EST_NON_RB_2_UM_AM;
        tdsrrcrb_send_rlc_re_est_req_to_smc(TDSRRC_PROCEDURE_RBE, rb_type, FALSE);

        /* send rbs indication to upperlayers */
        tdsrrcrbe_send_indication_to_upper_layers();
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"SRNS reloc flag is ON in rrc state %d",tdsrrc_get_state());
      }
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2 ack failed for RBS Complete msg");

    /* Here make sure that CU is not active.  If CU is active with OOS area handling
     then NACK coming from RLC shall be ingnored as RBE failure with cause
     CU complete should be sent to NW */
    if (TDSRRC_STATE_CELL_FACH == tdsrrc_get_state() &&
        TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status_with_cause(&cu_cause) &&
        (cu_cause == tdsrrc_CellUpdateCause_re_enteredServiceArea || cu_cause == tdsrrc_CellUpdateCause_rlc_unrecoverableError ||
         cu_cause == tdsrrc_CellUpdateCause_radiolinkFailure))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NACK being ingnored as RBE failure msg will be sent");
      (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
      return;
    }

    /* clear trans id*/
    tdsrrc_rbe.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;

    if ((tdsrrc_rbe.rbe_for_srns_relocation) ||
        (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
        (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH))
    {
      /* l2ack failed. If reconfiguration involves SARNS relocation then
      go to idle. Otherwise just complete the procedure */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"L2ack failed SRNSreloc:%d,next_st%d",tdsrrc_rbe.rbe_for_srns_relocation,tdsrrc_rbe.state_from_msg);
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
        cmd->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RBE;
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

#if defined(FEATURE_3GPP_FAST_DORMANCY)
  /* Update Dormancy Status before clearing procedure */
  if (tdsrrc_rbe.rbe_for_srns_relocation == TRUE)
  {
    tdsrrcdormancy_update_dormancy_for_srns_hho_cco(TDSRRC_PROCEDURE_RBE);
  }
#endif

  /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
  configurations */
  tdsrrcllc_clear_ordered_config();

  /* Go to the next sub-state in the state machine. That's
  the initial substate since we've sent the RBS complete
  message */
  tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

  /* At this point we don't need to remember any of the
  Radio Bearer information since it's stored in ESTABLISHED_RABS.
  Clear our internal variables */
  tdsrrcrbe_clear_procedure();
}


/*===========================================================================

FUNCTION   RRCRBE_INITIAL_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_INITIAL substate. This function
  looks at the received event and does the appropriate processing
  of the event.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcrbe_initial_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Setup Message. Otherwise
    we would not have received this command. Copy the
    received message pointer. */
    /* Process the RB Setup message */
    tdsrrcrbe_process_rbsetup_message( cmd_ptr );
    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }
}


 
/*===========================================================================

FUNCTION   RRCRBE_DCH_WFSRB2REESTABLISH_SUBSTATE_EVENT_HANDLER

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
static void tdsrrcrbe_dch_wfsrb2reestablish_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_RE_EST_RLC_FOR_SRNS_CNF:

    /* Confirmation from SMC of RLC re-establishment confirm */
    tdsrrcrbe_process_rlc_reestablishment_cnf( );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Reconfig Message. Otherwise
    we would not have received this command. Copy the
    received message pointer. */
    /* Process the RB Reconfig message */
    tdsrrcrbe_process_rbsetup_message( cmd_ptr );

    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }
}

/*===========================================================================

FUNCTION   RRCRBE_DCH_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_CHAN_CFG_CNF substate
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
static void tdsrrcrbe_dch_wfchcnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbe_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Setup Message. Otherwise
    we would not have received this command. Copy the
    received message pointer. */
    /* Process the RB Setup message */
    tdsrrcrbe_process_rbsetup_message( cmd_ptr );
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }
}


/*===========================================================================

FUNCTION   tdsrrcrbe_other_wfcucnf_substate_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_CELL_UPDATE_CNF substate
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
static void tdsrrcrbe_other_wfcucnf_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_STATE_CHANGE_IND:
      /* Indication that RRC's state has changed */
      tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;

    default:
      /* No other events expected in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
      break;
  }
}

/*===========================================================================

FUNCTION   tdsrrcrbe_other_wfcscnf_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_CELL_SELECTION_CNF substate
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
static void tdsrrcrbe_other_wfcscnf_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_STATE_CHANGE_IND:
      /* Indication that RRC's state has changed */
      tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
      break;

    default:
      /* No other events expected in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
      break;
  }
}

/*===========================================================================

FUNCTION   RRCRBE_DCH_WFCUCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_CELL_UPDATE_CNF substate
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

static void tdsrrcrbe_dch_wfcucnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      if(tdsrrc_rbe.status.failure_status == TDSRRCRB_SIMULTANEOUS_RECONFIG)
      {
        /* Send the RB Setup Failure Message */
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                  tdsrrc_rbe.status.prot_err_cause
                                );
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbe_clear_procedure();

        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
      }
      else if(tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_VALID)
      {
        /* Send the Radio Bearer Setup Complete message */
        if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
            (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH))
        {
          tdsrrcrbe_send_rbs_complete_message(TRUE);
        }
        else
        {
          tdsrrcrbe_send_rbs_complete_message(FALSE);
        }
        /* send rbs indication to upperlayers */
        tdsrrcrbe_send_indication_to_upper_layers();
        /* prints rate for each transport channel */
        tdsrrcllc_print_trans_chnl_rate();

        /*if next state is Cell_PCH or URA_PCH then we have to
        treat the procedure as successful only after getting L2ack
        for the response message */
        if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
            (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH))
        {
          /* Change the substate to the next substate - this is
          where we wait for L2 ack from RLC for sending the RB RC
          complete message. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBS Complete msg");
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
        }
        else
        {
          /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
          configurations */
          tdsrrcllc_clear_ordered_config();

          /* Go to the next sub-state in the state machine. That's
          the initial substate since we've sent the RBS complete
          message */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* At this point we don't need to remember any of the
          Radio Bearer information since it's stored in ESTABLISHED_RABS.
          Clear our internal variables */
          tdsrrcrbe_clear_procedure();
        }
      }
      /*  In the case of Phy_Chan_Failure, RB setup failure must be send
      and oc should be reset. */
      else if(tdsrrc_rbe.status.failure_status == TDSRRCRB_PHY_CHAN_FAILURE)
      {

        /* Send the RB Setup Failure Message */
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                  tdsrrc_rbe.status.prot_err_cause
                                );

        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbe_clear_procedure();
        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure status %d not handled",tdsrrc_rbe.status.failure_status);
      }

    break;

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbe_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Setup Message. Otherwise
    we would not have received this command. Copy the
    received message pointer. */
    /* Process the RB Setup message */
    tdsrrcrbe_process_rbsetup_message( cmd_ptr );
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }

}


/*===========================================================================

FUNCTION   RRCRBE_FACH_WFCUCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_CELL_UPDATE_CNF substate
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

static void tdsrrcrbe_fach_wfcucnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      if(tdsrrc_rbe.status.failure_status == TDSRRCRB_SIMULTANEOUS_RECONFIG)
      {
        /* Send the RB Setup Failure Message */
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                  tdsrrc_rbe.status.prot_err_cause
                                );
        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbe_clear_procedure();

        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
      }
      else if(tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_VALID)
      {
        /* Send the Radio Bearer Setup Complete message */
        if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
            (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH))
        {
          tdsrrcrbe_send_rbs_complete_message(TRUE);
        }
        else
        {
          tdsrrcrbe_send_rbs_complete_message(FALSE);
        }
        /* send rbs indication to upperlayers */
        tdsrrcrbe_send_indication_to_upper_layers();
        /* prints rate for each transport channel */
        tdsrrcllc_print_trans_chnl_rate();
        /* if next state is Cell_PCH or URA_PCH then we have to 
           treat the procedure as successful only after getting L2ack
           for the response message */
        if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
            (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH))
        {
          /* Change the substate to the next substate - this is
             where we wait for L2 ack from RLC for sending the RB RC
             complete message. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Waiting for L2 ACK for RBS Complete msg");
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_L2_ACK;
        }
        else
        {
          /* Clear TDSORDERED_CONFIG - UE can now accept other ordered
             configurations */
          tdsrrcllc_clear_ordered_config();  

          /* Go to the next sub-state in the state machine. That's
             the initial substate since we've sent the RBS complete
             message */
          tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

          /* At this point we don't need to remember any of the
             Radio Bearer information since it's stored in ESTABLISHED_RABS.
             Clear our internal variables */
          tdsrrcrbe_clear_procedure();
        }
      }
      /*  In the case of Phy_Chan_Failure, RB setup failure must be send
      and oc should be reset. */
      else if(tdsrrc_rbe.status.failure_status == TDSRRCRB_PHY_CHAN_FAILURE)
      {
        /* Send the RB Setup Failure Message */
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                  tdsrrc_rbe.status.prot_err_cause
                                );

        /* Then clear all local variables that may
        need to be cleared */
        tdsrrcrbe_clear_procedure();
        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure status %d not handled",tdsrrc_rbe.status.failure_status);
      }

    break;

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbe_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Setup Message. Otherwise
    we would not have received this command. Copy the
    received message pointer. */
    /* Process the RB Setup message */
    tdsrrcrbe_process_rbsetup_message( cmd_ptr );
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }

}

/*===========================================================================

FUNCTION   RRCRBE_FACH_WF_CSCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_CELL_UPDATE_CNF substate
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

static void tdsrrcrbe_fach_wfcscnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_STATE_CHANGE_IND:
   /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );

    break;


  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Setup Message. Otherwise
    we would not have received this command. Copy the
    received message pointer. */
    /* Process the RB Setup message */
    tdsrrcrbe_process_rbsetup_message( cmd_ptr );
    break;
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  case TDSRRC_NEW_CELL_IND:
    tdsrrcrbe_process_new_cell_ind( &(cmd_ptr->cmd.new_cell_ind) );
    break;

  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    if(tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_FACH )
    {
  
      /* if OOS was detected during trans from DCH, cell update
         procedure has completed.  send channel config req to 
         complete the reconfiguration procedure
      */
      if ((tdsrrcllc_get_toc_usage() ==TDSTOC_FOR_OOS) 
          || (tdsrrcllc_get_toc_usage() == TDSTOC_FOR_OOS_WITHOUT_DCH_INFO)
          ||(tdsrrcllc_get_toc_usage() == TDSTOC_FOR_OOS_WITH_DCH_INFO)
         )
      {
        tdsrrc_cmd_type  *out_cmd_ptr;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
        /* Call the function that creates and sends the  channel
        config information to be requested from LLC. */
    
        if ((out_cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL)
        {
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
          memscpy(out_cmd_ptr,sizeof(tdsrrc_cmd_int_type),&tdsrrc_cmd,sizeof(tdsrrc_cmd_int_type));
#else
          out_cmd_ptr->cmd = tdsrrc_cmd.cmd;
          out_cmd_ptr->cmd_hdr = tdsrrc_cmd.cmd_hdr;
#endif

          tdsrrc_put_int_cmd( out_cmd_ptr );
        }
        else
        {
          ERR_FATAL("Could not get internal cmd buf",0,0,0);
        }
        /* Also reset the tdsrrcrbe_send_initiate_cu_flag to false as cell update
         * will not be needed since CU and CUCNF for oos took care 
         * of that
         */
        tdsrrcrbe_send_initiate_cu_flag = FALSE;
  
        /* Change the substate since we're waiting for the
        confirmation from LLC for the channel configs */
        tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
      
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
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }

}

/*===========================================================================

FUNCTION   RRCRBE_FACH_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_CHAN_CFG_CNF substate
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
static void tdsrrcrbe_fach_wfchcnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbe_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Setup Message. Otherwise
    we would not have received this command. Copy the
    received message pointer. */
    /* Process the RB Setup message */
    tdsrrcrbe_process_rbsetup_message( cmd_ptr );
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }
}



/*===========================================================================

FUNCTION   RRCRBE_OTHER_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_CHAN_CFG_CNF substate
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
static void tdsrrcrbe_other_wfchcnf_substate_event_handler
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
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbe_clear_procedure();
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring downlink sdu since UE not in DCH or FACH");
    /* In this state this procedure should no longer be active. */

    /* Then change to initial substate */
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbe_clear_procedure();
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }
}



/*===========================================================================

FUNCTION   RRCRBE_DCH_WFL2ACK_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_L2_ACK substate
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
static void tdsrrcrbe_dch_wfl2ack_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_DOWNLINK_L2ACK_IND:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbe_process_l2_ack_ind( &(cmd_ptr->cmd.downlink_l2ack_ind) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Setup Message. Otherwise
    we would not have received this command. Copy the
    received message pointer. */
    /* Process the RB Setup message */
    tdsrrcrbe_process_rbsetup_message( cmd_ptr );
    break;

  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    tdsrrc_rbe.status.failure_status = TDSRRCRB_CELL_UPDATE_OCCURED;
    /* Send the RBE Failure Message */
    tdsrrcrbe_send_rbs_failure_message(tdsrrc_rbe.status.failure_status,
                              tdsrrc_rbe.status.prot_err_cause
                             );

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbe_clear_procedure();

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }
}


/*===========================================================================

FUNCTION   RRCRBE_FACH_WFL2ACK_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_L2_ACK substate
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
static void tdsrrcrbe_fach_wfl2ack_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_DOWNLINK_L2ACK_IND:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbe_process_l2_ack_ind( &(cmd_ptr->cmd.downlink_l2ack_ind) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC. This
    SDU should contain a Radio Bearer Setup Message. Otherwise
    we would not have received this command. Copy the
    received message pointer. */
    /* Process the RB Setup message */
    tdsrrcrbe_process_rbsetup_message( cmd_ptr );
    break;


  case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    tdsrrc_rbe.status.failure_status = TDSRRCRB_CELL_UPDATE_OCCURED;
    /* Send the RBE Failure Message */
    tdsrrcrbe_send_rbs_failure_message(tdsrrc_rbe.status.failure_status,
                              tdsrrc_rbe.status.prot_err_cause
                             );

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbe_clear_procedure();

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }
}



/*===========================================================================

FUNCTION   RRCRBE_OTHER_WFL2ACK_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_L2_ACK substate
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
static void tdsrrcrbe_other_wfl2ack_substate_event_handler
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
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbe_clear_procedure();
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:

    /* A Downlink SDU has been received from RLC.*/
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring downlink SDU since UE not in DCH or FACH");
    /* In this state this procedure should no longer be active. */

    /* Then change to initial substate */
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;

    /* Then clear all local variables that may
    need to be cleared */
    tdsrrcrbe_clear_procedure();
    break;

  default:

    /* No other events except Downlink SDU is expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }
}

/*===========================================================================

FUNCTION   RRCRBE_FACH_WFCRNTIUPDATECNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the rrcRBE_WAIT_FOR_CRNTI_UPDATE_CNF substate
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

static void tdsrrcrbe_fach_wfcrntiupdatecnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_STATE_CHANGE_IND:
   /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );

    break;


  case TDSRRC_RNTI_UPDATE_CNF:
    if (cmd_ptr->cmd.rnti_update_cnf.status == SUCCESS) 
    {
      tdsrrcrbe_process_rnti_update_cnf();
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RNTI update failed.  Send RBS failure msg");
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_VALID;
      
      tdsrrcrbe_send_rbs_failure_message(tdsrrc_rbe.status.failure_status,
                               tdsrrc_rbe.status.prot_err_cause);
      
      /* Then clear all local variables that may
         need to be cleared */
      tdsrrcrbe_clear_procedure();

      /* Make sure we're back in the initial sub-state */
      tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
    }
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }

}


/*===========================================================================

FUNCTION   RRCRBE_PCH_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_CHAN_CFG_CNF substate
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
static void tdsrrcrbe_pch_wfchcnf_substate_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{


  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_CHANNEL_CONFIG_CNF:

    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbe_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:

    /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;

  case TDSRRC_DOWNLINK_SDU_IND:
  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }
}

/*===========================================================================

FUNCTION   RRCRBE_PCH_WFCUCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the RRCRBE_WAIT_FOR_CELL_UPDATE_CNF substate
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
static void tdsrrcrbe_pch_wfcucnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_CHANNEL_CONFIG_CNF:
    /* Confirmation from LLC for the channel configurations */
    tdsrrcrbe_process_channel_config_cnf( &(cmd_ptr->cmd.chan_config_cnf) );
    break;

  case TDSRRC_STATE_CHANGE_IND:
    /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );
    break;



    case TDSRRC_DOWNLINK_SDU_IND:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unexpected DOWNLINK_SDU_IND in substate %d",tdsrrc_rbe.curr_substate);
    break;

  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }

}

/*===========================================================================

FUNCTION   RRCRBE_PCH_WF_CSCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the RBE procedure is in the rrcRBE_WAIT_FOR_CELL_SELECTION_CNF substate
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

static void tdsrrcrbe_pch_wfcscnf_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{

  /* Switch on the received command's id */
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {

  case TDSRRC_STATE_CHANGE_IND:
   /* Indication that RRC's state has changed */
    tdsrrcrbe_process_state_change_ind( &(cmd_ptr->cmd.state_change_ind) );

    break;


  default:

    /* No other events expected in this
    substate */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Ignoring RBE event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }

}



/*===========================================================================

FUNCTION   RRCRBE_CELL_DCH_EVENT_HANDLER

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
static void tdsrrcrbe_cell_dch_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  if ((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_L2ACK_IND) &&
      (tdsrrc_rbe.curr_substate != TDSRRCRB_WAIT_FOR_L2_ACK))
  {
    /* We could get L2ack for a previously sent message while we are already
    processing the next message. So l2ack could be received any any of the following
    substates. If RBE substate does not indicate that we are not waiting for l2ack
    then just print the status */
    if (cmd_ptr->cmd.downlink_l2ack_ind.status == SUCCESS)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Recd L2ack for MUI:%d, Lc_Id:%d, rbe_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui,
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_rbe.curr_substate);
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2ack failed for MUI:%d, Lc_Id:%d, rbe_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui,
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_rbe.curr_substate);
    }
    /* return from here so that procedure can continue with existing process */
    return;
  }
  /* Switch on the current substate */
  switch ( tdsrrc_rbe.curr_substate )
  {
  case TDSRRCRB_INITIAL:

    /* Call the event handler for RRCRBE_INITIAL substate */
    tdsrrcrbe_initial_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:

    /* Call the event handler for RRCRBE_WAIT_FOR_CHAN_CFG_CNF substate */
    tdsrrcrbe_dch_wfchcnf_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_L2_ACK:

    /* Call the event handler for RRCRBE_WAIT_FOR_L2_ACK substate */
    tdsrrcrbe_dch_wfl2ack_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_REESTABLISH_SRB2:

    /* Call the event handler for RRCRBE_WAIT_FOR_REESTABLISH_SRB2 substate */
    tdsrrcrbe_dch_wfsrb2reestablish_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
    /* Call the event handler for TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF substate */
    tdsrrcrbe_dch_wfcucnf_event_handler( cmd_ptr);
    break;

  default:

    /* No other substates processed for this RRC state    */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Command %x not processed in CELL_DCH for substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;

  }
}



/*===========================================================================

FUNCTION   RRCRBE_CELL_FACH_EVENT_HANDLER

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
static void tdsrrcrbe_cell_fach_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  if ((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_L2ACK_IND) &&
      (tdsrrc_rbe.curr_substate != TDSRRCRB_WAIT_FOR_L2_ACK))
  {
    /* We could get L2ack for a previously sent message while we are already
    processing the next message. So l2ack could be received any any of the following
    substates. If RBE substate does not indicate that we are not waiting for l2ack
    then just print the status */
    if (cmd_ptr->cmd.downlink_l2ack_ind.status == SUCCESS)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Recd L2ack for MUI:%d, Lc_Id:%d, rbe_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui,
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_rbe.curr_substate);
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2ack failed for MUI:%d, Lc_Id:%d, rbe_subst:%d",
               cmd_ptr->cmd.downlink_l2ack_ind.mui,
               cmd_ptr->cmd.downlink_l2ack_ind.lc_id, tdsrrc_rbe.curr_substate);
    }
    /* return from here so that procedure can continue with existing process */
    return;
  }
  /* Switch on the current substate */
  switch ( tdsrrc_rbe.curr_substate )
  {
  case TDSRRCRB_INITIAL:
    /* Call the event handler for RRCRBE_INITIAL substate */
    /* NOTE: Here we need to check if a cell-update procedure
    has been initialized or not. For now, just call the event handler */
    tdsrrcrbe_initial_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
    /* Call the event handler for RRCRBE_WAIT_FOR_CELL_UPDATE_CNF substate */
    tdsrrcrbe_fach_wfcucnf_event_handler( cmd_ptr);
    break;

  case TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF:
    tdsrrcrbe_fach_wfcscnf_event_handler( cmd_ptr);
    break;

  case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:
    /* Call the event handler for RRCRBE_WAIT_FOR_CHAN_CFG_CNF substate */
    tdsrrcrbe_fach_wfchcnf_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF:
    /* Call the event handler for RRCRBE_WAIT_FOR_CRNTI_UPDATE_CNF substate */
    tdsrrcrbe_fach_wfcrntiupdatecnf_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_L2_ACK:
    /* Call the event handler for RRCRBE_WAIT_FOR_L2_ACK substate */
    tdsrrcrbe_fach_wfl2ack_substate_event_handler( cmd_ptr );
    break;

  default:
    /* No other substates processed for this RRC state    */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Command %x not processed in CELL_FACH for substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }
}


/*===========================================================================

FUNCTION   RRCRBE_PCH_EVENT_HANDLER

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
static void tdsrrcrbe_pch_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the current substate */
  switch ( tdsrrc_rbe.curr_substate )
  {
  case TDSRRCRB_INITIAL:

    /* Call the event handler for RRCrbe_INITIAL substate */
    tdsrrcrbe_initial_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
    /* Call the event handler for RRCRBE_WAIT_FOR_CELL_UPDATE_CNF substate */
    tdsrrcrbe_pch_wfcucnf_event_handler( cmd_ptr);
    break;

  case TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF:

    tdsrrcrbe_pch_wfcscnf_event_handler( cmd_ptr);
    break;

  case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:

    /* Call the event handler for RRCRBE_WAIT_FOR_CHAN_CFG_CNF substate */
    tdsrrcrbe_pch_wfchcnf_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_L2_ACK:

    /* The only way we could be in pch and rb state is "wait_for_l2_ack is if
       oos happened while waiting for l2 ack and cell update confirm takes UE to pch
       In this case, send failure msg of tdsrrc_rbrc with cause cell update occured
    */
    tdsrrc_rbe.status.failure_status = TDSRRCRB_CELL_UPDATE_OCCURED;

    tdsrrcrbe_send_rbs_failure_message(tdsrrc_rbe.status.failure_status,
                              tdsrrc_rbe.status.prot_err_cause);

    /* Then clear all local variables that may
       need to be cleared */
    tdsrrcrbe_clear_procedure();

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
    break;

  default:

    /* No other substates processed for this RRC state    */
    TDSRRC_MSG3(MSG_LEGACY_MED,"Command %x not processed in substate%d rrc_state%d",
            cmd_ptr->cmd_hdr.cmd_id, tdsrrc_rbe.curr_substate, tdsrrc_get_state());
    break;

  }
}


/*===========================================================================

FUNCTION   RRCRBE_OTHER_STATE_EVENT_HANDLER

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
static void tdsrrcrbe_other_state_event_handler
(
tdsrrc_cmd_type *cmd_ptr
)
{
  /* Switch on the current substate */
  switch ( tdsrrc_rbe.curr_substate )
  {

  case TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF:

    /* Call the event handler for WAIT_FOR_CHAN_CFG_CNF substate */
    tdsrrcrbe_other_wfchcnf_substate_event_handler( cmd_ptr );
    break;

  case TDSRRCRB_WAIT_FOR_L2_ACK:

    /* Call the event handler for WAIT_FOR_L2_ACK substate */
    tdsrrcrbe_other_wfl2ack_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF:
      tdsrrcrbe_other_wfcucnf_substate_event_handler( cmd_ptr);
      break;

    case TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF:
      tdsrrcrbe_other_wfcscnf_event_handler( cmd_ptr);
      break;

    default:
    /* No other substates processed for this RRC state    */
    TDSRRC_MSG2(MSG_LEGACY_MED,"Command %x not processed in IDLE or PCH states for substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_rbe.curr_substate);
    break;
  }
}



/*===========================================================================

FUNCTION RRCRBE_PROCEDURE_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  Radio Bearer Establishment procedure. This function is called by the
  dispatcher whenever an event is received for this module. This function
  checks the current RRC state and calls the event handler for
  that state.

  Note that the Radio Bearer Establishment procedure can be initiated
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
void tdsrrcrbe_procedure_event_handler
(
tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  /* Get the current RRC State */
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();


  if ((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_STATE_CHANGE_IND) &&
      (cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED)) 
  {
    /* init mac_rab_Status to no rabs */
    tdsmac_rab_status = TDSNO_RABS;
    TDSRRC_MSG1(MSG_LEGACY_LOW,"MAC_RAB status;%d",tdsmac_rab_status);
  } 

  switch (rrc_state)
  {
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Recvd RBE message in %d",rrc_state);
  case TDSRRC_STATE_CELL_FACH:
    /* If it's CELL_FACH state, call the event handler
    for CELL_FACH state */
    tdsrrcrbe_cell_fach_event_handler(cmd_ptr);
    break;

  case TDSRRC_STATE_CELL_DCH:
    /* If it's CELL_DCH state, call the event handler
    for CELL_DCH state */
    tdsrrcrbe_cell_dch_event_handler(cmd_ptr);
    break;

    case TDSRRC_STATE_URA_PCH:
    case TDSRRC_STATE_CELL_PCH:
      /* If it's CELL_DCH state, call the event handler
      for CELL_PCH state */
      tdsrrcrbe_pch_event_handler(cmd_ptr);
      break;

  case TDSRRC_STATE_CONNECTING:
  case TDSRRC_STATE_DISCONNECTED:
    /* If it's any other state call the event handler
    for other states. The processing of events in
    other RRC states is identical for this procedure */
    tdsrrcrbe_other_state_event_handler(cmd_ptr);
    break;

  default:
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC State: %d",rrc_state);
    break;
  }
}/* end function tdsrrcrbe_procedure_event_handler */



/*===========================================================================

FUNCTION  RRCRBE_INIT_PROCEDURE

DESCRIPTION

  This function initializes the RBE procedure.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbe_init_procedure( void )
{
  uint32 count;

  tdsrrcrbe_send_initiate_cu_flag = FALSE;
  tdsrrcrbe_delete_crnti          = FALSE;

  /* Initialize the transaction id's for this procedure.
  Use a number that can't be an actual received transaction
  id. */
  tdsrrc_rbe.tr_ids.accepted_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;
  tdsrrc_rbe.tr_ids.rejected_transaction = TDSRRCRB_INVALID_TRANSACTION_ID;

  /* Initialize the current substate */
  tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  /* Initialize CRNTI valid to FALSE */
  tdsrrc_rbe.new_crnti_valid    = FALSE;
  tdsrrc_rbe.directed_cell_info = FALSE;

  tdsrrc_rbe.rbe_for_srns_relocation = FALSE;
  tdsrrc_rbe.new_urnti_valid = FALSE;

  /* Initialize the RABS per message structure */
  for ( count=0; count < TDSRRCRB_MAX_RAB_PER_MSG; count++ )
  {
    tdsrrc_rbe.rabs.rabs_in_msg[count].num_rbs_for_rab = 0;
    tdsrrc_rbe.rabs.rabs_in_msg[count].rab_type= TDSRRCRB_UNKNOWN_RAB;
    tdsrrc_rbe.rabs.chan_config_needed[count] = FALSE;
    tdsrrc_rbe.rabs.rabs_in_msg[count].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_NONE;
  }
  tdsrrc_rbe.rabs.num_rabs_in_msg = 0;
  /* Init mac rab status to NO_RABS */
  tdsmac_rab_status = TDSNO_RABS;
  
  tdsrrc_rbe.first_tm_rb_setup = FALSE;
  
  /* Register for state change notification -
  We want to be notified when the state changes
  from CELL_DCH to CELL_FACH and from any state
  to disconnected state. */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBE,  /* Procedure name */
                             TDSRRC_STATE_CELL_DCH, /* From State     */
                             TDSRRC_STATE_CELL_FACH /* To State       */
                           );

  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBE,  /* Procedure name */
                             TDSRRC_STATE_WILDCARD, /* From State     */
                             TDSRRC_STATE_DISCONNECTED /* To State    */
                           );

  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBE,  /* Procedure name */
                             TDSRRC_STATE_CELL_DCH, /* From State     */
                             TDSRRC_STATE_CELL_PCH /* To State       */
                           );
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBE,  /* Procedure name */
                             TDSRRC_STATE_CELL_DCH, /* From State     */
                             TDSRRC_STATE_URA_PCH /* To State       */
                           );
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU

  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBE,  /* Procedure name */
                             TDSRRC_STATE_CELL_FACH, /* From State     */
                             TDSRRC_STATE_CELL_PCH /* To State       */
                           );

  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_RBE,  /* Procedure name */
                             TDSRRC_STATE_CELL_FACH, /* From State     */
                             TDSRRC_STATE_URA_PCH /* To State       */
                           );
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
  /* Define a timer used to delay 5 seconds before
  sending the RB Setup Complete message */
  rex_def_timer(&tdsrrcrbe_temp_timer, rex_self(), TDSRRCRBE_TEMP_TIMER_SIG);


}
/*===========================================================================

FUNCTION tdsrrcrbe_get_est_rab_ptr

DESCRIPTION

  This function returns established rabs ptr to SMC procedure

DEPENDENCIES

  None.

RETURN VALUE

  Pointer to established rabs

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_rab_info_per_msg_type  * tdsrrcrbe_get_est_rab_ptr(void)
{
  return &(tdsrrc_rbe.rabs);
}

/*===========================================================================

FUNCTION   PROCESS_RBSETUP_R4

DESCRIPTION

  This function processes the Rel 4 RB Setup Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Setup Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -

    TDSORDERED_CONFIG will be set by calling LLC.

    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.

    If TDSORDERED_CONFIG could not be set, a Radio Bearer Setup Failure message
    will be sent to UTRAN.

     the function handle is copy from tdsrrcrbe_process_rbsetup_r5

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbe_process_rbsetup_r4
(
tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_DL_DCCH_Message     *dcch_msg_ptr; /* Pointer to the RB Setup message */
  tdsrrc_RadioBearerSetup *msg_ptr;    /* Pointer to the RB Setup message        */

  tdsrrc_rab_search_e_type tdsrab_status = TDSRRC_RAB_NOT_FOUND; /* To check if RAB is present in ESTABLISHED_RABS */
  tdsrrcllc_oc_status_e_type oc_status;/* To hold tdsordered_config status          */
  uint32 rab_index=0;               /* Used when more than 1 rab is present in
                                    a RBS message                             */
  tdsrrc_int_c_rnti_type      c_rnti;       /* Local C_RNTI value */
  boolean ordered_config_err_occured = FALSE; /* Used to indicate if an error
                                    occured while setting tdsordered_config. */
 
  tdsrrc_RadioBearerSetup_r4_IEs* rbsetup_ptr = NULL;

  tdsrrcsmc_srns_ip_check_status_e_type    ip_check_status = TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;

   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  boolean primary_plmn_Id_present = FALSE;
   /*Variable to hold primary plmn i.d if present in OTA msg*/
  tdsrrc_PLMN_Identity primary_plmn_Identity;
  tdsrrcrb_msg_failure_cause_type temp_failure_cause;
  uint8 temp_rejected_transaction;


  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:rbsetupr4 cur_substate %d",tdsrrc_rbe.curr_substate);

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Get the pointer to the actual RB Setup message */
  msg_ptr = &(dcch_msg_ptr->message.u.radioBearerSetup);


  /* The first thing to do is check if Rel 4 contents are present */
  if (msg_ptr->u.later_than_r3.criticalExtensions.t ==
       T_tdsrrc_RadioBearerSetup_criticalExtensions_6_r4)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:rbsetupr4 rcvd");

    /* Store Msg Ptr in a local variable */
    rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerSetup_r4;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported RB Setup t:%d",msg_ptr->t);
    temp_failure_cause.failure_status = tdsrrc_rbe.status.failure_status;
    temp_failure_cause.prot_err_cause = tdsrrc_rbe.status.prot_err_cause;
    
    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
    tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;

    if ((msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
        == tdsrrc_rbe.tr_ids.rejected_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBS msg! rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
      /* Nothing else to do, just ignore the message and return */
    }
    else
    {
      temp_rejected_transaction = tdsrrc_rbe.tr_ids.rejected_transaction;
      tdsrrc_rbe.tr_ids.rejected_transaction = msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;

      /*Check whether Cell Update is going on */
      if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
         (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
      {
        if(tdsrrc_rbe.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbe.curr_substate,tdsrrc_rbe.status.failure_status);
          tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                     tdsrrc_rbe.status.prot_err_cause );
          
           tdsrrc_rbe.status.failure_status = temp_failure_cause.failure_status;
           tdsrrc_rbe.status.prot_err_cause = temp_failure_cause.prot_err_cause ;
           tdsrrc_rbe.tr_ids.rejected_transaction = temp_rejected_transaction;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
          (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);

          /* Wait until Cell update procedure is completed */
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
        }
      }
      else
      {
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                tdsrrc_rbe.status.prot_err_cause );
      }
    }

    return;
  }
  
  /* copy checking integrity and srns relocation handle from R3 */
  if (TRUE == tdsrrcrbe_validate_srns_relocation_info(msg_ptr, TDSMSG_REL4))
  {
    ip_check_status = tdsrrcsmc_check_integrity_for_srns_relocation(cmd_ptr, TDSMSG_REL4);
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
                       == tdsrrc_rbe.tr_ids.rejected_transaction)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBE msg - rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
          /* Nothing else to do, just ignore the message and return */
        }
        else
        {
          /* send a reconfig failure messge with invalid
         configuration and
         continue the same substate with any ongoing procedure */
          temp_rejected_transaction = tdsrrc_rbe.tr_ids.rejected_transaction;
          temp_failure_cause.failure_status = tdsrrc_rbe.status.failure_status;
          tdsrrc_rbe.tr_ids.rejected_transaction =
            (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
          if (ip_check_status == TDSRRCSMC_SRNS_SECURITY_INFO_INVALID)
          {
            tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          }
          else
          {
            tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
          }
          /*Check whether Cell Update is going on */
          if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
               (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
          {
            if(tdsrrc_rbe.curr_substate == TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF)
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_rbe subsstate(%d) Send failure with %d cause",tdsrrc_rbe.curr_substate,tdsrrc_rbe.status.failure_status);
              tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                         tdsrrc_rbe.status.prot_err_cause );
              tdsrrc_rbe.tr_ids.rejected_transaction = temp_rejected_transaction;
              tdsrrc_rbe.status.failure_status = temp_failure_cause.failure_status;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
              (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
              /* Wait until Cell update procedure is completed */
              tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
            }
          }
          else
          {
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                            tdsrrc_rbe.status.prot_err_cause );
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
      /*do not clear the procedure or change the tdsrrc_rbe substate because
     some other tdsrrc_rbe procedure might be in progress. Just return from
     here */
      return;
    }

  }

  /* copy handlle from R5(R5 is the same as R3) */
  if ((tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET) ||
      (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
      (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incomatible simultaneous reconfig! Rejecting RBS message");
    if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
         == tdsrrc_rbe.tr_ids.accepted_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBS msg! accepted tr-id%d match!",tdsrrc_rbe.tr_ids.accepted_transaction);
      /* Nothing else to do, just return */
    }
    else if ( (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier)
                == tdsrrc_rbe.tr_ids.rejected_transaction )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Duplicate RBS msg! rejected tr-id%d match!",tdsrrc_rbe.tr_ids.rejected_transaction);
      /* Nothing else to do, just return */
    }
    else
    {
      /* We have received a RBS message with a new transaction id.
      This message should be in our rejected transaction
        list */

      tdsrrc_rbe.tr_ids.rejected_transaction =
      (msg_ptr->u.later_than_r3.rrc_TransactionIdentifier);
      tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;

      /*Check whether Cell Update is going on */
      if ( (tdsrrcccm_is_reselection_in_progress() == TRUE)     ||
           (TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status() ))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update Started! Register for CU CNF");
        (void)tdsrrccu_register_for_cell_update_to_complete(TDSRRC_PROCEDURE_RBE);
        /* Wait until Cell update procedure is completed */
        tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF;
      } else
      {
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                  tdsrrc_rbe.status.prot_err_cause );
        /*continue the same substate */
      }
    }
    return;
  }

  /* We have received a RB setup message with a new transaction id.
  Accept the transaction Id */
  tdsrrc_rbe.tr_ids.accepted_transaction =
  msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;


#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  /*DCH->DCH only 
    do not touch any tdsrrc_pcrc.status.failure_status and internal states*/

  if ((rbsetup_ptr->rrc_StateIndicator == tdsrrc_RRC_StateIndicator_cell_DCH)
      &&(TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
  {
    tdsrrcrb_sdu_lifetime_act_time_valid_type sdu_valid_result;
    sdu_valid_result = tdsrrc_validate_sdu_lifetime_acttime(
         rbsetup_ptr->m.activationTimePresent,
         rbsetup_ptr->activationTime
       );
    if(sdu_valid_result.sdu_discard == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RBE SDU is received before CU");
      return;  
    }
    else if(sdu_valid_result.sdu_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SDU Lifetime validation failed");
      tdsrrcrbe_send_lifetime_validation_failure(TDSRRCRB_PHY_CHAN_FAILURE);
      return; 
    }
    else if (sdu_valid_result.act_time_valid == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Force Activation time to NOW");
      rbsetup_ptr->m.activationTimePresent = 0;
    }
  }

#endif


  tdsrrcrbe_validate_rbsetup_r4(msg_ptr);

  /* copy CIPHERING handle from R5(R5 is the same as R3) */
  if ( tdsrrc_rbe.status.failure_status != TDSRRCRB_MSG_VALID )
  {
    /* RB Message was invalid. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB Message validation failed");

    /* Send the RB Setup Failure Message */
    tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                              tdsrrc_rbe.status.prot_err_cause
                            );

    /* Make sure we're back in the initial sub-state */
    tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
    tdsrrcrbe_clear_procedure();
  }
  /* If the message is valid, proceed further */
  else
  {
    tdsrrcrbe_indicate_layer1_if_rb_setup_configure_voice_rab();

    /* If new rabs are setup, update mac rab status */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
        tdsrrc_RadioBearerSetup_r4_IEs,rab_InformationSetupList))
    {
      tdsrrcrb_update_mac_rab_status(
        TDSRRC_PROCEDURE_RBE,
        tdsrrc_rbe.rabs.rabs_in_msg[0].cn_domain,
        NULL,
        FALSE);
    }
    
    /*calculate tdsciphering_activation_cfn only if ciphering is enabled*/
    if (tdsrrcsmc_is_ciphering_enabled() == TDSRRCSMC_SECURITY_STARTED)
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
        tdsrrc_RadioBearerSetup_r4_IEs,activationTime))
      {
        /*256 is the CFN rollover length*/
        /*250 is used to activate ciphering after 250 10ms time interval,
        this should give sufficient time for n/w side and ue side to have
        configured ciphering on the rb's successfully*/
        tdsciphering_activation_cfn =
        (uint8) ((rbsetup_ptr->activationTime + 250) % 256);
        /*round it to 8 frame boundary */
        tdsciphering_activation_cfn = (uint8)((tdsciphering_activation_cfn/8)*8);

      }
      else
      {
        /*256 is the CFN rollover length*/
        /*250 is used to activate ciphering after 250 10ms time interval,
        this should give sufficient time for n/w side and ue side to have
        configured ciphering on the rb's successfully*/
        tdsciphering_activation_cfn = (uint8)((tdsseq_get_cfn() + 250) % 256);
        /*round it to 8 frame boundary */
        tdsciphering_activation_cfn = (uint8)((tdsciphering_activation_cfn/8)*8);
      }
    }

    /*call rrcsmc function to update ciphering activation cfn*/
    tdsrrcsmc_update_ciphering_activation_cfn(tdsciphering_activation_cfn);

    /* Copy CN information handle from R5(R5 is the same as R3) */
    /* Check if there is CN information present. */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r4_IEs,cn_InformationInfo))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving CN info for NAS");
      /*Check to see if primary PLMN I.D is present in RBE message ext
       * radioBearerSetup-v690ext which is present in 
       * v690NonCriticalExtensions
       */
       primary_plmn_Id_present = tdsrrcrbe_check_and_get_primary_plmn_id(msg_ptr,
                                                                  &primary_plmn_Identity,TDSMSG_REL4);
      tdsrrc_rbe.mm_cmd_ptr = tdsrrcccm_buid_service_ind_with_nas_cn_info(
                     &(rbsetup_ptr->cn_InformationInfo) 
                     ,primary_plmn_Id_present
                     ,&primary_plmn_Identity
          );
      if (tdsrrc_rbe.mm_cmd_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not build SERVICE_IND cmd");
      }
      else
      {
        /* set cn_info_saved flag */
        tdsrrc_rbe.cn_info_saved = TRUE;
      }

    }

    /* Copy the rab infomation hadle from R5(R5 is the same as R3) */
    /* Check if there are new RABs to be established */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
        tdsrrc_RadioBearerSetup_r4_IEs,rab_InformationSetupList))
    {
      /* Check if the RAB id is already in ESTABLISHED_RABS */
      for (rab_index=0; rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg; rab_index++)
      {
        tdsrab_status = tdsrrc_find_rab_in_established_rabs
                     ( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id );
        tdsrrc_rbe.rabs.chan_config_needed[rab_index] = TRUE;
        if (tdsrab_status != TDSRRC_RAB_NOT_FOUND)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB %d already exists in EST_RABS!",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id);
          break;
        }
      }

#ifdef FEATURE_SCUDIF_ENHANCEMENT
      #error code not present
#endif

      if ( (tdsrab_status == TDSRRC_RAB_NOT_FOUND) || 
           (tdsrrc_find_if_voice_rab_exists_in_established_rabs( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id )
               == TRUE))
      {
        /* We have to establish this RAB. First set TDSORDERED_CONFIG
        so that LLC can process the necessary parameters.
          This function will copy all RABs from the rxd msg to oc. */
        oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBE,
                                               tdsrrc_rbe.state_from_msg,
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

            /* Ordered Config has been set. We can setup the
              Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
            /* Call the function that creates and sends a channel
            config request to LLC. */
            tdsrrcrbe_send_channel_config_req_r4( msg_ptr );
  
            if (((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
                (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
                (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
            {
              if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
                  tdsrrc_RadioBearerSetup_r4_IEs,new_C_RNTI))
              {
                tdsrrc_rbe.new_crnti_valid = TRUE;
                tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI,
                                    &c_rnti);
                tdsrrc_rbe.new_crnti = (uint16)c_rnti;
              }
              else
              {
                tdsrrc_rbe.new_crnti_valid = FALSE;
              }
              tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
            }
            else
            {
/* for "RRC_Issues_Management_QC" No.22 begin */
              if( tdsrrc_rbe.curr_substate != TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF )
              {
              /* Change the substate since we're waiting for the
                confirmation from LLC for the channel configs */
                tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
              }
/* for "RRC_Issues_Management_QC" No.22 end */
            }
            break;
  
          case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
            tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
            /* Send the RB Setup Failure Message */
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                      tdsrrc_rbe.status.prot_err_cause
                                    );
  
            /* Go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
  
          case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
            tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
            /* Send the RB Setup Failure Message */
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                      tdsrrc_rbe.status.prot_err_cause
                                    );
  
            /* Go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;

       case TDSORDERED_CONFIG_CONFIGURATION_INVALID:
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
            tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
            /* Send the RB Setup Failure Message */
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                      tdsrrc_rbe.status.prot_err_cause
                                    );
  
            /* Go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
  
          case TDSORDERED_CONFIG_NOT_SET_OTHER:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
            /* When LLC sends this, there is a serious problem
            and we'll go back to disconnected state. No need
            to send the failure message here, we can just
            go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
  
          default:
  
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
            /* Unknown error, just go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
        }/* end switch */

        if ( ordered_config_err_occured == TRUE )
        {
          /* No need to continue. Clear the procedure
            variables and return. */
          tdsrrcrbe_clear_procedure();
          return;
        }

        if (tdsrrc_rbe.rbe_for_srns_relocation == TRUE)
        {
          tdsrrcsmc_commit_fresh_for_srns_relocation();
        }

      }
      else
      {
        /* send a failure message if the rab in the message  already
        exists in established_rabs. Later add a check for the rbs
        associated with the rabs so that the failure will be sent only when
        any of the rb-ids match with the rb-ids in established_rab for that
          rab */
        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        /* Send the RB Setup Failure Message */
        tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                  tdsrrc_rbe.status.prot_err_cause
                                );

        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
        tdsrrcrbe_clear_procedure();
        return;
      }
    }




    /* If RABs need to be setup, the RBs to be setup for
    that RAB and any other RBs to be re-configured as indicated
    by the message will be sent in the same channel config
    command. But if no new RBs have been setup, then we
    need to send a separate command for any RBs that
    need to be re-configured */
    if ( tdsrrc_rbe.rabs.chan_config_needed[0] == FALSE )
    {
      /* RABs are not being setup for this message.
      Even though we don't have to setup RABs, there may
      be signalling RBs to be reconfigured in the message. Check
        if there are any */

      if (((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r4_IEs,rb_InformationAffectedList)) &&
          (rbsetup_ptr->rb_InformationAffectedList.n != 0 )) ||
          ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r4_IEs,srb_InformationSetupList)) &&
          (rbsetup_ptr->srb_InformationSetupList.n != 0)))
      {

        /* We have to reconfigure these RBs. First set TDSORDERED_CONFIG
          so that LLC can process the necessary parameters. */
        oc_status = tdsrrcllc_set_ordered_config( TDSRRC_PROCEDURE_RBE,
                                               tdsrrc_rbe.state_from_msg,
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
            /* Ordered Config has been set. We can setup the
              Radio Bearers now using the TDSRRC_CHANNEL_CONFIG_REQ command. */
            TDSRRC_MSG0(MSG_LEGACY_LOW,"Sending TDSRRC_CHANNEL_CONFIG_REQ to LLC");
            /* Call the function that creates and sends a channel
            config request to LLC. */
            tdsrrcrbe_send_channel_config_req_r4( msg_ptr );
  
            if (((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
                (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
                (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
            {
              if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
                  tdsrrc_RadioBearerSetup_r4_IEs,new_C_RNTI))
              {
                tdsrrc_rbe.new_crnti_valid = TRUE;
                tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI,
                                    &c_rnti);
                tdsrrc_rbe.new_crnti = (uint16)c_rnti;
              }
              else
              {
                tdsrrc_rbe.new_crnti_valid = FALSE;
              }
              tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
            }
            else
            {
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
              if( tdsrrc_rbe.curr_substate != TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF )
  #endif/* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
              {
                /* Change the substate since we're waiting for the
                confirmation from LLC for the channel configs */
                tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF;
              }
            }
  
            break;
  
          case TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG:
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned simultaneous reconfig!");
            tdsrrc_rbe.status.failure_status = TDSRRCRB_SIMULTANEOUS_RECONFIG;
            /* Send the RB Setup Failure Message */
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                      tdsrrc_rbe.status.prot_err_cause
                                    );
  
            /* Go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
  
          case TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED:
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned unsupported config!");
            tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
            /* Send the RB Setup Failure Message */
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                      tdsrrc_rbe.status.prot_err_cause
                                    );
  
            /* Go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
 
    case TDSORDERED_CONFIG_CONFIGURATION_INVALID:
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned invalid config!");
            tdsrrc_rbe.status.failure_status =TDSRRCRB_MSG_INVALID_CONFIG;
            /* Send the RB Setup Failure Message */
            tdsrrcrbe_send_rbs_failure_message( tdsrrc_rbe.status.failure_status,
                                      tdsrrc_rbe.status.prot_err_cause
                                    );
  
            /* Go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
  
  
          case TDSORDERED_CONFIG_NOT_SET_OTHER:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC returned other failure!");
            /* When LLC sends this, there is a serious problem
            and we'll go back to disconnected state. No need
            to send the failure message here, we can just
            go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
  
          default:
  
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown return value from LLC");
            /* Unknown error, just go back to initial substate */
            tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
  
            /* Indicate that an error occured */
            ordered_config_err_occured = TRUE;
            break;
        }/* end switch */

        if ( ordered_config_err_occured == TRUE )
        {
          /* No need to continue. Clear the procedure
            variables and return. */
          tdsrrcrbe_clear_procedure();
          return;
        }
      }
      else
      {
        /*If we are here, it means that RABs are not given in RB-Setup.
        Also, SRBs and RB Affected IEs are not given.*/

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No RB IEs are given in the OTA.");
        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_informationElementMissing;  

        /* Send the RB Setup Failure Message */
        tdsrrcrbe_send_rbs_failure_message(tdsrrc_rbe.status.failure_status, tdsrrc_rbe.status.prot_err_cause);
        /* Make sure we're back in the initial sub-state */
        tdsrrc_rbe.curr_substate = TDSRRCRB_INITIAL;
        tdsrrcrbe_clear_procedure();

        return;          
      }/* end if - RBs need to be reconfigured */
    }/* end if - tdsrrc_rbe.rabs.chan_config_needed[rab_index] == FALSE */
  }/* end if - valid rb message */
}/* end function process_rbsetup_message_r5 */

/*===========================================================================

FUNCTION   VALIDATE_RBSETUP_R4

DESCRIPTION

  This function validates the received R4 RB Setup Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Setup Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.

  The function handle is copy from tdsrrcrbe_validate_rbsetup_r5

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbe_validate_rbsetup_r4
(
tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
)
{

  tdsrrc_RadioBearerSetup_r4_IEs* rbsetup_ptr = NULL;

  tdsrrc_RAB_InformationSetupList_r4 *rab_list_ptr = NULL;

  tdsrrc_RB_InformationSetupList_r4 *rb_list_ptr = NULL;

#ifdef FEATURE_UMTS_PDCP

  tdsrrc_HeaderCompressionInfo_r4 *pdcpHeaderCompressionInfo = NULL;

#endif /* FEATURE_UMTS_PDCP*/


  boolean new_tm_rb = FALSE;
  rlc_lc_id_type local_lc_id;
  uecomdef_logch_mode_e_type rx_mode;
  tdsrrclcm_status_e_type result;
  boolean dl_tm_flag;

  rrc_rab_id_type rab_id;           /* To store RAB id in internal RRC format */
  rrcmn_rab_status_T rab_type = RRCMN_INVALID_RAB_ID;     /* To store the status returned by MN layer. This
                                    indicates what type of RAB is received. This is
                                    used only if the RAB is for the CS_domain */
  uint32 rab_index=0;               /* Used as an index when storing established
                                    RABs locally. Only when we have more than
                                    one RAB per message, this will be incremented */
  uint32 rb_index =0;                          
  boolean voice_rab_already_exists = FALSE;
 /* set the default nas_sync_ind to the current codec used*/
  uint32 nas_sync_ind = tdsrrc_get_current_codec_mode();
  /* Assume the message is valid for now */
  tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_VALID;


  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerSetup_r4;
  switch (rbsetup_ptr->rrc_StateIndicator)
  {
  case tdsrrc_RRC_StateIndicator_cell_DCH:

    tdsrrc_rbe.state_from_msg = TDSRRC_STATE_CELL_DCH;
    break;

  case tdsrrc_RRC_StateIndicator_cell_FACH:

    tdsrrc_rbe.state_from_msg = TDSRRC_STATE_CELL_FACH;
    break;

  default: /* Just to satisfy LINT */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid RRC state from Msg");
    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    break;

  } /* End of switch */

  if ( tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG )
  {
    /* No need to continue */
    return;
  }

  rab_index =0;
  /* Copy RAB info check from R5 */
  /* Check if there are new RABs to be established */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
    tdsrrc_RadioBearerSetup_r4_IEs,rab_InformationSetupList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS: RAB Info present in RBS Message");

    rab_list_ptr = &rbsetup_ptr->rab_InformationSetupList;

    /* Check if the RAB id is GSM_MAP type. We don't support any
    other type */
    if ( !(rab_list_ptr->elem[rab_index].rab_Info.rab_Identity.t ==
          T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB ID t%d is not GSM_MAP!",rab_list_ptr->elem[rab_index].rab_Info.rab_Identity.t);
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;
    }

    /* check for the unsupported PDCP info */

    do  /* going through the RAB info setup list */
    {
      rb_list_ptr = &rab_list_ptr->elem[rab_index].rb_InformationSetupList;
      rb_index= 0;
      do  /* going through the RB info setup list */
      {
        tdsrrc_RB_InformationSetup_r4 * rb_setup_info_ptr;
        rb_setup_info_ptr = &rb_list_ptr->elem[rb_index];
        if (tdsrrc_find_rb_in_est_rabs(rb_setup_info_ptr->rb_Identity) == TDSRRC_RB_PRESENT) 
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB id %d already exist in est rabs: Invalid config",rb_setup_info_ptr->rb_Identity);
          tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
          return;
        }

        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_setup_info_ptr,
            tdsrrc_RB_InformationSetup_r4,pdcp_Info))
        {
#ifdef FEATURE_UMTS_PDCP
          if (tdsrrc_pdcp_enabled)
          {
           /* If the RAB is for cs domain, send RBS failure with cause
                  *   invalid configuration
                  */
            if (rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info invalid for CS rab");
              tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
              return;
            }
            /* check whether lossless SRNS relocation flag is set
             * and header compression algo is present in the message */
            if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_setup_info_ptr->pdcp_Info,
                    tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
                (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_setup_info_ptr->pdcp_Info.losslessSRNS_RelocSupport,
                rrc_LosslessSRNS_RelocSupport_supported)))
            {
              if (rb_setup_info_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_absent)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP pdu header is absent.  Invalid config");
                tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                return;
              }

            } /* if rrc_PDCP_Info_r4_losslessSRNS_RelocSupport_present */
            if (rb_setup_info_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present)
            {
              if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_setup_info_ptr->pdcp_Info,
                    tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
                  (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_setup_info_ptr->pdcp_Info.losslessSRNS_RelocSupport,
                   rrc_LosslessSRNS_RelocSupport_notSupported)))
              {
                if (!(TDSRRC_MSG_COMMON_BITMASK_IE(rb_setup_info_ptr->pdcp_Info,
                     tdsrrc_PDCP_Info_r4,headerCompressionInfoList)))
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP header compress info list not present");
                  tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                  return;
                }
              }
            }

            if (TDSRRC_MSG_COMMON_BITMASK_IE(rb_setup_info_ptr->pdcp_Info,
                 tdsrrc_PDCP_Info_r4,headerCompressionInfoList))
            {
              uint32 local_idx=0;
             
              while (rb_setup_info_ptr->pdcp_Info.headerCompressionInfoList.n > local_idx)
              {
                 pdcpHeaderCompressionInfo = &rb_setup_info_ptr->pdcp_Info.headerCompressionInfoList.elem[local_idx];
                if (TDSRRC_CHECK_COMMON_MSG_TYPE(pdcpHeaderCompressionInfo->algorithmSpecificInfo,
                    rrc_AlgorithmSpecificInfo_r4_rfc2507_Info))
                {
                  /* if the UE capability "Maximum header compression context space", is exceeded with this configuration send
                   * failure with cause "Invalid Config"
                   */
                  if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER > IPHC_MAX_HC_CONTEXT_SPACE)
                  {
                    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  max header %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER);
                    tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                    return;
                  }
                }
                else if (TDSRRC_CHECK_COMMON_MSG_TYPE(pdcpHeaderCompressionInfo->algorithmSpecificInfo,
                         rrc_AlgorithmSpecificInfo_r4_rfc3095_Info))
                {
                  /*    the chosen MAX_CID shall not be greater than the value "Maximum number of ROHC context sessions" as 
                   *  indicated in the IE "PDCP Capability"
                   */
                  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.
                           u.rfc3095_Info,ul_RFC3095))
                  {
                    if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID);
                      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                      return;
                    }
                  }

                  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,
                                dl_RFC3095))
                  {
                    if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID);
                      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                      return;
                    }
                  }

                }  /* if (pdcpHeaderCompressionInfoList->value.algorithmSpecificInfo.t == rrc_rfc3095_Info)*/

                local_idx++;
              } /*  while (pdcpHeaderCompressionInfoList) */

            } /*  if (rb_list_ptr->value.pdcp_Info.bit_mask & rrc_PDCP_Info_r4_headerCompressionInfoList_present) */
          }
#endif
        } /* if (rb_list_ptr->value.bit_mask & rrc_RB_InformationSetup_r4_pdcp_Info_present) */

        if(new_tm_rb == FALSE)
        {
          dl_tm_flag = FALSE;
   
          if((TDSRRC_CHECK_COMMON_MSG_TYPE(rb_setup_info_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_rlc_Info)) &&
          (((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_setup_info_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,ul_RLC_Mode)) &&
               (rb_setup_info_ptr->rlc_InfoChoice.u.rlc_Info->ul_RLC_Mode.t==
                T_tdsrrc_UL_RLC_Mode_ul_TM_RLC_Mode))
               &&
               ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_setup_info_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,dl_RLC_Mode)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_setup_info_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode,
                rrc_DL_RLC_Mode_dl_TM_RLC_Mode)))))
          {
            new_tm_rb = TRUE;
          }
          else if(TDSRRC_CHECK_COMMON_MSG_TYPE(rb_setup_info_ptr->rlc_InfoChoice,
                    rrc_RLC_InfoChoice_same_as_RB))
          {
            local_lc_id = tdsrrclcm_get_dl_rlc_lc_id(rb_setup_info_ptr->rlc_InfoChoice.u.same_as_RB);
            if(local_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
            {
              result = tdsrrclcm_get_dl_rlc_mode(local_lc_id, &rx_mode);
              if(result == TDSRRCLCM_SUCCESS && rx_mode == UE_MODE_TRANSPARENT)
              {
                dl_tm_flag = TRUE;
              }
            }
  
            local_lc_id = tdsrrclcm_get_ul_rlc_lc_id(rb_setup_info_ptr->rlc_InfoChoice.u.same_as_RB);
            if(local_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
            {
              result = tdsrrclcm_get_ul_rlc_mode(local_lc_id, &rx_mode);
              if(dl_tm_flag == TRUE && result == TDSRRCLCM_SUCCESS && rx_mode == UE_MODE_TRANSPARENT)
              {
                new_tm_rb = TRUE;
              }
            }
          }
        }
        /*point to the next link */
        rb_index++;
      } while (rb_list_ptr->n > rb_index);
      /* Go to next RAB setup infomation  */
      rab_index++;
    } while (rab_list_ptr->n > rab_index);

  }
  else
  {
    /* There is no RAB to be setup in this message.
      No further validation is needed */
    tdsrrc_rbe.rabs.num_rabs_in_msg = 0;
  }
  if(new_tm_rb == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"One or more TM RBs being setup");
  }
  /*check for the SRNS relocation info */
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
      tdsrrc_RadioBearerSetup_r4_IEs,dl_CounterSynchronisationInfo))
#else
    if (tdsrrcrbe_validate_srns_relocation_info(msg_ptr, TDSMSG_REL4))
#endif

  {

    /*check whether the SRNS relocation is within Cell_DCH or not.
  SRNS relocation information can come in reconfiguration messages
    only when the state transition is Cell_DCH->Cell_DCH */

    if (!((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH) &&
          ( tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRNS reloc with invalid states cur_state %d,msg_state %d",tdsrrc_get_state(),tdsrrc_rbe.state_from_msg);
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has
    PDCP info list. If it is true then this configuration is unsupported */

    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rbsetup_ptr->dl_CounterSynchronisationInfo,
        rB_WithPDCP_InfoList)) 
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info in DL counter sync info not supported");
      tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation.
      Set the flag that indicates that this procedure
        involves SRNS relocation */
      tdsrrc_rbe.rbe_for_srns_relocation = TRUE;
      /* store if new U-RNTI is present in the message */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
          tdsrrc_RadioBearerSetup_r4_IEs,new_U_RNTI))
      {
        tdsrrc_rbe.new_urnti_valid = TRUE;
        tdsrrc_translate_urnti (&(rbsetup_ptr->new_U_RNTI),
                             &tdsrrc_rbe.new_urnti);
      }
    }

  }

  /* Copy handle from R5 and modify(R5 same as R3) */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
   tdsrrc_RadioBearerSetup_r4_IEs,rab_InformationSetupList))
  {
    /* GSM_MAP RAB Id. Check if it's valid by calling the
    appropriate NAS entity. First convert it to internal
    format. */
    rab_index =0;
    rab_list_ptr = &rbsetup_ptr->rab_InformationSetupList;

    /* check whether all RABs in the rabinfo list belong to same CN domain id or not
       otherwise this is an invalid configuration */
    while (rab_list_ptr->n > (rab_index+1) )
    {
      if (rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity !=
          rab_list_ptr->elem[rab_index+1].rab_Info.cn_DomainIdentity)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Different CN domains in rab-list %d %d",rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity,rab_list_ptr->elem[rab_index+1].rab_Info.cn_DomainIdentity);
        tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
        return;
      }
      rab_index++;;
    }
  
    /* Restore the Ptr */
    rab_list_ptr = &rbsetup_ptr->rab_InformationSetupList;
  
  
    for (rab_index=0; (rab_list_ptr->n > rab_index) && (rab_index < TDSRRCRB_MAX_RAB_PER_MSG); rab_index++)
    {
      rab_id = tdsrrcrb_translate_gsm_map_rab_id((rab_list_ptr->elem[rab_index].rab_Info.
                                                rab_Identity.u.gsm_MAP_RAB_Identity));
  
      /* Store this info locally so that ESTABLISHED_RABS can
    be updated later, when these RABs are established */
      tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id_is_gsm_map = TRUE;
      tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id = rab_id;
      tdsrrc_rbe.rabs.rabs_in_msg[rab_index].re_est_timer =
      rab_list_ptr->elem[rab_index].rab_Info.re_EstablishmentTimer;
      /* Check if this RAB is CS Domain or PS Domain */
      if ( rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
      {
        /* Store the domain identity. This will be later added to
          ESTABLISHED_RABS */
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain = RRC_CS_DOMAIN_CN_ID;
  
        /* For CS RABs, Call MN layer to verify this RAB */
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
        rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
        rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"For RAB-Id %d, RAB type from NAS = %d, RAB type from EST_RABs = %d",rab_id, rab_type,
            tdsrrc_get_rab_type_for_given_rab_id(tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id));
  
        switch ( rab_type )
        {
          case RRCMN_CS_DATA_CALL:
  
            TDSRRC_MSG1(MSG_LEGACY_LOW,"Received a CS Data RAB Id %d",rab_id);
            break;
  
          case RRCMN_CS_VOICE_CALL:

            {
              TDSRRC_MSG1(MSG_LEGACY_LOW,"Received a CS Voice RAB Id %d",rab_id);
              if (tdsrrc_find_if_voice_rab_exists_in_established_rabs( tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id )
                == TRUE)
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Voice RAB Id - %d already exists",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id );
                /*The condition of "if" covers the case when third RB is being added for AMR call*/
                if(rab_list_ptr->elem[rab_index].rb_InformationSetupList.n == 1)
                {
                  /* Most likely network is trying to add third RB to switch from lower AMR rate to higher */
                  voice_rab_already_exists = TRUE;
                }
                else
                {
                  tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid configuration .. more than one RAB subflow is being added");
                  return;
                }
              }
              else
              {
                /* Whenever a new voice RAB is to be set up, at least two RBs need to be associated with that RAB.
                Therefore, the condition of "if" checks for presence of more than 1 RB.*/
                if (rab_list_ptr->elem[rab_index].rb_InformationSetupList.n == 1)
                {
#ifdef FEATURE_SCUDIF_ENHANCEMENT
                  #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/

                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Not enough RBs mapped to voice RAB!");
                  tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
                }
              }
            }
  
            break;
  
          default:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS RAB validation failed! RABid %d",rab_id);
            break;
  
        } /* end switch */
  
        if ( (tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_INVALID_CONFIG) ||
           (tdsrrc_rbe.status.failure_status == TDSRRCRB_MSG_UNSUPPORTED_CONFIG))
        {
          /* No need to continue */
          return;
        }

#ifdef FEATURE_SCUDIF_ENHANCEMENT
        #error code not present
#else
        if(rab_type == RRCMN_CS_VOICE_CALL )
#endif/*FEATURE_SCUDIF_ENHANCEMENT*/
        {
          /* If the NAS Sync Indicator IE is present in the OTA msg then , extract it and update the RBE database */
          if(rab_list_ptr->elem[rab_index].rab_Info.m.nas_Synchronisation_IndicatorPresent)
          {
            nas_sync_ind =  tdsrrc_translate_nas_sync_indicator(&(rab_list_ptr->elem[rab_index].rab_Info.nas_Synchronisation_Indicator));
            if( nas_sync_ind == TDSCDMA_IVOCODER_ID_NONE )
            {
              tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported  configuration .. codec not supported %d",tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator );
              return;
            }
          }
          tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator = nas_sync_ind;    
        }

      }
      /* Else check for PS Domain */
      else if ( rab_list_ptr->elem[rab_index].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain )
      {
        /* Store the domain identity. This will be later added to
          ESTABLISHED_RABS */
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain = RRC_PS_DOMAIN_CN_ID;
  
        /* For PS Domain, if it's a loopback, then no need for query,
        otherwise, call RABM module to verify the RAB */
        if((tdsrrclbt_is_lb_on() == TRUE) ||
#ifdef FEATURE_DUAL_WCDMA
           (rabm_rabid_validate_per_sub(tdsrrc_get_as_id(), RAB_ESTABLISH_QUERY, (uint8)rab_id) == TRUE)
#else
           (rabm_rabid_validate(RAB_ESTABLISH_QUERY, (uint8)rab_id) == TRUE)
#endif
        )
        {
          TDSRRC_MSG1(MSG_LEGACY_LOW,"PS RAB Id %d to be setup",rab_id);
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"PS RAB validation failed! RABid %d",rab_id);
          tdsrrc_rbe.status.failure_status=TDSRRCRB_MSG_INVALID_CONFIG;
          return;
        }
      }
  
      if(voice_rab_already_exists == TRUE)
      {
        tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_VOICE_RAB;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Updated RAB type as voice");
      }
      else
      {
        /* ASN1 structure 'tdsrrc_RAB_InformationSetup' is validated at this point,
            hence update rab_type in structure 'tdsrrc_rbe.rabs.rabs_in_msg[rab_index]'. */
         tdsrrc_update_rab_type_in_rab_info_r4(
           &(msg_ptr->u.later_than_r3.criticalExtensions.u.r4.
               radioBearerSetup_r4.rab_InformationSetupList.elem[rab_index]),
           &(tdsrrc_rbe.rabs.rabs_in_msg[rab_index]));
      }
  
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
               tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rab_id,
               tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain,
               tdsrrc_rbe.rabs.rabs_in_msg[rab_index].re_est_timer);
      /* set the ordered codec mode which is sent in the ordered config to MAC.
        * If the RBE msg is not for Voice RAB also then also the current codec value
        * is passed to MAC 
        */
      tdsrrc_set_ordered_codec_mode(nas_sync_ind);
      tdsrrc_rbe.rabs.rabs_in_msg[rab_index].nas_Synchronisation_Indicator = nas_sync_ind;
  
      /* Go to next RAB info  */
      
    }
    if (rab_list_ptr->n > rab_index)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Too many RABs in an RBSetupMessage");
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
      return;    
    }
  }
  /* Store the number of the RAB info in the RBSetup message. */
  tdsrrc_rbe.rabs.num_rabs_in_msg = rab_index;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"%d RABs in an RBSetupMessage",rab_index);

  /* Copy handle from R5(mac_d_HFN_initial_value not present in R3) */
  /* check if we are going to dch from a non-dch state and dl-dpch-common-info is present
     if it is and if TM ciphering bearers are active, then we need to do special processing
     for step-1 and step-2 ciphering restart while going to dch
   */
  tdsrrc_rbe.trans_to_dch_ciph_config_needed = FALSE;
  if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH) && 
      (tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH)
     )
  {
    /* check to see if dl-dpch-common info is present while going to dch */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
                tdsrrc_RadioBearerSetup_r4_IEs,dl_CommonInformation)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CommonInformation, 
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
             (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CommonInformation.dl_DPCH_InfoCommon,
             tdsrrc_DL_DPCH_InfoCommon_r4,mac_d_HFN_initial_value)),
             &rbsetup_ptr->dl_CommonInformation.dl_DPCH_InfoCommon.mac_d_HFN_initial_value,
             &mac_d_hfn_l);
        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }
        if (tdsrrcsmc_process_cipher_config_for_hho(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing Ciphering Config for HHO failed");
        }
        tdsrrc_rbe.trans_to_dch_ciph_config_needed = TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"To-DCH-Step-1 not needed");
      }
    }
  }
  else if((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)&&
      (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH))
  {
    tdsrrc_rbe.first_tm_rb_setup = tdsrrcrbe_is_first_tm_rb_setup(new_tm_rb);
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"First TM RB being set status : %d",tdsrrc_rbe.first_tm_rb_setup);
    if(tdsrrc_rbe.first_tm_rb_setup == TRUE)
    {
      /* check to see if dl-dpch-common info is present while going to dch */
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
         tdsrrc_RadioBearerSetup_r4_IEs,dl_CommonInformation)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CommonInformation,
           tdsrrc_DL_CommonInformation_r4,dl_DPCH_InfoCommon)))
      {
        /* check if ciphering is active in mac for cs domain, if it is then,
          start 2 tiered HHO procedure STEP_1 and STEP_2 for rl-re-establishment
        */
        uint32 mac_d_hfn_l=TDSRRCSMC_INVALID_HFN_VALUE;
        uecomdef_status_e_type loc_ret_val;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"First time TM RB Setup HHO Step-1 config activated");
        loc_ret_val = tdsrrcrb_get_mac_d_hfn(
             (TDSRRC_MSG_COMMON_BITMASK_IE(rbsetup_ptr->dl_CommonInformation.dl_DPCH_InfoCommon,
             tdsrrc_DL_DPCH_InfoCommon_r4,mac_d_HFN_initial_value)),
             &rbsetup_ptr->dl_CommonInformation.dl_DPCH_InfoCommon.mac_d_HFN_initial_value,
             &mac_d_hfn_l);
        if(loc_ret_val == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"get_mac_d_hfn failed");
        }
        if (tdsrrcsmc_process_cipher_config_for_first_tm_setup(mac_d_hfn_l) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing Ciphering Config for First TM RB Setup failed");
          /*This step is reached only when the first step returns a failure,. i.e, if the ciphering is not active for CS domain.
              Hence setting tdsrrc_rbe.first_tm_rb_setup to FALSE so that step 2 is not done*/
          tdsrrc_rbe.first_tm_rb_setup = FALSE;
        }
      }
    }
  }
  
  /*  Copy UTRAN DRX coefficient handle from R5(R5 same as R3) */
  /* If UTRAN DRX coefficient is not present then it is invalid configuration */
  if ((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_rbe.state_from_msg  == TDSRRC_STATE_URA_PCH))
  {

    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
          tdsrrc_RadioBearerSetup_r4_IEs,utran_DRX_CycleLengthCoeff)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Coef not preset,State:%d",tdsrrc_rbe.state_from_msg);
      tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_INVALID_CONFIG;
      return;
    }
  }
  if (tdsrrc_rbe.state_from_msg == TDSRRC_STATE_URA_PCH)
  {
    /*should we do it after getting l2ack for response??*/
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
          tdsrrc_RadioBearerSetup_r4_IEs,ura_Identity)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"URA ID not present");
      tdsrrccu_clear_ura_id ();
    }
    else
    {
      /*URA ID present in the message set ura_identity variable with this value */
      if (rbsetup_ptr->ura_Identity.numbits == 16)
      {
        tdsrrccu_update_ura_id (&(rbsetup_ptr->ura_Identity));
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid URA ID length%d",rbsetup_ptr->ura_Identity.numbits);
        tdsrrc_rbe.status.failure_status = TDSRRCRB_MSG_PROTOCOL_ERROR;
        tdsrrc_rbe.status.prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
        return;
      }
    }
  }


  return;

}/* end function tdsrrcrbe_validate_rbsetup_r4 */

/*===========================================================================

FUNCTION   RRCRBE_SEND_CHANNEL_CONFIG_REQ_R4

DESCRIPTION

  This function sends the TDSRRC_CHANNEL_CONFIG_REQ command to LLC
  to setup the necessary RBs associated with a RAB. If signalling
  Radio bearers are affected and included in the Radio Bearer
  Setup Message, they will also be sent in the same command.

  Currently the messege is not expected to have Signalling
  Radio Bearers for setup. If these are added in the future,
  they will also need to be sent in the same command.

DEPENDENCIES

  TDSORDERED_CONFIG must be set before calling this function.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbe_send_channel_config_req_r4
(
tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
)
{
  tdsrrc_cmd_type *out_cmd_ptr;  /* To hold the Command                       */
  uint32 rb_count=0;          /* To count the number of RBs in the command */
  uint8  rbe_rb_cnt = 0;

  uint32 rab_index=0;         /* Used when more than one RAB is received in a RBS message */
  uint32 srb_cnt=0;
  tdsrrc_RB_Identity rb_id=0;

  tdsrrc_RB_InformationSetupList_r4 *rb_ptr = NULL; /* Temporary pointer to RB info      */
  tdsrrc_RB_InformationAffectedList*  rec_rb_ptr = NULL;  /* Pointer to RBs to be Reconfigured */

  tdsrrc_RAB_InformationSetupList_r4 *rab_info_ptr = NULL;
  tdsrrc_SRB_InformationSetupList   *      srb_setup_ptr = NULL;
  tdsrrcrb_directed_cell_info_type        directed_cell_info; /* directed cell info struct */

  tdsrrc_RadioBearerSetup_r4_IEs* rbsetup_ptr = NULL;

  /* Fill in the command id  */
  tdsrrc_cmd.cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;

  /* Set RBSetup Ptr */
  rbsetup_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.r4.radioBearerSetup_r4;


  /* First fill up the Radio Bearers that need to be setup.
  For this we need to know how many RBs need to be setup.
  If RABs need to be setup, check how many RBs are mapped
  to RABs. */

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r4_IEs,srb_InformationSetupList))
  {
    /* Get the SRB setup list */
    srb_setup_ptr = &rbsetup_ptr->srb_InformationSetupList;

    while ((srb_setup_ptr->n > srb_cnt) && (srb_cnt < UE_MAX_SRB_SETUP))
    {
      /* Check to see if the RB ID is there ... */
      if (srb_setup_ptr->elem[srb_cnt].m.rb_IdentityPresent)
      {
        rb_id = srb_setup_ptr->elem[srb_cnt].rb_Identity;
      }

      /*
      * ... if not, then start with the first SRB and get the next SRB ID
      * each time one is not included in the IE.
      */
      else
      {
        /* Get the next SRB, note this increments to 1 the first time */
        rb_id++;
      }

      /* Fill up the RB id and indicate that it needs to be established */
      tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rb_id;
      if(tdsrrclcm_get_dl_rlc_lc_id(rb_id) == TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
      {
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSESTABLISH_RB;
      }
      else
      {
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = 
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
      }

      rb_count++;
      srb_cnt++;


    } /* end SRB setup */

  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r4_IEs,rab_InformationSetupList))  
  {
    rab_info_ptr = &rbsetup_ptr->rab_InformationSetupList;
    for (rab_index=0; rab_index < tdsrrc_rbe.rabs.num_rabs_in_msg && (rab_info_ptr->n > rab_index); rab_index++)
    {
      if ( tdsrrc_rbe.rabs.chan_config_needed[rab_index] == TRUE )
      {
        rb_ptr = &rab_info_ptr->elem[rab_index].rb_InformationSetupList;
  
        if ( rb_ptr->n == 0 )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No RBs present in msg!");
        } else
        {
          rbe_rb_cnt = 0;
          while ( rb_ptr->n > rbe_rb_cnt )
          {
            /* Fill up the RB id and indicate that it needs to be established */
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rb_ptr->elem[rbe_rb_cnt].rb_Identity;
            tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSESTABLISH_RB;
  
            /* Update the local structure so that we know which RB id's we've
            requested to be setup */
            tdsrrc_rbe.rabs.rabs_in_msg[rab_index].rb_for_rab[rbe_rb_cnt].rb_id = rb_ptr->elem[rbe_rb_cnt].rb_Identity;
  
  
            /* Increment count of RBs */
            rb_count++;
            rbe_rb_cnt ++;
  
            /* Do a safety check on the count. We can't do more than 32
            configs in a command */
            if ( (rb_count >= TDS_MAX_RB) || (rbe_rb_cnt >= MAX_RB_PER_RAB))
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"MAX RB count exceeded!");
              break;
            }
  
            /* Go to the next RB */
          }/* end while */
          /* Save the number of RBs for this RAB */
          tdsrrc_rbe.rabs.rabs_in_msg[rab_index].num_rbs_for_rab = rbe_rb_cnt;
        }
      }/* end if rabs_setup_needed */
      
    }
  }

  /* Now check if the message contains an RB information to be
  affected list. If so, we need to reconfigure some RBs */
  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,tdsrrc_RadioBearerSetup_r4_IEs,rb_InformationAffectedList))
  {
    uint32 local_idx =0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB info affected list present");

    rec_rb_ptr = &rbsetup_ptr->rb_InformationAffectedList;

    if ( rec_rb_ptr->n == 0 )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No affected RBs in msg!");
    }
    else
    {
      while ( rec_rb_ptr->n > local_idx)
      {

        /* Fill up the RB id and indicate that it needs to be reconfigured */
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_id = rec_rb_ptr->elem[local_idx].rb_Identity;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].rb_config = TDSRECONFIG_RB_OTA;
        tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].ul_cfg_mask = tdsrrc_cmd.cmd.chan_config_req.rb[rb_count].dl_cfg_mask = TDSMODIFY_RB_CFG;
        /* Increment count of RBs. This is done here since we
        need to use the next index that's not used already. */
        rb_count++;

        /* Go to the next affected RB */
       local_idx++;

        /* Do a safety check on the count. We can't do more than 32
        configs in a command */
        if ( rb_count >= TDS_MAX_RB )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MAX RB count exceeded!");
          break;
        }
      } /* end while */
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"No affected RB list in msg");
  }/* end if affected list present */

  /* Number of RBs to config. Note rb_count is the total
  of established and reconfigured RBs */
  tdsrrc_cmd.cmd.chan_config_req.num_rb_to_config = (int) rb_count;

  if(rb_count < TDS_MAX_RB)
  {
    tdsrrc_fill_rb_id_chan_req_from_standalone_db(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
  }

  if(tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_DCH)
  {
    /* Fill those RBs in chan_config_req for which RLC PDU size has changed and 
    that are already not present in chan_config_req*/
    if(rb_count < TDS_MAX_RB)
    {
    tdsrrc_fill_rb_id_chan_req_from_rb_list_mapped_to_dch(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
    }
  }

  /*If state transition is Cell_DCH->Cell_FACH or vice-verca, add all 
  the existing AM PS RBs in chan_config_req*/   
  if(((tdsrrc_rbe.state_from_msg == TDSRRC_STATE_CELL_FACH) && (tdstransition_config.toc_usage == 
      TDSTOC_FOR_DCH_AND_FACH)) || ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && (tdsrrc_rbe.state_from_msg 
      == TDSRRC_STATE_CELL_DCH)))
  {
    tdsrrc_fill_rb_id_chan_req_from_est_rabs(&rb_count, &tdsrrc_cmd.cmd.chan_config_req);
  }

  /* Check if state change is required */
  if ( (tdsrrc_get_state()) == (tdsrrc_rbe.state_from_msg) )
  {
    tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required = FALSE;
  }
  else
  {
    /* The message indicates that state change is required.
    Indicate to LLC what the next state should be. */
    tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required = TRUE;
    tdsrrc_cmd.cmd.chan_config_req.next_state = tdsrrc_rbe.state_from_msg;
  }

  /* Indicate the id of the procedure sending the command */
  tdsrrc_cmd.cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_RBE;

  /* Indicate that we need a confirmation for this command */
  tdsrrc_cmd.cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

  /* We are not required to fill any other data for this command.
   Now, send the command */
  if (((tdsrrc_get_state()) == TDSRRC_STATE_CELL_DCH) &&
      (tdsrrc_cmd.cmd.chan_config_req.next_state== TDSRRC_STATE_CELL_FACH) &&
      (tdsrrc_cmd.cmd.chan_config_req.rrc_state_change_required))
  {

    /*Initiate the cell selection*/
    tdsrrcrb_send_initiate_cell_selection_req(TDSRRC_PROCEDURE_RBE,
                                           TDSRRC_TRANSITION_FROM_DCH,
                                           tdsrrc_rbe.state_from_msg);

    tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;

    /* Start T_305 [periodic cell update] timer */
    tdsrrccu_start_t_305_timer();

  } 
  else
  {
    /*check whether any frequency info is present in the message
    and transition is CELL_FACH->CELL_FACH. In this case
    initiate a cell update after channel config is successfully completed */
    if (((tdsrrc_get_state())==TDSRRC_STATE_CELL_FACH)&&
        (tdsrrc_rbe.state_from_msg==TDSRRC_STATE_CELL_FACH))
    {
      if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
           tdsrrc_RadioBearerSetup_r4_IEs,frequencyInfo)) &&
          (rbsetup_ptr->frequencyInfo.
           modeSpecificInfo.t == T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd))
      {
        directed_cell_info.freq_info_present = TRUE;
        directed_cell_info.uarfcn_dl = rbsetup_ptr->frequencyInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
      }
      else
      {
        directed_cell_info.freq_info_present = FALSE;
      }
      if (tdsrrcrbe_is_pccpch_info_present(msg_ptr, TDSMSG_REL4))
      {
        directed_cell_info.cpid_present = TRUE;
        directed_cell_info.cell_param_id = rbsetup_ptr->dl_InformationPerRL_List.elem[0].
              modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
      }
      else
      {
        directed_cell_info.cpid_present = FALSE;
      }
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info) == FALSE)
      {
        /*If Cell Reselection is going on and OTA is trying to perform Freq Redirection 
              (FACH->FACH ), treat it as un-supported configuration and send failure.*/ 
        if(tdsrrccsp_check_initiate_cell_selection_handling() == FALSE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq_Redir:Skipping Freq Redir bcoz Cell Resel in progress");
       
          tdsrrcrbe_send_initiate_cu_flag = TRUE;
          tdsrrcrbe_delete_crnti          = TRUE;
        }
        else
        {
          tdsrrc_int_c_rnti_type      c_rnti;       /* Local C_RNTI value */
          if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbsetup_ptr,
                  tdsrrc_RadioBearerSetup_r4_IEs,new_C_RNTI))
          {
            tdsrrc_rbe.new_crnti_valid = TRUE;
            tdsrrc_translate_crnti(&rbsetup_ptr->new_C_RNTI,
                                &c_rnti);
            tdsrrc_rbe.new_crnti = (uint16)c_rnti;
          }
          else
          {
            tdsrrc_rbe.new_crnti_valid = FALSE;
          }
          tdsrrcllcoc_update_toc_with_oc_from_fach();
          
          /*Initiate the cell selection*/
          tdsrrcrb_send_initiate_cell_selection_req(TDSRRC_PROCEDURE_RBE,
                                                  TDSRRC_TRANSITION_FROM_FACH,
                                                  tdsrrc_rbe.state_from_msg);
       
          tdsrrc_rbe.curr_substate = TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF;
           
          return;
        }
      }

#else/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
      /*call the function that evaluates the directed cell info and set the flags accordingly */
      if (tdsrrcrb_validate_directed_cell_info (directed_cell_info))
      {
        /*directed cell info matched. Cell update is not required */
        tdsrrcrbe_send_initiate_cu_flag = FALSE;
      }
      else
      {
        /*directed cell info do not match. Cell update is required */
        tdsrrcrbe_send_initiate_cu_flag = TRUE;
        tdsrrcrbe_delete_crnti          = TRUE;
      }
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

    } /* if CELL_FACH->CELL_FACH reconfiguration */
    /* Get a Command buffer and copy the local command to the buffer
    * so as to enqueue it on the internal queue.
    */
    if ((out_cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL)
    {
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
      memscpy(out_cmd_ptr,sizeof(tdsrrc_cmd_int_type),&tdsrrc_cmd,sizeof(tdsrrc_cmd_int_type));
#else
      out_cmd_ptr->cmd = tdsrrc_cmd.cmd;
      out_cmd_ptr->cmd_hdr = tdsrrc_cmd.cmd_hdr;
#endif

      tdsrrc_put_int_cmd( out_cmd_ptr );
    }
    else
    {
      ERR_FATAL("Could not get internal cmd buf",0,0,0);
    }
  }

}/* end function tdsrrcrbe_send_channel_config_req_r5 */
/*====================================================================
FUNCTION: rrc_l1_query_is_amr_call_enabled

DESCRIPTION:
  This function is called during the CPHY_SETUP processing and the return
  value signifies whether an AMR voice call is actually being setup or not.
  The initial call_setup_mask was returning VOICE CALL SETUP UP if the
  domain is CS. This function enhances more checks and returns TRUE only
  if it is going to be a CS voice call. 

DEPENDENCIES:
  NONE

RETURN VALUE:
  Return value is a boolean and gives information whether AMR call is being
  established.

SIDE EFFECTS:
  None.
====================================================================*/

boolean tdsrrc_l1_query_is_amr_call_enabled(void)
{
  uint8 rab_index;


  /*Assuming DCH to FACH transition doesn't setup Voice RAB*/
  if(tdsordered_config.set_by_proc == TDSRRC_PROCEDURE_RBE)
  {
    for(rab_index =0; rab_index < TDSRRCRB_MAX_RAB_PER_MSG; rab_index++)
    {
        if( (tdsrrc_rbe.rabs.rabs_in_msg[rab_index].cn_domain == RRC_CS_DOMAIN_CN_ID)&& 
          (tdsrrc_rbe.rabs.rabs_in_msg[rab_index].num_rbs_for_rab > 1) )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Voice RAB found in tdsrrc_rbe");
        return TRUE;
      }
    }
  }

  /*Loop through all the EST_RABs to find out any RAB is mapped to AMR call.*/
  for(rab_index =0; rab_index <MAX_RAB_TO_SETUP; rab_index++)
  {
    if( (tdsrrc_est_rabs.rabs[rab_index].cn_domain == RRC_CS_DOMAIN_CN_ID) && 
        (tdsrrc_est_rabs.rabs[rab_index].num_rbs_for_rab > 1) )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Voice RAB found in est rabs");
      return TRUE;
    }
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Voice RAB not found");
  return FALSE;
}

/*===========================================================================

FUNCTION   tdsrrcrbe_send_rabm_est_ind_to_upper_layers

DESCRIPTION
  This function sends RABMAS_RAB_ESTABLISH_IND primitive
  to the RABM layer of NAS.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsrrcrbe_send_rabm_est_ind_to_upper_layers
(
  void
)
{
  rabm_cmd_type *rabm_ptr; /* To send the RABMAS_RAB_ESTABLISH_IND command */
  uint32 rab_index=0;    /* Used if there is more than 1 RAB in the message */

   /* Check whether this RAB exists in established_rab db */
  for (rab_index=0; rab_index<MAX_RAB_TO_SETUP; rab_index++)
  {
    if ((tdsrrc_est_rabs.rabs[rab_index].cn_domain == RRC_PS_DOMAIN_CN_ID) &&
         (tdsrrc_est_rabs.rabs[rab_index].num_rbs_for_rab > 0) &&
         (tdsrrc_est_rabs.rabs[rab_index].rab_id !=  TDSRRC_INVALID_RAB_ID)
#ifdef FEATURE_WRLF_SYSTEM_SEL         
         &&(!((tdsrrccu_get_rabs_need_to_be_released_T314() && tdsrrc_est_rabs.rabs[rab_index].re_est_timer == tdsrrc_Re_EstablishmentTimer_useT314)||
         (tdsrrccu_get_rabs_need_to_be_released_T315() && tdsrrc_est_rabs.rabs[rab_index].re_est_timer == tdsrrc_Re_EstablishmentTimer_useT315)))
#endif 
        )
    {
      if ((rabm_ptr = rabm_get_cmd_buf()) == NULL)
      {
        ERR_FATAL("Can't send RABM cmd - out of memory!",0,0,0);
      }
      rabm_ptr->header.cmd_id = RABMAS_RAB_ESTABLISH_IND;
      rabm_ptr->header.message_set = MS_RABM_RRC;
      rabm_ptr->cmd.rrc_establish_ind.rabid = (rabid_T)(tdsrrc_est_rabs.rabs[rab_index].rab_id);
#ifdef FEATURE_DUAL_SIM
        rabm_ptr->cmd.rrc_establish_ind.as_id = tdsrrc_get_as_id();
#endif
      /* Put the command on RABM queue */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending RAB_ESTABLISH_IND to RABM RAB ID %d",tdsrrc_est_rabs.rabs[rab_index].rab_id);
      if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
      {
        tdscfa_rabm_put_cmd(rabm_ptr);
      }
      rabm_put_cmd(rabm_ptr);
    }
  }
}

