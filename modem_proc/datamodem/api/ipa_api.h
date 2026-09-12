/*!
  @file
  ipa_api.h

  @brief
  This file contains definitions of all IPA driver type definitions and APIs 
  available to Datamodem external modules.

*/
/*===========================================================================

  Copyright (c) 2014 QUALCOMM Technologies Incorporated. All Rights Reserved

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

$Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/05/17   gk      Support for Uplink Firewall.
07/01/16   ns      CR1037140: API change for physical address to be provided 
                   by Q6Zip.
05/17/16   tk      CR#1001776: Drop UL packets with bad length in status.
01/14/16   ns      ZIP IRQ coaleascing interface changes
08/29/15   mi      CR#898632: IPA ZIP cleanup during modem shutdown.
04/29/15   vm      FR# 22520 Support for HFN Resync.
04/16/15   ag      CR823712: Remove inline for stats query.
02/25/15   ag      CR795894: Stats query support.
02/17/15   mi      FR#739684:IPA ZIP feature check-in.
11/21/14   mi      CR#761035:IPA SIO driver initialization moved to IPA CTL task.
08/14/14   rp      Added callback to indicate some error on a bearer when 
                   there are successive number of exception packets  
04/25/14   ag      CR654784 Retrieve IPA lib version
01/02/14   SaCh    Adding external API to return ciphering key at given index.
10/01/13   am      Initial version
===========================================================================*/

#ifndef IPA_API_H
#define IPA_API_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include <comdef.h>
#include <dsm.h>
#include <sio.h>

/*===========================================================================

                              MACROS

===========================================================================*/
/*! @brief LTE ciphering key size (in bytes) */
#define IPA_WAN_CIPH_KEY_SIZE 16
/*! @brief Max Num of LTE ciphering keys */
#define IPA_WAN_CIPH_KEY_NUM 16
/*! @brief Maximum number of LTE ciphering keys */
#define IPA_WAN_CIPH_KEYS_MAX 16
/*! @brief Default value of subscription id */
#define IPA_WAN_DEFAULT_SUBSCRIPTION_ID 1

/*===========================================================================

             IPA Driver <-> WWAN Interface DEFINITIONS and TYPES

===========================================================================*/
/*! @brief typedef: Subscription Id */
typedef uint32 ipa_wan_subscription_id_t;

/*! @brief enum: LTE PDCP ciphering algorithms */
typedef enum
{
  IPA_WAN_CIPHER_ALGO_AES        = 0,
  IPA_WAN_CIPHER_ALGO_ZUC        = 1,
  IPA_WAN_CIPHER_ALGO_LTE_SNOW3G = 2,
  IPA_WAN_CIPHER_ALGO_NONE
} ipa_wan_cipher_algo_e;

/*! @brief parameters needed for cipher init task
*/
typedef struct
{
  uint8 key_index; /*!< valid range 0..31 */
  /*! Use uint8 instead of a2_cipher_algo_e algo to make sure size of this
      this sturcture is always 2 bytes in all compilers */
  uint8 algo;
} ipa_wan_cipher_init_param_s;

/*! @brief paramerters needed for cipher init task.
    union with "value" is needed for quick comparison.
*/
typedef union
{
  ipa_wan_cipher_init_param_s param;
  /*! value is used for optimized comparison */
  uint16                     value;
} ipa_wan_cipher_init_param_u;

/*! @brief parameters needed for cipher setup task.
*/
typedef struct
{
  uint8   bearer_id; /*!< valid range 0..31 */
} ipa_wan_cipher_setup_param_s;

/*! @brief paramerters needed for cipher init and setup task
*/

typedef struct
{
  ipa_wan_cipher_init_param_u      init;
  ipa_wan_cipher_setup_param_s     setup;
} ipa_wan_cipher_param_s;

/*! @brief A structure which defines the ciphering parameters of a stream.
* Provided by PDCP per packet when cipher is enabled.
* Union with uint32 is used to quickly check when the cipher 
 * parameters have changed.
*/
typedef union
{
  ipa_wan_cipher_param_s    param;
  uint32                    value; /*!< value is used for quick comparison */
} ipa_wan_cipher_param_u;

