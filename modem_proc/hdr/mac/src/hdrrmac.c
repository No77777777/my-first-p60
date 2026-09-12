/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

       H D R   R E V E R S E   T R A F F I C   C H A N N E L   M A C

                             P R O T O C O L

GENERAL DESCRIPTION
  This module contains the implementation of the HDR Reverse Traffic Channel
  Media Access Control (MAC) Protocol for an HDR access terminal.  
  The RTC MAC interface, for all supported subtypes, is in this file.

EXTERNALIZED FUNCTIONS
  hdrrmac_init
    Initialize the Reverse Traffic Channel MAC protocol.

  hdrrmac_activate
    Activate the Reverse Traffic Channel MAC protocol.
    Power up RF Tx hardware.

  hdrrmac_deactivate
    Deactivate the Reverse Traffic Channel MAC protocol.
    Turn PA off (if necessary), turn RF Tx off (if necessary).

  hdrrmac_is_active
    Determine if protocol is active.

  hdrrmac_start_tx
    Start TX after it has been stopped or after an activate command.
    If RF warmup is complete, turn PA on, start data.

  hdrrmac_stop_tx
    Stop (suspend) transmission.
    Turn PA off, send HDRIND_RTCMAC_TX_STOPPED, stop asking for data.

  hdrrmac_powerdown_tx
    Turns off RF TX hardware while RTC MAC is active.

  hdrrmac_powerup_tx
    Turns on RF Tx hardware that was turned off with hdrrmac_powerdown_tx().

  hdrrmac_suspend_connection
    Suspends a connection -- no more data is requested for tranmission over
    the reverse link.

  hdrrmac_resume_connection
    Resumes a connection that has been suspended. Turn PA, request data.

  hdrrmac_shutdown
    Give HDRIND_RTCMAC_SHUTDOWN_COMPLETE indication at the completion of
    the current frame, then cease data transmission (no more data is
    requested from PCP).

  hdrrmac_process_cmd
    Process Forward Reverse Channel MAC commands from the task command queue.

  hdrrmac_process_sig
    Process Forward Reverse Channel MAC signals received at task level.

  hdrrmac_process_msg
    Process received Reverse Traffic Channel MAC messages.

  hdrrmac_process_timer
    Process the timer expiry events for Reverse Traffic Channel MAC protocol.

  hdrrmac_msg_cb
    Callback function that queues received Reverse Traffic Channel MAC
    messages onto the task message queue.

  hdrrmac_start_flow_control - to be removed (use hdrrmac_flow_control)
    Enables RL flow control.  This disables data on non-SLP carriers in RevB.

  hdrrmac_stop_flow_control  - to be removed (use hdrrmac_flow_control)
    Disables RL flow control.
  
  hdrrmac_flow_control
    Enable/Disable RL flow control.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  The protocol must be initialized after system startup prior to activating
  the protocol.

  Copyright (c) 2000 - 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrrmac.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/22/18   pga     Fix KW P1 issues
04/15/16   rmv     Request SVS instead of Low SVS Q6 clk for FBRx optimization
11/16/15   rmv     Fixed crash in NS mode (FTM)
10/29/15   vko     Added debug code for HDRMC stuck waiting for 
                   RMAC deactivate
10/12/15   vke     Changes to complete pending shutdown 
                   or deactivate cmd execution
07/13/15   arm     Mainlined FEATURE_RF_SVDO_API feature.
07/10/15   bb      Changes to remove MCVS vote while starting TX
06/18/15   tnp     Fix compiler warnings
06/09/15   vke     Changes to complete pending deactivate cmd execution
05/11/15   wsh     Added support for RxTx Split and TRM grouped API
02/25/15   wsh     Replaced efs API with mcfg version
01/31/15   vke     Added API to check silence frame
12/10/14   vke     Changes to ignore deactivate request when rmac is 
                   already inactive
10/22/14   ljl     Fixed compiler warning. 
10/21/14   vke     Changes to correct the connection close status 
                   shared to HDR SRCH
10/03/14   arm     HDR QTA: check if TX is on before starting QTA
09/22/14   arm     HDR QTA code cleanup
09/09/14   vke     Redesign fix for connection close during QTA gap
09/02/14   sat     Fixing Klocwork issues
08/22/14   vke     Delay connection close during QTA gap
08/12/14   wsh     Disable RTT in TAP call; force HiCap if boost, new threshold
07/31/14   vko     Rectified the RevB compilation error, when disabled for 8909.
07/29/14   tnp     FR22375: Fix Medium compiler warnings in HDR
07/09/14   cnx     Support NV refresh without reset.
06/04/14   arm     Changes to support DO to G QTA
05/19/14   wsh     Cancel prev. RevLinkCarrier override in next TCA
11/14/13   arm     Enabled tDTX by default.
03/14/14   dsp     FR19338:Reduce call setup time by skipping TX RF shutdown.
01/14/14   kss     Fixed issue where rfm_hdr_set_pa_access_probe_config()
                   was sent twice.
11/27/13   arm     Consolidated Flexconn, sDTX, tDTX and nDTX into one log
                   packet and removed FW specific values.
12/16/13   sat     Changed memcpy() to assignment or memscpy() (safer version).
11/25/13   rmg     Added changes to reduce idle subframes before tuneaway.
09/23/13   rmg     Removed skipping of DAC and RF tx disable during retune.
10/02/13   rmg     Added support for RL rate estimate and RLP queue info for
                   modem API.
12/04/13   sat     Removed unused parameters in F3s.
10/29/13   sat     Reduced F3 messages (or/and) ASSERTs
08/06/13   rmg     Added support to skip DAC and RF tx disable during retune.
07/17/13   arm     Enabled device only dtx for DSDA based on band-chan 
                   combinations.
07/17/13   arm     Enabled flexconn for DSDA.
07/10/13   arm     Supported traffic DTX.
07/01/13   smd     Fixed a compile error. 
05/15/13   rmv     Added updates for Cost of Modem Power API support 
06/05/13   kss     Added voting for 144 MHz clk during RL connection.
05/23/13   smd     Fixed a compile error. 
02/17/13   arm     Supported network dtx and device only dtx co-existence.
04/29/13   smd     Disable RevB code if FEATURE_HDR_REVB_SUPPORTED is not defined.
04/22/13   arm     Added changes for DSDA coexistence manager. 
04/10/13   smd     Enabled DTX/FC by default (for TR PLs).
04/10/13   smd     Enabled DTX/FC by default (for DI PLs).
02/25/13   dsp     Removed the redundant check for FTM mode before 
                   calling hdrrmac_powerup_init()
02/20/12   smd     Handled the case of HHO during DRC supervision for DTX/FC.
02/11/13   vko     Fixed compiler warnings
12/19/12   smd     Don't call powerup_init() twice in FTM mode.
02/14/13   smd     Removed customer reference. 
12/09/12   smd     Used AuxDRCCover to detect CSM6850 for DTX. 
01/17/13   rmg     Removed references to feature HDRMOD_PA_UNDER_FW_CONTROL.
01/11/13   smd     Included hdrsrchmac.h.
01/05/13   smd     Flexconn and dtx NV disabled by default for DIME.
11/01/12   smd     Disabled FlexConn and DTX in TAP call.
10/31/12   smd     Enabled FlexConn and DTX by default for 4.1 build. 
10/10/12   smd     Added fix of enabling FlexConn mode one dsc length after
                   FlexConn being disabled.
10/10/12   smd     Fixed fc/dtx control for RevB DRC supervision timeout.
10/05/12   cnx     Corrected parsing of OLP message.
10/03/12   rmg     Corrected tx power initialization during optimized handoff.
09/18/12   smd     Flexconn and dtx NV disabled by default.
09/05/12   smd     Fixes for FlexConn and DTX.
08/23/12   smd     Added updates for Device only DTX and FlexCONN.
07/27/12   rmg     Added enhanced PA backoff support.
06/20/12   smd     Supported Device only DTX and FlexCONN.
06/21/12   vlc     Check for slot interrupt processing when suspending and 
                   resuming traffic state.  
06/20/12   kss     Don't fully process resume if RMAC is stopped.  
04/30/12   dsp     Fixed Compiler warnings on Dime. 
03/20/12   cnx     Resolved TCB compiler errors for Dime. 
03/19/12   cnx     Resolved TCB compiler errors for Dime. 
02/14/12   wsh     Added support for SVLTE+SHDR
02/07/12   arm     Fixed thermal mitigation bug, check if HDR is active 
                   before queing any command. 
02/09/12   rmg     Enabled RevB flow control support.
01/04/12   rmg     Shifted fw msg receiving code from hdrrmac to hdrtx.
11/29/11   kss     Fixed compiler warning.
11/22/11   kss     Execute all of resume_connection command as single command.
11/16/11   rmg     Removed fast connect workaround.
10/27/11   kss     Added support for LTE to DO optimized handoff.
10/21/11   kss     Initialize tx initial power adjust.
09/26/11   grl     Removed use of obsolete AEEstd.h
09/16/11   wsh     Reduce heap mem usage by moving memory back to static
08/08/11   rmg     Added changes to process suspend/resume in HDRTX context.
08/04/11   ssu     Reference ftm_mode using ftm_get_mode().       
08/04/11   ssu     Replace hdr_ftmi_xx with hdrhitmsg_xx using hdrhitmsg APIs.
06/21/11   rmg     Added support for updated hdrmod_tx_start/stop().
04/26/11   ljl     Added tx powerdown callback.
04/26/11   kss     Added KW fix.
03/22/11   arm     Added support for thermal pa backoff feature. 
03/23/11   kss     Adjusted RF includes for NikeL RF.
02/09/11   rmg     Shifted dipswitch callbacks to powerup_init. 
02/01/11   rmg     Removed references to deprecated clk APIs.
01/21/11   rmg     Supported RF API changes for NikeL.
10/15/10   pxu     Replaced HDRMC_FEATURE_HDR_RL_RATE_INERTIA #ifdef 
                   macros with dynamic function checks.
10/07/10   kss     Fixed race condition by calling RF tx enable in Tx context
06/21/10   cnx     Fixed compiler warnings.
06/10/10   rmg     Added hdrrmac_flow_control to support thermal flow control.
05/18/10   rmg     Shifted DRC supervision timer management to FMAC: added 
                   APIs hdrrmac_enter/exit_drc_supervision().
04/07/10   rmg     Obtained RMAC subtype in powerup_init().
                   Reverted the change to obtain subtype in get_flow_mapping().
04/07/10   rmg     Executed powerup init for selected subtype in FTM mode.
12/01/09   rmg     Added support to call a cb once TCA is processed by MC RMAC.
02/18/10   rmg     Removed call to hdrrmac4_start/stop_flow_control() till 
                   RMAC4 changes are merged.
01/26/10   kss     Removed use of RF Tx bring up functions.
01/26/10   wsh     Merged ZI memory reduction to main branch
12/17/09   etv     Cleaned up throttling interface.
12/11/09   rmg     Fixed MAC flow ID initialization for RMAC subtype 1.
12/07/09   kss     Temp fixes for MDM9K bringup.
11/11/09   etv     Initialize RMAC MSGR module only in online mode.
10/07/09   kss     Include AEEstd.h for offset macro.
09/22/09   rmg     Added support for CPU based RL flow control.
07/17/09   etv     Added support for receiving fw messages.
03/11/09   kss     Add Rev B support for hdrrmac_get_current_tx_packet_info().
12/11/08   kss     Removed FEATURE_HDR_REVB_RMAC_7500.
12/04/08   rmg     Removed intlocks.
12/04/08   rmg     Lint fixes.
10/26/08   rmg     Added support for RevB FTM.
07/09/08   etv     Moved initialization of pointer to common info from 
                   hdrrmac0_powerup_init to hdrrmac0_init.
12/10/07   kss     Change to support new RF API for access/traffic.
12/18/07   kss     Track nom. power and turnaround.
11/26/07   cws     Added support for hdrrmac0_set_target_ps_index().
11/30/07   cc      Moved hdrrmac_common pointer initialization from _init
                   to powerup_init function.
11/02/07   cc      Added 3x unit test support on 7500.
10/29/07   cc      Added subtype 3 set_target_ps_index & get_selected_ps_index support.
10/15/07   cc      Only retrieve the InUse attributes for the InUse subtype
                   during power up initialization.
09/21/07   kss     Include file cleanup.
09/17/07   cc      Fixed type of return variable of hdrrmac_get_selected_ps_index.
09/13/07   cc      Added hdrrmac_get_flow_mapping(), hdrrmac_get_changed_flow_mapping(),
                   & hdrrmac_is_flow_active() implementation.
09/05/07   etv     Fixed high lint warnings.
08/23/07   kss     Added Rev A support to hdrrmac_get_current_tx_packet_info().
08/23/07   kss     Added hdrrmac_get_current_tx_packet_info() function.
08/08/07   cc      DOrB update; running rmac4 over rmac3drv.
07/30/07   cc      Added protocol timer support.
07/17/07   cc      Sanity checking against input parameters of update_rl_carriers().
07/12/07   cc      Added hdrrmac_update_rl_carriers() & 
                   hdrrmac_get_slp_carrier_and_tx_pilot_pwr_info support for
                   MSM7500 Rev. B integration.
06/19/07   cc      Initial DOrB support.
03/03/07   rmg     Added overhead channel gains compensation
01/17/06   rmg     Added stopwatch, measurement of hard-handoff time 
10/27/06   kss     Make sure RTC MAC is active before attempting to resume. 
08/22/06   kss     Added hdrrmac_notify_suspend().
06/22/06   mpa     Updated FEATURE_HDR_RL_RATE_INERTIA featurization.
06/08/06   mpa     Fixed relA featurization.
04/13/06   kss     Call hdrrmac3_update_throttling() function for Rev A.
02/02/06   etv     Subtype check before AT-init proposal.
01/31/06   etv     Added support for AT-init Neg. for RMAC3.
12/12/05   etv     Removed hdrrmacc header file inclusion.
12/07/05   kss     Added missing break to case statement.
12/05/05   vm      Added FTM support. 
11/18/05   kss     Corrected REVA featurization problem.
11/04/05   etv     Reading the subtype from SCP in process_ind routine.
10/28/05   kss     Enable hdrrmac3_suspend/resume.
10/18/05   kss     Added reset of Rev A rate count; 
                   decouple rate inertia from subtype.
10/13/05   kss     Added hdrrmac_config_early_termination() function.
09/26/05   sr      Call SCP function to get current protocol subtype in
                   hdrrmac_set_fixed_rri()
09/09/05   kss     Removed references to hdrrmacdrv.h
09/06/05   kss     Updated hdrrmac_set_fixed_rri() to return boolean.
08/08/05   kss     Track start_tx and stop_tx calls; use changed hdrtx 
                   interface for enabling/disabling RF Tx.
08/02/05   kss     Ignore hdrrmac_start_tx() if RTC is suspended.
07/08/05   sr      Fixed featurization in hdrrmac_dm_reset_rtc_rate_count()
07/08/05   sr      Merged in from hdr_reva_6800sb sandbox
05/26/05   sr      Changed hdrpcp_get_config_subtype() to 
                   hdrpcp_get_current_subtype() in ...process_activate_cmd()
05/05/05   kss     File is now the RTCMAC generic interface. Rev 0 
                   functionality is now in hdrrmac0.c.
02/16/05   kss     Use macros for logging reverse link channel gains.
02/14/05   kss     Inform RF of rate changes to optimize PA switchpoints.
02/10/05   kss     Fixed overflow issue in Reverse Silence Interval computation
01/25/05   etv     Cleaned lint errors. 
01/24/05   kss     Always give TX_STOPPED ind. as response to stop_tx cmd.
01/12/05   kss     Added hdrrmac_disable_throttling() support.
01/04/05   etv     Added support for logging RTC physical layer packets. 
11/29/04   grl     Added support for connected state stat logging.
11/22/04   kss     Account for max rate=0 in hdrrmac_pa_is_available().
11/04/04   mpa     Added support for RMAC subtype 1 (GAUP)
10/20/04   kss     Added GAUP support.
10/20/04   kss     Use hdrmod interface to enable/disable frame interrupt.
01/08/04   kss     Disable data requests when entering DRC supervision;
                   include state parameter in indication event.
11/14/03   kss     Added FEATURE_HDR_RL_HYBRID_ENHANCEMENT.
                   Added suspend time parameter to hdrrmac_resume_connection().
11/03/03   kss     Remove redundant RPC enable from setup_state code (RPC is
                   enabled when PA is turned on).
10/21/03   kss     Include rf.h instead of rfhdr.h.
10/14/03   kss     Workaround to avoid the one slot deadline for loading data
                   given by the encoder. A DSP slot timer is used to delay the
                   encoder load until after the slot deadline has passed.
09/10/03   mpa     Converted F3 messages to use MSG2.0 SSID
09/08/03   kss     Write 0 rri on a data -> no data transition.
07/30/03   kss     Added MSM6500 support.
06/19/03   kss     (mpa) Give RTCMAC.ShutdownComplete indication on deactivate
                   if it was waited for.
05/15/03   kss     Fixed another small race condition with 1x tuneaway and a
                   call end.
05/01/03   kss     Corrected mask for FTM duty cycle.
04/04/03   kss     Reset data_allowed (boolean) on entry to setup state.
03/18/03   kss     Added hdrrmac_enable/disable_rate_inertia() interface.
03/06/03   kss     Added hdrrmac_pa_is_available() query function..
02/05/03   kss     Fixed RRI stat count in FTM mode.
01/20/03   kss     Check that PCP returns the correct amount of data;
                   process shutdown command if PA is in ON_PENDING state.
01/11/03   kss     Move data offset (gain) loading to DSP so it can be
                   triggered by TCA during traffic.
12/09/02   kss     Load encoder immediately after receiving data to reduce
                   latency.
12/03/02   vas     When resuming transmission from suspended, only if the RF
                   Status is not ON, then set the RMAC to start pending.
11/19/02   mpa     Use new dip switch interface for RPC and Throttling.
11/11/02   kss     Added IRAM support on function-by-function basis.
11/08/02   kss     Don't turn PA off after a suspend command; simply stop
                   asking for data. Also use RAB and probabilities for FTAP.
10/27/02   kss     Added support for logging Power Control pkt in FTM mode.
10/04/02   kss     Set curr_rate to zero after tuneaway; fix race condition
                   that could occur if a tuneaway immediately followed a
                   shutdown command.
08/20/02   kss     Synchronized deactivate commands with hdrmc task to avoid
                   race condition; significant chagnes to both fix some bugs
                   and make some enhancements to the reverse silence interval
                   handling.
07/23/02   om      Fixed accouting of RTAP pkts for non-configured rate mode.
07/18/02   kss     Added support for new synchronous tx logging.
07/15/02   kss     Restore frame interrupt after tx shutdown.
07/08/02   kss     Added mechanism to detect when the upper layers take too
                   long to get data and miss the encoder window; fix a timing
                   window in hdrrmac_deactivate which could result in the ISR
                   never being uninstalled.
06/05/02   kss     Fixed problem that occured when hdrrmac_start_tx() was called
                   before RF Tx enable was complete (removed timing dependency).
05/06/02   om      Changed for configured rate mode of RTAP
04/01/02   kss     Don't use DSP max rri rate when throttling is disabled.
02/22/02   kss     Turn off preamble detection when PA is off; completed
                   REV_PCTL packet logging.
01/16/02   kss     Don't turn RF off on deactivate if it has already been
                   turned off by hdrtx_powerdown_tx().
01/15/02   kss     Log max frame size recommended by DSP even if it is zero.
01/07/02   kss     Added hdrrmac_powerdown_tx, powerup_tx, suspend_connection
                   and resume_connection functions for hybrid support in
                   the connected state.  Removed AP msg check on activation.
12/12/01   kss     Disable RPC during DRC supervision and Reverse Silence
                   Interval. Also, don't ever disable DRC supervision.
12/07/01   kss     Fixed problem where DRC restart timer was not being reset.
12/03/01   kss     Added HDR_REV_PCTRL logging; added hdrrmac_shutdown()
                   command to cleanup RF prior to deactivation.
11/28/01   kss     Added support for HDR max power protection via DIP11.
11/07/01   kss     Added fixed RRI support for testing; added RPC
                   enable/disable dip switch support.
10/29/01   mpa     Added hdrrmac_reset_dm_rtc_rate_count().
10/15/01   kss     Remove maxHwRate workaround (DSP fixed).
10/10/01   kss     Uninstall ISR when deactivated in setup state.
10/08/01   kss     Added support for rate limit of zero; scaled random numbers
                   to 255; only compute silence duration if RTC MAC is active;
                   set current_rate to zero when in reverse silence interval.
09/27/01   kss     Fixed problem which prevented RMAC from deactivating
                   properly with DRC supervision timer going.
09/21/01   kss     Added test app support; improved workaround for maxHwRate.
09/21/01   kss     Distribute random numbers between 0 and 10000 for logs.
09/20/01   kss     Added support for Reverse Silence Interval.
09/12/01   kss     Added maxDRC setting.
09/07/01   kss     Added hdrrmac_set_silence_params() function.
09/05/01   dna     Added separate handling for in-use vs. in_config messages.
08/27/01   kss     Split hdrrmac_powerup_init() into two functions.
08/24/01   kss     DRC supervision support; session config; DDR 51.
07/10/01   kss     Added support for the broadcast rate limit; checking the
                   Tx AGC for max RRI; retrieving session parameters from
                   NV.
06/25/01   kss     Correctly initialize RPC step size.
06/15/01   kss     Added logging support, fixed rate determination mechanism.
06/08/01   kss     Enable DRC in DSP.
06/06/01   dna     new param for hmp_send_msg()
06/05/01   kss     Correctly sets RRIs higher than 1.
06/04/01   kss     RTC is not setup until START_TX is called.
05/08/01   kss     Added several TRACE macros; initialize configuration data.
05/07/01   kss     IS-856 functional changes.
03/23/01   kss     Fixed bug where PA would not turn off on a setup->inactive
                   state transition.
02/13/01   kss     Restructured to fix problem where it was taking too long
                   to retrieve data from upper layers.
01/26/01   dna     HDRRMAC_OPEN_STATE_ISR_SLOT_OFFSET = 2 (instead of 12)
12/07/00   kss     Added hdrrmac_start_tx() and _stop_tx() functionality.
11/03/00   dna     Added HDR event logging
03/30/00   st      Created.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/* General include files */
#include "hdr_variation.h"
#include "comdef.h"
#include "err.h"
#include "hdrdebug.h"
#include "rex.h"
#include "time_svc.h"
#include "qw.h"
#include "rfm_hdr.h" 
#include "mcpm_api.h"
#include "fs_lib.h"

#ifdef FEATURE_MEMCPY_REMOVAL
#include "stringl.h"
#endif /* FEATURE_MEMCPY_REMOVAL */

/* HDR protocol-related include files */
#include "hdrhai.h"
#include "hdrind.h"
#include "hdrmci.h"
#include "hdrscp.h"
#include "hdramac.h"
#include "hdrrmac.h"
#include "hdrrmaci.h"
#include "hdrmod.h"
#include "hdrsrch.h"
#include "hdrsrchrf.h"
#include "hdrts.h"

/* Other HDR-specific include files */
#include "hdrtrace.h"
#include "hdrrmaclog.h"
#include "hdrlog.h"
#include "hdrmdspmcr.h"

/* RTCMAC Subtype files */
#include "hdrrmac0.h"
#ifdef FEATURE_HDR_REVA_L1
#include "hdrrmac3.h"
#include "hdrrmac3drv.h"
#endif /* FEATURE_HDR_REVA_L1 */
#ifdef FEATURE_HDR_REVB_SUPPORTED
#include "hdrrmac4.h"
#include "hdrrmac4drv.h"
#endif /* FEATURE_HDR_REVB_SUPPORTED */

#ifdef FEATURE_FACTORY_TESTMODE
#include "ftm.h"
#include "hdrhitmsg.h"
#endif

#include "hdrmsg.h"
#include "hdrsrchmac.h"

#ifdef FEATURE_LTE_TO_HDR_OH
#include "hdrsrchtypes.h"
#endif /* FEATURE_LTE_TO_HDR_OH */

#include "hdrfmac.h"
#include "hdrmdspmcr.h"

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
#include "hdrmultirat.h"
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

#include "hdrutil.h"

/*===========================================================================

                LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

#define HDRSCP_SUBTYPE_3_RTCMAC 3
#define HDRSCP_SUBTYPE_1_RTCMAC HDRSCP_SUB1_RMAC
#define HDRSCP_DEFAULT_RTCMAC   HDRSCP_DEFAULT_RMAC
#ifdef FEATURE_HDR_REVB_SUPPORTED
#define HDRSCP_SUBTYPE_4_RTCMAC 4
#endif /* FEATURE_HDR_REVB_SUPPORTED */

#define HDRRMAC_SANITY_TIMER_MS 7000
/* sanity timer of 7sec to detect RMAC not deactivating */
/*
 * Define this to enable 3 carriers unit test on 7500.
 * Not needed for rmac4 single carrier unit test.
 */
#undef FEATURE_HDR_RMAC4_3X_UNIT_TEST

#ifdef FEATURE_HDR_THERMAL_BACKOFF

/* Forward declaration for call back functions */

void hdrrmac_configure_thermal_pa_backoff_cb (void *params);
void hdrrmac_control_thermal_pa_backoff_cb (ds707_ccm_flow_control_cmd_enum_type cmd);
void hdrrmac_thermal_power_bo_step_timer_cb( uint32 timer_id );
static void hdrrmac_set_thermal_power_non_bo_timer_cb( uint32 timer_id);
static void hdrrmac_set_thermal_power_bo_timer_cb(uint32 timer_id );
void hdrrmac_set_thermal_power_bo_pattern
( hdrrmac_thermal_backoff_pattern_enum_type pattern );


/* Tx backoff period */
#define HDRRMAC_THERMAL_DEFAULT_BO_PERIOD_MS 1200

static int hdr_thermal_power_bo_period_ms = HDRRMAC_THERMAL_DEFAULT_BO_PERIOD_MS;

#define HDRRMAC_THERMAL_POWER_BO_PATTERN_INC( pattern_num, max_pattern_num )            \
          (((max_pattern_num) < ((pattern_num) + (1)))? (max_pattern_num): ((pattern_num) + (1)))

#define HDRRMAC_THERMAL_POWER_BO_PATTERN_DEC( pattern_num, min_pattern_num )            \
          (((min_pattern_num) > ((pattern_num) - (1)))? (min_pattern_num): ((pattern_num) - (1)))

#endif /* FEATURE_HDR_THERMAL_BACKOFF*/

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW

#define HDRRMAC_IS_DSDA_ENABLED           \
       ( ( hdrmultirat_get_mode_pref() == HDRMULTIRAT_DUAL_SIM_DUAL_ACTIVE ) ? \
          TRUE : FALSE )
#else

#define HDRRMAC_IS_DSDA_ENABLED     FALSE

#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

#ifdef FEATURE_LTE_TO_HDR_OH
/*--------------------------------------------------------------------------
               HRPDOpenLoopParameters UNPACKING MACROS
--------------------------------------------------------------------------*/
#define HDRRMAC_OLP_MSG_ID_SIZE  8
#define HDRRMAC_OLP_NUM_PILOTS_SIZE 4
#define HDRRMAC_OLP_PILOT_PN_SIZE 9

#define HDRRMAC_OLP_NUM_REV_CDMA_CHANNELS_SIZE  4
#define HDRRMAC_OLP_REV_SYSTEM_TYPE_SIZE 8
#define HDRRMAC_OLP_REV_CDMA_BAND_SIZE  5
#define HDRRMAC_OLP_REV_CDMA_CHANNEL_SIZE 11
#define HDRRMAC_OLP_PILOT_INCL_SIZE 1

#define HDRRMAC_OLP_OPEN_LOOP_ADJUST_SIZE  8
#define HDRRMAC_OLP_INITIAL_ADJUST_SIZE  5
#define HDRRMAC_OLP_PILOT_STRENGTH_INCL_SIZE  1
#define HDRRMAC_OLP_PILOT_STRENGTH_NOMINAL_SIZE  3
#define HDRRMAC_OLP_PILOT_STRENGTH_CORRECTION_MIN_SIZE  3
#define HDRRMAC_OLP_PILOT_STRENGTH_CORRECTION_MIN_SIZE  3

