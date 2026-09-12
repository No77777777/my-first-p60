/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            H D R    S R C H    I D L E    M O N I T O R     S T A T E

                           Search Idle Monitor State Module

GENERAL DESCRIPTION
  This file contains the HDR Searcher Idle Monitor state machine. HDR Idle
  state has following different sub-states

  Monitor  : Terminal is awake and is monitoring the pilot 
  Access   : (Currently part of monitor) Terminal is in access state
  Sleep    : Terminal is in power-save sleep state
  Reacq    : Terminal is trying to re-acquire pilot after sleep
  OFS      : Terminal is searching for off-frequency pilots
  BCAST    : Terminal is actively listening to HDR broadcast 
  BCAccess : Terminal is listening to HDR broadcast and is in access
  Suspend  : Idle operation has been temporarily suspended 

This file contains files for monitor state implementation.

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2004 - 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchmon.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ------------------------------------------------------- 
02/20/2018   vko     Use OVHD reason for sync to idle state change
11/07/2017   vko     Change reason from Page demod to OVHD, after coming from traffic
10/21/2016   rmv     re/initialize channel change param on entering Monitor
06/09/2014   ljl     Added HDRSRCH_AFC_ENABLE_RGS_REPORT_CMD.
05/05/2014   ljl     Added HDRSRCH_CHANNEL_CHANGE_AND_NSET_UPDATE_CMD.
03/24/2014   dsp     Fixed OFS race condition.
10/24/2013   rmg     Disabled DLNA in idle state.
08/30/2013   dsp     Applied SCDO/MCDO clock settings when entering Idle.
07/09/2013   rmg     Fixed issue where early preamble detection was left
                     enabled during access.
06/26/2013   wsh     Added Selective TxD feature
05/24/2013   rmg     Added D-LNA support.
04/22/2013   arm     Added changes for DSDA coexistence manager.
05/14/2013   ljl     Activate decoder after RF tune.
05/02/2013   ljl     Saved scc offset and rate.
02/11/2013   vko     Fix compiler warnings.
10/05/2012   ukl     Resolved compilation errors after removing FEATURE_BCMCS.
09/05/2012   rmg     Added support for antenna tuner feature.
08/15/2012   smd     Supported idle tune away.  
07/26/2012   arm     Supported diversity in reacq. 
06/21/2012   mbs     Initialized monitor state when entering from reacq.
05/25/2012   wsh     Moved ARD logic into it's own state machine 
02/22/2012   arm     Fixed Idle Div bug for intra scc sleep. 
02/08/2012   arm     Supported diversity Ec/Io. 
02/07/2012   arm     Removed compiler warning. 
01/23/2012   arm     Idle Diversity bug fixes. 
01/11/2012   ljl     Added the parameter for search priority to hdrsrchdrv_enable_srch4().
11/29/2011   arm     Removed compiler warnings.
11/11/2011   arm     HDR idle and access state diversity implemetation. 
10/24/2011   wsh     Updated cancel FPD procedure 
07/21/2010   smd     Don't request continue afc tracking if ofs channel is 
                     same as aset channel.
03/24/2010   pxu     Removed FEATURE_HDR_QSM features 
10/14/2009   kss     Removed command to FW to disable DRC (done by RTCMAC). 
09/30/2009   kss     Send message to disable DRC.
07/16/2009   lyl     Supported CC config MSGR interface;
                     Removed the obsoleted function.
07/06/2009   lyl     Fixed the compilation issue in RUMI build.
05/18/2009   ljl/lyl Updated for firmware MSGR interface.
04/21/2009   etv     Replaced obsolete hdrrx APIs with hdrdec APIs.
03/09/2009   smd     Added support for pilot measurement velcro solution.
11/18/2008   ljl     Retried Update BC info cmd if tune away is gone.
11/10/2008   ljl     Supported HDRSRCH_ACMAC_DISABLE_TA_CMD.
08/22/2008   lyl     Supported primary chain ASET search.
03/27/2008   ljl     Cleaned up the Rev B macro.
01/29/2008   ljl     Fixed the featurization problem.
01/08/2008   ljl     Disabled tune away when entering BCA state.
10/26/2007   ljl     Notified AMAC no tune away when entering BCA state 
                     from access substate.
11/20/2007   rmg     Removed compilation errors on 7800.
11/13/2007   jyw     Added the power strobe event id.
10/17/2007   jyw     Added revB set management support.
09/27/2007   grl     Added time out of service support to sleep_info pkt.
08/19/2007   grl     Resolved Lint High warnings.
06/20/2007   rkc     Changed hdrsrchmon_process_int_cmd() input to payload ptr
04/10/2007   mt      Added support for RUP update request.
01/12/2007   grl     Remove reset of DAST when moving to idle.
10/23/2006   grl     Don't change the priority to paging when already in an
                     idle state such as reacq.
