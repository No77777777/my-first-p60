/*===========================================================================
                  R R C  U E  C A P A B I L I T Y  E N Q U I R Y

DESCRIPTION
  This header file contains function prototypes for UE Capability Enquiry
  procedure

EXTERNALIZED FUNCTIONS
  tdsrrcuece_procedure_event_handler
    This function is the event handler for all events that are sent to the
    UE Capability Enquiry procedure.

  tdsrrcuece_init_procedure
    This function initializes required parameters for UE Capability Enquiry
    procedure.



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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcuece.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/21/11   yuh     Merged LTE code
03/10/11   bj      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/10/10   bj      Made changes to get the value of IE "capability_update_requirement"(FDD->TDD).
11/02/10   zwj     Added code to handle REL4 OTA message.

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
#include "tdsrrcuece.h"
#include "msg.h"
#include "err.h"
#include "tdsrrcasn1.h"
#include "tdsrrclcm.h"
#include "tdsrrcsend.h"
#include "tdsrrcscmgr.h"
#include "tdsrrclogging.h"
#include "tdsrrc_dependancy.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define TDSRRCUECE_MAX_TRANSACTION_ID 3


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


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

FUNCTION tdsrrcuece_copy_capability_update_requirement

DESCRIPTION
  This function copy the Radio Capability Requirement into internal format.
  This stored information will be utilized by any RRC procedure to build 
  UE Capabilities.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcuece_copy_capability_update_requirement
(
  tdsrrc_CapabilityUpdateRequirement capability_update_requirement
                                          /* Capability Update requirement */
)
{
  uint32 i = 0,count=0;                            /* Local varible for counting */
  
  if(TDSRRC_MSG_COMMON_BITMASK_IE(capability_update_requirement,
       tdsrrc_CapabilityUpdateRequirement,systemSpecificCapUpdateReqList))
  {
    tdsrrc_capability_update_requirement.
      system_specific_cap_update_req_present =  TRUE;

    while((capability_update_requirement.systemSpecificCapUpdateReqList.n >  count) && 
        (i < TDSRRC_MAX_SYS_CAPABILITY)) 
    {   
      if(capability_update_requirement.systemSpecificCapUpdateReqList.elem[count]
        == tdsrrc_SystemSpecificCapUpdateReq_gsm)
      {
        tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[i] = TDSRRC_GSM_CAP;
        i++;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unknown System Specific Capability Update for %d",capability_update_requirement.systemSpecificCapUpdateReqList.elem[count]);
      }
      /* Advance to the next member of the list */     
     count++;
    }
  }
  else
  {
    tdsrrc_capability_update_requirement.
      system_specific_cap_update_req_present =  FALSE;
  }

  /* Update the count for the Inter system Capability Update Requirement */
  tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.count =(uint8) i;  
 
   
} /* tdsrrcuece_copy_capability_update_requirement */

/*===========================================================================

FUNCTION tdsrrcuece_copy_capability_update_requirement_r7

DESCRIPTION
  This function copy the Radio Capability Requirement into internal format.
  This stored information will be utilized by any RRC procedure to build 
  UE Capabilities.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcuece_copy_capability_update_requirement_r7
(
  tdsrrc_CapabilityUpdateRequirement_r7 *capability_update_requirement
                                          /* Capability Update requirement */
)
{
  uint32 i = 0,count =0;                            /* Local varible for counting */

  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(capability_update_requirement,
       tdsrrc_CapabilityUpdateRequirement_r7,systemSpecificCapUpdateReqList))
  {
    tdsrrc_capability_update_requirement.
      system_specific_cap_update_req_present =  TRUE;

    while((capability_update_requirement->systemSpecificCapUpdateReqList.n > count) && 
        (i < TDSRRC_MAX_SYS_CAPABILITY)) 
    {   
      if(capability_update_requirement->systemSpecificCapUpdateReqList.elem[count] == tdsrrc_SystemSpecificCapUpdateReq_r5_gsm)
      {
        tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[i] = TDSRRC_GSM_CAP;
        i++;
      }
      else if(capability_update_requirement->systemSpecificCapUpdateReqList.elem[count] == tdsrrc_SystemSpecificCapUpdateReq_r5_geranIu)
      {
        tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[i] = TDSRRC_GERANIU_CAP;
        i++;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unknown System Specific Capability Update for %d",capability_update_requirement->systemSpecificCapUpdateReqList.elem[count]);
      }
      /* Advance to the next member of the list */     
      count++;
    }
  }
  else
  {
    tdsrrc_capability_update_requirement.
      system_specific_cap_update_req_present =  FALSE;
  }

  /* Update the count for the Inter system Capability Update Requirement */
  tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.count = (uint8)i;  
 
  /* Update UE Radio Capability Update Requirement */  
  tdsrrc_capability_update_requirement.ue_radio_capability_update_requirement =
    capability_update_requirement->ue_RadioCapabilityTDDUpdateRequirement_TDD128;
   
} /* rrcuece_copy_capability_update_requirement_r5 */

