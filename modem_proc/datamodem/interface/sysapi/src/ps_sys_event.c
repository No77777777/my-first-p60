/******************************************************************************
  @file    ps_sys_event.c
  @brief   

  DESCRIPTION
  This file defines functions, enums and structs use to register for
  events

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2011-16 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.
 
  $Header: //components/rel/data.mpss/3.5..2.1/interface/sysapi/src/ps_sys_event.c#3 $ $DateTime: 2024/05/06 04:25:26 $ $Author: pwbldsvc $
 
  
when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/31/11   bvd     Created module
===========================================================================*/

/*---------------------------------------------------------------------------
                           INCLUDE FILES
---------------------------------------------------------------------------*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "queue.h"
#include "ds_sysi.h"
#include "ps_sys_eventi.h"
#include "ps_sys_conf.h"
#include "ps_sys_fast_dormancy.h"
#include "ps_system_heap.h"
#include "ps_crit_sect.h"
#include "dserrno.h"
#include "ds_Utils_DebugMsg.h"
#include <stringl/stringl.h>
#include "event_defs.h"
#include "event.h"           /* For logging DIAG event and event codes      */
#include "ps_dpm.h"
#ifdef FEATURE_DS_QSH_INTERFACE
#include "ds3gsubsmgr.h"
#include "ds3g_qsh_if.h"
#endif /* FEATURE_DS_QSH_INTERFACE */
#include "ps_utils.h"


extern ps_crit_sect_type ps_sys_crit_section;

void *ps_sys_eventi_alloc_cback_buf
(
  ps_sys_tech_enum_type             tech_type,
  ps_sys_event_cback_f_ptr_type     event_cback_f_ptr,
  ps_sys_event_cback_f_ptr_ex_type  event_cback_f_ex_ptr,
  void                            * user_data_ptr,
  uint32                            subscription_id,
  ps_sys_event_enum_type            event_name
)
{
  ps_sys_eventi_buf_type * buf_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (event_cback_f_ptr == NULL && event_cback_f_ex_ptr == NULL)
  {
    LOG_MSG_INVALID_INPUT_0("ps_sys_eventi_alloc_cback_buf(): Callback param "
                            "is null");
    return NULL;
  }

  /*-------------------------------------------------------------------------
    Allocate a new event callback buffer.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(buf_ptr, sizeof(ps_sys_eventi_buf_type), 
                           ps_sys_eventi_buf_type*);
  if (buf_ptr == NULL)
  {
    return buf_ptr;
  }

  memset(buf_ptr,0,sizeof(ps_sys_eventi_buf_type));
  /*-------------------------------------------------------------------------
    Initialize the event buffer
  -------------------------------------------------------------------------*/
  (void) q_link(buf_ptr, &(buf_ptr->link));
  buf_ptr->tech_type            = tech_type;
  buf_ptr->event_cback_f_ptr    = event_cback_f_ptr;
  buf_ptr->event_cback_f_ex_ptr = event_cback_f_ex_ptr;
  /*-------------------------------------------------------------------------
    For PCO change and MSISDN change events, client passes user data that
    is used to comapare if events needs to generated or not. If user data 
    in form of apn name matches the event's apn name then event would be 
    generated.
  -------------------------------------------------------------------------*/
  if(event_name == PS_SYS_EVENT_APN_PCO_INFO_CHANGE ||
     event_name == PS_SYS_EVENT_APN_MSISDN_INFO_CHANGE)
  { 
    /*-------------------------------------------------------------------------
      NULL APN is valid APN, so query modehandler for valid APN and store
      it in buffer
    -------------------------------------------------------------------------*/
    if (!(NULL == user_data_ptr))
    {
      /*-------------------------------------------------------------------------
        Allocate a new event callback buffer for apn name
      -------------------------------------------------------------------------*/
      buf_ptr->user_data_ptr = 
        ps_system_heap_mem_alloc(PS_SYS_MAX_APN_LEN);
      if (buf_ptr->user_data_ptr == NULL)
      {
        return NULL;
      }
      memscpy((char *)(buf_ptr->user_data_ptr), PS_SYS_MAX_APN_LEN, 
              (char *)user_data_ptr, strlen((char *)user_data_ptr) + 1);
    }
  }
  else
  {
    buf_ptr->user_data_ptr        = user_data_ptr;
  }
  buf_ptr->event_q_ptr          = NULL;
  buf_ptr->subscription_id      = subscription_id;
  buf_ptr->event_name           = event_name;
  return buf_ptr;

} /* ps_sys_eventi_alloc_cback_buf() */

int16 ps_sys_event_reg
(
  ps_sys_tech_enum_type                  tech_type,
  ps_sys_event_enum_type                 event_name,
  ps_sys_event_cback_f_ptr_type          event_cback_f_ptr,
  void                                 * user_data_ptr,
  int16                                * ps_errno
)
{
  return ps_sys_eventi_reg (tech_type,
                            event_name,
                            PS_SYS_DEFAULT_SUBS,
                            event_cback_f_ptr,
                            NULL,
                            user_data_ptr,
                            ps_errno);
} /* ps_sys_event_reg() */

int16 ps_sys_event_reg_ex
(
  ps_sys_tech_enum_type                  tech_type,
  ps_sys_event_enum_type                 event_name,
  ps_sys_subscription_enum_type          subscription_id,
  ps_sys_event_cback_f_ptr_ex_type       event_cback_f_ptr,
  void                                 * user_data_ptr,
  int16                                * ps_errno
)
{
  return ps_sys_eventi_reg (tech_type,
                            event_name,
                            subscription_id,
                            NULL,
                            event_cback_f_ptr,
                            user_data_ptr,
                            ps_errno);
} /* ps_sys_event_reg_ex() */

