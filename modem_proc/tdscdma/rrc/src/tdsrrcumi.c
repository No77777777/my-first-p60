/*===========================================================================
                  R R C  UTRAN MOBILITY INFO

DESCRIPTION
  This header file implements functions for UTRAN Mobility Info
  procedure.  For the current release ciphering and integrity
  protection is not used and is ignored.  Transaction id of
  the UTRAN Mobility Info is copied and sent in Confirm msg.

  The foll. fields of the UTRAN MOBILITY INFO is ignored.
  * cipheringModeInfo
  * dl_CounterSynchronisationInfo
  * integrityProtectionModeInfo
    (in UMI message will be supported when SRNS relocation is
     supported.  integrityProtectionModeInfo can be received in 
     UMI only when SRNS relocation is done. The same applies to
     cipheringModeInfo in the above bullet)

  The foll. fields of the UTRAN Mobility Confirm message is 
  sent uninitialized
  * ul_IntegProtActivationInfo
    (in UMI message will be supported when SRNS relocation is
     supported.  integrityProtectionModeInfo can be received in 
     UMI only when SRNS relocation is done)
  * ul_CounterSynchronisationInfo
  * ul_CounterSynchronisationInfo



EXTERNALIZED FUNCTIONS

  tdsrrcumi_procedure_event_handler
    This function is the event handler for all events that are sent to the
    UTRAN Mobility Info procedure.

  tdsrrcumi_init_procedure
    This function initializes required parameters for UTRAN Mobility Info
    procedure.

  tdsrrcumi_process_utran_mobility_info_command  
   This function processes the UTRAN MOBILITY INFO received on DL DCCH
   and extracts the Transaction ID.  Sends the SYNC_IND cmd to MM and
   also calls the function to send UTRAN Mobility Info

  tdsrrcumi_send_utran_mobility_info_confirm
   This function sends the UTRAN Mobility Info command

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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcumi.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/20/11   yuh   Merged LTE code
03/10/11   ttl     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/10/10   bj       Deleted IE "RNC support for change of UE capability".(3GPP: 8.6.2.3)

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdsrrccmd_v.h"              
#include "tdsrrcccm.h"              
#include "tdsrrcumi.h"
#include "msg.h"
#include "err.h"
#include "tdsrrcasn1.h"
#include "tdsrrclcm.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrcsend.h"
#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "tdsrrcscmgr.h"
#include "tdsrrclogging.h"
#include "tdsrrcsmc.h"
#include "tdsrrcrbcommon.h"
#include "tdsrrcmisc.h"
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
#include "tdsrrcsibproc.h"
#endif

#include "tdsrrcdormancy.h"

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
#include "tdsrrcmeas.h"
#endif
#include "tdsrrcasn1util.h"
/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* Enumerated type of status of validating UMI message                   */
typedef enum
{
  TDSRRCUMI_MSG_UNSUPPORTED_CONFIG, /* Message has un-supported configuration  */
  TDSRRCUMI_MSG_PROTOCOL_ERROR,     /* Message has protocol error              */
  TDSRRCUMI_MSG_INVALID_CONFIG      /* Message has invalid configuration       */
}tdsrrcumi_msg_status_e_type;


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
/* This variable holds substate for UE Capability Information Procedure
 */

/* Structure to store all internal data related to UMI procedure           */

typedef struct {
  tdsrrcumi_msg_status_e_type failure_status; /* Failure cause                 */
  tdsrrc_ProtocolErrorCause  prot_err_cause; /* Protocol Error cause          */
}tdsrrcumi_msg_failure_cause_type;

typedef struct {
  tdsrrcumi_substate_e_type curr_substate; /* Store the current substate     */
  tdsrrcumi_msg_failure_cause_type status;   /* Status of message validation   */

  boolean    new_crnti_valid; /*Indicates if a new crnti is recvd */           
  tdsrrc_int_c_rnti_type     new_crnti;
  tdsrrc_int_u_rnti_type          new_urnti;
  boolean                      new_urnti_valid;          /* Indicates if a valid U-RNTI was recd */ 
  boolean                      umi_for_srns_relocation;  /* Indicates SRNS reloc involved with 
                                                          this msg                             */
  uint16  new_hrnti;
  boolean    new_hrnti_valid;          /* Indicates if a valid H-RNTI was recd */ 
}tdsrrcumi_int_data_type;

tdsrrcumi_int_data_type tdsumi;

/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrcumi_substate_e_type                     tdsrrcumi_substate;

static tdsrrcumi_ue_timer_const_rcvd_status_e_type   tdsrrcumi_ue_timer_const_rcvd_status;

/*Local variable for confirm msg*/
static tdsrrc_utran_mob_info_cnf_type                tdsrrcumi_utran_mob_inf_cnf;

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                            Function Implementation
** ======================================================================= */
/*Declaration only*/
boolean tdsrrcumi_validate_srns_relocation_info
(
tdsrrc_UTRANMobilityInformation *msg_ptr /* Pointer to the UMI message        */
);

/*===========================================================================

FUNCTION tdsrrcumi_check_ue_timer_constants_update_status

DESCRIPTION
  Update global tdsrrc_ue_conn_timers_and_constants with timers and constants
  from SIB1.  Function provided for SIB procedure to query if we should
  indeed update connected mode timers and constants database.
  
DEPENDENCIES
  None

RETURN VALUE
  TDSRRC_CONN_TIMER_AND_CONSTANTS_UPDATE_REQ
  :  If SIB Procedure needs to update tdsrrc_ue_conn_timers_and_constants
  TDSRRC_CONN_TIMER_AND_CONSTANTS_UPDATE_NOT_REQ
  : If SIB Procedure need not update tdsrrc_ue_conn_timers_and_constants  
  
SIDE EFFECTS
  None
===========================================================================*/
/*lint -e715*/
tdsrrc_conn_timer_constants_update_status_e_type 
tdsrrcumi_check_ue_timer_constants_update_status
(
  boolean new_cell_ind
)
{

  /*if we have already received conn. mode timers and consts in a 
   *directed message, then do not read from SIBS ever
   *in UTRAN Mobility Info msg, then do not update timers and consts
   */
  if(tdsrrcumi_ue_timer_const_rcvd_status==TDSRRCUMI_UE_TIMER_CONST_RCVD)
  {
    return(TDSRRC_CONN_TIMER_AND_CONSTANTS_UPDATE_NOT_REQ);
  }
  else
  {
    return(TDSRRC_CONN_TIMER_AND_CONSTANTS_UPDATE_REQ);
  }

}/* rrcumi_update_sib1_ue_conn_timers_and_constants */
/*lint +e715*/

