#ifndef DS3GCOMMONUTIL_H
#define DS3GCOMMONUTIL_H
/*===========================================================================

                3G DATA COMMON UTIL

                           H E A D E R   F I L E

GENERAL DESCRIPTION
  This software unit contains functions for DS 3G Common Utility Functions

  Copyright (c) 2018 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:
  $Header:

when       who        what, where, why
--------   ---     ---------------------------------------------------------- 
03/10/17   pnanda      Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "sys.h"
#include "sys_v.h"
#include "dstask_v.h"
#include "ds_sys.h"
#include "ds_sys_ioctl.h"
#include "ps_sys.h"
#include "ds_dsd_ext_i.h"
#include "ds3gmgr.h"
#include "nv_items.h"
#include "ds_3gpp_pdn_throt_spec.h"
#ifdef FEATURE_DATA_TSRC
#include "ds3g_tsrc_mgr.h"
#endif /* FEATURE_DATA_TSRC */
#include "ds_3gppi_utils.h"

/*===========================================================================

            EXTERNAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains external definitions for constants, macros, types,
variables and other items needed by other modules.

===========================================================================*/

/*------------------------------------------------------------------------- 
  PDN label tag for OTASN PDN
------------------------------------------------------------------------*/
#define DS3G_OTASN_PDN_LABEL "ota"

/*-------------------------------------------------------------------------
  PAP and CHAP definitions used by 3GPP and 3GPP2 MH
-------------------------------------------------------------------------*/
#define DS3G_PDN_AUTH_PAP   0x0
#define DS3G_PDN_AUTH_CHAP  0x1

/*-------------------------------------------------------------------------
  ETSI command macro for dial string validation.
-------------------------------------------------------------------------*/
#define DS3G_ETSI_CMD_MODE      0
#define DS3G_NON_ETSI_CMD_MODE  1

/*-------------------------------------------------------------------------
  Structure for IPv6 IID type
-------------------------------------------------------------------------*/
typedef struct
{
  byte    iid_enum_type;
  uint64  iid_val;
}ds3g_ipv6_iid_type;
  
/*---------------------------------------------------------------------------
  Structure to store subscription specific IPv6 config info read from EFS/NV
---------------------------------------------------------------------------*/
typedef struct
{
  boolean                         pdsn_as_proxy_ipv6_dns_server;
  ip6_sm_config_type              sm_config;
  struct ps_in6_addr              primary_dns;
  struct ps_in6_addr              secondary_dns;
  ds3g_ipv6_iid_type              iid_info;
}ds3g_ipv6_efs_nv_subs_specific_info;

/*---------------------------------------------------------------------------
  Structure to store IPv6 config info read from EFS/NV
---------------------------------------------------------------------------*/
typedef struct
{
  boolean                               is_enabled;
  ds3g_ipv6_efs_nv_subs_specific_info  *subs_info[DS3GSUBSMGR_SUBS_ID_MAX];
  ps_iface_ip_ver_failover_e_type       failover_mode;
}ds3g_ipv6_efs_nv_config_info;

#ifdef FEATURE_DATA_PS_IPV6
/*---------------------------------------------------------------------------
  Global variable which is updated from the NV during powerup 
---------------------------------------------------------------------------*/
extern ds3g_ipv6_efs_nv_config_info ipv6_nv_efs_info;
#endif /* FEATURE_DATA_PS_IPV6 */

typedef struct
{
  list_link_type  link;
  ps_iface_type  *iface_ptr;
  boolean         is_attach_iface;
}ds3g_iface_list_type;

/*---------------------------------------------------------------------------
   Specifies dial character validation result
---------------------------------------------------------------------------*/  
typedef enum
{
  DS3G_DIAL_EMPTY,          /*  Empty                    */
  DS3G_DIAL_DIGIT,          /*  Digits                   */
  DS3G_DIAL_ASCII,          /*  ASCII chars              */
  DS3G_DIAL_ASCII_ABC,      /*  ASCII A,B, or C          */
  DS3G_DIAL_ASCII_D,        /*  ASCII D                  */
  DS3G_DIAL_ASCII_E,        /*  ASCII E                  */
  DS3G_DIAL_ALLOWED,        /*  OTHER ALLOWED CHARACTERS */
  DS3G_DIAL_SEMICOLON,      /*  SEMICOLON                */
  DS3G_DIAL_STAR,           /*  ASTERIX                  */
  DS3G_DIAL_POUND,          /*  POUND                    */
  DS3G_DIAL_PLUS,           /*  PLUS                     */
  DS3G_DIAL_STRIP,          /*  Stripped chars           */
  DS3G_DIAL_IGNORE,         /*  Insignificant chars      */
  DS3G_DIAL_ERROR,          /*  has illegal chars        */
  DS3G_DIAL_MAX             /*  Internal use only        */
} ds3g_dial_val_e_type;

