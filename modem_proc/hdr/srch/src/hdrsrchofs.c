/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     H D R    S R C H    O F S    S T A T E

                       Search Off-Frequency Search Module

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2001 - 2021
                by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchofs.c#2 $ $DateTime: 2021/05/18 05:54:37 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ------------------------------------------------------- 
05/18/2021   vaa     Fix for compilation errors in MSIM disable build flavor
09/20/2018   vke     Changes to increase the Burst length to 2
03/01/2017   vke     Changes to call APS STATE END for deactivate case
02/28/2017   vke     APS feature compilation warning fix
02/07/2017   vke     FR36756 Changes to update TRM for APS state change
01/30/2018   vlc     Added fixed time delay to immediate gap start for L2D Idle 
                     meas. 
01/29/2018   vko     Fixed HDR sending RF status indication to incorrect LTE SUB ID
08/24/2017   rmv     Enable FW log pkts in L2DO measurement scenario 
12/12/2016   svu     Fixed KW issues
07/04/2016   wsh     Skip OFS if ASDiv switch is pending
05/27/2016   vko     QSH framework for events capturing
01/04/2015   vke     Changes to avoid sending lte_pilot_meas_abort_rsp_msg twice
12/29/2015   vke     Changes to send hdrfw_pilot_meas_stop_stream_msg to fw
                     only when PILOT_MEAS_CFG_REQ is sent to fw
12/14/2015   vke     Changes to send hdrfw_pilot_meas_stop_stream_msg to fw
                     when abort measurement is received from LTE
07/16/2015   tnp     Fixed compilation error associate with complete 
                     feature flag FEATURE_HDR_LTE_TO_EHRPD_IRAT
06/26/2015   vke     Removed handling HDRSRCH_BAND_GRANT_RCVD_CMD
06/09/2015   tnp     Update feature flag configuration
05/15/2015   vko/vke Added TABASCO to featurization
04/01/2015   mbs     Supported RF interface change
01/22/2015   arm     Cancelled band grant request if it is pending for OFS 
                     state.
11/19/2014   wsh     Added DR-DSDS support for ASDiv
10/20/2014   arm     Handled band grant pending from TRM.
10/17/2014   wsh     Fixed reporting wrong mode to TRM in 4a->3a transistion 
10/10/2014   vke     Fix to prevent the stray schedule-signal triggering 
                     hdrsrchofs_sched()
10/03/2014   arm     Handle TRM band granted in OFS state
10/01/2014   wsh     Added Full PN search for reacq and IRAT searches
09/22/2014   arm     DR-DSDS code cleanup
08/22/2014   vke     Cancel the searches in ASDiv processing only 
                     when Searches are active
07/30/2014   sat     Checking Band compatibility with RF device while 
                     determining OFS channels.
06/30/2014   arm     Changes to support DR-DSDS.
05/12/2014   vke     Klockworks fix
03/05/2014   wsh     Bug fixes for DSDS-ASDiv and DSDA-ASDiv support  
01/15/2014   wsh     Added DSDS ASDiv support
02/03/2014   mbs     Added change to support new bit-width for LTE OSTMR
10/07/2013   rmv     Replaced ASSERTs on external/global variables as per 
                     go/assert guidelines 
06/27/2013   smd     Changed min time needed to config FW/RF before gap start 
                     back to 10ms 
06/26/2013   wsh     Added Selective TxD feature
05/06/2013   mbs     Fixed compiler errors for BOLT build
04/30/2013   mbs     Reset rfTuneBackTime to 0 for L2DO conn meas
04/18/2013   vke     Fix for assert when 40th element is added to ofreq list
03/18/2012   vke     Fix for assert in hdrsrchofs_update_ofreq_list() 
                     when N List is added
04/02/2013   mbs     Changed min time needed to config FW/RF before gap start 
03/06/2013   vlc     Changed API to get RF parameters from LTE.
02/19/2013   vlc     Added new parameters for Pilot Meas Req message from LTE.
02/05/2013   vlc     Added call to rank ASP pilots. 
01/16/2013   vlc     Moved call to hdrsrchdrv_set_srch4_param() to make sure
                     the parameters are set in all search cases. 
12/19/2012   vlc     Added call to set SRCH4 parameters to default values in 
                     function hdrsrchofs_exit_ofs(). 
12/11/2012   mbs     Passed new parameters to RF and SRCH4 to support IRAT
12/04/2012   mbs     Fixed compilation errors due to removal of LTE on Triton
10/19/2012   smd     Told fw whether the standby to active transition is for wakeup.
07/26/2012   wsh     Fixed ARD going into INACTIVE after 4a->4c/4c->4a
06/13/2012   mbs     Increased energy threshold used for SON searching only
04/27/2012   wsh     Moving ARD logic to TC state
05/25/2012   mbs     Corrected skip optimization for anti-aliasing algorithm
05/10/2012   smd     Checked if there is enough time for gap meas after rf 
                     build script.
03/20/2012   mbs     Added SON search changes
03/07/2012   rkc     Fixed bug to allow ARD after traffic-OFS.
02/14/2011   wsh     Added interband OFS/HO threshold
02/03/2012   wsh     Added support for SVLTE+SHDR
02/01/2012   mbs     Modified burst length for connected mode measurement
01/11/2012   ljl     Added the parameter for search priority to hdrsrchdrv_enable_srch4().
01/10/2012   ljl     Allowed ARD only in traffic state.
11/29/2011   arm     Removed compiler warnings.
10/24/2011   wsh     Updated cancel FPD procedure 
10/19/2011   smd     Added debug code for L2DO IRAT
10/19/2011   wsh     Cancel FPD and deactivate dec when entering OFS
10/17/2011   rmg     Removed calls to obsolete clk regim APIs.
09/30/2011   smd     Removed 288 cx1 offset from RTC_OFFSET given to RF. 
09/16/2011   smd     Set rfTuneBackTime to 900 cx1 temporarily for L2DO conn meas. 
08/31/2011   smd     Added fixes for L2DO idle pilot meas.
08/25/2011   smd     Merged fixes of XO freq tracking for IRAT.
08/13/2011   kss     Init pilot_meas_stop_stream_req msg header before sending.
07/19/2011   smd     Added sending XO_CFG for IRAT meas.
                     Mailined FEATURE_IRAT_USE_MEAS_RF_API.
06/29/2011   smd     Updated the order of sending stop_stream and config_state 
                     for IRAT idle and conn measurements.
06/29/2011   smd     Added RxAGC report after gap. 
05/05/2011   smd     Featurized rxlm related code. 
                     Initialized fll channel for L2DO measurement.
05/05/2011   cnx     Fixed linker error.
05/02/2011   kss     Fixed compiler warning.
04/27/2011   rmg     Featurized out clock regime calls for MCPM.
04/27/2011   smd     IRAT L2DO updates for NikeL.
04/12/2011   smd     Used srch4 search structure.
03/22/2011   smd     Keep ant clock under firmware control in conn meas. 
03/08/2011   smd     Fixed a LTE STMR wrap around bug.
03/02/2011   smd     Removed rxAgcAccum in PilotMeasCfgReq.
02/15/2011   smd     Fixed bugs of PilotMeasAbortRsp messages. 
01/27/2011   smd     Fixed rotator error in PilotMeasCfgReq message.
                     Called clk regime API to set CDMA BBRX clock for gap.
01/18/2011   smd     Put fw in standby before sending PilotMeasRsp. 
01/12/2011   smd     Added debug messages for IRAT fw interface.
12/03/2010   smd     Many bug fixes for connected mode meas.
11/18/2010   smd     Added support for offline search for connected meas.
11/18/2010   ljl     Processed deactivate cmd at tc ofs state. 
11/16/2010   smd     Put ant clock under software control for LToDO idle meas.
11/11/2010   smd     Fixed the parameters of PilotMeasCfg req and resp.
11/05/2010   smd     Added LTE bw/band/channel and pass them to RF.
10/26/2010   smd     Considered LTE OSTMR wrap up when calculating gap start. 
10/19/2010   lyl     Added null pointer check after hdrsrchset_get_sect().
09/17/2010   smd     Used new rf APIs for connected mode measurement.
                     Updated clock management for connected mode meas. 
09/09/2010   smd     Removed FEATUER_IRAT_CONNECTED_MODE. 
08/16/2010   lyl     Supported FW power management with suspend/resume.
08/02/2010   smd     Updated code for connected mode pilot measurement.
07/25/2010   smd     Added clock gating support for IRAT pilot measurement.
07/21/2010   smd     Don't stop/start freq tracking if ofs channel is same as
                     aset channel.
06/21/2010   cnx     Fixed compiler warnings.
06/10/2010   smd     Set up srch rf structures and sample server when calling
                     RF MEAS APIs in idle mode.
06/08/2010   smd     Put DO FW into meas state in order for both LTE and DO 
                     FWs running.
06/01/2010   smd     Supported RF MEAS APIs in idle mode.
04/26/2010   lyl/smd Added reverse time transfer support.
04/15/2010   smd     Cleaned up calling clock API in ofs_meas_init.
04/08/2010   smd     Used RF meas API for IRAT pilot measurement.
04/01/2010   smd     Added code to config CDMA clocks after time transfer.
03/18/2010   smd     Forced RF chain unlock after IRAT pilot measurent.
03/18/2010   smd     Added fixes for IRAT pilot measurement.
03/18/2010   smd     Added channel info in pilot meas rsp for failure cases.
03/11/2010   ljl     Supported the fw timing state.
03/05/2010   smd     Fixed bugs in LTE pilot measurement feature.
02/26/2010   ljl     Fixed the compiler error. 
02/09/2010   smd     Added support for LTE to DO pilot measurement.
09/30/2009   smd     Fixed KW errors.
05/18/2009   ljl     Updated for firmware MSGR interface.
03/09/2009   smd     Added support for pilot measurement velcro solution.
02/02/2009   smd     Included NSET pilots when counting strong pilots for small
                     SCIs(SCI <= 6).
12/10/2008   smd     Replaced assert.h with amssassert.h
10/13/2008   lyl     Fixed lint errors.
09/23/2008   rkc     Used srch_list's res_ptr instead of resbuf.
09/17/2008   smd     Added idle handoff improvement.
08/29/2008   lyl     Supported primary chain ASET search.
07/25/2008   ljl     Changed hdrsrchset_check_if_ofreq_chan() to
                     hdrsrchset_is_ofreq_chan().
04/07/2008   ljl     Checked whether array index is out of boundary.
01/30/2008   jyw     Used demod0 for the OFS search.
01/30/2008   jyw     Included the demod assignment in the RF tuning interface.
10/17/2007   jyw     Added revB set management support.
09/21/2007   mt      Removed debug F3 message.
08/28/2007   mt      Fixed negative array index lint issue.
08/19/2007   grl     Resolved Lint High warnings.
08/16/2007   mt      Decreased the number of Traffic OFS repetitions to three.
07/26/2007   grl     Fixed system loss during HHO just prior to OFS.
07/20/2007   mt      Fixed 1x pilot reporting issue.
06/30/2007   mt      Perform OFS after REACQ failure.
06/20/2007   rkc     Changed hdrsrchofs_process_int_cmd() input to payload ptr
04/30/2007   mt      Enhanced OFS changes and RUP request support.
04/28/2007   mt      Added hdrsrchofs_init_ofs_timeline().
03/08/2007   grl     Fixed RCVT compiler warnings.
11/10/2006   mt      Reverted OFS tune back to normal AGC acquisition.
11/08/2006   ljl     Stop AFC when exiting from OFS.
10/27/2006   mt      Changed conditions to perform idle OFS.
10/17/2006   ljl     Added HDR SRCH AFC stm.
08/21/2006   grl     Interface change to hdrsrchrf_set_agc_acq_params
08/07/2006   mt      Changes to use new fast AGC acq during OFS tuning.
06/07/2006   ljl     Supported idle OFS with small SCI.
06/07/2006   ljl     Added SBHO optimization.
05/17/2006   jyw     Added the process of DEACIVATE_CMD in the idle OFS.
05/12/2006   sq      Set OFS status to NO_HO after OFS starts
04/17/2006   ljl     Added a parameter to hdrsrchafc_stop_fll_tracking().
08/11/2005   grl     Mainlined FEATURE_HDR_TCXOMGR.
07/29/2005   sq      Mark of-freq channel with a strong pilot as searched
07/27/2005   grl     Allow OFS to continue without TRK_LO control.
07/20/2005   sq      Always demoting of-freq CSET pilots 
07/18/2005   sq      Reset the Ec/Io of off-freq NSET pilots
07/15/2005   sq      Fixed issue during CSET pilots demotion
06/30/2005   sq      Modified OFS condition for traffic state
06/29/2005   sq      Validated OFS search dumps
06/26/2005   jyw     Added probes for HDR sleep time line profiling
06/21/2005   ds      Added check to exit OFS on empty search after OFS tune
05/31/2005   grl     Added support for TRM and TCXOMgr.
05/12/2005   ds      Changes to support round-robin OFS
03/31/2005   grl     Changed HDR's use of trk_lo to be GPS friendly
11/29/2004   grl     Added connected state stat logging support.
11/09/2004   sq      Reset search energy filter for OFS searches
11/03/2004   mpa     Removed dependency on std.h (merged with sys.h)
07/22/2004   ajn     Added common signals to state machine
12/02/2003   sq      Merged in code review comment
11/11/2003   sq      Supported new log packet ( SLEEP INFO )
10/13/2003   sq      Turned on FEATURE_HDR_TC_OFS
10/06/2003   aaj     Mainlined HDRSRCH_MULTIPATH_WIN_CENTER
09/16/2003   sq      Added support for FEATURE_HDR_TC_OFS
09/10/2003   mpa     Converted F3 messages to use MSG2.0
03/11/2003   ajn     Updated Copyright notice
11/04/2002   aaj     Remove FEATURE_HDRSRCH_PROFILE.
10/30/2002   aaj     Optimized checking for need to perform OFS
09/18/2002   aaj     Support for multipath window centering for HDR
09/10/2002   aaj     Demote OF cset pilot to nset if no OFS
09/05/2002   aaj     Changed conditions for performing OFS a little bit
08/27/2002   aaj     Correct F3 msg parameters about purging ofs list
04/22/2002   aaj     Perform OFS in idle only if certain conditions are true
02/06/2002   ajn     hdrerrno -> errno
01/17/2002   aaj     support for RF band to the hdr sector structure type
12/07/2001   aaj     Idle handoff threshold support for OFS/SLEEP
11/26/2001   aaj     Disable OFS until rxAGC problems are resolved
11/09/2001   aaj     Timetest support for srch profiling
11/07/2001   aaj     Remember last state from which OFS was entered
11/01/2001   aaj     Off-frequency handoff support for idle. Lots of changes
10/29/2001   aaj     Set OFS required to FALSE temporarily
10/23/2001   aaj     Integrated OFS with mainline HDR search. First checkin.
10/20/2001   aaj     Create & maintain off-freq list. Send-process OFS srches
10/18/2001   aaj     Implemented Off-Frequency Searching - first cut
10/17/2001   ajn     Created this file

==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"

#include "hdrsrchtypes.h"
#include "hdrsrchidlei.h"
#include "hdrsrchstate.h"
#include "hdrsrchdef.h"
#include "hdrsrchi.h"
#include "hdrsrchtask.h"
#include "hdrsrchutil.h"
#include "hdrsrchset.h"
#include "hdrsrchrf.h"
#include "hdrsrchlog.h"
#include "hdrsrchafc.h"
#include "hdrsrchafc.h"

#include "hdrsrchstates.h"

#include "hdrmdspmcr.h"

#include "hdrts.h"

#include "hdrdebug.h"
#include "amssassert.h"
#include "timetick.h"

#include "hdrsrchrfdiv.h"

#include "hdrsrchard.h"

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV
#include "hdrsrchlog.h"
#endif

#include "hdrmc_v.h"

#include "rfmeas_types.h"
#include "rfmeas_lte.h"

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
#include "lte_ml1_irat_ext_api.h"
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#include "hdr_log_qsh.h"
#include "hdrutil.h"

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

#define HDRSRCHOFS_MIN_ASET_ECIO_HALF_DB         10
  /* minimum -5dB Ec/Io to trigger OFS */

/* Thresholds for idle OFS condition evaluation */
#define HDRSRCHOFS_IDLE_ECIO_THRESHOLD1_HALF_DB  10
#define HDRSRCHOFS_IDLE_ECIO_THRESHOLD2_HALF_DB  20

#define HDRSRCHOFS_LESS_AGRESSIVE_IDLE_ECIO_THRESHOLD1_HALF_DB  14

#define HDRSRCHOFS_IDLE_OFS_INTERVAL_MS          5000
  /* The idle OFS interval */

#define HDRSRCHOFS_USE_FAST_AGC_IN_OFS_TUNE      FALSE  

#define HDRSRCHOFS_DO_SYSTEM_OFS_ONLY      \
        ( 1 << HDRSRCH_SYSTYPE_DO )

#define HDRSRCHOFS_1X_SYSTEM_OFS_ONLY      \
        ( 1 << HDRSRCH_SYSTYPE_1X )
      
#define HDRSRCHOFS_BOTH_DO_AND_1X_SYSTEM_OFS      \
        ( ( 1 << HDRSRCH_SYSTYPE_DO ) | ( 1 << HDRSRCH_SYSTYPE_1X ) )

#define HDRSRCHOFS_IDLE_CHAN_REPEAT_COUNT  1

#ifdef FEATURE_HDR_ENHANCED_OFS
#define HDRSRCHOFS_TC_CHAN_REPEAT_COUNT    3
#else
#define HDRSRCHOFS_TC_CHAN_REPEAT_COUNT    1
#endif /* FEATURE_HDR_ENHANCED_OFS */
  

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
#define HDRSRCHOFS_LTE_PILOT_MEAS_INTERLEAVE_CHAN_NUM 4
  /* Maximum number of interleaving channels in IRAT pilot measuremnt */

#define HDRSRCHOFS_MAX_AGC_PERIOD_FOR_RESUME_MODE     261734
   /* Maximum time from last Rx AGC calculation for resume mode. Since CDMA
      chip rate is 1.2288 MHZ, 213 ms has to be multiplied by 1228.8.
      261724 = 213 * 1228.8 */

#define HDRSRCHOFS_MEAS_TIME_TO_CONFIG_BEFORE_GAP    10   
   /* In unit of ms. Search configure RF and firmware within this time before
      gap start */
#define HDRSRCHOFS_FIXED_MEAS_TIME_DELAY_MS           1
   /* For Idle search with gap_start = IMMEDIATE, add 1 ms delay as a work
      around to resolve FW contention. */

#ifdef FEATURE_HDR_BOLT_MODEM
#define HDRSRCHOFS_MEAS_LTE_STMR_WRAP     0x40000000  
#define HDRSRCHOFS_MEAS_LTE_STMR_MASK     0x3FFFFFFF
   /* LTE STRM has 30 bits */ 
#else
#define HDRSRCHOFS_MEAS_LTE_STMR_WRAP     0x400000   
#define HDRSRCHOFS_MEAS_LTE_STMR_MASK     0x3FFFFF   
   /* LTE STRM has 22 bits */ 
#endif /* FEATURE_HDR_BOLT_MODEM */

#define HDRSRCHOFS_MEAS_FIRST_GAP_AGC_HS   5
   /* Number of half slots for RxAGC acquisition in first gap */
#define HDRSRCHOFS_MEAS_RESUME_AGC_HS      3
   /* Number of half slots for RxAGC acquisition in following gap */

#define HDRSRCHOFS_US_TO_CX1( time_in_us )     \
        ( ( ( time_in_us ) * 768 ) / 625 )
   /* Conver micro second to CDMA chip. time_in_us*1.2288= time_in_us*768/625 */

#define HDRSRCHOFS_MEAS_AGC_ACCUM_INIT    21828
  /* agc accum init value is -30dbm, convert it to agc_accum
     dbm = -64 + ( agc_accum*102/65536 )
  */
#define HDRSRCH_OFS_MEAS_WAIT_GAP_END_US  1000
  /* 1ms to wait for gap end */

#define HDR_SRCH_SON_MIN_PATH_ENG          81
  /*-8dB:min ecio for a path to be valid*/
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

#define HDRSRCHOFS_BAND_GRANT_TIMEOUT_MS    (0)
  /* Maximum time to wait for tune to channel */

/*-------------------------------------------------------------------------

                        OFF-FREQUENCY LIST

 List of frequencies compiled from the off-frequency neighbors is maintained
 and is used for deciding which frequency to search during a o-freq search
 cycle. This mechanism searches the least recently serached frequency in the
 ofreq search cycle.

---------------------------------------------------------------------------*/

/* Least Recently Searched frequencies */

typedef struct
{
  sys_channel_type            chan;
    /* other-frequency nset pilot's channel number. ffff for invalid chan */

  hdrsrch_system_enum_type    system;
    /* system type for this channel DO or 1x */

  uint8                       searched;
    /* other-frequency nset pilot search count */

  boolean                     keep;
    /* indicates if this channel is present in the latest nlist message */
}
hdrsrchofs_ofreq_list_struct_type;

/* O-freq specific parameters are grouped together */

typedef struct
{
  hdrsrchofs_ofreq_list_struct_type   list[HDR_MAX_NSET_SIZE];
    /* List of channels at different frequencies */

  uint8                               lcount;
    /* Count of entries in the ofreq list */

  uint8                               next_srch_idx;
    /* Search index into the NSET list for round robin OFS. Stores which 
       NSET pilot has to be considered in the o-freq search next. Wraps 
       around to zero when all NSET pilots have been scanned and o-freq 
       pilots on current channel have been searched in this OFS visit */

}
hdrsrchofs_ofreq_struct_type;

/* <EJECT> */
/*==========================================================================

                       HDR SEARCH OFS STATE DATA

As much OFS data is lumped together in one structure, so that it becomes
possible to dynamically allocate data memory during the OFS operation, and
discard the memory when no longer needed.

This will become important when RAM is based on SDRAM.

All variables are accessed as "hdrsrchofs.member_name".  When dynamic
allocation is implemented, a pointer to the structure is needed instead.
At this point, hdrsrchacq must become a dereferenced pointer, instead of
a variable.  ie

  #define hdrsrchofs  (*hdrsrchofs_struct_ptr)

==========================================================================*/

/*---------------------------------------------------------------------
   OFS State data structure
---------------------------------------------------------------------*/

typedef enum
{
  HDRSRCHOFS_OFS_SRCH,
  HDRSRCHOFS_RUP_SRCH,
  HDRSRCHOFS_PILOT_MEAS
} 
hdrsrchofs_ofs_reason_enum_type;

