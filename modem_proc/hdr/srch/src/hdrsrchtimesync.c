/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         H D R    S R C H    TIMESYNC

                           Search Timesync Module

GENERAL DESCRIPTION
  This module provides time transfer functionality.

REGIONAL FUNCTIONS
  

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2014 - 2019 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchtimesync.c#2 $ $DateTime: 2019/11/19 23:12:56 $ $Author: pwbldsvc $

============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   --     ------------------------------------------------------- 
11/20/2019   vko/vke Reset finger info before doing reacq due to fast acq
06/26/2018   vke     Changes to process slam response status from fw
10/31/2017   vlc     Added check for sub ID before reading time snapshots.  
11/17/2015   rmv     Remove call to add/remove wait_mask from ISR context 
                     as that is leading to concurrent update 
07/23/2015   rmv     For reselection, call R&N after TRM exchange to get 
                     supported device
06/09/2015   tnp     Update feature flag configuration
05/15/2015   vko/vke Added TABASCO to featurization
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
04/01/2015   mbs     Supported RF interface change
02/16/2015   vke     Changes to use the Aset win size shared via IRAT
11/19/2014   sat     Setting freq. info before requesting chain from TRM.
08/07/2014   mbs     Added support for trm lock exchange during IRAT procedures
06/12/2014   kss     Corrected chipx8 read and enable isr after slam.
01/10/2014   mbs     Added BOLT specific changes to support reselection
12/18/2013   mbs     Added support for VSTMR changes in BOLT
09/03/2013   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
06/04/2013   mbs     Fixed an issue with frame number computaion
05/06/2013   mbs     Fixed compiler errors for BOLT build
02/19/2013   vlc     Changed F3 message.
02/11/2013   vko     Fixed compiler warnings.
01/23/2013   wsh     Initialize DIV/ARD on timesync
12/04/2012   mbs     Fixed compilation errors due to removal of LTE on Triton
10/28/2012   smd     Used uint32 to store rtc count.
10/19/2012   smd     Told fw whether the standby to active transition is for wakeup.
09/26/2012   mbs     Saved system lost cb and usability cb before fast acq
07/17/2012   smd     Added workarounds for issue of frame ISR never coming.
06/01/2012   mbs     Fixed possible race condition during time Xfer
04/27/2012   dsp     Fixed compiler warnings. 
04/19/2012   wsh     Fixed race cond. that lead to time tras cb being NULL
04/10/2012   smd     Got the timestamp of time transfer. 
03/19/2012   ljl     Fixed Dime compiler errors.
03/19/2012   mbs     Added Dime Time Transfer changes
01/11/2012   ljl     Added the parameter for search priority to hdrsrchdrv_enable_srch4().
11/10/2011   mbs     Enabled FW before sending it a command 
10/25/2011   ljl     Handled the failure cases during optimized HO.
10/21/2011   smd     Compensated the delay of RF path for time transfer.  
10/17/2011   rmg     Removed calls to obsolete clk regime APIs.
09/30/2011   ljl     Supported active HO.
08/11/2011   smd     Added an F3 message.
06/13/2011   ssu     Replaced time_sync_time_of_day() with 
                     hdrts_update_ts_time() in hdrsrchtimesync_on_time_valid(). 
05/19/2011   rmg     Updated HDR MCPM interface.
05/12/2011   ljl     Removed sleep clk estimate.
04/27/2011   rmg     Added HDR MCPM support changes.
04/27/2011   smd     IRAT L2DO updates for NikeL.
03/16/2011   smd     Included msm.h 
02/01/2011   rmg     Removed HDR_DATAPATH_RES access for NikeL builds.
01/12/2011   smd     Cleaned up code of clock configure for fast acq.
11/16/2010   smd     Put ant clock under software control for fast acq.
10/02/2010   smd     Fixed a bug of not considering HSTR wrap around.
09/09/2010   smd     Removed FEATUER_IRAT_CONNECTED_MODE. 
08/16/2010   lyl     Supported FW power management with suspend/resume.
08/12/2010   smd     Reverted the change of poking data_path reg to start 
                     RTC in idle mode meas. 
08/02/2010   smd     Added a timer to avoid stuck in timesync state.
07/25/2010   smd     Added clock gating support for fast acq.
06/29/2010   vlc     Fixed compiler warnings.
06/25/2010   smd     Supported hstr slam in slam cfg message.
                     Added fixes in sync time of the day.
06/23/2010   smd     Added setting FLL chan in fast acq.
06/21/2010   cnx     Fixed compiler warnings.
06/18/2010   smd     Added a temporary workaround to start rtc.
06/10/2010   kss     Remove cast of REX_ENABLE_PREMPTION due to Core change.
06/05/2010   smd/vlc Fixed lte ostmr width issue.
06/04/2010   smd/vlc Removed code that was no longer applicable for Fast Acq, 
                     changed F3 messages for clarity. 
05/27/2010   smd/vlc Added changes to get srch dump in Fast Acq mode.
05/18/2010   arm     Fixed high lint warnings. 
05/06/2010   smd     Moved HDRSRCH_TIMESYNC_RTC_DUMP_SUCCESS to a common 
                     header file.
04/15/2010   smd     Completed fast_acq feature.
04/06/2010   ljl     Fixed the order of REX premption calls. 
04/01/2010   smd     Fixed bugs in time transfer.
03/18/2010   smd     Updated mstr after time transfer is done.
03/18/2010   smd     Fixed bug of assuming wrong chip rate in RTC register.
03/18/2010   smd     Added channel info in pilot meas rsp for failure cases.
03/10/2010   smd     Removed TRAN_SYS_TIME_CMD.
03/05/2010   smd     Fixed bugs in LTE pilot measurement feature.
09/15/2009   smd     Created this file.
=========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"
#include "hdrsrch.h"

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
#include "hdrenc.h"
#include "hdrsrchstate.h"
#include "hdrsrchstates.h"
#include "hdrsrchi.h"
#include "hdrsrch.h"
#include "hdrsrchtask.h"
#include "hdrsrchutil.h"
#include "hdrsrchdef.h"
#include "hdrsrchi.h"
#include "hdrsrchtypes.h"
#include "hdrsrchfing.h"
#include "hdrsrchlog.h"
#include "hdrsrchmsg.h"
#include "hdrts.h"
#include "hdrdec.h"
#include "hdrdebug.h"

#include "hdrmdspmcr.h"
#include "time_svc.h"
#include "rex.h"
#ifndef FEATURE_HDR_DIME_MODEM
#include "sclk.h"
#endif /* FEATURE_HDR_DIME_MODEM */
#include "hdrsrchrfdiv.h"
#include "hdrsrchafc.h"
#include "msm.h"

#include "hdrsrchmcpm.h"

#ifdef FEATURE_HDR_BOLT_MODEM
#include "vstmr_lte.h"
#include "vstmr_hdr.h"
#endif /* FEATURE_HDR_BOLT_MODEM */

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/
#define HDRSRCH_TIMESYNC_DUMP_RTC_DELAY_CHIPX2  1024*2
  /* wait one slot to dump RTC. The register uses rate chipX2 */

#define HDRSRCH_TIMESYNC_RF_LOCK_MS                   500
  /* RF lock timer value */

#define HDRSRCH_TIMESYNC_PN_ROLL_IN_MS                26
  /* constant of how long a pn roll is in unit of ms */

#define HDRSRCH_TIMESYNC_RTC_WRAP_AROUND_CHIPX8       0x40000
  /* HDR RTC counter is in chip ratex8, and it wraps around at 32768 chip
     36768*8= 0x40000. */

#ifdef FEATURE_HDR_BOLT_MODEM
#define HDRSRCH_TIMESYNC_OSTMR_MAX_CNT               0x40000000
  /* LTE OSTMR uses 30 bit counter hence wraps around every 2^30 cnt */
