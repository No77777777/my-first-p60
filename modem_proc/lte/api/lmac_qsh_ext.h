/*!
  @file
  lmac_qsh_ext.h
 
  @brief
  This file contains all the define structures/function prototypes 
  for LTE MAC qsh info.

 
*/

/*===========================================================================

  Copyright (c) 2015 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law p_rohibited.

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

$Header: //components/rel/lte.mpss/6.6.1/api/lmac_qsh_ext.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/08/17   el      CR2084937: Porting DSDS L+L mac layer changes
03/04/15   ru      CR801937: Pull Mini dump fixes from BO2.1 R12
03/02/15   el      CR796639: GPS padding B13/B27 band restriction and efs 
                   control of feature 
02/24/15   ru      CR777357: Mini Dump And Metrics collection
02/04/15   el      Initial version
===========================================================================*/
#ifndef LMAC_QSH_EXT_H
#define LMAC_QSH_EXT_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include <qsh.h>


/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/
/*! @brief defined QSH major version */
#define LMAC_QSH_MAJOR_VER 0x0001

/*! @brief defined QSH minor version */
#define LMAC_QSH_MINOR_VER 0x0000

/*****************************************************************************
 
                                                     QSH Dump
 
 ****************************************************************************/
/*! @brief Enumeration of mac qsh dump */
typedef enum 
{
  LMAC_QSH_DUMP_TAG_CTRL               = 0,
  LMAC_QSH_DUMP_TAG_FC                 = 1,
  LMAC_QSH_DUMP_TAG_QOS                = 2,
  LMAC_QSH_DUMP_TAG_RACH               = 3,
  LMAC_QSH_DUMP_TAG_UL                 = 4, 
  LMAC_QSH_DUMP_TAG_UL_DATA_INT        = 5,
  LMAC_QSH_DUMP_TAG_DL                 = 6,
  LMAC_QSH_DUMP_TAG_MAX                = 7
} lmac_qsh_dump_tag_e;

/*! @brief Enumeration of mac qsh metric */
typedef enum 
{
  LMAC_QSH_METRIC_RACH_INFO            = 0,
  LMAC_QSH_METRIC_DL_INFO              = 1,
  LMAC_QSH_METRIC_MAX                  = 2
} lmac_qsh_metric_e;


/*! @brief defined Maximum number of BSR LC group */
#define LMAC_QSH_DUMP_BSR_LCGROUP_MAX 4

/*! @brief defined Maximum length of MAC header in core info */
#define LMAC_QSH_DUMP_MAC_HDR_MAX_LEN 20

/*! @brief Global CS structure for MAC CTRL task */
typedef PACK(struct)
{
  boolean        task_is_active; /*!< Whether the "main" function of the thread is running */
  int            msgr_client;    /*!< MSGR client */
  int            mb_id;          /*!< Mailbox Id */
} lmac_qsh_dump_ctrl_cs_s;

/*! @brief MAC internal random access request */
typedef PACK(struct) 
{
  uint8    ra_id;         /*!< RA index. Invalid value = 0xFF*/
  uint8    access_reason; /*!< access reason */
  uint16   crnti;         /*!< UE C-RNTI*/
} lmac_qsh_dump_intmsg_random_access_req_s;

/*! @brief Access related structure for MAC CTRL task*/
typedef PACK(struct)
{
  /*! flag to indicate that the Access procedure is needed to 
  re-start after receiving the abort confirmation*/
  boolean  restart_rach;
  /*! flag to indicate Control task needs to 
  send back the abort confirmation to RRC */
  boolean  send_abort_cnf;
  /*! current internal access request message */
  lmac_qsh_dump_intmsg_random_access_req_s   curr_req;
} lmac_qsh_dump_ctrl_access_data_s;

