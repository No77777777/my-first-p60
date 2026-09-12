/*!
  @file
  ipa.h

  @brief
  This file contains definitions of all type definitions and APIs available
  to external modules.


*/
/*===========================================================================

  Copyright (c) 2013-18, 2021-22 QUALCOMM Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/dataipa.mpss/3.2/api/ipa.h#4 $
$DateTime: 2022/12/12 07:57:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/12/22   ss      CR#3179581 DS registers call back function with IPA to 
                   know about the hw_init status
01/31/22   mk      CR #3114853 Add new API, that can be used by LTE to request
                   for fast devote of IPA clocks
09/22/21   zq      Changes to send QMAP ACK for UL_DATA_SENT_CNF and CMD_TYPE_REQ
08/28/18   yy      CR#2304699 Added API ipa_set_dl_burst_marker_control
06/14/18   gk      CR2258863: Return different error code when there is no 
                   change in bearer state
08/25/17   vm      CR#2024665 TLB-A Loopback Changes
06/28/17   gn      CR2050609:IPA Dev changes for TS Rewrite FR40402
04/24/17   pgm     CR1093218:New DS bearer reg interface
06/17/16   ns      CR1007396: HW replication interface changes.
03/15/16   pgm     Adding per pipe aggr info in endp mapping.
09/01/15   ag      CR887071 Cksum Offload WA + QMAPv4 handling.
08/03/15   ag      CR881000 Support flow control on offload data path.
07/14/15   ag      CR816817 Tethering/Quota Stats.
07/09/15   ag      CR861176 API to get bearer proc_state
05/18/15   ag      CR839981 Query UL Pipe stats.
02/04/15   SaCh    Adding subnet_addr in global clat context.
12/18/14   tz      Set IP version on DS enqueued DSM items.
11/10/14   tz      DL checksum offload for embedded data support.
07/25/14   mi      CLAT inteface changes. Addedsupport for CLAT global context.
07/07/14   SaCh    Aligning the prefix arrays to 16 byte boundary.
07/23/14   rp      Accelerated DPL changes 
06/19/14   rp      IPA_STATS log packet version 2
04/02/14   rp      DS UL Stats interface additon
06/23/14   pgm     Support for adding guaranteed padding bytes in DL QMAP 
                   aggr case.
05/30/14   pgm     CLAT TOS/TC translation config support.
05/29/14   SaCh    Added CLAT config complete event callback functionality.
04/28/14   pgm     IPA CLAT API support.
04/02/14   rp      DS MH UL Stats interface additon
03/11/14   pgm     API to set flow control on phys stream in MBIM cases.
01/23/14   rp      Changes to support QXDM log/event packets
01/13/14   am      CR#598954:Updating bearer state callback enum.
01/06/14   SaCh    Adding DS Event callback registration interface.
12/13/13   SaCh    Changes to accept action in SIO-config mapping API
11/12/13   SaCh    Introduced error_code for pending_remote_activation of UL
                   filter rules 
10/29/13   rp      Adding support for aggregation params per ep type
10/08/13   rp      Adding support for RNDIS aggregation
10/01/13   rp      Adding support for UL CFM/SIO flow control
10/01/13   am      Moved APIs used outside Datamodem in datamodem/api.
09/09/13   SaCh    Adding IPA_SIO_HDR_TYPE_QMAP_V3 as a valid hdr/aggr type
08/27/13   SaCh    Adding bearer_tech as param in register_bearer API.
08/21/13   rp      Adding support for cb function invocation after endp cfg  
08/15/13   SaCh    Changing the interface to register bearer with IPA.
08/02/13   rp      Adding support for QMAP cmd handling
04/19/13   SaCh    Initial version
===========================================================================*/

#ifndef IPA_H
#define IPA_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include <comdef.h>
#include <dsm.h>
#include <sio.h>
#include "ipa_api.h"

/*===========================================================================

                              MACROS

===========================================================================*/
/*! @brief Ethernet Header Length (in bytes) */
#define IPA_SIO_ETHERNET_HDR_LEN   14
/*! @brief MBIM Header Length (in bytes) */
#define IPA_SIO_MBIM_HDR_LEN  8
/*! @brief QMAP Header Length (in bytes) */
#define IPA_SIO_QMAP_HDR_LEN  4
/*! @brief INVALID MUX ID */
#define IPA_SIO_INVALID_MUX_ID 0
/*===========================================================================

                     COMMON DEFINITIONS and TYPES

===========================================================================*/
/*! @brief IPA return error codes */
typedef enum
{
  /*! Common Error Codes */
  IPA_SUCCESS = 0,
  IPA_ERR_INVALID_ARGUMENT,
  IPA_ERR_LOGICAL_SIO_STREAM_UNKNOWN,
  IPA_ERR_PHYSICAL_SIO_STREAM_UNKNOWN,
  IPA_ERR_PERIPHERAL_PIPES_NOT_FOUND,
  IPA_ERR_NOT_SUPPORTED,
  /*! UL Error Codes */
  IPA_ERR_UL_AGGR_HDR_TYPE_NOT_SUPPORTED,
  IPA_ERR_UL_AGGR_PARAMS_INCORRECT,
  /*! DL Error Codes */
  IPA_ERR_DL_AGGR_HDR_TYPE_NOT_SUPPORTED,
  IPA_ERR_DL_AGGR_PARAMS_INCORRECT,
  IPA_ERR_DL_HDR_TYPE_NOT_SUPPORTED,
  /*! IP Filtering Error Codes */
  IPA_ERR_IPFLTR_COMMIT_FAIL,
  IPA_ERR_IPFLTR_RULE_TBL_PENDING_ACTIVATION,
  IPA_ERR_IPFLTR_RULE_TBL_PENDING_REMOTE_ACTIVATION,
  IPA_ERR_IPFLTR_REMOVE_FAIL,
  IPA_ERR_FLTR_TBL_OVERFLOW,
  /*! Generic error */
  IPA_ERR_FAIL,
  IPA_ERR_BEARER_STATE_NO_CHANGE,
  IPA_ERR_SSR_INPROGRESS,
  IPA_ERR_CODE_MAX
} ipa_err_code_e;

/*! @brief enum: IP type */
typedef enum
{
  IPA_IP_TYPE_IPV4 = 0,
  IPA_IP_TYPE_IPV6,
  IPA_IP_TYPE_MAX
} ipa_ip_type_e;

/*! @brief enum: to indicate Flow Action or current Flow State */
typedef enum
{
  IPA_FLOW_STATE_DISABLE = 0,
  IPA_FLOW_STATE_ENABLE
} ipa_flow_state_e;

/*! @brief enum: Bearer Event */
typedef enum
{
  IPA_BEARER_EVENT_DELETED = 0,
  IPA_BEARER_EVENT_ADDED
} ipa_bearer_event_e;

/*! @brief enum: DL Bridge State */
typedef enum
{
  IPA_DL_BRIDGE_STATE_UNBRIDGE = 0,
  IPA_DL_BRIDGE_STATE_BRIDGE
} ipa_wan_dl_bridge_state_e;

/*! @brief enum: bearer DL processing state */
typedef enum
{
  IPA_WAN_BEARER_PROC_STATE_ACTIVE = 0,
  IPA_WAN_BEARER_PROC_STATE_SUSPEND
} ipa_wan_bearer_proc_state_e;

/*===========================================================================

             IPA Driver <-> SIO Interface DEFINITIONS and TYPES

===========================================================================*/

