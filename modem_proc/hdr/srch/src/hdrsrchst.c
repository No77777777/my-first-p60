/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           H D R    S R C H   I N A C T I V E   (START)   S T A T E

                     Search Inactive (start) State Module

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2000 - 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchst.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ------------------------------------------------------- 
01/30/2018   vko/vlc Calling deactivate done cb to almp only after receiving 
                     the DE_INIT_CNF REQ from LTE
12/07/2015   wsh     Block all but deactivate cmd when sys loss is pending
10/13/2015   vle     Release IRAT TRM grant before releasing DO TRM grant. 
10/06/2015   vlc     In function hdrsrchst_powerdown(), make sure DO cancels any 
                     outstanding IRAT TRM grant. 
08/17/2015   vlc     Added code to handle Run Control Event Notification errors.
07/13/2015   arm     Mainlined FEATURE_RF_SVDO_API feature.
06/18/2015   tnp     Fix compiler warnings
05/28/2015   vlc     Added support for DO->LTE redirection in connected state. 
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
04/27/2015   kss     Added support for dog HB.
02/10/2015   ljl     Fixed compiler warning.
10/14/2014   wsh     Changes to support Jolokia bringup
10/07/2014   rmv     For Bringup of JO hw at reduced clks, limit the max fingers 
03/05/2014   wsh     Bug fixes for DSDS-ASDiv and DSDA-ASDiv support 
12/20/2013   vlc     Added support for Tbackoff timer optimization. 
11/12/2013   mbs     Protected VSTMR registrations
09/26/1313   dsp     Reset RTC before enabling FW when we go to active state.
09/04/2013   wsh     Changes to support Bolt RUMI
05/30/2013   smd     Reported out of service to time service.
04/22/2013   arm     Added changes for DSDA coexistence manager. 
01/02/2013   mbs     Released primary chain only if HDR owns it
12/19/2012   smd     Added RUMI Tx support.
08/01/2012   mbs     Reset no_dump_cnt when DO is being reactivated
03/12/2012   mbs     Re-Enabled mcpm clks early to resume RTC
12/04/2012   mbs     Fixed compilation errors due to removal of LTE on Triton
10/19/2012   smd     Told fw whether the standby to active transition is for wakeup.
06/25/2012   rkc     Enabled MDSP App and srch4 driver for LTE to DO time sync. 
06/14/2012   rkc     Removed setting MCPM wakeup clocks hdrsrchst_done(). 
07/13/2012   vlc     Removed LTE deinit functionality in DO Inactive state to 
                     allow for Hybrid LTE reselection. 
06/07/2012   ljl     Set TDEC registers as a hack.
05/25/2012   wsh     Fixed FPD cancel related crashes
05/21/2012   ljl     Injected acq cmd for Dime RUMI.
03/19/2012   ljl     Fixed Dime compiler errors.
03/12/2012   mbs     Enabled mcpm clks early to resume RTC
02/13/2012   vlc     Added code to drop gRICE CSET in hdrsrchst_init(). 
02/01/2012   ljl     Deactivate decoder before disabling RF.
01/09/2012   smd     Supported dynamic acquisition diversity.
11/27/2011   grl     Removed obsolete sleep API.
11/11/2011   smd     Used hdrsrchsleep_reset_last_normal_sleep_sci(). 
11/04/2011   vlc     Added call to hdrsrchltemeas_lte_deinit_is_finished() 
                     in hdrsrchst_cmd().
10/28/2011   lyl     Supported MC gRice.
10/25/2011   ljl     Handled the failure cases during optimized HO.
10/17/2011   rmg     Removed calls to obsolete clk regim APIs.
09/29/2011   vlc     Moved code to send deinit request to after DO shutdown. 
                     Changed MCPM calls to make sure LTE can complete the
                     deinit operation.
09/17/2011   smd     Don't send MCPM request for L2DO meas.
07/29/2011   rmg     Removed unnecessary MCPM wake up req during srch init.
                     Added MCPM powerdown transition in srch powerdown.
05/19/2011   rmg     Updated HDR MCPM interface.
05/12/2011   ljl     Removed sleep clk estimate.
05/02/2011   kss     Fixed compiler warning.
04/27/2011   rmg     Added HDR MCPM support changes.
04/27/2011   smd     IRAT L2DO updates for NikeL.
04/12/2011   smd     Fixed input param type mismatch of hdrmdsp_config_hdr_app().
04/12/2011   smd     Used a hdr abstract type for fw state. 
03/03/2011   arm     Fixed NPA crash due to overlapping NPA calls. 
03/02/2011   vlc     Cleared Treselect timer when sending LTE deinit request. 
02/17/2011   vlc     Added LTE deinit handling to Inactive state.
01/27/2011   vlc     Added call to hdrsrchltemeas_clear_reselect_status(). 
01/20/2011   smd     Reset demod carrier mask when entering inactive state.
01/17/2011   smd     Supported FTM for IRAT connected mode measurement.
01/12/2010   smd     Removed code to set RF sample clock to TCXO clock after 
                     measurement. 
12/14/2010   ljl     Limited the max fingers and disabled div for mc on 9k.
11/25/2010   pxu     Added code to reset new sleep algorithm variables. 
11/18/2010   smd     Added support for offline search.
11/24/2010   ljl     Notified HDR CP that HDR SRCH was deactivated after releasing RF. 
11/16/2010   smd     Put ant clock under software control when DO is activated. 
10/11/2010   grl     Allow RF to control the RX front clocks for power savings.
10/1/2010    smd     Removed redundant call of config_cdma_clocks.
                     Don't call config_cdma_clocks for LTEToDO measurement.
