/*===========================================================================

                             D S 3 G U T I L . C
 
GENERAL DESCRIPTION
  Utility functions for DSMgr module

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2008 - 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/dsmgr/src/ds3gutil.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/01/16   ds      Removed logic to read NV 1896 for IPv6 and setting it to default.
04/24/15   sd      Fixed subs_id handling for 3GPP2 in DSDA mode.
02/10/15   sd      Added dial string validation utility function in DS3G.
07/13/12   jz      Support for emergency only mode when mandatory APN is disabled  
01/25/12   nd      Deprecating API cm_get_pref_srv_type_for_packet_orig usage.
01/02/12   msh     Coan: Feature cleanup
11/28/11   jee     Fix to stay in partial context after handoff to LTE
03/29/11   ttv     Merged changes to ensure that WLAN was selected as pref
                   mode in its presence in DSDS enabled builds.
03/04/11   ttv     Added changes to get the correct sys mdoe incase of dualsim.
11/22/10   ttv     Added support for DSDS.
09/09/10   op      Migrated to MSG 2.0 macros
09/22/09   vrk     Merged LTE related changes.
03/04/09   sa      AU level CMI modifications.
11/26/08   sn      Fixed off-target lint errors.
07/15/08   ar      Initial version.  Partitioned from ds3gmgr.c

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include <stringl/stringl.h>

#ifdef  FEATURE_DATA_LTE_OFFTARGET_TEST 
#include "dsut.h"
#endif /* FEATURE_DATA_LTE_OFFTARGET_TEST */
#include "cm.h"
#include "ds3gcfgmgr.h"
#include "ds3gmgr.h"
#include "ds3gsubsmgr.h"
#include "ds3gmgrint.h"
#include "err.h"
#include "msg.h"
#include "rex.h"
#include "sys.h"
#include "data_msg.h"
#include "dsutil.h"
#include "dstaski.h"
#include "ds3gutil.h"
#include "ds3g_ext.h"
#include "ds3gcommonutil.h"
#include "modem_mem.h"
#include "ds_3gpp_profile_api.h"

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/*===========================================================================

                      INTERNAL FUNCTION DEFINITIONS

==========================================================================*/

