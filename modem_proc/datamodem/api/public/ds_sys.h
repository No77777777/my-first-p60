/**
@file ds_sys.h
@brief
This file defines function, variables and data structures common to all PS
System API module
*/

#ifndef DS_SYS_H
#define DS_SYS_H

/*===========================================================================

                          D S  _ S Y S . H

DESCRIPTION

  Data Services System module interface file. This contains variables and
  definitions used by the various modules to access interface definitions.

Copyright (c) 2011-2020 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  Copyright (c) 2011-2021 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary.


  $Header: //components/rel/data.mpss/3.5..2.1/api/public/ds_sys.h#5 $ $DateTime: 2021/06/02 00:21:58 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/06/11    sy     Created module

===========================================================================*/

#include "comdef.h"
#include "dserrno.h"
#include "ps_in.h"
#include "dsm.h"
#include "queue.h"

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================

                         EXTERNAL DATA DECLARATIONS

===========================================================================*/
/**
  RAT Mask for 3GPP
 */
#define DS_SYS_RAT_3GPP_WCDMA                0x01
#define DS_SYS_RAT_3GPP_GPRS                 0x02
#define DS_SYS_RAT_3GPP_HSDPA                0x04
#define DS_SYS_RAT_3GPP_HSUPA                0x08
#define DS_SYS_RAT_3GPP_EDGE                 0x10
#define DS_SYS_RAT_3GPP_LTE                  0x20
#define DS_SYS_RAT_3GPP_HSDPAPLUS            0x40
#define DS_SYS_RAT_3GPP_DC_HSDPAPLUS         0x80
#define DS_SYS_RAT_3GPP_64_QAM               0x100
#define DS_SYS_RAT_3GPP_TDSCDMA              0x200
#define DS_SYS_RAT_3GPP_NULL_BEARER          0x8000

/**
  RAT Mask for 3GPP2
*/
#define DS_SYS_RAT_3GPP2_1X               0x01
#define DS_SYS_RAT_3GPP2_EVDO_REV0        0x02
#define DS_SYS_RAT_3GPP2_EVDO_REVA        0x04
#define DS_SYS_RAT_3GPP2_EVDO_REVB        0x08
#define DS_SYS_RAT_3GPP2_EHRPD            0x10
#define DS_SYS_RAT_3GPP2_FMC              0x20
#define DS_SYS_RAT_3GPP2_NULL_BEARER      0x8000

/**
  SO Mask for 1x
*/
#define DS_SYS_SO_3GPP2_1X_IS95                0x01
#define DS_SYS_SO_3GPP2_1X_IS2000              0x02
#define DS_SYS_SO_3GPP2_1X_IS2000_REL_A        0x04

/**
  SO Mask for DO

  The following table gives which of the following so_masks are supported
  by which technology

  Rev0 supports @li DS_SYS_SO_3GPP2_EVDO_DPA

  RevA supports @li DS_SYS_SO_3GPP2_EVDO_DPA
                @li DS_SYS_SO_3GPP2_EVDO_MFPA
                @li DS_SYS_SO_3GPP2_EVDO_EMPA
                @li DS_SYS_SO_3GPP2_EVDO_EMPA_EHRPD

  RevB supports @li DS_SYS_SO_3GPP2_EVDO_DPA
                @li DS_SYS_SO_3GPP2_EVDO_MFPA
                @li DS_SYS_SO_3GPP2_EVDO_EMPA
                @li DS_SYS_SO_3GPP2_EVDO_EMPA_EHRPD
                @li DS_SYS_SO_3GPP2_EVDO_MMPA
                @li DS_SYS_SO_3GPP2_EVDO_MMPA_EHRPD

*/
#define DS_SYS_SO_3GPP2_EVDO_DPA           0x01
#define DS_SYS_SO_3GPP2_EVDO_MFPA          0x02
#define DS_SYS_SO_3GPP2_EVDO_EMPA          0x04
#define DS_SYS_SO_3GPP2_EVDO_EMPA_EHRPD    0x08
#define DS_SYS_SO_3GPP2_EVDO_MMPA          0x10
#define DS_SYS_SO_3GPP2_EVDO_MMPA_EHRPD    0x20

/**
    3GPP RAT mask Values 
*/
#define  DS_SYS_3GPP_RAT_GSM      0x00000001
#define  DS_SYS_3GPP_RAT_WCDMA    0x00000002
#define  DS_SYS_3GPP_RAT_LTE      0x00000004
#define  DS_SYS_3GPP_RAT_TDSCDMA  0x00000008
#define  DS_SYS_3GPP_RAT_ANY      0xFFFFFFFF

/**
   Defines the MAX length for PDN/APN name string
*/
#define DS_SYS_MAX_APN_LEN        101
#define DS_SYS_MAX_APNS           15 /**< Max number of APNs */
#define DS_SYS_LTE_ATTACH_PDN_PROFILE_LIST_MAX  56 
/**< Max number of LTE Attach PDN profile IDs */

#define DS_SYS_EHRPD_FALLBACK_APN_MAX  16
/**< Max number of EHRPD fallback APNs */

#define DS_SYS_MAX_NUM_THROUGHPUT_INFO        18
/**< max_thrpt_info = num_pdn * num_ifaces_per_pdn 
     Currently we have 8PDNs + 1 emergency PDN,(total 9PDNs)
     and max ifaces per PDN = 2 */

#define DS_SYS_MAX_BEARER         8

#define DS_SYS_MAX_AVAIL_SYS      15

#define DS_SYS_MAC_ADDR_LEN       6

#define DS_SYS_MAX_SSID_LEN       33

#define DS_SYS_MAX_WQE_PROFILES   39

/* Max length of App specific information in Operator reserved PCO */
#define DS_SYS_MAX_OP_RES_PCO_APP_SPECIFIC_LEN 255

#define DS_SYS_MAX_NUM_PCO  10

#define DS_SYS_MAX_LTE_NUM_SCELL  7

#define DS_SYS_MAX_HESSID_LEN     6

/* PS Data Off Service type change reason */
#define DS_SYS_PS_DATA_AVAIL_CHG_REASON_ROAM_STATUS                0x01
#define DS_SYS_PS_DATA_AVAIL_CHG_REASON_MOBILE_DATA_STATUS         0x02
#define DS_SYS_PS_DATA_AVAIL_CHG_REASON_ROAMING_MOBILE_DATA_STATUS 0x04
#define DS_SYS_PS_DATA_AVAIL_CHG_REASON_SIM_REFRESH                0x08
#define DS_SYS_PS_DATA_AVAIL_CHG_REASON_OMADM                      0x10 

/**
  @brief Technology type values
*/
typedef enum
{
  DS_SYS_TECH_MIN     = 0x00,
  DS_SYS_TECH_3GPP    = DS_SYS_TECH_MIN, /**< Tech type 3gpp */
  DS_SYS_TECH_3GPP2   = 0x01,            /**< Tech type 3gpp2 */
  DS_SYS_TECH_WLAN    = 0x02,            /**< Tech type WLAN */
  DS_SYS_TECH_ALL     = 0x03,            /**< All the tech types */
  DS_SYS_TECH_MAX     = 0x04,
  DS_SYS_TECH_INVALID = 0xFF
} ds_sys_tech_enum_type;

/**
  @brief This is the type that is used to different network types
*/
typedef enum
{
  DS_SYS_NETWORK_3GPP,     /**< Network type 3gpp */
  DS_SYS_NETWORK_3GPP2,    /**< Network type 3gpp2 */
  DS_SYS_NETWORK_WLAN,     /**< Network type WLAN */
  DS_SYS_NETWORK_MAX,
} ds_sys_network_enum_type;

/**
  @brief This is the type that is used to define core network details
*/
typedef struct
{
  uint32   rat_mask;  /**< RAT Mask */
  uint32   so_mask;   /**< SO Mask */
} ds_sys_network_info_type;

/**
  @brief This is the type that is used to define system status. It is
         common to both configuration DS_SYS_CONF_SYSTEM_STATUS and event
         DS_SYS_EVENT_SYSTEM_STATUS_CHANGE

  @see   DS_SYS_CONF_SYSTEM_STATUS
  @see   DS_SYS_EVENT_SYSTEM_STATUS_CHANGE
*/
typedef struct
{
  ds_sys_network_enum_type         preferred_network; /**< pref network value*/
  ds_sys_network_info_type         network_info[DS_SYS_NETWORK_MAX];
  /**< Array of rat and SO mask for all networks */
} ds_sys_system_status_type;


