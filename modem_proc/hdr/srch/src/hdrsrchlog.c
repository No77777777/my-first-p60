/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               H D R   S E A R C H    L O G    F U N C T I O N S

GENERAL DESCRIPTION

  This module provides general purpose logging routines used in various
  states for search task module.

EXTERNALIZED FUNCTIONS (Global)

  hdrsrchlog_log_fing_record()
    Logs finger record to the QXDM for offline parsing

  hdrsrchlog_log_search_record()
    Logs searcher results to the QXDM for offline parsing

  hdrsrchlog_log_pilot_set_record()
    Logs pilot set status to the QXDM for offline parsing

  hdrsrchlog_log_acq_info()
    Logs acquisition info to the QXDM for offline parsing

  hdrsrchlog_log_reacq_info()
    Logs re-acquisition info to the QXDM for offline parsing

  hdrsrchlog_log_sleep_info()
    Logs sleep info to the QXDM for offline parsing

  hdrsrchlog_log_on_demand()
    Logs on-demand HDRSRCH log packets to the QXDM for offline parsing

  hdrsrchlog_log_debug_display_record()
    Logs debug display info to the QXDM for offline parsing

  hdrsrchlog_reset_tune_away_info
    Reset the tune away logging information
    
  hdrsrchlog_update_srch_state_info
    Update the search state for tune away logging
  
  hdrsrchlog_log_tune_away_info
    Log the tune away event
    
  hdrsrchlog_log_tune_away_completion_info                     
    Record the tune away event and send the logging information
    
  hdrsrchlog_log_conn_state_stats
    Log the search related connected state statistics log packet
    
  hdrsrchlog_inc_conn_state_stats_counter
    Increment the connected state stats counter by the given amount
    
  hdrsrchlog_inc_search_set_stats_counter
    Increment the search set stats counters as specified.
    
  hdrsrchlog_reset_conn_state_stats
    Reset the connected state statistics specified

  hdrsrchlog_set_serving_sect_handoff_count
    Track the number of serving sector handoffs.  This needs special
    handling beyond what is provided in hdrsrchlog_inc_conn_state_stats_counter

  hdrsrchlog_log_eq_data
    Logs the equalizer data record for offline parsing.

  hdrsrchlog_log_eq_ci_buffer
    Logs equalizer C/I values read per slot for offline parsing.

  hdrsrchlog_log_physical_layer_power_info
    Logs physical layer power info for DO technology 
 
EXTERNALIZED FUNCTIONS (Regional)


INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2001 - 2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchlog.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when         who     what, where, why
----------   ---     --------------------------------------------------------- 
08/24/2017   rmv     Added support for FW QTA and few other log packet logging.
08/24/2017   rmv     Enable FW log pkts in L2DO measurement scenario
12/12/2016   svu     Fix KW issues 
11/18/2015   wsh     Force resend MDSP info right after tuneaway
10/27/2015   vke     Added changes to filter DRC and use it for 
                     HDRSRCHARD_EVENT_DRC_CHANGED
08/19/2015   wsh     Added support for SAR limitation 
07/13/2015   arm     Mainlined FEATURE_RF_SVDO_API feature.
06/17/2015   wsh     Fixed some issues found in bring up 
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
04/01/2015   vlc     Reset EVDO physical layer power info log packet before use.
02/10/2015   vlc     Fixed KW error in hdrsrchlog_log_physical_layer_power_info().
12/10/2014   vko     API to retrieve serving DRC rate
12/16/2014   vlc     Read Power log buffer data from FW 
11/19/2014   wsh     Added DR-DSDS support for ASDiv
12/02/2014   ljl     Fixed KW issue.
10/28/2014   vke     Changes to not to log RXAGC when HDR is in 
                     traffic suspend state
11/05/2014   vlc     Added EVDO physical layer power info log packet (0x1878) 
03/05/2014   wsh     Bug fixes for DSDS-ASDiv and DSDA-ASDiv support 
05/15/2014   wsh     Added QXDM interface to support FW MMSE log packet.
02/06/2014   mbs     Added support for FING log packet logging.
01/21/2014   sat     Fixed memscpy logging bugs.
12/19/2013   sat     Changed memcpy() to assignment or memscpy() (safer version).
11/05/2013   dsp     Added support for AFC log packet logging.
10/18/2013   smd     Don't submit Air Link Sum log packet in suspended state.
08/14/2013   rmg     Added support for FW DLNA log packet logging.
08/08/2013   arm     Fixed bolt compiler errors.
07/16/2013   arm     Added support for  FW DSDA Rx/Tx log packet logging.
06/26/2013   wsh     Added Selective TxD feature
05/31/2013   vko     Fixed klocwork issues
05/24/2013   smd     Reverted KW fixes.
05/16/2013   vko     Fixed klocwork issues
03/19/2013   dsp     Added support for Tx AGC log packet logging.
09/20/2012   vke     Logging the current RF warmup time
12/20/2012   vko     Fixed compiler warnings
12/14/12     ukl     Fix Klocwork issues
12/05/2012   dsp     Added support for RX AGC log packet logging.
11/20/2012   wsh     Fixed incorrect DRC reading in ARD
10/24/2012   wsh     Reduced DRC changed event frequency
06/19/2012   smd     Don't send DRC_CHANGE event to ARD state machine if it is 
                     not connected state 
05/25/2012   wsh     Moved ARD logic into it's own state machine 
03/22/2012   arm     Keep only one definition of HDRLOG_VAR_SIZEOF. 
03/19/2012   rkc     Added gRICE support for pilot sets
03/09/2012   vlc/kss Added gRICE info to EQ logging.
                     Changed demod_decision to eq_status.
01/12/2012   grl     Fixed tune away logging timestamp issue.
01/05/2012   wsh     Added support for SVLTE+SHDR
02/07/2012   arm     Fixed hdrlog buffer overrun issue.
11/11/2011   arm     HDR idle and access state diversity implemetation.
09/28/2011   lyl     Used the predDRC from mdsp logging for DRC check in ARD.
05/31/2011   cnx     Added hdrsrchlog_get_subpkt_protocol_data().
04/12/2011   smd     Used srch4 search structure. 
06/21/2010   cnx     Fixed compiler warnings.
06/03/2010   cnx     Fixed Klocwork errors.
03/25/2010   pxu     Removed FEATURE_HDR_EQ feature checks 
03/20/2010   pxu     Added saturation for equalizer packet's 16 bit C2I fields 
03/10/2010   kss     Enable logging of RPC in link summary log.
03/04/2010   lyl     Used qw_equ to set the timestamp (temp workaround).
03/02/2010   lyl     Fixed KW errors. 
09/30/2009   smd     Fixed KW errors.
09/29/2009   kss     Firmware macro update for Q6.
08/11/2009   etv     Used actual demod carrier instead of HDRMDSP_DEMOD_CARRIER_0
07/22/2009   ljl     Added frame offset.
07/17/2009   etv     Fixed logging issue by using aset_cnt instead of 
                     subaset_cnt in DRC-ARQ-DSC log packet.
06/19/2009   grl     Switched to the hdrsrchrfdiv API.
06/18/2009   etv     Used demod_idx instead of subaset_idx to index log buffer
05/18/2009   lyl     Updated for firmware MSGR interface.
04/22/2009   etv     Fixed diag buffer overrun crash with mDSP slot logs.
04/21/2009   etv     Used hdrrxstats API directly instead of hdrrx wrappers.
04/20/2009   lyl     Fixed ASET count in pilot sets v2/v3 log packet 
04/07/2009   lyl     Added pilot sets v3 log packet for OFS condition fltr eng.
03/24/2009   lyl     Fixed the MC EQ data log packet overrun issue.
03/18/2009   rkc     Added RF SVDO API changes.
02/26/2009   etv     Floor RxAGC1 during diversity tune away.
02/13/2009   rkc     Removed check for hdrsrchlog.triggered.sleep when
                     incrementing cmac_expiry_count.
01/26/2009   lyl     Logged the number of reacq search pilots in enh_sleep_info.
01/09/2009   etv     Cleaned up legacy per-slot logging static variables.
01/06/2009   etv     Fixed issue where DRC value did not get logged every
                     fourth slot for legacy ALS (0x1068) log packet.
01/06/2009   lyl     Updated ANT1 RSSI in finger data log packet. 
12/22/2008   lyl     Fixed slot count logging issue in MC searcher data & MC 
                     pilot sets packets.
12/12/2008   lyl     Fixed EQ data log packet.
12/10/2008   smd     Replaced assert.h with amssassert.h
11/20/2008   rkc     Fixed logging of ccmac_pkt_number in enh sleep log.
11/18/2008   kss     Removed/replaced intlocks.
11/13/2008   lyl     Updated slot_cnt in MC pilot set log packet.
11/11/2008   grl     Added EnhSleepInfo version 1 support.
11/10/2008   lyl     Fixed pilot set log packet (version 2 0x108B)
10/08/2008   lyl     Added back pilot sets log packet (version 2 0x108B)
09/25/2008   rkc     Split pilot logging from srch diag into seperate func.
09/03/2008   smd     Fixed the duration calculations of OFS in tune way info.
08/29/2008   lyl     Updated the pilot sets log packet (version 3).
10/13/2008   lyl     Fixed lint errors.
10/09/2008   ljl     Updated  MC finger channel info based on subaset structure.
10/10/2008   rkc     Fixed multi-carrier Pilot Set logging of ASET.
10/07/2008   lyl     Added the phase info in MC finger data log packet.
10/06/2008   etv     Logged power log pkt even during SOODA but floored rxagc.
09/17/2008   etv     Fixed DRC-ARQ-DSC Logging issue where slot_count was zero
09/08/2008   lyl     Added ofs_cond_eng in MC pilot sets log packet
07/17/2008   etv     log_alloc only for the active subasets for EQ data.
06/06/2008   rkc     Updated time_to_search.
05/09/2008   etv     Trigger Rx logging only if tuning is done on primary chain
05/14/2008   etv     Ratchet BestASPSINR at 18.5dB for legacy ALS.
04/17/2008   ljl     Added NSET and RSET pilots in multi-carrier search log.
04/17/2008   etv     Stored Reference sub-active set in the MC ALS log packet.
03/10/2008   etv     Increased the size of BASPSINR from 16 to 32 bits.
04/09/2008   ljl     Updated multi-carrier finger log.
03/08/2008   etv     Replaced obsolete RF APIs with new ones
03/25/2008   ljl     Updated multi-carrier finger and searcher log packets
03/06/2008   etv     DSP gives Log timestamp in slots insteads of half-slots.
03/05/2008   jyw     Fixed a compiling error.
03/04/2008   jyw     Updated the multiple carrier pilot sets logging.
02/26/2008   jyw     Added multiple carrier pilot sets logging.
02/20/2008   ljl     Removed the parameter from hdrsrchutil_get_hdr_rssi().
02/12/2008   etv     Logged RCLPredAdj as a single signed int32 field.
01/30/2008   jyw     Included the demod list in the tuning interface.
01/30/2008   etv     Read both words of BestASPSINR and increment ASP index log
                     for legacy logs to maintain backward compatibility.
01/15/2008   vish    Added support for MC RF Rx/Tx AGC APIs.
01/14/2008   etv     Refactored the following log pkts for Artemis interface.
                     1. Rel-0 DRC-ARQ Buffer log packet.
                     2. Rev-A DRC-ARQ-DSC Buffer log packet.
                     3. Air link Summary log packet.
                     4. Equalizer C2I log packet.
01/08/2008   etv     ASP index runs from 0 thru 5 instead of 1 thru 6.
12/18/2007   etv     Included RCLPredAdj in AirlinkSummary log packet.
                     Fixed bug with mDSP slot based logging by using sub-aset 
                     count instead of Max. Added support for per-carrier
                     Instantaneous PER statistics.
12/21/2007   sb      Added support for multicarrier equalizer data log packet.
12/05/2007   etv     Added support for MC DRC-ARQ-DSC Buffer, MC AirLinkSummary
                     MC Equalizer CI buffer, MC Finger and MC Searcher log pkts.
10/31/2007   ljl     Merged from Rev B branch.
10/17/2007   grl     Merged Rev B changes from branch.
09/27/2007   grl     Added support for rel A sleep info pkt.
05/02/2007   ljl     Logged the pn offsets of cset pilots.
03/28/2007   rmg     Fixed invalid values on HDR debug screen
03/09/2007   rkc     Fixed lint warnings/errors.
03/08/2007   grl     Fixed RCVT compiler warnings.
09/12/2006   ljl     Checked aset when logging sleep info.
07/10/2006   ljl     Checked mDSP clock before logging.
04/18/2006   ds      Support for mdsp services logging. 
04/13/2006   ljl     Suppressed the lint error #419
04/05/2006   etv     Fixed preprocessor stmt to use #else instead of #elif.
03/30/2006   etv     Added support for DRC/DSC gain boost logging.
                     Updated AirLinkSummary to log Pred. instead of trans DRC.
02/26/2006   kss     Removed DRC/DSC gain boost logging (for now).
01/11/2006   ds      Fixed best FL ASP field for Equalizer logging
12/12/2005   sq      Avoid read fault on L4 build
10/18/2005   grl     Added FEATURE_HDR_REVA_L1 ifdef to Rel A logging.
10/10/2005   ds      Added support DRC-ARQ-DSC log packet
09/29/2005   kss     Corrected slot logging of rl_asp/fl_asp in DRC_ARQ log.
07/12/2005   ds      Corrected log_ptr check in equalizer C/I log packet 
07/11/2005   ds      Added Search Type2 log packet with Equalizer fields
07/05/2005   ds      Added support for generic MDSP debug logging
06/14/2005   ds      Added version number field to equalizer log packets
06/07/2005   grl     Fixed tune away logging to log for first tune away.
06/03/2005   ds      Added equalizer logging under FEATURE_HDR_EQ
05/31/2005   grl     Get current channel info from hdrsrchrf.
05/25/2005   ds      Added BCMCS feature check for BC states
04/19/2005   ljl     Added code to check null pointer before using it.
03/18/2005   grl     Fixed serving sector handoff logging.
03/08/2005   sq      Added check to returned sector pointer
02/22/2005   sq      Added logging of srch record in OFS states
11/29/2004   grl     Added connected state stat logging support.
11/19/2004   ds      Air link summary log packet bug fix for HSBS
08/23/2004   ds      Fixed HDR debug display record update
06/24/2004   sq      More bug fixes for HDRSRCH log packets
06/08/2004   sq      Bug fix for SLEEP INFO LOG packet
05/03/2004   ds      Added debug display log packet
01/19/2004   sq      Properly handled HDRLOG_ACQ_NULL acquisition result
12/05/2003   sq      Check DSP clock for PN roll logging which accesses DSP
12/04/2003   aaj     Replace rfhdr.h with rf.h
11/11/2003   sq      Supported 3 new log packets( acq,reacq,sleep )
10/06/2003   aaj     Mainlined HDRSRCH_MULTIPATH_WIN_CENTER
09/10/2003   mpa     Converted F3 messages to MSG2.0
07/20/2003   sq      Supportd (word *) for reading the DSP registers
03/11/2003   ajn     Updated Copyright notice
11/15/2002   aaj     Fixed DRC-ARQ logging delay problem
10/13/2002   aaj     Added support for two frames logging in drc-arq log packet
09/24/2002   sq      Added more srchr logging info related to window centering
09/18/2002   aaj     Support for multipath window centering for HDR
07/27/2002   sst/aaj Log window offset default for nset
07/19/2002   aaj     Log CSET instead of RSET in search log packet
06/19/2002   ajn     Diversity MSG's to LOW since now supported in log packets
04/16/2002   aaj     Fixed RPC logging to be per Cell map instead of per ASP
04/15/2002   aaj     Remove temp debug F3 messages added for diversity
04/02/2002   kss     Always use DSP value for DRC rate logging.
03/15/2002   aaj     Do not perform PN ROLL logging if in sleep state
01/17/2002   aaj     support for RF band to the hdr sector structure type
01/14/2002   aaj     Added RSSI for diversity antennae
12/07/2001   aaj     Fixed DRCLock bit
12/03/2001   aaj     Log DRC-ARQ and RAB Buffer only for connected state
11/29/2001   aaj     Add messge printing for diversity
11/26/2001   aaj     Massive chanegs to DRC-ARQ for DRCLock and slot timestamp
11/15/2001   aaj     Removed srch_status, gen_ta. Changed airlinksummary
11/14/2001   aaj     Fixed ASP logging in AirLinkSummary packet
11/01/2001   aaj     Log aset frequency in pilot sets instead of driver freq
10/29/2001   aaj     Support for RAB logging. Corrected basp sinr in ALSummary
10/08/2001   aaj     Log RSET searches in srch data. Increased set logging rate
09/13/2001   aaj     Removed temporary crc stats from this file.
09/05/2001   aaj     Antenna Diversity integrated in the idle state
09/01/2001   aaj     Corrected to search/finger/pilot set log implementations
08/21/2001   aaj     Fixed finger lock flag
08/01/2001   aaj     change to finger logging to read from common fing param
06/12/2001   aaj     Major changes for new logging for 1xEV
04/24/2001   aaj     Migrated to TAZ3
04/11/2001   ajn     freq_idx -> chan_num
03/26/2001   aaj     Consolidated finger variables into hdrsrchfing struct
03/08/2001   aaj     Added rate governer to hdr search log packets
03/02/2001   aaj     included customer.h file
02/22/2001   aaj     minor corrections to gen_ta function
02/08/2001   aaj     Added srch status packet
02/05/2001   aaj     Created module

==========================================================================*/

/* <EJECT> */
/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"

#include "hdrsrchlog.h"

#include "hdrsrchdrv.h"
#include "hdrsrchfing.h"
#include "hdrsrchset.h"
#include "hdrsrchdrv.h"
#include "hdrsrchutil.h"
#include "hdrsrchdef.h"
#include "hdrsrchi.h"
#include "hdrsrch.h"
#include "hdrsrcheq.h"

#include "rfm_hdr.h"

#include "hdrdebug.h"
#include "hdrplogsrch.h"
#include "amssassert.h"

#include "hdrsrchstate.h"
#include "hdrmdspmcr.h"

#include "hdrsrchset.h"

#include "hdrsrchrf.h"

#include "hdrsrchstates.h"
#include "hdrsrchrf.h"
#include "hdrsrchrfdiv.h"

#include "hdrrxstats.h"
#include "hdrmdsp.h"
#include "hdrlogi.h"
#include "hdrutil.h"
#include "hdrlog.h"
#include "hdrfmac.h"

#include "hdrsrchard.h"
#include "hdrfw_msg.h"
#include "hdrfw_logging.h"
#include "hdrmultirat.h"

#ifdef FEATURE_MEMCPY_REMOVAL
#include "stringl.h"
#endif /* FEATURE_MEMCPY_REMOVAL */

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#include "hdrmdspmcr.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */
#include "vstmr_hdr.h"

#include "hdrscp.h"
/* <EJECT> */

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

#define HDRSRCH_POSX8_ADD( p1, p2 ) \
   ((((p1) + (p2)) > HDR_PN_CIRC_X8) ? (p1)+(p2)-HDR_PN_CIRC_X8 : (p1)+(p2))

/* Is the given state a TC tune away state */
#define HDRSRCHLOG_IS_TC_TUNE_AWAY_STATE( state ) \
   ( ( state == HDRSRCH_OFS_TC_STATE ) ||      \
     ( state == HDRSRCH_SUSPENDED_TC_STATE ) ? \
     TRUE : FALSE )
         
#define HDRPLOG_NO_PN_FOUND                      0xFFFF /* No Pn found*/

#define HDRSRCHLOG_CONN_STATE_STATS_VER        0
  /* Version number of the connected state stats packet. */

#define HDRSRCHLOG_TA_INFO_STATS_VER           0
  /* Version number of the RRI info stats packet. */

#define HDRSRCHLOG_SEARCH_DATA_TYPE2_VER       0
  /* Version number of the search data type2 packet */

#define HDRSRCHLOG_EQUALIZER_DATA_VER          0
  /* Version number of the equalizer data packet. */

#define HDRSRCHLOG_EQUALIZER_CI_BUFFER_VER     1
  /* Version number of the equalizer C/I buffer log packet. */

#define HDRSRCHLOG_DRC_ARQ_DSC_BUFFER_VER      0
  /* Version number of the DRC-ARQ-DSC buffer log packet. */

#define HDRLOG_PILOT_SETS_VER                  3
  /* Version number of the pilot sets log packet. */

#define HDRSRCHLOG_MAX_HO_COUNT 65535
  /* The maximum number of handoffs tracked in the HO register. */

#define HDRSRCHLOG_PACKED_INVALID_CHAN 0x7ff
  /* An invalid channel in a 11 bit field. */

#define HDRSRCHLOG_PACKED_INVALID_BAND 0x1f
  /* An invalid band class in a 5 bit field. */

/*---------------------------------------------------------------------------
                           MDSP SLOT LOGGING MACROS
---------------------------------------------------------------------------*/
/* Number of slots logged by DSP at a time for slot logging */
#define HDRSRCHLOG_MDSP_LOG_BUFF_SIZE_SLOTS             4

/* Number of buffers maintained for each one of the log packet logged
   every PN roll */
#define HDRSRCHLOG_DOUBLE_LOG_BUFFERS                   2

/* Mask used to make the log buffer index wrap around to zero after 31.
   Please note that this mask is based on the size of the log buffer and
   if this buffer size ever changes(unlikely), this mask should be changed
   as well */
#define HDRSRCHLOG_SLOT_LOGGING_BUFF_SIZE_MASK          0x1F

/* Slot logging buffer size; includes double buffering */
#define HDRSRCHLOG_SLOT_LOGGING_BUFF_SIZE           \
          (HDRSRCHLOG_DOUBLE_LOG_BUFFERS * HDRLOG_NUM_SLOTS_IN_FRAME)

/* Starting Log buffer index for this subframe based on HSTR */
#define HDRSRCHLOG_SF_START_LOG_BUFF_IDX(str)         \
          (str & HDRSRCHLOG_SLOT_LOGGING_BUFF_SIZE_MASK)

/* Starting log buffer index for this frame, given the HSTR. */
#define HDRSRCHLOG_FRAME_START_LOG_BUFF_IDX(str)       \
          ((HDRSRCHLOG_SF_START_LOG_BUFF_IDX((str)) <= 15)? 0: 16)

/* Find the next log buffer index. Wrap around to zero after 31 */
#define HDRSRCHLOG_NEXT_LOG_BUFF_IDX(idx)           \
          (((idx) + 1) & HDRSRCHLOG_SLOT_LOGGING_BUFF_SIZE_MASK)

/* S/W accumulates logging information by sampling every subframe
   and ships out the information on frame boundaries. If the HSTR
   associated with a log buffer corresponds to any slot with in the
   last sub-frame of a frame, we are ready for logging the frame */
#define HDRSRCHLOG_BUFFER_READY_FOR_LOGGING(str, frame_offset)     \
          (((str - (frame_offset % 4)) % HDRLOG_NUM_SLOTS_IN_FRAME) > 11)

/* Find the absolute Slot Time Reference for the first slot of the frame 
   being logged. Each subframe ARM reads a sub-frame worth of log info
   from mDSP buffers. This buffer is timestamped by mDSP with HSTR of first 
   slot corresponding to the subframe. If ARM has been collecting stats
   periodically every sub-frame, during every fourth subframe(in a frame),
   ARM should have one frame worth of data to ship out. The Slot time
   reference of the first slot of frame is given as follows. */
#define HDRSRCHLOG_FRAME_START_STR(str, frame_offset)    \
  ((str) - (frame_offset % 4) - (HDRLOG_NUM_SLOTS_IN_SUBFRAME * 3))

/* The bit positions 0, 1 and 2 are set in the mask if the log buffers
   of carriers 0, 1 and 2 respectively are valid. This macro counts
   the number of carriers with valid log buffers */
#define HDRSRCHLOG_NUM_VALID_CARRIER_BUFFS(mask)    \
  (((mask) & 0x01) + (((mask) & 0x02) >> 1) + (((mask) & 0x04) >> 2))

/* The valid mask defined by mDSP has bits 0, 1, 2 assigned for carriers 
   0, 1 and 2 indicating whether the logging buffers associated with those 
   carriers are valid during this subframe */
#define HDRSRCHLOG_CARRIER_LOG_BUFF_IS_VALID(c_idx, valid_mask) \
          ((((valid_mask) & (0x01 << (c_idx))) != 0)? TRUE: FALSE)

#define HDRLOG_PILOT_SETS_VER                  3
  /* Version number of the pilot sets log packet. */

#ifdef FEATURE_HDR_REVB_SUPPORTED
#define HDRSCP_SUBTYPE_4_RTCMAC 4
#endif /* FEATURE_HDR_REVB_SUPPORTED */

#define HDR_FILT_PRED_DRC_FILT_LEN     2
  /* Filter length for data indicator IIR filter. This filtering is performed
     every subframe which is 6.7ms. Therefore filter length of 2 will
     give us half a frame average */



/* <EJECT> */
/*===========================================================================

                            HDRSRCHLOG GOVERNER

===========================================================================*/

typedef struct
{
  uint8 srch;                /* logs HDR searcher packet */
  uint8 fing;                /* logs Finger packet */
  uint8 eq;                  /* logs Equalizer packet */
  uint8 set;                 /* logs pilot set info packet */
  uint8 logdisp;             /* logs debug display packet */
} hdrsrchlog_rate_gov_struct_type;

/* The log rate is specified in the units of number of searcher dumps.
   Typically each dump is every 60ms except acquisition state */

LOCAL hdrsrchlog_rate_gov_struct_type hdrsrchlog_rate =
{
  1,          /* x 30ms HDR searcher packet  log rate */
  1,          /* x 30ms HDR Finger packet log rate */
  1,          /* x 30ms HDR Equalizer packet log rate */
  2,          /* x 30ms HDR Pilot Set packet log rate */
  33          /* x 30ms HDR debug display packet log rate */
};

/* Macros to check if it time to log the searcher packets */
#define HDRSRCH_LOG_SRCH(dumpc)   ((dumpc) % hdrsrchlog_rate.srch   == 0)
#define HDRSRCH_LOG_FING(dumpc)   ((dumpc) % hdrsrchlog_rate.fing   == 0)
#define HDRSRCH_LOG_EQ(dumpc)     ((dumpc) % hdrsrchlog_rate.eq     == 0)
#define HDRSRCH_LOG_SET(dumpc)    ((dumpc) % hdrsrchlog_rate.set    == 0)
#define HDRLOG_DEBUG_FILL(dumpc)  ((dumpc) % hdrsrchlog_rate.logdisp  == 0)

hdrsrchlog_diag_struct_type  hdrsrchlog;
  /* Global structure to store ACQ INFO/REACQ INFO/SLEEP INFO logging info  */

hdrlog_debug_display_struct_subtype    hdr_debug_disp;
  /* This is the global structure to store the debug display info. This
     needs to be passed on to the OEM layer for display on the UI */

/* Multi-carrier DRC-ARQ-DSC Buffer type */
typedef hdrlog_per_slot_channel_info_type
  hdrsrchlog_mc_drc_arq_dsc_buffer_type 
    [HDRSRCH_MAX_DEMOD_CARRIERS_CNT] 
    [HDRSRCHLOG_SLOT_LOGGING_BUFF_SIZE];

/* Multi-carrier per-slot link summary information type */
typedef hdrlog_per_slot_link_summary_type
  hdrsrchlog_mc_per_slot_link_summary_type
    [HDRSRCH_MAX_DEMOD_CARRIERS_CNT]
    [HDRSRCHLOG_SLOT_LOGGING_BUFF_SIZE];

/* Multi-carrier per-slot Equalizer C2I buffer type */
typedef hdrlog_per_slot_eq_ci_buffer_type 
  hdrsrchlog_mc_per_slot_eq_ci_buffer_type
    [HDRSRCH_MAX_DEMOD_CARRIERS_CNT]
    [HDRSRCHLOG_SLOT_LOGGING_BUFF_SIZE];

static struct
{

  log_hdr_drc_arq_buffer_type  
    drc_arq[HDRLOG_DRC_ARQ_FRAMES_PER_PKT];
    /* Global variable to store DRC ARQ frames which are logged over
       multiple PN rolls */

  hdrlog_link_summary_type 
    link_summary[HDRLOG_FRAMES_PER_PKT];
    /* Global variable to store Airlinksummary frames which are logged over 
       multiple PN rolls */

  log_hdr_drc_arq_dsc_buffer_type  
    drc_arq_dsc[HDRLOG_DRC_ARQ_DSC_FRAMES_PER_PKT];
    /* Global variable to store DRC ARQ DSC frames which are logged over
       multiple PN rolls */

  log_hdr_eq_ci_buffer_type    eq_ci[HDRLOG_EQ_CI_FRAMES_PER_PKT];
    /* Global variable to store Equalizer C/I frames which are logged over
       multiple PN rolls */


  hdrsrchlog_mc_drc_arq_dsc_buffer_type mc_drc_arq_dsc_buffer;
    /* Multi-carrier DRC-ARQ-DSC log Buffer - double buffered */

  hdrsrchlog_mc_per_slot_link_summary_type mc_link_summary;
    /* Multi-carrier Link Summary log buffer - double buffered */

  hdrsrchlog_mc_per_slot_eq_ci_buffer_type mc_eq_ci_buffer;
    /* Multi-carrier Equalizer C2I log buffer */

  uint8 rx_tx_cbuff_valid;
    /* Mask denoting which one of Rx & Tx carrier log buffers were both valid 
       across the four subframes of the past frame. This mask includes a union 
       of all carriers that were valid during the last frame */

  uint8 rx_cbuff_valid;
    /* Mask denoting which one of Rx carrier log buffers were valid across 
       the four subframes of the past frame. This mask includes a union 
       of all carriers that were valid during the last frame */
  
} hdrsrchlog_slot_logging;

#ifdef FEATURE_HDR_SELECTIVE_TXD
#define HDRSRCHLOG_STXD_LOGGING_NOTIFY_INTERVAL   32

static struct {
  boolean read_pending;
    /* Whether prev data has been read (false) or not (true) */

  boolean mdsp_info_needed;
    /* if sTxD is interested in getting AGC readouts */

  boolean force_resend;
    /* resend mdsp info if set */

  int rx0;
  int rx1;
  int tx_total;

  uint32 ratchet;

  int8   slots;

  rex_crit_sect_type crit_sect;
} hdrsrchlog_stxd_logging;
/* Used to detect possible FW logging miss. Only needed for debuging */

#endif /* FEATURE_HDR_SELECTIVE_TXD */

extern hdrsrcheq_config_struct_type     hdrsrcheq_default_init[ ];
  /* Default configuration parameters for the equalizers */

extern hdrsrcheq_config_struct_type     hdrsrcheq_default_div_init[ ];
  /* Diversity configuration parameters for the equalizers */

typedef struct
{
  uint32 log_id;
  boolean enabled;
} hdrsrchlog_fw_log_status_entry_type;

/* To add support for a new log pkt, simple add to the end of this
   table. All log pkts are disabled by default */
