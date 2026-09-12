/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  H D R    S R C H    S Y N C    S T A T E

                           Search Sync Module

GENERAL DESCRIPTION
  Process the timing changes required to synchronize the mobile with
  the HDR system.

EXTERNALIZED FUNCTIONS

  hdrsrchsync_thresh_init() - Sync State thresh related initialization

REGIONAL FUNCTIONS
  hdrsrchsync_init_sync_state()

INITIALIZATION AND SEQUENCING REQUIREMENTS
  hdrsrchsync_init_sync_state() is called when the first Quick Config
  message is received after acquisition.

  Copyright (c) 2000 - 2018 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchsync.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ------------------------------------------------------- 
06/26/2018   vke     Changes to process slam response status from fw
01/30/2018   rmv     Use the right sub id when getting the LTE vstmr handle
12/12/2016   svu     Fixed KW issue
10/14/2016   vke     Changes to clear the tune away count when deactivating
                     from sync suspend state
10/27/2015   wsh     Trigger sys loss if tune fails  
10/14/2015   vke     Change typo error for hdrsrchtc_substate_enum_type
10/13/2015   rmv     Removed check in brute force acq that checks if there is
                     enough time to proceed with sync. This also fixes the cause
                     for INT_CMD_SIG getting removed
10/06/2015   wsh     Added debug code to catch INT_CMD_SIG getting removed  
08/24/2015   vke     Clear sync inversion timer before entering 
                     sync suspend state
07/29/2015   vke     Changes to implement sync trm inversion priority
07/13/2015   arm     Mainlined FEATURE_RF_SVDO_API feature.
06/26/2015   vke     Added tune_to_chan() timeout value
05/13/2015   vke     Changes to modify the RF reason to page demod
02/13/2015   vke     Promotion for CR790412
02/04/2015   wsh     Added support 2->2a transition, removed PA on requirement
02/04/2015   vke     Changes to ignore sync cmd if processing delay > SCC
10/13/2013   arm     Updated hdrsrchutil_calc_time_to_tune_away to return if
                     TA time was updated or not
03/17/2014   vke     Reset the tune away count for deactivate command
07/25/2014   bb      Chanegs to reset TA count if it reaches to MAX and next
                     state is INACTIVE
06/25/2014   vke     Stop the sync timeout timer when exiting the sync state
04/25/2014   ljl     Supported finger assignment and slam message.
04/09/2014   vke     Fix to prevent HDR to report system lost in 
                     Acq/Sync state because of GSM re-select
03/18/2014   vke     Increment the tune-away count for slam failure 
                     due to TA
05/12/2014   vke     Klockworks fix
01/22/2014   vke     Fix for limiting the sync failure due to tune-away
11/29/2013   sat     NV control for Lock Release Enhancement (LRE)
11/18/2013   vke     HDR enters sync suspend state for unlock immediate and 
                     unlock required notifications
01/21/2014   mbs     Support reading timing counters from VSTMR for BOLT
10/29/2013   sat     Reduced F3 messages (or/and) ASSERTs
08/08/2013   vke     Added handling of sync substate callback
09/03/2013   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
06/11/2013   vke     Added check for hdrsrch.cmd before writing status
06/28/2013   rkc     Only do ACQ logging when transitioning to IDLE.
06/27/2013   vke     Removed the disable tuneaway in handling cancel unlock
06/26/2013   vke     Added condition check to allow slam based on Tune-away 
                     time available
06/07/2013   vke     Added new timer and event to handle Diversity enable
05/10/2013   rkc     Added tuneaways for Lock Release Enhancement (LRE). 
05/06/2013   vke     Fix for preventing HDR CP to go to IDLE from INIT 
                     unless SRCH goes to monitor state from sync state
05/06/2013   mbs     Fixed compiler errors for BOLT build
03/19/2013   ljl     Aborted srch at slam.
04/26/2012   ljl     Handled no ref PN before slam. 
03/22/2012   wsh     Retain lock in SYNC to allow 1x paging
03/21/2012   ljl     Removed tramp.
03/19/2012   mbs     Added Dime Time Transfer changes
03/19/2012   ljl     Fixed Dime compiler errors.
02/27/2012   smd     Added updates of FTM msgr interface changes. 
02/24/2012   wsh     Fixed SM stall in SYNC state 
01/09/2012   smd     Supported dynamic acquisition diversity.
01/05/2012   wsh     Added support for SVLTE+SHDR
10/21/2011   smd     Compensated the delay of RF path for time transfer. 
05/12/2011   ljl     Removed sleep clk estimate.
06/13/2011   ssu     Replaced time_sync_time_of_day() with 
                     hdrts_update_ts_time() in hdrsrchsync_on_time_valid(). 
02/12/2011   grl     Resolved HSTR wrap around syncing scenario.
01/18/2010   smd     Added F3 message for sync time out in RTT.
12/15/2010   grl     Added work around for the case the frame interrupt does
                     not fire due to a tramp issue.
10/28/2010   kss     Handled case where hdrts_set occurs in next SCC cycle.
09/27/2010   smd     Fixed a bug of using uint16 to store cdma sys time. 
07/15/2010   smd     Fixed bugs when getting pair of cdma sys time/lte ostmr. 
06/29/2010   vlc     Fixed compiler warnings.
06/25/2010   smd     Supported hstr slam in slam cfg message.
                     Added fix for reverse time transfer.
06/21/2010   cnx     Fixed compiler warnings.
06/10/2010   kss     Remove cast of REX_ENABLE_PREMPTION due to Core change.
06/03/2010   cnx     Fixed Klocwork errors.
05/20/2010   smd     Fixed bugs for reverse time transfer.
05/18/2010   arm     Fixed high lint warnings. 
05/10/2010   smd     Added decoding sync for brute-force acquisition.
05/03/2010   ljl     Updated pn offset at synchronization.
04/15/2010   smd     Adapted to new params in hdrsrchdrv_start_slam().
03/24/2010   pxu     Removed FEATURE_HDR_QSM and FEATURE_MSM6500_DSP_SLOT_TIMER features 
11/25/2009   rkc     Added time_sync_time_of_day() to hdrsrchsync_on_time_valid
09/14/2009   ljl     Supported srch MSGR interface.
07/09/2009   lyl     Sent ASP update message when jumping to hyperspace.
07/06/2009   lyl     Updated MinBestAspSinr interface.
06/30/2009   lyl     Updated for generic responses process.
06/25/2009   wsh     Fixed MOB compile error on ARM compiler
06/19/2009   grl     Switched to the hdrsrchrfdiv API.
05/18/2009   ljl     Updated for firmware MSGR interface.
05/07/2009   etv     Replaced obsolete HDRDEC API with new API.
04/16/2009   rkc     Fixed non-SVDO RF macro.
03/18/2009   rkc     Added RF SVDO API changes.
03/06/2009   pmk     Added support for QTF
11/10/2008   kss     Removed/replaced intlocks.
08/22/2008   lyl     Supported primary chain ASET search
06/11/2008   etv     Added MC suffix to HDRDRC_GET_FILTER_OUTPUT call.
05/16/2008   rkc     Fix check for weak HDR signal.
01/15/2008   vish    Added support for MC RF Rx/Tx AGC APIs.
10/31/2007   ljl     Merged from Rev B branch.
10/17/2007   jyw     Merged Rev B changes from branch.
08/19/2007   grl     Resolved Lint High warnings.
07/12/2007   grl     Empty CSET after sync operation.
10/17/2006   ljl     Added HDR SRCH AFC stm.
08/28/2006   sq      Added support for FEATURE_HDR_QSM
07/13/2006   etv     Fixed HSTR adjustment bug (CR 95258).
06/28/2006   jyw     Added supports to MPC.
06/06/2006   grl     Removed sync frame isr when leaving the sync state
08/18/2005   ljl     Added sclk_begin_estimate() at sync state
08/18/2005   ljl     Added code to check weak HDR coverage at sync state
08/11/2005   grl     Mainlined FEATURE_HDR_TCXOMGR.
07/27/2005   ljl     Keep NSET after system loss
05/31/2005   grl     Added support for TCXOMgr.
04/21/2005   kss/sq  Used new hdrdec mode setting APIs
01/24/2005   sq      Properly featurized code in hdrsrchsync_synchronize
12/01/2004   sq      Handled BC status update command in sync state
09/20/2004   sq      Included rf.h
08/23/2004   ds      Update HDR debug display record
07/22/2004   ajn     Added common signals to state machine
06/24/2004   sq      Fixed problem of Rx AGC values in ACQ INFO log packet
06/07/2004   aaj     Replaced hdrsrchidle_init_idle_state with new function
05/05/2004   aaj     Implemented DSP halt work-around to halt on known codes
12/03/2003   sq      Merged in code review comment
12/02/2003   ajn     Complete jump2hyperspace as soon as possible, to solve
                     issues with late decoded (>160ms) QuickConfig msgs
12/01/2003   ajn     Code cleanup
11/26/2003   aaj     Remove inlocks around finger drivers for jump2hyperspace
11/11/2003   sq      Supported new ACQ INFO log packet
10/14/2003   aaj     Support for search result validation to avoid bogus PNs
10/06/2003   aaj     Mainlined T_MSM6500 and HDRSRCH_MULTIPATH_WIN_CENTER
09/10/2003   mpa     Converted F3 messages to use MSG2.0 SSID
07/25/03     sq      Added support for FEATURE_MSM6500_DSP_SLOT_TIMER
04/10/2003   aaj     Handle idle mode command in sync state
03/11/2003   ajn     Updated Copyright notice
11/07/2002   ajn     Roll handler moved to hdrts.
09/18/2002   aaj     Support for multipath window centering for HDR
08/19/2002   ajn     Demod PN Roll ISR is used instead of MOD PN Roll ISR
06/06/2002   aaj     Check if MDSP halted after jump2hyperspace fails
05/15/2002   aaj     Set decoder stale timer to 0 for sync state
05/13/2002   ajn     Added frame ISR counting to avoid using 1st HSTR value,
                     which may be incorrect.
04/17/2002   ajn     Complete sync command if jump_to_hyperspace fails
04/02/2002   aaj     Declare system loss if jump_to_hyperspace fails
03/18/2002   ajn     Updated copyright notice.
03/12/2002   sq      Disable the Sync timeout timer while in FTM mode.
02/22/2002   kss     Set system time in HDR DSP (at syncronization).
02/06/2002   ajn     hdrerrno -> errno
11/26/2001   aaj     Do not re-create ASET after jump2hyper. It creates new LID
10/24/2001   ajn     Change state header files to common hdrsrchstates.h
                     Transition to Idle only after Time has been set.
10/08/2001   ajn     Added call to hdrsrchutil_system_lost().
09/04/2001   ajn     Command interface change.
07/17/2001   aaj/ajn Re-create ASET after jump2hyperspace
06/26/2001   aaj     Get search position dump after MSTR correction
06/02/2001   aaj     Fixed MSTR problems that caused DSP crash. Removed
                     execution of synchronize command on search dump.
05/02/2001   aaj     Changes to execute synchronization command on search dump
04/24/2001   aaj     Migration to TAZ3. ISR/Register name changes
04/11/2001   ajn     Reimplement detect of predetect dump (issue appeared
                     in Idle state too)
03/14/2001   aaj     Cleared DumpTimeout when the predetect dump is skipped
03/08/2001   ajn     Magic #'s->define. Hack: Ignore pipeline predetect dump
03/02/2001   aaj     included customer.h file
02/23/2001   ajn     Removed magic numbers (moved to HDRTS)
11/08/2000   ajn     Moved SCC/CCC lengths into general search parameters
10/31/2000   ajn     Added General Purpose Signals to State Machine
09/28/2000   kss     Added adjustment to HSTR value when read from DSP.
08/18/2000   ajn     Created this file

==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"

#include "hdrsrch.h"

#include "hdrmdspmcr.h"
#include "hdrsrchstate.h"
#include "hdrsrchstates.h"
#include "hdrsrchtask.h"
#include "hdrsrchdrv.h"
#include "hdrsrchfing.h"
#include "hdrsrchutil.h"
#include "hdrsrchset.h"
#include "hdrsrchi.h"
#include "hdrsrchdef.h"
#include "hdrsrchrf.h"
#include "hdrsrchrfdiv.h"
#include "hdrsrchlog.h"
#include "hdrmdsp.h"
#include "hdrsrchsync.h"
#include "hdrcp_msg.h"

#include "time_svc.h"
#include "hdrts.h"

#include "hdrdec.h"

#include "hdrdebug.h"
#include "msm.h"

#include "rfm_hdr.h"

#include "hdrsrchafc.h"
#include "hdrsrchmsg.h"

#ifdef FEATURE_FACTORY_TESTMODE
#include "task.h"
#include "mc.h"

#ifdef FEATURE_CMI
#include "mc_v.h"
#endif

#include "ftm.h"
#endif


#ifdef FEATURE_HDR_BOLT_MODEM
#include "vstmr_lte.h"
#include "vstmr_hdr.h"
#endif /* FEATURE_HDR_BOLT_MODEM */

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/
  
#define HDRSRCH_SYNC_TIMEOUT_MS         5200
  /* 5.2 seconds */

#define HDRSRCH_SYNC_SEARCH_INTERVAL_MS   30

#define HDRSRCH_SYNC_HALFSLOTS_PER_FRAME  32
  /* Number of halfslots in a 26.7ms frame */

#define HDRSRCH_SYNC_HSTR_FRAME_MASK   0x1e0
  /* Rounds raw HSTR value down to a frame boundary */

#define HDRSRCH_SYNC_160MS_IN_FRAMES       6
  /* Used in hdrsrch_synchronize(): 6 (six) 26.7ms frames in 160ms */

#define HDRSRCH_SYNC_MAGIC_TIME   0x400000000ULL
  /* When sync message time exceeds this value, the time is assumed
     to be in 26.7ms frames, otherwise in 80ms units.
     See: hdrsrchsync_synchronize( ) */

