/** @mainpage HDR Receive Task Statistics Module
    @file hdrrxstats.c
    @brief This module contains definitions and declarations for HDR Receive 
    task statistics processing */

/*===========================================================================
  Copyright (c) 2007 - 2010 Qualcomm Technologies Incorporated. All Rights Reserved

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

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrrxstats.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when         who     what, where, why
----------   ---     -------------------------------------------------------- 
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
10/28/2014   vke     Changes to not to log RXAGC when HDR is in 
                     traffic suspend state
07/29/2014   tnp     FR22375: Fix Medium compiler warnings in HDR
07/10/2014   sat     Validate return value of hdrsrchrf_get_rf_dev before passing 
                     it to RF APIs.
01/21/2014   sat     Fixed memscpy logging bugs.
12/31/2013   vke     Fix for control channel value corruption in 1192 log pcaket
12/16/2013   sat     Changed memcpy() to assignment or memscpy() (safer version).
01/05/2012   wsh     Added support for SVLTE+SHDR
04/28/2011   kss     Fixed compiler warning.
12/17/2010   kss     Switch to timer api (from clock callbacks).
06/20/2010   kss     Restore min/max power flag support for Power logging. 
06/07/2010   kss     Added hdrrxstats_get_connected_state_per_data().
06/03/2010   cnx     Fixed Klocwork errors.
08/17/2009   etv     Used actual demod index instead of HDRMDSP_DEMOD_CARRIER_0
                     during Rev-A.
08/17/2009   etv     Use sub-aset index instead of demod index for logging.
08/17/2009   kss     Added hdrrx_get_fl_slot_count() support.
07/29/2009   etv     Added support for connected state PER.
06/22/2009   etv     Fixed Lint warnings.
05/21/2009   etv     Moved Decoder header logging to HDRDEC module.
04/21/2009   etv     Q6 development. Adopted Doxygen commenting style.
02/26/2009   etv     Floor RxAGC1 during diversity tune away.
02/06/2009   kss     Lint indentation issue.
01/12/2009   etv     Fixed MC FL Packet header buffer overflow issue.
12/10/2008   etv     Fixed PER issue with Legacy ALS log packet.
                     Removed MACIndex from FL Packet header info log packet.
12/05/2008   kss     Removed/replaced intlocks.
12/04/2008   etv     Lint fixes.
10/13/2008   kss     Lint fixes.
10/06/2008   etv     Logged power log pkt even during SOODA but floored rxagc.
08/05/2008   kss     Fix chain 1 AGC reporting.
06/16/2008   etv     Added timestamp to FL phy layer log packets.
06/11/2008   etv     Merged Enhanced Rates support.
04/09/2008   kss     Use error checking rather than ASSERT as lint fix.
03/31/2008   etv     Removed PN field, stored values for LNA State and 
                     txTotalPower in MC Power log packet
                     Cleaned up FL Physical layer packet logging code.
03/13/2008   etv     Removed RevB 7500 featurization.
02/04/2008   etv     Added support for logging FL physical layer packet dump.
                     Added support for MC MUP Forward statistics.
                     Store the Channel record in MC Power log packet.
                     Pruned some F3 messages from hdrrxstats module.
01/15/2008   vish    Added support for MC RF Rx/Tx AGC APIs.
01/14/2008   etv     Refactored Air link Summary log packet for Artemis.
12/19/2007   etv     Added support for per-carrier Instantaneous PER.
12/21/2007   sb      Added support for multi carrier power log packet.
11/01/2007   etv     Added support for MC Air link summary log packet.
11/29/2007   etv     Fixed featurization issue for 7800 compilation.
11/14/2007   etv     Updated hdrlog_update_cc_status function header.
11/08/2007   etv     Added support for MC SUP fwd stats log packet.
11/02/2007   etv     Used FEATURE_HDR_REVB_7500 to control Rx 7500 feature
10/31/2007   etv     Fixed merge error to avoid compilation error.
10/19/2007   kss     Use new RF API.
10/19/2007   kss     Merge code to log dec hdr log pkt immediately in idle.
09/26/2007   cc      Renamed hdrlog_mc_fl_carrier_id_type to
                     hdrlog_mc_carrier_id_type.
09/14/2007   etv     Fixed compiler issue in MCfwdLinkhdrInfo with RevB 7500.
08/24/2007   sb      Added support for multi-carrier decoder header log packet.
08/28/2007   etv     Fixed bug with fwd link statistics updating code.
08/20/2007   etv     Added support for Multi-carrier forward link statistics.
05/07/2007   etv     Created module.

============================================================================*/

/* <EJECT> */
/*===========================================================================

                        INCLUDE FILES FOR MODULE

============================================================================*/

#include "hdr_variation.h"
#include "hdrrxstats.h"   /* Exports all APIs in hdrrxstats.c */

#ifdef FEATURE_MEMCPY_REMOVAL
#include "stringl.h"
#endif /* FEATURE_MEMCPY_REMOVAL */

/* External includes */   /* External APIs used */
#include "rf.h"           /* RF APIs to get power data */
#include "rfm_hdr.h"      /* RF APIs to get power data */
#include "rfm_gain_state_types.h"   /*LNA state*/
#include "timer.h"

/* HDR Includes */
#include "hdrdebug.h"     /* All debug message macros */
#include "hdrmdspmcr.h"   /* All mDSP macros */
#include "hdrlogi.h"      /* All log packet types */
#include "hdrdsm.h"       /* All DSM Macros to read the stored user fields */
#include "hdrdec.h"       /* hdrdec_pkt_info_s_type */
#include "hdrsrchlog.h"   /* hdrsrchlog_get_link_summary_data */
#include "hdrfmacutil.h"  /* 1. hdrfmacutil_reset_mu_user_stats_summ
                             2. hdrfmacutil_log_mu_user_stats_summ */  
#include "hdrrmac.h"      /* hdrrmac_log_rtc_rate_count */
#include "hdrmod.h"       /* hdrmod_pa_is_on */
#include "hdrsrchtypes.h"
#include "hdrsrch.h"
#include "hdrsrchrf.h"

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/** Number of bins used for PER computations */
#define HDRRXSTATS_NUM_AVERAGE_PER_BINS                           8

/** Number of samples per bin used for PER computations */
#define HDRRXSTATS_PER_SAMPLE_SIZE                                16

/** Number of bytes in a forward link MAC layer packet. */
#define HDRRXSTATS_PKT_SIZE                                       128

/** Number of Rev 0 control channel rates */
#define HDRRXSTATS_REL_0_CC_DRC_NUM                               2

/** Number of canonical traffic channel rates */
#define HDRRXSTATS_TC_DRC_NUM                                     15

/** Number of Rev 0 traffic channel rates */
#define HDRRXSTATS_REL_0_TC_DRC_NUM                               12

/** Period of logging in milli-seconds */
#define HDRRXSTATS_LOGGING_PERIOD_MS                              1000

/** Floor value for RxAGC at -128dBm when RF tuning is not done */
#define HDRRXSTATS_RX_AGC_FLOOR                                   0x8000

#ifdef FEATURE_HDR_REVA_L1
/*---------------------------------------------------------------------------
                              REV-A MACROS
----------------------------------------------------------------------------*/
 
/** Number of unique packet sizes in Rev-A Transmission formats */
#define HDRRXSTATS_NUM_REV_A_PKT_SIZES                            8
/** Number of unique packet sizes in Rev-B Transmission formats */
#define HDRRXSTATS_NUM_REV_B_PKT_SIZES                            11

#ifdef FEATURE_HDR_REVB_ENH_RATES
#define HDRRXSTATS_NUM_PKT_SIZES  HDRRXSTATS_NUM_REV_B_PKT_SIZES
#define HDRRXSTATS_NUM_DRCS       HDRHAI_MAX_NUM_REL_B_DRCS
#else
#define HDRRXSTATS_NUM_PKT_SIZES  HDRRXSTATS_NUM_REV_A_PKT_SIZES
#define HDRRXSTATS_NUM_DRCS       HDRHAI_MAX_NUM_REL_A_DRCS
#endif /* FEATURE_HDR_REVB_ENH_RATES */

/** Number of Multi-user packet Transmission Formats */
#define HDRRXSTATS_NUM_MUP_TRANS_FORMATS                          8

/** Maximum size of Rev-A FTC physical layer log packet */
#define HDRRXSTATS_MAX_REV_A_LOG_PKT_SIZE                         640

/** Size of CRC and Tail in Rev-A physical layer packets in bytes */
#define HDRRXSTATS_REV_A_CRC_AND_TAIL                             4

/** Number of short Control Channel packet formats */
#define HDRRXSTATS_MAX_SHORT_CC_PKT_FORMATS                       3

#endif /* FEATURE_HDR_REVA_L1 */


#ifdef FEATURE_HDR_BCMCS
/*---------------------------------------------------------------------------
                              BCMCS MACROS
----------------------------------------------------------------------------*/

/** Number of broadcast rates */
#define HDRRXSTATS_BCC_RATE_NUM                                   14

/** Offset for indexing enhanced (slot reduced) rates for tracking CRC
    pass/fail */
#define HDRRXSTATS_BCC_ENH_RATE_OFFSET                            10

/** Maximum index for mapping DRC/BCC_ENH bit to BCC rate */
#define HDRRXSTATS_BCC_MAX_MAPPING_INDEX                          16   

#endif /* FEATURE_HDR_BCMCS */


/*==========================================================================
                             TYPE DEFINITIONS 
===========================================================================*/

/** CRC good/bad structure */
typedef struct
{
  uint32 good_cnt;           
    /**< Good (passing) CRC count */
  uint32 bad_cnt;            
    /**< Bad (failed) CRC count */
} hdrrxstats_crc_type;


#ifdef FEATURE_HDR_REVA_L1 

/** Type definition for Rev-A packet Stats */
typedef struct
{
  PACKED uint32 *good_crc_count;
    /**< count of CRC failed and passed packets */

  PACKED uint32 *early_decode;
    /**< count of early decoded pkt instances for each slot position */

} hdrrxstats_rev_a_pkt_stats_type;

#endif /* FEATURE_HDR_REVA_L1 */


/*==========================================================================
                             GLOBAL VARIABLES
===========================================================================*/

