/*===========================================================================

                           DS_APPSRV_UTILS.C

DESCRIPTION

   Collection of utility functions being used by various modules in APPSRV.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                       EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/utils/src/ds_appsrv_utils.c#3 $
  $Author: pwbldsvc $  $DateTime: 2020/01/10 02:50:10 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/17/15   hr      Initial version.
===========================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "comdef.h"
#include "data_msg.h"
#include "datamodem_variation.h"
#include "ds_appsrv_utils.h"

#include "ds_profile.h"
#include "ds_profile_3gpp.h"
#include "ds_profile_3gpp2.h"

#include "ds_appsrv_task.h"
#include "ds_andsf_api.h"
#include "ds_autoreg.h"
#include "ds_lowlatency_dispatcher.h"
#include "ds_abss_event_handler.h"

#include "dstask_v.h"
#include "modem_mem.h"

#include "ds3gcfgmgr.h"
#include "ds3gdevmgr.h"
#include "ds3geventmgr.h"
#include "ps_sys.h"
#include "dsutil.h"

#include "cm.h"
#include "mcfg_fs.h"

#include <stringl/stringl.h>
#include "ps_iface_defs.h"

#define DS_APPSRV_CFG_MAX_WRITE_BUFFER_SIZE 255

/*===========================================================================
                       LOCAL VARIABLE DEFINITIONS
===========================================================================*/
static const char ds_appsrv_nv_conf_efs_path[] =
"/nv/item_files/data/appsrv/ds_lteu_meas_enabled\n\
/nv/item_files/data/appsrv/ds_lwa_meas_enabled\n\
/nv/item_files/data/appsrv/wqe_prof_wait_timer\n\
/nv/item_files/data/appsrv/substitute_modem_cne_profiles_functionality_enabled\n\
/nv/item_files/data/appsrv/data_wcdma_latency_opt_config\n\
/nv/item_files/data/appsrv/wqe_accumulate_timer\n\
/nv/item_files/data/appsrv/low_latency_capability\n\
/nv/item_files/data/appsrv/low_latency_ul_flow_opt_enable\n\
/nv/item_files/data/appsrv/low_latency_aps_enable\n";


static const char DS_APPSRV_UTILS_HEX_NUMBERS[] = "0123456789ABCDEF";

/*===========================================================================
                       LOCAL FUNCTION DEFINITIONS
===========================================================================*/
/*===========================================================================
FUNCTION      DS_APPSRV_NV_REFRESH_EV_HDLR

DESCRIPTION   Handles MCFG NV refresh event

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
static void ds_appsrv_nv_refresh_ev_hdlr
(
  uint32 *msg
)
{
  ds_sys_subscription_enum_type subs_id;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  subs_id = (ds_sys_subscription_enum_type)(*msg);

  /*------------------------------------------------------------------------
    Reset and re-initialize ANDSF
  ------------------------------------------------------------------------*/
  ds_andsf_nv_refresh(subs_id);
  
  /*------------------------------------------------------------------------
    Reset and re-initialize Low Latency
  ------------------------------------------------------------------------*/
  ds_lowlatency_nv_refresh(subs_id);

  if (FALSE == ds3geventmgr_set_and_notify_nv_refresh_status(DS3GEVENTMGR_CLIENT_ID_APPSRV,
                                                             ds3gsubsmgr_subs_id_ds_to_cm(subs_id)))
  {
     DATA_APPSRV_MSG1(
                     MSG_LEGACY_ERROR,
                     "ds_appsrv_nv_refresh_ev_hdlr: fail to notify NV refresh event, ds_subs_id: %d",
                     subs_id
                     );
  }
  modem_mem_free((void*)msg, MODEM_MEM_CLIENT_DATA_CRIT);
  return;
} /* ds_appsrv_nv_refresh_ev_hdlr */



/*===========================================================================
FUNCTION      DS_APPSRV_DATA_ROAMING_SETTING_EV_HDLR

DESCRIPTION   Handles device data roaming setting event

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
static void ds_appsrv_data_roaming_setting_ev_hdlr
(
  uint32 *msg
)
{
  ds_appsrv_data_roaming_setting_s_type *data_roaming_setting_ptr;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if (NULL == msg)
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR,
                     "ds_appsrv_data_roaming_setting_ev_hdlr, Invalid input data");
    return;
  }
  data_roaming_setting_ptr = (ds_appsrv_data_roaming_setting_s_type*)msg;

  /*------------------------------------------------------------------------
   ANDSF handle data roaming setting
  ------------------------------------------------------------------------*/
  ds_andsf_data_roaming_setting_chg_hdlr(data_roaming_setting_ptr);

  modem_mem_free((void*)data_roaming_setting_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
  return;
}/*ds_appsrv_data_roaming_setting_ev_hdlr*/



/*===========================================================================
FUNCTION      DS_APPSRV_ICCID_INFO_EV_HDLR

DESCRIPTION   Handles iccid info change event from ds3g

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
static void ds_appsrv_iccid_info_ev_hdlr
(
  uint32 *msg
)
{
  ds_appsrv_iccid_info_s_type *iccid_info_ptr;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if (NULL == msg)
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR,
                     "ds_appsrv_iccid_info_ev_hdlr, Invalid input data");
    return;
  }

  iccid_info_ptr = (ds_appsrv_iccid_info_s_type*)msg;

  /*------------------------------------------------------------------------
   ANDSF wqe mgr handle iccid change
  ------------------------------------------------------------------------*/
  ds_andsf_wqe_prof_mgr_iccid_info_chg_hdlr(iccid_info_ptr);
  ds_autoreg_iccid_info_ev_hdlr(iccid_info_ptr);
  ds_abss_iccid_info_chg_hdlr(iccid_info_ptr);

  modem_mem_free((void*)iccid_info_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
  return;
}/*ds_appsrv_iccid_info_ev_hdlr()*/
/*===========================================================================
FUNCTION      DS_APPSRV_MODE_LPM_EV_HDLR

DESCRIPTION   Handles LPM event from ds3g

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
static void ds_appsrv_mode_lpm_ev_hdlr
(
  uint32 *msg
)
{
  ds_sys_subscription_enum_type subs_id;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (NULL == msg)
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR,
                     "ds_appsrv_mode_lpm_ev_hdlr, Invalid input data");
    return;
  }
  subs_id = (ds_sys_subscription_enum_type)(*msg);
  /*------------------------------------------------------------------------
    Reset and re-config Low Latency
  ------------------------------------------------------------------------*/
  ds_lowlatency_lpm_on_handling(subs_id);

  modem_mem_free((void*)msg, MODEM_MEM_CLIENT_DATA_CRIT);
  return;
}/*ds_appsrv_mode_lpm_ev_hdlr()*/
/*===========================================================================
FUNCTION      DS_APPSRV_MODE_ONLINE_EV_HDLR

DESCRIPTION   Handles LPM event from ds3g

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
static void ds_appsrv_mode_online_ev_hdlr
(
  uint32 *msg
)
{
  ds_sys_subscription_enum_type subs_id;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (NULL == msg)
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR,
                     "ds_appsrv_mode_online_ev_hdlr, Invalid input data");
    return;
  }
  subs_id = (ds_sys_subscription_enum_type)(*msg);
  /*------------------------------------------------------------------------
    Reset and re-config Low Latency
  ------------------------------------------------------------------------*/
  ds_lowlatency_mode_online_handling(subs_id);

  modem_mem_free((void*)msg, MODEM_MEM_CLIENT_DATA_CRIT);
  return;
}/*ds_appsrv_mode_online_ev_hdlr()*/
/*===========================================================================
FUNCTION      DS_APPSRV_DEVICE_SIM_MODE_CHANGE_EV_HDLR

DESCRIPTION   Handles device sim mode change event from ds3g

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
static void ds_appsrv_device_sim_mode_change_ev_hdlr
(
  uint32 *msg
)
{
  ds_sys_subscription_enum_type subs_id;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (NULL == msg)
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR,
                     "ds_appsrv_device_sim_mode_change_ev_hdlr, Invalid input data");
    return;
  }
  subs_id = (ds_sys_subscription_enum_type)(*msg);
  /*------------------------------------------------------------------------
    Reset and re-config Low Latency
  ------------------------------------------------------------------------*/
  ds_lowlatency_device_sim_mode_change_handling(subs_id);

  modem_mem_free((void*)msg, MODEM_MEM_CLIENT_DATA_CRIT);
  return;
}/*ds_appsrv_mode_online_ev_hdlr()*/

