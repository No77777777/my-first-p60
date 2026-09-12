/*!
  @file
  lpdcp_qsh_ext.h

  @brief
  This file contains all the internally defined structures/function prototypes
  for PDCP metric, events and debug dump.

  @author
  sbathwal

*/

/*==============================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/lte.mpss/6.6.1/api/lpdcp_qsh_ext.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
01/05/16   ru      CR1107985: Get AS_ID from QSH during metric collect in PDCP
02/04/15   sb      initial version
==============================================================================*/

#ifndef LPDCP_QSH_EXT_H
#define LPDCP_QSH_EXT_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include <qsh.h>
#include <lte_security.h>

/*==============================================================================

                   EXTERNAL CONSTANT/MACRO DEFINITIONS

==============================================================================*/
/*! @brief defined QSH major version */
#define LPDCP_QSH_MAJOR_VER 0x0001

/*! @brief defined QSH minor version */
#define LPDCP_QSH_MINOR_VER 0x0000

/*! @brief Total dump size allocated for this client */
#define LPDCP_QSH_DEBUG_DUMP_SIZE                                (1024)

/*! @brief MAX number of per RB infos logged in the dump */
#define LPDCP_QSH_RB_INFO_MAX                                    (10)
/*! @brief MAX number of per RB stats logged in the dump */
#define LPDCP_QSH_RB_STATS_MAX                                   (4)



/*==============================================================================

                 EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/
/*! @brief 
    Enum for METRIC
 */
typedef enum
{
  LPDCP_QSH_METRIC_UL_STATS_AS_0 = 0,
  LPDCP_QSH_METRIC_DL_STATS_AS_0,
  LPDCP_QSH_METRIC_UL_STATS_AS_1,
  LPDCP_QSH_METRIC_DL_STATS_AS_1,
  LPDCP_QSH_METRIC_MAX
}lpdcp_qsh_metric_e;

/*! @brief 
    Enum for EVENT
 */
typedef enum
{
  LPDCP_QSH_EVENT_UL_STATS = 0,
  LPDCP_QSH_EVENT_DL_STATS,
  LPDCP_QSH_EVENT_MAX
}lpdcp_qsh_event_e;

/*! @brief 
    Enum for DUMP TAG
 */
typedef enum
{
  LPDCP_QSH_DUMP_TAG_DL_RB_INFO = 0,
  LPDCP_QSH_DUMP_TAG_DL_RB_STATS,
  LPDCP_QSH_DUMP_TAG_DL_GLOBAL_STATS,
  LPDCP_QSH_DUMP_TAG_UL_RB_INFO,
  LPDCP_QSH_DUMP_TAG_UL_RB_STATS,
  LPDCP_QSH_DUMP_TAG_UL_GLOBAL_STATS
}lpdcp_qsh_dump_tag_e;


/*==============================================================================

                   PDCP UL STRUCTURE DEFINITIONS

==============================================================================*/

/*! @brief 
    Structure containing PDCP UL bearer config and state. 
    This structure is maintained per RB.   
 */
typedef PACK(struct)
{
  uint32 err_mask;
  uint8  rb_cfg_idx; /*!< RB cfg index (1-32:DRB, 33-34:SRB) */
  uint8  eps_id;     /*!< EPS ID */
  uint8  seq_len;    /*!< PDCP seq len in bits (5, 7, 12 bits) */
  uint8  discard_timer; /*!< PDCP discard timer in 10 msec,
                                LTE_PDCP_DISCARD_TIMER_NOT_CONFIG is used
                                when discard timer is not configured or
                                discard timer is infinity */
  uint8  rohc_max_cid; /*!< ROHC max context ID, 0xFF means ROHC disabled */
  uint8  flow;    /*!< PDCP flow mask(0/1/2/3: Enable/Disable/Suspend/Disable_Suspend) */
  uint16  tx_seq;  /*!< PDCP TX seq */
  uint32  tx_hfn;  /*!< PDCP TX HFN */
  uint32  rohc_mask;    /*!< ROHC mask */
}lpdcp_qsh_ul_rb_info_s;


