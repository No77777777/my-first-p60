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

===========================================================================*/

#ifndef _REG_QSH_EXT_H_
#define _REG_QSH_EXT_H_

#include <customer.h>
#include "qsh.h"
#include "comdef.h"
#include "sys_plmn_selection.h" 
#include "reg_cm.h"
#include "reg_mm.h"


/*===========================================================================

                        MACROS DEFINITION

===========================================================================*/
#define REG_MODE_MAX_3GPP_RAT_DEBUG 4
#define REG_DEBUG_DUMP_BUFFER_SIZE      40

#define REG_QSH_MAJOR_VER 1
#define REG_QSH_MINOR_VER 1

/*===========================================================================

                        ENUMS DEFINITION

===========================================================================*/
typedef enum
{
  REG_SIM_MODE_PREF_GSM_RADIO_ACCESS_PREFERRED_DEBUG,
  REG_SIM_MODE_PREF_UMTS_RADIO_ACCESS_PREFERRED_DEBUG,
  REG_SIM_MODE_PREF_LTE_RADIO_ACCESS_PREFERRED_DEBUG,
  REG_SIM_MODE_PREF_TDSCDMA_RADIO_ACCESS_PREFERRED_DEBUG,
  REG_SIM_MODE_PREF_UNDEFINED_DEBUG
} reg_sim_mode_pref_debug_e_type;

typedef enum 
{
  REG_MODE_FOREGROUND_SEARCH_NONE_DEBUG = -1,
  REG_MODE_FOREGROUND_SEARCH_STARTED_DEBUG,
  REG_MODE_FOREGROUND_SEARCH_ATTEMPTED_DEBUG
} reg_mode_foreground_search_status_debug_e_type;

/**
** Enumeration of Active Subscription ID's 
*/
typedef enum {

  REG_AS_ID_1_DEBUG = 0,
    /**< Subscription ID 1 */

  REG_AS_ID_2_DEBUG,
    /**< Subscription ID 2 */

  REG_AS_ID_3_DEBUG

    /**< Subscription ID 3 */

} reg_as_id_debug_e_type;

typedef struct reg_mode_3gpp_capability_debug_s
{
   /* Indicate how many valid items are in this list.
   */
   uint8 num_items;  

   sys_radio_access_tech_e_type   rat_list[REG_MODE_MAX_3GPP_RAT_DEBUG];
   
}reg_mode_3gpp_capability_debug_s_type;

typedef enum 
{
  REG_MODE_AUTOMATIC_MODE_NONE_DEBUG = -1,             /* FOR INTERNAL USE ONLY! */
  REG_MODE_AUTOMATIC_MODE_NORMAL_DEBUG,
  REG_MODE_AUTOMATIC_MODE_USER_RESELECTION_DEBUG,
  REG_MODE_AUTOMATIC_MODE_HIGH_PRIORITY_SEARCH_DEBUG,
  REG_MODE_AUTOMATIC_MODE_MANUAL_DEBUG,
  REG_MODE_AUTOMATIC_MODE_SUITABLE_SEARCH_DEBUG,
  REG_MODE_AUTOMATIC_MODE_BETTER_SYSTEM_RESELECTION_DEBUG,
  REG_MODE_AUTOMATIC_MODE_EF_RAT_REFRESH_DEBUG,
  REG_MODE_AUTOMATIC_MODE_MMSS_HPLMN_DEBUG,
  REG_MODE_AUTOMATIC_MODE_MMSS_RPLMN_DEBUG,
  REG_MODE_AUTOMATIC_MODE_NO_PREFERENCE_DEBUG,
  REG_MODE_AUTOMATIC_MODE_IRAT_HIGH_PRIORITY_SEARCH_DEBUG,
  REG_MODE_AUTOMATIC_MODE_ROAMING_PLMN_DEBUG,
  REG_MODE_AUTOMATIC_MODE_MAX_DEBUG                    /* FOR INTERNAL USE ONLY! */
} reg_mode_automatic_mode_debug_e_type;


typedef enum 
{
  TO_REG_DEBUG,
  FROM_REG_DEBUG,  
}reg_message_direction_debug_type_T;