typedef struct
{
  boolean                           required;
    /* whether the autonomous OFS is required or not */

  boolean                           repeat_required;
    /* whether more repetitions for current OFS search is required or not */

#ifdef FEATURE_HDR_DDARF
  boolean                           idle_ofs_request;
    /* whether idle OFS has been requested due to conditions 
       such as Reacq failure, etc */
#endif /* FEATURE_HDR_DDARF */
    
  uint16                            repeat_count;
    /* how many times the current OFS channel has been searched */    

  uint16                            system_mask;
    /* mask for which systems to enable OFS */

  sys_channel_type                  last_chan;
    /* last searched OFS channel  */  
}
hdrsrchofs_ofs_struct_type;

typedef struct
{
  sys_channel_type                  chan;
    /* channel provided in RUP request */

  hdrsrch_system_enum_type          system;  
    /* system type for the channel */

  boolean                           pending;
    /* whether there is a pending RUP request */

  boolean                           report_pending;
    /* whether there is a pending RUP report */
    
  boolean                           repeat_required;
    /* whether more repetitions for current RUP search is required or not */

  uint16                            repeat_count;
    /* how many times the current RUP search has been repeated */
        
  uint16                            next_srch_index;
    /* index to RUP set to indicate where to start the next search */

  boolean                           set_completed;
    /* whether completed all sectors to be searched in current OFS visit */

  boolean                           last_set;
    /* whether the current RUP set is last one to be generated */

  boolean                           pilot_meas;
    /* whether this RUP search is for pending pilot measurement request */
}
hdrsrchofs_rup_struct_type;

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
typedef struct
{
   sys_channel_type                     channel;
     /* channel information - includes band and frequency */

   int16                                prev_agc_accum;
     /* agc accumulator value in previous gap */

   uint64                               time_of_prev_agc;
     /* timestamp when agc accum is calculated, in cdma system time */
}
hdrsrchofs_agc_accum_struct_type;

typedef struct
{
  hdrfw_pilot_meas_cfg_req_msg_t      pilot_meas_cfg_req;
    /* pilot meas cfg request message */

  hdrfw_pilot_meas_stop_stream_msg_t  pilot_meas_stop_stream_req;
    /* pilot meas stop stream message */
}
hdrsrchofs_fw_msg_struct_type;

typedef struct
{
  sys_channel_type                     channel;
    /* channel band and num */
  
  int32                                rot_error_accum;
   /* rotate accum value */

  uint32                               gap_start;
    /* the starting of GAP in unit of LTE OSTMR count 
       value 0xFFFFFFFF indicates immediate state */

  uint32                               gap_length;
    /* the duration of GAP in unit of LTE OSTMR count 
       value 0xFFFFFFFF indicates infinite length */

  boolean                              online_srch;
    /* Whether it is an online search */

  rfm_meas_common_param_type          *rf_params;
    /* RF parameters */

  hdrsrchrf_agc_acq_mode_enum_type     agc_acq_mode;
    /* Rx AGC acquisition mode of current pilot measurement */

  hdrsrchofs_agc_accum_struct_type      
                 agc_accum_list[HDRSRCHOFS_LTE_PILOT_MEAS_INTERLEAVE_CHAN_NUM]; 
    /* array to store agc accum of channels in last gap */

  hdrsrchofs_fw_msg_struct_type         fw_msg;

  boolean                               wait_for_gap;
    /* Whether search is waiting for gap */

  boolean                               abort_pilot_meas;
    /* Whether the current pilot measurement should be aborted*/

  hdrsrch_lte_pilot_meas_status_type    pilot_meas_status; 
    /* measurement status */

  uint16                                pilot_meas_req_to_gap_start_in_ms;
    /* Time from pilot meas is received to gap start point, in ms */

  timetick_type                         pilot_meas_req_recv_sclk;
    /* Time when pilot meas is received, in sclk */

#ifdef FEATURE_HDR_IRAT_DEBUG
  uint8                                 skip_gap;
#endif /* FEATURE_HDR_IRAT_DEBUG */

  boolean                               son_search;
  /* Is SON search enabled? */

  boolean                           is_meas_cfg_req_sent;
    /* Flag to store the status of whether PILOT_MEAS_CFG_REQ 
is sent to fw */
}
hdrsrchofs_lte_meas_struct_type;
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

typedef struct
{
  hdrsrchofs_ofreq_struct_type      ofreq;
    /* frequency list for searching off-frequency neighbors */

  hdrsrch_state_enum_type           last_state;
    /* previous state from which OFS was entered */

  boolean                           waiting_for_tcxo;
    /* Whether HDR is waiting for TCXO services to start OFS. */

  hdrsrch_state_enum_type           return_state;
    /* specific state to return to after idle OFS */
    
  timetick_type                     idle_ofs_time;
    /* The time of last idle ofs */

  timetick_type                     tc_ofs_time;
    /* The time of last traffic ofs */        

  hdrsrchofs_rup_struct_type        rup;
    /* Structure for RUP request based OFS searches */

  hdrsrchofs_ofs_struct_type        ofs;
    /* Structure for AT autonomus OFS searches */

  hdrsrchofs_ofs_reason_enum_type   ofs_reason;
    /* whether this is for an OFS search or RUP search */

  hdrsrch_system_enum_type          srch_system;
    /* system type for the current search */

  boolean                           manage_sets;
    /* whether to do set maintenance after the search */

  boolean                           reset_filters;
    /* whether to reset the OFS result filter*/

  uint16                            repeat_max;
    /* how many times to repeat the search on same channel */

  boolean                           is_channel_in_aset;
    /* whether the channel is same as aset channel */

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
  hdrsrchofs_lte_meas_struct_type   lte_meas;
    /* structure for LTE to DO pilot measurement */
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

#ifdef FEATURE_HDR_SELECTIVE_TXD
  boolean                               tune_done;
#endif
}
hdrsrchofs_struct_type;

LOCAL hdrsrchofs_struct_type      hdrsrchofs;
  /* all encompassing OFS strcture for the file */
  
/* structure used to store the Nlist which are to be added to the OFS search list */
typedef struct
{
  uint8 nlist2addLen; 
  /* Length of the Neighbhouring list to be added to OffFreqeuncy search */

  uint8 nlist2add[HDR_MAX_NSET_SIZE];
  /* Array of Index in Neighbhouring list to be added to OffFreqeuncy search */
}hdrsrch_nlist2add_struct_type;

/* EJECT */

/*==========================================================================

                     STATE DISPATCH TABLE

This section provides the dispatch functions for events that are received
by the task while in this state.

==========================================================================*/
LOCAL void            hdrsrchofs_init( hdrsrch_state_enum_type last_state );
LOCAL void            hdrsrchofs_done( hdrsrch_state_enum_type next_state );
LOCAL void            hdrsrchofs_tc_done( hdrsrch_state_enum_type next_state );
LOCAL errno_enum_type hdrsrchofs_cmd ( const hdrsrch_cmd_type * cmd );
LOCAL void            hdrsrchofs_dump( void );
LOCAL void            hdrsrchofs_tune(hdrsrchrf_tune_status_enum_type status);
LOCAL void            hdrsrchofs_exit_ofs( void );
LOCAL void            hdrsrchofs_process_int_cmd
                      (
                        hdrsrchsm_cmd_payload_type *
                      );

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
LOCAL void            hdrsrchofs_meas_init( hdrsrch_state_enum_type last_state );
LOCAL void            hdrsrchofs_meas_process_low_pri_rsp_ind
                      (
                        hdrsrchmsg_s_type         *msg
                      );
LOCAL void            hdrsrchofs_meas_dump( void );
LOCAL void            hdrsrchofs_meas_gap_process( void );
LOCAL void            hdrsrchofs_meas_start_search( void );
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

#ifdef FEATURE_HDR_SELECTIVE_TXD
LOCAL void hdrsrchofs_sched( void );
#endif

LOCAL hdrsrch_state_struct_type hdrsrchofs_idle_state =
{
  HDRSRCH_OFS_IDLE_STATE,
  hdrsrchofs_init,
  hdrsrchofs_done,
  hdrsrchofs_cmd,
  hdrsrchofs_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchofs_sched,
#else
  hdrsrch_default_sched,
#endif
  hdrsrch_default_lost_dump,
  hdrsrch_default_timeout,
  hdrsrch_default_gp_timer0,
  hdrsrch_default_gp_timer1,
  hdrsrch_default_gp_sig0,
  hdrsrch_default_gp_sig1,
  hdrsrch_default_time_valid,
  hdrsrch_default_rxchipx16,
  hdrsrch_default_pri_chain_event,
  hdrsrchofs_process_int_cmd,
  hdrsrch_default_process_low_pri_rsp_ind
};

LOCAL hdrsrch_state_struct_type hdrsrchofs_tc_state =
{
  HDRSRCH_OFS_TC_STATE,
  hdrsrchofs_init,
  hdrsrchofs_tc_done,
  hdrsrchofs_cmd,
  hdrsrchofs_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchofs_sched,
#else
  hdrsrch_default_sched,
#endif
  hdrsrch_default_lost_dump,
  hdrsrch_default_timeout,
  hdrsrch_default_gp_timer0,
  hdrsrch_default_gp_timer1,
  hdrsrch_default_gp_sig0,
  hdrsrch_default_gp_sig1,
  hdrsrch_default_time_valid,
  hdrsrch_default_rxchipx16,
  hdrsrch_default_pri_chain_event,
  hdrsrchofs_process_int_cmd,
  hdrsrch_default_process_low_pri_rsp_ind
};

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
LOCAL hdrsrch_state_struct_type hdrsrchofs_meas_state =
{
  HDRSRCH_OFS_MEAS_STATE,
  hdrsrchofs_meas_init,
  hdrsrch_default_done,
  hdrsrchofs_cmd,
  hdrsrchofs_meas_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchofs_sched,
#else
  hdrsrch_default_sched,
#endif
  hdrsrch_default_lost_dump,
  hdrsrch_default_timeout,
  hdrsrchofs_meas_gap_process,
  hdrsrch_default_gp_timer1,
  hdrsrch_default_gp_sig0,
  hdrsrch_default_gp_sig1,
  hdrsrch_default_time_valid,
  hdrsrch_default_rxchipx16,
  hdrsrch_default_pri_chain_event,
  hdrsrchofs_process_int_cmd,
  hdrsrchofs_meas_process_low_pri_rsp_ind
};

LOCAL void hdrsrchofs_meas_process_pilot_meas_abort_req( void );
LOCAL void hdrsrchofs_meas_process_meas_dump( void );
LOCAL void hdrsrchofs_meas_perform_son_search
(
  hdrsrch_lte_pilot_meas_rsp_msg_t      *srch_list,
  /* structure to store pilot information in */
  uint16                             srch_cnt
  /* search count in the list */
);
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

/* <EJECT> */
/*===========================================================================

FUNCTION    HDRSRCHOFS_OFREQ_CSET_COUNT

DESCRIPTION
  Returns number of off-frequency candidate set sector count

DEPENDENCIES

PARAMETERS

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS

===========================================================================*/

LOCAL int8 hdrsrchofs_ofreq_cset_count( void )
{
  int8   i, ofreq_cset_cnt = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( i = 0; i < hdrsrchset.cset_cnt; i++ )
  {
#ifdef FEATURE_HDR_REVB
    if ( hdrsrchset_is_ofreq_chan( hdrsrchset.nset[i]->chan ) == TRUE )
#else
    if ( !SYS_CHAN_CMP_EQUAL( hdrsrchset.cset[i]->chan,
                              hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
    {
      ofreq_cset_cnt++;

    }/* if (ofreq candidate) */

  }/* for i */

  return ofreq_cset_cnt;

}/* hdrsrchofs_ofreq_cset_count */


/* <EJECT> */
/*===========================================================================

FUNCTION    HDRSRCHOFS_OFREQ_NSET_COUNT

DESCRIPTION
  Returns number of off-frequency neighbor set sector count

DEPENDENCIES

PARAMETERS

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS

===========================================================================*/

LOCAL int8 hdrsrchofs_ofreq_nset_count( void )
{
  int8   i, ofreq_nset_cnt = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( i = 0; i < hdrsrchset.nset_cnt; i++ )
  {
#ifdef FEATURE_HDR_REVB
    if ( hdrsrchset_is_ofreq_chan( hdrsrchset.nset[i]->chan ) )
#else
    if ( !SYS_CHAN_CMP_EQUAL( hdrsrchset.nset[i]->chan,
                              hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
    {
      ofreq_nset_cnt++;

    }/* if (ofreq candidate) */

  }/* for i */

  return ofreq_nset_cnt;

}/* hdrsrchofs_ofreq_nset_count */


/* <EJECT> */
/*===========================================================================

FUNCTION    HDRSRCHOFS_DEMOTE_OFREQ_CSET_PILOTS

DESCRIPTION
  Demotes the off-freq CSET pilots to NSET and sets their Ec/Io to 0

DEPENDENCIES

PARAMETERS

RETURN VALUE
  none

SIDE EFFECTS
  could change nset/cset

===========================================================================*/

void hdrsrchofs_demote_ofreq_cset_pilots( void )
{
  int8                   i, cset_cnt;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cset_cnt = hdrsrchset.cset_cnt;

  for ( i = cset_cnt - 1; i >= 0; i-- )
  {
#ifdef FEATURE_HDR_REVB
    if ( hdrsrchset_is_ofreq_chan( hdrsrchset.cset[i]->chan ) == TRUE )
#else
    if ( !SYS_CHAN_CMP_EQUAL( hdrsrchset.cset[i]->chan,
                              hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
    {
      hdrsrchset.cset[i]->total_eng = 0;
        /* reset the total eng for the pilot */

      hdrsrchset.cset[i]->filt_length = 0;
        /* next time start off fresh with pilot filtering for this pilot */

      hdrsrchset.cset[i]->pri_chain_filt_eng = 0;
        /* reset the primary chain filtered eng for the pilot */

      hdrsrchset.cset[i]->pri_chain_eng_filt_length = 0;
        /* next time start off fresh with primary chain eng filtered for 
           this pilot */

      hdrsrchset.cset[i]->idho_filter.total_eng = 0;
        /* reset the total eng for the pilot */

      hdrsrchset.cset[i]->idho_filter.length = 0;
        /* next time start off fresh with pilot filtering for this pilot */
      
      hdrsrchset.cset[i]->sbho_filter.total_eng = 0;
        /* reset the total eng for the pilot */

      hdrsrchset.cset[i]->sbho_filter.length = 0;
        /* next time start off fresh with pilot filtering for this pilot */

      hdrsrchset.cset[i]->pil_filter.total_eng = 0;
        /* reset the total eng for the pilot, for OFS */

      hdrsrchset.cset[i]->pil_filter.length = 0;
        /* next time start off fresh with pilot filtering for this pilot */

      hdrsrchset_add_to_nset( hdrsrchset.cset[i] );
        /* add the pilot to NSET */

    }/* if (ofreq candidate) */

  }/* for i */

}/* hdrsrchofs_demote_ofreq_cset_pilots */


/* <EJECT> */
/*===========================================================================

FUNCTION    HDRSRCHOFS_RESET_NSET_PILOTS

DESCRIPTION
  Sets the Ec/Io of off-freq NSET pilots to 0

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchofs_reset_nset_pilots( void )
{
  int8                   i;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( i = 0; i < hdrsrchset.nset_cnt; i++ )
  {
#ifdef FEATURE_HDR_REVB
    if( hdrsrchset_is_ofreq_chan( hdrsrchset.nset[i]->chan ) == TRUE )
#else
    if ( !SYS_CHAN_CMP_EQUAL( hdrsrchset.nset[i]->chan,
                              hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
    {
      hdrsrchset.nset[i]->total_eng = 0;
        /* reset the total eng for the pilot */

      hdrsrchset.nset[i]->filt_length = 0;
        /* next time start off fresh with pilot filtering for this pilot */
      
      hdrsrchset.nset[i]->idho_filter.total_eng = 0;
        /* reset the total eng for the pilot */

      hdrsrchset.nset[i]->idho_filter.length = 0;
        /* next time start off fresh with pilot filtering for this pilot */
      
      hdrsrchset.nset[i]->sbho_filter.total_eng = 0;
        /* reset the total eng for the pilot */

      hdrsrchset.nset[i]->sbho_filter.length = 0;
        /* next time start off fresh with pilot filtering for this pilot */
      
      hdrsrchset.nset[i]->pil_filter.total_eng = 0;
        /* reset the total eng for the pilot, for OFS */

      hdrsrchset.nset[i]->pil_filter.length = 0;
        /* next time start off fresh with pilot filtering for this pilot */

    }/* if !(SYS_CHAN_CMP_EQUAL) */

  }/* for i */

}/* hdrsrchofs_reset_nset_pilots */


/* <EJECT> */
/*===========================================================================

FUNCTION    HDRSRCHOFS_GET_NEXT_OFREQ_CHAN

DESCRIPTION
  Gets the neighbor set frequency that should be searched next. It returns
  the least searched frequency

DEPENDENCIES

PARAMETERS

RETURN VALUE
  Least searched o-frequency to be searched next

SIDE EFFECTS

===========================================================================*/

LOCAL void hdrsrchofs_get_next_ofreq_chan( sys_channel_type *chan )
{
  int8                         i;
    /* index counter */

  hdrsrchofs_ofreq_struct_type *ofptr = &hdrsrchofs.ofreq;
    /* convenience pointer to o-freq list */

  boolean                      channel_found;
    /* whether a channel for OFS has been found or not */

  boolean                      band_supported;
    /* whether band is supported */
  
  rfm_device_enum_type         rf_device;
    /* Rf device type */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Obtain RF device id */
  rf_device =  hdrsrchrf_get_rf_dev( HDRSRCHRF_PRI_CHAIN );

  channel_found = FALSE;
    /* No suitable channel has been found yet */

  chan->chan_num = 0xffff;
    /* Initialize to no channel to search */

  hdrsrchofs.ofreq.next_srch_idx = 0;
    /* Reset the OFS search index since we are starting the OFS */

  if ( ofptr->lcount == 0 )
  {
    return;
    /* No other frequency neighbors in the list*/
  }

  /* Search for a unsearched o-freq channel with a desired system */
  for ( i = 0 ; (i < ofptr->lcount) && (i < HDR_MAX_NSET_SIZE) ; i++ )
  {
    if ( ( ofptr->list[i].searched == FALSE ) &&
         ( ( 1 << ofptr->list[i].system ) & hdrsrchofs.ofs.system_mask ) 
       )
    {
      band_supported = rfm_cdma_is_band_chan_supported(rf_device, ofptr->list[i].chan);
      if (band_supported == TRUE)
      {  
      *chan = ofptr->list[i].chan;
      hdrsrchofs.srch_system = ofptr->list[i].system;
      channel_found = TRUE;
      break;
    }
      else
      {
        HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                        "Band %d not supported on Device %d",
                        ofptr->list[i].chan.band, rf_device );
      }
    }
  }

  /* If no frequency found mark all frequencies as unsearched */
  if ( channel_found == FALSE )
  {
    for ( i = 0 ; i < ofptr->lcount ; i++ )
    {
      ofptr->list[i].searched = FALSE;
        /* set channel to not searched */
    }

    /* Search for an eligible channel again */
    for ( i = 0 ; i < ofptr->lcount ; i++ )
    {
      if ( ( 1 << ofptr->list[i].system ) & hdrsrchofs.ofs.system_mask )
      {
        band_supported = rfm_cdma_is_band_chan_supported(rf_device, ofptr->list[i].chan);
        if (band_supported == TRUE)
        {  
        *chan = ofptr->list[i].chan;
        hdrsrchofs.srch_system = ofptr->list[i].system;
        channel_found = TRUE;
        break;
      }
        else
        {
           HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                           "Band %d not supported on Device %d",
                           ofptr->list[i].chan.band, rf_device );
        }
      }
    }
  }

}/* hdrsrchofs_get_next_ofreq_chan */

/* <EJECT> */
/*===========================================================================

FUNCTION    HDRSRCHOFS_FIND_OFREQ_CHAN

DESCRIPTION
  Finds if a particular channel is present in o-freq list

DEPENDENCIES

PARAMETERS

RETURN VALUE
  Location (index) of the entry in the o-freq list

SIDE EFFECTS

===========================================================================*/

LOCAL int8 hdrsrchofs_find_ofreq_chan
(
  sys_channel_type           chan
    /* channel to be searched in the ofreq list */
)
{
  int       i;
    /* index into o-freq list */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( i = 0; 
        ( ( i < hdrsrchofs.ofreq.lcount ) &&
          ( i < HDR_MAX_NSET_SIZE ) );
        i++ )
  {
    if ( SYS_CHAN_CMP_EQUAL( hdrsrchofs.ofreq.list[i].chan, chan ) )
    {
      return i;
    }
  }

  return -1;

}/* hdrsrchofs_find_ofreq_chan */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHOFS_CLEAR_RUP_REQ

DESCRIPTION
  This function cancels any pending RUP request with OFS state.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchofs_clear_rup_req( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchofs.rup.pending = FALSE;
    /* reset flag that a RUP request is pending */

}  /* hdrsrchofs_clear_rup_req( ) */


/* EJECT */
/*===========================================================================

FUNCTION    HDRSRCHOFS_DETERMINE_OFS_CHAN

DESCRIPTION

  This function decides on which channel has to be searched for this 
  OFS tune away.

DEPENDENCIES

PARAMETERS

RETURN VALUE
  The channel to be used for OFS tune away.
  
SIDE EFFECTS

===========================================================================*/

LOCAL void hdrsrchofs_determine_ofs_chan( sys_channel_type *chan )
{
  uint16                       num_freq_searched;
    /* Number of frequencies searched in OFS list */

  uint16                       i;
    /* loop index */
    
  int16                        list_index;
    /* index into the other frequency list */

  boolean                      new_ofs_channel = TRUE;
    /* whether to search a new OFS channel or not */
    
  boolean                      band_supported;
    /* whether band is supported */
  
  rfm_device_enum_type         rf_device;
    /* Rf device type */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Obtain RF device id */
  rf_device =  hdrsrchrf_get_rf_dev( HDRSRCHRF_PRI_CHAIN );

  chan->chan_num = 0xffff;
    /* Initialize to no channel to search */

  hdrsrchofs.ofreq.next_srch_idx = 0;
    /* Reset the OFS index since we are starting this o-freq search */

  hdrsrchofs.reset_filters = FALSE;
    /* initialization: do not reset OFS result filter */

  if ( hdrsrch_state_id == HDRSRCH_OFS_IDLE_STATE )
  {
    hdrsrchofs.repeat_max = HDRSRCHOFS_IDLE_CHAN_REPEAT_COUNT;
  }
  else if ( hdrsrch_state_id == HDRSRCH_OFS_TC_STATE )
  {
    hdrsrchofs.repeat_max = HDRSRCHOFS_TC_CHAN_REPEAT_COUNT;
    
    if ( HDRSRCH_NV_OFS_TC_CHAN_REPEAT_COUNT != 0 )
    {
      hdrsrchofs.repeat_max = HDRSRCH_NV_OFS_TC_CHAN_REPEAT_COUNT;   
    }
  }
                   
  /* If pending RUP request use channel from RUP request 
     RUP is prioritized over OFS */
  if ( hdrsrchofs.rup.pending == TRUE ) 
  {
    if ( hdrsrchofs.rup.pilot_meas == FALSE )
    {
      /* this is not a RUP for pilot measurement. RUP for pilot measurement
         allows the same channel as active set */

      /* if request is for current channel then discard it */
#ifdef FEATURE_HDR_REVB
      if ( hdrsrchset_is_ofreq_chan( hdrsrchofs.rup.chan ) == FALSE )
        /* If the ofs channel is covered by the current ASET channels */
#else
      if ( SYS_CHAN_CMP_EQUAL( hdrsrchofs.rup.chan, hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
      {  
        chan->chan_num = 0xffff;
          /* Indicate that no channel to search */
        
        hdrsrchofs_clear_rup_req();
          /* clear the pending RUP request */        
       
        return;
      }
    }
  
    hdrsrchofs.ofs_reason = HDRSRCHOFS_RUP_SRCH;
      /* we are tuning away for RUP search */
        
    band_supported = rfm_cdma_is_band_chan_supported(rf_device, hdrsrchofs.rup.chan);

    if (band_supported)
    {
    chan->chan_num   = hdrsrchofs.rup.chan.chan_num ;
    chan->band       = hdrsrchofs.rup.chan.band ;
    }

    hdrsrchofs.srch_system = hdrsrchofs.rup.system;
     
    if ( hdrsrchofs.rup.repeat_count == 0 )
    {
      hdrsrchofs.reset_filters = TRUE;
      /* reset OFS result filter */
      
      if ( hdrsrchofs.rup.pilot_meas == FALSE )
      {
        /* this is ofs rup */

        hdrsrchofs.rup.last_set = hdrsrchutil_generate_rup_set( hdrsrchofs.srch_system );
          /* generate the RUP sectors to be searched */

        HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "**RUP Search** [Sectors: %d, Repetition= %d of %d ]",
                    hdrsrchset.rupset_cnt,
                    ( hdrsrchofs.rup.repeat_count + 1 ), 
                    hdrsrchofs.repeat_max );
      }
      else
      {
        /* this is ofs pilot measurement */

        hdrsrchofs.rup.last_set = 
             hdrsrchutil_generate_pilot_meas_search_set( hdrsrchofs.srch_system );
          /* generate the pilot measurement sectors to be searched */

        HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "**PILOT MEAS Search** [Sectors: %d, Repetition= %d of %d ]",
                    hdrsrchset.rupset_cnt,
                    ( hdrsrchofs.rup.repeat_count + 1 ), 
                    hdrsrchofs.repeat_max );
      }
    }
        
    hdrsrchofs.rup.next_srch_index = 0;
      /* reset index into RUP set  */
      
    hdrsrchofs.rup.set_completed = FALSE;
      /* reset flag indicating that all sectors in current RUP set 
         has been searched */


  }
  else if ( ( hdrsrchofs.ofs.required == TRUE ) || 
            ( hdrsrchofs.ofs.repeat_required == TRUE ) )
  {
    hdrsrchofs.ofs_reason = HDRSRCHOFS_OFS_SRCH;
      /* we are tuning away for OFS search */
      
    if ( hdrsrchofs.ofs.repeat_required == TRUE )
    {
      list_index = hdrsrchofs_find_ofreq_chan( hdrsrchofs.ofs.last_chan );
        /* see if this channel is still in neighbor set */
                               
      band_supported = rfm_cdma_is_band_chan_supported(rf_device, 
                                                      hdrsrchofs.ofs.last_chan);

      if ( (list_index != -1) && (band_supported == TRUE) )
      {
        new_ofs_channel = FALSE;
          /* repeat search on old channel */
        
        *chan = hdrsrchofs.ofs.last_chan;
          /* repeat measurement on last channel */
          
        hdrsrchofs.srch_system =  hdrsrchofs.ofreq.list[list_index].system;
          /* set system type for search */
      }
      else      
      {
        new_ofs_channel = TRUE;
          /* could not find old channel in OFS list 
             so get a new OFS channel to search */
      }
    }

    if ( new_ofs_channel == TRUE )
    {
      hdrsrchofs_get_next_ofreq_chan( chan );
        /* get the next suitable channel from ofreq list */
                
      hdrsrchofs.ofs.repeat_count = 0;
        /* reset OFS repeat count */
        
      hdrsrchofs.ofs.repeat_required = FALSE;
        /* reset OFS search repetition required flag */
              
      hdrsrchofs.reset_filters = TRUE;
        /* reset OFS result filter */
    }

    /* Count total number of searched frequencies. For Debug. Remove later */
    num_freq_searched = 0;

    for ( i = 0 ; i < hdrsrchofs.ofreq.lcount ; i++ )
    {
      if ( hdrsrchofs.ofreq.list[i].searched )
      {
        num_freq_searched++;
      }
    }

    HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "**Off-Freq Search** [Tot Freq= %d, Srched= %d Repetition= %d of %d ]",
                    hdrsrchofs.ofreq.lcount, num_freq_searched,
                    ( hdrsrchofs.ofs.repeat_count + 1 ) , hdrsrchofs.repeat_max );
  }
  else
  {
    chan->chan_num = 0xffff;
      /* Indicate that no channel to search */

    hdrsrchofs.ofs.repeat_count = 0;
      /* Initialize the number of times this channel has been searched */
  }

}/* hdrsrchofs_determine_ofs_chan */