/*! @brief 
    Structure containing PDCP UL bearer statistics. 
    This structure is maintained per RB.   
 */
typedef PACK(struct)
{
  uint8  rb_cfg_idx; /*!< RB cfg index (1-32:DRB, 33-34:SRB) */
  uint8  num_rst;     /*!< number of reestablishments since activated */
  uint16  num_rel_rlf_hfn_desync;  /*!< Num of Conn release or RLF triggered because of HFN desync */
  uint32  num_data_pdu;        /*!< number of PDCP pdu transmitted */
  uint64  data_pdu_bytes;      /*!< PDCP PDU in bytes transmitted */
  uint32  num_discard_sdu;   /*!< number of time based discard SDUs */
  uint32  num_pdcpul_wm_pkt_cnt;    /*!< curr number of packets in UL watermark*/
  uint32  pdcpul_wm_bytes;    /*!< curr  bytes in UL watermark*/
  uint32  num_retransmit_sdu;  /*!< number of retransmit SDUs */
  uint32  num_rohc_feedback;  /*!< number of ROHC feedback transmitted */
  uint32  num_rohc_fail;      /*!< nmuber of ROHC compression failed */
  uint32  num_piggyback_rohc_feedback_rxed;  /*!< number of piggyback ROHC 
                                                  feedback rxed */
}lpdcp_qsh_ul_rb_stats_s;


/*! @brief 
    Structure containing PDCP UL warnings. Each of these statistics are calculated 
    across all bearers. 
 */
typedef PACK(struct)
{
  uint32 num_discard_sdus;/*!< Total number of discarded PDCP SDUs */
}lpdcp_qsh_ul_warnings_s;


/*! @brief 
    Structure containing PDCP security config. 
    This structure is maintained per RB.   
 */
typedef PACK(struct)
{
  /*!< SRB Cipher Algo (NONE/SNOW3G/AES) */
  lte_cipher_algo_e srb_cipher_algo;
  uint8        srb_cipher_key_index; /*!< SRB Cipher Key index */
  uint8       srb_cipher_key[16]; /*!< SRB Cipher Key */

  /*!< SRB Integrity Algo (NONE/SNOW3G/AES)*/
  lte_integrity_algo_e srb_integrity_algo;
  uint8        srb_integrity_key_index; /*!< SRB Integrity Key index */
  uint8        srb_integrity_key[16]; /*!< SRB Integrity Key */

  /*!< DRB Cipher Algo (NONE/SNOW3G/AES) */
  lte_cipher_algo_e drb_cipher_algo;
  uint8        drb_cipher_key_index; /*!< DRB Cipher Key index */
  uint8        drb_cipher_key[16]; /*!< DRB Cipher Key */
} lpdcp_qsh_security_cfg_s;


/*! @brief 
    Structure Tag containing global PDCP UL statistics. Each of these statistics are 
    accumulated across all bearers.
 */
typedef PACK(struct)
{
  qsh_dump_tag_hdr_s      hdr;           /*!< this header will have a tag and length */
  boolean ftm_mode;                 /*!< boolean to indicate if UE is in FTM mode */
  boolean lte_not_active;               /*!< boolean to indicate if LTE is deactivatd */
  lpdcp_qsh_security_cfg_s security_cfg; /*!< security cfg */  
} lpdcp_qsh_dump_tag_ul_global_stats_s;


/*! @brief 
    Structure Tag containing PDCP UL bearer config and state of all the active bearers.
 */
typedef PACK(struct)
{
  qsh_dump_tag_hdr_s      hdr;           /*!< this header will have a tag and length */
  uint8                   num_active_rbs;/*!< num of active RBs */
  lpdcp_qsh_ul_rb_info_s  ul_rb_info[LPDCP_QSH_RB_INFO_MAX];
}lpdcp_qsh_dump_tag_ul_rb_info_s;


/*! @brief 
    Structure Tag containing PDCP UL bearer statistics  of all the active bearers. 
 */
