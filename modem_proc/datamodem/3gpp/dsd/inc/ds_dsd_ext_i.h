#ifndef DS_DSD_EXT_I_H
#define DS_DSD_EXT_I_H
/*===========================================================================

                      DS_DSD_EXT_I_H

DESCRIPTION


EXTERNALIZED FUNCTIONS

 Copyright (c) 2009 - 2019 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/dsd/inc/ds_dsd_ext_i.h#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ---------------------------------------------------------- 
08/23/13    rs    Added support for additional WLAN information  
11/11/11    sk    Created Module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "dstask_v.h"
#include "sys.h"
#include "cm.h"
#include "ds_dsd_event_defs.h"
#include "ds3gsubsmgr.h"

#include "ps_sys.h"

/*===========================================================================

                          PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
    DSD Client Notify Reason
---------------------------------------------------------------------------*/
#define NOTIFY_REASON_SRV_STATUS_CHANGE     0x01
#define NOTIFY_REASON_SRV_DOMAIN_CHANGE     0x02
#define NOTIFY_REASON_RAT_CHANGE            0x04
#define NOTIFY_REASON_SO_CHANGE             0x08
#define NOTIFY_REASON_CELL_SRV_CHANGE       0x10
#define NOTIFY_REASON_WLAN_CHANGE           0x20
#define NOTIFY_REASON_SM_THROTTLE_CHANGE    0x40
#define NOTIFY_REASON_ROAM_STATUS_CHANGE    0x80
#define NOTIFY_REASON_NULL_BEARER_REASON_CHANGE 0x100

/*---------------------------------------------------------------------------
    DSD Client radio Enum type
---------------------------------------------------------------------------*/
typedef enum
{
  DS_DSD_RADIO_INVALID = 0,
  DS_DSD_RADIO_OOS = DS_DSD_RADIO_INVALID,
  DS_DSD_RADIO_LTE,
  DS_DSD_RADIO_MIN = DS_DSD_RADIO_LTE,
  DS_DSD_RADIO_HDR,
  DS_DSD_RADIO_1X,
  DS_DSD_RADIO_GSM, 
  DS_DSD_RADIO_TDSCDMA,
  DS_DSD_RADIO_WCDMA,
#ifdef FEATURE_LAPP
  DS_DSD_RADIO_5G,
#endif /* FEATURE_LAPP */
  DS_DSD_RADIO_WLAN, //WLAN must be the last one
  DS_DSD_RADIO_MAX=DS_DSD_RADIO_WLAN
}dsd_radio_type;

/*-------------------------------------------------------------------------
  Bit Masks to define Radio. Each client is a bit
-------------------------------------------------------------------------*/
#define DS_DSD_RADIO_MASK_NONE                0x00
#define DS_DSD_RADIO_MASK_LTE                 0x01
#define DS_DSD_RADIO_MASK_HDR                 0x02
#define DS_DSD_RADIO_MASK_1X                  0x04
#define DS_DSD_RADIO_MASK_GSM                 0x08
#define DS_DSD_RADIO_MASK_WCDMA               0x10
#define DS_DSD_RADIO_MASK_TDS                 0x20
#define DS_DSD_RADIO_MASK_WLAN                0x40
#ifdef FEATURE_LAPP
#define DS_DSD_RADIO_MASK_5G                  0x80
#endif /* FEATURE_LAPP */

