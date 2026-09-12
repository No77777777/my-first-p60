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

$Header: //components/rel/mmcp.mpss/7.9.0/api/mm_qsh_ext.h#1 $
===========================================================================*/
#ifndef _MM_QSH_EXT_H_
#define _MM_QSH_EXT_H_

#include <customer.h>
#include "qsh.h"
#include "comdef.h"
#include "sys_plmn_selection.h"
#include "sys_stru.h"
#include "sys_type.h"
#include "lte_rrc_ext_msg.h"
#include "lte_nas_common.h"
#include "lte.h"
#include "reg_mm.h"
#include "lte_cphy_msg.h"
#include "ms_cc_mm.h"
#include "mm.h"
#include "rrcmmif.h"
#include "mm_as_types.h"
#include "lte_nas_common.h"
#include "msgr_umid.h"
#include "msgr_types.h"
#include "msgr_lte.h"
#include "msgr_nas.h"
#ifdef FEATURE_DISABLED_HABANERO
#include "qevent.h"
#include "cafi_kpi_client.h"
#endif

extern int32 mm_qsh_event_rplmn_id;



/*===========================================================================

                        MACROS DEFINITION

===========================================================================*/
#define MM_QSH_MAJOR_VER 1
#define MM_QSH_MINOR_VER 1

#define MM_DEBUG_DUMP_BUFFER_SIZE      64
#define EQUIVALENT_PLMN_LIST_MAX_LENGTH_DEBUG 15
#define MAX_LU_ATTEMPT_SIZE_DEBUG 4
#define MAX_MM_TIMER_DEBUG 91

typedef enum
{
  EMM_STATUS_UPDATED_DEBUG,
  EMM_STATUS_NOT_UPDATED_DEBUG,
  EMM_STATUS_ROAMING_NOT_ALLOWED_DEBUG
} emm_eps_update_status_debug_type ;

typedef enum
{
  EMM_NULL_DEBUG,
  EMM_DEREGISTERED_DEBUG,
  EMM_REGISTERED_INITIATED_DEBUG,
  EMM_REGISTERED_DEBUG,
  EMM_TRACKING_AREA_UPDATING_INITIATED_DEBUG,
  EMM_SERVICE_REQUEST_INITIATED_DEBUG,
  EMM_DEREGISTERED_INITIATED_DEBUG,
  EMM_INVALID_STATE_DEBUG // FOR INTRNAL USE ONLY
} emm_state_debug_type; 

typedef enum
{
  EMM_IDLE_STATE_DEBUG,
  EMM_WAITING_FOR_RRC_CONFIRMATION_STATE_DEBUG,
  EMM_CONNECTED_STATE_DEBUG,
  EMM_RELEASING_RRC_CONNECTION_STATE_DEBUG
} emm_connection_state_debug_type ;

typedef enum
{
  TA_UPDATING_DEBUG = 0,
  COMBINED_TA_LA_UPDATING_DEBUG = 1,
  COMBINED_TA_LA_UPDATING_IMSI_ATTACH_DEBUG = 2,
  PERIODIC_UPDATING_DEBUG  = 3
}lte_nas_emm_eps_update_val_debug_type;

typedef enum
{
  INVALID_DETACH_TYPE_DEBUG,
  EPS_DETACH_DEBUG              = 1,
  IMSI_DETACH_MO_DETACH_DEBUG   = 2,
  COMBINED_EPS_DETACH_DEBUG     = 3
}emm_mo_detach_debug_type;

typedef enum
{
  NORMAL_DETACH_DEBUG = 0,
  SWITCH_OFF_DEBUG    = 1,
  INVALID_DETACH_DEBUG
}emm_detach_reason_debug_type;

typedef enum{
  IMSI_ID_DEBUG = 1,
  IMEI_ID_DEBUG = 3,
  GUTI_ID_DEBUG = 6
}ue_id_debug_type;


typedef enum 
{
  TAU_CAUSE_INVALID_DEBUG,
  TAU_CAUSE_TA_CHANGE_DEBUG,
  TAU_CAUSE_LOAD_BALANCING_DEBUG,
  TAU_CAUSE_PERIODIC_DEBUG,
  TAU_CAUSE_LOW_LAYER_FAILURE_DEBUG,
  TAU_CAUSE_OTHERS_DEBUG
}lte_nas_tau_cause_debug_type;

typedef enum
{
  NAS_MM_TIN_P_TMSI_DEBUG,
  NAS_MM_TIN_GUTI_DEBUG,
  NAS_MM_TIN_RAT_RELATED_TMSI_DEBUG,
  NAS_MM_TIN_NONE_DEBUG
} nas_mm_tin_debug_type;

typedef enum
{  
  INVALID_MT_DETACH_TYPE_DEBUG, 
  REATTACH_REQUIRED_DEBUG              = 1,
  REATTACH_NOT_REQUIURED_DEBUG         = 2,
  IMSI_DETACH_MT_DETACH_DEBUG          = 3,
  RESERVED_MT_DETACH_TYPE_ONE_DEBUG    = 6,
  RESERVED_MT_DETACH_TYPE_TWO_DEBUG    = 7

}lte_nas_emm_mt_detach_debug_type;

typedef enum
{
  NAS_EMM_1xCSFB_DEBUG,
  NAS_EMM_3GPP_CSFB_DEBUG,
  NAS_EMM_1X_CSFB_BUFFERED_DEBUG,
  NAS_EMM_3GPP_CSFB_BUFFERED_DEBUG,
  NAS_EMM_CSFB_NONE_DEBUG
} emm_esr_debug_type;


typedef enum
{
  NAS_EMM_SG_ASSOC_NOT_REQ_DEBUG,
  NAS_EMM_SG_ASSOC_REQ_DUE_TO_CS_OR_CSPS_PROC_DEBUG,
  NAS_EMM_SG_ASSOC_REQ_DUE_TO_SRVCC_DEBUG,
  NAS_EMM_SG_ASSOC_REQ_FOR_CSFB_RAU_DEBUG
}sg_association_to_be_established_debug_enum_T;

typedef enum {
  EPS_ONLY_ATTACH_DEBUG = 1,
  COMBINED_EPS_IMSI_ATTACH_DEBUG,
  EPS_EMERGENCY_ATTACH_DEBUG = 6
}lte_nas_emm_attach_debug_type;

typedef enum
{
  EMM_PS_DETACH_NONE_DEBUG,
  EMM_PS_DETACH_REQ_PENDING_DEBUG,
  EMM_PS_DETACH_CNF_PENDING_DEBUG
}emm_ps_detach_debug_status;

typedef enum
{
  LTE_NAS_EMC_SRV_NONE_DEBUG,
  LTE_NAS_EMC_SRV_PENDING_DEBUG,
  LTE_NAS_EMC_SRV_ONLY_DEBUG,
  LTE_NAS_EMC_SRV_EXIST_DEBUG

}lte_nas_emc_srv_status_debug_type;


typedef enum
{
  EMM_LTOG_CCO_INACTIVE_DEBUG,
  EMM_LTOG_CCO_IN_PROGRESS_DEBUG,
  EMM_LTOG_CCO_ABORTED_DEBUG,
  EMM_LTOG_CCO_FAILED_DEBUG
} emm_l2gcco_in_progress_debug_enum_T;

typedef enum
{
  EMM_PENDING_REG_ACK_NONE_DEBUG = -1,
  EMM_PENDING_REG_ACK_ATTACH_COMPLETE_DEBUG,
  EMM_PENDING_REG_ACK_TAU_COMPLETE_DEBUG
}emm_pending_reg_ack_debug_enum_T;

typedef enum
{
  EMM_PS_RESUMPTION_NONE_DEBUG,
  /* This state is used for case when csfb MO call is ended by user
  ** but UE still gets redirected to G later as network is not aware of call end*/
  EMM_PS_RESUMPTION_ESR_STATE_DEBUG,
  EMM_PS_RESUMPTION_PENDING_DEBUG
}emm_ps_resumption_status_debug_enum_T;

typedef enum
{
  GMM_NULL_DEBUG,
  GMM_DEREGISTERED_DEBUG,
  GMM_REGISTERED_INITIATED_DEBUG,
  GMM_REGISTERED_DEBUG,
  GMM_DEREGISTERED_INITIATED_DEBUG,
  GMM_ROUTING_AREA_UPDATING_INITIATED_DEBUG,
  GMM_SERVICE_REQUEST_INITIATED_DEBUG
} gmm_state_debug_type;

typedef enum
{
  GMM_NORMAL_SERVICE_DEBUG,
  GMM_LIMITED_SERVICE_DEBUG,
  GMM_ATTACH_NEEDED_DEBUG,
  GMM_ATTEMPTING_TO_ATTACH_DEBUG,
  GMM_NO_IMSI_DEBUG,
  GMM_NO_CELL_AVAILABLE_DEBUG,
  GMM_PLMN_SEARCH_DEBUG,
  GMM_SUSPENDED_DEBUG,
  GMM_UPDATE_NEEDED_DEBUG,
  GMM_ATTEMPTING_TO_UPDATE_DEBUG,
  GMM_ATTEMPTING_TO_UPDATE_MM_DEBUG,
  GMM_IMSI_DETACH_INITIATED_DEBUG,
  GMM_NULL_SUBSTATE_DEBUG
} gmm_substate_debug_type;

typedef enum
{
 EMM_CSFB_PAGING_TYPE_IMSI_DEBUG = 0,
 EMM_CSFB_PAGINGI_TYPE_TMSI_DEBUG = 1
}lte_nas_paging_identity_debug_type;

