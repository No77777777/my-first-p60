/*!
  @file
  lte_pdcp_msg.h

  @brief
  This file contains all External message data structure and IDs interfaces
  exported by PDCP layer.


*/

/*===========================================================================

  Copyright (c) 2021 QUALCOMM Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/lte.mpss/6.6.1/api/lte_pdcp_msg.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/04/21   ve      CR2714603: Indicate DS when the pkt is discarded and buffer is empty
12/27/16   sb      CR1105435: Discrepancy in UDC compression gain and update log packets
11/29/16   sb      CR1085295:Overflow of Data bytes received causing Apex logging issue
12/15/15   sb      CR856806; HFN resync enhancement for RoHC bearer - FR28762
07/23/15   sb      CR867973: Enhance logging and QSH for UDC
06/15/15   el      CR820308: Mav Debug Snapshot Merged from following
                   (777357,801252,801937,809117)
05/21/15   sb      CR 803880: modified PDCP interface for UDC feature
05/14/15   sb      CR 838124: Featurize code for Tabasco
04/30/15   sb      CR800409: HFN resync enhancement - FR24395
09/19/14   sb      CR706535: Defense mechanism in UE to counter HFN mismatch
09/04/14   sb      CR694693: Fix to ensure UL PDUs are not built after RAB
                   is released
08/05/14   sb      CR696130: Reset Ciphering parameter in OFFLOAD when security 
                   parameter changes
04/11/14   mg      CR647594: QSH Phase 1
07/14/14   mg      CR665352: PDCP UL statistics API
07/03/14   mg      CR669056: PDCP discard timer algorithm
04/02/14   sb      CR642230: REL11 RoHC context continuity during HO 
                   within same eNB
01/22/14   md      CR564602: Support for continuing RoHC context 
                   during handover within the same eNB.
11/19/13   sb      CR535830: PDCP/RLC Re-arch and support of REL-11 features
11/18/13   sb      CR576794: New UL and DL RAB DEREG messages defined 
10/13/13   mg      CR 561786: Downlink PDCP changes for BOLT.
06/30/13   sb      CR495372: Remove LTE_PDCPDL_PEER_PIGGYBACK_ROHC_FB_IND
                   message and handle piggyback RoHC FB synchronously
05/13/13   sb      CR484633: Avoid crash because of PDCPDL message Q full with 
                   CRYPTO_DONE_IND message
06/16/12   sb      CR365196 and CR365004: Fix IOT to handle 
                   scenarios where RB switches back and forth with RoHC enabled and 
                   disabled. Also to reduce the delay in UL after HO
04/20/12   ap      added support for providing the sfn and subfn for DL DCCH
09/06/11   ax      add support for piggyback RoHC feedback
06/15/11   ax      RoHC support
05/16/11   ae      Inclue lte_security.h
03/21/11   mm      Defined ROHC profile mask type
02/23/11   ax      add support for SMC with invalid MAC-I followed by valid SMC
01/31/11   ax      add support for pdcp dl flow control
09/17/10   wshaw   add watchdog support for LTE tasks
04/28/10   ar      added message structures and UMID defs for PDCPUL and PDCPDL
                   START and STOP request and confirm messages
03/03/10   ar      added FTM related msg router messages and msg structures
10/13/09   dd      Changing the Counter RSPs to CNFs
07/20/09   dd      Added pdcp_hdr to the lte_pdcpdl_sdu_ind_msg_s
06/17/09   bq      Add Crypto Done Ind for UL and DL.
05/27/09   bq      Move Security Config from RB level to top level.
05/11/09   bq      Added Security related interface.
04/20/09   dd      Added lte_pdcdl_counter_req_msg_s,
                   lte_pdcdl_counter_rsp_msg_s,
                   lte_pdcul_counter_req_msg_s, lte_pdcul_counter_rsp_msg_s
04/15/09   mm      Removed hand_over field in lte_pdcpul_recfg_prep_req_msg_s
                   and added cfg_reason field in its place. This is for RRC
                   to indicate RLF to PDCP.
04/10/09   bq      add MACRO for LTE_PDCP_DISCARD_TIMER_NOT_CONFIG
07/07/08   bq      Initial version
===========================================================================*/

