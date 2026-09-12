#ifndef DS_WLAN_MEAS_LTEU_SM_H
#define DS_WLAN_MEAS_LTEU_SM_H


/*===========================================================================

                        DS_WLAN_MEAS_LTEU_SM.H

DESCRIPTION
   

                
EXTERNALIZED FUNCTIONS

 Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                       EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/wlan_meas/inc/ds_wlan_meas_lteu_sm.h#2 $
  $Author: pwbldsvc $  $DateTime: 2019/10/22 02:53:17 $
  
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
05/30/2015    Youjunf                          First version of file
===========================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"

#ifdef FEATURE_ELS_WIFIMEAS_LTE_U_PH1

#include "ds_wlan_meas_comm_mgr.h"
#include "ds_wlan_meas_rrc_if.h"

/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/

/*-------------------------------------------------------------------------
  Enum for lteu wlan mode type
-------------------------------------------------------------------------*/
typedef enum
{ 
  DS_WLAN_MEAS_LTEU_WLAN_MODE_OFF,
  DS_WLAN_MEAS_LTEU_WLAN_MODE_STA_DIS,
  DS_WLAN_MEAS_LTEU_WLAN_MODE_STA_CONN,
  DS_WLAN_MEAS_LTEU_WLAN_MODE_SOFTAP,
  DS_WLAN_MEAS_LTEU_WLAN_MODE_P2P_GO,
  DS_WLAN_MEAS_LTEU_WLAN_MODE_P2P_CLIENT,
  DS_WLAN_MEAS_LTEU_WLAN_MODE_STA_DIS_SOFTAP,
  DS_WLAN_MEAS_LTEU_WLAN_MODE_STA_CONN_SOFTAP,
  DS_WLAN_MEAS_LTEU_WLAN_MODE_SOFTAP_SOFTAP
}ds_wlan_meas_lteu_wlan_mode_e_type;

/*-------------------------------------------------------------------------
  Structure for lte-u wlan paramter type
-------------------------------------------------------------------------*/
typedef struct
{
  char                                ssid[PS_SYS_MAX_SSID_LEN]; 
  uint8                               bssid[PS_SYS_MAC_ADDR_LEN]; 
  boolean                             is_prim_channel_valid;            
  uint16                              prim_channel_freq;     
  boolean                             is_secd_channel_valid;
  uint16                              secd_channel_freq;
  ps_sys_wlan_bandwidth_type_enum     prim_channel_bw; 
  ps_sys_wlan_network_mode_type_enum  network_mode;
}ds_wlan_meas_lteu_wlan_param_s_type;

/*-------------------------------------------------------------------------
  Structure for lte-u wlan mode information type
-------------------------------------------------------------------------*/
typedef struct
{
  ds_wlan_meas_lteu_wlan_param_s_type   sta_param;
  ds_wlan_meas_lteu_wlan_param_s_type   softap_param[2];  
}ds_wlan_meas_lteu_wlan_mode_info_s_type;

/*-------------------------------------------------------------------------
  lte-u State machine structure
-------------------------------------------------------------------------*/
typedef struct
{  
 boolean                                     is_rrc_configed;
 uint16                                      rrc_meas_id;
 uint32                                      meas_id;  
 boolean                                     is_wlan_meas_started;
 boolean                                     is_first_scan_meas_reported_rrc;
 uint8                                       report_count;
 uint8                                       report_amount;
 uint32                                      report_interval;
 ds_wlan_meas_lteu_wlan_mode_e_type          wlan_mode;
 ds_wlan_meas_lteu_wlan_mode_info_s_type     wlan_mode_info;
 ds_wlan_meas_config_info_s_type             *meas_config_info_ptr;
 ds_wlan_meas_report_info_s_type             *scanned_meas_report_ptr;
}ds_wlan_meas_lteu_periodic_sm_s_type; 


/*===========================================================================
            EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/


/*===========================================================================
FUNCTION      DS_WLAN_MEAS_LTEU_SM_INIT

DESCRIPTION   This function initializes lteu meas state machine

DEPENDENCIES  Event Dispatcher module is initialized already.

RETURN VALUE  None.

SIDE EFFECTS  Registered with event dispatcher module to get notification
===========================================================================*/
void ds_wlan_meas_lteu_sm_init(void);

#endif /*FEATURE_ELS_WIFIMEAS_LTE_U_PH1*/

#endif /* DS_WLAN_MEAS_LTEU_SM_H */
