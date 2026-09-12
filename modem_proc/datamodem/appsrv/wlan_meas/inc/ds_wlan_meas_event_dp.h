#ifndef DS_WLAN_MEAS_EVENT_DP_H
#define DS_WLAN_MEAS_EVENT_DP_H

/*===========================================================================

                       DS_WLAN_MEAS_EVENT_DP.H
DESCRIPTION

   header for internal event dispatcher

EXTERNALIZED FUNCTIONS

 Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                       EDIT HISTORY FOR FILE

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

#if defined(FEATURE_ELS_WIFIMEAS_LTE_U_PH1) || defined(FEATURE_ELS_LWA) || defined(FEATURE_DATA_ANDSF)

#include "ps_sys.h"
#include "ds_wlan_meas_comm_mgr.h"
/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/
typedef enum
{
  DS_WLAN_MEAS_CLIENT_INVALID = 0,
  DS_WLAN_MEAS_CLIENT_ANDSF,
  DS_WLAN_MEAS_CLIENT_LTEU_SM_PERIODIC,
  DS_WLAN_MEAS_CLIENT_LTEU_WIFI_VACATE,
  DS_WLAN_MEAS_CLIENT_PDCP_SM_PERIODIC,
  DS_WLAN_MEAS_CLIENT_PDCP_SM_WA1,
  DS_WLAN_MEAS_CLIENT_PDCP_SM_WA2,
  DS_WLAN_MEAS_CLIENT_PDCP_SM_WN,
  DS_WLAN_MEAS_CLIENT_PDCP_SM_WS,
  DS_WLAN_MEAS_CLIENT_MAX = DS_WLAN_MEAS_CLIENT_PDCP_SM_WS
}ds_wlan_meas_event_client_e_type;


typedef enum
{
  DS_WLAN_MEAS_INVALID_EV = 0,

  /*wlan connection status change notified by ds_wlan_meas_conn_status
    module, only applicable to LTE-U */
  DS_WLAN_MEAS_WLAN_CONN_STATUS_CHG_EV,

  /*meas config status from comm manager*/
  DS_WLAN_MEAS_COMM_MEAS_CONFIG_STATUS_EV,

  /*Meas Report Events from common manager*/
  DS_WLAN_MEAS_COMM_REPORT_EV,

  /*serving or neihgh meas permision Events from WLAN aggr hdlr,
   only applicable to PDCP WA1, WA2, WN(WA4), WS(WA5)*/
  DS_WLAN_MEAS_SERV_ALLOW_EV,
  DS_WLAN_MEAS_NEIGH_ALLOW_EV,

  /*RRC meas config Event, applicable to PDCP aggr and LTE-U*/
  DS_WLAN_MEAS_RRC_ADD_CONFIG_EV,
  DS_WLAN_MEAS_RRC_REMOVE_CONFIG_EV,

  /*periodic report timer expire event, applicable to PDCP aggr and LTE-U*/
  DS_WLAN_MEAS_REPORT_TIMER_EXPIRE_EV,

  DS_WLAN_MEAS_MAX_EV = DS_WLAN_MEAS_REPORT_TIMER_EXPIRE_EV

}ds_wlan_meas_event_e_type;


typedef struct
{
  char  awlan_ssid[PS_SYS_MAX_SSID_LEN];
  uint8 awlan_bssid[PS_SYS_MAC_ADDR_LEN];
}ds_wlan_meas_awlan_serv_id_s_type;

typedef struct
{
  boolean is_allowed;
  ds_wlan_meas_awlan_serv_id_s_type  awlan_serv_id;
}ds_wlan_meas_allow_info_s_type;

typedef enum
{
  DS_WLAN_MEAS_INVALID_IND = -1,
  DS_WLAN_MEAS_STA_DIS_IND,
  DS_WLAN_MEAS_STA_CONN_IND,
  DS_WLAN_MEAS_AP_DIS_IND,
  DS_WLAN_MEAS_AP_ASSOC_IND,
  DS_WLAN_MEAS_P2P_GO_DIS_IND,
  DS_WLAN_MEAS_P2P_GO_ASSOC_IND,
  DS_WLAN_MEAS_P2P_CLIENT_DIS_IND,
  DS_WLAN_MEAS_P2P_CLIENT_ASSOC_IND,
  DS_WLAN_MEAS_STA_DIS_AP_ASSOC_IND,
  DS_WLAN_MEAS_STA_CONN_AP_ASSOC_IND,
  DS_WLAN_MEAS_WLAN_OFF_IND,
  DS_WLAN_MEAS_MAX_IND
}ds_wlan_meas_conn_status_ind_e_type;

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
  ds_wlan_meas_conn_status_ind_e_type conn_status_ind;
}ds_wlan_meas_conn_status_info_s_type;