typedef enum
{
  MM_CSFB_STATE_NULL_DEBUG,
  MM_CSFB_STATE_MO_CALL_ORIG_DEBUG,
  MM_CSFB_STATE_MT_CALL_ORIG_DEBUG,
  MM_CSFB_STATE_LU_PENDING_DEBUG,
  MM_CSFB_STATE_MT_CALL_CCO_DEBUG,
  MM_CSFB_STATE_MAX_DEBUG
} mm_csfb_state_debug_type;

typedef enum
{
  MM_PS_DETACH_STATE_NONE_DEBUG,
  MM_PS_DETACH_STATE_WAITING_TO_SEND_CNF_DEBUG,
  MM_PS_DETACH_STATE_DETACH_REQ_PENDING_DEBUG,
  MM_PS_DETACH_STATE_WAITING_FOR_REG_REQ_DEBUG,
}mm_ps_detach_state_debug_enum_T;

typedef enum
{
  MM_RLF_STOP_MODE_PENDING_NONE_DEBUG,
  MM_WCDMA_RLF_STOP_MODE_PENDING_DEBUG,
  MM_LTE_RLF_STOP_MODE_PENDING_DEBUG
}mm_rlf_stop_mode_pending_debug_enum_T;

typedef enum
{
  WCDMA_RLF_IDLE_DEBUG,
  WCDMA_RLF_STARTED_DEBUG,
  WCDMA_RLF_SERVICE_CNF_PENDING_DEBUG
} wcdma_rlf_state_debug_enum_T;

typedef enum
{
  CCO_INACTIVE_DEBUG,
  CCO_IN_PROGRESS_DEBUG,
  CCO_FAILED_DEBUG,
  CCO_COMPLETE_DEBUG
} WtoGCCO_in_progress_debug_enum_T ;

typedef enum
{
  EMERGENCY_MM_CONNECTION_DEBUG,
  NORMAL_MM_CONNECTION_DEBUG,
  NO_MM_CONNECTION_DEBUG
} mm_connection_debug_type_T;

typedef enum
{
  MM_RAT_LO_DEBUG,
  MM_RAT_INITIAL_DEBUG,
  MM_RAT_STOP_GSM_IN_PROGRESS_DEBUG,
  MM_RAT_STOP_WCDMA_IN_PROGRESS_DEBUG,
  MM_RAT_STOP_TDSCDMA_IN_PROGRESS_DEBUG,
  MM_RAT_ABORT_IN_PROGRESS_DEBUG,
  MM_RAT_ABORT_PENDING_DEBUG,
  MM_RAT_STOP_GSM_PENDING_ABORT_DEBUG,
  MM_RAT_STOP_WCDMA_PENDING_ABORT_DEBUG,
  MM_RAT_HI_DEBUG
} mm_rat_handler_status_debug_type;

typedef enum
{
  SERVICE_AVAILABLE_DEBUG,
  SERVICE_NOT_AVAILABLE_DEBUG
} service_state_debug_type;

typedef enum
{
  MM_INTERRAT_CHANGE_STATE_TYPE_LO_DEBUG,
  MM_UMTS_TO_GSM_INTERRAT_CHANGE_IN_PROGRESS_DEBUG,
  MM_GSM_TO_UMTS_INTERRAT_CHANGE_IN_PROGRESS_DEBUG,
  MM_LTE_TO_UMTS_INTERRAT_CHANGE_IN_PROGRESS_DEBUG,
  MM_LTE_TO_GSM_INTERRAT_CHANGE_IN_PROGRESS_DEBUG,
  MM_INTERRAT_CHANGE_STATE_TYPE_HI_DEBUG
} mm_interrat_change_state_debug_type;

typedef enum 
{
  MMRRC_IDLE_DEBUG,
  MMRRC_EST_IN_PROGRESS_DEBUG,
  MMRRC_ACTIVE_DEBUG,
  MMRRC_RELEASE_IN_PROGRESS_DEBUG
}mmrrc_connection_status_debug_T;

typedef enum
{
  AUTH_GSM_SECURITY_CONTEXT_DEBUG,
  AUTH_GSM_GPRS_SECURITY_CONTEXT_DEBUG,
  AUTH_CS_GPRS_SECURITY_CONTEXT_DEBUG,
  AUTH_PS_GPRS_SECURITY_CONTEXT_DEBUG,
  AUTH_COMBINED_SECURITY_CONTEXT_DEBUG
} auth_security_context_id_debug ;


typedef enum 
{
  TO_MM_DEBUG,
  FROM_MM_DEBUG,
  TO_EMM_DEBUG, 
  FROM_EMM_DEBUG
}mm_emm_message_direction_debug_type_T;




typedef enum 
{
  MM_NO_IDENTITY_DEBUG =0,
  MM_IMSI_DEBUG = 1,
  MM_IMEI_DEBUG = 2, 
  MM_IMEISV_DEBUG = 3,
  MM_TMSI_DEBUG = 4,
  MM_PTMSI_RAI_PTMSI_SIG_DEBUG = 5
}mm_identity_type_debug_T;

typedef enum 
{
  GMM_GPRS_ATTACH_DEBUG =1,
  GMM_GPRS_ATTACH_WITH_IMSI_ATTACHED_DEBUG = 2,
  GMM_COMBINED_GPRS_IMSI_ATTACH_DEBUG = 3
}gmm_gprs_attach_type_debug_T;

typedef enum 
{
  GMM_GPRS_ONLY_ATTACHED_DEBUG =1,
  GMM_COMBINED_GPRS_IMSI_ATTACHED_DEBUG = 3,
}gmm_gprs_attach_resut_debug_T;

typedef enum 
{
  GMM_REATTACH_REQUIRED_DEBUG =1,
  GMM_REATTACH_NOT_REQUIRED_DEBUG = 2,
  GMM_MT_IMSI_DETACH_DEBUG = 3,
}gmm_mt_detach_type_debug_T;

typedef enum 
{
  GMM_GPRS_DETACH_DEBUG =1,
  GMM_MO_IMSI_DETACH_DEBUG = 2,
  GMM_COMBINED_GPRS_IMSI_DETACH_DEBUG = 3,
}gmm_mo_detach_type_debug_T;


typedef enum 
{
  GMM_RA_UPDATING_DEBUG =1,
  GMM_COMBINED_RA_LA_UPDATING_DEBUG = 2,
  GMM_COMBINED_RA_LA_UPDATING_WITH_IMSI_ATTACH_DEBUG = 3,
  GMM_PERIODIC_UPDATING_DEBUG = 4
}gmm_ra_update_value_debug_T;

typedef enum 
{
  GMM_RA_UPDATED_DEBUG = 0,
  GMM_COMBINED_RA_LA_UPDATED_DEBUG = 1,
  GMM_RA_UPDATED_AND_ISR_ACTIVATED_DEBUG = 4,
  GMM_COMBINED_RA_LA_UPDATED_AND_ISR_ACTIVATED_DEBUG = 5
}gmm_ra_update_result_debug_T;

typedef enum 
{
  GMM_SIGNALLING_DEBUG = 0,
  GMM_DATA_DEBUG = 1,
  GMM_PAGING_RESPONSE_DEBUG = 2
}gmm_service_request_type_debug_T;

typedef enum {
  EPS_ONLY_ATTACH_RESULT_DEBUG = 1,
  COMBINED_EPS_IMSI_ATTACH_RESULT_DEBUG = 2
}lte_nas_emm_eps_attach_result_debug_type;

typedef enum
{
  NAS_NO_ID_DEBUG=0,
  NAS_IMSI_DEBUG=1,
  NAS_IMEI_DEBUG=2,
  NAS_IMEISV_DEBUG=3,
  NAS_TMSI_DEBUG=4,
  NAS_TMGI_MBMS_DEBUG=5
}mobile_id_debug_type;

typedef enum
{
  TA_UPDATE_DEBUG = 0,
  TA_LA_UPDATE_DEBUG = 1,
  TA_UPDATE_ISR_ATIVATE_DEBUG = 4,
  TA_LA_UPDATE_ISR_ACTIVATE_DEBUG = 5
}lte_nas_emm_eps_update_result_debug_type;

typedef enum
{
  MM_LU_CAUSE_NORMAL_DEBUG = 0,
  MM_LU_CAUSE_PERIODIC_DEBUG = 1,
  MM_LU_CAUSE_IMSI_ATTACH_DEBUG = 2,
  MM_LU_CAUSE_RESERVED_DEBUG = 3
}mm_location_updating_debug_type;

typedef enum
{
  PROTOCOL_DISCRMINATOR_MM_DEBUG = 5,
  PROTOCOL_DISCRMINATOR_EMM_DEBUG = 7,
  PROTOCOL_DISCRMINATOR_GMM_DEBUG = 8,
}mm_protocol_discriminator_debug_type;


typedef enum{
  
  EMM_ATTACH_REQUEST_DEBUG                  =65,
  EMM_ATTACH_ACCEPT_DEBUG                   =66,
  EMM_ATTACH_COMPLETE_DEBUG                 =67,
  EMM_ATTACH_REJECT_DEBUG                   =68,
  EMM_DETACH_REQUEST_DEBUG                  =69,
  EMM_DETACH_ACCEPT_DEBUG                   =70,
  
  EMM_TRACKING_AREA_UPADTE_REQUEST_DEBUG    =72,
  EMM_TRACKING_AREA_UPDATE_ACCEPT_DEBUG     =73,
  EMM_TRACKING_AREA_UPDATE_COMPLETE_DEBUG   =74,
  EMM_TRACKING_AREA_REJECT_DEBUG            =75,
  
  EMM_EXT_SERVICE_REQUEST_DEBUG             =76,
  EMM_SERVICE_REJECT_DEBUG                  =78,
  EMM_GUTI_REALLOCATION_COMMAND_DEBUG       =80,
  EMM_GUTI_REALLOCATION_COMPLETE_DEBUG      =81,
  EMM_AUTHENTICATION_REQUEST_DEBUG      =82,
  EMM_AUTHENTICATION_RESPONSE_DEBUG     =83,
  EMM_AUTHENTICATION_REJECT_DEBUG       =84,
  
  EMM_AUTHENTICATION_FAILURE_DEBUG      =92,
  EMM_IDENTITY_REQUEST_DEBUG            =85,
  EMM_IDENTITY_RESPONSE_DEBUG           =86,
  EMM_SECURITY_MODE_COMMAND_DEBUG           =93,
  EMM_SECURITY_MODE_COMPLETE_DEBUG          =94,
  EMM_SECURITY_MODE_REJECT_DEBUG            =95,
  
  EMM_STATUS_DEBUG                     =96,
  EMM_INFORMATION_DEBUG                =97,
  EMM_DOWNLINK_NAS_TRANSPORT_DEBUG         =98,
  EMM_UPLINK_NAS_TRANSPORT_DEBUG           =99,
  EMM_CS_SERV_NOTIFICATION_DEBUG           =100,
  
  EMM_DL_GENERIC_NAS_TRANSPORT_DEBUG       =104,
  EMM_UL_GENERIC_NAS_TRANSPORT_DEBUG       =105,
  EMM_SERVICE_REQUEST_DEBUG                =127 /* used only for debug purposes */
}emm_message_id_debug_type; 

typedef enum
{
  MM_IMSI_DETACH_INDICATION_DEBUG      = 0x01,
  MM_LOCATION_UPDATING_ACCEPT_DEBUG    = 0x02,
  MM_LOCATION_UPDATING_REJECT_DEBUG    = 0x04,
  MM_LOCATION_UPDATING_REQUEST_DEBUG   = 0x08,
  MM_AUTHENTICATION_REJECT_DEBUG       = 0x11,
  MM_AUTHENTICATION_REQUEST_DEBUG      = 0x12,
  MM_AUTHENTICATION_RESPONSE_DEBUG     = 0x14,
  MM_IDENTITY_REQUEST_DEBUG            = 0x18,
  MM_IDENTITY_RESPONSE_DEBUG           = 0x19,
  MM_TMSI_REALLOCATION_COMMAND_DEBUG   = 0x1a,
  MM_TMSI_REALLOCATION_COMPLETE_DEBUG  = 0x1b,
  MM_AUTHENTICATION_FAILURE_DEBUG      = 0x1C,
  MM_CM_SERVICE_ACCEPT_DEBUG           = 0x21,
  MM_CM_SERVICE_REJECT_DEBUG           = 0x22,
  MM_CM_SERVICE_ABORT_DEBUG            = 0x23,
  MM_CM_SERVICE_REQUEST_DEBUG          = 0x24,
  MM_CM_SERVICE_PROMPT_DEBUG           = 0x25,
  MM_PAGING_RESPONSE_DEBUG             = 0x27,
  MM_CM_REESTABLISHMENT_REQUEST_DEBUG  = 0x28,
  MM_ABORT_DEBUG                       = 0x29,
  MM_STATUS_DEBUG                      = 0x31,
  MM_INFORMATION_DEBUG                 = 0x32
}mm_message_id_debug_type;

typedef enum
{
  GMM_ATTACH_REQUEST_DEBUG                         = 0x01,
  GMM_ATTACH_ACCEPT_DEBUG                          = 0x02,
  GMM_ATTACH_COMPLETE_DEBUG                        = 0x03,
  GMM_ATTACH_REJECT_DEBUG                          = 0x04,
  GMM_DETACH_REQUEST_DEBUG                         = 0x05,
  GMM_DETACH_ACCEPT_DEBUG                          = 0x06,

  GMM_ROUTING_AREA_UPDATE_REQUEST_DEBUG            = 0x08,
  GMM_ROUTING_AREA_UPDATE_ACCEPT_DEBUG             = 0x09,
  GMM_ROUTING_AREA_UPDATE_COMPLETE_DEBUG           = 0x0A,
  GMM_ROUTING_AREA_UPDATE_REJECT_DEBUG             = 0x0B,

  GMM_SERVICE_REQUEST_DEBUG                        = 0x0C,
  GMM_SERVICE_ACCEPT_DEBUG                         = 0x0D,
  GMM_SERVICE_REJECT_DEBUG                         = 0x0E,

  GMM_PTMSI_REALLOCATION_COMMAND_DEBUG             = 0x10,
  GMM_PTMSI_REALLOCATION_COMPLETE_DEBUG            = 0x11,
  GMM_AUTHENTICATION_AND_CIPHERING_REQUEST_DEBUG   = 0x12,
  GMM_AUTHENTICATION_AND_CIPHERING_RESPONSE_DEBUG  = 0x13,
  GMM_AUTHENTICATION_AND_CIPHERING_REJECT_DEBUG    = 0x14,
  GMM_AUTHENTICATION_AND_CIPHERING_FAILURE_DEBUG   = 0x1C,
  GMM_IDENTITY_REQUEST_DEBUG                       = 0x15,
  GMM_IDENTITY_RESPONSE_DEBUG                      = 0x16,
  GMM_STATUS_DEBUG                                 = 0x20,
  GMM_INFORMATION_DEBUG                            = 0x21
}gmm_message_id_debug_type;


/* Header File where it is defined as a macro -->  ms.h */
typedef enum
{
  MS_CC_MM_DEBUG          = 1 ,  /* messages between CC and MM          */
  MS_MN_CM_DEBUG          = 2 ,  /* messages between MN and Call Manager*/
  MS_MN_CC_DEBUG          = 3 ,  /* messages between MN and CC          */
  MS_MN_SS_DEBUG          = 4 ,  /* messages between MN and SS          */
  MS_MN_SMS_DEBUG         = 5 ,  /* messages between MN and SMS         */

  MS_TIMER_DEBUG          = 6 ,  /* Messages indicating timeouts        */

  MS_CM_REG_DEBUG         = 7 ,  /* messages between CM and REG         */
  MS_MN_REG_DEBUG         = 8 ,  /* messages between MN and REG         */
  MS_MM_RR_DEBUG          = 9 ,  /* messages between MM and RR          */
  MS_MM_REG_DEBUG         = 10 , /* messages between MM and REG         */
  MS_TEST_HARNESS_DEBUG   = 11 , /* messages between test harness tasks */
  MS_GHDI_EXPORT_DEBUG    = 12 , /* messages from/to GHDI               */

  MS_GSDI_DEBUG           = 13 , /* messages from/to GSDI               */

  MS_GHDI_LOCAL_DEBUG     = 14 , /* Private GHDI messages.              */
  MS_MN_TEST_DEBUG        = 15 , /* special MN test messages            */
  MS_CM_DS_DEBUG          = 16 , /* messages between CM and DS          */
  MS_RR_L2_DEBUG          = 17 , /* messages between RR and layer 2     */
  MS_RR_L1_DEBUG          = 18  /* messages between RR and layer 1     */
}ms_message_set_debug_type;


/* Header File where it is defined as a macro -->  sms_gmm.h */
typedef enum 
{
  MM_SMS_BEARER_SELECTION_CMD_DEBUG = 0x01
}sms_gmm_message_id_debug_type;


/* Header File where it is defined as a macro -->  ms_cc_mm_v.h */
typedef enum
{
/*
* Messages from CC to  MM
*/

  MMCNM_DATA_REQ_DEBUG                      = 0x00,
  MMCNM_EST_REQ_DEBUG                       = 0x01,
  MMCC_REEST_REQ_DEBUG                      = 0x02,
  MMCNM_REL_REQ_DEBUG                       = 0x03,
  MMCNM_UNIT_DATA_REQ_DEBUG                 = 0x04,
  #ifdef FEATURE_CCBS
  MMCNM_PROMPT_RES_DEBUG                    = 0x05,
  #endif /* FEATURE_CCBS */

/*
* messages from MM to CC
*/

  MMCNM_DATA_IND_DEBUG                      = 0x80,
  MMCNM_EST_REJ_DEBUG                       = 0x81, 
  MMCNM_EST_CNF_DEBUG                       = 0x82,
  MMCNM_REL_IND_DEBUG                       = 0x83,
  MMCNM_REL_CNF_DEBUG                       = 0x84,
  MMCC_REEST_CNF_DEBUG                      = 0x85,
  MMCC_SYNC_IND_DEBUG                       = 0x86,
  MMCNM_UNIT_DATA_IND_DEBUG                 = 0x87,
  MMCC_REEST_REJ_DEBUG                      = 0x88,
  #ifdef FEATURE_CCBS
  MMCNM_PROMPT_IND_DEBUG                    = 0x89,
  #endif /* FEATURE_CCBS */
  MMCC_SIG_CONN_REL_IND_DEBUG               = 0x90,
  MMCNM_SRVCC_HANDOVER_COMPLETE_IND_DEBUG   = 0x91
}ms_cc_mm_v_message_id_debug_type;


/* Header File where it is defined as a macro -->  timers_v.h */
typedef enum
{
  TIMER_UNDEFINED_DEBUG                = ((timer_id_T)0xffffffff),
  ALL_TIMERS_DEBUG                     = 0,
  TIMER_T303_DEBUG                     = 1,
  TIMER_T305_DEBUG                     = 2,
  TIMER_T308_DEBUG                     = 3,
  TIMER_T310_DEBUG                     = 4,
  TIMER_T313_DEBUG                     = 5,
  TIMER_T323_DEBUG                     = 6,
  TIMER_T324_DEBUG                     = 7,
  TIMER_T336_DEBUG                     = 8,
  TIMER_T337_DEBUG                     = 9,
  TIMER_TC1M_MO_DEBUG                  = 10,
  TIMER_TC1M_MT_DEBUG                  = 11,
  TIMER_RAB_RELEASED_DEBUG             = 12,

/* MM Timers begin Here */

  TIMER_T3210_DEBUG                    = 13,
  TIMER_T3211_DEBUG                    = 14,
  TIMER_T3212_DEBUG                    = 15,
  TIMER_T3213_DEBUG                    = 16,
  TIMER_T3220_DEBUG                    = 17,
  TIMER_T3230_DEBUG                    = 18,
  TIMER_T3240_DEBUG                    = 19,
  TIMER_T3246_DEBUG                    = 20,
  TIMER_MM_REL_DEBUG                   = 21,
  TIMER_MM_FORBIDDEN_LAI_CLEAR_DEBUG   = 22,
  TIMER_T3214_DEBUG                    = 23,
  TIMER_T3216_DEBUG                    = 24,
  TIMER_T3302_DEBUG                    = 25,
  TIMER_T3310_DEBUG                    = 26,
  TIMER_T3311_DEBUG                    = 27,
  TIMER_T3312_DEBUG                    = 28,
  TIMER_T3314_DEBUG                    = 29,
  TIMER_T3317_DEBUG                    = 30,
  TIMER_T3318_DEBUG                    = 31,
  TIMER_T3320_DEBUG                    = 32,
  TIMER_T3321_DEBUG                    = 33,
  TIMER_T3330_DEBUG                    = 34,
  TIMER_POWERDOWN_DEBUG                = 35,
  CNM_MM_REL_TIMER_DEBUG               = 36,
  TIMER_DETACH_REL_DEBUG               = 37,
  TIMER_T3241_DEBUG                    = 38,

  TIMER_T3218_DEBUG                    = 39,
  TIMER_T3316_DEBUG                    = 40,

  TIMER_T3319_DEBUG                    = 41,

/*This is network timer which will be started 
 after it has sent RAU accept or Attach accept.
 Startng this timer in UE to take care of not to accept
 Attach accept or RAU accept when this timer is
 not running.*/

  TIMER_T3350_DEBUG                    = 42,
  TIMER_T3242_DEBUG                    = 43,
  TIMER_T3243_DEBUG                    = 44,
  TIMER_T3323_DEBUG                    = 45,

  TIMER_MM_RADIO_RETRY_DEBUG           = 46,
  TIMER_GMM_RADIO_RETRY_DEBUG          = 47,

  TIMER_SCRI_PAGING_DEBUG              = 48,

  TIMER_FOREGROUND_SEARCH_DEBUG        = 49,

  TIMER_MM_PS_DETACH_DEBUG          = 50,

//network initiated detach timer
  TIMER_NW_INIT_DETACH_DEBUG           = 51,

  TIMER_T3340_DEBUG                    = 52,

/* common to MM and EMM */
  TIMER_T3346_DEBUG                    = 53,

  TIMER_FORBIDDEN_OPERATOR_CSG_LIST_DEBUG =54,
  TIMER_FORCE_OOS_DEBUG                 = 55,
  TIMER_FORBID_TDS_RESELECTION_DEBUG     = 56,
  TIMER_CSMT_GUARD_DEBUG                 = 57,
  TIMER_PS_CONN_REL_DEBUG                = 58,
  TIMER_MM_DUAL_SWITCH_DEBUG             = 59,

/* Spec 24.008, Spec CRs 2488, 2937
 * If the service request attempt counter is greater than or equal to 5, 
 * the MS shall not attempt service request for TIMER_T3325 unless it 
 * is in response to paging from the network.
 * On expiry of this 1 min timer, reset the service attempt counter
 */
  TIMER_T3325_DEBUG                     = 60,

  TIMER_POLICY_CHANGE_PENDING_DEBUG     = 61,
/* MM Timers END Here */
/* EMM timers */
  TIMER_T3410_DEBUG                     = 70,
  TIMER_T3417_DEBUG                     = 71,
  TIMER_T3421_DEBUG                     = 72,
  TIMER_T3418_DEBUG                     = 73,
  TIMER_T3420_DEBUG                     = 74,
  TIMER_T3416_DEBUG                     = 75,
  TIMER_T3411_DEBUG                     = 76,
  TIMER_T3402_DEBUG                     = 77,
  TIMER_T3412_DEBUG                     = 78,
  TIMER_T3430_DEBUG                     = 79,
  TIMER_T3440_DEBUG                     = 80,
  TIMER_T3423_DEBUG                     = 81,
  TIMER_T3417_EXT_DEBUG                 = 82,

  EMM_POWEROFF_DETACH_TIMER_DEBUG       = 84, 
  EMM_ACCESS_BARRING_TIMER_DEBUG        = 85,
  TIMER_EMM_PS_DETACH_DEBUG             = 86,
  EMM_FORBIDDEN_TAI_CLEAR_TIMER_DEBUG   = 87,

  TIMER_T3442_DEBUG                     = 88,
  TIMER_EMM_RADIO_RETRY_DEBUG           = 89,
  TIMER_SRLTE_ESR_DEBUG                 = 90,
  TIMER_DELAY_TAU_DEBUG                 = 91,
  TIMER_T3425_DEBUG                     = 92,

  TIMER_EMC_BACKOFF_DEBUG               = 93,
  EMM_MT_PAGE_EXT_PRI_TIMER_DEBUG       = 94,
}timer_ids_message_id_debug_type;


/* Header File where it is defined as a macro -->  ms_timer.h */
typedef enum
{
  MID_START_TIMER_REQUEST_DEBUG      = 0x01,
  MID_MODIFY_TIMER_REQUEST_DEBUG     = 0x02,
  MID_TIMER_EXPIRY_DEBUG             = 0x03,
  MID_TIMER_ENQUIRY_DEBUG            = 0x04
}ms_timer_message_id_debug_type;


/* Header File where it is defined as a macro -->  cause.h */ 
typedef enum
{
  REASON_NOT_AVAILABLE_DEBUG                  = 0x00, 
  ACCESS_CLASS_NOT_ALLOWED_DEBUG              = 0x01, 
  RANDOM_ACCESS_FAILURE_DEBUG                 = 0x02, 
  OTHER_REASON_DEBUG                          = 0x03, 
  RR_CONNECTION_FAILURE_DEBUG                 = 0x04, 
  REASON_INTERRAT_RESELECTION_DEBUG           = 0x05, 
  RR_ABORT_REDIAL_NOT_ALLOWED_DEBUG           = 0x06, 
  RR_ABORT_LAI_CHANGED_DEBUG                  = 0X07, 
  RR_ABORT_GTOW_REDIRECTION_IN_PROGRESS_DEBUG = 0x08, 
  RR_ABORT_IA_REJECT_RECEIVED_DEBUG           = 0x09, 
  RR_ABORT_CON_EST_NOT_ATTEMPTED_DEBUG        = 0x0A
}cause_message_id_debug_type;


/* Header File where it is defined as an anonymous enum -->  emm_ext_msg.h  */
/* 
    The first variable in the enum will be of the name NAS_EMM_IND_PLMN_CHANGE_DEBUG 
    && another variable is of name NAS_EMM_PLMN_CHANGE_DEBUG_IND
    This is consistent with the variables in the orignial header file (emm_ext_msg.h) of name NAS_EMM_IND_PLMN_CHANGE 
    && NAS_EMM_PLMN_CHANGE_IND
 */
typedef enum
{
  MSGR_DEFINE_UMID(NAS, EMM, IND, PLMN_CHANGE_DEBUG, 0x05, emm_plmn_change_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND,ATTACH_COMPLETE_DEBUG, 0x06, emm_attach_complete_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, T3402_CHANGED_DEBUG, 0x0C, emm_t3402_changed_ind_s_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, DEACT_NON_EMC_BEARER_DEBUG, 0x0D, emm_deact_non_emc_bearer_ind_s_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, RESET_APN_SWITCH_DEBUG, 0x0E, emm_reset_apn_switch_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, TAI_LIST_DEBUG, 0x0F, emm_tai_list_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, TAU_COMPLETE_DEBUG, 0x10, emm_tau_complete_ind_type)
}emm_ext_msg_umids_debug_type;


/* Header File where it is defined as an anonymous enum -->  emm_gps_if_msg.h  */
/* 
    The first variable in the enum will be of the name NAS_EMM_REQ_UL_GENERIC_NAS_TRANSPORT_DEBUG
    && another variable is of name NAS_EMM_UL_GENERIC_NAS_TRANSPORT_DEBUG_REQ
    This is consistent with the variables in the orignial header file (emm_gps_if_msg.h) of name NAS_EMM_REQ_UL_GENERIC_NAS_TRANSPORT
    && NAS_EMM_UL_GENERIC_NAS_TRANSPORT_REQ
 */
typedef enum
{
  MSGR_DEFINE_UMID(NAS, EMM, REQ, UL_GENERIC_NAS_TRANSPORT_DEBUG, 0x07, lte_nas_emm_ul_generic_transport_s_type ), /*UL_GENERIC_NAS_TRANSPORT*/
  MSGR_DEFINE_UMID(NAS, EMM, IND, DL_GENERIC_NAS_TRANSPORT_DEBUG, 0x09, lte_nas_emm_dl_generic_transport_s_type ), /*DL_GENERIC_NAS_TRANSPORT*/
  MSGR_DEFINE_UMID(NAS, EMM, CNF, UL_GENERIC_NAS_TRANSPORT_DEBUG, 0x0A, lte_nas_emm_ul_generic_transport_cnf_type ), /*UL_GENERIC_NAS_TRANSPORT_CNF*/
  MSGR_DEFINE_UMID(NAS, EMM, IND, GENERIC_NAS_TRANSPORT_FAILURE_DEBUG, 0x0B, emm_irat_failure_ind_type),
} emm_gps_msg_umids_debug_type;


