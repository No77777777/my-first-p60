#ifndef DS_3GPP_DEVICE_EV_HDLR_H
#define DS_3GPP_DEVICE_EV_HDLR_H
/*===========================================================================

            DS 3GPP DEVICE EV HDLR

===========================================================================*/
/*!
  @file
  ds_3gpp_roaming_hdlr.h

  @brief
  This header file externalizes 3GPP Device Handler functionality

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/
/*==============================================================================

  Copyright (c) 2008 - 2018 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

==============================================================================*/
/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_device_ev_hdlr.h#3 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
1/21/14    vs      Initial File
==============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"

#include "dstask_v.h"
#include "ds3gsubsmgr.h"
#include "ds3gdevmgr.h"
#include "ds_3gpp_kamgr_ext.h"
#include "ds_3gpp_pdn_context.h"

#include "cm_gw.h"
#include "ps_sys.h"
#include "ds_autoreg_ext_msg.h"
/*=============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

=============================================================================*/
typedef enum
{
  DS3GPP_DEVICE_EV_HDLR_PS_DATA_OFF_DATA_ON = 0,
  /**For the use case of when overall data is ON */ 
  DS3GPP_DEVICE_EV_HDLR_PS_DATA_OFF_NONE    = 1,
  /**For the use case of when feature is disabled */ 
  DS3GPP_DEVICE_EV_HDLR_PS_DATA_OFF_USIM    = 2,
  /**Overall data is OFF, and PS Data Off reads from USIM for 
   * exempt services */ 
  DS3GPP_DEVICE_EV_HDLR_PS_DATA_OFF_OMADM   = 3,
  /**Overall data is OFF, and PS Data Off reads from OMADM for 
   * exempt services */ 
  DS3GPP_DEVICE_EV_HDLR_PS_DATA_OFF_DEFAULT = 4,
  /**Overall data is OFF, and PS Data Off reads from XML for 
   * default exempt services */ 
  DS3GPP_DEVICE_EV_HDLR_PS_DATA_OFF_MAX
} ds3gpp_device_ev_hdlr_ps_data_off_source_type;

/*===========================================================================
FUNCTION      DS_3GPP_DEVICE_EV_HDLR_INIT

DESCRIPTION   This function initializes the device event handler module

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gpp_device_ev_hdlr_init
(
  void
);

/*===========================================================================
FUNCTION      DS_3GPP_DEVICE_EV_HDLR_INIT

DESCRIPTION   This function initializes device event handler parameters
              after power up is complete

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gpp_device_ev_hdlr_init_after_powerup
(
  void
);

/*===========================================================================
FUNCTION      DS_3GPP_DEVICE_EV_HDLR_PROCESS_DATA_SETTINGS

DESCRIPTION   This function processes the data settings

PARAMETERS    Device Settings
              Boolean Pointer set to True if the attach apn is unblocked

DEPENDENCIES  None

RETURN VALUE  TRUE, if Data is disabled after processing the settings
              FALSE, if Data is enabled

SIDE EFFECTS  None
===========================================================================*/
boolean ds3gpp_device_ev_hdlr_process_data_settings
(
  ds3gdevmgr_device_settings_info_type  *device_settings_p,
  boolean                               *is_attach_apn_ptr
);

/*===========================================================================
FUNCTION      DS_3GPP_DEVICE_EV_HDLR_IS_APN_ALLOWED

DESCRIPTION   This function checks whether PDN can be brought up for this APN

PARAMETERS    1. APN name
              2. Subs id

DEPENDENCIES  None

RETURN VALUE  TRUE, if PDN can be brought up
              FALSE, otherwise

SIDE EFFECTS  None
===========================================================================*/
boolean ds3gpp_device_ev_hdlr_is_apn_allowed
(
  char                           *apn_p,
  sys_modem_as_id_e_type          subs_id,
  ps_iface_net_down_reason_type  *down_reason_p
);

