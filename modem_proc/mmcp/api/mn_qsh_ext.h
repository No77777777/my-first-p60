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

$Header: //components/rel/mmcp.mpss/7.9.0/api/mn_qsh_ext.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/03/2015 sar     API header file created for collecting a minimal set of debug 
                   data without QXDM log.  
===========================================================================*/
#ifndef MN_QSH_EXT_H
#define MN_QSH_EXT_H

#include "comdef.h"
#include "qsh.h"
#include "sys.h"
#include "nas_exp.h"
#include "mn_cm_exp.h"
#include "cm_gw.h"
#include "mn_uasms.h"
#include "mn_sms.h"
#include "sys_cnst.h"

#define MN_MAXNO_CONNECTION_IN_SESSION_DEBUG  (0x14)

//=============  Type Def Sction ================
typedef byte                      connection_id_debug_T;
#define MN_QSH_MAJOR_VER 1
#define MN_QSH_MINOR_VER 1


//=============  Enum Types Section  ==========
typedef enum
{
  MN_QSH_DUMP_TAG_AS_ID_1 = 0,
  MN_QSH_DUMP_TAG_AS_ID_2 = 1,
  MN_QSH_DUMP_TAG_MAX = 31
}mn_qsh_dump_tag_e;

//The following enum data types are exact duplicate of private data in MN
//sub-module. Any updated change must be made in both places.

typedef enum 
{
    MN_SS_ALT_NONE_AWAITED_DEBUG,
    MN_SS_ALT_HOLD_ACK_RETRIEVE_ACK_AWAITED_DEBUG,
    MN_SS_ALT_HOLD_ACK_AWAITED_DEBUG,
    MN_SS_ALT_RETRIEVE_ACK_AWAITED_DEBUG,
    MN_SS_ALT_NACKED_DEBUG
} mn_alternate_procedure_status_debug_e_type;

typedef enum
{
       MN_AS_ID_1_DEBUG=0,
       /**< Subscription ID 1 */

       MN_AS_ID_2_DEBUG,
       /**< Subscription ID 2 */
}mn_dsda_as_id_debug_e_type;

typedef enum
{
  RRCMN_CS_DATA_CALL_DEBUG,         /* Indicates CS Data Call */
  RRCMN_CS_VOICE_CALL_DEBUG,        /* Indicates CS Voice Call */ 
  RRCMN_INVALID_RAB_ID_DEBUG        /* Invalid Radio Access Bearer */
}mn_rrcmn_rab_status_debug_e_type;

typedef enum {
    DUAL_SERVICE_NONE_DEBUG,           /* non-Multical dual services */
    DUAL_SERVICE_IN_VOICE_DEBUG,
    DUAL_SERVICE_IN_DATA_DEBUG,
    DUAL_SERVICE_IN_MULTICALL_DEBUG
} mn_dual_service_debug_e_type;

//=============  Struct Types section  ==========
//The following enum data types are exact duplicate of private data in MN
//sub-module. Any updated change must be made in both places.

typedef  struct
{
    byte      call_activity;
    boolean   call_is_mpty;
    boolean   split_mpty;
    byte      id_index;
    byte      connection_id[MN_MAXNO_CONNECTION_IN_SESSION_DEBUG];
} mn_Multicall_Status_debug_T;

typedef struct
{
    boolean                                      is_alternate_procedure;
    mn_alternate_procedure_status_debug_e_type   status;
} mn_alternate_procedure_status_debug_T;

typedef struct {
    boolean              present;
    byte                 type;
    boolean              activated;
    boolean              receiver_busy;   /* TRUE = not ready */
} mn_user_user_data_debug_T;  /* queue */

typedef struct mn_call_information_debug_tag
{
   CNMServiceType_T                CNM_service_type;
   boolean                         progress_indicator_present;
   cm_progress_indicator_T         progress_indicator;
   byte                            repeat_indication_hlc;
   cm_high_layer_compatibility_T   high_layer_compatibility_1; 
   cm_high_layer_compatibility_T   high_layer_compatibility_2;
   byte                            repeat_indication_llc;
   cm_low_layer_compatibility_T    low_layer_compatibility_1; 
   cm_low_layer_compatibility_T    low_layer_compatibility_2;
   byte                            repeat_indicator_bc;
   cm_bearer_capability_T          bearer_capability_1; 
   cm_bearer_capability_T          bearer_capability_2; 
   cm_called_party_bcd_no_T        called_party_bcd_number;
   boolean                         called_party_bcd_number_present;
   cm_called_party_subaddress_T    called_party_subaddress;
   boolean                         called_party_subaddress_present;
   cm_calling_party_bcd_no_T       calling_party_bcd_number;
   boolean                         calling_party_bcd_number_present;
   cm_calling_party_subaddress_T   calling_party_subaddress;
   boolean                         calling_party_subaddress_present;
   cm_calling_party_bcd_no_T       redirect_party_bcd_number;
   boolean                         redirect_party_bcd_number_present;
   cm_calling_party_subaddress_T   redirect_party_subaddress;
   boolean                         redirect_party_subaddress_present;
   cm_connected_no_T               connected_number;
   boolean                         connected_number_present;
   cm_connected_subaddress_T       connected_subaddress;
   boolean                         connected_subaddress_present;
   boolean                         clir_suppression;
   boolean                         clir_invocation;
   boolean                         signal_present;
   byte                            SignalValue;
   byte                            active_bearer_capability;
   boolean                         mo_setup_pending;
   boolean                         mo_call_clear_pending;
   byte                            mo_release_indicator; 
   byte                            mt_call_accept_cause;
   int                             no_of_changes;
   byte                            TCH_channel_type;
   byte                            subchannel_number;
   boolean                         reverse_call_direction;
   ie_cm_cc_cause_T                cm_cc_cause;
   ie_cm_cc_cause_T                mo_release_cause_diag_packet; 
   ie_cm_cc_cause_T                call_reject_cause_diag_packet; 
   cm_forward_cug_info_T           cm_forward_cug_info;
   uint8                           radio_access_bearer_id;
   cm_cause_of_no_CLI_T            cause_of_no_CLI;
   cm_alerting_pattern_T           alerting_pattern;
   mn_user_user_data_debug_T       user_user_data;
   mn_dual_service_debug_e_type    dual_services;
   boolean                         ccbs_allowed;
   cm_stream_identifier_T          stream_id; 
   cm_supported_codecs_list_T      supported_codecs_list; 
   boolean                         immediate_modify_ind; 
   cm_bearer_capability_T          backup_bearer_capability; 
   cm_network_cc_capability_T      network_cc_capability;
   boolean                         nw_initiated_ind; 
   boolean                         modifiy_in_progress; 
   cm_emergency_category_T         emergency_category;
   boolean                         offline;
   byte                            hold_request;
   byte                            ss_request_pending;
   boolean                         received_start_dtmf;
   byte                            cause_before_gsm_reestablishment;
   sys_modem_as_id_e_type          as_id;
} mn_call_information_debug_T;