/* <EJECT> */
/*===========================================================================

FUNCTION    HDRSRCHOFS_PURGE_OFREQ_LIST

DESCRIPTION
  Removes the frequencies from the list which are not present in the new
  nlist.

DEPENDENCIES
  Should be called after the nset is updated with the new nlist

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

LOCAL void hdrsrchofs_purge_ofreq_list( void )
{
  int     i, orig_count;
    /* ofreq array index */

  hdrsrchofs_ofreq_struct_type *ofptr = &hdrsrchofs.ofreq;
    /* convenience pointer to o-freq list */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  orig_count = ofptr->lcount;
    /* remember original count for debugging purposes */

  /* Following piece of code tries to remove the keep=FALSE entries from the
     frequency list. It replaces the removed entries with the frequency
     entries from the end of the list so that there are no holes in the list*/

  i = 0;
  while ( i < ofptr->lcount )
  {
    if ( ofptr->list[i].keep == FALSE )
    {
      ofptr->list[i]  = ofptr->list[ofptr->lcount-1];
      /* remove this frequency entry. Push blanks at the end of array */

      ofptr->list[ofptr->lcount-1].chan.chan_num = 0xffff;
      /* Mark last entry as invalid. We don't *have* to do this but lets
         just do it for ease of debugging in the lab. TBD: remove later */

      ofptr->lcount--;
    }
    else
    {
      i++;
    }

  }/* while */

  if ( orig_count - ofptr->lcount )
  {
    /* Logging the number of list OffFreq list purged */
    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "OFS: Purged %d frequencies out of %d",
                    ( orig_count - ofptr->lcount ),
                    orig_count );
  }

}/* hdrsrchofs_purge_ofreq_list */

/* <EJECT> */
/*===========================================================================

FUNCTION    HDRSRCHOFS_GATHER_OFREQS

DESCRIPTION
  Collects off-frequencies in the given set

DEPENDENCIES

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

LOCAL void hdrsrchofs_gather_ofreqs
(
  hdrsrch_sect_struct_type       **set_ptr,
    /* pointer to the set to be searched for o-freq */

  const uint8                     num_sects,
    /* Number of sectors in the given set */
	
  hdrsrch_nlist2add_struct_type *nlist2add
    /* Pointer to Structure to store the Nlist Index to be added to OffFreq List */
)
{
  int8                         chan, n;

  hdrsrchofs_ofreq_struct_type *ofptr = &hdrsrchofs.ofreq;
    /* convenience pointer to o-freq list */
	
  /* Initialize the numder of Nlist2add length */
  nlist2add->nlist2addLen = 0;

#ifdef HDRSRCH_1X_SEARCH_TEST

  hdrsrch_sect_struct_type        *sector_1x;
    /* sector to hold 1x neighbor */

  sys_channel_type                chan_1x;
    /* local channel structure for 1x */

#endif /* HDRSRCH_1X_SEARCH_TEST */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( n = 0; n < num_sects; n++ )
  {
#ifdef FEATURE_HDR_REVB
    if ( hdrsrchset_is_ofreq_chan( set_ptr[n]->chan ) )
      /* If the channel is not one of the aset channels, then it is an ofs 
         neighbor set sector */
#else
    if ( !SYS_CHAN_CMP_EQUAL( set_ptr[n]->chan, hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
    {
      chan = hdrsrchofs_find_ofreq_chan( set_ptr[n]->chan );
        /* find if this entry is already present in the ofreq list */

      if ( chan == -1 )
      {
        /* frequency not found in ofreq list, so add it to the list. Note that
           all the additions are at the end of the list. All the null locations
           are pushed at the end */
   
        ASSERT( nlist2add->nlist2addLen < HDR_MAX_NSET_SIZE );
          /* Sanity check for List overflow */

        nlist2add->nlist2add[nlist2add->nlist2addLen] = n;
        nlist2add->nlist2addLen++;
          /* Storing the Nlist index to temp list */
      }
      else
      {
        ofptr->list[chan].keep = TRUE;
          /* entry found so mark it as a keep */
      }

    }/* if (ofreq candidate) */

  }/* for n */

#ifdef HDRSRCH_1X_SEARCH_TEST

  /* Add a 1x pilot to search */
  chan_1x.chan_num = HDRSRCH_NV_OFS_1X_FREQUENCY;
  chan_1x.band     = HDRSRCH_NV_OFS_1X_BAND;
  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "OFS: Added 1X pilot Band= %d, Freq= %d, PN= %d.",
                  HDRSRCH_NV_OFS_1X_BAND,
                  HDRSRCH_NV_OFS_1X_FREQUENCY,
                  HDRSRCH_NV_OFS_1X_PN );

  chan = hdrsrchofs_find_ofreq_chan( chan_1x );
    /* see whether ix sector has already present */

  if ( ( chan == - 1 ) && ( chan_1x.chan_num != 0 ) )
  {
    /* create a bogus 1x sector */
    sector_1x = hdrsrchset_get_sect( HDRSRCH_NV_OFS_1X_PN, 
                                     chan_1x );

    if ( sector_1x != NULL )
    {
      sector_1x->sys_type = HDRSRCH_SYSTYPE_1X;
                                      
      hdrsrchset_add_to_nset( sector_1x );
    }

    /* Add bogus 1x sector to OFS list */
    ofptr->list[ofptr->lcount].chan     = chan_1x;
    ofptr->list[ofptr->lcount].searched = FALSE;
    ofptr->list[ofptr->lcount].keep     = TRUE;
    ofptr->list[ofptr->lcount].system   = HDRSRCH_SYSTYPE_1X;

    /* increase OFS frequency count */
    ofptr->lcount++;
  }
#endif /* HDRSRCH_1X_SEARCH_TEST */

}/* hdrsrchofs_gather_ofreqs */


/* <EJECT> */
/*===========================================================================

FUNCTION    HDRSRCHOFS_UPDATE_OFREQ_LIST

DESCRIPTION
  Updates list of o-freq neighbor pilots when the new nlist has been received
  from the AT.

DEPENDENCIES
  Should be called after the nset is updated with the new nlist

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

LOCAL void hdrsrchofs_update_ofreq_list ( void )
{
  int    i;
  
  /* Structure to store the list of Nlist to be added to the Off Freq search */ 
  static hdrsrch_nlist2add_struct_type temp_nlist2add;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* initialize all the entries to be removed, until decided otherwise */
  for ( i = 0; i < hdrsrchofs.ofreq.lcount; i++ )
  {
    hdrsrchofs.ofreq.list[i].keep = FALSE;
  }

  HDR_ASSERT ( hdrsrchset.nset_cnt <= HDR_MAX_NSET_SIZE );
    /* KW fix : Check to limit the nset_cnt to max  */

  hdrsrchofs_gather_ofreqs( hdrsrchset.nset, hdrsrchset.nset_cnt, &temp_nlist2add);
    /* Look for off-frequency neighbor sectors and add o-freq to the list */

  //TBD: Change to hdr_sects[] table search later.

  hdrsrchofs_purge_ofreq_list();
    /* purge o-freq list from any frequencies that were marked as "not keep" */
  
  /* Add the stored Nlist to OffFreq list */
  for ( i = 0; i < temp_nlist2add.nlist2addLen; i++ )
  {
    /* check for list overflow */
    ASSERT( hdrsrchofs.ofreq.lcount < HDR_MAX_NSET_SIZE );

    /* Get the stored new Nlist and store in OffFreq list */
    /* add Nlist to the end of OffFreq list */
    hdrsrchofs.ofreq.list[hdrsrchofs.ofreq.lcount].chan 
        = hdrsrchset.nset[temp_nlist2add.nlist2add[i]]->chan;
    
    hdrsrchofs.ofreq.list[hdrsrchofs.ofreq.lcount].searched = FALSE;
    hdrsrchofs.ofreq.list[hdrsrchofs.ofreq.lcount].keep = TRUE;
    hdrsrchofs.ofreq.list[hdrsrchofs.ofreq.lcount].system   = HDRSRCH_SYSTYPE_DO;
    hdrsrchofs.ofreq.lcount++;
  }

  /* Logging the number of new list OffFreq added */
  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "OFS: Newly added Nlist %d and Current NList %d",
                  temp_nlist2add.nlist2addLen,
                  hdrsrchofs.ofreq.lcount );

} /* hdrsrchofs_update_ofreq_list( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION    HDRSRCHOFS_NOTIFY_NSET_UPDATE

DESCRIPTION
  This function notifies OFS state that NSET has been updated. 
  So that OFS state can update the list of o-freq neighbor pilots 
  when the new nlist has been received from the AT.

DEPENDENCIES
  Should be called after the nset is updated with the new nlist

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrsrchofs_notify_nset_update( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchofs_update_ofreq_list();
    /* update the other frequency list as it has changed */

} /* hdrsrchofs_notify_nset_update() */


/* <EJECT> */
/*===========================================================================

FUNCTION    HDRSRCHOFS_IDLE_OFS_REQUIRED

DESCRIPTION
  Returns whether or not ofreq search is required at this moment

  It may not be necessary to perform a neighbor search on a different
  frequency if our active set very strong or we have lots of good neighbors
  and candidate sectors on the current frequency.


  Check following conditions to decide if OFS is needed

  1. More than 5 seconds has elapsed since last OFS.
  3. All active and candidate set pilots are below -5 dB
  4. Second best pilot in active plus candidate set is below -10 dB
  5. Different frequency candidate or neighbour count is non-zero.

  OFS required = ( 1 && 2 && 3 && 4 )

DEPENDENCIES

PARAMETERS

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS

===========================================================================*/

boolean hdrsrchofs_idle_ofs_reqd( void )
{
  boolean           ofs_required;
    /* flag to indicate OFS required or not */

  timetick_type     now;
    /* Current time */

  uint16            energy_thresh1;
    /* first OFS threshold for energy comparison, in linear scale */

  uint16            energy_thresh2;
    /* second OFS threshold for energy comparison, in linear scale */

  uint16            count1 = 0;
    /* number of ASET and CSET pilots above first OFS threshold */

  uint16            count2 = 0;
    /* number of ASET and CSET pilots above second OFS threshold */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ofs_required = TRUE;
    /* Initialize OFS required to TRUE */

  now = timetick_get_ms();
    /* Get the current time */

  /* Check whether time interval requirement is satisfied */
  if ( (uint32) ( (int32) now - (int32) hdrsrchofs.idle_ofs_time ) > 
       HDRSRCHOFS_IDLE_OFS_INTERVAL_MS )
  {
    hdrsrchofs_demote_ofreq_cset_pilots();
      /* Demote the obsolete ofs CSET pilots  */
  }
  else
  {
    ofs_required = FALSE;
      /* Set OFS required to FALSE */
  }

  if ( ofs_required == TRUE )
  {
    /* Convert the threshold from DB scale to linear scale, because the 
       energy stored for each sector is in linear scale */
    if ( hdrmc_feature_is_enabled(HDRMC_FEATURE_KDDI_OFS_HO_THRESHOLD) )
    {
      energy_thresh1 = hdrsrchset_ecio_to_eng( 
        HDRSRCHOFS_LESS_AGRESSIVE_IDLE_ECIO_THRESHOLD1_HALF_DB );
    }
    else
    {
      energy_thresh1 = hdrsrchset_ecio_to_eng( HDRSRCHOFS_IDLE_ECIO_THRESHOLD1_HALF_DB );
    }

    energy_thresh2 = hdrsrchset_ecio_to_eng( HDRSRCHOFS_IDLE_ECIO_THRESHOLD2_HALF_DB );

    /* Check the energy threshold requirements: 
       if there is at least one pilot above first threshold and
       at least two pilots above second threshold, then do not 
       perform OFS */
    count1 = hdrsrchutil_count_strong_pilots( energy_thresh1 );
    count2 = hdrsrchutil_count_strong_pilots( energy_thresh2 );

    if ( ( count1 == 0 ) && ( count2 <= 1 ) )
    {
      ofs_required = TRUE;
        /* set OFS required to TRUE */
    }
    else
    {
      ofs_required = FALSE;
        /* set OFS required to FALSE */
    } 
  }

#ifdef FEATURE_HDR_DDARF

  /* If idle OFS is not necessary w.r.t the signal conditions,
     check whether it has been requested due to some other event 
     such as Reacq failure. */
  if ( ofs_required == FALSE )
  {
    if ( hdrsrchofs.ofs.idle_ofs_request == TRUE )
    {
      ofs_required = TRUE;
        /* set OFS required to TRUE */    
    }
  }
  
  hdrsrchofs.ofs.idle_ofs_request = FALSE;
    /* clear flag indicating that idle OFS has been requested */
    
#endif /* FEATURE_HDR_DDARF */
  
  /* Check if there are any off-frequency candidates or neighbors
     around here. updating freq list is pretty intensive job so
     perform this only when it is necessary */
  if ( ofs_required  == TRUE )
  {
    if ( hdrsrchofs_ofreq_nset_count() == 0 )
    {
      ofs_required = FALSE;
        /* set OFS required to FALSE */
    }
    else
    {
      hdrsrchofs_update_ofreq_list();
        /* update ofreq list */

      if ( hdrsrchofs.ofreq.lcount == 0 )
      {
        ofs_required = FALSE;
          /* set OFS required to FALSE */
      }
    }
  }

  HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "OFS: EcIo Thrsh1= -%d dB, Thrsh2= -%d dB, n1= %d , n2= %d",
                  HDRSRCHOFS_IDLE_ECIO_THRESHOLD1_HALF_DB/2,
                  HDRSRCHOFS_IDLE_ECIO_THRESHOLD2_HALF_DB/2,
                  count1,
                  count2 );

  HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "OFS: TimeDelta= %d ms, O-Nset= %d, Nfreq= %d, Required= %d",
                  (uint32) ( (int32) now - (int32) hdrsrchofs.idle_ofs_time ),
                  hdrsrchofs_ofreq_nset_count(),
                  hdrsrchofs.ofreq.lcount,
                  ofs_required );
    /* print OFS decision parameters */

#ifdef HDRSRCH_1X_SEARCH_TEST

  if ( HDRSRCH_NV_FORCE_IDLE_OFS_ENABLE == TRUE )
  {
    ofs_required = TRUE;
      /* For testing to enable OFS always by NV item */

    hdrsrchofs_update_ofreq_list();
  }

#endif /* HDRSRCH_1X_SEARCH_TEST */

  hdrsrchofs.ofs.required = ofs_required;
    /* Set ofs required flag in OFS structure */

  return ( ofs_required );

}/* hdrsrchofs_idle_ofs_reqd */


/* <EJECT> */
/*===========================================================================

FUNCTION    HDRSRCHOFS_TC_OFS_REQUIRED

DESCRIPTION
  Returns whether or not ofreq search is required at this moment

  It may not be necessary to perform a neighbor search on a different
  frequency if our active set very strong or we have lots of good neighbors
  and candidate sectors on the current frequency.

  Check following conditions to decide if OFS is needed for traffic state:
  
  1. all the aset and cset pilots are below -5db
  2. different frequency candidate or neighbour count is non-zero.

  OFS required = ( 1 && 2 )

DEPENDENCIES

PARAMETERS

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS

===========================================================================*/

boolean hdrsrchofs_tc_ofs_reqd( void )
{
  uint16  energy_thresh;
    /* threshold for energy comparison, in linear scale */

  boolean ofs_required = FALSE;
    /* start with OFS not required */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchofs_demote_ofreq_cset_pilots();
    /* Demote the obsolete ofs CSET pilots  */

  energy_thresh = hdrsrchset_ecio_to_eng( HDRSRCHOFS_MIN_ASET_ECIO_HALF_DB );
    /* Convert the threshold from DB scale to linear scale, because the 
       energy stored for each sector is in linear scale                    */

  if ( hdrsrchutil_check_ofs_condition( energy_thresh ) == TRUE )
  {
    /* TRUE means all the ASET and CSET pilots are below the threshold */

    if ( hdrsrchofs_ofreq_nset_count() > 0 ) 
    {
      /* Check if there are any off-frequency candidates or neighbors
         around here. updating freq list is pretty intensive job so
         perform this only when it is necessary */

      hdrsrchofs_update_ofreq_list();

      if ( hdrsrchofs.ofreq.lcount > 0 )
      {
        ofs_required = TRUE;

        HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "OFS: tot=%d",
                        hdrsrchofs.ofreq.lcount );
      }/* if ofreq pilots present */

    } /* if ofreq_nset_count > 0 */
  
  } /* if hdrsrchutil_check_ofs_condition  */

#ifdef HDRSRCH_1X_SEARCH_TEST

  if ( HDRSRCH_NV_FORCE_TC_OFS_ENABLE == TRUE )
  {
    ofs_required = TRUE;
      /* For testing to enable OFS always by NV item */

    hdrsrchofs_update_ofreq_list();
  }

#endif /* HDRSRCH_1X_SEARCH_TEST */

  hdrsrchofs.ofs.required = ofs_required;
    /* Set ofs required flag in OFS structure */

  return ( ofs_required );

}/* hdrsrchofs_tc_ofs_reqd */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHOFS_START_OFS

