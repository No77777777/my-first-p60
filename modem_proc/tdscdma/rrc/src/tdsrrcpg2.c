/*===========================================================================
                  R R C  P A G I N G  T Y P E 2

DESCRIPTION
  This file contains the implementation for the Paging type 2 procedure.

EXTERNALIZED FUNCTIONS
  tdsrrcpg2_procedure_event_handler
    This function is the event handler for all events that are sent to the
    Paging Type 2 procedure.
  
INTERNAL FUNCTIONS
  tdsrrcpg2_update_rrc_page_ind  
    This function updates the parameters that are required for RRC_PAGE_IND
    command.
  
  rrcpg2_process_paging_type1_msg  
    This function checks whether TDSRRC_DOWNLINK_SDU_IND contains Paging Type 2
    message.If it contains Paging Type 2 mesage,it will processes the same and
    send RRC_PAGE_IND to the MM. 
  

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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcpg2.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/08/11   mkh    Events renamed to TDS
03/10/11   bj     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "tdsrrclcm.h"
#include "tdsrrcsend.h"
#include "comdef.h"

#include "tdsrrccmd_v.h"               /* RRC command definitions */
#include "msg.h"
#include "err.h"
#include "tdsrrcasn1.h"
#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "tdsrrcscmgr.h"
#include "tdsrrcpg2.h"
#include "tdsrrclogging.h"

#ifdef T_WINNT  /* Include files when logging is enabled.*/
#error code not present
#endif /* T_WINNT */
#ifdef TEST_FRAMEWORK
#error code not present
#endif
#include "tdsrrc_dependancy.h"

#include "tdsCFAParser_i.h"
#include "tdsrrcnv.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */



typedef enum
{
  TDSRRCPG2_SUCCESS,                            /* Successful Status */      
  TDSRRCPG2_FAILURE_MINOR                      /* Minor Failure Status */
} tdsrrcpg2_status_e_type;


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

rrc_cn_domain_identity_e_type   tdsrrcpg2_cn_domain_id;
rrc_paging_cause_e_type         tdsrrcpg2_paging_cause;
/* The UE ID used for paging */
rrc_page_ue_id_choice_e_type    tdsrrcpg2_page_ue_id;

/* Transaction Id for Paging Type 2 Msg */
tdsrrc_RRC_TransactionIdentifier   tdsrrcpg2_transaction_id;
/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================

FUNCTION tdsrrcpg2_update_rrc_page_ind  