/*! @brief enum: Header Protocols type */
typedef enum
{
  IPA_SIO_HDR_TYPE_NONE = 0,
  IPA_SIO_HDR_TYPE_ETHERNET,
  IPA_SIO_HDR_TYPE_QMAP,
  IPA_SIO_HDR_TYPE_QMAP_V2,
  IPA_SIO_HDR_TYPE_QMAP_V3,
  IPA_SIO_HDR_TYPE_QMAP_V4,  
  IPA_SIO_HDR_TYPE_QCNCM,
  IPA_SIO_HDR_TYPE_MBIM,
  IPA_SIO_HDR_TYPE_DIRECT_DMA,
  IPA_SIO_HDR_TYPE_RNDIS,
  IPA_SIO_HDR_TYPE_MAX
} ipa_sio_hdr_type_e;


/*! @brief enum: to indicate DL burst marker enabled or disabled */
typedef enum
{
  IPA_DL_BURST_MARKER_DISABLE = 0,
  IPA_DL_BURST_MARKER_ENABLE
} ipa_dl_burst_marker_control_state_e;


/*! @brief struct: QMAP Aggregation Parameters */
typedef struct
{
  uint32 max_aggr_bytes;
  uint16 max_aggr_pkts;
} ipa_sio_qmap_params_s;

/*! @brief struct: QMAPv2 Aggregation Parameters */
typedef  ipa_sio_qmap_params_s ipa_sio_qmapv2_params_t;

/*! @brief struct: QMAPv3 Aggregation Parameters */
typedef  ipa_sio_qmap_params_s ipa_sio_qmapv3_params_t;

/*! @brief struct: QMAPv4 Aggregation Parameters */
typedef  ipa_sio_qmap_params_s ipa_sio_qmapv4_params_t;

/* -------
   Function     : ipa_dl_req_devote_cb_type
   Description  : Function prototype of CB fn to be triggered by L2 to request 
                  clock devote with IPA when possible.
   Parameters   : None
   Returns      : None
   -------*/

typedef void (*ipa_dl_req_devote_cb_type)( void );


/*! @brief struct: MBIM Aggregation/Header Parameters */
typedef struct
{
  uint32 max_aggr_bytes;
  uint32 ncm_sig;
  uint32 ndp_sig;
  uint16 max_aggr_pkts;
} ipa_sio_mbim_params_s;

/*! @brief struct: QCNCM Aggregation Parameters */
typedef ipa_sio_mbim_params_s ipa_sio_qcncm_params_t;

/*! @brief struct: Ethernet Aggregation/Header Parameters */
typedef struct
{
  uint32 max_aggr_bytes;
  uint16 max_aggr_pkts;
  uint8  ipv4_hdr_present; /* Flag indicating IPv4 Ethernet Header is present or not */
  uint8  ipv6_hdr_present; /* Flag indicating IPv6 Ethernet Header is present or not */
  uint8  ipv4_hdr[IPA_SIO_ETHERNET_HDR_LEN]; /* IPv4 Ethernet Header in Network Byte order */
  uint8  ipv6_hdr[IPA_SIO_ETHERNET_HDR_LEN]; /* IPv6 Ethernet Header in Network Byte order */
} ipa_sio_ethernet_params_s;

/*! @brief union: Aggregation/Header Parameters */
typedef union
{
  ipa_sio_qmap_params_s     qmap;
  ipa_sio_qmapv3_params_t   qmapv2;
  ipa_sio_qmapv3_params_t   qmapv3; 
  ipa_sio_qmapv4_params_t   qmapv4;
  ipa_sio_mbim_params_s     mbim;
  ipa_sio_qcncm_params_t    qcncm;
  ipa_sio_ethernet_params_s ether;
} ipa_sio_config_params_u;

/*! @brief struct: Downlink aggregation/header Configuration */
typedef struct
{
  ipa_sio_config_params_u params;
  ipa_sio_hdr_type_e      hdr_type;
  /* Guaranteed trailer byte length */
  uint32                  trailer_pad_len; 
} ipa_sio_dl_config_s;

/*! @brief struct: Uplink aggregation/header Configuration */
typedef struct
{
  ipa_sio_config_params_u params;
  ipa_sio_hdr_type_e      hdr_type;
  boolean                 is_qos_present;     
} ipa_sio_ul_config_s;

/*! @brief struct: SIO endpoint Configuration */
typedef struct
{
  ipa_sio_dl_config_s dl_config;
  ipa_sio_ul_config_s ul_config;
} ipa_sio_endpoint_config_s;

typedef enum
{
  IPA_SIO_CONFIG_ACTION_ADD = 0,
  IPA_SIO_CONFIG_ACTION_ETHERNET_HDR_UPDATE
} ipa_sio_config_action_e;

/*! @brief struct: SIO Configuration mapping received from DS */
typedef struct
{
  uint8                     mux_id;
  uint8                     mux_override;
  sio_stream_id_type        physical_sio_stream;
  ipa_sio_endpoint_config_s *aggr_hdr_config_ptr;
  dsm_watermark_type        *sio_tx_wm_ptr;
  dsm_watermark_type        *sio_rx_wm_ptr;
} ipa_sio_config_map_s;

/*! @brief struct: Peripheral Endpoint mapping */
typedef struct 
{
  uint8   per_cons_pipe_id;
  uint8   per_prod_pipe_id;
  uint32  per_ep_id;
  uint32  ul_fifo_size;
  uint32  dl_fifo_size;
  uint32  dl_buf_size;
  dsm_watermark_type  *phy_tx_wm_ptr;
  dsm_watermark_type  *phy_rx_wm_ptr;
} ipa_sio_per_ep_map_s;

/*! @brief struct: SIO unidirectional quota statistics */
typedef struct
{
  uint64 num_pkts;
  uint64 num_bytes;
} ipa_sio_unidir_quota_stats_s;

/*! @brief struct: UL SIO unidirectional traffic statistics */
typedef struct
{
  uint64 total_num_ipv4_pkts;
  uint64 total_num_ipv4_bytes;
  uint64 total_num_ipv6_pkts;
  uint64 total_num_ipv6_bytes;
  ipa_sio_unidir_quota_stats_s quota;  
} ipa_sio_unidir_ul_stats_s;

/*! @brief struct: DL SIO unidirectional traffic statistics */
typedef struct
{
  uint64 total_num_pkts;
  uint64 total_num_bytes;
  ipa_sio_unidir_quota_stats_s quota;
} ipa_sio_unidir_dl_stats_s;

/*! @brief struct: SIO bidirectional traffic statistics */
typedef struct
{
  ipa_sio_unidir_ul_stats_s ul_stats; 
  ipa_sio_unidir_dl_stats_s dl_stats;
} ipa_sio_bidir_stats_s;

/*! @brief Callback function signature to be called by the driver once the endpoint
  configuration corresponding to a given logical SIO port is complete */
typedef void (*ipa_sio_cfg_complete_cb_type)
(
  sio_stream_id_type stream_id,
  uint32             cb_data
);

/* -------
   Function     : ipa_ctl_hw_init_status_cb_type
   Description  : Function prototype of CB fn to be triggered by Data interfaces
                  to get the IPa h/w init status
   Parameters   : None
   Returns      : None
   -------*/

typedef void (*ipa_ctl_hw_init_status_cb_type)( void );


/*=============================================================================

  FUNCTION:  IPA_SIO_SET_PER_EP_MAPPING

=============================================================================*/
/*!
    @brief
    This API is invoked to set the mapping of physical SIO stream to peripheral
    endpoint id and peripheral pipe pair. 
 
    @return
    None
*/
/*===========================================================================*/
ipa_err_code_e  ipa_sio_set_per_ep_mapping
(
  sio_stream_id_type    phys_stream_id,  
  ipa_sio_per_ep_map_s* per_ep_map_ptr
);