/**
  @brief This is the type that is used for 
         DS_SYS_CONF_GET_WLAN_MAC_ADDR configuration and
         DS_SYS_EVENT_WLAN_MAC_ADDR_CHANGE
   
  @see DS_SYS_CONF_GET_WLAN_MAC_ADDR
  @see DS_SYS_EVENT_WLAN_MAC_ADDR_CHANGE 
*/
typedef struct
{
  uint8            ap_mac_addr[DS_SYS_MAC_ADDR_LEN]; 
  /**< WLAN Access Point MAC addr*/
} ds_sys_wlan_mac_addr_type;

/**
  @brief This is the type that is used for 
         DS_SYS_CONF_GET_WLAN_SSID configuration and
         DS_SYS_EVENT_WLAN_SSID_CHANGE
   
  @see DS_SYS_CONF_GET_WLAN_SSID
  @see DS_SYS_EVENT_WLAN_SSID_CHANGE 
*/
typedef struct
{
  char            ssid[DS_SYS_MAX_SSID_LEN]; 
  /**< WLAN Service Set Identifier*/
} ds_sys_wlan_ssid_type;

/**
  @brief This is the type that is used for 
         DS_SYS_CONF_GET_WLAN_IP_ADDR configuration and
         DS_SYS_EVENT_WLAN_IP_ADDR_CHANGE
   
  @see DS_SYS_CONF_GET_WLAN_IP_ADDR
  @see DS_SYS_EVENT_WLAN_IP_ADDR_CHANGE 
*/
typedef struct
{
  ps_ip_addr_type                  ipv4_addr;  /**< WLAN IPV4 address */
  ps_ip_addr_type                  ipv6_addr;  /**< WLAN IPV6 address */
  uint8                            ipv6_prefix_len;  /**< IPV6 addr prefix length */
} ds_sys_wlan_ip_addr_type;

/**
  @brief Data type used in ds_sys_3gpp2_page_monitor_type
  @see   ds_sys_3gpp2_page_monitor_type
*/
typedef uint8 ds_sys_3gpp2_page_monitor_period_type;

/**
  @brief Data type to be used with get operation for
         DS_SYS_CONF_3GPP2_PAGE_MONITOR_PERIOD and
         DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_CHANGE event

  @see   DS_SYS_CONF_3GPP2_PAGE_MONITOR_PERIOD
  @see   DS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_CHANGE
*/
typedef struct
{
  ds_sys_3gpp2_page_monitor_period_type    page_monitor_period;
  boolean                                  force_long_sleep;
} ds_sys_3gpp2_page_monitor_type;

typedef enum
{
  DS_SYS_LOW_LATENCY_TRAFFIC_STATE_STOP,
  DS_SYS_LOW_LATENCY_TRAFFIC_STATE_START,
  DS_SYS_LOW_LATENCY_TRAFFIC_STATE_CONTINUE,
  DS_SYS_LOW_LATENCY_TRAFFIC_STATE_MAX
} ds_sys_low_latency_traffic_state_enum_type;

/**
  @brief Low latency traffic status information. 
         Example : 
         1. If packet arrives at time T, then sleep for
         pkt_interval_time_in_ms - pkt_arrival_delta_in_ms. 
         2. Wait for next packet to arrive with in pkt_max_wait_time_in_ms.
         3. Once packet arrive. Goto step 1     
         4. If packet doesnt arrive then low latency traffic end indication 
            should have been sent         

  @see DS_SYS_EVENT_LOW_LATENCY_TRAFFIC_STATUS
*/
typedef struct
{
  /*! Traffic State */  
  ds_sys_low_latency_traffic_state_enum_type   traffic_state;
  
  /*! Filter result */  
  uint32    filter_handle;    
  /*! Expected periodicity of the packet arrivals for this low latency 
    traffic (in ms).
    Value 0xFFFFFFFF - means unknown */
  uint32                     pkt_interval_time_in_ms;
  
  /*! Expected delta time of the packet from its interval time to wake up
    Value 0xFFFFFFFF - means unknown */
  uint32                     pkt_arrival_delta_in_ms;
  /*! Expected time to wait for the packets at every period 
    for this low latency traffic (in ms).
    Value 0xFFFFFFFF - means unknown  */
  uint32                     pkt_max_wait_time_in_ms;
} ds_sys_low_latency_traffic_info_type;

/**
  @brief This is the type that is used to  define different RATs types
*/
typedef enum
{
  DS_SYS_RAT_EX_NULL_BEARER           = 0x0,
  /* 3GPP RAT Values */
  DS_SYS_RAT_EX_3GPP_WCDMA             = 1,     
  DS_SYS_RAT_EX_3GPP_GERAN             = 2,
  DS_SYS_RAT_EX_3GPP_LTE               = 3,
  DS_SYS_RAT_EX_3GPP_TDSCDMA           = 4,
  DS_SYS_RAT_EX_3GPP_WLAN              = 5,
#ifdef FEATURE_LAPP
  DS_SYS_RAT_EX_3GPP_5G                = 6,
#endif /* FEATURE_LAPP */
  DS_SYS_RAT_EX_3GPP_MAX               = 100,

  /* 3GPP2 RAT Values */
  DS_SYS_RAT_EX_3GPP2_1X               = 101,
  DS_SYS_RAT_EX_3GPP2_HRPD             = 102,
  DS_SYS_RAT_EX_3GPP2_EHRPD            = 103,
  DS_SYS_RAT_EX_3GPP2_WLAN             = 104,
  DS_SYS_RAT_EX_3GPP2_MAX              = 200, 

  /* WLAN RAT Values */
  DS_SYS_RAT_EX_WLAN                   = 201,
  DS_SYS_RAT_EX_WLAN_MAX               = 300,
 
  DS_SYS_RAT_UNKNOWN,
  DS_SYS_RAT_MAX,
} ds_sys_rat_ex_enum_type;

/* SO Mask has a size of 8 bytes */

/* SO Mask not available or not specified */
#define DS_SYS_SO_EX_UNSPECIFIED                    0x0

/* 3GPP SO Mask, it occupies the first 3 bytes. ie. bytes 0, 1 and 2 */
#define DS_SYS_SO_EX_3GPP_WCDMA                     0x01
#define DS_SYS_SO_EX_3GPP_HSDPA                     0x02
#define DS_SYS_SO_EX_3GPP_HSUPA                     0x04
#define DS_SYS_SO_EX_3GPP_HSDPAPLUS                 0x08
#define DS_SYS_SO_EX_3GPP_DC_HSDPAPLUS              0x10
#define DS_SYS_SO_EX_3GPP_64_QAM                    0x20
#define DS_SYS_SO_EX_3GPP_HSPA                      0x40
#define DS_SYS_SO_EX_3GPP_GPRS                      0x80
#define DS_SYS_SO_EX_3GPP_EDGE                      0x100

/* GSM so mask is depreciated. Can only be GPRS or EDGE */
#define DS_SYS_SO_EX_3GPP_GSM                       0x200

#define DS_SYS_SO_EX_3GPP_S2B                       0x400
#define DS_SYS_SO_EX_3GPP_LTE_LIMITED_SRVC          0x800
#define DS_SYS_SO_EX_3GPP_LTE_FDD                   0x1000
#define DS_SYS_SO_EX_3GPP_LTE_TDD                   0x2000
#define DS_SYS_SO_EX_3GPP_TDSCDMA                   0x4000
#define DS_SYS_SO_EX_3GPP_DC_HSUPA                  0x8000
#define DS_SYS_SO_EX_3GPP_LTE_CA_DL                 0x10000
#define DS_SYS_SO_EX_3GPP_LTE_CA_UL                 0x20000
#define DS_SYS_SO_EX_3GPP_S2B_LIMITED_SRVC          0x40000
#define DS_SYS_SO_EX_3GPP_FOUR_POINT_FIVE_G         0x80000
#define DS_SYS_SO_EX_3GPP_FOUR_POINT_FIVE_G_PLUS    0x100000