#define HDRSRCH_SYNC_FILT_C_I_THRESH1_DEFAULT           54
  /* The default thresh value 1 for filtered C/I is -27dB in -0.5dB unit */
  
#define HDRSRCH_SYNC_FILT_C_I_THRESH2_DEFAULT           20
  /* The default thresh value 2 for filtered C/I is -10dB in -0.5dB unit */
  
#define HDRSRCH_SYNC_THRESH_DRC_DEFAULT                 0x7fff 
  /* The default thresh value for filtered value of DRC is 1 */

#define HDRSRCHSYNC_MSTR_SLAM_TIMEOUT_SLOTS             4
  /* Maximumum number of slots for which to wait before giving up on MSTR
     SLAM operation during time synchronization */

#define HDRSRCH_SYNC_TIMEOUT_BRUTE_FORCE_MS             1380
  /* 1.38 seconds. We only wait 1.38 second for sync message after brute-force acq */

#define HDRSRCH_SYNC_TIME_WAIT_FOR_RTT_US                5000
   /* wait 5ms (5000 us) before read hstr and rtc for reverse time transfer */  

#define HDRSRCHSYNC_MIN_RF_TUNEAWAY_MS       15
  /* Tuneaway overhead in ms */

#define HDRSRCHSYNC_MIN_RF_TUNEAWAY_SCLK \
          (TIMETICK_SCLK_FROM_MS(HDRSRCHSYNC_MIN_RF_TUNEAWAY_MS))
  /* Tuneaway overhead in sclks */

#define HDRSRCHSYNC_MAX_TA_ALLOWED_COUNT (3)
  /* Maximum TA allowed in Sync state */

#define HDRSRCHSYNC_BAND_GRANT_TIMEOUT_MS (50)
  /* Band grant timeout for tune to chan */

#define HDRSRCHSYNC_TRM_PRIORITY_INV_DURATION_MS        (1000)
  /* Sync RF chain priority inversion time of 1sec */

/* EJECT */
/*==========================================================================

                     SYNC STATE PROTOCOL SUBSTATES

==========================================================================*/

typedef enum
{
  HDRSRCHSYNC_DECODE_SYNC_MSG_SUBSTATE,
    /* Decode sync message from AN and wait for sync cmd from CP */
    
  HDRSRCHSYNC_SYNC_CMD_RECEIVED_SUBSTATE,
    /* Sync cmd received from CP perform slam */

  HDRSRCHSYNC_NUM_SUBSTATES
}
hdrsrchsync_substate_enum_type;

/* <EJECT> */
/*==========================================================================

                     HDR SEARCH SYNC STATE DATA

As much SYNC data is lumped together in one structure, so that it becomes
possible to dynamically allocate acquisition data memory during the
SYNC state, and discard the memory when no longer needed.

This will become important when RAM is based on SDRAM.

All variables are accessed as "hdrsrchsync.member_name".  When dynamic
allocation is implemented, a pointer to the structure is needed instead.
At this point, hdrsrchacq must become a dereferenced pointer, instead of
a variable.  Ie)

  #define hdrsrchsync  (*hdrsrchsync_struct_ptr)

==========================================================================*/

/*----------------------------------------------------------------------*
                     hdrsrchdrv_struct_type
 *----------------------------------------------------------------------*/

typedef struct
{

  hdrfw_hstr_offset_cfg_msg_t       hstr_offset_cfg_msg;
    /* HSTR offset cfg message */

  hdrfw_min_best_asp_sinr_msg_t     min_best_asp_sinr_msg;
    /* minBestASPSinr message */

}
hdrsrchsync_fw_msg_struct_type;

/*--------------------------------------------------------------------------
  Tune away Structure
 --------------------------------------------------------------------------*/

typedef struct
{
  timer_type                              timer;
    /* Timer used for tune away */

  boolean                                 no_tune_away;
    /* Do we know when we are supposed to tune away yet? */

  timetick_type                           start_time_sclks;
    /* Sclk timestamp for when we should tune away. */

  timetick_type                           end_time_sclks;
    /* Sclk timestamp for when we should tune away. */

  uint8                                   tune_away_cnt;
    /* counts the number of times TA happened for unlock-by */

  timer_type                              trm_pri_inv_timer;
    /* Timer to count the time spent in CM priority inversion */

  boolean                                 trm_pri_inv_timer_expired;
    /* Priority Inversion timer expiry status */

  hdrsrchrf_reason_enum_t                 sync_trm_priority;
    /* Stores the sync RF chain priority */
}
hdrsrchsync_tune_away_struct_type;

typedef struct
{
  uint64                                sync_time;
    /* Synchronize Time from the SYNC channel message */

  hdrsrch_synchronize_params_type       sync_params;
    /* parameters to be used for synchronizing */

  int                                   frame_isr_cnt;
    /* Flag to skip the first frame sync ISR */

  uint16                                frames_since_scc;
    /* Frames since SCC boundary that we are syncing to */

  boolean                               sync_msg_received;
    /* Flag for the sync message received or not */

  uint16                                hstr;
    /* hstr value */
  
  uint32                                filt_c_i_thresh1;
    /* Thresh1 for filtered C/I. It is lniear scale Q9 */

  uint32                                filt_c_i_thresh2;
    /* Thresh2 for filtered C/I. It is lniear scale Q9 */

  uint16                                thresh_drc;
    /* Thresh for filtered value of DRC */

  uint16                                frame_roll_isr_missed_cnt;
    /* The number of consecutive times the frame roll isr failed to fire. */

  hdrsrchsync_fw_msg_struct_type        fw_msg;
    /* Firmware message */

  hdrsrchsync_tune_away_struct_type     tune_away;
    /* Tune away state */

  boolean                               waiting_for_close_loop;
    /* Whether we are waiting for TCXO close loop permission to continue. */

  timer_type                            div_timer;
    /* Timer used for diversity */

  hdrsrchsync_substate_enum_type        sync_substate;
    /* Current substate of the sync state */

  hdrsrch_sync_substate_cb_type         sync_substate_cb;
    /* callback function to return the substate of sync state */

}
hdrsrchsync_struct_type;



LOCAL hdrsrchsync_struct_type    hdrsrchsync;

/* EJECT */
/*==========================================================================

                     STATE DISPATCH TABLE

This section provides the dispatch functions for events that are received
by the task while in this state.

==========================================================================*/
#define HDRSRCHSYNC_SLAM_TIMER               &hdrsrch.gp_timer0
#define HDRSRCHSYNC_SLAM_TIMEOUT_SIG         HDRSRCH_GP_TMR0_SIG
#define HDRSRCHSYNC_FRAME_ROLL_TIMER         &hdrsrch.gp_timer1
#define HDRSRCHSYNC_FRAME_ROLL_TIMEOUT_SIG   HDRSRCH_GP_TMR1_SIG
#define HDRSRCHSYNC_FRAME_ROLL_TIMEOUT_SLOTS 34
  /* Number of slots taken by frame roll ISR to fire twice
 (32 slots + 2 slots for overheads) */

#define HDRSRCHSYNC_MAX_SYNC_CMD_PROCESS_SCLKS                                \
          ( TIMETICK_SCLK_FROM_MS ( SLOTS_TO_MS (                             \
                                        HDRSRCHSYNC_FRAME_ROLL_TIMEOUT_SLOTS  \
                                        + HDRSRCHSYNC_MSTR_SLAM_TIMEOUT_SLOTS \
                                        + HDRTS_SLOTS_PER_FRAME ) ) )
  /* Max Time required to perform sync = time taken for slam 
                            + time taken for Frame roll to settle
                            + one frame duration of time for other overheads */


LOCAL void            hdrsrchsync_init( hdrsrch_state_enum_type last_state );
LOCAL void            hdrsrchsync_done( hdrsrch_state_enum_type next_state );
LOCAL errno_enum_type hdrsrchsync_cmd( const hdrsrch_cmd_type * cmd );
LOCAL void            hdrsrchsync_timeout( void );
LOCAL void            hdrsrchsync_sched( void );
LOCAL void            hdrsrchsync_dump( void );
LOCAL void            hdrsrchsync_on_time_valid( void );
LOCAL void            hdrsrchsync_rf_unlock_event( void );
LOCAL void            hdrsrchsync_slam_timeout( void );
LOCAL void            hdrsrchsync_frame_roll_timeout( void );
LOCAL void            hdrsrchsync_process_low_pri_rsp_ind
                      (
                        hdrsrchmsg_s_type         *msg
                      );
LOCAL void            hdrsrchsync_exit_brute_force_sync( void );
LOCAL void            hdrsrchsync_process_int_cmd
                      (
                        hdrsrchsm_cmd_payload_type *
                      );

LOCAL void hdrsrchsync_gp_sig0( void );

void hdrsrchsync_trm_priority_timer_cb
( 
  timer_cb_data_type              data
);

LOCAL hdrsrchrf_reason_enum_t hdrsrchsync_update_trm_priority( void );

LOCAL hdrsrch_state_struct_type hdrsrchsync_state =
{
  HDRSRCH_SYNC_STATE,
  hdrsrchsync_init,
  hdrsrchsync_done,
  hdrsrchsync_cmd,
  hdrsrchsync_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
  hdrsrchsync_sched,
  hdrsrch_default_lost_dump,
  hdrsrchsync_timeout,
  hdrsrchsync_slam_timeout,
  hdrsrchsync_frame_roll_timeout,
  hdrsrchsync_gp_sig0,
  hdrsrch_default_gp_sig1,
  hdrsrchsync_on_time_valid,
  hdrsrch_default_rxchipx16,
  hdrsrchsync_rf_unlock_event,
  hdrsrchsync_process_int_cmd,
  hdrsrchsync_process_low_pri_rsp_ind
};

/*=========================================================================

FUNCTION     : HDRSRCHSYNC_UPDATE_ACQ_INFO

DESCRIPTION  :
  Updates ACQ INFO global structure, according to current acquisition result.


DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

LOCAL void hdrsrchsync_update_acq_info
(
  log_hdr_acq_result_enum_type result
  /* current acquisition result       */
)
{
  uint16        max_fing_rssi;  /* Maximum finger RSSI        */

  int           i;              /* Loop index                 */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchlog.acq_info.result          = result;
    /* Record acquisition result                */

  if ( log_status(LOG_HDR_ACQ_INFO_C) )
  {
    switch( result )
    {
      case  HDRLOG_ACQ_SUCCESS:

        hdrsrchlog.acq_info.pn_offset     = hdrsrchsync.sync_params.pilot_pn;
          /* Record the PN offset from SYNC messgae   */

        for( i = 0; i< HDR_BYTES_IN_SYSTEM_TIME; i++ )
        {
          hdrsrchlog.acq_info.system_time[i]  =
            (*((uint8 *)(&hdrsrchsync.sync_params.sync_time) + i));
        }
          /* Record the system time from SYNC message. We use uint64 internally
             to keep system time. But we only use HDR_BYTES_IN_SYSTEM_TIME(5)
             number of bytes in the log packet. Here we are doing byte by byte
             copying                                                         */

        /* Falls through here, since info below is needed for both cases    */

      case  HDRLOG_ACQ_SYNC_FAILURE:

        max_fing_rssi = 0;
          /* Initialize max_fing_rssi to zero   */

        for( i = 0; i< HDR_MAX_FINGS_CNT; i++ )
        {
          if( ( HDR_FING_ASSIGNED( HDRMDSP_DEMOD_CARRIER_0, i ) ) &&
              ( hdrsrchfing.fings[HDRMDSP_DEMOD_CARRIER_0][i].rssi > 
                                                              max_fing_rssi ) )
          {
            max_fing_rssi = hdrsrchfing.fings[HDRMDSP_DEMOD_CARRIER_0][i].rssi;
              /* Record the current maximum RSSI of an assigned finger    */
          } /* if */

        } /* for */

        hdrsrchlog.acq_info.fing_rssi_sync  = max_fing_rssi;
          /* Record finger RSSI in SYNC state */

        if ( hdrmdsp_check_dsp_clk( ) )
        {
          REX_DISABLE_PREMPTION();

#ifdef FEATURE_HDR_REVB
          hdrsrchlog.acq_info.agc0_sync = 
            rfm_hdr_get_rx_agc( hdrsrchrf_get_rf_dev(HDRSRCHRF_PRI_CHAIN),
                                RFM_HDR_CARRIER_ID__CUMULATIVE,
                                RFM_HDR_AGC_FORMAT__DBM256);
#else
          hdrsrchlog.acq_info.agc0_sync = RF_HDR_GET_RX0_AGC_DB256();
#endif /* FEATURE_HDR_REVB */
            /* read AGC value in 1/256th of a dbm unit */

          if ( hdrsrchrfdiv_diversity_demod_is_enabled( ) == TRUE )
          {
#ifdef FEATURE_HDR_REVB
            hdrsrchlog.acq_info.agc1_sync = 
              rfm_hdr_get_rx_agc( hdrsrchrf_get_rf_dev(HDRSRCHRF_DIV_CHAIN),
                                  RFM_HDR_CARRIER_ID__CUMULATIVE,
                                  RFM_HDR_AGC_FORMAT__DBM256);
#else
            hdrsrchlog.acq_info.agc1_sync = RF_HDR_GET_RX1_AGC_DB256();
#endif /* FEATURE_HDR_REVB */
              /* Diversity ON so read the second antenna as well */
          }
          else
          {
            hdrsrchlog.acq_info.agc1_sync = (int16)0x8000;
              /* 0x8000 indicates antenna is not ON, per requirement      */
          }

          REX_ENABLE_PREMPTION();
        }

        break;

      default:
        HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Can not recognize result: %d",
                        (uint8)result );
    } /* switch */

  } /* if log_status  */

} /* hdrsrchsync_update_acq_info */


