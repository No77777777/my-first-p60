#ifndef DS3GEVENTMGR_H
#define DS3GEVENTMGR_H
/*===========================================================================

                3 G   D A T A   S E R V I C E S   E V E N T   M A N A G E R

                           H E A D E R   F I L E

GENERAL DESCRIPTION
  This software unit contains functions for 3G Data Services event handling.
  It provides APIs for DS3G event registration and notification.

  Copyright (c) 2001-2018 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:
  $Header:

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/03/14   rs      Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "queue.h"
#include "ps_sys.h"
#include "ps_sys_event.h"
#include "ds3gsubsmgr.h"

#include "sys.h"
#include "sys_v.h"
#include "dstask_v.h"

#ifdef FEATURE_MMGSDI
#include "mmgsdisessionlib.h"
#endif /* FEATURE_MMGSDI */

#include "ds3g_data_call_activity.h"
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

#define DS3G_MMGSDI_ICCID_LEN        10
#define DS3G_MMGSDI_IMSI_M_LEN       10
#define DS3G_TECH_ALL                0x07

/*-------------------------------------------------------------------------
  Filter Structure, used for various filter options while publishing info.
-------------------------------------------------------------------------*/
typedef struct
{
  uint8           subs_mask;
  uint8           tech_type_mask[DS3GSUBSMGR_SUBS_ID_COUNT];
}ds3geventmgr_ds3g_filter_type;

/*------------------------------------------------------------------------
   Client id type
-------------------------------------------------------------------------*/
typedef enum
{
  DS3GEVENTMGR_CLIENT_ID_MIN       = 0x01,
  DS3GEVENTMGR_CLIENT_ID_3GPP2     = DS3GEVENTMGR_CLIENT_ID_MIN,
  DS3GEVENTMGR_CLIENT_ID_3GPP      = 0x02,
  DS3GEVENTMGR_CLIENT_ID_MPPM      = 0x04,
  DS3GEVENTMGR_CLIENT_ID_WLAN      = 0x08,
  DS3GEVENTMGR_CLIENT_ID_EPC       = 0X10,
  DS3GEVENTMGR_CLIENT_ID_ATCOP     = 0x20,
  DS3GEVENTMGR_CLIENT_ID_PROFILE   = 0x40,
  DS3GEVENTMGR_CLIENT_ID_KAMGR     = 0x80,
  DS3GEVENTMGR_CLIENT_ID_DATA_TEST = 0x100,
  DS3GEVENTMGR_CLIENT_ID_APPSRV    = 0x200,
  DS3GEVENTMGR_CLIENT_ID_DS3G      = 0x400,
  DS3GEVENTMGR_CLIENT_ID_DSD       = 0x800,
  DS3GEVENTMGR_CLIENT_ID_MAX       = DS3GEVENTMGR_CLIENT_ID_DSD
}ds3geventmgr_client_type;

/*-------------------------------------------------------------------------
  LPM POWER OFF Bitwise Configuration
-------------------------------------------------------------------------*/
typedef enum
{
  DS3GEVENTMGR_LPM_PWROFF_3GPP2_CTRL_BIT_MSK      = 0x01,
  DS3GEVENTMGR_LPM_PWROFF_3GPP_CTRL_BIT_MSK       = 0x02,
  DS3GEVENTMGR_LPM_PWROFF_WLAN_CTRL_BIT_MSK       = 0x04,
  DS3GEVENTMGR_LPM_PWFOFF_MASTER_CTRL_BIT_MSK     = 0x80
}ds3geventmgr_lpm_pwr_off_bit_msk_type;

/*-------------------------------------------------------------------------
  SUB_NOT_AVAIL Bitwise Configuration
-------------------------------------------------------------------------*/
typedef enum
{
  DS3GEVENTMGR_SUB_NOT_AVAIL_3GPP2_CTRL_BIT_MSK      = 0x01,
  DS3GEVENTMGR_SUB_NOT_AVAIL_3GPP_CTRL_BIT_MSK       = 0x02,
  DS3GEVENTMGR_SUB_NOT_AVAIL_MPPM_CTRL_BIT_MSK       = 0x04,
  DS3GEVENTMGR_SUB_NOT_AVAIL_WLAN_CTRL_BIT_MSK       = 0x08,
  DS3GEVENTMGR_SUB_NOT_AVAIL_MASTER_CTRL_BIT_MSK     = 0x80
}ds3geventmgr_sub_not_avail_bit_msk_type;

