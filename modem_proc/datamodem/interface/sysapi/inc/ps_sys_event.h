/**
@file ps_sys_event.h
@brief
This file defines functions, enums and structs use to register for events
*/

#ifndef PS_SYS_EVENT_H
#define PS_SYS_EVENT_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        P S _ S Y S _ E V E N T . H 


GENERAL DESCRIPTION
  This file defines functions, enums and structs use to register for
  events

Copyright (c) 2011-2015 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/sysapi/inc/ps_sys_event.h#3 $ $DateTime: 2024/05/06 04:25:26 $ $Author: pwbldsvc $
 
when        who    what, where, why
--------    ---    ---------------------------------------------------------- 
03/29/13    svj    New APIs to modify and query attach pdn list 
09/09/11    bvd    Created module 
 
===========================================================================*/


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "queue.h"
#include "ps_iface.h"
#include "ps_sys.h"
#include "ds_sys_event.h"
#include "ps_iface_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/

/** 
  Defines different SYS events 
*/
typedef enum
{
  PS_SYS_EVENT_MIN                   = DS_SYS_EVENT_MIN,

  /*---------------------------------------------------------------------------
      3GPP2 EVENTS
  ---------------------------------------------------------------------------*/
  /*3GPP2 Events common to DS and PS*/
  PS_SYS_EVENT_3GPP2_MIN             = DS_SYS_EVENT_3GPP2_MIN,                                   
  PS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_STATUS =
    DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_STATUS,
  /**
     @see  DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_STATUS  
  */

  PS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_CHANGE =
    DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_CHANGE,      
  /**
     @see  DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_CHANGE  
  */
                        
  PS_SYS_EVENT_3GPP2_OUTAGE_NOTIFICATION =
    DS_SYS_EVENT_3GPP2_OUTAGE_NOTIFICATION,      
  /**
     @see  DS_SYS_EVENT_3GPP2_OUTAGE_NOTIFICATION  
  */
      
  PS_SYS_EVENT_3GPP2_QOS_NETWORK_STATUS_CHANGE =
    DS_SYS_EVENT_3GPP2_QOS_NETWORK_STATUS_CHANGE,      
  /**
     @see  DS_SYS_EVENT_3GPP2_QOS_NETWORK_STATUS_CHANGE  
  */  
      
  PS_SYS_EVENT_3GPP2_MAX             = DS_SYS_EVENT_3GPP2_MAX,

  /*3GPP2 Events internal to PS*/
  PS_SYS_EVENT_3GPP2_INTERNAL_MIN    = PS_SYS_EVENT_3GPP2_MAX + 1,   
  PS_SYS_EVENT_3GPP2_EHRPD_PREREG_HANDOFF_COMPLETE_RSP = 
    PS_SYS_EVENT_3GPP2_INTERNAL_MIN,
  /**<
       When epc iface detects system moves to ehrpd. It sends out ioctl
       indication if all ifaces are preregistered or not. The event is triggered
       as a response to the
       PS_SYS_IOCTL_3GPP2_EHRPD_PREREG_HANDOFF_COMPLETE_CMD sent by the user.
       User will receive a boolean flag confirming success/failure of the
       prereg handoff. On success value of flag, user swaps the RAT and enables
       data transfer. On failure, user resyncs RAT. User should register for 
       the event before sending
       PS_SYS_IOCTL_3GPP2_EHRPD_PREREG_HANDOFF_COMPLETE_CMD.
  */

  PS_SYS_EVENT_3GPP2_S101_STATUS_CHANGE = PS_SYS_EVENT_3GPP2_INTERNAL_MIN + 1,
  /**<       
       Event is triggered whenever the tunnel call state changes from allowed
       to disallowed and vice versa In response to event, User gets info
       whether tunnel call is allowed or not. On recieving event result, when
       the status changes to tunnel call allowed, user will preregister the LTE
       PDN contexts (those PDNs that are not preregistered earlier). When the
       status changes to tunnel call disallowed, user will not attempt
       preregistration for any new LTE PDN contexts. User register for event on
       power up.        
  */

  PS_SYS_EVENT_3GPP2_EPZID_INFO     = PS_SYS_EVENT_3GPP2_INTERNAL_MIN + 2,
  /**<       
       Event is triggered whenever the hysterisis is cancelled or expires
       on the current EPZID
  */

  PS_SYS_EVENT_3GPP2_EHRPD_PREREG_HANDOFF_STATUS =
    PS_SYS_EVENT_3GPP2_INTERNAL_MIN + 3,

  PS_SYS_EVENT_3GPP2_INTERNAL_MAX   = PS_SYS_EVENT_3GPP2_INTERNAL_MIN + 100,

  /*---------------------------------------------------------------------------
      3GPP EVENTS
  ---------------------------------------------------------------------------*/

  /*3GPP Events common to DS and PS*/
  PS_SYS_EVENT_3GPP_MIN             = DS_SYS_EVENT_3GPP_MIN,
  PS_SYS_EVENT_3GPP_LTE_ATTACH_PDN_LIST_CHANGE = 
    DS_SYS_EVENT_3GPP_LTE_ATTACH_PDN_LIST_CHANGE,
  /**<        
       Event is triggered upon lte attach pdn list write completion to the UE
       using ioctl PS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST.
       Along with the event, the list of profile IDs are sent by MH.
       User Registers if interested to know that the LTE attach PDN list has
       changed
   
       @see PS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST
  */
  PS_SYS_EVENT_3GPP_EMERGENCY_PDN_STATUS = DS_SYS_EVENT_3GPP_EMERGENCY_PDN_STATUS,
  /**<        
       @see DS_SYS_EVENT_3GPP_EMERGENCY_PDN_STATUS
  */
  PS_SYS_EVENT_3GPP_ROAMING_DISALLOWED_INFO = DS_SYS_EVENT_3GPP_ROAMING_DISALLOWED_INFO,
  /**
    @see  DS_SYS_EVENT_ROAMING_DISALLOWED_INFO
  */

  PS_SYS_EVENT_WIFI_SETTINGS_CHANGE = DS_SYS_EVENT_WIFI_SETTINGS_CHANGE,
  /**
    @see  DS_SYS_EVENT_WIFI_SETTINGS_CHANGE
  */

  PS_SYS_EVENT_3GPP_LTE_CELL_INFO_CHANGE = DS_SYS_EVENT_3GPP_LTE_CELL_INFO_CHANGE,
  /**
    @see  DS_SYS_EVENT_3GPP_LTE_CELL_INFO_CHANGE
  */
  
  PS_SYS_EVENT_3GPP_DATA_CONNECTION_STATUS_CHANGE = DS_SYS_EVENT_3GPP_DATA_CONNECTION_STATUS_CHANGE,
  /**
    @see  DS_SYS_EVENT_3GPP_DATA_CONNECTION_STATUS_CHANGE
  */  

  PS_SYS_EVENT_3GPP_LTE_CELL_HANDOVER_COMPLETE = DS_SYS_EVENT_3GPP_LTE_CELL_HANDOVER_COMPLETE,
  /**
    @see  DS_SYS_EVENT_3GPP_LTE_CELL_HANDOVER_COMPLETE
  */

  PS_SYS_EVENT_3GPP_PS_DATA_AVAIL_CHANGE = DS_SYS_EVENT_3GPP_PS_DATA_AVAIL_CHANGE,
  /**< 
       @see DS_SYS_EVENT_3GPP_PS_DATA_AVAIL_CHANGE
  */ 
  
  PS_SYS_EVENT_3GPP_MAX             = DS_SYS_EVENT_3GPP_MAX,


  /*3GPP Events internal to PS*/
  PS_SYS_EVENT_3GPP_INTERNAL_MIN          = PS_SYS_EVENT_3GPP_MAX + 1, 
  PS_SYS_EVENT_3GPP_LTE_ATTACH_INIT       = PS_SYS_EVENT_3GPP_INTERNAL_MIN,
  /**< 
       Event is triggered currently if the FEATURE_EPC_HANDOFF is
       present aka there could be a context transfer from some other RAT.
       Along with the event the attach profile number is sent. On recieving
       event result user must let the Mode Handler know if there are contexts
       to be transferred by invoking the IOCTL PS_SYS_IOCTL_LTE_ATTACH_INIT.
       User registers for this event if its interested to know the LTE attach
        process has been started.
  */

  PS_SYS_EVENT_3GPP_LTE_ATTACH_COMPLETE   = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 1,
  /**<        
       Event is triggered upon receiving attach complete indication from NAS
       after L2 ACK. Along with the event, the details of ifaces and APN name
       are sent by MH. On recieving event result, user shall register for iface
       events. User Registers if interested to know the completion of LTE attach.
  */

  PS_SYS_EVENT_3GPP_LTE_ATTACH_TYPE_CHANGE = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 2,
  /**<        
       Event is triggered when the configuration item PS_SYS_CONF_3GPP_LTE_ATTACH_TYPE
       is set. User registers if interested to know about change in its value.
  */
  PS_SYS_EVENT_3GPP_POLICY_REFRESH_STATUS = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 3,
  /**<        
       Event is triggered when the policy refresh that was trigerred by 
       PS_SYS_IOCTL_3GPP_ANDSF_POLICY_REFRESH is complete. 
  */

  PS_SYS_EVENT_3GPP_POLICY_AVAILABLE = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 4,
  /**<        
       Event is triggered when a policy is available for transfer 
  */

  PS_SYS_EVENT_3GPP_LTE_ATTACH_PARAMS_EX   = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 5,
  /**<        
       Event is triggered upon receiving attach params ex indication.
  */

  PS_SYS_EVENT_3GPP_WWAN_CONN_STATE  = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 6,
  /**< 
      Event is triggered upon change in connection state of WCDMA.
  */

  PS_SYS_EVENT_3GPP_POLICYMAN_MSG_IND  = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 7,
  /**< 
      Event is triggered by ds3g module upon recieving indication from 
      policyman module
  */  

 PS_SYS_EVENT_3GPP_RAB_REJECT_IND  = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 8,
  /**< 
      Event is triggered upon RAB Rejection by network. This interface only 
      supports a subset of all the RAB reject cause codes and it is meant 
      to be used only for VOLTE scenarios. In case of non-VOLTE scenarios, 
      UNKNOWN cause code shall be returned. 
  */

  PS_SYS_EVENT_MODEM_ASST_KEEP_ALIVE_STATUS  = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 9,
  /**< 
        Event is triggered to notify modem asssited keep alive status to applications.
  */ 

  PS_SYS_EVENT_MODEM_ASST_KEEP_ALIVE_UPDATE  = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 10,
  /**< 
        Event is triggered when PS_SYS_CONF_MODEM_ASST_KEEP_ALIVE_CONFIG is updated.
  */  

  PS_SYS_EVENT_3GPP_BACKGROUND_TRAFFIC_IND  = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 11,
  /**< 
        Event is triggered whenever the PS_SYS_CONF_3GPP_BACKGROUND_TRAFFIC_IND is          updated by 3GPP mode handler to notify changes to background traffic status.
  */  

PS_SYS_EVENT_3GPP_LTE_ATTACH_PDN_LIST_ACTION_RESULT  = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 12,
  /**< 
        Event is triggered by MH when it processes the LTE attach PDN action request 
        from the application.
  */ 

  PS_SYS_EVENT_3GPP_CONFIG_APP_PREF_RESULT_IND = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 13,
  /**< 
        Event is triggered when appsrv is done with processing the applicaiton 
        preference configuration request.
  */ 

  PS_SYS_EVENT_3GPP_REMOVE_APP_PREF_RESULT_IND = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 14,
  /**< 
        Event is triggered when appsrv is done with processing the applicaiton
        preference remove request.
  */ 

  PS_SYS_EVENT_3GPP_INTERNAL_MAX          = PS_SYS_EVENT_3GPP_INTERNAL_MIN + 100,
  
  /*---------------------------------------------------------------------------
      ALL TECH COMMON EVENTS
  ---------------------------------------------------------------------------*/
  /*Common Tech Events common to both DS and PS*/  
  PS_SYS_EVENT_COMMON_MIN            = DS_SYS_EVENT_COMMON_MIN,
  PS_SYS_EVENT_SYSTEM_STATUS_CHANGE  = DS_SYS_EVENT_SYSTEM_STATUS_CHANGE,
  /**< 
      @see DS_SYS_EVENT_SYSTEM_STATUS_CHANGE      
  */

  PS_SYS_EVENT_DUN_CTRL_CALL_STATUS  = DS_SYS_EVENT_DUN_CTRL_CALL_STATUS,
  /**
     @see  DS_SYS_EVENT_DUN_CTRL_CALL_STATUS  
  */

  PS_SYS_EVENT_DUN_CTRL_ENTITLEMENT_CHECK    =
    DS_SYS_EVENT_DUN_CTRL_ENTITLEMENT_CHECK,
  /**
     @see  DS_SYS_EVENT_DUN_CTRL_ENTITLEMENT_CHECK  
  */

  PS_SYS_EVENT_DUN_CTRL_SILENT_REDIAL_CHECK  =
    DS_SYS_EVENT_DUN_CTRL_SILENT_REDIAL_CHECK,
  /**
     @see  DS_SYS_EVENT_DUN_CTRL_SILENT_REDIAL_CHECK  
  */

  PS_SYS_EVENT_DATA_CALL_STATUS_CHANGE  = DS_SYS_EVENT_DATA_CALL_STATUS_CHANGE,
  /**
     @see  DS_SYS_EVENT_DATA_CALL_STATUS_CHANGE  
  */

  PS_SYS_EVENT_SYSTEM_STATUS_EX = DS_SYS_EVENT_SYSTEM_STATUS_EX,
  /**
    @see  DS_SYS_EVENT_SYSTEM_STATUS_EX
  */

  PS_SYS_EVENT_UPDATED_THROUGHPUT_TIMER = DS_SYS_EVENT_UPDATED_THROUGHPUT_TIMER,
  /**
    @see  DS_SYS_EVENT_UPDATED_THROUGHPUT_TIMER
  */

  PS_SYS_EVENT_THROUGHPUT_INFO = DS_SYS_EVENT_THROUGHPUT_INFO,
  /**
    @see  DS_SYS_EVENT_THROUGHPUT_INFO
  */

  PS_SYS_EVENT_START_FAST_DORMANCY = DS_SYS_EVENT_START_FAST_DORMANCY,
   /**
    @see  DS_SYS_EVENT_START_FAST_DORMANCY
  */

  PS_SYS_EVENT_STOP_FAST_DORMANCY = DS_SYS_EVENT_STOP_FAST_DORMANCY,
   /**
    @see  DS_SYS_EVENT_STOP_FAST_DORMANCY
  */

  PS_SYS_EVENT_THERMAL_MITIGATION_INFO  =  DS_SYS_EVENT_THERMAL_MITIGATION_INFO,
  /**<
    @see  DS_SYS_EVENT_THERMAL_MITIGATION_INFO
  */
  
   PS_SYS_EVENT_UPDATED_DL_THROUGHPUT_TIMER    = DS_SYS_EVENT_UPDATED_DL_THROUGHPUT_TIMER,
   /**
    @see  PS_SYS_EVENT_UPDATED_DL_THROUGHPUT_TIMER
  */
 
   PS_SYS_EVENT_DL_THROUGHPUT_INFO              = DS_SYS_EVENT_DL_THROUGHPUT_INFO,
   /**
    @see  PS_SYS_EVENT_DL_THROUGHPUT_INFO
  */
 
   PS_SYS_EVENT_DATA_SERVICE_SETTING   = DS_SYS_EVENT_DATA_SERVICE_SETTING,
   /**
    @see  DS_SYS_EVENT_DATA_SERVICE_SETTING  
  */

  PS_SYS_EVENT_DATA_SERVICE_ROAMING_SETTING  = DS_SYS_EVENT_DATA_SERVICE_ROAMING_SETTING,
   /**
     @see  DS_SYS_EVENT_DATA_SERVICE_ROAMING_SETTING  
  */

  PS_SYS_EVENT_APN_INFO = DS_SYS_EVENT_APN_INFO,
   /**
     @see  DS_SYS_EVENT_APN_INFO
   */

  PS_SYS_EVENT_QUALITY_MEAS_POLICY_CHANGE = DS_SYS_EVENT_QUALITY_MEAS_POLICY_CHANGE,
   /**
    @see  DS_SYS_EVENT_QUALITY_MEAS_POLICY_CHANGE
  */

  PS_SYS_EVENT_RAT_PREFERENCE        = DS_SYS_EVENT_RAT_PREFERENCE,
  /**<        
       Event is triggered to notify the rat preference
  */

  PS_SYS_EVENT_UPDATED_DL_THROUGHPUT_INTERVAL_STATUS_CHANGE   = 
    DS_SYS_EVENT_UPDATED_DL_THROUGHPUT_INTERVAL_STATUS_CHANGE,
  /**<        
       Event is triggered whenever MH notifies change in throughput interval
       status using PS_SYS_CONF_DL_THROUGHPUT_INTERVAL_STATUS_CHANGE.
  */


   PS_SYS_EVENT_UPDATED_UL_THROUGHPUT_CONFIG    = DS_SYS_EVENT_UPDATED_UL_THROUGHPUT_CONFIG,
   /**
    @see  DS_SYS_EVENT_UPDATED_UL_THROUGHPUT_CONFIG
  */
 
   PS_SYS_EVENT_UL_THROUGHPUT_INTERVAL_STATUS_CHANGE    = DS_SYS_EVENT_UL_THROUGHPUT_INTERVAL_STATUS_CHANGE,
   /**
    @see  DS_SYS_EVENT_UL_THROUGHPUT_INTERVAL_STATUS_CHANGE
  */

   PS_SYS_EVENT_UL_THROUGHPUT_INFO    = DS_SYS_EVENT_UL_THROUGHPUT_INFO,
   /**
    @see  DS_SYS_EVENT_UL_THROUGHPUT_INFO
  */

  PS_SYS_EVENT_DATA_ACTIVITY_RESUMED = DS_SYS_EVENT_DATA_ACTIVITY_RESUMED,
  /**
    @see  DS_SYS_EVENT_DATA_ACTIVITY_RESUMED
  */

  PS_SYS_EVENT_ROAMING_STATUS_CHANGE = DS_SYS_EVENT_ROAMING_STATUS_CHANGE,
  /**<        
       Event is triggered when the UE moves away from the home network and is roaming.
       User registers for this event to know if the UE is in the home network or roaming.
  */  

  PS_SYS_EVENT_APN_PCO_INFO_CHANGE = DS_SYS_EVENT_APN_PCO_INFO_CHANGE,
  /**<        
       @see  DS_SYS_EVENT_APN_PCO_INFO_CHANGE
  */   

  PS_SYS_EVENT_APN_MSISDN_INFO_CHANGE = DS_SYS_EVENT_APN_MSISDN_INFO_CHANGE,
  /**<        
       @see  DS_SYS_EVENT_APN_PCO_INFO_CHANGE
  */ 
  PS_SYS_EVENT_LOW_LATENCY_TRAFFIC_STATUS = DS_SYS_EVENT_LOW_LATENCY_TRAFFIC_STATUS,
  /**<        
       @see DS_SYS_EVENT_LOW_LATENCY_TRAFFIC_STATUS
  */

  PS_SYS_EVENT_PDN_THROTTLE_INFO = DS_SYS_EVENT_PDN_THROTTLE_INFO,
  /**< 
       The event is triggered when PDN is throttled or un-throttled. In 
       response of event. User gets to know which PDN is throttled right now
       and what is the remaining time for each PDN throttling. After recieving
       this event, user has to go through PDN throttle table to see which PDN
       is throttled and only make the call for the PDN not throttled.
  */

  PS_SYS_EVENT_CA_CERT_RETRIEVAL_SUPPORT = DS_SYS_EVENT_CA_CERT_RETRIEVAL_SUPPORT,
  /**< 
       Event is triggered when the TE that supports CA certificate retrieval 
       registers/deregisters with modem.  
  */

  PS_SYS_EVENT_CURRENT_DDS_IND = DS_SYS_EVENT_CURRENT_DDS_IND,
  /**< 
      @see DS_SYS_EVENT_CURRENT_DDS_IND
  */ 

  PS_SYS_EVENT_COMMON_MAX            = DS_SYS_EVENT_COMMON_MAX,

  /*Common Events internal to PS*/
  PS_SYS_EVENT_COMMON_INTERNAL_MIN        = PS_SYS_EVENT_COMMON_MAX + 1, 
  PS_SYS_EVENT_MODEM_OUT_OF_SERVICE       = PS_SYS_EVENT_COMMON_INTERNAL_MIN,
  /**< 
       The event is triggered when qmimsglib sends indication for modem 
       restart in progress to sysapi on APPs Processor. 
        
       @see ds_sys_qmi_sys_event_handler
  */

  PS_SYS_EVENT_MODEM_IN_SERVICE	    	 =  PS_SYS_EVENT_COMMON_INTERNAL_MIN + 2,
  /**<
       This is internal event generated when modem is back in service,
       after modem restart.  The event is passed along to ps event -
       command handler which would reinitialize qmi client
  */

  PS_SYS_EVENT_APN_PREF_SYS_CHANGE        = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 3,
  /**<        
       Event is triggered when any user like IMS changes its technology 
       preference for data call. It results in handoff process if data call
       is not currently on that technology. Users can register whenever they 
       need to know about handoff tech change event.
  */

  PS_SYS_EVENT_PDN_STATUS_INFO       = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 4,
  /**< 
       The event is triggered when the PDN is active or inactive. In 
       response of event, user gets to know whether the pdn is up pr down and 
       the name of the apn.
  */

  PS_SYS_EVENT_NULL_RESOLVED_APN    = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 5,
  /**
       Event is triggered when EPC mode handler resolves the null apn.
  */

   PS_SYS_EVENT_UL_DL_THROUGHPUT_INFO_FREQ     = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 7,
   /**
       Event is triggered to notify uplink/downlink throughput info
       frequency updates to MH.
  */

   PS_SYS_EVENT_UL_DL_THROUGHPUT_INFO     = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 8,
   /**
       Event is triggered to notify uplink/downlink throughput
       information to external clients
  */

   PS_SYS_EVENT_CONFIGURED_UL_DL_THROUGHPUT_INFO  = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 9,
   /**
       Event is triggered to notify configured uplink/downlink
       throughput information to external clients
  */

  PS_SYS_EVENT_MODEM_POWER_STATS_CONFIGURATION        
                                          = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 10,
  /**<        
       Event is triggered to notify power stats configuration update
  */

  PS_SYS_EVENT_QUERY_MODEM_POWER_STATS        
                                          = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 11,
  /**<        
       Event is triggered to update that modem power stats is being queried
  */

  PS_SYS_EVENT_MODEM_POWER_STATISTICS        
                                          = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 12,
  /**<        
       Event is triggered to notify current modem power statistics
  */
  PS_SYS_EVENT_REMOTE_SOCKET_SERVICE_AVAIL  = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 13,
   /**
       Event is triggered to notify whether remote socket handling
       is available/not available
  */
  PS_SYS_EVENT_REMOTE_SOCKET_REQUEST  = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 14,
   /**
       Event is triggered to request for remote sockets from the TE
  */
  PS_SYS_EVENT_REMOTE_SOCKET_RELEASE  = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 15,
   /**
       Event is triggered to release remote sockets on the TE
  */
  PS_SYS_EVENT_REMOTE_SOCKET_SET_OPTION  = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 16,
   /**
       Event is triggered to set remote socket option on the TE
   */

  PS_SYS_EVENT_THROUGHPUT_REPORTING_INTERVAL_CHANGE  = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 17,
  /**< 
       Event is triggered when any user changes the Reporting Interval in sys conf.
       It results in UL and DL throughput report frequency/Timer changes in lower
       layers.
  */

  PS_SYS_EVENT_SWITCH_DDS_IND                 = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 18,
  /**< 
      Event is triggered when  modehandler is completed with DDS switch process.
  */ 

  PS_SYS_EVENT_UPDATED_IMSI_PUBLIC_KEY_INFO = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 19,
  /**< 
       Event is triggered when AP updates the IMSI INFO conf on the modem.
  */

  PS_SYS_EVENT_RETRIEVE_IMSI_PUBLIC_KEY_INFO = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 20,
  /**< 
       Event is triggered to retrieve the IMSI public key information from AP.
  */
PS_SYS_EVENT_UL_THRPUT_ACTUAL_THRES_CRITERIA = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 21,
  /**< 
       Event is triggered when any user changes the actual configuration for threshold 
       based uplink throughput reporting.
  */
  PS_SYS_EVENT_DL_THRPUT_ACTUAL_THRES_CRITERIA = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 22,
  /**< 
       Event is triggered when any user changes the actual configuration for threshold 
       based downlink throughput reporting.

  */

  PS_SYS_EVENT_GET_CA_CERTIFICATE = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 23,
  /**< 
       Event is triggered by framework to retrieve the CA 
       certificate information from TE. Internal use only.
  */

  PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_MODE = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 24,
  /**< 
       Event is triggered by AP to MPPM to enable handshake mode for changing 
       preferred RAT for MPPM
  */  
  
  PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_CHANGE_REQ = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 25,
  /**< 
       Event is triggered by MPPM module to notify AP about request to
       change APN preferred system status. AP shall respond using QMI which results 
       in generation of  PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_CHANGE.
  */
  
  PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_RESULT_IND = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 26,
  /**< 
       Event is triggered by MPPM module to notify AP about result of
       change in APN preferred system status requested using 
       PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_CHANGE
  */ 

  PS_SYS_EVENT_APN_PREF_SYS_CHANGE_EX        = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 27,
  /**<        
       Event is triggered when legacy clients changes the preferred RAT for APN using
       PS_SYS_CONF_APN_PREF_SYS. MPPM module shall consume this event and generate 
       PS_SYS_EVENT_APN_PREF_SYS_CHANGE which is consumed by DSD module. 
       If AP Assisted APN Pref Sys feature is off then it will generate 
       PS_SYS_EVENT_APN_PREF_SYS_CHANGE event immediately else it will be generated
       after handshake with AP
  */

  PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_CHANGE   = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 28,
  /**<        
       Event is triggered when AP changes preferred RAT for APN in response to 
       PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_CHANGE_REQ.
  */

#ifdef QWES_FEATURE_ENABLE 
  PS_SYS_EVENT_QTEELS_AVAILABLE  = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 33,
  /**< 
       Event is triggered by framework to CAAS modem manager to inform 
       that the license service is available
  */

   PS_SYS_EVENT_CAAS_FEATURE_STATUS  = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 34,
  /**< 
        Event is triggered by framework when license service responds to 
        modem with the feature status.
  */

   PS_SYS_EVENT_CAAS_GET_FEATURE_STATUS  = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 35,
  /**< 
        Event is triggered by framework to qmi qteels client when CAAS modem manager 
        needs the feature status from license service
  */

  PS_SYS_EVENT_SYNC_COMPLETE_BROADCAST = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 36,
  /**< 
        Event is triggered by qmi qteels client to framework
  */
  
  PS_SYS_EVENT_QWES_NACK  = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 62,
  /**< 
       Event is triggered by framework to CAAS modem manager to inform 
       that the qwes sent NACK for feature status request
  */
#endif /* QWES_FEATURE_ENABLE  */

#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif /* FEATURE_LAPP_UFS */


  PS_SYS_EVENT_COMMON_INTERNAL_MAX        = PS_SYS_EVENT_COMMON_INTERNAL_MIN + 100,

  PS_SYS_EVENT_WLAN_MIN                   = DS_SYS_EVENT_WLAN_MIN,
  PS_SYS_EVENT_WLAN_MAC_ADDR_CHANGE       = DS_SYS_EVENT_WLAN_MAC_ADDR_CHANGE,
  /**< 
   User can register for this event whenever they want to know regarding
   change in MAC address.
  */
  PS_SYS_EVENT_WLAN_SSID_CHANGE           = DS_SYS_EVENT_WLAN_SSID_CHANGE,
  /**< 
   User can register for this event whenever they want to know regarding
   change in SSID.
  */

  PS_SYS_EVENT_WLAN_HESSID_CHANGE           = DS_SYS_EVENT_WLAN_HESSID_CHANGE,
  /**< 
   User can register for this event whenever they want to know regarding
   change in HESSID.
  */

  PS_SYS_EVENT_WLAN_IP_ADDR_CHANGE        = DS_SYS_EVENT_WLAN_IP_ADDR_CHANGE,
  /**< 
   User can register for this event whenever they want to know regarding
   change in ip address.
  */
  
  PS_SYS_EVENT_WQE_PROFILES_MET           = DS_SYS_EVENT_WQE_PROFILES_MET,
  /**<
     @see  DS_SYS_EVENT_WQE_PROFILES_MET
  */
  
  PS_SYS_EVENT_WQE_PROFILE_STATUS         = DS_SYS_EVENT_WQE_PROFILES_STATUS,
  /**<
     @see  DS_SYS_EVENT_WQE_PROFILES_STATUS
  */
  
  PS_SYS_EVENT_WLAN_MAX                   = DS_SYS_EVENT_WLAN_MAX,

  PS_SYS_EVENT_WLAN_INTERNAL_MIN          = PS_SYS_EVENT_WLAN_MAX + 1,

  PS_SYS_EVENT_WLAN_PREFERENCE_CHANGE     = PS_SYS_EVENT_WLAN_INTERNAL_MIN,
  /**< 
     User can register for this event whenever they want to know regarding
     change in WLAN preference.
  */
  
  PS_SYS_EVENT_WLAN_START_WIFI_MEAS       = PS_SYS_EVENT_WLAN_INTERNAL_MIN + 1,
  /**< 
   User can register for this event whenever they want to know when to start
   sending WiFi measurements.
  */

  PS_SYS_EVENT_WLAN_STOP_WIFI_MEAS        = PS_SYS_EVENT_WLAN_INTERNAL_MIN + 2,
  /**< 
   User can register for this event whenever they want to know when to stop
   sending WiFi measurements.
  */
  
  PS_SYS_EVENT_WLAN_MAX_CONN              = PS_SYS_EVENT_WLAN_INTERNAL_MIN + 3,
  /**< 
   User can register for this event whenever they want to know if the max iwlan 
   conn is reached. The event would prevent new bringup/handoff when the max 
   connection is reached.
   For eg. If max pdns for iwlan is 3 and if there are already 2 pdns up on iwlan 
   and 1 pdn up on lte. If a 4th pdn comes up on iwlan, the bringup will be 
   prevented as the max connections are reached.
   During handoff scenarios, if lte to iwlan happens in the above case, an 
   indication would be sent as max connections is reached. This would prevent the
   handoff from happening. 
  */

  PS_SYS_EVENT_WLAN_STATUS_CHANGE         = PS_SYS_EVENT_WLAN_INTERNAL_MIN + 4,
  /**< 
   User can register for this event whenever they want to know regarding
   change in WLAN status.
  */
  
   PS_SYS_EVENT_WQE_PROFILE_CHANGE        = PS_SYS_EVENT_WLAN_INTERNAL_MIN + 5,
  /**<        
   Event is triggered by ANDSF to notify the new Wifi quality estimation profile
   type
  */

   PS_SYS_EVENT_NAT_KEEP_ALIVE_INFO        = PS_SYS_EVENT_WLAN_INTERNAL_MIN + 6,
  /**<        
   Event is triggered by IWLAN MH to configure AP for the Nat keep alive
   transmissions
  */

   PS_SYS_EVENT_SET_WQE_PROFILE_QUALITY_MEAS_INFO =  PS_SYS_EVENT_WLAN_INTERNAL_MIN + 7,
  /**<        
   Event is triggered by ANDSF to configure AP with the required WQE Profile quality 
   parameters for a given WQE Profiles.
  */

   PS_SYS_EVENT_WIFI_SERVICE_CAP          = PS_SYS_EVENT_WLAN_INTERNAL_MIN + 8,
  /**<        
     User can register for this event whenever they want to know regarding
     change in WIFI service capability.
  */

   PS_SYS_EVENT_WIFI_BAND_VACATE_IND  = PS_SYS_EVENT_WLAN_INTERNAL_MIN + 9,
  /**<        
     User can register for this event whenever they want to know regarding
     change in WIFI band vacate request. The Indication is generated by
     ANDSF if the modem is requested to use LTE-U.
  */

  
   PS_SYS_EVENT_WLAN_REFRESH_COUNTRY_CODE  = PS_SYS_EVENT_WLAN_INTERNAL_MIN + 10,
  /**<        
     Event is triggered by IWLAN MH to request AP for a new country code.
  */

  PS_SYS_EVENT_WLAN_INTERNAL_MAX          = PS_SYS_EVENT_WLAN_INTERNAL_MIN + 100,

  PS_SYS_EVENT_MAX
} ps_sys_event_enum_type;