#else
#define HDRSRCH_TIMESYNC_OSTMR_MAX_CNT               0x400000
  /* LTE OSTMR uses 22 bit counter hence wraps around every 2^22 cnt */
#endif /* FEATURE_HDR_BOLT_MODEM */

#define HDRSRCH_TIMESYNC_RTC_MASK_CHIPX8              0x3ffff
  /* HDR RTC counter has 19 bits since it wraps around at 0x40000 */

#define HDRSRCH_TIMESYNC_HSTR_WRAP_AROUND_IN_CHIP     0x80000
  /* HSTR rolls over 512 slots, which is 512*1024=0x80000 in chip */

#define HDRSRCH_TIMESYNC_CHIPS_PER_HALF_SLOT          1024
  /* evert half slot has 1024 chip */

#define HDRSRCH_TIMESYNC_22MS_IN_CHIPX8               26624*8
  /* 22 millisecond in chipx8. 26624/32768*26.67=21.6 ms */

#define HDRSRCH_TIMESYNC_HSTR_FRAME_MASK              0x1e0
  /* Rounds raw HSTR value down to a frame boundary */

#define HDRSRCH_TIMESYNC_HSTR_WRAP                    0x200
  /* HSTR wraps at 512 */

#define HDRSRCH_TIMESYNC_HSTR_MASK                    0x1FF
  /* HSTR has 9 bits */


#define HDRSRCH_TIME_SYNC_SLAM_TIMEOUT_MS             1000 /*ms*/
  /* MSTR slam should take effect immediately however give 1sec worth time
     for before timing out */

#define HDRSRCH_TIME_SYNC_MAX_TIME_IN_TIMESYNC          10  /* ms */
  /* Maximum time in timesync state. We don't expect to be stuck in this state
     for a long time. Either pilot meas or fast acq command should be followed
     with timesync command very soon
  */

#define HDRSRCH_TIME_SYNC_WAIT_FRAME_ISR_MS            17*1.667 /* ms */
  /* Wait for 17 slots for frame ISR. A frame ISR should come within 1
     frame (16 slots). If not we need to recover.
  */
/*==========================================================================

                     STATE DISPATCH TABLE

This section provides the dispatch functions for events that are received
by the task while in this state.

==========================================================================*/


LOCAL void            hdrsrchtimesync_init( hdrsrch_state_enum_type last );
LOCAL void            hdrsrchtimesync_done( hdrsrch_state_enum_type next );
LOCAL errno_enum_type hdrsrchtimesync_cmd ( const hdrsrch_cmd_type * cmd );
LOCAL void            hdrsrchtimesync_pri_chain_granted( void );
LOCAL void            hdrsrchtimesync_process_low_pri_rsp_ind
                      (
                        hdrsrchmsg_s_type         *msg
                      );
LOCAL void            hdrsrchtimesync_slam_timeout( void );
LOCAL void            hdrsrchtimesync_timeout( void );
LOCAL void            hdrsrchtimesync_on_time_valid( void );

LOCAL hdrsrch_state_struct_type hdrsrchtimesync_state =
{
  HDRSRCH_TIME_SYNC_STATE,
  hdrsrchtimesync_init,
  hdrsrchtimesync_done,
  hdrsrchtimesync_cmd,
  hdrsrch_default_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
  hdrsrch_default_sched,
  hdrsrch_default_lost_dump,
  hdrsrch_default_timeout,
  hdrsrchtimesync_slam_timeout,
  hdrsrchtimesync_timeout,
  hdrsrch_default_gp_sig0,
  hdrsrch_default_gp_sig1,
  hdrsrchtimesync_on_time_valid,
  hdrsrch_default_rxchipx16,
  hdrsrchtimesync_pri_chain_granted,
  hdrsrch_process_int_cmd_default,
  hdrsrchtimesync_process_low_pri_rsp_ind
};


/*==========================================================================

                        CONSTANTS

==========================================================================*/


/*==========================================================================

                     DATA

==========================================================================*/
typedef enum
{
  HDRSRCHTIMESYNC_TIMER_NONE = 0,
  HDRSRCHTIMESYNC_WAIT_CMD_AFTER_TIME_TRANS,
  HDRSRCHTIMESYNC_WAIT_FRAME_ISR
} hdrsrchtimesyn_timer_reason_enum_type;

typedef struct
{
  hdrsrch_tran_sys_time_params_type       params;
    /* Transfer System Time parameters */

  uint32                                  rtc_offset;
    /* RTC Offset in chiprate*8, MDSP get timing from it */

  uint64                                  slam_frame;
    /* Frame number when we set timing to firmware */

  boolean                                 time_transfer_done;
    /* Whether time transfer is successfully done */

  hdrsrch_cdma_and_lte_time_struct        cdma_and_lte_time_snapshot;
    /* A snapshot of CDMA time and LTE time */

  hdrsrch_fast_acq_params_type            fast_acq_params;
    /* Structure to store fast_acq parameters */

  hdrsrch_tran_sys_time_cb_type           tran_sys_time_ind_cb;
    /* Callback to indicate upper layer */

  hdrsrch_cdma_and_lte_time_struct        fast_acq_time;
    /* CDMA time for fast acq */

  uint16                                  hstr_at_slam;
    /* hstr number when fw do slam */

  hdrsrchtimesyn_timer_reason_enum_type   timer_reason;
    /* the reason to start timer gp_timer1 */
	
 } 
hdrsrchtimesync_struct_type;

LOCAL hdrsrchtimesync_struct_type        hdrsrchtimesync;

/* EJECT */

/*==========================================================================

                       MACROS

==========================================================================*/

/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_INIT

DESCRIPTION
  TimeSync State Initialization

DEPENDENCIES
  None

INPUTS
  last_state - Last HDR Searcher State

RETURN VALUE
  None

SIDE EFFECTS
  Many

==========================================================================*/

LOCAL void hdrsrchtimesync_init( hdrsrch_state_enum_type last_state )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* CDMA clocks are started when exiting inactive state. We don't need to 
     start them here */
  hdrsrch_default_init( last_state );
    /* Use default handling */

  /* Start a timer for timesync state. We don't want to be stuck in this state.
     We should receive either pilot_meas or fast_acq request after time transfer.
     We will wait them for 10ms. If not recieving them, the timer will exipres
     and we go back to inactive state.
  */
  (void) rex_set_timer( &hdrsrch.gp_timer1, 
                        HDRSRCH_TIME_SYNC_MAX_TIME_IN_TIMESYNC);

  hdrsrch_add_to_wait_mask( HDRSRCH_GP_TMR1_SIG );
  hdrsrchtimesync.timer_reason = HDRSRCHTIMESYNC_WAIT_CMD_AFTER_TIME_TRANS;

} /* hdrsrchtimesync_init( ) */


/*===========================================================================

FUNCTION HDRSRCHTIMESYNC_READ_SNAPSHOT

DESCRIPTION
  This function reads LTE STMR and HDR RTC snapshot dump. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtimesync_read_snapshot( void )
{
  uint32        lte_stmr_delta;
    /* The delta of LTE STMR */

  uint32        cdma_sys_timex8;
    /* CDMA system time in chipratex8 */
  
#ifdef FEATURE_HDR_BOLT_MODEM
  vstmr_ostmr_t vstmr_ostmr;
  vstmr_lte_view_s *lte_view = NULL;
  vstmr_hdr_view_s *hdr_view = NULL;
  uint32 ostmr = 0, ustmr = 0, hstmr = 0, pnroll_cx1 = 0;

  sys_modem_as_id_e_type sid = SYS_MODEM_AS_ID_1;
  vstmr_rtc_id lte_rtc_id = VSTMR_RTC_LTE_SUB0;
    /* Variables relating to subscription ID of LTE/HDR (same subscription) */

#else
  uint32        univ_stmr_snapshot1 = 0, /* First snapshot of univ timer */
                univ_stmr_snapshot2 = 0; /* Second snapshot of univ timer */

  const uint16  max_snapshot_attempts = 5;
  uint16        curr_snapshot_attempts = 0;
