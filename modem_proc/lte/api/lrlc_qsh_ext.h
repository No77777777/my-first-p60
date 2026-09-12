/*!
  @file
  lte_rlc_qsh_ext.h

  @brief
  This file contains all the internally defined structures/function prototypes
  for RLC metric, events and debug dump.

  @author
  rupretee

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

#ifndef LRLC_QSH_EXT_H
#define LRLC_QSH_EXT_H


/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include <qsh.h>


/*==============================================================================

                   EXTERNAL CONSTANT/MACRO DEFINITIONS

==============================================================================*/
/*! @brief defined QSH major version */
#define LRLC_QSH_MAJOR_VER 0x0001

/*! @brief defined QSH minor version */
#define LRLC_QSH_MINOR_VER 0x0000

/*! @brief Total dump size allocated for this client */
#define LRLC_QSH_DEBUG_DUMP_SIZE                                (1024)

/*! @brief Internal mask which is set upon detecting certain error conditions */

/*! @brief Indicates T_POLL timer is set on the higher side */
#define LRLC_QSH_UL_CFG_HIGH_T_POLL                             (1 << 0) 
/*! @brief Indicates high UL BLER */ 
#define LRLC_QSH_UL_CFG_PHY_BLER                                (1 << 1)
/*! @brief Indicates high amount of memory held in UL due to BLER */
#define LRLC_QSH_UL_CFG_HIGH_MEM_USAGE                          (1 << 2)

/*! @brief Indicates T_REORDERING timer is set on the higher side */
#define LRLC_QSH_DL_CFG_HIGH_T_REORDERING                       (1 << 0)  
/*! @brief Indicates T_STATUS_PROHIBIT timer is set on the higher side */   
#define LRLC_QSH_DL_CFG_HIGH_T_STATUS_PROHIBIT                  (1 << 1)  
/*! @brief Indicates high DL BLER */      
#define LRLC_QSH_DL_CFG_PHY_BLER                                (1 << 2)
/*! @brief Indicates DL PDUs were dropped */
#define LRLC_QSH_DL_CFG_DROPPED_PDUS                            (1 << 3)
/*! @brief Indicates high amount of memory held in DL due to BLER */
#define LRLC_QSH_DL_CFG_HIGH_MEM_USAGE                          (1 << 4)
/*! @brief Indicates DL PDUs were forced flushed in UM */
#define LRLC_QSH_DL_CFG_MISSED_UM_PDUS                          (1 << 5)

/*! @brief MAX number of per RB infos logged in the dump */
#define LRLC_QSH_RB_INFO_MAX                                    (10)
/*! @brief MAX number of per RB stats logged in the dump */
#define LRLC_QSH_RB_STATS_MAX                                   (4)

/*==============================================================================

                 EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/
/*! @brief 
    Enum for METRIC
 */
typedef enum
{
  LRLC_QSH_METRIC_UL_STATS_0 = 0,
  LRLC_QSH_METRIC_DL_STATS_0,
  LRLC_QSH_METRIC_UL_STATS_1,
  LRLC_QSH_METRIC_DL_STATS_1,
  LRLC_QSH_METRIC_MAX
}lrlc_qsh_metric_e;

/*! @brief 
    Enum for DUMP TAG
 */
typedef enum
{
  LRLC_QSH_DUMP_TAG_DL_RB_INFO = 0,
  LRLC_QSH_DUMP_TAG_DL_RB_STATS,
  LRLC_QSH_DUMP_TAG_DL_GLOBAL_STATS,
  LRLC_QSH_DUMP_TAG_UL_RB_INFO,
  LRLC_QSH_DUMP_TAG_UL_RB_STATS,
  LRLC_QSH_DUMP_TAG_UL_GLOBAL_STATS,
  LRLC_QSH_DUMP_TAG_MAX
}lrlc_qsh_dump_tag_e;


/*==============================================================================

                   RLC UL STRUCTURE DEFINITIONS

==============================================================================*/


/*! @brief 
    Structure containing RLC UL bearer config and state. 
    This structure is maintained per RB.   
 */
