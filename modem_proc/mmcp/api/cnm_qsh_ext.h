/*===========================================================================

  Copyright (c) 2010 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:

when       who       what, where, why
--------   ---       ----------------------------------------------------------
           swathis   moved to API folder and removed the ifdef
===========================================================================*/
#ifndef CNM_QSH_EXT_H
#define CNM_QSH_EXT_H
#include "comdef.h"
#include "qsh.h"
#include "sys.h"
#include "nas_exp.h"
#include "sys_type.h"
#include "sys_stru.h"
#include "cc_iei.h"
#include "mn_cc.h"

/*===========================================================================

                        MACROS DEFINITION

===========================================================================*/
#define CNM_QSH_MAJOR_VER 1
#define CNM_QSH_MINOR_VER 1

#define MAXNO_DTMF_QUEUE_ENTRIES_DEBUG        32 

#define PROTOCOL_DISCRMINATOR_CNM_DEBUG       3


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
typedef enum
{
  CNM_QSH_METRIC_SIGNALING = 0,
  CNM_QSH_METRIC_MAX
}cnm_qsh_metric_e;

typedef enum
{
  ALERTING_DEBUG           = 0x01,
  CALL_CONFIRMED_DEBUG     = 0x08,
  CALL_PROCEEDING_DEBUG    = 0x02,
  CC_ESTABLISHMENT_DEBUG   = 0x04,
  CC_ESTABLISHMENT_CONFIRMED_DEBUG = 0x06,
  RECALL_DEBUG             = 0x0b,
  START_CC_DEBUG           = 0x09,
  CONNECT_DEBUG            = 0x07,
  CONNECT_ACK_DEBUG        = 0x0f,
  EMERGENCY_SETUP_DEBUG    = 0x0e,
  PROGRESS_DEBUG           = 0x03,
  SETUP_DEBUG              = 0x05,
  MODIFY_DEBUG             = 0x17,
  MODIFY_COMPLETE_DEBUG    = 0x1f,
  MODIFY_REJECT_DEBUG      = 0x13,
  USER_INFORMATION_DEBUG   = 0x10,
  HOLD_DEBUG               = 0x18,
  HOLD_ACK_DEBUG           = 0x19,
  HOLD_REJECT_DEBUG        = 0x1a,
  RETRIEVE_DEBUG           = 0x1c,
  RETRIEVE_ACK_DEBUG       = 0x1d,
  RETRIEVE_REJ_DEBUG       = 0x1e,
  DISCONNECT_DEBUG         = 0x25,
  RELEASE_DEBUG            = 0x2d,
  RELEASE_COMPLETE_DEBUG   = 0x2a,
  CONGESTION_CONTROL_DEBUG = 0x39,
  NOTIFY_DEBUG             = 0x3e,
  STATUS_MSG_DEBUG         = 0x3d,
  STATUS_ENQUIRY_DEBUG     = 0x34,
  START_DTMF_DEBUG         = 0x35,
  STOP_DTMF_DEBUG          = 0x31,
  STOP_DTMF_ACK_DEBUG      = 0x32,
  START_DTMF_ACK_DEBUG     = 0x36,
  START_DTMF_REJ_DEBUG     = 0x37,
  FACILITY_DEBUG           = 0x3a,
  REGISTER_DEBUG           = 0x3b
}cnm_message_id_debug_type;