/**
  @brief Event info type for PS_SYS_EVENT_3GPP_LTE_ATTACH_INIT event. 
  @see  PS_SYS_EVENT_3GPP_LTE_ATTACH_INIT  
*/
typedef struct
{
  uint16         attach_profile; /**< Attach profile number for LTE */
} ps_sys_event_3gpp_lte_attach_init_type;

/**
  @brief Event info type for PS_SYS_EVENT_3GPP_LTE_ATTACH_COMPLETE event. 
  @see  PS_SYS_EVENT_3GPP_LTE_ATTACH_COMPLETE 
*/
typedef struct
{
  ps_iface_type        * v4_iface_ptr; /**< pointer to v4 iface of LTE */
  ps_iface_type        * v6_iface_ptr; /**< pointer to v6 iface of LTE */
} ps_sys_event_3gpp_lte_attach_complete_type;

/**
  @brief Event info type for PS_SYS_EVENT_3GPP_LTE_ATTACH_PARAMS_EX event. 
  @see  PS_SYS_EVENT_3GPP_LTE_ATTACH_PARAMS_EX
*/
typedef struct
{
  ps_sys_lte_attach_params_type      attach_params; /**< attach params */
} ps_sys_event_3gpp_lte_attach_params_ex_type;

/**
  @brief Event info type for 
         PS_SYS_EVENT_3GPP2_EHRPD_PREREG_HANDOFF_COMPLETE_RSP event.
  @see  PS_SYS_EVENT_3GPP2_EHRPD_PREREG_HANDOFF_COMPLETE_RSP  
*/
typedef struct
{
  
  boolean prereg_handoff_succeeded; /**< Flag for success/failure of prereg 
                                         handoff */
} ps_sys_event_3gpp2_ehrpd_prereg_handoff_complete_rsp_type;