/** Statistics and logging structure */
LOCAL struct
{

  hdrrxstats_crc_type cum_phy_stats;
    /**< Cumulative Physical layer packet CRC statistics */

  hdrrxstats_crc_type periodic_phy_stats;
    /**< Physical layer packet CRC statistics for previous period (one second) */

  boolean pkt_rxd_flag;
    /**< Air Link Summary: used to indicate whether a pkt has been rx'd */

  uint32 sequence_num;
    /**< Sequence number to place on each log packet */

  /** Structure for computing PER during a traffic session */
  struct
  {
    uint32 bad_tc_pkts;
      /**< Number of bad traffic packets during the traffic session */
    uint32 bad_cc_pkts;
      /**< Number of bad control packets during the traffic session */
    uint32 total_tc_pkts;
      /**< Total number of traffic packets during the traffic session */
    uint32 total_cc_pkts;
       /**< Total number of control packets during the traffic session */
  } connected_state_per;

  /** Structure for instant PER computation */
  struct
  {
    uint32 bad_pkts;
      /**< Number of bad packets */
    uint32 total_pkts;
      /**< Total number of packets */
    uint16 instant;
      /**< Instantaneous PER */
    uint16 sequence;
      /**< PER sequence */
  } per [HDRSRCH_MAX_DEMOD_CARRIERS_CNT];

  /** Array for average PER storage and computation */
  struct
  {
    uint16  bad_pkts;
      /**< Number of bad packets */
    boolean valid;
      /**< Is this bin valid? */
  } average_per[HDRSRCH_MAX_DEMOD_CARRIERS_CNT][HDRRXSTATS_NUM_AVERAGE_PER_BINS];

  hdrlog_cc_status_enum_type last_cc_status;
    /** This field is for HDRSRCH SLEEP INFO logging. It indicates the status
        of the latest control channel decoding status   */
  
  /*---------------------------
   Release-0 Forward Statistics 
  ----------------------------*/
  hdrrxstats_crc_type 
    std_cc_crc[HDRSRCH_MAX_DEMOD_CARRIERS_CNT][HDRRXSTATS_REL_0_CC_DRC_NUM];
    /**< Each control channel rate */

  hdrrxstats_crc_type rel_0_tc_crc[HDRRXSTATS_TC_DRC_NUM];
    /**< Each traffic channel rate */

  /** Array for storing CC Early Decode data */
  struct
  {
    uint32 drc1[16];
      /**< Early decode statistics for DRC 1 Control channel packets */
    uint32 drc2[8];
      /**< Early decode statistics for DRC 2 control channel packets */
  } std_cc_early_decode[HDRSRCH_MAX_DEMOD_CARRIERS_CNT];

  /** Arrays for storing TC Early Decode data */
  struct
  {
    uint32 drc0[16];
      /**< Early decode statistics for DRC0 traffic channel packets */
    uint32 drc1[16];
      /**< Early decode statistics for DRC1 traffic channel packets */
    uint32 drc2[8];
      /**< Early decode statistics for DRC2 traffic channel packets */
    uint32 drc3[4];
      /**< Early decode statistics for DRC3 traffic channel packets */
    uint32 drc4[2];
      /**< Early decode statistics for DRC4 traffic channel packets */
    uint32 drc5[4];
      /**< Early decode statistics for DRC5 traffic channel packets */
    uint32 drc6[1];
      /**< Early decode statistics for DRC6 traffic channel packets */
    uint32 drc7[2];
      /**< Early decode statistics for DRC7 traffic channel packets */
    uint32 drc8[2];
      /**< Early decode statistics for DRC8 traffic channel packets */
    uint32 drc9[1];
      /**< Early decode statistics for DRC9 traffic channel packets */
    uint32 drc10[2];
      /**< Early decode statistics for DRC10 traffic channel packets */
    uint32 drc11[1];
      /**< Early decode statistics for DRC11 traffic channel packets */
    uint32 drc12[1];
      /**< Early decode statistics for DRC12 traffic channel packets */
    uint32 drc13[2];  /* Rev A rate. _ETV_ Remove? */
    uint32 drc14[1];  /* Rev A rate _ETV_ Remove? */  
    uint32 drc15[1];  /* Not used   _ETV_ Remove? */
  } rel_0_tc_early_decode;

  /*---------------------------
   Release-A Forward Statistics 
  ----------------------------*/
#ifdef FEATURE_HDR_REVA_L1

  hdrlog_rev_a_short_cc_crc_stats_type 
    short_cc_crc[HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /**< RevA short Control channel packets CRC statistics */

  hdrlog_rev_a_short_cc_early_decode_type 
    short_cc_early_decode[HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /**< RevA short control channel packets early decode statistics */

  hdrlog_rev_a_tc_crc_stats_type 
    mc_tc_crc[HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /**< Multi-carrier traffic channel CRC statistics */

  hdrlog_rev_a_tc_early_decode_type 
    mc_tc_early_decode[HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /**< Multi-carrier trafffic channel packet early decode statistics */

#ifdef FEATURE_HDR_REVB_ENH_RATES

  hdrlog_rev_b_enhanced_tc_rates_crc_stats_type
    mc_enh_tc_crc[HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /**< Multi-carrier enhanced traffic channel CRC statistics */

  hdrlog_rev_b_enhanced_tc_rates_early_decode_type
    mc_enh_tc_early_decode[HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /**< Multi-carrier enhanced traffic channel early decode statistics */

#endif /* FEATURE_HDR_REVB_ENH_RATES */

  hdrlog_mup_crc_stats_type 
    mc_mup_crc[HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /**< Rev-A Traffic channel Multi-user packet CRC statistics */

  hdrlog_mup_early_decode_type 
    mc_mup_early_decode[HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /**< Rev-A Traffic Channel Multi-user packet Early Decode statistics */
  
#endif /* FEATURE_HDR_REVA_L1 */

  boolean tc_stats_ready[HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /**< Do we have traffic channel stats available for logging in each of the 
         demod carriers? */

  boolean cc_stats_ready[HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /**< Do we have control channel statistics available for logging in each 
         one of the demod carriers? */

  boolean mup_stats_ready[HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /**< Do we have Multi-user packet statistics available for logging in 
         each one of the demod carriers? */

  /*-----------------------
   BCMCS Packet Statistics 
  ------------------------*/
#ifdef FEATURE_HDR_BCMCS

  /** Arrays for storing BCC Early Decode data */
  struct
  {
    uint32    rate38400[16];
    uint32    rate76800[8];
    uint32    rate153600[4];
    uint32    rate204800[3];
    uint32    rate307200short[2];
    uint32    rate307200long[4];
    uint32    rate409600[3];
    uint32    rate614400short[1];
    uint32    rate614400long[2];
    uint32    rate921600[2];
    uint32    rate1228800short[1];
    uint32    rate1228800long[2];
    uint32    rate1843200[1];
    uint32    rate2457600[1];
  } bcc_early_decode;

  hdrrxstats_crc_type bcc_crc[HDRRXSTATS_BCC_RATE_NUM];   
    /**< CRC's for each BC channel rate */

#endif /* FEATURE_HDR_BCMCS */

  /*---------------------------
   Unexpected Packet Statistics 
  ----------------------------*/
  uint32 unexp_crc;
    /**< Debug variable used to record/monitor unexpected CRC statistics 
         reported */

  uint32 unexp_early_decode[16];
    /**< Debug variable used to record/monitor unexpected early decode 
         statistics reported */

  uint32 directed_slots;
    /**< Counts slots directed at this AT */

} hdrrxstats;

/** Timers for logging link data */
  timer_type hdrrxstats_log_stat_timer;

/** Air Link Summary logging info */
LOCAL hdrlog_link_summary_type hdrrxstats_link_summary[HDRLOG_FRAMES_PER_PKT];

/** Air Link Summary/RF power logging info */
LOCAL hdrlog_power_type hdrrxstats_power[HDRLOG_FRAMES_PER_PKT];

/*==========================================================================
                             LOOK-UP TABLES 
===========================================================================*/

/** DRC to Frame size translation table (128-byte packets) */
LOCAL const byte hdrrxstats_drc_to_size[] =
  { 1, 1, 1, 1, 1, 2, 1, 2,
    3, 2, 4, 3, 4, 5, 5, 0 };

/** Array of pointers to Rel0 Traffic early decode arrays indexed by DRC */
LOCAL uint32* const hdrrxstats_rel_0_tc_decode_count[16] =
{
  hdrrxstats.rel_0_tc_early_decode.drc0,
  hdrrxstats.rel_0_tc_early_decode.drc1,
  hdrrxstats.rel_0_tc_early_decode.drc2,
  hdrrxstats.rel_0_tc_early_decode.drc3,
  hdrrxstats.rel_0_tc_early_decode.drc4,
  hdrrxstats.rel_0_tc_early_decode.drc5,
  hdrrxstats.rel_0_tc_early_decode.drc6,
  hdrrxstats.rel_0_tc_early_decode.drc7,
  hdrrxstats.rel_0_tc_early_decode.drc8,
  hdrrxstats.rel_0_tc_early_decode.drc9,
  hdrrxstats.rel_0_tc_early_decode.drc10,
  hdrrxstats.rel_0_tc_early_decode.drc11,
  hdrrxstats.rel_0_tc_early_decode.drc12,
  hdrrxstats.rel_0_tc_early_decode.drc13, /* Rev A rate. _ETV_ Remove?  */
  hdrrxstats.rel_0_tc_early_decode.drc14, /* Rev A rate  _ETV_ Remove? */
  hdrrxstats.rel_0_tc_early_decode.drc15  /* Not used    _ETV_ Remove? */
};

LOCAL uint32* const 
  hdrrxstats_rel_0_cc_decode_count
    [HDRSRCH_MAX_DEMOD_CARRIERS_CNT][HDRRXSTATS_REL_0_CC_DRC_NUM] =
{
  {
    hdrrxstats.std_cc_early_decode[0].drc1,
      /**< Early Decode Stats for DRC 1 CC on Demod 0 */
    hdrrxstats.std_cc_early_decode[0].drc2,
      /**< Early Decode Stats for DRC 2 CC on Demod 0 */
  },
  {
    hdrrxstats.std_cc_early_decode[1].drc1,
      /**< Early Decode Stats for DRC 1 CC on Demod 1 */
    hdrrxstats.std_cc_early_decode[1].drc2,
      /**< Early Decode Stats for DRC 2 CC on Demod 1 */
  },
  {
    hdrrxstats.std_cc_early_decode[2].drc1,
      /**< Early Decode Stats for DRC 1 CC on Demod 2 */
    hdrrxstats.std_cc_early_decode[2].drc2,
      /**< Early Decode Stats for DRC 2 CC on Demod 2 */
  }
};


#ifdef FEATURE_HDR_REVA_L1

#ifdef FEATURE_HDR_REVB_ENH_RATES
/** Sizes of Enhanced FL physical layer packets in bytes */
LOCAL uint16 const hdrrxstats_fl_pkt_sizes
  [HDRRXSTATS_NUM_PKT_SIZES] = { 16, 32, 64, 128, 256, 384, 512, 640, 768, 896, 1024 };
#else
/** Sizes of Enhanced FL physical layer packets in bytes */
LOCAL uint16 const hdrrxstats_fl_pkt_sizes
  [HDRRXSTATS_NUM_PKT_SIZES] = { 16, 32, 64, 128, 256, 384, 512, 640 };
#endif 

/** Table of pointers to rev-A packet stats for control channel. There
    are only three new packet sizes for control channel namely
    (128, 256 and 512) and hence we have one stats pointer to each of
    these sizes initialized to respective arrays. */
LOCAL hdrrxstats_rev_a_pkt_stats_type const
  hdrrxstats_short_cc_pkt_stats
    [HDRSRCH_MAX_DEMOD_CARRIERS_CNT][HDRRXSTATS_MAX_SHORT_CC_PKT_FORMATS] = 
{
  /* Demod 0 */
  {
    { &( hdrrxstats.short_cc_crc[0].good_pktsize_128bits ),
      hdrrxstats.short_cc_early_decode[0].pktsize_128bits },
    { &( hdrrxstats.short_cc_crc[0].good_pktsize_256bits ),
      hdrrxstats.short_cc_early_decode[0].pktsize_256bits },
    { &( hdrrxstats.short_cc_crc[0].good_pktsize_512bits ),
      hdrrxstats.short_cc_early_decode[0].pktsize_512bits }
  },
  /* Demod 1 */
  {
    { &( hdrrxstats.short_cc_crc[1].good_pktsize_128bits ),
      hdrrxstats.short_cc_early_decode[1].pktsize_128bits },
    { &( hdrrxstats.short_cc_crc[1].good_pktsize_256bits ),
      hdrrxstats.short_cc_early_decode[1].pktsize_256bits },
    { &( hdrrxstats.short_cc_crc[1].good_pktsize_512bits ),
      hdrrxstats.short_cc_early_decode[1].pktsize_512bits }
  },
  /* Demod 2 */
  {
    { &( hdrrxstats.short_cc_crc[2].good_pktsize_128bits ),
      hdrrxstats.short_cc_early_decode[2].pktsize_128bits },
    { &( hdrrxstats.short_cc_crc[2].good_pktsize_256bits ),
      hdrrxstats.short_cc_early_decode[2].pktsize_256bits },
    { &( hdrrxstats.short_cc_crc[2].good_pktsize_512bits ),
      hdrrxstats.short_cc_early_decode[2].pktsize_512bits }
  }
};

/*-------------------------------------------------------------------------
                        SUP FWD STATS ACCESS TABLES 

 * AT decodes FL packets based on Preamble which is covered with MACIndex of
 * the user. Preambles for all transmission formats look identical and when
 * a preamble is detected while decoding at a particular DRC, AT cannot know 
 * which tx format of that particular DRC was scheduled by AN. Hence at the 
 * end of every slot, AT tries to decode all supported Tx.formats of that DRC.
 * When the CRC, passes, it knows the Tx. Format scheduled by AN. However, 
 * when the CRC fails, the "Data Length" field of decoder header denotes the
 * SHORTEST PACKET TYPE supported because it is the last hypothesis tried 
 * before a CRC failure is declared. Since there is no way of knowing the 
 * Tx. Format scheduled by AN when the CRC fails, only per-DRC CRC failed 
 * statistics are recorded.
 ------------------------------------------------------------------------*/

/** This table is used to update the number of CRC failed packets. Unlike,
    the good count and early decode statistics, the bad count is recorded
    per-DRC only. So a separate table of pointers is maintained for ease of
    update  */
LOCAL PACKED uint32* hdrrxstats_mc_tc_su_pkt_bad_cnt_lut
    [HDRSRCH_MAX_DEMOD_CARRIERS_CNT][HDRRXSTATS_NUM_DRCS];

/** This table is used to update the CRC passed and early termination
    statitics. */
LOCAL hdrrxstats_rev_a_pkt_stats_type 
  hdrrxstats_mc_tc_su_pkt_stats_lut [HDRSRCH_MAX_DEMOD_CARRIERS_CNT] 
                                    [HDRRXSTATS_NUM_DRCS] 
                                    [HDRRXSTATS_NUM_PKT_SIZES];
/*-------------------------------------------------------------------------
                        MUP FWD STATS ACCESS TABLES 
 ------------------------------------------------------------------------*/

/** Table of pointers to per-DRC Traffic channel Multi-user packet CRC
    failed counts. The only valid DRCs for Multi-user packets are 3, 5,
    8, 10 and 13. For all other DRCs the look-up table returns the address
    of a debug variable "unexp_crc" to record if the decoder gives us a
    corrupt DRC */
LOCAL PACKED uint32 *hdrrxstats_mc_tc_mu_pkt_bad_cnt
  [HDRSRCH_MAX_DEMOD_CARRIERS_CNT][HDRHAI_MAX_NUM_REL_A_DRCS];

/** Table of pointers to per-Tx.Format Traffic channel Multi-user packet
    CRC Passed counts and Early decode counts */
LOCAL hdrrxstats_rev_a_pkt_stats_type hdrrxstats_mc_mup_stats
  [HDRSRCH_MAX_DEMOD_CARRIERS_CNT][HDRRXSTATS_NUM_MUP_TRANS_FORMATS];

#endif /* FEATURE_HDR_REVA_L1 */

#ifdef FEATURE_HDR_BCMCS

/** Array for mapping DRC/BCC_ENH bit to BCC rate value for indexing For
    normal broadcast rate, use DRC to find BCC rate. For slot reduced
    (also called enhanced rate), add HDRRXSTATS_BCC_ENH_RATE_OFFSET (10)
    to DRC to find BCC rate */
LOCAL int const hdrrxstats_bcc_rates[HDRRXSTATS_BCC_MAX_MAPPING_INDEX] = 
      {0, 0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 12, 13, 3, 0, 6};

/** Array of pointers to early decode arrays for broadcast */
LOCAL uint32* const hdrrxstats_bcc_decode_count[] =
{
 hdrrxstats.bcc_early_decode.rate38400,
 hdrrxstats.bcc_early_decode.rate76800,
 hdrrxstats.bcc_early_decode.rate153600,
 hdrrxstats.bcc_early_decode.rate204800,
 hdrrxstats.bcc_early_decode.rate307200short,
 hdrrxstats.bcc_early_decode.rate307200long,
 hdrrxstats.bcc_early_decode.rate409600,
 hdrrxstats.bcc_early_decode.rate614400short,
 hdrrxstats.bcc_early_decode.rate614400long,
 hdrrxstats.bcc_early_decode.rate921600,
 hdrrxstats.bcc_early_decode.rate1228800short,
 hdrrxstats.bcc_early_decode.rate1228800long,
 hdrrxstats.bcc_early_decode.rate1843200,
 hdrrxstats.bcc_early_decode.rate2457600
};

#endif /* FEATURE_HDR_BCMCS */

/*============================================================================
                        LOCAL FUNCTION DEFINITIONS
=============================================================================*/

/*============================================================================
  HDRRXSTATS_INIT_MC_FWD_STATS_LUT
*/
/**
  Initialize the lookup table for updating forward statistics 

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
LOCAL void hdrrxstats_init_mc_fwd_stats_lut ( void )
{

  uint8 sub_aset_idx = 0;
  uint8 drc;
  uint8 ps;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Initialize everything to unexpected statistics first */
  for ( sub_aset_idx = 0; sub_aset_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; 
        ++sub_aset_idx )
  {
    for ( drc = 0; drc < (uint8) HDRRXSTATS_NUM_DRCS; ++drc )
    {
      for ( ps = 0; ps < HDRRXSTATS_NUM_PKT_SIZES; ++ps )
      {
        hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][drc][ps].good_crc_count =
          &hdrrxstats.unexp_crc;
        hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][drc][ps].early_decode = 
          hdrrxstats.unexp_early_decode;
      }
    }
  }

  for ( sub_aset_idx = 0; sub_aset_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; 
        ++sub_aset_idx )
  {
    /*========================================================================
          SINGLE-USER TRAFFIC CHANNEL PACKET STATISTICS Good & Early Decode
    =========================================================================*/

    /*=============================== DRC 0 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 0, Packet Size index 0 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][0][0].good_crc_count = 
      &hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc0_128bits;
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][0][0].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc0_128bits;

    /* CRC & EarlyDecode convenience pointers for DRC 0, Packet Size index 1 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][0][1].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc0_256bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][0][1].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc0_256bits;

    /* CRC & EarlyDecode convenience pointers for DRC 0, Packet Size index 2 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][0][2].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc0_512bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][0][2].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc0_512bits;

    /* CRC & EarlyDecode convenience pointers for DRC 0, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][0][3].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc0_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][0][3].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc0_1024bits;

    /*=============================== DRC 1 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 1, Packet Size index 0 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][1][0].good_crc_count =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc1_128bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][1][0].early_decode =
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc1_128bits;

    /* CRC & EarlyDecode convenience pointers for DRC 1, Packet Size index 1 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][1][1].good_crc_count =
      &(hdrrxstats.mc_tc_crc[0].good_drc1_256bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][1][1].early_decode = 
      hdrrxstats.mc_tc_early_decode[0].drc1_256bits;

    /* CRC & EarlyDecode convenience pointers for DRC 1, Packet Size index 2 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][1][2].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc1_512bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][1][2].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc1_512bits;

    /* CRC & EarlyDecode convenience pointers for DRC 1, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][1][3].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc1_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][1][3].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc1_1024bits;

    /*=============================== DRC 2 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 2, Packet Size index 0 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][2][0].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc2_128bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][2][0].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc2_128bits;

    /* CRC & EarlyDecode convenience pointers for DRC 2, Packet Size index 1 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][2][1].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc2_256bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][2][1].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc2_256bits;

    /* CRC & EarlyDecode convenience pointers for DRC 2, Packet Size index 2 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][2][2].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc2_512bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][2][2].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc2_512bits;

    /* CRC & EarlyDecode convenience pointers for DRC 2, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][2][3].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc2_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][2][3].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc2_1024bits;

    /*=============================== DRC 3 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 3, Packet Size index 0 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][3][0].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc3_128bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][3][0].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc3_128bits;

    /* CRC & EarlyDecode convenience pointers for DRC 3, Packet Size index 1 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][3][1].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc3_256bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][3][1].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc3_256bits;

    /* CRC & EarlyDecode convenience pointers for DRC 3, Packet Size index 2 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][3][2].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc3_512bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][3][2].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc3_512bits;

    /* CRC & EarlyDecode convenience pointers for DRC 3, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][3][3].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc3_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][3][3].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc3_1024bits;

    /*=============================== DRC 4 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 4, Packet Size index 0 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][4][0].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc4_128bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][4][0].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc4_128bits;

    /* CRC & EarlyDecode convenience pointers for DRC 4, Packet Size index 1 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][4][1].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc4_256bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][4][1].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc4_256bits;

    /* CRC & EarlyDecode convenience pointers for DRC 4, Packet Size index 2 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][4][2].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc4_512bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][4][2].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc4_512bits;

    /* CRC & EarlyDecode convenience pointers for DRC 4, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][4][3].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc4_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][4][3].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc4_1024bits;

    /*=============================== DRC 5 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 5, Packet Size index 2 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][5][2].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc5_512bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][5][2].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc5_512bits;

    /* CRC & EarlyDecode convenience pointers for DRC 5, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][5][3].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc5_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][5][3].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc5_1024bits;

    /* CRC & EarlyDecode convenience pointers for DRC 5, Packet Size index 4 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][5][4].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc5_2048bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][5][4].early_decode =
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc5_2048bits;

    /*=============================== DRC 6 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 6, Packet Size index 0 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][6][0].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc6_128bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][6][0].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc6_128bits;

    /* CRC & EarlyDecode convenience pointers for DRC 6, Packet Size index 1 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][6][1].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc6_256bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][6][1].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc6_256bits;

    /* CRC & EarlyDecode convenience pointers for DRC 6, Packet Size index 2 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][6][2].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc6_512bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][6][2].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc6_512bits;

    /* CRC & EarlyDecode convenience pointers for DRC 6, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][6][3].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc6_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][6][3].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc6_1024bits;

    /*=============================== DRC 7 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 7, Packet Size index 2 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][7][2].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc7_512bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][7][2].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc7_512bits;

    /* CRC & EarlyDecode convenience pointers for DRC 7, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][7][3].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc7_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][7][3].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc7_1024bits;

    /* CRC & EarlyDecode convenience pointers for DRC 7, Packet Size index 4 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][7][4].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc7_2048bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][7][4].early_decode =
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc7_2048bits;

    /*=============================== DRC 8 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 8, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][8][3].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc8_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][8][3].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc8_1024bits;

    /* CRC & EarlyDecode convenience pointers for DRC 8, Packet Size index 5 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][8][5].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc8_3072bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][8][5].early_decode =
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc8_3072bits;

    /*=============================== DRC 9 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 9, Packet Size index 2 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][9][2].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc9_512bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][9][2].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc9_512bits;

    /* CRC & EarlyDecode convenience pointers for DRC 9, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][9][3].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc9_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][9][3].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc9_1024bits;

    /* CRC & EarlyDecode convenience pointers for DRC 9, Packet Size index 4 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][9][4].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc9_2048bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][9][4].early_decode =
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc9_2048bits;

    /*=============================== DRC A =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 10, Packet Size index 6 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][10][6].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc10_4096bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][10][6].early_decode =
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc10_4096bits;

    /*=============================== DRC B =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 11, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][11][3].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc11_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][11][3].early_decode = 
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc11_1024bits;

    /* CRC & EarlyDecode convenience pointers for DRC 11, Packet Size index 5 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][11][5].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc11_3072bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][11][5].early_decode =
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc11_3072bits;

    /*=============================== DRC C =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 12, Packet Size index 6 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][12][6].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc12_4096bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][12][6].early_decode =
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc12_4096bits;

    /*=============================== DRC D =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 13, Packet Size index 7 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][13][7].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc13_5120bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][13][7].early_decode =
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc13_5120bits;

    /*=============================== DRC E =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 14, Packet Size index 7 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][14][7].good_crc_count = 
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].good_drc14_5120bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][14][7].early_decode =
      hdrrxstats.mc_tc_early_decode[sub_aset_idx].drc14_5120bits;

#ifdef FEATURE_HDR_REVB_ENH_RATES
    /*=============================== DRC 10 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 16, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][16][3].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc16_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][16][3].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc16_1024bits;

    /* CRC & EarlyDecode convenience pointers for DRC 16, Packet Size index 4 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][16][4].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc16_2048bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][16][4].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc16_2048bits;

    /* CRC & EarlyDecode convenience pointers for DRC 16, Packet Size index 5 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][16][5].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc16_3072bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][16][5].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc16_3072bits;

    /*=============================== DRC 11 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 17, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][17][3].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc17_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][17][3].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc17_1024bits;

    /* CRC & EarlyDecode convenience pointers for DRC 17, Packet Size index 4 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][17][4].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc17_2048bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][17][4].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc17_2048bits;

    /* CRC & EarlyDecode convenience pointers for DRC 17, Packet Size index 6 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][17][6].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc17_4096bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][17][6].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc17_4096bits;

    /*=============================== DRC 12 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 18, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][18][3].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc18_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][18][3].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc18_1024bits;

    /* CRC & EarlyDecode convenience pointers for DRC 18, Packet Size index 4 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][18][4].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc18_2048bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][18][4].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc18_2048bits;

    /* CRC & EarlyDecode convenience pointers for DRC 18, Packet Size index 7 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][18][7].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc18_5120bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][18][7].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc18_5120bits;

    /*=============================== DRC 13 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 19, Packet Size index 4 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][19][4].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc19_2048bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][19][4].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc19_2048bits;

    /* CRC & EarlyDecode convenience pointers for DRC 19, Packet Size index 8 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][19][8].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc19_6144bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][19][8].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc19_6144bits;

    /*=============================== DRC 14 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 20, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][20][3].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc20_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][20][3].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc20_1024bits;

    /* CRC & EarlyDecode convenience pointers for DRC 20, Packet Size index 9 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][20][9].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc20_7168bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][20][9].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc20_7168bits;

    /*=============================== DRC 15 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 21, Packet Size index 10 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][21][10].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc21_8192bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][21][10].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc21_8192bits;

    /*=============================== DRC 16 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 22, Packet Size index 4 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][22][4].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc22_2048bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][22][4].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc22_2048bits;

    /* CRC & EarlyDecode convenience pointers for DRC 22, Packet Size index 8 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][22][8].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc22_6144bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][22][8].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc22_6144bits;

    /*=============================== DRC 17 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 23, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][23][3].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc23_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][23][3].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc23_1024bits;

    /* CRC & EarlyDecode convenience pointers for DRC 23, Packet Size index 9 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][23][9].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc23_7168bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][23][9].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc23_7168bits;

    /*=============================== DRC 18 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 24, Packet Size index 10 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][24][10].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc24_8192bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][24][10].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc24_8192bits;

    /*=============================== DRC 19 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 25, Packet Size index 4 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][25][4].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc25_2048bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][25][4].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc25_2048bits;

    /* CRC & EarlyDecode convenience pointers for DRC 25, Packet Size index 8 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][25][8].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc25_6144bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][25][8].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc25_6144bits;

    /*=============================== DRC 20 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 26, Packet Size index 3 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][26][3].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc26_1024bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][26][3].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc26_1024bits;

    /* CRC & EarlyDecode convenience pointers for DRC 26, Packet Size index 9 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][26][9].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc26_7168bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][26][9].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc26_7168bits;

    /*=============================== DRC 21 =================================*/

    /* CRC & EarlyDecode convenience pointers for DRC 27, Packet Size index 10 */
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][27][10].good_crc_count = 
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].good_drc27_8192bits);
    hdrrxstats_mc_tc_su_pkt_stats_lut[sub_aset_idx][27][10].early_decode =
      hdrrxstats.mc_enh_tc_early_decode[sub_aset_idx].drc27_8192bits;

#endif /* FEATURE_HDR_REVB_ENH_RATES */
    /*=========================================================================
                SINGLE-USER TRAFFIC CHANNEL PACKET BAD STATISTICS
    =========================================================================*/

    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][0] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc0);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][1] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc1);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][2] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc2);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][3] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc3);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][4] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc4);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][5] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc5);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][6] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc6);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][7] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc7);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][8] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc8);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][9] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc9);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][10] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc10);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][11] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc11);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][12] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc12);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][13] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc13);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][14] =
      &(hdrrxstats.mc_tc_crc[sub_aset_idx].bad_drc14);