/* NumPilots */
#define HDRRMAC_OLP_NUM_PILOTS( ptr, val_ptr, offset )  \
         HDRBIT_UNPACK8(ptr, val_ptr, offset, HDRRMAC_OLP_NUM_PILOTS_SIZE)

/* PilotPN */
#define HDRRMAC_OLP_PILOT_PN( ptr, val_ptr, offset )  \
         HDRBIT_UNPACK16(ptr, val_ptr, offset, HDRRMAC_OLP_PILOT_PN_SIZE)

/* NumReverseCDMAChannels */
#define HDRRMAC_OLP_NUM_REV_CDMA_CHANNELS( ptr, val_ptr, offset )  \
         HDRBIT_UNPACK8(ptr, val_ptr, offset,  HDRRMAC_OLP_NUM_REV_CDMA_CHANNELS_SIZE)

/* ReverseCDMAChannel */
#define HDRRMAC_OLP_REV_CDMA_SYS_TYPE( ptr, val_ptr, offset ) \
         HDRBIT_UNPACK8(ptr, val_ptr, offset,  HDRRMAC_OLP_REV_SYSTEM_TYPE_SIZE)

#define HDRRMAC_OLP_REV_CDMA_BAND( ptr, val_ptr, offset ) \
         HDRBIT_UNPACK8(ptr, val_ptr, offset,  HDRRMAC_OLP_REV_CDMA_BAND_SIZE)

#define HDRRMAC_OLP_REV_CDMA_CHANNEL( ptr, val_ptr, offset ) \
         HDRBIT_UNPACK16(ptr, val_ptr, offset,  HDRRMAC_OLP_REV_CDMA_CHANNEL_SIZE)

/* PilotIncluded */
#define HDRRMAC_OLP_PILOT_INCL( ptr, val_ptr, offset )  \
         HDRBIT_UNPACK8(ptr, val_ptr, offset,  HDRRMAC_OLP_PILOT_INCL_SIZE)

/* OpenLoopAdjust */
#define HDRRMAC_OLP_OPEN_LOOP_ADJUST( ptr, val_ptr, offset )  \
         HDRBIT_UNPACK8(ptr, val_ptr, offset,  HDRRMAC_OLP_OPEN_LOOP_ADJUST_SIZE)

/* InitialAdjust */
#define HDRRMAC_OLP_INITIAL_ADJUST( ptr, val_ptr, offset )  \
         HDRBIT_UNPACK8(ptr, val_ptr, offset,  HDRRMAC_OLP_INITIAL_ADJUST_SIZE)

/* PilotStrengthIncluded */
#define HDRRMAC_OLP_PILOT_STRENGTH_INCL( ptr, val_ptr, offset )  \
         HDRBIT_UNPACK8(ptr, val_ptr, offset,  HDRRMAC_OLP_PILOT_STRENGTH_INCL_SIZE)

/* PilotStrengthNominal */
#define HDRRMAC_OLP_PILOT_STRENGTH_NOMINAL( ptr, val_ptr, offset )  \
         HDRBIT_UNPACK8(ptr, val_ptr, offset,  HDRRMAC_OLP_PILOT_STRENGTH_NOMINAL_SIZE)

/* PilotStrengthCorrectionMin */
#define HDRRMAC_OLP_PILOT_STRENGTH_CORRECTION_MIN( ptr, val_ptr, offset )  \
         HDRBIT_UNPACK8(ptr, val_ptr, offset,  HDRRMAC_OLP_PILOT_STRENGTH_CORRECTION_MIN_SIZE)

/* PilotStrengthCorrectionMax */
#define HDRRMAC_OLP_PILOT_STRENGTH_CORRECTION_MAX( ptr, val_ptr, offset )  \
         HDRBIT_UNPACK8(ptr, val_ptr, offset,  HDRRMAC_OLP_PILOT_STRENGTH_CORRECTION_MIN_SIZE)

#endif /* FEATURE_LTE_TO_HDR_OH */

/*------------------------------------------------------------------------
                              LOOKUP TABLES
 -----------------------------------------------------------------------*/

#ifdef FEATURE_HDR_THERMAL_BACKOFF

/* Backoff percentile for each pattern */ 
static uint16 
  hdrrmac_thermal_power_bo_pattern_backoff_percentile[HDRRMAC_NUM_BACKOFF_PATTERNS] = 
  { 0, 35, 65, 100 };

  /* Backoff duration for each pattern */ 
static uint16 
  hdrrmac_thermal_power_bo_pattern_backoff_duration_ms[HDRRMAC_NUM_BACKOFF_PATTERNS] = 
  { 0, 0, 0, 0 };

  /* Non backoff duration for each pattern */ 
static uint16 
  hdrrmac_thermal_power_bo_pattern_non_backoff_duration_ms[HDRRMAC_NUM_BACKOFF_PATTERNS] = 
  { 0, 0, 0, 0 };

#endif /* FEATURE_HDR_THERMAL_BACKOFF */

#ifdef FEATURE_LTE_TO_HDR_OH
/* Table for PilotStrengthNominal encoding */
LOCAL const int8 hdrrmac_pilot_str_nom_val[] =
{
  0, -1, -2, -3, -4, 1, 2, 3
};

/* Table for PilotStrengthCorrectionMin encoding */
LOCAL const int8 hdrrmac_pilot_str_corr_min_val[] =
{
  0, -1, -2, -3, -4, -5
};

/* Table for PilotStrengthCorrectionMax encoding */
LOCAL const int8 hdrrmac_pilot_str_corr_max_val[] =
{
  0, 1, 2, 3, 4, 5
};
#endif /* FEATURE_LTE_TO_HDR_OH */

/*---------------------------------------------------------------------------
                            PROTOCOL TASK CONTEXT

  The Control Channel MAC protocol is executed within the context of the
  following task and the following task signals are assigned to the Control
  Channel MAC protocol.  The following assignments are specific to the task
  within which the protocol executes.
---------------------------------------------------------------------------*/
#include "hdrtx.h"
#include "task.h"

/* Task context block pointer. */
#define HDRRMAC_TASK_PTR HDRTX_TASK_PTR

/* Task command and message queue functions */

#define HDRRMAC_QUEUE_CMD( protocol, cmd_ptr, size )\
        ( hdrtx_queue_cmd( protocol, cmd_ptr, size ) )

#define HDRRMAC_QUEUE_MSG( protocol, instance, msg_ptr )\
        ( hdrtx_queue_msg( protocol, instance, msg_ptr ) )

#define HDRRMAC_QUEUE_IND( protocol, name, data )\
        ( hdrtx_queue_ind( protocol, name, data ) )

/*---------------------------------------------------------------------------
                            PROTOCOL STATES

  The Forward Traffic Channel MAC protocol can operate in the following
  states.
---------------------------------------------------------------------------*/

/* Number of protocol states. */
#define NUM_STATES   3

/* Table of valid state transitions. */
LOCAL const boolean hdrrmac_is_valid_transition[ NUM_STATES ][ NUM_STATES ] =
{
  { FALSE, TRUE,  FALSE },              /* Transitions from Inactive       */
  { TRUE,  FALSE, TRUE  },              /* Transitions from Setup          */
  { TRUE,  FALSE, FALSE }               /* Transitions from Active         */
};

/* Array for translating period to frames.        */
/*  ( One could also use (0x0800 << period) - 1)  */
const uint16 hdrrmac_silence_period[] =
{
  2047,
  4095,
  8191,
  16383
};

/* Stop watch */
typedef struct
{
  qword                      start_ms;
    /* start time of some event */

  qword                      end_ms;
    /* end time of some event */

} hdrrmac_stop_watch_struct_type;

LOCAL hdrrmac_stop_watch_struct_type hdrrmac_stop_watch;

/* Set to indicate a hard-handoff event */
LOCAL boolean hdrrmac_hho_flag = FALSE;

/* Common parameters used by all RTCMAC subtypes */
hdrrmac_common_type hdrrmac;

/* RMAC flow control payload restriction look-up table for subtype 0.
   Gives max Rel0 payload size index for a given payload restriction 
   level set by flow control. */
const uint8 hdrrmac_flow_control_rel0_payload_restriction[13]
  = { HDRRMAC_PILOT_ONLY,/* Stop data for restriction level 0                */
      HDRRMAC_9K6_BPS,   /* tx payload idx for restriction levels 1 and 2    */
      HDRRMAC_9K6_BPS,    
      HDRRMAC_19K2_BPS,  /* tx payload idx for restriction levels 3, 4 and 5 */
      HDRRMAC_19K2_BPS, 
      HDRRMAC_19K2_BPS, 
      HDRRMAC_38K4_BPS,  /* tx payload idx for restriction levels 6, 7 and 8 */
      HDRRMAC_38K4_BPS, 
      HDRRMAC_38K4_BPS, 
      HDRRMAC_76K8_BPS,  /* tx payload idx for restriction levels 9 and 10   */
      HDRRMAC_76K8_BPS,    
      HDRRMAC_IS856_RRI, /* No payload restriction for levels 11 and 12      */
      HDRRMAC_IS856_RRI    
    };

/* If we've sent RevLinkCarrier override to RF */
LOCAL boolean hdrrmac_rev_link_carrier_override = FALSE;
  
/*===========================================================================
FUNCTION HDRRMAC_RESET_RTC_RATE_COUNT                                EXTERNAL

DESCRIPTION
  This function resets the count of reverse traffic frames of each rate.
  [Runs in caller's task context.]
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrrmac_reset_rtc_rate_count( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrrmac0_reset_rtc_rate_count();

#ifdef FEATURE_HDR_REVA_L1
  hdrrmac3_reset_rtc_rate_count();

#ifdef FEATURE_HDR_REVB_SUPPORTED
  hdrrmac4_reset_rtc_rate_count();
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

}

/*===========================================================================
FUNCTION HDRRMAC_RESET_DM_RTC_RATE_COUNT                             EXTERNAL

DESCRIPTION
  This function resets the count of reverse traffic frames of each rate and
  commits the changes to the DM.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_reset_dm_rtc_rate_count( void )
{
  hdrrmac0_reset_rtc_rate_count();

#ifdef FEATURE_HDR_REVA_L1
  hdrrmac3_reset_rtc_rate_count();

#ifdef FEATURE_HDR_REVB_SUPPORTED
  hdrrmac4_reset_rtc_rate_count();
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

  hdrrmac_log_rtc_rate_count();
}


/*===========================================================================
FUNCTION HDRRMAC_LOG_RTC_RATE_COUNT                                  EXTERNAL

DESCRIPTION
  This function logs the current rate count.  Note that this may be more
  appropriately implemented as a request/response type message.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_log_rtc_rate_count( void )
{

  switch (hdrrmac.subtype)
  {

    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      hdrrmac0_log_rtc_rate_count();
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      hdrrmac3_log_rtc_rate_count();
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      hdrrmac4_log_rtc_rate_count();
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );
  }
}


/*===========================================================================
FUNCTION HDRRMAC_DISABLE_THROTTLING                                 EXTERNAL

DESCRIPTION
  This function allows the caller to request that Reverse Link throttling
  be disabled.  If the RTC MAC is not active at the time of the call, the
  throttle state will be changed the next time it becomes active.
  
  A caller ID is taken as a parameter, along with a boolean to indicate 
  whether the caller is registering the request or de-registering the 
  request.
  
  Note that since this function takes a boolean to vote on whether
  throttling is disables or not, there is no separate 
  "hdrrmac_enable_throttling" function.

DEPENDENCIES
  None.

PARAMETERS
  caller_id       - The entity requesting that throttling be disabled.
  disable_request - TRUE indicates the caller is requesting disable.
                  - FALSE indicates the caller is ending the request. 
                    (RL throttling may be re-enabled).
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_disable_throttling
(
  hdrrmac_caller_id_enum_type caller_id,
  boolean                     disable_request
)
{

  uint16 caller_id_value = (uint16) caller_id;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (disable_request)
  {
    /* Set bit */
    hdrrmac.throttle_disable_requests |= caller_id_value;

    HDR_MSG_PROT_2( MSG_LEGACY_HIGH, 
                    "RL throttle disable requested (caller=%x, req mask=%x)",
                    caller_id_value,
                    hdrrmac.throttle_disable_requests );

#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)

    if ( ( caller_id == HDRRMAC_FTAP_ID ) || ( caller_id == HDRRMAC_RTAP_ID ) )
    {
      hdrrmac.is_tap_call = TRUE;
        /* This function will be called if this is a TAP call. Use it as a trigger to set
           tap call flag.
        */
    }
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/

  }
  else
  {
    /* Clear bit */
    hdrrmac.throttle_disable_requests &= ~caller_id_value;

    HDR_MSG_PROT_2( MSG_LEGACY_HIGH, 
                    "RL throttle disable request end (caller=%x, req mask=%x)",
                    caller_id_value, 
                    hdrrmac.throttle_disable_requests );

#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)

    if ( ( caller_id == HDRRMAC_FTAP_ID ) || ( caller_id == HDRRMAC_RTAP_ID ) )
    {
      hdrrmac.is_tap_call = FALSE;
        /* This function will be called if this is a TAP call. Use it as a trigger to set
           tap call flag.
        */
    }
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/

  }

  /* If we are active, update now. */
  if (hdrrmac.state != HDRRMAC_INACTIVE_STATE)
  {
    switch (hdrrmac.subtype)
    {

      case HDRSCP_DEFAULT_RTCMAC:
      case HDRSCP_SUBTYPE_1_RTCMAC:
        hdrrmac0_update_throttling();  
        break;

#ifdef FEATURE_HDR_REVA_L1
      case HDRSCP_SUBTYPE_3_RTCMAC:
        hdrrmac3_update_throttling();  
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
      case HDRSCP_SUBTYPE_4_RTCMAC:
        hdrrmac4_update_throttling();
        break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

      default:
        HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
          "This function not supported for subtype %d", 
          hdrrmac.subtype );
    }

  } /* RMAC active? */

} /* hdrrmac_disable_throttling */

/*===========================================================================
FUNCTION HDRRMAC_THROTTLING_DIPSW_SET

DESCRIPTION
  This is a callback function for hdrlog's DIPSW API.  It disables 
  RL throttling when the "dip switch" is set.

DEPENDENCIES
  None.

PARAMETERS
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
LOCAL void hdrrmac_throttling_dipsw_set( void )
{
  hdrrmac_disable_throttling(HDRRMAC_DIPSW_ID, TRUE);
}

/*===========================================================================
FUNCTION HDRRMAC_THROTTLING_DIPSW_RESET

DESCRIPTION
  This is a callback function for hdrlog's DIPSW API.  It reenables RL 
  throttling when the "dip switch" is reset.

DEPENDENCIES
  None.

PARAMETERS
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
LOCAL void hdrrmac_throttling_dipsw_reset( void )
{
  hdrrmac_disable_throttling(HDRRMAC_DIPSW_ID, FALSE);
}


/*===========================================================================
FUNCTION HDRRMAC_SET_FIXED_RRI                                      EXTERNAL

DESCRIPTION
  Sets the RRI to a fixed value for testing purposes.

DEPENDENCIES
  None.

PARAMETERS
  rri      - The RRI to fix to. Use 0xf, or HDRRMAC_IS856_RRI for
             normal operation.

RETURN VALUE
  TRUE If the given RRI is valid for current subtype.
  FALSE otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmac_set_fixed_rri
(
  byte rri
)
{

  boolean rc = FALSE;  /* Return value */

  /* Take appropriate action based on subtype */
  switch ( hdrscp_get_current_subtype(HDRHAI_RTC_MAC_PROTOCOL) )
  {

    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      rc = hdrrmac0_set_fixed_rri(rri);
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      rc = hdrrmac3_set_fixed_rri(rri);
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      rc = hdrrmac4_set_fixed_rri(rri);
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );
  }
  
  return rc;

} /* hdrrmac_set_fixed_rri */

/*===========================================================================
FUNCTION HDRRMAC_CONFIG_EARLY_TERMINATION                       EXTERNAL

DESCRIPTION
  Configures early termination on the RL link. This is a test mode function.  

DEPENDENCIES
  None.

PARAMETERS
  config_mode: 0 - Default behavior, RL early termination on.
               1 - Forced early termination after 1 subpacket.
               2 - Forced early termination after 2 subpackets.
               3 - Forced early termination after 3 subpackets.
               4 - Forced early termination after 4 subpackets.

RETURN VALUE
  TRUE (config mode accepted),
  FALSE (config mode not valid).

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmac_config_early_termination
(
  int config_mode
)
{

  boolean rc = FALSE;  /* Return value */

  /* Take appropriate action based on subtype */
  switch ( hdrscp_get_current_subtype(HDRHAI_RTC_MAC_PROTOCOL) )
  {

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      rc = hdrrmac3_config_early_termination( config_mode );
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      rc = hdrrmac4_config_early_termination( config_mode );
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "Early termination config is not supported for subtype %d", 
        hdrrmac.subtype );
  }
  
  return rc;

} /* hdrrmac_config_early_termination */



/*===========================================================================

FUNCTION HDRRMAC_SET_CHANNEL_PARAMS                                  EXTERNAL

DESCRIPTION
  This function 
  - conveys DRC, ACK and DSC channel gain information to RMAC3
  - sets RMAC frame offset

DEPENDENCIES
  None

PARAMETERS
  channel_gains - DRC, ACK and DSC channel gains received over Traffic 
                  channel Assignment message
  frame_offset  - Frame Offset in slots.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_set_channel_params
(
  hdrrmac_channel_gains_struct_type *channel_gains,
  uint8 frame_offset
)
{
  hdrrmac_set_frame_offset(frame_offset);

#ifdef FEATURE_HDR_REVA_L1
#ifdef FEATURE_HDR_REVB_SUPPORTED
  if ((hdrrmac.subtype == HDRSCP_SUBTYPE_3_RTCMAC)
      || (hdrrmac.subtype == HDRSCP_SUBTYPE_4_RTCMAC))
#else
  if (hdrrmac.subtype == HDRSCP_SUBTYPE_3_RTCMAC)
#endif /* FEATURE_HDR_REVB_SUPPORTED */
  {
    hdrrmac3_set_channel_gains(channel_gains);
  }
#endif /* FEATURE_HDR_REVA_L1 */

}


/*===========================================================================

FUNCTION HDRRMAC_SET_FRAME_OFFSET                                    EXTERNAL

DESCRIPTION
  This function sets the RMAC frame offset.

DEPENDENCIES
  The parameter must not be NULL.
  
PARAMETERS
  frame_offset - the Frame offset

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_set_frame_offset
(            
  uint8 frame_offset
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  switch (hdrrmac.subtype)
  {

    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      hdrrmac0_set_frame_offset(frame_offset);
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      hdrrmac3_set_frame_offset(frame_offset);
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      hdrrmac4_set_frame_offset(frame_offset);
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );

  }

} /* hdrrmac_set_frame_offset */


/*===========================================================================

FUNCTION HDRRMAC_SET_SILENCE_DURATION                               EXTERNAL

DESCRIPTION
  This function sets the RMAC silence duration and offset.

DEPENDENCIES
  The parameter must not be NULL.

PARAMETERS
  silence_duration
  silence_period

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_set_silence_params
(
  uint8 silence_duration,
  uint8 silence_period
)
{

  /* Block interrupts during this update */
  REX_DISABLE_PREMPTION();

  hdrrmac.silence_duration = silence_duration;
  hdrrmac.silence_period = hdrrmac_silence_period[silence_period];
    /* Copy the data */

  /* If duration is zero, don't bother to calulate/enable silence intervals.
  */
  if (hdrrmac.silence_duration == 0)
  {
    hdrrmac.silence_enabled = FALSE;
  }
  else
  {
    hdrrmac.silence_enabled = TRUE;

    /* Compute silence time if RTC MAC is active, otherwise we
       wait until activated */
    if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
    {
      switch (hdrrmac.subtype)
      {

        case HDRSCP_DEFAULT_RTCMAC:
        case HDRSCP_SUBTYPE_1_RTCMAC:
          hdrrmac0_compute_silence_time();
          break;

#ifdef FEATURE_HDR_REVA_L1
        case HDRSCP_SUBTYPE_3_RTCMAC:
#ifdef FEATURE_HDR_REVB_SUPPORTED
        case HDRSCP_SUBTYPE_4_RTCMAC:
#endif /* FEATURE_HDR_REVB_SUPPORTED */
          break;
#endif /* FEATURE_HDR_REVA_L1 */

        default:
          HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
            "Subtype %d RTCMAC not supported", 
            hdrrmac.subtype );

      }
    }
  }

  REX_ENABLE_PREMPTION();
    /* Unblock interrupts */

  HDR_MSG_PROT_2( MSG_LEGACY_MED, "Silence Interval: Config for %d/%d frames",
                  silence_duration, hdrrmac.silence_period );

}


/*===========================================================================

FUNCTION HDRRMAC_FRAME_OFFSET                                    EXTERNAL

DESCRIPTION
  This function checks whether QTA falls in the scheduled RLSI

DEPENDENCIES
  None.
  
PARAMETERS
  frame_num   -  frame which needs to be checked for RLSI

RETURN VALUE
  TRUE - Given Frame is in silence duration
  FALSE - Given Frame is not in silence duration
  
SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmac_is_silence_frame 
( 
  qword frame_num   /* frame which needs to be checked for RLSI */
)
{
  boolean is_silence = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if(hdrrmac.silence_enabled == FALSE)
  {
    /* Return FALSE if the RLSI is enabled even before checking the frame time */
    return FALSE;
  }

  switch (hdrrmac.subtype)
  {

    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      is_silence = hdrrmac0_is_silence_frame(frame_num);
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      is_silence = hdrrmac3drv_is_silence_frame(frame_num);
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      is_silence = hdrrmac4drv_is_silence_frame(frame_num);
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );

  }

  return is_silence;

} /* hdrrmac_is_silence_frame */


/*============================================================================
FUNCTION HDRRMAC_PROCESS_FW_MSG

DESCRIPTION
  This function propagates received firmware indication/message to currently
  active RMAC subtype for processing.

DEPENDENCIES
  None.

PARAMETERS
  msg_ptr - pointer to received message

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrrmac_process_fw_msg 
(
  hdrtx_msg_s_type * msg_ptr 
)
{
  uint16  subtype;
    /* RTCMAC subtype */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  subtype = hdrscp_get_current_subtype(HDRHAI_RTC_MAC_PROTOCOL);

  switch(subtype)
  {
    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      hdrrmac0_process_fw_msg( msg_ptr );
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
#ifdef FEATURE_HDR_FLEX_CONN
      if ( msg_ptr->msg.hdr.id == HDR_FW_FLEXCONN_REPOINT_IND )
      {
        hdrrmac_process_flexconn_repoint();
      }
#endif /* FEATURE_HDR_FLEX_CONN */
      break;
#endif /* FEATURE_HDR_REVA_L1 */

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
#ifdef FEATURE_HDR_FLEX_CONN
      if ( msg_ptr->msg.hdr.id == HDR_FW_FLEXCONN_REPOINT_IND )
      {
        hdrrmac_process_flexconn_repoint();
      }
#endif /* FEATURE_HDR_FLEX_CONN */
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */

    default:
      ERR_FATAL("Subtype %d RTCMAC not supported",
                hdrrmac.subtype,0,0);

  } /* switch - RMAC subtype */

} /* hdrrmac_process_fw_msg */


/*===========================================================================
FUNCTION HDRRMAC_INIT                                               EXTERNAL

DESCRIPTION
  This function initializes the protocol prior to use.  It should be called
  once upon system startup.  The supervision timer is initialized and the
  protocol state is set to Inactive.

DEPENDENCIES
  Should be run in HDRTx context, since timers are defined.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_init ( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Initialize the protocol state. */
  hdrrmac.state = HDRRMAC_INACTIVE_STATE;

  /* Init RF status */
  hdrrmac.rf_status = TX_OFF;

  /* Init link suspend status */
  hdrrmac.link_is_suspended = FALSE;   

  /* Init tx start. */
  hdrrmac.tx_started = FALSE;

  /* Init throttling request mask to false. */
  hdrrmac.throttle_disable_requests = 0;

#ifdef FEATURE_LTE_TO_HDR_OH
  /* Initialize OLP valid flag to false. */
  hdrrmac.olp_msg.valid = FALSE;
#endif /* FEATURE_LTE_TO_HDR_OH */

  /* Subtype specific init */
  hdrrmac0_init(&hdrrmac);

#ifdef FEATURE_HDR_QTA
  hdrrmac.tune_away_info.pend_on_ta = HDRRMAC_NO_CMD_PENDING;
#endif
} /* hdrrmac_init */

#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX) ||\
    defined (FEATURE_HDR_TRAFFIC_DTX)
/*===========================================================================
FUNCTION HDRRMAC_FLEXCONN_DTX_POWERUP_INIT                                       EXTERNAL

DESCRIPTION
  This function initializes FlexConn and DTX releated variables.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_flexconn_dtx_powerup_init ( void )
{
  uint16  dtx_cfg;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* enable FlexConn and DTX by default for DIME */
#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
  hdrrmac.is_dtx_nv_enabled = TRUE;
  hdrrmac.device_only_dtx_mode = FALSE;
  hdrrmac.dtx_monitor_started = FALSE;
  hdrrmac.dtx_nest_count = 1;
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */
#ifdef FEATURE_HDR_FLEX_CONN
  hdrrmac.is_flexconn_nv_enabled = TRUE;
  hdrrmac.flexconn_mode = FALSE;
  hdrrmac.flexconn_monitor_started = FALSE;
  hdrrmac.flexconn_nest_count = 1;
  hdrrmac.is_repointing_msg_rcvd = FALSE;
#endif /* FEATURE_HDR_FLEX_CONN */
#ifdef FEATURE_HDR_REVB_DTX_MODE
  hdrrmac.network_dtx_mode = FALSE;
#endif /* FEATURE_HDR_REVB_DTX_MODE */

#ifdef FEATURE_HDR_TRAFFIC_DTX
  hdrrmac.tdtx_nv_is_enabled = TRUE;
  hdrrmac.tdtx_mode = FALSE;
#endif /* FEATURE_HDR_TRAFFIC_DTX */

  hdrrmac.dtx_config = 0x0;

  if ( hdrutil_fs_get( HDRRMAC_EFS_NV_DTX_CFG,
                       &dtx_cfg,
                       sizeof(uint16) ) == MCFG_FS_STATUS_OK )
  {
    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
             "FlexConn/DTX/tDTX: nv item hdr_dtx_cfg =%d",
             dtx_cfg );
  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, 
            "FlexConn/DTX/tDTX: NV hdr_dtx_cfg is not available,set it to 0");
    dtx_cfg = 0;  
      /* Enable Flexconn and DTX if the NV is not available */ 
  }

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
   if ( ( dtx_cfg & HDRRMAC_EFS_NV_DEVICE_ONLY_DTX_DISABLED )
         == HDRRMAC_EFS_NV_DEVICE_ONLY_DTX_DISABLED )
   {
     /* Disable Device Only DTX if the NV bit is set to 1. */
     hdrrmac.is_dtx_nv_enabled = FALSE;

     HDR_MSG_PROT( MSG_LEGACY_HIGH, "RMAC DTX: DeviceOnly DTX NV disabled");
   }
   else
   {
     /* Leave it enabled, print notifying message */
     HDR_MSG_PROT( MSG_LEGACY_HIGH, "RMAC DTX: DeviceOnly DTX NV enabled");
   }
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */

#ifdef FEATURE_HDR_FLEX_CONN

   if ( ( dtx_cfg & HDRRMAC_EFS_NV_FLEX_CONN_DISABLED )
         == HDRRMAC_EFS_NV_FLEX_CONN_DISABLED ) 
   {
     /* Disble FlexConn if the NV bit is set. */
     hdrrmac.is_flexconn_nv_enabled = FALSE;

     HDR_MSG_PROT( MSG_LEGACY_HIGH, "RMAC FLEXCONN: FLEXCONN NV disabled");
   }
   else
   {
     /* Leave it enabled, print notifying message */
     HDR_MSG_PROT( MSG_LEGACY_HIGH, "RMAC FLEXCONN: FLEXCONN NV enabled");
   }
#endif /* FEATURE_HDR_FLEX_CONN */