/* 3GPP2 SO Mask, it occupies bytes 3, 4 and 5 */
#define DS_SYS_SO_EX_3GPP2_1X_IS95                  (uint64)0x01000000ull
#define DS_SYS_SO_EX_3GPP2_1X_IS2000                (uint64)0x02000000ull
#define DS_SYS_SO_EX_3GPP2_1X_IS2000_REL_A          (uint64)0x04000000ull
#define DS_SYS_SO_EX_3GPP2_HDR_REV0_DPA             (uint64)0x08000000ull
#define DS_SYS_SO_EX_3GPP2_HDR_REVA_DPA             (uint64)0x10000000ull
#define DS_SYS_SO_EX_3GPP2_HDR_REVB_DPA             (uint64)0x20000000ull
#define DS_SYS_SO_EX_3GPP2_HDR_REVA_MPA             (uint64)0x40000000ull
#define DS_SYS_SO_EX_3GPP2_HDR_REVB_MPA             (uint64)0x80000000ull
#define DS_SYS_SO_EX_3GPP2_HDR_REVA_EMPA            (uint64)0x100000000ull
#define DS_SYS_SO_EX_3GPP2_HDR_REVB_EMPA            (uint64)0x200000000ull
#define DS_SYS_SO_EX_3GPP2_HDR_REVB_MMPA            (uint64)0x400000000ull
#define DS_SYS_SO_EX_3GPP2_EVDO_FMC                 (uint64)0x800000000ull
#define DS_SYS_SO_EX_3GPP2_1X_CS                    (uint64)0x1000000000ull

#ifdef FEATURE_LAPP
/* 5G SO Mask, it occupies bytes 5 */
#define DS_SYS_SO_EX_3GPP_5G_TDD                    (uint64)0x10000000000ull
#endif /* FEATURE_LAPP */


/* Bytes 6 and 7 of SO Mask are reserved for future use */

/**
  @brief This is the type that is used to  define different subscription types
         Unknown subs id is used when the client does not care/provide the subs id
*/
#define DS_SYS_NULL_BEARER_REASON_NONE                  0
#define DS_SYS_NULL_BEARER_REASON_CSFB                  1
#define DS_SYS_NULL_BEARER_REASON_OOS                   2
#define DS_SYS_NULL_BEARER_REASON_LIMITED_SRVC          4
#define DS_SYS_NULL_BEARER_REASON_VOICE_ON_SAME_SUB     8
#define DS_SYS_NULL_BEARER_REASON_VOICE_ON_OTHER_SUB    16
#define DS_SYS_NULL_BEARER_REASON_SRVCC                 32
#define DS_SYS_NULL_BEARER_REASON_CS_ONLY               64
#define DS_SYS_NULL_BEARER_REASON_ATTACH_PENDING       128
/**
  @brief This is the type that is used to  define different subscription types
         Unknown subs id is used when the client does not care/provide the subs id
*/
typedef enum 
{
  DS_SYS_DEFAULT_SUBS      = 0x0000, /** < Default Data Subscription */
  DS_SYS_PRIMARY_SUBS      = 0x0001, /**< Primary */
  DS_SYS_SECONDARY_SUBS    = 0x0002, /**< Secondary */
  DS_SYS_TERTIARY_SUBS     = 0x0003,  /**< Tertiary */
  DS_SYS_SUBS_MAX,
} ds_sys_subscription_enum_type;

/**
  @brief This is the tuple that defines network, RAT, SO and service status. This structure is used in DS_SYS_IOCT_3GPP_GET_PREV_RAT_INFO.
*/
typedef struct 
{
  ds_sys_network_enum_type                 technology; /**< technology type*/
  ds_sys_rat_ex_enum_type                  rat_value;  /**< rat type*/
  uint64                                   so_mask;   /**< so type*/
} ds_sys_system_status_info_type;

/**
  @brief  This is the type that defines apn to preferred network relationship. 
          For every APN, there would be tuple that describes its preferred 
          system. 
*/
typedef struct 
{
   char                                apn_name[DS_SYS_MAX_APN_LEN]; /**< PDN name*/
   uint32                              num_avail_sys; 
  /**< Number of valid entries to available system status array */
   ds_sys_system_status_info_type      avail_sys[DS_SYS_MAX_AVAIL_SYS];
  /**<  Array of available systems for the APN. The first element of the 
        array will contain the preferred system for the APN.
        @see ds_sys_system_status_info_type 
        @see ds_sys_system_status_info_type */
  uint8                          recommended_pref_system_index;
  /**< Index in avail_sys array, which contains the 
       recommended preferred system for the APN */
} ds_sys_apn_pref_sys_info_type;

/**
  @brief This is the type that is used to define system status. It is
         common to both configuration DS_SYS_CONF_SYSTEM_STATUS_EX and event
         DS_SYS_EVENT_SYSTEM_STATUS_EX
*/
typedef struct 
{
  uint32   num_avail_sys; 
  /**< Number of valid entries to available system status array */

  ds_sys_system_status_info_type  avail_sys[DS_SYS_MAX_AVAIL_SYS]; 
  /**< Array of current system status, the first element in the array will 
       contain the preferred system */

  uint32   num_avail_apns;
  /**< Number of apn entries to pdn_info */

  ds_sys_apn_pref_sys_info_type apn_pref_sys_info[DS_SYS_MAX_APNS];    
  /**< Array of all apns available */


  boolean   is_default_data_subscription;
  /**< Flag indicates if system status is being reported on the 
       current DDS(Default data subscription) */
  uint8                          recommended_pref_system_index;
  /**< Index in avail_sys array for the APN, which contains the 
       recommended preferred system for the APN */
       
  uint64              null_bearer_reason;   
  /**< Null Bearer Reason for any RAT in null bearer state*/    

  boolean             only_null_bearer_reason_changed;   
  /**< Indicates that only null bearer reason field changed
       between two successive system status.
  */       
} ds_sys_system_status_ex_type;


/**
  @brief Enum type describing APN Preferred system.
  @see   ds_sys_conf_apn_pref_sys_type
*/
typedef enum
{
  DS_SYS_APN_PREF_SYS_WWAN  = 0,
  DS_SYS_APN_PREF_SYS_WLAN  = 1,
  DS_SYS_APN_PREF_SYS_IWLAN = 2
} ds_sys_apn_pref_sys_enum_type;

/**
  @brief Data type to be used with DS_SYS_CONF_APN_PREF_SYS and 
         DS_SYS_EVENT_APN_PREF_SYS_CHANGE. It describes preferred 
         system for apn.         
*/
typedef struct
{
  unsigned char                  apn[DS_SYS_MAX_APN_LEN];
  ds_sys_apn_pref_sys_enum_type  pref_sys;
} ds_sys_apn_pref_sys_type;

/**
  @brief Data type to be used with attach pdn list. 
   
  @see   DS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST 
  @see   DS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST 
  @see   DS_SYS_EVENT_3GPP_LTE_ATTACH_PDN_LIST_CHANGE  
*/
typedef struct
{
  uint8  attach_pdn_profile_list_num;   
  /**< Num of attach PDN in the array */
  uint16 attach_pdn_profile_list[DS_SYS_LTE_ATTACH_PDN_PROFILE_LIST_MAX]; 
    /**< Array of attach PDN profile IDs*/
} ds_sys_3gpp_lte_attach_pdn_list_type;

/**
  @brief Data type to be used to get pending and active attach pdn list. 
  
  @see   DS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST_EX 
*/
typedef struct
{
  ds_sys_3gpp_lte_attach_pdn_list_type active_attach_pdn_list;
  ds_sys_3gpp_lte_attach_pdn_list_type pending_attach_pdn_list;
} ds_sys_3gpp_get_lte_attach_pdn_list_ex_type;

/**
  @brief Data type to be used to bringup KAMGR PDN. 
  
  @see   DS_SYS_IOCTL_3GPP_BRING_UP_ALWAYS_ON_PDN 
*/
typedef struct
{
  uint8  profile_index;
  /**< 3GPP profile number to be used for bringup.
       A value of 0 is to use default profile */

  uint32 applicable_rat_mask;
  /**< 3GPP Rat mask to be used while bringup.
       Default value is 0x00000004(LTE) */

} ds_sys_3gpp_bring_up_always_on_pdn_type;

