/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            Q S H    M E M O R Y    D U M P    I N T E R F A C E


GENERAL DESCRIPTION

  This source file contains function which is called to dump mmgsdi variable
  as part of QSH dump collection.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_qsh_dump.c#1 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/04/18   vdc     Access SIMLOCK related response queue under FEATURE_SIMLOCK
08/23/18   rps     initial versoin
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "mmgsdi.h"
#include "mmgsdilib_common.h"
#include "uim_common_qsh.h"

#ifdef FEATURE_SIMLOCK
#include "mmgsdi_simlock.h"
#endif /* FEATURE_SIMLOCK */

#ifdef FEATURE_UIM_MULTI_PROFILE
#include "mmgsdi_multi_profile.h"
#endif /* FEATURE_UIM_MULTI_PROFILE */

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains functions for the current module.

===========================================================================*/

#ifdef FEATURE_UIM_MULTI_PROFILE
extern mmgsdi_multi_profile_card_system_data_type *mmgsdi_profile_info_ptr;
#endif /* FEATURE_UIM_MULTI_PROFILE */

extern boolean mmgsdi_process_qmi_uim_sig;

/*==========================================================================
FUNCTION UIM_QSH_MDUMP_MMGSDI

DESCRIPTION
  This function is called to dump mmgsdi globals

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/
QSH_MDUMP_FN_ATTR void uim_qsh_mdump_mmgsdi()
{
  uint8 index = 0;

  qsh_mdump_collect_high(mmgsdi_uim_cmd, sizeof(mmgsdi_uim_cmd));
  qsh_mdump_collect_high(mmgsdi_sap_connected, sizeof(mmgsdi_sap_connected));

  qsh_mdump_collect_high(mmgsdi_client_id_reg_table, sizeof(mmgsdi_client_id_reg_table));
  for(index = 0; index < MMGSDI_MAX_CLIENT_INFO; index++)
  {
    mmgsdi_client_id_reg_table_type *client_info_ptr = mmgsdi_client_id_reg_table[index];

    if(client_info_ptr != NULL)
    {
      uint32 session_info_index = 0;

      qsh_mdump_collect_high(client_info_ptr, sizeof(*client_info_ptr));
      /* Dump session information */
      for(session_info_index = 0; session_info_index < MMGSDI_MAX_SESSION_INFO; session_info_index++)
      {
        if(client_info_ptr->session_info_ptr[session_info_index] != NULL)
        {
          qsh_mdump_collect_high(client_info_ptr->session_info_ptr[session_info_index], sizeof(mmgsdi_session_info_type));
        }
      }
    }
  }

  qsh_mdump_collect_high(mmgsdi_client_req_table_info_ptr, sizeof(mmgsdi_client_req_table_info_ptr));
  for(index = 0; index < MMGSDI_MAX_CLIENT_REQ_INFO; index++)
  {
    mmgsdi_uim_client_req_user_data_type *client_req_ptr = mmgsdi_client_req_table_info_ptr[index];

    if(client_req_ptr != NULL)
    {
      qsh_mdump_collect_high(client_req_ptr, sizeof(*client_req_ptr));
      if(client_req_ptr->extra_param_ptr != NULL)
      {
        qsh_mdump_collect_high(client_req_ptr->extra_param_ptr, sizeof(mmgsdi_client_req_extra_info_type));
      }
    }
  }

  qsh_mdump_collect_high(mmgsdi_data_slot_ptr, sizeof(mmgsdi_data_slot_ptr));
  for(index = 0; index < MMGSDI_MAX_NUM_SLOTS; index++)
  {
    if(mmgsdi_data_slot_ptr[index] != NULL)
    {
      qsh_mdump_collect_high(mmgsdi_data_slot_ptr[index], sizeof(mmgsdi_slot_data_type));
      /* No need to dump mmgsdi_data_slot_ptr->app_info_pptr, as it will be taken care while
         dumping mmgsdi_app_pin_table_ptr */
    }
  }

  qsh_mdump_collect_high(mmgsdi_generic_data_ptr, sizeof(*mmgsdi_generic_data_ptr));
  for(index = 0; index < MMGSDI_MAX_PROV_APPS; index++)
  {
    if(mmgsdi_generic_data_ptr != NULL)
    {
      if(mmgsdi_generic_data_ptr->mmgsdi_switch_act_cnf_ptr[index] != NULL)
      {
        qsh_mdump_collect_high(mmgsdi_generic_data_ptr->mmgsdi_switch_act_cnf_ptr[index], sizeof(mmgsdi_callback_cnf_type));
      }
    }
  }

  qsh_mdump_collect_high(mmgsdi_app_pin_table_ptr, sizeof(mmgsdi_app_pin_table_ptr));
  for(index = 0; index < MMGSDI_MAX_NUM_SLOTS; index++)
  {
    uint32 app_id_index = 0;

    for(app_id_index = 0; app_id_index < MMGSDI_MAX_EXT_APP_INFO; app_id_index++)
    {
      if(mmgsdi_app_pin_table_ptr[index][app_id_index] != NULL)
      {
        qsh_mdump_collect_high(mmgsdi_app_pin_table_ptr[index][app_id_index], sizeof(mmgsdi_int_app_info_type));
        if(mmgsdi_app_pin_table_ptr[index][app_id_index]->pin1_ptr != NULL)
        {
          qsh_mdump_collect_high(mmgsdi_app_pin_table_ptr[index][app_id_index]->pin1_ptr, sizeof(mmgsdi_app_pin_info_type));
        }
  
        if(mmgsdi_app_pin_table_ptr[index][app_id_index]->universal_pin_ptr != NULL)
        {
          qsh_mdump_collect_high(mmgsdi_app_pin_table_ptr[index][app_id_index]->universal_pin_ptr, sizeof(mmgsdi_app_pin_info_type));
        }
  
        if(mmgsdi_app_pin_table_ptr[index][app_id_index]->cphs_info_ptr != NULL)
        {
          qsh_mdump_collect_high(mmgsdi_app_pin_table_ptr[index][app_id_index]->cphs_info_ptr, sizeof(mmgsdi_cphs_int_info_type));
        }
      }
    }
  }

  qsh_mdump_collect_high(mmgsdi_app_pin1_table_ptr, sizeof(mmgsdi_app_pin1_table_ptr));
  for(index = 0; index < MMGSDI_MAX_NUM_SLOTS; index++)
  {
    uint32 app_index = 0;

    for(app_index = 0; app_index < MMGSDI_MAX_PIN1_INFO; app_index++)
    {
      if(mmgsdi_app_pin1_table_ptr[index][app_index] != NULL)
      {
        qsh_mdump_collect_high(mmgsdi_app_pin1_table_ptr[index][app_index], sizeof(mmgsdi_app_pin_info_type));
      }
    }
  }

  qsh_mdump_collect_high(mmgsdi_app_univ_pin_table_ptr, sizeof(mmgsdi_app_univ_pin_table_ptr));
  for(index = 0; index < MMGSDI_MAX_NUM_SLOTS; index++)
  {
    if(mmgsdi_app_univ_pin_table_ptr[index] != NULL)
    {
      qsh_mdump_collect_high(mmgsdi_app_univ_pin_table_ptr[index], sizeof(mmgsdi_app_pin_info_type));
    }
  }

  qsh_mdump_collect_high(mmgsdi_refresh_info_ptr, sizeof(mmgsdi_refresh_info_ptr));
  for(index = 0; index < MMGSDI_MAX_NUM_SLOTS; index++)
  {
    if(mmgsdi_refresh_info_ptr[index] != NULL)
    {
      qsh_mdump_collect_high(mmgsdi_refresh_info_ptr[index], sizeof(mmgsdi_refresh_info_type));
    }
  }

  qsh_mdump_collect_high(mmgsdi_refresh_retry_settings_ptr, sizeof(mmgsdi_refresh_retry_settings_ptr));
  for(index = 0; index < MMGSDI_MAX_NUM_SLOTS; index++)
  {
    if(mmgsdi_refresh_retry_settings_ptr[index] != NULL)
    {
      qsh_mdump_collect_high(mmgsdi_refresh_retry_settings_ptr[index], sizeof(mmgsdi_refresh_retry_settings_type));
    }
  }

  qsh_mdump_collect_high(mmgsdi_channel_info_ptr_table, sizeof(mmgsdi_channel_info_ptr_table));
  for(index = 0; index < MMGSDI_MAX_CHANNEL_INFO; index++)
  {
    if(mmgsdi_channel_info_ptr_table[index] != NULL)
    {
      qsh_mdump_collect_high(mmgsdi_channel_info_ptr_table[index], sizeof(mmgsdi_channel_info_type));
    }
  }

  qsh_mdump_collect_high(mmgsdi_1x_prl, sizeof(mmgsdi_1x_prl));
  for(index = 0; index < MMGSDI_MAX_NUM_SLOTS; index++)
  {
    if(mmgsdi_1x_prl[index].prl_data_ptr != NULL)
    {
      qsh_mdump_collect_high(mmgsdi_1x_prl[index].prl_data_ptr, sizeof(mmgsdi_read_prl_data_type));
      if(mmgsdi_1x_prl[index].prl_data_ptr->roaming_list_ptr != NULL)
      {
        qsh_mdump_collect_high(mmgsdi_1x_prl[index].prl_data_ptr->roaming_list_ptr, sizeof(*(mmgsdi_1x_prl[index].prl_data_ptr->roaming_list_ptr)));
      }
    }
  }

  qsh_mdump_collect_low(mmgsdibt_pipe_apdu_ptr, sizeof(mmgsdibt_pipe_apdu_ptr));
  for(index = 0; index < MMGSDI_MAX_NUM_SLOTS; index++)
  {
    if(mmgsdibt_pipe_apdu_ptr[index] != NULL)
    {
      qsh_mdump_collect_low(mmgsdibt_pipe_apdu_ptr[index], sizeof(mmgsdi_cnf_type));
    }
  }

  for(index = 0; index < MMGSDI_NUM_TASKS; index++)
  {
    qsh_mdump_collect_high(&(mmgsdi_card_status_cmd_q[index].cnt), sizeof(mmgsdi_card_status_cmd_q[index].cnt));
  }

  for(index = 0; index < MMGSDI_NUM_TASKS; index++)
  {
    qsh_mdump_collect_high(&(mmgsdi_internal_cmd_q[index].cnt), sizeof(mmgsdi_internal_cmd_q[index].cnt));
  }

  for(index = 0; index < MMGSDI_NUM_TASKS; index++)
  {
    qsh_mdump_collect_high(&(mmgsdi_task_cmd_q[index].cnt), sizeof(mmgsdi_task_cmd_q[index].cnt));
  }

  for(index = 0; index < MMGSDI_NUM_TASKS; index++)
  {
    qsh_mdump_collect_high(&(mmgsdi_task_rsp_q[index].cnt), sizeof(mmgsdi_task_rsp_q[index].cnt));
  }

  qsh_mdump_collect_high(&(mmgsdi_refresh_cmd_q.cnt), sizeof(mmgsdi_refresh_cmd_q.cnt));
  qsh_mdump_collect_high(&(mmgsdi_nv_q.cnt), sizeof(mmgsdi_nv_q.cnt));

