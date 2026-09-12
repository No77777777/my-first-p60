/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=

         H D R    S R C H       S U S P E N D E D      S T A T E


GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2001 - 2021 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
                
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=

=============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchsus.c#2 $ $DateTime: 2021/05/18 05:54:37 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------- 
05/18/2021   vaa     Fix for compilation errors in MSIM disable build flavor
02/28/2017   vke     APS feature compilation warning fix
02/20/2018   vko     Use OVHD reason for 4a to 3a state change
02/20/2018   svu     Fixed KW issue
02/07/2017   vke     FR36756 Changes to update TRM for APS state change
02/05/2018   vke     Fix for connected mode ASdiv switch failure
01/24/2018   vke     Changes to resume txd in traffic only after rf tune
11/07/2017   vko     Change reason from Page demod to OVHD, after coming from 
                     traffic
11/07/2017   vke     Change RF reason to page demod only for traffic to 
                     idle state transistion
08/24/2017   rmv     Mainline FEATURE_HDR_QTA_THRU_TRM 
08/24/2017   vko     Add support for 1xSRLTE+W
10/14/2016   vke     Changes to clear the tune away count when deactivating
                     from sync suspend state
10/12/2016   rmv     Added timed_wait_for_high_pri_rsp for debug purpose 
05/27/2016   vko     QSH framework for events capturing
02/01/2016   rmv     When disablig Tx in QTA due to OFS/TCA, stop FLL tracking
01/20/2016   wsh     Fixed incorrect ARD/ASDiv transistion involving 4d state
12/22/2015   rmv     Allow system lost cmd while sys loss is pending 
12/07/2015   wsh     Block all but deactivate cmd when sys loss is pending
11/24/2015   wsh     Added logic to handle DIV grant after TX in QTA
10/06/2015   vlc     In function hdrsrchsus_idle_mode_cmd(), make sure DO cleans 
                     up IRAT TRM grant and deinits LTE.
09/02/2015   vke     Changes to E_AGAIN deactivate cmd inside QTA
08/18/2015   arm     Fixed QTA featurization
07/29/2015   vke     Changes to implement sync trm inversion priority
07/17/2015   vke     Changes to E_AGAIN IDLE_CMD inside QTA
07/16/2015   wsh     Resend FreqID to FW coming out of suspend 
07/10/2015   wsh     Fixed sys loss not processed in time 
06/26/2015   wsh     Fixed using demod_page with RxTx resource 
06/18/2015   tnp     Fix compiler warnings
06/03/2015   mbs/vlc Added support for D2L redirection during DO traffic.
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
05/04/2015   mbs     Supported QTA using TRM API
03/26/2015   kss     Added use of hdrsrchrf_get_rf_device(). 
03/05/2015   rmv     If empty QTA gap, do not wait for FW AGC TRK response 
02/04/2015   wsh     Added support 2->2a transition, removed PA on requirement
02/04/2015   arm     Check if primry chain is settled before sending QTA_END 
                     to TRM
12/17/2014   arm     Removed deprecated TRM APIs in HDR
12/12/2014   wsh     Fixed ASDiv statics getting reset in 4a/4c 
12/04/2014   arm     Changes to support new unified micro priority chan
11/27/2014   vke     Changes to R&N with minimum duration to avoid 
                     the deadtime in 4a to 4 state change
11/19/2014   wsh     Added DR-DSDS support for ASDiv
11/19/2014   vke/sat Changes to store QTA winning client id and share with TRM
10/31/2014   vke     Skip disabling ARD SM in 4a to 3a state change 
                     when QTA is enabled 
10/29/2014   arm     Called RF API to disable Intelliceiver during QTA gap.
10/17/2014   wsh     Fixed reporting wrong mode to TRM in 4a->3a transistion
10/01/2014   arm     Enabled QTA for DR mode.
10/01/2014   arm     Changes to support HDR band avoidance.
09/29/2014   arm     DR-DSDS code cleanup
09/22/2014   arm     HDR QTA code cleanup
09/09/2014   vke     Redesign fix for connection close during QTA gap
09/01/2014   sat     Fixing Klocwork issues
08/28/2014   arm     Changes to support QTA with diversity
08/22/2014   vke     Delay connection close during QTA gap
08/20/2014   arm     Disable Tx after qta if doing OFS or channel change.
08/12/2014   tnp     Fix compiler warnings
08/03/2014   arm     Changes to request RX_ANY chain in DR mode for idle. 
07/31/2014   arm     DO QTA: clean up after empty G QTA gap.
07/29/2014   tnp     FR22375: Fix Medium compiler warnings in HDR
07/16/2014   vke     Set trm extension flag to hold the BG traffic 
                     so that GSM FW is awake before QTA
06/04/2014   arm     Changes to support DO to G QTA
05/05/2014   ljl     Added HDRSRCH_CHANNEL_CHANGE_AND_NSET_UPDATE_CMD.
04/21/2014   sat     Changes to update MCS/FW of Num Carriers when in suspended state
04/07/2014   vke     Report different indication to CP when searcher moves from 
                     3a to 3f when compared with 3a to 3
01/15/2014   wsh     Added DSDS ASDiv support
12/26/2013   bb      Changes to send TUNE WAY stop message during 4A to 
                     non 4 state transitions
07/19/2013   arm     Merged DSDS feature for triton. 
11/29/2013   sat     NV control for Lock Release Enhancement (LRE)
01/09/2014   dsp     Fixed race condition between IDLE_MODE_CMD and 
                     rf granted in 4a -> 3a transition.
06/27/2013   vke     If sync cmd is received in suspend state then complete 
                     the cmd with e_failure 
06/26/2013   wsh     Added Selective TxD feature
05/15/2013   vke     Fix for not changing the TRM request priority for idle 
                     state from traffic suspend state
06/06/2013   vke     Added handling of sync suspend in hdrsrchsus_lock_rf
                     to request for RF with reason HDRSRCHRF_ACQUISITION
05/25/2013   rkc     Added SYNC suspend state for Lock Release Enhancement (LRE)
05/02/2013   ljl     Saved scc offset and rate.
02/11/2012   smd     Don't process TCA in idle suspend state. 
10/19/2012   smd     Told fw whether the standby to active transition is for wakeup.
10/09/2012   vlc     Added ASP ranking enable to Traffic Suspend Init. 
09/14/2012   wsh     Reset chain state before calling Req&Notify
08/24/2012   kss/wsh Fixed TAP mode turning off after tuneaway and OFS
08/15/2012   smd     Supported idle tune away. 
07/26/2012   wsh     Fixed ARD going into INACTIVE after 4a->4c/4c->4a
07/12/2012   wsh     Releases TRM lock with Req&Notify for when enter suspend
05/25/2012   wsh     Moved ARD logic into it's own state machine
04/02/2012   arm     Supported diversity in reacq. 
10/25/2011   ljl     Handled the failure cases during optimized HO.
02/24/2012   rkc     Fixed bug to allow ARD after traffic suspend.
02/09/2012   grl     Do 4->4c OFS only after 6 seconds after a tune away.
01/11/2012   ljl     Added the parameter for search priority to hdrsrchdrv_enable_srch4().
01/10/2012   ljl     Allowed ARD only in traffic state.
11/29/2011   arm     Removed compiler warnings.
09/25/2010   grl     Added support to inform 1x of HDR traffic state.
08/18/2010   arm     Remove DCVS changes for suspend state. 
08/06/2010   arm     DCVS changes for MDM9k 
06/21/2010   cnx     Fixed compiler warnings.
05/12/2010   ljl     Disabled rx before hdr fw timing state.
03/24/2010   pxu     Removed FEATURE_HDR_QSM features  
03/11/2010   ljl     Supported the fw timing state.
07/22/2009   ljl     Added frame offset.
05/18/2009   ljl     Updated for firmware MSGR interface.
12/10/2008   smd     Replaced assert.h with amssassert.h
11/10/2008   kss     Removed/replaced intlocks.
11/09/2009   lyl     Force demod carrier to be zero during idle suspend state.
10/07/2008   lyl     Fixed lint errors.
06/16/2008   lyl     hdrsrchbc_get_bc_reason() now used for high priority 
                     BCMCS support
01/30/2008   jyw     Included the demod list in the RF interface.
08/18/2007   jyw     Added revB set management support.
08/19/2007   grl     Resolved Lint High warnings.
04/27/2007   grl     Added support for access hybrid ctrl via the tc mode.
04/15/2007   mt      Changes for Enhanced OFS support.
01/17/2007   ljl     Added HDR SRCH AFC stm.
01/02/2006   ljl     Dropped ASET pilots to NSET if TCA has a different 
                     channel.