#ifdef FEATURE_HDR_REVB_ENH_RATES

    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][16] =
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].bad_drc16);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][17] =
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].bad_drc17);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][18] =
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].bad_drc18);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][19] =
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].bad_drc19);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][20] =
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].bad_drc20);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][21] =
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].bad_drc21);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][22] =
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].bad_drc22);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][23] =
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].bad_drc23);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][24] =
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].bad_drc24);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][25] =
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].bad_drc25);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][26] =
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].bad_drc26);
    hdrrxstats_mc_tc_su_pkt_bad_cnt_lut[sub_aset_idx][27] =
      &(hdrrxstats.mc_enh_tc_crc[sub_aset_idx].bad_drc27);

#endif /* FEATURE_HDR_REVB_ENH_RATES */
    /*=========================================================================
                MULTI-USER TRAFFIC CHANNEL PACKET BAD STATISTICS
    =========================================================================*/

    /* Short packet DRC 3 [128, 4, 256] */
    hdrrxstats_mc_mup_stats[sub_aset_idx][0].good_crc_count = 
      &hdrrxstats.mc_mup_crc[sub_aset_idx].good_cnt_drc3_128;
    hdrrxstats_mc_mup_stats[sub_aset_idx][0].early_decode = 
      hdrrxstats.mc_mup_early_decode[sub_aset_idx].size_128;

    /* Short packet DRC 3 [256, 4, 256] */
    hdrrxstats_mc_mup_stats[sub_aset_idx][1].good_crc_count = 
      &hdrrxstats.mc_mup_crc[sub_aset_idx].good_cnt_drc3_256;
    hdrrxstats_mc_mup_stats[sub_aset_idx][1].early_decode = 
      hdrrxstats.mc_mup_early_decode[sub_aset_idx].size_256;

    /* Short packet DRC 3 [512, 4, 256] */
    hdrrxstats_mc_mup_stats[sub_aset_idx][2].good_crc_count = 
      &hdrrxstats.mc_mup_crc[sub_aset_idx].good_cnt_drc3_512;
    hdrrxstats_mc_mup_stats[sub_aset_idx][2].early_decode = 
      hdrrxstats.mc_mup_early_decode[sub_aset_idx].size_512;

    /* Canonical packet DRC 3 [1024, 4, 256] */
    hdrrxstats_mc_mup_stats[sub_aset_idx][3].good_crc_count = 
      &hdrrxstats.mc_mup_crc[sub_aset_idx].good_cnt_drc3_1024;
    hdrrxstats_mc_mup_stats[sub_aset_idx][3].early_decode = 
      hdrrxstats.mc_mup_early_decode[sub_aset_idx].size_1024;

    /* Canonical packet DRC 5 [2048, 4, 128] */
    hdrrxstats_mc_mup_stats[sub_aset_idx][4].good_crc_count = 
      &hdrrxstats.mc_mup_crc[sub_aset_idx].good_cnt_drc5_2048;
    hdrrxstats_mc_mup_stats[sub_aset_idx][4].early_decode = 
      hdrrxstats.mc_mup_early_decode[sub_aset_idx].size_2048;

    /* Canonical packet DRC 8 [3072, 2, 64] */
    hdrrxstats_mc_mup_stats[sub_aset_idx][5].good_crc_count = 
      &hdrrxstats.mc_mup_crc[sub_aset_idx].good_cnt_drc8_3072;
    hdrrxstats_mc_mup_stats[sub_aset_idx][5].early_decode = 
      hdrrxstats.mc_mup_early_decode[sub_aset_idx].size_3072;

    /* Canonical packet DRC 10 [4096, 2, 64] */
    hdrrxstats_mc_mup_stats[sub_aset_idx][6].good_crc_count = 
      &hdrrxstats.mc_mup_crc[sub_aset_idx].good_cnt_drc10_4096;
    hdrrxstats_mc_mup_stats[sub_aset_idx][6].early_decode = 
      hdrrxstats.mc_mup_early_decode[sub_aset_idx].size_4096;

    /* Canonical packet DRC 13 [5120, 2, 64] */
    hdrrxstats_mc_mup_stats[sub_aset_idx][7].good_crc_count = 
      &hdrrxstats.mc_mup_crc[sub_aset_idx].good_cnt_drc13_5120;
    hdrrxstats_mc_mup_stats[sub_aset_idx][7].early_decode = 
      hdrrxstats.mc_mup_early_decode[sub_aset_idx].size_5120;

    /*=========================================================================
                MULTI-USER TRAFFIC CHANNEL PACKET BAD STATISTICS
    =========================================================================*/

    /* DRC 0, 1, 2, 4, 6, 7, 9, 11, 12, 14  Unexpected      */
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][0] = &hdrrxstats.unexp_crc;
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][1] = &hdrrxstats.unexp_crc;
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][2] = &hdrrxstats.unexp_crc;
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][4] = &hdrrxstats.unexp_crc;
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][6] = &hdrrxstats.unexp_crc;
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][7] = &hdrrxstats.unexp_crc;
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][9] = &hdrrxstats.unexp_crc;
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][11] = &hdrrxstats.unexp_crc;
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][12] = &hdrrxstats.unexp_crc;
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][14] = &hdrrxstats.unexp_crc;

    /* DRC 3  [1024, 4, 256]  */
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][3] = 
      &hdrrxstats.mc_mup_crc[sub_aset_idx].bad_cnt_drc3;
    /* DRC 5  [2048, 4, 128]  */
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][5] = 
      &hdrrxstats.mc_mup_crc[sub_aset_idx].bad_cnt_drc5;
    /* DRC 8  [3072, 2, 64]   */
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][8] = 
      &hdrrxstats.mc_mup_crc[sub_aset_idx].bad_cnt_drc8;
    /* DRC 10 [4096, 2, 64]   */
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][10] = 
      &hdrrxstats.mc_mup_crc[sub_aset_idx].bad_cnt_drc10;
    /* DRC 13 [5120, 2, 64]   */
    hdrrxstats_mc_tc_mu_pkt_bad_cnt[sub_aset_idx][13] = 
      &hdrrxstats.mc_mup_crc[sub_aset_idx].bad_cnt_drc13;

  } /* for each carrier */

} /* hdrrxstats_init_mc_fwd_stats_lut */


/*============================================================================
  HDRRXSTATS_GATHER_POWER_DATA
*/
/**
  This function samples RF power data.  The provided structure is filled in
  with the requested data.

  @context HDRRX task. */
/*============================================================================*/
LOCAL void hdrrxstats_gather_power_data
(
  hdrlog_power_type *power,
    /**< Structure to fill in for logging */
  boolean agc0_is_valid,
    /**< Indicates whether RxAGC on primary chain is valid or not */
  boolean agc1_is_valid
    /**< Indicates whether RxAGC on diversity chain is valid or not */
)
{
  rfm_device_enum_type rf_dev;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (agc0_is_valid)
  {
    rf_dev = hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN);

    /* Log PA status and ratchet mode only when agc0 is valid */
    power->power_info.pa_state = ( (hdrmod_pa_is_on() == TRUE)? 1: 0 );
    /* Set packet received flag if necessary */

    power->power_info.ratchet_mode = ( HDRMDSP_TX_IS_MIN_PWR_RATCHETING() ||
                                       HDRMDSP_TX_IS_MAX_PWR_RATCHETING() );
  }

  power->power_info.at_type = HDRLOG_AT_TYPE_MSM5500;
    /* Only MSM5500 for now */

  if (agc0_is_valid)
  {
    power->tx_open_loop_power = 
      rfm_hdr_get_tx_open_loop ( rf_dev, 
                                 RFM_HDR_CARRIER_ID__0, 
                                 RFM_HDR_AGC_FORMAT__DBM256 );

    power->tx_closed_loop_adjust = 
      rfm_hdr_get_tx_adjust ( rf_dev,
                              RFM_HDR_CARRIER_ID__0, 
                              RFM_HDR_AGC_FORMAT__DBM256 );

    power->tx_pilot_power = 
      rfm_hdr_get_tx_pilot_power ( rf_dev, 
                                   RFM_HDR_CARRIER_ID__0, 
                                   RFM_HDR_AGC_FORMAT__DBM256 );

    power->tx_total_power = 
      rfm_hdr_get_tx_total_power ( rf_dev, 
                                   RFM_HDR_CARRIER_ID__0, 
                                   RFM_HDR_AGC_FORMAT__DBM256 );
  }
  else
  {
    /* RF tuning is not done yet!Logging default values */
    power->tx_open_loop_power = 0xFFFF;
    power->tx_closed_loop_adjust = 0xFFFF;
    power->tx_pilot_power = 0xFFFF;
    power->tx_total_power = 0xFFFF;
    power->power_info.pa_state = 0;
    power->power_info.ratchet_mode = FALSE;
  }

  if ( agc0_is_valid )
  {
    power->rx_agc0 = rfm_hdr_get_rx_agc ( hdrsrchrf_get_rf_dev(HDRSRCHRF_PRI_CHAIN),
                                          RFM_HDR_CARRIER_ID__0,
                                          RFM_HDR_AGC_FORMAT__DBM256 );
  }
  else
  {
    power->rx_agc0 = (int16) HDRRXSTATS_RX_AGC_FLOOR;
      /* RF tuning is not done yet! Floor the RxAGC values to -128dBm */
  }

  if ( agc1_is_valid )
  {
    power->rx_agc1 = rfm_hdr_get_rx_agc ( hdrsrchrf_get_rf_dev(HDRSRCHRF_DIV_CHAIN), 
                                          RFM_HDR_CARRIER_ID__0,
                                          RFM_HDR_AGC_FORMAT__DBM256 );
  }
  else
  {
    power->rx_agc1 = (int16) HDRRXSTATS_RX_AGC_FLOOR;
      /* Diversity chain is disabled! Floor the RxAGC values to -128dBm */
  }

} /* hdrrxstats_gather_power_data */