#ifndef LTE_PDCP_MSG_H
#define LTE_PDCP_MSG_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include <comdef.h>
#include <dsm_queue.h>
#include <msgr.h>
#include <msgr_lte.h>
#include <lte_app.h>
#include "lte_l2_comdef.h"
#include <lte_security.h>
#include <qsh.h>
#include "ps_qos_defs.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*-----------------------------------------------------------------------*/
/*     FTM to PDCP External message definitions                          */
/*-----------------------------------------------------------------------*/

/*! @brief Enter Factory Test Mode(FTM) Request from FTM module to PDCPUL
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;  /*!< message header */
} lte_pdcpul_enter_ftm_req_msg_s;

/*! @brief Enter Factory Test Mode(FTM) confirm from PDCPUL to FTM module
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;  /*!< message header */
} lte_pdcpul_enter_ftm_cnf_msg_s;

/*! @brief Enter Factory Test Mode(FTM) Request from FTM module to PDCPDL
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;  /*!< message header */
} lte_pdcpdl_enter_ftm_req_msg_s;

/*! @brief Enter Factory Test Mode(FTM) confirm from PDCPDL to FTM module
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;  /*!< message header */
} lte_pdcpdl_enter_ftm_cnf_msg_s;

/*! @brief Exit Factory Test Mode(FTM) Request from FTM module to PDCPUL
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;  /*!< message header */
} lte_pdcpul_exit_ftm_req_msg_s;

/*! @brief Exit Factory Test Mode(FTM) confirm from PDCPUL to FTM module
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;  /*!< message header */
} lte_pdcpul_exit_ftm_cnf_msg_s;

/*! @brief Exit Factory Test Mode(FTM) Request from FTM module to PDCPDL
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;  /*!< message header */
} lte_pdcpdl_exit_ftm_req_msg_s;

/*! @brief Exit Factory Test Mode(FTM) confirm from PDCPDL to FTM module
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;  /*!< message header */
} lte_pdcpdl_exit_ftm_cnf_msg_s;

/*! @brief LTE_PDCPXX_START_REQ request message definition
*/
typedef struct
{
  msgr_hdr_struct_type   msg_hdr;    /*!< common message router header */
  boolean tune_away_enabled;        /*!< TRUE indicates TA can happen */
} lte_pdcp_start_req_msg_s;

/*! @brief LTE_PDCPXX_START_CNF request message definition
*/
typedef struct
{
  msgr_hdr_struct_type   msg_hdr;    /*!< common message router header */
  lte_errno_e            cnf_status; /*!< confimation status message */
} lte_pdcp_start_cnf_msg_s;

/*! @brief LTE_PDCPXX_STOP_REQ request message definition
*/
typedef struct
{
  msgr_hdr_struct_type   msg_hdr;  /*!< common message router header */
} lte_pdcp_stop_req_msg_s;

/*! @brief LTE_PDCPXX_STOP_CNF request message definition
*/
typedef struct
{
  msgr_hdr_struct_type   msg_hdr;    /*!< common message router header */
  lte_errno_e            cnf_status; /*!< confimation status message */
} lte_pdcp_stop_cnf_msg_s;

/*-----------------------------------------------------------------------*/
/*     IMS to PDCP External message definitions                          */
/*-----------------------------------------------------------------------*/

/*! @brief Number of different ranges of PDCP UL delays
*/
#define LTE_PDCP_DELAY_STATS_SIZE               21

