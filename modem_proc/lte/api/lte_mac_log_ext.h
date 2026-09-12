/*!
  @file
  lte_mac_log_ext.c

  @brief
   This header file contains the various defines, enums, and other data
  structure types which are necessary for logging binary packets via DIAG.

  @author
  tahmed

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

$Header:

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/08/16   el      CR992330: UTC time interface and logging fix
03/03/16   el      FR 28315 - MAC layer implementation
05/11/15   el      CR806429: FR 20286 LTE_UL_CA MAC log and event packet
10/18/13   ta      CR 553489: initial version

===========================================================================*/
#ifndef LTE_MAC_LOG_EXT_H
#define LTE_MAC_LOG_EXT_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include <comdef.h>
#include <lte_cphy_msg.h>
#include <lte_mac.h>
#include <lte_mac_prose_api.h>
#include "lte_as.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/
/*! @brief
    Length of UE ID in log packets
*/
#define LTE_MAC_LOG_UE_ID_LEN 6

/*! @brief
    Maximum length of RACH MSG3 MAC header in log packets
*/
#define LTE_MAC_LOG_MSG3_RACH_MAC_HDR_MAX_LEN 10

/*! @brief 
    Maximum number of filter we can report per match report
*/
#define LTE_MAC_LOG_MAX_NUM_FILTER_PER_MATCH    LTE_MAC_MAX_NUM_FILTER_PER_MATCH_RPT

#ifdef _WIN32
#pragma pack(push,1) // Save previous, and turn on 1 byte alignment
#endif

/*! @brief Structure for Config Type Log sub packet "scratchpad"
    PACKET LOG_CODE 0xB060, Version 1
    SUBPACKET ID = 0x00, Version 2
*/
typedef PACK(struct)
{
  uint8              sub_id;  /*!< subscribe ID */
  uint8              cfg_reason;  /*!< NORMAL, HO, RELEASE */
  uint8              cfg_bitmask; /*!< DL, UL, RACH, LC INFO Cfg */
} lte_mac_log_sub_pkt_cfg_type_s;

/*! @brief Structure for STAG and TA timer Info in DL Config Log sub packet "scratchpad"
*/
typedef PACK(struct)
{
  uint8               tag_id;  /*!< Secondary Timing Advance Group ID */
  uint8               cell_id_mask;  /*!< scell info belongs to this stag ID */
  uint8               ta_timer_present;  /*!< If TA timer is configured for this Stag ID */
  uint16  ta_timer; /*!< TA timer value. Note that 0xFFFF is invalid timer value*/
} lte_mac_log_sub_pkt_dl_cfg_info_s;

/*! @brief Structure for DL Config Log sub packet "scratchpad"
    PACKET LOG_CODE 0xB060, Version 1
    SUBPACKET ID = 0x01, Version 2
*/
typedef PACK(struct)
{
  uint8               sub_id;      /*!< subscribe ID */ 
  uint8               num_active_tag; /*!< number of active tag */  
  lte_mac_log_sub_pkt_dl_cfg_info_s    cell_tag_info[LTE_CPHY_MAX_NUM_STAG+1];  /*!< cell tag info, the first element is always reserved for ptag */
} lte_mac_log_sub_pkt_dl_cfg_s;

/*! @brief Structure for UL Config Log sub packet "scratchpad"
    PACKET LOG_CODE 0xB060, Version 1
    SUBPACKET ID = 0x02, Version 2
*/
typedef PACK(struct)
{
  uint8    sub_id;                  /*!< subscribe ID */
  uint8    sr_resource_present;     /*!< indicated if SR recourse is available or not*/
  uint16   sr_periodicity;          /*!< Periodicity of triggering SR in ms */
  uint16   bsr_timer;               /*!< SR periodicity timer in ms. 0xFFFF is invalid number */
  uint16   sps_num_tx_release;      /*!< Number of empty txed before release timer SPS */
  uint16   retxed_bsr_timer;        /*!< retxed BSR timer in ms */
} lte_mac_log_sub_pkt_ul_cfg_s;