/**
  @brief Data type to be used for setting throttling timer
         which starts when a pdn attach request gets rejected
         due to pdn being in throlled state. 
  
  @see   DS_SYS_IOCTL_3GPP_SET_THROTTLED_PDN_REJECT_TIMER
  @see   DS_SYS_IOCTL_3GPP_GET_THROTTLED_PDN_REJECT_TIMER
*/
typedef struct
{
  uint32 throttled_pdn_attach_rejected_start_timer;
  /**< Indicates how much time the control point must wait. */

} ds_sys_3gpp_throttled_pdn_attach_reject_timer_type;

//Ip addr type enum
typedef enum
{
  DS_SYS_IP_TYPE_V4 = 0x00,
  DS_SYS_IP_TYPE_V6 = 0x01
} ds_sys_ip_addr_enum_type;

// Throughput quality enum
typedef enum {
  DS_SYS_HIGHER_THROUGHPUT_UNKNOWN,
  /**< Throughput quality is unknown \n */
  DS_SYS_HIGHER_THROUGHPUT_NOT_POSSIBLE,
  /**< Best throughput possible \n */
  DS_SYS_HIGHER_THROUGHPUT_POSSIBLE
  /**< Better throughput than current throughput is possible \n */
} ds_sys_throughput_quality_enum_type;

/**
  @brief Data type to be used to represent per bearer
         throughput info
*/
typedef struct
{
  uint32 bearer_rlp_mac_id;
  /**< Bearer ID representing the Bearer
       for which the throughput is being 
       reported in case of 3gpp tech
  */

  uint32 bearer_uplink_actual_rate;
  /**<  Uplink actual rate in bits per second
        corresponding to the bearer id 
  */

  uint32 bearer_uplink_queue_size;
  /**< Number of bytes pending in uplink queue
       corresponding to the bearer id 
  */
  
  boolean is_default_bearer;
  /**< Boolean to determine if the bearer
       is a default one 
  */ 
} ds_sys_bearer_throughput_info_type;

typedef struct
{
  uint8 num_bearers; 
  /**< Num of bearer trpt info in the array */

  ds_sys_bearer_throughput_info_type bearer_trpt_info[DS_SYS_MAX_BEARER];
  /**< Array of per bearer throughput info*/
} ds_sys_bearer_throughput_info_list_type;

/**
  @brief Data type to be used with throughput info timer 
   
  @see   DS_SYS_CONF_THROUGHPUT_INFO
  @see   DS_SYS_EVENT_THROUGHPUT_INFO  
*/
typedef struct
{
  char apn_string[DS_SYS_MAX_APN_LEN];
  /**< String representing the APN.
       Maximum length is 100 bytes.
  */

  ds_sys_ip_addr_enum_type ip_type;
  /**< Values: \n
       - 4 -- IPv4 \n
       - 6 -- IPv6 
  */

  ds_sys_tech_enum_type  tech_type;
  /**< Technology type */

  ds_sys_subscription_enum_type subscription;
  /**< Subscription that APN is bound to.
  */

  uint32 uplink_actual_rate;
  /**<  Uplink actual rate(kbps)
  */

  uint32 uplink_allowed_rate;
  /**<  Uplink allowed rate(kbps)
  */

  uint32 uplink_queue_size;
  /**< Number of bytes pending in uplink queue
  */

  ds_sys_throughput_quality_enum_type throughput_quality;
  /**< Indicates whether UE can have better throughput 
       rate than the current throughput reporting
  */  

  void *iface_ptr;
  /**< ptr to physical interface on which to 
       operate on.(Um interface representing the APN)
  */  

  ds_sys_bearer_throughput_info_list_type bearer_throughput_info_list;
  /**< list of per bearer throughput information
  */

} ds_sys_throughput_info_type;

typedef struct
{
   uint8 num_trpt_info; 
   /**< Num of trpt info in the list */

   ds_sys_throughput_info_type trpt_info[1];
   /**< Pointer to throughput info list. Can have more than one info in 
        the list */
} ds_sys_throughput_info_list_type;

/**
  @brief Data type to be used with DS_SYS_CONF_3GPP_EMERGENCY_PDN_STATUS and 
         DS_SYS_EVENT_3GPP_EMERGENCY_PDN_STATUS. 
         TRUE : UE is attached to emergency PDN only.
         FALSE: UE is attached to other PDN(could be emergency pdn + 
                non-emergency pdn or non-emergency pdn)       
*/
typedef struct
{
  boolean emergency_pdn_only;
} ds_sys_is_pdn_only_emergency_type;

/**
  @brief Data type to store the apn name 
   
*/
typedef struct
{
  unsigned char     apn_name[DS_SYS_MAX_APN_LEN];
} ds_sys_apn_name_type;

/**
  @brief Data type to be used with roaming disallowed info
  is_roaming_disallowed : whether roaming is allowed or disallowed
                          on the apns
  num_apn_names         : No of apn names listed
  apn_list              : List of apn names 
   
  @see   DS_SYS_CONF_3GPP_ROAMING_DISALLOWED_INFO
  @see   DS_SYS_EVENT_3GPP_ROAMING_DISALLOWED_INFO  
*/
typedef struct
{
  boolean                 is_roaming_disallowed;
  uint8                   num_apn_names;
  ds_sys_apn_name_type    apn_list[DS_SYS_MAX_APNS];
} ds_sys_roaming_disallowed_info_type;

/**
  @brief Data type to be used with network aware status
   
  @see   DS_SYS_CONF_3GPP2_QOS_NETWORK_STATUS
  @see   DS_SYS_EVENT_3GPP2_QOS_NETWORK_STATUS_CHANGE  
*/
typedef boolean ds_sys_3gpp2_network_qos_aware_status;

/**
  @brief Data type to be used with page monitor period
   
  @see   DS_SYS_IOCTL_3GPP2_GET_PAGE_MONITOR_PERIOD
  @see   DS_SYS_EVENT_3GPP2_PAGE_MONITOR_PERIOD_CHANGE
 */
typedef struct
{
  ds_sys_3gpp2_page_monitor_period_type       slotted_mode_option;
  boolean     long_sleep_enabled;
} ds_sys_3gpp2_page_monitor_period_info_type;

/**
  @brief Data type to be used with downlink throughput info 
         timer
   
  @see   DS_SYS_CONF_DL_THROUGHPUT_INFO_TIMER
  @see   DS_SYS_EVENT_UPDATED_DL_THROUGHPUT_TIMER
*/
typedef uint32 ds_sys_report_dl_timer_type;

/**
  @brief Data type to be used with downlink throughput info
         status change
   
  @see   DS_SYS_CONF_DL_THROUGHPUT_INTERVAL_STATUS_CHANGE
  @see   DS_SYS_EVENT_UPDATED_DL_THROUGHPUT_TIMER
*/

typedef enum
{
  DS_SYS_THRPT_REPORT_ENABLED,
  /**< Throughput reporting enabled \n */
   DS_SYS_THRPT_REPORT_DISABLED_NO_DATA_CALL,
  /**< No data call \n */
   DS_SYS_THRPT_REPORT_DISABLED_ALL_CALLS_DORMANT,
  /**< All calls dormant \n */
  DS_SYS_THRPT_REPORT_DISABLED_UNSUPPORTED_RAT,
  /**< Unsupported RAT \n */
  DS_SYS_THRPT_REPORT_DISABLED_BY_CLIENT
  /**< Report disabled by client */
} ds_sys_thrpt_status_reason_enum_type;

typedef struct
{
  ds_sys_thrpt_status_reason_enum_type   thrpt_status;
  uint32                            actual_interval;
} ds_sys_thrpt_status_type;

/**
  @brief Data type to be used with downlink throughput info
   
  @see   DS_SYS_CONF_DL_THROUGHPUT_INFO
  @see   DS_SYS_EVENT_DL_THROUGHPUT_INFO
*/
typedef struct
{
  uint32   downlink_allowed_rate;
  uint8    confidence_level;
  boolean  is_suspended;
} ds_sys_dl_throughput_info_type;