/*! @brief PDCP UL statistics for UDC
*/
typedef struct
{
  uint32  num_udc_comp_pkts; /*!<*total num of UDC compressed pkts*/
  uint32  udc_comp_bytes; /*!<*total num of UDC compressed	bytes*/  
  uint32  num_udc_fc_uncomp_pkts; /*!< total num of uncompressed pkts not attemped UDC due to FC*/
  uint32  udc_fc_uncomp_bytes; /*!< total num of uncompressed bytes not attemped UDC due to FC*/
  uint32  num_udc_uncomp_pkts; /*!<*total num of uncompressed pkts attempted UDC*/
  uint32  udc_uncomp_bytes; /*!<*total num of uncompressed bytes attempted UDC compression*/
  uint32  num_udc_fail;  /*!< Number of UDC compression failed*/
  uint16  num_udc_reset; /*!< Num of time UDC RESET is called */
  uint32  num_ue_udc_ctrl_pdus;  /*!< num of UE triggered UDC Ctrl PDUs */
  uint32  num_enb_udc_ctrl_pdus;  /*!< num of eNB triggered UDC Ctrl PDUs */
  uint32  num_udc_dsm_fc_down;	   /*!< num of	CMD_DOWN for DSM monitor*/
  uint32  num_udc_dsm_fc_shutdown;  /*!< num of  CMD_SHUT_DOWN for DSM monitor*/
  uint32  num_udc_cpu_fc_down;  /*!< num of	CMD_DOWN for CPU monitor*/
  uint32  num_udc_cpu_fc_freeze;   /*!< num of	CMD_UP for CPU monitor*/
  uint32  num_enb_trigg_udc_reset;/*!<num of UDC reset triggered by enB during non-HO/non-RLF*/
}lte_pdcpul_udc_stats_s;

/*! @brief PDCP UL statistics, part of the control block
  */
typedef struct
{
  uint32  num_flow_ctrl_trig;  /*!< number of timer flow disable */
  uint32  num_data_pdu;        /*!< number of PDCP pdu transmitted */
  uint64  data_pdu_bytes;      /*!< PDCP PDU in bytes transmitted */
  uint64  data_bytes_from_ps;  /*!< Total bytes dequeued from PS Wm*/
  uint32  num_control_pdu;     /*!< number of PDCP pdu transmitted */
  uint32  control_pdu_bytes;   /*!< PDCP PDU in bytes transmitted */
  uint32  num_status_report;  /*!< number of status report transmitted */
  uint32  num_rohc_feedback;  /*!< number of ROHC feedback transmitted */
  uint32  num_rohc_fail;      /*!< nmuber of ROHC compression failed */
  uint32  num_discard_sdu;   /*!< number of time based discard SDUs */
  uint32  discard_sdu_bytes; /*!< time based discard SDUs bytes */
  uint32  num_retransmit_sdu;  /*!< number of retransmit SDUs */
  uint32  retransmit_sdu_bytes; /*!< retransmit SDU in bytes */
  uint32  num_piggyback_rohc_feedback_rxed;  /*!< number of piggyback ROHC 
                                                  feedback rxed */
  uint32  num_rerohc_pdu_drop;  /*!< number of RoHC PDUs drop during HO*/
  uint32  rerohc_pdu_drop_bytes; /*!< number of RoHC PDU bytes drop during HO*/
  uint32  pdcp_delay_pkt_cnt[LTE_PDCP_DELAY_STATS_SIZE]; /*number of packets
  for each delay range bin. Last bin conatins packets having delay greater
  than a range*/
  lte_pdcpul_udc_stats_s udc_stats;  /*!< Stats related to UDC*/
} lte_pdcpul_stats_set_s;

/*! @brief UL RB statistics
*/
typedef struct
{
  lte_eps_id_t eps_id;
  lte_pdcpul_stats_set_s rb_stats;
}lte_pdcpul_rb_stats_s;

typedef struct
{
  lte_pdcpul_rb_stats_s rb_metric_stats[LTE_MAX_ACTIVE_DRB];
}lte_pdcpul_qual_metrics_s;