/******************************************************************************
**                     REG DEBUG DATA
******************************************************************************/
typedef struct reg_debug_data_s
{
    reg_as_id_debug_e_type    reg_as_id_debug;
    reg_cm_service_req_e_type   reg_last_cm_service_req_type_debug;
    boolean reg_last_mm_message_only_for_reg_debug;
    reg_cm_network_list_req_e_type  reg_last_recieved_list_req_type_debug;
    boolean reg_mode_automatic_plmn_selection_first_attempt_debug;
    reg_mode_3gpp_capability_s_type reg_mode_automatic_plmn_selection_foreground_rat_list_debug;
    reg_mode_automatic_mode_debug_e_type  reg_mode_automatic_plmn_selection_mode_debug;
    reg_mode_automatic_mode_debug_e_type  reg_mode_automatic_type_debug;
    reg_mode_foreground_search_status_debug_e_type    reg_mode_foreground_search_status_debug;
    boolean reg_mode_higher_priority_plmn_selection_first_attempt_debug;
    boolean reg_mode_hplmn_to_be_selected_debug;
    sys_srv_domain_e_type   reg_mode_limited_service_domain_debug;
    reg_mode_3gpp_capability_s_type reg_mode_limited_service_rat_order_debug;
    byte    reg_mode_lte_retry_counter_debug;
    boolean reg_mode_manual_plmn_selection_first_attempt_debug;
    sys_network_selection_mode_e_type   reg_mode_network_selection_mode_debug;
    boolean reg_mode_plmn_selection_first_attempt_debug;
    reg_mode_automatic_mode_debug_e_type  reg_mode_plmn_selection_mode_debug;
    mmr_reg_req_e_type  reg_mode_plmn_selection_reg_req_type_debug;
    sys_rat_pri_list_info_s_type    reg_mode_rat_pri_list_debug;
    sys_plmn_id_s_type  reg_mode_req_plmn_debug;
    sys_radio_access_tech_e_type    reg_mode_req_rat_debug;
    sys_srv_domain_e_type   reg_mode_req_service_domain_debug;
    sys_scan_scope_e_type   reg_mode_scan_scope_debug;
    sys_radio_access_tech_e_type    reg_mode_service_available_rat_debug;
    boolean reg_mode_service_req_aborted_debug;
    boolean reg_mode_sms_only_debug;
    boolean reg_mode_timed_service_req_debug;
    boolean reg_mode_usim_compliant_debug;
    uint8   reg_mode_validate_sim_counter_debug;
    boolean reg_need_to_process_sim_avail_req_debug;
    uint8   reg_nv_context_id_debug;
    boolean reg_send_grat_scan_done_debug;
    mmr_reg_req_s_type  reg_send_mmr_reg_req_msg_debug;
    mmgsdi_app_enum_type    reg_sim_card_mode_debug;
    sys_plmn_id_s_type  reg_sim_cs_rplmn_debug;
    byte    reg_sim_ens_flag_debug;
    sys_plmn_id_s_type  reg_sim_hplmn_debug;
    boolean reg_sim_is_ahplmn_read_debug;
    boolean reg_sim_is_ens_read_debug;
    uint8   reg_sim_max_subscription_debug;
    reg_sim_mode_pref_debug_e_type    reg_sim_rplmn_rat_search_order_debug;
    boolean reg_sim_rplmn_read_debug;
    sys_sim_state_e_type    reg_sim_state_sim_debug;
    boolean reg_state_camped_ind_received_debug;
    boolean reg_state_cm_initiated_registration_debug;
    boolean reg_state_cm_sim_update_ind_pending_debug;
    reg_cm_transaction_id_type  reg_state_cm_transaction_id_debug;
    boolean reg_state_irat_hplmn_timer_search_ongoing_debug;
    boolean reg_state_irat_lte_timer_search_ongoing_debug;
    boolean reg_state_is_power_on_plmn_selection_debug;
    boolean reg_state_is_roaming_plmn_to_be_camped_debug;
    sys_stop_mode_reason_e_type     reg_state_last_stop_mode_reason_debug;
    byte    reg_state_plmn_search_transaction_id_debug;
    sys_plmn_id_s_type  reg_state_prev_plmn_debug;
    sys_plmn_id_s_type  reg_state_rlf_plmn_debug;
    sys_radio_access_tech_e_type    reg_state_rlf_rat_debug;
    boolean reg_state_rlf_started_debug;
    sys_plmn_service_state_s_type   reg_state_service_state_debug;
    sys_radio_access_tech_e_type    reg_state_user_resel_plmn_rat_debug;
    sys_plmn_id_s_type  reg_state_user_resel_plmn_debug;
    sys_overall_feature_t   reg_sub_config_debug;
    reg_as_id_debug_e_type    reg_sub_id_debug;
    reg_as_id_debug_e_type    reg_sub_id_stack_debug;
    boolean reg_waiting_for_open_session_cnf_debug;

}reg_debug_data_s_type;

/******************************************************************************
                     REG DEBUG BUFFER
******************************************************************************/
typedef struct reg_qsh_debug_buffer_s
{ 
    struct reg_qsh_info
    {
      byte message_set;
      dword message_id;
    } reg_qsh_info_debug_buffer;  
    reg_message_direction_debug_type_T reg_message_direction;
    sys_modem_as_id_e_type as_id;
}reg_qsh_debug_buffer_s_type;

/*===========================================================================

                        QSH MACRO AND STRUCTURE TYPE DEFINITIONS

===========================================================================*/

/******************************************************************************
**  QSH DUMP
**    0: Dump set for AS_ID_1
**    1: Dump set for AS_ID_2
**    2:. Dump set for DEBUG BUFFER
******************************************************************************/
typedef enum
{
  REG_QSH_DUMP_TAG_AS_ID_1 = 0,
  REG_QSH_DUMP_TAG_AS_ID_2 = 1,
  REG_QSH_DUMP_TAG_DEBUG_BUFFER = 2,
  REG_QSH_DUMP_TAG_MAX = 32
}reg_qsh_dump_tag_e;


/******************************************************************************
**      REG QSH DATA DUMP TYPE (Applicable for both REG_QSH_DUMP_TAG_AS_ID_1 and 
**                           REG_QSH_DUMP_TAG_AS_ID_2 tags)
******************************************************************************/
typedef struct reg_qsh_dump_data_s
{
  qsh_dump_tag_hdr_s hdr; /* this header will have a tag and length */
  reg_debug_data_s_type reg_debug_data;  
}reg_qsh_dump_data_s_type;



/******************************************************************************
**      REG QSH DEBUG BUFFER DUMP TYPE
**      REG_QSH_DUMP_TAG_DEBUG_BUFFER  tag
******************************************************************************/
typedef struct reg_qsh_dump_debug_buffer_s
{
  qsh_dump_tag_hdr_s hdr; /* this header will have a tag and length */
  reg_qsh_debug_buffer_s_type debug_buffer[REG_DEBUG_DUMP_BUFFER_SIZE];
}reg_qsh_dump_debug_buffer_s_type;

#endif