/**
  @brief Data type to be used with thermal level mitigation info
   
  @see   DS_SYS_CONF_THERMAL_MITIGATION_INFO
  @see   DS_SYS_EVENT_THERMAL_MITIGATION_INFO
 */
typedef enum
{
  DS_SYS_THERMAL_MITIGATION_ALL_CALLS_ALLOWED = 0,
  DS_SYS_THERMAL_MITIGATION_IMS_CALLS_ONLY    = 1,
  DS_SYS_THERMAL_MITIGATION_NO_CALLS_ALLOWED  = 2
} ds_sys_thermal_mitigation_action_enum_type;

/**
  @brief Data type to be used with data service
   
  @see   DS_SYS_EVENT_DATA_SERVICE_SETTING
 */
typedef boolean ds_sys_data_services_setting_type;

/**
  @brief Data type to be used with data service roaming

  @see   DS_SYS_EVENT_DATA_SERVICE_ROAMING_SETTING
 */
typedef boolean ds_sys_data_services_roaming_setting_type;

/**
  @brief This is the type that is used to different apn types. 
         DS_SYS_APN_TYPE_UNSPECIFIED is used if APN type has not configured
*/
#define DS_SYS_APN_TYPE_UNSPECIFIED 0xFF  /**< APN type unspecified */

typedef enum
{
  DS_SYS_APN_TYPE_INTERNET     = 0,  /**< Internet APN */
  DS_SYS_APN_TYPE_IMS          = 1,   /**< IMS APN */
  DS_SYS_APN_TYPE_MMS          = 2,  /**< APN type for Multimedia Messaging Service */
  DS_SYS_APN_TYPE_DUN          = 3,  /**< APN type for Dial Up Network */
  DS_SYS_APN_TYPE_SUPL         = 4,  /**< APN type for Secure User Plane Location */
  DS_SYS_APN_TYPE_HIPRI        = 5,  /**< APN type for High Priority Mobile Data */
  DS_SYS_APN_TYPE_FOTA         = 6,  /**< APN type for over the air administration */
  DS_SYS_APN_TYPE_CBS          = 7,  /**< APN type for Carrier Branded Services */
  DS_SYS_APN_TYPE_IA           = 8,  /**< APN type for Initial Attach */
  DS_SYS_APN_TYPE_EMERGENCY    = 9,  /**< APN type for emergency */
  DS_SYS_APN_TYPE_MAX
} ds_sys_apn_enum_type;

/**
  @brief Data type to store the apn info 
  @see   DS_SYS_EVENT_APN_INFO
   
*/
typedef struct
{
  boolean   is_configured;
  /**< TRUE- if explicitly configured by apps; 
       FALSE - if default setting is used */
  ds_sys_apn_enum_type apn_type;
  char        apn_name[DS_SYS_MAX_APN_LEN];
} ds_sys_apn_params_type;

typedef struct
{
  ds_sys_apn_params_type   curr_apn_info[DS_SYS_APN_TYPE_MAX];
  /**< Current APN list indexed by apn_type*/
  ds_sys_apn_params_type   prev_apn_info[DS_SYS_APN_TYPE_MAX];
  /**< Previous APN list indexed by apn_type*/
} ds_sys_apn_info_type;
typedef enum
{
  DS_SYS_RAT_PREFERENCE_INACTIVE           = 0,/**< rat preference not set */
  DS_SYS_RAT_PREFERENCE_CELLULAR_ONLY      = 1,/**< cellular only */
  DS_SYS_RAT_PREFERENCE_WIFI_ONLY          = 2, /**< wifi only */
  DS_SYS_RAT_PREFERENCE_CELLULAR_PREFERRED  = 3, /**< cellular preferred */
  DS_SYS_RAT_PREFERENCE_WIFI_PREFERRED      = 4, /**< Wifi preferred*/
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  DS_SYS_RAT_PREFERENCE_IMS_PREFERRED       = 5, /**< IMS preferred*/
#endif
  DS_SYS_RAT_PREFERENCE_MAX
} ds_sys_rat_preference_enum_type;

/**
  @brief Data type to be used with uplink throughput info 
         timer and uplink queue size request
   
  @see   DS_SYS_CONF_UL_THROUGHPUT_CONFIG
  @see   DS_SYS_EVENT_UPDATED_UL_THROUGHPUT_CONFIG
*/
typedef struct
{
  uint32  report_ul_timer;
  boolean report_ul_queue_size;
}ds_sys_report_ul_throughput_config_type;
/**
  @brief Data type to be used with uplink throughput info
   
  @see   DS_SYS_CONF_UL_THROUGHPUT_INFO
  @see   DS_SYS_EVENT_UL_THROUGHPUT_INFO
*/
typedef struct
{
  uint32   uplink_allowed_rate;
  uint8    confidence_level;
  uint32   uplink_queue_size;
} ds_sys_ul_throughput_info_type;


/**
  @brief Data type to be used with 
         DS_SYS_IOCTL_SET_WQE_PROFILE_TYPE and event payload for
         the DS_SYS_EVENT_WQE_PROFILES_MET

   */ 
#define DS_SYS_WQE_PROFILE_INACTIVE            0x0000000000000000
  /**< WiFi Quality Estimation profile inactive */
#define DS_SYS_WQE_PROFILE_INTERNET            0x0000000000000001
  /**< WiFi Quality Estimation profile internet */
#define DS_SYS_WQE_PROFILE_AUDIO               0x0000000000000002
  /**< WiFi Quality Estimation profile audio */
#define DS_SYS_WQE_PROFILE_VIDEO               0x0000000000000004
  /**< WiFi Quality Estimation profile video */
#define DS_SYS_WQE_PROFILE_IMS_DYNAMIC_TYPE_1  0x0000000000000008
/**< Wi-Fi Quality Estimation profile for IMS Dynamic Type 1 */  
#define DS_SYS_WQE_PROFILE_IMS_DYNAMIC_TYPE_2  0x0000000000000010
/**< Wi-Fi Quality Estimation profile for IMS Dynamic Type 2 */  
#define DS_SYS_WQE_PROFILE_IMS_DYNAMIC_TYPE_3  0x0000000000000020
/**< Wi-Fi Quality Estimation profile for IMS Dynamic Type 3 */  
#define DS_SYS_WQE_PROFILE_IMS_DYNAMIC_TYPE_4  0x0000000000000040
/**< Wi-Fi Quality Estimation profile for IMS Dynamic Type 4 */  
#define DS_SYS_WQE_PROFILE_IMS_DYNAMIC_TYPE_5  0x0000000000000080
/**< Wi-Fi Quality Estimation profile for IMS Dynamic Type 5 */  
#define DS_SYS_WQE_PROFILE_IMS_DYNAMIC_TYPE_6  0x0000000000000100
/**< Wi-Fi Quality Estimation profile for IMS Dynamic Type 6 */  
#define DS_SYS_WQE_PROFILE_IMS_DYNAMIC_TYPE_7  0x0000000000000200
/**< Wi-Fi Quality Estimation profile for IMS Dynamic Type 7 */  
#define DS_SYS_WQE_PROFILE_IMS_TYPE_1          0x0000000100000000
  /**< WiFi Quality Estimation profile IMS 1 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_2          0x0000000200000000
  /**< WiFi Quality Estimation profile IMS 2 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_3          0x0000000400000000
  /**< WiFi Quality Estimation profile IMS 3 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_4          0x0000000800000000
  /**< WiFi Quality Estimation profile IMS 4 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_5          0x0000001000000000
  /**< WiFi Quality Estimation profile IMS 5 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_6          0x0000002000000000
  /**< WiFi Quality Estimation profile IMS 6 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_7          0x0000004000000000
  /**< WiFi Quality Estimation profile IMS 7 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_8          0x0000008000000000
  /**< WiFi Quality Estimation profile IMS 8 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_9          0x0000010000000000
  /**< WiFi Quality Estimation profile IMS 9 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_10         0x0000020000000000
  /**< WiFi Quality Estimation profile IMS 10 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_11         0x0000040000000000
  /**< WiFi Quality Estimation profile IMS 11 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_12         0x0000080000000000
  /**< WiFi Quality Estimation profile IMS 12 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_13         0x0000100000000000
  /**< WiFi Quality Estimation profile IMS 13 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_14         0x0000200000000000
  /**< WiFi Quality Estimation profile IMS 14 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_15         0x0000400000000000
  /**< WiFi Quality Estimation profile IMS 15 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_16         0x0000800000000000
  /**< WiFi Quality Estimation profile IMS 16 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_17         0x0001000000000000
  /**< WiFi Quality Estimation profile IMS 17 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_18         0x0002000000000000
  /**< WiFi Quality Estimation profile IMS 18 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_19         0x0004000000000000
  /**< WiFi Quality Estimation profile IMS 19 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_20         0x0008000000000000
  /**< WiFi Quality Estimation profile IMS 20 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_21         0x0010000000000000
  /**< WiFi Quality Estimation profile IMS 21 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_22         0x0020000000000000
  /**< WiFi Quality Estimation profile IMS 22 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_23         0x0040000000000000
  /**< WiFi Quality Estimation profile IMS 23 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_24         0x0080000000000000
  /**< WiFi Quality Estimation profile IMS 24 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_25         0x0100000000000000
  /**< WiFi Quality Estimation profile IMS 25 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_26         0x0200000000000000
  /**< WiFi Quality Estimation profile IMS 26 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_27         0x0400000000000000
  /**< WiFi Quality Estimation profile IMS 27 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_28         0x0800000000000000
  /**< WiFi Quality Estimation profile IMS 28 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_29         0x1000000000000000
  /**< WiFi Quality Estimation profile IMS 29 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_30         0x2000000000000000
  /**< WiFi Quality Estimation profile IMS 30 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_31         0x4000000000000000
  /**< WiFi Quality Estimation profile IMS 31 */
