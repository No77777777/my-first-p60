/**
@file ds_sys_event.h
@brief
This file defines functions, enums and structs use to register for events
*/

#ifndef DS_SYS_EVENT_H
#define DS_SYS_EVENT_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        D S _ S Y S _ E V E N T . H


GENERAL DESCRIPTION
  This file defines functions, enums and structs used to register for an event

Copyright (c) 2011-2015 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/api/public/ds_sys_event.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/09/11    bvd    Create module

===========================================================================*/


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "dserrno.h"

#include "ds_sys.h"
#include "ps_iface_defs.h"
#include "ps_in.h"

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/
/**
  @brief Defines different events
*/
typedef enum
{
  DS_SYS_EVENT_MIN                       = 0x00,

  /*-------------------------------------------------------------------------
    3GPP2 EVENTS
  -------------------------------------------------------------------------*/
  DS_SYS_EVENT_3GPP2_MIN                             = DS_SYS_EVENT_MIN,
  DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_STATUS = DS_SYS_EVENT_3GPP2_MIN,
  DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_CHANGE = DS_SYS_EVENT_3GPP2_MIN + 1,
  DS_SYS_EVENT_3GPP2_OUTAGE_NOTIFICATION = DS_SYS_EVENT_3GPP2_MIN + 2,
  /**<
       Event is triggered when there is a HDR outage notifcation.
  */
  DS_SYS_EVENT_3GPP2_QOS_NETWORK_STATUS_CHANGE = DS_SYS_EVENT_3GPP2_MIN + 3,
  /**<
       Event is triggered when there is a change in the QOS nework status.
  */

  DS_SYS_EVENT_3GPP2_MAX                             = 200,

  /*-------------------------------------------------------------------------
    3GPP EVENTS
  -------------------------------------------------------------------------*/
  DS_SYS_EVENT_3GPP_MIN                  = 400,
  DS_SYS_EVENT_3GPP_LTE_ATTACH_PDN_LIST_CHANGE  = 
    DS_SYS_EVENT_3GPP_MIN + 1,
  /**<        
       Event is triggered upon lte attach pdn list write completion to the
       EFS using ioctl DS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST.
       Along with the event, the list of profile IDs are sent by MH.
       User Registers if interested to know that the LTE attach PDN list has
       changed.
       @see DS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST
  */
  DS_SYS_EVENT_3GPP_EMERGENCY_PDN_STATUS = DS_SYS_EVENT_3GPP_MIN + 2,
  /**<        
       Event is triggered if the emergency pdn is the only pdn present
       (with a flag set to true). The event is also triggered if the emergency 
       pdn is not the only pdn present (with a flag set to false). User can 
       register for this event to know the emergency pdn status.  
  */
  DS_SYS_EVENT_3GPP_ROAMING_DISALLOWED_INFO = DS_SYS_EVENT_3GPP_MIN + 3,
  /**<
       Event is triggered whenever the mode handler sends the list of apn names.
       Client gets to know the apns on which the client will not attempt to make call.
       Any active call on those apns will be torn down.

  */
  
  DS_SYS_EVENT_3GPP_LTE_CELL_INFO_CHANGE = DS_SYS_EVENT_3GPP_MIN + 4,
  /**<
       Event is triggered whenever the mode handler sends the change in
       primary or secondary cell information for LTE RAT.

  */  

  DS_SYS_EVENT_3GPP_DATA_CONNECTION_STATUS_CHANGE = DS_SYS_EVENT_3GPP_MIN + 5,
  /**<
       Event is triggered by 3GPP Modehandler whenever data connection status
       changes for any PDN
  */  
  
  DS_SYS_EVENT_3GPP_LTE_CELL_HANDOVER_COMPLETE = DS_SYS_EVENT_3GPP_MIN + 6,
  /**<
       Event is triggered whenever RRC notifies DS about LTE 
       cell handover completion from one cell to another.

  */

  DS_SYS_EVENT_3GPP_PS_DATA_AVAIL_CHANGE         = DS_SYS_EVENT_3GPP_MIN + 7,
  /**< 
     Event is triggered during bootup if there is default list that needs to be 
     published or when card is read and it has updated list or during OMA DM process.
     All the client shall take appropriate action when PS Data is turned off by
     checking against information from event payload.
  */      
  
  DS_SYS_EVENT_3GPP_MAX                  = 500,

  /*-------------------------------------------------------------------------
    COMMON EVENTS
  -------------------------------------------------------------------------*/
  DS_SYS_EVENT_COMMON_MIN                = 700,
  DS_SYS_EVENT_SYSTEM_STATUS_CHANGE      = DS_SYS_EVENT_COMMON_MIN,
  /**<        
       Event is triggered if UE camps on  to a different network or 
       RAT/SO is changed within the same network. In response of event, User
       gets to know which network is on right now. When user receives the event,
       it knows which network is UE on. User can register whenever they need to
       know network information.
  */

  DS_SYS_EVENT_DUN_CTRL_CALL_STATUS      = 
    DS_SYS_EVENT_COMMON_MIN + 1,
  /**< 
     Connection manager doesnt have to register for this event, registeration
     happens internally when connection manager sets
     DS_SYS_CONF_DUN_CTRL_PREF_ENTITLEMENT_CHECK_MASK and provides
     callback during set operation. Callback is recieved whenever dun call
     is made. User recieves callid of the call, whether call was success or
     failure and failure reason if call didnt go through in the event info
  */

  DS_SYS_EVENT_DUN_CTRL_ENTITLEMENT_CHECK   = 
    DS_SYS_EVENT_COMMON_MIN + 2,
  /**< 
     Connection manager doesnt have to register for this event, registeration
     happens internally when connection manager sets
     DS_SYS_CONF_DUN_CTRL_PREF_CALL_STATUS_NOTIFICATION_MASK and provides
     callback during set operation. Callback is recieved whenever attempt for 
     dun call is made to check with connection manager whether to allow ro
     disallow call. User recieves callid of the call and tech type in the
     event info
  */

  DS_SYS_EVENT_DUN_CTRL_SILENT_REDIAL_CHECK = 
    DS_SYS_EVENT_COMMON_MIN + 3,
  /**< 
     Connection manager doesnt have to register for this event, registeration
     happens internally when connection manager sets
     DS_SYS_CONF_DUN_CTRL_PREF_SILENT_REDIAL_CHECK_MASK and provides
     callback during set operation. Callback is recieved whenever dun call
     fails and modehandler can ask the connection manager to fix the issue.
     User recieves callid of the call, failure reason in the event info
  */

  DS_SYS_EVENT_DATA_CALL_STATUS_CHANGE        = DS_SYS_EVENT_COMMON_MIN + 4,
  /**< 
     User can register for this event whenever they want to know regarding
     call status (estabilshed or terminated) for different type of call.
     Event is triggered whenever the call goes into established or termniated
     state.
  */

  DS_SYS_EVENT_SYSTEM_STATUS_EX               = DS_SYS_EVENT_COMMON_MIN + 5,
  /**<        
       Event is triggered if UE camps on  to a different network or 
       RAT/SO is changed within the same network. In response of event, User
       gets to know which network is on right now. When user receives the event,
       it knows which network is UE on. User can register whenever they need to
       know system status information.
  */

  DS_SYS_EVENT_UPDATED_THROUGHPUT_TIMER      = DS_SYS_EVENT_COMMON_MIN + 6,
  /**<        
       Event is triggered whenever the timer for throughput is configured by
       the user using DS_SYS_CONF_THROUGHPUT_INFO_TIMER.
       This timer is maintained by the Mode Handler.
  */

  DS_SYS_EVENT_THROUGHPUT_INFO              = DS_SYS_EVENT_COMMON_MIN + 7,
  /**<        
       Event is triggered whenever the timer for throughput configured by
       the user expired and generates throughput info for all active
       PDNs. This timer is set using DS_SYS_CONF_THROUGHPUT_INFO_TIMER.
  */

  DS_SYS_EVENT_START_FAST_DORMANCY       = DS_SYS_EVENT_COMMON_MIN + 8,
  /**<
      This event is triggered whenever mode handler gives an indication to PS 
      to START fast dormancy algorithm
  */

  DS_SYS_EVENT_STOP_FAST_DORMANCY        =  DS_SYS_EVENT_COMMON_MIN + 9,
  /**<
      This event is triggered whenever mode handler gives an indication to PS 
      to STOP fast dormancy algorithm
  */
  
   DS_SYS_EVENT_UPDATED_DL_THROUGHPUT_TIMER    = DS_SYS_EVENT_COMMON_MIN + 10,
   /**<        
        Event is triggered whenever the timer for throughput is configured by
        the user using DS_SYS_CONF_DL_THROUGHPUT_INFO_TIMER.
        This timer is maintained by the Mode Handler.
   */

   DS_SYS_EVENT_DL_THROUGHPUT_INFO              = DS_SYS_EVENT_COMMON_MIN + 11,
   /**<        
        Event is triggered whenever the timer for throughput configured by
        the user expired and generates throughput info for all active
        PDNs. This timer is set using DS_SYS_CONF_DL_THROUGHPUT_INFO_TIMER.
   */

  DS_SYS_EVENT_THERMAL_MITIGATION_INFO      =  DS_SYS_EVENT_COMMON_MIN + 12,
  /**<
      This event is triggered whenever DS3g indicates a thermal level change
  */

 DS_SYS_EVENT_DATA_SERVICE_SETTING        =  DS_SYS_EVENT_COMMON_MIN + 13,
  /**<
      This event is triggered by the framework whenever application updates
      the data service setting.
  */

  DS_SYS_EVENT_DATA_SERVICE_ROAMING_SETTING  =  DS_SYS_EVENT_COMMON_MIN + 14,
  /**<
      This event is triggered by the framework whenever application updates
      the data service roaming setting.
  */

  DS_SYS_EVENT_APN_INFO                      =  DS_SYS_EVENT_COMMON_MIN + 15,
  /**<
      This event is triggered by the framework whenever application updates
      the apn info.
  */

  DS_SYS_EVENT_RAT_PREFERENCE               =  DS_SYS_EVENT_COMMON_MIN + 16,
  /**<
      This event is triggered by the framework whenever application updates
      the rat preference.
  */

  DS_SYS_EVENT_UPDATED_DL_THROUGHPUT_INTERVAL_STATUS_CHANGE   = DS_SYS_EVENT_COMMON_MIN + 17,
  /**<        
       Event is triggered whenever MH notifies change in throughput interval
       status using DS_SYS_CONF_DL_THROUGHPUT_INTERVAL_STATUS_CHANGE.
  */

  DS_SYS_EVENT_UPDATED_UL_THROUGHPUT_CONFIG    = DS_SYS_EVENT_COMMON_MIN + 18,
  /**<        
       Event is triggered whenever the timer for throughput is configured by
       the user using DS_SYS_CONF_UL_THROUGHPUT_CONFIG.
       This timer and queue size is maintained by the Mode Handler.
  */

  DS_SYS_EVENT_UL_THROUGHPUT_INTERVAL_STATUS_CHANGE    = DS_SYS_EVENT_COMMON_MIN + 19,
  /**<        
       Event is triggered whenever the timer for throughput is configured by
       the user using DS_SYS_CONF_UL_THROUGHPUT_CONFIG.
       This timer is maintained by the Mode Handler.
  */

  DS_SYS_EVENT_UL_THROUGHPUT_INFO    = DS_SYS_EVENT_COMMON_MIN + 20,
  /**<        
       Event is triggered whenever the timer for throughput is configured by
       the user using DS_SYS_CONF_UL_THROUGHPUT_CONFIG.
       This timer is maintained by the Mode Handler.
  */

  DS_SYS_EVENT_WIFI_SETTINGS_CHANGE    = DS_SYS_EVENT_COMMON_MIN + 21,
  /**<        
       Event is triggered whenever the wifi settings are changed by 
       the user using DS_SYS_CONF_DATA_SERVICE_WIFI_SETTING.
  */


  DS_SYS_EVENT_QUALITY_MEAS_POLICY_CHANGE = DS_SYS_EVENT_COMMON_MIN + 22,
  /**< 
     Event is triggered whenver there is a policy update for the quality measurements
     for technologies by the application. 
  */

  DS_SYS_EVENT_DATA_ACTIVITY_RESUMED    = DS_SYS_EVENT_COMMON_MIN + 23,
  /**<        
       Event is triggered whenever there is data activity once after 
       the user registration. to avoid back to back callback invocation
       clients are notified only once after the registration.however 
       clients must deregister immediately after recieving the event.
  */

  DS_SYS_EVENT_ROAMING_STATUS_CHANGE = DS_SYS_EVENT_COMMON_MIN + 24,
  /**<        
       Event is triggered when the UE moves away from the home network and is roaming.
       User registers for this event to know if the UE is in the home network or roaming.
  */    

  DS_SYS_EVENT_APN_PCO_INFO_CHANGE = DS_SYS_EVENT_COMMON_MIN + 25,
  /**<        
       Event is triggered when PCO information changes for given APN. User passes 
       the APN name when registering for this event.
  */   

  DS_SYS_EVENT_APN_MSISDN_INFO_CHANGE = DS_SYS_EVENT_COMMON_MIN + 26,
  /**<        
       Event is triggered when MSISDN information changes for given APN. User passes 
       the APN name when registering for this event.
  */ 

  DS_SYS_EVENT_LOW_LATENCY_TRAFFIC_STATUS = DS_SYS_EVENT_COMMON_MIN + 27,
  /**<        
       Event is triggered to notify low latency traffic start and end
  */   

  DS_SYS_EVENT_PDN_THROTTLE_INFO         = DS_SYS_EVENT_COMMON_MIN + 28,
  /**< 
     Event is triggered whenver there is a throttling info for technologies by the application. 
  */

  DS_SYS_EVENT_CA_CERT_RETRIEVAL_SUPPORT = DS_SYS_EVENT_COMMON_MIN + 29,
  /**< 
       Event is triggered when the TE that supports CA certificate retrieval 
       registers/deregisters with modem.  
  */

  DS_SYS_EVENT_CURRENT_DDS_IND = DS_SYS_EVENT_COMMON_MIN + 30,
  /**< 
      Event is triggered when dds switch is done from one subscription to another
  */ 

  DS_SYS_EVENT_COMMON_MAX                = 800,

  DS_SYS_EVENT_WLAN_MIN                  = 900,
  DS_SYS_EVENT_WLAN_MAC_ADDR_CHANGE      = DS_SYS_EVENT_WLAN_MIN,
  /**< 
   User can register for this event whenever they want to know regarding
   change in WLAN MAC Address. 
   @see DS_SYS_CONF_GET_WLAN_MAC_ADDR
  */

  DS_SYS_EVENT_WLAN_SSID_CHANGE      = DS_SYS_EVENT_WLAN_MIN + 1,
  /**< 
   User can register for this event whenever they want to know regarding
   change in WLAN SSID. Service set identification(SSID) is a string which
   represents the network name. 
   @see DS_SYS_CONF_GET_WLAN_SSID
  */
  
  DS_SYS_EVENT_WLAN_IP_ADDR_CHANGE   = DS_SYS_EVENT_WLAN_MIN + 2,
  /**< 
   User can register for this event whenever they want to know regarding
   change in WLAN IP ADDR.
   
   @see DS_SYS_CONF_GET_WLAN_IP_ADDR
  */  

  DS_SYS_EVENT_WQE_PROFILES_MET      =  DS_SYS_EVENT_WLAN_MIN + 3,
  /**<
      This event is triggered by the framework whenever application updates
      the WQE profiles that were met.
  */

  DS_SYS_EVENT_WQE_PROFILES_STATUS   = DS_SYS_EVENT_WLAN_MIN + 4,
  /**<
      This event is triggered by the framework whenever application updates
      the WQE profiles with their statuses.
  */

  DS_SYS_EVENT_WLAN_HESSID_CHANGE   = DS_SYS_EVENT_WLAN_MIN + 5,
  /**< 
   User can register for this event whenever they want to know regarding
   change in WLAN HESSID. 
  */

  DS_SYS_EVENT_WLAN_MAX                  = 1000,

  DS_SYS_EVENT_MAX
} ds_sys_event_enum_type;