09/21/2010   smd     Updated clock management for connected mode meas.
09/07/2010   lyl     Supported FW power management with suspend/resume.
08/18/2010   grl     Resync the clocks after TCXO shutdown.
08/16/2010   grl     Removed unused variable. 
08/06/2010   arm     DCVS changes for MDM9k 
07/26/2010   smd     Enabled clocks for timesync state in the same way as 
                     other states.
06/21/2010   cnx     Fixed compiler warnings.
06/17/2010   rmg     Added phase 2 clock gating changes.
06/02/2010   ljl     Removed mdsp.h.
05/12/2010   ljl     Disabled rx before hdr fw standby.
05/12/2010   rmg     Featurized clock gating changes.
05/11/2010   rmg     Added support for clock gating.
05/04/2010   rkc     Moved NPA client handle create and destroy.
04/23/2010   rkc     Cancelled NPA requests when entering inactive state.
04/15/2010   smd     Cleaned up clock API calling for pilot meas.
04/15/2010   ljl     Cleaned up the clock setting. 
04/06/2010   ljl     Enabled fws hdr app when exiting start state.
04/05/2010   ljl     Removed clock hack for Geneisis 1.1.
04/01/2010   smd     Added code not to config CDMA clock before time transfer.
03/20/2010   pxu     Added include "hdrmdspmcr.h" 
03/11/2010   ljl     Supported the fw timing state.
02/26/2010   ljl     Configured MODEM_CHIPXN_CLK_MISC_CTL for LTE to DO redirection.
02/17/2010   ljl     Cleaned up clock settings.
02/09/2010   smd     Added transit to TimeSync state.
02/02/2010   ljl     Stopped RxAGC before releasing RF.
01/08/2010   ljl     Removed SRCH FW enable from hdrsrch. 
01/06/2010   rkc     Removed HDRSRCHUTIL_NPA_CPU_ACTIVATED npa cancel request.
01/07/2010   lyl     Used the FWS API.
12/29/2009   ljl     Added API to enable firmware HDR App.
12/18/2009   ljl     Configured CDMA clocks when exiting from inactive state.
12/07/2009   ljl     Supported FWS.
11/11/2009   rkc     Fixed #elif/define error.
11/11/2009   rkc     Added NPA support.
11/09/2009   ljl     Added MDM9K SURF bringup code.
10/21/2009   rkc     Moved mdsp init back to srch task init.
10/19/2009   rkc     Initialized mdsp when exiting inactive state.
09/24/2009   rkc     Removed abort sleep when exiting inactive state.
09/14/2009   ljl     Supported srch MSGR interface.
07/09/2009   lyl     Sent the ASP udpate message during initialization.
05/18/2009   ljl     Updated for firmware MSGR interface.
04/21/2009   etv     Replaced obsolete hdrrx APIs with hdrdec APIs.
12/12/2008   rkc     Added include of clkregim.h
07/31/2008   rkc     Deregistered/Registered demod roll ISR when
                     entering/exiting INACTIVE state, respectively.
05/05/2008   rkc     Added clkrgm control of CLKRGM_RESOURCE_CDMA_TX
04/11/2008   rkc     Added clkrgm control of CLKRGM_RESOURCE_CDMA_DEMOD
01/03/2008   ljl     Deactivated afc before releasing rf.
11/20/2007   jyw     Used hdrsrch sleep macros.
10/17/2007   jyw     Added revB set management support.
09/27/2007   grl     Added time out of service info for rev A sleep log pkt.
06/30/2007   mt      HDR Adaptive diversity implementation.
06/12/2007   grl     Deregister the HDR activated clk client.
04/30/2007   mt      Added power up init for ATOM and RUP request support. 
04/28/2007   mt      Added call to initialize OFS timeline.
04/02/2007   grl     Changed sleep voting to poles of the acq timeline.
03/20/2007   grl     Register for high clock only after the RF lock is granted.
03/09/2007   grl     Moved search4 interface into hdrsrchdrv
01/25/2007   rkc     Included srch4trans.h.
01/19/2007   grl     Added srch4 request and release calls.
10/27/2006   etv     Disabled on-arrival RX pkt delivery before de-act SRCH.
10/17/2006   ljl     Added HDR SRCH AFC stm.
10/09/2006   mt      Reset forced preamble activation status at powerup.
09/22/2006   ljl     Added FEATURE_TRM_API_V2.
09/06/2006   jyw     Added FEATURE_DYNAMIC_CLOCK_FREQUENCY_CHANGING.
08/15/2006   rmg     Fixed a bug in HDR Clk disable feature
08/14/2006   rmg     Added HDR Clk disable feature
06/30/2006   jyw     Included the "task.h".
03/23/2006   ljl     Moved ASET and CSET pilots to NSET.
10/20/2005   jyw     Fixed the lint error.
10/04/2005   ds      Change to kill AFC sig and timer in hdrsrchst_init( ).
08/11/2005   grl     Mainlined FEATURE_HDR_TCXOMGR.
07/12/2005   sq      Added support for enhanced idle state protocol
05/31/2005   grl     Added support for TRM and TCXOMgr.
03/23/2005   hrk     Replaced hdrdec_deactivate by hdrsrchutil_dec_deactivate 
12/08/2004   sq/dna  Handled deactivation command in start state
09/07/2004   sq      Do not request partial results when aborting searrches
07/22/2004   ajn     Added common signals to state machine
06/30/2004   dna     Call the deactivate_done_cb in hdrsrchst_powerdown
06/25/2004   sq      Make Srch4 disable as part of power down process
06/21/2004   sq      Changed tune_timer to use SCLK timer
06/01/2004   sq      Replaced srch4_enable with new API
12/05/2003   sq      Check DSP clock before drivers shutdown
12/02/2003   aaj     Disable HDR application for in the start state
11/04/2003   bt      Replaced srch4_hdr_en/disable with srch4_enable iface.
10/28/2003   aaj     Support enable/disable of hdrmdsp and srch4
10/21/2003   aaj     Fixed compilation warnings in hdrsrchst_release_rf
09/16/2003   ajn     hdrsrchrf_sleep( ) -> hdrsrchrf_release( ),
                     MDSP: enable/disable HDR App moved to hdrsrchrf