#ifdef FEATURE_HDR_TRAFFIC_DTX
   if ( ( dtx_cfg & HDRRMAC_EFS_NV_TRAFFIC_DTX_DISABLED )
         == HDRRMAC_EFS_NV_TRAFFIC_DTX_DISABLED )
   {
     /* Disable tDTX if the NV bit is set. */
     hdrrmac.tdtx_nv_is_enabled = FALSE;

     HDR_MSG_PROT( MSG_LEGACY_HIGH, "tDTX: tDTX NV disabled");
   }
   else
   {
     /* Leave it enabled by default, print notifying message */
     HDR_MSG_PROT( MSG_LEGACY_HIGH, "tDTX: tDTX NV enabled");
   }
#endif /* FEATURE_HDR_TRAFFIC_DTX */

} /* hdrrmac_flexconn_dtx_powerup_init */
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX) */

static void hdrrmac_sanity_timer_cb
( 
   uint32 timer_id
)
{
  
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */  
  ERR_FATAL("HDR RMAC stuck - force crash!", 0, 0, 0 );
 
} /* hdrrmac_sanity_timer_cb */

/*===========================================================================
FUNCTION HDRRMAC_POWERUP_INIT                                       EXTERNAL

DESCRIPTION
  This function initializes RMAC session configuration.

DEPENDENCIES
  Note that this is run in HDRMC context.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_powerup_init ( void )
{
  boolean init_rmac0_attrib = FALSE;
#ifdef FEATURE_HDR_REVA_L1
  boolean init_rmac3_attrib = FALSE;
#endif /* FEATURE_HDR_REVA_L1 */
#ifdef FEATURE_HDR_REVB_SUPPORTED
  boolean init_rmac4_attrib = FALSE;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#ifdef FEATURE_HDR_THERMAL_BACKOFF
  ds707_ccm_hdlr_func_comm_tbl_type hdlr_tbl;
    /* Handler functions for CCM */
#endif /* FEATURE_HDR_FL_FLOW_CONTROL */


  /* Install callbacks for the HDR_DIPSW command to disable/enable
     Reverse Link throttling. */
  hdrlog_set_dipsw_cb(HDRLOG_DIPSW_THROTTLING_OFF,
                      hdrrmac_throttling_dipsw_set,
                      hdrrmac_throttling_dipsw_reset);

  hdrrmac.subtype = hdrscp_get_current_subtype(HDRHAI_RTC_MAC_PROTOCOL);

  switch(hdrrmac.subtype)
  {
    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      init_rmac0_attrib = TRUE;
      break;
  
#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      init_rmac3_attrib = TRUE;
      break;
#endif /* FEATURE_HDR_REVA_L1 */

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      init_rmac4_attrib = TRUE;
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */

    default:
      ERR_FATAL("Subtype %d RTCMAC not supported",
                hdrrmac.subtype,0,0);
  }
 
#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)||\
    defined ( FEATURE_HDR_TRAFFIC_DTX)
  hdrrmac_flexconn_dtx_powerup_init();
#endif /* (FEATURE_HDR_FLEX_CONN)||(FEATURE_HDR_DEVICE_ONLY_DTX)
|| FEATURE_HDR_TRAFFIC_DTX */

  hdrrmac0_powerup_init(init_rmac0_attrib);

#ifdef FEATURE_HDR_REVA_L1
  hdrrmac3_powerup_init(init_rmac3_attrib, &hdrrmac);
#endif /* FEATURE_HDR_REVA_L1 */

#ifdef FEATURE_HDR_REVB_SUPPORTED
  hdrrmac4_powerup_init(init_rmac4_attrib, &hdrrmac);
#endif /* FEATURE_HDR_REVB_SUPPORTED */

  /* Initialize TAP call flag */
  hdrrmac.is_tap_call = FALSE;

#ifdef FEATURE_HDR_THERMAL_BACKOFF

  /* Initialize working variables indicating thermal flow control state to 
     default values */
  hdrrmac.thermal_power_bo_is_on = FALSE;
  hdrrmac.thermal_power_bo_cmd = DS707_CCM_FLOW_OFF;
  hdrrmac.thermal_power_bo_pattern = HDRRMAC_NO_BACKOFF_PATTERN;
  
  if ( hdrmc_is_first_init() )
  {
    /* Initialize pa backoff step up timer */
    rex_def_timer_ex(&hdrrmac.thermal_power_bo_step_up_timer,
                     hdrrmac_thermal_power_bo_step_timer_cb,
                     HDRRMAC_STEP_UP_TIMER);

    /* Initialize pa backoff step down timer */
    rex_def_timer_ex(&hdrrmac.thermal_power_bo_step_down_timer,
                     hdrrmac_thermal_power_bo_step_timer_cb,
                     HDRRMAC_STEP_DOWN_TIMER);

    /* Intialize timer for backoff duration */
    rex_def_timer_ex(&hdrrmac.thermal_power_bo_backoff_timer,
                     hdrrmac_set_thermal_power_bo_timer_cb,
                     0);

    /* Intialize timer for non backoff duration in the backoff period */
    rex_def_timer_ex(&hdrrmac.thermal_power_bo_non_backoff_timer,
                     hdrrmac_set_thermal_power_non_bo_timer_cb,
                     0);

    rex_def_timer_ex(&hdrrmac.sanity_timer,
                     hdrrmac_sanity_timer_cb,
                     0);

    /* Initialize the thermal backoff critical section variable */
    rex_init_crit_sect(&hdrrmac.thermal_power_bo_crit_sect);
  }

  hdlr_tbl.ds707_ccm_comm_config_func = hdrrmac_configure_thermal_pa_backoff_cb;
  hdlr_tbl.ds707_ccm_comm_contrl_func = hdrrmac_control_thermal_pa_backoff_cb;

  ds707_ccm_common_register_handler(DS707_CCM_THERMAL_BACKOFF_FC, &hdlr_tbl);
  
#endif /* FEATURE_HDR_THERMAL_BACKOFF */
}

/*===========================================================================
FUNCTION HDRRMAC_ENTER_INACTIVE_STATE                               INTERNAL

DESCRIPTION
  Enters inactive state.

DEPENDENCIES
  RTC MAC must be active.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_enter_inactive_state( void )
{

#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
  /* In conn tearing down, rmac enters inactive state. We need to stop flexconn   
     and dtx here. PA off is done in next step
     rmac3/4_enter_inactive_state()->rmac3/4_stop()
  */
  hdrrmac_stop_flexconn_dtx_monitor( FALSE );
    /* No need to suspend RL inactive timer after conn is  broken */
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/
  hdrrmac.is_tap_call = FALSE;
    /* Clear the flag for tap call */

#ifdef FEATURE_HDR_TRAFFIC_DTX
  /* Reset tDTX mode */
  hdrrmac.tdtx_mode = FALSE;
#endif /* FEATURE_HDR_TRAFFIC_DTX */

  switch (hdrrmac.subtype)
  {

    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      hdrrmac0_enter_inactive_state();
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      hdrrmac3_enter_inactive_state();
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      hdrrmac4_enter_inactive_state();
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "Subtype %d RTCMAC not supported", 
        hdrrmac.subtype );

  }

  /* Trigger Tx DAC stop */
  hdrmod_tx_stop();

  /* Release tech request for bumped clk_bus rate. */
  MCPM_MCVSConfig_Release(MCPM_DO_TECH);

  /* Deactivate RF. */
  hdrrmac.rf_status = hdrtx_disable_tx( FALSE );

  /* Reset RevLinkCarrier override flag */
  hdrrmac_override_rev_link_carrier_set( FALSE, 
                                         NULL, 
                                         0 );

  /* reset HHO flag */
  hdrrmac_hho_flag = FALSE;

  /* Reset link suspend state. */
  hdrrmac.link_is_suspended = FALSE;

  /* Mark status as not yet started. */
  hdrrmac.tx_started = FALSE;

#ifdef FEATURE_HDR_THERMAL_BACKOFF

  
  HDR_MSG_PROT( MSG_LEGACY_HIGH, "hdrrmac_deactivate(), disabling thermal pa backoff" );

  rfm_hdr_set_thermal_state(hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN),  
                            RFM_HDR_THERMAL_STATE_DISABLE);
    
  /* Re-initialize thermal backoff params */
  hdrrmac.thermal_power_bo_is_on = FALSE;
  hdrrmac.thermal_power_bo_pattern = HDRRMAC_NO_BACKOFF_PATTERN;
    
  /* Stop step timer */
  rex_clr_timer(&hdrrmac.thermal_power_bo_step_down_timer);
  rex_clr_timer(&hdrrmac.thermal_power_bo_step_up_timer);

  /* Stop backoff timer */
  rex_clr_timer(&hdrrmac.thermal_power_bo_backoff_timer);
  rex_clr_timer(&hdrrmac.thermal_power_bo_non_backoff_timer);
  
#endif /* FEATURE_HDR_THERMAL_BACKOFF */

}

/*===========================================================================
FUNCTION HDRRMAC_ENTER_SETUP_STATE                                   INTERNAL

DESCRIPTION
  Enters open state.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_enter_setup_state( void )
{
  switch (hdrrmac.subtype)
  {

    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      hdrrmac0_init_mac_flow_ids();
#ifdef FEATURE_HDR_MODEM_API_RL_INFO
      hdrrmac0_init_modemapi();
#endif /* FEATURE_HDR_MODEM_API_RL_INFO */
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      hdrrmac3_init();
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      hdrrmac4_init();
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "Subtype %d RTCMAC not supported", 
        hdrrmac.subtype );

  }
}


/*===========================================================================
FUNCTION HDRRMAC_ENTER_OPEN_STATE                                   INTERNAL

DESCRIPTION
  Enters open state.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_enter_open_state( void )
{

#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
  /* In connection setup(idle->traffic), rmac enters open state after RTC ACK.   
     Need to start flexconn/dtx monitor from this point. 
  */
#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
  hdrrmac.dtx_nest_count = 1;
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */
#ifdef FEATURE_HDR_FLEX_CONN
  hdrrmac.flexconn_nest_count = 1;
#endif /* FEATURE_HDR_FLEX_CONN */
  hdrrmac_start_flexconn_dtx_monitor( FALSE );
    /* Inform RMAC to start monitor FL and RL and decide whether to enable
       Flexconn or DTX. No need to resume RL active timer at conn setup.
    */
#endif /* defined(FEATURE_HDR_FLEXCONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/

  switch (hdrrmac.subtype)
  {

    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      hdrrmac0_enter_open_state();
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      hdrrmac3_enter_open_state();
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      hdrrmac4_enter_open_state();
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "Subtype %d RTCMAC not supported", 
        hdrrmac.subtype );

  }
}



/*===========================================================================
FUNCTION HDRRMAC_ENTER_STATE                                         INTERNAL

DESCRIPTION
  This function manages the protocol state transitions.  For valid state
  transitions, the current state is exited by performing any required exit
  processing, then the new state is entered by perfroming any required entry
  processing.

DEPENDENCIES
  None.

PARAMETERS
  new_state - new state to enter.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_enter_state
(
  hdrrmac_state_enum_type new_state
)
{
  /*-------------------------------------------------------------------------
   Determine if the state transition is valid.
  -------------------------------------------------------------------------*/
  if ( hdrrmac_is_valid_transition[ hdrrmac.state ][ new_state ] == FALSE )
  {
    HDR_MSG_PROT(MSG_LEGACY_ERROR, "Invalid state transition!");
  }
  else
  {
    HDRTRACE_STATE( HDRHAI_RTC_MAC_PROTOCOL, (uint32) hdrrmac.state, 
                    (uint32) new_state );

    /*-----------------------------------------------------------------------
     Exit current state.
    -----------------------------------------------------------------------*/
    switch ( hdrrmac.state )
    {
      case HDRRMAC_INACTIVE_STATE:
        break;

      case HDRRMAC_SETUP_STATE:
        break;

      case HDRRMAC_OPEN_STATE:
        break;

      default:
        break;
    }

    /*-----------------------------------------------------------------------
     Enter new state.
    -----------------------------------------------------------------------*/
    switch ( new_state )
    {
      case HDRRMAC_INACTIVE_STATE:
        hdrrmac_enter_inactive_state();
        break;

      case HDRRMAC_SETUP_STATE:
        hdrrmac_enter_setup_state();
        break;

      case HDRRMAC_OPEN_STATE:
        hdrrmac_enter_open_state();
        break;

      default:
        break;
    }

    /*-----------------------------------------------------------------------
     Update state variable.
    -----------------------------------------------------------------------*/
    hdrrmac.state = new_state;
    HDRTRACE_STATE_ENTRY( HDRHAI_RTC_MAC_PROTOCOL, (uint32) new_state);
  }
}


/*===========================================================================
FUNCTION HDRRMAC_IS_ACTIVE                                           EXTERNAL

DESCRIPTION
  This function queries the current state of the protocol.

DEPENDENCIES
  None.

PARAMETERS
  caller - The protocol calling this externalized function (for sw trace)

RETURN VALUE
  TRUE  = protocol is Active.
  FALSE = protocol is Inactive.

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmac_is_active 
( 
  hdrhai_protocol_name_enum_type caller 
  /*lint -esym(715, caller)
   * Ignore argument(caller) not used lint error #715 */
)
{
  return ( ( hdrrmac.state != HDRRMAC_INACTIVE_STATE ) ? TRUE : FALSE );
}



/*===========================================================================
FUNCTION HDRRMAC_PA_IS_AVAILABLE                                    EXTERNAL

DESCRIPTION
  This function queries the current availability of the PA.

DEPENDENCIES
  None.

PARAMETERS
  caller - The protocol calling this externalized function (for sw trace)

RETURN VALUE
  TRUE  = PA is available
  FALSE = PA is not currently available, due to 1) RTC MAC is not active or
          2) RTC MAC is in DRC supervision.

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmac_pa_is_available ( hdrhai_protocol_name_enum_type caller )
{

  boolean pa_available = FALSE;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
  {
    switch (hdrrmac.subtype)
    {
  
      case HDRSCP_DEFAULT_RTCMAC:
      case HDRSCP_SUBTYPE_1_RTCMAC:
        pa_available = hdrrmac0_pa_is_available(caller);
        break;

#ifdef FEATURE_HDR_REVA_L1
      case HDRSCP_SUBTYPE_3_RTCMAC:
        pa_available = hdrrmac3_pa_is_available(caller);
        break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
      case HDRSCP_SUBTYPE_4_RTCMAC:
        pa_available = hdrrmac4_pa_is_available(caller);
        break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

      default:
        HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
          "Subtype %d RTCMAC not supported", 
          hdrrmac.subtype );

    }
  }

  return pa_available;
}



/*===========================================================================
FUNCTION HDRRMAC_START_STOP_WATCH                                    INTERNAL

DESCRIPTION
  Starts stop watch timing. This is implemented using time stamps
  for better accuracy. Call hdrrmac_read_stop_watch() to get
  the difference between start and stop timings.

DEPENDENCIES
  None.

INPUTS
  None

RETURN VALUE

SIDE EFFECTS
  none

==========================================================================*/
void hdrrmac_start_stop_watch( void )
{
  time_get_ms ( hdrrmac_stop_watch.start_ms );
    /* start timing the events for debug purposes */

}/* hdrrmac_start_stop_watch */



/*===========================================================================
FUNCTION HDRRMAC_READ _STOP_WATCH                                    INTERNAL

DESCRIPTION
  Returns the time between start and end events on the stop watch

DEPENDENCIES
  hdrrmac_start_stop_watch() should have been called before

INPUTS
  None

RETURN VALUE
 dword : time in msec since hdrrmac_start_stop_watch()

SIDE EFFECTS
  none

==========================================================================*/
dword hdrrmac_read_stop_watch( void )
{
  qword       delta_ms;
    /* difference between two stop watch events */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  time_get_ms ( hdrrmac_stop_watch.end_ms );
    /* read stop watch end time */

  /* take difference between start and end time */
  qw_sub ( delta_ms,
           hdrrmac_stop_watch.end_ms,
           hdrrmac_stop_watch.start_ms );

  return  qw_lo ( delta_ms );
    /* Compute and return the elapsed time in milliseconds */

}/* hdrrmac_read_stop_watch */


#ifdef FEATURE_LTE_TO_HDR_OH
/*===========================================================================
FUNCTION HDRRMAC_OH_INIT_TX_POWER

DESCRIPTION
  This function performs tx power initialization during optimized handoff.
 
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE  - if tx power could be initialized
  FALSE - if tx power could not be initialized

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmac_oh_init_tx_power( void )
{
  int8    olp_msg_nom, olp_msg_corr_min, olp_msg_corr_max;
  int8    pilot_str_corr;
  int16   open_loop_adjust, initial_tx_adjust;
  uint8   i, index = 0;
  sys_pilot_pn_type     best_sector_pn;

  hdrsrchmac_active_sectors_struct_type srch_info;
    /* Information on sectors in the active set */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  /* Proceed with tx power initialization only if we have received OLP message 
     that hasn't been used yet. */
  if ( !hdrrmac.olp_msg.valid )
  {
    return FALSE;
  }

  /* Get pilot PN to use from HDR SRCH */
  hdrsrchmac_get_mac_aset_info( HDRMDSP_DEMOD_CARRIER_0, &srch_info );
  best_sector_pn = 
    srch_info.active_sectors[srch_info.best_sector_idx].pn_offset;
  
  /* For now, just look at the first channel */
  if (hdrrmac.olp_msg.rev_cdma_ch_olp[0].num_pilots > 1)
  {
    /* loop from 0 to numpilot-1 */
    for (i=0; i< hdrrmac.olp_msg.rev_cdma_ch_olp[0].num_pilots; i++)
    {
      /* check in best_sector_pn = olp.pilot_pn */
      /* if so, set index and exit loop */
      if (best_sector_pn == hdrrmac.olp_msg.rev_cdma_ch_olp[0].olp[i].pilot_pn)
      {
        index = i;
        break; 
      }
    }
  }

  /* Needs to be indexed by PN (best_sector_pn) */
  open_loop_adjust  = 
    hdrrmac.olp_msg.rev_cdma_ch_olp[0].olp[index].open_loop_adjust;
  initial_tx_adjust = 
    hdrrmac.olp_msg.rev_cdma_ch_olp[0].olp[index].initial_adjust;

  HDR_MSG_PROT_2( MSG_LEGACY_HIGH,
    "OptHOMsg: OpenLoopAdjust %d, ClosedLoopAdjust %d",
    open_loop_adjust, initial_tx_adjust );

  if (hdrrmac.olp_msg.rev_cdma_ch_olp[0].olp[index].pilot_strength_incl)
  {
    olp_msg_nom      = 
      hdrrmac.olp_msg.rev_cdma_ch_olp[0].olp[index].pilot_strength_nominal;
    olp_msg_corr_min = 
      hdrrmac.olp_msg.rev_cdma_ch_olp[0].olp[index].pilot_strength_correction_min;
    olp_msg_corr_max = 
      hdrrmac.olp_msg.rev_cdma_ch_olp[0].olp[index].pilot_strength_correction_max;

    pilot_str_corr = hdrrmac_calc_pilot_strength_correction( 
      HDRMDSP_DEMOD_CARRIER_0, olp_msg_nom,
      olp_msg_corr_min, olp_msg_corr_max);

    /* add Pilot strength correction to Probe Init Adjust value */
    initial_tx_adjust += pilot_str_corr;
  }

  /* Set nominal pwr in RF driver */
  rfm_hdr_set_nominal_pwr(hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), open_loop_adjust);
    
  /* Set Tx power, in .5 dB. */
  if( rfm_hdr_set_tx_power_adjust( hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), (initial_tx_adjust * 2) ) )
  {
    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
      "Adjusting Tx power by %d dB",
      initial_tx_adjust );
  }
  else
  {
    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
      "Initial adjust hit max power (tried %d dB)",
      initial_tx_adjust );
  }

  hdrrmac.olp_msg.valid = FALSE;

  return TRUE;

} /* hdrrmac_oh_init_tx_power() */
#endif /* FEATURE_LTE_TO_HDR_OH */


/*===========================================================================
FUNCTION HDRRMAC_TX_ENABLED_CB

DESCRIPTION
  This function is called when the RF Tx enable is complete.  It enables
  RMAC operation.

DEPENDENCIES
  None.

PARAMETERS
  RF Tx status.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_tx_enabled_cb
(
  hdrtx_rf_status_enum_type rf_status
)
{
  dword   hho_ms;
    /* time in msec for which AT was performing HHO */

  int16 turnaround = 0;
  int16 nominal_pwr = 0;
    /* For getting RF turnaround info */

  int16 tx_pwr;
    /* Grab tx adjust power from access for initial call */

  HDR_MSG_PROT(MSG_LEGACY_MED, "hdrrmac_tx_enabled_cb()" );

  /* Sanity check state */
  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
  {

    /* Update RF status */
    hdrrmac.rf_status = rf_status;

    /* Initialize tx initial power adjust if this is first tx since RTC MAC
       was activated. */
    if (hdrrmac.tx_adjust_needs_init == TRUE)
    {
      HDR_MSG_PROT(MSG_LEGACY_HIGH, "First TCA after access" );

#ifdef FEATURE_LTE_TO_HDR_OH
      if(hdrrmac_oh_init_tx_power())
      {
        HDR_MSG_PROT( MSG_LEGACY_HIGH, 
          "Optimized handoff tx power initialization successful" );
      }
      else
      {
        HDR_MSG_PROT( MSG_LEGACY_HIGH, 
          "Optimized handoff tx power initialization skipped" );
#endif /* FEATURE_LTE_TO_HDR_OH */

        tx_pwr = hdramac_get_last_tx_pwr( HDRHAI_RTC_MAC_PROTOCOL );

        /* Set Tx power, in .5 dB. */
        if( rfm_hdr_set_tx_power_adjust( 
          hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN),
          tx_pwr ) )
        {
          HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
            "Adjusting Tx power by %d dB",
            tx_pwr/2 );
        }
        else
        {
          HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
            "Initial adjust hit max power (tried %d dB)",
            tx_pwr/2 );
        }

#ifdef FEATURE_LTE_TO_HDR_OH
      }
#endif 

      hdrrmac.tx_adjust_needs_init = FALSE;
    }

    /* Update RF turnaround info */
    rfm_hdr_get_turnaround_info( hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), 
                                 &turnaround, 
                                 &nominal_pwr );

    HDR_MSG_PROT_2( MSG_LEGACY_HIGH, 
      "Turnaround: %d, NominalPwr: %d",
      turnaround,
      nominal_pwr );

    hdrrmac.turnaround_db256 = turnaround * 256;
    hdrrmac.nominal_pwr_db256 = nominal_pwr * 256;

    if ((hdrrmac.tx_started) && !(hdrrmac.link_is_suspended))
    { 
      switch (hdrrmac.subtype)
      {

        case HDRSCP_DEFAULT_RTCMAC:
        case HDRSCP_SUBTYPE_1_RTCMAC:
          hdrrmac0_start_rmac();
          break;

#ifdef FEATURE_HDR_REVA_L1
        case HDRSCP_SUBTYPE_3_RTCMAC:

          if (hdrrmac_hho_flag)
            /* returning from hard-handoff */
          {
            hho_ms = hdrrmac_read_stop_watch();
              /* read the time between start of HHO and present time */

            hdrrmac3_resume_connection( hho_ms, hdrrmac_hho_flag );

            /* reset the hard-handoff flag */
            hdrrmac_hho_flag = FALSE;

          }
          else
          {
            hdrrmac3_start_rmac();
          }
          break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
        case HDRSCP_SUBTYPE_4_RTCMAC:

          if (hdrrmac_hho_flag)
            /* returning from hard-handoff */
          {
            hho_ms = hdrrmac_read_stop_watch();
              /* read the time between start of HHO and present time */

            hdrrmac4_resume_connection( hho_ms, hdrrmac_hho_flag );

            /* reset the hard-handoff flag */
            hdrrmac_hho_flag = FALSE;

          }
          else
          {
            hdrrmac4_start_rmac();
          }
          break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

        default:
          HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
            "Subtype %d RTCMAC not supported", 
            hdrrmac.subtype );

      }
    }
  }
  else
  {
     HDR_MSG_PROT(MSG_LEGACY_ERROR,
       "RTC MAC: Tx enable callback triggered in inactive state");     
  }
}


/*===========================================================================
FUNCTION HDRRMAC_ACTIVATE                                            EXTERNAL

DESCRIPTION
  This command transitions the protocol to the Setup Rate state.  If the
  protocol is already in the Setup or Open state then no action is taken.
  Hdrrmac_start_tx() must be called to enable actual transmission.

DEPENDENCIES
  The protocol must be initialized by calling the hdrrmac_init function prior
  to activation.

PARAMETERS
  sender - The protocol giving this command (only for software trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_activate ( hdrhai_protocol_name_enum_type sender )
{
  /* Temporary command buffer. */
  hdrrmac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_PROT( MSG_LEGACY_LOW, "hdrrmac_activate()" );

  cmd_buf.hdr.cmd = HDRRMAC_ACTIVATE_CMD;
  cmd_buf.hdr.sender = sender;

  HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );
}

/*===========================================================================
FUNCTION HDRRMAC_DEACTIVATE                                          EXTERNAL

DESCRIPTION
  This command transitions the protocol to the Inactive state and clears the
  Reverse Traffic Channel supervision timer.  If the protocol is already in
  the Inactive state then no action is taken.

DEPENDENCIES
  Unless in FTM mode, must be called in hdrmc context.

PARAMETERS
  sender - The protocol giving this command (only for software trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_deactivate ( hdrhai_protocol_name_enum_type sender )
{
  /* Temporary command buffer. */
  hdrrmac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrrmac.state == HDRRMAC_INACTIVE_STATE )
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "RMAC is already inactive, ignoring the deactivate request!" );

    return;
  }

#ifdef FEATURE_HDR_QTA
  if ( hdrsrch_is_ta_started() && hdrsrch_is_qta_enabled() )
  {
    /* QTA is started and hence wait for tuneback trigger before RMAC deactivate */
    hdrrmac.tune_away_info.pend_on_ta |= HDRRMAC_DEACTIVATE_CMD_PENDING;
    HDR_MSG_PROT_1 ( MSG_LEGACY_HIGH, 
          "RTCMAC recieved deactivate. Pending command id %d",hdrrmac.tune_away_info.pend_on_ta );
  }
  else
#endif /* FEATURE_HDR_QTA */
  {
    /* TA is not started and hence continue the RMAC deactivate */

#ifdef FEATURE_HDR_QTA
    if ( ! ( hdrrmac.tune_away_info.pend_on_ta & HDRRMAC_SHUTDOWN_CMD_PENDING ) )
    {
      /* If HDRRMAC_SHUTDOWN_CMD is not pending */
      hdrsrch_connection_close_state( TRUE );
        /* After shutdown TA will be still disabled for deactivate to complete */
    }
#endif /* FEATURE_HDR_QTA */
      /* disable TA in HDR-SRCH */

    HDR_MSG_PROT( MSG_LEGACY_HIGH, "hdrrmac_deactivate()" );
  
    cmd_buf.hdr.cmd = HDRRMAC_DEACTIVATE_CMD;
    cmd_buf.hdr.sender = sender;
  
    HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );
  }

#ifdef FEATURE_FACTORY_TESTMODE
  /* KSS look into removing */
  if( ftm_get_mode() != FTM_MODE )
#endif
  {
    rex_set_timer( &hdrrmac.sanity_timer, HDRRMAC_SANITY_TIMER_MS );
    (void) hdrmc_wait(HDRMC_MAC_DEACTIVATE_COMPLETE_SIG);
    (void) rex_clr_sigs(HDRMC_TASK_PTR, HDRMC_MAC_DEACTIVATE_COMPLETE_SIG);
     HDR_MSG_PROT ( MSG_LEGACY_HIGH, 
          "HDRMC recieved deactivate complete. Clearing timer");
     rex_clr_timer( &hdrrmac.sanity_timer );
  }
}

/*===========================================================================
FUNCTION HDRRMAC_SHUTDOWN                                            EXTERNAL

DESCRIPTION
  This command tells the RTCMAC to shutdown after the current Txing frame
  has completed.  It does NOT deactivate the protocol.

DEPENDENCIES
  None.

PARAMETERS
  sender - The protocol giving this command (only for software trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_shutdown ( hdrhai_protocol_name_enum_type sender )
{
  /* Temporary command buffer. */
  hdrrmac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDR_QTA
  if ( hdrsrch_is_ta_started() && hdrsrch_is_qta_enabled() )
  {
    /* QTA is started and hence wait for tuneback trigger before RMAC deactivate */
    hdrrmac.tune_away_info.pend_on_ta |= HDRRMAC_SHUTDOWN_CMD_PENDING;
    HDR_MSG_PROT_1 ( MSG_LEGACY_HIGH, 
          "Shutdown cmd recieved. Pending command id %d",hdrrmac.tune_away_info.pend_on_ta );
  }
  else