/**
  @brief Enum of type page monitor period result 
  @see   ds_sys_event_3gpp2_evdo_page_monitor_period_status_info_type 
*/
typedef enum
{
  DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_RESULT_SUCCESS,
  DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_RESULT_FAIL_REQUEST_REJECTED,
  DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_RESULT_FAIL_REQUEST_FAILED_TX,
  DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_RESULT_FAIL_NOT_SUPPORTED
} ds_sys_event_3gpp2_evdo_page_monitor_period_status_enum_type;

/**
  @brief Event info type for DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_STATUS
         event
  @see   DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_STATUS
*/
typedef struct
{
  ds_sys_event_3gpp2_evdo_page_monitor_period_status_enum_type status;
} ds_sys_event_3gpp2_evdo_page_monitor_period_status_info_type;

/**
  @brief Event info type for DS_SYS_EVENT_DUN_CTRL_CALL_STATUS 
  @see   DS_SYS_EVENT_DUN_CTRL_CALL_STATUS
  */
typedef struct
{
  ds_sys_tech_enum_type          tech_type;  /**< Different technologies */
  unsigned char                  cb_data;    /**< Callback data sent by
                                                  Modehandler */
  boolean                        is_call_established; /**< Call success or
                                                          failure */
  ps_iface_net_down_reason_type  net_down_reason; /**< Call failure reason */
} ds_sys_event_dun_ctrl_call_status_info_type;