#ifdef FEATURE_DATA_PS_IPV6
/*===========================================================================
FUNCTION      DS3G_READ_IPV6_EFS_NV_PER_SUBS_ID

DESCRIPTION   Read & store common IPv6 efs items used by 3gpp & 3gpp2 MHs 
              per subscription

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_read_ipv6_efs_nv_per_subs_id
(
  sys_modem_as_id_e_type  subs_id
)
{
  nv_item_type                ds_nv_item;
  nv_stat_enum_type           status;
  ds3gsubsmgr_subs_id_e_type  subs_indx;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return;
  }
  subs_indx = ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id);

  /*-------------------------------------------------------------------------
    Enable IPv6 irrespective of value of NV 1896 defined in EFS.
  -------------------------------------------------------------------------*/
  ipv6_nv_efs_info.is_enabled = IPV6_ENABLED_DEFAULT;

  if( ipv6_nv_efs_info.subs_info[subs_indx] == NULL )
  {
    ipv6_nv_efs_info.subs_info[subs_indx] =
      (ds3g_ipv6_efs_nv_subs_specific_info*)
        modem_mem_alloc (sizeof(ds3g_ipv6_efs_nv_subs_specific_info),
                         MODEM_MEM_CLIENT_DATA);

    if( ipv6_nv_efs_info.subs_info[subs_indx] != NULL)
    {
      DATA_3GMGR_MSG1(MSG_LEGACY_LOW, "Ipv6 nv info subs ID allocated %d",
                      subs_id);
    }
    else
    {
      DS_3GMGR_ERROR_LOG_1_EX(MSG_LEGACY_ERROR, subs_id,
                      "Cannot allocate memory for ipv6 NV for subs_id:d",
                      subs_id);
      return;
    }
  }

  /*-----------------------------------------------------------------------
    Initialize all subscription specific NV values to zero
  -----------------------------------------------------------------------*/
  memset(ipv6_nv_efs_info.subs_info[subs_indx],
         0,
         sizeof(ds3g_ipv6_efs_nv_subs_specific_info));

  /*-----------------------------------------------------------------------
    Read the IP6 State Machine config info
  -----------------------------------------------------------------------*/
  status = ds3gcfgmgr_read_legacy_nv_ex( NV_IPV6_SM_CONFIG_I,
                                         &ds_nv_item,
                                         subs_id);
  if (NV_DONE_S != status)
  {
    ds_nv_item.ipv6_sm_config.init_sol_delay   =
                                          IP6_SM_DEFAULT_INIT_SOL_DELAY;
    ds_nv_item.ipv6_sm_config.sol_interval     =
                                            IP6_SM_DEFAULT_SOL_INTERVAL;
    ds_nv_item.ipv6_sm_config.resol_interval   =
                                          IP6_SM_DEFAULT_RESOL_INTERVAL;
    ds_nv_item.ipv6_sm_config.max_sol_attempts =
                                        IP6_SM_DEFAULT_MAX_SOL_ATTEMPTS;
    ds_nv_item.ipv6_sm_config.max_resol_attempts =
                                      IP6_SM_DEFAULT_MAX_RESOL_ATTEMPTS;
    ds_nv_item.ipv6_sm_config.pre_ra_exp_resol_time =
                                   IP6_SM_DEFAULT_PRE_RA_EXP_RESOL_TIME;
  }

  ipv6_nv_efs_info.subs_info[subs_indx]->sm_config.init_sol_delay =
                    ((ds_nv_item.ipv6_sm_config.init_sol_delay > 0) ?
                      ds_nv_item.ipv6_sm_config.init_sol_delay :
                      IP6_SM_DEFAULT_INIT_SOL_DELAY
                    );
  
  ipv6_nv_efs_info.subs_info[subs_indx]->sm_config.sol_interval =
                    ((ds_nv_item.ipv6_sm_config.sol_interval > 0) ?
                      ds_nv_item.ipv6_sm_config.sol_interval :
                      IP6_SM_DEFAULT_SOL_INTERVAL
                    );

  ipv6_nv_efs_info.subs_info[subs_indx]->sm_config.resol_interval =
                    ((ds_nv_item.ipv6_sm_config.resol_interval > 0) ?
                      ds_nv_item.ipv6_sm_config.resol_interval :
                      IP6_SM_DEFAULT_RESOL_INTERVAL
                    );

  ipv6_nv_efs_info.subs_info[subs_indx]->sm_config.max_sol_attempts =
                    ((ds_nv_item.ipv6_sm_config.max_sol_attempts > 0) ?
                      ds_nv_item.ipv6_sm_config.max_sol_attempts :
                      IP6_SM_DEFAULT_MAX_SOL_ATTEMPTS
                    );

  ipv6_nv_efs_info.subs_info[subs_indx]->sm_config.max_resol_attempts =
                    ((ds_nv_item.ipv6_sm_config.max_resol_attempts > 0) ?
                      ds_nv_item.ipv6_sm_config.max_resol_attempts :
                      IP6_SM_DEFAULT_MAX_RESOL_ATTEMPTS
                    );

  ipv6_nv_efs_info.subs_info[subs_indx]->sm_config.pre_ra_exp_resol_time =
                    ((ds_nv_item.ipv6_sm_config.pre_ra_exp_resol_time > 0) ?
                      ds_nv_item.ipv6_sm_config.pre_ra_exp_resol_time :
                      IP6_SM_DEFAULT_PRE_RA_EXP_RESOL_TIME
                    );

  status = ds3gcfgmgr_read_legacy_nv_ex( NV_PDSN_AS_PROXY_IPV6_DNS_SERVER_I,
                                         &ds_nv_item, subs_id );
  if (NV_DONE_S == status)
  {
    ipv6_nv_efs_info.subs_info[subs_indx]->pdsn_as_proxy_ipv6_dns_server
                                    = ds_nv_item.pdsn_as_proxy_ipv6_dns_server;
  }
  else
  {
    ipv6_nv_efs_info.subs_info[subs_indx]->pdsn_as_proxy_ipv6_dns_server 
      = FALSE;
  }

  /*-------------------------------------------------------------------------
    Read the values of DNS addrs from NV and store:
    Get Primary DNS IP addresses from NV.
    Make IP addresses as NULL if NV was never written.
  -------------------------------------------------------------------------*/
  status = ds3gcfgmgr_read_legacy_nv_ex( NV_IPV6_PRIMARY_DNS_I, &ds_nv_item,
                                         subs_id );
  if (NV_DONE_S == status)
  {
    /*-----------------------------------------------------------------------
      Store number retrieved from NV.
    -----------------------------------------------------------------------*/
    ipv6_nv_efs_info.subs_info[subs_indx]->primary_dns.in6_u.u6_addr64[0] =
      ds_nv_item.ipv6_primary_dns.prefix;
    ipv6_nv_efs_info.subs_info[subs_indx]->primary_dns.in6_u.u6_addr64[1] =
      ds_nv_item.ipv6_primary_dns.iid;
  }
  else
  {
    /* NV item not populated; use default */
  }

  /*-------------------------------------------------------------------------
    Get Secondary DNS IP addresses from NV.
    Make IP addresses as NULL if NV was never written.
  -------------------------------------------------------------------------*/
  status = ds3gcfgmgr_read_legacy_nv_ex( NV_IPV6_SECONDARY_DNS_I, &ds_nv_item,
                                         subs_id );
  if (NV_DONE_S == status)
  {
    /*-----------------------------------------------------------------------
      Store number retrieved from NV.
    -----------------------------------------------------------------------*/
    ipv6_nv_efs_info.subs_info[subs_indx]->secondary_dns.in6_u.u6_addr64[0] =
      ds_nv_item.ipv6_secondary_dns.prefix;
    ipv6_nv_efs_info.subs_info[subs_indx]->secondary_dns.in6_u.u6_addr64[1] =
      ds_nv_item.ipv6_secondary_dns.iid;
  }
  else
  {
    /* NV item not populated; use default */
  }

  /*-------------------------------------------------------------------------
    Read IPv6 failover config nv
  -------------------------------------------------------------------------*/
  status = ds3gcfgmgr_read_legacy_nv(NV_IPV6_FAILOVER_CONFIG_I, &ds_nv_item);
  if(NV_DONE_S == status)
  {
    ipv6_nv_efs_info.failover_mode = (ps_iface_ip_ver_failover_e_type)
                                      ds_nv_item.ipv6_failover_config;
  }
  else
  {
    ipv6_nv_efs_info.failover_mode = IPV6_PREFERRED;
  }

  DATA_3GMGR_MSG1(MSG_LEGACY_LOW, "failover_mode = %d",
                  ipv6_nv_efs_info.failover_mode);

  /*-------------------------------------------------------------------------
    Get IPv6 IID info from NV.
  -------------------------------------------------------------------------*/
  status = ds3gcfgmgr_read_legacy_nv_ex( NV_PS_IPV6_IID_I, &ds_nv_item,
                                         subs_id );
  if (NV_DONE_S == status)
  {
    /*-----------------------------------------------------------------------
      Store number retrieved from NV.
    -----------------------------------------------------------------------*/
    ipv6_nv_efs_info.subs_info[subs_indx]->iid_info.iid_enum_type =
      IPV6_USER_IID;
    ipv6_nv_efs_info.subs_info[subs_indx]->iid_info.iid_val =
      ds_nv_item.ps_ipv6_iid.iid_val;
  }
  else
  {
    /* NV item not populated; use default */
    ipv6_nv_efs_info.subs_info[subs_indx]->iid_info.iid_enum_type =
      IPV6_RANDOM_IID;
    ipv6_nv_efs_info.subs_info[subs_indx]->iid_info.iid_val = 0;
  }

  return;
} /* ds3g_read_ipv6_efs_items() */
#endif /* FEATURE_DATA_PS_IPV6 */