/*-------------------------------------------------------------------------
  DS3G External Events Enum Type for DS3G events
-------------------------------------------------------------------------*/
typedef enum
{
  DS3GEVENTMGR_INVALID_EV                   = -1,
  DS3GEVENTMGR_MIN_EV,
  DS3GEVENTMGR_LTE_ATTACH_INIT_EV           = DS3GEVENTMGR_MIN_EV,
  DS3GEVENTMGR_NULL_RESOLVED_APN_EV,
  DS3GEVENTMGR_CONSOLIDATED_RESOLVED_APN_EV,
  DS3GEVENTMGR_PDN_STATUS_INFO_EV,
  DS3GEVENTMGR_PDN_THROTTLE_INFO_EV,
  DS3GEVENTMGR_WLAN_MAX_CONN_EV,
  DS3GEVENTMGR_S101_STATUS_CHG_EV,
  DS3GEVENTMGR_EHRPD_PREREG_HANDOFF_COMPLETE_RESP_EV,
  DS3GEVENTMGR_MODE_LPM_EV,
  DS3GEVENTMGR_NV_REFRESH_EV,
#ifdef FEATURE_MMGSDI
  DS3GEVENTMGR_MMGSDI_SUBSCRIPTION_READY_EV,
  DS3GEVENTMGR_MMGSDI_REFRESH_VOTING_EV,
  DS3GEVENTMGR_MMGSDI_OK_REFRESH_EV,
  DS3GEVENTMGR_MMGSDI_REFRESH_FCN_EV,
  DS3GEVENTMGR_MMGSDI_CARD_REFRESH_COMPLETE_EV,
  DS3GEVENTMGR_MMGSDI_SESSION_CHANGED_EV,
  DS3GEVENTMGR_MMGSDI_CARD_ERROR_EV,
  DS3GEVENTMGR_MMGSDI_IMSI_INFO_EV,
  DS3GEVENTMGR_MMGSDI_ICCID_INFO_EV,
  DS3GEVENTMGR_MMGSDI_IMPI_INFO_EV,
  DS3GEVENTMGR_MMGSDI_IMSI_M_INFO_EV,
#endif /* FEATURE_MMGSDI */
  DS3GEVENTMGR_THERMAL_MITIGATION_EV,
  DS3GEVENTMGR_DATA_CHANGED_EV,
  DS3GEVENTMGR_DATA_ROAMING_CHANGED_EV,
  DS3GEVENTMGR_PLMN_LIST_CHG_EV,
  DS3GEVENTMGR_ROAMING_STATUS_EV,
  DS3GEVENTMGR_LATENCY_INFO_EV,
  DS3GEVENTMGR_WLAN_PROXY_INFO_CHG_EV,
  DS3GEVENTMGR_PROFILE_APN_NAME_CHG_EV,
  DS3GEVENTMGR_APM_NEW_ATTACH_PROFILE_EV,
  DS3GEVENTMGR_SUB_NOT_AVAIL_EV,
  DS3GEVENTMGR_APM_SET_ATTACH_PROFILE_EV,
  DS3GEVENTMGR_SUB_AVAIL_EV,
  DS3GEVENTMGR_RRC_STATE_CHANGED_EV,
  DS3GEVENTMGR_CDMA_CAPABILITY_EV,
  DS3GEVENTMGR_WLAN_IDI_TYPE_EV,
  DS3GEVENTMGR_WLAN_CAPABILITY_EV,
  DS3GEVENTMGR_IWLAN_S2B_THROTTLE_EV,
  DS3GEVENTMGR_EPC_INFO_CHG_EV,
  DS3GEVENTMGR_IWLAN_S2B_EPDG_ADDR_USABILITY_EV,
  DS3GEVENTMGR_PS_CALL_ORIG_EV,
  DS3GEVENTMGR_PS_CALL_END_EV,
  DS3GEVENTMGR_PS_CALL_ACTIVE_EV,
  DS3GEVENTMGR_PS_CALL_DORMANT_EV,
  DS3GEVENTMGR_PS_CALL_NO_FLOW_EV,
  DS3GEVENTMGR_RS_RA_EV,
  DS3GEVENTMGR_IPV6_IFACE_BRINGUP_EV,
  DS3GEVENTMGR_TCP_RETRANSMIT_EV,
  DS3GEVENTMGR_HIGH_PRIO_PS_CALL_EV,
  DS3GEVENTMGR_MO_DATA_RAB_REESTAB_EV,
  DS3GEVENTMGR_PLMN_CHG_EV,
  DS3GEVENTMGR_LTE_ATTACH_SM_INFO_EV,
  DS3GEVENTMGR_WLAN_PREF_IP_AVAILABLE_EV,
  DS3GEVENTMGR_IWLAN_UNAUTH_EMERGENCY_STATUS,
#ifdef FEATURE_LAPP 
  DS3GEVENTMGR_5G_APN_INFO_EV,
#endif
  DS3GEVENTMGR_TAC_OR_LAC_INFO_EV,
  DS3GEVENTMGR_EMERGENCY_CALL_STATUS_EV,
  DS3GEVENTMGR_VOICE_CALL_STATUS_EV,
  DS3GEVENTMGR_ESM_CAUSE_54_HO_FAILURE_EV,
  DS3GEVENTMGR_MODE_ONLINE_EV,
  DS3GEVENTMGR_DEVICE_SIM_MODE_CHANGE_EV,
  DS3GEVENTMGR_MAX_EV
}ds3geventmgr_event_type;

typedef enum
{
  DS3GEVENTMGR_EMC_IWLAN_INFO_CHG = 0,
  DS3GEVENTMGR_EMC_WIFI_INFO_CHG = 1,
  DS3GEVENTMGR_EMC_INFO_MAX
}ds3geventmgr_emc_info_chg_enum_type;

typedef struct
{
  ds3geventmgr_emc_info_chg_enum_type    wlan_info_chg;
}ds3geventmgr_emc_info_type;

typedef enum
{
    DS3GEVENTMGR_TH_MIT_MIN                       = 0,
    DS3GEVENTMGR_TH_MIT_ALLOW_ALL_CALLS           = DS3GEVENTMGR_TH_MIT_MIN,
    DS3GEVENTMGR_TH_MIT_ALLOW_ONLY_IMS_CALLS,
    DS3GEVENTMGR_TH_MIT_TEAR_DOWN_BLOCK_ALL_CALLS,
    DS3GEVENTMGR_TH_MIT_MAX
}ds3geventmgr_th_mit_action_type;