typedef enum
{
  UNASSIGNED_CAUSE_DEBUG                            = 1,
  NO_ROUTE_TO_DEST_DEBUG                            = 3,
  CHANNEL_UNACCEPTABLE_DEBUG                        = 6,
  OPERATOR_DETERMINED_BARRING_DEBUG                 = 8,
  NORMAL_CALL_CLEARING_DEBUG                        = 16,
  USER_BUSY_DEBUG                                   = 17,
  NO_USER_RESPONDING_DEBUG                          = 18,
  USER_ALERTING_NO_ANSWER_DEBUG                     = 19,
  CALL_REJECTED_DEBUG                               = 21,
  NUMBER_CHANGED_DEBUG                              = 22,
  NON_SELECTED_USER_CLEARING_DEBUG                  = 26,
  DESTINATION_OUT_OF_ORDER_DEBUG                    = 27,
  INVALID_NUMBER_FORMAT_DEBUG                       = 28,
  FACILITY_REJECTED_DEBUG                           = 29,
  RESPONSE_TO_STATUS_ENQUIRY_DEBUG                  = 30,
  NORMAL_UNSPECIFIED_DEBUG                          = 31,
  NO_CIRCUIT_CHANNEL_AVAILABLE_DEBUG                = 34,
  NETWORK_OUT_OF_ORDER_DEBUG                        = 38,
  TEMPORARY_FAILURE_DEBUG                           = 41,
  SWITCHING_EQUIPMENT_CONGESTION_DEBUG              = 42,
  ACCESS_INFORMATION_DISCARDED_DEBUG                = 43,
  REQUESTED_CIRCUIT_CHANNEL_NOT_AVAILABLE_DEBUG     = 44,
  RESOURCES_UNAVAILABLE_UNSPECIFIED_DEBUG           = 47,
  QUALITY_OF_SERVICE_UNAVAILABLE_DEBUG              = 49,
  REQUESTED_FACILITY_NOT_SUBSCRIBED_DEBUG           = 50,
  INCOMING_CALL_BARRED_WITHIN_CUG_DEBUG             = 55,
  BEARER_CAPABILITY_NOT_AUTHORISED_DEBUG            = 57,
  BEARER_CAPABILITY_NOT_PRESENTLY_AVAILABLE_DEBUG   = 58,
  SERVICE_OR_OPTION_NOT_AVAILABLE_DEBUG             = 63,
  BEARER_SERVICE_NOT_IMPLEMENTED_DEBUG              = 65,
  ACM_GEQ_ACMMAX_DEBUG                              = 68,
  REQUESTED_FACILITY_NOT_IMPLEMENTED_DEBUG          = 69,
  ONLY_RESTRICTED_DIGITAL_INFO_BC_AVAILABLE_DEBUG   = 70,
  SERVICE_OR_OPTION_NOT_IMPLEMENTED_DEBUG           = 79,
  INVALID_TRANSACTION_ID_VALUE_DEBUG                = 81,
  USER_NOT_MEMBER_OF_CUG_DEBUG                      = 87,
  INCOMPATIBLE_DESTINATION_DEBUG                    = 88,
  INVALID_TRANSIT_NETWORK_SELECTION_DEBUG           = 91,
  SEMANTICALLY_INCORRECT_MESSAGE_DEBUG              = 95,
  INVALID_MANDATORY_INFORMATION_DEBUG               = 96,
  MESSAGE_TYPE_NON_EXISTENT_DEBUG                   = 97,
  MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROT_STATE_DEBUG = 98,
  IE_NON_EXISTENT_OR_NOT_IMPLEMENTED_DEBUG          = 99,
  CONDITIONAL_IE_ERROR_DEBUG                        = 100,
  MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE_DEBUG  = 101,
  RECOVERY_ON_TIMER_EXPIRY_DEBUG                    = 102,
  PROTOCOL_ERROR_UNSPECIFIED_DEBUG                  = 111,
  ACCESS_CLASS_DSAC_REJECTION_DEBUG                 = 112,
  REGISTRATION_IN_PROGRESS_DEBUG                    = 113,
  INTERWORKING_UNSPECIFIED_DEBUG                    = 127,
  RAB_REJ_CAUSE_LAST_DEBUG                          = INTERWORKING_UNSPECIFIED,
  RAB_REJ_CAUSE_ALL_DEBUG                           = 0xFF,
  RESERVED_DEBUG                                    = 11
}cnm_cause_debug_type;

