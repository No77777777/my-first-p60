/******************************************************************************
  @file    ps_sys.c
  @brief   

  DESCRIPTION
  This file defines function, variables and data structures common to all
  PS System API module

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2011-2015 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/sysapi/src/ps_sys.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $
 
when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/31/11   bvd     Created module
===========================================================================*/

/*---------------------------------------------------------------------------
                           INCLUDE FILES
---------------------------------------------------------------------------*/
#include "ps_sys.h"
#include "ps_svc.h"
#include "ps_sys_eventi.h"
#include "ps_sys_confi.h"
#include "ps_crit_sect.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_sys_conf_parser.h"
#include "ps_sys_ioctl.h"
#include "ps_utils.h"
#include "ps_sysi_ca_cert.h"

ps_crit_sect_type ps_sys_crit_section;

/*Cache for storing DDS*/
static struct
{
  ps_sys_subscription_enum_type   default_data_subs;
} ps_sys_global = {0,};

/*Cache for storing temp DDS*/
static struct
{
  ps_sys_subscription_enum_type   temp_default_data_subs;
  ps_sys_dds_switch_type_enum     dds_switch_type;
} ps_sys_global_temp = {0,0};

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/** 
  @brief Callback function to be invoked when NV refresh event is received from 
        MCFG framework.
*/
void ps_sys_refresh_cb
(
  ps_utils_refresh_notify_info  ps_sys_refresh_info
)
{
  int16      ps_errno = DS_ENOERR;
  int16      return_val = DSS_SUCCESS;
/*-------------------------------------------------------------------------*/
  LOG_MSG_INFO1_1("ps_sys_refresh_cb(): refresh_type : %d",
                   ps_sys_refresh_info.refresh_type);

  /*------------------------------------------------------------------------
    Re-read the required NV items as NV refersh event is received. 
    For all the below mentioned 3 types, actions are the same for now.
  ------------------------------------------------------------------------*/  
  if( (PS_UTILS_REFRESH_TYPE_SLOT == ps_sys_refresh_info.refresh_type) ||
      (PS_UTILS_REFRESH_TYPE_SUBS == ps_sys_refresh_info.refresh_type) || 
      (PS_UTILS_REFRESH_TYPE_SLOT_N_SUBS == ps_sys_refresh_info.refresh_type) )
  {
    return_val = ps_sys_conf_set_ex(PS_SYS_TECH_ALL,
                       PS_SYS_CONF_APN_INFO_RESET,
                       ps_sys_refresh_info.refresh_subs,
                       NULL,
                       &ps_errno);
  }
  
  if ( return_val != 0)
  {
    LOG_MSG_ERROR_1("PS_SYS_CONF_APN_INFO_RESET failed, err_no %d", ps_errno);
  }
} /* ps_sys_refresh_cb() */
#endif /* FEATURE_MODEM_CONFIG_REFRESH */


void ps_sys_init
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Initialize global event queue
  -------------------------------------------------------------------------*/
  (void) q_init(&ps_sys_eventi_q);

  /*-------------------------------------------------------------------------
    Set the command handlers for sys event and ioct tech callbacks
  -------------------------------------------------------------------------*/
  (void) ps_set_cmd_handler(PS_SYS_EVENT_CMD_HANDLER,
                            ps_sys_eventi_cback_cmd_handler);
  
  /*-------------------------------------------------------------------------
    Allocate memory for temp storage for config data. It would be removed
    once xml implementation is done
  -------------------------------------------------------------------------*/
  ps_sys_confi_alloc_temp_data();
  /*-------------------------------------------------------------------------
    Read persistent configuration settings from EFS and update conf cache
  -------------------------------------------------------------------------*/
  ps_sys_conf_parser_update_cfg_cache();

  /*------------------------------------------------------------------------
    Initialize the global PS critical section
  -------------------------------------------------------------------------*/
  PS_INIT_CRIT_SECTION(&ps_sys_crit_section);

  /*-------------------------------------------------------------------------
    Register for the TECH_ALL ioctls that are handled in the framework
  -------------------------------------------------------------------------*/  
  ps_sys_ioctl_register_common_cbacks();

  /*-----------------------------------------------------------------------
  Initialize the queue which will hold all info of CA certificate clients 
  ------------------------------------------------------------------------*/
  ps_sysi_ca_cert_init();
}

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

ps_sys_tech_enum_type ps_sys_get_tech_from_rat
(
  ps_sys_rat_ex_enum_type  rat
)
{
  if( ( rat == PS_SYS_RAT_EX_3GPP_WLAN ) ||
      ( rat == PS_SYS_RAT_EX_3GPP2_WLAN ) ||
      (rat >= PS_SYS_RAT_EX_WLAN && rat < PS_SYS_RAT_EX_WLAN_MAX)
    )
  {
    return PS_SYS_TECH_WLAN;
  }

  if(rat >= PS_SYS_RAT_EX_3GPP_WCDMA && rat < PS_SYS_RAT_EX_3GPP_MAX)
    return PS_SYS_TECH_3GPP;
  
  if(rat >= PS_SYS_RAT_EX_3GPP2_1X && rat < PS_SYS_RAT_EX_3GPP2_MAX)
    return PS_SYS_TECH_3GPP2;

  return PS_SYS_TECH_ALL;     
} /* ps_sys_get_tech_from_rat */