/*=========================================================================

FUNCTION     : HDRSRCHSYNC_SEND_HSTR_OFFSET_CFG_MSG

DESCRIPTION  : This function sends the message to config HSTR offset.

DEPENDENCIES : None

INPUT        : hstr_offset - HSTR offset

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchsync_send_hstr_offset_cfg_msg
( 
  uint16                     hstr_offset
    /* HSTR offset */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  msgr_init_hdr( &hdrsrchsync.fw_msg.hstr_offset_cfg_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_HSTR_OFFSET_CFG_CMD );
    /* Init message header */

  hdrsrchsync.fw_msg.hstr_offset_cfg_msg.hstrOffset = hstr_offset;
  hdrsrchmsg_send_msg( &hdrsrchsync.fw_msg.hstr_offset_cfg_msg,
                       sizeof( hdrfw_hstr_offset_cfg_msg_t ) );
    /* Send the msg */
} /* hdrsrchsync_send_hstr_offset_cfg_msg() */

/*===========================================================================

FUNCTION HDRSRCHSYNC_GET_CDMA_SYS_TIMING

DESCRIPTION
  This function gets cdma system time and return it to CP.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchsync_get_cdma_sys_timing( void )
{
  
  uint16         raw_hstr, hstr;
    /* Half-slot timing reference */

  uint64         sync_frame;
    /* Frame to synchronize time at */

  uint32         hdr_rtc_cnt_cx8 = 0;
    /* hdr rtc count in cx8 */

#ifdef FEATURE_HDR_BOLT_MODEM

  vstmr_ostmr_t vstmr_ostmr;
    /* Current OSTMR count */

  vstmr_lte_view_s *lte_view = NULL;
  vstmr_hdr_view_s *hdr_view = NULL;
    /* Handle LTE view */

  uint32 ustmr = 0;
		/* various counters */

  sys_modem_as_id_e_type sid = SYS_MODEM_AS_ID_1;

  vstmr_rtc_id lte_rtc_id = VSTMR_RTC_LTE_SUB0;
    /* Variables relating to subscription ID of LTE/HDR (same subscription) */

#endif /* FEATURE_HDR_BOLT_MODEM */

  uint32         lte_stmr_cnt = 0;
    /* lte stmr count */

  uint64         cdma_sys_time = 0;
    /* cdma system time */

  boolean        status = TRUE;
    /* reverse time transfer status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* First we need to get the current cdma frame number. */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "IRAT wait 5ms for Reverse Time Transfer");
  hdrsrch_timed_wait_usecs( HDRSRCH_SYNC_TIME_WAIT_FOR_RTT_US );
    /* This a workaround. If we don't wait 5ms here, hstr count is not updated
       and the timing is one frame off
    */

  REX_DISABLE_PREMPTION();     
       
  raw_hstr = HDRHSTR_GET_COUNT();
    /* Get raw HSTR value: 0 .. 0xffff half slots */

  hstr = raw_hstr & HDRSRCH_SYNC_HSTR_FRAME_MASK;
    /* Round raw HSTR to the start of frame */

  sync_frame = hstr / HDRSRCH_SYNC_HALFSLOTS_PER_FRAME;
    /* Convert HSTR to frame. Since hstr will sync to starting of SCC when
       hstr_offset is set, e.g hstr is 0 at the statring of SCC, this hstr
       is the distance from the current frame boundary to starting of 
       SyncChannelCycle(=512 half slots)
    */

  sync_frame += hdrsrchsync.sync_time;
    /* Get the real frame number of current frame */

  /* Now we need to read snapshot of HDR RTC and LTE STMR. With this snap shot
     we should be able to calculate pair of cdma system time and lte stmr */

#ifdef FEATURE_HDR_BOLT_MODEM
  /* Still under critical section */

  /* Get the subscriber ID.  From the subscriber ID, use the correct RTC ID for HDR
     and LTE to read time snapshots. At present, HDR and LTE are assumed to be on
     the same subscription. */
  sid = hdrsrch_get_subscriber_id( );
  switch ( sid ) 
  {
    case SYS_MODEM_AS_ID_1:
      lte_rtc_id = VSTMR_RTC_LTE_SUB0;
      break;

    case SYS_MODEM_AS_ID_2:
      lte_rtc_id = VSTMR_RTC_LTE_SUB1;
      break;

    default:
      sid = SYS_MODEM_AS_ID_1; 
      lte_rtc_id = VSTMR_RTC_LTE_SUB0;
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Unexpected HDR sub_id=%d, default to SYS_MODEM_AS_ID_1",sid );
      break;

  }/* switch */

  /* Get handle for both LTE and HDR RTC views.  While LTE view is
     subscription-specific, HDR view is always the default subscription */
  lte_view = vstmr_lte_get_view_handle
						 (
							 lte_rtc_id,
							 VSTMR_LTE_OSTMR_VIEW
						 );
  hdr_view = vstmr_hdr_get_view_handle
						 (
							 VSTMR_RTC_HDR_SUB0,
							 VSTMR_HDR_RTC_VIEW
						 );
	
  vstmr_ostmr = vstmr_lte_get_current_ostmr(lte_view);
  /* Read current OSTMR count and derive the XO and HSTMR counts */

  ustmr = vstmr_lte_ostmr_to_xo(lte_view, vstmr_ostmr);
  hdr_rtc_cnt_cx8 = vstmr_hdr_xo_to_hstmr(hdr_view, ustmr).cx8;
	/*  Convert values to HDR RTC */
	
  lte_stmr_cnt = vstmr_ostmr.ostmr_cnt;
#else
  HWIO_OUT( UNIV_STMR_STATUS_DUMP_CMD, 0x1 );
    /* enable dumping of HDR RTC and LTE STMR snapshot. 0x01 means dump them
       immediately.
    */

   hdr_rtc_cnt_cx8 = ( HWIO_IN( RTC_HDR_TIME_STATUS_RD ) >> 2 ) & 0x3ffff;
    /* Read HDR RTC value, it is in chipx32, 32 bits wide. Need to convert it
       to cx8 since we use cx8 in everywhere else 
    */

   lte_stmr_cnt = HWIO_IN( O_STMR_TIME_STATUS_RD );
      /* Read lte stmr value*/
#endif /* FEATURE_HDR_BOLT_MODEM */

  REX_ENABLE_PREMPTION();

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
    "IRAT sync read snapshot (sub_ID=%d), HDR_RTC: 0x%x, LTE STMR: 0x%x",
    sid, hdr_rtc_cnt_cx8, lte_stmr_cnt);


  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "IRAT Frame boundary: HSTR=%d raw=%d curr_frame=%d",
                  hstr, raw_hstr, sync_frame );


  /* We can calculate pair of cdma system time and lte stmr now */

    cdma_sys_time = sync_frame * HDRSRCH_SYNC_HALFSLOTS_PER_FRAME * 1024;
      /* Get cdma system time at the current frame boundary */

    hdr_rtc_cnt_cx8 += 0x40000;
      /* HDR RTC counter is in chip ratex8, and it wraps around at 32768 chip
         36768*8= 0x40000. */
    hdr_rtc_cnt_cx8 -= hdrsrchdrv_get_mstr_x8();
    hdr_rtc_cnt_cx8 &= 0x3ffff;
      /* Calcuate how many cdma chips has been passed from frame boundary till
         the moment of dumping the snapshot. This number is in cx8 */
    cdma_sys_time += hdr_rtc_cnt_cx8 >> 3;
      /* Get cdma system time at the moment of dumping the snapshot */

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
    "IRAT reverse timing, CDMA_SYS_TIME: 0x%x, LTE STMR: 0x%x hdr_rtc_offset : 0x%x",
    cdma_sys_time, lte_stmr_cnt, hdrsrchdrv_get_mstr_x8() );

  cdma_sys_time += HDRSRCH_IRAT_HDR_RTC_COMPENSATION;
    /* Because the delay of RF path, CDMA system time coming from SIB8 will introduce
       a fixed offset. To be consistent with SIB8 timing, we need to add the offset
       here. Because of the timing compensation done in time transfer,if we don't add
       the offset here, we will see a offset when doing time transfer of timeing from
       RTT.
    */
  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
    "IRAT reverse timing, CDMA_SYS_TIME adjust: 0x%x", cdma_sys_time);

  if ( hdrsrchsync.sync_params.return_timing_cb != NULL)
  {
    hdrsrchsync.sync_params.return_timing_cb( status, lte_stmr_cnt, cdma_sys_time );
      /* Send pair of lte stmr and cdma system time to CP */
  }
}/* hdrsrchsync_get_cdma_sys_timing */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSYNC_TIME_VALID

DESCRIPTION
  This callback function is called when hdrts_set( ) has set the time.

DEPENDENCIES
  Must only be passed as a callback function for hdrts_set( ).

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  Sets signal to inform the task to:
    Completes the Sync command.
    Transitions the state-machine to the Idle state.

===========================================================================*/

LOCAL void hdrsrchsync_time_valid( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_SIG_SET( HDRSRCH_TIME_VALID_SIG );
    /* Indicate time is valid */

} /* hdrsrchsync_time_valid( ) */



/*===========================================================================

FUNCTION HDRSRCHSYNC_ON_TIME_VALID

DESCRIPTION
  This function is called from task context when system time has been set.

DEPENDENCIES
  Called in response to the HDRSRCHSYNC_TIME_VALID signal.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  Completes the Sync command.
  Transitions the state-machine to the Idle state.

===========================================================================*/

LOCAL void hdrsrchsync_on_time_valid( )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "Synchronize Time of Day to HDR System Time" );

  hdrts_update_ts_time();
    /* Synchronize Time of Day to HDR System Time */

  hdrsrch_complete_cmd( );
    /* The Synchronize command is now complete */

  hdrsrchsync_update_acq_info( HDRLOG_ACQ_SUCCESS );
    /* Update ACQ INFO log packet for successful acquisition              */

  hdrsrchsync.tune_away.tune_away_cnt = 0;
    /* Sync completed. Reset the tune_away_cnt */

  if ( hdrsrchutil_is_brute_force_acq() == TRUE )
  {
    /* For brute force acq and sync*/

    hdrsrchsync_get_cdma_sys_timing();
      /* Read timing and give it to CP */

    hdrsrchafc_stop_fll_tracking();
  }
  else
  {
    /* For normal sync */

    hdrsrchmon_init_idle_state( );
      /* And switch to IDLE state processing */
  }

} /* hdrsrchsync_on_time_valid */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSYNC_FRAME_ISR

DESCRIPTION
  This Interrupt Service Routine completes the time synchronization.

DEPENDENCIES
  hdrsrchidle_synchronize( ) must be called to install this ISR.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  There will be a time discontinuity.
  The ISR auto uninstalls itself.
  Eventually, hdrsrchsync_time_valid( ) will be called.

===========================================================================*/

LOCAL void hdrsrchsync_frame_isr( void )
{
  uint16  raw_hstr, hstr;
    /* Half-slot timing reference */

  uint16  sync_frame;
    /* Frame to synchronize time at */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_kill_timer( HDRSRCHSYNC_FRAME_ROLL_TIMER );
    /* Clean up Frame Roll Timeout timer. */

  hdrsrchsync.frame_roll_isr_missed_cnt = 0;
    /* The frame roll fired as expected. */

  hdrsrchsync.frame_isr_cnt++;
    /* Increment frame count */

  raw_hstr = HDRHSTR_GET_COUNT();
    /* Get raw HSTR value: 0 .. 0xffff half slots */

  hstr = (raw_hstr + 16) & HDRSRCH_SYNC_HSTR_FRAME_MASK;
    /* Round raw HSTR to nearest frame, as a 9 bit number. Since frame ISR usually
       fires at 2 slots before frame ending, plus 16 would make hstr round to the next
       frame boundary */

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Frame boundary: HSTR=%d raw=%d Frames since scc=%d",
                  hstr, raw_hstr, hdrsrchsync.frames_since_scc );

  /* Since we are adjusting HSTR, the first read of HSTR may be incorrect.
     Only examine HSTR values for the 2nd or future occurances */

  if ( hdrsrchsync.frame_isr_cnt >= 2 )
  {
    sync_frame = hstr / HDRSRCH_SYNC_HALFSLOTS_PER_FRAME;
      /* Convert HSTR to frame after start of SCC */

    /* If the frame of current SCC AT is in is less then the frame 
       since recorded SCC start, we have rolled into the next SCC. */
    if ( sync_frame < hdrsrchsync.frames_since_scc )
    {
      sync_frame += HDRTS_FRAMES_PER_SCC;
        /* Increment by SCC cyle (16 frames) */
    }

    /* Uninstall the roll handler -- not needed any more */

    hdrts_deregister_roll_callback( hdrsrchsync_frame_isr );

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Setting HDR time HSTR=%d frame=%d",
                    hstr, sync_frame);

    hdrts_set( hdrsrchsync.sync_time + sync_frame, hdrsrchsync_time_valid );
      /* Set HDR time (# of 26.7ms periods from UTC=0), and call
         hdrsrchsync_time_valid when time has been successfully set. */

  } /* if frame_isr_cnt is >= 2 */

} /* hdrsrchsync_frame_isr( ) */


/*=========================================================================

FUNCTION     : HDRSRCHFING_JUMP_TO_HYPERSPACE

DESCRIPTION  : This function changes the finger positions to reflect the
               PN offset received in the sync channel. It also starts
               MSTR slam operation and aligns mobile timer reference with
               the earliest arriving locked finger.

DEPENDENCIES : None

RETURN VALUE : TRUE: timing change succeeded
               FALSE: failed. 

SIDE EFFECTS : Changes MSTR
=========================================================================*/