#endif /* FEATURE_HDR_BOLT_MODEM */
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "IRAT MEAS: Time snapshot is ready");

#ifdef FEATURE_HDR_BOLT_MODEM
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
    /* Get handle for both LTE and HDR RTC views.  While LTE view is subscription-specific,
       HDR view is always the default subscription */

  REX_DISABLE_PREMPTION();
  /* Read current OSTMR count and derive the XO and HSTMR counts */
  vstmr_ostmr = vstmr_lte_get_current_ostmr(lte_view);
  REX_ENABLE_PREMPTION();

  ostmr = vstmr_ostmr.ostmr_cnt;
  ustmr = vstmr_lte_ostmr_to_xo(lte_view, vstmr_ostmr);
  hstmr = vstmr_hdr_xo_to_hstmr(hdr_view, ustmr).cx8;
  pnroll_cx1 = vstmr_hdr_xo_to_hstmr(hdr_view, ustmr).pnroll_cx1;

  hdrsrchtimesync.cdma_and_lte_time_snapshot.lte_stmr = ostmr;
  hdrsrchtimesync.cdma_and_lte_time_snapshot.hdr_rtc = hstmr;

  HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
    "IRAT MEAS RTC read snapshot (sub_ID=%d), HDR_RTC:  0x%x, LTE STMR: 0x%x, pnroll_cx1:0x%x",
    sid,
    hdrsrchtimesync.cdma_and_lte_time_snapshot.hdr_rtc,
    hdrsrchtimesync.cdma_and_lte_time_snapshot.lte_stmr,
    pnroll_cx1
    );

#else /* FEATURE_HDR_BOLT_MODEM */

  REX_DISABLE_PREMPTION();

  do
  {
    HWIO_OUT( UNIV_STMR_STATUS_DUMP_CMD, 0x1 );
    /* enable dumping of HDR RTC and LTE STMR snapshot. 0x01 means dump them
        immediately. */

    univ_stmr_snapshot1 = HWIO_IN( UNIV_STMR_TIME_STATUS_RD );
    /* Obtain first snapshot of univ_stmr_time value   */

    hdrsrchtimesync.cdma_and_lte_time_snapshot.hdr_rtc = 
          ( HWIO_IN( RTC_HDR_TIME_STATUS_RD ) >> 2 ) & HDRSRCH_TIMESYNC_RTC_MASK_CHIPX8;
      /* Read HDR RTC value, it is in chipx32, 32 bits wide. Need to convert it
         to cx8 since we use cx8 in everywhere else */

    hdrsrchtimesync.cdma_and_lte_time_snapshot.lte_stmr = 
      HWIO_IN( O_STMR_TIME_STATUS_RD );
        /* Read lte stmr value*/

    univ_stmr_snapshot2 = HWIO_IN( UNIV_STMR_TIME_STATUS_RD );
    /* Obtain second snapshot of univ_stmr_time value   */

    HDR_MSG_SRCH( MSG_LEGACY_LOW, "IRAT MEAS: Time snapshot is ready");

    /* Now compare values from 1 and 2 snapshot. If they are identical, then
       we can be assured that no other RAT initiated dump before we were
       able to complete the reading of HDR RTX + ostmr values */

  } while ( ( univ_stmr_snapshot1 != univ_stmr_snapshot2 ) &&
            ( curr_snapshot_attempts++ < max_snapshot_attempts) );

  REX_ENABLE_PREMPTION();

  if ( curr_snapshot_attempts >= max_snapshot_attempts )
  {
    hdrsrchtimesync.time_transfer_done = FALSE;
    if ( hdrsrchtimesync.tran_sys_time_ind_cb != NULL )
    {
      hdrsrchtimesync.tran_sys_time_ind_cb( HDRSRCH_TRAN_SYS_TIME_FAILED );
        /* inform upper layers the status of time transfer */
    }
    HDR_MSG_SRCH ( MSG_LEGACY_FATAL,
            "IRAT MEAS: Unable to obtain time snapshot" );
    return;
  }

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
    "IRAT MEAS RTC read snapshot, HDR_RTC:  0x%x, LTE STMR: 0x%x",
     hdrsrchtimesync.cdma_and_lte_time_snapshot.hdr_rtc,
     hdrsrchtimesync.cdma_and_lte_time_snapshot.lte_stmr);

#endif /* FEATURE_HDR_BOLT_MODEM */


  hdrsrchtimesync.cdma_and_lte_time_snapshot.hdr_rtc += HDRSRCH_IRAT_HDR_RTC_COMPENSATION * 8;
  hdrsrchtimesync.cdma_and_lte_time_snapshot.hdr_rtc &= HDRSRCH_TIMESYNC_RTC_MASK_CHIPX8; 
    /* Compensate the delay of RF path. LTE OSTMR time is time at antena, HDR RTC is after
       demod. The difference is RF path delay. We will see a fixed offset in CDMA system time
       if we don't compensate it.
    */

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
    "IRAT MEAS RTC adjust, new HDR_RTC:  0x%x cb:%x", 
    hdrsrchtimesync.cdma_and_lte_time_snapshot.hdr_rtc,
    hdrsrchtimesync.tran_sys_time_ind_cb );

  if ( hdrsrchtimesync.cdma_and_lte_time_snapshot.lte_stmr >= hdrsrchtimesync.params.lte_ostmr)
  {
    lte_stmr_delta = ( hdrsrchtimesync.cdma_and_lte_time_snapshot.lte_stmr
                       - hdrsrchtimesync.params.lte_ostmr );
  }
  else
  {
#ifdef FEATURE_HDR_BOLT_MODEM
    lte_stmr_delta = ( hdrsrchtimesync.cdma_and_lte_time_snapshot.lte_stmr +
                          HDRSRCH_TIMESYNC_OSTMR_MAX_CNT
                             - hdrsrchtimesync.params.lte_ostmr ) ;
      /* Consider Wrap around of 2^HDRSRCH_TIMESYNC_OSTMR_MAX_CNT chips */
#else
    lte_stmr_delta = ( hdrsrchtimesync.cdma_and_lte_time_snapshot.lte_stmr + 0x400000
                       - hdrsrchtimesync.params.lte_ostmr ) ;
      /* Consider Wrap around of 2^22 chips */
#endif /* FEATURE_HDR_BOLT_MODEM */
  }

  hdrsrchtimesync.cdma_and_lte_time_snapshot.cdma_sys_time = 
          hdrsrchtimesync.params.cdma_sys_time + 
            ( lte_stmr_delta/HDRSRCH_LTE_TO_HDR_CHIP_RATE );
    /* calculate the CDMA system time when snapshot is dumped  */

  cdma_sys_timex8 = 
    (hdrsrchtimesync.cdma_and_lte_time_snapshot.cdma_sys_time & 0x7fff) << 3;
    /* (cdma_sys_time%32768)*8, since RTC is in cx8 
       RTC counter is running at chipx8 with a period of 32768(0x8000) chips. 
       Therefore, at cdma_sys_time, the RTC counter is (cdma_sys_time%0x8000)*8 
    */ 
 
  if (hdrsrchtimesync.cdma_and_lte_time_snapshot.hdr_rtc >= cdma_sys_timex8)
  {
     hdrsrchtimesync.rtc_offset =  
       hdrsrchtimesync.cdma_and_lte_time_snapshot.hdr_rtc - cdma_sys_timex8;
  }
  else
  {
     hdrsrchtimesync.rtc_offset = 
         (hdrsrchtimesync.cdma_and_lte_time_snapshot.hdr_rtc + 0x40000 )
          - cdma_sys_timex8;
       /* Consider wrap around of 32768 chips */
  }

  hdrsrchtimesync.time_transfer_done = TRUE;

  /* Update MSTR in hdrsrchdrv */
  hdrsrchdrv.mstr_valid = FALSE;
    /* Set mstr_valid flag to false. Then when adding pn to search list
       we use this flag to decide if the search is for pilot meas after 
       time transfer.
    */
  hdrsrchdrv.mstr_x8 = hdrsrchtimesync.rtc_offset ;
   /* Set MSTR with rtc_offset */

  if ( hdrsrchtimesync.tran_sys_time_ind_cb != NULL )
  {
    hdrsrchtimesync.tran_sys_time_ind_cb( HDRSRCH_TRAN_SYS_TIME_SUCCEEDED );
      /* inform upper layers the status of time transfer */
  }

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
     "IRAT MEAS LTE delta: 0x%x, new cdma time: 0x%x RTC_OFFSET(mstr_x8): 0x%x",
     lte_stmr_delta,
     hdrsrchtimesync.cdma_and_lte_time_snapshot.cdma_sys_time,
     hdrsrchtimesync.rtc_offset );

} /* hdrsrchtimesync_read_snapshot( ) */