#endif /* FEATURE_HDR_QTA */
  {
    /* TA is not started and continue the shutdown cmd */

#ifdef FEATURE_HDR_QTA
    hdrsrch_connection_close_state( TRUE );
      /* disable TA in HDR-SRCH */
#endif /* FEATURE_HDR_QTA */

  HDR_MSG_PROT( MSG_LEGACY_HIGH, "hdrrmac_shutdown()" );

  cmd_buf.hdr.cmd = HDRRMAC_SHUTDOWN_CMD;
  cmd_buf.hdr.sender = sender;

  HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );
  }
}


/*===========================================================================
FUNCTION HDRRMAC_START_TX                                            EXTERNAL

DESCRIPTION
  This command starts the transmitter after a hdrrmac_activate() command,
  or restarts the transmitter if it has been stopped.

DEPENDENCIES
  If hdrrmac_stop_tx() has been called, hdrrmac_start_tx() may NOT be
  called before the TxStopped indication has been given.

PARAMETERS
  sender - The protocol giving this command (only for software trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_start_tx(hdrhai_protocol_name_enum_type sender)
{
  /* Temporary command buffer. */
  hdrrmac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_PROT( MSG_LEGACY_LOW, "hdrrmac_start_tx()" );

  cmd_buf.hdr.cmd = HDRRMAC_START_TX_CMD;
  cmd_buf.hdr.sender = sender;

  HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );
}


/*===========================================================================
FUNCTION HDRRMAC_STOP_TX                                             EXTERNAL

DESCRIPTION
  This command stops the transmitter.  If a data packet is being
  transmitted at the time the command is processed, the protocol will
  wait until that packet tranmission is complete before shutting down.
  Otherwise, the PA is shut down immediately.  A TxStopped indication
  is given once the transmitter has been shut down.

DEPENDENCIES
  None.

PARAMETERS
  sender - The protocol giving this command (only for software trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_stop_tx(hdrhai_protocol_name_enum_type sender)
{
  /* Temporary command buffer. */
  hdrrmac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_PROT( MSG_LEGACY_LOW, "hdrrmac_stop_tx()" );

  cmd_buf.hdr.cmd = HDRRMAC_STOP_TX_CMD;
  cmd_buf.hdr.sender = sender;

  HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );
}



/*===========================================================================
FUNCTION HDRRMAC_POWERUP_TX                                         EXTERNAL

DESCRIPTION
  Powers up the RF Tx hardware.  The function queues an RF Tx powerup
  command and returns immediately.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_powerup_tx
( 
#ifdef FEATURE_HDR_QTA
  boolean is_qta
#else 
  void
#endif /* FEATURE_HDR_QTA */
)
{
  /* Temporary command buffer. */
  hdrrmac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_PROT( MSG_LEGACY_LOW, "hdrrmac_powerup_tx()" );

  cmd_buf.hdr.cmd = HDRRMAC_POWERUP_TX_CMD;
  cmd_buf.hdr.sender = HDRHAI_RTC_MAC_PROTOCOL;
#ifdef FEATURE_HDR_QTA
  cmd_buf.tx_powerup.is_qta = is_qta;
#endif /* FEATURE_HDR_QTA */

  HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );

}



/*===========================================================================
FUNCTION HDRRMAC_POWERDOWN_TX                                       EXTERNAL

DESCRIPTION
  Powers down the RF Tx hardware. 

DEPENDENCIES
  None.

PARAMETERS
  powerdown_reason - reason for tx powerdown
  cb               - callback to be called once powerdown is complete

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_powerdown_tx
(
  /* reason for tx powerdown */
  hdrrmac_tx_stop_reason_enum_type powerdown_reason,

  /* callback for tx powerdown */
  hdrrmac_tx_powerdown_cb_type cb 
)
{
  /* Temporary command buffer. */
  hdrrmac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_PROT( MSG_LEGACY_MED, "hdrrmac_powerdown_tx()" );

  /* Sanity check */
  ASSERT( powerdown_reason < HDRRMAC_TX_STOP_REASON_MAX );

  cmd_buf.hdr.cmd = HDRRMAC_POWERDOWN_TX_CMD;
  cmd_buf.hdr.sender = HDRHAI_RTC_MAC_PROTOCOL;
  cmd_buf.tx_powerdown.reason = powerdown_reason;
  cmd_buf.tx_powerdown.cb = cb;

  HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );

}



#ifdef FEATURE_HDR_REVB
/*===========================================================================
FUNCTION HDRRMAC_UPDATE_RL_CARRIERS                                 EXTERNAL

DESCRIPTION
  Update the RMAC with RL carrier information from TCA.

DEPENDENCIES
  None.

PARAMETERS
  sender          - The protocol giving this command (only for software trace)
  num_carriers    - Number of RL records
  rl_carriers_ptr - Pointer to "num_carriers" instnaces of 
                    hdrrmac_rl_carrier_info_type
  frame_offset    - Frame offset in unit of slots
  dsc_gain_base   - DSC channel gain in unit of -0.5dB
  num_subasets    - Number of sub-active sets
  subaset_ptr     - Pointer to "num_subasets" instances of
                    hdrrmac_subaset_info_type
  tca_proc_cb_ptr - Pointer to function to be called once TCA is processed by 
                    RMAC.  This is only used in RevB.

RETURN VALUE
  None.

SIDE EFFECTS
  This function is NOT reentrant.
===========================================================================*/
void hdrrmac_update_rl_carriers
(
  hdrhai_protocol_name_enum_type  sender,
  uint8                           num_carriers,
  hdrrmac_rl_carrier_info_type    *rl_carrier_ptr,
  uint8                           frame_offset,
  uint8                           dsc_gain_base,
  uint8                           num_subasets,
  hdrrmac_subaset_info_type       *subaset_ptr,
  hdrrmac_tca_processed_cb_type   tca_proc_cb_ptr
)
{
  /* Temporary command buffer. */
  static hdrrmac_cmd_update_rl_carriers_struct_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_PROT( MSG_LEGACY_HIGH, "hdrrmac_update_rl_carriers()" );

  /* sanity checking */
  if ((num_carriers > HDRRMAC_MC_MAX_NUM_RL_CHANS) ||
      (num_subasets > HDRRMAC_MC_MAX_NUM_SUBASETS))
  {
    ERR_FATAL("Invalid no. of carriers %d or subasets %d",
              num_carriers,
              num_subasets,
              0);
  }

  cmd_buf.hdr.cmd = HDRRMAC_UPDATE_RL_CARRIERS_CMD;
  cmd_buf.hdr.sender = sender;
  cmd_buf.num_rls = num_carriers;
#ifdef FEATURE_MEMCPY_REMOVAL
  memscpy(&cmd_buf.rl_carrier[0], 
           sizeof(cmd_buf.rl_carrier), 
           rl_carrier_ptr, 
          (num_carriers * sizeof(hdrrmac_rl_carrier_info_type)));
#else
  memcpy(&cmd_buf.rl_carrier[0], 
         rl_carrier_ptr, 
         (num_carriers * sizeof(hdrrmac_rl_carrier_info_type)));
#endif /* FEATURE_MEMCPY_REMOVAL */

  cmd_buf.frame_offset = frame_offset;
  cmd_buf.dsc_gain_base = dsc_gain_base;
  cmd_buf.num_subasets = num_subasets;
#ifdef FEATURE_MEMCPY_REMOVAL
  memscpy(&cmd_buf.subaset_info[0],
           sizeof(cmd_buf.subaset_info), 
           subaset_ptr,
          (num_subasets * sizeof(hdrrmac_subaset_info_type)));
#else
  memcpy(&cmd_buf.subaset_info[0],
         subaset_ptr,
         (num_subasets * sizeof(hdrrmac_subaset_info_type)));
#endif /* FEATURE_MEMCPY_REMOVAL */
  cmd_buf.tca_processed_cb_ptr = tca_proc_cb_ptr;

  HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );

} /* hdrrmac_update_rl_carriers */


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMAC_GET_SLP_CARRIER_AND_TX_PILOT_PWR_INFO              EXTERNAL

DESCRIPTION
  Retrieve the current SLP carrier and the TX pilot power from all the 
  assigned reverse CDMA Channels.  This function currently is only intended
  to be called by RUP1 when RMAC4 is in use.

DEPENDENCIES
  None.

PARAMETERS
  slp_carrier_ptr - Pointer to the SLP carrier structure to be filled.
  total_tx_pilot_pwr_ptr - Pointer to the total average TX pilot power variable
                           to be filled.  Variable is in unit of 0.5dBm in 2's
                           complement format.

RETURN VALUE
  success - TRUE if function executes successfully;
            FALSE if function failes (due to wrong subtype).

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmac_get_slp_carrier_and_tx_pilot_pwr_info
(
  sys_channel_type  *slp_carrier_ptr,
  int8              *total_tx_pilot_pwr_ptr
)
{
  boolean success = FALSE;

  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
  {
    switch (hdrrmac.subtype)
    {
      case HDRSCP_DEFAULT_RTCMAC:
      case HDRSCP_SUBTYPE_1_RTCMAC:
#ifdef FEATURE_HDR_REVA_L1
      case HDRSCP_SUBTYPE_3_RTCMAC:
#endif
        break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
      case HDRSCP_SUBTYPE_4_RTCMAC:
        hdrrmac4_get_slp_carrier_and_tx_pilot_pwr_info (
          slp_carrier_ptr,
          total_tx_pilot_pwr_ptr);
        success = TRUE;
        break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */

      default:
        HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
          "Subtype %d RTCMAC not supported", 
          hdrrmac.subtype );
    }
  }

  return (success);

} /* hdrrmac_get_slp_carrier_and_tx_pilot_pwr_info */

#endif /* FEATURE_HDR_REVB */



/*===========================================================================
FUNCTION HDRRMAC_PROCESS_ACTIVATE_CMD                                INTERNAL

DESCRIPTION
  This function processes the activate command.  Before activating the
  protocol, a copy of the current AccessParameters message is retreived.  If
  the AccessParemeters message is unavailable, then the protocol is not
  activated since the RLPowerRamp parameter is required in the Setup State.

DEPENDENCIES
  None.

PARAMETERS
  Pointer to activate command

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_process_activate_cmd
(
  hdrrmac_cmd_union_type * cmd_ptr
)
{
  if ( !hdramac_is_active(HDRHAI_RTC_MAC_PROTOCOL) &&
       ( hdrrmac.state == HDRRMAC_INACTIVE_STATE ) )
  {
    #ifdef FEATURE_FACTORY_TESTMODE
    if( ftm_get_mode() != FTM_MODE )
    {
    #endif
      /* Retrieve subtype from SCP */
      hdrrmac.subtype = hdrscp_get_current_subtype(HDRHAI_RTC_MAC_PROTOCOL);
    #ifdef FEATURE_FACTORY_TESTMODE
    }
    else 
    {
      if( hdrhitmsg_get_rmac_params_ptr()->modem_fwd_rev_link_type == 0 )
      {
        hdrrmac.subtype = HDRSCP_DEFAULT_RTCMAC;

        /* Powerup-init FTM specified RMAC subtype */
        hdrrmac0_powerup_init(TRUE);
      }
      else if( hdrhitmsg_get_rmac_params_ptr()->modem_fwd_rev_link_type == 1 )
      {
        hdrrmac.subtype = HDRSCP_SUBTYPE_3_RTCMAC;

        /* Powerup-init FTM specified RMAC subtype */
        hdrrmac3_powerup_init(TRUE, &hdrrmac);
      }
      #ifdef FEATURE_HDR_REVB_SUPPORTED
      else if ( hdrhitmsg_get_rmac_params_ptr()->modem_fwd_rev_link_type == 2 )
      {
        hdrrmac.subtype = HDRSCP_SUBTYPE_4_RTCMAC;

        /* Powerup-init FTM specified RMAC subtype */
        hdrrmac4_powerup_init(TRUE, &hdrrmac);
      }
      #endif /* FEATURE_HDR_REVB_SUPPORTED */
    }
    #endif

    HDR_MSG_PROT_1( MSG_LEGACY_MED, 
                    "HDRRMAC activation Subtype = %d",
                    hdrrmac.subtype );

    hdrrmac.tx_adjust_needs_init = TRUE;
      /* First tx enable after activation should get initial tx adjust from amac */

    hdrrmac_enter_state( HDRRMAC_SETUP_STATE );

  }
  else
  {
    HDRTRACE_CMD_IGNORED_STATE( HDRHAI_RTC_MAC_PROTOCOL, (uint32) 
      HDRRMAC_ACTIVATE_CMD, cmd_ptr->hdr.sender, (uint32) hdrrmac.state );
  }
}


/*===========================================================================
FUNCTION HDRRMAC_PROCESS_DEACTIVATE_CMD                              INTERNAL

DESCRIPTION
  This function processes the deactivate command.

DEPENDENCIES
  None.

PARAMETERS
  Pointer to deactivate command

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_process_deactivate_cmd
(
  hdrrmac_cmd_union_type * cmd_ptr
)
{

  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
  {
    hdrrmac_enter_state( HDRRMAC_INACTIVE_STATE );
  }
  else
  {
    HDRTRACE_CMD_IGNORED_STATE( HDRHAI_RTC_MAC_PROTOCOL, (uint32)
      HDRRMAC_DEACTIVATE_CMD, cmd_ptr->hdr.sender, (uint32) hdrrmac.state );
  }

#ifdef FEATURE_FACTORY_TESTMODE
  if( ftm_get_mode() != FTM_MODE )
#endif
  {
     HDR_MSG_PROT( MSG_LEGACY_HIGH, 
          "RTCMAC sending deactivate complete to HDRMC");
    /* Deactivate must be called from hdrmc task */
    (void) rex_set_sigs(HDRMC_TASK_PTR, HDRMC_MAC_DEACTIVATE_COMPLETE_SIG);
  }

#ifdef FEATURE_HDR_QTA
  hdrsrch_connection_close_state( FALSE );
    /* Enable TA in HDR-SRCH after HDRRMAC deactivate complete */
#endif /* FEATURE_HDR_QTA */
}


/*===========================================================================
FUNCTION HDRRMAC_PROCESS_SHUTDOWN_CMD                                INTERNAL

DESCRIPTION
  This function processes the shutdown command.

DEPENDENCIES
  None.

PARAMETERS
  Pointer to shutdown command

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_process_shutdown_cmd
(
  hdrrmac_cmd_union_type * cmd_ptr
)
{
  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
  {
    switch (hdrrmac.subtype)
    {

      case HDRSCP_DEFAULT_RTCMAC:
      case HDRSCP_SUBTYPE_1_RTCMAC:
        hdrrmac0_shutdown();
        break;

#ifdef FEATURE_HDR_REVA_L1
      case HDRSCP_SUBTYPE_3_RTCMAC:
        hdrrmac3_shutdown();
        break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
      case HDRSCP_SUBTYPE_4_RTCMAC:
        hdrrmac4_shutdown();
        break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

      default:
        HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
          "Subtype %d RTCMAC not supported", 
          hdrrmac.subtype );
    }

    hdrrmac.tx_started = FALSE;

  }
  else
  {
    HDRTRACE_CMD_IGNORED_STATE( HDRHAI_RTC_MAC_PROTOCOL, (uint32)
      HDRRMAC_SHUTDOWN_CMD, cmd_ptr->hdr.sender, (uint32) hdrrmac.state );
  }
}



/*===========================================================================
FUNCTION HDRRMAC_PROCESS_STOP_TX_CMD                                 INTERNAL

DESCRIPTION
  Stop, or suspend, the transmitter.  If no frame TX is in progress, the
  TX will turn off immediately.  Otherwise, TX shutdown is syncronized with
  the end of the current frame.

DEPENDENCIES
  None.

PARAMETERS
  cmd_ptr - pointer to received command buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_process_stop_tx_cmd
(
  hdrrmac_cmd_union_type * cmd_ptr
)
{
  HDR_MSG_PROT( MSG_LEGACY_HIGH, "hdrrmac_process_stop_tx_cmd()" );

#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
  /* In HHO, rmac_stop_tx is called. We stop flexconn_dtx monitoring here. Be noted that   
    PA off is done in rmac3/4_stop_tx() which is next step. Disabling dtx/flexconn must be 
    done before PA off. 
  */
  hdrrmac_stop_flexconn_dtx_monitor( TRUE );
    /* Suspend RL inactive timer for HHO and pass TRUE as input param */
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/

  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
  {
    switch (hdrrmac.subtype)
    {

      case HDRSCP_DEFAULT_RTCMAC:
      case HDRSCP_SUBTYPE_1_RTCMAC:
        hdrrmac0_stop_tx();
        break;

#ifdef FEATURE_HDR_REVA_L1
      case HDRSCP_SUBTYPE_3_RTCMAC:
        hdrrmac3_stop_tx();

        /* Start stop-watch to measure HHO time */
        hdrrmac_start_stop_watch();

        /* Set the resume_from source to hard-handoff */
        hdrrmac_hho_flag = TRUE;

        break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
      case HDRSCP_SUBTYPE_4_RTCMAC:
        hdrrmac4_stop_tx();

        /* Start stop-watch to measure HHO time */
        hdrrmac_start_stop_watch();

        /* Set the resume_from source to hard-handoff */
        hdrrmac_hho_flag = TRUE;

        break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

      default:
        HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
          "Subtype %d RTCMAC not supported", 
          hdrrmac.subtype );
    }

    hdrrmac.tx_started = FALSE;

  }
  else
  {
    HDRTRACE_CMD_IGNORED_STATE( HDRHAI_RTC_MAC_PROTOCOL, (uint32)
      HDRRMAC_STOP_TX_CMD, cmd_ptr->hdr.sender, (uint32) hdrrmac.state );
  }
}


/*===========================================================================
FUNCTION HDRRMAC_PROCESS_START_TX_CMD                                INTERNAL

DESCRIPTION
  Start or resume transmission from suspension.

DEPENDENCIES
  None.

PARAMETERS
  cmd_ptr - pointer to received command buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_process_start_tx_cmd
(
  hdrrmac_cmd_union_type * cmd_ptr
)
{
#ifdef FEATURE_RF_HAS_WTR3925
  mcpm_mcvsrequest_parms_type mcvs_param = {0};  /* Used to request q6_clock_KHz */
#endif /* FEATURE_RF_HAS_WTR3925 */

  HDR_MSG_PROT( MSG_LEGACY_MED, "hdrrmac_process_start_tx_cmd()" );

  /* Check that RTC is active and not suspended. */
  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE ) 
  {

    hdrrmac.tx_started = TRUE;

#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
    /* rmac_start_tx() is called during conn setup and HHO. In conn setup we start 
       flexconn/dtx monitoring after RTC ACK (at rmac_enter_open_state). Therefore
       we only need to start  flexconn/dtx monitoring for HHO here. Since we will wait
       the fl inactive timer timeout then enable flexconn/dtx, there should be enough
       time for PA on before enabling flexconn/dtx.
    */
    if ( hdrrmac_hho_flag )
    {
      hdrrmac_start_flexconn_dtx_monitor( TRUE );
        /* Resume RL inactive timer after HHO */
    }
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/

    if (!hdrrmac.link_is_suspended)
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "Waiting for Tx enable complete..." );

      #ifdef FEATURE_PA_ACCESS_PROBE_CONFIG      
      if (hdrrmac.rf_status == TX_OFF)
      {
        rfm_hdr_set_pa_access_probe_config(hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN) ,TRUE);
      }
      #endif

      /* Enable Tx */
      hdrrmac.rf_status = hdrtx_enable_tx(hdrrmac_tx_enabled_cb);

#ifdef FEATURE_RF_HAS_WTR3925
      /* Vote for SVS Q6 clk (480000 KHz) speed for C2K FBRx optimizations */
      mcvs_param.mcvs_req_type = MCVS_FULL_REQUEST;
      mcvs_param.mcvs_update_info = MCPM_MCVS_Q6_CLK_UPDATE;
      mcvs_param.q6_clock_KHz = 480000; 

      MCPM_MCVSConfig_Modem(MCPM_DO_TECH, &mcvs_param);
#endif /* FEATURE_RF_HAS_WTR3925 */
    }
    else
    {
      HDR_MSG_PROT(MSG_LEGACY_MED, "Start Tx while link suspended");
    }
    
  }
  else
  {
    HDRTRACE_CMD_IGNORED_STATE( HDRHAI_RTC_MAC_PROTOCOL, (uint32)
      HDRRMAC_START_TX_CMD, cmd_ptr->hdr.sender, (uint32) hdrrmac.state );
  }
  
}



/*===========================================================================
FUNCTION HDRRMAC_PROCESS_TX_POWERUP_CMD                             INTERNAL

DESCRIPTION
  Powers up the RF Tx chain.  RTC MAC will not use the reverse link until
  hdrrmac_tx_enabled_cb is called..

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_process_powerup_tx_cmd
(
#ifdef FEATURE_HDR_QTA
  hdrrmac_cmd_powerup_tx_struct_type *cmd_ptr
#else
  void
#endif /* FEATURE_HDR_QTA */
)
{

  HDR_MSG_PROT( MSG_LEGACY_HIGH, "hdrrmac_process_powerup_tx_cmd()" );

  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
  {
    if (hdrrmac.tx_started)
    {
      #ifdef FEATURE_PA_ACCESS_PROBE_CONFIG
      if (hdrrmac.rf_status == TX_OFF)
      {
        rfm_hdr_set_pa_access_probe_config(hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN) ,TRUE);
      }
      #endif

#ifdef FEATURE_HDR_QTA
      if ( cmd_ptr->is_qta )
      {
        /* Tx Enabled by FW hdrrmac_tx_enable_cb will be called when RF Tx is ready. */
        hdrrmac.rf_status = hdrtx_pseudo_enable_tx(hdrrmac_tx_enabled_cb);

      }
      else
#endif /* FEATURE_HDR_QTA */
      {
        /* Enable Tx. hdrrmac_tx_enable_cb will be called when RF Tx is ready. */
        hdrrmac.rf_status = hdrtx_enable_tx(hdrrmac_tx_enabled_cb);
      }
    }
    else
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "RMAC not started - not powering up" );
    }
  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "RMAC not active" );
  }
}



/*===========================================================================
FUNCTION HDRRMAC_PROCESS_POWERDOWN_TX                               INTERNAL

DESCRIPTION
  Powers down the RF Tx hardware.
           
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_process_powerdown_tx_cmd
(
  hdrrmac_cmd_union_type * cmd_ptr
)
{
  HDR_MSG_PROT( MSG_LEGACY_HIGH, "hdrrmac_process_powerdown_tx_cmd()" );

#ifdef FEATURE_HDR_QTA

  boolean is_qta;

  if ( cmd_ptr->tx_powerdown.reason == HDRRMAC_TX_STOP_FOR_TUNEAWAY_QTA )
  {
    is_qta= TRUE;
  }
  else
  {
    is_qta= FALSE;
  }
  HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "HDR QTA is_qta %d", is_qta );

#endif /* FEATURE_HDR_QTA */
  

  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
  {
    switch (hdrrmac.subtype)
    {

      case HDRSCP_DEFAULT_RTCMAC:
      case HDRSCP_SUBTYPE_1_RTCMAC:
        hdrrmac0_powerdown_tx(
           #ifdef FEATURE_HDR_QTA
           is_qta
           #endif /* FEATURE_HDR_QTA */
           );
        break;

#ifdef FEATURE_HDR_REVA_L1
      case HDRSCP_SUBTYPE_3_RTCMAC:
        hdrrmac3_powerdown_tx(
           #ifdef FEATURE_HDR_QTA
           is_qta
           #endif /* FEATURE_HDR_QTA */
           );
        break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
      case HDRSCP_SUBTYPE_4_RTCMAC:
        hdrrmac4_powerdown_tx(
           #ifdef FEATURE_HDR_QTA
           is_qta
           #endif /* FEATURE_HDR_QTA */
           );
        break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

      default:
        HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
          "This function not supported for subtype %d", 
          hdrrmac.subtype );
    }

#ifdef FEATURE_HDR_QTA
    if ( cmd_ptr->tx_powerdown.reason == HDRRMAC_TX_STOP_FOR_TUNEAWAY_QTA )
    {
      /* Deactivate RF Tx */
      hdrrmac.rf_status = hdrtx_disable_tx( TRUE );
    }
    else
#endif /* FEATURE_HDR_QTA */
    {
    /* Trigger Tx DAC stop */
    hdrmod_tx_stop();

    /* Deactivate RF Tx */
    hdrrmac.rf_status = hdrtx_disable_tx( FALSE );
    }

    /* Reset RevLinkCarrier override flag */
    hdrrmac_override_rev_link_carrier_set( FALSE, 
                                           NULL, 
                                           0 );
  }

  cmd_ptr->tx_powerdown.cb();
    /* Callback */

#ifdef FEATURE_HDR_QTA
  if ( cmd_ptr->tx_powerdown.reason != HDRRMAC_TX_STOP_FOR_TUNEAWAY_QTA )
  {
    hdrrmac_process_cmd_on_tune_back();
  }
#endif
}


#ifdef FEATURE_HDR_REVB
/*===========================================================================
FUNCTION HDRRMAC_PROCESS_UPDATE_RL_CARRIERS_CMD                      INTERNAL

DESCRIPTION
  This function dispatches the HDRRMAC_UPDATE_RL_CARRIERS_CMD to the 
  appropriate subtype processing function.
           
DEPENDENCIES
  None.

PARAMETERS
  update_rl_carriers_ptr  - Pointer to the HDRRMAC_UPDATE_RL_CARRIERS_CMD
                            command buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void hdrrmac_process_update_rl_carriers_cmd
(
  hdrrmac_cmd_union_type *cmd_ptr
)
{
#ifdef FEATURE_HDR_REVB_SUPPORTED
  sys_channel_type chan_list[HDRRMAC_MC_MAX_NUM_RL_CHANS];
  uint8            i;
#endif /* FEATURE_HDR_REVB_SUPPORTED */

  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE)
  {
    switch (hdrrmac.subtype)
    {
      case HDRSCP_DEFAULT_RMAC:
      case HDRSCP_SUBTYPE_1_RTCMAC:
      case HDRSCP_SUBTYPE_3_RTCMAC:
        HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
          "Rcv'd update_rl_carriers in subtype %d", 
          hdrrmac.subtype );
        break;
  
#ifdef FEATURE_HDR_REVB_SUPPORTED
      case HDRSCP_SUBTYPE_4_RTCMAC:
        hdrrmac4_update_rl_carriers(cmd_ptr);

        if ( hdrrmac_rev_link_carrier_override )
        {
          /* Build a chanlist and sent override */
          for ( i = 0; i < cmd_ptr->update_rl_carriers.num_rls; i++)
          {
            chan_list[i] = 
                cmd_ptr->update_rl_carriers.rl_carrier[i].channel_record;
          }

          hdrrmac_override_rev_link_carrier_set( FALSE, 
                                                 chan_list, 
                                                 cmd_ptr->update_rl_carriers.num_rls );
        }
        break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
  
      default:
        HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
          "Subtype %d RTCMAC not supported", 
          hdrrmac.subtype );
    }
  }
  else
  {
    HDRTRACE_CMD_IGNORED_STATE( HDRHAI_RTC_MAC_PROTOCOL, (uint32)
      HDRRMAC_UPDATE_RL_CARRIERS_CMD, cmd_ptr->hdr.sender, 
      (uint32) hdrrmac.state );
  }
}
#endif /* FEATURE_HDR_REVB */