/*===========================================================================
FUNCTION      DS_APPSRV_DS3G_EVENT_CB

DESCRIPTION   Called by clients to register to get DS3G event.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_appsrv_ds3g_event_cb
(
  ds3geventmgr_event_type   event_id,
  ds3geventmgr_filter_type *filter_info_ptr,
  void                     *event_info_ptr,
  void                     *data_ptr
)
{
  ps_sys_subscription_enum_type *ps_subs_id_ptr = NULL;
  ps_sys_subscription_enum_type          temp_subs_id = PS_SYS_DEFAULT_SUBS;
  ds_appsrv_data_roaming_setting_s_type *data_roaming_setting_ptr = NULL;
  ds3geventmgr_iccid_info_type *ds3g_iccid_info_ptr = NULL;
  ds_appsrv_iccid_info_s_type  *appsrv_iccid_info_ptr = NULL;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if ( filter_info_ptr == NULL )
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Invalid input data: No Filter Info");
    return;
  }

  if((filter_info_ptr->ps_subs_id < PS_SYS_PRIMARY_SUBS ||
     filter_info_ptr->ps_subs_id > PS_SYS_TERTIARY_SUBS))
  {
    DATA_APPSRV_MSG1(MSG_LEGACY_ERROR, "Invalid input data, Subs ID: %d",
                     filter_info_ptr->ps_subs_id);
    return;
  }

  DATA_APPSRV_MSG2(MSG_LEGACY_HIGH,
                   "ds_appsrv_ds3g_event_cb, subs_id: %d, event:%d",
                   filter_info_ptr->ps_subs_id, event_id);

  switch (event_id)
  {
    case DS3GEVENTMGR_NV_REFRESH_EV:
    {
      ps_subs_id_ptr = modem_mem_alloc(sizeof(ps_sys_subscription_enum_type),
                                     MODEM_MEM_CLIENT_DATA_CRIT);

      if (ps_subs_id_ptr == NULL)
      {
        DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Failed to allocate memory");
        break;
      }

      *ps_subs_id_ptr = filter_info_ptr->ps_subs_id;

      if(!ds_appsrv_put_cmd_ex(&ds_appsrv_nv_refresh_ev_hdlr, (uint32*)ps_subs_id_ptr))
      {
        DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Failed to send dstask msg");

        modem_mem_free((void*)ps_subs_id_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
      }
    }
    break;

    case DS3GEVENTMGR_DATA_ROAMING_CHANGED_EV:
    {
      data_roaming_setting_ptr = modem_mem_alloc(sizeof(ds_appsrv_data_roaming_setting_s_type),
                                                        MODEM_MEM_CLIENT_DATA_CRIT);

      if (data_roaming_setting_ptr == NULL)
      {
        DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Failed to allocate memory");
        break;
      }

      data_roaming_setting_ptr->subs_id = (ds_sys_subscription_enum_type)filter_info_ptr->ps_subs_id;
      ds3gdevmgr_get_roaming_settings(&data_roaming_setting_ptr->is_data_enabled,
                                      ds3gsubsmgr_subs_id_ds_to_cm(data_roaming_setting_ptr->subs_id));

      if(!ds_appsrv_put_cmd_ex(&ds_appsrv_data_roaming_setting_ev_hdlr, (uint32*)data_roaming_setting_ptr))
      {
        DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Failed to send dstask msg");
        modem_mem_free((void*)data_roaming_setting_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
      }
    }
    break;

    case DS3GEVENTMGR_MMGSDI_ICCID_INFO_EV:
    {
      ds3g_iccid_info_ptr = (ds3geventmgr_iccid_info_type*)event_info_ptr;

      appsrv_iccid_info_ptr = modem_mem_alloc(sizeof(ds_appsrv_iccid_info_s_type),
                                              MODEM_MEM_CLIENT_DATA_CRIT);
      if (appsrv_iccid_info_ptr == NULL)
      {
        DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Failed to allocate memory");
        break;
      }

      memset(appsrv_iccid_info_ptr, 0, sizeof(ds_appsrv_iccid_info_s_type));

      appsrv_iccid_info_ptr->subs_id = ds3gsubsmgr_subs_id_cm_to_ds(ds3g_iccid_info_ptr->subs_id);
      appsrv_iccid_info_ptr->is_valid = ds3g_iccid_info_ptr->is_valid;
      appsrv_iccid_info_ptr->iccid_len = ds3g_iccid_info_ptr->iccid_len;

      memscpy(appsrv_iccid_info_ptr->iccid,
              appsrv_iccid_info_ptr->iccid_len,
              ds3g_iccid_info_ptr->iccid,
              appsrv_iccid_info_ptr->iccid_len);

      if(!ds_appsrv_put_cmd_ex(&ds_appsrv_iccid_info_ev_hdlr, (uint32*)appsrv_iccid_info_ptr))
      {
        DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Failed to send dstask msg");
        modem_mem_free((void*)appsrv_iccid_info_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
      }
    }
    break;
	
  	case DS3GEVENTMGR_MODE_LPM_EV:
  	{
     for(temp_subs_id = PS_SYS_PRIMARY_SUBS; 
         temp_subs_id < PS_SYS_SUBS_MAX;
         temp_subs_id++)
     {
       ps_subs_id_ptr = modem_mem_alloc(sizeof(ps_sys_subscription_enum_type),
                                        MODEM_MEM_CLIENT_DATA_CRIT);
	
       if (ps_subs_id_ptr == NULL)
       {
         DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Failed to allocate memory");
         break;
       }

       *ps_subs_id_ptr = temp_subs_id;
	
       if(!ds_appsrv_put_cmd_ex(&ds_appsrv_mode_lpm_ev_hdlr, (uint32*)ps_subs_id_ptr))
       {
         DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Failed to send dstask msg");	
         modem_mem_free((void*)ps_subs_id_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
         break;
       }
     }
   }
   break;

     case DS3GEVENTMGR_MODE_ONLINE_EV:
	 {
       ps_subs_id_ptr = modem_mem_alloc(sizeof(ps_sys_subscription_enum_type),
                                       MODEM_MEM_CLIENT_DATA_CRIT);
	
       if (ps_subs_id_ptr == NULL)
       {
         DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Failed to allocate memory");
         break;
       }
       *ps_subs_id_ptr = filter_info_ptr->ps_subs_id;
	
       if(!ds_appsrv_put_cmd_ex(&ds_appsrv_mode_online_ev_hdlr, (uint32*)ps_subs_id_ptr))
       {
         DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Failed to send dstask msg");
	
         modem_mem_free((void*)ps_subs_id_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
       }
      }
      break;
case DS3GEVENTMGR_DEVICE_SIM_MODE_CHANGE_EV:
	{
      ps_subs_id_ptr = modem_mem_alloc(sizeof(ps_sys_subscription_enum_type),
                                       MODEM_MEM_CLIENT_DATA_CRIT);
	
      if (ps_subs_id_ptr == NULL)
      {
        DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Failed to allocate memory");
        break;
      }
      *ps_subs_id_ptr = filter_info_ptr->ps_subs_id;
	
      if(!ds_appsrv_put_cmd_ex(&ds_appsrv_device_sim_mode_change_ev_hdlr, (uint32*)ps_subs_id_ptr))
      {
        DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Failed to send dstask msg");
	
        modem_mem_free((void*)ps_subs_id_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
      }
    }
	break;
  default:
    break;
  }

  return;
}/* ds_appsrv_ds3g_event_cb */