/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_TRAN_SYS_TIME_CMD

DESCRIPTION
  Transfer CDMA system time

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchtimesync_tran_sys_time_cmd( void )
{
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchtimesync.time_transfer_done = FALSE;
    /* Reset time transfer done flag */

  hdrsrchtimesync.cdma_and_lte_time_snapshot.dump_sclk = timetick_get();
    /* Get sclk time when do dumping */

  hdrsrchtimesync_read_snapshot();
    /* Read LTE STMR and CDMA RTC snapshot */

} /* hdrsrchtimesync_tran_sys_time_cmd */

/*===========================================================================

FUNCTION HDRSRCHTIMESYNC_SET_TIME_VALID

DESCRIPTION
  This callback function is called when hdrts_set( ) has set the time.

DEPENDENCIES
  Must only be passed as a callback function for hdrts_set( ).

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

LOCAL void hdrsrchtimesync_set_time_valid( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_SIG_SET( HDRSRCH_TIME_VALID_SIG );
    /* Indicate time is valid */

} /* hdrsrchtimesync_set_time_valid( ) */


/*===========================================================================

FUNCTION HDRSRCHTIMESYNC_FRAME_ISR

DESCRIPTION
  This Interrupt Service Routine completes the time synchronization.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  There will be a time discontinuity.
  The ISR auto uninstalls itself.
  Eventually, hdrsrchtimesync_time_valid( ) will be called.

===========================================================================*/

LOCAL void hdrsrchtimesync_frame_isr( void )
{
  uint16  raw_hstr, hstr;
    /* Half-slot timing reference */

  uint16  sync_frame;
    /* Frame to synchronize time at */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
 hdrsrch_kill_timer( &hdrsrch.gp_timer1 );
    /* Clear the timer, and remove from wait mask */
  hdrsrchtimesync.timer_reason = HDRSRCHTIMESYNC_TIMER_NONE;
    /* clear the reason of starting timer */

  raw_hstr = HDRHSTR_GET_COUNT();
    /* Get raw HSTR value: 0 .. 0xffff half slots */

  hstr = (raw_hstr + 16) & HDRSRCH_TIMESYNC_HSTR_FRAME_MASK;
    /* Round raw HSTR to nearest next frame, as a 9 bit number. Since frame ISR usually
       fires at 2 slots before frame ending, plus 16 would make hstr round to the next
       frame boundary */

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
      "IRAT frame isr: HSTR_at_nearest_frame=0x%x hstr_at_slam=0x%x raw_hstr=0x%x",
      hstr, hdrsrchtimesync.hstr_at_slam, raw_hstr );

  hstr += HDRSRCH_TIMESYNC_HSTR_WRAP;
  hstr -= hdrsrchtimesync.hstr_at_slam & HDRSRCH_TIMESYNC_HSTR_FRAME_MASK;
   /* Now hstr is the number of half slots passed from the frame boundary 
      when slam happens to the next frame boundary
   */
  hstr &= HDRSRCH_TIMESYNC_HSTR_MASK;

    sync_frame = hstr / 32;
    /* Convert HSTR to frame after slam. Every frame has 32 half slots.
       sync_frame is number of frames from slam to next frame
    */

    /* Uninstall the roll handler -- not needed any more */

    hdrts_deregister_roll_callback( hdrsrchtimesync_frame_isr );

  HDR_MSG_SRCH_4( MSG_LEGACY_HIGH,
       "IRAT: HSTR_from_slam=0x%x sync_frame=0x%x, curr_frame=0x%x slam_frame=0x%x",
        hstr, sync_frame, hdrsrchtimesync.slam_frame + sync_frame, hdrsrchtimesync.slam_frame );

    hdrts_set( hdrsrchtimesync.slam_frame + sync_frame, hdrsrchtimesync_set_time_valid );
      /* Set HDR time (# of 26.7ms periods from UTC=0), and call
       hdrsrchtimesync_set_time_valid when time has been successfully set. 
       Need to give the frame number of next frame to hdrts
    */

} /* hdrsrchtimesync_frame_isr( ) */


/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_SET_TIMING_TO_FW

DESCRIPTION
  This function set CDMA system timing to FW.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