/*---------------------------------------------------------------------------
    DSD Client Events Enum type
---------------------------------------------------------------------------*/
typedef enum
{
  DS_DSD_CLIENT_INVALID_EV = 0,

  DS_DSD_CLIENT_RADIO_ACQUIRED_EV,
  DS_DSD_CLIENT_RADIO_LOST_EV,
  DS_DSD_CLIENT_RADIO_EMMATTACHING_EV,
  DS_DSD_CLIENT_RADIO_EPS_CONTEXT_TX_EV,
  DS_DSD_CLIENT_RADIO_EMMATTACHCOMPLETED_EV,
  DS_DSD_CLIENT_RADIO_EMMDETACHCOMPLETED_EV,
  DS_DSD_CLIENT_RADIO_HDRSESSIONOPENED_EV,
  DS_DSD_CLIENT_RADIO_HDRSESSIONCLOSED_EV,
  DS_DSD_CLIENT_RADIO_DCTMTHROTTLED_EV,
  DS_DSD_CLIENT_RADIO_DCTMUNTHROTTLED_EV,
  DS_DSD_CLIENT_RADIO_HANDDOWN_EV,
  DS_DSD_CLIENT_RADIO_NETWORK_PARM_CHANGED_EV,
  DS_DSD_CLIENT_RADIO_CALLCONNECTED_EV,
  DS_DSD_CLIENT_RADIO_CALLENDED_EV,
  DS_DSD_CLIENT_RADIO_VOICE_CALL_ORIG_EV,
  DS_DSD_CLIENT_RADIO_VOICE_CALL_CONNECTED_EV,
  DS_DSD_CLIENT_RADIO_VOICE_CALL_ENDED_EV,
  DS_DSD_CLIENT_RADIO_EMERG_CALL_ORIG_EV,
  DS_DSD_CLIENT_RADIO_EMERG_CALL_STARTED_EV,
  DS_DSD_CLIENT_RADIO_EMERG_CALL_ENDED_EV,
  DS_DSD_CLIENT_RADIO_VOICE_CALL_UP_ON_OTHER_SUB_EV,
  DS_DSD_CLIENT_RADIO_VOICE_CALL_DOWN_ON_OTHER_SUB_EV,
  DS_DSD_CLIENT_RADIO_RAT_EXPANSION_EV,
  DS_DSD_CLIENT_RADIO_LTE_EX_SO_CHANGED_EV,
  DS_DSD_CLIENT_RADIO_ROAM_STATUS_CHANGED_EV,
  DS_DSD_CLIENT_RADIO_RETRY_DELAY_THROTTLED_EV,
  DS_DSD_CLIENT_RADIO_RETRY_DELAY_UNTHROTTLED_EV,
  DS_DSD_CLIENT_RADIO_ATTACH_PENDING_EV,
#ifdef FEATURE_LAPP
  DS_DSD_CLIENT_RADIO_5G_ACQUIRED_EV,
  DS_DSD_CLIENT_RADIO_5G_LOST_EV,

  DS_DSD_CLIENT_RADIO_MAX_EV =
    DS_DSD_CLIENT_RADIO_5G_LOST_EV
#else
  DS_DSD_CLIENT_RADIO_MAX_EV =
    DS_DSD_CLIENT_RADIO_ATTACH_PENDING_EV
#endif /* FEATURE_LAPP */
}ds_dsd_client_radio_event_type;


/*-----------------------------------------------------------------
    DSD Clients Enum type
    Please note: max number of clients is 7 because client_mask
    is defined as unit8
-----------------------------------------------------------------*/
typedef enum
{
  DS_DSD_EXT_CLIENT_INVALID = 0,
  DS_DSD_EXT_CLIENT_DATA_TEST,
  DS_DSD_EXT_CLIENT_EPC,
  DS_DSD_EXT_CLIENT_DATA,
  DS_DSD_EXT_CLIENT_ANDSF,
  DS_DSD_EXT_CLIENT_MAPCON,
  DS_DSD_EXT_CLIENT_WLAN_LB_MH,
  DS_DSD_EXT_CLIENT_IWLAN_MH,
  DS_DSD_EXT_CLIENT_MAX=DS_DSD_EXT_CLIENT_IWLAN_MH
}ds_dsd_ext_client_type;

/*-------------------------------------------------------------------
    DSD EXTERNAL EVENTS ENUM TYPE.
    CLIENTS can register to get these events
-------------------------------------------------------------------*/
typedef enum
{
  DS_DSD_EXT_INVALID_EV      = 0,
  DS_DSD_EXT_PREF_SYS_CHANGED_EV,
  DS_DSD_EXT_MAX_EV = DS_DSD_EXT_PREF_SYS_CHANGED_EV
}ds_dsd_ext_event_type;

/*---------------------------------------------------------------------------
  Indicate LTE ATTACH COMPLETE TYPE: initial or GW to L IRAT.
  Please note: 0 cannot be used as invalid attach type, since in some case,
               APM will receive attach complete with type 0 and APM should
               ignore it.
---------------------------------------------------------------------------*/
typedef enum
{
  DS_DSD_NORMAL_ATTACH      = 0x1,       /* UE perform LTE attach procedure*/
  DS_DSD_3GPP_IRAT_ATTACH   = 0x2        /* UE hand off from GW to L*/
}ds_dsd_attach_complete_type;

