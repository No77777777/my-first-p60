/******************************************************************************
  @file    ds_sys_event.c
  @brief   Data Service System Event Configuration implementation

  DESCRIPTION
  This file contains implementation of Data Service System Configuration API
  for registering and processing events callbacks.

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2011-2013 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.
 
  $Header: //components/rel/data.mpss/3.5..2.1/interface/sysapi/src/ds_sys_event.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $ 

  
when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/31/11   bvd     Created module
===========================================================================*/

/*---------------------------------------------------------------------------
                           INCLUDE FILES
---------------------------------------------------------------------------*/
#include "ds_sys_event.h"
#include "ps_sys_event.h"
#include "dserrno.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_utils.h"

int16 ds_sys_event_reg
(
  ds_sys_tech_enum_type                  tech_type,
  ds_sys_event_enum_type                 event_name,
  ds_sys_event_cback_f_ptr_type          event_cback_f_ptr,
  void                                 * user_data_ptr,
  int16                                * dss_errno
)
{
  int16 ret;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3("ds_sys_event_reg(): event %d tech %d cb 0x%p",
                           event_name, tech_type, event_cback_f_ptr);

  if(event_name == DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_STATUS ||
     event_name == DS_SYS_EVENT_DUN_CTRL_CALL_STATUS ||
     event_name == DS_SYS_EVENT_DUN_CTRL_ENTITLEMENT_CHECK ||
     event_name == DS_SYS_EVENT_DUN_CTRL_SILENT_REDIAL_CHECK )
  {
    LOG_MSG_ERROR_2("Event cannot be registered. It is done implicitly" 
                    "Event %d Tech type %d ", event_name, tech_type);
    goto bail;
  }

  if (event_name < DS_SYS_EVENT_COMMON_MIN ||
     event_name >= DS_SYS_EVENT_COMMON_MAX)
  {
    switch (tech_type)
    {
      case DS_SYS_TECH_3GPP:
      {  
        if (event_name >= DS_SYS_EVENT_3GPP_MAX || 
            event_name < DS_SYS_EVENT_3GPP_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_reg(): 3gpp event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      }

      case DS_SYS_TECH_3GPP2:
      {  
        if (event_name >= DS_SYS_EVENT_3GPP2_MAX || 
            event_name < DS_SYS_EVENT_3GPP2_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_reg(): 3gpp2 event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      }  

      case DS_SYS_TECH_WLAN:
      {  
        if (event_name >= DS_SYS_EVENT_WLAN_MAX || 
            event_name < DS_SYS_EVENT_WLAN_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_reg(): WLAN event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      } 

      case DS_SYS_TECH_ALL:
      {  
        if (event_name >= DS_SYS_EVENT_COMMON_MAX || 
            event_name < DS_SYS_EVENT_COMMON_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_reg(): tech_call event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      }

      default:
      {     
        LOG_MSG_INVALID_INPUT_1("ds_sys_event_reg(): Invalid tech %d",
                                tech_type);
        goto bail;
      }
    }
  }
  
  ret = ps_sys_event_reg((ps_sys_tech_enum_type)tech_type,
                         (ps_sys_event_enum_type)event_name, 
                         (ps_sys_event_cback_f_ptr_type) event_cback_f_ptr, 
                         user_data_ptr,
                         dss_errno);
  return ret;

bail:
  *dss_errno = DS_EFAULT;
  return -1;

}/* ds_sys_event_reg() */

int16 ds_sys_event_reg_ex
(
  ds_sys_tech_enum_type                  tech_type,
  ds_sys_event_enum_type                 event_name,
  ds_sys_subscription_enum_type          subscription_id,
  ds_sys_event_cback_f_ptr_ex_type       event_cback_f_ptr,
  void                                 * user_data_ptr,
  int16                                * dss_errno
)
{
  int16 ret;
  ds_local_error_e_type local_error = ERR_NO_ERROR;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_QTRACE_HFT_MED(PS_TAG_SYSAPI,(ps_sys_subscription_enum_type)subscription_id,
                 "ds_sys_event_reg_ex(): event %d tech %d cb 0x%x subscription id %d",
                 event_name, tech_type, event_cback_f_ptr, subscription_id);

  do
  {
    if(event_name == DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_STATUS ||
       event_name == DS_SYS_EVENT_DUN_CTRL_CALL_STATUS ||
       event_name == DS_SYS_EVENT_DUN_CTRL_ENTITLEMENT_CHECK ||
       event_name == DS_SYS_EVENT_DUN_CTRL_SILENT_REDIAL_CHECK )
    {
      /* Event cannot be registered. It is done implicitly. */
      local_error = ERR_EVENT_REG_NOT_SUPP;
      break;
    }

    if (event_name < DS_SYS_EVENT_COMMON_MIN ||
        event_name >= DS_SYS_EVENT_COMMON_MAX)
    {
      switch (tech_type)
      {
        case DS_SYS_TECH_3GPP:
        {  
          if (event_name >= DS_SYS_EVENT_3GPP_MAX || 
              event_name < DS_SYS_EVENT_3GPP_MIN)
          {
            local_error = ERR_INVALID_TECH_3GPP_EVENT;
          }
          break;
        }

        case DS_SYS_TECH_3GPP2:
        {  
          if (event_name >= DS_SYS_EVENT_3GPP2_MAX || 
              event_name < DS_SYS_EVENT_3GPP2_MIN)
          {
            local_error = ERR_INVALID_TECH_3GPP2_EVENT;
          }
          break;
        }  

        case DS_SYS_TECH_WLAN:
        {  
          if (event_name >= DS_SYS_EVENT_WLAN_MAX || 
              event_name < DS_SYS_EVENT_WLAN_MIN)
          {
            local_error = ERR_INVALID_TECH_WLAN_EVENT;
          }
          break;
        } 

        case DS_SYS_TECH_ALL:
        {  
          if (event_name >= DS_SYS_EVENT_COMMON_MAX ||
              event_name < DS_SYS_EVENT_COMMON_MIN)
          {
            local_error = ERR_INVALID_TECH_COMMON_EVENT;
          }
          break;
        }

        default:
        {     
          /* Invalid tech */
          local_error = ERR_INVALID_TECH_EVENT;
        }
      }
      if(local_error != ERR_NO_ERROR)
      {
        break;
      }
    }
  
    if (subscription_id >= DS_SYS_SUBS_MAX || 
        subscription_id < DS_SYS_DEFAULT_SUBS)
    {
      /* Invalid subscription */
      local_error = ERR_INVALID_SUBS;
      break;
    }
  
    if (DS_SYS_DEFAULT_SUBS == subscription_id )
    {
      subscription_id = (ds_sys_subscription_enum_type)ps_sys_get_default_data_subscription();
    }
  
    ret = ps_sys_event_reg_ex ((ps_sys_tech_enum_type)tech_type,
                             (ps_sys_event_enum_type)event_name, 
                             (ps_sys_subscription_enum_type)subscription_id,
                             (ps_sys_event_cback_f_ptr_ex_type) event_cback_f_ptr, 
                             user_data_ptr,
                             dss_errno);
    return ret;
  }while(0);

  LOG_MSG_ERROR_1("ds_sys_event_reg_ex(): Returning with error %d", local_error);

  *dss_errno = DS_EFAULT;
  return -1;
} /* ds_sys_event_reg_ex() */


int16 ds_sys_event_dereg
(
  ds_sys_tech_enum_type                  tech_type,
  ds_sys_event_enum_type                 event_name,
  ds_sys_event_cback_f_ptr_type          event_cback_f_ptr,
  int16                                * dss_errno
)
{
  uint16 ret;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3("ds_sys_event_dereg(): event %d tech %d cb 0x%p",
                           event_name, tech_type, event_cback_f_ptr);

  if (event_name < DS_SYS_EVENT_COMMON_MIN ||
     event_name >= DS_SYS_EVENT_COMMON_MAX)
  {
    switch (tech_type)
    {
      case DS_SYS_TECH_3GPP:
      {  
        if (event_name >= DS_SYS_EVENT_3GPP_MAX || 
            event_name < DS_SYS_EVENT_3GPP_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg(): 3gpp event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      }

      case DS_SYS_TECH_3GPP2:
      {  
        if (event_name >= DS_SYS_EVENT_3GPP2_MAX || 
            event_name < DS_SYS_EVENT_3GPP2_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg(): 3gpp2 event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      }  

      case DS_SYS_TECH_WLAN:
      {  
        if (event_name >= DS_SYS_EVENT_WLAN_MAX || 
            event_name < DS_SYS_EVENT_WLAN_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_reg(): WLAN event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      }      

      case DS_SYS_TECH_ALL:
      {  
        if (event_name >= DS_SYS_EVENT_COMMON_MAX || 
            event_name < DS_SYS_EVENT_COMMON_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg(): tech_all event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      }

      default:
      {     
        LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg(): Invalid tech %d",
                                tech_type);
        goto bail;
      }
    }
  }
  
  ret = ps_sys_event_dereg((ps_sys_tech_enum_type)tech_type,
                           (ps_sys_event_enum_type)event_name, 
                           (ps_sys_event_cback_f_ptr_type) event_cback_f_ptr, 
                           dss_errno);
  return ret;

bail:
  *dss_errno = DS_EFAULT;
  return -1;
} /* ds_sys_event_dereg() */


int16 ds_sys_event_dereg_ex
(
  ds_sys_tech_enum_type                  tech_type,
  ds_sys_event_enum_type                 event_name,
  ds_sys_subscription_enum_type          subscription_id,
  ds_sys_event_cback_f_ptr_ex_type       event_cback_f_ptr,
  int16                                * dss_errno
)
{
  uint16 ret;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3("ds_sys_event_dereg_ex(): event %d tech %d cb 0x%p",
                           event_name, tech_type, event_cback_f_ptr);


  if (subscription_id >= DS_SYS_SUBS_MAX || 
      subscription_id < DS_SYS_DEFAULT_SUBS)
  {
    LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg_ex(): Invalid subscription_id %d",
                            subscription_id);
    goto bail;
  }                           

  if (DS_SYS_DEFAULT_SUBS == subscription_id )
  {
    subscription_id = (ds_sys_subscription_enum_type)ps_sys_get_default_data_subscription();
  }

  if (event_name < DS_SYS_EVENT_COMMON_MIN ||
     event_name >= DS_SYS_EVENT_COMMON_MAX)
  {
    switch (tech_type)
    {
      case DS_SYS_TECH_3GPP:
      {  
        if (event_name >= DS_SYS_EVENT_3GPP_MAX || 
            event_name < DS_SYS_EVENT_3GPP_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg_ex(): 3gpp event_name %d",
                                  event_name);
          goto bail;
}
        break;
      }

      case DS_SYS_TECH_3GPP2:
      {  
        if (event_name >= DS_SYS_EVENT_3GPP2_MAX || 
            event_name < DS_SYS_EVENT_3GPP2_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg_ex(): 3gpp2 event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      }  

      case DS_SYS_TECH_WLAN:
      {  
        if (event_name >= DS_SYS_EVENT_WLAN_MAX || 
            event_name < DS_SYS_EVENT_WLAN_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_reg(): WLAN event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      }

      case DS_SYS_TECH_ALL:
      {  
        if (event_name >= DS_SYS_EVENT_COMMON_MAX || 
            event_name < DS_SYS_EVENT_COMMON_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg_ex(): tech_all event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      }

      default:
      {     
        LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg_ex(): Invalid tech %d",
                                tech_type);
        goto bail;
      }
    }
  }
  
  if (subscription_id >= DS_SYS_SUBS_MAX || 
      subscription_id < DS_SYS_DEFAULT_SUBS)
  {
    LOG_MSG_INVALID_INPUT_1("ds_sys_event_reg_ex(): Invalid subscription_id %d",
                            subscription_id);
    goto bail;
  }  
  
  if (DS_SYS_DEFAULT_SUBS == subscription_id )
  {
    subscription_id = (ds_sys_subscription_enum_type)ps_sys_get_default_data_subscription();
  }
  
  ret = ps_sys_event_dereg_ex((ps_sys_tech_enum_type)tech_type,
                              (ps_sys_event_enum_type)event_name,
                              (ps_sys_subscription_enum_type)subscription_id,
                              (ps_sys_event_cback_f_ptr_ex_type) event_cback_f_ptr, 
                              dss_errno);
  return ret;

bail:
  *dss_errno = DS_EFAULT;
  return -1;
} /* ds_sys_event_dereg_ex() */

int16 ds_sys_event_dereg_ex2
(
  ds_sys_tech_enum_type                  tech_type,
  ds_sys_event_enum_type                 event_name,
  ds_sys_subscription_enum_type          subscription_id,
  ds_sys_event_cback_f_ptr_ex_type       event_cback_f_ptr,
  void                                 * client_data_ptr,
  int16                                * dss_errno
)
{
  uint16 ret;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3("ds_sys_event_dereg_ex2(): event %d tech %d cb 0x%p",
                           event_name, tech_type, event_cback_f_ptr);


  if (subscription_id >= DS_SYS_SUBS_MAX || 
      subscription_id < DS_SYS_DEFAULT_SUBS)
  {
    LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg_ex2(): Invalid subscription_id %d",
                            subscription_id);
    goto bail;
  }                           

  if (DS_SYS_DEFAULT_SUBS == subscription_id )
  {
    subscription_id = (ds_sys_subscription_enum_type)ps_sys_get_default_data_subscription();
  }

  if (event_name < DS_SYS_EVENT_COMMON_MIN ||
     event_name >= DS_SYS_EVENT_COMMON_MAX)
  {
    switch (tech_type)
    {
      case DS_SYS_TECH_3GPP:
      {  
        if (event_name >= DS_SYS_EVENT_3GPP_MAX || 
            event_name < DS_SYS_EVENT_3GPP_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg_ex2(): 3gpp event_name %d",
                                  event_name);
          goto bail;
}
        break;
      }

      case DS_SYS_TECH_3GPP2:
      {  
        if (event_name >= DS_SYS_EVENT_3GPP2_MAX || 
            event_name < DS_SYS_EVENT_3GPP2_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg_ex2(): 3gpp2 event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      }  

      case DS_SYS_TECH_WLAN:
      {  
        if (event_name >= DS_SYS_EVENT_WLAN_MAX || 
            event_name < DS_SYS_EVENT_WLAN_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg_ex2(): WLAN event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      }

      case DS_SYS_TECH_ALL:
      {  
        if (event_name >= DS_SYS_EVENT_COMMON_MAX || 
            event_name < DS_SYS_EVENT_COMMON_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg_ex(): tech_all event_name %d",
                                  event_name);
          goto bail;
        }
        break;
      }

      default:
      {     
        LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg_ex(): Invalid tech %d",
                                tech_type);
        goto bail;
      }
    }
  }
  
  if (subscription_id >= DS_SYS_SUBS_MAX || 
      subscription_id < DS_SYS_DEFAULT_SUBS)
  {
    LOG_MSG_INVALID_INPUT_1("ds_sys_event_dereg_ex2(): Invalid subscription_id %d",
                            subscription_id);
    goto bail;
  }  
  
  if (DS_SYS_DEFAULT_SUBS == subscription_id )
  {
    subscription_id = (ds_sys_subscription_enum_type)ps_sys_get_default_data_subscription();
  }
  
  ret = ps_sys_event_dereg_ex2((ps_sys_tech_enum_type)tech_type,
                              (ps_sys_event_enum_type)event_name,
                              (ps_sys_subscription_enum_type)subscription_id,
                              (ps_sys_event_cback_f_ptr_ex_type) event_cback_f_ptr,
                              client_data_ptr,
                              dss_errno);
  return ret;

bail:
  *dss_errno = DS_EFAULT;
  return -1;
} /* ds_sys_event_dereg_ex2() */