10/09/2006   ljl     Retry AHO cmd.
09/14/2006   ljl     Retry idle cmd.
08/28/2006   sq      Added support for FEATURE_HDR_QSM
08/01/2006   ljl     Added debug bit for access hybrid
06/14/2006   mt      Fixed BCMCS featurization
06/07/2006   ljl     Added support for hybrid access.
05/12/2006   sq      Disable traffic OFS when BCMCS is enabled
05/04/2006   grl     Don't process the sleep command while idle suspended.
04/06/2006   grl     Requested the RF lock with the BCMCS TRM reason when req.
11/16/2005   jyw     Disallowed 3A to 4A transistion when comes from 3B.
09/10/2005   grl     Added dynamic TC mode support for HPTS mode.
08/11/2005   grl     Mainlined FEATURE_HDR_TCXOMGR.
07/22/2005   ljl     Added nset broadcast and directed update
06/30/2005   sq      Changed OFS API name
05/31/2005   grl     Added support for TRM and TCXOMgr.
05/02/2005   sq      Updated comments 
01/05/2004   sq      Handled TC_ACK_RECEIVED_CMD in TC suspended state
01/04/2005   sq      Mainlined FEATURE_HDR_TC_OFS
11/03/2004   mpa     Removed dependency on std.h (merged with sys.h)
08/09/2004   aaj     Support for BCMCS hybrid 
07/30/2004   sq      Replaced hdrsrchutil_aset_tc_to_idle with new API
07/22/2004   ajn     Added common signals to state machine
06/01/2004   sq      Replaced srch4_enable with new API
04/12/2004   aaj     Fixed idle mode cmd handling in hdrsrchsus state
11/18/2003   aaj     Added support to disable search4 in suspended state
10/29/2003   sq      Disable diversity before leaving TC state
10/13/2003   sq      Turned on FEATURE_HDR_TC_OFS
09/16/2003   sq      Added support for FEATURE_HDR_TC_OFS
09/10/2003   mpa     Converted F3 messages to use MSG2.0 HDR search SSID
04/29/2003   ajn     Prevent exit from state with sam_retain_lock( ) active
03/11/2003   ajn     Updated Copyright notice
02/27/2003   aaj     No need for #ifdefs around include files
02/26/2003   sq      Removed compilation warnings
02/05/2002   sq      Added support for diversity in TC state only
12/11/2002   ajn     Added Idle Suspend state Timeout (60 seconds)
12/10/2002   ajn     Ensure Idle suspend callback used for STILL_SUSPENDED rpt
11/25/2002   ajn     sam_retain_lock( ) now used to hold lock until request.
                     Protocol suspend reports added.
09/05/2002   aaj     Use suspended dump processing in traffic suspended state
06/10/2002   aaj     Kill LOCK_GRANTED_SIG completely before exiting 3a/4a st
04/23/2002   aaj     Return E_AGAIN for access command in suspended idle state
02/26/2002   ajn     Kill lost dump timer in suspended state to handle quick 
                     tune-away in traffic
02/06/2002   ajn     hdrerrno -> errno
01/17/2002   aaj     Support for idle suspend mode
01/14/2002   aaj     Integrated with hybrid mode traffic operation
12/18/2001   aaj     Created this file

===========================================================================*/



/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "hdr_variation.h"
#include "customer.h"

#include "hdrmdsp.h"

#include "hdrsrchstate.h"
#include "hdrsrchset.h"
#include "hdrsrchdef.h"

#include "hdrsrch.h"
#include "hdrsrchi.h"
#include "hdrsrchfing.h"

#include "hdrsrchstates.h"
#include "hdrsrchtask.h"
#include "hdrsrchutil.h"
#include "hdrsrchrf.h"
#include "hdrsrchafc.h"
#include "hdrmdspmcr.h"
#include "hdrfw_msg.h"

#include "timetick.h"

#include "hdrdebug.h"
#include "err.h"
#include "amssassert.h"
#include "srch_hdr.h"

#include "hdrsrchrfdiv.h"

#include "hdrsrchard.h"

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV
#include "hdrlog.h"
#endif
#include "hdrsrcheq.h"

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#include "hdrsrchidlei.h"

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
#include "hdrsrchtclte.h"
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

#include "hdr_log_qsh.h"
#include "hdrutil.h"
/*===========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

===========================================================================*/

/* Suspended for too long timer/signal */

#define HDRSRCHSUS_TIMEOUT_SEC             60

#ifdef FEATURE_HDR_DUAL_SIM

#ifdef FEATURE_HDR_QTA

#ifdef FEATURE_HDR_DR_DSDS
#define HDRSRCHSUS_MIN_LOCK_SCLK           \
          ( ( ( hdrmultirat_is_dsds_enabled()|| hdrmultirat_is_dr_dsds_enabled()) == TRUE ) ? \
          ( ( hdrsrchtc_is_qta_enabled() == TRUE ) ? TIMETICK_SCLK_FROM_MS( 20 ): TIMETICK_SCLK_FROM_MS( 105 )) : TIMETICK_SCLK_FROM_MS( 427 ) )

#else
#define HDRSRCHSUS_MIN_LOCK_SCLK           \
          ( ( hdrmultirat_is_dsds_enabled() == TRUE ) ? \
          ( ( hdrsrchtc_is_qta_enabled() == TRUE ) ? TIMETICK_SCLK_FROM_MS( 20 ): TIMETICK_SCLK_FROM_MS( 105 )) : TIMETICK_SCLK_FROM_MS( 427 ) )
#endif /* FEATURE_HDR_DR_DSDS */

#else
#define HDRSRCHSUS_MIN_LOCK_SCLK           \
          ( ( hdrmultirat_is_dsds_enabled() == TRUE ) ? \
          TIMETICK_SCLK_FROM_MS( 200 ) : TIMETICK_SCLK_FROM_MS( 427 ) )

#endif /* FEATURE_HDR_QTA */

#else

#define HDRSRCHSUS_MIN_LOCK_SCLK           TIMETICK_SCLK_FROM_MS( 427 )
  /* Miniumum time in sclks we must acquire the lock for to exit suspended 
     state */
#endif /* FEATURE_HDR_DUAL_SIM */

#define HDRSRCHSUS_DRC_RAMP_ESTIMATE_MS    ( SLOTS_TO_MS( 80 ) + 10 )
  /* estimate for DRC rampdown duration 80 before tune away */

#define HDRSRCHRF_QTA_TRK_RSP_US             20000 /* 12 Slots */
  /* Max time to wait for QTA_TRK_RSP from FW */

#define HDRSRCHRF_QTA_QUERY_RSP_US           8300 /* 5 Slots */
  /* Max time to wait for QTA_QUERY_RSP */

#define HDRSRCHSUS_SYNC_MIN_LOCK_SCLK        TIMETICK_SCLK_FROM_MS( 40 )
  /* Minimum time taken by Sync state to give-away RF chain */

#ifdef FEATURE_HDR_QTA
/* If not on aset channel, then we need to retune */
#define HDSRCHSUS_TCA_DURING_QTA( carrier_idx )(  \
        ( !hdrsrchrf_get_carrier_idx( hdrsrch.aset_channel[0], &carrier_idx) ) )
#endif /* FEATURE_HDR_QTA */


/* EJECT */
/*===========================================================================

                     STATE DISPATCH TABLE

This section provides the dispatch functions for events that are received
by the task while in this state.

===========================================================================*/

LOCAL void            hdrsrchsus_init( hdrsrch_state_enum_type last_state );
LOCAL void            hdrsrchsus_done( hdrsrch_state_enum_type next_state );
LOCAL void            hdrsrchsus_dump( void );
LOCAL errno_enum_type hdrsrchsus_tc_cmd( const hdrsrch_cmd_type * cmd );
LOCAL errno_enum_type hdrsrchsus_idle_cmd( const hdrsrch_cmd_type * cmd );
LOCAL errno_enum_type hdrsrchsus_sync_cmd( const hdrsrch_cmd_type * cmd );
LOCAL void            hdrsrchsus_lock_granted( void );
LOCAL void            hdrsrchsus_idle_timeout( void );
LOCAL void            hdrsrchsus_tc_done
(
  hdrsrch_state_enum_type next_state
    /* next state after suspended state */
);
LOCAL void            hdrsrchsus_sync_timeout( void );

LOCAL hdrsrch_state_struct_type hdrsrchsus_tc_state =
{
  HDRSRCH_SUSPENDED_TC_STATE,
  hdrsrchsus_init,
  hdrsrchsus_tc_done,
  hdrsrchsus_tc_cmd,
  hdrsrchsus_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
  hdrsrch_default_sched,
  hdrsrch_default_lost_dump,
  hdrsrch_default_timeout,
  hdrsrch_default_gp_timer0,
  hdrsrch_default_gp_timer1,
  hdrsrch_default_gp_sig0,
  hdrsrch_default_gp_sig1,
  hdrsrch_default_time_valid,
  hdrsrch_default_rxchipx16,
  hdrsrchsus_lock_granted,
  hdrsrch_process_int_cmd_default,
  hdrsrch_default_process_low_pri_rsp_ind
};

