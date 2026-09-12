/*===========================================================================

                            TM_DS_IFace  Module

DESCRIPTION

Copyright (c) 2015 - 2019 Qualcomm Technologies, Inc.
All Rights Reserved.
Qualcomm Technologies Confidential and Proprietary. All Rights Reserved. 

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_ds_iface.c#2 $

when        who     what, where, why
--------    ---     ---------------------------------------------------------- 
11/08/19    py      Klockwork fix
02/19/19    skm     SUPL over WIFI phase 2  
11/30/18    skm     Initial Release
===========================================================================*/
#include <string.h>
#include "msg.h"
#include "sys.h"
#include "tm_common.h"
#include "tm_ds_iface.h"
#include "ds_sys_conf.h"
#include "ds_profile.h"
#include "ds_profile_3gpp.h"
#include "pd_comms_api.h"
#include "pdapibuf.h"
#include "tm_data.h"

#define TM_DS_CONFIG_MIN_SUB_ID DS_SYS_PRIMARY_SUBS
#define TM_DS_CONFIG_MAX_SUB_ID DS_SYS_SUBS_MAX

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                        GLOBAL DATA DECLARATIONS                         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static tm_ds_apn_name_type tm_ds_emergency_apn = {0};
static cgps_SrvSystemType tm_ds_emergency_srv_sys = CGPS_SRV_SYS_NONE;
static ds_sys_system_status_ex_type *tm_ds_system_status = NULL;

/* APN information for emergency connection per sub*/
static tm_ds_sub_apn_entry tm_ds_sys_status_sub[DS_SYS_SUBS_MAX-1];


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                         STATIC FUNCTION DECLARATIONS                    */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*===========================================================================
FUNCTION tm_ds_find_emergency_profile_info

DESCRIPTION
  Function to lookup Emergency Profile and extract APN name and
  Profile number.

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None
===========================================================================*/
static void tm_ds_find_emergency_profile_info(void);

/*===========================================================================
FUNCTION tm_ds_update_emergency_sys_info

DESCRIPTION
  Function to Update Emergency System info from DS System Info
  Profile number.

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None
===========================================================================*/
static void tm_ds_update_emergency_sys_info(ds_sys_system_status_ex_type* ds_sys_info,
                                            ds_sys_subscription_enum_type sub_id);

/*===========================================================================
FUNCTION tm_ds_update_emergency_sys_info

DESCRIPTION
  Function to Update Emergency System info from DS System Info
  Profile number.

DEPENDENCIES
  None

RETURN VALUE
 cgps_SrvSystemType : CGPS Servign System Type enum

SIDE EFFECTS
  None
===========================================================================*/
static cgps_SrvSystemType tm_ds_map_sys_info(ds_sys_rat_ex_enum_type ds_sys);

/*===========================================================================
FUNCTION tm_ds_send_event_msg

DESCRIPTION
  Function to send IPC message for DS Serving System Events.

DEPENDENCIES
  None

RETURN VALUE
 None 

SIDE EFFECTS
  None
===========================================================================*/
static void tm_ds_send_event_msg(tm_ds_event_info_s_type* ds_ss_event);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                         GLOBAL FUNCTION DECLARATIONS                    */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*===========================================================================
FUNCTION tm_ds_reg_callbacks

DESCRIPTION
  Registers callback with Data Services to listen to DS SYSTEM STATUS events.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tm_ds_reg_callbacks(void);

/*===========================================================================
FUNCTION tm_ds_event_cb

DESCRIPTION
  Callback function to process DS Events.

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None
===========================================================================*/
void tm_ds_event_cb(ds_sys_tech_enum_type               tech_type,
                    ds_sys_event_enum_type              event_name,
                    ds_sys_subscription_enum_type       subscription_id,
                    void *event_info_ptr,
                    void *user_data_ptr);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                   STATIC FUNCTION DEFINITIONS                           */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*===========================================================================
FUNCTION tm_ds_send_event_msg

DESCRIPTION
  Function to send IPC message for DS Serving System Events.

DEPENDENCIES
  None

RETURN VALUE
 None 

SIDE EFFECTS
  None