/*===========================================================================

FUNCTION tdsrrcumi_send_umi_failure

DESCRIPTION
  This function builds and sends a UMI failure message with appropriate cause
  
DEPENDENCIES
  None

RETURN VALUE
  void
  
SIDE EFFECTS
  None
===========================================================================*/
static void  tdsrrcumi_send_umi_failure
(  
tdsrrc_RRC_TransactionIdentifier  tdstransaction_id, /*transaction id of the message */
tdsrrcumi_msg_status_e_type failure_status,  /* Failure Cause - should be a subset
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
    ERR_FATAL("No memory for UMI Failure msg!",0,0,0);
  }

  TDSRRC_RESET_MSG_IE_PRESENT_PTR(ptr); /* Integrity Check info not present */
  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_utranMobilityInformationFailure; /* UMI Failure msg */

  /* mask non critical extensions */
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.utranMobilityInformationFailure);


  ptr->message.u.utranMobilityInformationFailure.rrc_TransactionIdentifier =
   tdstransaction_id;
    /* We no longer need to keep this id. Make rejected_transaction id
    invalid again */

  switch (failure_status)
  {
  case TDSRRCUMI_MSG_UNSUPPORTED_CONFIG:
    ptr->message.u.utranMobilityInformationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
    break;

  case TDSRRCUMI_MSG_PROTOCOL_ERROR:
    ptr->message.u.utranMobilityInformationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_protocolError;
    
     ptr->message.u.utranMobilityInformationFailure.failureCause.u.protocolError=
        rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation);
     
	  if(ptr->message.u.utranMobilityInformationFailure.failureCause.u.protocolError == NULL)
     {
       ERR_FATAL("Memory allocation failure",0,0,0);
     }

    /* When there is a Protocol Error, we also have to indicate
    the protocol error cause */
    ptr->message.u.utranMobilityInformationFailure.failureCause.u.protocolError->
      diagnosticsType.t = T_tdsrrc_ProtocolErrorInformation_diagnosticsType_type1;

    ptr->message.u.utranMobilityInformationFailure.failureCause.u.protocolError->
        diagnosticsType.u.type1 = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation_type1);

    if(ptr->message.u.utranMobilityInformationFailure.failureCause.u.protocolError->
        diagnosticsType.u.type1 == NULL)
    {
      ERR_FATAL("Memory allocation failure",0,0,0);
    }

    /* Copy the given protocol error cause */
    ptr->message.u.utranMobilityInformationFailure.failureCause.u.protocolError->
      diagnosticsType.u.type1->protocolErrorCause = prot_err_cause;
    /*log the protocol error*/
    tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                            (uint8)TDSRRC_PROCEDURE_UMI,
                            (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                            tdsrrc_log_map_protocol_cause(prot_err_cause)
                            );
    break;

  case TDSRRCUMI_MSG_INVALID_CONFIG:
    ptr->message.u.utranMobilityInformationFailure.failureCause.t =
    T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
    break;


  default:
    ptr->message.u.utranMobilityInformationFailure.failureCause.t =
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
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending UMI Failure with cause %d",ptr->message.u.utranMobilityInformationFailure.failureCause.t);
    status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_UMI,  /* Procedure is UMI    */
                                 (void *)ptr,        /* Message pointer     */
                                 ul_lc_id,           /* RLC id              */
                                 TDSRRCSEND_L2ACK_NOT_REQUIRED  /* ACK is required */
                                );
    if ( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Send chain failed for UMI failure msg: error cause %d",status);
      /* In case the send chain did not free the memory for the
      pointer, free the memory */
      if ( ptr != NULL )
      {
        tdsrrcasn1_free_pdu(tdsrrc_UL_DCCH_Message_PDU,ptr);
      }
    }
  }
  else
  {
    /* Unexpected failure. ERR for now. May need to ERR_FATAL here */
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't send UL DCCH msg! Invalid lc id: %d",ul_lc_id );
    /* It's important to free the memory since we didn't send the
    message. */
    tdsrrcasn1_free_pdu(tdsrrc_UL_DCCH_Message_PDU,ptr);
  }
}/* end function tdsrrcumi_send_umi_failure */

/*===========================================================================

FUNCTION  RRCUMI_CLEAR_PROCEDURE

DESCRIPTION

  This function clears all the internal variables in
  the UMI procedure and brings them to their default
  values.
    
DEPENDENCIES

  The Substate of the procedure should be changed
  to initial substate before calling this function.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcumi_clear_procedure( void )
{
  /* Clear the C-RNTI valid flag */
  tdsumi.new_crnti_valid = FALSE;

  tdsumi.umi_for_srns_relocation = FALSE;
  tdsumi.new_urnti_valid = FALSE;

}
/*===========================================================================

FUNCTION tdsrrcumi_validate_utran_mobility_info_command

DESCRIPTION
  This function validates that UTRAN MOBILITY INFO does not contain 
  Critical extensions.
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcumi_status_e_type : It returns TDSRRCUMI_FAILURE is Critical extensions
  are present.  Else TDSRRCUMI_SUCCESS is returned.
  
SIDE EFFECTS
  None
===========================================================================*/
static tdsrrcumi_status_e_type tdsrrcumi_validate_utran_mobility_info_command
(
  tdsrrc_DL_DCCH_Message *msg_ptr              /* Pointer to Downlink DCCH Msg */
)
{
  tdsrrcumi_status_e_type status = TDSRRCUMI_FAILURE; 
  boolean              pdcp_info_list_present = FALSE;
  boolean              dl_counter_sync_info_present = FALSE;

  tdsrrc_UTRANMobilityInformation *umi_msg_ptr;

  /* Get the pointer to the actual UMI message */
  umi_msg_ptr = &(msg_ptr->message.u.utranMobilityInformation);

  /* Check if critical extensions are present */
  if(msg_ptr->message.u.utranMobilityInformation.t == 
              T_tdsrrc_UTRANMobilityInformation_later_than_r3)
  {
    if((msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.criticalExtensions.t ==
        T_tdsrrc_UTRANMobilityInformation_criticalExtensions_2_r5) &&
       (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"R5 Critical extensions was choosen");

      if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.
         criticalExtensions.u.r5.utranMobilityInformation_r5,
         tdsrrc_UTRANMobilityInformation_r5_IEs,dl_CounterSynchronisationInfo))
      {
        dl_counter_sync_info_present = TRUE;

        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.
           criticalExtensions.u.r5.utranMobilityInformation_r5.
           dl_CounterSynchronisationInfo,
           rb_WithPDCP_InfoList))
        {
          pdcp_info_list_present = TRUE;
        }
      }
#ifdef FEATURE_TDSCDMA_REL8
      if((umi_msg_ptr->u.later_than_r3.criticalExtensions.u.r5.m.v690NonCriticalExtensionsPresent) &&
        (umi_msg_ptr->u.later_than_r3.criticalExtensions.u.r5.
        v690NonCriticalExtensions.m.v860NonCriticalExtentionsPresent))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:v860NonCriticalExtentions not supported yet");
      }
#endif /*FEATURE_TDSCDMA_REL8*/
    }
    else
    if((msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.criticalExtensions.t == 
          T_tdsrrc_UTRANMobilityInformation_criticalExtensions_2_criticalExtensions )
          && (msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.criticalExtensions.u.criticalExtensions.t == 
          T_tdsrrc_UTRANMobilityInformation_criticalExtensions_1_r7 ) &&
       (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7))
    {
       tdsrrc_UTRANMobilityInformation_r7_IEs * r7_umi_msg_ptr= 
        &msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.utranMobilityInformation_r7;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"R7 Critical extensions was choosen");
     
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(r7_umi_msg_ptr,
         tdsrrc_UTRANMobilityInformation_r7_IEs,dl_CounterSynchronisationInfo))
      {
        dl_counter_sync_info_present = TRUE;

        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(r7_umi_msg_ptr->dl_CounterSynchronisationInfo,
           rb_WithPDCP_InfoList))
        {
          pdcp_info_list_present = TRUE;
        }
      }
#ifdef FEATURE_TDSCDMA_REL8
#ifndef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
      if(msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.criticalExtensions.u.
        criticalExtensions.u.r7.m.v860NonCriticalExtensionsPresent)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:860ext is not supported yet");
      }
#endif
#endif /*FEATURE_TDSCDMA_REL8*/
    }
    else

    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Critical extensions are not supported");
      tdsumi.status.failure_status = TDSRRCUMI_MSG_PROTOCOL_ERROR;
      tdsumi.status.prot_err_cause = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;
      return(status); 
    } 
  } 
  else
  {
    /*UMI needs to be update after getting confirm with CMCC*/
    if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->message.u.utranMobilityInformation.u.r3.
       utranMobilityInformation_r3,
       tdsrrc_UTRANMobilityInformation_r3_IEs,dl_CounterSynchronisationInfo))
    {
      dl_counter_sync_info_present = TRUE;

      if(TDSRRC_MSG_COMMON_BITMASK_IE(umi_msg_ptr->u.r3.utranMobilityInformation_r3.
         dl_CounterSynchronisationInfo,
         tdsrrc_DL_CounterSynchronisationInfo,rB_WithPDCP_InfoList))
      {
        pdcp_info_list_present = TRUE;
      }
    } 
#ifdef FEATURE_TDSCDMA_REL8
    if((umi_msg_ptr->u.r3.m.v3a0NonCriticalExtensionsPresent) &&
      (umi_msg_ptr->u.r3.v3a0NonCriticalExtensions.m.laterNonCriticalExtensionsPresent) &&
      (umi_msg_ptr->u.r3.v3a0NonCriticalExtensions.laterNonCriticalExtensions.m.v690NonCriticalExtensionsPresent) &&
      (umi_msg_ptr->u.r3.v3a0NonCriticalExtensions.laterNonCriticalExtensions.v690NonCriticalExtensions.m.
       v860NonCriticalExtentionsPresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:v860NonCriticalExtentions not supported");
    }
#endif /*FEATURE_TDSCDMA_REL8*/

  }
#ifndef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  if (dl_counter_sync_info_present == TRUE)
#else
  if (TRUE == tdsrrcumi_validate_srns_relocation_info(umi_msg_ptr))
