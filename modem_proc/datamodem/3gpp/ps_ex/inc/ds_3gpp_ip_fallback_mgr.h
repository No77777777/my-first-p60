#ifndef DS_3GPP_IP_FALLBACK_MGR_H
#define DS_3GPP_IP_FALLBACK_MGR_H
/*===========================================================================

 3 G   D A T A   S E R V I C E S   C O N F I G U R A T I O N   M A N A G E R

                           H E A D E R   F I L E

GENERAL DESCRIPTION
  This software unit contains functions for 3G Data Services event handling.
  It provides APIs for DS3G event registration and notification.

  Copyright (c) 2001-2015 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:
  $Header:

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
10/20/17   rs      Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "ds3gsubsmgr.h"
#include "sys_v.h"
#include "mcfg_fs.h"
#include "ps_iface_defs.h"
#include "list.h"
#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_config_mgr.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

=============================================================================*/

/*-----------------------------------------------------------------------
  Structure that is used to cache all the necessary information to
  bring-up an iface when IP fallback is enabled
-----------------------------------------------------------------------*/
typedef struct
{
  list_link_type              link;
  ds_pdn_context_s           *pdn_cntxt_ptr; 
  ds_3gpp_iface_s            *ds3gpp_iface_ptr;  
}ds_3gpp_ip_fallback_iface_cache_link_type;