/*! @brief Function prototype to be triggered by IPA for
  detecting potential HFN mismatch.
  recover_success: TRUE whenever number of consecutive valid packets cross the
                   configured threshold after an HFN excpetion has occured.
                   FALSE when the number of HFN exceptions
                   cross configured threshold
  */
typedef void (*ipa_wan_bearer_err_ind_cb_type)
(
 /*! uid on which the error is detected. */
  uint8 uid,
  /* recover_success to indicate whether HFN exception has been recovered or not */
  boolean recover_success
);

/*===========================================================================

             IPA ZIP Interface DEFINITIONS and TYPES

===========================================================================*/

/*! @brief IPA ZIP return error codes */
typedef enum 
{
  /*! Common Error Codes */
  IPA_ZIP_SUCCESS = 0,
  IPA_ZIP_ERR_INVALID_ARGUMENT,
  /*! IPA  ZIP Error Codes */
  IPA_ZIP_ERR_MAX_REQS_RCHD,
  /*! Generic error */
  IPA_ZIP_ERR_FAIL,
  IPA_ZIP_ERR_CODE_MAX
} ipa_zip_err_code_e;

/*! @brief Type for passing clock requests */
typedef enum ipa_zip_clk_req_type
{
  IPA_ZIP_CLK_REQ_NO_CLK,
  IPA_ZIP_CLK_REQ_MAX_CLK
} ipa_zip_clk_req_type_e;
 
/*! @brief Type of callback events */
typedef enum ipa_zip_cb_event_type
{
  IPA_ZIP_CB_EVENT_REGISTER,
  IPA_ZIP_CB_EVENT_UNREGISTER,
  IPA_ZIP_CB_EVENT_DICT_LOAD,
  IPA_ZIP_CB_EVENT_CMD,
  IPA_ZIP_CB_EVENT_MODEM_SHUTDOWN,
  IPA_ZIP_CB_EVENT_MAX
} ipa_zip_cb_event_type_e;

/*! @brief Type of callback status */
typedef enum ipa_zip_cb_status_type
{
  IPA_ZIP_CB_STATUS_PASS = 1,
  IPA_ZIP_CB_STATUS_FAIL = 2
} ipa_zip_cb_status_type_e;

/*! @brief Type for comp/decomp commands */
typedef enum ipa_zip_cmd_type
{
  IPA_ZIP_CMD_COMP_RW,
  IPA_ZIP_CMD_DECOMP_RO,
  IPA_ZIP_CMD_DECOMP_RW,
  IPA_ZIP_CMD_DMA,
  IPA_ZIP_CMD_MAX
} ipa_zip_cmd_type_e;

/*! @brief type for completion tag */
typedef uint32 ipa_zip_cb_tag_type;

/*! @brief Comp event callback type */
typedef void (*ipa_zip_cb_type) 
(
  /*! @brief Callback event type */ 
  ipa_zip_cb_event_type_e  cb_evt,
  /*! @brief Status for the request */ 
  ipa_zip_cb_status_type_e cb_status,
  /*! @brief Tag provided in command request */ 
  ipa_zip_cb_tag_type      cmd_cb_tag,
  /*! @brief Actual dst buf size returned for the command */ 
  uint32                   cmd_dst_buf_size
);

/*! @brief Comp event callback type */
typedef void (*ipa_zip_cb_type_2) 
(
  /*! @brief Callback event type */ 
  ipa_zip_cb_event_type_e  cb_evt,
  /*! @brief Status for the request */ 
  ipa_zip_cb_status_type_e cb_status,
  /*! @brief Tag provided in command request */ 
  ipa_zip_cb_tag_type      cmd_cb_tag,
  /*! @brief Actual dst buf size returned for the command */ 
  uint32                   cmd_dst_buf_size,
  /*! @brief Status returned for the command */ 
  uint64*    			   status_pa,
  /*! @brief Last response flag */ 
  uint8					   last_response
);