/** 
  @brief This function sets the DDS(Default data subscription) 
         in the system. Only DS3g module sets this parameter.
*/
void ps_sys_set_default_data_subscription
(
  ps_sys_subscription_enum_type  def_data_subs
)
{
  ps_sys_current_dds_type  current_dds_info;
  int16                    ps_errno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(def_data_subs < PS_SYS_PRIMARY_SUBS ||
     def_data_subs >= PS_SYS_SUBS_MAX)
  {
    DS_UTILS_ASSERT(0);
  }

  LOG_MSG_INFO1_1("ps_sys_set_default_data_subscription() subs_id %d",
                  def_data_subs);

  memset(&current_dds_info, 0, sizeof(ps_sys_current_dds_type));
  
  PS_ENTER_CRIT_SECTION(&ps_sys_crit_section);
  if ( ps_sys_global.default_data_subs != def_data_subs)
  {
  ps_sys_global.default_data_subs = def_data_subs;
  
    current_dds_info.dds = ps_sys_global.default_data_subs;
    (void)ps_sys_event_ind_ex (PS_SYS_TECH_ALL, 
                               PS_SYS_EVENT_CURRENT_DDS_IND, 
                               ps_sys_global.default_data_subs, 
                               &current_dds_info,
                               &ps_errno);
  }
                                
  PS_LEAVE_CRIT_SECTION(&ps_sys_crit_section);
} /* ps_sys_set_default_data_subscription */

/** 
  @brief This function gets the DDS(Default data subscription) 
         in the system
*/
ps_sys_subscription_enum_type ps_sys_get_default_data_subscription
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION(&ps_sys_crit_section);
  if(PS_SYS_DEFAULT_SUBS == ps_sys_global.default_data_subs)
  {
    //Can happen only at boot up
    PS_LEAVE_CRIT_SECTION(&ps_sys_crit_section);
    return PS_SYS_PRIMARY_SUBS;
  }

  PS_LEAVE_CRIT_SECTION(&ps_sys_crit_section);
  return ps_sys_global.default_data_subs;
} /* ps_sys_get_default_data_subscription */

/** 
  @brief This function sets the Temp DDS(Default data subscription) 
         in the system. Only Ds3g module can set this value
*/
void ps_sys_set_temp_default_data_subscription
(
  ps_sys_subscription_enum_type  temp_def_data_subs,
  ps_sys_dds_switch_type_enum    dds_switch_type
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(temp_def_data_subs < PS_SYS_PRIMARY_SUBS ||
     temp_def_data_subs >= PS_SYS_SUBS_MAX)
  {
    ASSERT(0);
  }
  
  PS_ENTER_CRIT_SECTION(&ps_sys_crit_section);
  ps_sys_global_temp.temp_default_data_subs = temp_def_data_subs;
  ps_sys_global_temp.dds_switch_type = dds_switch_type;
  PS_LEAVE_CRIT_SECTION(&ps_sys_crit_section);
}

/** 
  @brief This function gets the temp DDS(Default data subscription) 
         in the system
*/
ps_sys_subscription_enum_type ps_sys_get_temp_default_data_subscription
(
  void
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION(&ps_sys_crit_section);
  if(PS_SYS_DEFAULT_SUBS == ps_sys_global_temp.temp_default_data_subs)
  {
    //Can happen only at boot up
    PS_LEAVE_CRIT_SECTION(&ps_sys_crit_section);
    return PS_SYS_PRIMARY_SUBS;
  }

  PS_LEAVE_CRIT_SECTION(&ps_sys_crit_section);
  return ps_sys_global_temp.temp_default_data_subs;
}

/** 
  @brief This function gets the DDS(Default data subscription) switch type
         in the system
*/
ps_sys_dds_switch_type_enum ps_sys_get_dds_switch_type
(
  void
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return ps_sys_global_temp.dds_switch_type;
}

/*===========================================================================
FUNCTION PS_SYS_NV_REFRESH_REG_INIT()

DESCRIPTION
  THIS FUNCTION REGISTER PS_SYS TO NV REFRESH EVENT

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
#ifdef FEATURE_MODEM_CONFIG_REFRESH  

void ps_sys_nv_refresh_reg_init
(
  void
)
{
  ps_utils_refresh_reg_info  ps_sys_reg_info;
  ps_utils_status_e_type     reg_status;

  /*------------------------------------------------------------------------
    NV refresh registration
  --------------------------------------------------------------------------*/
  ps_sys_reg_info.func_ptr = 
                 (ps_utils_refresh_notify_func_type) ps_sys_refresh_cb;
  ps_sys_reg_info.user_data_ptr = NULL;
 
  reg_status = ps_utils_refresh_reg(ps_sys_reg_info);
  if(PS_UTILS_REG_SUCCESS != reg_status)
  {
    LOG_MSG_ERROR_1("NV refresh registration failure status : %d", reg_status);
  }
}
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

