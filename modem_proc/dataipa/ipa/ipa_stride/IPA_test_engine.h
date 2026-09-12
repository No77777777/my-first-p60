/*!
  @file IPA_test_engine.h

  @brief
    IPA Test engine header file
*/                               
/*==============================================================================

  Copyright (c) 2013-14 QUALCOMM Technologies Incorporated. All Rights Reserved

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

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/dataipa.mpss/3.2/ipa/ipa_stride/IPA_test_engine.h#1 $
$Date: 2019/07/05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
05/30/17   db      CR13276332: Final submit for Napali BU.
07/24/14   mi      Adding CLAT UL static filter test cases.
07/20/14   SaCh    Adding support for IPA2.5 exception/error scenarios.
07/15/14   mi      IPA 2.1 regression test fixes. 
07/13/14   SaCh    Fixes in Dl/UL/Loopback CLAT areas.
06/20/14   mi      Added IPV6 fragmentation support.
06/16/14   SaCh    Added changes to include tcp_ack_pkt_type in tcp protocol 
                   parameters in ip_pkt_cfg passed from the testcase.
05/14/14   mi      IPA2.5 CLAT test support.
02/17/15   mi      CR#739684: IPA ZIP feature related test changes.
07/15/14   mi      IPA 2.1 regression test fixes. 
06/12/14   mi      IPA 2.1 Fixes.
06/03/14   SaCh    Moving all data type definitions to ipa_test_engine.h.
05/25/14   rp      Test engine fixes/cleanup for issues seen on full stack build
04/16/14   pgm     IPA2.1 aggr/deaggr test support.
03/24/14   mi      Regression test fixes.
03/06/14   mi      Added fragmentation test changes.
02/28/14   mi      Bearer suspend/resume and deregistration test changes.
01/17/14   SaCh    CR#600855: Expecting SIO_stream_id instead of uid value in 
                   ipa_ipfltr_alloc_rule_handle API for QOS rule.
12/11/13   mi      Added support for sio config mapping API changes.
12/09/13   pgm     BAM2BAM pipe config and flow control changes.
12/05/13   mi      Added DPL support. 
10/15/13   rp      DSM Chain fix & seed user cfg
09/30/13   pgm     Support for testing UL priority packets.
08/22/13   rp      Initial Checkin

==============================================================================*/
#ifndef IPA_TEST_ENGINE_H
#define IPA_TEST_ENGINE_H
/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include <comdef.h>
#include "IPA_test_mutils_zuc.h"
#include "IPA_test_util_ciph_aes_ctr_f8.h"
#include "IPA_test_util_ciph_snow3Gf8.h"

extern "C"
{
#include <ipa.h>
#include <ipa_cfg.h>
#include <ipa_stats.h>
#include <ipa_ctl.h>
#include <ipa_util.h>
#include <ipa_test_per_sim.h>
#include "ipa_ul.h"
#include "ipa_dl.h"
#include "ipa_hal.h"
#include "ipa_cfg.h"
#include "ipa_common.h"
}

/*==============================================================================

                         MACROS

==============================================================================*/
/*! Max number of peripheral sims that we wan't to configure for any test */
#define IPA_TEST_ENGINE_PER_SIM_NUM_MAX           IPA_SIO_MAX_PHYSICAL_SIO_STREAMS

/*! Max value of mux id */
#define IPA_TEST_ENGINE_MUX_ID_NUM_MAX            255

/*! Max value of subscribtion id */
#define IPA_TEST_ENGINE_SUBS_ID_NUM_MAX           255

/*! Max number of mux channels that we wan't to configure on any SIM */
#define IPA_TEST_ENGINE_PER_SIM_LOG_CH_NUM_MAX    8

/*! MAx number of bearers that we wan't to configure for any test */
#define IPA_TEST_ENGINE_BEARER_NUM_MAX            IPA_WAN_MAX_BEARERS

/*! Max number of subscription that we want to support */
#define IPA_TEST_ENGINE_SUBS_NUM_MAX              2

/*! Max number of logical ports supported */
#define IPA_TEST_ENGINE_LOG_CH_NUM_MAX            32

/*! Max number of physical ports. One per sim */
#define IPA_TEST_ENGINE_PHY_CH_NUM_MAX            IPA_TEST_ENGINE_PER_SIM_NUM_MAX

/*! Max num of packets per test case */
#define IPA_TEST_ENGINE_IP_PKT_NUM_MAX            80

/*! Max num of NDPs per test case */
#define IPA_TEST_ENGINE_NDP_NUM_MAX               2

/*! Max packet size to be generated */
#define IPA_TEST_ENGINE_IP_PKT_SIZE_MAX           1500

#define IPA_TEST_ENGINE_AGGR_IP_PKT_SIZE_MAX      4100

#define IPA_TEST_ENGINE_ZIP_OUTPUT_BUF_SIZE_MAX   4400

/*! Max Cipher params instances */
#define IPA_TEST_ENGINE_CIPH_PARAMS_NUM_MAX       15

/*! Max QMAP pad bytes*/
#define IPA_TEST_ENGINE_QMAP_PAD_BYTES_MAX        10

/*! QMAP hdr SIZE*/
#define IPA_TEST_ENGINE_QMAP_HDR_SIZE_MAX         4

/*! Max Aggregated buffer SIZE  */
#define IPA_TEST_ENGINE_AGGR_FRAME_SIZE_MAX  IPA_TEST_ENGINE_QMAP_HDR_SIZE_MAX + \
                                             IPA_TEST_ENGINE_IP_PKT_SIZE_MAX + 20 + \
                                             IPA_TEST_ENGINE_QMAP_PAD_BYTES_MAX 

/*! Max Aggregated buffer SIZE  */
//#define IPA_TEST_ENGINE_TOTAL_AGGR_FRAME_SIZE_MAX  (13000)
#define IPA_TEST_ENGINE_TOTAL_AGGR_FRAME_SIZE_MAX (4 * IPA_TEST_ENGINE_AGGR_IP_PKT_SIZE_MAX)

/*! Max value of the cipher key to be set */
#define IPA_TEST_ENGINE_CIPH_KEY_VAL_MAX          127

/*! Number of Ciphering key per subscriber */
#define IPA_TEST_ENGINE_CIPH_KEY_NUM_PER_SUBS     8  

/*! Number of Ciphering key per algo */
#define IPA_TEST_ENGINE_CIPH_KEY_NUM_PER_ALGO     2  

/*! AES ciphering key ofset for a subscription */
#define IPA_TEST_ENGINE_CIPH_PARAMS_AES_OFST      0  
/*! SNOW 3G ciphering key ofset for a subscription */
#define IPA_TEST_ENGINE_CIPH_PARAMS_SNOW_3G_OFST  2  
/*! ZUC ciphering key ofset for a subscription */
#define IPA_TEST_ENGINE_CIPH_PARAMS_ZUC_OFST      4  
/*! None ciphering key ofset for a subscription */
#define IPA_TEST_ENGINE_CIPH_PARAMS_NONE_OFST     6  
/*! None ciphering key ofset for a subscription */
#define IPA_TEST_ENGINE_CIPH_PARAMS_END_OFST      8  

/*! Split the packet across a max of this many DSM items */
#define IPA_TEST_ENGINE_DSM_CHAIN_NUM_MAX         45 

/*! Split the packet across this many DSM items to trigger DL collapse 
  logic */
#define IPA_TEST_ENGINE_DSM_CHAIN_NUM_COLLAPSE    40 

/*! Split the packet across an avg of this many DSM items */
#define IPA_TEST_ENGINE_DSM_CHAIN_NUM_AVG         10 

/*! Fraction of packets with DSM split more than avg */
#define IPA_TEST_ENGINE_DSM_CHAIN_FRACTION_AVG    4

/*! Max no of frames and aggr pkts for aggr/deaggr tests */
#define IPA_TEST_ENGINE_AGGR_FRAME_MAX            4
#define IPA_TEST_ENGINE_AGGR_PKTS_MAX             4