/*! @brief Comp event callback type */
typedef void (*ipa_zip_err_cb_type) 
(
  /*! @brief Error code returned by HW */
  uint8                     error_code,
  /*! @brief Cb tag of the request that caused the error */
  ipa_zip_cb_tag_type       cb_tag,
  /*! @brief Address in the source buffer that triggered the error */
  uint16                    bytes_left                    
);

/*! @brief Structure for passing compressor config */
typedef struct ipa_zip_cfg_type
{
  /*! @brief Completion callback function */
  ipa_zip_cb_type       req_cb_fn_ptr;
  /*! @brief Completion callback function */
  ipa_zip_cb_type_2     req_cb_fn_ptr_2;
  /*! @brief Error callback function */
  ipa_zip_err_cb_type   err_cb_fn_ptr;
  /*! @brief Max outstanding requests to IPA in default priority queue */
  uint32                max_def_prio_reqs;
  /*! @brief Max outstanding requests to IPA in hi priority queue */
  uint32                max_hi_prio_reqs;
  /*! @brief Flag to indicate if HWP should generate a status packet */
  uint8                pkt_status_enabled; 
  /*! @brief IRQ coalescing timer value in Sleep Clock Cycle (1 - 255) */
  uint8                irq_coal_timer_val;            
} ipa_zip_cfg_type_s;

/*! @brief Structure for passing comp/decomp dictionary */
typedef struct ipa_zip_dict_type
{
  /*! @brief Pointer to dictionary buffer */
  uint8                *dict_buf_ptr;
  /*! @brief Dictionary buffer length */
  uint32                dict_buf_len;
} ipa_zip_dict_type_s;

/*! @brief Structure for passing comp/decomp requests */
typedef struct ipa_zip_cmd_req_type
{
  /*! @brief Comp/Decompression type */
  ipa_zip_cmd_type_e        cmd_type;
  /*! @brief Pointer to source buffer */
  uint8                    *src_buf;
  /*! @brief Length of source buffer */
  uint32                    src_len;
  /*! @brief Pointer to destination buffer */
  uint8                    *dst_buf;
  /*! @brief Length of destination buffer */
  uint32                    dst_len;
  /*! @brief Completion tag for this request */
  ipa_zip_cb_tag_type       cb_tag;
  /*! @brief Interrupt flag for this request */
  uint8                     int_flag;
  /*! @brief Pointer (physical address) to source buffer */
  uint8                    *src_buf_pa;
  /*! @brief Pointer (physical address) to destination buffer */
  uint8                    *dst_buf_pa;
} ipa_zip_cmd_req_type_s;

/*===========================================================================

             IPA ZIP Interface DEFINITIONS and TYPES

===========================================================================*/
/*===========================================================================

  FUNCTION:  ipa_zip_register

===========================================================================*/
/*!
    @brief
    This API registers SW zip module with IPA driver.

    This would be trigger to setup IPA pipes for comp/decomp function and 
    is expected to be invoked before using any other ipa_zip_* APIs.

    The callback ipa_zip_cb_type is invoked with callback event as
    IPA_ZIP_CB_EVENT_REGISTER upon completion of the registration.
 
    @param 
    ipa_zip_cfg_type_s, Config params passed to IPA driver
 
    @return 
    IPA_SUCCESS, when all params passed error checking
    IPA_ERR_INVALID_ARGUMENT, when the parameters passed in the function are
    incorrect
*/ 
/*=========================================================================*/
ipa_zip_err_code_e ipa_zip_register
(
  ipa_zip_cfg_type_s  *cfg_s_ptr
);

/*===========================================================================

  FUNCTION:  ipa_zip_unregister

===========================================================================*/
/*!
    @brief
    This cleanup API de-registers SW comp/decomp module with IPA driver.

    Existing outstanding transactions would get completed and no new
    transactions would be processed after calling this API.

    The callback ipa_zip_cb_type is invoked with callback event as
    IPA_ZIP_CB_EVENT_UNREGISTER upon completion of the deregistration.

    @param 
    None
 
    @return 
    None
*/ 
/*=========================================================================*/
void ipa_zip_unregister ( void );