/**
  @brief Event info type for DS_SYS_EVENT_DUN_CTRL_ENTITLEMENT_CHECK 
  @see   DS_SYS_EVENT_DUN_CTRL_ENTITLEMENT_CHECK
*/
typedef struct
{
  ds_sys_tech_enum_type        tech_type;  /**< Different technologies */
  unsigned char                cb_data;    /**< Callback data sent by 
                                                Modehandler */
} ds_sys_event_dun_ctrl_entitlement_check_info_type;


/**
  @brief Event info type for DS_SYS_EVENT_DUN_CTRL_SILENT_REDIAL_CHECK 
  @see   DS_SYS_EVENT_DUN_CTRL_SILENT_REDIAL_CHECK
*/
typedef struct
{
  ds_sys_tech_enum_type          tech_type;      /**< Different technologies */
  unsigned char                  cb_data;        /**< Callback data sent by 
                                                       Modehandler */
  ps_iface_net_down_reason_type  net_down_reason; /**< Call failure reason */
} ds_sys_event_dun_ctrl_silent_redial_check_info_type;


/**
  @brief Enum representing call status 
  @see   ds_sys_event_data_call_status_change_type
*/
typedef enum
{
  DS_SYS_EVENT_DATA_CALL_ESTABLISHED,
  DS_SYS_EVENT_DATA_CALL_TERMINATED
} ds_sys_event_data_call_status_enum_type;