/*===========================================================================

  FUNCTION:  ipa_sio_register_ep_config_done_cb

===========================================================================*/
/*!
    @brief
    This API registers a callback function that IPA driver will invoke when
    the endpoint configuration corresponding to a given logical SIO port
    is complete.
 
    @param cfg_complete_cb_fn, Callback function to be registered with the
                               driver
    @param cfg_complete_cb_data, Callback data to be used while invocation
                                 of the cb function
    @return 
    IPA_SUCCESS, when all params passed error checking
    IPA_ERR_INVALID_ARGUMENT, to specify the first error encountered while
                              parsing the parameters
*/ 
/*=========================================================================*/
ipa_err_code_e ipa_sio_register_ep_config_done_cb
(
  ipa_sio_cfg_complete_cb_type  cfg_complete_cb_fn,
  uint32                        cfg_complete_cb_data
);

/*===========================================================================

  FUNCTION:  ipa_sio_set_sio_config_mapping

===========================================================================*/
/*!
    @brief
    This API is invoked by DS to set the configuration on the given logical
    SIO channel.
    This function assumes the caller has allocated the memory for the
    argument and will free the memory when the function returns.
 
    @return
    IPA_SUCCESS, when all params passed error checking
    IPA_ERR_XXX, to specify the first error encountered while parsing the
                 parameters
*/ 
/*=========================================================================*/
ipa_err_code_e ipa_sio_set_sio_config_mapping 
(
  ipa_sio_config_action_e action,
  sio_stream_id_type      logical_stream_id,  
  ipa_sio_config_map_s    *config_map_ptr
);

/*===========================================================================

  FUNCTION:  IPA_SIO_GET_UL_AGGR_PROT_SUPPORT

===========================================================================*/
/*!
    @brief
    This API is invoked by DS to check if the given UL aggregation
    protocol is supported by IPA.
 
    @return
 
    IPA_SUCCESS, when both UL aggr protocol is supported.
    IPA_ERR_UL_AGGR_TYPE_NOT_SUPPORTED, if UL aggr type is not supported.
*/ 
/*=========================================================================*/
ipa_err_code_e  ipa_sio_get_ul_aggr_prot_support
(
  uint32             per_ep_id,
  ipa_sio_hdr_type_e ul_aggr_prot,
  uint8              *max_ul_aggr_pkts,
  uint32             *max_ul_aggr_bytes
);

/*===========================================================================

  FUNCTION:  IPA_SIO_GET_DL_AGGR_PROT_SUPPORT

===========================================================================*/
/*!
    @brief
    This API is invoked by DS to check if the given DL aggregation
    protocol is supported by IPA.
 
    @return
 
    IPA_SUCCESS, when DL aggr protocol is supported.
    IPA_ERR_DL_AGGR_TYPE_NOT_SUPPORTED, if DL aggr type is not supported.
*/ 
/*=========================================================================*/
ipa_err_code_e  ipa_sio_get_dl_aggr_prot_support
(
  uint32             per_ep_id,
  ipa_sio_hdr_type_e dl_aggr_prot,
  uint8              *max_dl_aggr_pkts,
  uint32             *max_dl_aggr_bytes,
  uint32             *trailer_pad_len
);

/*===========================================================================

  FUNCTION:  IPA_SIO_RETRIEVE_TRAFFIC_STATS

=============================================================================*/
/*!
    @brief
    This API is invoked to retrieve per SIO traffic statis from IPA Driver.
    IPA driver will consolidate the UL and DL traffic statistics and return
    them as output parameter. It assumes the caller has allocated the memory
    for the statistics structure parameter.

    @return Following can be possible values in return code:
 
    @retval IPA_SUCCESS, retrieving statistics was successful.
    @retval IPA_ERR_INVALID_ARGUMENT when -
            1. SIO stream_id provided is invalid.
            2. traffic_stats pointer passed to retrieve statistics is NULL.
*/ 
/*===========================================================================*/
ipa_err_code_e  ipa_sio_retrieve_traffic_stats
(
  sio_stream_id_type    sio_stream_id,                          
  ipa_sio_bidir_stats_s *traffic_stats
);

/*=============================================================================

  FUNCTION:  ipa_sio_set_ul_phys_flow_control

=============================================================================*/
/*!
    @brief
    This API is invoked by DS to flow control the UL traffic on a Phys SIO stream
    on which the watermark level has hit high threshold.

    @param phys_stream_id, Phys stream id corresp to phys sio watermark which 
                           needs to be flow controlled.
    @param flow_action,   0 if flow disable is requested, 1 if flow enable is
                          requested

    @return Following can be the possible values in the return code:

    @retval IPA_SUCCESS
    @retval IPA_ERR_LOGICAL_SIO_STREAM_UNKNOWN, when sio_stream id is unknown
    @retval IPA_ERR_UL_FLOW_ALREADY_ENABLED,    when flow_action requests for
                                                enabling the flow and flow is
                                                already enabled on the given
                                                SIO stream.
    @retval IPA_ERR_UL_FLOW_ALREADY_DISABLED,   when flow_action requests for
                                                disabling the flow and flow
                                                is already disabled on the
                                                given SIO stream.
*/
/*===========================================================================*/
ipa_err_code_e ipa_sio_set_ul_phys_flow_control
(
  sio_stream_id_type phys_stream_id,
  ipa_flow_state_e   flow_action
);

/*=============================================================================

  FUNCTION:  IPA_SIO_SET_UL_OFFLOAD_FLOW_CONTROL

=============================================================================*/
/*!
    @brief
    This API is invoked by DS to flow control the UL offloaded traffic on a SIO 
    stream on which the watermark level has hit high threshold. This is 
    currently used only for Rndis/WLAN traffic binds with embedded pipe.

    @param sio_stream_id, stream id corresp to SIO watermark which its tethered
                          pipie needs to be flow controlled
    @param flow_action,   0 if flow disable is requested, 1 if flow enable is
                          requested

    @return Following can be the possible values in the return code:

    @retval IPA_SUCCESS
    @retval IPA_ERR_LOGICAL_SIO_STREAM_UNKNOWN, when sio_stream id is unknown
    @retval IPA_ERR_UL_FLOW_ALREADY_ENABLED,    when flow_action requests for
                                                enabling the flow and flow is
                                                already enabled on the given
                                                SIO stream.
    @retval IPA_ERR_UL_FLOW_ALREADY_DISABLED,   when flow_action requests for
                                                disabling the flow and flow
                                                is already disabled on the
                                                given SIO stream.
*/
/*===========================================================================*/
ipa_err_code_e ipa_sio_set_ul_offload_flow_control
(
  sio_stream_id_type stream_id,
  ipa_flow_state_e   flow_action
);

/*===========================================================================

  FUNCTION:  IPA_SIO_CONFIG_DPL

===========================================================================*/
/*!
    @brief
    This API is invoked by DS to configure accelerated DPL on the given
    SIO port.
    The DPL IID parameters for this SIO port needs to be
    passed to the driver for accelerated DPL

    @param sio_stream_id, SIO stream ID of the logical port
    @param ifname, Interface name associated to this SIO port's DPL IID
    @param num_bytes, Number of DPL bytes to be logged 

    @return Following can be possible values in the return code:

    @retval IPA_SUCCESS, when DPL config for the SIO stream ID is successful.
    @retval IPA_ERR_INVALID_ARGUMENT, when SIO stream ID is invalid
*/ 
/*=========================================================================*/
ipa_err_code_e ipa_sio_config_dpl
(
  sio_stream_id_type  sio_stream_id,
  uint8               ifname,
  uint16              num_bytes 
);

/*===========================================================================

             IPA Driver <-> WWAN Interface DEFINITIONS and TYPES

===========================================================================*/
typedef enum
{
  IPA_WAN_BEARER_TECH_LTE = 0,
  IPA_WAN_BEARER_TECH_UMTS,
  IPA_WAN_BEARER_TECH_OTHER
} ipa_wan_bearer_tech_e;

