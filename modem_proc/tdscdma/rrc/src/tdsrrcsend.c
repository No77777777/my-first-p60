/*===========================================================================

                                  R R C S E N D

GENERAL DESCRIPTION
 The RRCSEND module consists of details of SEND CHAIN. This module encodes the
 OTA message using ASN1 and converts the same into DSN item. Then enqueue into
 watermark queue.

EXTERNALIZED FUNCTIONS

  tdsrrcsend_send_ul_sdu()
    This function will encode the uplink OTA messages using ASN1. Then converts
    the encoded message into DSM Item and places the DSM iterm in the watermark
    queue pointed by the Logical Channel ID.

  tdsrrcsend_send_rrc_status_message()
    This function sends an RRC Status Message. This function is typically called
    when the ASN.1 decoder fails or when there is a semantic error in a message. 

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcsend.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/08/11   mkh     Events renamed to TDS
07/07/11   mkh     Renamed log pkts to TDSxxx. 
03/10/11   yzh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
02/23/11   rl      Removed setting the buffer context as it is done in the 
                   rrcasn1_free_buf() instead
02/23/11   rl      Setting the buffer pointer before encoding ASN pdu
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "tdscdma_variation.h"
#include "tdsrrcsend.h"
#include "tdsrrclcm.h"
#include "tdsrrcasn1.h"
#include "tdsrrcasn1util.h"
#include "dsm.h"
#include "dsmutil.h"
#include "err.h"
#include "rex.h"
#include "event.h"
#include "tdsrrcllc.h"
#include "tdsrrcsmc.h"
#include "tdsrrclogging.h"
#ifdef T_WINNT
#error code not present
#endif
#include "tdsrrccu.h"
#include "tdsrrcrce.h"
#include "tdsrrcscmgr.h"

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/* This defines an invalid PDU number and used to initialize the local
   variable */
#define TDSRRCSEND_INVALID_PDU_NUM 9999
#define TDSMAC_LENGTH 32

/* First 37 bits of RRC message contains integrity check info */
#define TDSRRCSEND_INTEGRITY_CHECK_BYTES  5 

/* The following two pointers hold the latest uplink message that was encoded 
   These pointers can be used for debugging purpose in case encoding fails */
/*lint -esym(552,tdsul_dcch_msg_debug_ptr,tdsul_ccch_msg_debug_ptr) */   
tdsrrc_UL_DCCH_Message *tdsul_dcch_msg_debug_ptr=NULL;  /* Pointer to Uplink DCCH message */
tdsrrc_UL_CCCH_Message *tdsul_ccch_msg_debug_ptr=NULL;  /* Pointer to Uplink CCCH message */

/* Following two variables are used to get the ASN.1 decoding/encoding status when
there is any decoding or encoding failure */
tdsrrc_encode_info_type tdsrrc_encode_info;
tdsrrc_decode_info_type tdsrrc_decode_info;