/*! History of states traversed */
#define IPA_TEST_ENGINE_STATE_TRACE_LEN           100
/*==============================================================================

                   INTERNAL DEFINITIONS AND TYPES

==============================================================================*/
/*==============================================================================
                   Input Params to the Engine
==============================================================================*/
/*! @brief Endp Configuration input to the test engine. Mux ID's will be 
  randomly generated */
typedef struct
{
  /*! Number of peripheral sim endpoints to be used for test */
  uint8                     num_per_sim;
  /*! If the test wants to set some delay in the SIM tx */
  uint8                     sim_tx_delay;
  /*! Endpoint config to be used for each of the peripheral sim */
  ipa_sio_endpoint_config_s sim_endp_cfg[IPA_TEST_ENGINE_PER_SIM_NUM_MAX];
  /*! To indicate System2BAM or BAM2BAM cfg for the sim pipe.TRUE-->B2B cfg*/
  boolean                   sim_endp_b2b_cfg[IPA_TEST_ENGINE_PER_SIM_NUM_MAX];
  /*! Number of mux'ed logical channels to be used on each peripheral sim. 
    (if aggr/hdr protocol supports muxing)*/
  uint8                     num_log_ch[IPA_TEST_ENGINE_PER_SIM_NUM_MAX];
  uint8 					ic_type;
} ipa_test_engine_endp_cfg_s;

/*! @brief Bearer Configuration input to the test engine. 
  BEARER ID, UID, Subsys ID will be randomly generated by the test engine */
typedef struct
{
  /*! Number of bearers required for this test */
  uint8                 num_bearers;        
  /*! Flag to indicate how many of the bearers will be bridged/unbridged */
  uint8                 is_bearer_bridged[IPA_TEST_ENGINE_BEARER_NUM_MAX];
  /*! Technology to use for all the bearers */
  ipa_wan_bearer_tech_e bearer_tech[IPA_TEST_ENGINE_BEARER_NUM_MAX];
  /*! Flag to indicate if we want to turn on ciphering or not. 
    Applicable to DL test cases only only if bearer is LTE */
  uint8                 is_ciph_enabled;
  /*!Flag to indicate if we want to deregister the bearer in between 
    packet transfer*/
  uint8                 is_bearer_dereg_case;
  uint8                 is_bearer_susp_case_1;
  uint8                 is_bearer_susp_case_2;
} ipa_test_engine_wan_cfg_s;

typedef struct
{
  /*! Source IPv4 address */
  uint32 v4_src_addr;
  /*! Src V6 prefix len */
  ipa_clat_prefix_len_e v6_src_prefix_len;
  /*! Dest V6 prefix len */
  ipa_clat_prefix_len_e v6_dest_prefix_len;
  /*! Src V6 prefix */
  uint8 v6_src_prefix[16] __attribute__((aligned(16)));
  /*! Dest V6 prefix */
  uint8 v6_dest_prefix[16] __attribute__((aligned(16)));
  /*! Destination IPV4 address*/
  uint32 v4_dest_addr;
  /*! Extension header length for the loopback case*/
  uint16 extn_hdr_len;
}ipa_test_engine_clat_cntxt_info_type;

/*! Enums for extension header types */
typedef enum
{
  /*! No error scenarios: default case*/
  IPA_TEST_ENGINE_CLAT_ERR_NONE = 0,
  /*! IHL exception error scenario: when the 
    extension header options length > 256 bytes*/
  IPA_TEST_ENGINE_CLAT_ERR_IPV6_EXT_HDR_EXCEEDED = 1
}ipa_test_engine_clat_err_scenario_e;

/*! Input config for CLAT */
typedef struct
{
  /*! Number of PDNs with CLAT configured */
  uint32                                num_clat_pdn;  
  /*! Global CLAT config */
  ipa_clat_global_cntxt_info_type       global_clat_cfg;
  /*! Array of CLAT cntxt info structure */
  ipa_test_engine_clat_cntxt_info_type  cntxt_info[IPA_TEST_ENGINE_LOG_CH_NUM_MAX];
  /*! If its an error case, the error scenario to be tested */
  ipa_test_engine_clat_err_scenario_e   err_scenario;
  /*! count of exceptions */
  uint8                                 exception_cnt;       
} ipa_test_engine_clat_cfg_s;

/*! @brief List of IP protocols supported */
typedef enum
{
  IPA_TEST_ENGINE_IP_PKT_PROT_ICMP,
  IPA_TEST_ENGINE_IP_PKT_PROT_TCP,
  IPA_TEST_ENGINE_IP_PKT_PROT_UDP,
  IPA_TEST_ENGINE_IP_PKT_PROT_MAX
} ipa_test_engine_ip_pkt_prot_e;

/*! Enums for extension header types */
typedef enum
{
  IPA_TEST_ENGINE_ROUTING_EXTN_HDR_TYPE   = 43,
  IPA_TEST_ENGINE_FRAG_EXTN_HDR_TYPE      = 44,
  IPA_TEST_ENGINE_DEST_OPTS_EXTN_HDR_TYPE = 60,
  IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE       = 0XFF
} ipa_test_engine_extn_hdr_type_e;

/*! @brief Protocol Specific params if any */
typedef union
{
  /*! ICMP Protocol params */
  struct _icmp
  {
    uint8 type;
    uint8 code;
  } icmp;
  /*! UDP Protocol params */
  struct _udp
  {
    uint16 src_port;
    uint16 dst_port;
  } udp;
  /*! TCP Protocol params */
  struct _tcp
  {
    uint16 src_port;
    uint16 dst_port;
    uint32 seq_num;
    uint32 ack_num;
    uint8  tcp_ack_pkt_type;
  } tcp;
} ipa_test_engine_ip_pkt_prot_params_u;

/*! @brief Some params of IP packet that user might want 
  to set */
typedef struct
{
  /*! Frag len for this packet, if applicable*/
  uint16  frag_pkt_len;
 /*! Max pkt size that can be used for 2.1 Aggr/Deaggr tests */
  uint16 max_pkt_size;
  boolean ooo_frag_case;
  ipa_test_engine_extn_hdr_type_e   ipv6_ext_hdr_type;
  uint8                             ipv6_ext_hdr_opt_len;
} ipa_test_engine_ip_pkt_params_s;

/*! @brief IP Packet Configuration input to the test engine */
typedef struct
{
  /*! Number of IP packets to generate for the test */
  uint8                                 num_pkts; 
  /*! Version of IP Packet */
  ipa_ip_type_e                         version;
  /*! Length of the IP payload required. 
    Does not include the IP header length and protocol header length
    Value of 0 means random num will be generated by engine for each pkt */
  uint16                                payload_len;
  /*! Length of the IP header Default: 20 for V4. 40 for V6 */
  uint16                                ip_hdr_len;
  /*! Packet Protocol: TCP/UDP/ICMP/etc */
  ipa_test_engine_ip_pkt_prot_e         prot;
  /*! Packet Protocol params */
  ipa_test_engine_ip_pkt_params_s       params;
  /*! Packet Protocol Type params: TCP/UDP/ICMP/etc */
  ipa_test_engine_ip_pkt_prot_params_u  prot_params;
  /*! Flag to indicate if we want random/patternised data in payload */
  uint8                                 is_random_data;
  /*! Each Packet to be spread across mutliple DSM items. 
    Value of 0 means random num will be generated by engine for each pkt */
  uint8                                 num_dsm_items;  
} ipa_test_engine_ip_pkt_cfg_s;


/*! @brief Type of test to be run */
typedef enum
{
  IPA_TEST_ENGINE_ZIP_ERR_R0_01 = 0,
  IPA_TEST_ENGINE_ZIP_ERR_R0_02 = 1,
  IPA_TEST_ENGINE_ZIP_ERR_R0_03 = 2,
  IPA_TEST_ENGINE_ZIP_ERR_RW_01 = 3,
  IPA_TEST_ENGINE_ZIP_ERR_RW_02 = 4,
  IPA_TEST_ENGINE_ZIP_ERR_RW_03 = 5,
  IPA_TEST_ENGINE_ZIP_ERR_MAX,
} ipa_test_engine_zip_err_type_e;