LOCAL hdrsrchlog_fw_log_status_entry_type hdrsrchlog_fw_log_status[] = {
  { HDRFW_DIAG_LOG_RX_AGC_PKT_ID, FALSE }, 
    /* Flag to indicate if RxAGC packet is being logged */
  { HDRFW_DIAG_LOG_TXAGC_PKT_ID, FALSE },
    /* Flag to indicate if TxAGC packet is being logged */
  { HDRFW_DIAG_LOG_DLNA_PKT_ID, FALSE },
    /* Flag to indicate if DLNA packet is being logged */
  { HDRFW_DIAG_LOG_AFC_PKT_ID, FALSE },
    /* Flag to indicate if AFC packet is being logged */
  { HDRFW_DIAG_LOG_FING_PKT_ID, FALSE },
    /* Flag to indicate if FING packet is being logged */
  { HDRFW_DIAG_LOG_MAC_PKT_ID, FALSE },
    /* Flag to indicate if MMSE packet is being logged */
  { HDRFW_DIAG_LOG_RX_IQ_SAMPLE_PKT_ID, FALSE },
    /* Flag to indicate if FW RX IQ packet is being logged */
  { HDRFW_DIAG_LOG_VSRC_STMR_PKT_ID, FALSE },
    /* Flag to indicate if FW VSTMR packet is being logged */
  { HDRFW_DIAG_LOG_DSDS_QTA_PKT_ID, FALSE }
    /* Flag to indicate if FW QTA packet is being logged */
};

uint8 hdrsrchlog_mc_trans_drc_value = 0;
/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/

#ifdef FEATURE_POWER_INFO_LOG_PACKET
void hdrsrchlog_log_physical_layer_power_info
(
  uint8                                 buffer_idx
);
#endif /* FEATURE_POWER_INFO_LOG_PACKET */

/* <EJECT> */
/*===========================================================================

                      FUNCTION  DEFINITIONS

===========================================================================*/

/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_EQ_DATA_LOG_REC

DESCRIPTION  : Fill in the information for the equalizer data  record.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_update_eq_data_log_rec
(
  LOG_HDR_EQUALIZER_DATA_C_type *log_ptr
    /* log pointer to the equalizer log record type */
)
{
  int8                         eq;       
    /* Equalizer loop variable */

  log_hdr_eq_data_type         *eq_ptr;
    /* Convenience pointer for log */

  hdrsrcheq_config_struct_type *hdreq_ptr;
    /* Convenince pointer */

  hdrsrch_sect_struct_type     *serv_sect;   
    /* Current serving sector pointer */

  boolean                      div_on;
    /* The flag to indicate diversity on or off */

  hdrsrch_demod_idx_type       demod_idx = 
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
    /* Demod index for IDLE subaset */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  log_ptr->version      = HDRSRCHLOG_EQUALIZER_DATA_VER;
    /* Get the version number of the log packet */

  log_ptr->srch_state   = hdrsrch_state_id;
    /* Current Srch State */

  log_ptr->mstrx8       = hdrsrchdrv.mstr_x8;
    /* Latest mobile station time reference */

  log_ptr->eq.num_eq    = hdrsrcheq.params.max_eqs;
    /* Maximum number of equalizers */

  log_ptr->eq.flag      = hdrsrcheq.flag;
    /* Equalizer on/off flag */

  /* PN offsets of serving ASET pilot */
  if( hdrmdsp_check_dsp_clk () )    
  {
    log_ptr->mstr_err_x8  = HDRMSTR_GET_MSTR_ERROR_X8();
      /* Latest mobile station timing error */

    log_ptr->mode         = HDREQ_GET_DEMOD_DECISION( demod_idx );
      /* Decision on Rake/Equalizer usage in current slot data demod */

    log_ptr->asp.best_asp_idx = HDRASP_GET_BASP_FL( demod_idx );
      /* Get the Best Forward link ASP Index */

#ifdef FEATURE_HDR_REVB
    serv_sect = hdrsrchset_get_asp_sect_ptr( HDRSRCH_IDLE_SUBASET_IDX,
                                             log_ptr->asp.best_asp_idx );
#else
    serv_sect = hdrsrchset_get_asp_sect_ptr( log_ptr->asp.best_asp_idx );
      /* Get the serving sector from best ASP index */
#endif /* FEATURE_HDR_REVB */

    if ( serv_sect != NULL )
    {
      log_ptr->serving_pn            =  serv_sect->pn_offset;
        /* Update the PN info when sector is found  */
    }
    else
    {
      log_ptr->serving_pn            =  0xffff;
        /* Set PN to invalid ( 0xffff ) if sector is not found */
    }
  } /* if hdrmdsp_check_dsp_clk */

  log_ptr->com_posx8  = 
    (uint16)hdrsrcheq.asp[demod_idx][log_ptr->asp.best_asp_idx].com_posx2 * 4;
    /* Center of Mass in chipx8 units */

  div_on = hdrsrchrfdiv_diversity_demod_is_enabled( );
    /* Read the current diversity demod status */

  for ( eq = 0; eq < hdrsrcheq.params.max_eqs; eq++ )
  {
    eq_ptr = &log_ptr->eq_data[eq];
      /* Convenience pointer */

    ASSERT( eq < HDRSRCHEQ_MAX_EQS_CNT );
      /* Make sure Eq number is less than HDRSRCHEQ_MAX_EQS_CNT */

    hdreq_ptr =  &hdrsrcheq.eqs[eq].config;
      /* Convenience pointer */

    /* Get the configuration parameters - antenna, diversity, lms step, filter
       length */
    eq_ptr->eq_info.antenna      = hdreq_ptr->ant_sel;
    eq_ptr->eq_info.diversity    = div_on;
    eq_ptr->eq_info.eq_index     = eq;
    eq_ptr->eq_info.lms_step     = hdrsrcheq_default_init[eq].lms_step;
    eq_ptr->eq_info.filt_length  = hdrsrcheq_default_init[eq].filt_len;

    /* Get the Multipass configuration */

    eq_ptr->eq_multipass_info.enable  = hdrsrcheq_default_init[eq].multipass.enable;
    eq_ptr->eq_multipass_info.multipass_c2i_used  = 
                               hdrsrcheq_default_init[eq].multipass.multipass_c2i_used;

    if ( !div_on )
    {
      eq_ptr->eq_multipass_info.multipass1_lms_step = 
                                 hdrsrcheq_default_init[eq].multipass.lms_step[0];
      eq_ptr->eq_multipass_info.multipass2_lms_step = 
                                 hdrsrcheq_default_init[eq].multipass.lms_step[1];
    }
    else
    {
      eq_ptr->eq_multipass_info.multipass1_lms_step = 
                                 hdrsrcheq_default_div_init[eq].multipass.lms_step[0];
      eq_ptr->eq_multipass_info.multipass2_lms_step = 
                                 hdrsrcheq_default_div_init[eq].multipass.lms_step[1];
    }

    /* Get the C/I of the multipass used in demodulation */

    switch( eq )
    {
      /* eq = 0 => pass 0; 
         eq = 1 => pass 3;
         eq = 2 => pass 4; */

      case 0:
      eq_ptr->c2i = hdrsrcheq.eqs[demod_idx].eq_pass_c2i[eq].c2i;
      break;

      default:
      eq_ptr->c2i = hdrsrcheq.eqs[demod_idx].eq_pass_c2i[eq + 2].c2i;
      break;
    }
  } /* for eq */

} /* hdrsrchlog_update_eq_data_log_rec() */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_EQ_DATA

DESCRIPTION  : Logs the equalizer data. It gets a log record from the diag
               log buffer, fills it with the latest equalizer information and
               commits the log record to the diag for delivery.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_eq_data( void )
{

  LOG_HDR_EQUALIZER_DATA_C_type *log_ptr;
  /* log pointer to the equalizer log record type */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  log_ptr = (LOG_HDR_EQUALIZER_DATA_C_type *)
            log_alloc_ex( (log_code_type) LOG_HDR_EQUALIZER_DATA_C,
                          HDRLOG_VAR_SIZEOF( LOG_HDR_EQUALIZER_DATA_C_type,
                                             log_hdr_eq_data_type,
                                             hdrsrcheq.params.max_eqs ) );

  /* Get an allocated log record from diag buffer */
  if ( log_ptr )
  {
    hdrsrchlog_update_eq_data_log_rec( log_ptr );
      /* Update the fields in the log record */

    log_commit( (log_type *) log_ptr );
      /* Submit the log to DIAG */

  }/* if log_ptr */

}/* hdrsrchlog_log_eq_data() */

/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_MC_EQ_DATA

DESCRIPTION  : Logs the multi carrier equalizer data. It gets a log record 
               from the diag log buffer, fills it with the latest equalizer
               information and commits the log record to diag for delivery.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_mc_eq_data( void )
{

  LOG_HDR_MC_EQUALIZER_DATA_C_type *log_ptr;
    /* Log pointer to the equalizer log record type */

  hdrlog_per_carrier_mc_eq_data_type  *eq_ptr;
    /* Convenience pointer for log */

  hdrsrch_sect_struct_type  *serv_sect;   
    /* Current serving sector pointer */

  uint8  demod_idx;       
    /* Demodulator index */

  uint8  num_eq;
    /* Number of equalizer passes loop variable */

  uint8  subaset_idx;
    /* Loop variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( hdrmdsp_check_dsp_clk () )    
  {

    log_ptr = ( LOG_HDR_MC_EQUALIZER_DATA_C_type * ) 
      log_alloc( LOG_HDR_MC_EQUALIZER_DATA_C,
                 sizeof( LOG_HDR_MC_EQUALIZER_DATA_C_type ) + 
                 ( hdrsrchset.subaset_cnt ) * 
                  sizeof( hdrlog_per_carrier_mc_eq_data_type ) );

    /* Get an allocated log record from diag buffer */
    if ( log_ptr )
    {
      /* PN offsets of serving ASET pilot */
        log_ptr->version = HDRLOG_MC_EQUALIZER_DATA_VER;
          /* Get the version number of the log packet */

        hdrsrcheq_read_eq_status();
          /* Update the equalizer data structure with latest equalizer 
           * information */

        log_ptr->srch_state = hdrsrch_state_id;
          /* Current Srch State */

        log_ptr->mstrx8 = hdrsrchdrv.mstr_x8;
          /* Latest mobile station time reference */

        log_ptr->mstr_err_x8 = HDRMSTR_GET_MSTR_ERROR_X8();
          /* Latest mobile station timing error */    

        log_ptr->eq.eq_status = HDREQ_GET_EQ_ENABLED( HDRMDSP_DEMOD_CARRIER_0 );
          /* Equalizer on/off flag */ 

        log_ptr->eq.num_eq = hdrsrcheq.params.max_eqs;
          /* Maximum number of equalizers */             

        log_ptr->str = HDRHSTR_GET_COUNT()/2;
          /* Number of slots to decode the packet */

        log_ptr->num_carriers = HDRSRCH_MAX_NUM_CARRIERS;
          /* Number of carriers in the log packet */

        for ( subaset_idx = 0; 
              subaset_idx < hdrsrchset.subaset_cnt; subaset_idx++ )
        {
          demod_idx = hdrsrchset.subaset[subaset_idx].demod_idx;

          eq_ptr = &log_ptr->per_carrier_eq_data[subaset_idx];
            /* Convenience pointer */

          eq_ptr->carrier_id.channel_id = demod_idx;
            /* Get the Demodulator Id */

          eq_ptr->carrier_id.channel_no = 
            hdrsrchset.subaset[subaset_idx].channel.chan_num;
            /* Get the channel number */

          eq_ptr->carrier_id.band_class = 
            hdrsrchset.subaset[subaset_idx].channel.band;
            /* Get the system band class */

          eq_ptr->bit_fields.best_asp_idx = HDRASP_GET_BASP_FL( demod_idx );
            /* Get the Best Forward link ASP Index */
            
          if(eq_ptr->bit_fields.best_asp_idx >= HDRSRCH_ASET_MAX_SIZE + 1)
          {
            HDR_MSG_SRCH_1( MSG_LEGACY_FATAL, "FW BestASPIndex val(%d)>= 7,\
                        breaking out of log pkt formation", 
                            eq_ptr->bit_fields.best_asp_idx );
            break;
          }

          eq_ptr->bit_fields.mode = HDREQ_GET_EQ_DEMOD_SELECT( demod_idx );
            /* Decision on Rake/Equalizer usage in demod */

          serv_sect = hdrsrchset_get_asp_sect_ptr( HDRSRCH_IDLE_SUBASET_IDX,
            eq_ptr->bit_fields.best_asp_idx );      
            /* Get the serving sector from best ASP index */

          if ( serv_sect != NULL )
          {
            eq_ptr->serving_pn = serv_sect->pn_offset;
              /* Update the PN info when sector is found  */
          }
          else
          {
            eq_ptr->serving_pn =  HDR_NO_PN_OFFSET;
              /* Set PN to invalid ( 0xffff ) if sector is not found */
          }

          eq_ptr->com_posx8 = (uint16)hdrsrcheq.
            asp[demod_idx][eq_ptr->bit_fields.best_asp_idx].com_posx2 * 4;
            /* Center of Mass in chipx8 units */

          for ( num_eq = 0; num_eq < HDRLOG_MC_NUM_EQ_PASSES; num_eq++ )
          {
            eq_ptr->c2i[ num_eq ] = HDREQ_GET_EQ_PASS_CI( demod_idx, num_eq );
              /* Get the C/I of the multipass used */
          }

        } /* for subaset_idx */


      log_commit( (log_type *) log_ptr );
        /* Submit the log to DIAG */

    }/* if log_ptr */

  } /* if hdrmdsp_check_dsp_clk */

}/* hdrsrchlog_log_mc_eq_data() */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_EQ_CI_BUFFER

DESCRIPTION  : Log equalizer C/I values read per slot.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_eq_ci_buffer ( void )
{
  LOG_HDR_EQUALIZER_CI_BUFFER_C_type *log_ptr;
    /* log pointer to the log record type */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If the SRCH state is either Connected or TC Suspended */
  if ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE ||
       hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE )
  {

    log_ptr = (LOG_HDR_EQUALIZER_CI_BUFFER_C_type*)
              log_alloc_ex( (log_code_type) LOG_HDR_EQUALIZER_CI_BUFFER_C,
                            HDRLOG_VAR_SIZEOF( 
                              LOG_HDR_EQUALIZER_CI_BUFFER_C_type,
                              log_hdr_eq_ci_buffer_type,
                              HDRLOG_EQ_CI_FRAMES_PER_PKT ) );

    if ( log_ptr )
    {
      log_ptr->version    = HDRSRCHLOG_EQUALIZER_CI_BUFFER_VER;
        /* Get the version number of the log packet */

      /* Copy two frames worth of data to log. */
#ifdef FEATURE_MEMCPY_REMOVAL
      memscpy( ( void * ) (log_ptr->eq_data),
               sizeof (log_hdr_eq_ci_buffer_type) * HDRLOG_EQ_CI_FRAMES_PER_PKT,
               ( void * ) (hdrsrchlog_slot_logging.eq_ci),
                sizeof ( hdrsrchlog_slot_logging.eq_ci ) );  /*lint !e419 */
#else
      memcpy( ( void * ) (log_ptr->eq_data),
              ( void * ) (hdrsrchlog_slot_logging.eq_ci),
                sizeof ( hdrsrchlog_slot_logging.eq_ci ) );  /*lint !e419 */
#endif /* FEATURE_MEMCPY_REMOVAL */

      log_commit( (log_type *) log_ptr );
        /* submit the log to DIAG */

    } /* if log_ptr */

  } /* Connected or TC Suspended */

  memset ((uint8*) &hdrsrchlog_slot_logging.eq_ci[0], 
          0, 
          sizeof(hdrsrchlog_slot_logging.eq_ci));
    /* Flush the buffer */

} /* hdrsrchlog_log_eq_ci_buffer */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_MC_EQ_CI_BUFFER

DESCRIPTION  : Logs one frame worth of Multicarrier Equalizer C2I Buffer 
               data read from mDSP buffers for each slot.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_mc_eq_ci_buffer
( 
  uint8 first_index,
    /* Index corresponding to first slot of logging in the buffer */
  uint8 cbuff_valid_mask,
  /* Bit mask denoting if log buffer is valid for each carrier */
  uint16 str
    /* Slot Time Reference */
)
{
  uint8 demod_idx = 0;
    /* Demod Carrier Index */

  uint8 subaset_idx = 0;
    /* Subaset Index */

  uint8 carrier_cnt = 0;
    /* Number of carriers logged this frame */

  LOG_HDR_MC_EQUALIZER_CI_BUFFER_C_type *log_ptr;
    /* Pointer to log buffer */

  hdrlog_per_slot_eq_ci_buffer_type *c_ptr;
    /* Convenience pointer */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  carrier_cnt = HDRSRCHLOG_NUM_VALID_CARRIER_BUFFS( cbuff_valid_mask );
    /* Find the number of carriers active to be logged this frame */

  log_ptr = (LOG_HDR_MC_EQUALIZER_CI_BUFFER_C_type*)
            log_alloc_ex( (log_code_type) LOG_HDR_MC_EQUALIZER_CI_BUFFER_C,
                          ( sizeof( LOG_HDR_MC_EQUALIZER_CI_BUFFER_C_type ) +
                            ( sizeof ( hdrlog_per_carrier_eq_ci_buffer_type ) *
                                       (hdrsrchset.subaset_cnt )) ) );
    /* Allocate log buffer! (Fails if log code is not enabled) */

  /* If log code is enabled */
  if ( log_ptr )
  {
    log_ptr->version  = HDRLOG_MC_EQUALIZER_CI_BUFFER_VERSION;
      /* Update the version number of the log packet */

    /* Indicate whether gRICE is in use */
    if (hdrsrcheq_is_grice_enabled())
    {
      log_ptr->eq_mode = 1;
    }
    else
    {
      log_ptr->eq_mode = 0;
    }

    log_ptr->num_carriers = carrier_cnt;
      /* Store the number of carriers logged */

    log_ptr->str = str;
      /* Store the slot count corresponding to the first index of this 
         buffer */

    carrier_cnt = 0;
      /* Reset the carrier count; to be used as array index */

    /* For each Sub active set */
    for ( subaset_idx = 0; 
          subaset_idx < hdrsrchset.subaset_cnt; ++subaset_idx )
    {

      demod_idx = hdrsrchset.subaset[subaset_idx].demod_idx;
        /* Get the demod index from subaset index */

      c_ptr = &hdrsrchlog_slot_logging.mc_eq_ci_buffer[demod_idx][first_index];
        /* Initialize the convenience pointer */

      /* If we have collected valid information in the buffer for this
         demod carrier */
      if ( HDRSRCHLOG_CARRIER_LOG_BUFF_IS_VALID( demod_idx, cbuff_valid_mask ) )
      {
        /* Fill-in the carrier information */
        log_ptr->per_carrier[carrier_cnt].carrier_id.channel_id = demod_idx;
        log_ptr->per_carrier[carrier_cnt].carrier_id.band_class =
          hdrsrchset.subaset[subaset_idx].channel.band;
        log_ptr->per_carrier[carrier_cnt].carrier_id.channel_no =
          hdrsrchset.subaset[subaset_idx].channel.chan_num;

        /*--------------------------------------------------------------
                           Per-slot Equalizer C2I Logging
         -------------------------------------------------------------*/

#ifdef FEATURE_MEMCPY_REMOVAL
        memscpy(
          ( void* ) ( &log_ptr->per_carrier[carrier_cnt].eq_data[0] ),
           sizeof(log_ptr->per_carrier[carrier_cnt].eq_data), 
          ( void* ) ( c_ptr ),
          ( sizeof ( hdrlog_per_slot_eq_ci_buffer_type ) * 
            HDRLOG_NUM_SLOTS_IN_FRAME ) );
#else
        memcpy(
          ( void* ) ( &log_ptr->per_carrier[carrier_cnt].eq_data[0] ),
          ( void* ) ( c_ptr ),
          ( sizeof ( hdrlog_per_slot_eq_ci_buffer_type ) * 
            HDRLOG_NUM_SLOTS_IN_FRAME ) );
#endif /* FEATURE_MEMCPY_REMOVAL */
          /* Copy one frame worth of data for this carrier to log buffer. */

        carrier_cnt++;
          /* One more carrier logged */

      } /* If carrier buffer is valid */

      memset( (void*) c_ptr,
              0,
              ( sizeof( hdrlog_per_slot_eq_ci_buffer_type ) * 
                HDRLOG_NUM_SLOTS_IN_FRAME ) );
        /* Irrespective of carrier buffer validity, clear the data accumulated
           over the logged frame for this carrier */

    } /* for each sub active set */

    log_commit( (log_type *) log_ptr );
      /* Submit the log to DIAG */

  } /* log_ptr */

} /* hdrsrchlog_log_mc_eq_ci_buffer */


/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_SEARCH_LOG_REC

DESCRIPTION  : Fill in the information for the search log record.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_update_search_log_rec
(
  LOG_HDR_SEARCH_DATA_TYPE2_C_type *log_ptr
  /* log pointer to the finger log record type */
)
{
  int8                             p;          /* pilot loop variable       */
  int8                             c;          /* count the searched pilots */
  const sys_channel_type           *channel;   /* Pointer to channel info   */
  log_hdr_search_data_type         *pilot_ptr; /* Convenience pointer       */
  hdrsrchdrv_srch_list_struct_type *srch_ptr;  /* Convenience pointer       */
  int8                             pk;         /* path peak loop variable   */
  uint8                            asp_idx;    /* Active set pilot index    */

#ifdef FEATURE_HDR_REVB
  uint8                            chan_cnt;   /* Number of the channels    */
  const hdrsrch_demod_idx_type      *demod_list; 
                                               /* demod index in use        */
#endif /* FEATURE_HDR_REVB */

  hdrsrch_demod_idx_type           demod_idx = 
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
    /* Demod index for IDLE subaset */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifdef FEATURE_HDR_REVB
  channel = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list );
    /* Grab pointer to primary chain's channel. */
#else
  channel = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN );
    /* Grab pointer to primary chain's channel. */
#endif /* FEATURE_HDR_REVB */

  log_ptr->version              = HDRSRCHLOG_SEARCH_DATA_TYPE2_VER;
    /* Version number of the search data packet. */

  log_ptr->srch_state           = hdrsrch_state_id;
  log_ptr->srch_substate        = 0; /* TBD: Yet to be implemented */
  log_ptr->srch_win             = hdrsrchdrv.srch_list[0].srch_task.win_size;
  log_ptr->cdma_freq.chan_no    = channel->chan_num;
  log_ptr->cdma_freq.band_class = channel->band;
  log_ptr->srch_info.mstrx8     = hdrsrchdrv.mstr_x8;
  log_ptr->best_asp_idx = HDRASP_GET_BASP_FL( demod_idx );
    /* Get the Best Forward link ASP Index */

    /* KW fix : Limit the list_cnt to MAX */
  for ( p = 0, c = 0; (p < hdrsrchdrv.list_cnt) && (p < HDR_MAX_SRCH_CNT); p++ )
  {
    pilot_ptr = &log_ptr->pilots[c];
      /* Convenience pointer */

    srch_ptr  = &hdrsrchdrv.srch_list[p];
      /* Convenience pointer */

    if ( srch_ptr->sect_ptr != NULL )
    {
      /* Fill in the pilot search results in the log buffer */
      pilot_ptr->pilot_pn.pn               = srch_ptr->srch_task.pn_offset;
      pilot_ptr->pilot_pn.ant_sel          = hdrsrchdrv.params.default_ant;

      if ( hdrsrch_state_id != HDRSRCH_ACQ_STATE )
      {
        pilot_ptr->pilot_pn.pilot_set      = srch_ptr->sect_ptr->set;
      }
      else
      {
        pilot_ptr->pilot_pn.pilot_set      = HDR_EMPTYSET;
      }

      /*-------------------------------------------*
       * Log win center and earliest peak position *
       *-------------------------------------------*/

      pilot_ptr->win_center_x2              = srch_ptr->sect_ptr->win_cenx2;
      pilot_ptr->earliest_pos_x2            = srch_ptr->sect_ptr->peak_pos;

      for ( pk = 0; pk < HDR_MAX_SRCH_PEAKS; pk++ )
      {
        /* Position of search peak in chipx2 units relative to MSTR */
        pilot_ptr->peak_info.peak_pos[pk]   = srch_ptr->srch_task.res_ptr->posx2[pk];
        pilot_ptr->peak_info.peak_pos[pk]  -= ((hdrsrchdrv.mstr_x8+2)/4);

        pilot_ptr->peak_info.peak_eng[pk]   = srch_ptr->srch_task.res_ptr->eng[pk];
          /* Energy of search peak */
      }
      
      log_ptr->eq.flag  = 0 ;
        /* Default value for the equalizer flag */

      /* Equalizer is On only if NV is set and HDR is in Traffic State */
      if ( hdrsrcheq.flag && hdrsrch_state_id == HDRSRCH_CONNECTED_STATE )
      {
        log_ptr->eq.flag  = 1 ;
          /* Update the equalizer on flag */
      }
      
      /* Log the following only if equalizer is turned on */
      if( log_ptr->eq.flag && srch_ptr->sect_ptr->set == HDR_ASET  )
      {
        asp_idx                             = srch_ptr->sect_ptr->aset.asp_idx;
          /* ASP index */

        pilot_ptr->com_info.com_posx2       = 
                     hdrsrcheq.asp[demod_idx][asp_idx].com_posx2;
        pilot_ptr->com_info.com_posx2      -= ((hdrsrchdrv.mstr_x8+2)/4);
         /* Update Center of mass (COM) chipx2 relative to the MSTR for ASP */

        pilot_ptr->com_info.peaks_used      = 
                     hdrsrcheq.asp[demod_idx][asp_idx].path_used;
          /* Update paths used in center of mass computation for this ASP */

        /* Update pilot energy threshold in COM calculation for this ASP */
        pilot_ptr->com_info.peak_eng_thresh = -2 * HDRSRCHEQ_COM_ENG_THRESH -
                  hdrsrchset_eng_to_ecio( pilot_ptr->peak_info.peak_eng[0] );

      }
      c++;
    }/* if */

  }/* for p */

  /* report all the ASET pilots just searched */
  log_ptr->num_pilots = c;

}/* hdrsrchlog_update_search_log_rec() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_SEARCH_RECORD

DESCRIPTION  : Logs the searcher record. It gets a log record from the diag
               log buffer, fills it with the latest searcher peak information
               and commits the log record to the diag for delivery.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_search_record( void )
{
  LOG_HDR_SEARCH_DATA_TYPE2_C_type *log_ptr;
  /* log pointer to the search log record type */

  int rcnt = 0;   /* record count */
  int p;        /* loop counter */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for ( p = 0; p < hdrsrchdrv.list_cnt; p++ )
  {
    if ( hdrsrchdrv.srch_list[p].sect_ptr != NULL )
    {
      rcnt++;     /* increment record count */
    }
  }/* for */

  /* Allocate Memory for the log buffer */

  log_ptr = (LOG_HDR_SEARCH_DATA_TYPE2_C_type *)
            log_alloc_ex( (log_code_type) LOG_HDR_SEARCH_DATA_TYPE2_C,
                          HDRLOG_VAR_SIZEOF( LOG_HDR_SEARCH_DATA_TYPE2_C_type,
                                             log_hdr_search_data_type,
                                             rcnt ) );

  if ( log_ptr )
  {
    hdrsrchlog_update_search_log_rec( log_ptr );
    /* fill in the search result parameters to the search log record */

    log_commit( (log_type*) log_ptr );
    /* submit the log to DIAG */

  }/* if log_ptr */

}/* hdrsrchlog_log_search_record() */


/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_MC_SEARCH_LOG_REC

DESCRIPTION  : Fill in the information for the multi-carrier search 
               log record.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_update_mc_search_log_rec
(
  LOG_HDR_MC_SEARCH_DATA_C_type *log_ptr,
    /* Log pointer to the finger log record type */

  uint8 pilot_cnts[]
    /* Number of pilot records in each one of the demod carrier */
)
{
  int8 p_idx;          
    /* Pilot loop variable */

  uint8 demod_idx;
    /* Demod Carrier Index */

  hdrlog_per_pilot_search_data_type *pilot_ptr; 
    /* Convenience pointer */

  hdrlog_mc_per_carrier_search_data_type
    *perc_srch_data_log_ptr[HDRSRCH_MAX_DEMOD_CARRIERS_CNT] = { NULL };
    /* Convenience pointer to each per-carrier Searcher data in the log 
     * buffer */

  uint8 pilot_cnts_w[HDRSRCH_MAX_DEMOD_CARRIERS_CNT] = { 0 };
    /* Pilot counts for each one of the carrier (working variable) */

  hdrsrchdrv_srch_list_struct_type *srch_ptr;  
    /* Convenience pointer */

  int8 pk;         
    /* Path peak loop variable */

  uint8 asp_idx;
    /* Active set pilot index */

  uint16 perc_byte_offset = 0;
    /* Per-carrier byte offset */

  uint8 perc_nth_pilot = 0;
    /* Temporary variable to keep track of Nth pilot of a particular carrier */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-----------------------------------
   FILL IN CARRIERS-WIDE INFORMATION 
   -----------------------------------*/
  log_ptr->time_to_search = hdrsrchdrv.time_to_search_sclk;
    /* Start of srch to srch dump timed in sclks */

  log_ptr->srch_state = hdrsrch_state_id;

  log_ptr->srch_substate = 0;
    /* TBD: Yet to be implemented */

  log_ptr->srch_win = hdrsrchdrv.srch_list[0].srch_task.win_size;
    /* Window size varies for each sector in normal triage. We log only
       the ASET window size. */

  log_ptr->srch_info.mstrx8 = hdrsrchdrv.mstr_x8;

  /* Equalizer is On only if NV is set and HDR is in Traffic State */
  if ( ( hdrsrcheq.flag ) && ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE ) )
  {
    log_ptr->eq.enabled  = 1;
      /* Update the equalizer on flag */
  }
  else
  {
    log_ptr->eq.enabled  = 0;
      /* Default value for the equalizer flag */
  }

  /* For each demod carrier, Initialize the convenience pointers for each
   * one of the per-carrier data in the log buffer and store the per-carrier
   * information in the log buffer */
  for ( demod_idx = 0; 
        demod_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; ++demod_idx )
  {
    /* Do we have any pilots in the search list for this demod carrier */
    if ( pilot_cnts[demod_idx] > 0 )
    {      
      perc_srch_data_log_ptr[demod_idx] =
        (hdrlog_mc_per_carrier_search_data_type*)
        (((uint8*) &log_ptr->carriers[0]) + perc_byte_offset);
        /* Initialize the convenience pointer for this demod carrier */

      (perc_srch_data_log_ptr[demod_idx])->num_pilots = pilot_cnts[demod_idx];
        /* Store number of pilots in each demod carrier */

      (perc_srch_data_log_ptr[demod_idx])->best_asp_idx = 
        HDRASP_GET_BASP_FL(demod_idx);
        /* Get the Best Forward link ASP Index */
      
      perc_byte_offset += ( sizeof ( hdrlog_mc_per_carrier_search_data_type ) + 
                            ( sizeof ( hdrlog_per_pilot_search_data_type ) * 
                              ( pilot_cnts[demod_idx] - 1 ) ) );
        /* Update num_bytes_copied_so_far to point to next per-carrier
         * data in the log buffer */

    } /* if (pilot_cnts[demod_idx] > 0) */

  } /* for each carrier */

  /* For each pilot in the search list... */
  for ( p_idx = 0; (p_idx < hdrsrchdrv.list_cnt) && (p_idx < HDR_MAX_SRCH_CNT); p_idx++ )
  {
    srch_ptr  = &hdrsrchdrv.srch_list[p_idx];
      /* Convenience pointer */

    if ( srch_ptr->sect_ptr != NULL )
    {
      perc_nth_pilot = pilot_cnts_w[srch_ptr->srch_task.carrier_idx];
        /* This carrier's nth pilot */

      ++pilot_cnts_w[srch_ptr->srch_task.carrier_idx];
        /* Increment the pilot count for this carrier */

      pilot_ptr =
        &( ( perc_srch_data_log_ptr[srch_ptr->srch_task.carrier_idx] )->
          pilots[perc_nth_pilot] );
        /* Convenience pointer to per-pilot searcher data */

      /* Copy the carrier ID information only for first pilot. Should
       * be same for rest of the pilots of the same carrier  */
      if (perc_nth_pilot == 0)
      {
        perc_srch_data_log_ptr[srch_ptr->srch_task.carrier_idx]->carrier_id.channel_id = 
          srch_ptr->srch_task.carrier_idx;
        perc_srch_data_log_ptr[srch_ptr->srch_task.carrier_idx]->carrier_id.channel_no =
          srch_ptr->sect_ptr->chan.chan_num;
        perc_srch_data_log_ptr[srch_ptr->srch_task.carrier_idx]->carrier_id.band_class =
          srch_ptr->sect_ptr->chan.band;
      }

      /* Fill in the pilot search results in the log buffer */

      pilot_ptr->pilot_pn.pn = srch_ptr->srch_task.pn_offset;

      /* _ETV_ Is this a param common to all pilots? */
      pilot_ptr->pilot_pn.ant_sel = hdrsrchdrv.params.default_ant;

      if ( hdrsrch_state_id != HDRSRCH_ACQ_STATE )
      {
        pilot_ptr->pilot_pn.pilot_set = srch_ptr->sect_ptr->set;
      }
      else
      {
        pilot_ptr->pilot_pn.pilot_set = HDR_EMPTYSET;
      }

      /*----------------------------------------------*
       * Log window center and earliest peak position *
       *----------------------------------------------*/
      pilot_ptr->win_center_x2 = srch_ptr->sect_ptr->win_cenx2;
      pilot_ptr->earliest_pos_x2 = srch_ptr->sect_ptr->peak_pos;

      /* For each search peak... */
      for ( pk = 0; pk < HDR_MAX_SRCH_PEAKS; pk++ )
      {
        /* Position of search peak in chipx2 units relative to MSTR */
        pilot_ptr->peak_info.peak_pos[pk] = srch_ptr->srch_task.res_ptr->posx2[pk];
        pilot_ptr->peak_info.peak_pos[pk] -= ((hdrsrchdrv.mstr_x8+2)/4);

        pilot_ptr->peak_info.peak_eng[pk] = srch_ptr->srch_task.res_ptr->eng[pk];
          /* Energy of search peak */

      }

      /* Log the following only if equalizer is turned on */
      if( log_ptr->eq.enabled && ( srch_ptr->sect_ptr->set == HDR_ASET )  )
      {
        asp_idx = srch_ptr->sect_ptr->aset.asp_idx;
          /* ASP index */

        pilot_ptr->com_info.com_posx2 = 
          hdrsrcheq.asp[srch_ptr->srch_task.carrier_idx][asp_idx].com_posx2;

        pilot_ptr->com_info.com_posx2 -= ((hdrsrchdrv.mstr_x8+2)/4);
          /* Update Center of mass (COM) chipx2 relative to the MSTR for ASP */

        pilot_ptr->com_info.peaks_used =
          hdrsrcheq.asp[srch_ptr->srch_task.carrier_idx][asp_idx].path_used;
          /* Update paths used in center of mass computation for this ASP */
        
        pilot_ptr->com_info.peak_eng_thresh = 
          ( ( -2 * HDRSRCHEQ_COM_ENG_THRESH ) - 
            ( hdrsrchset_eng_to_ecio( pilot_ptr->peak_info.peak_eng[0] ) ) );
          /* Update pilot energy threshold in COM calculation for this ASP */

      } /* if eq turned on */

    } /* if pilot eligible for logging  */

  } /* for each pilot in search list */

} /* hdrsrchlog_update_mc_search_log_rec() */

