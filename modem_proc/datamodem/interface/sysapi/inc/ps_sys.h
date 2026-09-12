/**
@file ps_sys.h
@brief
This file defines function, variables and data structures common to all PS 
System API module 
*/

#ifndef PS_SYS_H
#define PS_SYS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        P S _ S Y S . H


GENERAL DESCRIPTION
  This file defines function, variables and data structures common to all
  PS System API module

Copyright (c) 2011-2020 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/sysapi/inc/ps_sys.h#5 $ $DateTime: 2024/05/06 04:25:26 $ $Author: pwbldsvc $
 
when        who    what, where, why
--------    ---    ---------------------------------------------------------- 
08/20/15    gk     Added support for low latency traffic status indicaation
03/29/13    svj    New APIs to modify and query attach pdn list 
09/09/11    bvd    Created module 
 
===========================================================================*/


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "queue.h"
#include "ds_sys.h"
#include "ps_in.h"

/**
  RAT Mask for 3GPP
*/
#define PS_SYS_RAT_3GPP_WCDMA                DS_SYS_RAT_3GPP_WCDMA
#define PS_SYS_RAT_3GPP_GPRS                 DS_SYS_RAT_3GPP_GPRS
#define PS_SYS_RAT_3GPP_HSDPA                DS_SYS_RAT_3GPP_HSDPA
#define PS_SYS_RAT_3GPP_HSUPA                DS_SYS_RAT_3GPP_HSUPA
#define PS_SYS_RAT_3GPP_EDGE                 DS_SYS_RAT_3GPP_EDGE
#define PS_SYS_RAT_3GPP_LTE                  DS_SYS_RAT_3GPP_LTE
#define PS_SYS_RAT_3GPP_HSDPAPLUS            DS_SYS_RAT_3GPP_HSDPAPLUS
#define PS_SYS_RAT_3GPP_DC_HSDPAPLUS         DS_SYS_RAT_3GPP_DC_HSDPAPLUS
#define PS_SYS_RAT_3GPP_64_QAM               DS_SYS_RAT_3GPP_64_QAM
#define PS_SYS_RAT_3GPP_TDSCDMA              DS_SYS_RAT_3GPP_TDSCDMA
#define PS_SYS_RAT_3GPP_NULL_BEARER          DS_SYS_RAT_3GPP_NULL_BEARER

/**
  RAT Mask for 3GPP2
*/
#define PS_SYS_RAT_3GPP2_1X               DS_SYS_RAT_3GPP2_1X
#define PS_SYS_RAT_3GPP2_EVDO_REV0        DS_SYS_RAT_3GPP2_EVDO_REV0
#define PS_SYS_RAT_3GPP2_EVDO_REVA        DS_SYS_RAT_3GPP2_EVDO_REVA
#define PS_SYS_RAT_3GPP2_EVDO_REVB        DS_SYS_RAT_3GPP2_EVDO_REVB
#define PS_SYS_RAT_3GPP2_EHRPD            DS_SYS_RAT_3GPP2_EHRPD
#define PS_SYS_RAT_3GPP2_FMC              DS_SYS_RAT_3GPP2_FMC
#define PS_SYS_RAT_3GPP2_NULL_BEARER      DS_SYS_RAT_3GPP2_NULL_BEARER

/**
  SO Mask for 1x
*/
#define PS_SYS_SO_3GPP2_1X_IS95                DS_SYS_SO_3GPP2_1X_IS95
#define PS_SYS_SO_3GPP2_1X_IS2000              DS_SYS_SO_3GPP2_1X_IS2000
#define PS_SYS_SO_3GPP2_1X_IS2000_REL_A        DS_SYS_SO_3GPP2_1X_IS2000_REL_A

#define PS_SYS_MAX_HESSID_LEN             DS_SYS_MAX_HESSID_LEN


/**
  SO Mask for DO. 
   
  The following table gives which of the following so_masks are supported by 
  which technology 
   
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
#define PS_SYS_SO_3GPP2_EVDO_DPA           DS_SYS_SO_3GPP2_EVDO_DPA
#define PS_SYS_SO_3GPP2_EVDO_MFPA          DS_SYS_SO_3GPP2_EVDO_MFPA
#define PS_SYS_SO_3GPP2_EVDO_EMPA          DS_SYS_SO_3GPP2_EVDO_EMPA
#define PS_SYS_SO_3GPP2_EVDO_EMPA_EHRPD    DS_SYS_SO_3GPP2_EVDO_EMPA_EHRPD
#define PS_SYS_SO_3GPP2_EVDO_MMPA          DS_SYS_SO_3GPP2_EVDO_MMPA
#define PS_SYS_SO_3GPP2_EVDO_MMPA_EHRPD    DS_SYS_SO_3GPP2_EVDO_MMPA_EHRPD


/**
   Defines the MAX length for PDN/APN name string
*/
#define PS_SYS_MAX_APN_LEN        DS_SYS_MAX_APN_LEN
#define PS_SYS_MAX_APNS           DS_SYS_MAX_APNS /**< Max number of APNs */
#define PS_SYS_MAX_BEARER         DS_SYS_MAX_BEARER
#define PS_SYS_LTE_ATTACH_PDN_PROFILE_LIST_MAX DS_SYS_LTE_ATTACH_PDN_PROFILE_LIST_MAX  
/**< Max number of LTE Attach PDN profile IDs */
#define PS_SYS_EHRPD_FALLBACK_APN_MAX DS_SYS_EHRPD_FALLBACK_APN_MAX
/**< Max number of EHRPD fallback APNs */

#define PS_SYS_MAX_AVAIL_SYS      DS_SYS_MAX_AVAIL_SYS
#define PS_SYS_PREF_SYS_INDEX_INVALID DS_SYS_PREF_SYS_INDEX_INVALID
/**< If number of available system is zero then there are no available 
system on which UE can make the call. In such cases, preferred system index
would be set to  PS_SYS_PREF_SYS_INDEX_INVALID. If any APN's preferred system
is not available then its preferred system index field would be set to 
PS_SYS_PREF_SYS_INDEX_INVALID. */

#define PS_SYS_MAC_ADDR_LEN       DS_SYS_MAC_ADDR_LEN
#define PS_SYS_MAX_COUNTRY_CODE_LEN 2

#define PS_SYS_MAX_NUM_THROUGHPUT_INFO    DS_SYS_MAX_NUM_THROUGHPUT_INFO
#define PS_SYS_MAX_SSID_LEN       DS_SYS_MAX_SSID_LEN
#define PS_SYS_MAX_NUM_BSSID              16
#define PS_SYS_MAX_NUM_BSSID_PER_REPORT   50
#define PS_SYS_MAX_NUM_SSID               8
#define PS_SYS_MAX_CHANNELS               16

#define PS_SYS_MAX_REMOTE_SOCKETS         255

#define PS_SYS_MAX_LTE_NUM_SCELL  DS_SYS_MAX_LTE_NUM_SCELL


/* For low tier architecture like triton, only 6 pdns are supported */
#ifdef FEATURE_DATA_MPSS_ULT
  #define PS_SYS_MAX_AVAIL_PDNS 6
#else 
  #define PS_SYS_MAX_AVAIL_PDNS 8
#endif /* FEATURE_DATA_MPSS_ULT */

/* Max length of App specific information in Operator reserved PCO */
#define PS_SYS_MAX_OP_RES_PCO_APP_SPECIFIC_LEN DS_SYS_MAX_OP_RES_PCO_APP_SPECIFIC_LEN

#define PS_SYS_MAX_NUM_PCO  DS_SYS_MAX_NUM_PCO
/**
   Defines the MAX length for file name string
*/
#define PS_SYS_MAX_FILE_NAME_LEN            101

#define PS_SYS_MAX_ICCID_LEN                10

#define PS_SYS_MCC_MNC_SIZE 4
#define PS_SYS_HPLMN_LIST_MAX  32 
#define PS_SYS_EXCP_HPLMN_LIST_MAX  32
#define PS_SYS_DOMESTIC_RPLMN_LIST_MAX 32
#define PS_SYS_INTERNATIONAL_RPLMN_LIST_MAX 32

#define PS_SYS_MAX_IMSI_PUBLIC_KEY_LEN  512
#define PS_SYS_MAX_IMSI_PUBLIC_KEY_ID_AVP_LEN 256 

/* PS Data Off Service type change reason */
#define PS_SYS_PS_DATA_AVAIL_CHG_REASON_ROAM_STATUS   DS_SYS_PS_DATA_AVAIL_CHG_REASON_ROAM_STATUS
#define PS_SYS_PS_DATA_AVAIL_CHG_REASON_MOBILE_DATA_STATUS  DS_SYS_PS_DATA_AVAIL_CHG_REASON_MOBILE_DATA_STATUS
#define PS_SYS_PS_DATA_AVAIL_CHG_REASON_ROAMING_MOBILE_DATA_STATUS DS_SYS_PS_DATA_AVAIL_CHG_REASON_ROAMING_MOBILE_DATA_STATUS
#define PS_SYS_PS_DATA_AVAIL_CHG_REASON_SIM_REFRESH DS_SYS_PS_DATA_AVAIL_CHG_REASON_SIM_REFRESH
#define PS_SYS_PS_DATA_AVAIL_CHG_REASON_OMADM DS_SYS_PS_DATA_AVAIL_CHG_REASON_OMADM 

#ifdef QWES_FEATURE_ENABLE 
#define PS_SYS_MAX_NUM_FEATURE_IDS 10
#endif /* QWES_FEATURE_ENABLE  */

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/

/**
  @brief Defines different Technology type values
*/
typedef enum
{
  PS_SYS_TECH_MIN     = DS_SYS_TECH_MIN,
  PS_SYS_TECH_3GPP    = DS_SYS_TECH_3GPP,  /**< Tech type 3gpp */
  PS_SYS_TECH_3GPP2   = DS_SYS_TECH_3GPP2, /**< Tech type 3gpp2 */
  PS_SYS_TECH_WLAN    = DS_SYS_TECH_WLAN,  /**< Tech type WLAN */
  PS_SYS_TECH_ALL     = DS_SYS_TECH_ALL,   /**< All the tech types */
  PS_SYS_TECH_MAX     = DS_SYS_TECH_MAX,
  PS_SYS_TECH_INVALID = DS_SYS_TECH_INVALID
}ps_sys_tech_enum_type;


/**
  @brief This is the type that is used to define different network types
*/
typedef enum
{
  PS_SYS_NETWORK_3GPP,  /**< Network type 3gpp */
  PS_SYS_NETWORK_3GPP2, /**< Network type 3gpp2 */
  PS_SYS_NETWORK_WLAN,  /**< Network type WLAN */
  PS_SYS_NETWORK_MAX,
} ps_sys_network_enum_type;


/**
  @brief This is the type that is used to define core network details 
  @see   ds_sys_network_info_type 
*/
typedef struct
{
  uint32   rat_mask;   /**< RAT Mask */
  uint32   so_mask;    /**< SO Mask */
} ps_sys_network_info_type;

/**
  @brief This is the type that is used to define system status. It is 
         common to both configuration PS_SYS_CONF_SYSTEM_STATUS and event
         PS_SYS_EVENT_SYSTEM_STATUS_CHANGE
   
  @see PS_SYS_CONF_SYSTEM_STATUS
  @see PS_SYS_EVENT_SYSTEM_STATUS_CHANGE 
  @see ds_sys_system_status_type 
*/
typedef struct
{
  ps_sys_network_enum_type        preferred_network; /**< pref network value*/
  ps_sys_network_info_type        network_info[PS_SYS_NETWORK_MAX];
  /**< Array of rat and SO mask for all networks */
} ps_sys_system_status_type;


/**
  @brief This is the type that is used to define WQE status
*/
typedef enum
{
  PS_SYS_WLAN_WQE_INACTIVE     = 0x0,
  PS_SYS_WLAN_WQE_ACTIVE       = 0x1
} ps_sys_wlan_wqe_status_enum_type;

/**
  @brief This is the mask for WQE profile type and also used for 
         PS_SYS_EVENT_WQE_PROFILE_CHANGE payload
*/
typedef ds_sys_wqe_profile_type ps_sys_wqe_profile_type;

typedef struct
{
  uint64                  profile_type; /** WQE profile*/
  uint8                   iccid_len; /** ICCID len*/
  uint8                   iccid[PS_SYS_MAX_ICCID_LEN]; /** ICCID*/
  uint64                  measurement_id;
} ps_sys_wqe_profile_change_type;

/**
  @brief This is the type that is used to define Wifi association type 
*/
typedef enum {
  PS_SYS_WLAN_ASSOC_P2P_GO,     /**< Peer to Peer Group WLAN association */
  PS_SYS_WLAN_ASSOC_P2P_CLIENT, /**< Peer to Peer Client WLAN association */
  PS_SYS_WLAN_ASSOC_STA,        /**< Station WLAN association */
  PS_SYS_WLAN_ASSOC_SOFTAP,     /**< Software Access Point WLAN association */
  PS_SYS_WLAN_ASSOC_INVALID = 0xFF
} ps_sys_wlan_assoc_type_enum;

typedef enum {
  PS_SYS_WLAN_CONN_STATUS_DISASSOCIATED,/** Connection status specifying if wlan is 
                                            disassociated from an access point */
  PS_SYS_WLAN_CONN_STATUS_ASSOCIATED,   /** Connection status specifying if wlan is 
                                            associated with an access point */
  PS_SYS_WLAN_CONN_STATUS_IP_CONNECTED, /** Connection status specifying if wlan is 
                                            IP connected */
  PS_SYS_WLAN_CONN_STATUS_INVALID = 0xFF
} ps_sys_wlan_conn_status_type_enum;