/*===========================================================================

FUNCTION    tdsrrcsend_get_ul_message_type

DESCRIPTION
  This function determines the type of the message received and returns this
  information as a byte suitable for reporting in a QXDM event.  This value
  is *not* the same as the ASN.1 based message type values returned by other
  similar functions.  See the tdsrrclogging.h file for details.
  
DEPENDENCIES
  None

RETURN VALUE
  uint8 : Returns the message type if successful. Otherwise it returns 0xff.
  
SIDE EFFECTS
  None

===========================================================================*/
static uint8 tdsrrcsend_get_ul_message_type(
  uecomdef_logchan_e_type ul_lc,    /* logical channel type */
  tdspdu_buf_type encoded_sdu          /* encoded SDU */
)
{
  uint8 asn1_msg_type;
  uint8 msg_type = 0xff;

  switch(ul_lc)
  {
    case UE_LOGCHAN_DCCH:
      asn1_msg_type = 
        tdsrrc_get_ul_dcch_message_type(encoded_sdu.value,
                                     (uint32) encoded_sdu.length);
      switch(asn1_msg_type)
      {
        /* The following needs to be updated as and when new RRC
           procedures are implemented */
        case T_tdsrrc_UL_DCCH_MessageType_activeSetUpdateComplete:
          msg_type = TDSRRCLOG_ACTIVE_SET_UPDATE_COMPLETE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_activeSetUpdateFailure:
          msg_type = TDSRRCLOG_ACTIVE_SET_UPDATE_FAILURE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_cellChangeOrderFromUTRANFailure:
          msg_type = TDSRRCLOG_CELL_CHANGE_ORDER_FROM_UTRAN_FAILURE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_counterCheckResponse:
          msg_type = TDSRRCLOG_COUNTER_CHECK_RESPONSE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_handoverToUTRANComplete:
          msg_type = TDSRRCLOG_HANDOVER_TO_UTRAN_COMPLETE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_initialDirectTransfer:
          msg_type = TDSRRCLOG_INITIAL_DIRECT_TRANSFER_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_handoverFromUTRANFailure:
          msg_type = TDSRRCLOG_HANDOVER_FROM_UTRAN_FAILURE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_measurementControlFailure:
          msg_type = TDSRRCLOG_MEASUREMENT_CONTROL_FAILURE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_measurementReport:
          msg_type = TDSRRCLOG_MEASUREMENT_REPORT_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_physicalChannelReconfigurationComplete:
          msg_type = TDSRRCLOG_PHYSICAL_CHANNEL_RECONFIGURATION_COMPLETE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_physicalChannelReconfigurationFailure:
          msg_type = TDSRRCLOG_PHYSICAL_CHANNEL_RECONFIGURATION_FAILURE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_radioBearerReconfigurationComplete:
          msg_type = TDSRRCLOG_RADIO_BEARER_RECONFIGURATION_COMPLETE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_radioBearerReconfigurationFailure:
          msg_type = TDSRRCLOG_RADIO_BEARER_RECONFIGURATION_FAILURE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_radioBearerReleaseComplete:
          msg_type = TDSRRCLOG_RADIO_BEARER_RELEASE_COMPLETE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_radioBearerReleaseFailure:
          msg_type = TDSRRCLOG_RADIO_BEARER_RELEASE_FAILURE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_radioBearerSetupComplete:
          msg_type = TDSRRCLOG_RADIO_BEARER_SETUP_COMPLETE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_radioBearerSetupFailure:
              msg_type = TDSRRCLOG_RADIO_BEARER_SETUP_FAILURE_MSG;
              break;
        case T_tdsrrc_UL_DCCH_MessageType_rrcConnectionReleaseComplete:
          msg_type = TDSRRCLOG_RRC_CONNECTION_RELEASE_COMPLETE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_rrcConnectionSetupComplete:
          msg_type = TDSRRCLOG_RRC_CONNECTION_SETUP_COMPLETE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_rrcStatus:
          msg_type = TDSRRCLOG_RRC_STATUS_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_securityModeComplete:
          msg_type = TDSRRCLOG_SECURITY_MODE_COMPLETE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_securityModeFailure:
          msg_type = TDSRRCLOG_SECURITY_MODE_FAILURE_MSG;
          break;
        case  T_tdsrrc_UL_DCCH_MessageType_signallingConnectionReleaseIndication: 
          msg_type = TDSRRCLOG_SIGNALLING_CONNECTION_RELEASE_INDICATION_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_transportChannelReconfigurationComplete:
          msg_type = TDSRRCLOG_TRANSPORT_CHANNEL_RECONFIGURATION_COMPLETE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_transportChannelReconfigurationFailure:
          msg_type = TDSRRCLOG_TRANSPORT_CHANNEL_RECONFIGURATION_FAILURE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_transportFormatCombinationControlFailure:
          msg_type = TDSRRCLOG_TRANSPORT_FORMAT_COMBINATION_CONTROL_FAILURE_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_ueCapabilityInformation:
          msg_type = TDSRRCLOG_UE_CAPABILITY_INFORMATION_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_uplinkDirectTransfer:
          msg_type = TDSRRCLOG_UPLINK_DIRECT_TRANSFER_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_utranMobilityInformationConfirm:
          msg_type = TDSRRCLOG_UTRAN_MOBILITY_INFORMATION_CONFIRM_MSG;
          break;
        case T_tdsrrc_UL_DCCH_MessageType_utranMobilityInformationFailure:
          msg_type = TDSRRCLOG_UTRAN_MOBILITY_INFORMATION_FAILURE_MSG;
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"The msg type %d is not supported",msg_type );
          break;
      }  
      break;

    case UE_LOGCHAN_CCCH:
      asn1_msg_type = 
        tdsrrc_get_ul_ccch_message_type(encoded_sdu.value,
                                     (uint32) encoded_sdu.length);
      switch(asn1_msg_type)
      {
        /* The following needs to be updated as and when new RRC
           procedures are implemented */
        case T_tdsrrc_UL_CCCH_MessageType_cellUpdate:
          msg_type = TDSRRCLOG_CELL_UPDATE_MSG;
          break;
        case T_tdsrrc_UL_CCCH_MessageType_rrcConnectionRequest:
          msg_type = TDSRRCLOG_RRC_CONNECTION_REQUEST_MSG;
          break;
        case T_tdsrrc_UL_CCCH_MessageType_uraUpdate:
          msg_type = TDSRRCLOG_URA_UPDATE_MSG;
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"The msg type %d is not supported",msg_type);
          break;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"The SDU type %d is not supported",ul_lc);
      break;
  }

  return msg_type;
} /* end function tdsrrcsend_get_ul_message_type */
#ifdef FEATURE_TDSCDMA_ULDL_LOG_SUPPORT
/*===========================================================================

FUNCTION    tdsrrcsend_get_ul_asn1_message_type

DESCRIPTION
  This function determines the type of the message received and returns this
  information as a byte suitable for reporting in a QXDM event.  This value
  is *not* the same as the ASN.1 based message type values returned by other
  similar functions.  See the rrclogging.h file for details.
  
DEPENDENCIES
  None

RETURN VALUE
  uint8 : Returns the message type if successful. Otherwise it returns 0xff.
  
SIDE EFFECTS
  None

===========================================================================*/
static uint8 tdsrrcsend_get_ul_asn1_message_type(
  uecomdef_logchan_e_type ul_lc,    /* logical channel type */
  tdspdu_buf_type encoded_sdu          /* encoded SDU */
)
{
  uint8 asn1_msg_type=1;

  switch(ul_lc)
  {
    case UE_LOGCHAN_DCCH:
      asn1_msg_type = 
        tdsrrc_get_ul_dcch_message_type(encoded_sdu.value,
                                     (uint32) encoded_sdu.length);
      
      break;

    case UE_LOGCHAN_CCCH:
      asn1_msg_type = 
        tdsrrc_get_ul_ccch_message_type(encoded_sdu.value,
                                     (uint32) encoded_sdu.length);
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"The SDU type %d is not supported",ul_lc);
      break;
  }

  return (asn1_msg_type-1);
} /* end function rrc_get_ul_asn1_message_type */
#endif
/*===========================================================================
FUNCTION tdsrrcsend_log_ul_sig_message

DESCRIPTION
  This function will log the uplink encoded OTA messages into QXDM.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcsend_log_ul_sig_message
(   
  tdspdu_buf_type encoded_sdu,                /* encoded SDU */
  rlc_lc_id_type   lc_id,                   /* Logical Channel Id */
  void *msg_ptr
)
{
  size_t msg_size;
#ifdef T_WINNT
#error code not present
#else /* QXDM logging */
  uint32 log_record_length;                 /* Local variable to store Log 
                                               Record Length */
  tdscdma_rrc_signalling_message_log_packet_type *log_record_ptr;
                                            /* Local variable to store Log
                                               record */
  uecomdef_logchan_e_type rrclog_ul_lc;     /* Variable required for logging*/
  tdsrrclog_sig_msg_event_type sig_msg_event;  /* Variable for message rx event */
  uint8 rb_id;                              /* Variable for radio bearer ID */
  uint8 channel_type;                       /* Variable for channel type */
#ifdef FEATURE_TDSCDMA_ULDL_LOG_SUPPORT
  uint8 asn1_msg_type;
#endif /* FEATURE_TDSCDMA_ULDL_LOG_SUPPORT */

#ifdef FEATURE_QSH_EVENT_METRIC
  uint8 msg_choice;
  tdsrrcqsh_ota_pdu_type_e sdu_type = TDSRRC_QSH_SDU_TYPE_NONE;
#endif

#endif /* T_WINNT */ 

#ifdef T_WINNT
  #error code not present
#else /* QXDM logging */

  /*Get the Logical Channel Type from LCM */
  (void) tdsrrclcm_get_ul_lc_type(lc_id, &rrclog_ul_lc);

  /* Determine and store RB and channel type info */
  rb_id = (uint8)tdsrrclcm_get_ul_rb_id(lc_id);
  switch(rrclog_ul_lc)
  {
    case UE_LOGCHAN_CCCH:         /* Common Control Channel.*/
      channel_type = TDSRRCLOG_SIG_UL_CCCH;
      break;

    case UE_LOGCHAN_DCCH:         /* Dedicated Control Channel.*/
      channel_type = TDSRRCLOG_SIG_UL_DCCH;
      break;

    default:
      channel_type = 0xFF;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Sig Msg is received on invalid Logical Channel Type %d",rrclog_ul_lc);
      break;
  }

  /* Construct and send an event. */
  sig_msg_event.channel_type = channel_type;
  sig_msg_event.message_type = tdsrrcsend_get_ul_message_type(rrclog_ul_lc,
                                                       encoded_sdu);
#ifdef FEATURE_TDSCDMA_ULDL_LOG_SUPPORT
  asn1_msg_type = tdsrrcsend_get_ul_asn1_message_type(rrclog_ul_lc,encoded_sdu);
  tdsrrc_get_ul_log_message(encoded_sdu,channel_type,asn1_msg_type,rb_id,msg_ptr,rrclog_ul_lc);
#endif
  event_report_payload(EVENT_TDSCDMA_RRC_MESSAGE_SENT, sizeof(sig_msg_event),
                       (void *)&sig_msg_event);

  /*Calculate the record length */
  log_record_length = 
    FPOS(tdscdma_rrc_signalling_message_log_packet_type, signalling_message) + 
    (uint32) encoded_sdu.length;

  /*Allocate memory for Log Record. */
  log_record_ptr = (tdscdma_rrc_signalling_message_log_packet_type *)log_alloc( TDSCDMA_RRC_SIGNALLING_MESSAGE_LOG_PACKET, log_record_length);

  /* Make sure memory allocation is successful before to proceed with 
     logging */
  if(log_record_ptr != NULL)
  {
    /* Update RB Id.*/
    log_record_ptr->rb_id = rb_id;

    /*Update Logical Channel Type */
    log_record_ptr->channel_type = channel_type;

    /* Update length of the signalling message */
    log_record_ptr->signalling_msg_length = (uint16) encoded_sdu.length;

    /* Copy the Signalling mesaage payload to the log record */
    msg_size = memscpy((void *)log_record_ptr->signalling_message, 
                       (size_t) encoded_sdu.length,
                       (const void*) encoded_sdu.value, 
                       (size_t) encoded_sdu.length);

    TDSRRC_MSG1(MSG_LEGACY_MED,"Logging TDSCDMA_RRC_SIGNALLING_MESSAGE_LOG_PACKET,msg_size = %d",msg_size);

#ifdef FEATURE_QSH_EVENT_METRIC
    sdu_type = (rrclog_ul_lc == UE_LOGCHAN_DCCH) ? TDSRRC_QSH_DCCH_UL_SDU : TDSRRC_QSH_CCCH_UL_SDU;
    if(sdu_type == TDSRRC_QSH_CCCH_UL_SDU)
    {
      msg_choice = tdsrrc_get_ul_ccch_message_type(encoded_sdu.value, 
                                        (uint32)encoded_sdu.length);
    }
    else
    {
      msg_choice = tdsrrc_get_ul_dcch_message_type(encoded_sdu.value, 
                                        (uint32)encoded_sdu.length);
    }
    tdsrrcqsh_save_ota((uint8 *)encoded_sdu.value, 
                       encoded_sdu.length, 
                       sdu_type, 
                       msg_choice);
#endif

    /*commit the log record */
    log_commit(log_record_ptr);

  }
  else
  {
    /* The QXDM is not turn ON. Hence Do not log the OTA messages */
  }

#endif /*T_WINNT */
} /* rrcsend_log_sig_message */
/*===========================================================================

FUNCTION tdsrrcsend_send_ul_sdu

DESCRIPTION
  This function will encode the uplink OTA messages using ASN1. Then converts
  the encoded message into DSM Item and places the DSM iterm in the watermark
  queue pointed by the Logical Channel ID.

DEPENDENCIES
  The l2ack_status  should not be set TDSRRCSEND_L2ACK_REQUIRED for Transparent
  and Unacknowleded mode transmission.

RETURN VALUE
  tdsrrcsend_status_e_type : If it is successful, then it returns TDSRRCSEND_SUCCESS. 
  Otherwise it returns one  of the causes for failure.

SIDE EFFECTS
  None
===========================================================================*/