/*---------------------------------------------------------------------------
  DS3G APN Type Enum values allowed to be configured in NV.
---------------------------------------------------------------------------*/ 
typedef enum
{
  DS3G_APN_TYPE_NONE = 0,
  DS3G_APN_TYPE_SOS,
  DS3G_APN_TYPE_IMS,
  DS3G_APN_TYPE_INTERNET
}ds3g_apn_enum_type;

/*---------------------------------------------------------------------------
  When client queries for Emergency APN Type, DS3G shall return the 
  APN Type corresponding to the RAT on which Emergency call was made
  and the PDF that is active (in order of preference)
---------------------------------------------------------------------------*/ 
typedef enum
{
  DS3G_MIN_IDX = 0,
  DS3G_WWAN_PREFERRED_IDX = DS3G_MIN_IDX,
  DS3G_WWAN_SECONDARY_IDX,
  DS3G_IWLAN_PREFERRED_IDX,
  DS3G_IWLAN_SECONDARY_IDX,
  DS3G_MAX_IDX
}ds3g_emergency_pref_apn_type;


/*--------------------------------------------------------------------------
  Structure used to store the DS3G Util EFS item values. Any new 
  DS3G Util EFS item read should be stored here.
--------------------------------------------------------------------------*/
typedef struct
{
  boolean                   wlan_3g_ip_continuity_enabled;
  boolean                   optimized_ipv6_prefix_enabled;
  boolean                   optimized_handoff_ip_config_enabled;
  boolean                   optimized_third_party_ho_enabled;    
  boolean                   wwan_to_iwlan_ho_disallowed_on_internet_pdn;
  ds3g_apn_enum_type        emergency_apn_type[DS3G_MAX_IDX];
  boolean                   otasn_pdn_feature_support;
  boolean                   wlan_lte_emc_ho_enabled;
} ds3g_efs_info_type;
/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
/*===========================================================================
FUNCTION      DS3G_UTIL_INIT

DESCRIPTION   This function is used to initialize the ds3g util module 
              during power up 
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_util_init( void );

/*===========================================================================
FUNCTION      ds3g_util_read_nv

DESCRIPTION   This function reads all the ds3g specific NV items
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_util_read_nv( void );

/*===========================================================================
FUNCTION      DS3G_PROCESS_MCFG_REFRESH_IND

DESCRIPTION   This function process MCFG refresh ind and re-reads all ds3g/dsd 
              NV items.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_process_mcfg_refresh_ind
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3G_SYS_SYS_MODE_TO_DS_APN_SYS_MODE

DESCRIPTION   This function is a utility function to convert cm sys sys mode 
              to ds apn pkt sys mode.
  
DEPENDENCIES  None

RETURN VALUE  TRUE or FALSE

SIDE EFFECTS  None
===========================================================================*/
ds_pkt_sys_mode_e_type ds3g_sys_sys_mode_to_ds_apn_sys_mode
(
  sys_sys_mode_e_type  sys_sys_mode
);

/*===========================================================================
FUNCTION      DS3G_GET_APN_PREF_PKT_MODE

DESCRIPTION   This function returns the currently preferred mode for packet 
              data calls on the specified PDN when MPPM is enabled.

DEPENDENCIES  None

RETURN VALUE  TRUE or FALSE

SIDE EFFECTS  None
===========================================================================*/
ds_pkt_sys_mode_e_type ds3g_get_apn_pref_pkt_mode
(
  ds3g_apn_name_type  *apn_name
);