/*===========================================================================
FUNCTION DS_3GPP_DEVICE_EV_HDLR_UNBLOCK_THROTTLING

DESCRIPTION
  This function unblocks PDN throttling because UI Data was re-enabled.
  
PARAMETERS
  1. APN name
  2. Subscription Id
  3. Throttle Mask
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.
===========================================================================*/
void ds3gpp_device_ev_hdlr_unblock_throttling
(
  byte*                                apn,
  sys_modem_as_id_e_type               subs_id,
  uint32                               throttle_mask
);

/*===========================================================================
FUNCTION      DS_3GPP_DEVICE_EV_HDLR_ADVERTISE_THROTTLING_INFO

DESCRIPTION   This function is used to advertise throttling information

PARAMETERS    1. Status indicating data/ims enabled / disabled
              2. Subscription Id
              3. boolean- True if the attach apn is unblocked

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gpp_device_ev_hdlr_advertise_throttling_info
(
  boolean                 disable_lte,
  sys_modem_as_id_e_type  subs_id,
  boolean                 is_attach_apn
);

/*===========================================================================
FUNCTION   ds3gpp_device_ev_hdlr_is_ps_data_off_feature_enabled

DESCRIPTION
  This function checks all the criteria to determine whether or not the
  PS Data Off Feature should be enabled or not.

PARAMETERS
  Subscription ID.

DEPENDENCIES
  None.

RETURN VALUE
  True if PS Data Off feature is enabled
  False otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds3gpp_device_ev_hdlr_is_ps_data_off_feature_enabled
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION    DS3GPP_DEVICE_EV_HDLR_GET_PS_DATA_OFF_DATA_ON_STATUS

DESCRIPTION
  This function returns the overall data on status
 
PARAMETERS
  Subscription ID
  Data on status pointer to be returned to caller

DEPENDENCIES
  None

RETURN VALUE
  TRUE if get is successful
  FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean ds3gpp_device_ev_hdlr_get_ps_data_off_data_on_status
(
  sys_modem_as_id_e_type                 subs_id,
  ps_sys_ps_data_avail_status_enum_type *data_on_status_ptr
);

/*===========================================================================
FUNCTION  DS3GPP_DEVICE_EV_HDLR_CLEAR_OMADM_PS_DATA_OFF_SERVICE_INFO_ENTRY

DESCRIPTION 
  This function resets the OMADM PS Data Off entry from local cache and
  also the EFS file.

PARAMETERS
  subs_id   - subscription id passed by the client

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds3gpp_device_ev_hdlr_clear_omadm_ps_data_off_service_info_entry
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION    DS3GPP_DEVICE_EV_HDLR_EVALUATE_PS_DATA_OFF_EXEMPT_SERVICES

DESCRIPTION
  This function processes all of the PS DATA OFF Exempt Services caches
  (USIM, OMADM, Home/Roam, default values, etc) and determines what the
  finalized list is for external clients.
 
PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds3gpp_device_ev_hdlr_update_ps_data_off_finalized_info
(
  sys_modem_as_id_e_type                     subs_id
);

/*===========================================================================
FUNCTION    DS3GPP_DEVICE_EV_HDLR_UPDATE_OMADM_PS_DATA_OFF_CACHE

DESCRIPTION
  This function handles caching the PS DATA OFF Exempt Services list that is
  set by OMADM client.  It will also re-evaluate what the exempt services
  list should be advertised as to external clients. 
 
PARAMETERS
  Command Pointer
 
DEPENDENCIES
  NONE

RETURN VALUE
  NONE

SIDE EFFECTS
  NONE
===========================================================================*/
void ds3gpp_device_ev_hdlr_update_omadm_ps_data_off_cache
(
  const ds_cmd_type    *cmd_ptr
);