/*! @brief struct: Bearer unidirectional traffic statistics */
typedef struct
{
  uint64 num_ipv4_pkts;
  uint64 num_ipv4_bytes;
  uint64 num_ipv6_pkts;
  uint64 num_ipv6_bytes;
  uint64 total_num_pkts;
  uint64 total_num_bytes;
} ipa_wan_unidir_stats_s;

/*! @brief struct: Bearer bidirectional traffic statistics */
typedef struct
{
  ipa_wan_unidir_stats_s ul_stats; 
  ipa_wan_unidir_stats_s dl_stats;
} ipa_wan_bidir_stats_s;


/*! @brief enum: WAN Callback Bearer Events */
typedef enum
{
  IPA_WAN_DS_CB_EVENT_BEARER_SUSPENDED = 0,
  IPA_WAN_DS_CB_EVENT_BEARER_ACTIVATED,
  IPA_WAN_DS_CB_EVENT_BEARER_REGISTERED,
  IPA_WAN_DS_CB_EVENT_BEARER_DEREGISTERED
} ipa_wan_ds_cb_event_e;

/*! @brief struct: WAN bearer reg param */
typedef struct
{
  uint8                    uid;
  sio_stream_id_type       sio_stream_id;
  dsm_watermark_type       *l2_to_ipa_wm_ptr;
  dsm_watermark_type       *ipa_to_ps_wm_ptr;
  dsm_watermark_type       *ps_to_l2_wm_ptr;
  ipa_wan_bearer_tech_e    bearer_tech;
  uint8                    bearer_id;
  uint32                   subscription_id;
} ipa_wan_ds_reg_bearer_param_s;

/*! @brief function type for wan event callback */
typedef void (*ipa_wan_ds_event_cb_type) 
(
  ipa_wan_ds_cb_event_e cb_event,
  uint8                 uid
);
 
/*===========================================================================

  FUNCTION:  ipa_wan_ds_register_event_cb

===========================================================================*/
/*!
    @brief
    This API registers a callback function that IPA driver will invoke when
    certain WAN events occur.
 
    This API can also be used to deregister the callback when NULL is passed
    the argument.
 
    @param ipa_wan_ds_event_cb_type, Callback function to be registered
    with the driver
 
    @return 
    IPA_SUCCESS, when all params passed error checking
    IPA_ERR_INVALID_ARGUMENT, when the parameters passed in the function are
    incorrect
*/ 
/*=========================================================================*/
ipa_err_code_e ipa_wan_ds_register_event_cb
(
  ipa_wan_ds_event_cb_type  wan_ds_event_cb_fn
);
 
/*===========================================================================

  FUNCTION:  IPA_WAN_DS_REGISTER_BEARER

===========================================================================*/
/*!
    @brief
    This API registers downlink and uplink DS watermark, unique bearer_id and
    sio_stream with IPA driver for a given bearer.

    @return Following can be possible values in the return code:
 
    @retval IPA_SUCCESS, when Bearers params could be registered successfully.
    @retval IPA_ERR_LOGICAL_SIO_STREAM_UNKNOWN, when SIO Stream id is unknown
    @retval IPA_ERR_INVALID_ARGUMENT, when an argument value is invalid.
*/ 
/*=========================================================================*/
ipa_err_code_e ipa_wan_ds_register_bearer
(
  ipa_wan_ds_reg_bearer_param_s *reg_param
);

/*=============================================================================

  FUNCTION:  IPA_WAN_DS_DEREGISTER_BEARER

=============================================================================*/
/*!
    @brief
    This API deregisters the parameters associated with the given bearer.

    @return Following can be possible values in the return code:
 
    @retval IPA_SUCCESS, when bearers params could be deregistered successfully
    @retval IPA_ERR_INVALID_ARGUMENT, when uid value is invalid.
*/ 
/*===========================================================================*/
ipa_err_code_e ipa_wan_ds_deregister_bearer
(
  uint8 uid
);

/*===========================================================================

  FUNCTION:  IPA_WAN_DS_CONFIG_DL_BRIDGE

===========================================================================*/
/*!
    @brief
    This API is invoked by DS to enable or disable the downlink accelerated
    bridge.

    @return Following can be possible values in the return code:
 
    @retval IPA_SUCCESS, DL accelerated bridge configuration was successful
    @retval IPA_ERR_INVALID_ARGUMENT, when uid provided is invalid
    @retval IPA_ERR_ACCEL_BRIDGE_ALREADY_ENABLED, bridge is already enabled
                                                  on the given bearer
    @retval IPA_ERR_ACCEL_BRIDGE_ALREADY_DISABLED, bridge is already disabled
                                                   on the given bearer
*/ 
/*=========================================================================*/
ipa_err_code_e  ipa_wan_ds_config_dl_bridge
(
  uint8                     uid,
  ipa_wan_dl_bridge_state_e action
);

/*===========================================================================

  FUNCTION:  IPA_WAN_DS_GET_BEARER_STATS

=============================================================================*/
/*!
    @brief
    This API is invoked to retrieve per bearer traffic statis from IPA Driver.
    IPA driver will consolidate the UL and DL traffic statistics and return
    them as output parameter. It assumes the caller has allocated the memory
    for the statistics structure parameter.

    @return Following can be possible values in return code:
 
    @retval IPA_SUCCESS, retrieving statistics was successful.
    @retval IPA_ERR_INVALID_ARGUMENT when -
            1. when uid value provided is invalid
            2. uid state is Inactive 
            3. stats pointer passed to retrieve statistics is NULL.
*/ 
/*===========================================================================*/
ipa_err_code_e ipa_wan_ds_get_bearer_stats
(
  uint8                  uid,                          
  ipa_wan_bidir_stats_s *stats
);

/*===========================================================================

  FUNCTION:  IPA_WAN_DS_REGISTER_DPL_WM

===========================================================================*/
/*!
    @brief
    This API is invoked by DS to register DPL watermark with IPA driver.

    @return Following can be possible values in the return code:
 
    @retval IPA_SUCCESS, when DPL wm registration is successful
    @retval IPA_ERR_INVALID_ARGUMENT, when DPL watermark is invalid. 
*/ 
/*=========================================================================*/
void ipa_wan_ds_register_dpl_wm
(
  dsm_watermark_type *dpl_wm_ptr
);

/*===========================================================================

  FUNCTION:  IPA_WAN_DS_CONFIG_DPL

===========================================================================*/
/*!
    @brief
    This API is invoked by DS to configure DPL on the given bearer.
    The DPL IID parameters for this bearer needs to be
    passed to the driver for accelerated DPL

    @param uid, UID of the bearer 
    @param ifname, Interface name associated to this bearer's DPL IID
    @param num_bytes, Number of DPL bytes to be logged (Only for legacy DPL)
    @param cb_data, CB data (Only for legacy DPL)

    @return Following can be possible values in the return code:

    @retval IPA_SUCCESS, when DPL config for the bearer is successful.
    @retval IPA_ERR_INVALID_ARGUMENT, when uid is invalid. 
*/ 
/*=========================================================================*/
ipa_err_code_e ipa_wan_ds_config_dpl
(
  uint8  uid,
  uint8  ifname,
  uint16 num_bytes,
  uint32 cb_data
);

/*===========================================================================

  FUNCTION:  IPA_WAN_DS_SET_BEARER_PROC_STATE

===========================================================================*/
/*!
    @brief
    This function is invoked to suspend or activate DL processing of
    bearer by IPA. IPA will stop dequeueing packets from PDCP if the bearer
    is suspended.

    @return Following can be possible values in the return code:
 
    @retval IPA_SUCCESS, when bearer processing state is set successfully
                         as requested.
    @retval IPA_ERR_INVALID_ARGUMENT, when a passed argument is invalid.
*/
/*=========================================================================*/
ipa_err_code_e ipa_wan_ds_set_bearer_proc_state
(
  uint8                       uid,
  ipa_wan_bearer_proc_state_e proc_state
);