/*===========================================================================
FUNCTION      DS3G_GET_APN_PREF_PKT_MODE_EX

DESCRIPTION   This function returns the currently preferred mode for packet 
              data calls on the specified PDN when MPPM is enabled on
              passed sub.

DEPENDENCIES  None

RETURN VALUE  TRUE or FALSE

SIDE EFFECTS  None
===========================================================================*/
ds_pkt_sys_mode_e_type ds3g_get_apn_pref_pkt_mode_ex
( 
   ds3g_apn_name_type      *apn_name,
   sys_modem_as_id_e_type   subs_id
);
/*===========================================================================
FUNCTION      DS3G_IS_APN_PREF_PKT_MODE_EX

DESCRIPTION   This function determines for a PDN if the specified mode is the 
              currently preferred mode for packet data calls for passed sub.

DEPENDENCIES  None

RETURN VALUE  TRUE or FALSE

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_is_apn_pref_pkt_mode_ex
( 
  ds_pkt_sys_mode_e_type  mode,
  ds3g_apn_name_type     *apn_name,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3G_GET_PREF_SYS_MODE_FOR_PACKET_ORIG

DESCRIPTION   This function determines the currently preferred mode for packet
              data calls.

DEPENDENCIES  None

RETURN VALUE  sys_sys_mode_e_type: sys mode

SIDE EFFECTS  None
===========================================================================*/
sys_sys_mode_e_type ds3g_get_pref_sys_mode_for_packet_orig(sys_modem_as_id_e_type  subs_id );

/*===========================================================================
FUNCTION      DS3G_IS_DORM_ALLOWED_IN_PKT_MODE

DESCRIPTION   This function determines if dormancy is allowed in the currently 
              preferred mode for packet data calls.

DEPENDENCIES  None

RETURN VALUE  TRUE or FALSE

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_is_dorm_allowed_in_pkt_mode
(
  sys_sys_mode_e_type  mode,
  sys_modem_as_id_e_type  subs_id  
);

/*===========================================================================
FUNCTION      DS3G_IS_PREF_CS_DOMAIN_MODE

DESCRIPTION   This function determines if the specified mode is the currently 
              preferred mode for CS data calls.
  
DEPENDENCIES  None

RETURN VALUE  TRUE or FALSE

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_is_pref_cs_domain_mode
(
  sys_sys_mode_e_type  mode
);
/*===========================================================================
FUNCTION      DS3G_GET_IPV6_SM_CONFIG_INFO

DESCRIPTION   Get IPv6 SM config info configured through NV item

DEPENDENCIES  None

RETURN VALUE  ip6_sm_config_type* - IPv6 SM config info

SIDE EFFECTS  None
===========================================================================*/
ip6_sm_config_type* ds3g_get_ipv6_sm_config_info
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3G_GET_IPV6_PRIMARY_DNS_ADDR

DESCRIPTION   Get IPv6 primary DNS server address

DEPENDENCIES  None

RETURN VALUE  ps_in6_addr* - Ptr containing the primary DNS server addr

SIDE EFFECTS  None
===========================================================================*/
struct ps_in6_addr* ds3g_get_ipv6_primary_dns_addr
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3G_GET_IPV6_SECONDARY_DNS_ADDR

DESCRIPTION   Get IPv6 secondary DNS server address

DEPENDENCIES  None

RETURN VALUE  ps_in6_addr* - Ptr containing the secondary DNS server addr

SIDE EFFECTS  None
===========================================================================*/

struct ps_in6_addr* ds3g_get_ipv6_secondary_dns_addr
(
  sys_modem_as_id_e_type  subs_id
);
/*===========================================================================
FUNCTION DS3G_IS_OTASN_NV_ENABLED

DESCRIPTION
  This function return the NV configured value for OTASN feature support
 
PARAMETERS 
  subs_id: Subscription info
 
DEPENDENCIES
  None

RETURN VALUE
  TRUE, if OTASN NV is Enabled
  FALSE, Otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean ds3g_is_otasn_nv_enabled
(
  sys_modem_as_id_e_type    subs_id
);
/*===========================================================================
FUNCTION      DS3G_IS_IPV6_ENABLED

DESCRIPTION   Returns whether IPv6 is enabled through NV or not.

DEPENDENCIES  None

RETURN VALUE  TRUE: IPv6 enabled
              FALSE: IPv6 disabled

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_is_ipv6_enabled( void );

/*===========================================================================
FUNCTION      DS3G_IS_PDSN_AS_PROXY_IPv6_DNS_SERVER

DESCRIPTION   Returns whether PDSN acts as proxy IPv6 DNS server

DEPENDENCIES  None

RETURN VALUE  Boolean

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_is_pdsn_as_proxy_ipv6_dns_server
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3G_GET_FAILOVER_MODE

DESCRIPTION   Returns the failover mode set in NV

DEPENDENCIES  None

RETURN VALUE  Boolean

SIDE EFFECTS  None
===========================================================================*/
ps_iface_ip_ver_failover_e_type ds3g_get_failover_mode( void );

