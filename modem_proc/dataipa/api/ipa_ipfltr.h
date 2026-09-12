/*!
  @file
  ipa_ipfltr.h

  @brief
  This file contains definitions of all IPA IP Filter data structures and
  APIs

*/
/*===========================================================================

  Copyright (c) 2013 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/dataipa.mpss/3.2/api/ipa_ipfltr.h#1 $
$DateTime: 2019/07/05 03:48:36 $ 
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/08/19   sp      CR2371506: FR 51422: Prioritization of UL traffic in Low Latency Mode, 
                   Dynamic addition/deletion of low latency rule
05/02/18   SaCh    Adding API to install dest IPv6 Prefix match rule
06/22/16   SaCh    Adding register datapath_cb API and UL_STATIC_WWAN filter 
04/08/16   SaCh    Disabling QoS rule IPA offload.
07/15/15   vm      Avoid low latency handling Q6 IPA. This would now be handled
                   by DS
05/12/15   vm      Expose additional API to log all the existing filters
                   configured in IPA-HW on demand.
03/17/15   SaCh    Adding support to add UL CLAT static rules before Frag rule
03/17/15   SaCh    Adding support for DL CLAT ICMPv6 rule type
02/17/15   mi      CR#739684: IPA ZIP feature check-in.
11/10/14   tz      DL checksum offload for embedded data support.
10/07/14   ag      CR735266: ping latency logging and clock devote cnt logging
09/15/14   rp      Enable support for DL low latency traffic detection
06/06/14   rp      Enable support for low latency traffic detection in IPA driver
07/24/14   mi      Adding support for additional UL static filters and 
                   adding xlat field in the app_ptr of the DSM item.
04/25/14   rp      Changes to support ON-target testing on full stack CRM
04/15/14   pgm     CR649698:LLVM Compiler fixes.
04/08/14   SaCh    Accepting prefix length in num of bits instead of bytes in 
                   add_ul_ipv6_prefix_rule API
01/23/14   rp      Changes to support QXDM log/event packets
01/03/14   rp      UL packet tagging for Ethernet ARP packets
12/12/13   SaCh    Removing UL_STATIC_WWAN rule type. Added API for UL IPv6
                   Prefix rule.
12/12/13   rp      UL packet tagging with IP version & packet length
11/08/13   rp      Implementation of TCP-Ack prioritiation feature in the driver
10/01/13   am      Removed internal includes.
04/19/13   SaCh    Initial version
07/31/13   SaCh    Added new rule type: UL Static IPv4 Frag
===========================================================================*/

#ifndef IPA_IPFLTR_H
#define IPA_IPFLTR_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include <comdef.h>
#include <dsm.h>
#include "ipa.h"
/*===========================================================================

                              MACROS

===========================================================================*/
/*! @brief the maximum number of QoS rules */
#define IPA_IPFLTR_MAX_NUM_QOS_RULES 0

/*! @brief the maximum number of Socket rules */
#define IPA_IPFLTR_MAX_NUM_SOCKET_RULES 21

/*! @brief The maximum number of Mask Equal 32 Eqns */
#define IPA_IPFLTR_NUM_MEQ_32_EQNS 2

/*! @brief The maximum number of IHL offset Mask Equal 32 Eqns */
#define IPA_IPFLTR_NUM_IHL_MEQ_32_EQNS 2

/*! @brief The maximum number of Mask Equal 128 Eqns */
#define IPA_IPFLTR_NUM_MEQ_128_EQNS 2

/*! @brief The maximum number of meq 128 eqn bytes */
#define IPA_IPFLTR_NUM_MEQ_128_BYTES 16

/*! @brief The maximum number of IHL offset Range Check 16 Eqns */
#define IPA_IPFLTR_NUM_IHL_RANGE_16_EQNS 2

/*! @brief Value to indicate the applied rule is for priority packets */
#define IPA_IPFLTR_PRIORITY_PKT_RULE 1

/*! @brief Value to indicate the applied rule is Not for priority pkts */
#define IPA_IPFLTR_NON_PRIORITY_PKT_RULE 0

/*! @brief Invalid value for Filter rule result */
#define IPA_IPFLTR_RULE_RESULT_INVALID 0