DESCRIPTION
  This function updates the parameters that are required for RRC_PAGE_IND
  command.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcpg2_update_rrc_page_ind
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr        /* Pointer to DL_DCCH message*/
)
{
  
  tdsrrc_event_paging_info_type paging_event_info;
  
  switch(dcch_msg_ptr->message.u.pagingType2.cn_DomainIdentity)
  { 
    case tdsrrc_CN_DomainIdentity_cs_domain:
      tdsrrcpg2_cn_domain_id = RRC_CS_DOMAIN_CN_ID;
      break;

    case tdsrrc_CN_DomainIdentity_ps_domain:
      tdsrrcpg2_cn_domain_id = RRC_PS_DOMAIN_CN_ID;
      break;

    default:
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unknown CN Domian Id: %d is received", dcch_msg_ptr->message.u.
          pagingType2.cn_DomainIdentity,0,0);
      break;
  }

  switch(dcch_msg_ptr->message.u.pagingType2.pagingCause)
  {
    case tdsrrc_PagingCause_terminatingConversationalCall:
      tdsrrcpg2_paging_cause = RRC_PAGE_CAUSE_TERM_CONV_CALL;
      break;

    case tdsrrc_PagingCause_terminatingStreamingCall:
      tdsrrcpg2_paging_cause = RRC_PAGE_CAUSE_TERM_STREAM_CALL;
      break;

    case tdsrrc_PagingCause_terminatingInteractiveCall:
      tdsrrcpg2_paging_cause = RRC_PAGE_CAUSE_TERM_INTERACT_CALL;
      break;

    case tdsrrc_PagingCause_terminatingBackgroundCall:
      tdsrrcpg2_paging_cause = RRC_PAGE_CAUSE_TERM_BACKGND_CALL;
      break;

    case tdsrrc_PagingCause_terminatingHighPrioritySignalling:
      tdsrrcpg2_paging_cause = RRC_PAGE_CAUSE_HIGH_PRI_SIGNALLING;
      break;

    case tdsrrc_PagingCause_terminatingLowPrioritySignalling:
      tdsrrcpg2_paging_cause = RRC_PAGE_CAUSE_LOW_PRI_SIGNALLING;
      break;

    case tdsrrc_PagingCause_terminatingCauseUnknown:
      tdsrrcpg2_paging_cause = RRC_PAGE_CAUSE_UNKNOWN;
      break;

    default:
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unknown Paging Cause: %d", dcch_msg_ptr->message.u.
          pagingType2.pagingCause,0,0);
      break;
  }

  switch( dcch_msg_ptr->message.u.pagingType2.pagingRecordTypeID)
  {
    case tdsrrc_PagingRecordTypeID_imsi_GSM_MAP:
      tdsrrcpg2_page_ue_id = RRC_PAGE_WITH_IMSI;
      break;

    case tdsrrc_PagingRecordTypeID_tmsi_GSM_MAP_P_TMSI:
      if(tdsrrcpg2_cn_domain_id == RRC_CS_DOMAIN_CN_ID)
      {
        tdsrrcpg2_page_ue_id = RRC_PAGE_WITH_TMSI;
      }
      else if(tdsrrcpg2_cn_domain_id == RRC_PS_DOMAIN_CN_ID)
      {
        tdsrrcpg2_page_ue_id = RRC_PAGE_WITH_PTMSI;
      }
      break;

    case tdsrrc_PagingRecordTypeID_imsi_DS_41:
    case tdsrrc_PagingRecordTypeID_tmsi_DS_41:
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unsupported UE Id %d is received",dcch_msg_ptr->message.u.pagingType2.pagingRecordTypeID);
    break;
  }

  /* Fill the Paging type */
  paging_event_info.paging_type = TDSRRCLOG_PAGING_TYPE_2;
  /* Populate the paging cause field with the appropriate cause*/
  paging_event_info.paging_cause = tdsrrcpg2_paging_cause;
  /* Send the event */

  event_report_payload(EVENT_TDSCDMA_RRC_PAGE_RECEIVED, /* Paging event ID */
                       sizeof(tdsrrc_event_paging_info_type), /* payload size */
                       &paging_event_info); /* paging cause and type */
  
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Page2 type %d,cause %d ",paging_event_info.paging_type,paging_event_info.paging_cause);

} /* tdsrrcpg2_update_rrc_page_ind */
/*===========================================================================

FUNCTION tdsrrcpg2_validate_paging_type_msg  

DESCRIPTION
  This function looks into all Paging Records of the Paging Type 2 message
  until if finds a match for either IMSI, TMSI or PTMSI.

DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcpg2_status_e_type : Returns TDSRRCPG2_SUCCESS if it finds match. Otherwise
  it returns TDSRRCPG2_FAILURE_MINOR.

===========================================================================*/