/*===========================================================================
FUNCTION      DS3G_GET_IPV6_IID_INFO

DESCRIPTION   Returns IPv6 iid info

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_get_ipv6_iid_info
(
  sys_modem_as_id_e_type  subs_id,
  ds3g_ipv6_iid_type     *iid_info_p
);

/*===========================================================================
FUNCTION      DS3G_READ_EHRPD_AUTH_IN_USIM

DESCRIPTION   Returns ehrpd auth in usim NV item

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_get_ehrpd_auth_in_usim
( 
  sys_modem_as_id_e_type  subs_id
);


/*===========================================================================
FUNCTION      DS3G_GET_NV_MODE_PREF

DESCRIPTION   This function returns mode preference NV value
 
DEPENDENCIES  None

RETURN VALUE  NV Mode pref

SIDE EFFECTS  None
===========================================================================*/
nv_mode_enum_type ds3g_get_nv_mode_pref
(
  sys_modem_as_id_e_type  subs_id
);

#ifdef FEATURE_8960_SGLTE_FUSION
/*===========================================================================
FUNCTION      DS3G_GET_DISABLE_DSD_SYNC

DESCRIPTION   Returns Disable DSD sync NV item

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_get_disable_dsd_sync( void );
#endif /* FEATURE_8960_SGLTE_FUSION */


/*===========================================================================
FUNCTION      DS3G_GET_DUN_ACC_CFG

DESCRIPTION   Returns DUN acc cfg NV item

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
uint8 ds3g_get_dun_acc_cfg( void );

/*===========================================================================
FUNCTION      DS3G_GET_REFRESH_VOTE_OK_INFO

DESCRIPTION   Returns refresh vote ok NV item

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_get_refresh_vote_ok_info
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3G_GET_CONFIG_PDN_LEVEL_AUTH

DESCRIPTION   Returns config_pdn_level_auth NV item

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
uint8 ds3g_get_config_pdn_level_auth
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3G_GET_FLOW_DISABLE_TIMER

DESCRIPTION   Returns FLOW_DISABLE_TIMER in msecs

DEPENDENCIES  None

RETURN VALUE  timer value in msecs

SIDE EFFECTS  None
===========================================================================*/
uint32 ds3g_get_flow_disable_timer
(
  void
);

#ifdef FEATURE_DATA_FLOW_MGMT
/*===========================================================================
FUNCTION ds3_get_burst_hysteresis_timer

DESCRIPTION
  This Function gets the burst based hysteresis timer. This value is the one
  which is read from the nv item.

PARAMETERS
  void

DEPENDENCIES
  None.

RETURN VALUE
  uint32

SIDE EFFECTS
  None

===========================================================================*/
uint32  ds3g_get_burst_periodic_timer
(
  void
);

/*===========================================================================
FUNCTION ds3_get_burst_prohibit_timer

DESCRIPTION
  This Function gets the burst based prohibit timer. This value is the one
  which is read from the nv item.

PARAMETERS
  void

DEPENDENCIES
  None.

RETURN VALUE
  uint32

SIDE EFFECTS
  None

===========================================================================*/
uint32  ds3g_get_burst_prohibit_timer
(
  void
);
/*===========================================================================
FUNCTION ds3g_get_burst_2g_rat_frequency

DESCRIPTION
  This Function gets the burst based parameter - 2g frequency. This value is the one
  which is read from the nv item.

PARAMETERS
  void

DEPENDENCIES
  None.

RETURN VALUE
  uint32

SIDE EFFECTS
  None

===========================================================================*/
uint32  ds3g_get_burst_2g_rat_periodic_frequency
(
  void
);
/*===========================================================================
FUNCTION ds3g_get_burst_3g_rat_frequency

DESCRIPTION
  This Function gets the burst based parameter - 3g frequency.
  This value is the one which is read from the nv item.

PARAMETERS
  void

DEPENDENCIES
  None.

RETURN VALUE
  uint32

SIDE EFFECTS
  None

===========================================================================*/
uint32  ds3g_get_burst_3g_rat_periodic_frequency
(
  void
);
/*===========================================================================
FUNCTION ds3g_get_burst_4g_rat_frequency

DESCRIPTION
  This Function gets the burst based parameter - 3g frequency.
  This value is the one which is read from the nv item.

PARAMETERS
  void

DEPENDENCIES
  None.

RETURN VALUE
  uint32

SIDE EFFECTS
  None

===========================================================================*/
uint32  ds3g_get_burst_4g_rat_periodic_frequency
(
  void
);