typedef PACK(struct)
{
  uint32 err_mask;               /*!< Any type of error detected in RB config or state
                                          while taking the dump. This is to facilitate 
                                          debugging where any error like PHY BLER or HIGH
                                          memory usage etc is saved in the mask */
  uint8   rb_mode; /*!< TM/UM/AM */
  uint8  lc_id;                  /*!< logical channel ID */
  uint8  rb_cfg_idx;             /*!< RB configuration index: unique */
  uint8  max_retx_threshold;     /*!< Maximum number of re-transmissions */
  uint16 t_poll_retransmit;      /*!< Indicates the value of timer T_poll_retransmit
                                           in ms */
  uint16 vt_s;  /*!< Send state variable: VT(S) or VT(US). */
  uint16 vt_a;  /*!< Acknowledgment state variable: It holds the SN of the next
                     AMD PDU for which a positive ACK is to be received in
                     sequence. It also serves as the lower edge of the TX window
                     and the STATUS receiving window. */
  uint16 vt_ms; /*!< Maximum send state variable:This state variable equals
                     VT(A) + AM_Window_Size, and it serves as the higher edge of
                     the transmitting window (TX_WIN) */
  uint32  retx_q_size; /*!< Size in bytes of the data in the retx q */

}lrlc_qsh_ul_rb_info_s;


/*! @brief 
    Structure containing RLC UL bearer statistics. 
    This structure is maintained per RB.   
 */
typedef PACK(struct)
{
  uint8 rb_cfg_idx;                   /*!< RB configuration index */
  uint8 num_rst;                      /*!< number of reestablishments since
                                                activated */
  uint32 num_new_data_pdus_txed;      /*!< Total number of new RLC PDUs txed */
  uint64 new_data_pdu_bytes_txed;     /*!< Total new data pdu bytes transmitted
                                                including header */
  uint32 num_pdus_re_txed;           /*!< Total number of re-txed RLC PDUs */
 
  uint32 num_complete_nacks_rcvd;     /*!< Total number of NACKs received for
                                                complete RLC PDUs */
  uint32 num_segment_nacks_rcvd;      /*!< Total number of NACKs received for
                                                RLC PDU segments */
  uint32 num_invalid_ctrl_pdus_rcvd;  /*!< Total num of invalid control PDUs
                                                received */
  uint32 num_t_poll_retx_expiry;      /*!< Total number of times
                                                t_PollRetransmission expires */
  uint32 num_max_pdus_per_lc_exceeded; /*!< num of times rlc built max pdus per
                                                  logical channel */
  uint32 num_fretx_pdus;               /*!< num of fast retx RLC PDUs */
}lrlc_qsh_ul_rb_stats_s;


/*! @brief 
    Structure containing RLC UL throughput. Each of these statistics are
    calculated across all bearers. 
 */
typedef PACK(struct)
{
  uint32 new_data_bytes_txed;    /*!< Total data bytes transmitted */
  uint32 retx_data_bytes_txed;   /*!< Total retx data bytes transmitted */
}lrlc_qsh_ul_tput_s;


/*! @brief 
    Structure containing RLC UL warnings. Each of these statistics are
    calculated across all bearers. 
 */
typedef PACK(struct)
{
  uint32 unrecoverable_errors;   /*!< This is for MAX_RETX causing RLF */
}lrlc_qsh_ul_warnings_s;


/*! @brief 
    Structure Tag containing global RLC UL statistics. Each of these statistics are 
    accumulated across all bearers.
 */
typedef PACK(struct)
{
  qsh_dump_tag_hdr_s      hdr;          /* this header will have a tag and length */
  uint32  am_bytes_held;       /*!< total bytes held for AM */
  uint32  um_bytes_held;       /*!< total bytes held for UM */
  uint32  am_rlc_pdus;         /*!< num of AM RLC PDUs  */
  uint32  num_rlcul_errors;    /*!< num of errors in RLC DL  */
}lrlc_qsh_dump_tag_ul_global_stats_s;