/*===========================================================================

  FUNCTION:  IPA_WANI_DS_GET_BEARER_PROC_STATE

===========================================================================*/
/*!
    @brief
    This function is invoked by DPM to retieve current Bearer processing 
    state by UID.

    @return Following can be possible values in the return code:
 
    @retval IPA_SUCCESS, when bearer processing state is get successfully.
            IPA_ERR_INVALID_ARGUMENT, when a passed argument is invalid.
*/
/*=========================================================================*/
ipa_err_code_e ipa_wan_ds_get_bearer_proc_state
(
  uint8                        uid,
  ipa_wan_bearer_proc_state_e *proc_state
);

/*===========================================================================

  FUNCTION:  ipa_wan_cipher_register_subs_id

===========================================================================*/
/*!
    @brief
    Registers valid subs_id.
 
 
    @return
    None
*/ 
/*=========================================================================*/
boolean ipa_wan_pdcp_cipher_register_subs_id
(
  ipa_wan_subscription_id_t subs_id
);

/*===========================================================================

             IPA Driver <-> SIO QMAP Interface DEFINITIONS and TYPES

===========================================================================*/
/*! @brief QMAP Cmd Types */
typedef enum
{
  /*! CMD Request */
  IPA_QMAP_CMD_TYPE_REQ,
  /*! CMD Ack */
  IPA_QMAP_CMD_TYPE_ACK,
  /*! CMD Unsupported */
  IPA_QMAP_CMD_TYPE_UNS,
  /*! CMD Invalid */
  IPA_QMAP_CMD_TYPE_INV,
  IPA_QMAP_CMD_TYPE_MAX
} ipa_qmap_cmd_type_e;

/*! @brief QMAP Cmd Names */
typedef enum
{
  IPA_QMAP_CMD_NAME_MIN,
  /*! Qmap Flow disable cmd */
  IPA_QMAP_CMD_NAME_FLOW_DIS,
  /*! Qmap Flow enable cmd */
  IPA_QMAP_CMD_NAME_FLOW_EN,
  /*! QMAP Accelerated DPL Enable request command */
  IPA_QMAP_CMD_NAME_ACC_DPL_EN,
  IPA_QMAP_CMD_UL_DATA_SENT_CNF,
  IPA_QMAP_CMD_NAME_MAX
} ipa_qmap_cmd_name_e;

/*! @brief QMAP header struct definition after ntohl conversion */
typedef PACKED struct PACKED_POST
{
  uint32 pkt_len   : 16;
  uint32 mux_id    : 8;
  uint32 pad_len   : 6;
  uint32 reserved  : 1;
  uint32 hdr_type  : 1;
} ipa_qmap_hdr_s;

/*! @brief QMAP cmd header info after ntohl conversion */
typedef PACKED struct PACKED_POST
{
  uint32 reserved_1 : 16;
  uint32 type       : 2;
  uint32 reserved_2 : 6;
  uint32 name       : 8;
  uint32 trans_id   : 32;
} ipa_qmap_cmd_info_s;

/*! @brief QMAP Cmd handler function prototype */
typedef void (*ipa_qmap_cmd_hdlr_fn)(sio_stream_id_type sio_stream_id,
                                     ipa_qmap_hdr_s *qmap_hdr_ptr, 
                                     ipa_qmap_cmd_info_s *qmap_cmd_info_ptr,
                                     dsm_item_type *cmd_payload_ptr);

/*=============================================================================

  FUNCTION:  ipa_sio_send_qmap_cmd_req

=============================================================================*/
/*!
    @brief
    This API is invoked by DS to send a QMAP cmd request to a given
    peripheral endpoint
 
    @param sio_stream_id, Stream id on which the QMAP cmd is sent
    @param cmd_name, Cmd Name request to send
    @param cmd_payload_ptr, Cmd payload dsm pointer
    @param trans_id, Transaction ID - Will be filled by the driver for new
                                      requests.
                                      Needs to be filled by the client for
                                      retransmissions
    @param is_retx, Boolean to indicate whether it is a retransmission or not

    @return Following can be the possible values in the return code:
 
    @retval IPA_SUCCESS
    @retval IPA_ERR_LOGICAL_SIO_STREAM_UNKNOWN, when sio_stream id is unknown
    @retval IPA_ERR_FAIL, Internal failure in sending the QMAP cmd    
*/ 
/*===========================================================================*/
ipa_err_code_e ipa_sio_send_qmap_cmd_req
(
  sio_stream_id_type  sio_stream_id, 
  ipa_qmap_cmd_name_e cmd_name,
  dsm_item_type       *cmd_payload_ptr,
  uint32              *trans_id,
  uint8               is_retx  
);

/*=============================================================================

  FUNCTION:  ipa_sio_register_qmap_cmd_hdlr_fn

=============================================================================*/
/*!
    @brief
    This API is invoked by DS to register a callback function when a QMAP
    command of specified name & specified type is received by the driver
 
    @param cmd_name, Cmd Name for which to register the CB
    @param cmd_type, Cmd Type for which to register the CB
    @param hdlr_fn, Handler callback function

    @return Following can be the possible values in the return code:
 
    @retval IPA_SUCCESS
    @retval IPA_ERR_FAIL, when an unknown cmd name, type is sent or when
    there is already an external callback registered
*/ 
/*===========================================================================*/
ipa_err_code_e ipa_sio_register_qmap_cmd_hdlr_fn
(
  ipa_qmap_cmd_name_e   cmd_name,
  ipa_qmap_cmd_type_e   cmd_type,
  ipa_qmap_cmd_hdlr_fn  hdlr_fn
);

/*===========================================================================

             IPA Driver <-> DS Stats Interface DEFINITIONS and TYPES

===========================================================================*/
/*! @brief Stats retrived from DS Modehandler for a particular bearer */
typedef struct
{
  /*! The number of bytes currently sitting in the bearer's UL WM */ 
  uint64 num_wm_curr_byte_cnt;
  /*! The number of bytes dropped due to DNE in the bearer's UL WM */ 
  uint64 num_wm_dne_drop_byte_cnt;
  /*! Number of times UL flow was enabled */
  uint32 num_qos_flow_enable;
  /*! Number of times UL flow was disabled */
  uint32 num_qos_flow_disable;
  /*! Number of times UL flow was enabled */
  uint32 num_wds_flow_enable;
  /*! Number of times UL flow was disabled */
  uint32 num_wds_flow_disable;
  /*! Number of times Backpressure was enabled */
  uint32 num_bp_enable;
  /*! Number of times Backpressure was disabled */
  uint32 num_bp_disable;
  /*! Reserved for future use */
  uint32 reserved_1;
  /*! Reserved for future use */
  uint32 reserved_2;
  /*! Reserved for future use */
  uint32 reserved_3;
  /*! Reserved for future use */
  uint32 reserved_4;
} ipa_wan_ds_ul_stats_s;

/*! @brief Prototype for the callback function to be registered with the 
    IPA driver for  stats retrieval */
typedef void (*ipa_wan_ds_ul_stats_cb_fn)
(
  /*! Subscription ID of the bearer */
  uint32                subs_id, 
  /*! UID of the bearer */
  uint8                 uid,
  /*! Stats structrue pointer to be filled */
  ipa_wan_ds_ul_stats_s *stats_ptr
);