/**
  @brief Enum of type page monitor period result
*/
typedef enum
{
  PS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_RESULT_SUCCESS =
    DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_RESULT_SUCCESS,
  PS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_RESULT_FAIL_REQUEST_REJECTED =
    DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_RESULT_FAIL_REQUEST_REJECTED,
  PS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_RESULT_FAIL_REQUEST_FAILED_TX =
    DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_RESULT_FAIL_REQUEST_FAILED_TX,
  PS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_RESULT_FAIL_NOT_SUPPORTED =
    DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_RESULT_FAIL_NOT_SUPPORTED
} ps_sys_event_3gpp2_evdo_page_monitor_period_status_enum_type;


/**
  @brief Event info type for PS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_STATUS
         event
*/
typedef struct
{
  ps_sys_event_3gpp2_evdo_page_monitor_period_status_enum_type status;
} ps_sys_event_3gpp2_evdo_page_monitor_period_status_info_type;

/**
  @brief Event info type for PS_SYS_EVENT_DUN_CTRL_CALL_STATUS
*/
typedef struct
{
  ps_sys_tech_enum_type          tech_type;
  unsigned char                  cb_data;
  boolean                        is_call_established;
  ps_iface_net_down_reason_type  net_down_reason;
} ps_sys_event_dun_ctrl_call_status_info_type;