===========================================================================*/
static void tm_ds_send_event_msg(tm_ds_event_info_s_type* ds_ss_event)
{
  os_IpcMsgType           *ipc_msg_ptr = NULL;
  tm_ds_event_info_s_type *pz_ds_event = NULL;
  boolean                 status = FALSE;

  if ( NULL == ds_ss_event ) 
  {
     MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_ds_send_event_msg: NULL payload");
     return;
  }

  ipc_msg_ptr = os_IpcCreate(sizeof(tm_ds_event_info_s_type), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if ( ipc_msg_ptr == NULL )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_ds_send_event_msg: IPC message allocation failed");
  }
  else
  {
     pz_ds_event = (tm_ds_event_info_s_type *)ipc_msg_ptr->p_Data;

     ipc_msg_ptr->q_MsgId = TM_CORE_DS_MSG_ID_EVENT_NTFY;

     pz_ds_event->ds_event_type = ds_ss_event->ds_event_type;
     pz_ds_event->srv_system    = ds_ss_event->srv_system; 
     
     status    = os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM);
     if (status == FALSE)
     {
       MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_ds_send_event_msg: Error in sending DS Event msg_id=0x%x to TM thread", ipc_msg_ptr->q_MsgId);
       status = os_IpcDelete(ipc_msg_ptr);
       ipc_msg_ptr = NULL;
     } /* End status */
     else
     {
       MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "tm_ds_send_event_msg: Sending DS Event msg_id=0x%x to TM thread", ipc_msg_ptr->q_MsgId);
     }
  }

}


/*===========================================================================
FUNCTION tm_ds_find_emergency_profile_info

DESCRIPTION
  Function to lookup Emergency Profile and extract APN name and
  Profile number.

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None
===========================================================================*/
static void tm_ds_find_emergency_profile_info(void)
{
  ds_profile_list_type search_list_info;
  ds_profile_list_info_type list_info;
  ds_profile_info_type profile_info;
  ds_profile_itr_type itr;
  uint8 value[128];
  ds_profile_3gpp_emergency_calls_are_supported emergency_calls_supported = TRUE;
  boolean result = FALSE;
  ds_profile_subs_etype  sub = DS_PROFILE_ACTIVE_SUBSCRIPTION_1; 

  search_list_info.dfn = DS_PROFILE_LIST_SEARCH_PROFILES;

  search_list_info.ident =  DS_PROFILE_3GPP_PROFILE_PARAM_EMERGENCY_CALLS_SUPPORTED; /* identifies the field to search upon */

  search_list_info.info.len = sizeof(ds_profile_3gpp_emergency_calls_are_supported);
  search_list_info.info.buf = &emergency_calls_supported; /* value of the field we are looking for in the profile */

  for (sub = DS_PROFILE_ACTIVE_SUBSCRIPTION_1; sub < DS_PROFILE_ACTIVE_SUBSCRIPTION_MAX; sub ++) 
  {
     if (ds_profile_get_list_itr_per_sub(DS_PROFILE_TECH_3GPP, &search_list_info, &itr, sub) == DS_PROFILE_REG_RESULT_SUCCESS)
     {

       memset(&list_info, 0, sizeof(ds_profile_list_info_type));
       memset(&profile_info, 0, sizeof(ds_profile_info_type));

       list_info.name = &profile_info;
       list_info.name->len  = sizeof(value);
       list_info.name->buf  = (void *)value;

       MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Going through the list of profiles matched");
       if (ds_profile_get_info_by_itr(itr, &list_info) == DS_PROFILE_REG_RESULT_SUCCESS)
       {

         ds_profile_status_etype status = DS_PROFILE_REG_RESULT_FAIL;
         ds_profile_tech_etype   profile_type = DS_PROFILE_TECH_3GPP;
         ds_profile_trn_etype    trn_type     = DS_PROFILE_TRN_READ;
         ds_profile_action_etype act          = DS_PROFILE_ACTION_CANCEL;
         ds_profile_num_type     prf_num  = list_info.num;

         ds_profile_hndl_type    profile_hndl = NULL;
         ds_profile_identifier_type ident;
         ds_profile_info_type       info;


         MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Profile Number: %d", list_info.num);

         ident = DS_PROFILE_3GPP_PROFILE_PARAM_PDP_CONTEXT_APN;

         status = ds_profile_begin_transaction(trn_type,
                                               profile_type,
                                               prf_num,
                                               &profile_hndl);

         if ((status == DS_PROFILE_REG_RESULT_SUCCESS) &&
             (profile_hndl != NULL))
         {
           MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Getting the APN Name");

           memset(&(tm_ds_sys_status_sub[sub-1].apn_info.apn_string) ,0,DS_SYS_MAX_APN_LEN );
           info.buf = &(tm_ds_sys_status_sub[sub-1].apn_info.apn_string);
           info.len = DS_SYS_MAX_APN_LEN;

           status = ds_profile_get_param(profile_hndl,
                                         ident,
                                         &info);
           /*Successfully got the APN Name. Ensure to capture APN length and 
             Mark the record as Valid.*/
           tm_ds_sys_status_sub[sub-1].apn_info.apn_string_len = 
                           strlen((const char*)tm_ds_sys_status_sub[sub-1].apn_info.apn_string);
           tm_ds_sys_status_sub[sub-1].ds_sub = (ds_sys_subscription_enum_type)sub;

           MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Got APN length: %d, SUB : %d",
                 tm_ds_sys_status_sub[sub-1].apn_info.apn_string_len,
                 tm_ds_sys_status_sub[sub-1].ds_sub);
           
         }

         if (profile_hndl != NULL)
         {
           (void)ds_profile_end_transaction(profile_hndl, act);
         }
         result = TRUE; /*Even if getting PDP type was not sucesfull, 
                                             We got the profile, so return SUCCESS*/
       }

       if ( TRUE == result )
         MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Emergency APN name get succeded");
       else
         MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Emergency APN name get failed");
       /*Free the DS List iterator object*/
       ds_profile_itr_destroy(itr);
     }
  }
  return ;
}

