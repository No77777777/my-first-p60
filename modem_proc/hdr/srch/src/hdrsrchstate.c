/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                       H D R    S R C H    S T A T E

                           Search State Module

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2000 - 2018  by Qualcomm Technologies, Incorporated.
                All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchstate.c#2 $
$DateTime: 2020/04/27 09:08:35 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ------------------------------------------------------- 
04/22/2020   vaa     Migrate to dog_hb_report as dog_report API is deprecated
06/26/2018   vke     Changes to process slam response status from fw
02/01/2018   svu     Clearing the variant id value in cmd rcvd
02/01/2018   svu     Added check for Variant ID comparision for processing LTE cmds
01/30/2018   vko     Fixed D2L deinit confirmation for SUB 2
01/29/2018   vko     Fixed HDR sending RF status indication to incorrect LTE SUB ID
08/24/2017   rmv     Mainline FEATURE_HDR_QTA_THRU_TRM 
10/12/2016   rmv     Added timed_wait_for_high_pri_rsp for debug purpose 
07/04/2016   vko     Reduce F3 msgs
06/06/2016   vlc     Cleaned up TIMED_WAIT_SIG in wait functions. 
02/09/2016   vlc     Added debug code to catch TIMED_WAIT_SIG getting set.
02/08/2016   vke     Changes to move less significant f3 logging 
                     to legacy low
10/06/2015   wsh     Added debug code to catch INT_CMD_SIG getting removed 
08/17/2015   vlc     Added code to handle Run Control Event Notification errors.
08/17/2015   vlc     Mainlined FEATURE_MODEM_RCINIT.
08/05/2015   wsh     Added per band antenna imbalance support 
07/08/2015   vlc     Added call to hdrsrchtclte_powerup_init().  
06/30/2015   wsh     Added WTR hopping support
06/26/2015   vke     Added new function hdrsrch_timed_wait_with_sig_status()
06/18/2015   tnp     Fix compiler warnings
06/12/2015   bb/vlc  Added hdrsrch_is_in_suspend(). 
06/09/2015   tnp     Update feature flag configuration
05/28/2015   vlc     Added support for DO->LTE redirection in connected state.
05/15/2015   vko/vke Added TABASCO to featurization
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
05/07/2015   vlc     Added processing of HDRSRCH_ABORT_LTE_RESELECT_CMD. 
04/27/2015   kss     Added support for dog HB API.
05/04/2015   mbs     Supported QTA using TRM API
04/27/2015   vke     Added f3 in the TCXOMgr grant callback  and processing
                     of grant by AFC SM
04/01/2015   mbs     Supported RF interface change
03/16/2015   wsh     Reduced stack usage to avoid overflow 
02/25/2015   wsh     Replaced efs API with mcfg version
01/22/2015   arm     Added Band Avoidance feature control NV. 
01/12/2015   wsh     Added L1/FW interface to confirm ASDiv prio ended
01/05/2015   sat     Changes to wait for TX_STOP response from FW (RUMI).
12/17/2014   sat     Changes to fix compilation errors in Tabasco
12/03/2014   ljl     Declared system lost if CC packet HSTR was incorrect.
11/04/2014   arm     Band avoidance: move power info saving to hdrsrch task and
                     enable BA only for DSDA mode.
10/14/2014   wsh     Changes to support Jolokia bringup
10/01/2014   arm     Changes to support HDR band avoidance.
06/04/2014   arm     Changes to support DO to G QTA
05/15/2014   arm     TRM API changes for DR-DSDS.
05/05/2014   ljl     Added HDRSRCH_CHANNEL_CHANGE_AND_NSET_UPDATE_CMD.
04/25/2014   ljl     Supported finger assignment and slam message.
03/05/2014   wsh     Bug fixes for DSDS-ASDiv and DSDA-ASDiv support 
01/22/2014   ljl     Fixed crash when aborting cmd. 
01/20/2014   vlc     Changed timer interface. 
01/17/2014   dsp     Cleaned up abort_command() code.  
12/20/2013   vlc     Added support for Tbackoff timer optimization. 
11/27/2013   dsp     FR17482 for Page blocking management.
10/08/2013   smd     Aborted pending cmd in supend state during deactivation. 
10/08/2013   mbs     Added support for RUMI_TX_ONLY
09/25/2013   wsh     Changes to support BOLT bring up
09/04/2013   wsh     Changes to support BOLT bring up
08/08/2013   rmv     Featurized tmc APIs with ifndef FEATURE_MODEM_RCINIT
09/03/2013   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
06/26/2013   wsh     Added Selective TxD feature
04/22/2013   arm     Added changes for DSDA coexistence manager.
05/06/2013   mbs     Fixed compiler errors for BOLT build
05/02/2013   ljl     Added HDRSRCH_SET_SCC_INFO_CMD.
12/20/2012   vko     Fixed compiler warnings 
12/19/2012   smd     Added RUMI Tx support.
12/04/2012   mbs     Fixed compilation errors due to removal of LTE on Triton
10/18/2012   arm     Do not abort channel change command prior to 
                     command completion.  
08/15/2012   vlc     Do not abort TCA command prior to command completion. 
08/29/2012   ljl     Added RTC Offset valid support.
06/29/2012   dsp     New offline notify processing is added. 
05/22/2012   smd     Fixed a bug in cmd purge when both tx and srch accessing
                     command queue.
05/21/2012   ljl     Init IRATMan for RCInit.
04/02/2012   arm     Supported diversity in reacq.
04/25/2012   smd     Changes to repost some commands when purging command queue.
04/30/2012   dsp     Fixed compiler warnings on Dime. 
03/22/2011   wsh     Do not abort deactivate command 
03/22/2012   arm     Fixed featurization bug for RCINIT. 
03/20/2011   wsh     Do not abort deactivate command 
03/08/2012   wsh     Merged NLB support
03/20/2012   cnx     Resolved TCB compiler errors for Dime. 
03/16/2012   arm     Resolve TCB errors for Dime. 
03/19/2012   ljl     Fixed Dime compiler errors.
03/14/2011   rmg     Added debug code to save tramp interrupt info when 
03/05/2012   arm     Fixed RCINIT bugs found during integration. 
                     timed wait fails.
03/05/2012   vlc     Added IRATMan interface to D2L reselection feature. 
02/27/2012   ljl     Added F3 message for debugging.
01/09/2012   smd     Supported dynamic acquisition diversity. 
11/29/2011   arm     Removed compiler warnings.
11/29/2011   kss     Fixed compiler warning.
11/27/2011   grl     Removed obsolete sleep API.
11/18/2011   wsh     Init critical section before entering in ts_invalidate()
11/11/2011   arm     HDR idle and access state diversity implemetation.
11/07/2011   smd     Add NV items for idle/access diversity.
11/09/2011   lyl     Checked FW state before sending SCC config message.
11/03/2011   ljl     Set HDRSRCH_TICKLE_SIG for on_searcher_idle functions. 
10/28/2011   lyl     Added hdrsrch_set_fing_and_div_limita
10/24/2011   ljl     Supported RPM optimized timeline.
10/10/2011   lyl     Supported ASP update response message.
10/17/2011   rmg     Replaced obsolete clk APIs with DAL APIs.
09/23/2011   ljl     Added HDRSRCH_SET_CC_START_CMD, HDRSRCH_SET_SCC_OFFSET_CMD,
                     and HDRSRCH_AT_SET_DRC_CMD.
08/02/2011   ljl     Cleared HDRSRCH_TIMED_WAIT_SIG before wating for high pri rsp.
06/01/2011   kss     Revert temp NikeL bringup changes.
04/27/2011   rmg     Added HDR MCPM support changes.
03/16/2011   smd     Included msm.h. 
02/17/2011   vlc     Added LTE deinit handling to 
                     hdrsrch_default_process_low_pri_rsp_ind(). 
01/21/2011   ljl     Declared system lost if FW requests reset.
12/21/2010   kss     Fixed compiler issues on NikeL. 
10/29/2010   ljl     Fixed compiler warning. 
10/21/2010   arm     Replace TASKLOCK  with critical section. 
10/08/2010   ljl     Declared fatal error if the expected signal was not received.
10/08/2010   ljl     Cleared HDRSRCH_TIMED_WAIT_SIG before starting 
                     the timer gp_timer2.
10/04/2010   grl     Added HW register dumping during searcher stall scenarios.
09/22/2010   lyl     Fixed compiler warning. 
09/15/2010   ljl     Supported dynamic wdog.
09/14/2010   arm     Replace TASKLOCK  with critical section. 
09/01/2010   vlc     Added support for DO->LTE pilot measurements/reselection.
06/24/2010   cnx     Added hdrsrch_sm.smh and hdrsrchcom_sm.smh.
06/21/2010   cnx     Fixed compiler warnings.
06/03/2010   cnx     Fixed Klocwork errors.
04/16/2010   lyl     Updated generic timed waiting for signal function.
04/08/2010   smd     Added hdrsrch_wait_for_sigs() in mainline.
04/08/2010   rkc     Moved NPA creating of client to ACQ.
03/24/2010   pxu     Removed FEATURE_HDR_QSM features and 
                             FEATURE_HDR_EQ feature checks
03/18/2010   smd     Added channel info in pilot meas rsp for failure cases.
03/10/2010   smd     Removed code of copying message struct to command struct.
03/08/2010   smd     Fixed IRAT Pilot Measurement bugs.
02/11/2010   ljl     Updated F3 message.
01/27/2010   smd     Added LTE to DO pilot measurement feature.
01/08/2010   ljl     Removed the legacy SRCH FW interface.  
01/07/2010   lyl     Used the FWS API.
12/11/2009   grl     Added FW state config support
12/08/2009   wsh     Fixed dog timeout after TASK_STIOP_SIG 
12/07/2009   ljl     Supported FWS.
12/01/2009   grl     Remove dump count error fataling during bringup.
11/11/2009   ljl/lyl Renamed hdrmdsp_process_enable_rsp() to 
                     hdrmdsp_process_hdr_app_cfg_rsp()
11/11/2009   rkc     Created NPA client at task init.
11/09/2009   ljl     Initialized sleepctl, trm, tcxomgr for MDM9K bringup.
                     Passed fll lock status from acq response to AFC stm.
10/21/2009   rkc     Moved mdsp init back to srch task init.
10/19/2009   rkc     Removed hdrmdsp init from task startup.
10/07/2009   ljl     Fixed the msg pointer to srch4 driver.
                     Added message buffers.
10/01/2009   ljl     Processed srch app cfg rsp msg.
10/01/2009   lyl     Added diag for RUMI bringup.
09/17/2009   ljl     Passed the msg pointer to srch4.
09/14/2009   ljl     Supported srch MSGR interface.
08/07/2009   lyl     Added hdrsrch_wait_for_sig().
06/30/2009   lyl     Updated for generic responses process.
06/23/2009   lyl     Fixed the lint errors.
06/19/2009   grl     Switched to the hdrsrchrfdiv API.
05/18/2009   ljl/lyl Updated for firmware MSGR interface.
04/29/2009   rkc     Cleared wdog signal and restarted wdog timer in
                     hdrsrch_wait_for_sigs().
04/16/2009   rkc     Removed T_MSM6175 from mainline.
04/14/2009   ljl     Fixed lint error.
04/13/2009   rkc     Added #include clkregim.h.
03/20/2009   rkc     Added hdrsrchstate_set_revb_mode() and
                     hdrsrchstate_is_revb_mode().
03/06/2009   pmk     Added support for MSGR
01/28/2009   lyl     Define RF tune timer during SRCH task initialization.
11/10/2008   kss     Removed/replaced intlocks.
08/19/2008   smd     Added hdrsrch_get_current_state.
07/15/2008   lyl     Fixed lint error (506: constant value boolean )
05/06/2008   rkc     Turn on DEMOD clock resource regime before mdsp init.
02/20/2008   rmg     Added support for MSM7225.
02/08/2008   grl     Added full time SHDR support 3.
01/21/2008   ljl     Removed the busy wait during RF tune.
11/20/2007   jyw     Added sleep registration for new sleep api.
11/14/2007   grl     Added full time SHDR support.
11/13/2007   rkc     Fixed handdown mode initialization.
11/06/2007   grl     Fixed HPT mode not being saved properly in idle.
10/17/2007   grl     Merged Rev B changes from branch.
08/28/2007   grl     Fixed external command priority issue.
08/21/2007   rkc     HDRSRCH_INT_CMD_SIG added to rex wait mask.
08/19/2007   grl     Resolved Lint High warnings.
06/24/2007   rmg     Added support for MSM6175.
06/20/2007   rkc     Passed HDR generic payload to internal state machines
04/27/2007   grl     Added support for access hybrid ctrl via the tc mode.
03/08/2007   grl     Fixed RCVT compiler warning.
01/29/2006   ljl     Renamed the function hdrsrch_process_int_cmd_tf() to
                     hdrsrch_process_int_cmd().
01/17/2007   ljl     Added HDR SRCH AFC stm.
12/20/2006   jyw     Added hdrsrch_clear_tune_away_on_searcher_idle.
11/20/2006   jyw     Printed the info if the command processing halts.
08/28/2006   sq      Added FEATURE_HDR_QSM
06/28/2006   jyw     Added the support to MPC.
06/30/2006   jyw     Included "task.h".
06/14/2006   mt      Fixed BCMCS featurization
05/17/2006   jyw     Aborted E_IN_PROGRESS command when deactivated.
04/26/2006   jyw     Added the second level on searcher idle callbacks.
03/30/2006   sq      Initialized BCMCS sleep cycle value to 0
03/22/2006   sq      Added HDRSRCH_UPDATE_BOM_PERIOD_CMD in default command
10/20/2005   jyw     Fixed the lint errors.
09/06/2005   ljl     Added code to check weak HDR coverage at sync state
09/06/2005   ds      Define AFC timer and added AFC signal handling
09/01/2005   grl     Retain lock unlock callback added.
08/11/2005   grl     Mainlined FEATURE_HDR_TCXOMGR. Added hdrsrchafc_init call
07/27/2005   ljl     Called hdrsrchset_init() in hdrsrch_task()
07/27/2005   sq      Added support for BCMCS page monitoring
06/20/2005   jyw     Print a message when on_searcher_idle is pending
06/14/2005   rng     Moved tcxomgr_init to tmc and removed hdrsrchstate_wait().
06/10/2005   hrk     Using dog_report() to pet watchdog. 
05/31/2005   grl     Added support for TRM and TCXOMgr.
04/15/2005   ds      Added function call to initialize equalizers 
03/24/2005   sq      Added check to the return value of hdrsrch_retry_cmd
03/23/2005   hrk     Changes to implement hdrsrch_timed_wait_usecs
01/05/2005   sq      Added TC_ACK_RECEIVED_CMD into default cmd processing
11/29/2004   grl     Added connected state stat logging support.
11/12/2004   sq      Print message before retrying command instead of after
11/04/2004   kss     Call new function in hdrsrchutil for PN roll processing. 
11/01/2004   sq      Retry cmd after RF_TUNE_SIG is removed from wait mask
08/12/2004   ajn     sleephdr -> sleepctl
07/31/2004   sq      Added function hdrsrch_reset_no_dump_cnt
07/30/2004   sq      Added support for FEATURE_HDR_BCMCS
07/22/2004   ajn     Added common signals to state machine
06/21/2004   sq      Re-defined tune_timer as a SCLK timer
05/05/2004   aaj     Implemented DSP halt work-around to halt on known codes
01/26/2004   aaj     Check if RF_TUNE_SIG is pending before re-try command
11/26/2003   aaj     Handle Lost Dump interrupt
11/21/2003   sq      Added function call to "hdrsrchlog_init"
11/18/2003   aaj     f3 msg change 
10/09/2003   ajn     Added support for "Advise Unlock RF" command
10/06/2003   aaj     Mainlined T_MSM6500
09/10/2003   mpa     Converted F3 messages to use new MSG2.0 HDR search SSID
07/30/2003   sq      Merged Henti's changes for MM task support
05/01/2003   aaj     %d to %x change for hdrsrchstate debug message
02/19/2003   ajn     Avoid command processing when tuning is in progress.
08/13/2002   aaj     Added support for E_WOULD_BLOCK to hdrsrch cmds
07/25/2002   aaj     Corrected on_searcher_idle() processing in state loop
06/06/2002   aaj     Print "No dump count" if the count exceeds 1
05/29/2002   ajn     Shortened F3 messages to avoid truncation.
04/29/2002   aaj     Fixed on_searcher_idle. SM stalled 6->10sec. More msgs
04/25/2002   aaj     Init hdrsrchdrv at task start. Create clear_on_dump i/f
04/03/2002   aaj     Check for MDSP halt if LOST DUMP timer expires
04/03/2002   ajn     on_searcher_idle will now never immediate execute func.
02/25/2002   ajn     ERR_FATAL on Searcher or Command State Machine stall.
                     Command SM stall increased to 80 seconds (20 seconds
                     longer than longest legitamate sleep).
02/06/2002   ajn     hdrerrno->errno; complete_cmd marks E_IN_PROGRESS done
01/30/2002   ajn     Moved status into command structure
01/23/2002   ajn     Added purge_and_deactivate.
12/31/2001   ajn     Added _kill_signal, _stop_timer, and _kill_timer funcs.
11/01/2001   aaj     %d to %x change for hdrsrchstate debug message
10/24/2001   ajn     Change state header files to common hdrsrchstates.h
10/19/2001   ajn     Handle non E_SUCCESS codes from retry_cmd.
                     OFFLINE and TASK signals handle from Inactive state.