boolean hdrsrchsync_jump_to_hyperspace
(
  uint16 pilot_pn
    /* pilot pn offset obtained from CC message */
)
{
  uint16   ref_pn;          /* reference pn offset */
  int16    delta_pn;        /* delta in PN offsets */
  int32    delta_pnx8;      /* delta in PN offsets in chipx8 */
  uint32   rtc_offx8;       /* finger rtc offset in chipx8 */
  int      f;               /* finger index */
  int8     c;               /* carrier index */
  int      timeout_slots;   /* Count number of slots being waited for */

#ifdef FEATURE_HDR_REVB
  hdrsrch_demod_idx_type ref_demod_idx = 0;
    /* Reference demod carrier index */

  hdrsrch_sect_struct_type*  ref_sect = NULL; 
    /* Reference sector pointor */
#endif /* FEATURE_HDR_REVB */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*hdrsrch_timed_wait_usecs( HDRSRCHFING_ASSIGNMENT_DONE_WAIT_US );*/
  (void) hdrsrch_wait_for_high_pri_rsp( HDRSRCHFING_ASSIGNMENT_DONE_WAIT_US, 
                                        HDRSRCH_FING_ASSIGN_RSP );
    /* wait for 1 slot so that triage finger assignments can take effect
       before jump2hyperspace is done. After last triage, the finger
       assignments would be commited to the DSP but may not take effect
       until 1.67 ms. During jump2hyperspace, the fingers are read and
       their positions are corrected. We MUST have updated the assignment
       registers before jump2hypespace so that the correction is accurate.
       This delay gurantees that
        */

  hdrsrchfing_read_fing_status();
    /* Get latest finger status dump and update the hdrsrchfing.fings[] table */

#ifndef FEATURE_HDR_REVB
  ref_pn = HDRMSTR_GET_REF_PN();
    /* Get Reference PN offset in the units of 64 PN chips */
#else
  ref_sect = hdrsrchutil_get_ref_sector_ptr();
    /* Get the reference sector pointor */

  if( ref_sect != NULL )
  {
    ref_pn = ref_sect->pn_offset;
      /* Get Reference PN offset in the units of 64 PN chips */
  }
  else
  {
    ref_pn = HDR_NO_PN_OFFSET;
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "No ref PN b4 jump2hyperspace" );
  }
#endif /* FEATURE_HDR_REVB */

  /* This loop may not be required since the reference PN offset would
     have been set already during the MSTR slam operation in acquisition
     state machine. This can be removed later */

  timeout_slots = 0;
    /* start counting for how long are we waiting */

  while( ref_pn == HDR_NO_PN_OFFSET )
  {
    hdrsrch_timed_wait_usecs( 1800 );
      /* wait for 1800 uSec which is approx a slot period */

#ifndef FEATURE_HDR_REVB
    ref_pn = HDRMSTR_GET_REF_PN();
      /* read again */
#else
    ref_sect = hdrsrchutil_get_ref_sector_ptr();
 
    if( ref_sect != NULL )
    {
      ref_pn = ref_sect->pn_offset;
        /* Get Reference PN offset in the units of 64 PN chips */
    }
    else
    {
      ref_pn = HDR_NO_PN_OFFSET;
    }
#endif /* FEATURE_HDR_REVB */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Waiting for MSTR slam b4 jump2hyperspace" );

    if ( timeout_slots++ >= HDRSRCHSYNC_MSTR_SLAM_TIMEOUT_SLOTS )
    {
      /* Have a way to exit from this condition */
      return FALSE;
    }

  }

  hdrsrchdrv_abort_search( FALSE );
    /* Abort search */

  hdrsrch_stop_timer( &hdrsrch.schedule_timer );
    /* Stop schedule timer */

  delta_pn    = pilot_pn - ref_pn;
    /* Get the adjustment required for the aligning the system time. */

  delta_pnx8 =  delta_pn * HDR_PILOT_INCR * 8;
    /* Multiply by 8 to get adjustment in chipx8 units */

  for ( c = 0; (c < hdrsrchfing.params.max_demod_carriers) && (c < HDRSRCH_MAX_DEMOD_CARRIERS_CNT); c++ )
  {
    if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) )
    {

      for ( f = 0; (f < hdrsrchfing.params.max_fings)&& (f < HDR_MAX_FINGS_CNT); f++ )
      {
        if ( HDR_FING_ASSIGNED( c, f ) )
        {
          /* Finger f is assigned */

          rtc_offx8 = (hdrsrchfing.fings[c][f].posx8 - delta_pnx8) & 0x3ffff;
            /* Get the RAW finger offset in chipx8 units from the hardware
               and adjust it with new position */

          hdrsrchfing.fings[c][f].pn_offset += delta_pn;
            /* adjust the PN offset */

          hdrsrchfing_set_rtc_offset( &hdrsrchfing.fings[c][f], rtc_offx8 );
            /* write the rtc offset */

          hdrsrchfing.fings[c][f].modified = TRUE;
            /* The finger record was modified */

        }/* if assigned */
      }/* for each finger */
    }/* if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) ) */
  }/* for c */

#ifdef FEATURE_HDR_REVB

  hdrsrchset.subaset[ HDRSRCH_IDLE_SUBASET_IDX ].aset[ 0 ]->pn_offset = pilot_pn;
    /* Update PN offset */

  ref_demod_idx = hdrsrchset_get_demod_carrier_index( ref_sect->aset.subaset_idx );
    /* Get demod carrier index */

  ASSERT((ref_demod_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT) &&
         (ref_sect->aset.asp_idx < HDRSRCH_MAX_NUM_ASPS));
  HDRASP_SET_PN( ref_demod_idx, ref_sect->aset.asp_idx, pilot_pn );
    /* Update PN offset in ASP structure */

  hdrsrchmsg_send_asp_update_msg( ref_demod_idx,
                                  ref_sect->aset.asp_idx );
    /* Send the ASP update message */
#endif /* FEATURE_HDR_REVB */

  (void) hdrsrchfing_program_fings_and_slam();
    /* Program the fingers to the hardware and slam */

  /* Wait for 1.67msec to ensure that the finger assignment is done
     before initiating the slam operation. This wait loop is must.
     This can be replaced with rex timers if necessary but do not
     delete this

     More explaination: After the finger positions are changed the
     changes are comitted to the DSP but may not take effect until
     next slot boundary which could be after 1.67ms. To start MSTR
     slam we need to have fingers locked in position hence this
     delay is added to ensure that finger assignment is complete
     before slam operation */

  /* (void) hdrsrch_wait_for_high_pri_rsp( 1800, HDRSRCH_FING_ASSIGN_RSP ); */
  /* Since finger assignment and slam are in one message and this delay is not needed */

  hdrsrch_kill_timer( HDRSRCHSYNC_SLAM_TIMER );
    /* Clear the slam timer, and remove from wait mask */

  hdrsrch_add_to_wait_mask( HDRSRCHSYNC_SLAM_TIMEOUT_SIG );
    /* Watch for slam timer signal */

  (void) rex_set_timer( HDRSRCHSYNC_SLAM_TIMER, 
                        SLOTS_TO_MS( HDRSRCHSYNC_MSTR_SLAM_TIMEOUT_SLOTS ) );
    /* Timer for checking for MSTR slam */

  return TRUE;
} /* hdrsrchsync_jump_to_hyperspace() */

/*=========================================================================

FUNCTION     : HDRSRCHSYNC_EXIT_BRUTE_FORCE_SYNC

DESCRIPTION  : This function does clean up process and transits to inactive
               state in brute-force acq case.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None
=========================================================================*/

LOCAL void hdrsrchsync_exit_brute_force_sync( void )
{

  hdrsrch_stop_timer( &hdrsrch.schedule_timer );
    /* Stop the schedule timer since we are declaring system loss */

  hdrsrchafc_deactivate();
    /* Deactivate afc */

  if ( hdrsrchsync.sync_params.return_timing_cb != NULL)
  {
    hdrsrchsync.sync_params.return_timing_cb(FALSE,0,0);
      /* Indicate reverse time transfer failure to CP */
  }

} /* hdrsrchsync_exit_brute_force_sync */


/*=========================================================================

FUNCTION     : HDRSRCHSYNC_SLAM_FAILED

DESCRIPTION  : This function completes synchronization command and declares 
               system lost.

DEPENDENCIES : None

RETURN VALUE : None 

SIDE EFFECTS : Changes MSTR
=========================================================================*/

void hdrsrchsync_slam_failed( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "slam failed" );

  if ( hdrsrch.cmd )
  {
    hdrsrch.cmd->status = E_FAILURE;
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "No command is available");
      /* Command is expected here */
  }

  hdrsrch_complete_cmd();
    /* Mark command as failed, but finished. */

  hdrsrchsync.tune_away.tune_away_cnt = 0;
    /* Sync failed Reset the tune_away_cnt */

  /* check if the DSP halted on us during this operation */

  if ( hdrsrchutil_check_mdsp_halt() )
  {
    hdrsrchutil_handle_mdsp_halt();
      /* handle the DSP halt emergency */
  }
  else
  {
    if ( hdrsrchutil_is_brute_force_acq() == TRUE )
    {
      hdrsrchsync_exit_brute_force_sync();
        /* Clean up and go to inactive state */
    }
    else
    {
      hdrsrchutil_system_lost( HDRSRCH_SYSLOST_MSTR_SLAM_TIMEOUT );
        /* declare system lost since jump-to-hyperspace was unsuccessful */
    }
  }
} /* hdrsrchsync_slam_failed */


/*=========================================================================

FUNCTION     : HDRSRCHSYNC_SLAM_TIMEOUT

DESCRIPTION  : This function handles the slam failure after the timeout.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None
=========================================================================*/

void hdrsrchsync_slam_timeout( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Slam timer expired and no slam response has been received */
  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "slam in Jump2Hyperspace failed" );

  if ( hdrsrchdrv_is_slam_pending() ) 
  {
    /* Sync state is wating for slam rsp, and slam timed out */
    hdrsrchsync_slam_failed();
  }
  else
  {
    /* Slam rsp has been received, ignore the timeout */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "slam timed out but slam rsp was received" );
  }

} /* hdrsrchsync_slam_timeout */


/*=========================================================================

FUNCTION     : HDRSRCHSYNC_FRAME_ROLL_TIMEOUT

DESCRIPTION  : This function handles the case where tramp loses the PN roll 
               interrupt.  System loss is declared when this happens.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : It clears the PN roll interrupt. 
 
=========================================================================*/

void hdrsrchsync_frame_roll_timeout( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Slam timer expired and no slam response has been received */
  HDR_MSG_SRCH( MSG_LEGACY_FATAL, "Frame roll interrupt failed to fire" );

  hdrsrchsync.frame_roll_isr_missed_cnt++;
    /* The frame roll didn't fire. Keep track of that */

  if ( hdrsrchsync.frame_roll_isr_missed_cnt > 2 )
  {
    ERR_FATAL( "The frame roll failed to fire three syncs in a row", 0, 0, 0 );
  }

  hdrts_clear_pn_roll_interrupt();
  /* Sync state is waiting for the frame roll interrupt that isn't firing.
     Reset TRAMP and deactivate. */

  if ( hdrsrch.cmd )
  {
    hdrsrch.cmd->status = E_FAILURE;
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "No command is available");
    /* Command is expected here */
  }

  hdrsrch_complete_cmd();
    /* Mark command as failed, but finished. */

  hdrsrchsync.tune_away.tune_away_cnt = 0;
    /* Sync failed Reset the tune_away_cnt */

  if ( hdrsrchutil_is_brute_force_acq() == TRUE )
  {
    hdrsrchsync_exit_brute_force_sync();
      /* Clean up and go to inactive state */
  }
  else
  {
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_MSTR_SLAM_TIMEOUT );
      /* Sync was unsuccessful */
  }

} /* hdrsrchsync_frame_roll_timeout */


/*=========================================================================

FUNCTION     : HDRSRCHSYNC_PROCESS_SLAM_RSP

DESCRIPTION  : This function processes slam response.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Changes MSTR
=========================================================================*/

void hdrsrchsync_process_slam_rsp( void )
{
  hdrsrch_synchronize_params_type  *sync;
    /* Pointer to synchronization command parameters */

  uint16                            hstr_offset;
    /* HSTR Offset */

  uint16                            raw_hstr;
    /* Half-slot timing reference */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrchdrv_is_slam_pending() ) 
  {

    hdrsrch_kill_timer( HDRSRCHSYNC_SLAM_TIMER );
      /* Clear the slam timer, and remove from wait mask */

    sync = &hdrsrchsync.sync_params;
      /* point to the synchronize command parameters */

    hdrsrchdrv_send_hyperspace_cfg_msg( HDRSRCH_SET_HYPERSPACE_MODE );
      /* Write jump to hyperspace flag to 1 so that the preamble detection
         happens only on even half slot boundaries */

     /* I am not sure why following delay was added but I will leave it
        unchanged until it can be proven that this is not needed */

    hdrsrch_timed_wait_usecs( 5000 );
      /* wait for 5 msec */

    /* Successful timing change. Proceed with timing correction */

    raw_hstr = HDRHSTR_GET_COUNT();
    /* Get raw HSTR value: 0 .. 0xffff half slots */

    hstr_offset = sync->half_slot - 
                  (uint16) ((hdrsrchsync.sync_time * 
                             HDRSRCH_SYNC_HALFSLOTS_PER_FRAME) & 0xFFFF);

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                    "before set hstrOffset, hstr_offset= %d, raw_hstr=%d",
                    hstr_offset, raw_hstr );

    REX_DISABLE_PREMPTION();

    /* Do not create ASET here since that gives a new link ID to the
       active set after jump 2 hyperspace. Instead, just modify the
       existing sector with the new pilot PN */

#ifdef FEATURE_HDR_REVB
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->pn_offset
                                                       = sync->pilot_pn;
      /* Re-create the aset pilot after time change */

    hdrsrchset_force_unique_aset_pilot( 
                 hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0] );
      /* Checked if any pilot with the same pn offset in CSET or NSET */