LOCAL hdrsrch_state_struct_type hdrsrchsus_idle_state =
{
  HDRSRCH_SUSPENDED_IDLE_STATE,
  hdrsrchsus_init,
  hdrsrchsus_done,
  hdrsrchsus_idle_cmd,
  hdrsrchsus_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
  hdrsrch_default_sched,
  hdrsrch_default_lost_dump,
  hdrsrchsus_idle_timeout,
  hdrsrch_default_gp_timer0,
  hdrsrch_default_gp_timer1,
  hdrsrch_default_gp_sig0,
  hdrsrch_default_gp_sig1,
  hdrsrch_default_time_valid,
  hdrsrch_default_rxchipx16,
  hdrsrchsus_lock_granted,
  hdrsrch_process_int_cmd_default,
  hdrsrch_default_process_low_pri_rsp_ind
};

LOCAL hdrsrch_state_struct_type hdrsrchsus_sync_state =
{
  HDRSRCH_SUSPENDED_SYNC_STATE,
  hdrsrchsus_init,
  hdrsrchsus_done,
  hdrsrchsus_sync_cmd,
  hdrsrchsus_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
  hdrsrch_default_sched,
  hdrsrch_default_lost_dump,
  hdrsrchsus_sync_timeout,
  hdrsrch_default_gp_timer0,
  hdrsrch_default_gp_timer1,
  hdrsrch_default_gp_sig0,
  hdrsrch_default_gp_sig1,
  hdrsrch_default_time_valid,
  hdrsrch_default_rxchipx16,
  hdrsrchsus_lock_granted,
  hdrsrch_process_int_cmd_default,
  hdrsrch_default_process_low_pri_rsp_ind
};

/* <EJECT> */
/*===========================================================================

                     HDR SEARCH SUSPENDED STATE DATA

As much SUSPENDED data is lumped together in one structure, so that it
becomes possible to dynamically allocate acquisition data memory during the
SUSPENDED state, and discard the memory when no longer needed.

===========================================================================*/

typedef struct
{
  hdrsrch_state_enum_type                 previous_state;
    /* Previous state of operation for hdr searcher state machine */

#ifdef FEATURE_HDR_QTA
  boolean                                 is_rxagc_trk_msg_rcvd;
    /* Did we receive rxagc trk msg from FW already? */

  boolean                                 is_status_query_msg_rcvd;
    /* Did we receive status query resp msg from FW already? */

  hdrfw_qta_status_rsp_t                  is_empty_gap;
    /* Is the previous gap and empty gap? */
#endif /* FEATURE_HDR_QTA */
}
hdrsrchsus_struct_type;


LOCAL hdrsrchsus_struct_type    hdrsrchsus;

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSUS_LOCK_GRANTED

DESCRIPTION
  Notifies that the lock has been granted to HDR

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchsus_lock_granted( void )
{

  timetick_type                  sclks_now;
    /* current sclks count */

  boolean                        init_ofs_tuneaway = FALSE;
    /* whether to switch to OFS state or not */

#ifdef FEATURE_HDR_QTA
  boolean                        received = FALSE;
    /* Did we receive rx agc ind from FW? */
#endif /* FEATURE_HDR_QTA */

  hdrsrch_carrier_idx_type carrier_idx;
    /* Dummy CSR index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* now that the lock has been granted, either perform OFS or go
     back to HDR traffic state */

#ifdef FEATURE_HDR_SUPPORTS_FW_TIMING_STATE
#ifdef FEATURE_HDR_QTA
  if ( !hdrsrchtc_is_qta_enabled() )
#endif /* FEATURE_HDR_QTA */
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR FW TIMING STATE END. START ACTIVE" );
    hdrmdsp_config_hdr_app( HDRFW_STATE_ACTIVE, TRUE, FALSE );
    /* Config hdr fw to be active */
  }
#endif /* FEATURE_HDR_SUPPORTS_FW_TIMING_STATE */


#ifdef FEATURE_HDR_MODEM_COEXISTENCE_FW
  hdrmultirat_send_dsdx_frequency_cfg_msg( );
    /* During sleep FW loses freq id info, 
       so resend during reacq */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_FW */

  /* This condition is to handle the corner case where the RF grant is received for 
        traffic suspend state when the state has already transistioned to idle suspend state */
  if ( ( hdrsrch_state_id == HDRSRCH_SUSPENDED_IDLE_STATE ) 
    && ( hdrsrchsus.previous_state == HDRSRCH_SUSPENDED_TC_STATE ) )
  {
    /* There is change in the state from the point the hdrsrchrf_request_and_notify() 
            is called and hence the holding priority of the RF chain has to be changed */
      hdrsrchrf_change_priority( HDRSRCHRF_PRI_CHAIN, HDRSRCHRF_OVHD );
  }

#ifdef FEATURE_HDR_QTA
    if ( hdrsrchtc_is_qta_enabled() )
    {
      hdrmultirat_send_qta_status_query_msg();
        /* Ask the FW status of just completed QTA gap */

      if ( !hdrsrchsus.is_status_query_msg_rcvd )
      {
        hdrsrchmsg_set_rsp_waiting( HDRSRCH_QTA_STATUS_QUERY_RSP, TRUE );
          /* Wait for FW response to make sure FW is done with tune */

        received = hdrsrch_wait_for_high_pri_rsp( HDRSRCHRF_QTA_QUERY_RSP_US,
                                                  HDRSRCH_QTA_STATUS_QUERY_RSP );
          /* Wait for query resp indication from FW */

        if ( received == FALSE )
        {
          ERR_FATAL( "HDR QTA: QTA query from FW timeout!! ", 0, 0, 0 );
        }
      }

      if(hdrsrchsus.is_empty_gap == HDRFW_QTA_YIELD_RCVD)
      {
        if ( !hdrsrchsus.is_rxagc_trk_msg_rcvd )
        {
          hdrsrchmsg_set_rsp_waiting( HDRSRCH_RXAGC_TRACK_QTA_RSP, TRUE );
            /* Wait for FW response to make sure FW is done with tune */

          received = hdrsrch_timed_wait_for_high_pri_rsp( HDRSRCHRF_QTA_TRK_RSP_US,
                                                   HDRSRCH_RXAGC_TRACK_QTA_RSP );
            /* Wait for AGC done indication from FW */

          if ( received == FALSE )
          {
            ERR_FATAL( "HDR QTA: RXAGC_TRK_IND from FW timeout!! ", 0, 0, 0 );
          }
        }
      }
      else
      {
        HDR_MSG_SRCH( MSG_LEGACY_ERROR, "HDR QTA empty gap not waiting for FW!!!");
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_EMPTY_QTA_GAP);        
#endif
        /* No need to wait for RX AGC TRK response from FW, hence marking it as
           rcvd for later checks */
        hdrsrchsus.is_rxagc_trk_msg_rcvd = TRUE;
      }

      if ( hdrsrchsus.is_rxagc_trk_msg_rcvd )
      {
        hdrsrchrfdiv_chk_and_redo_warmup( );
        /* Check and re-do DIV warmup if needed */
      }

      hdrmultirat_send_qta_cfg_msg( HDRMULTIRAT_QTA_END );
        /* Let FW know QTA is done */

      if( rfm_hdr_end_qta ( hdrsrchrf_get_rf_device( TRM_HDR ) ) == FALSE ) 
      {
        HDR_MSG_SRCH( MSG_LEGACY_ERROR, "HDR QTA: end QTA to RF Failed!!!");
      }
      
#ifdef FEATURE_HDR_UNIFIED_MICRO_PRIORITY
      hdrmultirat_priority_switch( HDRMULTIRAT_NORMAL_PRIORITY );
#else
      hdrmultirat_priority_switch( HDRMULTIRAT_LOWER_PRIORITY_20 );
#endif /* FEATURE_HDR_UNIFIED_MICRO_PRIORITY */

      /* If no div then let TRM know QTA has ended
         otherwise QTA will end once secondary is granted */
      if ( !hdrsrchrfdiv_is_diversity_in_qta() )
      {
        hdrsrchrf_set_qta_client_state(HDRSRCHRF_QTA_END, NULL);
          /* Let TRM know QTA has ended */
      }
    }