/*! @brief 
    Structure Tag containing RLC UL bearer config and state of all the active bearers. 
 */
typedef PACK(struct)
{
  qsh_dump_tag_hdr_s      hdr;           /*!< this header will have a tag and length */
  uint8                   num_active_rbs;/*!< num of active RBs */
  lrlc_qsh_ul_rb_info_s   ul_rb_info[LRLC_QSH_RB_INFO_MAX];
}lrlc_qsh_dump_tag_ul_rb_info_s;


/*! @brief 
    Structure Tag containing RLC UL bearer statistics of all the active bearers.
 */
typedef PACK(struct)
{
  qsh_dump_tag_hdr_s      hdr;           /*!< this header will have a tag and length */
  uint8                   num_active_rbs;/*!< num of active RBs */
  lrlc_qsh_ul_rb_stats_s  ul_rb_stats[LRLC_QSH_RB_STATS_MAX];
}lrlc_qsh_dump_tag_ul_rb_stats_s;


/*! @brief 
    Structure containing RLC UL metrics. Each of these statistics are
    calculated across all bearers. Each member of the structure holds value for a sample 
    period of 1s.
 */
typedef struct
{
  qsh_metric_hdr_s hdr;          /*!< This header has the timestamp (uint16) */
  lrlc_qsh_ul_tput_s rlc_ul_tput;/*!< RLC UL throughput */
  lrlc_qsh_ul_warnings_s rlc_ul_warnings;/*!< RLC UL warnings */
}lrlc_qsh_metric_ul_stats_s;


/*==============================================================================

                   RLC DL STRUCTURE DEFINITIONS

==============================================================================*/


/*! @brief 
    Structure containing RLC DL bearer configuration and state. 
    This structure is maintained per RB.   
 */
typedef PACK(struct)
{
  uint32 err_mask;               /*!< Any type of error detected in RB config or state
                                          while taking the dump. This is to facilitate 
                                          debugging where any error like PHY BLER or HIGH
                                          memory usage etc is saved in the mask */
  uint8  rb_mode;  /*!< TM/UM/AM */
  uint8  rb_cfg_idx;             /*!< RB configuration index: unique
                                          RB cfg idx 1 - 32 for DRBs
                                          RB cfg idx 33-34 for SRBs */
  uint16 t_reordering;          /*!< Indicates the value of timer T_reordering in ms */
  uint16 t_status_prohibit;     /*!< Indicates the value of timer
                                          T_status_prohibit in ms */
 
  uint16 vr_r;                  /*!< VR(R) or VR(UR) */
  uint16 vr_x;                  /*!< VR(X) or VR(UX) */
  uint16 vr_h;                  /*!< VR(H) */
}lrlc_qsh_dl_rb_info_s;


/*! @brief 
    Structure containing RLC DL bearer statistics. 
    This structure is maintained per RB.   
 */
typedef PACK(struct)
{
  uint8  rb_cfg_idx;          /*!< RB configuration index */
  uint16  num_rst;            /*!< number of reestablishments (cfg) since powerup */
  uint32  num_data_pdu;       /*!< number of RLC data pdu received */
  uint64  data_pdu_bytes;     /*!< RLC data PDU in bytes received */
  uint32  num_invalid_pdu;    /*!< number of invalid PDUs */
  uint32  num_dup_dropped_pdu;/*!< number of duplicate and dropped PDUs received */
  uint32  num_dropped_pdu_fc; /*!< number of dropped PDUs due to flow control */
  uint32  num_comp_nack;      /*!< number of complete PDU NACKs sent in status */
  uint32  num_segm_nack;      /*!< number of segmented PDU NACKs sent */
  uint32  num_reord_exp;      /*!< number of times t_reordering expired */
  uint32  num_reord_start;    /*!< number of times t_reordering started */
  uint32  num_missed_um_pdu;  /*!< number of UM PDUs missing on DL*/

 }lrlc_qsh_dl_rb_stats_s;


/*! @brief 
    Structure containing RLC DL TPUT. Each of these statistics are calculated 
    across all bearers. 
 */
