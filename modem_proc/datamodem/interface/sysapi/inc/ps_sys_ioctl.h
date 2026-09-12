/**
@file ps_sys_ioctl.h
@brief
This file defines functions, enums and structs used for two modehandlers to 
communicate with each other through ioctls 
*/

#ifndef PS_SYS_IOCTL_H
#define PS_SYS_IOCTL_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        P S _ S Y S _ I O C T L . H


GENERAL DESCRIPTION
  This file defines functions, enums and structs used for two modehandlers
  to communicate with each other through ioctls

Copyright (c) 2011-2020 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/sysapi/inc/ps_sys_ioctl.h#4 $ $DateTime: 2020/04/20 07:55:35 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ---------------------------------------------------------- 
05/27/15    vrk    Added support for filter info logging
03/29/13    svj    New APIs to modify and query attach pdn list 
09/09/11    bvd    Created module 
 
===========================================================================*/


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "ps_iface_defs.h"
#include "ps_sys.h"
#include "ds_sys_ioctl.h"
#include "ps_acl.h"
#include "ps_iface.h"
#include "ps_in.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PS_SYS_MAX_FQDN_LEN 200
/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/

/**
  @brief Defines different SYS IOCTL types  
*/
typedef enum
{
  PS_SYS_IOCTL_MIN         = 0x00,  

  /*3GPP2 specific ioctl common to QMI and DS SYS should be added within 
    the range PS_SYS_IOCTL_3GPP2_MIN to PS_SYS_IOCTL_3GPP2_MIN + 99
    3GPP2 specific ioctl applicable only to QMI should be added within
    the range PS_SYS_IOCTL_3GPP2_MIN +100 to PS_SYS_IOCTL_3GPP2_MIN +200*/
  PS_SYS_IOCTL_3GPP2_MIN         = DS_SYS_IOCTL_3GPP2_MIN,

  PS_SYS_IOCTL_3GPP2_PRE_DORMANCY_SYS_INFO = DS_SYS_IOCTL_3GPP2_PRE_DORMANCY_SYS_INFO,
  /**< 
      This command retrieves the network and service option information for the 
      3GPP2 data session before going into dormancy.
      If the device is not in a 3GPP2 data session, the
      service option is set to the NULL service option (0xFFFF) and the
      data session network is set to no service (0x00).
      If the device is not in a CDMA-1X data session, the service option is
      set to the NULL service option (0xFFFF
  */
  PS_SYS_IOCTL_3GPP2_SET_CAM_TIMER   =  DS_SYS_IOCTL_3GPP2_SET_CAM_TIMER,
  /**< 
      This command sets the Chatty App Manager timer value to the one specified
      in the TLV, cam_timer. This timer is used for CDMA-1X calls only.
      The CAM timer value can be changed at any point for 3GPP2 devices. If set
      during an active CDMA-1X call, the new value is used in the subsequent
      1X data call, and does not affect the current call.
      The default value of the CAM timer is set to 300 seconds. A value of 0 
      causes the timer to be disabled
  */
  PS_SYS_IOCTL_3GPP2_GET_CAM_TIMER   =  DS_SYS_IOCTL_3GPP2_GET_CAM_TIMER,
  /**< 
      This command queries the Chatty App Manager timer setting for the device.
  */
  PS_SYS_IOCTL_3GPP2_SET_SCRM        =  DS_SYS_IOCTL_3GPP2_SET_SCRM,
  /**< 
      Disables/enables the Supplemental Channel Request Message (SCRM).
      The SCRM setting can be changed at any point for 3GPP2 devices. If set
      during an active CDMA-1X call, the new value is used in the subsequent
      1X data call, and does not effect the current call.
  */ 
  PS_SYS_IOCTL_3GPP2_GET_SCRM        =  DS_SYS_IOCTL_3GPP2_GET_SCRM,
  /**< 
      This command queries whether the SCRM is enabled or disabled for the device.
  */ 
  PS_SYS_IOCTL_3GPP2_SET_RDUD        =  DS_SYS_IOCTL_3GPP2_SET_RDUD,
  /**< 
      Enables or disables reduced dormancy followed by unsolicited data.
  */  
  PS_SYS_IOCTL_3GPP2_GET_RDUD        =  DS_SYS_IOCTL_3GPP2_GET_RDUD,
  /**< 
      Retrieves whether reduced dormancy followed by unsolicited
      data is enabled or disabled.
  */ 
  PS_SYS_IOCTL_3GPP2_SET_PAGE_MONITOR_PERIOD = 
    DS_SYS_IOCTL_3GPP2_SET_PAGE_MONITOR_PERIOD,
  /**< 
      Sets the EV-DO slot cycle index.
  */  
  PS_SYS_IOCTL_3GPP2_GET_PAGE_MONITOR_PERIOD = 
    DS_SYS_IOCTL_3GPP2_GET_PAGE_MONITOR_PERIOD,
  /**< 
      Retrieves details about the EV-DO page monitoring period.
  */  
  PS_SYS_IOCTL_3GPP2_SET_FORCE_LONG_SLEEP =
    DS_SYS_IOCTL_3GPP2_SET_FORCE_LONG_SLEEP,
  /**< 
      Enables or disables the EV-DO force long sleep feature.
  */ 
  PS_SYS_IOCTL_3GPP2_GET_CALL_THROTTLE_INFO  = 
    DS_SYS_IOCTL_3GPP2_GET_CALL_THROTTLE_INFO,
  /**< 
      Queries whether the system is call throttled and returns
      the remaining throttled delay.
  */  
  PS_SYS_IOCTL_3GPP2_SET_EHRPD_FALLBACK_APN_LIST = DS_SYS_IOCTL_3GPP2_SET_EHRPD_FALLBACK_APN_LIST,
  /**<
      Sets the fallback APN list specified by control point in persistent
      memory of the device.This list will be used by modem to fall back to 
      HRPD in case of PDN bring up failure on eHRPD.
  */
  PS_SYS_IOCTL_3GPP2_GET_EHRPD_FALLBACK_APN_LIST = DS_SYS_IOCTL_3GPP2_GET_EHRPD_FALLBACK_APN_LIST,
  /**<
      Queries the fallback APN list configured on the device.
  */

  /*3GPP2 specific ioctl internal to data should be added here. 
    Range: DS_SYS_IOCTL_3GPP2_MAX + 1 to DS_SYS_IOCTL_3GPP2_MAX + 99*/
  PS_SYS_IOCTL_3GPP2_INTERNAL_MIN    = DS_SYS_IOCTL_3GPP2_MAX + 1,
     
  PS_SYS_IOCTL_3GPP2_EHRPD_PREREG_HANDOFF_COMPLETE_CMD = 
    PS_SYS_IOCTL_3GPP2_INTERNAL_MIN,
  /**< 
       User shall call this ioctl when the system moves to eHRPD and UE has
       LTE pdn contexts. User need to register for
       PS_SYS_EVENT_3GPP2_EHRPD_PREREG_HANDOFF_COMPLETE_RSP. In response,
       User receives the event
       PS_SYS_EVENT_3GPP2_EHRPD_PREREG_HANDOFF_COMPLETE_RSP.
       @see PS_SYS_EVENT_3GPP2_EHRPD_PREREG_HANDOFF_COMPLETE_RSP
  */

  PS_SYS_IOCTL_3GPP2_GET_PROFILE_ID_FROM_APN_INFO = 
    PS_SYS_IOCTL_3GPP2_INTERNAL_MIN + 1,
  /**< 
      User invokes this IOCTL to get the profile ID for the specified APN Name 
      and/or APN Type. This is used by routing framework. */

  PS_SYS_IOCTL_3GPP2_INTERNAL_MAX   = DS_SYS_IOCTL_3GPP2_MAX + 99,

  /*Max number of 3GPP2 IOCTLs.Includes internal and external 3GPP2 IOCTLs*/
  PS_SYS_IOCTL_3GPP2_MAX            = PS_SYS_IOCTL_3GPP2_INTERNAL_MAX,

  /*3GPP specific ioctl common to QMI and DS SYS should be added within 
    the range PS_SYS_IOCTL_3GPP_MIN to PS_SYS_IOCTL_3GPP_MIN + 99
    3GPP specific ioctl applicable only to QMI should be added within
    the range PS_SYS_IOCTL_3GPP_MIN +100 to PS_SYS_IOCTL_3GPP_MIN +200*/
  PS_SYS_IOCTL_3GPP_MIN             = DS_SYS_IOCTL_3GPP_MIN,

  PS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST = DS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST,
  /**< 
      Users can configure the lte attach pdn list stored in the persistent
      memory of the device anytime. The ioctl contains list of profile IDs
      in the decreasing order of priority. Successful write completion of
      the list triggers the event
      PS_SYS_EVENT_3GPP_LTE_ATTACH_PDN_LIST_CHANGE to be
      generated.
   
      @see PS_SYS_EVENT_3GPP_LTE_ATTACH_PDN_LIST_CHANGE
  */

  PS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST = DS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST,
  /**< 
      Users can query the lte attach pdn list stored in the persistent
      memory of the device anytime. The ioctl contains list of profile IDs
      in the decreasing order of priority. However, it was introduced for App 
      Processor to query the modem for the Attach PDN's list whenever it 
      wants to configure the new list using the IOCTL,
      PS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST. This ioctl returns
      list only list of active lte attach pdn list profile numbers.
   
      @see PS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST
      @see PS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST_EX
  */

  PS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST_EX = DS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST_EX,
  /**< 
      Users can query the lte attach pdn list stored in the persistent
      memory of the device anytime. The ioctl contains list of profile IDs
      in the decreasing order of priority. However, it was introduced for App 
      Processor to query the modem for the Attach PDN's list whenever it 
      wants to configure the new list using the IOCTL,
      PS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST. This ioctl returns
      list of both active and pending lte attach pdn list profile numbers.
   
      @see PS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST
      @see PS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST
  */

  PS_SYS_IOCTL_3GPP_GET_LTE_MAX_ATTACH_PDN_NUM = DS_SYS_IOCTL_3GPP_GET_LTE_MAX_ATTACH_PDN_NUM,
  /**< 
      Users can query the maximum of number of attach pdn supported by
      the device anytime. However, it is mandatory for the client to query
      the number of attach pdn supported before it can set the new attach pdn
      list using PS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST message. 

      @see PS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST
  */

  PS_SYS_IOCTL_3GPP_NSAPI = DS_SYS_IOCTL_3GPP_NSAPI,
  /**< 
     User can query the NSAPI for a specified APN.

      @see DS_SYS_IOCTL_3GPP_NSAPI
  */

  PS_SYS_IOCTL_3GPP_SET_THROTTLED_PDN_REJECT_TIMER = DS_SYS_IOCTL_3GPP_SET_THROTTLED_PDN_REJECT_TIMER,
  /**<
       set throttling timer which starts when a pdn attach 
       request gets rejected due to pdn being in throlled state.
       @see PS_SYS_IOCTL_3GPP_SET_THROTTLED_PDN_REJECT_TIMER
  */

  PS_SYS_IOCTL_3GPP_GET_THROTTLED_PDN_REJECT_TIMER = DS_SYS_IOCTL_3GPP_GET_THROTTLED_PDN_REJECT_TIMER,
  /**<
       Get throttling timer.
       @see PS_SYS_IOCTL_3GPP_GET_THROTTLED_PDN_REJECT_TIMER
  */
  
  PS_SYS_IOCTL_3GPP_BRING_UP_ALWAYS_ON_PDN = DS_SYS_IOCTL_3GPP_BRING_UP_ALWAYS_ON_PDN,
  /**<
       Bring up KAMGR (Keep Alive Manager) PDN.
       @see PS_SYS_IOCTL_3GPP_TEAR_DOWN_ALWAYS_ON_PDN
  */

  PS_SYS_IOCTL_3GPP_TEAR_DOWN_ALWAYS_ON_PDN = DS_SYS_IOCTL_3GPP_TEAR_DOWN_ALWAYS_ON_PDN,
  /**<
       Tear down KAMGR (Keep Alive Manager) PDN.
       @see PS_SYS_IOCTL_3GPP_BRING_UP_ALWAYS_ON_PDN
  */

  PS_SYS_IOCTL_3GPP_GET_DATA_ACTIVITY_STATUS = DS_SYS_IOCTL_3GPP_GET_DATA_ACTIVITY_STATUS,
  /**<
       @see DS_SYS_IOCTL_3GPP_GET_DATA_ACTIVITY_STATUS
  */

  PS_SYS_IOCTL_3GPP_DATA_ACTIVITY_REGISTRATION_STATUS = DS_SYS_IOCTL_3GPP_DATA_ACTIVITY_REGISTRATION_STATUS,
  /**<
       @see DS_SYS_IOCTL_3GPP_DATA_ACTIVITY_REGISTRATION_STATUS
  */

  PS_SYS_IOCTL_3GPP_LTE_ATTACH_PDN_PARAMS = DS_SYS_IOCTL_3GPP_LTE_ATTACH_PDN_PARAMS,
  /**< 
      User can query PDN name anytime however it was introduced for App 
      Processor to query the modem for the Attach PDN's name whenever it 
      gets notified about LTE system via the DATA_SYSTEM_STATUS notification. 
      In response, User should expect attach pdn name.
  */

  PS_SYS_IOCTL_3GPP_GET_PREV_RAT_INFO = DS_SYS_IOCTL_3GPP_GET_PREV_RAT_INFO,
  /**<
       Query for previous 3GPP RAT and SO information
       @see PS_SYS_IOCTL_3GPP_GET_PREV_RAT_INFO
  */

  PS_SYS_IOCTL_3GPP_LTE_EMERGENCY_ATTACH_PARAMS = DS_SYS_IOCTL_3GPP_LTE_EMERGENCY_ATTACH_PARAMS,
  /**<
       Query LTE emergency attach parameters. User can query this whenever it 
       gets notified about 3GPP_LTE_LIMITED_SRVC via the DATA_SYSTEM_STATUS 
       notification.
       @see  DS_SYS_IOCTL_3GPP_LTE_EMERGENCY_ATTACH_PARAMS
  */

  PS_SYS_IOCTL_OMADM_GET_PS_DATA_AVAIL_APPL_SERVICE_INFO = 
    DS_SYS_IOCTL_OMADM_GET_PS_DATA_AVAIL_APPL_SERVICE_INFO,
   /**<
      @see DS_SYS_IOCTL_OMADM_GET_PS_DATA_AVAIL_APPL_SERVICE_INFO
  */

  PS_SYS_IOCTL_OMADM_SET_PS_DATA_AVAIL_APPL_SERVICE_INFO = 
    DS_SYS_IOCTL_OMADM_SET_PS_DATA_AVAIL_APPL_SERVICE_INFO,
   /**<
      @see DS_SYS_IOCTL_OMADM_SET_PS_DATA_AVAIL_APPL_SERVICE_INFO
  */

   /* Add QMI specific 3gpp IOCTLs here. If any of the below IOCTL are
      to be made DS_SYS specific, define a new DS_SYS ioctl with the values
      defined here and change the PS_SYS ioctl definition to the newly
      defined corresponding DS_SYS ioctl.
      For example, we would define a PS_SYS_IOCTL_3GPP_SET_LTE_DATA_RETRY with
      the value DS_SYS_IOCTL_3GPP_MIN + 101 and change
      PS_SYS_IOCTL_3GPP_SET_LTE_DATA_RETRY = 
      DS_SYS_IOCTL_3GPP_SET_LTE_DATA_RETRY
  */

  PS_SYS_IOCTL_3GPP_PS_REG_REQ = DS_SYS_IOCTL_3GPP_PS_REG_REQ,
  /**<
       @see DS_SYS_IOCTL_3GPP_PS_REG_REQ
  */
  
  PS_SYS_IOCTL_3GPP_SET_LTE_DATA_RETRY = PS_SYS_IOCTL_3GPP_MIN + 101,
  /**< 
      User can change the LTE data retry setting. It dynamicaly
      overrides the NV setting that controls the same behavior.
      If enabled, modem will do LTE data reattach on a different
      PDN when attach on current PDN fails. 
  */

  PS_SYS_IOCTL_3GPP_GET_LTE_DATA_RETRY = PS_SYS_IOCTL_3GPP_MIN + 102,
  /**< 
      User can get the current LTE data retry setting. 
  */


  PS_SYS_IOCTL_3GPP_SET_WWAN_ACTIVITY = PS_SYS_IOCTL_3GPP_MIN + 103,
  /**< 
      User can set the activity with respect to WCDMA.
      User can ask modem to start/stop uplink data monitoring
      or move WCDMA from idle state to connected state.
  */

  PS_SYS_IOCTL_3GPP_GET_WWAN_CONN_STATE = PS_SYS_IOCTL_3GPP_MIN + 104,
  /**< 
      User can set HPLMN List through this ioctl.
  */

  PS_SYS_IOCTL_3GPP_SET_HPLMN_LIST = PS_SYS_IOCTL_3GPP_MIN + 105,
  /**< 
      User can retrieve HPLMN List through this ioctl.
  */
  
  PS_SYS_IOCTL_3GPP_GET_HPLMN_LIST = PS_SYS_IOCTL_3GPP_MIN + 106,
  /**< 
      User can set the activity with respect to WCDMA.
      User can ask modem to start/stop uplink data monitoring
      or move WCDMA from idle state to connected state.
  */  
  
  PS_SYS_IOCTL_3GPP_SWITCH_DDS = PS_SYS_IOCTL_3GPP_MIN + 107,
  /**< 
      User request the switch to new DDS through this ioctl. All
      the calls on current DDS would be brought down and indication
      would be sent to user
  */

  PS_SYS_IOCTL_3GPP_GET_LAST_RAB_REJECT_INFO = PS_SYS_IOCTL_3GPP_MIN + 108,
  /**< 
      User can retrieve last RAB Reject information. This interface only 
      supports a subset of all the RAB reject cause codes and it is meant 
      to be used only for VOLTE scenarios. In case of non-VOLTE scenarios, 
      UNKNOWN cause code shall be returned.  
  */  

   PS_SYS_IOCTL_3GPP_SET_DATA_LOW_LATENCY = PS_SYS_IOCTL_3GPP_MIN + 109,
  /**< 
      User can set data low latency mode for data traffic on the modem through 
      this ioctl.
  */  

  PS_SYS_IOCTL_3GPP_GET_DATA_LOW_LATENCY = PS_SYS_IOCTL_3GPP_MIN + 110,
  /**< 
      User can query information of data low latency mode for data traffic 
      on the modem.
  */   

  PS_SYS_IOCTL_3GPP_SET_UL_THRPUT_THRES_CRITERIA = PS_SYS_IOCTL_3GPP_MIN + 111,
  /**< 
      User can set uplink threshold based throughput configuration through this ioctl.
  */  

  PS_SYS_IOCTL_3GPP_SET_DL_THRPUT_THRES_CRITERIA = PS_SYS_IOCTL_3GPP_MIN + 112,
  /**< 
      User can set downlink threshold based throughput configuration through this ioctl.
  */  

  /*3GPP specific ioctl internal to data should be added here. 
    Range: DS_SYS_IOCTL_3GPP_MAX + 1 to DS_SYS_IOCTL_3GPP_MAX + 99*/
  PS_SYS_IOCTL_3GPP_INTERNAL_MIN    = DS_SYS_IOCTL_3GPP_MAX + 1,
  PS_SYS_IOCTL_3GPP_LTE_ATTACH_INIT = PS_SYS_IOCTL_3GPP_INTERNAL_MIN,
  /**< 
       User invokes this IOCTL as a response to
       PS_SYS_EVENT_3GPP_LTE_ATTACH_INIT, providing the info whether a
       context transfer is needed or not. In response, user can expect the
       Modehandler to bring up the ifaces if there are no contexts to be 
       transferred. If there are context to be transferred then the Modehandler 
       does nothing. Once response is recieved, user can bring up ifaces if 
       there are contexts to be transferred
  */
  PS_SYS_IOCTL_3GPP_ANDSF_POLICY_REFRESH = PS_SYS_IOCTL_3GPP_INTERNAL_MIN + 1,
  /**< 
       User invokes this IOCTL to refresh the contents of the ANDSF file via
       HTTP. The result of this file refresh will be conveyed through
       PS_SYS_EVENT_3GPP_POLICY_REFRESH_STATUS.
  */

  PS_SYS_IOCTL_3GPP_LTE_ATTACH_PARAMS_EX = PS_SYS_IOCTL_3GPP_INTERNAL_MIN + 2,
  /**< 
       User invokes this IOCTL to query extended LTE attach params
  */

  PS_SYS_IOCTL_SET_ATTACH_PROFILE_INFO = PS_SYS_IOCTL_3GPP_INTERNAL_MIN + 3,
  /**< 
      User invokes this IOCTL to update the 3GPP MH with the new attach profile
      configuration info*/

  PS_SYS_IOCTL_GET_ATTACH_PROFILE_INFO = PS_SYS_IOCTL_3GPP_INTERNAL_MIN + 4,
  /**< 
      User invokes this IOCTL to get the 3GPP MH with the new attach profile
      configuration info*/

  PS_SYS_IOCTL_3GPP_GET_PROFILE_ID_FROM_APN_INFO = 
    PS_SYS_IOCTL_3GPP_INTERNAL_MIN + 5,
  /**< 
      User invokes this IOCTL to get the profile ID for the specified APN Name 
      and/or APN Type. This is used by routing framework. */

  PS_SYS_IOCTL_3GPP_CONFIG_DYNAMIC_DDS_SWITCH = PS_SYS_IOCTL_3GPP_INTERNAL_MIN + 6,
  /**< 
       User invokes this IOCTL to configure dynamic DDS switch feature 
       on the modem
  */

  PS_SYS_IOCTL_3GPP_CONFIG_APP_PREFERENCES = PS_SYS_IOCTL_3GPP_INTERNAL_MIN + 7,
  /**< 
       User invokes this IOCTL to add or modify the application preferences
       for dynamic DDS switch feature
  */

  PS_SYS_IOCTL_3GPP_REMOVE_APP_PREFERENCES = PS_SYS_IOCTL_3GPP_INTERNAL_MIN + 8,
  /**< 
       User invokes this IOCTL to remove the application preferences for
       dynamic DDS switch feature
  */

  PS_SYS_IOCTL_3GPP_GET_APP_PREFERENCES  = PS_SYS_IOCTL_3GPP_INTERNAL_MIN + 9,
  /**< 
       User invokes this IOCTL to get the application preference for
       a given application for dynamic DDS switch feature
  */

  PS_SYS_IOCTL_3GPP_SET_FOREGROUND_APP_LIST = PS_SYS_IOCTL_3GPP_INTERNAL_MIN + 10,
  /**< 
       User invokes this IOCTL to set a list of foreground applications 
       used for dynamic DDS switching feature
  */

  PS_SYS_IOCTL_3GPP_GET_FOREGROUND_APP_LIST = PS_SYS_IOCTL_3GPP_INTERNAL_MIN + 11,
  /**< 
       User invokes this IOCTL to get a list of foreground applications 
       used for dynamic DDS switching feature
  */

  PS_SYS_IOCTL_3GPP_GET_RAN_ASST_SUPPORT_CONFIG = PS_SYS_IOCTL_3GPP_INTERNAL_MIN + 12,
  /**< 
      This IOCTL queries 3GPP MH about RAN assisted media adaptation
      support configuration. Internal use only by framework only. */

  PS_SYS_IOCTL_3GPP_GET_APPLICATION_LIST  = PS_SYS_IOCTL_3GPP_INTERNAL_MIN +13,
  /**< 
       User invokes this IOCTL to get all applications with application 
       preferences stored in modem for dynamic DDS switch feature
  */

  PS_SYS_IOCTL_3GPP_SET_OS_VERSION_INFO  = PS_SYS_IOCTL_3GPP_INTERNAL_MIN +14,
  /**< 
       User invokes this IOCTL to set AP operating system information
  */

  PS_SYS_IOCTL_3GPP_SET_SW_VERSION_INFO  = PS_SYS_IOCTL_3GPP_INTERNAL_MIN +15,
  /**< 
       User invokes this IOCTL to set AP software information
  */

  PS_SYS_IOCTL_3GPP_GET_LOW_LAT_FILTER_SUPPORT_CONFIG = PS_SYS_IOCTL_3GPP_INTERNAL_MIN +16,
  /**< 
       User invokes this IOCTL to query MH about Low Latency Filter support 
       confirguration for UL traffic prioritization.
  */

  PS_SYS_IOCTL_3GPP_SET_AP_WLAN_MACID  = PS_SYS_IOCTL_3GPP_INTERNAL_MIN +17,
  /**< 
       User invokes this IOCTL to set AP WLAN MACID
  */
  PS_SYS_IOCTL_3GPP_INTERNAL_MAX   = DS_SYS_IOCTL_3GPP_MAX + 99,

  /*Max number of 3GPP IOCTLs.Includes internal and external 3GPP IOCTLs*/
  PS_SYS_IOCTL_3GPP_MAX            = PS_SYS_IOCTL_3GPP_INTERNAL_MAX,

  /*Common ioctl applicable to QMI and DS SYS should be added within 
    the range PS_SYS_IOCTL_COMMON_MIN to PS_SYS_IOCTL_COMMON_MIN + 99
    Common ioctl applicable only to QMI should be added within
    the range PS_SYS_IOCTL_COMMON_MIN +100 to PS_SYS_IOCTL_COMMON_MIN +200*/
  PS_SYS_IOCTL_COMMON_MIN        = DS_SYS_IOCTL_COMMON_MIN,
  PS_SYS_IOCTL_PDN_THROTTLE_INFO = DS_SYS_IOCTL_PDN_THROTTLE_INFO,
  /**< 
      @see DS_SYS_IOCTL_PDN_THROTTLE_INFO
  */

  PS_SYS_IOCTL_DUN_CTRL_RESPONSE = DS_SYS_IOCTL_DUN_CTRL_RESPONSE,
  /**< 
      @see DS_SYS_IOCTL_DUN_CTRL_RESPONSE
  */

  PS_SYS_IOCTL_MODEM_POWER_COST = DS_SYS_IOCTL_MODEM_POWER_COST,
  /**< 
      @see DS_SYS_IOCTL_MODEM_POWER_COST
  */

  PS_SYS_IOCTL_DL_THROUGHPUT_INFO = DS_SYS_IOCTL_DL_THROUGHPUT_INFO,
  /**< 
      @see DS_SYS_IOCTL_DL_THROUGHPUT_INFO
  */


  PS_SYS_IOCTL_SET_APN_CALL_TYPE = DS_SYS_IOCTL_SET_APN_CALL_TYPE,
    /**< 
      @see DS_SYS_IOCTL_SET_APN_CALL_TYPE
  */

  PS_SYS_IOCTL_LOG_ALL_FILTER_INFO = DS_SYS_IOCTL_LOG_ALL_FILTER_INFO,
    /**< 
      @see DS_SYS_IOCTL_LOG_ALL_FILTER_INFO
    */

  PS_SYS_IOCTL_GET_GLOBAL_STATS = DS_SYS_IOCTL_GET_GLOBAL_STATS,
    /**< 
      @see DS_SYS_IOCTL_GET_GLOBAL_STATS
  */

  PS_SYS_IOCTL_GET_APN_PCO_INFO = DS_SYS_IOCTL_GET_APN_PCO_INFO,
    /**< 
      @see DS_SYS_IOCTL_GET_APN_PCO_INFO
  */

  PS_SYS_IOCTL_GET_APN_MSISDN_INFO = DS_SYS_IOCTL_GET_APN_MSISDN_INFO,
    /**< 
      @see DS_SYS_IOCTL_GET_APN_MSISDN_INFO
  */

  PS_SYS_IOCTL_GET_CA_CERTIFICATE = DS_SYS_IOCTL_GET_CA_CERTIFICATE,
   /**< 
      @see DS_SYS_IOCTL_GET_CA_CERTIFICATE
  */

  PS_SYS_IOCTL_GET_CURRENT_DDS = DS_SYS_IOCTL_GET_CURRENT_DDS,
  /**< 
    @see DS_SYS_IOCTL_GET_CURRENT_DDS
  */

  /*Common ioctl internal to data should be added here. 
    Range: DS_SYS_IOCTL_COMMON_MAX + 1 to DS_SYS_IOCTL_COMMON_MAX + 99*/
  PS_SYS_IOCTL_COMMON_INTERNAL_MIN     = DS_SYS_IOCTL_COMMON_MAX + 1,

  PS_SYS_IOCTL_SWITCH_DATA_PATH_TO_SW  = PS_SYS_IOCTL_COMMON_INTERNAL_MIN,
  /**< 
      IOCTL switches the data path in the modem from HW to SW. Also disables setting up 
      of the A2 bridge
  */

  PS_SYS_IOCTL_SWITCH_DATA_PATH_TO_HW  = PS_SYS_IOCTL_COMMON_INTERNAL_MIN + 1,
  /**< 
       IOCTL switches the data path in the modem from SW to HW, enables setting up 
       of the A2 bridge. If attached on LTE RAT, sets up A2 bridge for all RMNET calls. 
       User can request for in-oder delivery of packets.
  */

  PS_SYS_IOCTL_PDN_HO_THROTTLE    = PS_SYS_IOCTL_COMMON_INTERNAL_MIN + 2,
  /**< 
     IOCTL called when Handoff failed, and EPC decides to give up the 
     HO but not release PDN context on SRAT. 
     ps_sys_pdn_ho_throttle_info_type is the argval parameter to this IOCTL
  */
  
  PS_SYS_IOCTL_RELEASE_SIGNALLING_CONNECTION  = PS_SYS_IOCTL_COMMON_INTERNAL_MIN + 3,
  /**<
    This IOCTL is called when there is no Data Activity on all IFACES. This will indicate
    Mode Handler to Release the Signalling connection to NW as data activity is not present. 
   */
  
  PS_SYS_IOCTL_REMOTE_SOCKET_ALLOCATED  = PS_SYS_IOCTL_COMMON_INTERNAL_MIN + 4,
  /**<
    This IOCTL is called when TE responds to the PS_SYS_EVENT_REMOTE_SOCKET_REQUEST. 
   */
  
  PS_SYS_IOCTL_IS_CLAT_SUPPORTED_BY_PROFILE  = PS_SYS_IOCTL_COMMON_INTERNAL_MIN + 5,
  /**<
    This IOCTL is called by CLAT ACL to check if clat is supported from
    below network layer. IOCTL sends request to all technology modehandlers
    and if one of them replies positive for clat, it sends true    
   */   
  
  PS_SYS_IOCTL_GET_IFACE_PTR_FROM_APN  = PS_SYS_IOCTL_COMMON_INTERNAL_MIN + 6,
  /**<
    This IOCTL is called to retrieve iface ptr from apn name. Currently only
    3gpp modehandler handles this ioctl.    
   */ 

  PS_SYS_IOCTL_GET_APN_NAME_FROM_IFACE = PS_SYS_IOCTL_COMMON_INTERNAL_MIN + 7,
  /**<
    This IOCTL is called to retrieve apn name from iface ptr. Currently only
    3gpp modehandler supports this ioctl.    
   */ 

  PS_SYS_IOCTL_GET_RESOLVED_APN = PS_SYS_IOCTL_COMMON_INTERNAL_MIN + 8,
  /**<
    This IOCTL is called to retrieve resolved apn name from apn name. if
    resolved apn does not exist then modehandler shall return same apn name as
    resolved apn. Currently only 3gpp modehandler handles this ioctl.    
   */ 

  PS_SYS_IOCTL_GET_ROAMING_STATUS = PS_SYS_IOCTL_COMMON_INTERNAL_MIN + 9,
   /**<
      This IOCTL is called to receive current roaming status of device for the           given subscription
  */

  PS_SYS_IOCTL_SEND_CA_CERTIFICATE = PS_SYS_IOCTL_COMMON_INTERNAL_MIN + 10,
   /**<
      This IOCTL is called to send CA certificate information to registered 
      modem clients. Internal use only. It is a response for                              PS_SYS_IOCTL_GET_CA_CERTIFICATE.This will trigger a callback response 
      to the clients.
  */

  PS_SYS_IOCTL_COMMON_INTERNAL_MAX     = DS_SYS_IOCTL_COMMON_MAX + 99,

  /*Max number of 3GPP2 IOCTLs.Includes internal and external common IOCTLs*/
  PS_SYS_IOCTL_COMMON_MAX              = PS_SYS_IOCTL_COMMON_INTERNAL_MAX,

  /*WLAN ioctl Min*/
  PS_SYS_IOCTL_WLAN_MIN                = DS_SYS_IOCTL_WLAN_MIN,

  PS_SYS_IOCTL_SET_WQE_PROFILE_TYPE    = DS_SYS_IOCTL_SET_WQE_PROFILE_TYPE,
  /**< 
      @see DS_SYS_IOCTL_SET_WQE_PROFILE_TYPE
  */
  
  PS_SYS_IOCTL_SET_WQE_PROFILE_QUALITY_MEAS_INFO = DS_SYS_IOCTL_SET_WQE_PROFILE_QUALITY_MEAS_INFO,
  /**< 
      @see DS_SYS_IOCTL_SET_WQE_PROFILE_THRESHOLD_INFO
  */

  PS_SYS_IOCTL_WLAN_SET_APN_INFO = DS_SYS_IOCTL_WLAN_SET_APN_INFO,
  /**< 
      @see DS_SYS_IOCTL_WLAN_SET_APN_INFO
  */

  PS_SYS_IOCTL_WLAN_GET_NETWORK_MODE         =  DS_SYS_IOCTL_WLAN_GET_NETWORK_MODE,
  /**<       
       @see DS_SYS_IOCTL_WLAN_GET_NETWORK_MODE
  */
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE  
  PS_SYS_IOCTL_WLAN_UNTHROTTLE =  DS_SYS_IOCTL_WLAN_UNTHROTTLE,
  /**<       
       @see DS_SYS_IOCTL_WLAN_UNTHROTTLE
  */
#endif 

  /*WLAN ioctl internal to data should be added here. 
    Range: DS_SYS_IOCTL_WLAN_MAX + 1 to DS_SYS_IOCTL_WLAN_MAX + 99*/
  PS_SYS_IOCTL_WLAN_INTERNAL_MIN     = DS_SYS_IOCTL_WLAN_MAX + 1,

  PS_SYS_IOCTL_WLAN_WIFI_MEAS_REPORT  = PS_SYS_IOCTL_WLAN_INTERNAL_MIN,
  /**< 
      IOCTL to send WiFI measurement parameters from AP to interested module
      eg. ANDSF
  */

  PS_SYS_IOCTL_WLAN_WIFI_MEAS_REPORT_CONFIG_STATUS  = PS_SYS_IOCTL_WLAN_INTERNAL_MIN + 1,
  /**< 
      IOCTL from AP to signal the status of the previously sent
      QMI_DSD_START_WIFI_MEAS_IND to interested module eg. ANDSF
  */

  PS_SYS_IOCTL_WLAN_NAT_KEEP_ALIVE_CONFIG_STATUS  = PS_SYS_IOCTL_WLAN_INTERNAL_MIN + 2,
  /**< 
      IOCTL from AP to signal the status of the previously sent
      PS_SYS_EVENT_NAT_KEEP_ALIVE_INFO to interested module eg. IWLAN
  */

  PS_SYS_IOCTL_WLAN_SET_WQE_PROFILE_QUALITY_STATUS = PS_SYS_IOCTL_WLAN_INTERNAL_MIN + 3,
  /**< 
      IOCTL from AP to signal the status of the WQE PRofile , eg: if the threshold
      criteria was met or not to interested module. eg. ANDSF.
  */

  PS_SYS_IOCTL_WLAN_CONFIGURE_VOWIFI_SERVICE = PS_SYS_IOCTL_WLAN_INTERNAL_MIN + 4,
  /**< 
      IOCTL from AP to configure and deconfigure VOWIFI service. Presence
      of FQDN or IP address will result in configuration of VOWIFI service and empty FQDN
      and empty IP address would result in deconfiguration of VOWIFI service
  */  

  PS_SYS_IOCTL_WIFI_BAND_VACATE_RESULT = PS_SYS_IOCTL_WLAN_INTERNAL_MIN + 5,
  /**< 
      Indicates that WIFI did not vacate the requested band. This can happen
      for two different reason.
      - First the modem cannot currently vacate 5GHz. In this case LTE-U cannot
        be established
      - Second the modem wasn't in 5GHz band. Continue establishing the LTE-U
        call.
        
  */

  PS_SYS_IOCTL_WLAN_INTERNAL_MAX     = DS_SYS_IOCTL_WLAN_MAX + 99,

  /*Max number of WLAN IOCTLs.Includes internal and external common IOCTLs*/
  PS_SYS_IOCTL_WLAN_MAX              = PS_SYS_IOCTL_WLAN_INTERNAL_MAX,
  PS_SYS_IOCTL_MAX
} ps_sys_ioctl_enum_type;