DESCRIPTION
  This function start off-frequency searching on the next candidate frequency.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchofs_start_ofs( void )
{

  sys_channel_type        ofs_chan;
    /* off-frequency channel */

#ifdef FEATURE_HDR_REVB
  hdrsrch_demod_idx_type   ofs_demod_idx = HDRSRCH_DEMOD_IDX_0;
    /* Demod index used in the OFS by default is demod0 */
#endif /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_PROFILE( OFS_START );
    /* sleep timeline profiling output if enabled */

  hdrsrchofs_determine_ofs_chan( &ofs_chan );
    /* Determine the frequency that needs to be searched */

  hdrsrchlog_inc_conn_state_stats_counter( HDRSRCHLOG_OFS_COUNT, 1 );
    /* Count the number of OFS searches for connected state logging purposes. 
     */

  if ( ofs_chan.chan_num != 0xffff )
  {

    hdrsrchlog.sleep_info.ofs_status = HDRLOG_OFS_NO_HO;
      /* Record OFS status as NO_HO for now */

#ifdef FEATURE_HDR_AGC_OPTIMIZATION_P1

#if ( HDRSRCHOFS_USE_FAST_AGC_IN_OFS_TUNE == TRUE )

    hdrsrchrf_set_agc_acq_params( HDRSRCHRF_FAST_AGC_ACQ_MODE,
                                  hdrsrchutil_get_agc_rtcoffset(),
                                  0 );
      /* Set AGC acquisition params for fast mode. */
#else

    hdrsrchrf_set_agc_acq_params( HDRSRCHRF_NORMAL_AGC_ACQ_MODE, 0, 0 );
      /* Set AGC acquisition params for normal mode. */
#endif

#endif /* FEATURE_HDR_AGC_OPTIMIZATION_P1 */

    /* REVISIT: For OFS we might want to set a timeout if we can't tune
       band within a set time */

    if( !hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                                 &ofs_chan,
#ifdef FEATURE_HDR_REVB
                                 &ofs_demod_idx,
                                 1,
#endif /* FEATURE_HDR_REVB */
                                 hdrsrchofs_tune,
                                 HDRSRCHOFS_BAND_GRANT_TIMEOUT_MS ) )
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Band tune not allowed skipping ofs " );
      hdrsrchlog.sleep_info.ofs_status  = HDRLOG_OFS_NO_OFS;
        /* Record OFS status as NO_OFS */

      /* exit ofs is called as part of hdrsrchofs_tune hence not required here */
    }
    else
    {
      if ( hdrsrchofs.ofs_reason == HDRSRCHOFS_OFS_SRCH )
      {
        hdrsrchofs.ofs.last_chan = ofs_chan;
         /* record last searched OFS channel */
      }    
    }
     
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "No freqs to search in OFS state" );

    HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "OFS O-Nset= %d ,Nfreq= %d, RUP Pending= %d, OFS Req= %d.",
                    hdrsrchofs_ofreq_nset_count(),
                    hdrsrchofs.ofreq.lcount,
                    hdrsrchofs.rup.pending,                    
                    hdrsrchofs.ofs.required );
    
    hdrsrchlog.sleep_info.ofs_status  = HDRLOG_OFS_NO_OFS;
      /* Record OFS status as NO_OFS */

    hdrsrchofs_exit_ofs();
      /* Nothing to search for. exit ofs */
  }

} /* hdrsrchofs_start_ofs( ) */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHOFS_LOG_HO_INFO

DESCRIPTION
  This function records hand-off information after an Off-frequency search.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchofs_log_ho_info
( 
  hdrsrch_sect_struct_type *sect,
  /* sectore that we are handing off to */
  
  uint16 best_eng  
  /* energy of the pilot that we are handing off to */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchlog.sleep_info.ofs_status          = HDRLOG_OFS_HO;
    /* Record OFS status                                                   */

  hdrsrchlog.sleep_info.ofs_pn              = sect->pn_offset;
  hdrsrchlog.sleep_info.ofs_pn_energy       = best_eng;
    /* Record the new pilot info                                           */

  hdrsrchlog.sleep_info.ofs_channel.chan_no = sect->chan.chan_num;
  hdrsrchlog.sleep_info.ofs_channel.band    = sect->chan.band;
    /* Record the new channel info                                         */

  INC_SAT( hdrsrchlog.sleep_info.ofs_handoff_count );
    /* Records the cumulative OFS hand-off counter. It saturates at 0xffff  */

} /* hdrsrchofs_log_ho_info */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHOFS_OFS_IDLE_ASET_MGMT

DESCRIPTION
  This function handles aset management after searching OFS in idle state

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Handoff indication

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL boolean hdrsrchofs_ofs_idle_aset_mgmt
(
  int new_rpts
    /* Number of new pilot report events (R/N to C, C>A, Weak A) */
)
{
  int                             i;
    /* Search loop index */

  hdrsrch_sect_struct_type        *sect;
    /* Strongest sector */

  uint16                          best_eng;
    /* Best energy so far */

  uint16                          comp_eng;
    /* OFS sector should be comparable to this eng */

  int16                           slew_x2;
    /* Reacquisition slew - the chipx2 error that occurred during sleep */

  int16                           prev_slew_x2;
    /* Slew of the weaker pilot from which AT is RAHOing to strong pilot */

  boolean                         handoff;
    /* Handoff indication */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------*
   * Find the best OFS pilot *
   *-------------------------*/

  sect      = hdrsrchdrv.srch_list[0].sect_ptr;
  best_eng  = sect->total_eng;
  slew_x2   = hdrsrchdrv.srch_list[0].srch_task.res_ptr->posx2[0];
  slew_x2  -= sect->win_cenx2;
    /* Assume first pilot is the best */

  INC_SAT( hdrsrchlog.sleep_info.ofs_count );
    /* Record the cumulative OFS counter.          */

  if ( !( hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT ) )
  {
     ERR_FATAL("Check hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT Failed", 0, 0, 0);
  }

  for( i = 1; i < hdrsrchdrv.list_cnt; i++ )
  {
    if ( hdrsrchdrv.srch_list[i].sect_ptr->total_eng > best_eng )
    {
      sect      = hdrsrchdrv.srch_list[i].sect_ptr;
      best_eng  = sect->total_eng;
      slew_x2   = hdrsrchdrv.srch_list[i].srch_task.res_ptr->posx2[0];
      slew_x2  -= sect->win_cenx2;
    }
  } /* for i */

  /*------------------------------------*
   * Check if aset needs to be replaced *
   *------------------------------------*/

#ifdef FEATURE_HDR_REVB
  if (hdrmc_feature_is_enabled(HDRMC_FEATURE_KDDI_OFS_HO_THRESHOLD) &&
      sect->chan.band != 
      hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].channel.band)
  {
    comp_eng = hdrsrchset_scale_eng( 
      hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->total_eng,
      HDRSRCHIDLE_IB_OFS_HO_THRESH_HALF_DB );
  }
  else
  {
    comp_eng = hdrsrchset_scale_eng( 
        hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->total_eng,
                                     HDRSRCHIDLE_OFS_HO_THRESH_HALF_DB );  
  }
#else
  if (hdrmc_feature_is_enabled(HDRMC_FEATURE_KDDI_OFS_HO_THRESHOLD) &&
      sect->chan.band != 
      hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].channel.band)
  {
    comp_eng = hdrsrchset_scale_eng( hdrsrchset.aset[0]->total_eng,
                                     HDRSRCHIDLE_IB_OFS_HO_THRESH_HALF_DB );
  }
  else
  {
    comp_eng = hdrsrchset_scale_eng( hdrsrchset.aset[0]->total_eng,
                                     HDRSRCHIDLE_OFS_HO_THRESH_HALF_DB );
  }

#endif /* FEATURE_HDR_REVB */

    /* Start with ASET energy. The OFS sector should be at least 1.5 dB more
       than the active set to avoid ping-pong between equal energy OFS
       and active set sectors. */

  if ( best_eng > comp_eng )
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "OFS HO to: PN=%d Eng=%d Slew=%d cx2",
                    sect->pn_offset, best_eng, slew_x2 );

    prev_slew_x2  = hdrsrchdrv.srch_list[0].srch_task.res_ptr->posx2[0];
    prev_slew_x2 -= hdrsrchdrv.srch_list[0].sect_ptr->win_cenx2;

#ifdef FEATURE_HDR_REVB
    HDR_MSG_SRCH_3( MSG_LEGACY_MED, "OFS HO from: PN=%d Eng=%d Slew=%d cx2",
            hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->pn_offset,
            hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->total_eng,
            prev_slew_x2 );
#else
    HDR_MSG_SRCH_3( MSG_LEGACY_MED, "OFS HO from: PN=%d Eng=%d Slew=%d cx2",
                    hdrsrchset.aset[0]->pn_offset,
                    hdrsrchset.aset[0]->total_eng,
                    prev_slew_x2 );
#endif /* FEATURE_HDR_REVB */

    hdrsrchofs_log_ho_info( sect, best_eng );
      /* Record the handoff info here       */

    hdrsrchutil_aset_idle_handoff( sect );
      /* Handoff to new pilot */

    hdrsrchidle_handoff_link_report( sect );
      /* Inform protocol of Link Maintenance handoff to given sector */

    hdrsrchrf_pdm_capture_values( );
      /* Capture PDM values for fast RF warmup at new freq */

    handoff = TRUE;
      /* Indicate an ASET pilot handoff was done */

  } /* if ( best_eng > comp_eng ) */
  else
  {
    hdrsrchlog.sleep_info.ofs_status = HDRLOG_OFS_NO_HO;
      /* Record OFS status */
      
    hdrsrchlog.sleep_info.ofs_channel.chan_no = sect->chan.chan_num;
    hdrsrchlog.sleep_info.ofs_channel.band    = sect->chan.band;
      /* Update the OFS band and channel information in the sleep info log */
      
    handoff = FALSE;
      /* Indicate No ASET pilot handoff was done */
  }

  return handoff;
    /* return the handoff indication */

}/* hdrsrchofs_ofs_idle_aset_mgmt */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHOFS_EXIT_OFS

DESCRIPTION
  Exit from OFS state and return back to where you came from or to the
  next state

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

===========================================================================*/

LOCAL void hdrsrchofs_exit_ofs( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch ( hdrsrchofs.last_state )
  {
    case HDRSRCH_IDLE_STATE :
    
      if ( hdrsrchofs.return_state == HDRSRCH_IDLE_STATE )
      {
        hdrsrchmon_init_idle_state();
          /* go back to monitor state */
      }
      else if ( hdrsrchofs.return_state == HDRSRCH_SLEEP_STATE )
      {
        (void) hdrsrchsleep_enter_sleep( );
          /* go to sleep state. function will error fatal if it doesn't sleep
             in this case. */
      }
      else
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,  "Invalid return state =%x from idle OFS",
                        hdrsrchofs.return_state );
      }
      break;

    case HDRSRCH_CONNECTED_STATE :
    case HDRSRCH_SUSPENDED_TC_STATE :
      hdrsrchtc_init_connected_state();
      break;

    case HDRSRCH_TIME_SYNC_STATE:

      hdrsrchofs.lte_meas.is_meas_cfg_req_sent = FALSE;
        /* Reset flag */

      hdrsrchrf_force_chain_unlock();
        /* This is for idle mode. */
#ifdef FEATURE_XO
      hdrsrchafc_xo_disable_mnd();
      HDR_MSG_SRCH( MSG_LEGACY_HIGH,"IRAT MEAS: send FLL_XO_CFG mnd disabled");
#endif
      hdrsrchst_init_inactive_state();
        /* Go back to inactive state. Clock will be turned off in inactivate
           state */
      break;

    default:
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,  "OFS entered from invalid state=%x",
                      hdrsrchofs.last_state );

  }/* switch */

}/* hdrsrchofs_exit_ofs */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHOFS_TUNE

DESCRIPTION
  Indicates that ofreq tunning is complete.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

===========================================================================*/

LOCAL void hdrsrchofs_tune
( 
  hdrsrchrf_tune_status_enum_type status
)
{
  boolean              ofs_dump_pending = FALSE;
    /* OFS search dump pending indication. Initialized to False because 
       we may be able to leave OFS state after this dump */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  HDR_MSG_SRCH_1(MSG_LEGACY_MED, "OFS tune status %d",
                 status);

  if ( status == HDRSRCHRF_TUNE_SUCCESS )
  {
#ifdef FEATURE_HDR_SELECTIVE_TXD
    hdrsrchofs.tune_done = TRUE;
#endif

    hdrsrchrf_report_rx_power();
  
    HDRSRCH_PROFILE( OFS_TUNE_DONE );
      /* sleep timeline profiling output if enabled */
  
    if ( hdrsrchofs.ofs_reason == HDRSRCHOFS_OFS_SRCH )
    {
      ofs_dump_pending = hdrsrchutil_program_ofs_search_set( hdrsrchofs.srch_system ); 
        /* RF is tuned to the new Frequency that needs to be searched. Send new
           searches for all the sectors on that channel */
    }
    else if ( hdrsrchofs.ofs_reason == HDRSRCHOFS_RUP_SRCH )
    {
      hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
        /* Set Srch4 param to default values */

      ofs_dump_pending = hdrsrchutil_program_rup_search_set( hdrsrchofs.srch_system );
    }
  
    /* The following check is to make sure that the AT does not stay in OFS 
       waiting for a non-existent search dump. This may occur in Connected state 
       OFS because there is a delay between the time when we decide to perform 
       OFS and the actual tuning to the OFS channel. By the time the actual 
       tuning takes place, there might be no OFS pilots in this OFS channel */
    if ( ofs_dump_pending == FALSE )
    {
      hdrsrchofs_exit_ofs( );
        /* We've done our off-frequency searching ... now, time to exit */
    }
  }
  else
  {
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_OFS_FAILURE);
#endif
    hdrsrchofs_exit_ofs( );
      /* Tune failed, exit OFS */
  }

}/* hdrsrchofs_tune */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHOFS_CMD

DESCRIPTION
  This function dispatches commands to the appropriate command handler

DEPENDENCIES
  None

PARAMETERS
  cmd   - Command to be processed

RETURN VALUE
  E_SUCCESS       - command has been successfully executed
  E_FAILURE       - command has failed
  E_NOT_SUPPORTED - command is not support
  E_IN_PROGRESS   - command has been started, but has not completed
  E_AGAIN         - command cannot be started in present state, but is a
                    valid command.  Execute hdrsrch_retry_cmd( ) at a
                    later point in time to reattempt the command.
                    [hdrsrch_retry_cmd( ) is called at every state change]

SIDE EFFECTS
  If E_IN_PROGRESS or E_AGAIN is returned, command processing is disabled
  until the command buffer is passed to hdrsrch_complete_cmd( ).

===========================================================================*/

LOCAL errno_enum_type hdrsrchofs_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type           status = E_AGAIN;
    /* Command completion status */

  boolean                  band_granted;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( ( cmd->name & HDRSRCHCMD_CMD_MASK ) == HDRSRCH_DEACTIVATE_CMD )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Deactivated from %x", hdrsrch_state_id );

    hdrsrchlog.sleep_info.next_srch_state = HDRSRCH_INACTIVE_STATE;
      /* Updates the next_srch_state field in the log packet      */

    hdrsrchlog_log_sleep_info();
      /* State transition from IDLE to INACTIVE, needs to log this packet. */

    /* Exiting from traffic OFS state to inactive state and hence 
    then update APS state information */
    if( hdrsrch_get_current_state() == HDRSRCH_OFS_TC_STATE )
    {
#ifdef FEATURE_HDR_QTA
      hdrsrchrf_set_aps_client_state( HDRSRCHRF_TRM_END_STATE );
        /* Inform TRM for APS state end */   
#endif /* FEATURE_HDR_QTA */
    }

    hdrsrchst_init_inactive_state();
      /* Initialize start state */

    status = E_SUCCESS;
  }
#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
  else if ( ( cmd->name & HDRSRCHCMD_CMD_MASK ) == HDRSRCH_LTE_PILOT_MEAS_ABORT_REQ )
  {
    
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDRSRCH_LTE_PILOT_MEAS_ABORT_REQ cmd received in OFS" );
    
    if ( hdrsrch_state_id == HDRSRCH_OFS_MEAS_STATE )
    {
#ifdef FEATURE_HDR_IRAT_DEBUG
      if ( hdrsrchofs.lte_meas.skip_gap == 1 )
      { 
 
        hdrsrchofs.lte_meas.wait_for_gap = FALSE;
 
        if ( ( hdrsrchofs.lte_meas.is_meas_cfg_req_sent == TRUE ) 
         && ( hdrsrchofs.lte_meas.online_srch == TRUE ) )
        {           
          /* Init message header, then send. */
          msgr_init_hdr( &hdrsrchofs.lte_meas.fw_msg.pilot_meas_stop_stream_req.hdr, 
                         MSGR_HDR_SRCH, 
                         HDR_FW_PILOT_MEAS_STOP_STREAM_CMD );
          
          hdrsrchmsg_send_msg( &hdrsrchofs.lte_meas.fw_msg.pilot_meas_stop_stream_req.hdr, 
                                 sizeof( hdrfw_pilot_meas_stop_stream_msg_t ) );
            /* Send the message */
 
          hdrsrchofs.lte_meas.is_meas_cfg_req_sent = FALSE;
            /* Reset flag */
 
          HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
           "sent HDR_FW_PILOT_MEAS_STOP_STREAM_CMD to fw" );
        }
        else
        {
          HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
           "Skip sending HDR_FW_PILOT_MEAS_STOP_STREAM_CMD to fw" );
        }
 
        hdrsrchmsg_send_lte_pilot_meas_rsp( 
        PILOT_MEAS_BE_ABORTED,
        hdrsrchofs.lte_meas.channel );
 
        /* Send Pilot Meas Abort Response */
        HDR_MSG_SRCH( MSG_LEGACY_HIGH, "sent PILOT_MEAS_BE_ABORTED cmd to LTE" );
 
        hdrsrchmsg_send_msg( &hdrsrchmsg_lte.lte_pilot_meas_abort_rsp, 
                              sizeof( hdrsrch_lte_pilot_meas_abort_rsp_msg_t ) );
 
        hdrsrchofs_exit_ofs();
      }
      else
#endif /* FEATURE_HDR_IRAT_DEBUG */
      {
         hdrsrchofs_meas_process_pilot_meas_abort_req();
      }
    }
    status = E_SUCCESS;
  }
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "Received cmd %d in OFS. Punting...",
                    cmd->name );
  }

  return status;
} /* hdrsrchofs_cmd(cmd) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHOFS_DUMP

DESCRIPTION
  This function handles the off frequency search dump signal.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchofs_dump( void )
{
  int8                            new_rpts;
   /* number of new reports generated during set evaluation */

  boolean                         handoff = FALSE;
    /* Handoff indication after aset management and Pilot report to RUP */

  boolean                         ofs_dump_pending = FALSE;
    /* OFS search dump pending indication. Initialized to False because 
       we may be able to leave OFS state after this dump */

  int8                            ofs_chan_idx = 0;     
    /* Index into the off-freq list */
  
  boolean                         valid;
    /* Flag to indicate whether or not searcher results are valid */

  boolean                         rur_pilots_reported = FALSE;
    /* Flag to indicate whether pilots were reported due to RUP search */

  const sys_channel_type*         chan;
    /* CDMA Channel Number */

#ifdef FEATURE_HDR_REVB
  uint8                           chan_cnt;
    /* Number of the current assigned channels */
  const hdrsrch_demod_idx_type    *demod_list;
    /* A pointor to the demod index assignment from hdrsrchrf. */
