/******************************************************************************
  @file    ds_sys_conf.c
  @brief   Data Service System Configuration implementation

  DESCRIPTION
  This file contains implementation of Data Service System Configuration API.

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
 
  $Header: //components/rel/data.mpss/3.5..2.1/interface/sysapi/src/ds_sys_conf.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $ 

  
when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/31/11   bvd     Created module
===========================================================================*/

/*---------------------------------------------------------------------------
                           INCLUDE FILES
---------------------------------------------------------------------------*/
#include "ds_sysi.h"
#include "ds_sys_conf.h"
#include "ps_sys_conf.h"
#include "dserrno.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_utils.h"

int16 ds_sys_conf_get
(
  ds_sys_tech_enum_type              tech_type,
  ds_sys_conf_enum_type              conf_name,
  void                             * argval_ptr,
  int16                            * dss_errno
)
{
  return ds_sys_conf_get_ex(tech_type,
                            conf_name,
                            DS_SYS_PRIMARY_SUBS,
                            argval_ptr,
                            dss_errno);
} /* ds_sys_conf_get() */

int16 ds_sys_conf_get_ex
(
  ds_sys_tech_enum_type              tech_type,
  ds_sys_conf_enum_type              conf_name,
  ds_sys_subscription_enum_type      subscription_id,
  void                             * argval_ptr,
  int16                            * dss_errno
)
{
  uint16 ret;
  ds_local_error_e_type        local_error = ERR_NO_ERROR;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_QTRACE_HFT_MED(PS_TAG_SYSAPI,(ps_sys_subscription_enum_type)subscription_id,
                 "ds_sys_conf_get_ex(): conf_name:%d, argval_ptr:0x%x subscription_id %d",
                 conf_name, argval_ptr, subscription_id);

  do
  {
    if (TRUE == ds_sysi_is_modem_reset_in_progress())
    {
      *dss_errno = DS_EINVAL;
      local_error = ERR_MODEM_SSR_IN_PROGRESS;
      break;
    }

    if (dss_errno == NULL)
    {
      local_error = ERR_INVALID_ERRNO;
      break;
    }

    if (tech_type >= DS_SYS_TECH_MAX || conf_name >= DS_SYS_CONF_MAX ||
        argval_ptr == NULL)
    {
      *dss_errno = DS_EFAULT;
      local_error = ERR_INVALID_PARAMS;
      break;
    }
  
    if (subscription_id >= DS_SYS_SUBS_MAX || 
        subscription_id < DS_SYS_DEFAULT_SUBS)
    {
      *dss_errno = DS_EFAULT;
      local_error = ERR_INVALID_SUBS;
      break;
    }  
  }while(0);

  if(local_error != ERR_NO_ERROR)
  {
    LOG_MSG_ERROR_1("ds_sys_conf_get_ex(): Error %d", local_error);
    return -1;
  }

  if (DS_SYS_DEFAULT_SUBS == subscription_id )
  {
    subscription_id = (ds_sys_subscription_enum_type)ps_sys_get_default_data_subscription();
  }

  ret = ps_sys_conf_get_ex((ps_sys_tech_enum_type)tech_type, 
                           (ps_sys_conf_enum_type)conf_name, 
                           (ps_sys_subscription_enum_type)subscription_id,
                           argval_ptr, 
                           dss_errno);

  return ret;
} /* ds_sys_conf_get_ex() */

int16 ds_sys_conf_set
(
  ds_sys_tech_enum_type              tech_type,
  ds_sys_conf_enum_type              conf_name,
  void                             * argval_ptr,
  int16                            * dss_errno
)
{

  return ds_sys_conf_set_ex(tech_type,
                            conf_name,
                            DS_SYS_PRIMARY_SUBS,
                            argval_ptr,
                            dss_errno);
} /* ds_sys_conf_set() */

int16 ds_sys_conf_set_ex
(
  ds_sys_tech_enum_type              tech_type,
  ds_sys_conf_enum_type              conf_name,
  ds_sys_subscription_enum_type      subscription_id,
  void                             * argval_ptr,
  int16                            * dss_errno
)
{
  uint16 ret;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3("ds_sys_conf_set_ex(): tech %d, conf %d  arg 0x%p",
                           tech_type, conf_name, argval_ptr);

  if (dss_errno == NULL)
  {
    LOG_MSG_INVALID_INPUT_0("ds_sys_conf_set_ex(): Invalid parameter dss_errno");
    return -1;
  }

  if (tech_type >= DS_SYS_TECH_MAX || conf_name >= DS_SYS_CONF_MAX ||
     argval_ptr == NULL)
  {
    *dss_errno = DS_EFAULT;
    LOG_MSG_INVALID_INPUT_0("ds_sys_conf_set_ex(): Invalid parameter passed");
    return -1;
  }

  if (subscription_id >= DS_SYS_SUBS_MAX || 
      subscription_id < DS_SYS_DEFAULT_SUBS)
  {
    *dss_errno = DS_EFAULT;
    LOG_MSG_INVALID_INPUT_1("ds_sys_conf_set_ex(): Invalid subscription_id %d",
                            subscription_id);
    return -1;
  }

  if (DS_SYS_DEFAULT_SUBS == subscription_id )
  {
    subscription_id = (ds_sys_subscription_enum_type)ps_sys_get_default_data_subscription();
  }

  if ((conf_name == DS_SYS_CONF_3GPP_ROAMING_DISALLOWED_INFO) ||
      (conf_name == DS_SYS_CONF_SYSTEM_STATUS) ||
      (conf_name == DS_SYS_CONF_SYSTEM_STATUS_EX) ||
      (conf_name == DS_SYS_CONF_GET_WLAN_IP_ADDR) ||
      (conf_name == DS_SYS_CONF_ROAMING_STATUS_INFO))
  {
    *dss_errno = DS_EFAULT;
    LOG_MSG_INVALID_INPUT_1("ds_sys_conf_set_ex(): Set operation for conf %d"
                            "is not supported",
                            conf_name);
    return -1;
  }

  ret = ps_sys_conf_set_ex((ps_sys_tech_enum_type)tech_type, 
                           (ps_sys_conf_enum_type)conf_name, 
                           (ps_sys_subscription_enum_type)subscription_id,
                           argval_ptr, 
                           dss_errno);
  return ret;
} /* ds_sys_conf_set_ex() */