#endif /* FEATURE_HDR_QTA */

  if ( hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE )
  {
    hdrsrchdrv_enable_srch4( SRCH4_PRIO_HDR_TRAFFIC );
      /* enable searcher4 since we got the lock and we would need the
         srch4 resources */

    {
      sclks_now = timetick_get();
        /* Current time in sclks. */
      
      sclks_now -= TIMETICK_SCLK_FROM_MS (HDRSRCHSUS_DRC_RAMP_ESTIMATE_MS );
        /* to keep OFS repeat intercal uniformity */    
    
      if ( !hdrsrchtc_ofs_allowed() )
      {
        HDR_MSG_SRCH(MSG_LEGACY_MED, "OFS disabled due to TAP");
          /* OFS is not allowed (set by TAP), skip all OFS checking */
      }
      else if ( hdrsrchofs_rup_pending() == TRUE )
      {
        init_ofs_tuneaway = TRUE;
          /* we will initiate OFS tuneaway to begin new RUP search */
      }
      else if ( hdrsrchofs_repetition_required() == TRUE )
      {
        init_ofs_tuneaway = TRUE;
          /* we will initiate OFS tuneaway to repeat last search */
      }
      /* Has 6 seconds passes since the last OFS? */
      else if ( hdrsrchutil_tc_ofs_allowed() )
      {
        hdrsrchtc_update_tc_ofs_check_time( sclks_now );
          /* update the last OFS check time, need to check 
             ofs condition again in HDRSRCHTC_OFS_CHECK_INTERVAL_MS */
             
        hdrsrchtc_update_tc_ofs_group_time( sclks_now );
          /* update the last OFS search group start time */

        /* Is there OFS pilots to search? */
        if ( hdrsrchofs_tc_ofs_reqd() == TRUE )
        {
          init_ofs_tuneaway = TRUE;
            /* we will switch to OFS state to begin new OFS search  */      
        }
      }
      else
      {
        /* Make sure we only do 4 -> 4c OFS at least 6 seconds from
           a 4a tune away. In hybrid mode we should never do 4 -> 4c
           transitions. */
        hdrsrchtc_update_tc_ofs_check_time( sclks_now );
          /* update the last OFS check time, need to check 
             ofs condition again in HDRSRCHTC_OFS_CHECK_INTERVAL_MS */
          
          hdrsrchtc_update_tc_ofs_group_time( sclks_now );
            /* update the last OFS search group start time */
        }
      }
    
#ifdef FEATURE_HDR_QTA
      if ( hdrsrchtc_is_qta_enabled() )
      {
        /* If we are starting OFS or if we received TCA with channel change during QTA
           then shutdown TX */
        if ( ( init_ofs_tuneaway == TRUE )|| ( HDSRCHSUS_TCA_DURING_QTA(carrier_idx) ) )
        {
          HDR_MSG_SRCH( MSG_LEGACY_ERROR, "HDR QTA: Disable TX due to OFS/TCA");
          hdrsrchtc_disable_tx_qta();
          hdrsrchafc_stop_fll_tracking();
            /* Disable FLL tracking and save the FLL accum value. */
        }        
      }
#endif /* FEATURE_HDR_QTA */

    if ( init_ofs_tuneaway == TRUE )
    {
      hdrsrchutil_tc_ofs_start_gov();
        /* Reset the OFS rate governer timer */
      
      hdrsrchtc_update_tc_ofs_time( sclks_now );
        /* update the last OFS start time */
        
      hdrsrchofs_init_ofs_state ( );
        /* Enter OFS since OFS is needed */
    }
    else
    {
      hdrsrchtc_resume_connected_state();
        /* go back to traffic/connected state */
    }
  }
  else if ( (hdrsrch_state_id == HDRSRCH_SUSPENDED_SYNC_STATE) && 
                                        (hdrsrch_is_lre_enabled()) )
  {
    hdrsrchdrv_enable_srch4( SRCH4_PRIO_HDR_ACQ );
      /* Enable SRCH4 for HDR ACQ (same as SYNC) */

    hdrsrchsync_init_sync_state();
      /* Go back to SYNC state */
  }
  else
  {
    hdrsrchdrv_enable_srch4( SRCH4_PRIO_HDR_REACQ_PAGING );
      /* enable searcher4 since we got the lock and we would need the
         srch4 resources */

#ifdef FEATURE_HDR_BCMCS_HYBRID
    if ( hdrsrchsus.previous_state == HDRSRCH_BC_IDLE_STATE &&
         hdrsrchbc_get_bc_status() == BCMCS_ENABLED            )
    {
      hdrsrchutil_suspend_rpt( HDRSRCH_SUSPEND_ENDED );
        /* RF Resources have been regained */

      hdrsrchbc_init_bc_state( );
        /* we are suspended in idle state entering from BC and BC has not
           been disabled while in the suspended state */
    }
    else 
#endif /* FEATURE_HDR_BCMCS_HYBRID */

#ifdef FEATURE_HDR_HYBRID_ACCESS
    if ( hdrsrchsus.previous_state == HDRSRCH_IDLE_STATE )
    {
      hdrsrchutil_suspend_rpt( HDRSRCH_SUSPEND_ENDED );
        /* RF Resources have been regained */

      /* We were in access */
      hdrsrchmon_init_idle_state();
    }
    else
#endif /* FEATURE_HDR_HYBRID_ACCESS */
    {
      hdrsrchutil_suspend_rpt( HDRSRCH_SUSPEND_TO_REACQ );
        /* Transition from 3A to 3F state */

    hdrsrchreacq_init_reacq_state();
      /* we are suspended in idle state */
    }
  }/* else */

} /* hdrsrchsus_lock_granted */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHSUS_LOCK_RF

DESCRIPTION
  Ask for the best RF chain so that we can do either traffic, if we are in
  traffic suspended state, or paging, if we are in an idle suspend state. 
  We might end up doing an OFS before traffic, but we won't tell TRM about
  that since that is only a minor side activity.  Suspending us because we
  are doing OFS wouldn't make sense since sometimes we must go through OFS
  to get to traffic.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  hdrsrchsus_lock_notify( ) will be called when RF lock is granted.

==========================================================================*/

LOCAL void hdrsrchsus_lock_rf( void )
{
  hdrsrchrf_reason_enum_t rf_lock_reason;
    /* The reason we are asking for the RF chain0.  This is either for paging
       or traffic.  We don't ask for OFS at this point (see the 
       description). */

  hdrsrchrf_resource_enum_t trm_resource = HDRSRCHRF_RX_ANY;
    /* What is the resource we are requesting? */

  uint32 min_sclks = HDRSRCHSUS_MIN_LOCK_SCLK;
    /* Initialize with min sclks */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrch_add_to_wait_mask( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* Watch for Lock Granted signal */

  if ( hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE )
  {
    rf_lock_reason = hdrsrchtc_get_tc_reason();
      /* Since we are in traffic suspended state, we need the RF to do 
         some type of traffic. Find out what that is. */
  }
  else if ( hdrsrch_state_id == HDRSRCH_SUSPENDED_SYNC_STATE )
  {
    min_sclks = HDRSRCHSUS_SYNC_MIN_LOCK_SCLK;

    rf_lock_reason = hdrsrchsync_get_trm_priority();
      /* Current TRM reason used in Sync state */
  }
  else if ( (hdrsrch_state_id == HDRSRCH_SUSPENDED_IDLE_STATE ) && 
              ( hdrsrchsus.previous_state == HDRSRCH_SUSPENDED_TC_STATE ))
  {
      /* We were in idle monitor state before tune away */
      rf_lock_reason = HDRSRCHRF_OVHD;
	  
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                      "4a->3a : Request RF lock with reason %d", 
                      rf_lock_reason );  
  }
  else
  {
#ifdef FEATURE_HDR_HYBRID_ACCESS
    if ( hdrsrchsus.previous_state == HDRSRCH_IDLE_STATE )
    {
#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
      if ( hdrsrchidle_is_in_access( ) ||
           hdrsrchidle_is_tx_pending( ) )
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */
      {
#ifdef FEATURE_HDR_DUAL_SIM
        /* We were in access before tune away and coming to suspend state */
        if ( !hdrmultirat_is_dsds_enabled() )
        {
#endif /* FEATURE_HDR_DUAL_SIM */
          if (  ( HDRSRCH_ACCESS_HYBRID_NV_ENABLE ) &&
                ( hdrsrch.tc_mode != HDRSRCH_S1XHDR_HP_TC_MODE ) )
          {
            rf_lock_reason = HDRSRCHRF_ACCESS;
          }
          else
          {
            rf_lock_reason = HDRSRCHRF_ACCESS_URGENT;
          }
#ifdef FEATURE_HDR_DUAL_SIM
        }
        else if ( ( hdrmultirat_is_dsds_enabled() )&&
                  ( HDRSRCH_ACCESS_HYBRID_NV_ENABLE ) && 
                 ( hdrsrch.tc_mode != HDRSRCH_S1XHDR_HP_TC_MODE ) )
        {
          rf_lock_reason = HDRSRCHRF_ACCESS;
        }
        else
        {
          rf_lock_reason = HDRSRCHRF_ACCESS_URGENT;
        }

#endif /* FEATURE_HDR_DUAL_SIM */
      } 
#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
      else
      {
        /* We were in idle monitor state before tune away */
          rf_lock_reason = HDRSRCHRF_OVHD;

          HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
			       "NO access/tx here. Request RF lock with reason %d", 
			        rf_lock_reason );
		  
      }
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */
    }
    else
#endif /* FEATURE_HDR_HYBRID_ACCESS */  
    {
#ifdef FEATURE_HDR_BCMCS
      rf_lock_reason = ( ( hdrsrchbc_get_bc_status( ) == BCMCS_ENABLED ) ? 
                     hdrsrchbc_get_bc_reason() : HDRSRCHRF_DEMOD_PAGE );
      /* Since we are in idle suspended state, we need the RF to do 
         paging or broadcast. */
#else
      rf_lock_reason = HDRSRCHRF_DEMOD_PAGE;
#endif /* FEATURE_HDR_BCMCS */
    }
  }

  if ( hdrsrchidle_is_tx_pending( ) ||
       hdrsrchidle_is_in_access( ) ||
       ( hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE ) )
  {
     trm_resource = HDRSRCHRF_RXTX_BEST;
       /* Request rx any for idle demod in DR mode */
  }

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
                  "Request and notify for the best transceiver "
                  "for duration %d resource %d reason %d", 
                  min_sclks, trm_resource, rf_lock_reason );

  hdrsrchrf_request_and_notify( HDRSRCHRF_PRI_CHAIN, 
                                trm_resource, 
                                min_sclks,
                                rf_lock_reason,
                                hdrsrch_rf_grant_cb );
    /* Notify this module when RF resources have been locked */

