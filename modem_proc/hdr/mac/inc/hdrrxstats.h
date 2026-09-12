#ifndef HDRRXSTATS_H
#define HDRRXSTATS_H
/** @file hdrrxstats.h
    @brief This module contains external declarations for HDR Receive task
    statistics processing. */

/*===========================================================================
  Copyright (c) 2000 - 2010 Qualcomm Technologies Incorporated. All Rights Reserved

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
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/hdr.mpss/6.0/mac/inc/hdrrxstats.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when         who     what, where, why
----------   ---     -------------------------------------------------------
06/07/2010   kss     Added hdrrxstats_get_connected_state_per_data().
08/17/2009   kss     Added hdrrx_get_fl_slot_count() support.
07/29/2009   etv     Added support for connected state PER.
05/21/2009   etv     Moved Decoder header logging to HDRDEC module.
04/22/2009   etv     Adopted Doxygen commenting style.
02/26/2009   etv     Floor RxAGC1 during diversity tune away.
01/12/2009   etv     Fixed MC FL Packet header buffer overflow issue.
12/04/2008   etv     Lint fixes.
10/06/2008   etv     Logged power log pkt even during SOODA but floored rxagc.
03/31/2008   etv     Cleaned up FL Physical layer packet logging code.
01/14/2008   etv     Refactored Air link Summary log packet for Artemis.
12/19/2007   etv     Added support for per-carrier Instantaneous PER stats.
11/01/2007   etv     Added support for MC Air link summary log packet.
10/31/2007   etv     Fixed merge error for compilation.
10/19/2007   kss     Merge code to log dec hdr log pkt immediately in idle.
08/14/2007   sb      Added support for multi-carrier decoder header log packet.
07/11/2007   etv     Created module.

===========================================================================*/

/**
 *  @defgroup hdrrxstats HDR Receive Task Statistics Module */
/*\{*/

/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/
#include "hdr_variation.h"
#include "dsm.h"

#include "hdrlogi.h"
#include "hdrdec.h"

/*===========================================================================

                            DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                          FUNCTION DECLARATIONS

===========================================================================*/

/*============================================================================
  HDRRXSTATS_INIT
*/
/**
  This procedure initializes the statistics and the periodic logging timer on 
  powerup.

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_init ( void );

/*============================================================================
  HDRRXSTATS_ENTRY_PROCESSING
*/
/**
  This function performs Rx task entry processing related to statistics when 
  entering HDR mode.

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_entry_processing ( void );

/*============================================================================
  HDRRXSTATS_EXIT_PROCESSING
*/
/**
  This function performs exit processing when exiting HDR mode.

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_exit_processing ( void );

/*============================================================================
  HDRRXSTATS_LOG_RX_DATA
*/
/**
  This function performs Rx logging that occurs on each frame (PN roll).

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_log_rx_data
( 
  boolean agc0_is_valid, 
    /**< Indicates whether RxAGC on primary chain is valid or not */
  boolean agc1_is_valid 
    /**< Indicates whether RxAGC on diversity chain is valid or not */
);

/*============================================================================
  HDRRXSTATS_RESET_DM_FWD_STATS
*/
/**
  This function resets all RX rate statistics and sends a log packet to the DM.

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_reset_dm_fwd_stats ( void );


/*============================================================================
  HDRRXSTATS_LOG_FL_PHY_LAYER_PKT
*/
/**
  This function logs the forward link physical packets received from the 
  decoder before sending it to upper layers. Each log packet contains exactly 
  one physical layer packet.

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_log_fl_phy_layer_pkt
(
  dsm_item_type *fl_phy_pkt_ptr
    /**< DSM item containing the forward link physical layer packet to be 
         logged */
);

/* <EJECT> */
#ifdef FEATURE_HDR_BCMCS 
/*============================================================================
  HDRRXSTATS_CLEAR_BCC_STATS
*/
/**
  This function resets the broadcast channel statistics.
  - BCC early decode and
  - BCC rate CRC count

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_clear_bcc_stats ( void );

#endif /* FEATURE_HDR_BCMCS */