typedef enum {
  PS_SYS_WLAN_NETWORK_MODE_802_11_1997    = DS_SYS_WLAN_NETWORK_MODE_802_11_1997,/**< Legacy network Mode. */
  PS_SYS_WLAN_NETWORK_MODE_802_11a        = DS_SYS_WLAN_NETWORK_MODE_802_11a,    /**< OFDM Network mode. */
  PS_SYS_WLAN_NETWORK_MODE_802_11b        = DS_SYS_WLAN_NETWORK_MODE_802_11b,    /**< 802.11b */
  PS_SYS_WLAN_NETWORK_MODE_802_11g        = DS_SYS_WLAN_NETWORK_MODE_802_11g,    /**< 802.11g */
  PS_SYS_WLAN_NETWORK_MODE_802_11_2007    = DS_SYS_WLAN_NETWORK_MODE_802_11_2007,/**< 802.11.2007 */
  PS_SYS_WLAN_NETWORK_MODE_802_11n        = DS_SYS_WLAN_NETWORK_MODE_802_11n,    /**< 802.11n */
  PS_SYS_WLAN_NETWORK_MODE_802_11_2012    = DS_SYS_WLAN_NETWORK_MODE_802_11_2012,/**< 802.11.2012 */
  PS_SYS_WLAN_NETWORK_MODE_802_11ac       = DS_SYS_WLAN_NETWORK_MODE_802_11ac,   /**< 802.11.ac */
  PS_SYS_WLAN_NETWORK_MODE_802_11ad       = DS_SYS_WLAN_NETWORK_MODE_802_11ad,   /**< 802.11.ad */
  PS_SYS_WLAN_NETWORK_MODE_802_11af       = DS_SYS_WLAN_NETWORK_MODE_802_11af,   /**< 802.11.af */
  PS_SYS_WLAN_NETWORK_MODE_802_11ah       = DS_SYS_WLAN_NETWORK_MODE_802_11ah,   /**< 802.11.ah */
  PS_SYS_WLAN_NETWORK_MODE_802_11ai       = DS_SYS_WLAN_NETWORK_MODE_802_11ai,   /**< 802.11.ai */
  PS_SYS_WLAN_NETWORK_MODE_802_11aj       = DS_SYS_WLAN_NETWORK_MODE_802_11aj,   /**< 802.11.aj */
  PS_SYS_WLAN_NETWORK_MODE_802_11aq       = DS_SYS_WLAN_NETWORK_MODE_802_11aq,   /**< 802.11.aq */
  PS_SYS_WLAN_NETWORK_MODE_802_11ax       = DS_SYS_WLAN_NETWORK_MODE_802_11ax,   /**< 802.11.ax */
  PS_SYS_WLAN_NETWORK_MODE_802_11ay       = DS_SYS_WLAN_NETWORK_MODE_802_11ay,   /**< 802.11.ay */
  PS_SYS_WLAN_NETWORK_MODE_INVALID        = DS_SYS_WLAN_NETWORK_MODE_INVALID
} ps_sys_wlan_network_mode_type_enum;

typedef enum {
  PS_SYS_BANDWIDTH_TYPE_20MHz, /**< 20 MHz Bandwidth type*/
  PS_SYS_BANDWIDTH_TYPE_40MHz, /**< 40 MHz Bandwidth type*/
  PS_SYS_BANDWIDTH_TYPE_80MHz, /**< 80 MHz Bandwidth type*/
  PS_SYS_BANDWIDTH_TYPE_160MHz, /**< 160 MHz Bandwidth type*/
  PS_SYS_BANDWIDTH_TYPE_80MHz_80MHz, /**< 80 + 80 MHz Bandwidth type*/
  PS_SYS_BANDWIDTH_TYPE_INVALID = 0xFF
} ps_sys_wlan_bandwidth_type_enum;

/**
  @brief This is the type that is used for 
         PS_SYS_CONF_WLAN_STATUS configuration and
         PS_SYS_EVENT_WLAN_STATUS_CHANGE
   
  @see PS_SYS_CONF_WLAN_STATUS
  @see PS_SYS_EVENT_WLAN_STATUS_CHANGE 
*/

typedef struct
{
  boolean                            is_wlan_available; /**< WLAN availability*/
  ps_ip_addr_type                    ipv4_addr;  /**< WLAN IPV4 address */
  ps_ip_addr_type                    ipv6_addr;  /**< WLAN IPV6 address */
  uint8                              ipv6_prefix_len;  /**< IPV6 addr prefix length */
  uint8                              ap_mac_addr[PS_SYS_MAC_ADDR_LEN]; /**< WLAN Access Point MAC addr*/
  ps_sys_wlan_wqe_status_enum_type   wqe_status;  /**< Whether Wifi Quality Estimation was performed on the Apps Proc*/
  ps_ip_addr_type                    dns_ipv4_addr1;  /**< DNS IPV4 address 1 */
  ps_ip_addr_type                    dns_ipv4_addr2;  /**< DNS IPV4 address 2 */
  ps_ip_addr_type                    dns_ipv6_addr1;  /**< DNS IPV6 address 1 */
  ps_ip_addr_type                    dns_ipv6_addr2;  /**< DNS IPV6 address 2 */
  ps_ip_addr_type                    epdg_ipv4_addr1;  /**< EPDG IPV4 address 1 */
  ps_ip_addr_type                    epdg_ipv4_addr2;  /**< EPDG IPV4 address 2 */
  ps_ip_addr_type                    epdg_ipv6_addr1;  /**< EPDG IPV6 address 1 */
  ps_ip_addr_type                    epdg_ipv6_addr2;  /**< EPDG IPV6 address 2 */
  char                               ssid[PS_SYS_MAX_SSID_LEN]; /**< SSID */
  boolean                            is_channel_valid;  /**< Is channel valid*/
  uint16                             channel;           /**< Channel in MHz*/
  boolean                            is_channel_bandwidth_valid;  /**< Is channel valid*/
  uint16                             channel_bandwidth; /**< Channel bandwidth*/ 
  ps_sys_wqe_profile_type            wqe_profile_type;  /**< Wifi Quality Estimation Profile Type*/
  ps_sys_wlan_assoc_type_enum        wlan_assoc_type;   /**< Wlan Association Type*/
  ps_sys_wlan_network_mode_type_enum network_mode;      /**< Network Mode*/
  ps_sys_wlan_conn_status_type_enum  connection_status; /**< Wlan Connection Status */
  ps_sys_wlan_bandwidth_type_enum    bandwidth_type;    /**< Bandwidth Type*/
  boolean                            is_secondary_channel_valid; /**< Is channel valid*/
  uint16                             secondary_channel; /**< Secondary Channel*/
  boolean                            is_default_route;  /**< Default Route. */
  uint8                              hessid[PS_SYS_MAX_HESSID_LEN];
  uint8                              country_code[PS_SYS_MAX_COUNTRY_CODE_LEN]; 
  /**< Country Code. Country code 0 means invalid country code */  
} ps_sys_wlan_status_type;

/**
  @brief This is the type that is used for 
         PS_SYS_CONF_GET_WLAN_MAC_ADDR configuration and
         PS_SYS_EVENT_WLAN_MAC_ADDR_CHANGE
   
  @see PS_SYS_CONF_GET_WLAN_MAC_ADDR
  @see PS_SYS_EVENT_WLAN_MAC_ADDR_CHANGE 
*/
typedef struct
{
  uint8            ap_mac_addr[DS_SYS_MAC_ADDR_LEN]; 
  /**< WLAN Access Point MAC addr*/
} ps_sys_wlan_mac_addr_type;

/**
  @brief This is the type that is used for 
         PS_SYS_CONF_GET_WLAN_SSID configuration and
         PS_SYS_EVENT_WLAN_SSID_CHANGE
   
  @see PS_SYS_CONF_GET_WLAN_SSID
  @see PS_SYS_EVENT_WLAN_SSID_CHANGE 
*/
typedef struct
{
  char             ssid[PS_SYS_MAX_SSID_LEN]; 
  /**< WLAN Service Set Identifier*/
} ps_sys_wlan_ssid_type;

/**
  @brief This is the type that is used for 
         PS_SYS_CONF_GET_WLAN_HESSID configuration and
         PS_SYS_EVENT_WLAN_HESSID_CHANGE
   
  @see PS_SYS_CONF_GET_WLAN_HESSID
  @see PS_SYS_EVENT_WLAN_HESSID_CHANGE 
*/

typedef ds_sys_wlan_hessid_type ps_sys_wlan_hessid_type;

/**
  @brief This is the type that is used for 
         PS_SYS_CONF_GET_WLAN_IP_ADDR configuration and
         PS_SYS_EVENT_WLAN_IP_ADDR_CHANGE
   
  @see PS_SYS_CONF_GET_WLAN_IP_ADDR
  @see PS_SYS_EVENT_WLAN_IP_ADDR_CHANGE 
*/
typedef struct
{
  ps_ip_addr_type                  ipv4_addr;  /**< WLAN IPV4 address */
  ps_ip_addr_type                  ipv6_addr;  /**< WLAN IPV6 address */
  uint8                            ipv6_prefix_len;  /**< IPV6 addr prefix length */
} ps_sys_wlan_ip_addr_type;

/**
  @brief This is the type that is used for 
         PS_SYS_CONF_WLAN_PREFERENCE configuration and
         PS_SYS_EVENT_WLAN_PREFERENCE_CHANGE
   
  @see PS_SYS_CONF_WLAN_PREFERENCE
  @see PS_SYS_EVENT_WLAN_PREFERENCE_CHANGE 
*/
typedef enum
{
  PS_SYS_WLAN_LB_PREFERRED     = 0x00,
  PS_SYS_WLAN_LB_NOT_PREFERRED = 0x01
} ps_sys_wlan_preference_enum_type;

/**
  @brief Defines structure for PS_SYS_EVENT_WLAN_MAX_CONN event
  
  @see PS_SYS_EVENT_WLAN_MAX_CONN 
*/
typedef struct
{
  boolean is_max_conn_reached; 
  uint8   num_apns;
  char    active_apn_name[PS_SYS_MAX_AVAIL_PDNS][PS_SYS_MAX_APN_LEN];
}ps_sys_wlan_max_conn_info_type;

/**
  @brief This is the bit mask for the different WIFI service capabilities
*/
#define PS_SYS_WIFI_SRV_CAP_MASK_UNSPECIFIED           0x0000000000000000
#define PS_SYS_WIFI_SRV_CAP_MASK_BAND_VACATE_CAPABLE   0x0000000000000001
#define PS_SYS_WIFI_SRV_CAP_MASK_MAX                   0x0000000000000002

/**
  @brief Data type to store the wifi service capabilities
  @see PS_SYS_EVENT_WIFI_SERVICE_CAP
*/
typedef struct
{
  /**< Bitmask of wifi service capabilities types*/
  uint32      wifi_srv_cap_mask;
} ps_sys_wifi_srv_cap_type;


/**
  @brief This is the type that is used for
         PS_SYS_EVENT_WIFI_BAND_VACATE_IND
   
  @see PS_SYS_EVENT_WIFI_BAND_VACATE_IND 
*/
typedef enum
{
  PS_SYS_WIFI_BAND_VACATE_REQUEST_NOT_RESTRICTED  = 0x00,
  /**< Wifi can operate in any band */
  PS_SYS_WIFI_BAND_VACATE_REQUEST_5GHZ            = 0x01
  /**< Wifi request to leave 5GHz band */
} ps_sys_wifi_band_vacate_request_enum_type;

/**
  @brief Data type to be used with PS_SYS_EVENT_SET_WLAN_BAND_VACATE_IND
*/
typedef struct
{
  ps_sys_wifi_band_vacate_request_enum_type      wifi_band_vacate_request;
  /**< wifi band vacate enum */
} ps_sys_wifi_band_vacate_type;


/**
  @brief This is the type that is used to identify the list of 
         channels associated with each BSSID per SSID for which 
         WiFi measurements are required with each 
         PS_SYS_EVENT_WLAN_START_WIFI_MEAS
  @see PS_SYS_EVENT_WLAN_START_WIFI_MEAS
*/

typedef struct
{
  uint8              bssid[PS_SYS_MAC_ADDR_LEN];         /**BSSID*/
  uint8              num_channel_info;                   /** Num of channels*/
  uint16             channel_list[PS_SYS_MAX_CHANNELS];  /**< List of channels in MHz */
}ps_sys_wlan_wifi_meas_bssid_type;

typedef struct
{
  char               ssid[PS_SYS_MAX_SSID_LEN];                     
  /**< SSID */
  boolean            is_hidden;
  /**< Is the SSID Hidden. */
  uint8              num_bssid_info;                     
  /**< num of BSSID info*/
  ps_sys_wlan_wifi_meas_bssid_type bssid_info[PS_SYS_MAX_NUM_BSSID];
   /**< List of BSSID info*/
} ps_sys_wlan_wifi_meas_ssid_type;

typedef struct
{
  uint8              num_ssid_info;
  /**< num of SSID info*/
  ps_sys_wlan_wifi_meas_ssid_type ssid_info[PS_SYS_MAX_NUM_SSID];
  /**< List of SSID info*/
} ps_sys_wlan_wifi_meas_ssid_list;

/* Wifi measurement interested parameter mask has a size of 8 bytes */
#define PS_SYS_WIFI_MEAS_PARAM_UNSPECIFIED          0x0000000000000000
#define PS_SYS_WIFI_MEAS_PARAM_RSSI                 0x0000000000000001
#define PS_SYS_WIFI_MEAS_PARAM_SINR                 0x0000000000000002
#define PS_SYS_WIFI_MEAS_PARAM_BSS_LOAD             0x0000000000000004
#define PS_SYS_WIFI_MEAS_PARAM_PHY_RATE             0x0000000000000008
#define PS_SYS_WIFI_MEAS_PARAM_PACKET_ERROR_RATE    0x0000000000000010