tdsrrcsend_status_e_type tdsrrcsend_send_ul_sdu
(    
  tdsrrc_proc_e_type  rrc_procedure,          /* RRC procedure */
  void             *msg_ptr,               /* Pointer to the Uplink OTA 
                                              message */
  rlc_lc_id_type   lc_id,                  /* Logical Channel Id */
  tdsrrcsend_l2ack_status_e_type l2ack_status /* Indicates whether L2 Ack 
                                              required or not */
)
{  
  tdspdu_buf_type encoded_sdu;                /* encoded SDU */
  uecomdef_logchan_e_type ul_lc_ch;        /* Logical Channel Type */
  uint32 pdu_num = TDSRRCSEND_INVALID_PDU_NUM;
                                           /* PDU type a.k.a. SDU type*/
  dsm_item_type *dsm_item=NULL;                 /* Pointer to DSM Item */
  tdsrrc_UL_DCCH_Message *ul_dcch_msg=NULL;        /* Pointer to Uplink DCCH
                                              message */
  tdsrrc_UL_CCCH_Message *ul_ccch_msg=NULL;        /* Pointer to Uplink CCCH
                                              message */
  tdsrrclcm_mui_type mui;                     /* Local variable to store 
                                              MUI */
  tdsrrc_RB_Identity rb_id;                   /* Local variable to store
                                              RB Id. */  
  uint32 rrc_tm_sdu_size_bits;             /* The SDU size in bits for
                                              tranaparent mode */
  uint32 rrc_tm_sdu_size_bytes;            /* The SDU size in bytes for
                                              tranaparent mode */
  uint8 *rrc_tm_sdu_ptr;                   /* The pointer to the SDU
                                              in Transparent */
  uint32 index;                            /* Local parameter for 
                                              indexing. */
#ifdef  FEATURE_TDSCDMA_SRB_RLC_SDU_RESTORE
  uint8 ul_msg_type=0;
  #define SDU_REL_COMP_BIT 0x10000000
  #define SDU_REL_COMPL_MASK  0xEFFFFFFF
#endif 
                                              

  tdsrrcsend_status_e_type status = TDSRRCSEND_SUCCESS;

  /*initialize the global debug pointers before any assignment */
  tdsul_dcch_msg_debug_ptr = NULL;
  tdsul_ccch_msg_debug_ptr = NULL;

  /* Gets the logical Channel Type and identifies the PDU number */
  if(TDSRRCLCM_SUCCESS == tdsrrclcm_get_ul_lc_type(lc_id, &ul_lc_ch) )
  {
    switch (ul_lc_ch)
    {
      case UE_LOGCHAN_CCCH:
        pdu_num = tdsrrc_UL_CCCH_Message_PDU;
        ul_ccch_msg = (tdsrrc_UL_CCCH_Message *) msg_ptr;
        tdsul_ccch_msg_debug_ptr = (tdsrrc_UL_CCCH_Message *) msg_ptr;

        /* Check if Integrity Protection started */
        if(TDSRRCSMC_IP_NOT_STARTED == tdsrrcsmc_check_integrity_status() ||
           ul_ccch_msg->message.t == T_tdsrrc_UL_CCCH_MessageType_rrcConnectionRequest )
        {
          TDSRRC_RESET_MSG_IE_PRESENT_PTR(ul_ccch_msg);     /*  Integrity Check is disabled */
        }
        else
        {
          TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(ul_ccch_msg,        /*  Integrity Check is enabled */
            tdsrrc_UL_CCCH_Message,integrityCheckInfo); 

          /* Set the MAC value to Radio Bearer Id  0 for CCCH Message*/
          ul_ccch_msg->integrityCheckInfo.messageAuthenticationCode.numbits = TDSMAC_LENGTH;
          *(ul_ccch_msg->integrityCheckInfo.messageAuthenticationCode.data) = 0;
          *(ul_ccch_msg->integrityCheckInfo.messageAuthenticationCode.data + 1) = 0;
          *(ul_ccch_msg->integrityCheckInfo.messageAuthenticationCode.data + 2) = 0;
          *(ul_ccch_msg->integrityCheckInfo.messageAuthenticationCode.data + 3) = 0;
          
          /* Set the RRC Sequence Number to 0 */
          ul_ccch_msg->integrityCheckInfo.rrc_MessageSequenceNumber = 0;
                                        
        }
        break;
      case UE_LOGCHAN_DCCH:
        pdu_num = tdsrrc_UL_DCCH_Message_PDU;
        ul_dcch_msg = (tdsrrc_UL_DCCH_Message *) msg_ptr;
        tdsul_dcch_msg_debug_ptr = (tdsrrc_UL_DCCH_Message *) msg_ptr;
        
        #ifdef FEATURE_TDSCDMA_RCR_CANCEL_RECFG
        if ((((tdsrrc_UL_DCCH_Message *) msg_ptr)->message.t != T_tdsrrc_UL_DCCH_MessageType_rrcConnectionReleaseComplete) 
            &&(TRUE == tdsrrcrcr_is_proc_active()))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"In RCR active state,no need to send redundant message.");
          status = TDSRRCSEND_LOGICAL_CHANNEL_NOT_SUPPORTED;
          break;
        }
        #endif
        
        /* Check if Integrity Protection started */
        if(TDSRRCSMC_IP_NOT_STARTED == tdsrrcsmc_check_integrity_status() )
        {
          TDSRRC_RESET_MSG_IE_PRESENT_PTR(ul_dcch_msg);   /*  Integrity Check is disabled */
        }
        else
        {
         /*lint -e570 loss of sign is ok here since RHS operand is constant > 0 */
          TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(ul_dcch_msg,       /*  Integrity Check is enabled */
             tdsrrc_UL_DCCH_Message,integrityCheckInfo); 
          /*lint +e570 end we're ok with the loss of sign */

          /* Set the MAC value to Radio Bearer Id */
          ul_dcch_msg->integrityCheckInfo.messageAuthenticationCode.numbits= 
            TDSMAC_LENGTH;
         
          *(ul_dcch_msg->integrityCheckInfo.messageAuthenticationCode.data) = 0;
          *(ul_dcch_msg->integrityCheckInfo.messageAuthenticationCode.data + 1) = 0;
          *(ul_dcch_msg->integrityCheckInfo.messageAuthenticationCode.data + 2) = 0;
          *(ul_dcch_msg->integrityCheckInfo.messageAuthenticationCode.data + 3) = 
            (uint8)tdsrrclcm_get_ul_rb_id(lc_id);
          
          /* Set the RRC Sequence Number to 0 */
          ul_dcch_msg->integrityCheckInfo.rrc_MessageSequenceNumber = 0;  
        }                
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERROR:Invalid Logical Channel Type: %d",ul_lc_ch);
        status = TDSRRCSEND_LOGICAL_CHANNEL_NOT_SUPPORTED;
        break;
    }
    if(status != TDSRRCSEND_SUCCESS)
    {
      /* The memory freeing for msg_ptr should be done here even though SEND
         CHAIN fails. This function should free memory allocated for msg_ptr and as 
         well as linked lists in the message if they are any allocated.
      */
      if(tdsrrcasn1_free_pdu((int )pdu_num,msg_ptr)) 
      {
        /* This is a memory leak and it should never happen. */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful");
      }
      return(status);
    }
  }
  else
  {

    /* Here we do not know the PDU Number of the unencoded message. Hence we can
       not de-allocate memory. Normally this should never occur since validity of
       the RLC Id is checked by the procedure before calling SEND CHAIN. */

    ERR_FATAL("Logical channel is not setup %d",lc_id,0,0);
  }
  /* Encodes the OTA message using ASN1. The memory for msg_ptr is allocated
     by the RRC procedure and the memory for encoded_sdu will be allocated
     by the following function */
  encoded_sdu = tdsrrcasn1_encode_pdu(msg_ptr, (int) pdu_num);

  if(encoded_sdu.value !=NULL)              /* Checks here whether encoding is
                                               successful */
  {
    rb_id = tdsrrclcm_get_ul_rb_id (lc_id);  /* Gets the RB Id from RLC LC Id. */

    if (rb_id == CCCH_RADIO_BEARER_ID)
    {
      /* Gets the lowest Transport Format from the LLC to enqueue TM SDU */
      rrc_tm_sdu_size_bits = tdsrrcllc_calculate_ul_tm_ccch_rlc_size();
     
      if ((rrc_tm_sdu_size_bits != TDSRRCLLC_INVALID_RLC_SIZE) &&((uint32)(encoded_sdu.length * 8)<rrc_tm_sdu_size_bits))
      {
        /* Convert the received size in bits to the size in bytes */
        rrc_tm_sdu_size_bytes = rrc_tm_sdu_size_bits / 8;

        rrc_tm_sdu_size_bytes += (rrc_tm_sdu_size_bits % 8) ? 1: 0;
        /* Allocate memory for the size in bytes */
        rrc_tm_sdu_ptr =  (uint8 *) tdsrrc_malloc (rrc_tm_sdu_size_bytes); 

        if (rrc_tm_sdu_ptr == NULL)
        {
          ERR_FATAL("Failed to allocate memory for TM SDU",0,0,0);
        }
      
        /* Copy the encoded byte stream */
        for(index = 0; index < (uint32) encoded_sdu.length; index++)
        {
          *(rrc_tm_sdu_ptr + index) = *(encoded_sdu.value + index);
        }
        /* Pad the addional bytes with zeros */
        for(index = (uint32) encoded_sdu.length; index < rrc_tm_sdu_size_bytes;
            index++)
        {
          *(rrc_tm_sdu_ptr + index) = 0;
        }

        if( ul_ccch_msg != NULL )
        {
          /* Update Integrity Check Info  if Integrity Protection started */
          if(TDSRRCSMC_IP_STARTED == tdsrrcsmc_check_integrity_status() && 
             (ul_ccch_msg->message.t != T_tdsrrc_UL_CCCH_MessageType_rrcConnectionRequest))
          {
            if((tdsrrcsmc_append_uplink_integrity_check_info( rrc_tm_sdu_ptr,
               rrc_tm_sdu_size_bits, 0)) == FAILURE )
            {
              ERR_FATAL("Integrity append failed",0,0,0);
            }

            /* Fill 'encoded_sdu' with integrity check info if integrity information is added, 
                This is done for appropriate logging of CCCH messages.
                First 37 bits of RRC message contains integrity check info, hence copying first
                five bytes of RRC message.
             */
            (void) memscpy(encoded_sdu.value, 
                           TDSRRCSEND_INTEGRITY_CHECK_BYTES,
                           rrc_tm_sdu_ptr, 
                           TDSRRCSEND_INTEGRITY_CHECK_BYTES);
          }
        }
        dsm_item = dsm_sdu_to_pkt_chain_bit(rrc_tm_sdu_ptr, 
                                           (unsigned short) rrc_tm_sdu_size_bits );
                                             /* Converts the encoded SDU into 
                                                DSM item */
        /* Free the allocated for the TM SDU */  
        if (rrc_tm_sdu_ptr != NULL)
        {
          tdsrrc_free( rrc_tm_sdu_ptr );
        }
      }
      else if(rrc_tm_sdu_size_bits != TDSRRCLLC_INVALID_RLC_SIZE)
      {
        if( ul_ccch_msg != NULL )
        {
          if(ul_ccch_msg->message.t == T_tdsrrc_UL_CCCH_MessageType_cellUpdate )
          {
            /*Special treatment for CU message here. Need to trim this 
             message & see if we can construct a CU OTA message which 
             will fit UL TM TB size*/
    
            /*Variable to keep track of whether CU message trimming is done. 
              This will be set to TRUE when CU message can fit in UL RLC TM TB 
              size or no more trimming possible */
            boolean is_trim_cu_done = FALSE; 
            tdsrrccu_reset_trim_bit_mask();
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"CU size big so kick in CU trimming opt....");
            do
            {
              /*Start CU trimming here*/
              if(tdsrrccu_trim_cu_msg_ul_tm_tb_size(msg_ptr,(uint32)encoded_sdu.length) == TRUE)
              {
                /*Lets discard this encoded PDU*/
                tdsrrcasn1_free_buf(encoded_sdu.value);
                /*Lets re-encoded this PDU*/
                encoded_sdu = tdsrrcasn1_encode_pdu(msg_ptr, (int) pdu_num);

                if(encoded_sdu.value !=NULL)
                {
                  /* Gets the lowest Transport Format from the LLC to enqueue TM SDU */
                  rrc_tm_sdu_size_bits = tdsrrcllc_calculate_ul_tm_ccch_rlc_size();
                  /*Check whether CU OTA message can be sent on UL*/
                  if ((uint32)(encoded_sdu.length * 8)<rrc_tm_sdu_size_bits)
                  {
                    /* Convert the received size in bits to the size in bytes */
                    rrc_tm_sdu_size_bytes = rrc_tm_sdu_size_bits / 8;
            
                    rrc_tm_sdu_size_bytes += (rrc_tm_sdu_size_bits % 8) ? 1: 0;
                   
                    /* Allocate memory for the size in bytes */
                    rrc_tm_sdu_ptr =  (uint8 *) tdsrrc_malloc (rrc_tm_sdu_size_bytes); 
            
                    if (rrc_tm_sdu_ptr == NULL)
                    {
                      ERR_FATAL("Failed to allocate memory for TM SDU",0,0,0);
                    }
                  
                    /* Copy the encoded byte stream */
                    for(index = 0; index < (uint32) encoded_sdu.length; index++)
                    {
                      *(rrc_tm_sdu_ptr + index) = *(encoded_sdu.value + index);
                    }
                    /* Pad the addional bytes with zeros */
                    for(index = (uint32) encoded_sdu.length; index < rrc_tm_sdu_size_bytes;
                        index++)
                    {
                      *(rrc_tm_sdu_ptr + index) = 0;
                    }
            
                    if( ul_ccch_msg != NULL )
                    {
                      /* Update Integrity Check Info  if Integrity Protection started */
                      if(TDSRRCSMC_IP_STARTED == tdsrrcsmc_check_integrity_status() && 
                         (ul_ccch_msg->message.t != T_tdsrrc_UL_CCCH_MessageType_rrcConnectionRequest))
                      {
                        if((tdsrrcsmc_append_uplink_integrity_check_info( rrc_tm_sdu_ptr,
                           rrc_tm_sdu_size_bits, 0)) == FAILURE )
                        {
                          ERR_FATAL("Integrity append failed",0,0,0);
                        }
            
                        /* Fill 'encoded_sdu' with integrity check info if integrity information is added, 
                            This is done for appropriate logging of CCCH messages.
                            First 37 bits of RRC message contains integrity check info, hence copying first
                            five bytes of RRC message.
                         */
                        (void) memscpy(encoded_sdu.value, 
                                       TDSRRCSEND_INTEGRITY_CHECK_BYTES,
                                       rrc_tm_sdu_ptr, 
                                       TDSRRCSEND_INTEGRITY_CHECK_BYTES);
                      }
                    }
                    dsm_item = dsm_sdu_to_pkt_chain_bit(rrc_tm_sdu_ptr, 
                                                       (unsigned short) rrc_tm_sdu_size_bits );
                                                         /* Converts the encoded SDU into 
                                                            DSM item */
                    /* Free the allocated for the TM SDU */  
                    if (rrc_tm_sdu_ptr != NULL)
                    {
                      tdsrrc_free( rrc_tm_sdu_ptr );
                    }
                    is_trim_cu_done = TRUE;
                    tdsrrccu_reset_trim_bit_mask();
                  }
                }
                else
                {/*ASN.1 encoding failed*/
                  status = TDSRRCSEND_FAILED_TO_ENCODE;
                  is_trim_cu_done = TRUE;
                  tdsrrccu_reset_trim_bit_mask();
                  if (tdsrrc_encode_info.encode_status == TDSASN1_OUT_MEMORY)
                  {
                    ERR_FATAL("No memory for encoding %d %d",tdsrrc_encode_info.encode_status,tdsrrc_encode_info.pdu_num,0);
                  }
                  else if (tdsrrc_encode_info.encode_status == TDSASN1_CONSTRAINT_VIOLATED)
                  {
                    ERR_FATAL("Constraint violation in ASN1 encoding %d %d",tdsrrc_encode_info.encode_status,tdsrrc_encode_info.pdu_num,0);
                  }
                  else
                  {
                    ERR_FATAL("ASN1 encoding failed %d %d",tdsrrc_encode_info.encode_status,tdsrrc_encode_info.pdu_num,0);
                  }
                }
              }
              else
              {
                /* Set the status to invalid RLC Size. This implies that RRC LLC does not
                 have a suitable Tranport Format for the encoded SDU */
                TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid RLC Size is revd: %d,encoded_sdu.length = %d",rrc_tm_sdu_size_bits,(encoded_sdu.length * 8));
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Need to add more cases in CU trim opt...");
                status = TDSRRCSEND_INVALID_TM_RLC_SIZE_RECEIVED;
                is_trim_cu_done = TRUE;
                tdsrrccu_reset_trim_bit_mask();
              }
            }while(is_trim_cu_done == FALSE);
          }
          else if (ul_ccch_msg->message.t == T_tdsrrc_UL_CCCH_MessageType_rrcConnectionRequest )
          {
            /*Special treatment for rrc connection request message here. Need to trim this 
             message & see if we can construct a rrc connection request OTA message which 
             will fit UL TM TB size*/
    
            /*Variable to keep track of whether rrc connection request message trimming is done. 
              This will be set to TRUE when rrc connection request message can fit in UL RLC TM TB 
              size or no more trimming possible */
            boolean is_trim_rce_done = FALSE; 
            tdsrrcrce_reset_trim_bit_mask();
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCE size big so kick in RCE trimming opt....");
            do
            {
              /*Start rrc connection request trimming here*/
              if(tdsrrcrce_trim_rce_msg_ul_tm_tb_size(msg_ptr,(uint32)encoded_sdu.length) == TRUE)
              {
                /*Lets discard this encoded PDU*/
                tdsrrcasn1_free_buf(encoded_sdu.value);
                /*Lets re-encoded this PDU*/
                encoded_sdu = tdsrrcasn1_encode_pdu(msg_ptr, (int) pdu_num);

                if(encoded_sdu.value !=NULL)
                {
                  /* Gets the lowest Transport Format from the LLC to enqueue TM SDU */
                  rrc_tm_sdu_size_bits = tdsrrcllc_calculate_ul_tm_ccch_rlc_size();
                  /*Check whether rrc connection request OTA message can be sent on UL*/
                  if ((uint32)(encoded_sdu.length * 8)<rrc_tm_sdu_size_bits)
                  {
                    /* Convert the received size in bits to the size in bytes */
                    rrc_tm_sdu_size_bytes = rrc_tm_sdu_size_bits / 8;
            
                    rrc_tm_sdu_size_bytes += (rrc_tm_sdu_size_bits % 8) ? 1: 0;
                   
                    /* Allocate memory for the size in bytes */
                    rrc_tm_sdu_ptr =  (uint8 *) tdsrrc_malloc (rrc_tm_sdu_size_bytes); 
            
                    if (rrc_tm_sdu_ptr == NULL)
                    {
                      ERR_FATAL("Failed to allocate memory for TM SDU",0,0,0);
                    }
                  
                    /* Copy the encoded byte stream */
                    for(index = 0; index < (uint32) encoded_sdu.length; index++)
                    {
                      *(rrc_tm_sdu_ptr + index) = *(encoded_sdu.value + index);
                    }
                    /* Pad the addional bytes with zeros */
                    for(index = (uint32) encoded_sdu.length; index < rrc_tm_sdu_size_bytes;
                        index++)
                    {
                      *(rrc_tm_sdu_ptr + index) = 0;
                    }
            
                    if( ul_ccch_msg != NULL )
                    {
                      /* Update Integrity Check Info  if Integrity Protection started */
                      if(TDSRRCSMC_IP_STARTED == tdsrrcsmc_check_integrity_status() && 
                         (ul_ccch_msg->message.t != T_tdsrrc_UL_CCCH_MessageType_rrcConnectionRequest))
                      {
                        if((tdsrrcsmc_append_uplink_integrity_check_info( rrc_tm_sdu_ptr,
                           rrc_tm_sdu_size_bits, 0)) == FAILURE )
                        {
                          ERR_FATAL("Integrity append failed",0,0,0);
                        }
            
                        /* Fill 'encoded_sdu' with integrity check info if integrity information is added, 
                            This is done for appropriate logging of CCCH messages.
                            First 37 bits of RRC message contains integrity check info, hence copying first
                            five bytes of RRC message.
                         */
                        (void) memscpy(encoded_sdu.value, 
                                       TDSRRCSEND_INTEGRITY_CHECK_BYTES,
                                       rrc_tm_sdu_ptr, 
                                       TDSRRCSEND_INTEGRITY_CHECK_BYTES);
                      }
                    }
                    dsm_item = dsm_sdu_to_pkt_chain_bit(rrc_tm_sdu_ptr, 
                                                       (unsigned short) rrc_tm_sdu_size_bits );
                                                         /* Converts the encoded SDU into 
                                                            DSM item */
                    /* Free the allocated for the TM SDU */  
                    if (rrc_tm_sdu_ptr != NULL)
                    {
                      tdsrrc_free( rrc_tm_sdu_ptr );
                    }
                    is_trim_rce_done = TRUE;
                    tdsrrcrce_reset_trim_bit_mask();
                  }
                }
                else
                {/*ASN.1 encoding failed*/
                  status = TDSRRCSEND_FAILED_TO_ENCODE;
                  is_trim_rce_done = TRUE;
                  tdsrrcrce_reset_trim_bit_mask();
                  if (tdsrrc_encode_info.encode_status == TDSASN1_OUT_MEMORY)
                  {
                    ERR_FATAL("No memory for encoding %d %d",tdsrrc_encode_info.encode_status,tdsrrc_encode_info.pdu_num,0);
                  }
                  else if (tdsrrc_encode_info.encode_status == TDSASN1_CONSTRAINT_VIOLATED)
                  {
                    ERR_FATAL("Constraint violation in ASN1 encoding %d %d",tdsrrc_encode_info.encode_status,tdsrrc_encode_info.pdu_num,0);
                  }
                  else
                  {
                    ERR_FATAL("ASN1 encoding failed %d %d",tdsrrc_encode_info.encode_status,tdsrrc_encode_info.pdu_num,0);
                  }
                }
              }
              else
              {
                /* Set the status to invalid RLC Size. This implies that RRC LLC does not
                 have a suitable Tranport Format for the encoded SDU */
                TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid RLC Size is revd: %d,encoded_sdu.length = %d",rrc_tm_sdu_size_bits,(encoded_sdu.length * 8));
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Need to add more cases in CU trim opt...");
                status = TDSRRCSEND_INVALID_TM_RLC_SIZE_RECEIVED;
                is_trim_rce_done = TRUE;
                tdsrrcrce_reset_trim_bit_mask();
              }
            }while(is_trim_rce_done == FALSE);
          }
          else
          {
            /* Set the status to invalid RLC Size. This implies that RRC LLC does not
               have a suitable Tranport Format for the encoded SDU */
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RLC Size is revd: %d",rrc_tm_sdu_size_bits);
            status = TDSRRCSEND_INVALID_TM_RLC_SIZE_RECEIVED;
          }
        }
        else
        {
          /* Set the status to invalid RLC Size. This implies that RRC LLC does not
             have a suitable Tranport Format for the encoded SDU */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RLC Size is revd: %d",rrc_tm_sdu_size_bits);
          status = TDSRRCSEND_INVALID_TM_RLC_SIZE_RECEIVED;
        }
      }
      else        
      {
        /* Set the status to invalid RLC Size. This implies that RRC LLC does not
           have a suitable Tranport Format for the encoded SDU */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RLC Size is revd: %d",rrc_tm_sdu_size_bits);
        status = TDSRRCSEND_INVALID_TM_RLC_SIZE_RECEIVED;
      }
    }
    else
    {
      /* Update Integrity Check Info  if Integrity Protection started */
        if(TDSRRCSMC_IP_STARTED == tdsrrcsmc_check_integrity_status() )
        {
           if(tdsrrcsmc_append_uplink_integrity_check_info( 
             (byte *) encoded_sdu.value, 
             (uint32) encoded_sdu.length * 8,
             tdsrrclcm_get_ul_rb_id(lc_id)))
           {
             ERR_FATAL("Integrity append failed",0,0,0);
           }
        }   
      /* tdsrrcsmc_append_uplink_integrity_check_info */
      dsm_item = dsm_sdu_to_pkt_chain_byte(encoded_sdu.value, 
                                         (unsigned short)encoded_sdu.length);
                                           /* Converts the encoded SDU into 
                                              DSM item */       
    }   
    /* log the encoded uplink signalling message */
    tdsrrcsend_log_ul_sig_message(encoded_sdu, lc_id, msg_ptr); 

    /* memory freeing for encoded_sdu should be done here using OSS compiler
       functions.  It doesn't matter whether SDU is converted successfully 
       into DSM item or not */
    
    tdsrrcasn1_free_buf(encoded_sdu.value);
  
    if(status != TDSRRCSEND_SUCCESS)
    {
      if(tdsrrcasn1_free_pdu((int )pdu_num,msg_ptr)) 
      {
        /* This is a memory leak and it should never happen.*/
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful");
      }
      return(status);
    }

    if(dsm_item != NULL)                   /* If conversion is successful, then
                                              enqueues the same into the 
                                              watermark queue */
    {      
      /* Switch on Radio Bearer in order to know the mode of transmission */
   
      switch(rb_id)
      {
        case CCCH_RADIO_BEARER_ID:
        case DCCH_UM_RADIO_BEARER_ID:
          if(l2ack_status == TDSRRCSEND_L2ACK_REQUIRED)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Incomptible L2 Req from the proc : %d",rrc_procedure);
            status = TDSRRCSEND_INCOMPATIBLE_L2ACK_REQUEST;
          }
          break;

        case DCCH_AM_RADIO_BEARER_ID:
        case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
        case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
#ifdef FEATURE_TDSCDMA_SRB_RLC_SDU_RESTORE 
          ul_msg_type = (uint8)(((tdsrrc_UL_DCCH_Message *) msg_ptr)->message.t);
          if (T_tdsrrc_UL_DCCH_MessageType_rrcConnectionReleaseComplete == ul_msg_type )
          {
            if (dsm_item != NULL)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"FEATURE_TDSCDMA_SRB_RLC_SDU_RESTORE:No need resotre RRC Connection release complete ");
              dsm_item->app_field &= SDU_REL_COMPL_MASK;
              dsm_item->app_field |= (uint32)SDU_REL_COMP_BIT;
            }         
          }