/**
  @brief Enum representing different call types 
  @see   ds_sys_event_data_call_status_change_type 
*/
typedef enum
{
  DS_SYS_EVENT_EMBEDDED_DATA_CALL,
  DS_SYS_EVENT_EMBEDDED_RMNET_DATA_CALL,
  DS_SYS_EVENT_TETHERED_RMNET_DATA_CALL,
  DS_SYS_EVENT_DUN_DATA_CALL
} ds_sys_event_data_call_type_enum_type;


/**
  @brief Data type to be used with DS_SYS_EVENT_DATA_CALL_STATUS_CHANGE 
  @see   DS_SYS_EVENT_DATA_CALL_STATUS_CHANGE
*/
typedef struct
{
  ds_sys_event_data_call_status_enum_type     call_status;
  /**< Call is established or terminated */
  ds_sys_event_data_call_type_enum_type       call_type;
  /**< Type of call i.e embedded, tethered etc */
} ds_sys_event_data_call_status_change_type;

/**
  @brief Data type to be used with 
         DS_SYS_EVENT_3GPP2_OUTAGE_NOTIFICATION
  @see   DS_SYS_EVENT_3GPP2_OUTAGE_NOTIFICATION
*/
typedef struct
{
  uint32            time_to_outage;
  uint32            duration;
} ds_sys_event_outage_notification_type;