/*---------------------------------------------------------------------------
  ENUM to specify DSD LTE EX SO update type
---------------------------------------------------------------------------*/
typedef enum
{
  DS_DSD_LTE_EX_SO_DIV_DUPLEX_MSK = 0x01,
  DS_DSD_LTE_EX_SO_CA_MSK         = 0x02,
#ifdef FEATURE_DATA_4_5G
   DS_DSD_LTE_EX_SO_MOD_MIMO_MSK   = 0x04,
#endif
  DS_DSD_LTE_EX_SO_CONNECT_MODE_MSK = 0x08  //update so_mask when enterring or exitting connect mode
}ds_dsd_lte_ex_so_bit_msk_type;

/*---------------------------------------------------------------------------
  ENUM to specify DSD RESET reason
---------------------------------------------------------------------------*/
typedef enum
{
  DS_DSD_RESET_NV_REFRESH = 0,
  DS_DSD_RESET_MAX
}ds_dsd_reset_enum_type;

/*---------------------------------------------------------------------------
  Reset cmd info structure
---------------------------------------------------------------------------*/
typedef struct
{
  ds_dsd_reset_enum_type      reset_type;
  ds3gsubsmgr_subs_id_e_type  subs_id;
}ds_dsd_reset_cmd_info_type;

#ifdef FEATURE_DATA_WLAN_MAPCON
typedef struct
{
  ps_ip_addr_type           wlan_ipv4_addr1;
  ps_ip_addr_type           wlan_ipv4_addr2;
  ps_ip_addr_type           wlan_ipv6_addr1;
  ps_ip_addr_type           wlan_ipv6_addr2;
}wlan_addr_type;

typedef struct
{
  ps_ip_addr_type           wlan_ipv4_addr; 
  ps_ip_addr_type           wlan_ipv6_addr;
  uint8                     wlan_ipv6_prefix_len;
  uint8                     wlan_mac_addr[PS_SYS_MAC_ADDR_LEN];
  wlan_addr_type            wlan_dns_addr;
  wlan_addr_type            wlan_epdg_addr;
  char                      wlan_ssid[PS_SYS_MAX_SSID_LEN];
  uint8                     wlan_hessid[PS_SYS_MAX_HESSID_LEN];
  uint8                     country_code[PS_SYS_MAX_COUNTRY_CODE_LEN];
}ds_dsd_ext_wlan_addr_type;
#endif /* FEATURE_DATA_WLAN_MAPCON */

/*-------------------------------------------------------------------
  Auxillary information
-------------------------------------------------------------------*/
typedef union
{
  struct
  {
    uint32                  rat_mask;
    uint64                  so_mask;
    sys_srv_status_e_type   srv_status;
    boolean                 is_remote;
    sys_srv_domain_e_type   srv_domain;
  }attach_radio;

  struct
  {
    uint32                      rat_mask;
    uint32                      so_mask;
    ds_dsd_attach_complete_type attach_type;
  }emm_attach;

  struct
  {
    dsd_radio_type    source_radio;
    dsd_radio_type    target_radio;
  }hand_down;

  struct
  {
    uint32    rat_mask;
    uint32    so_mask;
  }hdr_session;

  struct
  {
    uint32  rat_mask;
    uint32  so_mask;
  }cdma_p_rev_ind;

  struct
  {
    cm_cell_srv_ind_s_type  cell_srv_ind;
  }cell_srv_info;

#ifdef FEATURE_DATA_WLAN_MAPCON
  struct
  {
    boolean                           is_wlan_available;
    boolean                           wqe_supported_flag;
    ps_sys_wlan_wqe_status_enum_type  wqe_supported;
    ps_ip_addr_type                   ipv4_addr;
    ps_ip_addr_type                   ipv6_addr;
    uint8                             ipv6_prefix_len;
    uint8                             ap_mac_addr[PS_SYS_MAC_ADDR_LEN];
    wlan_addr_type                    dns_addr;
    wlan_addr_type                    epdg_addr;
    char                              ssid[PS_SYS_MAX_SSID_LEN];
    uint8                             hessid[PS_SYS_MAX_HESSID_LEN];
    uint8                             country_code[PS_SYS_MAX_COUNTRY_CODE_LEN];
  }wlan_param_info;
#endif /* FEATURE_DATA_WLAN_MAPCON */

  struct
  {
    boolean    in_progress;
  }rat_expansion_info;

  /* LTE extended so mask info */
  struct
  {
    ds_dsd_lte_ex_so_bit_msk_type  lte_so_ex_bit_msk;                 
    sys_div_duplex_e_type          div_duplex;
    boolean                        ca_dl_available;
    boolean                        ca_ul_available;
#ifdef FEATURE_DATA_4_5G
    uint8                          num_configured_scells;
    boolean                        dl_256_qam;
    boolean                        ul_64_qam;
    boolean                        mimo_4_layers;
#endif /* #ifdef FEATURE_DATA_4_5G */
    boolean                        connect_mode;
  }lte_ex_so_info;

  struct
  {
    boolean                        roaming_status;
    ds_sys_roaming_type_enum_type  roaming_type;
  }roaming_status_info;

}ds_dsd_aux_info_type;