/* Header File where it is defined as an anonymous enum -->  emm_irat_if_msg.h  */
/* 
    The first variable in the enum will be of the name NAS_EMM_REQ_IRAT_UL_MSG_DEBUG
    && another variable is of name NAS_EMM_IRAT_UL_MSG_DEBUG_REQ
    This is consistent with the variables in the orignial header file (emm_irat_if_msg.h) of name NAS_EMM_REQ_IRAT_UL_MSG
    && NAS_EMM_IRAT_UL_MSG_REQ
 */
typedef enum
{
  MSGR_DEFINE_UMID(NAS, EMM, REQ, IRAT_UL_MSG_DEBUG,      0x03, emm_irat_ul_msg_req_type),
  MSGR_DEFINE_UMID(NAS, EMM, REQ, IRAT_HDR_UL_MSG_DEBUG,  0x04, emm_irat_3gpp2_ul_msg_req_type),
  MSGR_DEFINE_UMID(NAS, EMM, CNF, IRAT_UL_MSG_DEBUG,      0x01, emm_irat_ul_msg_cnf_type),
  MSGR_DEFINE_UMID(NAS, EMM, CNF, IRAT_HDR_UL_MSG_DEBUG,  0x02, emm_irat_3gpp2_ul_msg_cnf_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, IRAT_DL_MSG_DEBUG,      0x01, emm_irat_dl_msg_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, IRAT_IMSI_ATTACH_DEBUG, 0x02, emm_irat_imsi_attach_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, IRAT_CTXT_LOST_DEBUG,   0x03, emm_irat_ctxt_lost_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, IRAT_FAILURE_DEBUG,     0x04, emm_irat_failure_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, IRAT_HDR_FAILURE_DEBUG, 0x07, emm_irat_failure_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, IRAT_HDR_DL_MSG_DEBUG,  0x08, emm_irat_3gpp2_dl_msg_ind_type),
}emm_irat_msg_umids_debug_type;


/* Header File where it is defined as an anonymous enum -->  emm_esm_if_msg.h */
/* 
    The first variable in the enum will be of the name NAS_EMM_REQ_SERVICE_DEBUG
    && another variable is of name NAS_EMM_SERVICE_DEBUG_REQ
    This is consistent with the variables in the orignial header file (emm_esm_if_msg.h) of name NAS_EMM_REQ_SERVICE
    && NAS_EMM_SERVICE_REQ
 */
typedef enum
{
  MSGR_DEFINE_UMID(NAS, EMM, REQ, SERVICE_DEBUG, 0x01, emm_service_req_T ),
  MSGR_DEFINE_UMID(NAS, EMM, REQ, DATA_DEBUG, 0x02, emm_data_req_T ),
  MSGR_DEFINE_UMID(NAS, EMM, CMD, DETACH_DEBUG, 0x03, emm_detach_cmd_T ),
  MSGR_DEFINE_UMID(NAS, EMM, CMD, EPS_BEARER_STATUS_DEBUG, 0x04, emm_eps_bearer_status_cmd_T ),
  MSGR_DEFINE_UMID(NAS, EMM, REQ, 1XCSFB_ESR_CALL_DEBUG, 0x05, emm_1xCSFB_esr_call_req_T ),
  MSGR_DEFINE_UMID(NAS, EMM, REQ, 1XCSFB_ESR_CALL_ABORT_DEBUG , 0x06, emm_1xCSFB_esr_call_abort_req_T ),
#ifdef FEATURE_LTE_REL9
  MSGR_DEFINE_UMID(NAS, EMM, CMD, EMC_SRV_STATUS_DEBUG, 0x07, emm_emc_srv_status_cmd_T),
#endif
#ifdef FEATURE_DUAL_SIM
  MSGR_DEFINE_UMID(NAS, EMM, CMD, PS_CALL_STATUS_DEBUG , 0x08, emm_ps_call_status_ind_cmd_T )
#endif
}emm_esm_msg_umids_debug_type ;


/* Header File where it is defined as an anonymous enum -->  esm_emm_msgr_enum_msg.h */
/* 
    The first variable in the enum will be of the name NAS_EMM_IND_DATA_DEBUG
    && another variable is of name NAS_EMM_DATA_DEBUG_IND
    This is consistent with the variables in the orignial header file (esm_emm_msgr_enum_msg.h) of name NAS_EMM_IND_DATA
    && NAS_EMM_DATA_IND
 */
typedef enum
{
  NAS_ESM_IND_FIRST_DEBUG = MSGR_UMID_BASE(MSGR_NAS_ESM, MSGR_TYPE_IND),

  MSGR_DEFINE_UMID(NAS, ESM, IND, DATA_DEBUG, 0x01, esm_data_ind_T ),
  MSGR_DEFINE_UMID(NAS, ESM, IND, FAILURE_DEBUG, 0x02,esm_failure_ind_T ),
  MSGR_DEFINE_UMID(NAS, ESM, IND, SIG_CONNECTION_RELEASED_DEBUG, 0x03, esm_sig_connection_released_ind_T ),
  MSGR_DEFINE_UMID(NAS, ESM, IND, ACTIVE_EPS_DEBUG, 0x04, esm_active_eps_ind_T ),
  MSGR_DEFINE_UMID(NAS, ESM, IND, DETACH_DEBUG, 0x05, esm_detach_ind_T ),
  MSGR_DEFINE_UMID(NAS, ESM, IND, EPS_BEARER_STATUS_DEBUG, 0x06, esm_eps_bearer_status_ind_T ),
  MSGR_DEFINE_UMID(NAS, ESM, IND, GET_PDN_CONNECTIVITY_REQ_DEBUG, 0x07, esm_get_pdn_connectivity_req_T),
  MSGR_DEFINE_UMID(NAS, ESM, IND, GET_ISR_STATUS_DEBUG, 0x08, esm_get_isr_status_ind_T),
  MSGR_DEFINE_UMID(NAS, ESM, IND, ISR_STATUS_CHANGE_DEBUG, 0x0a, esm_isr_status_change_ind_T),
  MSGR_DEFINE_UMID(NAS, ESM, RSP,1XCSFB_ESR_CALL_DEBUG, 0x0b, emm_1xCSFB_esr_call_rsp_T),
  MSGR_DEFINE_UMID(NAS, ESM, RSP,1XCSFB_ESR_CALL_ABORT_DEBUG, 0x0c, emm_1xCSFB_esr_call_abort_rsp_T),
#ifdef FEATURE_LTE_REL10  
  MSGR_DEFINE_UMID(NAS, ESM, IND, UNBLOCK_ALL_APNS_DEBUG, 0x0d, esm_unblock_all_apns_ind_T),
#endif
  NAS_ESM_IND_MAX_DEBUG,
  NAS_ESM_IND_LAST_DEBUG = NAS_ESM_IND_MAX_DEBUG - 1
} esm_emm_msgr_enum_msg_umids_debug_type;


typedef enum
{
  MM_GMM_NO_FAILURE_DEBUG                               = -1,
  IMSI_UNKNOWN_IN_HLR_DEBUG                             = 0x02,
  ILLEGAL_MS_DEBUG                                      = 0x03,
  IMSI_UNKNOWN_IN_VLR_DEBUG                             = 0x04,
  IMEI_NOT_ACCEPTED_DEBUG                               = 0x05,
  ILLEGAL_ME_DEBUG                                      = 0x06,
  GPRS_SERVICES_NOT_ALLOWED_DEBUG                       = 0x07,
  GPRS_SERVICES_AND_NON_GPRS_SERVICES_NOT_ALLOWED_DEBUG = 0x08,
  MS_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK_DEBUG    = 0x09,
  IMPLICITLY_DETACHED_DEBUG                             = 0x0A,
  PLMN_NOT_ALLOWED_DEBUG                                = 0x0B,
  LA_NOT_ALLOWED_DEBUG                                  = 0x0C,
  NATIONAL_ROAMING_NOT_ALLOWED_DEBUG                    = 0x0D,
  GPRS_SERVICES_NOT_ALLOWED_IN_THIS_PLMN_DEBUG          = 0x0E,
  NO_SUITABLE_CELLS_IN_LA_DEBUG                         = 0x0F,
  MSC_TEMPORARILY_NOT_REACHABLE_DEBUG                   = 0x10,
  NETWORK_FAILURE_DEBUG                                 = 0x11,
  MAC_FAILURE_DEBUG                                     = 0x14,
  SYNCH_FAILURE_DEBUG                                   = 0x15,
  CONGESTTION_DEBUG                                     = 0x16,
  GSM_AUTH_UNACCEPTED_DEBUG                             = 0x17,
  CSG_NOT_AUTHORIZED_DEBUG                              = 0x19,
  SERVICE_OPTION_NOT_SUPPORTED_DEBUG                    = 0x20,
  REQ_SERV_OPT_NOT_SUBSCRIBED_DEBUG                     = 0x21,
  SERVICE_OPT__OUT_OF_ORDER_DEBUG                       = 0x22,
  CALL_CANNOT_BE_IDENTIFIED_DEBUG                       = 0x26,
  NO_PDP_CONTEXT_ACTIVATED_DEBUG                        = 0x28,
  RETRY_UPON_ENTRY_INTO_A_NEW_CELL_MIN_VALUE_DEBUG      = 0x30,
  RETRY_UPON_ENTRY_INTO_A_NEW_CELL_MAX_VALUE_DEBUG      = 0x3F,
  SEMANTICALLY_INCORRECT_MSG_DEBUG                      = 0x5F,
  INVALID_MANDATORY_INFO_DEBUG                          = 0x60,
  MESSAGE_TYPE_NON_EXISTANT_DEBUG                       = 0x61,
  MESSAGE_TYPE_NOT_COMP_PRT_ST_DEBUG                    = 0x62,
  IE_NON_EXISTANT_DEBUG                                 = 0x63,
  MSG_NOT_COMPATIBLE_PROTOCOL_STATE_DEBUG               = 0x65
}mm_gmm_reject_cause_debug_type;