/*! @brief RRC-MAC configuration parameters structure. Depend of the 
    cfg_type, The specific action will be taken by MAC as following:
  - LTE_L2_CFG_RELEASE : MAC will delete all the existing
    configuration by itself and go back to idle state.
  - LTE_L2_CFG_HO: Go to HO state and depend on the cfg_bitmask,
    MAC should take in the corresponding new configurations.
  - LTE_L2_CFG_RADIO_FAILURE: Go to radio failure state and depend on the 
    cfg_bitmask, MAC should take in the corresponding new configurations.
  - LTE_L2_CFG_RECFG : No state change and depend on the 
    cfg_bitmask, MAC should take in the corresponding new configurations.
  - LTE_L2_CFG_EMBMS : MAC will process the MSI based on the LC info sent by RRC
  - LTE_L2_CFG_RELEASE_EMBMS : MAC will delete all existing multicast LCs
  - LTE_L2_CFG_RELEASE_ALL : MAC will delete all the existing unicast and 
    multicast configuration
  Note that: RRC can still indicate MAC to delete a given configure by
  setting the corresponding bitmask to 1 and set the pointer to NULL.
*/
typedef PACK(struct)
{
  /*! type of configuration */
  uint8     cfg_type; 
  /*! bit masks to indicate which configurations are still pending */
  uint8     cfg_bitmask;
} lmac_qsh_dump_cfg_s;


/*! @brief Global Data structure for MAC CTRL task */
typedef PACK(struct)
{
  uint8                       curr_state;        /*!< current state */
  uint16                      valid_cfg_bitmask; /*!< indicated valid config */
  uint8                       cfg_result_status; /*!< current config status */
  uint8                       informed_action;   /*!<informed action */
  uint16                      crnti;             /*!< current C-RNTI of the UE */
  lmac_qsh_dump_ctrl_access_data_s  access;  /*!< access related data*/
  lmac_qsh_dump_cfg_s     mac_cfg;           /*!< mac config data*/
} lmac_qsh_dump_ctrl_data_s; 

/*! @brief Global structure for MAC CTRL task*/
typedef PACK(struct)
{
  /*! QSH dump tag harder, this header will have a tag and length */
  qsh_dump_tag_hdr_s    hdr; 
  lmac_qsh_dump_ctrl_cs_s     cs;    /*!< all the task control related data */
  lmac_qsh_dump_ctrl_data_s   data;  /*!< all the local data structure */
} lmac_qsh_dump_tag_ctrl_s;

/*! @brief Global CS structure for MAC DL task */
typedef PACK(struct)
{
  boolean         task_is_active; /*!< Whether the "main" function of the thread is running */
  int             msgr_client;    /*!< MSGR client */
  int             mb_id;          /*!< Mailbox Id */
} lmac_qsh_dump_dl_task_s;

/*! @brief Structure for MAC data process statistics counters*/
typedef PACK(struct)
{
  uint32          total_num_dropped_pkts;           /*!< Total number of dropped packets*/
} lmac_qsh_dump_dl_embms_stats_s;

/*! @brief Structure for MAC data process statictis counters */
typedef PACK(struct)
{
  /*! counter to indicate the total number of bad PDU*/
  uint32  total_bad_pdu_cnt;
  /*! counter to indicate the total number of good PDU */
  uint32  total_good_pdu_cnt;
  /*! counter to indicate the total number of bad RAR*/
  uint32  total_bad_rar_cnt;
  /*! counter to indicate the total number of good RAR */
  uint32  total_good_rar_cnt;
  /*! counter to indicate the total number of unexpected RAR */
  uint32  total_unexpected_rar_cnt;
  /*! counter to indicate the number of invalid reserved bits */
  uint32  invalid_reserved_bits_cnt;
  /*! counter to indicate the number of BCCH_IND data drops */
  uint32  bcch_data_drop_cnt;
  /*! counter to indicate the number of PCCH_IND data drops */
  uint32  pcch_data_drop_cnt;
  /*! counter to indicate the number of PCCH_IND data drops */
  uint32  ccch_data_drop_cnt;
  /*! eMBMS specific statistics structure */
  lmac_qsh_dump_dl_embms_stats_s embms;
  /*! DSDS Num of RLC PDU enqueue operations*/
  uint32  num_rlc_pdu_enq;
  /*! Number of timing advances received*/
  uint32  num_ta_rcvd;
  /*! Number of total valid payload bytes received on the DL*/
  uint64  total_bytes;
  /*! Number of padding bytes received on the DL*/
  uint64  padding_bytes;
} lmac_qsh_dump_dl_data_stats_s;