/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_MC_SEARCH_RECORD

DESCRIPTION  : Logs the searcher record. It gets a log record from the diag
               log buffer, fills it with the latest searcher peak information
               and commits the log record to the diag for delivery.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_mc_search_record ( void )
{
  LOG_HDR_MC_SEARCH_DATA_C_type *log_ptr;
    /* Log pointer to the search log record type */

  int carrier_cnt = 0;
    /* Total number of carriers with non-zero pilot search records for
     * logging */

  int p;
    /* Pilot loop counter */

  uint8 pilot_cnt[HDRSRCH_MAX_DEMOD_CARRIERS_CNT] = { 0 };
    /* Number of pilots in each one of the demod carriers */

  uint8 cum_pilot_cnt = 0;
    /* Total number of pilot search record to be logged across all 
     * demod carriers */

  uint8 demod_idx = 0;
    /* Demod carrier index used as loop counter */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* For each demod carrier... */
  for ( demod_idx = 0; demod_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; ++demod_idx )
  {
    /* For each pilot in the search list... */
    for ( p = 0; p < hdrsrchdrv.list_cnt; p++ )
    {

      if ( ( hdrsrchdrv.srch_list[p].sect_ptr != NULL )             &&
           ( hdrsrchdrv.srch_list[p].srch_task.carrier_idx == demod_idx )      )
      {
        ++pilot_cnt[demod_idx];
          /* Increment pilot count for this demod carrier */
      }

    } /* for each pilot */

    /* Does the demod carrier have any pilot search records to be logged? */
    if ( pilot_cnt[demod_idx] > 0 )
    {
      /* Non-zero pilot count for this demod carrier */

      cum_pilot_cnt += pilot_cnt[demod_idx];
        /* Increment the cumulative pilot count */

      ++carrier_cnt;
        /* Increment the carrier count */
    }

  } /* for each demod carrier */

  /* Allocate Memory for the log buffer */
  log_ptr = ( LOG_HDR_MC_SEARCH_DATA_C_type * ) 
    log_alloc( ( log_code_type ) LOG_HDR_MC_SEARCH_DATA_C, 
               ( sizeof ( LOG_HDR_MC_SEARCH_DATA_C_type ) + 
                 ( sizeof ( hdrlog_mc_per_carrier_search_data_type ) * 
                   ( carrier_cnt - 1 ) ) + 
                 ( sizeof ( hdrlog_per_pilot_search_data_type ) * 
                   ( cum_pilot_cnt - carrier_cnt ) ) ) );
  
  if ( log_ptr )
  {
    log_ptr->version = HDRLOG_MC_SEARCH_DATA_VER;
      /* Version number of the search data packet. */

    log_ptr->slot_count = HDRHSTR_GET_COUNT() % 512 / 2 ;
      /* Store the current slot count computed from HSTR */

    log_ptr->num_carriers = carrier_cnt;
      /* Store the carrier count */

    hdrsrchlog_update_mc_search_log_rec( log_ptr, pilot_cnt );
      /* Fill in the search result parameters to the search log record */

    log_commit( ( log_type* ) log_ptr );
      /* Submit the log to DIAG */

  } /* if log_ptr */

} /* hdrsrchlog_log_mc_search_record() */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_FING_LOG_REC

DESCRIPTION  : Fill in the information for the finger record.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_update_fing_log_rec
(
  LOG_HDR_FINGER_DATA_C_type *log_ptr
  /* log pointer to the finger log record type */
)
{
  int8                       f;
    /* finger loop variable */

  log_hdr_finger_data_type *fptr;
    /* finger pointer */

  hdrsrch_sect_struct_type*  ref_sect = NULL; 
    /* Reference sector pointer */

  hdrsrch_demod_idx_type     demod_idx = 
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
    /* Demod index for IDLE subaset */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrmdsp_check_dsp_clk() )
  {
  hdrsrchfing_read_fing_status();
  /* update the finger data structure with latest finger information */

  log_ptr->srch_state   = hdrsrch_state_id;
  /* Current Srch State */

  log_ptr->mstrx8       = hdrsrchdrv.mstr_x8;
  /* Latest mobile station time reference */

  log_ptr->mstr_err_x8   = HDRMSTR_GET_MSTR_ERROR_X8();
  /* latest mobile station timing error */

  ref_sect = hdrsrchutil_get_ref_sector_ptr();
    /* Get the reference sector pointer */

  if( ref_sect != NULL )
  {
    log_ptr->mstr_pn    = ref_sect->pn_offset;
      /* Get Reference PN offset in the units of 64 PN chips */
  }
  else
  {
    log_ptr->mstr_pn    = HDR_NO_PN_OFFSET;
  }

  log_ptr->num_fings     = hdrsrchfing.params.max_fings;

  for ( f = 0; f < hdrsrchfing.params.max_fings; f++ )
  {
    fptr = &log_ptr->fing_data[f];
    /* convenience pointer */

    fptr->pilot_pn             = 
                       hdrsrchfing.fings[demod_idx][f].pn_offset;
    fptr->posx8                = 
                           hdrsrchfing.fings[demod_idx][f].posx8;
    fptr->posx8               -= hdrsrchdrv.mstr_x8;
    fptr->rssi                 = 
                            hdrsrchfing.fings[demod_idx][f].rssi;
    fptr->fing_info.locked     = 
                (hdrsrchfing.fings[demod_idx][f].locked ? 1 : 0);
    fptr->fing_info.antenna    = 
                hdrsrchfing.fings[demod_idx][f].dsp_regs.ant_sel;
    fptr->fing_info.diversity  = 
                    hdrsrchfing.fings[demod_idx][f].dsp_regs.div;
    fptr->fing_info.fing_index = f;
    fptr->fing_sect.asp        = 
                hdrsrchfing.fings[demod_idx][f].dsp_regs.asp_idx;
    fptr->fing_sect.rpc        = 
           hdrsrchfing.fings[demod_idx][f].dsp_regs.rpc_cell_idx;

    /* Fill in Ant0, Ant1 RSSIs for this finger pair if div is enabled */
    fptr->ant0_rssi            = HDRFING_GET_ANT_RSSI( demod_idx, 
                                                       f, 
                                                       HDRMDSP_ANT_0 );
    fptr->ant1_rssi            = HDRFING_GET_ANT_RSSI( demod_idx, 
                                                       f, 
                                                       HDRMDSP_ANT_1 );
  }/* for f */

  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "MDSP clock is off");
  }  /* if hdrmdsp_check_dsp_clk() */

}/* hdrsrchlog_update_fing_log_rec() */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_FING_RECORD

DESCRIPTION  : Logs the finger record. It gets a log record from the diag
               log buffer, fills it with the latest finger information and
               commits the log record to the diag for delivery.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_fing_record( void )
{

  LOG_HDR_FINGER_DATA_C_type *log_ptr;
  /* log pointer to the finger log record type */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  log_ptr = (LOG_HDR_FINGER_DATA_C_type *)
            log_alloc_ex( (log_code_type) LOG_HDR_FINGER_DATA_C,
                          HDRLOG_VAR_SIZEOF( LOG_HDR_FINGER_DATA_C_type,
                                             log_hdr_finger_data_type,
                                             hdrsrchfing.params.max_fings ) );

  if ( log_ptr )
  {
    hdrsrchlog_update_fing_log_rec( log_ptr );
    /* update the fields in the log record */

    log_commit( (log_type *) log_ptr );
    /* submit the log to DIAG */

  }/* if log_ptr */


}/* hdrsrchlog_log_fing_record() */

/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_MC_FING_LOG_REC

DESCRIPTION  : Fill in the information for the multi-carrier finger record.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_update_mc_fing_log_rec
(
  LOG_HDR_MC_FINGER_DATA_C_type *log_ptr
    /* Log pointer to the finger log record type */
)
{
  int8  fing_idx;
    /* Finger loop variable */

  hdrlog_per_carrier_finger_data_type *perc_ptr;
    /* Convenience pointer to per carrier information*/

  hdrlog_per_finger_data_type *perf_ptr;
    /* Convenience pointer to per finger records */

  int8  demod_idx;
    /* Demod Carrier loop variable */

  int8  sa;
    /* Subaset index */

  int8  carrier_num = 0;
    /* Number of carriers */

  hdrsrch_sect_struct_type*  ref_sect = NULL; 
    /* Reference sector pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrmdsp_check_dsp_clk() )
  {
    hdrsrchfing_read_fing_status(); 
      /* Update the finger data structure with latest finger information */

    log_ptr->version = HDRLOG_MC_FINGER_DATA_VER;
      /* Version number of the log packet */
    
    log_ptr->srch_state = hdrsrch_state_id;
      /* Current Srch State */
         
    log_ptr->mstr_x8 = hdrsrchdrv.mstr_x8;
      /* Latest mobile station time reference */
           
    log_ptr->mstr_err_x8 = HDRMSTR_GET_MSTR_ERROR_X8();
      /* Latest mobile station timing error */
        
    ref_sect = hdrsrchutil_get_ref_sector_ptr();
      /* Get the reference sector pointer */

    if( ref_sect != NULL )
    {
      log_ptr->mstr_pn    = ref_sect->pn_offset;
        /* Get Reference PN offset in the units of 64 PN chips */
    }
    else
    {
      log_ptr->mstr_pn    = HDR_NO_PN_OFFSET;
    }
     
    perc_ptr = &log_ptr->perc_finger_data[0];
      /* Initialize the per-carrier pointer to per-c data of first carrier */

    /* For each demod carrier */
    for ( demod_idx = 0; 
          demod_idx < hdrsrchfing.params.max_demod_carriers; 
          demod_idx++ )
    {

      if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED(demod_idx) ) 
      {
        carrier_num++;
          /* Increment carrier number */

        /* The hdrsrchfings.fings array is 2-d for each demod carrier on 
           one dimension and for each finger on the */

        /* Carrier information: Demod ID */
        perc_ptr->carrier_id.channel_id = demod_idx;

        for ( sa =0; (sa < hdrsrchset.subaset_cnt) && (sa < HDRSRCH_MAX_NUM_CARRIERS) ; sa++ ) 
        {
          if ( hdrsrchset.subaset[sa].demod_idx == demod_idx ) 
          {
            break;
          }
        }
        if ( sa == HDRSRCH_MAX_NUM_CARRIERS ) 
        {
          /* The demod carrier index is not used by any subaset */
          ERR_FATAL("Demod carrier is not used by any subaset", 0, 0, 0 );
        }

        /* Carrier information: BandClass and Channel number */
        perc_ptr->carrier_id.band_class = hdrsrchset.subaset[sa].channel.band;
        perc_ptr->carrier_id.channel_no = hdrsrchset.subaset[sa].channel.chan_num;

        perc_ptr->sub_aset_id = hdrsrchset.subaset[sa].tca_idx;
          /* TCA Index */

        perc_ptr->num_fingers = hdrsrchfing.params.max_fings;

        perf_ptr = &perc_ptr->finger_data[0];
          /* Initialize the convenience pointer to start of current finger */

        /* For each finger */
        for ( fing_idx = 0; fing_idx < hdrsrchfing.params.max_fings; fing_idx++ )
        {

          perf_ptr->pilot_pn = hdrsrchfing.fings[demod_idx][fing_idx].pn_offset;
          perf_ptr->rtc_pos_x8 = hdrsrchfing.fings[demod_idx][fing_idx].posx8;
          perf_ptr->rtc_pos_x8 -= hdrsrchdrv.mstr_x8;
          perf_ptr->rssi = hdrsrchfing.fings[demod_idx][fing_idx].rssi;

          perf_ptr->fing_info.locked = 
            (hdrsrchfing.fings[demod_idx][fing_idx].locked ? 1 : 0);
          perf_ptr->fing_info.antenna = 
            hdrsrchfing.fings[demod_idx][fing_idx].dsp_regs.ant_sel;
          perf_ptr->fing_info.diversity = 
            hdrsrchfing.fings[demod_idx][fing_idx].dsp_regs.div;
          perf_ptr->fing_info.fing_index = fing_idx;

          perf_ptr->fing_sect.asp_idx = 
            hdrsrchfing.fings[demod_idx][fing_idx].dsp_regs.asp_idx;
          perf_ptr->fing_sect.rpc_idx = 
            hdrsrchfing.fings[demod_idx][fing_idx].dsp_regs.rpc_cell_idx;

          /* Fill in Ant0,Ant1 RSSIs */
          perf_ptr->ant0_rssi =
            HDRFING_GET_ANT_RSSI( demod_idx, fing_idx, HDRMDSP_ANT_0 );

          perf_ptr->ant1_rssi = 
            HDRFING_GET_ANT_RSSI( demod_idx, fing_idx, HDRMDSP_ANT_1 );

           perf_ptr->ant0_phase =
            HDRFING_GET_ANT_PHASE( demod_idx, fing_idx, HDRMDSP_ANT_0 );

          perf_ptr->ant1_phase =
            HDRFING_GET_ANT_PHASE( demod_idx, fing_idx, HDRMDSP_ANT_1 );

          perf_ptr++;

        } /* for each finger */

        perc_ptr = (hdrlog_per_carrier_finger_data_type*) perf_ptr;
          /* We are done with all fingers of this carrier. Next byte pointed to by
             perf_ptr should belong to next carrier. Re-initialize the pointer */

      } /* if (HDRSRCHFING_DEMOD_CARRIER_ENABLED()) */
    } /* for each demod carrier */

    log_ptr->num_carriers = carrier_num;
      /* Number of carriers in the log packet */

  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "MDSP clock is off");
  } /* if hdrmdsp_check_dsp_clk() */

} /* hdrsrchlog_update_mc_fing_log_rec() */

/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_MC_FING_RECORD

DESCRIPTION  : Logs the multi-carrier finger record. It gets a log record 
               from the diag log buffer, fills it with the latest finger
               information and commits the log record to the diag for
               delivery.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_mc_fing_record( void )
{  
  LOG_HDR_MC_FINGER_DATA_C_type *log_ptr;
    /* Log pointer to the finger log record type */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Allocate memory for log packet */
  log_ptr = ( LOG_HDR_MC_FINGER_DATA_C_type * )
    log_alloc ( LOG_HDR_MC_FINGER_DATA_C, 
                sizeof ( LOG_HDR_MC_FINGER_DATA_C_type ) + 
                ( ( hdrsrchfing.params.max_demod_carriers - 1 ) * 
                  sizeof ( hdrlog_per_carrier_finger_data_type ) ) + 
                ( ( ( hdrsrchfing.params.max_fings - 1 ) * 
                    hdrsrchfing.params.max_demod_carriers ) * 
                  sizeof ( hdrlog_per_finger_data_type ) ) );   

  if ( log_ptr )
  {    
    hdrsrchlog_update_mc_fing_log_rec( log_ptr );
      /* Update the fields in the log record */
    
    log_commit( ( log_type * ) log_ptr ); 
      /* Submit the log to DIAG */

  } /* if log_ptr */
    
} /* hdrsrchlog_log_mc_fing_record() */

/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_PILOT_SET_V2_LOG_REC

DESCRIPTION  : Fill in the information for the pilot set version 2 log record.
               Note: for ASET, it logs single carrier subaset[0] info only.
DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_update_pilot_set_v2_log_rec
(
  LOG_HDR_PILOT_SETS_V2_C_type *log_ptr
  /* log pointer to the pilot sets log record type */
)
{
  int8      p;            /* pilot loop variable */
  int8      c = 0;        /* count of number of pilots reported */

  hdrsrch_sect_struct_type *sptr;   /* handy pointer */

#ifdef FEATURE_HDR_REVB
  uint8     i;            /* loop index */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  log_ptr->pn_inc          = hdrsrch.srch_params.pilot_inc;
#ifdef FEATURE_HDR_REVB
  log_ptr->aset_cnt       =  hdrsrchset_total_aset_cnt();
#else
  log_ptr->aset_cnt        = hdrsrchset.aset_cnt;
#endif /* FEATURE_HDR_REVB */
  log_ptr->cset_cnt        = hdrsrchset.cset_cnt;
  log_ptr->nset_cnt        = hdrsrchset.nset_cnt;

  log_ptr->srch_state      = hdrsrch_state_id;

#ifdef FEATURE_HDR_REVB
  if ( log_ptr->aset_cnt > 0 )
  {
    log_ptr->aset_channel.chan_no = 
                               hdrsrchset.subaset[0].aset[0]->chan.chan_num;
    log_ptr->aset_channel.band_class  = 
                               hdrsrchset.subaset[0].aset[0]->chan.band;
      /* Use subaset0 for Now before the change of logging format for revB */
      /* TBD: include entire channel list for revB*/

    log_ptr->aset_win = hdrsrchutil_get_winsize( hdrsrchset.subaset[0].aset[0] );
  }
  else
  {
    log_ptr->aset_channel.chan_no     = HDRSRCHLOG_PACKED_INVALID_CHAN;
    log_ptr->aset_channel.band_class  = HDRSRCHLOG_PACKED_INVALID_BAND;

    log_ptr->aset_win = 0;
  }

  for( i=0; i<hdrsrchset.subaset_cnt;i++)
  {
    /* TBD: need include more info about subaset for revB logging */

    for ( p = 0; p < hdrsrchset.subaset[i].aset_cnt; p++ )
    {
      sptr = hdrsrchset.subaset[i].aset[p];
        /* convenience pointer */

      /* Update ASET record */
      /*lint -save -e661 -e662 Ignore Possible access of out-of-bounds pointer*/
      log_ptr->set_info[c].aset.pilot_pn             = sptr->pn_offset;
      log_ptr->set_info[c].aset.total_eng            = sptr->total_eng;
      log_ptr->set_info[c].aset.index_info.asp_index = sptr->aset.asp_idx;
      log_ptr->set_info[c].aset.index_info.rpc_index = sptr->aset.rpc_cell_idx;
      log_ptr->set_info[c].aset.index_info.drc_cover = sptr->aset.drc_cover;

      log_ptr->set_info[c].aset.mac_info.index       = sptr->aset.mac_idx;

      log_ptr->set_info[c].aset.mac_info.droptimer_expired =
                                       ( sptr->drop_timer.expired ? 1 : 0 );

      log_ptr->set_info[c].aset.mac_info.droptimer_active =
                                       ( sptr->drop_timer.active ? 1 : 0 );

      log_ptr->set_info[c].aset.win_center_x2        = sptr->win_cenx2;
      /* lint -restore */

      c++;

    }/* for p */
  } /* for i */
#else
  if ( hdrsrchset.aset_cnt > 0 )
  {
    log_ptr->aset_channel.chan_no     = hdrsrchset.aset[0]->chan.chan_num;
    log_ptr->aset_channel.band_class  = hdrsrchset.aset[0]->chan.band;

    log_ptr->aset_win = hdrsrchutil_get_winsize( hdrsrchset.aset[0] );
  }
  else
  {
    /* Both of these values denote an invalid channel number and band class. */
    log_ptr->aset_channel.chan_no     = HDRSRCHLOG_PACKED_INVALID_CHAN;
    log_ptr->aset_channel.band_class  = HDRSRCHLOG_PACKED_INVALID_BAND;

    log_ptr->aset_win = 0;
  }

  for ( p = 0; p < hdrsrchset.aset_cnt; p++ )
  {
    sptr = hdrsrchset.aset[p];
    /* convenience pointer */

    /* Update ASET record */
    /*lint -save -e661 -e662 Ignore Possible access of out-of-bounds pointer*/
    log_ptr->set_info[c].aset.pilot_pn             = sptr->pn_offset;
    log_ptr->set_info[c].aset.total_eng            = sptr->total_eng;
    log_ptr->set_info[c].aset.index_info.asp_index = sptr->aset.asp_idx;
    log_ptr->set_info[c].aset.index_info.rpc_index = sptr->aset.rpc_cell_idx;
    log_ptr->set_info[c].aset.index_info.drc_cover = sptr->aset.drc_cover;

    log_ptr->set_info[c].aset.mac_info.index       = sptr->aset.mac_idx;

    log_ptr->set_info[c].aset.mac_info.droptimer_expired =
                                       ( sptr->drop_timer.expired ? 1 : 0 );

    log_ptr->set_info[c].aset.mac_info.droptimer_active =
                                       ( sptr->drop_timer.active ? 1 : 0 );

    log_ptr->set_info[c].aset.win_center_x2        = sptr->win_cenx2;
    /* lint -restore */

    c++;

  }/* for p */
#endif /* FEATURE_HDR_REVB */

  if ( hdrsrchset.cset_cnt > 0 )
  {
    log_ptr->cset_win = hdrsrchutil_get_winsize( hdrsrchset.cset[0] );
  }
  else
  {
    log_ptr->cset_win = 0;
  }

  for ( p = 0; p < hdrsrchset.cset_cnt; p++ )
  {
    sptr = hdrsrchset.cset[p];
    /* convenience pointer */

    /* Update CSET record */
    /*lint -save -e661 -e662 Ignore Possible access of out-of-bounds pointer*/
    log_ptr->set_info[c].cset.pilot_pn                   = sptr->pn_offset;
    log_ptr->set_info[c].cset.total_eng                  = sptr->total_eng;
    log_ptr->set_info[c].cset.cset_freq.chan_no          = sptr->chan.chan_num;
    log_ptr->set_info[c].cset.cset_freq.band_class       = sptr->chan.band;

    log_ptr->set_info[c].cset.cset_info.droptimer_expired=
                                       ( sptr->drop_timer.expired ? 1 : 0 );

    log_ptr->set_info[c].cset.cset_info.droptimer_active =
                                       ( sptr->drop_timer.active ? 1 : 0 );

    log_ptr->set_info[c].cset.win_center_x2          = sptr->win_cenx2;
    /* lint -restore */
    c++;

  }/* for p */

  for ( p = 0; p < hdrsrchset.nset_cnt; p++ )
  {
    sptr = hdrsrchset.nset[p];
    /* convenience pointer */

    /* Update NSET record */
    /*lint -save -e661 -e662 Ignore Possible access of out-of-bounds pointer*/
    log_ptr->set_info[c].nset.pilot_pn              = sptr->pn_offset;
    log_ptr->set_info[c].nset.total_eng             = sptr->total_eng;
    log_ptr->set_info[c].nset.nset_freq.chan_no     = sptr->chan.chan_num;
    log_ptr->set_info[c].nset.nset_freq.band_class  = sptr->chan.band;

    if ( sptr->nset.win_offset == HDRSRCH_WINOFFSET_DEFAULT )
    {
      log_ptr->set_info[c].nset.win_info.offset     = HDRSRCH_WINOFFSET_NONE;
    }
    else
    {
      log_ptr->set_info[c].nset.win_info.offset     = sptr->nset.win_offset;
    }

    log_ptr->set_info[c].nset.win_info.age          = sptr->nset.age;
    log_ptr->set_info[c].nset.win       = hdrsrchutil_get_winsize( sptr );
    /* lint -restore */
  
    c++;
  }/* for p */

  /* search window size for remaining set */
  if ( hdrsrchset.rset )
  {
    log_ptr->rset_win = hdrsrchutil_get_winsize( hdrsrchset.rset );
  }

}/* hdrsrchlog_update_pilot_set_v2_log_rec() */

/* <EJECT> */



/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_PILOT_SET_LOG_REC

DESCRIPTION  : Fill in the information for the pilot set log record (v3).
               Note: for ASET, it logs single carrier subaset[0] info only.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_update_pilot_set_log_rec
(
  LOG_HDR_PILOT_SETS_C_type *log_ptr
  /* log pointer to the pilot sets log record type */
)
{
  int8      p;            /* pilot loop variable */
  int8      c = 0;        /* count of number of pilots reported */

  hdrsrch_sect_struct_type *sptr;   /* handy pointer */

#ifdef FEATURE_HDR_REVB
  uint8     i;            /* loop index */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  log_ptr->version         = HDRLOG_PILOT_SETS_VER;

  log_ptr->pn_inc          = hdrsrch.srch_params.pilot_inc;
#ifdef FEATURE_HDR_REVB
  log_ptr->aset_cnt       =  hdrsrchset_total_aset_cnt();
#else
  log_ptr->aset_cnt        = hdrsrchset.aset_cnt;
#endif /* FEATURE_HDR_REVB */
  log_ptr->cset_cnt        = hdrsrchset.cset_cnt;
  log_ptr->nset_cnt        = hdrsrchset.nset_cnt;

  log_ptr->srch_state      = hdrsrch_state_id;

#ifdef FEATURE_HDR_REVB
  if ( log_ptr->aset_cnt > 0 )
  {
    log_ptr->aset_channel.chan_no = 
                               hdrsrchset.subaset[0].aset[0]->chan.chan_num;
    log_ptr->aset_channel.band_class  = 
                               hdrsrchset.subaset[0].aset[0]->chan.band;
      /* Use subaset0 for Now before the change of logging format for revB */
      /* TBD: include entire channel list for revB*/

    log_ptr->aset_win = hdrsrchutil_get_winsize( hdrsrchset.subaset[0].aset[0] );
  }
  else
  {
    log_ptr->aset_channel.chan_no     = HDRSRCHLOG_PACKED_INVALID_CHAN;
    log_ptr->aset_channel.band_class  = HDRSRCHLOG_PACKED_INVALID_BAND;

    log_ptr->aset_win = 0;
  }

  for( i=0; i<hdrsrchset.subaset_cnt;i++)
  {
    /* TBD: need include more info about subaset for revB logging */

    for ( p = 0; p < hdrsrchset.subaset[i].aset_cnt; p++ )
    {
      sptr = hdrsrchset.subaset[i].aset[p];
        /* convenience pointer */

      /* Update ASET record */
      /*lint -save -e661 -e662 Ignore Possible access of out-of-bounds pointer*/
      log_ptr->set_info[c].aset.pilot_pn             = sptr->pn_offset;
      log_ptr->set_info[c].aset.total_eng            = sptr->total_eng;
      log_ptr->set_info[c].aset.ofs_cond_eng         = sptr->pil_filter.total_eng;
      log_ptr->set_info[c].aset.index_info.asp_index = sptr->aset.asp_idx;
      log_ptr->set_info[c].aset.index_info.rpc_index = sptr->aset.rpc_cell_idx;
      log_ptr->set_info[c].aset.index_info.drc_cover = sptr->aset.drc_cover;

      log_ptr->set_info[c].aset.mac_info.index       = sptr->aset.mac_idx;

      log_ptr->set_info[c].aset.mac_info.droptimer_expired =
                                       ( sptr->drop_timer.expired ? 1 : 0 );

      log_ptr->set_info[c].aset.mac_info.droptimer_active =
                                       ( sptr->drop_timer.active ? 1 : 0 );

      log_ptr->set_info[c].aset.win_center_x2        = sptr->win_cenx2;
      /* lint -restore */

      c++;

    }/* for p */
  } /* for i */
#else
  if ( hdrsrchset.aset_cnt > 0 )
  {
    log_ptr->aset_channel.chan_no     = hdrsrchset.aset[0]->chan.chan_num;
    log_ptr->aset_channel.band_class  = hdrsrchset.aset[0]->chan.band;

    log_ptr->aset_win = hdrsrchutil_get_winsize( hdrsrchset.aset[0] );
  }
  else
  {
    /* Both of these values denote an invalid channel number and band class. */
    log_ptr->aset_channel.chan_no     = HDRSRCHLOG_PACKED_INVALID_CHAN;
    log_ptr->aset_channel.band_class  = HDRSRCHLOG_PACKED_INVALID_BAND;

    log_ptr->aset_win = 0;
  }

  for ( p = 0; p < hdrsrchset.aset_cnt; p++ )
  {
    sptr = hdrsrchset.aset[p];
    /* convenience pointer */

    /* Update ASET record */
    /*lint -save -e661 -e662 Ignore Possible access of out-of-bounds pointer*/
    log_ptr->set_info[c].aset.pilot_pn             = sptr->pn_offset;
    log_ptr->set_info[c].aset.total_eng            = sptr->total_eng;
    log_ptr->set_info[c].aset.index_info.rpc_index = sptr->aset.rpc_cell_idx;
    log_ptr->set_info[c].aset.index_info.drc_cover = sptr->aset.drc_cover;

    log_ptr->set_info[c].aset.mac_info.index       = sptr->aset.mac_idx;

    log_ptr->set_info[c].aset.mac_info.droptimer_expired =
                                       ( sptr->drop_timer.expired ? 1 : 0 );

    log_ptr->set_info[c].aset.mac_info.droptimer_active =
                                       ( sptr->drop_timer.active ? 1 : 0 );

    log_ptr->set_info[c].aset.win_center_x2        = sptr->win_cenx2;
    /* lint -restore */

    c++;

  }/* for p */
#endif /* FEATURE_HDR_REVB */

  if ( hdrsrchset.cset_cnt > 0 )
  {
    log_ptr->cset_win = hdrsrchutil_get_winsize( hdrsrchset.cset[0] );
  }
  else
  {
    log_ptr->cset_win = 0;
  }

  for ( p = 0; p < hdrsrchset.cset_cnt; p++ )
  {
    sptr = hdrsrchset.cset[p];
    /* convenience pointer */

    /* Update CSET record */
    /*lint -save -e661 -e662 Ignore Possible access of out-of-bounds pointer*/
    log_ptr->set_info[c].cset.pilot_pn                   = sptr->pn_offset;
    log_ptr->set_info[c].cset.total_eng                  = sptr->total_eng;
    log_ptr->set_info[c].cset.ofs_cond_eng               = sptr->pil_filter.total_eng;    log_ptr->set_info[c].cset.cset_freq.chan_no          = sptr->chan.chan_num;
    log_ptr->set_info[c].cset.cset_freq.band_class       = sptr->chan.band;

    log_ptr->set_info[c].cset.cset_info.droptimer_expired=
                                       ( sptr->drop_timer.expired ? 1 : 0 );

    log_ptr->set_info[c].cset.cset_info.droptimer_active =
                                       ( sptr->drop_timer.active ? 1 : 0 );

    log_ptr->set_info[c].cset.win_center_x2          = sptr->win_cenx2;
    /* lint -restore */
    c++;

  }/* for p */

  for ( p = 0; p < hdrsrchset.nset_cnt; p++ )
  {
    sptr = hdrsrchset.nset[p];
    /* convenience pointer */

    /* Update NSET record */
    /*lint -save -e661 -e662 Ignore Possible access of out-of-bounds pointer*/
    log_ptr->set_info[c].nset.pilot_pn              = sptr->pn_offset;
    log_ptr->set_info[c].nset.total_eng             = sptr->total_eng;
    log_ptr->set_info[c].nset.ofs_cond_eng          = sptr->pil_filter.total_eng;
    log_ptr->set_info[c].nset.nset_freq.chan_no     = sptr->chan.chan_num;
    log_ptr->set_info[c].nset.nset_freq.band_class  = sptr->chan.band;

    if ( sptr->nset.win_offset == HDRSRCH_WINOFFSET_DEFAULT )
    {
      log_ptr->set_info[c].nset.win_info.offset     = HDRSRCH_WINOFFSET_NONE;
    }
    else
    {
      log_ptr->set_info[c].nset.win_info.offset     = sptr->nset.win_offset;
    }

    log_ptr->set_info[c].nset.win_info.age          = sptr->nset.age;
    log_ptr->set_info[c].nset.win       = hdrsrchutil_get_winsize( sptr );
    /* lint -restore */
  
    c++;
  }/* for p */

  /* search window size for remaining set */
  if ( hdrsrchset.rset )
  {
    log_ptr->rset_win = hdrsrchutil_get_winsize( hdrsrchset.rset );
  }

}/* hdrsrchlog_update_pilot_set_log_rec() */