/*! @brief Default Mask Value */
#define IPA_IPFLTR_DEFAULT_32_BIT_MASK 0xFFFFFFFF

/*! @brief Macro to get DL filter result from a DSM ptr */
#define IPA_IPFLTR_GET_DL_FLTR_RESULT(dsm_ptr) \
  ((dsm_ptr != NULL) ? \
        (((ipa_dsm_dl_app_field_info_s *)(&((dsm_ptr)->app_field)))->filter_result) : 0)

/*! @brief Macro to check if DL filter result is set in a DSM ptr */
#define IPA_IPFLTR_GET_DL_FLTR_RESULT_IS_SET(dsm_ptr) \
  ((dsm_ptr != NULL) ? \
  (((ipa_dsm_dl_app_field_info_s *)(&((dsm_ptr)->app_field)))->filter_res_present) : 0)

/*! @brief Macro to set DL filter result on a DSM ptr */
#define IPA_IPFLTR_SET_DL_FLTR_RESULT(dsm_ptr, _filter_result) \
  do \
{ \
  ((ipa_dsm_dl_app_field_info_s *)(&((dsm_ptr)->app_field)))->filter_result = _filter_result; \
  ((ipa_dsm_dl_app_field_info_s *)(&((dsm_ptr)->app_field)))->filter_res_present = 1; \
} while (0) 

/*! @brief Macro to get UL filter result from a DSM ptr */
#define IPA_IPFLTR_GET_UL_FLTR_RESULT(dsm_ptr) \
  ((dsm_ptr != NULL) ? \
  (((ipa_dsm_ul_app_field_info_s *)(&((dsm_ptr)->app_field)))->filter_result) : 0)

/*! @brief Macro to set UL filter result on a DSM ptr */
#define IPA_IPFLTR_SET_UL_FLTR_RESULT(dsm_ptr, _filter_result) \
  do \
{ \
  ((ipa_dsm_ul_app_field_info_s *)(&((dsm_ptr)->app_field)))->filter_result = _filter_result; \
} while (0) 

/*! @brief Macro to set the UL packet info tag on a DSM ptr */
#define IPA_IPFLTR_SET_UL_PKT_INFO(dsm_ptr, ip_ver, len, uCP) \
do \
{ \
  ((ipa_ipfltr_ul_pkt_info_s *)(&((dsm_ptr)->app_ptr)))->ip_version = ip_ver; \
  ((ipa_ipfltr_ul_pkt_info_s *)(&((dsm_ptr)->app_ptr)))->pkt_len = len ; \
  ((ipa_ipfltr_ul_pkt_info_s *)(&((dsm_ptr)->app_ptr)))->xlat = uCP; \
} while (0) 

/*! @brief Macro to get the UL packet IP version tag from a DSM ptr 
  0 - IPV4, 1- IPV6 (Type: ipa_ip_type_e) */
#define IPA_IPFLTR_GET_UL_PKT_INFO_IP_VERSION(dsm_ptr) \
   ((dsm_ptr != NULL) ? \
    (((ipa_ipfltr_ul_pkt_info_s *)(&((dsm_ptr)->app_ptr)))->ip_version) : 3)

/*! @brief Macro to get the UL packet Length tag from a DSM ptr */
#define IPA_IPFLTR_GET_UL_PKT_INFO_LENGTH(dsm_ptr) \
   ((dsm_ptr != NULL) ? \
    (((ipa_ipfltr_ul_pkt_info_s *)(&((dsm_ptr)->app_ptr)))->pkt_len) : 0)

/*! @brief Macro to get the UL uCP_bit tag from a DSM ptr */
#define IPA_IPFLTR_GET_UL_PKT_INFO_XLAT(dsm_ptr) \
   ((ipa_ipfltr_ul_pkt_info_s *)(&((dsm_ptr)->app_ptr)))->xlat 