#define PROTOCOL_DISCRMINATOR_CC_DEBUG  PD_CC

typedef struct 
{
  byte protocol_discriminator;

  //Message ID => Table 10.3/3GPP TS 24.008: Message types for Call Control and call related SS messages (eg. Alerting)
  cnm_message_id_debug_type   message_type; 

  //0 (UE to NW), 1 (NW to UE)
  byte   direction; 

  //Length of the IE
  uint16   length; 

  // CNM cause as per 3GPP spec => Table 10.5.123/3GPP TS 24.008: Cause information element values (eg. UNASSIGNED_CAUSE)
  cnm_cause_debug_type   cause_value; 

  byte   as_id; 
}cnm_qsh_signaling_s;

typedef struct 
{
  qsh_metric_hdr_s hdr; /* This header has the timestamp (uint16) */
  cnm_qsh_signaling_s metric_sig;
}cnm_qsh_metric_signaling_s;


typedef  byte    nas_cnm_sms_state_T_debug;

typedef enum
{
  CNM_QSH_DUMP_TAG_AS_ID_1 = 0,
  CNM_QSH_DUMP_TAG_AS_ID_2 = 1,
  CNM_QSH_DUMP_TAG_MAX_DEBUG = 31
} cnm_qsh_dump_tag_e;

typedef enum  
{
    DTMF_IDLE_DEBUG, 
    WAIT_FOR_START_DTMF_ACK_DEBUG,  
    WAIT_FOR_STOP_DTMF_ACK_DEBUG  
} nas_cnm_cc_dtmf_state_enum_type_debug;

typedef enum 
{
    SS_IDLE_DEBUG               = 0x00, 
    SS_MM_CONN_PEND_DEBUG,  
    SS_CONN_EST_DEBUG,  
    SS_CONN_REESTABLISH_DEBUG, 
    SS_UNKNOWN_DEBUG            = 0xFF 
} nas_cnm_ss_state_T_debug;

typedef enum 
{
    SS_MN_IDLE_DEBUG     = 0x00, 
    SS_MN_NOT_IDLE_DEBUG
} cnm_nas_ss_mn_state_T_debug;

typedef enum
{
  T_NULL_DEBUG        = 0x00,
  T_2_SEND_DEBUG,
  T_MT_CALL_RES_DEBUG,
  T_START_T310_DEBUG
} nas_cnm_srvcc_trigger_e_type_debug;

/*===========================================================================

                        TYPES DEFINITION

===========================================================================*/

typedef union
{
   MNCC_START_DTMF_REQ_T         start_dtmf_queue_entry;
   MNCC_STOP_DTMF_REQ_T          stop_dtmf_queue_entry;
} nas_cnm_dtmf_queue_entry_T_debug;

typedef struct
{
   nas_cnm_sms_state_T_debug    sms_state;
   boolean                      release_pending;
   byte                         pref_rl_domain;  /* prefered RL for CS/PS domain flag */
} nas_cnm_sms_transaction_information_T_debug;

typedef struct
{
   cc_state_T                                   cc_state;
   nas_cnm_cc_dtmf_state_enum_type_debug        cc_dtmf_state;
   CNMServiceType_T                             CNM_service_type;
   hold_auxiliary_state_T                       hold_auxiliary_state;
   mpty_auxiliary_state_T                       mpty_auxiliary_state;
   byte                                         cc_cause[GSM_CAUSE_SIZE];
   boolean                                      T308_expired;
   boolean                                      reserved;    
   byte                                         dtmf_queue_read_index;
   byte                                         dtmf_queue_write_index;
   nas_cnm_dtmf_queue_entry_T_debug             dtmf_queue[MAXNO_DTMF_QUEUE_ENTRIES_DEBUG];
   boolean                                      repeat; 
} nas_cnm_ccc_transaction_information_T_debug;

