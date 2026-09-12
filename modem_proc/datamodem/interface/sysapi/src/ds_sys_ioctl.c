/******************************************************************************
  @file    ds_sys_ioctl.c
  @brief   

  DESCRIPTION
  This file defines functions, enums and structs used for two modehandlers
  to communicate with each other through ioctls

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2011 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.
 
  $Header: //components/rel/data.mpss/3.5..2.1/interface/sysapi/src/ds_sys_ioctl.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $ 
  
when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/31/11   bvd     Created module
===========================================================================*/

/*---------------------------------------------------------------------------
                           INCLUDE FILES
---------------------------------------------------------------------------*/
#include "comdef.h"
#include "queue.h"
#include "dserrno.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_sys_ioctl.h"

int16 ds_sys_ioctl
(
  ds_sys_tech_enum_type           tech_type,
  ds_sys_ioctl_enum_type          ioctl_name,
  void                          * arg_val_ptr,
  int16                         * dss_errno
)
{
  return ds_sys_ioctl_ex (tech_type,
                          ioctl_name,
                          DS_SYS_DEFAULT_SUBS,
                          arg_val_ptr,
                          dss_errno);
}

int16 ds_sys_ioctl_ex
(
  ds_sys_tech_enum_type           tech_type,
  ds_sys_ioctl_enum_type          ioctl_name,
  ds_sys_subscription_enum_type   subscription_id,
  void                          * arg_val_ptr,
  int16                         * dss_errno
)
{
  int16 result;
  ps_sys_3gpp_lte_attach_pdn_list_type  ps_attach_pdn_list;
  ds_sys_3gpp_lte_attach_pdn_list_type  *ds_attach_pdn_list_ptr = NULL;
  ps_sys_3gpp_lte_attach_pdn_list_type  *ps_attach_pdn_list_ptr = NULL;
  ds_sys_3gpp_lte_attach_pdn_list_type  ds_attach_pdn_list;
  ps_sys_3gpp_get_lte_attach_pdn_list_ex_type  ps_attach_pdn_list_ex;
  ps_sys_3gpp_get_lte_attach_pdn_list_ex_type  *ps_attach_pdn_list_ex_ptr = NULL;
  ds_sys_3gpp_get_lte_attach_pdn_list_ex_type  ds_attach_pdn_list_ex;
  int                                   pdn_cnt = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3("ds_sys_ioctl_ex(): tech %d ioctl %d arg_val 0x%p",
                           tech_type, ioctl_name, arg_val_ptr);

  memset(&ds_attach_pdn_list, 0 , sizeof(ds_attach_pdn_list));
  memset(&ps_attach_pdn_list, 0 , sizeof(ps_attach_pdn_list));
  memset(&ps_attach_pdn_list_ex, 0, sizeof(ps_attach_pdn_list_ex));
  memset(&ds_attach_pdn_list_ex, 0, sizeof(ds_attach_pdn_list_ex));

  if (subscription_id >= DS_SYS_SUBS_MAX || 
      subscription_id < DS_SYS_DEFAULT_SUBS)
  {
    *dss_errno = DS_EFAULT;
    LOG_MSG_INVALID_INPUT_1("ds_sys_ioctl_ex(): Invalid subscription_id %d",
                           subscription_id);
    return -1;
  }                           

  if (DS_SYS_DEFAULT_SUBS == subscription_id )
  {
    subscription_id = (ds_sys_subscription_enum_type)ps_sys_get_default_data_subscription();
  }
  
  if (ioctl_name < DS_SYS_IOCTL_COMMON_MIN ||
      ioctl_name >= DS_SYS_IOCTL_COMMON_MAX)
  {
    switch (tech_type)
    {
      case DS_SYS_TECH_3GPP:
      {  
        if (ioctl_name >= DS_SYS_IOCTL_3GPP_MAX || 
            ioctl_name < DS_SYS_IOCTL_3GPP_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_ioctl_ex(): 3gpp ioctl %d", ioctl_name);
          goto bail;
        }
        break;
      }

      case DS_SYS_TECH_3GPP2:
      {  
        if (ioctl_name >= DS_SYS_IOCTL_3GPP2_MAX || 
            ioctl_name < DS_SYS_IOCTL_3GPP2_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_ioctl_ex(): 3gpp2 ioctl %d",
                                  ioctl_name);
          goto bail;
        }
        break;
      }  
   
      case DS_SYS_TECH_WLAN:
      {
        if (ioctl_name >= DS_SYS_IOCTL_WLAN_MAX ||
            ioctl_name < DS_SYS_IOCTL_WLAN_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_ioctl_ex(): wlan ioctl %d",
                                  ioctl_name);
          goto bail;
        }
        break;
      }

      case DS_SYS_TECH_ALL:
      {  
        if (ioctl_name >= DS_SYS_IOCTL_COMMON_MAX || 
            ioctl_name < DS_SYS_IOCTL_COMMON_MIN)
        {
          LOG_MSG_INVALID_INPUT_1("ds_sys_ioctl_ex(): comm ioctl %d", ioctl_name);
          goto bail;
        }
        break;
      }

      default:
      {     
        LOG_MSG_INVALID_INPUT_1("ds_sys_ioctl_ex(): tech %d", tech_type);
        goto bail;
      }
    }
  }

  /*-------------------------------------------------------------------------
    For some ds sys ioctls, need to convert DS sys specific payload to PS sys 
    payload because they have different structure definitions in PS and DS layers.
  -----------------------------------------------------------------------------*/
  switch(ioctl_name)
  {
    case DS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST:
    {
      ds_attach_pdn_list_ptr = (ds_sys_3gpp_lte_attach_pdn_list_type* )arg_val_ptr;
      ps_attach_pdn_list.attach_pdn_profile_list_num = ds_attach_pdn_list_ptr->attach_pdn_profile_list_num;
      for (pdn_cnt=0; pdn_cnt < ds_attach_pdn_list_ptr->attach_pdn_profile_list_num; pdn_cnt++)
      {
        ps_attach_pdn_list.attach_pdn_profile_list[pdn_cnt] = 
                ds_attach_pdn_list_ptr->attach_pdn_profile_list[pdn_cnt];
      }
      arg_val_ptr = &ps_attach_pdn_list;
      break;
    }
    case DS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST:
    {
      arg_val_ptr = &ps_attach_pdn_list;
      break;
    }
    case DS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST_EX:
    {
      arg_val_ptr = &ps_attach_pdn_list_ex;
      break;
  }
    default:
      break;
  }

  result =  ps_sys_ioctl_ex((ps_sys_tech_enum_type)tech_type,
                         (ps_sys_ioctl_enum_type)ioctl_name,
                         (ps_sys_subscription_enum_type)subscription_id,
                         arg_val_ptr,
                         dss_errno);

  /*-------------------------------------------------------------------------
    Convert PS sys payload back to DS sys payload because DS and PS have 
    different structure definition. This conversion is needed only for GET operation.
  -----------------------------------------------------------------------------*/
  switch(ioctl_name)
  {
    case DS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST:
    {

      ps_attach_pdn_list_ptr = (ps_sys_3gpp_lte_attach_pdn_list_type* )arg_val_ptr;
      ds_attach_pdn_list.attach_pdn_profile_list_num = ps_attach_pdn_list_ptr->attach_pdn_profile_list_num;
      for (pdn_cnt=0; pdn_cnt < ps_attach_pdn_list_ptr->attach_pdn_profile_list_num; pdn_cnt++)
      {
        ds_attach_pdn_list.attach_pdn_profile_list[pdn_cnt] = 
                 ps_attach_pdn_list_ptr->attach_pdn_profile_list[pdn_cnt];
      }
      arg_val_ptr = &ds_attach_pdn_list;
      break;
    }

    case DS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST_EX:
    {  
       ps_attach_pdn_list_ex_ptr = (ps_sys_3gpp_get_lte_attach_pdn_list_ex_type *)arg_val_ptr; 
       memscpy(&ds_attach_pdn_list_ex.pending_attach_pdn_list,
               sizeof(ds_sys_3gpp_lte_attach_pdn_list_type),
               &ps_attach_pdn_list_ex_ptr->pending_attach_pdn_list,
               sizeof(ps_sys_3gpp_lte_attach_pdn_list_type));

       memscpy(&ds_attach_pdn_list_ex.active_attach_pdn_list,
               sizeof(ds_sys_3gpp_lte_attach_pdn_list_type),
               &ps_attach_pdn_list_ex_ptr->active_attach_pdn_list,
               sizeof(ps_sys_3gpp_lte_attach_pdn_list_type));

       arg_val_ptr = &ds_attach_pdn_list_ex;
       break;
    }

    default:
      break;
  }
  return result;

bail:
  *dss_errno = DS_EFAULT;
  return -1;
}