/*===========================================================================
FUNCTION HDRRMAC_PROCESS_ENTER_DRC_SUPERVISION_CMD                  INTERNAL

DESCRIPTION
  This function dispatches the ENTER_DRC_SUPERVISION_CMD to the 
  appropriate subtype processing function.
           
DEPENDENCIES
  None.

PARAMETERS
  cmd_ptr  - Pointer to the HDRRMAC_ENTER_DRC_SUPERVISION_CMD
             command buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void hdrrmac_process_enter_drc_supervision_cmd
(
  hdrrmac_cmd_union_type *cmd_ptr
)
{
  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE)
  {
    switch (hdrrmac.subtype)
    {
      case HDRSCP_DEFAULT_RMAC:
      case HDRSCP_SUBTYPE_1_RTCMAC:
        hdrrmac0_enter_drc_supervision();
        break;
  
      case HDRSCP_SUBTYPE_3_RTCMAC:
        hdrrmac3_enter_drc_supervision();
        break;
    
#ifdef FEATURE_HDR_REVB_SUPPORTED
      case HDRSCP_SUBTYPE_4_RTCMAC:
        hdrrmac4_enter_drc_supervision(cmd_ptr);
          break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
    
      default:
        HDR_MSG_PROT_1(MSG_LEGACY_FATAL, 
          "Subtype %d RTCMAC not supported", 
          hdrrmac.subtype );
    }
  }
  else
  {
    HDRTRACE_CMD_IGNORED_STATE( HDRHAI_RTC_MAC_PROTOCOL, (uint32)
      HDRRMAC_ENTER_DRC_SUPERVISION_CMD, cmd_ptr->hdr.sender, 
      (uint32) hdrrmac.state );
  }

} /* hdrrmac_process_enter_drc_supervision_cmd() */


/*===========================================================================
FUNCTION HDRRMAC_PROCESS_EXIT_DRC_SUPERVISION_CMD                   INTERNAL

DESCRIPTION
  This function dispatches the EXIT_DRC_SUPERVISION_CMD to the 
  appropriate subtype processing function.
           
DEPENDENCIES
  None.

PARAMETERS
  cmd_ptr  - Pointer to the HDRRMAC_EXIT_DRC_SUPERVISION_CMD
             command buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void hdrrmac_process_exit_drc_supervision_cmd
(
  hdrrmac_cmd_union_type *cmd_ptr
)
{
  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE)
  {
    switch (hdrrmac.subtype)
    {
      case HDRSCP_DEFAULT_RMAC:
      case HDRSCP_SUBTYPE_1_RTCMAC:
        hdrrmac0_exit_drc_supervision();
        break;
  
      case HDRSCP_SUBTYPE_3_RTCMAC:
        hdrrmac3_exit_drc_supervision();
        break;
    
#ifdef FEATURE_HDR_REVB_SUPPORTED
      case HDRSCP_SUBTYPE_4_RTCMAC:
        hdrrmac4_exit_drc_supervision(cmd_ptr);
          break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
   
    
      default:
        HDR_MSG_PROT_1(MSG_LEGACY_FATAL, 
          "Subtype %d RTCMAC not supported", 
          hdrrmac.subtype );
    }
  }
  else
  {
    HDRTRACE_CMD_IGNORED_STATE( HDRHAI_RTC_MAC_PROTOCOL, (uint32)
      HDRRMAC_EXIT_DRC_SUPERVISION_CMD, cmd_ptr->hdr.sender, 
      (uint32) hdrrmac.state );
  }

} /* hdrrmac_process_exit_drc_supervision_cmd() */

#ifdef FEATURE_HDR_THERMAL_BACKOFF
/*===========================================================================

FUNCTION HDRRMAC_PROCESS_CONTROL_THERMAL_PA_BACKOFF_CMD                EXTERNAL

DESCRIPTION
  This interface processes flow control command from CCM. 
  
DEPENDENCIES
  None.

PARAMETERS
  cmd - Flow control command to be processed.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

void hdrrmac_process_control_thermal_pa_backoff_cmd
(
  hdrrmac_cmd_union_type *cmd_ptr
)
{
  /* step timer */
  uint16 temp_timer;

  hdrrmac_cmd_thermal_pa_backoff_struct_type *thermal_power_bo;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_PROT(MSG_LEGACY_HIGH, " Processing thermal backoff cmd in RMAC");

  /* If the AT is in traffic */
  if ( hdrrmac_is_active(HDRHAI_RTC_MAC_PROTOCOL) ) 
  {
    /* Entering critical section... */
    rex_enter_crit_sect(&hdrrmac.thermal_power_bo_crit_sect);

    thermal_power_bo = &cmd_ptr->thermal_pa_backoff;
    hdrrmac.thermal_power_bo_cmd = thermal_power_bo->cmd;

   
    /* Stop temp_timer if active */
    rex_clr_timer(&hdrrmac.thermal_power_bo_step_down_timer);
    rex_clr_timer(&hdrrmac.thermal_power_bo_step_up_timer);

    /* Stop backoff timer */
    rex_clr_timer(&hdrrmac.thermal_power_bo_backoff_timer);
    rex_clr_timer(&hdrrmac.thermal_power_bo_non_backoff_timer);
  

    switch(hdrrmac.thermal_power_bo_cmd)
    {
      case DS707_CCM_FLOW_SHUT_DOWN:

        hdrrmac.thermal_power_bo_is_on = TRUE;

        hdrrmac.thermal_power_bo_pattern = HDRRMAC_TOTAL_BACKOFF_PATTERN;

        HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Thermal Backoff cmd: FLOW_SHUT_DOWN,"
                                          " setting pattern  %d",
                                          hdrrmac.thermal_power_bo_pattern);

        /* Program mDSP with the initial pattern */
        hdrrmac_set_thermal_power_bo_pattern(hdrrmac.thermal_power_bo_pattern);
        break;

      case DS707_CCM_FLOW_DOWN_MORE:

        /* If the first down command set to default pattern*/
        if (!hdrrmac.thermal_power_bo_is_on)
        {
          hdrrmac.thermal_power_bo_is_on = TRUE;

          /* pattern is default pattern */
          hdrrmac.thermal_power_bo_pattern = 
            hdrrmac.thermal_power_bo_params.default_tx_power_bo_pattern_number;

          temp_timer = hdrrmac.thermal_power_bo_params.step_down_timer_ms +
            hdrrmac.thermal_power_bo_params.delay_time_ms;

          rex_set_timer( &hdrrmac.thermal_power_bo_step_down_timer, temp_timer );
        }
        else
        {
          hdrrmac.thermal_power_bo_pattern = HDRRMAC_THERMAL_POWER_BO_PATTERN_INC(
                                             hdrrmac.thermal_power_bo_pattern,
                                             hdrrmac.thermal_power_bo_params.max_tx_power_bo_pattern_number);

          /* If we are in total backoff no need to start timer */
          if (hdrrmac.thermal_power_bo_pattern < HDRRMAC_TOTAL_BACKOFF_PATTERN)
          {
            /* Restart the step timer to enable periodic flow decrease */
            temp_timer = hdrrmac.thermal_power_bo_params.step_down_timer_ms +
              hdrrmac.thermal_power_bo_params.delay_time_ms;

            rex_set_timer( &hdrrmac.thermal_power_bo_step_down_timer, temp_timer );
          }
            
        }

        HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Thermal Backoff cmd: FLOW_DOWN_MORE,"
                                        " setting pattern  %d"
                      ,hdrrmac.thermal_power_bo_pattern);

        /* Program rf with the power backoff pattern */
        hdrrmac_set_thermal_power_bo_pattern(hdrrmac.thermal_power_bo_pattern);
        break;

      case DS707_CCM_FLOW_OFF:

        hdrrmac.thermal_power_bo_pattern = HDRRMAC_THERMAL_POWER_BO_PATTERN_DEC(
                                            hdrrmac.thermal_power_bo_pattern,
                                            HDRRMAC_FIRST_BACKOFF_PATTERN);

        HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Thermal Backoff cmd: FLOW_OFF,"
                                        " setting pattern  %d"
                       , hdrrmac.thermal_power_bo_pattern);


        if (hdrrmac.thermal_power_bo_pattern > HDRRMAC_NO_BACKOFF_PATTERN)
        {
          /* Restart the step timer to enable periodic flow decrease */
          temp_timer = hdrrmac.thermal_power_bo_params.step_up_timer_ms
             + hdrrmac.thermal_power_bo_params.delay_time_ms;

          rex_set_timer( &hdrrmac.thermal_power_bo_step_up_timer, temp_timer );
        }
        else
        {
          hdrrmac.thermal_power_bo_is_on = FALSE;
        }
        /* Program rf with the power backoff pattern */
        hdrrmac_set_thermal_power_bo_pattern(hdrrmac.thermal_power_bo_pattern);

        break;

        default:
          HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Unexpected Thermal Backoff cmd: %d", hdrrmac.thermal_power_bo_cmd);
      }

      HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Thermal Backoff Cmd: %d", hdrrmac.thermal_power_bo_cmd);
    
    
    /* Leave Critical section */
    rex_leave_crit_sect(&hdrrmac.thermal_power_bo_crit_sect);

  } /* If the AT is in traffic */
  else
  {
    HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Thermal Backoff Cmd: %d, ignored RMAC is not active",
                    hdrrmac.thermal_power_bo_cmd);
  }

} /* hdrrmac_process_control_thermal_pa_backoff_cmd */

#endif /*FEATURE_HDR_THERMAL_BACKOFF*/

/*===========================================================================
FUNCTION HDRRMAC_PROCESS_CMD                                         EXTERNAL

DESCRIPTION
  This function processes the commands for this protocol that were queued
  on the task command queue from a different task context.

DEPENDENCIES
  None.

PARAMETERS
  cmd_ptr - pointer to received command buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_process_cmd
(
  hdrrmac_cmd_union_type * cmd_ptr
)
{
  HDRTRACE_CMD( HDRHAI_RTC_MAC_PROTOCOL, (uint32) cmd_ptr->hdr.cmd,
    cmd_ptr->hdr.sender, (uint32) hdrrmac.state );

  switch ( cmd_ptr->hdr.cmd )
  {
    case HDRRMAC_ACTIVATE_CMD:
      hdrrmac_process_activate_cmd(cmd_ptr);
      break;

    case HDRRMAC_DEACTIVATE_CMD:
      hdrrmac_process_deactivate_cmd(cmd_ptr);
      break;

    case HDRRMAC_STOP_TX_CMD:
      hdrrmac_process_stop_tx_cmd(cmd_ptr);
      break;

    case HDRRMAC_START_TX_CMD:
      hdrrmac_process_start_tx_cmd(cmd_ptr);
      break;

    case HDRRMAC_SHUTDOWN_CMD:
      hdrrmac_process_shutdown_cmd(cmd_ptr);
      break;

    case HDRRMAC_POWERUP_TX_CMD:
      hdrrmac_process_powerup_tx_cmd(
#ifdef FEATURE_HDR_QTA
      (hdrrmac_cmd_powerup_tx_struct_type*) cmd_ptr
#endif /* FEATURE_HDR_QTA */
);
      break;

    case HDRRMAC_POWERDOWN_TX_CMD:
      hdrrmac_process_powerdown_tx_cmd(cmd_ptr);
      break;

#ifdef FEATURE_HDR_REVB
    case HDRRMAC_UPDATE_RL_CARRIERS_CMD:
      hdrrmac_process_update_rl_carriers_cmd(cmd_ptr);
      break;
#endif /* FEATURE_HDR_REVB */

    case HDRRMAC_ENTER_DRC_SUPERVISION_CMD:
      hdrrmac_process_enter_drc_supervision_cmd(cmd_ptr);
      break;
  
    case HDRRMAC_EXIT_DRC_SUPERVISION_CMD:
      hdrrmac_process_exit_drc_supervision_cmd(cmd_ptr);
      break;

    case HDRRMAC_CONFIGURE:
#ifdef FEATURE_HDR_REVA_L1
      (void) hdrrmac3_configure();
#endif /* FEATURE_HDR_REVA_L1 */
      break;

#ifdef FEATURE_HDR_THERMAL_BACKOFF
    case HDRRMAC_CTRL_PA_BACKOFF_CMD:
      hdrrmac_process_control_thermal_pa_backoff_cmd(cmd_ptr);
      break;
#endif

    case HDRRMAC_NOTIFY_SUSPEND_CMD:
      hdrrmac_notify_suspend( cmd_ptr->notify_suspend.slots );
      break;

    case HDRRMAC_SUSPEND_CMD:
      hdrrmac_suspend_connection();
      break;

    case HDRRMAC_RESUME_CMD:
      hdrrmac_resume_connection( cmd_ptr->resume.ms
#ifdef FEATURE_HDR_QTA
      , cmd_ptr->resume.is_qta
#endif /* FEATURE_HDR_QTA */
);
      break;

#ifdef FEATURE_HDR_FLEX_CONN
    case HDRRMAC_SET_FLEXCONN_CMD:
      hdrrmac_set_flexconn_mode( cmd_ptr->set_flexconn.flexconn_mode );
      break;
#endif /* FEATURE_HDR_FLEX_CONN */

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
    case HDRRMAC_SET_DTX_CMD:
      hdrrmac_set_device_only_dtx_mode( cmd_ptr->set_dtx.dtx_mode );
      break;
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */

#ifdef FEATURE_HDR_MODEM_API
    case HDRRMAC_REPORT_COST_OF_MODEM_POWER:
      hdrrmac_process_report_cost_of_modem_power_est_cmd();
      break;
#endif /* FEATURE_HDR_MODEM_API */

    default:
      HDR_MSG_PROT_1(MSG_LEGACY_ERROR, "Unknown cmd! %d ", cmd_ptr->hdr.cmd);
  }
}


/*===========================================================================
FUNCTION HDRRMAC_PROCESS_SIG                                         EXTERNAL

DESCRIPTION
  This function handles the task signals assigned to this protocol.

DEPENDENCIES
  None.

PARAMETERS
  signal - task signal to be processed.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_process_sig
(
  rex_sigs_type signal
)
{
  switch (hdrrmac.subtype)
  {

    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      hdrrmac0_process_sig(signal);
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      hdrrmac3_process_sig(signal);
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      hdrrmac4_process_sig(signal);
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
        HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
          "This function not supported for subtype %d", 
          hdrrmac.subtype );

  }
}



/*===========================================================================
FUNCTION HDRRMAC_PROCESS_MSG                                         EXTERNAL

DESCRIPTION
  This function processes incoming messages for this protocol.

DEPENDENCIES
  None.

PARAMETERS
  rcv_msg_ptr - pointer to receive message buffer.
  protocol_instance - Instance of the protocol the message is for

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_process_msg
(
  dsm_item_type * rcv_msg_ptr,
  hdrhai_protocol_instance_enum_type protocol_instance
)
{
  switch (hdrrmac.subtype)
  {

    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      hdrrmac0_process_msg(rcv_msg_ptr, protocol_instance);
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      hdrrmac3_process_msg(rcv_msg_ptr, protocol_instance);
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      hdrrmac4_process_msg(rcv_msg_ptr, protocol_instance);
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );

  }
}


/*===========================================================================
FUNCTION HDRRMAC_PROCESS_TIMER                                       EXTERNAL

DESCRIPTION
  This function processes timer expiry event for this protocol.

DEPENDENCIES
  None.

PARAMETERS
  timer_id: Timer Id

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_process_timer(uint32 timer_id)
{
  switch (hdrrmac.subtype)
  {
    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
    case HDRSCP_SUBTYPE_3_RTCMAC:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "no timer support in RMAC st %d", 
        hdrrmac.subtype );
      break;
#ifdef FEATURE_HDR_REVB_SUPPORTED  
    case HDRSCP_SUBTYPE_4_RTCMAC:
      hdrrmac4_process_timer(timer_id);
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
  
    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );
  }
} /* hdrrmac_process_timer */


/*===========================================================================
FUNCTION HDRRMAC_MSG_CB                                              EXTERNAL

DESCRIPTION
  This callback function queues incoming messages for this protocol.

DEPENDENCIES
  None.

PARAMETERS
  rcv_msg_ptr - pointer to receive message buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_msg_cb
(
  dsm_item_type * msg_ptr
)
{
  HDRRMAC_QUEUE_MSG( HDRHAI_RTC_MAC_PROTOCOL, HDRHAI_IN_USE, msg_ptr );
}



/*===========================================================================
FUNCTION HDRRMAC_CONFIG_MSG_CB                                       EXTERNAL

DESCRIPTION
  This callback function queues incoming messages for the in config instance
  of this protocol.

DEPENDENCIES
  None.

PARAMETERS
  rcv_msg_ptr - pointer to receive message buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_config_msg_cb
(
  dsm_item_type * msg_ptr
)
{
  HDRRMAC_QUEUE_MSG( HDRHAI_RTC_MAC_PROTOCOL, HDRHAI_IN_CONFIG, msg_ptr );
}


/* EJECT */
/*===========================================================================

FUNCTION HDRRMAC_PROCESS_IND

DESCRIPTION
  This function processes indications given to AMAC.

DEPENDENCIES
  None.

PARAMETERS
  ind_name - Name of the indication for AMAC to process
  ind_data_ptr - pointer to data associated with the indication

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrrmac_process_ind
(
  hdrind_ind_name_enum_type ind_name,
  hdrind_ind_data_union_type * ind_data_ptr
)
{
  hdrrmac.subtype = hdrscp_get_current_subtype(HDRHAI_RTC_MAC_PROTOCOL);

  switch (hdrrmac.subtype)
  {

    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      hdrrmac0_process_ind(ind_name, ind_data_ptr);
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      hdrrmac3_process_ind(ind_name, ind_data_ptr);
      break;
#endif /* FEATURE_HDR_REVA_L1 */

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      hdrrmac4_process_ind(ind_name, ind_data_ptr);
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );
  }

} /* hdrrmac_process_ind */

/*===========================================================================
FUNCTION HDRRMAC_IND_CB                                              EXTERNAL

DESCRIPTION
  This function processes incoming indications for this protocol.

DEPENDENCIES
  None.

PARAMETERS
  ind_name     - Name of the indication from hdrhai.h
  ind_data_ptr - Pointer to any indication-specific data

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_ind_cb
(
  hdrind_ind_name_enum_type ind_name,
  void *ind_data
)
{

  HDR_MSG_PROT_1( MSG_LEGACY_LOW, "hdrrmac_ind_cb %d", ind_name );

  if (rex_self () == HDRRMAC_TASK_PTR)
  {
    /* Indication is from another protocol in the same task, so consider it
     * an internal indication
     */
    hdrrmac_process_ind (ind_name, (hdrind_ind_data_union_type *)ind_data);
  }
  else
  {
    HDRRMAC_QUEUE_IND(HDRHAI_RTC_MAC_PROTOCOL, ind_name, ind_data);
  }
} /* hdrrmac_ind_cb */


/*===========================================================================

FUNCTION HDRRMAC_NOTIFY_SUSPEND

DESCRIPTION
  This function tells the RTC MAC about an upcoming tune away.
    
DEPENDENCIES
  None.

PARAMETERS
  slots_to_suspend - number of slots until suspension (tuneaway) is 
                     scheduled.
    
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrrmac_notify_suspend
(
  int slots_until_suspend
)
{
  /* Temporary command buffer. */
  hdrrmac_cmd_notify_suspend_struct_type cmd_buf;

  /* current time in frames and slots */
  qword   current_time_frames;
  uint32  current_slot_in_frame;

  /* TRUE is scheduled suspend is to be cancelled. */
  boolean cancel_suspend = FALSE;

  /* Slot when tuneaway is scheduled. */
  uint32  suspend_slot = 0;

  if( rex_self() != HDRRMAC_TASK_PTR )
  {
    /* Post command to perform suspend notification processing in HDRTX 
       context */

    memset ( &cmd_buf, 0, sizeof (hdrrmac_cmd_notify_suspend_struct_type) );

    cmd_buf.hdr.cmd    = HDRRMAC_NOTIFY_SUSPEND_CMD;
    cmd_buf.hdr.sender = HDRHAI_RTC_MAC_PROTOCOL;
    cmd_buf.slots      = slots_until_suspend;

    HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );

    HDR_MSG_PROT_1( MSG_LEGACY_MED, 
      "Queued notify suspend(0x%x)",
      slots_until_suspend );
  }
  else
  {
    if( slots_until_suspend == HDRRMAC_CANCEL_SUSPEND )
    {
      cancel_suspend = TRUE;
    }
    else
    {
      /* Get the current time in units of frames. Slot is the remainder, 
         so it indicates the slot we are on within the frame. */
      hdrts_get_cycle_and_slot( current_time_frames, 
                                &current_slot_in_frame, 
                                HDRTS_SLOTS_PER_FRAME );

      /* Tuneaway slot
         = Current slot + ramp down slots
         = Current frame time*16 + current slot within frame + 
           ramp down slots */
      qw_shift( current_time_frames, 4 );
      suspend_slot = qw_lo( current_time_frames ) + current_slot_in_frame + 
        slots_until_suspend;
    }

    /* Perform suspend notification processing */

    if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
    {
      switch (hdrrmac.subtype)
      {
  
#ifdef FEATURE_HDR_REVA_L1
        case HDRSCP_SUBTYPE_3_RTCMAC:
          hdrrmac3_notify_suspend( cancel_suspend, suspend_slot ); 
  
          break;
  
#ifdef FEATURE_HDR_REVB_SUPPORTED
        case HDRSCP_SUBTYPE_4_RTCMAC:
          hdrrmac4_notify_suspend( cancel_suspend, suspend_slot );
          break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */
  
        case HDRSCP_DEFAULT_RTCMAC:
        case HDRSCP_SUBTYPE_1_RTCMAC:
  
          /* Not needed for default RTC MAC.  No need to print F3. */
          break;
  
        default:
          HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
            "This function not supported for subtype %d", 
            hdrrmac.subtype );
      }

    } /* If RMAC is not in inactive state */

  } /* If this function is called within HDRTX task context */


}


/*===========================================================================
FUNCTION HDRRMAC_SUSPEND_CONNECTION                                 EXTERNAL

DESCRIPTION
  Suspends an HDR traffic connection.  The RTC MAC will not ask for more
  data from PCP until hdrrmac_resume_connection() is called.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_suspend_connection( void )
{

  /* Temporary command buffer. */
  hdrrmac_cmd_union_type cmd_buf;

  if( rex_self() != HDRRMAC_TASK_PTR )
  {
    /* Post command to perform suspend connection processing in HDRTX 
       context */

    memset ( &cmd_buf, 0, sizeof (hdrrmac_cmd_union_type ) );

    cmd_buf.hdr.cmd    = HDRRMAC_SUSPEND_CMD;
    cmd_buf.hdr.sender = HDRHAI_RTC_MAC_PROTOCOL;

    HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );
  }
  else
  {
    /* Perform suspend connection processing */
  
#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
    /* In OFS or traffic tune away, rmac_suspend_conn is called. We stop flexconn/dtx 
       monitoring here. PA off is done at rmac_powerdown_tx(). It is called after
       rmac_suspend_conn(). Therefore we satisfy fw requirements of disabling flexconn/fc
       before PA off.
    */
    hdrrmac_stop_flexconn_dtx_monitor( TRUE );
      /* Suspend RL inactive timer when OFS or tune away , pass TRUE to suspend timer */
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/

    if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
    {
      /* Informational message only. */
      if (hdrrmac.tx_started != TRUE)
      {
        HDR_MSG_PROT( MSG_LEGACY_HIGH, "Suspending when RMAC is already stopped"); 
      }

      /* Change link suspend state, and suspend connection */
      hdrrmac.link_is_suspended = TRUE;
  
      switch (hdrrmac.subtype)
      {
  
        case HDRSCP_DEFAULT_RTCMAC:
        case HDRSCP_SUBTYPE_1_RTCMAC:
          
          hdrrmac0_suspend_connection();
  
          break;
  
  #ifdef FEATURE_HDR_REVA_L1
        case HDRSCP_SUBTYPE_3_RTCMAC:
          hdrrmac3_suspend_connection(); 
          break;
  
  #ifdef FEATURE_HDR_REVB_SUPPORTED
        case HDRSCP_SUBTYPE_4_RTCMAC:
          hdrrmac4_suspend_connection();
          break;
  #endif /* FEATURE_HDR_REVB_SUPPORTED */
  #endif /* FEATURE_HDR_REVA_L1 */
  
        default:
          HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
            "This function not supported for subtype %d", 
            hdrrmac.subtype );
      }

    } /* If RMAC is not in inactive state */

    hdrfmac_fac_gain_check_and_suspend();
      /* Check if FAC gain calculation slot interrupt should be suspended */

  } /* If this function is called within HDRTX task context */


}

/*===========================================================================
FUNCTION HDRRMAC_RESUME_CONNECTION                                 EXTERNAL

DESCRIPTION
  Resume an HDR traffic connection which was suspended by
  hdrrmac_suspend_connection().

DEPENDENCIES
  None.

PARAMETERS
  ms - mS that connection was suspended

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_resume_connection
(
  int ms  /* time, in mS, that connection was suspended. */
#ifdef FEATURE_HDR_QTA
  ,boolean is_qta
#endif /* FEATURE_HDR_QTA */
)
{
#ifdef FEATURE_HDR_QTA
  hdrrmac_cmd_powerup_tx_struct_type tx_pu;
#endif /* FEATURE_HDR_QTA */

  /* Temporary command buffer. */
  hdrrmac_cmd_resume_struct_type cmd_buf;

  if( rex_self() != HDRRMAC_TASK_PTR )
  {
    /* Post command to process resume connection in HDRTX context */

    memset ( &cmd_buf, 0, sizeof (hdrrmac_cmd_resume_struct_type ) );

    cmd_buf.hdr.cmd    = HDRRMAC_RESUME_CMD;
    cmd_buf.hdr.sender = HDRHAI_RTC_MAC_PROTOCOL;
    cmd_buf.ms         = ms;
#ifdef FEATURE_HDR_QTA
    cmd_buf.is_qta     = is_qta;
#endif /* FEATURE_HDR_QTA */

    HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );
  }
  else
  {
    /* Do resume connection processing */
  
    /* Make sure RTC MAC is active before attempting to resume. */
    if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
    {
    
#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
      /* When OFS or traffic tune away are done, rmac_resume is called. Need to
         start flexconn/dtx monitoring here.
      */
      hdrrmac_start_flexconn_dtx_monitor( TRUE );
        /* Resume RL inactive timer after tune away or OFS. Pass TRUE for resume. */
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/

      /* Change link suspend state, and resume connection */
      hdrrmac.link_is_suspended = FALSE;

      /* Only resume if link is also started */
      if (hdrrmac.tx_started)
      { 
  
        switch (hdrrmac.subtype)
        {
  
          case HDRSCP_DEFAULT_RTCMAC:
          case HDRSCP_SUBTYPE_1_RTCMAC:
    
            hdrrmac0_resume_connection(ms);
            break;
  
#ifdef FEATURE_HDR_REVA_L1
          case HDRSCP_SUBTYPE_3_RTCMAC:
            hdrrmac3_resume_connection((dword)ms, FALSE);
            break;
    
#ifdef FEATURE_HDR_REVB_SUPPORTED
          case HDRSCP_SUBTYPE_4_RTCMAC:
            hdrrmac4_resume_connection((dword)ms, FALSE);
            break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */
  
          default:
            HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
              "This function not supported for subtype %d", 
              hdrrmac.subtype );
  
        }
    
        /* Start RTC MAC once Tx is enabled.
           If already enabled, it will start immediately. */

#ifdef FEATURE_HDR_QTA
        tx_pu.is_qta = is_qta;
#endif /* FEATURE_HDR_QTA */
        hdrrmac_process_powerup_tx_cmd( 
#ifdef FEATURE_HDR_QTA
        &tx_pu 
#endif /* FEATURE_HDR_QTA */
);
      }

    } /* If RMAC is not in inactive state */

    hdrfmac_fac_gain_check_and_resume();
      /* Check if FAC gain calculation slot interrupt should be resumed */

#ifdef FEATURE_HDR_QTA
    hdrrmac_process_cmd_on_tune_back();
      /* Post shutdown/deactivate cmd  if HDR they are pending */ 
#endif
  } /* If this function is called within HDRTX task context */
}