/*===========================================================================

  FUNCTION:  ipa_zip_load_dict

===========================================================================*/
/*!
    @brief
    This API requests for comp/decomp dictionary to be loaded onto
    IPA HW. There are no sync requirements associated with this API except
    that the first invocation should occur before a comp/decomp request.

    ipa_zip_register needs to be called before invoking this API.
 
    The callback ipa_zip_cb_type is invoked with callback event as
    IPA_ZIP_CB_EVENT_DICT_LOAD upon completion of the dictionary load.

    @param 
    ipa_zip_dict_type_s, Config params passed to IPA driver
 
    @return 
    IPA_SUCCESS, when all params passed error checking and request 
    is enqueued.
    IPA_ERR_INVALID_ARGUMENT, when the parameters passed in the function are
    incorrect.
*/ 
/*=========================================================================*/
ipa_zip_err_code_e ipa_zip_load_dict
(
  ipa_zip_dict_type_s  const *dict_s_ptr
);

/*===========================================================================

  FUNCTION:  ipa_zip_cmd_req_def_prio

===========================================================================*/
/*!
    @brief
    This API enqueues a request for comp/decomp command to IPA driver in
    the default priority queue.

    ipa_zip_register needs to be called before invoking this API.
 
    The callback ipa_zip_cb_type is invoked with callback event as
    IPA_ZIP_CB_EVENT_CMD upon completion of the command.
    The parameters cmd_cb_tag and cmd_cb_dst_buf_size are valid only for
    the command callback event type.    

    @param 
    ipa_zip_cmd_req_type_s, Config params passed to IPA driver
 
    @return 
    IPA_SUCCESS, when all params passed error checking and request 
    is enqueued.
    IPA_ERR_INVALID_ARGUMENT, when the parameters passed in the function are
    incorrect.
    IPA_ERR_MAX_REQS_RCHD, when the max outstanding requests have reached.
*/ 
/*=========================================================================*/
ipa_zip_err_code_e ipa_zip_cmd_req_def_prio
(
  ipa_zip_cmd_req_type_s  *req_s_ptr
);

/*===========================================================================

  FUNCTION:  ipa_zip_cmd_req_hi_prio

===========================================================================*/
/*!
    @brief
    This API enqueues a request for comp/decomp command to IPA driver in
    the high priority queue.

    ipa_zip_register needs to be called before invoking this API.
 
    The callback ipa_zip_cb_type is invoked with callback event as
    IPA_ZIP_CB_EVENT_CMD upon completion of the command.
    The parameters cmd_cb_tag and cmd_cb_dst_buf_size are valid only for
    the command callback event type.    
 
    @param 
    ipa_zip_cmd_req_type_s, Config params passed to IPA driver
 
    @return 
    IPA_SUCCESS, when all params passed error checking and request 
    is enqueued.
    IPA_ERR_INVALID_ARGUMENT, when the parameters passed in the function are
    incorrect.
    IPA_ERR_MAX_REQS_RCHD, when the max outstanding requests have reached.
*/ 
/*=========================================================================*/
ipa_zip_err_code_e ipa_zip_cmd_req_hi_prio
(
  ipa_zip_cmd_req_type_s  *req_s_ptr
);

/*===========================================================================

  FUNCTION:  ipa_zip_clk_req

===========================================================================*/
/*!
    @brief
    This async API requests for highest/lowest IPA clock. The vote would be 
    sticky until the next API invocation or if ipa_zip_unregister is called.
 
    ipa_zip_register needs to be called before invoking this API.

    API is async, API return does not mean IPA HW clock request has taken
    effect.

    @param 
    ipa_zip_clk_req_type_e, Clock vote request to IPA driver
 
    @return 
    None

*/ 
/*=========================================================================*/
void ipa_zip_clk_req
(
  ipa_zip_clk_req_type_e  req
);