static uint32 ds_appsrv_utils_cfg_get_token_id(
                                               uint32     num_token_name,
                                               const char token_name_list[][DS_APPSRV_CFG_MAX_TOKEN_NAME_LEN],
                                               char*      from,
                                               char*      to
                                               )
{
  uint32 len = 0;
  uint32 i = 0;

  if(NULL == from || NULL == to || to <= from)
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "ds_appsrv_utils_cfg_get_token_id: invalid input");
    return num_token_name;
  }

  len = to - from;

  for(i = 0; i < num_token_name; i++)
  {
    if( len == strlen(token_name_list[i]) && 0 == strncasecmp(from, token_name_list[i], len) )
    {
      return i;
    }
  }
  return num_token_name;
}



/*===========================================================================
                    EXTERNALIZED FUNCTIONS DEFINITIONS
===========================================================================*/
/*===========================================================================
FUNCTION      DS_APPSRV_EFS_ITEM_FILE_PATH_SIZE

DESCRIPTION   Returns the total APPSRV EFS item file paths size

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  uint32: total APPSRV EFS item file paths size

SIDE EFFECTS  None
===========================================================================*/
uint32 ds_appsrv_efs_item_file_path_size
(
  void
)
{
  return sizeof(ds_appsrv_nv_conf_efs_path);
}/* ds_appsrv_efs_item_file_path_size */



/*===========================================================================
FUNCTION      DS_APPSRV_UPDATE_DATA_CONFIG_INFO

DESCRIPTION   Puts all the required nv item file paths in the
              data_config_info.conf file. Would be called during powerup.

DEPENDENCIES  None

RETURN VALUE  length of string added to buffer for success.
              -1 for failure.

SIDE EFFECTS  None
===========================================================================*/
int32 ds_appsrv_update_data_config_info
(
  char   *file_paths_buf
)
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if (NULL == file_paths_buf)
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "file_paths_buf is NULL");
    return -1;
  }

  memscpy(file_paths_buf, strlen(ds_appsrv_nv_conf_efs_path),
          ds_appsrv_nv_conf_efs_path, strlen(ds_appsrv_nv_conf_efs_path));

  return strlen(ds_appsrv_nv_conf_efs_path);
} /* ds_appsrv_update_data_config_info() */

/*===========================================================================
FUNCTION      DS_APPSRV_UTILS_GET_PROFILE_ID_FROM_APN

DESCRIPTION   This function searches for profile with specified APN. If 
              an existing one is not found, it creates a non-persistent
              profile and modify the APN.
 
DEPENDENCIES  None

RETURN VALUE  -1             if failed to obtain usable profile ID
              int:profile_id otherwise
              TRUE  if non-persistent profile is created
              FALSE if no profile is created

SIDE EFFECTS  None
===========================================================================*/
boolean ds_appsrv_utils_get_profile_id_from_apn
(
  ds_appsrv_apn_name_type    *apn_ptr,
  int                        *profile_id,
  ds_profile_tech_etype       profile_tech,
  ds_profile_subs_etype       subs_id,
  boolean                     allow_creation
)
{
  ds_profile_list_type        search_list_info; 
  ds_profile_list_info_type   list_info;
  ds_profile_info_type        profile_info;
  ds_profile_itr_type         itr;
  uint8                       value[128];
  ds_profile_status_etype     status = DS_PROFILE_REG_RESULT_SUCCESS;
  ds_profile_config_type      config_ptr = {0};
  ds_profile_hndl_type        profile_hndl = NULL;
  ds_profile_identifier_type  ident;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if (profile_id == NULL)
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Null profile ID pointer passed, return");  
    return FALSE;
  }

  if ( apn_ptr == NULL )
  {
    *profile_id = 0;
    return FALSE;
  }

  if (subs_id > DS_PROFILE_ACTIVE_SUBSCRIPTION_MAX)
  {
    DATA_APPSRV_MSG1(MSG_LEGACY_ERROR, 
                  "Invalid sub_id %d passed.",
                  subs_id);
    return FALSE;
  }

  if (profile_tech == DS_PROFILE_TECH_3GPP)
  {
    ident = (ds_profile_identifier_type)DS_PROFILE_3GPP_PROFILE_PARAM_PDP_CONTEXT_APN;
  }
  else if (profile_tech == DS_PROFILE_TECH_3GPP2)
  {
    ident = (ds_profile_identifier_type)DS_PROFILE_3GPP2_PROFILE_PARAM_APN_STRING;    
  }
  else
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "Invalid profile tech passed, return");
    return FALSE;
  }
  /*---------------------------------------------------------------------
    Search for existing profile, profile number may have changed
   ---------------------------------------------------------------------*/
  search_list_info.dfn = DS_PROFILE_LIST_SEARCH_PROFILES;
  search_list_info.ident = ident;
  search_list_info.info.len = apn_ptr->apn_len;
  search_list_info.info.buf = (void*)apn_ptr->apn_name;

  if( DS_PROFILE_REG_RESULT_SUCCESS == ds_profile_get_list_itr_per_sub(
                                         profile_tech,
                                         &search_list_info,
                                         &itr,
                                         subs_id)
    )
  {
    memset(&list_info, 0, sizeof (ds_profile_list_info_type));
    memset(&profile_info, 0, sizeof (ds_profile_info_type));

    list_info.name = &profile_info;
    list_info.name->len = sizeof(value);
    list_info.name->buf = (void*)value;

    status = ds_profile_get_info_by_itr(itr, &list_info);

    ds_profile_itr_destroy(itr);

    DATA_APPSRV_MSG2(MSG_LEGACY_MED, 
                     "Found profile ID %d, status %d", 
                     list_info.num, status);

    if ( DS_PROFILE_REG_RESULT_SUCCESS == status )
    {
      *profile_id = list_info.num;
    }
    else
    {
      *profile_id = -1;
    }

    return FALSE;
  }

  /*---------------------------------------------------------------------
    Create a new profile with requested APN
   ---------------------------------------------------------------------*/
  if (allow_creation)
  {
    config_ptr.config_mask = DS_PROFILE_CONFIG_MASK_PERSISTENCE;
    config_ptr.is_persistent = FALSE;
    config_ptr.config_mask |= DS_PROFILE_CONFIG_MASK_SUBS_ID;
    config_ptr.subs_id = subs_id;

    status = ds_profile_create_ex(profile_tech, 
                                  &config_ptr, 
                                  (ds_profile_num_type*)profile_id);

    if( DS_PROFILE_REG_RESULT_SUCCESS != status )
    {
      DATA_APPSRV_MSG1(MSG_LEGACY_ERROR, "Failed to create profile %d", status);

      *profile_id = -1;
      return FALSE;
    }

	  status = ds_profile_begin_transaction_per_sub(DS_PROFILE_TRN_RW,
                                                    profile_tech, 
                                                    (ds_profile_num_type)*profile_id,
                                                    subs_id,
                                                    &profile_hndl);
    
    if (status == DS_PROFILE_REG_RESULT_SUCCESS)
    {
      profile_info.buf = (void*)apn_ptr->apn_name;
      profile_info.len = apn_ptr->apn_len;

      status = ds_profile_set_param(profile_hndl,
                                    ident,
                                    &profile_info);
      
      (void)ds_profile_end_transaction(profile_hndl, DS_PROFILE_ACTION_COMMIT);
    }

    if (status != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      DATA_APPSRV_MSG1(MSG_LEGACY_ERROR, 
                       "Failed to modify profile APN status %d, deleting profile",
                       status );

      ds_profile_delete_per_sub(profile_tech, 
                                (ds_profile_num_type)*profile_id, 
                                subs_id); 

      *profile_id = -1;
      return FALSE;
    }

    DATA_APPSRV_MSG1(MSG_LEGACY_MED, "Created profile %d", *profile_id);
    return TRUE;
  }
  else
  {
    *profile_id = -1;
    return FALSE;
  }
}/* ds_appsrv_utils_get_profile_id_from_apn */