/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_PILOT_SET_RECORD

DESCRIPTION  : Logs the pilot set record. It gets a log record from the diag
               log buffer, fills it with the latest pilot set information
               and commits the log record to the diag for delivery.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_pilot_set_record( void )
{

  LOG_HDR_PILOT_SETS_V2_C_type *log_v2_ptr;
  /* log pointer to the pilot sets v2 log record type */

  LOG_HDR_PILOT_SETS_C_type    *log_ptr;
  /* log pointer to the pilot sets v3 log record type */

#ifdef FEATURE_HDR_REVB
  int                        set_cnt = 0;
#else
  int                        set_cnt;
#endif /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDR_REVB
  set_cnt += hdrsrchset_total_aset_cnt();
  set_cnt += ( hdrsrchset.cset_cnt + hdrsrchset.nset_cnt );
    /* calculate the total number of pilots for memory allocation */
#else
  set_cnt = hdrsrchset.aset_cnt + hdrsrchset.cset_cnt + hdrsrchset.nset_cnt;
  /* calculate the total number of pilots for memory allocation */
#endif /* FEATURE_HDR_REVB */

  log_v2_ptr = (LOG_HDR_PILOT_SETS_V2_C_type *)
            log_alloc_ex( (log_code_type) LOG_HDR_PILOT_SETS_V2_C,
                          HDRLOG_VAR_SIZEOF( LOG_HDR_PILOT_SETS_V2_C_type,
                                             log_hdr_pilot_sets_v2_union_type,
                                             set_cnt ) );

  log_ptr = (LOG_HDR_PILOT_SETS_C_type *)
            log_alloc_ex( (log_code_type) LOG_HDR_PILOT_SETS_C,
                          HDRLOG_VAR_SIZEOF( LOG_HDR_PILOT_SETS_C_type,
                                             log_hdr_pilot_sets_union_type,
                                             set_cnt ) );

  if ( log_v2_ptr )
  {
    hdrsrchlog_update_pilot_set_v2_log_rec( log_v2_ptr );
    /* fill in the search result parameters to the search log record */

    log_commit( (log_type *) log_v2_ptr );
    /* submit the log to DIAG */

  }/* if log_v2_ptr */

  if ( log_ptr )
  {
    hdrsrchlog_update_pilot_set_log_rec( log_ptr );
    /* fill in the search result parameters to the search log record */

    log_commit( (log_type *) log_ptr );
    /* submit the log to DIAG */

  }/* if log_ptr */

}/* hdrsrchlog_log_pilot_set_record() */

/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_DEBUG_DISPLAY_FILL_REC

DESCRIPTION  : Fills the debug display record with the latest HDR debug 
               display information..

DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchlog_debug_display_fill_rec ( void )
{
  int p;           /* Loop counter */
  int rx0_dbm;     /* Receive agc0 local variable */
  int rx1_dbm;     /* Receive agc1 local variable */
  int tx_dbm;      /* Transmit agc local variable */
  boolean diversity; /* Receive diversity local variable */
  hdrsrch_sect_struct_type *serv_sect; /* Current serving sector */
  const sys_channel_type   *channel;   /* Pointer to channel info */
#ifdef FEATURE_HDR_REVB
  uint8 i, j, chan_cnt;         /* Looping index */
  const hdrsrch_demod_idx_type      *demod_list; /* current Demod list */

  hdrsrch_demod_idx_type demod_idx = 
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
    /* Demod index for IDLE subaset */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifdef FEATURE_HDR_REVB
  channel = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN,
                                   &chan_cnt,
                                   &demod_list );
    /* Grab pointer to primary chain's channel. */
#else
  channel = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN );
    /* Grab pointer to primary chain's channel. */
#endif /* FEATURE_HDR_REVB */

  hdr_debug_disp.hdr_freq.chan_num       =  channel->chan_num;
    /* Channel Number */

  hdr_debug_disp.hdr_freq.band_class     =  channel->band;
    /* Band class */

  hdrsrchrf_get_rx_power (  &rx0_dbm,  &rx1_dbm,  &diversity );
    /* Receive agc and  diversity */
  
  hdr_debug_disp.rx_agc0                 =  rx0_dbm;
  hdr_debug_disp.rx_agc1                 =  rx1_dbm;

  /* Transmit AGC in dBm */
  if (hdrsrch_state_id == HDRSRCH_CONNECTED_STATE) 
  { 
    tx_dbm  =  rfm_hdr_get_tx_total_power( hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), 
                                           RFM_HDR_CARRIER_ID__0, 
                                           RFM_HDR_AGC_FORMAT__DBM256 );

    tx_dbm += (tx_dbm<0?-128:128); 
      /* Round of to nearest 0.5dB */

    tx_dbm /= 256;                 
      /* Convert to full dBm unit  */

    hdr_debug_disp.tx_agc                =  tx_dbm;
  
  }
  
  /* PN offsets of serving ASET pilot */
  if( hdrmdsp_check_dsp_clk () )    
  { 

#ifdef FEATURE_HDR_REVB
    serv_sect = hdrsrchset_get_asp_sect_ptr( HDRSRCH_IDLE_SUBASET_IDX,
                               HDRASP_GET_BASP_FL( demod_idx ) );
#else
    serv_sect = hdrsrchset_get_asp_sect_ptr( HDRASP_GET_BASP_FL() );
      /* Get the serving sector from best ASP index */
#endif /* FEATURE_HDR_REVB */

    if ( serv_sect != NULL )
    {
      hdr_debug_disp.serving_pn            =  serv_sect->pn_offset;
        /* Update the PN info when sector is found  */
    }
    else
    {
      hdr_debug_disp.serving_pn            =  0xffff;
        /* Set PN to invalid ( 0xffff ) if sector is not found */
    }
  } 

#ifdef FEATURE_HDR_REVB
  /* PN offsets of ASET pilots */
  p = 0;

  for ( i=0; i<hdrsrchset.subaset_cnt; i++ )
  {
    for( j=0; j<hdrsrchset.subaset[i].aset_cnt; j++ )
    {
      if ( p < HDRLOG_ASP_NUM )
      {
          hdr_debug_disp.aset_pn[p++] =  hdrsrchset.subaset[i].aset[j]->pn_offset;
      }
    }
  }

  /* Fill up the empty spot */
  for ( i = p; i < HDRLOG_ASP_NUM; i++ )
  {
      hdr_debug_disp.aset_pn[p]       =  HDRPLOG_NO_PN_FOUND;
  }
#else
  /* PN offsets of ASET pilots */
  for ( p = 0; p < HDRLOG_ASP_NUM; p++ )
  {
    if ( hdrsrchset.aset[p] )
    {
      hdr_debug_disp.aset_pn[p]          =  hdrsrchset.aset[p]->pn_offset;
    }
    else
    {
      hdr_debug_disp.aset_pn[p]          =  HDRPLOG_NO_PN_FOUND;
    }
    
  }
#endif /* FEATURE_HDR_REVB */

  /* HDR Sleep mode */
  hdr_debug_disp.sleep_mode              =  hdrrxstats_get_cc_status( );
  
  /* SCI */
  hdr_debug_disp.sci.rel0_sci            =  hdrsrchsleep_get_sci( );
  hdr_debug_disp.sci.relA_sci            =  0;

  /* HDR Searcher state */
  hdr_debug_disp.srch_state              =  hdrsrch_state_id;

  /* receiver diversity */
  hdr_debug_disp.rx_div                  =  diversity;
#ifndef FEATURE_RUMI_BRINGUP
  /* Protocol information */             
  hdrplog_srch_debug_display_info (&hdr_debug_disp);
#endif 
 
} /* hdrsrchlog_debug_display_fill_rec(); */


/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_DEBUG_DISPLAY_RECORD

DESCRIPTION  : Logs the debug display record. It gets a log record from the 
               diag log buffer, fills it with the latest debug display 
               information and commits the log record to the diag for delivery.

DEPENDENCIES : None

PARAMETERS   : log_ptr - pointer to the log packet

RETURN VALUE : Returns the log packet via the input parameter log_ptr

SIDE EFFECTS : none

=========================================================================*/


void hdrsrchlog_log_debug_display_record( void )
{
  int p;           /* Loop counter */

  LOG_HDR_DEBUG_DISPLAY_C_type *log_ptr;

 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  log_ptr = ( LOG_HDR_DEBUG_DISPLAY_C_type *)
          log_alloc_ex( (log_code_type) LOG_HDR_DEBUG_DISPLAY_C,
                            sizeof( LOG_HDR_DEBUG_DISPLAY_C_type ) );

  if ( log_ptr )
  {
    /* Copy values from hdr_debug_disp (passed to OEM layer for display
       on the UI) to log_ptr->debug_display (used for logging) */

    (log_ptr->debug_display).hdr_freq.chan_num = 
      hdr_debug_disp.hdr_freq.chan_num;

    (log_ptr->debug_display).hdr_freq.band_class = 
      hdr_debug_disp.hdr_freq.band_class;

    (log_ptr->debug_display).rx_agc0 = hdr_debug_disp.rx_agc0;
    (log_ptr->debug_display).rx_agc1 = hdr_debug_disp.rx_agc1;
    (log_ptr->debug_display).tx_agc = hdr_debug_disp.tx_agc;

    (log_ptr->debug_display).serving_pn = hdr_debug_disp.serving_pn;

    for ( p = 0; p < HDRLOG_ASP_NUM; p++ )
    {
      (log_ptr->debug_display).aset_pn[p] = hdr_debug_disp.aset_pn[p];
    }
    
    (log_ptr->debug_display).sleep_mode = hdr_debug_disp.sleep_mode;

    (log_ptr->debug_display).sci.rel0_sci = hdr_debug_disp.sci.rel0_sci;
    (log_ptr->debug_display).sci.relA_sci = hdr_debug_disp.sci.relA_sci;

    (log_ptr->debug_display).srch_state = hdr_debug_disp.srch_state;

    (log_ptr->debug_display).rx_div = hdr_debug_disp.rx_div;

    (log_ptr->debug_display).prot_state = hdr_debug_disp.prot_state;
    
    (log_ptr->debug_display).hdr_session_state = 
      hdr_debug_disp.hdr_session_state;

    (log_ptr->debug_display).uati_info.uati24 = 
      hdr_debug_disp.uati_info.uati24;
    (log_ptr->debug_display).uati_info.color_code = 
      hdr_debug_disp.uati_info.color_code;

    for ( p = 0; p < HDRLOG_NUM_STREAMS; p++ )
    {
      (log_ptr->debug_display).stream_config[p] = 
        hdr_debug_disp.stream_config[p];
    }

    log_commit( (log_type *) log_ptr );
      /* submit the log to DIAG */

  }/* if slog_ptr */

}/* hdrsrchlog_log_debug_display_record() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_DRC_ARQ_BUFFER_RECORD

DESCRIPTION  : Log DRC and ARQ data read per slot.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_drc_arq_buffer_record ( void )
{
  LOG_HDR_DRC_ARQ_BUFFER_C_type *log_ptr;
  /* log pointer to the log record type */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If the SRCH state is either Connected or TC Suspended */
  if ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE ||
       hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE )
  {
    /* We got all the frames needed to log the packet */
    log_ptr = (LOG_HDR_DRC_ARQ_BUFFER_C_type*)
              log_alloc_ex( (log_code_type) LOG_HDR_DRC_ARQ_BUFFER_C,
                            sizeof( LOG_HDR_DRC_ARQ_BUFFER_C_type ) );

    if ( log_ptr )
    {
      /* Copy two frames' worth of data to log. */
#ifdef FEATURE_MEMCPY_REMOVAL
      memscpy( ( void * ) (log_ptr->drc_arq),
               sizeof(log_ptr->drc_arq), 
                ( void * ) (hdrsrchlog_slot_logging.drc_arq),
               sizeof ( hdrsrchlog_slot_logging.drc_arq ) );
#else
      memcpy( ( void * ) (log_ptr->drc_arq),
              ( void * ) (hdrsrchlog_slot_logging.drc_arq),
               sizeof ( hdrsrchlog_slot_logging.drc_arq ) );
#endif /* FEATURE_MEMCPY_REMOVAL */

      log_commit( (log_type *) log_ptr );
        /* submit the log to DIAG */

    } /* if log_ptr */

  }

  memset ( (uint8*) &hdrsrchlog_slot_logging.drc_arq[0],
           0, 
           sizeof(hdrsrchlog_slot_logging.drc_arq) );
    /* Flush the buffer */

}/* hdrsrchlog_log_drc_arq_buffer_record */

#ifdef FEATURE_HDR_REVA_L1
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_DRC_ARQ_DSC_BUFFER_RECORD

DESCRIPTION  : Log DRC, ARQ and DSC data read per slot.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_drc_arq_dsc_buffer_record ( void )
{
  LOG_HDR_DRC_ARQ_DSC_BUFFER_C_type *log_ptr;
    /* log pointer to the log record type */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If the SRCH state is either Connected or TC Suspended */
  if ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE ||
       hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE )
  {

    log_ptr = (LOG_HDR_DRC_ARQ_DSC_BUFFER_C_type*)
              log_alloc_ex( (log_code_type) LOG_HDR_DRC_ARQ_DSC_BUFFER_C,
                            sizeof( LOG_HDR_DRC_ARQ_DSC_BUFFER_C_type ) );

    if ( log_ptr )
    {
      log_ptr->version  = HDRSRCHLOG_DRC_ARQ_DSC_BUFFER_VER;
        /* Update the version number of the log packet */

      /* Copy two frames' worth of data to log. */
#ifdef FEATURE_MEMCPY_REMOVAL
      memscpy( ( void * ) (log_ptr->drc_arq_dsc_data), 
                 sizeof(log_ptr->drc_arq_dsc_data), 
               ( void * ) (hdrsrchlog_slot_logging.drc_arq_dsc),
                 sizeof ( hdrsrchlog_slot_logging.drc_arq_dsc ) );
#else
      memcpy( ( void * ) (log_ptr->drc_arq_dsc_data),
            ( void * ) (hdrsrchlog_slot_logging.drc_arq_dsc),
             sizeof ( hdrsrchlog_slot_logging.drc_arq_dsc ) );
#endif /* FEATURE_MEMCPY_REMOVAL */

      log_commit( (log_type *) log_ptr );
        /* Submit the log to DIAG */

    } /* if log_ptr */

  }

  memset ( (uint8*) &hdrsrchlog_slot_logging.drc_arq_dsc[0],
           0, 
           sizeof(hdrsrchlog_slot_logging.drc_arq_dsc) );
    /* Flush the buffer */

} /* hdrsrchlog_log_drc_arq_dsc_buffer_record */

#endif /* FEATURE_HDR_REVA_L1 */


/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_ASP_LOG_REC

DESCRIPTION  : Updates the ASP packet data

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_update_asp_log_rec
(
  LOG_HDR_ASP_C_type *log_ptr          /* log pointer */
)
{
  uint8                    serv_asp, pred_asp;
  /* index for best and serving ASPs */

  hdrsrch_sect_struct_type *serv_ptr, *pred_ptr;
  /* pointers for best and serving sectors */

  hdrsrch_demod_idx_type   demod_idx = 
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
    /* Demod index for IDLE subaset */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( hdrmdsp_check_dsp_clk () )    
  {
#ifdef FEATURE_HDR_REVB
  log_ptr->aset_cnt = hdrsrchset_total_aset_cnt();

  pred_asp = HDRASP_GET_BASP_RL( demod_idx );
    /* read the predictor ASP's index */
#else
  log_ptr->aset_cnt                = hdrsrchset.aset_cnt;
  /* write the total number of active set sectors */

  pred_asp = HDRASP_GET_BASP_RL();
  /* read the predictor ASP's index */
#endif /* FEATURE_HDR_REVB */

  ASSERT(pred_asp < HDRSRCH_MAX_NUM_ASPS);
  pred_ptr = hdrsrchset_get_asp_sect_ptr( HDRSRCH_IDLE_SUBASET_IDX,
                                          pred_asp );
  /* get the sector pointer for the predicted ASP */

  if ( pred_ptr )
  {
    log_ptr->best_drc_cover          = pred_ptr->aset.drc_cover;
    log_ptr->best_pn                 = pred_ptr->pn_offset;
    log_ptr->best_rpc_index          = pred_ptr->aset.rpc_cell_idx;
    log_ptr->best_sinr               = 
                    HDRASP_GET_FILT_SINR( demod_idx, pred_asp );
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "BestASP Index %d not in a/c/n set",
                    pred_asp );
  }

#ifdef FEATURE_HDR_REVB
  serv_asp = HDRASP_GET_BASP_FL( demod_idx );
    /* read the serving ASP's index */
#else
  serv_asp = HDRASP_GET_BASP_FL();
    /* read the serving ASP's index */
#endif /* FEATURE_HDR_REVB */

  ASSERT(serv_asp < HDRSRCH_MAX_NUM_ASPS);
  serv_ptr = hdrsrchset_get_asp_sect_ptr( HDRSRCH_IDLE_SUBASET_IDX,
                                          serv_asp );
  /* get the sector pointer for the serving ASP */

  if ( serv_ptr )
  {
    log_ptr->serving_drc_cover_index = serv_ptr->aset.drc_cover;
    log_ptr->serving_pn              = serv_ptr->pn_offset;
    log_ptr->serving_rpc_index       = serv_ptr->aset.rpc_cell_idx;
    log_ptr->serving_sinr            = 
                     HDRASP_GET_FILT_SINR( demod_idx, serv_asp );
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "BestASP Index %d not in a/c/n set",
                    pred_asp );
  }

  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "MDSP clock is off");
  }  /* if hdrmdsp_check_dsp_clk() */

}/* hdrsrchlog_update_asp_log_rec */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_ASP_RECORD

DESCRIPTION  : Logs ASP log record

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_asp_record( void )
{

  LOG_HDR_ASP_C_type *log_ptr;
  /* log pointer to the log record type */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  log_ptr = (LOG_HDR_ASP_C_type*)
            log_alloc_ex( (log_code_type) LOG_HDR_ASP_C,
                          sizeof( LOG_HDR_ASP_C_type ) );

  if ( log_ptr )
  {
    hdrsrchlog_update_asp_log_rec( log_ptr );
    /* fill in the parameters to the search log record */

    log_commit( (log_type *) log_ptr );
    /* submit the log to DIAG */

  }/* if slog_ptr */

}/* hdrsrchlog_log_asp_record */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_HANDOFF_STATE_LOG_REC

DESCRIPTION  : Fill in the information for the handoff state packet

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/
#ifdef FEATURE_HDR_REVB
void hdrsrchlog_update_handoff_state_log_rec
(
  LOG_HDR_HANDOFF_STATE_C_type  *log_ptr
)
{
  int8   p;        /* loop variable */

  uint8 i, j;
    /* loop index and active set count */

  hdrsrch_sect_struct_type *sect;
    /* Convinience pointer */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  log_ptr->srch_state  = hdrsrch_state_id;
  log_ptr->aset_cnt    = hdrsrchset_total_aset_cnt();

  p = 0;
    /* Reset aset logging index */
  for( i=0; i<hdrsrchset.subaset_cnt; i++ )
  {
    for( j=0; j<hdrsrchset.subaset[i].aset_cnt; j++)
    {
      sect = hdrsrchset.subaset[i].aset[j];

      log_ptr->aset[p].pilot_pn    = sect->pn_offset;
      log_ptr->aset[p].drc_cover   = sect->aset.drc_cover;
      log_ptr->aset[p].mac_index   = sect->aset.mac_idx;
      log_ptr->aset[p].rpc_index   = sect->aset.rpc_cell_idx;
      log_ptr->aset[p].pilot_strength  = 
                              hdrsrchset_eng_to_ecio( sect->total_eng );
      p++;
        /* Log packet has been allocated aset_cnt slots */
    }
  }
}   /* hdrlog_update_handoff_state_log_rec() */
#else
void hdrsrchlog_update_handoff_state_log_rec
(
  LOG_HDR_HANDOFF_STATE_C_type  *log_ptr
)
{
  int8   p;        /* loop variable */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  log_ptr->srch_state  = hdrsrch_state_id;
  log_ptr->aset_cnt    = hdrsrchset.aset_cnt;

  for ( p = 0; p < hdrsrchset.aset_cnt; p++ )
  {
    log_ptr->aset[p].pilot_pn    = hdrsrchset.aset[p]->pn_offset;
    log_ptr->aset[p].drc_cover   = hdrsrchset.aset[p]->aset.drc_cover;
    log_ptr->aset[p].mac_index   = hdrsrchset.aset[p]->aset.mac_idx;
    log_ptr->aset[p].rpc_index   = hdrsrchset.aset[p]->aset.rpc_cell_idx;
    log_ptr->aset[p].pilot_strength  =
    hdrsrchset_eng_to_ecio( hdrsrchset.aset[p]->total_eng );
  }

}   /* hdrlog_update_handoff_state_log_rec() */
#endif /* FEATURE_HDR_REVB */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_HANDOFF_STATE_RECORD

DESCRIPTION  : Logs the handoff state of the AT

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_handoff_state_record ( void )
{
  LOG_HDR_HANDOFF_STATE_C_type  *log_ptr;
  /* log pointer to the Handoff State record type */

#ifdef FEATURE_HDR_REVB
  uint8 aset_cnt;
    /* Total number of ASET sectors */
#endif /* FEATURE_HDR_REVB */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifdef FEATURE_HDR_REVB
  aset_cnt = hdrsrchset_total_aset_cnt();

  log_ptr   = (LOG_HDR_HANDOFF_STATE_C_type *)
              log_alloc_ex( (log_code_type) LOG_HDR_HANDOFF_STATE_C,
                            HDRLOG_VAR_SIZEOF ( LOG_HDR_HANDOFF_STATE_C_type,
                                                log_hdr_handoff_state_type,
                                                aset_cnt ) );
#else
  log_ptr   = (LOG_HDR_HANDOFF_STATE_C_type *)
              log_alloc_ex( (log_code_type) LOG_HDR_HANDOFF_STATE_C,
                            HDRLOG_VAR_SIZEOF ( LOG_HDR_HANDOFF_STATE_C_type,
                                                log_hdr_handoff_state_type,
                                                hdrsrchset.aset_cnt ) );
#endif /* FEATURE_HDR_REVB */

  if ( log_ptr )
  {
    hdrsrchlog_update_handoff_state_log_rec (log_ptr);
    /* update the fields in the log record */

    log_commit( (log_type *) log_ptr);
    /* submit the log to DIAG */

  }/* if slog_ptr */

}   /* hdrlog_log_handoff_state_record() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHLOG_LOG_LINK_SUMMARY

DESCRIPTION
  This function performs link summary logging.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL void hdrsrchlog_log_link_summary( void )
{
  LOG_HDR_LINK_SUMMARY_C_type *log_ptr;
    /* Log record for air link summary */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( ( hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE ) || 
       ( hdrsrch_state_id == HDRSRCH_SUSPENDED_IDLE_STATE ) )
  {
    return;
  }

  log_ptr = (LOG_HDR_LINK_SUMMARY_C_type *) 
              log_alloc( LOG_HDR_LINK_SUMMARY_C, 
                         sizeof(LOG_HDR_LINK_SUMMARY_C_type) );

  if (log_ptr != NULL)
  {
    /* Copy two frames' worth of data to log. */
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy( (void*) (log_ptr->link_summary), 
            sizeof(log_ptr->link_summary), 
            (void*) hdrsrchlog_slot_logging.link_summary, 
             sizeof(hdrsrchlog_slot_logging.link_summary) ); 
#else
    memcpy( (void*) (log_ptr->link_summary), 
            (void*) hdrsrchlog_slot_logging.link_summary, 
            sizeof(hdrsrchlog_slot_logging.link_summary) ); 
#endif /* FEATURE_MEMCPY_REMOVAL */

    /* Ship the log packet. */
    log_commit( (log_type*) log_ptr ); 
  }

  memset ( (uint8*) &hdrsrchlog_slot_logging.link_summary[0],
           0, 
           sizeof(hdrsrchlog_slot_logging.link_summary) );
    /* Flush the buffer */

} /* hdrsrchlog_log_link_summary */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_RAB_BUFFER_LOG_REC

DESCRIPTION  : Fill in the information for the RA bits buffer packet

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_update_rab_buffer_log_rec
(
  LOG_HDR_RAB_BUFFER_C_type *log_ptr
)
{
  /* LJL */
  /* TBD */
  /* The fw logging interface may change for RevB.*/
  /* ETV: Check with AST and Obsolete */
#if 0

  uint32     buf_addr;   /* valid log buffer address */
  int8       valid_buf, i;  /* valid buffer index */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  valid_buf = HDRLOG_GET_VALID_RAB_LOG_BUF();
  /* read valid buffer index */

  buf_addr = HDRLOG_GET_RAB_LOG_BUF_ADDR( valid_buf );
  /* get the DSP offset address of the valid log buffer */

  for ( i = 0; i < HDR_RAB_BUFFER_LEN; i++ )
  {
    log_ptr->rab[i] = MDSP_READ((word *) buf_addr );
    buf_addr += 2;
  }

#endif/* if 0*/
}   /* hdrsrchlog_update_rab_buffer_log_rec */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_RAB_BUFFER_RECORD

DESCRIPTION  : Logs the RAB buffer record

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_rab_buffer_record ( void )
{
  LOG_HDR_RAB_BUFFER_C_type   *log_ptr;
  /* log pointer to the reverse activity bit buffer record type */


  /* ETV: Check with AST and Obsolete */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  log_ptr   = (LOG_HDR_RAB_BUFFER_C_type *)
              log_alloc_ex( (log_code_type) LOG_HDR_RAB_BUFFER_C,
                            (short) sizeof ( LOG_HDR_RAB_BUFFER_C_type ) );
  if ( log_ptr )
  {
    hdrsrchlog_update_rab_buffer_log_rec (log_ptr);
    /* update the fields in the log record */

    log_commit( (log_type *) log_ptr);
    /* submit the log to DIAG */
  }

}    /* hdrsrchlog_log_rab_buffer_record */


/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_HRES_TA_LOG_REC

DESCRIPTION  : Fill in the information for the high resolution TA search
               result packet

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_update_hres_ta_log_rec
(
  LOG_HDR_HRES_TA_C_type  *log_ptr
)
{
  uint32  p; /* loop variable */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* TBD: Need to replace HRES Log packet with real data read from the
     DSP. This is a temporary log for testing the parser */

  log_ptr->ta_start_x2     = 0xabcd;
  log_ptr->antenna         = 0x0;
  log_ptr->ta_win_size     = 127;
  log_ptr->num_samples     = 10;

  for ( p = 0; p < HDR_LOG_MAX_HRES_TA_LEN; p++ )
  {
    log_ptr->pk_eng[p] = 0xa;   /* TBD : Read from the DSP */
  }

}   /* hdrsrchlog_update_hres_ta_log_rec */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_HRES_TA_RECORD

DESCRIPTION  : Log high resolution TA record

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_hres_ta_record ( void )
{
  LOG_HDR_HRES_TA_C_type  *log_ptr;
  /* log pointer to the high resolution TA search results record type */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  log_ptr   = (LOG_HDR_HRES_TA_C_type *)
              log_alloc_ex( (log_code_type) LOG_HDR_HRES_TA_C,
                            (short) sizeof ( LOG_HDR_HRES_TA_C_type ) );
  if ( log_ptr )
  {
    hdrsrchlog_update_hres_ta_log_rec (log_ptr);
    /* update the fields in the log record */

    log_commit( (log_type *) log_ptr);
    /* submit the log to DIAG */
  }

}   /* hdrsrchlog_log_hres_ta_record */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_MC_DRC_ARQ_DSC_BUFFER_RECORD

DESCRIPTION  : Logs one frame's worth of Multi-carrier DRC, ARQ and DSC 
               data read from mDSP buffers for each slot.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_mc_drc_arq_dsc_buffer_record 