/*===========================================================================

  FUNCTION:  IPA_WAN_PDCP_TAG_CIPHER_PARAMS

===========================================================================*/
/*!
    Tags the passed pdcp_sdu_ptr with the provided ciphering parameters
    to be used when transmitting.
   
    This will overwrite any data in the app pointer and app fields in the
    passed in dsm item.
   
*/ 
/*=========================================================================*/
void ipa_wan_pdcp_tag_cipher_params
(
  dsm_item_type         *pdcp_sdu_ptr,
  uint32                 count_c,
  uint8                  bearer_id,
  uint8                  ciph_key_idx,
  ipa_wan_cipher_algo_e  cipher_algo
);

/*==============================================================================

  FUNCTION:  IPA_WAN_PDCP_WRITE_CIPER_KEY

==============================================================================*/
/*!
    @brief
    This API is invoked by PDCP to write DRB PDCP ciphering key to IPA internal
    memory.  
    
    @return This API returns the index at which the given key was stored in IPA
    internal memory
*/
/*============================================================================*/
uint8 ipa_wan_pdcp_write_cipher_key
(
  ipa_wan_subscription_id_t subs_id,     /*! Subscription id */
  uint8 ciph_key[IPA_WAN_CIPH_KEY_SIZE]  /*!< 16 byte key value > */
);

/*=============================================================================

  FUNCTION:  ipa_wan_pdcp_get_cipher_key_ptr

=============================================================================*/
/*!
    @brief
    This API returns ciphering key as written in the IPA internal memory at
    index provided as argument.
 
    @param key_index, index at which the ciphering key is requested.
    @param ciph_key[], output parameter into which the ciphering key at given
           index will be copied.
 
    @return
    @retval FALSE, if key_index passed was out of bound.
    @retval TRUE, if key_index was correct and a ciphering key at that index
            was copied in the ciph_key output parameter.
*/
/*===========================================================================*/
boolean ipa_wan_pdcp_get_cipher_key_ptr
(
  uint8 key_index,
  uint8 ciph_key[IPA_WAN_CIPH_KEY_SIZE]
);

/*===========================================================================

  FUNCTION:  IPA_WAN_REGISTER_BEARER_ERR_IND

===========================================================================*/
/*!
   This function registers a callback function with IPA driver to detect
   any errors on the bearer. 
     
    @param
    err_pkt_thresh, Number of error packets threshold on which to detect the
                    error on that bearer
    err_ind_cb_fn, Callback function to be registered with the driver
*/ 
/*=========================================================================*/
void ipa_wan_register_bearer_err_ind
(
  uint32                         err_pkt_thresh,
  ipa_wan_bearer_err_ind_cb_type err_ind_cb_fn
);

/*===========================================================================

  FUNCTION:  IPA_WAN_DEREGISTER_BEARER_ERR_IND

===========================================================================*/
/*!
   This function deregisters a callback function with IPA driver. 
     
    @param
    none.
*/ 
/*=========================================================================*/
void ipa_wan_deregister_bearer_err_ind(void);


/*===========================================================================

             IPA Driver <-> SIO Interface DEFINITIONS and TYPES

===========================================================================*/

/*===========================================================================

             IPA Driver <-> Customer Helper DEFINITIONS and TYPES

===========================================================================*/

/*=============================================================================

  FUNCTION:  ipa_get_version

=============================================================================*/
/*!
  @brief Provide IPA library version for external module
 
  @return
    char *: pointer to char string
*/
/*===========================================================================*/
const char *ipa_get_version( void );

/*===========================================================================

             IPA Driver <-> Low Latency Interface DEFINITIONS and TYPES

===========================================================================*/
/*! @brief Enum of the traffic state */
typedef enum
{
  /*! Low latency traffic has stopped */
  IPA_IPFLTR_EXT_LOW_LAT_TRAFFIC_STATE_STOP,
  /*! Low latency traffic has started */
  IPA_IPFLTR_EXT_LOW_LAT_TRAFFIC_STATE_START
} ipa_ipfltr_ext_low_lat_traffic_state_e;

/*! @brief Enum defining the different external clients that register for 
  low latency indications */