10/12/2001   ajn     Added check for pending DUMP signal to on_searcher_idle
10/08/2001   ajn     Added command retry on state change
09/15/2001   aaj     Added initialization for hdrsrchutil_init
09/04/2001   ajn     Cmd interface change to allow restarting commands.
08/09/2001   ajn     SLEEP VU Integration
06/19/2001   aaj     Added logging function in PN Roll Signal handler
06/18/2001   ajn     Added TASK OFFLINE/STOP signal handling.
06/06/2001   ajn     Added OnDump and OnSearcherIdle function.
04/24/2001   aaj     Added a dummy dword parameter to hdrsrch_task
03/02/2001   aaj     included customer.h file
11/22/2000   ajn     Code review changes / Added Tickle signal
10/31/2000   ajn     Added General Purpose Signals to State Machine
10/30/2000   ajn     SCLK module initialization added.
10/04/2000   ajn     Added dynamic wait_mask control.  Prevented masked
                     events from being handled when non-masked event causes
                     the rex_wait( ) to return.
07/07/2000   ajn     Created this file

==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"
#include "comdef.h"
#include "DALSys.h"
#include "sys.h"
#include "dog_hb_rex.h"

#ifdef FEATURE_CMI
#include "sys_v.h"
#endif

#include "fs_public.h"
#include "fs_sys_types.h"
#include "fs_errno.h"

#include "hdrsrch.h"
#include "hdrsrchcmd.h"
#include "hdrsrchmsg.h"
#include "hdrsrchstate.h"
#include "hdrsrchstates.h"
#include "hdrts.h"
#include "hdrsrchtask.h"
#include "hdrsrchi.h"
#include "hdrsrchdrv.h"
#include "hdrsrchfing.h"
#include "hdrsrchrf.h"
#include "hdrsrchutil.h"
#include "hdrsrchsync.h"
#include "hdrsrcheq.h"
#include "hdrsrchafc.h"
#include "hdrsrchrfdiv.h"
#include "hdrmdspmcr.h"
#include "hdrmdsp.h"
#include "msgr_umid.h"
#include "hdrmc_v.h"

#include "hdrsrchlog.h"
#ifndef FEATURE_HDR_DIME_MODEM
#include "sclk.h"
#endif /* FEATURE_HDR_DIME_MODEM */
#include "mc.h"

#include "srch4trans.h"

#ifdef FEATURE_CMI
#include "mc_v.h"
#endif

#include "task.h"
#include "tcxomgr.h"

#include "hdrdebug.h"
#include "err.h"


#include "hdrsrchsm.h"
#include "hdrsrch_sm.smh"
#include "hdrsrchcom_sm.smh"
#include "hdrsrchbm.h"
#include "hdrts.h"
#ifndef FEATURE_HDR_DIME_MODEM
#include "hw.h"
#endif /* FEATURE_HDR_DIME_MODEM */
#include "msm.h"

#ifdef FEATURE_HDR_REVB
#include "hdrsrchschdgrp.h"
#endif /* FEATURE_HDR_REVB */

#ifdef FEATURE_MSM_SUPPORT
#include "msm_support.h"
#endif /* FEATURE_MSM_SUPPORT */

#ifdef FEATURE_HDR_TO_LTE
#include "hdrsrchltemeas.h"
#endif /* FEATURE_HDR_TO_LTE */

#include "hdrsrchmcpm.h"

#include "mcpm_api.h"

#include <DDIChipInfo.h>
#include "DALDeviceId.h"

#include "rcinit.h"
#define HDRSRCH_RCEVT_PREFIX "HDRSRCH:"
#define HDRSRCH_RCEVT_READY HDRSRCH_RCEVT_PREFIX "ready"
#define HDRRX_RCEVT_PREFIX "HDRRX:"
#define HDRRX_RCEVT_READY HDRRX_RCEVT_PREFIX "ready"

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV
#include "hdrsrchutil.h"
#endif

#ifndef FEATURE_HDR_DIME_MODEM
/* Debug: for tramp interrupt trace */
#include "tramp.h"
#endif /* FEATURE_HDR_DIME_MODEM */
#include "mmoc_msgr.h"

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
#include "hdrmultirat.h"
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_HDR_RUMI
#include "hdrhitmsg.h"
#endif /* FEATURE_HDR_RUMI */

#include "hdrutil.h"
#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
#include "hdrsrchtclte.h"
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

/* err fatal if INT_CMD_SIG is cleared from wait_mask */
#define HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL( )  hdrsrch_dbg_wait_mask( )

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/
rex_tcb_type*    hdrsrch_tcb_ptr;

#define HDRSRCH_SYS_LOST_ON_LOST_DUMP
  /* Causes system lost when HDR SRCH loses dump interrupt */

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
#define HDRSRCH_ACQ_DIV_THRESH0_DEFAULT        -98
#define HDRSRCH_ACQ_DIV_THRESH1_DEFAULT        -105
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV
#define HDRSRCH_IDLE_STATE_DIV_SUBPKT_THRESH_DEFAULT     4
#define HDRSRCH_IDLE_STATE_DIV_SCI_THRESH_DEFAULT        9
#define HDRSRCH_IDLE_STATE_DIV_PSINR_THRESHOLD_DEFAULT  -3
#endif


/*--------------------------------------------------------------------------
                        SRCH COMMAND EXECUTION STATUS
--------------------------------------------------------------------------*/

typedef enum
{
  HDRSRCH_CMD_EXECUTED,
  HDRSRCH_CMD_NOT_EXECUTED
}
hdrsrch_cmd_executed_enum_type;


/* ----------------------------- Watchdog ------------------------------ */

#undef  DOG_HDRSRCH_RPT
#define DOG_HDRSRCH_RPT hdrsrch_dog_rpt_var
dog_report_type   hdrsrch_dog_rpt_var = 0;
  /* Dog rpt */

#define HDRSRCH_DOG_RPT_TIME_MS 2000
  /* Report to the watchdog every 2 seconds */

typedef struct
{
  boolean                             dump_occurred;
    /* Flag indicating a dump occurred */

  boolean                             cmd_executed;
    /* Flag indicating a command has been processed */

  int4                                no_dump_cnt;
    /* Time in ms with no dumps */

  int4                                no_cmd_cnt;
    /* Time in ms with no commands executed */
}
hdrsrchstate_dog_struct_type;

/* ------------------------ State Machine Data ------------------------- */

typedef struct
{
  const hdrsrch_state_struct_type   * state;
    /* Event Dispatch Table for the current state */

  const hdrsrch_state_struct_type   * next_state;
    /* Next state of the state machine */

  hdrsrch_state_enum_type             next_state_id;
    /* Identifier for next state of state machine */

  uint32                              guard1;
    /* guard to wait_mask */

  rex_sigs_type                       wait_mask;
    /* Signals the Task may be waiting for */

  uint32                              guard2;
    /* guard to wait_mask */

  hdrsrch_on_event_func               on_dump;
    /* Function to call after search dump processing */

  hdrsrch_on_event_func           on_searcher_idle[HDRSRCH_MAX_ON_SEARCH_IDLE];
    /* Function to call when searcher is idle */

  hdrsrchstate_dog_struct_type        dog;
    /* Watch-dog parameters */

  boolean                             is_revb_mode;
    /* TRUE if REVB mode is enabled */

#ifdef FEATURE_HDR_RUMI
  rex_timer_type                      sram_stuck_timer;
#endif 
}
hdrsrchstate_struct_type;


LOCAL hdrsrchstate_struct_type  hdrsrchstate;
  /* State Machine Data Structure */


/* ------------------------- Module Global Data ------------------------- */

hdrsrch_state_enum_type    hdrsrch_state_id;
  /* Current State of the HDR Searcher */

typedef struct
{
  uint32 rtc_int_status;
  uint32 rtc_int_enable;
  uint32 modem_micro_irq_mux_en;
  uint32 modem_micro_irq_mux_status;
}
hdrsrchstate_irq_registers_type;

hdrsrchstate_irq_registers_type hdrsrchstate_irq_registers;
  /* Dump of HW IRQ registers */

/* Declare HDR dal variables for chip information */
DalDeviceHandle *hdr_dal_chip_info;
DalChipInfoFamilyType hdr_dal_chip_family;
DalChipInfoVersionType hdr_dal_chip_version;

/* Debug: for tramp interrupt trace */
struct 
{
  boolean interrupt_cdma_mod_tx_frame_is_set;
  boolean interrupt_cdma_mod_tx_frame_is_pending;
  boolean interrupt_cdma_mod_tx_frame_hdr_is_set;
  boolean interrupt_cdma_mod_tx_frame_hdr_is_pending;

} hdrsrch_interrupt_trace;

/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_INIT

DESCRIPTION
  Generic initialization processing

DEPENDENCIES
  None

PARAMETERS
  last state

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_init
(
  hdrsrch_state_enum_type last_state
    /* State this state was entered from */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2( MSG_LEGACY_LOW, "Entering state %x from state %x",
                  hdrsrch_state_id,
                  last_state);
}


/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_DONE

DESCRIPTION
  Generic cleanup processing

DEPENDENCIES
  None

PARAMETERS
  last state

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_done
(
  hdrsrch_state_enum_type next_state
    /* State this state will transition to. */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2( MSG_LEGACY_LOW, "Exiting state %x to state %x",
                  hdrsrch_state_id,
                  next_state);
}


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSTATE_FORCE_UNLOCK_RF

DESCRIPTION
  A "hint" from "above" that some other protocol REALLY NEEDS the RF
  resources, and we should give them up.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  System lost

=========================================================================*/

void hdrsrchstate_force_unlock_rf( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  REX_DISABLE_PREMPTION();

#if !defined ( FEATURE_FULL_TIME_SHDR_3 )
  {
    /* Does HDR have any RF lock? If so, lets unlock the RF. */
    if ( ( hdrsrchrf_has_lock( HDRSRCHRF_PRI_CHAIN ) == TRUE ) ||
         ( hdrsrchrf_has_lock( HDRSRCHRF_DIV_CHAIN ) == TRUE ) )
    {
      hdrsrchutil_system_lost( HDRSRCH_SYSLOST_ADVISE_UNLOCK_RF );
    }
  }
#else /* FEATURE_FULL_TIME_SHDR_3 */
  hdrsrchutil_system_lost( HDRSRCH_SYSLOST_ADVISE_UNLOCK_RF );
    /* Always declare system loss. */
#endif

  REX_ENABLE_PREMPTION();

} /* hdrsrchstate_force_unlock_rf() */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_CMD

DESCRIPTION
  Generic command processing

DEPENDENCIES
  None

PARAMETERS
  cmd         - command to be processed.

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

=========================================================================*/

errno_enum_type hdrsrch_default_cmd
(
  const hdrsrch_cmd_type  * cmd
    /* Command to be processed */
)
{

  errno_enum_type           status = E_SUCCESS;
    /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch ( cmd->name & HDRSRCHCMD_CMD_MASK )
  {
    case HDRSRCH_DEACTIVATE_CMD:
      /* Transition to INACTIVE state is usually allowed from all states */
      HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "Deactivate from %x", hdrsrch_state_id );

      hdrsrchst_init_inactive_state();
        /* Initialize start state */

      status = E_SUCCESS;
      break;


    case HDRSRCH_ADVISE_UNLOCK_RF_CMD:

      /* Release the RF, if held */

      hdrsrchstate_force_unlock_rf();
        /* Some other protocol NEEDS the RF (voice origination?).
           Release it. */

      status = E_SUCCESS;
      break;


    case HDRSRCH_TC_ACK_RECEIVED_CMD:

      /* This command is intended to be processed in traffic state. But under
         certain condition, it can also be received by other states. When it 
         is received by a non-traffic state, it is basically discarded 
         silently, because we can not do much ( we are not in traffic, and 
         sometimes we even don't have RF ). The only exception will be 
         traffic suspended state where we mark the status as "E_AGAIN" with 
         the hope that we can go back to traffic pretty soon and have this
         command retried   */

      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                      "Completed TC_ACK_RECEIVED cmd in state %x",
                      hdrsrch_state_id  );

      status = E_SUCCESS;
      break;

#ifdef FEATURE_HDR_BCMCS
    case HDRSRCH_UPDATE_BOM_PERIOD_CMD:

      hdrsrchsleep_set_bcmcs_page_cycle( cmd->params.bom.period );
        /* Update the BOM period  */

      status = E_SUCCESS;
      break;
#endif /* FEATURE_HDR_BCMCS */

    case HDRSRCH_TC_MODE_CMD:

      status = E_SUCCESS;
        /* Do nothing by default. */

      break;

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
    case  HDRSRCH_LTE_PILOT_MEAS_REQ:
      hdrsrchmsg_send_lte_pilot_meas_rsp( 
        PILOT_MEAS_RECV_IN_WRONG_STATE,
        cmd->params.lte_pilot_meas_req.lte_rfm_param.target.channel );
      break;

    case HDRSRCH_LTE_PILOT_MEAS_ABORT_REQ :
      hdrsrchmsg_send_lte_pilot_meas_abort_rsp();
      break;
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

#ifdef FEATURE_HDR_TO_LTE
    case HDRSRCH_LTE_NSET_UPDATE_CMD:
      hdrsrchltemeas_prep_to_update_lte_neighbor_list( &cmd->params.lte_nset_update );
        /* Update LTE neighbor list - used in DO->LTE reselection */
      break;

    case HDRSRCH_LTE_TRESELECT_EXP_CMD:
      hdrsrchltemeas_treselect_timer_exp();
        /* LTE Treselect timer just expired, now process it in DO task context */
      break;

    case HDRSRCH_ABORT_LTE_RESELECT_CMD:
      hdrsrchltemeas_abort_lte_reselection();
        /* LTE reselection has been aborted by upper layer */
      break;

#endif /* FEATURE_HDR_TO_LTE */

    case HDRSRCH_SET_CC_START_CMD:

      hdrsrchsleep_set_cc_start( cmd->params.cc_start.cc_start );
        /* Set CC start */
      
      if( hdrmdsp_check_dsp_clk() )
      {
        hdrsrchsleep_send_msg_update_cc_start( );
          /* Send CC config message to update CC start */
      }
      else
      {
        HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                      "FW is in standby and scc_cfg_cmd is not sent" );
      }

      status = E_SUCCESS;

      break;

    case HDRSRCH_SET_SCC_OFFSET_CMD:

      hdrsrchsleep_set_scc_offset( cmd->params.scc_offset.scc_offset );
        /* Set SCC offset and send SCC control message */
      
      if( hdrmdsp_check_dsp_clk() )
      {
        hdrsrchsleep_send_msg_update_scc_offset( );
          /* Send the message to update scc offset in FW */
      }
      else
      {
        HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                      "FW is in standby and scc_cfg_cmd is not sent" );
      }

      status = E_SUCCESS;

      break;


    case HDRSRCH_SET_SCC_INFO_CMD:

      hdrsrchset_set_scc_info( cmd->params.scc_info.scc_offset,
                               cmd->params.scc_info.scc_rate,
                               cmd->params.scc_info.link_id );
        /* Save the SCC info */

      hdrsrchsleep_set_scc_info( cmd->params.scc_info.scc_offset,
                                 cmd->params.scc_info.scc_rate );
        /* Set SCC offset and send SCC control message */
      
      if( hdrmdsp_check_dsp_clk() )
      {
        hdrsrchsleep_send_msg_update_scc_offset( );
          /* Send the message to update scc offset in FW */
      }
      else
      {
        HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                      "FW is in standby and scc_cfg_cmd is not sent" );
      }

      status = E_SUCCESS;

      break;

    case HDRSRCH_AT_SET_DRC_CMD:
      hdrsrchtc_set_drc_param( cmd->params.at_set_drc.demod_idx,
                               cmd->params.at_set_drc.drc_param );
        /* save the DRC param */
#ifdef FEATURE_HDR_RUMI
      hdrsrchtc_drc_param_ctrl(HDRMDSP_DEMOD_CARRIER_0);
        /* moved this call from hdrhitmsg.c */
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                    "RUMI: send DRC_CFG_CMD to fw drc=%d", cmd->params.at_set_drc.drc_param );
#endif
      status = E_SUCCESS;

      break;

    case HDRSRCH_CHANGE_SLEEP_TIMELINE_CMD:

      hdrsrchsleep_set_sleep_timeline_default( cmd->params.optimized_sleep_timeline.optimized );
      break;

#ifdef FEATURE_HDR_REVC
    case HDRSRCH_ENABLE_NLB_CMD:
      HDR_MSG_SRCH_2( MSG_LEGACY_MED, "enable_nlb %d st %x",
                      cmd->params.enable_nlb, 
                      hdrsrch_state_id );
      hdrsrchutil_enable_nlb( cmd->params.enable_nlb );
      status = E_SUCCESS;
      break;
#endif

    case HDRSRCH_SYSTEM_LOST_CMD:
      hdrsrchutil_system_lost( cmd->params.system_lost_reason.reason );
      break;

    default:

      /* Unhandled command */

      HDR_MSG_SRCH_2( MSG_LEGACY_ERROR, "Unexpected command %d, in state %x",
                      cmd->name, hdrsrch_state_id );

      status = E_NOT_SUPPORTED;
        /* Unknown command! */

  } /* switch (cmd->name) */

#ifndef FEATURE_HDR_BOLT_MODEM
if ( cmd->name == MM_MMOC_OFFLINE_IND)
{
      HDR_MSG_SRCH( MSG_LEGACY_MED, "Rxed Offline Cmd");
      hdrsrchst_init_inactive_state( );
      /* Transition back to start state */

      status = E_SUCCESS;
}