09/18/2003   aaj     Restored rf_hdr_sleep change for hybrid mode integration
09/18/2003   mpa     Converted F3 messages to use new MSG2.0 HDR search SSID
09/18/2003   aaj     Temporarily back out "rf_hdr_sleep( )->hdrsrchrf_sleep( )"
09/10/2003   ajn     rf_hdr_sleep( ) -> hdrsrchrf_sleep( )
03/11/2003   ajn     Updated Copyright notice
11/11/2002   aaj     Included hdrsrchutil.h to avoid warning for disable_roll
11/07/2002   ajn     disable_roll( ) utility used to stop roll events
05/24/2002   aaj     Remove claim from RF lock requested before during init
04/29/2002   aaj     Major overhaul of init. Wait for pending dump in st state
04/25/2002   aaj     Clear on_dump processinging in start state
03/08/2002   ajn     Only deactive HDRDEC clock if RF is owned by HDR
03/04/2002   kss     Deactivate MDSP and HDRDEC to save power.
02/06/2002   ajn     hdrerrno -> errno
01/28/2002   ajn     Abort slow clock estimate on transition to Start state.
12/31/2001   ajn     RF put to sleep in no module is pending after unlock.
12/07/2001   aaj     Don't do power down init if entering from inactive/off.
11/26/2001   aaj     Removed srch_status log packet support
10/24/2001   ajn     Change state header files to common hdrsrchstates.h
10/19/2001   ajn     Handle OFFLINE and STOP signal only in Inactive state.
                     Added a "done" procedure for exiting Inactive state.
09/04/2001   ajn     Cmd interface change.
08/09/2001   ajn     SLEEP VU Integration
06/18/2001   ajn     Unassign fingers on transition back to start state.
06/07/2001   ajn     Added sleep signals.
06/06/2001   ajn     Invalidated channel on transition to start state.
03/30/2001   ajn     Clear signals & release RF on return to inactive state
03/02/2001   aaj     included customer.h file
02/27/2001   ajn     400ms/426.7ms magic numbers to TS.
11/22/2000   ajn     Code review changes
11/08/2000   ajn     Moved SCC/CCC lengths into general search parameters
10/31/2000   ajn     Added General Purpose Signals to State Machine
10/05/2000   ajn     Added power on defaults
07/07/2000   ajn     Created this file

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

#ifdef FEATURE_CMI
#include "sys_v.h"
#endif

#include "time_genoff.h"

#include "hdrdec.h"

#include "hdrsrchtypes.h"
#include "hdrsrchstate.h"
#include "hdrsrchstates.h"
#include "hdrsrchtask.h"
#include "hdrsrchcmd.h"
#include "hdrsrchi.h"
#include "hdrsrchdrv.h"
#include "hdrsrchmsg.h"
#include "hdrsrchutil.h"
#include "hdrsrchfing.h"
#include "hdrsrchrf.h"
#include "hdrsrchrfdiv.h"
#include "hdrsrchlog.h"

#include "task.h"
#include "timer.h"
#include "fws.h"

#include "trm.h"

#include "hdrts.h"

#include "hdrdebug.h"

#include "hdrsrchafc.h"

#include "hdrrx.h"

#include "hdrsrch.h"
#include "hdrmdspmcr.h"

#ifdef FEATURE_HDR_TO_LTE
#include "hdrsrchltemeas.h"
#endif /* FEATURE_HDR_TO_LTE */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
#include "hdrsrchtclte.h"
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

#include "hdrsrchset.h"

#include "hdrsrchmcpm.h"

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_HDR_RUMI
#include "hdrhitmsg.h"
#endif /* FEATURE_HDR_RUMI */

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/



/*==========================================================================

                     STATE DISPATCH TABLE

This section provides the dispatch functions for events that are received
by the task while in this state.

==========================================================================*/

LOCAL void            hdrsrchst_init( hdrsrch_state_enum_type last );
LOCAL void            hdrsrchst_done( hdrsrch_state_enum_type next );
LOCAL void            hdrsrchst_dump( void );
LOCAL errno_enum_type hdrsrchst_cmd ( const hdrsrch_cmd_type *cmd);

LOCAL hdrsrch_state_struct_type hdrsrchst_state =
{
  HDRSRCH_INACTIVE_STATE,
  hdrsrchst_init,
  hdrsrchst_done,
  hdrsrchst_cmd,
  hdrsrchst_dump,
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
  hdrsrch_default_pri_chain_event,
  hdrsrch_process_int_cmd_default,
  hdrsrch_default_process_low_pri_rsp_ind
};