#endif

  {

    
    /* msg has SRNS relocation info. Check whether dl_counterSynchronizationInfo has 
    PDCP info list. If it is true then this configuration is unsupported */
    if (pdcp_info_list_present == TRUE)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UMI: PDCP info in DL counter sync info not supported,dl_counter_sync_info_present:%d",dl_counter_sync_info_present);
      tdsumi.status.failure_status=TDSRRCUMI_MSG_UNSUPPORTED_CONFIG;
      return status;
    }
    else
    {
      /* The procedure involves Lossy SRNS relocation. 
        Set the flag that indicates that this procedure
      involves SRNS relocation */
      tdsumi.umi_for_srns_relocation = TRUE;

      status = TDSRRCUMI_SUCCESS;
    }


  }
  else
  {
    status = TDSRRCUMI_SUCCESS;
  }
  return(status);

}/* tdsrrcumi_validate_utran_mobility_info_command */

/*===========================================================================

FUNCTION tdsrrcumi_build_utran_mobility_info_confirm_msg

DESCRIPTION
  This function builds the UTRAN Mobility Info command which will be
  sent on UL DCCH

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcumi_build_utran_mobility_info_confirm_msg
(
  tdsrrc_utran_mob_info_cnf_type *utran_mob_inf_cnf_ptr,
  tdsrrc_UL_DCCH_Message *msg_ptr
)
{

  /* Build a UTRAN Mobility Info msg.
   * Identify this message as an UTRAN Mobility Info
   */
  msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_utranMobilityInformationConfirm;

  /* Set bit_mask to zero */
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.utranMobilityInformationConfirm); 

  /* If SRNS relocation was triggered as a result of this message then 
  append the start list */
  if (tdsumi.umi_for_srns_relocation == TRUE)
  {
    /*start list is included in ul_counter_synchronization_info. So enable this IE in the
    bitmask */
    TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.utranMobilityInformationConfirm,
    tdsrrc_UTRANMobilityInformationConfirm,ul_CounterSynchronisationInfo);

    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.utranMobilityInformationConfirm.ul_CounterSynchronisationInfo);


    /* call SMC API to append the start list */
    if (SUCCESS != tdsrrcsmc_append_start_list(&msg_ptr->message.u.utranMobilityInformationConfirm.
                                            ul_CounterSynchronisationInfo.startList,
                                            TDSRRCSMC_EXTRACT_AND_APPEND_START))
    {
      /*should not come here */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"UMI : Could not append Start list");
    }
  }

  /* Copy the transaction ID of UTRAN MOBILITY INFO to  Msg*/
  msg_ptr->message.u.utranMobilityInformationConfirm.rrc_TransactionIdentifier = 
    utran_mob_inf_cnf_ptr->transaction_id; 

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
    if(tdsrrcsib_is_sib11_11bis_12_read_deferred(TDSDEFERRED_ANY_ALLOWED_SIB))
    {
      msg_ptr->message.u.utranMobilityInformationConfirm.m.
	  	laterNonCriticalExtensionsPresent =1;
 
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.utranMobilityInformationConfirm.
	  	laterNonCriticalExtensions);
 
      msg_ptr->message.u.utranMobilityInformationConfirm.
	  	laterNonCriticalExtensions.m.v770NonCriticalExtensionPresent =1;
 
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.utranMobilityInformationConfirm.
	  	laterNonCriticalExtensions.v770NonCriticalExtension);
 
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.utranMobilityInformationConfirm.laterNonCriticalExtensions.v770NonCriticalExtension.
        utranMobilityInformationConfirm_v770ext);
 
      msg_ptr->message.u.utranMobilityInformationConfirm.laterNonCriticalExtensions.v770NonCriticalExtension.
        utranMobilityInformationConfirm_v770ext.m.deferredMeasurementControlReadingPresent =1;
      
      msg_ptr->message.u.utranMobilityInformationConfirm.laterNonCriticalExtensions.v770NonCriticalExtension.
        utranMobilityInformationConfirm_v770ext.deferredMeasurementControlReading = 
        tdsrrc_UTRANMobilityInformationConfirm_v770ext_IEs_deferredMeasurementControlReading_true_;
    }
#endif
  }

  return;

} /* rrcumi_build_utran_mobility_info_confirm */


/*===========================================================================

FUNCTION   tdsrrcumi_validate_umi_ip_mode

DESCRIPTION

  This function checks whether IP mode info is present or not
        
DEPENDENCIES

  None.
 
RETURN VALUE

  Boolean. TRUE : IP mode info is present
           FALSE : IP mode info is not present

SIDE EFFECTS

  None.

===========================================================================*/
static boolean tdsrrcumi_validate_umi_ip_mode
(
tdsrrc_UTRANMobilityInformation *msg_ptr /* Pointer to the UMI message        */
)
{
  boolean  ip_mode_present = FALSE;

  /*check whether dl_counter_sync info is present or not . Do not check for
  the contents of DL_counter_sync_info. First the integrity check has tobe made
  with new integrity protection mode info in the message. Later we can check
  the dl_counter_sync_info contents and send a failure if PDCP info is present */

  if((msg_ptr->t == T_tdsrrc_UTRANMobilityInformation_later_than_r3) &&
     (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5))
  {
    if(msg_ptr->u.later_than_r3.criticalExtensions.t == 
        T_tdsrrc_UTRANMobilityInformation_criticalExtensions_2_r5)
    {
      if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.r5.
         utranMobilityInformation_r5,
         tdsrrc_UTRANMobilityInformation_r5_IEs,integrityProtectionModeInfo))
      {
        ip_mode_present = TRUE;
      }
    }
  }
  else 
  if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.r3.utranMobilityInformation_r3,
      tdsrrc_UTRANMobilityInformation_r3_IEs,integrityProtectionModeInfo))
  {
    ip_mode_present = TRUE;
  }

  return ip_mode_present;
} /* tdsrrcumi_validate_umi_ip_mode */



/*===========================================================================

FUNCTION   RRCUMI_VALIDATE_SRNS_RELOCATION_INFO()

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
boolean tdsrrcumi_validate_srns_relocation_info
(
tdsrrc_UTRANMobilityInformation *msg_ptr /* Pointer to the UMI message        */
)
{
  boolean  srns_relocation_present = FALSE;

  /*check whether dl_counter_sync info is present or not . Do not check for
  the contents of DL_counter_sync_info. First the integrity check has tobe made
  with new integrity protection mode info in the message. Later we can check
  the dl_counter_sync_info contents and send a failure if PDCP info is present */
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Validate if UMI is for srns relocation");
  if (TRUE == tdsrrc_is_fresh_in_reconfiguration_message())
  {
    return TRUE;
  }
#endif

  if((msg_ptr->t == T_tdsrrc_UTRANMobilityInformation_later_than_r3) &&
     (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5))
  {
    if(msg_ptr->u.later_than_r3.criticalExtensions.t == 
        T_tdsrrc_UTRANMobilityInformation_criticalExtensions_2_r5)
    {
      if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.r5.
         utranMobilityInformation_r5,
         tdsrrc_UTRANMobilityInformation_r5_IEs,dl_CounterSynchronisationInfo))
      {
        srns_relocation_present = TRUE;
      }
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
      else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                 msg_ptr->u.later_than_r3.criticalExtensions.u.r5.utranMobilityInformation_r5,
                                               new_U_RNTI))
      { /*check if SRNC-ID changed*/
        if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.u.r5.
           utranMobilityInformation_r5.new_U_RNTI)) == FALSE)
        {
          srns_relocation_present = TRUE;
        }
      }
#endif
    }
    else if(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.t == 
        T_tdsrrc_UTRANMobilityInformation_criticalExtensions_1_r7)
    {
      if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.
        utranMobilityInformation_r7,
         tdsrrc_UTRANMobilityInformation_r7_IEs,dl_CounterSynchronisationInfo))
      {
        srns_relocation_present = TRUE;
      }
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
      else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
           msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.
        utranMobilityInformation_r7, new_U_RNTI))
      {
        /*check if SRNC-ID changed*/
        if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.
        utranMobilityInformation_r7.new_U_RNTI)) == FALSE)
        {
          srns_relocation_present = TRUE;
        }
    }