( 
  uint8 first_index,
    /* Index corresponding to first slot of logging in the buffer */
  uint8 cbuff_valid_mask,
    /* Bit mask denoting if log buffer is valid for each carrier */
  uint16 str
    /* Slot Time Reference */
)
{

  uint8 demod_idx = 0;
    /* Demod Carrier Index */

  uint8 subaset_idx = 0;
    /* Sub active set index */

  uint8 carrier_cnt = 0;
    /* Number of carriers logged this frame */

  LOG_HDR_MC_DRC_ARQ_DSC_BUFFER_C_type *log_ptr;
    /* Convenience pointer to log buffer */

  uint8 aspi = 0;
    /* ASP index */

  uint8 aset_ind = 0;
    /* Loop index */

  hdrsrch_sect_struct_type *sect_ptr = NULL;
    /* Convenience pointer to sector information */

  hdrlog_per_slot_channel_info_type *c_ptr = NULL;
    /* Convenience pointer to per slot channel information */

  hdrsrch_sect_struct_type          *serv_sect_ptr;
    /* Serving sector */

  uint8                             best_asp;
    /* index for serving sector */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  carrier_cnt = HDRSRCHLOG_NUM_VALID_CARRIER_BUFFS( cbuff_valid_mask );
    /* Find the number of carriers active to be logged this frame */

  log_ptr = (LOG_HDR_MC_DRC_ARQ_DSC_BUFFER_C_type*)
            log_alloc_ex( (log_code_type) LOG_HDR_MC_DRC_ARQ_DSC_BUFFER_C,
                          ( sizeof( LOG_HDR_MC_DRC_ARQ_DSC_BUFFER_C_type ) +
                            ( sizeof ( hdrlog_per_carrier_drc_arq_dsc_type ) *
                              ( hdrsrchset.subaset_cnt ) ) ) );
    /* Allocate log buffer! (Fails if log code is not enabled) */

  /* If the log code is enabled */
  if ( log_ptr )
  {
    log_ptr->version = HDRLOG_MC_DRC_ARQ_DSC_BUFFER_VER;
      /* Update the version number of the log packet */

    log_ptr->num_carriers = carrier_cnt;
      /* Store the number of carriers logged */

    log_ptr->slot_count = str;
      /* Store the slot count corresponding to the first index of this 
         buffer */

    carrier_cnt = 0;
      /* Reset the carrier count; to be used as array index */

    /* For each sub active sets */
    for ( subaset_idx = 0;
          subaset_idx < hdrsrchset.subaset_cnt; ++subaset_idx )
    {
      demod_idx = hdrsrchset.subaset[subaset_idx].demod_idx;
        /* Get the demod carrier index from sub active set index */

      c_ptr = 
        &hdrsrchlog_slot_logging.mc_drc_arq_dsc_buffer[demod_idx][first_index];
        /* Initialize the convenience pointer */

      /* If we have collected valid information in the buffer for this
         carrier */
      if ( HDRSRCHLOG_CARRIER_LOG_BUFF_IS_VALID ( demod_idx, cbuff_valid_mask ) )
      {
        /* Copy Carrier Information */
        log_ptr->per_carrier[carrier_cnt].carrier_id.channel_id = demod_idx;
        log_ptr->per_carrier[carrier_cnt].carrier_id.band_class = 
          hdrsrchset.subaset[subaset_idx].channel.band;
        log_ptr->per_carrier[carrier_cnt].carrier_id.channel_no =
          hdrsrchset.subaset[subaset_idx].channel.chan_num;

        best_asp = HDRASP_GET_BASP_FL(demod_idx);
          /* get ASP index for the serving sector */

        serv_sect_ptr = hdrsrchset_get_asp_sect_ptr( subaset_idx, best_asp );
          /* Get a pointer to serving sector information */

        if ( serv_sect_ptr != NULL )
        {
          log_ptr->per_carrier[carrier_cnt].pn = serv_sect_ptr->pn_offset;
            /* Store the PN offset of Best FL ASP */
        }
        else
        {
          log_ptr->per_carrier[carrier_cnt].pn = HDR_NO_PN_OFFSET;
            /* If we don't have a valid sector, set this to invalid value */
        }

        /*--------------------------------------------------------------
                        Per-Slot DRC/DSC/ACK/ASP Logging
         -------------------------------------------------------------*/

        /* Copy one frame worth of per-slot data for this carrier to 
           log buffer. */
#ifdef FEATURE_MEMCPY_REMOVAL
         memscpy(
          (void*) (&log_ptr->per_carrier[carrier_cnt].slot_info[0]), 
           sizeof(log_ptr->per_carrier[carrier_cnt].slot_info), 
          (void*) (c_ptr),
          (sizeof (hdrlog_per_slot_channel_info_type) *
           HDRLOG_NUM_SLOTS_IN_FRAME));
#else
        memcpy(
          (void*) (&log_ptr->per_carrier[carrier_cnt].slot_info[0]),
          (void*) (c_ptr),
          (sizeof (hdrlog_per_slot_channel_info_type) *
           HDRLOG_NUM_SLOTS_IN_FRAME));
#endif /* FEATURE_MEMCPY_REMOVAL */

        /*--------------------------------------------------------------
                    Per-ASP DRC Lock / DRC Lock Update Logging
         -------------------------------------------------------------*/

        log_ptr->per_carrier[carrier_cnt].drc_lock = 0;
          /* Initialize DRC Lock bit field to zero */

        for ( aset_ind = 0; 
              aset_ind < hdrsrchset.subaset[subaset_idx].aset_cnt; aset_ind++ )
        {

          sect_ptr = hdrsrchset.subaset[subaset_idx].aset[aset_ind];
            /* Initialize the convenience pointer */

          if ( sect_ptr != NULL )
          {
            if ( sect_ptr->pn_offset != HDR_NO_PN_OFFSET )
            {
            aspi = sect_ptr->aset.asp_idx;
              /* Read the asp index for the active set pilot */

              /* Every pair of bits from LSB denotes the DRCLock and 
                 DRCLockValid bits for each ASP from 0 through 5 respectively. 
                 0x02 (b0010) in the formula below sets the DRCLockValid bit; 
                 The DRCLock bit is returned from the macro and these pairs of 
                 bits are shifted left based on the ASP index */
              log_ptr->per_carrier[carrier_cnt].drc_lock |=
                ((0x02 | HDRASP_GET_DRC_LOCK(demod_idx, aspi)) << (aspi * 2));

            } /* if sector has valid PN */

          } /* If the sector pointer is non-null */

        } /* for each aset */

        log_ptr->per_carrier[carrier_cnt].drc_lock_update =
          HDRDRC_GET_DRC_LOCK_UPDATE_MC(demod_idx);
          /* get the slot count when the DRC lock bit was updated */

        carrier_cnt++;
          /* One more carrier logged */

      } /* If carrier buffer is valid */

      memset( (void*) c_ptr,
              0,
              ( sizeof(hdrlog_per_slot_channel_info_type) * 
                HDRLOG_NUM_SLOTS_IN_FRAME ) );
        /* Irrespective of carrier buffer validity, clear the data accumulated
           over the logged frame for this carrier */

    } /* for each sub active set */

    log_commit( (log_type *) log_ptr );
      /* Submit the log to DIAG */

  } /* if log_ptr */

} /* hdrsrchlog_log_mc_drc_arq_dsc_buffer_record */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_MC_LINK_SUMMARY

DESCRIPTION  : Logs one frame worth of Multi-carrier air link summary log
               information read from mDSP buffers for each slot.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_mc_link_summary
( 
  uint8 first_index,
    /* Index corresponding to first slot of logging in the buffer */
  uint8 cbuff_valid_mask,
    /* Bit mask denoting if log buffer is valid for each carrier */
  uint16 str
    /* Slot Time Reference */
)
{
  uint8 demod_idx = 0;
    /* Carrier Index */

  uint8 subaset_idx = 0;
    /* Sub active set index */

  uint8 carrier_cnt = 0;
    /* Number of carriers logged this frame */

  LOG_HDR_MC_LINK_SUMMARY_C_type *log_ptr;
    /* Convenience pointer to log buffer */

  uint8 aspi = 0;
    /* ASP index */

  hdrsrch_sect_struct_type *sect_ptr;
    /* Pointer to the sector information */

  hdrlog_per_slot_link_summary_type *c_ptr;
    /* Convenience pointer to per-slot link summary data */

  boolean locked_mode = FALSE;
    /* Are the serving sectors locked? */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( ( hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE ) || 
       ( hdrsrch_state_id == HDRSRCH_SUSPENDED_IDLE_STATE ) )
  {
    return;
  }

  carrier_cnt = HDRSRCHLOG_NUM_VALID_CARRIER_BUFFS(cbuff_valid_mask);
    /* Find the number of carriers active to be logged this frame */

  log_ptr = (LOG_HDR_MC_LINK_SUMMARY_C_type*)
            log_alloc_ex( (log_code_type) LOG_HDR_MC_LINK_SUMMARY_C,
                          ( sizeof( LOG_HDR_MC_LINK_SUMMARY_C_type ) +
                            ( sizeof ( hdrlog_per_carrier_link_summary_type ) *
                              (hdrsrchset.subaset_cnt ) ) ) );
    /* Allocate log buffer! (Fails if log code is not enabled) */

  /* If log code is enabled */
  if ( log_ptr )
  {
    log_ptr->version  = HDRLOG_MC_LINK_SUMMARY_VER;
      /* Update the version number of the log packet */

    log_ptr->num_carriers = carrier_cnt;
      /* Store the number of carriers logged */

    log_ptr->str = str;
      /* Store the slot count corresponding to the first index of this 
         buffer */

    if ( hdrfmac_serving_sector_locked_across_sub_asets( &locked_mode ) 
                                                                  != E_SUCCESS )
    {
      locked_mode = TRUE;
    }

    if ( locked_mode )
    {
      log_ptr->ref_sub_aset_locked_mode = hdrsrchset_get_ref_subaset_idx();
        /* Store SubASETIndex of Reference sub-active set */
    }
    else
    {
      log_ptr->ref_sub_aset_locked_mode = 0xFF;
    }

    carrier_cnt = 0;
      /* Reset the carrier count; to be used as array index */

    /* For each sub-active set */
    for ( subaset_idx = 0; 
          subaset_idx < hdrsrchset.subaset_cnt; ++subaset_idx )
    {
      demod_idx = hdrsrchset.subaset[subaset_idx].demod_idx;
        /* Get the Demod Index from the subaset index */

      c_ptr = &hdrsrchlog_slot_logging.mc_link_summary[demod_idx][first_index];
        /* Initialize the convenience pointer */

      /* If we have collected valid information in the buffer for this
         carrier */
      if ( HDRSRCHLOG_CARRIER_LOG_BUFF_IS_VALID ( demod_idx, cbuff_valid_mask ) )
      {
        /* Fill the carrier information */
        log_ptr->per_carrier[carrier_cnt].carrier_id.channel_id = demod_idx;
        log_ptr->per_carrier[carrier_cnt].carrier_id.band_class =
          hdrsrchset.subaset[subaset_idx].channel.band;
        log_ptr->per_carrier[carrier_cnt].carrier_id.channel_no =
          hdrsrchset.subaset[subaset_idx].channel.chan_num;

        /*--------------------------------------------------------------
                        Instantaneous PER statistics
         -------------------------------------------------------------*/

        hdrrxstats_gather_mc_link_summary_data ( log_ptr, carrier_cnt, demod_idx );
          /* Gather PER statistics and PA state from RX module */

        /*--------------------------------------------------------------
                        Per-Slot Link Summary Data Logging
         -------------------------------------------------------------*/

#ifdef FEATURE_MEMCPY_REMOVAL
        memscpy(
            (void*) (&log_ptr->per_carrier[carrier_cnt].per_slot[0]), 
             sizeof(log_ptr->per_carrier[carrier_cnt].per_slot), 
            (void*) (c_ptr),
            (sizeof (hdrlog_per_slot_link_summary_type) *
             HDRLOG_NUM_SLOTS_IN_FRAME));
#else
        memcpy(
          (void*) (&log_ptr->per_carrier[carrier_cnt].per_slot[0]),
          (void*) (c_ptr),
          (sizeof (hdrlog_per_slot_link_summary_type) *
           HDRLOG_NUM_SLOTS_IN_FRAME));
#endif /* FEATURE_MEMCPY_REMOVAL */
          /* Copy one frame worth of data for this carrier to log buffer. */

        /*--------------------------------------------------------------
                      Best ASP/Per-ASP Link Summary Data Logging
         -------------------------------------------------------------*/

#ifdef FEATURE_HDR_BCMCS
        if ( ( hdrsrch_state_id == HDRSRCH_BC_IDLE_STATE ) ||
             ( hdrsrch_state_id == HDRSRCH_BC_ACCESS_STATE ) )
        {
          log_ptr->per_carrier[carrier_cnt].basp_info.basp_pred = 
            HDRASP_GET_BASP_FL( demod_idx );
            /* The forward link best ASP is dictated by ARM during BC idle 
               state and BC access state */
        }
        else
#endif /* FEATURE_HDR_BCMCS */
        {

          log_ptr->per_carrier[carrier_cnt].basp_info.basp_pred = 
            HDRDRC_GET_VAR_BEST_ASP_PRED( demod_idx );
            /* Best asp index predicted */

          log_ptr->per_carrier[carrier_cnt].basp_info.last_basp_changed_slot = 0;
            /* ETV TODO - Find info source for this field */
        }

        /* Read ASP info (sinr, pilotPN, rpc, and rpc cell index) */
        for ( aspi = 0; aspi < HDRLOG_ASP_NUM; aspi++ )
        {
          log_ptr->per_carrier[carrier_cnt].per_asp[aspi].asp_filt_sinr =
                          HDRASP_GET_FILT_SINR( demod_idx, aspi );
            /* Read ASP filtered SINR */

          sect_ptr = hdrsrchset_get_asp_sect_ptr( subaset_idx, aspi );
            /* Get the sector pointer for the ASP */

          if ( sect_ptr && sect_ptr->set == HDR_ASET )
          {
            log_ptr->per_carrier[carrier_cnt].per_asp[aspi].pilot_pn_asp =
              sect_ptr->pn_offset;
              /* Read Sector's PN offset */

            log_ptr->per_carrier[carrier_cnt].per_asp[aspi].rpc_cell_idx =
              sect_ptr->aset.rpc_cell_idx;
              /* Read Sector's RPC Cell index */

            log_ptr->per_carrier[carrier_cnt].per_asp[aspi].rpc = 
              HDRASP_GET_MEAN_RPC( demod_idx, sect_ptr->aset.rpc_cell_idx );
              /* Read the RPC mean value for each cell. Note that multiple ASPs
                 could have same RPC cell index and hence may store same value 
                 in the RPC value array */
          }
          else
          {
            log_ptr->per_carrier[carrier_cnt].per_asp[aspi].pilot_pn_asp = 
              HDR_NO_PN_OFFSET;
            log_ptr->per_carrier[carrier_cnt].per_asp[aspi].rpc_cell_idx = 0;
            log_ptr->per_carrier[carrier_cnt].per_asp[aspi].rpc = 0;
          }

        } /* for each ASP */

        /* RCL Predictor Adjust in Q26 */
        log_ptr->per_carrier[carrier_cnt].rcl_pred_adj = 
          HDRDRC_GET_RCL_PRED_ADJ( demod_idx );

        carrier_cnt++;
          /* One more carrier logged */

      } /* If carrier log buffer is valid */

      memset( (void*) c_ptr,
              0,
              ( sizeof(hdrlog_per_slot_link_summary_type) * 
                HDRLOG_NUM_SLOTS_IN_FRAME ) );
        /* Irrespective of carrier buffer validity, clear the data accumulated
           over the logged frame for this carrier */

    } /* for each sub active set */

    log_commit( (log_type *) log_ptr );
      /* Submit the log to DIAG */

  } /* if log_ptr */

} /* hdrsrchlog_log_mc_link_summary */


#ifdef FEATURE_HDR_REVB
/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_MC_PILOT_SET_LOG_REC

DESCRIPTION  : Fill in the information for the pilot set log record.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_update_mc_pilot_set_log_rec
(
  LOG_HDR_MC_PILOT_SETS_C_type *log_ptr
  /* log pointer to the pilot sets log record type */
)
{
  int8      p;            
    /* Pilot loop variable */

  hdrsrch_sect_struct_type *sptr;   
    /* Convinience pointer */

  uint8     i;            
    /* General loop index */

  log_hdr_mc_pilot_sets_aset_type    *aset_info_ptr;
  log_hdr_mc_pilot_sets_cset_type    *cset_info_ptr;
  log_hdr_mc_pilot_sets_nset_type    *nset_info_ptr;
    /* Temporary log buffer pointors */

  int                        tot_gcset_cnt = 0;
    /* Total number of GCSETs across all subasets */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for( i=0; i<hdrsrchset.subaset_cnt; i++ )
  {
    tot_gcset_cnt += hdrsrchset.subaset[i].gcset_cnt;
  }

  /* --- Log header information --- */

  log_ptr->version         = HDRLOG_MC_PILOT_SETS_VER;
    /* The version number of the log packet */

  log_ptr->pn_inc          = hdrsrch.srch_params.pilot_inc;
    /* The PN increments  */

  log_ptr->srch_state      = hdrsrch_state_id;
    /* Current hdr search state */

  log_ptr->aset_cnt        = hdrsrchset_total_aset_cnt() + tot_gcset_cnt;
    /* Active set pilots + GCSET's + Ghost GCSET's count */

  log_ptr->aset_win = ( log_ptr->aset_cnt > 0 ) ?
                  hdrsrchutil_get_winsize( hdrsrchset.subaset[0].aset[0] ) : 0;
      /* Active set windows size use subaset[0].aset[0] if ASET is not empty */

  log_ptr->cset_cnt        = hdrsrchset.cset_cnt;
    /* CSET pilot count */

  log_ptr->cset_win = ( log_ptr->cset_cnt > 0 ) ?
                             hdrsrchutil_get_winsize( hdrsrchset.cset[0] ) : 0;
      /* Active set windows size use subaset[0].aset[0] if ASET is not empty */

  log_ptr->nset_cnt        = hdrsrchset.nset_cnt;
    /* NSET pilot count */

  log_ptr->slot_cnt        = HDRHSTR_GET_COUNT() % 512 / 2;
    /* Current HSTR converted to slots */

  /* --- Log RSET information --- */

  if ( hdrsrchset.rset != NULL )
  {
    log_ptr->rset_win = hdrsrchutil_get_winsize( hdrsrchset.rset );
    log_ptr->rset_channel.chan_no = hdrsrchset.rset->chan.chan_num;
    log_ptr->rset_channel.band_class = hdrsrchset.rset->chan.band;
  }
  else
  {
    log_ptr->rset_channel.chan_no     = HDRSRCHLOG_PACKED_INVALID_CHAN;
    log_ptr->rset_channel.band_class  = HDRSRCHLOG_PACKED_INVALID_BAND;
    log_ptr->rset_win = 0;
  }

  /* --- Log ASET pilots --- */

  aset_info_ptr = &log_ptr->aset_info[0];
    /* Init the point to the first aset info record */

  for( i=0; i<hdrsrchset.subaset_cnt;i++)
  {
    for ( p = 0; p < ( hdrsrchset.subaset[i].aset_cnt + hdrsrchset.subaset[i].gcset_cnt ); p++ )
    {
      sptr = hdrsrchset.subaset[i].aset[p];
        /* Convenience pointer */

      /*lint -save -e661 -e662 Ignore Possible access of out-of-bounds pointer*/

      /* ASET header field */

      aset_info_ptr->pilot_pn              = sptr->pn_offset;
      aset_info_ptr->total_eng             = sptr->total_eng;
      aset_info_ptr->ofs_cond_eng          = sptr->pil_filter.total_eng;
      aset_info_ptr->pilot_gid             = sptr->pilot_gid;

      /* ASET sector channel information field */

      aset_info_ptr->chan_info.chan_no     = sptr->chan.chan_num;
      aset_info_ptr->chan_info.band_class  = sptr->chan.band;

      /* Index info field */

      aset_info_ptr->index_info.link_id    = sptr->link_id;
      aset_info_ptr->index_info.demod_index  = 
                           hdrsrchset.subaset[sptr->aset.subaset_idx].demod_idx;
      aset_info_ptr->index_info.reportable =
                     !hdrsrchset.subaset[sptr->aset.subaset_idx].non_reportable;
      aset_info_ptr->index_info.subaset_index =
                             hdrsrchset.subaset[sptr->aset.subaset_idx].tca_idx;
      aset_info_ptr->index_info.sched_tag  = sptr->aset.scheduler_tag; 
      aset_info_ptr->index_info.asp_index  = sptr->aset.asp_idx;
      aset_info_ptr->index_info.rpc_index  = sptr->aset.rpc_cell_idx;
      aset_info_ptr->index_info.drc_cover  = sptr->aset.drc_cover;
      aset_info_ptr->index_info.droptimer_expired =
                                       ( sptr->drop_timer.expired ? 1 : 0 );
      aset_info_ptr->index_info.droptimer_active =
                                       ( sptr->drop_timer.active ? 1 : 0 );

      /* MAC info field */

      aset_info_ptr->mac_info.fl_mac_index  = sptr->aset.mac_idx;
      aset_info_ptr->mac_info.dsc_value     = sptr->aset.dsc;
      aset_info_ptr->mac_info.aux_drc_cover = 0;
        /* TBD: seems we dont have this information from LMAC/SRCH interface */

      aset_info_ptr->mac_info.rl_mac_index  = sptr->aset.rl_mac_idx;
      aset_info_ptr->mac_info.rab_mac_index = sptr->aset.rab_mac_idx;

      /* Search window center field 2 bytes */

      aset_info_ptr->win_center_x2         = sptr->win_cenx2;

      /* lint -restore */

      aset_info_ptr ++;
    }/* for p */
  } /* for i */

  /* --- Log CSET pilots --- */

  cset_info_ptr = ( log_hdr_mc_pilot_sets_cset_type * )aset_info_ptr;

  for ( p = 0; p < hdrsrchset.cset_cnt; p++ )
  {
    sptr = hdrsrchset.cset[p];
      /* convenience pointer */

    /*lint -save -e661 -e662 Ignore Possible access of out-of-bounds pointer*/
    /* Header Field */

    cset_info_ptr->pilot_pn         = sptr->pn_offset;
    cset_info_ptr->total_eng        = sptr->total_eng;
    cset_info_ptr->ofs_cond_eng     = sptr->pil_filter.total_eng;
    cset_info_ptr->pilot_gid        = sptr->pilot_gid;

    /* CSET channel information */

    cset_info_ptr->chan_info.chan_no     = sptr->chan.chan_num;
    cset_info_ptr->chan_info.band_class  = sptr->chan.band;

    /* CSET info field */

    cset_info_ptr->cset_info.droptimer_expired =
                                    ( sptr->drop_timer.expired ? 1 : 0 );
    cset_info_ptr->cset_info.droptimer_active  =
                                    ( sptr->drop_timer.active ? 1 : 0 );

    /* CSET searcher window center */

    cset_info_ptr->win_center_x2         = sptr->win_cenx2;

    /* lint -restore */

    cset_info_ptr ++;
  }/* for p */

  /* --- Log the NSET pilots --- */

  nset_info_ptr = ( log_hdr_mc_pilot_sets_nset_type * )cset_info_ptr;

  for ( p = 0; p < hdrsrchset.nset_cnt; p++ )
  {
    sptr = hdrsrchset.nset[p];
    /* Convenience pointer */

    /*lint -save -e661 -e662 Ignore Possible access of out-of-bounds pointer*/
    /* Header field */

    nset_info_ptr->pilot_pn              = sptr->pn_offset;
    nset_info_ptr->total_eng             = sptr->total_eng;
    nset_info_ptr->ofs_cond_eng          = sptr->pil_filter.total_eng;
    nset_info_ptr->pilot_gid             = sptr->pilot_gid;

    /* Channel info field */

    nset_info_ptr->chan_info.chan_no     = sptr->chan.chan_num;
    nset_info_ptr->chan_info.band_class  = sptr->chan.band;

    /* Search window info field */
    if ( sptr->nset.win_offset == HDRSRCH_WINOFFSET_DEFAULT )
    {
      nset_info_ptr->win_info.offset     = HDRSRCH_WINOFFSET_NONE;
    }
    else
    {
      nset_info_ptr->win_info.offset     = sptr->nset.win_offset;
    }

    nset_info_ptr->win_info.win_size     = hdrsrchutil_get_winsize( sptr );
    nset_info_ptr->age                   = sptr->nset.age;

    /* lint -restore */
  
    nset_info_ptr ++;  
  }/* for p */
}/* hdrsrchlog_update_mc_pilot_set_log_rec() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_MC_PILOT_SET_RECORD

DESCRIPTION  : Logs the MC pilot set record. It gets a log record from the diag
               log buffer, fills it with the latest MC pilot set information
               and commits the log record to the diag for delivery.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_mc_pilot_set_record( void )
{

  LOG_HDR_MC_PILOT_SETS_C_type *log_ptr;
    /* log pointer to the search log record type */

  int                        aset_cnt = 0;
    /* Total number of ASETs across all subasets including GCSETs which
       are conveniently stored in the ASET data structure. */
  int                        tot_gcset_cnt = 0;
    /* Total number of GCSETs across all subasets */
  int                        i;
    /* Loop counter */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for( i=0; i<hdrsrchset.subaset_cnt; i++ )
  {
    tot_gcset_cnt += hdrsrchset.subaset[i].gcset_cnt;
  }

  aset_cnt = hdrsrchset_total_aset_cnt() + tot_gcset_cnt;
    /* Calculate the total number of the ASET pilots for memory allocation */

  log_ptr = ( LOG_HDR_MC_PILOT_SETS_C_type * )
            log_alloc_ex( ( log_code_type ) LOG_HDR_MC_PILOT_SETS_C,
                                 ( sizeof( LOG_HDR_MC_PILOT_SETS_C_type )    -
                                   sizeof( log_hdr_mc_pilot_sets_aset_type ) - 
                                   sizeof( log_hdr_mc_pilot_sets_cset_type ) - 
                                   sizeof( log_hdr_mc_pilot_sets_nset_type ) + 
                    ( aset_cnt * sizeof( log_hdr_mc_pilot_sets_aset_type ) ) + 
         ( hdrsrchset.cset_cnt * sizeof( log_hdr_mc_pilot_sets_cset_type ) ) + 
        ( hdrsrchset.nset_cnt * sizeof( log_hdr_mc_pilot_sets_nset_type ) ) ) );

  if ( log_ptr )
  {
    hdrsrchlog_update_mc_pilot_set_log_rec( log_ptr );
      /* Fill in the search result parameters to the search log record */

    log_commit( ( log_type * ) log_ptr );
      /* Submit the log to DIAG */
  }/* if slog_ptr */
}/* hdrsrchlog_log_mc_pilot_set_record() */
#endif /* FEATURE_HDR_REVB */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_GET_PER_FRAME_LINK_SUMMARY_DATA

DESCRIPTION  : This function is called every frame to update link summary 
               data into the log buffer.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_get_per_frame_link_summary_data
(
  hdrlog_link_summary_type *link_summary_ptr
    /* Pointer to the link summary buffer where the data is filled in */
)
{

  uint8 aspi;
    /* ASP Index */

  hdrsrch_sect_struct_type *sect_ptr = NULL;
    /* Pointer to struct containing info about the sector */

  hdrsrch_demod_idx_type demod_idx = 
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
    /* Demod index for IDLE subaset */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDR_BCMCS
  if ( ( hdrsrch_state_id == HDRSRCH_BC_IDLE_STATE   )   || 
       ( hdrsrch_state_id == HDRSRCH_BC_ACCESS_STATE )    )
  {
    link_summary_ptr->packed_field.best_asp_pred = 
      HDRASP_GET_BASP_FL( demod_idx );
      /* The forward link best ASP is dictated by ARM during BC
         idle state and BC access state                         */
  }
  else
  {
    link_summary_ptr->packed_field.best_asp_pred = 
      HDRDRC_GET_VAR_BEST_ASP_PRED( demod_idx );
      /* Best asp index predicted */
  }
#else
  link_summary_ptr->packed_field.best_asp_pred = 
    HDRDRC_GET_VAR_BEST_ASP_PRED(demod_idx);
    /* Best asp index predicted */
#endif /* FEATURE_HDR_BCMCS */

  link_summary_ptr->packed_field.best_asp_pred++;
    /* ASP in Artemis goes from 0 thru 5 instead of 1 thru 6. Adding
       1 to ASP index to maintain backward compatibility to legacy
       log packets. */

  /* Read ASP info (sinr, pilotPN, rpc, and rpc cell index) */
  for ( aspi = 0; aspi < HDRLOG_ASP_NUM; aspi++ )
  {
    link_summary_ptr->aspfilt_sinr[aspi] = 
      HDRASP_GET_FILT_SINR( demod_idx, aspi );

    sect_ptr  = 
      hdrsrchset_get_asp_sect_ptr( HDRSRCH_IDLE_SUBASET_IDX, aspi );
    /* Get the sector pointer for the ASP */

    if ( sect_ptr && sect_ptr->set == HDR_ASET )
    {
      link_summary_ptr->pn_asp[aspi ]= sect_ptr->pn_offset;
      link_summary_ptr->rpc_index[aspi] = sect_ptr->aset.rpc_cell_idx;

      /* Read the RPC mean value for each cell. Note that multiple ASPs
         could have same RPC cell index and hence may store same value in
         the RPC value array */
      link_summary_ptr->rpc[aspi] = 
        HDRASP_GET_MEAN_RPC( demod_idx, 
                             sect_ptr->aset.rpc_cell_idx );
    }
    else
    {
      link_summary_ptr->pn_asp[aspi] = 0xffff;
      link_summary_ptr->rpc_index[aspi] = 0;
    }

  } /* for each ASP */

  hdrrxstats_gather_link_summary_data( link_summary_ptr );
    /* Gather Rx statistics */

} /* hdrsrchlog_get_per_frame_link_summary_data */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_DRC_LOCK

DESCRIPTION  : This function is called every frame to update DRC-ARQ-Buffer

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

LOCAL void hdrsrchlog_update_drc_lock
(
  uint16 *drc_lock,
  uint8  *drc_lock_update
)
{
  uint8 aset_ind = 0;
    /* ASET Index */

  hdrsrch_sect_struct_type *sect_ptr;
    /* Convenience pointer to ASP sector struct */

  uint8 aspi = 0;
    /* ASP Index */

  hdrsrch_demod_idx_type demod_idx = 
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
    /* Demod index for IDLE subaset */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*--------------------------------------------------------------
              Per-ASP DRC Lock / DRC Lock Update Logging
   -------------------------------------------------------------*/

  *drc_lock = 0;
    /* Initialize DRC Lock bit field to zero */

  for ( aset_ind = 0; 
        aset_ind < hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset_cnt; 
        aset_ind++ )
  {
    sect_ptr = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[aset_ind];
      /* Initialize the convenience pointer */

    if ( sect_ptr != NULL )
    {
      if ( sect_ptr->pn_offset != HDR_NO_PN_OFFSET )
      {
      aspi = sect_ptr->aset.asp_idx;
        /* Read the asp index for the active set pilot */

        /* Every pair of bits from LSB denotes the DRCLock and 
           DRCLockValid bits for each ASP from 0 through 5 respectively. 
           0x02 (b0010) in the formula below sets the DRCLockValid bit; 
           The DRCLock bit is returned from the macro and these pairs of 
           bits are shifted left based on the ASP index */
        *drc_lock |=
          ( (0x02 | HDRASP_GET_DRC_LOCK( demod_idx, aspi ) ) << 
            (aspi * 2) );

      } /* if sector has valid PN */

    } /* If the sector pointer is non-null */

  } /* for each aset */

  *drc_lock_update = HDRDRC_GET_DRC_LOCK_UPDATE_MC( demod_idx );
    /* get the slot count when the DRC lock bit was updated */

} /* hdrsrchlog_update_drc_lock */