/*! @brief Structure for UL Config Log sub packet "scratchpad"
    PACKET LOG_CODE 0xB060, Version 1
    SUBPACKET ID = 0x12, Version 1
*/
typedef PACK(struct)
{
  uint8    sub_id;                  /*!< subscribe ID */
  uint8    els_ul_lc_id;            /*!< els ul logic channel id */
  uint8    els_dl_lc_id;            /*!< els dl logic channel id  */
  uint8    els_mce_retx_tmr_length; /*!< NW configured retransmit timer length for ELS MCE */
} lte_mac_log_sub_pkt_els_cfg_s;


/*! @brief Structure for RACH Config for each cell
*/
typedef PACK(struct)
{
  uint8               scell_id;                      /*!< Secondary cell ID, only applicable for scell */
  int16               preamble_init_power;           /*!< Initial Power Level in dB */
  uint8               power_ramping_step;            /*!< Step power */
  uint8               ra_index1;                     /*!< ra_index1 = sizeOfRA-PreamblesGroupA. GroupA is from 1 to ra_index1 */
  uint8               ra_index2;                     /*!< ra_index2 = numberOfRA-Preambles. GroupB = (ra_index1 + 1) to ra_index2 */
  uint8               preamble_trans_max;            /*!< MAX preamble attempts */
  uint16              contention_resolution_timer;   /*!< Contention timer in ms */
  uint16              message_size_qroup_a;          /*!< Threshold message size to select which RA group */
  uint8               pwr_offset_group_b;            /*! group B power offset */
  /*! Pmax to be used in the target cell. If absent the UE applies the maximum power according to the UE capability */
  int16               p_max;
  int16               delta_preamble_msg3;           /*!< delta preable message 3 */
  uint8               prach_config;                  /*!< Random Access Preamble timing for preamble format Range: 0-15 */
  uint8               cs_zone_length;                /*!< Cyclic Shifts for various preamble formats */
  uint16              log_root_seq_index;            /*!< Logical Root Zadoff-Chu Sequence number for various preamble formats */
  uint8               prach_freq_offset;             /*!< PRACH Frequency Offset for MSG1 */
  uint8               high_speed_flag;               /*!< Controls grouping of preamble sequences in two sets */
  uint8               max_retx_msg3;                 /*!< Maximum number of HARQ re-transmissions from UE for RACH message3 */
  uint8               ra_rsp_win_size;               /*!< Window size for reception of RACH message2 in ms */
}lte_mac_log_sub_pkt_cell_rach_cfg_s;

/*! @brief Structure for RACH Config Log sub packet "scratchpad"
    PACKET LOG_CODE 0xB061, Version 1
    SUBPACKET ID = 0x03, Version 4
*/
typedef PACK(struct)
{
  uint8                                  sub_id;                                   /*!< subscribe ID */
  uint8                                  num_active_cell;                          /*!< number of active cell */ 
  lte_mac_log_sub_pkt_cell_rach_cfg_s    cell_rach_info[LTE_MAC_MAX_CELL_ID_NUM];  /*!< cell info, the first element is always reserved for pcell */
} lte_mac_log_sub_pkt_rach_cfg_s;

/*! @brief Structure for All RACH Config Log sub packet "scratchpad"
    PACKET LOG_CODE 0xB060, Version 1
    SUBPACKET ID = 0x0E, Version 1
*/
typedef PACK(struct)
{
  uint8                                  sub_id;                                   /*!< subscribe ID */
  uint8                                  valid_cell_cfg_mask;                        /*!< valid cell cfg mask */
  uint8                                  new_cell_cfg_mask;                        /*!< new cell cfg mask */ 
  lte_mac_log_sub_pkt_cell_rach_cfg_s    cell_rach_info[LTE_MAC_MAX_CELL_ID_NUM];  /*!< cell info, the first element is always reserved for pcell*/
} lte_mac_log_sub_pkt_all_rach_cfg_s;


/*! @brief Structure for LC Info in LC Config Log sub packet "scratchpad"
*/
typedef PACK(struct)
{
  uint8               lc_id;              /*!< Logical channel ID of each channel */
  uint16              pbr;                /*!< Priority bite rate in Kbyte/s */
  uint8               priority;           /*!< Priority Level */
  uint8               lc_group;           /*!< BSR reporting LC group */
  uint32              token_bucket_size;  /*!< token bucket max size in bytes */
} lte_mac_log_sub_pkt_lc_cfg_info_s;