typedef struct
{
  uint16                              rrc_meas_id;
  /*all below field only applicable when rrc add config*/
  boolean                             is_valid_rssi_threshold;
  int16                               rssi_threshold;     //in dbm
  int16                               hysteresis;         //in dbm
  uint16                              time_to_trigger;    //in ms
  uint32                              report_interval;    //in ms
  uint8                               report_amount;
  ds_wlan_meas_blacklist_info_s_type  blacklist;
  ps_sys_wlan_wifi_meas_ssid_list     ssid_list;
}ds_wlan_meas_rrc_config_info_s_type;


typedef struct
{
  uint32 timer_id;
}ds_wlan_meas_report_timer_expire_info_s_type;


typedef union
{
  ds_wlan_meas_conn_status_info_s_type         conn_status_info;
  ds_wlan_meas_config_status_s_type            meas_config_status;
  ds_wlan_meas_report_info_s_type              meas_report_info;
  ds_wlan_meas_allow_info_s_type               meas_allow_info;
  ds_wlan_meas_rrc_config_info_s_type          rrc_cofig_info;
  ds_wlan_meas_report_timer_expire_info_s_type report_timer_expire_info;
}ds_wlan_meas_event_info_u_type;


typedef struct
{
  ds_wlan_meas_event_client_e_type  client_id;
  ds_wlan_meas_event_e_type         event_id;
  ds_wlan_meas_event_info_u_type    event_info;
}ds_wlan_meas_event_notify_info_s_type;


typedef void (*ds_wlan_meas_event_notify_func_type)(
               ds_wlan_meas_event_client_e_type  client_id,
               ds_wlan_meas_event_e_type         event_id,
               ds_wlan_meas_event_info_u_type    *event_info_ptr);

/*===========================================================================
           EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/


/*===========================================================================
FUNCTION      DS_WLAN_MEAS_EVENT_DP_INIT

DESCRIPTION   Called at initialization.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  Queue is defined and initialized.
===========================================================================*/
void ds_wlan_meas_event_dp_init(void);



/*===========================================================================
FUNCTION      DS_WLAN_MEAS_EVENT_DP_REG_CLIENT

DESCRIPTION   Called by clients to register to get event notification.

DEPENDENCIES  None.

RETURN VALUE  TRUE: if registration successful
                    if client previously registgered

              FALSE: if registration unsuccessful

SIDE EFFECTS  Registered client is entered on a queue
===========================================================================*/
boolean ds_wlan_meas_event_dp_reg_client
(
  ds_wlan_meas_event_client_e_type     client_id,
  ds_wlan_meas_event_e_type            event_id,
  ds_wlan_meas_event_notify_func_type  client_notify_func_ptr
);


/*===========================================================================
FUNCTION      DS_DSD_WLAN_MEAS_EVENT_DP_DEREG_CLIENT

DESCRIPTION   Called by clients to de-register for event notification.

DEPENDENCIES  None.

RETURN VALUE  TRUE: if de-registration successful
                    if client previously de-registgered

              FALSE: if de-registration unsuccessful

SIDE EFFECTS  De-registered client is dequeued
===========================================================================*/
boolean ds_wlan_meas_event_dp_dereg_client
(
  ds_wlan_meas_event_client_e_type     client_id,
  ds_wlan_meas_event_e_type            event_id
);

void ds_wlan_meas_event_notify_client
(
 ds_wlan_meas_event_client_e_type     client_id,
 ds_wlan_meas_event_e_type            event_id,
 ds_wlan_meas_event_info_u_type       *event_info_ptr
);

#endif /* FEATURE_ELS_WIFIMEAS_LTE_U_PH1 || FEATURE_ELS_LWA || FEATURE_DATA_ANDSF*/
#endif /* DS_WLAN_MEAS_EVENT_DP_H */
