#ifndef DS_IWLAN_S2B_EXT_H
#define DS_IWLAN_S2B_EXT_H
/*===========================================================================
                           DS_IWLAN_S2B_EXT_H
===========================================================================*/

/*!
  @file
  ds_iwlan_s2b_ext.h

  @brief
  This module includes the function declarations used by other modules.

  @detail  
*/

/*===========================================================================

  Copyright (c) 2019 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/wlan/iwlan_s2b/inc/ds_iwlan_s2b_ext.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/08/19   my      Added support for QSH MDUMP collection
06/08/18   js      Changed rel14 emergency param to unauth emergency
04/12/18   js      FR46730: Added support for Rel.14 Emergency
02/02/18   vl     Added changes for Rel. 13 VoWiFi emergency calls.
07/20/17   js      Added support to check EPC throttling status during HO
03/27/17   fn      Added support for dsda
12/06/16   fn      Added pco support and addr hdlr changes
06/15/16   fn      Added support for dsds capable subs
04/07/16   op      Added support for handoff info in call allowed handler
04/28/15   op      Added support to get ePDG address IP pref config item
11/25/14   ab      Added support to handle ds3g events hdlr cb via one func 
10/16/14   fn      Added support for thermal mitigation
10/13/14   vl      Added changes for DS3G API for MH - MPPM interaction to 
                   avoid race conditions
06/12/14   op      Added support to handle DS3G Event Mgr events
04/03/14   op      Added new API to get MMGSDI events
11/15/13   ab      Initial Version

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "ds3geventmgr.h"
#include "ds_dsd_ext_i.h"

#ifdef FEATURE_DATA_IWLAN_S2B
#include "ds_wlan_util.h"

#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif

/*===========================================================================
                         EXTERNAL FUNCTION DEFINTIONS
===========================================================================*/
typedef boolean (*ds_iwlan_s2b_call_allowed_hdlr_f_type)
(
  uint32                                pf_id,
  ps_sys_subscription_enum_type         subs_id,
  boolean                               is_v6_call,
  boolean                              *is_ho,
  acl_handoff_ctxt_type                *handoff_ctxt
);

typedef boolean (*ds_iwlan_s2b_ho_throttle_flag_hdlr_f_type)
(
  ps_iface_type                        *iface_ptr
);

/*-------------------------------------------------------------------------
  EPDG IP Preference
-------------------------------------------------------------------------*/
typedef enum
{
  DS_IWLAN_S2B_EPDG_ADDR_TYPE_INVALID         = 0,

  DS_IWLAN_S2B_EPDG_ADDR_TYPE_IP4_ONLY,
  DS_IWLAN_S2B_EPDG_ADDR_TYPE_IP6_ONLY,
  DS_IWLAN_S2B_EPDG_ADDR_TYPE_IP4_PREFERRED,
  DS_IWLAN_S2B_EPDG_ADDR_TYPE_IP6_PREFERRED

} ds_iwlan_s2b_epdg_addr_ip_pref_type;

#define DS_IWLAN_S2B_CONFIG_EPDG_ADDR_PREF_TYPE_IS_VALID(epdg_addr_ip_pref)    \
  ((epdg_addr_ip_pref == DS_IWLAN_S2B_EPDG_ADDR_TYPE_IP4_ONLY) ||              \
   (epdg_addr_ip_pref == DS_IWLAN_S2B_EPDG_ADDR_TYPE_IP6_ONLY) ||              \
   (epdg_addr_ip_pref == DS_IWLAN_S2B_EPDG_ADDR_TYPE_IP4_PREFERRED) ||         \
   (epdg_addr_ip_pref == DS_IWLAN_S2B_EPDG_ADDR_TYPE_IP6_PREFERRED))

/*===========================================================================
FUNCTION      DS_IWLAN_S2B_PDN_CONTEXT_PROCESS_CMD

DESCRIPTION   Called by 3G DSMGR, to handle commands specific to this
              mode handler.

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None 
===========================================================================*/
void ds_iwlan_s2b_pdn_context_process_cmd
(
  ds_cmd_type   *ds_cmd_ptr
);