#endif
  }
  }

  else if(msg_ptr->t == T_tdsrrc_UTRANMobilityInformation_r3)
  {

  if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.r3.utranMobilityInformation_r3, 
      tdsrrc_UTRANMobilityInformation_r3_IEs,dl_CounterSynchronisationInfo))
  {
    srns_relocation_present = TRUE;
  }
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
         msg_ptr->u.r3.utranMobilityInformation_r3, new_U_RNTI))
    {
      /*check if SRNC-ID changed*/
      if (tdsrrc_is_srncid_within_urnti_same(&(msg_ptr->u.r3.utranMobilityInformation_r3.new_U_RNTI)) == FALSE)
      {
        srns_relocation_present = TRUE;
      }
    }
#endif
  }

  return srns_relocation_present;
}


/*===========================================================================

FUNCTION tdsrrcumi_send_utran_mobility_info_confirm

DESCRIPTION
  This function sends the UTRAN Mobility Info command on UL DCCH

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcumi_send_utran_mobility_info_confirm
(
 void
)
{
  tdsrrc_UL_DCCH_Message *msg_ptr=NULL;    /* Pointer to uplink DCCH message */ 

  rlc_lc_id_type ul_lc_id;              /* Uplink logical channel ID */
  
  tdsrrcsend_status_e_type send_status;    /* The status of sending the Initial
                                         * Direct Transfer message */


  /* Build a UTRAN Mobility Info msg. Start by allocating an Uplink 
   * DCCH message. This memory is released by tdsrrcsend_send_ul_sdu().
   */
  /* Determine the logical channel id for the Uplink DCCH
   */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                       DCCH_AM_RADIO_BEARER_ID, 
                                       UE_MODE_ACKNOWLEDGED_DATA);
 /* Ensure that LCM found a valid logical channel id
   */
  if (ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {  
    /* Found a valid RLC Logical Channel Id */
    msg_ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc( 
                sizeof(struct tdsrrc_UL_DCCH_Message));

    /* Call error fatal if the memory allocation failed
     */
    if (msg_ptr == NULL)
    {
      ERR_FATAL("mem_malloc(%d) failure",sizeof(struct tdsrrc_UL_DCCH_Message),0,0);
    }



  /*call the build function to build the confirm message.  build message is
   *seperate in this procedure as Cell Update also needs to build the message
   *and this is exerned.
   */  
    tdsrrcumi_build_utran_mobility_info_confirm_msg(&tdsrrcumi_utran_mob_inf_cnf, msg_ptr);

   /* Send the UTRAN Mobility Info on the Uplink DCCH
    */
    if (tdsumi.umi_for_srns_relocation == TRUE)
    {
      send_status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_UMI,
                                          msg_ptr,
                                          ul_lc_id,
                                        TDSRRCSEND_L2ACK_REQUIRED);

      tdsumi.curr_substate = TDSRRCUMI_WAIT_FOR_L2_ACK;
      tdsrrcumi_substate = TDSRRCUMI_WAIT_FOR_L2_ACK; 
    }
    else
    {
      send_status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_UMI,
                                        msg_ptr,
                                        ul_lc_id,
                                        TDSRRCSEND_L2ACK_NOT_REQUIRED);
      tdsumi.curr_substate = TDSRRCUMI_INITIAL;
      tdsrrcumi_substate = TDSRRCUMI_INITIAL;
    }
    

    if (send_status == TDSRRCSEND_SUCCESS)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"UTRAN Mobility Info Message Sent");
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Error %d sending UTRAN Mobility Info",send_status);
    }
  }/* Found a valid RLC Logical Channel Id */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't find RLC ID for RB 2");
  }
} /* tdsrrcumi_send_utran_mobility_info_confirm */

/*===========================================================================

FUNCTION tdsrrcumi_process_rnti_info

DESCRIPTION
   

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcumi_process_rnti_info ( void )
{
  /*if neither c_rnti or u_rnti needs updating, then do not send the
     *update_req command
     */

  tdsrrcrb_rnti_update_info_type rnti_update_info; /* structure to be used to request RNTI update */

  /* send rnti update request if URNTI was present in the rebsetup message */
  if ((tdsumi.new_urnti_valid == TRUE) || (tdsumi.new_crnti_valid == TRUE))
  {
    /* Send the RNTI update request to rrcllc to update with new-URNTI */
    /* Send a RNTI Update Request */
    if (tdsumi.new_crnti_valid)
    {
      rnti_update_info.crnti_action    = TDSRRC_RNTI_UPDATE;
    }
    else
    {
      rnti_update_info.crnti_action    = TDSRRC_RNTI_NOCHANGE;
    }
    if (tdsumi.new_urnti_valid)
    {
      rnti_update_info.urnti_action    = TDSRRC_RNTI_UPDATE;
    }
    else
    {
      rnti_update_info.urnti_action    = TDSRRC_RNTI_NOCHANGE;
    }
    rnti_update_info.urnti                = tdsumi.new_urnti;
    rnti_update_info.crnti                = (uint16)tdsumi.new_crnti;
    rnti_update_info.procedure            = TDSRRC_PROCEDURE_UMI;
    rnti_update_info.rlc_re_establish_srb = FALSE;
    rnti_update_info.rlc_re_establish_rab = FALSE;
    rnti_update_info.cnf_reqd             = TRUE;

    /*change tdsrrcumi_substate to wait for rnti update confirm*/
    tdsrrcrb_send_rnti_update_req(rnti_update_info);
    tdsrrcumi_substate = TDSRRCUMI_WAIT_FOR_RNTI_UPDATE_CNF;
    tdsumi.curr_substate = TDSRRCUMI_WAIT_FOR_RNTI_UPDATE_CNF;
  }
  else
  {
    if(tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)
    {
      tdsrrc_notify_cell_id_in_dch();
    }
    /*no update of rnti is required.  so just send UMI CNF message*/
    tdsrrcumi_send_utran_mobility_info_confirm( );
  }
}

/*===========================================================================

FUNCTION tdsrrcumi_process_rnti_update_cnf_substate  

DESCRIPTION
   This function processes UMI messages in WAIT_FOR_RNTI_UPDATE_CNF state.
   We could receive an update cnf or a state change ind in this substate.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcumi_process_rnti_update_cnf_substate
(
tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */  
)
{

  tdsrrcumi_msg_status_e_type       failure_status;  /* Failure Cause - should be a subset
                                                   * of the failure causes specified by
                                                   * the spec. 
                                                   */
  tdsrrc_ProtocolErrorCause protocolErrorCause;
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_RNTI_UPDATE_CNF:
      if (cmd_ptr->cmd.rnti_update_cnf.status == SUCCESS) 
      {
        /*if the received message in update cnf, then send confirm message
         * with the saved data in tdsrrcumi_utran_mob_inf_cnf
         */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received RNTI_UPDATE_CNF from LLC");

        if(tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)
        {
          tdsrrc_notify_cell_id_in_dch();
        }

        tdsrrcumi_send_utran_mobility_info_confirm( );

        /* Clear the C-RNTI/U-RNTI valid flag */
        tdsumi.new_crnti_valid = FALSE; 
        tdsumi.new_urnti_valid = FALSE;
      }
      else
      {
        /* RNTI update didn't succeed.  Send UMI failure with cause Invalid_Config */
        failure_status = TDSRRCUMI_MSG_INVALID_CONFIG;
        protocolErrorCause = tdsrrc_ProtocolErrorCause_spare1;
        
        tdsrrcumi_send_umi_failure(tdsrrcumi_utran_mob_inf_cnf.transaction_id, failure_status,protocolErrorCause);
        
        tdsumi.curr_substate = TDSRRCUMI_INITIAL;
        tdsrrcumi_substate = TDSRRCUMI_INITIAL;
        tdsrrcumi_clear_procedure();
      }

      break;
    
    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid message rcvd in UMI substate: %d",tdsrrcumi_substate);
      break;
  }
}
/*===========================================================================

FUNCTION   tdsrrcumi_check_and_get_primary_plmn_id

DESCRIPTION

  This function will check whether primary plmn id is present 
  in r3,r5 path of UMI message. If present then it will return 
  primary plmn id.
        
DEPENDENCIES
  None.

RETURN VALUE
TRUE : IF Primary PLMN I.D is present in UMI message in r3,r5 path
FFALSE: Otherwise

SIDE EFFECTS

  None.

===========================================================================*/
static boolean tdsrrcumi_check_and_get_primary_plmn_id
(
  /* Pointer to the UMI message */
  tdsrrc_UTRANMobilityInformation *msg_ptr,
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
    case TDSMSG_REL99:
      /*Check to see if primary PLMN I.D is present in UMI message ext
       * tdsrrc_UTRANMobilityInformation-v690ext which is present in 
       * v690NonCriticalExtensions in r3 message
       */
      if((msg_ptr->u.r3.m.v3a0NonCriticalExtensionsPresent)&&
          (msg_ptr->u.r3.v3a0NonCriticalExtensions.m.laterNonCriticalExtensionsPresent)&&
          (msg_ptr->u.r3.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          m.v690NonCriticalExtensionsPresent)&&
          (msg_ptr->u.r3.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            v690NonCriticalExtensions.utranMobilityInformation_v690ext.m.
            primary_plmn_IdentityPresent)
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:UMI msg contains primary_plmn_Id in r3 path");
        primary_plmn_Id_present = TRUE;
        *primary_plmn_Identity = msg_ptr->u.r3.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
            v690NonCriticalExtensions.utranMobilityInformation_v690ext.primary_plmn_Identity;
      }
      break;
    case TDSMSG_REL5:
      /*Check to see if primary PLMN I.D is present in UMI message ext
       * tdsrrc_UTRANMobilityInformation-v690ext which is present in 
       * v690NonCriticalExtensions in r5 message
       */
      if((msg_ptr->u.later_than_r3.criticalExtensions.t == T_tdsrrc_UTRANMobilityInformation_criticalExtensions_2_r5)&&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.r5.m.v690NonCriticalExtensionsPresent)&&
          (msg_ptr->u.later_than_r3.criticalExtensions.u.r5.v690NonCriticalExtensions.
            utranMobilityInformation_v690ext.m.primary_plmn_IdentityPresent)
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:UMI msg contains primary_plmn_Id in r5 path");
        primary_plmn_Id_present = TRUE;
        *primary_plmn_Identity = msg_ptr->u.later_than_r3.criticalExtensions.u.r5.
             v690NonCriticalExtensions.utranMobilityInformation_v690ext.primary_plmn_Identity;
      }
      break;
   
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"NWS: Msg version = %d,Not supported",msg_version);
      break;
  }
  
  return primary_plmn_Id_present;
}