/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*! @brief enum: Rule Types that DS can apply */
typedef enum
{
  /*! UL Static IPV4 TTL filter rules */
  IPA_IPFLTR_RULE_UL_STATIC_TTL,

  /*! UL Static rules for CLAT NAT PDN to be run before Frag filters */
  IPA_IPFLTR_RULE_UL_CLAT_STATIC_PRE_FRAG,

  /*! UL Static IPV4 Frag filter rule for CLAT NAT PDN */
  IPA_IPFLTR_RULE_UL_CLAT_STATIC_FRAG,

  /*! UL Static IPv4 fragment Rule: Applied in UL Filter Table,
      Packets will be captured as exception and handled in IPA driver */
  IPA_IPFLTR_RULE_UL_STATIC_FRAG,

  IPA_IPFLTR_RULE_UL_STATIC_IPV4_FRAG = IPA_IPFLTR_RULE_UL_STATIC_FRAG,

  /*! UL Static Local Consumption Rules: Applied in UL Filter Table,
      Packets will be routed through UL Routing Table and intended to
      be consumed Locally in PS Stack */
  IPA_IPFLTR_RULE_UL_STATIC_LOCAL,

  /*! UL Static WWAN Consumption Rules: Applied in UL Filter Table,
      Packets will be routed through UL Routing Table and intended to
      be transmitted OTA */
  IPA_IPFLTR_RULE_UL_STATIC_WWAN,

  /*! UL Low latency Rules: Applied in UL Filter Table,
      Packets are intended to be consumed by WWAN Stack */
  IPA_IPFLTR_RULE_UL_STATIC_LOW_LAT,

  /*! UL Static Local Consumption Rules for CLAT NAT PDNs */
  IPA_IPFLTR_RULE_UL_CLAT_STATIC_LOCAL,

  /*! UL QoS Rules: Applied in UL Routing Table,
      Packets are intended to be consumed by WWAN Stack */
  IPA_IPFLTR_RULE_UL_QOS,

  /*! DL Socket Rules: Applied in DL Filter Table,
      Packets are caught as exception and intended to be consumed by
      Socket application */
  IPA_IPFLTR_RULE_DL_SOCKET,

  /*! DL Low latency rules: Applied in DL Filter Table,
      Packets will be routed through DL Routing Table.
      All packets matching these filter
      will trigger the system to go into a low latency mode */
  IPA_IPFLTR_RULE_DL_LOW_LATENCY,

  /*! DL ICMPv6 rule on CLAT PDN */
  IPA_IPFLTR_RULE_DL_CLAT_ICMPV6,

  IPA_IPFLTR_RULE_TYPE_MAX
} ipa_ipfltr_rule_type_e;

/*! @brief enum: IP Type of the Filter/Routing Rules */
typedef enum
{
  IPA_IPFLTR_IPV4,
  IPA_IPFLTR_IPV6,
  IPA_IPFLTR_NUM_IP_TYPES
} ipa_ipfltr_ip_type_e;

/*! @brief enum: Table Type of the Filter/Routing Rules */
typedef enum
{
  IPA_IPFLTR_TBL_TYPE_UL,
  IPA_IPFLTR_TBL_TYPE_DL
} ipa_ipfltr_tbl_type_e;

/*! @brief enum: Result of adding equation to a Rule */
typedef enum
{
  IPA_IPFLTR_SUCCESS,
  IPA_IPFLTR_INSUFFICIENT_SPACE,
  IPA_IPFLTR_TOO_MANY_EQNS_OF_THAT_TYPE
} ipa_ipfltr_eq_result_e;

/*! @brief enum: Data Packet Type */
typedef enum
{
  /*! DNS packet */
  IPA_DATA_PKT_TYPE_DNS,
  /*! TCP Syn HTTP or HTTPS packet */
  IPA_DATA_PKT_TYPE_TCP_SYN_HTTP,
  IPA_DATA_PKT_TYPE_MAX
} ipa_data_pkt_type_e;

/*! @brief Struct: UL packet info tag used to pass packet len & IP version */
typedef struct
{
  /*! IP version of packet. 0 - IPV4, 1- IPV6 (Type: ipa_ip_type_e) */
  uint32 ip_version : 2;
  /*! uCP_bit. To indeicate whether the packet has undergone translation */
  uint32 xlat       : 1;
  /*! Reserved for future use */
  uint32 reserved   : 13;
  /*! Length of the IP packet */
  uint32 pkt_len    : 16;
} ipa_ipfltr_ul_pkt_info_s;