typedef enum
{
  LTE_NAS_CAUSE_NONE_DEBUG  = -1,
  LTE_NAS_IMSI_UNKNOWN_IN_HSS_DEBUG  = 2,
  LTE_NAS_ILLEGAL_UE_DEBUG  = 3,
  LTE_NAS_IMEI_NOT_ACCEPTED_DEBUG  = 5,
  LTE_NAS_ILLEGAL_ME_DEBUG  = 6,
  LTE_NAS_EPS_SERVICES_NOT_ALLOWED_DEBUG  = 7,
  LTE_NAS_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED_DEBUG  = 8,
  LTE_NAS_UE_ID_CANNOT_BE_DERIVED_BY_NW_DEBUG  = 9,
  LTE_NAS_IMPLICITLY_DETACHED_DEBUG  = 10,
  LTE_NAS_PLMN_NOT_ALLOWED_DEBUG  = 11,
  LTE_NAS_TRACKING_AREA_NOT_ALLOWED_DEBUG  =12,
  LTE_NAS_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA_DEBUG =13,
  LTE_NAS_EPS_SERVICES_NOT_ALLOWED_IN_PLMN_DEBUG  =14,
  LTE_NAS_NO_SUITABLE_CELLS_IN_TRACKING_AREA_DEBUG =15,
  LTE_NAS_MSC_TEMPORARILY_NOT_REACHABLE_DEBUG =16,
  LTE_NAS_NW_FAILURE_DEBUG =17,
  LTE_NAS_CS_DOMAIN_NOT_AVAILABLE_DEBUG =18,
  LTE_NAS_ESM_FAILURE_DEBUG =19,
  LTE_NAS_MAC_FAILURE_DEBUG =20,
  LTE_NAS_SYNCH_FAILURE_DEBUG =21,
  LTE_NAS_CONGESTION_DEBUG =22,
  LTE_NAS_UE_SECURITY_CAPABILITIES_MISMATCH_DEBUG =23,
  LTE_NAS_SECURITY_MODE_REJ_UNSPECIFIED_DEBUG  =24,
  LTE_NAS_NOT_AUTHORIZED_FOR_THIS_CSG_DEBUG =25,
  LTE_NON_EPS_AUTH_UNACCEPTABLE_DEBUG  = 26,
  LTE_NAS_CS_FALLBACK_CALL_EST_NOT_ALLOWED_DEBUG  = 38,
  LTE_NAS_CS_DOMAIN_TEMP_NOT_AVAILABLE_DEBUG = 39,
  LTE_NAS_NO_EPS_BEARER_CONTEXT_ACTIVATED_DEBUG =40,
  LTE_NAS_SEVERE_NW_FAILURE_DEBUG =42,
  LTE_NAS_SEMANTICALLY_INCORRECT_MSG_DEBUG=95,
  LTE_NAS_INVALID_MANDATORY_INFO_DEBUG =96,
  LTE_NAS_NON_EXISTENT_MSG_TYPE_DEBUG=97,
  LTE_NAS_MSG_TYPE_INCOMPATIBLE_WITH_PROTOCOL_STATE_DEBUG=98,
  LTE_NAS_NON_EXISTANT_INFO_DEBUG=99,
  LTE_NAS_CONDITIONAL_IE_ERROR_DEBUG =100,
  LTE_NAS_MSG_INCOMPATIBLE_WITH_PROTOCOL_STATE_DEBUG=101,
  LTE_NAS_PROTOCOL_ERROR_UNSPECIFIED_DEBUG =111
}lte_nas_emm_cause_debug_type;





/*===========================================================================

                        TYPES DEFINITION

===========================================================================*/


typedef struct emm_debug_rrc_service_ind_tag
{
  uint32 trans_id;
  lte_rrc_svc_status_e svc_status;
  lte_rrc_plmn_s selected_plmn;
  uint16 tac;
  boolean ims_emergency_support_enabled;
  sys_no_svc_cause_e_type no_svc_cause;
  boolean sib8_available;
  boolean detach_reqd;
  sys_acq_status_e_type  acq_status;
  boolean skip_acb_for_volte_call;
}emm_rrc_service_ind_debug_type;

typedef struct serving_plmn_info_debug
{
  sys_plmn_service_state_s_type     info;
  location_area_code_T              lac;
  routing_area_code_T               rac;
  service_state_debug_type          mm_service_state;
  service_state_debug_type          gmm_service_state;
  network_op_mode_type              network_op_mode;
  sys_srv_domain_e_type             ms_op_mode;
  boolean                           update_equiv_plmn_list;
  byte                              equiv_plmn_list_length;
  sys_plmn_id_s_type                equiv_plmn_list_plmn[EQUIVALENT_PLMN_LIST_MAX_LENGTH_DEBUG];
#ifdef FEATURE_GSM_DTM
  boolean                           dtm_supported;
#endif /* FEATURE_GSM_DTM */
  boolean                           egprs_supported;
  sys_hs_ind_e_type                 hsdpa_hsupa_ind;
  boolean                           t3122_running;
  ARFCN_T                           arfcn;
  uint8                             bsic;
} serving_plmn_info_debug_T;

typedef struct mm_msg_head_debug_tag
{
   IMH_T IMH;        /* Intertask Message Header                     */
   byte  skip_pd;    /* skip indicator, protocol discrim.            */
   byte  type;       /* Message type                                 */
} mm_msg_head_debug_T;

typedef struct classmark_2_debug_tag
{
   byte        iei;
   byte        length;
   byte        data[3];
} ie_classmark_2_debug_T;

typedef struct ie_mobile_identity_debug_tag
{
   byte        iei;
   byte        length;
   byte        data[9];
} ie_mobile_identity_debug_T;



typedef struct cm_service_request_msg_debug_tag
{
   mm_msg_head_debug_T msg_head;
   byte              cipher_key_seq_no_service_type;
   byte              data[
                        (sizeof( ie_classmark_2_debug_T ) - 1) +    /* no type */
                        (sizeof( ie_mobile_identity_debug_T ) - 1) +  /* no type */
                        1    /*Additional Update parameter*/                      
                         ];
} cm_service_request_msg_debug_T;


typedef struct held_service_request_debug_tag
{
   boolean                           present;
   cm_service_request_msg_debug_T    message;
   byte                              establishment_cause;
   auth_security_context_id_debug    security_context ;
   mm_as_status_T                    interat_status ;
   boolean                           include_additional_update_ie;
} held_service_request_debug_T;






/******************************************************************************
**                     EMM DEBUG DATA
******************************************************************************/
typedef struct emm_debug_data_s
{
  emm_eps_update_status_debug_type    emm_update_status;
  emm_state_debug_type                emm_state;
  dword                               emm_substate;
  emm_connection_state_debug_type     emm_connection_state;
  dword                         t3402_value;
  boolean                       is_t3402_recv_attach_rej; //emm_t3402_attach_rej_s_type.is_t3402_recv_attach_rej
  sys_plmn_id_s_type            t3402_attach_rej_plmn;   //is_t3402_recv_attach_rej.t3402_attach_rej_plmn
  dword                         t3423_value;
  dword                         t3442_value;
  dword                         access_barring_flag;
  boolean                       mmtel_voice_call_status;
  boolean                       mmtel_video_call_status;
  boolean                       mmtel_sms_status;
  byte                          emm_attach_attempt_counter;
  byte                          emm_attach_rej_esm_fail_counter;
  byte                          emm_tau_attempt_counter;
  boolean                       eps_bearer_context_status_updated;
  lte_nas_eps_bearer_context_status_type eps_bearer_context_status;
  lte_nas_emm_eps_update_val_debug_type emm_ongoing_eps_ta_update;
  byte                          emm_detach_attempt_counter;
  emm_mo_detach_debug_type      ongoing_detach_type;
  boolean                       ongoing_detach_procedure;
  emm_detach_reason_debug_type  detach_reason;
  boolean                       emm_guti_valid;
  ue_id_debug_type              emm_guti_ue_id;
  sys_plmn_id_s_type            emm_guti_plmn;
  byte                          emm_guti_mme_group_id[2];
  byte                          emm_guti_mme_code;
  byte                          emm_guti_m_tmsi[4];
  word                          access_class;
  boolean                       last_registered_tai_valid;
  sys_plmn_id_s_type            last_registered_tai_plmn;
  word                          last_registered_tai_tac;
  byte                          emm_location_area_tmsi[TMSI_SIZE];
  byte                          emm_location_area_lai[LAI_SIZE];
  byte                          emm_location_area_tmsi_time;
  byte                          emm_location_area_location_update_status;
  emm_rrc_service_ind_debug_type last_rrc_service_ind;
  lte_rrc_est_cause_e           service_req_cause;
  dword                         pdn_conn_req_trans_id;
  sys_stop_mode_reason_e_type   rrc_deact_req_deact_reason;
  boolean                       rrc_deact_req_deact_req_pending;
  boolean                       rrc_deact_req_deact_cnf_pending;
  lte_nas_tau_cause_debug_type  tau_cause;
  boolean                       start_t3440_on_accept;
  boolean                       eps_storage_supported;
  nas_mm_tin_debug_type         TIN;
  boolean                       ignore_GW_activation_ind;
  lte_nas_emm_cause_type        combined_attach_cs_rej_cause;
  boolean                       emm_sms_active;
  boolean                       tau_pending_gw_attach;
  boolean                       aborting_conn;
  lte_nas_emm_mt_detach_debug_type mt_detach_info_type;
  lte_nas_emm_cause_type        mt_detach_info_emm_cause;
  sys_plmn_id_s_type            t3346_plmn;
  sys_plmn_id_s_type            last_attempted_tai_plmn;
  word                          last_attempted_tai_tac;
  boolean                       is_srvcc_in_progress;
  emm_esr_debug_type            esr_type_in_progress;
  sys_srv_domain_e_type         srv_domain_registered;
  lte_3gpp_release_ver_e        emm_3gpp_spec_ver;
  sg_association_to_be_established_debug_enum_T sg_association_to_be_established;
  boolean                       rrc_active;
  lte_nas_emm_attach_debug_type eps_attach_type;
  mmr_cause_e_type              last_reg_cause;
  mmr_ps_detach_req_s_type      ps_detach_info_ps_detach_req;
  emm_ps_detach_debug_status    ps_detach_info_status;
  boolean                       cell_barred_tau_pending;
  boolean                       manual_user_sel;
  lte_nas_emc_srv_status_debug_type emc_srv_status;
  boolean                       non_emc_bearer_deact_in_progress;
  boolean                       emm_ph_status_change_cnf_pending;
  boolean                       srvcc_capability_support;
  boolean                       onexsrvcc_capability_support;
  boolean                       l2g_srvcc_capability_support;
  boolean                       w2l_psho;
  emm_l2gcco_in_progress_debug_enum_T l2gcco_state;
  boolean                       sync_drb_status;
  boolean                       is_accepted_on_lte;
  boolean                       suitable_search_end_pending;
  emm_pending_reg_ack_debug_enum_T    reg_complete_ack_pending;
  boolean                       is_plmn_block_req_pending;
  boolean                       emm_lte_mode_change_pending;
  sys_proc_type_e_type          highest_proc_type;
  boolean                       reg_lte_cs_domain_param_is_voice_domain_ie_valid;
  sys_ue_usage_setting_e_type   reg_lte_cs_domain_param_voice_or_data_centric;
  sys_voice_domain_pref_e_type  reg_lte_cs_domain_param_voice_domain_pref;
  boolean                       plmn_search_in_progress;
  emm_ps_resumption_status_debug_enum_T ps_resumption_status;
  boolean                       rr_blind_redir_pending_call_abort;
  sys_plmn_id_s_type            rlf_plmn;
}emm_debug_data_s_type ;