int16 ps_sys_eventi_reg
(
  ps_sys_tech_enum_type                  tech_type,
  ps_sys_event_enum_type                 event_name,
  ps_sys_subscription_enum_type          subscription_id,
  ps_sys_event_cback_f_ptr_type          event_cback_f_ptr,
  ps_sys_event_cback_f_ptr_ex_type       event_cback_f_ex_ptr,
  void                                 * user_data_ptr,
  int16                                * ps_errno
)
{
  void                        * buf;
  ps_sys_eventi_buf_type      * buf_ptr;
  boolean                       event_cb_exist;
  int                           ret_val;
  ps_sys_system_status_type     system_status_conf_val;
  ps_sys_system_status_ex_type  *system_status_ex_val;
  int16                         sys_status_ps_errno = DS_ENOERR;
  ps_sys_modem_asst_ka_update_info_type  ka_update_info;
  ps_sys_data_services_roaming_setting_type romaing_setting_conf_val;
  ps_sys_imsi_public_key_info_type     imsi_public_key_info_val;
  ds_local_error_e_type                local_error = ERR_NO_ERROR;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_QTRACE_HFT_MED(PS_TAG_SYSAPI, subscription_id,
                 "ps_sys_eventi_reg(): tech %d event %d cb 0x%x subscription_id %d",
                 tech_type, event_name, event_cback_f_ptr, subscription_id);

  do
  {
    if (ps_errno == NULL)
    {
      local_error = ERR_INVALID_ERRNO;
      break;
    }

    if (TRUE == ds_sysi_is_modem_reset_in_progress())
    {
      *ps_errno = DS_EINVAL;
      local_error = ERR_MODEM_SSR_IN_PROGRESS;
      break;
    }

    ret_val = ps_sys_eventi_verify_tech_event(tech_type,event_name);
    if (ret_val == -1)
    {
      *ps_errno = DS_EFAULT;
      local_error = ERR_VERIFY_TECH_EVENT_FAIL;
      break;
    }

    if (subscription_id >= PS_SYS_SUBS_MAX || 
        subscription_id < PS_SYS_DEFAULT_SUBS)
    {
      *ps_errno = DS_EFAULT;
      local_error = ERR_INVALID_SUBS;
      break;
    }
  
    if (PS_SYS_DEFAULT_SUBS == subscription_id )
    {
      subscription_id = ps_sys_get_default_data_subscription();
    }

    if (event_cback_f_ptr == NULL && event_cback_f_ex_ptr == NULL)
    {
      *ps_errno = DS_EFAULT;
      local_error = ERR_NULL_CALLBACK;
      break;
    }

    if (event_cback_f_ptr != NULL && event_cback_f_ex_ptr != NULL)
    {
      *ps_errno = DS_EFAULT;
      local_error = ERR_ONLY_ONE_CB_ALLOWED;
      break;
    }
  }while(0);

  if(local_error != ERR_NO_ERROR)
  {
    LOG_MSG_ERROR_1("ps_sys_eventi_reg(): Validation error %d", local_error);
    return -1;
  }

  *ps_errno = 0; /*Setting it to success intially*/

  PS_ENTER_CRIT_SECTION(&ps_sys_crit_section);

  do
  {
    event_cb_exist = ps_sys_eventi_check_event_cback_exist
                     (
                       tech_type, 
                       event_name, 
                       event_cback_f_ptr,
                       event_cback_f_ex_ptr,
                       user_data_ptr,
                       subscription_id
                     );

    if (event_cb_exist) 
    {
      *ps_errno = DS_EINVAL;
      local_error = ERR_CB_EXIST_WITH_EVENT;
      break;
    }

    buf = ps_sys_eventi_alloc_cback_buf
          (
            tech_type, 
            event_cback_f_ptr,
            event_cback_f_ex_ptr,            
            user_data_ptr,
            subscription_id,
            event_name
          );

    if (NULL == buf)
    {
      *ps_errno = DS_NOMEMORY;
      local_error =  ERR_ALLOC_EVENT_BUF_FAIL;
      break;
    }

    buf_ptr = (ps_sys_eventi_buf_type*)buf;
  
     /*-------------------------------------------------------------------------
      make sure that this is not already queued - if not mark it as such
    -------------------------------------------------------------------------*/
    if (Q_ALREADY_QUEUED(&(buf_ptr->link)))
    {
      *ps_errno = DS_EINVAL;
      local_error = ERR_CB_ALREADY_QUEUED;
      break;
    }
  
    /*-------------------------------------------------------------------------
      As the contents of the buffer are supposed to be set by ps_iface
      functions it should NEVER have a NULL callback
    -------------------------------------------------------------------------*/
    if (buf_ptr->event_cback_f_ptr == NULL && 
         buf_ptr->event_cback_f_ex_ptr == NULL)
    {
      *ps_errno = DS_EFAULT;
      local_error = ERR_NULL_CALLBACK;
      break;
    }
  
    buf_ptr->event_q_ptr = &ps_sys_eventi_q;
    q_put(&ps_sys_eventi_q, &(buf_ptr->link));

    /*-------------------------------------------------------------------------
      For PS_SYS_EVENT_SYSTEM_STATUS_CHANGE we want to make sure that if system
      conf changes between user registers for event, we want to make sure that
      user doesnt miss the indication if system conf changed while user is
      registering for the event
    -------------------------------------------------------------------------*/

    if (event_name == PS_SYS_EVENT_SYSTEM_STATUS_CHANGE)
    {

      ret_val = ps_sys_conf_get_ex(tech_type, 
                                   PS_SYS_CONF_SYSTEM_STATUS,
                                   subscription_id,
                                   &system_status_conf_val,
                                   &sys_status_ps_errno);
      if (0 > ret_val)
      {
        local_error = ERR_GET_SYS_CONF_SYSTEM_STATUS_FAIL;
        break;
      }
      else
      {
        /* Sending event ind PS_SYS_EVENT_SYSTEM_STATUS_CHANGE */
		
        if(event_cback_f_ptr != NULL)
        {
          event_cback_f_ptr (tech_type,
                             event_name,
                             &system_status_conf_val,
                             user_data_ptr);
        }
      
        if(event_cback_f_ex_ptr != NULL)
        {
          event_cback_f_ex_ptr (tech_type,
                                event_name,
                                subscription_id,
                                &system_status_conf_val,
                                user_data_ptr);
        }
      }
    }
  
    /*-------------------------------------------------------------------------
      For PS_SYS_EVENT_SYSTEM_STATUS_EX we want to make sure that if 
      system conf changes between user registers for event, we want to make 
      sure that user doesnt miss the indication if system conf changed while 
      user is registering for the event
    -------------------------------------------------------------------------*/

    if (event_name == PS_SYS_EVENT_SYSTEM_STATUS_EX)
    {
      PS_SYSTEM_HEAP_MEM_ALLOC( system_status_ex_val, sizeof(ps_sys_system_status_ex_type),
                                ps_sys_system_status_ex_type*);

      if (NULL == system_status_ex_val)
      {
        *ps_errno = DS_NOMEMORY;
         local_error = ERR_SYS_STATUS_EX_MEM_ALLOC_FAIL;
         break;
      }

      memset(system_status_ex_val, 0, sizeof(ps_sys_system_status_ex_type));

      ret_val = ps_sys_conf_get_ex(tech_type, 
                                   PS_SYS_CONF_SYSTEM_STATUS_EX,
                                   subscription_id,
                                   system_status_ex_val,
                                   &sys_status_ps_errno);
      if (0 > ret_val)
      {
        local_error = ERR_GET_SYS_CONF_SYSTEM_STATUS_EX_FAIL;
      }
      else
      {
        /* Sending event ind PS_SYS_EVENT_SYSTEM_STATUS_EX */

        if(event_cback_f_ptr != NULL)
        {
          event_cback_f_ptr (tech_type,
                             event_name,
                             system_status_ex_val,
                             user_data_ptr);
        }
        
        if(event_cback_f_ex_ptr != NULL)
        {
          event_cback_f_ex_ptr (tech_type,
                                event_name,
                                subscription_id,
                                system_status_ex_val,
                                user_data_ptr);
        }
      }      
      if(system_status_ex_val != NULL)
      {
        PS_SYSTEM_HEAP_MEM_FREE(system_status_ex_val);
      }
    }
    /* Special Case: to inform DPM that a new client has been 
       registered for data activity resumed event*/
    if ( event_name == PS_SYS_EVENT_DATA_ACTIVITY_RESUMED )
    {
      ps_dpm_data_activity_client_status_ind(subscription_id,TRUE);
    }
  
    if( event_name == PS_SYS_EVENT_DATA_SERVICE_ROAMING_SETTING )
    {
      romaing_setting_conf_val = FALSE;
      ret_val = ps_sys_conf_get_ex(tech_type, 
                                   PS_SYS_CONF_DATA_SERVICE_ROAMING_SETTING,
                                   subscription_id,
                                   (void *)&romaing_setting_conf_val,
                                   &sys_status_ps_errno);
      if (0 > ret_val)
      {
        local_error = ERR_GET_SYS_CONF_DATA_SERVICE_ROAMING_SETTING_FAIL;
        break;
      }
      else
      {
        /* ps_sys_eventi_reg(): Sending event ind */

        if(event_cback_f_ptr != NULL)
        {
          event_cback_f_ptr (tech_type,
                             event_name,
                             &romaing_setting_conf_val,
                             user_data_ptr);
        }
        
        if(event_cback_f_ex_ptr != NULL)
        {
          event_cback_f_ex_ptr (tech_type,
                                event_name,
                                subscription_id,
                                &romaing_setting_conf_val,
                                user_data_ptr);
        }
      }      
    }

    if (event_name == PS_SYS_EVENT_MODEM_ASST_KEEP_ALIVE_UPDATE)
    {
      ret_val = ps_sys_conf_get_ex(tech_type, 
                                   PS_SYS_CONF_MODEM_ASST_KEEP_ALIVE_CONFIG,
                                   subscription_id,
                                   &ka_update_info,
                                   &sys_status_ps_errno);

      if (0 > ret_val)
      {
        local_error = ERR_GET_SYS_CONF_MODEM_ASST_KEEP_ALIVE_CONFIG_FAIL;
        break;
      }
      else
      {
        /* Sending event ind PS_SYS_EVENT_MODEM_ASST_KEEP_ALIVE_UPDATE */
		
        if(event_cback_f_ptr != NULL)
        {
          event_cback_f_ptr (tech_type,
                             event_name,
                             &ka_update_info,
                             user_data_ptr);
        }
  
        if(event_cback_f_ex_ptr != NULL)
        {
          event_cback_f_ex_ptr (tech_type,
                                event_name,
                                subscription_id,
                                &ka_update_info,
                                user_data_ptr);
        }
      }
    }

    if (event_name == PS_SYS_EVENT_UPDATED_IMSI_PUBLIC_KEY_INFO)
    {

      ret_val = ps_sys_conf_get_ex(tech_type, 
                                   PS_SYS_CONF_IMSI_PUBLIC_KEY_INFO,
                                   subscription_id,
                                   &imsi_public_key_info_val,
                                   &sys_status_ps_errno);
      if (0 > ret_val)
      {
        local_error = ERR_GET_SYS_CONF_IMSI_PUBLIC_KEY_INFO_FAIL;
        break;
      }
      else
      {
        /* Sending event ind PS_SYS_EVENT_UPDATED_IMSI_PUBLIC_KEY_INFO */
  
        if(event_cback_f_ptr != NULL)
        {
          event_cback_f_ptr (tech_type,
                             event_name,
                             &imsi_public_key_info_val,
                             user_data_ptr);
        }
      
        if(event_cback_f_ex_ptr != NULL)
        {
          event_cback_f_ex_ptr (tech_type,
                                event_name,
                                subscription_id,
                                &imsi_public_key_info_val,
                                user_data_ptr);
        }
      }
    }


  } while (0);
  
  PS_LEAVE_CRIT_SECTION(&ps_sys_crit_section);

  if(local_error != ERR_NO_ERROR) 
  {
    LOG_MSG_ERROR_3("ps_sys_eventi_reg(): Error %d occurred with ps_errno %d sys_status ps_errno %d",
                     local_error, *ps_errno, sys_status_ps_errno);
  }

  if(*ps_errno != 0)
  {
    return -1;  
  }
  else 
  {
    return 0;
  }
} /* ps_sys_eventi_reg() */

boolean ps_sys_eventi_check_event_cback_exist
(
  ps_sys_tech_enum_type                 tech_type,
  ps_sys_event_enum_type                event,
  ps_sys_event_cback_f_ptr_type         event_cback_f_ptr,
  ps_sys_event_cback_f_ptr_ex_type      event_cback_f_ex_ptr,
  void                                 * user_data_ptr,
  ps_sys_subscription_enum_type         subscription_id
)
{
  q_type                       * event_q_ptr = NULL;
  void                         * event_buf_ptr;
  void                         * next_event_buf_ptr;
  ps_sys_eventi_buf_type       * ps_sys_event_buf_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  event_q_ptr = &ps_sys_eventi_q;
  
  /*-------------------------------------------------------------------------
    Traverse the queue of callbacks for this event
  -------------------------------------------------------------------------*/
  event_buf_ptr = q_check(event_q_ptr);
  while (event_buf_ptr != NULL)
  {
    ps_sys_event_buf_ptr = (ps_sys_eventi_buf_type *) event_buf_ptr;

    /*-----------------------------------------------------------------------
      if event callback is present in queue for event and tech
      return true
    -----------------------------------------------------------------------*/
    if (ps_sys_event_buf_ptr->tech_type == tech_type && 
        ps_sys_event_buf_ptr->event_name == event &&
        ps_sys_event_buf_ptr->subscription_id == subscription_id &&
        ((event_cback_f_ptr != NULL && 
          ps_sys_event_buf_ptr->event_cback_f_ptr == event_cback_f_ptr) ||
        (event_cback_f_ex_ptr != NULL && 
          ps_sys_event_buf_ptr->event_cback_f_ex_ptr == event_cback_f_ex_ptr)))
    {
      if(event == PS_SYS_EVENT_APN_PCO_INFO_CHANGE ||
         event == PS_SYS_EVENT_APN_MSISDN_INFO_CHANGE)
      { 
        /*-------------------------------------------------------------------------
          NULL APN is valid APN, so query modehandler for valid APN and store
          it in buffer
        -------------------------------------------------------------------------*/
        if ( !((ps_sys_event_buf_ptr->user_data_ptr == user_data_ptr) ||
               (0 == (strcmp((char *)(ps_sys_event_buf_ptr->user_data_ptr),
                      (char *)user_data_ptr)))))
        {
          return FALSE;
        }
      }
      return TRUE;
    }
    next_event_buf_ptr =
    q_next(event_q_ptr, &((q_generic_item_type *) event_buf_ptr)->link);

    event_buf_ptr = next_event_buf_ptr;
  } /* while (items in queue) */

  return FALSE;
} /* ps_sys_eventi_check_event_cback_exist() */


/** 
  @brief  This function calls all callbacks for given event.

  @param[in] tech_type            States Technology type
  @param[in] event                Name of event
  @param[in] event_info           Event information 
  @param[in] subscription_id      Subscription Id
  
  */