#else
    hdrsrchset.aset[0]->pn_offset = sync->pilot_pn;
      /* Re-create the aset pilot after time change */

    hdrsrchset_force_unique_aset_pilot( hdrsrchset.aset[0] );
      /* Checked if any pilot with the same pn offset in CSET or NSET */
#endif /* FEATURE_HDR_REVB */

    /* Find the number of frames between now and the start of the SyncCC 
       cycle AT with Sync capsule. */
    hdrsrchsync.frames_since_scc = 
      ( uint16 ) ( HDRHSTR_GET_COUNT() - sync->half_slot )
      / HDRSRCH_SYNC_HALFSLOTS_PER_FRAME;

    hdrsrchsync_send_hstr_offset_cfg_msg( hstr_offset );
       /* Config hstr offset */

    hdrsrchsync.frame_isr_cnt = 0;
      /* Reset frame_isr count to zero */

    hdrts_register_roll_callback( hdrsrchsync_frame_isr );
      /* Install PN roll handler*/

    hdrsrch_add_to_wait_mask( HDRSRCHSYNC_FRAME_ROLL_TIMEOUT_SIG );
      /* Watch for slam timer signal */

    (void) rex_set_timer( HDRSRCHSYNC_FRAME_ROLL_TIMER, 
                          SLOTS_TO_MS( HDRSRCHSYNC_FRAME_ROLL_TIMEOUT_SLOTS ) );
      /* Timer for making sure the frame roll actually fires. */

#if (defined (HDRSRCH_UT_PLATFORM) || defined(TEST_FRAMEWORK))
    #error code not present
#endif /* HDRSRCH_UT_PLATFORM */

    hdrsrchdrv.mstr_valid = TRUE;
      /* Now that we have jumped to hyperspace, MSTR is valid */

    hdrsrchdrv_update_mstr_x8( );
      /* Update with the latest MSTR */

    hdrsrchutil_set_win_center_to_mstr();
      /* initialize window center to MSTR to start with */

    hdrsrch_add_to_wait_mask( HDRSRCH_TIME_VALID_SIG );
      /* Advance state machine when system time is set */

    REX_ENABLE_PREMPTION();

    hdrsrchsync_sched();
      /* Start search */
  }
  else
  {
    /* Bogus slam rsp */
     HDR_MSG_SRCH( MSG_LEGACY_MED, "Unexpected slam rsp" );
  } /* if ( hdrsrchdrv_is_waiting_for_slam_rsp() ) */

} /* hdrsrchsync_process_slam_rsp() */


/*===========================================================================

FUNCTION HDRSRCHSYNC_SYNCHRONIZE

DESCRIPTION
  This function begins the synchronization process of setting the AT's
  concept of time to match the Access Network.

DEPENDENCIES
  Acquired system, fingers in lock, SCC Message recv'd

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  There will be a time discontinuity.
  Sync Timeout Timer is stopped.

===========================================================================*/

LOCAL void hdrsrchsync_synchronize ( void )
{
  hdrsrch_synchronize_params_type  *sync;
    /* Pointer to synchronization command parameters */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsync.sync_msg_received = TRUE;;
    /* Sync message has been received */

  sync = &hdrsrchsync.sync_params;
    /* point to the synchronize command parameters */

  hdrsrch_stop_timer( &hdrsrch.timeout_timer );
    /* Sync message received - Can't time-out anymore. */
  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "Synchronizing: time=%lx hstr=%d pn=%d",
                  sync->sync_time, sync->half_slot, sync->pilot_pn );


  hdrsrchsync.sync_time = sync->sync_time - HDRSRCH_SYNC_160MS_IN_FRAMES;
    /* Since "sync_time" is for a time 160ms after the time specified
       by "half-slot", we can decrement "sync_time" by two 80ms periods
       Remember the sync time for ISR */

  /* Adjust fingers for new pilot PN value. */
  if ( hdrsrchsync_jump_to_hyperspace( sync->pilot_pn ) == FALSE )
  {  
    hdrsrchsync_slam_failed();
      /* Timing change failed*/
  }
} /* hdrsrchsync_synchronize */


/*===========================================================================

FUNCTION HDRSRCHSYNC_SAVE_SYNC_SUBSTATE_CALLBACK

DESCRIPTION
  Save sync substate callback

DEPENDENCIES
  None

PARAMETERS
  substate_cb : call back for sync substate

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchsync_save_sync_substate_callback
( 
  hdrsrch_sync_substate_cb_type substate_cb 
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsync.sync_substate_cb = substate_cb;

}  /* hdrsrchsync_save_sync_substate_callback() */


/*===========================================================================

FUNCTION HDRSRCHSYNC_THRESH_INIT

DESCRIPTION
  Sync State thresh related initialization

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchsync_thresh_init( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsync.filt_c_i_thresh1 = hdrsrchset_ecio_to_eng( HDRSRCH_SYNC_FILT_C_I_THRESH1_DEFAULT );
    /* Convert linear Q9 */

  hdrsrchsync.filt_c_i_thresh2 = hdrsrchset_ecio_to_eng( HDRSRCH_SYNC_FILT_C_I_THRESH2_DEFAULT );
    /* Convert to linear Q9 */

  hdrsrchsync.thresh_drc = ( ( uint16 ) HDRSRCH_SYNC_THRESH_DRC_DEFAULT) & 0x7FFF;
    /* Set Thresh_DCR */

}  /* hdrsrchsync_thresh_init() */


/*=========================================================================

FUNCTION     : HDRSRCHSYNC_SEND_MIN_BASP_SINR_MSG

DESCRIPTION  : This function sends the message to set minimum SINR input to
               the predictor.

DEPENDENCIES : None

INPUT        : min_basp_sinr - minimum SINR input

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchsync_send_min_basp_sinr_msg
(
  uint16              min_basp_sinr
    /* minimum SINR input to the predictor */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  msgr_init_hdr( &hdrsrchsync.fw_msg.min_best_asp_sinr_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_MIN_BEST_ASP_SINR_CMD );
    /* Init message header */

  hdrsrchsync.fw_msg.min_best_asp_sinr_msg.predMinBestAspSinr = min_basp_sinr;
    /* Set the value */

  hdrsrchmsg_send_msg( &hdrsrchsync.fw_msg.min_best_asp_sinr_msg.hdr,
                       sizeof( hdrfw_min_best_asp_sinr_msg_t ) );
    /* Send the msg */
} /* hdrsrchsync_send_min_basp_sinr_msg */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSYNC_ENABLE_TUNE_AWAYS

DESCRIPTION
  Turns on the ability for tune aways to happen within the SYNC state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchsync_enable_tune_aways( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_add_to_wait_mask( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* We are now interested in primary chain events. */

  hdrsrchsync.tune_away.start_time_sclks = timetick_get();
    /* Save the current time in sclks. */

  hdrsrchsync.tune_away.no_tune_away = TRUE;
    /* There is no tune away as of now. */

  hdrsrchrf_retain_lock( HDRSRCHRF_PRI_CHAIN, hdrsrch_rf_unlock_cb );
    /* Register an unlock callback so that we may be interrupted. */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                "HDR LRE: SYNC enable tune_aways" );  /* REVISIT */

} /* hdrsrchsync_enable_tune_aways */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSYNC_DISABLE_TUNE_AWAYS

DESCRIPTION
  Turns off the ability for tune aways to happen within the ACQ state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchsync_disable_tune_aways( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void) timer_clr( &hdrsrchsync.tune_away.timer, T_NONE );
    /* Stop the tune away timer. */

  hdrsrchrf_unregister_unlock_cb( HDRSRCHRF_PRI_CHAIN );
    /* Unregister for unlock callbacks.  We now will respond to them. */

  hdrsrch_kill_signal( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* We are no longer interested in primary chain events. */

  hdrsrch_clear_tune_away_on_searcher_idle( );
    /* Clear tune away on search idle if not NULL. We don't want to perform
       pending tune way if there is any.
    */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH,
              "hdrsrchsync_disable_tune_aways" );  /* REVISIT */

} /* hdrsrchsync_disable_tune_aways */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSYNC_TA_TIMER_INIT

  Initialize RF lock timer after entering SYNC state

DESCRIPTION
  TBD

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  NOne

===========================================================================*/

LOCAL void hdrsrchsync_ta_timer_init( void )
{
  static boolean          tune_away_timer_defined = FALSE;
    /* initialize the extend lock timer to be not defined for first time */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* One time timer definition */

  if ( !tune_away_timer_defined )
  {
    timer_def( &hdrsrchsync.tune_away.timer, NULL, HDRSRCH_TCB, 
               HDRSRCH_PRI_CHAIN_EVENT_SIG, NULL, 0 );

    timer_def( &hdrsrchsync.tune_away.trm_pri_inv_timer, NULL, HDRSRCH_TCB, 
               0, hdrsrchsync_trm_priority_timer_cb, 0 );

    tune_away_timer_defined = TRUE;
  }
}/* hdrsrchsync_ta_timer_init */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSYNC_INIT

DESCRIPTION
  Sync State Initialization

DEPENDENCIES
  Acquired system, fingers in lock ...

PARAMETERS
  old_state - Last HDR Searcher State

RETURN VALUE
  None

SIDE EFFECTS
  TBD

===========================================================================*/