#endif /* FEATURE_HDR_BOLT_MODEM */

  return status;

} /* hdrsrch_default_cmd( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_DUMP

DESCRIPTION
  Generic Search Dump Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_dump( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Unexpected DUMP - ignored.  State=%x",
                  hdrsrch_state_id );

} /* hdrsrch_default_dump( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_ROLL

DESCRIPTION
  Generic PN ROLL Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_roll( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil_handle_pn_roll();
    /* do PN roll processing */


} /* hdrsrch_default_dump( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_WAKE

DESCRIPTION
  Generic Wake Up Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_wake( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Unexpected WAKE - ignored.  State=%x",
                  hdrsrch_state_id );

} /* hdrsrch_default_dump( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_TUNE

DESCRIPTION
  Generic RF Tune Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_tune( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Unexpected TUNE - ignored.  State=%x",
                  hdrsrch_state_id );

} /* hdrsrch_default_dump( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_SCHED

DESCRIPTION
  Generic Search Schedule Timer Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_sched( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Unexpected SCHED - ignored.  State=%x",
                  hdrsrch_state_id );

} /* hdrsrch_default_sched( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_LOST_DUMP

DESCRIPTION
  Generic Lost Dump Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_lost_dump( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrmdsp_check_dsp_clk() )
  {
    if ( hdrsrchutil_check_mdsp_halt() )
    {
      hdrsrchutil_handle_mdsp_halt();
        /* handle the DSP halt emergency */
    }
    else
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "Lost Searcher Dump. State=%x",
                      hdrsrch_state_id );

      #ifdef HDRSRCH_SYS_LOST_ON_LOST_DUMP
      hdrsrchutil_handle_lost_dump();
        /* handle the scenario where the search dump is lost */
      #endif /* HDRSRCH_SYS_LOST_ON_LOST_DUMP */
    }
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "LOST DUMP - ignored.  State=%x",
                    hdrsrch_state_id );
  }

} /* hdrsrch_default_lost_dump( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_TIMEOUT

DESCRIPTION
  Generic Timeout Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_timeout( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "TIMEOUT - ignored.  State=%x",
                  hdrsrch_state_id );

} /* hdrsrch_default_timeout( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_GP_TIMER0

DESCRIPTION
  Generic Timer Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_gp_timer0( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Unexpected GP TMR #0 ignored. State=%x",
                  hdrsrch_state_id );

} /* hdrsrch_default_gp_timer0( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_GP_TIMER1

DESCRIPTION
  Generic Timer Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_gp_timer1( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Unexpected GP TMR #1 ignored. State=%x",
                  hdrsrch_state_id );

} /* hdrsrch_default_gp_timer1( ) */



/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_GP_SIG0

DESCRIPTION
  General Purpose Signal #0 Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_gp_sig0( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Unexpected GP SIG #0 ignored. State=%x",
                  hdrsrch_state_id );

} /* hdrsrch_default_gp_sig0( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_GP_SIG1

DESCRIPTION
  General Purpose Signal #1 Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_gp_sig1( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Unexpected GP SIG #1 ignored. State=%x",
                  hdrsrch_state_id );

} /* hdrsrch_default_gp_sig1( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_TIME_VALID

DESCRIPTION
  Generic Time Valid Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_time_valid( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                  "Unexpected TIME VALID sig - ignored. State=%x",
                  hdrsrch_state_id );

} /* hdrsrch_default_time_valid( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_RXCHIPX16

DESCRIPTION
  Generic RXCHIPX8 Processing

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_rxchipx16( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                  "Unexpected RXCHIPX16 sig - ignored. State=%x",
                  hdrsrch_state_id );

} /* hdrsrch_default_rxchipx16( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_DIV_CHAIN_EVENT

DESCRIPTION
  Generic processing of a diversity chain related event.  Note that diversity
  chain does not imply either chain0 or chain1.  It is possible it is
  either although HDR only supports chain1 as the diversity reciever due to 
  the RF driver implementation.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_div_chain_event( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                  "Unexpected diversity chain sig - ignored. State=%x",
                  hdrsrch_state_id );

} /* hdrsrch_default_div_chain_event( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DEFAULT_PRI_CHAIN_EVENT

DESCRIPTION
  Generic processing of a primary chain related event.  Note that primary
  chain does not imply either chain0 or chain1.  It is possible it is
  either although HDR only supports chain0 as the primary due to the 
  RF driver implementation.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_default_pri_chain_event( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                  "Unexpected primary chain sig - ignored. State=%x",
                  hdrsrch_state_id );

} /* hdrsrch_default_pri_chain_event( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSTATE_PROCESS_WDOG

DESCRIPTION
  This function pets the heart-beat watchdog.
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

=========================================================================*/
void hdrsrch_process_wdog( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_SIG_CLR( HDRSRCH_DOG_HB_SIG );
  dog_hb_report( DOG_HDRSRCH_RPT ); 

} /* hdrsrch_process_wdog */

/*=========================================================================

FUNCTION HDRSRCHSTATE_DUMP_HW_STATE

DESCRIPTION
  Dumps the state of HW registers that may help in debugging stalled
  searcher issues.
 
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchstate_dump_hw_state( void )
{
/* TBD for Dime */
#ifndef FEATURE_HDR_DIME_MODEM
  /* Save the interrupt controller state if the searcher is in a time slew
     state. */
  if ( ( hdrsrch_state_id == HDRSRCH_SYNC_STATE ) || 
       ( hdrsrch_state_id == HDRSRCH_TIME_SYNC_STATE) )
  {
    hdrsrchstate_irq_registers.rtc_int_enable = HWIO_SHDW( RTC_INT_ENABLE ) ;
    hdrsrchstate_irq_registers.rtc_int_status = HWIO_IN( RTC_INT_STATUS) ;

#ifdef T_MSM8960
    hdrsrchstate_irq_registers.modem_micro_irq_mux_en = HWIO_IN( MODEM_MICRO_IRQ_MUX_EN1 );
    hdrsrchstate_irq_registers.modem_micro_irq_mux_status = HWIO_IN( MODEM_MICRO_IRQ_MUX_STATUS1);
#else
    hdrsrchstate_irq_registers.modem_micro_irq_mux_en = HWIO_IN( MODEM_MICRO_IRQ_MUX_EN );
    hdrsrchstate_irq_registers.modem_micro_irq_mux_status = HWIO_IN( MODEM_MICRO_IRQ_MUX_STATUS);
#endif

    /* Print out the interrupt status registers */
    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "RTC_INT_ENABLE = %d, RTC_INT_STATUS = %d",
                                           hdrsrchstate_irq_registers.rtc_int_enable,
                                           hdrsrchstate_irq_registers.rtc_int_status );

    HDR_MSG_SRCH_2(MSG_LEGACY_HIGH, "MODEM_MICRO_IRQ_MUX_EN = %d, MODEM_MICRO_IRQ_MUX_STATUS = %d",
                                           hdrsrchstate_irq_registers.modem_micro_irq_mux_en,
                                           hdrsrchstate_irq_registers.modem_micro_irq_mux_status );
  }
#endif /* FEATURE_HDR_DIME_MODEM */
} /* hdrsrchstate_dump_hw_state */ 

/*=========================================================================

FUNCTION HDRSRCHSTATE_PROCESS_WDOG_TIMER

DESCRIPTION
  Handle the Watch Dog signal. This function is called every 2secs on
  expiration of watchdog timer report. If the intent is to simply pet the 
  watchdog at times other that the regular 2secs watchdog timer interval,
  the function dog_report() should be used.
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Resets the DOG timer.

=========================================================================*/

LOCAL void hdrsrchstate_process_wdog_timer( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*
  ** Ensure the searcher state machine is running
  */

  if ( hdrsrchstate.dog.dump_occurred )
  {
    hdrsrchstate.dog.no_dump_cnt = 0;
      /* We've had a search dump, so reset no-dump count */

    hdrsrchstate.dog.dump_occurred = FALSE;
      /* Reset reports for next watchdog report time */
  }
  else
  {
    /* No dumps have occurred since last dog report */

    /* Does HDR have any RF lock? */
    if ( hdrsrchrf_has_lock( HDRSRCHRF_PRI_CHAIN ) == TRUE )
    {
      hdrsrchstate.dog.no_dump_cnt += HDRSRCH_DOG_RPT_TIME_MS;
        /* We own the RF!  Advance the no-dump count */

      if ( hdrsrchstate.dog.no_dump_cnt > 0 )
      {
        HDR_MSG_SRCH_2( MSG_LEGACY_ERROR, "No dump for %d ms. St=%x",
                        hdrsrchstate.dog.no_dump_cnt,
                        hdrsrch_state_id );
      }

      if ( hdrsrchstate.dog.no_dump_cnt >= 10000 )
      {
        hdrsrchstate_dump_hw_state();

        ERR_FATAL("Searcher SM Stalled (%d ms)",hdrsrchstate.dog.no_dump_cnt,0,0);
      }
    }
  }


  /*
  ** Ensure the command processor is running
  */

  if ( hdrsrchstate.dog.cmd_executed )
  {
    hdrsrchstate.dog.no_cmd_cnt = 0;
      /* We've executed commands, so reset no-cmd count */

    hdrsrchstate.dog.cmd_executed = FALSE;
      /* Reset reports for next watchdog report time */
  }
  else if ( hdrsrch.cmd != NULL )
  {
    /* No commands have been executed since last dog report,
       and a command is pending. */

    hdrsrchstate.dog.no_cmd_cnt += HDRSRCH_DOG_RPT_TIME_MS;
      /* Advance the no-cmd count */

    if ( hdrsrchstate.dog.no_cmd_cnt > 0 )
    {
      /* If the command processing halts, print the information */

      HDR_MSG_SRCH_5( MSG_LEGACY_ERROR, 
                      "No cmd proc for %d ms, St=%x, Name=%d, Svc=%d, Tune=%x",
                      hdrsrchstate.dog.no_cmd_cnt,
                      hdrsrch_state_id,
                      hdrsrch.cmd->name,
                      hdrsrchafc_service_request_pending(),
                      hdrsrchstate.wait_mask & HDRSRCH_TUNE_SIG );
    }

    if ( hdrsrchstate.dog.no_cmd_cnt >= 80000 )
    {
      hdrsrchstate_dump_hw_state();

      ERR_FATAL("Command SM Stalled (%d ms)", hdrsrchstate.dog.no_cmd_cnt, 0, 0);
    }
  }


  /*
  ** Pet the dog; we are happy.
  */

  hdrsrch_process_wdog();

  (void) rex_set_timer( &hdrsrch.dog_timer, HDRSRCH_DOG_RPT_TIME_MS );


} /* hdrsrchstate_process_wdog_timer( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_RESET_NO_DUMP_CNT

DESCRIPTION
  This function resets no_dump_cnt in dog structure in hdrsrchstate

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

void hdrsrch_reset_no_dump_cnt(  )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchstate.dog.no_dump_cnt = 0;
    /* reset no_dump_cnt to 0     */

} /* hdrsrch_reset_no_dump_cnt  */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_CHANGE_STATE

DESCRIPTION
  TBD

DEPENDENCIES
  TBD

PARAMETERS
  TBD

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_change_state
(
  const hdrsrch_state_struct_type *new_state
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchstate.next_state = new_state;

  if ( hdrsrchstate.next_state != NULL )
  {
    hdrsrchstate.next_state_id = hdrsrchstate.next_state->id;
  }
  else
  {
    hdrsrchstate.next_state_id = HDRSRCH_NO_STATE;
  }

  /* If an interrupt service routine is responsible for calling this
     function, the task may be sitting at a rex_wait( ) waiting for any
     signal in the wait mask.  We need the state machine to respond to
     this ISR based state transition, so we must tickle the task to
     force rex_wait to schedule the task again. */

  HDRSRCH_SIG_SET( HDRSRCH_TICKLE_SIG );
    /* Tickle the task.  */

} /* hdrsrch_change_state( ) */


/* EJECT */
/*=========================================================================

FUNCTION    HDRSRCH_ADD_TO_WAIT_MASK

DESCRIPTION
  Add signals to the set the search task will wait on.

DEPENDENCIES
  TBD

INPUTS
  sigs    - New signals to wait on (add to wait mask)

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_add_to_wait_mask
(
  rex_sigs_type   sigs
    /* Signals to add to the wait mask */
)
{
  rex_sigs_type                       wait_mask = hdrsrchstate.wait_mask;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
  hdrsrchstate.wait_mask |= sigs;
    /* Include specified signals */

  HDR_MSG_SRCH_3( MSG_LEGACY_LOW, 
                  "add waitmask %x+%x=%x", 
                  wait_mask, sigs, hdrsrchstate.wait_mask );
} /* hdrsrch_add_to_wait_mask( ) */


/* EJECT */
/*=========================================================================

FUNCTION    HDRSRCH_REMOVE_FROM_WAIT_MASK

DESCRIPTION
  Remove signals from the set the search task will wait on.

DEPENDENCIES
  TBD

INPUTS
  sigs    - Signals the search task will no longer wait on or process.

RETURN VALUE
  None

SIDE EFFECTS
  TBD

=========================================================================*/

void hdrsrch_remove_from_wait_mask
(
  rex_sigs_type   sigs
    /* Signals to remove from the wait mask */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  rex_sigs_type                       wait_mask = hdrsrchstate.wait_mask;

  hdrsrchstate.wait_mask &= ~sigs;
    /* Exclude specified signals */

  HDR_MSG_SRCH_3( MSG_LEGACY_LOW, 
                  "remove waitmask %x-%x=%x", 
                  wait_mask, sigs, hdrsrchstate.wait_mask );


}  /* hdrsrch_remove_from_wait_mask( ) */


/* EJECT */
/*=========================================================================

FUNCTION    HDRSRCH_KILL_SIGNAL

DESCRIPTION
  Clears the given signal if set, and removes the it from the wait mask.

DEPENDENCIES
  None

INPUTS
  Timer - Address of timer to kill

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

void hdrsrch_kill_signal
(
  rex_sigs_type   sigs
    /* Signals to be cleared and removed from the wait mask */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void) rex_clr_sigs( HDRSRCH_TCB, sigs );
    /* Clear the requested signals ... */

  hdrsrch_remove_from_wait_mask( sigs );
    /* ... and remove signals from the wait mask */

} /* hdrsrch_kill_signal( ) */


/*=========================================================================

FUNCTION    HDRSRCH_STOP_TIMER

DESCRIPTION
  Stops the given timer, and clears the timer's signal if set.

DEPENDENCIES
  None

INPUTS
  timer - Timer to be stopped

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

void hdrsrch_stop_timer
(
  rex_timer_type   *timer
    /* Timer to be stopped */
)
{
  timer_attrib_struct_type timer_attrib_struct_data;
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void) rex_clr_timer( timer );
    /* Clear the timer ... */

  timer_get_params(timer,&timer_attrib_struct_data);

  (void) rex_clr_sigs( HDRSRCH_TCB, (rex_sigs_type)timer_attrib_struct_data.sigs );
    /* ... and clear the timer's signal */

} /* hdrsrch_stop_timer( ) */


/*=========================================================================

FUNCTION    HDRSRCH_KILL_TIMER

DESCRIPTION
  Stops the given timer, clears the timer's signal if set, and removes the
  signal from the wait mask

DEPENDENCIES
  None

INPUTS
  timer - Timer to be killed

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

void hdrsrch_kill_timer
(
  rex_timer_type   *timer
    /* Timer to be killed */
)
{
  timer_attrib_struct_type timer_attrib_struct_data;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void) rex_clr_timer( timer );
    /* Clear the timer ... */

  timer_get_params(timer,&timer_attrib_struct_data);
  
  hdrsrch_kill_signal( (rex_sigs_type)timer_attrib_struct_data.sigs  );
    /* ... and kill the timer's signal */

} /* hdrsrch_kill_signal( ) */


/* EJECT */
/*=========================================================================

FUNCTION    HDRSRCHSTATE_OFFLINE

DESCRIPTION
  Handles signal to go Offline

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Unconditional transistion to start state

=========================================================================*/

LOCAL void hdrsrchstate_offline( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "OFFLINE received in state %x",
                  hdrsrch_state_id );

  hdrsrchst_init_inactive_state( );
    /* Transition back to start state */

} /* hdrsrchstate_offline( ) */


/*=========================================================================

FUNCTION    HDRSRCHSTATE_STOP

DESCRIPTION
  Handles signal to STOP

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Unconditional transistion to start state

=========================================================================*/

LOCAL void hdrsrchstate_stop( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "STOP received in state %x",
                  hdrsrch_state_id );

  hdrsrchst_init_inactive_state( );
    /* Transition back to start state */

  /* Clean up the resources
  */
  (void) rex_clr_timer( &hdrsrch.schedule_timer );
  (void) rex_clr_timer( &hdrsrch.lostdump_timer );
  (void) rex_clr_timer( &hdrsrch.timeout_timer );
  (void) rex_clr_timer( &hdrsrch.gp_timer0 );
  (void) rex_clr_timer( &hdrsrch.gp_timer1 );

  (void) rex_clr_timer( &hdrsrch.gp_timer2 );

  /* Clean up the MSGR resources
  */
  hdrsrchmsg_stop();

  /* TBD
  ** Does MSGR and Shared memory interfaces need to be supported from the
  ** same code base?
  */

} /* hdrsrchstate_stop( ) */


/* EJECT */
/*=========================================================================

FUNCTION    HDRSRCH_PROCESS_CMD

DESCRIPTION
  Dispatches the command.  If the command executes completely, the command
  buffer is returned to the free queue.  Otherwise, command processing
  is suspended until HDRSRCH_COMPLETE_CMD( ) is called to return the
  command to the free queue.

DEPENDENCIES
  TBD

INPUTS
  cmd - command to be processed.

RETURN VALUE
  None

SIDE EFFECTS
  Command processing may be suspended.

=========================================================================*/