void ps_sys_eventi_invoke_cbacks
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event,
  void                                         * event_info,
  ps_sys_subscription_enum_type                  subscription_id
)
{
  q_type                                     * event_q_ptr = NULL;
  void                                       * event_buf_ptr;
  void                                       * next_event_buf_ptr;
  ps_sys_eventi_buf_type                     * ps_sys_event_buf_ptr;
  int16                                        dss_errno;
  ps_sys_system_status_type                  * system_status_ptr;
  ps_sys_system_status_ex_type               * system_status_ex_ptr;
  ps_sys_system_status_diag_event_logging_type system_status_log;
  ps_sys_pco_info_per_apn_type               * apn_pco_info_ptr;
  ps_sys_msisdn_info_per_apn_type            * apn_msisdn_info_ptr;
  boolean                                      invoke_cb = TRUE;
  ps_sys_ioctl_get_resolved_apn_type           apn_info;
  int16                                        ps_errno;
  int16                                        ret_val = 0;
  ps_sys_ioctl_get_resolved_apn_type           null_apn_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&system_status_log, 0, sizeof(system_status_log));

  if( event != PS_SYS_EVENT_LOW_LATENCY_TRAFFIC_STATUS )
  {
    LOG_MSG_FUNCTION_ENTRY_2("ps_sys_eventi_invoke_cbacks(): event %d tech %d ",
                             event, tech_type );
  }
  /*---------------------------------------------------------------------------
   Make sure the callback invocation is protected under critical section to 
   avoid invalid memory accesses
  ---------------------------------------------------------------------------*/
  #ifdef FEATURE_DS_QSH_INTERFACE
  ds3g_qsh_if_report_event(ds3gsubsmgr_subs_id_ds_to_cm((ds_sys_subscription_enum_type)subscription_id),DS_QSH_EVENT_PS_SYS_INVOKE_CBACK);
  #endif /* FEATURE_DS_QSH_INTERFACE */
  
  PS_ENTER_CRIT_SECTION(&ps_sys_crit_section);
  event_q_ptr = &ps_sys_eventi_q;
 
  /*-------------------------------------------------------------------------
    Traverse the queue of callbacks for this event
  -------------------------------------------------------------------------*/
  event_buf_ptr = q_check(event_q_ptr);
  
  while (event_buf_ptr != NULL)
  {
    next_event_buf_ptr =
    q_next(event_q_ptr, &((q_generic_item_type *) event_buf_ptr)->link);

    ps_sys_event_buf_ptr = (ps_sys_eventi_buf_type *) event_buf_ptr;

    invoke_cb = TRUE;

    if ((ps_sys_event_buf_ptr->tech_type == tech_type || 
       ps_sys_event_buf_ptr->tech_type == PS_SYS_TECH_ALL) && 
       ps_sys_event_buf_ptr->subscription_id == subscription_id &&
       ps_sys_event_buf_ptr->event_name == event)
    {
    
      if(event == PS_SYS_EVENT_APN_PCO_INFO_CHANGE ||
         event == PS_SYS_EVENT_APN_MSISDN_INFO_CHANGE)
      { 
        memset(&apn_info, 0, sizeof(ps_sys_ioctl_get_resolved_apn_type));

        if(event == PS_SYS_EVENT_APN_PCO_INFO_CHANGE)
        {
          // Retrieve resolved apn from another apn 
          apn_pco_info_ptr = 
            (ps_sys_pco_info_per_apn_type *)event_info;

          /* Copy APN name to retrieve resolved apn name*/     
          memscpy(&(apn_info.apn_name),
                  strlen(apn_pco_info_ptr->apn_name) + 1,
                  apn_pco_info_ptr->apn_name,
                  strlen(apn_pco_info_ptr->apn_name) + 1);
        }
        else
        {
          apn_msisdn_info_ptr = 
            (ps_sys_msisdn_info_per_apn_type *)event_info;  

          /* Copy APN name to retrieve resolved apn name*/     
          memscpy(&(apn_info.apn_name),
                  strlen(apn_msisdn_info_ptr->apn_name) + 1,
                  apn_msisdn_info_ptr->apn_name,
                  strlen(apn_msisdn_info_ptr->apn_name) + 1);      
        }
        
        /* MH will return same APN name if there is no resolved APN*/       
        ret_val = ps_sys_ioctl_ex(PS_SYS_TECH_ALL, 
                                  PS_SYS_IOCTL_GET_RESOLVED_APN, 
                                  subscription_id, 
                                  &apn_info, 
                                  &ps_errno);
        
       /* Compare apn name stored in event buffer with apn name and its resolved
          apn name. If either one of them matches, then generate the event
        */
        if (-1 != ret_val)
        {
          /*-------------------------------------------------------------------------
            NULL APN is valid APN, so query modehandler for valid APN and 
            compare with apn of event
          -------------------------------------------------------------------------*/
          if (NULL == ps_sys_event_buf_ptr->user_data_ptr || 
             0 == strlen((char *)(ps_sys_event_buf_ptr->user_data_ptr)))
          {        
            memset(&null_apn_info, 0, sizeof(ps_sys_ioctl_get_resolved_apn_type));

            /* Copy APN name to retrieve resolved apn name*/     
            memscpy(&(null_apn_info.apn_name), 0, "",0);
 
            ret_val = ps_sys_ioctl_ex(PS_SYS_TECH_ALL, 
                                      PS_SYS_IOCTL_GET_RESOLVED_APN, 
                                      subscription_id, 
                                      &null_apn_info, 
                                      &ps_errno);

            if (0 != strcmp(null_apn_info.resolved_apn_name, 
                             apn_info.resolved_apn_name))
            {                        
              invoke_cb = FALSE;
            }
          }
          else if(event == PS_SYS_EVENT_APN_MSISDN_INFO_CHANGE)
          {
            if ((0 != strcmp((char *)ps_sys_event_buf_ptr->user_data_ptr, 
                             apn_msisdn_info_ptr->apn_name)) &&
                (0 != strcmp((char *)ps_sys_event_buf_ptr->user_data_ptr, 
                              apn_info.resolved_apn_name)))
            {                        
              invoke_cb = FALSE;
            }
          }
          else
          {
            if ((0 != strcmp((char *)ps_sys_event_buf_ptr->user_data_ptr, 
                             apn_pco_info_ptr->apn_name)) &&
                (0 != strcmp((char *)ps_sys_event_buf_ptr->user_data_ptr, 
                              apn_info.resolved_apn_name)))
            {                        
              invoke_cb = FALSE;
            }
          }
        }
      }       
      
      /* 
         Only one of the two Callbacks would be called. Either the
         the callback event_cback_f_ex_ptr or event_cback_f_ptr
      */
      if(NULL != ps_sys_event_buf_ptr->event_cback_f_ex_ptr && 
         invoke_cb == TRUE)    
      {
        ps_sys_event_buf_ptr->event_cback_f_ex_ptr
        (
          tech_type,
          event,
          subscription_id,
          event_info,
          ps_sys_event_buf_ptr->user_data_ptr
        );
      }

      if(NULL != ps_sys_event_buf_ptr->event_cback_f_ptr && 
         invoke_cb == TRUE)    
      {
        ps_sys_event_buf_ptr->event_cback_f_ptr
        (
          tech_type,
          event,
          event_info,
          ps_sys_event_buf_ptr->user_data_ptr
        );
      }      

      if(event == PS_SYS_EVENT_SYSTEM_STATUS_CHANGE)
      {
        /* Diag event for system status */
        system_status_ptr              = (ps_sys_system_status_type *) event_info;
        system_status_log.pref_network = system_status_ptr->preferred_network;
        system_status_log.rat_value    = system_status_ptr->network_info \
                                         [system_status_ptr->preferred_network].rat_mask;
        system_status_log.so_mask      = system_status_ptr->network_info \
                                         [system_status_ptr->preferred_network].so_mask;
        system_status_log.subs_id      = subscription_id;
        event_report_payload( EVENT_PS_SYSTEM_STATUS, 
                              sizeof(ps_sys_system_status_diag_event_logging_type),
                              (void*) &system_status_log);
      }
      else if(event == PS_SYS_EVENT_SYSTEM_STATUS_EX)
      {
        /* Diag event for system status ex */
        system_status_ex_ptr           = (ps_sys_system_status_ex_type *) event_info;
        system_status_log.pref_network = system_status_ex_ptr->avail_sys[0].technology;
        system_status_log.rat_value    = system_status_ex_ptr->avail_sys[0].rat_value;
        system_status_log.so_mask      = system_status_ex_ptr->avail_sys[0].so_mask;
        system_status_log.subs_id      = subscription_id;
        event_report_payload( EVENT_PS_SYSTEM_STATUS_EX, 
                              sizeof(ps_sys_system_status_diag_event_logging_type),
                              (void*) &system_status_log);
      } 

      /*
        Since we are implicitly registering for
        PS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_STATUS, when application 
        sets the configuration value for page monitor period status. We 
        have to dereg when event is recieved
      */
      if (event == PS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_STATUS)
      {
        if(ps_sys_event_buf_ptr->event_cback_f_ptr)
        {
           ps_sys_event_dereg
           (
             tech_type,
             event,
             ps_sys_event_buf_ptr->event_cback_f_ptr,                            
             &dss_errno
           );
        }

        if(ps_sys_event_buf_ptr->event_cback_f_ex_ptr)
        {
           ps_sys_event_dereg_ex
           (
             tech_type,
             event,
             subscription_id,
             ps_sys_event_buf_ptr->event_cback_f_ex_ptr,                            
             &dss_errno
           );
        }        
      }
    }
    event_buf_ptr = next_event_buf_ptr;
  } /* while (items in queue) */

  PS_LEAVE_CRIT_SECTION(&ps_sys_crit_section);
} /* ps_sys_eventi_invoke_cbacks() */


int16 ps_sys_event_ind
(
  ps_sys_tech_enum_type             tech_type,
  ps_sys_event_enum_type            event_name,
  void                            * event_info,
  int16                           * ps_errno
)
{
  return ps_sys_event_ind_ex (tech_type,
                              event_name,
                              PS_SYS_DEFAULT_SUBS,
                              event_info,
                              ps_errno);
}/* ps_sys_event_ind() */

int16 ps_sys_event_ind_ex
(
  ps_sys_tech_enum_type             tech_type,
  ps_sys_event_enum_type            event_name,
  ps_sys_subscription_enum_type     subscription_id,
  void                            * event_info,
  int16                           * ps_errno
)
{
  ps_sys_eventi_cmd_type                 * cmd_data_info;
  void                                   * cmd_event_info;
  boolean                                  invoke_cb = FALSE;
  ps_sys_eventi_info_type                * sys_event_info_ptr;
  int                                      ret_val;
  ps_sys_system_status_type              * sys_status_ptr;
  ps_sys_system_status_ex_type           * system_status_ex_ptr;
  uint32                                   cnt_pdn;
  uint32                                   cnt_network;
  ps_sys_apn_pref_sys_type               * apn_pref_sys_ptr;
  ps_sys_wlan_mac_addr_type              * wlan_mac_addr_ptr;
  ps_sys_wlan_ip_addr_type               * wlan_ip_addr_ptr;
  ps_sys_wlan_ssid_type                  * wlan_ssid_ptr;
  ps_sys_wlan_status_type                * wlan_status_ptr;
  ps_sys_wlan_preference_enum_type       * wlan_preference_ptr;
#if defined(FEATURE_DATA_LTE)
  ps_sys_3gpp_lte_attach_pdn_list_type   * attach_pdn_info_ptr;
  int                                      pdn_cnt;
#endif /* defined(FEATURE_DATA_LTE) */
  ps_sys_lte_attach_enum_type            * lte_attach_type_ptr;
  ps_sys_wlan_max_conn_info_type         * wlan_max_conn_info_ptr = NULL;
  ps_sys_low_latency_traffic_info_type   * low_latency_traffic_info = NULL;
  ps_sys_roaming_status_info_type        * roaming_status_ptr = NULL;
  ps_sys_policy_refresh_status_type      * policy_refresh_status_ptr;
  ps_sys_policy_available_info_type      * policy_available_info_ptr;
  ps_sys_wlan_wifi_meas_start_type       * start_wifi_meas_ptr = NULL;
  ps_sys_wlan_wifi_meas_stop_type        * stop_wifi_meas_ptr  = NULL;
  ps_sys_wqe_profile_type                * wqe_profile_mask = NULL;
  ps_sys_event_nat_keep_alive_info_type  * nat_keep_alive_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( event_name != PS_SYS_EVENT_LOW_LATENCY_TRAFFIC_STATUS )
  { 
    LOG_MSG_INFO1_4("ps_sys_event_ind_ex() : event %d tech %d "
                    "event_info 0x%p subscription_id %d",
                    event_name, tech_type, event_info, subscription_id);
  }

  if (ps_errno == NULL)
  {
    LOG_MSG_INVALID_INPUT_0("ps_sys_event_ind_ex(): Invalid argument ps_errno ");
    return -1;
  }

  ret_val = ps_sys_eventi_verify_tech_event(tech_type,event_name);
  if (event_info == NULL || ret_val == -1)
  {
    *ps_errno = DS_EFAULT;
    LOG_MSG_ERROR_0("ps_sys_event_ind_ex(): Invalid argument passed");
    return -1;
  }

  if (subscription_id >= PS_SYS_SUBS_MAX || 
      subscription_id < PS_SYS_DEFAULT_SUBS)
  {
    *ps_errno = DS_EFAULT;
    LOG_MSG_ERROR_1("ps_sys_event_ind_ex(): Invalid subscription_id %d",
                    subscription_id);
    return -1;
  }

  if (PS_SYS_DEFAULT_SUBS == subscription_id )
  {
    subscription_id = ps_sys_get_default_data_subscription();
  }

  PS_SYSTEM_HEAP_MEM_ALLOC(sys_event_info_ptr, sizeof(ps_sys_eventi_info_type),
                           ps_sys_eventi_info_type*);


  if (NULL == sys_event_info_ptr)
  {
    *ps_errno = DS_NOMEMORY;
    return -1;
  }

  cmd_event_info = (void *)sys_event_info_ptr;

  /*-------------------------------------------------------------------------
    Process the event
  -------------------------------------------------------------------------*/
  switch(event_name)
  {
    case PS_SYS_EVENT_SYSTEM_STATUS_CHANGE: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_system_status_type),
             (ps_sys_system_status_type *) event_info,
             sizeof(ps_sys_system_status_type));
      
      sys_status_ptr = (ps_sys_system_status_type *) sys_event_info_ptr;
             
      LOG_MSG_INFO2_3("ps_sys_event_ind_ex : pref network %d, RAT mask 0x%x"
                      "so mask 0x%x",
                      sys_status_ptr->preferred_network,
                      sys_status_ptr->network_info[sys_status_ptr->preferred_network].rat_mask,
                      sys_status_ptr->network_info[sys_status_ptr->preferred_network].so_mask);
      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_SYSTEM_STATUS_EX: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_system_status_ex_type),
             (ps_sys_system_status_ex_type *) event_info,
             sizeof(ps_sys_system_status_ex_type));

      system_status_ex_ptr = (ps_sys_system_status_ex_type *) sys_event_info_ptr;
      
      LOG_MSG_INFO1_2("ps_sys_event_ind_ex :num of rats %d num of pdns %d",
                      system_status_ex_ptr->num_avail_sys,
                      system_status_ex_ptr->num_avail_apns);

      LOG_MSG_INFO1_0("ps_sys_event_ind_ex : First tupple of technology, RAT"
                      " and SO is preferred system"); 
                    
      for (cnt_network=0; cnt_network < system_status_ex_ptr->num_avail_sys; 
           cnt_network++)
      {
        LOG_MSG_INFO1_3("ps_sys_event_ind_ex: technology %d rat value %d so mask 0x%x",
                        system_status_ex_ptr->avail_sys[cnt_network].technology,
                        system_status_ex_ptr->avail_sys[cnt_network].rat_value,
                        system_status_ex_ptr->avail_sys[cnt_network].so_mask);
      }

      for (cnt_pdn=0; cnt_pdn < system_status_ex_ptr->num_avail_apns; 
           cnt_pdn++)
      {
        LOG_MSG_INFO1_2("ps_sys_conf_set pdn name %s available sys for pdn %d",
                        system_status_ex_ptr->apn_pref_sys_info[cnt_pdn].apn_name,
                        system_status_ex_ptr->apn_pref_sys_info[cnt_pdn].num_avail_sys);

        if(system_status_ex_ptr->apn_pref_sys_info[cnt_pdn].num_avail_sys > 0)
        {
          LOG_MSG_INFO1_3("ps_sys_conf_set pdn name %s pref tech %d pref rat %d pref so mask 0x%x",
                          system_status_ex_ptr->apn_pref_sys_info[cnt_pdn].avail_sys[0].technology,
                          system_status_ex_ptr->apn_pref_sys_info[cnt_pdn].avail_sys[0].rat_value,
                          system_status_ex_ptr->apn_pref_sys_info[cnt_pdn].avail_sys[0].so_mask);
        }                   
      }
      
      invoke_cb = TRUE;      
      break;
    }

    case PS_SYS_EVENT_APN_PREF_SYS_CHANGE: 
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_apn_pref_sys_type),
             (ps_sys_apn_pref_sys_type *) event_info,
             sizeof(ps_sys_apn_pref_sys_type));
             
      apn_pref_sys_ptr = (ps_sys_apn_pref_sys_type *) sys_event_info_ptr;
             
      LOG_MSG_INFO2_2("ps_sys_event_ind_ex : PS_SYS_EVENT_APN_PREF_SYS_CHANGE"
                      "apn name %d, tech pref %d",
                      apn_pref_sys_ptr->apn, apn_pref_sys_ptr->pref_sys);
                    
      invoke_cb = TRUE;
      break;
    }    

    case PS_SYS_EVENT_3GPP_LTE_ATTACH_INIT: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_3gpp_lte_attach_init_type),
             (ps_sys_event_3gpp_lte_attach_init_type *) event_info,
             sizeof(ps_sys_event_3gpp_lte_attach_init_type));
      invoke_cb = TRUE;      
      break;
    }

    case PS_SYS_EVENT_3GPP_LTE_ATTACH_COMPLETE: 
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_3gpp_lte_attach_complete_type),
             (ps_sys_event_3gpp_lte_attach_complete_type *) event_info,
             sizeof(ps_sys_event_3gpp_lte_attach_complete_type));
      invoke_cb = TRUE;      
      break;
    }

    case PS_SYS_EVENT_PDN_THROTTLE_INFO: 
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_pdn_throttle_info_type),
             (ps_sys_pdn_throttle_info_type *) event_info,
             sizeof(ps_sys_pdn_throttle_info_type));
      invoke_cb = TRUE;      
      break;
    }

    case PS_SYS_EVENT_PDN_STATUS_INFO: 
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_pdn_status_info_type),
             (ps_sys_pdn_status_info_type *) event_info,
              sizeof(ps_sys_pdn_status_info_type));
      invoke_cb = TRUE;      
      break;
    }

    case PS_SYS_EVENT_3GPP2_EHRPD_PREREG_HANDOFF_COMPLETE_RSP: 
    {
      memscpy
      (
        sys_event_info_ptr,
        sizeof(ps_sys_event_3gpp2_ehrpd_prereg_handoff_complete_rsp_type),
        (ps_sys_event_3gpp2_ehrpd_prereg_handoff_complete_rsp_type *) \
          event_info,
        sizeof(ps_sys_event_3gpp2_ehrpd_prereg_handoff_complete_rsp_type)
      );
      invoke_cb = TRUE;      
      break; 
    }

    case PS_SYS_EVENT_3GPP2_S101_STATUS_CHANGE:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_3gpp2_s101_status_change_type),
             (ps_sys_3gpp2_s101_status_change_type *) event_info,
             sizeof(ps_sys_3gpp2_s101_status_change_type));
      invoke_cb = TRUE;      
      break;
    }

    case PS_SYS_EVENT_3GPP2_EPZID_INFO: 
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_3gpp2_epzid_info_type),
             (ps_sys_3gpp2_epzid_info_type *)event_info,
             sizeof(ps_sys_3gpp2_epzid_info_type));
      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_STATUS: 
    {
      memscpy
      (
        sys_event_info_ptr,
        sizeof(ps_sys_event_3gpp2_evdo_page_monitor_period_status_info_type),
        (ps_sys_event_3gpp2_evdo_page_monitor_period_status_info_type *) \
          event_info,
         sizeof(ps_sys_event_3gpp2_evdo_page_monitor_period_status_info_type)
      );         
      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_CHANGE: 
    {       
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_3gpp2_evdo_page_monitor_period_type),
             (ps_sys_3gpp2_evdo_page_monitor_period_type *) event_info,
             sizeof(ps_sys_3gpp2_evdo_page_monitor_period_type));         
      invoke_cb = TRUE;
      break;             
    }

    case PS_SYS_EVENT_DUN_CTRL_CALL_STATUS: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_dun_ctrl_call_status_info_type),
             (ps_sys_event_dun_ctrl_call_status_info_type *) event_info,
             sizeof(ps_sys_event_dun_ctrl_call_status_info_type));         
      invoke_cb = TRUE;
      break;              
    }

    case PS_SYS_EVENT_DUN_CTRL_ENTITLEMENT_CHECK: 
    { 
      memscpy
      (
        sys_event_info_ptr,
        sizeof(ps_sys_event_dun_ctrl_entitlement_check_info_type),
        (ps_sys_event_dun_ctrl_entitlement_check_info_type *) event_info,
        sizeof(ps_sys_event_dun_ctrl_entitlement_check_info_type)
      );         
      invoke_cb = TRUE;
      break;          
    }

    case PS_SYS_EVENT_DUN_CTRL_SILENT_REDIAL_CHECK: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_dun_ctrl_silent_redial_check_info_type),
             (ps_sys_event_dun_ctrl_silent_redial_check_info_type *) \
               event_info,
             sizeof(ps_sys_event_dun_ctrl_silent_redial_check_info_type));         
      invoke_cb = TRUE;
      break;      
    }
    
    case PS_SYS_EVENT_DATA_CALL_STATUS_CHANGE: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_data_call_status_change_type),
             (ps_sys_event_data_call_status_change_type *) event_info,
             sizeof(ps_sys_event_data_call_status_change_type));
      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_LOW_LATENCY_TRAFFIC_STATUS: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_low_latency_traffic_info_type),
             (ps_sys_low_latency_traffic_info_type *) event_info,
             sizeof(ps_sys_low_latency_traffic_info_type));

      low_latency_traffic_info = 
        (ps_sys_low_latency_traffic_info_type *)sys_event_info_ptr;            

      LOG_MSG_DATA_PATH_INFO_2( "ps_sys_event_ind_ex: traffic_state %d,"
                                "fltr_hdl %d",
                                low_latency_traffic_info->traffic_state,
                                low_latency_traffic_info->filter_handle);
      
      invoke_cb = TRUE;
      break;
    }
    
    case PS_SYS_EVENT_WLAN_STATUS_CHANGE: 
    {     
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_wlan_status_type),
             (ps_sys_wlan_status_type *) event_info,
             sizeof(ps_sys_wlan_status_type));

      wlan_status_ptr = 
        (ps_sys_wlan_status_type *)sys_event_info_ptr;            

      LOG_MSG_INFO2_3("ps_sys_event_ind_ex : is_wlan_available %d, "
                      "ipv6_prefix_len %d mac address %d",
                      wlan_status_ptr->is_wlan_available,
                      wlan_status_ptr->ipv6_prefix_len,
                      wlan_status_ptr->ap_mac_addr);

      LOG_MSG_INFO2_1("ps_sys_event_ind_ex : wqe_status %d ",
                      wlan_status_ptr->wqe_status);
  
      IPV4_ADDR_MSG(wlan_status_ptr->ipv4_addr.addr.v4.ps_s_addr);  
        
      IPV6_ADDR_MSG(wlan_status_ptr->ipv4_addr.addr.v6.ps_s6_addr64);             
                     
      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_WLAN_PREFERENCE_CHANGE: 
    {     
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_wlan_preference_enum_type),
             (ps_sys_wlan_preference_enum_type *) event_info,
             sizeof(ps_sys_wlan_preference_enum_type));

      wlan_preference_ptr = 
        (ps_sys_wlan_preference_enum_type *)sys_event_info_ptr;             
             
      LOG_MSG_INFO2_1("ps_sys_event_ind_ex : wlan mac addr %d ",
                      *wlan_preference_ptr);
                     
      invoke_cb = TRUE;
      break;
    }    
    
    case PS_SYS_EVENT_WLAN_MAC_ADDR_CHANGE: 
    {     
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_wlan_mac_addr_type),
             (ps_sys_wlan_mac_addr_type *) event_info,
             sizeof(ps_sys_wlan_mac_addr_type));

      wlan_mac_addr_ptr = 
        (ps_sys_wlan_mac_addr_type *)sys_event_info_ptr;             
             
      LOG_MSG_INFO2_1("ps_sys_event_ind_ex : wlan mac addr %d ",
                      wlan_mac_addr_ptr->ap_mac_addr);
                     
      invoke_cb = TRUE;
      break;
    }    

    case PS_SYS_EVENT_WLAN_IP_ADDR_CHANGE: 
    {     
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_wlan_ip_addr_type),
             (ps_sys_wlan_ip_addr_type *) event_info,
             sizeof(ps_sys_wlan_ip_addr_type));

      wlan_ip_addr_ptr = 
        (ps_sys_wlan_ip_addr_type *)sys_event_info_ptr;             
             
                      
      IPV4_ADDR_MSG(wlan_ip_addr_ptr->ipv4_addr.addr.v4.ps_s_addr);  
      
      LOG_MSG_INFO2_1("ps_sys_event_ind_ex : ipv6_prefix_len %d",
                      wlan_ip_addr_ptr->ipv6_prefix_len);      
        
      IPV6_ADDR_MSG(wlan_ip_addr_ptr->ipv6_addr.addr.v6.ps_s6_addr64);                         
                     
      invoke_cb = TRUE;
      break;
    }    

    case PS_SYS_EVENT_WLAN_SSID_CHANGE: 
    {     
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_wlan_ssid_type),
              (ps_sys_wlan_ssid_type *) event_info,
              sizeof(ps_sys_wlan_ssid_type));

      wlan_ssid_ptr = (ps_sys_wlan_ssid_type *)sys_event_info_ptr;             

      LOG_MSG_INFO2_1("ps_sys_event_ind_ex : wlan ssid %s ",
                      wlan_ssid_ptr->ssid);
      invoke_cb = TRUE;
      break;
    }    

    case PS_SYS_EVENT_WLAN_MAX_CONN: 
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_wlan_max_conn_info_type),
              (ps_sys_wlan_max_conn_info_type *) event_info,
              sizeof(ps_sys_wlan_max_conn_info_type));

      wlan_max_conn_info_ptr = (ps_sys_wlan_max_conn_info_type *)sys_event_info_ptr;
      LOG_MSG_INFO1_2("ps_sys_event_ind_ex : wlan max conn reached %d "
                      "no of active apns %d",
                      wlan_max_conn_info_ptr->is_max_conn_reached, 
                      wlan_max_conn_info_ptr->num_apns);
      invoke_cb = TRUE;      
      break;
    }
  
#if defined(FEATURE_DATA_LTE)
    case PS_SYS_EVENT_3GPP_LTE_ATTACH_PDN_LIST_CHANGE: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_3gpp_lte_attach_pdn_list_type),
             ( ps_sys_3gpp_lte_attach_pdn_list_type *) event_info,
             sizeof( ps_sys_3gpp_lte_attach_pdn_list_type));

      attach_pdn_info_ptr = 
        (ps_sys_3gpp_lte_attach_pdn_list_type *) sys_event_info_ptr;
      
      LOG_MSG_INFO1_1("ps_sys_event_ind_ex :num of lte attach pdns %d",
                      attach_pdn_info_ptr->attach_pdn_profile_list_num);

      for (pdn_cnt=0; pdn_cnt < attach_pdn_info_ptr->attach_pdn_profile_list_num; pdn_cnt++)
      {
        LOG_MSG_INFO1_2("ps_sys_event_ind_ex: index %d profile_id %d",
                        pdn_cnt,
                        attach_pdn_info_ptr->attach_pdn_profile_list[pdn_cnt]);
      }
    
      invoke_cb = TRUE;      
      break;
    }
#endif /* defined(FEATURE_DATA_LTE) */
    case PS_SYS_EVENT_3GPP_LTE_ATTACH_TYPE_CHANGE: 
    {     
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_lte_attach_enum_type),
             ( ps_sys_lte_attach_enum_type *) event_info,
             sizeof( ps_sys_lte_attach_enum_type));

      lte_attach_type_ptr = 
        (ps_sys_lte_attach_enum_type *)sys_event_info_ptr;             
             
      LOG_MSG_INFO2_1("ps_sys_event_ind_ex : lte attach type %d ",
                      *lte_attach_type_ptr);
                     
      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_ROAMING_STATUS_CHANGE:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_roaming_status_info_type),
              (ps_sys_roaming_status_info_type *) event_info,
              sizeof(ps_sys_roaming_status_info_type));

      roaming_status_ptr = (ps_sys_roaming_status_info_type*)sys_event_info_ptr;

      LOG_MSG_INFO2_1("ps_sys_event_ind_ex : Roaming status %d",
                       roaming_status_ptr->roaming_status);

      invoke_cb = TRUE;
      break;
    }
    case PS_SYS_EVENT_3GPP_POLICY_REFRESH_STATUS:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_policy_refresh_status_type),
             ( ps_sys_policy_refresh_status_type *) event_info,
             sizeof( ps_sys_policy_refresh_status_type));

      policy_refresh_status_ptr = 
         ( ps_sys_policy_refresh_status_type *) sys_event_info_ptr;
     
      LOG_MSG_INFO1_1("ps_sys_event_ind_ex :policy refresh status %d",
                      policy_refresh_status_ptr->status);

      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_3GPP_POLICY_AVAILABLE:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_policy_available_info_type),
             ( ps_sys_policy_available_info_type *) event_info,
             sizeof( ps_sys_policy_available_info_type));

      policy_available_info_ptr = 
         (ps_sys_policy_available_info_type *)sys_event_info_ptr;

      LOG_MSG_INFO1_1("ps_sys_event_ind_ex :policy file name %s",
                      policy_available_info_ptr->policy_file_name);


      invoke_cb = TRUE;
      break;
    }
    
     
    case PS_SYS_EVENT_WLAN_START_WIFI_MEAS:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_wlan_wifi_meas_start_type),
              (ps_sys_wlan_wifi_meas_start_type *) event_info,
              sizeof(ps_sys_wlan_wifi_meas_start_type));

      start_wifi_meas_ptr = (ps_sys_wlan_wifi_meas_start_type*)sys_event_info_ptr;

      LOG_MSG_INFO2_1("ps_sys_event_ind_ex : Start Wifi Meas ID %d",
                       start_wifi_meas_ptr->meas_id);

      invoke_cb = TRUE;
      break;
    }  
    
    case PS_SYS_EVENT_WLAN_STOP_WIFI_MEAS:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_wlan_wifi_meas_stop_type),
              (ps_sys_wlan_wifi_meas_stop_type *) event_info,
              sizeof(ps_sys_wlan_wifi_meas_stop_type));

      stop_wifi_meas_ptr = (ps_sys_wlan_wifi_meas_stop_type*)sys_event_info_ptr;

      LOG_MSG_INFO2_1("ps_sys_event_ind_ex : Stop Wifi Meas ID %d",
                       stop_wifi_meas_ptr->meas_id);

      invoke_cb = TRUE;
      break;
    }               

    case PS_SYS_EVENT_WQE_PROFILES_MET:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_wqe_profile_type),
              (ps_sys_wqe_profile_type *) event_info,
              sizeof(ps_sys_wqe_profile_type));

      wqe_profile_mask = (ps_sys_wqe_profile_type*)sys_event_info_ptr;

      LOG_MSG_INFO2_2("ps_sys_event_ind_ex : WQE Profile mask 0x%x%08x",
                       (uint32)((wqe_profile_mask->wqe_profile_type) >> 32),
                       (uint32)(wqe_profile_mask->wqe_profile_type));

      invoke_cb = TRUE;
      break;
    }               
    
    case PS_SYS_EVENT_WQE_PROFILE_CHANGE:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_wqe_profile_change_type),
              (ps_sys_wqe_profile_change_type *) event_info,
              sizeof(ps_sys_wqe_profile_change_type));

      invoke_cb = TRUE;
      break;
    }
                   
    case PS_SYS_EVENT_REMOTE_SOCKET_REQUEST: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_remote_socket_request_type),
             (ps_sys_remote_socket_request_type *) event_info,
             sizeof(ps_sys_remote_socket_request_type));
      invoke_cb = TRUE;
      break;
    }  
    
    case PS_SYS_EVENT_REMOTE_SOCKET_RELEASE: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_remote_socket_release_list_type),
             (ps_sys_remote_socket_release_list_type *) event_info,
             sizeof(ps_sys_remote_socket_release_list_type));
      invoke_cb = TRUE;
      break;
    }  
    
    case PS_SYS_EVENT_REMOTE_SOCKET_SET_OPTION: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_remote_socket_set_option_type),
             (ps_sys_remote_socket_set_option_type *) event_info,
             sizeof(ps_sys_remote_socket_set_option_type));
      invoke_cb = TRUE;
      break;
    }               

    case PS_SYS_EVENT_NAT_KEEP_ALIVE_INFO:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_nat_keep_alive_info_type),
              (ps_sys_event_nat_keep_alive_info_type *) event_info,
              sizeof(ps_sys_event_nat_keep_alive_info_type));

      nat_keep_alive_ptr = (ps_sys_event_nat_keep_alive_info_type*)sys_event_info_ptr;

      LOG_MSG_INFO2_1("ps_sys_event_ind_ex : NAT Keep ALive start/stop %d",
                       nat_keep_alive_ptr->is_start);

      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_DATA_ACTIVITY_RESUMED:
    {
      /*no event payload . just invoke callbacks*/
      LOG_MSG_INFO2_0("ps_sys_event_ind_ex : PS_SYS_EVENT_DATA_ACTIVITY_RESUMED");
      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_QUALITY_MEAS_POLICY_CHANGE:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_quality_meas_policy_change_type),
              (ps_sys_quality_meas_policy_change_type*)event_info,
              sizeof(ps_sys_quality_meas_policy_change_type));
      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_SET_WQE_PROFILE_QUALITY_MEAS_INFO:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_set_wqe_profile_quality_meas_info_type),
              (ps_sys_event_set_wqe_profile_quality_meas_info_type*)event_info,
              sizeof(ps_sys_event_set_wqe_profile_quality_meas_info_type));
    
      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_3GPP_LTE_ATTACH_PARAMS_EX: 
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_3gpp_lte_attach_params_ex_type),
             (ps_sys_event_3gpp_lte_attach_params_ex_type *) event_info,
             sizeof(ps_sys_event_3gpp_lte_attach_params_ex_type));
      invoke_cb = TRUE;      
      break;
    }

    case PS_SYS_EVENT_APN_PCO_INFO_CHANGE: 
    {     
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_pco_info_per_apn_type),
             (ps_sys_pco_info_per_apn_type *) event_info,
             sizeof(ps_sys_pco_info_per_apn_type));
                     
      invoke_cb = TRUE;
      break;
    }     

    case PS_SYS_EVENT_APN_MSISDN_INFO_CHANGE: 
    {     
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_msisdn_info_per_apn_type),
             (ps_sys_msisdn_info_per_apn_type *) event_info,
             sizeof(ps_sys_msisdn_info_per_apn_type));
                     
      invoke_cb = TRUE;
      break;
    } 

    case PS_SYS_EVENT_WQE_PROFILE_STATUS:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_wqe_profiles_status_type),
              (ps_sys_wqe_profiles_status_type*)event_info,
              sizeof(ps_sys_wqe_profiles_status_type));
    
      invoke_cb = TRUE;
      break;
    }

     case PS_SYS_EVENT_3GPP_WWAN_CONN_STATE:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_wwan_conn_state_type),
              (ps_sys_wwan_conn_state_type*)event_info,
              sizeof(ps_sys_wwan_conn_state_type));
    
      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_SWITCH_DDS_IND: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_switch_dds_type),
             (ps_sys_event_switch_dds_type *) event_info,
             sizeof(ps_sys_event_switch_dds_type));
      invoke_cb = TRUE;
      break;
    }    

    case PS_SYS_EVENT_CURRENT_DDS_IND: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_current_dds_type),
             (ps_sys_current_dds_type *) event_info,
             sizeof(ps_sys_current_dds_type));
      invoke_cb = TRUE;
      break;
    } 

    case PS_SYS_EVENT_3GPP_POLICYMAN_MSG_IND:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_policyman_msg_info_type),
              (ps_sys_policyman_msg_info_type*)event_info,
              sizeof(ps_sys_policyman_msg_info_type));
    
      invoke_cb = TRUE;
      break;
    } 

    case PS_SYS_EVENT_3GPP_RAB_REJECT_IND:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_3gpp_rab_reject_info_type),
              (ps_sys_3gpp_rab_reject_info_type*)event_info,
              sizeof(ps_sys_3gpp_rab_reject_info_type));
    
      invoke_cb = TRUE;
      break;
    }
    
    case PS_SYS_EVENT_WIFI_BAND_VACATE_IND: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_wifi_band_vacate_type),
             (ps_sys_wifi_band_vacate_type *) event_info,
             sizeof(ps_sys_wifi_band_vacate_type));
      invoke_cb = TRUE;
      break;
    } 

    case PS_SYS_EVENT_MODEM_ASST_KEEP_ALIVE_STATUS: 
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_modem_asst_ka_status_info_type),
             (ps_sys_event_modem_asst_ka_status_info_type *) event_info,
              sizeof(ps_sys_event_modem_asst_ka_status_info_type));
      invoke_cb = TRUE;
      break;
    } 
    
    case PS_SYS_EVENT_MODEM_ASST_KEEP_ALIVE_UPDATE: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_modem_asst_ka_update_info_type),
             (ps_sys_modem_asst_ka_update_info_type *) event_info,
              sizeof(ps_sys_modem_asst_ka_update_info_type));
      invoke_cb = TRUE;
      break;
    }     
     case PS_SYS_EVENT_UPDATED_IMSI_PUBLIC_KEY_INFO: 
     {       
       memscpy(sys_event_info_ptr,
               sizeof(ps_sys_imsi_public_key_info_type),
              (ps_sys_imsi_public_key_info_type *) event_info,
              sizeof(ps_sys_imsi_public_key_info_type));   

       invoke_cb = TRUE;
       break;             
     }

    case PS_SYS_EVENT_RETRIEVE_IMSI_PUBLIC_KEY_INFO:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_imsi_public_key_retrieve_info),
              (ps_sys_event_imsi_public_key_retrieve_info *) event_info,
              sizeof(ps_sys_event_imsi_public_key_retrieve_info));

      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_WLAN_REFRESH_COUNTRY_CODE: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_wlan_country_code_action_enum),
             (ps_sys_event_wlan_country_code_action_enum *) event_info,
             sizeof(ps_sys_event_wlan_country_code_action_enum));
      invoke_cb = TRUE;
      break;
    } 

    case PS_SYS_EVENT_3GPP_BACKGROUND_TRAFFIC_IND: 
    { 
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_3gpp_background_traffic_info),
             (ps_sys_3gpp_background_traffic_info *) event_info,
              sizeof(ps_sys_3gpp_background_traffic_info));
      invoke_cb = TRUE;
      break;
    }
  
    case PS_SYS_EVENT_3GPP_LTE_ATTACH_PDN_LIST_ACTION_RESULT: 
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_3gpp_lte_attach_pdn_list_action_result),
             (ps_sys_event_3gpp_lte_attach_pdn_list_action_result *) event_info,
             sizeof(ps_sys_event_3gpp_lte_attach_pdn_list_action_result));
      invoke_cb = TRUE;      
      break;
    }

    case PS_SYS_EVENT_3GPP_DATA_CONNECTION_STATUS_CHANGE:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_3gpp_data_connection_status_info_type),
              (ps_sys_event_3gpp_data_connection_status_info_type *) event_info,
              sizeof(ps_sys_event_3gpp_data_connection_status_info_type));

      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_UL_THRPUT_ACTUAL_THRES_CRITERIA: 
    {       
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_ul_thrput_actual_thres_criteria_type),
             (ps_sys_ul_thrput_actual_thres_criteria_type *) event_info,
             sizeof(ps_sys_ul_thrput_actual_thres_criteria_type));
      invoke_cb = TRUE;
      break;             
    }
    
    case PS_SYS_EVENT_DL_THRPUT_ACTUAL_THRES_CRITERIA: 
    {       
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_dl_thrput_actual_thres_criteria_type),
             (ps_sys_dl_thrput_actual_thres_criteria_type *) event_info,
             sizeof(ps_sys_dl_thrput_actual_thres_criteria_type));   
      invoke_cb = TRUE;
      break;             
    }

    case PS_SYS_EVENT_CA_CERT_RETRIEVAL_SUPPORT:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_ca_cert_retrieval_support),
              (ps_sys_event_ca_cert_retrieval_support *) event_info,
              sizeof(ps_sys_event_ca_cert_retrieval_support));

      invoke_cb = TRUE;
      break;
    }
    case PS_SYS_EVENT_GET_CA_CERTIFICATE:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_get_ca_certificate),
              (ps_sys_event_get_ca_certificate *) event_info,
              sizeof(ps_sys_event_get_ca_certificate));

      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_3GPP_CONFIG_APP_PREF_RESULT_IND:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_3gpp_config_app_pref_result_type),
              (ps_sys_event_3gpp_config_app_pref_result_type *) event_info,
              sizeof(ps_sys_event_3gpp_config_app_pref_result_type));

      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_3GPP_REMOVE_APP_PREF_RESULT_IND:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_3gpp_remove_app_pref_result_type),
              (ps_sys_event_3gpp_remove_app_pref_result_type *) event_info,
              sizeof(ps_sys_event_3gpp_remove_app_pref_result_type));

      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_3GPP_PS_DATA_AVAIL_CHANGE:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_ps_data_avail_type),
              (ps_sys_ps_data_avail_type *) event_info,
              sizeof(ps_sys_ps_data_avail_type));

      invoke_cb = TRUE;
      break;
    }  

    case PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_MODE:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_ap_asst_apn_pref_sys_type),
              (ps_sys_ap_asst_apn_pref_sys_type *) event_info,
              sizeof(ps_sys_ap_asst_apn_pref_sys_type));

      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_CHANGE:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_ap_asst_apn_pref_sys_type),
              (ps_sys_ap_asst_apn_pref_sys_type *) event_info,
              sizeof(ps_sys_ap_asst_apn_pref_sys_type));

      invoke_cb = TRUE;
      break;
    }
    
    case PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_CHANGE_REQ:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_ap_asst_apn_pref_sys_change_req_type),
              (ps_sys_event_ap_asst_apn_pref_sys_change_req_type *) event_info,
              sizeof(ps_sys_event_ap_asst_apn_pref_sys_change_req_type));

      invoke_cb = TRUE;
      break;
    }

    case PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_RESULT_IND:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_ap_asst_apn_pref_sys_result_type),
              (ps_sys_event_ap_asst_apn_pref_sys_result_type *) event_info,
              sizeof(ps_sys_event_ap_asst_apn_pref_sys_result_type));

      invoke_cb = TRUE;
      break;
    }
    
    case PS_SYS_EVENT_APN_PREF_SYS_CHANGE_EX:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_apn_pref_sys_type),
              (ps_sys_apn_pref_sys_type *) event_info,
              sizeof(ps_sys_apn_pref_sys_type));

      invoke_cb = TRUE;
      break;
    }
    
    case PS_SYS_EVENT_3GPP_LTE_CELL_HANDOVER_COMPLETE:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_3gpp_lte_cell_ho_info_type),
              (ps_sys_event_3gpp_lte_cell_ho_info_type *) event_info,
              sizeof(ps_sys_event_3gpp_lte_cell_ho_info_type));

      invoke_cb = TRUE;
      break;
    }
    #ifdef QWES_FEATURE_ENABLE 

    case PS_SYS_EVENT_SYNC_COMPLETE_BROADCAST: 
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_sync_complete_broadcast_type),
             (ps_sys_event_sync_complete_broadcast_type *) event_info,
             sizeof(ps_sys_event_sync_complete_broadcast_type));
      invoke_cb = TRUE;      
      break;
    }   

    case PS_SYS_EVENT_CAAS_FEATURE_STATUS:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_caas_feature_status_list_type),
              (ps_sys_caas_feature_status_list_type *) event_info,
              sizeof(ps_sys_caas_feature_status_list_type));

      invoke_cb = TRUE;
      break;
    } 

    case PS_SYS_EVENT_QTEELS_AVAILABLE:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_qteels_avail_info_type),
              (ps_sys_event_qteels_avail_info_type *) event_info,
              sizeof(ps_sys_event_qteels_avail_info_type));

      invoke_cb = TRUE;
      break;
    }
    case PS_SYS_EVENT_QWES_NACK:
    {
      memscpy(sys_event_info_ptr,
              sizeof(ps_sys_event_qwes_nack_info_type),
              (ps_sys_event_qwes_nack_info_type *) event_info,
              sizeof(ps_sys_event_qwes_nack_info_type));
      invoke_cb =TRUE;
      break;
    }
    #endif /* QWES_FEATURE_ENABLE  */
    default:
    {
      *ps_errno = DS_EINVAL;
      PS_SYSTEM_HEAP_MEM_FREE(sys_event_info_ptr);
      LOG_MSG_ERROR_1("ps_sys_event_ind_ex(): Invalid Ev %d ", event_name);
      return -1;
    }
  }

  if (invoke_cb == TRUE)
  {
    PS_SYSTEM_HEAP_MEM_ALLOC(cmd_data_info, sizeof(ps_sys_eventi_cmd_type),
                              ps_sys_eventi_cmd_type*);    

    if (NULL == cmd_data_info)
    {
      *ps_errno = DS_NOMEMORY;
      PS_SYSTEM_HEAP_MEM_FREE(sys_event_info_ptr);
      return -1;
    }

    cmd_data_info->event           = event_name;
    cmd_data_info->tech_type       = tech_type;
    cmd_data_info->event_info      = cmd_event_info;
    cmd_data_info->subscription_id = subscription_id;

    ps_send_cmd(PS_SYS_EVENT_CMD_HANDLER,(void *)cmd_data_info);
  }
  
  return 0;
}/* ps_sys_event_ind_ex() */