/*! @brief UL data metric statistics
*/
typedef struct
{
  msgr_hdr_struct_type   hdr;          /*!< common message router header */
  lte_eps_id_t eps_id;                     /*!< eps bearer for which info is requested*/
  lte_pdcpul_qual_metrics_s* rb_stats_ptr; /*!< Pointer to the allocated memory where L2 will return the requested DRB stats*/
}lte_pdcpul_rb_stats_req_msg_s;

/*! @brief UL data Stats CNF message
*/
typedef struct
{
  msgr_hdr_struct_type   msg_hdr;     /*!< common message router header */
  lte_errno_e            cnf_status;  /*!< confirmation status message  - SUCCESS/FAILURE*/
  uint8                  num_report;  /*!<number of reported structure(s) (one report per DRB).*/
}lte_pdcpul_rb_stats_cnf_msg_s;

/*! @brief PDCP AMR codec Info type*/
typedef enum
{
  LTE_PDCP_AMR_CODEC_INVALID = 0,
  LTE_PDCP_AMR_CODEC_NB_4_75_KBPS,
  LTE_PDCP_AMR_CODEC_NB_5_15_KBPS,
  LTE_PDCP_AMR_CODEC_NB_5_90_KBPS,
  LTE_PDCP_AMR_CODEC_NB_6_70_KBPS,
  LTE_PDCP_AMR_CODEC_NB_7_40_KBPS,
  LTE_PDCP_AMR_CODEC_NB_7_95_KBPS,
  LTE_PDCP_AMR_CODEC_NB_10_20_KBPS,
  LTE_PDCP_AMR_CODEC_NB_12_20_KBPS,
  LTE_PDCP_AMR_CODEC_WB_6_60_KBPS,
  LTE_PDCP_AMR_CODEC_WB_8_85_KBPS,
  LTE_PDCP_AMR_CODEC_WB_12_65_KBPS,
  LTE_PDCP_AMR_CODEC_WB_14_25_KBPS,
  LTE_PDCP_AMR_CODEC_WB_15_85_KBPS,
  LTE_PDCP_AMR_CODEC_WB_18_25_KBPS,
  LTE_PDCP_AMR_CODEC_WB_19_85_KBPS,
  LTE_PDCP_AMR_CODEC_WB_23_05_KBPS,
  LTE_PDCP_AMR_CODEC_WB_23_85_KBPS,
  LTE_PDCP_AMR_CODEC_MAX
}lte_pdcp_amr_codec_e;

/*! @brief voice call AMR Codec Info Ind
*/
typedef struct
{
  msgr_hdr_struct_type   hdr;            /*!< common message router header */
  lte_eps_id_t eps_id;                   /*!< eps bearer for which info is requested*/
  lte_qci_enum_type  qci_value ; /* QOS information of this EPS bearer. */
  lte_pdcp_amr_codec_e  amr_codec_type;  /*!< AMR codec rate of this EPS bearer*/
}lte_pdcpul_amr_codec_info_ind_msg_s;

/*! @brief low latency info Ind
*/
typedef struct
{
  msgr_hdr_struct_type   hdr;            /*!< common message router header */
  boolean                wm_delay_flg;   /*!< wm delay reach threshold or not */
}lte_pdcpul_low_latency_ind_msg_s;

/*! @brief Pkt discard indication to DS
*/
typedef struct
{
  msgr_hdr_struct_type   hdr;            /*!< message header */
  lte_eps_id_t           eps_id;         /*!< eps bearer for which info is related*/
}lte_pdcpul_pkt_discard_msg_s;