/**
  @brief Callback function to be returned for ioctl operation 
*/
typedef int16 (* ps_sys_ioctl_tech_cback_f_ptr_type)
(
  ps_sys_ioctl_enum_type          ioctl_name,
  void                          * arg_val_ptr,
  int16                         * ps_errno
);

/**
  @brief Callback function to be returned for extended ioctl 
         operation
*/
typedef int16 (* ps_sys_ioctl_tech_ex_cback_f_ptr_type)
(
  ps_sys_ioctl_enum_type          ioctl_name,
  void                          * arg_val_ptr,
  ps_sys_subscription_enum_type   subscription_id,
  int16                         * ps_errno
);

/**
  @brief Defines structure where diffent modehandlers can register their ioctl
         functions
*/
typedef struct
{
  ps_sys_ioctl_tech_cback_f_ptr_type      tech_f_ptr; /**< Modehandler specific
                                                           Callback */
  ps_sys_ioctl_tech_ex_cback_f_ptr_type   tech_ex_f_ptr; 
}ps_sys_tech_func_type;


/**
  @brief Defines type for PS_SYS_IOCTL_3GPP_LTE_ATTACH_INIT ioctl command. 
   
  @see   PS_SYS_IOCTL_3GPP_LTE_ATTACH_INIT
*/
typedef boolean ps_sys_ioctl_3gpp_lte_attach_init_type;