/**
  @brief Event info type for PS_SYS_EVENT_DUN_CTRL_ENTITLEMENT_CHECK
*/
typedef struct
{
  ps_sys_tech_enum_type          tech_type;
  unsigned char                  cb_data;
} ps_sys_event_dun_ctrl_entitlement_check_info_type;

/**
  @brief Event info type for PS_SYS_EVENT_DUN_CTRL_SILENT_REDIAL_CHECK
*/
typedef struct
{
  ps_sys_tech_enum_type          tech_type;
  unsigned char                  cb_data;
  ps_iface_net_down_reason_type  net_down_reason;
} ps_sys_event_dun_ctrl_silent_redial_check_info_type;


/**
  @brief Enum representing WDS call status
*/
typedef enum
{
  PS_SYS_EVENT_DATA_CALL_ESTABLISHED = DS_SYS_EVENT_DATA_CALL_ESTABLISHED,
  PS_SYS_EVENT_DATA_CALL_TERMINATED  = DS_SYS_EVENT_DATA_CALL_TERMINATED
} ps_sys_event_data_call_status_enum_type;

/**
  @brief Enum representing different WDS call types
*/
typedef enum
{
  PS_SYS_EVENT_EMBEDDED_DATA_CALL       = DS_SYS_EVENT_EMBEDDED_DATA_CALL,
  PS_SYS_EVENT_EMBEDDED_RMNET_DATA_CALL =
    DS_SYS_EVENT_EMBEDDED_RMNET_DATA_CALL,
  PS_SYS_EVENT_TETHERED_RMNET_DATA_CALL =
    DS_SYS_EVENT_TETHERED_RMNET_DATA_CALL,
  PS_SYS_EVENT_DUN_DATA_CALL            = DS_SYS_EVENT_DUN_DATA_CALL,
} ps_sys_event_data_call_type_enum_type;