LOCAL void hdrsrch_process_cmd
(
  hdrsrch_cmd_type      * cmd
    /* Command to be processed */
)
{
   sys_modem_as_id_e_type variant;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch.cmd = cmd;
    /* Remember which command is being executed, in case it suspends */

  /* Store the  variant ID info of the command*/
   variant = MSGR_VARIANT_VAL(cmd->name);
   MSGR_CLEAR_VARIANT(cmd->name);
	
  switch(cmd->name & HDRSRCHCMD_CMD_MASK)
  {
    case HDRSRCH_TRAN_SYS_TIME_REQ:   
    case HDRSRCH_LTE_PILOT_MEAS_REQ:      
    case HDRSRCH_LTE_PILOT_MEAS_ABORT_REQ:
	     if(variant == hdrmc_get_hdr_sub_id())
	     {
               hdrsrch.cmd->status = hdrsrchstate.state->cmd( cmd );
               /* Process the command */
	     }
	     else
	     {
	         HDR_MSG_PROT_2(MSG_LEGACY_ERROR,"Ignoring the request received from other sub. L sub:%d DO sub:%d",
                                variant,hdrmc_get_hdr_sub_id());
		  hdrsrch.cmd->status = E_NOT_SUPPORTED;
	     }
			
	     break;
	default:
	    hdrsrch.cmd->status = hdrsrchstate.state->cmd( cmd );
		/* Process the command */	
  }


  switch ( hdrsrch.cmd->status )
  {
    case E_SUCCESS:
    case E_FAILURE:
    case E_NOT_SUPPORTED:
    {
      hdrsrch_complete_cmd( );
        /* This command has been completely processed */

    } break;

    case E_IN_PROGRESS:
    case E_WOULD_BLOCK:
    case E_AGAIN:
    {
      /* This command did not complete immediately.
         Do not return the command buffer to the free queue. */

      hdrsrch_remove_from_wait_mask( HDRSRCH_CMD_Q_SIG );
        /* Suspend future command processing until the command
           is completed. */
    } break;

    default:
      ERR_FATAL("Unknown status %d from cmd %d",
                hdrsrch.cmd->status, cmd->name, 0);
  } /* switch */

} /* hdrsrch_process_cmd( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_COMPLETE_CMD

DESCRIPTION
  Indicates that the pending command is now complete, and the buffer
  may be returned to the free queue.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Command processing is re-enabled.

=========================================================================*/

void hdrsrch_complete_cmd( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrch.cmd )
  {
    if ( hdrsrch.cmd->status == E_IN_PROGRESS ||
         hdrsrch.cmd->status == E_WOULD_BLOCK )
    {
      hdrsrch.cmd->status = E_SUCCESS;
        /* Command is now complete */
    }

    hdrsrchcmd_free( hdrsrch.cmd );
      /* Return command structure to the free pool */

    hdrsrch.cmd = NULL;
      /* No command is pending anymore */

    hdrsrch_add_to_wait_mask( HDRSRCH_CMD_Q_SIG );
      /* Re-enable command processing. */

    /* If an interrupt service routine is responsible for calling this
       function, the task may be sitting at a rex_wait( ) waiting for any
       signal in the wait mask.  The signal we have just set may already
       be asserted for the task, but since rex_wait was not waiting for
       it at the time, the task will not wake up.  Instead, we will tickle
       the task, to force the state machine to wait on the correct wait
       mask. */

    HDRSRCH_SIG_SET( HDRSRCH_TICKLE_SIG );
      /* Tickle the task. */

    hdrsrchstate.dog.cmd_executed = TRUE;
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "No pending command!" );
  }

} /* hdrsrch_complete_cmd( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_RETRY_CMD

DESCRIPTION
  Attempt to re-execute a command which returned E_AGAIN.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Flag that indicates whether the command is executed or not after retry.

SIDE EFFECTS
  Command processing may be re-enabled.

=========================================================================*/

hdrsrch_cmd_executed_enum_type hdrsrch_retry_cmd( void )
{
  hdrsrch_cmd_executed_enum_type   status = HDRSRCH_CMD_NOT_EXECUTED;
    /* flag that indicates whether command is executed or not */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrch.cmd != NULL )
  {
    if ( hdrsrch.cmd->status == E_AGAIN )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Retry cmd=%d",
                      hdrsrch.cmd->name );

      hdrsrch.cmd->status = hdrsrchstate.state->cmd( hdrsrch.cmd );
        /* Process the command */

      switch ( hdrsrch.cmd->status )
      {
        case E_SUCCESS:
        case E_FAILURE:
        case E_NOT_SUPPORTED:
        {
          /* This command has now been completely processed */

          hdrsrch_complete_cmd();

          status = HDRSRCH_CMD_EXECUTED;
            /* the pending command has been completed */

          break;
        }
        default:

          break;
      } /* switch */
    }
    else
    {
      HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Can't retry cmd=%d, stat=%d",
                      hdrsrch.cmd->name, hdrsrch.cmd->status );
    }
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_LOW, "No command to retry" );
  }

  return status;

} /* hdrsrch_retry_cmd( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_ABORT_COMMAND

DESCRIPTION
  This function aborts suspended/in-progress commands 

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  In-progress commands are aborted

=========================================================================*/

static void hdrsrch_abort_command( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Abort suspended/in-progress commands */

  if ( ( hdrsrch.cmd != NULL ) &&
       ( hdrsrch.cmd->name != HDRSRCH_DEACTIVATE_CMD ) )
  {
    /* If TCA and Channel Change cmds, then we abort only if status is
       E_AGAIN or E_WOULD_BLOCK */
    if ( ( hdrsrch.cmd->name == HDRSRCH_TC_ASSIGNMENT_CMD ) || 
         ( hdrsrch.cmd->name == HDRSRCH_CHANNEL_CHANGE_CMD ) )
    { 
      if ( ( hdrsrch.cmd->status == E_AGAIN ) ||
           ( hdrsrch.cmd->status == E_WOULD_BLOCK ) )
      { 
        HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "Aborted %d command",
                        hdrsrch.cmd->name );

        hdrsrch.cmd->status = E_ABORTED;
        hdrsrch_complete_cmd( );
         /* Return command to free queue */
      }
      else
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "Did not abort %d command",
                        hdrsrch.cmd->name );
      }
    }
    /* If not TCA and Channel Change cmds, then we abort if status is
       E_AGAIN or E_WOULD_BLOCK or E_IN_PROGRESS */
    else
    { 
      if ( hdrsrch.cmd->status == E_AGAIN ||
           hdrsrch.cmd->status == E_WOULD_BLOCK ||
           hdrsrch.cmd->status == E_IN_PROGRESS )
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "Aborted %d command",
                        hdrsrch.cmd->name );

        hdrsrch.cmd->status = E_ABORTED;
        hdrsrch_complete_cmd( );
         /* Return command to free queue */
      }
      else
      {
        ERR("Can't abort cmd=%d status=%d",
            hdrsrch.cmd->name,
            hdrsrch.cmd->status,
            0);
      }
    }
  }
} /* hdrsrch_abort_command */

/* EJECT */

/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_ON_DUMP

DESCRIPTION
  Queue a function for execution after the next search dump.

  The function is only called once.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Function is executed in future.

=========================================================================*/

void hdrsrch_on_dump
(
  hdrsrch_on_event_func   func
    /* Function to call after the next search dump. */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchstate.on_dump == NULL )
  {
    hdrsrchstate.on_dump = func;
      /* Store the function for execution after next search dump */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "On_dump() still set." );
      /* Would lose older, unexecuted on_dump procedure! */
  }

} /* hdrsrch_on_dump( ) */

/*=========================================================================

FUNCTION HDRSRCH_CLEAR_ON_SEARCHER_IDLE_PROC

DESCRIPTION
  The function clears on_searcher_idle processing

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Resets hdrsrchstate.on_dump and hdrsrchstate.on_searcher_idle to NULL

=========================================================================*/

void hdrsrch_clear_on_searcher_idle_proc ( void )
{

  uint8 i;
    /* Loop index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchstate.on_dump          = NULL;

  for ( i=0; i < HDRSRCH_MAX_ON_SEARCH_IDLE; i++)
  {
    /* Loop through all priorities */

    hdrsrchstate.on_searcher_idle[i] = NULL;
  }
    /* start with no functions pending */

}/* hdrsrch_clear_on_searcher_idle_proc */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_ON_SEARCHER_IDLE_REGED

DESCRIPTION
  The function check if any on_searcher_idle callbacks has been registered.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

LOCAL boolean hdrsrch_on_searcher_idle_reged( void )
{

  uint8 i; 
    /* loop index */

  boolean reged = FALSE;
    /* return value */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( i=0; i<HDRSRCH_MAX_ON_SEARCH_IDLE; i++)
  {
    /* loop through all the priority levels */

    if ( hdrsrchstate.on_searcher_idle[i] != NULL )
    {
      /* if a call back has been registered */

      reged = TRUE;
      break;
    }
  }

  return reged;

} /* hdrsrch_on_searcher_idle_reged( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_PRI_ON_SEARCHER_IDLE

DESCRIPTION
  Queue a command at its priority level for execution when the searcher is 
  idle, or immediately if searcher is presently idle.

  The function is only called once.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Command is executed now or in future.

=========================================================================*/

LOCAL void hdrsrch_pri_on_searcher_idle
(
  hdrsrch_on_event_func   func,
    /* Function to call when searcher is idle. */

  hdrsrch_on_search_idle_pri_enum_type pri
    /* Priority of the callback. */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchstate.on_searcher_idle[pri] == NULL )
  {
    hdrsrchstate.on_searcher_idle[pri] = func;
      /* Store the function for execution when the searcher is idle */
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "On_searcher_idle[%d] still set.", pri);
      /* Would lose older, unexecuted on_searcher_idle procedure! */
  }

} /* hdrsrch_pri_on_searcher_idle( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_ON_SEARCHER_IDLE

DESCRIPTION
  Queue a command at default priority for execution when the searcher is 
  idle, or immediately if searcher is presently idle.

  The function is only called once.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Command is executed now or in future.

=========================================================================*/

void hdrsrch_on_searcher_idle
(
  hdrsrch_on_event_func   func
    /* Function to call when searcher is idle. */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_pri_on_searcher_idle( func, HDRSRCH_DEFAULT_ON_SEARCH_IDLE );
    /* Register the function at the default(top) level priority */

  HDRSRCH_SIG_SET( HDRSRCH_TICKLE_SIG );
    /* Tickle the task so that on_searcher_idle function can be called.  */

} /* hdrsrch_on_searcher_idle( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_TUNE_AWAY_ON_SEARCHER_IDLE

DESCRIPTION
  Queue a command at the second level priority for execution when the searcher 
  is idle, or immediately if searcher is presently idle. Curret the function is
  reserved for the tune away on search idle callbacks.

  The function is only called once.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Command is executed now or in future.

=========================================================================*/

void hdrsrch_tune_away_on_searcher_idle
(
  hdrsrch_on_event_func   func
    /* Function to call when searcher is idle. */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_pri_on_searcher_idle( func, HDRSRCH_TUNEAWY_ON_SEARCH_IDLE );
    /* Register the function at the second level priority */

  HDRSRCH_SIG_SET( HDRSRCH_TICKLE_SIG );
    /* Tickle the task so that on_searcher_idle function can be called.  */

} /* hdrsrch_on_searcher_idle( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_CLEAR_TUNE_AWAY_ON_SEARCHER_IDLE

DESCRIPTION
  Clear the tune away on search idle.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

void hdrsrch_clear_tune_away_on_searcher_idle ( )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchstate.on_searcher_idle[ HDRSRCH_TUNEAWY_ON_SEARCH_IDLE ] != NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "Clear tune away on search idle" );

    hdrsrchstate.on_searcher_idle[ HDRSRCH_TUNEAWY_ON_SEARCH_IDLE ] = NULL;
      /* Clear the tuneaway on search idle call back function */
  }
} /* hdrsrch_clear_tune_away_on_searcher_idle() */


/* EJECT */
/*============================================================================

CALLBACK HDRSRCH_RF_GRANT_CB

DESCRIPTION
  This function is called by HDRSRCHRF to notify HDR search that a transceiver
  that has been granted.
    
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void hdrsrch_rf_grant_cb
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  boolean                               granted
    /* Whether the resources where granted */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Chain ID %d granted", chain_id );
    /* Print out debug message. */

  SYS_ARG_NOT_USED( granted );
    /* to avoid the lint error for the unused varialbe */

  switch ( chain_id )
  {
    case HDRSRCHRF_PRI_CHAIN:

      HDRSRCH_SIG_SET( HDRSRCH_PRI_CHAIN_EVENT_SIG );
        /* Set a signal so that the HDRSRCH task will respond to the 
           primary chain event. */

      break;

    case HDRSRCHRF_DIV_CHAIN:

      HDRSRCH_SIG_SET( HDRSRCH_DIV_CHAIN_EVENT_SIG );
        /* Set a signal so that the HDRSRCH task will respond to the 
           secondary chain event. */

      break;

    default:

      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "TRM grant sent to wrong client");

      break;
  }

} /* hdrsrch_rf_grant_cb( ) */


/* EJECT */
/*============================================================================

CALLBACK HDRSRCH_RF_UNLOCK_CB

DESCRIPTION
  This function is called by HDRSRCHRF to notify HDR search that a transceiver
  is requested to be unlocked.
    
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void hdrsrch_rf_unlock_cb
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  hdrsrchrf_unlock_event_enum_t         event,
    /* The event being sent to the client */

  uint32                                unlock_by_sclk
    /* Sclk timestamp for HDRSRCHRF_UNLOCK_BY */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  SYS_ARG_NOT_USED( unlock_by_sclk );
    /* To avoid the lint error on the unused variable */

  SYS_ARG_NOT_USED( event );
    /* To avoid the lint error on the unused variable */

  switch ( chain_id )
  {
    case HDRSRCHRF_PRI_CHAIN:

      HDRSRCH_SIG_SET( HDRSRCH_PRI_CHAIN_EVENT_SIG );
        /* Set a signal so that the HDRSRCH task will respond to the 
           primary chain event. */

      break;

    case HDRSRCHRF_DIV_CHAIN:

      HDRSRCH_SIG_SET( HDRSRCH_DIV_CHAIN_EVENT_SIG );
        /* Set a signal so that the HDRSRCH task will respond to the 
           secondary chain event. */

      break;

    default:

      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "TRM unlock sent to wrong client");

      break;
  }

} /* hdrsrch_rf_unlock_cb( ) */


/*=========================================================================

FUNCTION HDRSRCH_PROCESS_HIGH_PRI_RSP

DESCRIPTION
  This function processes high priority responses.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
#ifdef FEATURE_HDR_RUMI
extern volatile uint32 hdrtx_tx_start_rsp_rcved;
extern volatile uint32 hdrtx_tx_stop_rsp_rcved;
#endif /* FEATURE_HDR_RUMI */
void hdrsrch_process_high_pri_rsp( void )
{

   hdrsrchmsg_s_type         *msg;
    /* Message */

/*-----------------------------------------------------------------------*/

  while( ( msg = (hdrsrchmsg_s_type *) hdrsrchmsg_get_high_pri_rsp() ) != NULL )
  {
    /* Process the message */
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Rcvd FW rsp 0x%8x", msg->msg.hdr.id );

    hdrsrchmsg_trace( &hdrsrchmsg.msg_rcv, msg->msg.hdr.id );

    switch( msg->msg.hdr.id )
    {
      case HDR_FW_SRCH_FING_ASSIGN_SLAM_RSP:
        hdrsrchfing_process_slam_rsp( 
          msg->msg.hdr_msg.rsp.fing_assign_slam_rsp.slamSuccess );
         /* Process HDRSRCH_MSTR_SLAM_RSP */

      case HDR_FW_SRCH_FING_ASSIGN_RSP:
        hdrsrchmsg_process_rcvd_rsp( HDRSRCH_FING_ASSIGN_RSP );
        break;

      case HDR_FW_SRCH_EQ_CONFIG_RSP:

        hdrsrchmsg_process_rcvd_rsp( HDRSRCH_EQ_CONFIG_RSP );
        break;

      case HDR_FW_ASP_UPDATE_RSP:

        hdrsrchmsg_process_rcvd_rsp( HDRSRCH_ASP_UPDATE_RSP );
        break;

      case HDR_FW_STATE_CFG_RSP:

        hdrmdsp_process_hdr_state_cfg_rsp( &(msg->msg.hdr_msg.rsp.state_cfg) );
        break;

#ifdef FEATURE_HDR_RUMI
      case HDR_FW_RX_START_RSP:
        hdrsrchmsg_process_rcvd_rsp( HDRSRCH_RX_START_RSP );
          /* RUMI: needed to RX_STOP_RSP, or FW will crash at suspend */
        break;

      case HDR_FW_RX_STOP_RSP:
        hdrsrchmsg_process_rcvd_rsp( HDRSRCH_RX_STOP_RSP );
          /* RUMI: needed to RX_STOP_RSP, or FW will crash at suspend */
        break;
#endif /* FEATURE_HDR_RUMI */
#ifdef FEATURE_HDR_RUMI
      case HDR_FW_TX_START_RSP:
        hdrtx_tx_start_rsp_rcved = 1;
        break;
      case HDR_FW_TX_STOP_RSP:
        hdrtx_tx_stop_rsp_rcved = 1;
        /*RUMI: Need to wait for TX Stop response to avoid crash in FW */
        break;
#endif /* FEATURE_HDR_RUMI */
#ifdef FEATURE_HDR_QTA
      case HDR_FW_RXAGC_TRACK_QTA_IND:
        hdrsrchsus_process_rxagc_trk_rsp();
        break;    

      case HDR_FW_QTA_STATUS_QUERY_RSP:
        hdrsrchsus_process_query_rsp(
            msg->msg.hdr_msg.rsp.qta_status_query_rsp.qta_status);
        break;    

#endif /* FEATURE_HDR_QTA */

#ifdef FEATURE_HDR_SELECTIVE_TXD
      case HDR_FW_ASDIV_PRI_CHANGE_RSP:
        hdrsrchrftxd_asdiv_pri_ended( );
        break;
#endif /* FEATURE_HDR_SELECTIVE_TXD */
      default:

        HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, "msg %x ignored", msg->msg.hdr.id);
        break;
    } /* switch */

    hdrsrchmsg_free_high_pri_rsp( msg );
      /* Return the buffer to the free queue */

  } /* while */

} /* hdrsrch_process_high_pri_rsp() */