/*===========================================================================
FUNCTION ds3g_set_burst_params

DESCRIPTION
  This Function sets the burst based hysteresis params. This value is the one configured
  from the diag.

PARAMETERS
  void

DEPENDENCIES
  None.

RETURN VALUE
  uint32

SIDE EFFECTS
  None

===========================================================================*/
void  ds3g_set_burst_params
(
  uint32     periodic_timer_value,
  uint32     rat_2g_periodic_frequency,
  uint32     rat_3g_periodic_frequency,
  uint32     rat_4g_periodic_frequency,
  uint32     prohibit_timer_value 
);
#endif

/*===========================================================================
FUNCTION      DS3G_GET_ENABLE_ARBITRATION

DESCRIPTION   Returns enable arbitartion NV item

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_get_enable_arbitration( void );

/*===========================================================================
FUNCTION      DS3G_GET_SUPPRESS_GSM_ON_SRVCC_CSFB

DESCRIPTION   Returns Suppress GSM on SRVCC/CSFB NV item

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_get_suppress_gsm_on_srvcc_csfb
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3G_GET_CONFIG_LATENCY_INFO

DESCRIPTION   Returns config latency info

DEPENDENCIES  None

RETURN VALUE  TRUE/FALSE - return Latency Info nv value

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_get_config_latency_info(void);

/*===========================================================================
FUNCTION      DS3G_PKT_SYS_MODE_TO_SYS_SYS_MODE

DESCRIPTION   This function is a utility function to convert ds apn pkt 
              sys mode to sys sys mode.

DEPENDENCIES  None

RETURN VALUE  Mapped SYS mode : sys_sys_mode_e_type

SIDE EFFECTS  None
===========================================================================*/
sys_sys_mode_e_type ds3g_pkt_sys_mode_to_sys_sys_mode
(
  ds_pkt_sys_mode_e_type  ds_pkt_mode
);

#ifdef FEATURE_DATA_LTE
/*===========================================================================
FUNCTION      DS3G_GET_DIV_DUPLEX_FROM_SO_MASK

DESCRIPTION   Utility function to get LTE div duplex from so mask.

DEPENDENCIES  None.

RETURN VALUE  The div duplex for LTE

SIDE EFFECTS  None.
===========================================================================*/
sys_div_duplex_e_type ds3g_get_div_duplex_from_so_mask
(
  uint32  so_mask
);

/*===========================================================================
FUNCTION      DS3G_GET_SO_MASK_FROM_DIV_DUPLEX

DESCRIPTION   Utility function to get so mask from div duplex.

DEPENDENCIES  None.

RETURN VALUE  The so mask for LTE

SIDE EFFECTS  None.
===========================================================================*/
uint64 ds3g_get_so_mask_from_div_duplex
(
  sys_div_duplex_e_type  div_duplex,
  sys_sys_mode_e_type    mode
);
#endif /* FEATURE_DATA_LTE */

/*===========================================================================
FUNCTION      DS3G_CONVERT_DSD_RADIO_INFO

DESCRIPTION   This is the function to convert dsd radio info to ps status

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_convert_dsd_radio_info
(
  dsd_radio_type                   radio_idx,
  ds_dsd_ext_radio_info_type      *dsd_radio_info,
  ps_sys_subscription_enum_type    ps_subs_id,
  ps_sys_system_status_info_type  *dsd_sys_info
);

/*===========================================================================
FUNCTION      DS3G_UPDATE_DATA_CONFIG_INFO

DESCRIPTION   Puts all the required nv item file paths in the 
              data_config_info.conf file. Would be called during powerup.
 
DEPENDENCIES  None

RETURN VALUE  length of string added to buffer for success. 
              -1 for failure.

SIDE EFFECTS  None
===========================================================================*/
int32 ds3g_update_data_config_info
(
  char   *file_paths_buf,
  uint32  efs_conf_file_size
);