typedef PACK(struct)
{
  qsh_dump_tag_hdr_s       hdr;           /*!< this header will have a tag and length */
  uint8                    num_active_rbs;
  lpdcp_qsh_ul_rb_stats_s  ul_rb_stats[LPDCP_QSH_RB_STATS_MAX];
}lpdcp_qsh_dump_tag_ul_rb_stats_s;


/*! @brief 
    Structure containing PDCP UL metrics. Each of these statistics are calculated 
    across all bearers. Each member of the structure holds value for a sample 
    period of 1s.
 */
typedef struct
{
  qsh_metric_hdr_s hdr;          /*!< This header has the timestamp (uint16) */
  lpdcp_qsh_ul_warnings_s pdcp_ul_warnings;/*!< PDCP UL Warnings */
}lpdcp_qsh_metric_ul_stats_s;


/*==============================================================================

                   PDCP DL STRUCTURE DEFINITIONS

==============================================================================*/


/*! @brief 
    Structure containing PDCP DL bearer config and state. 
    This structure is maintained per RB.   
 */
typedef PACK(struct)
{
  uint32        err_mask;          /*!< mask to determine the veridct: */
  uint8         rb_cfg_idx; /*!< RB cfg index (1-32:DRB, 33-34:SRB) */
  boolean       status_report; /*!< PDCP status report enabled or not */
  uint8         rohc_max_cid; /*!< ROHC max context ID, 0xFF means ROHC is disabled */
  uint8         state;   /*!< PDCP state(0:IDLE, 1:ACTIVE, 2:SECURITY_PENDING) */
  uint32        rohc_mask;    /*!< ROHC mask */
  uint32        rx_hfn;  /*!< PDCP RX HFN */
  uint16        rx_seq;  /*!< PDCP RX seq */
  uint16        last_submitted_rx_seq; /*!< PDCP Last Submitted RX seq */
  uint8         hfn_state;      /*!< HFN state after IPA callback trigger on HFN desync */
  uint8         reserved1;
  uint16        reserved2;
} lpdcp_qsh_dl_rb_info_s;


/*! @brief 
    Structure containing PDCP DL bearer statistics. 
    This structure is maintained per RB.   
 */
typedef PACK(struct)
{
  uint8   rb_cfg_idx; /*!< RB cfg index (1-32:DRB, 33-34:SRB) */
  uint8  num_integrity_fail;  /*!< number of integrity check failed */
  uint16  num_cb_trigger; /*!< Absolute number of callback trigger from IPA on HFN desync */
  uint16  num_rel_rlf_hfn_desync;  /*!< Num of Conn release or RLF triggered because of HFN desync */
  uint16  num_probable_cb_trigger; /*!< Probable number of callback trigger from IPA that resulted in action by PDCP */
  uint32  num_data_pdu;         /*!< number of PDCP data pdu received */
  uint64  data_pdu_bytes;       /*!< PDCP data PDU in bytes received */
  uint32  num_miss_pdu_to_upper_layer; /*!< gap to upper layer */
  uint32  num_miss_pdu_from_low_layer; /*!< gap from low layer during reestablishment*/
  uint32  num_dup_or_out_of_win_pdu;   /*!< number of discard PDUs */
  uint32  num_invalid_pdu;   /*!< number of invalid PDUs */
  uint32  pdcpdl_wm_bytes;    /*!< curr number of bytes in PDCP DL watermark*/
  uint32  num_pdcpdl_wm_drop; /*!< number of pkt drop in PDCP DL watermark because of DNE */
  uint32  pdcpdl_wm_bytes_drop; /*!< PDCP SDU bytes drop in PDCP DL watermark because of DNE */
  uint32  num_data_pdus_reorder_q;  /*!< number of PDUs in PDCP reordering queue */
  uint32  num_empty_wm_sdu;/*!< number of sdus dropped because of empty 
                                    watermark pointer*/
  uint32  num_rohc_feedback;  /*!< number of ROHC feedback received */
  uint32  num_rohc_fail;      /*!< nmuber of ROHC decompression failed */
 }lpdcp_qsh_dl_rb_stats_s;