#endif /* FEATURE_HDR_REVB */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* To make sure TC or BC pending ASET search dump is processed after transition 
     to OFS state */
  if ( hdrsrchdrv.srch_task_type == HDRSRCH_PRI_CHAIN_ASET_SRCH )
  {
    hdrsrchutil_pri_chain_aset_dump( );
  }
  else
  {
    HDRSRCH_PROFILE( OFS_DUMP );
      /* sleep timeline profiling output if enabled */
    HDR_MSG_SRCH_5( MSG_LEGACY_MED, "OFS Dump: Systype= %d [%d], Srched %d pilots[pn=%d e=%d]",
                    hdrsrchofs.srch_system,
                    hdrsrchdrv.system,
                    hdrsrchdrv.list_cnt,
                    hdrsrchdrv.srch_list[0].srch_task.pn_offset,
                    hdrsrchdrv.srch_list[0].srch_task.res_ptr->eng[0] );

  #ifdef FEATURE_HDR_REVB
    chan = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, 
                                  &chan_cnt,
                                  &demod_list );
      /* Grab the current channel */
  #else
    chan = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN );
      /* Grab the current channel */
  #endif /* FEATURE_HDR_REVB */

    if ( hdrsrchofs.ofs_reason == HDRSRCHOFS_OFS_SRCH )
    {
      ofs_chan_idx = hdrsrchofs_find_ofreq_chan( *chan );
        /* Find the index of this entry in the ofreq list */

      if ( ofs_chan_idx == -1 )
      {
        HDR_MSG_SRCH_2( MSG_LEGACY_ERROR,
                        "HDR OFS: Searched chan. not in list. Freq= %d, Band= %d",
                        chan->chan_num,
                        chan->band );

        hdrsrchofs.ofs.required = FALSE;
          /* reset OFS required flag */

        hdrsrchofs.ofs.repeat_required = FALSE;
          /* reset OFS search repetition required flag */

        hdrsrchofs_exit_ofs( );
          /* Searched an OFS channel that has been removed from list */

        return;
      }
    }

    if ( hdrsrchofs.srch_system == HDRSRCH_SYSTYPE_1X )
    {
      valid = TRUE;
        /* Can we validate 1x searcher results??? */
    }
    else
    {
      valid = hdrsrchutil_validate_searcher_results();
        /* Perform searcher result validation before using the results */
    }

    hdrsrchofs.manage_sets = FALSE;
      /* Assume no set management is needed yet */

    if ( valid == FALSE )
    {
      hdrsrchutil_invalidate_search_dump();
        /* invalidate searcher results */
    }
    else
    {
      if ( hdrsrchofs.reset_filters == TRUE )
      {
        if ( hdrsrchofs.ofs_reason == HDRSRCHOFS_OFS_SRCH )
        {
          hdrsrchutil_reset_srch_filters();
            /* reset the filters if first search of the 
               of the OFS measurements */
        }

        hdrsrchutil_reset_ofs_filter( );
          /* Reset OFS filter */
      }

      if ( hdrsrchofs.ofs_reason == HDRSRCHOFS_OFS_SRCH )
      {
        /* Only do set maintenance if this the last search of the
           OFS measurement set and system type is DO */        
        if ( ( hdrsrchofs.ofs.repeat_count >= hdrsrchofs.repeat_max-1 ) &&
             ( hdrsrchofs.srch_system == HDRSRCH_SYSTYPE_DO ) )
        {
          hdrsrchofs.manage_sets = TRUE;
        }

        /* DO OFS search dump processing */
        new_rpts = hdrsrchutil_process_ofs_search_dump( hdrsrchofs.manage_sets );
          /* If manage_sets is FALSE then only do pilot filtering */

        /* do ASET management as well if we need to manage sets */
        if ( hdrsrchofs.manage_sets == TRUE )
        {
          if ( hdrsrch_state_id == HDRSRCH_OFS_IDLE_STATE )
          {
            handoff = hdrsrchofs_ofs_idle_aset_mgmt( new_rpts );
              /* perform idle state aset management .... */

          } /* if HDRSRCH_OFS_IDLE_STATE */ 
          else if ( hdrsrch_state_id == HDRSRCH_OFS_TC_STATE )
          {
            if ( new_rpts > 0 )
            {
              hdrsrchtc_aset_mgmt( new_rpts );
                /* Set the pilot report pending flag in TC */

              handoff = hdrsrchtc_send_pilot_report();
                /* Send the pilot report to protocols */
            }
          } /* else if HDRSRCH_OFS_TC_STATE */
        }
      }
      else if ( hdrsrchofs.ofs_reason == HDRSRCHOFS_RUP_SRCH )
      /* If not HDRSRCHOFS_OFS_SRCH then it is HDRSRCHOFS_RUP_SRCH */    
      {
        if ( hdrsrchofs.rup.pilot_meas == FALSE )
        {
          /* this search dump is for RUP request */

          if ( hdrsrchofs.rup.repeat_count >= hdrsrchofs.repeat_max-1 )
          {
            hdrsrchofs.manage_sets = TRUE;
              /* Only do set maintenance if this the last search of the
                 RUP measurement set */
          }

          /* DO RUP search dump processing */
          new_rpts = hdrsrchutil_process_ofs_search_dump( hdrsrchofs.manage_sets );
            /* If manage_sets is FALSE then only do pilot filtering */

          if ( new_rpts != 0 )
          {
            handoff = TRUE;
              /* If new events to report then finish RUP request processing */
            
            hdrsrchofs.rup.report_pending = TRUE;
              /* set the flag that a RUP report is pending */

            hdrsrchofs_clear_rup_req();
              /* the RUP search is concluded, do not search further */
            
            hdrsrchutil_send_rup_pilot_report();            
              /* Send the pilot report corresponding to the RUP request */
            
            rur_pilots_reported = TRUE;
              /* set flag indicating that pilots were reported due to RUP search */
          }
        }
#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
        else  /* hdrsrchofs.rup.pilot_meas == TRUE */
        {
          /* this is dump for pilot measurement search,
             check if we did full PN search and triage the result */
          hdrsrchutil_chk_triage_full_pn( );

          hdrsrchutil_process_ofs_search_dump_for_pilot_meas();
            /* calculate pilot energy */

          hdrsrchofs_clear_pilot_meas_req();
            /* the search for pilot measuremnt is concluded */

          hdrsrchofs.rup.report_pending = TRUE;
              /* set the flag that a RUP report is pending */

          hdrsrchutil_send_rup_pilot_report();            
            /* Send the pilot report corresponding to the pilot meas request */

          rur_pilots_reported = TRUE;
            /* set flag indicating that pilots were reported due to RUP search */
        }
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */
      }    
    }
    if ( hdrsrchofs.manage_sets == TRUE )
    {
      hdrsrchmsg_send_multi_asp_update_msg( TRUE );
        /* Send an ASP update message if needed.  This function may get called
           back to back as it is also called in hdrsrchtc_aset_mgmt(), but the
           mDSP update will only happen once then the ASP update flags are
           cleared.  Thus, it is safe to call
           hdrsrchmsg_send_multi_asp_update_msg() back to back. */
    }

    /* If there is no handoff yet and there are more off-freq neighbors
       to search, send next search on the current channel */
    if ( handoff == FALSE )
    {
      if ( hdrsrchofs.ofs_reason == HDRSRCHOFS_OFS_SRCH )
      {
        if ( hdrsrchofs.ofreq.list[ofs_chan_idx].searched == FALSE )
        {
          /* RF is tuned to the new frequency that needs to be searched. The new
             channel number is reflected in hdrsrchdrv.channel field. Send new
             search for all the sectors not yet searched on that channel. If pilots 
             were submitted for search, stay in OFS state for pending search dump 
          */

          ofs_dump_pending = hdrsrchutil_program_ofs_search_set( hdrsrchofs.srch_system );
        }
      }
      else if ( hdrsrchofs.ofs_reason == HDRSRCHOFS_RUP_SRCH )
      {        
        if ( hdrsrchofs.rup.set_completed == FALSE )
        {
          hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
            /* Set Srch4 param to default values */

          ofs_dump_pending = hdrsrchutil_program_rup_search_set( hdrsrchofs.srch_system );
            /* program search for remaining pilots in RUP set */
        }
        /* If we are in IDLE state then complete RUP request in one OFS visit */
        else if ( ( hdrsrch_state_id == HDRSRCH_OFS_IDLE_STATE ) &&
                  ( hdrsrchofs.rup.last_set == FALSE ) )
        {
          if ( hdrsrchofs.rup.pilot_meas == FALSE)
          {
            /* this is a OFS search for RUP request */

            hdrsrchofs.rup.last_set = hdrsrchutil_generate_rup_set( hdrsrchofs.srch_system );
              /* generate the RUP sectors to be searched */
          }
          else
          {
            /* this is a OFS search for pilot measurement request */
            hdrsrchofs.rup.last_set = 
                 hdrsrchutil_generate_pilot_meas_search_set( hdrsrchofs.srch_system );
              /* generate the pilot measurement sectors to be searched */
          }
            
          hdrsrchofs.rup.set_completed = FALSE;
            /* reset flag for newly generated RUP set */

          hdrsrchofs.rup.next_srch_index = 0;
            /* reset index into RUP set  */

          hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
            /* Set Srch4 param to default values */

          ofs_dump_pending = hdrsrchutil_program_rup_search_set( hdrsrchofs.srch_system );
        }
      }
    } /* else if valid == TRUE */

    /* check whether we are done searching in this visit of OFS state */
    if ( ofs_dump_pending == FALSE )
    {
      if ( hdrsrchofs.ofs_reason == HDRSRCHOFS_OFS_SRCH )
      {
        hdrsrchofs.ofs.repeat_count++;
          /* Increment the search repeat count for current OFS channel */

        if ( hdrsrchofs.ofs.repeat_count >= hdrsrchofs.repeat_max )
        {
          hdrsrchofs.ofreq.list[ofs_chan_idx].searched = TRUE;
            /* Mark the current frequency as being searched */

          hdrsrchofs.ofs.repeat_count = 0;
            /* This is the last repeat search for the current OFS channel 
             reset repeat count for new OFS channel next time */

          hdrsrchofs.ofs.repeat_required = FALSE;
            /* Desired number of repetitions for OFS searches are done */

          hdrsrchofs_reset_nset_pilots( );
            /* Sets the Ec/Io of NSET off-freq pilots to 0,
               otherwise they will be frozen. 
               This can be confusing in some cases */
        }
        else
        {
          hdrsrchofs.ofreq.list[ofs_chan_idx].searched = FALSE;
            /* Mark the current frequency as not being searched yet 
               we did not do hdrsrchofs.repeat_max searches yet*/

          hdrsrchofs.ofs.repeat_required = TRUE;
            /* Need to repeat the OFS searches */
        }
      }
      else if ( hdrsrchofs.ofs_reason == HDRSRCHOFS_RUP_SRCH )
      {
        hdrsrchofs.rup.repeat_count++;
          /* Increment the search repeat count for current RUP search */

        if ( hdrsrchofs.rup.repeat_count >= hdrsrchofs.repeat_max )
        {
          hdrsrchofs.rup.repeat_count = 0;
            /* This is the last repeat search for the current RUP,
               reset it for the next search */

          hdrsrchofs.rup.repeat_required = FALSE;
            /* Desired repetitions for current RUP set search are done */

          if ( hdrsrchofs.rup.last_set == TRUE )
          {
            if ( hdrsrchofs.rup.pilot_meas == FALSE )
            {                                      
              hdrsrchofs_clear_rup_req();
                /* the RUP search is concluded */
            }
            else
            {
              hdrsrchofs_clear_pilot_meas_req();
                /* the RUP search is concluded */
            }
            
            if ( rur_pilots_reported == FALSE )
            {
              hdrsrchutil_send_rup_pilot_report();            
                /* even if no report has been generated trigger RUP anyway
                   so that at least a reference pilot will be sent to AN */
            }
          }
        }
        else
        {
          hdrsrchofs.rup.repeat_required = TRUE;
            /* Need to repeat the RUP searches */
        }
      }

      hdrsrchofs_exit_ofs( );
        /* We've done our off-frequency searching ... now, time to exit */
    }
  }
} /* hdrsrchofs_dump( ) */

/*===========================================================================

FUNCTION HDRSRCHOFS_REPETITION_REQUIRED

DESCRIPTION
  This function returns the flag that indicates 
  whether repetitions for RUP or OFS search are pending.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Returns the flag whether repetitions for current RUP or OFS search are pending

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchofs_repetition_required( void )
{

  boolean            repeat_required = FALSE;
    /* flag whether repetitions for current RUP or OFS search are pending */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  repeat_required = ( ( hdrsrchofs.ofs.repeat_required == TRUE ) || 
                      ( hdrsrchofs.rup.repeat_required == TRUE ) );

  return ( repeat_required );

} /* hdrsrchofs_get_ofs_repeat_status( ) */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHOFS_RESET_REPEAT_COUNT

DESCRIPTION
  This function reset repetition count for OFS and RUP searches.
    
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchofs_reset_repeat_count( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Reset repeat count for OFS and RUP searches */
  hdrsrchofs.ofs.repeat_count = 0;
    /* reset repetition count for OFS search */

  hdrsrchofs.ofs.repeat_required = FALSE;
    /* reset OFS search repetition required flag */
  
  hdrsrchofs.rup.repeat_count = 0;
    /* reset repetition count for RUP search */
                
  hdrsrchofs.rup.repeat_required = FALSE;
    /* reset RUP search repetition required flag */

} /* hdrsrchofs_reset_repeat_count( ) */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHOFS_INIT

DESCRIPTION
  Off Frequency Search State Initialization

DEPENDENCIES
  Idle on an HDR System

PARAMETERS
  last_state  - Last HDR Searcher State

RETURN VALUE
  None

SIDE EFFECTS
  TBD

===========================================================================*/

LOCAL void hdrsrchofs_init( hdrsrch_state_enum_type last_state )
{
  boolean asdiv_pending = hdrsrchrftxd_switch_is_pending();
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_default_init( last_state );
    /* Use default handling */

#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchofs.tune_done = FALSE;
#endif

  hdrsrchofs.last_state = last_state;
    /* save the last state from where OFS was entered */

  hdrsrchofs.waiting_for_tcxo = FALSE;
    /* Initialize as though we are ready to go. */

  hdrsrchofs.is_channel_in_aset = FALSE;
    /* Initialize it to false. */

  hdrsrchofs.ofs_reason = HDRSRCHOFS_OFS_SRCH;
    /* assume we are tuning away for OFS search not RUP search */

  hdrsrchofs_update_ofreq_list();
    /* Make sure the ofreq list is up-to-date so that we don't scan a 
       channel that has been removed since we made the decision to OFS. */



  if ( ( !asdiv_pending ) &&
       ( ( hdrsrchofs.rup.pending == TRUE ) ||
       ( ( ( hdrsrchofs.ofs.required == TRUE ) || 
           ( hdrsrchofs.ofs.repeat_required == TRUE ) ) && 
         ( hdrsrchofs.ofreq.lcount > 0 ) ) 
      )
     )
  {
    if ( hdrsrch_state_id == HDRSRCH_OFS_IDLE_STATE )
    {
      hdrsrchofs.idle_ofs_time = timetick_get_ms();
        /* Set last idle ofs time as the current time */
#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV
      if( hdrsrch.idle_state_div.idle_div_enabled ||
          hdrsrch.idle_state_div.access_div_enabled )
      {
        
          HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: Turning off div in Idle OFS"
                                      " mode after tune back");
           hdrsrchrfdiv_diversity_ctrl( FALSE );
            /* Enable Diversity */ 
          
        if( hdrsrch.idle_state_div.idle_div_enabled )
        {
         /* Deactivate decode m slot indication */
          hdrsrchsleep_deactivate_scc_decode_ind();
      
          hdrlog_register_slot_logging_cb ( HDRLOG_SRCHDIV_MDSP_SLOT_LOGGING_CLIENT,
                                            NULL );
        }
      }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */
    }
    else if ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE )
    {
      hdrsrchofs.tc_ofs_time = timetick_get_ms();
        /* Set last tc ofs time as the current time */
    }

    if ( ( hdrsrchofs.rup.pending == TRUE ) 
         && ( hdrsrchofs.rup.pilot_meas == TRUE )
#ifdef FEATURE_HDR_REVB
         && ( hdrsrchset_is_ofreq_chan( hdrsrchofs.rup.chan ) == FALSE )
#else
         && ( SYS_CHAN_CMP_EQUAL( hdrsrchofs.rup.chan, hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
       )
    {
      /* For pilot measurement velcro/fusion solution, the searched channel may be
         the same as aset channel. We don't need to stop and start freq tracking in
         this case
      */

      hdrsrchofs.is_channel_in_aset = TRUE;
        /* This is not a "real" off-freq search. The searched channel is the same
           as aset channel */

      hdrsrch_on_searcher_idle( hdrsrchofs_start_ofs );
          /* There is Off Frequency Searching to do.  Start it when
             we go idle. */
    }
    else
    {
      hdrsrchafc_stop_fll_tracking();
        /* Disable FLL tracking and save the FLL accum value. */

      hdrsrchofs.waiting_for_tcxo = TRUE;
        /* Now we are waiting for tcxo services before we start OFS. */

      hdrsrchafc_request_open_loop();
        /* Ask tcxo manager for open loop mode privileges.  We'll set the trk_lo
         to a reasonable value if granted open loop. */
    }

  }
  else
  {
    if ( asdiv_pending )
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                    "Entered OFS while ASDiv pending. Exiting!" );
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                    "Entered OFS without any ofreqs. Exiting!" );

      HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "OFS O-Nset= %d ,Nfreq= %d, RUP Pending= %d, OFS req= %d.",
                      hdrsrchofs_ofreq_nset_count(),
                      hdrsrchofs.ofreq.lcount,
                      hdrsrchofs.rup.pending,                    
                      hdrsrchofs.ofs.required );
    }

    hdrsrchlog.sleep_info.ofs_status  = HDRLOG_OFS_NO_OFS;
      /* Records OFS status   */

    hdrsrchofs_exit_ofs();
      /* exit ofs state */
  }
} /* hdrsrchofs_init( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHOFS_DONE

DESCRIPTION
  Off Frequency Search State Finalization

DEPENDENCIES
  Exiting Off Frequency Search state

PARAMETERS
  next_state  - State we are transitioning to

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchofs_done( hdrsrch_state_enum_type next_state )
{
#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_event_payload_type payload;
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_default_done( next_state );
    /* Use default handling */

  /* TODO: Other finalizations... */
#ifdef FEATURE_HDR_SELECTIVE_TXD
  if ( ( next_state != HDRSRCH_IDLE_STATE ) &&
       ( next_state != HDRSRCH_SUSPENDED_IDLE_STATE ) && 
       ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE ) )
  {
    /* if going into IDLE state, trigger from prep_mon */
    HDR_MSG_SRCH( MSG_LEGACY_MED, "TxD: ofs_done exit access");
    payload.next_state = next_state;
    hdrsrchrftxd_event_with_payload( HDRSRCHRFTXD_EVENT_EXITING_ACCESS,
                                     &payload );
  }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  if ( hdrsrchofs.is_channel_in_aset == FALSE )
  {
    /* Stop AFC. If it is to enter traffic, it will request continue tracking */
    hdrsrchafc_stop_fll_tracking();
  }
  else
  {
    /* no need to stop AFC if in ofs we searched the same channel as aset channel */
  }

  hdrsrchofs.lte_meas.abort_pilot_meas = FALSE;
    /* Reset abort_pilot_meas flag at the end */

} /* hdrsrchofs_done( ) */


/*===========================================================================

FUNCTION HDRSRCHOFS_TC_DONE

DESCRIPTION
  Off Frequency Search State Finalization

DEPENDENCIES
  Exiting Off Frequency Search state

PARAMETERS
  next_state  - State we are transitioning to

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchofs_tc_done( hdrsrch_state_enum_type next_state )
{
#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_event_payload_type payload;
#endif /* FEATURE_HDR_SELECTIVE_TXD */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_default_done( next_state );
    /* Use default handling */

  /* TODO: Other finalizations... */

  if ( hdrsrchofs.is_channel_in_aset == FALSE )
  {
    /* Stop AFC. If it is to enter traffic, it will request continue tracking */
    hdrsrchafc_stop_fll_tracking();
  }
  else
  {
    /* no need to stop AFC if in ofs we searched the same channel as aset channel */
  }

  if ( next_state != HDRSRCH_CONNECTED_STATE )
  {
#ifdef FEATURE_HDR_SELECTIVE_TXD
    payload.next_state = next_state;
    hdrsrchrftxd_event_with_payload( HDRSRCHRFTXD_EVENT_EXITING_TRAFFIC, 
                                     &payload );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
    hdrsrchard_event( HDRSRCHARD_EVENT_EXIT_TC );
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
  }
} /* hdrsrchofs_tc_done( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_INIT_OFS_STATE

DESCRIPTION
  This function causes the state machine to begin ofs state processing.

DEPENDENCIES
  Idle state

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Transitions to the sleep state

===========================================================================*/

void hdrsrchofs_init_ofs_state( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( (hdrsrch_state_id == HDRSRCH_CONNECTED_STATE) ||
       (hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE) )
  {
    hdrsrch_change_state( &hdrsrchofs_tc_state );
      /* Switch to suspended state processing */
  }
  else if ( hdrsrch_state_id == HDRSRCH_IDLE_STATE )
  {
    hdrsrch_change_state( &hdrsrchofs_idle_state );
      /* Switch to suspended state processing */
  }
#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
  else if ( hdrsrch_state_id == HDRSRCH_TIME_SYNC_STATE ) 
  {
    hdrsrch_change_state( &hdrsrchofs_meas_state );
      /* Switch to suspended state processing */
  }
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */
  else
  {
    ERR("Incorrect transition to ofs from %x", hdrsrch_state_id, 0, 0);
  }

} /* hdrsrchofs_init_ofs_state( ) */

/*===========================================================================

FUNCTION HDRSRCHOFS_GET_NEXT_SRCH_IDX

DESCRIPTION
  This function returns the next NSET pilot index  to be considered for 
  round-robin o-freq search.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Returns the index into the next sector to be searched

SIDE EFFECTS
  None

===========================================================================*/

uint8 hdrsrchofs_get_next_srch_idx( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  return( hdrsrchofs.ofreq.next_srch_idx );

} /* hdrsrchofs_get_ofs_idx( ) */

/*===========================================================================

FUNCTION HDRSRCHOFS_UPDATE_NEXT_SRCH_IDX

DESCRIPTION
  This function updates the search index used for round robin o-freq search.

DEPENDENCIES
  None

PARAMETERS
  Updated o-freq search index

RETURN VALUE
  None

SIDE EFFECTS
  If the o-freq search index has wrapped to zero then this o-freq channel is 
  marked as having been searched 

===========================================================================*/

void hdrsrchofs_update_next_srch_idx
(
  uint8                   next_srch_idx
    /* Updated o-freq search index */
)
{
  int8                    ofs_chan_idx = 0;     
    /* Index into the o-freq channel list */

  const sys_channel_type* chan;
    /* CDMA Channel Number */

#ifdef FEATURE_HDR_REVB
  uint8                   chan_cnt;
    /* The number of channels covered by the current RF config */

  const hdrsrch_demod_idx_type  *demod_list;
    /* A pointor to the demod index assignment from hdrsrchrf. */
#endif /* FEATURE_HDR_REVB */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_REVB
  chan = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, 
                                &chan_cnt,
                                &demod_list );
    /* Get the current channel or channel list; in revB phase 1, we assumed
       the same behavior as in rev0/revA, in which only one carrier is 
       programmed in the OFS RF configuration.  */

  ASSERT( chan_cnt == 1 );
    /* This is not true if we decide to search multiple carriers in the OFS */

  ofs_chan_idx   = hdrsrchofs_find_ofreq_chan( chan[0] );
    /* Find the index of this entry in the ofreq list */
#else
  chan = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN );
    /* Grab the current channel */

  ofs_chan_idx   = hdrsrchofs_find_ofreq_chan( *chan );
    /* Find the index of this entry in the ofreq list */
#endif /* FEATURE_HDR_REVB */

  hdrsrchofs.ofreq.next_srch_idx = next_srch_idx;
    /* Update the next o-freq NSET pilot to be considred */
  
  /* TBD: change for Multiple carrier OFS search */
  if ( next_srch_idx == 0 )
  {
    if ( ( ofs_chan_idx >=0 ) && ( ofs_chan_idx < ( HDR_MAX_NSET_SIZE ) ) )
    {
      hdrsrchofs.ofreq.list[ofs_chan_idx].searched = TRUE;
        /* Mark this frequency as searched - all o-freq pilots in this frequency 
           have been searched. Note that ofs_dump_pending indication decides exit 
           from OFS state */
    }
    else
    {
      ERR_FATAL( "OFS chan list is corrupted", 0, 0, 0 );
    }
  }

} /* hdrsrchofs_update_next_srch_idx( ) */


/*===========================================================================

FUNCTION HDRSRCHOFS_RUP_PENDING

DESCRIPTION
  This function returns the flag that whether
  there is a RUP request pending.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchofs_rup_pending( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( ( hdrsrchofs.rup.pending == TRUE ) &&
       ( hdrsrchofs.rup.pilot_meas == FALSE ) )
  {
    /* Only if pilot_meas flag is false, this is a RUP request pending */

    /* if request is for current channel then discard it */
#ifdef FEATURE_HDR_REVB
    if ( hdrsrchset_is_ofreq_chan( hdrsrchofs.rup.chan ) == FALSE )
#else
    if ( SYS_CHAN_CMP_EQUAL( hdrsrchofs.rup.chan, hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
    {        
      hdrsrchofs_clear_rup_req();
        /* clear the pending RUP request */        
    }

  }

  return ( hdrsrchofs.rup.pending );
  
} /* hdrsrchofs_rup_pending() */

/*===========================================================================

FUNCTION HDRSRCHOFS_RUP_REPORT_PENDING

DESCRIPTION
  This function returns the flag that whether
  there is a RUP search report pending.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  Flag whether there is RUP search report pending.

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchofs_rup_report_pending( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return( hdrsrchofs.rup.report_pending );
  
}
 
/*===========================================================================

FUNCTION HDRSRCHOFS_RESET_RUP_REPORT_PENDING

DESCRIPTION
  This function resets the flag that whether
  there is a RUP search report pending.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchofs_reset_rup_report_pending( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchofs.rup.report_pending = FALSE;
  
}

/*===========================================================================

FUNCTION HDRSRCHOFS_GET_RUPSET_SRCH_INDEX

DESCRIPTION
  This function returns the index into the RUP set 
  to indicate where in RUP set to start adding sectors to search.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  Index into the RUP set.
  
SIDE EFFECTS
  None

===========================================================================*/

uint16 hdrsrchofs_get_rupset_srch_index( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return( hdrsrchofs.rup.next_srch_index );
  
} /* hdrsrchofs_get_rupset_srch_index( ) */


/*===========================================================================

FUNCTION HDRSRCHOFS_SET_RUPSET_SRCH_INDEX

DESCRIPTION
  This function sets the index into the RUP set 
  to indicate where in RUP set to start getting sectors to search.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  If the rupset search index has wrapped to zero then we have completed 
  searching the desired RUP sectors for this round of OFS visit. 

===========================================================================*/

void hdrsrchofs_set_rupset_srch_index
(
  uint16                  next_srch_index
    /* Updated RUPset search index */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchofs.rup.next_srch_index = next_srch_index;
    /* Update the next o-freq NSET pilot to be considred */
  
  if ( next_srch_index == 0 )
  {
    hdrsrchofs.rup.set_completed = TRUE;
      /* set flag indicating that all sectors in current RUP set 
       has been searched */
  }
  
} /* hdrsrchofs_set_rupset_srch_index() */

/*===========================================================================

FUNCTION HDRSRCHOFS_REGISTER_RUP_REQ

DESCRIPTION
  This function registers pending RUP request with OFS state.

DEPENDENCIES
  None

PARAMETERS
  chan            - channel of the sector
  system          - system type of the channel
  is_pilot_meas   - whether the RUP search is for pilot measurement request
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchofs_register_rup_req
(
  sys_channel_type                chan,
    /* Channel of the sector */
    
  hdrsrch_system_enum_type        sys,
    /* system type for the current channel */

  boolean                         is_pilot_meas
    /* whether the RUP is for pilot measurement request */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchofs.rup.pending = TRUE;
    /* set flag that a RUP request is pending */

  hdrsrchofs.rup.chan = chan;
    /* set channel to be searched */
    
  hdrsrchofs.rup.system = sys;
    /* set system to be searched */

  /* perform RUP request related OFS state initializations */
  hdrsrchofs.rup.repeat_count = 0;
    /* reset repetition count for RUP search */
    
  hdrsrchofs.rup.repeat_required = FALSE;
    /* reset RUP search repetition required flag */  
    
  hdrsrchofs.rup.next_srch_index = 0;
    /* reset index into RUP set */
    
  hdrsrchofs.rup.set_completed = FALSE;
    /* reset flag indicating that all sectors in current RUP set 
       has been searched */
      
  hdrsrchofs.rup.last_set = TRUE;
    /* assume this is last RUP search set to be generated */

  hdrsrchofs.rup.pilot_meas = is_pilot_meas;
    /* set flag whether this is for pilot measurement */
  
  hdrsrchutil_reset_rup_srch_state( );
    /* Reset RUP srch state variables */

} /* hdrsrchofs_register_rup_req( ) */




/* EJECT */
/*==========================================================================

FUNCTION HDRSRCHOFS_PROCESS_INT_CMD

DESCRIPTION
  This function processes internal commands for idle and TC ofs states.

DEPENDENCIES
  None

INPUTS
  payload   - Command+payload to be processed

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchofs_process_int_cmd
(
  hdrsrchsm_cmd_payload_type                     *payload
    /* The internal command + payload */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  switch ( payload->cmd )
  {
    case HDRSRCH_AFC_OPEN_LOOP_CMD:
    
      /* It may be open loop or no control */

      /* Are we waiting for tcxo services before starting ofs */
      if ( hdrsrchofs.waiting_for_tcxo == TRUE )
      {
        hdrsrchofs.waiting_for_tcxo = FALSE;
          /* We are no longer waiting for TCXO. */

        hdrsrch_on_searcher_idle( hdrsrchofs_start_ofs );
          /* There is Off Frequency Searching to do.  Start it when
             we go idle. */
      }

      break;

    default:
  
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Unexpected internal cmd %d at OFS state",
                      payload->cmd );
  }

} /* hdrsrchofs_process_int_cmd */

/*===========================================================================

FUNCTION HDRSRCHOFS_IDLE_OFS_STATE_VISIT_REQUIRED

DESCRIPTION
  This function returns the flag that whether 
  we need to visit idle OFS state due to pending RUP 
  search or autonomus OFS search.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchofs_idle_ofs_state_visit_required( void )
{

  boolean            idle_ofs_visit = FALSE;
    /* flag whether need to transition to idle OFS state */
    
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  idle_ofs_visit = ( hdrsrchofs_idle_ofs_reqd() || hdrsrchofs_rup_pending() );
    /* if RUP search or autonomous OFS search is required 
       then need to visit idle OFS state */
       
  return( idle_ofs_visit );
  
} /* hdrsrchofs_idle_ofs_state_visit_required() */

#ifdef FEATURE_HDR_DDARF

/*===========================================================================

FUNCTION HDRSRCHOFS_REQUEST_IDLE_OFS

DESCRIPTION

  This function provides an interface to register an idle OFS request 
  to OFS state. So that an idle OFS can be performed regardless of 
  whether signal conditions for idle OFS is satisfied or not.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchofs_request_idle_ofs( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        
  hdrsrchofs.ofs.idle_ofs_request = TRUE;
    /* Indicate that idle OFS has been requested */

} /* hdrsrchofs_request_idle_ofs() */

#endif /* FEATURE_HDR_DDARF */

/*===========================================================================

FUNCTION HDRSRCHOFS_RESET_IDLE_OFS_TIME

DESCRIPTION

  This function initializes the last idle ofs time stamp to 
  HDRSRCHOFS_IDLE_OFS_INTERVAL_MS before the current time 
  so that an OFS search is not skipped due to OFS 
  interarrival duration limitation.
  
DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchofs_reset_idle_ofs_time( void )
{
  timetick_type     now;
    /* Current time */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    
  now = timetick_get_ms();
    /* Get the current time */
    
  hdrsrchofs.idle_ofs_time = now - HDRSRCHOFS_IDLE_OFS_INTERVAL_MS;
    /* initialize the last idle ofs time stamp to 
       HDRSRCHOFS_IDLE_OFS_INTERVAL_MS before the current time 
       so that we do not prevent an OFS search due to OFS 
       interarrival duration limitation */
       
} /* hdrsrchofs_reset_idle_ofs_time() */

/*===========================================================================

FUNCTION HDRSRCHOFS_INIT_OFS_TIMELINE

DESCRIPTION

  This function initializes the last idle ofs time stamp to 
  HDRSRCHOFS_IDLE_OFS_INTERVAL_MS before the current time 
  so that an OFS search is not skipped due to OFS 
  interarrival duration limitation.
  
  It also resets state flags related to OFS and RUP search.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchofs_init_ofs_timeline( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchofs_reset_idle_ofs_time();
    /* initialize the last idle ofs time stamp to 
       HDRSRCHOFS_IDLE_OFS_INTERVAL_MS before the current time 
       so that we do not prevent an OFS search due to OFS 
       interarrival duration limitation */
       
  hdrsrchofs.return_state = HDRSRCH_NO_STATE;
    /* don't know which state to return to yet after idle ofs */
       
  hdrsrchofs.ofs.required = FALSE;
    /* reset OFS required flag */
       
#ifdef FEATURE_HDR_DDARF
  hdrsrchofs.ofs.idle_ofs_request = FALSE;
    /* reset flag indicating that idle OFS has been requested; regardless of
       whether signal conditions for idle OFS is satisfied or not. */
#endif /* FEATURE_HDR_DDARF */

  hdrsrchofs_reset_repeat_count();
    /* Reset OFS and RUP repeat counts */
    
  hdrsrchofs_clear_rup_req();
    /* clear any pending RUP request */
    
  hdrsrchofs_reset_rup_report_pending();
    /* reset pending RUP search report flag */
           
#ifdef HDRSRCH_1X_SEARCH_TEST
  hdrsrchofs.ofs.system_mask = HDRSRCHOFS_BOTH_DO_AND_1X_SYSTEM_OFS;
    /* Enable both 1x and DO searches */
#else
  hdrsrchofs.ofs.system_mask = HDRSRCHOFS_DO_SYSTEM_OFS_ONLY;
    /* Enable only DO searches */
#endif /* HDRSRCH_1X_SEARCH_TEST */
               
} /* hdrsrchofs_init_ofs_timeline() */

/*===========================================================================

FUNCTION HDRSRCHOFS_REGISTER_IDLE_OFS_RETURN_STATE

DESCRIPTION

  This function specifies which state to return to after 
  performing idle OFS.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchofs_register_idle_ofs_return_state
(
  hdrsrch_state_enum_type           return_state
    /* specific state to return to after idle OFS */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    
  hdrsrchofs.return_state = return_state;
    /* register specific state to return to after idle OFS */

} /* hdrsrchofs_register_idle_ofs_return_state() */

/*===========================================================================

FUNCTION HDRSRCHOFS_PILOT_MEAS_PENDING

DESCRIPTION
  This function returns the flag that whether
  there is a pilot measurement request pending.

DEPENDENCIES
  None

RETURN VALUE
  Whether there is pending pilot measurement request

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchofs_pilot_meas_pending( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ( hdrsrchofs.rup.pending );  
    /* If there is any pending pilot measurement request or RUP request,
       this pilot measurement won't be allowed  */
} /* hdrsrchofs_pilot_meas_pending() */

/*===========================================================================

FUNCTION HDRSRCHOFS_CLEAR_PILOT_MEAS_REQ

DESCRIPTION
  This function cancels any pending pilot meas request with OFS state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchofs_clear_pilot_meas_req( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchofs.rup.pending = FALSE;
    /* reset flag that a RUP/pilot meas request is pending */

  hdrsrchofs.rup.pilot_meas = FALSE;
    /* reset flag that this is a pilot measurement request */ 

}  /* hdrsrchofs_clear_pilot_meas_req( ) */


#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
/*===========================================================================

FUNCTION HDRSRCHOFS_MEAS_STMR_TO_HDR_RTC

DESCRIPTION
  This function converts a timestamp in LTE STMR to hdr rtc count.

DEPENDENCIES
  None

RETURN VALUE
  Timestamp as hdr rtc count (chipx8)

SIDE EFFECTS
  None

===========================================================================*/

uint32 hdrsrchofs_meas_stmr_to_hdr_rtc
( 
  uint32        time_in_lte_stmr
    /* A timestamp in LTE STMR */
)
{
  hdrsrch_cdma_and_lte_time_struct    time_snapshot;
    /* structure to store snapshot of cdma time and lte time */

  uint64                              time_passed_in_cdma_sys_time = 0;
    /* time passed from last snapshot in cdma system time */

  uint32                              time_in_hdr_rtc;
    /* timestamp in hdr rtc count */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchtimesync_get_time_snapshot( &time_snapshot );

  /* Get time passed since the snapshot is captured */
  time_in_lte_stmr += HDRSRCHOFS_MEAS_LTE_STMR_WRAP;
  time_in_lte_stmr -= time_snapshot.lte_stmr;
  time_in_lte_stmr &= HDRSRCHOFS_MEAS_LTE_STMR_MASK;
    /* Calculate time delta */
  time_passed_in_cdma_sys_time = time_in_lte_stmr / HDRSRCH_LTE_TO_HDR_CHIP_RATE;
    /* LTE chip rate is 30.72 MHZ, CDMA chip rate is 1.2288 MHZ. 
       1.2288 * 25 = 30.72 */

  time_in_hdr_rtc = ( time_passed_in_cdma_sys_time & 0x7fff ) << 3 ;
    /* Time passed in rtc count chipx8. HDR rtc counter is running at
       chipx8 with a period of 32768 chips  */

  time_in_hdr_rtc += time_snapshot.hdr_rtc;
  time_in_hdr_rtc &= 0x3ffff;
    /* get timestamp in hdr rtc chipx8 */

  return time_in_hdr_rtc;
}/* hdrsrchofs_meas_stmr_to_hdr_rtc */

/*===========================================================================

FUNCTION HDRSRCHOFS_MEAS_STMR_TO_CDMA_SYS_TIME

DESCRIPTION
  This function converts a timestamp in LTE STMR to CDMA sytem time.

DEPENDENCIES
  None

RETURN VALUE
  Timestamp as CDMA system time

SIDE EFFECTS
  None

===========================================================================*/

uint64 hdrsrchofs_meas_stmr_to_cdma_sys_time
( 
  uint32        time_in_lte_stmr
    /* A timestamp in LTE STMR */
)
{
  hdrsrch_cdma_and_lte_time_struct    time_snapshot;
    /* structure to store snapshot of cdma time and lte time */

  uint64                              time_in_cdma_sys_time = 0;
    /* timestamp in cdma system time */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Time transfer was successfully done */
  hdrsrchtimesync_get_time_snapshot( &time_snapshot );

  time_in_lte_stmr += HDRSRCHOFS_MEAS_LTE_STMR_WRAP;
  time_in_lte_stmr -= time_snapshot.lte_stmr;
  time_in_lte_stmr &= HDRSRCHOFS_MEAS_LTE_STMR_MASK;
  time_in_cdma_sys_time = time_in_lte_stmr / HDRSRCH_LTE_TO_HDR_CHIP_RATE;
    /* LTE chip rate is 30.72 MHZ, CDMA chip rate is 1.2288 MHZ. 
       1.2288 * 25 = 30.72 */
  time_in_cdma_sys_time += time_snapshot.cdma_sys_time;

  return time_in_cdma_sys_time;
}/* hdrsrchofs_meas_stmr_to_cdma_sys_time */

/*===========================================================================

FUNCTION HDRSRCHOFS_MEAS_DETERMINE_AGC_CFG

DESCRIPTION
  This function determines agc acquisition mode.

DEPENDENCIES
  None

RETURN VALUE
  Array index of item whose channel is being measured currently

SIDE EFFECTS
  None

===========================================================================*/

int hdrsrchofs_meas_determine_agc_cfg( void )
{
  int                 i;
    /* array index */

  int                 curr_agc_idx;
    /* array index of agc accum for current channel */

  int                 oldest_agc_index = 0;
    /* array index of oldest rx agc accum */

  uint64              oldest_time_for_agc;
    /* timestamp of the oldest rx agc accum */

  uint64              gap_start_in_cdma_sys_time;
    /* timestamp of the oldest rx agc accum */

  hdrsrch_cdma_and_lte_time_struct    time_snapshot;
    /* structure to store snapshot of cdma time and lte time */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchofs.lte_meas.gap_start == HDRSRCH_PILOT_MEAS_START_IMMEDIATE )
  {
    /* This is for idle mode. Always use fast agc mode in idle meas */

    curr_agc_idx = 0;

    /* Store the new agc acquisition timestamp */
    hdrsrchofs.lte_meas.agc_accum_list[curr_agc_idx].channel.band = 
      hdrsrchofs.lte_meas.channel.band;
    hdrsrchofs.lte_meas.agc_accum_list[curr_agc_idx].channel.chan_num = 
      hdrsrchofs.lte_meas.channel.chan_num;
    hdrsrchofs.lte_meas.agc_accum_list[curr_agc_idx].prev_agc_accum = 
            HDRSRCHOFS_MEAS_AGC_ACCUM_INIT;

    hdrsrchtimesync_get_time_snapshot( &time_snapshot );
    hdrsrchofs.lte_meas.agc_accum_list[curr_agc_idx].time_of_prev_agc =
       time_snapshot.cdma_sys_time;
      /* Since TranSysTime command and PilotMeas command is back to back, the
         cdma sys time at time transfer is about the agc aquisition time in
         idle pilot meas.
      */
    hdrsrchofs.lte_meas.agc_acq_mode = HDRSRCHRF_FAST_AGC_ACQ_MODE;
      /* Fast mode agc acq, need 8 half slots for agc acq/ */
  }
  else 
  {
    /* This is for connected mode pilot meas */

    for ( i = 0; i < HDRSRCHOFS_LTE_PILOT_MEAS_INTERLEAVE_CHAN_NUM; i++ )
    {
      if ( ( hdrsrchofs.lte_meas.agc_accum_list[i].channel.chan_num == 
             hdrsrchofs.lte_meas.channel.chan_num ) &&
           ( hdrsrchofs.lte_meas.agc_accum_list[i].channel.band == 
             hdrsrchofs.lte_meas.channel.band ) )
      {
        /* find the same channel in the list. This means we did pilot
           measurement for the same channel before. The last Rx AGC accum
           may be useful */
        break;
      }
    }

    gap_start_in_cdma_sys_time = 
          hdrsrchofs_meas_stmr_to_cdma_sys_time( hdrsrchofs.lte_meas.gap_start );
      /* convert gap start to cdma system time */

    if ( i < HDRSRCHOFS_LTE_PILOT_MEAS_INTERLEAVE_CHAN_NUM )
    {
      /* The same channel has been searched before */

      if ( ( gap_start_in_cdma_sys_time - 
             hdrsrchofs.lte_meas.agc_accum_list[i].time_of_prev_agc ) 
           > HDRSRCHOFS_MAX_AGC_PERIOD_FOR_RESUME_MODE )
      {
        /* From last Rx AGC calculation it is more than 213 ms. Has to use fast 
           mode*/
        hdrsrchofs.lte_meas.agc_acq_mode = HDRSRCHRF_FAST_AGC_ACQ_MODE;
        hdrsrchofs.lte_meas.agc_accum_list[i].prev_agc_accum = 
                HDRSRCHOFS_MEAS_AGC_ACCUM_INIT;
          /* No need for previous agc accum in fast agc mode. Reset it to -30 */
      }
      else
      {
        /* It is within 213ms from last agc calculation. Resume mode is good */
        hdrsrchofs.lte_meas.agc_acq_mode = HDRSRCHRF_RESUME_AGC_ACQ_MODE;
      }

      /* Update timestamp in agc_accum_list */
      hdrsrchofs.lte_meas.agc_accum_list[i].time_of_prev_agc =
         gap_start_in_cdma_sys_time;

      curr_agc_idx = i;
    }
    else /* i == HDRSRCHOFS_LTE_PILOT_MEAS_INTERLEAVE_CHAN_NUM */
    {
      /* This channel is never searched before. Find a empty item in 
         agc_accum_list or replace an oldest item */

      oldest_time_for_agc = 
        hdrsrchofs.lte_meas.agc_accum_list[oldest_agc_index].time_of_prev_agc;
        /* initialize timestamp of oldest agc */

      for ( i = 0; i < HDRSRCHOFS_LTE_PILOT_MEAS_INTERLEAVE_CHAN_NUM; i++ )
      {
        if ( hdrsrchofs.lte_meas.agc_accum_list[i].time_of_prev_agc == 0 )
        {
          oldest_agc_index = i;
            /* this item is not used yet */
          break;
        }
        else
        { 
          if ( hdrsrchofs.lte_meas.agc_accum_list[i].time_of_prev_agc 
               < oldest_time_for_agc )
          {
            /* This agc acq was done earlier */

            oldest_time_for_agc =
                hdrsrchofs.lte_meas.agc_accum_list[i].time_of_prev_agc;
            oldest_agc_index = i;
          }
        }
      }
      /* Store the new agc acquisition timestamp */
      hdrsrchofs.lte_meas.agc_accum_list[oldest_agc_index].channel.band = 
          hdrsrchofs.lte_meas.channel.band;
      hdrsrchofs.lte_meas.agc_accum_list[oldest_agc_index].channel.chan_num = 
          hdrsrchofs.lte_meas.channel.chan_num;
      hdrsrchofs.lte_meas.agc_accum_list[oldest_agc_index].prev_agc_accum = 
          HDRSRCHOFS_MEAS_AGC_ACCUM_INIT;
      hdrsrchofs.lte_meas.agc_accum_list[oldest_agc_index].time_of_prev_agc =
          gap_start_in_cdma_sys_time;

      hdrsrchofs.lte_meas.agc_acq_mode = HDRSRCHRF_FAST_AGC_ACQ_MODE;
        /* Fast mode agc acq, need 8 half slots for agc acq */

      curr_agc_idx = oldest_agc_index;
    }/* i == HDRSRCHOFS_LTE_PILOT_MEAS_INTERLEAVE_CHAN_NUM */

  } /* hdrsrchofs.lte_meas.gap_start != HDRSRCH_PILOT_MEAS_START_IMMEDIATE */

  return curr_agc_idx;
} /* hdrsrchofs_meas_determine_agc_cfg */

/*=========================================================================

FUNCTION  HDRSRCHMSG_SEND_PILOT_MEAS_CFG_MSG

DESCRIPTION 
  This function creates and sends the Pilot Meas Config message.

DEPENDENCIES
  None

RETURN VALUE 
  None

SIDE EFFECTS 
  None

=========================================================================*/

void hdrsrchofs_meas_send_pilot_meas_cfg_req_msg
(
  boolean                           start_immediately,
    /* Whether pilot measurement start immediately */

  hdrsrchrf_agc_acq_mode_enum_type  rx_agc_mode,
    /* Rx AGC acquisition mode */

  uint32                             previous_agc_accum
    /* Rx AGC accumuloator of previous gap */
)
{
  hdrfw_pilot_meas_cfg_req_msg_t            *p_pilot_meas_cfg_req;
    /* temporary pointer to pilot meas cfg request message */

  uint32                                    gap_start_in_rtc;
    /* gap start time in rtc count */        

/*-----------------------------------------------------------------------*/


  memset( &hdrsrchofs.lte_meas.fw_msg.pilot_meas_cfg_req, 
          0, 
          sizeof( hdrfw_pilot_meas_cfg_req_msg_t ) );
    /* clear the message */
  msgr_init_hdr( &hdrsrchofs.lte_meas.fw_msg.pilot_meas_cfg_req.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_PILOT_MEAS_CFG_REQ_CMD );
    /* Init message header */
  
  /* Create the message payload */
  p_pilot_meas_cfg_req = &hdrsrchofs.lte_meas.fw_msg.pilot_meas_cfg_req;

  if ( start_immediately == TRUE )
  {
    /* This is an idle mode pilot measurement */

    p_pilot_meas_cfg_req->durationInfinite = TRUE ;
      /* no limit for duration */

    p_pilot_meas_cfg_req->startImmediate = TRUE ;
      /* start pilot measurement ASAP */

    p_pilot_meas_cfg_req->idleOfflineSearch = 
                ~( hdrsrchofs.lte_meas.online_srch );
      /* Copy the online/offline search flag */

    p_pilot_meas_cfg_req->rotError = 
            hdrsrchofs.lte_meas.rot_error_accum;
      /* Firmware needs rot accumulator error */
    HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
      "IRAT Meas PilotMeasCfgReq IDLE rot_accum 0x%x offlineSearch=0x%x,durationInfinite=%d",
        hdrsrchofs.lte_meas.rot_error_accum,
        p_pilot_meas_cfg_req->idleOfflineSearch,p_pilot_meas_cfg_req->durationInfinite); 
  }
  else
  {
    /* This is a connected mode measurement, everything is within gap */

    p_pilot_meas_cfg_req->durationInfinite = FALSE;
      /* duration has limit */

    p_pilot_meas_cfg_req->startImmediate = FALSE;
      /* start pilot measurement after gap */

    p_pilot_meas_cfg_req->idleOfflineSearch = TRUE; 
      /* In connected mode we do offline search only */

    gap_start_in_rtc = 
          hdrsrchofs_meas_stmr_to_hdr_rtc( hdrsrchofs.lte_meas.gap_start );
      /* get gap_start in rtc chipx8 */
    p_pilot_meas_cfg_req->startRtcCx1 = gap_start_in_rtc >> 3;
      /* assign gap start in chipx1 */

    p_pilot_meas_cfg_req->durationRtcCx1 = 
             hdrsrchofs.lte_meas.gap_length / HDRSRCH_LTE_TO_HDR_CHIP_RATE;

    p_pilot_meas_cfg_req->rotError = 
            hdrsrchofs.lte_meas.rot_error_accum;
      /* Firmware needs rot accumulator error */

    if ( rx_agc_mode == HDRSRCHRF_RESUME_AGC_ACQ_MODE )
    {
      p_pilot_meas_cfg_req->numSamplesDesired = 2048;
        /* 2 half slots (2048 samples) captured in this gap */
    }
    else
    {
      /* Fast mode Rx AGC acquistion */
      p_pilot_meas_cfg_req->numSamplesDesired = 0;
        /* No half slot left for sample capture in this gap */
    }
  
    (void) rfm_meas_common_get_time_constant(
                           RFM_LTE_MODE,        /* source mode */
                           RFM_1XEVDO_MODE );   /* target mode */
      /* Get time constants from RF */

    p_pilot_meas_cfg_req->rfTuneBackTimeConstant = 0;
    /*  This  is  the total  tuneback  time  needed  by LTE  to  resume
     *  activity after the gap time. Set  this time constant to zero as
     *  LTE  shrinks the  gap size  to address  this requirement.  This
     *  value is specified in units of CDMA chip to firmware.
    */

    HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
      "IRAT Meas PilotMeasCfgReq CONN,offlineSearch=0x%x,cleanUpTime=%d, rot_err_accum=%d",
        p_pilot_meas_cfg_req->idleOfflineSearch,
        p_pilot_meas_cfg_req->rfTuneBackTimeConstant,
        p_pilot_meas_cfg_req->rotError );
     
    HDR_MSG_SRCH_4( MSG_LEGACY_MED, 
      "IRAT Meas PilotMeasCfgReq CONN startRTCCx1=0x%x,gapLenCx1=0x%x,numSample=0x%x,prev_agc_accum=0x%x",
       p_pilot_meas_cfg_req->startRtcCx1,
       p_pilot_meas_cfg_req->durationRtcCx1,
       p_pilot_meas_cfg_req->numSamplesDesired,
       previous_agc_accum );

  }

  hdrsrchmsg_send_msg( &hdrsrchofs.lte_meas.fw_msg.pilot_meas_cfg_req.hdr, 
                         sizeof( hdrfw_pilot_meas_cfg_req_msg_t ) );
    /* Send the message */

  hdrsrchofs.lte_meas.is_meas_cfg_req_sent = TRUE;
    /*  meas_cfg_req is sent need to send meas stop stream message to fw  */

} /* hdrsrchofs_send_pilot_meas_cfg_req_msg */

/*===========================================================================

FUNCTION HDRSRCHOFS_MEAS_INIT

DESCRIPTION
  Measurement Off Frequency Search State Initialization

DEPENDENCIES
  TimeSync state on an HDR System

RETURN VALUE
  None

SIDE EFFECTS
  TBD

===========================================================================*/

LOCAL void hdrsrchofs_meas_init( hdrsrch_state_enum_type last_state )
{

  hdrsrch_cdma_and_lte_time_struct    time_snapshot;
    /* structure to store snapshot of cdma time and lte time */

  int32              time_to_gap_start_in_ms = 0;
    /* timestamp of the oldest rx agc accum */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchofs.lte_meas.pilot_meas_req_recv_sclk = 0;
  hdrsrchofs.lte_meas.pilot_meas_req_to_gap_start_in_ms = 0;
  /* initialize pilot_meas_req_to_gap_start_in_ms */

  hdrsrch_default_init( last_state );
    /* Use default handling */

  hdrsrchofs.last_state = last_state;
    /* save the last state from where OFS was entered */

  hdrsrchofs.waiting_for_tcxo = FALSE;
    /* Initialize as though we are ready to go. */

  hdrsrchofs.ofs_reason = HDRSRCHOFS_PILOT_MEAS ;
  hdrsrchofs.lte_meas.wait_for_gap = FALSE;
  hdrsrchofs.lte_meas.abort_pilot_meas = FALSE;
#ifdef FEATURE_HDR_IRAT_DEBUG  
  hdrsrchofs.lte_meas.skip_gap = 0;  
    /* It is very useful to debug IRAT issue when skipping gap */
#endif /* FEATURE_HDR_IRAT_DEBUG */
  /* This is pilot measurement after time synce. We need to configure
     firmware, srch4, and others first */

  hdrsrchofs.lte_meas.is_meas_cfg_req_sent = FALSE;
    /* Reset flag */

  hdrmdsp_fws_resume_app();
    /* wake up FW */

  /* Put HDR FW into meas or meas_connected mode won't reset hdr rtc. This is
     the agreement between HDR srch and FW. hdr rtc is started in timesync_init
     via poking data_path register.
  */

#ifdef FEATURE_HDR_IRAT_DEBUG  
  if ( hdrsrchofs.lte_meas.skip_gap != 1 )
  {
    hdrmdsp_config_hdr_app( HDRFW_STATE_MEAS_CONNECTED, TRUE, FALSE );

        /* Enable SRCH4 for HDR */
  }
#else
  hdrmdsp_config_hdr_app( HDRFW_STATE_MEAS_CONNECTED, TRUE, FALSE );
#endif /* FEATURE_HDR_IRAT_DEBUG */

  hdrsrchlog_enable_irat_fw_logging();
      /* Enable RX_AGC, AFC, RX_IQ FW log pkts if log mask is enabled */

  hdrsrchdrv_enable_srch4( SRCH4_PRIO_HDR_OFREQ );
      /* Enable SRCH4 for HDR */

  hdrsrchafc_set_fll_chan( hdrsrchofs.lte_meas.channel );
    /* Init FLL channel. arrier freq is needed when calculate rot error */

  hdrsrch_add_to_wait_mask( HDRSRCH_DUMP_SIG | HDRSRCH_LOSTDUMP_SIG );
    /* Wait for Dump signals, and perhaps Lost Dump signals */

  hdrsrchtimesync_get_time_snapshot( &time_snapshot );

  if ( hdrsrchofs.lte_meas.gap_start == HDRSRCH_PILOT_MEAS_START_IMMEDIATE )
  {
    /* Even though the request from LTE indicates IMMEDIATE, insert a 1ms delay to
       avoid FW */
    hdrsrchofs.lte_meas.pilot_meas_req_to_gap_start_in_ms = 
      HDRSRCHOFS_FIXED_MEAS_TIME_DELAY_MS;
      /* Save delay for debugging purpose */

    hdrsrchofs.lte_meas.wait_for_gap = TRUE;
      /* Set the flag as we are waiting */

    (void) rex_set_timer
      ( 
        &hdrsrch.gp_timer0, 
        ( hdrsrchofs.lte_meas.pilot_meas_req_to_gap_start_in_ms ) 
      );
    hdrsrch_add_to_wait_mask( HDRSRCH_GP_TMR0_SIG );
      /* When timer expires, we will call meas_gap_process */

    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "IRAT Meas: Insert %d ms delay to Idle meas",
                    HDRSRCHOFS_MEAS_TIME_TO_CONFIG_BEFORE_GAP ); 
  }
  else
  {
    /* This is a connected mode measurement. Calculate how long from now to
       gap start */ 

    time_to_gap_start_in_ms = 
        ( ( hdrsrchofs.lte_meas.gap_start + HDRSRCHOFS_MEAS_LTE_STMR_WRAP
            - time_snapshot.lte_stmr ) & HDRSRCHOFS_MEAS_LTE_STMR_MASK ) / 30720 ;
      /* Calculate the distance to gap start, with consideration of LTE STMR wrap
         around. And LTE chip rate is 30.72 MHZ */

    if ( time_to_gap_start_in_ms >= HDRSRCHOFS_MEAS_TIME_TO_CONFIG_BEFORE_GAP )
    {
      /* Wait within HDRSRCHOFS_MEAS_TIME_TO_CONFIG_BEFORE_GAP ms before
       * gap start, to config firmware and RF */

      hdrsrchdrv_set_burst_len( HDR_SRCH_LONG_BURST_LENGTH );
        /* Set the burst length for Connected mode measurement srch request */

      hdrsrchofs.lte_meas.pilot_meas_req_to_gap_start_in_ms = time_to_gap_start_in_ms;
      hdrsrchofs.lte_meas.pilot_meas_req_recv_sclk = timetick_get();
      /* store the timestamp of receiving pilot_meas req */


      hdrsrchofs.lte_meas.wait_for_gap = TRUE;
        /* Set the flag */
      (void) rex_set_timer
        ( 
          &hdrsrch.gp_timer0, 
          (time_to_gap_start_in_ms - HDRSRCHOFS_MEAS_TIME_TO_CONFIG_BEFORE_GAP ) 
        );
      hdrsrch_add_to_wait_mask( HDRSRCH_GP_TMR0_SIG );
        /* When timer expires, we will call meas_gap_process */
    }
    else
    {
      /* Receiving pilot meas request is too close to gap start, there is no enough 
         time to configure RF and firmware. */

      hdrsrchmsg_send_lte_pilot_meas_rsp( 
        PILOT_MEAS_CONFIG_TIME_TOO_SHORT,
        hdrsrchofs.lte_meas.channel );
        /* Send response with error status */
      hdrsrchofs_exit_ofs();
        /* Exit ofs meas state */
    }
  }

} /* hdrsrchofs_meas_init( ) */

/*===========================================================================

FUNCTION HDRSRCHOFS_MEAS_STORE_CONFIG_PARAMS

DESCRIPTION
  Store parameters related to IRAT Measurement configuration

DEPENDENCIES
  None 
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchofs_meas_store_config_params
(
    const hdrsrch_lte_pilot_meas_params_type *params 
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Store the parameters from LTE to local structure */
  hdrsrchofs.lte_meas.rf_params = params->lte_rfm_param.rfm_common_meas_param;
  hdrsrchofs.lte_meas.gap_start = params->gap_start;
  hdrsrchofs.lte_meas.gap_length = params->gap_length;
  hdrsrchofs.lte_meas.online_srch = params->online_srch;
  hdrsrchofs.lte_meas.son_search = params->son_search;
} /* hdrsrchofs_meas_store_config_params( ) */

/*=========================================================================

FUNCTION     : HDRL1_IRAT_LTE_MEAS_ENTER

DESCRIPTION  : This function initializes the target fields of the RF
               common structure for enter operation and triggers RF
               enter procedure

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Executes enter procedure to be followed by build scripts
               and exit. Always enter-build scripts-exit sequence must
               be followed by source and target RAT for RF state to be
               in coherent state

=========================================================================*/

void hdrl1_irat_lte_meas_enter
(
  hdrl1_irat_lte_rfm_param_type   *rf_params
    /* All the RF parameters needed to trigger enter procedure */
)
{
  rfm_meas_enter_param_type rfm_enter;
  rfm_meas_result_type      ret_val;
  
  rf_params->rfm_common_meas_param->header.target_tech = RFCOM_1XEVDO_MODE;
  rf_params->rfm_common_meas_param->header.num_target_item = 1;
  rf_params->rfm_common_meas_param->header.
    target_param_v2[0].band =
    rf_params->target.channel.band;
  rf_params->rfm_common_meas_param->header.
    target_param_v2[0].rx_dev_handle[0].device =
    rf_params->target.rf_device;
  rf_params->rfm_common_meas_param->header.
    target_param_v2[0].rx_dev_handle[0].lm_handle =
    rf_params->target.rxlm_handle_hdr_0;
  rf_params->rfm_common_meas_param->target_param.hdr_params.band_chan =
                                    rf_params->target.channel;
    /* Add band and channel details */
  
  hdrsrchofs.lte_meas.channel = rf_params->target.channel;
  rfm_enter.enter_params = rf_params->rfm_common_meas_param;
  ret_val = rfm_meas_common_script_enter(&rfm_enter, NULL);
  if(ret_val == RFM_MEAS_COMMON_FAILURE)
  {
    ERR_FATAL("Unexpected rf status %d: please check RF cal", 
                ret_val, 0, 0);
  }
}

/*=========================================================================

FUNCTION     : HDRL1_IRAT_LTE_MEAS_EXIT

DESCRIPTION  : This function initializes the target fields of the RF
               common structure for exit operation and triggers RF
               exit procedure

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Executes exit procedure followed by enter and build
               scripts. Always enter-build scripts-exit sequence must
               be followed by source and target RAT for RF state to be
               in coherent state

=========================================================================*/

void hdrl1_irat_lte_meas_exit
(
  hdrl1_irat_lte_rfm_param_type   *rf_params
    /* All the RF parameters needed to trigger enter procedure */
)
{
  rfm_meas_exit_param_type rfm_exit;
  rfm_meas_result_type      ret_val;
  
  rfm_exit.is_measurement_gap_aborted = rf_params->flag.is_gap_aborted;
  rfm_exit.exit_params = rf_params->rfm_common_meas_param;
  ret_val = rfm_meas_common_script_exit(&rfm_exit, NULL);
  if(ret_val == RFM_MEAS_COMMON_FAILURE)
  {
    ERR_FATAL("Unexpected rf status %d: please check RF cal", 
                ret_val, 0, 0);
  }
}

/*===========================================================================

FUNCTION HDRSRCHOFS_MEAS_START_SEARCH

DESCRIPTION
  This function start submit search task to firmware.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchofs_meas_start_search( void )
{
  
  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "IRAT Meas: Start search"); 

  hdrsrchutil_program_rup_search_set( hdrsrchofs.srch_system );

} /* hdrsrchofs_meas_start_search */

/*==========================================================================

FUNCTION        HDRSRCHOFS_MEAS_PROCESS_PILOT_MEAS_CFG_RSP

DESCRIPTION
  This function processes pilot measurement cfg response.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchofs_meas_process_pilot_meas_cfg_rsp
( 
  hdrsrchmsg_s_type         *msg 
)
{
  hdrfw_pilot_meas_cfg_rsp_msg_t     *p_pilot_meas_cfg_rsp;
    /* pointer to pilot measurement config response */

  int                                i;
    /* array index */
/*-----------------------------------------------------------------------*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "IRAT Meas Rcvd. FW msg PilotMeasCfgRsp umid 0x%8x", 
                 msg->msg.hdr.id );

  hdrsrchmsg_set_rsp_waiting( HDRSRCH_PILOT_MEAS_CFG_RSP, FALSE );
   /* Clear the flag to wait for response */

  if ( hdrsrchofs.lte_meas.abort_pilot_meas == TRUE )
  {
    /* Need to abort the measurement */

    if ( hdrsrchofs.lte_meas.online_srch == TRUE )
    {
      /* Init message header, then send. */
      msgr_init_hdr( &hdrsrchofs.lte_meas.fw_msg.pilot_meas_stop_stream_req.hdr, 
                     MSGR_HDR_SRCH, 
                     HDR_FW_PILOT_MEAS_STOP_STREAM_CMD );
      
      hdrsrchmsg_send_msg( &hdrsrchofs.lte_meas.fw_msg.pilot_meas_stop_stream_req.hdr, 
                             sizeof( hdrfw_pilot_meas_stop_stream_msg_t ) );
        /* Send the message */

      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "sent HDR_FW_PILOT_MEAS_STOP_STREAM_CMD cmd to fw" );
    }

    hdrsrchmsg_send_lte_pilot_meas_rsp( 
      PILOT_MEAS_BE_ABORTED,
      hdrsrchofs.lte_meas.channel );

	hdrsrchmsg_send_lte_pilot_meas_abort_rsp();
      /* Send Pilot Meas Abort Response */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
      "sent lte_pilot_meas_rsp_msg with abort Online/Offline-search(1/0) %d",
      hdrsrchofs.lte_meas.online_srch);

    hdrsrchofs_exit_ofs();
    return;
  }

  p_pilot_meas_cfg_rsp = &(msg->msg.hdr_msg.rsp.pilot_meas_cfg);
    /* Get the message pointer */

  if ( p_pilot_meas_cfg_rsp->status != 0  )
  {
     /* Firmware gap ends with failure */

    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
          "IRAT MEAS: Rcvd FW msg PilotMeasCfgRsp failure status = 0x%x", 
          p_pilot_meas_cfg_rsp->status );
    hdrsrchmsg_send_rf_status_ind( RF_STATUS_SAMPLE_CAPTURE_DONE );
      /* Sample capture is done */
    hdrsrchmsg_send_lte_pilot_meas_rsp( 
        PILOT_MEAS_SEARCH_FAIL,
        hdrsrchofs.lte_meas.channel 
    );
      /* Send pilot meas response with failure status */
    hdrsrchofs_exit_ofs();
    return;
  }

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
    "IRAT MEAS: PilotMeasCfgRsp agcAccum=0x%x,numSample=0x%x,timeStampRspRtcCx2=0x%x",
     p_pilot_meas_cfg_rsp->rxAgcAccum, p_pilot_meas_cfg_rsp->numSamplesCaptured,
     p_pilot_meas_cfg_rsp->timeStampRspRtcCx2);
                     
  hdrsrchrf_report_rx_power(); 

  /* Store Rx AGC accum */
  for ( i = 0; i < HDRSRCHOFS_LTE_PILOT_MEAS_INTERLEAVE_CHAN_NUM; i++ )
  {
    if ( ( hdrsrchofs.lte_meas.agc_accum_list[i].channel.chan_num ==
          hdrsrchofs.lte_meas.channel.chan_num ) &&
         ( hdrsrchofs.lte_meas.agc_accum_list[i].channel.band ==
          hdrsrchofs.lte_meas.channel.band ) )
    {
      hdrsrchofs.lte_meas.agc_accum_list[i].prev_agc_accum = 
        (int16) ( p_pilot_meas_cfg_rsp->rxAgcAccum & 0x0000FFFF );
           /* convert returned uint32 to int16 */
    }
  }

  if ( hdrsrchofs.lte_meas.online_srch == FALSE )
  {
    /* Send RF Release Indication now for offline search */
    if ( p_pilot_meas_cfg_rsp->numRxAgcHalfSlots == 
             HDRSRCHOFS_MEAS_FIRST_GAP_AGC_HS )
    {
      /* This should be for first gap in connected mode */

      hdrsrchmsg_send_rf_status_ind( RF_STATUS_SAMPLE_CAPTURE_NOT_DONE );
        /* Has not done sample capture */
  
      hdrsrchmsg_send_lte_pilot_meas_rsp( 
        PILOT_MEAS_NOT_SRCH_YET,
        hdrsrchofs.lte_meas.channel );
        /* Send pilot meas response with NOT_SRCH_YET status */
      hdrsrch_timed_wait_usecs( HDRSRCH_OFS_MEAS_WAIT_GAP_END_US );
        /* Wait 1ms here. Firmware may not finish its gap end processing now.
           It would crash if we exit ofs and put it to standby mode
        */
      hdrsrchofs_exit_ofs();
    }
    else
    {
      /* This is the second gap */

      hdrsrchmsg_send_rf_status_ind( RF_STATUS_SAMPLE_CAPTURE_DONE );
        /* Sample capture is done */

      if ( p_pilot_meas_cfg_rsp->numRxAgcHalfSlots == 
               HDRSRCHOFS_MEAS_RESUME_AGC_HS  )
      {
        /* There are 2 half slots for samples */

        if ( p_pilot_meas_cfg_rsp->numSamplesCaptured >= 2048 )
        {
          hdrsrchdrv_set_srch4_param(SRCH4_MODE_IRAT,
                              p_pilot_meas_cfg_rsp->streamingStopRtcCx2);
          /* set RTC count in CX2 at which sample capture is done
             and next IRAT search mode */
          hdrsrch_on_searcher_idle( hdrsrchofs_meas_start_search );
            /* Sample capture is done. We are ready for search */
        }
        else
        {
          ERR_FATAL("IRAT MEAS: Not enough samples to search", 0, 0, 0 );
            /* We should never see it happens */
        }
      }
    }
  }
  else
  {
    hdrsrchdrv_set_srch4_param(SRCH4_MODE_DEFAULT,
                            p_pilot_meas_cfg_rsp->streamingStopRtcCx2);
    /* set RTC count in CX2 at which sample capture is done and next
       IRAT search mode */

    /* For online search, RF can not be released during search. Start search now
     */
    hdrsrchofs_meas_start_search();
  }

} /* hdrsrchofs_meas_process_pilot_meas_cfg_rsp */

