#ifndef DS_WLAN_MEAS_RRC_IF_H
#define DS_WLAN_MEAS_RRC_IF_H
/*===========================================================================

                        DS_WLAN_MEAS_RRC_IF.H

DESCRIPTION
   
   header of wlan meas rrc interface
                
EXTERNALIZED FUNCTIONS

 Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                       EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/wlan_meas/inc/ds_wlan_meas_rrc_if.h#2 $
  $Author: pwbldsvc $  $DateTime: 2019/10/22 02:53:17 $
  
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/11/15   hr      Implementated wlan meas rrc interface
05/30/15   Youjunf First version of file
===========================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"

#if defined(FEATURE_ELS_WIFIMEAS_LTE_U_PH1) || defined(FEATURE_ELS_LWA)

#include "lte_rrc_ext_msg.h"
#include "ps_sys.h"

/*===========================================================================
                    EXTERNALIZED FUNCTIONS DEFINITIONS
===========================================================================*/
/*===========================================================================
FUNCTION      DS_WLAN_MEAS_RRC_IF_INIT

DESCRIPTION   Initiation function for wlan meas rrc interface module
 
PARAMETERS    None
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_wlan_meas_rrc_if_init
(
  void
);

/*===========================================================================
FUNCTION      DS_WLAN_MEAS_RRC_IF_CHANNEL_FREQ_TO_NUM

DESCRIPTION   This function converts channel freq to channel number defined 
              by lte_wifi_channel_num_e in rrc interface 
 
PARAMETERS    uint16: channel frequency in MHz
 
DEPENDENCIES  None

RETURN VALUE  lte_wifi_channel_num_e: channel number

SIDE EFFECTS  None
===========================================================================*/
lte_wifi_channel_num_e ds_wlan_meas_rrc_if_channel_freq_to_num
(
  uint16 channel_freq
);

/*===========================================================================
FUNCTION      DS_WLAN_MEAS_RRC_IF_CHANNEL_NUM_TO_FREQ

DESCRIPTION   This function converts channel number defined by 
              lte_wifi_channel_num_e in rrc interface to channel freq 
 
PARAMETERS    lte_wifi_channel_num_e: channel number
 
DEPENDENCIES  None

RETURN VALUE  uint16: channel frequency in MHz

SIDE EFFECTS  None
===========================================================================*/
uint16 ds_wlan_meas_rrc_if_channel_num_to_freq
(
  lte_wifi_channel_num_e channel_num
);

/*===========================================================================
FUNCTION      DS_WLAN_MEAS_RRC_IF_PS_TO_LTE_WIFI_NW_MODE

DESCRIPTION   This function converts PS wlan network mode to LTE RRC wifi 
              nw mode enum 
 
PARAMETERS    ps_sys_wlan_network_mode_type_enum: PS wlan network mode
 
DEPENDENCIES  None

RETURN VALUE  lte_wifi_nw_mode_e: LTE RRC wifi nw mode

SIDE EFFECTS  None
===========================================================================*/
lte_wifi_nw_mode_e ds_wlan_meas_rrc_if_ps_to_lte_wifi_nw_mode
(
  ps_sys_wlan_network_mode_type_enum network_mode
);

/*===========================================================================
FUNCTION      DS_WLAN_MEAS_RRC_IF_PS_TO_LTE_BW_TYPE

DESCRIPTION   This function converts PS wlan bandwidth type to LTE RRC LTE 
              wifi bandwidth enum 
 
PARAMETERS    ps_sys_wlan_bandwidth_type_enum: PS wlan bandwidth type
 
DEPENDENCIES  None

RETURN VALUE  lte_wifi_bandwidth_e: LTE RRC LTE wifi bandwidth enum 

SIDE EFFECTS  None
===========================================================================*/
lte_wifi_bandwidth_e ds_wlan_meas_rrc_if_ps_to_lte_bw_type
(
  ps_sys_wlan_bandwidth_type_enum bw_type
);

/*===========================================================================
FUNCTION      DS_WLAN_MEAS_RRC_IF_SEND_RRC_MEAS_REPORT

DESCRIPTION   This function sends the wifi measurement repport to interested 
              clients through message router 
 
PARAMETERS    lte_rrc_wifi_meas_rpt_ind_s: wifi meas report info
 
DEPENDENCIES  None

RETURN VALUE  TRUE:  if message sent to message router successfully 
              FALSE: otherwise

SIDE EFFECTS  None
===========================================================================*/
boolean ds_wlan_meas_rrc_if_send_rrc_meas_report
(
  lte_rrc_wifi_meas_rpt_ind_s *rrc_meas_report_ptr
);
#endif /* FEATURE_ELS_WIFIMEAS_LTE_U_PH1 || FEATURE_ELS_LWA*/

#endif /* DS_WLAN_MEAS_RRC_IF_H */