/*===========================================================================
FUNCTION tm_ds_update_emergency_sys_info

DESCRIPTION
  Function to Update Emergency System info from DS System Info
  Profile number.

DEPENDENCIES
  None

RETURN VALUE
 cgps_SrvSystemType : CGPS Servign System Type enum

SIDE EFFECTS
  None
===========================================================================*/
static cgps_SrvSystemType tm_ds_map_sys_info(ds_sys_rat_ex_enum_type ds_sys)
{
   cgps_SrvSystemType srv_sys = CGPS_SRV_SYS_NONE;

   switch (ds_sys)
   {
     case DS_SYS_RAT_EX_3GPP_WCDMA:
       srv_sys = CGPS_SRV_SYS_WCDMA;
       break;             

     case DS_SYS_RAT_EX_3GPP_GERAN:
       srv_sys = CGPS_SRV_SYS_GSM;
       break;

     case DS_SYS_RAT_EX_3GPP_LTE:
       srv_sys = CGPS_SRV_SYS_LTE;
       break;

     case DS_SYS_RAT_EX_3GPP_TDSCDMA:
      srv_sys = CGPS_SRV_SYS_TDSCDMA;
      break;

     case DS_SYS_RAT_EX_3GPP_WLAN:
     case DS_SYS_RAT_EX_WLAN:
     case DS_SYS_RAT_EX_3GPP2_WLAN:
       srv_sys = CGPS_SRV_SYS_WLAN;
       break;

     case DS_SYS_RAT_EX_3GPP2_1X:
       srv_sys = CGPS_SRV_SYS_CDMA;
       break;

     case DS_SYS_RAT_EX_3GPP2_HRPD:
     case DS_SYS_RAT_EX_3GPP2_EHRPD:
       srv_sys = CGPS_SRV_SYS_HDR;
       break;         
     
     default: /*srv_sys is set to CGPS_SRV_SYS_NONE at initialization*/
       break;
   }
   return (srv_sys);
}