boolean hdrsrchtimesync_set_timing_to_fw ( void )
{
  boolean                     status = TRUE;
    /* status of setting timing to firmware */

  uint32                      time_delta_ms;
    /* distance from time transfer to fast acq in unit of ms */

  int32                       hdr_rtc_delta;
    /* delta of hdr rtc between two dumps */

  int32                       rtc_offset;
    /* rtc offset */

  uint32                      hstr;
    /* hstr number */

#ifdef FEATURE_HDR_BOLT_MODEM
  vstmr_hdr_view_s *hdr_view = NULL;
    /* HDR vstmr view */
#endif /* FEATURE_HDR_BOLT_MODEM */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "IRAT: fast acq: pass timing to fw ");
  REX_DISABLE_PREMPTION();

#ifdef FEATURE_HDR_BOLT_MODEM

  hdr_view = vstmr_hdr_get_view_handle
                  (
                    VSTMR_RTC_HDR_SUB0,
                    VSTMR_HDR_RTC_VIEW
                  );
    /* Get handle for HDR RTC view */

  hdrsrchtimesync.fast_acq_time.hdr_rtc =
              vstmr_hdr_get_current_hstmr(hdr_view).pnroll_cx8;
    /* Read current RTC value in cx8 from VSTMR */

#else
  HWIO_OUT( UNIV_STMR_STATUS_DUMP_CMD, 0x1 );
    /* enable dumping of HDR RTC and LTE STMR snapshot. 0x01 means dump them
       immediately.
    */

  hdrsrchtimesync.fast_acq_time.hdr_rtc = 
         ( HWIO_IN( RTC_HDR_TIME_STATUS_RD ) >> 2 ) &
                                HDRSRCH_TIMESYNC_RTC_MASK_CHIPX8;
    /* Read HDR RTC value, it is in chipx32, 32 bits wide. Need to convert it
       to cx8 since we use cx8 in everywhere else 
     */
#endif /* FEATURE_HDR_BOLT_MODEM */

  REX_ENABLE_PREMPTION();

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                  "IRAT: fast acq original hdr_rtc=0x%x ",
                  hdrsrchtimesync.fast_acq_time.hdr_rtc );

  hdrsrchtimesync.fast_acq_time.hdr_rtc += HDRSRCH_IRAT_HDR_RTC_COMPENSATION * 8;
  hdrsrchtimesync.fast_acq_time.hdr_rtc &= HDRSRCH_TIMESYNC_RTC_MASK_CHIPX8; 
    /* Compensate the delay of RF path. We have compensate it in first time transfer.
       Need to do it here again.
    */

  hdrsrchtimesync.fast_acq_time.dump_sclk = timetick_get();
    /* get sclk time when dumping hdr rtc */

  time_delta_ms = timetick_cvt_from_sclk(
      hdrsrchtimesync.fast_acq_time.dump_sclk - 
         hdrsrchtimesync.cdma_and_lte_time_snapshot.dump_sclk, T_MSEC );
    /* Compute the time distance from last dump of hdr rtc to this dump */

  if ( time_delta_ms > HDRSRCH_TIMESYNC_PN_ROLL_IN_MS )
  {
    /* It has been too long from time transfer command to fast acq command */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "IRAT: fast acq is more than 26ms after time transfer ");
    status = FALSE;
    return status;
  }

  /* Within a PN roll hdr rtc won't be roll over twice. We should be able to
     calculate pair of CDMA system time and hdr_rtc */

  hdr_rtc_delta = hdrsrchtimesync.fast_acq_time.hdr_rtc -
           hdrsrchtimesync.cdma_and_lte_time_snapshot.hdr_rtc;
  hdr_rtc_delta += HDRSRCH_TIMESYNC_RTC_WRAP_AROUND_CHIPX8;
  hdr_rtc_delta &= HDRSRCH_TIMESYNC_RTC_MASK_CHIPX8;
    /* Compute hdr rtc delta, with consideration of wrap around */

  hdrsrchtimesync.fast_acq_time.cdma_sys_time = 
      hdrsrchtimesync.cdma_and_lte_time_snapshot.cdma_sys_time + ( hdr_rtc_delta >> 3 );
    /* Get cdma sys time at the moment of this dump */

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                  "IRAT: fast acq hdr_rtc=0x%x cdma_sys_time=0x%x",
                  hdrsrchtimesync.fast_acq_time.hdr_rtc,
                  hdrsrchtimesync.fast_acq_time.cdma_sys_time 
                 );

  hstr = hdrsrchtimesync.fast_acq_time.cdma_sys_time %
             HDRSRCH_TIMESYNC_HSTR_WRAP_AROUND_IN_CHIP;
  hstr /= HDRSRCH_TIMESYNC_CHIPS_PER_HALF_SLOT;
    /* hstr = (cdma_sys_time%0x80000)/1024 */
      
  rtc_offset = hdrsrchtimesync.fast_acq_time.hdr_rtc - 
                   ( hdrsrchtimesync.fast_acq_time.cdma_sys_time << 3 );
  rtc_offset += HDRSRCH_TIMESYNC_RTC_WRAP_AROUND_CHIPX8;
  rtc_offset &= HDRSRCH_TIMESYNC_RTC_MASK_CHIPX8;
    /* Calculate rtc offset */

  hdrsrchtimesync.rtc_offset = rtc_offset;
  hdrsrchdrv.mstr_x8 = rtc_offset;
    /* Update rtc offset */

  hdrsrchtimesync.slam_frame = hdrsrchtimesync.fast_acq_time.cdma_sys_time >> 15;
    /* frame_number = cdms_system_time/32768 */

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
                  "IRAT: fast acq rtc_offset=0x%x hstr=0x%x frame=0x%x",
                  rtc_offset, hstr, hdrsrchtimesync.slam_frame
                 );

  hdrsrchdrv_start_slam( TRUE, hstr, ( uint32 )rtc_offset,
                         hdrsrchtimesync.fast_acq_time.cdma_sys_time,
                         hdrsrchtimesync.fast_acq_time.hdr_rtc );
    /* Pass timing to firmware */

  hdrsrch_add_to_wait_mask( HDRSRCH_GP_TMR0_SIG );
     /* Watch for time out signal */
  (void) rex_set_timer( &hdrsrch.gp_timer0, HDRSRCH_TIME_SYNC_SLAM_TIMEOUT_MS );
      /* Poll timer for checking for MSTR slam */

  return status;
} /* hdrsrchtimesync_set_timing_to_fw */


/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_PROCESS_SLAM_RSP

DESCRIPTION
  This function processes slam response for fast acq.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchtimesync_process_slam_rsp
(
  uint64 fw_cdma_sys_time,
  uint16 hstr_at_slam
)
{
  uint32  hdr_rtc_cx8 = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_kill_timer( &hdrsrch.gp_timer0 );
   /* Clear the timer to wait slam rsp, and remove from wait mask */

  hdrsrchutil_enable_roll();
    /* Register the PN Roll callback */

  hdrts_abort_sleep();
    /* Re-enable time stamp service's demod roll isr.  Nothing installed yet. */

  REX_DISABLE_PREMPTION();

  hdrsrchtimesync.hstr_at_slam = hstr_at_slam;
    /* hstr number when firmware does hstr and mstr slam */
  hdrsrchtimesync.slam_frame = fw_cdma_sys_time >> 15;
    /* frame_number = cdms_system_time/32768 */

#ifndef FEATURE_HDR_BOLT_MODEM
  hdr_rtc_cx8 = ( HWIO_IN( HDR_RTC_CNT )) << 2;
#else
  hdr_rtc_cx8 = hdrts_get_rtc_cx8();
#endif /* FEATURE_HDR_BOLT_MODEM */

  HDR_MSG_SRCH_5( MSG_LEGACY_HIGH,
       "IRAT: hdr_rtc_cx8=0x%x fw_time=0x%x hstr at slam=0x%x frame#=0x%x mstr_cx8=0x%x", 
       hdr_rtc_cx8,
       fw_cdma_sys_time,
       hdrsrchtimesync.hstr_at_slam,
       hdrsrchtimesync.slam_frame,
       hdrsrchtimesync.rtc_offset );

  hdr_rtc_cx8 += HDRSRCH_TIMESYNC_RTC_WRAP_AROUND_CHIPX8 ;
  hdr_rtc_cx8 = ( hdr_rtc_cx8 - hdrsrchtimesync.rtc_offset )& HDRSRCH_TIMESYNC_RTC_MASK_CHIPX8 ;
    /* now hdr_rtc_cx8 is the time from the start of the frame */

  if ( hdr_rtc_cx8 > 14*2048*8 )
  {
    /* we are already after slot 14, wait 2 more slots to install frame isr. Otherwise
       the time between installing frame isr and isr firing may be too close. It might
       trigger the issue of frame isr never gets fired
    */
    HDR_MSG_SRCH( MSG_LEGACY_HIGH,"IRAT fast acq: wait 4ms then install frame isr");
    hdrsrch_timed_wait_usecs( 4000 );
  }

  hdrsrch_add_to_wait_mask( HDRSRCH_GP_TMR1_SIG );
     /* Watch for time out signal */
  (void) rex_set_timer( &hdrsrch.gp_timer1, HDRSRCH_TIME_SYNC_WAIT_FRAME_ISR_MS );
                                                                    
      /* Poll timer for checking for MSTR slam */
  hdrsrchtimesync.timer_reason = HDRSRCHTIMESYNC_WAIT_FRAME_ISR;

  hdrts_register_roll_callback( hdrsrchtimesync_frame_isr );
    /* Install PN roll handler*/

  hdrsrch_add_to_wait_mask( HDRSRCH_TIME_VALID_SIG );
      /* Advance state machine when system time is set */

  hdrsrchdrv.mstr_valid = TRUE;
    /* Now that we have jumped to hyperspace, MSTR is valid */

  REX_ENABLE_PREMPTION();


}/* hdrsrchtimesync_process_slam_rsp */

/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_FAST_ACQ_CMD

DESCRIPTION
  Process FAST_ACQ command.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchtimesync_fast_acq_cmd