/*---------------------------------------------------------------------------
    DSD Event Info Struct. Used to notify radio event information
---------------------------------------------------------------------------*/
typedef struct
{
  ds_dsd_client_radio_event_type  event;
  ds3gsubsmgr_subs_id_e_type      subs_id;
  ds_dsd_aux_info_type            data;
}ds_dsd_event_info_type;

/*---------------------------------------------------------------------------
  DSD Radio Info Structure
---------------------------------------------------------------------------*/
typedef struct
{
  uint32                  changed_fields; /* changed fields mask */
  sys_sys_mode_e_type     system_mode;    /* System mode */
  sys_srv_status_e_type   srv_status;     /* Service status */
  sys_srv_domain_e_type   srv_domain;     /* Service domain */
  uint32                  rat_mask;       /* RAT mask info */
  uint64                  so_mask;        /* SO mask info */
  cm_cell_srv_ind_s_type  cell_srv_ind;   /* Cell serving indication */
  boolean                 remote_flag;    /* remote flag */
  boolean                 roaming_status; /* Roaming status */    

#ifdef FEATURE_DATA_WLAN_MAPCON
  /*--------------------------------------------------------------------
  Report WLAN information
  --------------------------------------------------------------------*/
  boolean                 is_wlan_available;
  ps_ip_addr_type         wlan_ipv4_addr; 
  ps_ip_addr_type         wlan_ipv6_addr;
  uint8                   wlan_ipv6_prefix_len;
  uint8                   wlan_mac_addr[PS_SYS_MAC_ADDR_LEN];
  wlan_addr_type          wlan_dns_addr;
  wlan_addr_type          wlan_epdg_addr;
  char                    wlan_ssid[PS_SYS_MAX_SSID_LEN];
  uint8                   wlan_hessid[PS_SYS_MAX_HESSID_LEN];
  uint8                   country_code[PS_SYS_MAX_COUNTRY_CODE_LEN];
#endif /* FEATURE_DATA_WLAN_MAPCON */
}ds_dsd_ext_radio_info_type;

/*---------------------------------------------------------------------------
  DSD Event Info Notification Structure

  Members:
  - pref_index: index corresponding to preferred system in dsd_radio_info array
  - changed_radio_mask: mask indicating changed radios in the radio_info array
  - dsd_radio_info: array of radio information
---------------------------------------------------------------------------*/
typedef struct
{
  ds_sys_subscription_enum_type  subs_id;
  uint8                          pref_index;
  uint32                         changed_radio_mask;
  boolean                        mode_pref_notify;
  ds_dsd_ext_radio_info_type     dsd_radio_info[DS_DSD_RADIO_MAX];
}ds_dsd_ext_event_info_type;

/*-------------------------------------------------------------------
    DSD Client Registers a callback function with DSD module
--------------------------------------------------------------------*/
typedef void (*ds_dsd_ext_notify_func_type)(
               ds_dsd_ext_event_type        event,
               ds_dsd_ext_event_info_type  *event_info_ptr,
               void                        *data_ptr);