/*===========================================================================
FUNCTION tm_ds_update_emergency_sys_info

DESCRIPTION
  Function to Update Emergency System info from DS System Info
  Profile number.

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None
===========================================================================*/
static void tm_ds_update_emergency_sys_info(ds_sys_system_status_ex_type* ds_sys_info,
                                            ds_sys_subscription_enum_type sub_id)
{
  tm_ds_event_info_s_type              z_ds_ss_event_info={0};

  uint16 l_i = 0; 

  if (NULL == ds_sys_info)
  {
    MSG(MSG_SSID_GPS, MSG_LEGACY_ERROR, "TM_DS: DS Sys Info NULL pointer");
    return;
  }

  if( (DS_SYS_DEFAULT_SUBS >= sub_id) || (DS_SYS_SUBS_MAX <= sub_id))
  {
    MSG_1(MSG_SSID_GPS, MSG_LEGACY_ERROR, "TM_DS: Sub id %d out of range [0:2]",sub_id-1);
    return;
  }
  
  /*Iterate through the DS System Info structure and get the SYS info 
    associated with the Emergency APN*/
  for ( l_i = 0; l_i < ds_sys_info->num_avail_apns; l_i++ )
  {
    if ( 0 == memcmp ( &(tm_ds_sys_status_sub[sub_id-1].apn_info.apn_string),
                       &ds_sys_info->apn_pref_sys_info[l_i].apn_name,
                       tm_ds_sys_status_sub[sub_id-1].apn_info.apn_string_len )
       )
    {
      /*The first index in the available DS systems for a given APN name is 
        the system on which the data call is established*/
      MSG_2(MSG_SSID_GPS, MSG_LEGACY_MED, "TM_DS Tech: %d, RAT: %d ",
             ds_sys_info->apn_pref_sys_info[l_i].avail_sys[0].technology,
             ds_sys_info->apn_pref_sys_info[l_i].avail_sys[0].rat_value);

      /*Map the DS RAT to CGPS SRV SYSTEM.*/
      tm_ds_emergency_srv_sys = tm_ds_map_sys_info(ds_sys_info->apn_pref_sys_info[l_i].avail_sys[0].rat_value);
      tm_ds_sys_status_sub[sub_id-1].srv_sys = tm_ds_emergency_srv_sys;

      if ( CGPS_SRV_SYS_WLAN ==  tm_ds_emergency_srv_sys ) 
      {
#ifndef FEATURE_GNSS_SA
        {
          pd_comms_sys_info_change_params_type z_sys_info_change_params={0};

          /*Notify PD_COMMS of WLAN RAT*/  
          z_sys_info_change_params.srv_system = SYS_SYS_MODE_WLAN;
          pd_comms_app_srv_sys_change_notification(z_sys_info_change_params);
        }
#endif /* ! FEATURE_GNSS_SA */
        
        /*Notify TM Core, to end LPP CP call flow, if it is active*/
        z_ds_ss_event_info.ds_event_type = TM_DS_SS_EVENT;
        z_ds_ss_event_info.srv_system = SYS_SYS_MODE_WLAN;
        z_ds_ss_event_info.subs = sub_id-1;
        tm_ds_send_event_msg(&z_ds_ss_event_info);

        tm_ds_sys_status_sub[sub_id-1].v_subActive = TRUE;
      }
      else
      {
        tm_ds_sys_status_sub[sub_id-1].v_subActive = FALSE;
      }
    }
  }

  MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED, "TM_DS: tm_ds_update_emergency_sys_info : %d, sub_id: %d, WLAN active: %d",
                                      tm_ds_emergency_srv_sys,
                                      sub_id,
                                      tm_ds_sys_status_sub[sub_id-1].v_subActive);

  if (NULL != ds_sys_info)
  {
    (void)os_MemFree((void **)&ds_sys_info);
  }

  return;
}

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                   GLOBAL FUNCTION DEFINITIONS                           */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*===========================================================================
FUNCTION tm_ds_event_cb

DESCRIPTION
  Callback function to process DS Events.

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None
===========================================================================*/
void tm_ds_event_cb(ds_sys_tech_enum_type               tech_type,
                    ds_sys_event_enum_type              event_name,
                    ds_sys_subscription_enum_type       subscription_id,
                    void *event_info_ptr,
                    void *user_data_ptr)
{
  ds_sys_system_status_ex_type *ds_sys_status = NULL;


  MSG_2(MSG_SSID_GPS, MSG_LEGACY_MED, "DSS event = %d, sub_id: %d", event_name, subscription_id);

  if ( subscription_id < DS_SYS_DEFAULT_SUBS || subscription_id > DS_SYS_TERTIARY_SUBS )
  {
    MSG(MSG_SSID_GPS, MSG_LEGACY_ERROR, "TM_DS: DS Event cb with wrong subscription_id");
    return;
  }

  if (NULL == event_info_ptr)
  {
    MSG(MSG_SSID_GPS, MSG_LEGACY_ERROR, "TM_DS: DS Event cb NULL payload");
    return;
  }

  if (DS_SYS_EVENT_SYSTEM_STATUS_EX != event_name)
  {
    MSG(MSG_SSID_GPS, MSG_LEGACY_ERROR, "TM_DS: DS Event cb not SYSTEM_STATUS_EX");
    return;
  }

  ds_sys_status = (ds_sys_system_status_ex_type*)event_info_ptr;


  /* On receiving the call back, Copy the DS SYSTEM INFO and update the
     Serving system*/

  tm_ds_system_status = (ds_sys_system_status_ex_type*)os_MemAlloc(sizeof(ds_sys_system_status_ex_type), OS_MEM_SCOPE_TASK); 

  if ( NULL == tm_ds_system_status) 
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "os_MemAlloc failed, tm_ds_event_cb");
    return;
  }

  memscpy((void*)tm_ds_system_status,sizeof(ds_sys_system_status_ex_type),
                (void*)ds_sys_status,sizeof(ds_sys_system_status_ex_type));
        
  tm_ds_update_emergency_sys_info(tm_ds_system_status,subscription_id);

  return;
}