(
   const hdrsrch_fast_acq_params_type* p_fast_acq_params 
     /* parameters of fast acq command */
)
{
  hdrsrchrf_trm_freq_info_type  freq_info;
    /* Band/chan info for TRM reservation. */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchtimesync.fast_acq_params = *p_fast_acq_params;
    /* store parameters */

  /* Limit to max win size from: Table 6.6.6.5-1. Search Window Sizes */
  if( hdrsrchtimesync.fast_acq_params.srch_window_size == 0xFF )
  {
    hdrsrchtimesync.fast_acq_params.srch_window_size = HDRSRCH_WINSIZE_60_CHIPS;
      /* For window size 0xFF use the default value */
  }
  else if( hdrsrchtimesync.fast_acq_params.srch_window_size > HDRSRCH_WINSIZE_452_CHIPS )
  {
    hdrsrchtimesync.fast_acq_params.srch_window_size = HDRSRCH_WINSIZE_452_CHIPS;
      /* Limit to maximum window size */
  }

  hdrsrch.system_lost = p_fast_acq_params->system_lost_cb;
    /* The SYSTEM LOST parameter is needed everywhere in HDR Search! */

  hdrsrch.system_usable_cb =
    p_fast_acq_params->usability_cb;
    /* The SYSTEM USABLE param is needed everywhere in hdr search !*/

  HDR_MSG_SRCH( MSG_LEGACY_LOW,
                  "IRAT fast acq: syslost/usable cb set");

  /* request RF chain */

  hdrsrch_add_to_wait_mask( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* Watch for Lock Granted signal */

  memset(&freq_info, 0, sizeof(freq_info));
  freq_info.channel_list[0] = hdrsrchtimesync.fast_acq_params.channel.chan_num;
  freq_info.freq_info.num_channels = 1;
  freq_info.freq_info.band = hdrsrchtimesync.fast_acq_params.channel.band;
  hdrsrchrf_set_trm_freq_info( &freq_info );
    /* Convey band/chan info update for TRM interaction. */

  if( hdrsrchrf_exchange( HDRSRCH_TIMESYNC_RF_LOCK_MS,
                          HDRSRCHRF_DEMOD_PAGE ) == TRUE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_LOW, "Primary chain exchanged" );
      /* Print out debug message. */
  }
  else
  {
     HDR_MSG_SRCH( MSG_LEGACY_HIGH, "TRM lock could not be exchanged" );
  }

    hdrsrchrf_request_and_notify( HDRSRCHRF_PRI_CHAIN,
                                  HDRSRCHRF_RX_BEST,
                                HDRSRCH_TIMESYNC_RF_LOCK_MS,
                                HDRSRCHRF_DEMOD_PAGE,
                                hdrsrch_rf_grant_cb );
    /* Notify this module when RF resources have been locked */

} /* hdrsrchtimesync_fast_acq_cmd  */


/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_PREPARE_FOR_REACQ

DESCRIPTION
  Prepare for transition to REACQ state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
LOCAL void hdrsrchtimesync_prepare_for_reacq ( void )
{

  hdrsrchfing_set_demod_carrier_mask( HDRSRCHFING_DEMOD_CARRIER_0_MASK );
    /* Enabled demod carrier 0 */

  /* Set ASET channel */
#ifdef FEATURE_HDR_REVB
   hdrsrch.aset_channel_cnt = 1;
   hdrsrch.aset_channel[0] = hdrsrchtimesync.fast_acq_params.channel;
   hdrsrch.aset_demod[0] = HDRSRCH_DEMOD_IDX_0;
      /* Give initial value of demod_idx, channel and channel count */
#else
   hdrsrch.aset_channel = hdrsrchtimesync.fast_acq_params.channel;
     /* Give initial value of channel */
 
#endif /* FEATURE_HDR_REVB */

  hdrsrchfing_reset_all_fings();
    /* Reset finger entries */

  /* Create a new ASET */
  hdrsrchset_create_aset_sect(
      hdrsrchtimesync.fast_acq_params.pilot_pn,
      1,                                        /* RPC cell index*/
      3                                         /* mac index */
    );

  hdrsrchutil_save_and_set_aset_win_size ( hdrsrchtimesync.fast_acq_params.srch_window_size );
    /* Set the Aset window size received */

  hdrsrchutil_set_win_center_to_mstr();
    /* initialize window center to MSTR to start with */

  hdrsrch_add_to_wait_mask( HDRSRCH_SCHEDULE_SIG );
    /* Watch for schedule timer signals */

  hdrsrchsync_send_min_basp_sinr_msg( 16 );
    /* Set minBestAspSinr to 0 */

  hdrsrchdrv_send_hyperspace_cfg_msg( HDRSRCH_SET_HYPERSPACE_MODE );
    /* Write jump to hyperspace flag to 1 so that the preamble detection
       happens only on even half slot boundaries */

  hdrsrchreacq_set_fast_acq_report(
      hdrsrchtimesync.fast_acq_params.fast_acq_cb );

  hdrsrchreacq_init_reacq_state();
    /* Transit to REACQ state */

}


/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_PREPARE_FOR_TC_HO

DESCRIPTION
  Prepare for transition to TC state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
LOCAL void hdrsrchtimesync_prepare_for_tc_ho ( void )
{

  uint32                            agc_rtc_offset;
    /* RTC offset to use for fast and resume AGC acquisition mode */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_add_to_wait_mask( HDRSRCH_SCHEDULE_SIG |
                            HDRSRCH_DUMP_SIG | 
                            HDRSRCH_LOSTDUMP_SIG );
    /* Watch for schedule timer signals */

  hdrsrchsync_send_min_basp_sinr_msg( 16 );
    /* Set minBestAspSinr to 0 */

  hdrsrchdrv_send_hyperspace_cfg_msg( HDRSRCH_SET_HYPERSPACE_MODE );
    /* Write jump to hyperspace flag to 1 so that the preamble detection
       happens only on even half slot boundaries */

  hdrdec_reset_pkt_sequences();
    /* Reset packet sequence numbers */

  hdrdec_activate();
    /* activate HDR decoder before going to 1x */

  hdrdec_enter_mode( HDRDEC_MONITOR_MODE );
    /* Set the decoder for idle/sync mode where the packets are delivered
       as soon as they are decoded.  Usually decoder mode is set in sync
       state. Since we skipped Sync state, we need to set it here. */                                  

  agc_rtc_offset = ( hdrsrchtimesync_get_rtc_offset() + 288*8 ) & 0x3ffff;
    /* Calculate pilot position for AGC measurements. We skew the center
       by 288 chips so that we allow the LNA gain state and DC offsets
       to settle. */

  hdrsrchrf_set_agc_acq_params( HDRSRCHRF_FAST_AGC_ACQ_MODE,
                                agc_rtc_offset,
                                0 );
    /* Set AGC Acq mode to fast */

  hdrsrchfing_restore_fing_thresh();
    /* Restore the finger in-lock/out-of-lock thresholds if any is pending */

  if ( hdrsrchtimesync.fast_acq_params.fast_acq_cb != NULL )
  {
    hdrsrchtimesync.fast_acq_params.fast_acq_cb( HDRSRCH_FAST_ACQ_SUCCEEDED );
      /* fast acq succeeded */
  }
} /* hdrsrchtimesync_prepare_for_tc_ho */

/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_PRI_CHAN_GRANTED

DESCRIPTION
  Prepare for transition to REACQ state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