/*! @brief Rule handle used to identify a rule */
typedef void* ipa_ipfltr_rule_handle_t;

/*! @brief Callback function signature to be called by the driver once we detect 
  the start/end of low latency traffic */
typedef void (*ipa_ipfltr_low_lat_traffic_ind_cb_type)
(
  /*! Index of the filter that triggered this low latency callback */
  uint32  filter_result
);

/*! @brief Callback function signature to be called by the driver when it 
    encounters a data packet of types specified in ipa_data_pkt_type_e */
typedef void (*ipa_ipfltr_datapath_ind_cb_type)
(
  /*! pkt_type encountered */
  ipa_data_pkt_type_e  pkt_type,
  /*! ip version of pkt */
  ipa_ipfltr_ip_type_e ip_type
);

/*! @brief Low Lat filter idx, this idx is mapped to low_lat_fltr_idx in 
 * ipa_ipfltr_dl_lookup_pkt_low_lat() and ipa_ipfltr_ul_lookup_pkt_low_lat()
 * This order is determined by order of rules committed in 
 * ipa_ipfltri_enable_low_lat_prioritization() */
typedef enum
{
  IPA_IPFLTR_LOW_LAT_FLTR_IDX_V4_REPLY,
  IPA_IPFLTR_LOW_LAT_FLTR_IDX_V4_ECHO,
  IPA_IPFLTR_LOW_LAT_FLTR_IDX_V6_REPLY,
  IPA_IPFLTR_LOW_LAT_FLTR_IDX_V6_ECHO
} ipa_ipfltr_low_lat_fltr_idx_e;

/*==============================================================================

  FUNCTION:  ipa_ipfltr_log_all_filters

==============================================================================*/
/*!
  @brief
    Log all the filters that are configured in IPA-HW
    
  @return None
*/
/*============================================================================*/
void ipa_ipfltr_log_all_filters();


/*==============================================================================

  FUNCTION:  ipa_ipfltr_alloc_rule_handle

==============================================================================*/
/*!
  @brief
  Allocate a rule memory to add filter/routing equation to generate a rule

  @return NULL when no rule is available, else a handle to the allocated
          rule.
*/
/*============================================================================*/
ipa_ipfltr_rule_handle_t ipa_ipfltr_alloc_rule_handle
(
  ipa_ipfltr_rule_type_e rule_type,
  ipa_ipfltr_ip_type_e   ip_type,
  uint32                 rule_result,
  uint32                 usr_data
);

/*==============================================================================

  FUNCTION:  ipa_ipfltr_add_meq_32

==============================================================================*/
/*!
  @brief
  Adds a Mask Equal 32 rule to the filter rule set.

  This rule will cause the IPA to perform the following comparison:

  <code>
  value == (*(uint32)((((uint8*)pkt_ptr) + offset)) & mask;
  </code>

  To apply the added rules call ipa_ipfltr_commit_rules() to update the
  rules in the IPA hardware.

  @return IPA_IPFLTR_SUCCESS upon success.
*/
/*============================================================================*/
ipa_ipfltr_eq_result_e ipa_ipfltr_add_meq_32
(
  /*! The rule set to add the rule to. */
  ipa_ipfltr_rule_handle_t rule_handle,
  /*! The offset in bytes to use in the rule */
  uint8 offset,
  /*! The mask to use in the rule */
  uint32 mask,
  /*! The value to compare against after masking */
  uint32 value
);

/*==============================================================================

  FUNCTION:  ipa_ipfltr_add_meq_128

==============================================================================*/
/*!
  @brief
  Adds a Mask Equal 128 rule to the filter rule set.

  This rule will cause the IPA to perform the following comparison in
  pseudo code:

  <code>
  value == (*(uint128)((((uint8*)pkt_ptr) + offset)) & mask;
  </code>

  To apply the added rules call ipa_ipfltr_commit_rules() to update the
  rules in the IPA hardware.

  @return IPA_IPFLTR_SUCCESS upon success.
*/
/*============================================================================*/
ipa_ipfltr_eq_result_e ipa_ipfltr_add_meq_128
(
  /*! The rule set to add the rule to. */
  ipa_ipfltr_rule_handle_t rule_handle,
  /*! The offset in bytes to use in the rule */
  uint8 offset,
  /*! Pointer to the mask to use in the rule, the bytes shall be ordered most
   significant byte of the mask in mask[0] -> least significant byte in mask[15]
   */
  uint8 mask[IPA_IPFLTR_NUM_MEQ_128_BYTES],
  /*! Pointer to the value to compare against after masking
    The bytes shall be ordered most significant byte of the mask in value[0]
    -> least significant byte in value[15].
   */
  uint8 value[IPA_IPFLTR_NUM_MEQ_128_BYTES]
);