/*============================================================================
  HDRRXSTATS_LOG_RF_POWER
*/
/**
  This function performs RF power sampling and logging.  A log packet is
  produced every other time the function is called.

  @context HDRRX task. */
/*============================================================================*/
LOCAL void hdrrxstats_log_rf_power
( 
  boolean agc0_is_valid,
    /**< Indicates whether RxAGC on primary chain is valid or not */
  boolean agc1_is_valid
    /**< Indicates whether RxAGC on diversity chain is valid or not */
)
{
  static byte frame_index = 0;  
    /* Frame index into packet */

  LOG_HDR_POWER_C_type *record;
    /* Log record for air link summary */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrrxstats_gather_power_data ( 
    &hdrrxstats_power[frame_index], agc0_is_valid, agc1_is_valid );
    /* Gather data from this past frame */

  frame_index++;

  /* A log packet is only sent every other frame */
  if ( frame_index >= HDRLOG_FRAMES_PER_PKT )
  {
    frame_index = 0;

    record = (LOG_HDR_POWER_C_type *) log_alloc ( LOG_HDR_POWER_C,
                                                  sizeof(LOG_HDR_POWER_C_type) );

    if ( record != NULL )
    {
#ifdef FEATURE_MEMCPY_REMOVAL
      memscpy ( (void*) ( record->power ), 
                sizeof(record->power),
                (void*) hdrrxstats_power, 
                sizeof ( hdrrxstats_power ) );
#else
      memcpy ( (void*) ( record->power ), 
               (void*) hdrrxstats_power, 
               sizeof ( hdrrxstats_power ) );
#endif /* FEATURE_MEMCPY_REMOVAL */
        /* Copy two frames' worth of data to log. */

      log_commit ( (log_type*) record );
    }
  }

} /* hdrrxstats_log_rf_power */


/*============================================================================
  HDRRXSTATS_GATHER_MC_POWER_DATA
*/
/**
  This function samples RF power data over a multi carrier network.
  The provided structure is filled in with the requested data.

  @context HDRRX task. */
/*============================================================================*/
LOCAL void hdrrxstats_gather_mc_power_data
(
  hdrlog_mc_power_per_carrier_info_type *cptr,
    /**< Convenience pointer to per carrier information */
  uint8 demod_idx,
    /**< Demod Index */
  boolean agc0_is_valid,
    /**< Indicates whether RxAGC on primary chain is valid or not */
  boolean agc1_is_valid
    /**< Indicates whether RxAGC on diversity chain is valid or not */
)
{
  rfm_device_enum_type rf_dev;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (agc0_is_valid)
  {
    rf_dev = hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN);

    cptr->power_info.ratchet_mode = 
      ( HDRMDSP_TX_IS_MIN_PWR_RATCHETING() || 
        HDRMDSP_TX_IS_MAX_PWR_RATCHETING() );

    cptr->power_info.carrier_enabled = ( (hdrmod_pa_is_on() == TRUE)? 1: 0 );
    /* Set packet received flag if necessary */  
  } 

  if (agc0_is_valid)
  {
    cptr->tx_open_loop_power =
      rfm_hdr_get_tx_open_loop ( rf_dev, 
                                 (rfm_hdr_carrier_id_type) demod_idx,
                                  RFM_HDR_AGC_FORMAT__DBM256 );
      /* TX power determined by the open loop adjust mechanism */

    cptr->tx_closed_loop_adjust =
      rfm_hdr_get_tx_adjust ( rf_dev,
                              (rfm_hdr_carrier_id_type) demod_idx,
                               RFM_HDR_AGC_FORMAT__DBM256 );
      /* TX power determined by the closed loop adjust mechanism */

    cptr->tx_pilot_power =
      rfm_hdr_get_tx_pilot_power ( rf_dev, 
                                   (rfm_hdr_carrier_id_type) demod_idx,
                                    RFM_HDR_AGC_FORMAT__DBM256 );
       /* Pilot power that is transmitted over the pilot channel */

    cptr->tx_total_power =
      rfm_hdr_get_tx_total_power ( rf_dev, 
                                   (rfm_hdr_carrier_id_type) demod_idx, 
                                    RFM_HDR_AGC_FORMAT__DBM256 );
      /* Total Tx power as determined by the entire Tx AGC mechanism */
  }
  else
  {
    /* RF tuning is not done yet!Logging default values */
    cptr->tx_open_loop_power = 0xFFFF;
    cptr->tx_closed_loop_adjust = 0xFFFF;
    cptr->tx_pilot_power =0xFFFF;
    cptr->tx_total_power =0xFFFF;
    cptr->power_info.ratchet_mode = FALSE;
    cptr->power_info.carrier_enabled = 0;
  }

  if ( agc0_is_valid )
  {
    cptr->rx_agc0 =
      rfm_hdr_get_rx_agc ( hdrsrchrf_get_rf_dev(HDRSRCHRF_PRI_CHAIN), 
                           (rfm_hdr_carrier_id_type) demod_idx, 
                            RFM_HDR_AGC_FORMAT__DBM256 );
      /* Total receive power as seen by Antenna0 */
  }
  else
  {
    cptr->rx_agc0 = (int16) HDRRXSTATS_RX_AGC_FLOOR;
      /* RF tuning is not done yet! Floor the RxAGC values to -128dBm */
  }

  if ( agc1_is_valid )
  {
    cptr->rx_agc1 =
      rfm_hdr_get_rx_agc ( hdrsrchrf_get_rf_dev(HDRSRCHRF_DIV_CHAIN), 
                           (rfm_hdr_carrier_id_type) demod_idx, 
                            RFM_HDR_AGC_FORMAT__DBM256 );
      /* Total receive power as seen by the Antenna1 */
  }
  else
  {
    cptr->rx_agc1 = (int16) HDRRXSTATS_RX_AGC_FLOOR;
      /* Diversity chain is not enabled! Floor the RxAGC values to -128dBm */
  }

} /* hdrrxstats_gather_mc_power_data */


/*============================================================================
  HDRRXSTATS_LOG_MC_RF_POWER
*/
/**
  This function performs multi carrier RF power sampling and logging.  
  A log packet is produced every time the function is called.

  @context HDRRX task. */
/*============================================================================*/
LOCAL void hdrrxstats_log_mc_rf_power
( 
  boolean agc0_is_valid, 
    /**< Indicates whether RxAGC on primary chain is valid or not */
  boolean agc1_is_valid 
    /**< Indicates whether RxAGC on diversity chain is valid or not */
)
{
  LOG_HDR_MC_POWER_C_type *log_ptr;
    /* Log pointer to power log record type */

  hdrlog_mc_power_per_carrier_info_type *perc_pwr_log_ptr;
    /* Convenience pointer to per carrier information */

  uint8 demod_idx;
    /* Demod Index */

  hdrsrch_fl_carrier_info_type saset_info [HDRSRCH_MAX_NUM_SUBASETS];
    /* Information including channel number and various FL indices for 
       each carrier */

  boolean saset_in_use [HDRSRCH_MAX_NUM_SUBASETS];
    /* Boolean flag denoting whether the carrier is in use */

  uint8 num_carriers = 0;
    /* Number of carriers in use */

  uint8 saset_idx;
    /* Sub ASET index */

  rfm_device_enum_type rf_dev;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (agc0_is_valid)
  {
    rf_dev = hdrsrchrf_get_rf_dev(HDRSRCHRF_PRI_CHAIN);
  }
  
  for ( saset_idx = 0; saset_idx < HDRSRCH_MAX_NUM_SUBASETS; ++saset_idx )
  {
    saset_in_use[saset_idx] = 
      hdrsrch_get_fl_carrier_info( saset_idx, &saset_info[saset_idx] );

    if (saset_in_use [saset_idx])
    {
      num_carriers++;
    }

  } /* for each sub-active set */
  
  log_ptr = (LOG_HDR_MC_POWER_C_type *) 
    log_alloc( LOG_HDR_MC_POWER_C, 
               ( sizeof(LOG_HDR_MC_POWER_C_type) + 
                 ( (num_carriers - 1) * 
                   sizeof(hdrlog_mc_power_per_carrier_info_type) ) ) );
   
  if (log_ptr != NULL)
  {
    log_ptr->version = HDRLOG_MC_POWER_VER;
    /* The version number of the packet. */

    if (agc0_is_valid)
    {
      log_ptr->lna_state = 
        (uint8) rfm_hdr_get_lna_gain_state(rf_dev);
        /* LNA state of the log packet */
    }
    else
    {
      /* RF tuning is not done yet */
      log_ptr->lna_state = RFM_LNA_INVALID_GAIN_STATE;
     /* log_ptr->lna_state = RFM_LNA_INVALID_GAIN_STATE */
    }

    log_ptr->tx_enabled = ( (hdrmod_pa_is_on() == TRUE)? 1: 0 );
      /* State of the power amplifier  0 – PA Off 1 – PA On */

    log_ptr->hstr = HDRHSTR_GET_COUNT();
      /* Current half-slot reference count */

    if (agc0_is_valid)
    {
      log_ptr->total_tx_power =
        rfm_hdr_get_tx_total_power ( hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), 
                                     RFM_HDR_CARRIER_ID__CUMULATIVE, 
                                     RFM_HDR_AGC_FORMAT__DBM256 );
        /* Total Tx power over all the carriers */
    }
    else
    {
     /* RF tuning is not done yet!Logging default values */
      log_ptr->total_tx_power =0xFFFF;
    }

    log_ptr->num_carriers = num_carriers;
      /* Number of carriers in the log packet */    

    perc_pwr_log_ptr = &log_ptr->per_carrier_power_info[0];
      /* Initialize the convenience pointer to per carrier information */ 

    for ( saset_idx = 0; saset_idx < HDRSRCH_MAX_NUM_SUBASETS; saset_idx++ )
    {
      if ( saset_in_use [saset_idx] )
      {
        demod_idx = saset_info[saset_idx].demod_idx;

        perc_pwr_log_ptr->carrier_id.channel_id = demod_idx;
        perc_pwr_log_ptr->carrier_id.channel_no = 
          saset_info[saset_idx].channel.chan_num;
        perc_pwr_log_ptr->carrier_id.band_class = 
          saset_info[saset_idx].channel.band;

        hdrrxstats_gather_mc_power_data( 
          perc_pwr_log_ptr, demod_idx, agc0_is_valid, agc1_is_valid );

        perc_pwr_log_ptr++;
          /* Move to the next per-carrier Power information */
      }

    } /* for each demod carrier */

    log_commit( (log_type*) log_ptr );

  } /* log_ptr != NULL */


} /* hdrrxstats_log_mc_rf_power */

/*============================================================================
  HDRRXSTATS_RESET_AVERAGE_PER
*/
/**
  Resets the average PER sample bins (used by this module).

  @context HDRRX task. */
/*============================================================================*/
LOCAL void hdrrxstats_reset_average_per( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  memset (
    (void*) &hdrrxstats.average_per[0][0], 0, sizeof(hdrrxstats.average_per) );
    /* Clear up average per statistics. */

} /* hdrrxstats_reset_average_per */

/*============================================================================
  HDRRXSTATS_LOG_REL_0_FWD_STATS
*/
/**
  This function logs EVDO Release 0 forward link statistics:
  - ftc slot count
  - Rx (forward and control channel) rate CRC count

  @context HDRRX task. */
/*============================================================================*/
LOCAL void hdrrxstats_log_rel_0_fwd_stats( void )
{
  LOG_HDR_FWD_STATS_SUMMARY_C_type *log_ptr;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  log_ptr = (LOG_HDR_FWD_STATS_SUMMARY_C_type*)
            log_alloc ( LOG_HDR_FWD_STATS_SUMMARY_C, 
                        sizeof(LOG_HDR_FWD_STATS_SUMMARY_C_type) );

  if ( log_ptr != NULL )
  {

    log_ptr->seq_num = hdrrxstats.sequence_num;

    /*-------------------------------------------------------------------------
      Lint Error 420: Apparent access beyond array for function memcpy
      Lint Error 419: Apparent data overrun for function memcpy.
        The memcpy calls here are used to copy several arrays of the early 
        decode and crc stats for all the drc's starting from drc1. For 
        instance, the array size of drc1 is only 64 bytes for cc_early_decode.
        However, we are copying the stats for both drc1 and drc2 here. Both the
        source and destination arrays are smaller than the data being copied 
        leading to errors 419 and 420 respectively. The only way to get rid of
        these Lint errors is to use several memcpy calls instead of one. Since 
        we know for sure that the arrays are in consecutive memory areas, we 
        can safely ignore these lint errors.
     ------------------------------------------------------------------------*/
    /*lint -save -e420 -e419 */ 
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy(
     (void*)&(log_ptr->cc_rate_slt_cnt), sizeof(log_ptr->cc_rate_slt_cnt), 
     (void*)&(hdrrxstats.std_cc_early_decode[HDRMDSP_DEMOD_CARRIER_0].drc1[0]),
     FSIZ(LOG_HDR_FWD_STATS_SUMMARY_C_type, cc_rate_slt_cnt));

    memscpy((void*)&(log_ptr->tc_rate_slt_cnt), 
            sizeof(log_ptr->tc_rate_slt_cnt), 
           (void*)&(hdrrxstats.rel_0_tc_early_decode.drc1[0]), 
           FSIZ(LOG_HDR_FWD_STATS_SUMMARY_C_type, tc_rate_slt_cnt));

    memscpy((void*)&(log_ptr->rate_stats.cc38400_good), 
           (sizeof(hdrrxstats_crc_type) * HDRRXSTATS_REL_0_CC_DRC_NUM),
           (void*)&(hdrrxstats.std_cc_crc[HDRMDSP_DEMOD_CARRIER_0][0]),
           (sizeof(hdrrxstats_crc_type) * HDRRXSTATS_REL_0_CC_DRC_NUM));

    memscpy((void*)&(log_ptr->rate_stats.tc38400_good), 
           (sizeof(hdrrxstats_crc_type) * HDRRXSTATS_REL_0_TC_DRC_NUM),
           (void*)&(hdrrxstats.rel_0_tc_crc[1]),
           (sizeof(hdrrxstats_crc_type) * HDRRXSTATS_REL_0_TC_DRC_NUM));
#else
    memcpy(
      (void*)&(log_ptr->cc_rate_slt_cnt), 
      (void*)&(hdrrxstats.std_cc_early_decode[HDRMDSP_DEMOD_CARRIER_0].drc1[0]),
      FSIZ(LOG_HDR_FWD_STATS_SUMMARY_C_type, cc_rate_slt_cnt));

    memcpy((void*)&(log_ptr->tc_rate_slt_cnt), 
           (void*)&(hdrrxstats.rel_0_tc_early_decode.drc1[0]), 
           FSIZ(LOG_HDR_FWD_STATS_SUMMARY_C_type, tc_rate_slt_cnt));

    memcpy((void*)&(log_ptr->rate_stats.cc38400_good),
           (void*)&(hdrrxstats.std_cc_crc[HDRMDSP_DEMOD_CARRIER_0][0]),
           (sizeof(hdrrxstats_crc_type) * HDRRXSTATS_REL_0_CC_DRC_NUM));

    memcpy((void*)&(log_ptr->rate_stats.tc38400_good),
           (void*)&(hdrrxstats.rel_0_tc_crc[1]),
           (sizeof(hdrrxstats_crc_type) * HDRRXSTATS_REL_0_TC_DRC_NUM));
#endif /* FEATURE_MEMCPY_REMOVAL */
           
    /*lint -restore */
         
    log_commit ( (log_type*)log_ptr );
  }

} /* hdrrxstats_log_rel_0_fwd_stats */


#ifdef FEATURE_HDR_REVA_L1

/*============================================================================
  HDRRXSTATS_LOG_REV_A_SU_FWD_STATS
*/
/**
  Logs forward link statistics of CRC and early decode for Revision A packets.
    - Rev-A FTC early termination slot counts.
    - Rev-A Rx (forward and control channel) rate CRC count

  @context HDRRX task. */