/*=========================================================================

FUNCTION HDRSRCH_PROCESS_LOW_PRI_RSP_IND

DESCRIPTION
  This function processes low priority responses and indications.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

void hdrsrch_process_low_pri_rsp_ind( void )
{
  hdrsrchmsg_s_type *rsp_ind_msg;
    /* Message */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  while( ( rsp_ind_msg = hdrsrchmsg_get_low_pri_rsp_ind() ) != NULL )
  {

    hdrsrchmsg_trace( &hdrsrchmsg.msg_rcv, rsp_ind_msg->msg.hdr.id );

    hdrsrchstate.state->process_low_pri_rsp_ind( rsp_ind_msg );
      /* Process the response/indication */

    hdrsrchmsg_free_low_pri_rsp_ind( rsp_ind_msg );
      /* Return the buffer to the free queue */

  }
} /* hdrsrch_process_low_pri_rsp_ind() */


/*=========================================================================

FUNCTION    HDRSRCH_PROCESS_SRCH_RSP

DESCRIPTION
  Process the srch responses.

DEPENDENCIES
  TBD

INPUTS
  msg - Message pointer

RETURN VALUE
  None

SIDE EFFECTS
  

=========================================================================*/

void hdrsrch_process_srch_rsp( void )
{
  hdrsrchmsg_s_type         *msg;
    /* Message */

/*-----------------------------------------------------------------------*/

  while( ( msg = hdrsrchmsg_get_srch_rsp() ) != NULL )
  {

    /* Process the message */
    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Rcvd. FW msg %s umid 0x%8x", 
                    hdrsrchmsg_get_msg_name( msg->msg.hdr.id ),
                    msg->msg.hdr.id );

    hdrsrchmsg_trace( &hdrsrchmsg.msg_rcv, msg->msg.hdr.id );

    srch4_hdr_process_dump( &msg->msg.srch_msg.search );
      /* Process the search dump by srch4 driver */

    if ( hdrsrchdrv_dump_is_ready() )
    {
      hdrsrchstate.state->dump();
        /* State specific processing of srch results */
    }

    hdrsrchmsg_free_srch_rsp( msg );
      /* Return the buffer to the free queue */
  }

  hdrsrchdrv_set_dump_status( FALSE );
    /* Clear the dump status */

} /* hdrsrch_process_srch_rsp() */


/*=========================================================================

FUNCTION    HDRSRCH_DISCARD_SRCH_DUMP

DESCRIPTION
  This function discards the srch responses in the queue.

DEPENDENCIES
  TBD

INPUTS
  msg - Message pointer

RETURN VALUE
  None

SIDE EFFECTS
  

=========================================================================*/

void hdrsrch_discard_srch_dump( void )
{
  hdrsrchmsg_s_type         *msg;
    /* Message */

/*-----------------------------------------------------------------------*/

  while( ( msg = hdrsrchmsg_get_srch_rsp() ) != NULL )
  {
    hdrsrchmsg_free_srch_rsp( msg );
      /* Return the buffer to the free queue */
  }

  hdrsrchdrv_set_dump_status( FALSE );
    /* Clear the dump status */

} /* hdrsrch_discard_srch_dump() */


/*=========================================================================

FUNCTION HDRSRCHSTATE_DO_ON_DUMP

DESCRIPTION
  Called after regular "dump" processing.  This function executes any
  function stored by "on_dump".

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  "on_dump" function is cleared and executed.

=========================================================================*/

LOCAL void hdrsrchstate_do_on_dump( void )
{
  hdrsrch_on_event_func   on_dump;
    /* Storage for function to call */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchstate.on_dump != NULL )
  {
    on_dump = hdrsrchstate.on_dump;
      /* Retrieve the function to call */

    hdrsrchstate.on_dump = NULL;
      /* Blank the on_dump function, in case we need to set it again in
         the processing of this "on_dump( )" function call. */

    on_dump( );
      /* And call the required on_dump function */
  }

} /* hdrsrchstate_do_on_dump( ) */


/*=========================================================================

FUNCTION HDRSRCHSTATE_DO_ON_SEARCHER_IDLE

DESCRIPTION
  Called when the searcher is idle and any of "on searcher idle" functions is
  registered.  This function executes the given function, and deregisters it.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  "on_searcher_idle" function is cleared and executed.

=========================================================================*/

LOCAL void hdrsrchstate_do_on_searcher_idle( void )
{
  uint8 i;
    /* Loop index */

  hdrsrch_on_event_func   on_searcher_idle;
    /* Storage for function to call */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( i = 0; i < HDRSRCH_MAX_ON_SEARCH_IDLE; i ++ ) 
  {
    /* Loop through from high priority to low priority */

    on_searcher_idle = hdrsrchstate.on_searcher_idle[i];
      /* Retrieve the function to call */

    if ( on_searcher_idle != NULL )
    {

      hdrsrchstate.on_searcher_idle[i] = NULL;
        /* Blank the on_searcher_idle function, in case we need to set it
           again in the processing of the "on_searcher_idle()" function call */

      on_searcher_idle( );
    }
  }
    /* And call the required on_searcher_idle function */

} /* hdrsrchstate_do_on_searcher_idle( ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_DISABLE_SEARCHER_UNTIL_SHUTDOWN

DESCRIPTION
  This function is called by hdrsrch_task() to disable HDR SRCH for a 
  non-DO MSM.

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  This function may never return.

=========================================================================*/

void hdrsrch_disable_searcher_until_shutdown( void )
{
  hdrsrch_cmd_type *cmd;
    /* command structure pointer */

  rex_sigs_type     sigs;
    /* Activate task signals */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "Disable searcher util shutdown" );

  hdrsrchcmd_init();
    /* Initialize command engine */

  rex_def_timer( &hdrsrch.dog_timer, HDRSRCH_TCB, HDRSRCH_WDOG_SIG );
    /* Define watchdog timer */

  hdrsrch_process_wdog();
    /* Report wdog */

  hdrsrch_state_id = HDRSRCH_NO_STATE;
    /* No current and next states */

  hdrsrchstate.wait_mask = 0;
    /* Start off with no signals in wait mask */

  hdrsrch.cmd = NULL;
    /* Start with no commands pending */

  while( 1 ) /* Never ending loop */
  {
    sigs = rex_wait( HDRSRCH_DOG_HB_SIG | TASK_STOP_SIG | 
                     TASK_OFFLINE_SIG | HDRSRCH_CMD_Q_SIG);
      /* Retrieve any active signals the task is waiting for */

    cmd = NULL;
      /* Assume no command */

    if (sigs & HDRSRCH_CMD_Q_SIG)
    {
      HDRSRCH_SIG_CLR( HDRSRCH_CMD_Q_SIG );
      cmd = hdrsrchcmd_get();
        /* Command is fetched but not acted upon */

      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                      "Command sent while searcher shutdown: %d", cmd);
    }

    if ( sigs & HDRSRCH_DOG_HB_SIG )
    {
      hdrsrch_process_wdog();
        /* Report wdog */
    }
    else if ( sigs & TASK_OFFLINE_SIG )
    {
      HDRSRCH_SIG_CLR( TASK_OFFLINE_SIG );
      hdrsrchstate_offline( );
    }
    else if ( sigs & TASK_STOP_SIG )
    {
      HDRSRCH_SIG_CLR( TASK_STOP_SIG );
      hdrsrchstate_stop();
    }
  }

} /* hdrsrch_disable_searcher_until_shutdown */


/*==========================================================================

FUNCTION  HDRSRCH_DAL_INIT

DESCRIPTION
  This function will read Dal interface to get chip information.
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrch_dal_init( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 if (DAL_DeviceAttach(DALDEVICEID_CHIPINFO, &hdr_dal_chip_info) != DAL_SUCCESS)
 {
   return;
 }
  
 DalDevice_Open(hdr_dal_chip_info, DAL_OPEN_SHARED);

 DalChipInfo_GetChipFamily(hdr_dal_chip_info, &hdr_dal_chip_family);

 DalChipInfo_GetChipVersion(hdr_dal_chip_info, &hdr_dal_chip_version);
 
 HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "HDR Init HW detection Chip Family %d, Rev. %d \n", 
                 hdr_dal_chip_family, hdr_dal_chip_version >> 16 );

} /* hdrsrch_dal_init */


/*==========================================================================

FUNCTION  HDRSRCH_set_fing_and_div_limitation

DESCRIPTION
  This function will set finger max number and diversity on/off based on
  whether the chip is V2 HW or not.
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrch_set_fing_and_div_limitation( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdr_dal_chip_family == DALCHIPINFO_FAMILY_MSM8960 )
  {
    if( hdr_dal_chip_version < DALCHIPINFO_VERSION( 2, 0 ) )
    {
      if ( hdrsrch.aset_channel_cnt == 1 )
      {
        hdrsrchfing_limit_max_fings( FALSE );
          /* Single carrier */
      
        hdrsrchrfdiv_user_diversity_pref( hdrsrch.div_ctrl, hdrsrch.div_band_mask );
          /* Use the NV setting for diversity */
      }
      else
      {
        hdrsrchfing_limit_max_fings( TRUE );
          /* Muli-carriers*/
      
        hdrsrchrfdiv_user_diversity_pref( 0, 0 );
          /* Disable div for multi-carriers */
      }
    }
  }
} /* hdrsrch_set_fing_and_div_limitation */

/*===========================================================================
FUNCTION HDRSRCH_READ_EFS_NV_ITEM_CONF_FILE

DESCRIPTION
  Read value of EFS NV items.
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
 None

SIDE EFFECTS
  None

NOTE
  Please list all the EFS/NV item nos. & description in the commnents below
===========================================================================*/
void hdrsrch_read_efs_nv_items( void )
{

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV

  if ( hdrutil_fs_get( HDRSRCH_EFS_NV_ACQ_DIV_ENABLE,
                       &hdrsrch.acq_div.acq_div_nv_enable,
                       sizeof(uint8) ) == MCFG_FS_STATUS_OK )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
             "ACQ Div: hdr_acq_div_enable =%d",
             hdrsrch.acq_div.acq_div_nv_enable);
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
            "ACQ Div: hdr_acq_div_enable is not available in NV, set it: 0");
    hdrsrch.acq_div.acq_div_nv_enable = 0;  
      /* Disable acquisition diversity if the NV is not available */ 
  }

  if ( hdrutil_fs_get( HDRSRCH_EFS_NV_ACQ_DIV_THRESH0,
                       &hdrsrch.acq_div.acq_div_thresh0,
                       sizeof(int16) ) == MCFG_FS_STATUS_OK )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
             "ACQ Div: hdr_acq_div_thresh0=%d ",
             hdrsrch.acq_div.acq_div_thresh0 );
  }
  else
  {
    hdrsrch.acq_div.acq_div_thresh0 = HDRSRCH_ACQ_DIV_THRESH0_DEFAULT;
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
            "ACQ Div: hdr_acq_div_thresh0 is not available in NV,set it:%d",
             hdrsrch.acq_div.acq_div_thresh0);
  }


  if ( hdrutil_fs_get( HDRSRCH_EFS_NV_ACQ_DIV_THRESH1,
                       &hdrsrch.acq_div.acq_div_thresh1,
                       sizeof(int16) ) == MCFG_FS_STATUS_OK )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
             "ACQ Div: hdr_acq_div_thresh1=%d ",
             hdrsrch.acq_div.acq_div_thresh1 );
  }
  else
  {
    hdrsrch.acq_div.acq_div_thresh1 = HDRSRCH_ACQ_DIV_THRESH1_DEFAULT;

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
            "ACQ Div: hdr_acq_div_thresh1 is not available in NV,set it: %d",
             hdrsrch.acq_div.acq_div_thresh1 );
  }
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV

  if ( hdrutil_fs_get( HDRSRCH_EFS_NV_IDLE_DIV_PSINR_THRESH,
                       &hdrsrch.idle_state_div.psinr_thresh,
                       sizeof(int16) ) == MCFG_FS_STATUS_OK )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
             "Idle Div: state diversity psinr_thresh =%d",
             hdrsrch.idle_state_div.psinr_thresh);
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
            "Idle Div: psinr_thresh value not available in NV");
   hdrsrch.idle_state_div.psinr_thresh = 
      HDRSRCH_IDLE_STATE_DIV_PSINR_THRESHOLD_DEFAULT;
     
  }

  if ( hdrutil_fs_get( HDRSRCH_EFS_NV_IDLE_DIV_SUBPKT_THRESH,
                       &hdrsrch.idle_state_div.subpkt_thresh,
                       sizeof(uint8) ) == MCFG_FS_STATUS_OK )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
             "Idle Div: subpkt_thresh=%d ",
             hdrsrch.idle_state_div.subpkt_thresh);
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
            "Idle Div: Pref default subpkt thresh not available in NV");
   
    hdrsrch.idle_state_div.subpkt_thresh = 
      HDRSRCH_IDLE_STATE_DIV_SUBPKT_THRESH_DEFAULT;    
  }

  if ( hdrutil_fs_get( HDRSRCH_EFS_NV_IDLE_DIV_SCI_THRESH,
                       &hdrsrch.idle_state_div.sci_thresh,
                       sizeof(uint8) ) == MCFG_FS_STATUS_OK )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
             "Idle Div: sci_thresh=%d ",
             hdrsrch.idle_state_div.sci_thresh);
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
            "Idle Div:Pref default idle div value not available in NV");
   
    hdrsrch.idle_state_div.sci_thresh = 
      HDRSRCH_IDLE_STATE_DIV_SCI_THRESH_DEFAULT;
  }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

}

/*===========================================================================
FUNCTION HDRSRCH_CREATE_EFS_ITEM_CONF_FILE

DESCRIPTION
  Put(append and add newline) item_file_path into conf_file_path
  A call to this function is necessary so that QPST can back up EFS items
  to QCN files. For HDR SRCH, use "/nv/item_files/conf/hdrsrch_config_info.conf"
  for conf_file_path.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
 None

SIDE EFFECTS
  None

NOTE
  Please list all the EFS/NV item nos. & description in the commnents below
===========================================================================*/
void hdrsrch_create_efs_item_conf_file( void )
{
  int32              config_fd, result;

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
  char               new_line[] = "\n";
#endif 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 


  /* Open conf file. If it doesn't exist create it and reopen it. */
  config_fd = hdrutil_fs_open( HDRSRCH_EFS_NV_CONF_FILE_PATH, O_RDWR|O_APPEND );
  if ( config_fd >= 0  )
  {

    result = hdrutil_fs_close ( config_fd );
    return;
      /* File already exists, no need to put nv item into conf file */
  }
  else 
  {
    if ( ENOENT == hdrutil_fs_errno() ) /* Conf file does not exist */
    {
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Creating EFS NV item config file");

      config_fd = hdrutil_fs_open( HDRSRCH_EFS_NV_CONF_FILE_PATH, 
                                   O_WRONLY|O_CREAT|O_TRUNC );

      if ( 0 > config_fd )
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                        "Error creating EFS NV item config file errno %d",
                        hdrutil_fs_errno() );
        return;
      }

      result = hdrutil_fs_close( config_fd );
      if ( 0 != result )
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                        "Error closing newly created EFS NV item config file errno %d",
                        hdrutil_fs_errno() );
        return;
      }

      config_fd = hdrutil_fs_open( HDRSRCH_EFS_NV_CONF_FILE_PATH, O_RDWR|O_APPEND );
      if ( 0 > config_fd )
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                        "Error opening newly created EFS NV item config file, errno %d",
                        hdrutil_fs_errno() );
        return;
      }
    }
    else /* Could not open conf file for some other reason */
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Error opening config file errno %d", 
                      hdrutil_fs_errno() );
      return;
    }

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
    result = hdrutil_fs_write( config_fd, HDRSRCH_EFS_NV_IDLE_DIV_PSINR_THRESH,
                      strlen( HDRSRCH_EFS_NV_IDLE_DIV_PSINR_THRESH ) );
    result += hdrutil_fs_write( config_fd, new_line, 1 );
    if ( result != ( strlen( HDRSRCH_EFS_NV_IDLE_DIV_PSINR_THRESH )+1 ) )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Error writing to config file errno %d", 
                      hdrutil_fs_errno() );
      result = hdrutil_fs_close( config_fd );
      return;
    }

    result = hdrutil_fs_write( config_fd, HDRSRCH_EFS_NV_IDLE_DIV_SUBPKT_THRESH,
                               strlen( HDRSRCH_EFS_NV_IDLE_DIV_SUBPKT_THRESH ) );
    result += hdrutil_fs_write( config_fd, new_line, 1 );
    if ( result != ( strlen( HDRSRCH_EFS_NV_IDLE_DIV_SUBPKT_THRESH )+1 ) )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Error writing to config file errno %d", 
                      hdrutil_fs_errno() );
      result = hdrutil_fs_close( config_fd );
      return;
    }

    result = hdrutil_fs_write( config_fd, HDRSRCH_EFS_NV_IDLE_DIV_SCI_THRESH,
                               strlen( HDRSRCH_EFS_NV_IDLE_DIV_SCI_THRESH ) );
    result += hdrutil_fs_write( config_fd, new_line, 1 );
    if ( result != ( strlen( HDRSRCH_EFS_NV_IDLE_DIV_SCI_THRESH )+1 ) )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Error writing to config file errno %d", 
                      hdrutil_fs_errno() );
      result = hdrutil_fs_close( config_fd );
      return;
    }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
    result = hdrutil_fs_write( config_fd, HDRSRCH_EFS_NV_ACQ_DIV_ENABLE,
                               strlen( HDRSRCH_EFS_NV_ACQ_DIV_ENABLE ) );
    result += hdrutil_fs_write( config_fd, new_line, 1 );
    if ( result != ( strlen( HDRSRCH_EFS_NV_ACQ_DIV_ENABLE )+1 ) )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Error writing to config file errno %d", 
                      hdrutil_fs_errno() );
      result = hdrutil_fs_close( config_fd );
      return;
    }

    result = hdrutil_fs_write( config_fd, HDRSRCH_EFS_NV_ACQ_DIV_THRESH0,
                               strlen( HDRSRCH_EFS_NV_ACQ_DIV_THRESH0 ) );
    result += hdrutil_fs_write( config_fd, new_line, 1 );
    if ( result != ( strlen( HDRSRCH_EFS_NV_ACQ_DIV_THRESH0 )+1 ) )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Error writing to config file errno %d", 
                      hdrutil_fs_errno() );
      result = hdrutil_fs_close( config_fd );
      return;
    }

    result = hdrutil_fs_write( config_fd, HDRSRCH_EFS_NV_ACQ_DIV_THRESH1,
                               strlen( HDRSRCH_EFS_NV_ACQ_DIV_THRESH1 ) );
    result += hdrutil_fs_write( config_fd, new_line, 1 );
    if ( result != ( strlen( HDRSRCH_EFS_NV_ACQ_DIV_THRESH1 )+1 ) )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Error writing to config file errno %d", 
                      hdrutil_fs_errno() );
      result = hdrutil_fs_close( config_fd );
      return;
    }                                
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