/*===========================================================================
FUNCTION HDRRMAC_ENABLE_RATE_INERTIA                                EXTERNAL

DESCRIPTION
  Enables HDR Reverse Link Rate inertia.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_enable_rate_inertia( void )
{
  if(hdrmc_feature_is_enabled( HDRMC_FEATURE_HDR_RL_RATE_INERTIA))
    hdrrmac0_enable_rate_inertia();
}

/*===========================================================================
FUNCTION HDRRMAC_DISABLE_RATE_INERTIA                               EXTERNAL

DESCRIPTION
  Disables HDR Reverse Link Rate inertia.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_disable_rate_inertia( void )
{
  if(hdrmc_feature_is_enabled( HDRMC_FEATURE_HDR_RL_RATE_INERTIA))
    hdrrmac0_disable_rate_inertia();
}

/*===========================================================================

FUNCTION HDRRMAC_CONFIGURE

DESCRIPTION
  This function is called by SCP to start AT-initiated configuration.
  
DEPENDENCIES
  None.

PARAMETERS
  sender - The calling protocol.

RETURN VALUE
  TRUE if the configuration is required; FALSE if configuration is not
  needed.

SIDE EFFECTS
  None.

===========================================================================*/
boolean hdrrmac_configure
(
  hdrhai_protocol_name_enum_type sender
)
{
  boolean ret_val = TRUE;
    /* Return Value */

#ifdef FEATURE_HDR_REVA_L1

  hdrrmac_cmd_union_type hdrrmac_cmd; 
    /* Build command into local struct */

  uint16 subtype;
    /* RMAC subtype */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  subtype = hdrscp_get_config_subtype(HDRHAI_RTC_MAC_PROTOCOL);

  if( (subtype == HDRSCP_SUB3_RMAC) &&  hdrrmac3_is_config_needed() )
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "RMAC Config is required" );

    /*-------------------------------------------------------------------------
       Fill in the command ID
     -------------------------------------------------------------------------*/

    hdrrmac_cmd.hdr.cmd = HDRRMAC_CONFIGURE;
    hdrrmac_cmd.hdr.sender  = sender;

    if( rex_self() == HDRRMAC_TASK_PTR )
    {
      /* call the processing function directly if in the same task */
      hdrrmac_process_cmd( &hdrrmac_cmd );
    }
    else
    {
      HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL,
                         &hdrrmac_cmd,
                         sizeof(hdrrmac_cmd_union_type) );
    }

  }
  else
#ifdef FEATURE_HDR_REVB_SUPPORTED
  if ((subtype == HDRSCP_MC_RMAC) && hdrscmrmac4_config_is_needed() )
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "RMAC Config is required" );

    (void) hdrscmrmac4_configure();
  }
  else
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "RMAC Config is not required" );
    ret_val = FALSE;
  }

  return ret_val;

} /* hdrrmac_configure() */


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMAC_GET_FLOW_MAPPING                                   EXTERNAL

DESCRIPTION
  Returns the AssociatedFlowsNN attribute to caller. 

DEPENDENCIES
  None.

PARAMETERS
  mapping - Pointer to structure giving the set of upper layer flows mapped 
            to a given MAC flow. 
  flow_id - MAC Flow Id 

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_get_flow_mapping
( 
  hdrrmac_flow_mapping_struct_type *mapping, 
  uint8 flow_id 
)
{
  switch(hdrrmac.subtype)
  {
#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      hdrrmac3_get_flow_mapping(mapping, flow_id);
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED  
    case HDRSCP_SUBTYPE_4_RTCMAC:
      hdrrmac4_get_flow_mapping(mapping, flow_id);
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */
  
    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );
  }

  return;

} /* hdrrmac_get_flow_mapping */


 /* EJECT */
/*=========================================================================

FUNCTION     : HDRRMAC_GET_CHANGED_FLOW_MAPPING

DESCRIPTION  : 
  This function finds a set of App flows that are either bound or unbound from
  a particular MAC flow and stores it in the arrays bound_flows and 
  unbound_flows respectively.

Note: This function is called from DS task.

DEPENDENCIES : None

PARAMETERS   :
  unbound_flows - Array of Flow mapping structs with a set of App flows for 
                  each MAC flow that are unbound from this MAC flow due to 
                  recent GAUPing
  bound_flows   - Array of Flow mapping structs with a set of App flows for 
                  each MAC flow that are bound due to recent GAUPing

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/
void hdrrmac_get_changed_flow_mapping
(
  hdrrmac_flow_mapping_struct_type *unbound_flows,
  hdrrmac_flow_mapping_struct_type *bound_flows
)
{
  switch(hdrrmac.subtype)
  {
#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      hdrrmac3_get_changed_flow_mapping(unbound_flows, bound_flows);
      break;
 
#ifdef FEATURE_HDR_REVB_SUPPORTED 
    case HDRSCP_SUBTYPE_4_RTCMAC:
      hdrrmac4_get_changed_flow_mapping(unbound_flows, bound_flows);
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */
  
    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );
  }

  return;

} /* hdrrmac_get_changed_flow_mapping */



/*===========================================================================
FUNCTION HDRRMAC_IS_FLOW_ACTIVE                                     EXTERNAL

DESCRIPTION
  Function to find out if a given flow is active. A flow is considered active
  if its BucketLevelMax attribute is > 0

DEPENDENCIES
  None.

PARAMETERS
  MAC flow ID in the range [0x0 - 0xF]

RETURN VALUE
  TRUE if flow is active; false otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmac_is_flow_active( uint8 flow )
{
  boolean rc = FALSE;

  switch(hdrrmac.subtype)
  {
#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      rc = hdrrmac3_is_flow_active(flow);
      break;
  
#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      rc = hdrrmac4_is_flow_active(flow);
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */
  
    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );
  }

  return rc;

} /* hdrrmac_is_flow_active */

 /* EJECT */
#ifdef FEATURE_HDR_IS890A
/*===========================================================================
FUNCTION HDRRMAC_SET_TARGET_PS_INDEX

DESCRIPTION
  This function is invoked by RETAP to set the maximum RRI value on the 
  specified channel.

DEPENDENCIES
  None.

PARAMETERS
  phy_chan_idx  - Physical channel index.  Ignore if subtype is not rmac4.
  max_rri       - maximum RRI value.

RETURN VALUE
  rri_valid - TRUE if the specified RRI is valid; FALSE otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmac_set_target_ps_index
(
  uint8 phy_chan_idx,
  uint8 max_rri
)
{

  boolean ret_val = FALSE;
    /* Return value based on set_target_ps call */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch (hdrrmac.subtype)
  {
    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      ret_val = hdrrmac0_set_target_ps_index(max_rri);
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      ret_val = hdrrmac3_set_target_ps_index(max_rri);
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      ret_val = hdrrmac4_set_target_ps_index(phy_chan_idx, max_rri);
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );
  }

  return ret_val;

} /* hdrrmac_set_target_ps_index */


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMAC_GET_SELECTED_PS_INDEX

DESCRIPTION
  This function is invoked by RETAP to get the payload size index selected 
  by RTCMAC3 or RTCMAC4.

DEPENDENCIES
  None.

PARAMETERS
  phy_chan_idx  - Physical channel index.  Ignored if subtype is not rmac4.

RETURN VALUE
  payload size index in the range [1..12]

SIDE EFFECTS
  None.
===========================================================================*/
uint8 hdrrmac_get_selected_ps_index
(
  uint8 phy_chan_idx
)
{

  uint8 ret_val = 0;
    /* Return value based on get_selected_ps call */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch (hdrrmac.subtype)
  {
#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      ret_val = hdrrmac3_get_selected_ps_index();
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      ret_val = hdrrmac4_get_selected_ps_index(phy_chan_idx);
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );
  }

  return ret_val;

} /* hdrrmac_get_selected_ps_index */
#endif /* FEATURE_HDR_IS890A */


/*===========================================================================
FUNCTION HDRRMAC_GET_CURRENT_TX_PACKET_INFO

DESCRIPTION
  Returns the currently transmitting RRI or packet size, as well as the
  T2P ratio. 

DEPENDENCIES
  None.

PARAMETERS
  Pointer to packet info structure to be filled out.

RETURN VALUE
  TRUE if valid data was filled in. 
  FALSE if there was no data (due to PA not on, or RTCMAC not 
    active, for instance).

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmac_get_current_tx_packet_info
(
  hdrrmac_current_tx_packet_info_type * pkt_info
)
{

  boolean info_valid = FALSE;
    /* Return value -- indicates whether packet info is valid */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check for NULL */
  if (pkt_info != NULL)
  {

    if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
    {

      switch (hdrrmac.subtype)
      {

        case HDRSCP_DEFAULT_RTCMAC:
        case HDRSCP_SUBTYPE_1_RTCMAC:
        
          info_valid = hdrrmac0_get_current_tx_packet_info(pkt_info);

          break;

#ifdef FEATURE_HDR_REVA_L1
        case HDRSCP_SUBTYPE_3_RTCMAC:
          info_valid = hdrrmac3_get_current_tx_packet_info(pkt_info);
          break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
        case HDRSCP_SUBTYPE_4_RTCMAC: 
          info_valid = hdrrmac4_get_current_tx_packet_info(pkt_info);
          break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */

#endif /* FEATURE_HDR_REVA_L1 */

        default:
          HDR_MSG_PROT_1(MSG_LEGACY_ERROR,
                       "This function not supported for subtype %d",
                       hdrrmac.subtype);
      }
    }

    pkt_info->subtype = hdrrmac.subtype;
      /* Fill in subtype */

  }
  else
  {
    HDR_MSG_PROT(MSG_LEGACY_ERROR,"Unexp NULL pointer!");
  }

  return info_valid;

}


/*===========================================================================
FUNCTION HDRRMAC_FLOW_CONTROL

DESCRIPTION
  This function is used to flow control by  - 
  1. restricting transmit packet sizes.  Once in this mode, RMAC would 
     continue to restrict transmit packet sizes till hdrrmac_flow_control 
     is called with the first argument set to '12' to  bring RMAC out of 
     packet size based flow control mode.
  2. disabling the production of reverse link packets on non-SLP carriers 
     [only applicable when multicarrier RMAC is operational].  Once in this 
     mode, RMAC will not support data on non-SLP carriers till 
     hdrrmac_flow_control is called with the second argument set to FALSE.
  Examples: 
  1. hdrrmac_flow_control(12,TRUE) stops data on non-SLP carriers.  No 
     restriction is placed on SLP carrier payload.
  2. hdrrmac_flow_control(5,FALSE) enforces payload restriction leve 5 on all
     RL carriers.
  3. hdrrmac_flow_control(5,TRUE) stops data on non-SLP carriers and set the
     payload restriction level on SLP carrier to 5.

DEPENDENCIES
  None.

PARAMETERS
  payload_restriction_level - This is a number from 0 to 12.  This is 
    internally mapped to the max payload level RMAC can transmit.  Number 12 
    removes payload restriction.
  enable_cpu_flow_control - When set to TRUE, data is stopped on non-SLP 
    carriers.  When set to FALSE, data is re-enabled on non-SLP carriers.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_flow_control
(
  uint8 payload_restriction_level,
  boolean enable_cpu_flow_control
)
{

#ifdef FEATURE_HDR_REVB_SUPPORTED
  uint8 ch; /* Loop variable */
#endif /* FEATURE_HDR_REVB_SUPPORTED */

  uint8 rel0_payload_restriction; /* Payload restriction computed for Rel0 */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Do not let payload restriction level be set to 0 */
  payload_restriction_level = MAX( payload_restriction_level, 1 );

  switch (hdrrmac.subtype)
  {
    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:

      /* Translate given payload restriction level to Rel0 payload 
         restriction. */
      if (payload_restriction_level > 12)
      {
        /* Remove payload restriction. */
        rel0_payload_restriction = HDRRMAC_IS856_RRI;
      }
      else
      {
        rel0_payload_restriction = 
          hdrrmac_flow_control_rel0_payload_restriction[
          payload_restriction_level];
      }

      /* Set payload restriction on Rel0 RL packets. */
      (void )hdrrmac0_set_target_ps_index(rel0_payload_restriction);

      if (enable_cpu_flow_control)
      {
        HDR_MSG_PROT( MSG_LEGACY_HIGH, 
          "CPU flow control command received for Rel0 RMAC, ignoring.");
      }
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
  
      if (payload_restriction_level >= 12)
      {
        /* Remove payload restriction. */
        payload_restriction_level = HDRRMACCMN_IS856_A_RRI;
      }

      /* Set payload restriction on RevA RL packets. */
      (void )hdrrmac3_set_target_ps_index(payload_restriction_level);

      if (enable_cpu_flow_control)
      {
        HDR_MSG_PROT( MSG_LEGACY_HIGH, 
          "CPU flow control command received for RevA RMAC, ignoring.");
      }
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:

      if (payload_restriction_level >= 12)
      {
        /* Remove payload restriction. */
        payload_restriction_level = HDRRMACCMN_IS856_A_RRI;
      }

      for (ch = 0; ch < HDRRMAC_MC_MAX_NUM_RL_CHANS; ch++)
      {
        /* Set payload restriction on RevB RL packets on all carriers. */
        (void )hdrrmac4_set_target_ps_index(ch, payload_restriction_level);
      }

      if (enable_cpu_flow_control)
      {
        /* Disallow data on non-SLP carriers. */
        hdrrmac4_start_flow_control();
      }
      else
      {
        /* Allow data on non-SLP carriers. */
        hdrrmac4_stop_flow_control();
      }
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );
  }
}


/*===========================================================================
FUNCTION HDRRMAC_START_FLOW_CONTROL

DESCRIPTION
  This function is used to reduce CPU consumption by stopping the production
  of reverse link packets on non-SLP carriers.  Once in this mode, RMAC will 
  not support data on non-SLP carriers till hdrrmac_stop_flow_control
  is called to bring RMAC out of flow conrol mode.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_start_flow_control( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch (hdrrmac.subtype)
  {
    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
#endif /* FEATURE_HDR_REVA_L1 */
      HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
        "RMAC Config %d received flow control start, ignoring.",
        hdrrmac.subtype );
      break;

#ifdef FEATURE_HDR_REVA_L1
#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      hdrrmac4_start_flow_control();
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );
  }
}


/*===========================================================================
FUNCTION HDRRMAC_STOP_FLOW_CONTROL

DESCRIPTION
  This function brings RMAC out of flow control mode ( Calling 
  hdrrmac_start_flow_control() puts RMAC in flow control mode ) and 
  re-enables data on non-SLP flows.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_stop_flow_control( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch (hdrrmac.subtype)
  {
    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
#endif /* FEATURE_HDR_REVA_L1 */
      HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
        "RMAC Config %d received flow control stop, ignoring.",
        hdrrmac.subtype );
      break;

#ifdef FEATURE_HDR_REVA_L1
#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      hdrrmac4_stop_flow_control();
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );
  }
}

 
 /* EJECT */
/*===========================================================================
FUNCTION HDRRMAC_ENTER_DRC_SUPERVISION                              EXTERNAL

DESCRIPTION
  The function queues DRC supervision entry command to RMAC.
  
DEPENDENCIES
  None.

PARAMETERS
  sender        - The protocol giving this command (only for software trace)
  phy_chan_mask - Physical channel ID mask
                  Set 'n'th bit to indicate that reverse channel 'n' is 
                  now in DRC supervision

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_enter_drc_supervision
(
  hdrhai_protocol_name_enum_type  sender,
  uint16                          phy_chan_mask
)
{
  /* Temporary command buffer. */
  hdrrmac_cmd_drc_supervision_struct_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_PROT( MSG_LEGACY_MED, "hdrrmac_enter_drc_supervision()" );

  cmd_buf.hdr.cmd = HDRRMAC_ENTER_DRC_SUPERVISION_CMD;
  cmd_buf.hdr.sender = sender;
  cmd_buf.rl_carrier_mask = phy_chan_mask;

  HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );

} /* hdrrmac_enter_drc_supervision */


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMAC_EXIT_DRC_SUPERVISION                               EXTERNAL

DESCRIPTION
  The function queues DRC supervision exit command to RMAC.
    
DEPENDENCIES
  None.

PARAMETERS
  sender        - The protocol giving this command (only for software trace)
  phy_chan_mask - Physical channel ID mask
                  Set 'n'th bit to indicate that reverse channel 'n' is 
                  now out of DRC supervision

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_exit_drc_supervision
(
  hdrhai_protocol_name_enum_type  sender,
  uint16 phy_chan_mask
)
{
  /* Temporary command buffer. */
  hdrrmac_cmd_drc_supervision_struct_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_PROT( MSG_LEGACY_MED, "hdrrmac_exit_drc_supervision()" );

  cmd_buf.hdr.cmd = HDRRMAC_EXIT_DRC_SUPERVISION_CMD;
  cmd_buf.hdr.sender = sender;
  cmd_buf.rl_carrier_mask = phy_chan_mask;

  HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );

} /* hdrrmac_exit_drc_supervision */

#ifdef FEATURE_HDR_THERMAL_BACKOFF

/*===========================================================================

FUNCTION HDRRMAC_CONFIGURE_THERMAL_PA_BACKOFF_CB                     EXTERNAL

DESCRIPTION
  This interface is used to configure the parameters specific to flow control
  algorithm.

  This function will run in the same task context as CCM.
    
DEPENDENCIES
  None.

PARAMETERS
  params - Pointer to struct containing flow control configuration parameters.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

void hdrrmac_configure_thermal_pa_backoff_cb 
(
  void *params
)
{

  /* Temporary loop variable. */
  int i;

  /* Typed pointer */
  ds707_ccm_pa_backoff_flow_control_params_type *fc_params;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Null Check */
  if (params != NULL)
  {
    fc_params = (ds707_ccm_pa_backoff_flow_control_params_type *)params;

    hdrrmac.thermal_power_bo_params.step_down_timer_ms = fc_params->step_down_timer_ms;
    hdrrmac.thermal_power_bo_params.step_up_timer_ms = fc_params->step_up_timer_ms;
    hdrrmac.thermal_power_bo_params.delay_time_ms = fc_params->delay_time_ms;
    hdrrmac.thermal_power_bo_params.default_tx_power_bo_pattern_number =
       fc_params->default_tx_power_bo_pattern_number;
    hdrrmac.thermal_power_bo_params.max_tx_power_bo_pattern_number =
       fc_params->max_tx_power_bo_pattern_number;
    hdrrmac.thermal_power_bo_params.tx_power_bo_period_ms = 
       fc_params->tx_power_bo_period_ms;
    hdr_thermal_power_bo_period_ms = fc_params->tx_power_bo_period_ms;

    for(i = 0; i < HDRRMAC_NUM_BACKOFF_PATTERNS; i++)
    {
      hdrrmac_thermal_power_bo_pattern_backoff_duration_ms[i]= 
         ((( hdrrmac_thermal_power_bo_pattern_backoff_percentile[i] )
                         * hdr_thermal_power_bo_period_ms )/100);

      hdrrmac_thermal_power_bo_pattern_non_backoff_duration_ms[i]=
        (((100 - hdrrmac_thermal_power_bo_pattern_backoff_percentile[i])
                          * hdr_thermal_power_bo_period_ms)/100);

       HDR_MSG_PROT_4( MSG_LEGACY_MED,
                    "Thermal backoff: "
                    "hdrrmac_thermal_power_bo_pattern_backoff_duration_ms[%d] %d"
                    "hdrrmac_thermal_power_bo_pattern_non_backoff_duration_ms[%d] %d",
                       i,
                       hdrrmac_thermal_power_bo_pattern_backoff_duration_ms[i],
                       i,
                       hdrrmac_thermal_power_bo_pattern_non_backoff_duration_ms[i]
                    );
      
    }

   
    HDR_MSG_PROT_6( MSG_LEGACY_MED,
                    "Thermal backoff: config(step_down_timer_ms: %d, step_up_timer_ms: %d,"
                    " delay_time_ms: %d default_tx_power_bo_pattern_number: %d,"
                    " max_tx_power_bo_pattern_number: %d, tx_power_bo_period_ms: %d)",
                    hdrrmac.thermal_power_bo_params.step_down_timer_ms,
                    hdrrmac.thermal_power_bo_params.step_up_timer_ms,
                    hdrrmac.thermal_power_bo_params.delay_time_ms,
                    hdrrmac.thermal_power_bo_params.default_tx_power_bo_pattern_number,
                    hdrrmac.thermal_power_bo_params.max_tx_power_bo_pattern_number,
                    hdrrmac.thermal_power_bo_params.tx_power_bo_period_ms);

  } /* Null check */

} /* hdrrmac_configure_thermal_pa_backoff_cb */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRRMAC_CONTROL_THERMAL_PA_BACKOFF_CB                        EXTERNAL

DESCRIPTION
  The function queues control pa bckoff command to RMAC .
  
DEPENDENCIES
  None.

PARAMETERS
  cmd - Thermal flow control command to be processed.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

void hdrrmac_control_thermal_pa_backoff_cb
(
  ds707_ccm_flow_control_cmd_enum_type cmd
)
{
  /* Temporary command buffer. */
  hdrrmac_cmd_thermal_pa_backoff_struct_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If the AT is in traffic */
  if ( hdrrmac_is_active(HDRHAI_RTC_MAC_PROTOCOL) ) 
  {
     HDR_MSG_PROT_1( MSG_LEGACY_MED, "Thermal backoff: "
                    "hdrrmac_control_thermal_pa_backoff_cb()cmd %d", cmd );

     cmd_buf.hdr.cmd = HDRRMAC_CTRL_PA_BACKOFF_CMD;
     cmd_buf.cmd = cmd;

     HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) ); 
  }
  else
  {
     HDR_MSG_PROT_1( MSG_LEGACY_LOW, "Thermal Backoff Cmd: %d,"
                     " ignored RMAC is not active",
                     cmd );
  }
} /* hdrrmac_control_thermal_pa_backoff_cb */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRRMAC_THERMAL_POWER_BO_STEP_TIMER_CB                      EXTERNAL

DESCRIPTION
 This function is the callback for the step timer.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrrmac_thermal_power_bo_step_timer_cb( uint32 timer_id )
{
  /* step timer */
  uint16 temp_timer;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If the AT is in traffic and back off is on */
  if ( hdrrmac_is_active(HDRHAI_RTC_MAC_PROTOCOL) && hdrrmac.thermal_power_bo_is_on ) 
  {
     /* Stop backoff timer */
    rex_clr_timer(&hdrrmac.thermal_power_bo_backoff_timer);
    rex_clr_timer(&hdrrmac.thermal_power_bo_non_backoff_timer);

    /* Entering critical section... */
    rex_enter_crit_sect(&hdrrmac.thermal_power_bo_crit_sect);

    /* Last command received was P_DOWN and TEPD occurs */
    if ( timer_id == HDRRMAC_STEP_DOWN_TIMER )
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "Thermal backoff: event TEPD!" );

      rex_clr_timer(&hdrrmac.thermal_power_bo_step_down_timer);
     
      hdrrmac.thermal_power_bo_pattern = HDRRMAC_THERMAL_POWER_BO_PATTERN_INC(
                                   hdrrmac.thermal_power_bo_pattern,
                                   hdrrmac.thermal_power_bo_params.max_tx_power_bo_pattern_number);


      /* If we are in total backoff no need to start timer */
      if (hdrrmac.thermal_power_bo_pattern < HDRRMAC_TOTAL_BACKOFF_PATTERN)
      {
        /* Restart the step timer to enable periodic flow decrease */
         temp_timer = hdrrmac.thermal_power_bo_params.step_down_timer_ms +
         hdrrmac.thermal_power_bo_params.delay_time_ms;

         rex_set_timer( &hdrrmac.thermal_power_bo_step_down_timer, temp_timer );
      }

      /* Program rf with the backoff pattern */
      hdrrmac_set_thermal_power_bo_pattern(hdrrmac.thermal_power_bo_pattern);
      
    }
    else if ( timer_id == HDRRMAC_STEP_UP_TIMER )
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "Thermal backoff: event TEPU!" );

      rex_clr_timer(&hdrrmac.thermal_power_bo_step_up_timer);
      

      hdrrmac.thermal_power_bo_pattern = HDRRMAC_THERMAL_POWER_BO_PATTERN_DEC(
                                   hdrrmac.thermal_power_bo_pattern,
                                   HDRRMAC_FIRST_BACKOFF_PATTERN);

      
      if (hdrrmac.thermal_power_bo_pattern > HDRRMAC_NO_BACKOFF_PATTERN)
      {
          /* Restart the step timer to enable periodic flow increase */
        temp_timer = hdrrmac.thermal_power_bo_params.step_up_timer_ms +
           hdrrmac.thermal_power_bo_params.delay_time_ms;

        rex_set_timer( &hdrrmac.thermal_power_bo_step_up_timer, temp_timer );
      }
      else
      {
        hdrrmac.thermal_power_bo_is_on = FALSE;        
      }

      /* Program rf with the backoff pattern */
      hdrrmac_set_thermal_power_bo_pattern(hdrrmac.thermal_power_bo_pattern);
      
    }

    /* Leave Critical section */
    rex_leave_crit_sect(&hdrrmac.thermal_power_bo_crit_sect);

  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "Thermal backoff: Step timer cb ignored"
                                   ", RMAC not active!" );
  }

} /* hdrrmac_thermal_power_bo_step_timer_cb */

/*===========================================================================

FUNCTION HDRRMAC_SET_THERMAL_POWER_BO_PATTERN                         INTERNAL

DESCRIPTION
  This function is used to configure thermal backoff pattern in RF.

    
DEPENDENCIES
  None.

PARAMETERS
  pattern - Pattern to configure

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

void hdrrmac_set_thermal_power_bo_pattern
( 
  hdrrmac_thermal_backoff_pattern_enum_type pattern
)
{

  /* temporary working variable */
  uint16 backoff_duration;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if(pattern == HDRRMAC_TOTAL_BACKOFF_PATTERN)
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "Thermal Backoff: "
                                   "Enter state HDRRMAC_TOTAL_BACKOFF_PATTERN" );
    /* Start backoff no timer needed */
    rfm_hdr_set_thermal_state(hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN),  RFM_HDR_THERMAL_STATE_ENABLE);
  }
  else if(pattern == HDRRMAC_NO_BACKOFF_PATTERN)
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "Thermal Backoff: "
                                   "Enter state HDRRMAC_NO_BACKOFF_PATTERN" );
    /* Start backoff no timer needed */
    rfm_hdr_set_thermal_state(hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN),  RFM_HDR_THERMAL_STATE_DISABLE);
  }
  else
  {
    /* Calculate backoff duration for pattern */
    backoff_duration = hdrrmac_thermal_power_bo_pattern_backoff_duration_ms[pattern];

    HDR_MSG_PROT_2( MSG_LEGACY_HIGH, 
                    "Thermal Backoff: Enter backoff state %d, backoff duration %d"
                    ,pattern, backoff_duration );
     

    /* Start timer for backoff duration */
    rex_set_timer( &hdrrmac.thermal_power_bo_backoff_timer, backoff_duration );

    /* Program backoff to rf */
    rfm_hdr_set_thermal_state(hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN),  RFM_HDR_THERMAL_STATE_ENABLE);

  }

} /* hdrrmac_set_thermal_power_bo_pattern */

/*===========================================================================

FUNCTION HDRRMAC_SET_THERMAL_POWER_BO_TIMER_CB                        INTERNAL

DESCRIPTION
  This function is the callback called when backoff timer expires.

DEPENDENCIES
  None.

PARAMETERS
  pattern - Pattern to configure

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

static void hdrrmac_set_thermal_power_bo_timer_cb
(
  uint32 timer_id 
)
{
  /* temporary working variable */
  uint16 non_backoff_duration;

  hdrrmac_thermal_backoff_pattern_enum_type pattern;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  pattern = hdrrmac.thermal_power_bo_pattern;

  /* calculate non backoff duration for current pattern */
  non_backoff_duration = 
    hdrrmac_thermal_power_bo_pattern_non_backoff_duration_ms[pattern];

  HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "Thermal backoff: Enter non backoff state "
                                   "for pattern %d, non backoff duration %d"
                  ,pattern, non_backoff_duration );

  /* start timer for backoff duration */
  rex_set_timer( &hdrrmac.thermal_power_bo_non_backoff_timer, non_backoff_duration );
  
  /* Program rf to backoff */
  rfm_hdr_set_thermal_state(hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN),  RFM_HDR_THERMAL_STATE_DISABLE);
  

} /* hdrrmac_set_thermal_power_bo_timer_cb */

/*===========================================================================

FUNCTION HDRRMAC_SET_THERMAL_POWER_NON_BO_TIMER_CB                    INTERNAL

DESCRIPTION
  This function is callback called when backoff timer expires.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

static void hdrrmac_set_thermal_power_non_bo_timer_cb
( 
   uint32 timer_id
)
{
  
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Start the whole backoff period cycle again */
   hdrrmac_set_thermal_power_bo_pattern(hdrrmac.thermal_power_bo_pattern);
 
} /* hdrrmac_set_thermal_power_non_bo_timer_cb */

#endif /* FEATURE_HDR_THERMAL_BACKOFF */