#ifdef FEATURE_HDR_QTA
  /* Request div again after giving up primary to make sure div is granted asap at gap end */
  if ( hdrsrchrfdiv_is_diversity_in_qta() )
  {
    hdrsrchrfdiv_diversity_ctrl( TRUE );
     /* Request for diversity after QTA */
  }
#endif /* FEATURE_HDR_QTA */

} /* hdrsrchsus_lock_rf( ) */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHSUS_YIELD_RF

DESCRIPTION
  Free the RF resources, and reattempt to acquire it.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchsus_yield_rf( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrchrf_has_lock( HDRSRCHRF_PRI_CHAIN ) == TRUE )
  {
    if (
#ifdef FEATURE_HDR_QTA
         ( !hdrsrchtc_is_qta_enabled()) &&
           /* If feature QTA is defined, check if QTA is currently enabled */
#endif /* FEATURE_HDR_QTA */
#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
         ( !hdrsrchtc_is_measuring_lte() ) &&
           /* If feature LTE_REDIRECTION is defined, check if DO is in the process of measuring LTE */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
         TRUE
           /* Set to TRUE as default.  So otherwise always disable RX */
       )
    {
      hdrsrchrf_disable_rx( HDRSRCHRF_PRI_CHAIN );
        /* Disable rx only if it is no longer required */
    }
  }

#ifdef FEATURE_HDR_SUPPORTS_FW_TIMING_STATE
  if ( 
#ifdef FEATURE_HDR_QTA
       ( !hdrsrchtc_is_qta_enabled() ) &&
#endif /* FEATURE_HDR_QTA */
#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
       ( !hdrsrchtc_is_measuring_lte() ) &&
           /* If DO is in the process of measuring LTE, delay TIMING state until after RF has been configured */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
       TRUE
     )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR FW TIMING STATE START" );

    hdrmdsp_config_hdr_app( HDRFW_STATE_TIMING, TRUE, FALSE );
      /* Config HDR fw to timing state */
  }
#endif /* FEATURE_HDR_SUPPORTS_FW_TIMING_STATE */

  REX_DISABLE_PREMPTION();

  if ( hdrsrchrf_has_lock( HDRSRCHRF_PRI_CHAIN ) == TRUE )
  {
    /* We currently have the lock. HDR enters suspend due to
       unlock callback, so Req&Notify will automatically
       release the lock. Just reset the chain state instead */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
    if ( !hdrsrchtc_is_measuring_lte() )
           /* If feature LTE_REDIRECTION is defined, check if DO is in the process of measuring LTE */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
    {
#ifdef FEATURE_HDR_QTA
      if ( !hdrsrchtc_is_qta_enabled() )
           /* If feature QTA is defined, check if QTA is currently enabled */
#endif /* FEATURE_HDR_QTA */
    {
      hdrsrchrf_reset_rf_chain_state( HDRSRCHRF_PRI_CHAIN );
        /* Reset RF chain state only if it is no longer required */
    }

    HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DLR: hdrsrchsus_lock_rf" );

    hdrsrchsus_lock_rf( );
      /* ... and asked for it back, when time again permits. */

      /* With SAM we used to look to see if another client was trying to
         get the lock.  If not, we would retain the lock.  Then only
         after we were kicked off the lock we would request the lock again
         so that we can transition out of the suspended state.  This 
         shouldn't be needed with TRM because of TRM's more precise timing.
         We'll just request the lock for traffic or paging and TRM will not
         give us the lock until the reservation made by 1x paging is satisfied.
      */
    } /* if ( !hdrsrchtc_is_measuring_lte() ) */
  }
  else
  {
      /* We do not currently have the lock ... */
    hdrsrchsus_lock_rf( );
      /* Asked for it back ... when time again permits. */
  }

  REX_ENABLE_PREMPTION();

} /* hdrsrchsus_yield_rf */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSUS_INIT

DESCRIPTION
 Suspended State Initialization

DEPENDENCIES

PARAMETERS
  last_state  - Last HDR Searcher State

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchsus_init( hdrsrch_state_enum_type last_state )
{
  hdrsrch_sect_struct_type   *sect;
    /* active set sector */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsus.previous_state = last_state;
    /* Remember the last state */

  hdrsrchdrv_disable_srch4( );
    /* Vote to disable searcher4 since hdr does not need it anymore during
       suspended state. When gps application is activated, srch4 application
       needs to be deactivated. If everyone has not voted to deactivate 
       srch4 application then srch4 drivers can error fatal */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
  HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DLR: entered susp state" );
  if ( ( last_state == HDRSRCH_CONNECTED_STATE ) &&
       ( hdrsrchtc_ready_to_measure_lte () ) )
  {
    /* HDR enters susp state from traffic state and is ready
       to make LTE measurements. It is done with DRC rampdown and TX has been disabled.
       Now request LTE measurement. */

    HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DLR: start measurement" );
    hdrsrchtc_start_measure_lte();
    hdrsrchtclte_init_tclte_state();
      /* Transition to LTE measurement state from Traffic Suspend state.
         hdrsrchstate.next_state_id will change to HDRSRCH_LTE_MEAS_TC_STATE */
  }
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

  if ( last_state == HDRSRCH_SUSPENDED_TC_STATE )
  {
    hdrsrchutil_suspend_rpt( HDRSRCH_SUSPEND_STILL_SUSPENDED );
      /* Report that we're going from TC Suspended to IDLE Suspended */

    /* Always keep demod carrier to be 0 in idle suspend to be consistent 
       with regular idle state */
    if( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx != 
                                                       HDRSRCH_DEMOD_IDX_0 )
    {
      sect = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0];
        /* convenience pointer */

      hdrsrchutil_aset_idle_handoff( sect );
        /* Force to change the demod carrier 0 */

      hdrsrchrf_force_demod_carrier_zero();
        /* Force to change channel in hdrsrchrf to demod 0 but the real tune will 
           be done in other state, ie. reacq and etc. */

      hdrsrchsleep_set_scc_info( sect->scc_offset, sect->scc_rate );
        /* Mark the SCC offset as invalid since we are performing the IHO and
           we don't know what the SCC offset would be for new sector. This will
           be set by the MAC layer after confirming the new SCC offset */

      hdrsrchsleep_send_msg_update_scc_offset( );
        /* Send the message to update SCC offset */
    }
  }

if ( hdrsrch_state_id == HDRSRCH_SUSPENDED_IDLE_STATE )
  {
  
#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV
   
   if( hdrsrch.idle_state_div.idle_div_enabled )
   {
     HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: turning off div in Idle/Access mode--> Idle Suspend");

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
     /* Deactivate decode m slot indication */
     hdrsrchsleep_deactivate_scc_decode_ind();

     hdrlog_register_slot_logging_cb ( HDRLOG_SRCHDIV_MDSP_SLOT_LOGGING_CLIENT,
                                       NULL );
      /* Deregister for callback() for mdsp logging */   
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */
   }

   hdrsrchrfdiv_diversity_ctrl( FALSE );
#endif
}

#ifdef FEATURE_HDR_DUAL_SIM
  /* if DSDS enabled and not switching between SUSP_IDLE & SUSP_TC */
  if ((last_state != HDRSRCH_SUSPENDED_IDLE_STATE) &&
      (last_state != HDRSRCH_SUSPENDED_TC_STATE))
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_ERROR,
                    "DSDS tuneaway enabled %d state %d ",
                    hdrmultirat_is_dsds_enabled(),
                    last_state );

#ifdef FEATURE_HDR_QTA
    if ( hdrsrchtc_is_qta_enabled() )
    {
      hdrsrchsus.is_rxagc_trk_msg_rcvd = FALSE;
       /* Reset ack msg flag */

      hdrsrchsus.is_status_query_msg_rcvd = FALSE;
       /* Reset query msg flag */
    }