typedef enum
{
  DS3GEVENTMGR_RS_RA_START = 0,
  DS3GEVENTMGR_RS_RA_END   = 1
}ds3geventmgr_rs_ra_enum_type;

typedef enum
{
  DS3GEVENTMGR_IPV6_IFACE_UP_START = 0,
  DS3GEVENTMGR_IPV6_IFACE_UP_END   = 1
}ds3geventmgr_ipv6_iface_up_in_progress_enum_type;

typedef enum
{
  DS3GEVENTMGR_TCP_RETRANS_START = 0,
  DS3GEVENTMGR_TCP_RETRANS_END   = 1
}ds3geventmgr_tcp_retransmit_enum_type;

typedef enum
{
  DS3GEVENTMGR_HIGH_PRIO_PS_CALL_START = 0,
  DS3GEVENTMGR_HIGH_PRIO_PS_CALL_END   = 1
}ds3geventmgr_high_prio_ps_call_enum_type;

typedef enum
{
  DS3GEVENTMGR_MO_DATA_RAB_REESTAB_PROC_START = 0,
  DS3GEVENTMGR_MO_DATA_RAB_REESTAB_PROC_END   = 1
}ds3geventmgr_mo_data_rab_reestab_enum_type;

/*-------------------------------------------------------------------------
  Filter Structure, used for various filter options while publishing info.
-------------------------------------------------------------------------*/
typedef struct
{
  ps_sys_subscription_enum_type          ps_subs_id;
  ps_sys_tech_enum_type                  tech;
}ds3geventmgr_filter_type;

/*-------------------------------------------------------------------------
  Structure to contain attach prof information
-------------------------------------------------------------------------*/
typedef struct
{
  boolean                                     emergency_calls_supported;
  char                                        apn_name[DS_SYS_MAX_APN_LEN];
}ds3geventmgr_attach_prof_info_type;

/*-------------------------------------------------------------------------
  Structure to notify LTE attach Init info
-------------------------------------------------------------------------*/
typedef struct
{
  boolean                                     in_progress;
  ds3geventmgr_attach_prof_info_type          prof_info;
}ds3geventmgr_attach_info_type;

typedef struct
{
  mmgsdi_slot_id_enum_type slot;
}ds3geventmgr_nv_refresh_info_type;

#ifdef FEATURE_MMGSDI
typedef struct
{
  mmgsdi_session_id_type               session_id;
  mmgsdi_session_type_enum_type        session_type;
  mmgsdi_app_enum_type                 app_type;
  mmgsdi_slot_id_enum_type             slot;
  mmgsdi_session_changed_evt_info_type session_changed_info; 
}ds3geventmgr_mmgsdi_info_type;
#endif /* FEATURE_MMGSDI */

typedef struct
{
  ds3geventmgr_th_mit_action_type action;
}ds3geventmgr_thermal_mitigation_info_type;

typedef struct
{
  sys_plmn_list_type_e_type plmn_type;
}ds3geventmgr_plmn_list_chg_info_type;

typedef struct
{
  boolean                        is_roaming;
  ds_sys_roaming_type_enum_type  roaming_type;
  sys_sys_mode_e_type            sys_mode;
}ds3geventmgr_roaming_status_info_type;

typedef struct
{
  ps_sys_pdn_status_info_type ps_pdn_status;
  ps_sys_rat_ex_enum_type     rat;
  uint16                      profile_num;
}ds3geventmgr_pdn_status_info_type;

typedef ds_sys_3gpp_lte_attach_pdn_list_type ds3geventmgr_attach_pdn_list_type;

/*-------------------------------------------------------------------------
  Currently this enum only defines granular 3GPP RAT information. 
  It can be updated later to add granular 3GPP2 RAT information, if needed.
-------------------------------------------------------------------------*/
typedef enum
{
  DS3GEVENT_MGR_THROT_RAT_UNSPECIFIFED = 0,
  DS3GEVENT_MGR_THROT_RAT_LTE          = 1,
  DS3GEVENT_MGR_THROT_RAT_UMTS         = 2,
  DS3GEVENT_MGR_THROT_RAT_5G           = 3,
  DS3GEVENT_MGR_THROT_RAT_IWLAN        = 4,
  DS3GEVENT_MGR_THROT_RAT_MAX          = 5
} ds3gevent_mgr_throt_rat_type;

typedef enum
{
  DS3GEVENT_MGR_IDI_TYPE_IMSI = 0,
  DS3GEVENT_MGR_IDI_TYPE_IMPI = 1
}ds3gevent_mgr_idi_enum_type;

/*-------------------------------------------------------------------------
  This structure defines the PDN Throttling Information to be sent to
  Internal Clients within DS
-------------------------------------------------------------------------*/
typedef struct
{
 ps_sys_pdn_throttle_info_type  pdn_throttle_info;
                                  /* PDN Throttle Information*/
 ds3gevent_mgr_throt_rat_type   throttled_rat;  
                                  /* RAT for which given PDN Throttling Info
                                     is applicable. Only 3GPP MH and EPC
                                     need to populate this. Default value
                                     is DS3GEVENT_MGR_THROT_RAT_UNSPECIFIFED */
} ds3gevent_mgr_throttle_info_type;