/*! @brief Structure for LC Config Log sub packet "scratchpad"
    PACKET LOG_CODE 0xB060, Version 1
    SUBPACKET ID = 0x04, Version 2
*/
typedef PACK(struct)
{
  uint8               sub_id;                                   /*!< subscribe ID */
  uint8               num_deleted_lc;                           /*!< number of LC to be deleted */
  uint8               deleted_lc[LTE_MAX_LC];                   /*!< Deleted LC IDs */
  uint8               num_addmod_lc;                            /*!< number of added or modified logical channel*/
  lte_mac_log_sub_pkt_lc_cfg_info_s    addmod_lc[LTE_MAX_LC];   /*!< Added/Modified LC Info */
} lte_mac_log_sub_pkt_lc_cfg_s;

/*! @brief Structure for eMBMS Info in eMBMS Config Log sub packet "scratchpad"
*/
typedef PACK(struct)
{
  uint8               area_id;          /*!< area id */
  uint8               pmch_id;          /*!< pmch_id */
  uint8               lc_id;            /*!< lc_id */
} lte_mac_log_sub_pkt_embms_cfg_info_s;

/*! @brief Structure for eMBMS Config Log sub packet "scratchpad"
    PACKET LOG_CODE 0xB060, Version 1
    SUBPACKET ID = 0x0D, Version 2
*/
typedef PACK(struct)
{
  uint8               sub_id;                                                   /*!< subscribe ID */
  uint8               num_active_embms_lc;                                      /*!< number of active embms lc */
  lte_mac_log_sub_pkt_embms_cfg_info_s  active_embms_lc_info[LTE_MAX_EMBMS_LC]; /*!< info of all the active embms lc */
} lte_mac_log_sub_pkt_embms_cfg_s;


/*! @brief Structure for RACH reason Log sub packet "scratchpad"
    PACKET LOG_CODE 0xB061, Version 1
    SUBPACKET ID = 0x05, Version 2
*/
typedef PACK(struct)
{
  uint8               sub_id;                /*!< subscribe ID */
  uint8               scell_id;              /*!< Secondary cell ID, only applicable for scell */
  uint8               rach_reason;           /*!< cause due to which RACH was triggered */
  /*! The UE_ID that will be matched in case of CONTENTION_BASED RACH Procedure.  */
  uint8               matching_id[LTE_MAC_LOG_UE_ID_LEN];
  uint8               rach_contention;       /*!< Indicates if this is a contention free or contention based RACH procedure */
  uint8               preamble;              /*!< Preamble chosen in case of contention free */
  uint8               preamble_index_mask;   /*!< Preamble RA index mask in case of contention free*/
  uint8               msg3_size;             /*!< Size of MSG3 based on which the group would be chosen */
  uint8               group_chosen;          /*!< Refers to the group A or group B from which the preamble was picked */
  uint8               radio_condn;           /*!< Gives the value of the pathloss as a result of call to the L1 API */
  uint16              crnti;                 /*!< If the UE is in connected mode then this indicates the CRNTI of the UE */
} lte_mac_log_sub_pkt_rach_reason_s;

/*! @brief Structure for Message 1 in RACH attempt Log sub packet "scratchpad"
*/
typedef PACK(struct)
{
  uint8               preamble_index;           /*!< Index is in the range 0..63 */
  uint8               preamble_index_mask;      /*!< Range: 0-15. 0xFF means invalid.*/
  int16               preamble_power;           /*!< Power level */
} lte_mac_log_rach_mesg1_s;

/*! @brief Structure for Message 2 in RACH attempt Log sub packet "scratchpad"
*/
typedef PACK(struct)
{
  uint16               backoff_val;      /*!< index into the backoff_table */
  uint8                result;           /*!< TRUE indicates that the RAPID received in the DL matched with the one the UE sent. */
  uint16               tcrnti;           /*!< Temp CRNTI received in MSG2  */
  uint16               ta_value;         /*!< Timing Adjustment value received */
} lte_mac_log_rach_mesg2_s;