int16 ps_sys_event_dereg
(
  ps_sys_tech_enum_type                  tech_type,
  ps_sys_event_enum_type                 event_name,
  ps_sys_event_cback_f_ptr_type          event_cback_f_ptr,
  int16                                 *ps_errno
)
{
  return ps_sys_eventi_dereg (tech_type,
                              event_name,
                              PS_SYS_DEFAULT_SUBS,
                              event_cback_f_ptr,
                              NULL,
                              NULL,
                              ps_errno);
} /* ps_sys_event_dereg() */


int16 ps_sys_event_dereg_ex
(
  ps_sys_tech_enum_type                  tech_type,
  ps_sys_event_enum_type                 event_name,
  ps_sys_subscription_enum_type          subscription_id,
  ps_sys_event_cback_f_ptr_ex_type       event_cback_f_ptr,
  int16                                * ps_errno
)
{
  return ps_sys_eventi_dereg (tech_type,
                              event_name,
                              subscription_id,
                              NULL,
                              event_cback_f_ptr,
                              NULL,
                              ps_errno);
} /* ps_sys_event_dereg_ex() */

int16 ps_sys_event_dereg_ex2
(
  ps_sys_tech_enum_type                  tech_type,
  ps_sys_event_enum_type                 event_name,
  ps_sys_subscription_enum_type          subscription_id,
  ps_sys_event_cback_f_ptr_ex_type       event_cback_f_ptr,
  void                                 * client_data_ptr,
  int16                                * ps_errno
)
{
  return ps_sys_eventi_dereg (tech_type,
                              event_name,
                              subscription_id,
                              NULL,
                              event_cback_f_ptr,
                              client_data_ptr,
                              ps_errno);
} /* ps_sys_event_dereg_ex() */