/*! @brief Zip configuration input for the test engine */
typedef struct
{
  /* Maximum High priority requests configured*/
  uint8 max_hi_prio_reqs;
  /* Maximum Default priority requests configured */
  uint8 max_def_prio_reqs;
  /* Number of high priority requests */
  uint8 num_hi_prio_reqs;
  /* Number of default priority requests */
  uint8 num_def_prio_reqs;
  /* Is HOLB verification enabled */
  uint8 is_holb_ver_enabled;
  uint8 *mirror_hpq_resp_dest_buf[8];
  uint8 *mirror_dpq_resp_dest_buf[8];
  /* Pointer to the dictionary to be loaded */
  ipa_zip_dict_type_s  dict_type;
  /* Error type to be tested  */
  ipa_test_engine_zip_err_type_e err_type;
   /* Array of High priority zip requests to be sent */
  ipa_zip_cmd_req_type_s hp_req_arr[8];
  /* Array of default priority zip requests to be sent */
  ipa_zip_cmd_req_type_s dp_req_arr[8];
  /* Array of error codes for ZIP error scenarios */
  uint8 error_code[8];
  /* Array of error addresses for ZIP error scenarios */
  uint16 bytes_left[8];
} ipa_test_engine_zip_cfg_s;

/*! @brief Zip configuration input for the test engine */
typedef struct
{
  uint8  register_complete;
  uint8  dict_load_complete;
  uint8  *hi_prio_req_dest_buf[8] ;
  uint8  *def_prio_req_dest_buf[8];
  uint32 num_req;
  uint32 num_resp;
  uint32 num_success_resp;
  uint32 num_resp_rx_wdog_prev;
  uint32 cb_tag_rcvd[16];
  uint32 dest_buf_len_rcvd[16];
  /* Array of error codes for ZIP error scenarios */
  uint8 error_code[8];
  /* Array of error addresses for ZIP error scenarios */
  uint16 bytes_left[8];
} ipa_test_engine_zip_s;


/*! @brief Type of test to be run */
typedef enum
{
  IPA_TEST_ENGINE_TEST_TYPE_DL,
  IPA_TEST_ENGINE_TEST_TYPE_UL,
  IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK,
  IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK,
  IPA_TEST_ENGINE_TEST_TYPE_DL_HOLB,
  IPA_TEST_ENGINE_TEST_TYPE_UL_HOLB
} ipa_test_engine_test_type_e;

/*! @brief Type of Non-Pkt Wait tests. For tests where result depends on 
    IPA Stats.and not pkts being received back*/
typedef enum
{
  /* All tests where pkts are received back in the test engine */
  IPA_TEST_ENGINE_RX_PKT_WAIT,
  /* Other tests where result depends on right status */
  IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_HIST_STATS,
  IPA_TEST_ENGINE_RX_PKT_WAIT_PREVEOT_BYTE_THRESHOLD,
  IPA_TEST_ENGINE_RX_PKT_WAIT_DROPPED_PKT_BYTE_THRESHOLD,
  IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING,
  IPA_TEST_ENGINE_RX_PKT_WAIT_AGGR_FORCE_CLOSE_PIPES,
  IPA_TEST_ENGINE_RX_PKT_WAIT_EMPTY_DSM_BUFFER_CHAIN,
  IPA_TEST_ENGINE_RX_PKT_WAIT_TX_THRESHOLD_INT_STATS
} ipa_test_engine_rx_pkt_test_type_e;

/*! @brief Filter Rule categories for Directed and Random flr tests */
typedef enum
{
  IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL = 0,
  IPA_TEST_ENGINE_FLTR_RULE_UL_QOS = 1,
  IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_AND_QOS = 2,
  IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_QOS_TCP_ACK = 3,
  IPA_TEST_ENGINE_FLTR_RULE_DL_SOCKET = 4,
  IPA_TEST_ENGINE_FLTR_RULE_UL_V4_FRAG = 5,
  IPA_TEST_ENGINE_FLTR_RULE_MAX
}ipa_test_engine_fltr_cat_type_e;

/*! @brief UL Static fltr Directed Test case types*/
typedef enum
{
  /*! @breif V4 UL Static+QOS fltr tests*/
  IPA_TEST_ENGINE_FLTR_TYPE_FRAG_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_DHCP_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_CLAT_ANY_ICMP_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_CLAT_UDP_CHKSUM_ZERO_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_CLAT_TTL_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_CLAT_DNS_PROXY_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_CLAT_FRAG_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_UL_STATIC_MAX_V4,
  /*! @breif V6 UL Static+QOS fltr tests*/
  IPA_TEST_ENGINE_FLTR_TYPE_FRAG_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_DHCP_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_ICMP_REQ_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_ICMP_REP_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_ICMP_NDP_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_UL_STATIC_MAX,
   /*! @breif UL QOS fltr tests*/
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE1_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE2_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE3_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE4_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE5_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE6_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE7_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE8_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE_MAX_V4,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE9_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE10_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE11_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE12_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE13_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE14_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE15_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE16_V6,
  IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX
} ipa_test_engine_fltr_type_e;

/*! The signals used by the QTF task*/
typedef enum
{
  /*! Signal is set when the data is received*/
  IPA_TEST_ENGINE_WAIT_SIG               = 0x1,
  /*! Signal is set when the media connect callback received*/
  IPA_TEST_ENGINE_ENDP_WAIT_SIG          = 0x2,
  /*! Signal is set when the DS event callback is received for Bearer Suspend*/
  IPA_TEST_ENGINE_BEARER_SUSP_SIG        = 0x4, 
  /*! Signal is set when the DS event callback is received for Bearer Active*/
  IPA_TEST_ENGINE_BEARER_ACTIVE_SIG      = 0x8,
  /*! Signal is set when the DS event callback is received for Bearer Deregistered*/
  IPA_TEST_ENGINE_BEARER_DEREG_SIG       = 0x10,
   /*! Signal is set when the DS event callback is received for Bearer Registered*/
  IPA_TEST_ENGINE_BEARER_REG_SIG         = 0x20,
  /*! Signal is set when CLAT ocnfiguration complete callback is called. */
  IPA_TEST_ENGINE_CLAT_CONFIG_SIG        = 0x40,
  /*! Signal is set when CLAT assoc sio ocnfiguration complete callback is called. */
  IPA_TEST_ENGINE_CLAT_ASSOC_SIO_SIG     = 0x80,
  /*! Signal is set when CLAT translation complete callback is called. */
  IPA_TEST_ENGINE_CLAT_TRANSLATION_SIG   = 0x100,
  /*! Signal is set when CLAT dereg complete callback is called. */
  IPA_TEST_ENGINE_CLAT_DEREG_SIG         = 0x200,
    /*! Signal is set when the Zip Registration complete cb is received*/
  IPA_TEST_ENGINE_ZIP_REG_DONE_SIG       =  0x400,
  /*! Signal is set when the Dictionary load complete cb is received*/
  IPA_TEST_ENGINE_ZIP_DICT_LOAD_DONE_SIG =  0x800,
  /*! Signal is set when the response complete cb is received*/
  IPA_TEST_ENGINE_ZIP_RESP_RCVD_SIG      =  0x1000,
 /*! Signal is set when the error response complete cb is received*/
  IPA_TEST_ENGINE_ZIP_ERR_RESP_RCVD_SIG  =  0x2000,
} ipa_test_engine_task_sig_e;

/*! @brief Filter params for this test */
typedef struct
{
  /*! Fltr Rule Category type*/
  ipa_test_engine_fltr_cat_type_e fltr_rule_cat_type;
  /*! Ftlr type in a category*/
  ipa_test_engine_fltr_type_e     fltr_rule_type;
  /*! No of DL Socket Filters*/
  uint16   num_dl_sckt_fltr;
  /*! No of UL QOS Filters*/
  uint16   num_ul_qos_fltr;
  /*!Enable/Disable Filter Framework */
  uint8    is_filter_enabled;
  /*!For TCP ACK and QOS Filtering*/
  boolean  is_tcp_ack_qos_rule; 
} ipa_test_engine_fltr_cfg_s;