/*===========================================================================
FUNCTION      DS3G_SET_CM_OPRT_LPM_MODE

DESCRIPTION   Set CM operation mode to LPM

DEPENDENCIES  None

RETURN VALUE  TRUE: Successfully set 
              FALSE: Fail to set 

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_set_cm_oprt_lpm_mode( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DATA_3GMGR_MSG0(MSG_LEGACY_HIGH, "ds3g_set_cm_oprt_online_mode");
  return cm_ph_cmd_oprt_mode(NULL,
                             NULL,
                             ds3g_get_cm_client_id(),
                             SYS_OPRT_MODE_LPM);
} /* ds3g_set_cm_oprt_lpm_mode() */

/*===========================================================================
FUNCTION      DS3G_SET_CM_OPRT_ONLINE_MODE

DESCRIPTION   Set CM operation mode to online

DEPENDENCIES  None

RETURN VALUE  TRUE: Successfully set 
              FALSE: Fail to set 

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_set_cm_oprt_online_mode(void)
{
  boolean ret_val = TRUE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DATA_3GMGR_MSG0(MSG_LEGACY_HIGH, "ds3g_set_cm_oprt_online_mode");
  ret_val = cm_ph_cmd_oprt_mode(NULL,
                                NULL,
                                ds3g_get_cm_client_id(),
                                SYS_OPRT_MODE_ONLINE);
  if(cm_ph_cmd_get_ph_info(NULL, NULL, ds3g_get_cm_client_id()) != TRUE)
  {
  	DS_3GMGR_ERROR_LOG_0(MSG_LEGACY_ERROR,
                    "ds3g_set_cm_oprt_online_mode: Problem requesting initial "
                    "CM PH info ");
  	ret_val = FALSE;
  }
  return ret_val;
} /* ds3g_set_cm_oprt_online_mode() */