/******************************************************************************
**                     MM DEBUG DATA
******************************************************************************/
typedef struct mm_debug_data_s
{
  boolean                            gmm_detach_after_rau_pending_sim_debug;
  boolean                            gmm_detach_in_progress_sim_debug;
  boolean                            gmm_follow_on_request_sim_debug;
  boolean                            gmm_gprs_attach_pending_sim_debug;
  boolean                            gmm_gprs_detach_pending_sim_debug;
  boolean                            gmm_integrity_protection_activated_sim_debug;
  boolean                            gmm_is_lte_advertised_in_last_reg_sim_debug;
  boolean                            gmm_is_prev_mode_connected_sim_debug;
  boolean                            gmm_is_suspended_sim_debug;
  boolean                            gmm_power_down_active_sim_debug;
  gmm_state_debug_type               gmm_state_sim_debug;
  gmm_substate_debug_type            gmm_substate_sim_debug;
  lte_nas_paging_identity_debug_type mm_csfb_mt_call_info_ue_identity;
  boolean                            mm_csfb_mt_call_info_mt_csfb_highPriority;
  boolean                            mm_csfb_mt_call_info_debug_is_mt_call_recv_in_conn_state;
  mm_csfb_state_debug_type           mm_csfb_state_sim_debug;
  sys_radio_access_tech_e_type       mm_service_loss_on_multimode_rat_sim_debug;
  int8                               mm_waiting_for_regs_complete_txn_sim_debug;
  boolean                            mm_dual_standby_mode_debug;
  boolean                            mm_lu_pended_nmo_unknown_sim_debug;
  mm_ps_detach_state_debug_enum_T    mm_ps_detach_state_sim_debug;
  mm_rlf_stop_mode_pending_debug_enum_T mm_rlf_stop_mode_pending_sim_debug;
  boolean                            mm_stop_mode_req_pended_sim_debug;
  uint32                              mm_subs_capability_mask_sim_debug;
  boolean                            mm_waiting_for_abort_cnf_sim_debug;
  boolean                            mm_waiting_for_open_session_cnf_sim_debug;
  wcdma_rlf_state_debug_enum_T       mm_wcdma_rlf_state_sim_debug;
  boolean                            mm_wrlf_rat_change_pending_sim_debug;
  boolean                            mmcoord_reg_req_pending_sim_debug;
  call_type_for_rrc_est_req_T        call_type_for_rrc_est_req_sim_debug;
  WtoGCCO_in_progress_debug_enum_T   GtoWCCO_state_sim_debug;
  sys_plmn_id_s_type                 mm_3gpp_t3246_plmn_id_sim_debug;
  sys_stop_mode_reason_e_type        mm_abort_dual_switch_pending_sim_debug;
  sys_modem_as_id_e_type             mm_as_id_debug;
  boolean                            mm_cnm_is_active_sim_debug;
  boolean                            mm_connection_activated_sim_debug;
  mm_connection_debug_type_T         mm_connection_type_sim_debug;
  boolean                            mm_csfb_is_blind_redir_to_lte_req_to_be_sent_sim_debug;
  boolean                            mm_deep_sleep_pending_sim_debug;
  boolean                            mm_hard_abort_pending_sim_debug;
  held_service_request_debug_T       mm_held_cnm_service_req_sim_debug;
  byte                               mm_idle_substate_sim_debug;
  sys_radio_access_tech_e_type       mm_last_active_rat_stopped_sim_debug;
  byte                               mm_lu_reject_cause_sim_debug[MAX_LU_ATTEMPT_SIZE_DEBUG];
  byte                               mm_lu_start_reason_sim_debug;
  byte                               mm_msg_info_debug;
  boolean                            mm_nreg_release_pending_sim_debug;
  boolean                            mm_nreg_req_received_sim_debug;
  mmr_stop_mode_req_s_type           mm_pended_stop_mode_req_sim_mmr_stop_mode_req;
  boolean                            mm_pended_stop_mode_req_sim_valid;
  byte                               mm_pending_lu_request_sim_debug;
  byte                               mm_previous_serving_lai_sim_debug[LAI_SIZE];
  boolean                            mm_rat_change_pending_sim_debug;
  mm_rat_handler_status_debug_type   mm_rat_handler_status_sim_debug;
  sys_rat_pri_list_info_s_type       mm_rat_pri_list_info_sim_debug;
  boolean                            mm_ready_to_send_reg_cnf_sim_debug;
  boolean                            mm_received_suitable_search_end_ind_sim_debug;
  boolean                            mm_reg_waiting_for_reg_cnf_sim_debug;
  boolean                            mm_serv_change_pending_sim_debug;
  uint32                             mm_serving_cell_id_sim_debug;
  serving_plmn_info_debug_T          mm_serving_plmn_sim_debug;
  byte                               mm_state_sim_debug;
  boolean                            mm_stop_mode_local_detach_peding_sim_debug;
  byte                               mm_stored_imsi_sim_length;
  byte                               mm_stored_imsi_sim_data[IMSI_LEN_IN_BYTES]; 
  dword                              mm_stored_per_ms_t3212_value_sim_debug;
  sys_overall_feature_t              mm_sub_config_sim_debug;
  sys_modem_as_id_e_type             mm_sub_id_debug;
  mm_system_information_T            mm_system_information_sim_debug;
  dword                              mm_t3246_backoff_timer_value_sim_debug;
  byte                               mm_timer_status_sim_debug[MAX_MM_TIMER_DEBUG+1];
  boolean                            mm_waiting_for_deact_cnf_sim_debug;
  boolean                            mm_waiting_for_ph_status_change_cnf_sim_debug;
  boolean                            mm_waiting_for_service_cnf_sim_debug;
  boolean                            mm_waiting_for_stop_mode_cnf_sim_debug;
  boolean                            mmcoord_svc_cnf_pending_sim_debug;
  sys_modem_device_mode_e_type       mmode_nv_setting_for_device_mode_debug;
  WtoGCCO_in_progress_debug_enum_T   WtoGCCO_state_sim_debug;
  byte                               lu_cause_sim_debug;
  boolean                            mm_plmn_search_in_progress_sim_debug;
  sys_network_selection_mode_e_type  mm_waiting_for_list_cnf_type_sim_debug;
  mm_interrat_change_state_debug_type mm_interrat_state_sim_debug;
  sys_radio_access_tech_e_type       mm_service_loss_on_rat_sim_debug;
  mmrrc_connection_status_debug_T    cs_session_status_sim_debug;
  boolean                            mm_cs_domain_possible_reest_sim_debug;
  boolean                            mm_ps_domain_possible_reest_sim_debug;
  boolean                            pending_gmm_msg_after_release_sim_debug;
  rrc_establish_cause_e_type         pending_gmm_msg_cause_sim_debug;
  boolean                            pending_gmm_msg_sim_debug;
  boolean                            pending_gmm_msg_valid_sim_debug;
  boolean                            pending_mm_msg_after_release_sim_debug;
  rrc_establish_cause_e_type         pending_mm_msg_cause_sim_debug;
  boolean                            pending_mm_msg_sim_debug;
  boolean                            pending_mm_msg_valid_sim_debug;
  mmrrc_connection_status_debug_T    ps_session_status_sim_debug;
  mmrrc_connection_status_debug_T    rrc_connection_status_sim_debug;
}mm_debug_data_s_type;