/*=============================================================================

  FUNCTION:  ipa_wan_register_ds_ul_stats_cb_fn

=============================================================================*/
/*!
    @brief
    This API is invoked by DS to register a callback function that can be 
    invoked by IPA driver to retrieve UL stats from DS 
 
    @param subs_id,     Corresponding subscription ID for the stats cb function 
    @param stats_cb_fn, Stats retrieval callback function

    @return Following can be the possible values in the return code:
 
    @retval IPA_SUCCESS
    @retval IPA_ERR_FAIL, Unkown subscription ID passed
*/ 
/*===========================================================================*/
ipa_err_code_e ipa_wan_register_ds_ul_stats_cb_fn
(
  ipa_wan_ds_ul_stats_cb_fn stats_cb_fn
);

/*===========================================================================

  FUNCTION:  ipa_sio_get_ul_pipe_stats

===========================================================================*/
/*!
    @brief
    Retrieves the number of packets/bytes going through a UL pipe based on 
    pipe index

    @return 
    IPA_SUCCESS, when all params passed error checking
    IPA_ERR_INVALID_ARGUMENT, when the parameters passed in the function are
    incorrect
    
*/ 
/*=========================================================================*/
ipa_err_code_e ipa_sio_get_ul_pipe_stats
(
  /*! Pipe index */
  uint32  pipe_id,
  uint32 *num_pkts
);

/*===========================================================================

             IPA Driver <-> Accelerated DPL Interface DEFINITIONS and TYPES

===========================================================================*/
/*! @brief Enum to capture the direction of DPL in acc_dpl_tx API */
typedef enum
{
  IPA_ACC_DPL_DIR_DL,
  IPA_ACC_DPL_DIR_UL
} ipa_acc_dpl_dir_e;

/*! Enum to disable/enable accelerated DPL */
typedef enum
{
  IPA_ACC_DPL_STATE_DISABLE,
  IPA_ACC_DPL_STATE_ENABLE
} ipa_acc_dpl_state_e;

typedef enum
{
  IPA_ACC_DPL_STATE_CB,
  IPA_ACC_DPL_HDR_BYTES_CB 
}ipa_acc_dpl_cb_type_e;

typedef enum 
{
  IPA_ENDP_DIRECTION_UL = 0,
  IPA_ENDP_DIRECTION_DL,
  IPA_ENDP_DIRECTION_MAX
}ipa_endp_direction_e;

/*! Parameters passed in the DPL callback functio */
typedef struct
{
  ipa_acc_dpl_cb_type_e cb_type;
  union
  {
    struct
    {
      /*! Whether the accelerated DPL path is available or not  */
      uint8  is_available;
      /*! DPL version currently in use */
      uint8  version;
    }ipa_acc_dpl_state_params_s;
    
    struct
    {
      /*! @brief Source endpoint */ 
      uint8                src_endp;
      /*! @brief Header bytes for the endpoint */ 
      uint8                hdr_bytes;
      /*! @brief UL or DL endpoint */ 
      ipa_endp_direction_e direction;
      /*! @brief Mux ID for the endpoint */ 
      uint8                mux_id;
      /*! @brief Callback data */
      uint32               cb_data;
    }ipa_acc_dpl_hdr_bytes_params_s;
  }ipa_acc_dpl_params_u;
} ipa_acc_dpl_cb_fn_param_s;

/*! @brief Callback function signature to be called by the driver once the 
  accelerated DPL path is avaiable/unavailable */
typedef void (*ipa_acc_dpl_cb_fn_type)
(
  ipa_acc_dpl_cb_fn_param_s *params_ptr
);

/*=============================================================================

  FUNCTION:  ipa_acc_dpl_reg_cb

=============================================================================*/
/*!
    @brief
    This API is invoked by clients to register for notification cb from IPA
    driver when accelerated DPL path is available
 
    @param acc_dpl_cb_fn, Callback function to be invoked when the driver
                          activates the accelerated DPL path

    @return Following can be the possible values in the return code:

    @retval IPA_SUCCESS
    @retval IPA_ERR_FAIL, Internal API failure
*/ 
/*===========================================================================*/
ipa_err_code_e ipa_acc_dpl_reg_cb
(
  ipa_acc_dpl_cb_fn_type acc_dpl_cb_fn
);

/*=============================================================================

  FUNCTION:  ipa_acc_dpl_configure

=============================================================================*/
/*!
    @brief
    This API is invoked by DS to enable/disable the accelerated DPL path.
    This state is maintained across the cable disconnect
 
    @param state, Accelerated DPL state 

    @return Following can be the possible values in the return code:

    @retval IPA_SUCCESS
    @retval IPA_ERR_FAIL, Internal API failure
*/ 
/*===========================================================================*/
ipa_err_code_e ipa_acc_dpl_configure
(
  ipa_acc_dpl_state_e state
);

/*=============================================================================

  FUNCTION:  ipa_acc_dpl_tx

=============================================================================*/
/*!
    @brief
    This API is invoked by clients to send a DPL log packet via the accelerated
    DPL path.

    @param dir, Direction with this packet's DPL IID
    @param ifname, Interface name associated with this packet's DPL IID
    @param pkt_len, Total Length of the packet to be logged
    @param pkt_ptr, DSM pointer of the packet

    @return Following can be the possible values in the return code:

    @retval IPA_SUCCESS
    @retval IPA_ERR_INVALID_ARGUMENT, when payload_ptr is NULL or payload_len = 0
    @retval IPA_ERR_FAIL, Internal API failure
*/ 
/*===========================================================================*/
ipa_err_code_e ipa_acc_dpl_tx
(
  ipa_acc_dpl_dir_e dir,
  uint8             ifname, 
  uint16            pkt_len,
  dsm_item_type     *pkt_dsm_ptr 
);

/*===========================================================================

                     CLAT DEFINITIONS and TYPES

===========================================================================*/

/*! @brief Clat handle used to identify CLAT PDN context config */
typedef uint8 ipa_clat_handle_t;

/*! @brief IPA CLAT V6 prefix length types */
typedef enum
{
  /*!prefix_len = 96bits*/
  IPA_CLAT_V6_PREFIX_LEN_96 = 0,
  /*!prefix_len = 64bits*/
  IPA_CLAT_V6_PREFIX_LEN_64 = 1,
  /*!prefix_len = 56bits*/
  IPA_CLAT_V6_PREFIX_LEN_56 = 2,
  /*!prefix_len = 48bits*/
  IPA_CLAT_V6_PREFIX_LEN_48 = 3,
  /*!prefix_len = 40bits*/
  IPA_CLAT_V6_PREFIX_LEN_40 = 4,
  /*!prefix_len = 32bits*/
  IPA_CLAT_V6_PREFIX_LEN_32 = 5,
  IPA_CLAT_V6_PREFIX_LEN_MAX = 0xFF
} ipa_clat_prefix_len_e;

/*! @brief struct: Global clat translation context info.
 *  This holds the clat context configuration info that is common for all PDNs*/
typedef struct
{
  /*! Source IPv4 subnet mask */
  uint32 v4_client_subnet_mask;
  /*! Source IPv4 subnet address */
  uint32  v4_client_subnet_addr;
  /*! TOS translation action to be taken */
  uint8 tos_ignore_bit_flag;
  /*! tos override value that needs to be filled in DL when tos_ignore_bit 
    flag is set*/
  uint8 tos_override_val;
} ipa_clat_global_cntxt_info_type;

/*! @brief struct: clat PDN translation context info.
 * This holds the PDN specific clat configuration data for both  4to6 and 6to4
 * translation that clients need to provide the IPA driver to enable XLAT translation in IPA.*/
typedef struct
{
  /*! Source IPv4 address */
  uint32 v4_src_addr;
  /*! MTU value for the PDN*/
  uint16 mtu;
  /*! Device V6 prefix len */
  ipa_clat_prefix_len_e v6_dev_prefix_len;
  /*! Device prefix length in bits*/
  uint8                 dev_addr_match_bits;
  /*! PLAT V6 prefix len */
  ipa_clat_prefix_len_e v6_plat_prefix_len;
  /*! Device V6 prefix */
  uint8 v6_dev_prefix[16] __attribute__((aligned(4)));
  /*! PLAT V6 prefix */
  uint8 v6_plat_prefix[16] __attribute__((aligned(4)));
} ipa_clat_pdn_cntxt_info_type;