/*===========================================================================

FUNCTION tdsrrcumi_process_utran_mobility_info_command  

DESCRIPTION
   This function processes the UTRAN MOBILITY INFO received on DL DCCH
   and extracts the Transaction ID.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcumi_process_utran_mobility_info_command
(
  tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;       /* Pointer to Downlink DCCH */
  
  uint8 message_t;                         /* Local variable for message
                                                   t */
  tdsrrc_RRC_TransactionIdentifier  tdstransaction_id; /*transaction id of the message */
  tdsrrcumi_msg_status_e_type       failure_status;  /* Failure Cause - should be a subset
                                                of the failure causes specified by
                                                  the spec. */
  tdsrrc_ProtocolErrorCause protocolErrorCause;

  tdsrrc_UE_ConnTimersAndConstants* ue_conn_timers_and_constants_ptr;
  tdsrrc_UE_ConnTimersAndConstants_v3a0ext * ext_ptr;
  tdsrrc_msg_ver_enum_type msg_version = TDSMSG_REL99;
#if defined(FEATURE_3GPP_FAST_DORMANCY)
  tdsrrc_UE_ConnTimersAndConstants_v860ext *ue_conn_timers_and_constants_v860ext_ptr = NULL;
#endif

  tdsrrc_rlc_re_est_for_srns_rb_e_type rb_type; /*rb type tobe reestablished */

  /* Get a pointer to the UTRAN Mobility Information message */
  tdsrrc_UTRANMobilityInformation            *msg_ptr=NULL;

  /* IP check status */
  tdsrrcsmc_srns_ip_check_status_e_type      ip_check_status;


   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  boolean primary_plmn_Id_present = FALSE;
   /*Variable to hold primary plmn i.d if present in OTA msg*/
  tdsrrc_PLMN_Identity primary_plmn_Identity;
  tdsrrc_UTRANMobilityInformation_r7_IEs *r7_umi_msg_ptr=NULL;

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
  tdsrrc_DedicatedPriorityInformation *ded_pri_info_ptr = NULL;
#endif


  message_t = tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                                cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

  /* Check if the received message is UTRAN Mobility Info message */
  if( message_t != T_tdsrrc_DL_DCCH_MessageType_utranMobilityInformation)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Message: %d is forwarded",message_t);
    return;
  }

  /* Get the OTA message pointer */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) 
                  cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received UTRAN Mobility Info Message");

  if(dcch_msg_ptr == NULL) /* DCCH ASN1 decoding is un-successful */
  {
#if(!defined(T_WINNT))
    /*log the protocol error*/
    tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                            (uint8)TDSRRC_PROCEDURE_UMI,
                            (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                            (uint8)TDSRRCLOG_ASN1_ENC_ERR
                            );
#endif
    /*can not find the transaction id. So use zero */ 
    tdstransaction_id = 0;
    failure_status = TDSRRCUMI_MSG_PROTOCOL_ERROR;
    protocolErrorCause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;

    tdsrrcumi_send_umi_failure (tdstransaction_id, failure_status,protocolErrorCause);

    tdsumi.curr_substate = TDSRRCUMI_INITIAL;

    tdsrrcumi_clear_procedure();

  }
  else
  { 
    tdsrrc_CN_InformationInfoFull  *core_network_info = NULL;
    ASN1BitStr32                *msg_crnti_ptr = NULL;
    tdsrrc_U_RNTI                  *msg_urnti_ptr = NULL;
    tdsrrc_H_RNTI                  *msg_hrnti_ptr = NULL;
    /* Get the pointer to the actual UMI message */
    msg_ptr = &(dcch_msg_ptr->message.u.utranMobilityInformation);

    
    /*Get the transaction id from the UMI message */
    if(dcch_msg_ptr->message.u.utranMobilityInformation.t == 
                                 T_tdsrrc_UTRANMobilityInformation_r3)
    {
      msg_version = TDSMSG_REL99;
      tdstransaction_id = 
                   dcch_msg_ptr->message.u.utranMobilityInformation.
                   u.r3.utranMobilityInformation_r3.rrc_TransactionIdentifier;
    }
    else
    {
      if ((dcch_msg_ptr->message.u.utranMobilityInformation.t == 
           T_tdsrrc_UTRANMobilityInformation_later_than_r3)
          && (dcch_msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.criticalExtensions.t == 
          T_tdsrrc_UTRANMobilityInformation_criticalExtensions_2_criticalExtensions )
          && (dcch_msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.criticalExtensions.u.criticalExtensions.t == 
          T_tdsrrc_UTRANMobilityInformation_criticalExtensions_1_r7 ))
      {
        /* Strictly speaking non-R99 message doesn't mean Rel-5 but if the 
           received message belongs to any other release, it would be filtered by
           "tdsrrcumi_validate_utran_mobility_info_command" call below. */
        msg_version = TDSMSG_REL7;
        r7_umi_msg_ptr = &dcch_msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.utranMobilityInformation_r7;
      }
      else
      {
      /* Strictly speaking non-R99 message doesn't mean Rel-5 but if the 
         received message belongs to any other release, it would be filtered by
         "tdsrrcumi_validate_utran_mobility_info_command" call below. */
      msg_version = TDSMSG_REL5;
      }
      tdstransaction_id = dcch_msg_ptr->message.u.utranMobilityInformation.u.
                         later_than_r3.rrc_TransactionIdentifier;
    }

    /*Check to see if primary PLMN I.D is present in UMI message ext
     * tdsrrc_UTRANMobilityInformation-v690ext which is present in 
     * v690NonCriticalExtensions
     */
    if(msg_version == TDSMSG_REL7)
    {
       if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(r7_umi_msg_ptr, primary_plmn_Identity))
       {
           primary_plmn_Id_present = TRUE;
           primary_plmn_Identity = r7_umi_msg_ptr->primary_plmn_Identity;
       }
    }
    else
    {
      primary_plmn_Id_present = tdsrrcumi_check_and_get_primary_plmn_id(msg_ptr,
                                                               &primary_plmn_Identity,msg_version);
    }


    /* ASN1 decoding is successful */
    /* Validate the RRC UTRAN MOBILITY INFO.  If validation==FALSE then return */
    if (TDSRRCUMI_SUCCESS != tdsrrcumi_validate_utran_mobility_info_command(dcch_msg_ptr) )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"UTRAN Mobility InfoMSG VALIDATION FAILED");

      /* Set the failure status */
      failure_status = TDSRRCUMI_MSG_UNSUPPORTED_CONFIG;

      /* Send UMI failure */
      tdsrrcumi_send_umi_failure (tdstransaction_id,
                               tdsumi.status.failure_status,
                               tdsumi.status.prot_err_cause);

      return;
    }



    /* If the UMI msg contains information for doing SRNS relocation */
    if (TRUE == tdsrrcumi_validate_srns_relocation_info(msg_ptr))
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
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IP check failed. Discarding UMI msg");
        return;

      case TDSRRCSMC_SRNS_SECURITY_INFO_INVALID:
      case TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION:

        tdsumi.status.failure_status = TDSRRCUMI_MSG_INVALID_CONFIG;
        /*do not clear the procedure or change the tdsrrc_rbe substate because
        some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
        return;

      default:
        /*should not come here */
        break;
      }
    }
    //code to process integrity info in UMI message..
    else if(tdsrrcumi_validate_umi_ip_mode(msg_ptr) == TRUE)
    {
      ip_check_status = tdsrrcsmc_check_integrity_for_srns_relocation(cmd_ptr, msg_version);

      switch (ip_check_status)
      {
      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS:
        //commit fresh values for srns relocation
        tdsrrcsmc_commit_fresh_for_srns_relocation();
        break;

      case TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE:
        /*Discard the message and continue with ongoing procedure as if this
        message was not received */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"IP check failed. Discarding UMI msg");
        return;

      case TDSRRCSMC_SRNS_SECURITY_INFO_INVALID:
      case TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION:

        tdsumi.status.failure_status = TDSRRCUMI_MSG_INVALID_CONFIG;
        /*do not clear the procedure or change the tdsrrc_rbe substate because
        some other tdsrrc_rbe procedure might be in progress. Just return from
        here */
        return;

      default:
        /*should not come here */
        break;
      }
    }


    ue_conn_timers_and_constants_ptr = NULL;
    ext_ptr = NULL;