/**
  @brief Defines structure for PS_SYS_IOCTL_PDN_THROTTLE_INFO ioctl command.
   
  @see   PS_SYS_IOCTL_PDN_THROTTLE_INFO 
  @see   ps_sys_pdn_ctxt_throttle_info_type 
*/
typedef ps_sys_pdn_ctxt_throttle_info_type 
          ps_sys_ioctl_pdn_ctxt_throttle_info_type;
typedef ps_sys_pdn_throttle_info_type ps_sys_ioctl_pdn_throttle_info_type;
/**
  @brief Defines structure for PS_SYS_IOCTL_PDN_HO_THROTTLE 
    	 ioctl command.
   
  @see   PS_SYS_IOCTL_PDN_HO_THROTTLEs  
*/
typedef struct
{
  boolean                       is_ipv4_throttled;/**< ipv4 is throttled */
  boolean                       is_ipv6_throttled;/**< ipv6 is throttled */
  char                          apn_string[PS_SYS_MAX_APN_LEN]; /**< PDN name */
  uint16                        apn_string_len; /**< PDN name length */
  ps_iface_net_down_reason_type net_down_reason; /**< Net down reason*/
} ps_sys_pdn_ho_throttle_info_type;

/**
  @brief Defines structure for 
         PS_SYS_IOCTL_3GPP2_EHRPD_PREREG_HANDOFF_COMPLETE_CMD ioctl command.
   
  @see   PS_SYS_IOCTL_3GPP2_EHRPD_PREREG_HANDOFF_COMPLETE_CMD  
*/
typedef struct
{
  boolean all_pdn_ctxts_prereg; /**< true means all iface are prereg and 
                                     false means not all iface are prereg */
} ps_sys_ioctl_3gpp2_prereg_handoff_complete_cmd_type;