/*==============================================================================

  FUNCTION:  ipa_ipfltr_add_ihl_offset_eq_32

==============================================================================*/
/*!
  @brief
  Adds an IHL Equal 32 rule to the filter rule set. This rule will add
  the header length read from the ip header to the offset.

  This rule will cause the IPA to perform the following comparison:

  <code>
  value == (*(uint32)((((uint8*)pkt_ptr) + ip_header_length + offset))
  </code>

  To apply the added rules call ipa_ipfltr_commit_rules() to update the
  rules in the IPA hardware.

  @return IPA_IPFLTR_SUCCESS upon success.
*/
/*============================================================================*/
ipa_ipfltr_eq_result_e ipa_ipfltr_add_ihl_offset_eq_32
(
  /*! The rule set to add the rule to. */
  ipa_ipfltr_rule_handle_t rule_handle,
  /*! The offset in bytes to use in the rule */
  uint8 offset,
  /*! The value to compare against after masking */
  uint32 value
);

/*==============================================================================

  FUNCTION:  ipa_ipfltr_add_ihl_offset_eq_16

==============================================================================*/
/*!
  @brief
  Adds an IHL offset 16 rule to the filter rule set. This rule will add
  the header length read from the ip header to the offset.

  This rule will cause the IPA to perform the following comparison:

  <code>
  value == (*(uint16)((((uint8*)pkt_ptr) ip_header_length + offset)))
  </code>

  To apply the added rules call ipa_ipfltr_commit_rules() to update the
  rules in the IPA hardware.

  @return IPA_IPFLTR_SUCCESS upon success.
*/
/*============================================================================*/
ipa_ipfltr_eq_result_e ipa_ipfltr_add_ihl_offset_eq_16
(
  /*! The rule set to add the rule to. */
  ipa_ipfltr_rule_handle_t rule_handle,
  /*! The offset in bytes to use in the rule */
  uint8 offset,
  /*! The value to compare against */
  uint16 value
);

/*==============================================================================

  FUNCTION:  ipa_ipfltr_add_ihl_offset_range_eq_16

==============================================================================*/
/*!
  @brief
  Adds an IHL offset range 16 rule to the filter rule set. This rule will add
  the header length read from the ip header to the offset.

  This rule will cause the IPA to perform the following comparison:

  <code>
  ( range_low <= (*(uint16)((((uint8*)pkt_ptr) ip_header_length + offset))) &&
  (*(uint16)((((uint8*)pkt_ptr) ip_header_length + offset))) <= range_high )
  </code>

  To apply the added rules call ipa_ipfltr_commit_rules() to update the
  rules in the IPA hardware.

  @return IPA_IPFLTR_SUCCESS upon success.
*/
/*============================================================================*/
ipa_ipfltr_eq_result_e ipa_ipfltr_add_ihl_offset_range_eq_16
(
  /*! The rule set to add the rule to. */
  ipa_ipfltr_rule_handle_t rule_handle,
  /*! The offset in bytes to use in the rule */
  uint8 offset,
  /*! The low threshold of the range */
  uint16 range_low,
  /*! The high threshold of the range */
  uint16 range_high
);

/*==============================================================================

  FUNCTION:  ipa_ipfltr_add_tos_eq

==============================================================================*/
/*!
  @brief
  Checks that the type of service field in the ipv4 header matches the
  specified value.

  To apply the added rules call ipa_ipfltr_commit_rules() to update the
  rules in the IPA hardware.

  @return IPA_IPFLTR_SUCCESS upon success.
*/
/*============================================================================*/
ipa_ipfltr_eq_result_e ipa_ipfltr_add_tos_eq
(
  /*! The rule set to add the rule to. */
  ipa_ipfltr_rule_handle_t rule_handle,
  /*! The value to compare against */
  uint8 value
);