#if defined(FEATURE_3GPP_FAST_DORMANCY)
  ue_conn_timers_and_constants_v860ext_ptr = NULL;
#endif

    /* Update UE Conn Timers and Constants if present and set internal flag*/
    if(msg_version == TDSMSG_REL99)
    {
      if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.r3.
         utranMobilityInformation_r3,
         tdsrrc_UTRANMobilityInformation_r3_IEs,ue_ConnTimersAndConstants))
      {
        tdsrrcumi_ue_timer_const_rcvd_status = TDSRRCUMI_UE_TIMER_CONST_RCVD;
        ue_conn_timers_and_constants_ptr = &(msg_ptr->u.r3.utranMobilityInformation_r3.
                    ue_ConnTimersAndConstants);
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.r3,
         tdsrrc_UTRANMobilityInformation_r3,v3a0NonCriticalExtensions))
      {
        tdsrrcumi_ue_timer_const_rcvd_status = TDSRRCUMI_UE_TIMER_CONST_RCVD;

        ext_ptr = &(msg_ptr->u.r3.v3a0NonCriticalExtensions.
          utranMobilityInformation_v3a0ext.ue_ConnTimersAndConstants_v3a0ext);

#if defined(FEATURE_3GPP_FAST_DORMANCY)
        if(msg_ptr->u.r3.v3a0NonCriticalExtensions.m.laterNonCriticalExtensionsPresent)
        {
          if(msg_ptr->u.r3.v3a0NonCriticalExtensions.laterNonCriticalExtensions.m.
		  	v690NonCriticalExtensionsPresent)
          {
            if(msg_ptr->u.r3.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
              v690NonCriticalExtensions.m.v860NonCriticalExtentionsPresent)
            {
              if(msg_ptr->u.r3.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                 v690NonCriticalExtensions.v860NonCriticalExtentions.utranMobilityInformation_v860ext.
                 ue_ConnTimersAndConstants.m.t_323Present)
              {
                ue_conn_timers_and_constants_v860ext_ptr = &(msg_ptr->u.r3.v3a0NonCriticalExtensions.
                  laterNonCriticalExtensions.v690NonCriticalExtensions.v860NonCriticalExtentions.
                  utranMobilityInformation_v860ext.ue_ConnTimersAndConstants);
              }
              else
              {
                ue_conn_timers_and_constants_v860ext_ptr = NULL;
              }              
            }
          }
        }
#endif
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.r3.
         utranMobilityInformation_r3,
         tdsrrc_UTRANMobilityInformation_r3_IEs,cn_InformationInfo))
      {
        core_network_info = &(msg_ptr->
                          u.r3.utranMobilityInformation_r3.cn_InformationInfo);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Extracted core network info from R3 IE");
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.r3.
         utranMobilityInformation_r3, 
         tdsrrc_UTRANMobilityInformation_r3_IEs,new_C_RNTI))
      {
        msg_crnti_ptr = &msg_ptr->u.r3.
             utranMobilityInformation_r3.new_C_RNTI;
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.r3.
         utranMobilityInformation_r3,
         tdsrrc_UTRANMobilityInformation_r3_IEs,new_U_RNTI))
      {
        msg_urnti_ptr = &(msg_ptr->u.
         r3.utranMobilityInformation_r3.new_U_RNTI);
      }
    }
    else if(msg_version == TDSMSG_REL7)
    {
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(r7_umi_msg_ptr,
         tdsrrc_UTRANMobilityInformation_r7_IEs,ue_ConnTimersAndConstants))
      {
        tdsrrcumi_ue_timer_const_rcvd_status = TDSRRCUMI_UE_TIMER_CONST_RCVD;

        /*lint -e740 */
        ue_conn_timers_and_constants_ptr =  (tdsrrc_UE_ConnTimersAndConstants *) &(r7_umi_msg_ptr->ue_ConnTimersAndConstants);
        /*lint +e740 */
        
#if defined(FEATURE_3GPP_FAST_DORMANCY)
        if(dcch_msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.criticalExtensions.u.
           criticalExtensions.u.r7.m.v860NonCriticalExtensionsPresent)
        {
          ue_conn_timers_and_constants_v860ext_ptr = &(dcch_msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.
                                                      criticalExtensions.u.criticalExtensions.u.r7.v860NonCriticalExtensions.
                                                      utranMobilityInformation_v860ext.ue_ConnTimersAndConstants);
        }
        else
        {
          ue_conn_timers_and_constants_v860ext_ptr = NULL;
        }
#endif
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(r7_umi_msg_ptr, 
         tdsrrc_UTRANMobilityInformation_r7_IEs,cn_InformationInfo))
      {
        core_network_info = &(r7_umi_msg_ptr->cn_InformationInfo);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Extracted core network info from R5 IE");
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(r7_umi_msg_ptr,
         tdsrrc_UTRANMobilityInformation_r7_IEs,new_C_RNTI))
      {
        msg_crnti_ptr = &r7_umi_msg_ptr->new_C_RNTI;
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(r7_umi_msg_ptr,
         tdsrrc_UTRANMobilityInformation_r7_IEs,new_U_RNTI))
      {
        msg_urnti_ptr = &(r7_umi_msg_ptr->new_U_RNTI);
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(r7_umi_msg_ptr, 
         tdsrrc_UTRANMobilityInformation_r7_IEs,new_H_RNTI))
      {
        msg_hrnti_ptr = &(r7_umi_msg_ptr->new_H_RNTI);
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"HRNTI not yet supported in UMI");
      }
    }

    else if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
    {
      if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.r5.
         utranMobilityInformation_r5,
         tdsrrc_UTRANMobilityInformation_r5_IEs,ue_ConnTimersAndConstants))
      {
        tdsrrcumi_ue_timer_const_rcvd_status = TDSRRCUMI_UE_TIMER_CONST_RCVD;

        /*lint -e740 */
        ue_conn_timers_and_constants_ptr =  (tdsrrc_UE_ConnTimersAndConstants *) &(msg_ptr->u.
            later_than_r3.criticalExtensions.u.r5.utranMobilityInformation_r5.
            ue_ConnTimersAndConstants);
        /*lint +e740 */
        
#if defined(FEATURE_3GPP_FAST_DORMANCY)
        if(msg_ptr->u.later_than_r3.criticalExtensions.u.r5.m.v690NonCriticalExtensionsPresent)
          
        {
          if(msg_ptr->u.later_than_r3.criticalExtensions.u.r5.v690NonCriticalExtensions.
		  	m.v860NonCriticalExtentionsPresent)
          {

            ue_conn_timers_and_constants_v860ext_ptr = &(msg_ptr->u.later_than_r3.criticalExtensions.u.r5.
                                                     v690NonCriticalExtensions.v860NonCriticalExtentions.
                                                     utranMobilityInformation_v860ext.ue_ConnTimersAndConstants);
          }
          else
          {
            ue_conn_timers_and_constants_v860ext_ptr = NULL;
          }

        }
#endif
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.
        criticalExtensions.u.r5.
         utranMobilityInformation_r5,
         tdsrrc_UTRANMobilityInformation_r5_IEs,cn_InformationInfo))
      {
        core_network_info = &(msg_ptr->u.later_than_r3.criticalExtensions.
                            u.r5.utranMobilityInformation_r5.cn_InformationInfo);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Extracted core network info from R5 IE");
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE(dcch_msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.
         criticalExtensions.u.r5.utranMobilityInformation_r5,
         tdsrrc_UTRANMobilityInformation_r5_IEs,new_C_RNTI))
      {
        msg_crnti_ptr = &dcch_msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.
         criticalExtensions.u.r5.utranMobilityInformation_r5.new_C_RNTI;
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE(dcch_msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.
         criticalExtensions.u.r5.utranMobilityInformation_r5,
         tdsrrc_UTRANMobilityInformation_r5_IEs,new_U_RNTI))
      {
        msg_urnti_ptr = &(msg_ptr->u.
          later_than_r3.criticalExtensions.u.r5.utranMobilityInformation_r5.
          new_U_RNTI);
      }
    }

    if((ue_conn_timers_and_constants_ptr != NULL) || 
        (ext_ptr != NULL))
    {    
      (void)tdsrrc_update_ue_conn_timers_and_constants
                  (ue_conn_timers_and_constants_ptr, 
                  ext_ptr
#if defined(FEATURE_3GPP_FAST_DORMANCY)
                  ,ue_conn_timers_and_constants_v860ext_ptr
#endif
                  );

      if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
      {
        if(msg_version == TDSMSG_REL5)
        {  
          (void)tdsrrc_update_constants_n312_and_n315_r5(&(msg_ptr->u.
              later_than_r3.criticalExtensions.u.r5.utranMobilityInformation_r5.
              ue_ConnTimersAndConstants));
        } 
        else if(msg_version == TDSMSG_REL7)
        {  
          (void)tdsrrc_update_constants_n312_and_n315_r5(&(r7_umi_msg_ptr->ue_ConnTimersAndConstants));
        }
      }

      if(tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)
      {
        tdsl1_upd_dl_sync_parms_cmd_type  umi_info;
        tdsrrc_cmd_type                  *rrc_cmd_ptr;

        /* Fill the contents of the connected mode timers */
        umi_info.N315 = (uint16)TDSRRC_GET_N315();
        umi_info.N312 = (uint16)TDSRRC_GET_N312();
        umi_info.N313 = tdsrrc_translate_n313(TDSRRC_GET_N313());
        umi_info.T312 = (uint8)TDSRRC_GET_T312();
        umi_info.T313 = (uint8)TDSRRC_GET_T313();

        if ((rrc_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
        { 
          tdsl1_req_cmd * rrc_cmd_l1_req = NULL;
          tdsrrc_get_pointer_l1_req_value(&rrc_cmd_ptr->cmd.rrc_llc_req.l1_req, &rrc_cmd_l1_req);
  
         
          rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_LLC_CMD_REQ;
          rrc_cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_UMI;
          rrc_cmd_ptr->cmd.rrc_llc_req.cmd_dest  = TDSLAYER1_PHY;

          rrc_cmd_l1_req->connected_timer_info = umi_info;
          rrc_cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_UPD_DL_SYNC_PARMS;
          tdsrrc_put_int_cmd(rrc_cmd_ptr);
        } 
        else
        { 
          ERR_FATAL("Could not get internal cmd buf",0,0,0);
        } 
      } 
    }

    /* Get the transaction identifier from the UTRAN MOBILITY INFO and
           call the UTRAN Mobility Info command
    */
    tdsrrcumi_utran_mob_inf_cnf.transaction_id = tdstransaction_id;

    /* Get the cn information if present and update upper layers */
    if(core_network_info != NULL)
    {
      if( tdsrrcccm_send_nas_cn_info_full(
            core_network_info
                     ,primary_plmn_Id_present
                     ,&primary_plmn_Identity
            ) == FAILURE)
      {
        /*flag an error if not successful*/
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Nas CN Info send failed");
      }
    }

    /*check if a new c_rnti is present and update rnti_update_req struct*/
    if(msg_crnti_ptr != NULL)
    {
      tdsrrc_translate_crnti(msg_crnti_ptr, &tdsumi.new_crnti);
      tdsumi.new_crnti_valid = TRUE;
    }

    /*check if a new u_rnti is present and update rnti_update_req struct*/
    if(msg_urnti_ptr != NULL)
    {
      tdsrrc_translate_urnti(msg_urnti_ptr, &tdsumi.new_urnti);
      tdsumi.new_urnti_valid = TRUE;
    }
    /*check if a new u_rnti is present and update rnti_update_req struct*/
    if(msg_hrnti_ptr != NULL)
    {
      (void)tdsrrc_translate_hrnti(msg_hrnti_ptr, &tdsumi.new_hrnti);
      tdsumi.new_hrnti_valid = TRUE;
    }
    /*check whether this reconfiguration involves SRNS relocation */
    if (tdsumi.umi_for_srns_relocation == TRUE)
    {
      //commit fresh values for srns relocation
      tdsrrcsmc_commit_fresh_for_srns_relocation();

      /* Re-establish SRB2 before sending the response message */
      rb_type  = TDSRRC_RE_EST_RB_2;
      tdsrrcrb_send_rlc_re_est_req_to_smc(TDSRRC_PROCEDURE_UMI, rb_type, TRUE);

      tdsumi.curr_substate = TDSRRCUMI_WAIT_FOR_REESTABLISH_SRB2;
      tdsrrcumi_substate = TDSRRCUMI_WAIT_FOR_REESTABLISH_SRB2;

#if defined(FEATURE_3GPP_FAST_DORMANCY)
      tdsrrcdormancy_update_dormancy_for_srns_hho_cco(TDSRRC_PROCEDURE_UMI);
#endif  
    }
    else
    { /*normal reconfiguration without SRNS relocation */   
      /* Send the UMI Complete message */
      tdsrrcumi_process_rnti_info();
    }

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
    if((msg_version == TDSMSG_REL7) &&
        (dcch_msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.
        criticalExtensions.u.criticalExtensions.u.r7.m.v860NonCriticalExtensionsPresent) &&
         (dcch_msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.
          v860NonCriticalExtensions.utranMobilityInformation_v860ext.m.dedicatedPriorityInformationPresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"ABSOL PRI: Dedicated priority information present in UMI message");
      ded_pri_info_ptr =
        &(dcch_msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.
          v860NonCriticalExtensions.utranMobilityInformation_v860ext.dedicatedPriorityInformation);

      /*Call the measurement function to update the dedicated priority information stored*/
      tdsrrcmeas_update_dedicated_priority_info_from_ota(ded_pri_info_ptr);

      /*Post the command to measurement, so that meas can send the meas_req to WL1*/
      tdsrrcmeas_post_dedicated_priority_change_indication();
    }
#endif
  }
} /* tdsrrcumi_process_utran_mobility_info_command */