/*-----------------------------------------------------------------------*/
/*     LTE PDCP/PS External message definitions                          */
/*-----------------------------------------------------------------------*/
/*! @brief RAB register request from PS to PDCP UL
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;         /*!< message header */
  /* Message payload */
  lte_eps_id_t                        eps_id;      /*!< EPS ID */
  void *                              ps_pdcp_wm_ptr; /*!< PS to PDCP WM ptr */
  boolean                             allow_llm_discard_timer; /*  TRUE = allow use llm_discard_timer */
} lte_pdcpul_rab_register_req_msg_s;

/*! @brief RAB register request confirmation from PDCP UL to PS
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;         /*!< message header */
  /* Message payload */
  lte_eps_id_t                        eps_id;      /*!< EPS ID */
  lte_errno_e                         status;      /*!< status */
} lte_pdcpul_rab_register_cnf_msg_s;

/*! @brief RAB register request from PS to PDCP DL to let PDCP DL register PDCP DL
     to A2 WM with A2
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;         /*!< message header */
  /* Message payload */
  lte_eps_id_t                        eps_id;      /*!< EPS ID */
  dsm_watermark_type*                 pdcp_wm_ptr; /*!< PS to PDCP WM ptr */
} lte_pdcpdl_rab_register_req_msg_s;

/*! @brief RAB register request confirmation from PDCP DL to PS
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;         /*!< message header */
  /* Message payload */
  lte_eps_id_t                        eps_id;      /*!< EPS ID */
  lte_errno_e                         status;      /*!< status */
} lte_pdcpdl_rab_register_cnf_msg_s;

/*! @brief RAB deregister request from PS to PDCP  to let PDCP DL/UL deregister
     DL/UL WM
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;         /*!< message header */
 lte_eps_id_t                        eps_id;      /*!< EPS ID */
} lte_pdcp_rab_deregister_req_msg_s;

/*! @brief RAB deregister request confirmation from PDCP DL/UL to PS
*/
typedef struct
{
  msgr_hdr_struct_type                hdr;         /*!< message header */
  lte_eps_id_t                        eps_id;      /*!< EPS ID */
  lte_errno_e                         status;      /*!< status */
} lte_pdcp_rab_deregister_cnf_msg_s;

/*-----------------------------------------------------------------------*/
/*     QSH to PDCP External message definitions                          */
/*-----------------------------------------------------------------------*/
/*! @brief LTE_PDCPXX_QSH_REQ request message definition
*/
typedef struct
{
  msgr_hdr_struct_type   msg_hdr;    /*!< common message router header */
  /*! category mask. Use enum qsh_cat_e values to define mask e.g. 
      QSH_CAT_CFG|QSH_CAT_PERF (since the values are bit mask simple
      "or" will work) */
  uint32 category_mask;  
  /*!
    Whether or not to reset stats after analysis. Client should act on this
    flag after processing category_mask.
  */
  boolean reset_flag; 
} lte_pdcp_qsh_analysis_req_msg_s;

/*! @brief LTE_PDCPXX_QSH_REQ request message definition
*/
typedef struct
{
  msgr_hdr_struct_type   msg_hdr;    /*!< common message router header */
    /*! Parameters passed into the callback functions registered with QSH */
  qsh_client_cb_params_s params;
} lte_pdcp_qsh_metric_req_msg_s;