/**
  @brief Defines enum type representing different ip types
   
  @see   ps_sys_ioctl_3gpp_lte_attach_pdn_params_type 
*/
typedef enum
{
  PS_SYS_IOCTL_IP_SUPPORT_TYPE_IPV4,     /**<  IPV4 type */
  PS_SYS_IOCTL_IP_SUPPORT_TYPE_IPV6,     /**<  IPV4 type */
  PS_SYS_IOCTL_IP_SUPPORT_TYPE_IPV4IPV6  /**<  IPV4 and IPV6 type */
} ps_sys_ioctl_ip_support_type_enum_type;

/**
  @brief Defines structure for PS_SYS_IOCTL_3GPP_LTE_ATTACH_PDN_PARAMS ioctl 
         command.
   
  @see   PS_SYS_IOCTL_3GPP_LTE_ATTACH_PDN_PARAMS 
*/
typedef struct
{
  ps_sys_ioctl_ip_support_type_enum_type ip_type; /** <  IP types */
  char  apn_string[PS_SYS_MAX_APN_LEN]; /**< Attached PDN name */
  uint8 apn_string_len;                /**<  Length of attached PDN name */
  boolean ota_attach_performed;        /**<  LTE OTA Attach state */
} ps_sys_ioctl_3gpp_lte_attach_pdn_params_type;