int16 ps_sys_eventi_dereg
(
  ps_sys_tech_enum_type                  tech_type,
  ps_sys_event_enum_type                 event_name,
  ps_sys_subscription_enum_type          subscription_id,
  ps_sys_event_cback_f_ptr_type          event_cback_f_ptr,
  ps_sys_event_cback_f_ptr_ex_type       event_cback_f_ex_ptr,
  void                                 * client_data_ptr,
  int16                                * ps_errno
)
{
  q_type                       * event_q_ptr = NULL;
  void                         * event_buf_ptr;
  void                         * next_event_buf_ptr;
  ps_sys_eventi_buf_type       * ps_sys_event_buf_ptr = NULL;
  int                            ret_val;
  boolean                        event_buf_found = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if((event_name == PS_SYS_EVENT_APN_PCO_INFO_CHANGE ||
      event_name == PS_SYS_EVENT_APN_MSISDN_INFO_CHANGE) &&
      client_data_ptr == NULL)
  {
    *ps_errno = DS_EINVAL;
    return -1;
  }

  LOG_MSG_INFO1_4("ps_sys_eventi_dereg(): event %d tech %d cb 0x%p cb_ex 0x%p",
                   event_name, tech_type, event_cback_f_ptr, event_cback_f_ex_ptr);

                          
  if (TRUE == ds_sysi_is_modem_reset_in_progress())
  {
    LOG_MSG_ERROR_2("ps_sys_eventi_dereg(): Modem restart in progress "
                    "event %d tech %d ", event_name, tech_type);
    return 0;
  }

  if (ps_errno == NULL)
  {
    LOG_MSG_ERROR_0("ps_sys_eventi_dereg():Invalid argument ps_errno passed");
    return -1;
  }

  ret_val = ps_sys_eventi_verify_tech_event(tech_type,event_name);
  if ((event_cback_f_ptr == NULL && event_cback_f_ex_ptr == NULL) || 
       ret_val == -1)
  {
    *ps_errno = DS_EFAULT;
    LOG_MSG_ERROR_0("ps_sys_eventi_dereg():Invalid argument passed");
    return -1;
  }

  if (subscription_id >= PS_SYS_SUBS_MAX || 
      subscription_id < PS_SYS_DEFAULT_SUBS)
  {
    *ps_errno = DS_EFAULT;
    LOG_MSG_ERROR_1("ps_sys_eventi_dereg(): Invalid subscription_id %d",
                     subscription_id);
    return -1;
  }

  if (PS_SYS_DEFAULT_SUBS == subscription_id )
  {
    subscription_id = ps_sys_get_default_data_subscription();
  }

  /* Special Case: to inform DPM that a client has been 
     deregistered for data activity resumed event*/
  if ( event_name == PS_SYS_EVENT_DATA_ACTIVITY_RESUMED )
  {
    ps_dpm_data_activity_client_status_ind(subscription_id,FALSE);
  }

  event_q_ptr = &ps_sys_eventi_q;

  PS_ENTER_CRIT_SECTION(&ps_sys_crit_section);
  /*-------------------------------------------------------------------------
    Traverse the queue of callbacks for this event
  -------------------------------------------------------------------------*/
  event_buf_ptr = q_check(event_q_ptr);
  while (event_buf_ptr != NULL)
  {
    next_event_buf_ptr =
    q_next(event_q_ptr, &((q_generic_item_type *) event_buf_ptr)->link);

    ps_sys_event_buf_ptr = (ps_sys_eventi_buf_type *) event_buf_ptr;

    if (ps_sys_event_buf_ptr->tech_type == tech_type && 
        ps_sys_event_buf_ptr->subscription_id == subscription_id &&
        ps_sys_event_buf_ptr->event_name == event_name)
    {
      if((event_cback_f_ptr != NULL && 
           ps_sys_event_buf_ptr->event_cback_f_ptr == event_cback_f_ptr) ||
         (event_cback_f_ex_ptr != NULL && 
           ps_sys_event_buf_ptr->event_cback_f_ex_ptr == event_cback_f_ex_ptr))
      {
        switch(event_name)
        {
          case PS_SYS_EVENT_APN_PCO_INFO_CHANGE:
          case PS_SYS_EVENT_APN_MSISDN_INFO_CHANGE:
          {
            if ((ps_sys_event_buf_ptr->user_data_ptr == client_data_ptr) ||
               ((NULL != client_data_ptr) && (0 == strcmp((char *)ps_sys_event_buf_ptr->user_data_ptr, 
                            (char *)client_data_ptr))))
            {
              event_buf_found = TRUE;
            }
            break;
          }
          default:
            event_buf_found = TRUE;
            break;
        }
        
        if (TRUE == event_buf_found)
        {
          break;
        }
      }
    }
    event_buf_ptr = next_event_buf_ptr;
  } /* while (items in queue) */
  
  if (event_buf_ptr == NULL || ps_sys_event_buf_ptr == NULL)
  {
    *ps_errno = DS_EFAULT;
    LOG_MSG_ERROR_4("ps_sys_eventi_dereg(): cb 0x%p cb_ex 0x%p for event %d" 
                    " user data ptr 0x%x not found",
                     event_cback_f_ptr, event_cback_f_ex_ptr, event_name,
                     client_data_ptr);
    PS_LEAVE_CRIT_SECTION(&ps_sys_crit_section);
    return -1;
  }

  /*-------------------------------------------------------------------------
    make sure that this event is on a queue - if not mark it as such
  -------------------------------------------------------------------------*/
  if (!Q_ALREADY_QUEUED(&(ps_sys_event_buf_ptr->link)))
  {
    *ps_errno = DS_EINVAL;
    LOG_MSG_ERROR_1("ps_sys_eventi_dereg(): Event %d not present in queue",
                    event_name);
    PS_LEAVE_CRIT_SECTION(&ps_sys_crit_section);
    return -1;
  }
  
  /*-------------------------------------------------------------------------
    make sure we have a valid queue to remove from - if so use the queue
    function to remove the item from the queue.
  -------------------------------------------------------------------------*/
  #ifdef FEATURE_Q_NO_SELF_QPTR
    q_delete(event_q_ptr, &(ps_sys_event_buf_ptr->link));
  #else
    q_delete(&(ps_sys_event_buf_ptr->link));
  #endif

  ps_sys_eventi_free_cback_buf((void *)event_buf_ptr);

  PS_LEAVE_CRIT_SECTION(&ps_sys_crit_section);

  return 0;
} /* ps_sys_eventi_dereg() */