/*==============================================================================

  FUNCTION:  ipa_ipfltr_add_tc_eq

==============================================================================*/
/*!
  @brief
  Checks that the traffic class field of the ipv4 header matches
  the specified value.

  To apply the added rules call ipa_ipfltr_commit_rules() to update the
  rules in the IPA hardware.

  @return IPA_IPFLTR_SUCCESS upon success.
*/
/*============================================================================*/
ipa_ipfltr_eq_result_e ipa_ipfltr_add_tc_eq
(
  /*! The rule set to add the rule to. */
  ipa_ipfltr_rule_handle_t rule_handle,
  /*! The value to compare against */
  uint8 value
);

/*==============================================================================

  FUNCTION:  ipa_ipfltr_add_protocol_eq

==============================================================================*/
/*!
  @brief
  Checks that the protocol field of the ipv6 header compares matches
  specified value.

  To apply the added rules call ipa_ipfltr_commit_rules() to update the
  rules in the IPA hardware.

  @return IPA_IPFLTR_SUCCESS upon success.
*/
/*============================================================================*/
ipa_ipfltr_eq_result_e ipa_ipfltr_add_protocol_eq
(
  /*! The rule set to add the rule to. */
  ipa_ipfltr_rule_handle_t rule_handle,
  /*! The value to compare against */
  uint8 value
);

/*==============================================================================

  FUNCTION:  ipa_ipfltr_add_flow_eq

==============================================================================*/
/*!
  @brief
  Checks that the flow field of the ipv6 header matches the
  specified value.

  To apply the added rules call ipa_ipfltr_commit_rules() to update the
  rules in the IPA hardware.

  @return IPA_IPFLTR_SUCCESS upon success.
*/
/*============================================================================*/
ipa_ipfltr_eq_result_e ipa_ipfltr_add_flow_eq
(
  /*! The rule set to add the rule to. */
  ipa_ipfltr_rule_handle_t rule_handle,
  /*! The value to compare against */
  uint32 value
);

/*==============================================================================

  FUNCTION:  ipa_ipfltr_add_ihl_meq_32

==============================================================================*/
/*!
  @brief
  Adds an IHL  Mask Equal 32 rule to the filter rule set. This rule will add
  the header length read from the ip header to the offset.

  This rule will cause the IPA to perform the following comparison:

  <code>
  value == (*(uint32)((((uint8*)pkt_ptr) + ip_header_length + offset)) & mask
  </code>

  To apply the added rules call ipa_ipfltr_commit_rules() to update the
  rules in the IPA hardware.

  @return IPA_IPFLTR_SUCCESS upon success.
*/
/*============================================================================*/
ipa_ipfltr_eq_result_e ipa_ipfltr_add_ihl_meq_32
(
  /*! The rule set to add the rule to. */
  ipa_ipfltr_rule_handle_t rule_handle,
  /*! The offset in bytes to use in the rule */
  uint8 offset,
  /*! The mask to use in the rule */
  uint32 mask,
  /*! The value to compare against after masking */
  uint32 value
);

/*==============================================================================

  FUNCTION:  ipa_ipfltr_add_metadata_meq_32

===========================================================================*/
/*!
  @brief
  Adds Metadata Mask Equal 32 rule to the rule.

  This rule will cause the IPA to perform the following comparison:

  <code>
  value == (*(uint32*)metadata_ptr & mask);
  </code>

  To apply the added rules call ipa_ipfltr_commit_rules() to update the
  rules in the IPA hardware.

  @return IPA_IPFLTR_SUCCESS upon success.
*/
/*===========================================================================*/
ipa_ipfltr_eq_result_e ipa_ipfltr_add_metadata_meq_32
(
  /*! The rule set to add the rule to. */
  ipa_ipfltr_rule_handle_t rule_handle,
  /*! The mask to use in the rule */
  uint32 mask,
  /*! The value to compare against after masking */
  uint32 value
);