/*-------------------------------------------------------------------
    Callback function to notify SS event processing complete
--------------------------------------------------------------------*/
typedef void (*ds_dsd_ext_notify_ss_event_complete_func_type)
             (
                ds3gsubsmgr_subs_id_e_type subs_id
             );

/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
FUNCTION       DS_DSD_EXT_EVENT_NOTIFY

DESCRIPTION    This is the external function called by external clients.
               The clients have to notify the DSD module by identifying
               themselves with the the publicly advertised API.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/
boolean ds_dsd_ext_event_notify
(
  dsd_radio_type           client_radio,
  ds_dsd_event_info_type  *event_info
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_EVENT_REG_CLIENT

DESCRIPTION   Called by clients to register to get DSD event notification.

DEPENDENCIES  None.

RETURN VALUE  TRUE: if registration successful
                    if client previously registgered

              FALSE: if registration unsuccessful

SIDE EFFECTS  Registered client is entered on a queue.
===========================================================================*/
boolean ds_dsd_ext_event_reg_client
(
  ds_dsd_ext_client_type        dsd_ext_client,
  ds_dsd_ext_event_type         dsd_ext_event,
  ds_dsd_ext_notify_func_type   dsd_ext_client_cb_func_ptr,
  void                         *data_ptr
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_EVENT_DEREG_CLIENT

DESCRIPTION   Called by clients to de-register for event notification.

DEPENDENCIES  None.

RETURN VALUE  TRUE: if de-registration successful
                    if client previously de-registgered

              FALSE: if de-registration unsuccessful

SIDE EFFECTS  De-registered client is dequeued.
===========================================================================*/
boolean ds_dsd_ext_event_dereg_client
(
  ds_dsd_ext_client_type        dsd_ext_client,
  ds_dsd_ext_event_type         dsd_ext_event
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_REG_SS_EVENT_PROCESS_COMPLETE

DESCRIPTION   Called by clients to register for SS event processing complete 
              by DSD. Currently only one callback registration is supported.

DEPENDENCIES  None.

RETURN VALUE  TRUE: if registration successful
              FALSE: if registration unsuccessful

SIDE EFFECTS  None.
===========================================================================*/
boolean ds_dsd_ext_reg_ss_event_process_complete
(
  ds_dsd_ext_notify_ss_event_complete_func_type  dsd_ext_client_cb_func_ptr
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_DEREG_SS_EVENT_PROCESS_COMPLETE

DESCRIPTION   Called by clients to de-register for SS event processing 
              complete by DSD.

DEPENDENCIES  None.

RETURN VALUE  TRUE: if de-registration successful
              FALSE: if de-registration unsuccessful

SIDE EFFECTS  None.
===========================================================================*/
boolean ds_dsd_ext_dereg_ss_event_process_complete(void);

/*===========================================================================
FUNCTION       DS_DSD_CMD_PROCESS

DESCRIPTION    This is the function called by dstask to dispatch any command
               posted to DSD module.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/
void ds_dsd_cmd_process
(
  ds_cmd_type  *cmd_ptr
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_TRANSLATE_SS_TO_DSD_RADIO

DESCRIPTION   This is a utility to translate DS RADIO type to SYS type
 
DEPENDENCIES  None.

RETURN VALUE  DSD Radio.

SIDE EFFECTS  None.
===========================================================================*/
dsd_radio_type  ds_dsd_ext_translate_ss_to_dsd_radio
(
  sys_sys_mode_e_type  ss_sys
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_TRANSLATE_DSD_SYS_TO_SS

DESCRIPTION   This is a utility to translate DS RADIO type to SYS type
 
DEPENDENCIES  None.

RETURN VALUE  CM SS type.

SIDE EFFECTS  None.
===========================================================================*/
sys_sys_mode_e_type ds_dsd_ext_translate_dsd_sys_to_ss
(
  dsd_radio_type  dsd_radio
);

/*===========================================================================
FUNCTION      ds_dsd_fmc_if_available

DESCRIPTION   This function notifies FMC availability to DSD

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_fmc_if_available
(
  boolean  fmc_available
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_EPC_NOTIFY

DESCRIPTION   This function is used to notify if EPC iface is up.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_ext_epc_notify
(
  boolean                     epc_present,
  ds3gsubsmgr_subs_id_e_type  subs_indx
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_NOTIFY_MODE_PREF_CHG

DESCRIPTION   This is the external function called by DS3GMGR to notify the 
              mode preference change to the DSD module.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_ext_notify_mode_pref_chg
(
  cm_mode_pref_e_type         mode_pref,
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_NOTIFY_SUBS_CAPABILITY_CHG

DESCRIPTION   This is the external function called by DS3GMGR to notify the 
              subs capability change to the DSD module.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_ext_notify_subs_capability_chg
(
  sys_sys_mode_mask_e_type    subs_capability,
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_NOTIFY_LTE_EMERG_CALL_IN_PROG

DESCRIPTION   This is the external function called by DS3GMGR to notify lte 
              emergency call starts or finishes to the DSD module.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_ext_notify_lte_emerg_call_in_prog
(
  ds3gsubsmgr_subs_id_e_type               subs_id,
  boolean                                  in_prog
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_NOTIFY_DEVICE_MODE

DESCRIPTION   This is the external function called by DS3GDSDIF to notify the 
              device mode to the DSD module.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_ext_notify_device_mode
(
  sys_modem_device_mode_e_type  device_mode
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_NOTIFY_DDS_CHANGE

DESCRIPTION   This is the external function called by DS3GDSDIF to notify the 
              Default Data Subscription change to the DSD module.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_ext_notify_dds_change
(
  ds3gsubsmgr_subs_id_e_type  def_data_subs
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_NOTIFY_PREF_SYSTEM_APM


DESCRIPTION   This is static function and called by lower registered element
              when system change is to be notified to external world.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_ext_notify_pref_system_apm
(
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_EXT_NOTIFY_NR5G_PROC

DESCRIPTION   This function notifies nr5g proc of attach complete.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/

void ds_dsd_ext_notify_nr5g_proc
(
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_PREF_SYSTEM_IND_COMPLETED


DESCRIPTION   This function is called by mode handler to notify it has 
              completed the parsing preferred system indication.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_ext_pref_system_ind_completed
(
  sys_sys_mode_e_type         sys_mode,
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_RESET

DESCRIPTION   Reset DSD statemachines. No need for clients to re-register 
              with DSD again.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS . Resets DSD statemachine.
===========================================================================*/
void ds_dsd_ext_reset
(
  ds3gsubsmgr_subs_id_e_type  subs_id,
  ds_dsd_reset_enum_type      reset_type
);

/*===========================================================================
FUNCTION        DS_DSD_EXT_NOTIFY_NV_REFRESH_IND

DESCRIPTION     Notify DSD about NV refresh indication.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.
===========================================================================*/
void ds_dsd_ext_notify_nv_refresh_ind
(
  sys_modem_as_id_e_type  subs_id
);


/*===========================================================================
FUNCTION      ds_dsd_ext_create_radio_mask_from_mode_pref_subs_cap

DESCRIPTION   This function reads the mode preference and and updates the 
              preferred order list accordingly.
  
DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  Preferred order list is updated based on mode preference.
===========================================================================*/
uint16 ds_dsd_ext_create_radio_mask_from_mode_pref_subs_cap
(
  cm_mode_pref_e_type         mode_pref,
  sys_sys_mode_mask_e_type    subs_capability
);

/*===========================================================================
FUNCTION        DS_DSD_INIT

DESCRIPTION     Entire DSD Module Initialization entry point. This function
                is called from dstask during power up.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    All the variables are initialized.
===========================================================================*/
void ds_dsd_init(void);

#ifdef FEATURE_LAPP
/*===========================================================================
FUNCTION      DS_DSD_EXT_NOTIFY_NR5G_ACQ_LOST

DESCRIPTION   This is the external function called by DS3GDSDNR5GPRCO to notify the 
              NR5G acquisition and loss to the DSD module.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_ext_notify_nr5g_acq_lost
(
  ds3gsubsmgr_subs_id_e_type  subs_id
);
#endif /* FEATURE_LAPP */

/*===========================================================================
FUNCTION      DS_DSD_EXT_GET_PREF_SYSTEM

DESCRIPTION   This function returns the preferred system.

DEPENDENCIES  None.

RETURN VALUE  DSD preferred system.

SIDE EFFECTS  None.
===========================================================================*/
dsd_radio_type ds_dsd_ext_get_pref_system
(
  ds3gsubsmgr_subs_id_e_type  subs_id
);


/*===========================================================================
FUNCTION      DS_DSD_EXT_GET_RAT_MASK_FRM_PREF_SYS

DESCRIPTION   This function returns the rat mask based on the 
                      preferred radio passed
 
DEPENDENCIES  None.

RETURN VALUE  Network mode.

SIDE EFFECTS  None.
===========================================================================*/
ps_sys_rat_ex_enum_type ds_dsd_ext_get_rat_mask_frm_pref_sys
(
  dsd_radio_type  preferred_radio
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_CONVERT_DSD_PRF_NW_TO_PS_PRF_NW

DESCRIPTION   This function returns the PS preferred network mode based on the 
              DSD preferred network passed
 
DEPENDENCIES  None.

RETURN VALUE  Network mode.

SIDE EFFECTS  None.
===========================================================================*/
ps_sys_network_enum_type ds_dsd_ext_convert_dsd_pref_nw_to_ps_pref_nw
(
  ds_dsd_preferred_network_type preferred_network 
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_GET_PRF_NW_FRM_PREF_SYS

DESCRIPTION   This function returns the preferred network mode based on the 
                      preferred radio passed
 
DEPENDENCIES  None.

RETURN VALUE  Network mode.

SIDE EFFECTS  None.
===========================================================================*/
ds_dsd_preferred_network_type ds_dsd_ext_get_pref_nw_frm_pref_sys
(
  dsd_radio_type  preferred_radio
);


/*===========================================================================
FUNCTION      DS_DSD_EXT_GET_OOS_TIMER_ID

DESCRIPTION   Utility function to get OOS timer id from dsd radio 

DEPENDENCIES  None.

RETURN VALUE  Timer id.

SIDE EFFECTS  None.
===========================================================================*/
uint8 ds_dsd_ext_get_oos_timer_id
(
  dsd_radio_type              dsd_radio
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_TIMER_RUNNING

DESCRIPTION   Utility to find if timer is running.

DEPENDENCIES  None.

RETURN VALUE  TRUE:     Timer running 
              FALSE:    Timer not running or Timer id not valid.

SIDE EFFECTS  None.
===========================================================================*/
boolean ds_dsd_ext_timer_running
(
  uint8                       dsd_timer_id,
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_TIMER_START_TIMER

DESCRIPTION   Starts the DSD timer for specified duration.

DEPENDENCIES  None.

RETURN VALUE  TRUE:     Timer started or Timer already running.
              FALSE:    Timer id not valid.

SIDE EFFECTS  Starts Timer.
===========================================================================*/
boolean ds_dsd_ext_timer_start_timer
(
  uint8                       dsd_timer_id,
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_TIMER_STOP_TIMER

DESCRIPTION   Stop the dsd Timer.

DEPENDENCIES  None.

RETURN VALUE  TRUE: Timer stopped or Timer already stopped.
              FALSE: Timer ID invalid.

SIDE EFFECTS  Stops timer.
===========================================================================*/
boolean  ds_dsd_ext_timer_stop_timer
(
  uint8                       dsd_timer_id,
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_COMMON_TRANSLATE_DSD_SYS_TO_SS

DESCRIPTION   This is a utility to translate DS RADIO type to SYS type
 
DEPENDENCIES  None.

RETURN VALUE  

SIDE EFFECTS  
===========================================================================*/
sys_sys_mode_e_type ds_dsd_common_translate_dsd_sys_to_ss(
  dsd_radio_type dsd_radio
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_NOTIFY_ATTACH_PENDING

DESCRIPTION 
  This function is used to set NULL bearer reason: ATTACH_PENDING and notify
  DSD to recompute best system and notify NULL bearer to the client.

PARAMETERS
  subs_id: subscription ID

DEPENDENCIES 
  None

RETURN VALUE 
  None

SIDE EFFECTS 
  None 
===========================================================================*/
void ds_dsd_ext_notify_attach_pending
(
  sys_modem_as_id_e_type  subs_id
);


#endif /* DS_DSD_EXT_I_H */