/**
  @brief Callback function provided by client which is to be called when
         particular event is triggered for a given technology
   
  @param[in] tech_type            States Technology type
  @param[in] event_name           Name of event
  @param[in] event_info_ptr       Event related information
  @param[in] user_data_ptr        User Data 
   
  @see ds_sys_event_reg  
*/
typedef void (* ds_sys_event_cback_f_ptr_type)
(
  ds_sys_tech_enum_type                          tech_type,
  ds_sys_event_enum_type                         event_name,
  void                                         * event_info_ptr,
  void                                         * user_data_ptr
);

/**
  @brief Callback function provided by client which is to be called when
         user has registered for non-default subscription event
   
  @param[in] tech_type            States Technology type
  @param[in] event_name           Name of event
  @param[in] subscription_id      Subscription id
  @param[in] event_info_ptr       Event related information
  @param[in] user_data_ptr        User Data 
   
  @see ds_sys_event_reg  
*/
typedef void (* ds_sys_event_cback_f_ptr_ex_type)
(
  ds_sys_tech_enum_type                          tech_type,
  ds_sys_event_enum_type                         event_name,
  ds_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,
  void                                         * user_data_ptr
);

/**
  @brief Struct to be used with DS_SYS_EVENT_QUALITY_MEAS_POLICY_CHANGE
  @see   DS_SYS_EVENT_QUALITY_MEAS_POLICY_CHANGE 
*/