#endif /* FEATURE_HDR_QTA */

  }
#endif /* FEATURE_HDR_DUAL_SIM */

  /* This condition is added to handle the TRM request_and_notify calls in case 
        where there is a transition from traffic suspend state to idle suspens state */
  if(( last_state == HDRSRCH_SUSPENDED_TC_STATE ) 
    && ( hdrsrch_state_id == HDRSRCH_SUSPENDED_IDLE_STATE ))
  {
    /* Allow calling another hdrsrchrf_request_and_notify() */
    hdrsrchrf_reenable_request_notify();
  }

  /* If IDLE-SUSPEND state is entered from TC-SUSPEND state and the rf lock was already granted
    then skip yielding RF.  */
  if( !( (last_state == HDRSRCH_SUSPENDED_TC_STATE) && 
         (hdrsrch_state_id == HDRSRCH_SUSPENDED_IDLE_STATE) &&
         (hdrsrchrf_has_lock( HDRSRCHRF_PRI_CHAIN ) == TRUE) ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DLR: hdrsrchsus_yield_rf" );
    hdrsrchsus_yield_rf();
      /* after entering suspended state, yield RF. hdrsrchsus_lock_notify
         will be called when RF lock is granted to HDR */
  }
  if ( hdrsrch_state_id == HDRSRCH_SUSPENDED_IDLE_STATE )
  {
    (void) rex_set_timer( &hdrsrch.timeout_timer, HDRSRCHSUS_TIMEOUT_SEC * 1000L );
      /* Timeout if in Idle Suspended state for too long */

    hdrsrch_add_to_wait_mask( HDRSRCH_TIMEOUT_SIG );
      /* Watch for the timeout */
  }
  else if ( hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE )
  {
    if ( hdrsrcheq_is_grice_enabled() ) 
    {
      hdrsrchset_grice_activate_asp_snr_order_updates( TRUE );
        /* Activate ASP ranking to catch corner case when DO receives TCA while
           suspended.  In this case, DO goes to traffic suspend state but still
           commits the ASPs to FW.  At the time DO goes to traffic state the
           ranking must be valid prior to enabling the gRICE equalizer. */
    }
  }

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW 

#ifdef FEATURE_HDR_QTA
    if ( !hdrsrchtc_is_qta_enabled())
#endif /* FEATURE_HDR_QTA */
    {
  /* Sending Freq. update to MCS that we do not own the chain */
  hdrmultirat_process_freq_change(RFM_INVALID_DEVICE, 
                                  NULL, 
                                  HDRMULTIRAT_FORWARD_LINK, 
                                  0);
    }
  
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

} /* hdrsrchsus_init */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSUS_IDLE_MODE_CMD

DESCRIPTION
  This function commands the searcher to transition to IDLE

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL errno_enum_type hdrsrchsus_idle_mode_cmd
( 
  const hdrsrch_idle_params_type          *params
    /* Idle Mode parameters */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "IDLE_MODE_CMD" );

  hdrsrchutil_aset_enter_idle( );
    /* Deallocate n-1 ASET pilots, set DRC covers to NULL, etc. */

  /* The following order is important:
    1. Empty directed list 
    2. Reset nset search parameters to default
    3. Update with broadcast */
    
  hdrsrchset_init_nset_update_list( HDRSRCH_NSET_UPDATE_DIRECTED );
    /* Empty the directed nset update list */

  hdrsrchset_reset_nset_search_window_params();
    /* Set search window parameters to default */

  hdrsrchset_update_nset_search_window_params( HDRSRCH_NSET_UPDATE_BROADCAST );
    /* Update with broadcast parameters */

  srch_hdr_set_hdr_in_traffic( FALSE );
    /* Tell 1x search that HDR is no longer in traffic */

#ifdef FEATURE_HDR_REVB
  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "TC Suspended to Idle with pn=%d", 
                  hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->pn_offset );
#else
  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "TC Suspended to Idle with pn=%d", 
                  hdrsrchset.aset[0]->pn_offset );
#endif /* FEATURE_HDR_REVB */

  hdrsrch.suspend_cb = params->suspend_cb;
    /* Record Idle mode suspend callback, for hdrsrchsus_init( ) */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
  hdrsrchtclte_release_trm_grant();
    /* Cancel outstnding TRM grant for D2L redirection, if any. */

  ( void ) hdrsrchtclte_send_deinit_req();
    /* If DO has sent an Init request to LTE to perform LTE pilot measurements,
       make sure DO also sends a Deinit request.  Otherwise, LTE will be left
       in an unexpected state when it is activated. */

#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

  hdrsrch_change_state( &hdrsrchsus_idle_state );

  return E_AGAIN;

} /* hdrsrchsus_idle_mode_cmd( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSUS_TCA_CMD

DESCRIPTION
  This function handles a traffic channel assignment msg in traffic sus mode.

DEPENDENCIES
  None

PARAMETERS
  params  - Traffic Channel Assignment parameters

RETURN VALUE
  FALSE   - Command processing is to be suspended until TCA is complete.

SIDE EFFECTS
  Waits for any search in progress to complete,
  Tunes to the new channel,
  Sets the new Active Pilot Set.
  Starts a search on the new channel,
  Assigns fingers to ASET multipath,
  Calls "traffic channel assignment complete" callback.
  Command processing is halted until TCA is complete.

===========================================================================*/

LOCAL errno_enum_type hdrsrchsus_tca_cmd
(
  const hdrsrch_tc_assignment_params_type *params
    /* Traffic Channel Assignment parameters */
)
{
  int                        i;
    /* temporary loop variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_HDR_REVB
  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "TC_ASSIGN_CMD Num=%d", params->num_subaset );
#else
  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "TC_ASSIGN_CMD Num=%d", params->num_pilots );
#endif /* FEATURE_HDR_REVB */

  /*---------------------------------------*
   * Log active set change related packets *
   *---------------------------------------*/

  hdrsrchtc_log_rab_info_record( params );
    /* log rev activity bit info */

#ifndef FEATURE_HDR_REVB
  if ( !SYS_CHAN_CMP_EQUAL( hdrsrch.aset_channel, params->channel) )
  {
    /* We are changing the active set channel - drop current ASET pilots */

    for ( i=hdrsrchset.aset_cnt-1; i >= 0; i-- )
    {
      hdrsrchset_drop_from_aset_to_nset( hdrsrchset.aset[i] );
        /* Drop this pilot from the ASET to NSET to avoid stale OFS energies
           from being reported to the AN during RUPs*/
    }

    hdrsrchfing_update_all_sectors( );
      /* Ensure fingers get deassigned after dropping ASET pilots */
  }

  hdrsrch.aset_channel = params->channel;
    /* Set the new ASET channel */

  (void) hdrsrchutil_update_aset_pilots( params->aset_pilots, params->num_pilots );
    /* Set the active set pilots */
#else
  hdrsrch.aset_channel_cnt = params->num_subaset;
    /* Update the number of the aset channels */

  for( i=0; i<params->num_subaset; i++)
  {
    hdrsrch.aset_channel[i] = params->subaset_list[i].channel;
    hdrsrch.aset_demod[i]   = params->subaset_list[i].demod_idx;
      /* Set the new ASET channel */
  }

  (void) hdrsrchutil_update_aset_pilots_revb( params->subaset_list,
                                       params->num_subaset,
                                       params->cc_subactive_set_idx );
    /* Set the active set pilots */

  /* Finger should not be assigned in the suspended state */
  hdrsrchfing_update_all_sectors( );

  hdrsrch.frame_offset = params->frame_offset;
    /* Save frame offset */
#endif /* FEATURE_HDR_REVB */

  params->tc_assign_cb( HDRSRCH_TCA_SUCCEEDED );
    /* call the TC assign callback to indicate that the command is done */

  return E_SUCCESS;

} /* hdrsrchsus_tca_cmd */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSUS_TC_CMD

DESCRIPTION
  This function handles commands received in suspended traffic state

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