/*-----------------------------------------------------------------------*/
/*     LTE PDCPUL External Request messages                              */
/*-----------------------------------------------------------------------*/
/*! @brief external requests that the LTE PDCPUL module receives.  */
enum
{
  LTE_PDCPUL_FIRST_REQ = MSGR_UMID_BASE(MSGR_LTE_PDCPUL, MSGR_TYPE_REQ),
  MSGR_DEFINE_UMID(LTE,PDCPUL,REQ,RAB_REGISTER,0x00,
                   lte_pdcpul_rab_register_req_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPUL,REQ,RAB_DEREGISTER,0x01,
                   lte_pdcp_rab_deregister_req_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPUL,REQ,ENTER_FTM,0x02,
                   lte_pdcpul_enter_ftm_req_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPUL,REQ,EXIT_FTM,0x03,
                   lte_pdcpul_exit_ftm_req_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPUL,REQ,RB_STATS,0x04,
                   lte_pdcpul_rb_stats_req_msg_s),                   
  MSGR_DEFINE_UMID(LTE,PDCPUL,REQ,START,0x5,
                   lte_pdcp_start_req_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPUL,REQ,STOP,0x6,
                   lte_pdcp_stop_req_msg_s),  
  MSGR_DEFINE_UMID(LTE,PDCPUL,REQ,QSH_METRIC,0x7,
                   lte_pdcp_qsh_metric_req_msg_s),  
  LTE_PDCPUL_MAX_REQ,
  LTE_PDCPUL_LAST_REQ = LTE_PDCPUL_MAX_REQ - 1
};

/*-----------------------------------------------------------------------*/
/*     LTE PDCPDL External Request messages                              */
/*-----------------------------------------------------------------------*/
/*! @brief external requests that the LTE PDCPDL module sends/receives.  */
enum
{
  LTE_PDCPDL_FIRST_REQ = MSGR_UMID_BASE(MSGR_LTE_PDCPDL, MSGR_TYPE_REQ),
  MSGR_DEFINE_UMID(LTE,PDCPDL,REQ,ENTER_FTM,0x00,
                   lte_pdcpdl_enter_ftm_req_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPDL,REQ,EXIT_FTM,0x01,
                   lte_pdcpdl_exit_ftm_req_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPDL,REQ,FC,0x02,
                   cfcm_cmd_msg_type_s),
  MSGR_DEFINE_UMID(LTE,PDCPDL,REQ,RAB_REGISTER,0x03,
                   lte_pdcpdl_rab_register_req_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPDL,REQ,RAB_DEREGISTER,0x04,
                   lte_pdcp_rab_deregister_req_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPDL,REQ,QSH_ANALYSIS,0x05,
                   lte_pdcp_qsh_analysis_req_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPDL,REQ,START,0x06,
                   lte_pdcp_start_req_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPDL,REQ,STOP,0x07,
                   lte_pdcp_stop_req_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPDL,REQ,QSH_METRIC,0x08,
                   lte_pdcp_qsh_metric_req_msg_s),
  LTE_PDCPDL_MAX_REQ,
  LTE_PDCPDL_LAST_REQ = LTE_PDCPDL_MAX_REQ - 1
};

/*-----------------------------------------------------------------------*/
/*     LTE PDCPUL External Confirmation messages                         */
/*-----------------------------------------------------------------------*/
/*! @brief external Confirmations that the LTE PDCPUL module sends  */
enum
{
  LTE_PDCPUL_FIRST_CNF = MSGR_UMID_BASE(MSGR_LTE_PDCPUL, MSGR_TYPE_CNF),
  MSGR_DEFINE_UMID(LTE,PDCPUL,CNF,RAB_REGISTER,0x00,
                   lte_pdcpul_rab_register_cnf_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPUL,CNF,ENTER_FTM,0x01,
                   lte_pdcpul_enter_ftm_cnf_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPUL,CNF,EXIT_FTM,0x02,
                   lte_pdcpul_exit_ftm_cnf_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPUL,CNF,RAB_DEREGISTER,0x03,
                   lte_pdcp_rab_deregister_cnf_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPUL,CNF,RB_STATS,0x04,
                   lte_pdcpul_rb_stats_cnf_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPUL,CNF,START,0x05,
                   lte_pdcp_start_cnf_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPUL,CNF,STOP,0x06,
                   lte_pdcp_stop_cnf_msg_s),
  LTE_PDCPUL_MAX_CNF,
  LTE_PDCPUL_LAST_CNF = LTE_PDCPUL_MAX_CNF - 1
};