/* Wifi Report type*/
typedef enum 
{
  PS_SYS_WIFI_MEAS_REPORT_TYPE_PERIODIC_RAW,             
  /**< Report Raw Wifi Measurements values periodically. */
  PS_SYS_WIFI_MEAS_REPORT_TYPE_ONE_TIME_AFTER_THRESHOLD, 
  /**< Report Wifi Measurements once after crossing the threshold. */
  PS_SYS_WIFI_MEAS_REPORT_TYPE_PERIODIC_AFTER_THRESHOLD 
  /**< Report Wifi Measurements periodically after crossing the threshold. */
} ps_sys_wlan_wifi_meas_report_type_enum;

/* Wifi RSSI Threshold Type */
typedef struct 
{
  int16 rssi_high;
  /**< RSSI high threshold value for the channel*/
  int16 rssi_low;
  /**< RSSI low  threshold value for the channel*/
} ps_sys_wlan_wifi_ident_rssi_threshold_type;

/* Wifi Signal Interference to Noise ratio Threshold*/
typedef struct 
{
  int16 sinr_high;
  /**< SINR high threshold value for the channel*/
  int16 sinr_low;
  /**< SINR low threshold value for the channel*/
} ps_sys_wlan_wifi_ident_sinr_threshold_type;

/* BSS Load Threshold type */
typedef struct 
{
  uint16 station_count_high;
  /**< Station count high threshold. */
  uint16 station_count_low;
  /**< Station count low threshold.  */
  uint16 channel_utilization_high;
  /**< Channel utilization high threshold.  */
  uint16 channel_utilization_low;
  /**< Channel utilization low threshold. */
  uint16 available_admission_capacity_high;
  /**< Available Admission Capacity high threshold. */
  uint16 available_admission_capacity_low;
  /**< Available Admission Capacity low threshold.  */
} ps_sys_wlan_wifi_ident_bss_load_threshold_type;

typedef struct
{
  uint8              num_blacklisted_bssid;
  /**< num of SSID info*/
  ps_sys_wlan_mac_addr_type bssid[PS_SYS_MAX_NUM_BSSID]; 
  /**< BSSID list*/
} ps_sys_wlan_wifi_blacklist_bssid_type;

typedef enum 
{
  PS_SYS_WLAN_WIFI_FW_ACTION_PNO,
  /**< Preferred Network Offload . 
       Wlan firmware to provide measurement reports. */ 
  PS_SYS_WLAN_WIFI_ACTION_PNO_ASSOCIATE_WN,
  /**< Preferred Network Offload for neighboring Wlan. This indicates TE to find a
       good neighbor and connect automatically.*/
  PS_SYS_WLAN_WIFI_ACTION_LFR,
  /**< Legacy Fast Roaming.
       Wlan Firmware Scans and reports measurements based on threshold*/
  PS_SYS_WLAN_WIFI_FW_ACTION_INVALID = 0xFF
  /**< No action required by WLAN FW. */
} ps_sys_wlan_wifi_fw_action_type_enum;

typedef struct
{
  uint32                           meas_id; 
  /**< ID associated with every wifi meas start*/
  uint64                           meas_param_mask;  
  /**< Mask of interested parameters */
  ps_sys_wlan_wifi_meas_report_type_enum report_type;
  /**< Wifi meas report type*/
  boolean                          is_valid_rssi_threshold;
  /**< boolean to specify if rssi threshold is set*/
  ps_sys_wlan_wifi_ident_rssi_threshold_type rssi_threshold;
  /**< RSSI Threshold values*/
  boolean                          is_valid_sinr_threshold;
  /**< boolean to specify if sinr threshold is set*/
  ps_sys_wlan_wifi_ident_sinr_threshold_type sinr_threshold;
  /**< SINR Threshold values*/
  boolean                          is_valid_bss_load_threshold;
  /**< boolean to specify if bss_load threshold is set*/
  ps_sys_wlan_wifi_ident_bss_load_threshold_type bss_load_threshold;
  /**< BSS Threshold values*/
  ps_sys_wlan_wifi_meas_ssid_list   ssid_list;        
  /**< Key based on which the measurements are reported */
  boolean                          is_valid_sampling_timer;
  /**< boolean to specify if sampling timer is set*/
  uint32                           sampling_timer;  
  /**< Time in ms after which all measurements are to be sampled */
  boolean                          is_valid_report_interval;
  /**< boolean to specify if report interval is set*/
  uint32                           report_interval;  
  /**< Time in ms over which report is to be sent*/
  boolean                          is_valid_alpha;
  /**< boolean to specify if alpha is set*/
  float                            alpha; 
  /**< Coefficient used to calculate average */
  boolean                          is_valid_time_to_trigger;
  /**< boolean to specify if time to trigger is set*/
  uint64                           time_to_trigger;
  /**< Time in ms to wait before sending Wifi reports*/
  ps_sys_wlan_wifi_blacklist_bssid_type blacklisted_bssid;
  /**< List of blacklisted BSSIDs*/
  ps_sys_wlan_wifi_fw_action_type_enum  fw_action;
  /**< Wlan firmware action*/
} ps_sys_wlan_wifi_meas_start_type;

/**
  @brief This is the type that is used for 
         PS_SYS_EVENT_WLAN_STOP_WIFI_MEAS
   
  @see PS_SYS_EVENT_WLAN_STOP_WIFI_MEAS 
*/

typedef struct
{
  uint32   meas_id;
  /**< ID associated with every wifi meas start*/
} ps_sys_wlan_wifi_meas_stop_type;

/**
  @brief This is the type that is used for 
         PS_SYS_IOCTL_WLAN_WIFI_MEAS_REPORT
   
  @see PS_SYS_IOCTL_WLAN_WIFI_MEAS_REPORT 
*/

typedef enum
{
  PS_SYS_WLAN_WIFI_MEAS_THRESHOLD_NOT_CROSSED  = 0,  
  /**<  Threshold not crossed.  */
  PS_SYS_WLAN_WIFI_MEAS_THRESHOLD_HIGH_CROSSED = 1,
  /**<  High Threshold crossed. */
  PS_SYS_WLAN_WIFI_MEAS_THRESHOLD_LOW_CROSSED  = 2  
  /**<  Low Threshold crossed.  */
}ps_sys_wlan_wifi_meas_threshold_state_enum;

typedef struct 
{
  uint16 downlink_phy_rate;
  /**<   Downlink Data Rate of Physical layer. */
  uint16 uplink_phy_rate;
  /**<   Uplink Data Rate of Physical layer. */
} ps_sys_wlan_wifi_phy_rate_type;

typedef struct 
{
  uint16 downlink_packet_error_ratio;
  /**<   Downlink Packet Error Ratio. */
  uint16 uplink_packet_error_ratio;
  /**<   Uplink Packet Error Ratio. */
} ps_sys_wlan_wifi_packet_error_ratio_type;

typedef struct
{
  uint16 station_count;
  /**< Station count.  */
  uint16 channel_utilization;
  /**< Channel utilization.  */
  uint16 available_admission_capacity;
  /**< Available Admission Capacity.  */
} ps_sys_wlan_wifi_bss_load_type;

typedef struct
{
  uint16  channel;
  /**< Channel in MHz.  */
  uint64  meas_param_mask;  
  /**< Mask of valid parameters  */
  ps_sys_wlan_wifi_meas_threshold_state_enum threshold_state;
  /**< Enum specifying the state of the threshold */
  int16   rssi;
  /**< Received Signal Strength Indication value for the channel*/
  int16   sinr;
  /**< Signal to Inteference plus Noise Ratio value for the channel */
  ps_sys_wlan_wifi_bss_load_type bss_load;
  /**< Basic Service Set Load value for the channel */
  ps_sys_wlan_wifi_phy_rate_type phy_rate;
  /**< Physical Layer transmission rate */
  ps_sys_wlan_wifi_packet_error_ratio_type packet_err_ratio;
  /**< Packet error Ratio*/
  ps_sys_wlan_network_mode_type_enum network_mode;      
  /**< Network Mode*/
} ps_sys_wlan_wifi_meas_info_per_channel_type;

typedef struct {
  uint8 bssid[PS_SYS_MAC_ADDR_LEN];
  /**< Basic Service Set Identifier.  */
  ps_sys_wlan_wifi_meas_info_per_channel_type channel_info;
  /**< WiFi measurement information for the channel.  */
} ps_sys_wlan_wifi_meas_info_per_bssid_type;

typedef struct {
  uint16 secondary_channel;
  /**< Secondary Channel in MHz.  */
  ps_sys_wlan_bandwidth_type_enum bandwidth_type;
  /**< Bandwidth type per BSSID.  
  */
} ps_sys_wlan_wifi_meas_secondary_bssid_info_type;


typedef struct
{
  uint32  meas_id;
  /**< ID associated with every wifi meas start*/
  uint32  tx_id;
  /**< Identify a set of measurements for a given meas_id*/
  ps_sys_wlan_wifi_meas_report_type_enum report_type;
  /**< Wifi meas report type*/
  boolean is_last_report_of_transaction;
  /**< Boolean to specify the last report, 
    if reports per SSID are split,*/
  char    ssid[PS_SYS_MAX_SSID_LEN]; 
  /**< SSID */
  uint8   num_bssid_info;
  /**< Number of BSSID info*/
  ps_sys_wlan_wifi_meas_info_per_bssid_type per_bssid_info[PS_SYS_MAX_NUM_BSSID_PER_REPORT];
  /**< WiFi measurement information for each BSSID. */
  ps_sys_wlan_wifi_meas_secondary_bssid_info_type 
    secondary_bssid_info[PS_SYS_MAX_NUM_BSSID_PER_REPORT]; 
  /**< Secondary info per BSSID. */
} ps_sys_wlan_wifi_meas_report_type;

/**
  @brief This is the type that is used for 
         PS_SYS_IOCTL_WLAN_WIFI_MEAS_REPORT_CONFIG_STATUS
   
  @see PS_SYS_IOCTL_WLAN_WIFI_MEAS_REPORT_CONFIG_STATUS 
*/

typedef enum 
{
  PS_SYS_WLAN_WIFI_MEAS_REPORT_CONFIG_OK, 
  /**< Configuration OK \n */
  PS_SYS_WLAN_WIFI_MEAS_REPORT_CONFIG_ERROR, 
  /**< Configuration error \n */
  PS_SYS_WLAN_WIFI_MEAS_REPORT_STOPPED  
  /**< Report Stopped */
} ps_sys_wlan_wifi_meas_report_config_status_enum_type;

typedef struct 
{
  uint32 wifi_meas_id;
  /**< ID provided with every start Wifi meas. 
  */
  ps_sys_wlan_wifi_meas_report_config_status_enum_type status;
  /**< Status regarding Wifi report config */
} ps_sys_wlan_wifi_meas_report_config_status_type;

/**
  @brief This is the type that is used for PS_SYS_CONF_3GPP2_S101_STATUS_CHANGE 
         configuration and PS_SYS_EVENT_3GPP2_S101_STATUS_CHANGE event
  @see   PS_SYS_CONF_3GPP2_S101_STATUS_CHANGE 
  @see   PS_SYS_EVENT_3GPP2_S101_STATUS_CHANGE 
*/
typedef struct
{
  boolean tunnel_call_allowed;           /**< if tunnel call is allowed */
  boolean lcp_throttled;                 /**< if lcp is throttle */
  boolean virtual_connection_throttled;  /**< if virtual connection is 
                                              throttled */
  boolean ehrpd_active_handoff_in_prog;   /**< if eHRPD Active Handoff is in
                                              progress*/
} ps_sys_3gpp2_s101_status_change_type;

typedef ds_sys_pdn_ctxt_throttle_info_type ps_sys_pdn_ctxt_throttle_info_type;

typedef ds_sys_pdn_throttle_info_type ps_sys_pdn_throttle_info_type;

/**
  @brief Defines the pdn status
*/
typedef enum
{
  PS_SYS_PDN_DOWN,
  PS_SYS_PDN_UP, 
} ps_sys_pdn_status_enum_type;

/**
  @brief Defines structure for PS_SYS_EVENT_PDN_STATUS_INFO event
  pdn_status     : pdn is up or down
  ip_type        : IP address type
  apn_string     : apn name
  apn_string_len : length of the apn name
  @see PS_SYS_EVENT_PDN_STATUS_INFO 
*/
typedef struct
{
  ps_sys_pdn_status_enum_type  pdn_status; 
  ip_addr_enum_type            ip_type;                     
  uint16                       apn_string_len;                 
  char                         apn_string[PS_SYS_MAX_APN_LEN]; 
} ps_sys_pdn_status_info_type;

/**
  @brief Event info type for PS_SYS_EVENT_3GPP2_EVDO_PAGE_MONITOR_PERIOD_CHANGE
         event
*/
typedef uint8 ps_sys_3gpp2_page_monitor_period_type;

typedef struct
{
  ps_sys_3gpp2_page_monitor_period_type       page_monitor_period;
  boolean                                     force_long_sleep;
} ps_sys_3gpp2_evdo_page_monitor_period_type;

/**
  @brief Gives the information about hysteresis cancellation 
         or expiry on the current EPZID.
   
  @see PS_SYS_EVENT_3GPP2_EPZID_INFO
*/
typedef struct
{
  uint8   epzid_info_val;
} ps_sys_3gpp2_epzid_info_type;

/**
  @brief Enum type for roaming type
 */
typedef enum{
  PS_SYS_ROAMING_UNSPECIFIED   = DS_SYS_ROAMING_UNSPECIFIED,
  PS_SYS_ROAMING_DOMESTIC      = DS_SYS_ROAMING_DOMESTIC,
  PS_SYS_ROAMING_INTERNATIONAL = DS_SYS_ROAMING_INTERNATIONAL
}ps_sys_roaming_type_enum_type;

/**
  @brief Event info type for roaming status change  
  @see PS_SYS_EVENT_ROAMING_STATUS_CHANGE
  @see PS_SYS_CONF_ROAMING_STATUS_INFO
*/
typedef ds_sys_roaming_status_info_type ps_sys_roaming_status_info_type;

/**
  @brief This is the type that is used to  define different RATs types
*/
typedef enum
{
  PS_SYS_RAT_EX_NULL_BEARER           = DS_SYS_RAT_EX_NULL_BEARER,

  /* 3GPP RAT Values */
  PS_SYS_RAT_EX_3GPP_WCDMA             = DS_SYS_RAT_EX_3GPP_WCDMA,     
  PS_SYS_RAT_EX_3GPP_GERAN             = DS_SYS_RAT_EX_3GPP_GERAN,
  PS_SYS_RAT_EX_3GPP_LTE               = DS_SYS_RAT_EX_3GPP_LTE,
  PS_SYS_RAT_EX_3GPP_TDSCDMA           = DS_SYS_RAT_EX_3GPP_TDSCDMA,
  PS_SYS_RAT_EX_3GPP_WLAN              = DS_SYS_RAT_EX_3GPP_WLAN,
#ifdef FEATURE_LAPP
  PS_SYS_RAT_EX_3GPP_5G                = DS_SYS_RAT_EX_3GPP_5G,
#endif /* FEATURE_LAPP */
  PS_SYS_RAT_EX_3GPP_MAX               = DS_SYS_RAT_EX_3GPP_MAX,

  /* 3GPP2 RAT Values */
  PS_SYS_RAT_EX_3GPP2_1X               = DS_SYS_RAT_EX_3GPP2_1X,
  PS_SYS_RAT_EX_3GPP2_HRPD             = DS_SYS_RAT_EX_3GPP2_HRPD,
  PS_SYS_RAT_EX_3GPP2_EHRPD            = DS_SYS_RAT_EX_3GPP2_EHRPD,
  PS_SYS_RAT_EX_3GPP2_WLAN             = DS_SYS_RAT_EX_3GPP2_WLAN,
  PS_SYS_RAT_EX_3GPP2_MAX              = DS_SYS_RAT_EX_3GPP2_MAX, 

  /* WLAN RAT Values */
  PS_SYS_RAT_EX_WLAN                   = DS_SYS_RAT_EX_WLAN,
  PS_SYS_RAT_EX_WLAN_MAX               = DS_SYS_RAT_EX_WLAN_MAX,
 
  PS_SYS_RAT_UNKNOWN                   = DS_SYS_RAT_UNKNOWN,
  PS_SYS_RAT_MAX,
} ps_sys_rat_ex_enum_type;

/* SO Mask not available or not specified */
#define PS_SYS_SO_EX_UNSPECIFIED            DS_SYS_SO_EX_UNSPECIFIED

/* 3GPP SO Mask */
#define PS_SYS_SO_EX_3GPP_WCDMA             DS_SYS_SO_EX_3GPP_WCDMA
#define PS_SYS_SO_EX_3GPP_HSDPA             DS_SYS_SO_EX_3GPP_HSDPA
#define PS_SYS_SO_EX_3GPP_HSUPA             DS_SYS_SO_EX_3GPP_HSUPA
#define PS_SYS_SO_EX_3GPP_HSDPAPLUS         DS_SYS_SO_EX_3GPP_HSDPAPLUS
#define PS_SYS_SO_EX_3GPP_DC_HSDPAPLUS      DS_SYS_SO_EX_3GPP_DC_HSDPAPLUS
#define PS_SYS_SO_EX_3GPP_64_QAM            DS_SYS_SO_EX_3GPP_64_QAM
#define PS_SYS_SO_EX_3GPP_HSPA              DS_SYS_SO_EX_3GPP_HSPA
#define PS_SYS_SO_EX_3GPP_GPRS              DS_SYS_SO_EX_3GPP_GPRS
#define PS_SYS_SO_EX_3GPP_EDGE              DS_SYS_SO_EX_3GPP_EDGE

/* GSM so mask is depreciated. Can only be GPRS or EDGE */
#define PS_SYS_SO_EX_3GPP_GSM               DS_SYS_SO_EX_3GPP_GSM

#define PS_SYS_SO_EX_3GPP_S2B               DS_SYS_SO_EX_3GPP_S2B
#define PS_SYS_SO_EX_3GPP_S2B_LIMITED_SRVC  DS_SYS_SO_EX_3GPP_S2B_LIMITED_SRVC
#define PS_SYS_SO_EX_3GPP_LTE_LIMITED_SRVC  DS_SYS_SO_EX_3GPP_LTE_LIMITED_SRVC
#define PS_SYS_SO_EX_3GPP_LTE_FDD           DS_SYS_SO_EX_3GPP_LTE_FDD
#define PS_SYS_SO_EX_3GPP_LTE_TDD           DS_SYS_SO_EX_3GPP_LTE_TDD
#define PS_SYS_SO_EX_3GPP_TDSCDMA           DS_SYS_SO_EX_3GPP_TDSCDMA
#define PS_SYS_SO_EX_3GPP_DC_HSUPA          DS_SYS_SO_EX_3GPP_DC_HSUPA
#define PS_SYS_SO_EX_3GPP_LTE_CA_DL         DS_SYS_SO_EX_3GPP_LTE_CA_DL
#define PS_SYS_SO_EX_3GPP_LTE_CA_UL         DS_SYS_SO_EX_3GPP_LTE_CA_UL
#define PS_SYS_SO_EX_3GPP_FOUR_POINT_FIVE_G \
  DS_SYS_SO_EX_3GPP_FOUR_POINT_FIVE_G
#define PS_SYS_SO_EX_3GPP_FOUR_POINT_FIVE_G_PLUS \
  DS_SYS_SO_EX_3GPP_FOUR_POINT_FIVE_G_PLUS

/* 3GPP2 SO Mask */
#define PS_SYS_SO_EX_3GPP2_1X_IS95           DS_SYS_SO_EX_3GPP2_1X_IS95
#define PS_SYS_SO_EX_3GPP2_1X_IS2000         DS_SYS_SO_EX_3GPP2_1X_IS2000
#define PS_SYS_SO_EX_3GPP2_1X_IS2000_REL_A   DS_SYS_SO_EX_3GPP2_1X_IS2000_REL_A
#define PS_SYS_SO_EX_3GPP2_HDR_REV0_DPA      DS_SYS_SO_EX_3GPP2_HDR_REV0_DPA
#define PS_SYS_SO_EX_3GPP2_HDR_REVA_DPA      DS_SYS_SO_EX_3GPP2_HDR_REVA_DPA
#define PS_SYS_SO_EX_3GPP2_HDR_REVB_DPA      DS_SYS_SO_EX_3GPP2_HDR_REVB_DPA
#define PS_SYS_SO_EX_3GPP2_HDR_REVA_MPA      DS_SYS_SO_EX_3GPP2_HDR_REVA_MPA
#define PS_SYS_SO_EX_3GPP2_HDR_REVB_MPA      DS_SYS_SO_EX_3GPP2_HDR_REVB_MPA
#define PS_SYS_SO_EX_3GPP2_HDR_REVA_EMPA     DS_SYS_SO_EX_3GPP2_HDR_REVA_EMPA
#define PS_SYS_SO_EX_3GPP2_HDR_REVB_EMPA     DS_SYS_SO_EX_3GPP2_HDR_REVB_EMPA
#define PS_SYS_SO_EX_3GPP2_HDR_REVB_MMPA     DS_SYS_SO_EX_3GPP2_HDR_REVB_MMPA
#define PS_SYS_SO_EX_3GPP2_EVDO_FMC          DS_SYS_SO_EX_3GPP2_EVDO_FMC
#define PS_SYS_SO_EX_3GPP2_1X_CS             DS_SYS_SO_EX_3GPP2_1X_CS

#ifdef FEATURE_LAPP
#define PS_SYS_SO_EX_3GPP_5G_TDD             DS_SYS_SO_EX_3GPP_5G_TDD
#endif /* FEATURE_LAPP */

#define PS_SYS_NULL_BEARER_REASON_NONE                  DS_SYS_NULL_BEARER_REASON_NONE
#define PS_SYS_NULL_BEARER_REASON_CSFB                  DS_SYS_NULL_BEARER_REASON_CSFB
#define PS_SYS_NULL_BEARER_REASON_OOS                   DS_SYS_NULL_BEARER_REASON_OOS
#define PS_SYS_NULL_BEARER_REASON_LIMITED_SRVC          DS_SYS_NULL_BEARER_REASON_LIMITED_SRVC
#define PS_SYS_NULL_BEARER_REASON_VOICE_ON_SAME_SUB     DS_SYS_NULL_BEARER_REASON_VOICE_ON_SAME_SUB
#define PS_SYS_NULL_BEARER_REASON_VOICE_ON_OTHER_SUB    DS_SYS_NULL_BEARER_REASON_VOICE_ON_OTHER_SUB
#define PS_SYS_NULL_BEARER_REASON_SRVCC                 DS_SYS_NULL_BEARER_REASON_SRVCC
#define PS_SYS_NULL_BEARER_REASON_CS_ONLY               DS_SYS_NULL_BEARER_REASON_CS_ONLY
#define PS_SYS_NULL_BEARER_REASON_ATTACH_PENDING        DS_SYS_NULL_BEARER_REASON_ATTACH_PENDING

/**
  @brief This is the type that is used to  define different subscription types
         Unknown subs id is used when the client does not care/provide the subs id
*/
typedef enum 
{
  PS_SYS_DEFAULT_SUBS      = DS_SYS_DEFAULT_SUBS, /**< Default data subscription */
  PS_SYS_PRIMARY_SUBS      = DS_SYS_PRIMARY_SUBS, /**< Primary */
  PS_SYS_SECONDARY_SUBS    = DS_SYS_SECONDARY_SUBS, /**< Secondary */
  PS_SYS_TERTIARY_SUBS     = DS_SYS_TERTIARY_SUBS,  /**< Tertiary */  
  PS_SYS_SUBS_MAX          = DS_SYS_SUBS_MAX
} ps_sys_subscription_enum_type;

/**
  @brief This is the tupple that defines network, RAT, SO and service status. This structure is used in PS_SYS_IOCT_3GPP_GET_PREV_RAT_INFO.
*/
typedef struct 
{
  ps_sys_network_enum_type                 technology; /**< technology type*/
  ps_sys_rat_ex_enum_type                  rat_value;  /**< rat type*/
  uint64                                   so_mask;   /**< so type*/
} ps_sys_system_status_info_type;

/**
  @brief  This is the type that defines apn to preferred network relationship. 
          For every APN, there would be tuple that describes its preferred 
          system.  
*/
typedef struct 
{
   char                           apn_name[PS_SYS_MAX_APN_LEN]; /**< PDN name*/
   uint32                         num_avail_sys; 
   /**< Number of valid entries to available system status array */
   ps_sys_system_status_info_type avail_sys[PS_SYS_MAX_AVAIL_SYS];  
  /**<  Array of available systems for the APN. The topmost element of the 
        array will convey the current preferred system for the APN.
        @see ps_sys_system_status_ex_type 
        @see ps_sys_system_status_info_type */
   uint8                          recommended_pref_system_index;
  /**< Index in avail_sys array, which contains the 
       recommended preferred system for the APN */
} ps_sys_apn_pref_sys_info_type;

/**
  @brief This is the type that is used to define system status. It is
         common to both configuration PS_SYS_CONF_SYSTEM_STATUS_EX and event
         PS_SYS_EVENT_SYSTEM_STATUS_EX
*/
typedef struct 
{
  uint32   num_avail_sys; 
  /**< Number of valid entries to available system status array */

  ps_sys_system_status_info_type  avail_sys[PS_SYS_MAX_AVAIL_SYS]; 
  /**< Array of current system status,  the topmost element in the array will 
       contain the current preferred system*/

  uint32   num_avail_apns;
  /**< Number of apn entries to pdn_info */

  ps_sys_apn_pref_sys_info_type apn_pref_sys_info[PS_SYS_MAX_APNS];    
  /**< Array of all apn available */

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
} ps_sys_system_status_ex_type;

/**
  @brief Enum type describing APN Preferred system.
  @see   ps_sys_conf_apn_pref_sys_type
*/
typedef enum
{
  PS_SYS_APN_PREF_SYS_WWAN  = DS_SYS_APN_PREF_SYS_WWAN,
  PS_SYS_APN_PREF_SYS_WLAN  = DS_SYS_APN_PREF_SYS_WLAN,
  PS_SYS_APN_PREF_SYS_IWLAN = DS_SYS_APN_PREF_SYS_IWLAN
} ps_sys_apn_pref_sys_enum_type;

/**
  @brief Data type to be used with PS_SYS_CONF_APN_PREF_SYS and 
         PS_SYS_EVENT_APN_PREF_SYS_CHANGE. It describes preferred 
         system for apn.         
*/
typedef struct
{
  unsigned char                  apn[PS_SYS_MAX_APN_LEN];
  ps_sys_apn_pref_sys_enum_type  pref_sys;
} ps_sys_apn_pref_sys_type;

/**
  @brief Data type to hold the apn name
*/
typedef struct
{
  unsigned char     apn_name[PS_SYS_MAX_APN_LEN];
} ps_sys_apn_name_type;

/**   
  @see ds_sys_roaming_disallowed_info_type
*/
typedef struct
{
  boolean                 is_roaming_disallowed;
  uint8                   num_apn_names;
  ps_sys_apn_name_type    apn_list[PS_SYS_MAX_APNS];
} ps_sys_roaming_disallowed_info_type;

/**
  @brief Data type to be used with lte attach pdn list. 
   
  @see  PS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST 
  @see  PS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST 
  @see  PS_SYS_EVENT_3GPP_LTE_ATTACH_PDN_LIST_CHANGE 
*/
typedef enum
{
  PS_SYS_3GPP_ATTACH_PDN_LIST_ACTION_NONE                     = 1,
  PS_SYS_3GPP_ATTACH_PDN_LIST_ACTION_DETACH_OR_PDN_DISCONNECT = 2
}ps_sys_3gpp_attach_pdn_list_action_enum_type;

typedef struct
{
  uint8  attach_pdn_profile_list_num;   
  /**< Num of attach PDN in the array */
  uint16 attach_pdn_profile_list[DS_SYS_LTE_ATTACH_PDN_PROFILE_LIST_MAX]; 
    /**< Array of attach PDN profile IDs*/
  ps_sys_3gpp_attach_pdn_list_action_enum_type attach_action;
    /**< Attach Action */
  void* user_data_ptr;
}ps_sys_3gpp_lte_attach_pdn_list_type;

/**
  @brief Data type to be used with get operation for lte attach pdn list. 
   
  @see  PS_SYS_IOCTL_3GPP_GET_LTE_ATTACH_PDN_LIST_EX 
*/

typedef struct
{
  ps_sys_3gpp_lte_attach_pdn_list_type active_attach_pdn_list;
  ps_sys_3gpp_lte_attach_pdn_list_type pending_attach_pdn_list;
} ps_sys_3gpp_get_lte_attach_pdn_list_ex_type;


/**
  @brief Data type to be used with KAMGR PDN bringup. 
   
  @see  PS_SYS_IOCTL_3GPP_BRING_UP_ALWAYS_ON_PDN 
*/
typedef ds_sys_3gpp_bring_up_always_on_pdn_type ps_sys_3gpp_bring_up_always_on_pdn_type;

/**
  @brief Data type to be used for setting throttling timer
         which starts when a pdn attach request gets rejected
         due to pdn being in throlled state. 
   
  @see  PS_SYS_IOCTL_3GPP_SET_THROTTLED_PDN_REJECT_TIMER
  @see  PS_SYS_IOCTL_3GPP_GET_THROTTLED_PDN_REJECT_TIMER 
*/
typedef ds_sys_3gpp_throttled_pdn_attach_reject_timer_type ps_sys_3gpp_throttled_pdn_attach_reject_timer_type;

/**
  @brief This is the type that is used for 
         PS_SYS_CONF_3GPP_LTE_ATTACH_TYPE configuration and
         PS_SYS_EVENT_3GPP_LTE_ATTACH_TYPE_CHANGED
   
  @see PS_SYS_CONF_3GPP_LTE_ATTACH_TYPE
  @see PS_SYS_EVENT_3GPP_LTE_ATTACH_TYPE_CHANGED 
*/
typedef enum
{
  PS_SYS_LTE_ATTACH_TYPE_INITIAL  = 0x00,
  PS_SYS_LTE_ATTACH_TYPE_HANDOFF  = 0x01
} ps_sys_lte_attach_enum_type;

/**
  @brief system status event payload for DIAG events
         EVENT_PS_SYSTEM_STATUS
         EVENT_PS_SYSTEM_STATUS_EX
         EVENT_QMI_SYSTEM_STATUS
         EVENT_QMI_SYSTEM_STATUS_EX
  */
typedef struct 
{
  ps_sys_network_enum_type       pref_network; 
  uint32                         rat_value;  
  uint64                         so_mask;
  ps_sys_subscription_enum_type  subs_id;   
} ps_sys_system_status_diag_event_logging_type;

/**
  @brief Traffic state enum type
  Note : Donot change the existing values of below enum type. 
         START : 1 (TRUE) and STOP : 0 (FALSE)
  */

/**
  @brief Event is triggered to notify low latency traffic start and end
  @see  DS_SYS_EVENT_LOW_LATENCY_TRAFFIC_STATUS
*/
typedef enum
{
  PS_SYS_LOW_LATENCY_TRAFFIC_STATE_STOP     = DS_SYS_LOW_LATENCY_TRAFFIC_STATE_STOP,
  PS_SYS_LOW_LATENCY_TRAFFIC_STATE_START    = DS_SYS_LOW_LATENCY_TRAFFIC_STATE_START,
  PS_SYS_LOW_LATENCY_TRAFFIC_STATE_CONTINUE = DS_SYS_LOW_LATENCY_TRAFFIC_STATE_CONTINUE,
  PS_SYS_LOW_LATENCY_TRAFFIC_STATE_MAX = DS_SYS_LOW_LATENCY_TRAFFIC_STATE_MAX
} ps_sys_low_latency_traffic_state_enum_type;
  
typedef struct
{
  /*! Traffic State */  
  ps_sys_low_latency_traffic_state_enum_type   traffic_state;
  
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
} ps_sys_low_latency_traffic_info_type;

/**
  @brief This is the type that is used to define the policy 
         being refreshed
*/
typedef enum
{
  PS_SYS_REFRESH_POLICY_ANDSF  = 0x00,
  PS_SYS_REFRESH_POLICY_MAX    = PS_SYS_REFRESH_POLICY_ANDSF
} ps_sys_refresh_policy_enum_type;

/**
  @brief This is the type that is used to define the result of 
         the refresh
*/
typedef enum
{
  PS_SYS_POLICY_REFRESH_RESULT_SUCCESS = 0x00,
  /**< Success */
  PS_SYS_POLICY_REFRESH_RESULT_HTTP_FAILURE = 0x01,
  /**< HTTP Failure */
  PS_SYS_POLICY_REFRESH_RESULT_NW_BRINGUP_FAILURE = 0x02,
  /**< Network bringup failure */
  PS_SYS_POLICY_REFRESH_RESULT_START_FTP_SERVER_FAILURE = 0x03,
  /**< Start FTP Server Failure */
  PS_SYS_POLICY_REFRESH_RESULT_LL_IFACE_DOWN_FAILURE = 0x04,
  /**< Link Local IFACE Down */
  PS_SYS_POLICY_REFRESH_RESULT_OTHER_FAILURE = 0x05
  /**< Other Unknown Error */
} ps_sys_policy_refresh_status_enum_type;

/**
  @brief This is the type that is used for 
         PS_SYS_IOCTL_3GPP_ANDSF_POLICY_REFRESH
   
  @see PS_SYS_IOCTL_3GPP_ANDSF_POLICY_REFRESH
*/

typedef struct 
{
  void*                         user_data_ptr;  
} ps_sys_policy_refresh_info_type;

/**
  @brief This is the type that is used for 
         PS_SYS_EVENT_3GPP_POLICY_REFRESH_STATUS
   
  @see PS_SYS_EVENT_3GPP_POLICY_REFRESH_STATUS
*/

typedef struct 
{
  ps_sys_refresh_policy_enum_type        policy_type; 
  ps_sys_policy_refresh_status_enum_type status;
  void*                                  user_data_ptr;  
} ps_sys_policy_refresh_status_type;

/**
  @brief This is the type that is used for 
         PS_SYS_EVENT_3GPP_POLICY_AVAILABLE
   
  @see PS_SYS_EVENT_3GPP_POLICY_AVAILABLE
*/

typedef struct 
{
  ps_sys_refresh_policy_enum_type      policy_type; 
  ps_ip_addr_type                      ftp_server_ipv6_addr;
  uint16                               tcp_port;
  char                                 policy_file_name[PS_SYS_MAX_FILE_NAME_LEN]; 
} ps_sys_policy_available_info_type;

/**
  @brief Throughput information for all active PDNs  
  @see  PS_SYS_CONF_THROUGHPUT_INFO 
  @see  PS_SYS_EVENT_THROUGHPUT_INFO 
*/

typedef ds_sys_ip_addr_enum_type                ps_sys_ip_addr_enum_type;
typedef ds_sys_throughput_quality_enum_type     ps_sys_throughput_quality_enum_type;
typedef ds_sys_throughput_info_list_type        ps_sys_throughput_info_list_type;
typedef ds_sys_throughput_info_type             ps_sys_throughput_info_type; 
typedef ds_sys_bearer_throughput_info_list_type ps_sys_bearer_throughput_info_list_type;
typedef ds_sys_bearer_throughput_info_type      ps_sys_bearer_throughput_info_type;

/**
  @brief Data type to be used with PS_SYS_CONF_3GPP_EMERGENCY_PDN_STATUS and 
         PS_SYS_EVENT_3GPP_EMERGENCY_PDN_STATUS. 
  @see   ds_sys_is_pdn_only_emergency_type
*/
typedef ds_sys_is_pdn_only_emergency_type ps_sys_is_pdn_only_emergency_type;

/**
  @brief Data type to be used with PS_SYS_CONF_NULL_RESOLVED_APN
         and PS_SYS_EVENT_NULL_RESOLVED_APN
  apn_length       : Length of the apn name
  apn_name         : Apn name 
  */
typedef struct 
{
  uint8         apn_length;  
  char          apn_name[PS_SYS_MAX_APN_LEN];
} ps_sys_null_resolved_apn_info_type;

/**
  @brief Data type to be used with network aware status
   
  @see   PS_SYS_CONF_3GPP2_QOS_NETWORK_STATUS
  @see   PS_SYS_EVENT_3GPP2_QOS_NETWORK_STATUS_CHANGE  
*/
typedef ds_sys_3gpp2_network_qos_aware_status ps_sys_3gpp2_network_qos_aware_status;

/**
  @brief Data type to be used with page monitor period
   
  @see   PS_SYS_IOCTL_3GPP2_GET_PAGE_MONITOR_PERIOD
  @see   PS_SYS_EVENT_3GPP2_PAGE_MONITOR_PERIOD_CHANGE
 */
typedef ds_sys_3gpp2_page_monitor_period_info_type ps_sys_3gpp2_page_monitor_period_info_type;

/**
  @brief Data type to be used with downlink throughput info 
         timer
   
  @see   PS_SYS_CONF_DL_THROUGHPUT_INFO_TIMER
  @see   PS_SYS_EVENT_UPDATED_DL_THROUGHPUT_TIMER
*/
typedef ds_sys_report_dl_timer_type ps_sys_report_dl_timer_type;

/**
  @brief Data type to be used with downlink throughput info 
   
  @see   PS_SYS_CONF_DL_THROUGHPUT_INFO
  @see   PS_SYS_EVENT_DL_THROUGHPUT_INFO
*/
typedef ds_sys_dl_throughput_info_type ps_sys_dl_throughput_info_type;

/**
  @brief Data type to be used with downlink throughput info 
         status change
   
  @see   DS_SYS_CONF_DL_THROUGHPUT_INTERVAL_STATUS_CHANGE
  @see   DS_SYS_EVENT_UPDATED_DL_THROUGHPUT_TIMER
*/

typedef enum
{
  PS_SYS_THRPT_REPORT_ENABLED = DS_SYS_THRPT_REPORT_ENABLED,
  /**< Throughput reporting enabled \n */
  PS_SYS_THRPT_REPORT_DISABLED_NO_DATA_CALL = DS_SYS_THRPT_REPORT_DISABLED_NO_DATA_CALL,
  /**< No data call \n */
  PS_SYS_THRPT_REPORT_DISABLED_ALL_CALLS_DORMANT = DS_SYS_THRPT_REPORT_DISABLED_ALL_CALLS_DORMANT,
  /**< All calls dormant \n */
  PS_SYS_THRPT_REPORT_DISABLED_UNSUPPORTED_RAT = DS_SYS_THRPT_REPORT_DISABLED_UNSUPPORTED_RAT,
  /**< Unsupported RAT \n */
  PS_SYS_THRPT_REPORT_DISABLED_BY_CLIENT = DS_SYS_THRPT_REPORT_DISABLED_BY_CLIENT
  /**< Report disabled by client \n */
} ps_sys_thrpt_status_reason_enum_type;

typedef struct
{
  ps_sys_thrpt_status_reason_enum_type   thrpt_status;
  uint32                                 actual_interval;
} ps_sys_thrpt_status_type;

/**
  @brief Data type to be used with thermal level mitigation info
   
  @see   PS_SYS_CONF_THERMAL_MITIGATION_INFO
  @see   PS_SYS_EVENT_THERMAL_MITIGATION_INFO
 */
typedef enum
{
  PS_SYS_THERMAL_MITIGATION_ALL_CALLS_ALLOWED = DS_SYS_THERMAL_MITIGATION_ALL_CALLS_ALLOWED,
  PS_SYS_THERMAL_MITIGATION_IMS_CALLS_ONLY    = DS_SYS_THERMAL_MITIGATION_IMS_CALLS_ONLY,
  PS_SYS_THERMAL_MITIGATION_NO_CALLS_ALLOWED  = DS_SYS_THERMAL_MITIGATION_NO_CALLS_ALLOWED
} ps_sys_thermal_mitigation_action_enum_type;

/*---------------------------------------------------------------------------
  APN type enum. 
  PS_SYS_APN_TYPE_UNSPECIFIED is used if APN type has not configured.
---------------------------------------------------------------------------*/
#define PS_SYS_APN_TYPE_UNSPECIFIED DS_SYS_APN_TYPE_UNSPECIFIED

typedef enum
{
  PS_SYS_APN_TYPE_INTERNET     = DS_SYS_APN_TYPE_INTERNET,
  PS_SYS_APN_TYPE_IMS          = DS_SYS_APN_TYPE_IMS,   
  PS_SYS_APN_TYPE_MMS          = DS_SYS_APN_TYPE_MMS, 
  PS_SYS_APN_TYPE_DUN          = DS_SYS_APN_TYPE_DUN, 
  PS_SYS_APN_TYPE_SUPL         = DS_SYS_APN_TYPE_SUPL, 
  PS_SYS_APN_TYPE_HIPRI        = DS_SYS_APN_TYPE_HIPRI, 
  PS_SYS_APN_TYPE_FOTA         = DS_SYS_APN_TYPE_FOTA,
  PS_SYS_APN_TYPE_CBS          = DS_SYS_APN_TYPE_CBS, 
  PS_SYS_APN_TYPE_IA           = DS_SYS_APN_TYPE_IA, 
  PS_SYS_APN_TYPE_EMERGENCY    = DS_SYS_APN_TYPE_EMERGENCY,
  PS_SYS_APN_TYPE_MAX          = DS_SYS_APN_TYPE_MAX
} ps_sys_apn_enum_type;

/**
  @brief Data type to be used with data service
   
  @see   PS_SYS_EVENT_DATA_SERVICE_SETTING
  @see   PS_SYS_CONF_DATA_SERVICE_SETTING 
 */
typedef ds_sys_data_services_setting_type ps_sys_data_services_setting_type;

/**
  @brief Data type to be used with  data service roaming
   
  @see   PS_SYS_EVENT_DATA_SERVICE_ROAMING_SETTING
  @see   PS_SYS_CONF_DATA_SERVICE_ROAMING_SETTING
 */
typedef ds_sys_data_services_roaming_setting_type ps_sys_data_services_roaming_setting_type;

/**
  @brief Data type to store the apn info 
  @see   PS_SYS_EVENT_APN_INFO 
  @see   PS_SYS_CONF_APN_INFO 
*/
typedef ds_sys_apn_params_type ps_sys_apn_params_type;

typedef ds_sys_apn_info_type ps_sys_apn_info_type;

/**
  @brief Data type to store the ul/dl thrpt frequency request 
  @see  PS_SYS_EVENT_CONFIGURED_UL_DL_THROUGHPUT_INFO 
*/
typedef struct
{
  uint32  ul_configured_throughput; //in kbps
  uint32  dl_configured_throughput; //in kbps
}ps_sys_configured_throughput_info_type;


/**
  @brief Data type to store the timer for uplink/downlink info
  @see     PS_SYS_CONF_UL_DL_THROUGHPUT_INFO 
  @see     PS_SYS_EVENT_UL_DL_THROUGHPUT_INFO 
*/
typedef struct
{
  uint32  ul_dl_thrpt_freq; 
}ps_sys_ul_dl_throughput_freq_type;

/**
  @brief Data type to store the ul/dl thrpt information
  @see     PS_SYS_CONF_UL_DL_THROUGHPUT_INFO 
  @see     PS_SYS_EVENT_UL_DL_THROUGHPUT_INFO 
*/
typedef struct
{
  uint32  ul_achievable_throughput; //in kbps
  uint32  dl_achievable_throughput; //in kbps
  uint32  ul_actual_throughput; //in kbps
  uint32  dl_actual_throughput; //in kbps
}ps_sys_ul_dl_throughput_info_type;

typedef enum
{
  PS_SYS_RAT_PREFERENCE_INACTIVE           = DS_SYS_RAT_PREFERENCE_INACTIVE,
  PS_SYS_RAT_PREFERENCE_CELLULAR_ONLY      = DS_SYS_RAT_PREFERENCE_CELLULAR_ONLY,
  PS_SYS_RAT_PREFERENCE_WIFI_ONLY          = DS_SYS_RAT_PREFERENCE_WIFI_ONLY,
  PS_SYS_RAT_PREFERENCE_CELLULAR_PREFERRED = DS_SYS_RAT_PREFERENCE_CELLULAR_PREFERRED,
  PS_SYS_RAT_PREFERENCE_WIFI_PREFERRED     = DS_SYS_RAT_PREFERENCE_WIFI_PREFERRED,
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  PS_SYS_RAT_PREFERENCE_IMS_PREFERRED      = DS_SYS_RAT_PREFERENCE_IMS_PREFERRED,
#endif
  PS_SYS_RAT_PREFERENCE_MAX                = DS_SYS_RAT_PREFERENCE_MAX
} ps_sys_rat_preference_enum_type;

/**
  @brief Modem power stats config type

  @see PS_SYS_CONF_MODEM_POWER_STATS_CONFIGURATION 
  @see PS_SYS_EVENT_MODEM_POWER_STATS_CONFIGURATION 
*/
typedef struct
{
  boolean   enable_power_statistics;
} ps_sys_modem_power_stats_config_type;

//Enumeration for tx mode levels
typedef enum {
  PS_SYS_MODEM_POWER_LEVEL_ONE,
  PS_SYS_MODEM_POWER_LEVEL_TWO,
  PS_SYS_MODEM_POWER_LEVEL_THREE,
  PS_SYS_MODEM_POWER_LEVEL_FOUR,
  PS_SYS_MODEM_POWER_LEVEL_FIVE,
  PS_SYS_MODEM_POWER_LEVEL_MAX
} ps_sys_modem_power_level_enum_type;

typedef struct {
  ps_sys_modem_power_level_enum_type  tx_mode_level;
  /**< Power levels in Tx(Transmitter) mode */
   
  uint32  tx_mode_duration;
  /**< Duration that modem was active on 
       Tx mode(Transmitter mode) in milliseconds 
  */
} ps_sys_tx_mode_type;

/* Enumeration for modem RAT */
typedef enum
{
  /* 1X technology definition */
  PS_SYS_MODEM_POWER_1X_TECH,
  /* DO technology definition */
  PS_SYS_MODEM_POWER_DO_TECH,
  /* WCDMA technology definition for SUB1 */
  PS_SYS_MODEM_POWER_WCDMA1_TECH,
  /* WCDMA technology definition for SUB2 */
  PS_SYS_MODEM_POWER_WCDMA2_TECH,
  /* LTE technology definition for SUB1*/
  PS_SYS_MODEM_POWER_LTE1_TECH,
  /* LTE technology definition for SUB2*/
  PS_SYS_MODEM_POWER_LTE2_TECH,
  /* TDSCDMA technology definition */
  PS_SYS_MODEM_POWER_TDSCDMA_TECH,
  /* GERAN technology definition for SUB1 */
  PS_SYS_MODEM_POWER_GERAN1_TECH,
  /* GERAN technology definition for SUB2 */
  PS_SYS_MODEM_POWER_GERAN2_TECH,
  PS_SYS_MODEM_POWER_MAX_RAT_TECH
} ps_sys_modem_power_tech_enum_type;

typedef struct {
  ps_sys_modem_power_tech_enum_type  modem_power_tech;
  /**< Modem power RAT or Technology */
   
  uint32  modem_tech_active_time;
  /**< Duration that the modem technology was active since last query
       (in milliseconds). 
  */
} ps_sys_modem_power_tech_active_time_type;


/**
  @brief Modem power stats config type

  @see PS_SYS_CONF_MODEM_POWER_STATISTICS 
  @see PS_SYS_EVENT_MODEM_POWER_STATISTICS 
*/
typedef struct
{
  uint32 idle_mode_duration;
  /**< The duration that modem was awake but RRC connection 
       was released in milliseconds
  */

  uint32 sleep_mode_duration;
  /**< Duration that modem was in power collapse in milliseconds
  */

  uint32 rx_mode_duration;
  /**< Duration that modem was active on Rx mode( Receiver mode) 
       in milliseconds
  */

  ps_sys_tx_mode_type tx_mode_duration[PS_SYS_MODEM_POWER_LEVEL_MAX];
  /**< Duration that modem was active on Tx mode(Transmitter mode)
       in milliseconds 
  */

  ps_sys_modem_power_tech_active_time_type
              modem_tech_active_time[PS_SYS_MODEM_POWER_MAX_RAT_TECH];
  /**< Duration that modem was active in msec since last query
  */

  uint32 wlan_active_time;
  /**< Duration that WLAN was active in msec since last query
  */

  uint32 gps_active_time;
  /**< Duration that GPS was active in msec since last query
  */

  boolean modem_active_vote_status;
  /**< This boolean gives active status of modem RAT/TECH based on NPA vote.
       TRUE means that modem RAT/TECH is active as per vote.
  */
  boolean wlan_active_vote_status;
  /**< This boolean gives active status of WLAN based on NPA vote.
       TRUE means that WLAN is active as per vote.
  */
  boolean gps_active_vote_status;
  /**< This boolean gives active status of GPS based on NPA vote.
       TRUE means that GPS is active as per vote.
  */
} ps_sys_modem_power_stats_info_type;

typedef enum
{
    PS_SYS_REMOTE_SOCKET_HANDLING_V0 = 0x00, 
    /**<  Only ephemeral port allocation support  */
    PS_SYS_REMOTE_SOCKET_HANDLING_V1 = 0x01 
    /**<  Addition preference TLV is supported  .  */
}ps_sys_remote_socket_service_ver;

/**
  @brief Data type that provides whether remote socket handling 
  is available on the TE or not. 
  remote_socket_service_avail = TRUE : TE has registered for remote 
  socket socket handling indications 
  remote_socket_service_avail = FALSE : TE has not registered for 
  remote socket socket handling indications 
  @see PS_SYS_CONF_REMOTE_SOCKET_SERVICE_AVAIL 
  @see PS_SYS_EVENT_REMOTE_SOCKET_SERVICE_AVAIL
*/
typedef struct
{
  boolean                              remote_socket_service_avail;
  ps_sys_remote_socket_service_ver     remote_service_version;
} ps_sys_remote_socket_service_avail_type;

typedef struct
{
  ip_version_enum_type      ip_family;   //v4 or v6
  ps_ip_protocol_enum_type  xport_proto; //will only contain TCP or UDP
  uint16                    port_no;     //0 or specific port number
}ps_sys_remote_socket_type;

/**
  @brief Data type that contains parameters for remote socket 
         allocation request
  @see     PS_SYS_EVENT_REMOTE_SOCKET_REQUEST 
*/
typedef struct
{
  uint8                      num_sockets; //num of sockets in list
  ps_sys_remote_socket_type  socket_list[PS_SYS_MAX_REMOTE_SOCKETS]; 
}ps_sys_remote_socket_list_type;

typedef struct
{
  uint32                          request_handle;
  uint32                          port_property;
  uint16                          base_port_range;
  uint16                          max_port_range;
  ps_sys_remote_socket_list_type  sockets_list;
}ps_sys_remote_socket_request_type;

typedef enum
{
  PS_SYS_REMOTE_SOCKET_ALLOC_SUCCESS,
  PS_SYS_REMOTE_SOCKET_ALLOC_IN_USE_FAILURE,
  PS_SYS_REMOTE_SOCKET_ALLOC_GENERAL_FAILURE
} ps_sys_remote_socket_alloc_status;

typedef struct
{
  ps_sys_remote_socket_alloc_status  status;   //SUCCESS or FAILURE
  uint32                             remote_socket_handle; //handle to socket on the TE
  boolean                            is_ephemeral_port; //whether original request was for ephemeral port
  ps_sys_remote_socket_type          socket_info; //Info related to socket
}ps_sys_remote_socket_allocated_type;

/**
  @brief Data type that contains parameters for remote socket 
         allocation information from the TE
  @see     PS_SYS_IOCTL_REMOTE_SOCKET_ALLOCATED 
*/
typedef struct
{
  uint32                               request_handle; // handle sent in sockets request
  uint8                                num_sockets; //num of sockets in list
  ps_sys_remote_socket_allocated_type  socket_list[PS_SYS_MAX_REMOTE_SOCKETS]; 
}ps_sys_remote_socket_allocated_list_type;

/**
  @brief Data type that contains parameters for remote socket 
         release
  @see     PS_SYS_EVENT_REMOTE_SOCKET_RELEASE 
*/
typedef struct
{
  uint8   num_sockets; //num of sockets in list
  uint32  socket_list[PS_SYS_MAX_REMOTE_SOCKETS]; //List of handles to be released
}ps_sys_remote_socket_release_list_type;

/**
  @brief Data type that contains parameters for setting options 
         on a remote socket
  @see     PS_SYS_EVENT_REMOTE_SOCKET_SET_OPTION 
*/
typedef struct
{
  uint32  socket_handle; //Handle to socket on the TE
  boolean is_udp_encaps; // Is UDP encapsulated or not
}ps_sys_remote_socket_set_option_type;

/**
  @brief Data type to be used with 
         PS_SYS_IOCTL_SET_WQE_PROFILE_QUALITY_MEAS_INFO
  @see   PS_SYS_IOCTL_SET_WQE_PROFILE_QUALITY_MEAS_INFO 
*/
typedef ds_sys_set_wqe_profile_quality_meas_info_list_type 
  ps_sys_set_wqe_profile_quality_meas_info_list_type;



/**
   @brief Data type to be used with uplink throughput config
   
  @see   PS_SYS_CONF_UL_THROUGHPUT_CONFIG
  @see   PS_SYS_EVENT_UPDATED_UL_THROUGHPUT_CONFIG
*/
typedef ds_sys_report_ul_throughput_config_type ps_sys_report_ul_throughput_config_type;


/**
  @brief Data type to be used with downlink throughput info 
   
  @see   PS_SYS_CONF_UL_THROUGHPUT_INFO
  @see   PS_SYS_EVENT_UL_THROUGHPUT_INFO
*/
typedef ds_sys_ul_throughput_info_type ps_sys_ul_throughput_info_type;

/**
  @brief Data type to be used with wifi switch settings
   
  @see   PS_SYS_CONF_DATA_SERVICE_WIFI_SETTING 
  @see   PS_SYS_EVENT_DATA_SERVICE_WIFI_SETTING
 */
typedef ds_sys_data_services_wifi_setting_type ps_sys_data_services_wifi_setting_type;

/**
  @brief Data type to be used with ehrpd fallback apn list. 
   
  @see   PS_SYS_IOCTL_3GPP2_SET_EHRPD_FALLBACK_APN_LIST 
  @see   PS_SYS_IOCTL_3GPP2_GET_EHRPD_FALLBACK_APN_LIST  
*/
typedef ds_sys_3gpp2_ehrpd_fallback_apn_list_type ps_sys_3gpp2_ehrpd_fallback_apn_list_type;

/**
  @brief Defines enum type representing different IP types
*/
typedef enum
{
  PS_SYS_IP_TYPE_IPV4     = 0,   /**< IPV4 type */
  PS_SYS_IP_TYPE_IPV6     = 1,   /**< IPV6 type */
  PS_SYS_IP_TYPE_IPV4IPV6 = 2,   /**< IPV4V6 type */
  PS_SYS_IP_TYPE_MAX
} ps_sys_ip_type_enum_type;

/**
  @brief Event info type for PS_SYS_EVENT_3GPP_LTE_ATTACH_PARAMS_EX event. 
  @see  PS_SYS_EVENT_3GPP_LTE_ATTACH_PARAMS_EX
  @see  PS_SYS_IOCTL_3GPP_LTE_ATTACH_PARAMS_EX
*/
typedef struct
{
  boolean                   is_valid; /**< If attach params are valid */
  ps_sys_ip_type_enum_type  ip_type;  /**< IP address type */
  char                      apn_string[PS_SYS_MAX_APN_LEN]; /**< Attached PDN name */
  uint8                     apn_string_len; /**<  Length of attached PDN name */
  uint32                    ipv4_addr;         /**< IPv4 address */
  uint32                    ipv4_gateway_addr; /**< IPv4 gateway address */
  uint32                    ipv4_subnet_mask;  /**< IPv4 subnet mask */
  uint64                    ipv6_addr_prefix;  /**< IPv6 address prefix */
  uint64                    ipv6_addr_iid;     /**< IPv6 address iid */
  uint64                    ipv6_gateway_prefix; /**< IPv6 gateway prefix */
  uint64                    ipv6_gateway_iid;  /**< IPv6 gateway iid */
} ps_sys_lte_attach_params_type;

typedef enum
{
  PS_SYS_OP_RESERVED_PCO_ID1 = DS_SYS_OP_RESERVED_PCO_ID1,
  PS_SYS_OP_RESERVED_PCO_ID2 = DS_SYS_OP_RESERVED_PCO_ID2,
  PS_SYS_OP_RESERVED_PCO_ID3 = DS_SYS_OP_RESERVED_PCO_ID3,
  PS_SYS_OP_RESERVED_PCO_ID4 = DS_SYS_OP_RESERVED_PCO_ID4,
  PS_SYS_OP_RESERVED_PCO_ID5 = DS_SYS_OP_RESERVED_PCO_ID5,
  PS_SYS_OP_RESERVED_PCO_ID6 = DS_SYS_OP_RESERVED_PCO_ID6,
  PS_SYS_OP_RESERVED_PCO_ID7 = DS_SYS_OP_RESERVED_PCO_ID7,
  PS_SYS_OP_RESERVED_PCO_ID8 = DS_SYS_OP_RESERVED_PCO_ID8,
  PS_SYS_OP_RESERVED_PCO_ID9 = DS_SYS_OP_RESERVED_PCO_ID9,
  PS_SYS_OP_RESERVED_PCO_ID10 = DS_SYS_OP_RESERVED_PCO_ID10
} ps_sys_op_reserved_pco_id_enum_type;

typedef struct
{
  uint16  mcc;
  uint16  mnc;
  boolean mnc_includes_pcs_digit;
  uint8 app_specific_info_len;
  uint8  app_specific_info[PS_SYS_MAX_OP_RES_PCO_APP_SPECIFIC_LEN];
  uint16 container_id;
  ps_sys_op_reserved_pco_id_enum_type   pco_id;
} ps_sys_op_reserved_pco_type;

/**
  @brief Data struture associated with DS_SYS_CONF_GET_APN_PCO_INFO and 
         DS_SYS_EVENT_APN_PCO_INFO_CHANGE. 
  @see  DS_SYS_CONF_GET_APN_PCO_INFO
  @see  DS_SYS_EVENT_APN_PCO_INFO_CHANGE
*/
typedef struct
{
  int                           num_pcos;
  ps_sys_op_reserved_pco_type   pco_info[PS_SYS_MAX_NUM_PCO];
} ps_sys_op_reserved_pco_list_type;


/**
  @brief Data struture associated with PS_SYS_CONF_GET_APN_PCO_INFO and 
         PS_SYS_EVENT_APN_PCO_INFO_CHANGE. 
  @see  PS_SYS_CONF_GET_APN_PCO_INFO
  @see  PS_SYS_EVENT_APN_PCO_INFO_CHANGE
*/
typedef struct
{
  char                                apn_name[PS_SYS_MAX_APN_LEN];
  ps_sys_op_reserved_pco_list_type    apn_pco_info;
} ps_sys_pco_info_per_apn_type;

typedef struct
{
  uint8                               msisdn_length;
  uint8                               msisdn[255];
} ps_sys_msisdn_info_type;

/**
  @brief Data struture associated with PS_SYS_IOCTL_GET_APN_MSISDN_INFO and 
         PS_SYS_EVENT_APN_MSISDN_CHANGE. 
  @see  PS_SYS_IOCTL_GET_APN_MSISDN_INFO
  @see  PS_SYS_EVENT_APN_MSISDN_CHANGE
*/
typedef struct
{
  char                                apn_name[DS_SYS_MAX_APN_LEN];
  ps_sys_msisdn_info_type             msisdn_info;
} ps_sys_msisdn_info_per_apn_type;

/**
  @brief Data type to be used with  
         PS_SYS_EVENT_3GPP_WWAN_CONN_STATE
         PS_SYS_IOCTL_3GPP_GET_WWAN_CONN_STATE 
*/

typedef enum 
{
  PS_SYS_WWAN_IDLE_STATE = 0x0000, 
  /**<  WWAN is in idle state.
        Start uplink data monitor on the TE. */
  PS_SYS_WWAN_CONNECTED_STATE = 0x0001, 
  /**<  WWAN is in idle state.
       Stop uplink data monitor on the TE. */
  PS_SYS_WWAN_INVALID_STATE = 0x00FF
  /**< WWAN is in invalid state. */
} ps_sys_wwan_conn_state_enum_type;

typedef struct
{
  ps_sys_wwan_conn_state_enum_type conn_state;
  /**< Connection state enum*/
} ps_sys_wwan_conn_state_type; 


typedef enum
{ 
  /*! 1.4MHz bandwidth */
  PS_SYS_LTE_BW_6 = DS_SYS_LTE_BW_6,
  /*! 3MHz bandwidth */
  PS_SYS_LTE_BW_15 = DS_SYS_LTE_BW_15,
  /*! 5MHz bandwidth */
  PS_SYS_LTE_BW_25 = DS_SYS_LTE_BW_25,
  /*! 10MHz bandwidth */
  PS_SYS_LTE_BW_50 = DS_SYS_LTE_BW_50,
  /*! 15MHz bandwidth */
  PS_SYS_LTE_BW_75 = DS_SYS_LTE_BW_75,
  /*! 20MHz bandwidth */
  PS_SYS_LTE_BW_100 = DS_SYS_LTE_BW_100,
} ps_sys_lte_cell_bandwidth_enum_type;


typedef enum
{
  PS_SYS_LTE_SCELL_STATE_DECONFIGURED     = 
    DS_SYS_LTE_SCELL_STATE_DECONFIGURED,
  PS_SYS_LTE_SCELL_CONFIGURED_DEACTIVATED = 
    DS_SYS_LTE_SCELL_CONFIGURED_DEACTIVATED,
  PS_SYS_LTE_SCELL_CONFIGURED_ACTIVATED   = 
    DS_SYS_LTE_SCELL_CONFIGURED_ACTIVATED
} ps_sys_lte_scell_state_enum_type;


typedef struct
{
  uint8                                 serving_cell_id;
  /** Serving Cell id assigned to cell*/
  uint16                                pci;
  /** Physical cell ID */
  uint32                                freq;
  /** Cell's frequency Range */
  ps_sys_lte_cell_bandwidth_enum_type   dl_bandwidth;
  /** Cell's Downlink Bandwidth  */
  uint32                                band_info;
  /** Cell's band  */
} ps_sys_lte_cell_type;


typedef struct
{
  ps_sys_lte_cell_type                  cell_info;
  /** Cell's info  */
  ps_sys_lte_scell_state_enum_type      scell_state;
  /* State of secondary cell */
  boolean                               ul_enabled;
  /** Uplink is enabled for cell */
} ps_sys_lte_scell_type;


/**
  @brief Data type to be used with PS_SYS_CONF_LTE_CELL_INFO and 
         PS_SYS_EVENT_LTE_CELL_INFO_CHANGE.
*/
typedef struct
{     
  /*Primary cell info*/
  ps_sys_lte_cell_type    pcell_info;
  
  uint8                   num_scell;

  /*Secondary cell info*/
  ps_sys_lte_scell_type   scell_info[PS_SYS_MAX_LTE_NUM_SCELL];

} ps_sys_lte_cell_info_type;

typedef struct
{
  char mcc[PS_SYS_MCC_MNC_SIZE];
  char mnc[PS_SYS_MCC_MNC_SIZE];
} ps_sys_3gpp_mcc_mnc_type;

typedef struct
{
  uint8  hplmn_list_num;
  /** Number of hplmn list  */
  ps_sys_3gpp_mcc_mnc_type hplmn_list[PS_SYS_HPLMN_LIST_MAX]; 
  /** HPLMN list  */
  uint8  excp_hplmn_list_num;
  /** Number of exception hplmn list  */
  ps_sys_3gpp_mcc_mnc_type excp_hplmn_list[PS_SYS_EXCP_HPLMN_LIST_MAX]; 
  /** Exception hplmn list  */
  uint8  domestic_rplmn_list_num;
  /** Number of domestic roaming plmn list */
  ps_sys_3gpp_mcc_mnc_type domestic_rplmn_list[PS_SYS_DOMESTIC_RPLMN_LIST_MAX];
  /** Domestic Roaming PLMN list */
  
  uint8  international_rplmn_list_num;
  /** Number of International roaming plmn list */
  ps_sys_3gpp_mcc_mnc_type international_rplmn_list[PS_SYS_INTERNATIONAL_RPLMN_LIST_MAX];
  /** International Roaming PLMN list */  
} ps_sys_3gpp_plmn_list_type;

/**
  @brief Data type to be used with PS_SYS_EVENT_CURRENT_DDS_IND
*/
typedef enum 
{
  PS_SYS_DDS_SWITCH_PERMANENT = 0x00,
  /**< Permanent(Default) */
  PS_SYS_DDS_SWITCH_TEMPORARY = 0x01
  /**< Temporary */
} ps_sys_dds_switch_type_enum;

typedef struct
{
  ps_sys_subscription_enum_type    dds;
  ps_sys_dds_switch_type_enum      dds_switch_type;
} ps_sys_current_dds_type;

typedef enum 
{
  PS_SYS_POLICYMAN_DEV_CURR_CAPABILITY, 
  /**< Device capability is updated by policyman */
  PS_SYS_POLICYMAN_MAX 
} ps_sys_policyman_msg_enum_type;

/**
  @brief Data type to be used with PS_SYS_EVENT_3GPP_POLICYMAN_MSG_IND
*/
typedef struct
{
  ps_sys_policyman_msg_enum_type  policyman_msg;
  /** Policyman message type  */
} ps_sys_policyman_msg_info_type;


//Enumeration for RAB Reject Cause
typedef enum
{
  PS_SYS_3GPP_RAB_REJECT_UNKNOWN  = 0,
  /**< RAB Reject Cause is unknown */ 
  PS_SYS_3GPP_RAB_REJECT_ILLEGAL_MS = 1, 
  /**< Network refuses service to the MS because either an identity of the MS 
      is not acceptable to the network or the MS does not pass the 
      authentication check*/
  PS_SYS_3GPP_RAB_REJECT_ILLEGAL_ME  = 2, 
  /**< ME could not be authenticated and the ME used is not acceptable to 
       the network*/
  PS_SYS_3GPP_RAB_REJECT_EPS_SERVICES_NOT_ALLOWED = 3, 
  /**< EPS and non-EPS services are not allowed by the network */
  PS_SYS_3GPP_RAB_REJECT_MS_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK = 4,
  /**< No matching identity or context could be found in the network */
  PS_SYS_3GPP_RAB_REJECT_IMPLICITLY_DETACHED = 5, 
  /**< Mobile reachable timer has expired, or the GMM context data related to the
       subscription does not exist in the SGSN*/
  PS_SYS_3GPP_RAB_REJECT_PLMN_NOT_ALLOWED  = 6, 
  /**< UE requests GPRS service, or the network initiates a detach request in a 
      PLMN which does not offer roaming for GPRS services to that MS*/
  PS_SYS_3GPP_RAB_REJECT_TRACKING_AREA_NOT_ALLOWED = 7, 
  /**< Tracking Area is not allowed */
  PS_SYS_3GPP_RAB_REJECT_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA = 8,
  /**< Roaming is not allowed in the current tracking area */
  PS_SYS_3GPP_RAB_REJECT_NO_SUITABLE_CELLS_IN_TRACKING_AREA = 9, 
  /**< No suitable cells in the tracking area */
  PS_SYS_3GPP_RAB_REJECT_CONGESTION  = 10, 
  /**< Network cannot serve the request from MS because of congestion*/
  PS_SYS_3GPP_RAB_REJECT_NOT_AUTHORIZED_FOR_THIS_CSG = 11, 
  /**< Unauthorized Request */
  PS_SYS_3GPP_RAB_REJECT_NO_EPS_BEARER_CONTEXT_ACTIVATED = 12,
  /**< No EPS bearer context was activated */
  PS_SYS_3GPP_RAB_REJECT_EMM_ACCESS_BARRED = 13, 
  /**< Connection establishment failed due to Lower layer RRC connection Failure*/
  PS_SYS_3GPP_RAB_REJECT_LRRC_CONN_EST_FAILURE_CONN_REJECT  = 14, 
  /**< Connection establishment failed due to NW rejecting UE connection request*/
  PS_SYS_3GPP_RAB_REJECT_EMM_T3417_EXPIRED = 15, 
  /**< T3417 expires for service request procedure */
  PS_SYS_3GPP_RAB_REJECT_EMM_T3417_EXT_EXPIRED  = 16
  /**< ESR fails as T3417 EXT timer expires */ 
} ps_sys_3gpp_rab_reject_cause_code_type_enum;

/**
  @brief Data type to be used with PS_SYS_IOCTL_3GPP_GET_LAST_RAB_REJECT_INFO and 
         PS_SYS_EVENT_3GPP_RAB_REJECT_IND
*/
typedef struct
{
  ps_sys_3gpp_rab_reject_cause_code_type_enum  rab_reject_cause;
} ps_sys_3gpp_rab_reject_info_type;
/**
  @brief Data type to be used with  
         PS_SYS_EVENT_MODEM_ASST_KEEP_ALIVE_UPDATE and 
         PS_SYS_CONF_MODEM_ASST_KEEP_ALIVE_CONFIG
  @see   PS_SYS_EVENT_MODEM_ASST_KEEP_ALIVE_UPDATE 
         PS_SYS_CONF_MODEM_ASST_KEEP_ALIVE_CONFIG
*/
typedef enum {
  KEEPALIVE_TYPE_NAT = 0 /**< NAT \n */
} ps_sys_keep_alive_type_enum;

typedef struct
{
  boolean start_ka_timer;   
  /**< Value:
        - 0: Stop timer
        - 1: Start timer
  */
  uint32                                  ka_handle;
  ps_sys_keep_alive_type_enum             ka_type;
  uint32                                  ka_timer_value;
  char                                    apn_name[PS_SYS_MAX_APN_LEN];
  uint32                                  apn_name_len;
  ps_ip_addr_type                         dest_ip_addr;
  ps_ip_addr_type                         source_ip_addr;
  uint16                                  dest_port;
  uint16                                  source_port;    
}ps_sys_modem_asst_ka_update_info_type;

/**
  @brief Data type to be used with PS_SYS_CONF_IMSI_PUBLIC_KEY and 
         PS_SYS_EVENT_UPDATED_IMSI_PUBLIC_KEY.
*/

typedef ps_sys_3gpp_mcc_mnc_type ps_sys_mcc_mnc_type;

typedef struct
{  
  uint16 imsi_public_key_size;
  uint8 imsi_public_key[PS_SYS_MAX_IMSI_PUBLIC_KEY_LEN];

  uint16 key_identifier_len;
  char key_identifier[PS_SYS_MAX_IMSI_PUBLIC_KEY_ID_AVP_LEN];

  boolean                      mcc_mnc_valid;
  ps_sys_mcc_mnc_type          mcc_mnc_info; 
  /**< MCC/MNC values */

  boolean                      expiry_time_valid;
  uint64                       expiry_time;
  /**< Format in UTC. Time value in milliseconds with an offset of 
       "Jan 1, 1970 00:00:00". */
}ps_sys_imsi_public_key_info_type;

/**
  @brief Data type to be used with PS_SYS_CONF_3GPP_BACKGROUND_TRAFFIC_INFO
         and PS_SYS_EVENT_3GPP_BACKGROUND_TRAFFIC_IND.
*/
typedef struct
{
  boolean background_traffic_status;
}ps_sys_3gpp_background_traffic_info;

typedef enum
{
  PS_SYS_CLIENT_TE = 0,
  PS_SYS_CLIENT_MODEM = 1
} ps_sys_client_type;

#define PS_SYS_THRESHOLD_LIST_LEN 8

typedef struct{
  ps_sys_thrpt_status_reason_enum_type thrput_status;
  ps_sys_rat_ex_enum_type              rat;
  uint32                               hyst_time;
  uint32                               hyst_mag_chng_size;
  uint32                               threshold_list[PS_SYS_THRESHOLD_LIST_LEN];
  uint8                                threshold_list_len;
}ps_sys_thrput_actual_thres_criteria_type;

/**
  @brief Actual uplink threshold throughput configuration 
         information
   
  @see   PS_SYS_CONF_ACTUAL_UL_THRESHOLD_CONFIG
  @see   PS_SYS_EVENT_UL_THRPUT_ACTUAL_THRES_CRITERIA 
*/
typedef ps_sys_thrput_actual_thres_criteria_type ps_sys_ul_thrput_actual_thres_criteria_type;

/**
  @brief Actual downlink threshold throughput configuration 
         information
   
  @see   PS_SYS_CONF_UL_THRPUT_ACTUAL_THRES_CRITERIA 
  @see   PS_SYS_EVENT_UL_THRPUT_ACTUAL_THRES_CRITERIA 
*/
typedef ps_sys_thrput_actual_thres_criteria_type ps_sys_dl_thrput_actual_thres_criteria_type;

/**
  @brief Data type to be used with dynamic DDS switch feature.
*/
#define PS_SYS_3GPP_APP_ID_MAX         256

typedef struct{
  char   app_id_string[PS_SYS_3GPP_APP_ID_MAX]; /**< Application ID */
  uint8  app_id_string_len;                /**<  Length of application ID */
}ps_sys_3gpp_app_id_info_type;

#define PS_SYS_3GPP_IIN_ID_LEN_MAX 10
#define PS_SYS_3GPP_IIN_LIST_MAX 20

typedef struct{
  uint32 iin_len;
  uint8  iin[PS_SYS_3GPP_IIN_ID_LEN_MAX];
  uint16 weight;
}ps_sys_3gpp_iin_info_type;

typedef enum
{
  PS_SYS_APPL_SERVICE_NAME_USSI          = DS_SYS_APPL_SERVICE_NAME_USSI,
  PS_SYS_APPL_SERVICE_NAME_MMTEL_VOICE   = DS_SYS_APPL_SERVICE_NAME_MMTEL_VOICE,
  PS_SYS_APPL_SERVICE_NAME_MMTEL_VIDEO   = DS_SYS_APPL_SERVICE_NAME_MMTEL_VIDEO,
  PS_SYS_APPL_SERVICE_NAME_SS_XCAP_CFG   = DS_SYS_APPL_SERVICE_NAME_SS_XCAP_CFG,
  PS_SYS_APPL_SERVICE_NAME_SMSOIP        = DS_SYS_APPL_SERVICE_NAME_SMSOIP,
  PS_SYS_APPL_SERVICE_NAME_BIP           = DS_SYS_APPL_SERVICE_NAME_BIP,  
  PS_SYS_APPL_SERVICE_NAME_OMADM_OVER_PS = DS_SYS_APPL_SERVICE_NAME_OMADM_OVER_PS,
  PS_SYS_APPL_SERVICE_NAME_OTHER         = DS_SYS_APPL_SERVICE_NAME_OTHER,
  PS_SYS_APPL_SERVICE_NAME_MAX           = DS_SYS_APPL_SERVICE_NAME_MAX
} ps_sys_appl_service_name_enum_type;

typedef enum
{
  PS_SYS_PS_DATA_AVAIL_TYPE_NONE            = DS_SYS_PS_DATA_AVAIL_TYPE_NONE,
  PS_SYS_PS_DATA_AVAIL_TYPE_EXEMPT          = DS_SYS_PS_DATA_AVAIL_TYPE_EXEMPT,
  PS_SYS_PS_DATA_AVAIL_TYPE_NON_EXEMPT      = DS_SYS_PS_DATA_AVAIL_TYPE_NON_EXEMPT,
} ps_sys_ps_data_avail_type_enum_type;

typedef enum
{
  PS_SYS_PS_DATA_AVAIL_LIST_TYPE_NONE       = DS_SYS_PS_DATA_AVAIL_LIST_TYPE_NONE,
  PS_SYS_PS_DATA_AVAIL_LIST_TYPE_HOME       = DS_SYS_PS_DATA_AVAIL_LIST_TYPE_HOME,
  PS_SYS_PS_DATA_AVAIL_LIST_TYPE_ROAM       = DS_SYS_PS_DATA_AVAIL_LIST_TYPE_ROAM,
  PS_SYS_PS_DATA_AVAIL_LIST_TYPE_HOME_ROAM  = DS_SYS_PS_DATA_AVAIL_LIST_TYPE_HOME_ROAM,
} ps_sys_ps_data_avail_list_type_enum_type;


typedef struct
{
  ps_sys_appl_service_name_enum_type        service;
  /** Service type */
  ps_sys_ps_data_avail_type_enum_type         type;
  /** Service type if its exempt or non-exempt */
} ps_sys_ps_data_avail_info_type;

typedef enum
{
  PS_SYS_PS_DATA_AVAIL_STATUS_UNKNOWN = DS_SYS_PS_DATA_AVAIL_STATUS_UNKNOWN,
  PS_SYS_PS_DATA_AVAIL_STATUS_ON      = DS_SYS_PS_DATA_AVAIL_STATUS_ON,
  PS_SYS_PS_DATA_AVAIL_STATUS_OFF     = DS_SYS_PS_DATA_AVAIL_STATUS_OFF
} ps_sys_ps_data_avail_status_enum_type;

typedef struct
{
  uint32                                   num_services;
  /** Number of services*/
  ps_sys_ps_data_avail_info_type           service_info[PS_SYS_APPL_SERVICE_NAME_MAX];
  /** Array of Service and type*/
  uint32                                   reason;
  /** Reason for service type change in service info arrary*/
  ps_sys_ps_data_avail_list_type_enum_type list_type;
  /**List type*/
  ps_sys_ps_data_avail_status_enum_type    data_avail_status;
  /**Data available status*/
} ps_sys_ps_data_avail_type;

/**
  @brief Data type to be used with 
         PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_MODE and
         PS_SYS_CONF_AP_ASST_APN_PREF_SYS
  @see   PS_SYS_EVENT_AP_ASST_APN_PREF_SYS_MODE
*/
typedef struct
{
  boolean      ap_asst_mode;
}ps_sys_ap_asst_apn_pref_sys_type;


#ifdef QWES_FEATURE_ENABLE 

/**
  @brief CAAS Feature Id list
   
  @see   PS_SYS_CONF_CAAS_FEATURE_ID_LIST 
*/
typedef struct
{
  uint8   feature_cnt;
  /** Number of feature Ids */  
  int     feature_ids[PS_SYS_MAX_NUM_FEATURE_IDS];
  /** List of feature Ids */
} ps_sys_caas_feature_list_type;

typedef enum
{
  PS_SYS_CAAS_FEATURE_STATUS_ALLOWED = 0,
  PS_SYS_CAAS_FEATURE_STATUS_NOT_ALLOWED  = 1,
  PS_SYS_CAAS_FEATURE_STATUS_PERMISSION_EXPIRED = 2,
  PS_SYS_CAAS_FEATURE_STATUS_PROCESSING_ERROR = 3,
  PS_SYS_CAAS_FEATURE_STATUS_NO_LICENSES = 4,
  PS_SYS_CAAS_FEATURE_STATUS_GRACE_ACTIVATION_NEEDED =5
} ps_sys_cass_feature_status_enum_type;

typedef enum
{
  PS_SYS_CAAS_STATUS_REQUEST_SERVICED,
  PS_SYS_CAAS_STATUS_HAVEN_TA_BUSY,
  PS_SYS_CAAS_STATUS_REQUEST_LOST,
  PS_SYS_CAAS_STATUS_REQUEST_NOT_SECURED_PROPERLY,
  PS_SYS_CAAS_STATUS_REQUEST_NOT_ENCODED_PROPERLY,
  PS_SYS_CAAS_STATUS_HAVEN_TA_NOT_AVAILABLE,
  PS_SYS_CAAS_STATUS_REQUEST_NOT_SERVICED
}ps_sys_cass_status_type;

typedef struct
{
  int                             feature_id;
  /** Feature Id */
  
  ps_sys_cass_feature_status_enum_type feature_status;
  /** Feature status */
} ps_sys_caas_feature_status_type;


/**
  @brief CAAS Feature Status List
   
  @see   PS_SYS_EVENT_CAAS_FEATURE_STATUS 
*/
typedef struct
{ 
  ps_sys_cass_status_type          req_status;
  ps_sys_caas_feature_status_type  status[PS_SYS_MAX_NUM_FEATURE_IDS];
  /** Feature status */  
} ps_sys_caas_feature_status_list_type;

#endif /* QWES_FEATURE_ENABLE  */

/*===========================================================================

                      PUBLIC FUNCTION DEFINITIONS

===========================================================================*/

/** 
  @brief This function initializes data structures for ps_sys 
         module. It initializes event queues, ps sys critical section,
         and sets the command handler for event callback in PS task. It also
         allocates memory for holding temporary storage for config data
*/
void ps_sys_init
(
  void
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

/** 
  @brief This function returns technology type given rat value
*/
ps_sys_tech_enum_type ps_sys_get_tech_from_rat
(
  ps_sys_rat_ex_enum_type  rat
);

/** 
  @brief This function sets the DDS(Default data subscription) 
         in the system. Only Ds3g module can set this value
*/
void ps_sys_set_default_data_subscription
(
  ps_sys_subscription_enum_type  def_data_subs
);

/** 
  @brief This function gets the DDS(Default data subscription) 
         in the system
*/
ps_sys_subscription_enum_type ps_sys_get_default_data_subscription
(
  void
);

/** 
  @brief This function sets the Temp DDS(Default data subscription) 
         in the system. Only Ds3g module can set this value
*/
void ps_sys_set_temp_default_data_subscription
(
  ps_sys_subscription_enum_type  def_data_subs,
  ps_sys_dds_switch_type_enum    dds_switch_type
);

/** 
  @brief This function gets the temp DDS(Default data subscription) 
         in the system
*/
ps_sys_subscription_enum_type ps_sys_get_temp_default_data_subscription
(
  void
);

/** 
  @brief This function gets the DDS(Default data subscription) switch type
         in the system
*/
ps_sys_dds_switch_type_enum ps_sys_get_dds_switch_type
(
  void
);

void ps_sys_nv_refresh_reg_init
(
  void
);


#ifdef __cplusplus
}
#endif

#endif /* PS_SYS_H */