typedef struct {
   connection_id_T               connection_id;
   mn_call_information_debug_T   mn_call_information;    
} mn_call_information_ext_debug_T;

typedef struct {
   connection_id_T            connection_id;
   byte                       cause;
   byte                       duration;
   sms_link_control_mode_T    mode;
   sms_link_control_status_T  status;
   boolean                    cp_ack_deferred;
} mn_sms_link_control_debug_T;

typedef struct
{

/* MO components */

   byte             ss_stored_operation;
   connection_id_T  connection_id;
   byte              cm_ss_ref;
   boolean      ss_password_present;
   ie_ss_password_T ss_password;
   boolean      ss_new_password_present;
   ie_ss_password_T ss_new_password;
   boolean      ss_new_password_again_present;
   ie_ss_password_T ss_new_password_again;

/* MT components */

   byte             mt_invoke_id;
   boolean          mt_invoke_id_valid;
   byte             mt_ss_operation;
   connection_id_T  mt_connection_id;
   byte             mt_cm_ss_ref;
}mn_ss_invoke_data_debug_T;

typedef  struct
{ boolean                OTA_channel_connected;
  boolean                speech_channel_connected;
  boolean                data_channel_connected;
  byte                   tch_rate;
  byte                   subchannel;
  channel_mode_T         channel_mode;
  umts_codecs_list_T                   umts_codec;
  umts_wcdma_cs_voice_bearer_enum_T    cs_voice_type;
}mn_tch_user_connections_store_debug_T;


/******************************************************************************
**                     MM DEBUG DATA
******************************************************************************/
typedef struct mn_debug_data_tag
{
  mn_Multicall_Status_debug_T              Multicall_Status_debug;
  connection_id_debug_T                    last_allocated_mo_sms_connection_id_debug;
  mn_alternate_procedure_status_debug_T    mn_alternate_proc_status_debug; 
  sys_modem_as_id_e_type                   mn_as_id_debug;
  uint32                                   mn_asid_2_vsid_map_debug;
  mn_call_information_ext_debug_T          mn_call_information_debug;
  mn_dsda_as_id_debug_e_type               mn_dsda_as_id_debug;
  boolean                                  mn_waiting_for_tch_in_srvcc_debug;
  nas_msg_transport_type_e_type            mo_sms_transport_type_debug;
  nas_msg_transport_type_e_type            mt_sms_transport_type_debug;
  sm_rl_transaction_information_T          sm_rl_transaction_information_debug;
  mn_sms_link_control_debug_T              sms_link_control_debug;  
  mn_ss_invoke_data_debug_T                ss_invoke_data_debug; 
  word                                     suppsvc_l3_data_length_debug;
  mn_tch_user_connections_store_debug_T    tch_user_connections_store_debug;
  boolean                                  waiting_for_next_network_ussd_trigger_debug;
  byte                                     mn_als_itc_aux_speech_debug;
  byte                                     mn_nv_mt_call_reject_cause_debug;
  byte                                     mn_nv_no_auto_answer_on_hold_debug;
  mn_rrcmn_rab_status_debug_e_type         mn_rab_status_debug;
  boolean                                  MN_turned_on_vocoder_debug;
} mn_debug_data_type;

typedef struct mn_qsh_dump_data_s
{
  qsh_dump_tag_hdr_s hdr; /* this header will have a tag and length */
  mn_debug_data_type mn_debug_data;
}mn_qsh_dump_data_s_type;

typedef mn_qsh_dump_data_s_type mn_qsh_dump_tag_as_id_1_s;
typedef mn_qsh_dump_data_s_type mn_qsh_dump_tag_as_id_2_s;

#endif /* MN_QSH_EXT_H */