LOCAL void hdrsrchtimesync_pri_chain_granted ( void )
{

  boolean     status = FALSE;
    /* status of whether set timing to firmware succeeds */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchmcpm_set_state( HDRSRCHMCPM_WAKE_UP_REQ );
    /* Tell MCPM to turn on clocks grouped under wake-up */

  hdrsrchrf_init();
    /* initialize RF driver afresh */

  hdrsrch_remove_from_wait_mask( HDRSRCH_PRI_CHAIN_EVENT_SIG );

  hdrmdsp_fws_resume_app( );
    /* Wake up FW is it's been suspended by hdr app */

  hdrmdsp_config_hdr_app( HDRFW_STATE_ACTIVE, TRUE, FALSE ); 
    /* Put MDSP in active mode */

  hdrsrchrfdiv_start_stm();
    /* Start div stm */

  hdrsrchrfdiv_user_diversity_pref( hdrsrch.div_ctrl, hdrsrch.div_band_mask );
    /* set user preference for antenna diversity setting */

  hdrsrchdrv_enable_srch4( SRCH4_PRIO_HDR_REACQ_PAGING );
    /* Enable SRCH4 for HDR */

  hdrsrch_add_to_wait_mask( HDRSRCH_DUMP_SIG | HDRSRCH_LOSTDUMP_SIG );
    /* Wait for Dump signals, and perhaps Lost Dump signals */

  hdrenc_init();
    /* Enable HDR encode and modulator. It is used being enabled in acq state.
       Since we skipped acq state, enabling encoder should be done here */

  if ( hdrsrchtimesync.fast_acq_params.reason == HDRSRCH_FAST_ACQ_RESELECTION )
  {
    hdrsrchafc_set_fll_chan( hdrsrchtimesync.fast_acq_params.channel );
      /* Init FLL channel */

    /* Assume ASP always starts from 0 and there is no hole in ASP assignment */
    hdrsrchmsg_basp_ho_ovrd( HDRMDSP_DEMOD_CARRIER_0, 0 );
      /* Set the best ASP to the first piot after slamming is done */
  }

  status = hdrsrchtimesync_set_timing_to_fw();
    /* Pass cdma system timing to firmware. MSTR_SLAM message will be sent to
       firmware. */

  if ( status == FALSE )
  {
    /* the operation to set timing to firmware fails */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "IRAT fast acq: failed to set timing to fw, send ind");
    hdrsrchtimesync.fast_acq_params.fast_acq_cb( HDRSRCH_FAST_ACQ_FAILED );
      /* send indication to cp */
    hdrsrchst_init_inactive_state();
     /* Go back to inactive state */
  }
  else
  {
    /* Do nothing here.*/
  }

}/* hdrsrchtimesync_pri_chain_granted */


/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_SET_CHAN

DESCRIPTION
  Set channel info to AFC stm.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchtimesync_set_chan
(

 const hdrsrch_tc_assignment_params_type *tca_params
    /* TCA parameters */
)
{
  hdrsrchafc_chan_list_type            chan_list;
    /* Multi-carrier channel list */

  int8                                 i;
    /* loop index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset( &chan_list, 0, sizeof( hdrsrchafc_chan_list_type ) );
    /* Initialized the structure before being passed to the hdrsrchafcsm */

  for( i=0; i<tca_params->num_subaset; i++ )
  {
    chan_list.carrier[tca_params->subaset_list[i].demod_idx].enable = TRUE;
    chan_list.carrier[tca_params->subaset_list[i].demod_idx].chan = 
                                    tca_params->subaset_list[i].channel;

    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                    "demod idx %d, band %d channel %d ", 
                    tca_params->subaset_list[i].demod_idx,
                    chan_list.carrier[tca_params->subaset_list[i].demod_idx].chan.band,
                    chan_list.carrier[tca_params->subaset_list[i].demod_idx].chan.chan_num );
  }

  hdrsrchafc_set_chan( &chan_list );
    /* Set channel info. Assume AFC stm is at inactive state */

}


/*===========================================================================

FUNCTION HDRSRCHTIMESYNC_CMD

DESCRIPTION
  This function processes commands in the TimeSync state.

DEPENDENCIES
  None

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

LOCAL errno_enum_type hdrsrchtimesync_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type                     status = E_SUCCESS;
    /* Command completion status */

  hdrsrch_lte_pilot_meas_status_type  pilot_meas_status = PILOT_MEAS_SUCCESS;
    /* Pilot measurement request status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "Received cmd %d in TimeSync state", 
                  cmd->name );

  switch ( cmd->name & HDRSRCHCMD_CMD_MASK )
  {
    case  HDRSRCH_TRAN_SYS_TIME_REQ:
      hdrsrchtimesync.params = cmd->params.tran_sys_time_req;
        /* store parameters */
#ifdef FEATURE_HDR_BOLT_MODEM
      hdrsrchtimesync.params.lte_ostmr &= 0x3fffffff;
        /* lte ostmr is 30 bit wide */
#else
      hdrsrchtimesync.params.lte_ostmr &= 0x3fffff;
        /* lte ostmr is 22 bit wide */
#endif /* FEATURE_HDR_BOLT_MODEM */

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
        "IRAT MEAS TranSysTimeReq, ostmr:  0x%x, cdma time: 0x%x ",
         hdrsrchtimesync.params.lte_ostmr,
         hdrsrchtimesync.params.cdma_sys_time);

      hdrsrchtimesync_tran_sys_time_cmd();
        /* transfer cdma system time */

      break;

    case  HDRSRCH_LTE_PILOT_MEAS_REQ:
      HDR_MSG_SRCH_5( MSG_LEGACY_HIGH, 
        "IRAT MEAS PilotMeas Req, rf_dev: %d, sect_cnt: %d pn[0]: %d gap start: %d len: %d",
         cmd->params.lte_pilot_meas_req.lte_rfm_param.target.rf_device,
         cmd->params.lte_pilot_meas_req.sector_count,
         cmd->params.lte_pilot_meas_req.pn_list[0],
         cmd->params.lte_pilot_meas_req.gap_start,
         cmd->params.lte_pilot_meas_req.gap_length
      );

      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
        "IRAT MEAS PilotMeas Req,win_size:%d ",
         cmd->params.lte_pilot_meas_req.window_size
      );

      HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
        "IRAT MEAS PilotMeas Req, band:%d chan:%d rot_err:%d online srch: %d",
         cmd->params.lte_pilot_meas_req.lte_rfm_param.target.channel.band,
         cmd->params.lte_pilot_meas_req.lte_rfm_param.target.channel.chan_num,
         cmd->params.lte_pilot_meas_req.rot_error,
         cmd->params.lte_pilot_meas_req.online_srch
      );

      hdrsrch_kill_timer( &hdrsrch.gp_timer1 );
       /* Clear the timer, and remove from wait mask */
      hdrsrchtimesync.timer_reason = HDRSRCHTIMESYNC_TIMER_NONE;
        /* clear the reason of starting timer */

      if (hdrsrchtimesync.time_transfer_done == FALSE)
      {
        /* Time transfer failed. No need to proceed pilot measurement. 
           Send a failure response here */

        HDR_MSG_SRCH( MSG_LEGACY_HIGH, "IRAT: LTE_PILOT_MEAS_REQ failed:time transfer is not done ");
        hdrsrchmsg_send_lte_pilot_meas_rsp( 
          PILOT_MEAS_TIME_TRAN_FAIL,
          cmd->params.lte_pilot_meas_req.lte_rfm_param.target.channel );

        status = E_FAILURE;
        hdrsrchst_init_inactive_state();
          /* Go back to inactive state */
      }
      else
      {
        /* process LTE pilot meas command */
        pilot_meas_status =
            hdrsrchutil_lte_pilot_meas_req_cmd( &cmd->params.lte_pilot_meas_req );
        if ( pilot_meas_status == PILOT_MEAS_SUCCESS )
        {
          /* Doing pilot measurement in MEAS OFS state  */

          hdrsrchofs_init_ofs_state();
            /* transit to idle ofs state */
        }
        else
        {
          /* Send a failure response */
          HDR_MSG_SRCH( MSG_LEGACY_HIGH, "IRAT: LTE_PILOT_MEAS_REQ failed ");

          hdrsrchmsg_send_lte_pilot_meas_rsp( 
            pilot_meas_status,
            cmd->params.lte_pilot_meas_req.lte_rfm_param.target.channel );


          status = E_FAILURE;
          hdrsrchst_init_inactive_state();
            /* Go back to inactive state */
        }
      }
      break;

    case HDRSRCH_FAST_ACQ_CMD :

      hdrsrch_kill_timer( &hdrsrch.gp_timer1 );
       /* Clear the timer, and remove from wait mask */
      hdrsrchtimesync.timer_reason = HDRSRCHTIMESYNC_TIMER_NONE;
        /* clear the reason of starting timer */

      if (hdrsrchtimesync.time_transfer_done == FALSE)
      {
        /* Time transfer is not done, cannot perform fast acq */

        HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                      "IRAT: FAST_ACQ failed:time transfer is not done ");
        status = E_FAILURE;
        hdrsrchst_init_inactive_state();
          /* Go back to inactive state */
      }
      else
      {
        /* Time transfer is done successfully, transit to REACQ state */

        HDR_MSG_SRCH( MSG_LEGACY_HIGH, "IRAT: Process FAST_ACQ");
        hdrsrchtimesync_fast_acq_cmd( &(cmd->params.fast_acq_req) );

        status = E_IN_PROGRESS;
      }
      break;

    case HDRSRCH_TC_ASSIGNMENT_CMD:

      hdrsrchtimesync_set_chan( &(cmd->params.tc_assign ));
        /* Set channel info */

      hdrsrchtc_init_connected_state(  );
        /* Transition to connected state */

      status = E_AGAIN;
        /* Retry this command once we have switched to the Connected State */

      break;

    default:
      status = hdrsrch_default_cmd( cmd );
        /* Handle state-independent commands */

  } /* switch (cmd->name) */

  return status;

} /* hdrsrchtimesync_cmd(cmd) */