LOCAL void hdrsrchsync_init( hdrsrch_state_enum_type last_state )
{
#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
  static boolean   diversity_timer_defined = FALSE;
    /* define diversity rex timer only one time during powerup */
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_default_init( last_state );
    /* Default handling */

  if (hdrsrch_is_lre_enabled())
  {
    if ( last_state == HDRSRCH_ACQ_STATE )
    {
      hdrsrchsync_ta_timer_init();
        /* Initialize tuneaway timers */

      hdrsrchsync.tune_away.sync_trm_priority = HDRSRCHRF_SYNC;
        /* Initiatalize trm_priority for first time entry to sync state */
    }   
    else if ( last_state == HDRSRCH_SUSPENDED_SYNC_STATE )
    {
      hdrsrchsync_update_trm_priority();
        /* entering the sync suspend state hence request the RF chain 
        with appropriate priority */
    }

    hdrsrchrf_change_priority( 
      HDRSRCHRF_PRI_CHAIN, 
      hdrsrchsync.tune_away.sync_trm_priority );
      /* Will be decoding sync message and changing the RF reason accordingly */

    hdrsrchsync.sync_substate = HDRSRCHSYNC_DECODE_SYNC_MSG_SUBSTATE;
      /* Update the state of Sync substate */

    if ( last_state != HDRSRCH_SUSPENDED_SYNC_STATE )
    {
      hdrsrchsync_enable_tune_aways();
        /* Enable tune aways as soon as possible when entering sync state. */
    }
  }
  else
  {

    hdrsrch_add_to_wait_mask(HDRSRCH_PRI_CHAIN_EVENT_SIG);

    hdrsrchrf_retain_lock(HDRSRCHRF_PRI_CHAIN, hdrsrch_rf_unlock_cb);

  }

  if ( hdrsrchutil_is_brute_force_acq() == TRUE )
  {
    /* sync after brute-force acq */

    if ( hdrsrch.brute_force_params.time_for_acq_ms > 
         (  HDRSRCH_MIN_TIME_FOR_ACQ_SYNC_REVERSE_TIME_TRAN_MS 
            - HDRSRCH_SYNC_TIMEOUT_BRUTE_FORCE_MS ) )
    {
      /* acq takes too much time, we don't have enough time for sync */

      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "IRAT MEAS: brute-force acq took "
                           " long %d ms",
                      hdrsrch.brute_force_params.time_for_acq_ms );
    }

    /* Only start timer is not returning from SUSPENDED SYNC state */
    if ((last_state != HDRSRCH_SUSPENDED_SYNC_STATE) || 
                                      (!hdrsrch_is_lre_enabled()))
    {
      hdrsrchutil_start_stop_watch();
        /* Start record time used for sync */
    }

    /* TODO: Currently CP is not honoring or abiding by the overall time allowed
       for RTT which comes in request from LTE to DO RTT param time_allowed_ms
       CP has 2.5s timer for Acq and 5s timer for sync.
       SRCH has 2s timer for Brute force acq+sync, for RTT. But when this timer
       expires nothing is done in exit_brute_force_sync as CP call back is not
       set. CP sets the callback only after sync decode, via synchronize cmd */

    rex_set_timer( &hdrsrch.timeout_timer, HDRSRCH_SYNC_TIMEOUT_BRUTE_FORCE_MS );
    hdrsrch_add_to_wait_mask( HDRSRCH_TIMEOUT_SIG );
    /* Start a 1.38 second timeout timer, which will cause the SYNC state to
       exit if receiving the SYNC message takes too long. */


  }
  else 
  {
    /* a regular sync */

#ifdef FEATURE_FACTORY_TESTMODE
    if(ftm_get_mode() != FTM_MODE)
    {
      (void) rex_set_timer( &hdrsrch.timeout_timer, HDRSRCH_SYNC_TIMEOUT_MS);
      hdrsrch_add_to_wait_mask( HDRSRCH_TIMEOUT_SIG );
      /* Start a 5.12 second timeout timer, which will cause the SYNC state to
         exit if receiving the SYNC message takes too long.                 */
    }
#else
    rex_set_timer( &hdrsrch.timeout_timer, HDRSRCH_SYNC_TIMEOUT_MS);
    hdrsrch_add_to_wait_mask( HDRSRCH_TIMEOUT_SIG );
    /* Start a 5.12 second timeout timer, which will cause the SYNC state to
       exit if receiving the SYNC message takes too long. */
#endif
  }
  /* TBD */
  /* The sets are initialzied at acquisition */
  /*hdrsrchset_init_sets();*/
    /* Initialize set variables */

  /* TBD */
  /* Aset pilot has been created at pull in. */
  /*hdrsrchset_create_aset();*/

    /* Create the first aset pilot */

  hdrsrchlog_debug_display_fill_rec();
    /* Update HDR debug display record */ 

  hdrsrch_add_to_wait_mask( HDRSRCH_SCHEDULE_SIG );
    /* Watch for schedule timer signals */

  /* Check if:
     1. Searcher is still active from ACQ state - wait for the dump
        signal, before starting a new search
     2. Start searcher scheduling after rf tune if returning from
        SUSPEND SYNC state */
  if ( ( hdrsrchdrv.active_srch == SRCH_STATUS_INACTIVE ) &&
    (( last_state != HDRSRCH_SUSPENDED_SYNC_STATE ) || 
     ( !hdrsrch_is_lre_enabled() ) ))
  {
    /* Searcher is inactive */

    HDRSRCH_SIG_CLR( HDRSRCH_DUMP_SIG );
      /* Ignore any pending dump signals */

    hdrsrch_discard_srch_dump();
      /* Discard the srch dump */

    hdrsrchsync_sched( );
      /* Begin regular search schedule */
  }

  hdrdec_enter_mode( HDRDEC_MONITOR_MODE );
    /* set the decoder for idle/sync mode where the packets are delivered
       as soon as they are decoded */

  hdrsrchsync.sync_msg_received = FALSE;

  hdrsrchsync.hstr              = HDRHSTR_GET_COUNT();
    /* Get the hstr */
   
  hdrsrchsync_send_min_basp_sinr_msg( 0 );
    /* Set minBestAspSinr to 0 */

  if ( hdrsrch_is_lre_enabled() )
  {
    hdrsrchsync.waiting_for_close_loop = FALSE;
      /* Not waiting for TCXOMgr to give permission to close the TCXO loop. */

    if ( last_state == HDRSRCH_SUSPENDED_SYNC_STATE )
    {
      hdrdec_reset_pkt_sequences();
        /* Reset packet sequence numbers */

      hdrdec_activate();
        /* activate HDR decoder before going to 1x */

      hdrsrchsync.waiting_for_close_loop = TRUE;
        /* Only wait for AFC close loop cmd once */

      hdrsrchafc_request_continue_trk();
        /* Now we need to close the TCXO loop so we can resume sync.
           This will eventually cause AFC state machine to send
           HDRSRCH_AFC_CLOSE_LOOP_CMD to SYNC STM and finish
           SYNC resume init. */

      if(hdrsrchsync.sync_substate_cb != NULL)
      {
        ( *hdrsrchsync.sync_substate_cb )( HDRSRCH_SYNC_RESUME );
          /* entering sync init from suspend state (2a -> 2) inform the CP 
            to unpause the timer */
            
        HDR_MSG_SRCH( MSG_LEGACY_HIGH, "SYNC TIMEOUT: Indicate sync state change (2A -> 2) to CP ");
      }
      else
      {
        /* for Debug only */
        HDR_MSG_SRCH( MSG_LEGACY_HIGH, "SYNC TIMEOUT: Null pointer Callback !");
      }
    }

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
    if ( diversity_timer_defined == FALSE  )
    {
      /* define special timer to handle diversity enabling */
      rex_def_timer( &hdrsrchsync.div_timer,
                     HDRSRCH_TCB,
                     HDRSRCH_GP_SIG0_SIG );

      diversity_timer_defined = TRUE;
    }
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */
  }
  HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                  "Threshes: filt_c_i_1 0x%x(linQ25) filt_c_i_2 0x%x(linQ25)",
                  hdrsrchsync.filt_c_i_thresh1, 
                  hdrsrchsync.filt_c_i_thresh2 );

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Thresh drc 0x%x", hdrsrchsync.thresh_drc );

} /* hdrsrchsync_init( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSYNC_TUNE_BACK

DESCRIPTION
  Tune back to SYNC after visit to 1x.

DEPENDENCIES
  None

PARAMETERS
  status - Tune status

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchsync_tune_back
( 
  hdrsrchrf_tune_status_enum_type status
)
{

  int               rx0_agc, rx1_agc;
    /* Rx0 and Rx1 AGC measurements */

  boolean           diversity;
    /* Diversity settings */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( status != HDRSRCHRF_TUNE_SUCCESS )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Tune status %d not expected",
                   status);
  
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );

    return;
  }

  hdrsrchsync_enable_tune_aways();
    /* Enable tune aways after rf is tuned. */

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
  if ( HDRSRCHACQ_ACQ_DIV_ENABLED )
  {
    hdrsrchrf_get_rx_power( &rx0_agc, &rx1_agc, &diversity );
      /* get power measurements */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HDR LRE: SYNC Div:  RxAGC0=%ddBm", rx0_agc);
    if ( rx0_agc < hdrsrch.acq_div.acq_div_thresh0 )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                      "HDR LRE: SYNC Div:  enable diversity acq_div_thresh0=%d",
                      hdrsrch.acq_div.acq_div_thresh0 );

      hdrsrchrfdiv_diversity_ctrl( TRUE ); 
        /* Enable Diversity */ 
      
      hdrsrch_add_to_wait_mask( HDRSRCH_GP_SIG0_SIG );
      (void) rex_set_timer( &hdrsrchsync.div_timer, HDRSRCHACQ_DIVERSITY_ENABLE_TIME );
        /* Wait a few ms for TRM grant and diversity enable */
    }
  }
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

    HDRSRCH_SIG_CLR( HDRSRCH_DUMP_SIG );
      /* Ignore any pending dump signals */

    hdrsrch_discard_srch_dump();
      /* Discard the srch dump */

    hdrsrchsync_sched( );
      /* Begin regular search schedule */

} /* hdrsrchsync_tune_back */


/*==========================================================================

FUNCTION        HDRSRCHSYNC_GP_SIG0

DESCRIPTION
  This function processes gp_sig0 expiration event.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchsync_gp_sig0( void )
{
#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV

  int               rx0_agc, rx1_agc;
    /* Rx0 and Rx1 AGC measurements */

  boolean           diversity;
    /* Diversity settings */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* When timer expires, we check the RxAGC of diverstiy chain */
  
  hdrsrchrf_get_rx_power( &rx0_agc, &rx1_agc, &diversity );
    /* get power measurements */
  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                  "Sync Div: pri rxagc=%d diversity rxagc =%d,acq_div_thresh1=%d",
                  rx0_agc,rx1_agc, hdrsrch.acq_div.acq_div_thresh1);

  if ( rx1_agc <  hdrsrch.acq_div.acq_div_thresh1 )
  {
    /* We don't want to enable diversity chain if it is too weak */
    HDR_MSG_SRCH( MSG_LEGACY_MED,"Sync Div: disable diversity");
    hdrsrchrfdiv_diversity_ctrl( FALSE );
      /* Turn off diversity */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED,"Sync Div: Diversity remains enabled");
  }

  (void) rex_clr_timer( &hdrsrchsync.div_timer );
    /* clear the diversity timer if it is active */

  hdrsrch_remove_from_wait_mask( HDRSRCH_GP_SIG0_SIG ); 
    /* clear the signal wait mask */

#else
  hdrsrch_default_gp_sig0();
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

} /* hdrsrchsync_gp_sig0 */


/*==========================================================================

FUNCTION        HDRSRCHSYNC_TRM_PRIORITY_TIMER_CB

DESCRIPTION
  This function processes priority timer timeout event

DEPENDENCIES
  None

INPUTS
  data        : Timer callback data

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchsync_trm_priority_timer_cb
( 
  timer_cb_data_type              data
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  (void) timer_clr( &hdrsrchsync.tune_away.trm_pri_inv_timer, T_NONE );
    /* clear the trm_pri_inv_timer */

  hdrsrchsync.tune_away.trm_pri_inv_timer_expired = TRUE;

  HDRSRCH_SIG_SET( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* Handle it as part of rf_unlock_event */
} /* hdrsrchsync_trm_priority_timer_cb */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSYNC_RESUME_SYNC_INIT

DESCRIPTION
  SYNC State Resumption after tune-away

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  NOne

===========================================================================*/

void hdrsrchsync_resume_sync_init ( void )
{
  boolean tune_status;
    /* Tune to chan status */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_set_agc_acq_params( HDRSRCHRF_NORMAL_AGC_ACQ_MODE, 0, 0 );
    /* Set AGC acquisition params for normal mode. */

#ifdef FEATURE_HDR_REVB
  HDR_ASSERT ( hdrsrch.aset_channel_cnt <= HDRSRCH_MAX_NUM_CARRIERS );
    /* KW fix : Check to limit aset_channel_cnt to max */
#endif /* FEATURE_HDR_REVB */

  /* Enable RF and Tune to the channel */
  tune_status = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                          hdrsrch.aset_channel,
#ifdef FEATURE_HDR_REVB
                          hdrsrch.aset_demod,
                          hdrsrch.aset_channel_cnt,
#endif /* FEATURE_HDR_REVB */
                          hdrsrchsync_tune_back,
                          HDRSRCHSYNC_BAND_GRANT_TIMEOUT_MS );

  if ( tune_status == FALSE )
  {
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );

    return;
  }
}/* hdrsrchsync_resume_sync_init */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSYNC_DONE

DESCRIPTION
  Sync State Cleanup

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Clears the SYNC Message Timeout Timer.
  (Note: Probably already cleared due to a SYNCHRONIZE msg received, or
         the timer expiring.  Would only not be cleared yet if we were
         commanded to DEACTIVATE).

===========================================================================*/

LOCAL void hdrsrchsync_done( hdrsrch_state_enum_type next_state )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_dbg_wait_mask();

  hdrsrch_default_done( next_state );
    /* Use default handling */

  hdrsrch_dbg_wait_mask();
 
  if ( hdrsrch_is_lre_enabled() )
  {
    hdrsrchsync_disable_tune_aways();

    hdrsrch_dbg_wait_mask();

    if ( next_state == HDRSRCH_SUSPENDED_SYNC_STATE )
    {
      hdrsrch_stop_timer( &hdrsrchsync.tune_away.trm_pri_inv_timer );
        /* Clear the trm_pri_inv_timer while entering the suspend state */
    }
  }
  
  hdrsrchrf_unregister_unlock_cb( HDRSRCHRF_PRI_CHAIN );

  hdrsrch_dbg_wait_mask();

  hdrsrch_kill_signal( HDRSRCH_PRI_CHAIN_EVENT_SIG );
 
  hdrts_deregister_roll_callback( hdrsrchsync_frame_isr );

  hdrsrch_dbg_wait_mask();

  /* Tuneaway processing already disables div chain and also maintains
     HDRSRCH_SYNC_TIMEOUT_MS timeout timer across tuneaways. */
  if ((next_state != HDRSRCH_SUSPENDED_SYNC_STATE) || ( !hdrsrch_is_lre_enabled() ))
  {
 #ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Sync Div: disable diversity");
    hdrsrchrfdiv_diversity_ctrl( FALSE );

    hdrsrch_dbg_wait_mask();
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

    if ( hdrsrch_is_lre_enabled() )
    {
#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
    hdrsrch_kill_timer( &hdrsrchsync.div_timer );
      /* kill the timer, so that it may be used in other places */
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

      hdrsrch_kill_timer( &hdrsrchsync.tune_away.trm_pri_inv_timer );
        /* Cleanup TimeOut processing */
    }

    hdrsrch_kill_timer( &hdrsrch.timeout_timer );
      /* Cleanup TimeOut processing */
  }

  hdrsrch_kill_timer( HDRSRCHSYNC_SLAM_TIMER );
    /* Clear the slam timer, and remove from wait mask */

  hdrsrch_kill_signal( HDRSRCH_TIME_VALID_SIG );
    /* Clean up Time Valid processing */

  hdrsrch_kill_timer( HDRSRCHSYNC_FRAME_ROLL_TIMER );
    /* Clean up Frame Roll Timeout timer. */

  /* Skip if next state is SUSPENDED-SYNC state */
  if ((next_state != HDRSRCH_SUSPENDED_SYNC_STATE) || 
                                  (!hdrsrch_is_lre_enabled()))
  {
    hdrsrchlog_log_acq_info( );
      /* Log acq info packet when leaving SYNC state                       */

    hdrsrch_dbg_wait_mask();

    hdrsrchsync_send_min_basp_sinr_msg( 16 );
      /* Send the message to set minBestAspSinr to 16 (-15dB) */

    hdrsrch_dbg_wait_mask();
  }
	
  /* reset the TA count if next state is INACTIVE */
  if ( ( next_state == HDRSRCH_INACTIVE_STATE ) &&
       ( hdrsrchsync.tune_away.tune_away_cnt >= HDRSRCHSYNC_MAX_TA_ALLOWED_COUNT ) )
  {
     hdrsrchsync.tune_away.tune_away_cnt = 0;
     HDR_MSG_SRCH( MSG_LEGACY_MED, "SYNC: reset TA count");
  }
	
    /* execute the callback to indicate the CP that HDR srch completed processing sync state */
  if ( ( hdrsrchsync.sync_params.return_timing_cb != NULL) &&
     (( next_state != HDRSRCH_SUSPENDED_SYNC_STATE ) || 
      ( !hdrsrch_is_lre_enabled() )))
  {
    hdrsrchsync.sync_params.return_timing_cb(FALSE,0,0);
      /* Indicate reverse time transfer failure to CP */
  }

  hdrsrch_dbg_wait_mask();
} /* hdrsrchsync_done */