/******************************************************************************
**                     MM EMM DEBUG BUFFER
******************************************************************************/
typedef mm_emm_debug_buffer_s_type mm_qsh_debug_buffer_s_type;

/******************************************************************************
**                     MM Metric Signaling sub structure
******************************************************************************/
typedef struct 
{
  mm_protocol_discriminator_debug_type  protocol_discriminator;

  union
  {
    mm_message_id_debug_type  mm_message_id;
    gmm_message_id_debug_type gmm_message_id;
    emm_message_id_debug_type emm_message_id;
  } message_type;

  union
  {
    mm_gmm_reject_cause_debug_type mm_cause;
    mm_gmm_reject_cause_debug_type gmm_cause;
    lte_nas_emm_cause_debug_type emm_cause;
  } cause_value;

  union
  {
    uint8                                             type;
    /* ATTACH_ACCEPT result type*/
    lte_nas_emm_eps_attach_result_debug_type          emm_eps_attach_result;
    /* ATTACH_REQUEST update type*/
    lte_nas_emm_attach_debug_type                     emm_eps_attach_type;
    /*  CS_SERV_NOTIFICATION paging identity*/
    lte_nas_paging_identity_debug_type                emm_paging_identity;
    /* DETACH_REQUEST Mobile originated, detach type*/
    emm_mo_detach_debug_type                          emm_mo_detach_type;
    /* DETACH_REQUEST Mobile terminated, detach type*/
    lte_nas_emm_mt_detach_debug_type                  emm_mt_detach_type;
    /* EXT_SERVICE_REQUEST service type*/
    lte_csfb_service_type                             emm_csfb_service;
    /*  EMM_IDENTITY_REQUEST identity type*/
    mobile_id_debug_type                              emm_identity_type;
    /*  TRACKING_AREA_UPDATE_ACCEPT UPDATE result type*/
    lte_nas_emm_eps_update_result_debug_type          emm_eps_update_result;
    /* TRACKING_AREA_UPADTE_REQUEST update type*/
    lte_nas_emm_eps_update_val_debug_type             emm_eps_update_val;
    /* PD_MM -> IDENTITY_REQUEST/TMSI_REALLOCATION_COMMAND identity type */
    mm_identity_type_debug_T                          mm_identity_type;
    /* LOCATION_UPDATING_REQUEST update type*/
    mm_location_updating_debug_type                   mm_location_updating_type;
    /* GMM_ATTACH_REQUEST update type*/
    gmm_gprs_attach_type_debug_T                      gmm_gprs_attach_type;
    /* GMM_ATTACH_ACCEPT result type*/
    gmm_gprs_attach_resut_debug_T                     gmm_gprs_attach_result;
    /* GMM_DETACH_REQUEST Mobile Originated detach type*/
    gmm_mo_detach_type_debug_T                        gmm_mo_detach_type;
    /* GMM_DETACH_REQUEST Mobile terminated detach type*/
    gmm_mt_detach_type_debug_T                        gmm_mt_detach_type;
    /* GMM_IDENTITY_REQUEST/GMM_PTMSI_REALLOCATION_COMMAND identity type*/
    mobile_id_debug_type                              gmm_identity_type;
    /* GMM_ROUTING_AREA_UPDATE_REQUEST update type*/
    gmm_ra_update_value_debug_T                       gmm_ra_update_value;
    /* GMM_ROUTING_AREA_UPDATE_ACCEPT result type*/
    gmm_ra_update_result_debug_T                      gmm_ra_update_result;
    /* GMM_SERVICE_REQUEST type*/
    gmm_service_request_type_debug_T                  gmm_service_request_type_debug;
  } procedure_type;

  uint8  direction;
  uint8  as_id;
}mm_qsh_signaling_s;



/*===========================================================================

                        QSH MACRO AND STRUCTURE TYPE DEFINITIONS

===========================================================================*/


/******************************************************************************
**  METRICS
**    1. OTA Messages => NAS Signalling Messages  - NAS OTA procedures 
**               ATTACH, TAU, MO/ MT DETACH, LAU & RAU
**    2. MM intertask messages.
******************************************************************************/
typedef enum
{
  MM_QSH_METRIC_SIGNALING = 0,
  MM_QSH_METRIC_DEBUG_BUFFER = 1,
  MM_QSH_METRIC_MAX
}mm_qsh_metric_e;

/******************************************************************************
**  QSH DUMP
**    1. Dump set for AS_ID_1
**    2. Dump set for AS_ID_2
**    3. Dump set for DEBUG BUFFER
******************************************************************************/
typedef enum
{
  MM_QSH_DUMP_TAG_AS_ID_1 = 0,
  MM_QSH_DUMP_TAG_AS_ID_2 = 1,
  MM_QSH_DUMP_TAG_DEBUG_BUFFER = 2,
  MM_QSH_DUMP_TAG_MAX = 31
}mm_qsh_dump_tag_e;

/******************************************************************************
**  QSH EVENTS
******************************************************************************/
typedef enum {
  MM_QSH_EVENT_LAU_FAILED = 0,
  MM_QSH_EVENT_LAU_REJECTED,
  MM_QSH_EVENT_ATTACH_ACCEPTED_PS_ONLY,
  MM_QSH_EVENT_ATTACH_FAILED,
  MM_QSH_EVENT_ATTACH_REJECTED,
  MM_QSH_EVENT_RAU_ACCEPTED_PS_ONLY,
  MM_QSH_EVENT_RAU_FAILED,
  MM_QSH_EVENT_RAU_REJECTED,
  MM_QSH_EVENT_TAU_ACCEPTED_PS_ONLY,
  MM_QSH_EVENT_TAU_FAILED,
  MM_QSH_EVENT_TAU_REJECTED,
  MM_QSH_EVENT_MO_DETACH_FAILED,
  MM_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE,
  MM_QSH_EVENT_EMM_SPECIFIC_SIGNALING,
  MM_QSH_EVENT_RPLMN_UPDATE,
  MM_QSH_EVENT_CS_CALL_STATUS,
  MM_QSH_EVENT_EMM_PROC,
  MM_QSH_EVENT_NAS_OTA_MSG_ACK,
  MM_QSH_EVENT_CS_PAGE_IND,
  MM_QSH_EVENT_MAX
}mm_qsh_event_e;

typedef enum {
MM_QSH_NO_SRV_DOMAIN=0,
MM_QSH_SRV_DOMAIN_CS_ONLY,
/**< Circuit switched only capable. */
MM_QSH_SRV_DOMAIN_PS_ONLY,
/**< Packet switched only capable. */
MM_QSH_SRV_DOMAIN_CS_PS
}mm_event_notify_id;

/******************************************************************************
**  QSH EVENT VERSION
**  for new QSH event definitions
******************************************************************************/
#define MM_QSH_EVENT_CS_CALL_STATUS_VERSION         (0x01)
#define MM_QSH_EVENT_EMM_PROC_VERSION               (0x01)
#define MM_QSH_EVENT_NAS_OTA_MSG_ACK_VERSION        (0x01)

/******************************************************************************
**      MM QSH DATA DUMP TYPE.(Applicable for both MM_QSH_DUMP_TAG_AS_ID_1 and 
**                           MM_QSH_DUMP_TAG_AS_ID_2 tags)
******************************************************************************/
typedef struct mm_qsh_dump_data_s
{
  qsh_dump_tag_hdr_s    hdr; /* this header will have a tag and length */
  emm_debug_data_s_type emm_debug_data;
  mm_debug_data_s_type  mm_debug_data;
}mm_qsh_dump_data_s_type;


/******************************************************************************
**      MM QSH DEBUG BUFFER DUMP TYPE.(Applicable for
**                           MM_QSH_DUMP_TAG_DEBUG_BUFFER  tag)
******************************************************************************/
typedef struct mm_qsh_dump_debug_buffer_s
{
  qsh_dump_tag_hdr_s hdr; /* this header will have a tag and length */
  mm_qsh_debug_buffer_s_type debug_buffer[MM_DEBUG_DUMP_BUFFER_SIZE];
}mm_qsh_dump_debug_buffer_s_type;

typedef mm_qsh_dump_data_s_type mm_qsh_dump_tag_as_id_1_s;
typedef mm_qsh_dump_data_s_type mm_qsh_dump_tag_as_id_2_s;
typedef mm_qsh_dump_debug_buffer_s_type mm_qsh_dump_tag_debug_buffer_s;


/******************************************************************************
**      MM QSH METRICS TYPE.for DEBUG BUFFER(Applicable for
**                           MM_QSH_METRIC_DEBUG_BUFFER metrics tag)
******************************************************************************/
typedef struct 
{
  qsh_metric_hdr_s hdr; /* This header has the timestamp (uint16) */
  mm_qsh_debug_buffer_s_type debug_buffer;
}mm_qsh_metric_debug_buffer_s;


/******************************************************************************
**      MM QSH METRICS TYPE.for DEBUG SIGNALING(Applicable for
**                           MM_QSH_METRIC_SIGNALING metrics tag)
******************************************************************************/
typedef struct 
{
  qsh_metric_hdr_s hdr; /* This header has the timestamp (uint16) */
  mm_qsh_signaling_s metric_sig;
}mm_qsh_metric_signaling_s;


/*===========================================================================

FUNCTION mm_qevent_call_status_set

DESCRIPTION
This function sets the flag when CSFB call is started and QEVENT is sent

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void mm_qevent_call_status_set(boolean status);

/*===========================================================================

FUNCTION mm_qevent_call_status_get

DESCRIPTION
This function returns the flag 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern boolean mm_qevent_call_status_get();

#endif /* _MM_QSH_EXT_H_ */