LOCAL errno_enum_type hdrsrchsus_tc_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type               status = E_SUCCESS;
     /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch ( cmd->name )
  {
#ifdef FEATURE_HDR_QTA
    case HDRSRCH_DEACTIVATE_CMD:
      if( hdrsrchtc_is_qta_enabled() )
      {
        status = E_AGAIN;
          /* If HDRSRCH_DEACTIVATE_CMD is received inside QTA 
          then wait for transistion to traffic state */
      }
      else
      {
        /* Exiting from traffic suspended state to inactive state and hence 
        update APS state information */
        hdrsrchrf_set_aps_client_state( HDRSRCHRF_TRM_END_STATE );
          /* Inform TRM for APS state end */   

        status = hdrsrchtc_default_traffic_cmd( cmd );
          /* Handle default for deactivate cmd */
      }
      break;
#endif /* FEATURE_HDR_QTA */

    case HDRSRCH_IDLE_CMD:
#ifdef FEATURE_HDR_QTA    
      if( hdrsrchtc_is_qta_enabled() )
      {
        status = E_AGAIN;
          /* If IDLE_CMD is received in inside QTA then wait for transistion to traffic state */
      }
      else
#endif /* FEATURE_HDR_QTA */
      {
        /* Exiting from traffic suspended state to idle state and hence 
        update APS state information */
#ifdef FEATURE_HDR_QTA
        hdrsrchrf_set_aps_client_state( HDRSRCHRF_TRM_END_STATE );
          /* Inform TRM for APS state end */   
#endif /* FEATURE_HDR_QTA */

        status = hdrsrchsus_idle_mode_cmd( &cmd->params.idle );
      }
      break;

    case HDRSRCH_TC_ASSIGNMENT_CMD:

      status = hdrsrchsus_tca_cmd( &cmd->params.tc_assign );
      break;

    case HDRSRCH_TC_ACK_RECEIVED_CMD:

      /* We will re-try this command when we go back to traffic state, if
         we don't spend too much time suspended. If somehow we can't return
         to traffic state, this command will be abandoned                */
      status = E_AGAIN;
      break;

    /* BCMCS command is handled in the default TC command handling */

    default:
      status = hdrsrchtc_default_traffic_cmd( cmd );
        /* Handle default traffic mode commands */

  } /* switch (cmd->name) */

  return status;

} /* hdrsrchsus_tc_cmd(cmd) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSUS_IDLE_CMD

DESCRIPTION
  This function handles commands received in suspended traffic state

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

LOCAL errno_enum_type hdrsrchsus_idle_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type               status;
     /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( ( cmd->name != HDRSRCH_DEACTIVATE_CMD ) &&
       ( cmd->name != HDRSRCH_SYSTEM_LOST_CMD ) &&
       hdrsrch_sys_loss_is_pending( ) )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                    "SysLoss pending, fail command %d",
                    cmd->name );

    status = E_FAILURE;
  }
  else switch ( cmd->name )
  {
#ifdef FEATURE_HDR_HYBRID_ACCESS
    case HDRSRCH_ACCESS_HANDOFF_CMD:
    
      status = E_AGAIN;
        /* Retry AHO command */

      break;
#endif /* FEATURE_HDR_HYBRID_ACCESS */
    
    case HDRSRCH_TC_ASSIGNMENT_CMD:
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Received TCA in idle suspend state");

      status = E_AGAIN;
        /* retry this command when we go to either the traffic suspended state 
         * or the broadcast idle state */
      break;

    case HDRSRCH_ACCESS_CMD:
      status = E_AGAIN;
        /* retry this command later after we exit this stage. TBD: What 
           happens if we go to traffic state from here ? This command
           will be dropped with "not supported" status. Not bad */
      break;

    case HDRSRCH_IDLE_CMD:

      /* Got idle mode command in idle suspended state. We don't have RF so 
         can not switch to IDLE monitor yet. Wait for RF lock and mark the 
         command as complete since we successfully switched to idle suspend */

      status = E_AGAIN;
      break;
      
    #ifdef FEATURE_HDR_BCMCS_HYBRID
    case HDRSRCH_UPDATE_BC_INFO_CMD:

      hdrsrchbc_set_bc_status( cmd->params.bc_info.bcmcs_status );

      status = E_SUCCESS;

      break;
    #endif /* FEATURE_HDR_BCMCS_HYBRID */

    case HDRSRCH_SLEEP_CMD:

      status = E_AGAIN;
        /* Since we are suspended, by definition another technology is using
           the RF chain so making a jump to sleep doesn't make a big difference
           in terms of power. So we will try to go to sleep once we acquire
           the RF and go to idle. */

      break;

#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
    case HDRSRCH_CHANNEL_CHANGE_CMD:
    case HDRSRCH_NSET_UPDATE_CMD:
    case HDRSRCH_CHANNEL_CHANGE_AND_NSET_UPDATE_CMD:
    case HDRSRCH_REENABLE_HANDOFFS_CMD:
    case HDRSRCH_RUP_REQ_CMD:
      status = E_AGAIN;
      break;
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */

    default:
      status = hdrsrchidle_default_idle_cmd( cmd );
        /* Handle state-independent commands */

  } /* switch (cmd->name) */

  return status;

} /* hdrsrchsus_idle_cmd(cmd) */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSUS_SYNC_CMD

DESCRIPTION
  This function handles commands during HDR SUSPENDED SYNC state.

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

LOCAL errno_enum_type hdrsrchsus_sync_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type               status;
    /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "Received cmd %d", cmd->name );

  switch ( cmd->name )
  {
    case HDRSRCH_SYNCHRONIZE_CMD:

      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Received Synchronize Cmd PN=%d in SUSPENDED SYNC state",
                      cmd->params.synchronize.pilot_pn );

      status = E_FAILURE;
        /* Cannot process the sync cmd in suspend state, but will be processed in sync state */
        /* CP will initiate new sync cmd if the hdrsrchsync.sync_params.return_timing_cb()
           is not called */

      HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                    "Sync cmd is received in the sync suspend state");
      break;

    case HDRSRCH_IDLE_CMD:

      HDR_MSG_SRCH( MSG_LEGACY_MED,"Waiting for timing change to enter Idle in SUSPENDED SYNC state" );

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
      HDR_MSG_SRCH( MSG_LEGACY_HIGH,"Deactivate from sync suspend state" );

      hdrsrchsync_reset_ta_count();
        /* Reset the sync tune away count when HDR deactivates */

    default:

      status = hdrsrch_default_cmd( cmd );
        /* Handle state-independent commands */

  } /* switch (cmd->name) */

  return status;

} /* hdrsrchsus_sync_cmd(cmd) */


/* EJECT */
/*===========================================================================

FUNCTION      HDRSRCH_INIT_SUSPENDED_STATE

DESCRIPTION

DEPENDENCIES
  This function needs to be called when searcher is idle.

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Causes state transition.

===========================================================================*/

void hdrsrchsus_init_suspended_state( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE )
  {
    hdrsrch_change_state( &hdrsrchsus_tc_state );
      /* Switch to suspended state processing */
  }
  else if ( 
#ifdef FEATURE_HDR_BCMCS_HYBRID
            hdrsrch_state_id == HDRSRCH_BC_IDLE_STATE    ||
#endif  /* FEATURE_HDR_BCMCS_HYBRID */
            hdrsrch_state_id == HDRSRCH_IDLE_STATE        )
  {
    hdrsrch_change_state( &hdrsrchsus_idle_state );
      /* Switch to suspended state processing */
  }
  else if ( (hdrsrch_state_id == HDRSRCH_SYNC_STATE) && 
                                        (hdrsrch_is_lre_enabled()) )
  {
    hdrsrch_change_state( &hdrsrchsus_sync_state );
      /* Switch to suspended state processing */
  }
  else
  {
    ERR("Incorrect transition to suspended from %x", hdrsrch_state_id, 0, 0);
  }

} /* hdrsrchsus_init_suspended_state ( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION      HDRSRCHSUS_DONE

DESCRIPTION
  Exit processing for search suspended state.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Causes state transition.

===========================================================================*/

void hdrsrchsus_done
(
  hdrsrch_state_enum_type next_state
    /* next state after suspended state */
)
{
#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_event_payload_type payload;
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_kill_signal( HDRSRCH_PRI_CHAIN_EVENT_SIG );

#ifdef FEATURE_HDR_DUAL_SIM
  /* if DSDS enabled and not switching between SUSP_IDLE & SUSP_TC */
  if ((next_state != HDRSRCH_SUSPENDED_IDLE_STATE) &&
      (next_state != HDRSRCH_SUSPENDED_TC_STATE))
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_ERROR,
                    "DSDS tuneaway done %d state %d ",
                    hdrmultirat_is_dsds_enabled(),
                    next_state );
  }
#endif /* FEATURE_HDR_DUAL_SIM */

  /* For SYNC and SUSPENDED SYNC states, leave timeout timer running. */
  if ( (next_state != HDRSRCH_SYNC_STATE) || (!hdrsrch_is_lre_enabled()) )
  {
    hdrsrch_kill_timer( &hdrsrch.timeout_timer );
  }