/*! @brief Aggr Error handling test scenarios*/
typedef enum
{
  /*! QMAP deaggr error cases*/

  /* pkt_len > EOT frame len for every first pkt in frame */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT,           
  /* pkt_len > EOT frame len for random pkt */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT_1,     
  /*EOT len > Max msg size(16k) */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE,  
  /*EOT len > Max msg size(16k) - zeroed qmap hdr*/
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE_1,
  /* No Padding bytes */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_NO_PAD_BYTES,
  /* EOT less than the minimal Frame size */   
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOT_LT_MIN_FRAME_SIZE,
  /* msg len > max msg size(as configured in deaggr.max_pkt_len) */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_MSGLEN_GT_MAX_MSG_SIZE,  

  /*!RNDIS */

  /* EOT less than the minimal Frame size(44+14) */   
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_EOT_LT_MIN_FRAME_SIZE,
  /* RNDIS hdr split across 2 buffers */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_SPLIT_2_DESC,
  /* Invalid msg type in the hdr */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_TYPE,
  /* Invalid msg len- msg len > EOT len */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_LEN,
  /* Invalid data offset in the hdr */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_OFFSET,
  /* Invalid data len in the hdr */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_LEN,
  /* msg len > max msg size(as configured in deaggr.max_pkt_len) */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_MSGLEN_GT_MAX_MSG_SIZE,  

  /*!MBIM */

  /* Invalid NTH sig in NTH hdr */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NTH_SIG, 
  /* Invalid NTH hdr len (other than 0x0C)*/
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NTH_HDR_LEN, 
  /* Invalid first NDP idx in NTH hdr */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NDP_IDX_IN_NTH, 
  /* Invalid NDP sig in NDP hdr */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NDP_SIG, 
  /* Invalid Nxt NDP idx in NDP hdr */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NXT_NDP_IDX, 
  /* Invalid Dgrm idx in NDP hdr */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_DGRM_IDX, 
  /* NDP hdr with no terminating zeros */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_NDP_WITH_NO_TERM_ZEROS, 
  /* Invalid MBIM frames among many */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_SEC_EOT_FRAME, 
  /* EOT Len > Max Msg length*/
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_EOTLEN_GT_MAX_MSG_SIZE,
  /* EOT len < min mbim frame size */
  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_EOT_LT_MIN_FRAME_SIZE,

  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MAX
} ipa_test_engine_deaggr_err_type_e;

typedef struct
{
  ipa_test_engine_deaggr_err_type_e err_type;
} ipa_test_engine_deaggr_err_cfg_s;

/*! UL Deaggr error test structure */
typedef struct
{
  /*! QMAP */
  ipa_hw_qmap_hdr_s qmap_hdr_info[IPA_TEST_ENGINE_AGGR_FRAME_MAX] \
                                 [IPA_TEST_ENGINE_AGGR_PKTS_MAX]; 
  ipa_hw_qmap_hdr_s qmap_hdr[IPA_TEST_ENGINE_AGGR_PKTS_MAX]; //Max pkts aggregated for these tests

  /*! RNDIS */
  ipa_hw_rndis_hdr_s rndis_hdr_info[IPA_TEST_ENGINE_AGGR_FRAME_MAX] \
                                        [IPA_TEST_ENGINE_AGGR_PKTS_MAX];
  dsm_item_type * aggr_frame_dsm_ptr;
  dsm_item_type * err_frame_dsm_ptr;
  dsm_item_type * rcvd_frame_dsm_ptr[IPA_TEST_ENGINE_AGGR_FRAME_MAX];
  dsm_item_type * frame_arr[IPA_TEST_ENGINE_AGGR_FRAME_MAX];
  uint16 sent_frame_len_arr[IPA_TEST_ENGINE_AGGR_FRAME_MAX];
  uint16 rcvd_frame_len_arr[IPA_TEST_ENGINE_AGGR_FRAME_MAX];
  uint32 eot_aggr_frame_len_arr[IPA_TEST_ENGINE_AGGR_FRAME_MAX];
  uint16 frame_arr_idx;
  int8 pkt_no_pad_index;
  boolean err_pkt_gen;
  uint16 exception_cnt;
  uint16 iptype_exception_cnt;
  uint16 ihl_exception_cnt;
  uint16 pkt_cnt;
  uint16 frame_cnt;
  uint16 rcvd_frame_cnt;
} ipa_test_engine_deaggr_err_info_s;
  

/*! @brief Top level test engine input params for a test */
typedef struct
{
  /*! Endpoint Cfg */
  ipa_test_engine_endp_cfg_s   endp_cfg;
  /*! Bearer Cfg */
  ipa_test_engine_wan_cfg_s    wan_cfg;
  /*! IP Packet generator Cfg */
  ipa_test_engine_ip_pkt_cfg_s ip_pkt_cfg;
  /*! Filter Cfg */
  ipa_test_engine_fltr_cfg_s   fltr_cfg;
  /*! CLAT Cfg*/
  ipa_test_engine_clat_cfg_s          clat_cfg;
  /*! Deaggr Error Cfg */
  ipa_test_engine_deaggr_err_cfg_s  deaggr_err_cfg;
  /*! IPA ZIP Cfg */
  ipa_test_engine_zip_cfg_s    zip_cfg;     
  /*! Type of test to be run: UL, DL or Loopback */
  ipa_test_engine_test_type_e  test_type;
  /*! Type of Rx pkt test: Pkt-Wait or No-Pkt Wait*/
  ipa_test_engine_rx_pkt_test_type_e  rx_pkt_test_type;
  /*! Boolean to indicate if power colapse testing is enabled or not */
  uint8                        is_pc_enabled;
  /*! Boolean to indicate if DPL enabled */
  uint8                        is_dpl_enabled;
  /*! Boolean to indicate if IPA ZIP enabled */
  uint8                        is_zip_enabled;
  /*! Boolean to indicate if flow control testing is enabled or not */
  uint8                        is_fc_enabled;
  /*! Boolean to indicate if CLAT is enabled */
  uint8                        is_clat_enabled;
  /*! Seed used for this test. Useful for reproducing 
    scenario */
  uint32                       seed;
  /*! Boolean to indicate if HW Replication enabled */
  uint8                        is_hw_replication_enabled;
  uint8 		       ul_holb_enabled;
  uint8 		       dl_holb_enabled;
  uint8 		       hw_rep_holb_enabled;
  /*! Boolean to indicate if test case need UL producer pipe to be suspended */
  uint8                        is_ul_prod_pipe_suspend_enabled;
} ipa_test_engine_params_s;

/*==============================================================================
                   Internal database of the Engine
==============================================================================*/
/*! List of return codes returned by API's within the Engine */
typedef enum
{
  IPA_TEST_RET_CODE_SUCCESS,
  IPA_TEST_RET_CODE_FAILURE
} ipa_test_ret_code_e;

/*! @brief Some forward declarations to circumvent compilation issues because of 
cross reference of structs */
struct ipa_test_engine_log_ch_struct;
struct ipa_test_engine_phy_ch_struct;
struct ipa_test_engine_per_sim_struct;
struct ipa_test_engine_bearer_struct;

typedef struct ipa_test_engine_log_ch_struct  ipa_test_engine_log_ch_s;
typedef struct ipa_test_engine_phy_ch_struct  ipa_test_engine_phy_ch_s;
typedef struct ipa_test_engine_per_sim_struct ipa_test_engine_per_sim_s;
typedef struct ipa_test_engine_bearer_struct  ipa_test_engine_bearer_s;
typedef struct ipa_test_engine_dpl_struct     ipa_test_engine_dpl_s;

/*! Each Simulator endp info */
struct ipa_test_engine_per_sim_struct
{
  uint32             sim_hdl;
  dsm_watermark_type to_ipa_wm;
  q_type             to_ipa_wm_q;
  dsm_watermark_type from_ipa_wm;
  q_type             from_ipa_wm_q;
  dsm_watermark_type mirror_from_ipa_wm;
  q_type             mirror_from_ipa_wm_q;
  dsm_watermark_type mirror_from_ipa_wm_ul;
  q_type             mirror_from_ipa_wm_ul_q;
  dsm_watermark_type mirror_from_ipa_wm_dl;
  q_type             mirror_from_ipa_wm_dl_q;
  dsm_watermark_type deagg_from_ipa_wm;
  q_type             deagg_from_ipa_wm_q;
  boolean            b2b_mode;
};