#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================
FUNCTION tdsrrcuece_copy_capability_update_requirement_r8

DESCRIPTION
  This function copy the Radio Capability Requirement into internal format.
  This stored information will be utilized by any RRC procedure to build 
  UE Capabilities.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcuece_copy_capability_update_requirement_r8
(
  tdsrrc_CapabilityUpdateRequirement_r8 *capability_update_requirement
                                          /* Capability Update requirement */
)
{
  uint8 i = 0,count =0;                            /* Local varible for counting */

  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(capability_update_requirement,
        tdsrrc_CapabilityUpdateRequirement_r8,systemSpecificCapUpdateReqList))
  {
    tdsrrc_capability_update_requirement.
      system_specific_cap_update_req_present =  TRUE;

    while((capability_update_requirement->systemSpecificCapUpdateReqList.n > count) && 
        (i < TDSRRC_MAX_SYS_CAPABILITY)) 
    {   
      if(capability_update_requirement->systemSpecificCapUpdateReqList.elem[count] == tdsrrc_SystemSpecificCapUpdateReq_r8_gsm)
      {
        tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[i] = TDSRRC_GSM_CAP;
        i++;
      }
      else if(capability_update_requirement->systemSpecificCapUpdateReqList.elem[count] == tdsrrc_SystemSpecificCapUpdateReq_r8_geranIu)
      {
        tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[i] = TDSRRC_GERANIU_CAP;
        i++;
      }
      else if(capability_update_requirement->systemSpecificCapUpdateReqList.elem[count] == tdsrrc_SystemSpecificCapUpdateReq_r8_eutra)
      {
        tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[i] = TDSRRC_EUTRA_CAP;
        i++;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unknown System Specific Capability Update for %d",capability_update_requirement->systemSpecificCapUpdateReqList.elem[count]);
      }
      /* Advance to the next member of the list */     
      count++;
    }
  }
  else
  {
    tdsrrc_capability_update_requirement.system_specific_cap_update_req_present =  FALSE;
  }

  /* Update the count for the Inter system Capability Update Requirement */
  tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.count =(uint8) i;  
 
  /* Update UE Radio Capability Update Requirement */  
  tdsrrc_capability_update_requirement.ue_radio_capability_update_requirement =
    capability_update_requirement->ue_RadioCapabilityTDDUpdateRequirement_TDD128;

  return;   
} /* tdsrrcuece_copy_capability_update_requirement_r8 */
#endif /*FEATURE_TDSCDMA_REL8*/


/*===========================================================================

FUNCTION tdsrrcuece_copy_capability_update_requirement_later_than_r3

DESCRIPTION
  This function copy the Radio Capability Requirement into internal format.
  This stored information will be utilized by any RRC procedure to build 
  UE Capabilities.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcuece_copy_capability_update_requirement_later_than_r3
(
  tdsrrc_CapabilityUpdateRequirement_r5 capability_update_requirement
                                          /* Capability Update requirement */
)
{
  uint32 i = 0,count =0;                            /* Local varible for counting */

  if(TDSRRC_MSG_COMMON_BITMASK_IE(capability_update_requirement,
       tdsrrc_CapabilityUpdateRequirement_r5,systemSpecificCapUpdateReqList))
  {
    tdsrrc_capability_update_requirement.
      system_specific_cap_update_req_present =  TRUE;

    /* Update System Specific Capability Update Requirement info */
    while((capability_update_requirement.systemSpecificCapUpdateReqList.n > count) && 
        (i < TDSRRC_MAX_SYS_CAPABILITY)) 
    {   
      if(capability_update_requirement.systemSpecificCapUpdateReqList.elem[count] == tdsrrc_SystemSpecificCapUpdateReq_r5_gsm)
      {
        tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[i] = TDSRRC_GSM_CAP;
        i++;
      }
      else if(capability_update_requirement.systemSpecificCapUpdateReqList.elem[count] == tdsrrc_SystemSpecificCapUpdateReq_r5_geranIu)
      {
        tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[i] = TDSRRC_GERANIU_CAP;
        i++;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unknown System Specific Capability Update for %d",capability_update_requirement.systemSpecificCapUpdateReqList.elem[count]);
      }
      /* Advance to the next member of the list */     
      count++;
    }
  }
  else
  {
    tdsrrc_capability_update_requirement.
      system_specific_cap_update_req_present =  FALSE;
  }

  /* Update the count for the Inter system Capability Update Requirement */
  tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.count =(uint8) i;  
 
  /* Update UE Radio Capability Update Requirement */  
  tdsrrc_capability_update_requirement.ue_radio_capability_update_requirement =
    capability_update_requirement.ue_RadioCapabilityTDDUpdateRequirement_TDD128;
   
} /* rrcuece_copy_capability_update_requirement_r5 */