/*==============================================================================

  FUNCTION:  ipa_ipfltr_add_ipv4_frag_eq

===========================================================================*/
/*!
  @brief
  Adds is_IPv4_Fragment_Present equation to the rule.

  To apply the added rules call ipa_ipfltr_commit_rules() to update the
  rules in the IPA hardware.

  @return IPA_IPFLTR_SUCCESS upon success.
*/
/*===========================================================================*/
#define ipa_ipfltr_add_ipv4_frag_eq ipa_ipfltr_add_frag_eq

ipa_ipfltr_eq_result_e ipa_ipfltr_add_frag_eq
(
  /*! The rule to add the equation to */
  ipa_ipfltr_rule_handle_t rule_handle
);

/*=============================================================================

  FUNCTION:  ipa_ipfltr_commit_rules

=============================================================================*/
/*!
  @brief
  Commits the changes to the filter and routing tables in the IPA hardware.
 
  @return Following can be possible values in the return code:
  @retval IPA_SUCCESS, closing inprogress rules succeeded
  @retval IPA_ERR_IPFLTR_RULE_TBL_PENDING_ACTIVATION, if table is pending
          activation and rules were moved to commit_done state
  @retval IPA_ERR_FLTR_TBL_OVERFLOW, filter table overflow resulted from
          closing the given rules.
  @retval IPA_ERR_INVALID_ARGUMENT, if arguments to the API is/are invalid
*/
/*===========================================================================*/
ipa_err_code_e ipa_ipfltr_commit_rules
(
  uint16                   num_rules,
  ipa_ipfltr_rule_handle_t *handle_array
);

/*==============================================================================

  FUNCTION:  ipa_ipfltr_remove_rule

==============================================================================*/
/*!
  @brief
  This is an external interface API. It removes a rule corresponding with the
  rule handle.

  @return
  IPA_SUCCESS, if rule could be removed successfully
  IPA_ERR_IPFLTR_REMOVE_FAIL, if rule could not be removed
*/
/*============================================================================*/
ipa_err_code_e ipa_ipfltr_remove_rule
(
  ipa_ipfltr_rule_handle_t handle
);

/*=============================================================================

  FUNCTION:  IPA_IPFLTR_REMOVE_RULES

=============================================================================*/
/*!
  @brief
  This is an external interface API. It removes rules corresponding with the
  array of rule handles.

  @return
  IPA_SUCCESS, if rules could be removed successfully
  IPA_ERR_IPFLTR_REMOVE_FAIL, if rules could not be removed
*/
/*===========================================================================*/
ipa_err_code_e ipa_ipfltr_remove_rules
(
  uint16                   num_rules,
  ipa_ipfltr_rule_handle_t handle_arr[]
);

/*=============================================================================

  FUNCTION:  IPA_IPFLTR_ENABLE_TCP_ACK_PRIORITIZATION

=============================================================================*/
/*!
  @brief
  This is an external interface API. This API installs TCP ack filters
  for activating TCP ACK prioritization rule 

  @return
  IPA_SUCCESS, if rules could be added successfully
  IPA_ERR_FAIL, if rules could not be added
*/
/*===========================================================================*/
ipa_err_code_e ipa_ipfltr_enable_tcp_ack_prioritization
(
  void
);

/*=============================================================================

  FUNCTION:  IPA_IPFLTR_FLTR_ADD_LOW_LATENCY_RULE

=============================================================================*/
/*!
  @brief
  This is an external interface API. This API installs low latency filter
  rule based on ToS/DSCP field

  @return
  IPA_SUCCESS, if rules could be added successfully
  IPA_ERR_FAIL, if rules could not be added
*/
/*===========================================================================*/
ipa_err_code_e ipa_ipfltr_fltr_add_low_latency_rule
(
  void
);

/*=============================================================================

  FUNCTION:  IPA_IPFLTR_FLTR_DEL_LOW_LATENCY_RULE

=============================================================================*/
/*!
  @brief
  This is an external interface API. This API removes low latency filter
  rule based on ToS/DSCP field

  @return
  IPA_SUCCESS, if rules could be removed successfully
  IPA_ERR_FAIL, if rules could not be removed
*/
/*===========================================================================*/
ipa_err_code_e ipa_ipfltr_fltr_del_low_latency_rule
(
  void
);