/*! @brief Global Data structure for MAC DL task */
typedef PACK(struct)
{
  uint8                     state;                 /*!< MAC DL state */
  /*! This is a bitmask to decide what data indication to send to RLC
   * 0x0 -> Do nothing
   * 0x1 -> Send LMAC_RLC_DL_DATA_IND
   * 0x2 -> Send LMAC_BCAST_DATA_IND
   * 0x3 Send LMAC_DUAL_DATA_IND
   */
  uint8                    rlc_ind_bm;
  uint8                    mac_hdr[LMAC_QSH_DUMP_MAC_HDR_MAX_LEN]; /*!< Leading portion of MAC header of the MAC PDU */
  uint16                   last_sfn;                                   /*!< sfn with the last DL TB */
  uint16				   ta_timer_val;		   /*!< Timing Advance timer expiry value */
  boolean				   ta_sync;				/*!< TRUE if Uplink is time-synced */
  lmac_qsh_dump_dl_data_stats_s	stats;		 /*!< statistic counters */
} lmac_qsh_dump_dl_data_s;

/*! @brief Global structure for MAC DL task*/
typedef PACK(struct)
{
  /*! QSH dump tag harder, this header will have a tag and length */
  qsh_dump_tag_hdr_s    hdr; 
  lmac_qsh_dump_dl_task_s   task;        /*!< all the task control related data */
  lmac_qsh_dump_dl_data_s   data;        /*!< all the local data structure */
} lmac_qsh_dump_tag_dl_s;

/*! @brief MAC UL FC stats data structure definition */
typedef PACK(struct)
{
  /*! Number of CFM_CMD_DOWN triggered*/
  uint32  num_fc_down;
  /*! Number of CFM_CMD_UP triggered*/
  uint32  num_fc_up;
  /*! Number of CFM_CMD_FREEZE triggered*/
  uint32  num_fc_freeze;
  /*! Number of CFM_CMD_SHUT_DOWN triggered*/
  uint32  num_fc_shutdown;
} lmac_qsh_dump_fc_stats_s;

/*! @brief flow control configuration */
typedef PACK(struct)
{
  uint8           num_state;             /*!< number of states*/
  uint8           default_state;         /*!< default state when fc triggers */  
} lte_fc_qsh_dump_cfg_s;

/*! @brief CPU based flow control parameters */
typedef PACK(struct)
{
  uint32  default_rate;  /*!< target data rate when entering CPU based FC */
  uint32  min_rate;      /*!< minimum data rate for CPU based FC */
  uint32  step_timer;    /*!< step timer in ms for CPU based FC */
} lte_fc_qsh_dump_cpu_fc_cfg_s;

/*! @brief BW based flow control parameters */
typedef PACK(struct)
{
  uint32  default_rate;  /*!< target data rate when entering CPU based FC */
  uint32  min_rate;      /*!< minimum data rate for CPU based FC */
  uint32  step_timer;    /*!< step timer in ms for CPU based FC */
} lte_fc_qsh_dump_bw_fc_cfg_s;

/*! @brief MAC FC local data structure definition */
typedef PACK(struct)
{
  /*! QSH dump tag harder, this header will have a tag and length */
  qsh_dump_tag_hdr_s    hdr; 
  /*! FC state */
  uint8 state;
  /*! current state level*/
  uint8     state_level;
  /*! the lowest state level mapped to 
  the lowest target data rate*/
  uint8     state_level_lowest;
  /*! CPU FC default rate level */
  uint8     cpu_fc_default_level;
  /*! CPU FC min rate level */
  uint8     cpu_fc_min_level;
  /*! BW FC default rate level */
  uint8     bw_fc_default_level;
  /*! BW FC min rate level */
  uint8     bw_fc_min_level;
  /*! time counter for step timer */
  uint32  timer_cnt;
  /*! step timer */
  uint32  step_timer;
  /*! Local statistics*/
  lmac_qsh_dump_fc_stats_s stats;
  /*! FC UL configuration reads out from EFS */
  lte_fc_qsh_dump_cfg_s  ul_cfg;
  /*! CPU FC configuration reads out from EFS */
  lte_fc_qsh_dump_cpu_fc_cfg_s  cpu_cfg;
  /*! BW FC configuration reads out from EFS */
  lte_fc_qsh_dump_bw_fc_cfg_s  bw_cfg;
} lmac_qsh_dump_tag_fc_s;