/*===========================================================================
FUNCTION  ds_iwlan_s2b_task_init()

DESCRIPTION  Run task init functions
  
PARAMETERS  
  None.

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None. 
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_iwlan_s2b_task_init
(
  void
);

/*===========================================================================
FUNCTION  ds_iwlan_s2b_task_deinit()

DESCRIPTION  Cleanup memory allocated during ds_iwlan_s2b_task_init
  
PARAMETERS  
  None.

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None. 
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
#ifdef TEST_FRAMEWORK
#error code not present
#endif

/*===========================================================================
FUNCTION  ds_iwlan_s2b_process_nv_refresh_event()

DESCRIPTION  Process NV Refresh event
  
PARAMETERS  
  ds_wlan_nv_refresh_event_info_type* nv_refresh_event_info_ptr.

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None. 
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
#ifdef FEATURE_MODEM_CONFIG_REFRESH
void ds_iwlan_s2b_process_nv_refresh_event
(
  ds_wlan_nv_refresh_event_info_type* nv_refresh_event_info_ptr
);
#endif

/*===========================================================================
FUNCTION  ds_iwlan_s2b_process_wlan_capable_event()

DESCRIPTION  Process wlan_capable event
  
PARAMETERS  
  ds_wlan_nv_refresh_event_info_type* nv_refresh_event_info_ptr.

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None. 
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_iwlan_s2b_process_wlan_capable_event
(
  ds_wlan_capable_event_info_type* capable_event_info_ptr
);

/*===========================================================================
FUNCTION ds_iwlan_s2b_reg_call_allowed_hdlr

DESCRIPTION
  This function is used to register the call allowed handler with the
  EPDG Mode Handler. This API is called by EPC Mode Handler. 

PARAMETERS
  call_allowed_hdlr_f - Pointer to the call allowed handler provided by EPC MH

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_iwlan_s2b_pdn_context_reg_call_allowed_hdlr
(
  ds_iwlan_s2b_call_allowed_hdlr_f_type call_allowed_hdlr_f
);

/*===========================================================================
FUNCTION ds_iwlan_s2b_pdn_context_reg_ho_throttle_flag_hdlr

DESCRIPTION
  This function is used to register the EPC HO throttle flag handler with the
  EPDG Mode Handler. This API is called by EPC Mode Handler. 

PARAMETERS
  ho_throttle_flag_hdlr_f - Pointer to the EPC HO throttle flag handler
                            provided by EPC MH

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_iwlan_s2b_pdn_context_reg_ho_throttle_flag_hdlr
(
  ds_iwlan_s2b_ho_throttle_flag_hdlr_f_type ho_throttle_flag_hdlr_f
);

/*===========================================================================
FUNCTION       ds_iwlan_s2b_ds3g_event_hdlr_cb

DESCRIPTION    This is the function called by DS3G to process the 
               IWLAN S2B EVENT HDLR callback

PARAMETERS       ds3geventmgr_event_type       event_id,
                 ds3geventmgr_filter_type      *filter_info_ptr,
                 void                          *event_info_ptr,
                 void                          *data_ptr

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_iwlan_s2b_ds3g_event_hdlr_cb
(
  ds3geventmgr_event_type       event_id,
  ds3geventmgr_filter_type      *filter_info_ptr,
  void                          *event_info_ptr,
  void                          *data_ptr
);

/*===========================================================================
FUNCTION DS_IWLAN_S2B_DSD_EVENT_CB()

DESCRIPTION
  This function is registered as a callback for DSD's pref system change
  event notification

PARAMETERS
  ds_dsd_ext_event_type        event,
  ds_dsd_ext_event_info_type * event_info_ptr,
  void                       * data_ptr

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_iwlan_s2b_dsd_event_cb
(
  ds_dsd_ext_event_type        event,
  ds_dsd_ext_event_info_type * event_info_ptr,
  void                       * data_ptr
);

/*===========================================================================  
FUNCTION ds_iwlan_s2b_pdn_context_process_thermal_mit_teardown 
 
DESCRIPTION
  Handle teardowns due to thermal mitigation levels L1/L2
 
PARAMETERS  
  void * action    Pointer to thermal action
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None 
===========================================================================*/
void ds_iwlan_s2b_pdn_context_process_thermal_mit_teardown
(
  void * action
);

/*===========================================================================
FUNCTION      ds_iwlan_s2b_config_is_rls13_emergency_support_enabled

DESCRIPTION   Used by wlan_util to query is_rls13_emergency_support_enabled.

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds_iwlan_s2b_config_is_rls13_emergency_support_enabled
(
  sys_modem_as_id_e_type            cm_subs_id
);

/*===========================================================================
FUNCTION      DS_IWLAN_S2B_CONFIG_IS_UNATH_EMERGENCY_SUPPORT_ENABLED

DESCRIPTION   Used by wlan_util to query is_unauth_emergency_support_enabled.

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds_iwlan_s2b_config_is_unauth_emergency_support_enabled
(
  sys_modem_as_id_e_type            cm_subs_id
);

/*===========================================================================
FUNCTION      ds_iwlan_s2b_config_is_emergency_epdg_support_enabled

DESCRIPTION   This functions returns TRUE if release 13 emergency EPDG support 
              is enabled OR emergency EPDG selection is enabled 

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds_iwlan_s2b_config_is_emergency_epdg_support_enabled
(
  sys_modem_as_id_e_type            cm_subs_id
);

#ifdef FEATURE_QSH_MDUMP

/*===========================================================================

FUNCTION ds_iwlan_s2b_epdg_addr_reslver_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for ds_iwlan_s2b_is_impi_ready static variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_iwlan_s2b_epdg_addr_reslver_qsh_mdump_collect();

/*===========================================================================
FUNCTION ds_iwlan_s2b_iface_hdlr_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for ds_iwlan_s2b_iface_cb_tbl static variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_iwlan_s2b_iface_hdlr_qsh_mdump_collect();

/*===========================================================================
FUNCTION ds_iwlan_s2b_ikev2_hdlr_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for ikev2_cb_ptr_table static variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_iwlan_s2b_ikev2_hdlr_qsh_mdump_collect();

/*===========================================================================
FUNCTION ds_iwlan_s2b_pdn_context_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for ds_iwlan_s2b_global_nat_ka_offload_info static variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_iwlan_s2b_pdn_context_qsh_mdump_collect();

/*===========================================================================
FUNCTION ds_iwlan_s2b_pdn_throttle_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for ds_iwlan_s2b_pdn_throttle_per_ssid_tbl static variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_iwlan_s2b_pdn_throttle_qsh_mdump_collect();

#endif /* FEATURE_QSH_MDUMP */

#endif /* FEATURE_DATA_IWLAN_S2B */
#endif /* DS_IWLAN_S2B_IFACE_EXT_H */