#ifdef FEATURE_SIMLOCK
  qsh_mdump_collect_high(&(mmgsdi_simlock_rsp_q.cnt), sizeof(mmgsdi_simlock_rsp_q.cnt));
#endif /* FEATURE_SIMLOCK */

  qsh_mdump_collect_high(mmgsdi_apdu_get_rsp_table_ptr, sizeof(mmgsdi_apdu_get_rsp_table_ptr));
  for(index = 0; index < MMGSDI_MAX_AVAIL_GET_RSP_CACHE_ENTRIES; index++)
  {
    if(mmgsdi_apdu_get_rsp_table_ptr[index] != NULL)
    {
      qsh_mdump_collect_high(mmgsdi_apdu_get_rsp_table_ptr[index], sizeof(mmgsdi_apdu_data_get_rsp_cache_type));
    }
  }

  qsh_mdump_collect_high(mmgsdi_internal_synch_uim_rpt_rsp, sizeof(mmgsdi_internal_synch_uim_rpt_rsp));
  for(index = 0; index < MMGSDI_NUM_TASKS; index++)
  {
    if(mmgsdi_internal_synch_uim_rpt_rsp[index] != NULL)
    {
      qsh_mdump_collect_high(mmgsdi_internal_synch_uim_rpt_rsp[index], sizeof(mmgsdi_uim_report_rsp_type));
    }
  }

  qsh_mdump_collect_high(mmgsdi_task_state, sizeof(mmgsdi_task_state));
  qsh_mdump_collect_high(mmgsdi_sync_cmd_canceled, sizeof(mmgsdi_sync_cmd_canceled));
  qsh_mdump_collect_high(mmgsdi_need_sync_cmd_cancel_ack, sizeof(mmgsdi_need_sync_cmd_cancel_ack));

#ifdef FEATURE_SIMLOCK
  qsh_mdump_collect_high(mmgsdi_simlock_status_table, sizeof(mmgsdi_simlock_status_table));
#endif /* FEATURE_SIMLOCK */

  qsh_mdump_collect_high(&mmgsdi_process_qmi_uim_sig, sizeof(mmgsdi_process_qmi_uim_sig));

#ifdef FEATURE_UIM_MULTI_PROFILE
  qsh_mdump_collect_high(&mmgsdi_profile_info_ptr, sizeof(mmgsdi_profile_info_ptr));
#endif /* FEATURE_UIM_MULTI_PROFILE */
} /* uim_qsh_mdump_mmgsdi */
#endif /* FEATURE_QSH_MDUMP */