/**
  @brief Defines structure for PS_SYS_IOCTL_3GPP_LTE_ATTACH_PARAMS_EX ioctl 
         command.
   
  @see   PS_SYS_IOCTL_3GPP_LTE_ATTACH_PARAMS_EX
*/
typedef struct
{
  ps_sys_lte_attach_params_type  attach_params; /**< attach params */
  boolean                        ota_attach_performed; /**<  LTE OTA Attach state */
} ps_sys_ioctl_3gpp_lte_attach_params_ex_type;

/**
  @brief Defines structure for PS_SYS_IOCTL_DUN_CTRL_RESPONSE ioctl command 
   
  @details Allows application like connection manager to tell modehandlers 
           whether to allow dun call. This is in response to event sent by 
           modehandlers for entitilement check or silen redial check
*/
typedef struct
{
  unsigned char         cb_data;
  boolean               allow_call;
} ps_sys_ioctl_dun_ctrl_response_type;

/**
  @brief Defines structure for PS_SYS_IOCTL_3GPP_SWITH_DATA_PATH_TO_HW ioctl command 
   
  set to request in-order delivery of data
*/
typedef uint32 ps_sys_ioctl_in_order_delivery_info;

/**
  @brief Refers to the no. of max lte attach pdns supported by 
         the device
   
  @see   PS_SYS_IOCTL_3GPP_GET_LTE_MAX_ATTACH_PDN_NUM
*/
typedef ds_sys_ioctl_3gpp_lte_max_attach_pdn_num_type ps_sys_ioctl_3gpp_lte_max_attach_pdn_num_type;
/**
  @brief This is the type that is used for 
         PS_SYS_IOCTL_3GPP_SET_LTE_DATA_RETRY and
         PS_SYS_IOCTL_3GPP_GET_LTE_DATA_RETRY
  
*/
typedef struct
{
  boolean lte_data_retry;
} ps_sys_ioctl_lte_data_retry_type;

typedef ds_sys_modem_power_cost_enum_type ps_sys_modem_power_cost_enum_type;

/**
  @brief Enum describing different data session
  @see   ps_sys_ioctl_3gpp2_pre_dormancy_sys_info_type
*/
typedef enum
{
  PS_SYS_IOCTL_DATA_SESSION_NO_SERVICE  = 0,
  PS_SYS_IOCTL_DATA_SESSION_NETWORK_1X  = 2,
  PS_SYS_IOCTL_DATA_SESSION_NETWORK_HDR = 4
} ps_sys_ioctl_data_session_network_enum_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP2_PRE_DORMANCY_SYS_INFO
  @see   PS_SYS_IOCTL_3GPP2_PRE_DORMANCY_SYS_INFO
*/
typedef struct
{
  uint16                                       service_option;
  ps_sys_ioctl_data_session_network_enum_type  data_session_network;
} ps_sys_ioctl_3gpp2_pre_dormancy_sys_info_type;

/**
  @brief This is the type that is used with 
         PS_SYS_IOCTL_3GPP2_SET_CAM_TIMER/
         PS_SYS_IOCTL_3GPP2_GET_CAM_TIMER 
  @see   PS_SYS_IOCTL_3GPP2_SET_CAM_TIMER/ 
         PS_SYS_IOCTL_3GPP2_GET_CAM_TIMER 
*/
typedef ds_sys_ioctl_3gpp2_cam_timer_type ps_sys_ioctl_3gpp2_cam_timer_type;

/**
  @brief This is the type that is used with 
         PS_SYS_IOCTL_3GPP2_SET_SCRM/
         PS_SYS_IOCTL_3GPP2_GET_SCRM
  @see   PS_SYS_IOCTL_3GPP2_SET_SCRM/ 
         PS_SYS_IOCTL_3GPP2_GET_SCRM
                                      */
typedef ds_sys_ioctl_3gpp2_scrm_type ps_sys_ioctl_3gpp2_scrm_type;

/**
  @brief This is the type that is used with 
         PS_SYS_IOCTL_3GPP2_FORCE_LONG_SLEEP
  @see   PS_SYS_IOCTL_3GPP2_FORCE_LONG_SLEEP
*/
typedef ds_sys_ioctl_3gpp2_force_long_sleep_type ps_sys_ioctl_3gpp2_force_long_sleep_type;

/**
  @brief This is the type that is used with 
         PS_SYS_IOCTL_3GPP2_SET_RDUD/
         PS_SYS_IOCTL_3GPP2_GET_RDUD
  @see   PS_SYS_IOCTL_3GPP2_SET_RDUD/ 
         PS_SYS_IOCTL_3GPP2_GET_RDUD
                                      */
typedef ds_sys_ioctl_3gpp2_rdud_type ps_sys_ioctl_3gpp2_rdud_type;

/**
  @brief Callback function provided by client which is to be called when
         evdo set page monitor is triggered
   
  @param[in] evt              Event
  @param[in] failure_code     Failure code
  
  @see PS_SYS_IOCTL_3GPP2_SET_PAGE_MONITOR_PERIOD  
*/