#ifdef FEATURE_HDR_SELECTIVE_TXD
    result = hdrutil_fs_write( config_fd, HDRSRCH_EFS_NV_STXD_CONFIG,
                               strlen( HDRSRCH_EFS_NV_STXD_CONFIG ) );
    result += hdrutil_fs_write( config_fd, new_line, 1 );
    if ( result != ( strlen( HDRSRCH_EFS_NV_STXD_CONFIG )+1 ) )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Error writing to config file errno %d", 
                      hdrutil_fs_errno() );
      result = hdrutil_fs_close( config_fd );
      return;
    }

    result = hdrutil_fs_write( config_fd, HDRSRCH_EFS_NV_STXD_INTERNAL_CONFIG,
                               strlen( HDRSRCH_EFS_NV_STXD_INTERNAL_CONFIG ) );
    result += hdrutil_fs_write( config_fd, new_line, 1 );
    if ( result != ( strlen( HDRSRCH_EFS_NV_STXD_INTERNAL_CONFIG )+1 ) )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Error writing to config file errno %d", 
                      hdrutil_fs_errno() );
      result = hdrutil_fs_close( config_fd );
      return;
    }

    result = hdrutil_fs_write( config_fd, HDRSRCH_EFS_NV_STXD_ANT_IMBAL_CONFIG,
                               strlen( HDRSRCH_EFS_NV_STXD_ANT_IMBAL_CONFIG ) );
    result += hdrutil_fs_write( config_fd, new_line, 1 );
    if ( result != ( strlen( HDRSRCH_EFS_NV_STXD_ANT_IMBAL_CONFIG )+1 ) )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Error writing to config file errno %d", 
                      hdrutil_fs_errno() );
      result = hdrutil_fs_close( config_fd );
      return;
    }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

    result = hdrutil_fs_close ( config_fd );
    if ( 0 != result )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                      "Error closing newly created EFS NV item config file errno %d",
                      hdrutil_fs_errno() );
      return;
    }
  }
} /* hdrsrch_create_efs_item_conf_file() */

#ifdef FEATURE_HDR_RUMI
/*==========================================================================

FUNCTION        hdrsrchacq_check_sram

DESCRIPTION
  Funciton to chekc if SRAM is stuck/all 0. Needed for bringup

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Many

==========================================================================*/
void hdrsrchstate_sram_check_cb
( 
  uint32 timer_id
)
{
  uint32* sram_start_addr = (uint32*) 0xedc09e00;
    /* must be updated for proper address */

  uint32 sample = 0;
  uint32 sum = 0;  
  uint8 i;
  
  static uint32 last_sample = 0;
  static uint32 last_sum = 0;
  static uint8 stuck_cnt = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  sample = *sram_start_addr;
  sum = sample;

  for (i=1; i<8; i++)
  {
    sum += *(sram_start_addr+i);
  }

  if ( ( last_sum == sum ) &&
       ( last_sample == sample ) )
  {
    stuck_cnt++;

    HDR_MSG_SRCH_3( MSG_LEGACY_ERROR, "SRAM STUCK: %x/%x/%d", 
                    sample, sum, stuck_cnt );

    ASSERT(stuck_cnt <= 5);
  }
  else
  {
    stuck_cnt = 0;

    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "SRAM OK: %x/%x", 
                    sample, sum );
  }  

  last_sample = sample;
  last_sum = sum;

  (void) rex_set_timer( &hdrsrchstate.sram_stuck_timer, 10 );
}  /* hdrsrchacq_check_sram */
#endif /* FEATURE_HDR_RUMI */

/* EJECT */
/*=========================================================================

FUNCTION HDRSRCH_TASK

DESCRIPTION
  This function implements a state machine for the HDR Searcher.  This
  function will only return when the HDR Searcher is halted.

DEPENDENCIES
  This function must only be called from the main search task, or perhaps
  a new task dedicated to HDR search processing.  This function does not
  return until the HDR Searcher is halted.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  This function may not return for a long, long time.

=========================================================================*/

void hdrsrch_task
(
  dword param /* dummy parameter to make rex_def_task happy */
)
{

  hdrsrch_state_enum_type last_state_id;
    /* Temporary to hold last state on state transitions */

  hdrsrch_cmd_type        *cmd;
    /* Command structure pointer */

  rex_sigs_type           sigs;
    /* Active task signals */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  rcinit_handshake_startup();

  /* Run time synch with other tasks is done here if needed, following APIs
  ** used: rcinit_lookup(),  rcevt_wait_name()
  */

  if ( rcevt_wait_name(HDRRX_RCEVT_READY) == RCEVT_NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Unexpected Run Control Event Notification err" );
  }
  if ( rcevt_wait_name("TRM_INIT_COMPLETE") == RCEVT_NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Unexpected Run Control Event Notification err" );
  }
  if ( rcevt_signal_name(HDRSRCH_RCEVT_READY) == RCEVT_NULL ) 
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Unexpected Run Control Event Notification err" );
  }

  /* RCINIT calls init function defined in task scons file in rcinit context
     All tcb related intializations should be done after rcinit starts our
     task */
  hdrsrch_tcb_ptr = rex_self();

#ifdef FEATURE_MSM_SUPPORT

  if( msm_support_is_available( MSM_SUPPORT_1XEVDO ) == 
      MSM_SUPPORT_DISABLED )
  {
      /* MSM does not support DO */
      hdrsrch_disable_searcher_until_shutdown();
  }
  else
  {
#endif /* FEATURE_MSM_SUPPORT*/

    hdrsrchmsg_init();
     /* Initialize MSGR functionality */

    hdrsrchdrv_init();
      /* initialize searcher drivers to a known state */

    hdrsrchfing_init( );
      /* initialize finger drivers to a known state */

#ifdef FEATURE_HDR_TO_LTE
    hdrsrchltemeas_powerup_init();
      /* initialize D2L parameters */
#endif /* FEATURE_HDR_TO_LTE */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   
    /* Define Timers */

    rex_def_timer( &hdrsrch.dog_timer,     HDRSRCH_TCB, HDRSRCH_WDOG_SIG);
    rex_def_timer( &hdrsrch.schedule_timer,HDRSRCH_TCB, HDRSRCH_SCHEDULE_SIG);
    rex_def_timer( &hdrsrch.lostdump_timer,HDRSRCH_TCB, HDRSRCH_LOSTDUMP_SIG);
    rex_def_timer( &hdrsrch.timeout_timer, HDRSRCH_TCB, HDRSRCH_TIMEOUT_SIG);
    rex_def_timer( &hdrsrch.gp_timer0,     HDRSRCH_TCB, HDRSRCH_GP_TMR0_SIG);
    rex_def_timer( &hdrsrch.gp_timer1,     HDRSRCH_TCB, HDRSRCH_GP_TMR1_SIG);

    rex_def_timer( &hdrsrch.gp_timer2, HDRSRCH_TCB, HDRSRCH_TIMED_WAIT_SIG );

    hdrsrchreacq_define_fpd_timer();
      /* Define FPD timer */

#ifdef FEATURE_MSM_SUPPORT
  }
#endif

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
    hdrsrchtclte_powerup_init();
      /* Initialize DO to LTE redirection parameters (connected state) */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

    hdrsrch_dog_rpt_var = dog_hb_register_rex( (rex_sigs_type) HDRSRCH_DOG_HB_SIG );
      /* Register with dog HB */

    (void) rex_set_timer( &hdrsrch.dog_timer, HDRSRCH_DOG_RPT_TIME_MS );
      /* Kick start internal stale timeout counter */

    /* TBD: This funtion has to be called after MDSP task is active*/
    hdrmdsp_init();

    hdrsrch_read_efs_nv_items();

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW

    hdrmultirat_powerup_init();

    timer_def( &hdrsrch.coex_timer, NULL, HDRSRCH_TCB,
               0, hdrsrchutil_report_coex_power, 0 );
      /* Define the coex_power warm up timer. */

#ifdef FEATURE_HDR_BAND_AVOIDANCE

    timer_def( &hdrsrch.band_avoid_timer, NULL, HDRSRCH_TCB,
               0, hdrsrchutil_save_ba_power, 0 );
     /* Initialize band avoidance timer */
#endif /* FEATURE_HDR_BAND_AVOIDANCE */

#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_RUMI
    if ( hdrhit_rumi_cfg.sram_check )
    {
      rex_def_timer_ex( &hdrsrchstate.sram_stuck_timer, 
                        hdrsrchstate_sram_check_cb, 
                        0 );
    }
#endif /* FEATURE_HDR_RUMI */

    HDR_MSG_SRCH( MSG_LEGACY_LOW, "HDR Search State Machine - started" );

    hdrsrch_state_id = HDRSRCH_NO_STATE;
    /* No current state */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
    hdrsrchutil_reset_prev_wakeup_info();
    /* Reset all previous wakeup info to defaults. */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

    hdrsrchrf_register_with_trm();
      /* Register for trm callbacks */

    hdrsrchstate_set_revb_mode( FALSE );
      /* Initialize to non-REVB mode */

    hdrsrchstate.wait_mask = HDRSRCH_TICKLE_SIG |
                             HDRSRCH_DEACTIVATE_SIG |
                             HDRSRCH_INT_CMD_SIG |
                             HDRSRCH_HIGH_PRI_RSP_SIG |
                             HDRSRCH_LOW_PRI_RSP_IND_SIG;

    hdrsrchstate.guard1 = 0xDEADBEEF;
    hdrsrchstate.guard2 = 0xDEADBEEF;

    /* Start off with these signals in wait mask:
         tickle signal       - to allow state changes from ISR context,
         deactivate signal   - to deactivate in task context
         internal cmd signal - to allow stm commands
         message signal      - to allow messages
       Note: HDRSRCH_INT_CMD_SIG is necessary when an stm command is sent
             to HDR srch state machine from an external task.
             stm_process_group() will clear this signal which is defined in
             an smf file */

    hdrsrch.cmd = NULL;
    /* And start with no commands pending */

    hdrsrch.tc_mode = HDRSRCH_DEFAULT_TC_MODE;
    /* Start off with the default traffic mode. */

    hdrsrch.handdown_mode = HDRSRCH_1X_HANDDOWN_ENABLED_MODE;
      /* Enable handown from HDR to 1x */

    hdrsrch_clear_on_searcher_idle_proc();
    /* And start with no functions pending to be executed on dump */

    hdrsrchbm_init_cmd_buf_pool();
    /* Initialize the buffer manager */

    hdrsrchafc_init();
    /* Initialize the AFC module and register the state machine callback
       to recieve incoming TCXOMgr commands.  The state machine can 
       act on them or just pass them on to hdrsrchafc for handling. */

    hdrsrchrf_init_tune_timer();
    /* Initialize the RF tune timer */

    stm_start_group( &hdrsrchcom_stm_group, FALSE );
    /* Start common stm group */

    stm_start_group( &hdrsrch_stm_group, FALSE );
    /* Start hdr srch stm group */

#ifdef FEATURE_HDR_SELECTIVE_TXD
    hdrsrchrftxd_start_stm();
    /* TxD needs to be started before ACQ state, because it needs to 
       be up and running in ACQ state */
#endif /* FEATURE_HDR_SELECTIVE_TXD */

    hdrsrchst_init_inactive_state();
    /* Transition to the start state */

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
    hdrsrchrf_register_pbr_mode_with_trm();
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */

    hdrsrchrf_register_aps_mode_with_trm();
      /* Register APS for BG_TRAFFIC */

    if ( rcevt_wait_name(RCINIT_RCEVT_INITGROUPS) == RCEVT_NULL ) 
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Unexpected Run Control Event Notification err" );
    }

    while ( hdrsrchstate.next_state != NULL )
    {
      /* -------------------------------------------------------------------
         State Entry
      ------------------------------------------------------------------- */

      last_state_id         = hdrsrch_state_id;
      /* Remember the "last" state we were in */

      hdrsrchstate.state    = hdrsrchstate.next_state;
      /* Set the new state method table */

      hdrsrch_state_id      = hdrsrchstate.state->id;
      /* And set the current state id */

      hdrsrchstate.state->init( last_state_id );
      /* State initialization */

      /* -------------------------------------------------------------------
         State Event Response Loop
      ------------------------------------------------------------------- */

      HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();

      while ( hdrsrchstate.state == hdrsrchstate.next_state )
      {
        HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();

        sigs = HDRSRCH_SIG_GET() & hdrsrchstate.wait_mask;
        /* Retrieve any active signals the task is waiting for */

        /* Internal commands are highest priority. Make sure the int cmd queue
           is drained before moving on to process external commands and
           other signals. */
        if ( sigs & HDRSRCH_INT_CMD_SIG )
        {
          HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                          "SM Debug: Processing Internal Cmnd Signal %x and hdrsrch_stm_group %d", 
                           sigs & HDRSRCH_INT_CMD_SIG,
                           stm_process_group( &hdrsrch_stm_group));/* stm_process_group() will drain all commands in queue */

          HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
        }
        else
        {

          if ( hdrsrch.cmd             != NULL               &&  
               hdrsrch.cmd->status     == E_AGAIN            &&
               !hdrsrchafc_service_request_pending()         &&
               (hdrsrchstate.wait_mask &  HDRSRCH_TUNE_SIG ) == 0 )
          {
            /* A command was received in a state it wasn't ready for, and we
               have now switched to a new state. We are going to keep checking
               the retry condition, until the command is successfully retried.
               Failure to retry the command can stall the command processing */

            HDR_MSG_SRCH_2( MSG_LEGACY_LOW, "Command %d retried in state %x",
                            hdrsrch.cmd->name, 
                            hdrsrch_state_id );

            if ( hdrsrch_retry_cmd( ) == HDRSRCH_CMD_EXECUTED )
            {
              continue;
            }

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();

            /* Attempt the command again in this new state. The pending command
               that has been executed during command retrying has the potential
               of causing a search state transition. The search state needs to
               be checked after the command execution. Otherwise the next command
               or signal might be processed in a state not intended           */
          }

          /* Special case for the command queue, ignore the command queue
             signal, and retrieve a command pointer if we are waiting for
             commands.  Otherwise, a race condition where commands are in
             the queue, but the signal has been reset could stall the command
             processing. */

          cmd = NULL;
          /* Assume no command */

          /* Process HDRSRCH_DEACTIVATE_SIG if set before any other 
             command or signal */
          if ( sigs & HDRSRCH_DEACTIVATE_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_DEACTIVATE_SIG );
            hdrsrch_abort_command( );

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }          

          if ( ( (hdrsrchstate.wait_mask & HDRSRCH_CMD_Q_SIG) != 0  ) &&
               ( !hdrsrchafc_service_request_pending() ) &&
               ( (hdrsrchstate.wait_mask & HDRSRCH_TUNE_SIG ) == 0  )  )
          {
            /* Avoid processing command if we have a tune in progress,
               to prevent a command from turning off clocks we
               expect to use when the tune completes */

            HDRSRCH_SIG_CLR( HDRSRCH_CMD_Q_SIG );
            cmd = hdrsrchcmd_get();
            /* Attempt to retrieve a command */
          }          

          if ( cmd != NULL )
          {
            /* Log the command event */

            hdrsrch_process_cmd( cmd );

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_WDOG_SIG )  
          {
            HDRSRCH_SIG_CLR( HDRSRCH_WDOG_SIG );
            hdrsrchstate_process_wdog_timer();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_DOG_HB_SIG ) 
          {
            hdrsrch_process_wdog();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_DUMP_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_DUMP_SIG );
            hdrsrch_process_srch_rsp();
            hdrsrchstate_do_on_dump();
            hdrsrchstate.dog.dump_occurred = TRUE;

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_ROLL_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_ROLL_SIG );
            hdrsrchstate.state->roll();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_WAKE_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_WAKE_SIG );
            hdrsrchstate.state->wake();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_TUNE_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_TUNE_SIG );
            hdrsrchrf_tune_sig( HDRSRCHRF_PRI_CHAIN );

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_SCHEDULE_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_SCHEDULE_SIG );
            hdrsrchstate.state->sched();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_LOSTDUMP_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_LOSTDUMP_SIG );
            hdrsrchstate.state->lost_dump();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_TIMEOUT_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_TIMEOUT_SIG );
            hdrsrchstate.state->timeout();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_GP_TMR0_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_GP_TMR0_SIG );
            hdrsrchstate.state->gp_timer0();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_GP_TMR1_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_GP_TMR1_SIG );
            hdrsrchstate.state->gp_timer1();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_GP_SIG0_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_GP_SIG0_SIG );
            hdrsrchstate.state->gp_sig0();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_RXTX_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_RXTX_SIG );
            /* Handle RxTx event first, and only passes event
               to each state's handler if needed */
            hdrsrchrf_rxtx_event_handler();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_RXCHIPX16_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_RXCHIPX16_SIG );
            hdrsrchstate.state->rxchipx16();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_TIME_VALID_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_TIME_VALID_SIG );
            hdrsrchstate.state->time_valid();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_PRI_CHAIN_EVENT_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_PRI_CHAIN_EVENT_SIG );
            hdrsrchstate.state->pri_chain_event();
            /* Each state is responsible for primary chain events. */

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_HIGH_PRI_RSP_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_HIGH_PRI_RSP_SIG );
            hdrsrch_process_high_pri_rsp();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_LOW_PRI_RSP_IND_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_LOW_PRI_RSP_IND_SIG );
            hdrsrch_process_low_pri_rsp_ind();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & HDRSRCH_TICKLE_SIG )
          {
            HDRSRCH_SIG_CLR( HDRSRCH_TICKLE_SIG );
          }
          else if ( sigs & TASK_OFFLINE_SIG )
          {
            HDRSRCH_SIG_CLR( TASK_OFFLINE_SIG );
            hdrsrchstate_offline( );

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else if ( sigs & TASK_STOP_SIG )
          {
            HDRSRCH_SIG_CLR( TASK_STOP_SIG );
            hdrsrchstate_stop( );

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }

          /* Special case for on_searcher_idle processing. If we use "sigs" here,
             it is possible that we hit a race condition where HDRSRCH_DUMP_SIG
             occured "after" the variable "sigs" was read. In that case we would
             be executing on_searcher_idle() while dump proc is pending. Read
             the task signal mask directly and check for HDRSRCH_DUMP_SIG */

          else if ( hdrsrch_on_searcher_idle_reged() &&
                    hdrsrchdrv.active_srch == SRCH_STATUS_INACTIVE &&
                    ( HDRSRCH_SIG_GET() & HDRSRCH_DUMP_SIG )   == 0 )
          {
            hdrsrchstate_do_on_searcher_idle();

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
          else
          {
            if ( hdrsrchstate.wait_mask & HDRSRCH_PRI_CHAIN_EVENT_SIG )
            {
              hdrsrch_add_to_wait_mask( HDRSRCH_RXTX_SIG );
            }
            else
            {
              hdrsrch_remove_from_wait_mask( HDRSRCH_RXTX_SIG );
            }

            if ( hdrsrchstate.wait_mask & HDRSRCH_TUNE_SIG ) 
            {
              /* If HDR SRCH is tuning RF, it cannot process external commands.
                 It should not wait on HDRSRCH_CMD_Q_SIG. Otherwise, rex_wait() 
                 returns immediately if HDRSRCH_CMD_Q_SIG is set. The task 
                 then busy waits and starves the lower priority tasks. */

              ( void ) rex_wait( hdrsrchstate.wait_mask & ( ~HDRSRCH_CMD_Q_SIG ) );
            }
            else
            {
              ( void ) rex_wait( hdrsrchstate.wait_mask );
            }

            HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();
          }
        }

      } /* while same state */


      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "HDR SRCH State Change: %x --> %x ",
                      hdrsrchstate.state->id, hdrsrchstate.next_state_id );
      /* Log this event of state change */

      if ( hdrsrch_on_searcher_idle_reged() )
      {
        HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "On_Searcher_Idle is pending: %x %x",
                        hdrsrchstate.on_searcher_idle[0],
                        hdrsrchstate.on_searcher_idle[1] );
      }

      hdrsrchlog_update_srch_state_info( hdrsrchstate.state->id, 
                                         hdrsrchstate.next_state_id );
      /* Let the tune away logging module know about this state change
         so that it may log tune away statistics. */

      /* -------------------------------------------------------------------
         State Exit
      ------------------------------------------------------------------- */

      hdrsrchstate.state->done( hdrsrchstate.next_state_id );
      /* State exit & cleanup */

      HDR_CHK_INT_CMD_SIG_AND_ERR_FATAL();

    } /* while state is not NULL (the exit state) */

#ifdef FEATURE_MSM_SUPPORT
    }
#endif /* FEATURE_MSM_SUPPORT */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_process_wdog();
    /* Pet the watchdog one last time before exiting the state machine.
       This will allow a bit of time for the next state-machine (if any) to
       take over the pacification of the watchdog */

  hdrsrch_kill_timer( &hdrsrch.dog_timer );
    /* Kill the dog timer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_LOW, "HDR Search State Machine - stopped" );

}   /* hdrsrch_task() */

/*===========================================================================

FUNCTION HDRSRCH_TIMED_WAIT_USECS

DESCRIPTION
  This function performs a timed wait. The timed value should be less
  than watch dog timeout period.

DEPENDENCIES
  None

PARAMETERS
  The time to wait in micro seconds.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrch_timed_wait_usecs
(
  uint16  us_to_wait
    /* time to wait in micro seconds */
)
{

  uint8   ms_to_wait = 0;
   /* time to wait in milli seconds */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( us_to_wait > 0 )
  {
    ms_to_wait = (us_to_wait + 999) / 1000;
      /* calculate nearest milli second >= us_to_wait */

    (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_TIMED_WAIT_SIG );
      /* Clear the timer signal */

    hdrsrch_add_to_wait_mask( HDRSRCH_TIMED_WAIT_SIG );
      /* update wait mask in HDRSRCH task to reflect that the task is
         currently waiting only on HDRSRCH_TIMED_WAIT_SIG */

    hdrsrch_process_wdog();
      /* pet the watchdog before a timed wait */

    (void) rex_timed_wait( HDRSRCH_TIMED_WAIT_SIG, 
                           &hdrsrch.gp_timer2, 
                           ms_to_wait );
      /* notify REX to set HDRSRCH_TIMED_WAIT_SIG after time interval
         specified by function parameter ms_to_wait.
         HDRSRCH task will wait only on HDRSRCH_TIMED_WAIT_SIG.
         Give up control to REX to schedule next ready task.  */

    /* control back to HDRSRCH task */

    hdrsrch_kill_signal( HDRSRCH_TIMED_WAIT_SIG );
      /* clear HDRSRCH_TIMED_WAIT_SIG from HDRSRCH tcb and
         remove the signal from wait mask managed in HDRSRCH task */
  }
} /* hdrsrch_timed_wait_usecs */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_WAIT_FOR_SIGS

DESCRIPTION
  This procedure waits for specified signals to be set.  This routine also
  checks any queues that are being waited on.  If the queue has an item, 
  this routine sets the signal without waiting.  This allows for the queue
  signal to be cleared after each item is removed.  If a queue signal is set,
  this routine checks the queue and clears the signal if there is nothing
  on it.  This centralizes the queue checking.

DEPENDENCIES
  None.

RETURN VALUE
  For non-queue signals, a signal bit is set in the return mask if the signal 
  was in the requested mask and is also set in the REX TCB.
  
  For queue signals, a signal bit is set in the return mask if the signal was 
  in the requested mask and the queue associated with the signal has an item
  on it.

SIDE EFFECTS
  None.

===========================================================================*/
rex_sigs_type hdrsrch_wait_for_sigs
(
  rex_sigs_type  requested_mask         /* Mask of REX signals to wait for */
)
{
  rex_sigs_type  rex_signals_mask; /* Signal mask returned by rex_get_sigs */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
   Loop until at least one requested signal is set
  -------------------------------------------------------------------------*/
  do {

    /* Actually wait for one of the requested signals to be set */
    rex_signals_mask = rex_wait( requested_mask | HDRSRCH_DOG_HB_SIG );

    /*-----------------------------------------------------------------------
     Watchdog report timer signal. Kick watchdog and reset timer.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRSRCH_DOG_HB_SIG) != 0 ) 
    {
      hdrsrch_process_wdog();
        /* Report wdog */
    }

  } while ( (requested_mask & rex_signals_mask) == 0 );

  /*-------------------------------------------------------------------------
   Kick watchdog one last time on the way out.
  -------------------------------------------------------------------------*/
  hdrsrch_process_wdog();

  /*-------------------------------------------------------------------------
   Assemble return mask.  Only return signals that were requested.
  -------------------------------------------------------------------------*/
  return ( rex_signals_mask & requested_mask );

}/* hdrsrch_wait_for_sigs */


/*===========================================================================

FUNCTION HDRSRCH_WAIT_FOR_HIGH_PRI_RSP

DESCRIPTION
  This function checks whether the response is received with a certain interval
  of time. The timed value should be less than watch dog timeout period.

DEPENDENCIES
  None

PARAMETERS
  us_to_wait - The time to wait in micro seconds.
  rsp       - The response to be received.

RETURN VALUE
  TRUE  - if the response is received.
  FALSE - if the response is not received within us_to_wait. 

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrsrch_wait_for_high_pri_rsp
(
  uint32                     us_to_wait,
    /* time to wait in micro seconds */

  hdrsrchmsg_rsp_e_type      rsp
    /* The response to be received */
)
{
  boolean                    received = FALSE;
    /* Whetehr the response is received */

  rex_sigs_type              sigs = 0;
    /* Signals received */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "TIMED_WAIT_SIG debug wait_for_high_pri_rsp(us=%d,rsp=%d)",
                  us_to_wait, rsp );

  received = hdrsrchmsg_rsp_is_rcvd( rsp );
    /* Check whether the response is received */

  if ( ( received == FALSE ) && ( us_to_wait > 0 ) )
  {
    hdrsrch_process_wdog();
      /* pet the watchdog before a timed wait */

    while ( received == FALSE ) 
    {
      sigs = rex_wait( HDRSRCH_HIGH_PRI_RSP_SIG );
        /* wait for signals */

      if ( sigs & HDRSRCH_HIGH_PRI_RSP_SIG ) 
      {
        (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_HIGH_PRI_RSP_SIG );
          /* Clear signal */

        hdrsrch_process_high_pri_rsp();
          /* Process high priority responses */

        received = hdrsrchmsg_rsp_is_rcvd( rsp );
      } /* if */

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "TIMED_WAIT_SIG debug received=%d,sigs=0x%x",
                      received, sigs );

    } /* while */

  } /* if ( ( received == FALSE ) && ( us_to_wait > 0 ) ) */

  return received;

} /* hdrsrch_wait_for_high_pri_rsp */


/*===========================================================================

FUNCTION HDRSRCH_TIMED_WAIT_FOR_HIGH_PRI_RSP

DESCRIPTION
  This function checks whether the response is received with a certain interval
  of time. The timed value should be less than watch dog timeout period.

DEPENDENCIES
  None

PARAMETERS
  us_to_wait - The time to wait in micro seconds.
  rsp       - The response to be received.

RETURN VALUE
  TRUE  - if the response is received.
  FALSE - if the response is not received within us_to_wait. 

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrsrch_timed_wait_for_high_pri_rsp
(
  uint32                     us_to_wait,
    /* time to wait in micro seconds */

  hdrsrchmsg_rsp_e_type      rsp
    /* The response to be received */
)
{
  uint8                      ms_to_wait = 0;
    /* time to wait in milli seconds */

  boolean                    received = FALSE;
    /* Whetehr the response is received */

  rex_sigs_type              sigs = 0;
    /* Signals received */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "TIMED_WAIT_SIG debug timed_wait_for_high_pri_rsp(us=%d,rsp=%d)",
                  us_to_wait, rsp );

  received = hdrsrchmsg_rsp_is_rcvd( rsp );
    /* Check whether the response is received */

  if ( ( received == FALSE ) && ( us_to_wait > 0 ) )
  {
    ms_to_wait = (us_to_wait + 999) / 1000;
      /* calculate nearest milli second >= us_to_wait */

    hdrsrch_process_wdog();
      /* pet the watchdog before a timed wait */

    (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_TIMED_WAIT_SIG );
      /* Clear the requested signals */

    while ( ( received == FALSE )                    && 
            ( ( sigs & HDRSRCH_TIMED_WAIT_SIG ) == 0 ) )
    {
      /* The response is not received and not time out */

      sigs = rex_timed_wait( HDRSRCH_TIMED_WAIT_SIG | HDRSRCH_HIGH_PRI_RSP_SIG,
                             &hdrsrch.gp_timer2, 
                             ms_to_wait );
      /* notify REX to set HDRSRCH_TIMED_WAIT_SIG after time interval
         specified by function parameter ms_to_wait.
         HDRSRCH task will wait only on HDRSRCH_TIMED_WAIT_SIG and
         HDRSRCH_HIGH_PRI_RSP_SIG. Give up control to REX to schedule
         next ready task.  */

      if ( sigs & HDRSRCH_HIGH_PRI_RSP_SIG ) 
      {
        (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_HIGH_PRI_RSP_SIG );
          /* Clear sugnal */

        hdrsrch_process_high_pri_rsp();
          /* Process high priority responses */

        received = hdrsrchmsg_rsp_is_rcvd( rsp );
      } /* if */

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "TIMED_WAIT_SIG debug received=%d,sigs=0x%x",
                      received, sigs );

    } /* while */

    hdrsrch_kill_signal( HDRSRCH_TIMED_WAIT_SIG );
      /* clear HDRSRCH_TIMED_WAIT_SIG from HDRSRCH tcb 
      and remove from wait mask */

  } /* if ( ( received == FALSE ) && ( us_to_wait > 0 ) ) */

  return received;

} /* hdrsrch_timed_wait_for_high_pri_rsp */


/*===========================================================================

FUNCTION HDRSRCH_PROCESS_INT_CMD_DEFAULT

DESCRIPTION
  This function is the default function to process the internal commands.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrch_process_int_cmd_default
(
  hdrsrchsm_cmd_payload_type    *payload
    /* The internal command + payload */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

}  /* hdrsrch_process_int_cmd_default */


/*===========================================================================

FUNCTION HDRSRCH_PROCESS_INT_CMD

DESCRIPTION
  This function processes internal commands.

DEPENDENCIES
  None

PARAMETERS
  The payload of the internal command.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrch_process_int_cmd
(
  void                       *payload
    /* Payload */
)
{
  hdrsrchsm_cmd_payload_type *hdrsrch_payload
    = ((hdrsrchsm_cmd_payload_type *) payload);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchstate.state->process_int_cmd( hdrsrch_payload );
    /* Process internal command by the current state */

}  /* hdrsrch_process_int_cmd_tf */


/*=========================================================================

FUNCTION    HDRSRCH_GET_CURRENT_STATE

DESCRIPTION
  Get the current state of the HDR SEARCH STATE MACHINE

DEPENDENCIES
  TBD

INPUTS
  None

RETURN VALUE
  Current state of the HDR SEARCH STATE MACHINE

SIDE EFFECTS
  

=========================================================================*/

hdrsrch_state_enum_type hdrsrch_get_current_state( void )
{
  return hdrsrchstate.next_state_id;
} /* hdrsrch_get_current_state */


/*=========================================================================

FUNCTION HDRSRCHSTATE_SET_REVB_MODE

DESCRIPTION
  Sets boolean Rev B mode.

DEPENDENCIES
  None

PARAMETERS
  is_revb   - TRUE if Rev B mode, FALSE otherwise.

RETURN VALUE
  None

SIDE EFFECTS
  System lost

=========================================================================*/

void hdrsrchstate_set_revb_mode
(
  boolean         is_revb
    /* TRUE to set Rev B mode */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchstate.is_revb_mode = is_revb;

  HDR_MSG_SRCH_1( MSG_LEGACY_MED,"hdr srch state revb mode(%d) - 1==RevB",
                  is_revb );

} /* hdrsrchstate_set_revb_mode */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSTATE_IS_REVB_MODE

DESCRIPTION
  Returns whether srch is in REVB mode or not.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - When REVB mode is enabled
  FALSE - When not in REVB mode

SIDE EFFECTS
  System lost

=========================================================================*/

boolean hdrsrchstate_is_revb_mode( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrsrchstate.is_revb_mode;

} /* hdrsrchstate_is_revb_mode */


/*=========================================================================

FUNCTION    HDRSRCH_DEFAULT_PROCESS_LOW_PRI_RSP_IND

DESCRIPTION
  Process the low priority responses and indications.

DEPENDENCIES
  TBD

INPUTS
  msg - Message pointer

RETURN VALUE
  None

SIDE EFFECTS
  

=========================================================================*/

void hdrsrch_default_process_low_pri_rsp_ind
(
   hdrsrchmsg_s_type         *msg
    /* Message */
)
{
#ifdef FEATURE_HDR_MSGR_VARIANT 
	  sys_modem_as_id_e_type variant;
	  /* Indicates which subscription should receive the message */
#endif  /* FEATURE_HDR_MSGR_VARIANT */
/*-----------------------------------------------------------------------*/

  if ( msg == NULL ) 
  {
    return;
  }

  /* Process the message */
  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Rcvd. FW msg %s umid 0x%8x", 
                  hdrsrchmsg_get_msg_name( msg->msg.hdr.id ),
                  msg->msg.hdr.id );

#ifdef FEATURE_HDR_MSGR_VARIANT 
  variant = MSGR_GET_VARIANT(&msg->msg.hdr);
	
  MSGR_CLEAR_VARIANT( msg->msg.hdr.id);
	
#endif /* FEATURE_HDR_MSGR_VARIANT */

  switch( msg->msg.hdr.id )
  {
    case HDR_FW_SRCH_FLL_ACQ_RSP:

      hdrsrchafc_acq_rsp( msg->msg.hdr_msg.rsp.fll_acq.fllLocked );
      break;

    case HDR_FW_SRCH_FING_ASSIGN_SLAM_RSP:
      hdrsrchmsg_process_rcvd_rsp( HDRSRCH_MSTR_SLAM_RSP );

      hdrsrchfing_process_slam_rsp( 
      msg->msg.hdr_msg.rsp.fing_assign_slam_rsp.slamSuccess );
        /* FW structure fing_assign_slam_rsp */

#ifdef FEATURE_HDR_RUMI
      if ( hdrhit_rumi_cfg.tx_only_support )
      {
        hdrhitmsg_process_slam_rsp( msg->msg.hdr_msg.rsp.srch_slam.hstrCount );
      }
#endif /* FEATURE_HDR_RUMI */
      break;
  
    case HDR_FW_SRCH_SLAM_RSP:
      hdrsrchmsg_process_rcvd_rsp( HDRSRCH_MSTR_SLAM_RSP );
  
        hdrsrchfing_process_slam_rsp( 
        msg->msg.hdr_msg.rsp.srch_slam.slamSuccess );
          /* FW structure srch_slam */
  
#ifdef FEATURE_HDR_RUMI
      if ( hdrhit_rumi_cfg.tx_only_support )
      {
        hdrhitmsg_process_slam_rsp( msg->msg.hdr_msg.rsp.srch_slam.hstrCount );
      }
#endif /* FEATURE_HDR_RUMI */
      break;

    case HDR_FW_SRCH_RESET_DRC_FILT_RSP:

      hdrsrchmsg_process_rcvd_rsp( HDRSRCH_DRCFILT_RESET_RSP );
      break;

    case HDR_FW_SRCH_FLL_ACCUM_LD_RSP:

      hdrsrchmsg_process_rcvd_rsp( HDRSRCH_ACCUM_LOAD_RSP );
      break;

    case HDR_FW_BEST_ASP_CHANGE_IND:

      hdrsrchset_process_asp_change_ind( 
        msg->msg.hdr_msg.ind.best_asp_change.bestAspChangeMask );
      break;

    case HDR_FW_RESET_REQ_IND:

      hdrsrchutil_system_lost( HDRSRCH_SYSLOST_FW_RESET_REQ );
      break;

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
    case HDRSRCH_LTE_IRAT_MEAS_D2L_DEINIT_CNF: 

      /* Only process messages meant for the current subscription */
      if(variant != hdrsrch_get_subscriber_id())
      {
         HDR_MSG_SRCH_3 (MSG_LEGACY_ERROR,
                         "Unexpected HDRSRCH_LTE_IRAT_MEAS_D2L_DEINIT_CNF %x received on %d variant curr_sub_id %d",
                          msg->msg.hdr.id,variant,hdrsrch_get_subscriber_id() );		  
      }
      else
      {
          /* Receive LTE Deinit confirmation message via message router */
          ( void ) hdrsrchtclte_process_deinit_cnf_msg();

          HDR_MSG_SRCH_1(MSG_LEGACY_MED, 
          "DRL Receives LTE Deinit CNF in state %d", hdrsrch_state_id );
      }
      break;
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
    case HDRSRCHMSG_MCS_CXM_FREQID_LIST_IND: 

      /* Receive freq id list from cxm via message router */
      hdrmultirat_process_freq_id_list( &msg->msg.cxm_msg.freqid_msg );

      HDR_MSG_SRCH_1( 
        MSG_LEGACY_MED, 
        "Received freq id list in state %d", hdrsrch_state_id );

      break;
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

#ifdef FEATURE_HDR_BAND_AVOIDANCE
    case HDRSRCHMSG_MCS_BAND_AVOID_BLIST_IND: 

      /* Receive freq id list from cxm via message router */
      hdrmultirat_process_band_avoid_ind( );

      HDR_MSG_SRCH_1( 
        MSG_LEGACY_MED, 
        "Received band avoidance ind in state %d", hdrsrch_state_id );
      break;
#endif /* FEATURE_HDR_BAND_AVOIDANCE */


    default:
      HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, " msg %x ignored", msg->msg.hdr.id);
      break;
  } /* switch */

} /* hdrsrch_default_process_low_pri_rsp_ind() */


/*===========================================================================

FUNCTION HDRSRCH_TIMED_WAIT_FOR_SIG

DESCRIPTION
  This function waits for a signal within a certain amount of time specified 
  in the parameter. If it cannot receive the requested signals before timing out,
  it will fatal error.

DEPENDENCIES
  None

PARAMETERS
  sig        - The signal to wait
  ms_to_wait - The time to wait in micro seconds.
               If it's negative value, the function will infinetely wait for 
               the signal.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrch_timed_wait_for_sig
(
  rex_sigs_type            sig,
    /* The signal to wait */

  int32                    ms_to_wait
    /* Time to wait in milli seconds */
)
{
  rex_sigs_type            sig_rcvd = 0;
    /* The signal received */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
  (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_TIMED_WAIT_SIG );
    /* Clear the timer signal */

  hdrsrch_add_to_wait_mask( HDRSRCH_TIMED_WAIT_SIG );
    /* update wait mask in HDRSRCH task to reflect that the task is
       currently waiting only on HDRSRCH_TIMED_WAIT_SIG */

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "TIMED_WAIT_SIG debug hdrsrch_timed_wait_for_sig(sig=0x%x,ms=%d)",
                  sig, ms_to_wait );

  if ( ms_to_wait >= 0 )
  {
    (void) rex_set_timer( &hdrsrch.gp_timer2, ms_to_wait );
      /* Start the timer for maxium wait */
  }

  do
  {
    sig_rcvd = rex_wait( sig | HDRSRCH_TIMED_WAIT_SIG | HDRSRCH_DOG_HB_SIG );
      /* Wait for one of the signals to be set */


    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "sig_rcvd = 0x%x", sig_rcvd );

    if ( ( sig_rcvd & HDRSRCH_TIMED_WAIT_SIG ) && ( ( sig_rcvd & sig ) == 0 ) )
    {
#ifndef FEATURE_HDR_DIME_MODEM
      /* Debug: for tramp interrupt trace */
      hdrsrch_interrupt_trace.interrupt_cdma_mod_tx_frame_is_set = 
        tramp_is_interrupt_set(TRAMP_CDMA_MOD_TX_FRAME_IRQ);
      hdrsrch_interrupt_trace.interrupt_cdma_mod_tx_frame_is_pending = 
        tramp_is_interrupt_pending(TRAMP_CDMA_MOD_TX_FRAME_IRQ);
      hdrsrch_interrupt_trace.interrupt_cdma_mod_tx_frame_hdr_is_set = 
        tramp_is_interrupt_set(TRAMP_CDMA_MOD_TX_FRAME_HDR_IRQ);
      hdrsrch_interrupt_trace.interrupt_cdma_mod_tx_frame_hdr_is_pending = 
        tramp_is_interrupt_pending(TRAMP_CDMA_MOD_TX_FRAME_HDR_IRQ);
#endif /* FEATURE_HDR_DIME_MODEM */

      HDR_MSG_SRCH_2( MSG_LEGACY_FATAL, 
        "cdma_mod_tx_frame: set = %u, pending = %d",
        hdrsrch_interrupt_trace.interrupt_cdma_mod_tx_frame_is_set, 
        hdrsrch_interrupt_trace.interrupt_cdma_mod_tx_frame_is_pending );

      HDR_MSG_SRCH_2( MSG_LEGACY_FATAL, 
        "cdma_mod_tx_frame_hdr: set = %u, pending = %d",
        hdrsrch_interrupt_trace.interrupt_cdma_mod_tx_frame_hdr_is_set,
        hdrsrch_interrupt_trace.interrupt_cdma_mod_tx_frame_hdr_is_pending );

      ERR_FATAL( "The requested signal 0x%x is not rcvd in %d ms!",
                 sig, ms_to_wait, 0 );
    }
    else if ( sig_rcvd & HDRSRCH_DOG_HB_SIG )
    {

      hdrsrch_process_wdog();
        /* Report wdog */
    }
  } while ( ( sig & sig_rcvd ) == 0 );

  hdrsrch_process_wdog();
    /* Pet the watchdog one more time on the way out */

  (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_TIMED_WAIT_SIG | sig );
    /* Clear the requested signals */

  (void) rex_clr_timer( &hdrsrch.gp_timer2 );
    /* Clear the timer */

} /* hdrsrch_timed_wait_for_sig */


/*===========================================================================

FUNCTION HDRSRCH_TIMED_WAIT_WITH_SIG_STATUS

DESCRIPTION
  This function waits for a signal within a certain amount of time specified 
  in the parameter. The time left in ms is returned after the wait.

DEPENDENCIES
  None

PARAMETERS
  sig        - The signal to wait
  ms_to_wait - The time to wait in micro seconds.
               If it's negative value, the function will infinetely wait for 
               the signal.

RETURN VALUE
  Time left in ms if SIG is received, or 0 if timer has expired.

SIDE EFFECTS
  None

===========================================================================*/

rex_timer_cnt_type hdrsrch_timed_wait_with_sig_status
(
  rex_sigs_type            sig,
    /* The signal to wait */

  rex_timer_cnt_type       ms_to_wait
    /* Time to wait in milli seconds */
)
{
  rex_sigs_type            sig_rcvd = 0;
    /* The signal received */

  rex_timer_cnt_type       time_left;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
  (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_TIMED_WAIT_SIG );
    /* Clear the timer signal */

  hdrsrch_add_to_wait_mask( HDRSRCH_TIMED_WAIT_SIG );
    /* update wait mask in HDRSRCH task to reflect that the task is
       currently waiting only on HDRSRCH_TIMED_WAIT_SIG */

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "TIMED_WAIT_SIG debug hdrsrch_timed_wait_with_sig_status(sig=0x%x,ms=%d)",
                  sig, ms_to_wait );

  (void) rex_set_timer( &hdrsrch.gp_timer2, ms_to_wait );
    /* Start the timer for maxium wait */

  do
  {
    sig_rcvd = rex_wait( sig | HDRSRCH_TIMED_WAIT_SIG | HDRSRCH_WDOG_SIG );
      /* Wait for one of the signals to be set */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "sig_rcvd = 0x%x", sig_rcvd );

    if ( ( sig_rcvd & HDRSRCH_TIMED_WAIT_SIG ) && ( ( sig_rcvd & sig ) == 0 ) )
    {
      HDR_MSG_SRCH_2( MSG_LEGACY_FATAL, 
        "The requested signal 0x%x is not rcvd in %d ms!",
        sig, ms_to_wait);

      break;
    }
    else if ( sig_rcvd & HDRSRCH_WDOG_SIG )
    {
      HDRSRCH_SIG_CLR( HDRSRCH_WDOG_SIG );
        /* Clear the WDOG signal for HDRSRCH */

      hdrsrch_process_wdog();
        /* Report wdog */
    }
  } while ( ( sig & sig_rcvd ) == 0 );

  time_left = rex_get_timer( &hdrsrch.gp_timer2 );

  dog_hb_report( DOG_HDRSRCH_RPT );
    /* Pet the watchdog one more time on the way out */

  (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_TIMED_WAIT_SIG | sig );
    /* Clear the requested signals */

  (void) rex_clr_timer( &hdrsrch.gp_timer2 );
    /* Clear the timer */
  HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                  "wait for sig time left:%d", 
                  time_left );

  return time_left;

} /* hdrsrch_timed_wait_with_sig_status */


/*===========================================================================

FUNCTION HDRSRCH_INIT_BEFORE_TASK_START

DESCRIPTION
  This function takes care of all intitalizations needed before hdrsrch task
  start. Called only in RCINIT framework.

DEPENDENCIES
  RCINIT framework should be supported.

PARAMETERS
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrch_init_before_task_start()
{
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
    hdrsrch_create_efs_item_conf_file();
    /* Creat EFS configure file */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */
 
   hdrmultirat_create_efs_item_conf_file();
    /* Creat EFS configure file */

    hdrsrchmcpm_init();
    /* Initialize SRCH-MCPM */

    MCPM_NPA_Set_Sleep_Timeline_Callback( MCPM_DO_TECH, hdrsrch_change_sleep_timeline );
      /* Set the callback for optimzied timeline */

    hdrsrchcmd_init();
    /* Initialize command engine */

    hdrsrchutil_init( );
    /* initialize HDR Search utilities */

    hdrsrchsync_thresh_init();
    /* Sync state C/I thresh etc related initialization */

    hdrsrchlog_init( );
    /* initialize HDR Search Log packets    */

#ifdef FEATURE_HDR_BCMCS
    hdrsrchbc_set_bc_status( BCMCS_DISABLED );
    /* initialize BCMCS status to FALSE     */

    hdrsrchsleep_set_bcmcs_page_cycle( 0 );
    /* initialize BCMCS page cycle value    */

    hdrsrchsleep_set_bcmcs_sleep_cycle( 0 );
    /* initialize BCMCS sleep cycle value    */
#endif /* FEATURE_HDR_BCMCS */

    hdrsrchset_init();
    /* Initialize sets */


#ifdef FEATURE_HDR_REVB
    hdrsrchschdgrp_init();
#endif /* FEATURE_HDR_REVB */

    hdrsrchsm_init();
      /* Initialize state machine */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
   
    hdrsrchutil_reset_prev_wakeup_info();
    /* Reset all previous wakeup info to defaults. */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

    hdrsrch_dal_init( );
      /* To check the chip info */

    /* Initialize hdrsrch Critical Section*/
    rex_init_crit_sect( &hdrsrch.crit_sect );

      /* intialize time services */
    hdrts_init();

    hdrsrcheq_init( );
      /* Reassign all equalizers */

} /* hdrsrch_init_before_task_start */

#ifdef FEATURE_HDR_RUMI
/*===========================================================================

FUNCTION HDRSRCHSTATE_SRAM_CHECK

DESCRIPTION
  This function starts/stops SRAM check.

DEPENDENCIES
  Must only start after RXLM initialization (RX_START)

PARAMETERS
  start -  Whether to start or stop SRAM check

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchstate_sram_check
(
  boolean start
)
{
  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "SRAM START %d", 
                  start );  

  if ( start && hdrhit_rumi_cfg.sram_check )
  {
    /* call cb immediately to get a reading. cb will start timer */
    hdrsrchstate_sram_check_cb( 0 );
  }
  else
  {
    rex_clr_timer( &hdrsrchstate.sram_stuck_timer );
  }
}
#endif /* FEATURE_HDR_BOLT_RUMI */

/*============================================================================

FUNCTION HDRSRCH_IS_IN_SUSPEND

DESCRIPTION
  This function provide a boolean value indicating if HDR is in suspend
  states.  Currently, there are 3 suspend states as listed below.
  
DEPENDENCIES
  None
 
PARAMETERS
  None

RETURN VALUE
  TRUE:  HDR is in:
         HDRSRCH_SUSPENDED_TC_STATE    
         HDRSRCH_SUSPENDED_IDLE_STATE
         HDRSRCH_SUSPENDED_SYNC_STATE
  FALSE: HDR is in another state
 
SIDE EFFECTS
  None
 
============================================================================*/
boolean hdrsrch_is_in_suspend( void )
{
  return ( ( hdrsrch_get_current_state() == HDRSRCH_SUSPENDED_TC_STATE ) ||
           ( hdrsrch_get_current_state() == HDRSRCH_SUSPENDED_IDLE_STATE ) ||
           ( hdrsrch_get_current_state() == HDRSRCH_SUSPENDED_SYNC_STATE ) );
} /* hdrsrch_get_current_state */

/*============================================================================

FUNCTION HDRSRCH_DBG_WAIT_MASK

DESCRIPTION
  This function will crash if wait_mask or the gaurds is corrupted
  
DEPENDENCIES
  None
 
PARAMETERS
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None
 
============================================================================*/
void hdrsrch_dbg_wait_mask( void )
{
  if ( ( ( hdrsrchstate.wait_mask & HDRSRCH_INT_CMD_SIG ) == 0 ) ||
       ( hdrsrchstate.guard1 != 0xDEADBEEF ) ||
       ( hdrsrchstate.guard2 != 0xDEADBEEF ) )
  {
    ERR_FATAL( "DBG: guard or waitmask corrupted:%08x:%08x:%08x",
               hdrsrchstate.guard1, hdrsrchstate.wait_mask, hdrsrchstate.guard2 );
  }
}