typedef struct
{
  boolean            wlan_availability;
  ps_ip_addr_type    ipv4_addr;
  ps_ip_addr_type    ipv6_addr;

  ps_ip_addr_type    dns_v4_addr_1;
  ps_ip_addr_type    dns_v4_addr_2;
  ps_ip_addr_type    dns_v6_addr_1;
  ps_ip_addr_type    dns_v6_addr_2;
  char               ssid[PS_SYS_MAX_SSID_LEN];
  uint8              bssid[PS_SYS_MAC_ADDR_LEN];
  char               country_code[PS_SYS_MAX_COUNTRY_CODE_LEN];
  uint32             info_change_mask;
}ds3geventmgr_wlan_info_type;

typedef struct
{
  byte old_apn_name[DS_SYS_MAX_APN_LEN];
  byte new_apn_name[DS_SYS_MAX_APN_LEN];
}ds3gevent_mgr_apn_name_chg_info_type;

typedef struct
{
 sys_sys_mode_e_type                  sys_mode; 
 sys_wcdma_rrc_state_e_type           rrc_state;
}ds3gevent_mgr_rrc_state_type;

typedef struct
{
  boolean                 is_valid;
  nv_ehrpd_imsi_type      imsi_data;
  mmgsdi_app_enum_type    app_type;
}ds3geventmgr_imsi_info_type;

typedef struct
{
  boolean                 is_valid;
  sys_modem_as_id_e_type  subs_id;
  uint8                   iccid_len;
  uint8                   iccid[DS3G_MMGSDI_ICCID_LEN];
}ds3geventmgr_iccid_info_type;

typedef struct
{
  boolean                 cdma_capable;
}ds3geventmgr_cdma_capability_info_type;

typedef struct
{
  boolean                 is_valid;
  nv_ehrpd_imsi_type      impi_data;
}ds3geventmgr_impi_info_type;

typedef struct
{
  ds3gevent_mgr_idi_enum_type  idi_type;
}ds3geventmgr_idi_info_type;

typedef struct
{
  boolean                 is_valid;
  uint8                   imsi_m_data[DS3G_MMGSDI_IMSI_M_LEN];
  mmgsdi_app_enum_type    app_type;
}ds3geventmgr_imsi_m_info_type;

typedef struct
{
  boolean                 pref_ip_is_available;
}ds3geventmgr_wlan_pref_ip_available_info_type;

typedef struct
{
  boolean                 pdn_is_up;
}ds3geventmgr_iwlan_unauth_emergency_status;

typedef struct
{
  boolean                 wlan_capable;
}ds3geventmgr_wlan_capability_info_type;

typedef enum
{
  DS3GEVENT_MGR_IWLAN_S2B_THROTTLE_REASON_NONE = 0,
  DS3GEVENT_MGR_IWLAN_S2B_THROTTLE_REASON_L1   = 1,
  DS3GEVENT_MGR_IWLAN_S2B_THROTTLE_REASON_L2   = 2,
  DS3GEVENT_MGR_IWLAN_S2B_THROTTLE_REASON_L3   = 3,
  DS3GEVENT_MGR_IWLAN_S2B_THROTTLE_REASON_MAX  = DS3GEVENT_MGR_IWLAN_S2B_THROTTLE_REASON_L3,
}ds3geventmgr_iwlan_s2b_throttle_reason_enum_type;

typedef enum
{
  DS3GEVENT_CALL_STATE_MIN =0,
  DS3GEVENT_CALL_STATE_IDLE = DS3GEVENT_CALL_STATE_MIN,
  DS3GEVENT_CALL_STATE_CONNECTING = 1,
  DS3GEVENT_CALL_STATE_CONNECTED = 2,
  DS3GEVENT_CALL_STATE_MAX = DS3GEVENT_CALL_STATE_CONNECTED
} ds3geventmgr_call_state_enum_type;

typedef struct
{
  ds3geventmgr_call_state_enum_type status;
  sys_sys_mode_e_type               sys_mode;
} ds3geventmgr_call_status_info_type;

typedef struct
{
  uint8   num_active_apns;
  char    active_apn_name[PS_SYS_MAX_AVAIL_PDNS][PS_SYS_MAX_APN_LEN];
}ds3geventmgr_iwlan_s2b_level_throttle_info_type;

typedef union
{
  ds3geventmgr_iwlan_s2b_level_throttle_info_type level_throttle_info;
}ds3geventmgr_iwlan_s2b_throttle_reason_info_type;

typedef struct
{
  boolean                                          is_throttled;
  ds3geventmgr_iwlan_s2b_throttle_reason_enum_type throttle_reason;  
  ds3geventmgr_iwlan_s2b_throttle_reason_info_type throttle_reason_info;
}ds3geventmgr_iwlan_s2b_throttle_info_type;

typedef enum
{
  DS3GEVENT_MGR_IWLAN_S2B_EPDG_ADDR_USABILITY_NONE      = 0,
  DS3GEVENT_MGR_IWLAN_S2B_EPDG_ADDR_USABILITY_AVAILABLE = 1,
  DS3GEVENT_MGR_IWLAN_S2B_EPDG_ADDR_USABILITY_REACHABLE = 2,
  DS3GEVENT_MGR_IWLAN_S2B_EPDG_ADDR_USABILITY_MAX       =
                      DS3GEVENT_MGR_IWLAN_S2B_EPDG_ADDR_USABILITY_REACHABLE
}ds3geventmgr_iwlan_s2b_epdg_addr_usability_enum_type;

typedef struct
{
  ds3geventmgr_iwlan_s2b_epdg_addr_usability_enum_type  usability;  
}ds3geventmgr_iwlan_s2b_epdg_addr_usability_info_type;

typedef struct
{
  boolean                              is_last_non_ims_pdn;
  ps_sys_network_enum_type             tech;
  ds3g_data_call_activity_ps_call_type call_type;
}ds3geventmgr_ps_call_event_info_type;

typedef struct
{
  ds3geventmgr_rs_ra_enum_type  rs_ra_type;
}ds3geventmgr_rs_ra_event_info_type;

typedef struct
{
  uint32                     apn_string_len;
  char                       apn_string[DS_SYS_MAX_APN_LEN];
}ds3g_apn_name_type;


#ifdef FEATURE_LAPP 
typedef struct
{
  int                 num_apns;
  ds3g_apn_name_type  apn_names[DS_SYS_MAX_APNS];
}ds3geventmgr_5g_apn_info_type;
#endif /* FEATURE_LAPP */

typedef struct
{
  ds3geventmgr_ipv6_iface_up_in_progress_enum_type  ipv6_iface_up_type;
}ds3geventmgr_ipv6_iface_up_info_type;

typedef struct
{
  ds3geventmgr_tcp_retransmit_enum_type  tcp_retrans_type;
}ds3geventmgr_tcp_retrans_info_type;

typedef struct
{
  ds3geventmgr_high_prio_ps_call_enum_type  high_prio_ps_call_type;
}ds3geventmgr_high_prio_ps_call_info_type;
typedef struct
{
  ds3geventmgr_mo_data_rab_reestab_enum_type  mo_data_rab_reestab_type;
}ds3geventmgr_mo_data_rab_reestab_info_type;



typedef enum
{
  DS3GEVENT_MGR_TAC_INFO  = 0,
  DS3GEVENT_MGR_LAC_INFO  = 1
}ds3geventmgr_tac_or_lac_info_enum_type;

typedef struct
{
  sys_lac_type                            tac_or_lac;
  ds3geventmgr_tac_or_lac_info_enum_type  tac_or_lac_info;
}ds3geventmgr_tac_or_lac_info_type;

/*-------------------------------------------------------------------------
  Structure to notify events
-------------------------------------------------------------------------*/
typedef union
{
  ds3geventmgr_nv_refresh_info_type           nv_refresh_info;
  ds3geventmgr_mmgsdi_info_type               mmgsdi_info;
  ds3geventmgr_attach_info_type          lte_attach_info;
  ds3geventmgr_pdn_status_info_type           pdn_status_info;
  ds3gevent_mgr_throttle_info_type       pdn_throttle_info;
  ps_sys_wlan_max_conn_info_type         wlan_max_conn_info;
  ps_sys_null_resolved_apn_info_type     null_resolved_apn_info;
  ds3geventmgr_thermal_mitigation_info_type   thermal_mitigation_info;
  ps_sys_low_latency_traffic_info_type   latency_info;
  void                                  *event_payload_p;
  ds3geventmgr_plmn_list_chg_info_type        plmn_list_chg_info;
  ds3geventmgr_roaming_status_info_type       roaming_status_info;
  ds3geventmgr_wlan_info_type                 wlan_info;
  ds3gevent_mgr_apn_name_chg_info_type        apn_name_chg_info;
  ds3gevent_mgr_rrc_state_type                rrc_state_info;
  ds3geventmgr_imsi_info_type                 imsi_info;
  ds3geventmgr_iccid_info_type                iccid_info; 
  ds3geventmgr_cdma_capability_info_type      cdma_capability_info;
  ds3geventmgr_impi_info_type                 impi_info;
  ds3geventmgr_idi_info_type                  idi_info;
  ds3geventmgr_imsi_m_info_type               imsi_m_info;
  ds3geventmgr_wlan_capability_info_type      wlan_capability_info;
  ds3geventmgr_iwlan_s2b_throttle_info_type   iwlan_s2b_throttle_info;
  ds3geventmgr_iwlan_s2b_epdg_addr_usability_info_type 
                                        iwlan_s2b_epdg_addr_usability_info;
  ds3geventmgr_ps_call_event_info_type        ps_call_info;
  ds3geventmgr_rs_ra_event_info_type          rs_ra_info;
#ifdef FEATURE_LAPP
  ds3geventmgr_5g_apn_info_type               apn_5g_info;
#endif
  ds3geventmgr_ipv6_iface_up_info_type        ipv6_iface_up_info;
  ds3geventmgr_tcp_retrans_info_type          tcp_retrans_info;
  ds3geventmgr_high_prio_ps_call_info_type    high_prio_ps_call_info;
  ds3geventmgr_mo_data_rab_reestab_info_type  mo_data_rab_reestab_info;
}ds3geventmgr_event_info_type;

/*------------------------------------------------------------------------
   Clients Registers a callback function with DS3GCMIF module
   for events.
-------------------------------------------------------------------------*/
typedef void (*ds3geventmgr_notify_func_type)(
           ds3geventmgr_event_type       event_id,
           ds3geventmgr_filter_type      *ps_filter_info,
           void                          *event_info_ptr,
           void                          *data_ptr);
/*------------------------------------------------------------------------
DESCRIPTION   Returns Enum which is a mapping Subs ID to a Mask 
              (Single Sub at a time)
-------------------------------------------------------------------------*/
#define DS3GEVENTMGR_MAP_SUBS_ID_TO_MASK(subs_id, subs_mask)    \
    subs_mask = (DS3GSUBSMGR_SUBS_ID_MIN+1) << subs_id;

/*------------------------------------------------------------------------
DESCRIPTION   Returns Enum which is a mapping Subs Mask to Subs ID 
              (Single Sub at a time)
-------------------------------------------------------------------------*/
#define DS3GEVENTMGR_POP_SUBS_MASK_TO_ID(subs_id, subs_mask)    \
    subs_id = (ds3gsubsmgr_subs_id_e_type)(subs_mask >> 1);

/*------------------------------------------------------------------------
DESCRIPTION   Returns tech type mask with the tech input added
-------------------------------------------------------------------------*/
#define DS3GEVENTMGR_SET_TECH_TYPE_MASK(tech, tech_type_mask)    \
    tech_type_mask = 1 << tech;

/*------------------------------------------------------------------------
DESCRIPTION   Returns enum with tech type one at a time starting with
-------------------------------------------------------------------------*/
#define DS3GEVENTMGR_MAP_DS3G_TO_PS_TECH(ps_tech, tech_type_mask)    \
    ps_tech = (ps_sys_tech_enum_type)(tech_type_mask >> 1);

/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
/*===========================================================================
FUNCTION      DS3GEVENTMGR_INIT

DESCRIPTION   This function performs initialization of ds3geventmgr event q.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3geventmgr_init(void);

/*===========================================================================
FUNCTION      DS3GEVENTMGR_PROCESS_MCFG_REFRESH_IND_CMD

DESCRIPTION   This function process MCFG refresh ind command

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3geventmgr_process_mcfg_refresh_ind_cmd
(
  ds_cmd_type  *cmd_ptr
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

/*===========================================================================
FUNCTION      DS3GEVENTMGR_EVENT_REG

DESCRIPTION   Called by clients to register to get DS3GCMIF events.

DEPENDENCIES  None.

RETURN VALUE  TRUE: If registration successful.
              FALSE: If registration unsuccessful.

SIDE EFFECTS  Registered client is entered on a queue. 
 
NOTE          For device level events like 'DS3GEVENTMGR_MODE_LPM_EV' 
              cliets can register on any sub. Clients callback will
              be invoked whenever this event occurs, irrespective of
              the subs id on which clients register for this event.
              Clients should ignore the subs id in the filter info
              which is received as a part of callback and process
              this event. If clients are registered on all the subs, 
              the callback will be invoked only once.
===========================================================================*/
boolean ds3geventmgr_event_reg
(
  ds3geventmgr_event_type         event_id,
  ds3geventmgr_client_type        client_id,
  ds3geventmgr_filter_type       *filter_info_p,
  ds3geventmgr_notify_func_type   cb_func_ptr,
  void                           *cb_data_ptr
);

/*===========================================================================
FUNCTION      DS3G_EVENT_DEREG

DESCRIPTION   Called by clients to de-register DS3CMIF event notification.

DEPENDENCIES  None.

RETURN VALUE  TRUE: If de-registration successful.
                    If client previously de-registgered.

              FALSE: If de-registration unsuccessful.

SIDE EFFECTS  De-registered client is dequeued.
===========================================================================*/
boolean ds3geventmgr_event_dereg
(
  ds3geventmgr_client_type       client_id,
  ds3geventmgr_event_type        event_id,
  ds3geventmgr_filter_type      *filter_info_p
);
/*===========================================================================
FUNCTION      DS3GEVENTMGR_CHECK_ALL_TECH_TYPE_MASKS_INVALID

DESCRIPTION   Cycles through tech type mask for all subs and checks to see if 
              all tech type masks are invalid. 

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None
===========================================================================*/
boolean ds3geventmgr_check_all_tech_type_masks_invalid
(
  ds3geventmgr_ds3g_filter_type ds3g_filter_info
);
/*===========================================================================
FUNCTION      DS3GEVENTMGR_CLR_AND_NOTIFY_LPM_PWR_OFF_STATUS

DESCRIPTION   Sets the Incoming LPM/Power off Bit mask. If all the
              DS internal modules has finished processing successfully, 
              this function notifies CM that the processing is complete
 
DEPENDENCIES  None 

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3geventmgr_clr_and_notify_lpm_pwr_off_status
(
  ds3geventmgr_lpm_pwr_off_bit_msk_type bit_msk
);

/*===========================================================================
FUNCTION      DS3G_IS_LPM_PWR_OFF_MODE_PROCESSING_IN_PROGRESS

DESCRIPTION   returns whether DS processing of LPM/Power off mode is 
              still in progress (or) complete 
 
DEPENDENCIES  None 

RETURN VALUE  TRUE  --> LPM/PWR off processing still in progress 
              FALSE --> LPM/PWR Off processing complete/No LPM/PWR off
                        processing in progress

SIDE EFFECTS  None
===========================================================================*/
boolean ds3geventmgr_is_lpm_pwr_off_mode_processing_in_progress(void);

/*===========================================================================
FUNCTION      DS3GEVENTMGR_SET_LPM_PWR_OFF_STATUS

DESCRIPTION   Sets master control bit for LPM processing to indicate 
              DS processing of LPM/Power off mode has been initiated
 
DEPENDENCIES  None 

RETURN VALUE  TRUE  - If the status bit has been set successfully
              FALSE - if the status bit cannot be set

SIDE EFFECTS  None
===========================================================================*/
boolean ds3geventmgr_set_lpm_pwr_off_status
(
  ds3geventmgr_lpm_pwr_off_bit_msk_type status
);

/*===========================================================================
FUNCTION      DS3GEVENTMGR_SET_SUB_NOT_AVAIL_STATUS

DESCRIPTION   Sets the master control bit for SUB_NOT_AVAIL processing
              to indicate DS processing of SUB_NOT_AVAIL has been initiated
 
DEPENDENCIES  None 

RETURN VALUE  TRUE  - If the status bit has been set successfully
              FALSE - if the status bit cannot be set

SIDE EFFECTS  None
===========================================================================*/
boolean ds3geventmgr_set_sub_not_avail_status
(
  ds3geventmgr_sub_not_avail_bit_msk_type  client_id_mask,
  sys_modem_as_id_e_type                   subs_id
);

/*===========================================================================
FUNCTION      DS3GEVENTMGR_CLR_AND_NOTIFY_SUB_NOT_AVAIL_STATUS

DESCRIPTION   Sets the Incoming SUB_NOT_AVAIL Bit mask. If all the
              DS internal modules has finished processing successfully, 
              this function notifies CM that the processing is complete
 
DEPENDENCIES  None 

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3geventmgr_clr_and_notify_sub_not_avail_status
(
  ds3geventmgr_sub_not_avail_bit_msk_type  client_id_mask,
  sys_modem_as_id_e_type                   subs_id,
  uint8                                    subs_id_mask
);

/*===========================================================================
FUNCTION      DS3GEVENTMGR_IS_SUB_NOT_AVAIL_PROCESSING_IN_PROGRESS

DESCRIPTION   returns whether DS processing of SUBS_NOT_AVAIL is 
              still in progress (or) complete 
 
DEPENDENCIES  None 

RETURN VALUE  TRUE  --> SUB_NOT_AVAIL processing still in progress 
              FALSE --> SUB_NOT_AVAIL processing complete/No CARD REFRESH
                        processing in progress

SIDE EFFECTS  None
===========================================================================*/
boolean ds3geventmgr_is_sub_not_avail_processing_in_progress
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3GEVENTMGR_SET_AND_NOTIFY_NV_REFRESH_STATUS

DESCRIPTION   Sets the Incoming bit mask, if found valid. If all required 
              bits are set, then clear all bits and notify MCFG that NV
              refresh is complete. 
 
DEPENDENCIES  None 

RETURN VALUE  TRUE  - If the status bit has been set successfully
              FALSE - If the status bit cannot be set

SIDE EFFECTS  None
===========================================================================*/
boolean ds3geventmgr_set_and_notify_nv_refresh_status
(
  ds3geventmgr_client_type              client_id,
  sys_modem_as_id_e_type                subs_id
);

/*===========================================================================
FUNCTION      DS3GEVENTMGR_GET_CONF_INFO

DESCRIPTION   Called by clients to get cached information

DEPENDENCIES  None.

RETURN VALUE  TRUE: If operation was successful & payload was provided 
              FALSE: Otherwise 
 
CAVEAT        Information can be provided for 1 Subs/Tech Pair, if 
              mask has more than 1 bit set the request is rejected. 

SIDE EFFECTS  None
===========================================================================*/
boolean ds3geventmgr_get_conf_info
(
  ds3geventmgr_event_type                 event_id,
  void                                   *payload,
  ds3geventmgr_filter_type               *filter_info_p
);

/*===========================================================================
FUNCTION      DS3GEVENTMGR_SET_EVENT_INFO

DESCRIPTION   Called by clients to publish payload info to be given to 
              registered clients.

DEPENDENCIES  None.

RETURN VALUE  None. 
 
CAVEAT        Information can be provided for 1 Subs/Tech Pair, if 
              mask has more than 1 bit set the request is rejected. 

SIDE EFFECTS  None
===========================================================================*/
boolean ds3geventmgr_set_event_info
(
  ds3geventmgr_event_type                   event_id,
  void                                     *payload,
  ds3geventmgr_filter_type                 *filter_info_p
);

/*===========================================================================
FUNCTION      DS3GEVENTMGR_PERFORM_MASK_VALIDATION

DESCRIPTION   This function performs mask validation to check only 1 bit is
              set at a time since we notify/set/get payload one subs/Tech
              at a time.

DEPENDENCIES  None

RETURN VALUE  TRUE: If mask is valid as per the design
              FALSE: Otherwise

SIDE EFFECTS  None
===========================================================================*/
boolean ds3geventmgr_perform_mask_validation
(
  uint32  mask
);

/*===========================================================================
FUNCTION      DS3GEVENTMGR_NOTIGY_PS_CALL_EVENT_PER_SUB

DESCRIPTION   Called by clients to notify the ps call events.

DEPENDENCIES  None.

RETURN VALUE  None. 

SIDE EFFECTS  None
===========================================================================*/
void ds3geventmgr_notify_ps_call_event_per_sub
(
  sys_modem_as_id_e_type    subs_id,
  ds3geventmgr_event_type   ps_call_event,
  ps_sys_network_enum_type  tech
);