/* LTE RSSI Threshold Type */
typedef struct 
{
  boolean is_rsrp_high_valid;
  /**< Is Valid RSRP low threshold */
  int16 rsrp_high;
  /**< RSRP high threshold value for the channel*/
  boolean is_rsrp_mid_valid;
  /**< Is Valid RSRP Mid threshold */
  int16 rsrp_mid;
  /**< RSSI mid threshold value for the channel*/
  boolean is_rsrp_low_valid;
  /**< Is Valid RSRP low threshold */
  int16 rsrp_low;
  /**< RSSI low threshold value for the channel*/
} ds_sys_lte_ident_rsrp_threshold_type;

/* Wifi Signal Interference to Noise ratio Threshold*/
typedef struct 
{
  boolean is_sinr_high_valid;
  /**< Is Valid SINR High threshold */
  int16 sinr_high;
  /**< SINR high threshold value for the channel*/
  boolean is_sinr_low_valid;
  /**< Is Valid SINR low threshold */
  int16 sinr_low;
  /**< SINR low threshold value for the channel*/
} ds_sys_wifi_ident_sinr_threshold_type;

/* 1X Signal Interference to Noise ratio Threshold*/
typedef struct 
{
  boolean is_ecio_high_valid;
  /**< Is ECIO High Valid */
  int16 ecio_high;
  /**< SINR high threshold value for the channel*/
  boolean is_ecio_low_valid;
  /**< Is ECIO low valid */
  int16 ecio_low;
  /**< SINR low threshold value for the channel*/
} ds_sys_1x_ident_ecio_threshold_type;

typedef struct
{
   ds_sys_lte_ident_rsrp_threshold_type in_call_lte_threshold_params;
   /** LTE threshold parameters to be used when in call. */
   ds_sys_wifi_ident_rssi_threshold_type in_call_wifi_rssi_threshold_params;
   /** WIFI RSSI threshold params to be used when in call. */
   ds_sys_wifi_ident_sinr_threshold_type in_call_wifi_sinr_threshold_params;
   /** WIFI SINR threshold params to be used when in call. */
   ds_sys_lte_ident_rsrp_threshold_type idle_lte_threshold_params;
   /** LTE threshold parameters to be used when in idle state. */
   ds_sys_wifi_ident_rssi_threshold_type idle_wifi_rssi_threshold_params;
   /** WIFI RSSI threshold params to be used when in idle state. */
   ds_sys_wifi_ident_sinr_threshold_type idle_wifi_sinr_threshold_params;
   /** WIFI SINR threshold params to be used when in idle state. */
   ds_sys_1x_ident_ecio_threshold_type ecio_threshold_params;
   /**1x ECIO threshold params.*/
} ds_sys_quality_meas_policy_change_type;


typedef enum {
  DS_SYS_DATA_CONN_PDN_STATUS_UNSPECIFIED      = 0,
  DS_SYS_DATA_CONN_PDN_STATUS_REJECTED     = 1,
  DS_SYS_DATA_CONN_PDN_STATUS_ACTIVATED    = 2,
  DS_SYS_DATA_CONN_PDN_STATUS_DEACTIVATED  = 3
} ds_sys_data_conn_pdn_status_enum_type;

typedef enum {
  DS_SYS_PDP_TYPE_IPV4           = 0,
  DS_SYS_PDP_TYPE_IPV6           = 1,
  DS_SYS_PDP_TYPE_IPV4V6         = 2,
  DS_SYS_PDP_TYPE_PPP            = 3
} ds_sys_pdp_enum_type;

/**
  @brief Event info type for DS_SYS_EVENT_3GPP_DATA_CONNECTION_STATUS_CHANGE 
  @see   DS_SYS_EVENT_3GPP_DATA_CONNECTION_STATUS_CHANGE
*/
typedef struct
{
  uint8                                     call_id;  /**< Call Id, it would be 0xFF if CM has no assigned Id */
  ds_sys_rat_ex_enum_type                   rat;     /**< RAT information */
  ds_sys_data_conn_pdn_status_enum_type     pdn_status;  /**< PDN Status */
  uint16                                    cause_code; /**< Cause Code Refer 3GPP SPEC 24.301*/
  char                                      apn_name[DS_SYS_MAX_APN_LEN];  /**< APN Name */
  ds_sys_pdp_enum_type                      pdp_type; /*PDP type*/
} ds_sys_event_3gpp_data_connection_status_info_type;