/**
  @brief Data type to be used with PS_SYS_EVENT_DATA_CALL_STATUS_CHANGE
*/
typedef struct
{
  ps_sys_event_data_call_status_enum_type        call_status;
  ps_sys_event_data_call_type_enum_type          call_type;
} ps_sys_event_data_call_status_change_type;


/**
  @brief Enum defining DDS Switch result
*/
typedef enum 
{
  PS_SYS_EVENT_DDS_SWITCH_ALLOWED = 0,
  /**< Sent when DDS switch is allowed and was successful */
  PS_SYS_EVENT_DDS_SWITCH_NOT_ALLOWED = 1,
  /**< Sent when DDS switch is not allowed */
  PS_SYS_EVENT_DDS_SWITCH_FAILURE = 2
  /**< Sent when DDS switch is allowed but resulted in failure */
} ps_sys_event_dds_switch_result_type;

/**
  @brief Data type to be used with PS_SYS_EVENT_SWITCH_DDS_IND
*/
typedef struct
{
  ps_sys_event_dds_switch_result_type        result;
  ps_sys_subscription_enum_type              subscription_id;
  void                                     * client_info_ptr;
  ps_sys_client_type                         client_id;
} ps_sys_event_switch_dds_type;

/**
  @brief Data type to be used with 
         PS_SYS_EVENT_3GPP2_OUTAGE_NOTIFICATION
  @see   PS_SYS_EVENT_3GPP2_OUTAGE_NOTIFICATION
*/