/*===========================================================================
FUNCTION      DS3GEVENTMGR_NOTIGY_PS_CALL_EVENT_PER_SUB_EX

DESCRIPTION   This extended function by clients to notify the ps call 
              events with some additional parameters.

DEPENDENCIES  None.

RETURN VALUE  None. 

SIDE EFFECTS  None
===========================================================================*/
void ds3geventmgr_notify_ps_call_event_per_sub_ex
(
  sys_modem_as_id_e_type               subs_id,
  ds3geventmgr_event_type              ps_call_event,
  ds3g_data_call_activity_ps_call_type call_type,
  boolean                              is_last_non_ims_pdn,
  ps_sys_network_enum_type             tech
);
/*=========================================================================
FUNCTION       DS3GEVENTMGR_NOTIFY_IPV6_IFACE_UP_EVENT

DESCRIPTION    This function notifies DS3GEVENTMGR_IPV6_IFACE_BRINGUP_EV
               while here input ipv6_iface_up_info can only be 
               DS3GEVENTMGR_IPV6_IFACE_UP_START

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
=========================================================================*/
void ds3geventmgr_notify_ipv6_iface_up_event
(
  ds_sys_subscription_enum_type                     ds_subs_id,
  ds3geventmgr_ipv6_iface_up_in_progress_enum_type  ipv6_iface_up_info
);
/*=========================================================================
FUNCTION       DS3GEVENTMGR_NOTIFY_TCP_RETRANS_EVENT

DESCRIPTION    This function notifies DS3GEVENTMGR_TCP_RETRANSMIT_EV to ds3g

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
=========================================================================*/
void ds3geventmgr_notify_tcp_retrans_event
(
  ds_sys_subscription_enum_type          ds_subs_id,
  ds3geventmgr_tcp_retransmit_enum_type  tcp_retrans_info
);

/*=========================================================================
FUNCTION       DS3GEVENTMGR_NOTIFY_HIGH_PRIO_PS_CALL_EVENT

DESCRIPTION    This function notifies DS3GEVENTMGR_HIGH_PRIO_PS_CALL_EV to ds3g

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
=========================================================================*/
void ds3geventmgr_notify_high_prio_ps_call_event
(
  sys_modem_as_id_e_type      subs_id,
  boolean                     high_prio_ps_call_start
);
/*=========================================================================
FUNCTION       DS3GEVENTMGR_NOTIFY_MO_DATA_RAB_REESTAB_EVENT

DESCRIPTION    This function notifies DS3GEVENTMGR_MO_DATA_RAB_REESTAB_EV to ds3g

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
=========================================================================*/
void ds3geventmgr_notify_mo_data_rab_reestab_event
(
  ds_sys_subscription_enum_type                ds_subs_id,
  ds3geventmgr_mo_data_rab_reestab_enum_type   mo_data_rab_reestab_info
);

/*===========================================================================
FUNCTION      DS3GEVENTMGR_CLEAR_SUB_NOT_AVAIL_STATUS

DESCRIPTION   This function resets the SUB_NOT_AVAIL status.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3geventmgr_clear_sub_not_avail_status
(
  sys_modem_as_id_e_type                   subs_id
);

/*===========================================================================
FUNCTION      DS3GEVENTMGR_CLEAR_LPM_PWR_OFF_STATUS

DESCRIPTION   This function resets the LPM_PWR_OFF status.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3geventmgr_clear_lpm_pwr_off_status
(
  void
);

/*===========================================================================
FUNCTION      ds3geventmgr_check_event_reg

DESCRIPTION   returns whether a client is registered for a given id with 
              specific filter information 
 
DEPENDENCIES  None 

RETURN VALUE  TRUE  - If registered
              FALSE - If Not

SIDE EFFECTS  None
===========================================================================*/
boolean ds3geventmgr_check_event_reg
(
  ds3geventmgr_client_type      client_id,
  ds3geventmgr_event_type       event_id,
  ds3geventmgr_filter_type     *ps_filter_info_p
);
/*===========================================================================
FUNCTION      DS3GEVENTMGR_MAP_DS3G_TO_PS_FILTER

DESCRIPTION   This function validates the DS3G filter info from the event queue
              and converts it to PS specific filter info.  Note that it can only
              take one subscription at a time as input, SUBS all will not work.

DEPENDENCIES  None

RETURN VALUE  TRUE: If validation was successful
              FALSE: Otherwise

SIDE EFFECTS  None
===========================================================================*/
boolean ds3geventmgr_map_ds3g_to_ps_filter
(
  ds3geventmgr_filter_type       *ps_filter_info_ptr,
  ds3geventmgr_ds3g_filter_type  *ds3g_filter_info_ptr
);
/*===========================================================================
FUNCTION      DS3GEVENTMGR_SET_MODE_ONLINE_EVT

DESCRIPTION   this func send DS3GEVENTMGR_MODE_ONLINE_EV

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3geventmgr_set_mode_online_evt
(
  sys_modem_as_id_e_type    subs_id
);
/*===========================================================================
FUNCTION      DS3GEVENTMGR_SET_DEVICE_SIM_MODE_CHANGE_EVT

DESCRIPTION   this func send DS3GEVENTMGR_DEVICE_SIM_MODE_CHANGE_EV

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3geventmgr_set_deive_sim_mode_change_evt
(
  sys_modem_as_id_e_type    subs_id
);

#endif /* DS3GEVENTMGR_H */