#ifdef FEATURE_LTE_TO_HDR_OH
/*===========================================================================
FUNCTION HDRRMAC_UNPACK_OLP_MSG                                  INTERNAL

DESCRIPTION
  This function unpacks the HRPDOpenLoopParameters message.

DEPENDENCIES
  None.

PARAMETERS
  rcv_msg_ptr - pointer to receive message buffer.
  per_pn_olp - pointer to location where to unpack the per PN OLP
  initial_offset - Offset at which to start unpacking
  final_offset - Offset after the per PN OLP is unpacked

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
hdrerrno_enum_type hdrrmac_unpack_per_pn_olp
(
  dsm_item_type * rcv_msg_ptr,
  /*lint -esym(715,rcv_msg_ptr) 
   * Ignore argument(rcv_msg_ptr) not used lint error #715*/
  hdrrmac_open_loop_params_type *per_pn_olp,
  int initial_offset,
  int *final_offset
)
{
  int  offset = 0;                       
    /* Offset(bits) of field from msg start */
  int  err = 0;                          
    /* Error value */
  hdrerrno_enum_type unpack_result = E_SUCCESS; 
    /* Return value of unpack Grant msg */
/*-------------------------------------------------------------------------*/

  offset = initial_offset;

  err |= (int) HDRRMAC_OLP_OPEN_LOOP_ADJUST( rcv_msg_ptr , 
           (uint8*)&per_pn_olp->open_loop_adjust,
           offset );
  offset += HDRRMAC_OLP_OPEN_LOOP_ADJUST_SIZE;

  err |= (int) HDRRMAC_OLP_INITIAL_ADJUST( rcv_msg_ptr , 
           (uint8*)&per_pn_olp->initial_adjust,
           offset );
  offset += HDRRMAC_OLP_INITIAL_ADJUST_SIZE;

  err |= (int) HDRRMAC_OLP_PILOT_STRENGTH_INCL( rcv_msg_ptr , 
           &per_pn_olp->pilot_strength_incl,
           offset );
  offset += HDRRMAC_OLP_PILOT_STRENGTH_INCL_SIZE;

  if ( per_pn_olp->pilot_strength_incl )
  {
    err |= (int) HDRRMAC_OLP_PILOT_STRENGTH_NOMINAL( rcv_msg_ptr , 
             &per_pn_olp->pilot_strength_nominal,
             offset );
    offset += HDRRMAC_OLP_PILOT_STRENGTH_NOMINAL_SIZE;

    err |= (int) HDRRMAC_OLP_PILOT_STRENGTH_CORRECTION_MIN( rcv_msg_ptr , 
             &per_pn_olp->pilot_strength_correction_min,
             offset );
    offset += HDRRMAC_OLP_PILOT_STRENGTH_CORRECTION_MIN_SIZE;

    err |= (int) HDRRMAC_OLP_PILOT_STRENGTH_CORRECTION_MAX( rcv_msg_ptr , 
             &per_pn_olp->pilot_strength_correction_max,
             offset );
    offset += HDRRMAC_OLP_PILOT_STRENGTH_CORRECTION_MIN_SIZE;
  }

  *final_offset = offset;

  if (err != 0)
  {
    unpack_result = E_NO_DATA;

    HDR_MSG_PROT( MSG_LEGACY_ERROR, "Error unpacking per PN OL" );
  }

  return unpack_result;

} /* hdrrmac_unpack_per_pn_olp */

/*===========================================================================
FUNCTION HDRRMAC_UNPACK_OLP_MSG                                  INTERNAL

DESCRIPTION
  This function unpacks the HRPDOpenLoopParameters message.

DEPENDENCIES
  None.

PARAMETERS
  rcv_msg_ptr - pointer to receive message buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
hdrerrno_enum_type hdrrmac_unpack_olp_msg
(
  dsm_item_type * rcv_msg_ptr
  /*lint -esym(715,rcv_msg_ptr) 
   * Ignore argument(rcv_msg_ptr) not used lint error #715*/
)
{
  int  offset = 0;                       
    /* Offset(bits) of field from msg start */
  int  err = 0;                          
    /* Error value */
  hdrerrno_enum_type unpack_result = E_SUCCESS; 
    /* Return value of unpack Grant msg */
  uint8 pn_index = 0;
    /* PN Loop index */
  uint16 rmac_subtype = hdrscp_get_current_subtype (HDRHAI_RTC_MAC_PROTOCOL);
    /* Current RMAC subtype */
  uint16 ch_index = 0;
    /* Rev CDMA Ch Loop index */
  int offset_per_pn = 0;
    /* Offset(bits) of field from msg start */
  boolean pilot_is_included = FALSE;
   /* Whether pilot is included or not in OLP */
  uint8 num_pilots = 0;
  uint8 num_rev_cdma_ch = 0;
  uint8 system_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  offset = HDRRMAC_OLP_MSG_ID_SIZE;
  err = 0; /* E_SUCCESS = 0 */
  unpack_result = E_SUCCESS;

  /* Unpack num pilots */
  err |= (int) HDRRMAC_OLP_NUM_PILOTS( rcv_msg_ptr , 
                    &num_pilots,
                     offset );
  offset += HDRRMAC_OLP_NUM_PILOTS_SIZE;

  /* When the AN sets this field to zero that means values of the fields
     from PilotIncluded to PilotStrengthCorrectionMax are common to all 
     the PilotPNs in the TrafficChannelAssignment message */
  if ( num_pilots == 0)
  {
    hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].num_pilots = 1;
  }
  else
  {
    hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].num_pilots = num_pilots;
  }

  /* Sanity check for buffer overflow */
  if( hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].num_pilots >= 
      HDRRMAC_NUM_PILOTS_MAX )
  {
    HDR_MSG_PROT_1(MSG_LEGACY_ERROR, "Invalid num_pilots %d rvcd in OLP", 
                   hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].num_pilots);
    err |= (int32) E_NO_DATA;

    return E_NO_DATA;
  }

  if (rmac_subtype == HDRSCP_SUB3_RMAC)
  {
    /* For Subtype 3 RMAC there is only one rev cdma channel */
    hdrrmac.olp_msg.num_rev_cdma_channels = 1;
    //hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].rev_cdma_chan = 0;

    for( pn_index = 0; 
         pn_index < hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].num_pilots; 
         pn_index++ )
    {
      if ( num_pilots > 0 )
      {
        err |= (int) HDRRMAC_OLP_PILOT_PN( rcv_msg_ptr , 
              &hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].olp[pn_index].pilot_pn,
              offset );
        offset += HDRRMAC_OLP_PILOT_PN_SIZE;
      }

      err |= hdrrmac_unpack_per_pn_olp ( rcv_msg_ptr, 
                      &hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].olp[pn_index],
                      offset,
                      &offset_per_pn );
      offset = offset_per_pn;

      if (err != 0)
      {
        break;
      }
    }
  }
  else if (rmac_subtype == HDRSCP_MC_RMAC)
  {
    /* Unpack all the PN's and store in the first rev cdma channel index */
    for( pn_index = 0; 
         ( (pn_index < hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].num_pilots) &&
           (num_pilots > 0) ); 
         pn_index++ )
    {
      err |= (int) HDRRMAC_OLP_PILOT_PN( rcv_msg_ptr , 
            &hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].olp[pn_index].pilot_pn,
            offset );
      offset += HDRRMAC_OLP_PILOT_PN_SIZE;
    }

    err |= (int) HDRRMAC_OLP_NUM_REV_CDMA_CHANNELS( rcv_msg_ptr , 
                    &num_rev_cdma_ch,
          offset );
    offset += HDRRMAC_OLP_NUM_REV_CDMA_CHANNELS_SIZE;

    /* If NumReverseCDMAChannels is set to 0, one occurrence of the 
       following record */
    if ( num_rev_cdma_ch == 0)
    {
      hdrrmac.olp_msg.num_rev_cdma_channels = 1;
    }
    else
    {
      hdrrmac.olp_msg.num_rev_cdma_channels = num_rev_cdma_ch;
    }

    for( ch_index = 0; 
         ch_index < hdrrmac.olp_msg.num_rev_cdma_channels; 
         ch_index++ )
    {
      if ( num_rev_cdma_ch > 0 )
      {
        err |= (int) HDRRMAC_OLP_REV_CDMA_SYS_TYPE( rcv_msg_ptr , 
                &system_type,
                offset );
        offset += HDRRMAC_OLP_REV_SYSTEM_TYPE_SIZE;

        if ( ( system_type != SYS_HDR_CHAN_REC_MODE_IS856 ) &&
             ( system_type != SYS_HDR_CHAN_REC_MODE_IS856_FWD_CH ) )
        {
          HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "Invalid system_type=%d in OLP message", 
                          system_type );
          err |= E_NO_DATA;
          break;
        }

        err |= (int) HDRRMAC_OLP_REV_CDMA_BAND( rcv_msg_ptr , 
              (uint8*)&hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].rev_cdma_chan.band,
              offset );
        offset += HDRRMAC_OLP_REV_CDMA_BAND_SIZE;

        err |= (int) HDRRMAC_OLP_REV_CDMA_CHANNEL( rcv_msg_ptr , 
              &hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].rev_cdma_chan.chan_num,
              offset );
        offset += HDRRMAC_OLP_REV_CDMA_CHANNEL_SIZE;
      }
      else
      {
      //  hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].rev_cdma_chan = 0;
      }

      hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].num_pilots =
            hdrrmac.olp_msg.rev_cdma_ch_olp[0].num_pilots;

      for( pn_index = 0; 
           pn_index < hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].num_pilots; 
           pn_index++ )
      {
        pilot_is_included = FALSE;
        if ( num_pilots > 0 )
        {
          err |= (int) HDRRMAC_OLP_PILOT_INCL( rcv_msg_ptr , 
                                               &pilot_is_included,
                                               offset );
          offset += HDRRMAC_OLP_PILOT_INCL_SIZE;
        }
        else
        {
          pilot_is_included = FALSE;
        }

        /* Skip Per PN parse if the PilotIncluded field is included and is
         * equal to 0. */
        if ( (num_pilots == 0) || pilot_is_included )
        {
          err |= hdrrmac_unpack_per_pn_olp ( rcv_msg_ptr, 
                      &hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].olp[pn_index],
                      offset,
                      &offset_per_pn );
          offset = offset_per_pn;

          if (err != 0)
          {
            break;
          }

          /* Since pilot pn was unpacked common for all the channels and stored 
            in index 0, copy the pilot pn to each channel structure here */
          if (ch_index > 0)
          {
            hdrrmac.olp_msg.rev_cdma_ch_olp[ch_index].olp[pn_index].pilot_pn =
              hdrrmac.olp_msg.rev_cdma_ch_olp[0].olp[pn_index].pilot_pn;
          }
        }
      } /* per pilot pn */

      if (err != 0)
      {
        break;
      }
    } /* per channel */
  } /* mc rmac subtype */

  if (err != 0)
  {
    unpack_result = E_NO_DATA;

    HDR_MSG_PROT( MSG_LEGACY_ERROR, "Error unpacking OLP Msg" );
  }

  return unpack_result;
} /* hdrrmac_unpack_olp_msg */

/*===========================================================================
FUNCTION HDRRMAC_PROCESS_OLP_MSG                                  INTERNAL

DESCRIPTION
  This function processes the HRPDOpenLoopParameters message.

DEPENDENCIES
  None.

PARAMETERS
  rcv_msg_ptr - pointer to receive message buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_process_olp_msg
(
  dsm_item_type * rcv_msg_ptr
  /*lint -esym(715,rcv_msg_ptr) 
   * Ignore argument(rcv_msg_ptr) not used lint error #715*/
)
{
  hdrerrno_enum_type      errnum;
    /* Result of unpacking OLP message */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  errnum = hdrrmac_unpack_olp_msg( rcv_msg_ptr );

  if (errnum != E_NO_DATA)
  {
    hdrrmac.olp_msg.valid = TRUE;
  }
}

/*===========================================================================
FUNCTION HDRRMAC_CALC_PILOT_STRENGTH_CORRECTION

DESCRIPTION
  This function computes pilot strength correction value.

DEPENDENCIES
  None.

PARAMETERS
  carrier, msg_nom, msg_corr_min, msg_corr_max

RETURN VALUE
  Pilot strength correction value.

SIDE EFFECTS
  None.
===========================================================================*/
int8 hdrrmac_calc_pilot_strength_correction
(
  uint8 carrier,         /* Carrier to use */
  int8 msg_nom,          /* Nom. value from msg. */
  int8 msg_corr_min,     /* Min corr from msg. */
  int8 msg_corr_max      /* Max corr from msg. */
)
{
  uint16  pilot_eng;  
  int8    nom, corr_min, corr_max, pilot_str_corr;
  int     pilot_eng_db;

  /* Best active sector pilot energy */
  pilot_eng    = hdrsrchmac_get_basp_pri_chain_eng( carrier );

  /* pilot strength as ecio */
  pilot_eng_db = hdrsrchmac_eng_to_ecio( pilot_eng ); /* -0.5 dB units */
  pilot_eng_db = -1 * (pilot_eng_db >> 1);  /* converted to 1 dB units */

  nom      = hdrrmac_pilot_str_nom_val [msg_nom];
  corr_min = hdrrmac_pilot_str_corr_min_val [msg_corr_min];
  corr_max = hdrrmac_pilot_str_corr_max_val [msg_corr_max];

  /* pilot_str_corr is in 1 dB units */
  pilot_str_corr = MIN( corr_max, (MAX( (nom - pilot_eng_db), corr_min )) );

  HDR_MSG_PROT_5( MSG_LEGACY_HIGH,
                  "Using PilotStr (dB): Nom %d, CorrMin %d, "
                  "CorrMax %d, PS DB %d, PS Corr %d",
                  nom, corr_min, corr_max,
                  pilot_eng_db, pilot_str_corr);

  return pilot_str_corr;
}

#endif  /* FEATURE_LTE_TO_HDR_OH */


/*===========================================================================
FUNCTION HDRRMAC_CONFIG_FUNNEL_MODE

DESCRIPTION
  In FTM mode, this function configures funnel mode.

DEPENDENCIES
  None.

PARAMETERS
  enable - TRUE  => Enable Funnel mode
           FALSE => Disable funnel mode
  data_active_carrier_idx 
         - Index of data active carrier to be conveyed to FW

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac_config_funnel_mode
(
  boolean  enable,
  uint8    data_active_carrier_idx
)
{

#if defined(FEATURE_HDR_REVA_L1) && defined(FEATURE_HDR_REVB_SUPPORTED)

  /* Configure funnel mode.  */
  hdrrmac4_config_funnel_mode(enable, data_active_carrier_idx);

#endif 

}

#ifdef FEATURE_HDR_FLEX_CONN
/*==========================================================================

FUNCTION  HDRRMAC_FL_ACTIVITY_NOTIFY_FLEXCONN

DESCRIPTION
  FL activity notification function from FMAC.
    
DEPENDENCIES
  None

INPUTS
  TRUE:  a FL packet is received, i.e. FL is acitive again
  FALSE: FL is idle for a period of time

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_fl_activity_notify_flexconn( boolean is_fl_active )
{
  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
  {
    switch (hdrrmac.subtype)
    {
#ifdef FEATURE_HDR_REVA_L1
      case HDRSCP_SUBTYPE_3_RTCMAC:
#ifdef FEATURE_HDR_REVB_SUPPORTED
      case HDRSCP_SUBTYPE_4_RTCMAC:
#endif /* FEATURE_HDR_REVB_SUPPORTED */
        hdrrmac.is_fl_active_flexconn = is_fl_active;
        HDR_MSG_PROT_2( MSG_LEGACY_HIGH,
                       "FLEXCONN: fl activity notification = %d, hstr=%d", 
                        hdrrmac.is_fl_active_flexconn, HDRHSTR_GET_COUNT() );

        if ( is_fl_active )
        {
          /* FL is active again, disable FLEXCONN */
          hdrrmac_set_flexconn_mode( FALSE );
        }
        break;

#endif /* FEATURE_HDR_REVA_L1 */

      default:
        HDR_MSG_PROT_1( MSG_LEGACY_FATAL, "FLEXCONN for subtype %d RTCMAC not supported",
                        hdrrmac.subtype);
    }

  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_ERROR, 
      "RMAC is not active, ignoring fl_activity_cb");
  }
} /* hdrrmac_fl_activity_notify_flexconn */

/*==========================================================================

FUNCTION  HDRRMAC_PROCESS_FLEXCONN_REPOINT

DESCRIPTION
  Process flexconn_repoing indication from fw. Right now fw sends this indication   
  when dsc repointing has happened and one full dsc length transmission has been   
  done with new dsc. 
     
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_process_flexconn_repoint( void )
{
  hdrrmac.flexconn_mode = FALSE;
    /* reset flexconn_mode to false, since fw has disabled flexconn */
  hdrfmac_dereg_fl_active_cb( HDRFMAC_FL_ACTIVE_CB_FLEXCONN, FALSE );
    /* deregister fmac active callback, in order to clear the FL activity timer */
  hdrrmac.rl_inactive_sf_cnt = 0;
    /* reset RL idle timer */
  hdrrmac.is_fl_active_flexconn = TRUE;
    /* reset the flag for FL active to true. */
  hdrfmac_reg_fl_active_cb( hdrrmac_fl_activity_notify_flexconn,
                            HDRFMAC_FL_ACTIVE_CB_FLEXCONN, FALSE );
    /* register the callback again, the FL activity timer will be restart again. We
       want to reset the timer after DSC/DRC repointing
    */
  hdrrmac.is_repointing_msg_rcvd = TRUE;
  HDR_MSG_PROT_2( MSG_LEGACY_HIGH,
    "FLEXCONN: recv Repointing MSG, reset FL/RL timer, fl_activity_fc=%d, rl_inactive_sf_cnt=%d",
    hdrrmac.is_fl_active_flexconn, hdrrmac.rl_inactive_sf_cnt );
    
} /* hdrrmac_process_flexconn_repoint */

/*==========================================================================

FUNCTION  HDRRMAC_SET_FLEXCONN_MODE

DESCRIPTION
  This function call appropriate driver function to send a message to FW to
  enable or disable FLEXCONN mode
    
DEPENDENCIES
  None

INPUTS
  flexconn_mode:  flexconn mode, TRUE means enabling flexconn
  
RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_set_flexconn_mode( boolean flexconn_mode )
{
  /* Temporary command buffer. */
  hdrrmac_cmd_union_type cmd_buf;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( rex_self() != HDRRMAC_TASK_PTR )
  {
    /* Post command to perform suspend connection processing in HDRTX 
       context */

    memset ( &cmd_buf, 0, sizeof (hdrrmac_cmd_union_type ) );

    cmd_buf.hdr.cmd    = HDRRMAC_SET_FLEXCONN_CMD;
    cmd_buf.hdr.sender = HDRHAI_RTC_MAC_PROTOCOL;
    cmd_buf.set_flexconn.flexconn_mode = flexconn_mode;

    HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );
  }
  else
  {
    if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
    {
      switch (hdrrmac.subtype)
      {
  #ifdef FEATURE_HDR_REVA_L1
         case HDRSCP_SUBTYPE_3_RTCMAC:

          hdrrmac3_set_flexconn_mode( flexconn_mode );
          break;
  #endif /* FEATURE_HDR_REVA_L1 */

  #ifdef FEATURE_HDR_REVB_SUPPORTED
         case HDRSCP_SUBTYPE_4_RTCMAC:
           hdrrmac4_set_flexconn_mode( flexconn_mode );
          break;
  #endif /* FEATURE_HDR_REVB_SUPPORTED */

        default:
          HDR_MSG_PROT_1( MSG_LEGACY_FATAL, "FLEXCONN for subtype %d RTCMAC not supported", 
                          hdrrmac.subtype);
      }
    }
    else
    {
      HDR_MSG_PROT( MSG_LEGACY_ERROR, 
        "RMAC is not active, ignoring fl_activity_cb");
    }
  }
 
} /* hdrrmac_set_flexconn_mode */

/*==========================================================================

FUNCTION  HDRRMAC_UPDATE_NUM_MAC_INDEXES

DESCRIPTION
  This function updates number of mac indexes to rmac
    
DEPENDENCIES
  None

INPUTS
  uint16: num_mac_indexes

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_update_num_mac_indexes( uint16 num_mac_indexes )
{
  hdrrmac.mac_indexes_cnt = num_mac_indexes;
}/* hdrrmac_update_num_max_indexes */

/*==========================================================================

FUNCTION  HDRRMAC_STOP_FLEXCONN_MONITOR

DESCRIPTION
  Stop monitoring RL/FL and turn off flexconn
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_stop_flexconn_monitor( boolean suspend_timer_for_flexconn )
{
  if ( hdrrmac.state == HDRRMAC_INACTIVE_STATE)
  {
    HDR_MSG_PROT( MSG_LEGACY_ERROR, 
                  "FLEXCONN/DTX: stop flexconn_dtx_monitor in inactive, no op");
    return;
  }
  if ( hdrrmac.subtype == HDRSCP_SUBTYPE_3_RTCMAC
#ifdef FEATURE_HDR_REVB_SUPPORTED
       || hdrrmac.subtype == HDRSCP_SUBTYPE_4_RTCMAC
#endif /* FEATURE_HDR_REVB_SUPPORTED */
     )
  {
    hdrrmac.flexconn_nest_count++;
    if ( hdrrmac.is_flexconn_nv_enabled && hdrrmac.flexconn_monitor_started ) 
    {
      hdrfmac_dereg_fl_active_cb( HDRFMAC_FL_ACTIVE_CB_FLEXCONN, suspend_timer_for_flexconn );
      hdrrmac.flexconn_monitor_started = FALSE;
      hdrrmac_set_flexconn_mode(FALSE);
      if ( suspend_timer_for_flexconn )
      {
        HDR_MSG_PROT( MSG_LEGACY_HIGH, 
          "FLEXCONN: suspend timer ");
      }
    }
    if ( hdrrmac.is_flexconn_nv_enabled )
    {

      HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
        "FLEXCONN/DTX: stop flexconn_dtx_monitor,fc_nested %d",
         hdrrmac.flexconn_nest_count );
    }   
  }
} /* hdrrmac_stop_flexconn_monitor */

/*==========================================================================

FUNCTION  HDRRMAC_START_FLEXCONN_MONITOR

DESCRIPTION
  Start to monitor RL/FL and decide whether to turn on/off DTX mode
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_start_flexconn_monitor( boolean resume_timer_for_flexconn )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
  {
    if ( hdrrmac.is_tap_call == TRUE )
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH,
                    "FLEXCONN/DTX: don't enable flexconn or dtx in tap call");
      return;
    }

   if (    hdrrmac.subtype == HDRSCP_SUBTYPE_3_RTCMAC
#ifdef FEATURE_HDR_REVB_SUPPORTED
         || hdrrmac.subtype == HDRSCP_SUBTYPE_4_RTCMAC
#endif /* FEATURE_HDR_REVB_SUPPORTED */
       )
    {
      if ( hdrrmac.is_flexconn_nv_enabled  )
      {
        HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
          "FLEXCONN/DTX: start flexconn_dtx_monitor,fc_nested %d",
           hdrrmac.flexconn_nest_count );
      }
      if ( hdrrmac.flexconn_nest_count == 0 )
      {
        HDR_MSG_PROT( MSG_LEGACY_ERROR," flexconn_nest_count < 1 !!");
      }
      else if ( hdrrmac.flexconn_nest_count > 1 )
      {
        /* There is more than one nested flexconn stop. We don't want to start flexconn
           monitoring here.
           One example of nested start/stop flexconn could be: traffic start -> start
           flexconn monitoting -> enter DRC supervison -> stop flexconn monitoring ->
           OFS -> stop flexconn monitoring -> OFS is done -> start flexconn monitor ->
           exit DRC supervison -> start flexconn monitoring
           There are other scenarios of nested flexconn start/stop, like OFS after RLSI.
           Starting flexconn monitoring may trigger resuming FL/RL idle timer. We want to
           start flexconn at the last place. In the example above, we want to start flexconn
           monitoring after exit DRC supervision, not after OFS.
        */
        hdrrmac.flexconn_nest_count --;
      }
      if ( hdrrmac.flexconn_nest_count == 1)
      {
        /* flexconn_nested_count == 1, we are at the right place to start flexconn monitoring
        */
        if ( hdrrmac.is_flexconn_nv_enabled && !hdrrmac.flexconn_monitor_started )
        {
           hdrrmac.first_dsc_duty_cycle = TRUE;
             /* We just start transmitting again, this is the first dsc duty cycle */
           hdrrmac.flexconn_disabled_before_tx_stop = FALSE;
             /* Initialize flexconn_disabled_before_tx_stop. This flag will be set
                when flexconn is disabled 2 dsc length before RLSI, or 80 slots before
                tune away or OFS. During the period when this flag is set, we will
                continue monitoring flexconn enable conditions. However we will not
                enable flexconn if all conditions are met. 
             */
           hdrrmac.is_fl_active_flexconn = TRUE;
            /* Assume fl is active first */
          if ( hdrfmac_flexconn_check_drc_lock_len() == FALSE )
          {
            /* drc lock length and dsc length are negotiated during session neg. They
               won't change during a connection. We only need to check it when starting
               flexconn monitoring.
               If DRCLockLength (in units of slots)  <= DSCLength (in units of slots),
               we might enable flexconn later if other conditions are satisfied. Otherwise
               we won't enable flexconn at all.
            */
            hdrrmac.flexconn_monitor_started = FALSE;
            HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                          "FLEXCONN: DRCLockLength > DSCLength, don't start flexconn monitoring");
          }
          else
          {
            if ( resume_timer_for_flexconn == FALSE )
            {
              /* reset timer if resume is not needed */
              hdrrmac.rl_inactive_sf_cnt = 0;
              hdrfmac_reg_fl_active_cb( hdrrmac_fl_activity_notify_flexconn,
                                        HDRFMAC_FL_ACTIVE_CB_FLEXCONN, FALSE );
            }
            else
            {
              hdrfmac_reg_fl_active_cb( hdrrmac_fl_activity_notify_flexconn,
                                        HDRFMAC_FL_ACTIVE_CB_FLEXCONN, TRUE );
              HDR_MSG_PROT_1( MSG_LEGACY_HIGH,
                              "FlexConn: resume timer rl_inactive_sf_cnt %d",
                              hdrrmac.rl_inactive_sf_cnt
                            );
            }
            hdrrmac.flexconn_monitor_started = TRUE;
            hdrrmac.rl_sf_cnt = 0;

          }
        }
      }

    } /* hdrrmac.subtype == HDRSCP_SUBTYPE_3_RTCMAC ||
         hdrrmac.subtype == HDRSCP_SUBTYPE_4_RTCMAC */
  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_ERROR, 
      "RMAC is not active, ignoring fl_activity_cb");
  }
 

} /* hdrrmac_start_flexconn_monitor */
#endif /* FEATURE_HDR_FLEX_CONN */

#if defined(FEATURE_HDR_DEVICE_ONLY_DTX)|| defined FEATURE_HDR_REVB_DTX_MODE\
 || defined FEATURE_HDR_TRAFFIC_DTX