#define DS_SYS_WQE_PROFILE_IMS_TYPE_32         0x8000000000000000
  /**< WiFi Quality Estimation profile IMS 32 */

typedef enum {
  DS_SYS_WQE_CLIENT_INVALID = 0,
  DS_SYS_WQE_CLIENT_1       = 1, 
  DS_SYS_WQE_CLIENT_2       = 2
}ds_sys_wqe_client_enum_type;


typedef struct
{
  uint64                         wqe_profile_type;
  ds_sys_wqe_client_enum_type    client_id;
  uint32                         tx_id;
}ds_sys_wqe_profile_type;

/**
  @brief Data type to be used with wifi switch settings
   
  @see  DS_SYS_CONF_DATA_SERVICE_WIFI_SETTING 
  @see   DS_SYS_EVENT_DATA_SERVICE_WIFI_SETTING
 */
typedef boolean ds_sys_data_services_wifi_setting_type;

/**
  @brief Data type to be used with 
         DS_SYS_IOCTL_SET_WQE_PROFILE_QUALITY_MEAS_INFO
*/

/* Wifi RSSI Threshold Type */
typedef struct 
{
  boolean is_rssi_high_valid;
  /**< Valid RSSI High threshold */
  int16 rssi_high;
  /**< RSSI high threshold value for the channel*/
  boolean is_rssi_low_valid;
  /**< Valid RSSI low threshold */
  int16 rssi_low;
  /**< RSSI low  threshold value for the channel*/
} ds_sys_wifi_ident_rssi_threshold_type;


typedef struct
{
   uint64 wqe_profile;
   /**< Particular WQE profile for which threshold has to be configured. 
   */

   ds_sys_wifi_ident_rssi_threshold_type rssi_threshold;
   /**< 2.4 GHz band RSSI threshold for the wqe profile. */

   ds_sys_wifi_ident_rssi_threshold_type rssi_threshold_5_ghz;
   /**< 5 GHz band RSSI threshold for the wqe profile. */

   ds_sys_wifi_ident_rssi_threshold_type rssi_threshold_6_ghz;
   /**< 6 GHz band RSSI threshold for the wqe profile. */
} ds_sys_wqe_profile_quality_meas_info_type;

typedef struct
{
  int num_wqe_profiles;
  /**< Particular WQE Profiles for which information is being provided */
  ds_sys_wqe_profile_quality_meas_info_type meas_info[DS_SYS_MAX_WQE_PROFILES];
  /**< Info per WQE profile. */
  ds_sys_wqe_client_enum_type               client_id;
} ds_sys_set_wqe_profile_quality_meas_info_list_type;

/**
  @brief This is the enum that is used to specify roaming type.
*/
typedef enum{
  DS_SYS_ROAMING_UNSPECIFIED   = 0,
  DS_SYS_ROAMING_DOMESTIC      = 1,
  DS_SYS_ROAMING_INTERNATIONAL = 2
}ds_sys_roaming_type_enum_type;

/**
  @brief Gives whether the UE is roaming or in the home network.
  roaming_status = TRUE  : UE is roaming
  roaming_status = FALSE : UE is in the home network   
  
  technology : Technology for which roaming status is set/unset
               with roaming_type: Type of roaming
  @see DS_SYS_EVENT_ROAMING_STATUS_CHANGE
  @see DS_SYS_CONF_ROAMING_STATUS_INFO
*/
typedef struct
{
  boolean                  roaming_status;
  ds_sys_network_enum_type technology;
  ds_sys_roaming_type_enum_type roaming_type;
} ds_sys_roaming_status_info_type;

typedef struct
{
  boolean    is_ipv4_throttled;             /**< ipv4 is throttled */
  boolean    is_ipv6_throttled;             /**< ipv6 is throttled */
  uint32     remaining_ipv4_throttled_time; /**< ipv4 remaining time in 
                                                 miliseconds */ 
  uint32     remaining_ipv6_throttled_time; /**< ipv6 remaining time in 
                                                 miliseconds */
  char       apn_string[DS_SYS_MAX_APN_LEN]; /**< PDN name */
  uint8      apn_string_len;                 /**< PDN name length */
} ds_sys_pdn_ctxt_throttle_info_type;

typedef struct
{
  ds_sys_pdn_ctxt_throttle_info_type pdn_throttle_info[DS_SYS_MAX_APNS];
  /**< For each internet, we have the values ie max apns */

  uint8 num_apn; /**< Refers to the no. of valid array elements in 
                      pdn_throttle_info[] */
  boolean  is_epc_ho_throttle; /**< This flag indicates whether the 
                                    throttle cmd is invoked by EPC  
                                    or mode handler */
} ds_sys_pdn_throttle_info_type;

/**
 *@brief Data type to be used with 
 *       DS_SYS_EVENT_WQE_PROFILES_STATUS . It describes
 *       status of all the wqe profiles for which
 *       client has registered.
 *       Eg: Status =
 *       DS_SYS_WLAN_WQE_PROFILE_STATUS_QUALITY_NOT_MET
 *       and  status_code =
 *       DS_SYS_WQE_PROFILE_ERROR_CQ_FAIL_INCONCLUSIVE
 *       for a WQE profile X means threshold is in middle
 *       state 
*/

typedef enum
{
  DS_SYS_WLAN_WQE_PROFILE_STATUS_QUALITY_MET = 0x00,
  /**< Quality Met \n*/
  DS_SYS_WLAN_WQE_PROFILE_STATUS_QUALITY_NOT_MET = 0x01,
  /**< Quality Not Met \n*/
  DS_SYS_WLAN_WQE_PROFILE_STATUS_QUALITY_UNKNOWN = 0x03,
  /**< Quality is unknown \n 
       0x02 is reserved */
  DS_SYS_WLAN_WQE_PROFILE_STATUS_QUALITY_INVALID = 0xFF
  /**< Invalid/No status \n */
} ds_sys_wlan_wqe_profile_status_enum_type;


typedef enum
{ 
  DS_SYS_WQE_PROFILE_ERROR_NONE,
  /**< No error \n*/
  DS_SYS_WQE_PROFILE_ERROR_CQ_FAIL_RSSI,
  /**< Rssi threshold not met \n*/
  DS_SYS_WQE_PROFILE_ERROR_CQ_FAIL_MAC,
  /**< MAC Metrics not met \n*/
  DS_SYS_WQE_PROFILE_ERROR_CQ_FAIL_INCONCLUSIVE,
  /**< Could not conclude wlan status \n*/
  DS_SYS_WQE_PROFILE_ERROR_BQ_FAIL,
  /**<  Backhaul quality is degraded. \n*/
  DS_SYS_WQE_PROFILE_ERROR_INTERNET_CONNECTIVITY_FAIL
  /**< Internet Connectivity failure \n*/
} ds_sys_wqe_profile_cause_code_enum_type;