/*===========================================================================

FUNCTION   tdsrrcumi_is_srns_in_progress

DESCRIPTION
  This function returns if SRNS is in progress with UMI procedure

DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcumi_is_srns_in_progress
(
void
)
{
  return tdsumi.umi_for_srns_relocation;
}

/*===========================================================================

FUNCTION   RRCUMI_PROCESS_RLC_REESTABLISHMENT_CNF

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
static void tdsrrcumi_process_rlc_reestablishment_cnf
(
void
)
{
  /* Check if rnti update has to be done */
  tdsrrcumi_process_rnti_info();

}

/*===========================================================================

FUNCTION   RRCUMI_PROCESS_L2_ACK_IND

DESCRIPTION

  This function processes the received Layer 2 ACK indication.
  If the L2 ACK indicates that the UMI Complete message has
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
static void tdsrrcumi_process_l2_ack_ind
(
  tdsrrc_downlink_l2ack_ind_type *ptr /* Pointer to the L2 Ack Ind cmd */
)
{

  tdsrrc_rlc_re_est_for_srns_rb_e_type rb_type; /*rb type tobe reestablished */

  if ( ptr->status == SUCCESS )
  {
    /* if SRNS relocation is involved with this message then re
    establish the RBs other than RB2 here */
    if (tdsumi.umi_for_srns_relocation)
    {
      /* Initiate the re-establishment of remaining RBs before 
         RBRC procedure goes back to initial substate. 
         No need to wait for the confirmation */
      rb_type  = TDSRRC_RE_EST_NON_RB_2_UM_AM;
      tdsrrcrb_send_rlc_re_est_req_to_smc(TDSRRC_PROCEDURE_UMI, rb_type, FALSE);
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"SRNS reloc flag is ON in rrc state %d",tdsrrc_get_state());
    }
  }
  else
  {
	/* L2 Ack has failed.Take UE to idle */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2 Ack failed for UMICnf.Taking UE to Idle");

	(void)tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_UMI, TDSRRC_TX_TO_DISCON_OTHER);
  }

  /* Go to the next sub-state in the state machine. That's
  the initial substate since we've sent the TCRC complete
  message */
  tdsumi.curr_substate = TDSRRCUMI_INITIAL;
  tdsrrcumi_substate = TDSRRCUMI_INITIAL;

  /* Clear our internal variables */
  tdsrrcumi_clear_procedure();
}