/*============================================================================*/
LOCAL void hdrrxstats_log_rev_a_su_fwd_stats ( void )
{
  LOG_HDR_REVA_SU_FWD_STATS_C_type *log_ptr;
    /* pointer to fwd stats log packet */

  uint8 demod_idx = 0; //hdrfmac_get_rel_0_a_demod_index();
    /* Get demod index */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Sanity check */
  ASSERT (demod_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT);

  /* Allocate memory for log packet */
  log_ptr = (LOG_HDR_REVA_SU_FWD_STATS_C_type*)
              log_alloc( LOG_HDR_REVA_SU_FWD_STATS_C,
                         sizeof ( LOG_HDR_REVA_SU_FWD_STATS_C_type ) );

  /* If memory allocation hasn't failed or if log code not enabled.
     Note: Failed log_alloc doesn't always mean failed memory allocation.
     log_alloc could fail if the log code is not enabled too.
     log_alloc calls log_status internally */  
  if ( log_ptr != NULL )
  {
    /* Record the sequence number of log packet */
    log_ptr->log_seq_num = hdrrxstats.sequence_num;

#ifdef FEATURE_MEMCPY_REMOVAL
    /* Copy traffic channel CRC stats */
    log_ptr->tc_crc = hdrrxstats.mc_tc_crc[0];

    /* Copy traffic channel early decode stats */ 
    log_ptr->tc_early_decode = hdrrxstats.mc_tc_early_decode[0];

    /* Copy control channel short packet CRC stats */
    log_ptr->cc_short_crc = hdrrxstats.short_cc_crc[demod_idx];
  
    /* Copy control channel short packet early decode stats */
    log_ptr->cc_short_early_decode = hdrrxstats.short_cc_early_decode[demod_idx];
    
    /* Copy control channel standard packet CRC stats */
    memscpy ( ( void* ) &( log_ptr->cc_std_crc ), 
              sizeof(hdrlog_cc_crc_stats_type ), 
              ( void* ) &( hdrrxstats.std_cc_crc[demod_idx][0]),
              ( sizeof( hdrrxstats_crc_type ) * HDRRXSTATS_REL_0_CC_DRC_NUM ) );
  
    /* Copy control channel standard packet early decode stats */
    memscpy ( ( void* ) &( log_ptr->cc_std_early_decode ), 
              sizeof(hdrlog_cc_early_decode_type ), 
              ( void* ) &( hdrrxstats.std_cc_early_decode[demod_idx] ),
              sizeof ( hdrlog_cc_early_decode_type ) );
#else
    /* Copy traffic channel CRC stats */
    memcpy( ( void* ) &(log_ptr->tc_crc),
            ( void* ) &(hdrrxstats.mc_tc_crc[0]),
            sizeof ( hdrlog_rev_a_tc_crc_stats_type ) );

    /* Copy traffic channel early decode stats */ 
    memcpy( ( void* ) &(log_ptr->tc_early_decode),
            ( void* ) &(hdrrxstats.mc_tc_early_decode[0]),
            sizeof ( hdrlog_rev_a_tc_early_decode_type ) );

    /* Copy control channel short packet CRC stats */
    memcpy( ( void* ) &(log_ptr->cc_short_crc),
            ( void* ) &(hdrrxstats.short_cc_crc[demod_idx]),
            sizeof ( hdrlog_rev_a_short_cc_crc_stats_type ) );
  
    /* Copy control channel short packet early decode stats */
    memcpy( 
      ( void* ) &(log_ptr->cc_short_early_decode), 
      ( void* ) &(hdrrxstats.short_cc_early_decode[demod_idx]),
      sizeof ( hdrlog_rev_a_short_cc_early_decode_type ) );
    
    /* Copy control channel standard packet CRC stats */
    memcpy ( ( void* ) &( log_ptr->cc_std_crc ),
             ( void* ) &( hdrrxstats.std_cc_crc[demod_idx][0]),
             sizeof ( hdrlog_cc_crc_stats_type  ) );
  
    /* Copy control channel standard packet early decode stats */
    memcpy ( 
      ( void* ) &( log_ptr->cc_std_early_decode ), 
      ( void* ) &( hdrrxstats.std_cc_early_decode[demod_idx] ),
      sizeof ( hdrlog_cc_early_decode_type ) );
#endif /* FEATURE_MEMCPY_REMOVAL */


    HDR_MSG_PROT (MSG_LEGACY_LOW, "Logging Rev-A SU pkt fwd Stats");

    log_commit((log_type*)log_ptr);

  }

} /* hdrrxstats_log_rev_a_su_fwd_stats */


/*============================================================================
  HDRRXSTATS_LOG_MC_SUP_FWD_STATS
*/
/**
  Logs forward link statistics of CRC and early decode for Multi-carrier 
  packets.
    - FTC / CC early termination slot counts.
    - Rx (forward and control channel) rate CRC count

  @context HDRRX task. */