/**
  @brief Data type to be used with  
         DS_SYS_EVENT_CA_CERT_SUPPORT
*/
typedef boolean ds_sys_event_ca_cert_retrieval_support;
  /** Value:
      -0: Not supported
      -1: Supported
  */

/**
  @brief data type for DS_SYS_EVENT_3GPP_LTE_CELL_HANDOVER_COMPLETE 
  @see   DS_SYS_EVENT_3GPP_LTE_CELL_HANDOVER_COMPLETE
*/
typedef struct
{
  boolean   handover_complete;
  /*Values:\n
      - 0: Handover failure 
      - 1: Handover complete 
  */
}ds_sys_event_3gpp_lte_cell_ho_info_type;
       

/*===========================================================================

                      PUBLIC FUNCTION DEFINITIONS

===========================================================================*/
/**
  @brief  This function allows client to register for particular event.
          Subscription id defaults to default primary subscription. In 
          order to deregister the event registered using ds_sys_event_reg, 
          user has to use ds_sys_event_dereg API

  @param[in] tech_type            States Technology type
  @param[in] event_name           Name of event
  @param[in] event_cback_f_ptr    Callback function when event is triggered
  @param[in] user_data_ptr        User Data
  @param[out] dsss_errno          Error code is filled when return value is not
                                  success. Possible values are
                                  @li DS_EFAULT if parameter is invalid
                                  @li DS_EINVAL if operation is invalid
                                  @li DS_NOMEMORY if memory allocation fails

  @return  DSS_SUCCESS           On Success.
  @return  DSS_ERROR             On failure.
 
  @code
 
  Example :
  In below code snippet, user registers for DS_SYS_EVENT_SYSTEM_STATUS_CHANGE
  event, In caseof success user receives event info in the callback. 
  system_status_change_cb is of type ds_sys_event_cback_f_ptr_type.
 
 
  void system_status_change_cb(..)                                               
  {                                                                         
    ds_sys_system_status_type * event_info = NULL;             
    LOG_MSG_INFO("Event callback recieved");
                                                                            
    event_info =  (ds_sys_system_status_type *)event_info_ptr; 
    LOG_MSG_INFO("Preferred network %d", event_info->preferred_network);   
  }                                                                         
                                                                            
  ret_val =  ds_sys_event_reg(DS_SYS_TECH_3GPP,                             
                              DS_SYS_EVENT_SYSTEM_STATUS_CHANGE,            
                              &system_status_change_cb,                          
                              NULL,                                         
                              &ds_errno);                                   
  if(0 > ret_val)                                                           
  {                                                                         
    LOG_MSG_ERROR("Event registration failed, err %d", ds_errno,0,0);      
  }
 
  @endcode
*/
int16 ds_sys_event_reg
(
  ds_sys_tech_enum_type                  tech_type,
  ds_sys_event_enum_type                 event_name,
  ds_sys_event_cback_f_ptr_type          event_cback_f_ptr,
  void                                 * user_data_ptr,
  int16                                * dss_errno
);

/**
  @brief  This function deregisters callback for the event. Subscription id
          defaults to default primary subscription. Callback registered through 
          ds_sys_event_reg should be deregistered using this API

  @param[in] tech_type           States Technology type
  @param[in] event_name          Name of event
  @param[in] event_cback_f_ptr   Event callback function to deregister
  @param[out] dss_errno          Error code is filled when return value is 
                                 not success. Possible values are
                                 @li DS_EFAULT if parameter is invalid
                                 @li DS_EINVAL if operation is invalid
                                 @li DS_NOMEMORY if memory allocation fails

  @return  DSS_SUCCESS           On Success.
  @return  DSS_ERROR             On failure. 
   
  @code 
   
  Example :
  In below code snippet, user registers for DS_SYS_EVENT_SYSTEM_STATUS_CHANGE
  event,  User use same event name, tech type and callback it used to register 
  for deregistering the event. 
 
  void system_status_change_cb(..) 
  { 
    ...
  }
 
  ret_val =  ds_sys_event_reg(DS_SYS_TECH_3GPP,
                              DS_SYS_EVENT_SYSTEM_STATUS_CHANGE,
                              &system_status_change_cb,
                              NULL,
                              &ds_errno);
  if(0 > ret_val) 
  { 
    LOG_MSG_ERROR("Event registration failed, err %d", ds_errno,0,0);
  } 
   
  ret_val =  ds_sys_event_dereg(DS_SYS_TECH_3GPP,
                                DS_SYS_EVENT_SYSTEM_STATUS_CHANGE,
                                &system_status_change_cb,
                                &ds_errno);
  if(0 > ret_val) 
  { 
    LOG_MSG_ERROR("Event deregistration failed, err %d", ds_errno,0,0);
  } 
   
  @endcode  
*/
int16 ds_sys_event_dereg
(
  ds_sys_tech_enum_type                  tech_type,
  ds_sys_event_enum_type                 event_name,
  ds_sys_event_cback_f_ptr_type          event_cback_f_ptr,
  int16                                * dss_errno
);