/* <EJECT> */
/*===========================================================================

FUNCTION      HDRSRCHSYNC_TUNE_AWAY_PROC

DESCRIPTION
  Stops search schedular

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrsrchsync_tune_away_proc( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrfdiv_diversity_ctrl( FALSE );
    /* Disable Diversity */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "SYNC Div: Disable diversity and Tune away");

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
  (void) rex_clr_timer( &hdrsrchsync.div_timer );
    /* clear the diversity timer if it is active */
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

  hdrsrch_stop_timer( &hdrsrch.schedule_timer );
    /* Stop scheduler ... we are tuning away */

  hdrsrchfing_deassign_all_fings( );
    /* Unassign fingers */

  hdrsrchutil_dec_deactivate( );
    /* Deactivate HDR decoder before going to 1x */

  hdrsrchdrv_shutdown_proc();
    /* Stop RX AGC */

  hdrsrchafc_stop_fll_tracking();
    /* Disable FLL tracking and save the FLL accum value. */

  hdrsrchrf_prep_to_sleep();
    /* Prepare the RF for sleep */

} /* hdrsrchsync_tune_away_proc */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSYNC_ON_SEARCH_IDLE_CB

DESCRIPTION
  This callback function is invoked when searcher is idle during tune away
  processing.
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchsync_on_search_idle_cb( void )
{

  /* Not worth continuing SYNC if it is taking too long and scheduled 1x demod wake-up is waiting. */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                "Complete Tuneaway in SYNC state" );

  hdrsrchsync_tune_away_proc();
    /* Complete SYNC tune away processing */

  hdrsrchsus_init_suspended_state();
    /* Begin SUSPENDED SYNC state */

} /* hdrsrchsync_on_search_idle_cb */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSYNC_CMD

DESCRIPTION
  This function ...

DEPENDENCIES
  TBD

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

LOCAL errno_enum_type hdrsrchsync_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type               status;
    /* Command completion status */

  int64                         sclks_left_till_ta;
    /* Sclks remaining until lock release */

  int64                         sclks_left_for_slam;
    /* Sclks available for slam operation */

  boolean                       slam_allowed = TRUE;
    /* By default allow slam */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "Received cmd %d", cmd->name );

  switch ( cmd->name )
  {
    case HDRSRCH_SYNCHRONIZE_CMD:

      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Synchronize Cmd PN=%d",
                      cmd->params.synchronize.pilot_pn );

      /* Check whether the delay in processing of sync cmd is greater than SCC cycle */
      if( hdrsrchutil_stop_timer_ms( cmd->params.synchronize.sync_cmd_delay ) 
        > SLOTS_TO_MS ( HDRTS_SLOTS_PER_SCC ) )
      {
        HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Delayed synchronize cmd ignored" );

        /* return failure for sync cmd */
        status = E_FAILURE;

        break;
      }

      if ( hdrsrch_is_lre_enabled() )
      {
        hdrsrchsync.sync_substate = HDRSRCHSYNC_SYNC_CMD_RECEIVED_SUBSTATE;
          /* Update the state of Sync substate */
      }

      hdrsrchsync.sync_params = cmd->params.synchronize;
        /* store synchronize parameters */

      if ( (hdrsrchsync.tune_away.no_tune_away == FALSE) 
                             && (hdrsrch_is_lre_enabled()))
      {
        sclks_left_till_ta = hdrsrchsync.tune_away.end_time_sclks - timetick_get_safe();
        /* Get sclks left till tune away */

        HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HDR LRE: Sclks before TA = %u", sclks_left_till_ta );

        sclks_left_for_slam = sclks_left_till_ta - HDRSRCHSYNC_MAX_SYNC_CMD_PROCESS_SCLKS;
          /* Get difference between the time required for sync to complete and sclks left for TA */

        if ( ( hdrsrchsync.tune_away.tune_away_cnt < HDRSRCHSYNC_MAX_TA_ALLOWED_COUNT ) 
           && ( sclks_left_for_slam < 0 ) )
        {
          /* if TA count < max TA count and sclks left is less than max time taken for sync then dont allow sync operation */
          slam_allowed = FALSE;

        }/* if ( sclks_left_for_slam < 0 ) */
      }/* if ( hdrsrchsync.tune_away.no_tune_away == FALSE ) */

      if ( slam_allowed || ( !hdrsrch_is_lre_enabled() ) )
      {
        if ( (hdrsrchsync.tune_away.no_tune_away == FALSE) && 
                                    (hdrsrch_is_lre_enabled()))
        {
           (void) timer_clr( &hdrsrchsync.tune_away.timer, T_NONE );
             /* Stop the tuneaway timer, no unlockby processing allowed after slam processing */
        }

        hdrsrch_on_searcher_idle( hdrsrchsync_synchronize );
          /* execute this function when search is idle */

        status = E_IN_PROGRESS;
          /* This command is in progress */
      }
      else
      {
        /* Tuneaway now */

        if(hdrsrchsync.sync_substate_cb != NULL)
        {
           ( *hdrsrchsync.sync_substate_cb )( HDRSRCH_SYNC_SUSPEND );
            /* Releasing the RF chain and entering suspend state (2 -> 2a) inform 
              the CP to pause the timer */

          HDR_MSG_SRCH( MSG_LEGACY_HIGH, "SYNC TIMEOUT: Indicate TA in sync state to CP ");
        }
        else
        {
          /* for debug only */
          HDR_MSG_SRCH( MSG_LEGACY_HIGH, "SYNC TIMEOUT: Null pointer Callback !");
        }

        hdrsrch_stop_timer( &hdrsrch.timeout_timer );
          /* Disable the timer when exiting the sync state */

        hdrsrchdrv_abort_search( FALSE );
          /* Abort pending searches, if any */

        hdrsrchsync_disable_tune_aways();
          /* Disable tune aways now because we do not permit processing unlock
             cancels which will abort the tune away at this stage of the
             process. */

#ifdef FEATURE_HDR_SELECTIVE_TXD
        hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_SUSPENDING );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

        hdrsrch_tune_away_on_searcher_idle( hdrsrchsync_on_search_idle_cb );
          /* change to suspended sync state, only when searcher is idle  */

        hdrsrchsync.tune_away.tune_away_cnt++;
          /* Count tune-away which results in Sync failure */

        status = E_FAILURE;
          /* Cannot process the sync cmd in suspend state */

        /* if the timing callback (hdrsrchsync.sync_params.return_timing_cb()) is not called
           then HDR CP will send Sync cmd when a sync message is decoded */

        HDR_MSG_SRCH( 
          MSG_LEGACY_HIGH, 
          "HDR LRE: Tune-away scheduled inbetween the slam operation hence suspend slam");
      }/* if ( slam_allowed ) */

      break;

    case HDRSRCH_IDLE_CMD:

      HDR_MSG_SRCH( MSG_LEGACY_MED,"Waiting for timing change to enter Idle" );

      status = E_SUCCESS;
        /* this command is complete since idle mode will be entered
           automatically after timing change is complete */
      break;

#ifdef FEATURE_HDR_BCMCS
    case HDRSRCH_UPDATE_BC_INFO_CMD:

      hdrsrchbc_set_bc_status( cmd->params.bc_info.bcmcs_status );

      status = E_SUCCESS;
      break;
#endif /* FEATURE_HDR_BCMCS */

    case HDRSRCH_DEACTIVATE_CMD:

      hdrsrchsync_update_acq_info( HDRLOG_ACQ_SYNC_FAILURE );
        /* Updates ACQ INFO for SYNC failure case                            */

      hdrsrchsync.tune_away.tune_away_cnt = 0;
        /* For deactivate cmnd reset the tune_away_cnt */

        /* Fall through                                                      */

    default:

      status = hdrsrch_default_cmd( cmd );
        /* Handle state-independent commands */

  } /* switch (cmd->name) */

  return status;

} /* hdrsrchsync_cmd(cmd) */



/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSYNC_TIMEOUT

DESCRIPTION
  This function ....

DEPENDENCIES
  TBD

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  TBD

===========================================================================*/

void hdrsrchsync_timeout( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsync.tune_away.tune_away_cnt = 0;
    /* Sync failed Reset the tune_away_cnt */

  if ( hdrsrchutil_is_brute_force_acq() == TRUE )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "IRAT: RTT SYNC Message Timeout (%dms)",
                  HDRSRCH_SYNC_TIMEOUT_BRUTE_FORCE_MS );
      /* Report the problem to DIAG */

     hdrsrchsync_exit_brute_force_sync();
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "SYNC Message Timeout (%dms)",
                  HDRSRCH_SYNC_TIMEOUT_MS );
      /* Report the problem to DIAG */

    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_SYNC_MSG_TIMEOUT );
      /* Let upper layer know the system has been lost, and deactivate*/
  }

} /* hdrsrchsync_timeout( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHSYNC_SCHED

DESCRIPTION
  This function starts a search and sets a timer for the next time a search
  should be scheduled.

DEPENDENCIES
  TBD

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  TBD

===========================================================================*/

LOCAL void hdrsrchsync_sched( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Program the search only if the searcher is idle */
  if ( hdrsrchdrv.active_srch == SRCH_STATUS_INACTIVE )
  { 
    if ( hdrsrchrfdiv_diversity_demod_is_enabled() ) 
    {
      hdrsrchdrv.srch_task_type = HDRSRCH_DIVERSITY_SRCH;
    }
    else
    {
      hdrsrchdrv.srch_task_type = HDRSRCH_NON_DIV_SRCH;
    }

    (void) hdrsrchutil_program_search_set( );
      /* Perform the standard search scheduling */
  }

  (void) rex_set_timer( &hdrsrch.schedule_timer, 
                        HDRSRCH_SYNC_SEARCH_INTERVAL_MS );
    /* And schedule next search request */

} /* hdrsrchsync_sched( ) */

/*===========================================================================

FUNCTION HDRSRCHSYNC_WEAK_HDR_COVERAGE

DESCRIPTION
  This function checks whether the HDR coverage is weak.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE:     HDR coverage is weak
  FALSE:    HDR coverage is strong
  
SIDE EFFECTS
  TBD

===========================================================================*/

boolean hdrsrchsync_weak_hdr_coverage( void )
{
  uint32                      filt_c_i;
    /* filtered C/I */

  uint16                     filt_drc;
    /* Filtered DRC */

  uint16                     hstr;
    /* HSTR value */

  int16                      slot_diff;
    /* Diffence in slot between this moment and when enetrimg sync state */

  boolean                    weak_hdr = FALSE;
    /* return value */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchsync.sync_msg_received == TRUE )
  {
    /* Sync message has been received so don't need to check HDR coverage */
    weak_hdr                 = FALSE;
    
  }
  else
  {

    filt_c_i                   =
      HDRMDSP_GET_FILT_BEST_ASP_SNR( hdrsrchset.subaset[ 0 ].demod_idx );
      /* Get the filtered C/I  in Q9 format */


    filt_drc                   =
      HDRDRC_GET_FILTER_OUTPUT_MC( hdrsrchset.subaset[ 0 ].demod_idx );
      /* Filtered value of DRC */

    hstr                       = HDRHSTR_GET_COUNT();
      /* half slot counter */

    slot_diff                  = (int16)hstr - (int16)hdrsrchsync.hstr;
    slot_diff                  = slot_diff / 2;
      /* slot diff */

#ifdef FEATURE_HDR_WEAK_COVERAGE_CHECK_THRESH1
  
    if ( ( slot_diff > 256 ) && (slot_diff < 768 ) )
    {
      if ( filt_c_i < hdrsrchsync.filt_c_i_thresh1 )
      {
        weak_hdr = TRUE;
      }
    }
    else /* if slot_diff is between 256 and 768 */

#endif /* FEATURE_HDR_WEAK_COVERAGE_CHECK_THRESH1 */

    {
      if ( slot_diff >= 768 )
      {
        if ( ( filt_c_i < hdrsrchsync.filt_c_i_thresh2 ) && 
             ( filt_drc < (uint32)hdrsrchsync.thresh_drc ) )
        {
          weak_hdr = TRUE;
        }
      
      } /* slot_diff > 768 */
    
    }/* if slot_diff is between 256 and 768 */
  
  }  /* if */

  return weak_hdr;

}  /* hdrsrchsync_weak_hdr_coverage() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHSYNC_DUMP

DESCRIPTION
  This function processes the searcher dump.

DEPENDENCIES
  TBD

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  TBD

===========================================================================*/

LOCAL void hdrsrchsync_dump( void )
{
  boolean                    valid;
    /* Flag to indicate whether or not searcher results are valid */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check the HDR coverage */  
  if ( hdrsrchsync_weak_hdr_coverage() == TRUE )
  {
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_SYNC_WEAK_HDR_COVERAGE );
      /* declare system lost since HDR coverage is weak */

    hdrsrchsync.tune_away.tune_away_cnt = 0;
      /* Sync failed Reset the tune_away_cnt */
  }
  else
  {
    if ( hdrsrchdrv.srch_list[0].sect_ptr != NULL )
    {
      /* This is a real sync state search, not a pipeline acquisition one */

      valid = hdrsrchutil_validate_searcher_results();
        /* Perform searcher result validation before using the results */

      if ( valid )
      {
        hdrsrchutil_process_search_set( NULL );
        /* Perform standard processing on the search results.
          No special ASET management callback required in Sync */
      }
      else
      {
        hdrsrchutil_invalidate_search_dump();
          /* invalidate searcher results */
      }

    }
    else
    {
      /* This dump is from a acquisition predetect search - ignore it and
         start the regular search schedule */

      hdrsrchsync_sched( );
    } /* if sect_ptr is not null */

  } /* if HDR coverage is weak */

} /* hdrsrchsync_dump( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHSYNC_INIT_SYNC_STATE

DESCRIPTION
  This function causes the state machine to begin sync state processing.

DEPENDENCIES
  System Acquired, fingers in lock, ...

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

===========================================================================*/

void hdrsrchsync_init_sync_state(void)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_change_state( &hdrsrchsync_state );
    /* Switch to idle state processing */

} /* hdrsrchsync_init_sync_state( ) */

