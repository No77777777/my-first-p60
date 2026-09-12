#ifndef DS_WLAN_MEAS_COMM_MGR_H
#define DS_WLAN_MEAS_COMM_MGR_H

/*===========================================================================
                      DS_WLAN_MEAS_COMM_MGR.H

DESCRIPTION

   This module handles wlan meas request and report.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                      EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/wlan_meas/inc/ds_wlan_meas_comm_mgr.h#2 $
  $Author: pwbldsvc $  $DateTime: 2019/10/22 02:53:17 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
05/18/2015    Youjunf                          First version of file
===========================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"

#if defined(FEATURE_ELS_WIFIMEAS_LTE_U_PH1) || defined(FEATURE_ELS_LWA) || defined(FEATURE_DATA_ANDSF)

#include "ps_sys.h"


/*===========================================================================
                                  MACROS
===========================================================================*/
#define DS_WLAN_MEAS_MAX_MEAS_IDS    15

#define DS_WLAN_MEAS_MAX_SSID        16
#define DS_WLAN_MEAS_MAX_BSSID       16
#define DS_WLAN_MEAS_MAX_BLACKLIST   15


/*define the max and min 5G channel freq in MHz*/
#define  WIFI_CHANNEL_FREQ_5G_MIN    4890     
#define  WIFI_CHANNEL_FREQ_5G_MAX    5825    

/*reserved channel value*/
#define  WIFI_CHANNEL_SERVING        0x0000
#define  WIFI_CHANNEL_ANY            0xFFFF
#define  WIFI_CHANNEL_BAND_2_DOT_4   0x0001
#define  WIFI_CHANNEL_BAND_3_DOT_6   0x0002
#define  WIFI_CHANNEL_BAND_5         0x0003


/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/

typedef ps_sys_wlan_wifi_meas_start_type ds_wlan_meas_config_info_s_type;
typedef ps_sys_wlan_wifi_meas_report_config_status_type ds_wlan_meas_config_status_s_type;

typedef struct
{
  uint8  bssid[PS_SYS_MAC_ADDR_LEN];        
}ds_wlan_meas_bssid_s_type;

typedef struct
{
  uint8 num_bssid;
  ds_wlan_meas_bssid_s_type bssid_list[DS_WLAN_MEAS_MAX_BLACKLIST];
}ds_wlan_meas_blacklist_info_s_type;


typedef struct
{
  uint8                              bssid[PS_SYS_MAC_ADDR_LEN];
  uint16                             prim_channel;      //primary channel
  uint16                             secd_channel;      //seconday channel
  uint16                              meas_param_mask;
  int16                              rssi;
  int16                              sinr;
  uint16                             bss_load;          //only the channel utilization filed of bss_load
  uint16                             ul_phy_rate;
  uint16                             dl_phy_rate;
  uint16                             ul_per;
  uint16                             dl_per;
  ps_sys_wlan_bandwidth_type_enum    prim_channel_bw;          //bandwidth of primary channel
  ps_sys_wlan_network_mode_type_enum network_mode;     //network_mode
}ds_wlan_meas_report_bssid_info_s_type;

typedef struct
{
  char  ssid[PS_SYS_MAX_SSID_LEN]; 
  uint8 num_bssid;
  ds_wlan_meas_report_bssid_info_s_type bssid_info[DS_WLAN_MEAS_MAX_BSSID];
}ds_wlan_meas_report_ssid_info_s_type;


typedef struct
{
  uint32 meas_id;
  uint8  num_ssid;
  ds_wlan_meas_report_ssid_info_s_type ssid_info[DS_WLAN_MEAS_MAX_SSID];
}ds_wlan_meas_report_info_s_type;



/*===========================================================================
            EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/

/*===========================================================================
FUNCTION      DS_WLAN_MEAS_COMM_MGR_INIT

DESCRIPTION   This function initializes common wlan meas manager module.

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_wlan_meas_comm_mgr_init(void);



/*===========================================================================
FUNCTION      DS_WLAN_MEAS_START

DESCRIPTION   start wlan meas, return TRUE if started successfully

PARAMETERS 
    meas_config_info_ptr - pointer of wlan meas configuration 

DEPENDENCIES  None

RETURN VALUE  TRUE   - return true if wlan meas start successfully
              FALSE  - return false if fail to start wlan meas

SIDE EFFECTS  None
===========================================================================*/
boolean ds_wlan_meas_start
(
  ds_wlan_meas_config_info_s_type *meas_config_info_ptr
);



/*===========================================================================
FUNCTION      DS_WLAN_MEAS_STOP

DESCRIPTION   stop wlan meas, return TRUE if successfully stop

PARAMETERS 
    meas_id

DEPENDENCIES  None

RETURN VALUE  TRUE   - return true if wlan meas stop successfully
              FALSE  - return false if fail to stop wlan meas

SIDE EFFECTS  None
===========================================================================*/
boolean ds_wlan_meas_stop(uint32 meas_id);



/*===========================================================================
FUNCTION      DS_WLAN_MEAS_COMM_CREATE_MEAS_ID

DESCRIPTION   create comm meas id accoridng to specified client_type and 
              per_client meas_id 

PARAMETERS 
    client_type
    client_meas_id

DEPENDENCIES  None

RETURN VALUE  comm meas_id

SIDE EFFECTS  None
===========================================================================*/
uint32 ds_wlan_meas_comm_create_meas_id
(
  uint8  client_type,
  uint8  client_meas_id
);


/*===========================================================================
FUNCTION      DS_WLAN_MEAS_GET_CLIENT_ID_FROM_MEAS_ID

DESCRIPTION   get client id from comm meas id

PARAMETERS 
    meas_id - comm meas_id

DEPENDENCIES  None

RETURN VALUE  client_id

SIDE EFFECTS  None
===========================================================================*/
uint8 ds_wlan_meas_get_client_id_from_meas_id(uint32 meas_id);


/*===========================================================================
FUNCTION      DS_WLAN_MEAS_IS_MEAS_ID_VALID

DESCRIPTION   check if the comm meas id valid. return TRUE if meas_id valid

PARAMETERS 
    meas_id - comm meas_id

DEPENDENCIES  None

RETURN VALUE  TRUE    if meas_id is valid
              FALSE   if meas_id is false

SIDE EFFECTS  None
===========================================================================*/
boolean ds_wlan_meas_is_meas_id_valid(uint32 meas_id);

#endif /* FEATURE_ELS_WIFIMEAS_LTE_U_PH1 || FEATURE_ELS_LWA || FEATURE_DATA_ANDSF*/
#endif /* DS_WLAN_MEAS_COMM_MGR_H */