/**
  @brief Data type used in ds_sys_ioctl_3gpp2_page_monitor_type
  @see   ps_sys_ioctl_3gpp2_page_monitor_type
*/
typedef uint8 ps_sys_ioctl_3gpp2_page_monitor_period_type;
/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP2_SET_PAGE_MONITOR_PERIOD/
         PS_SYS_IOCTL_3GPP2_GET_PAGE_MONITOR_PERIOD 
         
  @see 
         PS_SYS_IOCTL_3GPP2_SET_PAGE_MONITOR_PERIOD/
         PS_SYS_IOCTL_3GPP2_GET_PAGE_MONITOR_PERIOD
*/
typedef struct
{
  ps_sys_ioctl_3gpp2_page_monitor_period_type       page_monitor_period;
  void (* ps_sys_ioctl_set_evdo_pmp_cb) (int16   evt, 
                                         ps_hdr_slotted_mode_failure_code_enum_type*    failure_code);
} ps_sys_ioctl_3gpp2_set_page_monitor_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP2_GET_CALL_THROTTLE_INFO
         
  @see 
         PS_SYS_IOCTL_3GPP2_GET_CALL_THROTTLE_INFO
*/
typedef void (* ps_sys_ioctl_call_thr_info_hdlr_cb) (void*, int16);

typedef struct
{
  ps_sys_ioctl_call_thr_info_hdlr_cb  call_thro_info_f_ptr;
} ps_sys_ioctl_3gpp2_call_thr_info_hdlr_cb_type;

/**
  @brief Data type to be used with PS_SYS_IOCTL_NSAPI
*/
typedef ds_sys_ioctl_3gpp_nsapi_type ps_sys_ioctl_3gpp_nsapi_type;

/**
  @brief Data type to be used with PS_SYS_IOCTL_3GPP_GET_DATA_ACTIVITY_STATUS
*/
typedef ds_sys_ioctl_3gpp_get_data_activity_status_type ps_sys_ioctl_3gpp_get_data_activity_status_type;

/**
  @brief Data type to be used with PS_SYS_IOCTL_3GPP_DATA_ACTIVITY_REGISTRATION_STATUS
*/
typedef ds_sys_ioctl_3gpp_data_activity_registration_status_type ps_sys_ioctl_3gpp_data_activity_registration_status_type;

/**
  @brief Data type to be used with PS_SYS_IOCTL_DL_THROUGHPUT_INFO
*/
typedef ds_sys_ioctl_dl_throughput_info_type ps_sys_ioctl_dl_througput_info_type;
/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_SET_APN_CALL_TYPE
*/
typedef ds_sys_ioctl_apn_call_type ps_sys_ioctl_apn_call_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_WLAN_NAT_KEEP_ALIVE_CONFIG_STATUS
*/
typedef enum 
{
  PS_SYS_WLAN_NAT_KEEP_ALIVE_CONFIG_OK, 
  /**< Configuration OK \n */
  PS_SYS_WLAN_NAT_KEEP_ALIVE_CONFIG_ERROR, 
  /**< Configuration error \n */
} ps_sys_wlan_nat_keep_alive_config_status_enum_type;

typedef struct
{
  ps_sys_wlan_nat_keep_alive_config_status_enum_type status;
} ps_sys_wlan_nat_keep_alive_config_type;

/**
  @brief Data type to be used with PS_SYS_IOCTL_GET_GLOBAL_STATS 
*/
typedef ds_sys_ioctl_global_stats_type ps_sys_ioctl_global_stats_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_WLAN_SET_WQE_PROFILE_QUALITY_STATUS
*/
typedef ds_sys_wlan_set_wqe_profile_status_type ps_sys_wlan_set_wqe_profile_status_type;

/**
  @brief Data type to be used with both
         PS_SYS_IOCTL_GET_ATTACH_PROFILE_INFO,
         PS_SYS_IOCTL_SET_ATTACH_PROFILE_INFO
*/

typedef enum
{
  PS_SYS_ATTACH_RULE_V4_ADDR_DONTCARE_AND_V6_ADDR_DONTCARE  = 0,
  /**< Default value: Attach address rule v4 and v6 dont care. */
  PS_SYS_ATTACH_RULE_V4_ADDR_MANDATORY_AND_V6_ADDR_DONTCARE = 1,
  /**< Attach address rule v4 mandatory dont care */
  PS_SYS_ATTACH_RULE_V4_ADDR_DONTCARE_AND_V6_ADDR_MANDATORY = 2,
  /**< Attach address rule v4 dont care and v6 mandatory. */
  PS_SYS_ATTACH_RULE_V4_ADDR_MANDATORY_AND_V6_ADDR_MANDATORY = 3,
  /**< Attach address rule v4 and v6 mandatory. */
  PS_SYS_ATTACH_RULE_V4_ADDR_MANDATORY_OR_V6_ADDR_MANDATORY = 4
  /**< Attach address rule v4 or v6 mandatory. */
} ps_sys_attach_address_rule_enum_type;

typedef struct
{
  ps_sys_attach_address_rule_enum_type ip_addr_rules;
  /**< Address rule for Ip address. */
  boolean ip_addr_rules_valid;
  ps_sys_attach_address_rule_enum_type dns_addr_rules;
  /**< Address rule for Dns address. */
  boolean dns_addr_rules_valid;
  ps_sys_attach_address_rule_enum_type pcscf_addr_rules;
  /**< Address rule for PCSCF address. */
  boolean pcscf_addr_rules_valid;
  ps_sys_attach_address_rule_enum_type pcscf_or_dns_addr_rules;
  /**< Address rule for PCSCF OR DNS address. */
  boolean pcscf_or_dns_addr_rules_valid;
} ps_sys_attach_profile_rules_type;

typedef struct
{
  uint16 profile_id;
  /**< Attach profile id*/

  ps_sys_attach_profile_rules_type attach_apn_rules;
  /**< Address rules for attach apn. */

  ps_sys_attach_profile_rules_type on_demand_apn_rules;
  /**< Address rules for on Demand apn.*/

} ps_sys_attach_rules_info_type;

/**
  @brief Data type to be used with both
         PS_SYS_IOCTL_SET_ATTACH_PROFILE_INFO
*/

typedef enum 
{
  PS_SYS_ATTACH_PROFILE_ACTION_DEFAULT = 0,
  /**< Not the last profile and no action required.*/
  PS_SYS_ATTACH_PROFILE_ACTION_NO_DETACH = 1,
  /**< Last attach profile and NO Detach required.*/
  PS_SYS_ATTACH_PROFILE_ACTION_DETACH = 2
  /**< Last attach profile and detach required. */
} ps_sys_attach_profile_action_enum_type;

typedef struct
{
  boolean append_flag;
  /**< Is new or append flag. 
    TRUE: append to existing configuration.
    FALSE: new configuration. */

  ps_sys_attach_profile_action_enum_type attach_config_action;
  /**< Value specifying the action needed to be taken for 
    the given configuration
  */
 
} ps_sys_attach_profile_action_info_type;

typedef struct
{
  ps_sys_attach_rules_info_type attach_rules;
  /**< Attach Profile Rule associated with a profile.*/
  
  ps_sys_attach_profile_action_info_type attach_profile_action_type;
  /**< Attach profile action type.*/

} ps_sys_set_attach_profile_info_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_IS_CLAT_SUPPORTED_BY_PROFILE
  @see   PS_SYS_IOCTL_IS_CLAT_SUPPORTED_BY_PROFILE
*/
typedef struct
{
  acl_policy_info_type   * acl_policy_info_ptr;
  boolean                  is_clat_supported;
} ps_sys_ioctl_clat_support_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_GET_IFACE_PTR_FROM_APN and PS_SYS_IOCTL_GET_APN_NAME_FROM_IFACE
  @see   PS_SYS_IOCTL_GET_IFACE_PTR_FROM_APN
  @see   PS_SYS_IOCTL_GET_APN_NAME_FROM_IFACE
*/
typedef struct
{
  char              apn_name[PS_SYS_MAX_APN_LEN];
  ps_iface_type   * iface_ptr;
} ps_sys_ioctl_apn_iface_info_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_GET_RESOLVED_APN
  @see   PS_SYS_IOCTL_GET_RESOLVED_APN
*/
typedef struct
{
  char              apn_name[PS_SYS_MAX_APN_LEN];
  char              resolved_apn_name[PS_SYS_MAX_APN_LEN];
} ps_sys_ioctl_get_resolved_apn_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP_SET_WWAN_ACTIVITY
*/

typedef enum 
{
  PS_SYS_WWAN_UL_DATA_MONITOR_STOP = 0x0000, 
  /**<  WWAN Uplink Data monitor stop. */
  PS_SYS_WWAN_UL_DATA_MONITOR_START = 0x0001, 
  /**<  WWAN Uplink Data monitor start. */
  PS_SYS_WWAN_TRANSITION_TO_CONNECTED = 0x0002, 
  /**<  Transition WWAN to connected state. */
} ps_sys_wwan_activity_enum_type;

typedef struct
{
  ps_sys_wwan_activity_enum_type wwan_activity;
} ps_sys_wlan_set_wwan_activity_info_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP_SWITCH_DDS
  @see   PS_SYS_IOCTL_3GPP_SWITCH_DDS
*/