/*============================================================================*/
LOCAL void hdrrxstats_log_mc_sup_fwd_stats(void)
{

  uint8 demod_idx;
    /* Demod Index */

  hdrsrch_subaset_idx_type saset_idx;
    /* Sub ASET Index */

  uint8 num_carriers = 0;
    /* Number of carriers */

  uint32 log_pkt_size = 0;
    /* Size of the log packet */

  LOG_HDR_MC_SUP_FWD_STATS_C_type *log_ptr = NULL;
    /* Pointer to allocated log buffer */

  hdrsrch_fl_carrier_info_type saset_info;
    /* Demod carrier Information */

  boolean tc_stats_ready [HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /* Working variable */

  boolean cc_stats_ready [HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /* Working variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  /* Make a local copy so that an update to hdrrxstats.[tc/cc]_stats_ready 
     between the two subsequent reads doesn't confuse the logging routine. */
#ifdef FEATURE_MEMCPY_REMOVAL
  memscpy ( (uint8*) &tc_stats_ready[0], sizeof(tc_stats_ready), 
            (uint8*) &hdrrxstats.tc_stats_ready[0],
            sizeof(hdrrxstats.tc_stats_ready) );

  memscpy ( (uint8*) &cc_stats_ready[0], sizeof(cc_stats_ready), 
            (uint8*) &hdrrxstats.cc_stats_ready[0],
            sizeof(hdrrxstats.cc_stats_ready) );
#else
  memcpy ( (uint8*) &tc_stats_ready[0], 
           (uint8*) &hdrrxstats.tc_stats_ready[0],
           (sizeof(boolean) * HDRSRCH_MAX_DEMOD_CARRIERS_CNT) );

  memcpy ( (uint8*) &cc_stats_ready[0], 
           (uint8*) &hdrrxstats.cc_stats_ready[0],
           (sizeof(boolean) * HDRSRCH_MAX_DEMOD_CARRIERS_CNT) );
#endif /* FEATURE_MEMCPY_REMOVAL */

  /* Find Number of demod carriers with valid statistics for logging.
     This needs to change if the HDRSRCH_MAX_DEMOD_CARRIERS_CNT change
     (unlikely). This is more efficient than looping around. */
  num_carriers = ( ((tc_stats_ready[0])? 1: 0) +
                   ((tc_stats_ready[1])? 1: 0) +
                   ((tc_stats_ready[2])? 1: 0) );

  /* for each demod carrier */
  for (saset_idx = 0; saset_idx < HDRSRCH_MAX_NUM_SUBASETS; ++saset_idx)
  {
    /* Get the FL carrier information. THe API returns true only if the 
       demod is in use. */
    if ( hdrsrch_get_fl_carrier_info( saset_idx, &saset_info ) )
    {
      demod_idx = saset_info.demod_idx;

    /* If the demod carrier has valid stats to log */
    if (tc_stats_ready[demod_idx])
    {
      /* Find the size of the log packet */
      log_pkt_size = sizeof(LOG_HDR_MC_SUP_FWD_STATS_C_type);

      /* If this carrier does not have any CC stats */
      if (!cc_stats_ready[demod_idx])
      { 
        /* Exclude the CC stats size from log packet size */
        log_pkt_size -= sizeof(hdrlog_cc_stats_type);
      }

      /* Allocate log buffer */
      log_ptr = (LOG_HDR_MC_SUP_FWD_STATS_C_type*)
                  log_alloc_ex(LOG_HDR_MC_SUP_FWD_STATS_C, log_pkt_size);

      /* If the log buffer allocation succeeds (could fail if the log code 
         is not enabled) */
      if (log_ptr != NULL)
      {
        /* Version number */
        log_ptr->version = HDRLOG_MC_SUP_FWD_STATS_VERSION;

        /* Store the sequence number */
        log_ptr->log_seq_num = hdrrxstats.sequence_num; 

        /* Store the number of carriers */
        log_ptr->num_carriers = num_carriers;

        /* Store the carrier id in the record packet
           This information is pulled from SRCH every second.
           TODO ETV Is there a better way to do this? */
        log_ptr->perc_fwd_stats.carrier_id.channel_id = demod_idx;
        log_ptr->perc_fwd_stats.carrier_id.channel_no = 
            saset_info.channel.chan_num;
        log_ptr->perc_fwd_stats.carrier_id.band_class = 
            saset_info.channel.band;

#ifdef FEATURE_MEMCPY_REMOVAL
        /* Copy traffic channel CRC stats */
        log_ptr->perc_fwd_stats.tc.crc = hdrrxstats.mc_tc_crc[demod_idx];

        /* Copy traffic channel early decode stats */
        log_ptr->perc_fwd_stats.tc.early_decode = 
                                    hdrrxstats.mc_tc_early_decode[demod_idx];
#else
        /* Copy traffic channel CRC stats */
        memcpy( ( void* ) &(log_ptr->perc_fwd_stats.tc.crc),
                ( void* ) &(hdrrxstats.mc_tc_crc[demod_idx]),
                sizeof ( hdrlog_rev_a_tc_crc_stats_type ) );

        /* Copy traffic channel early decode stats */
        memcpy( ( void* ) &(log_ptr->perc_fwd_stats.tc.early_decode),
                ( void* ) &(hdrrxstats.mc_tc_early_decode[demod_idx]),
                sizeof ( hdrlog_rev_a_tc_early_decode_type ) );
#endif /* FEATURE_MEMCPY_REMOVAL */

#ifdef FEATURE_HDR_REVB_ENH_RATES

#ifdef FEATURE_MEMCPY_REMOVAL
        /* Copy traffic channel enhanced DRCs' CRC stats */
        log_ptr->perc_fwd_stats.tc.enh_crc = 
                                   hdrrxstats.mc_enh_tc_crc[demod_idx];

        /* Copy traffic channel enhanced DRCs' early decode stats */
        log_ptr->perc_fwd_stats.tc.enh_early_decode = 
                                   hdrrxstats.mc_enh_tc_early_decode[demod_idx];
#else
        /* Copy traffic channel enhanced DRCs' CRC stats */
        memcpy( ( void* ) &(log_ptr->perc_fwd_stats.tc.enh_crc),
                ( void* ) &(hdrrxstats.mc_enh_tc_crc[demod_idx]),
                sizeof ( hdrlog_rev_b_enhanced_tc_rates_crc_stats_type ) );

        /* Copy traffic channel enhanced DRCs' early decode stats */
        memcpy( ( void* ) &(log_ptr->perc_fwd_stats.tc.enh_early_decode),
                ( void* ) &(hdrrxstats.mc_enh_tc_early_decode[demod_idx]),
                sizeof ( hdrlog_rev_b_enhanced_tc_rates_early_decode_type ) );
#endif /* FEATURE_MEMCPY_REMOVAL */

#endif /* FEATURE_HDR_REVB_ENH_RATES */

        /* Does the log packet include CC statistics for this carrier */
        log_ptr->perc_fwd_stats.cc_stats_included = 
          cc_stats_ready[demod_idx];

        if (cc_stats_ready[demod_idx])
        {

#ifdef FEATURE_MEMCPY_REMOVAL
          /* Copy control channel short packet CRC stats */
          log_ptr->perc_fwd_stats.cc.short_crc = 
                                  hdrrxstats.short_cc_crc[demod_idx];

          /* Copy control channel short packet early decode stats */

          log_ptr->perc_fwd_stats.cc.short_early_decode = 
                                  hdrrxstats.short_cc_early_decode[demod_idx];

          /* Copy control channel standard packet CRC stats */
          memscpy ( ( void* ) &( log_ptr->perc_fwd_stats.cc.std_crc ),
                   sizeof(hdrlog_cc_crc_stats_type),
                    ( void* ) &( hdrrxstats.std_cc_crc[demod_idx][0] ),
                   sizeof ( hdrlog_cc_crc_stats_type  ) );

          /* Copy control channel standard packet early decode stats */
          memscpy ( 
            ( void* ) &( log_ptr->perc_fwd_stats.cc.std_early_decode ),
              sizeof(hdrlog_cc_early_decode_type), 
            ( void* ) &( hdrrxstats.std_cc_early_decode[demod_idx] ),
              sizeof ( hdrlog_cc_early_decode_type ) );
#else
          /* Copy control channel short packet CRC stats */
          memcpy( ( void* ) &(log_ptr->perc_fwd_stats.cc.short_crc),
                  ( void* ) &(hdrrxstats.short_cc_crc[demod_idx]),
                  sizeof ( hdrlog_rev_a_short_cc_crc_stats_type ) );

          /* Copy control channel short packet early decode stats */
          memcpy( 
            ( void* ) &(log_ptr->perc_fwd_stats.cc.short_early_decode),
            ( void* ) &(hdrrxstats.short_cc_early_decode[demod_idx]),
            sizeof ( hdrlog_rev_a_short_cc_early_decode_type ) );

          /* Copy control channel standard packet CRC stats */
          memcpy ( ( void* ) &( log_ptr->perc_fwd_stats.cc.std_crc ),
                   ( void* ) &( hdrrxstats.std_cc_crc[demod_idx][0] ),
                   sizeof ( hdrlog_cc_crc_stats_type  ) );

          /* Copy control channel standard packet early decode stats */
          memcpy ( 
            ( void* ) &( log_ptr->perc_fwd_stats.cc.std_early_decode ),
            ( void* ) &( hdrrxstats.std_cc_early_decode[demod_idx] ),
            sizeof ( hdrlog_cc_early_decode_type ) );
#endif /* FEATURE_MEMCPY_REMOVAL */

        } /* If CC stats ready for this carrier */

        log_commit((log_type*)log_ptr);

      } /* if (log_ptr) */

    } /* If TC stats ready for this carrier */

    } /* if demod is in use */

  } /* for each demod carrier */

  HDR_MSG_PROT_1(MSG_LEGACY_LOW, "Logged MC fwd stats for %d carriers",
                 num_carriers);

} /* hdrrxstats_log_mc_sup_fwd_stats */


/*============================================================================
  HDRRXSTATS_LOG_REV_A_MU_FWD_STATS
*/
/**
  Logs forward link statistics of CRC and early decode for Revision A 
  Multi-user packets.
    - Rev-A FTC early termination slot counts.
    - Rev-A Rx (forward and control channel) rate CRC count

  @context HDRRX task. */
/*============================================================================*/
LOCAL void hdrrxstats_log_rev_a_mu_fwd_stats ( void )
{
  LOG_HDR_REVA_MU_FWD_STATS_C_type *log_ptr;
    /* pointer to fwd stats log packet */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  log_ptr = (LOG_HDR_REVA_MU_FWD_STATS_C_type*)
              log_alloc( LOG_HDR_REVA_MU_FWD_STATS_C,
                         sizeof ( LOG_HDR_REVA_MU_FWD_STATS_C_type ) );
    /* Allocate memory for log packet */

  /* If memory allocation hasn't failed or if log code not enabled.
     Note: Failed log_alloc doesn't always mean failed memory allocation.
     log_alloc could fail if the log code is not enabled too.
     log_alloc calls log_status internally */  
  if ( log_ptr != NULL )
  {
    log_ptr->log_seq_num = hdrrxstats.sequence_num;
      /* Record the sequence number of log packet */
                                                   
#ifdef FEATURE_MEMCPY_REMOVAL
    log_ptr->crc = hdrrxstats.mc_mup_crc[0];
      /* Copy Multi-user packet CRC stats */

    log_ptr->early_decode = hdrrxstats.mc_mup_early_decode[0];
      /* Copy Multi-user packet early decode statistics */
#else
    memcpy( ( void* ) &(log_ptr->crc),
            ( void* ) &(hdrrxstats.mc_mup_crc[0]),
            sizeof ( hdrlog_mup_crc_stats_type ) );
      /* Copy Multi-user packet CRC stats */

    memcpy( ( void* ) &(log_ptr->early_decode),
            ( void* ) &(hdrrxstats.mc_mup_early_decode[0]),
            sizeof ( hdrlog_mup_early_decode_type ) );
      /* Copy Multi-user packet early decode statistics */
#endif /* FEATURE_MEMCPY_REMOVAL */
    
    HDR_MSG_PROT (MSG_LEGACY_LOW, "Logging RevA MU pkt fwd Stats");

    log_commit((log_type*)log_ptr);

  }

} /* hdrrxstats_log_rev_a_mu_fwd_stats */

#ifdef FEATURE_HDR_REVB 
/*============================================================================
  HDRRXSTATS_LOG_MC_MUP_FWD_STATS
*/
/**
  Logs forward link statistics of CRC and early decode for Multi-carrier
  Multi-user packets.
    - MUP early termination slot counts.
    - CRC count

  @context HDRRX task. */
/*============================================================================*/
LOCAL void hdrrxstats_log_mc_mup_fwd_stats ( void )
{
  LOG_HDR_MC_MUP_FWD_STATS_C_type *log_ptr;
    /* pointer to fwd stats log packet */

  uint8 num_carriers = 0;
    /* Number of demod carriers with valid MUP stats for logging */
  
  uint8 demod_idx;
    /* Demod carrier index */

  hdrlog_perc_mup_fwd_stats_type *perc_mup_ptr;
    /* Convenience pointer */

  hdrsrch_fl_carrier_info_type saset_info;
    /* Forward link carrier Info */

  boolean mup_stats_ready [HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /* Working variable */

  hdrsrch_subaset_idx_type saset_idx;
    /* Sub ASET index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  /* Make a local copy so that an update to hdrrxstats.mup_stats_ready between
     the two subsequent reads doesn't confuse the logging routine. */
#ifdef FEATURE_MEMCPY_REMOVAL
  memscpy ( (uint8*) &mup_stats_ready[0], sizeof(mup_stats_ready), 
            (uint8*) &hdrrxstats.mup_stats_ready[0],
            sizeof(hdrrxstats.mup_stats_ready) );
#else
  memcpy ( (uint8*) &mup_stats_ready[0], 
           (uint8*) &hdrrxstats.mup_stats_ready[0],
           (sizeof(boolean) * HDRSRCH_MAX_DEMOD_CARRIERS_CNT) );
#endif /* FEATURE_MEMCPY_REMOVAL */

  /* Find Number of demod carriers with valid statistics for logging.
     This needs to change if the HDRSRCH_MAX_DEMOD_CARRIERS_CNT change
     (unlikely). This is more efficient than looping around. */
  num_carriers = ( ((mup_stats_ready[0])? 1: 0) +
                   ((mup_stats_ready[1])? 1: 0) +
                   ((mup_stats_ready[2])? 1: 0) );

  if (num_carriers > 0)
  {
    /* Allocate memory for log packet */
    log_ptr = (LOG_HDR_MC_MUP_FWD_STATS_C_type*)
      log_alloc( LOG_HDR_MC_MUP_FWD_STATS_C, 
                 ( sizeof ( LOG_HDR_MC_MUP_FWD_STATS_C_type ) + 
                   ( (num_carriers - 1) * 
                     sizeof (hdrlog_perc_mup_fwd_stats_type) ) ) );

    /* If memory allocation hasn't failed or if log code not enabled.
       Note: Failed log_alloc doesn't always mean failed memory allocation.
       log_alloc could fail if the log code is not enabled too.
       log_alloc calls log_status internally */  
    if (log_ptr != NULL)
    {
      log_ptr->version = HDRLOG_MC_MUP_FWD_STATS_LOG_VERSION;

      /* Record the sequence number of log packet */
      log_ptr->log_seq_num = hdrrxstats.sequence_num;

      log_ptr->num_carriers = num_carriers;

      perc_mup_ptr = &log_ptr->perc_mup_stats[0];

      /* for each demod carrier */
      for ( saset_idx = 0; saset_idx < HDRSRCH_MAX_NUM_SUBASETS; ++saset_idx )
      {
        /* Retrieve the carrier ID and store. The API returns TRUE only if the
           demod is in use. */
        if ( hdrsrch_get_fl_carrier_info ( saset_idx, &saset_info ) )
        {
          demod_idx = saset_info.demod_idx;

          if ( mup_stats_ready[demod_idx] )
          {
            perc_mup_ptr->carrier_id.channel_id = demod_idx;
              perc_mup_ptr->carrier_id.channel_no = saset_info.channel.chan_num;
              perc_mup_ptr->carrier_id.band_class = saset_info.channel.band;

#ifdef FEATURE_MEMCPY_REMOVAL
            /* Copy Multi-user packet CRC stats */
            perc_mup_ptr->crc = hdrrxstats.mc_mup_crc[demod_idx];

            /* Copy Multi-user packet early decode statistics */
            perc_mup_ptr->early_decode = hdrrxstats.mc_mup_early_decode[demod_idx];
#else
            /* Copy Multi-user packet CRC stats */
            memcpy( ( void* ) &(perc_mup_ptr->crc),
                    ( void* ) &(hdrrxstats.mc_mup_crc[demod_idx]),
                    sizeof ( hdrlog_mup_crc_stats_type ) );

            /* Copy Multi-user packet early decode statistics */
            memcpy( ( void* ) &(perc_mup_ptr->early_decode),
                    ( void* ) &(hdrrxstats.mc_mup_early_decode[demod_idx]),
                    sizeof ( hdrlog_mup_early_decode_type ) );
#endif /* FEATURE_MEMCPY_REMOVAL */

            /* Move to the beginning of next per-carrier statistics */
            perc_mup_ptr++;

          } /* If we have stats for this demod carrier */

        } /* If demod is in use */

      } /* for each sub aset */

      log_commit ( (log_type*) log_ptr);

    } /* If log_ptr != NULL */

  }
  
} /* hdrrxstats_log_mc_mup_fwd_stats */

#endif /* FEATURE_HDR_REVB */

#endif /* FEATURE_HDR_REVA_L1 */
/*============================================================================
  HDRRXSTATS_RESET_FWD_STATS
*/
/**
  This function resets all Rx rate statistics.

  @context HDRRX task. */
/*============================================================================*/
LOCAL void hdrrxstats_reset_fwd_stats ( void )
{
  memset ( (void*) &hdrrxstats, 0x00, sizeof(hdrrxstats) );

#ifdef FEATURE_HDR_REVA_L1 

  hdrfmacutil_reset_mu_user_stats_summ();
    /* Reset the multi-user packet User statistics */

#endif /* FEATURE_HDR_REVA_L1 */

} /* hdrrxstats_reset_fwd_stats */

/* <EJECT> */
#ifdef FEATURE_HDR_BCMCS
/*============================================================================
  HDRRXSTATS_LOG_BCC_STATS
*/
/**
  This function logs forward link statistics for broadcast packets.
  - BCC slot count
  - BCC rate CRC count

  @context HDRRX task. */
/*============================================================================*/
LOCAL void hdrrxstats_log_bcc_stats ( void )
{
  LOG_HDR_MCS_PKT_RATE_METRICS_C_type *log_ptr;

  uint8 demod_idx = 0; //hdrfmac_get_rel_0_a_demod_index();
    /* Demod index */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Sanity check */
  ASSERT (demod_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT);

  log_ptr = (LOG_HDR_MCS_PKT_RATE_METRICS_C_type*)
            log_alloc(LOG_HDR_MCS_PKT_RATE_METRICS_C,
            sizeof(LOG_HDR_MCS_PKT_RATE_METRICS_C_type));

  if (log_ptr != NULL)
  { 
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy((void*)&(log_ptr->cc_rate_stats), sizeof(log_ptr->cc_rate_stats), 
            (void*)&(hdrrxstats.std_cc_crc[demod_idx][0]),
            FSIZ(LOG_HDR_MCS_PKT_RATE_METRICS_C_type, cc_rate_stats));

    memscpy((void*)&(log_ptr->mcs_rate_slt_cnt), sizeof(log_ptr->mcs_rate_slt_cnt), 
            (void*)&(hdrrxstats.bcc_early_decode),
            FSIZ(LOG_HDR_MCS_PKT_RATE_METRICS_C_type, mcs_rate_slt_cnt));

    memscpy((void*)&(log_ptr->mcs_rate_stats), sizeof(log_ptr->mcs_rate_stats),
            (void*)&(hdrrxstats.bcc_crc[0]),
            FSIZ(LOG_HDR_MCS_PKT_RATE_METRICS_C_type, mcs_rate_stats));
#else
    memcpy((void*)&(log_ptr->cc_rate_stats),
           (void*)&(hdrrxstats.std_cc_crc[demod_idx][0]),
           FSIZ(LOG_HDR_MCS_PKT_RATE_METRICS_C_type, cc_rate_stats));

    memcpy((void*)&(log_ptr->mcs_rate_slt_cnt),
           (void*)&(hdrrxstats.bcc_early_decode),
           FSIZ(LOG_HDR_MCS_PKT_RATE_METRICS_C_type, mcs_rate_slt_cnt));

    memcpy((void*)&(log_ptr->mcs_rate_stats),
           (void*)&(hdrrxstats.bcc_crc[0]),
           FSIZ(LOG_HDR_MCS_PKT_RATE_METRICS_C_type, mcs_rate_stats));
#endif /* FEATURE_MEMCPY_REMOVAL */

    log_commit((log_type*)log_ptr);
  }

} /* hdrrxstats_log_bcc_stats */

#endif /* FEATURE_HDR_BCMCS */

/*============================================================================
  HDRRXSTATS_PERIODIC_LOGGING_CB
*/
/**
  This function is a clock callback function to control logging of the
  Rx and Tx rate counts.

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_periodic_logging_cb
( 
  int4 ms 
  /*lint -esym(715,ms)
   * Ignore the argument(ms) not used lint error #715 */ 
)
{
  static hdrrxstats_crc_type last_cum_phy_stats = {0,0};
    /* Phy pkt CRC cumulative until end of last period */

  uint8 slot = 0;
    /* Loop index */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


  hdrrxstats.sequence_num++;
    /* Increment the sequence number */

  /*-------------------------------------------------------------------------
                   LOG FORWARD/REVERSE LINK PACKET STATISTICS
  --------------------------------------------------------------------------*/

  hdrrxstats_log_rel_0_fwd_stats();
    /* Log forward link statistics */

#ifdef FEATURE_HDR_REVA_L1

  hdrrxstats_log_rev_a_su_fwd_stats();
    /* Log Rev-A Single-user packet Forward statistics */
  
  hdrrxstats_log_rev_a_mu_fwd_stats();
    /* Log Rev-A Multi-user packet forward statistics */

  hdrrxstats_log_mc_mup_fwd_stats();
    /* Log Multi-carrier MUP fwd statistics */

  hdrfmacutil_log_mu_user_stats_summ();
    /* Log Multi-user packet User statistics */

  hdrfmacutil_log_mc_mu_user_stats_summ();

  hdrrxstats_log_mc_sup_fwd_stats();

#endif /* FEATURE_HDR_REVA_L1 */

#ifdef FEATURE_HDR_BCMCS
  hdrrxstats_log_bcc_stats();
    /* Log broadcast packet statistics */
#endif

  hdrrmac_log_rtc_rate_count();
    /* Log reverse link packet count */

  /*-------------------------------------------------------------------------
                 PERIODIC (1sec) MAC CRC STATISTICS CALCULATION
  --------------------------------------------------------------------------*/

  /* Calculate periodic count */

  hdrrxstats.periodic_phy_stats.good_cnt = 
    (hdrrxstats.cum_phy_stats.good_cnt - last_cum_phy_stats.good_cnt);

  hdrrxstats.periodic_phy_stats.bad_cnt = 
    (hdrrxstats.cum_phy_stats.bad_cnt - last_cum_phy_stats.bad_cnt);

  /* Set last period's cumulative statistics to current cumulative stats */
  last_cum_phy_stats.good_cnt = hdrrxstats.cum_phy_stats.good_cnt;
  last_cum_phy_stats.bad_cnt = hdrrxstats.cum_phy_stats.bad_cnt;

  /* If the mobile received any packet (good/bad) during the last second, */
  if ((hdrrxstats.periodic_phy_stats.good_cnt != 0 ) ||
      (hdrrxstats.periodic_phy_stats.bad_cnt != 0) )
  {
    HDR_MSG_PROT_2 (MSG_LEGACY_MED, "FL Phy (1s): P: %d; F: %d",
                    hdrrxstats.periodic_phy_stats.good_cnt,
                    hdrrxstats.periodic_phy_stats.bad_cnt);
      /* Displays good phy pkts rx'd, bad pkts rx'd during last period (1s). */
  }

  /*-------------------------------------------------------------------------
                        UNEXPECTED STATISTICS DETECTION
  --------------------------------------------------------------------------*/

  /* Unexpected CRC detection */
  if (hdrrxstats.unexp_crc != 0)
  {
    HDR_MSG_PROT_1(MSG_LEGACY_ERROR,
                   "Recorded unexpected CRC in the past. CRC: %d",
                   hdrrxstats.unexp_crc);

    hdrrxstats.unexp_crc = 0;
      /* Reset the statistics */
  }

  /* Unexpected early decode detection */
  for (slot = 0; slot < 16; ++slot)
  {
    if (hdrrxstats.unexp_early_decode[slot] != 0)
    {
      HDR_MSG_PROT_2( MSG_LEGACY_ERROR, 
                      "Unexpected EarlyDecode for slot: %d count: %d",
                      slot, 
                      hdrrxstats.unexp_early_decode[slot]);

      hdrrxstats.unexp_early_decode[slot] = 0;
        /* Reset the statistics */
    }
  }

} /* hdrrxstats_periodic_logging_cb */

/*============================================================================
                        EXTERNAL FUNCTION DEFINITIONS
=============================================================================*/

/*============================================================================
  HDRRXSTATS_INIT
*/
/**
  This procedure initializes the statistics and the periodic logging timer on 
  powerup.

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_init ( void )
{

  timer_def2(&hdrrxstats_log_stat_timer, NULL);

  hdrrxstats_reset_fwd_stats();
    /* Reset frame rate statistics. */

  hdrrxstats_init_mc_fwd_stats_lut();
    /* Initialize Forward Statistics Look-up table */

  hdrrxstats.directed_slots = 0;
    /* Reset directed slots */

} /* hdrrxstats_init */


/*============================================================================
  HDRRXSTATS_ENTRY_PROCESSING
*/
/**
  This function performs Rx task entry processing related to statistics when 
  entering HDR mode.

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_entry_processing ( void )
{

  /* Enable call back for HDR Rx task logging */
  timer_reg(&hdrrxstats_log_stat_timer,
            (timer_t2_cb_type) hdrrxstats_periodic_logging_cb,
            (timer_cb_data_type) NULL,
            HDRRXSTATS_LOGGING_PERIOD_MS,
            HDRRXSTATS_LOGGING_PERIOD_MS);

  hdrrxstats_reset_average_per();
    /* Reset the average PER sample array. */

} /* hdrrxstats_entry_processing */

/*============================================================================
  HDRRXSTATS_EXIT_PROCESSING
*/
/**
  This function performs exit processing when exiting HDR mode.

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_exit_processing ( void )
{

  /* Remove call back for HDR Rx task logging */
  (void) timer_clr(&hdrrxstats_log_stat_timer, T_NONE);

} /* hdrrxstats_exit_processing */

/*============================================================================
  HDRRXSTATS_LOG_RX_DATA
*/
/**
  This function performs Rx logging that occurs on each frame (PN roll).

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_log_rx_data
( 
  boolean agc0_is_valid, 
    /**< Indicates whether RxAGC on primary chain is valid or not */
  boolean agc1_is_valid 
    /**< Indicates whether RxAGC on diversity chain is valid or not */
)
{

  /* Gather data and log power packet. */
  if (log_status(LOG_HDR_POWER_C))
  {
    hdrrxstats_log_rf_power( agc0_is_valid, agc1_is_valid );
  }

  /* Log decoder packet headers. */
  if (log_status(LOG_HDR_FL_PHY_PKT_HEADER_C))
  {
    hdrdec_log_pkt_headers(FALSE);
  }

  /* Gather data and log multi carrier power packet. */
  if (log_status(LOG_HDR_MC_POWER_C))
  {
    hdrrxstats_log_mc_rf_power( agc0_is_valid, agc1_is_valid );
  }

  hdrdec_ship_aged_log_buffer();
    /* Ship log buffers that are too old */

} /* hdrrxstats_log_rx_data */