/*! @brief 
    Structure containing PDCP DL warnining. Each of these statistics are calculated 
    across all bearers. 
 */
typedef PACK(struct)
{
  uint32  num_rrc_sdu_fail; /*!< num of times LTE_PDCPDL_SDU_IND msg sending fails*/
  uint32  pdcp_offload_q_full_cnt;/*!<number of times the PDCP->OFFLOAD Q is full */
  uint32  num_pkt_drop_pdcp_offload_q_full; /*!< num of pkt drops because of PDCP-OFFLOAD q full */
}lpdcp_qsh_dl_warnings_s;

/*! @brief 
    Structure containing PDCP DL PDUs drops. Each of these statistics are calculated 
    across all bearers. 
 */
typedef struct
{
  uint32  num_miss_pdu_to_upper_layer; /*!< gap to upper layer */
  uint32  num_dup_or_out_of_win_pdu;   /*!< number of discard PDUs */
  uint32  num_invalid_pdu;   /*!< number of invalid PDUs */
}lpdcp_qsh_dl_pdu_drops_s;


/*! @brief 
    Structure Tag containing global PDCP DL statistics. Each of these statistics are 
    accumulated across all bearers.
 */
typedef PACK(struct)
{
  qsh_dump_tag_hdr_s      hdr;           /*!< this header will have a tag and length */
  boolean  lte_not_active;            /*!< boolean to indicate if LTE is deactivatd */
  boolean  security_valid;        /*!< security configured */
  uint16   reserved;
  lpdcp_qsh_dl_warnings_s pdcp_dl_warnings;/*!< PDCP DL Warnings */
} lpdcp_qsh_dump_tag_dl_global_stats_s;


/*! @brief 
    Structure containing PDCP DL bearer config and state of all the active bearers.
 */
typedef PACK(struct)
{
  qsh_dump_tag_hdr_s      hdr;           /*!< this header will have a tag and length */
  uint8                   num_active_rbs;/*!< num of active RBs */
  lpdcp_qsh_dl_rb_info_s  dl_rb_info[LPDCP_QSH_RB_INFO_MAX];
}lpdcp_qsh_dump_tag_dl_rb_info_s;


/*! @brief 
    Structure containing PDCP DL bearer statistics of all the active bearers. 
 */
typedef PACK(struct)
{
  qsh_dump_tag_hdr_s       hdr;           /*!< this header will have a tag and length */
  uint8                    num_active_rbs;/*!< num of active RBs */
  lpdcp_qsh_dl_rb_stats_s  dl_rb_stats[LPDCP_QSH_RB_STATS_MAX];
}lpdcp_qsh_dump_tag_dl_rb_stats_s;


/*! @brief 
    Structure containing PDCP DL metrics. Each of these statistics are calculated 
    across all bearers. Each member of the structure holds value for a sample 
    period of 1s.
 */
typedef struct
{
  qsh_metric_hdr_s hdr;          /*!< This header has the timestamp (uint16) */
  lpdcp_qsh_dl_pdu_drops_s pdcp_dl_drops;/*!< PDCP DL Drop PDUs count */
  lpdcp_qsh_dl_warnings_s pdcp_dl_warnings;/*!< PDCP DL Warnings */
}lpdcp_qsh_metric_dl_stats_s;

/* Define a structure with QSH dump header and all TLV structures */
typedef struct
{
  qsh_dump_hdr_s hdr;
  lpdcp_qsh_dump_tag_dl_rb_info_s dl_rb_info;
  lpdcp_qsh_dump_tag_dl_rb_stats_s dl_rb_stats;
  lpdcp_qsh_dump_tag_dl_global_stats_s dl_global_stats;
  lpdcp_qsh_dump_tag_ul_rb_info_s ul_rb_info;
  lpdcp_qsh_dump_tag_ul_rb_stats_s ul_rb_stats;
  lpdcp_qsh_dump_tag_ul_global_stats_s ul_global_stats;
}lpdcp_qsh_dump_collect_s;

/*==============================================================================
 *

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

#endif /* LPDCP_QSH_EXT_H */