/*===========================================================================
FUNCTION      DS_3GPP_IP_FALLBACK_IS_FALLBACK_CONFIGURED

DESCRIPTION 
  This function checks if IP fallback logic can be applied 

PARAMETERS   
  subs_idex    -- Subscription Index
   
DEPENDENCIES 
  None
  
RETURN VALUE 
  TRUE   -- If fallback logic can be applied
  FALSE  -- otherwise
   
SIDE EFFECTS 
  None

===========================================================================*/
boolean ds_3gpp_ip_fallback_is_ip_fallback_active
(
  sys_modem_as_id_e_type            subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_IP_FALLBACK_GET_IFACE_TYPE_TO_BRINGUP

DESCRIPTION 
  This function is used to decide which iface (IPv4 or IPv6) needs to be
  selected to be sent over the air during PDN connection request.
  The function takes care of cleaning up v4 iface information from PDN
  context and also updates APN table with the appropriate IP support
  information when the roaming PDP type in the profile is configured as IPv4v6

PARAMETERS 
  iface_ptr    -- Pointer to an iface pointer that is being brought up
  subs_idex    -- Subscription Index
   
DEPENDENCIES 
  None
  
RETURN VALUE 
  iface_ptr    -- Pointer to the iface that needs to be brought up.
  TRUE         -- Iface info changed. Call bring-up.
  FALSE        -- Iface info was not changed. Don't call bring-up again
   
SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_3gpp_ip_fallback_get_iface_type_to_bringup
(
  ps_iface_type                    **iface_ptr,
  sys_modem_as_id_e_type            subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_IP_FALLBACK_BIND_CACHED_IFACE_WITH_PDN_CNTXT

DESCRIPTION 
  This function updates pdn context table with the iface information
  to be brought up eventually.

PARAMETERS 
  pdn_cntx_ptr        -- pointer to PDN context that needs to be updated.
  fallback_info_ptr   -- pointer to the cached fallback information.
  down_reason         -- Reason for PDN teardown
  subs_id             -- Subscription Index
 
DEPENDENCIES 
  None
  
RETURN VALUE 
  TRUE   -- If Iface binding is successful
  FALSE  -- otherwise
   
SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_3gpp_ip_fallback_bind_cached_iface_with_pdn_cntxt
(
  ds_pdn_context_s                            *pdn_cntx_ptr,
  ds_3gpp_ip_fallback_iface_cache_link_type   *fallback_info_ptr,
  ps_iface_net_down_reason_type                down_reason,
  sys_modem_as_id_e_type                       subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_IP_FALLBACK_CLEAR_CACHED_CLIENT_CALL

DESCRIPTION 
  This function sends call failure to external clients on the iface which is
  cached in the IP fallback DB. It also clears IP fallback cache.

PARAMETERS 
  pdn_cntx_ptr       -- pointer to PDN context that needs to be updated.
  netdown_reason     -- reason for the call going down
  subs_id            -- Subscription Index
 
DEPENDENCIES 
  None
  
RETURN VALUE 
  None
   
SIDE EFFECTS 
  None
===========================================================================*/
void ds_3gpp_ip_fallback_clear_cached_client_call
(
  ds_pdn_context_s                            *pdn_cntx_ptr,
  ps_iface_net_down_reason_type                netdown_reason,
  sys_modem_as_id_e_type                       subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_IP_FALLBACK_GET_CACHED_IFACE

DESCRIPTION 
  This function checks IP fallback DB to see if there is any any iface
  cached for a particular PDN context. If present, it populates the IP
  fallback information to the caller.
   
PARAMETERS 
  pdn_cntx_ptr       -- Ptr to the PDN context tbl
  fallback_info_ptr  -- Ptr to fallback info that needs to be populated
  subs_id            -- Subscription Index
 
DEPENDENCIES 
  None
  
RETURN VALUE 
  TRUE   -- If an iface is cached for a PDN context
  FALSE  -- Otherwise
   
SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_3gpp_ip_fallback_get_cached_iface
(
  ds_pdn_context_s                            *pdn_cntx_ptr,  
  ds_3gpp_ip_fallback_iface_cache_link_type   *fallback_info_ptr,
  sys_modem_as_id_e_type                       subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_IP_FALLBACK_RESET_FALLBACK_INFO

DESCRIPTION 
  This function takes care of resetting IP fallback logic in a particular
  reset scenario if the corresponding reset condition is configured in the
  XML file.

PARAMETERS 
  reset_value     -- reset condition
  subs_id         -- Subscription Index
 
DEPENDENCIES 
  None
  
RETURN VALUE 
  None
   
SIDE EFFECTS 
  None
===========================================================================*/
void ds_3gpp_ip_fallback_reset_fallback_info
(
  reset_condition_enum_type     reset_value,
  sys_modem_as_id_e_type        subs_id  
);

/*===========================================================================
FUNCTION      DS_3GPP_CONFIG_MGR_INIT_DATA_3GPP_CFG_PER_SUB

DESCRIPTION 
  This function takes care of freeing memory associated with data 3GPP config
  DB

PARAMETERS 
  subs_id           -- Subscription Index
  
DEPENDENCIES 
  None
  
RETURN VALUE 
  None
   
SIDE EFFECTS 
  None

===========================================================================*/
void ds_3gpp_config_mgr_init_data_3gpp_cfg_per_sub
(
  sys_modem_as_id_e_type     subs_id 
);

/*===========================================================================
FUNCTION      DS_3GPP_IP_FALLBACK_IFACE_OUT_OF_USE_CMD

DESCRIPTION 
  This function checks the IP fallback DB to see if the iface going out of
  use is present in the IP fallback DB. If the iface is present then we clear
  the cache and post iface down indication.

PARAMETERS 
  reset_value     -- reset condition
  subs_id         -- Subscription Index
 
DEPENDENCIES 
  None
  
RETURN VALUE 
  TRUE   -- If there was a cached iface and it's cleaned up
  FALSE  -- otherwise
   
SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_3gpp_ip_fallback_iface_out_of_use_cmd
(
  ps_iface_type                   *iface_ptr,
  sys_modem_as_id_e_type           subs_id  
);

/*===========================================================================
FUNCTION      DS_3GPP_IP_FALLBACK_DISSOCIATE_DS_IFACE

DESCRIPTION 
  This function is used to remove DS iface information from PDN context table.
  This function will be called when the IP type requested by UE is rejected
  by network due to any of the fallback cause codes and there is a cached
  iface in the IP fallback DB

PARAMETERS  
  pdn_cntx_ptr    -- Pointer to PDN context
  ip_type         -- IP type of the DS iface that needs to be dissociated
  subs_id         -- Subscription Index
 
DEPENDENCIES 
  None
  
RETURN VALUE 
  TRUE    -- If successful
  FALSE   -- otherwise
   
SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_3gpp_ip_fallback_dissociate_ds_iface
(
  ds_pdn_context_s                *pdn_cntx_ptr,
  ds_apn_ip_support_type_e         ip_type,
  sys_modem_as_id_e_type           subs_id  
);

/*===========================================================================
FUNCTION      DS_3GPP_IP_FALLBACK_ACTIVE_TEARDOWN_CMD

DESCRIPTION 
  This function is used to teardown an iface.

PARAMETERS  
  ps_iface_ptr    -- Pointer to the iface being torn down  
 
DEPENDENCIES 
  None
  
RETURN VALUE 
  None
   
SIDE EFFECTS 
  None
===========================================================================*/
void ds_3gpp_ip_fallback_active_teardown_cmd
(
  ps_iface_type                   *ps_iface_ptr
);

/*===========================================================================
FUNCTION      DS_3GPP_IP_FALLBACK_IS_IFACE_CACHED

DESCRIPTION 
  This function is used to check if the given iface is cached in the
  fallback DB

PARAMETERS  
  ps_iface_ptr    -- Pointer to the iface being torn down  
 
DEPENDENCIES 
  None
  
RETURN VALUE 
  TRUE  -- If the ps_iface is cached in the db
  FALSE -- otherwise
   
SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_3gpp_ip_fallback_is_iface_cached
(
  ds_3gpp_iface_s   		      *ds3gpp_iface_ptr,
  sys_modem_as_id_e_type               subs_id  
);

/*===========================================================================
FUNCTION      DS_3GPP_IP_FALLBACK_UPDATE_PDN_CNTXT_INFO

DESCRIPTION 
  This function is used to update PDN context table and APN IP support
  information when profile PDP type is IPv4v6 and UE requested IPV6 over the
  air

PARAMETERS  
  
 
DEPENDENCIES 
  None
  
RETURN VALUE 
  TRUE  -- If updation was succesful or updation was not needed
  FALSE -- otherwise
   
SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_3gpp_ip_fallback_update_pdn_cntxt_info
(
  ds_pdn_context_s                   *pdn_cntx_ptr,
  sys_modem_as_id_e_type              subs_id  
);

#endif /* DS_3GPP_IP_FALLBACK_MGR_H */