typedef struct
{
   nas_cnm_ss_state_T_debug     ss_state;
   boolean                      offline;
   cnm_nas_ss_mn_state_T_debug  ss_mn_state;
   sys_modem_as_id_e_type       as_id;
} nas_cnm_ss_transaction_information_T_debug;

typedef struct
{
  nas_cnm_srvcc_trigger_e_type_debug  trigger;
  connection_id_T                     connection_id;
} nas_cnm_srvcc_transient_triggers_T_debug;

typedef struct
{
   CNMServiceType_T     CNM_service_type;
   connection_id_T      connection_id;
   mmcnm_data_req_T     mmcnm_data_req;

} nas_cnm_CNM_request_id_T_debug;

typedef struct
{
   byte  *start;        
   byte  IEI;           
   byte  length;        
   byte  presence;      
   byte  syntax_ok;    
} nas_cnm_IE_info_T_debug;

/******************************************************************************
**                     CNM DEBUG DATA
******************************************************************************/


typedef struct nas_cnm_debug_data_tag
{
        qsh_dump_tag_hdr_s hdr; /* this header will have a tag and length */
        /* CC/SMS/SS state information */
	cc_state_T    next_state_debug;                        /* Holds the next state of call or sms or Supplementary Service per connection id. */
	cc_state_T    last_state_debug;                        /* Holds the last CC state information (U0-U27) */
	
	/* Transaction information stores */
	transaction_id_T                              sms_transaction_id_debug;
	nas_cnm_sms_transaction_information_T_debug   sms_transaction_information_debug;        /* Information store for SMS transaction */
	
	transaction_id_T                               cc_transaction_id_debug;
	nas_cnm_ccc_transaction_information_T_debug    cc_transaction_information_debug;        /* Holds information like CC/DTMF/multiparty call state of CC transaction */
	
	transaction_id_T                              ss_transaction_id_debug;
	nas_cnm_ss_transaction_information_T_debug    ss_transaction_information_debug;         /* Information store for Supplementary Services transactions */
	
	/* Generic variables */
	mmcc_sync_ind_T                                    CNM_mmcc_sync_ind_debug;                  /* Stores the sync information from MM about Traffic channel assignment or channel mode change */
	nas_cnm_srvcc_transient_triggers_T_debug           buffer_transient_triggers_debug;          /* Used to store the transient procedures (like HOLD  request, CONNECT) to be performed after srvcc completion */
	nas_cnm_CNM_request_id_T_debug                     mm_connection_pending_debug;              /* Holds the information(like connection id, service request type etc) for which MM connection is pending or being established */
	
	/* IE realted information */
        byte                            no_of_IEs_debug;                          /* Number of information elements in received OTA */
	nas_cnm_IE_info_T_debug         cc_IE_info_debug;                         /* IEs of the received call control related (voice call/facility etc) OTA messages. */
	byte                            cc_progress_information_debug;            /* Stores the progress value from Progress indicator IE from network */
	
	/* Counters */
	byte                            CNM_mo_cp_data_retransmit_counter_debug;  /* Counter to determine when to stop TC1M timer */
	byte                            CNM_mt_cp_data_retransmit_counter_debug;  /* Counter to determine when to stop TC1M timer - TIMER_TC1M_MT- for MT SMSes */
	
	/* Other variables */
	boolean                         cp_data_l2_ack_sim_debug;                 /* Indicates whether L2 ack was received from network while sending MO SMS */
        boolean                         mm_link_reestablishing_debug;             /* Flag indicating whether mm link re-establishment is in progress */
	boolean                         mm_rel_req_sent_debug;              
} cnm_qsh_dump_data_s_type ;

typedef cnm_qsh_dump_data_s_type cnm_qsh_dump_tag_as_id_1_s;
typedef cnm_qsh_dump_data_s_type cnm_qsh_dump_tag_as_id_2_s;


#endif /* _CNM_QSH_EXT_H_ */