/*! @brief Each Physical SIO Port info */
struct ipa_test_engine_phy_ch_struct
{
  uint8                     idx;
  sio_stream_id_type        stream_id;
  ipa_test_engine_per_sim_s per_sim;
  dsm_watermark_type        phy_rx_wm;
  q_type                    phy_rx_wm_q;
  dsm_watermark_type        phy_tx_wm;
  q_type                    phy_tx_wm_q;
  dsm_watermark_type        mirror_phy_rx_wm;
  q_type                    mirror_phy_rx_wm_q;
  ipa_sio_per_ep_map_s      per_ep_map;
  uint8                     log_ch_cnt;
  uint8                     curr_log_ch_cnt;
  uint8                     ic_type;
  uint8                     mux_id_arr[IPA_TEST_ENGINE_PER_SIM_LOG_CH_NUM_MAX];
  ipa_test_engine_log_ch_s  *log_ch_ptr_arr \
    [IPA_TEST_ENGINE_PER_SIM_LOG_CH_NUM_MAX];
};

/*! @brief Each Logical Port info */
struct ipa_test_engine_log_ch_struct 
{
  uint8                     idx;
  sio_stream_id_type        stream_id;
  uint8                     mux_id; 
  uint8                     mux_override;
  dsm_watermark_type        tx_wm;
  q_type                    tx_wm_q;
  dsm_watermark_type        rx_wm;
  q_type                    rx_wm_q;
  dsm_watermark_type        mirror_rx_wm;
  q_type                    mirror_rx_wm_q;
  uint32                    bearer_cnt;
  ipa_test_engine_bearer_s  *bearer_ptr_arr[IPA_TEST_ENGINE_BEARER_NUM_MAX];
  ipa_test_engine_phy_ch_s  *phy_ch_ptr;
  ipa_sio_endpoint_config_s *endp_cfg_ptr;
  ipa_test_engine_clat_cntxt_info_type
                                    *clat_cntxt_ptr;
  ipa_clat_handle_t                  clat_hdl;
  boolean                            clat_sio_assocd;
  uint8                              is_clat;
  boolean                            clat_enabled;
  ipa_sio_config_action_e   action;
};

/*! @brief Each Bearer Info */
struct ipa_test_engine_bearer_struct
{
  uint8                    idx;
  uint8                    bearer_id;
  uint8                    subs_id;
  uint8                    uid;
  uint8                    is_bridged;
  uint16                   dpl_bytes_to_log;
  ipa_wan_bearer_tech_e    bearer_tech;
  dsm_watermark_type       pdcp_wm;
  q_type                   pdcp_wm_q;
  dsm_watermark_type       dl_wm;
  q_type                   dl_wm_q;
  dsm_watermark_type       mirror_dl_wm;
  q_type                   mirror_dl_wm_q;
  dsm_watermark_type       ul_wm;
  q_type                   ul_wm_q;
  ipa_test_engine_log_ch_s *log_ch_ptr;
};

/*! @brief DPL watermark Info*/
struct ipa_test_engine_dpl_struct
{ 
  dsm_watermark_type       dpl_wm;
  q_type                   dpl_wm_q;
  dsm_watermark_type       mirror_dpl_wm;
  q_type                   mirror_dpl_wm_q;
  uint32                   num_pkts_rcvd;
};

/*! @brief Structure storing the Cipher params for one instance
*/
typedef struct
{
  uint8                 key_index;
  uint8                 bearer_id;
  uint8                 subs_id;
  uint8                 direction;
  ipa_wan_cipher_algo_e ciph_algo;
  uint32                count_c;
  uint8                 ciph_key[IPA_WAN_CIPH_KEY_SIZE];
  dsm_item_type         *keystream_dsm_ptr;
  dsm_item_type         *dummy_data_dsm_ptr;
  uint32                num_pkts_processed;
} ipa_test_engine_ciph_params_s;

/*! @brief Store the IP packet ptr and size */
typedef struct
{
  /* Whether the DF bit is set for the packet or not*/
  boolean       df_bit;
  boolean       is_ipv6_extn_hdr_present;
  boolean       is_qmap_v3;
  uint16        checksum_val;
  /* Is it a prioritized packet*/
  boolean       is_prioritized;
  ipa_ip_type_e ip_pkt_version;
  uint8         *pkt_buf_ptr;
  uint8         mux_id;
  uint16        pkt_len;
  /*! Length of the IP header Default: 20 for V4. 40 for V6 */
  uint16        ip_hdr_len;
  uint16        tot_hdr_len;
  uint16        extn_hdr_len;
  uint16        payload_len;
  uint32        filter_result;
  uint32        checksum;
  dsm_item_type *dsm_ptr;
} ipa_test_engine_ip_pkt_info_s;

/*! @brief Structure to store the ongoing test Packet comparison result */
typedef struct
{
  uint8         sent_pkt_buf[IPA_TEST_ENGINE_IP_PKT_SIZE_MAX + 20];
  uint8         rcvd_pkt_buf[IPA_TEST_ENGINE_IP_PKT_SIZE_MAX + 20];
  /*! Below is only for deaggr err scenarios we need bigger buffers to hold aggr frame */
  uint8         sent_aggr_pkt_buf[IPA_TEST_ENGINE_TOTAL_AGGR_FRAME_SIZE_MAX];
  uint8         rcvd_aggr_pkt_buf[IPA_TEST_ENGINE_TOTAL_AGGR_FRAME_SIZE_MAX];
  uint16        sent_pkt_len;   
  uint16        rcvd_pkt_len;   
  dsm_item_type *sent_pkt_dsm_ptr; 
  dsm_item_type *rcvd_pkt_dsm_ptr; 
  uint16        no_match_locn;
  uint32        compare_failure_id;
} ipa_test_engine_pkt_cmp_s; 

/*==============================================================================

                                FUNCTIONS

==============================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_engine_set_num_pkts_rx_to_process

==============================================================================*/
/*!
  @brief Function used to handle multiple enqueu.

  @return 
  None
*/
/*============================================================================*/
void ipa_test_engine_set_num_pkts_rx_to_process
(
  uint16 num_pkts_rx_to_process
);

/*==============================================================================
                           Non Empty Watermark Callbacks
==============================================================================*/

/*==============================================================================

  FUNCTION:  ipa_test_engine_sio_rx_wm_each_enqueue_cb

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_sio_rx_wm_each_enqueue_cb
(
  struct dsm_watermark_type_s *wm_ptr,
  void                        *cb_data
);

/*==============================================================================

  FUNCTION:  ipa_test_engine_phy_rx_wm_each_enqueue_cb

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_phy_rx_wm_each_enqueue_cb
(
  struct dsm_watermark_type_s *wm_ptr,
  void                        *cb_data
);

/*==============================================================================

  FUNCTION:  ipa_test_engine_sio_ds_rx_wm_ne_cb

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_sio_ds_rx_wm_ne_cb
(
  struct dsm_watermark_type_s *wm_ptr,
  void                        *cb_data
);

/*==============================================================================

  FUNCTION:  ipa_test_engine_dpl_rx_wm_ne_cb

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_dpl_rx_wm_ne_cb
(
  struct dsm_watermark_type_s *wm_ptr,
  void                        *cb_data
);

/*==============================================================================

  FUNCTION:  ipa_test_engine_sim_rx_wm_ne_cb

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_sim_rx_wm_ne_cb
(
  struct dsm_watermark_type_s *wm_ptr,
  void                        *cb_data
);

/*==============================================================================

  FUNCTION:  ipa_test_engine_init

==============================================================================*/
/*!
  @brief
  Initialize the Test Engine 

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_init(void);

/*==============================================================================

  FUNCTION:  ipa_test_engine_configure

==============================================================================*/
/*!
  @brief
  Does all the configuration before we start with the test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_configure
(
  ipa_test_engine_params_s *params_ptr
);

/*==============================================================================

  FUNCTION:  ipa_test_engine_run

==============================================================================*/
/*!
  @brief
  Runs the test after the cfg is done 

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_run(void);

/*==============================================================================

  FUNCTION:  ipa_test_engine_get_sio_streamid

==============================================================================*/
/*!
  @brief
  Gets the SIO stream id for the particular bearer.
  
  @returns
  SIO stream id for the particular bearer
*/
/*============================================================================*/
sio_stream_id_type ipa_test_engine_get_sio_stream_id
(
  uint8 uid
);