typedef enum
{
  DS_SYS_WQE_WIFI_BAND_2_4_GHZ,
  /**< 2.4 GHz \n*/
  DS_SYS_WQE_WIFI_BAND_5_GHZ,
  /**< 5 GHz \n*/
  DS_SYS_WQE_WIFI_BAND_6_GHZ,
  /**< 6 GHz \n*/
} ds_sys_wqe_wifi_band_enum_type;

typedef struct
{
  uint64 wqe_profile;
  /**< Particular WQE profile for which threshold 
    has to be configured. */

  ds_sys_wlan_wqe_profile_status_enum_type wqe_status;
  /**< Enum with wqe status stating the quality*/

  ds_sys_wqe_profile_cause_code_enum_type cause_code;
  /**< Cause code for the wqe status*/

  uint64                                  measurement_id;

  ds_sys_wqe_wifi_band_enum_type          wifi_band;
  /**< WiFi band used for threshold */
} ds_sys_wlan_set_wqe_profile_status_type;

typedef struct
{
  uint8 num_profiles;
  /**< Specifies the number of profiles */

  ds_sys_wlan_set_wqe_profile_status_type profile_status[DS_SYS_MAX_WQE_PROFILES];
  /**< Profile status array*/

  ds_sys_wqe_client_enum_type             client_id;
  uint32                                  tx_id;
} ds_sys_wqe_profiles_status_type;

typedef enum
{
  DS_SYS_OP_RESERVED_PCO_ID1 = 0xFF00,
  DS_SYS_OP_RESERVED_PCO_ID2 = 0xFF01,
  DS_SYS_OP_RESERVED_PCO_ID3 = 0xFF02,
  DS_SYS_OP_RESERVED_PCO_ID4 = 0xFF03,
  DS_SYS_OP_RESERVED_PCO_ID5 = 0xFF04,
  DS_SYS_OP_RESERVED_PCO_ID6 = 0xFF05,
  DS_SYS_OP_RESERVED_PCO_ID7 = 0xFF06,
  DS_SYS_OP_RESERVED_PCO_ID8 = 0xFF07,
  DS_SYS_OP_RESERVED_PCO_ID9 = 0xFF08,
  DS_SYS_OP_RESERVED_PCO_ID10 = 0xFF09
} ds_sys_op_reserved_pco_id_enum_type;

typedef struct
{
  uint16  mcc;
  uint16  mnc;
  boolean mnc_includes_pcs_digit;
  uint8  app_specific_info_len;
  uint8  app_specific_info[DS_SYS_MAX_OP_RES_PCO_APP_SPECIFIC_LEN];
  uint16 container_id;
  ds_sys_op_reserved_pco_id_enum_type   pco_id;
} ds_sys_op_reserved_pco_type;

typedef struct
{
  int                           num_pcos;
  ds_sys_op_reserved_pco_type   pco_info[DS_SYS_MAX_NUM_PCO];
} ds_sys_op_reserved_pco_list_type;


/**
  @brief Data struture associated with DS_SYS_IOCTL_GET_APN_PCO_INFO and 
         DS_SYS_EVENT_APN_PCO_INFO_CHANGE. 
  @see  DS_SYS_IOCTL_GET_APN_PCO_INFO
  @see  DS_SYS_EVENT_APN_PCO_INFO_CHANGE
*/
typedef struct
{
  char                                apn_name[DS_SYS_MAX_APN_LEN];
  ds_sys_op_reserved_pco_list_type    apn_pco_info;
} ds_sys_pco_info_per_apn_type;

typedef struct
{
  uint8                               msisdn_length;
  uint8                               msisdn[255];
} ds_sys_msisdn_info_type;

/**
  @brief Data struture associated with DS_SYS_IOCTL_GET_APN_MSISDN_INFO and 
         DS_SYS_EVENT_APN_MSISDN_CHANGE. 
  @see  DS_SYS_IOCTL_GET_APN_MSISDN_INFO
  @see  DS_SYS_EVENT_APN_MSISDN_CHANGE
*/
typedef struct
{
  char                                apn_name[DS_SYS_MAX_APN_LEN];
  ds_sys_msisdn_info_type             msisdn_info;
} ds_sys_msisdn_info_per_apn_type;

typedef enum
{ 
  /*! 1.4MHz bandwidth */
  DS_SYS_LTE_BW_6 = 6,
  /*! 3MHz bandwidth */
  DS_SYS_LTE_BW_15 = 15,
  /*! 5MHz bandwidth */
  DS_SYS_LTE_BW_25 = 25,
  /*! 10MHz bandwidth */
  DS_SYS_LTE_BW_50 = 50,
  /*! 15MHz bandwidth */
  DS_SYS_LTE_BW_75 = 75,
  /*! 20MHz bandwidth */
  DS_SYS_LTE_BW_100 = 100
} ds_sys_lte_cell_bandwidth_enum_type;


typedef enum
{
  DS_SYS_LTE_SCELL_STATE_DECONFIGURED     = 0,
  /** Cell dosent have context and is not configured yet*/
  DS_SYS_LTE_SCELL_CONFIGURED_DEACTIVATED = 1,
  /** Cell is in passive state. It wouldnt perform decoding */
  DS_SYS_LTE_SCELL_CONFIGURED_ACTIVATED   = 2
  /** Cell is in active state */
} ds_sys_lte_scell_state_enum_type;


typedef struct
{
  uint8                                 serving_cell_id;
  /** Serving Cell id assigned to cell*/
  uint16                                pci;
  /** Physical cell ID  */
  uint32                                freq;
  /** Cell's frequency Range */
  ds_sys_lte_cell_bandwidth_enum_type   dl_bandwidth;
  /** Cell's Downlink Bandwidth  */
  uint32                                band_info;
  /** Cell's band  */
} ds_sys_lte_cell_type;


typedef struct
{
  ds_sys_lte_cell_type                  cell_info;
  /** Cell's band  */
  ds_sys_lte_scell_state_enum_type      scell_state;
  /* State of secondary cell */
  boolean                               ul_enabled;
  /** Uplink is enabled for cell */
} ds_sys_lte_scell_type;


/**
  @brief Data type to be used with DS_SYS_CONF_LTE_CELL_INFO and 
         DS_SYS_EVENT_LTE_CELL_INFO_CHANGE.
*/
typedef struct
{     
  /*Primary cell info*/
  ds_sys_lte_cell_type    pcell_info;
  
  uint8                   num_scell;

  /*Secondary cell info*/
  ds_sys_lte_scell_type   scell_info[DS_SYS_MAX_LTE_NUM_SCELL];

} ds_sys_lte_cell_info_type;

/**
  @brief Data type to be used with ehrpd fallback apn list. 
   
  @see   DS_SYS_IOCTL_3GPP2_SET_EHRPD_FALLBACK_APN_LIST 
  @see   DS_SYS_IOCTL_3GPP2_GET_EHRPD_FALLBACK_APN_LIST  
*/
typedef struct
{
  uint8 fallback_apn_list_num;
  /** Num of fallback APNs */
  ds_sys_apn_name_type fallback_apn_list[DS_SYS_EHRPD_FALLBACK_APN_MAX];
  /** Array of fallback APNs */
}ds_sys_3gpp2_ehrpd_fallback_apn_list_type;

/**
  @brief This is the type that is used for 
         DS_SYS_CONF_GET_WLAN_HESSID configuration and
         DS_SYS_EVENT_WLAN_HESSID_CHANGE
   
  @see DS_SYS_CONF_GET_WLAN_HESSID
  @see DS_SYS_EVENT_WLAN_HESSID_CHANGE 
*/

typedef struct
{
  uint8             hessid[DS_SYS_MAX_HESSID_LEN]; 
  /**< Access point HESSID */
} ds_sys_wlan_hessid_type;

/**
  @brief Data type to be used with DS_SYS_IOCTL_WLAN_GET_NETWORK_MODE
*/