/*===========================================================================

FUNCTION HDRSRCHOFS_MEAS_PROCESS_MEAS_DUMP

DESCRIPTION
  This function processes the search dump for IRAT pilot measurement.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchofs_meas_process_meas_dump( void )
{
  boolean                              valid;
  /* Flag to indicate whether or not searcher results are valid */

  hdrsrch_lte_pilot_meas_status_type   pilot_meas_status;
  /* pilot measurement status */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrmdsp_config_hdr_app( HDRFW_STATE_STANDBY, TRUE, FALSE );
  /* Put DO fw in standby first then send pilot_meas_rsp */

  /* Are we awaiting to process abort request from LTE? */
  if(  hdrsrchofs.lte_meas.abort_pilot_meas == TRUE )
  {
    /* Send pilot meas rsp with abort status */
    hdrsrchmsg_send_lte_pilot_meas_rsp(
      PILOT_MEAS_BE_ABORTED,
      hdrsrchofs.lte_meas.channel );

    /* Send abort rsp in response to LTE abort req */
    hdrsrchmsg_send_lte_pilot_meas_abort_rsp();
    /* Send Pilot Meas Abort Response */

    if ( hdrsrchofs.lte_meas.online_srch == FALSE )
    {
      /* Idle measurement offline search or connected mode measurement,
       * no need to wait for STOP_STREAM_RESP. Exit ofs here
       */
      hdrsrchofs_exit_ofs();
    }

    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "IRAT: Pilot Meas aborted");
    return;
  }

  /* check if we did full PN search and triage the result */
  hdrsrchutil_chk_triage_full_pn();

  valid = hdrsrchutil_validate_searcher_results();
      /* Perform searcher result validation before using the results */

  if ( valid == FALSE )
  {
    hdrsrchutil_invalidate_search_dump();
      /* invalidate searcher results */
    pilot_meas_status = PILOT_MEAS_SEARCH_FAIL;
      /* This is an invalid search */
  }
  else
  {
    hdrsrchutil_process_ofs_search_dump_for_pilot_meas();
      /* calculate pilot energy */

    hdrsrchofs_clear_pilot_meas_req();
      /* the search for pilot measuremnt is concluded */

    pilot_meas_status = PILOT_MEAS_SUCCESS;
     /* This is a successful search */
  }

  hdrsrchofs.lte_meas.pilot_meas_status = pilot_meas_status;
  /* Store the status for later usage */

  HDR_MSG_SRCH_2( MSG_LEGACY_LOW,
                  "Preparing pilot meas report %d %d",
                  hdrsrchofs.lte_meas.son_search,
                  hdrsrchset.rupset_cnt);

  hdrsrchmsg_prepare_lte_pilot_meas_rsp_report(
    pilot_meas_status,
    hdrsrchofs.lte_meas.channel );

  /* Perform SON search */
  if ( ( valid == TRUE ) && ( hdrsrchofs.lte_meas.son_search == TRUE ) )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_LOW,
                    "IRAT starting SON Srch %d ",
                    hdrsrchmsg_lte.lte_pilot_meas_rsp.sector_count );
    hdrsrchofs_meas_perform_son_search(
      &hdrsrchmsg_lte.lte_pilot_meas_rsp,
      hdrsrchmsg_lte.lte_pilot_meas_rsp.sector_count);
    HDR_MSG_SRCH_1( MSG_LEGACY_LOW,
                    "IRAT ending SON Srch %d ",
                    hdrsrchmsg_lte.lte_pilot_meas_rsp.sector_count );
  }

  hdrsrchmsg_send_msg( &hdrsrchmsg_lte.lte_pilot_meas_rsp,
                       sizeof( hdrsrch_lte_pilot_meas_rsp_msg_t ) );
  /* Send Pilot Measurement Response */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                  "sent lte_pilot_meas_rsp_msg Online/Offline-search(1/0) %d",
                  hdrsrchofs.lte_meas.online_srch);

  /* After sending pilot meas report exit from OFS state */
  hdrsrchofs_exit_ofs( );
} /* hdrsrchofs_meas_process_meas_dump( ) */

/*==========================================================================

FUNCTION        HDRSRCHOFS_MEAS_PROCESS_LOW_PRI_RSP_IND

DESCRIPTION
  This function processes low priority responses and indications from FW.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchofs_meas_process_low_pri_rsp_ind
(
   hdrsrchmsg_s_type         *msg
    /* Message */
)
{

/*-----------------------------------------------------------------------*/

  switch( msg->msg.hdr.id )
  {
    case HDR_FW_PILOT_MEAS_CFG_RSP:

      hdrsrchofs_meas_process_pilot_meas_cfg_rsp( msg );
        /* Process slam rsp for acq */

      break;

    case HDR_FW_PILOT_MEAS_STOP_STREAM_RSP:
      if ( hdrsrchofs.lte_meas.abort_pilot_meas == FALSE )
      {
        hdrsrchofs_meas_process_meas_dump();
          /* Abort pilot meas would have already done 
          in processing cfg rsp or in ofs_dump, ignore stop response for aboer case */
      }

      break;

    default:
      hdrsrch_default_process_low_pri_rsp_ind( msg );
      break;
  }
} /* hdrsrchofs_meas_process_low_pri_rsp_ind */

/*=========================================================================

FUNCTION     : HDRSRCHOFS_MEAS_PERFORM_SON_SEARCH

DESCRIPTION  : This function searches for sectors with energy better than
               its neighboring sector for sectors in the list

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

LOCAL void hdrsrchofs_meas_perform_son_search
(

  hdrsrch_lte_pilot_meas_rsp_msg_t      *srch_list,
    /* structure to store pilot information in */

  uint16                             srch_cnt
    /* search count in the list */

)
{
  uint8                           s = 0,
                                  srch_sect_count = 0;
    /* pilot peak and search list loop index */

  hdrsrch_pilot_strength_type     *left_pilot_ptr = NULL,
                                  *right_pilot_ptr = NULL,
                                  *center_pilot_ptr = NULL;
  hdrsrch_pilot_strength_type     *left_pilot_db_ptr = NULL,
                                  *center_pilot_db_ptr = NULL,
                                  *right_pilot_db_ptr = NULL;
    /* Neighboring pilot strengths */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if we have atleast 3 cells to perform SON srch */
  if ( srch_cnt < 3 )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
        "Unable to perform SON srch for %d items", srch_cnt);
    return;
  }

  srch_sect_count = MIN ( ( srch_cnt ), HDRSRCH_LTE_SECTOR_LIST_MAX_SIZE ) - 1;
  /* Limit the searching to ignore the last element */

  HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "sect count %d", srch_sect_count);

  for ( s = 1; s < srch_sect_count; s++ )
  {
    left_pilot_ptr =    &(srch_list->strength_lin_list[s - 1]);
    center_pilot_ptr =  &(srch_list->strength_lin_list[s]);
    right_pilot_ptr =   &(srch_list->strength_lin_list[s + 1]);

    left_pilot_db_ptr =    &(srch_list->strength_db_list[s - 1]);
    center_pilot_db_ptr =  &(srch_list->strength_db_list[s]);
    right_pilot_db_ptr =   &(srch_list->strength_db_list[s + 1]);
    /* Assign neighboring pilot strength */

    HDR_MSG_SRCH_3 ( MSG_LEGACY_LOW, "Comparing seq %d %d %d", s - 1, s, s + 1);

    HDR_MSG_SRCH_3 ( MSG_LEGACY_LOW,
                    "Comparing sect %d %d %d",
                    srch_list->pn_list[s - 1],
                    srch_list->pn_list[s],
                    srch_list->pn_list[s + 1] );

    HDR_MSG_SRCH_3 ( MSG_LEGACY_LOW,
                    "Comparing eng %d %d %d",
                    *left_pilot_ptr, *center_pilot_ptr, *right_pilot_ptr );

    if ( *center_pilot_ptr > HDR_SRCH_SON_MIN_PATH_ENG )
    {
      if ( *center_pilot_ptr >  *left_pilot_ptr )
      {
        if ( *center_pilot_ptr >= *right_pilot_ptr )
        {
          HDR_MSG_SRCH_3( MSG_LEGACY_LOW,
                          "left %d center %d right %d",
                          *left_pilot_ptr,
                          *center_pilot_ptr,
                          *right_pilot_ptr);

          *left_pilot_ptr = *right_pilot_ptr = 0;
          *left_pilot_db_ptr = *right_pilot_db_ptr =
                                 hdrsrchset_eng_to_ecio_ceiling_convert ( 0 );
          s++;

          HDR_MSG_SRCH_3( MSG_LEGACY_LOW,
                          "left %d center %d right %d",
                          *left_pilot_ptr,
                          *center_pilot_ptr,
                          *right_pilot_ptr);

        } /* if ( *center_pilot_ptr >= *right_pilot_ptr ) */
      } /* if ( *center_pilot_ptr >  *left_pilot_ptr ) */
    } /* if ( *center_pilot_ptr > HDR_SRCH_SON_MIN_PATH_ENG ) */
    else
    {
      *center_pilot_ptr = 0;
      *center_pilot_db_ptr = hdrsrchset_eng_to_ecio_ceiling_convert ( 0 );
    }
  } /* for ( s = 1; s < srch_sect_count; s++ ) */
  /* Always discard first and last values */
  srch_list->strength_lin_list[0] = 0;
  srch_list->strength_lin_list[srch_sect_count] = 0;
  srch_list->strength_db_list[0] = hdrsrchset_eng_to_ecio_ceiling_convert ( 0 );
  srch_list->strength_db_list[srch_sect_count] =
                                hdrsrchset_eng_to_ecio_ceiling_convert ( 0 );
  for ( s = 0; s <= srch_sect_count; s++ )
  {
    HDR_MSG_SRCH_2 ( MSG_LEGACY_LOW,
                     "sect %d eng %d", s,
                     srch_list->strength_lin_list[s] );
  }
} /* hdrsrchofs_meas_perform_son_search */


/*===========================================================================

FUNCTION HDRSRCHOFS_MEAS_DUMP

DESCRIPTION
  This function handles the search dump for IRAT pilot measurement.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchofs_meas_dump( void )
{
  HDR_MSG_SRCH_3( MSG_LEGACY_MED, " IRAT Meas Dump: Srched sys %d pilots[pn=%d e=%d]",
                    hdrsrchdrv.system,
                    hdrsrchdrv.srch_list[0].srch_task.pn_offset,
                    hdrsrchdrv.srch_list[0].srch_task.res_ptr->eng[0] );

  /* Are the srch dumps from online search? */
  if ( hdrsrchofs.lte_meas.online_srch == TRUE )
  {
    /* For ONLINE search we need to send STOP STREM request before
     * processing search results
     */

    /* Send RF release indication here */
    hdrsrchmsg_send_rf_status_ind( RF_STATUS_SAMPLE_CAPTURE_DONE );
      /* Sample capture is done */

    /* Init message header, then send. */
    msgr_init_hdr( &hdrsrchofs.lte_meas.fw_msg.pilot_meas_stop_stream_req.hdr, 
                   MSGR_HDR_SRCH, 
                   HDR_FW_PILOT_MEAS_STOP_STREAM_CMD );

    hdrsrchmsg_send_msg( &hdrsrchofs.lte_meas.fw_msg.pilot_meas_stop_stream_req.hdr, 
                           sizeof( hdrfw_pilot_meas_stop_stream_msg_t ) );
      /* Send the message */

    if(  hdrsrchofs.lte_meas.abort_pilot_meas == TRUE )
    {
      hdrsrchmsg_send_lte_pilot_meas_rsp( 
        PILOT_MEAS_BE_ABORTED,
        hdrsrchofs.lte_meas.channel );
      
      hdrsrchmsg_send_lte_pilot_meas_abort_rsp();
        /* Send Pilot Meas Abort Response */
      
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
        "sent lte_pilot_meas_rsp_msg with abort Online/Offline-search(1/0) %d",
        hdrsrchofs.lte_meas.online_srch);
      
      hdrsrchofs_exit_ofs();
    }
  }
  else
  {
    /* Process srch results for OFFLINE search */
    hdrsrchofs_meas_process_meas_dump();
  }
  hdrsrchdrv_set_burst_len( HDR_SRCH_LONG_BURST_LENGTH );
  /* Reset the burst length for future srch requests */

  return;
} /* hdrsrchofs_meas_dump( ) */

/*===========================================================================

FUNCTION HDRSRCHOFS_MEAS_AGC_LIST_INIT

DESCRIPTION
  This function initialize the agc accum list.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchofs_meas_agc_list_init( void )
{
  int       i;
    /* array index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize array for agc accumulator value */
  for ( i = 0; i < HDRSRCHOFS_LTE_PILOT_MEAS_INTERLEAVE_CHAN_NUM; i++ )
  {
    hdrsrchofs.lte_meas.agc_accum_list[i].channel.band = 0;
    hdrsrchofs.lte_meas.agc_accum_list[i].channel.chan_num = 0;
    hdrsrchofs.lte_meas.agc_accum_list[i].prev_agc_accum = 
              HDRSRCHOFS_MEAS_AGC_ACCUM_INIT;
      /* Initialize agc_accum to -30 */
    hdrsrchofs.lte_meas.agc_accum_list[i].time_of_prev_agc = 0;
  }

} /* hdrsrchofs_meas_agc_list_init */

/*===========================================================================

FUNCTION HDRSRCHOFS_MEAS_GAP_PROCESS

DESCRIPTION
  This function starts processing for gap -- config rf and firmware.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchofs_meas_gap_process( void )
{
#ifdef FEATURE_HDR_REVB
  hdrsrch_demod_idx_type            meas_demod_idx = HDRSRCH_DEMOD_IDX_0;
    /* Demod index used in the OFS by default is demod0 */
#endif /* FEATURE_HDR_REVB */
   sys_channel_type                 meas_channel_num; 

   int                                                curr_agc_idx;
     /* array index */

   boolean                                            start_immediately = FALSE;
     /* Whether gap start immediately -- for idle mode */
  
   rfm_meas_result_type                               rf_result = 
                                                      RFM_MEAS_COMMON_FAILURE;
     /* Whether build script succeeds or not */

   boolean                                            has_lock;
     /* Whether HDR got the TRM lock, only for SVLTE+SHDR */

   timetick_type                                      pilot_meas_req_to_now;
    /* Slow clock counter value for current time */

   int16                                              curr_to_gap_ms;
     /* current to gap (might be negative if current is after gap start) in ms */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   hdrsrchofs.lte_meas.wait_for_gap = FALSE;
     /* Reset the flag */

   hdrsrchutil_generate_pilot_meas_search_set( HDRSRCH_SYSTYPE_DO );
     /* Create RUP set for search */

  curr_agc_idx = hdrsrchofs_meas_determine_agc_cfg();
    /* Decide Rx AGC acquistion mode */

  hdrsrchofs.lte_meas.rf_params->target_param.hdr_params.agc_rtc_offset =
                                    hdrsrchtimesync_get_rtc_offset();

  hdrsrchofs.lte_meas.rf_params->target_param.hdr_params.agc_mode =
    ( rfm_hdr_agc_mode_type ) hdrsrchofs.lte_meas.agc_acq_mode;

  hdrsrchofs.lte_meas.rf_params->target_param.hdr_params.prev_agc =
    hdrsrchofs.lte_meas.agc_accum_list[curr_agc_idx].prev_agc_accum;
  if ( hdrsrchofs.lte_meas.gap_start == HDRSRCH_PILOT_MEAS_START_IMMEDIATE )
  {
    start_immediately = TRUE;
      /* Idle mode measurement, measurement should start ASAP */
  }
  else
  {
    start_immediately = FALSE;
      /* Connected mode measuremnt, wait gap start */
  }
  
  /* Set up internal hdrsrchrf data structure here. Otherwise we cannot pass
     the check to submit search task.
  */
  hdrsrchrf_init();

/* Set rf chain lock flag. This flag needs to be set to submit search
       task. With SVLTE + SHDR, HDR might not always have the lock */
  has_lock = hdrsrchrf_force_chain_lock( HDRSRCHRF_IRAT_MEASUREMENT,
    hdrsrchofs.lte_meas.rf_params->header.source_param_v2[0].rx_dev_handle[0].device );


  if ( has_lock )
  {
    /* call RF to build measurement script, must have lock to do it */
    rf_result = hdrsrchrf_meas_build_script( 
        hdrsrchofs.lte_meas.rf_params, 
        start_immediately );

    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "IRAT GAP, build script is done");
  }

  /* Check if the current time is still before gap start. Sometime build script
     might be blocked by 1x tune. When build script is done it is too late to start
     measurement. In this case, we want to send pilot measure resp with failure status
     back to LTE. This is only needed for connected mode meas.
  */
  if ( start_immediately == FALSE  )
  {
     pilot_meas_req_to_now = 
       timetick_get() - hdrsrchofs.lte_meas.pilot_meas_req_recv_sclk;
       /* the time from pilot meas received to now */
     pilot_meas_req_to_now = timetick_cvt_from_sclk( pilot_meas_req_to_now, T_MSEC );
       /*  the time from pilot meas received to now in ms */
     curr_to_gap_ms = hdrsrchofs.lte_meas.pilot_meas_req_to_gap_start_in_ms - pilot_meas_req_to_now;
     if ( curr_to_gap_ms < 4  )
     {
       /* We are too close to gap if current to gap start is positive and less than 4 ms.
          Probably becasue rf build script takes too long. curr_to_gap_ms might be 
          negative if current time is already after gap start point.
          We consider both cases as build script failure since we won't be able to 
          do pilot measurement.
       */
          
       rf_result = RFM_MEAS_COMMON_FAILURE; 
     }

     HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
                     "IRAT GAP MEAS:pilot_meas_to_curr=%d curr_to_gap=%d pilot_meas_to_gap_start=%d,build_script=%d",
                     pilot_meas_req_to_now, curr_to_gap_ms,
                    hdrsrchofs.lte_meas.pilot_meas_req_to_gap_start_in_ms, rf_result );
  }

  if ( has_lock && rf_result == RFM_MEAS_COMMON_SUCCESS )
  {
    /* Build RF script succeeds.  */

    hdrsrchofs.lte_meas.rot_error_accum =  hdrsrchafc_calc_curr_rot_val();
      /* Get rot error from RGS */

    hdrsrchmsg_send_rf_status_ind( RF_STATUS_GEN_SCRIPT_SUCCESS );
      /* Send RF status ind with gen script success */

    /* This is connected mode pilot measurement */
    meas_channel_num = hdrsrchofs.lte_meas.channel;
    hdrsrchrf_force_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                            &meas_channel_num,
                            &meas_demod_idx,
                            1,
                            NULL );

    /* This is connected mode pilot measurement */

#ifdef FEATURE_XO
    hdrsrchafc_xo_enable_mnd( hdrsrchofs.lte_meas.rot_error_accum,
                    hdrsrchofs.lte_meas.channel );
      /* send FLL_XO_CFG message */
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                  "IRAT idle meas XO:send fll_xo_cfg mnd enabled rot_accum %x ",
                   hdrsrchofs.lte_meas.rot_error_accum );
#endif

#ifdef FEATURE_HDR_IRAT_DEBUG  
    if ( hdrsrchofs.lte_meas.skip_gap == 1 )
    {      
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "IRAT GAP DEBUG, wait 13ms then send RFStatusInd");
      hdrsrch_timed_wait_usecs( 13000 );
      hdrsrchmsg_send_rf_status_ind( RF_STATUS_SAMPLE_CAPTURE_DONE );
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "IRAT GAP DEBUG, wait 2ms then send PilotMeasRsp");
      hdrsrch_timed_wait_usecs( 2000 );
      hdrsrchmsg_send_fake_lte_pilot_meas_rsp( 
        PILOT_MEAS_SUCCESS,
        hdrsrchofs.lte_meas.channel 
      );
      hdrsrchofs_exit_ofs();
       /* Go back to inactive state */
    }
    else
#endif /* FEATURE_HDR_IRAT_DEBUG */
    {
      hdrsrchofs_meas_send_pilot_meas_cfg_req_msg
      (
        start_immediately,
        hdrsrchofs.lte_meas.agc_acq_mode,
        (uint32) hdrsrchofs.lte_meas.agc_accum_list[curr_agc_idx].prev_agc_accum
      );
        /* Send pilot meas req to MDSP here */

      hdrsrchmsg_set_rsp_waiting( HDRSRCH_PILOT_MEAS_CFG_RSP, TRUE );
       /* Set the flag to wait for response */
    }
  }
  else
  {
    /* No TRM lock or build RF scripts failed, send indication to LTE */
    if ( has_lock )
    {
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "IRAT meas,send build script faiure");
      hdrsrchmsg_send_rf_status_ind( RF_STATUS_GEN_SCRIPT_FAIL );
        /* Send RF status ind with gen script fail */
      hdrsrchmsg_send_lte_pilot_meas_rsp( 
         PILOT_MEAS_BUILD_SCRIPT_FAIL,
         hdrsrchofs.lte_meas.channel );
       /* Send pilot meas resp with failure status */
    }
    else
    {
      hdrsrchmsg_send_rf_status_ind( RF_STATUS_NO_TRM_LOCK );
        /* Send RF status ind with TRM lock fail */
      hdrsrchmsg_send_lte_pilot_meas_rsp( 
         PILOT_MEAS_NO_TRM_LOCK,
         hdrsrchofs.lte_meas.channel );
       /* Send pilot meas resp with failure status */
    }

    hdrsrchofs_exit_ofs();
     /* Go back to inactive state */
  }

} /* hdrsrchofs_meas_gap_process */

/*===========================================================================

FUNCTION HDRSRCHOFS_MEAS_PROCESS_PILOT_MEAS_ABORT_REQ

DESCRIPTION
  This function handles pilot meas abort request.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchofs_meas_process_pilot_meas_abort_req( void )
{
   if ( hdrsrchofs.lte_meas.wait_for_gap == TRUE )
   {
     /* Search is waiting for gap. Search will configure RF and firmware 10 ms
        before gap start. During the waiting time pilot measurement could be
        aborted */

     hdrsrchofs.lte_meas.wait_for_gap = FALSE;

     hdrsrchmsg_send_lte_pilot_meas_rsp( 
       PILOT_MEAS_BE_ABORTED,
       hdrsrchofs.lte_meas.channel );

     hdrsrchmsg_send_lte_pilot_meas_abort_rsp();
       /* Send Pilot Meas Abort Response */

     HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
       "sent lte_pilot_meas_rsp_msg with abort Online/Offline-search(1/0) %d",
       hdrsrchofs.lte_meas.online_srch);

     hdrsrchofs_exit_ofs();
   }
   else
   {
     /* Not able to abort measurement */

     hdrsrchofs.lte_meas.abort_pilot_meas = TRUE;
   }

} /* hdrsrchofs_meas_process_pilot_meas_abort_req */
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

/*===========================================================================

FUNCTION HDRSRCHOFS_IS_OFS_CHANNEL_IN_ASET

DESCRIPTION
  This function returns the flag that indicates whether
  the searched channel in OFS is the same as aset channel.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  Whether it is a same channel OFS

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchofs_is_ofs_channel_in_aset( void )
{
  return ( hdrsrchofs.is_channel_in_aset );
} /* hdrsrchofs_is_same_channel_ofs */


#ifdef FEATURE_HDR_SELECTIVE_TXD
/*===========================================================================

FUNCTION HDRSRCHOFS_SCHED

DESCRIPTION
  This function handles schedule_sig to schedule search. It is needed
  for ASDiv as search maybe aborted and rescheduled.
  If reschedule comes before tune_done(), it can be ignored, as tune_done
  will kick off the search
  If it comes after tune_done(), need to resubmit search. this is done
  by emulating another tune_done().

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  Whether it is a same channel OFS

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void hdrsrchofs_sched( void )
{

  if ( hdrsrchofs.tune_done )
  {
    /* if tune already done, emulate another tunedone to trigger
       a search. */
    hdrsrchofs_tune( HDRSRCHRF_TUNE_SUCCESS );
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Stary Schedule signal recevied ignroing it...");
  }
} /* hdrsrchofs_sched */
#endif /* FEATURE_HDR_SELECTIVE_TXD */