/*==============================================================================

  FUNCTION:  ipa_test_engine_destroy

==============================================================================*/
/*!
  @brief
  Destroy the cfg after the end of the test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_destroy(void);

/*==============================================================================

  FUNCTION:  ipa_test_engine_cfg_generate

==============================================================================*/
/*!
  @brief
  Generates a random cfg for a test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_cfg_generate
(
  ipa_test_engine_params_s *params_ptr
);
/*==============================================================================

  FUNCTION:  ipa_test_get_if_frag_case

==============================================================================*/
/*!
  @brief
  Whether it is a frag test

  @returns
  None
*/
/*============================================================================*/
boolean ipa_test_engine_get_if_frag_case();

/*==============================================================================

  FUNCTION:  ipa_test_engine_generate_clat_cntxt_arr

==============================================================================*/
/*!
  @brief
  Generates random CLAT context.

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_generate_clat_cntxt_arr
(
  ipa_clat_global_cntxt_info_type       *global_cntxt_info_ptr,
  ipa_test_engine_clat_cntxt_info_type  *cntxt_info_arr_ptr
);
/*==============================================================================

                         MACROS

==============================================================================*/
/*! Interval for which to detect any stalls */
#define IPA_TEST_ENGINE_WATCHDOG_STALL_DETECT_INTERVAL      5000

/*! Interval for which to detect any stalls */
#define IPA_TEST_ENGINE_WATCHDOG_STALL_DETECT_INTERVAL_SIO  10000

/*! Interval for which to detect any stalls */
#define IPA_TEST_ENGINE_DS_EVNT_DETECT_INTERVAL             50000

/*! Interval for which to detect any stalls */
#define IPA_TEST_ENGINE_FLOW_CTL_DETECT_INTERVAL            50

/*! Interval to wait to check on aggr force close stats */
#define IPA_TEST_ENGINE_AGGR_FORCE_CLOSE_TEST_INTERVAL      30

/*! Interval for which to detect any stalls */
#define IPA_TEST_ENGINE_ZIP_RESP_INTERVAL                  10000

/*! Number of prev tests that we'll store */
#define IPA_TEST_ENGINE_TEST_TRACE_LEN       25

/*! Callback bitmask related info */
#define IPA_TEST_ENGINE_UL_BITMASK_POS      0
#define IPA_TEST_ENGINE_DPL_BITMASK_POS     24
#define IPA_TEST_ENGINE_DL_BITMASK_POS      26
#define IPA_TEST_ENGINE_SIM_BITMASK_POS     48
#define IPA_TEST_ENGINE_UL_PHY_BITMASK_POS  56
#define IPA_TEST_ENGINE_ALL_BITMASK_POS     64

#define IPA_TEST_ENGINE_UL_BITMASK  \
 (uint64)(((1 << IPA_TEST_ENGINE_DPL_BITMASK_POS) - 1) & \
   (~((1 << IPA_TEST_ENGINE_UL_BITMASK_POS) - 1)))    
#define IPA_TEST_ENGINE_DPL_BITMASK \
  (uint64)(((1 << IPA_TEST_ENGINE_DL_BITMASK_POS) - 1) & \
    (~((1 << IPA_TEST_ENGINE_DPL_BITMASK_POS) - 1)))    
#define IPA_TEST_ENGINE_DL_BITMASK \
  (uint64)(((1 << IPA_TEST_ENGINE_SIM_BITMASK_POS) - 1) & \
     (~((1 << IPA_TEST_ENGINE_DL_BITMASK_POS) - 1)))    
#define IPA_TEST_ENGINE_SIM_BITMASK \
  (uint64)(((1 << IPA_TEST_ENGINE_ALL_BITMASK_POS) - 1) & \
     (~((1 << IPA_TEST_ENGINE_SIM_BITMASK_POS) - 1)))    

/*! Power collapse sleep interval */
#define IPA_TEST_ENGINE_PC_SLEEP_INTERVAL   4000

#define UINT8_MINUS_1                       0xFF

extern ipa_dl_info_s ipa_dl_info;
extern ipa_cfg_s ipa_cfg;
extern ipa_sio_s ipa_sio;
/*==============================================================================

                   INTERNAL DEFINITIONS AND TYPES

==============================================================================*/
typedef enum
{
  IPA_TEST_ENGINE_STATE_NONE,
  IPA_TEST_ENGINE_STATE_ENDP_CFG,
  IPA_TEST_ENGINE_STATE_WAN_CFG,
  IPA_TEST_ENGINE_STATE_CLAT_CFG,
  IPA_TEST_ENGINE_STATE_CIPH_CFG,
  IPA_TEST_ENGINE_STATE_STATIC_FLTR_CFG,
  IPA_TEST_ENGINE_STATE_FLTR_CFG,
  IPA_TEST_ENGINE_STATE_PKT_GEN,
  IPA_TEST_ENGINE_STATE_FLTR_PROC,
  IPA_TEST_ENGINE_STATE_DSM_CHAIN_PROC,
  IPA_TEST_ENGINE_STATE_DL_PKT_PROC,
  IPA_TEST_ENGINE_STATE_UL_PKT_PROC,
  IPA_TEST_ENGINE_STATE_PKT_CMP,
  IPA_TEST_ENGINE_STATE_DL_TX,
  IPA_TEST_ENGINE_STATE_UL_TX,
  IPA_TEST_ENGINE_STATE_SIM_RX,
  IPA_TEST_ENGINE_STATE_DL_RX,
  IPA_TEST_ENGINE_STATE_UL_RX,
  IPA_TEST_ENGINE_STATE_UL_PHY_RX,
  IPA_TEST_ENGINE_STATE_UL_LOOPBACK,
  IPA_TEST_ENGINE_STATE_DL_LOOPBACK,
  IPA_TEST_ENGINE_STATE_CIPH_GEN,
  IPA_TEST_ENGINE_STATE_UL_AGGR,
  IPA_TEST_ENGINE_STATE_DL_DEAGGR,
  IPA_TEST_ENGINE_STATE_RX_WAIT,
  IPA_TEST_ENGINE_STATE_ENDP_DESTROY,
  IPA_TEST_ENGINE_STATE_WAN_DESTROY,
  IPA_TEST_ENGINE_STATE_CIPH_DESTROY,
  IPA_TEST_ENGINE_STATE_FLTR_DESTROY,
  IPA_TEST_ENGINE_STATE_ZIP_REQUEST,
  IPA_TEST_ENGINE_STATE_ZIP_RESPONSE,
  IPA_TEST_ENGINE_STATE_ZIP_DESTROY,
  IPA_TEST_ENGINE_STATE_WM_CB,
  IPA_TEST_ENGINE_STATE_DONE
} ipa_test_engine_state_e;

typedef enum
{
  IPA_TEST_ENGINE_CLAT_CB_ENABLED = 1,
  IPA_TEST_ENGINE_CLAT_CB_CFG_DONE = 2,
  IPA_TEST_ENGINE_CLAT_CB_ASSOC_SIO_DONE = 3,
  IPA_TEST_ENGINE_CLAT_CB_DEREG_DONE = 4,
} ipa_test_engine_clat_cb_wait_type_e;

/*! Struct to track all the SIO mapping callbacks */
typedef struct
{
  uint8 num_pend_sio_cb;
  uint8 pend_stream_id_arr[SIO_MAX_STREAM];
} ipa_test_engine_sio_map_cb_s;