typedef struct
{
  ps_sys_subscription_enum_type   subscription_id;
  void                          * client_info_ptr;
  ps_sys_dds_switch_type_enum     dds_switch_type;
  ps_sys_client_type              client_id;
} ps_sys_ioctl_switch_dds_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_WLAN_CONFIGURE_VOWIFI_SERVICE
  @see   PS_SYS_IOCTL_WLAN_CONFIGURE_VOWIFI_SERVICE
*/
typedef struct
{
  char                      fqdn[PS_SYS_MAX_FQDN_LEN];
  /**< Alphanumeric string for epdg configuration */
  struct ps_in_addr         v4_addr;
  /**< IPv4 address */
  struct ps_in6_addr        v6_addr;
  /**< IPv6 address */
} ps_sys_ioctl_wlan_configure_vowifi_service_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_WIFI_BAND_VACATE_RESULT
*/

typedef enum 
{
  PS_SYS_WIFI_BAND_VACATE_NOT_ALLOWED = 0x0000, 
  /**<  Currently not able to vacate 5GHz. */
  PS_SYS_WIFI_BAND_VACATE_CURRENTLY_VACATED = 0x0001, 
  /**<  At time of the vacate request WIFI was not on 5GHz. */
} ps_sys_wifi_band_vacate_enum_type;

typedef struct
{
  ps_sys_wifi_band_vacate_enum_type wifi_band_vacate_result;
} ps_sys_ioctl_wifi_band_vacate_info_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_GET_ROAMING_STATUS
  @see   PS_SYS_IOCTL_GET_ROAMING_STATUS
*/
typedef struct
{
  uint8       profile_id_3GPP;
  uint8       profile_id_3GPP2;
  boolean     in_roaming;
} ps_sys_ioctl_roaming_status_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_WLAN_SET_APN_INFO 
  @see   PS_SYS_IOCTL_WLAN_SET_APN_INFO 
*/
typedef ds_sys_ioctl_wlan_apn_info_type ps_sys_ioctl_wlan_apn_info_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP_SET_DATA_LOW_LATENCY and
         PS_SYS_IOCTL_3GPP_GET_DATA_LOW_LATENCY
  @see   PS_SYS_IOCTL_3GPP_SET_DATA_LOW_LATENCY
  @see   PS_SYS_IOCTL_3GPP_GET_DATA_LOW_LATENCY
*/

typedef enum
{
  PS_SYS_LATENCY_INVALID              = 0,
  PS_SYS_LATENCY_NORMAL               = 1,  
  PS_SYS_LATENCY_LOW                  = 2,
  PS_SYS_LATENCY_VERY_LOW             = 3,
  PS_SYS_LATENCY_SUPER_LOW            = 4,
  PS_SYS_LATENCY_CONFIGURABLE_LEVEL_1 = 5,
  PS_SYS_LATENCY_CONFIGURABLE_LEVEL_2 = 6
}ps_sys_latency_level_enum_type;

typedef enum
{
  PS_SYS_PRIO_PS_DATA_INVALID   = 0,
  PS_SYS_PRIO_PS_DATA_OFF       = 1,  
  PS_SYS_PRIO_PS_DATA_ON        = 2,
}ps_sys_prio_ps_data_enum_type;

typedef enum
{
  PS_SYS_ENABLE_RRC_CONN_EXT_INVALID   = 0,
  PS_SYS_ENABLE_RRC_CONN_EXT_OFF       = 1,  
  PS_SYS_ENABLE_RRC_CONN_EXT_ON        = 2,
}ps_sys_enable_rrc_conn_ext_enum_type;

typedef struct
{
  ps_sys_latency_level_enum_type               ul_latency_level;

  ps_sys_latency_level_enum_type               dl_latency_level;

  ps_sys_prio_ps_data_enum_type                prioritize_ps_data;

  ps_sys_enable_rrc_conn_ext_enum_type         enable_rrc_connection_extension;
}ps_sys_ioctl_data_low_latency_info_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP2_MAP_PROFILE_NUM_BY_APN_INFO and
         PS_SYS_IOCTL_3GPP_MAP_PROFILE_NUM_BY_APN_INFO
  @see   PS_SYS_IOCTL_3GPP2_MAP_PROFILE_NUM_BY_APN_INFO
  @see   PS_SYS_IOCTL_3GPP_MAP_PROFILE_NUM_BY_APN_INFO
*/
    
typedef struct
{
/**< Input Param */
  boolean              apn_name_is_valid;
  uint8                apn_name_length;
  byte                 apn_name[PS_SYS_MAX_APN_LEN];  
  boolean              apn_type_is_valid;
  ps_sys_apn_enum_type apn_type;
  
/**< Output Param */
  uint16              *profile_num;
}ps_sys_ioctl_mapping_apn_info_type;

typedef enum{
  PS_SYS_THRESHOLD_ACTION_INVALID = 0x00,
  PS_SYS_THRESHOLD_ACTION_STOP    = 0x01,
  PS_SYS_THRESHOLD_ACTION_START   = 0x02
}ps_sys_threshold_action_type;

typedef struct{
  boolean                      is_valid_rat;
  ps_sys_rat_ex_enum_type      rat;
  boolean                      is_valid_hyst_time;
  uint32                       hyst_time;
  boolean                      is_valid_hyst_mag_chng_size;
  uint32                       hyst_mag_chng_size;
  boolean                      is_valid_threshold_list;
  uint32                       threshold_list[PS_SYS_THRESHOLD_LIST_LEN];
  uint8                        threshold_list_len;
  ps_sys_threshold_action_type action;
  boolean                      reset_to_defaults;
  /**< Values:\n
    -0: Do not reset (Default)
    -1: Reset throughput thresholds to default */
}ps_sys_thrput_thres_criteria_type;

/**
  @brief Data type to be used to set uplink throughput configuration
   
  @see   PS_SYS_IOCTL_3GPP_SET_UL_THRPUT_THRES_CRITERIA
*/
typedef ps_sys_thrput_thres_criteria_type ps_sys_ul_thrput_thres_criteria_type;

/**
  @brief Data type to be used to set downlink throughput configuration
   
  @see   PS_SYS_IOCTL_3GPP_SET_DL_THROUGHPUT_THRESHOLD
*/
typedef ps_sys_thrput_thres_criteria_type ps_sys_dl_thrput_thres_criteria_type;


/**
  @brief Data type to be used with 
          PS_SYS_IOCTL_3GPP_LTE_EMERGENCY_ATTACH_PARAMS
  @see    PS_SYS_IOCTL_3GPP_LTE_EMERGENCY_ATTACH_PARAMS
*/
typedef ds_sys_ioctl_3gpp_emer_attach_params_type ps_sys_ioctl_3gpp_emer_attach_params_type;

/**
  @brief Data type to be used with PS_SYS_IOCTL_GET_CA_CERTIFICATE.
*/
typedef ds_sys_ioctl_get_ca_cert_info_type ps_sys_ioctl_get_ca_cert_info_type;

/**
  @brief Data type to be used with PS_SYS_IOCTL_SEND_CA_CERTIFICATE,
         a response for PS_SYS_IOCTL_GET_CA_CERTIFICATE.This will trigger
         a callback response to the clients.
*/

typedef ds_sys_ca_cert_result_enum_type ps_sys_ca_cert_result_enum_type;

typedef struct
{
  uint32                                  tx_id;
  q_type                                 *ca_cert_q_ptr;
  ps_sys_ca_cert_result_enum_type         failure_info;
}ps_sys_ioctl_send_ca_cert_info_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP_CONFIG_DYNAMIC_DDS_SWITCH
  @see   PS_SYS_IOCTL_3GPP_CONFIG_DYNAMIC_DDS_SWITCH
*/
typedef enum
{
  PS_SYS_DYNAMIC_DDS_SWITCH_OFF = 0,
  PS_SYS_DYNAMIC_DDS_SWITCH_ON  = 1
}ps_sys_ioctl_3gpp_config_dynamic_dds_switch;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP_SET_FOREGROUND_APP_LIST
         PS_SYS_IOCTL_3GPP_GET_FOREGROUND_APP_LIST
  @see   PS_SYS_IOCTL_3GPP_SET_FOREGROUND_APP_LIST
         PS_SYS_IOCTL_3GPP_GET_FOREGROUND_APP_LIST
*/

#define PS_SYS_IOCTL_3GPP_FG_APP_LIST_MAX    4

typedef struct{
  ps_sys_3gpp_app_id_info_type ps_sys_ioctl_3gpp_fg_app_list[PS_SYS_IOCTL_3GPP_FG_APP_LIST_MAX];
  uint8                              fg_app_list_length;
}ps_sys_ioctl_3gpp_fg_app_list_info_type;


/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP_CONFIG_APP_PREFERENCES
  @see   PS_SYS_IOCTL_3GPP_CONFIG_APP_PREFENENCES
*/

typedef struct{

  ps_sys_3gpp_app_id_info_type app_id;  
  boolean priority_valid;
  uint16  priority;
  boolean iin_list_valid;
  uint8   iin_list_len;
  ps_sys_3gpp_iin_info_type iin_list[PS_SYS_3GPP_IIN_LIST_MAX];
  void                          * client_info_ptr;
}ps_sys_ioctl_3gpp_config_app_pref_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP_REMOVE_APP_PREFERENCES
  @see   PS_SYS_IOCTL_3GPP_REMOVE_APP_PREFERENCES
*/