typedef ds_sys_event_outage_notification_type ps_sys_event_outage_notification_type;

/**
  @brief This is PS Sys level Callback function provided by client which is to 
         be called when particular event is triggered for a given technology.
  @see   ds_sys_event_cback_f_ptr_type 
  @see   ps_sys_event_reg 
*/
typedef void (* ps_sys_event_cback_f_ptr_type)
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  void                                         * event_info_ptr,
  void                                         * user_data_ptr
);

/**
  @brief This is PS Sys level Callback function provided by client which is to 
         be called while registering for event using ps_sys_event_reg_ex API
   
  @param[in] tech_type            States Technology type
  @param[in] event_name           Name of event
  @param[in] subscription_id      Subscription id
  @param[in] event_info_ptr       Event related information
  @param[in] user_data_ptr        User Data 
   
  @see ds_sys_event_reg  
*/
typedef void (* ps_sys_event_cback_f_ptr_ex_type)
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,
  void                                         * user_data_ptr
);

/**
  @brief Data type to be used with 
         PS_SYS_EVENT_NAT_KEEP_ALIVE_INFO_IND
  @see   PS_SYS_EVENT_NAT_KEEP_ALIVE_INFO_IND
*/
typedef struct
{
  boolean                                        is_start;
  ps_ip_addr_type                                dest_ip_addr;
  uint16                                         source_port;
  uint16                                         dest_port;
  uint32                                         timer_value;
} ps_sys_event_nat_keep_alive_info_type;

/**
  @brief Data type to be used with 
         PS_SYS_EVENT_QUALITY_MEAS_POLICY_CHANGE
  @see   DS_SYS_EVENT_QUALITY_MEAS_POLICY_CHANGE
*/
typedef ds_sys_quality_meas_policy_change_type 
  ps_sys_quality_meas_policy_change_type;

/**
  @brief Data type to be used with 
         PS_SYS_EVENT_SET_WQE_PROFILE_QUALITY_MEAS_INFO
  @see   DS_SYS_EVENT_SET_WQE_PROFILE_QUALITY_MEAS_INFO
*/
typedef ds_sys_wqe_profile_quality_meas_info_type
  ps_sys_set_wqe_profile_quality_meas_info_type;

typedef struct
{
  ps_sys_set_wqe_profile_quality_meas_info_type wqe_quality_info;
  /*Quality information along with WQE profile type. */
  uint8                   iccid_len; /** ICCID len*/
  uint8                   iccid[PS_SYS_MAX_ICCID_LEN]; /** ICCID*/
  uint64                  measurement_id;
} ps_sys_event_set_wqe_profile_quality_meas_info_type;

/**
  @brief Data type to be used with  
         PS_SYS_EVENT_WQE_PROFILE_STATUS
  @see   DS_SYS_EVENT_WQE_PROFILE_STATUS 
*/
typedef ds_sys_wqe_profiles_status_type ps_sys_wqe_profiles_status_type;


/**
  @brief Data type to be used with  
         PS_SYS_EVENT_MODEM_ASST_KEEP_ALIVE_STATUS
  @see   PS_SYS_EVENT_MODEM_ASST_KEEP_ALIVE_STATUS
*/
typedef enum
{
  PS_SYS_EVENT_CONFIG_OK          = 0,
  PS_SYS_EVENT_CONFIG_ERROR       = 1,
  PS_SYS_EVENT_NETWORK_DOWN_ERROR = 2
}ps_sys_modem_asst_ka_result_type_enum;