#endif
          if(l2ack_status == TDSRRCSEND_L2ACK_REQUIRED)
          {
            dsm_set_sdu_cnf (dsm_item, TRUE); /* Informs RLC L2 ACK for this
                                                 SDU is required */
            /* Get MUI from the Logical Channel Manager */
            if(TDSRRCLCM_SUCCESS == tdsrrclcm_get_mui(rrc_procedure,
                                                rb_id, TRUE, &mui))
            {
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
              #error code not present
#endif // TEST_FRAMEWORK && TF_UNIT_TEST
              dsm_write_sdu_mui(dsm_item,mui);
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"The MUI: %d for RB Id: %d",mui,rb_id);
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"The LCM is unable to allocate MUI");
            }
          }
          else
          {
            dsm_set_sdu_cnf (dsm_item, TRUE); /* Informs RLC L2 ACK for this
                                                 SDU is required.This is done
                                                 for debugging purposes.The L2 Ack
                                                 will get dropped in the dispatcher */
            /* Get MUI from the Logical Channel Manager */
            if(TDSRRCLCM_SUCCESS == tdsrrclcm_get_mui(rrc_procedure,
                                                rb_id, FALSE, &mui))
            {
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
              #error code not present
#endif // TEST_FRAMEWORK && TF_UNIT_TEST
              dsm_write_sdu_mui(dsm_item,mui);
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"The MUI: %d for RB Id: %d",mui,rb_id);
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"The LCM is unable to allocate MUI");
            }
          }
          
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Radio Bearer ID: %d",rb_id);
          status = TDSRRCSEND_INVALID_RB_ID;
          break;
      }        
      if(status == TDSRRCSEND_SUCCESS)
      {
        dsm_enqueue(tdsrrclcm_get_ul_watermark_ptr(lc_id), &dsm_item);
      } 
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failed to enqueue UL SDU");
      }     
    }
    else /* Failed convert the SDU into DSM Items */
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to convert SDU to DSM Item");
      status = TDSRRCSEND_FAILED_CONVERT_SDU_TO_PACKECT;
    } 
  }
  else /* ASN1 Encoding is failed */
  {
    status = TDSRRCSEND_FAILED_TO_ENCODE;
    if (tdsrrc_encode_info.encode_status == TDSASN1_OUT_MEMORY)
    {
      ERR_FATAL("No memory for encoding %d %d",tdsrrc_encode_info.encode_status,tdsrrc_encode_info.pdu_num,0);
    }
    else if (tdsrrc_encode_info.encode_status == TDSASN1_CONSTRAINT_VIOLATED)
    {
      ERR_FATAL("Constraint violation in ASN1 encoding %d %d",tdsrrc_encode_info.encode_status,tdsrrc_encode_info.pdu_num,0);
    }
    else
    {
      ERR_FATAL("ASN1 encoding failed %d %d",tdsrrc_encode_info.encode_status,tdsrrc_encode_info.pdu_num,0);
    }
  }

  /* The memory freeing for msg_ptr should be done here. using OSS compiler
     functions. This function should free memory allocated for msg_ptr and as 
     well as linked lists in the message if they are any allocated.*/
  if(tdsrrcasn1_free_pdu((int )pdu_num,msg_ptr)) 
  {
    /* This is a memory leak and it should never happen.*/
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful");
  }
  
  return (status);
}