/*! Struct to track all the CLAT config callbacks */
typedef struct
{
  ipa_test_engine_clat_cb_wait_type_e
     clat_cfg_arr[IPA_CLAT_MAX_INSTANCES];
  uint8 num_pend_clat_cfg_cb;
  uint8 num_pend_assoc_sio_cb;
  uint8 num_pend_dereg_cb;
} ipa_test_engine_clat_cfg_cb_s;

/*! Top Level Endpoint Database */
typedef struct
{
  ipa_test_engine_phy_ch_s      phy_ch[IPA_TEST_ENGINE_PHY_CH_NUM_MAX];
  uint8                         curr_phy_ch_cnt; 
  ipa_test_engine_log_ch_s      log_ch[IPA_TEST_ENGINE_LOG_CH_NUM_MAX]; 
  uint8                         curr_log_ch_cnt; 
  uint8                         curr_b2b_cnt; 
  /*! Structure to hold the map of pending transactions */
  ipa_test_engine_sio_map_cb_s  sio_map_cb;
} ipa_test_engine_endp_s;

typedef struct
{
  /*! Structure to hold the map of pending CLAT configurations */
  ipa_test_engine_clat_cfg_cb_s      clat_cfg_cb;
} ipa_test_engine_clat_s;

/*! Top Level WAN Database */
typedef struct
{
  uint8                     uid_arr[IPA_TEST_ENGINE_BEARER_NUM_MAX];
  uint8                     curr_bearer_cnt; 
  ipa_test_engine_bearer_s  bearer[IPA_TEST_ENGINE_BEARER_NUM_MAX];
} ipa_test_engine_wan_s;

/*! This struct holds the allocation of packets for the various bearer/log channels */
typedef struct
{
  ipa_test_engine_ip_pkt_info_s *pkt_arr[IPA_TEST_ENGINE_IP_PKT_NUM_MAX];
  uint16                        num_pkts;    
} ipa_test_engine_ip_pkt_dl_alloc_bucket_s;

/*! This struct holds the allocation of packets for the various phys channels */
typedef struct
{
  ipa_test_engine_ip_pkt_info_s *pkt_arr[IPA_TEST_ENGINE_IP_PKT_NUM_MAX];
  ipa_test_engine_log_ch_s      *log_ch_ptr[IPA_TEST_ENGINE_IP_PKT_NUM_MAX];
  uint16                        num_pkts;    
} ipa_test_engine_ip_pkt_ul_alloc_bucket_s;

/*! @brief IP Packet Cache of the engine. This is a cache used by the engine for 
memory allocations for pkts used in the test. It is used for different things 
at different points of the test */
typedef struct
{
  /*! Array of packets for this test. The buffers will be dynamically allocated */
  ipa_test_engine_ip_pkt_info_s             pkt_info_arr[IPA_TEST_ENGINE_IP_PKT_NUM_MAX];
  ipa_test_engine_ip_pkt_info_s             pkt_info_arr_replication_dl[IPA_TEST_ENGINE_IP_PKT_NUM_MAX];
  ipa_test_engine_ip_pkt_info_s             pkt_info_arr_replication_ul[IPA_TEST_ENGINE_IP_PKT_NUM_MAX];
  /*! Current number of pkts in the buf */
  uint16                                    curr_num_pkts;
  uint16                                    curr_num_pkts_ul;
  uint16                                    curr_num_pkts_dl;
  uint16 				    pkt_count_for_holb;
  /*! Current number of DSM items in the pkt info array */
  uint16                                    curr_dsm_items;
  /*! UL allocation of pkts to various phy channels */
  ipa_test_engine_ip_pkt_ul_alloc_bucket_s  ul_alloc_buckets[IPA_TEST_ENGINE_PHY_CH_NUM_MAX];
  /*! DL allocation of pkts to various bearer/log channels */
  ipa_test_engine_ip_pkt_dl_alloc_bucket_s  dl_alloc_buckets[IPA_TEST_ENGINE_LOG_CH_NUM_MAX];
} ipa_test_engine_ip_pkt_s;

/*! Filter Data base */
typedef struct
{
  uint32 dummy;
} ipa_test_engine_fltr_s;

/*! @brief Endpoint Cfg Stats */
typedef struct
{
  uint64 num_qmap_tests; 
  uint64 num_qmap_v3_tests; 
  uint64 num_eth_tests;
  uint64 num_none_tests;
  uint64 num_mbim_tests;
} ipa_test_engine_stats_endp_dl_s;

/*! @brief Endpoint Cfg Stats */
typedef struct
{
  uint64 num_qmap_tests; 
  uint64 num_qmap_v3_tests; 
  uint64 num_eth_tests;
  uint64 num_none_tests;
  uint64 num_dma_tests;
  uint64 num_mbim_tests;
} ipa_test_engine_stats_endp_ul_s;

/*! @brief Endpoint Cfg Stats */
typedef struct
{
  ipa_test_engine_stats_endp_dl_s dl;
  ipa_test_engine_stats_endp_ul_s ul;
} ipa_test_engine_stats_endp_s;

/*! @brief WAN Cipher Stats */
typedef struct
{
  uint64 num_ciph_tests; 
  uint64 num_aes_pkts;
  uint64 num_zuc_pkts;
  uint64 num_snow_pkts;
  uint64 num_none_pkts;
} ipa_test_engine_stats_ciph_s;

/*! @brief WAN Cfg Stats */
typedef struct
{
  uint64 num_pdcp_tests;
  uint64 num_sio_tests;
  uint64 num_bridge_tests;
  uint64 num_unbridge_tests;
} ipa_test_engine_stats_wan_s;

/*! @brief Packet Cfg Stats */
typedef struct
{
  uint64 num_v4_tests; 
  uint64 num_v6_tests; 
  uint64 num_frag_tests; 
  uint64 num_dsm_split_tests; 
  uint64 num_pkts_dl_split_avg;
  uint64 num_pkts_dl_split_col;
} ipa_test_engine_stats_pkt_s;

/*! @brief Filter Cfg Stats */
typedef struct
{
  uint64 num_fltr_tests; 
} ipa_test_engine_stats_fltr_s;

/*! @brief Top Level Stats */
typedef struct
{
  uint64                       num_tests_run; 
  uint64                       num_pkts;
  uint64                       num_dl_pkts;
  uint64                       num_ul_pkts;
  uint64                       num_dl_tests_run; 
  uint64                       num_ul_tests_run; 
  uint64                       num_loopback_tests_run; 
  ipa_test_engine_stats_endp_s endp;
  ipa_test_engine_stats_wan_s  wan;
  ipa_test_engine_stats_ciph_s ciph;
  ipa_test_engine_stats_pkt_s  pkt;
  ipa_test_engine_stats_fltr_s fltr;
} ipa_test_engine_stats_s;

typedef enum
{
  IPA_TEST_ENGINE_PKT_SRC_UNKNOWN,
  IPA_TEST_ENGINE_PKT_SRC_BEARER_BRIDGED,
  IPA_TEST_ENGINE_PKT_SRC_BEARER_UNBRIDGED,
  IPA_TEST_ENGINE_PKT_SRC_LOG_CHANNEL,
  IPA_TEST_ENGINE_PKT_SRC_SIM
} ipa_test_engine_pkt_src_e;

typedef enum
{
  IPA_TEST_ENGINE_PKT_DST_UNKNOWN,
  IPA_TEST_ENGINE_PKT_DST_SIM,
  IPA_TEST_ENGINE_PKT_DST_BEARER_DL,
  IPA_TEST_ENGINE_PKT_DST_LOG_CHANNEL,
  IPA_TEST_ENGINE_PKT_DST_PHY_CHANNEL,
  IPA_TEST_ENGINE_PKT_DST_DPL
} ipa_test_engine_pkt_dst_e;

/*! @brief Pkt Trace for the curr test */
typedef struct
{
  /*! Store the ip versio of the original packet.  */
  ipa_ip_type_e             ip_ver;
  /* If an xlat conversion is expected. */
  boolean                   xlat;                    
  uint16                    pkt_len;
  ipa_test_engine_pkt_src_e src;
  uint8                     src_idx;
  ipa_test_engine_pkt_dst_e dst;
  uint8                     dst_idx;
  uint32                    filter_result;
  uint32                    checksum;
  uint8                     filter_priority;
  ipa_wan_cipher_param_u    ciph_params;
  uint32                    ciph_count;  
} ipa_test_engine_pkt_trace_elem_s;