/*===========================================================================
FUNCTION    DS3GPP_DEVICE_EV_HDLR_SET_PS_DATA_OFF_CHANGE_REASON

DESCRIPTION
  This function allows other entities to update the change reason before
  they call a re-evaluation of the PS Data Off Exempt services list
 
PARAMETERS
  subs_id:        Subscription ID to set the change reason for
  change_reason:  New change reason to be set

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds3gpp_device_ev_hdlr_set_ps_data_off_change_reason
(
  sys_modem_as_id_e_type               subs_id,
  uint32                               new_change_reason
);

/*===========================================================================
FUNCTION    DS3GPP_DEVICE_EV_HDLR_SET_OMADM_PS_DATA_OFF_SERVICE_INFO

DESCRIPTION
  This function handles caching the PS DATA OFF Exempt Services list that is
  set by OMADM client.  It will also re-evaluate what the exempt services
  list should be advertised as to external clients. 
 
PARAMETERS
  arg_val_ptr: Argument to be populated
  subs_id:     Subscription id
  ps_errno:    Err number if IOCTL cant be handled
 
DEPENDENCIES
  NONE

RETURN VALUE
  0  - Success
  -1 - Failure

SIDE EFFECTS
  NONE
===========================================================================*/
int16 ds3gpp_device_ev_hdlr_set_omadm_ps_data_off_service_info
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   cm_subs_id,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION    DS3GPP_DEVICE_EV_HDLR_GET_OMADM_PS_DATA_OFF_SERVICE_INFO

DESCRIPTION
  This function handles retrieving the PS DATA OFF Exempt Services list that
  was previously cached that was set by OMADM client.
 
PARAMETERS
  arg_val_ptr: Argument to be populated
  subs_id:     Subscription id
  ps_errno:    Err number if IOCTL cant be handled

DEPENDENCIES
  None

RETURN VALUE
  0  - Success
  -1 - Failure

SIDE EFFECTS
  None
===========================================================================*/
int ds3gpp_device_ev_hdlr_get_omadm_ps_data_off_service_info
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);
/*===========================================================================
FUNCTION    DS3GPP_DEVICE_EV_HDLR_EVALUATE_PS_DATA_OFF_STATUS

DESCRIPTION
  This function absorbs all the criteria needed to determine whether
  data is overall ON or overall OFF.  If it has changed, it will update
  the PS data off finalized exempt services list and send bearer
  resource modification request to the network to notify network that
  3GPP PS Data Off feature is activated or deactivated. 
 
PARAMETERS
  Device settings to be used to help determine the overall data on status

DEPENDENCIES
  None

RETURN VALUE
  TRUE if get is successful
  FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean ds3gpp_device_ev_hdlr_evaluate_ps_data_off_status
(
  ds3gdevmgr_device_settings_info_type  *device_settings_info_p
);
/*===========================================================================
FUNCTION    DS3GPP_DEVICE_EV_APN_NAME_CHANGE_HDLR

DESCRIPTION    process apn_name_change to unthrottle old apn and throttle 
               new apn

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
 void ds3gpp_device_ev_apn_name_change_hdlr
(
  sys_modem_as_id_e_type             subs_id,
  byte*                              old_apn,
  byte*                              new_apn,
  uint64                             old_apn_type_mask
);

/*===========================================================================
FUNCTION      DS3GPP_DEVICE_EV_HDLR_VOLTE_SWITCH_STATUS_IND

DESCRIPTION   Handles VoLTE switch status change
  
PARAMETERS
  msgrtype   -- Message UMID 
  subs_id    -- Subscription ID
  dsmsg_ptr  -- Pointer to payload

DEPENDENCIES  None

RETURN VALUE  TRUE  if message is successfully received
              FALSE otherwise

SIDE EFFECTS  None
===========================================================================*/
void ds3gpp_device_ev_hdlr_volte_switch_status_ind
(
  msgr_umid_type              msgrtype,
  sys_modem_as_id_e_type      subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

/*===========================================================================
FUNCTION      DS3GPP_DEVICE_EV_HDLR_PROCESS_IMS_SETTINGS

DESCRIPTION   Handles ims setting 
 
PARAMETERS
  subs_id    -- Subscription ID

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gpp_device_ev_hdlr_process_ims_settings
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION    DS3GPP_DEVICE_EV_APN_TYPE_CHANGE_HDLR

DESCRIPTION    process apn_type_change to unthrottle/throttle apn per 
               new/old type

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds3gpp_device_ev_apn_type_change_hdlr
(
  sys_modem_as_id_e_type             subs_id,
  byte*                              apn_name,
  uint64                             new_apn_type,
  uint64                             old_apn_type
);

#endif /* DS_3GPP_DEVICE_EV_HDLR_H */