/*===========================================================================

FUNCTION tdsrrcsend_send_ul_sdu_with_mui

DESCRIPTION
  This function will encode the uplink OTA messages using ASN1. Then converts
  the encoded message into DSM Item and places the DSM iterm in the watermark
  queue pointed by the Logical Channel ID.

DEPENDENCIES
  The l2ack_status  should not be set TDSRRCSEND_L2ACK_REQUIRED for Transparent
  and Unacknowleded mode transmission.

RETURN VALUE
  tdsrrcsend_status_e_type : If it is successful, then it returns TDSRRCSEND_SUCCESS. 
  Otherwise it returns one  of the causes for failure.

SIDE EFFECTS
  None
===========================================================================*/

tdsrrcsend_status_e_type tdsrrcsend_send_ul_sdu_with_mui
(    
  tdsrrc_proc_e_type  rrc_procedure,          /* RRC procedure */
  void             *msg_ptr,               /* Pointer to the Uplink OTA 
                                              message */
  rlc_lc_id_type   lc_id,                  /* Logical Channel Id */
  tdsrrcsend_l2ack_status_e_type l2ack_status,
                                           /* Indicates whether L2 Ack 
                                              required or not */
  uint8 *msg_mui                           /* MUI Tagged to the message
                                             transmitted */
)
{  
  tdspdu_buf_type encoded_sdu;                /* encoded SDU */
  uecomdef_logchan_e_type ul_lc_ch;        /* Logical Channel Type */
  uint32 pdu_num = TDSRRCSEND_INVALID_PDU_NUM;
                                           /* PDU type a.k.a. SDU type*/
  dsm_item_type *dsm_item=NULL;                 /* Pointer to DSM Item */
  tdsrrc_UL_DCCH_Message *ul_dcch_msg;        /* Pointer to Uplink DCCH
                                              message */
  tdsrrc_UL_CCCH_Message *ul_ccch_msg;        /* Pointer to Uplink CCCH
                                              message */
  tdsrrclcm_mui_type mui;                     /* Local variable to store 
                                              MUI */
  tdsrrc_RB_Identity rb_id;                   /* Local variable to store
                                              RB Id. */  
  uint32 rrc_tm_sdu_size_bits;             /* The SDU size in bits for
                                              tranaparent mode */
  uint32 rrc_tm_sdu_size_bytes;            /* The SDU size in bytes for
                                              tranaparent mode */
  uint8 *rrc_tm_sdu_ptr;                   /* The pointer to the SDU
                                              in Transparent */
  uint32 index;                            /* Local parameter for 
                                              indexing. */
                                              

  tdsrrcsend_status_e_type status = TDSRRCSEND_SUCCESS;
  /*initialize the global debug pointers before any assignment */
  tdsul_dcch_msg_debug_ptr = NULL;
  tdsul_ccch_msg_debug_ptr = NULL;

  /* Gets the logical Channel Type and identifies the PDU number */
  if(TDSRRCLCM_SUCCESS == tdsrrclcm_get_ul_lc_type(lc_id, &ul_lc_ch) )
  {
    switch (ul_lc_ch)
    {
      case UE_LOGCHAN_CCCH:
        pdu_num = tdsrrc_UL_CCCH_Message_PDU;
        ul_ccch_msg = (tdsrrc_UL_CCCH_Message *) msg_ptr;
        tdsul_ccch_msg_debug_ptr = (tdsrrc_UL_CCCH_Message *) msg_ptr;

        /* Check if Integrity Protection started */
        if(TDSRRCSMC_IP_NOT_STARTED == tdsrrcsmc_check_integrity_status() ||
           ul_ccch_msg->message.t == T_tdsrrc_UL_CCCH_MessageType_rrcConnectionRequest )
        {
          TDSRRC_RESET_MSG_IE_PRESENT_PTR(ul_ccch_msg);     /*  Integrity Check is disabled */
        }
        else
        {
          /*lint -e570 we're ok with the loss of sign */
          TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(ul_ccch_msg,        /*  Integrity Check is enabled */
            tdsrrc_UL_CCCH_Message,integrityCheckInfo); 
          /*lint +e570 we're ok with the loss of sign */

          /* Set the MAC value to Radio Bearer Id  0 for CCCH Message*/
          ul_ccch_msg->integrityCheckInfo.messageAuthenticationCode.numbits= 
            TDSMAC_LENGTH;
         
          *(ul_ccch_msg->integrityCheckInfo.messageAuthenticationCode.data) = 0;
          *(ul_ccch_msg->integrityCheckInfo.messageAuthenticationCode.data + 1) = 0;
          *(ul_ccch_msg->integrityCheckInfo.messageAuthenticationCode.data + 2) = 0;
          *(ul_ccch_msg->integrityCheckInfo.messageAuthenticationCode.data + 3) = 0;
          
          /* Set the RRC Sequence Number to 0 */
          ul_ccch_msg->integrityCheckInfo.rrc_MessageSequenceNumber = 0;
                                        
        }
        break;
      case UE_LOGCHAN_DCCH:
        pdu_num = tdsrrc_UL_DCCH_Message_PDU;
        ul_dcch_msg = (tdsrrc_UL_DCCH_Message *) msg_ptr;
        tdsul_dcch_msg_debug_ptr = (tdsrrc_UL_DCCH_Message *) msg_ptr;
        
        /* Check if Integrity Protection started */
        if(TDSRRCSMC_IP_NOT_STARTED == tdsrrcsmc_check_integrity_status() )
        {
          TDSRRC_RESET_MSG_IE_PRESENT_PTR(ul_dcch_msg);   /*  Integrity Check is disabled */
        }
        else
        {
          /*lint -e570 we're ok with the loss of sign */
         TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(ul_dcch_msg,        /*  Integrity Check is enabled */
            tdsrrc_UL_DCCH_Message,integrityCheckInfo); 
          /*lint +e570 we're ok with the loss of sign */

          /* Set the MAC value to Radio Bearer Id */
          ul_dcch_msg->integrityCheckInfo.messageAuthenticationCode.numbits= 
            TDSMAC_LENGTH;
          
          *(ul_dcch_msg->integrityCheckInfo.messageAuthenticationCode.data) = 0;
          *(ul_dcch_msg->integrityCheckInfo.messageAuthenticationCode.data + 1) = 0;
          *(ul_dcch_msg->integrityCheckInfo.messageAuthenticationCode.data + 2) = 0;
          *(ul_dcch_msg->integrityCheckInfo.messageAuthenticationCode.data + 3) = 
            (uint8)tdsrrclcm_get_ul_rb_id(lc_id);
          
          /* Set the RRC Sequence Number to 0 */
          ul_dcch_msg->integrityCheckInfo.rrc_MessageSequenceNumber = 0;  
        }                
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERROR:Invalid Logical Channel Type: %d",ul_lc_ch);
        status = TDSRRCSEND_LOGICAL_CHANNEL_NOT_SUPPORTED;
        break;
    }
    if(status != TDSRRCSEND_SUCCESS)
    {
      /* The memory freeing for msg_ptr should be done here even though SEND
         CHAIN fails. This function should free memory allocated for msg_ptr and as 
         well as linked lists in the message if they are any allocated. */
      if(tdsrrcasn1_free_pdu((int )pdu_num,msg_ptr)) 
      {
        /* This is a memory leak and it should never happen.*/
        ERR_FATAL("Memory freeing is unsuccessful",0,0,0);
      }
      return(status);
    }
  }
  else
  {

    /* Here we do not know the PDU Number of the unencoded message. Hence we can
       not de-allocate memory. Normally this should never occur since validity of
       the RLC Id is checked by the procedure before calling SEND CHAIN. */

    ERR_FATAL("Logical channel is not setup %d",lc_id,0,0);
  }
  /* Encodes the OTA message using ASN1. The memory for msg_ptr is allocated
     by the RRC procedure and the memory for encoded_sdu will be allocated
     by the following function */
  encoded_sdu = tdsrrcasn1_encode_pdu(msg_ptr, (int) pdu_num);

  if(encoded_sdu.value !=NULL)              /* Checks here whether encoding is
                                               successful */
  {
    rb_id = tdsrrclcm_get_ul_rb_id (lc_id);  /* Gets the RB Id from RLC LC Id. */

    if (rb_id == CCCH_RADIO_BEARER_ID)
    {
      /* Gets the lowest Transport Format from the LLC to enqueue TM SDU */
      rrc_tm_sdu_size_bits = tdsrrcllc_calculate_ul_tm_ccch_rlc_size();
     
      if ((rrc_tm_sdu_size_bits != TDSRRCLLC_INVALID_RLC_SIZE)&&((uint32)(encoded_sdu.length * 8)<rrc_tm_sdu_size_bits))
      {
        /* Convert the received size in bits to the size in bytes */
        rrc_tm_sdu_size_bytes = rrc_tm_sdu_size_bits / 8;

        rrc_tm_sdu_size_bytes += (rrc_tm_sdu_size_bits % 8) ? 1: 0;
       
        /* Allocate memory for the size in bytes */
        rrc_tm_sdu_ptr =  (uint8 *) tdsrrc_malloc (rrc_tm_sdu_size_bytes); 

        if (rrc_tm_sdu_ptr == NULL)
        {
          ERR_FATAL("Failed to allocate memory for TM SDU",0,0,0);
        }
      
        /* Copy the encoded byte stream */
        for(index = 0; index < (uint32) encoded_sdu.length; index++)
        {
          *(rrc_tm_sdu_ptr + index) = *(encoded_sdu.value + index);
        }
        /* Pad the addional bytes with zeros */
        for(index = (uint32) encoded_sdu.length; index < rrc_tm_sdu_size_bytes;
            index++)
        {
          *(rrc_tm_sdu_ptr + index) = 0;
        }
               
        /* Update Integrity Check Info  if Integrity Protection started */
        if(TDSRRCSMC_IP_STARTED == tdsrrcsmc_check_integrity_status() )
        {
          if((tdsrrcsmc_append_uplink_integrity_check_info( rrc_tm_sdu_ptr,
              rrc_tm_sdu_size_bits, 0)) == FAILURE )
           {
             ERR_FATAL("Integrity append failed",0,0,0);
           }
        }                          
        dsm_item = dsm_sdu_to_pkt_chain_bit(rrc_tm_sdu_ptr, 
                                           (unsigned short) rrc_tm_sdu_size_bits );
                                             /* Converts the encoded SDU into 
                                                DSM item */
        /* Free the allocated for the TM SDU */  
        if (rrc_tm_sdu_ptr != NULL)
        {
          tdsrrc_free( rrc_tm_sdu_ptr );
        }
      }
      else
      {
        /* Set the status to invalid RLC Size. This implies that RRC LLC does not
           have a suitable Tranport Format for the encoded SDU */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RLC Size is revd: %d",rrc_tm_sdu_size_bits);

        status = TDSRRCSEND_INVALID_TM_RLC_SIZE_RECEIVED;
      }

    }
    else
    {
      /* Update Integrity Check Info  if Integrity Protection started */
        if(TDSRRCSMC_IP_STARTED == tdsrrcsmc_check_integrity_status() )
        {
           if(tdsrrcsmc_append_uplink_integrity_check_info( 
             (byte *) encoded_sdu.value, 
             (uint32) encoded_sdu.length * 8,
             tdsrrclcm_get_ul_rb_id(lc_id)))
           {
             ERR_FATAL("Integrity append failed",0,0,0);
           }
        }   
      /* tdsrrcsmc_append_uplink_integrity_check_info */
      dsm_item = dsm_sdu_to_pkt_chain_byte(encoded_sdu.value, 
                                         (unsigned short)encoded_sdu.length);
                                           /* Converts the encoded SDU into 
                                              DSM item */       
    }     

    /* log the encoded uplink signalling message */
    tdsrrcsend_log_ul_sig_message(encoded_sdu, lc_id, msg_ptr ); 

    /* memory freeing for encoded_sdu should be done here using OSS compiler
       functions.  It doesn't matter whether SDU is converted successfully 
       into DSM item or not */
    
    tdsrrcasn1_free_buf(encoded_sdu.value);
  
    if(status != TDSRRCSEND_SUCCESS)
    {
      if(tdsrrcasn1_free_pdu((int )pdu_num,msg_ptr)) 
      {
        /* This is a memory leak and it should never happen.*/
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful");
      }
      return(status);
    }

    if(dsm_item != NULL)                   /* If conversion is successful, then
                                              enqueues the same into the 
                                              watermark queue */
    {      
      /* Switch on Radio Bearer in order to know the mode of transmission */
   
      switch(rb_id)
      {
        case CCCH_RADIO_BEARER_ID:
        case DCCH_UM_RADIO_BEARER_ID:
          if(l2ack_status == TDSRRCSEND_L2ACK_REQUIRED)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Incomptible L2 Req from the proc : %d",rrc_procedure);
            status = TDSRRCSEND_INCOMPATIBLE_L2ACK_REQUEST;
          }
          break;

        case DCCH_AM_RADIO_BEARER_ID:
        case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
        case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
          if(l2ack_status == TDSRRCSEND_L2ACK_REQUIRED)
          {
            dsm_set_sdu_cnf (dsm_item, TRUE); /* Informs RLC L2 ACK for this
                                                 SDU is required */
            /* Get MUI from the Logical Channel Manager */
            if(TDSRRCLCM_SUCCESS == tdsrrclcm_get_mui(rrc_procedure,
                                                rb_id, TRUE, &mui))
            {
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
              #error code not present
#endif // TEST_FRAMEWORK && TF_UNIT_TEST
              dsm_write_sdu_mui(dsm_item,mui);
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"The MUI: %d for RB Id: %d",mui,rb_id);
              *msg_mui = mui;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"The LCM is unable to allocate MUI");
            }
          }
          else
          {
            dsm_set_sdu_cnf (dsm_item, FALSE); /* Informs RLC L2 ACK for this
                                                 SDU is not required */
            /* Get MUI from the Logical Channel Manager */
            if(TDSRRCLCM_SUCCESS == tdsrrclcm_get_mui(rrc_procedure,
                                                rb_id, FALSE, &mui))
            {
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
              #error code not present
#endif // TEST_FRAMEWORK && TF_UNIT_TEST
              dsm_write_sdu_mui(dsm_item,mui);
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"The MUI: %d for RB Id: %d",mui,rb_id);
              *msg_mui = mui;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"The LCM is unable to allocate MUI");
            }
          }
          
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Radio Bearer ID: %d",rb_id);
          status = TDSRRCSEND_INVALID_RB_ID;
          break;
      }        
      if(status == TDSRRCSEND_SUCCESS)
      {
        dsm_enqueue(tdsrrclcm_get_ul_watermark_ptr(lc_id), &dsm_item);
      } 
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failed to enqueue UL SDU");
      }     
    }
    else /* Failed convert the SDU into DSM Items */
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to convert SDU to DSM Item");
      status = TDSRRCSEND_FAILED_CONVERT_SDU_TO_PACKECT;
    } 
  }
  else /* ASN1 Encoding is failed */
  {
    status = TDSRRCSEND_FAILED_TO_ENCODE;

    if (tdsrrc_encode_info.encode_status == TDSASN1_OUT_MEMORY)
    {
      ERR_FATAL("No memory for encoding %d %d",tdsrrc_encode_info.encode_status,tdsrrc_encode_info.pdu_num,0);
    }
    else if (tdsrrc_encode_info.encode_status == TDSASN1_CONSTRAINT_VIOLATED)
    {
      ERR_FATAL("Constraint violation in ASN1 encoding %d %d",tdsrrc_encode_info.encode_status,tdsrrc_encode_info.pdu_num,0);
    }
    else
    {
      ERR_FATAL("ASN1 encoding failed %d %d",tdsrrc_encode_info.encode_status,tdsrrc_encode_info.pdu_num,0);
    }    
  }

  /* The memory freeing for msg_ptr should be done here. using OSS compiler
     functions. This function should free memory allocated for msg_ptr and as 
     well as linked lists in the message if they are any allocated.*/
  if(tdsrrcasn1_free_pdu((int )pdu_num,msg_ptr)) 
  {
    /* This is a memory leak and it should never happen.*/
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful");
  }
  
  return (status);
}