/*============================================================================
  HDRRXSTATS_UPDATE_GOOD_CC_PKT_STATS
*/
/**
  This function is used to update good Control channel packet statistics

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_update_good_cc_pkt_stats
(
  hdrdec_pkt_info_s_type *pkt_info,
    /**< Pointer to decoder packet header information */
  uint8 pkt_size_index
    /**< Packet size index */
);

/*============================================================================
  HDRRXSTATS_UPDATE_BAD_CC_PKT_STATS
*/
/**
  This function is used to update bad Control channel packet statistics

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_update_bad_cc_pkt_stats
(
  hdrdec_pkt_info_s_type *pkt_info,
    /**< Pointer to decoder packet header information */
  uint8 dummy
    /**< Dummy variable to keep interface consistent */
);

/*============================================================================
  HDRRXSTATS_UPDATE_GOOD_TC_PKT_STATS
*/
/**
  This function is used to update good traffic channel packet statistics

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_update_good_tc_pkt_stats
(
  hdrdec_pkt_info_s_type *pkt_info,
    /**< Pointer to decoder packet header information */
  uint8 pkt_size_index
    /**< Packet size index */
);

/*============================================================================
  HDRRXSTATS_UPDATE_BAD_TC_PKT_STATS
*/
/**
  This function is used to update bad traffic channel packet statistics

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_update_bad_tc_pkt_stats
(
  hdrdec_pkt_info_s_type *pkt_info,
    /**< POinter to decoder packet header information */
  uint8 pkt_size_index
    /**< Packet size index */
);

#ifdef FEATURE_HDR_BCMCS
/*============================================================================
  HDRRXSTATS_UPDATE_GOOD_BCC_PKT_STATS
*/
/**
  This function is used to update good broadcast channel packet statistics

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_update_good_bcc_pkt_stats
(
  hdrdec_pkt_info_s_type *pkt_info,
    /**< Pointer to decoder packet header information */
  uint8 dummy
    /**< Dummy variable to keep the interface consistent */
);

/*============================================================================
  HDRRXSTATS_UPDATE_BAD_BCC_PKT_STATS
*/
/**
  This function is used to update bad broadcast channel packet statistics

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_update_bad_bcc_pkt_stats
(
  hdrdec_pkt_info_s_type *pkt_info,
    /**< Pointer to decoder packet header information */
  uint8 dummy
    /**< Dummy variable to keep the interface consistent */
);

#endif /* FEATURE_HDR_BCMCS */

/*============================================================================
  HDRRXSTATS_UPDATE_PER_STATS
*/
/**
  This function is used to update Packet Error Rate Statistics

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_update_per_stats 
(
  uint8 demod_id
    /**< Demod carrier on which the packet was received */
);

/*============================================================================
  HDRRXSTATS_GET_AVERAGE_PER
*/
/**
  Function to access average packet error rate.  The supplied structure
  is filled in with the measured sample size and the number of those packets
  that were in error.

  @context HDRRX task. */
/*============================================================================*/
extern void hdrrxstats_get_average_per
(
  uint16 *total,
    /**< Total number of packets */
  uint16 *errors
    /**< Number of bad packets */
);

/*============================================================================
  HDRRXSTATS_UPDATE_CC_STATUS
*/
/**
  This function is used the record the CC status of the last control channel
  packet received based on the CRC status.

  @context HDRRX task. */
/*============================================================================*/
extern void  hdrrxstats_update_cc_status
(
  hdrlog_cc_status_enum_type cc_crc_status
    /**< Control Channel packet CRC status. Could be one of the following 
         - HDRLOG_CC_GOOD_CRC (CRC pass) 
         - HDRLOG_CC_PREAMBLE_LOST (Preamble detection failed) 
         - HDRLOG_CC_BAD_CRC (CRC fails) 
         - HDRLOG_CC_FALSE_CRC (False CRC pass) 
         - HDRLOG_CC_REACQ_FAIL (Re acquisition failed) 
         - HDRLOG_CC_SUBSYNC_PREAMBLE_LOST (Subsync Preamble Detection failed) 
         - HDRLOG_CC_NULL (Initial NULL state) */
);


