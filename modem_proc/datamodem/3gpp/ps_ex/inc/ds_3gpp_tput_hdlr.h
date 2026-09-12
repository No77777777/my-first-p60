#ifndef DS_3GPP_TPUT_HDLR_H
#define DS_3GPP_TPUT_HDLR_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
         
                       D A T A   S E R V I C E S 
                       TPUT HDLR HEADER FILE

GENERAL DESCRIPTION

  All the declarations and definitions necessary to support APN Switch MGR file

 Copyright (c) 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_tput_hdlr.h#1 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/26/18    sxi    Add handling throughput functions to ps_ex folder

===========================================================================*/

/*===========================================================================
                              INCLUDE FILES
===========================================================================*/
#include "sys.h"
#include "ps_sys.h"
#include "ds3gsubsmgr.h"
#include "ds3gtimer.h"
#include "ds3gtputmgr.h"
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
FUNCTION      DS_3GPP_TPUT_HDLR_CONFIGURE_UPLINK_SETTINGS_HANDLE_IRAT

DESCRIPTION   This function handles irat and call tear down, updates the lower 
              layer with timer value and start/stop for uplink reporting.

PARAMETERS 
ds3g_subs_id: subscription ID
any_lte_bearer_up: boolean of if any lte bearer is up
any_wcdma_bearer_up: boolean of if any wcdma bearer is up
reporting_timer:  reporting timer
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
uint32 ds_3gpp_tput_hdlr_configure_uplink_settings_handle_irat
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id,
  boolean                     any_lte_bearer_up,
  boolean                     any_wcdma_bearer_up,
  uint32                      reporting_timer
);

/*===========================================================================
FUNCTION      DS_3GPP_TPUT_HDLR_CONFIGURE_UPLINK_SETTINGS_TPUT_REPORT_ENABLED

DESCRIPTION   This function informs lower layer when throughput reporting enabled

PARAMETERS 
ds3g_subs_id: subscription ID
any_lte_bearer_up: boolean of if any lte bearer is up
any_wcdma_bearer_up: boolean of if any wcdma bearer is up
reporting_timer: reporting timer
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
uint32 ds_3gpp_tput_hdlr_configure_uplink_settings_tput_report_enabled
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id,
  boolean                     any_lte_bearer_up,
  boolean                     any_wcdma_bearer_up,
  uint32                      reporting_timer
);

/*===========================================================================
FUNCTION      DS_3GPP_TPUT_HDLR_CONFIGURE_UPLINK_SETTINGS_TPUT_REPORT_DISABLED

DESCRIPTION   This function informs lower layer when throughput reporting disabled

PARAMETERS 
ds3g_subs_id: subscription ID
reporting_timer: reporting timer
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
uint32 ds_3gpp_tput_hdlr_configure_uplink_settings_tput_report_disabled
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id,
  uint32                      reporting_timer
);

/*===========================================================================
FUNCTION  DS_3GPP_TPUT_HDLR_UPDATE_TIMER_BASED_DOWNLINK_THROUGHPUT_INTERVAL

DESCRIPTION
  This function update the actual interval and t_accumulate interval for 
  LTE and WCDMA mode for timer based throughput reporting.

PARAMETERS
  subs_id - Subscription ID
  current_nw_mode - current network mode, LTE or WCDMA

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_3gpp_tput_hdlr_update_timer_based_downlink_throughput_interval
(
   sys_modem_as_id_e_type subs_id,
   sys_sys_mode_e_type    current_nw_mode
);

/*===========================================================================
FUNCTION  DS_3GPP_TPUT_HDLR_UPDATE_THRES_BASED_DOWNLINK_THROUGHPUT_INTERVAL

DESCRIPTION
  This function update the actual interval and t_accumulate interval for 
  LTE and WCDMA mode for threshold based throughput reporting.

PARAMETERS
  subs_id - Subscription ID
  current_nw_mode - current network mode, LTE or WCDMA

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_tput_hdlr_update_thres_based_downlink_throughput_interval
(
   sys_modem_as_id_e_type subs_id,
   sys_sys_mode_e_type    current_nw_mode
);

/*===========================================================================
FUNCTION  DS_3GPP_TPUT_HDLR_DOWNLINK_UPDATE_REPORT_PARAMS

DESCRIPTION
  This function updates params: whether needs to report and report status


PARAMETERS
  need_to_report_ptr - pointer of whether needs to report
  report_status_pte - pointer of report status
  current_nw_mode - current network mode
  subs_id - subscription ID
 
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_tput_hdlr_downlink_update_report_params
(
  boolean                                  *need_to_report_ptr,  
  ds3gtputmgr_est_reporting_status_e_type  *report_status_ptr,
  sys_sys_mode_e_type                       current_nw_mode,
  sys_modem_as_id_e_type                    subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_TPUT_HDLR_DOWNLINK_STOP_LOWER_LAYER_REPORTING

DESCRIPTION
  This function stops lower layer throughput reporting 

PARAMETERS
  subs_id - Subscription ID

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_tput_hdlr_downlink_stop_lower_layer_reporting
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_TPUT_HDLR_DOWNLINK_TPUT_REPORT_ENABLED

DESCRIPTION   This function checks if timer/threshold based throughput
              reporting is started or stopped.
 
PARAMETERS
  subs_id - Subscription ID
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds_3gpp_tput_hdlr_downlink_tput_report_enabled
(
  ds3gsubsmgr_subs_id_e_type   ds3g_subs_id
); 

/*===========================================================================
FUNCTION  DS_3GPP_TPUT_HDLR_DOWNLINK_TIMER_BASED_REPORTING

DESCRIPTION
  This function handles when downlink timer based reporting is enabled

PARAMETERS
  subs_id - Subscription ID
 

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_tput_hdlr_downlink_timer_based_reporting
(
   sys_modem_as_id_e_type subs_id,
   boolean                timer_value_updated
);

/*===========================================================================
FUNCTION  DS_3GPP_TPUT_HDLR_DOWNLINK_THRES_BASED_REPORTING

DESCRIPTION
  This function handles when downlink threshold based reporting is enabled

PARAMETERS
  subs_id - Subscription ID
 

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_tput_hdlr_downlink_thres_based_reporting
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_TPUT_HDLR_DOWNLINK

DESCRIPTION
  This function handles changes of downlink throughput reporting

PARAMETERS
  subs_id - Subscription ID
  timer_value_updated - boolean of whether timer is updated

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_tput_hdlr_downlink
(
   sys_modem_as_id_e_type subs_id,
   boolean                timer_value_updated
);

#endif /* DS_3GPP_TPUT_HDLR_H*/