#ifdef FEATURE_HDR_SELECTIVE_TXD
/* ToDo: The following macro are only needed when HDR directly
   access FW logging buffer. When RF provides the proper API,
   they should be removed/replaced */

#define HDRSRCHLOG_HS_PER_SUBFRAME     8

/*===========================================================================

FUNCTION     : HDRSRCHLOG_STXD_GET_MDSP_INFO

DESCRIPTION  : This function retrieves the saved MDSP info

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

===========================================================================*/
void hdrsrchlog_stxd_get_mdsp_info
( 
  hdrsrchlog_txd_mdsp_info_type* mdsp_info 
)
{
  rex_enter_crit_sect( &hdrsrchlog_stxd_logging.crit_sect );

  /* If MDSP info is not being written */
  hdrsrchlog_stxd_logging.read_pending = FALSE;

  mdsp_info->rx0 = hdrsrchlog_stxd_logging.rx0;
  mdsp_info->rx1 = hdrsrchlog_stxd_logging.rx1;
  mdsp_info->tx_total = hdrsrchlog_stxd_logging.tx_total;
  mdsp_info->ratchet = hdrsrchlog_stxd_logging.ratchet;
  mdsp_info->slots = hdrsrchlog_stxd_logging.slots;

  rex_leave_crit_sect( &hdrsrchlog_stxd_logging.crit_sect );
}

/*===========================================================================

FUNCTION     : HDRSRCHLOG_STXD_SEND_MDSP_INFO

DESCRIPTION  : This function send the RxAGCCommon value averaged over 
               the past 4 slots and the rachet value

DEPENDENCIES : None

RETURN VALUE : None.

SIDE EFFECTS : None

===========================================================================*/
void hdrsrchlog_stxd_send_mdsp_info
(
   uint8 rx_buf_idx,
   uint8 tx_buf_idx
   )
{
  uint8 i;

  static boolean crit_initialized = FALSE;
  int16 tx_total;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( !crit_initialized )
  {
    rex_init_crit_sect( &hdrsrchlog_stxd_logging.crit_sect );
    crit_initialized = TRUE;
  }

  if ( !hdrsrchlog_stxd_logging.mdsp_info_needed )
  {
    return;
  }

  rex_enter_crit_sect( &hdrsrchlog_stxd_logging.crit_sect );

  if ( !hdrsrchlog_stxd_logging.read_pending )
  {
    hdrsrchlog_stxd_logging.rx0 = 0;
    hdrsrchlog_stxd_logging.rx1 = 0;
    hdrsrchlog_stxd_logging.tx_total = 0;
    hdrsrchlog_stxd_logging.ratchet = 0;
    hdrsrchlog_stxd_logging.slots = 0;
  }

  for ( i = 0;  i < HDRFW_LOG_SLOTS; i++ )
  {
    hdrsrchlog_stxd_logging.rx0 += HDRLOG_GET_PER_SLOT_AGC(rx_buf_idx, i, 0);

    hdrsrchlog_stxd_logging.rx1 += HDRLOG_GET_PER_SLOT_AGC(rx_buf_idx, i, 1);
  }

  /* save TxTotal Pwr in FW raw format */
  tx_total =  rfm_hdr_get_tx_total_power( 
                hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), 
                RFM_HDR_CARRIER_ID__0, 
                RFM_HDR_AGC_FORMAT__DBM256 );

  hdrsrchlog_stxd_logging.tx_total += tx_total;

  hdrsrchlog_stxd_logging.ratchet += HDRLOG_GET_PER_SUBFRAME_RATCHET(tx_buf_idx);

  hdrsrchlog_stxd_logging.slots += HDRFW_LOG_SLOTS;

  if ( hdrsrchlog_stxd_logging.force_resend ||
       ( !hdrsrchlog_stxd_logging.read_pending ) || /* if no pending read*/
       ( ( hdrsrchlog_stxd_logging.slots % /* or a lot of data has queued up */
           HDRSRCHLOG_STXD_LOGGING_NOTIFY_INTERVAL ) == HDRFW_LOG_SLOTS ) )
  {
    /* Send a MDSP ready every HDRSRCHLOG_STXD_LOGGING_INTERVAL slots,
       or when forced to resend. This is to minimize STM Q usage */    
    hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_MDSP_INFO_READY ); 

    hdrsrchlog_stxd_logging.force_resend = FALSE;
  }

  hdrsrchlog_stxd_logging.read_pending = TRUE;

  rex_leave_crit_sect( &hdrsrchlog_stxd_logging.crit_sect );
}

/*===========================================================================

FUNCTION     : HDRSRCHLOG_STXD_SET_MDSP_INFO_NEEDED

DESCRIPTION  : This function sets if sTxD state machine is interested 
               in receiving MDSP logging data 

DEPENDENCIES : None

RETURN VALUE : None.

SIDE EFFECTS : None

===========================================================================*/
void hdrsrchlog_stxd_set_mdsp_info_needed
(
  boolean needed
)
{  
  if ( hdrsrchlog_stxd_logging.mdsp_info_needed != needed )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "TxD: MDSP info needed %d=>%d", 
                    hdrsrchlog_stxd_logging.mdsp_info_needed, 
                    needed );

    hdrsrchlog_stxd_logging.mdsp_info_needed = needed;

    if ( needed )
    {
      /* MDSP info just requested, send an update immediately */
      hdrsrchlog_stxd_logging.force_resend = TRUE;
    }
  }
}
#endif /* FEATURE_HDR_SELECTIVE_TXD */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_PROCESS_MDSP_LOG_EVENT

DESCRIPTION  : This function is called by LOG ISR every subframe and copies 
               one subframe worth of per-slot information from RX and Tx
               log buffer.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/
void hdrsrchlog_process_mdsp_log_event
(
  hdrlog_mdsp_slot_logging_data_type *sl_params
    /* mDSP slot logging event parameters */
)
{
  static uint8 frame_index = 0;
    /* Frame Index */

  static uint8 num_frames_buffered = 0;
    /* Number of frames buffered */

  uint8 c_idx;
    /* Carrier Index */

  uint8 slot_idx;
    /* Slot index used as loop counter */

  uint8 buff_idx;
    /* Buffer index */

  uint8 frame_offset = hdrsrch.frame_offset;
    /* Frame offset */

  uint16 first_index_str;
    /* Slot time reference corresponding to the first index in the log 
       packet */

  hdrlog_link_summary_type *link_summary_ptr;
    /* Convenience pointer to Rel-0/A Airlinksummary buffer */

  log_hdr_drc_arq_dsc_buffer_type *drc_arq_dsc_ptr;
    /* Convenience pointer to Rev-A DRC-ARQ-DSC Buffer */

  log_hdr_drc_arq_buffer_type *drc_arq_ptr;
    /* Convenience Pointer to Rel-0 DRC-ARQ Buffer */

  log_hdr_eq_ci_buffer_type *eq_ci_ptr;
    /* Convenience pointer to Rel-0/A EQ C2I Buffer */

  hdrlog_per_slot_eq_ci_buffer_type *mc_eq_ci_ptr;
    /* Convenience pointer to MC Equalizer C2I Buffer */

  uint8 pred_drc_rate = 0;
    /* Predicted DRC */

  uint32 basp_sinr_tmp = 0;
    /* Temp variable for Capping Best ASP SINR */

  uint32 eq_best_ci_tmp = 0;
    /* Temp variable for storing the best equalizer C2I value */ 

  uint8 subframe_offset = (frame_offset % HDRLOG_NUM_SLOTS_IN_SUBFRAME);

  hdrsrch_demod_idx_type demod_idx = 
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
    /* Demod index for IDLE subaset */

  uint16 drc_lock;
    /* Temp to hold drc lock value for logging. */

  uint8 drc_lock_update;
    /* Temp to hold drc lock update value for logging. */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-----------------------------------------------------------------
                            REV-0 AND REV-A
  ------------------------------------------------------------------*/
#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchlog_stxd_send_mdsp_info( sl_params->rx_log_buff_idx, 
                                  sl_params->tx_log_buff_idx );
    /* read AGC every subframe and send to sTxD */
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  /* For each Rx buffer in mDSP log buffer */
  for ( buff_idx = 0;
        buff_idx < HDRLOG_MDSP_LOG_BUFF_SIZE_SLOTS; ++buff_idx )
  {
    slot_idx = 
      ( ( sl_params->slot_time + buff_idx ) % HDRLOG_NUM_SLOTS_IN_FRAME );
      /* Find the index of the slot with in a frame */

    /*----------------------------------------------------------------- 
                            EQUALIZER C2I BUFFER
    ------------------------------------------------------------------*/

    eq_ci_ptr = &hdrsrchlog_slot_logging.eq_ci[frame_index];
      /* Initialize the convenience pointer */

    eq_ci_ptr->eq_status[slot_idx] = 
      HDRLOG_GET_PER_SLOT_EQ_DEMOD_DECISION( 
        sl_params->rx_log_buff_idx, demod_idx, buff_idx);
      /* Demod Decision */

    eq_best_ci_tmp =
      HDRLOG_GET_PER_SLOT_EQ_BEST_C2I( 
        sl_params->rx_log_buff_idx, demod_idx, buff_idx); /*lint !e123 */

    if ( eq_best_ci_tmp >= 0xFFFF )
    {
      eq_ci_ptr->eq_best_ci[slot_idx] = 0xFFFF;
    }
     /* Saturate the best C2I value if over 16 bits long */
    else
    {
      eq_ci_ptr->eq_best_ci[slot_idx] = eq_best_ci_tmp; 
    }
     /* Equalizer Best C2I */

    eq_ci_ptr->eq_alt_rake_ci[slot_idx] =
      HDRLOG_GET_PER_SLOT_ALT_RAKE_C2I( 
        sl_params->rx_log_buff_idx,  demod_idx, buff_idx);
      /* Equalizer Alt Rake C2I */


    /*----------------------------------------------------------------- 
                            AIR LINK SUMMARY
    ------------------------------------------------------------------*/

    link_summary_ptr = &hdrsrchlog_slot_logging.link_summary[frame_index];
      /* Initialize the convenience pointer */

    /* BestASPSINR in Rel-0/A is only 16 bits. The formula for tools to
       convert this linear value to dB is 10log(BestASPSINR/512) */
    basp_sinr_tmp = HDRLOG_GET_PER_SLOT_BASP_SINR( 
      sl_params->rx_log_buff_idx, demod_idx, buff_idx ); /*lint !e123 */

    /* BestASPSINR is a 16-bit field in legacy Airlinksummary log packet.
       When equalizer is enabled, with higher I-Q bitwidth on recent 
       targets, this is not enough to capture the entire range of SINR.
       So, the BASPSINR is ratcheted at 18.5dB when it goes beyond this. */
    link_summary_ptr->basp_sinr[slot_idx] = 
      (uint16) ( (basp_sinr_tmp > HDRLOG_LEGACY_MAX_BASP_SINR)? 
                 HDRLOG_LEGACY_MAX_BASP_SINR: basp_sinr_tmp );

#ifdef FEATURE_HDR_SELECTIVE_TXD
    if ( buff_idx == HDRLOG_MDSP_LOG_BUFF_SIZE_SLOTS - 1)
    {
      hdrsrchlog.last_basp_sinr = link_summary_ptr->basp_sinr[slot_idx];
    }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

    /* In Release-0 and A, it takes only a Nibble to store a DRC value.
       So, 16 predicted DRC values, one for each slot of a frame are 
       bit packed in 8 bytes. 

       The variable 'pred_drc_rate' variable is packed every two iterations 
       of the 'for' loop starting from an even slot index. However, for 
       an odd frame_offset, the slot_time given by firmware would be odd 
       and hence the starting slot_idx would also be odd, leading to 
       executing the else part below for the first iteration of the loop. 
       In this case, the local variable (pred_drc_rate) would start as 
       zero if we don't preserve this value from previous function call 
       resulting in an incorrect logged value */

    /* If this slot is an even slot */
    if ((slot_idx % 2) == 0)
    {
      pred_drc_rate = ( HDRLOG_GET_PER_SLOT_PRED_DRC(
        sl_params->tx_log_buff_idx, demod_idx, buff_idx) & 0x0F );
        /* For even slot, just read the predicted DRC */

      /* If this is the last iteration of the loop, that means we started off
         with odd slot_idx, remember the DRC value in the buffer temporarily. */
      if (buff_idx == (HDRLOG_MDSP_LOG_BUFF_SIZE_SLOTS - 1))
      {
        link_summary_ptr->drc_buffer[(slot_idx / 2)] = pred_drc_rate;
      }
    }
    else
    {
      /* If this is the first iteration of the loop, that means we have started
         with an odd slot_idx, restore the DRC value from the buffer */
      if (buff_idx == 0)
      {
        pred_drc_rate = link_summary_ptr->drc_buffer[(slot_idx / 2)];
      }

      pred_drc_rate =
        ( pred_drc_rate | 
          ( ( HDRLOG_GET_PER_SLOT_PRED_DRC( sl_params->tx_log_buff_idx, 
                                            demod_idx, 
                                            buff_idx ) & 0x0F ) << 4 ) );
        /* For Odd slots, read predicted DRC, left shift by 4 bits and OR
           with predicted DRC of previous slot (even slot) and hence 
           bit pack DRCs for two slots in a byte */

      link_summary_ptr->drc_buffer[(slot_idx / 2)] = pred_drc_rate;
        /* Store the hence built pred_drc into the log buffer */
    }

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
    if ( HDRSRCH_FL_DATA_ADAPTIVE_RXD_NV_ENABLE &&
         ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE ) &&
         ( buff_idx == HDRLOG_MDSP_LOG_BUFF_SIZE_SLOTS - 1) )
         /* Only check the last slot to minimize unnecesary
            DRC changed event. If the DRC change was temp.
            and it recovered by the last slot, don't need
            to process it. */
    {
      hdrsrchlog.pred_drc_rate = HDRLOG_GET_PER_SLOT_PRED_DRC(
                              sl_params->tx_log_buff_idx, demod_idx, buff_idx) & 0x0F;

      hdrsrchlog.filt_pred_drc = IIR_FILT_N(  hdrsrchlog.filt_pred_drc,
                                              hdrsrchlog.pred_drc_rate,
                                              HDR_FILT_PRED_DRC_FILT_LEN
                                          );

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Predicted DRC (Instaneous) %d , Filtered DRC %d", 
                      hdrsrchlog.pred_drc_rate, hdrsrchlog.filt_pred_drc );

      if ( ( hdrsrchlog.drc_below_div_on_thres && 
             ( hdrsrchlog.filt_pred_drc > HDRSRCHARD_DIV_OFF_DRC ) ) ||
           ( ( !hdrsrchlog.drc_below_div_on_thres ) &&
             ( hdrsrchlog.filt_pred_drc < HDRSRCHARD_DIV_ON_DRC ) ) )
      {
        hdrsrchlog.drc_below_div_on_thres = 
          ( hdrsrchlog.filt_pred_drc < HDRSRCHARD_DIV_ON_DRC );
  
        hdrsrchard_event( HDRSRCHARD_EVENT_DRC_CHANGED );
      }
    }
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */

    /*----------------------------------------------------------------- 
                            DRC-ARQ-DSC Buffer
    ------------------------------------------------------------------*/

    drc_arq_ptr = &hdrsrchlog_slot_logging.drc_arq[frame_index];
    drc_arq_dsc_ptr = &hdrsrchlog_slot_logging.drc_arq_dsc[frame_index];

    if (slot_idx == 0)
    {
      uint8 frame_boundary_offset = 0;
        /* Number of slots from "slot_time" to nearest slot marking frame 
           boundary */

      /* slot_time marks the first slot of subframe (relative to frame offset).
         However, the slot corresponding to slot_index 0 is the first slot of 
         frame boundary (not relative to frame offset). So, in order to find
         this time based on slot_time, we find the offset in slots from 
         slot_time to the nearest frame boundary. Note that this offset is
         zero if subframe_offset is zero */

      frame_boundary_offset = 
        ((subframe_offset > 0)? 
         (HDRLOG_NUM_SLOTS_IN_SUBFRAME - subframe_offset): 0 );

      drc_arq_ptr->slot_count = 
        (uint8) ( sl_params->slot_time + frame_boundary_offset );
      drc_arq_dsc_ptr->slot_count = 
        (uint8) (sl_params->slot_time + frame_boundary_offset);
    }

    /* Except for DRC Value all other fields are in same bit position
       in Artemis interface as before */
    drc_arq_ptr->drc[slot_idx] = 
      drc_arq_dsc_ptr->drc_arq_dsc[slot_idx] = 
        HDRLOG_GET_PER_SLOT_MC_RL_INFO( sl_params->tx_log_buff_idx, 
                                        demod_idx, 
                                        buff_idx );
      /* Copy the RL field */

    /* Bit 10 and 11 of RL field just read above has DSCGainBoost and 
       DRCGainBoost. These bits are reserved fields in Rel-0 and Rev-A 
       log packets. Just interpret those bits and store in appropriate
       location */
    if (drc_arq_dsc_ptr->drc_arq_dsc[slot_idx] & 0x0800)
    {
      drc_arq_dsc_ptr->drc_gain_boost |= (1 << slot_idx);
    }

    if (drc_arq_dsc_ptr->drc_arq_dsc[slot_idx] & 0x0400)
    {
      drc_arq_dsc_ptr->dsc_gain_boost |= (1 << slot_idx);
    }

    drc_arq_ptr->drc[slot_idx] = 
      ( ( drc_arq_ptr->drc[slot_idx] & 0xF0FF ) | 
        ( ( HDRLOG_GET_PER_SLOT_TRANS_DRC( sl_params->tx_log_buff_idx, 
                                           demod_idx, 
                                           buff_idx ) & 0x0F ) << 8 ) );
      /* Copy the DRC Value */

    drc_arq_dsc_ptr->drc_arq_dsc[slot_idx] = drc_arq_ptr->drc[slot_idx];

    /* ASP in Artemis goes from 0 thru 5 instead of 1 thru 6. Adding
       1 to ASP indices to maintain backward compatibility to legacy
       log packets. */

    drc_arq_ptr->best_asp_index[slot_idx].asp_fl = 
      drc_arq_dsc_ptr->best_asp_index[slot_idx].asp_fl = 
        ( ( ( HDRLOG_GET_PER_SLOT_ASP( sl_params->rx_log_buff_idx, 
                                       demod_idx,
                                       buff_idx ) & 0x70 ) >> 4 ) + 1 );
      /* Copy the Demod ASP for this slot from Rx log buffer */

    drc_arq_ptr->best_asp_index[slot_idx].asp_rl = 
      drc_arq_dsc_ptr->best_asp_index[slot_idx].asp_rl = 
        ( ( HDRLOG_GET_PER_SLOT_ASP( sl_params->rx_log_buff_idx, 
                                   demod_idx, 
                                   buff_idx ) & 0x07 ) + 1 );
      /* Copy the Next ASP for this slot from Rx log buffer */


    /*----------------------------------------------------------------- 
           COLLECT PER-FRAME STATS AND TRIGGER LOGGING (WHEN READY)
    ------------------------------------------------------------------*/

    /* If this slot marks the boundary of the frame */
    if (slot_idx == (HDRLOG_NUM_SLOTS_IN_FRAME - 1))
    {
      num_frames_buffered++;
        /* Increment the number of frames buffered */

      hdrsrchlog_get_per_frame_link_summary_data( link_summary_ptr );
        /* Read per-frame link summary data into the buffer */

      hdrsrchlog_update_drc_lock ( &drc_lock,
                                   &drc_lock_update );
        /* Update DRC Lock and DRC Lock Update */

      drc_arq_ptr->drc_lock = drc_lock;
      drc_arq_ptr->drc_lock_update = drc_lock_update;
        /* Copy DRC Lock and DRC Lock Update for Rel-0 log */

      drc_arq_dsc_ptr->drc_lock = drc_lock;
      drc_arq_dsc_ptr->drc_lock_update = drc_lock_update;
        /* Copy DRC Lock and DRC Lock Update for Rel-A log */

      frame_index = (frame_index + 1) % HDRLOG_FRAMES_PER_PKT;
        /* We are done with the current frame. Toggle the frame index to 
           continue working on next frame's buffer */

      /* If we have two frames worth of Eq data buffered already, */
      if ( num_frames_buffered == HDRLOG_FRAMES_PER_PKT )
      {
        /* Trigger logging */

        hdrsrchlog_log_link_summary();
        hdrsrchlog_log_drc_arq_buffer_record();
        hdrsrchlog_log_drc_arq_dsc_buffer_record();
        hdrsrchlog_log_eq_ci_buffer();

        num_frames_buffered = 0;
          /* We are done logging the frames buffered. Reset Number 
             of Frames Buffered */

      } /* If we have two frames worth of data buffered */

    } /* slot marks Frame boundary */

  } /* for each Rx buffer in mDSP log buffer */

  /*-----------------------------------------------------------------
                              REV-B
  ------------------------------------------------------------------*/

  /* For each carrier */
  for ( c_idx = 0; c_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; ++c_idx )
  {

    /* For each slot logged by mDSP */
    for ( slot_idx = 0, 
          buff_idx = HDRSRCHLOG_SF_START_LOG_BUFF_IDX( sl_params->slot_time );
          slot_idx < HDRSRCHLOG_MDSP_LOG_BUFF_SIZE_SLOTS; ++slot_idx )
    {

      /*--------------------------------------------------------------
                      Read information from Rx Log buffer
       -------------------------------------------------------------*/

      mc_eq_ci_ptr = &hdrsrchlog_slot_logging.mc_eq_ci_buffer[c_idx][buff_idx];
        /* Initialize the convenience pointer */

      mc_eq_ci_ptr->eq_status = HDRLOG_GET_PER_SLOT_EQ_DEMOD_DECISION( 
        sl_params->rx_log_buff_idx, c_idx, slot_idx );
        /* Read Equalizer Demod Decision for this slot */

      mc_eq_ci_ptr->eq_best_ci = HDRLOG_GET_PER_SLOT_EQ_BEST_C2I( 
        sl_params->rx_log_buff_idx, c_idx, slot_idx ); /*lint !e123 */
        /* Read Equalizer Best C2I for this slot */

      mc_eq_ci_ptr->eq_alt_rake_ci = HDRLOG_GET_PER_SLOT_ALT_RAKE_C2I( 
        sl_params->rx_log_buff_idx, c_idx, slot_idx );
        /* Read Alt Rake C2I for this slot */

      mc_eq_ci_ptr->g_asp = HDRLOG_GET_PER_SLOT_G_ASP( 
        sl_params->rx_log_buff_idx, c_idx, slot_idx );
        /* Read G_ASP data for this slot */

      /*--------------------------------------------------------------
                        MULTI-CARRIER DRC-ARQ-DSC BUFFER
       -------------------------------------------------------------*/

      hdrsrchlog_slot_logging.mc_drc_arq_dsc_buffer [c_idx][buff_idx].asp =
        HDRLOG_GET_PER_SLOT_ASP( sl_params->rx_log_buff_idx, c_idx, slot_idx );
        /* Copy the Demod ASP and Next ASP for this slot from Rx log buffer */

      hdrsrchlog_slot_logging.mc_drc_arq_dsc_buffer [c_idx][buff_idx].rl =
        HDRLOG_GET_PER_SLOT_MC_RL_INFO( sl_params->tx_log_buff_idx, 
                                        c_idx, 
                                        slot_idx );
        /* Read RL field from tx log buffer which includes DRCCover, DSC Value,
           DRC/DSCBoost Info, ACK/NAK Info. Please note that these fields in
           the log packet are bit-packed in a similar manner to mDSP logging 
           buffer. So copying the RL field directly is efficient. */

      hdrsrchlog_slot_logging.mc_drc_arq_dsc_buffer [c_idx][buff_idx].trans_drc =
        HDRLOG_GET_PER_SLOT_TRANS_DRC( sl_params->tx_log_buff_idx, 
                                       c_idx, 
                                       slot_idx );
      if(c_idx == 0)
      {
         hdrsrchlog_mc_trans_drc_value = 
                 hdrsrchlog_slot_logging.mc_drc_arq_dsc_buffer [c_idx][buff_idx].trans_drc;
      }
        /* Copy the transmitted DRC value for this slot */


      /*--------------------------------------------------------------
                        MULTI-CARRIER AIR LINK SUMMARY
       -------------------------------------------------------------*/

      hdrsrchlog_slot_logging.mc_link_summary [c_idx][buff_idx].basp_sinr = 
        HDRLOG_GET_PER_SLOT_BASP_SINR( 
          sl_params->rx_log_buff_idx, c_idx, slot_idx ); /*lint !e123 */
        /* Read BestASP SINR */

      hdrsrchlog_slot_logging.mc_link_summary [c_idx][buff_idx].pred_drc = 
        HDRLOG_GET_PER_SLOT_PRED_DRC( sl_params->tx_log_buff_idx, 
                                      c_idx, 
                                      slot_idx );
        /* Read Predicted DRC */

      buff_idx = HDRSRCHLOG_NEXT_LOG_BUFF_IDX( buff_idx );
        /* Move-on to the next index in the buffer */

    } /* for each slot */

  } /* for each demod carrier */

  hdrsrchlog_slot_logging.rx_tx_cbuff_valid |= sl_params->rx_tx_buff_valid_mask;
    /* Find a union of all carriers that were active during the current frame */

  hdrsrchlog_slot_logging.rx_cbuff_valid |= sl_params->rx_buff_valid_mask;
    /* Find a union of all carriers that were active during the current frame */

  /*----------------------------------------------------------------- 
         COLLECT PER-FRAME STATS AND TRIGGER LOGGING (WHEN READY)
  ------------------------------------------------------------------*/

  /* If we are in last sub-frame of a freme, ship the logs collected for
     the current frame */
  if ( HDRSRCHLOG_BUFFER_READY_FOR_LOGGING( sl_params->slot_time, frame_offset ) )
  {
    first_index_str = 
      HDRSRCHLOG_FRAME_START_STR( sl_params->slot_time, frame_offset );
      /* STR corresponding to first slot of the frame */

    hdrsrchlog_log_mc_drc_arq_dsc_buffer_record( 
      HDRSRCHLOG_FRAME_START_LOG_BUFF_IDX( sl_params->slot_time ),
      hdrsrchlog_slot_logging.rx_tx_cbuff_valid,
      first_index_str );
      /* Log Multi-carrier DRC ARQ DSC buffer */

    hdrsrchlog_log_mc_link_summary(
      HDRSRCHLOG_FRAME_START_LOG_BUFF_IDX( sl_params->slot_time ),
      hdrsrchlog_slot_logging.rx_tx_cbuff_valid,
      first_index_str);

    hdrsrchlog_log_mc_eq_ci_buffer(
      HDRSRCHLOG_FRAME_START_LOG_BUFF_IDX( sl_params->slot_time ),
      hdrsrchlog_slot_logging.rx_cbuff_valid,
      first_index_str );
      /* Log Multi-carrier DRC ARQ DSC buffer */

    /* Reset the masks */
    hdrsrchlog_slot_logging.rx_cbuff_valid = 0;
    hdrsrchlog_slot_logging.rx_tx_cbuff_valid = 0;

#ifdef FEATURE_POWER_INFO_LOG_PACKET
    hdrsrchlog_log_physical_layer_power_info( HDRLOG_GET_PWR_LOG_BUF_INDEX() );
      /* Trigger Physical layer power info log packet to be sent every frame.
         DTX and DRX information is read from FW */
#endif /* FEATURE_POWER_INFO_LOG_PACKET */

  } /* If ready for logging */

} /* hdrsrchlog_process_mdsp_log_event */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_GET_FW_LOG_STATUS_ENTRY

DESCRIPTION  : The function returns the FW log status entry of the given 
               log_id, or NULL if not found 

DEPENDENCIES : None

RETURN VALUE : Pointer to the log entry, or NULL if not found

SIDE EFFECTS : None
=========================================================================*/
hdrsrchlog_fw_log_status_entry_type* hdrsrchlog_get_fw_log_status_entry
(
  uint32 log_id
)
{
  uint8 i;
  hdrsrchlog_fw_log_status_entry_type* entry = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  for ( i = 0; 
        i < sizeof(hdrsrchlog_fw_log_status) / 
            sizeof(hdrsrchlog_fw_log_status_entry_type); 
        i++ )
  {
    if ( hdrsrchlog_fw_log_status[i].log_id == log_id )
    {
      entry = &hdrsrchlog_fw_log_status[i];
      break;
    }
  }

  return entry;
}

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_PN_ROLL_LOGGING

DESCRIPTION  : The function checks if a log pkt is enabled/disabled by 
               QXDM, and passes the setting to FW 

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : updates *enabled parameter 
=========================================================================*/
void hdrsrchlog_chk_and_set_fw_logging
( 
  uint32 log_id
)
{
  boolean log_enabled;
  hdrfw_diag_log_config_msg_t log_cfg_msg;
  hdrsrchlog_fw_log_status_entry_type* log_entry;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  log_entry = hdrsrchlog_get_fw_log_status_entry( log_id );

  ASSERT( log_entry != NULL );

  log_enabled = log_status( log_id );

  if ( log_enabled != log_entry->enabled )
  {
    memset( &log_cfg_msg, 0, sizeof( log_cfg_msg ) );

    msgr_init_hdr ( &log_cfg_msg.hdr, 
                    MSGR_HDR_SRCH,
                    HDR_FW_CMD_DIAG_LOG_CONFIG );

    log_cfg_msg.diagLogID = log_id;
    log_cfg_msg.enable = log_enabled;

    hdrsrchmsg_send_msg( &log_cfg_msg, sizeof( log_cfg_msg ) );

    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "Set log id %x: %d=>%d", 
                    log_id, log_entry->enabled, log_enabled );

    log_entry->enabled = log_enabled;
  }
}

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_PN_ROLL_LOGGING