typedef struct
{
  uint32 ka_handle;
  ps_sys_modem_asst_ka_result_type_enum result;
}ps_sys_event_modem_asst_ka_status_info_type;

/**
  @brief Enum representing different action for IMSI public key
*/
typedef enum
{
  PS_SYS_EVENT_IMSI_PUBLIC_KEY_RETRIEVE = 1,
  PS_SYS_EVENT_IMSI_PUBLIC_KEY_REFRESH = 2
} ps_sys_event_imsi_public_key_action_type;

/**
  @brief Data type to be used with  
         PS_SYS_EVENT_RETRIEVE_IMSI_PUBLIC_KEY_INFO 
*/
typedef struct
{
  ps_sys_event_imsi_public_key_action_type action; 
  /** Value: 
      @ENUM
  */
}ps_sys_event_imsi_public_key_retrieve_info;

/**
  @brief Data type to be used with  
         PS_SYS_EVENT_WLAN_REFRESH_COUNTRY_CODE 
*/
typedef enum{
  PS_SYS_EVENT_WLAN_COUNTRY_CODE_REFRESH = 0x01
}ps_sys_event_wlan_country_code_action_enum;

/**
  @brief Data type to be used with  
         PS_SYS_EVENT_3GPP_LTE_ATTACH_PDN_LIST_ACTION_RESULT 
*/
typedef struct
{
  boolean action_result;
  /**< Values: 
       - 0: Action Failure
       - 1: Action Success 
   */
  void*    user_data_ptr;
}ps_sys_event_3gpp_lte_attach_pdn_list_action_result;

typedef enum {
  PS_SYS_DATA_CONN_PDN_STATUS_UNSPECIFIED  = DS_SYS_DATA_CONN_PDN_STATUS_UNSPECIFIED,
  PS_SYS_DATA_CONN_PDN_STATUS_REJECTED     = DS_SYS_DATA_CONN_PDN_STATUS_REJECTED,
  PS_SYS_DATA_CONN_PDN_STATUS_ACTIVATED    = DS_SYS_DATA_CONN_PDN_STATUS_ACTIVATED,
  PS_SYS_DATA_CONN_PDN_STATUS_DEACTIVATED  = DS_SYS_DATA_CONN_PDN_STATUS_DEACTIVATED
} ps_sys_data_conn_pdn_status_enum_type;

typedef enum {
  PS_SYS_PDP_TYPE_IPV4           = DS_SYS_PDP_TYPE_IPV4,
  PS_SYS_PDP_TYPE_IPV6           = DS_SYS_PDP_TYPE_IPV6,
  PS_SYS_PDP_TYPE_IPV4V6         = DS_SYS_PDP_TYPE_IPV4V6,
  PS_SYS_PDP_TYPE_PPP            = DS_SYS_PDP_TYPE_PPP
} ps_sys_pdp_enum_type;


/**
  @brief Event info type for PS_SYS_EVENT_3GPP_DATA_CONNECTION_STATUS_CHANGE 
  @see   PS_SYS_EVENT_3GPP_DATA_CONNECTION_STATUS_CHANGE
*/
typedef struct
{
  uint8                                     call_id;  /**< Call Id, it would be 0xFF if CM has no assigned Id */
  ds_sys_rat_ex_enum_type                   rat;     /**< RAT information */
  ps_sys_data_conn_pdn_status_enum_type     pdn_status;  /**< PDN Status */
  uint16                                    cause_code; /**< ESM Cause Code */
  char                                      apn_name[DS_SYS_MAX_APN_LEN];  /**< APN Name */
  ps_sys_pdp_enum_type                      pdp_type; /*PDP type*/
} ps_sys_event_3gpp_data_connection_status_info_type;

/**
  @brief Data type to be used with  
         PS_SYS_EVENT_CA_CERT_SUPPORT
*/
typedef ds_sys_event_ca_cert_retrieval_support ps_sys_event_ca_cert_retrieval_support;
  /** Value:
      -0: Not supported
      -1: Supported
  */

/**
  @brief Data type to be used with PS_SYS_EVENT_GET_CA_CERTIFICATE. This event
         is triggered by PS_SYS_IOCTL_GET_CA_CERTIFICATE
*/
typedef struct{
  uint32 tx_id;
  uint8                  cert_dn[DS_SYS_CERT_DN_MAX];
  uint8                  cert_dn_length;
}ps_sys_event_get_ca_certificate;

/**
  @brief Enum defining appplication preference configuration result
*/
typedef enum 
{
  PS_SYS_EVENT_CONFIG_APP_PREF_RESULT_SUCCESS                = 0,
  /**< Aplication preference update was successful */
  PS_SYS_EVENT_CONFIG_APP_PREF_RESULT_INVALID_PRIORITY       = 1,
  /**< Priority is invalid */
  PS_SYS_EVENT_CONFIG_APP_PREF_RESULT_INVALID_WEIGHT         = 2,
  /**< Weight is invalid */
  PS_SYS_EVENT_CONFIG_APP_PREF_RESULT_INVALID_IIN            = 3,
  /**< IIN is invalid */
  PS_SYS_EVENT_CONFIG_APP_PREF_RESULT_MISSING_IIN            = 4,
  /**< IIN is missing to add the application ID */
  PS_SYS_EVENT_CONFIG_APP_PREF_RESULT_MISSING_PRIORITY       = 5,
  /**< Priority is missing to add the application ID */
  PS_SYS_EVENT_CONFIG_APP_PREF_RESULT_APP_LIST_FULL          = 6,
  /**< Application list is full */
  PS_SYS_EVENT_CONFIG_APP_PREF_RESULT_ADD_FAILURE            = 7
  /**< Fail to add Application ID */
} ps_sys_event_config_app_pref_result_enum_type;

/**
  @brief Data type to be used with PS_SYS_EVENT_3GPP_CONFIG_APP_PREF_REUSLT_IND
*/
typedef struct
{
  ps_sys_event_config_app_pref_result_enum_type   result;
  ps_sys_3gpp_app_id_info_type                    app_id;  
  uint16                                          priority;
  uint8                                           iin_list_len;
  ps_sys_3gpp_iin_info_type                       iin_list[PS_SYS_3GPP_IIN_LIST_MAX];
  void                                     * client_info_ptr;
} ps_sys_event_3gpp_config_app_pref_result_type;

/**
  @brief Enum defining appplication preference removal result
*/
typedef enum 
{
  PS_SYS_EVENT_REMOVE_APP_PREF_RESULT_SUCCESS        = 0,
  /**< Application preference removal was successful */
  PS_SYS_EVENT_REMOVE_APP_PREF_RESULT_INVALID_APP_ID = 1
  /**< Application ID is invalid */

} ps_sys_event_3gpp_remove_app_pref_result_enum_type;

/**
  @brief Data type to be used with PS_SYS_EVENT_3GPP_REMOVE_APP_PREF_REESULT_IND
*/
typedef struct
{
  ps_sys_event_3gpp_remove_app_pref_result_enum_type   result;
  boolean                                              app_id_valid;
  ps_sys_3gpp_app_id_info_type                         app_id;  
  void                                               * client_info_ptr;
} ps_sys_event_3gpp_remove_app_pref_result_type;