/*===========================================================================
FUNCTION      DS_APPSRV_UTILS_INIT

DESCRIPTION   This function initializes all APPSRV common services.
              The function is called during power up.

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_appsrv_utils_init(void)
{
  uint32 subs_id;
  ds3geventmgr_filter_type filter_info = {0};
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  filter_info.tech = PS_SYS_TECH_ALL;

  for(subs_id = PS_SYS_PRIMARY_SUBS; subs_id < PS_SYS_SUBS_MAX; subs_id++)
  {
    filter_info.ps_subs_id = (ps_sys_subscription_enum_type)subs_id;
    if(ds3geventmgr_event_reg(DS3GEVENTMGR_NV_REFRESH_EV,
                              DS3GEVENTMGR_CLIENT_ID_APPSRV,
                              &filter_info,
                              ds_appsrv_ds3g_event_cb,
                              NULL) == FALSE)
    {
      DATA_APPSRV_MSG1(MSG_LEGACY_ERROR,
                       "Registration for NV REFRESH EV failed for subs:%d!", subs_id);
    }

    if(ds3geventmgr_event_reg(DS3GEVENTMGR_DATA_ROAMING_CHANGED_EV,
                              DS3GEVENTMGR_CLIENT_ID_APPSRV,
                              &filter_info,
                              ds_appsrv_ds3g_event_cb,
                              NULL) == FALSE)
    {
      DATA_APPSRV_MSG1(MSG_LEGACY_ERROR,
                       "Registration for DATA ROAMING SETTING EV failed for subs:%d!", subs_id);
    }

    if(ds3geventmgr_event_reg(DS3GEVENTMGR_MMGSDI_ICCID_INFO_EV,
                              DS3GEVENTMGR_CLIENT_ID_APPSRV,
                              &filter_info,
                              ds_appsrv_ds3g_event_cb,
                              NULL) == FALSE)
    {
      DATA_APPSRV_MSG1(MSG_LEGACY_ERROR,
                       "Registration for MMGSDI ICCID INFO_EV failed for subs:%d!", subs_id);
    }
	if(ds3geventmgr_event_reg(DS3GEVENTMGR_MODE_LPM_EV,
                              DS3GEVENTMGR_CLIENT_ID_APPSRV,
                              &filter_info,
                              ds_appsrv_ds3g_event_cb,
                              NULL) == FALSE)
    {
      DATA_APPSRV_MSG1(MSG_LEGACY_ERROR,
                       "Registration for DS3GEVENTMGR_MODE_LPM_EV failed for subs:%d!", subs_id);
    }
	if(ds3geventmgr_event_reg(DS3GEVENTMGR_MODE_ONLINE_EV,
                              DS3GEVENTMGR_CLIENT_ID_APPSRV,
                              &filter_info,
                              ds_appsrv_ds3g_event_cb,
                              NULL) == FALSE)
    {
      DATA_APPSRV_MSG1(MSG_LEGACY_ERROR,
                       "Registration for DS3GEVENTMGR_MODE_ONLINE_EV failed for subs:%d!", subs_id);
    }
    if(ds3geventmgr_event_reg(DS3GEVENTMGR_DEVICE_SIM_MODE_CHANGE_EV,
                              DS3GEVENTMGR_CLIENT_ID_APPSRV,
                              &filter_info,
                              ds_appsrv_ds3g_event_cb,
                              NULL) == FALSE)
    {
      DATA_APPSRV_MSG1(MSG_LEGACY_ERROR,
                       "Registration for DS3GEVENTMGR_DEVICE_SIM_MODE_CHANGE_EV failed for subs:%d!", subs_id);
    }
  }
}/* ds_appsrv_utils_init */




boolean ds_appsrv_utils_parse_cfg_file(
                                       const char*                               cfg_file_path,
                                       uint32                                    num_token_name,
                                       const char                                token_name_list[][DS_APPSRV_CFG_MAX_TOKEN_NAME_LEN],
                                       ds_appsrv_utils_cfg_set_token_content_fnc cfg_set_fnc,
                                       void*                                     user_data_ptr
                                       )
{
  char    *from      = NULL, *to = NULL;
  boolean param_name = TRUE;
  uint32  token_id   = num_token_name;
  ds3gcfgmgr_efs_token_type              efs_db;
  ds3gcfgmgr_efs_token_parse_status_type ret_val;

  memset(&efs_db, 0, sizeof(ds3gcfgmgr_efs_token_type));

  if(-1 == ds3gcfgmgr_efs_file_init(cfg_file_path, &efs_db))
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "ds_appsrv_utils_parse_cfg_file, failed to open config file");
    return FALSE;
  }
  efs_db.seperator = ':';

  DATA_APPSRV_MSG_SPRINTF_2(MSG_LEGACY_HIGH,
                            "ds_appsrv_utils_parse_cfg_file, file_path: %s, read_buffer_size:%d ",
                            efs_db.efs_file_path,
                            efs_db.read_buffer_size);

  while( DS3GCFGMGR_EFS_TOKEN_PARSE_EOF != ( ret_val = ds3gcfgmgr_efs_tokenizer( &efs_db, &from, &to ) ) )
  {
    if (DS3GCFGMGR_EFS_TOKEN_PARSE_FAILURE == ret_val)
    {
      DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "ds_appsrv_utils_parse_cfg_file, Token Parse Failure");
      break;
    }

    if((from == to) || (DS3GCFGMGR_EFS_TOKEN_PARSE_EOL == ret_val))
    {
      /*----------------------------------------------------------------------
      Skip empty tokens.
      ----------------------------------------------------------------------*/
      if (param_name == FALSE)
      {
        param_name = TRUE;
        efs_db.seperator = ':';
        DATA_APPSRV_MSG1(MSG_LEGACY_LOW, "ds_appsrv_utils_parse_cfg_file, Token Id %d Param value NULL", token_id);
      }
      continue;
    }
    else if (DS3GCFGMGR_EFS_TOKEN_PARSE_SUCCESS == ret_val)
    {
      if (param_name == TRUE)
      {
        token_id = ds_appsrv_utils_cfg_get_token_id(num_token_name, token_name_list, from, to);
        param_name = FALSE;
        /* set the seperator as ';' */
        efs_db.seperator = ';';
      }
      else
      {
        if(token_id < num_token_name)
        {
          cfg_set_fnc(token_id, from, to, user_data_ptr);
        }
        else
        {
          DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "ds_appsrv_utils_parse_cfg_file, unrecognized param name, skip");
        }
        param_name = TRUE;
        /* Set the seperator as ':' */
        efs_db.seperator = ':';
      }
    }
  }
  ds3gcfgmgr_efs_file_close(&efs_db);
  return TRUE;
}