/*===========================================================================
FUNCTION      DS3G_GET_EFS_FILE_PATH_SIZE

DESCRIPTION   Returns the total ds3g EFS item file paths size

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
uint32 ds3g_get_efs_file_path_size( void );

/*===========================================================================
FUNCTION DS3G_PROC_DIAL_STR

DESCRIPTION
  Parses a dial string, looking for allowed non-digits, and
  illegal non-digits.  Digits, and allowed non-digits are copied
  to the output.  Illegal characters terminate processing.  All
  other characters are ignore.  Upper and lower case are treated
  as being the same.

  Since the output will be no longer than the input, the output
  string may be the same as the input string.

DEPENDENCIES
  None

RETURN VALUE
  Returns enum value indicating dial string content or error

SIDE EFFECTS
  None
===========================================================================*/
ds3g_dial_val_e_type ds3g_proc_dial_str
(
  const byte *in_ptr,          /*  Input string, null terminated   */
  byte       *out_ptr,         /*  Output string, null terminated  */
  boolean     etsi_cmd_mode    /*  3GPP(1) or 3GPP2(0) mode  */
);

/*===========================================================================
FUNCTION DS3G_IS_EPDG_3G_IP_CONTINUITY_ENABLED

DESCRIPTION
  This functions tell whether EPDG-3G IP Continuity is enabled or not

DEPENDENCIES
  None

RETURN VALUE
  TRUE, EPDG-3G IP Continuity is enabled
  FALSE, otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean ds3g_is_epdg_3g_ip_continuity_enabled
(
  ps_sys_subscription_enum_type    ps_subs_id
);

/*===========================================================================
FUNCTION DS3G_IS_OPTIMIZED_IPV6_PREFIX_ENABLED

DESCRIPTION
  This function tell whether Optimized IPv6 Prefix shall be used during
  handovers

DEPENDENCIES
  None

RETURN VALUE
  TRUE, Optimized Ipv6 Prefix is Enabled
  FALSE, Otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean ds3g_is_optimized_ipv6_prefix_enabled
(
  ps_sys_subscription_enum_type    ps_subs_id
);

/*===========================================================================
FUNCTION DS3G_SET_OPTIMIZED_IPV6_PREFIX_ENABLED

DESCRIPTION
  This function tell whether Optimized IPv6 Prefix shall be used during
  handovers

DEPENDENCIES
  None

RETURN VALUE
  TRUE, Optimized Ipv6 Prefix is Enabled
  FALSE, Otherwise

SIDE EFFECTS
  None
===========================================================================*/
void ds3g_set_optimized_ipv6_prefix_enabled
(
  boolean                          flag,
  ps_sys_subscription_enum_type    ps_subs_id
);
/*===========================================================================
FUNCTION DS3G_IS_OPTIMIZED_HANDOFF_IP_CONFIG_ENABLED

DESCRIPTION
  This function tell whether Optimized IP config shall be used during
  handovers

DEPENDENCIES
  None

RETURN VALUE
  TRUE, Optimized Handoff Ip Config is Enabled
  FALSE, Otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean ds3g_is_optimized_handoff_ip_config_enabled
(
  ps_sys_subscription_enum_type    ps_subs_id
);
/*===========================================================================
FUNCTION DS3G_IS_WWAN_TO_IWLAN_HO_DISALLOWED_ON_INTERNET_PDN

DESCRIPTION
  This function tell whether allow internet pdn handover from 3gpp rat to wlan

DEPENDENCIES
  None

RETURN VALUE
  TRUE, it is disallowed
  FALSE, Otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean ds3g_is_wwan_to_iwlan_ho_disallowed_on_internet_pdn
(
  ps_sys_subscription_enum_type    ps_subs_id
);
/*===========================================================================
FUNCTION DS3G_SET_OPTIMIZED_HANDOFF_IP_CONFIG_ENABLED

DESCRIPTION
  This function sets nv optimized_handoff_ip_config_enabled

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds3g_set_optimized_handoff_ip_config_enabled
(
  boolean                          flag,
  ps_sys_subscription_enum_type    ps_subs_id
);

/*===========================================================================
 
FUNCTION DS3G_CONVERT_TIMER3_VALUE_IE_TO_MSECS
 
DESCRIPTION
  This function is an API to convert GPRS timer 3 value IE to milliseconds.
  Note that a return value of zero indicates that the timer has been deactivated.
 
DEPENDENCIES
  None
 
RETURN VALUE
  uint64 - timer value in milliseconds
 
SIDE EFFECTS
  None
============================================================================*/
uint64 ds3g_convert_timer3_value_ie_to_msecs
(
  uint8    timer_value,
  boolean *timer_is_active_p
);

/*===========================================================================
FUNCTION DS3G_GET_EMERGENCY_APN_TYPE

DESCRIPTION
  This functions returns the APN Type on which Emergency call is active.

DEPENDENCIES
  None

RETURN VALUE
  APN Type on which Emergency call is active.

SIDE EFFECTS
  None
===========================================================================*/
ds3g_apn_enum_type ds3g_get_emergency_apn_type
(
  sys_modem_as_id_e_type       subs_id,
  ds3g_emergency_pref_apn_type preferred_apn_idx
);

/*===========================================================================
FUNCTION DS3G_IS_OPTIMIZED_THIRD_PARTY_HO_ENABLED

DESCRIPTION
  This function tell whether Optimized third party handoff shall be used during
  handovers

DEPENDENCIES
  None

RETURN VALUE
  TRUE, Optimized third party handoff is Enabled
  FALSE, Otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean ds3g_is_optimized_third_party_ho_enabled
(
  ds_sys_subscription_enum_type    ds_subs_id
);

/*===========================================================================
FUNCTION DS3G_SET_OPTIMIZED_THIRD_PARTY_HO_ENABLED

DESCRIPTION
  This function tell whether Optimized third party handoff shall be used during
  handovers

DEPENDENCIES
  None

RETURN VALUE
  TRUE, Optimized third party handoff is Enabled
  FALSE, Otherwise

SIDE EFFECTS
  None
===========================================================================*/
void ds3g_set_optimized_third_party_ho_enabled
(
  boolean                          flag,
  ds_sys_subscription_enum_type    ds_subs_id
);

/*===========================================================================
FUNCTION       DS3G_GET_ROAMING_INFO_FOR_INTERNET_APN_TYPE

DESCRIPTION    This utility function fetchs roaming info for internet apn type

PARAMETERS     profile_id_3gpp: provided by AP
               profile_id_3gpp2: provided by AP
               roaming_status : roamng status to be filled in
               subs_id: sub that is needed to be checked
               ps_errno: Error code if any to be returned to PS

DEPENDENCIES   NONE

RETURN VALUE   0: If Operation was successful
               -1: If for some reason the input params are invalid

SIDE EFFECTS   NONE
===========================================================================*/
int16 ds3g_get_roaming_info_for_internet_apn_type
(
  uint8                    profile_id_3gpp,
  uint8                    profile_id_3gpp2,
  boolean                 *roaming_status,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION       DS3G_GET_APN_NAME_FROM_PROFILE_ID

DESCRIPTION    This utility function is extracts the apn name string for a 
               given profile id. 3GPP/3GPP2 Tech insensitive. 

PARAMETERS     profile_id: provided by AP
               subs_id: Active PS subs
               profile_param_info_ptr: Structure to hold the data

DEPENDENCIES   NONE

RETURN VALUE   0: If Operation was successful
               -1: If for some reason the input params are invalid

SIDE EFFECTS   NONE
===========================================================================*/

boolean ds3g_get_apn_name_from_profile_id
(
  uint16                            profile_id,
  sys_modem_as_id_e_type            subs_id,
  ds_profile_info_type              *profile_param_info_ptr
);

/*===========================================================================
FUNCTION      DS3G_GET_PROFILE_PARAM_INFO

DESCRIPTION   This function gets the profile param info from profile cache. 
              tech type can be both 3GPP/3GPP2 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_get_profile_param_info
(
  uint16                      profile_id,
  ds_profile_identifier_type  profile_param_id,
  ds_profile_info_type       *profile_param_info_ptr,
  sys_modem_as_id_e_type      subs_id,
  ds_profile_tech_etype       tech_type
);

/*===========================================================================
FUNCTION      DS3G_GET_PROFILE_ID_FROM_APN_INFO

DESCRIPTION   This function gets the Profile id from APN Info passed in 
              the IOCTL. 

DEPENDENCIES  None

RETURN VALUE  -1: Failure 
               0: Success 

SIDE EFFECTS  None
===========================================================================*/
int ds3g_get_profile_id_from_apn_info
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno,
  ps_sys_tech_enum_type    tech_type
);

/*===========================================================================
FUNCTION      DS3G_MAP_DS3G_APN_TYPE_TO_DS_SYS_APN_TYPE

DESCRIPTION   This function maps the DS3G APN Type to DS Sys APN Type.

DEPENDENCIES  None

RETURN VALUE  DS Sys APN Type 

SIDE EFFECTS  None
===========================================================================*/
ds_sys_apn_enum_type ds3g_map_ds3g_apn_type_to_ds_sys_apn_type
(
  ds3g_apn_enum_type  apn_type
);

/*===========================================================================
FUNCTION      DS3G_MAP_DS_SYS_APN_TYPE_TO_APN_NAME

DESCRIPTION   This function maps the DS Sys APN Type to APN Name.

DEPENDENCIES  None

RETURN VALUE  TRUE: If APN Name found 
              FALSE: Otherwise 

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_map_ds_sys_apn_type_to_apn_name
(
  ds_sys_apn_enum_type      ds_sys_apn_type,
  sys_modem_as_id_e_type    cm_subs_id,
  char                      *apn_name
);

/*===========================================================================
FUNCTION      DS3G_MAP_DS_SYS_APN_TYPE_TO_DS_PROFILE_APN_TYPE

DESCRIPTION   This function maps the DS Sys APN Type to DS Profile APN 
              Type. 

DEPENDENCIES  None

RETURN VALUE  TRUE: If APN Name found 
              FALSE: Otherwise 

SIDE EFFECTS  None
===========================================================================*/
ds_apn_type_mask_enum_type ds3g_map_ds_sys_apn_type_to_ds_profile_apn_type
(
  ds_sys_apn_enum_type ds_sys_apn_type
);
/*===========================================================================
FUNCTION      DS3G_MAP_DS_PROFILE_APN_TYPE_TO_DS_SYS_APN_TYPE

DESCRIPTION   This function maps the DS Profile APN Type to DS Sys APN Type. 

DEPENDENCIES  None

RETURN VALUE  DS APN type

SIDE EFFECTS  None
===========================================================================*/
ds_sys_apn_enum_type ds3g_map_ds_profile_apn_type_to_ds_sys_apn_type
(
  ds_apn_type_mask_enum_type        ds_profile_apn_type  
);

/*===========================================================================
FUNCTION      DS3G_MAP_DS_SYS_APN_TYPE_TO_PS_IFACE_APN_TYPE

DESCRIPTION   This function maps the DS Sys APN Type to PS iface APN Type. 

DEPENDENCIES  None

RETURN VALUE  PS IFace APN type

SIDE EFFECTS  None
===========================================================================*/
ps_iface_apn_enum_type ds3g_map_ds_sys_apn_type_to_ps_iface_apn_type
(
  ds_sys_apn_enum_type        ds_sys_apn_type  
);

/*===========================================================================
FUNCTION      DS3G_MAP_PS_IFACE_APN_TYPE_TO_DS_SYS_APN_TYPE

DESCRIPTION   This function maps the PS iface APN Type. to DS Sys APN Type

DEPENDENCIES  None

RETURN VALUE  DS SYS APN type

SIDE EFFECTS  None
===========================================================================*/
ds_sys_apn_enum_type ds3g_map_ps_iface_apn_type_to_ds_sys_apn_type
(
  ps_iface_apn_enum_type      ps_iface_apn_type 
);

/*===========================================================================
FUNCTION DS3G_IS_EPDG_LTE_EMC_HO_ENABLED

DESCRIPTION
  This functions tell whether EPDG LTE EMC HO is enabled or not

DEPENDENCIES
  None

RETURN VALUE
  TRUE, EPDG-LTE EMC HO is enabled
  FALSE, otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean ds3g_is_epdg_lte_emc_ho_enabled
(
  ps_sys_subscription_enum_type    ps_subs_id
);

/*===========================================================================
FUNCTION DS3G_READ_EPDG_LTE_EMC_HO_ENABLED

DESCRIPTION
  This functions tell whether EPDG-LTE EMC HOis enabled or not

DEPENDENCIES
  None

RETURN VALUE
  TRUE, EPDG-LTE EMC HO is enabled
  FALSE, otherwise

SIDE EFFECTS
  None
===========================================================================*/
void ds3g_read_epdg_lte_emc_ho_enabled
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION             ds3g_chk_apn_type

DESCRIPTION
  This function checks if is IMS APN or not  

PARAMETERS 
  ps_iface_type               iface pointer: srat or trat iface pointer

RETURN VALUE
  boolean TRUE  : if is IMS APN Type
          FALSE : otherwise

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ds3g_chk_apn_type
(
  ps_iface_type           *iface_ptr,
  ds_profile_info_type     apn_name,
  ps_sys_apn_enum_type     apn_type,
  uint16                   profile_id,
  sys_modem_as_id_e_type   subs_id
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/
#endif /* DS3GCOMMONUTIL_H */