/*===========================================================================

FUNCTION RRCSEND_SEND_RRC_STATUS_MESSAGE

DESCRIPTION
  This function sends an RRC Status Message. This function is typically called
  when the ASN.1 decoder fails or when there is a semantic error in a message.

DEPENDENCIES
  An uplink DCCH must be set up.

RETURN VALUE
  SUCCESS if the message was successfully sent, otherwise FAILURE.

SIDE EFFECTS
  None.

===========================================================================*/

uecomdef_status_e_type tdsrrcsend_send_rrc_status_message
(
  uint32 protocol_error_choice, /* The protocol error cause to be placed in
                                   the message*/
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id,
                                /* Transaction Id received */

  tdsrrc_ReceivedMessageType received_msg_id, 
                                 /* Identification of the Recvd message.*/
  tdsrrc_proc_e_type rrc_procedure,           /* RRC procedure */
  rlc_lc_id_type   lc_id,                  /* Logical Channel Id */
  tdsrrcsend_l2ack_status_e_type l2ack_status /* Indicates whether L2 Ack 
                                              required or not */
)
{
  tdsrrc_UL_DCCH_Message *msg;     /* Pointer to the RRC Status Message */

  /* Allocate a buffer for an uplink DCCH message. This buffer is released
   * by tdsrrcsend_send_ul_sdu().
   */
  msg = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc( 
                                          sizeof (struct tdsrrc_UL_DCCH_Message));
  if (msg == NULL)
  {
    /* Error allocating memory for the uplink message. Print a message and 
     * return failure. 
     */
    ERR_FATAL("Failed to allocate memory for Proc : %d ",rrc_procedure,0,0);
  }

  /* Identify the message as an RRC Status Message */
  msg->message.t = T_tdsrrc_UL_DCCH_MessageType_rrcStatus;

  /* First set bit mask to 0 */
  TDSRRC_RESET_MSG_IE_PRESENT(msg->message.u.rrcStatus);

  /* Set the diagnostic type to Type 1 */
  msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.t = 
    T_tdsrrc_ProtocolErrorMoreInformation_diagnosticsType_type1;

  
  msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1 
   = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorMoreInformation_type1);
  /* Set the protocol error cause IE to be what was passed to this function */
  msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
    t = protocol_error_choice;

  switch(protocol_error_choice)
  {
    case T_tdsrrc_ProtocolErrorMoreInformation_type1_asn1_ViolationOrEncodingError:
    case T_tdsrrc_ProtocolErrorMoreInformation_type1_messageTypeNonexistent:
      /*Do nothing */
      break;

    case T_tdsrrc_ProtocolErrorMoreInformation_type1_messageNotCompatibleWithReceiverState:

      msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
        u.messageNotCompatibleWithReceiverState
        = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_IdentificationOfReceivedMessage);
      
      msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
        u.messageNotCompatibleWithReceiverState->rrc_TransactionIdentifier = 
        tdstransaction_id;
      
      
      msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
        u.messageNotCompatibleWithReceiverState->receivedMessageType =
        received_msg_id;
      break;

    case T_tdsrrc_ProtocolErrorMoreInformation_type1_ie_ValueNotComprehended:
        msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
        u.ie_ValueNotComprehended
        = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_IdentificationOfReceivedMessage);
         
      msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
        u.ie_ValueNotComprehended->rrc_TransactionIdentifier = 
        tdstransaction_id;

      msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
        u.ie_ValueNotComprehended->receivedMessageType =
        received_msg_id;
      break;

    case T_tdsrrc_ProtocolErrorMoreInformation_type1_conditionalInformationElementError:

      msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
        u.conditionalInformationElementError
        = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_IdentificationOfReceivedMessage);
       
      msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
        u.conditionalInformationElementError->rrc_TransactionIdentifier = 
        tdstransaction_id;

      msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
        u.conditionalInformationElementError->receivedMessageType =
        received_msg_id;
      break;

    case T_tdsrrc_ProtocolErrorMoreInformation_type1_messageExtensionNotComprehended:
      msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
        u.messageExtensionNotComprehended
        = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_IdentificationOfReceivedMessage);
       
      msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
        u.messageExtensionNotComprehended->rrc_TransactionIdentifier = 
        tdstransaction_id;

      msg->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
        u.messageExtensionNotComprehended->receivedMessageType =
        received_msg_id;
      break;

    case T_tdsrrc_ProtocolErrorMoreInformation_type1_spare1:
    case T_tdsrrc_ProtocolErrorMoreInformation_type1_spare2:
      /*Do Nothing */
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid protocol Error t: %d",protocol_error_choice);
      break;
  } 
    /*log the protocol error*/
  tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                            (uint8)rrc_procedure,
                            (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                            tdsrrc_log_map_protocol_cause(protocol_error_choice)
                            );
  /* Send the signalling message */
  if (tdsrrcsend_send_ul_sdu(rrc_procedure,msg,lc_id,
                          l2ack_status) == TDSRRCSEND_SUCCESS)
  {
    return SUCCESS;
  }
  else
  {
    return FAILURE;
  }
} /* tdsrrcsend_send_rrc_status_message */