boolean ds_appsrv_utils_generate_cfg_file(
                                      const char*                                    cfg_file_path,
                                      uint32                                         num_token_name,
                                      const char                                     token_name_list[][DS_APPSRV_CFG_MAX_TOKEN_NAME_LEN],
                                      ds_appsrv_utils_cfg_generate_token_content_fnc cfg_get_param_str_fnc,
                                      void*                                          user_data_ptr
                                      )
{
  char*   content_buf = NULL;
  char*   tmp_buf     = NULL;
  uint32  i           = 0;
  int     fd          = -1;

  if( NULL == cfg_file_path ||
      -1   == ( fd = mcfg_fopen(cfg_file_path, MCFG_FS_O_CREAT | MCFG_FS_O_TRUNC | MCFG_FS_O_WRONLY | MCFG_FS_O_AUTODIR, MCFG_FS_DEFFILEMODE, MCFG_FS_TYPE_EFS, MCFG_FS_SUBID_NONE) ) )
  {
    DATA_APPSRV_MSG1(MSG_LEGACY_ERROR, "ds_appsrv_utils_generate_cfg_file - Failed to open file %d", cfg_file_path);
    return FALSE;
  }


  content_buf = (char*) modem_mem_alloc(DS_APPSRV_CFG_MAX_WRITE_BUFFER_SIZE * 2 + 1, MODEM_MEM_CLIENT_DATA);
  if(content_buf == NULL)
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "memory allocation failed");
    return FALSE;
  }

  tmp_buf = (char*) modem_mem_alloc(DS_APPSRV_CFG_MAX_WRITE_BUFFER_SIZE + 1, MODEM_MEM_CLIENT_DATA);
  if(tmp_buf == NULL)
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "memory allocation failed");
    modem_mem_free(content_buf, MODEM_MEM_CLIENT_DATA);
    return FALSE;
  }


  for(i = 0; i < num_token_name; ++i)
  {
    int written = 0;
    uint32 write_size = 0;

    memset(content_buf, 0, DS_APPSRV_CFG_MAX_WRITE_BUFFER_SIZE * 2 + 1);
    memset(tmp_buf, 0, DS_APPSRV_CFG_MAX_WRITE_BUFFER_SIZE + 1);
    cfg_get_param_str_fnc(i, tmp_buf, DS_APPSRV_CFG_MAX_WRITE_BUFFER_SIZE, user_data_ptr);

    if( 0 == strlen(tmp_buf) )
      continue;

    snprintf(content_buf, DS_APPSRV_CFG_MAX_WRITE_BUFFER_SIZE, "%s:%s;\r\n", token_name_list[i], tmp_buf);

    write_size = strlen(content_buf);

    while(0 < write_size)
    {
      written = mcfg_fwrite(fd, content_buf, write_size, MCFG_FS_TYPE_EFS);
      if(0 > written || write_size < written)
        break;

      write_size -= written;
    }

    if(0 != write_size)
      break;
  }

  mcfg_fclose(fd, MCFG_FS_TYPE_EFS);
  modem_mem_free(content_buf, MODEM_MEM_CLIENT_DATA);
  modem_mem_free(tmp_buf, MODEM_MEM_CLIENT_DATA);
  return TRUE;
}




void uctohex(uint8 value, char dst[DS_APPSRV_UTILS_UINT8_HEX_LEN])
{
  uint8 tmpval = value >> 4;
  tmpval = tmpval & 0x0F;
  dst[0] = DS_APPSRV_UTILS_HEX_NUMBERS[tmpval];

  tmpval = value & 0x0F;
  dst[1] = DS_APPSRV_UTILS_HEX_NUMBERS[tmpval];
}


/*===========================================================================
FUNCTION      DS_APPSRV_UTILS_IS_PDN_FAILURE_TEMPORARY_EX

DESCRIPTION   This function checks the netdown reason, and according to the preconfiged hard failure list, 
              determines if soft failure or hard failure.
 
PARAMETERS     
 
DEPENDENCIES  None 

RETURN VALUE  boolean
              TRUE  - soft failure
              FALSE - hard failure

SIDE EFFECTS  None
===========================================================================*/
boolean ds_appsrv_utils_is_pdn_failure_temporary_ex
(
  uint32* cust_hard_failure_list, 
  uint32 cust_hard_failure_list_size, 
  uint32 failure_reason
)
{
  uint32 i = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if((NULL != cust_hard_failure_list) && (0 != cust_hard_failure_list_size))
  {
    for(i = 0; i < cust_hard_failure_list_size; ++i)
    {
      if(cust_hard_failure_list[i] == failure_reason)
      {
        return FALSE;
      }
    }
  }

  return ds_appsrv_utils_is_pdn_failure_temporary(failure_reason);
}/*ds_appsrv_utils_is_pdn_failure_temporary_ex()*/


/*===========================================================================
FUNCTION      DS_APPSRV_UTILS_IS_PDN_FAILURE_TEMPORARY

DESCRIPTION   This function checks the netdown reason and determines if soft failure or hard failure.
 
PARAMETERS     
 
DEPENDENCIES  None 

RETURN VALUE  boolean
              TRUE  - soft failure
              FALSE - hard failure

SIDE EFFECTS  None
===========================================================================*/
boolean ds_appsrv_utils_is_pdn_failure_temporary(uint32 failure_reason)
{
  boolean ret = FALSE;

  switch(failure_reason)
  {
    // Temporary/permanent
    case PS_NET_DOWN_REASON_OPERATOR_DETERMINED_BARRING:
    case PS_NET_DOWN_REASON_OPTION_UNSUBSCRIBED:
    case PS_NET_DOWN_REASON_INVALID_MANDATORY_INFO:
    case PS_NET_DOWN_REASON_PROTOCOL_ERROR:
    case PS_NET_DOWN_REASON_APN_TYPE_CONFLICT:
    case PS_NET_DOWN_REASON_MESSAGE_TYPE_UNSUPPORTED:
    case PS_NET_DOWN_REASON_UNKNOWN_INFO_ELEMENT:
    case PS_NET_DOWN_REASON_CONDITIONAL_IE_ERROR:

    // Temporary failure + retry
    case PS_NET_DOWN_REASON_NOT_SPECIFIED:
    case PS_NET_DOWN_REASON_NAS_SIGNALLING_ERROR:
    case PS_NET_DOWN_REASON_LLC_SNDCP_FAILURE:
    case PS_NET_DOWN_REASON_INSUFFICIENT_RESOURCES:
    case PS_NET_DOWN_REASON_GGSN_REJECT:
    case PS_NET_DOWN_REASON_ACTIVATION_REJECT:
    case PS_NET_DOWN_REASON_OPTION_NOT_SUPPORTED:
    case PS_NET_DOWN_REASON_OPTION_TEMP_OOO:
    case PS_NET_DOWN_REASON_NSAPI_ALREADY_USED:
    case PS_NET_DOWN_REASON_REGULAR_DEACTIVATION:
    case PS_NET_DOWN_REASON_QOS_NOT_ACCEPTED:
    case PS_NET_DOWN_REASON_NETWORK_FAILURE:
    case PS_NET_DOWN_REASON_UMTS_REATTACH_REQ:
    case PS_NET_DOWN_REASON_LAST_PDN_DISCONN_NOT_ALLOWED:
    case PS_NET_DOWN_REASON_ESM_INFO_NOT_RECEIVED:
    case PS_NET_DOWN_REASON_PDN_CONN_DOES_NOT_EXIST:
    case PS_NET_DOWN_REASON_COLLISION_WITH_NW_INIT_REQ:
    case PS_NET_DOWN_REASON_UNSUPPORTED_QCI_VALUE:
    case PS_NET_DOWN_REASON_BEARER_HANDLING_NOT_SUPPORTED:
    case PS_NET_DOWN_REASON_MAX_ACTIVE_PDP_CONTEXT_REACHED:
    case PS_NET_DOWN_REASON_INVALID_TRANSACTION_ID:
    case PS_NET_DOWN_REASON_MSG_TYPE_NONCOMPATIBLE_STATE:
    case PS_NET_DOWN_REASON_MSG_AND_PROTOCOL_STATE_UNCOMPATIBLE:
    case PS_NET_DOWN_REASON_EMM_ACCESS_BARRED:
    case PS_NET_DOWN_REASON_EMERGENCY_IFACE_ONLY:
    case PS_NET_DOWN_REASON_IP_ADDRESS_MISMATCH:
    case PS_NET_DOWN_REASON_IFACE_AND_POL_FAMILY_MISMATCH:
    case PS_NET_DOWN_REASON_EMM_ACCESS_BARRED_INFINITE_RETRY:
    case PS_NET_DOWN_REASON_AUTH_FAILURE_ON_EMERGENCY_CALL:
    case PS_NET_DOWN_REASON_INVALID_DNS_ADDR:
    case PS_NET_DOWN_REASON_INVALID_PCSCF_DNS_ADDR:
    case PS_NET_DOWN_REASON_TEST_LOOPBACK_MODE_A_OR_B_ENABLED:
    case PS_NET_DOWN_REASON_INTERNAL_CALL_ENDED:
    case PS_NET_DOWN_REASON_CLOSE_IN_PROGRESS:
    case PS_NET_DOWN_REASON_APP_PREEMPTED:
    case PS_NET_DOWN_REASON_ERR_PDN_IPV4_CALL_DISALLOWED:
    case PS_NET_DOWN_REASON_ERR_PDN_IPV4_CALL_THROTTLED:
    case PS_NET_DOWN_REASON_ERR_PDN_IPV6_CALL_DISALLOWED:
    case PS_NET_DOWN_REASON_ERR_PDN_IPV6_CALL_THROTTLED:
    case PS_NET_DOWN_REASON_MODEM_RESTART:
    case PS_NET_DOWN_REASON_PDP_PPP_NOT_SUPPORTED:
    case PS_NET_DOWN_REASON_UNPREFERRED_RAT:
    case PS_NET_DOWN_REASON_PHYS_LINK_CLOSE_IN_PROGRESS:
    case PS_NET_DOWN_REASON_APN_PENDING_HANDOVER:
    case PS_NET_DOWN_REASON_PROFILE_BEARER_INCOMPATIBLE:
    case PS_NET_DOWN_REASON_INTERNAL_MMGSDI_CARD_EVT:
    case PS_NET_DOWN_REASON_INTERNAL_MPIT_EXPIRED:
    case PS_NET_DOWN_REASON_INTERNAL_IPV6_ADDR_TRANSFER_FAILED:
    case PS_NET_DOWN_REASON_INTERNAL_TRAT_SWAP_FAILED:
    case PS_NET_DOWN_REASON_INTERNAL_EHRPD_TO_HRPD_FALLBACK:
    case PS_NET_DOWN_REASON_INTERNAL_PDN_INACTIVITY_TIMER_EXPIRED:
    case PS_NET_DOWN_REASON_MAX_V4_CONNECTIONS:
    case PS_NET_DOWN_REASON_MAX_V6_CONNECTIONS:
    case PS_NET_DOWN_REASON_APN_MISMATCH:
    case PS_NET_DOWN_REASON_IP_VERSION_MISMATCH:
    case PS_NET_DOWN_REASON_DUN_CALL_DISALLOWED:
    case PS_NET_DOWN_REASON_INVALID_PROFILE:
    case PS_NET_DOWN_REASON_INTERNAL_EPC_NONEPC_TRANSITION:
    case PS_NET_DOWN_REASON_INVALID_PROFILE_ID:
    case PS_NET_DOWN_REASON_IFACE_IN_USE:
    case PS_NET_DOWN_REASON_IP_PDP_MISMATCH:
    case PS_NET_DOWN_REASON_APN_DISALLOWED_ON_ROAMING:
    case PS_NET_DOWN_REASON_APN_PARAM_CHANGE:
    case PS_NET_DOWN_REASON_IFACE_IN_USE_CFG_MATCH:
    case PS_NET_DOWN_REASON_NULL_APN_DISALLOWED:
    case PS_NET_DOWN_REASON_THERMAL_MITIGATION:
    case PS_NET_DOWN_REASON_SUBS_ID_MISMATCH:
    case PS_NET_DOWN_REASON_DATA_SETTINGS_DISABLED:
    case PS_NET_DOWN_REASON_DATA_ROAMING_SETTINGS_DISABLED:
    case PS_NET_DOWN_REASON_APN_FORMAT_INVALID:
    case PS_NET_DOWN_REASON_DDS_CALL_ABORT:
    case PS_NET_DOWN_REASON_VALIDATION_FAILURE:
    case PS_NET_DOWN_REASON_INVALID_APN_NAME:
    case PS_NET_DOWN_REASON_INTERCEPT:
    case PS_NET_DOWN_REASON_REORDER:
    case PS_NET_DOWN_REASON_INCOM_CALL:
    case PS_NET_DOWN_REASON_ALERT_STOP:
    case PS_NET_DOWN_REASON_ACTIVATION:
    case PS_NET_DOWN_REASON_MAX_ACCESS_PROBE:
    case PS_NET_DOWN_REASON_NO_RESPONSE_FROM_BS:
    case PS_NET_DOWN_REASON_ALREADY_IN_TC:
    case PS_NET_DOWN_REASON_USER_CALL_ORIG_DURING_SMS:
    case PS_NET_DOWN_REASON_NO_CDMA_SRV:
    case PS_NET_DOWN_REASON_MC_ABORT:
    case PS_NET_DOWN_REASON_PSIST_NG:
    case PS_NET_DOWN_REASON_RETRY_ORDER:
    case PS_NET_DOWN_REASON_ACCESS_BLOCK:
    case PS_NET_DOWN_REASON_ACCESS_BLOCK_ALL:
    case PS_NET_DOWN_REASON_IS707B_MAX_ACC:
    case PS_NET_DOWN_REASON_CALL_ORIG_THROTTLED:
    case PS_NET_DOWN_REASON_USER_CALL_ORIG_DURING_VOICE_CALL:
    case PS_NET_DOWN_REASON_CONF_FAILED:
    case PS_NET_DOWN_REASON_INCOM_REJ:
    case PS_NET_DOWN_REASON_NO_GW_SRV:
    case PS_NET_DOWN_REASON_NO_GPRS_CONTEXT:
    case PS_NET_DOWN_REASON_MS_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK:
    case PS_NET_DOWN_REASON_IMPLICITLY_DETACHED:
    case PS_NET_DOWN_REASON_PDP_DUPLICATE:
    case PS_NET_DOWN_REASON_UE_RAT_CHANGE:
    case PS_NET_DOWN_REASON_CONGESTION:
    case PS_NET_DOWN_REASON_NO_PDP_CONTEXT_ACTIVATED:
    case PS_NET_DOWN_REASON_ACCESS_CLASS_DSAC_REJECTION:
    case PS_NET_DOWN_REASON_PDP_ACTIVATE_MAX_RETRY_FAILED:
    case PS_NET_DOWN_REASON_RAB_FAILURE:
    case PS_NET_DOWN_REASON_ESM_UNKNOWN_EPS_BEARER_CONTEXT:
    case PS_NET_DOWN_REASON_DRB_RELEASED_AT_RRC:
    case PS_NET_DOWN_REASON_NAS_SIG_CONN_RELEASED:
    case PS_NET_DOWN_REASON_EMM_DETACHED:
    case PS_NET_DOWN_REASON_EMM_ATTACH_FAILED:
    case PS_NET_DOWN_REASON_EMM_ATTACH_STARTED:
    case PS_NET_DOWN_REASON_LTE_NAS_SERVICE_REQ_FAILED:
    case PS_NET_DOWN_REASON_ESM_ACTIVE_DEDICATED_BEARER_REACTIVATED_BY_NW:
    case PS_NET_DOWN_REASON_ESM_LOWER_LAYER_FAILURE:
    case PS_NET_DOWN_REASON_ESM_SYNC_UP_WITH_NW:
    case PS_NET_DOWN_REASON_ESM_NW_ACTIVATED_DED_BEARER_WITH_ID_OF_DEF_BEARER:
    case PS_NET_DOWN_REASON_ESM_BAD_OTA_MESSAGE:
    case PS_NET_DOWN_REASON_ESM_DS_REJECTED_THE_CALL:
    case PS_NET_DOWN_REASON_ESM_CONTEXT_TRANSFERED_DUE_TO_IRAT:
    case PS_NET_DOWN_REASON_DS_EXPLICIT_DEACT:
    case PS_NET_DOWN_REASON_ESM_LOCAL_CAUSE_NONE:
    case PS_NET_DOWN_REASON_LTE_NAS_SERVICE_REQ_FAILED_NO_THROTTLE:
    case PS_NET_DOWN_REASON_ACL_FAILURE:
    case PS_NET_DOWN_REASON_LTE_NAS_SERVICE_REQ_FAILED_DS_DISALLOW:
    case PS_NET_DOWN_REASON_EMM_T3417_EXPIRED:
    case PS_NET_DOWN_REASON_EMM_T3417_EXT_EXPIRED:
    case PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_TXN:
    case PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_HO:
    case PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_CONN_REL:
    case PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_RLF:
    case PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_CTRL_NOT_CONN:
    case PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE:
    case PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_ABORTED:
    case PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_ACCESS_BARRED:
    case PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_CELL_RESEL:
    case PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_CONFIG_FAILURE:
    case PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_TIMER_EXPIRED:
    case PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_LINK_FAILURE:
    case PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_NOT_CAMPED:
    case PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_SI_FAILURE:
    case PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_CONN_REJECT:
    case PS_NET_DOWN_REASON_LRRC_CONN_REL_NORMAL:
    case PS_NET_DOWN_REASON_LRRC_CONN_REL_RLF:
    case PS_NET_DOWN_REASON_LRRC_CONN_REL_CRE_FAILURE:
    case PS_NET_DOWN_REASON_LRRC_CONN_REL_OOS_DURING_CRE:
    case PS_NET_DOWN_REASON_LRRC_CONN_REL_ABORTED:
    case PS_NET_DOWN_REASON_LRRC_CONN_REL_SIB_READ_ERROR:
    case PS_NET_DOWN_REASON_DETACH_WITH_REATTACH_LTE_NW_DETACH:
    case PS_NET_DOWN_REASON_DETACH_WITH_OUT_REATTACH_LTE_NW_DETACH:
    case PS_NET_DOWN_REASON_ESM_PROC_TIME_OUT:
    case PS_NET_DOWN_REASON_INVALID_CONNECTION_ID:
    case PS_NET_DOWN_REASON_INVALID_NSAPI:
    case PS_NET_DOWN_REASON_INVALID_PRI_NSAPI:
    case PS_NET_DOWN_REASON_INVALID_FIELD:
    case PS_NET_DOWN_REASON_RAB_SETUP_FAILURE:
    case PS_NET_DOWN_REASON_PDP_ESTABLISH_MAX_TIMEOUT:
    case PS_NET_DOWN_REASON_PDP_MODIFY_MAX_TIMEOUT:
    case PS_NET_DOWN_REASON_PDP_INACTIVE_MAX_TIMEOUT:
    case PS_NET_DOWN_REASON_PDP_LOWERLAYER_ERROR:
    case PS_NET_DOWN_REASON_PPD_UNKNOWN_REASON:
    case PS_NET_DOWN_REASON_PDP_MODIFY_COLLISION:
    case PS_NET_DOWN_REASON_PDP_MBMS_REQUEST_COLLISION:
    case PS_NET_DOWN_REASON_MBMS_DUPLICATE:
    case PS_NET_DOWN_REASON_SM_PS_DETACHED:
    case PS_NET_DOWN_REASON_SM_NO_RADIO_AVAILABLE:
    case PS_NET_DOWN_REASON_SM_ABORT_SERVICE_NOT_AVAILABLE:
    case PS_NET_DOWN_REASON_MESSAGE_EXCEED_MAX_L2_LIMIT:
    case PS_NET_DOWN_REASON_SM_NAS_SRV_REQ_FAILURE:
    case PS_NET_DOWN_REASON_RRC_CONN_EST_FAILURE_REQ_ERROR:
    case PS_NET_DOWN_REASON_RRC_CONN_EST_FAILURE_TAI_CHANGE:
    case PS_NET_DOWN_REASON_RRC_CONN_EST_FAILURE_RF_UNAVAILABLE:
    case PS_NET_DOWN_REASON_RRC_CONN_REL_ABORTED_IRAT_SUCCESS:
    case PS_NET_DOWN_REASON_RRC_CONN_REL_RLF_SEC_NOT_ACTIVE:
    case PS_NET_DOWN_REASON_RRC_CONN_REL_IRAT_TO_LTE_ABORTED:
    case PS_NET_DOWN_REASON_RRC_CONN_REL_IRAT_FROM_LTE_TO_G_CCO_SUCCESS:
    case PS_NET_DOWN_REASON_RRC_CONN_REL_IRAT_FROM_LTE_TO_G_CCO_ABORTED:
    case PS_NET_DOWN_REASON_IMSI_UNKNOWN_IN_HSS:
    case PS_NET_DOWN_REASON_MSC_TEMPORARILY_NOT_REACHABLE:
    case PS_NET_DOWN_REASON_CS_DOMAIN_NOT_AVAILABLE:
    case PS_NET_DOWN_REASON_ESM_FAILURE:
    case PS_NET_DOWN_REASON_MAC_FAILURE:
    case PS_NET_DOWN_REASON_SYNCH_FAILURE:
    case PS_NET_DOWN_REASON_UE_SECURITY_CAPABILITIES_MISMATCH:
    case PS_NET_DOWN_REASON_SECURITY_MODE_REJ_UNSPECIFIED:
    case PS_NET_DOWN_REASON_NON_EPS_AUTH_UNACCEPTABLE:
    case PS_NET_DOWN_REASON_CS_FALLBACK_CALL_EST_NOT_ALLOWED:
    case PS_NET_DOWN_REASON_NO_EPS_BEARER_CONTEXT_ACTIVATED:
    case PS_NET_DOWN_REASON_EMM_INVALID_STATE:
    case PS_NET_DOWN_REASON_NAS_LAYER_FAILURE:
    case PS_NET_DOWN_REASON_MULTI_PDN_NOT_ALLOWED:
    case PS_NET_DOWN_REASON_EMBMS_NOT_ENABLED:
    case PS_NET_DOWN_REASON_PENDING_REDIAL_CALL_CLEANUP:
    case PS_NET_DOWN_REASON_EMBMS_REGULAR_DEACTIVATION:
    case PS_NET_DOWN_REASON_TLB_REGULAR_DEACTIVATION:
    case PS_NET_DOWN_REASON_LOWER_LAYER_REGISTRATION_FAILURE:
    case PS_NET_DOWN_REASON_DETACH_EPS_SERVICES_NOT_ALLOWED:
    case PS_NET_DOWN_REASON_SM_INTERNAL_PDP_DEACTIVATION:
    case PS_NET_DOWN_REASON_CD_GEN_OR_BUSY:
    case PS_NET_DOWN_REASON_CHG_HDR:
    case PS_NET_DOWN_REASON_EXIT_HDR:
    case PS_NET_DOWN_REASON_HDR_NO_SESSION:
    case PS_NET_DOWN_REASON_HDR_ORIG_DURING_GPS_FIX:
    case PS_NET_DOWN_REASON_HDR_CS_TIMEOUT:
    case PS_NET_DOWN_REASON_HDR_RELEASED_BY_CM:
    case PS_NET_DOWN_REASON_COLLOC_ACQ_FAIL:
    case PS_NET_DOWN_REASON_OTASP_COMMIT_IN_PROG:
    case PS_NET_DOWN_REASON_NO_HYBR_HDR_SRV:
    case PS_NET_DOWN_REASON_HDR_NO_LOCK_GRANTED:
    case PS_NET_DOWN_REASON_HOLD_OTHER_IN_PROG:
    case PS_NET_DOWN_REASON_HDR_FADE:
    case PS_NET_DOWN_REASON_HDR_ACC_FAIL:
    case PS_NET_DOWN_REASON_NO_SRV:
    case PS_NET_DOWN_REASON_FADE:
    case PS_NET_DOWN_REASON_REL_NORMAL:
    case PS_NET_DOWN_REASON_ACC_IN_PROG:
    case PS_NET_DOWN_REASON_ACC_FAIL:
    case PS_NET_DOWN_REASON_REDIR_OR_HANDOFF:
    case PS_NET_DOWN_REASON_CM_UNKNOWN_ERROR:
    case PS_NET_DOWN_REASON_PHONE_IN_USE:
    case PS_NET_DOWN_REASON_NO_COLLOC_HDR:
    case PS_NET_DOWN_REASON_MIP_FA_ERR_REQUESTED_LIFETIME_TOO_LONG:
    case PS_NET_DOWN_REASON_MIP_FA_ERR_UNKNOWN_CHALLENGE:
    case PS_NET_DOWN_REASON_MIP_FA_ERR_STALE_CHALLENGE:
    case PS_NET_DOWN_REASON_MIP_HA_ERR_REGISTRATION_ID_MISMATCH:
    case PS_NET_DOWN_REASON_PPP_ERR_TIMEOUT:
    case PS_NET_DOWN_REASON_PPP_ERR_CLOSE_IN_PROGRESS:
    case PS_NET_DOWN_REASON_NV_REFRESH_IN_PROGRESS:
    case PS_NET_DOWN_REASON_EHRPD_ERR_VSNCP_TIMEOUT:
    case PS_NET_DOWN_REASON_EHRPD_ERR_VSNCP_3GPP2I_PDN_ID_IN_USE:
    case PS_NET_DOWN_REASON_IPV6_ERR_PREFIX_UNAVAILABLE:
    case PS_NET_DOWN_REASON_TUN_CONN_DENIED:
    case PS_NET_DOWN_REASON_TUN_CONN_CR_ROR_FAILED:
    case PS_NET_DOWN_REASON_TUN_CONN_SAP_L2_FAILED:
    case PS_NET_DOWN_REASON_TUN_LCP_THROTTLED:
    case PS_NET_DOWN_REASON_TUN_VIRTUAL_CONN_THROTTLED:
    case PS_NET_DOWN_REASON_TUN_NON_EHPRD_MODE:
    case PS_NET_DOWN_REASON_TUN_NON_TUNNEL_MODE:
    case PS_NET_DOWN_REASON_TUN_PREREG_DISALLOWED:
    case PS_NET_DOWN_REASON_TUN_CONN_SAP_L3_FAILED:
    case PS_NET_DOWN_REASON_TUN_CONN_SAP_TIMEOUT:
    case PS_NET_DOWN_REASON_TUN_CONN_SAP_NO_SESSION:
    case PS_NET_DOWN_REASON_TUN_CONN_SAP_PREREG_NOT_ALLOW:
    case PS_NET_DOWN_REASON_TUN_CONN_SAP_ACTIVE_HO:
    case PS_NET_DOWN_REASON_TUN_CONN_SAP_NO_SRV:
    case PS_NET_DOWN_REASON_TUN_NO_LTE_SRV:
    case PS_NET_DOWN_REASON_TUN_HDR_SESSION_CLOSED:
    case PS_NET_DOWN_REASON_IWLAN_S2B_IKEV2_MSG_TIMEOUT:
    case PS_NET_DOWN_REASON_IWLAN_S2B_NETWORK_FAILURE:
    case PS_NET_DOWN_REASON_IWLAN_S2B_ROAMING_NOT_ALLOWED:
    case PS_NET_DOWN_REASON_IWLAN_S2B_RAT_DISALLOWED:
    case PS_NET_DOWN_REASON_IWLAN_S2B_NO_SUBSCRIPTION:
    case PS_NET_DOWN_REASON_IWLAN_S2B_NETWORK_TOO_BUSY:
    case PS_NET_DOWN_REASON_IWLAN_S2B_DNS_RESOLUTION_CONFIG_FAILURE:
    case PS_NET_DOWN_REASON_IWLAN_S2B_DNS_RESOLUTION_NAME_FAILURE:
    case PS_NET_DOWN_REASON_IWLAN_S2B_DNS_RESOLUTION_TIMEOUT:
    case PS_NET_DOWN_REASON_IWLAN_S2B_BLOCKING_FAILURE:
    case PS_NET_DOWN_REASON_IWLAN_S2B_DNS_RESOLUTION_FAILURE:
    case PS_NET_DOWN_REASON_IWLAN_S2B_NW_INIT_CONN_DOWN:
    case PS_NET_DOWN_REASON_IWLAN_S2B_IKEV2_DPD_FAILURE:
    case PS_NET_DOWN_REASON_IWLAN_S2B_IKE_REKEY_FAILURE:
    case PS_NET_DOWN_REASON_IWLAN_S2B_IPSEC_REKEY_FAILURE:
    case PS_NET_DOWN_REASON_HANDOFF_PREF_SYS_BACK_TO_SRAT:

    // temporary failure / other
    case PS_NET_DOWN_REASON_IFACE_MISMATCH:              // temp, iface mismatch
    case PS_NET_DOWN_REASON_COMPANION_IFACE_IN_USE:      // temp, ip mismatch
    case PS_NET_DOWN_REASON_IWLAN_S2B_IKEV2_CERT_EXPIRY: // temp, need to make new call
      ret = TRUE;
      break;

    default:
      ret = FALSE;
      break;
  }

  DATA_APPSRV_MSG1(MSG_LEGACY_HIGH, "ds_appsrv_utils_is_pdn_failure_temporary - is_temp_failure:%d", ret);
  return ret;
}/*ds_appsrv_utils_is_pdn_failure_temporary*/