typedef struct
{
  boolean         powerdown_done;
    /* Flag that indicates whether the powerdown sequence is completed  */
}
hdrsrchst_struct_type;


LOCAL hdrsrchst_struct_type    hdrsrchst;


/* <EJECT> */
/*=========================================================================

FUNCTION HDRSRCHST_INIT_START_STATE

DESCRIPTION
  Initializes the START state

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchst_init_inactive_state( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_DEACTIVATING );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  hdrsrch_change_state( &hdrsrchst_state );

} /* hdrsrchst_init_inactive_state( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHST_DUMP

DESCRIPTION
  Start State Pending Search Dump Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrchst_dump( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Pending dump rcvd in inactive State" );

} /* hdrsrch_default_dump( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHST_POWER_ON_DEFAULTS

DESCRIPTION
  This function provides initialization for immediately following Power On.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchst_power_on_defaults( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* TODO: Check default values */


  /*----------------------------------------------------
    Defaults from 6.6.6.5.1.1
      SearchParameters Attribute
  ----------------------------------------------------*/

  hdrsrch.srch_params.pilot_inc        = 4;
    /* Initially, assume all HDR pilots are valid */

  hdrsrch.srch_params.aset_win_size    = HDRSRCH_WINSIZE_60_CHIPS;
  hdrsrch.saved_aset_win_size          = hdrsrch.srch_params.aset_win_size;
  hdrsrch.srch_params.nset_win_size    = HDRSRCH_WINSIZE_100_CHIPS;
  hdrsrch.srch_params.rset_win_size    = HDRSRCH_WINSIZE_100_CHIPS;
    /* Window size codes 8,10,10 */


  /*----------------------------------------------------
    Defaults from 6.6.6.5.1.2
       SetManagementSameChannelParameters Attribute
  ----------------------------------------------------*/

  hdrsrch.same_chan_mgmt_params.pilot_add           = 0x0e;  /* -7.0dB */
  hdrsrch.same_chan_mgmt_params.pilot_compare       = 0x05;  /*  2.5dB */
  hdrsrch.same_chan_mgmt_params.pilot_drop          = 0x12;  /* -9.0dB */
  hdrsrch.same_chan_mgmt_params.pilot_drop_timer    = HDRSRCH_DROPTIMER_4_SEC;
  hdrsrch.same_chan_mgmt_params.dynamic_thresholds  = FALSE;
  hdrsrch.same_chan_mgmt_params.nset_max_age        = 0;


  /*----------------------------------------------------
    Defaults from 6.6.6.5.1.3
       SetManagementDifferentChannelParameters Attribute
  ----------------------------------------------------*/

  hdrsrch.diff_chan_mgmt_params.pilot_add           = 0x0e;  /* -7.0dB */
  hdrsrch.diff_chan_mgmt_params.pilot_compare       = 0x05;  /*  2.5dB */
  hdrsrch.diff_chan_mgmt_params.pilot_drop          = 0x12;  /* -9.0dB */
  hdrsrch.diff_chan_mgmt_params.pilot_drop_timer    = HDRSRCH_DROPTIMER_4_SEC;
  hdrsrch.diff_chan_mgmt_params.dynamic_thresholds  = FALSE;
  hdrsrch.diff_chan_mgmt_params.nset_max_age        = 0;

  hdrsrch.chan_mgmt_1x_params.pilot_add = HDRSRCH_1X_DEFAULT_PILOT_ADD ;
  hdrsrch.chan_mgmt_1x_params.pilot_inc = HDRSRCH_1X_DEFAULT_PILOT_INCREMENT;
    /* set default 1x channel management parameters */

  /*----------------------------------------
    Idle slot monitoring
  ----------------------------------------*/

  hdrsrchsleep_set_control_channel_cycle( HDRSRCH_NO_CCC_HASH, 0 );
    /* No idea what our slot is - can only sleep until next SCC */

  hdrsrchreacq_reset_fpd_activation_status();
    /* Reset forced preamble detection activation status */

  hdrsrchreacq_reset_winsize_fctr();
    /* Reset adjustment window size factor to 1 */

  hdrsrchsleep_reset_last_normal_sleep_sci();
    /* default wake up slot is 9 (5.12 ms) */

} /* hdrsrchst_power_on_defaults( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHST_RELEASE_RF

DESCRIPTION
  This function releases any remaining lock on the RF resources.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchst_release_rf( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  if ( hdrsrchrf_get_chain_held_reason( HDRSRCHRF_PRI_CHAIN ) ==
                                    HDRSRCHRF_IRAT_MEASUREMENT )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "No need to unlock RF" );
    hdrsrchrf_reset_rf_chain_state( HDRSRCHRF_PRI_CHAIN );
      /* clean up our hdrsrchrf state for the RF chain. */

    return;
  }
  
  /* If HDR has a hold of a transceiver, shutdown the decoder and the
     transceivers. */
  if ( hdrsrchrf_has_lock( HDRSRCHRF_PRI_CHAIN ) == TRUE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Unlocking RF" );

  }

  hdrsrchrf_release( HDRSRCHRF_PRI_CHAIN );

} /* hdrsrchst_release_rf( ) */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHST_INIT

DESCRIPTION
  This function provides initialization for the INACTIVE (Start) State.

DEPENDENCIES
  None