#ifdef FEATURE_HDR_SELECTIVE_TXD
  if ( ( next_state == HDRSRCH_IDLE_STATE ) ||
       ( next_state == HDRSRCH_OFS_IDLE_STATE ) ||
       ( next_state == HDRSRCH_SYNC_STATE ) )
  {
    hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_RESUMED );
  }
  else
  {    
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "TxD: exiting access %d", 
                    next_state );
    payload.next_state = next_state;
    hdrsrchrftxd_event_with_payload( HDRSRCHRFTXD_EVENT_EXITING_ACCESS, 
                                     &payload );
  }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  if( ( next_state == HDRSRCH_OFS_TC_STATE ) || 
    ( next_state == HDRSRCH_SUSPENDED_IDLE_STATE ) )
  {
#ifdef FEATURE_HDR_DUAL_SIM

#ifdef FEATURE_HDR_QTA
      if ( hdrmultirat_is_dsds_enabled() 
#ifdef FEATURE_HDR_DR_DSDS
         || hdrmultirat_is_dr_dsds_enabled()
#endif /* FEATURE_HDR_DR_DSDS */
         )
      {
        if( hdrsrchtc_is_qta_nv_enabled() == TRUE )
        {
          trm_set_extension_flag( TRM_HDR, TRM_BG_TRAFFIC, TRM_QUICK_TA_SUPPORTED, FALSE );
            /* BG traffic end allow G to sleep */
        }
      }
#endif /* FEATURE_HDR_QTA */
    if ( hdrmultirat_is_dsds_enabled() )
    {
      trm_set_extension_flag( TRM_HDR, TRM_BG_TRAFFIC, TRM_PRIORITY_INV_ENABLED, FALSE );

      HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DSDS: Disabled BG traffic" );
    }
#endif /* FEATURE_HDR_DUAL_SIM */
  }

} /* hdrsrchsus_done( ) */


/*===========================================================================

FUNCTION      HDRSRCHSUS_TC_DONE

DESCRIPTION
  Exit processing for search suspended state.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Causes state transition.

===========================================================================*/

void hdrsrchsus_tc_done
(
  hdrsrch_state_enum_type next_state
    /* next state after suspended state */
)
{
#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_event_payload_type payload;
#endif /* FEATURE_HDR_SELECTIVE_TXD */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* This is to take care of following race condition,
     - HDR call ends and Connection is closed in Traffic(state 4)
     - Processing IDLE_MODE_CMD delayed so HDR moves to Traffic suspend (4a)
     - IDLE_MODE_CMD is processed when HDR is in Traffic suspend (4a). This
       triggers transition to Idle Suspend (3a)
     - At this point, when transitioning from Traffic suspend to
       Idle suspend, HDR has RF chain. This is causing problem in
       Idle suspend init.
     We don't kill PRI_CHAIN_EVENT_SIG here if transitioning from 4a -> 3a, so
     that Idle suspend init can process the signal */

  if( !( ( next_state == HDRSRCH_SUSPENDED_IDLE_STATE ) && 
         ( hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE ) ) )
  {
    hdrsrch_kill_signal( HDRSRCH_PRI_CHAIN_EVENT_SIG );
  }  

  hdrsrch_kill_timer( &hdrsrch.timeout_timer );

  /* The following if-else statement implement:
              next_state  action
     ASDIV:   4           resume
              4c          resume
              4d          no-op
              others      exit tc
   
     ARD:     4           no-op (done in TC tuneback to ensure PRI is on)
              4c          no-op
              4d          no-op
              others      exit tc
   
              */

#ifdef FEATURE_HDR_SELECTIVE_TXD
  payload.next_state = next_state;
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  if ( next_state == HDRSRCH_CONNECTED_STATE )
  {
#ifdef FEATURE_HDR_SELECTIVE_TXD
    hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_RESUMED );
#endif /* FEATURE_HDR_SELECTIVE_TXD */
  }
  else if ( next_state == HDRSRCH_OFS_TC_STATE )
  {
#ifdef FEATURE_HDR_SELECTIVE_TXD
    hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_RESUMED );
#endif /* FEATURE_HDR_SELECTIVE_TXD */
  }
#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
  else if ( next_state != HDRSRCH_LTE_MEAS_TC_STATE )
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
  {
#ifdef FEATURE_HDR_SELECTIVE_TXD
    hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_EXITING_TRAFFIC );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_HDR_QTA
    if ( !hdrsrchtc_is_qta_enabled( ) )
#endif /* FEATURE_HDR_QTA */ 
    {
#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
      hdrsrchard_event( HDRSRCHARD_EVENT_EXIT_TC );
#else
      hdrsrchrfdiv_diversity_ctrl( FALSE );
#endif
    }

    hdrsrchafc_set_tap( FALSE );
     /* tap is not running */
  }

  if ( next_state != HDRSRCH_CONNECTED_STATE )
  {
    hdrsrchmsg_send_msg( &hdrsrchmsg.tuneaway_stop_msg.hdr, 
                         sizeof( hdrfw_tuneaway_stop_msg_t ) );

    MSG_HIGH("Send TUNE_AWAY_STOP msg - new state %x",next_state,0,0);
    /* Send out TUNEWAY stop message if missed to send out */
    /* get back to normal mode of DRC i.e. stop DRC ramp down mode */

#ifdef FEATURE_HDR_BAND_AVOIDANCE
    (void) timer_clr( &hdrsrch.band_avoid_timer, T_NONE );
      /* Clear the timer, and remove from wait mask */
#endif /* FEATURE_HDR_BAND_AVOIDANCE */
  }
} /* hdrsrchsus_tc_done( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION      HDRSRCHSUS_IDLE_TIMEOUT

DESCRIPTION
  Called when in Idle Suspended state for too long to assume idle operations
  can continue.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Causes System Lost.

===========================================================================*/

void hdrsrchsus_idle_timeout( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Idle suspend time out!  T=%d secs",
                  HDRSRCHSUS_TIMEOUT_SEC );

  hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );
    /* Could not get RF back for far too long */

} /* hdrsrchsus_idle_timeout( ) */



/* <EJECT> */
/*===========================================================================

FUNCTION      HDRSRCHSUS_SYNC_TIMEOUT

DESCRIPTION
  Called when in Suspended SYNC state for too long to assume SYNC operations
  can continue.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Causes System Lost.

===========================================================================*/

LOCAL void hdrsrchsus_sync_timeout( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsync_timeout();
    /* In SYNC or SUSPEND SYNC state too long.  Handle timeout.  */

} /* hdrsrchsus_idle_timeout( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION      HDRSRCHSUS_DUMP

DESCRIPTION
  Handle any dump interrupt that may occur in suspended state. It is possible
  that the active state (traffic or idle) did not have enough time for
  aborting the searches sent to the hardware. This will result in getting a
  dump in suspended state. Do nothing in this state.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Causes state transition.

===========================================================================*/

LOCAL void hdrsrchsus_dump ( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_kill_timer( &hdrsrch.lostdump_timer );
    /* Stop the timer, clear its signal, and remove from wait mask */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Received pending dump in %x state", 
                  hdrsrch_state_id );


} /* hdrsrchsus_dump( ) */

#ifdef FEATURE_HDR_QTA
/*===========================================================================

FUNCTION      HDRSRCHSUS_PROCESS_RXAGC_TRK_RSP

DESCRIPTION
  Process RX AGC track response received from FW during QTA tuneback.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchsus_process_rxagc_trk_rsp ( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDR QTA: Received RXAGC TRK from FW" );

  if ( hdrsrchtc_is_qta_enabled() )
  {
    hdrsrchsus.is_rxagc_trk_msg_rcvd = TRUE;
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "HDR QTA disabled" );
  }

  hdrsrchmsg_process_rcvd_rsp( HDRSRCH_RXAGC_TRACK_QTA_RSP );

} /* hdrsrchsus_process_rxagc_trk_rsp( ) */

/*===========================================================================

FUNCTION      HDRSRCHSUS_PROCESS_QUERY_RSP

DESCRIPTION
  Process query response received from FW when L1 queries FW for
  status of the previous gap.

DEPENDENCIES
  None

INPUTS
  QTA status indicating if just completed QTA gap was an empty gap
  or a gap used by GSM to decode page

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchsus_process_query_rsp ( hdrfw_qta_status_rsp_t qta_status )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDR QTA: Received Query resp from FW" );

  if ( hdrsrchtc_is_qta_enabled() )
  {
    hdrsrchsus.is_status_query_msg_rcvd = TRUE;
    hdrsrchsus.is_empty_gap = qta_status;
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "HDR QTA disabled" );
  }

  hdrsrchmsg_process_rcvd_rsp( HDRSRCH_QTA_STATUS_QUERY_RSP );

} /* hdrsrchsus_process_query_rsp( ) */

/*===========================================================================

FUNCTION      HDRSRCHSUS_IS_PRI_CHAIN_SETTLED

DESCRIPTION
  This function returns whether DO L1 received RX AGC track response
  from FW during QTA tuneback.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  TRUE: Yes response has been received.
  FALSE: Still waiting on response.

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrsrchsus_is_pri_chain_settled ( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HDR QTA: Received RXAGC TRK from FW %d", 
                  hdrsrchsus.is_rxagc_trk_msg_rcvd );

  return hdrsrchsus.is_rxagc_trk_msg_rcvd;
  
} /* hdrsrchsus_is_pri_chain_settled( ) */
#endif /* FEATURE_HDR_QTA */