/*===========================================================================

FUNCTION tdsrrcuece_process_ue_capability_enquiry_msg  

DESCRIPTION
  This function checks whether TDSRRC_DOWNLINK_SDU_IND contains UE Capability
  Enquiry message.If it contains UE Capability  Enquiry message,
  it will processes the same and send TDSRRC_INITIATE_UE_CAPABILITY_REQ.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/

static void tdsrrcuece_process_ue_capability_enquiry_msg
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;    /* Pointer to Downlink DCCH */

  tdsrrc_cmd_type *rrc_cmd_ptr;                 /* Pointer to the RRC Command */

  rlc_lc_id_type lc_id;                      /* Local variable to store 
                                                RLC LC Id */
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id;
                                             /* Local variable to store 
                                                transaction Id */
  uint8 message_choice;                      /* Local variable for message
                                                choice */

  /* Set the protocol error reject to FALSE */
  tdsrrc_protocol_error_reject = FALSE;

  /* Assert the received message is of type tdsrrc_DL_CCCH_Message */
  if((TDSRRC_DOWNLINK_SDU_IND == cmd_ptr->cmd_hdr.cmd_id) && 
     (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_DCCH_Message_PDU) )
  {
    message_choice = 
      tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                           cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

    /* Get the OTA message pointer.  If ASN1 decoding has failed then the
       dcch_msg_ptr will be initialized to NULL */
    dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) 
                    cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;
  
    /* Check the received message is UE Capability  Enquiry message */
    if( message_choice ==
        T_tdsrrc_DL_DCCH_MessageType_ueCapabilityEnquiry)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received UE Capability Enquiry Message");

      if(dcch_msg_ptr == NULL) /* DCCH ASN1 decoding failed */
      {

#if(!defined(T_WINNT))
        /*log the protocol error*/
        tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                                (uint8)TDSRRC_PROCEDURE_UECE,
                                (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                                (uint8)TDSRRCLOG_ASN1_ENC_ERR
                               );
#endif
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
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't find RLC ID for UL RB 2");
        } 
        else /* Valid RLC ID found */        
        {
          if(SUCCESS != tdsrrcsend_send_rrc_status_message(
             T_tdsrrc_ProtocolErrorMoreInformation_type1_asn1_ViolationOrEncodingError,
                                           /* The protocol error cause to be
                                              placed in the message */
             0,                            /* Set the transaction id to 0 
                                              since ASN1 decoding failed */
             tdsrrc_ReceivedMessageType_ueCapabilityEnquiry,      /* Received message identification */                                          
             TDSRRC_PROCEDURE_UECE,           /* RRC procedure */
             lc_id,                        /* Logical Channel Id */
             TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                           /* Indicates whether L2 Ack 
                                              required or not */
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
          } 
        } /* Valid RLC ID found */      
      }
      
      else /* ASN1 decoding is successful */
      {
        if(dcch_msg_ptr->message.u.ueCapabilityEnquiry.t ==
           T_tdsrrc_UECapabilityEnquiry_later_than_r3 )
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Critical Extensions are not supported");

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
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't find RLC ID for UL RB 2");
          } 
          else
          {
            if(SUCCESS != tdsrrcsend_send_rrc_status_message(
               T_tdsrrc_ProtocolErrorMoreInformation_type1_messageExtensionNotComprehended,
                                             // The protocol error cause to be placed in
                                             // the message

               dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.
               ueCapabilityEnquiry_r3.rrc_TransactionIdentifier,                            
                                             // Received transaction Id 

               tdsrrc_ReceivedMessageType_ueCapabilityEnquiry,  
                                             // Received message identification
               TDSRRC_PROCEDURE_UECE,           // RRC procedure
               lc_id,                        /* Logical Channel Id */
               TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                       /* Indicates whether L2 Ack required or not */
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
            }
          }
          return;
        }
        /* Update Transaction Id */
        tdstransaction_id = dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.
                   ueCapabilityEnquiry_r3.rrc_TransactionIdentifier;

        if(tdstransaction_id > TDSRRCUECE_MAX_TRANSACTION_ID)
        {
#if(!defined(T_WINNT))
          /*log the protocol error*/
          tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                                  (uint8)TDSRRC_PROCEDURE_UECE,
                                  (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                                  (uint8)TDSRRCLOG_BAD_IE
                                 );
#endif
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
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't find RLC ID for UL RB 2");
          } 
          else
          {
            if(SUCCESS != tdsrrcsend_send_rrc_status_message(
               T_tdsrrc_ProtocolErrorMoreInformation_type1_ie_ValueNotComprehended,
                                             // The protocol error cause to be placed in
                                             // the message
               tdstransaction_id,               // Received transaction Id 
               tdsrrc_ReceivedMessageType_ueCapabilityEnquiry,  
                                             // Received message identification
               TDSRRC_PROCEDURE_UECE,           // RRC procedure
               lc_id,                        /* Logical Channel Id */
               TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                       /* Indicates whether L2 Ack required or not */
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
            }
          }
        }
       
        else /* Valid Transaction id */
        {
          /*Initialize the UE Capability Update Requirement */
          tdsrrc_capability_update_requirement.
            ue_radio_capability_update_requirement = FALSE;

          tdsrrc_capability_update_requirement.
            system_specific_cap_update_req_present = FALSE;

          /* Copy UE Capability Update Requirement from the UE Capability
             Enquiry message */
          tdsrrcuece_copy_capability_update_requirement(
            dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.
            ueCapabilityEnquiry_r3.capabilityUpdateRequirement);

          /* Update UE Radio Capability Update Requirement */
		  if (TDSRRC_MSG_COMMON_BITMASK_IE(dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3,
              tdsrrc_UECapabilityEnquiry_r3,laterNonCriticalExtensions))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE(dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.
              laterNonCriticalExtensions,tdsrrc_UECapabilityEnquiry_laterNonCriticalExtensions,
              v4b0NonCriticalExtensions))
            {
              tdsrrc_capability_update_requirement.ue_radio_capability_update_requirement =
              dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.laterNonCriticalExtensions.
              v4b0NonCriticalExtensions.ueCapabilityEnquiry_v4b0ext.
              capabilityUpdateRequirement_r4_ext.ue_RadioCapabilityUpdateRequirement_TDD128;
            }
          }
		  
		  #ifdef FEATURE_TDSCDMA_TO_LTE
          if((dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.m.laterNonCriticalExtensionsPresent) &&
              (dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.laterNonCriticalExtensions.m.v4b0NonCriticalExtensionsPresent) &&
              (dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.laterNonCriticalExtensions.v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent) &&
              (dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.
                   v770NonCriticalExtensionsPresent) &&
              (dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
                   v770NonCriticalExtensions.m.v860NonCriticalExtensionsPresent) &&
              (dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
                   v770NonCriticalExtensions.v860NonCriticalExtensions.ueCapabilityEnquiry_v860ext.m.capabilityUpdateRequirementPresent) &&
              (dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.
                   v770NonCriticalExtensions.v860NonCriticalExtensions.ueCapabilityEnquiry_v860ext.capabilityUpdateRequirement.m.systemSpecificCapUpdateReqListPresent))
          {
              /* Appending the list filled with ueCapabilityEnquiry_r3.capabilityUpdateRequirement.rrcuece_copy_capability_update_requirement */
              uint8 count = 0, i = tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.count;
              tdsrrc_SystemSpecificCapUpdateReqList_r8 *systemSpecificCapUpdateReqList;

              systemSpecificCapUpdateReqList = &dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
                   v590NonCriticalExtensions.v770NonCriticalExtensions.v860NonCriticalExtensions.ueCapabilityEnquiry_v860ext.capabilityUpdateRequirement.systemSpecificCapUpdateReqList;

              tdsrrc_capability_update_requirement.system_specific_cap_update_req_present =  TRUE;

              while((systemSpecificCapUpdateReqList != NULL ) && (systemSpecificCapUpdateReqList->n > count) && 
                  (i < TDSRRC_MAX_SYS_CAPABILITY)) 
              {   
                if(systemSpecificCapUpdateReqList->elem[count] == tdsrrc_SystemSpecificCapUpdateReq_r8_gsm)
                {
                  tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[i] = TDSRRC_GSM_CAP;
                  i++;
                }
                else if(systemSpecificCapUpdateReqList->elem[count] == tdsrrc_SystemSpecificCapUpdateReq_r8_geranIu)
                {
                  tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[i] = TDSRRC_GERANIU_CAP;
                  i++;
                }
                else if(systemSpecificCapUpdateReqList->elem[count] == tdsrrc_SystemSpecificCapUpdateReq_r8_eutra)
                {
                  tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[i] = TDSRRC_EUTRA_CAP;
                  i++;
                }
                else
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unknown System Specific Capability Update for %d",systemSpecificCapUpdateReqList->elem[count]);
                }
                /* Advance to the next member of the list */     
                count++;
              }

              /* Update the count for the Inter system Capability Update Requirement */
              tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.count = i;
          }