typedef enum {
  DS_SYS_WLAN_NETWORK_MODE_802_11_1997,/**< Legacy network Mode. */
  DS_SYS_WLAN_NETWORK_MODE_802_11a,    /**< OFDM Network mode. */
  DS_SYS_WLAN_NETWORK_MODE_802_11b,    /**< 802.11b */
  DS_SYS_WLAN_NETWORK_MODE_802_11g,    /**< 802.11g */
  DS_SYS_WLAN_NETWORK_MODE_802_11_2007,/**< 802.11.2007 */
  DS_SYS_WLAN_NETWORK_MODE_802_11n,    /**< 802.11n */
  DS_SYS_WLAN_NETWORK_MODE_802_11_2012,/**< 802.11.2012 */
  DS_SYS_WLAN_NETWORK_MODE_802_11ac,   /**< 802.11.ac */
  DS_SYS_WLAN_NETWORK_MODE_802_11ad,   /**< 802.11.ad */
  DS_SYS_WLAN_NETWORK_MODE_802_11af,   /**< 802.11.af */
  DS_SYS_WLAN_NETWORK_MODE_802_11ah,   /**< 802.11.ah */
  DS_SYS_WLAN_NETWORK_MODE_802_11ai,   /**< 802.11.ai */
  DS_SYS_WLAN_NETWORK_MODE_802_11aj,   /**< 802.11.aj */
  DS_SYS_WLAN_NETWORK_MODE_802_11aq,   /**< 802.11.aq */
  DS_SYS_WLAN_NETWORK_MODE_802_11ax,   /**< 802.11.ax */
  DS_SYS_WLAN_NETWORK_MODE_802_11ay,   /**< 802.11.ay */
  DS_SYS_WLAN_NETWORK_MODE_INVALID = 0xFF
} ds_sys_wlan_network_mode_type_enum;

#define DS_SYS_CERT_DN_MAX  256
/**
  @brief Data type to be used with the callback response from 
         DS_SYS_IOCTL_SEND_CA_CERTIFICATE. 
*/

typedef enum{
  DS_SYS_CA_CERT_RESULT_SUCCESS           = 0x00,
  /* Success */
  DS_SYS_CA_CERT_RESULT_GENERAL_FAILURE   = 0x01,
  /* General Failure */
  DS_SYS_CA_CERT_RESULT_CERT_NOT_FOUND    = 0x02,
  /* CA certificate not found on TE */
  DS_SYS_CA_CERT_RESULT_INFO_UNAVAILABLE  = 0x03
  /* CA certificate info unavailable due to TE
     not registered yet */
}ds_sys_ca_cert_result_enum_type;

/* q link for CA certificates, each node contains a CA certificate */
typedef struct{
  q_link_type              link;        /* Link for records */
  dsm_item_type          **ca_cert_ptr; /* Chain of DSM items*/
}ds_sys_ca_cert_records_q_type;

/* Callback pointer to be called for CA certificate retrieval. 
   Client has to free the memory and the queue list passed in the payload. */

typedef void (* ds_sys_ca_cert_cback_f_ptr_type)
(
  uint8                                   cert_dn[DS_SYS_CERT_DN_MAX],
  uint8                                   cert_dn_length,
  ds_sys_ca_cert_result_enum_type         result,
  q_type                                 *ca_cert_q_ptr,
  void                                   *user_data_ptr
);

typedef enum
{
  DS_SYS_APPL_SERVICE_NAME_USSI          = 0,
  DS_SYS_APPL_SERVICE_NAME_MMTEL_VOICE   = 1,
  DS_SYS_APPL_SERVICE_NAME_MMTEL_VIDEO   = 2,
  DS_SYS_APPL_SERVICE_NAME_SS_XCAP_CFG   = 3,
  DS_SYS_APPL_SERVICE_NAME_SMSOIP        = 4,
  DS_SYS_APPL_SERVICE_NAME_BIP           = 5,  
  DS_SYS_APPL_SERVICE_NAME_OMADM_OVER_PS = 6,
  DS_SYS_APPL_SERVICE_NAME_OTHER         = 7,
  DS_SYS_APPL_SERVICE_NAME_MAX
} ds_sys_appl_service_name_enum_type;

typedef enum
{
  DS_SYS_PS_DATA_AVAIL_TYPE_NONE            = 0,
  DS_SYS_PS_DATA_AVAIL_TYPE_EXEMPT          = 1,
  DS_SYS_PS_DATA_AVAIL_TYPE_NON_EXEMPT      = 2
} ds_sys_ps_data_avail_type_enum_type;

typedef enum
{
  DS_SYS_PS_DATA_AVAIL_LIST_TYPE_NONE       = 0,
  DS_SYS_PS_DATA_AVAIL_LIST_TYPE_HOME       = 1,
  DS_SYS_PS_DATA_AVAIL_LIST_TYPE_ROAM       = 2,
  DS_SYS_PS_DATA_AVAIL_LIST_TYPE_HOME_ROAM  = 3
} ds_sys_ps_data_avail_list_type_enum_type;


/**
  @brief Data type to be used 
          DS_SYS_CONF_PS_DATA_AVAIL_APPL_SERVICE_INFO. 
   
  @see   DS_SYS_CONF_PS_DATA_AVAIL_APPL_SERVICE_INFO 
  @see   DS_SYS_CONF_PS_DATA_AVAIL_APPL_SERVICE_INFO  
*/
typedef struct
{
  ds_sys_appl_service_name_enum_type      service;
  /** Service type */
  ds_sys_ps_data_avail_type_enum_type         type;
  /** Service type if its exempt or non-exempt */
} ds_sys_ps_data_avail_info_type;

typedef enum
{
  DS_SYS_PS_DATA_AVAIL_STATUS_UNKNOWN = 0,
  DS_SYS_PS_DATA_AVAIL_STATUS_ON      = 1,
  DS_SYS_PS_DATA_AVAIL_STATUS_OFF     = 2
} ds_sys_ps_data_avail_status_enum_type;

/**
  @brief Data type to be used with
         DS_SYS_EVENT_PS_DATA_AVAIL_CHANGE. 
   
  @see   DS_SYS_EVENT_PS_DATA_AVAIL_CHANGE 
  @see   DS_SYS_EVENT_PS_DATA_AVAIL_CHANGE  
*/
typedef struct
{
  uint32                                 num_services;
  /** Number of services*/
  ds_sys_ps_data_avail_info_type         service_info[DS_SYS_APPL_SERVICE_NAME_MAX];
  /** Array of Service and type*/
  uint32                                 reason;
  /** Reason for service type change in service info arrary*/
  ds_sys_ps_data_avail_list_type_enum_type list_type;
  /** List type*/
  ds_sys_ps_data_avail_status_enum_type  data_avail_status;
  /**Data available status*/
} ds_sys_ps_data_avail_type;

/**
  @brief Data type to be used with DS_SYS_IOCTL_GET_CURRENT_DDS
         and DS_SYS_EVENT_CURRENT_DDS_IND.

  @see   DS_SYS_IOCTL_GET_CURRENT_DDS
  @see   DS_SYS_EVENT_CURRENT_DDS_IND   
*/
typedef enum 
{
  DS_SYS_DDS_SWITCH_PERMANENT = 0x00,
  /**< Permanent(Default) */
  DS_SYS_DDS_SWITCH_TEMPORARY = 0x01
  /**< Temporary */
} ds_sys_dds_switch_type_enum;

typedef struct
{
  ds_sys_subscription_enum_type    dds;
  /** Current Default Data Subscription */
  ds_sys_dds_switch_type_enum      dds_switch_type;
  /** Enum to indicate if the DDS is permanent or temporary.
      Temporary DDS will be returned if it was set at modem by 
      APSS processor otherwise Permanent DDS will be returned. */
} ds_sys_current_dds_type;

/*===========================================================================

                           EXTERNALIZED FUNCTIONS

===========================================================================*/
/**
  @brief   This function initializes the DS Sys Conf library.

  @param   None

  @return  DSS_SUCCESS          On Success.
  @return  DSS_ERROR            On failure.
*/
int16 ds_sys_init_lib
(
  void
);

#ifdef __cplusplus
}
#endif

#endif /* DS_SYS_H */