typedef struct
{
  uint32 data_bytes_rxed;       /*!< Total data bytes received */
}lrlc_qsh_dl_tput_s;


/*! @brief 
    Structure containing RLC DL Warnings. Each of these statistics are calculated 
    across all bearers. 
 */
typedef struct
{
  uint32  num_rlcdl_errors;     /*!< number of errors in RLC DL */
  uint16  rlc_pdcp_q_full_cnt;  /*!< number of times the RLC->PDCP Q is full */
  uint16  num_fc_down_cmd;      /*!< number of down commands */
  uint16  num_fc_up_cmd;        /*!< number of up commands */
  uint16  num_shutdown_issued;  /*!< number of shutdown commands */
  uint16  num_rst;              /*!< number of reestablishments */
}lrlc_qsh_dl_warnings_s;


/*! @brief 
    Structure Tag containing global RLC DL statistics. Each of these statistics are 
    accumulated across all bearers. 
 */
typedef PACK(struct)
{
  qsh_dump_tag_hdr_s      hdr;           /* this header will have a tag and length */
  uint32  total_data_bytes_held;/*!< total bytes held in DL */
  uint32  num_rlcdl_errors;     /*!< number of errors in RLC DL */
  uint16  num_fc_down_cmd;      /*!< number of down commands */
  uint16  num_fc_up_cmd;        /*!< number of up commands */
  uint16  num_shutdown_issued;  /*!< number of shutdown commands */
  uint32  num_fc_level1_down_cmd;/*!< number of down commands */  
  uint32  num_fc_level1_up_cmd;  /*!< number of up commands */
  uint16  rlc_pdcp_q_full_cnt;  /*!< number of times the RLC->PDCP Q is full */
}lrlc_qsh_dump_tag_dl_global_stats_s;


/*! @brief 
    Structure Tag containing RLC DL bearer config and state of all the active bearers.
*/
typedef PACK(struct)
{
  qsh_dump_tag_hdr_s      hdr;           /*!< this header will have a tag and length */
  uint8                   num_active_rbs;/*!< number of active rbs */
  lrlc_qsh_dl_rb_info_s   dl_rb_info[LRLC_QSH_RB_INFO_MAX];
}lrlc_qsh_dump_tag_dl_rb_info_s;


/*! @brief 
    Structure Tag containing RLC DL bearer stats of all the active bearers.
*/
typedef PACK(struct)
{
  qsh_dump_tag_hdr_s      hdr;           /*!< this header will have a tag and length */
  uint8                   num_active_rbs;/*!< number of active rbs */
  lrlc_qsh_dl_rb_stats_s  dl_rb_stats[LRLC_QSH_RB_STATS_MAX];
}lrlc_qsh_dump_tag_dl_rb_stats_s;


/*! @brief 
    Structure containing RLC DL metrics. Each of these statistics are calculated 
    across all bearers. Each member of the structure holds value for a sample 
    period of 1s.
 */
typedef struct
{
  qsh_metric_hdr_s hdr;          /*!< This header has the timestamp (uint16) */
  lrlc_qsh_dl_tput_s rlc_dl_tput;/*!< RLC DL TPUT */
  lrlc_qsh_dl_warnings_s rlc_dl_warnings;/*!< RLC DL Warnings */
}lrlc_qsh_metric_dl_stats_s;

typedef struct
{
   qsh_dump_hdr_s hdr;
   lrlc_qsh_dump_tag_dl_rb_info_s dl_rb_info;
   lrlc_qsh_dump_tag_dl_rb_stats_s dl_rb_stats;
   lrlc_qsh_dump_tag_dl_global_stats_s dl_gbl;
   lrlc_qsh_dump_tag_ul_rb_info_s ul_rb_info;
   lrlc_qsh_dump_tag_ul_rb_stats_s ul_rb_stats;
   lrlc_qsh_dump_tag_ul_global_stats_s ul_gbl;
}lrlc_qsh_dump_collect_s;
/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

#endif /* LRLC_QSH_EXT_H */