int16 ps_sys_eventi_empty_event_queue
(
  ps_sys_event_enum_type                 event_name,
  int16                                * ps_errno
)
{
  q_type                       * event_q_ptr = NULL;
  void                         * event_buf_ptr;
  void                         * next_event_buf_ptr;
  ps_sys_eventi_buf_type       * ps_sys_event_buf_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (ps_errno == NULL)
  {
    LOG_MSG_INVALID_INPUT_0("ps_sys_eventi_empty_event_queue() Invalid pserrno");
    return -1;
  }

  event_q_ptr = &ps_sys_eventi_q;

  /*-------------------------------------------------------------------------
    Traverse the queue of callbacks for this event
  -------------------------------------------------------------------------*/
  PS_ENTER_CRIT_SECTION(&ps_sys_crit_section);

  event_buf_ptr = q_check(event_q_ptr);
  while (event_buf_ptr != NULL)
  {
    next_event_buf_ptr =
      q_next(event_q_ptr, &((q_generic_item_type *) event_buf_ptr)->link);

    ps_sys_event_buf_ptr = (ps_sys_eventi_buf_type *) event_buf_ptr;
    
    /*-----------------------------------------------------------------------
      make sure we have a valid queue to remove from - if so use the queue
      function to remove the item from the queue.
    -----------------------------------------------------------------------*/
#ifdef FEATURE_Q_NO_SELF_QPTR
    q_delete(event_q_ptr, &(ps_sys_event_buf_ptr->link));
#else
    q_delete(&(ps_sys_event_buf_ptr->link));
#endif

    ps_sys_eventi_free_cback_buf((void *) event_buf_ptr);

    event_buf_ptr = next_event_buf_ptr;
  } /* while (items in queue) */

  PS_LEAVE_CRIT_SECTION(&ps_sys_crit_section); 
  return 0;

} /* ps_sys_eventi_empty_event_queue() */

void ps_sys_eventi_free_cback_buf
(
  void * buf
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_1("ps_sys_eventi_free_cback_buf(): buf 0x%p", buf);

  if (buf == NULL)
  {
    DS_UTILS_ERROR_FATAL("ps_sys_eventi_free_cback_buf(): Buffer in input parameter is null");
    return;
  }

  /*-------------------------------------------------------------------------
    Validate the buffer that is passed in not already on a queue
  -------------------------------------------------------------------------*/
  if (Q_ALREADY_QUEUED(&(((ps_sys_eventi_buf_type*)buf)->link)))
  {
    DS_UTILS_ERROR_FATAL("ps_sys_eventi_free_cback_buf(): Buffer passed in already in queue");
    return;
  }

  /*-------------------------------------------------------------------------
    Invalidate the buffer so that the caller cannot use it without
    explicitly allcating it again.
  -------------------------------------------------------------------------*/
  ((ps_sys_eventi_buf_type *) buf)->event_cback_f_ptr = NULL;

  ((ps_sys_eventi_buf_type *) buf)->event_cback_f_ex_ptr = NULL;

  if(((ps_sys_eventi_buf_type *) buf)->event_name == PS_SYS_EVENT_APN_PCO_INFO_CHANGE ||
     ((ps_sys_eventi_buf_type *) buf)->event_name == PS_SYS_EVENT_APN_MSISDN_INFO_CHANGE)
  { 
    if (((ps_sys_eventi_buf_type *) buf)->user_data_ptr != NULL)
    {
      PS_SYSTEM_HEAP_MEM_FREE(((ps_sys_eventi_buf_type *) buf)->user_data_ptr);
    }
  }

  PS_SYSTEM_HEAP_MEM_FREE(buf);

} /* ps_sys_eventi_free_cback_buf() */


void ps_sys_eventi_cback_cmd_handler
(
  ps_cmd_enum_type    cmd,
  void *              userDataPtr
)
{
  ps_sys_eventi_cmd_type       * cmd_event_info;
  ps_sys_event_enum_type        event_id;
  int                           ret_val;
  int16                         ds_errno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == userDataPtr || PS_SYS_EVENT_CMD_HANDLER != cmd)
  {
    LOG_MSG_INVALID_INPUT_2("ps_sys_eventi_cback_cmd_handler(): Invalid args: "
                            "cmd %d, userDataPtr 0x%p", cmd, userDataPtr);
    DS_UTILS_ASSERT(0);
    return;
  }

  cmd_event_info = (ps_sys_eventi_cmd_type *)userDataPtr;

  /*-------------------------------------------------------------------------
    In case of modem restart, free up the event queue
  -------------------------------------------------------------------------*/
  if (cmd_event_info->event == PS_SYS_EVENT_MODEM_OUT_OF_SERVICE)
  {
    LOG_MSG_INFO2_0("ps_sys_eventi_cback_cmd_handler(): out of service event" );

    (void)ds_sys_qmi_client_release();

    for (event_id = PS_SYS_EVENT_MIN; event_id < PS_SYS_EVENT_MAX; event_id++)
    {
      ret_val = ps_sys_eventi_empty_event_queue(event_id, &ds_errno);
      if (0 > ret_val)
      {
        LOG_MSG_INFO2_2("ps_sys_eventi_cback_cmd_handler(): empty queue"
                        " event %d ds_errno %d", event_id, ds_errno);
      }
    }
  }
  else if (cmd_event_info->event == PS_SYS_EVENT_MODEM_IN_SERVICE)
  {
    LOG_MSG_INFO2_0("ps_sys_eventi_cback_cmd_handler(): in service event" );

    (void)ds_sys_qmi_client_init();

    ds_sysi_set_modem_reset_in_progress(FALSE);
  }
  else if (cmd_event_info->event == PS_SYS_EVENT_START_FAST_DORMANCY)
  {
    /* sys fast dormancy API to start fast dormancy algorithm */
    if (0 == ps_sys_fast_dormancy_start(cmd_event_info->event_info))
    {
      LOG_MSG_INFO2_0("ps_sys_eventi_cback_cmd_handler():"
                                    " Start Fast dormancy Success" );
    }
    /* freeing the dynamic memory */
    PS_SYSTEM_HEAP_MEM_FREE(cmd_event_info->event_info);
  }
   /* Check if FD algorithm is running or not before stopping it. It is that 
      MH has no knowledge of fast dormancy, so can trigger stop FD 
      even if it was not running*/
  else if((cmd_event_info->event == PS_SYS_EVENT_STOP_FAST_DORMANCY)&&
          (ps_sys_fast_dormancy_is_timer_running(PS_SYS_FAST_DORMANCY_TIMER_1))
                                                                       == TRUE)
  {   
    /* sys fast dormancy API to stop fast dormancy algorithm*/
    ps_sys_fast_dormancy_stop();
  }
  else
  {
    ps_sys_eventi_invoke_cbacks(cmd_event_info->tech_type, 
                                cmd_event_info->event, 
                                cmd_event_info->event_info,
                                cmd_event_info->subscription_id);

    PS_SYSTEM_HEAP_MEM_FREE(cmd_event_info->event_info);
  }

  PS_SYSTEM_HEAP_MEM_FREE(cmd_event_info);

} /* ps_sys_eventi_cback_cmd_handler() */

int ps_sys_eventi_verify_tech_event
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name
)
{
  ds_local_error_e_type                local_error = ERR_NO_ERROR;
  do
  {
    if (event_name >= PS_SYS_EVENT_3GPP_MIN && 
        event_name < PS_SYS_EVENT_3GPP_INTERNAL_MAX)
    {
      if(tech_type != PS_SYS_TECH_3GPP)
      {
        local_error = ERR_INVALID_TECH_3GPP_EVENT;
        break;
      }
      return 0;
    }

    if (event_name >= PS_SYS_EVENT_3GPP2_MIN && 
        event_name < PS_SYS_EVENT_3GPP2_INTERNAL_MAX)
    {
      if(tech_type != PS_SYS_TECH_3GPP2)
      {
        local_error = ERR_INVALID_TECH_3GPP2_EVENT;
        break;
      }
      return 0;
    }

    if (event_name >= PS_SYS_EVENT_WLAN_MIN && 
        event_name < PS_SYS_EVENT_WLAN_INTERNAL_MAX)
    {
      if(tech_type != PS_SYS_TECH_WLAN)
      {
        local_error = ERR_INVALID_TECH_WLAN_EVENT;
        break;
      }
      return 0;
    }

    if (event_name >= PS_SYS_EVENT_COMMON_MIN && 
        event_name < PS_SYS_EVENT_COMMON_INTERNAL_MAX)
    {
      if(event_name == PS_SYS_EVENT_SYSTEM_STATUS_CHANGE)
      {
        if(tech_type != PS_SYS_TECH_ALL)
        {
          local_error = ERR_INVALID_TECH_COMMON_EVENT;
          break;
        }
      }
      return 0;
    }
  }
  while(0);

  LOG_MSG_INVALID_INPUT_2("ps_sys_eventi_verify_tech_event(): Invalid event %d "
                          "name passed, local_error %d", event_name, local_error);
  return -1;
} /* ps_sys_eventi_verify_tech_event() */