/*==========================================================================

FUNCTION  HDRRMAC_CHECK_DTX_AFTER_TCA

DESCRIPTION
  This function update DTXFrameOffset based on the received TCA.It may disable
  dtx mode if conditions are met. It checks drc_length too.
    
DEPENDENCIES
  None

INPUTS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_check_dtx_after_tca( boolean tca_for_soft_ho )
{
#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
  hdrsrchmac_active_sectors_struct_type srch_info;
    /* Information on sectors in the active set */
  int                                   i, ch;
    /* loop index */
  uint8                                 num_of_carrier;
    /* number of carriers */

  boolean                               aux_drc_cover_incl_in_all_aset = TRUE;
    /* whether “AuxDRCCoverIncluded” field in the TCA “=1” for all the ASET
       pilots
    */

  boolean                               new_drc_boost_flag;
    /* flag to indicate whether we need to boost drc channel gain */

  uint8                                 new_dtx_frame_offset;
    /* new dtx frame offset */
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */

  uint16                                drc_length;
    /* drc length */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   /* Check drc_length */
  drc_length = (uint16) hdrfmac_get_drc_length( HDRMDSP_DEMOD_CARRIER_0 );
    /* all three carriers should have same drc length (for RevB) */

  /* For TCA of conn setup or HHO, we remember drc_length here. Then drc_length will be
     checked when we start monitoring dtx.
  */
   hdrrmac.dtx_drc_length = drc_length; 

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
  if ( hdrrmac.is_dtx_nv_enabled == FALSE )
  {
    return;
  }

  if (  ( hdrrmac.subtype != HDRSCP_SUBTYPE_3_RTCMAC )
#ifdef FEATURE_HDR_REVB_SUPPORTED         
      &&( hdrrmac.subtype != HDRSCP_SUBTYPE_4_RTCMAC )
#endif /* FEATURE_HDR_REVB_SUPPORTED */
     )
  {
    /* dtx is for revA or revB */
    return;
  }

  /* Get maximum number of carriers */
  if  ( hdrrmac.subtype == HDRSCP_SUBTYPE_3_RTCMAC )
    num_of_carrier = 1;
  else
    num_of_carrier = HDRRMAC_MAX_NUM_RL_CARRIERS;

  /* Check if there is CSM6850 base station, based on if AUCDRCoverIncluded == 1 */
  for (ch=0; ch < num_of_carrier ; ch++)
  {
    hdrsrchmac_get_mac_aset_info( ch, &srch_info );
      /* if carrier ch does not exist (for example, there are one or two carriers
         in RevB mode, the returned srch_info would have HDR_NO_PN_OFFSET as 
         pn_offset in all active_sectors. The check below inside the loop should
         cover this case.
  */
  /* Loop through each sector */
  for (i=0; i<HDRSRCH_ASET_MAX_SIZE; i++)
  {
    /* Only look at valid ASPs */
    if (srch_info.active_sectors[i].pn_offset != HDR_NO_PN_OFFSET)
    {
        if ( srch_info.active_sectors[i].aux_drc_cover == 0)
      {
          aux_drc_cover_incl_in_all_aset = FALSE ;
            /* aux_drc_cover == 0 means it is a csm6800 sector, or AuxDRCCoverIncluded == 0
               in RevB TCA
            */
      }
      }
    }
  }

  if ( aux_drc_cover_incl_in_all_aset == TRUE )
      {
    new_dtx_frame_offset = hdrrmac_get_frame_offset();
      /* dtx frame offset is frame_offset if “AuxDRCCoverIncluded” field
         in the TCA “=1” for all the ASET pilots
        */
      }
  else
  {
    new_dtx_frame_offset = hdrrmac_get_frame_offset() % 2;
  }

  hdrrmac.dtx_frame_offset = new_dtx_frame_offset;
    /* update dtx_frame_offset */
  
 
  if  ( ( drc_length < HDRRMAC_DTX_DRC_LENGTH_MIN_IN_SLOTS) && tca_for_soft_ho )
  {
    /* We don't want to enable DTX if drc length is less than 4 slots
       drc_lengh in TCA: 0 -> 1 slot, 1->2 slot, 2->4 slots, 3->8 slots.
       The drc_length returned from fmac is converted to slots already
    */
    hdrrmac_set_device_only_dtx_mode( FALSE );
      /* disable dtx mode if drc_length is too short after handoff */
  }
  

  /* The requirement to boost/deboost DRC channel gain is listed below. 
     IF {“AuxDRCCoverIncluded” field in the TCA “=1” for all the ASET pilots} OR
      {DRCLength>4} OR {FrameOffset%4=0/1}
          DRC channel gain as normal;
     ELSE
          DRC channel gain is boosted by 2dB on two of the half-slots and reduced
          by 2dB on another two of the half-slots and normal for the rest of the
          half-slots for each subframe 
   */

   if ( ( aux_drc_cover_incl_in_all_aset == TRUE ) || 
        ( hdrrmac.dtx_drc_length > HDRRMAC_DTX_DRC_LENGTH_MIN_IN_SLOTS ) ||
        ( ( hdrrmac_get_frame_offset() % 4 )< 2 )
      )
   {
     new_drc_boost_flag = FALSE;
   }
   else
   {
     new_drc_boost_flag = TRUE;
   }

   HDR_MSG_PROT_3( MSG_LEGACY_HIGH, 
                   "DTX:  after TCA drc_length=%d slots dtxFrameOffset=%d drc_boost=%d ",
                 drc_length, new_dtx_frame_offset, new_drc_boost_flag );

   if ( tca_for_soft_ho && 
        ( hdrrmac.dtx_drc_boost != new_drc_boost_flag ) &&
        ( hdrrmac.device_only_dtx_mode == TRUE )
      )
   {

     /* We only change the flag to boost/deboost DRC channel gain after soft handoff. In HHO  
        or conn set up, DTX monitoring would restart. We may change the flag at the time when
        DTX mode is enabled.
     */

     hdrrmac_dtx_update_drc_boost( new_drc_boost_flag );   
   }

   hdrrmac.dtx_drc_boost = new_drc_boost_flag;
     /* Update the flag stored interally */
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */ 

}/* hdrrmac_update_dtx_frame_offset */
#endif /*  FEATURE_HDR_DEVICE_ONLY_DTX || FEATURE_HDR_REVB_DTX_MODE 
|| FEATURE_HDR_TRAFFIC_DTX */

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
/*==========================================================================

FUNCTION  HDRRMAC_FL_ACTIVITY_NOTIFY_DTX

DESCRIPTION
  FL activity notification function from FMAC.
    
DEPENDENCIES
  None

INPUTS
  TRUE:  a FL packet is received, i.e. FL is acitive again
  FALSE: FL is idle for a period of time

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_fl_activity_notify_dtx( boolean is_fl_active )
{
  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
  {
    switch (hdrrmac.subtype)
    {
#ifdef FEATURE_HDR_REVA_L1
      case HDRSCP_SUBTYPE_3_RTCMAC:
#ifdef FEATURE_HDR_REVB_SUPPORTED
      case HDRSCP_SUBTYPE_4_RTCMAC:
#endif /* FEATURE_HDR_REVB_SUPPORTED */
        hdrrmac.is_fl_active_dtx = is_fl_active;
        HDR_MSG_PROT_2( MSG_LEGACY_HIGH,
                       "DTX: fl activity notification = %d, hstr=%d", 
                        hdrrmac.is_fl_active_dtx, HDRHSTR_GET_COUNT() );

        if ( is_fl_active )
        {
          /* FL is active again, disable FLEXCONN */
          hdrrmac_set_device_only_dtx_mode( FALSE );
        }
        break;

#endif /* FEATURE_HDR_REVA_L1 */
      default:
        HDR_MSG_PROT_1( MSG_LEGACY_FATAL, "Device DTX for subtype %d RTCMAC not supported", 
                        hdrrmac.subtype);
    }

  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_ERROR, 
      "RMAC is not active, ignoring fl_activity_cb");
  }
} /* hdrrmac_fl_activity_notify_dtx */

/*==========================================================================

FUNCTION  HDRRMAC_SET_DEVICE_ONLY_DTX_MODE

DESCRIPTION
  This function calls driver to send a message to FW to enable or disable
  sDTX mode
    
DEPENDENCIES
  None

INPUTS
  dtx_mode:  sdtx mode, TRUE means enabling sDTX

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_set_device_only_dtx_mode( boolean dtx_mode )
{
  /* Temporary command buffer. */
  hdrrmac_cmd_union_type cmd_buf;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 
  if( rex_self() != HDRRMAC_TASK_PTR )
  {
    /* Post command to perform suspend connection processing in HDRTX 
       context */

    memset ( &cmd_buf, 0, sizeof (hdrrmac_cmd_union_type ) );

    cmd_buf.hdr.cmd    = HDRRMAC_SET_DTX_CMD;
    cmd_buf.hdr.sender = HDRHAI_RTC_MAC_PROTOCOL;
    cmd_buf.set_dtx.dtx_mode = dtx_mode;

    HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );
  }
  else
  {
    if ( hdrrmac.device_only_dtx_mode == dtx_mode)
    {
      return;
    }

    if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
    {
      switch (hdrrmac.subtype)
      {
#ifdef FEATURE_HDR_REVA_L1
         case HDRSCP_SUBTYPE_3_RTCMAC:
           hdrrmac.device_only_dtx_mode = dtx_mode;
           hdrrmac3_update_dtx_mode();
           break;
#endif /* FEATURE_HDR_REVA_L1 */

#ifdef FEATURE_HDR_REVB_SUPPORTED
         case HDRSCP_SUBTYPE_4_RTCMAC:
           hdrrmac.device_only_dtx_mode = dtx_mode;
           hdrrmac4_update_dtx_mode();
           break;

#endif /* FEATURE_HDR_REVB_SUPPORTED */
                default:
          HDR_MSG_PROT_1( MSG_LEGACY_FATAL, "Device DTX for subtype %d RTCMAC not supported", 
                          hdrrmac.subtype);
      }
    }/* If (hdrrmac.state != HDRRMAC_INACTIVE_STATE )*/
  } 

} /* hdrrmac_set_device_only_dtx_mode */

/*==========================================================================

FUNCTION  HDRRMAC_DTX_UPDATE_DRC_BOOST

DESCRIPTION
  This function sends DTX_CFG message to FW to update drc boost flag
    
DEPENDENCIES
  None

INPUTS
  new_drc_boost_flag:  wheter to boost/deboost DRC channel gain

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_dtx_update_drc_boost( boolean new_drc_boost_flag )
{
  /* Firmware command */
  hdrfw_msg_u fw_cmd;

  /* Error status */
  errno_enum_type estatus;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 
  if (  hdrrmac.device_only_dtx_mode == TRUE )
  {
    /* Initialize DTXMode configuration message */
    memset ( &fw_cmd, 0, sizeof(hdrfw_msg_u) );
    msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_RMAC, HDR_FW_RMAC_DTXMODE_CFG_CMD );

    fw_cmd.rmac_dtxmode_cfg.dtxConfig = 0xC000;
       /* Bit 15 15 enables DTX, bit14 enables PA control. We could update drc boost
          flag only when DTX mode is enabled. So set dtxConfig to enabled.
       */

    fw_cmd.rmac_dtxmode_cfg.dtxFrameOffset = hdrrmac.dtx_frame_offset;

    fw_cmd.rmac_dtxmode_cfg.drcGainBoostCfg = (uint8)new_drc_boost_flag;

    fw_cmd.rmac_dtxmode_cfg.flexConnEnable = hdrrmac.flexconn_mode;
      /* keep flexconn_mode same as before. We won't want to touch it here */

    HDR_MSG_PROT_4( MSG_LEGACY_HIGH, 
                    "DTX: Update DRC boost dtxConfig 0x%x frame offset %d drc_boost %d HSTR %d",
                    fw_cmd.rmac_dtxmode_cfg.dtxConfig,
                    fw_cmd.rmac_dtxmode_cfg.dtxFrameOffset,
                    fw_cmd.rmac_dtxmode_cfg.drcGainBoostCfg,
                    HDRHSTR_GET_COUNT()
                   );
    /* Send DTXMode config command to firmware */
    estatus = msgr_send ( &fw_cmd.hdr, sizeof(hdrfw_msg_u) );
    ASSERT ( estatus == E_SUCCESS );
  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_ERROR, "DTX: no need to update drc_boost when DTX is disabled "); 
  }

} /* hdrrmac_dtx_update_drc_boost */

/*===========================================================================

FUNCTION HDRRMAC_GET_FRAME_OFFSET                                    EXTERNAL

DESCRIPTION
  This function gets the RMAC frame offset.

DEPENDENCIES
  None
  
PARAMETERS
  None

RETURN VALUE
  frame_offset.
  
SIDE EFFECTS
  None.
===========================================================================*/
uint8 hdrrmac_get_frame_offset( void )
{
  uint8 frame_offset = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  switch (hdrrmac.subtype)
  {

    case HDRSCP_DEFAULT_RTCMAC:
    case HDRSCP_SUBTYPE_1_RTCMAC:
      HDR_MSG_PROT( MSG_LEGACY_ERROR, 
        "This function not supported for subtype 0/1" );
      break;

#ifdef FEATURE_HDR_REVA_L1
    case HDRSCP_SUBTYPE_3_RTCMAC:
      frame_offset = hdrrmac3_get_frame_offset( );
      break;

#ifdef FEATURE_HDR_REVB_SUPPORTED
    case HDRSCP_SUBTYPE_4_RTCMAC:
      frame_offset = hdrrmac4_get_frame_offset();
      break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
        "This function not supported for subtype %d", 
        hdrrmac.subtype );

  }
  return frame_offset;
} /* hdrrmac_get_frame_offset */

/*==========================================================================

FUNCTION  HDRRMAC_STOP_DEVICE_ONLY_DTX_MONITOR

DESCRIPTION
  Stop monitoring RL/FL and turn off DTX mode
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_stop_device_only_dtx_monitor( void )
{

  if ( hdrrmac.state == HDRRMAC_INACTIVE_STATE)
  {
    HDR_MSG_PROT( MSG_LEGACY_ERROR, 
                  "FLEXCONN/DTX: stop device_only_dtx_monitor in inactive, no op");
    return;
  }

  if (    hdrrmac.subtype == HDRSCP_SUBTYPE_3_RTCMAC 
#ifdef FEATURE_HDR_REVB_SUPPORTED
       || hdrrmac.subtype == HDRSCP_SUBTYPE_4_RTCMAC
#endif /* FEATURE_HDR_REVB_SUPPORTED */
     )
  {
    hdrrmac.dtx_nest_count++;
    if ( hdrrmac.is_dtx_nv_enabled &&  hdrrmac.dtx_monitor_started ) 
    {
      hdrfmac_dereg_fl_active_cb( HDRFMAC_FL_ACTIVE_CB_DEVICE_DTX,FALSE );
      hdrrmac.dtx_monitor_started = FALSE;
      hdrrmac_set_device_only_dtx_mode(FALSE);      
    }

    if ( hdrrmac.is_dtx_nv_enabled )
    {

      HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
        "FLEXCONN/DTX: stop device_only_dtx_monitor dtx_nested %d",
         hdrrmac.dtx_nest_count );
    }
  }
} /* hdrrmac_stop_device_only_dtx_monitor */

/*==========================================================================

FUNCTION  HDRRMAC_START_DEVICE_ONLY_DTX_MONITOR

DESCRIPTION
  Start to monitor RL/FL and decide whether to turn on/off DTX mode
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_start_device_only_dtx_monitor( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrrmac.state != HDRRMAC_INACTIVE_STATE )
  {
    if ( hdrrmac.is_tap_call == TRUE )
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH,
                    "FLEXCONN/DTX: don't enable flexconn or dtx in tap call");
      return;
    }
    if (    hdrrmac.subtype == HDRSCP_SUBTYPE_3_RTCMAC 
#ifdef FEATURE_HDR_REVB_SUPPORTED
         || hdrrmac.subtype == HDRSCP_SUBTYPE_4_RTCMAC
#endif /* FEATURE_HDR_REVB_SUPPORTED */
       )
    {
      if ( hdrrmac.is_dtx_nv_enabled )
      {
        HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
          "FLEXCONN/DTX: start device_only_dtx_monitor dtx_nested %d ",
           hdrrmac.dtx_nest_count );
      }

      if ( hdrrmac.dtx_nest_count == 0 )
      {
        HDR_MSG_PROT( MSG_LEGACY_ERROR," flexconn_nest_count < 1 !!");
      }
      else if ( hdrrmac.dtx_nest_count > 1 )
      {
        /* There is more than one nested dtx stop. We don't want to start dtx
           monitoring here.See comments in flexconn about how the nested start/stop
           dtx could happen.
        */
        hdrrmac.dtx_nest_count --;
      }
      if ( hdrrmac.dtx_nest_count == 1)
      {
        /* dtx_nested_count == 1, we are at the right place to start flexconn monitoring
        */
        if ( hdrrmac.is_dtx_nv_enabled && !hdrrmac.dtx_monitor_started 
#ifdef FEATURE_HDR_REVB_DTX_MODE
             && ( !hdrrmac.network_dtx_mode || 
                 ( hdrrmac.subtype == HDRSCP_SUBTYPE_3_RTCMAC ) )
#endif /* FEATURE_HDR_REVB_DTX_MODE */
        )
        {
          hdrrmac.is_fl_active_dtx = TRUE;
            /* Assume fl is active first */
          hdrfmac_reg_fl_active_cb( hdrrmac_fl_activity_notify_dtx,
                                  HDRFMAC_FL_ACTIVE_CB_DEVICE_DTX, FALSE );
          hdrrmac.dtx_monitor_started = TRUE;
        }
      } /* hdrrmac.dtx_dtx_nest_count == 1 */

    } /* hdrrmac.subtype == HDRSCP_SUBTYPE_4_RTCMAC */
  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_ERROR, 
      "RMAC is not active, ignoring fl_activity_cb");
  }

} /* hdrrmac_start_device_only_dtx_monitor */
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */

#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
/*==========================================================================

FUNCTION  HDRRMAC_START_FLEXCONN_DTX_MONITOR

DESCRIPTION
  Start to monitor RL/FL and decide whether to turn on/off FLEXCONN mode or
  DTX mode
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_start_flexconn_dtx_monitor
(
   boolean resume_timer_for_flexconn
     /* whether to resume FL activity timer */
)
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 
#ifdef FEATURE_HDR_FLEX_CONN
  hdrrmac_start_flexconn_monitor( resume_timer_for_flexconn );
#endif /* FEATURE_HDR_FLEX_CONN */

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
  hdrrmac_start_device_only_dtx_monitor();
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */  

} /* hdrrmac_start_flexconn_dtx_monitor */


/*==========================================================================

FUNCTION  HDRRMAC_STOP_FLEXCONN_DTX_MONITOR

DESCRIPTION
  Stop monitoring RL/FL and turn off FLEXCONN mode or DTX mode
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac_stop_flexconn_dtx_monitor( boolean suspend_timer_for_flexconn )
{
    
#ifdef FEATURE_HDR_FLEX_CONN
  hdrrmac_stop_flexconn_monitor( suspend_timer_for_flexconn );
#endif /* FEATURE_HDR_FLEX_CONN */

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
  hdrrmac_stop_device_only_dtx_monitor();
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */
  
} /* hdrrmac_stop_flexconn_dtx_monitor */
#endif /*(FEATURE_HDR_FLEX_CONN)||(FEATURE_HDR_DEVICE_ONLY_DTX)*/

#ifdef FEATURE_HDR_MODEM_API
/*==========================================================================

FUNCTION  HDRRMAC_PROCESS_REPORT_COST_OF_MODEM_POWER_EST_CMD

DESCRIPTION
  This function calculates and reports estimated cost of modem power
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  Estimated cost of modem power LOW, MEDIUM, HIGH

SIDE EFFECTS
  None

==========================================================================*/
hdrrmac_cost_of_power_est_enum_type 
hdrrmac_process_report_cost_of_modem_power_est_cmd( void )
{
   /* Initialize to invalid value */
   hdrrmac_cost_of_power_est_enum_type cost_of_power_est = 
      HDRRMAC_EST_COST_OF_POWER_IS_INVALID;

   #ifdef FEATURE_HDR_MODEM_API_UT
   /* For Unit Testing print below statement, will be removed later */
   HDR_MSG_PROT_2(MSG_LEGACY_ERROR,"FHMA: Entered Cost of Modem Power."
                                   "RMAC State=%d,RMAC TYPE=%d",
                  hdrrmac.state, hdrrmac.subtype);
   #endif /* FEATURE_HDR_MODEM_API_UT */

   /* This command is expected to be executed when AT is in Idle or Connected */
   switch (hdrrmac.subtype)
   {
      case HDRSCP_DEFAULT_RTCMAC:
      case HDRSCP_SUBTYPE_1_RTCMAC:
         cost_of_power_est = hdrrmac0_report_cost_of_modem_power_est();
         break;
#ifdef FEATURE_HDR_REVA_L1
      case HDRSCP_SUBTYPE_3_RTCMAC:
         cost_of_power_est = hdrrmac3_report_cost_of_modem_power_est();
         break;
#ifdef FEATURE_HDR_REVB_SUPPORTED
      case HDRSCP_SUBTYPE_4_RTCMAC:
         cost_of_power_est = hdrrmac4_report_cost_of_modem_power_est();
         break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */
      default:
         HDR_MSG_PROT_1(MSG_LEGACY_FATAL,
                        "Subtype %d RTCMAC not supported", hdrrmac.subtype);
         break;
   }

   #ifdef FEATURE_HDR_MODEM_API_UT
   /* For Unit Testing print below statement, will be removed later */
   HDR_MSG_PROT_2(MSG_LEGACY_ERROR,"FHMA: Exiting Cost of Modem Power."
                                   "RMAC State=%d,RMAC TYPE=%d",
                  hdrrmac.state, hdrrmac.subtype);
   #endif /* FEATURE_HDR_MODEM_API_UT */

   return cost_of_power_est;
}
#endif /* FEATURE_HDR_MODEM_API */


#ifdef FEATURE_HDR_MODEM_API_RL_INFO
/*===========================================================================
FUNCTION HDRRMAC_GET_RL_RATE_INFO

DESCRIPTION
  This function populates the following in the passed struct:
  1. Estimated reverse link throughput in kbps
  2. Flag indicating if throughput greater than estimated rate may be possible
  3. Current reverse link throughput in kbps
  4. RLP queue length
  
DEPENDENCIES
  None
 
PARAMETERS
  Pointer to location where reverse link rate info is to be populated

RETURN VALUE 
  E_SUCCESS       - if valid rate information is populated at the passed 
                    pointer location
  E_NOT_AVAILABLE – Rate information is not available cannot be returned 
                    since AT is not in traffic state
  E_BAD_ADDRESS   - Passed argument is an invalid/NULL pointer

SIDE EFFECTS
  None
===========================================================================*/
errno_enum_type hdrrmac_get_rl_rate_info
(
  hdrrmac_rl_rate_info_struct_type * rl_info_ptr
)
{
  errno_enum_type return_val = E_NOT_AVAILABLE; 
    /* Function return value. */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Sanity check */
  if( rl_info_ptr == NULL )
  {
    HDR_MSG_PROT(MSG_LEGACY_ERROR, 
      "Invalid input pointer passed to RL rate info function" );

    return E_BAD_ADDRESS;
  }

  if ( hdrrmac.state == HDRRMAC_OPEN_STATE )
  {
    switch (hdrrmac.subtype)
    {
      case HDRSCP_DEFAULT_RMAC:
      case HDRSCP_SUBTYPE_1_RTCMAC:
        hdrrmac0_get_rl_rate_info( rl_info_ptr );
        break;

#ifdef FEATURE_HDR_REVA_L1
      case HDRSCP_SUBTYPE_3_RTCMAC:
        hdrrmac3_get_rl_rate_info( rl_info_ptr );
        break;

#ifdef FEATURE_HDR_REVB_SUPPORTED    
      case HDRSCP_SUBTYPE_4_RTCMAC:
        hdrrmac4_get_rl_rate_info( rl_info_ptr );
        break;
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* FEATURE_HDR_REVA_L1 */

      default:
        HDR_MSG_PROT_1(MSG_LEGACY_FATAL, 
          "Subtype %d RTCMAC not supported", 
          hdrrmac.subtype );
    }

    return_val = E_SUCCESS;
  }
  else
  {
    HDR_MSG_PROT_1(MSG_LEGACY_ERROR, 
      "RL rate info can only be provided in open state. Current state: %d",
      hdrrmac.state);
  }

  return return_val;

} /* hdrrmac_get_rl_rate_info */

#endif /* FEATURE_HDR_MODEM_API_RL_INFO */

/*=============================================================================
FUNCTION HDRRMAC_OVERRIDE_REV_LINK_CARRIER_SET                     EXTERNAL

DESCRIPTION
  This function is a wrapper to rfm_hdr_update_rev_link_carrier_set() which
  updates RF of the reverse link carrier set.

DEPENDENCIES
  None.

PARAMETERS
  enabled - If override to 1 carrier is enabled
  chan_list - List of rev links channels to override to
  chan_cnt  - Number of channels in the list. If chan_cnt is 0, this is
              called because of a new TCA. In this case, the TCA processing
              will automatically cancel the override

RETURN VALUE
  None

SIDE EFFECTS
  None.
==============================================================================*/
void hdrrmac_override_rev_link_carrier_set
(
  boolean enabled,
  sys_channel_type chan_list[],
  uint8 chan_cnt
)
{
  HDR_MSG_PROT_3( MSG_LEGACY_MED, "Rev Link Override: %d=>%d, chan:%d", 
                  hdrrmac_rev_link_carrier_override, enabled, chan_cnt );

  if ( hdrrmac_rev_link_carrier_override != enabled )
  {
    hdrrmac_rev_link_carrier_override = enabled;

    if ( chan_cnt != 0)
    {
      rfm_hdr_update_rev_link_carrier_set(
           hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN),
           chan_list,
           chan_cnt );
    }
  }
}

#ifdef FEATURE_HDR_QTA
/*=============================================================================
FUNCTION HDRRMAC_DEACTIVATE_ON_TUNEBACK                     EXTERNAL

DESCRIPTION
  This function post pending deactivate command after HDR-SRCH tuneback

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.
==============================================================================*/
void hdrrmac_process_cmd_on_tune_back( void )
{
  /* Temporary command buffer. */
  hdrrmac_cmd_union_type cmd_buf;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( ( hdrrmac.tune_away_info.pend_on_ta & HDRRMAC_DEACTIVATE_CMD_PENDING ) || 
    ( hdrrmac.tune_away_info.pend_on_ta & HDRRMAC_SHUTDOWN_CMD_PENDING ) )
  {
    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
                  "Command id pending in RMAC = %d",hdrrmac.tune_away_info.pend_on_ta);

    if (( hdrsrch_is_ta_started() == TRUE ) && ( hdrsrch_is_qta_enabled() == TRUE ))
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "TA still inprogress, delaying cmd");
    }
    else
    {
      if ( hdrrmac.tune_away_info.pend_on_ta & HDRRMAC_SHUTDOWN_CMD_PENDING )
      {
        /* Make sure we are not still in QTA TA when we finish shutdown cmd */
      
        cmd_buf.hdr.cmd = HDRRMAC_SHUTDOWN_CMD;
        cmd_buf.hdr.sender = HDRHAI_RTC_MAC_PROTOCOL;
      
        HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );
        hdrrmac.tune_away_info.pend_on_ta &= ~HDRRMAC_SHUTDOWN_CMD_PENDING;
        /* Only one cmd will be received and processed at a time and hence reset the flag */
        HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
                  "Shutdown command queued, pending cmd id on ta = %d",hdrrmac.tune_away_info.pend_on_ta);
      }/* if ( hdrrmac.tune_away_info.pend_on_ta & HDRRMAC_SHUTDOWN_CMD_PENDING ) */

      if ( hdrrmac.tune_away_info.pend_on_ta & HDRRMAC_DEACTIVATE_CMD_PENDING )
      {
        /* Make sure we are not still in QTA TA when we finish deactivate cmd */
      
        cmd_buf.hdr.cmd = HDRRMAC_DEACTIVATE_CMD;
        cmd_buf.hdr.sender = HDRHAI_RTC_MAC_PROTOCOL;
        HDRRMAC_QUEUE_CMD( HDRHAI_RTC_MAC_PROTOCOL, &cmd_buf, sizeof(cmd_buf) );

        hdrrmac.tune_away_info.pend_on_ta &= ~HDRRMAC_DEACTIVATE_CMD_PENDING;
        HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
                  "Deactivate command queued, pending cmd id on ta = %d",hdrrmac.tune_away_info.pend_on_ta);
      }/* if ( hdrrmac.tune_away_info.pend_on_ta & HDRRMAC_DEACTIVATE_CMD_PENDING ) */

    }/* else ( hdrsrch_is_ta_started() == TRUE ) && ( hdrsrch_is_qta_enabled() == TRUE )*/
  }/* if ( hdrrmac.tune_away_info.pend_on_ta & HDRRMAC_DEACTIVATE_CMD_PENDING ) || 
    ( hdrrmac.tune_away_info.pend_on_ta & HDRRMAC_SHUTDOWN_CMD_PENDING )*/
 }


/*=============================================================================
FUNCTION HDRRMAC_IS_TX_ON                                        EXTERNAL

DESCRIPTION
  This function checks if TX is on.

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  TRUE: if TX is ON ( TX_POWERING_UP, TX_ON, TX_OFF_RF_ON )
  FALSE: if TX is OFF ( both RF & SW )

SIDE EFFECTS
  None.
==============================================================================*/
boolean hdrrmac_is_tx_on( void )
{
 
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrrmac.rf_status == TX_OFF )
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
 
}
#endif /* FEATURE_HDR_QTA */