typedef enum
{
  /*! LTE lower layers client*/
  IPA_IPFLTR_EXT_LOW_LAT_TRAFFIC_CLIENT_LTE,
  /*! WCDMA lower layers client */
  IPA_IPFLTR_EXT_LOW_LAT_TRAFFIC_CLIENT_WCDMA,
  /*! TDSCDMA lower layers client */
  IPA_IPFLTR_EXT_LOW_LAT_TRAFFIC_CLIENT_TDSCDMA,
  /*! GERAN lower layers client */
  IPA_IPFLTR_EXT_LOW_LAT_TRAFFIC_CLIENT_GERAN,
  /*! Data Services client */
  IPA_IPFLTR_EXT_LOW_LAT_TRAFFIC_CLIENT_DATA,
  IPA_IPFLTR_EXT_LOW_LAT_TRAFFIC_CLIENT_MAX
} ipa_ipfltr_ext_low_lat_traffic_client_e;

/*! @brief Structure to be passed to the clients with the required info 
  for indicating low latency traffic */
typedef struct
{
  /*! Enum to indicate the start/end of low latency traffic */
  ipa_ipfltr_ext_low_lat_traffic_state_e traffic_state;
  /*! Index of the filter that triggered this low latency callback */
  uint32                                 filter_result;
} ipa_ipfltr_ext_low_lat_traffic_ind_info_s;

/*! @brief Callback function signature to be called by the driver once we detect 
  the start/end of low latency traffic */
typedef void (*ipa_ipfltr_ext_low_lat_traffic_ind_cb_type)
(
  ipa_ipfltr_ext_low_lat_traffic_ind_info_s *info_ptr
);

/*=============================================================================

  FUNCTION:  IPA_IPFLTR_EXT_REGISTER_LOW_LAT_IND_CB

=============================================================================*/
/*!
  @brief
  This is an external interface API. This API registers a callback function
  of a client with the IPA driver which will be invoked when any of the packets
  matching LOW_LATENCY_FILTERS are detected in the driver. Each client
  can only register one callback function.
 
  @param
  client_id, Id of the client who is registering the callback     
  low_latency_cb, Callback function pointer to be invoked when low latency
                  traffic is detected
 
  @return
*/
/*===========================================================================*/
void ipa_ipfltr_ext_register_low_latency_ind_cb
(
  ipa_ipfltr_ext_low_lat_traffic_client_e    client_id,
  ipa_ipfltr_ext_low_lat_traffic_ind_cb_type low_lat_ind_cb_fn
);

/*=============================================================================

  FUNCTION:  IPA_IPFLTR_EXT_DEREGISTER_LOW_LAT_IND_CB

=============================================================================*/
/*!
  @brief
  This is an external interface API. This API deregisters the callback function
  of a client with the IPA driver.
 
  @param
  client_id, Id of the client who is registering the callback     
 
  @return
*/
/*===========================================================================*/
void ipa_ipfltr_ext_deregister_low_latency_ind_cb
(
  ipa_ipfltr_ext_low_lat_traffic_client_e client_id
);

/*===========================================================================

    IPA Driver <-> External Stats/Debug Logging DEFINITIONS and TYPES

===========================================================================*/
/*! Logging first 20 bytes of dropped pkt. DO NOT MODIFY */
#define IPA_STATS_UL_PKT_DROP_LOG_LEN  20

/*! @brief UL Drop Pkt reason */
typedef enum
{
  /*! Default */
  IPA_UL_DEAGGR_DROP_REASON_NONE,
  /*! Received bad inserted hdr from packet status */
  IPA_UL_DEAGGR_DROP_REASON_BAD_INS_HDR,
  /*! Packet received from invalid source endpoint */
  IPA_UL_DEAGGR_DROP_REASON_BAD_ENDP,
  /*! Received packet with bad length */
  IPA_UL_DEAGGR_DROP_REASON_BAD_LEN,
  /*! Bad checksum trailer */
  IPA_UL_DEAGGR_DROP_REASON_BAD_CKSUM_TRLER,
  /*! Received exception packet */
  IPA_UL_DEAGGR_DROP_REASON_EXCEPTION,
  /*! Packet drop due to CFM flow control */
  IPA_UL_DEAGGR_DROP_REASON_CFM,
  /*! Packet drop due to invalid QMAP Mux ID */
  IPA_UL_DEAGGR_DROP_REASON_BAD_QMAP_MUX_ID,
  /*! Packet drop due to corrupted QMAP header */
  IPA_UL_DEAGGR_DROP_REASON_BAD_QMAP_FRAME,
  /*! Packet drop due to packet matching UL firewall drop all rule */
  IPA_UL_DEAGGR_DROP_REASON_UL_FIREWALL_DROP_ALL

} ipa_stats_ul_deaggr_drop_reason_e;