typedef ds_sys_event_3gpp_lte_cell_ho_info_type ps_sys_event_3gpp_lte_cell_ho_info_type;

/**
  @brief This is the type that is used with /
  @see PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_CHANGE_REQ
*/
typedef struct 
{
  uint32                    num_avail_apns;
  /**< Number of apn entries to apn_pref_sys */

  ps_sys_apn_pref_sys_type  apn_pref_sys[PS_SYS_MAX_APNS]; 
  /**< Array of APNs and preferred RATs*/

} ps_sys_event_ap_asst_apn_pref_sys_change_req_type;

/**
  @brief This is the type that is used with PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_RESULT_IND
  @see PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_RESULT_IND
*/
typedef struct 
{
  boolean                    is_success;
  /**< Preferred system change successful or not */

  ps_sys_apn_pref_sys_type   apn_pref_sys; 
  /**< APN preferred system*/

} ps_sys_event_ap_asst_apn_pref_sys_result_type;

#ifdef QWES_FEATURE_ENABLE 
/**
  @brief Data type to be used with PS_SYS_EVENT_QTEELS_AVAILABLE
   
  @see   PS_SYS_EVENT_QTEELS_AVAILABLE 
*/
typedef struct
{ 
  boolean  is_avail;
} ps_sys_event_qteels_avail_info_type;

typedef boolean ps_sys_event_sync_complete_broadcast_type;
/**
  @brief Data type to be used with PS_SYS_EVENT_QWES_NACK
   
  @see   PS_SYS_EVENT_QWES_NACK 
*/

typedef struct
{
  uint8 ack_type;
} ps_sys_event_qwes_nack_info_type;


#endif /* QWES_FEATURE_ENABLE  */

/*===========================================================================

                      PUBLIC FUNCTION DEFINITIONS

===========================================================================*/

/**
  @brief  This is PS Sys level API for event registeration. This function 
          allows client to register for particual event. In order to
          deregister the event registered using ds_sys_event_reg, 
          user has to call ds_sys_event_dereg API
          
  @see    ds_sys_event_reg
*/
int16 ps_sys_event_reg
(
  ps_sys_tech_enum_type                  tech_type,
  ps_sys_event_enum_type                 event_name,
  ps_sys_event_cback_f_ptr_type          event_cback_f_ptr,
  void                                 * user_data_ptr,
  int16                                * ps_errno
);


/** 
  @brief  This is PS Sys level API for event deregistration. This function 
          deregisters callback for the event.Callback registered through 
          ps_sys_event_reg should be deregistered using this API
          
  @see    ds_sys_event_dereg
*/
int16 ps_sys_event_dereg
(
  ps_sys_tech_enum_type                  tech_type,
  ps_sys_event_enum_type                 event_name,
  ps_sys_event_cback_f_ptr_type          event_cback_f_ptr,
  int16                                * ps_errno
);

/**
  @brief  This is PS Sys level API for event deregistration. This function 
          deregisters callback for the event for particular subscription
          id. Callback registered through ps_sys_event_reg_ex
          should be deregistered using this API.
          
  @see    ds_sys_event_dereg_ex
*/
int16 ps_sys_event_dereg_ex
(
  ps_sys_tech_enum_type                  tech_type,
  ps_sys_event_enum_type                 event_name,
  ps_sys_subscription_enum_type          subscription_id,
  ps_sys_event_cback_f_ptr_ex_type       event_cback_f_ptr,
  int16                                * ps_errno
);

/**
  @brief  This is PS Sys level API for event deregistration. This function 
          deregisters callback for the event for particular subscription
          id and for given client data. Callback registered through 
          ps_sys_event_reg_ex with user data provided  should be 
          deregistered using this API.
          
  @see    ds_sys_event_dereg_ex2
*/
int16 ps_sys_event_dereg_ex2
(
  ps_sys_tech_enum_type                  tech_type,
  ps_sys_event_enum_type                 event_name,
  ps_sys_subscription_enum_type          subscription_id,
  ps_sys_event_cback_f_ptr_ex_type       event_cback_f_ptr,
  void                                 * client_data_ptr,
  int16                                * ps_errno
);


/** 
  @brief  This function sends the indication to all the user who has
          registered for the event for particular technology and default
          subscription

  @param[in] tech_type            States Technology type
  @param[in] event_name           Name of event
  @param[in] event_info           Event specific information 
  @param[out] ps_errno            Error code is filled when return value is not
                                  success. Possible values are
                                  @li DS_EFAULT if parameter is invalid
                                  @li DS_EINVAL if operation is invalid
                                  @li DS_NOMEMORY if memory allocation fails

  @return  DSS_SUCCESS            On Success.
  @return  DSS_ERROR              On failure. 
   
  @code 
   
  Example : 
  Assuming user has already registered for PS_SYS_EVENT_3GPP_LTE_ATTACH_INIT. 
  Modehandlers sends out event indication if there is context transfer from 
  other RAT as follow 
   
  ps_sys_event_3gpp_lte_attach_init_type event_info = NULL;
  event_info.attach_profile = 344;
   
  ret_val =  ps_sys_event_ind(PS_SYS_TECH_3GPP,
                              PS_SYS_EVENT_3GPP_LTE_ATTACH_INIT,
                              &event_info,
                              &ps_errno);
  if(0 > ret_val)
  {
    LOG_MSG_ERROR_1("Event indication failed, err %d", ps_errno);
  } 
   
  @endcode 
*/
int16 ps_sys_event_ind
(
  ps_sys_tech_enum_type             tech_type,
  ps_sys_event_enum_type            event_name,
  void                            * event_info,
  int16                           * ps_errno
);

/** 
  @brief  This function sends the indication to all the user who has
          registered for the event for particular technology and
          subscription

  @param[in] tech_type            States Technology type
  @param[in] event_name           Name of event
  @param[in] subscription_id      Subscription Id
  @param[in] event_info           Event specific information 
  @param[out] ps_errno            Error code is filled when return value is not
                                  success. Possible values are
                                  @li DS_EFAULT if parameter is invalid
                                  @li DS_EINVAL if operation is invalid
                                  @li DS_NOMEMORY if memory allocation fails

  @return  DSS_SUCCESS            On Success.
  @return  DSS_ERROR              On failure. 
*/
int16 ps_sys_event_ind_ex
(
  ps_sys_tech_enum_type             tech_type,
  ps_sys_event_enum_type            event_name,
  ps_sys_subscription_enum_type     subscription_id,
  void                            * event_info,
  int16                           * ps_errno
);

/** 
  @brief  This is PS Sys level API for event registration with ability to
          users to apply filter based on subscription id.In order to 
          deregister the event registered using ds_sys_event_reg_ex,  
          user has to use ps_sys_event_dereg_ex API
          
  @see    ds_sys_event_reg_ex
*/
int16 ps_sys_event_reg_ex
(
  ps_sys_tech_enum_type                  tech_type,
  ps_sys_event_enum_type                 event_name,
  ps_sys_subscription_enum_type          subscription_id,
  ps_sys_event_cback_f_ptr_ex_type       event_cback_f_ptr,
  void                                 * user_data_ptr,
  int16                                * ps_errno
);


#ifdef __cplusplus
}
#endif

#endif /* PS_SYS_EVENT_H */