/**
  @brief  This function deregisters callback the event for particular
          subscription id. Callback registered through ds_sys_event_reg_ex
          should be deregistered using this API.

  @param[in] tech_type           States Technology type
  @param[in] event_name          Name of event
  @param[in] subscription_id     Subscription id
  @param[in] event_cback_f_ptr   Event callback function to deregister
  @param[out] dss_errno          Error code is filled when return value is 
                                 not success. Possible values are
                                 @li DS_EFAULT if parameter is invalid
                                 @li DS_EINVAL if operation is invalid
                                 @li DS_NOMEMORY if memory allocation fails

  @return  DSS_SUCCESS           On Success.
  @return  DSS_ERROR             On failure. 
*/
int16 ds_sys_event_dereg_ex
(
  ds_sys_tech_enum_type                  tech_type,
  ds_sys_event_enum_type                 event_name,
  ds_sys_subscription_enum_type          subscription_id,
  ds_sys_event_cback_f_ptr_ex_type       event_cback_f_ptr,
  int16                                * dss_errno
);

/**
  @brief  This function deregisters callback the event for particular
          subscription id. Callback registered through ds_sys_event_reg_ex
          should be deregistered using this API.

  @param[in] tech_type           States Technology type
  @param[in] event_name          Name of event
  @param[in] subscription_id     Subscription id
  @param[in] event_cback_f_ptr   Event callback function to deregister
  @param[in] client_data_ptr     Client data information
  @param[out] dss_errno          Error code is filled when return value is 
                                 not success. Possible values are
                                 @li DS_EFAULT if parameter is invalid
                                 @li DS_EINVAL if operation is invalid
                                 @li DS_NOMEMORY if memory allocation fails

  @return  DSS_SUCCESS           On Success.
  @return  DSS_ERROR             On failure. 
*/
int16 ds_sys_event_dereg_ex2
(
  ds_sys_tech_enum_type                  tech_type,
  ds_sys_event_enum_type                 event_name,
  ds_sys_subscription_enum_type          subscription_id,
  ds_sys_event_cback_f_ptr_ex_type       event_cback_f_ptr,
  void                                 * client_data_ptr,
  int16                                * dss_errno
);


/**
  @brief  This function allows client to register for particualar event 
          for particular subscription id. In order to deregister the event
          registered using ds_sys_event_reg_ex, user has to use 
          ds_sys_event_dereg_ex API

  @param[in] tech_type            States Technology type
  @param[in] event_name           Name of event
  @param[in] subscription_id      Subscription id
  @param[in] filter               Filters to be applied on event indication.
                                  Each event can have multiple filters associated 
                                  with it. User can set the required filters per 
                                  event using a data structure associated with it.
  @param[in] event_cback_f_ptr    Callback function when event is triggered
  @param[in] user_data_ptr        User Data
  @param[out] dsss_errno          Error code is filled when return value is not
                                  success. Possible values are
                                  @li DS_EFAULT if parameter is invalid
                                  @li DS_EINVAL if operation is invalid
                                  @li DS_NOMEMORY if memory allocation fails

  @return  DSS_SUCCESS           On Success.
  @return  DSS_ERROR             On failure.
 
 
*/
int16 ds_sys_event_reg_ex
(
  ds_sys_tech_enum_type                  tech_type,
  ds_sys_event_enum_type                 event_name,
  ds_sys_subscription_enum_type          subscription_id,
  ds_sys_event_cback_f_ptr_ex_type       event_cback_f_ptr,
  void                                 * user_data_ptr,
  int16                                * dss_errno
);

#ifdef __cplusplus
}
#endif

#endif /* DS_SYS_EVENT_H */