PARAMETERS
  last_state  - State just prior to this state

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_HDR_RUMI
hdrsrch_acquire_params_type hdrsrchst_acq_cmd_param_rumi;
volatile unsigned int hdrsrchst_force_acq_cmd = 0;
/* bit 0: force the acq cmd
   bit 1: wait for all tasks init */
#endif /* FEATURE_HDR_RUMI */

LOCAL void hdrsrchst_powerdown( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Search Dump has happened or no searches are in progress */


  HDR_MSG_SRCH( MSG_LEGACY_MED, "Powering down HDR modules" );

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV || defined FEATURE_HDR_DYNAMIC_ACQ_DIV
    hdrsrchrfdiv_diversity_ctrl( FALSE );
      /* Disable diversity when we go to sleep. */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

  hdrsrch_kill_timer( &hdrsrch.lostdump_timer );
    /* No need of lost dump anymore */

  hdrsrch_kill_signal( HDRSRCH_DUMP_SIG  );
    /* No need of dump signal anymore */

  hdrsrch_discard_srch_dump();
    /* Discard the srch dump */

  hdrsrchafc_deactivate( );
    /* We don't need TCXO services anymore so tell TCXO Mgr.  There is no
       need to wait for the release command to go on since whatever TCXO
       mgr commands HDR, HDR will insist on releasing the TCXO. */
  
  if ( hdrsrchrf_has_lock( HDRSRCHRF_PRI_CHAIN ) == TRUE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Unlocking RF" );

    hdrsrchutil_cancel_fpd();
      /* Cancel pending FPD */

    hdrsrchutil_dec_deactivate( );
      /* Disable HDR decoder */
    
    hdrsrchrf_prep_to_sleep();
      /* Prepare for sleep: Stop PDM Loops and Disable PDMs */


    hdrsrchrf_disable_rx( HDRSRCHRF_PRI_CHAIN );
      /* Disable rx */
  }

  hdrmdsp_config_hdr_app( HDRFW_STATE_STANDBY, TRUE, FALSE );
    /* Disable the HDR MDSP Application. */

  if( hdrmdsp_fws_app_is_enabled() == TRUE )
  {
    hdrmdsp_fws_suspend_app( );
      /* Vote to suspend FW if app is enabled */
  }

  hdrsrch_add_to_wait_mask( HDRSRCH_CMD_Q_SIG |
                            TASK_OFFLINE_SIG  | TASK_STOP_SIG );
    /* Wait for comands, Task OFFLINE and STOP signals in this state. */

  hdrsrchdrv_disable_srch4( );
    /* Disable SRCH4 for HDR */

  hdrsrchdrv_release_srch4_mdsp_app();
    /* Release the srch4 mdsp app and permit its removal from mdsp memory. */

  hdrsrchst.powerdown_done = TRUE;
    /* Set the flag to TRUE when powerdown completes               */

#ifdef FEATURE_NPA
  ( void ) hdrsrchutil_npa_cancel_cpu_request( HDRSRCHUTIL_NPA_CPU_WAKEUP );
    /* Cancel CPU request for HDR wakeup. */

  hdrsrchutil_npa_destroy_client();
    /* Release NPA handle including cancelling HDR's NPA requests. */
#endif /* FEATURE_NPA */
#ifdef FEATURE_HDR_JOLOKIA_MODEM_BRINGUP
  hdrsrchutil_reset_max_q6_clk_speed();
#endif /* FEATURE_HDR_JOLOKIA_MODEM_BRINGUP */

  hdrdec_disable_on_arrival_delivery();
    /* Disable on-arrival RX pkt delivery before de-activating SRCH */

  /* Safe to powerdown HDR mode completely.  Make sure to release any IRAT
     grant HDR may hold *before* releasing HDR own TRM grant.  So always maintain
     the same order for the calls to release IRAT grant *first*, then
     release HDR RF. */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
  hdrsrchtclte_release_trm_grant();
    /* Cancel outstanding TRM grant for D2L redirection, if any. */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

  hdrsrchst_release_rf( );
    /* Kill both foreground and background RF locks (if any) */

  if( hdrsrch.deactivate_done_cb != NULL )
  {
    hdrsrch.deactivate_done_cb();    
    hdrsrch.deactivate_done_cb = NULL;
  }

#ifdef FEATURE_HDR_TO_LTE
  hdrsrchltemeas_enter_inactive_state();
    /* Prepare D2L for inactive state */
#endif /* FEATURE_HDR_TO_LTE */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
  if ( hdrsrchtclte_send_deinit_req() == FALSE )
    /* If DO has sent an Init request to LTE to perform LTE pilot measurements,
       make sure DO also sends a Deinit request.  Otherwise, LTE will be left
       in an unexpected state when it is activated.  Since DO is going into
       Inactive state, DO is done with LTE pilot measurements. */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "DLR:hdrsrchmcpm_set_state( HDRSRCHMCPM_STOP_REQ )" );
    hdrsrchmcpm_set_state( HDRSRCHMCPM_STOP_REQ );
      /* Tell MCPM to switch to powerdown clock configuration.  
         This is done here only if DO does not need to send Deinit request to LTE.
         If DO needs to send Deinit request to LTE, DO clocks is shutdown after LTE
         sends Deinit confirmation.  LTE sends the confirmation after it successfully
         shuts down the LTE FW.
         (If DO sends MCPM stop request and LTE has not shut down, DO causes the
         power collapse of the FW processor which does not allow LTE to talk to FW 
         to shut it down.) */
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Deactivate done called during power down");
	  
    if( hdrsrch.deactivate_done_cb != NULL )
    {
      hdrsrch.deactivate_done_cb();    
      hdrsrch.deactivate_done_cb = NULL;
    }
  }
  else
  {
     HDR_MSG_SRCH( MSG_LEGACY_HIGH, "L1 sent DEINIT Request to LTE");
  }

  hdrsrch_sys_loss_done();

#ifdef FEATURE_HDR_RUMI
  if ( hdrsrchst_force_acq_cmd & 0x1 )
  {
    /* Send acq cmd to start acquisition */
    hdrsrchst_acq_cmd_param_rumi.mode = SYS_HDR_ACQ_MODE_FULL;
    hdrsrchst_acq_cmd_param_rumi.channel.band = SYS_BAND_BC0;
    hdrsrchst_acq_cmd_param_rumi.channel.chan_num = 500;
    hdrsrchst_acq_cmd_param_rumi.acq_cb = NULL;
    hdrsrchst_acq_cmd_param_rumi.num_of_pn = 0;
    hdrsrchst_acq_cmd_param_rumi.system_lost_cb = NULL;
    hdrsrchst_acq_cmd_param_rumi.usability_cb = NULL;
  
    hdrsrch_acquire( &hdrsrchst_acq_cmd_param_rumi );
  }
#endif /* FEATURE_HDR_RUMI */
}/* hdrsrchst_powerdown */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHST_INIT

DESCRIPTION
  This function provides initialization for the INACTIVE (Start) State.

DEPENDENCIES
  None

PARAMETERS
  last_state  - State just prior to this state

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchst_init( hdrsrch_state_enum_type last_state )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_default_init( last_state );
    /* Default state initialization - currently just emits a msg */

  //---------------------------------------------------------------------
  // Last State independent initializations ...
  //---------------------------------------------------------------------

  hdrsrchst.powerdown_done = FALSE;
    /* We are about to start powering down HDR module. Set the flag 
       to FALSE                                                           */

  (void) hdrsrchset_grice_drop_gcset();
    /* Clear gRICE CSET */


  hdrsrchutil_disable_roll( );
    /* Uninstall and disable PN Roll processing */

  time_genoff_report_out_of_coverage(ATS_HDR);   
  HDR_MSG_SRCH( MSG_LEGACY_LOW,"HDR: report out of coverage to time service");

  hdrts_invalidate();
    /* If we've lost the HDR system, we don't have valid HDR time-keeping */

  hdrts_sleep();
    /* Ensure demod roll ISR is disabled when HDR clk_regime is disabled.
       Note: Even though HDR is not going to sleep, this function does the
             necessary steps leading up to disabling time stamp services. */

  hdrsrch_clear_on_searcher_idle_proc();
    /* Clear the "on idle processing" since we can not be processing anything
       in start state that were meant to be executed when searcher was idle */

  /* Set wait mask to a known state */

  hdrsrch_add_to_wait_mask( HDRSRCH_WDOG_SIG | HDRSRCH_DUMP_SIG |
                            HDRSRCH_DOG_HB_SIG);
    /* Wait for the HB dog sig, srch internal watchdog signal
       and dump interrupt, if any */

  hdrsrch_kill_signal( HDRSRCH_WAKE_SIG     | HDRSRCH_CMD_Q_SIG
                     | HDRSRCH_GP_SIG0_SIG  | HDRSRCH_GP_SIG1_SIG
                     | HDRSRCH_RXCHIPX16_SIG| HDRSRCH_TIME_VALID_SIG
                     | HDRSRCH_PRI_CHAIN_EVENT_SIG
                     | HDRSRCH_DIV_CHAIN_EVENT_SIG  
                     );
    /* ... and ignore all other events for now */

  hdrsrch_kill_timer( &hdrsrch.timeout_timer );
  hdrsrch_kill_timer( &hdrsrch.schedule_timer );
  
  hdrsrch_kill_timer( &hdrsrch.gp_timer0 );
  hdrsrch_kill_timer( &hdrsrch.gp_timer1 );
  
  hdrsrchrf_cancel_in_progress_tunes();
    /* Cancel any tunes that are in progress so that we can power down
       sooner. */

#ifdef FEATURE_ATOM
  hdrsrchutil_operational_mode_algorithm_init();
    /* Initialization for AT operational mode */
#endif /* FEATURE_ATOM */

  hdrsrchofs_init_ofs_timeline();
    /* OFS timeline initializations */
    
  hdrsrchtc_clear_voip_outage_cb();
    /* initialize VOIP outage callback function to NULL */

  if ( last_state == HDRSRCH_NO_STATE )
  {
    /* Power On initialization */
    hdrsrchst_power_on_defaults( );
      /* Set reasonable default values */
  }
  else if ( last_state != HDRSRCH_SLEEP_STATE  && 
            hdrmdsp_check_dsp_clk() )
  {
    hdrsrchdrv_abort_search( FALSE );
      /* abort pending searches if any */

    hdrsrchdrv_shutdown_proc();
      /* do the shutdown processing. Stops AGC. Ongoing searches will not
           be correct after this */
  }

  hdrsrchfing_set_demod_carrier_mask( 0 );
    /* reset demod carrier mask to 0 */

  if ( ( last_state != HDRSRCH_INACTIVE_STATE ) &&
       ( last_state != HDRSRCH_ACQ_STATE )      &&
       ( last_state != HDRSRCH_NO_STATE )       &&
       ( last_state != HDRSRCH_OFS_MEAS_STATE ) &&
       ( last_state != HDRSRCH_SYNC_STATE )      )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_MED,
                    "Move acset to nset, last state 0x%x",
                    last_state );

#ifndef FEATURE_HDR_REVB
    HDR_MSG_SRCH_2( MSG_LEGACY_MED,
                    "aset.cnt %d cset.cnt %d",
                    hdrsrchset.aset_cnt,
                    hdrsrchset.cset_cnt );
#else
    HDR_MSG_SRCH_2( MSG_LEGACY_MED,
                    "aset.cnt %d cset.cnt %d",
                    hdrsrchset.subaset[0].aset_cnt,
                    hdrsrchset.cset_cnt );
#endif /* FEATURE_HDR_REVB */

    hdrsrchset_move_acset_to_nset();
      /* Move ASET and CSET pilots to NSET */

    hdrsrchutil_start_timer( &hdrsrchlog.oos_timer );
      /* out of service timer needs to be started now, because we just lost 
         service. */

    hdrsrchlog.oos_timer_active = TRUE;
      /* The oos timer is active. We only want to start this timer once we've 
         acquired once. */

  }

  hdrsrch_on_searcher_idle( hdrsrchst_powerdown );
    /* wait until the pending searches, if any, are finished before
       powering down */

  /* TODO: Disable HDR MDSP Application (#48) */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  /* Let MCS know Rx is done */
  hdrmultirat_process_freq_change( RFM_INVALID_DEVICE,
                                   NULL,
                                   HDRMULTIRAT_FORWARD_LINK,
                                   0
                                 );
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

} /* hdrsrchst_init( ) */



/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHST_DONE

DESCRIPTION
  This function provides cleanup for the INACTIVE (Start) State.

DEPENDENCIES
  None

PARAMETERS
  next_state  - State just after this state

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchst_done( hdrsrch_state_enum_type next_state )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SYS_ARG_NOT_USED( next_state );
    /* to avoid the lint error for the ununsed variable */

  hdrsrch_sys_loss_done( );
    /* Mark sys loss done so it won't block any other command, this
       is to safe guard cases deactivate may be ignored */

  if ( next_state ==  HDRSRCH_TIME_SYNC_STATE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "IRAT meas: Need to send MCPM request");
 
    /* Need to enable MDSP App and srch4 driver for LTE to DO time sync
       transition. */

    hdrsrchmcpm_set_state( HDRSRCHMCPM_WAKE_UP_REQ );
    /* Tell MCPM to turn on clocks grouped under wake-up */
    
    hdrts_reset_rtc();
      /* Reset RTC count before enabling FW */

    hdrmdsp_fws_enable_app();
      /* Enable fws hdr app */

    hdrsrchdrv_request_srch4_mdsp_app();
      /* Request use of the search4 app and make sure it is downloaded into
         the firmware. */
  }

#ifdef FEATURE_HDR_RUMI
#ifndef FEATURE_HDR_BOLT_MODEM  
  HWIO_OUT( TDEC_CLK_CTL, 0x3 );
    /* Hack to set HW ctl */

  HWIO_OUT( DEMBACK_EN, 0x1 );
   /* Hack */
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif /* FEATURE_HDR_RUMI */


  hdrsrch_remove_from_wait_mask( TASK_OFFLINE_SIG | TASK_STOP_SIG );
    /* Don't handle these signals in any state but INACTIVE */

#ifdef FEATURE_NPA
  hdrsrchutil_npa_create_sync_client();
    /* Initialize HDR NPA CPU handle. */
#endif /* FEATURE_NPA */

#ifdef FEATURE_HDR_JOLOKIA_MODEM_BRINGUP
  hdrsrchutil_npa_query_max_q6_clk_speed();
#endif /* FEATURE_HDR_JOLOKIA_MODEM_BRINGUP */
  hdrsrchfing_limit_max_fings( FALSE );
    /* Do not limit max fingers */

  hdrsrchrfdiv_user_diversity_pref( hdrsrch.div_ctrl, hdrsrch.div_band_mask );
    /* Use the NV setting for diversity */

#ifdef FEATURE_HDR_TO_LTE
  hdrsrchltemeas_clear_reselect_status();
    /* DO is being re-activated, so clear the LTE reselection in progress
       indicator in case it is set.  LTE reselection must be over at this
       point. */
#endif /* FEATURE_HDR_TO_LTE */
  hdrsrch_reset_no_dump_cnt( );
  /* reset no_dump_cnt when DO is being reactivated */
  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Reset srch dump cnt");
} /* hdrsrchst_done( ) */

/*===========================================================================

FUNCTION HDRSRCHST_ENABLE_FW_HDR_APP_CMD

DESCRIPTION
  This function process ENABLE_FW_HDR_APP command which is used in FTM.

DEPENDENCIES
  None

PARAMETERS
  cmd   - Command to be processed

RETURN VALUE
  None
 
SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchst_enable_fw_hdr_app_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( cmd->params.enable_fw_hr_app.fw_state == HDRSRCH_FW_STATE_ACTIVE )
  {
    /* configure clocks for FW active state */
#ifdef T_MDM9X00A
      /* TBD */
      /* Set to CDMA configuration */
      //HWIO_OUT( MODEM_CHIPXN_CLK_MISC_CTL, 0x6C826200 );
      HWIO_OUT( MODEM_CLK_MISC_CTL, 0x08000400 );
      //HWIO_OUT( MODEM_CLK_MISC_CTL_2, 0x02940442 );
      //HWIO_OUT( MODEM_OFFLINE_CLK_CTL, 0x0049A40A );
#endif /* T_MDM9X00A */

  }

  hdrsrchmcpm_set_state( HDRSRCHMCPM_WAKE_UP_REQ );
    /* Tell MCPM to turn on clocks grouped under wake-up */

  hdrmdsp_fws_enable_app();
    /* Enable fws hdr app */

  hdrmdsp_fws_resume_app( );
    /* Wake up FW is it's been suspended by hdr app */


  hdrmdsp_config_hdr_app( 
       ( hdrfw_state_t )( cmd->params.enable_fw_hr_app.fw_state ),TRUE, FALSE );

  if ( cmd->params.enable_fw_hr_app.enable_fw_hdr_app_cb != NULL )
  {
    cmd->params.enable_fw_hr_app.enable_fw_hdr_app_cb( TRUE );
      /* HDR App is enabled */
  }

}/* hdrsrchst_enable_fw_hdr_app_cmd */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHST_CMD

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

LOCAL errno_enum_type hdrsrchst_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type               status = E_SUCCESS;
    /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "Received cmd %d", cmd->name );

  switch ( cmd->name & HDRSRCHCMD_CMD_MASK )
  {
    case HDRSRCH_ENABLE_FW_HDR_APP:
      /* This command is used only for FTM */
      hdrsrchst_enable_fw_hdr_app_cmd( cmd );
      break;

    case HDRSRCH_DEACTIVATE_CMD:

      /* The decactivation callback is intended to be called after 
         "hdrsrchst_powerdown" is called. The latter is only called
         when HDR searcher is idle. So before calling the callback,
         we check if "hdrsrchst_powerdown" is pending or not     */
      if ( hdrsrchst.powerdown_done == TRUE 
#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
	  	&& (hdrsrchtclte_lte_deinit_is_finished() == TRUE)
#endif
	  	)
      {
        if ( hdrsrch.deactivate_done_cb  != NULL )
        {
          hdrsrch.deactivate_done_cb();        
          hdrsrch.deactivate_done_cb = NULL;
            /* Call the deactivation callback function when we receive the
               deactivation command in inactive state. This command was 
               ignored before */
        }                             
      }
      else
      {
#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
        HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Ignore DEACTIVATE CMD from INACTIVE %d %d",
			hdrsrchst.powerdown_done,hdrsrchtclte_lte_deinit_is_finished());  
#else
        HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Ignore DEACTIVATE CMD from INACTIVE %d",
			hdrsrchst.powerdown_done);  
#endif
      }
      hdrsrch_sys_loss_done( );

      break;

    case HDRSRCH_ACQUIRE_CMD:
#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
      if( hdrsrchtclte_lte_deinit_is_finished() == TRUE )
        /* Finish LTE Deinit operation before leaving Inactive state.  Clocks
           and FW resource have not been properly turned off until LTE Deinit
           operation is finished. */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
      {
#ifdef FEATURE_HDR_RUMI
        if ( hdrsrchst_force_acq_cmd & 0x2 )
        {
          if ( rcevt_wait_name("PS: READY") == RCEVT_NULL )
          {
            HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Unexpected Run Control Event Notification err" );
          }
        }
#endif /* FEATURE_HDR_RUMI */

        hdrsrchacq_init_acq_state();
      }
      status = E_AGAIN;
        /* We cannot handle the acquire command parameters here.
           Redo the command again once we enter ACQ state. */
      break;

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
    case  HDRSRCH_TRAN_SYS_TIME_REQ:
      hdrsrchtimesync_init_timesync_state();
      status = E_AGAIN;
        /* We don't handle time transfer in init state. We will handle
           it when we are in TimeSync state  */
      break;
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

#ifdef FEATURE_HDR_RUMI
     case HDRSRCH_TC_ASSIGNMENT_CMD:
       if ( hdrhit_rumi_cfg.tx_only_support )
       {
         HDR_MSG_SRCH( MSG_LEGACY_HIGH, "RUMI TX only: tc_assign_cb" ); 
         (cmd->params.tc_assign.tc_assign_cb)( HDRSRCH_TCA_SUCCEEDED );
       }
       break;
#endif /* FEATURE_HDR_RUMI */

    default:
      status = hdrsrch_default_cmd( cmd );
        /* Handle state-independent commands */

  } /* switch (cmd->name) */

  return status;

} /* hdrsrchst_cmd(cmd) */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION

/* =============================================================================
 
FUNCTION HDRSRCHST_DEACTIVATE_DONE_NOTIFY
 
DESCRIPTION
  This function notifies L3, that L1 is finally done with deactivation. The
  delay is due to D2L redirection IRAT, where L1 has to wait for LTE to send
  the DEINIT cnf before L3 can continue with redirection procedure.
 
DEPENDENCIES
  None
 
PARAMETERS
  None
 
RETURN VALUE
  None
 
SIDE EFFECTS
  None
 
===============================================================================*/
void hdrsrchst_deactivate_done_notify( void )
{
 
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Deactivate done cb called after receiving LTE DEINIT cnf");
  if( hdrsrch.deactivate_done_cb != NULL )
    /* Notify L3 that deactivation is done */
  {
    hdrsrch.deactivate_done_cb();    
    hdrsrch.deactivate_done_cb = NULL;
  }        
 
} /* end hdrsrchst_deactivate_done_notify */

#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