/*==========================================================================

FUNCTION        HDRSRCHSYNC_PROCESS_LOW_PRI_RSP_IND

DESCRIPTION
  This function processes low priority responses and indications from FW.

DEPENDENCIES
  None

INPUTS
  msg - Response/indication

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchsync_process_low_pri_rsp_ind
(
   hdrsrchmsg_s_type         *msg
    /* Message */
)
{

/*-----------------------------------------------------------------------*/

  switch( msg->msg.hdr.id )
  {
    case HDR_FW_SRCH_FING_ASSIGN_SLAM_RSP:
    case HDR_FW_SRCH_SLAM_RSP:

      if ( msg->msg.hdr_msg.rsp.srch_slam.slamSuccess == TRUE )
      {
        hdrsrchsync_process_slam_rsp(  );
        /* process slam response for synchronization */
      }
      else
      {
        hdrsrchmsg_process_rcvd_rsp( HDRSRCH_MSTR_SLAM_RSP );

        HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Slam in FW failed, exit Sync state");

        hdrsrchsync_slam_failed();

        break;
      }

      /* Fall through */

    default:
      hdrsrch_default_process_low_pri_rsp_ind( msg );
      break;
  }
} /* hdrsrchsync_process_low_pri_rsp_ind */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTCSYNC_PROCESS_TUNE_AWAY

DESCRIPTION
  This function performs proper tune away procedure based on the time that
  is currently left on the lock and the current tune away state. It then
  schedules the next event to be processed.

DEPENDENCIES
  None

PARAMETERS
  extend_ms - maximum duration for which we can have the RF.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchsync_process_tune_away
(
  unsigned                   extend_ms,
    /* maximum duration for which we can have the RF */

  hdrsrchrf_unlock_event_enum_t  ta_event
    /* unlock event type */

)
{
  if ( extend_ms > HDRSRCHSYNC_MIN_RF_TUNEAWAY_MS )
  {

    timer_set(&hdrsrchsync.tune_away.timer, extend_ms - HDRSRCHSYNC_MIN_RF_TUNEAWAY_MS, 0, T_MSEC);
      /* Set the timer for the appropriate duration */

    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "HDR LRE: %dms - %dms (buffer) from tune-away Next event=%dms",
                    extend_ms,
                    HDRSRCHSYNC_MIN_RF_TUNEAWAY_MS,
                    extend_ms - HDRSRCHSYNC_MIN_RF_TUNEAWAY_MS );
  }
  else
  {
    /* Tuneaway now */

    hdrsrch_stop_timer( &hdrsrch.timeout_timer );
      /* Disable the timer when exiting the sync state */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "SYNC TIMEOUT: Disabled the Sync timeout timer");

    if(hdrsrchsync.sync_substate_cb != NULL)
    {
       ( *hdrsrchsync.sync_substate_cb )( HDRSRCH_SYNC_SUSPEND );
        /* Releasing the RF chain and entering suspend state (2 -> 2a) inform 
          the CP to pause the timer */

      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "SYNC TIMEOUT: Indicate TA in sync state to CP ");
    }
    else
    {
      /* for debug only */
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "SYNC TIMEOUT: Null pointer Callback !");
    }

    hdrsrchdrv_abort_search( FALSE );
      /* Abort pending searches, if any */

    hdrsrchsync_disable_tune_aways();
      /* Disable tune aways now because we do not permit processing unlock
         cancels which will abort the tune away at this stage of the
         process. */

#ifdef FEATURE_HDR_SELECTIVE_TXD
    hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_SUSPENDING );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

    hdrsrch_tune_away_on_searcher_idle( hdrsrchsync_on_search_idle_cb );
      /* change to suspended traffic state, only when searcher is idle  */

    if( ( ta_event == HDRSRCHRF_UNLOCK_BY ) || ( ta_event == HDRSRCHRF_UNLOCK_REQUIRED ) )
    {
      hdrsrchsync.tune_away.tune_away_cnt++;
        /* Count tune-away which results in Sync failure */
    }

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "HDR LRE: %dms <= %dms (buffer) from tune-away, start tune-away now",
                    extend_ms,
                    HDRSRCHSYNC_MIN_RF_TUNEAWAY_MS );
  }

}  /* hdrsrchsync_process_tune_away */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSYNC_RF_UNLOCK_EVENT

DESCRIPTION
  Handle the incoming RF lock event based on the current state of the retain
  lock unlock information.  Schedule a DRC ramp down if we are supposed to
  tune away. If the required tune away is urgent, just force an unlock
  immediately.

  It may transition to TC OFS state when it detects that HDR has been in 
  traffic state for too long ( with FEATURE_HDR_TC_OFS_ENHANCEMENT defined ).

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchsync_rf_unlock_event( void )
{

  unsigned                       ms_left;
    /* maximum duration for which we can have the RF */

  hdrsrchrf_retain_lock_state_t  rl_state;
    /* State of retain lock. */

  uint32                         sclks_left;
    /* Sclks remaining until lock release */

  uint32                         sclks_now;
    /* Sclks remaining until lock release */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &rl_state );
    /* Retrieve the retain lock state. */

  if ( hdrsrch_is_lre_enabled() )
  {
    if ( hdrsrchsync.tune_away.trm_pri_inv_timer_expired == TRUE )
    {     
      hdrsrchsync.tune_away.sync_trm_priority = HDRSRCHRF_SYNC;
        /* Hold RF chain with normal priority till 3 TAs */
      
      hdrsrchrf_change_priority( 
        HDRSRCHRF_PRI_CHAIN, 
        hdrsrchsync.tune_away.sync_trm_priority );
        /* Will be decoding sync message and changing the RF reason accordingly */
      
      hdrsrchsync.tune_away.tune_away_cnt = 0;
        /* Reset TA count */  

      hdrsrchsync.tune_away.trm_pri_inv_timer_expired = FALSE;

      HDR_MSG_SRCH_1( 
        MSG_LEGACY_HIGH,
        "TRM-U: Sync inversion timer expired, Sync TRM Priority %d", 
        hdrsrchsync.tune_away.sync_trm_priority );
    }
    else if ( hdrsrchsync.sync_substate == HDRSRCHSYNC_DECODE_SYNC_MSG_SUBSTATE )
    {
      /* In this state TA are allowed. On TA enter sync suspend state and resume after obtaining the RF chain */
    if( ( ( rl_state.event != HDRSRCHRF_UNLOCK_BY ) 
      && ( rl_state.event != HDRSRCHRF_UNLOCK_REQUIRED ) )
      || ( ( hdrsrchsync.tune_away.tune_away_cnt < HDRSRCHSYNC_MAX_TA_ALLOWED_COUNT ) ||
          ( hdrsrchsync.tune_away.sync_trm_priority == HDRSRCHRF_SYNC )))
    {
      /* If Sync message decode stopped because of TA for MAX TA times with Sync inversion priority,
      then ignore unlock by notification */

      HDR_MSG_SRCH_1( 
        MSG_LEGACY_HIGH,
        "HDR LRE: Sync msg decode inprogress, Unlock notification received with event %d", 
        rl_state.event );

      sclks_now = timetick_get();
      /* Current time in sclks. */ 

      sclks_left = hdrsrchutil_calc_time_to_tune_away( 
                                          sclks_now,
                                          (int32) HDRSRCHSYNC_MIN_RF_TUNEAWAY_SCLK,
                                          &rl_state,
                                          &hdrsrchsync.tune_away.no_tune_away,
                                          &hdrsrchsync.tune_away.end_time_sclks,
                                          NULL);
      /* How much more time should we stay in sync? */

      hdrsrchsync.tune_away.start_time_sclks = sclks_now;
      /* Keep track of the current time. */

      ms_left = timetick_cvt_from_sclk( sclks_left, T_MSEC );
        /* Convert lock extension to milliseconds. */

      hdrsrchsync_process_tune_away( ms_left , rl_state.event );
    }
    else
    {
      HDR_MSG_SRCH( 
        MSG_LEGACY_HIGH,
      "HDR LRE: Sync state interrupted for MAX TA with Sync inversion pri, cancelling current TA");
    }
    }
    else
    {
      /* Time sync and slamming inprogress and TA should be avoided */
        
      HDR_MSG_SRCH_1( 
        MSG_LEGACY_HIGH,
        "HDR LRE: Sync cmd in-progress, unlock event =%d, tuneaway for only unlock immediate/required", 
        rl_state.event );
        
      if ( ( rl_state.event == HDRSRCHRF_UNLOCK_IMMEDIATELY ) ||
                 ( rl_state.event == HDRSRCHRF_UNLOCK_REQUIRED ) )
      {
#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
        hdrsrchrfdiv_diversity_ctrl( FALSE );
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

        hdrsrchsync_disable_tune_aways();
        /* Unregister for unlock callbacks. */

        hdrsrchutil_system_lost( HDRSRCH_SYSLOST_ADVISE_UNLOCK_RF );
      }
    }
  }
  else
  {
    if ( rl_state.event == HDRSRCHRF_UNLOCK_IMMEDIATELY )
    {
      /* only honor immediate (1x page) */

     HDR_MSG_SRCH( MSG_LEGACY_MED, 
                  "Primary chain unlock immediately requested" );
      /* Print out debug message. */

      hdrsrchutil_system_lost( HDRSRCH_SYSLOST_ADVISE_UNLOCK_RF );
    }
  }
  
} /* hdrsrchsync_rf_unlock_event */

/* EJECT */
/*==========================================================================

FUNCTION HDRSRCHSYNC_PROCESS_INT_CMD

DESCRIPTION
  This function processes internal commands for SYNC state.

DEPENDENCIES
  None

INPUTS
  payload   - Command+payload to be processed

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchsync_process_int_cmd
(
  hdrsrchsm_cmd_payload_type                     *payload
    /* The internal command + payload */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  switch ( payload->cmd )
  {
    case HDRSRCH_AFC_CLOSE_LOOP_CMD:

      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "SYNC Process HDRSRCH_AFC_CLOSE_LOOP_CMD" );

      /* Closed loop */

      if ( hdrsrchsync.waiting_for_close_loop == TRUE )
      {
        hdrsrchsync.waiting_for_close_loop = FALSE;
          /* We don't actually track until we assign fingers, but this is close
             enough, there isn't any harm declaring this a bit early, and
             we currently don't check to see when we end up acquiring again
             when we resume from traffic. */

        hdrsrchsync_resume_sync_init();
          /* Now go ahead and resume traffic. */

}
      break;
    
    case HDRSRCH_AFC_TCXO_RELEASED_CMD:
      /* TCXO is released */
      break;

    default:
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Unexpected internal cmd %d at TC state",
                      payload->cmd );
      break;

  }  /* switch */
}  /* hdrsrchsync_process_int_cmd */

/*===========================================================================

FUNCTION HDRSRCHSYNC_UPDATE_TRM_PRIORITY

DESCRIPTION
  This function updates the current priority of sync state

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  HDRSRCHRF_SYNC        :    Default Sync TRM priority
  HDRSRCHRF_SYNC_INV :    Sync High TRM priority
  
SIDE EFFECTS
  TBD

===========================================================================*/

hdrsrchrf_reason_enum_t hdrsrchsync_update_trm_priority( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( hdrsrch_is_lre_enabled() )
  {
#ifdef FEATURE_HDR_TRM_UNIFICATION
    if( hdrsrchsync.tune_away.sync_trm_priority == HDRSRCHRF_SYNC_INV )
    {
      timer_set( &hdrsrchsync.tune_away.trm_pri_inv_timer, 
        HDRSRCHSYNC_TRM_PRIORITY_INV_DURATION_MS, 
        0, 
        T_MSEC );
        /* Restart the sync inversion timer if the priority is sync inversion */
    }
    else if( hdrsrchsync.tune_away.tune_away_cnt >= HDRSRCHSYNC_MAX_TA_ALLOWED_COUNT )
    {
      if( hdrsrchsync.tune_away.sync_trm_priority == HDRSRCHRF_SYNC )
      {
        hdrsrchsync.tune_away.sync_trm_priority = HDRSRCHRF_SYNC_INV;
        
        timer_set( &hdrsrchsync.tune_away.trm_pri_inv_timer, 
          HDRSRCHSYNC_TRM_PRIORITY_INV_DURATION_MS, 
          0, 
          T_MSEC );
          /* Hold RF chain with higher priority for 1sec */
      }

      hdrsrchsync.tune_away.tune_away_cnt = 0;
        /* Start new count */

      HDR_MSG_SRCH_1( 
        MSG_LEGACY_HIGH,
        "TRM-U: Max TA count reached, Sync TRM Priority %d", 
        hdrsrchsync.tune_away.sync_trm_priority );

    }
    else
#endif
    {
      hdrsrchsync.tune_away.sync_trm_priority = HDRSRCHRF_SYNC;
        /* Hold RF chain with normal priority till 3 TAs */
    }
  }
  else
  {
    hdrsrchsync.tune_away.sync_trm_priority = HDRSRCHRF_ACQUISITION;
      /* Without LRE use legacy priority */
  }

  return hdrsrchsync.tune_away.sync_trm_priority;
}  /* hdrsrchsync_weak_hdr_coverage() */


/*===========================================================================

FUNCTION HDRSRCHSYNC_GET_TRM_PRIORITY

DESCRIPTION
  This function returns the current priority of sync state

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  HDRSRCHRF_SYNC     :    Default Sync TRM priority
  HDRSRCHRF_SYNC_INV :    Sync High TRM priority
  
SIDE EFFECTS
  TBD

===========================================================================*/

hdrsrchrf_reason_enum_t hdrsrchsync_get_trm_priority( void )
{
  return hdrsrchsync.tune_away.sync_trm_priority;
}/* hdrsrchsync_get_trm_priority */
/*===========================================================================

FUNCTION HDRSRCHSYNC_RESET_TA_COUNT

DESCRIPTION
  This function resets the sync tune away count

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  TBD

===========================================================================*/

void hdrsrchsync_reset_ta_count( void )
{

  hdrsrchsync.tune_away.tune_away_cnt = 0;

  HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR LRE: Clear the tune away count" );

  return;
}/* hdrsrchsync_reset_ta_count */