DESCRIPTION  : Logging to be done at PN ROLL ISR context

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none
=========================================================================*/
void hdrsrchlog_pn_roll_logging( void )
{

  boolean pri_tune_done = FALSE;
    /* Is RF tuning done on the primary chain */

  boolean div_enabled = FALSE;
    /* Is diversity chain enabled? */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrch_state_id != HDRSRCH_SLEEP_STATE && hdrmdsp_check_dsp_clk() )
  {
    /* For Rx AGC log packet logging */ 
    hdrsrchlog_chk_and_set_fw_logging( HDRFW_DIAG_LOG_RX_AGC_PKT_ID );

    /* For Tx AGC log packet logging */
    hdrsrchlog_chk_and_set_fw_logging( HDRFW_DIAG_LOG_TXAGC_PKT_ID );

    /* For DLNA log packet logging */
    hdrsrchlog_chk_and_set_fw_logging( HDRFW_DIAG_LOG_DLNA_PKT_ID );
     
    /* For AFC log packet logging */
    hdrsrchlog_chk_and_set_fw_logging( HDRFW_DIAG_LOG_AFC_PKT_ID );

    /* For FING log packet logging */
    hdrsrchlog_chk_and_set_fw_logging( HDRFW_DIAG_LOG_FING_PKT_ID );

    /* For MMSE log packet logging */
    hdrsrchlog_chk_and_set_fw_logging( HDRFW_DIAG_LOG_MAC_PKT_ID );

    /* For FW RX IQ log packet logging */
    hdrsrchlog_chk_and_set_fw_logging( HDRFW_DIAG_LOG_RX_IQ_SAMPLE_PKT_ID );

    /* For FW VSTMR log packet logging */
    hdrsrchlog_chk_and_set_fw_logging( HDRFW_DIAG_LOG_VSRC_STMR_PKT_ID );

    /* For QTA FW log packet logging */
    hdrsrchlog_chk_and_set_fw_logging( HDRFW_DIAG_LOG_DSDS_QTA_PKT_ID );

    if( hdrsrch_get_current_state() != HDRSRCH_SUSPENDED_TC_STATE )
    {
      /* When HDR is in QTA gap/traffic suspend state prevent the rxagc logging */
    pri_tune_done = hdrsrchrf_rf_chain_tune_done (HDRSRCHRF_PRI_CHAIN);
      /* Check if RF tuning is done on primary chain */

    div_enabled = hdrsrchrfdiv_diversity_is_enabled ();
      /* Check if diversity chain is owned and enabled by DO */
    }

    hdrrxstats_log_rx_data( pri_tune_done, div_enabled );
      /* log the RX Task data that needs to be logged at PN roll */

#if   defined( FEATURE_MDSPSVC_LOG )
    hdrmdsp_update_log_mode();
      /* Update MDSP debug logging mode if needed  */
#endif /* FEATURE_MDSPSVC_LOG */

    /* If either one of the mDSP slot based log packets are enabled, */
    if ( log_status (LOG_HDR_LINK_SUMMARY_C) ||
         log_status (LOG_HDR_DRC_ARQ_BUFFER_C) ||
         log_status (LOG_HDR_DRC_ARQ_DSC_BUFFER_C) ||
         log_status (LOG_HDR_EQUALIZER_CI_BUFFER_C) ||
         log_status (LOG_HDR_MC_EQUALIZER_CI_BUFFER_C) ||
         log_status (LOG_HDR_MC_DRC_ARQ_DSC_BUFFER_C) ||
         log_status (LOG_HDR_MC_LINK_SUMMARY_C) 
#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
         || ( HDRSRCH_FL_DATA_ADAPTIVE_RXD_NV_ENABLE )
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
#ifdef FEATURE_HDR_SELECTIVE_TXD
         || hdrsrchrftxd_asdiv_is_enabled( )
#endif /* FEATURE_HDR_SELECTIVE_TXD */
       )
    {
      
      hdrlog_register_slot_logging_cb ( HDRLOG_SRCH_MDSP_SLOT_LOGGING_CLIENT,
                                        hdrsrchlog_process_mdsp_log_event);
        /* Register call back for slot based logging */
    }

    hdrlog_enable_mdsp_log_isr();
      /* Enable mDSP slot log ISR if necessary */

    /* Log DSDA FW packets if needed */
    hdrmultirat_enable_rx_fw_logging();
    if ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE )
    {
      hdrmultirat_enable_tx_fw_logging();
    }


  } /* if not sleep */

} /* hdrsrchlog_pn_roll_logging */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_ENABLE_IRAT_FW_LOGGING

DESCRIPTION  : Enable FW log pkts required for IRAT meas debug

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none
=========================================================================*/
void hdrsrchlog_enable_irat_fw_logging( void )
{

   if ( hdrsrch_state_id == HDRSRCH_OFS_MEAS_STATE && hdrmdsp_check_dsp_clk() )
   {
     /* For Rx AGC log packet logging */ 
     hdrsrchlog_chk_and_set_fw_logging( HDRFW_DIAG_LOG_RX_AGC_PKT_ID );

     /* For AFC log packet logging */
     hdrsrchlog_chk_and_set_fw_logging( HDRFW_DIAG_LOG_AFC_PKT_ID );

     /* For FW RX IQ log packet logging */
     hdrsrchlog_chk_and_set_fw_logging( HDRFW_DIAG_LOG_RX_IQ_SAMPLE_PKT_ID );
   }

} /* hdrsrchlog_enable_irat_fw_logging */

/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_SRCH_DIAGNOSTICS

DESCRIPTION  : This function logs the searcher diagnostics information. Note
               that these functions will log data only if the log mask for
               the packet has been enabled. Log mask will be enabled if the
               QXDM screen/display which shows packet information is active
               or the user has specifically enabled logging for that packet.

INPUT        : TRUE  - Log diagnostics instantly ignoring rate governer
               FALSE - Log diagnostics at restrained rate controlled by dump
                       count

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_srch_diagnostics
(
  boolean instant
    /* 0 : log diagnostics at restrained rate controlled by dump count
       1 : log diagnostics instantly. Ignores rate governer */
)
{
  uint8 dumpcnt;       /* local copy of dump count */

  hdrsrch_demod_idx_type demod_idx = 
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
    /* Demod index for IDLE subaset */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  dumpcnt = hdrsrchdrv.dump_cnt;
  /* make a local copy of the dump count */

  if ( !dumpcnt )
  {
    dumpcnt = 1;
    /* dump count goes from 1 to 255 instead of 0 to 255 */
  }


  /* Print messages once every 70 dumps = 70 * 30ms = 2100 msec */

  if ( hdrsrchdrv.dump_cnt % 70 == 0 )
  {
#ifdef FEATURE_HDR_REVB
    HDR_MSG_SRCH_2( MSG_LEGACY_LOW, "AGC0=%d AGC1=%d ",
                    rfm_hdr_get_rx_agc( hdrsrchrf_get_rf_dev(HDRSRCHRF_PRI_CHAIN),
                                        RFM_HDR_CARRIER_ID__CUMULATIVE,
                                        RFM_HDR_AGC_FORMAT__DBM256)/256,
                    rfm_hdr_get_rx_agc( hdrsrchrf_get_rf_dev(HDRSRCHRF_DIV_CHAIN),
                                        RFM_HDR_CARRIER_ID__CUMULATIVE,
                                        RFM_HDR_AGC_FORMAT__DBM256)/256 );
#else
    HDR_MSG_SRCH_2( MSG_LEGACY_LOW, "AGC0=%d AGC1=%d ",
                    RF_HDR_GET_RX0_AGC_DB256()/256,
                    RF_HDR_GET_RX1_AGC_DB256()/256 );
#endif /* FEATURE_HDR_REVB */

    HDR_MSG_SRCH_3( MSG_LEGACY_LOW, "F0RSSI ant0=%d ant1=%d comb=%d",
                    HDRFING_GET_ANT_RSSI( demod_idx, 
                                          0, 
                                          HDRMDSP_ANT_0 ),
                    HDRFING_GET_ANT_RSSI( demod_idx, 
                                          0, 
                                          HDRMDSP_ANT_1 ),
                    HDRFING_GET_RSSI( demod_idx, 0 ) );

    HDR_MSG_SRCH_3( MSG_LEGACY_LOW, "F1RSSI ant0=%d ant1=%d comb=%d",
                    HDRFING_GET_ANT_RSSI( demod_idx, 
                                          1, 
                                          HDRMDSP_ANT_0 ),
                    HDRFING_GET_ANT_RSSI( demod_idx, 
                                          1, 
                                          HDRMDSP_ANT_1 ),
                    HDRFING_GET_RSSI( demod_idx, 1 ) );

  }

  /*-----------------------------------------------------------
                  Log search packets
   *----------------------------------------------------------*/

  if ( HDRSRCH_LOG_FING( dumpcnt ) || instant )
  {
    hdrsrchlog_log_fing_record();
    /* log finger record with the latest finger dump */

    hdrsrchlog_log_mc_fing_record();
      /* Log multi carrier finger record with the latest finger dump */
  }

  if ( HDRSRCH_LOG_EQ( dumpcnt ) || instant )
  {
    hdrsrchlog_log_eq_data();
      /* log equalizer data record with the latest equalizer dump */

    hdrsrchlog_log_mc_eq_data();
      /* Log multi carrier equalizer data record with latest equalizer dump */
  }

  if ( HDRLOG_DEBUG_FILL( dumpcnt ) || instant )
  {
    hdrsrchlog_debug_display_fill_rec();
      /* Fill debug display global structure */

    hdrsrchlog_log_debug_display_record();
      /* log debug display  record */

  }

}/* hdrsrchlog_srch_diagnostics() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_SRCH_DIAGNOSTICS_PILOT_SETS

DESCRIPTION  : This function logs the searcher dignostics information which
               contains updated pilot sets information.  This was previously
               part of hdrsrchlog_srch_diagnostics(), but pilot sets
               information was stale, i.e., before pilot set management.  Note
               that these functions will log data only if the log mask for
               the packet has been enabled. Log mask will be enabled if the
               QXDM screen/display which shows packet information is active
               or the user has specifically enabled logging for that packet.

INPUT        : TRUE  - Log diagnostics instantly ignoring rate governer
               FALSE - Log diagnostics at restrained rate controlled by dump
                       count

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_srch_diagnostics_pilot_sets
(
  boolean instant
    /* 0 : log diagnostics at restrained rate controlled by dump count
       1 : log diagnostics instantly. Ignores rate governer */
)
{
  uint8 dumpcnt;       /* local copy of dump count */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  dumpcnt = hdrsrchdrv.dump_cnt;
    /* make a local copy of the dump count */

  if ( !dumpcnt )
  {
    dumpcnt = 1;
      /* dump count goes from 1 to 255 instead of 0 to 255 */
  }

  /*-----------------------------------------------------------
          Log search packets related to pilot sets
   *----------------------------------------------------------*/

  if ( HDRSRCH_LOG_SRCH( dumpcnt ) || instant )
  {
    hdrsrchlog_log_search_record();
      /* log search record */

    hdrsrchlog_log_mc_search_record();
      /* Log Multi-carrier search record */

  }

  if ( HDRSRCH_LOG_SET( dumpcnt ) || instant )
  {
    hdrsrchlog_log_pilot_set_record();
      /* log pilot set list to this record */

    hdrsrchlog_log_mc_pilot_set_record();
      /* log MC pilot set list to this record */

  }

} /* hdrsrchlog_srch_diagnostics_pilot_sets() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_ASET_CHANGE_LOG

DESCRIPTION  : Log packets which are triggered by the change in the ASET

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_aset_change_log( void )
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchlog_log_asp_record();

  hdrsrchlog_log_handoff_state_record();

}/* hdrsrchlog_aset_change_log */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_GLOBAL_ACQ_INFO

DESCRIPTION  : Updates some fields of ACQ_INFO log paket that need to be
               tracked from reset, regardless whether the corresponding
               log bit is set or not.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

LOCAL void hdrsrchlog_update_global_acq_info( void )
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  INC_SAT( hdrsrchlog.acq_info.attempt_count );
    /* This field will saturate at 0xffff. It will not roll over    */

  switch ( hdrsrchlog.acq_info.result )
  {
    case HDRLOG_ACQ_SUCCESS:

      INC_SAT( hdrsrchlog.acq_info.attempt_count_success );
        /* This field will saturate at 0xffff. It will not roll over  */
      break;

    case HDRLOG_ACQ_MICRO_FAILURE:
    case HDRLOG_ACQ_PILOT_FAILURE:
    case HDRLOG_ACQ_FLL_FAILURE:
    case HDRLOG_ACQ_SLAM_FAILURE:
    case HDRLOG_ACQ_SYNC_FAILURE:
    case HDRLOG_ACQ_NULL:

      INC_SAT( hdrsrchlog.acq_info.attempt_count_failure );
        /* This field will saturate at 0xffff. It will not roll over  */

      break;

    default:
        HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                        "Undefined ACQ result: %d",
                        hdrsrchlog.acq_info.result );
  }

} /* hdrsrchlog_update_global_acq_info */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_ACQ_INFO

DESCRIPTION  : Logs the acquisition info. It gets a log record from the diag
               log buffer, fills it with the latest acq information and
               commits the log record to the diag for delivery.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_acq_info( void )
{
  LOG_HDR_ACQ_INFO_C_type *log_ptr;
  /* log pointer to the acq info log record type */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchlog_update_global_acq_info( );
    /* Update global acq info field, regardless of log bit setting      */

  if ( hdrsrchlog.triggered.acq || hdrsrchlog.log_on_demand )
  {
    /* Log the packet when triggered or in on_demand case          */

    log_ptr = (LOG_HDR_ACQ_INFO_C_type *)
              log_alloc( (log_code_type) LOG_HDR_ACQ_INFO_C,
                         sizeof( LOG_HDR_ACQ_INFO_C_type ) );

    if ( log_ptr )
    {
      REX_DISABLE_PREMPTION();

#ifdef FEATURE_MEMCPY_REMOVAL
      log_ptr->acq_info = hdrsrchlog.acq_info;
#else
      memcpy( (void *) (&log_ptr->acq_info),
              (void *) (&hdrsrchlog.acq_info),
              sizeof( hdrsrchlog_acq_info_struct_subtype ) );
#endif /* FEATURE_MEMCPY_REMOVAL */
        /* Copy log info into the log record */

      REX_ENABLE_PREMPTION();

      log_commit( (log_type *) log_ptr );
        /* submit the log to DIAG */

    }/* if log_ptr */

    if ( hdrsrchlog.triggered.acq == TRUE )
    {
      hdrsrchlog.triggered.acq  = FALSE;
        /* Turn off trigger after logging is completed                  */
    }

  }/* if triggered */

}/* hdrsrchlog_log_acq_info() */

/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_REACQ_INFO

DESCRIPTION  : Logs the re-acquisition info. It gets a log record from the 
               diag log buffer, fills it with the latest acq information and
               commits the log record to the diag for delivery.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_reacq_info( void )
{
  LOG_HDR_REACQ_INFO_C_type *log_ptr;
  /* log pointer to the reacq info log record type */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  INC_SAT( hdrsrchlog.reacq_info.sleep_cycle_count );
    /* This field will saturate at 0xffffffff. It will not roll over  */

  if ( hdrsrchlog.triggered.reacq || hdrsrchlog.log_on_demand )
  {
    /* Log the packet when triggered or in on_demand case          */

    log_ptr = (LOG_HDR_REACQ_INFO_C_type *)
              log_alloc( (log_code_type) LOG_HDR_REACQ_INFO_C,
                         sizeof( LOG_HDR_REACQ_INFO_C_type ) );

    if ( log_ptr )
    {
      REX_DISABLE_PREMPTION();

#ifdef FEATURE_MEMCPY_REMOVAL
      log_ptr->reacq_info = hdrsrchlog.reacq_info;
#else
      memcpy( (void *)( &log_ptr->reacq_info ),
              (void *)( &hdrsrchlog.reacq_info ),
              sizeof( hdrsrchlog_reacq_info_struct_subtype ) );
#endif /* FEATURE_MEMCPY_REMOVAL */
        /* Copy log info into the log record */

      REX_ENABLE_PREMPTION();

      log_commit( (log_type *) log_ptr );
        /* submit the log to DIAG */

    }/* if log_ptr */
    
    if ( hdrsrchlog.triggered.reacq == TRUE )
    {
      hdrsrchlog.triggered.reacq  = FALSE;
        /* Turn off trigger after logging is completed                  */
    }

  }/* if triggered */

}/* hdrsrchlog_log_reacq_info() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_SLEEP_INFO

DESCRIPTION  : Logs the sleep info. It gets a log record from the diag
               log buffer, fills it with the latest acq information and
               commits the log record to the diag for delivery.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchlog_log_sleep_info( void )
{
  LOG_HDR_ENH_SLEEP_INFO_C_type *log_ptr;
  /* log pointer to the sleep info log record type */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrchlog.triggered.sleep )
  {
    /* Log the packet when triggered or in on_demand case          */

    if ( hdrsrchsleep_get_intra_scc_valid() == FALSE )
    {
      INC_SAT( hdrsrchlog.sleep_info.sleep_cycle_count );
        /* Increments the cumulative sleep cycle for only non-intra SCC
           sleep. */

      if ( hdrsrchlog.sleep_info.cc_status == HDRLOG_CC_GOOD_CRC )
      {
        INC_SAT( hdrsrchlog.sleep_info.cc_mer_denom );
          /* In the good CRC case we need to increase the CC MER denom. */
      }
    }
  }

  if ( hdrsrchlog.triggered.sleep || hdrsrchlog.log_on_demand )
  {
    /* Log the packet when triggered or in on_demand case          */

    hdrsrchlog.sleep_info.awake_time_ms  = (uint16)
      hdrsrchutil_stop_timer_ms( hdrsrchlog.awake_timer );
      /* Records the awake duration   */

    hdrsrchlog.sleep_info.hdr_sci     = hdrsrchsleep_get_sci( );
      /* Get SCI info from HDR SLEEP  */

    hdrsrchlog.sleep_info.cur_warmup_time_us = hdrsrchsleep_get_cur_warmup_time();
      /* Get current warmup time from HDR SLEEP  */

    if ( hdrsrchlog.sleep_info.cc_status != HDRLOG_CC_REACQ_FAIL )
    {
      hdrsrchlog.sleep_info.cc_status   = hdrrxstats_get_cc_status( );
        /* Get control channel information from HDRRX */
    }


#ifdef FEATURE_HDR_REVB
    if ( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0] == NULL )
    {
      hdrsrchlog.sleep_info.pilot_energy  = 0;
      hdrsrchlog.sleep_info.pilot_pn      = HDR_NO_PN_OFFSET;
    }
    else
    {
      hdrsrchlog.sleep_info.pilot_energy  = 
          hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->total_eng;
      hdrsrchlog.sleep_info.pilot_pn      = 
          hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->pn_offset;
        /* Record ASET PN offset and pilot strength when logging packet */
    }
#else
    if ( hdrsrchset.aset[0] == NULL )
    {
      hdrsrchlog.sleep_info.pilot_energy  = 0;
      hdrsrchlog.sleep_info.pilot_pn      = HDR_NO_PN_OFFSET;
    }
    else
    {
      hdrsrchlog.sleep_info.pilot_energy  = hdrsrchset.aset[0]->total_eng;
      hdrsrchlog.sleep_info.pilot_pn      = hdrsrchset.aset[0]->pn_offset;
        /* Record ASET PN offset and pilot strength when logging packet */
    }
#endif /* FEATURE_HDR_REVB */

    log_ptr = (LOG_HDR_ENH_SLEEP_INFO_C_type *)
              log_alloc( (log_code_type) LOG_HDR_ENH_SLEEP_INFO_C,
                         sizeof( LOG_HDR_ENH_SLEEP_INFO_C_type ) );

    if ( log_ptr )
    {
      REX_DISABLE_PREMPTION();

#ifdef FEATURE_MEMCPY_REMOVAL
      log_ptr->sleep_info = hdrsrchlog.sleep_info;
#else
      memcpy( (void *)( &log_ptr->sleep_info ),
              (void *)( &hdrsrchlog.sleep_info ),
              sizeof( hdrsrchlog_enh_sleep_info_struct_subtype ) );
#endif /* FEATURE_MEMCPY_REMOVAL */
        /* Copy log info into the log record */

      REX_ENABLE_PREMPTION();

      log_commit( (log_type *) log_ptr );
        /* submit the log to DIAG */

    } /* if log_ptr */
    
    hdrsrchlog.sleep_info.slot_count                  = 0xff;
    hdrsrchlog.sleep_info.cc_rate                     = 0xff;
    hdrsrchlog.sleep_info.cc_status                   = HDRLOG_CC_NULL;
    hdrsrchlog.sleep_info.ccmac_pkt_number            = 0;
    hdrsrchlog.sleep_info.ccmac_pkt_within_wakeup_cnt = 0;
      /* Reset the above fields after it has been logged            */

    if ( hdrsrchlog.triggered.sleep == TRUE )
    {
      hdrsrchlog.triggered.sleep  = FALSE;
        /* Turn off trigger after logging is completed                  */
    }

  }/* if triggered */

}/* hdrsrchlog_log_sleep_info() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_ON_DEMAND

DESCRIPTION  : Turns on the log_on_demand flag before logging the
               corresponding packet, and turns it off after logging finishes.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_log_on_demand
(
  hdrsrchlog_log_info_type log_info_type
    /* Indicates which HDRSRCH log packet interested    */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchlog.log_on_demand = TRUE;
    /* Turn on log_on_demand flag before logging    */

  switch ( log_info_type )
  {
    case HDRSRCHLOG_ACQ_INFO:

      hdrsrchlog_log_acq_info( );
        /* ACQ_INFO packet  */
      break;

    case HDRSRCHLOG_REACQ_INFO:

      hdrsrchlog_log_reacq_info( );
        /* REACQ_INFO packet  */
      break;

    case HDRSRCHLOG_SLEEP_INFO:

      hdrsrchlog_log_sleep_info( );
        /* SLEEP_INFO packet  */
      break;

    default:
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "Wrong log packet type: %d!",
                      log_info_type );
  } /* switch ( ... ) */

  hdrsrchlog.log_on_demand = FALSE;
    /* Turn off log_on_demand flag after logging    */

}/* hdrsrchlog_log_on_demand () */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_ACQ_INFO_INIT

DESCRIPTION  : Initialize ACQ INFO log packets

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

LOCAL void hdrsrchlog_acq_info_init( void )
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  hdrsrchlog.acq_info.attempt_count         = 0;
  hdrsrchlog.acq_info.attempt_count_success = 0;
  hdrsrchlog.acq_info.attempt_count_failure = 0;

  hdrsrchlog.acq_info.total_time_ms         = 0;
  hdrsrchlog.acq_info.total_failure_time_ms = 0;
    /* Initialize all cumulative counters to 0    */

  hdrsrchlog.acq_info.result                = HDRLOG_ACQ_NULL;
    /* No result at power-up  */

} /* hdrsrchlog_acq_info_init */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_REACQ_INFO_INIT

DESCRIPTION  : Initialize REACQ INFO log packets

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

LOCAL void hdrsrchlog_reacq_info_init( void )
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchlog.reacq_info.sleep_cycle_count   = 0;

  hdrsrchlog.reacq_info.failure_count       = 0;
    /* Initialize all cumulative counters to 0  */

  hdrsrchlog.reacq_info.result              = HDRLOG_REACQ_NULL;
    /* No result at power-up  */

} /* hdrsrchlog_reacq_info_init */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_SLEEP_INFO_INIT

DESCRIPTION  : Initialize SLEEP INFO log packets.

DEPENDENCIES : This is only called once on power up. Therefore, it assumes
               that all the new version 1 fields that are not directly set
               are zero inited.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

LOCAL void hdrsrchlog_sleep_info_init( void )
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  hdrsrchlog.sleep_info.version             = HDR_LOG_ENH_SLEEP_INFO_VER;
    /* Set packet version number. */

  hdrsrchlog.sleep_info.sleep_cycle_count   = 0;
  hdrsrchlog.sleep_info.cc_mer_count        = 0;
  hdrsrchlog.sleep_info.reacq_loss_count    = 0;
  hdrsrchlog.sleep_info.cmac_expiry_count   = 0;
  hdrsrchlog.sleep_info.iho_count           = 0;
  hdrsrchlog.sleep_info.ofs_count           = 0;
  hdrsrchlog.sleep_info.ofs_handoff_count   = 0;
  hdrsrchlog.sleep_info.chan_hash_count     = 0;
    /* Initialize all the cumulative counters to 0    */

  hdrsrchlog.sleep_info.cc_status           = HDRLOG_CC_NULL;
    /* No control channel status at power-up  */

  hdrsrchlog.sleep_info.ofs_status          = HDRLOG_OFS_NO_OFS;
    /* No OFS status at power-up              */

  hdrsrchlog.sleep_info.slot_count          = 0xff;
  hdrsrchlog.sleep_info.cc_rate             = 0xff;
    /* Info not valid at power up            */

  hdrsrchlog.sleep_info.cc_mer_denom        = 0;         
  hdrsrchlog.sleep_info.cur_warmup_time_us  = 0;
  hdrsrchlog.sleep_info.force_preamble_used = 0;

  hdrsrchlog.sleep_info.force_preamble_count         = 0;
  hdrsrchlog.sleep_info.force_preamble_failed_count  = 0;
  hdrsrchlog.sleep_info.total_time_out_of_service_ms = 0;
  hdrsrchlog.sleep_info.est_out_of_service_ccmer     = 0;
    /* More superfluous zero initing. */

  hdrsrchlog.oos_timer                        = 0;
  hdrsrchlog.oos_timer_active                 = FALSE;
    /* Initialize the out of service timers. */

} /* hdrsrchlog_sleep_info_init */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_INIT

DESCRIPTION  : 
  Initializes the global log packets. This function is called only once
  at power up, when HDRSRCH task is created.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_init( void )
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  hdrsrchlog.triggered.acq    = FALSE;
  hdrsrchlog.triggered.reacq  = FALSE;
  hdrsrchlog.triggered.sleep  = FALSE;
    /* All packets are not triggered at power-up    */

  hdrsrchlog.log_on_demand    = FALSE;
    /* No log_on_demand at power-up                 */

  hdrsrchlog_acq_info_init( );
  /* Initializes ACQ INFO log packet                */

  hdrsrchlog_reacq_info_init( );
  /* Initializes REACQ INFO log packet              */

  hdrsrchlog_sleep_info_init( );
  /* Initializes SLEEP INFO log packet              */

  hdrsrchlog_reset_tune_away_info();
    /* Initialize the tune away logging packet. */

  hdrsrchlog_reset_conn_state_stats(HDRSRCHLOG_ALL_STATS);
    /* Initialize the connected state statistics logging packet. */

  hdrsrchlog_slot_logging.rx_cbuff_valid = 0;
  hdrsrchlog_slot_logging.rx_tx_cbuff_valid = 0;
    /* Initialize the mask */

}/* hdrsrchlog_init */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_CMAC_EXPIRY_COUNT

DESCRIPTION  : This funtion will increment the cmac expiry count by 1 in 
               the sleep info log packet. This is an access function 
               provided to HDR RX.
               
DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

void hdrsrchlog_update_cmac_expiry_count( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  INC_SAT( hdrsrchlog.sleep_info.cmac_expiry_count );
    /* Increments CMAC expiry counter. It saturates at 0xffff   */

}/* hdrsrchlog_update_cmac_expiry_count */

/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_CC_INFO

DESCRIPTION  : This funtion will update the CCstart2CCDec, cc_rate and
               slot_count, force_preamble, cc_mer and other fields in the sleep
               info log packet. This is an access function provided to HDR RX.
               
DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchlog_update_cc_info
( 
  uint16  cc_ccdec, 
    /* start of CC to CC decoding, in half slots      */

  uint8   cc_rate,
    /* control channel rate */

  uint8   slot_count,
    /* Number of slots to decode the control channel  */

  boolean force_preamble_used,
    /* Whether force preamble detection was used in this wakeup. */

  boolean inc_cc_mer,
    /* Whether to count this wakeup as a message error. */

  uint16  packet_num,
    /* The CC packet number relative to the first CC packet. */

  boolean is_subsync
    /* The current packet is a subsynchronous packet. */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  if ( hdrsrchlog.triggered.sleep )
  {
    hdrsrchlog.sleep_info.cc_start_to_ccdec_hs  = cc_ccdec;
    hdrsrchlog.sleep_info.cc_rate               = cc_rate;
    hdrsrchlog.sleep_info.slot_count            = slot_count;
      /* Get control channel status info from HDRRX     */

    hdrsrchlog.sleep_info.force_preamble_used   = force_preamble_used;
      /* Save whether force preamble was used in this decode attempt. */

    /* Increment the following counts once per CC capsule. */
    if ( packet_num == 1 )
    {
      /* Increment fpd counts, not to be confused with intra-scc fpd counts.
         This can only be done on the 1st CC packet to maintain pre-intra-scc
         behavior. */

      if ( force_preamble_used )
      {
        INC_SAT( hdrsrchlog.sleep_info.force_preamble_count );
          /* Count the number of times a packet decode was attempted due to
             force preamble. */
  
        if ( inc_cc_mer )
        {
          INC_SAT( hdrsrchlog.sleep_info.force_preamble_failed_count );
            /* Accumulate the force preamble failed count. */
        }
      }
    }

    if ( hdrsrchlog.sleep_info.ccmac_pkt_within_wakeup_cnt == 0 )
    {
       hdrsrchlog.sleep_info.ccmac_pkt_number = packet_num;
      /* Capture the number of the first packet in the Control Channel 
         Capsule decoded in this wakeup.  If the wakeup is for the first
         packet of the SCC it is 1.  If it is the second packet of the SCC 
         it is 2, and so on.  If it is the subsync packet, it should always
         be 1. If the wakeup was for the quick paging capsule it is a 0. */
    }

    if ( inc_cc_mer == TRUE ) 
    {
      INC_SAT( hdrsrchlog.sleep_info.cc_mer_count );
        /* Accumulate the CC MER count. */

      INC_SAT( hdrsrchlog.sleep_info.cc_mer_denom );
        /* Accumulate the CC MER denominator. */

    }

    /* Keep track of the subsync stats. */
    if ( is_subsync == TRUE )
    {
      if ( inc_cc_mer == TRUE ) 
      {
        INC_SAT( hdrsrchlog.sleep_info.subsync_mer_cnt );
          /* Accumulate the subsync CC MER count. */

        INC_SAT( hdrsrchlog.sleep_info.subsync_mer_denom );
          /* Accumulate the subsync CC MER denominator. */
      }
      else if ( slot_count != HDRSRCHLOG_PKT_DECODE_FAILURE_SLOT_CNT )
      {
        INC_SAT( hdrsrchlog.sleep_info.subsync_mer_denom );
          /* Accumulate the subsync CC MER denominator. We don't to give the AT
             credit for the case the subsync missed preamble fires. In this
             case we will be told not to count the CC_MER.  But we don't want
             to count this as a good packet either. */
      }
    }

    /* Keep track of all continuation packet stats. */
    if ( packet_num > 1 )
    {
      /* If cc_status is null we know this is the first packet after reacq. */
      if ( hdrsrchlog.sleep_info.ccmac_pkt_within_wakeup_cnt == 0 )
      {
        /* Keep track of stats for packet decodes immediately following 
           intra-scc wakeups */
        if ( inc_cc_mer ) 
        {
          INC_SAT( hdrsrchlog.sleep_info.cont_cc_from_sleep_failure_cnt );
            /* Capture the numerator of the CC MER stat. */
        }
        INC_SAT( hdrsrchlog.sleep_info.cont_cc_from_sleep_cnt );
            /* Capture the denominator of the CC MER stat. */
      }
      else
      {
        /* This is not the first packet we've seen after a wakeup. Keep
           track of those stats. */
        if ( inc_cc_mer ) 
        {
          INC_SAT( hdrsrchlog.sleep_info.cont_cc_from_awake_failure_cnt );
            /* Capture the numerator of the CC MER stat. */
        }
        INC_SAT( hdrsrchlog.sleep_info.cont_cc_from_awake_cnt );
          /* Capture the denominator of the CC MER stat. */
      }

      /* Keep track of fpd stats for continuation packets. This should only
         happen for the first packet after wakeup, but we won't enforce that
         in this code should we decide to change that. */
      if ( force_preamble_used )
      {
        if ( inc_cc_mer ) 
        {
          INC_SAT( hdrsrchlog.sleep_info.intra_scc_fpd_failure_cnt );
            /* Capture the numerator of the CC MER stat. */
        }
        INC_SAT( hdrsrchlog.sleep_info.intra_scc_fpd_cnt );
            /* Capture the denominator of the CC MER stat. */
      }
    }
    else if ( packet_num == 1 )
    {
      /* Keep track of the stats of the first packet of the CC capsule. */
      if ( inc_cc_mer ) 
      {
        INC_SAT( hdrsrchlog.sleep_info.first_cc_failure_cnt );
          /* Capture the numerator of the CC MER stat. The denominator
             is the sleep cycle count. */
      }
    }

    INC_SAT( hdrsrchlog.sleep_info.ccmac_pkt_within_wakeup_cnt );
      /* Increment the number of ccmac packets we've seen this wakeup. */
  }
} /* hdrsrchlog_update_cc_info */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_RESET_TUNE_AWAY_INFO

DESCRIPTION  : This function resets all tune away related statistics logged
               to diag.
               
DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchlog_reset_tune_away_info( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  memset( (void *) &( hdrsrchlog.ta_info ), 0, 
          sizeof( hdrsrchlog.ta_info ) );
    /* Reset the tune away related logging information. */

  hdrsrchlog.ta_info.version = HDRSRCHLOG_TA_INFO_STATS_VER;
    /* Set the version number of the packet for future expandability. */

  hdrsrchlog.is_connected = FALSE;
    /* Start off assuming we are not in the connected state. */

} /* hdrsrchlog_reset_tune_away_info */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_UPDATE_SRCH_STATE_INFO

DESCRIPTION  : This function does the searcher state housekeeping for 
               the search logging APIs.

DEPENDENCIES : Can only be called from the SRCH context.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchlog_update_srch_state_info
(
  hdrsrch_state_enum_type prev_state,
    /* the previous search state before the current transition. */

  hdrsrch_state_enum_type next_state
    /* the next search state we are currently transitioning to. */
)
{
  time_type time_stamp;
    /* The current timestamp. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  (void) time_get ( time_stamp );
    /* Track the time we started an OFS. */

  REX_DISABLE_PREMPTION();

  switch ( next_state )
  {
    case HDRSRCH_OFS_TC_STATE:

      hdrsrchlog.ta_info.instance.performed_ofs = TRUE;
        /* We are performing an OFS. Track the fact we are doing that. */

      if ( prev_state == HDRSRCH_CONNECTED_STATE )
      {
        qw_equ( hdrsrchlog.ta_info.instance.hdrtc_tune_away_time_stamp, 
                time_stamp );
          /* Tune away time is the same as OFS start time. */
      }

      qw_equ( hdrsrchlog.ta_info.instance.hdrtc_ofs_time_stamp, 
              time_stamp );
      /* Track the time we started the OFS. */

      hdrsrchlog.ta_info.ta_stats_since_reset.hdrtc_tune_away_count++;
        /* Increment the number of tune aways since the last reset. */

      hdrsrchlog.ta_info.ta_stats_since_call.hdrtc_tune_away_count++;
        /* Increment the number of tune aways since the start of the 
           last call. */

      hdrsrchlog.ta_info.instance.hdrtc_tune_away_reason = 
          HDRSRCHLOG_UNKNOWN_TUNEAWAY;
        /* Who is taking the transciever away from HDR. There is no
           way to currently find out who. */

      break;

    case HDRSRCH_SUSPENDED_TC_STATE:

      /* Starting a tuneaway.  Record the relevant info */

      qw_equ( hdrsrchlog.ta_info.instance.hdrtc_tune_away_time_stamp, 
              time_stamp );
        /* Track the time we started the tune away. We could throw this
           away if it turns out we have a supervision timeout. */

      hdrsrchlog.ta_info.ta_stats_since_reset.hdrtc_tune_away_count++;
        /* Increment the number of tune aways since the last reset. */

      hdrsrchlog.ta_info.ta_stats_since_call.hdrtc_tune_away_count++;
        /* Increment the number of tune aways since the start of the 
           last call. */

        hdrsrchlog.ta_info.instance.hdrtc_tune_away_reason = 
          HDRSRCHLOG_UNKNOWN_TUNEAWAY;
        /* Who is taking the transciever away from HDR. There is no
           way to currently find out who. */

      break;

    case HDRSRCH_CONNECTED_STATE:


      /* This is a new call so clear out the call stats and save
         the time traffic started. */
      if ( !HDRSRCHLOG_IS_TC_TUNE_AWAY_STATE( prev_state ) )
      {
        memset((void *)&hdrsrchlog.ta_info.ta_stats_since_call, 0,
               sizeof(hdrsrchlog_tune_away_stats_struct));
          /* Clear out all TA stats retained on a per call basis. */

        qw_equ( hdrsrchlog.ta_info.instance.hdrtc_starttc_time_stamp, 
                time_stamp );
          /* Record when we started with traffic again. */
      }
      else 
      {
        /* We have transitioned to the connected state returning from a tune
           away.  Record the relevant info. */
      qw_equ( hdrsrchlog.ta_info.instance.hdrtc_tune_back_time_stamp, 
              time_stamp );
        /* Track the time we tuned back.  We may end up not using this
           information if this isn't a true tune away. */
      }

      hdrsrchlog.last_asp_changed_counter = 
                 hdrsrchset_get_asp_change_cnt( HDRSRCH_IDLE_SUBASET_IDX );
        /* Track what the current count is. */

      hdrsrchlog.is_connected = TRUE;
        /* Clearly we are connected.  But if ever we hit a state that isn't
           connected, traffic suspend, or ofs, we must have disconnected and
           we'll change this variable back to FALSE. */

      break;

    default:

      hdrsrchlog.is_connected = FALSE;
        /* If ever we hit a state that isn't connected, traffic suspend, 
           or ofs, we must have disconnected and we'll change this
           variable back to FALSE. */

      break;
  }

  REX_ENABLE_PREMPTION();

} /* hdrsrchlog_update_srch_state_info */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_TUNE_AWAY_INFO

DESCRIPTION  : This function sends out the current tune away info log 
               packet.
               
DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchlog_log_tune_away_info( void )
{
  LOG_HDR_TUNE_AWAY_INFO_C_type *log_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  log_ptr = (LOG_HDR_TUNE_AWAY_INFO_C_type *)
          log_alloc( (log_code_type) LOG_HDR_TUNE_AWAY_INFO_C,
                     sizeof( LOG_HDR_TUNE_AWAY_INFO_C_type ) );
    /* Get a log packet so that we can send out the tune away logging
       information.  This only works if the log mask is set properly. */
  
  if ( log_ptr )
  {
    REX_DISABLE_PREMPTION();

#ifdef FEATURE_MEMCPY_REMOVAL
    log_ptr->tune_away_info  = hdrsrchlog.ta_info;
#else
    memcpy( (void *)( &log_ptr->tune_away_info ),
            (void *)( &hdrsrchlog.ta_info ),
            sizeof( hdrsrchlog_tune_away_info_struct_subtype ) );
#endif /* FEATURE_MEMCPY_REMOVAL */
      /* Copy log info into the log record */

    REX_ENABLE_PREMPTION();
  
    log_commit( (log_type *) log_ptr );
      /* Commit the log packet to the diag port. */
  }
}

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_TUNE_AWAY_COMPLETION_INFO

DESCRIPTION  : This function should be called when HDR traffic actually
               resumes to send a tune_away info log packet out diag.  

               This function records the time stamp where the tune away
               completes and traffic is actually resumed and then sends
               out the log packet.
               
DEPENDENCIES : The hdrsrchlog_update_srch_state_info must have been
               called every time there was a searcher state change.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchlog_log_tune_away_completion_info( void )
{
  uint64 duration;
    /* Used to calculate the duration in a given state. */

  time_type time_stamp;
    /* Used to grab the current timestamp. */

  time_type init_value;
    /* Temporary variable to set time stamp */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  memset( init_value, 0, sizeof( time_type ) );
    /* Init the temporary variable to zero */

  (void) time_get( time_stamp ); 
    /* Track the time we finished the tune away. */

  REX_DISABLE_PREMPTION();

  qw_equ( hdrsrchlog.ta_info.instance.hdrtc_resume_time_stamp, 
          time_stamp );
    /* Track the time we resumed traffic. */

  hdrsrchlog.ta_info.ta_stats_since_reset.hdrtc_tune_back_count++;
    /* Update the tune back count since the last reset. */

  hdrsrchlog.ta_info.ta_stats_since_call.hdrtc_tune_back_count++;
    /* Update the tune back count since the start of the last call. */

  if (hdrsrchlog.ta_info.instance.performed_ofs == FALSE) 
  {
    duration = 
      hdrutil_calc_duration(
        hdrsrchlog.ta_info.instance.hdrtc_starttc_time_stamp,
        hdrsrchlog.ta_info.instance.hdrtc_tune_away_time_stamp );
        /* Calculate the HDR traffic time since the start of traffic last tune
         away. */
  }
  else
  {

    duration = 
      hdrutil_calc_duration(
        hdrsrchlog.ta_info.instance.hdrtc_starttc_time_stamp,
        hdrsrchlog.ta_info.instance.hdrtc_ofs_time_stamp );
        /* Calculate the HDR traffic time since the start of traffic last tune
           away. */
  }
  
  hdrsrchlog.ta_info.ta_stats_since_reset.cum_hdrtc_time += 
    duration;
    /* Update the total HDR traffic time since the last reset not including
       the last tune away. */

  hdrsrchlog.ta_info.ta_stats_since_call.cum_hdrtc_time +=
    duration;
    /* Update the total HDR traffic time since the last call not including
       the last tune away. */

  if ( hdrsrchlog.ta_info.instance.performed_ofs == FALSE)
  {
    duration =
      hdrutil_calc_duration(
        hdrsrchlog.ta_info.instance.hdrtc_tune_away_time_stamp,
        hdrsrchlog.ta_info.instance.hdrtc_resume_time_stamp );
        /* Calculate the cumulative HDR suspend time including RF tuning time*/
  }
  else
  {
    duration =
      hdrutil_calc_duration(
        hdrsrchlog.ta_info.instance.hdrtc_ofs_time_stamp,
        hdrsrchlog.ta_info.instance.hdrtc_resume_time_stamp );
        /* Calculate the cumulative HDR suspend time including RF tuning time*/
  }
  
  hdrsrchlog.ta_info.ta_stats_since_reset.cum_hdrtc_suspend_time += 
    duration;
    /* Update the total HDR suspend time since the last reset. */

  hdrsrchlog.ta_info.ta_stats_since_call.cum_hdrtc_suspend_time +=
    duration;
    /* Update the total HDR suspend time since the last call. */

  if ( hdrsrchlog.ta_info.instance.performed_ofs == FALSE)
  {
    duration =
      hdrutil_calc_duration(
        hdrsrchlog.ta_info.instance.hdrtc_tune_away_time_stamp,
        hdrsrchlog.ta_info.instance.hdrtc_tune_back_time_stamp );
        /* Calculate the cumulative HDR suspend time. */
  }
  else
  {
    duration =
      hdrutil_calc_duration(
        hdrsrchlog.ta_info.instance.hdrtc_ofs_time_stamp,
        hdrsrchlog.ta_info.instance.hdrtc_tune_back_time_stamp );
      /* Calculate the cumulative HDR suspend time. */
  }
  
  hdrsrchlog.ta_info.ta_stats_since_reset.cum_hdrtc_tune_away_time += 
    duration;
    /* Update the cumulative HDR tune away time since the last reset. */

  hdrsrchlog.ta_info.ta_stats_since_call.cum_hdrtc_tune_away_time +=
    duration;
    /* Update the cumulative HDR tune away time ince the last call. */

  hdrsrchlog_log_tune_away_info();
    /* Log the tune away information. */

  /* Now let's reinitialize the relevant state variables to 0 */
  qw_equ( hdrsrchlog.ta_info.instance.hdrtc_ofs_time_stamp, init_value );
  qw_equ( hdrsrchlog.ta_info.instance.hdrtc_tune_away_time_stamp, init_value );
  qw_equ( hdrsrchlog.ta_info.instance.hdrtc_tune_back_time_stamp, init_value );
  qw_equ( hdrsrchlog.ta_info.instance.hdrtc_resume_time_stamp, init_value );
  hdrsrchlog.ta_info.instance.performed_ofs = FALSE;

  qw_equ( hdrsrchlog.ta_info.instance.hdrtc_starttc_time_stamp, 
          time_stamp );
    /* Record when we started with traffic again. */

  REX_ENABLE_PREMPTION();

} /* hdrsrchlog_log_tune_away_completion_info */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_LOG_CONN_STATE_STATS

DESCRIPTION  : Logs the connected state stats. It gets a log record
               from the diag log buffer, fills it with the latest 
               connected state info and commits the log record to
               the diag for delivery.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchlog_log_conn_state_stats( void )
{
  LOG_HDR_CONN_STATE_SRCH_STATS_C_type *log_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  log_ptr = (LOG_HDR_CONN_STATE_SRCH_STATS_C_type *)
            log_alloc( (log_code_type) LOG_HDR_CONN_STATE_SRCH_STATS_C,
                       sizeof( LOG_HDR_CONN_STATE_SRCH_STATS_C_type ) );
    /* Actually log the packet if the log mask is set properly. */

  if ( log_ptr )
  {
    REX_DISABLE_PREMPTION();

    hdrsrchlog_set_serving_sect_handoff_count( 
                     hdrsrchset_get_asp_change_cnt( HDRSRCH_IDLE_SUBASET_IDX ), 
                                                                      FALSE );
      /* Get the latest number of Handoffs for logging purposes right
         before we send the logging packet out to make sure the count
         isn't stale. */

    /* TBD */
    hdrsrchlog.conn_state_stats.curr_rssi = hdrsrchutil_get_hdr_rssi( );
      /* Before sending the connected state stats, get the RSSI value 
         for the connected state statistics packet. */

    hdrsrchlog.conn_state_stats.curr_dli = hdrsrch_get_data_indicator();
      /* Before sending the connected state stats, get the data level
         value indicator for the connected state statistics packet. */

#ifdef FEATURE_MEMCPY_REMOVAL
    log_ptr->conn_state_srch_stats = hdrsrchlog.conn_state_stats;
#else
    memcpy( (void *)( &log_ptr->conn_state_srch_stats ),
            (void *)( &hdrsrchlog.conn_state_stats ),
            sizeof( hdrsrchlog_conn_state_stats_struct_subtype ) );
#endif /* FEATURE_MEMCPY_REMOVAL */
      /* Copy log info into the log record */

    REX_ENABLE_PREMPTION();

    log_commit( (log_type *) log_ptr );
      /* Actually log the packet to diag. */
  }
} /* hdrsrchlog_log_conn_state_stats */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_INC_CONN_STATE_STATS_COUNTER

DESCRIPTION  : This function increments the requested connected state 
               counter by the given amount.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchlog_inc_conn_state_stats_counter
( 
  hdrsrchlog_conn_state_stats_count_type counter,
    /* The counter to increment. */

  uint16                                 inc_count
    /* How much to increment the counter by. */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_DISABLE_PREMPTION();

  /* Only log these statistics if we are in a call. */
  if( hdrsrchlog.is_connected )
  {

    hdrsrchlog.conn_state_stats.since_reset_counts[counter] += inc_count;
      /* Increment the specified counter kept since the last AT reset. */

    hdrsrchlog.conn_state_stats.since_call_counts[counter] += inc_count;
      /* Increment the specified counter kept since the beginning of
         the last call. */

  }

  REX_ENABLE_PREMPTION();

} /* hdrsrchlog_inc_conn_state_stats_counter */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_INC_SEARCH_SET_STATS_COUNTER

DESCRIPTION  : This function tracks the number of pilots in each search
               set.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchlog_inc_search_set_stats_counter
( 
  hdrsrchset_struct_type *hdrsrchset_ptr
    /* Pointer to the search set information */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  /* Track the number of pilots in the aset, cset, and nset every triage
     cycle.  This info is logged in search's connected state stats 
     packet.*/
#ifdef FEATURE_HDR_REVB
  hdrsrchlog_inc_conn_state_stats_counter( HDRSRCHLOG_ASET_PILOT_COUNT, 
                                           hdrsrchset_total_aset_cnt());
#else
  hdrsrchlog_inc_conn_state_stats_counter( HDRSRCHLOG_ASET_PILOT_COUNT, 
                                           hdrsrchset_ptr->aset_cnt );
#endif /* FEATURE_HDR_REVB */
  hdrsrchlog_inc_conn_state_stats_counter( HDRSRCHLOG_CSET_PILOT_COUNT,
                                           hdrsrchset_ptr->cset_cnt );
  hdrsrchlog_inc_conn_state_stats_counter( HDRSRCHLOG_NSET_PILOT_COUNT,
                                           hdrsrchset_ptr->nset_cnt );

} /* hdrsrchlog_inc_search_set_stats_counter */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_RESET_CONN_STATE_STATS

DESCRIPTION  : This function resets the connected state stats as
               specified by the stats reset parameter.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchlog_reset_conn_state_stats
( 
  hdrsrchlog_reset_stats_type stats_reset 
    /* Which statistics to clear... all or just the ones related to the 
       last call. */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_DISABLE_PREMPTION();

  switch ( stats_reset )
  {

    case HDRSRCHLOG_ALL_STATS:
      memset( ( void* ) &hdrsrchlog.conn_state_stats, 0, 
              sizeof( hdrsrchlog_conn_state_stats_struct_subtype ) );
        /* Reset all of the statistics tracked for this log packet. */

      hdrsrchlog.conn_state_stats.version = HDRSRCHLOG_CONN_STATE_STATS_VER;
        /* Set the version number of the packet for future expandability. */
      
      break;

    case HDRSRCHLOG_LAST_CALL_STATS:

      memset( (void*) hdrsrchlog.conn_state_stats.since_call_counts, 0, 
              HDRSRCHLOG_NUM_CONN_STATE_STATS_COUNTS * sizeof ( uint32 ) );
        /* Reset only the statistics tracked since the start of the
           last call. */

      break;
  }

  REX_ENABLE_PREMPTION();

} /* hdrsrchlog_reset_conn_state_stats */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_SET_SERVING_SECT_HANDOFF_COUNT 

DESCRIPTION  : This function tracks the number of times the serving
               sector was changed via a handoff.  This function tracks
               past values of the counter to determine how many handoffs
               occurred since the last call.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchlog_set_serving_sect_handoff_count
(
  uint16  counter_value,
    /* Report the active serving pilot changed counter. */

  boolean counter_was_reset
    /* Report whether the counter was reset after reading this value. */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_DISABLE_PREMPTION();

  hdrsrchlog_inc_conn_state_stats_counter( 
    HDRSRCHLOG_SERVING_SECT_HANDOFF_COUNT,
    ( counter_value >= hdrsrchlog.last_asp_changed_counter ) ? 
    ( counter_value - hdrsrchlog.last_asp_changed_counter ) :
    ( counter_value + 1 + ( HDRSRCHLOG_MAX_HO_COUNT - 
                            hdrsrchlog.last_asp_changed_counter ) ) );
    /* We are connected so keep track of the number of handoffs since
       the last time this function was called.  Handle the wrap case
       as well. */

  if ( counter_was_reset )
  {
    hdrsrchlog.last_asp_changed_counter = 0;
      /* The counter was reset after this read so save 0 as the last
         read value. */
  }
  else
  {
    hdrsrchlog.last_asp_changed_counter = counter_value;
      /* Save the counter value for the next call. */
  }

  REX_ENABLE_PREMPTION();

} /* hdrsrchlog_set_serving_sect_handoff_count */

/*=========================================================================

FUNCTION HDRPSRCHLOG_GET_SUBPKT_PROTOCOL_DATA

DESCRIPTION
  This function gets value for the fields, which are related to HDRSRCH, in
  subpacket protocol data. 

DEPENDENCIES
  None

PARAMETERS
  subpkt_ptr - pointer to the subpacket to be set

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchlog_get_subpkt_protocol_data
(
  hdrdiag_hdr_protocol_data_subpkt_protocol_data_type *subpkt_ptr
)
{
  int io = 0;
    /* IO */
  int16 ecio = 0;
    /* Ec/Io */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  subpkt_ptr->srch_state = (uint8)hdrsrch_state_id;
    /* Searcher state. */

  hdrsrch_get_ecio_and_io(&ecio, &io);
  subpkt_ptr->ecio = (int16)ecio;
    /* Ec/Io */

#ifdef FEATURE_HDR_BCMCS
  subpkt_ptr->bcmcs_sleep_cycle = hdrsrchsleep_get_bcmcs_sleep_cycle();
    /* BCMCS sleep cycle. */
#endif

} /* hdrsrchlog_get_subpkt_protocol_data() */


/*=========================================================================

FUNCTION HDRPSRCHLOG_GET_PRED_DRC_RATE

DESCRIPTION
  This function gets the value of the predicted DRC rate from MDSP logging.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  pred DRC rate

SIDE EFFECTS
  None

=========================================================================*/
uint8 hdrsrchlog_get_pred_drc_rate( void )
{
  return ( hdrsrchlog.pred_drc_rate );
} /* hdrsrchlog_get_pred_drc_rate */

/*=========================================================================

FUNCTION HDRPSRCHLOG_GET_SERV_DRC_RATE

DESCRIPTION
  This function gets the value of the serving DRC rate from MDSP logging.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  serv DRC rate

SIDE EFFECTS
  None

=========================================================================*/
uint8 hdrsrchlog_get_serv_drc_rate( void )
{  

#ifdef FEATURE_HDR_REVB_SUPPORTED
      if(hdrscp_get_current_subtype(HDRHAI_RTC_MAC_PROTOCOL) == HDRSCP_SUBTYPE_4_RTCMAC)
      {
        return hdrsrchlog_mc_trans_drc_value;
      }
      else  
#endif /* FEATURE_HDR_REVB_SUPPORTED */
      {
      
      /*
        Bit2 :Bit0  :: DRC cover
        Bit7 :Bit3  :: Reserved
        Bit11:Bit8  :: DRC value //Need to retrive only this value
        Bit12       :: ACK/NACK
        Bit13       :: ACK channel enabled
        Bit15:Bit14 :: Reserved
      */
      /* Retrieve only the first subframe, slot 0 value at this instant of query */
        return(((hdrsrchlog_slot_logging.drc_arq[0].drc[0])&0x0F00)>>8);
      }  
} /* hdrsrchlog_get_pred_drc_rate */



#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
/*=========================================================================

FUNCTION HDRSRCHLOG_IS_DRC_BELOW_DIV_ON_THRES

DESCRIPTION
  This function check if the Pred DRC is below DIV ON threshold or
  above DIV OFF threshold. The most recent cross of either threshold
  determines the return value. 

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE  -  If drc has recently fallen below HDRSRCHARD_DIV_ON_DRC
  FALSE -  If drc has recently risen above HDRSRCHARD_DIV_OFF_DRC
 

SIDE EFFECTS
  None

=========================================================================*/
boolean hdrsrchlog_is_drc_below_div_on_thres( void )
{
  return ( hdrsrchlog.drc_below_div_on_thres );
} /* hdrsrchlog_is_drc_below_div_on_thres */

/*=========================================================================

FUNCTION HDRSRCHLOG_LOG_ARD_EVENT

DESCRIPTION
  This function logs ARD related events

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None
 

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchlog_log_ard_event
( 
  hdrsrchard_event_enum_type  event,
  stm_state_type              new_state,
  uint32                      time_left,
  boolean                     div,
  uint8                       drc
)
{
  LOG_HDR_ARD_EVENT_C_type *log_ptr;
    /* Log record for air link summary */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( log_status( LOG_HDR_ARD_EVENT_C ) )
  {
    log_ptr = (LOG_HDR_ARD_EVENT_C_type *) 
                log_alloc( LOG_HDR_ARD_EVENT_C, 
                           sizeof(LOG_HDR_ARD_EVENT_C_type) );

    if ( log_ptr != NULL )
    {
      log_ptr->version = HDRLOG_ARD_EVENT_LOG_VERSION;
      log_ptr->event = (hdrlog_ard_event_enum_type) event;
      log_ptr->new_state = (hdrlog_ard_state_enum_type) new_state;
      log_ptr->time_left = (uint8) time_left;
      log_ptr->div_requested = (boolean) div;
      log_ptr->drc = (uint8) drc;
      log_commit( (log_type*) log_ptr );
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "log_alloc failed for ARD_EVENT" );
    }
  }
}
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
#ifdef FEATURE_HDR_SELECTIVE_TXD
/*==========================================================================

FUNCTION  HDRSRCHLOG_GET_LAST_BEST_ASP_SINR

DESCRIPTION
  This function logs the TxD related event and its states
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  Time since last FL dec HSTR in ms  

SIDE EFFECTS
  None

==========================================================================*/
uint16 hdrsrchlog_get_last_basp_sinr( void )
{
  return hdrsrchlog.last_basp_sinr;
}
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_POWER_INFO_LOG_PACKET
/*=========================================================================

FUNCTION HDRSRCHLOG_PHYSICAL_LAYER_POWER_INFO

DESCRIPTION
  This function logs the physical layer power information.  On QXDM, this log
  packet is with similar log packets for all modem technologies.  It is not
  part of the HDR sequence of log packets.
  EVDO physical layer power info     0x1878

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchlog_log_physical_layer_power_info
(
  uint8                                 buffer_idx
    /* mDSP power logging buffer index (double buffered) */
)

{
  LOG_EVDO_PHYSICAL_LAYER_POWER_INFO_C_type *log_ptr;
    /* Log record for air link summary */

  int8                                      c_cnt;     /* carrier loop variable */
  int8                                      f_cnt;     /* finger loop variable */
  rfcom_device_enum_type                    rf_device; /* RF device used by HDR */
  int8                                      slot_idx;  /* slot index */
  int16                                     rf_txpwr;  /* as given by RF */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( log_status( LOG_EVDO_PHYSICAL_LAYER_POWER_INFO_C ) )
  {
    log_ptr = ( LOG_EVDO_PHYSICAL_LAYER_POWER_INFO_C_type * ) 
                log_alloc( LOG_EVDO_PHYSICAL_LAYER_POWER_INFO_C, 
                           sizeof( LOG_EVDO_PHYSICAL_LAYER_POWER_INFO_C_type) );

    if ( log_ptr != NULL )
    {
      memset( log_ptr->padding, 0, HDRLOG_PHYSICAL_LAYER_POWER_INFO_PAD_LEN );
        /* Reset padding area */

      log_ptr->log_pkt_version = HDRLOG_PHYSICAL_LAYER_POWER_INFO_VERSION;

      log_ptr->ustmr = VSTMR_XO_READ_FULL(); 
        /* Time when first sample in packet was collected */

      log_ptr->number_of_carriers = hdrsrchset.subaset_cnt; 
        /* [1,2,3] */
                                                                                 
      log_ptr->aset_size_carrier0 = 0;
      log_ptr->aset_size_carrier1 = 0;
      log_ptr->aset_size_carrier2 = 0;
        /* 0-6 */  
      if ( hdrsrchset.subaset[ 0 ].aset[ 0 ] != NULL ) 
      {
        log_ptr->aset_size_carrier0 = hdrsrchset.subaset[ 0 ].aset_cnt; 
      }

      if ( hdrsrchset.subaset[ 1 ].aset[ 0 ] != NULL ) 
      {
        log_ptr->aset_size_carrier1 = hdrsrchset.subaset[ 1 ].aset_cnt; 
      }

      if ( hdrsrchset.subaset[ 2 ].aset[ 0 ] != NULL ) 
      {
        log_ptr->aset_size_carrier2 = hdrsrchset.subaset[ 2 ].aset_cnt; 
      }

      log_ptr->number_of_fingers = 0;
        /* 0-8 */ 
      for ( c_cnt = 0; 
            ( ( c_cnt < hdrsrchfing.params.max_demod_carriers ) &&
              ( c_cnt < HDRSRCH_MAX_DEMOD_CARRIERS_CNT ) );
            c_cnt++ ) 
      {
        for ( f_cnt = 0; 
              ( ( f_cnt < hdrsrchfing.params.max_fings ) &&
                ( f_cnt < HDR_MAX_FINGS_CNT ) ); 
              f_cnt++ )
        {
          if ( hdrsrchfing.fings[ c_cnt ][ f_cnt ].pn_offset != HDR_NO_PN_OFFSET )
          {
            log_ptr->number_of_fingers++;
              /* Get number of assigned fingers */
          }
        } /* for f_cnt */
      } /* for c_cnt */
                                                                                      
      log_ptr->rxd = hdrsrchrfdiv_diversity_is_enabled( );
        /* TRUE if RxD is enabled. */

      rf_device = hdrsrchrf_get_rf_dev ( HDRSRCHRF_PRI_CHAIN ); 
        /* Get RF device ID */

      rf_txpwr = -128; 
        /* Initialize TX power to minimum value in .05dBm unit */

      /* Make sure TX power is valid */
      if ( ( hdrsrch_state_id != HDRSRCH_SLEEP_STATE ) && 
           ( hdrmdsp_check_dsp_clk() ) &&
           ( rf_device != RFM_INVALID_DEVICE ) &&
           ( hdrsrchrf_rf_chain_tune_done ( HDRSRCHRF_PRI_CHAIN ) ) )
      {
        rf_txpwr = rfm_hdr_get_tx_total_power( hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN),
                                               RFM_HDR_CARRIER_ID__0, 
                                               RFM_HDR_AGC_FORMAT__DBM256 );

        if ( rf_txpwr != ( int16 ) 0xFFFF )
          /* If error condition is encountered, report default lowest power of -128 */

        {
          /* Round off and convert to .05 dBm unit */
          rf_txpwr += ( rf_txpwr < 0 ? -64 : 64 ); 
          rf_txpwr /= 128;                
        }
      }

      log_ptr->txpwr = ( uint8 ) rf_txpwr;
        /* Convert to unit specified in the log packet template */

      log_ptr->dtx = 0;
      log_ptr->num_drx_slots = 0;
        /* Reset DTX and DRX slots before reading from FW. */

      /* Read number of slots DTX'ed during the last 26.66ms period.*/                     
      for ( slot_idx = 0; slot_idx < HDRFW_LOG_SLOTS_IN_FRAME; slot_idx++ )
      {
        if ( HDRLOG_GET_PWRLOG_DTX_SLOT ( buffer_idx, slot_idx ) > 0 )
        {
            log_ptr->dtx++;
              /* Get count of DTX slots */
        }
      } 

      /* Read number of slots DRX'ed during the last 26.66ms period.*/                     
      for ( slot_idx = 0; slot_idx < HDRFW_LOG_SLOTS_IN_FRAME; slot_idx++ )
      {
        if ( HDRLOG_GET_PWRLOG_DRX_SLOT ( buffer_idx, slot_idx ) > 0 )
        {
            log_ptr->num_drx_slots++;
              /* Get count of DRX slots */
        }
      } 
#if 0
      HDR_MSG_SRCH_5( MSG_LEGACY_FATAL, "ustmr=%ld,numcar=%d,car0=%d,car1=%d,car2=%d", 
                      log_ptr->ustmr,
                      log_ptr->number_of_carriers,
                      log_ptr->aset_size_carrier0,
                      log_ptr->aset_size_carrier1,
                      log_ptr->aset_size_carrier2 );

      HDR_MSG_SRCH_5( MSG_LEGACY_FATAL, "fings=%ld,rxd=%d,txpwr=%d,dtx=%d,drx=%d", 
                      log_ptr->number_of_fingers,
                      log_ptr->rxd,
                      log_ptr->txpwr,
                      log_ptr->dtx,
                      log_ptr->num_drx_slots );
#endif

      log_commit( ( log_type* ) log_ptr );
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "log_alloc failed for LOG_EVDO_PHYSICAL_LAYER_POWER_INFO_C" );
    }
  }
} /* end hdrsrchlog_log_physical_layer_power_info() */
#endif /* FEATURE_POWER_INFO_LOG_PACKET */