10/17/2006   ljl     Added HDR SRCH AFC stm.
10/09/2006   ljl     Fixed hybrid access problem related to AHO.
10/04/2006   mt      If FPD is active, do not enable missed CC ISR.
09/19/2006   grl     Added support for DDARF.
08/28/2006   sq      Added support for FEATURE_HDR_QSM
08/21/2006   grl     Reset the AGC estimator when appropriate.
07/27/2006   mt      Leave missed preamble isr enabled.
07/24/2006   jyw     Added power profiling strobes.
06/28/2006   jyw     Added MPC supports.
07/10/2006   mt      Added support for sleep timeline optimization.
06/07/2006   ljl     Added SBHO optimization.
05/03/2006   ljl     Added support for hybrid access
03/30/2006   sq      Check last state before going back to BC states
08/18/2005   ljl     IHO optimization
08/18/2005   ljl     Removed sclk_begin_estimate() from idle state
09/01/2005   grl     Added support for lock releases during access attempts.
08/29/2005   sq      Set priority when entering monitor state
08/11/2005   grl     Mainlined FEATURE_HDR_TCXOMGR.
05/31/2005   grl     Added support for TRM and TCXOMgr.
05/05/2005   ds      Included header file plt_mc.h for PLT
04/21/2005   kss/sq  Used new hdrdec mode setting APIs
03/09/2005   sq      Keep schedule timer running after 3->3d transition
01/14/2005   ks/sq   Reset SCC offset at 4->3 and 3b->3 state transitions
01/13/2005   sq      Mainlined FEATURE_HDR_NO_HO_DURING_SCC
12/02/2004   sq      Supported 3->3d state transition
10/06/2004   aaj     Fixed problem with BC transition during idle access
09/10/2004   sq      Added function hdrsrchmon_handle_idle_ho
08/30/2004   aaj     Use enum type to represent BCMCS status
08/23/2004   sq      Fixed the issue of 3b to 3 state transition
08/23/2004   ds      Update HDR debug display record
07/30/2004   sq      Added support to FEATURE_HDR_BCMCS
07/29/2004   aaj     Fixed access handoff bug for incremental threshold change
07/22/2004   ajn     Added common signals to state machine
06/08/2004   sq      Bug fix for SLEEP INFO LOG packet
05/11/2004   aaj/sq  Separated monitor state from hdrsrchidle.c. Refer to 
                     hdrsrchidle.c for complete edit history
==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"
#include "sys.h"

#ifdef FEATURE_CMI
#include "sys_v.h"
#endif



#include "hdrdebug.h"
#include "err.h"

#include "hdrsrchidlei.h"
#include "hdrsrchfing.h"
#include "hdrsrchdrv.h"
#include "hdrmdspmcr.h"

#include "hdrsrchtask.h"
#include "hdrsrchutil.h"
#include "hdrsrchset.h"
#include "hdrsrchrf.h"
#include "hdrsrchlog.h"

#include "hdrdec.h"
#include "hdrrx.h"
#include "hdrsrchafc.h"


#ifdef FEATURE_FACTORY_TESTMODE
#include "ftm.h"
#endif /* FEATURE_FACTORY_TESTMODE */


#include "hdrsrchstates.h"

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV
#include "hdrsrchutil.h"
#include "hdrsrchrfdiv.h"
#endif

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/


/* <EJECT> */
/*==========================================================================

                     HDR SEARCH IDLE STATE DATA

As much IDLE data is lumped together in one structure, so that it becomes
possible to dynamically allocate acquisition data memory during the
IDLE state, and discard the memory when no longer needed.

This will become important when RAM is based on SDRAM.

All variables are accessed as "hdrsrchidle.member_name".  When dynamic
allocation is implemented, a pointer to the structure is needed instead.
At this point, hdrsrchacq must become a dereferenced pointer, instead of
a variable.  Ie)

  #define hdrsrchidle  (*hdrsrchmon_struct_ptr)

==========================================================================*/


/* EJECT */
/*==========================================================================

                     STATE DISPATCH TABLE

This section provides the dispatch functions for events that are received
by the task while in this state.

==========================================================================*/

LOCAL void            hdrsrchmon_init( hdrsrch_state_enum_type last_state );
LOCAL void            hdrsrchmon_done( hdrsrch_state_enum_type next_state );
LOCAL errno_enum_type hdrsrchmon_cmd( const hdrsrch_cmd_type * cmd );
LOCAL void            hdrsrchmon_dump( void );
LOCAL void            hdrsrchmon_process_int_cmd
                      (
                        hdrsrchsm_cmd_payload_type *
                      );

LOCAL void            hdrsrchmon_process_low_pri_rsp_ind
                      (
                        hdrsrchmsg_s_type         *msg
                      );

LOCAL hdrsrch_state_struct_type hdrsrchmon_state =
{
  HDRSRCH_IDLE_STATE,
  hdrsrchmon_init,
  hdrsrchmon_done,
  hdrsrchmon_cmd,
  hdrsrchmon_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
  hdrsrchidle_sched,
  hdrsrch_default_lost_dump,
  hdrsrchidle_timeout,
  hdrsrchidle_rx_power_report,
#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
  hdrsrchidle_ta_timer_event,
#else /* FEATURE_HDR_IDLE_TUNE_AWAY */
  #ifdef FEATURE_HDR_HYBRID_ACCESS
  hdrsrchidle_access_ta_timer_event,
  #else
  hdrsrch_default_gp_timer1,
  #endif /* FEATURE_HDR_HYBRID_ACCESS */
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */
  hdrsrch_default_gp_sig1,
  hdrsrch_default_gp_sig1,
  hdrsrch_default_time_valid,
  hdrsrch_default_rxchipx16,
  hdrsrchidle_rf_unlock_event,
  hdrsrchmon_process_int_cmd,
  hdrsrchmon_process_low_pri_rsp_ind
};


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHMON_INIT

DESCRIPTION
  Idle Monitor State Initialization

DEPENDENCIES
  Synchronized on an HDR System

PARAMETERS
  last_state  - Last HDR Searcher State

RETURN VALUE
  None

SIDE EFFECTS
  Sets 26.67ms Dmod Frame ISR

===========================================================================*/

LOCAL void hdrsrchmon_init( hdrsrch_state_enum_type last_state )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_default_init( last_state );
    /* Use default handling */

  HDR_PWR_PROFILE_STRB_TOGGLE( HDRSRCH_PWR_EVENT_MON_INIT );

#ifndef FEATURE_HDR_REVB
  HDRDRC_SET_BEST_ASP( hdrsrchset.aset[0]->aset.asp_idx );
#else
  hdrsrchmsg_basp_ho_ovrd( HDRMDSP_DEMOD_CARRIER_0,
      hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->aset.asp_idx );
    /* Send best ASP handoff override */
#endif /* FEATURE_HDR_REVB */

  hdrsrchfing_restore_fing_thresh();
    /* Restore the finger in-lock/out-of-lock thresholds if any is pending */

  hdrsrchdrv_enable_srch4( SRCH4_PRIO_HDR_REACQ_PAGING );
    /* Set search priority */

  hdrsrchutil_mcdo_check();

  if ( last_state == HDRSRCH_REACQ_STATE )
  {
    /* Only enable missed preamble detection if the reacq succeeded. Otherwise
       failed reacq indication was already sent to the protocols and supercedes
       missed preamble detection. */
#ifdef FEATURE_HDR_DDARF
    if ( hdrsrchreacq_last_reacq_succeeded() )
#endif
    {

      /* Enable missed CC detection only if 
         forced preamble detection is not active. 
         
         If not enabled here ,CMAC enables missed CC detection 
         after the first good or bad CC packet.
      */

      if ( hdrsrchreacq_get_fpd_activation_status() == FALSE )
      {
        hdrsrchutil_enable_missed_scc_detection( FALSE );
        /* Enable missed SyncCC detect in idle. Don't clear the interrupt if one
           is pending since we may be entering monitor state after the preamble. */
      }
#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
      if(hdrsrch.idle_state_div.idle_div_enabled && 
         hdrsrch.idle_state_div.wakeup_info.div_enabled )
      {
        if( !hdrsrchrfdiv_diversity_is_enabled() )
        {
          /* If we timed out and were not able to enable div in reacq
             enable it now.*/

           hdrsrchrfdiv_diversity_ctrl( TRUE );
             /* Enable Diversity */

           HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: Enabling div in Monitor");
        }
      }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */
    }
  }
  else
  {
    hdrsrchutil_enable_missed_scc_detection( TRUE );
      /* Enable missed SyncCC detect in idle. Clear the interrupt if one
         is pending. */
  }

  hdrsrchidle.supervision_timer_active  = FALSE;
  hdrsrch_add_to_wait_mask( HDRSRCH_TIMEOUT_SIG );
    /* Initialize pilot supervision timer support */

  hdrsrchidle.handoff.allowed   = TRUE;
  hdrsrchidle.handoff.last_hstr = HDRHSTR_GET_COUNT() % 512;
    /* Handoffs are allowed until the start of the next CCC */

  if ( hdrsrchidle.handoff.last_hstr >= HDRSRCHIDLE_NO_HANDOFF_HSTR )
  {
    hdrsrchidle.handoff.last_hstr -= 512;
    hdrsrchidle.handoff.allowed   = FALSE;
      /* Next CCC appears to be now - disable handoffs */
  }

  switch ( last_state )
  {
    case HDRSRCH_SYNC_STATE:
    {
      hdrsrchrf_change_priority( HDRSRCHRF_PRI_CHAIN, HDRSRCHRF_OVHD );
        /* Alter priority to indicate we are monitoring page decodes. */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
      if( hdrsrch.idle_state_div.idle_div_enabled )
      {
        /* Sync to Monitor enable div ny default*/
        
           hdrsrchrfdiv_diversity_ctrl( TRUE );
             /* Enable Diversity */

        HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: Enabling div in Monitor");
        
      }        
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

      hdrsrchidle_init_monitor_state( );
        /* Enter IDLE in Monitor state */

      if ( hdrsrchlog.oos_timer_active )
      {
        hdrsrchlog.time_oos_ms = hdrsrchutil_stop_timer_ms( hdrsrchlog.oos_timer );
          /* Capture how long we were out of service, because we just gained service. */

        hdrsrchlog.sleep_info.total_time_out_of_service_ms += hdrsrchlog.time_oos_ms;
          /* Accumulate the time out of service. */

        hdrsrchlog.oos_timer_active = FALSE;
          /* Clear the oos timer active field. */
      }

      hdrsrchidle.handoff.report  = NULL;
        /* No Idle Handoff callback (yet) */

      hdrsrchidle.channel_change  = NULL;
        /* No channel change is in progress */

      hdrsrchsleep_reset_sci( );
        /* Set default slot cycle index to 0 */

      hdrsrchafc_enable_rgs_report();
        /* Enable RGS report */
    }
    break;

    case HDRSRCH_REACQ_STATE:
    {
      hdrsrchidle_prepare_monitor_state();
      /* Initialize many state variables as we enter Monitor state */
      
      hdrsrchidle.handoff.allowed   = FALSE;
        /* Handoffs are disabled coming out of reacq - always */

      HDRSRCH_SIG_SET( HDRSRCH_SCHEDULE_SIG );
        /* Restart the search scheduler */

      hdrsrchidle.channel_change  = NULL;
        /* Re/Initialize channel change params. This should get assigned
           when the CHANNEL_CHANGE_CMD is processed in Monitor state */
    }
    break;

    case HDRSRCH_CONNECTED_STATE:
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Previous state is connected. Change reason to %d",
						HDRSRCHRF_OVHD );
      hdrsrchrf_change_priority( HDRSRCHRF_PRI_CHAIN, HDRSRCHRF_OVHD );

#ifndef FEATURE_HDR_REVB
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "HO Override. BASP=%d",
                      hdrsrchset.aset[0]->aset.asp_idx );
#else
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "HO Override. BASP=%d",
        hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->aset.asp_idx );
#endif /* FEATURE_HDR_REVB */

      hdrsrchsleep_set_scc_info( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->scc_offset,
                                   hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->scc_rate );
        /* Mark the SCC offset as invalid. It is because that the current ASP
           we selected when leaving traffic state may not be the last ASP from
           which we received the latest SCC offset value.                   */

      hdrsrchsleep_send_msg_update_scc_offset( );
        /* Send the message to update SCC offset */

#ifndef FEATURE_HDR_REVB
      hdrsrchidle_handoff_link_report( hdrsrchset.aset[0] );
#else
      hdrsrchidle_handoff_link_report( 
                      hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0] );
#endif /* FEATURE_HDR_REVB */
        /* When HDRSRCH enters idle state from connected state, the 
           strongest pilot in ASET is kept in aset[0] and others (if
           there is any) will be removed from ASET. The decoder might
           still be decoding CC packets from a sector different than
           aset[0], when this state transition happens. If this is the
           case and if the AT has access probes to send right after the
           state change, the access probe will be sent to the sector from
           where we received those CC packets, not to aset[0]. But we only
           monitor aset[0] when HDRSRCH is in idle state, so we will miss
           the ACK for the probes. Adding this extra handoff indication 
           will force protocol to wait for an overhead update from aset[0]
           before sending out new access probes.                        */

      hdrsrchsleep_reset_sci( );
        /* Set default slot cycle index to 0 */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
      if( hdrsrch.idle_state_div.idle_div_enabled )
      {
        /* Connected to Monitor enable div by default*/
        
           hdrsrchrfdiv_diversity_ctrl( TRUE );
             /* Enable Diversity */

        HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: Enabling div in Monitor");
        
      }             
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

      hdrsrchidle_init_monitor_state( );
        /* Enter IDLE in Monitor state */

      hdrdec_enter_mode( HDRDEC_MONITOR_MODE );
        /* set the decoder for idle/sync mode where the packets are delivered
           as soon as they are decoded */

      (void) rex_set_timer( &hdrsrch.schedule_timer, HDRSRCH_IDLE_SEARCH_INTERVAL_MS );
        /* And schedule next search request */
    }
    break;

#ifdef FEATURE_HDR_BCMCS
    case HDRSRCH_BC_IDLE_STATE:
    {
      hdrsrchrf_change_priority( HDRSRCHRF_PRI_CHAIN, HDRSRCHRF_DEMOD_PAGE );
        /* Alter priority to indicate we are monitoring page decodes. */

      hdrsrchsleep_set_scc_info( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->scc_offset,
                                   hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->scc_rate );
        /* Mark the SCC offset as invalid. It is because that the current ASP
           we selected when leaving idle broadcast state may not be the last 
           ASP from which we received the latest SCC offset value.          */

      hdrsrchsleep_send_msg_update_scc_offset( );
        /* Send the message to update SCC offset */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
      if(hdrsrch.idle_state_div.idle_div_enabled && 
         hdrsrch.idle_state_div.wakeup_info.div_enabled )
      {
        if( !hdrsrchrfdiv_diversity_is_enabled() )
        {
          hdrsrchrfdiv_diversity_ctrl( TRUE );
             /* Enable Diversity */;

           HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: Enabling div in Monitor");
        }
      }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

      hdrsrchidle_init_monitor_state( );
        /* Enter IDLE in Monitor state */
    }
    break;
#endif /* FEATURE_HDR_BCMCS */

#ifdef FEATURE_HDR_HYBRID_ACCESS
    case HDRSRCH_SUSPENDED_IDLE_STATE:
      /* Intentional fall-through. */
#endif /* FEATURE_HDR_HYBRID_ACCESS */
    case HDRSRCH_OFS_IDLE_STATE:
    {

      /* We were in access before entering idle suspend */

      hdrdec_reset_pkt_sequences();
        /* Reset packet sequence numbers */

#ifdef FEATURE_HDR_HYBRID_ACCESS
      hdrsrchidle.waiting_for_close_loop = TRUE;
        /* Now we need permission from TCXO Manager before we
           close the TCXO loop. */
#endif /* FEATURE_HDR_HYBRID_ACCESS */
      if ( hdrsrchofs_is_ofs_channel_in_aset() == FALSE )
      {
        /* We only need to request continue trk if the searched channel 
           is not the same as aset channel OFS. In case of the searched 
           channel is same as aset channel, we don't stop freq tracking in 
           ofs state. That's why we don't request continue tracking here. */

        hdrsrchafc_request_continue_trk();
          /* Now we need to close the TCXO loop so we can resume tc. */
      }

      break;
    }

    default:
    {
      ERR("Invalid %x -> %x transition", last_state, hdrsrch_state_id, 0);
    }

  } /* switch( last_state ) */


  hdrsrchutil_enable_roll( );

  hdrsrch_add_to_wait_mask( HDRSRCHIDLE_RX_RPT_SIG );
  (void) rex_set_timer( HDRSRCHIDLE_RX_RPT_TIMER, HDRSRCHIDLE_RX_RPT_TIME_MS );
    /* Report RX power periodically while in IDLE state */

  /* On SUSPENDED to IDLE/ACCESS transition, report ACCESS state entry */

  /* The following old code is to complete Access command. 
     The new code should remember whether we come here from Suspend state. 
     If we come from suspend state, the access command has completed 
     and we should resume access. */
  
  if ( ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE ) && 
       ( last_state != HDRSRCH_SUSPENDED_IDLE_STATE ) )
  {
      /* Complete access command */

    hdrsrchidle.access.suspend_ho_cb();
      /* Report handoffs have been suspended */

    hdrsrch_complete_cmd( );
     /* We've completed the ACCESS command now */

    hdrsrchidle_enable_tune_aways( );
      /* Allow interruptions during access attempts. */

  }

#ifdef FEATURE_HDR_BCMCS
  if ( ( last_state != HDRSRCH_BC_IDLE_STATE    ) &&
       ( last_state != HDRSRCH_BC_ACCESS_STATE  )  )
  {
    if ( hdrsrchbc_get_bc_status( ) == BCMCS_ENABLED )
    {
      if ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE ) 
      {
        hdrsrchbca_init_bca_state( );
          /* Transition to BCA state if BCMCS is enabled at this moment */
      }
      else
      {
        hdrsrchbc_init_bc_state( );
          /* Transition to BC idle state if BCMCS is enabled at this moment */
      }
    }
#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
    else
    {
      if ( last_state != HDRSRCH_SUSPENDED_IDLE_STATE )
      {
        hdrsrchidle_monitor_enable_tune_away();
         /* Don't retain lock for BC state which has its own handling. If we come from
            idle suspend state, we will retain lock after RF tune back is done
         */
      }
    }
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */
  }
#else /* FEATURE_HDR_BCMCS */
#ifdef FEATURE_HDR_IDLE_TUNE_AWAY

  if ( last_state != HDRSRCH_SUSPENDED_IDLE_STATE )
  {
    hdrsrchidle_monitor_enable_tune_away();
     /* If we come from idle suspend state, we will retain lock after RF tune back is done */
  }
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */
#endif /* FEATURE_HDR_BCMCS */

  /* Signal PLT MC task that HDR searcher is in idle state  */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
  if( hdrsrch.idle_state_div.idle_div_enabled  && 
      hdrsrchidle.monitor_substate == HDRSRCHIDLE_MONITOR_STATE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_LOW, "IDLE DIV: Registering for slot based logging" );

    hdrlog_register_slot_logging_cb ( HDRLOG_SRCHDIV_MDSP_SLOT_LOGGING_CLIENT,
                                      hdrsrchidle_process_psinr_mdsp_logging_event);
      /* Register call back for slot based logging of psinr */
  }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */ 

} /* hdrsrchmon_init */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHMON_DONE

DESCRIPTION
  Idle State Finalization

DEPENDENCIES
  Exiting IDLE state

PARAMETERS
  next_state  - State we are transitioning to

RETURN VALUE
  None

SIDE EFFECTS
  Various state timers cleared.

===========================================================================*/

LOCAL void hdrsrchmon_done( hdrsrch_state_enum_type next_state )
{
#ifdef FEATURE_HDR_BCMCS
#ifdef FEATURE_HDR_HYBRID_ACCESS

  qword                          ta_time;
    /* Tune away time in slots */

#endif /* FEATURE_HDR_HYBRID_ACCESS */
#endif  /* FEATURE_HDR_BCMCS */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "TxD: mon_done");

  hdrsrchutil_cancel_fpd();

  hdrrx_disable_missed_scc_detection();
    /* Only need to check missed SyncCC in idle state, so disable on exit */

#ifdef FEATURE_HDR_BCMCS
  if ( (next_state != HDRSRCH_CONNECTED_STATE ) &&
       (next_state != HDRSRCH_BC_ACCESS_STATE ) &&
       (next_state != HDRSRCH_BC_IDLE_STATE   )  )
#else /* FEATURE_HDR_BCMCS */
  if ( next_state != HDRSRCH_CONNECTED_STATE )
#endif /* FEATURE_HDR_BCMCS */
  {
    hdrsrch_stop_timer( &hdrsrch.schedule_timer );
      /* Stop the Search Scheduler, unless heading to traffic state */
      
    HDRSRCH_SIG_CLR( HDRSRCH_SCHEDULE_SIG );
     /* Clear HDRSRCH_SCHEDULE_SIG too*/

  }

  if ( 
#ifdef FEATURE_HDR_BCMCS
       ( next_state                   != HDRSRCH_BC_ACCESS_STATE )  &&
#endif  /* FEATURE_HDR_BCMCS */
#ifdef FEATURE_HDR_HYBRID_ACCESS
       ( next_state                   != HDRSRCH_SUSPENDED_IDLE_STATE )  &&
#endif /* FEATURE_HDR_HYBRID_ACCESS */
       ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE )  )
  {
    /* When leaving IDLE state, ensure we leave IDLE/Access state */

    HDR_MSG_SRCH( MSG_LEGACY_MED, "Exiting Access Mode" );

    hdrsrchidle_init_monitor_state( );
      /* Transition to IDLE/Monitor */
  }

#ifdef FEATURE_HDR_IDLE_TUNE_AWAY

  if ( ( next_state                   != HDRSRCH_CONNECTED_STATE ) 
#ifdef FEATURE_HDR_BCMCS
       && ( next_state                   != HDRSRCH_BC_ACCESS_STATE )
#endif  /* FEATURE_HDR_BCMCS */
      )
  {
    hdrsrch_kill_timer( HDRSRCHIDLE_MONITOR_TA_TIMER );
      /* Kill monitor tune away timer */

    hdrsrchidle_disable_tune_aways( );
      /* Turn off interruptions now that we are leaving idle state. */
  }

#else /* FEATURE_HDR_IDLE_TUNE_AWAY */

  if ( ( next_state                   != HDRSRCH_CONNECTED_STATE ) &&
#ifdef FEATURE_HDR_BCMCS
       ( next_state                   != HDRSRCH_BC_ACCESS_STATE ) &&
#endif  /* FEATURE_HDR_BCMCS */
       ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE ) ) 
  {
    hdrsrchidle_disable_tune_aways( );
      /* Turn off interruptions now that we are leaving the access state. */
  }                                                                         a

#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */

#ifdef FEATURE_HDR_BCMCS
#ifdef FEATURE_HDR_HYBRID_ACCESS  
  if ( ( next_state                   == HDRSRCH_BC_ACCESS_STATE ) &&
       ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE ) &&
       ( hdrsrchidle.access.tune_away.amac_ready == TRUE ) )
  {
    
    qw_set(ta_time, 0xFFFFFFFF, 0xFFFFFFFF );
      /* Set tune away time to max */

    if ( hdrsrchidle.access.ta_time_cb != NULL )
    {
      hdrsrchidle.access.ta_time_cb( TRUE, ta_time );
        /* Notify ACMAC */
    }
    else
    {
      ERR_FATAL( "No ta_time_cb()", 0, 0, 0 );
    }

    if ( hdrsrchidle.access.tune_away.state == 
                                HDRSRCHIDLE_TUNE_AWAY_GONE ) 
    {
      hdrsrchidle_disable_tune_aways( );
        /* Disable tune away */
    }

  }
#endif  /* FEATURE_HDR_HYBRID_ACCESS */
#endif  /* FEATURE_HDR_BCMCS */

  hdrsrch_default_done( next_state );
    /* Use default handling */

  hdrsrchidle_kill_rx_power_report();
    /* Terminate periodic RX power reports. */

  hdrsrch_kill_timer( &hdrsrch.timeout_timer );
    /* Clear TimeOut processing */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  (void) timer_clr( &hdrsrch.coex_timer, T_NONE );
    /* Clear the timer, and remove from wait mask */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */


  /* TODO: Other finalizations... */

} /* hdrsrchmon_done( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHMON_CMD

DESCRIPTION
  This function ...

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

LOCAL errno_enum_type hdrsrchmon_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type               status = E_SUCCESS;
     /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "Received cmd %d", cmd->name );


  /* Dispatch command to appropriate handler */

  switch ( cmd->name )
  {
#ifdef FEATURE_HDR_HYBRID_ACCESS

    case HDRSRCH_ACMAC_READY_CMD:
      
      status = hdrsrchidle_acmac_ready_cmd( &cmd->params.acmac_ready_params );
      break;

    case HDRSRCH_ACMAC_TA_TIME_OK_CMD:
      
      status = hdrsrchidle_acmac_ta_time_ok_cmd( 
                                              &cmd->params.ta_time_ok_params );
      break;

    case HDRSRCH_ACMAC_DISABLE_TA_CMD:

      status = hdrsrchidle_acmac_disable_ta_cmd( 
                                        &cmd->params.acmac_disable_ta_params );
      break;

#endif /* FEATURE_HDR_HYBRID_ACCESS */
    
    case HDRSRCH_CHANNEL_CHANGE_CMD:
      status =
        hdrsrchidle_channel_change_cmd( &cmd->params.channel_change );
      break;

    case HDRSRCH_CHANNEL_CHANGE_AND_NSET_UPDATE_CMD:
      status = hdrsrchidle_channel_change_and_nset_update_cmd( 
                        &cmd->params.channel_change_and_nset_update );
      break;

    case HDRSRCH_ACCESS_HANDOFF_CMD:
      status =
        hdrsrchidle_access_handoff_cmd( &cmd->params.access_handoff );
      break;

    case HDRSRCH_TC_ASSIGNMENT_CMD:
      status = hdrsrchidle_tc_assign_cmd( );
      break;

    case HDRSRCH_TC_ACK_RECEIVED_CMD:
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Delayed TCC_ACK cmd in st=%x",
                      hdrsrch_state_id );
      break;

#ifdef FEATURE_HDR_BCMCS
    case HDRSRCH_UPDATE_BC_INFO_CMD:

      hdrsrchbc_set_bc_status( cmd->params.bc_info.bcmcs_status );
        /* Update the overall status of BC service in HDR srch */

      /* Depending on the current idle substate, we decide whether
         to transition to Idle BC state or Idle BC access state.
         If tune away is gone, retry the cmd. */
      if ( hdrsrchbc_get_bc_status() == BCMCS_ENABLED )
      {
        if ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE )
        {
          if ( hdrsrchidle.access.tune_away.state == HDRSRCHIDLE_TUNE_AWAY_GONE ) 
          {
            HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                            "Received cmd %d but access tune away gone! ",
                            hdrsrch_state_id );
            status = E_AGAIN;
          }
          else
          {
            hdrsrchbca_init_bca_state( );
              /* access substate -> 3d transition  */
          }
        }
        else
        {
          hdrsrchbc_init_bc_state( );
            /* We are asked to transition to idle broadcast state  */
        }
      }
      /* This command always is successful so status = SUCCESS */
    break;
#endif /* FEATURE_HDR_BCMCS */


    case HDRSRCH_PILOT_MEAS_CMD:
      status = hdrsrchutil_pilot_meas_req_cmd( &cmd->params.rup_req );
       
      if ( ( status == E_SUCCESS) && 
           ( hdrsrchofs_pilot_meas_pending() == TRUE ) )
      {
        hdrsrchofs_register_idle_ofs_return_state( HDRSRCH_IDLE_STATE );
          /* after idle ofs we should come back to monitor state */
      
        hdrsrch_on_searcher_idle( hdrsrchofs_init_ofs_state );
      } 
      break;

    default:
      status = hdrsrchidle_default_idle_cmd( cmd );
        /* Handle more general purpose commands */

  } /* switch (cmd->name) */

  return status;

} /* hdrsrchmon_cmd(cmd) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHMON_DUMP

DESCRIPTION
  This function handles the search dump signal.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchmon_dump( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchdrv.srch_list[0].sect_ptr == NULL )
  {
    /* This was a acquisition predetect search - ignore the dump and
       begin the regular search schedule */

    HDRSRCH_SIG_SET( HDRSRCH_SCHEDULE_SIG );
      /* Start the search schedule */

    return;
  }

  /* To make sure pending ASET search dump is processed after transition to 
     monitor state */
  if ( hdrsrchdrv.srch_task_type == HDRSRCH_PRI_CHAIN_ASET_SRCH )
  {
    hdrsrchutil_pri_chain_aset_dump( );
  }
  else
  {
    switch ( hdrsrchidle.monitor_substate )
    {
      case HDRSRCHIDLE_MONITOR_STATE:
        hdrsrchidle_dump_proc( hdrsrchidle_monitor_aset_mgmt );
          /* Search dump processing in idle monitor state  */
        break;

      case HDRSRCHIDLE_ACCESS_STATE:
        hdrsrchidle_dump_proc( hdrsrchidle_access_aset_mgmt );
          /* Search dump processing in idle access state  */

        /* Call RF API every x search dumps during access where 
            x = antenna tuner action interval */
        if( ( hdrsrchdrv.dump_cnt % 
          HDRSRCHRF_ANTENNA_TUNER_ACTION_INTERVAL ) == 0 )
        {
          hdrsrchrf_antenna_tuner_action();
            /* Send antenna tuner action tick to RF. */
        }

        break;

      default:

        break;
    } /* switch */

    /* Update HDR debug display record every 300 ms */
    if ( hdrsrchdrv.dump_cnt % 10  == 0 )
    {
      hdrsrchlog_debug_display_fill_rec();

    }
    if ( hdrsrchdrv.srch_task_type == HDRSRCH_DIVERSITY_SRCH )
    {
       hdrsrchdrv.srch_task_type = HDRSRCH_PRI_CHAIN_ASET_SRCH;
        /* To indicate primary chain ASET search */

       (void) hdrsrchutil_program_pri_chain_aset_search( );
         /* To perform the primary chain ASET search */
    }
  } /* end if hdrsrchdrv.srch_task_type */

} /* hdrsrchmon_dump( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHMON_INIT_IDLE_STATE

DESCRIPTION
  This function causes the state machine to begin idle state processing.

DEPENDENCIES
  System Acquired, Sync'd, fingers in lock, ...

PARAMETERS
  params - Idle state parameters.  (May be NULL)

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchmon_init_idle_state( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_change_state( &hdrsrchmon_state );
    /* Switch to idle state processing */

} /* hdrsrchmon_init_idle_state( ) */


/*==========================================================================

FUNCTION HDRSRCHMON_PROCESS_INT_CMD

DESCRIPTION
  This function processes internal commands for idle state.
    
DEPENDENCIES
  None

INPUTS
  payload   - Command+payload to be processed

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchmon_process_int_cmd
( 
  hdrsrchsm_cmd_payload_type                     *payload
    /* The internal command + payload */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1( MSG_LEGACY_MED,
                  "Received internal cmd %d at monitor state",
                  payload->cmd );

#ifdef FEATURE_HDR_HYBRID_ACCESS

  /* TBD */
  /* Need to support hybrid access */
  switch ( payload->cmd )
  {
    case HDRSRCH_AFC_CLOSE_LOOP_CMD:

      /* Closed loop */
      if ( hdrsrchidle.waiting_for_close_loop == TRUE )
      {
        hdrsrchidle.waiting_for_close_loop = FALSE;
          /* We don't actually track until we assign fingers, but this is close
             enough, there isn't any harm declaring this a bit early, and
             we currently don't check to see when we end up acquiring again
             when we resume from traffic. */
  
        hdrsrchidle_resume_idle_init();
          /* Tune back to HDR Idle state */
      } 

      break;

    default:

      break;
  }

#endif /* FEATURE_HDR_HYBRID_ACCESS */

} /* hdrsrchmon_process_int_cmd */

/*==========================================================================

FUNCTION        HDRSRCHMON_PROCESS_LOW_PRI_RSP_IND

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

void hdrsrchmon_process_low_pri_rsp_ind
(
   hdrsrchmsg_s_type         *msg
    /* Message */
)
{

/*-----------------------------------------------------------------------*/

  switch( msg->msg.hdr.id )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Rcvd FW rsp 0x%8x", msg->msg.hdr.id );

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
    case HDR_FW_SCC_DECODE_IND:

      hdrsrchidle_process_scc_decode_ind();
      break;
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

    default:
      hdrsrch_default_process_low_pri_rsp_ind( msg );
      break;
  }
} /* hdrsrchmon_process_low_pri_rsp_ind */
