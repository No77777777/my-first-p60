#ifndef DS_WLAN_MEAS_CMD_HDLR_H
#define DS_WLAN_MEAS_CMD_HDLR_H
/*===========================================================================
                      DS_WLAN_MEAS_CMD_HDLR.H

DESCRIPTION
 cmd hdlr header file for wlan meas.

EXTERNALIZED FUNCTIONS
 Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
05/01/15    Youjunf                          First version of file
===========================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"

#include "ps_sys.h"
#include "ps_sys_ioctl.h"
#include "ds_wlan_meas_event_dp.h"
#include "wcn_coex_mgr.h"

/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/

/*---------------------------------------------------------------------------
  Enum for wlan meas internal cmd types
---------------------------------------------------------------------------*/
typedef enum
{
  DS_WLAN_MEAS_CMD_ID_MIN,
  DS_WLAN_MEAS_CMD_WLAN_SYS_CHG = DS_WLAN_MEAS_CMD_ID_MIN,
  DS_WLAN_MEAS_CMD_PS_IOCTL_MEAS_CONFIG_STATUS,
  DS_WLAN_MEAS_CMD_PS_IOCTL_MEAS_REPORT,
  DS_WLAN_MEAS_CMD_EVENT_NOTIFY,
  DS_WLAN_MEAS_CMD_WIFI_VACATE_SRV_CAP,
  DS_WLAN_MEAS_CMD_PS_IOCTL_WIFI_BAND_VACATE,
  DS_WLAN_MEAS_CMD_LTEU_RRC_INFO,
  DS_WLAN_MEAS_CMD_ID_MAX
}ds_wlan_meas_cmd_id_enum_type;


/*-------------------------------------------------------------------------
wlan connection status type
-------------------------------------------------------------------------*/
typedef ps_sys_wlan_status_type  ds_wlan_meas_wlan_status_type;


typedef union
{
  ds_wlan_meas_wlan_status_type  wlan_status;
#if defined(FEATURE_ELS_WIFIMEAS_LTE_U_PH1) || defined(FEATURE_ELS_LWA) || defined(FEATURE_DATA_ANDSF)
  ps_sys_wlan_wifi_meas_report_config_status_type   ps_ioctl_meas_config_status;
  ps_sys_wlan_wifi_meas_report_type                 ps_ioctl_meas_report;
  ds_wlan_meas_event_notify_info_s_type             event_notify_info;
#endif /* FEATURE_ELS_WIFIMEAS_LTE_U_PH1 || FEATURE_ELS_LWA || FEATURE_DATA_ANDSF*/
  ps_sys_wifi_srv_cap_type                          srv_cap_info;
  ps_sys_ioctl_wifi_band_vacate_info_type           wifi_band_vacate_response;
  cxm_coex_lteu_rrc_info_s                          lteu_rrc_info;
}ds_wlan_meas_cmd_content_u_type;


/*---------------------------------------------------------------------------
 wlan meas internal cmd structure
---------------------------------------------------------------------------*/
typedef struct
{
  ds_wlan_meas_cmd_id_enum_type    cmd_id;
  ds_wlan_meas_cmd_content_u_type  cmd_content;
}ds_wlan_meas_cmd_type;

/*===========================================================================
            EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/

/*===========================================================================
FUNCTION      DS_WLAN_MEAS_CMD_INIT

DESCRIPTION   This function initializes wlan meas cmd module.

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_wlan_meas_cmd_init(void);


/*===========================================================================
FUNCTION DS_WLAN_MEAS_CMD_POST

DESCRIPTION
  Posts a wlan meas internal cmd.

DEPENDENCIES
  None

PARAMETERS
  cmd_id          - id of the command to be posted
  cmd_content_ptr - pointer to the cmd content to be send along with the DS cmd


RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_wlan_meas_cmd_post(ds_wlan_meas_cmd_id_enum_type   cmd_id,
                           void                            *cmd_content_ptr);




/*===========================================================================
FUNCTION DS_WLAN_MEAS_PROCESS_WLAN_SYS_CHG_CMD

DESCRIPTION
  This function process the DS_WLAN_MEAS_CMD_WLAN_SYS_CHG cmd

PARAMETERS
  wlan_status_ptr: wlan status pointer

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None.
===========================================================================*/
void ds_wlan_meas_process_wlan_sys_chg_cmd
(
  ds_wlan_meas_wlan_status_type *wlan_status_ptr
);



#if defined(FEATURE_ELS_WIFIMEAS_LTE_U_PH1) || defined(FEATURE_ELS_LWA) || defined(FEATURE_DATA_ANDSF)
/*===========================================================================
FUNCTION DS_WLAN_MEAS_PROCESS_EVENT_NOTIFY_CMD

DESCRIPTION
  This function process the DS_WLAN_MEAS_CMD_EVENT_NOTIFY cmd

PARAMETERS
  notify_event_info_ptr: notified event info pointer

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None.
===========================================================================*/
void ds_wlan_meas_process_event_notify_cmd
(
  ds_wlan_meas_event_notify_info_s_type *notify_event_info_ptr
);

void ds_wlan_meas_process_ps_iotcl_meas_report_cmd
(
  ps_sys_wlan_wifi_meas_report_type *per_ssid_meas_report_ptr
);

void ds_wlan_meas_process_ps_ioctl_meas_config_status_cmd
(
  ps_sys_wlan_wifi_meas_report_config_status_type *meas_config_status_ptr
);
#endif /* FEATURE_ELS_WIFIMEAS_LTE_U_PH1 || FEATURE_ELS_LWA || FEATURE_DATA_ANDSF*/

void ds_wlan_meas_process_wifi_vacate_srv_cap_cmd
(
  ps_sys_wifi_srv_cap_type* srv_cap_info
);

void ds_wlan_meas_process_wifi_band_vacate_response_cmd
(
  ps_sys_ioctl_wifi_band_vacate_info_type* wifi_band_vacate_response
);

void ds_wlan_meas_wifi_vacate_lteu_rrc_info_cmd
(
  cxm_coex_lteu_rrc_info_s* lteu_rrc_info
);

#endif /* DS_WLAN_MEAS_CMD_HDLR */