/*! @brief enum: XLAT Callback Event Types */
typedef enum
{
  IPA_CLAT_CFG_CB_EVT_REGISTERED = 0,
  IPA_CLAT_CFG_CB_EVT_DEREGISTERED,
  IPA_CLAT_CFG_CB_EVT_RECONFIGURED,
  IPA_CLAT_CFG_CB_EVT_SIO_ASSOCIATED
} ipa_clat_cfg_cb_evt_e;

/*! Callback function signature to be called by the driver for various
 *  XLAT config event types.*/
typedef void (*ipa_clat_cfg_evt_cb_type)
(
  ipa_clat_cfg_cb_evt_e  cb_event,
  ipa_clat_handle_t      clat_hdl,
  void                   *cb_data
);

/*! @brief Supported IPA functionality list used for capability mask */
typedef enum
{
  IPA_CAPABILITY_CLAT_DL = 0x1,
  IPA_CAPABILITY_CLAT_UL = 0x2
} ipa_capability_e;

/*=============================================================================

  FUNCTION:  IPA_CLAT_GLOBAL_REGISTER

=============================================================================*/
/*!
    @brief
    This function provides necessary global clat configuration info
    common across all CLAT PDNs.

    @return
    IPA_SUCCESS, when all params passed error checking
    IPA_ERR_INVALID_ARGUMENT, when the parameters passed in the function are
    incorrect
    TODO-->return a new handle?
*/
/*===========================================================================*/
ipa_err_code_e  ipa_clat_global_register
(
  ipa_clat_global_cntxt_info_type  *clat_global_cntxt_info
);

/*=============================================================================

  FUNCTION:  IPA_CLAT_CONFIGURE

=============================================================================*/
/*!
    @brief
    This API is invoked by the client to enable and configure 4to6 and 6to4
    CLAT translation in IPA.sio_stream_id is used as the unique identifier
    for a particular CLAT PDN context.

    @return
    IPA_SUCCESS, when all params passed error checking
    IPA_ERR_INVALID_ARGUMENT, when the parameters passed in the function are
    incorrect
    *clat_hdl : OUT parameter
*/
/*===========================================================================*/
ipa_err_code_e  ipa_clat_register
(
  ipa_clat_pdn_cntxt_info_type   *clat_cntxt_info,
  ipa_clat_handle_t              *clat_hdl
);

/*=============================================================================

  FUNCTION:  IPA_CLAT_DEREGISTER

=============================================================================*/
/*!
    @brief
    This API is invoked by the client to dereigster CLAT configuration on a PDN 
    identified by the clat handle. This is invoked all bearers on the CLAT PDN
    are torn down.

    @return
    IPA_SUCCESS, when all params passed error checking
    IPA_ERR_INVALID_ARGUMENT, when the parameters passed in the function are
    incorrect
    
*/
/*===========================================================================*/
ipa_err_code_e  ipa_clat_deregister
(
  ipa_clat_handle_t          clat_hdl
);

/*=============================================================================

  FUNCTION:  IPA_CLAT_RECONFIGURE

=============================================================================*/
/*!
    @brief
    This API is invoked by the client to re-configure an already registerd CLAT
    context with any changes in context info. 

    @return
    IPA_SUCCESS, when all params passed error checking
    IPA_ERR_INVALID_ARGUMENT, when the parameters passed in the function are
    incorrect
    
*/
/*===========================================================================*/
ipa_err_code_e  ipa_clat_reconfigure
(
  ipa_clat_handle_t               clat_hdl,
  ipa_clat_pdn_cntxt_info_type   *clat_cntxt_info
);

/*=============================================================================

  FUNCTION:  IPA_CLAT_ASSOC_SIO_STREAM

=============================================================================*/
/*!
    @brief
    This API is invoked by the client to associate a SIO stream with CLAT PDN
    context. This needs to be invoked after a tethered SIO call comes and sio
    cfg mapping is complete.

    @return
    IPA_SUCCESS, when all params passed error checking
    IPA_ERR_INVALID_ARGUMENT, when the parameters passed in the function are
    incorrect
    
*/
/*===========================================================================*/
ipa_err_code_e  ipa_clat_assoc_sio_stream
(
  ipa_clat_handle_t               clat_hdl,
  sio_stream_id_type              sio_stream_id
);

/*===========================================================================

  FUNCTION:  ipa_clat_register_evt_cb

===========================================================================*/
/*!
    @brief
    This API registers a callback function that IPA driver will invoke when
    a valid CLAT event happens on an SIO stream.

    @param clat_cfg_evt_cb_fn, Callback function to be registered with IPA.
    @param clat_cb_data, Callback data to be used while invocation of the cb.
    @return
    IPA_SUCCESS, when cb function could be registered successfully.
    IPA_ERR_INVALID_ARGUMENT, if a parameter passed was incorrect
*/
/*=========================================================================*/
ipa_err_code_e ipa_clat_register_evt_cb
(
  ipa_clat_cfg_evt_cb_type  clat_cfg_evt_cb_fn,
  void                     *clat_cb_data
);

/*===========================================================================

  FUNCTION:  IPA_QUERY_CAPABILITY

===========================================================================*/
/*!
    @brief
    This API is invoked by client to query capabilites supported by IPA.

    @param clat_cfg_evt_cb_fn, Callback function to be registered with IPA.
    @param clat_cb_data, Callback data to be used while invocation of the cb.
    @return
    capability_mask with bits set for corresponding functionality supported.
*/
/*=========================================================================*/
uint64 ipa_query_capability
(
  void
);

/*===========================================================================

             IPA Driver <-> Checksum offload DEFINITIONS and TYPES

===========================================================================*/
/*=============================================================================*/
/*!
    @brief
    This API is invoked by the DS client to enable checksum offload on the
    DL embedded data-path.

    @return Following can be the possible values in the return code:

    @retval IPA_SUCCESS
    @retval IPA_ERR_FAIL, Internal API failure
    
*/
/*===========================================================================*/
ipa_err_code_e ipa_offload_checksum
(
  void
);

/*! @brief enum: IP version type 
    Used in the ip_version of the DSM items passed to DS.
*/
typedef enum
{
  IPA_DSM_IP_VERSION_4 = IPA_IP_TYPE_IPV4,
  IPA_DSM_IP_VERSION_6 = IPA_IP_TYPE_IPV6
} ipa_dsm_ip_version_type;

/*! @brief UL info in the DSM application field */
typedef PACKED struct PACKED_POST
{
  uint32 filter_result;
} ipa_dsm_ul_app_field_info_s;

/*! @brief DL info in the DSM application field */
typedef PACKED struct PACKED_POST
{
  uint32 filter_result      : 16;
  uint32 hw_filtering       : 1;
  uint32 filter_res_present : 1;
  uint32 ip_version         : 2;
  uint32 reserved           : 2;
  uint32 is_packet_logged   : 1;
  uint32 csum_reserved      : 1;
  uint32 csum_computed      : 1;
  uint32 start_offset       : 7;
} ipa_dsm_dl_app_field_info_s;

/*! @brief Checksum offload info in the DSM application pointer */
typedef PACKED struct PACKED_POST
{
  uint32 pkt_len  : 16;
  uint32 csum_val : 16;
} ipa_dsm_app_ptr_info_s;

/*! @brief Macro to get the checksum value from the pointed dsm item 
    Note: checksum value endianity is in network byte order
*/
#define IPA_CSUM_GET_CSUM_VALUE(dsm_ptr) \
  ((ipa_dsm_app_ptr_info_s *)(&((*(dsm_ptr))->app_ptr)))->csum_val