/*============================================================================
  HDRRXSTATS_GET_CC_STATUS
*/
/**
  This function returns the last control channel packet status ( CRC pass,
  CRC failure, False CRC pass, Missed preamble )

  @return Control channel packet CRC status. Could be one of the following.
    - HDRLOG_CC_GOOD_CRC (CRC pass) 
    - HDRLOG_CC_PREAMBLE_LOST (Preamble detection failed) 
    - HDRLOG_CC_BAD_CRC (CRC fails) 
    - HDRLOG_CC_FALSE_CRC (False CRC pass) 
    - HDRLOG_CC_REACQ_FAIL (Re acquisition failed) 
    - HDRLOG_CC_SUBSYNC_PREAMBLE_LOST (Subsync Preamble Detection failed) 
    - HDRLOG_CC_NULL (Initial NULL state)

  @context HDRRX task. */
/*============================================================================*/
extern hdrlog_cc_status_enum_type hdrrxstats_get_cc_status ( void );

/*============================================================================
  HDRRXSTATS_GATHER_MC_LINK_SUMMARY_DATA
*/
/**
  This function samples link summary data.  The provided structure is
  filled in with the required data.

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_gather_mc_link_summary_data
(
  LOG_HDR_MC_LINK_SUMMARY_C_type *link_summary,
    /**< Pointer to link summary struct to be filled in for logging */
  uint8 carrier_cnt,
    /**< Number of demod carriers */
  uint8 demod_idx 
    /**< Index of demod carrier being logged */
);


/*============================================================================
  HDRRXSTATS_GATHER_LINK_SUMMARY_DATA
*/
/**
  This function samples link summary data.  The provided structure is
  filled in with the required data.

  @context HDRRX task. 

  @sideeffects: This function disables premption briefly to avoid race condition 
  that could arise from reading some variables updated in the HDR decoder task 
  context as well. */
/*============================================================================*/
extern void hdrrxstats_gather_link_summary_data
(
  hdrlog_link_summary_type *link_summary
    /**< Structure to fill in for logging */
);

/*============================================================================
  HDRRXSTATS_RESET_CONNECTED_STATE_PER
*/
/**
  Reset packet error rate statistics maintained for recent traffic session.

  @context Calling task. 

  @sideeffects None */
/*============================================================================*/

extern void hdrrxstats_reset_connected_state_per ( void );

/*============================================================================
  HDRRXSTATS_GET_CONNECTED_STATE_PER
*/
/**
  Return packet error rate observed during current or recent traffic session.

  Upper layers expect support for PERs in range 0.01% to 8.0%. In order
  to support this range, PER is scaled up by 10000, so that the resulting 
  PER would be an integer for easy representation and also support the 
  expected range

  @context Calling task. 

  @return Connected State Packet error rate scaled up by 10000

  @sideeffects None */
/*============================================================================*/

extern uint16 hdrrxstats_get_connected_state_per ( void );

/*============================================================================
  HDRRXSTATS_GET_CONNECTED_STATE_PER_DATA
*/
/**
  Return packet counts for bad and total traffic packets. Control channel
  is not counted.

  total_tc_packets and total_tc_errors must point to valid uint32 
    variable locations to be filled in.

  @context Calling task. 

  @return  Bad and total packet counts are filled in. 

  @sideeffects None */
/*============================================================================*/

extern void hdrrxstats_get_connected_state_per_data
(
  uint32 * total_tc_packets, 
    /**< Total traffic channel packets received. */

  uint32 * total_tc_errors
    /**< Traffic channel packets received with errors (failed CRC). */
);


/*============================================================================
  HDRRXSTATS_GET_FL_SLOT_COUNT
*/
/**
  Returns the number of slots occupied by packet directed at this AT. All 
  TC packets (Rev 0, Rev A/B SU and MU), good or bad, contribute to this 
  count.

  @return Number of slots occupied by packets directed at this AT since the last 
  time the counter was cleared.

  @context Calling task. 

  @sideeffects Count is cleared if reset is TRUE */
/*============================================================================*/

extern uint32 hdrrxstats_get_fl_slot_count
(
  boolean reset
    /**< Count is reset if TRUE */
);

/*\}*/

#endif /* HDRRXSTATS_H */