/*===========================================================================

FUNCTION   tdsrrcumi_process_cell_fach_state

DESCRIPTION
   This function processes the UTRAN MOBILITY INFO received on DL DCCH
   and extracts the Transaction ID.  

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcumi_process_cell_fach_state
(
  tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{

  switch (tdsrrcumi_substate)
  {
    case TDSRRCUMI_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_DOWNLINK_SDU_IND:
          tdsrrcumi_process_utran_mobility_info_command(cmd_ptr);
          break;

        default:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid event: %d in Substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcumi_substate);
          break;
      }
      break;

    case TDSRRCUMI_WAIT_FOR_RNTI_UPDATE_CNF:
      tdsrrcumi_process_rnti_update_cnf_substate(cmd_ptr);
      break;


  case TDSRRCUMI_WAIT_FOR_REESTABLISH_SRB2:
    tdsrrcumi_process_rlc_reestablishment_cnf();
    break;

  case TDSRRCUMI_WAIT_FOR_L2_ACK:
    if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_L2ACK_IND )
    {
      if (tdsumi.umi_for_srns_relocation == TRUE)
      {
        tdsrrcumi_process_l2_ack_ind(&(cmd_ptr->cmd.downlink_l2ack_ind));
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"UMI not expecting L2 ACK now");
      }
      
    }
    break;
  
  default:
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Substate for UMI: %d",tdsrrcumi_substate);
    break;
  }
}

/*===========================================================================

FUNCTION   tdsrrcumi_process_cell_dch_state

DESCRIPTION
   This function processes the UTRAN MOBILITY INFO received on DL DCCH
   and extracts the Transaction ID.  

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcumi_process_cell_dch_state
(
tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{

  switch (tdsrrcumi_substate)
  {
  case TDSRRCUMI_INITIAL:
    switch (cmd_ptr->cmd_hdr.cmd_id)
    {
    case TDSRRC_DOWNLINK_SDU_IND:
      tdsrrcumi_process_utran_mobility_info_command(cmd_ptr);
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid event: %d in Substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcumi_substate);
      break;
    }
    break;

  case TDSRRCUMI_WAIT_FOR_RNTI_UPDATE_CNF:
    tdsrrcumi_process_rnti_update_cnf_substate(cmd_ptr);
    break;


  case TDSRRCUMI_WAIT_FOR_REESTABLISH_SRB2:
    tdsrrcumi_process_rlc_reestablishment_cnf();
    break;

  case TDSRRCUMI_WAIT_FOR_L2_ACK:
    if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_L2ACK_IND )
    {
      if (tdsumi.umi_for_srns_relocation == TRUE)
      {
        tdsrrcumi_process_l2_ack_ind(&(cmd_ptr->cmd.downlink_l2ack_ind));
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"UMI not expecting L2 ACK now");
      }
      
    }
    break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Substate for UMI: %d",tdsrrcumi_substate);
      break;
  }
}

/*===========================================================================

FUNCTION   tdsrrcumi_process_disconnected_state

DESCRIPTION
   This function processes UMI in disconnected state.  This function will be
   called with there is state change while UMI is in wait for rnti update cnf

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcumi_process_disconnected_state
(
  tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{
  switch (tdsrrcumi_substate)
  {
    case TDSRRCUMI_WAIT_FOR_RNTI_UPDATE_CNF:
      tdsrrcumi_process_rnti_update_cnf_substate(cmd_ptr);
      break;
    
    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Substate for UMI: %d",tdsrrcumi_substate);
      break;
  }
}/*tdsrrcumi_process_disconnected_state*/
/*===========================================================================

FUNCTION tdsrrcumi_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for UTRAN MOBILITY INFO
  The following events are handled by this procedure:

  TDSRRC_DOWNLINK_SDU_IND Primitive (received from RLC) containing 
  UTRAN MOBILITY INFO and only in DCH state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcumi_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
 
  tdsrrc_state_e_type rrc_state;

  //UMI only registers from wildcard to disconnected, handle this right
  //here in procedure event handler
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_STATE_CHANGE_IND)
  {
    tdsrrcumi_substate             = TDSRRCUMI_INITIAL;
    tdsumi.umi_for_srns_relocation = FALSE;
    tdsumi.new_crnti_valid         = FALSE;
    tdsumi.new_urnti_valid         = FALSE;
   tdsumi.new_hrnti_valid = FALSE;


    tdsrrcumi_ue_timer_const_rcvd_status  = TDSRRCUMI_UE_TIMER_CONST_NOT_RCVD;
    return;
  }

  //get rrc state
  rrc_state = tdsrrc_get_state();

  switch (rrc_state)
  {

    case TDSRRC_STATE_CELL_FACH:    
      /* CELL_FACH only valid state to receive UTRAN MOBILITY INFO message */
    tdsrrcumi_process_cell_fach_state(cmd_ptr); 
    break;


    case TDSRRC_STATE_DISCONNECTED: 
      /*we may receive this message when there is a transition to idle discon
        state while waiting for crnti_update_req_cnf.  so reset RRCUMI's state
        machine in this case*/
      tdsrrcumi_process_disconnected_state(cmd_ptr);
      break;

  case TDSRRC_STATE_CELL_DCH:
    tdsrrcumi_process_cell_dch_state(cmd_ptr);
    break;

    case TDSRRC_STATE_CONNECTING:
    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:
      /* UTRAN MOBILITY INFO is valid only in cell fach state */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCUMI Msg received in Invalid State: %d",rrc_state); 
      break;

    default: 
      /* This should never happen */
      ERR_FATAL("RRCUMI Msg received in Invalid State: %d",rrc_state,0,0);
      break;
   }  
} /* tdsrrcumi_procedure_event_handler */
/*===========================================================================

FUNCTION tdsrrcumi_init_procedure

DESCRIPTION
  This function initializes required parameters for UTRAN MOBILITY INFO
  procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcumi_init_procedure
( 
  void
)
{
  /*initialize UMI substate to INITIAL and const_timer received state to
   *not received
   */
  tdsrrcumi_substate            = TDSRRCUMI_INITIAL;
  tdsrrcumi_ue_timer_const_rcvd_status = TDSRRCUMI_UE_TIMER_CONST_NOT_RCVD;
  
  tdsumi.umi_for_srns_relocation = FALSE;
  tdsumi.new_crnti_valid = FALSE;
  tdsumi.new_urnti_valid = FALSE;
   tdsumi.new_hrnti_valid = FALSE;


  tdsrrcscmgr_register_for_scn(TDSRRC_PROCEDURE_UMI,
                            TDSRRC_STATE_WILDCARD,
                            TDSRRC_STATE_DISCONNECTED);     
} /* tdsrrcumi_init_procedure */