/*! @brief Structure for Message 3 in RACH attempt Log sub packet "scratchpad"
*/
typedef PACK(struct)
{
  uint32   msg3_grant_raw;  /*!< raw grant data byte for MSG3. From bit0 to bit20*/
  uint16   msg3_grant;      /*!< Grant in bytes */
  uint8    harq_id;         /*!< Harq ID where the data would be transmitted from */
  /*! Maximum of up to 10 bytes of MCE and MAC sub-headers */
  uint8    mac_pdu[LTE_MAC_LOG_MSG3_RACH_MAC_HDR_MAX_LEN];
} lte_mac_log_rach_mesg3_s;

/*! @brief Structure for RACH attempt Log sub packet "scratchpad"
    PACKET LOG_CODE 0xB062, Version 1
    SUBPACKET ID = 0x06, Version 3
*/
typedef PACK(struct)
{
  uint8  sub_id;                 /*!< subscribe ID */
  uint8  scell_id;               /*!< Secondary cell ID, only applicable for scell */
  uint8  retx_counter;           /*!< RACH attempt number */
  uint8  rach_result;            /*!< RACH attempt result*/
  uint8  contention_procedure;   /*!< Indicates if this is a contention free or contention based RACH procedure */
  uint8  rach_msg_bmask;         /*!< Bitmask to indicate which of MSG 1, 2, 3 are actually present*/
  lte_mac_log_rach_mesg1_s  rach_msg1;  /*!< Message 1 */
  lte_mac_log_rach_mesg2_s  rach_msg2;  /*!< Message 2 */
  lte_mac_log_rach_mesg3_s  rach_msg3;  /*!< Message 3 */
} lte_mac_log_sub_pkt_rach_attempt_s;

/*! @brief Structure for UL TX statistics log sub packet "scratchpad"
    PACKET LOG_CODE 0xB067, Version 1
    SUBPACKET ID = 0x0B, Version 2
*/
typedef PACK(struct)
{
  uint8    sub_id;             /*!< subscribe ID */
  uint8    num_samples;        /*!< Number of samples collected */
  uint8    num_padding_bsr;    /*!< Number of times padding BSRs were sent */
  uint8    num_regular_bsr;    /*!< Number of times regular BSRs were sent */
  uint8    num_periodic_bsr;   /*!< Number of times periodic BSRs were sent */
  uint8    num_cancel_bsr;     /*!< Number of times cancel BSRs were happended */
  uint32   grant_received;     /*!< Grant received in bytes */
  uint32   grant_utilized;     /*!< Grant utilized in bytes */
} lte_mac_log_ul_tx_stats_s;

/*! @brief Structure for LTE-D PAC match report log packet,
    below structure is organized as PACK structure already 
    PACKET LOG_CODE 0xB069, Version 1
    SUBPACKET ID = 0x0F, Version 32
*/
typedef struct
{        
  /*! DL PAC code value received, LTE_MAC_PAC_CODE_LEN is 23 bytes */
  uint8           pac[LTE_MAC_PAC_CODE_LEN];  
  /*! number of active handler id in the above array, put here for word 
  alignment purpose with below pac structure */
  uint8           active_handler_id_num; 
  /*! Matched mac_handler_id associated with matched filter PAC */
  uint32          mac_handler_id_arr[LTE_MAC_LOG_MAX_NUM_FILTER_PER_MATCH]; 
  /*! MIC value */
  uint32          mic;
  /*! UTC system time in seconds */
  uint32          utc_time; 
  /*! The frequency of the cell on which the transport block was received */
  uint32          freq : 20;
  /*! time_lsb */
  uint32          time_lsb : 4;
  /*! reserved bits */
  uint32          reserve_bits_1 : 8;
  /*! System timing frame info */
  uint16          sfn;
  /*! reserved bits 2 */
  uint16          reserve_bits_2;
} lte_mac_log_pac_match_rpt_s;

#ifdef _WIN32
#pragma pack(pop) // Revert alignment to what it was previously
#endif

#endif