#endif

          /* Validate UE Capability Update Requirement */
          if(( tdsrrc_capability_update_requirement.
              ue_radio_capability_update_requirement == TRUE) ||
 
              (tdsrrc_capability_update_requirement.
               system_specific_cap_update_req_present == TRUE) )
          {
            if (tdsrrc_capability_update_requirement.
               system_specific_cap_update_req_present == TRUE)
            {
              if(tdsrrc_capability_update_requirement.
                 system_specific_cap_update_req_list.count == 0)
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
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't find RLC ID for UL RB 2");
                } 
                else
                {
                  if(SUCCESS != tdsrrcsend_send_rrc_status_message(
                     T_tdsrrc_ProtocolErrorMoreInformation_type1_ie_ValueNotComprehended,
                                                   // The protocol error cause to be placed in
                                                   // the message
                     tdstransaction_id,               // Received transaction Id 
                     tdsrrc_ReceivedMessageType_ueCapabilityEnquiry,  
                                                   // Received message identification
                     TDSRRC_PROCEDURE_UECE,           // RRC procedure
                     lc_id,                        /* Logical Channel Id */
                     TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                             /* Indicates whether L2 Ack required or not */
                  {
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
                  }
                }
                return;
              }
            } /* System capability == TRUE */
            
            /* Allocates the buffer to RRC Internal command */
            if( (rrc_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
            {     
              /* Fill in the command parameters for 
               TDSRRC_INITIATE_UE_CAPABILITY_REQ */
              rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_INITIATE_UE_CAPABILITY_REQ;

              rrc_cmd_ptr->cmd.initiate_ue_cap.originator = 
              TDSRRC_UE_CAPABILITY_ORIGINATOR_UTRAN;
            
              rrc_cmd_ptr->cmd.initiate_ue_cap.transaction_id = 
              tdstransaction_id;
            
              /* Send TDSRRC_INITIATE_UE_CAPABILITY_REQ primitive */
              tdsrrc_put_int_cmd(rrc_cmd_ptr);   
            } 
            else
            {
              ERR_FATAL("Memory allocation for the %d cmd failed",TDSRRC_INITIATE_UE_CAPABILITY_REQ,0,0);
            }
          }
          else
          {
            /* UE Radio Capability Update Requirement is not set. Then send
               RRC Status message */
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"UE Radio Cap Update Req. is not set: %d %d",
                      tdsrrc_capability_update_requirement.
                        ue_radio_capability_update_requirement,
                      tdsrrc_capability_update_requirement.
                        system_specific_cap_update_req_present,0);

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
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't find RLC ID for UL RB 2");
            } 
            else
            {
              if(SUCCESS != tdsrrcsend_send_rrc_status_message(
                 T_tdsrrc_ProtocolErrorMoreInformation_type1_ie_ValueNotComprehended,
                                               // The protocol error cause to be placed in
                                               // the message
                 tdstransaction_id,               // Received transaction Id 
                 tdsrrc_ReceivedMessageType_ueCapabilityEnquiry,  
                                               // Received message identification
                 TDSRRC_PROCEDURE_UECE,           // RRC procedure
                 lc_id,                        /* Logical Channel Id */
                 TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                         /* Indicates whether L2 Ack required or not */
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
              }
            }
          } 
        } /* Valid Transaction id */
      } /* ASN1 decoding is successful */
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid msg: %d is forwarded",message_choice);
    }
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d or SDU type: %d",cmd_ptr->cmd_hdr.cmd_id,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
  }

} /* tdsrrcuece_process_ue_capability_enquiry_msg */
/*===========================================================================

FUNCTION tdsrrcuece_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the UE Capability Enquiry procedure.
  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (received from RLC) containing UE 
    CAPABILITY ENQUIRY message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcuece_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
 
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  switch (rrc_state)
  {
    case TDSRRC_STATE_DISCONNECTED: 
    case TDSRRC_STATE_CONNECTING:
      /* In this state there is no RRC Connection. Hence trash the message */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UECE Msg is received in invalid state: %d",rrc_state); 
      break;

    case TDSRRC_STATE_CELL_FACH:    
    case TDSRRC_STATE_CELL_DCH:  
      /* These two states are valid to receive UE Capability Enquiry message */
      tdsrrcuece_process_ue_capability_enquiry_msg(cmd_ptr); 
      break;

    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:
      /* The logical DCCH is released in these states.Due to race conditions, RRC
         may receive a valid UE Capability Enquiry message. Hence process the 
         same. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"UECE msg is recievd in PCH state");
      tdsrrcuece_process_ue_capability_enquiry_msg(cmd_ptr);
      break;

    default: 
      /* This should never happen */
      ERR_FATAL("Event fwded to UECE in invalid State: %d",rrc_state,0,0);
      break;
   }  
} /* tdsrrcuece_procedure_event_handler */
/*===========================================================================

FUNCTION tdsrrcuece_init_procedure

DESCRIPTION
  This function initializes required parameters for UE Capability Enquiry
  procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcuece_init_procedure
( 
  void
)
{
  // This is a template for future expansion */
} /* tdsrrcuece_init_procedure */
/*===========================================================================

FUNCTION tdsrrcuece_copy_capability_update_requirement_r4

DESCRIPTION
  This function copy the Radio Capability Requirement into internal format.
  This stored information will be utilized by any RRC procedure to build 
  UE Capabilities.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcuece_copy_capability_update_requirement_r4
(
  tdsrrc_CapabilityUpdateRequirement_r4 capability_update_requirement
                                          /* Capability Update requirement */
)
{
  uint32 i = 0,count =0;                            /* Local varible for counting */

  if(TDSRRC_MSG_COMMON_BITMASK_IE(capability_update_requirement,
       tdsrrc_CapabilityUpdateRequirement_r4,systemSpecificCapUpdateReqList))
  {
    tdsrrc_capability_update_requirement.
      system_specific_cap_update_req_present =  TRUE;

    /* Update System Specific Capability Update Requirement info */
    while((capability_update_requirement.systemSpecificCapUpdateReqList.n > count) && 
        (i < TDSRRC_MAX_SYS_CAPABILITY)) 
    {   
      if(capability_update_requirement.systemSpecificCapUpdateReqList.elem[count] == tdsrrc_SystemSpecificCapUpdateReq_gsm)
      {
        tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.value[i] = TDSRRC_GSM_CAP;
        i++;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unknown System Specific Capability Update for %d",capability_update_requirement.systemSpecificCapUpdateReqList.elem[count]);
      }
      /* Advance to the next member of the list */     
      count++;
    }
  }
  else
  {
    tdsrrc_capability_update_requirement.
      system_specific_cap_update_req_present =  FALSE;
  }

  /* Update the count for the Inter system Capability Update Requirement */
  tdsrrc_capability_update_requirement.system_specific_cap_update_req_list.count =(uint8) i;  
 
  /* Update UE Radio Capability Update Requirement */  
  tdsrrc_capability_update_requirement.ue_radio_capability_update_requirement =
    capability_update_requirement.ue_RadioCapabilityTDDUpdateRequirement_TDD128;
}