/*===========================================================================
FUNCTION tm_ds_reg_callbacks

DESCRIPTION
  Registers callback with Data Services to listen to DS SYSTEM STATUS events.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tm_ds_reg_callbacks(void)
{
  int16  dss_errno        = 0;
  int16  dss_ret_val      = 0;
  ds_sys_subscription_enum_type  sub = DS_SYS_PRIMARY_SUBS; 

  /* Register for DS_SYS_EVENT_SYSTEM_STATUS_EX event */
  for (sub = TM_DS_CONFIG_MIN_SUB_ID; sub < TM_DS_CONFIG_MAX_SUB_ID; sub ++) 
  {
     dss_ret_val =  ds_sys_event_reg_ex(DS_SYS_TECH_ALL,
                                        DS_SYS_EVENT_SYSTEM_STATUS_EX,
                                        sub,
                                        tm_ds_event_cb,
                                        NULL,
                                        &dss_errno);
    if ( 0 != dss_ret_val )
    {
      MSG_2(MSG_SSID_GPS, MSG_LEGACY_ERROR, "TM_DS: DS Event Reg failed ret_val=0x%x, err 0x%x", dss_ret_val, dss_errno);
    }
  }


  return;
}

/*===========================================================================
FUNCTION tm_ds_get_emergency_sys_info

DESCRIPTION
  Function to get the APN name associated with Emergency DS Profile.

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None
===========================================================================*/
void tm_ds_get_emergency_sys_info(void)
{

  /* on 911 dial get the DS APN name associated with the Emergency PDN.
     The APN name is saved in tm_ds_emergency_apn structure.
     */
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_ds_get_emergency_sys_info");
  tm_ds_find_emergency_profile_info();

  return;
}

/*===========================================================================

FUNCTION tm_ds_get_emergency_srv_sys

DESCRIPTION
   Function returns the Serving System on which Data services has brought up
   the Emergency data call.
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern cgps_SrvSystemType tm_ds_get_emergency_srv_sys()
{
  uint8 l_i=0;
  cgps_SrvSystemType srv_system = CGPS_SRV_SYS_NONE;

  for (l_i = 0; l_i < (TM_DS_CONFIG_MAX_SUB_ID-1) ; l_i++)
  {
     if( TRUE == tm_ds_sys_status_sub[l_i].v_subActive) 
         srv_system = tm_ds_sys_status_sub[l_i].srv_sys;
  }
  return (srv_system);
  //return(tm_ds_emergency_srv_sys);
}

/*===========================================================================

FUNCTION  tm_ds_iface_event_proc

DESCRIPTION 
  Handles an incoming DS event

RETURN VALUE 
  None

DEPENDENCIES

===========================================================================*/
void tm_ds_iface_event_proc( tm_ds_event_info_s_type* pz_data )
{
  tm_ds_event_info_s_type *pz_ds_event_info = NULL;
  tm_phone_event_payload_u_type *cm_evt_payload_ptr = NULL;

  if (pz_data == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_DS_IFace: Null pointer to function ");
    return;
  }
  pz_ds_event_info = (tm_ds_event_info_s_type*)pz_data;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM_DS_IFace: tm_cm_iface_event_proc() getting called");

  switch (pz_ds_event_info->ds_event_type)
  {
    case TM_DS_SS_EVENT:
        /* Send Phone event to TM CORE for Serving System change.*/
        cm_evt_payload_ptr =
        (tm_phone_event_payload_u_type *)pdsm_getbuf(sizeof(tm_phone_event_payload_u_type));
        if (cm_evt_payload_ptr != NULL)
        {
          cm_evt_payload_ptr->ss_info.sys_mode = pz_ds_event_info->srv_system;
          tm_core_cm_notify_event(TM_PHONE_EVENT_SS_SRV_STATUS, cm_evt_payload_ptr);
          (void)pdsm_freebuf((char *)cm_evt_payload_ptr);
        }
       break;
    default:
       MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM_DS_IFace: Event not handled : %d",
                                              pz_ds_event_info->ds_event_type);
       break;
  }
}

/*===========================================================================

FUNCTION tm_ds_iface_init

DESCRIPTION
   Function initializes callbacks with Data Services to listen to
   SYSTEM STATUS EX information from DS subsystem.
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void tm_ds_iface_init(void)
{
  /* Lookup emergency APN if configured and save it*/
  tm_ds_get_emergency_sys_info();

/* Register call backs with Data Services for listening to SYSTEM STATUS EX events*/
  tm_ds_reg_callbacks();

  return;
}