/*! @brief Data structure for QoS statistics */
typedef PACK(struct)
{
  uint32 num_retxed_bsr;
}lmac_qsh_dump_qos_stats_s;

/*! VoLTE LCID cfg backup structure type */
typedef PACK(struct)
{
  uint8        priority;  /*!< Priority Level */
  uint8        lcgroup;   /*!< LC group */
  uint8        lcid;      /*!< LC id */
} lmac_qsh_dump_volte_lcid_cfg_s;


/*! @brief MAC UL FC packet build data structure definition */
typedef PACK(struct)
{
  /*! Flag to indicate if UL FC is enable*/
  boolean enable;
  /*! tick count in number of ITI since the last 
  new target data rate*/
  uint32  tick_cnt;
  /*! target data rate in number of byte per TTI or ms*/
  uint32  target_rate;
  /*! Initial BSR buffer size based on the target data rate*/
  uint32  initial_bs;
} lmac_qsh_dump_fc_ul_pkt_s;

/*! @brief QoS feedback Information Structure from 
MAC Up Link Task to MAC Control Task. This will help the MAC Control
to update it BSR logic and QoS/Token bucket logic */
typedef PACK(struct)
{
  uint8   total_act_lc;       /*!< Total number of active LC in lc_data array */
  uint8   bsr_triggered;      /*!< indicate the last BSR triggered*/
  uint16  padding_byte;       /*!< padding byte */
  uint16  grant;              /*!< grant in byte */
  uint16  sys_fn;             /*!< System Frame number */
  uint8   sub_fn;             /*!< Subframe number */
  uint32  rlc_used_size;      /*!< total RLC used size */
  boolean send_phr_failed;    /*!< Don't have enough grant to include PHR */
  /*!UL FC is sending zero BSR now, stop reloading
  the rexted timer so that the timer will tick until
  it is expired for the next starting FC ON/OFF period*/
  boolean ul_fc_zero_bsr_enable;
  uint32  highest_nz_lcid;    /*!< Highest priority LCID which is not empty */
  uint32  sr_send_cnt_echo;   /*!< Echo of the SR req counter sent by QoS */
  uint8   bsr_event;          /*!< BSR Event */
} lmac_qsh_dump_qos_feedback_info_s;

/*! @brief QoS Preprocessing Information Structure from 
MAC Control Task to MAC Up Link Task. As far as the BSR report and QoS 
Allotment, this is all the informations that MAC Up Link Task needs */
typedef PACK(struct)
{
  uint8   last_none_zero_bs_index;                                   /*!< last none zero buffer size index*/
  uint32  bsr_group_buf_size[LMAC_QSH_DUMP_BSR_LCGROUP_MAX];     /*!< BS reports*/
  uint8   lcgroup_priority_level[LMAC_QSH_DUMP_BSR_LCGROUP_MAX]; /*!< LCG priority level*/
  uint16  bsr_instance_number;                                       /*!< indicate the current request BSR instance number*/
  lmac_qsh_dump_fc_ul_pkt_s    fc_ul;                            /*!< Flow Control information for UL*/
} lmac_qsh_dump_qos_prep_info_s;

/*! @brief Data structure for QoS logic */
typedef PACK(struct)
{
  uint8   bsr_triggered;               /*!< current BSR triggered */
  uint8   last_bsr_event;              /*!< last BSR event */
  uint16  retxed_bsr_timer_count;      /*!< retxed BSR timer counter in ms*/
  uint8   sr_request_pending;          /*!< SR request pending state to L1*/
  uint16  request_bsr_instance_number; /*!< indicate the request BSR instance number*/
  uint8   last_high_data_arrival_pri;  /*!< last high data arrival priority */
  uint8   total_active_lc;             /*!< total active LCID inthe sorted_active_lc*/
  uint8   ul_highest_nz_prio;          /*!< Indicate that a HDA event is needed to due buffer dip */
  lmac_qsh_dump_qos_prep_info_s  curr_prep_info; /*!< working */
  uint8   last_qos_event_bitmasks;     /*!< last QoS event bit masks*/
  boolean fc_bsr_trigger;              /*!< flag to indicate if the FC BSR is triggered*/
  uint16  fc_bak_rbsr_timer;           /*!< Backup Flow Control Robustness BSR timer */
  /*! define default value of retxed BSR interval. For FC, this value will
  be used. It is a compromise between maximizing SR internal and stastifying
  TCP windows */
  uint16  lmac_qos_fc_integration_peroid;
  uint8   delay_ul_rach_duration;      /*!< delay UL RACH request in ms */
  uint32  total_lc_buffer_size;        /*!< total buffer size from all the active LCIDs */
  uint32  sr_send_cnt;                 /*!< Count of SR requests to ML1;to sync QoS and UL */
  /*! Flag to indicate that MAC is already working on an external high data
     arrival SR trigger from PDCP */
  boolean ext_hdr_trig_pending;
  /*! BSR event tracker for external high data arrival */
  uint8   ext_hdr_bsr_event_type; 
  /*! VoLTE LCID configuration backup */
  lmac_qsh_dump_volte_lcid_cfg_s volte_cfg_bkp;
  /*! snapshot of dl task dequeue count */
  uint32  dl_task_num_rlc_enq;
  /*! monitor traffic activity */
  uint32  traffic_monitor_timer;
  /*! Traffic monitor active */
  boolean traffic_monitor_active;
  /*! Traffic inactivity timer - Time before inactivity logic kicks in*/
  uint32  traffic_inactivity_timer;
  /*! SFN Known flag. Set to FALSE when handover starts and later 
  to TRUE once PBCH is decoded. Default is TRUE */
  boolean sfn_known;
  /*! Number of failed SR */
  uint32  num_sr_req_fail;
  /*! last feedback info */
  lmac_qsh_dump_qos_feedback_info_s  last_feedback_info;
} lmac_qsh_dump_qos_data_s;

/*! @brief Configuration using in QoS logic/processing */
typedef PACK(struct)
{
  boolean  sr_resource_present;    /*!< indicated if SR recourse is available or not*/
  uint8    ul_rach_delay_duration; /*!< brief defined UL RACH delay in ms */
  uint16   sr_periodicity;         /*!< Periodicity of triggering SR in ms*/
  uint16   retxed_bsr_timer;       /*!< retxed BSR periodicity timer in ms */
} lmac_qsh_dump_qos_cfg_s;

/*! @brief QoS local data structure definition*/
typedef PACK(struct)
{
  /*! QSH dump tag harder, this header will have a tag and length */
  qsh_dump_tag_hdr_s    hdr; 
  /*! QoS configuration info */
  lmac_qsh_dump_qos_cfg_s            cfg;
  /*! QoS operated data */
  lmac_qsh_dump_qos_data_s           data;
  /*! QoS stats*/
  lmac_qsh_dump_qos_stats_s          stats;
} lmac_qsh_dump_tag_qos_s;

/*! @brief Global structure for RACH statistics*/
typedef PACK(struct)
{
  uint8    num_preambles_sent;  /*!< number of preambles sent */
  uint8    num_contentions;     /*!< number of contention */
  uint32   num_rach_sent;       /*!< Total number of rach preambles including retry sent */
  uint32   num_rach_retry_sent; /*!< Number of retry rach preambles */
  uint32   num_rach_fails_rep;  /*!< Number of rach failure indications sent to ML1 */
} lmac_qsh_dump_rach_stats_s;


/*! @brief Global structure for RACH procedure */
typedef PACK(struct)
{
  /*! QSH dump tag harder, this header will have a tag and length */
  qsh_dump_tag_hdr_s    hdr; 
  /*! internal access procedure state */
  uint8    state;
  /*! access reason */
  uint8    access_reason;
  /*! current access attempt number */
  uint16   attempt_number;
  /*! RACH sequence number, increase by one when the 
  attempt number reach the maximum */
  uint8    sequence_number;
  /*! contention based or contention free */
  boolean  contention_based_rach;
  /*! Current used RA index, it can be computed 
  or from configured RA index */
  uint8    current_raid;
  /*! contention free RA index. This is configured */
  uint8    contention_free_raid;
  /*! actual lenght of MSG3 */
  uint16   msg3_length;
  /*! in HO RACH case, MSG3 needs to store for 
  subsequence usage */
  boolean  request_duplication_msg3;
  /*! current power level in dB */
  int16    power_level;
  /* RA group, 0 or 1 */
  uint8    ra_group;
  /*! backoff duration in ms from MSG2*/
  uint16   backoff_duration;
  /*! current backoff duration in ms. Randomize value between 0->backoff_duration */
  uint16   curr_backoff_duration;
  /*! TEMP RNTI  received in RAR response */
  uint16   temp_rnti;
  /*! the UE C-RNTI */
  uint16   c_rnti;   
  /*! RACH procedure status */
  uint8    rach_status;
  /*! estimated MAC PDU length */
  uint16   estimated_macpdu_length;
  /*! Grant size is sufficient or not */
  boolean  invalid_grant;   
  /*! Rach statistics info */
  lmac_qsh_dump_rach_stats_s stats;
} lmac_qsh_dump_tag_rach_s;

/*! @brief Global CS structure for MAC UL task */
typedef PACK(struct)
{
  boolean        task_is_active; /*!< Whether the "main" function of the thread is running */
  int            msgr_client;    /*!< MSGR client */
  int            mb_id;          /*!< Mailbox Id */
} lmac_qsh_dump_ul_cs_s;

/*! @brief Global Data structure for MAC UL task */
typedef PACK(struct)
{
  uint8    prev_state;          /*!< previous state */
  uint8    curr_state;          /*!< current state */
  uint8    pending_curr_state;  /*!< pending current state */
} lmac_qsh_dump_ul_data_s;

/*! @brief Global Stats structure for MAC UL task*/
typedef PACK(struct)
{
  /*! number of tuneaways received*/
  uint32 num_rfta_rcvd;         
  /*! Total tuneaway time, does not protect against 
   tuneaways greater than 10240 milliseconds*/
  uint32 total_rfta_time;    
} lmac_qsh_dump_ul_stats_s;

/*! @brief Global structure for MAC UL task*/
typedef PACK(struct)
{
  /*! QSH dump tag harder, this header will have a tag and length */
  qsh_dump_tag_hdr_s    hdr; 
  lmac_qsh_dump_ul_cs_s     cs;      /*!< all the task control related data */
  lmac_qsh_dump_ul_data_s   data;    /*!< all the local data structure */
  lmac_qsh_dump_ul_stats_s  stats;   /*!< all the local stats structure*/
} lmac_qsh_dump_tag_ul_s;

/*! @brief RLC to MAC actual Logical channel PDU build info */
typedef PACK(struct)
{
  uint16               used_size;           /*!< actual used size */
  uint16               total_mac_sdu_size;  /*!< actual mac sdu size*/
  uint32               remaining_size;      /*!< logical channel buffer status after pdu build */
  uint16               num_sdu_built;       /*!< total number of MAC SDU built */
} lmac_qsh_dump_ul_data_info_s;

/*! @brief MAC UL data internal statistics structure */
typedef PACK(struct) 
{
  uint32 num_lcg_bsr[LMAC_QSH_DUMP_BSR_LCGROUP_MAX];        /*!< Number of times BSR is sent for LCG 0 to 3*/
  uint32 total_bsr_sent_lcg[LMAC_QSH_DUMP_BSR_LCGROUP_MAX]; /*!< Total accummulated BSR value for LCG 0 to 3*/
} lmac_qsh_dump_ul_data_bsr_stats_s;


/*! @brief MAC UL data internal statistics structure */
typedef PACK(struct) 
{
  uint32 grant_size_change_rach_cnt;     /*!< Counter to indicate number of times grant size is altered during RACH*/
  uint32 no_qos_prep_cnt;                /*!< qos prep count */
  uint32 no_grant_for_crnti_cnt;         /*!< grant for crnti count */
  uint32 no_a2_reserved_task_cnt;        /*!< a2 reserved task count */
  uint32 total_pkt_build_ind_cnt;        /*!< Number of new MAC PDUs built*/
  uint32 total_retx_pkt_build_ind_cnt;   /*!< Number of retx MAC PDUs built*/
  uint64 total_grant_received_bytes;     /*!< Total grant received in bytes*/
  uint64 total_padding_bytes;            /*!< Total padding sent in bytes including the header*/
  lmac_qsh_dump_ul_data_bsr_stats_s bsr_stats; /*!< Stats used to calculcate avg BSR*/
  uint32 no_phr_sent;                    /*!< Number of PHR sent on the uplink*/
  uint32 total_phr_sent;                 /*!< Accumulated value of the PHR sent so far*/
} lmac_qsh_dump_ul_data_stats_s;