/*=============================================================================

  FUNCTION:  IPA_IPFLTR_APPLY_UL_STATIC_RULES_DONE

=============================================================================*/
/*!
  @brief
  This is an external API to indicate to IPA driver that all UL static rules
  have been applied.
  
  @return
  None
*/
/*===========================================================================*/
void ipa_ipfltr_apply_ul_static_rules_done
(
  void
);

/*=============================================================================

  FUNCTION:  IPA_IPFLTR_ADD_UL_IPV6_PREFIX_RULE

=============================================================================*/
/*!
  @brief
  This is an external interface API. This API installs UL Source IPv6 Prefix
  rule for filtering packets that match a given IPv6 Prefix
 
  @param
  stream_id, SIO stream id on which the v6 prefix rule needs to be applied.
  prefix_len, lenght of prefix in number of bits.
  prefix, prefix value in array of bytes in network order.
  rule_result, value to be passed in the packets which will match ths rule.
 
  @return
  handle, if rule could be added successfully
  NULL, if rule could not be added
*/
/*===========================================================================*/
ipa_ipfltr_rule_handle_t ipa_ipfltr_add_ul_ipv6_prefix_rule
(
  sio_stream_id_type stream_id,
  /*! prefix length in num of bits */
  uint8              prefix_len,
  /*! prefix value in byte array */
  uint8              prefix[16],
  /*! rule_result to provide when this rule matches a pkt */
  uint32             rule_result
);

/*=============================================================================

  FUNCTION:  IPA_IPFLTR_ADD_UL_DEST_IPV6_PREFIX_RULE

=============================================================================*/
/*!
  @brief
  This is an external interface API. This API installs UL Destination IPv6
  Prefix rule for filtering packets that match a given IPv6 Prefix
 
  @param
  stream_id, SIO stream id on which the v6 prefix rule needs to be applied.
  prefix_len, lenght of prefix in number of bits.
  prefix, prefix value in array of bytes in network order.
  rule_result, value to be passed in the packets which will match ths rule.
 
  @return
  handle, if rule could be added successfully
  NULL, if rule could not be added
*/
/*===========================================================================*/
ipa_ipfltr_rule_handle_t ipa_ipfltr_add_ul_dest_ipv6_prefix_rule
(
  sio_stream_id_type stream_id,
  /*! prefix length in num of bits */
  uint8              prefix_len,
  /*! prefix value in byte array */
  uint8              prefix[16],
  /*! rule_result to provide when this rule matches a pkt */
  uint32             rule_result
);

/*=============================================================================

  FUNCTION:  IPA_IPFLTR_REGISTER_LOW_LAT_IND_CB

=============================================================================*/
/*!
  @brief
  This is a DS only interface API. This API registers a callback function
  with the IPA driver which will be invoked when any of the packets
  matching LOW_LATENCY_FILTERS are detected in the driver.
 
  @param
  low_latency_cb, Callback function pointer to be invoked when low latency
                  traffic is detected
 
  @return
  IPA_SUCCESS, if callback was successfully registered
  IPA_ERR_INVALID_ARGUMENT, if callback registeration failed
*/
/*===========================================================================*/
ipa_err_code_e ipa_ipfltr_register_low_latency_ind_cb
(
  ipa_ipfltr_low_lat_traffic_ind_cb_type low_lat_ind_cb_fn
);

/*=============================================================================

  FUNCTION:  IPA_IPFLTR_REGISTER_DATAPATH_CB

=============================================================================*/
/*!
  @brief
  This is a DS only interface API. This API registers a callback function
  with the IPA driver which will be invoked when it encounters any packet
  of type specified in ipa_data_pkt_type_e
 
  @param
  datapath_cb, Callback function pointer to be invoked when specified packet
  type is encountered
 
  @return
  IPA_SUCCESS, if callback was successfully registered
  IPA_ERR_INVALID_ARGUMENT, if callback registeration failed
*/
/*===========================================================================*/
ipa_err_code_e ipa_ipfltr_register_datapath_ind_cb
(
  ipa_ipfltr_datapath_ind_cb_type datapath_ind_cb_fn
);

#endif