/*============================================================================
  HDRRXSTATS_RESET_DM_FWD_STATS
*/
/**
  This function resets all RX rate statistics and sends a log packet to the DM.

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_reset_dm_fwd_stats ( void )
{
  hdrrxstats_reset_fwd_stats();
    /* reset fwd statistics */

  hdrrxstats_log_rel_0_fwd_stats();
    /* log forward link statistics */

#ifdef FEATURE_HDR_REVA_L1

  /* Log Rev-A forward link statistics */
  hdrrxstats_log_rev_a_su_fwd_stats();
  hdrrxstats_log_rev_a_mu_fwd_stats();

#ifdef FEATURE_HDR_BCMCS
  hdrrxstats_log_bcc_stats();
    /* Log broadcast statistics */
#endif /* FEATURE_HDR_BCMCS */

#endif /* FEATURE_HDR_REVA_L1 */

} /* hdrrxstats_reset_dm_fwd_stats */


/*============================================================================
  HDRRXSTATS_LOG_FL_PHY_LAYER_PKT
*/
/**
  This function logs the forward link physical packets received from the 
  decoder before sending it to upper layers. Each log packet contains exactly 
  one physical layer packet.

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_log_fl_phy_layer_pkt
(
  dsm_item_type *fl_phy_pkt_ptr
    /**< DSM item containing the forward link physical layer packet to be 
         logged */
)
{
  LOG_HDR_FL_PHY_LAYER_PKT_C_type *log_ptr = NULL; 
    /* Pointer to the log packet */

  hdrlog_fl_phy_layer_pkt_type *phy_pkt_ptr = NULL;
    /* Starting address physical layer packet in the array in 
     * log packet */

  uint16 phy_pkt_size = 0;
  /* Size of the physical layer packet */

  static uint16 fl_phy_layer_pkt_log_seq_num = 0;
    /* Sequence number of the FL PHY log packet. Needs to retain value
     * across function call. Hence declared static */

  uint32 pkt_payload_offset = 0;
    /* Array index into the byte array representing the packet */

  uint32 copy_size = 0;
    /* Number of bytes copied each time */

  uint32 bytes_left = 0;
    /* Number of bytes yet to be copied */

  uint8 pkt_size_ind = 0;
    /* packet size index got from packet size field */

#ifdef FEATURE_HDR_REVB
  hdrsrch_fl_carrier_info_type saset_info;
    /* Demod carrier Information */

  hdrsrch_subaset_idx_type saset_idx;
    /* Sub ASET Index */
#endif /* FEATURE_HDR_REVB */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (fl_phy_pkt_ptr == NULL)
  {
    HDR_MSG_PROT( MSG_LEGACY_MED,
        "hdrrxstats_log_fl_phy_layer_pkt - received NULL pointer");
    return;
  } 
  else
  {
    pkt_size_ind = HDRDSM_GET_PACKET_SIZE(fl_phy_pkt_ptr);
  }

  if ( pkt_size_ind < HDRRXSTATS_NUM_PKT_SIZES )
  {
    phy_pkt_size = 
      hdrrxstats_fl_pkt_sizes[ pkt_size_ind ];

    /*----------------------------------------------------------------------
     * Log packet size computation                                            
     * ===========================                                              
     * struct LOG_HDR_FL_PHY_LAYER_PKT_C_type                                  
     * {
     *    ...
     *    hdrlog_fl_phy_layer_pkt_type phy_layer_pkts [...]                    
     *    {
     *      ...                                                                 
     *      uint8 data [...]                                                    
     *    }
     * }
     * Each log packet has exactly one physical layer packet and each 
     * physical packet in turn has variable number of MAC packets
     *
     * The struct LOG_HDR_FL_PHY_LAYER_PKT_C_type has an array named 
     * phy_layer_pkts[1] which is intended to be the start of a variable
     * length array of physical packets and each phy_layer_pkt has an array 
     * named data[] which is again intended to be the starting byte of one or 
     * more MAC packets.  The LOG_HDR_FL_PHY_LAYER_PKT_C_type already includes 
     * one starting byte defined in the structure. Hence one byte less than the 
     * actual phy_pkt_size;
     *---------------------------------------------------------------------*/

    log_ptr = ( LOG_HDR_FL_PHY_LAYER_PKT_C_type* )
              log_alloc( 
                LOG_HDR_FL_PHY_LAYER_PKT_C, 
                (sizeof(LOG_HDR_FL_PHY_LAYER_PKT_C_type) + 
                 (phy_pkt_size - HDRLOG_MAX_FL_PKT_SIZE_BYTES)));
      /* Allocating the memory for log packet as a chunk of bytes*/

    /* If log_alloc hasn't failed */
    if ( log_ptr != NULL )
    {
      log_ptr->version = HDRLOG_FL_PHY_LAYER_PKT_LOG_VERSION;
        /* Record the version number */

      log_ptr->log_seq_num = ++fl_phy_layer_pkt_log_seq_num;
        /* Record the sequence number of log packet. */

      log_ptr->num_phy_layer_pkts = 1;
        /* Record the number of FL physical layer packets stored in the 
           current log pkt */

      log_ptr->hstr = HDRDSM_GET_TIMESTAMP( fl_phy_pkt_ptr );
        /* Record the packet timestamp */

      phy_pkt_ptr = &(log_ptr->phy_layer_pkts[0]);
        /* Initialize the convenience pointer */

#ifdef FEATURE_HDR_REVB
      phy_pkt_ptr->carrier_id.channel_id = 
        HDRDSM_GET_CARRIER_ID( fl_phy_pkt_ptr );
        /* Get Demod ID from the DSM item */

      /* Get the subaset index from packet's demod carrier index */
      saset_idx = 
        hdrsrch_get_subaset_index ( phy_pkt_ptr->carrier_id.channel_id );

      /* If the SubASET index is valid, get the Band class & Channel 
         Number information for this carrier. The hdrsrch API returns
         TRUE only if the demod carrier identifed by saset_idx is 
         in use */
      if ( ( saset_idx != HDRSRCH_INVALID_SUBASET_INDEX ) &&
           ( hdrsrch_get_fl_carrier_info ( saset_idx, &saset_info ) ) )
      {
      /* Store Channel number and band class */ 
        phy_pkt_ptr->carrier_id.channel_no = saset_info.channel.chan_num;
        phy_pkt_ptr->carrier_id.band_class = saset_info.channel.band;
      }
      else
      {
        /* This shouldn't happen. In case it does, setting explicit invalid
           values rather than random numbers */
        phy_pkt_ptr->carrier_id.channel_no = HDRLOG_INVALID_CHANNEL_NUM;
        phy_pkt_ptr->carrier_id.band_class = HDRLOG_INVALID_BAND_CLASS;
      }

#endif /* FEATURE_HDR_REVB */

      /* Recording the packet's physical layer revision */
      phy_pkt_ptr->pkt_info.pkt_rev = HDRDSM_IS_REV_A( fl_phy_pkt_ptr );

      /* Recording the physical layer packet length in units of 16-bytes */
      phy_pkt_ptr->pkt_info.pkt_len_in_16bytes = (phy_pkt_size / 16); 

      /* Initialize the working variable with the number of bytes left to be 
         copied */
      bytes_left = phy_pkt_size;

      /* Iterate through the DSM chain and log all MAC packets that belong
       * to the current physical packet. The second condition below is 
       * redundant. It is still safe to check... */
      while ((fl_phy_pkt_ptr != NULL) && (bytes_left > 0))
      {
        copy_size = ( ( bytes_left > HDRRXSTATS_PKT_SIZE )? 
                      HDRRXSTATS_PKT_SIZE: bytes_left );
          /* If the number of bytes left for copy is greater than the maximum
             bytes held by a DSM item, copy the max; else copy the bytes left */

        /* Copy the PHY packet content from DSM item to log pkt */
        /*lint -save -e669 The memcpy below copies multiple consecutive bytes
          in a single shot. It is more efficient that looping around. The
          statement above ensures that copy_size will be less than or equal
          to HDRRXSTATS_PKT_SIZE which is also the size of the data array in a 
          DSM item. So, it is safe to ignore this lint warning */
#ifdef FEATURE_MEMCPY_REMOVAL
        memscpy(
          (void*) &(phy_pkt_ptr->data[pkt_payload_offset]), 
          sizeof(phy_pkt_ptr->data), 
          (void*) (fl_phy_pkt_ptr->data_ptr),
          copy_size );
#else
        memcpy(
          (void*) &(phy_pkt_ptr->data[pkt_payload_offset]),
          (void*) (fl_phy_pkt_ptr->data_ptr),
          copy_size );
#endif /* FEATURE_MEMCPY_REMOVAL */
        /*lint -restore */

        pkt_payload_offset += copy_size;
          /* Shift the offset accordingly */

        bytes_left -= copy_size;
          /* Adjust the bytes left for next iteration */

        fl_phy_pkt_ptr = fl_phy_pkt_ptr->pkt_ptr;
          /* Move to DSM item of next MAC packet if there is one */

      } /* while not done */

      log_commit( (log_type*) log_ptr );

    } /* If log_ptr is not NULL */

  } /* pkt_size_ind < HDRRXSTATS_NUM_PKT_SIZES */
  
} /* hdrrxstats_log_fl_phy_layer_pkt */

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
void hdrrxstats_clear_bcc_stats ( void )
{
    
  memset( (void*)( &(hdrrxstats.bcc_early_decode) ), 
          0, 
          sizeof(hdrrxstats.bcc_early_decode) );

  memset( (void*)( &(hdrrxstats.bcc_crc[0]) ), 
          0, 
          sizeof(hdrrxstats.bcc_crc) );

} /* hdrrxstats_clear_bcc_stats */

#endif /* FEATURE_HDR_BCMCS */

/*============================================================================
  HDRRXSTATS_UPDATE_GOOD_CC_PKT_STATS
*/
/**
  This function is used to update good Control channel packet statistics

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_update_good_cc_pkt_stats
(
  hdrdec_pkt_info_s_type *pkt_info,
    /**< Pointer to decoder packet header information */
  uint8 pkt_size_index
    /**< Packet size index */
)
{
  uint8 demod_id = pkt_info->demod_id;

#ifdef FEATURE_HDR_REVA_L1 

  hdrrxstats_rev_a_pkt_stats_type const *pkt_stats = NULL;
    /* Convenience Pointer */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If the packet is a rev-A CC packet */
  if ( pkt_info->rev_a )
  {

    HDR_MSG_PROT_3( MSG_LEGACY_LOW,
                    "Good RevA CC pkt drc: %d decode: %d, slots: %d", 
                    pkt_info->drc_decoded, 
                    pkt_info->slots_to_decode, 
                    pkt_size_index );

    /* Error checking */
    if( pkt_size_index < 3 )
    {

      pkt_stats = 
        &hdrrxstats_short_cc_pkt_stats[demod_id][pkt_size_index];
        /* Initialize the convenience pointer */

      ++ (*(pkt_stats->good_crc_count));
        /* Update good CRC count */

      ++ (pkt_stats->early_decode[pkt_info->slots_to_decode - 1]);
        /* Update early decode count for this slot position */
    }
    else
    {
      HDR_MSG_PROT( MSG_LEGACY_ERROR,
                    "Bogus Rev-A CC packet size!");
    } 
  }
  /* else - if the packet is a release 0 CC packet */
  else
#endif /* FEATURE_HDR_REVA_L1 */
  {

    HDR_MSG_PROT_2( MSG_LEGACY_LOW, "Good Rel0 CC pkt drc: %d slots: %d", 
                    pkt_info->drc_decoded, 
                    pkt_info->slots_to_decode );

    /* Error checking */
    if ((pkt_info->drc_decoded <= (int) HDRHAI_DRC_76_8) &&
         (pkt_info->drc_decoded > (int) HDRHAI_DRC_NULL_RATE))
    {

      hdrrxstats.std_cc_crc
        [demod_id][pkt_info->drc_decoded-1].good_cnt++;
        /* Update good CRC count */

      ( hdrrxstats_rel_0_cc_decode_count[demod_id]
                                        [pkt_info->drc_decoded-1]
                                        [pkt_info->slots_to_decode-1] )++;
        /* Update early decode count for this slot position */
    }
    else
    {
      HDR_MSG_PROT( MSG_LEGACY_ERROR,
                    "Bogus drc rate for CC channel!");
    }

  }

  hdrrxstats.cc_stats_ready[demod_id] = TRUE;

  hdrrxstats.connected_state_per.total_cc_pkts++;
    /* Increment total CC packets for statistics */

  hdrrxstats.cum_phy_stats.good_cnt++;
    /* Increment the number of good packets received for periodic (1s) count 
       f3 message */

} /* hdrrxstats_update_good_cc_pkt_stats */


/*============================================================================
  HDRRXSTATS_UPDATE_BAD_CC_PKT_STATS
*/
/**
  This function is used to update bad Control channel packet statistics

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_update_bad_cc_pkt_stats
(
  hdrdec_pkt_info_s_type *pkt_info,
    /**< Pointer to decoder packet header information */
  uint8 dummy
    /**< Dummy variable to keep interface consistent */
)
{
  uint8 demod_id = pkt_info->demod_id;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  SYS_ARG_NOT_USED ( dummy );

#ifdef FEATURE_HDR_REVA_L1
  if ( pkt_info->rev_a )
  {
    HDR_MSG_PROT( MSG_LEGACY_LOW, "Bad Rev A CC short pkt");

    ++ (hdrrxstats.short_cc_crc[demod_id].bad_short_cc);
      /* Update bad CRC counts for Rev-A short control channel packet */
  }
  else
#endif /* FEATURE_HDR_REVA_L1 */
  {
    HDR_MSG_PROT_1( MSG_LEGACY_LOW, 
                    "Bad Rel 0 CC pkt. drc: %d", pkt_info->drc_decoded );

    hdrrxstats.std_cc_crc
      [demod_id][pkt_info->drc_decoded-1].bad_cnt++;
      /* Update bad CRC counts for Rel-0 standard CC packets */
  }

  hdrrxstats.cc_stats_ready[demod_id] = TRUE;

  hdrrxstats.per[demod_id].bad_pkts++;
    /* Increment bad packets for Average/Instantaneous PER statistics */
  
  hdrrxstats.connected_state_per.bad_cc_pkts++;
  hdrrxstats.connected_state_per.total_cc_pkts++;  
    /* Increment bad packets for traffic session PER statistics */

  hdrrxstats.cum_phy_stats.bad_cnt++;
    /* Increment the number of bad packets received for periodic (1s) count 
       f3 message*/

} /* hdrrxstats_update_bad_cc_pkt_stats */