static tdsrrcpg2_status_e_type tdsrrcpg2_validate_paging_type_msg
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr        /* Pointer to DCCH message*/
)
{
  tdsrrcpg2_status_e_type status = TDSRRCPG2_FAILURE_MINOR;

  /* Check whether externsion chosen */
  if(TDSRRC_MSG_COMMON_BITMASK_IE(dcch_msg_ptr->message.u.pagingType2,
      tdsrrc_PagingType2,laterNonCriticalExtensions))
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"Non Critical Extensions not supported");
  }
  if( (dcch_msg_ptr->message.u.pagingType2.cn_DomainIdentity !=
      tdsrrc_CN_DomainIdentity_cs_domain) &&
      (dcch_msg_ptr->message.u.pagingType2.cn_DomainIdentity !=
      tdsrrc_CN_DomainIdentity_ps_domain) )
  {
    return (status);
  }
  if(dcch_msg_ptr->message.u.pagingType2.pagingCause >
    tdsrrc_PagingCause_terminatingCauseUnknown )
  {
    return (status);
  }
  if( (dcch_msg_ptr->message.u.pagingType2.pagingRecordTypeID != 
      tdsrrc_PagingRecordTypeID_imsi_GSM_MAP) &&
      (dcch_msg_ptr->message.u.pagingType2.pagingRecordTypeID != 
      tdsrrc_PagingRecordTypeID_tmsi_GSM_MAP_P_TMSI) )
  {
    return(status);
  }
  return(TDSRRCPG2_SUCCESS);

} /*tdsrrcpg2_validate_paging_type_msg */
/*===========================================================================

FUNCTION tdsrrcpg2_process_paging_type2_msg  

DESCRIPTION
  This function checks whether TDSRRC_DOWNLINK_SDU_IND contains Paging Type 2
  message.If it contains Paging Type 2 mesage,it will processes the same and
  send RRC_PAGE_IND to the MM. 

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcpg2_process_paging_type2_msg
(
  tdsrrc_cmd_type *cmd_ptr                    /* Pointer to the RRC Command */
)
{
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;  /* Pointer to DCCH message*/

  mm_cmd_type *mm_cmd_ptr;                 /* Pointer to the MM Command */

  uint8 message_choice;                    /* Local varibale for message choice*/  

  rlc_lc_id_type lc_id;                    /* Local variable to store RLC LC Id */

  tdsrrc_state_e_type rrc_state;              /* local variable to store rrc_state */
   
  /* Assert received message is Downlink DCCH message */
  if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_DCCH_Message_PDU) 
  {
    message_choice = 
      tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                   cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

    /*check if decoding was successful otherwise set error flag to true*/
    tdsrrc_protocol_error_reject = FALSE;

    if (cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL)
    {
      dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) 
      cmd_ptr->cmd.downlink_sdu_ind.decoded_msg; 
    }
    else
    { /* ASN1 decoding failed */
      tdsrrc_protocol_error_reject = TRUE;
    } 
    
    /* Assert the received message is Paging Type 2 */
    if( message_choice == T_tdsrrc_DL_DCCH_MessageType_pagingType2 )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Paging Type2 Message is received");

      if(tdsrrc_protocol_error_reject == TRUE)
      {
#if(!defined(T_WINNT))
         /*log the protocol error*/
        tdsrrc_log_protocol_error( (uint8) tdsrrc_get_state(),
                                (uint8 )TDSRRC_PROCEDURE_PG2,
                                (uint8) TDSRRCLOG_PROTOCOL_ERROR,
                                TDSRRCLOG_ASN1_ENC_ERR
                               );
#endif
         
        /*if we are in cell_pch or ura_pch state then do not send status
         *message, as these cases we cannot send the status on ul dcch.
         *we are handling this case only in case of race conditions
         *as a precaution
         */
        rrc_state = tdsrrc_get_state();

        if( ( rrc_state == TDSRRC_STATE_CELL_PCH ) ||
            ( rrc_state == TDSRRC_STATE_URA_PCH  )  )
        {
          return;
        }

        lc_id = tdsrrclcm_check_ul_rlc_lc_id(    
                UE_LOGCHAN_DCCH,          /* Uplink Logical Channel Type. */
                DCCH_AM_RADIO_BEARER_ID,  /* Radio Bearer Id assoicated with
                                            the above Downlink Logical 
                                            Channel type. */
                UE_MODE_ACKNOWLEDGED_DATA);
                                          /* Mode of transmission for the RLC 
                                             service entity. */
        if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND == lc_id)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cann't find the RLC Id for RB 2");
        }       
        else /* Valid RLC Id */
        {
          if(SUCCESS != tdsrrcsend_send_rrc_status_message(
             T_tdsrrc_ProtocolErrorMoreInformation_type1_asn1_ViolationOrEncodingError,
                                           // The protocol error cause to be placed in
                                           // the message
             0,                            // Set transaction id to 0 since ASN1 
                                           // decoding failed 
             tdsrrc_ReceivedMessageType_pagingType2, 
                                           // Received message identification
             TDSRRC_PROCEDURE_PG2,            // RRC procedure
             lc_id,                        /* Logical Channel Id */
             TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                     /* Indicates whether L2 Ack required or not */
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to xmit RRC STATUS msg");
          } 
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC STATUS msg xmitted");
          }
        } /* Valid RLC Id */
      }
      else
      {
        /*lint -save -e613 */
        /* Update the Transaction Id */
        tdsrrcpg2_transaction_id = 
          dcch_msg_ptr->message.u.pagingType2.rrc_TransactionIdentifier; 

        /* ASN1 decoding is successful.  Validate the Paging Type 2 message */
        if(TDSRRCPG2_SUCCESS == tdsrrcpg2_validate_paging_type_msg(dcch_msg_ptr) )
        {
          /* Allocate memory for RRC_PAGE_IND command */
          if( (mm_cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_PAGE_IND)) != NULL)
          {
            /*update the global variables that will populate the mm cmd buffer*/
            tdsrrcpg2_update_rrc_page_ind(dcch_msg_ptr);

            /* Fill in parameters fro RRC_PAGE_IND command */
     
            mm_cmd_ptr->cmd.rrc_page_ind.cn_domain_id = tdsrrcpg2_cn_domain_id;

            mm_cmd_ptr->cmd.rrc_page_ind.paging_cause = tdsrrcpg2_paging_cause;

            mm_cmd_ptr->cmd.rrc_page_ind.page_ue_id = tdsrrcpg2_page_ue_id;

#ifdef FEATURE_DUAL_SIM
            mm_cmd_ptr->cmd.rrc_page_ind.as_id = tdsrrc_get_as_id();
#endif

            TDSRRC_MSG0(MSG_LEGACY_HIGH," Sent RRC_PAGE_IND to the MM");
            /* Sends the command to MM */
            if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
            {
              tdscfa_mm_put_cmd(mm_cmd_ptr);
            }
            mm_put_cmd(mm_cmd_ptr);
          }
          else
          {
            ERR_FATAL(" Failed allocated memory for MM Command: %d",RRC_PAGE_IND,0,0);
          }
        }/*if for validate function else*/
        else
        {
          lc_id = tdsrrclcm_check_ul_rlc_lc_id(    
                UE_LOGCHAN_DCCH,          /* Uplink Logical Channel Type. */
                DCCH_AM_RADIO_BEARER_ID,  /* Radio Bearer Id assoicated with
                                            the above Downlink Logical 
                                            Channel type. */
                UE_MODE_ACKNOWLEDGED_DATA);
                                          /* Mode of transmission for the RLC 
                                             service entity. */
          if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND == lc_id)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cann't find the RLC Id for RB 2");
          }
          else
          {
            if(SUCCESS != tdsrrcsend_send_rrc_status_message(
               T_tdsrrc_ProtocolErrorMoreInformation_type1_ie_ValueNotComprehended,
                                             // The protocol error cause to be placed in
                                             // the message
               tdsrrcpg2_transaction_id,        // Set transaction id 
               tdsrrc_ReceivedMessageType_pagingType2, 
                                             // Received message identification
               TDSRRC_PROCEDURE_PG2,            // RRC procedure
               lc_id,                        /* Logical Channel Id */
               TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                       /* Indicates whether L2 Ack required or not */
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to xmit RRC STATUS msg");
            } 
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC STATUS msg xmitted");
            }
          }
        }
        /*lint -restore */
      }/*else for asn1 decoding success*/
    }/*if for message type check*/
  }/*if for sdu type check*/
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d or SDU type: %d",cmd_ptr->cmd_hdr.cmd_id,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
  }
} /* tdsrrcpg2_process_paging_type2_msg */
/*===========================================================================

FUNCTION tdsrrcpg2_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the Paging Type 2 procedure.

  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (with PAGING TYPE 1 Message)


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcpg2_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
 
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  switch (rrc_state)
  {
    case TDSRRC_STATE_DISCONNECTED: 
    case TDSRRC_STATE_CONNECTING:
      /* This should never happen in above states */
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"PG2 received in Invalid RRC State: %d",rrc_state);
      break;
 
    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:
      /* These two states are invalid to receive Paging Type 2 message. 
       * We are handling the cases to take care of race conditions when
       * we get this message as we move from fach/dch to cell_pch/ura_pch state */
        tdsrrcpg2_process_paging_type2_msg(cmd_ptr); 
        break;

    case TDSRRC_STATE_CELL_FACH:    
    case TDSRRC_STATE_CELL_DCH:  
        /* These two states are valid to receive Paging Type 2 message. */
        tdsrrcpg2_process_paging_type2_msg(cmd_ptr); 
      break;

    default: 
      /* This should never happen since all the states are covered above */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"PG2 received in Invalid RRC State: %d",rrc_state);
      break;
   }  
} /* tdsrrcpg2_procedure_event_handler */