typedef struct{

  ps_sys_3gpp_app_id_info_type              app_id;
  boolean                                   remove_all;
  /**< Values: \n
        - 0: Remove specified application 
        - 1: Remove all applications
  */
  void                                    * client_info_ptr;
}ps_sys_ioctl_3gpp_remove_app_pref_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP_GET_APP_PREFERENCES
  @see   PS_SYS_IOCTL_3GPP_GET_APP_PREFERENCES
*/
typedef struct{

ps_sys_3gpp_app_id_info_type app_id;
uint16  priority;
uint8 iin_list_len;
ps_sys_3gpp_iin_info_type iin_list[PS_SYS_3GPP_IIN_LIST_MAX];

}ps_sys_ioctl_3gpp_get_app_pref;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP_GET_RAN_ASST_SUPPORT_CONFIG
  @see   PS_SYS_IOCTL_3GPP_GET_RAN_ASST_SUPPORT_CONFIG

*/
typedef struct
{
  boolean    bit_rate_update_support;
  /* Values:\n
     -0: Bit rate update not supported 
     -1: Bit rate update supported 
  */
} ps_sys_ioctl_3gpp_ran_asst_support_config_type; 

typedef enum 
{
  PS_SYS_IOCTL_3GPP_PS_REG = DS_SYS_IOCTL_3GPP_PS_REG,
  PS_SYS_IOCTL_3GPP_PS_DEREG = DS_SYS_IOCTL_3GPP_PS_DEREG
} ps_sys_ioctl_3gpp_ps_reg_enum_type;

/**
  @brief Data type to be used with PS_SYS_IOCTL_3GPP_PS_REG_REQ
*/
typedef struct
{
  ps_sys_ioctl_3gpp_ps_reg_enum_type  op;
} ps_sys_ioctl_3gpp_ps_reg_type;

typedef struct
{
  uint32                                   num_services;
  /** Number of services*/
  ps_sys_ps_data_avail_info_type           service_info[PS_SYS_APPL_SERVICE_NAME_MAX];
  /** Array of Service and type*/
} ps_sys_ioctl_omadm_ps_data_avail_service_type;

#define PS_SYS_IOCTL_3GPP_APP_LIST_MAX 100

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP_GET_APPLICATION_LIST
  @see   PS_SYS_IOCTL_3GPP_GET_APPLICATION_LIST
*/
typedef struct{

  uint8 app_id_list_len;
  // ps_sys_3gpp_app_id_info_type app_id_list[PS_SYS_IOCTL_3GPP_APP_LIST_MAX];
  const char* app_id_list[PS_SYS_IOCTL_3GPP_APP_LIST_MAX];
}ps_sys_ioctl_3gpp_get_app_list;


/**
  @brief This is the type that is used with PS_SYS_IOCTL_3GPP_SET_OS_VERSION_INFO
         and PS_SYS_IOCTL_3GPP_SET_SW_VERSION_INFO
         and PS_SYS_IOCTL_3GPP_SET_AP_WLAN_MACID

  @see PS_SYS_IOCTL_3GPP_SET_OS_VERSION_INFO
  @see PS_SYS_IOCTL_3GPP_SET_SW_VERSION_INFO
  @see PS_SYS_IOCTL_3GPP_SET_AP_WLAN_MACID
*/

#define PS_SYS_AP_VER_MAX_LEN       61
typedef struct 
{
  char  version_info[PS_SYS_AP_VER_MAX_LEN];
  uint8 version_info_length;
} ps_sys_ioctl_3gpp_ap_version_info_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_3GPP_GET_LOW_LAT_FILTER_SUPPORT_CONFIG
  @see   PS_SYS_IOCTL_3GPP_GET_LOW_LAT_FILTER_SUPPORT_CONFIG

*/
typedef struct
{
  boolean    low_lat_filter_support;
  /* Values:\n
     -0: Low Latency Filter installation not supported 
     -1: Low Latency Filter installation supported 
  */
} ps_sys_ioctl_3gpp_low_lat_filter_support_config_type;

/*===========================================================================

                      PUBLIC FUNCTION DEFINITIONS

===========================================================================*/

/**
  @brief  This is PS Sys level API for ioctl. This function performs the
          operation for particular ioctl

  @see    ds_sys_ioctl 
*/
int16 ps_sys_ioctl
(
  ps_sys_tech_enum_type           tech_type,
  ps_sys_ioctl_enum_type          ioctl_name,
  void                          * arg_val_ptr,
  int16                         * ps_errno
);


/**
  @brief  This is PS Sys level API for ioctl. This function performs the
          operation for requested ioctl for particular technology and 
          subscription

  @see    ds_sys_ioctl_ex 
*/
int16 ps_sys_ioctl_ex
(
  ps_sys_tech_enum_type           tech_type,
  ps_sys_ioctl_enum_type          ioctl_name,
  ps_sys_subscription_enum_type   subscription_id,
  void                          * arg_val_ptr,
  int16                         * ps_errno
);


/**
  @brief  This function allows client to register callback for of particular
          technology 

  @param[in] tech_type            States Technology type
  @param[in] tech_cback_f_ptr     Callback function 
  @param[out] ps_errno            DS_EFAULT if parameter is invalid
                                  DS_EINVAL if operation is invalid
                                  DS_NOMEMORY if memory allocation fails

  @return  DSS_SUCCESS            On Success.
  @return  DSS_ERROR              On failure. 
   
  @code
 
  Example
  3GPP modehandler registers for ioctl callback as follow 
   
  int16  ioctl_3gpp_cback(...)
  {
    .....
  }
 
  ps_sys_ioctl_lte_attach_pdn_params_type * ioctl_info;
 
  ret_val =  ps_sys_ioctl_register_tech_cback(PS_SYS_TECH_3GPP,
                                              &ioctl_3gpp_cback,
                                              &ps_errno);
 
  if(0 > ret_val)
  {
    LOG_MSG_ERROR_1("Callback reg failed, err %d", ps_errno);
  }
   
  @endcode 
*/
int16 ps_sys_ioctl_register_tech_cback
(
  ps_sys_tech_enum_type                 tech_type,
  ps_sys_ioctl_tech_cback_f_ptr_type    tech_cback_f_ptr,
  int16                               * ps_errno
);


/**
  @brief  This function allows client to register callback for particular
          technology and subscription to perform given Ioctl operation

  @param[in] tech_type            States Technology type
  @param[in] tech_cback_f_ptr     Callback function 
  @param[in] subscription_id      Subscription Id
  @param[out] ps_errno            DS_EFAULT if parameter is invalid
                                  DS_EINVAL if operation is invalid
                                  DS_NOMEMORY if memory allocation fails

  @return  DSS_SUCCESS            On Success.
  @return  DSS_ERROR              On failure. 
   
  @code
 
  Example
  3GPP modehandler registers for ioctl callback as follow 
   
  int16  ioctl_3gpp_cback(...)
  {
    .....
  }
 
  ps_sys_ioctl_lte_attach_pdn_params_type * ioctl_info;
 
  ret_val =  ps_sys_ioctl_register_tech_cback(PS_SYS_TECH_3GPP,
                                              PS_SYS_SECONDARY_SUBS,
                                              &ioctl_3gpp_cback,
                                              &ps_errno);
 
  if(0 > ret_val)
  {
    LOG_MSG_ERROR("Callback reg failed, err %d",ps_errno,0,0);
  }
   
  @endcode 
*/
int16 ps_sys_ioctl_register_tech_cback_ex
(
  ps_sys_tech_enum_type                    tech_type,
  ps_sys_ioctl_tech_ex_cback_f_ptr_type    tech_cback_ex_f_ptr,
  ps_sys_subscription_enum_type            subscription_id,  
  int16                                  * ps_errno
);

/**
  @brief  This function allows client to register callback for 
          particular common IOCTL and subscription to perform
          given Ioctl operation. The handler will be invoked if
          client calls the IOCTL with tech type as
          PS_SYS_TECH_ALL
   
  @param[in] ioctl_name           States IOCTL name
  @param[in] tech_cback_f_ptr     Callback function 
  @param[in] subscription_id      Subscription Id
  @param[out] ps_errno            DS_EFAULT if parameter is invalid
                                  DS_EINVAL if operation is invalid
                                  DS_NOMEMORY if memory allocation fails

  @return  DSS_SUCCESS            On Success.
  @return  DSS_ERROR              On failure. 
   
  @code
 
  Example 
  Tech agnostic modehandler registers for ioctl callback as 
  follow 
   
  int16  ioctl_tech_agnostic_cback(...)
  {
    .....
  }
 

  ret_val = 
           ps_sys_ioctl_register_ioctl_cback(PS_SYS_IOCTL_PDN_THROTTLE_INFO,
           PS_SYS_SECONDARY_SUBS, &ioctl_tech_agnostic_cback,
           &ps_errno);
 
  if(0 > ret_val)
  {
    LOG_MSG_ERROR_1("Callback reg failed, err %d",ps_errno);
  }
   
  @endcode 
*/
int16 ps_sys_ioctl_register_ioctl_cback_ex
(
  ps_sys_ioctl_enum_type                   ioctl_name,
  ps_sys_ioctl_tech_ex_cback_f_ptr_type    tech_cback_ex_f_ptr,
  ps_sys_subscription_enum_type            subscription_id,  
  int16                                  * ps_errno
);

/**
  @brief  This API initializes default ioctl handlers for ioctls which
          are handled at ps sys level

  @return  None. 
*/
void ps_sys_ioctl_register_common_cbacks
(
  void
);

#ifdef __cplusplus
}
#endif

#endif /* PS_SYS_IOCTL_H */