/*============================================================================
  HDRRXSTATS_UPDATE_GOOD_TC_PKT_STATS
*/
/**
  This function is used to update good traffic channel packet statistics

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_update_good_tc_pkt_stats
(
  hdrdec_pkt_info_s_type *pkt_info,
    /**< Pointer to decoder packet header information */
  uint8 pkt_size_index
    /**< Packet size index */
)
{
#ifdef FEATURE_HDR_REVA_L1

  uint8 demod_id = pkt_info->demod_id;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Update statistics for rev-A TC packet */
  if ( pkt_info->rev_a )
  {

    switch (pkt_info->channel)
    {
      case HDRDEC_FTC_UNICAST:
      {
        (*(hdrrxstats_mc_tc_su_pkt_stats_lut
           [demod_id][pkt_info->drc_decoded][pkt_size_index].
           good_crc_count))++;
          /* Update traffic channel good CRC count statistics */

        hdrrxstats_mc_tc_su_pkt_stats_lut
          [demod_id][pkt_info->drc_decoded][pkt_size_index].
          early_decode[pkt_info->slots_to_decode-1]++;
          /* Fill in early slot decode data */

        hdrrxstats.tc_stats_ready[demod_id] = TRUE;
          /* Mark this demod carrier ready for logging TC stats */

        HDR_MSG_DEBUG_4( HDR_MSG_DBG_FTC_PKT_INFO,
          "Good Enhanced TC SU Carrier: %d pkt drc: %d decode: %d ,slots: %d ",
          demod_id, 
          pkt_info->drc_decoded, 
          pkt_info->slots_to_decode, 
          pkt_size_index ); 
        break;

      } /* Single-user packet */ 

      /* Multi-user packet */
      case HDRDEC_FTC_MUP:
      {
        (*(hdrrxstats_mc_mup_stats[demod_id][pkt_size_index].
            good_crc_count))++;
          /* Increment the count of good (CRC passed) Multi-user 
             packets by one */

        hdrrxstats_mc_mup_stats[demod_id][pkt_size_index].
          early_decode[pkt_info->slots_to_decode-1]++;
          /* Increment the count of Multi-user packet early terminated at
             slot slot_index by one */

        hdrrxstats.mup_stats_ready[demod_id]= TRUE;
          /* Mark this demod carrier ready for logging MUP stats */

        HDR_MSG_DEBUG_4( HDR_MSG_DBG_FTC_PKT_INFO,
          "Good MC TC MU pkt Carrier: %d drc: %d slots: %d ,size: %d ",
          demod_id, pkt_info->drc_decoded, 
          pkt_info->slots_to_decode, pkt_size_index );

        break;

      } /* Multi-user packet */

      default:
      {
        HDR_MSG_PROT_4( 
          MSG_LEGACY_ERROR, 
          "Unexpected packet type: %d, drc: %d slots: %d size: %d",
          pkt_info->pkt_type, pkt_info->drc_decoded, 
          pkt_info->slots_to_decode, pkt_size_index);
      }
    } /* switch (pkt_type) */

  } /* Rev-A */
  else  /* If the packet is a release-0 packet */
#endif /* FEATURE_HDR_REVA_L1 */
  {
    HDR_MSG_PROT_2( MSG_LEGACY_LOW, "Good Rel 0 TC pkt drc: %d slots: %d ", 
                    pkt_info->drc_decoded, pkt_info->slots_to_decode );

    hdrrxstats.rel_0_tc_crc[pkt_info->drc_decoded].good_cnt++;
      /* Update traffic channel good CRC count statistics */

    (hdrrxstats_rel_0_tc_decode_count
      [pkt_info->drc_decoded][pkt_info->slots_to_decode-1])++;
      /* Fill in early slot decode data */
  }

  hdrrxstats.connected_state_per.total_tc_pkts++;
    /* Increment total TC packets for statistics */

  hdrrxstats.cum_phy_stats.good_cnt++;
    /* Increment the number of good packets received for periodic (1s) count 
       f3 message*/

  hdrrxstats.directed_slots += pkt_info->slots_to_decode;
    /* Update slot count for all good TC packets */

} /* hdrrxstats_update_good_tc_pkt_stats */

/*============================================================================
  HDRRXSTATS_UPDATE_BAD_TC_PKT_STATS
*/
/**
  This function is used to update bad traffic channel packet statistics

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_update_bad_tc_pkt_stats
(
  hdrdec_pkt_info_s_type *pkt_info,
    /**< POinter to decoder packet header information */
  uint8 pkt_size_index
    /**< Packet size index */
)
{
  uint8 demod_id = pkt_info->demod_id;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDR_REVA_L1
  if ( pkt_info->rev_a )
  {
    switch (pkt_info->channel)
    {
      case HDRDEC_FTC_UNICAST:
      {

        ++ (*(hdrrxstats_mc_tc_su_pkt_bad_cnt_lut 
              [demod_id][pkt_info->drc_decoded]));

        hdrrxstats.tc_stats_ready[demod_id] = TRUE;
          /* Mark this demod carrier ready for logging TC stats */

        HDR_MSG_DEBUG_3( HDR_MSG_DBG_FTC_PKT_INFO, 
                         "Bad CRC: MC TC SU pkt. Carrier: %d DRC=%d hstr16=%d",
                         demod_id, 
                         pkt_info->drc_decoded, 
                         pkt_info->hstr);
        break;

      }  /* Single-user packet */

      /* Multi-user packet */
      case HDRDEC_FTC_MUP:
      {
        ++ ( *(hdrrxstats_mc_tc_mu_pkt_bad_cnt
                [demod_id][pkt_info->drc_decoded]) );
          /* Increment the count of bad Multi-user packet received at 
             DRC "drc_rate" by one */

        HDR_MSG_PROT_3( MSG_LEGACY_LOW,
                        "Bad CRC: RevA TC MU pkt. Ca: %d DRC=%d hstr16=%d",
                        demod_id,
                        pkt_info->drc_decoded, pkt_info->hstr);
        break;
      } /* Multi-user packet */

      default:
      {
        HDR_MSG_PROT_1(MSG_LEGACY_ERROR,
                       "Invalid traffic packet type: %d", pkt_info->channel);
      }

    } /* switch */

  } /* Rev-A */
  else
#endif  /* FEATURE_HDR_REVA_L1 */ 
  {
    hdrrxstats.rel_0_tc_crc[pkt_info->drc_decoded].bad_cnt++;
      /* Update bad CRC counts for Rel-0 Traffic channel packets */

    HDR_MSG_PROT_2( MSG_LEGACY_LOW, 
                    "Bad CRC: Rel 0 TC pkt. DRC=%d hstr16=%d", 
                    pkt_info->drc_decoded, pkt_info->hstr);
  }

  hdrrxstats.per[demod_id].bad_pkts++;
    /* Increment bad packets for Average/Instantaneous PER statistics */

  /* Increment bad packets for traffic session PER statistics */
  hdrrxstats.connected_state_per.bad_tc_pkts++;
  hdrrxstats.connected_state_per.total_tc_pkts++;

  hdrrxstats.cum_phy_stats.bad_cnt++;
    /* Increment the number of bad packets received for periodic (1s) count 
       f3 message*/

  /* Update slot count for all bad TC packets */
  hdrrxstats.directed_slots += pkt_info->slots_to_decode;

} /* hdrrxstats_update_bad_tc_pkt_stats */

#ifdef FEATURE_HDR_BCMCS
/*============================================================================
  HDRRXSTATS_UPDATE_GOOD_BCC_PKT_STATS
*/
/**
  This function is used to update good broadcast channel packet statistics

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_update_good_bcc_pkt_stats
(
  hdrdec_pkt_info_s_type *pkt_info,
    /**< Pointer to decoder packet header information */
  uint8 dummy
    /**< Dummy variable to keep the interface consistent */
)
{

  uint8 bcc_rate_lookup_idx;
   /* Index to look up bcc_rate in look-up table */

  uint16 bcc_rate;
  /* Broadcast rate */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  SYS_ARG_NOT_USED ( dummy );

  /* Obtain BCC rate look up index */
  if (pkt_info->pkt_type == HDRDEC_BCC_ENH_PKT)
  {
    bcc_rate_lookup_idx = pkt_info->drc_decoded + HDRRXSTATS_BCC_ENH_RATE_OFFSET;
  }
  else
  {
    bcc_rate_lookup_idx = pkt_info->drc_decoded;
  }

  if ( bcc_rate_lookup_idx < HDRRXSTATS_BCC_MAX_MAPPING_INDEX)
  {
    bcc_rate = hdrrxstats_bcc_rates[bcc_rate_lookup_idx];
    /* Get broadcast rate from DRC rate */

    hdrrxstats.bcc_crc[bcc_rate].good_cnt++;
    /* Update broadcast channel rate and good CRC count statistics */ 

    ((hdrrxstats_bcc_decode_count[bcc_rate])[pkt_info->slots_to_decode-1])++;
    /* Fill in early slot decode data */ 
  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_ERROR,
                    "Bogus drc rate for broadcast channel!");
  }  
} /* hdrrxstats_update_good_bcc_pkt_stats */

/*============================================================================
  HDRRXSTATS_UPDATE_BAD_BCC_PKT_STATS
*/
/**
  This function is used to update bad broadcast channel packet statistics

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_update_bad_bcc_pkt_stats
(
  hdrdec_pkt_info_s_type *pkt_info,
    /**< Pointer to decoder packet header information */
  uint8 dummy
    /**< Dummy variable to keep the interface consistent */
)
{
  uint8 bcc_rate_lookup_idx;
   /* Index to look up bcc_rate in look-up table */

  uint16 bcc_rate;
  /* Broadcast rate */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  SYS_ARG_NOT_USED ( dummy );

  /* Obtain BCC rate look up index */
  if (pkt_info->pkt_type == HDRDEC_BCC_ENH_PKT)
  {
    bcc_rate_lookup_idx = pkt_info->drc_decoded + HDRRXSTATS_BCC_ENH_RATE_OFFSET;
  }
  else
  {
    bcc_rate_lookup_idx = pkt_info->drc_decoded;
  }

  if ( bcc_rate_lookup_idx < HDRRXSTATS_BCC_MAX_MAPPING_INDEX)
  {
    bcc_rate = hdrrxstats_bcc_rates[bcc_rate_lookup_idx];
    /* Get broadcast rate from DRC rate */

    hdrrxstats.bcc_crc[bcc_rate].bad_cnt++;
    /* Update broadcast channel rate and good CRC count statistics */
  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_ERROR,
                    "Bogus drc rate for broadcast channel!");
  } 
} /* hdrrxstats_update_bad_bcc_pkt_stats */

#endif /* FEATURE_HDR_BCMCS */

/*============================================================================
  HDRRXSTATS_UPDATE_PER_STATS
*/
/**
  This function is used to update Packet Error Rate Statistics

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_update_per_stats 
(
  uint8 demod_id
    /**< Demod carrier on which the packet was received */
)
{
  int seq;
    /* PER Bin sequence number */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrrxstats.per[demod_id].total_pkts++;
    /* Increment the number of decoder packets received */

  /* If the total number of decoder packets received since last update is 
     equal to the PER bin sample size, */
  if ( hdrrxstats.per[demod_id].total_pkts == 
       HDRRXSTATS_PER_SAMPLE_SIZE )
  {
    seq = ( hdrrxstats.per[demod_id].sequence % 
            HDRRXSTATS_NUM_AVERAGE_PER_BINS );
      /* Find the sequence number of the current bin */

    /* Update Average PER statistics */
    hdrrxstats.average_per[demod_id][seq].bad_pkts = 
      (uint16) hdrrxstats.per[demod_id].bad_pkts;
    hdrrxstats.average_per[demod_id][seq].valid = TRUE;

    /* Update Instantaneous PER statistics */
    hdrrxstats.per[demod_id].instant = 
      (uint16) (hdrrxstats.per[demod_id].bad_pkts);
    hdrrxstats.per[demod_id].sequence++;

    /* Reset the PER stats */
    hdrrxstats.per[demod_id].bad_pkts = 0;
    hdrrxstats.per[demod_id].total_pkts = 0;
  }

} /* hdrrxstats_update_per_stats */

/*============================================================================
  HDRRXSTATS_GET_AVERAGE_PER
*/
/**
  Function to access average packet error rate.  The supplied structure
  is filled in with the measured sample size and the number of those packets
  that were in error.

  @context HDRRX task. */
/*============================================================================*/
void hdrrxstats_get_average_per
(
  uint16 *total,
    /**< Total number of packets */
  uint16 *errors
    /**< Number of bad packets */
)
{
  int    bin_idx;

  uint8 demod_idx = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Initialize the input pointer variables */
  (*total) = 0;
  (*errors) = 0;

  for (demod_idx = 0; demod_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; ++demod_idx)
  {
    /* Sum up bad packet counts. */
    for (bin_idx = 0; bin_idx < HDRRXSTATS_NUM_AVERAGE_PER_BINS; bin_idx++)
    {
      if (hdrrxstats.average_per[demod_idx][bin_idx].valid)
      {
        (*total) += HDRRXSTATS_PER_SAMPLE_SIZE;
        (*errors) += hdrrxstats.average_per[demod_idx][bin_idx].bad_pkts;
      }
    }

  } /* for each demod carrier */

} /* hdrrxstats_get_average_per */

/*============================================================================
  HDRRXSTATS_UPDATE_CC_STATUS
*/
/**
  This function is used the record the CC status of the last control channel
  packet received based on the CRC status.

  @context HDRRX task. */
/*============================================================================*/
void  hdrrxstats_update_cc_status
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
)
{

  hdrrxstats.last_cc_status = cc_crc_status;

} /* hdrrxstats_update_cc_status */

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
hdrlog_cc_status_enum_type hdrrxstats_get_cc_status ( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrrxstats.last_cc_status;

} /* hdrrxstats_get_cc_status */

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
)
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    
  link_summary->per_carrier[carrier_cnt].num_bad_pkts = 
    hdrrxstats.per[demod_idx].instant;
    /* Read Packet-Error-Rate */
  
} /* hdrrxstats_gather_mc_link_summary_data */


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
void hdrrxstats_gather_link_summary_data
(
  hdrlog_link_summary_type *link_summary
    /**< Structure to fill in for logging */
)
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  link_summary->hstr = HDRHSTR_GET_COUNT();
    /* Store HSTR Counter */

  /* -- NOTE: It appears pkt_rxd_flag is not updated? */

  link_summary->packed_field.rxed_flag = 
    ( ( hdrrxstats.pkt_rxd_flag == TRUE )? 1: 0 );
    /* Set/clear packet received flag */

  hdrrxstats.pkt_rxd_flag = FALSE;

  link_summary->packed_field.pa_state = ( ( hdrmod_pa_is_on() == TRUE )? 1: 0 );
    /* Set/clear pa state flag */


  REX_DISABLE_PREMPTION(); 

  link_summary->per_inst = 
    ( hdrrxstats.per[HDRMDSP_DEMOD_CARRIER_0].instant * 1024 );
    /* Read Packet-Error-Rates */
    /* The "1024" multiplier is here for legacy reasons. The legacy ALS display
       computes PER using the following formula [per_inst / (16 * 1024)]. where, 
       16 is the sample size. In order to be backward compatible to legacy
       displays including the multiplier here */

  link_summary->per_seq = hdrrxstats.per[HDRMDSP_DEMOD_CARRIER_0].sequence;
    /* Update PER sequence */

  REX_ENABLE_PREMPTION();

} /* hdrrxstats_gather_link_summary_data */

/*============================================================================
  HDRRXSTATS_RESET_CONNECTED_STATE_PER
*/
/**
  Reset packet error rate statistics maintained for recent traffic session.

  @context Calling task. 

  @sideeffects None */
/*============================================================================*/

void hdrrxstats_reset_connected_state_per ( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrrxstats.connected_state_per.bad_cc_pkts = 0;
  hdrrxstats.connected_state_per.bad_tc_pkts = 0;
  hdrrxstats.connected_state_per.total_cc_pkts = 0;
  hdrrxstats.connected_state_per.total_tc_pkts = 0;

} /* hdrrxstats_reset_connected_state_per */

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

uint16 hdrrxstats_get_connected_state_per ( void )
{
  uint16 per;
  uint32 bad_pkts = 0, total_pkts = 0;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_DISABLE_PREMPTION(); 
  bad_pkts = hdrrxstats.connected_state_per.bad_cc_pkts + 
    hdrrxstats.connected_state_per.bad_tc_pkts;
  total_pkts = hdrrxstats.connected_state_per.total_cc_pkts + 
    hdrrxstats.connected_state_per.total_tc_pkts;
  REX_ENABLE_PREMPTION();

  if(total_pkts > 0)
  {
    per = (uint16) (( bad_pkts * 10000) / total_pkts );
  }
  else
  {
    per = 0;
    HDR_MSG_PROT_2(
      MSG_LEGACY_ERROR,
      "Unable to compute PER: bad pkts %d, total pkts %d", 
      bad_pkts, total_pkts);
  }

  return per;

} /* hdrrxstats_get_connected_state_per */


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
)
{
  if ((total_tc_packets != NULL) && (total_tc_errors != NULL))
  {
    REX_DISABLE_PREMPTION(); 
    *total_tc_errors = hdrrxstats.connected_state_per.bad_tc_pkts;
    *total_tc_packets = hdrrxstats.connected_state_per.total_tc_pkts;
    REX_ENABLE_PREMPTION();
  }
} /* hdrrxstats_get_connected_state_per_data */


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

uint32 hdrrxstats_get_fl_slot_count
(
  boolean reset
)
{

  uint32 directed_slots = hdrrxstats.directed_slots;
    /* Copy current count for return value. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Reset count if directed to */
  if (reset)
  {
    hdrrxstats.directed_slots = 0;
  }

  return directed_slots;

} /* hdrrxstats_get_fl_slot_count */