/*! @brief Macro to get the packet length from the pointed dsm item 
    Note: packet length endianity is in network byte order
*/
#define IPA_CSUM_GET_PKT_LEN(dsm_ptr) \
  ((ipa_dsm_app_ptr_info_s *)(&((*(dsm_ptr))->app_ptr)))->pkt_len


/*! @brief Macro to get the checksum start offset from the pointed dsm item 
*/
#define IPA_CSUM_GET_START_OFFSET(dsm_ptr) \
  ((ipa_dsm_dl_app_field_info_s *)(&((*(dsm_ptr))->app_field)))->start_offset

/*! @brief Macro to verify if checksum offload was enabled in the pointed dsm item 
*/
#define IPA_CSUM_GET_CSUM_COMPUTED(dsm_ptr) \
  ((ipa_dsm_dl_app_field_info_s *)(&((*(dsm_ptr))->app_field)))->csum_computed

/*! Macro to get the IP version flag in the DSM item's
    app field
*/
#define IPA_DSM_GET_IP_VERSION_PARAM(dsm_ptr) \
  ((ipa_dsm_dl_app_field_info_s *)(&((*(dsm_ptr))->app_field)))->ip_version

/*===========================================================================

                  IPA Driver <-> Loopback DEFINITIONS and TYPES

===========================================================================*/
/*! @brief Peripheral interconnect type for the Loopback */
typedef enum
{
  IPA_TEST_INT_LOOPBACK_PER_EP_IC_TYPE_HSIC = 0x1,
  IPA_TEST_INT_LOOPBACK_PER_EP_IC_TYPE_PCIE = 0x2,
  IPA_TEST_INT_LOOPBACK_PER_EP_IC_TYPE_HSUSB = 0x3
} ipa_test_int_loopback_per_ep_ic_type_e;

/*! @brief Configuration recived for internal loopback setup */
typedef struct
{
  /*! IPA Pipe Number to be used for UL data reception */
  uint8                                  ul_src_pipe_num;
  /*! IPA Pipe Number to be used for DL data transmission */
  uint8                                  dl_dst_pipe_num;
  /*! Number of times each packed needs to be replicated in DL */
  uint8                                  dl_replicate_cnt;
  /*! Max size of packet to be transmitted */
  uint16                                 max_pkt_size;
  /*! Mux ID to be used if the hdr_type == QMAP */
  uint8                                  qmap_mux_id;
  /*! If IP packet loopback is required. This will 
    result in IPA driver switching the source/dest IP address
    for UDP/TCP packets & ICMP REQ->REPLY conversion */
  uint8                                  is_ip_loopback; 
  /*! Header protocol to be used for loopback (NONE or QMAP) */
  ipa_sio_hdr_type_e                     ul_hdr_type;
    /*! Header protocol to be used for loopback (NONE or QMAP) */
  ipa_sio_hdr_type_e                     dl_hdr_type;
  /*! Interconnect to be used for loopback (HSIC or PCIE) */
  ipa_test_int_loopback_per_ep_ic_type_e ic_type;
} ipa_test_int_loopback_cfg_s;

/*=============================================================================

  FUNCTION:  ipa_test_int_loopback_configure

=============================================================================*/
/*!
    @brief
    This API is invoked to setup the IPA internal loopback 
 
    @detail
    We're going to shortcircuit the UL SIO watermark with the DL PDCP watermark 
    to achieve the loopback.
    Flow: UL per(PCIe/HSIC) pipe -> IPA HW  -> Q6 UL pipe -> UL SIO WM =
          DL PDCP WM -> Q6 DL pipe -> IPA HW -> DL per pipe (PCIe/HSIC)

    @param loopback_cfg_ptr, Loopback configuration parameters

    @return Following can be the possible values in the return code:
 
    @retval IPA_SUCCESS, Loopback configured successfully
    @retval IPA_ERR_INVALID_ARGUMENT, Invalid argument passed
    @retval IPA_ERR_FAIL, Internal error
*/ 
/*===========================================================================*/
ipa_err_code_e ipa_test_int_loopback_configure
(
  ipa_test_int_loopback_cfg_s *loopback_cfg_ptr
);

/*===========================================================================

  FUNCTION:  ipa_ul_ts_rewrite_enable_indication

===========================================================================*/
/*!
    @brief            
    This API is invoked by DS module to signal ON/OFF indication
    for timestamp rewrite
 
    @detail
    1. If DS module signals ON and TS_enable was Off, update internally, 
       store the t_diff value received, configure and start the periodic timer
       to handle ts_state state machine
    2. If DS module signals ON and TS_enable was already ON, update t_diff value
       received and update delta_TSER for all valid active connections
    3. If DS module signals OFF, update internally and stop the periodic timer
       handling ts_state state machine
    4. If DS module signals OFF and TS_enable was already Off, do nothing
 
    @param ts_enable (TS Rewrite indication, 1- ON, 0- OFF)
           t_diff    (timestamp stretch value in ms)
           default_stretch_ratio  (Default stretch ratio to be used)

    @return

    @note
*/
/*=========================================================================*/
void ipa_ul_ts_rewrite_enable_indication  
(
  boolean ts_enable, 
  uint32 t_diff,      
  uint32 default_stretch_ratio
);

/*===========================================================================

  FUNCTION:  IPA_WAN_DS_REGISTER_TLB_CFG

===========================================================================*/
/*!
    @brief
    This API registers registers TLB Mode A configuration with IPA.

    @return Following can be possible values in the return code:
 
    @retval IPA_SUCCESS, when Bearers params could be registered successfully.
    @retval IPA_ERR_INVALID_ARGUMENT, when an argument value is invalid.
*/ 
/*=========================================================================*/
ipa_err_code_e ipa_wan_ds_register_tlb_cfg
(
  uint8 bearer_id,
  uint16 uplink_pkt_size
);


/*=============================================================================

  FUNCTION:  ipa_set_dl_burst_marker_control

=============================================================================*/
/*!
    @brief
    This API is invoked by DS to enable or disable dl burst marker.

    @param burst_marker_action,
          IPA_DL_BURST_MARKER_DISABLE if dl burst marker disable is requested,
          IPA_DL_BURST_MARKER_ENABLE  if dl burst marker is requested

    @return Following can be the possible values in the return code:

    @retval IPA_SUCCESS
    @retval IPA_ERR_FAIL
*/
/*===========================================================================*/
ipa_err_code_e ipa_set_dl_burst_marker_control
(
    ipa_dl_burst_marker_control_state_e burst_marker_action
);

/*=========================================================================

   FUNCTION     :  ipa_ctl_register_ds_hw_init_cb

===========================================================================

   Description  : used to register DS callback functions with IPA driver
   Parameters   : ipa_dl_req_devote_cb_type
   Returns      : None

========================================================================== */

void ipa_ctl_register_ds_hw_init_cb
(
  ipa_ctl_hw_init_status_cb_type ds_hw_init_cb
);

/*=========================================================================

   FUNCTION     :  ipa_dl_register_l2_cb

===========================================================================

   Description  : used to register DL callback functions with IPA driver
   Parameters   : ipa_dl_req_devote_cb_type
   Returns      : None

========================================================================== */

void ipa_dl_register_l2_cb
(
  ipa_dl_req_devote_cb_type dl_reg_cb
);


/*=============================================================================

  FUNCTION:  ipa_ctl_clk_send_q6_fast_devote_msg

=============================================================================*/
/*!
    @brief
    This API is invoked by LTE to send fast devote message to q6.
*/
/*===========================================================================*/



void ipa_ctl_clk_send_q6_fast_devote_msg();



#endif /* IPA_H */