/*-----------------------------------------------------------------------*/
/*     LTE PDCPDL External Confirmation messages                         */
/*-----------------------------------------------------------------------*/
/*! @brief external Confirmations that the LTE PDCPDL module sends  */
enum
{
  LTE_PDCPDL_FIRST_CNF = MSGR_UMID_BASE(MSGR_LTE_PDCPDL, MSGR_TYPE_CNF),
  MSGR_DEFINE_UMID(LTE,PDCPDL,CNF,RAB_REGISTER,0x00,
                   lte_pdcpdl_rab_register_cnf_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPDL,CNF,ENTER_FTM,0x01,
                   lte_pdcpdl_enter_ftm_cnf_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPDL,CNF,EXIT_FTM,0x02,
                   lte_pdcpdl_exit_ftm_cnf_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPDL,CNF,RAB_DEREGISTER,0x03,
                   lte_pdcp_rab_deregister_cnf_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPDL,CNF,START,0x04,
                   lte_pdcp_start_cnf_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPDL,CNF,STOP,0x05,
                   lte_pdcp_stop_cnf_msg_s),
  LTE_PDCPDL_MAX_CNF,
  LTE_PDCPDL_LAST_CNF = LTE_PDCPDL_MAX_CNF - 1
};

/*-----------------------------------------------------------------------*/
/*     LTE PDCPDL External Indication messages                           */
/*-----------------------------------------------------------------------*/
/*! @brief external Indications that the LTE PDCPDL module sends  */
enum
{
  LTE_PDCPDL_FIRST_IND = MSGR_UMID_BASE(MSGR_LTE_PDCPDL, MSGR_TYPE_IND),
  LTE_PDCPDL_MAX_IND,
  LTE_PDCPDL_LAST_IND = LTE_PDCPDL_MAX_IND - 1
};

/*-----------------------------------------------------------------------*/
/*     LTE PDCPUL External Indication messages                           */
/*-----------------------------------------------------------------------*/
/*! @brief external Indications that the LTE PDCPUL module sends         */
enum
{
  LTE_PDCPUL_FIRST_IND = MSGR_UMID_BASE(MSGR_LTE_PDCPUL, MSGR_TYPE_IND),
  MSGR_DEFINE_UMID(LTE,PDCPUL,IND,AMR_CODEC_INFO,0x00,
                   lte_pdcpul_amr_codec_info_ind_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPUL,IND,LOW_LATENCY,0x01,
                   lte_pdcpul_low_latency_ind_msg_s),
  MSGR_DEFINE_UMID(LTE,PDCPUL,IND,PKT_DISCARD,0x02,
                   lte_pdcpul_pkt_discard_msg_s),
  LTE_PDCPUL_MAX_IND,
  LTE_PDCPUL_LAST_IND = LTE_PDCPUL_MAX_IND - 1
};

/*-----------------------------------------------------------------------*/
/*     LTE PDCP COMP External Indication messages                           */
/*-----------------------------------------------------------------------*/
/*! @brief external Indications that the LTE PDCP COMP module sends  */
enum
{
  LTE_PDCPCOMP_FIRST_IND = MSGR_UMID_BASE(MSGR_LTE_PDCPCOMP, MSGR_TYPE_IND),
  MSGR_DEFINE_UMID(LTE,PDCPCOMP,IND,FC,0x00,
                   cfcm_cmd_msg_type_s),
  LTE_PDCPCOMP_MAX_IND,
  LTE_PDCPCOMP_LAST_IND = LTE_PDCPCOMP_MAX_IND - 1
};

/*-----------------------------------------------------------------------*/
/*     LTE PDCPUL Internal timer Indication messages                           */
/*-----------------------------------------------------------------------*/
/*! @brief Internal timer Indications that the LTE PDCPUL module receives  */
enum
{
  LTE_PDCPUL_FIRST_TMRI = MSGR_UMID_BASE(MSGR_LTE_PDCPUL, MSGR_TYPE_TMRI),
  LTE_PDCPUL_MAX_TMRI,
  LTE_PDCPUL_LAST_TMRI = LTE_PDCPUL_MAX_TMRI - 1
};

/*-----------------------------------------------------------------------*/
/*     LTE PDCPDL Internal timer Indication messages                           */
/*-----------------------------------------------------------------------*/
/*! @brief Internal timer Indications that the LTE PDCPDL module receives  */
enum
{
  LTE_PDCPDL_FIRST_TMRI = MSGR_UMID_BASE(MSGR_LTE_PDCPDL, MSGR_TYPE_TMRI),
  LTE_PDCPDL_MAX_TMRI,
  LTE_PDCPDL_LAST_TMRI = LTE_PDCPDL_MAX_TMRI - 1
};

enum
{
  MSGR_DEFINE_UMID(LTE, PDCPDL, SPR, LOOPBACK, MSGR_ID_LOOPBACK,
                   msgr_spr_loopback_s),
  MSGR_DEFINE_UMID(LTE, PDCPDL, SPR, LOOPBACK_REPLY, MSGR_ID_LOOPBACK_REPLY,
                   msgr_spr_loopback_reply_s),
  MSGR_DEFINE_UMID(LTE, PDCPDL, SPR, THREAD_READY, LTE_APP_ID_THREAD_READY,
                   none),
  MSGR_DEFINE_UMID(LTE, PDCPDL, SPR, THREAD_KILL, LTE_APP_ID_THREAD_KILL,
                   none)
};

/*! @brief Supervisory messages that PDCP UL receives or throws
*/
enum
{
  MSGR_DEFINE_UMID(LTE, PDCPUL, SPR, LOOPBACK, MSGR_ID_LOOPBACK,
                   msgr_spr_loopback_s),
  MSGR_DEFINE_UMID(LTE, PDCPUL, SPR, LOOPBACK_REPLY, MSGR_ID_LOOPBACK_REPLY,
                   msgr_spr_loopback_reply_s),
  MSGR_DEFINE_UMID(LTE, PDCPUL, SPR, THREAD_READY, LTE_APP_ID_THREAD_READY,
                   none)
};

/*! @brief Supervisory messages that PDCP COMP receives or throws
*/
enum
{  
  MSGR_DEFINE_UMID(LTE, PDCPCOMP, SPR, LOOPBACK, MSGR_ID_LOOPBACK,
                   msgr_spr_loopback_s),
  MSGR_DEFINE_UMID(LTE, PDCPCOMP, SPR, LOOPBACK_REPLY, MSGR_ID_LOOPBACK_REPLY,
                   msgr_spr_loopback_reply_s),
  MSGR_DEFINE_UMID(LTE, PDCPCOMP, SPR, THREAD_READY, LTE_APP_ID_THREAD_READY,
                   none),
  MSGR_DEFINE_UMID(LTE, PDCPCOMP, SPR, THREAD_KILL, LTE_APP_ID_THREAD_KILL,
                   none)
};

/*! @brief Supervisory messages that PDCP OFFLOAD receives or throws
*/

enum
{  
  MSGR_DEFINE_UMID(LTE, PDCPOFFLOAD, SPR, LOOPBACK, MSGR_ID_LOOPBACK,
                   msgr_spr_loopback_s),
  MSGR_DEFINE_UMID(LTE, PDCPOFFLOAD, SPR, LOOPBACK_REPLY, MSGR_ID_LOOPBACK_REPLY,
                   msgr_spr_loopback_reply_s),
  MSGR_DEFINE_UMID(LTE, PDCPOFFLOAD, SPR, THREAD_READY, LTE_APP_ID_THREAD_READY,
                   none),
  MSGR_DEFINE_UMID(LTE, PDCPOFFLOAD, SPR, THREAD_KILL, LTE_APP_ID_THREAD_KILL,
                   none)
};

#endif /* LTE_PDCP_MSG_H */