typedef struct
{
  /*! Length of pkt dropped */
  uint16                            pkt_len;
  /*! Reason of pkt drop */
  ipa_stats_ul_deaggr_drop_reason_e drop_reason;
  /*! Copy first 20 bytes from raw pkt dropped */
  uint8                             pkt_dump[IPA_STATS_UL_PKT_DROP_LOG_LEN];
} ipa_stats_ul_pkt_drop_cb_s;

/*! @brief Callback function signature to be called by the driver once
 *  detect a pkt drop on UL */
typedef void (*ipa_stats_ul_pkt_drop_cb_fn_type)
(
  ipa_stats_ul_pkt_drop_cb_s *params
);

/*! Stats for debugging purpose */
typedef struct
{
  /*! Total number of DL pkts to host */
  uint32 total_dl_pkts;
  /*! Total number of DL bytes to host */
  uint32 total_dl_bytes;
  /*! Total number of UL pkts to host */
  uint32 total_ul_pkts;
  /*! Total number of UL bytes to host */
  uint32 total_ul_bytes;
} ipa_stats_query_s;

/*! Extended stats for debugging purpose */
typedef struct
{
  /*! Total number of DL pkts to host */
  uint32 total_dl_pkts;
  /*! Total number of DL bytes to host */
  uint32 total_dl_bytes;
  /*! Total number of UL pkts to host */
  uint32 total_ul_pkts;
  /*! Total number of UL bytes to host */
  uint32 total_ul_bytes;
  /*! Total number of DL pkts dropped */
  uint32 total_dl_drop_pkts;
  /*! Total number of DL bytes dropped */
  uint32 total_dl_drop_bytes;
  /*! Total number of UL pkts dropped */
  uint32 total_ul_drop_pkts;
  /*! Total number of UL bytes dropped */
  uint32 total_ul_drop_bytes;
  /*! Current num of outstanding pkts on DL*/
  uint32 num_outstanding_pkts;
} ipa_stats_query_ex_s;

/*=============================================================================

  FUNCTION:  ipa_stats_query

=============================================================================*/
/*!
    @brief
    This API is invoked by external clients to retrieve IPA stats
 
*/ 
/*===========================================================================*/
void ipa_stats_query
(
  ipa_stats_query_s *stats_ptr
);

/*=============================================================================

  FUNCTION:  ipa_stats_query_ex

=============================================================================*/
/*!
    @brief
    This API is invoked by external clients to retrieve IPA stats
 
*/ 
/*===========================================================================*/
void ipa_stats_query_ex
(
  ipa_stats_query_ex_s *stats_ptr
);

/*=============================================================================

  FUNCTION:  ipa_stats_ul_pkt_drop_cb_reg

=============================================================================*/
/*!
    @brief
    This API is invoked by clients to register for notification cb from IPA
    driver when a UL pkt drop is detected
 
    @param 
    ipa_stats_ul_pkt_drop_cb_fn_type, callback function to be invoked when 
                                       the driver detects a UL pkt drop. 
    ipa_stats_ul_pkt_drop_cb_s, callback data to be filled in.

    The callback registered here should copy the pkt_dump within the callback
    handler immediately. No guarantee on correctness of pkt_dump after callback
    invocation. This callback needs to be lightweight as it is called on UL
    datapath and this handling delay will be accounted into overall delay
    on UL.
*/ 
/*===========================================================================*/
void ipa_stats_ul_pkt_drop_cb_reg
(
  ipa_stats_ul_pkt_drop_cb_fn_type pkt_drop_cb_fn,
  ipa_stats_ul_pkt_drop_cb_s *params
);

#endif /* IPA_API_H */