/*! @brief Pkt Trace for the curr test */
typedef struct
{
  ipa_test_engine_pkt_trace_elem_s tx_pkts[2 * IPA_TEST_ENGINE_IP_PKT_NUM_MAX];
  ipa_test_engine_pkt_trace_elem_s rx_pkts[2 * IPA_TEST_ENGINE_IP_PKT_NUM_MAX];
} ipa_test_engine_pkt_trace_s;

typedef struct
{
  ipa_test_engine_state_e     state;
  ipa_timetick_t              timetick;
} ipa_test_engine_state_trace_elem_s;

typedef struct
{
  ipa_test_engine_state_trace_elem_s trace_arr[IPA_TEST_ENGINE_STATE_TRACE_LEN];
  uint32                             trace_idx;
  ipa_crit_sect_t                    crit_sect;
} ipa_test_engine_state_trace_s;

/*! @brief Aggregation/Deaggregation tmp cache */
typedef struct
{
  dsm_item_type         *frame_dsm_ptr;
  /*! Stats for maintaining the deaggregation size histogram for this endpoint */
  ipa_stats_aggr_hist_s aggr_hist[IPA_STATS_AGGR_BUCKET_MAX];
  /*! Stats for maintaining the deaggregation size histogram for this endpoint */
  ipa_stats_aggr_hist_s deaggr_hist[IPA_STATS_AGGR_BUCKET_MAX];
} ipa_test_engine_aggr_s;

/*! @brief Structure to store the ongoing test status */
typedef struct
{
  ipa_test_engine_state_e           state;
  /*! Total number of pkts recvd & sent in the test */
  uint32                            num_pkts_tot;
  uint16                            num_pkts_tx;
  /*! Number of packets rcvd that passed validation*/
  uint16                            num_pkts_rx;
  uint32                            num_pkts_hw_rep;
  /*! Number of UL packets received that are yet to be processed*/
  uint16                            num_pkts_rx_to_process;
  /*! Number of DL packets that are split above > 10 items */
  uint16                            num_pkts_dl_split_avg;
  /*! Number of DL packets that are split above > 40 items to trigger 
  DSM collapsing */
  /*! Number of pkts to trigger UL Flow Control */
  uint16                            num_pkts_to_flow_ctl;
  /*! If flow disabled for current test*/
  boolean                           is_flow_disabled;
  boolean                           pkt_wait;
  uint16                            num_pkts_dl_split_col;
  uint64                            wm_cb_bit_mask;
  ipa_test_engine_pkt_trace_s       pkt_trace;
  uint16                            num_pkts_rx_wdog_prev;
  ipa_test_engine_aggr_s            aggr[IPA_TEST_ENGINE_PER_SIM_NUM_MAX];
  ipa_test_engine_deaggr_err_info_s deaggr_err_info;
  ipa_test_engine_params_s          cfg;
  /*! Keeping track of the write indexe for the status log for
      the current test run.*/
  uint8                             prev_stts_log_wr_idx;
  uint8                             curr_stts_log_wr_idx;
  uint8                             is_holb;
} ipa_test_engine_curr_test_s;

typedef enum
{
  IPA_TEST_ENGINE_CIPH_DIR_UL_DECIPH = 0,
  IPA_TEST_ENGINE_CIPH_DIR_DL_DECIPH = 1
} ipa_test_engine_ciph_dir_e;

/*! Top Level Cipher info storage */
typedef struct
{
  ipa_test_engine_ciph_params_s   ciph_params \
    [IPA_TEST_ENGINE_CIPH_PARAMS_NUM_MAX];
  dsm_item_type                   *temp_keystream_dsm_ptr;
  dsm_item_type                   *temp_data_dsm_ptr;
  uint8                           init_done;
  uint8                           is_ciph_enabled;
} ipa_test_engine_ciph_s; 

/*! @brief Test Engine results */
typedef struct
{
  ipa_test_engine_params_s    cfg;
  ipa_test_engine_pkt_trace_s pkt;
  ipa_test_engine_aggr_s      aggr[IPA_TEST_ENGINE_PER_SIM_NUM_MAX];
} ipa_test_engine_trace_elem_s;

/*! @brief Test Engine results */
typedef struct
{
  ipa_test_engine_trace_elem_s tests[IPA_TEST_ENGINE_TEST_TRACE_LEN];
  uint16                       idx;                       
} ipa_test_engine_trace_s;

/*! @brief Top level test engine input params for a test */
typedef struct
{
  ipa_test_engine_endp_s        endp;
  ipa_test_engine_wan_s         wan;
  ipa_test_engine_clat_s        clat;
  ipa_test_engine_dpl_s         dpl;
  ipa_test_engine_ip_pkt_s      ip_pkt;
  ipa_test_engine_fltr_s        fltr;
  ipa_test_engine_ciph_s        ciph;
  ipa_test_engine_zip_s         zip;
  ipa_test_engine_pkt_cmp_s     pkt_cmp;
  ipa_test_engine_curr_test_s   curr_test;
  ipa_test_engine_state_trace_s state_trace;
  ipa_test_engine_stats_s       stats;
  ipa_test_engine_trace_s       test_trace;
  ipa_wan_subscription_id_t     subs_id_arr[IPA_TEST_ENGINE_SUBS_NUM_MAX];
  ipa_crit_sect_t               crit_sect;
  ipa_signal_s                  wait_signal;
  ipa_signal_s                  endp_wait_signal;
  ipa_signal_s                  bearer_susp_wait_signal;
  ipa_signal_s                  bearer_reg_wait_signal;
  ipa_signal_s                  bearer_dereg_wait_signal;
  ipa_signal_s                  bearer_active_wait_signal;
  ipa_signal_s                  zip_reg_done_signal;
  ipa_signal_s                  zip_dict_load_done_signal;
  ipa_signal_s                  zip_resp_rcvd_signal;
  ipa_signal_s                  zip_err_resp_rcvd_signal;
  ipa_signal_s                  zip_unreg_done_signal;
  ipa_signal_s                  clat_cfg_signal;
  ipa_signal_s                  clat_assoc_sio_signal;
  ipa_signal_s                  clat_translation_signal;
  ipa_signal_s                  clat_dereg_signal;
  ipa_util_timer_s              watchdog_timer;
  ipa_util_timer_s              flow_ctl_timer;
  ipa_util_timer_s              ds_evnt_timer;
  ipa_util_timer_s              zip_resp_timer;
  ipa_util_timer_s              aggr_force_close_timer;
  uint8                         init_done; 
  uint16 			pkt_count_for_holb;
} ipa_test_engine_s;

/*==============================================================================

  FUNCTION:  ipa_test_engine_clat_embed_ipv4_addr_in_ipv6_addr

==============================================================================*/
/*!
  @brief
  embeds IPv4 addr in IPv6 addr for CLAT translation.

  @returns
  None
*/
/*============================================================================*/
void ipa_test_engine_clat_embed_ipv4_addr_in_ipv6_addr
(
  uint8   v6_prefix_len_in_bytes,
  uint8   *dest_v6_buf,
  uint32  v4_addr    
);

/*==============================================================================

  FUNCTION:  ipa_test_engine_generate_ip_pkts

==============================================================================*/
/*!
  @brief
  Generates IP packets for a test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_generate_ip_pkts(void);

/*==============================================================================

  FUNCTION:  ipa_test_engine_dl_tx_pkts

==============================================================================*/
/*!
  @brief
  1. Generate aggregated frame and send it to mirror watermark
  2. Send pkts over a random bearer or SIO port

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_dl_tx_pkts(void);

/*==============================================================================

  FUNCTION:  ipa_test_engine_destroy_ip_pkts

==============================================================================*/
/*!
  @brief
  Generates IP packets for a test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_destroy_ip_pkts(void);


#endif /* IPA_TEST_ENGINE_H */