/*! @brief SPS related data structure */
typedef PACK(struct)
{
  /*!< number of emptied MAC SDU to implicit SPS release  */
  uint16  sps_num_tx_release;
  /*!< current counter of SPS release */
  uint16  sps_num_tx_release_cnt;
  /*!< sps enable / disable flag */
  boolean is_sps_enabled;
} lmac_qsh_dump_ul_data_sps_s;


/*! @brief Global structure for MAC UL Data processing*/
typedef PACK(struct)
{
  /*! QSH dump tag harder, this header will have a tag and length */
  qsh_dump_tag_hdr_s    hdr; 
  /*! current BSR triggered */
  uint8           bsr_triggered;
  /*! BSR periodicity timer config in ms */
  uint16          bsr_timer_cfg;
  /*! BSR periodicity timer in ms */
  uint16          bsr_timer_cur_use_cfg;
  /*! last none zero LCG*/
  uint8           last_none_zero_lcg;
  /*! current QoS Prep Info using in packet build*/
  lmac_qsh_dump_qos_prep_info_s  curr_qos_prep_info;
  /*! flag to indicate if UL need to force a BSR */
  boolean         forced_bsr;
  /*! total used size for UL Flow Control*/
  uint32          fc_total_used_size; 
  /*! indicate the UL FC BSR is zero now so 
 all the packet should be padding now */
  boolean         ul_fc_zero_bsr_enable;
  /*! last sent power headroom */
  uint8           sent_power_headroom;
  /*! Statistics structure */
  lmac_qsh_dump_ul_data_stats_s stats;
  /*! SPS related data */
  lmac_qsh_dump_ul_data_sps_s sps_data;
  /*! current HARQ ID */
  uint8           harq_id;
  /*! sfn of the last packet build */
  uint16          last_sfn;
  /*! Leading portion of MAC header of the MAC PDU */
  uint8           mac_hdr[LMAC_QSH_DUMP_MAC_HDR_MAX_LEN]; 
  /*! Last TB grant */
  uint16 last_grant_size;
  /*! Boolean to indicate whether extended bsr is used */  
  boolean extended_bsr;
  /*! Boolean to indicate whether gps padding is enabled or not */
  boolean padding_for_gps_is_required;
} lmac_qsh_dump_tag_ul_data_int_s;

/*! @brief rach related qsh metrics */
typedef struct
{
  /*! QSH metric header */
  qsh_metric_hdr_s hdr; 
  /*! Total accumulated number of access attempt */
  uint32  num_rach_attempt;
  /*! Accumulated number of access failure */
  uint32  num_rach_fails;
} lmac_qsh_metric_rach_info_s;

/*! @brief dl throuput related qsh metrics */
typedef struct
{
  /*! QSH metric header */
  qsh_metric_hdr_s hdr; 
  /*! Accumulated number of DL bytes */
  uint32	total_bytes;
} lmac_qsh_metric_dl_info_s;

/*! @brief all the dump collection data for test cases */
typedef struct
{
  qsh_dump_hdr_s hdr;
  lmac_qsh_dump_tag_ctrl_s ctrl_dump;
  lmac_qsh_dump_tag_fc_s fc_dump;
  lmac_qsh_dump_tag_qos_s qos_dump;
  lmac_qsh_dump_tag_rach_s rach_dump;
  lmac_qsh_dump_tag_ul_s ul_dump;
  lmac_qsh_dump_tag_ul_data_int_s ul_data_dump;
  lmac_qsh_dump_tag_dl_s dl_dump;
}lmac_qsh_tc_dump_collect_s;

#endif /* LMAC_QSH_EXT_H */