/*==========================================================================

FUNCTION        HDRSRCHTIME_DONE

DESCRIPTION
  TimeSync State Exit Code

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

==========================================================================*/

LOCAL void hdrsrchtimesync_done( hdrsrch_state_enum_type next_state )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchtimesync.time_transfer_done = FALSE;
  hdrsrchtimesync.tran_sys_time_ind_cb = NULL;

  hdrsrch_kill_signal( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* clear the lock granted signal if it was set */
  hdrsrch_kill_timer( &hdrsrch.gp_timer0 );
  hdrsrch_kill_timer( &hdrsrch.gp_timer1 );
    /* clean up timers */
 hdrsrchtimesync.timer_reason = HDRSRCHTIMESYNC_TIMER_NONE;
    /* clear the reason of starting timer */

  hdrsrch_kill_signal( HDRSRCH_TIME_VALID_SIG );
    /* Clean up Time Valid processing */

} /* hdrsrchtimesync_done() */


/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_INIT_TIMESYNC_STATE

DESCRIPTION
  This function switches the HDR Searcher state to TimeSync state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchtimesync_init_timesync_state( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrch_change_state( &hdrsrchtimesync_state );
    /* Enter the TimeSync state */

} /* hdrsrchtimesync_init_timesync_state() */


/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_GET_TIME_SNAPSHOT

DESCRIPTION
  This function returns the snapshot of CDMA time and LTE time.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchtimesync_get_time_snapshot
(
   hdrsrch_cdma_and_lte_time_struct * p_cdma_lte_snapshot
)
{
  p_cdma_lte_snapshot->hdr_rtc = 
        hdrsrchtimesync.cdma_and_lte_time_snapshot.hdr_rtc;

  p_cdma_lte_snapshot->lte_stmr = 
        hdrsrchtimesync.cdma_and_lte_time_snapshot.lte_stmr;

  p_cdma_lte_snapshot->cdma_sys_time = 
        hdrsrchtimesync.cdma_and_lte_time_snapshot.cdma_sys_time;

} /* hdrsrchtimesync_get_time_snapshot */


/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_GET_RTC_OFFSET

DESCRIPTION
  This function returns the rtc offset after time transfer is done

DEPENDENCIES
  None

RETURN VALUE
  RTC offset

SIDE EFFECTS
  None

==========================================================================*/
uint32 hdrsrchtimesync_get_rtc_offset( void )
{
  return hdrsrchtimesync.rtc_offset;
}/* hdrsrchtimesync_get_rtc_offset */


/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_SET_CB

DESCRIPTION
  This function set the call back for time transfer command

DEPENDENCIES
  None

RETURN VALUE
  

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchtimesync_set_cb( hdrsrch_tran_sys_time_cb_type cb_func )
{
  hdrsrchtimesync.tran_sys_time_ind_cb = cb_func;
} /* hdrsrchtimesync_set_cb */

/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_PROCESS_LOW_PRI_RSP_IND

DESCRIPTION
  This function processes low priority responses and indications from FW.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchtimesync_process_low_pri_rsp_ind
(
   hdrsrchmsg_s_type         *msg
    /* Message */
)
{

/*-----------------------------------------------------------------------*/

  switch( msg->msg.hdr.id )
  {
    case HDR_FW_SRCH_SLAM_RSP:
      if ( msg->msg.hdr_msg.rsp.srch_slam.slamSuccess == TRUE )
      {
        hdrsrchtimesync_process_slam_rsp(
                      msg->msg.hdr_msg.rsp.srch_slam.slamCdmaSystemTime,
                      msg->msg.hdr_msg.rsp.srch_slam.hstrCount );
        /* Process slam rsp for acq */
      }
      else
      {
        hdrsrchmsg_process_rcvd_rsp( HDRSRCH_MSTR_SLAM_RSP );

        HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Slam in FW failed, exit time-sync state");
        
        hdrsrchtimesync_slam_timeout();

        break;
      }
      /* Fall through */

    default:
      hdrsrch_default_process_low_pri_rsp_ind( msg );
      break;
  }
} /* hdrsrchtimesync_process_low_pri_rsp_ind */


/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_SLAM_TIMEOUT

DESCRIPTION
  This function processes slam timeout and failed slam.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchtimesync_slam_timeout( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "IRAT: fast acq Slam failure - timeout" );

  hdrsrchtimesync.fast_acq_params.fast_acq_cb( HDRSRCH_FAST_ACQ_FAILED );
    /* send indication to cp */
  hdrsrchst_init_inactive_state();
   /* Go back to inactive state */

} /* hdrsrchacq_slam_timeout */

/*==========================================================================

FUNCTION        HDRSRCHTIMESYNC_TIMEOUT

DESCRIPTION
  This function processes GP_Timer1 timeout, to prevent srch stucks in timesync
  state.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchtimesync_timeout( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 if ( hdrsrchtimesync.timer_reason == HDRSRCHTIMESYNC_WAIT_CMD_AFTER_TIME_TRANS )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "IRAT: HDR TimeSync waits too long - timeout" );

    hdrsrchst_init_inactive_state();
     /* Go back to inactive state */
  }
  else if ( hdrsrchtimesync.timer_reason == HDRSRCHTIMESYNC_WAIT_FRAME_ISR )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "IRAT: HDR TimeSync waits frame ISR too long - timeout" );
    hdrsrchtimesync.fast_acq_params.fast_acq_cb( HDRSRCH_FAST_ACQ_FAILED );
      /* send indication to cp */
    hdrsrchst_init_inactive_state();
     /* Go back to inactive state */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "IRAT: unknown time out reason");
  }
} /* hdrsrchacq_slam_timeout */

/*===========================================================================

FUNCTION HDRSRCHTIMESYNC_ON_TIME_VALID

DESCRIPTION
  This function is called from task context when system time has been set.

DEPENDENCIES
  Called in response to the HDRSRCH_TIME_VALID signal.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtimesync_on_time_valid( void )
{

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "IRAT fast acq: Synchronize Time of Day to HDR System Time" );

  hdrts_update_ts_time();
    /* Synchronize Time of Day to HDR System Time */

  hdrsrch_complete_cmd( );
    /* The fast acq command is now complete */

  hdrsrch_init_div_sm();

  if ( hdrsrchtimesync.fast_acq_params.reason == HDRSRCH_FAST_ACQ_RESELECTION )
  {
    hdrsrchtimesync_prepare_for_reacq();
      /* Reselection */
  }
  else
  {
    hdrsrchtimesync_prepare_for_tc_ho();
      /* TC HO */
  }

} /* hdrsrchtimesync_on_time_valid */

#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */
