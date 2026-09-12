/*====*====*===*====*====*====*====*====*====*====*====*====*====*====*====*
               M U L T I M O D E   C O N T R O L L E R ( MMoC )

                 I M P L E M E N T A T I O N    F I L E

GENERAL DESCRIPTION
  This module is the main implementation for the Multi-Mode Controller
  (MMoC) task. The MMoC is responsible for facilitating the switch between
  different modes, initializing the system determination (SD) module and
  interfacing with SD and various protocols to handle the operating mode
  change, subscription change and other commands. It also provides a task
  context for SD reselection timer.

  The MMoC task has 2 seperate message queues, command queue for messages
  from CM and report queue for messages from the various protocols.

EXTERNALIZED FUNCTIONS

  Command Interface:
    mmoc_cmd_subscription_chgd
    mmoc_cmd_oprt_mode_chgd
    mmoc_cmd_pref_sys_chgd
    mmoc_cmd_get_networks_gw
    mmoc_cmd_get_term_networks_gw
    mmoc_cmd_term_get_networks_wlan
    mmoc_cmd_term_get_networks_wlan
    mmoc_cmd_wakeup_from_pwr_save

  Report Interface:
    mmoc_rpt_prot_deactd_cnf
    mmoc_rpt_prot_auto_deactd_ind
    mmoc_rpt_ph_stat_chgd_cnf
    mmoc_rpt_prot_gen_cmd_cnf

  Functional Interface:
    mmoc_proc_prot_gen_cmd

  Others:
    mmoc_task

REGIONAL FUNCTIONS
  None.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  The MMoC task should be started before MMoC APIs are called.


Copyright (c) 2002 - 2013 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/mmoc/src/mmoc.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/31/14   rm      Adding debug buffers
01/15/14   jvo     Fix compilation warning in
                   mmoc_process_subsc_chgd_ph_stat_enter()
12/20/13   jvo     Changed failure to queue message router message to
                   ERR_FATAL in mmoc_msgr_send_else_err_fatal()
10/04/13   jvo     MMoC always calls MMOC_ERR_FATAL after sanity timeout in
                   mmoc_print_sanity_err_fatal()
09/05/13   jvo     Replaced calls to MMOC_PRINT_MSG_HIGH with mmocdbg
                   functions
08/28/13   jvo     Cleanup remaining Fusion pieces
07/19/13   jvo     Remove all MDM/MSM Fusion features
07/21/13   skk     FR2921: Network initiated Authentication Procedure Support
                   during emergency call When SIM is perso Locked
07/15/13   ss      FR1860 - T/G on DSDS
06/19/13   gm      Send pref to SD-ALL stacks before ONLINE/PWRUP ind
06/14/13   gm      EFRAT ind should be sent on subscription change event
06/13/13   jqi     Add of TTL markers to prfile the boot up time.
06/11/13   dk      replace ERR_FATAL_REPORT with ERR_FATAL
05/22/13   qf      Handle auto_deact_ind while in lte->do redir trans
05/15/13   jqi     Memory optimization for suspend command processing.
03/15/13   gm      Do not register for STOP_SIG with RCINIT
03/15/13   gm      UT fix for sending pref only on MAIN at ONLINE
03/09/13   aj      Allow FTM reactivation after subsc processing
02/27/13   ss      Using New Watchdog Heartbeat Interface instead of the old Watchdog
02/21/13   cl      Re-initialize SD core during power-down event
01/25/13   gm      Avoid duplicate initialization of SD
02/03/13   cl      FR1975: Dynamic Switch SVLTE <-> CSFB
01/25/13   gm      FR 3059 - ONLINE optimization - skip duplicate subscription
09/27/12   vm      Removing DIME featurization and cleanup
09/20/12   vm      Check if MMOC is already in suspend during suspend processing
09/12/12   ag/mg   Update service state in CM to NO SRV when LTE gets deactivated by MMOC
08/08/12   vm     Removing processing for rf initialization as cm handles it
05/30/12   ab      MMOC is recording abort from HDRCP when MMOC is in
                   MMOC_TRANS_STATE_WAIT_AUTO_DEACTD_IND
05/18/12   xs      Reset redir sanity timer
05/10/12   cl      Send resume report to CM regardless the HDR stack state
05/02/12   rk      Moving the funxtionality of setting SD context to 1XCP
                   upon HO_IND to MMOC
05/01/12   gm      Add offline notify send from MMOC
04/20/12   sk      Fixing compiler warnings
04/17/12   cl      Remove software-switch-9 usage on hybr-2 stacks
04/17/12   xs      Convert MMOC BSR timer to ms in LTE->DO redir
04/17/12   sk      Donot allow operating mode switches after PWR_OFF operating mode command is processed.
04/16/12   gm      Add new init before task for RCINIT.
11/11/11   fj      Fix KW errors.
03/28/12   xs      Extend MMOC timer to BSR timer in LTE->DO redir
03/24/12   gm      DIME integration support - fix to compilation errors
03/21/12   ab      Added support for handling Auto deact ind
                   whem MMOC is waiting for open session confirm
02/15/12   vk      Replace usage of AEE library function with corresponding
                   CoreBSP library
11/30/11   jh      Added feature for dynamic rat acq order change
02/12/12   aj      send suspend ind to CM even for duplicate requests
01/17/12   aj/cl   SC SVLTE SO 33 changes
01/11/12   xs      Remove MMOC_TRANS_PWR_SAVE_EXIT transaction state
01/11/12   aj      Call sd_init only once during subsc chgd transaction
01/10/12   gm      Mainline HDR API changes integration
12/15/11   sg      Add TDS band pref to mmoc_cmd_get_networks_gw().
11/02/11   ak      Enable manual PLMN search.
11/02/11   gm      Changes for HDR API cleanup
10/31/11   ak      Recover from miscommunication between HDRCP and MMOC.
10/21/11   aj      update HYBR2 stack to ONLINE state when oprt mode is ONLINE
09/14/11   vk      New MMOC transaction "MMOC_TRANS_DEACT_1XCSFB_CMD" related changes.
08/27/11   aj      Clear LTE srv status during LTE-DO redir
08/19/11   fc      Fix featurization issue.
08/10/11   rn      Handle DO redire abort during offline/power down
06/10/11   rn      Qshrink2 changes
08/10/11   aj      clear MMOC transaction on getting IRAT in non-ONLINE mode
08/11/11   xs      Remove rflib.h
08/04/11   xs      Replace FEATURE_MMODE_NIKEL with FEATURE_MMODE_SC_SVLTE
08/02/11   aj      update sanity expiry debug messages for GWL deact cnf state
07/22/11   xs      MMOC SC_SVLTE 1.0 merging to Mainline
07/13/11   nk      Updated stubs in UT environment to be in sync with QTF.
07/06/11   aj      Enable postprocessing for HDR to LTE resel
06/10/11   xs      Change order to deactivate active protocol first
05/20/11   ak      Updating the buffering mechanism in MMOC to kick blocked
                   RAT when permission rcvd in SVLTE II.
05/16/11   xs      Added another stop mode rquest to deactivate NAS in suspend state
05/14/11   rk      Passing user mode preference from CM to SD to update RAT
                   priority list
05/12/11   mp      BPLMN - LTE Only capable is not able to perform Manual Search
05/04/11   mp      Enhanced MMOC sanity timer expiry function for GWL.
05/04/11   rk      Passing CDMA time related info to SD during handover from
                   LTE to CDMA
05/04/11   rk      revert change for IRAT indication to domain selection
05/03/11   rk      Indicate about redirection to domain selection on getting
                   the redirection indication itself
04/25/11   sv      Remove including test_mc.h
04/14/11   cl      MSM Crash due to prot-state mis-sycnh when 1X service lost
04/09/11   cl/ak   Current state from the mmoc_info_ptr instead of gen_cmd.
04/05/11   sv      Ind SD to unlock CDMA, if locked, when going ONLINE
04/04/11   ak      DIfferentiate hdr_acq_blocked for HICPS and MMSS
04/01/11   xs      After move to MMOC_TRANS_STATE_WAIT_PS_DETACH_CNF
                   MMOC transaction should return MMOC_EVT_STATUS_CONSUMED
03/25/11   xs      Extend MMOC sanity timer to 82 sec for PS detach operation
03/09/11   cl      Add support for SVLTE2 MMSS arbitration
02/28/11   aj      Add wait for activation_cnf state in DO to LTE reselection
02/23/11   rk      Passing correct sid and nid received in HO complete ind
02/02/11   rm      Adding LTE RAT-B changes
02/08/11   aj      Update SD regarding HDR deactivation due to GWL acq
01/13/11   aj      send correct LTE band pref to SD after 1x subscription chgd
01/11/11   am      MUTE migration to qtf changes
01/05/10   rk      Send CM_LTE_1X_IRAT_F report to CM before entering into
                   protocol deactivatio transaction state
12/22/10   rm      Fixing lint errors
12/10/10   rk      MO/MT 1XCSFB call successful scenarios support
12/02/10   gm      Fix to MOB compiler warnings.
11/30/10   aj      Revert change to inform SD about HDR deactivation
11/24/10   xs      Fusion featurization clean up
11/29/10   rm      Added missing 'break' in wake-up scenario
11/15/10   rm      Memory optimization changes
11/10/10   am      Changes for MUTE to MOB migration
11/06/10   aj      DO -> LTE reselection code
11/05/10   sv/gm   Changes to wait for session open confirmation
10/27/10   rk      Reverting support for deativating dormant protocol
10/28/10   gm      Fix to compiler warning.
10/27/10   rk      Adding support for deativating dormant protocol through MMOC
10/15/10   ak      Featurizing the call to err_reset_modem API.
10/08/10   ak      Disable error handling on reset in MSM.
10/05/10   pm      Remove clk.h & FEATURE_USE_TIME_VU
10/01/10   ak      Merged the Fusion code from Dev branch.
09/27/10   xs      Fixed target compilation errors
09/22/10   aj      Update SD regarding HDR deactivation due to GWL acq
09/17/10   cl      Fixing lint error
09/17/10   cl      Adding support for dynamic watch dog
08/02/10   nm      Removed EXTERN
07/14/10   mh      Fixed compile warnings
07/06/10   gm      Fix to Klocwork deferred critical errors.
07/05/10   sg      CM should send SIM_AVAILBLE_REQ to NAS, even if UE goes to
                   power_save mode during subscription change.
07/08/10   sv      Added mmoc_cmd_msm_sys_info_chgd handling
06/10/10   ak      Mainlining memory optimization changes.
06/10/10   cl/ak   Update MMOC state machine to ignore postprocessing when receiving
                   autodeactivate while waiting for gen_cmd_cnf
05/24/10   mp      Added support for session close and
                   MMOC_TRANS_MMGSDI_INFO_IND transaction to avoid nested callbacks
05/04/10   mh      Removed RF device from RF API
04/27/10   rn      Send CM_LTE_DO_IRAT_F to CM to indicate LTE->eHRPD IRAT
03/21/10   mp      MLPL/MSPL/PLMN Interface for MMSS 3.1 support
03/19/10   am      Fixing Compilation Warning
02/11/10   rn      LTE-1x redirection/reselection
01/28/10   mp      EF_EPRL Implementation
01/19/10   rn      Updated code for LTE-eHRPD redirection
01/12/10   mh      Fixed memory leak related to power save
01/06/10   ay      LTE support : subscription for GW
12/31/09   ay      Added LTE support
12/28/09   rn      LTE-eHRPD Redirection support
12/08/09   fj      Added lte_band_pref.
12/04/09   sg      Added a counter "mmoc_modem_heap_blocks" to keep track of memory
                   buf allocated through new API modem_mem_alloc()/modem_mem_free().
12/02/09   mh      Added LTE support when FEATURE_UIM_RUIM is defined
11/09/09   sg      Add non-demand paging macro to function that are not to be
                   demand-paged10/30/09   sv      Updating RF API as part of CMI change.
10/21/09   mh      Added code for GW/LTE Support
10/13/09   sg      ZI memory reduction. Mmode should call new Heap Manager API to
                   allocate memory dynamically.
07/30/09   fj      Removed FEATURE_LTE_PENDING for 2G/3G UT environment.
07/13/09   np      Added code for LTE support
08/21/09   rm      Including uim_v.h
08/14/09   rm      Making bio.h related changes
05/19/09   rm      Mainlining of FEATURE_DIAG_NON_STREAMING
05/05/09   aj      When MMOC and protocol are not in sync,print error
                   instead of crashing.
04/13/09   aj      Change behavior to process auto deact rpt while waiting for
                   ph stat chgd cnf or deact_cnf
03/16/09   rm      Removed inclusion of hdrmc.h. Added hdrmc_v.h.
02/25/09   sv      Removing inclusion of cmxll_v.h
02/23/09   rm      Removing inclusion of cmxll.h and sd.h
                   Including mc_v.h
02/06/09   aj      WLAN subscription decoupling
02/05/09   aj      Modify mmoc_is_hdr_acq_allowed() to allow hdr acq in DED_MEAS mode
12/22/08   rn      Added support for TMC memory utilization
10/17/08   sv      Corrected featurization MMOC_LOW_MEM_TARGET to
                   FEATURE_MMOC_LOW_MEM_TARGET.
10/09/08   fj      Fixed Klocwork defects
09/19/08   jd      Memory reduction changes
09/12/08   fj/aj   Added support for Modem Reset
09/04/08   sv      Implemented WPS support for WLAN
09/05/08   ak      Fixing compiler warnings.
08/25/08   ak      Fixing compiler warnings.
08/20/08   st      Resolve Klockwork defects
07/30/08   sv      Added FEATURE_HICPS_STACK_SYNC_UP to buffer the dropped
                   HDR acq
06/16/08   sv      Fixed Klocwork defects
04/28/08   fj      Lint cleanup, CR 144389
03/22/08   ic      Undo of "Replace custmmode.h with modem_mmode_cust.h" and
                   "FEATURE_MMODE_CUSTMMODE_H with FEATURE_MODEM_MMODE_CUST_H"
01/31/08   ic      Replace custmmode.h with custmmode.h
                   Replace FEATURE_MMODE_CUSTMMODE_H with FEATURE_CUSTMMODE_H
01/31/08   cl      Added in FEATURE_MMOC_DETECT_HDR_HW to support hardware
                   run-time detection.
09/11/07   pk      Added support for FEATURE_DISABLE_DEEP_SLEEP
08/20/07   ic      Lint cleanup
04/16/07   rp      Corrected handling of FTM mode
03/19/07   ka      Replacing banned apis
01/31/07   ka      Adding new state in MMOC to wait for TMC to ack operating
                   mode change.
01/04/07   pk      Added support for VoIP -> 1X Handoff
11/30/06   rp      Added support for background BPLMN search in limited service.
11/15/06   sk      Added hybrid GW support.
08/29/06   sk      Added WLAN Adhoc support.
04/24/06   ka      Featurizing ftm.h under GW flags.
04/03/06   ka      Changing copyright.
12/30/05   ka      Adding string maps to mmoc commands, reports and state
                   transitions.
11/28/05   ka      Fixing klockwork errors.
07/27/05   ka      Fixing implementation of common_hdr_deactd_cnf()
07/21/05   ka      Adding MMOC_DUP_CMD_REM_SIG to the list of signals
                   that mmoc task waits for.
07/07/05   ka      Request to activate GW on main when HDR is online in
                   hybrid instance proceeds with request to deactivate hdr.
                   On hdr deactivate cnf GW acquisition gets sent.
06/27/05   ic      Fixed RVDS 2.1 compiler warnings
06/17/05   ka      Rejecting Generic cmd to activate HDR hybrid instance
                   when GW is on Main instance.
                   Fixing issue where sd_ss_act_get() was called for Auto
                   deactivation indication from HDR hybrid instance resulting
                   in HDR hybrid staying online (sanity error fatal).
03/28/05   ka      Correcting Lint errors.
03/02/05   ka      Handling subscription available along with protocol name
                   that has subscription available.
02/19/05   dk      Fixed MMOC Pref_sys_cmd processing. MMOC does not queue
                   pref_sys_chgd_cmd for WLAN SS if WLAN feature is not
                   enabled.
01/12/05   sj      Fixed issue of mmoc activating ss-main after reset
                   because of SS timer expiry after processing RESET cmd.
12/21/04   sj      Fixed lint errors.
12/03/04   sj      Updated to issue standby sleep only after all instances
                   enter pwr_save.
09/27/04   ajn     Created an mmoc_timer_group for MMOC timers.
08/20/04   sj      Added support for prl_pref.
08/13/04   sj      Do not err_fatal if online command is received after
                   poweroff.
08/06/04   sj      Converted calls to _true_curr_act to sd_ss_act_get().
08/04/04   sj      For subsc_changed or sd_inited case, call user_pwr_up ind.
07/29/04   sj      Do not reset sd_initialized flag after LPM or FTM.
06/21/04   jqi     Use sd nam selection indication when there is a
                   subscription change.
06/07/04   sj      Send CM_STANDBY_SLEEP_F report upon entering power save.
05/25/04   jqi     Fixed mode preference reset issue after power up.
05/10/04   sj      Merged in changes for srch inactive state.
04/22/04   sj      Fixed issue of not starting pwr_save timer when next act
                   is also PWR_SAVE for SS-Main.
03/18/04   jqi     Used the true act type from SD System selection.
                   Replaced diag_enter_panic_mode with
                   ERR_FATAL_REPORT().
03/28/04   sj      Do not move srch to START upon getting ONLINE in LPM.
03/12/04   sj      Do not place srch in standby if entering pwr save from
                   HDR.
03/01/04   sj      Changed the order of deactivating protocols, SS_HDR first
                   followed by SS_MAIN.
02/24/04   sj      Removed LINT warnings.
01/13/03   sj      Merged in duplicate pref_sys_chgd cmd detection code.
                   Send HDR unlock RF command if 1x doesn't respond for
                   sometime.
                   Fixed sanity err_fatal when trans_state is AUTO_DEACT_IND.
10/22/03   sj      Send GPS_MODE_EXIT_F to CM before activating other
                   protocols.
09/15/03   sj      Fixed issues with HDR <-> 1x runtime switching.
08/02/03   dyc     Support for FEATURE_MULTI_RX_CHAIN
07/20/03   sj      Added hybrid support.
06/05/03   ht      Removed extra comma
07/08/03   sj      Changes for thin_ui support.
06/18/03   sj/ck   Vote off UIM power ctl after processing subscription
                   available.
05/14/03   sj      Fixed compiler error when FEATURE_FACTORY_TESTMODE is
                   not defined.
04/24/03   sj      Added support for early power scan.
03/18/03   sj      Process online even if subscription is not present.
03/18/03   lcl     Added reference to zrf library.
03/10/03   SJ      Added support for runtime FTM in leopard.
03/06/03   JQ/SJ   Moving phone to offline if rf init fails.
03/05/03   SJ      Fixed bug dring handling reset command wrt SD term.
02/26/03   SJ      Modified sanity timeout err_fatal to print the offending
                   protocol task's name instead of mmoc.c.
02/24/03   SJ      Handling GET_NET_GW in activate_prot_online().
02/24/03   SJ      Sending ONLINE command to TMC during online processing.
02/21/03   SJ      Calling sd_ss_ind_user_pwr_down() before powering down
                   the phone.
02/19/03   SJ      Handling subsc_chgd command only in powerup or online.
02/14/03   SJ      Added new MMOC API for wakeup from power save.
02/11/03   SJ/JQI  Added support for runtime FTM.
02/05/03   PK      Fixed compiler problem (missing end comment terminator).
02/05/03   SJ      Temporarily turning off rfr & rfl before activating GW.
01/23/03   SJ      Calling nam_sel SD indication only if the subsc_chgd CM
                   cmd was received, otherwise invoke pwr_up SD indication.
01/16/03   SJ      Updated _send_cm_rpt() to use cm_sd_rpt_u_type always.
01/16/03   SJ      Featurized DB accesses for non 1x targets.
12/19/02   SJ      Fixed activate_prot_offline for targets not supporting
                   CDMA or AMPS.
12/18/02   SJ      Added more debug messages.
12/06/02   SJ      Added fix to move protocol to same state after processing
                   subscription changed command for FTM/OFFLINE oprt mode.
11/22/02   SJ      Modified call to SD user_ss_pref -> user_ss_pref2().
09/17/02   SJ      Updated to send CM report on completion of LPM processing.
07/24/02   SJ      Cleared rex signal before processing event.
06/10/02   SJ      Cleared rex signal after processing event.
06/07/02   SJ      Renamed PROT_TRANS_NONE_PENDING to PROT_TRANS_NONE.
03/28/02   SJ      Initial release.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "mmcp_variation.h"
#include "customer.h" /* Customer configuration file */
#ifndef FEATURE_CUSTMMODE_H
#error Need to pull in custmmode.h!!! /* Make sure custmmode.h is pulled in */
#endif

#include "comdef.h"   /* Definition for basic types and macros             */
#include "msg.h"      /* Message output services.                          */
#include "cmd.h"      /* Command header for inter-task communication.      */
#include <stdio.h>    /* For calling sprintf.                               */

#include "sd_v.h"
#include "prot.h"     /* Common header for mmoc <-> protocol communications*/
#include "prot_v.h"
#include "mmoci.h"    /* Internal interface definitions for MMoC.          */
#include "mmoc.h"     /* External interface definitions for MMoC.          */
#ifndef FEATURE_MMOC_UNIT_TEST
#include "DDISoftwareSwitches.h" /* Interface for GPIO &other bit i/o services */
#include "DALSys.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"
#endif
#include "task.h"     /* Interface for global task resources.              */
#include "dog_hb_rex.h"    /*Interface for the new heartbeat watchdog services*/
#include "db.h"       /* Interface to database services.                   */

#if defined(MMOC_CDMA_SUPPORTED)
#include "cmxll.h"
#endif /* defined(MMOC_CDMA_SUPPORTED) */

#include "cm.h"       /* Interface to CM client service.                   */
#include "rex.h"
#include "cm_v.h"
#include "cm_i.h"
#include "sd_v.h"
#if defined(FEATURE_MMODE_ML_LOGGING)
#include "logging_ext.h"
#endif

#if defined(MMOC_CDMA_SUPPORTED)
#include "mc.h"       /* Interface for CDMA and AMPS protocols.            */
#include "mc_v.h"
#endif /* defined(MMOC_CDMA_SUPPORTED) */

#ifdef MMOC_HDR_SUPPORTED
#include "hdrcp_msg.h"
#include "hdrcp_api.h"
#endif /* defined(MMOC_HDR_SUPPORTED) */

#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
#include "cmregprx.h" /* Interface for RegProxy module.                    */
#include "mm.h"       /* Interface for MM module.                          */
#endif /* MMOC_GW_SUPPORTED || FEATURE_MMOC_LTE*/



#ifdef FEATURE_UIM
#include "uim.h"
#include "uim_v.h"
#endif

#include "nvruimi.h"

#include "event.h"    /* Interface for system event report service.        */
#include "modem_mem.h"   /* interface for modem_mem_alloc */

#include "rcinit.h"
#include "mcs_wrappers.h"  // MCS wrappers for 'critical' rcevt_* functions

#define MMOC_RCEVT_PREFIX "MMOC:"
#define MMOC_RCEVT_READY MMOC_RCEVT_PREFIX "ready"

#ifdef MMOC_FTM_SUPPORTED
#include "ftm.h"
#endif /* MMOC_FTM_SUPPORTED */


#if defined(RF_MULTIMODE) || defined(FEATURE_MULTIMODE_RFAPI)
#include "rfm.h"      /* Interface for multimode RF APIs                  */
#endif /* defined(RF_MULTIMODE) || defined(FEATURE_MULTIMODE_RFAPI)  */


#include "mmocdbg.h"  /* For mmocdbg_print_message support */

#ifdef FEATURE_MMOC_UNIT_TEST
#error code not present
#endif
#ifdef FEATURE_MODEM_CONFIG_REFRESH

#include "mcfg_refresh.h"
#endif
#ifndef FEATURE_MMOC_UNIT_TEST
static DalDeviceHandle  *switches_handle;
static DALResult         switches_dal_attach;
#endif /* FEATURE_MMOC_UNIT_TEST */

#ifdef MMODE_ADDITIONAL_DEBUG_INFO
static uint8           curr_rpt_idx = 0;
#endif

#include <string.h>

#include <stringl/stringl.h>


#ifdef TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_MMODE_QTF */

/* Header file Time Test Lite (TTL) framework for profiles modem boot up time
*/
#include "ttl_map.h"
#include "policyman.h"
#include "subs_prio.h"
#include "mmoc_qsh_ext.h"
#include "mm.h"
#include "lte_rrc_ext_msg.h"
#include "timer.h"

/* <EJECT> */
/*===========================================================================
=============================================================================
=============================================================================
============================== DEPENDENT FEATURES ===========================
=============================================================================
=============================================================================
===========================================================================*/

/* This set of features must be defined for Multi-mode operation.
*/
#if !defined(FEATURE_SD20)
#error FEATURE_SD20 must be defined.
#endif
/* <EJECT> */
/*===========================================================================
=============================================================================
=============================================================================
==================== MMoC Forward declarations ==============================
=============================================================================
=============================================================================
===========================================================================*/

static rex_sigs_type                   mmoc_wait(rex_sigs_type  mask);
static void                            mmoc_kick_dog (void);

/* Dog Variables - Handle for DOG HB interface */
#ifndef MMOC_DEBUG
#undef DOG_MMOC_RPT
#define DOG_MMOC_RPT mmoc_dog_rpt_var
static dog_report_type mmoc_dog_rpt_var             = 0;        /*Initial Safety*/
#endif

static void mmoc_reg_multimode_sanity_err_fatal( mmoc_sanity_err_e_type reason);
static void mmoc_hdr_multimode_sanity_err_fatal(mmoc_sanity_err_e_type reason);

static void set_mmoc_tcb(void);

#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
static boolean mmoc_is_hybr_2_acq_allowed
(
  const mmoc_state_info_s_type    *mmoc_info_ptr,
  /* Pointer to MMoC's state information */

  sys_modem_as_id_e_type           asubs_id
);
#endif

#ifdef MMOC_HDR_SUPPORTED
#ifdef MMOC_LTE_TO_HDR
static void mmoc_send_hdr_protocol_activate_cnf
(
  prot_act_e_type      act_reason
);
static void mmoc_send_hdr_redir_protocol_cnf
(
  prot_act_e_type act_reason,
  /* Activate reason for protocol */
  boolean         is_irat_ho_allowed
  /* Response to HDR MC if IRAT is allowed */
);
#endif
#endif

#if defined MMOC_HDR_SUPPORTED && defined FEATURE_MMODE_DUAL_SIM
static void mmoc_send_hdr_standby_pref_chgd_cmd
(
  prot_trans_type trans_id,
  sys_modem_dual_standby_pref_e_type ds_pref

);
#endif

boolean mmoc_is_standby_pref_and_active_subs_in_sync
(
  mmoc_state_info_s_type         *mmoc_info_ptr
);

#ifdef FEATURE_MMODE_DUAL_SIM
void  mmoc_hold_acq (
  sys_modem_as_id_e_type        asubs_id,
  mmoc_state_info_s_type        *mmoc_info_ptr
);

boolean  mmoc_delay_srv_req (
  sys_modem_as_id_e_type        hold_sub,
  mmoc_state_info_s_type        *mmoc_info_ptr
);

#endif

/* <EJECT> */
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/


/* List of modes for which the phone status changed command has to be sent.
*/
static mmoc_mode_info_s_type ph_stat_cmd_tbl[(int)MMOC_MODE_MAX] =
{
  { "MODE_NONE",      NULL,                                PROT_TRANS_NONE },

#ifdef MMOC_GW_SUPPORTED
  { "MODE_GWL_1",     mmoc_reg_multimode_sanity_err_fatal, PROT_TRANS_NONE },
#else
  { "MODE_GWL_1",     NULL,                                PROT_TRANS_NONE },
#endif

#ifdef MMOC_CDMA_SUPPORTED
  { "MODE_CDMA_1",    mc_cdma_multimode_sanity_err_fatal,  PROT_TRANS_NONE },
#else
  { "MODE_CDMA_1",    NULL,                                PROT_TRANS_NONE },
#endif

#ifdef MMOC_HDR_SUPPORTED
  { "MODE_HDR_1",     mmoc_hdr_multimode_sanity_err_fatal, PROT_TRANS_NONE },
#else
  { "MODE_HDR_1",     NULL,                                PROT_TRANS_NONE },
#endif

#ifdef FEATURE_MMODE_DUAL_SIM
#ifdef MMOC_GW_SUPPORTED
  { "MODE_GWL_2",     mmoc_reg_multimode_sanity_err_fatal, PROT_TRANS_NONE },
#else
  { "MODE_GWL_2",     NULL,                                PROT_TRANS_NONE },
#endif

#ifdef MMOC_CDMA_SUPPORTED
  { "MODE_CDMA_2",    mc_cdma_multimode_sanity_err_fatal,  PROT_TRANS_NONE },
#else
  { "MODE_CDMA_2",    NULL,                                PROT_TRANS_NONE },
#endif

#ifdef MMOC_HDR_SUPPORTED
  { "MODE_HDR_2",     mmoc_hdr_multimode_sanity_err_fatal, PROT_TRANS_NONE },
#else
  { "MODE_HDR_2",     NULL,                                PROT_TRANS_NONE },
#endif
#endif

#ifdef FEATURE_MMODE_TRIPLE_SIM
#ifdef MMOC_GW_SUPPORTED
  { "MODE_GWL_3",     mmoc_reg_multimode_sanity_err_fatal, PROT_TRANS_NONE },
#else
  { "MODE_GWL_3",     NULL,                                PROT_TRANS_NONE },
#endif

#ifdef MMOC_CDMA_SUPPORTED
  { "MODE_CDMA_3",    mc_cdma_multimode_sanity_err_fatal,  PROT_TRANS_NONE },
#else
  { "MODE_CDMA_3",    NULL,                                PROT_TRANS_NONE },
#endif

#ifdef MMOC_HDR_SUPPORTED
  { "MODE_HDR_3",     mmoc_hdr_multimode_sanity_err_fatal, PROT_TRANS_NONE }
#else
  { "MODE_HDR_3",     NULL,                                PROT_TRANS_NONE }
#endif
#endif
};


/* MMoC's task info.
*/

/* Pointer to TCB for MMOC task */
static rex_tcb_type *mmoc_tcb_ptr;

static mmoc_task_info_s_type           mmoc_task_info;

/* Static data structure used to store MMGSDI info
*/
static mmoc_mmgsdi_info_s_type        mmoc_mmgsdi_info;

/* Local buffer to printing debug messages.
*/
#ifdef FEATURE_MODEM_HEAP_PHASE_2
static char                           *mmoc_debug_str;
#else
static char                            mmoc_debug_str[256];
#endif

/* Static data structure used to store state related variable in MMOC
*/
static mmoc_state_info_s_type   mmoc_state_info;

/* Timer group which will not be disabled during sleep.
** This group ensures MMOC can wake the mobile up from deep sleep.
*/
static timer_group_type                mmoc_timer_group;

#ifdef FEATURE_MODEM_HEAP_PHASE_2
#define MMOC_MAX_CHAR   256
#endif

/* No. of blocks currently allocated from the MODEM heap.
*/
/*lint -esym(765, mmoc_modem_heap_blocks)
** Accessed by unit test cases.
*/
/*lint -esym(552, mmoc_modem_heap_blocks)
** Doesn't consider MMOC_MSG_HIGH as an access.
*/
uint16 mmoc_modem_heap_blocks;


#ifdef MMODE_ADDITIONAL_DEBUG_INFO
/* Global variable for mmoc debug buffer
*/
mmoc_debug_buffer_s_type  mmoc_debug_buffer;
#endif

/* Global variable to store the status on what stacks deactivate
** request has been sent.
*/
static boolean mmoc_susp_resume_status[MAX_SIMS][MAX_STACKS];


/* <EJECT> */
/*===========================================================================
=============================================================================
=============================================================================
==================== MMoC IMPLEMENATATION RELATED HELPER FUNCTIONS ==========
=============================================================================
=============================================================================
===========================================================================*/

/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_mem_free

DESCRIPTION
   This function returuns the specified memory back to the Heap.

   If the memory came from MMOC heap then it is returned there else
   it is returned to the TMC heap.
   If feature FEATURE_MMOC_MODEM_HEAP is defined then it return the
   memory to the main heap.

DEPENDENCIES

   MMOC and TMC heap must have been initialized.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void mmoc_mem_free(

  void *ptr

)
{
  if(ptr != NULL)
  {
    mmoc_modem_heap_blocks--;

    /* Deallocate the memory to heap */
    modem_mem_free(ptr, MODEM_MEM_CLIENT_MMODE);
  }
  else
  {
    return;
  }

} /* mmoc_mem_free() */

#ifdef MMOC_DEBUG
#error code not present
#endif
/*===========================================================================

FUNCTION mmoc_mem_alloc

DESCRIPTION
   This function allocates memory for MMOC from the heap.

DEPENDENCIES

   MMOC and TMC heap must have been initialized.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void *mmoc_mem_alloc(

  size_t                size

)
{
  void *result = modem_mem_alloc(size, MODEM_MEM_CLIENT_MMODE);

  if(result == NULL)
  {
    MMOC_MSG_HIGH_1("MMOC mem alloc failed for size %d", size);
  }
  else
  {
    memset(result, 0, size);
    mmoc_modem_heap_blocks++;
  }

  return result;

} /* mmoc_mem_alloc() */


#ifdef FEATURE_MMODE_DUAL_SIM

/*===========================================================================

FUNCTION mmoc_any_sub_in_srlte_mode

DESCRIPTION
   This function returns if any sub is in srlte sub mode.  

===========================================================================*/
static boolean mmoc_any_sub_in_srlte_mode(

  mmoc_state_info_s_type         *mmoc_info_ptr
)
{
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;

  /* check if any sub is in srlte sub mode */
  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if (mmoc_info_ptr->sub_prop[sub]->is_srlte)
    {
      return TRUE;
    }
  }

  return FALSE;

} 

/*===========================================================================

FUNCTION mmoc_get_srlte_sub

DESCRIPTION
   This function returns the sub in srlte sub mode.  

===========================================================================*/
static sys_modem_as_id_e_type  mmoc_get_srlte_sub(

  mmoc_state_info_s_type         *mmoc_info_ptr
)
{
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_1;

  /* check if any sub is in srlte sub mode */
  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if (mmoc_info_ptr->sub_prop[sub]->is_srlte)
    {
      return sub;
    }
  }

  return SYS_MODEM_AS_ID_NONE;
} 

/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_calculate_sub_on_hold

DESCRIPTION
   This function returns the sub to be put on hold for 2 secs. The purpose
   of this is to allow the priority sub to activate first, before the sub
   on hold.

===========================================================================*/
static sys_modem_as_id_e_type mmoc_calculate_sub_on_hold(

  mmoc_state_info_s_type         *mmoc_info_ptr
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  sys_modem_as_id_e_type sub_on_hold = SYS_MODEM_AS_ID_NONE;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /* Calculate sub_on_hold if two subs are active in DSDS mode and
  ** if MMOC is not in emergency call pending state*/
  
  if(!mmoc_skip_online_activation(mmoc_info_ptr) &&
     cm_get_device_mode() != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE &&
      mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM) &&
      mmoc_dev_info->active_subs == (SYS_MODEM_AS_ID_1_MASK | SYS_MODEM_AS_ID_2_MASK) &&
      (mmoc_sub_info[SYS_MODEM_AS_ID_1]->ps_enabled ||
       mmoc_sub_info[SYS_MODEM_AS_ID_2]->ps_enabled))
  {
    /* If PS is not priority, mark the first ps_enabled sub as on hold */
    if(!mmoc_dev_info->is_ps_priority)
    {
      for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
      {
        if(mmoc_sub_info[sub]->ps_enabled)
        {
          sub_on_hold = sub;
          break;
        }
      }
    }
    /* If PS is priority, mark the first non-ps_enabled sub as on hold */
    else
    {
      for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
      {
        if(!mmoc_sub_info[sub]->ps_enabled)
        {
          sub_on_hold = sub;
          break;
        }
      }
    }
  }

  MMOC_MSG_HIGH_1("HOLD_ACQ: Calculated sub on hold: %d", sub_on_hold);

  return sub_on_hold;

} /* mmoc_calculate_ss_on_hold() */

#endif

/*===========================================================================

FUNCTION mmoc_is_multi_standby


DESCRIPTION
  This function returns true incase standby_pref is multi

DEPENDENCIES
  None.

RETURN VALUE
None


SIDE EFFECTS
  None
===========================================================================*/
static boolean  mmoc_is_multi_standby
(

  sys_modem_dual_standby_pref_e_type   standby_pref
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  if(standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY
#ifdef FEATURE_MMODE_TRIPLE_SIM
     || standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY
#endif
  )
  {
    return TRUE;
  }

  return FALSE;
}

/* <EJECT> */
/*===========================================================================
FUNCTION mmoc_get_mmoc_mmgsdi_info_ptr

DESCRIPTION
  This function used to retrieve the pointer to general information in MMOC

RETURN VALUE
  Pointer to mmoc_mmgsdi_info

DEPENDENCIES
  None
===========================================================================*/
mmoc_mmgsdi_info_s_type *mmoc_get_mmgsdi_info_ptr( void )
{
  return &mmoc_mmgsdi_info;
}  /* mmoc_get_mmgsdi_info_ptr() */

/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_get_task_info_ptr

DESCRIPTION
  This function will return the pointer to the MMoC's task info struct.

DEPENDENCIES
  None.

RETURN VALUE
  mmoc_task_info_s_type* - Pointer to the local definition of MMoC's task
                           info struct.
  Note! Cannot be NULL.

SIDE EFFECTS
  None.
===========================================================================*/
#ifdef FEATURE_NON_DEMAND_PAGED_FUNCTION
#error code not present
#endif
static mmoc_task_info_s_type          *mmoc_get_task_info_ptr
(

  void

)
{
  /* Return the pointer to the MMoC's task info.
  */
  return &mmoc_task_info;
}
#ifdef FEATURE_NON_DEMAND_PAGED_FUNCTION
#error code not present
#endif

/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_get_state_info_ptr

DESCRIPTION
  This function will return the pointer to the MMoC's state info struct.

DEPENDENCIES
  None.

RETURN VALUE
  mmoc_state_info_s_type* - Pointer to the local definition of MMoC's state
                            info struct.
  Note! Cannot be NULL.

SIDE EFFECTS
  None.
===========================================================================*/
mmoc_state_info_s_type  *mmoc_get_state_info_ptr(void)
{
  return &mmoc_state_info;
}


/*===========================================================================

FUNCTION mmoc_init_state_dev_info

DESCRIPTION
  This function will initialize the MMoC state info to the power up state.

===========================================================================*/
static void                            mmoc_init_state_dev_info
(
  mmoc_state_info_s_type          *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  static boolean is_called = FALSE;
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = NULL;
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  mmoc_dev_info = &mmoc_info_ptr->dev_prop;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize the device properties */

  mmoc_dev_info->trans = MMOC_TRANS_NULL;
  mmoc_dev_info->trans_state = MMOC_TRANS_STATE_NULL;
  memset(&mmoc_dev_info->trans_info,0,sizeof(mmoc_dev_info->trans_info));
  mmoc_dev_info->is_sd_initialized = FALSE;
  mmoc_dev_info->is_sd_init_called = FALSE;
  mmoc_dev_info->trans_id = 0;
  mmoc_dev_info->oprt_mode = PROT_OPRT_MODE_NULL;
  mmoc_dev_info->true_oprt_mode = SYS_OPRT_MODE_NONE;
  mmoc_dev_info->is_ph_stat_sent = FALSE;
  mmoc_dev_info->deact_req_idx.asubs_id = SYS_MODEM_AS_ID_NONE;
  mmoc_dev_info->deact_req_idx.stk_id = MM_STACK_NONE;
  mmoc_dev_info->insanity_count = 0;
  memset(&mmoc_dev_info->sanity_timer,0,sizeof(mmoc_dev_info->sanity_timer));
  mmoc_dev_info->exit_fn_ptr = NULL;
  mmoc_dev_info->is_standby_sleep = FALSE;
  mmoc_dev_info->is_redir_allowed = TRUE;
  mmoc_dev_info->prev_standby_pref = SYS_MODEM_DS_PREF_NONE;
  mmoc_dev_info->standby_pref = SYS_MODEM_DS_PREF_SINGLE_STANDBY;
  mmoc_dev_info->prev_active_subs = SYS_MODEM_AS_ID_1_MASK;
  mmoc_dev_info->active_subs = SYS_MODEM_AS_ID_1_MASK;
  mmoc_dev_info->device_mode = SYS_MODEM_DEVICE_MODE_NONE;
  mmoc_dev_info->prev_device_mode = SYS_MODEM_DEVICE_MODE_NONE;
  mmoc_dev_info->max_sanity_time_multiple = 1;
  mmoc_dev_info->is_scan_permission = TRUE;
  mmoc_dev_info->redir_sanity_timer = 30000;
  mmoc_dev_info->is_buffer_auto_deact_ind = FALSE;
  mmoc_dev_info->last_stop_req_sent_timestamp = 0;
  mmoc_dev_info->onebuild_feature = SYS_OVERALL_FEATURE_MODE_NORMAL;
  mmoc_dev_info->dual_switch_subs = SYS_MODEM_AS_ID_NO_ACTIVE_MASK;
  memset(&mmoc_dev_info->mmoc_mmgsdi_wait_info,0,sizeof(mmoc_dev_info->mmoc_mmgsdi_wait_info));
#ifdef FEATURE_MMODE_SC_SVLTE
  mmoc_dev_info->pri_slot = MMGSDI_MAX_SESSION_TYPE_ENUM;
#endif
  mmoc_dev_info->is_ps_priority = FALSE;
  mmoc_dev_info->sub_with_3gpp2 = SYS_MODEM_AS_ID_1;
  mmoc_dev_info->prev_sub_with_3gpp2 = SYS_MODEM_AS_ID_1;
  mmoc_dev_info->sub_with_cap_chg = SYS_MODEM_AS_ID_NONE;
  mmoc_dev_info->sd_nam_sel_mask = 0;
  memset(&mmoc_dev_info->hold_timer,0,sizeof(mmoc_dev_info->hold_timer));
  mmoc_dev_info->pending_emerg_state = CM_E911_PENDING_STATE_NONE;
    
#ifdef MMOC_CDMA_SUPPORTED
  mcc_set_asid_vfr_info(SYS_MODEM_AS_ID_1, SYS_VFR_PRIMARY);
#endif

  for(sub = SYS_MODEM_AS_ID_1; sub < MAX_SIMS; sub++)
  {
    if(is_called && mmoc_info_ptr->sub_prop[sub] != NULL)
    {
      mmoc_mem_free(mmoc_info_ptr->sub_prop[sub]);
    }
    mmoc_info_ptr->sub_prop[sub] = NULL;
  }

  is_called = TRUE;

}

/*===========================================================================

FUNCTION mmoc_init_state_sub_info

DESCRIPTION
  This function will initialize the MMoC state info to the power up state.

===========================================================================*/
static void                            mmoc_init_state_sub_info
(
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info,
  /* Pointer to MMoC's state information.
  */

  sys_modem_as_id_e_type           asubs_id,

  sys_subs_feature_t               sfmode,

  sys_overall_feature_t            ofmode
)
{
  multimode_stack_e_type stk;
  mmoc_task_info_s_type          *mmoc_task_ptr = mmoc_get_task_info_ptr();

  MMOC_ASSERT( mmoc_sub_info != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize the subscription properties */

  mmoc_sub_info->nam = asubs_id + 1;
  mmoc_sub_info->sub_avail = PROT_SUBSC_MASK_NONE;
  mmoc_sub_info->prot_subsc_chg = PROT_SUBSC_CHG_NONE;
  mmoc_sub_info->hdr_deact_activate_stack = MM_STACK_NONE;
  mmoc_sub_info->subs_feature = SYS_SUBS_FEATURE_MODE_NORMAL;
  mmoc_sub_info->pm_subs_feature = SYS_SUBS_FEATURE_MODE_NORMAL;
  mmoc_sub_info->subs_capability = SD_SS_MODE_PREF_NONE;
  mmoc_sub_info->ps_enabled = FALSE;
  mmoc_sub_info->prev_active_stacks = 0;
  mmoc_sub_info->mode_pref = SD_SS_MODE_PREF_NONE;
  mmoc_sub_info->is_srlte = FALSE;

  if(mmoc_is_ofmode_1x_sxlte() && asubs_id == SYS_MODEM_AS_ID_1)
  {
    switch(ofmode)
    {
      case SYS_OVERALL_FEATURE_MODE_SVLTE:
        sfmode = SYS_SUBS_FEATURE_MODE_SVLTE;
        break;

      case SYS_OVERALL_FEATURE_MODE_SRLTE:
        sfmode = SYS_SUBS_FEATURE_MODE_SRLTE;
        break;

      default:
        break;
    }
  }

  if(mmoc_is_sglte(asubs_id))
  {
    sfmode = SYS_SUBS_FEATURE_MODE_SGLTE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Set number of stacks based on feature mode */

  switch(sfmode)
  {
    case SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE:
    case SYS_SUBS_FEATURE_MODE_SVLTE:
    case SYS_SUBS_FEATURE_MODE_SRLTE:
    case SYS_SUBS_FEATURE_MODE_SGLTE:
      mmoc_sub_info->nStacks = 3;
      break;

    case SYS_SUBS_FEATURE_MODE_NORMAL:
    default:
      mmoc_sub_info->nStacks = 2;
      break;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Allocate memory for stack timers and properties */

  mmoc_task_ptr->timer[asubs_id] = (timer_type *) mmoc_mem_alloc(
                                     mmoc_sub_info->nStacks*sizeof(timer_type));

  for(stk = MM_STACK_0; stk < mmoc_sub_info->nStacks && stk < MAX_STACKS; stk++)
  {
    MMOC_MSG_LOW_2("Defining timer for sub %d stk %d", asubs_id, stk);
    timer_def2( &mmoc_task_ptr->timer[asubs_id][stk], &mmoc_timer_group );
    mmoc_sub_info->stack_prop[stk] = (mmoc_state_info_stack_prop_s_type *) mmoc_mem_alloc(
                                       sizeof(mmoc_state_info_stack_prop_s_type));
  }

}

/*===========================================================================

FUNCTION mmoc_init_subsc_trans_sub_info

DESCRIPTION
  This function will initialize the MMoC state info to the power up state.

===========================================================================*/
static void                            mmoc_init_subsc_trans_sub_info
(
  mmoc_subsc_chgd_sub_prop_s_type *mmoc_sub_info,
  /* Pointer to MMoC's state information.
  */

  sys_modem_as_id_e_type           asubs_id
)
{
  MMOC_ASSERT( mmoc_sub_info != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize the subscription properties */

  mmoc_sub_info->as_id = asubs_id;
  mmoc_sub_info->active_stacks = MM_STACK_NO_ACTIVE_MASK;
  mmoc_sub_info->nv_context = asubs_id;
  mmoc_sub_info->is_perso_locked = FALSE;
  mmoc_sub_info->sub_avail = PROT_SUBSC_MASK_NONE;
  mmoc_sub_info->prot_subsc_chg = PROT_SUBSC_CHG_NONE;
  mmoc_sub_info->subs_capability = SD_SS_MODE_PREF_NONE;
  mmoc_sub_info->orig_mode = SD_SS_ORIG_MODE_NONE;
  mmoc_sub_info->mode_pref = SD_SS_MODE_PREF_NONE;
  mmoc_sub_info->band_pref = SD_SS_BAND_PREF_NONE;
  mmoc_sub_info->roam_pref = SD_SS_ROAM_PREF_ANY;
  mmoc_sub_info->lte_band_pref = SYS_LTE_BAND_MASK_CONST_NONE;
  mmoc_sub_info->tds_band_pref = SD_SS_BAND_PREF_NONE;

  /* 3GPP preferences */
  mmoc_sub_info->gpp_session_type = MMGSDI_GW_PROV_PRI_SESSION;
  mmoc_sub_info->acq_order_pref = SD_SS_ACQ_ORDER_PREF_AUTO;
  mmoc_sub_info->srv_domain_pref = SD_SS_SRV_DOMAIN_PREF_NONE;
  memset(&mmoc_sub_info->manual_sys_info,0,sizeof(mmoc_sub_info->manual_sys_info));

  /* 3GPP2 preferences */
  mmoc_sub_info->gpp2_session_type = MMGSDI_1X_PROV_PRI_SESSION;
  mmoc_sub_info->prl_pref = SD_SS_PRL_PREF_NONE;
  mmoc_sub_info->hybr_pref = SD_SS_HYBR_PREF_NONE;
  mmoc_sub_info->ue_mode = SYS_UE_MODE_NONE;
  mmoc_sub_info->rat_capability = SD_SS_MODE_PREF_NONE;
  mmoc_sub_info->is_ue_mode_substate_srlte = FALSE;

}



#ifdef MMODE_ADDITIONAL_DEBUG_INFO
/*===========================================================================

FUNCTION mmoc_dbg_buf_init_rpt_queue

DESCRIPTION
  Initialize the report queue for current transaction in debug_buffer
  This is done to ensure that reports last stored at the same index
  are cleared.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                            mmoc_dbg_buf_init_rpt_queue
(
  uint8       mmoc_dbg_buf_idx
)
{
  int i, j, k;

  for( i = 0 ; i < MMOC_MAX_RPT_COUNT; i++ )
  {
    mmoc_debug_buffer.dbg_buf[mmoc_dbg_buf_idx].rpt_queue[i].rpt_name =
      MMOC_RPT_MAX;
    mmoc_debug_buffer.dbg_buf[mmoc_dbg_buf_idx].rpt_queue[i].task_name =
      MMOC_TASK_NAME_NONE;

    for ( j = 0; j < (int)MAX_SIMS; j++ )
    {
      for( k = 0; k < (int)MM_STACK_ALL; k++)
      {
        mmoc_debug_buffer.dbg_buf[mmoc_dbg_buf_idx].rpt_queue[i].prot_state[j][k] =
          PROT_STATE_NULL;
      }
    }
  }

}/*mmoc_dbg_buf_init_rpt_queue()*/


/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_dbg_buf_init

DESCRIPTION
  This function will initialize the MMOC _Debug_buffer info
  at time of power up.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void                            mmoc_dbg_buf_init
(

  void
)
{
  int i = 0;

  mmoc_debug_buffer.dbg_buf_idx = -1;

  for( i = 0 ; i < MMOC_MAX_DEBUG_BUFFER_SIZE ; i++ )
  {
    mmoc_debug_buffer.dbg_buf[i].trans_name = MMOC_TRANS_NULL;
    mmoc_debug_buffer.dbg_buf[i].trans_id = PROT_TRANS_NONE;
    memset(mmoc_debug_buffer.dbg_buf[i].addl_info, 0, MMOC_DBG_BUF_TRANS_ADDL_INFO_SIZE);
    mmoc_dbg_buf_init_rpt_queue(i);
  }

}/*mmoc_dbg_buf_init()*/

#endif /* MMODE_ADDITIONAL_DEBUG_INFO */

/*===========================================================================

FUNCTION mmoc_sanity_timer_reset

DESCRIPTION
  Clear the insanity_count and reset the timer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void                            mmoc_sanity_timer_reset(

  mmoc_state_info_s_type         *mmoc_info_ptr
)
{
  MMOC_ASSERT( mmoc_info_ptr != NULL );

  mmoc_info_ptr->dev_prop.insanity_count = 0;

  if(rex_get_timer(&mmoc_info_ptr->dev_prop.sanity_timer) != 0)
  {
    (void) rex_clr_timer( &mmoc_info_ptr->dev_prop.sanity_timer );

  }

}

/*===========================================================================

FUNCTION mmoc_sanity_timer_set

DESCRIPTION
  Set the MMOC sanity timer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void                            mmoc_sanity_timer_set(

  mmoc_state_info_s_type         *mmoc_info_ptr,

  rex_timer_cnt_type              sanity_timeout
)
{
  MMOC_ASSERT( mmoc_info_ptr != NULL );

  if (sanity_timeout != MMOC_SANITY_TIME)
  {
    MMOC_MSG_MED_1("Set the Sanity Time out %ld",sanity_timeout);
  }

#ifdef TEST_FRAMEWORK
  #error code not present
#else
  (void) rex_set_timer( &mmoc_info_ptr->dev_prop.sanity_timer,
                        sanity_timeout
                      );
#endif

}


/*===========================================================================

FUNCTION mmoc_next_trans_id

DESCRIPTION
  This function gets the next valid transaction Id. Also, sets the value
  in the MMoC's state information.

===========================================================================*/
static prot_trans_type                 mmoc_next_trans_id
(
  mmoc_state_info_s_type          *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  MMOC_ASSERT( mmoc_info_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Print out debug messages.
  */
  MMOC_MSG_LOW_2("trans %d trans_state %d", mmoc_info_ptr->dev_prop.trans, mmoc_info_ptr->dev_prop.trans_state);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ++mmoc_info_ptr->dev_prop.trans_id;

  /* PROT_TRANS_NONE is used by protocols when autodeactivating themselves and
  ** reporting to MMOC, and PROT_TRANS_ID_RESERVED is a generic reserved
  ** trans_id (currently only used by HDR to send response to MMOC when CM
  ** sends a tunnel deactivate request to it), so keep incrementing trans_id
  ** if it is equal to either of them.
  */
  while ( (mmoc_info_ptr->dev_prop.trans_id) == PROT_TRANS_NONE ||
          (mmoc_info_ptr->dev_prop.trans_id) == PROT_TRANS_ID_RESERVED )
  {
    ++mmoc_info_ptr->dev_prop.trans_id;
  }


  return mmoc_info_ptr->dev_prop.trans_id;

} /* mmoc_next_trans_id() */


/*===========================================================================

FUNCTION mmoc_update_standby_sleep_wakeup_state

DESCRIPTION
  This function checks if transitioned in/out of standby sleep. If so
  informs CM of the new standby state.

===========================================================================*/
static void                            mmoc_update_standby_sleep_wakeup_state
(
  mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  boolean is_hybr_2_pwrsave = FALSE;
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  sub = mmoc_info_ptr->dev_prop.sub_with_3gpp2;

  if (!(sub >= SYS_MODEM_AS_ID_1 && sub < MAX_SIMS)) 
  {
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(mmoc_is_1x_sxlte(sub) && sub < MAX_SIMS)
  {
    is_hybr_2_pwrsave =
      mmoc_info_ptr->sub_prop[sub]->stack_prop[MM_STACK_2]->prot_state == PROT_STATE_NULL ||
      mmoc_info_ptr->sub_prop[sub]->stack_prop[MM_STACK_2]->prot_state == PROT_STATE_PWR_SAVE;
  }
  else
  {
    /* If no SV/SG then don't care about hybr_2 stack status */
    is_hybr_2_pwrsave = TRUE;
  }

  /* Check if we are in pwr_save.
  */
  if ( mmoc_info_ptr->sub_prop[sub]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_PWR_SAVE &&
       ( mmoc_info_ptr->sub_prop[sub]->stack_prop[MM_STACK_1]->prot_state == PROT_STATE_NULL ||
         mmoc_info_ptr->sub_prop[sub]->stack_prop[MM_STACK_1]->prot_state == PROT_STATE_PWR_SAVE
       ) && is_hybr_2_pwrsave)
  {
    /* Check if mmoc state is not in standby sleep.
    */
    if ( !mmoc_info_ptr->dev_prop.is_standby_sleep )
    {
      /* MMoc state in standby wakeup,
      ** Send the STANDBY SLEEP report to CM.
      */
      mmoc_send_cm_rpt( CM_STANDBY_SLEEP_F, NULL,sub);
      mmoc_info_ptr->dev_prop.is_standby_sleep = TRUE;
    }
  }
  else
  {
    /* Phone is not in standby sleep, check mmoc state.
    */
    if ( mmoc_info_ptr->dev_prop.is_standby_sleep )
    {
      /* MMOC state in standby sleep,
      ** Send the STANDBY WAKEUP report to CM.
      */
      mmoc_send_cm_rpt( CM_STANDBY_WAKE_F, NULL,sub);
      mmoc_info_ptr->dev_prop.is_standby_sleep = FALSE;
    }
  }
} /* mmoc_update_standby_sleep_wakeup_state() */


/*===========================================================================

FUNCTION mmoc_update_prot_state

DESCRIPTION
  This function sets the MMoC's protocol state.

===========================================================================*/
void                            mmoc_update_prot_state
(

  prot_state_e_type           new_prot_state,
  /* The new protocol state of the MMoC.
  */

  mm_sub_stk_id_s_type        mm_id,
  /* Update prot. state corresponding to this SS instance.
  */

  mmoc_state_info_s_type     *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = NULL;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS]= {NULL};

  MMOC_ASSERT_ENUM_IS_INRANGE(new_prot_state, PROT_STATE_MAX);
  MMOC_ASSERT( mmoc_info_ptr != NULL );

  mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /* if subs id or stk id is out of range, return
  */
  if (!MMOC_ENUM_IS_INRANGE(mm_id.asubs_id, MAX_SIMS) ||
      !MMOC_ENUM_IS_INRANGE(mm_id.stk_id, MAX_STACKS))
    return;

  /* if subs id or stk id has not been allocated or prot state is unchanged, return
  */
  if( mmoc_sub_info[mm_id.asubs_id] == NULL ||
      mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id] == NULL ||
      new_prot_state == mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state)
  {
    return;
  }

  /* Print out debug messages. */
  MMOC_MSG_LOW_6("trans %d trans_state %d prot_state %d new_prot_state %d sub %d stk %d",
                 mmoc_dev_info->trans,
                 mmoc_dev_info->trans_state,
                 mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state,
                 new_prot_state,
                 mm_id.asubs_id,
                 mm_id.stk_id);

#if (defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)) && (defined(MMOC_CDMA_SUPPORTED) || defined(MMOC_HDR_SUPPORTED))
  if(new_prot_state == PROT_STATE_ONLINE_CDMA)
  {
    cmregprx_cmd_3gpp2_status_chgd(SYS_SYS_MODE_CDMA,TRUE,mm_id);
  }
  else if (mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state == PROT_STATE_ONLINE_CDMA)
  {
    cmregprx_cmd_3gpp2_status_chgd(SYS_SYS_MODE_CDMA,FALSE,mm_id);
  }


  if(new_prot_state == PROT_STATE_ONLINE_HDR)
  {
    cmregprx_cmd_3gpp2_status_chgd(SYS_SYS_MODE_HDR,TRUE,mm_id);
  }
  else if(mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state == PROT_STATE_ONLINE_HDR)
  {
    cmregprx_cmd_3gpp2_status_chgd(SYS_SYS_MODE_HDR,FALSE,mm_id);
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the new protocol state. */

  mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state = new_prot_state;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Error fatal if HDR online on both primary and secondary stack */

  if(mm_id.asubs_id == mmoc_dev_info->sub_with_3gpp2 &&
      mmoc_sub_info[mm_id.asubs_id]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_ONLINE_HDR &&
      mmoc_sub_info[mm_id.asubs_id]->stack_prop[MM_STACK_1]->prot_state == PROT_STATE_ONLINE_HDR)
  {
    MMOC_ERR_FATAL_0("HDR on both primary and secondary stack");
  }


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update standby sleep/wakeup transition, if changed. */

  mmoc_update_standby_sleep_wakeup_state(mmoc_info_ptr);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Each time we set prot_state to NULL, we will check if we have any auto_deact
  ** buffered for that stack. Setting prot_state to NULL means we are deactivating
  ** prot. Hence, if any auto_deact report found, we will discard it */

  if(new_prot_state == PROT_STATE_NULL)
  {
    mmoc_is_auto_deact_ind_rpt_buf_per_stack(mmoc_info_ptr,mm_id,TRUE);
  }

} /* mmoc_update_prot_state() */


/*===========================================================================

FUNCTION mmoc_update_prot_dormant_state

DESCRIPTION
  This function sets the MMoC's protocol dormant state.

===========================================================================*/
static void mmoc_update_prot_dormant_state
(
  sd_ss_mode_pref_e_type             dormant_prot,
  /* The new protocol dormant state of the MMoC.
  */

  mm_sub_stk_id_s_type               mm_id,
  /* Update prot. state corresponding to this SS instance.
  */

  mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */

)
{
  MMOC_ASSERT_ENUM_IS_INRANGE(dormant_prot, SD_SS_MODE_PREF_MAX );
  MMOC_ASSERT( mmoc_info_ptr != NULL );
  if(mm_id.asubs_id < SYS_MODEM_AS_ID_1 || mm_id.asubs_id >= MAX_SIMS ||
      mm_id.stk_id < MM_STACK_0 || mm_id.stk_id >= MAX_STACKS ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id] == NULL ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id] == NULL)
  {
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Print out debug messages.
  */
  MMOC_MSG_HIGH_3("New dormant prot %d sub %d stk %d", dormant_prot, mm_id.asubs_id, mm_id.stk_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the new protocol state.
  */
  mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_dormant = dormant_prot;

} /* mmoc_update_prot_dormant_state() */



/*===========================================================================

FUNCTION mmoc_update_db_dmss_state

DESCRIPTION
  This function sets the DB DMSS state based on the current protocol
  state.

===========================================================================*/
static void                            mmoc_update_db_dmss_state
(
  const  mmoc_state_info_s_type   *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
#ifdef MMOC_USES_DB_SERVICES
  static db_items_value_type  db_item;

  /* Determine what the DB state is.
  */
  if(mmoc_info_ptr->dev_prop.sub_with_3gpp2 < SYS_MODEM_AS_ID_1 ||
      mmoc_info_ptr->dev_prop.sub_with_3gpp2 >= MAX_SIMS ||
      mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2] == NULL)
  {
    return;
  }
  switch(mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_0]->prot_state)
  {
    case PROT_STATE_NULL:
      db_item.dmss_state = DB_DMSS_POWERUP_S;
      break;

    case PROT_STATE_ONLINE_CDMA:
    case PROT_STATE_ONLINE_GWL:
    case PROT_STATE_ONLINE_HDR:
    case PROT_STATE_ONLINE_DED_MEAS:
      db_item.dmss_state = DB_DMSS_ONLINE_DIGITAL_S;
      break;

    case PROT_STATE_OFFLINE_CDMA:
    case PROT_STATE_OFFLINE:
      db_item.dmss_state = DB_DMSS_OFFLINE_DIGITAL_S;
      break;

    case PROT_STATE_RESET:
      db_item.dmss_state = DB_DMSS_RESET_S;
      break;

    case PROT_STATE_LPM:
    case PROT_STATE_PWR_DOWN:
      db_item.dmss_state = DB_DMSS_POWERDOWN_S;
      break;

    case PROT_STATE_GPSONE_MSBASED:
      db_item.dmss_state = DB_DMSS_GPS_S;
      break;

    case PROT_STATE_BLOCKED:
    case PROT_STATE_FTM:
    case PROT_STATE_PWR_SAVE:
    case PROT_STATE_MAX:
    default:
      /* No DB state defined for these protocol state, don't update
      ** the DB state.
      */
      break;

  } /* switch( mmoc_info_ptr->prot_state ) */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the dmss state.
  */

  mcc_db_put(DB_DMSS_STATE, &db_item, mmoc_info_ptr->dev_prop.sub_with_3gpp2);

#endif /* defined(MMOC_USES_DB_SERVICES) */

} /* mmoc_update_db_dmss_state() */



#ifdef MMOC_HDR_SUPPORTED
/*===========================================================================

FUNCTION mmoc_is_hdr_acq_allowed_hicps

DESCRIPTION
  Checks if MAIN is in an acceptable state for HDR to be acquired on the
  hybrid instance.

RETURN VALUE
  TRUE  - Hdr acquisition can proceed.
  FALSE - Hdr acquisition cannot be done.

===========================================================================*/
static boolean                         mmoc_is_hdr_acq_allowed_hicps
(

  mmoc_state_info_s_type    *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = NULL;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /* Consider state of hybr2 stack before providing permission
  */
  if(mmoc_is_1x_sxlte(mmoc_dev_info->sub_with_3gpp2) &&
      mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_2]->prot_state
      == PROT_STATE_ONLINE_GWL)
  {
    return FALSE;
  }

  /* If HYBR HDR is in suspend state, do not allow HYBR HDR acquisition
  */
  if(mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_1]->is_suspend)
  {
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if((mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_ONLINE_CDMA) ||
      (mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_ONLINE_DED_MEAS) ||
      (mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_PWR_SAVE) ||
      (mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_NULL)
    )
  {
    return TRUE;
  }

  return FALSE;

} /* mmoc_is_hdr_acq_allowed_hicps () */

/*===========================================================================

FUNCTION mmoc_is_hdr_acq_allowed_mmss

DESCRIPTION
  Checks if MDM has permission to proceed with HDR acquisition.

RETURN VALUE
  TRUE  - Hdr acquisition can proceed.
  FALSE - Hdr acquisition cannot be done.

===========================================================================*/
static boolean                         mmoc_is_hdr_acq_allowed_mmss
(

  const mmoc_state_info_s_type    *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  MMOC_ASSERT( mmoc_info_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return( mmoc_info_ptr->dev_prop.is_scan_permission );

} /* mmoc_is_hdr_acq_allowed_mmss () */

#endif /*  MMOC_HDR_SUPPORTED */

#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
/*===========================================================================

FUNCTION mmoc_is_hybr_2_acq_allowed

DESCRIPTION
  Checks if MAIN is in an acceptable state for HYBR 2 to begin acquisition

RETURN VALUE
  TRUE  - Hybr 2 acquisition can proceed.
  FALSE - Hybr 2 acquisition cannot be done.

===========================================================================*/
static boolean                         mmoc_is_hybr_2_acq_allowed
(
  const mmoc_state_info_s_type    *mmoc_info_ptr,
  /* Pointer to MMoC's state information.
  */

  sys_modem_as_id_e_type           asubs_id
)
{
  MMOC_ASSERT( mmoc_info_ptr != NULL );

  if (!(asubs_id >= SYS_MODEM_AS_ID_1 && asubs_id < MAX_SIMS)) 
  {
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If HYBR2 is in suspend state, do not allow HYBR2 acquisition
  */
  if(mmoc_info_ptr->sub_prop[asubs_id]->stack_prop[MM_STACK_2]->is_suspend)
  {
    return FALSE;
  }

  if (mmoc_info_ptr->sub_prop[asubs_id]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_ONLINE_CDMA ||
      mmoc_info_ptr->sub_prop[asubs_id]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_ONLINE_DED_MEAS ||
      mmoc_info_ptr->sub_prop[asubs_id]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_PWR_SAVE ||
      mmoc_info_ptr->sub_prop[asubs_id]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_NULL)
  {
    return TRUE;
  }

  return FALSE;

} /* mmoc_is_hybr_2_acq_allowed () */
#endif /*  MMOC_GW_SUPPORTED */


/*===========================================================================

FUNCTION mmoc_is_featuremode

DESCRIPTION
  Checks featureMode.
===========================================================================*/
boolean mmoc_is_featuremode(sys_overall_feature_t fmode)
{
  mmoc_state_info_s_type *mmoc_info_ptr = mmoc_get_state_info_ptr();

  return(fmode == mmoc_info_ptr->dev_prop.onebuild_feature);
}

/*===========================================================================

FUNCTION mmoc_is_ofmode_sglte

DESCRIPTION
  Checks if the overall featureMode is sglte.
===========================================================================*/
boolean mmoc_is_ofmode_sglte(void)
{
  mmoc_state_info_s_type *mmoc_info_ptr = mmoc_get_state_info_ptr();

  return(mmoc_info_ptr->dev_prop.onebuild_feature == SYS_OVERALL_FEATURE_MODE_SGLTE);
}

/*===========================================================================

FUNCTION mmoc_is_ofmode_1x_sxlte

DESCRIPTION
  Checks if the overall featureMode is svlte/srlte.
===========================================================================*/
boolean mmoc_is_ofmode_1x_sxlte(void)
{
  mmoc_state_info_s_type *mmoc_info_ptr = mmoc_get_state_info_ptr();

  return(mmoc_info_ptr->dev_prop.onebuild_feature == SYS_OVERALL_FEATURE_MODE_SVLTE ||
         mmoc_info_ptr->dev_prop.onebuild_feature == SYS_OVERALL_FEATURE_MODE_SRLTE);
}

/*===========================================================================

FUNCTION mmoc_is_sglte

DESCRIPTION
  Checks if the overall featureMode or the sub feature mode is sglte.
===========================================================================*/
boolean mmoc_is_sglte(sys_modem_as_id_e_type as_id)
{
  mmoc_state_info_s_type *mmoc_info_ptr = mmoc_get_state_info_ptr();

  return(mmoc_info_ptr->dev_prop.onebuild_feature == SYS_OVERALL_FEATURE_MODE_SGLTE ||
         mmoc_info_ptr->sub_prop[as_id]->subs_feature == SYS_SUBS_FEATURE_MODE_SGLTE);
}

/*===========================================================================

FUNCTION mmoc_is_1x_sxlte

DESCRIPTION
  Checks if the featureMode/subfeature mode is either svlte or srlte.
===========================================================================*/
boolean mmoc_is_1x_sxlte(sys_modem_as_id_e_type as_id)
{
  mmoc_state_info_s_type *mmoc_info_ptr = mmoc_get_state_info_ptr();

  return(mmoc_info_ptr->dev_prop.onebuild_feature == SYS_OVERALL_FEATURE_MODE_SVLTE ||
         mmoc_info_ptr->dev_prop.onebuild_feature == SYS_OVERALL_FEATURE_MODE_SRLTE ||
         mmoc_info_ptr->sub_prop[as_id]->subs_feature == SYS_SUBS_FEATURE_MODE_SVLTE ||
         mmoc_info_ptr->sub_prop[as_id]->subs_feature == SYS_SUBS_FEATURE_MODE_SRLTE);
}

/*===========================================================================

FUNCTION mmoc_is_sub_mode_sxlte

DESCRIPTION
  Checks if the subfeature mode is either svlte or srlte or sglte
===========================================================================*/
boolean mmoc_is_sub_mode_sxlte(sys_modem_as_id_e_type as_id)
{
  mmoc_state_info_s_type *mmoc_info_ptr = mmoc_get_state_info_ptr();

  return(mmoc_info_ptr->sub_prop[as_id]->subs_feature == SYS_SUBS_FEATURE_MODE_SGLTE ||
         mmoc_info_ptr->sub_prop[as_id]->subs_feature == SYS_SUBS_FEATURE_MODE_SVLTE ||
         mmoc_info_ptr->sub_prop[as_id]->subs_feature == SYS_SUBS_FEATURE_MODE_SRLTE);
}

/*===========================================================================

FUNCTION mmoc_is_sxlte

DESCRIPTION
  Checks if the overall featureMode is MSIM and sub feature mode is sxlte.
===========================================================================*/
boolean mmoc_is_sxlte(sys_modem_as_id_e_type as_id)
{
  mmoc_state_info_s_type *mmoc_info_ptr = mmoc_get_state_info_ptr();

  return(mmoc_info_ptr->dev_prop.onebuild_feature == SYS_OVERALL_FEATURE_MODE_MULTISIM &&
         (mmoc_info_ptr->sub_prop[as_id]->subs_feature == SYS_SUBS_FEATURE_MODE_SGLTE ||
          mmoc_info_ptr->sub_prop[as_id]->subs_feature == SYS_SUBS_FEATURE_MODE_SRLTE ||
          mmoc_info_ptr->sub_prop[as_id]->subs_feature == SYS_SUBS_FEATURE_MODE_SVLTE));
}

/*===========================================================================

FUNCTION mmoc_is_any_sxlte

DESCRIPTION
  Checks if the overall featureMode is MSIM and any sub feature mode is sxlte.
===========================================================================*/
boolean mmoc_is_any_sxlte()
{
  mmoc_state_info_s_type *mmoc_info_ptr = mmoc_get_state_info_ptr();
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(mmoc_info_ptr->dev_prop.onebuild_feature == SYS_OVERALL_FEATURE_MODE_MULTISIM &&
        (mmoc_info_ptr->sub_prop[sub]->subs_feature == SYS_SUBS_FEATURE_MODE_SGLTE ||
         mmoc_info_ptr->sub_prop[sub]->subs_feature == SYS_SUBS_FEATURE_MODE_SRLTE ||
         mmoc_info_ptr->sub_prop[sub]->subs_feature == SYS_SUBS_FEATURE_MODE_SVLTE))
    {
      return TRUE;
    }
  }

  return FALSE;

}

/*===========================================================================

FUNCTION mmoc_convert_sys_mode_asubsid_to_mmoc_mode

DESCRIPTION
  Maps sys_mode and asubs_id to mmoc_mode_e_type

===========================================================================*/
mmoc_mode_e_type mmoc_convert_sys_mode_asubsid_to_mmoc_mode(

  sys_sys_mode_e_type     sys_mode,
  sys_modem_as_id_e_type  asubs_id
)
{
  mmoc_mode_e_type ret = MMOC_MODE_NONE;

  switch(sys_mode)
  {
    case SYS_SYS_MODE_GSM:
    case SYS_SYS_MODE_WCDMA:
    case SYS_SYS_MODE_LTE:
    case SYS_SYS_MODE_TDS:
    case SYS_SYS_MODE_GW:
    case SYS_SYS_MODE_GWL:
      switch(asubs_id)
      {
        case SYS_MODEM_AS_ID_1:
          ret = MMOC_MODE_GWL_1;
          break;

        case SYS_MODEM_AS_ID_2:
          ret = MMOC_MODE_GWL_2;
          break;

#ifdef FEATURE_MMODE_TRIPLE_SIM
        case SYS_MODEM_AS_ID_3:
          ret = MMOC_MODE_GWL_3;
          break;
#endif

        default:
          break;
      }
      break;

    case SYS_SYS_MODE_CDMA:
      switch(asubs_id)
      {
        case SYS_MODEM_AS_ID_1:
          ret = MMOC_MODE_CDMA_1;
          break;

        case SYS_MODEM_AS_ID_2:
          ret = MMOC_MODE_CDMA_2;
          break;

#ifdef FEATURE_MMODE_TRIPLE_SIM
        case SYS_MODEM_AS_ID_3:
          ret = MMOC_MODE_CDMA_3;
          break;
#endif

        default:
          break;
      }
      break;

    case SYS_SYS_MODE_HDR:
      switch(asubs_id)
      {
        case SYS_MODEM_AS_ID_1:
          ret = MMOC_MODE_HDR_1;
          break;

        case SYS_MODEM_AS_ID_2:
          ret = MMOC_MODE_HDR_2;
          break;

#ifdef FEATURE_MMODE_TRIPLE_SIM
        case SYS_MODEM_AS_ID_3:
          ret = MMOC_MODE_HDR_3;
          break;
#endif

        default:
          break;
      }
      break;

    default:
      break;
  }

  return ret;

}

/*===========================================================================

FUNCTION mmoc_free_memory

DESCRIPTION
  Frees all dynamic memory that will need to be re-allocated per new
  configuration from PM.

===========================================================================*/
static void mmoc_free_memory
(
  mmoc_state_info_s_type *mmoc_info_ptr,
  sys_modem_as_id_e_type  mcfg_refresh_sub,
  boolean                 is_power_on
)
{
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stk = MM_STACK_NONE;
  mmoc_task_info_s_type *mmoc_task_ptr = mmoc_get_task_info_ptr();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* De-allocate all required subs/stacks/timers */

  for(sub = SYS_MODEM_AS_ID_1; sub < MAX_SIMS; sub++)
  {
    if(mmoc_task_ptr->timer[sub]    != NULL &&
        mmoc_info_ptr->sub_prop[sub] != NULL &&
        (is_power_on || sub == mcfg_refresh_sub))
    {
      for(stk = MM_STACK_0; stk < MAX_STACKS; stk++)
      {
        if(mmoc_info_ptr->sub_prop[sub]->stack_prop[stk] != NULL)
        {
          /* Undefine the stack timer */
          if(stk < mmoc_info_ptr->sub_prop[sub]->nStacks)
          {
            MMOC_MSG_LOW_2("Freeing sub %d stk %d timer", sub, stk);
            timer_undef(&mmoc_task_ptr->timer[sub][stk]);
          }

          MMOC_MSG_LOW_2("Freeing sub %d stk %d memory", sub, stk);

          /* Free the stack memory */
          mmoc_mem_free(mmoc_info_ptr->sub_prop[sub]->stack_prop[stk]);
          mmoc_info_ptr->sub_prop[sub]->stack_prop[stk] = NULL;
        }
      }

      MMOC_MSG_LOW_1("Freeing sub %d memory and timer", sub);

      /* Free the sub memory */
      mmoc_mem_free(mmoc_info_ptr->sub_prop[sub]);
      mmoc_info_ptr->sub_prop[sub] = NULL;

      /* Free the timer memory (for all stacks of this sub) */
      mmoc_mem_free(mmoc_task_ptr->timer[sub]);
      mmoc_task_ptr->timer[sub] = NULL;
    }
  }
}


/*===========================================================================

FUNCTION mmoc_read_policyman_config

DESCRIPTION
  Reads policy manager config.
===========================================================================*/
static void mmoc_read_policyman_config
(

  mmoc_state_info_s_type *mmoc_info_ptr,
  /* Pointer to MMoC's state information.
         */

  sys_modem_as_id_e_type  mcfg_refresh_sub,
  /* MCFG refresh sub
   */

  boolean                  is_power_on
  /* Is it power on
  */
)
{
  const policyman_item_t *pItem = NULL;
  const policyman_item_id_t id = POLICYMAN_ITEM_DEVICE_CONFIGURATION;
  policyman_item_collection_t const   *pCollection;
  policyman_status_t      status            = POLICYMAN_STATUS_ERR;
  size_t                  nSubs = 0;
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stk = MM_STACK_NONE;

  MMOC_MSG_HIGH_2("mmoc_read_policyman_config: is_power_on %d mcfg_refresh_sub %d",
                  is_power_on, mcfg_refresh_sub);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* De-allocate all required subs/stacks/timers so they can be re-allocated
  ** per PM policy
  */
  mmoc_free_memory(mmoc_info_ptr, mcfg_refresh_sub, is_power_on);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Read MSIM items from PM */

  if (POLICYMAN_SUCCEEDED(policyman_get_items_block_msim((policyman_item_id_t const *)&id,
                          1, &pCollection)))
  {
    pItem = pCollection->pItems[0];
    status = policyman_device_config_overall_feature( pItem,
             &mmoc_info_ptr->dev_prop.onebuild_feature);
    MMOC_MSG_HIGH_1("policyman fmode %d", mmoc_info_ptr->dev_prop.onebuild_feature);
  }

  if(POLICYMAN_SUCCEEDED(status))
  {
    if(mmoc_info_ptr->dev_prop.onebuild_feature != SYS_OVERALL_FEATURE_MODE_MULTISIM)
    {
      mmoc_info_ptr->nSubs = 1;
    }
    else if((status = POLICYMAN_SUCCEEDED(policyman_device_config_num_sims(pItem, &nSubs))))
    {
      mmoc_info_ptr->nSubs = nSubs;
      MMOC_MSG_HIGH_1("policyman nSIM %d", mmoc_info_ptr->nSubs);
    }
    else
    {
      mmoc_info_ptr->nSubs = 1;
      MMOC_MSG_HIGH_1("policyman nSIM read failed, default to %d", mmoc_info_ptr->nSubs);
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Read sfmodes from PM */

  if ( mmoc_info_ptr->dev_prop.onebuild_feature == SYS_OVERALL_FEATURE_MODE_MULTISIM )
  {
    for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
    {
      sys_subs_feature_t pm_sfmode = SYS_SUBS_FEATURE_MODE_NORMAL;
      if(POLICYMAN_SUCCEEDED(policyman_device_config_get_subs_feature(pItem,0,sub,&pm_sfmode)))
      {
        /* Allocate memory for subscription data */
        if(is_power_on || sub == mcfg_refresh_sub)
        {
          mmoc_info_ptr->sub_prop[sub] = (mmoc_state_info_sub_prop_s_type *) mmoc_mem_alloc(
                                           sizeof(mmoc_state_info_sub_prop_s_type));

          /* Initialize subscription data */
          mmoc_init_state_sub_info(mmoc_info_ptr->sub_prop[sub],sub,pm_sfmode,mmoc_info_ptr->dev_prop.onebuild_feature);
        }

        /* During MCFG refresh, update only the sub which got refresh indication
        */
        if(!is_power_on && sub != mcfg_refresh_sub)
        {
          MMOC_MSG_HIGH_2("MCFG_REFRESH: sub %d skip updating feature mode for sub %d", mcfg_refresh_sub, sub);
          continue;
        }

        mmoc_info_ptr->sub_prop[sub]->pm_subs_feature = pm_sfmode;

        if (pm_sfmode == SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE)
        {
#ifdef FEATURE_1XSRLTE
          mmoc_info_ptr->sub_prop[sub]->subs_feature = SYS_SUBS_FEATURE_MODE_SRLTE;
#else
          mmoc_info_ptr->sub_prop[sub]->subs_feature = SYS_SUBS_FEATURE_MODE_NORMAL;
#endif
        }
        else
        {
          mmoc_info_ptr->sub_prop[sub]->subs_feature = pm_sfmode;
        }

        MMOC_MSG_HIGH_3("sfmode: asubs_id %d, pm_sfmode %d, sfmode %d",
                        sub, pm_sfmode, mmoc_info_ptr->sub_prop[sub]->subs_feature);

      }
    }
  }
  else
  {
    sys_subs_feature_t pm_sfmode = SYS_SUBS_FEATURE_MODE_NORMAL;
    if(POLICYMAN_SUCCEEDED(policyman_device_config_get_subs_feature(pItem,0,SYS_MODEM_AS_ID_1,&pm_sfmode)))
    {
      /* Allocate memory for subscription data */
      if(is_power_on || mcfg_refresh_sub == SYS_MODEM_AS_ID_1)
      {
        mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1] = (mmoc_state_info_sub_prop_s_type *) mmoc_mem_alloc(
              sizeof(mmoc_state_info_sub_prop_s_type));

        /* Initialize subscription data */
        mmoc_init_state_sub_info(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1],SYS_MODEM_AS_ID_1,pm_sfmode,mmoc_info_ptr->dev_prop.onebuild_feature);
      }

      /* During MCFG refresh, update only the sub which got refresh indication
      */
      if(is_power_on || SYS_MODEM_AS_ID_1 == mcfg_refresh_sub)
      {
        mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->pm_subs_feature = pm_sfmode;

        if (pm_sfmode == SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE)
        {
          mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->subs_feature = SYS_SUBS_FEATURE_MODE_SRLTE;
        }
        else
        {
          mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->subs_feature = pm_sfmode;
        }
      }
      else
      {
        MMOC_MSG_HIGH_2("MCFG_REFRESH: sub %d skip updating feature mode for sub %d", mcfg_refresh_sub, SYS_MODEM_AS_ID_1);
      }
    }


    if(mmoc_info_ptr->dev_prop.onebuild_feature == SYS_OVERALL_FEATURE_MODE_SVLTE)
    {
      mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->subs_feature = SYS_SUBS_FEATURE_MODE_SVLTE;
    }
    else if(mmoc_info_ptr->dev_prop.onebuild_feature == SYS_OVERALL_FEATURE_MODE_SRLTE)
    {
      mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->subs_feature = SYS_SUBS_FEATURE_MODE_SRLTE;
    }
    else if(mmoc_info_ptr->dev_prop.onebuild_feature == SYS_OVERALL_FEATURE_MODE_NORMAL)
    {
      mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->subs_feature = SYS_SUBS_FEATURE_MODE_NORMAL;
    }

    MMOC_MSG_HIGH_2("SSIM, pm_sfmode %d, sfmode %d",
                    pm_sfmode, mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->subs_feature);
  }

  if(mmoc_info_ptr->dev_prop.onebuild_feature != SYS_OVERALL_FEATURE_MODE_MULTISIM)
  {
    mmoc_info_ptr->dev_prop.prev_device_mode = SYS_MODEM_DEVICE_MODE_SINGLE_SIM;
    mmoc_info_ptr->dev_prop.device_mode = SYS_MODEM_DEVICE_MODE_SINGLE_SIM;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Allocate and initialize memory for transaction info */

  for(sub = SYS_MODEM_AS_ID_1; sub < MAX_SIMS; sub++)
  {
    if(mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub] != NULL)
    {
      mmoc_mem_free(mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]);
      mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub] = NULL;
    }

    mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub] =
      (mmoc_subsc_chgd_sub_prop_s_type *) mmoc_mem_alloc(
        sizeof(mmoc_subsc_chgd_sub_prop_s_type));

    mmoc_init_subsc_trans_sub_info(mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub], sub);
  }

  policyman_item_collection_release(pCollection);
}

/*===========================================================================

FUNCTION is_pref_chng_restore_req

DESCRIPTION
  Checks if gen cmd is pref change due to reason RESTORE.
===========================================================================*/
static boolean is_pref_chng_restore_req
(
  const prot_gen_cmd_s_type *gen_cmd_ptr
)
{
  boolean is_restore_pref = FALSE;

  if(gen_cmd_ptr == NULL)
  {
    return is_restore_pref;
  }

  if(gen_cmd_ptr->cmd_type == PROT_GEN_CMD_PREF_SYS_CHGD)
  {
    sd_ss_pref_reas_e_type pref_reas = gen_cmd_ptr->param.pref_sys_chgd.pref_reas;

    is_restore_pref = (pref_reas == SD_SS_PREF_REAS_RESTORE) ? TRUE : FALSE;
  }

  return is_restore_pref;
}

/*===========================================================================

FUNCTION mmoc_print_lte_band_mask

DESCRIPTION
Print the LTE band mask.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
none

===========================================================================*/
static void mmoc_print_lte_band_mask(sys_lte_band_mask_e_type lte_band_pref)
{
#if (LTE_BAND_NUM == 256)
  if(lte_band_pref.bits_193_256 || lte_band_pref.bits_129_192)
  {
    MMOC_MSG_MED_4("LTE bands 193_256: 0x%08x %08x, LTE bands 129_192: 0x%08x %08x",
                   QWORD_HIGH(lte_band_pref.bits_193_256),
                   QWORD_LOW(lte_band_pref.bits_193_256),
                   QWORD_HIGH(lte_band_pref.bits_129_192),
                   QWORD_LOW(lte_band_pref.bits_129_192));
  }

  MMOC_MSG_MED_4("LTE bands 65_128: 0x%08x %08x, LTE bands 1_64: 0x%08x %08x",
                 QWORD_HIGH(lte_band_pref.bits_65_128),
                 QWORD_LOW(lte_band_pref.bits_65_128),
                 QWORD_HIGH(lte_band_pref.bits_1_64),
                 QWORD_LOW(lte_band_pref.bits_1_64));
#else
  MMOC_MSG_MED_2("LTE bands    1_64: 0x%08x %08x", QWORD_HIGH(lte_band_pref),
                 QWORD_LOW(lte_band_pref));
#endif
}


/*===========================================================================

FUNCTION mmoc_get_other_sub

DESCRIPTION
  Return the other sub.

DEPENDENCIES
  None

RETURN VALUE
  sys_modem_as_id_e_type

SIDE EFFECTS
  None

===========================================================================*/
sys_modem_as_id_e_type  mmoc_get_other_sub(
  sys_modem_as_id_e_type    asubs_id
)
{
  sys_modem_as_id_e_type other_sub;

#ifdef FEATURE_MMODE_TRIPLE_SIM
  other_sub = ((asubs_id==SYS_MODEM_AS_ID_3 || asubs_id==SYS_MODEM_AS_ID_2)?
               SYS_MODEM_AS_ID_1:(asubs_id==SYS_MODEM_AS_ID_1?SYS_MODEM_AS_ID_2:SYS_MODEM_AS_ID_1));
#else
  other_sub = (asubs_id==SYS_MODEM_AS_ID_2)?SYS_MODEM_AS_ID_1:SYS_MODEM_AS_ID_2;
#endif

  return other_sub;
}

/*===========================================================================
FUNCTION mmoc_update_trans_state

DESCRIPTION
  This function sets the MMoC's transaction state as per the value passed in.

===========================================================================*/
void                            mmoc_update_trans_state
(
  mmoc_trans_state_e_type            new_trans_state,
  /* The new transaction state of the MMoC.
  */

  mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  MMOC_ASSERT_ENUM_IS_INRANGE(new_trans_state, MMOC_TRANS_STATE_MAX);
  MMOC_ASSERT( mmoc_info_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (mmoc_info_ptr->dev_prop.trans_state != new_trans_state)
  {
    /* Print out debug messages.
    */
    MMOC_MSG_HIGH_3("trans %d trans_state %d new_trans_state %d",
                    mmoc_info_ptr->dev_prop.trans, mmoc_info_ptr->dev_prop.trans_state,
                    new_trans_state);

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Update the MMoC's new transaction state.
    */
    mmoc_info_ptr->dev_prop.trans_state = new_trans_state;
  }

} /* mmoc_update_trans_state() */



/*===========================================================================

FUNCTION mmoc_update_curr_trans

DESCRIPTION
  This function sets the current MMoC transaction given the new transaction.
  Also, the new transaction state is set to NULL.

===========================================================================*/
static void                            mmoc_update_curr_trans
(

  mmoc_trans_e_type                  new_trans,
  /* The new transaction state of the MMoC.
  */

  mmoc_state_info_s_type             *mmoc_info_ptr,
  /* Pointer to MMoC's state information.
  */

  sys_oprt_mode_e_type               current_oprt_mode
  /* Flag to indicate if RESET_MODEM cmd is being processed.
  */
)
{
  rex_timer_cnt_type  sanity_timeout = MMOC_SANITY_TIME;
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = NULL;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stack = MM_STACK_NONE;

  MMOC_ASSERT_ENUM_IS_INRANGE(new_trans, MMOC_TRANS_MAX);
  MMOC_ASSERT( mmoc_info_ptr != NULL );

  mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(mmoc_dev_info->trans >= MMOC_TRANS_MAX)
  {
    MMOC_ERR_1( "curr_trans out-of-bound: %d, Aborting!!!",
                mmoc_dev_info->trans);
    return;
  }

  if(new_trans >= MMOC_TRANS_MAX)
  {
    MMOC_ERR_1( "invalid new_trans: %d", new_trans);
    return;
  }

  if(mmoc_dev_info->trans_state >= MMOC_TRANS_STATE_MAX)
  {
    MMOC_ERR_1( "invalid trans_state: %d",
                mmoc_dev_info->trans_state);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the new transaction.
  */
  mmoc_dev_info->trans  = new_trans;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the new true_oprt_mode
  */
  mmoc_dev_info->true_oprt_mode = current_oprt_mode;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* When curr_trans is changed, always start from NULL trans_state.
  */
  mmoc_dev_info->trans_state = MMOC_TRANS_STATE_NULL;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Do common initialization.
  */
  mmoc_dev_info->is_ph_stat_sent       = FALSE;
  mmoc_dev_info->deact_req_idx.asubs_id = SYS_MODEM_AS_ID_NONE;
  mmoc_dev_info->deact_req_idx.stk_id = MM_STACK_NONE;
  mmoc_dev_info->is_buffer_auto_deact_ind = FALSE;
  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    mmgsdi_info_ptr->read_items[sub] = MMOC_MMGSDI_MASK_NONE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Clear the insanity_count and reset the timer.
  */
  mmoc_sanity_timer_reset(mmoc_info_ptr);

#ifdef FEATURE_MMODE_TRIPLE_SIM
  if(mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY)
  {
    mmoc_dev_info->max_sanity_time_multiple = 3;
  }
  else
#endif
#ifdef FEATURE_MMODE_DUAL_SIM
    if(mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY)
    {
      mmoc_dev_info->max_sanity_time_multiple = 2;
    }
    else
    {
      mmoc_dev_info->max_sanity_time_multiple = 1;
    }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Start the sanity timer if the current transaction is not NULL.
  */
  if ( mmoc_dev_info->trans != MMOC_TRANS_NULL )
  {
    /* If it is powerdown transaction, load the special timeout value.
    */
    if ( mmoc_dev_info->trans == MMOC_TRANS_PWR_DOWN )
    {
      sanity_timeout = MMOC_PWRDOWN_SANITY_TIME;
    }

    /* If it is waiting for  PS detach conf, load special timeout value.
    */
    if( mmoc_dev_info->trans == MMOC_TRANS_PROT_GEN_CMD &&
        mmoc_dev_info->trans_state == MMOC_TRANS_STATE_WAIT_PS_DETACH_CNF)
    {
      sanity_timeout = MMOC_PS_DETACH_SANITY_TIME;
    }
    if(mmoc_dev_info->trans == MMOC_TRANS_PROT_REDIR_IND)
    {
      mmoc_dev_info->redir_sanity_timer = sd_bsr_timer_get()*1000/6 + 1000;
      sanity_timeout = mmoc_dev_info->redir_sanity_timer;
    }

    /* Start the sanity timer.
    */
    mmoc_sanity_timer_set(mmoc_info_ptr,sanity_timeout);
  } /* if ( mmoc_info_ptr != MMOC_TRANS_NULL ) */

} /* mmoc_update_curr_trans() */



/*===========================================================================

FUNCTION mmoc_clear_transaction

DESCRIPTION
  This function clears the MMoC's transaction.

RETURN VALUE
 MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
 MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
mmoc_evt_status_e_type          mmoc_clear_transaction
(
  const mmoc_cmd_msg_s_type      *cmd_ptr,

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = NULL;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stack = MM_STACK_NONE;

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if exit fn. needs to be called.
  */
  if ( mmoc_dev_info->exit_fn_ptr )
  {
    /* Invoke exit transaction fn.
    */
    if ( (mmoc_dev_info->exit_fn_ptr)( mmoc_info_ptr ) ==
         MMOC_EVT_STATUS_NOT_CONSUMED )
    {
      /* New transaction needs to be invoked.
      */
      mmoc_dev_info->exit_fn_ptr = NULL;
      return MMOC_EVT_STATUS_NOT_CONSUMED;
    }

  } /*  if ( mmoc_info_ptr->exit_fn_ptr ) */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Need to reset this at the end of transaction */

  if(mmoc_dev_info->trans == MMOC_TRANS_PROT_REDIR_IND)
  {
    mmoc_dev_info->trans_info.redir_ind_info.mm_id.asubs_id = SYS_MODEM_AS_ID_NONE;
    mmoc_dev_info->trans_info.redir_ind_info.mm_id.stk_id = MM_STACK_NONE;
    mmoc_dev_info->trans_info.redir_ind_info.cmd_info.prot_state = PROT_STATE_NULL;
  }

  mmoc_dev_info->dual_switch_subs = SYS_MODEM_AS_ID_NO_ACTIVE_MASK;

#if defined (FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
  /* Need to reset this at the end of transaction */
  if(mmoc_dev_info->trans == MMOC_TRANS_PROT_HO_IND)
  {
    mmoc_dev_info->trans_info.ho_ind_info.mm_id.asubs_id = SYS_MODEM_AS_ID_NONE;
    mmoc_dev_info->trans_info.ho_ind_info.mm_id.stk_id = MM_STACK_NONE;
    mmoc_dev_info->trans_info.ho_ind_info.cmd_info.prot_state = PROT_STATE_NULL;
  }
#endif

  if(mmoc_dev_info->trans == MMOC_TRANS_PROT_GEN_CMD)
  {
    mmoc_dev_info->trans_info.gen_prot_cmd.is_activate_main = FALSE;
  }

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    for(stack = MM_STACK_0; stack < mmoc_sub_info[sub]->nStacks && stack < MAX_STACKS; stack++)
    {
      mmoc_sub_info[sub]->stack_prop[stack]->is_gwl_deact_sent = FALSE;
    }
  }

  /* At end of each transaction, Set prev_ds_pref with new ds_pref value */
  mmoc_dev_info->prev_standby_pref = mmoc_dev_info->standby_pref;
  mmoc_dev_info->prev_active_subs  = mmoc_dev_info->active_subs;
  mmoc_dev_info->prev_device_mode = mmoc_dev_info->device_mode;
  mmoc_dev_info->prev_sub_with_3gpp2 = mmoc_dev_info->sub_with_3gpp2;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Clear the transaction and its state.
  */
  mmoc_update_curr_trans( MMOC_TRANS_NULL,
                          mmoc_info_ptr,
                          SYS_OPRT_MODE_NONE
                        );

  mmoc_update_trans_state( MMOC_TRANS_STATE_NULL,
                           mmoc_info_ptr
                         );

  mmoc_dev_info->exit_fn_ptr = NULL;

  return MMOC_EVT_STATUS_CONSUMED;

} /* mmoc_clear_transaction() */

/*===========================================================================

FUNCTION mmoc_hdr_multimode_sanity_err_fatal

DESCRIPTION
   This function is called since the HDR stack did not respond to
   protocol commands sent by MMOC.
   Response timeout is 60 seconds

===========================================================================*/
static void mmoc_hdr_multimode_sanity_err_fatal(

  mmoc_sanity_err_e_type     reason
)
{
  int sanity_time =
    ( mmoc_state_info.dev_prop.insanity_count * ( MMOC_SANITY_TIME / 1000 ) );


  switch ( reason )
  {
    case MMOC_SANITY_ERR_DEACT_CMD_CNF:
      /* HDR stack did not respond to deactivate cmd.
      */
      MMOC_ERR_FATAL_1("HDR did not respond to deactivate for %ds", sanity_time);
      break;

    case MMOC_SANITY_ERR_PH_STAT_CMD_CNF:
      /* HDR stack did not respond to ph stat chgd cmd
      */
      MMOC_ERR_FATAL_1("HDR did not respond to ph stat chgd for %ds", sanity_time);
      break;

    case MMOC_SANITY_ERR_GENERIC_CMD_CNF:
      /* HDR stack did not respond to gen cmd */
      MMOC_ERR_FATAL_1("HDR did not respond to gen cmd for %ds", sanity_time);
      break;

    default:
      break;

  } /* switch ( reason ) */

}


/*===========================================================================

FUNCTION MMOC_REG_MULTIMODE_SANITY_ERR_FATAL

DESCRIPTION
   This function is called since the GSM/WCDMA/LTE stack did not respond to
   CM_STOP_MODE_REQ or CM_SIM_AVAILABLE_REQ or CM_SIM_NOT_AVAILABLE_REQ.
   Response timeout is 72 seconds(144 seconds for DSDS target).

===========================================================================*/
static void mmoc_reg_multimode_sanity_err_fatal(

  mmoc_sanity_err_e_type     reason
)
{
  int sanity_time = ( mmoc_state_info.dev_prop.insanity_count * ( MMOC_SANITY_TIME / 1000 ) );



  /* This feature is owned by NAS team and used to log
   * MM, EMM, REG variables during crash.
   */
#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
  cmregprx_print_state_variables();
#ifdef MMODE_ADDITIONAL_DEBUG_INFO
  cmregprx_dbg_buf_print_info();
#endif
#endif
  switch ( reason )
  {
    case MMOC_SANITY_ERR_DEACT_CMD_CNF:
      /* NAS(REG Task) stack did not respond to CM_STOP_MODE_REQ.
      */
      MMOC_ERR_FATAL_1("NAS(REG Task) did not respond to CM_STOP_MODE_REQ for %dsec", sanity_time);
      break;

    case MMOC_SANITY_ERR_PH_STAT_CMD_CNF:
      /* NAS(REG Task) stack did not respond to CM_SIM_AVAILABLE_REQ or
      ** CM_SIM_NOT_AVAILABLE_REQ.
      */
      MMOC_ERR_FATAL_1("NAS(REG Task) did not respond to CM_SIM_XXXX_REQ for %dsec", sanity_time);

      break;

    default:
      break;

  } /* switch ( reason ) */

} /* mmoc_reg_multimode_sanity_err_fatal */



/*===========================================================================

FUNCTION mmoc_active_protocol_name

DESCRIPTION
  This function returns the active protocol name.

===========================================================================*/
static char                           *mmoc_active_protocol_name
(
  mm_sub_stk_id_s_type                mm_id,
  /* SD instance
  */
  const mmoc_state_info_s_type       *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  char *s;
  MMOC_ASSERT( mmoc_info_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(mm_id.asubs_id < SYS_MODEM_AS_ID_1 || mm_id.asubs_id >= MAX_SIMS ||
      mm_id.stk_id < MM_STACK_0 || mm_id.stk_id >= MAX_STACKS ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id] == NULL ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id] == NULL)
  {
    s = "Invalid mmid";
    return s;
  }

  /* Determine the active protocol name using the protocol state.
  */
  switch(mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state)
  {

    case PROT_STATE_ONLINE_CDMA:
      s = "MC(CDMA-online)";
      break;

    case PROT_STATE_OFFLINE_CDMA:
      s = "MC(CDMA-offline)";
      break;

    case PROT_STATE_ONLINE_DED_MEAS:
      s = "MC(DED. MEAS)";
      break;

    case PROT_STATE_ONLINE_GWL:
      s = "REG(NAS)";
      break;

    case PROT_STATE_ONLINE_HDR:
      s = "HDR";
      break;


    case PROT_STATE_FTM:
      s = "MC(FTM)";
      break;

    case PROT_STATE_GPSONE_MSBASED:
      s = "GPS-MSBASED";
      break;

    case PROT_STATE_BLOCKED:
    case PROT_STATE_OFFLINE:
    case PROT_STATE_PWR_SAVE:
    case PROT_STATE_RESET:
    case PROT_STATE_LPM:
    case PROT_STATE_PWR_DOWN:
    case PROT_STATE_NULL:
    case PROT_STATE_MAX:
    default:
      s = "No active protocols";
      break;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Return the active protocol name.
  */
  return s;

} /* mmoc_active_protocol_name() */


#ifndef FEATURE_MMOC_NO_SANITY_ERR_FATAL
/*===========================================================================

FUNCTION mmoc_print_active_prot_sanity_err_fatal

DESCRIPTION
  This function calls the sanity_err_fatal function corresponding to the
  active protocol on the given sub/stack.

===========================================================================*/
static void                            mmoc_print_active_prot_sanity_err_fatal
(
  mmoc_sanity_err_e_type             reason,
  /* Reason for "insanity".
  */

  mm_sub_stk_id_s_type               mm_id,
  /* Which active protocol.
  */

  const mmoc_state_info_s_type      *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  MMOC_ASSERT( mmoc_info_ptr != NULL );

  if(mm_id.asubs_id < SYS_MODEM_AS_ID_1 || mm_id.asubs_id >= MAX_SIMS ||
      mm_id.stk_id < MM_STACK_0 || mm_id.stk_id >= MAX_STACKS ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id] == NULL ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id] == NULL)
  {
    return;
  }

  /* Determine the active protocol.
  */
  switch(mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state)
  {
    case PROT_STATE_ONLINE_CDMA:
    case PROT_STATE_OFFLINE_CDMA:
#ifdef MMOC_CDMA_SUPPORTED
      mc_cdma_multimode_sanity_err_fatal( reason );
#endif
      break;

    case PROT_STATE_ONLINE_DED_MEAS:
#ifdef MMOC_CDMA_SUPPORTED
      if(mm_id.asubs_id == mmoc_info_ptr->dev_prop.sub_with_3gpp2)
      {
        mc_cdma_multimode_sanity_err_fatal( reason );
      }
#endif
      break;

    case PROT_STATE_ONLINE_GWL:
#if defined MMOC_GW_SUPPORTED || defined FEATURE_MMOC_LTE
      mmoc_reg_multimode_sanity_err_fatal( reason );
#endif
      break;

    case PROT_STATE_ONLINE_HDR:
#ifdef MMOC_HDR_SUPPORTED
      mmoc_hdr_multimode_sanity_err_fatal( reason );
#endif
      break;


    case PROT_STATE_FTM:
      break;

    case PROT_STATE_BLOCKED:
    case PROT_STATE_GPSONE_MSBASED:
    case PROT_STATE_OFFLINE:
    case PROT_STATE_PWR_SAVE:
    case PROT_STATE_RESET:
    case PROT_STATE_LPM:
    case PROT_STATE_PWR_DOWN:
    case PROT_STATE_NULL:
    case PROT_STATE_MAX:
    default:
      break;
  }

} /* mmoc_print_active_prot_sanity_err_fatal() */

#endif /* FEATURE_MMOC_NO_SANITY_ERR_FATAL */

#ifdef MMODE_ADDITIONAL_DEBUG_INFO
/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_dbg_buf_print_before_err_fatal

DESCRIPTION
  This function displays the mmoc debug buffer as F3 messages.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void                 mmoc_dbg_buf_print_before_err_fatal( void )
{

#ifdef MMODE_ADDITIONAL_DEBUG_INFO_EXT

  int i = 0, j = 0;

  MMOC_MSG_HIGH_1("MMOC DEBUG BUFFER: Current index: %d", mmoc_debug_buffer.dbg_buf_idx);

  for(i = 0 ; i < MMOC_MAX_DEBUG_BUFFER_SIZE ; i++ )
  {
    /* Print a transaction only if it has some valid value
    */
    if(mmoc_debug_buffer.dbg_buf[i].trans_name != MMOC_TRANS_NULL)
    {
      MMOC_MSG_HIGH_3("MMOC DEBUG BUFFER: Transaction[%d] : %d | Trans_ID = %d",
                      i,
                      mmoc_debug_buffer.dbg_buf[i].trans_name,
                      mmoc_debug_buffer.dbg_buf[i].trans_id);

      for(j = 0 ; j < MMOC_MAX_RPT_COUNT; j++)
      {
        /* Print a report only if it is not same as intial defult value
        */
        if(mmoc_debug_buffer.dbg_buf[i].rpt_queue[j].rpt_name != MMOC_RPT_MAX)
        {
          MMOC_MSG_HIGH_3("MMOC DEBUG BUFFER: Report[%d] : %d from task %d",
                          j,
                          mmoc_debug_buffer.dbg_buf[i].rpt_queue[j].rpt_name,
                          mmoc_debug_buffer.dbg_buf[i].rpt_queue[j].task_name);

#if defined(FEATURE_MMODE_TRIPLE_SIM) || defined(FEATURE_MMODE_SXLTE_G)
          MMOC_MSG_HIGH_4("MMOC DEBUG BUFFER:   Prot States : [MAIN] %d | [HYBR_GW] %d | [HYBR_GW3] %d HYBR_HDR] %d",
                          mmoc_debug_buffer.dbg_buf[i].rpt_queue[j].prot_state[SD_SS_MAIN],
                          mmoc_debug_buffer.dbg_buf[i].rpt_queue[j].prot_state[SD_SS_HYBR_2],
                          mmoc_debug_buffer.dbg_buf[i].rpt_queue[j].prot_state[SD_SS_HYBR_3],
                          mmoc_debug_buffer.dbg_buf[i].rpt_queue[j].prot_state[SD_SS_HYBR_1]);
#else
          MMOC_MSG_HIGH_3(" MMOC DEBUG BUFFER:  Prot States : [MAIN] %d | [HYBR_GW] %d | [HYBR_HDR] %d",
                          mmoc_debug_buffer.dbg_buf[i].rpt_queue[j].prot_state[SD_SS_MAIN],
                          mmoc_debug_buffer.dbg_buf[i].rpt_queue[j].prot_state[SD_SS_HYBR_2],
                          mmoc_debug_buffer.dbg_buf[i].rpt_queue[j].prot_state[SD_SS_HYBR_1]);
#endif
        }
      }
    }
  }

#endif /* MMODE_ADDITIONAL_DEBUG_INFO_EXT */

}/* mmoc_dbg_buf_print_before_err_fatal() */
#endif /* MMODE_ADDITIONAL_DEBUG_INFO */

static void mmoc_print_pwrup_parameters(mmoc_state_info_s_type *mmoc_info_ptr)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  MMOC_MSG_HIGH_6("go pwrup fmode %d device_mode %d standby_pref %d prev_active_subs %d active_subs %d sub_with_3gpp2 %d",
                  mmoc_dev_info->onebuild_feature,
                  mmoc_dev_info->device_mode,
                  mmoc_dev_info->standby_pref,
                  mmoc_dev_info->prev_active_subs,
                  mmoc_dev_info->active_subs,
                  mmoc_dev_info->sub_with_3gpp2);

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    MMOC_MSG_HIGH_5("go pwrup as_id %d sfmode %d ps_enabled %d prot_subsc_chg %d sub_avail 0x%x",
                    sub,
                    mmoc_sub_info[sub]->subs_feature,
                    mmoc_sub_info[sub]->ps_enabled,
                    mmoc_sub_info[sub]->prot_subsc_chg,
                    mmoc_sub_info[sub]->sub_avail);
  }



}


/*===========================================================================

FUNCTION mmoc_display_debug_info

DESCRIPTION
  This function displays the debug info as F3 messages.

===========================================================================*/
static void                            mmoc_display_debug_info
(
  const mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
#ifdef FEATURE_MODEM_HEAP_PHASE_2
  mmoc_debug_str = (char *) mmoc_mem_alloc (MMOC_MAX_CHAR);
#endif

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
#ifdef MMOC_STRING_MAPS_ACTIVE
      (void) snprintf( mmoc_debug_str, sizeof(mmoc_debug_str),
                       "Deactivate request sent to prot %s ",
                       mmoc_active_protocol_name( mmoc_info_ptr->dev_prop.deact_req_idx,
                           mmoc_info_ptr )
                     );
      MSG_SPRINTF_1(MSG_SSID_DFLT, MSG_LEGACY_ERROR, "%s", mmoc_debug_str);
#else
      MMOC_MSG_HIGH_1("Deactivate request sent to prot %d",
                      mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id]->stack_prop[mmoc_info_ptr->dev_prop.deact_req_idx.stk_id]->prot_state);
#endif
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL:
#ifdef MMOC_STRING_MAPS_ACTIVE
      (void) snprintf( mmoc_debug_str, sizeof(mmoc_debug_str),
                       "GWL Deactivate request sent to sub %d stk %d",
                       mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id,
                       mmoc_info_ptr->dev_prop.deact_req_idx.stk_id);
      MSG_SPRINTF_1(MSG_SSID_DFLT, MSG_LEGACY_ERROR, "%s", mmoc_debug_str);
#else
      MMOC_MSG_HIGH_2("GWL Deactivate request sent to sub %d stk %d",
                      mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id,
                      mmoc_info_ptr->dev_prop.deact_req_idx.stk_id);
#endif
      break;

    case MMOC_TRANS_STATE_WAIT_PH_STAT_CNF:
    {
      int i;
      /* Search for pending confirmation.
      */
      for (i=0; i < (int)MMOC_MODE_MAX; i++ )
      {
        if ( ph_stat_cmd_tbl[i].trans_id != PROT_TRANS_NONE )
        {
#ifdef MMOC_STRING_MAPS_ACTIVE
          (void) snprintf( mmoc_debug_str, sizeof(mmoc_debug_str),
                           "Ph. Status request sent to %s",
                           ph_stat_cmd_tbl[i].task_name
                         );
          MSG_SPRINTF_1(MSG_SSID_DFLT, MSG_LEGACY_ERROR, "%s", mmoc_debug_str);
#else
          MMOC_MSG_HIGH_1("Ph. Status request sent to ph_stat_cmd_tbl[%d]",i);
#endif
          /* there is pending cnf.
          */
          break;
        }
      }

    }
    break;

    case MMOC_TRANS_STATE_WAIT_DS_STAT_CHGD_CNF:
      MMOC_MSG_HIGH_0("DS stat chgd request sent to GW");
      break;

    case MMOC_TRANS_STATE_WAIT_SUBS_CAP_CHGD_CNF:
      MMOC_MSG_HIGH_0("SUBS CAP chgd request sent to GW");

      break;

    case MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF:
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.asubs_id;
      mm_id.stk_id   = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.stk_id;

#ifdef MMOC_STRING_MAPS_ACTIVE
      (void) snprintf( mmoc_debug_str, sizeof(mmoc_debug_str),
                       "Generic cmd request sent to prot %s ",
                       mmoc_active_protocol_name(
                         mm_id,
                         mmoc_info_ptr )
                     );
      MSG_SPRINTF_1(MSG_SSID_DFLT, MSG_LEGACY_ERROR, "%s", mmoc_debug_str);
#else
      MMOC_MSG_HIGH_1("Generic cmd request sent to prot %d",
                      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state);
#endif
    }
    break;

    case MMOC_TRANS_STATE_WAIT_AUTO_DEACTD_IND:
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.asubs_id;
      mm_id.stk_id   = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.stk_id;

#ifdef MMOC_STRING_MAPS_ACTIVE
      (void) snprintf( mmoc_debug_str, sizeof(mmoc_debug_str),
                       "Wait for deactd ind from prot %s",
                       mmoc_active_protocol_name(
                         mm_id,
                         mmoc_info_ptr )
                     );
      MSG_SPRINTF_1(MSG_SSID_DFLT, MSG_LEGACY_ERROR, "%s", mmoc_debug_str);
#else
      MMOC_MSG_HIGH_1("Wait for deactd ind from prot %d",
                      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state);
#endif
    }
    break;

    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
      MMOC_MSG_HIGH_0("Deactivate req. entry state");
      break;

    case MMOC_TRANS_STATE_PROT_PH_STAT_ENTER:
      MMOC_MSG_HIGH_0("Phone status req. entry state");
      break;

    case MMOC_TRANS_STATE_GEN_CMD_ENTER:
      MMOC_MSG_HIGH_0("Generic cmd req. entry state");
      break;

    case MMOC_TRANS_STATE_NULL:
      MMOC_MSG_HIGH_0("In NULL transaction state");
      break;

    case MMOC_TRANS_STATE_MAX:
    default:
      MMOC_MSG_HIGH_1("Invalid transaction state %d",mmoc_info_ptr->dev_prop.trans_state);
      break;
  } /* switch ( mmoc_info_ptr->trans_state ) */


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_MODEM_HEAP_PHASE_2
  modem_mem_free ((void *)mmoc_debug_str, MODEM_MEM_CLIENT_MMODE);
#endif

} /* mmoc_display_debug_info() */

#ifndef FEATURE_MMOC_NO_SANITY_ERR_FATAL
/*===========================================================================

FUNCTION mmoc_print_sanity_err_fatal

DESCRIPTION
  This function calls the insanity timeout err_fatal.

===========================================================================*/
static void                            mmoc_print_sanity_err_fatal
(
  const mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  word    err_code_no;
  int     i = 0;
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stk = MM_STACK_NONE;

  MMOC_ASSERT(mmoc_info_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
  /* Log NAS debug info */
  mm_log_mm_reg_variables();
#endif

#ifdef MMODE_ADDITIONAL_DEBUG_INFO
  /* Print mmoc_debug_buffer information */
  mmoc_dbg_buf_print_before_err_fatal();
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
      mmoc_print_active_prot_sanity_err_fatal(MMOC_SANITY_ERR_DEACT_CMD_CNF,
                                              mmoc_info_ptr->dev_prop.deact_req_idx,
                                              mmoc_info_ptr);
      err_code_no = (word)__LINE__;
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL:
      MMOC_ERR_FATAL_3("MMOC sanity expired, trans_state %d NAS did not respond to deact sub %d stk %d",
                       mmoc_info_ptr->dev_prop.trans_state,
                       mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id,
                       mmoc_info_ptr->dev_prop.deact_req_idx.stk_id);
      err_code_no = (word)__LINE__;
      break;

    case MMOC_TRANS_STATE_WAIT_PH_STAT_CNF:
    {
      int i;
      /* Search for pending confirmation */
      for(i=0; i < (int)MMOC_MODE_MAX; i++)
      {
        if(ph_stat_cmd_tbl[i].trans_id != PROT_TRANS_NONE)
        {
          if(ph_stat_cmd_tbl[i].sanity_err_fn)
          {
            (*ph_stat_cmd_tbl[i].sanity_err_fn)
            (MMOC_SANITY_ERR_PH_STAT_CMD_CNF);
          }
          /* There is pending cnf */
          break;
        }
      }
      err_code_no = (word)__LINE__;
    }
    break;

    case MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF:
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.asubs_id;
      mm_id.stk_id   = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.stk_id;

      mmoc_print_active_prot_sanity_err_fatal(MMOC_SANITY_ERR_GENERIC_CMD_CNF,
                                              mm_id,
                                              mmoc_info_ptr);
      err_code_no = (word)__LINE__;
    }
    break;


    case MMOC_TRANS_STATE_WAIT_AUTO_DEACTD_IND:
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.asubs_id;
      mm_id.stk_id   = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.stk_id;

      mmoc_print_active_prot_sanity_err_fatal(MMOC_SANITY_ERR_GENERIC_CMD_CNF,
                                              mm_id,
                                              mmoc_info_ptr);
      err_code_no = (word)__LINE__;
    }
    break;

    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
      err_code_no = (word)__LINE__;
      break;

    case MMOC_TRANS_STATE_PROT_PH_STAT_ENTER:
      err_code_no = (word)__LINE__;
      break;

    case MMOC_TRANS_STATE_GEN_CMD_ENTER:
      err_code_no = (word)__LINE__;
      break;

    case MMOC_TRANS_STATE_NULL:
      err_code_no = (word)__LINE__;
      break;

    case MMOC_TRANS_STATE_MMGSDI_READ_CNF:
      err_code_no = (word)__LINE__;
      break;

    case MMOC_TRANS_STATE_WAIT_AUTO_ACTD_IND:
      err_code_no = (word)__LINE__;
      break;

    case MMOC_TRANS_STATE_WAIT_PS_DETACH_CNF:
      err_code_no = (word)__LINE__;
      break;

    case MMOC_TRANS_STATE_WAIT_SESSION_OPEN_CNF:
      err_code_no = (word)__LINE__;
      break;

    case MMOC_TRANS_STATE_WAIT_1XCSFB_DEACT_CNF:
      err_code_no = (word)__LINE__;
      break;

    case MMOC_TRANS_STATE_WAIT_UE_MODE_SWITCH:
      err_code_no = (word)__LINE__;
      break;

    case MMOC_TRANS_STATE_MAX:
    default:
      err_code_no = (word)__LINE__;
      break;
  } /* switch(mmoc_info_ptr->trans_state) */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Always call MMOC_ERR_FATAL, even if not in debug */
  MMOC_ERR_FATAL_2("MMOC sanity expired, trans_state %d err_code %d",
                   mmoc_info_ptr->dev_prop.trans_state,
                   err_code_no);

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    for(stk = MM_STACK_0; stk < mmoc_info_ptr->sub_prop[sub]->nStacks && stk < MAX_STACKS; stk++)
    {
      MMOC_MSG_HIGH_3( "sub %d stk %d prot_state %d",
                       sub, stk,
                       mmoc_info_ptr->sub_prop[sub]->stack_prop[stk]->prot_state);
    }
  }

} /* mmoc_print_sanity_err_fatal() */
#endif /* FEATURE_MMOC_NO_SANITY_ERR_FATAL */


/*===========================================================================

FUNCTION mmoc_set_sd_initialized

DESCRIPTION
  This function sets the MMoC's state to SD initialized.

===========================================================================*/
static void                            mmoc_set_sd_initialized
(

  mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  MMOC_ASSERT( mmoc_info_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_MSG_LOW_2("sd_initialized(),curr_trans %d,trans_state %d",
                 mmoc_info_ptr->dev_prop.trans,
                 mmoc_info_ptr->dev_prop.trans_state);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Set the MMoC's is_sd_initialized to TRUE,
  */
  mmoc_info_ptr->dev_prop.is_sd_initialized = TRUE;

} /* mmoc_set_sd_initialized() */

#ifdef MMODE_ADDITIONAL_DEBUG_INFO
/*===========================================================================

FUNCTION mmoc_dbg_buf_map_rex_name_to_mmoc

DESCRIPTION
  This function maps the task_ name returned by REX to MMOC's TASK_NAME_ENUM.

DEPENDENCIES
  None.

RETURN VALUE
  mmoc Task Name

SIDE EFFECTS
  None.
===========================================================================*/
static mmoc_rpt_task_name_e_type mmoc_dbg_buf_map_rex_name_to_mmoc
(

  char         *rex_task_name


)
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* No need to compare rex_task_name in case it is an empty string
  */
  if(strlen(rex_task_name) != 0)
  {
    /* Switch on the rex task name
    */
    if(strcmp(rex_task_name, "mmoc") == 0)
    {
      return MMOC_TASK_NAME_MMOC;
    }
    if(strcmp(rex_task_name, "cm") == 0)
    {
      return MMOC_TASK_NAME_CM;
    }
    if(strcmp(rex_task_name, "mc") == 0)
    {
      return MMOC_TASK_NAME_CDMA;
    }
    if(strcmp(rex_task_name, "hdrmc") == 0)
    {
      return MMOC_TASK_NAME_HDR;
    }
    if((strcmp(rex_task_name, "gsdi") == 0)
        ||(strcmp(rex_task_name, "mmgsdi_1") == 0)
        ||(strcmp(rex_task_name, "mmgsdi_2") == 0)
        ||(strcmp(rex_task_name, "mmgsdi_3") == 0)
      )
    {
      return MMOC_TASK_NAME_MMGSDI;
    }

  }

  return MMOC_TASK_NAME_MAX;

} /* mmoc_dbg_buf_map_rex_name_to_mmoc() */
#endif /* MMODE_ADDITIONAL_DEBUG_INFO */


/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_map_sys_oprt_mode_to_mmoc

DESCRIPTION
  This function sets the MMoC's state to SD uninitialized.

DEPENDENCIES
  None.

RETURN VALUE
  mmoc type operating mode status.

SIDE EFFECTS
  None.
===========================================================================*/
static prot_oprt_mode_e_type           mmoc_map_sys_oprt_mode_to_mmoc
(

  sys_oprt_mode_e_type               sys_oprt_mode
  /* Operating mode - SYS type.
  */
)
{
  prot_oprt_mode_e_type  mmoc_oprt_mode = PROT_OPRT_MODE_MAX;

  MMOC_ASSERT_ENUM_IS_INRANGE( sys_oprt_mode, SYS_OPRT_MODE_MAX );


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Switch on the sys_oprt_mode.
  */
  switch ( sys_oprt_mode )
  {
    case SYS_OPRT_MODE_PWROFF:
      mmoc_oprt_mode = PROT_OPRT_MODE_PWROFF;
      break;

    case SYS_OPRT_MODE_OFFLINE:
      mmoc_oprt_mode = PROT_OPRT_MODE_OFFLINE;
      break;

    case SYS_OPRT_MODE_OFFLINE_CDMA:
      mmoc_oprt_mode = PROT_OPRT_MODE_OFFLINE_CDMA;
      break;

    case SYS_OPRT_MODE_ONLINE:
      mmoc_oprt_mode = PROT_OPRT_MODE_ONLINE;
      break;

    case SYS_OPRT_MODE_FTM:
      mmoc_oprt_mode = PROT_OPRT_MODE_FTM;
      break;

    case SYS_OPRT_MODE_RESET:
    case SYS_OPRT_MODE_RESET_MODEM:
      mmoc_oprt_mode = PROT_OPRT_MODE_RESET;
      break;


    case SYS_OPRT_MODE_LPM:
      mmoc_oprt_mode = PROT_OPRT_MODE_LPM;
      break;

    case SYS_OPRT_MODE_NONE:
    case SYS_OPRT_MODE_NET_TEST_GW:
    case SYS_OPRT_MODE_OFFLINE_IF_NOT_FTM:
    case SYS_OPRT_MODE_PSEUDO_ONLINE:
    case SYS_OPRT_MODE_MAX:
#ifdef FEATURE_RPC
    case SYS_OPRT_MODE_BIG:
#endif /* FEATURE_RPC */

    default:
      sys_err_fatal_invalid_value_exception((int)sys_oprt_mode);
      break;

  } /* switch( sys_oprt_mode ) */


  return mmoc_oprt_mode;

} /* mmoc_map_sys_oprt_mode_to_mmoc() */


/*===========================================================================

FUNCTION mmoc_accept_cmds

DESCRIPTION
  This function will determine if the commands have to be dequeued and
  processed. Based on the return value, the REX signal corresponding to the
  command queue has to be set or reset.

RETURN VALUE
  TRUE  - MMoC can accept commands.
  FALSE - do not accept any commands at this time.

===========================================================================*/
static boolean                         mmoc_accept_cmds
(
  const mmoc_state_info_s_type    *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  MMOC_ASSERT( mmoc_info_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if transaction is in progress.
  */
  if ( mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_NULL )
  {
    /* No transaction in progress, so can process next command.
    */
    return TRUE;
  }

  return FALSE;

} /* mmoc_accept_cmds() */


/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_is_prot_deactivating

DESCRIPTION
  This function will determine if the protocol needs to be deactivated based
  on the SS action.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE  -  Protocol is deactivating.
  FALSE -  No protocol is not deactivating.

SIDE EFFECTS
  None.
===========================================================================*/
static boolean                         mmoc_is_prot_deactivating
(
  prot_state_e_type               prot_state,
  /* Active protocol.
  */

  sd_ss_act_e_type                next_act
  /* Next SD SS-Action.
  */
)
{
  boolean is_prot_deactivating = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Check if protocol should deactivate.
  */
  switch ( prot_state )
  {
    case PROT_STATE_ONLINE_CDMA:
      if ( next_act != SD_SS_ACT_ACQ_CDMA  &&
           next_act != SD_SS_ACT_CONTINUE )
      {
        is_prot_deactivating = TRUE;
      }
      break;

    case PROT_STATE_ONLINE_HDR:
      if ( next_act != SD_SS_ACT_ACQ_HDR  &&
           next_act != SD_SS_ACT_CONTINUE )
      {
        is_prot_deactivating = TRUE;
      }
      break;

    case PROT_STATE_ONLINE_GWL:
      if ( next_act != SD_SS_ACT_ACQ_GWL  &&
           next_act != SD_SS_ACT_GET_NET_GW  &&
           next_act != SD_SS_ACT_CONTINUE
#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
           && next_act != SD_SS_ACT_MEAS_BACK
#endif
         )
      {
        is_prot_deactivating = TRUE;
      }
      break;

    case PROT_STATE_ONLINE_DED_MEAS:
      if ( next_act != SD_SS_ACT_MEAS_DED &&
           next_act != SD_SS_ACT_CONTINUE )
      {
        is_prot_deactivating = TRUE;
      }


      break;


    case PROT_STATE_BLOCKED:
    case PROT_STATE_NULL:
    default:
      break;

  } /* switch( act_protocol ) */


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* return.
  */
  return is_prot_deactivating;
  /*lint -esym(715,ss) */ /* not referenced */

} /* mmoc_is_prot_deactivating() */


/* <EJECT> */
/*===========================================================================

FUNCTION  mmoc_send_cm_rpt

DESCRIPTION
  This is a utility function called by the transaction state handlers to send
  the report to CM.

  Note! Only CM reports with no parameters can use this function.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                            mmoc_send_cm_rpt
(
  cm_name_type                         rpt_name,
  /* Type of report to send.
  */

  const cm_sd_rpt_u_type                *mmoc_rpt_ptr,
  /* Payload of report to send.
  */

  sys_modem_as_id_e_type               asubs_id
)
{

  /* Pointer to CM report buffer. */
  cm_sd_rpt_u_type *rpt_ptr;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get the message buffer.
  */
  rpt_ptr = cm_sd_rpt_get_buf_else_err_fatal();


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Fill in the message buffer and place it in CM report queue.
  */
  MMOC_MSG_HIGH_2("Sending CM report %d, sub %d", rpt_name, asubs_id);

  /* copy payload contents
  */
  if(mmoc_rpt_ptr != NULL)
  {
    switch(rpt_name)
    {
      case CM_SUSPEND_SS_F:
        rpt_ptr->suspend_ss.is_suspend = mmoc_rpt_ptr->suspend_ss.is_suspend;
        rpt_ptr->suspend_ss.stack_id         = mmoc_rpt_ptr->suspend_ss.stack_id;
        break;

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case CM_SUBS_CHGD_DONE:
        rpt_ptr->sub_done.active_stk_bm = mmoc_rpt_ptr->sub_done.active_stk_bm;
        break;

      case CM_MCFG_PROCEED_WITH_REFRESH_F:
        rpt_ptr->mcfg_proceed_with_refresh.active_subs = mmoc_rpt_ptr->mcfg_proceed_with_refresh.active_subs;
        break;

      case CM_HOLD_ACQ:
        rpt_ptr->hold_acq.online_hold_acq = mmoc_rpt_ptr->hold_acq.online_hold_acq;
        break;
        
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      case CM_MMOC_SUBSCRIPTION_FAILURE:
      default:
        break;
    }
  }

  /* copy rpt name
  */
  rpt_ptr->hdr.cmd        = rpt_name;
  rpt_ptr->hdr.asubs_id   = asubs_id;

  /* queue report to CM
  */
  cm_sd_rpt(rpt_ptr);


} /* mmoc_send_cm_rpt() */

/*===========================================================================

FUNCTION  mmoc_msgr_send_else_err_fatal

DESCRIPTION
  This is a utility function that initializes message header and sends the
  message through MSGR primitives.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void mmoc_msgr_send_else_err_fatal
(
  msgr_umid_type             umid,

  msgr_hdr_struct_type      *msg_ptr,
  /* Pointer to message to be sent. Note that the first member of the message
   *  that is pointed to by this pointer must be of msgr_hdr_struct_type type
   */
  uint32                     msg_len,
  /* Total message length (header and payload) in bytes */

  sys_modem_as_id_e_type     asubs_id
)
{
  errno_enum_type msgr_error = E_SUCCESS;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( msg_ptr != NULL )
  {
    msgr_init_hdr_variant( msg_ptr, MSGR_MM_MMOC, umid, asubs_id );

#ifdef TEST_FRAMEWORK
    #error code not present
#else
    msgr_error = msgr_send( msg_ptr, msg_len );
#endif

    if ( msgr_error != E_SUCCESS )
    {
      MMOC_ERR_FATAL_2( "MSGR send error %d to %d", msgr_error, umid);
    }


  }
} /* mmoc_msgr_send_else_err_fatal */

#ifdef MMOC_HDR_SUPPORTED
/*===========================================================================

FUNCTION  mmoc_deactivate_hdrmc_protocol

DESCRIPTION
  Sends deactivate request to HDR CP through message router.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void mmoc_send_hdr_deactivate_protocol
(
  prot_deact_e_type deact_reason,

  prot_trans_type   trans_id,

  sys_modem_as_id_e_type  asubs_id

)
{
  hdrcp_msg_req_u      *hdrcp_req;

  hdrcp_req = (hdrcp_msg_req_u *) mmoc_mem_alloc(sizeof(hdrcp_msg_req_u));

  if(hdrcp_req == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  hdrcp_req->deactivate_protocol.deact_reason = deact_reason;
  hdrcp_req->deactivate_protocol.trans_id = trans_id;

  MMOC_MSG_HIGH_0("deactivate HDR tunnel");
  mmoc_msgr_send_else_err_fatal( HDR_CP_PROT_DEACT_REQ,
                                 &(hdrcp_req->hdr),
                                 sizeof(hdrcp_msg_req_u),
                                 asubs_id);

  mmoc_mem_free(hdrcp_req);
  return;

}

#ifdef MMOC_LTE_TO_HDR
/*===========================================================================

FUNCTION  mmoc_send_hdr_protocol_activate_cnf

DESCRIPTION
  Sends confirmation for deactivate request to HDR CP through message router.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void mmoc_send_hdr_protocol_activate_cnf
(
  prot_act_e_type      act_reason
)
{
  hdrcp_msg_req_u      *hdrcp_req;

  hdrcp_req = (hdrcp_msg_req_u *) mmoc_mem_alloc(sizeof(hdrcp_msg_req_u));

  if(hdrcp_req == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  hdrcp_req->activate_protocol_cnf.act_reason = act_reason;

  MMOC_MSG_HIGH_0("send activate cnf to HDR MC");
  mmoc_msgr_send_else_err_fatal( HDR_CP_ACTIVATE_PROTOCOL_CNF_REQ,
                                 &(hdrcp_req->hdr),
                                 sizeof(hdrcp_msg_req_u),
                                 mmoc_get_state_info_ptr()->dev_prop.sub_with_3gpp2);

  mmoc_mem_free(hdrcp_req);
  return;

}
#endif /* MMOC_LTE_TO_HDR */

/*===========================================================================

FUNCTION  mmoc_send_hdr_unlock_rf

DESCRIPTION
  Sends request unlock hdr rf to HDR MC

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void mmoc_send_hdr_unlock_rf( void )
{
  hdrcp_msg_req_u      *hdrcp_req;

  hdrcp_req = (hdrcp_msg_req_u *) mmoc_mem_alloc(sizeof(hdrcp_msg_req_u));

  if(hdrcp_req == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  MMOC_MSG_HIGH_0("send hdrmc hdr unlock rf");
  mmoc_msgr_send_else_err_fatal( HDR_CP_UNLOCK_HDR_RF_REQ,
                                 &(hdrcp_req->hdr),
                                 sizeof(hdrcp_msg_req_u),
                                 mmoc_get_state_info_ptr()->dev_prop.sub_with_3gpp2);

  mmoc_mem_free(hdrcp_req);
  return;

}

/*===========================================================================
FUNCTION mmoc_send_hdr_ph_status_chgd

DESCRIPTION
  Sends change in phone status to HDR protocol.
  Initiates the housekeeping tasks if necessary.

DEPENDENCIES
  None.

RETURN VALUE
  void.

SIDE EFFECTS
  None.
===========================================================================*/
static void mmoc_send_hdr_ph_status_chgd
(
  prot_ph_stat_chg_e_type         chg_type,
  /* Type of phone status change( NAM change, operating mode change
  ** etc.)
  */

  byte                            nam,
  /* Current NAM selected by the user.
  */

  boolean                         is_cdma_subs_avail,
  /* Current CDMA subscription availability status.
  */

  prot_oprt_mode_e_type            oprt_mode,
  /* Current operating mode of the phone.
  */

  prot_trans_type                  trans_id,
  /* Transaction Id for this request.
  */

  prot_subsc_chg_e_type            prot_subsc_chg
  /* Enumeration of protocols that have a change in
  ** subscription available status. Just an api change
  ** here not being used in HDR.
  */
)
{
  hdrcp_msg_req_u      *hdrcp_req;

  hdrcp_req = (hdrcp_msg_req_u *) mmoc_mem_alloc(sizeof(hdrcp_msg_req_u));

  if(hdrcp_req == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  hdrcp_req->ph_status_chgd.chg_type = chg_type;
  hdrcp_req->ph_status_chgd.nam = nam;
  hdrcp_req->ph_status_chgd.is_cdma_subs_avail = is_cdma_subs_avail;
  hdrcp_req->ph_status_chgd.oprt_mode = oprt_mode;
  hdrcp_req->ph_status_chgd.trans_id = trans_id;
  hdrcp_req->ph_status_chgd.prot_subsc_chg = prot_subsc_chg;

  MMOC_MSG_HIGH_0("send HDR_CP_PROT_PH_STAT_CHGD_REQ");
  mmoc_msgr_send_else_err_fatal( HDR_CP_PROT_PH_STAT_CHGD_REQ,
                                 &(hdrcp_req->hdr),
                                 sizeof(hdrcp_msg_req_u),
                                 mmoc_get_state_info_ptr()->dev_prop.sub_with_3gpp2);

  mmoc_mem_free(hdrcp_req);
  return;
}

#ifdef MMOC_LTE_TO_HDR
/*===========================================================================
FUNCTION mmoc_send_hdr_redir_protocol_cnf

DESCRIPTION
  Send redirection conf from MSG router

DEPENDENCIES
  None.

RETURN VALUE
  void.

SIDE EFFECTS
  None.
===========================================================================*/

static void mmoc_send_hdr_redir_protocol_cnf
(
  prot_act_e_type act_reason,
  /* Activate reason for protocol */

  boolean         is_irat_ho_allowed
  /* Response to HDR MC if IRAT is allowed */
)
{
  hdrcp_msg_req_u      *hdrcp_req;

  hdrcp_req = (hdrcp_msg_req_u *) mmoc_mem_alloc(sizeof(hdrcp_msg_req_u));

  if(hdrcp_req == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  hdrcp_req->redir_protocol_cnf.act_reason = act_reason;
  hdrcp_req->redir_protocol_cnf.is_irat_ho_allowed = is_irat_ho_allowed;

  MMOC_MSG_HIGH_0("send HDR_CP_REDIR_PROTOCOL_CNF_REQ");
  mmoc_msgr_send_else_err_fatal( HDR_CP_REDIR_PROTOCOL_CNF_REQ,
                                 &(hdrcp_req->hdr),
                                 sizeof(hdrcp_msg_req_u),
                                 mmoc_get_state_info_ptr()->dev_prop.sub_with_3gpp2);

  mmoc_mem_free(hdrcp_req);
  return;
}
#endif /* MMOC_LTE_TO_HDR */

/* <EJECT> */

/*===========================================================================
FUNCTION mmoc_hdr_act_poll

DESCRIPTION
  POLL NAS for HDR, T/W serialization

DEPENDENCIES
  None.

RETURN VALUE
  void.

SIDE EFFECTS
  None.
===========================================================================*/

static void mmoc_hdr_act_poll(void)
{

  sys_activation_allowed_e_type mmoc_activation_status;
  uint8 mmoc_hdr_activation_wait_counter = 1;

  sys_modem_as_id_e_type asubs_id = SYS_MODEM_AS_ID_NONE;

  // Getting the other sub_id 
  asubs_id = mmoc_get_other_sub(mmoc_get_state_info_ptr()->dev_prop.sub_with_3gpp2);
  
  mmoc_activation_status = mm_per_subs_is_DO_activation_allowed (asubs_id);

  while(mmoc_hdr_activation_wait_counter < MMOC_MAX_HDR_ACTIVATION_WAIT)
  {

    if(mmoc_activation_status == SYS_ACTIVATION_ALLOWED_WAIT_AND_CONTINUE)
    {
      mmoc_kick_dog();
      timer_sleep(1000,T_MSEC,TRUE);
      break;
    }
    else if(mmoc_activation_status == SYS_ACTIVATION_ALLOWED_CONTINUE)
    {
      break;
    }
    else if(mmoc_activation_status == SYS_ACTIVATION_ALLOWED_WAIT)
    {
      mmoc_kick_dog();
      mmoc_hdr_activation_wait_counter++;
      timer_sleep(2000,T_MSEC,TRUE);
      mmoc_activation_status = mm_per_subs_is_DO_activation_allowed (asubs_id);
    }

  }

  if(mmoc_hdr_activation_wait_counter == MMOC_MAX_HDR_ACTIVATION_WAIT)
  {
    MMOC_ERR_1("mmoc_hdr_activation_wait_counter=%d reaches maximum",mmoc_hdr_activation_wait_counter);
  }

  MMOC_MSG_HIGH_2("HDR_ACT_POLL: returned %d for asubs_id %d", mmoc_activation_status, asubs_id);
  return;
}

/*===========================================================================
FUNCTION mmoc_send_hdr_activate_protocol

DESCRIPTION
  Send activate protocol command to HDR MC

DEPENDENCIES
  None.

RETURN VALUE
  void.

SIDE EFFECTS
  None.
===========================================================================*/
static void mmoc_send_hdr_activate_protocol
(
  prot_act_e_type act_reason
)
{
  hdrcp_msg_req_u      *hdrcp_req;
  mmoc_state_info_s_type *mmoc_info_ptr = mmoc_get_state_info_ptr();

  hdrcp_req = (hdrcp_msg_req_u *) mmoc_mem_alloc(sizeof(hdrcp_msg_req_u));

  /* Perform HDR polling only when the UE is in Multi standby 
  */
  
  if(mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM) &&
     mmoc_is_multi_standby(mmoc_info_ptr->dev_prop.standby_pref))
  {
    mmoc_hdr_act_poll();
  }

  if(hdrcp_req == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  hdrcp_req->activate_protocol.act_reason = act_reason;

  MMOC_MSG_HIGH_0("send HDR_CP_PROT_ACT_REQ");
  mmoc_msgr_send_else_err_fatal( HDR_CP_PROT_ACT_REQ,
                                 &(hdrcp_req->hdr),
                                 sizeof(hdrcp_msg_req_u),
                                 mmoc_get_state_info_ptr()->dev_prop.sub_with_3gpp2);

  mmoc_mem_free(hdrcp_req);
  return;
}

/*===========================================================================
FUNCTION mmoc_send_hdr_generic_prot_cmd

DESCRIPTION
  Send generic protocol command to HDR MC

DEPENDENCIES
  None.

RETURN VALUE
  void.

SIDE EFFECTS
  None.
===========================================================================*/
static void mmoc_send_hdr_generic_prot_cmd
(
  const prot_gen_cmd_s_type      *gen_cmd_ptr
)
{
  hdrcp_msg_req_u      *hdrcp_req;

  hdrcp_req = (hdrcp_msg_req_u *) mmoc_mem_alloc(sizeof(hdrcp_msg_req_u));

  if(hdrcp_req == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  hdrcp_req->generic_prot.info = *gen_cmd_ptr;

  MMOC_MSG_HIGH_0("send HDR_CP_HDR_CP_PROT_GEN_REQ");
  mmoc_msgr_send_else_err_fatal( HDR_CP_PROT_GEN_REQ,
                                 &(hdrcp_req->hdr),
                                 sizeof(hdrcp_msg_req_u),
                                 mmoc_get_state_info_ptr()->dev_prop.sub_with_3gpp2);

  mmoc_mem_free(hdrcp_req);
  return;
}

#ifdef FEATURE_MMODE_DUAL_SIM

/*===========================================================================
FUNCTION mmoc_send_hdr_standby_pref_chgd_cmd

DESCRIPTION
  Send activate protocol command to HDR MC

DEPENDENCIES
  None.

RETURN VALUE
  void.

SIDE EFFECTS
  None.
===========================================================================*/
static void mmoc_send_hdr_standby_pref_chgd_cmd
(
  prot_trans_type trans_id,
  sys_modem_dual_standby_pref_e_type ds_pref

)
{
  hdrcp_msg_req_u      *hdrcp_req;

  hdrcp_req = (hdrcp_msg_req_u *) mmoc_mem_alloc(sizeof(hdrcp_msg_req_u));

  if(hdrcp_req == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  hdrcp_req->sys_standby_cmd.trans_id = trans_id;
  hdrcp_req->sys_standby_cmd.info  = ds_pref ;

  MMOC_MSG_HIGH_0("send HDR_CP_STANDBY_PREF_CHGD_REQ");
  mmoc_msgr_send_else_err_fatal( HDR_CP_STANDBY_PREF_CHGD_REQ,
                                 &(hdrcp_req->hdr),
                                 sizeof(hdrcp_msg_req_u),
                                 MSGR_NO_VARIANT);

  mmoc_mem_free(hdrcp_req);
  return;
}
#endif

#endif /* MMOC_HDR_SUPPORTED */


/*===========================================================================

FUNCTION mmoc_sanity_timer_exp

DESCRIPTION
  This function will check if a particular transaction is stuck in MMoC
  thereby validating the MMoC sanity.
  The function will do ERR_FATAL if it determines that the MMoC is insane.

  Note! This function should be called at the expiration of the sanity timer.

===========================================================================*/
void                              mmoc_sanity_timer_exp
(

  mmoc_state_info_s_type          *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */

)
{
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stk = MM_STACK_NONE;
  rex_timer_cnt_type  sanity_timeout = MMOC_SANITY_TIME;

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Transaction has lasted more than sanity time, print debug messages.
  */
  MMOC_MSG_HIGH_2( "Trans %d trans_state %d",
                   mmoc_info_ptr->dev_prop.trans,
                   mmoc_info_ptr->dev_prop.trans_state);

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    for(stk = MM_STACK_0; stk < mmoc_info_ptr->sub_prop[sub]->nStacks && stk < MAX_STACKS; stk++)
    {
      MMOC_MSG_HIGH_3( "sub %d stk %d prot_state %d",
                       sub, stk,
                       mmoc_info_ptr->sub_prop[sub]->stack_prop[stk]->prot_state);
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if this was a race condition, sanity timer expired at the
  ** same time the transaction was made NULL.
  */
  if(mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_NULL)
  {
    /* Clear insanity_count.
    */
    mmoc_info_ptr->dev_prop.insanity_count = 0;
    return;

  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Print debug info about the offending protocol stack.
  */
  MMOC_MSG_HIGH_0( "No response yet for...");
  mmoc_display_debug_info( mmoc_info_ptr );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if defined(MMOC_HDR_SUPPORTED)
  /* WORKAROUND: Send HDR release RF lock command if 1x did not respond to
  ** MMoC's generic command for more than 48 seconds.
  */
  if ( (mmoc_info_ptr->dev_prop.trans_state ==  MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF ||
        mmoc_info_ptr->dev_prop.trans_state ==
        MMOC_TRANS_STATE_WAIT_AUTO_DEACTD_IND) &&
       mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.asubs_id == mmoc_info_ptr->dev_prop.sub_with_3gpp2 &&
       (mmoc_info_ptr->dev_prop.insanity_count * MMOC_SANITY_TIME) >= MMOC_HDR_RF_UNLOCK_TIME
     )
  {
    /* Send HDR release rf lock command.
    */
    mmoc_send_hdr_unlock_rf();
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Check if MAX insanity count was reached.
  */
  mmoc_info_ptr->dev_prop.insanity_count++;

#if defined(MMOC_LTE_TO_1X)
  if ((mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_PROT_REDIR_IND) &&
      (mmoc_info_ptr->dev_prop.trans_state == MMOC_TRANS_STATE_WAIT_AUTO_ACTD_IND) &&
      (mmoc_info_ptr->dev_prop.insanity_count == (MMOC_MAX_INSANITY_COUNT - 1)))
  {
    mc_cdma_abort_redir(mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.cmd_info.asubs_id);
    MMOC_MSG_HIGH_0("send MC_PROT_ABORT_REDIR_CMD_F due to AUTO_ACT_IND threshold");

  }
#endif

  if ( mmoc_info_ptr->dev_prop.insanity_count == MMOC_MAX_INSANITY_COUNT )
  {
#ifdef FEATURE_MMOC_NO_SANITY_ERR_FATAL

    /* Simulate the lower layer response and allow the transaction to
    ** proceed.
    */
    if ( mmoc_info_ptr->dev_prop.trans_state == MMOC_TRANS_STATE_WAIT_DEACTD_CNF )
    {
      /* Send deactivated confirmation.
      */
      mmoc_rpt_prot_deactd_cnf( mmoc_info_ptr->dev_prop.trans_id );
      MMOC_ERR_0( "Simulating deactd_cnf");
    }
    else if( mmoc_info_ptr->dev_prop.trans_state == MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL )
    {
      /* Send deactivated confirmation.
      */
      mmoc_rpt_prot_deactd_cnf( mmoc_info_ptr->dev_prop.trans_id );
      MMOC_ERR_0( "Simulating gwl deactd_cnf");
    }
    else if ( mmoc_info_ptr->dev_prop.trans_state == MMOC_TRANS_STATE_WAIT_PH_STAT_CNF )
    {
      /* Send Phone status changed confirmation
      */
      mmoc_rpt_ph_stat_chgd_cnf( mmoc_info_ptr->dev_prop.trans_id, SD_MODE_INACT,MMOC_PH_STAT_NO_ERROR);
      MMOC_ERR_0( "Simulating ph_stat_chgd_cnf");
    }
    else if ( mmoc_info_ptr->dev_prop.trans_state == MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF )
    {
      /* Send protocol generic command confirmation.
      */
      mmoc_rpt_prot_gen_cmd_cnf( FALSE, mmoc_info_ptr->dev_prop.trans_id, FALSE );
      MMOC_ERR_0( "Simulating prot_gen_cmd_cnf");

    }
#else
    /* MMoC stuck at this transaction for more than MAX_COUNT.
    */
    MMOC_MSG_HIGH_4("Sanity timer expired, Last Stop Mode sent %ld, Current timestamp %ld"
                    "insanity_count %d, max_sanity_time_multiple %d",
                    mmoc_info_ptr->dev_prop.last_stop_req_sent_timestamp, time_get_uptime_secs(),
                    mmoc_info_ptr->dev_prop.insanity_count, mmoc_info_ptr->dev_prop.max_sanity_time_multiple);
    mmoc_print_sanity_err_fatal( mmoc_info_ptr );
#endif
  }
  else
  {
    /* If it is powerdown transaction, load the special timeout value.
    */
    if ( mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_PWR_DOWN )
    {
      sanity_timeout = MMOC_PWRDOWN_SANITY_TIME;
    }

    /* If it is waiting for  PS detach conf, load special timeout value.
    */
    if( mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_PROT_GEN_CMD &&
        mmoc_info_ptr->dev_prop.trans_state == MMOC_TRANS_STATE_WAIT_PS_DETACH_CNF)
    {
      sanity_timeout = MMOC_PS_DETACH_SANITY_TIME;
    }

    if(mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_PROT_REDIR_IND)
    {
      sanity_timeout = mmoc_info_ptr->dev_prop.redir_sanity_timer;
    }

    /* Restart the sanity timer for the new transaction.
    */
    mmoc_sanity_timer_set(mmoc_info_ptr,sanity_timeout);

  } /* else */

} /* mmoc_sanity_timer_exp() */

/*===========================================================================

FUNCTION mmoc_activate_prot_online_ss_main2

DESCRIPTION
  This is a utility function called by the transaction state handlers to
  activate the protocol based on the SD20 action.
  It would set the MMoC's protocol state after activating the protocol based
  on main SS_instance.

  Note! For some SD actions( example power save ), new transactions have to
  be invoked. In those cases, this function will change the current
  transaction and return EVT_STATUS_NOT_CONSUMED.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion. No new
                                 transaction needs to be invoked.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                 invoke the new transaction handler
                                 again.

===========================================================================*/
mmoc_evt_status_e_type          mmoc_activate_prot_online_ss_main2
(
  sd_ss_act_e_type                   sd_act,
  /* Activate protocol based on this SD action.
  */

  sys_modem_as_id_e_type             asubs_id,

  mmoc_state_info_s_type            *mmoc_info_ptr,

  boolean                            delay_srv_req
  /* Indicate whether or not delay srv req - used by cmregprx to delay sending
  ** srv req to NAS to ensure for srlte+x config, dds lte acquisition starts before
  ** non dds.
  */
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = NULL;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  mmoc_evt_status_e_type  evt_status = MMOC_EVT_STATUS_CONSUMED;
  mm_sub_stk_id_s_type mm_id;

#ifndef TEST_FRAMEWORK
#ifndef FEATURE_MMOC_UNIT_TEST
#ifndef FEATURE_DISABLE_DEEP_SLEEP
  /* Change in bio(Basic IP/OP) interface.
  ** Now MMOC will use DAL interface instead of bio.
  */
  uint32           sw_switch9_state    = 0;

  if (switches_handle == NULL)
  {
    switches_dal_attach =
      DAL_DeviceAttach(DALDEVICEID_SOFTWARESWITCHES,&switches_handle);
  }

  if((switches_dal_attach == DAL_SUCCESS) && (switches_handle != NULL))
  {
    (void)DalSoftwareSwitches_GetMask(
      switches_handle,(uint32)DAL_SOFTWARE_SWITCH_9,&sw_switch9_state);
  }
#endif /* FEATURE_DISABLE_DEEP_SLEEP */
#endif /* FEATURE_MMOC_UNIT_TEST */
#endif

#if defined(FEATURE_HICPS_STACK_SYNC_UP)
  static boolean is_hdr_acq_blocked = FALSE;
#endif

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));
  mm_id.asubs_id = asubs_id;
  mm_id.stk_id = MM_STACK_0;

  MMOC_MSG_HIGH_3("SD->MMOC: ACTIVATE: stk0, sub %d sd_act=%d, delay=%d",
                  asubs_id, sd_act, delay_srv_req);

  if(!(mmoc_dev_info->active_subs & BM(asubs_id)) ||
      !mmoc_sub_info[asubs_id]->active_stacks & MM_STACK_0_MASK)
  {
    MMOC_MSG_HIGH_4("Activation not allowed on sub %d main stk, active_stacks %d active_subs %d prot_state %d",
                    asubs_id,
                    mmoc_sub_info[asubs_id]->active_stacks,
                    mmoc_dev_info->active_subs,
                    mmoc_sub_info[asubs_id]->stack_prop[MM_STACK_0]->prot_state);
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check of gps_exit rpt needs to be sent.
  */
  if ( mmoc_sub_info[asubs_id]->stack_prop[MM_STACK_0]->prot_state
       == PROT_STATE_GPSONE_MSBASED &&
       sd_act != SD_SS_ACT_CONTINUE &&
       sd_act != SD_SS_ACT_ACQ_GPS
     )
  {
    /* send GPS exit to CM.
    */
    mmoc_send_cm_rpt( CM_GPS_MODE_EXIT_F, NULL, asubs_id);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if acq is allowed to prevent acquisition on MAIN in suspended state
  */
  if(sd_act != SD_SS_ACT_PWR_SAVE &&
      sd_act != SD_SS_ACT_CONTINUE)
  {
    if(mmoc_sub_info[asubs_id]->stack_prop[MM_STACK_0]->is_suspend)
    {
      MMOC_MSG_HIGH_1("Sub %d main stk in suspended state, ignore acq",
                      asubs_id);

      return evt_status;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Activate the protocol based on the SD action or return
  ** EVT_STATUS_NOT_CONSUMED to invoke new transaction.
  */
  switch ( sd_act )
  {
#ifdef MMOC_CDMA_SUPPORTED
    case SD_SS_ACT_ACQ_CDMA:
      /* If SD says that CDMA should be in pseudo online mode, then bring up
      ** the CDMA protocol in pseudo online mode, else bring it up in online
      ** mode.
      */
      if(asubs_id == mmoc_dev_info->sub_with_3gpp2)
      {
        if( sd_misc_is_cdma_pseudo_online(asubs_id) )
        {
          mc_cdma_activate_protocol( asubs_id, PROT_ACT_PSEUDO_ONLINE );
        }
        else if( sd_misc_is_handoff (mm_id, SD_SS_HO_HDR_TO_CDMA) )
        {
          mc_cdma_activate_protocol( asubs_id, PROT_ACT_ONLINE_HDR_CDMA_HO );
        }
        else
        {
          mc_cdma_activate_protocol( asubs_id, PROT_ACT_ONLINE_ACQ );
        }

        mmoc_update_prot_state( PROT_STATE_ONLINE_CDMA,
                                mm_id,
                                mmoc_info_ptr
                              );
        mmoc_update_db_dmss_state( mmoc_info_ptr );
      }
      break;
#endif

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
    case SD_SS_ACT_ACQ_GWL:
    case SD_SS_ACT_GET_NET_GW:

      /* With GW acquisition request on main and if HDR is active on
      ** the hybrid instance deactivate hdr.
      */
      if(asubs_id == mmoc_dev_info->sub_with_3gpp2 &&
          mmoc_sub_info[asubs_id]->stack_prop[MM_STACK_1]->prot_state
          == PROT_STATE_ONLINE_HDR)
      {
        /* Deactivation request needs to be sent for online HDR.
        */
        mmoc_sub_info[asubs_id]->hdr_deact_activate_stack = MM_STACK_0;
        mmoc_update_trans_state( MMOC_TRANS_STATE_HDR_DEACT_ENTER,
                                 mmoc_info_ptr
                               );

        MMOC_MSG_HIGH_1("SD->MMOC: ACTIVATE: GW acq from SD, deact HDR, sub %d", asubs_id);

        return MMOC_EVT_STATUS_NOT_CONSUMED;
      }
      else if(mmoc_is_1x_sxlte(asubs_id) &&
              mmoc_sub_info[asubs_id]->stack_prop[MM_STACK_2]->prot_state
              == PROT_STATE_ONLINE_GWL)
      {
        /* Deactivation request needs to be sent for hybr2
        */
        mmoc_update_trans_state( MMOC_TRANS_STATE_HYBR2_DEACT_ENTER,
                                 mmoc_info_ptr
                               );

        MMOC_MSG_HIGH_0("SC_SVLTE: GW acq from SD, deact LTE");

        return MMOC_EVT_STATUS_NOT_CONSUMED;
      }

      mmoc_update_prot_state( PROT_STATE_ONLINE_GWL,
                              mm_id,
                              mmoc_info_ptr
                            );

      cmregprx_cmd_gwl_activate_protocol2( PROT_ACT_ONLINE_ACQ, mm_id, 
                                            delay_srv_req);


      mmoc_update_db_dmss_state( mmoc_info_ptr );

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  -*/

#ifdef FEATURE_MMODE_LTE_RESEL
      /* In case of IRAT to GWL from 3gpp2, move to state WAIT_FOR_ACTIVATION
      ** after sending activate to GWL
      */
      if ( mmoc_dev_info->trans == MMOC_TRANS_PROT_GEN_CMD &&
           mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.cmd_type
           == PROT_GEN_CMD_IRAT_TO_GWL &&
           mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.asubs_id == asubs_id &&
           mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.stk_id == MM_STACK_0)
      {
        /* This is necessary to avoid scenarios where MMOC clears the transaction
        */
        mmoc_update_trans_state( MMOC_TRANS_STATE_GEN_CMD_ACTIVATION_ENTER,
                                 mmoc_info_ptr
                               );

        return MMOC_EVT_STATUS_NOT_CONSUMED;
      }
#endif // FEATURE_MMODE_LTE_RESEL
      break;
#endif /* defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE) */

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef MMOC_HDR_SUPPORTED
    case SD_SS_ACT_ACQ_HDR:

      if(mmoc_is_1x_sxlte(asubs_id) &&
          mmoc_sub_info[asubs_id]->stack_prop[MM_STACK_2]->prot_state == PROT_STATE_ONLINE_GWL)
      {
#ifdef FEATURE_HICPS_STACK_SYNC_UP
        /* Update the flag to remember that hdr acq was ignored */
        is_hdr_acq_blocked = TRUE;
        MMOC_MSG_HIGH_0("HICPS : HDR acq buffered");
#else
        MMOC_ERR_0(" HDR acq request in incorrect state");
#endif /* FEATURE_HICPS_STACK_SYNC_UP */
        break;
      }

      mmoc_send_hdr_activate_protocol( PROT_ACT_ONLINE_ACQ );
      mmoc_update_prot_state( PROT_STATE_ONLINE_HDR,
                              mm_id,
                              mmoc_info_ptr
                            );
      mmoc_update_db_dmss_state( mmoc_info_ptr );
#if defined(FEATURE_HICPS_STACK_SYNC_UP)
      is_hdr_acq_blocked = FALSE;
#endif

      break;
#endif

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef MMOC_CDMA_SUPPORTED
    case SD_SS_ACT_MEAS_DED:
      mc_cdma_activate_protocol( asubs_id, PROT_ACT_DED_MEAS_REQUEST );
      mmoc_update_prot_state( PROT_STATE_ONLINE_DED_MEAS,
                              mm_id,
                              mmoc_info_ptr
                            );
      mmoc_update_db_dmss_state( mmoc_info_ptr );
      break;
#endif

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_GPSONE_OOS
    case SD_SS_ACT_ACQ_GPS:
      /* Notify CM that the protocol is placed in GPS state.
      */
      mmoc_send_cm_rpt( CM_GPS_MODE_GRANTED_F, NULL,asubs_id);
      mmoc_update_prot_state( PROT_STATE_GPSONE_MSBASED,
                              mm_id,
                              mmoc_info_ptr
                            );
      mmoc_update_db_dmss_state( mmoc_info_ptr );
      break;
#endif

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case SD_SS_ACT_PWR_SAVE:
      /* New "power save enter transaction" needs to be invoked,
      ** curr_trans should be "pwr save enter".
      */
      (void)sd_ss_ind_misc_pwr_save_enter(mm_id, NULL);
      MMOC_ASSERT( mmoc_dev_info->trans != MMOC_TRANS_PWR_SAVE_ENTER);

#if (defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE))
      if( (mmoc_dev_info->trans == MMOC_TRANS_ONLINE ||
           mmoc_dev_info->trans == MMOC_TRANS_SUBSC_CHGD) &&
          cmregprx_no_service_event_needed(mm_id)
        )
      {

        MMOC_MSG_HIGH_0("deact NAS with NO_SYS_TO_ACQ");
        cmregprx_cmd_gwl_deactivate_protocol( PROT_DEACT_NO_SYS_TO_ACQ,
                                              PROT_TRANS_NONE,
                                              mm_id
                                            );
      }
#endif

      mmoc_update_prot_state( PROT_STATE_PWR_SAVE,
                              mm_id,
                              mmoc_info_ptr
                            );
      /* Check if power save is not allowed, exit immediately */
#ifndef TEST_FRAMEWORK
#ifndef FEATURE_MMOC_UNIT_TEST
      MMOC_MSG_HIGH_1("sw_switch9_state [main] = 0x%x",sw_switch9_state);
      /* Check on GPIO to see if power save mode is allowed.
      */
      if (
#ifndef FEATURE_DISABLE_DEEP_SLEEP
        sw_switch9_state  &&
#endif
        sd_ss_ind_user_pwr_save_exit( asubs_id, NULL ) != SD_SS_ACT_PWR_SAVE
      )

      {
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        /* Power save mode is not allowed and SD can activate a new protocol.
        ** Aborted power save enter, activate the protocol or invoke
        ** transaction handler if this function returns EVT_STATUS_NOT_CONSUMED.
        */
        mmoc_activate_prot_online_ss_main( sd_ss_act_get_per_mm_id(NULL, mm_id),
                                           asubs_id,
                                           mmoc_info_ptr
                                         );

        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      } /* if (BIO_GET_SW(BIO_SW9_M)) */

#endif /* #if (!mmoc unit test) */
#endif
      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case SD_SS_ACT_CONTINUE:
      /* Do nothing.
      */
      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case SD_SS_ACT_MEAS_BACK:
    case SD_SS_ACT_ACCEPT:
    case SD_SS_ACT_REJECT:
    case SD_SS_ACT_RELEASE:
    case SD_SS_ACT_MAX:
    default:
      MMOC_ERR_1(" Err: SD act %d", sd_act);
      break;

  } /* switch( sd_act ) ) */


  return evt_status;

} /* mmoc_activate_prot_online_ss_main */

/*===========================================================================

FUNCTION mmoc_activate_prot_online_ss_main

DESCRIPTION
  This is a utility function called by the transaction state handlers to
  activate the protocol based on the SD20 action.
  It would set the MMoC's protocol state after activating the protocol based
  on main SS_instance.

  Note! For some SD actions( example power save ), new transactions have to
  be invoked. In those cases, this function will change the current
  transaction and return EVT_STATUS_NOT_CONSUMED.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion. No new
                                 transaction needs to be invoked.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                 invoke the new transaction handler
                                 again.

===========================================================================*/
mmoc_evt_status_e_type          mmoc_activate_prot_online_ss_main
(
  sd_ss_act_e_type                   sd_act,
  /* Activate protocol based on this SD action.
  */

  sys_modem_as_id_e_type             asubs_id,

  mmoc_state_info_s_type            *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  return mmoc_activate_prot_online_ss_main2(sd_act, asubs_id, mmoc_info_ptr, FALSE);
}

#if defined(MMOC_HYBR_1_SUPPORTED) || defined(MMOC_HYBR_SUPPORTED)
/*===========================================================================

FUNCTION mmoc_activate_prot_online_ss_hdr

DESCRIPTION
  This is a utility function called by the transaction state handlers to
  activate the protocol based on the SD20 action for SS_HDR instance.
  It would set the MMoC's protocol state after activating the protocol.

  Note! For some SD actions( example power save ), new transactions have to
  be invoked. In those cases, this function will change the current
  transaction and return EVT_STATUS_NOT_CONSUMED.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion. No new
                                 transaction needs to be invoked.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the new transaction handler
                                  again.

===========================================================================*/
mmoc_evt_status_e_type          mmoc_activate_prot_online_ss_hdr
(
  sd_ss_act_e_type                   sd_act,
  /* Activate protocol based on this SD action.
  */
  sys_modem_as_id_e_type             asubs_id,

  mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  mmoc_evt_status_e_type  evt_status = MMOC_EVT_STATUS_NOT_CONSUMED;

#if defined(FEATURE_HICPS_STACK_SYNC_UP)
  static boolean is_hdr_acq_blocked = FALSE;
#endif

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  /* Can't be const as functions called from here modify it.
  ** Example - mmoc_update_prot_state()
  */
  SYS_ARG_NOT_CONST(mmoc_info_ptr);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  if(!(mmoc_info_ptr->dev_prop.active_subs & BM(asubs_id)) ||
      !(mmoc_info_ptr->sub_prop[asubs_id]->active_stacks & MM_STACK_1_MASK))
  {
    MMOC_MSG_HIGH_3("HYBR1 Activation not allowed, sub %d MAIN not active. active_subs %d HYBR1 prot_state %d",
                    asubs_id,
                    mmoc_info_ptr->dev_prop.active_subs,
                    mmoc_info_ptr->sub_prop[asubs_id]->stack_prop[MM_STACK_1]->prot_state);

    return MMOC_EVT_STATUS_CONSUMED;
  }

  while ( evt_status != MMOC_EVT_STATUS_CONSUMED )
  {
    evt_status = MMOC_EVT_STATUS_CONSUMED;

    /* Activate the protocol based on the SD action.
    */
    switch ( sd_act )
    {
#ifdef MMOC_HDR_SUPPORTED
      case SD_SS_ACT_ACQ_HDR:
        /* HDR system selection timer expiry when Main instance is online GW
        ** results in HDR and GW becoming active at the same time. This check
        ** prevents hdr from being kicked online when GW is active.
        */

        if (mmoc_is_hdr_acq_allowed_hicps (mmoc_info_ptr))
        {
          if( mmoc_is_hdr_acq_allowed_mmss (mmoc_info_ptr))
          {
            mm_sub_stk_id_s_type mm_id;
            mm_id.asubs_id = asubs_id;
            mm_id.stk_id   = MM_STACK_1;

            mmoc_send_hdr_activate_protocol( PROT_ACT_ONLINE_ACQ );
            mmoc_update_prot_state( PROT_STATE_ONLINE_HDR,
                                    mm_id,
                                    mmoc_info_ptr
                                  );
#if defined(FEATURE_HICPS_STACK_SYNC_UP)
            is_hdr_acq_blocked = FALSE;
#endif
          }
          else
          {
            MMOC_ERR_0("MMSS scan_permission set incorrectly");
          }
        }
        else
        {
#ifdef FEATURE_HICPS_STACK_SYNC_UP
          /* Update the flag to remember that hdr acq was ignored */
          is_hdr_acq_blocked = TRUE;
          MMOC_MSG_HIGH_0("HICPS : HDR acq buffered");
#else
          MMOC_ERR_0("HDR acq request in incorrect state");
#endif
        }

        break;
#endif

#ifdef MMOC_HYBR_GW_SUPPORTED
      case SD_SS_ACT_ACQ_GWL:
      case SD_SS_ACT_GET_NET_GW:
      {
        mm_sub_stk_id_s_type mm_id;
        mm_id.asubs_id = asubs_id;
        mm_id.stk_id   = MM_STACK_1;

        cmregprx_cmd_gwl_activate_protocol( PROT_ACT_ONLINE_ACQ, mm_id );
        mmoc_update_prot_state( PROT_STATE_ONLINE_GWL,
                                mm_id,
                                mmoc_info_ptr
                              );
        mmoc_update_db_dmss_state( mmoc_info_ptr );
      }
      break;
#endif

#ifdef MMOC_HYBR_1_SUPPORTED
      case SD_SS_ACT_PWR_SAVE:
      {
        mm_sub_stk_id_s_type mm_id;
        mm_id.asubs_id = asubs_id;
        mm_id.stk_id   = MM_STACK_1;

        sd_act = sd_ss_ind_misc_pwr_save_enter( mm_id, NULL );
        if ( sd_act != SD_SS_ACT_CONTINUE)
        {
          evt_status = MMOC_EVT_STATUS_NOT_CONSUMED;
        }
        mmoc_update_prot_state( PROT_STATE_PWR_SAVE,
                                mm_id,
                                mmoc_info_ptr
                              );
      }
      break;
#endif

      case SD_SS_ACT_CONTINUE:
#if (defined(MMOC_HDR_SUPPORTED) && defined(FEATURE_HICPS_STACK_SYNC_UP))

        /* In Case of Stack Sync-up. The previously issues hdr acq action
        ** needs to be continued.
        ** - Check if a previous HDR acq action was blocked
        ** - Start HDR Protocol
        ** - Reset the flag
        */
        if(is_hdr_acq_blocked)
        {

          if ( mmoc_is_hdr_acq_allowed_hicps (mmoc_info_ptr)
               && mmoc_is_hdr_acq_allowed_mmss (mmoc_info_ptr)
             )
          {
            mm_sub_stk_id_s_type mm_id;
            mm_id.asubs_id = asubs_id;
            mm_id.stk_id   = MM_STACK_1;

            mmoc_send_hdr_activate_protocol( PROT_ACT_ONLINE_ACQ );
            mmoc_update_prot_state( PROT_STATE_ONLINE_HDR,
                                    mm_id,
                                    mmoc_info_ptr
                                  );
            is_hdr_acq_blocked = FALSE;
          }
        }
#endif /* MMOC_HDR_SUPPORTED */
        break;

      case SD_SS_ACT_ACQ_CDMA:
      case SD_SS_ACT_MEAS_DED:
      case SD_SS_ACT_MEAS_BACK:
      case SD_SS_ACT_ACCEPT:
      case SD_SS_ACT_REJECT:
      case SD_SS_ACT_RELEASE:
      case SD_SS_ACT_ACQ_GPS:
      case SD_SS_ACT_MAX:
      default:
        MMOC_ERR_1(" Err: SD act %d", sd_act);
        break;

    } /* switch( sd_act ) ) */

  } /* while ( sd_act ) */

  return evt_status;

} /* mmoc_activate_prot_online_ss_hdr() */

#endif /* (MMOC_HYBR_1_SUPPORTED) || (MMOC_HYBR_SUPPORTED) */


/*===========================================================================

FUNCTION mmoc_activate_prot_online_ss_hybr_2

DESCRIPTION
  This is a utility function called by the transaction state handlers to
  activate the protocol based on the SD20 action.
  It would set the MMoC's protocol state after activating the protocol based
  on hybr_2 SS_instance.

  Note! For some SD actions( example power save ), new transactions have to
  be invoked. In those cases, this function will change the current
  transaction and return EVT_STATUS_NOT_CONSUMED.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion. No new
                                 transaction needs to be invoked.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the new transaction handler
                                  again.

===========================================================================*/
mmoc_evt_status_e_type          mmoc_activate_prot_online_ss_hybr_2
(
  sd_ss_act_e_type                   sd_act,
  /* Activate protocol based on this SD action.
  */

  sys_modem_as_id_e_type             asubs_id,

  mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  mmoc_evt_status_e_type  evt_status = MMOC_EVT_STATUS_CONSUMED;

  static boolean is_hybr_2_acq_blocked = FALSE;

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  MMOC_MSG_HIGH_2("MMOC->CMREG: ACTIVATE: stk2, sub %d sd_act=%d",
                  asubs_id, sd_act);

  if(!(mmoc_info_ptr->dev_prop.active_subs & BM(asubs_id)) ||
      !(mmoc_info_ptr->sub_prop[asubs_id]->active_stacks & MM_STACK_2_MASK))
  {
    MMOC_MSG_HIGH_3("HYBR2 Activation not allowed, sub %d MAIN not active. active_subs %d HYBR2 prot_state %d",
                    asubs_id,
                    mmoc_info_ptr->dev_prop.active_subs,
                    mmoc_info_ptr->sub_prop[asubs_id]->stack_prop[MM_STACK_2]->prot_state);

    return MMOC_EVT_STATUS_CONSUMED;
  }

  /* Activate the protocol based on the SD action or return
  ** EVT_STATUS_NOT_CONSUMED to invoke new transaction.
  */
  switch ( sd_act )
  {
#ifdef MMOC_GWL_SUPPORTED
    case SD_SS_ACT_ACQ_GWL:
    case SD_SS_ACT_GET_NET_GW:
    {
      if(mmoc_is_1x_sxlte(asubs_id))
      {
        if(!mmoc_is_hybr_2_acq_allowed(mmoc_info_ptr, asubs_id))
        {
          /* Update the flag to remember that HYBR 2 acq was ignored
          */
          is_hybr_2_acq_blocked = TRUE;
          MMOC_MSG_HIGH_0("SC_SVLTE: lte acq buffered");
          break;
        }

        if(mmoc_info_ptr->sub_prop[asubs_id]->stack_prop[MM_STACK_1]->prot_state == PROT_STATE_ONLINE_HDR)
        {
          /* Deactivation request needs to be sent for online HDR.
          */
          sd_ss_hdr_irat_fail_ind(asubs_id);
          mmoc_info_ptr->sub_prop[asubs_id]->hdr_deact_activate_stack = MM_STACK_2;
          mmoc_update_trans_state( MMOC_TRANS_STATE_HDR_DEACT_ENTER,
                                   mmoc_info_ptr
                                 );

          MMOC_MSG_HIGH_1("MMOC->CMREG: ACTIVATE: GW acq from SD HYBR2, deact HDR, sub %d", asubs_id);

          return MMOC_EVT_STATUS_NOT_CONSUMED;
        }
      }

      {
        mm_sub_stk_id_s_type mm_id;
        mm_id.asubs_id = asubs_id;
        mm_id.stk_id   = MM_STACK_2;

        cmregprx_cmd_gwl_activate_protocol( PROT_ACT_ONLINE_ACQ, mm_id );
        mmoc_update_prot_state( PROT_STATE_ONLINE_GWL,
                                mm_id,
                                mmoc_info_ptr
                              );
      }

      mmoc_update_db_dmss_state( mmoc_info_ptr );

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  -*/

      /* In case of IRAT to GWL from 3gpp2, move to state WAIT_FOR_ACTIVATION
      ** after sending activate to GWL
      */
#ifdef FEATURE_MMODE_LTE_RESEL
      if ( mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_PROT_GEN_CMD &&
           mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.cmd_type
           == PROT_GEN_CMD_IRAT_TO_GWL &&
           mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.stk_id == MM_STACK_2)
      {
        /* This is necessary to avoid scenarios where MMOC clears the transaction
        */
        mmoc_update_trans_state( MMOC_TRANS_STATE_GEN_CMD_ACTIVATION_ENTER,
                                 mmoc_info_ptr
                               );

        MMOC_MSG_HIGH_0("returning not consumed");
        return MMOC_EVT_STATUS_NOT_CONSUMED;
      }
#endif // FEATURE_MMODE_LTE_RESEL
    }
    break;
#endif //MMOC_GWL_SUPPORTED

    case SD_SS_ACT_PWR_SAVE:
      /* New "power save enter transaction" needs to be invoked,
      ** curr_trans should be "pwr save enter".
      */
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = asubs_id;
      mm_id.stk_id   = MM_STACK_2;

      sd_act = sd_ss_ind_misc_pwr_save_enter(mm_id, NULL);
      MMOC_ASSERT( mmoc_info_ptr->dev_prop.trans != MMOC_TRANS_PWR_SAVE_ENTER);

      mmoc_update_prot_state( PROT_STATE_PWR_SAVE,
                              mm_id,
                              mmoc_info_ptr
                            );
    }

    break;

    case SD_SS_ACT_CONTINUE:
      /* In Case of Stack Sync-up. The previously issues hybr 2 acq action
      ** needs to be continued.
      ** - Check if a previous HYBR 2 acq action was blocked
      ** - Start HYBR 2 Protocol
      ** - Reset the flag
      */
#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
      if(is_hybr_2_acq_blocked)
      {
        if (mmoc_is_hybr_2_acq_allowed(mmoc_info_ptr, asubs_id))
        {
          mmoc_activate_prot_online_ss_hybr_2( SD_SS_ACT_ACQ_GWL,
                                               asubs_id,
                                               mmoc_info_ptr
                                             );
        }
        is_hybr_2_acq_blocked = FALSE;
      }
#endif
      break;

    case SD_SS_ACT_MEAS_BACK:
    case SD_SS_ACT_ACCEPT:
    case SD_SS_ACT_REJECT:
    case SD_SS_ACT_RELEASE:
    case SD_SS_ACT_MAX:
    default:
      MMOC_ERR_1(" Err: SD act %d", sd_act);
      break;

  } /* switch( sd_act ) ) */


  MMOC_MSG_HIGH_1("retunrning evt_status %d",evt_status);
  return evt_status;

}


/*===========================================================================

FUNCTION mmoc_activate_prot_online

DESCRIPTION
  This is a utility function called by the transaction state handlers to
  activate the protocol based on the SD20 action.


DEPENDENCIES
  None.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion. No new
                                 transaction needs to be invoked.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the new transaction handler
                                  again.

SIDE EFFECTS
  None.
===========================================================================*/
static mmoc_evt_status_e_type          mmoc_activate_prot_online
(
  mm_sub_stk_id_s_type                mm_id,
  /* Activate protocol for this System selection instance.
  */

  mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  mmoc_evt_status_e_type  evt_status = MMOC_EVT_STATUS_CONSUMED;

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Invoke the activate_prot function based on the SS instance.
  */
  switch(mm_id.stk_id)
  {
    case MM_STACK_0:
      evt_status = mmoc_activate_prot_online_ss_main(
                     sd_ss_act_get_per_mm_id(NULL, mm_id),
                     mm_id.asubs_id,
                     mmoc_info_ptr);
      break;

    case MM_STACK_1:
#ifdef MMOC_HYBR_1_SUPPORTED
      evt_status = mmoc_activate_prot_online_ss_hdr(
                     sd_ss_hdr_act_get(mm_id.asubs_id, NULL ),
                     mm_id.asubs_id,
                     mmoc_info_ptr);
#endif
      break;


    case MM_STACK_2:
      evt_status = mmoc_activate_prot_online_ss_hybr_2(
                     sd_ss_act_get_per_mm_id(NULL, mm_id),
                     mm_id.asubs_id,
                     mmoc_info_ptr);
      break;

    default:
      sys_err_fatal_invalid_value_exception((int)mm_id.stk_id);

  } /* switch ( ss ) */

  return evt_status;

} /* mmoc_activate_prot_online() */


/*===========================================================================

FUNCTION mmoc_activate_prot_offline

DESCRIPTION
  This is a utility function called by the transaction state handlers to
  activate the protocol based the current operating mode.
  It would set the MMoC's protocol state after activating the protocol.

  Note! This function should be called only when the operating mode is
  OFFLINE_CDMA or OFFLINE_AMPS.

===========================================================================*/
static void                            mmoc_activate_prot_offline
(

  mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = NULL;
  MMOC_ASSERT( mmoc_info_ptr != NULL );
  mmoc_dev_info = &mmoc_info_ptr->dev_prop;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Activate the protocol based on the offline operating mode.
  */
  switch(mmoc_info_ptr->dev_prop.oprt_mode)
  {
    case PROT_OPRT_MODE_OFFLINE_CDMA:
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = mmoc_dev_info->sub_with_3gpp2;
      mm_id.stk_id = MM_STACK_0;
#ifdef MMOC_CDMA_SUPPORTED
      mc_cdma_activate_protocol( mm_id.asubs_id, PROT_ACT_OFFLINE );
#endif /* MMOC_CDMA_SUPPORTED */
      mmoc_update_prot_state( PROT_STATE_OFFLINE_CDMA,
                              mm_id,
                              mmoc_info_ptr
                            );
      mmoc_update_db_dmss_state( mmoc_info_ptr );
    }
    break;

    case PROT_OPRT_MODE_OFFLINE:
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = mmoc_dev_info->sub_with_3gpp2;;
      mm_id.stk_id = MM_STACK_0;
      mmoc_update_prot_state( PROT_STATE_OFFLINE,
                              mm_id,
                              mmoc_info_ptr
                            );
      /* Need a seperate db_dmss_state here.
      */
      mmoc_update_db_dmss_state( mmoc_info_ptr );
    }
    break;

    case PROT_OPRT_MODE_FTM:
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = mmoc_dev_info->sub_with_3gpp2;
      mm_id.stk_id = MM_STACK_0;

      /* Activate the protocol stack in FTM mode.
      */
      TTLBOOTUP(MMOC_FTM);

#if defined(MMOC_FTM_SUPPORTED)
#if defined(MMOC_CDMA_SUPPORTED)
      mc_cdma_activate_protocol( mm_id.asubs_id, PROT_ACT_FTM );
#endif /* MMOC_CDMA_SUPPORTED */
#if defined(MMOC_GW_SUPPORTED) || defined(MMOC_CDMA_SUPPORTED)
      ftm_activate_protocol(rex_self(),MMOC_FTM_RESUME_MMOC_SIG, (void (*)( rex_sigs_type )) mmoc_wait, PROT_ACT_FTM, mmoc_next_trans_id(mmoc_info_ptr));
#endif
      mmoc_update_prot_state( PROT_STATE_FTM,
                              mm_id,
                              mmoc_info_ptr
                            );
      mmoc_update_db_dmss_state( mmoc_info_ptr );
#else
      MMOC_ERR_FATAL_0("FTM not supported");
#endif /* defined(MMOC_FTM_SUPPORTED) */
    }
    break;

    case PROT_OPRT_MODE_NULL:
    case PROT_OPRT_MODE_PWROFF:
    case PROT_OPRT_MODE_ONLINE:
    case PROT_OPRT_MODE_LPM:
    case PROT_OPRT_MODE_RESET:
    case PROT_OPRT_MODE_MAX:
    default:
      sys_err_fatal_invalid_value_exception((int)mmoc_info_ptr->dev_prop.oprt_mode);
      break;

  } /* switch ( mmoc_info_ptr->curr_oprt_mode ) */

} /* mmoc_activate_prot_offline() */



/*===========================================================================

FUNCTION mmoc_deactivate_gwl_protocol

DESCRIPTION
  This is a utility function called by transaction state handlers to
  deactivate GWL protocol when protocol could be in suspend state.

RETURN VALUE
  TRUE  - A protocol deactivate request was sent to the active protocol.
  FALSE - No request was sent.

===========================================================================*/
#ifdef MMOC_GWL_SUPPORTED

static boolean                         mmoc_deactivate_gwl_protocol
(
  mm_sub_stk_id_s_type            mm_id,
  /* SS instance to be deactivated.
  */

  prot_deact_e_type               deact_reason,
  /* Reason for deactivating the protocol.
  */

  mmoc_state_info_s_type          *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(mm_id.asubs_id < SYS_MODEM_AS_ID_1 || mm_id.asubs_id >= MAX_SIMS ||
      mm_id.stk_id < MM_STACK_0 || mm_id.stk_id >= MAX_STACKS ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id] == NULL ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id] == NULL)
  {
    return FALSE;
  }

  /* Set flag to avoid sending deactivate GWL request multiple times
  */

  if(mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->is_gwl_deact_sent)
  {
    return FALSE;
  }

  /* Send GWL deactivate command when subscription change or oprt mode change
  ** This is added to solve issue when ALT_SCAN/DEEP_SLEEP/IRAT happens, NAS
  ** goes to suspend state. It needs another STOP_MODE_REQ to go to deactivate
  ** state
  */
  switch(mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state)
  {
    case PROT_STATE_NULL:
      switch(mmoc_dev_info->trans)
      {
        case MMOC_TRANS_SUBSC_CHGD:
          /* Only need to deactivate GWL when GW subsc changed
          **
          */
          deact_reason = mmoc_evaluate_deact_reason(mm_id,deact_reason, mmoc_info_ptr);
          if((CHECK_PROTO(mmoc_sub_info[mm_id.asubs_id]->prot_subsc_chg, PROT_SUBSC_CHG_3GPP) || deact_reason == PROT_DEACT_DUAL_SWITCH ) &&
              mm_id.stk_id == MM_STACK_0)
          {
            cmregprx_cmd_gwl_deactivate_protocol( deact_reason,
                                                  mmoc_next_trans_id(mmoc_info_ptr),
                                                  mm_id
                                                );
            mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->is_gwl_deact_sent = TRUE;
            mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->last_prot_state =
              mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state;
          }

          break;

        /* Handle LPM/OFFLINE command here */
        case MMOC_TRANS_OFFLINE:
        case MMOC_TRANS_PWR_DOWN:
	case MMOC_TRANS_DUAL_STANDBY_CHGD:

          cmregprx_cmd_gwl_deactivate_protocol( deact_reason,
                                                mmoc_next_trans_id(mmoc_info_ptr),
                                                mm_id
                                              );
          mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->is_gwl_deact_sent = TRUE;
          mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->last_prot_state =
            mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state;
          break;

        default:
          break;
      }
      break;


    default:
      break;
  }


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the last deactivate request sent timestamp with current time */
  mmoc_dev_info->last_stop_req_sent_timestamp = time_get_uptime_secs();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->is_gwl_deact_sent)
  {
    MMOC_MSG_HIGH_6("MMOC->PROT: DEACT_REQ to inactive 3GPP, deact_reason %d sub %d stk %d insanity_count %d trans %d prot_state %d",
                    deact_reason,
                    mm_id.asubs_id,
                    mm_id.stk_id,
                    mmoc_dev_info->insanity_count,
                    mmoc_dev_info->trans,
                    mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state);

    /* when sending deact to GW protocol on a stack, check for any buffered
    ** auto_deact only if some GW protocol is active on that stack.
    ** If yes, discard the report */
    if(mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state == PROT_STATE_ONLINE_GWL)
    {
      mmoc_is_auto_deact_ind_rpt_buf_per_stack(mmoc_info_ptr, mm_id, TRUE);
    }
  }

  return mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->is_gwl_deact_sent;

} /* mmoc_deactivate_gwl_protocol() */

#endif


/*===========================================================================

FUNCTION mmoc_deactivate_active_protocol

DESCRIPTION
  This is a utility function called by transaction state handlers to
  deactivate the active protocol if one exists.

RETURN VALUE
  TRUE  - A protocol deactivate request was sent to the active protocol.
  FALSE - No request was sent.

===========================================================================*/
static boolean                         mmoc_deactivate_active_protocol
(
  mm_sub_stk_id_s_type            mm_id,
  /* SS instance to be deactivated.
  */

  prot_deact_e_type               deact_reason,
  /* Reason for deactivating the protocol.
  */

  mmoc_state_info_s_type          *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  boolean is_deact_req_sent = FALSE;
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = NULL;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  if(mm_id.asubs_id < SYS_MODEM_AS_ID_1 || mm_id.asubs_id >= MAX_SIMS ||
      mm_id.stk_id < MM_STACK_0 || mm_id.stk_id >= MAX_STACKS ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id] == NULL ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id] == NULL)
  {
    return FALSE;
  }

  /* If the SIM has already been removed, convert deact reason to subsc chgd
  ** so that the UE will do OTA detach.
  */
  if(deact_reason == PROT_DEACT_DUAL_SWITCH &&
     mmgsdi_info_ptr->sim_available_status[mm_id.asubs_id] == FALSE)
  {
    MMOC_MSG_HIGH_1("SIM already removed on sub %d, use deact reason subsc chgd", mm_id.asubs_id);
    deact_reason = PROT_DEACT_SUBSC_CHGD;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check on the current MMoC's protocol state. */

  switch(mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state)
  {
#ifdef MMOC_CDMA_SUPPORTED
    case PROT_STATE_ONLINE_CDMA:
    case PROT_STATE_OFFLINE_CDMA:
      mc_cdma_deactivate_protocol( mm_id.asubs_id,
                                   deact_reason,
                                   mmoc_next_trans_id(mmoc_info_ptr)
                                 );
      is_deact_req_sent = TRUE;
      break;
#endif

    case PROT_STATE_ONLINE_DED_MEAS:
#ifdef MMOC_CDMA_SUPPORTED
      if(mm_id.asubs_id == mmoc_dev_info->sub_with_3gpp2 &&
          (mm_id.stk_id == MM_STACK_0 ||
           mm_id.stk_id == MM_STACK_1 ))
      {
        mc_cdma_deactivate_protocol( mm_id.asubs_id,
                                     deact_reason,
                                     mmoc_next_trans_id(mmoc_info_ptr)
                                   );
        is_deact_req_sent = TRUE;
      }
#endif
      break;

#ifdef MMOC_FTM_SUPPORTED
    case PROT_STATE_FTM:

      /* Preserve the deactivate order ( FTM first, MC next ), this
      ** FTM activity is completely deactivated, when we get the confirmation
      ** from MC. Note that ftm_deactivate_protocol() is executed in MMOC's
      ** context.
      */
#if defined(MMOC_GW_SUPPORTED) || defined(MMOC_CDMA_SUPPORTED)
      ftm_deactivate_protocol(rex_self(),MMOC_FTM_RESUME_MMOC_SIG, (void (*)( rex_sigs_type )) mmoc_wait, deact_reason, mmoc_next_trans_id(mmoc_info_ptr));

#endif
#ifdef MMOC_CDMA_SUPPORTED
      mc_cdma_deactivate_protocol( mm_id.asubs_id,
                                   deact_reason,
                                   mmoc_next_trans_id(mmoc_info_ptr)
                                 );
      is_deact_req_sent = TRUE;
#endif

      break;
#endif /* MMOC_FTM_SUPPORTED */


#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
    case PROT_STATE_ONLINE_GWL:
      cmregprx_cmd_gwl_deactivate_protocol( mmoc_evaluate_deact_reason(mm_id,\
                                            deact_reason, mmoc_info_ptr),
                                            mmoc_next_trans_id(mmoc_info_ptr),
                                            mm_id
                                          );

      mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->is_gwl_deact_sent = TRUE;
      is_deact_req_sent = TRUE;

      
      /* If LTE on stack 2 is getting deactivated due to dual switch inform SD 
         ** so that if the acquired 1x is in LTE_NOT_AVAIL file SD can enter power save
         ** BSR timeline immediately
         */
      if(deact_reason == PROT_DEACT_DUAL_SWITCH)
      {
        sd_ss_set_lte_deact_dual_switch_flag(mm_id,TRUE);
      }
      break;
#endif


#ifdef MMOC_HDR_SUPPORTED
    case PROT_STATE_ONLINE_HDR:
      mmoc_send_hdr_deactivate_protocol( deact_reason,
                                         mmoc_next_trans_id(mmoc_info_ptr),
                                         mm_id.asubs_id
                                       );
      is_deact_req_sent = TRUE;
      break;
#endif


#ifdef FEATURE_GPSONE_OOS
    case PROT_STATE_GPSONE_MSBASED:
      /* Notify CM that GPS state is aborted.
      */
      mmoc_send_cm_rpt( CM_GPS_MODE_EXIT_F, NULL,mm_id.asubs_id);

      break;
#endif /* FEATURE_GPSONE_OOS */


    case PROT_STATE_BLOCKED:
    case PROT_STATE_NULL:
    case PROT_STATE_OFFLINE:
    case PROT_STATE_PWR_SAVE:
    case PROT_STATE_RESET:
    case PROT_STATE_LPM:
    case PROT_STATE_PWR_DOWN:
    case PROT_STATE_MAX:
    default:
      MMOC_MSG_LOW_0(" No active protocol to deactivate");
      break;
  } /* switch( mmoc_info_ptr->prot_state ) */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Store the protocol state of deactivated index in main_last_prot_state  */
  if ( is_deact_req_sent )
  {
    mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->last_prot_state =
      mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state;
  }

  if ( !is_deact_req_sent )
  {
    /* If the prot state is already in OFFLINE/RESET/LPM/PWR_DOWN we don't need to
      ** send deact gwl again since. These states are updated in ph_state_enter
      */
    if(mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state != PROT_STATE_NULL &&
        mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state != PROT_STATE_PWR_SAVE
      )
    {
      mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->is_gwl_deact_sent = TRUE;
    }

    mmoc_update_prot_state( PROT_STATE_NULL,
                            mm_id,
                            mmoc_info_ptr
                          );

  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the last deactivate request sent timestamp with current time */
  mmoc_dev_info->last_stop_req_sent_timestamp = time_get_uptime_secs();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (is_deact_req_sent)
  {
    mmoc_sanity_timer_reset(mmoc_info_ptr);

    mmoc_sanity_timer_set(mmoc_info_ptr, MMOC_PWRDOWN_SANITY_TIME);

    MMOC_MSG_HIGH_6("MMOC->PROT: DEACT_REQ to ACTIVE protocol: %d, reason: %d, sub %d stk %d, insanity_count %d, ps_enabled %d",
                    mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->last_prot_state,
                    deact_reason,
                    mm_id.asubs_id,
                    mm_id.stk_id,
                    mmoc_dev_info->insanity_count,
                    mmoc_info_ptr->sub_prop[mm_id.asubs_id]->ps_enabled);

    /* While sending deactivate req to a protocol, check if we have any
    ** auto_deact buffered from that protocol. If yes, discard the report
    */
    mmoc_is_auto_deact_ind_rpt_buf_per_stack(mmoc_info_ptr, mm_id, TRUE);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return is_deact_req_sent;

} /* mmoc_deactivate_active_protocol() */


/*===========================================================================

FUNCTION mmoc_deactivate_dormant_protocol

DESCRIPTION
  This is a utility function called by transaction state handlers to
  deactivate the dormant protocol if one exists.

RETURN VALUE
  TRUE  - A protocol deactivate request was sent to the dormant protocol.
  FALSE - No request was sent.

===========================================================================*/
static boolean                         mmoc_deactivate_dormant_protocol
(
  mm_sub_stk_id_s_type            mm_id,
  /* SS instance to be deactivated.
  */

  prot_deact_e_type               deact_reason,
  /* Reason for deactivating the protocol.
  */

  mmoc_state_info_s_type          *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  boolean is_deact_req_sent = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  if(mm_id.asubs_id < SYS_MODEM_AS_ID_1 || mm_id.asubs_id >= MAX_SIMS ||
      mm_id.stk_id < MM_STACK_0 || mm_id.stk_id >= MAX_STACKS ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id] == NULL ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id] == NULL)
  {
    return is_deact_req_sent;
  }

  if(mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_dormant == SD_SS_MODE_PREF_NONE )
  {
    MMOC_MSG_LOW_0("No dormant protocol to deactivate");
    return is_deact_req_sent;
  }

#ifdef MMOC_GWL_SUPPORTED
  /* If GWL to be deactivated and GWL is in dormant state */
  else if(((int)mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_dormant
           & (int)SD_SS_MODE_PREF_GWL))
  {
    MMOC_MSG_LOW_0("dormant_state If GW to be deactivated and GW is in dormant state");
    cmregprx_cmd_gwl_deactivate_protocol( deact_reason,
                                          mmoc_next_trans_id(mmoc_info_ptr),
                                          mm_id
                                        );

    is_deact_req_sent = TRUE;
  }
#else
  SYS_ARG_NOT_USED(deact_reason);
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the last deactivate request sent timestamp with current time */

  mmoc_info_ptr->dev_prop.last_stop_req_sent_timestamp = time_get_uptime_secs();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (is_deact_req_sent)
  {
    MMOC_MSG_HIGH_4("Deact req sent(dormant), deact_reason %d, req_index sub %d stk %d, insanity_count %d",
                    deact_reason, mm_id.asubs_id, mm_id.stk_id, mmoc_info_ptr->dev_prop.insanity_count);

    /* when sending deact to GW protocol in dormant state on a stack,
    ** check for any buffered auto_deact only if some GW protocol is
    ** active on that stack.If yes, discard the report */
    if(mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state == PROT_STATE_ONLINE_GWL)
    {
      mmoc_is_auto_deact_ind_rpt_buf_per_stack(mmoc_info_ptr, mm_id, TRUE);
    }
  }

  return is_deact_req_sent;

} /* mmoc_deactivate_dormant_protocol */



/*===========================================================================

FUNCTION mmoc_deactivate_all_active_protocol

DESCRIPTION
  This is a utility function called by transaction state handlers to
  deactivate all the active protocol.

RETURN VALUE
  TRUE  - A protocol deactivate request was sent to the active protocol.
  FALSE - No request was sent.

===========================================================================*/
static boolean                         mmoc_deactivate_all_active_protocol
(
  prot_deact_e_type               deact_reason,
  /* Reason for deactivating the protocol.
  */

  mmoc_state_info_s_type          *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{

  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  boolean deact_req_sent = FALSE;
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stack = MM_STACK_NONE;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /* Deact non dds sub first */
  for(sub = (mmoc_info_ptr->nSubs - 1); sub > SYS_MODEM_AS_ID_NONE; sub--)
  {
    if(sub < MAX_SIMS && !mmoc_sub_info[sub]->ps_enabled)
    {
      for(stack = (mmoc_sub_info[sub]->nStacks - 1); stack > MM_STACK_NONE; stack--)
      {
        if(!deact_req_sent)
        {
          mmoc_dev_info->deact_req_idx.asubs_id = sub;
          mmoc_dev_info->deact_req_idx.stk_id = stack;
          deact_req_sent = mmoc_deactivate_active_protocol(
                             mmoc_dev_info->deact_req_idx,
                             deact_reason,
                             mmoc_info_ptr);
        }
      }
    }
  }

  for(sub = (mmoc_info_ptr->nSubs - 1); sub > SYS_MODEM_AS_ID_NONE; sub--)
  {
    if(sub < MAX_SIMS && mmoc_sub_info[sub]->ps_enabled)
    {
      for(stack = (mmoc_sub_info[sub]->nStacks - 1); stack > MM_STACK_NONE; stack--)
      {
        if(!deact_req_sent)
        {
          mmoc_dev_info->deact_req_idx.asubs_id = sub;
          mmoc_dev_info->deact_req_idx.stk_id = stack;
          deact_req_sent = mmoc_deactivate_active_protocol(
                             mmoc_dev_info->deact_req_idx,
                             deact_reason,
                             mmoc_info_ptr);
        }
      }
    }
  }

#if defined(MMOC_HDR_SUPPORTED) && !defined(FEATURE_MMOC_UNIT_TEST)
  mmoc_send_hdr_unlock_rf();
#endif

  return deact_req_sent;

} /*  mmoc_deactivate_all_active_protocol */

/*===========================================================================

FUNCTION  mmoc_notify_offline_mode

DESCRIPTION
  Notifies offline mode change command other modules in modem.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void mmoc_notify_offline_mode( void )
{
  mmoc_offline_ind_s_type *offline_req;

  offline_req = (mmoc_offline_ind_s_type *)
                mmoc_mem_alloc(sizeof(mmoc_offline_ind_s_type));
  if(offline_req == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  mmoc_msgr_send_else_err_fatal( MM_MMOC_OFFLINE_IND,
                                 &(offline_req->hdr),
                                 sizeof(mmoc_offline_ind_s_type),
                                 MSGR_NO_VARIANT);
  mmoc_mem_free(offline_req);
}


/*===========================================================================

FUNCTION mmoc_send_ph_stat_chgd_cmd

DESCRIPTION
  This is a utility function called by the transaction state handlers to send
  the phone status changed command to the various supported modes. The command
  will be sent to one mode for every function call.

RETURN VALUE
  TRUE  - Phone status command was sent to support mode.
  FALSE - Completed sending the phone status command to all supported modes.

===========================================================================*/
static boolean                         mmoc_send_ph_stat_chgd_cmd
(
  prot_ph_stat_chg_e_type            chg_type,
  /* Phone status changed type.
  */

  mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = NULL;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  mmoc_mode_e_type mode = MMOC_MODE_NONE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the ph. status changed commands have already been sent.
  */
  if(mmoc_dev_info->is_ph_stat_sent)
  {
    /* Already sent, return FALSE,
    */
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Decide to whom the next command should be sent.
  */
  mmoc_dev_info->is_ph_stat_sent = TRUE;

#ifdef MMOC_CDMA_SUPPORTED
  mode = mmoc_convert_sys_mode_asubsid_to_mmoc_mode(SYS_SYS_MODE_CDMA, mmoc_dev_info->sub_with_3gpp2);
  mc_cdma_ph_status_chgd_cmd( mmoc_dev_info->sub_with_3gpp2,
                              chg_type,
                              mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->nam,
                              ((mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->sub_avail & PROT_SUBSC_MASK_3GPP2) != 0),
                              mmoc_dev_info->oprt_mode,
                              ph_stat_cmd_tbl[mode].trans_id =
                                mmoc_next_trans_id(mmoc_info_ptr),
                              mmoc_dev_info->subsc_chgd.sub_prop[mmoc_dev_info->sub_with_3gpp2]->prot_subsc_chg
                            );
#endif

#ifdef MMOC_HDR_SUPPORTED
  mode = mmoc_convert_sys_mode_asubsid_to_mmoc_mode(SYS_SYS_MODE_HDR, mmoc_dev_info->sub_with_3gpp2);
  mmoc_send_hdr_ph_status_chgd( chg_type,
                                mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->nam,
                                ((mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->sub_avail & PROT_SUBSC_MASK_3GPP2) != 0),
                                mmoc_dev_info->oprt_mode,
                                ph_stat_cmd_tbl[mode].trans_id =
                                  mmoc_next_trans_id(mmoc_info_ptr),
                                mmoc_dev_info->subsc_chgd.sub_prop[mmoc_dev_info->sub_with_3gpp2]->prot_subsc_chg
                              );
#endif

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    mode = mmoc_convert_sys_mode_asubsid_to_mmoc_mode(SYS_SYS_MODE_GWL, sub);

    cmregprx_cmd_gwl_ph_stat_chgd_per_subs(
      chg_type,
      (cm_subs_avail_e_type)(mmoc_sub_info[sub]->sub_avail & PROT_SUBSC_MASK_3GPP),
      mmoc_dev_info->oprt_mode,
      ph_stat_cmd_tbl[mode].trans_id = mmoc_next_trans_id(mmoc_info_ptr),
      PROT_SUBSC_CHG_NONE,
      MMGSDI_GW_PROV_PRI_SESSION,
      sub,
      SD_SS_MODE_PREF_NONE,
      sub,
      SYS_UE_MODE_NONE,
      FALSE);
  }

  mmoc_dev_info->is_ph_stat_sent = TRUE;
  return mmoc_dev_info->is_ph_stat_sent;

} /* mmoc_send_ph_stat_chgd_cmd() */


/*===========================================================================

FUNCTION mmoc_send_subsc_chg_ph_stat_chgd_cmd

DESCRIPTION
  This is a utility function called by the SUBSCRIPTION CHANGE transaction
  handler to send the phone status changed command to the various supported
  modes. The command will be sent to one mode for every function call.

RETURN VALUE
  TRUE  - Phone status command was sent to support mode.
  FALSE - Completed sending the phone status command to all supported modes.

===========================================================================*/
static boolean                         mmoc_send_subsc_chg_ph_stat_chgd_cmd
(
  prot_ph_stat_chg_e_type            chg_type,
  /* Phone status changed type.
  */

  mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = NULL;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  byte no_chg = 0;
  mmoc_mode_e_type mode = MMOC_MODE_NONE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT( mmoc_info_ptr != NULL );

  mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the ph. status changed commands have already been sent. */

  if(mmoc_dev_info->is_ph_stat_sent)
  {
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Determine if there is a subscription change on any sub */

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(mmoc_sub_info[sub]->prot_subsc_chg == PROT_SUBSC_CHG_NONE)
    {
      no_chg++;
    }
    else
    {
      break;
    }
  }

  if(no_chg == mmoc_info_ptr->nSubs)
  {
    MMOC_ERR_FATAL_0("Invalid prot_subsc_chg sent by CM");
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Decide to whom the next command should be sent. */

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(mmoc_dev_info->subsc_chgd.sub_prop[sub] != NULL &&
        CHECK_PROTO(mmoc_sub_info[sub]->prot_subsc_chg, PROT_SUBSC_CHG_ALL))
    {
#ifdef MMOC_GW_SUPPORTED
      cm_subs_avail_e_type subs_avail = CM_SUBS_NOT_AVAIL;

      if(mmoc_dev_info->trans == MMOC_TRANS_SUBSC_CHGD &&
          mmoc_dev_info->subsc_chgd.sub_prop[sub]->is_perso_locked == TRUE)
      {
        subs_avail = CM_SUBS_PERSO_LOCK;
      }
      else
      {
        subs_avail = ((mmoc_sub_info[sub]->sub_avail & PROT_SUBSC_MASK_3GPP) != 0)
                     ? CM_SUBS_AVAIL : CM_SUBS_NOT_AVAIL;
      }
      MMOC_MSG_HIGH_2("sub %d avail_type %d", sub, subs_avail);

      /* Send PH status chgd cmd to regprx */
      mode = mmoc_convert_sys_mode_asubsid_to_mmoc_mode(SYS_SYS_MODE_GWL, sub);

      cmregprx_cmd_gwl_ph_stat_chgd_per_subs(
        chg_type,
        subs_avail,
        mmoc_dev_info->oprt_mode,
        ph_stat_cmd_tbl[mode].trans_id = mmoc_next_trans_id(mmoc_info_ptr),
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->prot_subsc_chg,
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->gpp_session_type,
        sub,
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->mode_pref,
        sub,
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->ue_mode,
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->is_ue_mode_substate_srlte);

      mmoc_dev_info->is_ph_stat_sent = TRUE;
#endif

      /* Send PH status chgd cmd to 1x/HDR */
      if(CHECK_PROTO(mmoc_sub_info[sub]->prot_subsc_chg, PROT_SUBSC_CHG_3GPP2) &&
          sub == mmoc_dev_info->sub_with_3gpp2)
      {
#ifdef MMOC_CDMA_SUPPORTED
        mode = mmoc_convert_sys_mode_asubsid_to_mmoc_mode(SYS_SYS_MODE_CDMA, sub);

        mc_cdma_ph_status_chgd_cmd( mmoc_dev_info->sub_with_3gpp2,
                                    chg_type,
                                    mmoc_sub_info[sub]->nam,
                                    ((mmoc_sub_info[sub]->sub_avail & PROT_SUBSC_MASK_3GPP2) != 0),
                                    mmoc_dev_info->oprt_mode,
                                    ph_stat_cmd_tbl[mode].trans_id = mmoc_next_trans_id(mmoc_info_ptr),
                                    mmoc_dev_info->subsc_chgd.sub_prop[sub]->prot_subsc_chg
                                  );
        mmoc_dev_info->is_ph_stat_sent = TRUE;
#endif

#ifdef MMOC_HDR_SUPPORTED
        mode = mmoc_convert_sys_mode_asubsid_to_mmoc_mode(SYS_SYS_MODE_HDR, sub);

        mmoc_send_hdr_ph_status_chgd( chg_type,
                                      mmoc_sub_info[sub]->nam,
                                      ((mmoc_sub_info[sub]->sub_avail & PROT_SUBSC_MASK_3GPP2) != 0),
                                      mmoc_dev_info->oprt_mode,
                                      ph_stat_cmd_tbl[mode].trans_id = mmoc_next_trans_id(mmoc_info_ptr),
                                      mmoc_dev_info->subsc_chgd.sub_prop[sub]->prot_subsc_chg
                                    );
        mmoc_dev_info->is_ph_stat_sent = TRUE;
#endif
      }
    }
  }

  return mmoc_dev_info->is_ph_stat_sent;

} /* mmoc_send_subsc_chg_ph_stat_chgd_cmd() */


/*===========================================================================

FUNCTION mmoc_dual_standby_pref_chgd

DESCRIPTION
  This is a utility function called to check if dual standby preference has changed

RETURN VALUE
  TRUE  - If pref has changed
  FALSE - If pref is same

===========================================================================*/

static boolean mmoc_dual_standby_pref_chgd
(
  mmoc_state_info_s_type    *mmoc_info_ptr
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;

  MMOC_MSG_HIGH_4("standby_pref %d prev_standby_pref %d active_subs %d prev_active_subs %d",
                  mmoc_dev_info->standby_pref,
                  mmoc_dev_info->prev_standby_pref,
                  mmoc_dev_info->active_subs,
                  mmoc_dev_info->prev_active_subs);

  return(mmoc_dev_info->standby_pref != mmoc_dev_info->prev_standby_pref ||
         mmoc_dev_info->active_subs  != mmoc_dev_info->prev_active_subs);

}

/*===========================================================================

FUNCTION mmoc_send_subsc_chgd_sd_nam_sel_cmd

DESCRIPTION
  This is a utility function called to send nam sel cmd to sd,


RETURN VALUE
  TRUE  - Phone status command was sent to support mode.
  FALSE - Completed sending the phone status command to all supported modes.

===========================================================================*/
static void  mmoc_send_subsc_chgd_sd_nam_sel_cmd
(

  mmoc_state_info_s_type         *mmoc_info_ptr,
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

  mmoc_state_info_sub_prop_s_type *mmoc_sub_info_ptr,
  /* Pointer for MMOC sub info for that sub */

  sys_modem_as_id_e_type         sub

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_rpt_subs_sd_nam_sel_cnf_s_type *context = NULL;

  if((context = mmoc_mem_alloc(sizeof(mmoc_rpt_subs_sd_nam_sel_cnf_s_type)))== NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  context->asubs_id = sub;
  context->trans_id = mmoc_next_trans_id(mmoc_info_ptr);

  /* Set nam sel mask for this sub */
  mmoc_dev_info->sd_nam_sel_mask |= BM(sub);

  sd_nam_sel4(mmoc_rpt_sd_nam_sel_cnf,
              (void *)context,
              sub,
              (sd_nam_e_type) mmoc_sub_info_ptr->nam,
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->orig_mode,
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->mode_pref,
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->band_pref,
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->lte_band_pref,
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->tds_band_pref,
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->prl_pref,
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->roam_pref,
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->hybr_pref,
              &mmoc_dev_info->subsc_chgd.sub_prop[sub]->manual_sys_info,
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->srv_domain_pref,
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->acq_order_pref,
              ((mmoc_dev_info->subsc_chgd.sub_prop[sub]->sub_avail & PROT_SUBSC_MASK_3GPP) != 0),
              ((mmoc_dev_info->subsc_chgd.sub_prop[sub]->sub_avail & PROT_SUBSC_MASK_3GPP2) != 0),
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->subs_capability,
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->active_stacks,
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->voice_domain_pref,
              ((sub == SYS_MODEM_AS_ID_1)?mmoc_dev_info->subsc_chgd.sub_prop[sub]->camp_mode:SD_SS_PREF_CAMP_MODE_OFF),
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->fullrat_status,
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->rat_capability
             );

  return ;

}


#ifdef FEATURE_MMODE_DUAL_SIM
/*===========================================================================

FUNCTION mmoc_subs_capability_change

DESCRIPTION
  This is a utility function called to check if subs capability has changed

RETURN VALUE
  TRUE  - If pref has changed
  FALSE - If pref is same

===========================================================================*/
static boolean mmoc_subs_capability_chgd
(
  mmoc_state_info_s_type    *mmoc_info_ptr
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  boolean subs_capability_chgd = FALSE;
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  if(!mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM))
  {
    return FALSE;
  }

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(mmoc_dev_info->subsc_chgd.sub_prop[sub] != NULL &&
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->subs_capability != mmoc_sub_info[sub]->subs_capability &&
        CHECK_PROTO(mmoc_sub_info[sub]->prot_subsc_chg, PROT_SUBSC_CHG_ALL))
    {
      subs_capability_chgd = TRUE;
      mmoc_dev_info->sub_with_cap_chg = sub;
      mmoc_sub_info[sub]->subs_capability = mmoc_dev_info->subsc_chgd.sub_prop[sub]->subs_capability;
    }
  }

  return subs_capability_chgd;

}
#endif

#ifdef MMOC_HDR_SUPPORTED
/*===========================================================================

@FUNCTION mmoc_send_hdr_ue_mode_chg_req

@DESCRIPTION
  Send ue mode change request to HDR.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
void mmoc_send_hdr_ue_mode_chg_req
(
  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  hdrcp_msg_req_u      *hdrcp_req;

  hdrcp_req = (hdrcp_msg_req_u *) modem_mem_alloc(sizeof(hdrcp_msg_req_u),
              MODEM_MEM_CLIENT_MMODE);
  mmoc_modem_heap_blocks++;

  if(hdrcp_req == NULL)
  {
    sys_err_fatal_null_ptr_exception();
  }

  memset(hdrcp_req, 0, sizeof(hdrcp_msg_req_u));

  hdrcp_req->ue_mode_chg_req.ue_mode            =
    mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.param.pref_sys_chgd.addl_action.ue_mode;

  hdrcp_req->ue_mode_chg_req.is_sub_state_srlte =
    mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.param.pref_sys_chgd.addl_action.is_ue_mode_substate_srlte;

  hdrcp_req->ue_mode_chg_req.trans_id = mmoc_next_trans_id(mmoc_info_ptr);

  MMOC_MSG_HIGH_3("CM->HDR: ue_mode_change request %d %d %d",
                  hdrcp_req->ue_mode_chg_req.ue_mode,
                  hdrcp_req->ue_mode_chg_req.is_sub_state_srlte,
                  mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.asubs_id);

  mmoc_msgr_send_else_err_fatal( HDR_CP_UE_MODE_CHANGE_REQ,
                                 &(hdrcp_req->hdr),
                                 sizeof(hdrcp_msg_req_u),
                                 mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.asubs_id);
  mmoc_mem_free(hdrcp_req);

  return;
}
#endif

/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_send_mmgsdi_card_read_req

DESCRIPTION
  This is a utility function called by the SUBSCRIPTION CHANGE transaction
  handler to check whether read request needs to be sent to MMGSDI card.

RETURN VALUE
  TRUE  - Send Read request to MMGSDI card.
  FALSE - No more sending Read request to MMGSDI.

===========================================================================*/
static boolean                         mmoc_send_mmgsdi_card_read_req
(
  void
)
{
  mmoc_mmgsdi_info_s_type   *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();
  mmoc_state_info_s_type    *mmoc_info_ptr = mmoc_get_state_info_ptr();
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  boolean read_items = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT( mmgsdi_info_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    MMOC_MSG_HIGH_2("sub %d read_items = 0x%x", sub, mmgsdi_info_ptr->read_items[sub]);

    if(mmgsdi_info_ptr->read_items[sub] != MMOC_MMGSDI_MASK_NONE)
    {
      read_items = TRUE;
    }
  }
  return read_items;

} /* mmoc_send_mmgsdi_card_read_req() */


#if defined(FEATURE_MMOC_LTE)
/*===========================================================================

FUNCTION mmoc_send_lte_rrc_mode_pref_update

DESCRIPTION
  Send mode pref update to LTE RRC on other sub if required

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                           mmoc_send_lte_rrc_mode_pref_update()
{
  lte_rrc_mode_pref_update_ind_s  *mode_pref_update_ind;
  mmoc_state_info_s_type    *mmoc_info_ptr = mmoc_get_state_info_ptr();
  static sd_ss_mode_pref_e_type lte_mode_pref_indicated[MAX_AS_IDS] = {SD_SS_MODE_PREF_NONE};
  uint8 index = 0;
  uint8 active_subs =0;
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;

  if(!mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM))
  {
    /* No need to send mode change to LTE RRC if not msim*/
    return;
  }

  if(mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_PROT_GEN_CMD)
  {
    sys_modem_as_id_e_type     asubs_id = SYS_MODEM_AS_ID_1;
    prot_gen_cmd_s_type        cmd_info = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info;

    if(cmd_info.asubs_id != SYS_MODEM_AS_ID_1)
    {
      /* Not accounting for TSTS deliberately*/
      asubs_id = SYS_MODEM_AS_ID_2;
    }

    if(cmd_info.param.pref_sys_chgd.user_mode_pref != lte_mode_pref_indicated[asubs_id])
    {
      lte_mode_pref_indicated[asubs_id] = cmd_info.param.pref_sys_chgd.user_mode_pref;
    }
    else
    {
      /* If no change in preferences, no update needed for LTE RRC */
      return;
    }
  }
  else if(mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_SUBSC_CHGD)
  {
    sd_ss_mode_pref_e_type mode_pref[MAX_AS_IDS] = {SD_SS_MODE_PREF_NONE};

    for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
    {
      if(mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub] != NULL)
      {
        mode_pref[sub] = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->mode_pref;
      }
    }

    if(mmoc_info_ptr->dev_prop.active_subs == mmoc_info_ptr->dev_prop.prev_active_subs)
    {
      boolean ret = TRUE;

      for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
      {
        if(mode_pref[sub] != lte_mode_pref_indicated[sub])
        {
          ret = FALSE;
          break;
        }
      }

      if(ret)
      {
        /* If no change in preferences and active subs, no update needed for LTE RRC */
        return;
      }
    }

    if(mode_pref[SYS_MODEM_AS_ID_1] == lte_mode_pref_indicated[SYS_MODEM_AS_ID_1] &&
        mode_pref[SYS_MODEM_AS_ID_2] == lte_mode_pref_indicated[SYS_MODEM_AS_ID_2] &&
        mmoc_info_ptr->dev_prop.active_subs == mmoc_info_ptr->dev_prop.prev_active_subs)
    {
      /* If no change in preferences and active subs, no update needed for LTE RRC */
      return;
    }

    for(index = 0; index < MAX_AS_IDS; index++)
    {
      lte_mode_pref_indicated[index] = mode_pref[index];
    }
  }
  else if(mmoc_info_ptr->dev_prop.trans != MMOC_TRANS_DUAL_STANDBY_CHGD ||
          mmoc_info_ptr->dev_prop.active_subs == mmoc_info_ptr->dev_prop.prev_active_subs)
  {
    /* For all other transactions, neither preferences not active subs can change*/
    return;
  }

  mode_pref_update_ind = (lte_rrc_mode_pref_update_ind_s *) mmoc_mem_alloc(sizeof(lte_rrc_mode_pref_update_ind_s));

  if(mode_pref_update_ind == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

#ifdef FEATURE_MMODE_TRIPLE_SIM
  if(mmoc_info_ptr->dev_prop.device_mode == SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY)
  {
    mode_pref_update_ind->num_subs = 3;
  }
  else
#endif
  if(mmoc_info_ptr->dev_prop.device_mode == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE ||
          mmoc_info_ptr->dev_prop.device_mode == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY)
  {
    mode_pref_update_ind->num_subs = 2;
  }
  else
  {
    mode_pref_update_ind->num_subs = 1;
  }

  for(index = 0; index < MAX_AS_IDS; index++)
  {
    mode_pref_update_ind->sub_info[index].asubs_id = (sys_modem_as_id_e_type)index;

    mode_pref_update_ind->sub_info[index].mode_pref_mask =
      sys_map_sd_mode_pref_to_sys_mode_mask(lte_mode_pref_indicated[index]);

    mode_pref_update_ind->sub_info[index].is_active = (mmoc_info_ptr->dev_prop.active_subs & BM(index));

  }

  if (mmoc_info_ptr->dev_prop.device_mode == SYS_MODEM_DEVICE_MODE_SINGLE_SIM)
  {
    MMOC_MSG_HIGH_3("LTE_RRC_MODE_PREF_UPDATE_IND: asubs_id:%d, is_active:%d, mode_pref_mask:%d ",
                        mode_pref_update_ind->sub_info[0].asubs_id,
                        mode_pref_update_ind->sub_info[0].is_active,
                        mode_pref_update_ind->sub_info[0].mode_pref_mask);
  }
  else
  {
    MMOC_MSG_HIGH_6("LTE_RRC_MODE_PREF_UPDATE_IND: asubs_id:%d, is_active:%d, mode_pref_mask:%d "
                    "asubs_id:%d, is_active:%d, mode_pref_mask:%d",
                    mode_pref_update_ind->sub_info[0].asubs_id,
                    mode_pref_update_ind->sub_info[0].is_active,
                    mode_pref_update_ind->sub_info[0].mode_pref_mask,
                    mode_pref_update_ind->sub_info[1].asubs_id,
                    mode_pref_update_ind->sub_info[1].is_active,
                    mode_pref_update_ind->sub_info[1].mode_pref_mask);
  }
  mmoc_msgr_send_else_err_fatal( LTE_RRC_MODE_PREF_UPDATE_IND,
                                 &(mode_pref_update_ind->msg_hdr),
                                 sizeof(lte_rrc_mode_pref_update_ind_s),
                                 MSGR_NO_VARIANT);

  mmoc_mem_free(mode_pref_update_ind);

  return;


} /* mmoc_send_lte_rrc_mode_pref_update() */
#endif


/*===========================================================================

FUNCTION mmoc_send_prot_gen_cmd

DESCRIPTION
  This is a utility function called by the transaction state handlers to send
  the generic command to the active protocol.

RETURN VALUE
  TRUE  - Command was sent to the active protocol.
  FALSE - Command was not sent, because no protocol was active.

===========================================================================*/
static boolean                         mmoc_send_prot_gen_cmd
(
  prot_gen_cmd_s_type             *gen_cmd_ptr,
  /* Pointer to the generic protocol command.
  */

  mmoc_state_info_s_type          *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  boolean is_cmd_sent = TRUE;

  mm_sub_stk_id_s_type mm_id;
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = NULL;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];

  MMOC_ASSERT( mmoc_info_ptr != NULL );
  MMOC_ASSERT( gen_cmd_ptr != NULL );

  mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));
  mm_id.asubs_id = gen_cmd_ptr->asubs_id;
  mm_id.stk_id   = gen_cmd_ptr->stk_id;

  if(mm_id.asubs_id < SYS_MODEM_AS_ID_1 || mm_id.asubs_id >= MAX_SIMS ||
      mm_id.stk_id < MM_STACK_0 || mm_id.stk_id >= MAX_STACKS ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id] == NULL ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id] == NULL)
  {
    return FALSE;
  }

  /* check if we have auto_deact from protcol to which we want to send
  ** prot_gen_cmd. If yes, protocol has already deactivated itself.
  ** So, set prot_state to NULL and discard buffered auto_deact.
  */
  mmoc_is_auto_deact_ind_rpt_buf_per_stack(mmoc_info_ptr,mm_id,TRUE);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check on the active protocol.
  */
  switch(mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state)
  {
#ifdef MMOC_CDMA_SUPPORTED
    case PROT_STATE_ONLINE_CDMA:
    case PROT_STATE_ONLINE_DED_MEAS:
      gen_cmd_ptr->trans_id = mmoc_next_trans_id(mmoc_info_ptr);
      mc_cdma_generic_prot_cmd( gen_cmd_ptr->asubs_id, gen_cmd_ptr );
      break;
#endif

#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
    case PROT_STATE_ONLINE_GWL:
      gen_cmd_ptr->trans_id = mmoc_next_trans_id(mmoc_info_ptr);
      cmregprx_cmd_gwl_generic_cmd( gen_cmd_ptr );
      break;
#endif

#ifdef MMOC_HDR_SUPPORTED
    case PROT_STATE_ONLINE_HDR:
      gen_cmd_ptr->trans_id = mmoc_next_trans_id(mmoc_info_ptr);
      mmoc_send_hdr_generic_prot_cmd( gen_cmd_ptr );
      break;
#endif

    case PROT_STATE_FTM:
    case PROT_STATE_LPM:
    case PROT_STATE_OFFLINE:
    case PROT_STATE_OFFLINE_CDMA:
    case PROT_STATE_PWR_DOWN:
    case PROT_STATE_RESET:
      sys_err_fatal_invalid_value_exception((int)(mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state));
      break;

    case PROT_STATE_BLOCKED:
    case PROT_STATE_GPSONE_MSBASED:
    case PROT_STATE_NULL:
    case PROT_STATE_PWR_SAVE:
    case PROT_STATE_MAX:
    default:
      is_cmd_sent = FALSE;
      MMOC_MSG_LOW_0(" No active protocol to send the generic command");
      break;
  } /* switch( mmoc_info_ptr->prot_state ) */

  MMOC_MSG_HIGH_4("MMOC->PROT: mmoc_send_prot_gen_cmd(), sub %d stk %d, prot_state=%d, is_cmd_sent=%d",
                  mm_id.asubs_id,
                  mm_id.stk_id,
                  mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state,
                  is_cmd_sent);

  return is_cmd_sent;

} /* mmoc_send_prot_generic_cmd() */



/*===========================================================================
=============================================================================
=============================================================================
==================== MMoC TASK RELATED HELPER FUNCTIONS =====================
=============================================================================
=============================================================================
===========================================================================*/


/*===========================================================================

FUNCTION mmoc_get_cmd_buf_else_err_fatal

DESCRIPTION
  Allocate a MMoC task command buffer. Error fatal if allocation is NOT
  successful.

DEPENDENCIES
  none

RETURN VALUE
  Pointer to allocated MMoC command buffer.

SIDE EFFECTS
  none
===========================================================================*/
#ifdef FEATURE_NON_DEMAND_PAGED_FUNCTION
#error code not present
#endif
mmoc_cmd_msg_s_type*            mmoc_get_cmd_buf_else_err_fatal
(

  void

)
{

  /* Try allocating a MMoC Task command buffer.
  */
  mmoc_cmd_msg_s_type *mmoc_cmd_ptr = NULL;

  if((mmoc_cmd_ptr = mmoc_mem_alloc(sizeof(mmoc_cmd_msg_s_type))) == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return NULL;
  }

  /* If we got here, allocation is successful,
  ** so no queue to place command buffer on when done,
  ** and specify NO task to be signalled when done
  */
  mmoc_cmd_ptr->cmd.hdr.done_q_ptr      = NULL;
  mmoc_cmd_ptr->cmd.hdr.task_ptr        = NULL;


  return mmoc_cmd_ptr;

} /* mmoc_get_cmd_buf_else_err_fatal() */
#ifdef FEATURE_NON_DEMAND_PAGED_FUNCTION
#error code not present
#endif
/* <EJECT> */
#if defined(FEATURE_MMODE_ML_LOGGING)
/*===========================================================================

FUNCTION mmoc_fetch_source_task

DESCRIPTION
  This function fetches the source task which is responsible for sending the command to MMOC.

DEPENDENCIES
  The MMoC task must have been started up already.  All of the items in
  the mmoc_cmd_msg_s_type must be already set.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
#ifdef FEATURE_NON_DEMAND_PAGED_FUNCTION
#error code not present
#endif
static log_inter_layer_module_type                mmoc_fetch_source_task
(

  mmoc_cmd_msg_s_type             *msg_ptr
  /* Pointer to MMoC command message buffer.
  */

)
{
  switch(msg_ptr->cmd.name)
  {
    case MMOC_CMD_MMGSDI_INFO_IND:  
	  return LOG_ML_MMGSDI;

	case MMOC_CMD_MCFG_REFRESH:    
	  return LOG_ML_MCFG;

	case MMOC_CMD_PROT_REDIR_IND:
      if(msg_ptr->param.prot_redir_ind.prot_state == PROT_STATE_ONLINE_CDMA)
	  {
	    return LOG_ML_CDMA;
	  }
	  else
	  {
	    return LOG_ML_HDR;
	  }
	case MMOC_CMD_PROT_HO_IND:
	  return LOG_ML_CDMA;

	default:
	  return LOG_ML_CM;

  }
}
/*===========================================================================

FUNCTION mmoc_fetch_as_id

DESCRIPTION
  This function fetches the as_id on which cmd is being sent.

DEPENDENCIES
  The MMoC task must have been started up already.  All of the items in
  the mmoc_cmd_msg_s_type must be already set.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
#ifdef FEATURE_NON_DEMAND_PAGED_FUNCTION
#error code not present
#endif
static sys_modem_as_id_e_type                mmoc_fetch_as_id
(

  mmoc_cmd_msg_s_type             *msg_ptr
  /* Pointer to MMoC command message buffer.
  */

)
{
  switch(msg_ptr->cmd.name)
  {
    case MMOC_CMD_PROT_GEN_CMD:  
	  return msg_ptr->param.gen_prot_cmd.asubs_id;

	case MMOC_CMD_SUBSCRIPTION_CHGD:   
      #ifdef FEATURE_MMODE_DUAL_SIM
	  if(msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_1]->prot_subsc_chg && msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_2] != NULL && msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_2]->prot_subsc_chg)
	  {
        return SYS_MODEM_AS_ID_NONE;
	  }
	  else if(msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_1]->prot_subsc_chg)
	  {
	    return SYS_MODEM_AS_ID_1;  
	  }
	  else
	  {
        return SYS_MODEM_AS_ID_2;
	  }
      #else
	    if(msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_1]->prot_subsc_chg)
		{
		  return SYS_MODEM_AS_ID_1;
		}
		else
		{
          return SYS_MODEM_AS_ID_NONE;
		}
      #endif
	case MMOC_CMD_PROT_REDIR_IND:
	  return msg_ptr->param.prot_redir_ind.asubs_id;

	case MMOC_CMD_PROT_HO_IND:
       return msg_ptr->param.prot_ho_ind.asubs_id;

	case MMOC_CMD_SUSPEND_STACK:
	  return msg_ptr->param.suspend_stack.mm_id.asubs_id;

	case MMOC_CMD_DEACT_FROM_DORMANT:
	  return msg_ptr->param.deact_dormant.mm_id.asubs_id;

	case MMOC_CMD_MCFG_REFRESH:
	  return msg_ptr->param.mcfg_refresh.as_id;

	default:
	  return SYS_MODEM_AS_ID_NONE;

  }
}
#endif
/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_send_cmd

DESCRIPTION
  This function takes an already filled out mmoc_cmd_msg_type and places it
  on the mmoc command queue.

DEPENDENCIES
  The MMoC task must have been started up already.  All of the items in
  the mmoc_cmd_msg_s_type must be already set.

RETURN VALUE
  None.

SIDE EFFECTS
  The mmoc_cmd_msg_s_type is placed on the MMoC command queue.  It must not be
  modified until the command has been processed.
===========================================================================*/
#ifdef FEATURE_NON_DEMAND_PAGED_FUNCTION
#error code not present
#endif
void                            mmoc_send_cmd
(

  mmoc_cmd_msg_s_type             *msg_ptr
  /* Pointer to MMoC command message buffer.
  */

)
{
  /* Pointer to MMoC's task info.
  */
  #if defined(FEATURE_MMODE_ML_LOGGING)
  log_inter_layer_msg_type* log_msg;
  #endif
  mmoc_task_info_s_type  *mmoc_task_ptr = mmoc_get_task_info_ptr();

  if(msg_ptr == NULL)
  {
    return;
  }
  #if defined(FEATURE_MMODE_ML_LOGGING)
  log_msg =(log_inter_layer_msg_type*)mmoc_mem_alloc(sizeof(log_inter_layer_msg_type));
  
  if(log_msg == NULL)
  {
    return;
  }
  
  log_msg->msg_id = (byte)msg_ptr->cmd.name;
  log_msg->src_sub_id = mmoc_fetch_as_id(msg_ptr);
  log_msg->dest_sub_id = mmoc_fetch_as_id(msg_ptr);
  log_msg->source_module = mmoc_fetch_source_task(msg_ptr);
      
  log_msg->dest_module = LOG_ML_MMOC;
  log_inter_layer_message(log_msg);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  #endif
 
  /* Initialize and link the command onto the MC command queue.
  */
  (void) q_link(msg_ptr, &msg_ptr->cmd.hdr.link);
  q_put(&mmoc_task_ptr->cmd_q, &msg_ptr->cmd.hdr.link);


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Set the REX signal corresponding to the MMoC's command queue and
  ** return.
  */
  (void) rex_set_sigs(mmoc_tcb_ptr, MMOC_CMD_Q_SIG);
  #if defined(FEATURE_MMODE_ML_LOGGING)
  mmoc_mem_free(log_msg);
  #endif

} /* mmoc_send_cmd() */
#ifdef FEATURE_NON_DEMAND_PAGED_FUNCTION
#error code not present
#endif

/*===========================================================================

FUNCTION mmoc_handle_deact_for_dual_switch

DESCRIPTION
  This is a function for handling the transaction state DEACT_ENTER for the
  subscription changed transaction .

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Should wait for deactivated confirmation.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start ph_stat_enter processing.

===========================================================================*/
static boolean  mmoc_handle_deact_for_dual_switch
(

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  boolean is_deact_req_sent = FALSE;
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stack = MM_STACK_NONE;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  if(mmoc_is_multi_standby(mmoc_dev_info->standby_pref) &&
      mmoc_dev_info->prev_standby_pref == SYS_MODEM_DS_PREF_SINGLE_STANDBY
    )
  {
    mmoc_dev_info->dual_switch_subs = mmoc_dev_info->prev_active_subs;
  }
  else if(mmoc_is_multi_standby(mmoc_dev_info->prev_standby_pref) &&
          mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_SINGLE_STANDBY)
  {
    mmoc_dev_info->dual_switch_subs = mmoc_dev_info->active_subs;
  }
  else
  {
    mmoc_dev_info->dual_switch_subs = SYS_MODEM_AS_ID_NO_ACTIVE_MASK;
    return FALSE;
  }

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(mmoc_dev_info->dual_switch_subs & BM(sub))
    {
      for(stack = MM_STACK_0; stack < mmoc_sub_info[sub]->nStacks && stack < MAX_STACKS; stack++)
      {
        if(!is_deact_req_sent)
        {
          mmoc_dev_info->deact_req_idx.asubs_id = sub;
          mmoc_dev_info->deact_req_idx.stk_id = stack;

          is_deact_req_sent = mmoc_deactivate_active_protocol(
                                mmoc_dev_info->deact_req_idx,
                                PROT_DEACT_DUAL_SWITCH,
                                mmoc_info_ptr);
        }
      }
      #if defined MMOC_GWL_SUPPORTED
      for(stack = (mmoc_sub_info[sub]->nStacks - 1); stack > MM_STACK_NONE; stack--)
      {
        if(!is_deact_req_sent && stack != MM_STACK_1 && (mmoc_sub_info[sub]->prev_active_stacks & BM(stack)))
        {
          mmoc_dev_info->deact_req_idx.asubs_id = sub;
          mmoc_dev_info->deact_req_idx.stk_id = stack;
          is_deact_req_sent = mmoc_deactivate_gwl_protocol(
                                mmoc_dev_info->deact_req_idx,
                                PROT_DEACT_DUAL_SWITCH,
                                mmoc_info_ptr);

          if(is_deact_req_sent)
          {
            mmoc_update_trans_state(MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL,
                                    mmoc_info_ptr
                                   );
          }
        }
      }
#endif
    }
  }

  return is_deact_req_sent;

}


/* <EJECT> */

/*===========================================================================

FUNCTION mmoc_handle_deact_for_lpm


DESCRIPTION
  This is a function for handling the transaction state DEACT_ENTER for the
  subscription changed transaction .

DEPENDENCIES
  None.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Should wait for deactivated confirmation.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start ph_stat_enter processing.


SIDE EFFECTS
  None
===========================================================================*/
static boolean  mmoc_handle_deact_for_lpm
(

  mmoc_state_info_s_type         *mmoc_info_ptr,
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

  mm_sub_stk_id_s_type            mm_id
)
{
  boolean is_deact_req_sent = FALSE;
  if(mmoc_info_ptr->dev_prop.prev_active_subs & BM(mm_id.asubs_id) &&
      !(mmoc_info_ptr->dev_prop.active_subs & BM(mm_id.asubs_id)))
  {
    mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id = mm_id.asubs_id;
    mmoc_info_ptr->dev_prop.deact_req_idx.stk_id = mm_id.stk_id;

    is_deact_req_sent= mmoc_deactivate_active_protocol(mm_id,PROT_DEACT_LPM,mmoc_info_ptr);
   
    if(!is_deact_req_sent && mm_id.stk_id == MM_STACK_0)
    {
      #if defined MMOC_GWL_SUPPORTED
      is_deact_req_sent = mmoc_deactivate_gwl_protocol(
                                mm_id,
                                PROT_DEACT_LPM,
                                mmoc_info_ptr);
      if(is_deact_req_sent)
      {
        mmoc_update_trans_state(MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL,
                                    mmoc_info_ptr
                                   );

      }
      #endif
  }

  }

  return is_deact_req_sent;

}

/*===========================================================================

FUNCTION mmoc_process_dual_standby_chgd_null

DESCRIPTION
  This function handles the dual standby changed transaction in the NULL
  transaction state.

  In this state, the state handler will

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_dual_standby_chgd_null
(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  mmoc_state_info_s_type               *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  MMOC_ASSERT( cmd_ptr != NULL );
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* The subscription changed command from CM should only create this
  ** transaction.
  */
  if ( cmd_ptr == NULL ||
       cmd_ptr->cmd.name != MMOC_CMD_DUAL_STANDBY_CHGD
     )
  {
    MMOC_ERR_0("Dual standby chgd trans incorrectly initiated");

    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  MMOC_MSG_HIGH_6("DS_CHGD: standby_pref %d prev_standby_pref %d active_subs %d prev_active_subs %d device_mode %d prev_device_mode %d",
                  mmoc_dev_info->standby_pref,
                  mmoc_dev_info->prev_standby_pref,
                  mmoc_dev_info->active_subs,
                  mmoc_dev_info->prev_active_subs,
                  mmoc_dev_info->device_mode,
                  mmoc_dev_info->prev_device_mode);

  /* copy new standby info into the current fields */
  mmoc_dev_info->standby_pref = cmd_ptr->param.dual_standby_chgd.standby_pref;
  mmoc_dev_info->active_subs  = cmd_ptr->param.dual_standby_chgd.active_subs;

  MMOC_MSG_HIGH_2("DS_CHGD: new standby_pref %d new active_subs %d",
                  mmoc_dev_info->standby_pref,
                  mmoc_dev_info->active_subs);


  /* Check if dual standby preference changed */
  if(!mmoc_dual_standby_pref_chgd(mmoc_info_ptr))
  {
#ifdef FEATURE_MMODE_DUAL_SIM
    if(mmoc_dev_info->device_mode != cmd_ptr->param.dual_standby_chgd.device_mode)
    {
      mmoc_dev_info->prev_device_mode = mmoc_dev_info->device_mode;
      mmoc_dev_info->device_mode = cmd_ptr->param.dual_standby_chgd.device_mode;

      cmregprx_cmd_gw_ds_stat_chgd( mmoc_dev_info->standby_pref,
                                    mmoc_next_trans_id(mmoc_info_ptr),
                                    mmoc_dev_info->active_subs,
                                    mmoc_dev_info->device_mode);

      /* DS status chgd request sent
      */
      mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DS_STAT_CHGD_CNF,
                               mmoc_info_ptr
                             );
      return MMOC_EVT_STATUS_CONSUMED;
    }
    else
#endif
    {
      cm_mcfg_proceed_with_refresh_type    mcfg_refresh_proceed;
      mcfg_refresh_proceed.active_subs = mmoc_dev_info->active_subs;

      /* Send the CM_STANDY_PREF_CHG_OK_F report to CM.
      */
      MMOC_MSG_HIGH_1("MCFG_Refresh: sending from ds null active_subs %d",mcfg_refresh_proceed.active_subs);
      mmoc_send_cm_rpt( CM_MCFG_PROCEED_WITH_REFRESH_F, (cm_sd_rpt_u_type *)&mcfg_refresh_proceed,
                        SYS_MODEM_AS_ID_1); // hard-coded to send on Sub 1, no use, just to fulfill signature demand

      /* Clear the transaction.
      */
      return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
    }
  }//Check if dual standby preference changed

  /* Update device mode in MMOC */
  mmoc_dev_info->device_mode = cmd_ptr->param.dual_standby_chgd.device_mode;

  /* Move to DEACT ENTER state */
  mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_DEACT_ENTER,
                           mmoc_info_ptr
                         );

  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_dual_standby_chgd_null() */


/*===========================================================================

FUNCTION mmoc_process_dual_standby_chgd_deact_enter

DESCRIPTION
  This function handles the dual standby changed transaction in the NULL
  transaction state.

  In this state, the state handler will

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_dual_standby_chgd_deact_enter
(

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  boolean is_deact_req_sent = FALSE;
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stack = MM_STACK_NONE;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    for(stack = MM_STACK_0; stack < mmoc_sub_info[sub]->nStacks && stack < MAX_STACKS; stack++)
    {
      if(!is_deact_req_sent && mmoc_sub_info[sub]->active_stacks & BM(stack))
      {
        mm_sub_stk_id_s_type mm_id;
        mm_id.asubs_id = sub;
        mm_id.stk_id = stack;

        is_deact_req_sent = mmoc_handle_deact_for_lpm(mmoc_info_ptr,mm_id);
      }
    }
  }

  if(mmoc_info_ptr->dev_prop.trans_state == MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL)
  {
    return MMOC_EVT_STATUS_CONSUMED;
  }
  
  if(is_deact_req_sent)
  {
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DEACTD_CNF,
                             mmoc_info_ptr
                           );

    return MMOC_EVT_STATUS_CONSUMED;
  }


  if(!is_deact_req_sent &&
      mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM) &&
      (cm_get_device_mode() != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE ||
       mmoc_is_sxlte(mmoc_dev_info->sub_with_3gpp2)))

  {
    is_deact_req_sent = mmoc_handle_deact_for_dual_switch(mmoc_info_ptr);
  }

  if(mmoc_info_ptr->dev_prop.trans_state == MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL)
  {
    return MMOC_EVT_STATUS_CONSUMED;
  }
  
  if(is_deact_req_sent)
  {
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DEACTD_CNF,
                             mmoc_info_ptr
                           );

    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If no active protocols present or feature mode is single SIM,
  ** start sending the dual standby status changed command.
  */
  if(!mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM) ||
      mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_NO_STANDBY_PREF)
  {
    cm_mcfg_proceed_with_refresh_type    mcfg_refresh_proceed;
    mcfg_refresh_proceed.active_subs = mmoc_dev_info->active_subs;

    /* This check only applies for single sim case */
    if(mmoc_dev_info->active_subs & SYS_MODEM_AS_ID_1_MASK &&
        mmoc_dev_info->oprt_mode == PROT_OPRT_MODE_ONLINE)
    {
      sd_ss_act_e_type sd_act = sd_ss_ind_user_pwr_up( TRUE, SYS_MODEM_AS_ID_1, NULL );
      (void)mmoc_activate_prot_online_ss_main(sd_act, SYS_MODEM_AS_ID_1, mmoc_info_ptr);
    }
    else
    {
      MMOC_MSG_HIGH_0("Not activating SD as phone is not online");
    }

    /* Send the CM_STANDY_PREF_CHG_OK_F report to CM.
    */
    MMOC_MSG_HIGH_1("MCFG_Refresh: sending from ds chgd deact enter active_subs %d",mcfg_refresh_proceed.active_subs);
    mmoc_send_cm_rpt( CM_MCFG_PROCEED_WITH_REFRESH_F, (cm_sd_rpt_u_type *)&mcfg_refresh_proceed, SYS_MODEM_AS_ID_1);

    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

#ifdef FEATURE_MMODE_DUAL_SIM
  mmoc_update_trans_state( MMOC_TRANS_STATE_DS_STAT_CHGD_ENTER,
                           mmoc_info_ptr
                         );
#endif

  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_dual_standby_chgd_deact_enter() */

/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_get_rpt_buf_else_err_fatal

DESCRIPTION
  Allocate a MMoC task report buffer. Error fatal if allocation is NOT
  successful.

DEPENDENCIES
  none

RETURN VALUE
  Pointer to allocated MMoC report buffer.

SIDE EFFECTS
  none
===========================================================================*/
mmoc_rpt_msg_s_type            *mmoc_get_rpt_buf_else_err_fatal
(

  void

)
{

  mmoc_rpt_msg_s_type *mmoc_rpt_ptr = NULL;

  if((mmoc_rpt_ptr = mmoc_mem_alloc(sizeof(mmoc_rpt_msg_s_type))) == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return NULL;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* If we got here, allocation is successful
  */
  mmoc_rpt_ptr->rpt.hdr.done_q_ptr      = NULL;
  mmoc_rpt_ptr->rpt.hdr.task_ptr        = NULL;


  return mmoc_rpt_ptr;

} /* mmoc_get_rpt_buf_else_err_fatal() */



/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_send_rpt

DESCRIPTION
  This function takes an already filled out mmoc_rpt_msg_type and places it
  on the mmoc report queue.

DEPENDENCIES
  The MMoC task must have been started up already.  All of the items in
  the mmoc_rpt_msg_s_type must be already set.

RETURN VALUE
  None.

SIDE EFFECTS
  The mmoc_rpt_msg_s_type is placed on the MMoC report queue.  It must not be
  modified until the command has been processed.
===========================================================================*/
void                            mmoc_send_rpt
(

  mmoc_rpt_msg_s_type             *msg_ptr
  /* Pointer to MMoC report message buffer.
  */

)
{
  /* Pointer to MMoC's task info.
  */
  mmoc_task_info_s_type  *mmoc_task_ptr = mmoc_get_task_info_ptr();
#ifdef MMODE_ADDITIONAL_DEBUG_INFO
  rex_tcb_type  *temp = NULL;
  char rtask_name[MMOC_MAX_TASK_NAME_LEN];

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the task name of the protocol that sent the report
  */

  temp = rex_self();
  if(temp != NULL)
  {
#ifdef FEATURE_REX_OPAQUE_TCB_APIS
    /*lint -esym(119,rex_get_task_name) complains of false prototype mismatch */
    (void)rex_get_task_name(temp, rtask_name,
                            sizeof(rtask_name), NULL);
#else
    (void)strlcpy(rtask_name, temp->task_name, ARR_SIZE(rtask_name) );
    rtask_name[(ARR_SIZE(rtask_name))-1] = '\0';
#endif

    /* Map the task name received from rex to mmoc_rpt_task_name_e_type
    ** and store it in task_name field of rpt_ptr
    */
    msg_ptr->rpt.task_name = mmoc_dbg_buf_map_rex_name_to_mmoc(rtask_name);
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /** Also, set is_reused flag for every report to FALSE by default
  */
  msg_ptr->rpt.is_reused = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Initialize and link the command onto the MC report queue.
  */
  (void) q_link(msg_ptr, &msg_ptr->rpt.hdr.link);
  q_put(&mmoc_task_ptr->rpt_q, &msg_ptr->rpt.hdr.link);


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Set the REX signal corresponding to MMoC's report queue and return.
  */
  (void) rex_set_sigs(mmoc_tcb_ptr, MMOC_RPT_Q_SIG);


} /* mmoc_send_rpt() */

/*===========================================================================

FUNCTION get_mmoc_tcb

DESCRIPTION
  Gets pointer MMOC TCB.

DEPENDENCIES
  set_mmoc_tcb.

RETURN VALUE
  Pointer to MMOC TCB.

SIDE EFFECTS
  None.
===========================================================================*/
rex_tcb_type *get_mmoc_tcb(void)
{
  return mmoc_tcb_ptr;
}

/*===========================================================================

FUNCTION get_mmoc_tcb

DESCRIPTION
  Sets pointer to MMOC TCB.

DEPENDENCIES
  Should be called after CM Task is started by RC Init.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/

static void set_mmoc_tcb(void)
{
  mmoc_tcb_ptr = rex_self();
}

/*===========================================================================

FUNCTION mmoc_init_before_task_start

DESCRIPTION
  This function initializes MMOC global data before MMOC task is created.
  This function is called by RCinit framework if it is enabled. Otherwise
  this will be called from MMOC task.

DEPENDENCIES
  This should be called only before task start.

===========================================================================*/
void mmoc_init_before_task_start( void )
{
  mmoc_task_info_s_type          *mmoc_task_ptr = mmoc_get_task_info_ptr();
  mmoc_state_info_s_type         *mmoc_info_ptr = &mmoc_state_info;

  /* Initialize report queues. */
  ( void ) q_init( &mmoc_task_ptr->rpt_q );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize command queue.
  */
  (void) q_init( &mmoc_task_ptr->cmd_q );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*
  ** Initialize MMOC Heap.
  */
  mmoc_modem_heap_blocks = 0;

  /* Initialize err_fatal debug info.
  */
#ifdef FEATURE_MMOC_UNIT_TEST
  #error code not present
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialization for mmoc_state_info.
  */
  mmoc_init_state_dev_info( mmoc_info_ptr );
#ifdef MMODE_ADDITIONAL_DEBUG_INFO
  /* Initialization for  mmoc_debug_buffer values.
  */
  mmoc_dbg_buf_init();
#endif

#ifdef FEATURE_MMOC_UNIT_TEST
  #error code not present
#endif

}


/*===========================================================================

FUNCTION mmoc_task_init

DESCRIPTION
  This function is responsible for initializing the MMoC task before task
  start. This includes
  1. Initializing command and report queues
  2. Performing system determination initializing before task start.

DEPENDENCIES
  This should be called only after task start.

===========================================================================*/
void                           mmoc_task_init ( void )
{
  /* Pointer to MMoC's task information.
  */
  static boolean is_called = FALSE;
  mmoc_task_info_s_type          *mmoc_task_ptr = mmoc_get_task_info_ptr();
  mmoc_state_info_s_type         *mmoc_info_ptr = &mmoc_state_info;
  uint8 sub = 0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for(sub = 0; sub < MAX_SIMS; sub++)
  {
    if(is_called)
    {
      if(mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub] != NULL)
      {
        mmoc_mem_free(mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]);
      }
      if(mmoc_info_ptr->sub_prop[sub] != NULL)
      {
        mmoc_mem_free(mmoc_info_ptr->sub_prop[sub]);
      }
      if(mmoc_task_ptr->timer[sub] != NULL)
      {
        mmoc_mem_free(mmoc_task_ptr->timer[sub]);
      }
    }
    mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub] = NULL;
    mmoc_info_ptr->sub_prop[sub] = NULL;
    mmoc_task_ptr->timer[sub] = NULL;
  }

  is_called = TRUE;

  set_mmoc_tcb();

#ifndef MMOC_DEBUG
  /* Register with DOG HB APIs */
  mmoc_dog_rpt_var = dog_hb_register_rex((rex_sigs_type) MMOC_RPT_TIMER_SIG);
#endif

  rex_def_timer( &mmoc_task_ptr->rex_wait_timer,
                 get_mmoc_tcb(),
                 MMOC_REX_WAIT_SIG
               );

  rex_def_timer( &mmoc_info_ptr->dev_prop.sanity_timer,
                 get_mmoc_tcb(),
                 MMOC_SANITY_TIMER_SIG
               );

  timer_def2(&mmoc_info_ptr->dev_prop.hold_timer, &mmoc_timer_group );
} /* mmoc_task_init() */


/* <EJECT> */
/*===========================================================================
FUNCTION mmoc_kick_dog

DESCRIPTION
  Kick the watchdog and set the watchdog timer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
 None.
===========================================================================*/
static void                            mmoc_kick_dog
(

  void

)
{

#ifndef MMOC_DEBUG
  /* Kick the Watchdog */
  dog_hb_report(DOG_MMOC_RPT);
#endif

} /* mmoc_kick_dog() */


/*===========================================================================

FUNCTION mmoc_timer_done_cb

DESCRIPTION
  This function is the callback procedure used by system determination when
  system selection SS_MAIN timer expires.
  This function will queue the reselection timer expired generic command to
  the MMoC command queue.

===========================================================================*/
#ifdef FEATURE_NON_DEMAND_PAGED_FUNCTION
#error code not present
#endif
void                            mmoc_timer_done_cb
(
  int32                           time_ms,

  timer_cb_data_type              data
)
{
  mmoc_cmd_msg_s_type                 *msg_ptr;
  mm_sub_stk_id_s_type mm_id;
  uint8 raw_data[2];

  memscpy(&raw_data[0], sizeof(raw_data), &data, sizeof(data));
  mm_id.asubs_id = raw_data[0];
  mm_id.stk_id = raw_data[1];

  MMOC_MSG_HIGH_2("ss-timer: sub %d stk %d system selection timer expired",
                      mm_id.asubs_id,
                      mm_id.stk_id);


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                    = MMOC_CMD_PROT_GEN_CMD;
  msg_ptr->param.gen_prot_cmd.cmd_type = PROT_GEN_CMD_SS_TIMER_EXP;
  msg_ptr->param.gen_prot_cmd.asubs_id = mm_id.asubs_id;
  msg_ptr->param.gen_prot_cmd.stk_id   = mm_id.stk_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

} /* mmoc_timer_done_cb() */


#ifdef FEATURE_NON_DEMAND_PAGED_FUNCTION
#error code not present
#endif
/*===========================================================================

FUNCTION mmoc_timer_cb

DESCRIPTION
  This callback function enables or disables system selection SS_MAIN timer.
  It is used by system determination.

===========================================================================*/
void                            mmoc_timer_cb
(
  int4                            ss_timer_duration,
  /* Duration in ms for the timer to be active.
  */

  mm_sub_stk_id_s_type            mm_id
)
{
  /* Pointer to MMoC's task information.
  */
  mmoc_task_info_s_type          *mmoc_task_ptr = mmoc_get_task_info_ptr();
  timer_cb_data_type data;
  uint8 raw_data[2];

  raw_data[0] = mm_id.asubs_id;
  raw_data[1] = mm_id.stk_id;
  memset(&data, 0, sizeof(data));
  memscpy(&data, sizeof(data), &raw_data[0], sizeof(raw_data));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Determine whether to cancel the timer or start it.
  */
  if (ss_timer_duration == 0)
  {
    /* Disable System Selection timer.
    */
    (void) timer_clr(&mmoc_task_ptr->timer[mm_id.asubs_id][mm_id.stk_id], T_NONE);
  }
  else
  {
    /* Start System Selection timer.
    */
    timer_reg(&mmoc_task_ptr->timer[mm_id.asubs_id][mm_id.stk_id],
              (timer_t2_cb_type)mmoc_timer_done_cb,
              (timer_cb_data_type) data,
              (timetick_type)ss_timer_duration,
              0
             );
  }

} /* mmoc__timer_cb() */
#ifdef FEATURE_NON_DEMAND_PAGED_FUNCTION
#error code not present
#endif


/*===========================================================================

FUNCTION mmoc_wait

DESCRIPTION
  This procedure waits for specified signals to be set.  While it waits,
  this procedure continues to set the Watchdog report signal and reset
  the mmoc_task_ptr->rpt_timer.

DEPENDENCIES
  mmoc_task_info->rpt_timer must have been defined with rex_def_timer().

RETURN VALUE
  Signal mask from signals that were set after rex_wait call.

===========================================================================*/
static rex_sigs_type                   mmoc_wait
(
  rex_sigs_type                   mask
  /* Mask of signals to wait on.
  */
)
{

  /* Signals returned by rex_wait.
  */
  rex_sigs_type    sigs;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Loop while waiting for one of the specified signals
  ** to be set.
  */
  for( ;; )
  {
    /* Wait on specified signals, as well as,
    ** on the watchdog signal.
    */
    sigs = rex_wait( mask | MMOC_RPT_TIMER_SIG );

    #ifdef MMOC_DEBUG
    #error code not present
#endif

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* If watchdog signal is set, report back to the watchdog.
    */
    if( sigs & MMOC_RPT_TIMER_SIG )
    {
      (void)rex_clr_sigs( rex_self(), MMOC_RPT_TIMER_SIG );
      mmoc_kick_dog();
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* If one or more of the specified signals is set
    ** return now.
    */
    if( sigs & mask )
    {
      return sigs;
    }
    
  } /* for( ;;) */
} /* mmoc_wait() */

/*===========================================================================

FUNCTION mmoc_reset_timers

DESCRIPTION
  This is a utility function called to clear timers on all active stacks at
  time of subsc chgd or power down transaction.
  It should be called after active protocols are deactivated to ensure that
  SD does not call into the API at the same time in response to a protocol
  requesting a new SD action.

RETURN VALUE
  None

===========================================================================*/
static void mmoc_reset_timers(

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information. */
)
{
  sys_modem_as_id_e_type        sub;
  multimode_stack_e_type        stk;
  mm_sub_stk_id_s_type          mm_id;
  mmoc_task_info_s_type         *mmoc_task_ptr = mmoc_get_task_info_ptr();

  /* Power down: reset all timers
  ** Subsc chg: reset timers only for changed subs
  */
  for (sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; ++sub)
  {
    mm_id.asubs_id = sub;
    for (stk = MM_STACK_0; stk < mmoc_info_ptr->sub_prop[sub]->nStacks && stk < MAX_STACKS; ++stk)
    {
      if(mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_PWR_DOWN ||
          CHECK_PROTO(mmoc_info_ptr->sub_prop[sub]->prot_subsc_chg, PROT_SUBSC_CHG_ALL))
      {
        MMOC_MSG_HIGH_2("Resetting ss timer sub %d stk %d", sub, stk);

        mm_id.stk_id = stk;
        mmoc_timer_cb(0, mm_id);
      }
    }
  }
}

/*===========================================================================

FUNCTION mmoc_process_subsc_chgd_deact_enter

DESCRIPTION
  This is a function for handling the transaction state DEACT_ENTER for the
  subscription changed transaction. If a protocol needs to be deactivated,
  it sets the next trans_state to DEACTD_CNF else sets the next trans_state
  PH_STAT_ENTER/MMGSDI_READ_ENTER. Unlike mmoc_process_common_deact_enter
  it need not deactivate all active protocols

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Should wait for deactivated confirmation.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start ph_stat_enter processing.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_subsc_chgd_deact_enter
(
  prot_deact_e_type              deact_reason,
  /* Reason for deactivating the active protocol. */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information. */

)
{
  boolean is_deact_req_sent = FALSE;
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stack = MM_STACK_NONE;
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();
  
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Deactivate 1x stack if it is in 1x CSFB mode */

#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)) && \
       defined(FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
  if(cmsds_is_1xcsfb_activated(mmoc_dev_info->prev_sub_with_3gpp2) &&
      CHECK_PROTO(mmoc_sub_info[mmoc_dev_info->prev_sub_with_3gpp2]->prot_subsc_chg, PROT_SUBSC_CHG_ALL))
  {
    cmsds_deactivate_1xcsfb_protocol(mmoc_next_trans_id(mmoc_info_ptr),
                                     deact_reason,
                                     mmoc_dev_info->prev_sub_with_3gpp2);

    /* Protocol deactivate request sent, wait for confirmation. */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DEACTD_CNF,
                             mmoc_info_ptr
                           );
    return MMOC_EVT_STATUS_CONSUMED;
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Deactivate all stacks with a subscription change */

  for(sub = (mmoc_info_ptr->nSubs - 1); sub > SYS_MODEM_AS_ID_NONE; sub--)
  {
    uint32 subs_capability_diff;
    uint32 mode_pref_diff;
    if(sub >= MAX_SIMS || mmoc_dev_info->subsc_chgd.sub_prop[sub] ==  NULL)
    {
      continue;
    }
    subs_capability_diff = (mmoc_dev_info->subsc_chgd.sub_prop[sub]->subs_capability ^ mmoc_sub_info[sub]->subs_capability );
    mode_pref_diff = (mmoc_dev_info->subsc_chgd.sub_prop[sub]->mode_pref^mmoc_sub_info[sub]->mode_pref);
    if(!is_deact_req_sent &&
        sub < MAX_SIMS &&
        CHECK_PROTO(mmoc_sub_info[sub]->prot_subsc_chg, PROT_SUBSC_CHG_ALL))
    {
#if defined(MMOC_HDR_SUPPORTED) && !defined(FEATURE_MMOC_UNIT_TEST)
      if(sub == mmoc_dev_info->prev_sub_with_3gpp2)
      {
        mmoc_send_hdr_unlock_rf();
      }
#endif

      for(stack = (mmoc_sub_info[sub]->nStacks - 1); stack > MM_STACK_NONE; stack--)
      {
        if(!is_deact_req_sent)
        {

          mmoc_dev_info->deact_req_idx.asubs_id = sub;
          mmoc_dev_info->deact_req_idx.stk_id = stack;

          /* When the sub has no change in mode pref and only 3GPP2 presence in the
          ** max cap changes and mode_pref/subs didn't have 3GPP2, and the sub has not already been removed,
          ** send deact reason as dual switch so that UE will not perform OTA detach on that sub.
          */
          if((mmoc_dev_info->active_subs & BM(sub)) &&
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->only_subs_cap_change == TRUE &&
             ((subs_capability_diff | SD_SS_MODE_PREF_CDMA_HDR) == SD_SS_MODE_PREF_CDMA_HDR) &&
             mmgsdi_info_ptr->sim_available_status[sub] == TRUE)
          {
            /* When there is no change in mode cap send stop mode with DUAL switch so that there is no OTA detach */
            if(mode_pref_diff == SD_SS_MODE_PREF_NONE)
            {
              is_deact_req_sent = mmoc_deactivate_active_protocol(
                                    mmoc_dev_info->deact_req_idx,
                                    PROT_DEACT_DUAL_SWITCH,
                                    mmoc_info_ptr);
            }
            /* If there is change in mode_cap and TAU CAP UPDATE NV is set, detach is not needed , hence mark the TAU pending flag to TRUE */
            else if(cm_get_tau_cap_update_enabled(sub))
            {
               is_deact_req_sent = mmoc_deactivate_active_protocol(
                                    mmoc_dev_info->deact_req_idx,
                                    PROT_DEACT_DUAL_SWITCH,
                                    mmoc_info_ptr);
               if(is_deact_req_sent == TRUE
                  && mmoc_sub_info[sub]->stack_prop[stack]->prot_state == PROT_STATE_ONLINE_GWL
                 )
               {
                 cmmsc_set_force_reg_pending(sub, TRUE);
               }
            }
            /*If there is change in mode_cap and TAU CAP UPDATE NV is FALSE do a OTA detach */
            else
            {
                is_deact_req_sent = mmoc_deactivate_active_protocol(
                                      mmoc_dev_info->deact_req_idx,
                                      deact_reason,
                                      mmoc_info_ptr);
            }
          }
          else
          {
            is_deact_req_sent = mmoc_deactivate_active_protocol(
                                  mmoc_dev_info->deact_req_idx,
                                  deact_reason,
                                  mmoc_info_ptr);
          }
        }
      }

#if defined MMOC_GWL_SUPPORTED
      for(stack = (mmoc_sub_info[sub]->nStacks - 1); stack > MM_STACK_NONE; stack--)
      {
        if(!is_deact_req_sent && stack != MM_STACK_1 && (mmoc_sub_info[sub]->prev_active_stacks & BM(stack)))
        {
          mmoc_dev_info->deact_req_idx.asubs_id = sub;
          mmoc_dev_info->deact_req_idx.stk_id = stack;
          if((mmoc_dev_info->active_subs & BM(sub)) &&
              mmoc_dev_info->subsc_chgd.sub_prop[sub]->only_subs_cap_change == TRUE &&
             ((subs_capability_diff | SD_SS_MODE_PREF_CDMA_HDR) == SD_SS_MODE_PREF_CDMA_HDR) &&
             mmgsdi_info_ptr->sim_available_status[sub] == TRUE)
          {
            is_deact_req_sent = mmoc_deactivate_gwl_protocol(
                                    mmoc_dev_info->deact_req_idx,
                                    PROT_DEACT_DUAL_SWITCH,
                                    mmoc_info_ptr);
          }
          else
          {
            is_deact_req_sent = mmoc_deactivate_gwl_protocol(
                                  mmoc_dev_info->deact_req_idx,
                                  deact_reason,
                                  mmoc_info_ptr);
          }

          if(is_deact_req_sent)
          {
            mmoc_update_trans_state(MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL,
                                    mmoc_info_ptr
                                   );

            return MMOC_EVT_STATUS_CONSUMED;
          }
        }
      }
#endif
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If DS pref has also changed, then pause/deactivate other stack
  ** Deactivate other stack for SS -> SS case
  ** If DS --> SS, then send on new single_stby_ss
  ** If SS --> DS, then send on prev single_stby_ss
  ** If SS --> SS, then call with new single_stby_ss (no-op)
  */

  if(!is_deact_req_sent &&
      mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM) &&
      mmoc_dual_standby_pref_chgd(mmoc_info_ptr)
#if defined(FEATURE_MMODE_DUAL_ACTIVE) && !defined(FEATURE_MMODE_DSDA_DSDS_SWITCH)
      && (cm_get_device_mode() != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE ||
          mmoc_is_any_sxlte())
#endif
    )
  {
    is_deact_req_sent = mmoc_handle_deact_for_dual_switch(mmoc_info_ptr);

  }

  if(mmoc_info_ptr->dev_prop.trans_state == MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL)
  {
    return MMOC_EVT_STATUS_CONSUMED;
  }

  if (is_deact_req_sent)
  {
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DEACTD_CNF,
                             mmoc_info_ptr
                           );

    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* No active protocols present, start sending the phone status
  ** changed command.
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_PH_STAT_ENTER,
                           mmoc_info_ptr
                         );


  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_subsc_chgd_deact_enter() */



/*===========================================================================

FUNCTION mmoc_process_common_deact_enter

DESCRIPTION
  This is a common function for handling in the transaction state DEACT_ENTER.
  If protocol needs to be deactivated, it sets the next trans_state to
  DEACTD_CNF else sets the next trans_state PH_STAT_ENTER.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Should wait for deactivated confirmation.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start ph_stat_enter processing.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_common_deact_enter
(
  prot_deact_e_type              deact_reason,
  /* Reason for deactivating the active protocol.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type  asubs_id = mmoc_dev_info->sub_with_3gpp2;
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stack = MM_STACK_NONE;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /* Deactivate 1x stack if it is in 1x CSFB mode */
#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)) && \
       defined(FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
  if (cmsds_is_1xcsfb_activated(asubs_id))
  {
    cmsds_deactivate_1xcsfb_protocol (mmoc_next_trans_id(mmoc_info_ptr),
                                      deact_reason,
                                      asubs_id);
    /* Protocol deactivate request sent, wait for confirmation. */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DEACTD_CNF,
                             mmoc_info_ptr
                           );
    return MMOC_EVT_STATUS_CONSUMED;
  }
#endif

  /* Send the deactivate request to active protocol.
  */
  if(mmoc_deactivate_all_active_protocol(deact_reason,
                                         mmoc_info_ptr
                                        )
    )
  {
    /* Protocol deactivate request sent, wait for confirmation.
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DEACTD_CNF,
                             mmoc_info_ptr
                           );
    return MMOC_EVT_STATUS_CONSUMED;
  }

#if defined MMOC_GWL_SUPPORTED

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(mmoc_dev_info->active_subs & BM(sub))
    {
      for(stack = MM_STACK_0; stack < mmoc_sub_info[sub]->nStacks && stack < MAX_STACKS; stack++)
      {
        /* Do not send deactivate request for Stack 2 of the non 1x sub */
        if(!(stack == MM_STACK_2 && sub != mmoc_dev_info->prev_sub_with_3gpp2)
           && (stack != MM_STACK_1 && (mmoc_sub_info[sub]->prev_active_stacks & BM(stack))))
        {
          mm_sub_stk_id_s_type mm_id;
          mm_id.asubs_id = sub;
          mm_id.stk_id = stack;
          if(mmoc_deactivate_gwl_protocol(mm_id,
                                          deact_reason,
                                          mmoc_info_ptr
                                         )
            )
          {
            /* Protocol deactivate request sent, wait for confirmation.
            */
            mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL,
                                     mmoc_info_ptr
                                   );

            return MMOC_EVT_STATUS_CONSUMED;
          }
        }
      }
    }
  }

#endif //MMOC_GWL_SUPPORTED

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* No active protocols present, start sending the phone status
  ** changed command.
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_PH_STAT_ENTER,
                           mmoc_info_ptr
                         );


  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_common_deact_enter() */

/*===========================================================================

FUNCTION mmoc_buf_auto_deact_ind_rpt

DESCRIPTION
 This function will buffer the auto_deact_ind report.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.

===========================================================================*/
mmoc_evt_status_e_type  mmoc_buf_auto_deact_ind_rpt
(

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr,
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

  mm_sub_stk_id_s_type            mm_id

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  if (mmoc_dev_info->sub_with_3gpp2 < MAX_SIMS &&
     mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2] != NULL &&
     mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_1] != NULL
  )
  {
    MMOC_MSG_HIGH_5("mmoc_buf_auto_deact_ind_rpt: sub %d stk %d prot_state %d sub_with_3gpp2 %d HDR prot_state %d",
                  mm_id.asubs_id, mm_id.stk_id,
                  rpt_ptr->param.prot_deactd_ind.prot_state,
                  mmoc_dev_info->sub_with_3gpp2,
                  mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_1]->prot_state);
  }

  /* set the mm_id for HDR based on the stack on which HDR is active, if auto deact is for HDR
  */
  if( rpt_ptr->param.prot_deactd_ind.prot_state  == PROT_STATE_ONLINE_HDR &&
      mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_1]->prot_state == PROT_STATE_ONLINE_HDR)
  {
    mm_id.asubs_id = mmoc_dev_info->sub_with_3gpp2;
    mm_id.stk_id   = MM_STACK_1;
  }
  else if(mm_id.asubs_id == SYS_MODEM_AS_ID_NO_CHANGE ||
          mm_id.stk_id == MM_STACK_ALL)
  {
    mm_id.asubs_id = SYS_MODEM_AS_ID_1;
    mm_id.stk_id   = MM_STACK_0;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Process LTE to DO redirection abort.
  ** If we have any redirection transaction for LTE to DO redir and we
  ** receive auto_deact from HDR with reason REDIR_ABORT, handle this
  ** auto_deact now and set is_redir_allowed->FALSE.
  ** It is done to make sure that when MMoC starts handling redir trans,
  ** it should immediately clear the trans as HDR has already aborted redir
  */
  if( rpt_ptr->param.prot_deactd_ind.prot_state == PROT_STATE_ONLINE_HDR &&
      rpt_ptr->param.prot_deactd_ind.reason == PROT_DEACT_REDIR_ABORT &&
      mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_1]->prot_state !=
      PROT_STATE_ONLINE_HDR)
  {
    MMOC_MSG_HIGH_0("mmoc_buf_auto_deact_ind_rpt: DO Redir aborted");

    mmoc_dev_info->is_redir_allowed = FALSE;

#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
    sd_ss_hdr_irat_fail_ind(mmoc_dev_info->sub_with_3gpp2);
#endif

    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* For the received auto_deact_ind report, check:
  ** 1.If waiting for DEACT_CNF on same stack.
  ** 2.If waiting for DEACT_CNF from GW protocol and a GW protocol is active
       on same stack,
  ** set prot_state to NULL and discard auto_deact.
  ** Else, buffer the auto_deact and set is_reused flag to TRUE so report is not
  ** from rpt queue. */
  if((mmoc_dev_info->trans_state == MMOC_TRANS_STATE_WAIT_DEACTD_CNF ||
      (mmoc_dev_info->trans_state == MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL
       && mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state ==
       PROT_STATE_ONLINE_GWL)) &&
      (mm_id.asubs_id == mmoc_dev_info->deact_req_idx.asubs_id &&
       mm_id.stk_id == mmoc_dev_info->deact_req_idx.stk_id))
  {
    mmoc_update_prot_state(PROT_STATE_NULL, mm_id, mmoc_info_ptr);

    MMOC_MSG_HIGH_2("Updated prot_state for sub %d stk %d to NULL",
                    mm_id.asubs_id,
                    mm_id.stk_id);
  }
  else
  {
    mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->buffered_auto_deact = (mmoc_rpt_msg_s_type *)rpt_ptr;
    mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->buffered_auto_deact->rpt.is_reused = TRUE;
    MMOC_MSG_HIGH_0("Buffered this AUTO DEACT");
  }

  return MMOC_EVT_STATUS_CONSUMED;

}/* mmoc_buf_auto_deact_ind_rpt() */

/*===========================================================================

FUNCTION mmoc_is_auto_deact_ind_rpt_buf

DESCRIPTION
 This function will check if auto_deact_ind is buffered for any stack.
 If yes, it will return stack index, else it will return SD_SS_MAX.

RETURN VALUE
  stack_idx - on which auto deact is buffered
  SD_SS_MAX - else

===========================================================================*/
mm_sub_stk_id_s_type mmoc_is_auto_deact_ind_rpt_buf
(

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stk = MM_STACK_NONE;
  mm_sub_stk_id_s_type mm_id;

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    mm_id.asubs_id = sub;

    for(stk = MM_STACK_0; stk < mmoc_info_ptr->sub_prop[sub]->nStacks; stk++)
    {
      mm_id.stk_id = stk;

      if(mmoc_is_auto_deact_ind_rpt_buf_per_stack(mmoc_info_ptr, mm_id, FALSE))
      {
        MMOC_MSG_HIGH_2("Return sub %d stk %d", mm_id.asubs_id, mm_id.stk_id);
        return mm_id;
      }
    }
  }

  mm_id.asubs_id = SYS_MODEM_AS_ID_NO_CHANGE;
  mm_id.stk_id   = MM_STACK_ALL;

  return mm_id;

}/* mmoc_is_auto_deact_ind_rpt_buf() */

/*===========================================================================

FUNCTION mmoc_is_auto_deact_ind_rpt_buf

DESCRIPTION
 This function will check if auto_deact_ind is buffered for particular ss.
 If yes, it return TRUE, else by default it will return FALSE .
 If is_discard is TRUE and buffered auto deact ind is found, it will
 discard that stored indication and set prot_state to NULL.

RETURN VALUE
  TRUE - if auto_deact report was buffered for ss
  FALSE - else

===========================================================================*/
boolean         mmoc_is_auto_deact_ind_rpt_buf_per_stack
(
  mmoc_state_info_s_type         *mmoc_info_ptr,
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

  mm_sub_stk_id_s_type           mm_id,

  boolean                        is_discard
)
{
  if(mm_id.asubs_id < SYS_MODEM_AS_ID_1 || mm_id.asubs_id >= MAX_SIMS ||
      mm_id.stk_id < MM_STACK_0 || mm_id.stk_id >= MAX_STACKS ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id] == NULL ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id] == NULL)
  {
    return FALSE;
  }
  if(mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->buffered_auto_deact != NULL)
  {
    MMOC_MSG_HIGH_3("AUTO_DEACT buffered for sub %d stk %d discard_flag:%d",mm_id.asubs_id, mm_id.stk_id, is_discard);

    if(is_discard)
    {
#ifdef MMOC_CDMA_SUPPORTED
      /* Do not process AUTO_DEACT_IND from 1x, if 1x protocol is still in active state
            * Ideally this should never happen
            */

      if( mm_id.stk_id == MM_STACK_0 &&
          mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_PROT_GEN_CMD &&
          mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state == PROT_STATE_ONLINE_CDMA &&
          mc_is_cdma_active() == TRUE )
      {
        MMOC_ERR_0 ("Received AUTO_DEACT_IND from 1x but 1x is still active");
        return FALSE;
      }
#endif
      /* to discard the report, free the memory and set the buffer to NULL
      */
      mmoc_mem_free( mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->buffered_auto_deact );
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->buffered_auto_deact = NULL;

      mmoc_update_prot_state(PROT_STATE_NULL,
                             mm_id,
                             mmoc_info_ptr );
    }

    return TRUE;
  }
  return FALSE;

}/* mmoc_is_auto_deact_ind_rpt_buf() */
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
/*===========================================================================

FUNCTION mmoc_process_deactd_cnf_for_lmtd_net_sel_mode

DESCRIPTION
 This function handles the processing  in the WAIT_DEACTD_CNF_GWL
 transaction state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_deactd_cnf_for_lmtd_net_sel_mode
(

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub_id = mmoc_dev_info->deact_req_idx.asubs_id;
  multimode_stack_e_type stk_id = mmoc_dev_info->deact_req_idx.stk_id;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  MMOC_ASSERT( rpt_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the rpt ptr is NULL
  */
  if ( rpt_ptr == NULL )
  {
    /* Not the expected report.
    */
    MMOC_ERR_0("rpt NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_MSG_HIGH_1("deact_cnf_gwl, rpt name %d",rpt_ptr->rpt.name);

  /* Process the received report.
  */
  switch(rpt_ptr->rpt.name)
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      return  mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    case MMOC_RPT_PROT_DEACTD_CNF:
      if(mmoc_dev_info->trans_id != rpt_ptr->param.prot_deactd_cnf.trans_id)
      {
        /* Not the expected report. Go back to waiting for cnf
        */
        MMOC_MSG_HIGH_0(" Unexpected event received for gwl DEACTD_CNF ");
        return MMOC_EVT_STATUS_CONSUMED;
      }

      /* Expected rpt.
      */

      /* We need to go back to PROT_GEN_CMD_ENTER to send the PROT_GEN_CMD
      */

      MMOC_MSG_HIGH_2("deact_cnf sub %d stk %d", sub_id, stk_id);

      if(stk_id == MM_STACK_2)
      {
        (void) sd_ss_ind_hybr_2_user_prot_deactivate(sub_id, NULL, SD_SS_PROT_DEACT_INFO_NONE);
      }
      #ifdef MMOC_HYBR_1_SUPPORTED
      else if(stk_id == MM_STACK_1)
      {
        (void) sd_ss_ind_hdr_user_prot_deactivate(sub_id, NULL, SD_SS_PROT_DEACT_INFO_NONE);
      }
      #endif
      else if(stk_id == MM_STACK_0)
      {
        (void) sd_ss_ind_user_prot_deactivate(sub_id, NULL, SD_SS_PROT_DEACT_INFO_NONE);
      }

      (void) sd_ss_ind_misc_pwr_save_enter(mmoc_dev_info->deact_req_idx, NULL);
     
                     
      /* If we reached here, prot state needs to be updated.
      */
      if((sub_id >= SYS_MODEM_AS_ID_1 && sub_id < MAX_SIMS &&
          stk_id >= MM_STACK_0 && stk_id < MAX_STACKS)
          &&
          (mmoc_sub_info[sub_id]->stack_prop[stk_id]->prot_state == PROT_STATE_ONLINE_GWL ||
           mmoc_sub_info[sub_id]->stack_prop[stk_id]->prot_state == PROT_STATE_PWR_SAVE)
        )
      {
        mmoc_update_prot_state( PROT_STATE_NULL,
                                mmoc_dev_info->deact_req_idx,
                                mmoc_info_ptr
                              );
      }
      mmoc_update_trans_state( MMOC_TRANS_STATE_GEN_CMD_ENTER,
                               mmoc_info_ptr
                             );

      return MMOC_EVT_STATUS_NOT_CONSUMED;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    default:
      MMOC_MSG_HIGH_1( "Unexpected report received gwl %d", rpt_ptr->rpt.name);
      return MMOC_EVT_STATUS_CONSUMED;

  } /* switch ( rpt_ptr->rpt.name ) */

} /* mmoc_process_deactd_cnf_for_lmtd_net_sel_mode() */
#endif

/*===========================================================================

FUNCTION mmoc_process_deactd_cnf_gwl

DESCRIPTION
 This function handles the processing  in the WAIT_DEACTD_CNF_GWL
 transaction state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_deactd_cnf_gwl
(

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub_id = mmoc_dev_info->deact_req_idx.asubs_id;
  multimode_stack_e_type stk_id = mmoc_dev_info->deact_req_idx.stk_id;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  MMOC_ASSERT( rpt_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the rpt ptr is NULL
  */
  if ( rpt_ptr == NULL )
  {
    /* Not the expected report.
    */
    MMOC_ERR_0("rpt NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_MSG_HIGH_1("deact_cnf_gwl, rpt name %d",rpt_ptr->rpt.name);

  /* Process the received report.
  */
  switch(rpt_ptr->rpt.name)
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      return  mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    case MMOC_RPT_PROT_DEACTD_CNF:
      if(mmoc_dev_info->trans_id != rpt_ptr->param.prot_deactd_cnf.trans_id)
      {
        /* Not the expected report. Go back to waiting for cnf
        */
        MMOC_MSG_HIGH_0(" Unexpected event received for gwl DEACTD_CNF ");
        return MMOC_EVT_STATUS_CONSUMED;
      }

      /* Expected rpt.
      */

      /* Multiple protocols have to be deactivated, trans_state has to be
      ** set to DEACT_ENTER.
      */
      mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_DEACT_ENTER,
                               mmoc_info_ptr
                             );

      

      if (!(mmoc_dev_info->active_subs & BM(sub_id)))
      {

        MMOC_MSG_HIGH_2("deact_cnf sub %d stk %d", sub_id, stk_id);

        if(stk_id == MM_STACK_2)
        {
          (void) sd_ss_ind_hybr_2_user_prot_deactivate(sub_id, NULL, SD_SS_PROT_DEACT_INFO_NONE);
        }
        #ifdef MMOC_HYBR_1_SUPPORTED
        else if(stk_id == MM_STACK_1)
        {
          (void) sd_ss_ind_hdr_user_prot_deactivate(sub_id, NULL, SD_SS_PROT_DEACT_INFO_NONE);
        }
        #endif
        else if(stk_id == MM_STACK_0)
        {
          (void) sd_ss_ind_user_prot_deactivate(sub_id, NULL, SD_SS_PROT_DEACT_INFO_NONE);
        }

        (void) sd_ss_ind_misc_pwr_save_enter(mmoc_dev_info->deact_req_idx, NULL);
      }
                     
      /* If we reached here, prot state needs to be updated.
      */
      if((sub_id >= SYS_MODEM_AS_ID_1 && sub_id < MAX_SIMS &&
          stk_id >= MM_STACK_0 && stk_id < MAX_STACKS)
          &&
          (mmoc_sub_info[sub_id]->stack_prop[stk_id]->prot_state == PROT_STATE_ONLINE_GWL ||
           mmoc_sub_info[sub_id]->stack_prop[stk_id]->prot_state == PROT_STATE_PWR_SAVE)
        )
      {
        mmoc_update_prot_state( PROT_STATE_NULL,
                                mmoc_dev_info->deact_req_idx,
                                mmoc_info_ptr
                              );
      }

      return MMOC_EVT_STATUS_NOT_CONSUMED;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    default:
      MMOC_MSG_HIGH_1( "Unexpected report received gwl %d", rpt_ptr->rpt.name);
      return MMOC_EVT_STATUS_CONSUMED;

  } /* switch ( rpt_ptr->rpt.name ) */

} /* mmoc_process_deactd_cnf_gwl() */


/*===========================================================================

FUNCTION mmoc_process_common_deactd_cnf

DESCRIPTION
 This function handles the common processing  in the WAIT_DEACTD_CNF
 transaction state.

  In this state, the state handler will
  1. Validate the deactivated confirmation report received
  2. Move the trans. state to send the phone status command.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_common_deactd_cnf
(

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub_id = mmoc_dev_info->deact_req_idx.asubs_id;
  multimode_stack_e_type stk_id = mmoc_dev_info->deact_req_idx.stk_id;
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  MMOC_ASSERT( rpt_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the rpt ptr is NULL */

  if( rpt_ptr == NULL )
  {
    /* Not the expected report.
    */
    MMOC_ERR_0("rpt NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Process the received report. */

  switch(rpt_ptr->rpt.name)
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      return mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - --*/

    case MMOC_RPT_PROT_DEACTD_CNF:
      if(mmoc_dev_info->trans_id != rpt_ptr->param.prot_deactd_cnf.trans_id)
      {
        /* Not the expected report. Go back to waiting for cnf
        */
        MMOC_MSG_HIGH_2("Unexpected event; mmoc_info trans_id: %d, rpt_ptr trans_id: %d",
                        mmoc_dev_info->trans_id,
                        rpt_ptr->param.prot_deactd_cnf.trans_id);
        return MMOC_EVT_STATUS_CONSUMED;
      }

      /* Expected rpt.
      */
      mmoc_sanity_timer_reset(mmoc_info_ptr);

      /* Multiple protocols have to be deactivated, trans_state has to be
      ** set to DEACT_ENTER.
      */
      mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_DEACT_ENTER,
                               mmoc_info_ptr
                             );

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - --*/

      if(sub_id >= SYS_MODEM_AS_ID_1 && sub_id < MAX_SIMS &&
          stk_id >= MM_STACK_0 && stk_id < MAX_STACKS)
      {
        /* SUBSC_CHGD transaction*/

        if(mmoc_dev_info->trans == MMOC_TRANS_SUBSC_CHGD)
        {
          MMOC_MSG_HIGH_4("SUBSC_CHGD: DEACT_CNF: device_mode %d, deact_sub %d, deact_stk %d, sub_avail %d",
                          cm_get_device_mode(), sub_id, stk_id, mmoc_sub_info[sub_id]->sub_avail);

          if((!(mmoc_dev_info->active_subs & BM(sub_id)) ||
              !mmoc_sub_info[sub_id]->sub_avail ||
              cm_get_device_mode() == SYS_MODEM_DEVICE_MODE_SINGLE_SIM ||
              stk_id == MM_STACK_1 ||
               (stk_id == MM_STACK_2 && mmoc_is_sub_mode_sxlte(sub_id))) &&
              (mmoc_info_ptr->dev_prop.is_sd_initialized)
            )
          {
            sd_ss_prot_deact_adtnl_info_e_type deact_info = SD_SS_PROT_DEACT_INFO_NONE;

            MMOC_MSG_HIGH_2("SUBSC_CHGD: prot_deactivate and pwr_save, sub %d stk %d",
                            sub_id, stk_id);

            if(mmoc_dev_info->active_subs & BM(sub_id))
            {
              deact_info = SD_SS_PROT_DEACT_INFO_SUBS_CHANGE_TRANS;
            }

            if(stk_id == MM_STACK_2)
            {
              (void) sd_ss_ind_hybr_2_user_prot_deactivate(sub_id, NULL, deact_info);
            }
#ifdef MMOC_HYBR_1_SUPPORTED
            else if(stk_id == MM_STACK_1)
            {
              (void) sd_ss_ind_hdr_user_prot_deactivate(sub_id, NULL, deact_info);
            }
#endif
            else if(stk_id == MM_STACK_0)
            {
              (void) sd_ss_ind_user_prot_deactivate(sub_id, NULL, deact_info);
            }

            (void) sd_ss_ind_misc_pwr_save_enter(mmoc_dev_info->deact_req_idx, NULL);
          }
        }

        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - --*/

        /* Dual standby chgd transaction */

        if(mmoc_dev_info->trans == MMOC_TRANS_DUAL_STANDBY_CHGD && mmoc_info_ptr->dev_prop.is_sd_initialized)
        {
          if((BM(sub_id) & mmoc_dev_info->prev_active_subs) &&
              !(BM(sub_id) & mmoc_dev_info->active_subs))
          {
            if(stk_id == MM_STACK_2)
            {
              (void) sd_ss_ind_hybr_2_user_prot_deactivate(sub_id, NULL, SD_SS_PROT_DEACT_INFO_NONE);
            }
#ifdef MMOC_HYBR_1_SUPPORTED
            else if(stk_id == MM_STACK_1)
            {
              (void) sd_ss_ind_hdr_user_prot_deactivate(sub_id, NULL, SD_SS_PROT_DEACT_INFO_NONE);
            }
#endif
            else if(stk_id == MM_STACK_0)
            {
              (void) sd_ss_ind_user_prot_deactivate(sub_id, NULL, SD_SS_PROT_DEACT_INFO_NONE);
            }

            (void) sd_ss_ind_misc_pwr_save_enter(mmoc_dev_info->deact_req_idx, NULL);
          }
        }

#ifdef FEATURE_MMODE_DUAL_SIM
        if(mmoc_dev_info->trans == MMOC_TRANS_SUSPEND_STACK &&
            mmoc_dev_info->trans_info.suspend_stack.susp_reason == PROT_DEACT_DSDX_SUSP &&
            mmoc_info_ptr->dev_prop.is_sd_initialized)
        {
          if(stk_id == MM_STACK_2)
          {
            (void) sd_ss_ind_hybr_2_user_prot_deactivate(sub_id, NULL, SD_SS_PROT_DEACT_INFO_NONE);
          }
#ifdef MMOC_HYBR_1_SUPPORTED
          else if(stk_id == MM_STACK_1)
          {
            (void) sd_ss_ind_hdr_user_prot_deactivate(sub_id, NULL, SD_SS_PROT_DEACT_INFO_NONE);
          }
#endif
          else if(stk_id == MM_STACK_0)
          {
            (void) sd_ss_ind_user_prot_deactivate(sub_id, NULL, SD_SS_PROT_DEACT_INFO_NONE);
          }

          (void) sd_ss_ind_misc_pwr_save_enter(mmoc_dev_info->deact_req_idx, NULL);

        }
#endif

        /* If Deact_Cnf is received for a dormant protocol durng redirection
        ** transaction, then update prot_dormant state, and not prot_state
        */
        if(((mmoc_dev_info->trans == MMOC_TRANS_PROT_REDIR_IND &&
             (mmoc_dev_info->trans_info.redir_ind_info.cmd_info.prot_state ==
              PROT_STATE_ONLINE_HDR
              ||
              mmoc_dev_info->trans_info.redir_ind_info.cmd_info.prot_state ==
              PROT_STATE_ONLINE_CDMA))
#if defined(FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
            ||
            (mmoc_dev_info->trans == MMOC_TRANS_PROT_HO_IND &&
             mmoc_dev_info->trans_info.ho_ind_info.cmd_info.prot_state ==
             PROT_STATE_ONLINE_CDMA)
#endif
           )
            &&
            ((int)mmoc_sub_info[sub_id]->stack_prop[stk_id]->prot_dormant &
             (int)SD_SS_MODE_PREF_GWL )
          )
        {

          mmoc_update_prot_dormant_state( SD_SS_MODE_PREF_NONE,
                                          mmoc_dev_info->deact_req_idx,
                                          mmoc_info_ptr
                                        );

#if defined(FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
          if(mmoc_dev_info->trans == MMOC_TRANS_PROT_HO_IND &&
              mmoc_dev_info->trans_info.ho_ind_info.cmd_info.prot_state ==
              PROT_STATE_ONLINE_CDMA)
          {

            /* Send HO_CNF back to protocol
            */
            mc_cdma_handover_cnf(sub_id, mmoc_dev_info->trans_info.ho_ind_info.cmd_info.actd_reason);

          }
#endif /* FEATURE_LTE_TO_1X */

          return MMOC_EVT_STATUS_NOT_CONSUMED;

        }

        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        /* If we reached here, prot state needs to be updated.
        */

        MMOC_MSG_HIGH_4("deact_index: sub %d stk %d, last_prot_state %d, prot_state %d",
                        sub_id, stk_id,
                        mmoc_sub_info[sub_id]->stack_prop[stk_id]->last_prot_state,
                        mmoc_sub_info[sub_id]->stack_prop[stk_id]->prot_state);

        /* Check if the deact cnf is for MAIN stack and the last protocol state of MAIN when deact_req
           was sent and the current protocol state of MAIN is same or not, if not , dont update the
           protocol state of MAIN to NULL
        */
        if(stk_id != MM_STACK_0 ||
            mmoc_sub_info[sub_id]->stack_prop[stk_id]->last_prot_state ==
            mmoc_sub_info[sub_id]->stack_prop[stk_id]->prot_state)
        {
          mm_sub_stk_id_s_type mm_id;

          if(stk_id == MM_STACK_0)
          {
            mmoc_sub_info[sub_id]->stack_prop[stk_id]->last_prot_state = PROT_STATE_NULL;
          }

          mm_id.asubs_id = sub_id;
          mm_id.stk_id = stk_id;
          mmoc_update_prot_state(PROT_STATE_NULL,
                                 mm_id,
                                 mmoc_info_ptr
                                );
        }
      }

      return MMOC_EVT_STATUS_NOT_CONSUMED;

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#if defined(FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
    case MMOC_RPT_1XCSFB_PROT_DEACTD_CNF:
      if(mmoc_dev_info->trans_id != rpt_ptr->param.prot_deactd_cnf.trans_id)
      {
        /* Not the expected report. Go back to waiting for cnf */
        MMOC_ERR_0(" Unexpected event received ");
        return MMOC_EVT_STATUS_CONSUMED;
      }

      mmoc_sanity_timer_reset(mmoc_info_ptr);

      /* Multiple protocols have to be deactivated, trans_state has to be
      ** set to DEACT_ENTER.
      */
      mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_DEACT_ENTER,
                               mmoc_info_ptr
                             );
      return MMOC_EVT_STATUS_NOT_CONSUMED;
#endif
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    default:
      MMOC_MSG_HIGH_1( "Unexpected report received for deact_cnf %d", rpt_ptr->rpt.name);
      return MMOC_EVT_STATUS_CONSUMED;

  } /* switch ( rpt_ptr->rpt.name ) */

  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_common_deactd_cnf() */


/*===========================================================================

FUNCTION mmoc_process_common_ds_stat_chgd_cnf

DESCRIPTION
 This function handles the common processing  in the WAIT_DEACTD_CNF
 transaction state.

  In this state, the state handler will
  1. Validate the deactivated confirmation report received
  2. Move the trans. state to send the phone status command.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_common_ds_stat_chgd_cnf
(

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stack = MM_STACK_NONE;
  boolean is_priority_sub_activated = FALSE;
  boolean is_sub_on_hold_sleep = FALSE;
  sd_ss_act_e_type                   sd_act = SD_SS_ACT_CONTINUE;

#ifdef FEATURE_MMODE_DUAL_SIM
  sys_modem_as_id_e_type     sub_on_hold = SYS_MODEM_AS_ID_NONE;
#endif

  MMOC_ASSERT( rpt_ptr != NULL );

  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the rpt ptr is NULL
  */
  if ( rpt_ptr == NULL )
  {
    /* Not the expected report.
    */
    MMOC_ERR_0("rpt NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Process the received report.
  */
  switch(rpt_ptr->rpt.name)
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      return  mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    case MMOC_RPT_DS_STAT_CHGD_CNF:

      if(mmoc_dev_info->trans_id != rpt_ptr->param.ds_stat_chgd_cnf.trans_id)
      {
        /* Not the expected report. Go back to waiting for cnf
        */
        MMOC_ERR_0(" Unexpected event received ");
        return MMOC_EVT_STATUS_CONSUMED;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Dual standby chgd transaction */

      if(mmoc_dev_info->trans == MMOC_TRANS_DUAL_STANDBY_CHGD)
      {
        if(mmoc_dev_info->oprt_mode == PROT_OPRT_MODE_ONLINE)
        {

#ifdef FEATURE_MMODE_DUAL_SIM
          sub_on_hold = mmoc_calculate_sub_on_hold(mmoc_info_ptr);
          if(sub_on_hold != SYS_MODEM_AS_ID_NONE)
          {
            for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
            {
              if(sub_on_hold != sub &&
                  mmoc_dev_info->active_subs & BM(sub) &&
                  (mmoc_dev_info->dual_switch_subs & BM(sub) ||
                   !(mmoc_dev_info->prev_active_subs & BM(sub))))
              {
                sd_act = sd_ss_ind_user_pwr_up( TRUE, sub, NULL );
                mmoc_activate_prot_online_ss_main(sd_act, sub, mmoc_info_ptr);
                is_priority_sub_activated = TRUE;
              }
              else if(sub_on_hold == sub &&
                      cm_get_device_mode() != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE &&
                      mmoc_dev_info->prev_standby_pref == SYS_MODEM_DS_PREF_SINGLE_STANDBY &&
                      (mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY
#ifdef FEATURE_MMODE_TRIPLE_SIM
                       || mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY
#endif
                       ) && is_priority_sub_activated)
              {
                boolean delay_srv_req = FALSE;
                
                is_sub_on_hold_sleep = TRUE;
                if(sd_act == SD_SS_ACT_ACQ_CDMA
                    || sd_act == SD_SS_ACT_ACQ_GWL
                    || sd_act == SD_SS_ACT_ACQ_HDR)
                {
                  mmoc_hold_acq(sub_on_hold, mmoc_info_ptr);

                  /* need to delay srv req for non dds sub (sub_on_hold) for srlte+x config */
                  delay_srv_req = mmoc_delay_srv_req(sub_on_hold, mmoc_info_ptr);
                }

                sd_act = sd_ss_ind_user_pwr_up( TRUE, sub, NULL );
                mmoc_activate_prot_online_ss_main2(sd_act, sub_on_hold, mmoc_info_ptr,
                                                     delay_srv_req);
              }
            }

            if(!is_sub_on_hold_sleep)
            {
              boolean delay_srv_req = FALSE;
              
              is_sub_on_hold_sleep = TRUE;

              if(sd_act == SD_SS_ACT_ACQ_CDMA
                  || sd_act == SD_SS_ACT_ACQ_GWL
                  || sd_act == SD_SS_ACT_ACQ_HDR)
              {
                mmoc_hold_acq(sub_on_hold, mmoc_info_ptr);

                /* need to delay srv req for non dds sub (sub_on_hold) for srlte+x config */
                delay_srv_req = mmoc_delay_srv_req(sub_on_hold, mmoc_info_ptr);
              }

              sd_act = sd_ss_ind_user_pwr_up( TRUE, sub_on_hold, NULL );
              mmoc_activate_prot_online_ss_main2(sd_act, sub_on_hold, mmoc_info_ptr, 
                                                   delay_srv_req);
            }
          }
          else
          {
            for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
            {
              if( (mmoc_dev_info->active_subs & BM(sub)) &&
                (mmoc_dev_info->dual_switch_subs & BM(sub) ||
               !(mmoc_dev_info->prev_active_subs & BM(sub))))
              {
                sd_act = sd_ss_ind_user_pwr_up(FALSE, sub, NULL);
                mmoc_activate_prot_online_ss_main(sd_act, sub, mmoc_info_ptr);
              }
            }
          }

#else //SSIM

          if(mmoc_dev_info->active_subs & BM(SYS_MODEM_AS_ID_1) ||
              !(mmoc_dev_info->prev_active_subs & BM(SYS_MODEM_AS_ID_1)))
          {
            sd_act = sd_ss_ind_user_pwr_up( TRUE, SYS_MODEM_AS_ID_1, NULL );
            mmoc_activate_prot_online_ss_main(sd_act, SYS_MODEM_AS_ID_1, mmoc_info_ptr);
          }
#endif

        }
        else
        {
          MMOC_MSG_HIGH_1("Device not online, not activating any stacks, oprt_mode:%d",\
                          mmoc_dev_info->oprt_mode );
        }

        {
          cm_mcfg_proceed_with_refresh_type    mcfg_refresh_proceed;
          mcfg_refresh_proceed.active_subs = mmoc_dev_info->active_subs;
          /* Send the CM_STANDY_PREF_CHG_OK_F report to CM.
          */
          MMOC_MSG_HIGH_1("MCFG_Refresh: sending from cmn ds chgd cnf active_subs %d",mcfg_refresh_proceed.active_subs);
          mmoc_send_cm_rpt( CM_MCFG_PROCEED_WITH_REFRESH_F, (cm_sd_rpt_u_type *)&mcfg_refresh_proceed, SYS_MODEM_AS_ID_1);

        }
        /* Clear the transaction.
        */
        return mmoc_clear_transaction( NULL, mmoc_info_ptr );
      }
      else
      {

        /* Set prev_ds_pref with new ds_pref value */
        mmoc_dev_info->prev_standby_pref = mmoc_dev_info->standby_pref;
        mmoc_dev_info->prev_active_subs  = mmoc_dev_info->active_subs;

        /* Move back to DS STAT CHGD ENTER state
        */
        mmoc_update_trans_state( MMOC_TRANS_STATE_DS_STAT_CHGD_ENTER,
                                 mmoc_info_ptr
                               );


        return MMOC_EVT_STATUS_NOT_CONSUMED;
      }


    default:
      MMOC_MSG_HIGH_1( "Unexpected report received for ds_stat_chgd %d",
                       rpt_ptr->rpt.name);
      return MMOC_EVT_STATUS_CONSUMED;


  } /* switch ( rpt_ptr->rpt.name ) */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

}


/*===========================================================================

FUNCTION mmoc_process_common_ps_detach_cnf

DESCRIPTION
 This function handles the common processing  in the PS DETACH CNF
 transaction state.

  In this state, the state handler will
  1. Validate the detach confirmation report received
  2. Move the trans. state to send the phone status command.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_common_ps_detach_cnf
(

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  MMOC_ASSERT( rpt_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the rpt ptr is NULL
  */
  if ( rpt_ptr == NULL )
  {
    /* Not the expected report.
    */
    MMOC_ERR_0("rpt NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Process the received report.
  */
  switch(rpt_ptr->rpt.name)
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      return  mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    case MMOC_RPT_PS_DETACH_CNF:

      if(mmoc_info_ptr->dev_prop.trans_id != rpt_ptr->param.ps_detach_cnf.trans_id)
      {
        /* Not the expected report. Go back to waiting for cnf
        */
        MMOC_ERR_2("Unexpected event received. rpt trans_id %d trans_id %d",
                   rpt_ptr->param.ps_detach_cnf.trans_id,
                   mmoc_info_ptr->dev_prop.trans_id);

        return MMOC_EVT_STATUS_CONSUMED;
      }

      /* Move back to GEN CMD enter state
      */
      mmoc_update_trans_state( MMOC_TRANS_STATE_GEN_CMD_ENTER,
                               mmoc_info_ptr
                             );


      return MMOC_EVT_STATUS_NOT_CONSUMED;


    default:
      MMOC_ERR_1( "Unexpected report received %d",
                  rpt_ptr->rpt.name);
      return MMOC_EVT_STATUS_CONSUMED;


  } /* switch ( rpt_ptr->rpt.name ) */

}// mmoc_process_common_ps_detach_cnf

/*===========================================================================

FUNCTION mmoc_process_ue_mode_switch_cnf_1X

DESCRIPTION
 This function handles the ue mode swicth confirmation from 1X.
 transaction state.

  In this state, the state handler will
  1. Validate the ue mode change confirmation report received
  2. Move the trans. state to ue_mode_change reqeust to HDR

DEPENDENCIES
  None.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

SIDE EFFECTS
  None.
===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_ue_mode_switch_cnf_1x
(

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  MMOC_ASSERT( rpt_ptr != NULL );


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Check if the rpt ptr is NULL
  */
  if ( rpt_ptr == NULL )
  {
    /* Not the expected report.
    */
    MMOC_ERR_0("rpt NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /* Process the received report.
  */
  switch (rpt_ptr->rpt.name )
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      /* Autonomous deactivated report received.
            */
      return  mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    case MMOC_RPT_UE_MODE_SWITCH_CNF_1X:

      /* Check if it is the right acknowledgement.
             */
      if(mmoc_info_ptr->dev_prop.trans_id != rpt_ptr->param.ue_mode_switch_cnf_1x.trans_id)
      {
        /* Not the expected report. Go back to waiting for cnf
        */
        MMOC_ERR_1(" Received wrong trans_id %d",
                   rpt_ptr->param.ue_mode_switch_cnf_1x.trans_id);

        return MMOC_EVT_STATUS_CONSUMED;
      }
      break;

    default:
      MMOC_MSG_HIGH_1("Wrong report %d", rpt_ptr->rpt.name);

      return MMOC_EVT_STATUS_CONSUMED;
  } /* switch (rpt_ptr->rpt.name ) */


  mmoc_update_trans_state( MMOC_TRANS_STATE_GEN_CMD_ENTER,
                           mmoc_info_ptr
                         );

  return MMOC_EVT_STATUS_NOT_CONSUMED;

}


/*===========================================================================

FUNCTION mmoc_process_ue_mode_switch_cnf_hdr

DESCRIPTION
 This function handles the ue mode swicth confirmation from HDR.
 transaction state.

  In this state, the state handler will
  1. Validate the ue mode change confirmation report received
  2. Move the trans. state to ue_mode_change reqeust to NAS

DEPENDENCIES
  None.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

SIDE EFFECTS
  None.
===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_ue_mode_switch_cnf_hdr
(

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  MMOC_ASSERT( rpt_ptr != NULL );


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Check if the rpt ptr is NULL
  */
  if ( rpt_ptr == NULL )
  {
    /* Not the expected report.
    */
    MMOC_ERR_0("rpt NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /* Process the received report.
  */
  switch (rpt_ptr->rpt.name )
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      /* Autonomous deactivated report received.
            */
      return  mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    case MMOC_RPT_UE_MODE_SWITCH_CNF_HDR:

      /* Check if it is the right acknowledgement.
            */
      if(mmoc_info_ptr->dev_prop.trans_id != rpt_ptr->param.ue_mode_switch_cnf_hdr.trans_id)
      {
        /* Not the expected report. Go back to waiting for cnf
        */
        MMOC_ERR_1(" Received wrong trans_id %d",
                   rpt_ptr->param.ue_mode_switch_cnf_hdr.trans_id);
        return MMOC_EVT_STATUS_CONSUMED;
      }
      break;

    default:
      MMOC_MSG_HIGH_1("Wrong report %d", rpt_ptr->rpt.name);

      return MMOC_EVT_STATUS_CONSUMED;
  } /* switch (rpt_ptr->rpt.name ) */

  mmoc_update_trans_state( MMOC_TRANS_STATE_GEN_CMD_ENTER,
                           mmoc_info_ptr
                         );

  return MMOC_EVT_STATUS_NOT_CONSUMED;

}

/*===========================================================================

FUNCTION mmoc_process_ue_mode_switch_cnf

DESCRIPTION
 This function handles the common processing  in the PS DETACH CNF
 transaction state.

  In this state, the state handler will
  1. Validate the detach confirmation report received
  2. Move the trans. state to send the phone status command.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_ue_mode_switch_cnf
(
  const mmoc_rpt_msg_s_type      *rpt_ptr,
  mmoc_state_info_s_type         *mmoc_info_ptr
)
{
  mm_sub_stk_id_s_type mm_id;

  MMOC_ASSERT( rpt_ptr != NULL );

  if ( rpt_ptr == NULL )
  {
    MMOC_ERR_0("rpt NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  mm_id.asubs_id = rpt_ptr->param.ue_mode_switch_cnf.mm_id.asubs_id;
  mm_id.stk_id   = rpt_ptr->param.ue_mode_switch_cnf.mm_id.stk_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Process the received report.
  */
  switch(rpt_ptr->rpt.name)
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      return mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    case MMOC_RPT_UE_MODE_SWITCH_CNF:

      if(mmoc_info_ptr->dev_prop.trans_id != rpt_ptr->param.ue_mode_switch_cnf.trans_id)
      {
        /* Not the expected report. Go back to waiting for cnf */
        MMOC_ERR_2("Unexpected event received. rpt trans_id %d trans_id %d",
                   rpt_ptr->param.ue_mode_switch_cnf.trans_id,
                   mmoc_info_ptr->dev_prop.trans_id);

        return MMOC_EVT_STATUS_CONSUMED;
      }

      {
        mm_sub_stk_id_s_type deact_mm_id;

        /* Update main stack state too for handling below usecase
        ** UE is in 1xsrlte , and HYBR2 lost service and in PWRSAVE .
        ** main acquired GSM/WCDMA service and in PROT_STATE_ONLINE_GWL state.
        ** in this case as part of handling PROT_CMD_UE_MODE_SWITCH ,
        ** NAS stops main stack and hence we need to update main stack state .
        */
        if(mmoc_is_1x_sxlte(mm_id.asubs_id) &&
            rpt_ptr->param.ue_mode_switch_cnf.is_prot_deactivated)
        {
          if(mm_id.stk_id == MM_STACK_0 &&
              mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[MM_STACK_2]->prot_state == PROT_STATE_ONLINE_GWL)
          {
            deact_mm_id.asubs_id = mm_id.asubs_id;
            deact_mm_id.stk_id = MM_STACK_2;

            mmoc_update_prot_state( PROT_STATE_NULL,
                                    deact_mm_id,
                                    mmoc_info_ptr
                                  );
          }
          else if(mm_id.stk_id == MM_STACK_2 &&
                  mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_ONLINE_GWL)
          {
            deact_mm_id.asubs_id = mm_id.asubs_id;
            deact_mm_id.stk_id = MM_STACK_0;

            mmoc_update_prot_state( PROT_STATE_NULL,
                                    deact_mm_id,
                                    mmoc_info_ptr
                                  );
          }
        }
      }

      /* Move back to GEN CMD enter state
      */
      if(mmoc_info_ptr->dev_prop.oprt_mode == PROT_OPRT_MODE_ONLINE)
      {
        if(mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state != PROT_STATE_ONLINE_CDMA &&
            mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state != PROT_STATE_ONLINE_HDR &&
            (!mmoc_is_1x_sxlte(mm_id.asubs_id) ||
             rpt_ptr->param.ue_mode_switch_cnf.is_prot_deactivated)
          )
        {
          mmoc_update_prot_state( PROT_STATE_NULL,
                                  mm_id,
                                  mmoc_info_ptr
                                );
        }

        mmoc_update_trans_state( MMOC_TRANS_STATE_GEN_CMD_ENTER,
                                 mmoc_info_ptr
                               );
      }
      else
      {
        mmoc_update_trans_state( MMOC_TRANS_STATE_NULL,
                                 mmoc_info_ptr
                               );
        return mmoc_clear_transaction(NULL, mmoc_info_ptr);
      }
      return MMOC_EVT_STATUS_NOT_CONSUMED;


    default:
      MMOC_ERR_1( "Unexpected report received %d",rpt_ptr->rpt.name);
      return MMOC_EVT_STATUS_CONSUMED;


  } /* switch ( rpt_ptr->rpt.name ) */

}// mmoc_process_ue_mode_switch_cnf


/*===========================================================================

FUNCTION mmoc_process_prot_gen_cmd_csfb_deact_cnf

DESCRIPTION
 This function handles the generic command processing of CSFB DEACT CNF
 transaction state.

  In this state, the state handler will
  1. Validate the detach confirmation report received
  2. Move the trans. state to send the phone status command.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_gen_cmd_csfb_deact_cnf
(

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  MMOC_ASSERT( rpt_ptr != NULL );
  MMOC_ASSERT( mmoc_info_ptr != NULL );

  if (rpt_ptr == NULL)
  {
    MMOC_ERR_0("rpt_ptr is NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  if ((rpt_ptr->rpt.name != MMOC_RPT_1XCSFB_PROT_DEACTD_CNF) ||
      (mmoc_info_ptr->dev_prop.trans_id != rpt_ptr->param.cdma_csfb_deactd_cnf.trans_id))
  {
    MMOC_ERR_2( "Unexpected report received %d in state %d",
                rpt_ptr->rpt.name, mmoc_info_ptr->dev_prop.trans_state);
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /* Move back to GEN CMD enter state
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_GEN_CMD_ENTER,
                           mmoc_info_ptr
                         );


  return MMOC_EVT_STATUS_NOT_CONSUMED;

}


/*===========================================================================

FUNCTION mmoc_process_common_hdr_deact_enter

DESCRIPTION
  This is a common function for handling transaction state hdr_deact_enter.
  It sets the next state to WAIT_DEACTD_CNF

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Should wait for deactivated confirmation.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start ph_stat_enter processing.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_common_hdr_deact_enter
(
  prot_deact_e_type              deact_reason,
  /* Reason for deactivating the active protocol.
  */

  mmoc_state_info_s_type        *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;

  /* Can't be const as when HDR features are on, we get
  ** assignment to const object error
  */
  SYS_ARG_NOT_CONST(mmoc_info_ptr);

#ifdef MMOC_HDR_SUPPORTED

  mmoc_dev_info->deact_req_idx.asubs_id = mmoc_dev_info->sub_with_3gpp2;
  mmoc_dev_info->deact_req_idx.stk_id = MM_STACK_1;

  mmoc_send_hdr_deactivate_protocol( deact_reason,
                                     mmoc_next_trans_id(mmoc_info_ptr),
                                     mmoc_dev_info->sub_with_3gpp2);

  /* Deactivation request sent, wait for cnf.
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_HDR_DEACTD_CNF,
                           mmoc_info_ptr
                         );

#endif


  return MMOC_EVT_STATUS_CONSUMED;

} /* mmoc_process_common_hdr_deact_enter () */



/*===========================================================================

FUNCTION mmoc_process_common_hdr_deactd_cnf

DESCRIPTION
  This function handles the hdr deactivation cnf and proceeds to kick the
  MAIN instance to activate any protocol that was waiting on hdr deactf.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_common_hdr_deactd_cnf
(
  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  mm_sub_stk_id_s_type mm_id;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  MMOC_ASSERT(rpt_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (mmoc_process_common_deactd_cnf(rpt_ptr, mmoc_info_ptr) ==
      MMOC_EVT_STATUS_CONSUMED)
  {
    /* Rpt does not carry hdr deact cnf, so continue waiting on rpt queues
    */
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /* Set HDR deact by Hybr2 bsr flag
  */
  if((mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->hdr_deact_activate_stack == MM_STACK_2) &&
      (mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->active_stacks & BM(MM_STACK_2)))
  {
    sd_ss_set_hdr_deact_by_hybr2_bsr(mmoc_dev_info->sub_with_3gpp2);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Since HDR has been deactivated , send indication to SD that HDR service
   ** is lost. SD will then inform CM of HDR srv lost.
   */
#ifdef MMOC_HDR_SUPPORTED
  (void) sd_ss_ind_hdr_opr_sys_lost(mmoc_dev_info->sub_with_3gpp2,SD_SS_SYS_LOST_PROT_DEACTIVATE,NULL);
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Protocol in ss instance is made to acquire
  */
  mm_id.asubs_id = mmoc_dev_info->sub_with_3gpp2;
  mm_id.stk_id = mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->hdr_deact_activate_stack;

  MMOC_MSG_HIGH_2("SC_SVLTE:hdr_deact_activate sub %d stk %d",
                  mm_id.asubs_id, mm_id.stk_id);

  if(mmoc_activate_prot_online(mm_id, mmoc_info_ptr)
      == MMOC_EVT_STATUS_NOT_CONSUMED
    )
  {

    /* Code reaches here only when we have to exit PWR save state
    */
    MMOC_ASSERT (mmoc_dev_info->trans == MMOC_TRANS_PROT_GEN_CMD);

    /* Invoke the new transaction handler.
    */
    return MMOC_EVT_STATUS_NOT_CONSUMED;

  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If mmoc_clear_transaction gets called with is_activate_main as TRUE
  ** then activate_prot_online_ss_main will get called twice resulting in
  ** activation request being sent twice
  */
  if(mmoc_dev_info->trans_info.gen_prot_cmd.is_activate_main)
  {
    MMOC_ERR_0("NOT Expecting is_activate_main to be TRUE");
    MMOC_ASSERT(mmoc_dev_info->trans_info.gen_prot_cmd.is_activate_main == FALSE);
  }


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Done with processing, clear the transaction if MMOC is not waiting for
  ** activation notification.
  */
  if(mmoc_dev_info->trans_state != MMOC_TRANS_STATE_WAIT_ACTIVATION_CNF)
  {
    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  return MMOC_EVT_STATUS_CONSUMED;

} /* mmoc_process_common_hdr_deactd_cnf() */



/*===========================================================================

FUNCTION mmoc_process_common_hybr2_deact_enter

DESCRIPTION
  This is a common function for handling transaction state hybr2_deact_enter.
  It sets the next state to WAIT_DEACTD_CNF

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Should wait for deactivated confirmation.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start ph_stat_enter processing.

===========================================================================*/

static  mmoc_evt_status_e_type         mmoc_process_common_hybr2_deact_enter
(
  prot_deact_e_type              deact_reason,
  /* Reason for deactivating the active protocol.
  */

  mmoc_state_info_s_type        *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */

)
{
  SYS_ARG_NOT_CONST(mmoc_info_ptr);

  mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
  mmoc_info_ptr->dev_prop.deact_req_idx.stk_id = MM_STACK_2;

  (void)mmoc_deactivate_active_protocol(
    mmoc_info_ptr->dev_prop.deact_req_idx,
    deact_reason,
    mmoc_info_ptr
  );

  /* Deactivation request sent, wait for cnf.
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_HYBR2_DEACTD_CNF,
                           mmoc_info_ptr
                         );

  return MMOC_EVT_STATUS_CONSUMED;

} /*mmoc_process_common_hybr2_deact_enter () */


/*===========================================================================

FUNCTION mmoc_process_common_hybr2_deactd_cnf

DESCRIPTION
  This function handles the hdr deactivation cnf and proceeds to kick the
  MAIN instance to activate any protocol that was waiting on hdr deactf.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_common_hybr2_deactd_cnf
(
  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type               *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  MMOC_ASSERT( rpt_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (mmoc_process_common_deactd_cnf(rpt_ptr, mmoc_info_ptr) ==
      MMOC_EVT_STATUS_CONSUMED)
  {
    /* Rpt does not carry hdr deact cnf, so continue waiting on rpt queues
    */
    return MMOC_EVT_STATUS_CONSUMED;
  }
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Since hybr2 has been deactivated , send indication to SD that HYBR2 service
  ** is lost. SD will then inform CM of srv loss
  */
  {
    mm_sub_stk_id_s_type mm_id;
    mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
    mm_id.stk_id = MM_STACK_2;
    sd_ss_ind_misc_srv_lost(mm_id);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Protocol in ss instance is made to acquire
  */
  {
    mm_sub_stk_id_s_type mm_id;
    mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
    mm_id.stk_id = MM_STACK_0;
    if(mmoc_activate_prot_online(mm_id, mmoc_info_ptr) == MMOC_EVT_STATUS_NOT_CONSUMED)
    {
      /* Code reaches here only when we have to exit PWR save state
      */
      MMOC_ASSERT (mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_PROT_GEN_CMD);

      /* Invoke the new transaction handler.
      */
      return MMOC_EVT_STATUS_NOT_CONSUMED;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If mmoc_clear_transaction gets called with is_activate_main as TRUE
  ** then activate_prot_online_ss_main will get called twice resulting in
  ** activation request being sent twice
  */
  if (mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.is_activate_main)
  {
    MMOC_ERR_0("NOT Expecting is_activate_main to be TRUE");
    MMOC_ASSERT(mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.is_activate_main == FALSE);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Done with processing, clear the transaction if MMOC is not waiting for
  ** activation notification.
  */
  if(mmoc_info_ptr->dev_prop.trans_state != MMOC_TRANS_STATE_WAIT_ACTIVATION_CNF)
  {
    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  return MMOC_EVT_STATUS_CONSUMED;

} /* mmoc_process_common_hybr2_deactd_cnf() */

/*===========================================================================

FUNCTION mmoc_process_common_ph_stat_cnf

DESCRIPTION
   This function handles the common processing in the
 WAIT_PH_STAT_CNF transaction state.

  In this state, the state handler will
  1. Validate the phone status command ack received from the protocols.
  2. Move to PH_STAT_ENTER state to determine if phone status command
     has to be sent.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_common_ph_stat_cnf
(

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  MMOC_ASSERT( rpt_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if this is the correct ack.
  */
  if(rpt_ptr == NULL ||
      (rpt_ptr->rpt.name == MMOC_RPT_PH_STAT_CHGD_CNF &&
       ph_stat_cmd_tbl[rpt_ptr->param.ph_stat_cnf.prot].trans_id !=
       rpt_ptr->param.ph_stat_cnf.trans_id)
    )
  {
    /* Not the expected report.
    */
    MMOC_MSG_HIGH_0("Ph_stat_chgd_cnf: Trans id mismatch");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(!MMOC_ENUM_IS_INRANGE(rpt_ptr->param.ph_stat_cnf.prot,MMOC_MODE_MAX))
  {
    /* Not the expected report.
    */
    MMOC_ERR_0("Ph_stat_chgd_cnf:prot name error");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch(rpt_ptr->rpt.name)
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      return mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case MMOC_RPT_PH_STAT_CHGD_CNF:
      if(rpt_ptr->param.ph_stat_cnf.mc_status == MMOC_PH_STAT_ERR_SIM_BUSY)
      {
        mmoc_send_cm_rpt(CM_MMOC_SUBSCRIPTION_FAILURE, NULL,rpt_ptr->param.ph_stat_cnf.as_id);
      }

      /* Clear the pending cnf for protocol.
      */
      ph_stat_cmd_tbl[rpt_ptr->param.ph_stat_cnf.prot].trans_id = PROT_TRANS_NONE;

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

      /* Check if all the confirmation have been received.
      */
      {
        int i = 0;
        for(i = 0; i < (int)(ARR_SIZE(ph_stat_cmd_tbl)); i++)
        {
          if(ph_stat_cmd_tbl[i].trans_id != PROT_TRANS_NONE)
          {
            /* Still waiting for additional "cnf".
            */
            return MMOC_EVT_STATUS_CONSUMED;
          }
        }
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* All cnf. recvd. move to PH_STAT_ENTER.
      */
      mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_PH_STAT_ENTER,
                               mmoc_info_ptr
                             );


      return MMOC_EVT_STATUS_NOT_CONSUMED;

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    case MMOC_RPT_UE_MODE_SWITCH_CNF_1X:
    case MMOC_RPT_UE_MODE_SWITCH_CNF_HDR:
      MMOC_MSG_HIGH_1("Ignore report %d", rpt_ptr->rpt.name);
      return MMOC_EVT_STATUS_CONSUMED;

    default:
      /* Not the expected report.
      */
      MMOC_ERR_0("Unknown report");
      return MMOC_EVT_STATUS_CONSUMED;

  } //switch(rpt_ptr->rpt.name)

} /* mmoc_process_common_ph_stat_cnf() */

/* <EJECT> */
/*===========================================================================
=============================================================================
=============================================================================
==================== SUBSCRIPTION CHANGED TRANSACTION =======================
=============================================================================
=============================================================================
===========================================================================*/


/*===========================================================================

FUNCTION mmoc_process_subsc_chgd_null

DESCRIPTION
  This function handles the subscription changed transaction in the NULL
  transaction state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                 invoke the appropriate transaction handler
                                 again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_subsc_chgd_null
(

  mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received. */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information. */

)
{
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  mmoc_subsc_chgd_dev_prop_s_type *subsc_chgd_dev_info = &cmd_ptr->param.subsc_chgd.dev_prop;
  mmoc_subsc_chgd_sub_prop_s_type *subsc_chgd_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  boolean                  subs_cap_changed_only[MAX_SIMS] = { FALSE };
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  memscpy(&subsc_chgd_sub_info[0],
          sizeof(subsc_chgd_sub_info),
          &cmd_ptr->param.subsc_chgd.sub_prop[0],
          sizeof(cmd_ptr->param.subsc_chgd.sub_prop));

  MMOC_ASSERT( cmd_ptr != NULL );

  /* The subscription changed command from CM should only create this
  ** transaction.
  */
  if ( cmd_ptr == NULL ||
       cmd_ptr->cmd.name != MMOC_CMD_SUBSCRIPTION_CHGD
     )
  {
    MMOC_ERR_0("Subsc chgd trans incorrectly initiated");
    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Print subscription data */
  if((mmoc_dev_info->active_subs != subsc_chgd_dev_info->active_subs) ||
  	(mmoc_dev_info->device_mode !=subsc_chgd_dev_info->device_mode ))
  {
    MMOC_MSG_HIGH_5("SUBSC_CHGD: active_subs %d -> %d, device_mode %d -> %d, chg_type %d",
                    mmoc_dev_info->active_subs,
                    subsc_chgd_dev_info->active_subs,
                    mmoc_dev_info->device_mode,
                    subsc_chgd_dev_info->device_mode,
                    subsc_chgd_dev_info->chg_type);
  }
  for(sub = SYS_MODEM_AS_ID_1; sub < MAX_SIMS; sub++)
  {
    if(subsc_chgd_sub_info[sub] != NULL)
    {
      MMOC_MSG_HIGH_6("SUBSC_CHGD: as_id %d, active_stacks %d, prot_subsc_chg %d, orig_mode %d, mode_pref %d, hybr_pref %d",
                          subsc_chgd_sub_info[sub]->as_id,
                          subsc_chgd_sub_info[sub]->active_stacks,
                          subsc_chgd_sub_info[sub]->prot_subsc_chg,
                          subsc_chgd_sub_info[sub]->orig_mode,
                          subsc_chgd_sub_info[sub]->mode_pref,
                          subsc_chgd_sub_info[sub]->hybr_pref);

      MMOC_MSG_HIGH_6("SUBSC_CHGD: sub_avail %d, camp_mode %d, band_pref 0x%x 0x%x, tds_band_pref 0x%x 0x%x",
                      subsc_chgd_sub_info[sub]->sub_avail,
                      subsc_chgd_sub_info[sub]->camp_mode,
                      QWORD_HIGH(subsc_chgd_sub_info[sub]->band_pref),
                      QWORD_LOW( subsc_chgd_sub_info[sub]->band_pref),
                      QWORD_HIGH(subsc_chgd_sub_info[sub]->tds_band_pref),
                      QWORD_LOW( subsc_chgd_sub_info[sub]->tds_band_pref));

      mmoc_print_lte_band_mask(subsc_chgd_sub_info[sub]->lte_band_pref);
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update MMOC state info */

  if(mmoc_dev_info->active_subs  != subsc_chgd_dev_info->active_subs ||
      mmoc_dev_info->standby_pref != subsc_chgd_dev_info->ds_pref)
  {
    uint8 active_sims = 0;

    mmoc_dev_info->prev_standby_pref = mmoc_dev_info->standby_pref;
    mmoc_dev_info->prev_active_subs  = mmoc_dev_info->active_subs;
    mmoc_dev_info->prev_device_mode  = mmoc_dev_info->device_mode;

    mmoc_dev_info->active_subs = subsc_chgd_dev_info->active_subs;
    mmoc_dev_info->standby_pref = subsc_chgd_dev_info->ds_pref;

  }

  mmoc_dev_info->device_mode = subsc_chgd_dev_info->device_mode;

  if(mmoc_dev_info->prev_standby_pref != mmoc_dev_info->standby_pref)
  {
    MMOC_MSG_HIGH_2("SUBSC_CHGD: standby_pref %d -> %d",
                    mmoc_dev_info->prev_standby_pref,
                    mmoc_dev_info->standby_pref);
  }
  
  mmoc_check_if_only_subs_cap_chgd(cmd_ptr, subs_cap_changed_only, mmoc_info_ptr);
  mmoc_dev_info->is_sd_init_called = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Copy device/subscription level transaction info */

  memscpy(&mmoc_dev_info->subsc_chgd.dev_prop,
          sizeof(mmoc_subsc_chgd_dev_prop_s_type),
          &cmd_ptr->param.subsc_chgd.dev_prop,
          sizeof(mmoc_subsc_chgd_dev_prop_s_type));

  for(sub = SYS_MODEM_AS_ID_1; sub < MAX_SIMS; sub++)
  {
    if(subsc_chgd_sub_info[sub] != NULL)
    {
      if(mmoc_dev_info->subsc_chgd.sub_prop[sub] == NULL)
      {
        mmoc_dev_info->subsc_chgd.sub_prop[sub] =
          (mmoc_subsc_chgd_sub_prop_s_type *) mmoc_mem_alloc(
            sizeof(mmoc_subsc_chgd_sub_prop_s_type));
      }
      memscpy(mmoc_dev_info->subsc_chgd.sub_prop[sub],
              sizeof(mmoc_subsc_chgd_sub_prop_s_type),
              subsc_chgd_sub_info[sub],
              sizeof(mmoc_subsc_chgd_sub_prop_s_type));
    }
    else if(mmoc_dev_info->subsc_chgd.sub_prop[sub] != NULL)
    {
      mmoc_mem_free(mmoc_dev_info->subsc_chgd.sub_prop[sub]);
      mmoc_dev_info->subsc_chgd.sub_prop[sub] = NULL;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check for only subs cap chgd in MSIM */

  if(mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM))
  {
    for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
    {
      if(mmoc_dev_info->subsc_chgd.sub_prop[sub] != NULL)
      {
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->only_subs_cap_change =
          subs_cap_changed_only[sub];
      }
    }
  }

  mmoc_dev_info->subsc_chgd.nSubs = cmd_ptr->param.subsc_chgd.nSubs;



  for(sub = SYS_MODEM_AS_ID_1; sub < MAX_SIMS; sub++)
  {
    if(subs_cap_changed_only[sub] == TRUE)
    {
      MMOC_MSG_HIGH_2("as_id %d sub_cap_changed_only %d", sub, subs_cap_changed_only[sub]);
  }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Store subscription info in MMOC state info */

  for(sub = SYS_MODEM_AS_ID_1; (sub < MAX_SIMS && sub < mmoc_info_ptr->nSubs); sub++)
  {
    if(subsc_chgd_sub_info[sub] != NULL)
    {
      mmoc_sub_info[sub]->prot_subsc_chg = subsc_chgd_sub_info[sub]->prot_subsc_chg;
      mmoc_sub_info[sub]->nam = mmoc_dev_info->subsc_chgd.dev_prop.nam;

      if(CHECK_PROTO(mmoc_sub_info[sub]->prot_subsc_chg, PROT_SUBSC_CHG_ALL))
      {
        mmoc_sub_info[sub]->sub_avail = subsc_chgd_sub_info[sub]->sub_avail;
      }

      if(subsc_chgd_sub_info[sub]->subs_capability & BM(SYS_SYS_MODE_CDMA))
      {
        mmoc_dev_info->prev_sub_with_3gpp2 = mmoc_dev_info->sub_with_3gpp2;
        mmoc_dev_info->sub_with_3gpp2 = sub;
      }

      if(mmoc_is_1x_sxlte(sub) &&
          sub == mmoc_dev_info->sub_with_3gpp2 &&
          mmoc_dev_info->onebuild_feature == SYS_OVERALL_FEATURE_MODE_MULTISIM)
      {
        MMOC_MSG_HIGH_3("SUBSC_CHGD: sub %d, ue_mode %d, is_ue_mode_substate_srlte=%d",
                        sub, mmoc_dev_info->subsc_chgd.sub_prop[sub]->ue_mode,
                        mmoc_dev_info->subsc_chgd.sub_prop[sub]->is_ue_mode_substate_srlte);
      }

      if(mmoc_sub_info[sub]->active_stacks != subsc_chgd_sub_info[sub]->active_stacks)
      {
        MMOC_MSG_HIGH_3("SUBSC_CHGD: sub %d active_stacks %d -> %d", sub,
                        mmoc_sub_info[sub]->active_stacks,
                        subsc_chgd_sub_info[sub]->active_stacks);

        mmoc_sub_info[sub]->prev_active_stacks = mmoc_sub_info[sub]->active_stacks;
        mmoc_sub_info[sub]->active_stacks = subsc_chgd_sub_info[sub]->active_stacks;
      }
    }
    else
    {
      mmoc_sub_info[sub]->prot_subsc_chg = PROT_SUBSC_CHG_NONE;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_MMODE_DUAL_SIM

  for(sub = SYS_MODEM_AS_ID_1; (sub < MAX_SIMS && sub < mmoc_info_ptr->nSubs); sub++)
  {
    if(subsc_chgd_sub_info[sub] != NULL)
    {
      if(mmoc_sub_info[sub]->subs_capability != subsc_chgd_sub_info[sub]->subs_capability)
      {
          MMOC_MSG_HIGH_3("SUBSC_CHGD: as_id %d, subs_capability 0x%x -> 0x%x",
                        sub,
                        mmoc_sub_info[sub]->subs_capability,
                        subsc_chgd_sub_info[sub]->subs_capability);
      }
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If any subs capability changed, mark that SUB as 3GPP and 3GPP2 change 
   ** Otherwise we may not send ph_status_chgd to 3gpp2 protocols if 1x capaility changes */

  for(sub = SYS_MODEM_AS_ID_1; (sub < MAX_SIMS && sub < mmoc_info_ptr->nSubs); sub++)
  {
    if(subsc_chgd_sub_info[sub] != NULL &&
        mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM))
    {
      if(mmoc_sub_info[sub]->subs_capability !=
          subsc_chgd_sub_info[sub]->subs_capability &&
          subsc_chgd_sub_info[sub]->subs_capability != 0 &&
         mmoc_sub_info[sub]->prot_subsc_chg != PROT_SUBSC_CHG_ALL )
      {
        if( mmoc_sub_info[sub]->prot_subsc_chg == PROT_SUBSC_CHG_NONE )
        {
          mmoc_dev_info->subsc_chgd.sub_prop[sub]->only_subs_cap_change = TRUE;
        }
        mmoc_sub_info[sub]->prot_subsc_chg |= PROT_SUBSC_CHG_ALL;
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->as_id = subsc_chgd_sub_info[sub]->as_id;
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->active_stacks = subsc_chgd_sub_info[sub]->active_stacks;
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->mode_pref = subsc_chgd_sub_info[sub]->mode_pref;
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->sub_avail |= (mmoc_sub_info[sub]->sub_avail & PROT_SUBSC_MASK_ALL);
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->nv_context = subsc_chgd_sub_info[sub]->nv_context;
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->subs_capability = subsc_chgd_sub_info[sub]->subs_capability;
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->gpp_session_type = subsc_chgd_sub_info[sub]->gpp_session_type;
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->gpp2_session_type = subsc_chgd_sub_info[sub]->gpp2_session_type;
      }
      else if(mmoc_sub_info[sub]->prot_subsc_chg == PROT_SUBSC_CHG_NONE &&
              ((mmoc_dev_info->prev_active_subs ^ mmoc_dev_info->active_subs) & BM(sub)))
      {
        /* If initially there was no subscription available (only primary SUB was
        ** active by default) and now secondary/tertiary SUB became available,
        ** mark primary SUB as changed so Stop Mode will be sent for primary SUB.
        */

        MMOC_MSG_HIGH_1("Mark SUB: %d as 3GPP chg since change in active sub",sub);
        mmoc_sub_info[sub]->prot_subsc_chg |= PROT_SUBSC_CHG_3GPP;
        if(mmoc_dev_info->subsc_chgd.sub_prop[sub] == NULL)
        {
          mmoc_dev_info->subsc_chgd.sub_prop[sub] =
            (mmoc_subsc_chgd_sub_prop_s_type *) mmoc_mem_alloc(
              sizeof(mmoc_subsc_chgd_sub_prop_s_type));
          mmoc_dev_info->subsc_chgd.sub_prop[sub]->as_id = subsc_chgd_sub_info[sub]->as_id;
          mmoc_dev_info->subsc_chgd.sub_prop[sub]->active_stacks = subsc_chgd_sub_info[sub]->active_stacks;
        }
      }
    }
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Capture the error state - should not happen
  ** Error fatal when mode pref is not sub-set of max mode cap
  */
#ifndef MMOC_DEBUG
  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_dev_info->subsc_chgd.nSubs && sub < MAX_SIMS; sub++)
  {
    if((mmoc_dev_info->active_subs & BM(sub)) &&
       ((uint32)mmoc_dev_info->subsc_chgd.sub_prop[sub]->mode_pref &
       ~(mmoc_dev_info->subsc_chgd.sub_prop[sub]->subs_capability)) != 0 &&
         mmoc_dev_info->subsc_chgd.sub_prop[sub]->subs_capability != 0
      )
    {
      MMOC_ERR_FATAL_3("as_id %d maxcap 0x%x and mode 0x%x out of sync",
                       mmoc_dev_info->subsc_chgd.sub_prop[sub]->as_id,
                       mmoc_dev_info->subsc_chgd.sub_prop[sub]->subs_capability,
                       mmoc_dev_info->subsc_chgd.sub_prop[sub]->mode_pref);
    }
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Retrieve the bitmask to identify which items to be read from card */

#if defined (FEATURE_RUIM) && defined (FEATURE_MMGSDI_SESSION_READ_PRL)

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    mmgsdi_info_ptr->read_items[sub] = mmocmmgsdi_get_bitmask_to_read_items(mmoc_info_ptr,sub);
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Free subsc cmd specific memory that are created within the cmd_ptr since the info has been copied to
  ** mmoc globals
  */
  for(sub = SYS_MODEM_AS_ID_1; sub < cmd_ptr->param.subsc_chgd.nSubs && sub < MAX_SIMS; sub++)
  {
    if(cmd_ptr->param.subsc_chgd.sub_prop[sub] != NULL)
    {
      mmoc_mem_free(cmd_ptr->param.subsc_chgd.sub_prop[sub]);
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Clear the subsc chgd transaction here if UE is in DSDS and there is no
  ** active_subs change, and the non-1x-capable sub has 3GPP2 subsc chg while
  ** the 1x sub has no subsc chg, and 1x capability remains on the 1x sub
  */
#ifdef FEATURE_MMODE_DUAL_SIM
  {
    sys_modem_as_id_e_type non_1x_sub = (mmoc_dev_info->sub_with_3gpp2 == SYS_MODEM_AS_ID_1 ?
                                         SYS_MODEM_AS_ID_2 : SYS_MODEM_AS_ID_1);

    if(mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM) &&
        mmoc_dev_info->sub_with_3gpp2 == mmoc_dev_info->prev_sub_with_3gpp2 &&
        mmoc_dev_info->active_subs == mmoc_dev_info->prev_active_subs &&
        mmoc_dev_info->active_subs == (SYS_MODEM_AS_ID_1_MASK | SYS_MODEM_AS_ID_2_MASK) &&
        subsc_chgd_sub_info[non_1x_sub] != NULL &&
        subsc_chgd_sub_info[non_1x_sub]->prot_subsc_chg == PROT_SUBSC_CHG_3GPP2 &&
        (subsc_chgd_sub_info[mmoc_dev_info->sub_with_3gpp2] == NULL ||
         subsc_chgd_sub_info[mmoc_dev_info->sub_with_3gpp2]->prot_subsc_chg == PROT_SUBSC_CHG_NONE))
    {
      MMOC_MSG_HIGH_0("non-1x sub has 3GPP2 subsc chg but no 1x capability chg, clear transaction");
      return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
    }
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Reset SD nam sel mask */
  mmoc_dev_info->sd_nam_sel_mask = 0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Deactivate the active protocols, Move to DEACT_ENTER trans state. */

  mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_DEACT_ENTER,
                           mmoc_info_ptr
                         );


  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_subsc_chgd_null() */



/*===========================================================================

FUNCTION mmoc_process_subsc_chgd_ph_stat_enter

DESCRIPTION
  This function handles the subsc chgd transaction in the PH_STAT_ENTER
  transaction state.

  In this transaction state, the handler will
  1. Send the Phone status changed command to the next supported protocol
     and move to WAIT_PH_STAT_CNF.
     If the phone status changed command was sent to all the protocols,
     then SD is initialized with new subscription and activate the protocol
     if the operating mode is online.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type       mmoc_process_subsc_chgd_ph_stat_enter(

  const mmoc_cmd_msg_s_type      *cmd_ptr,
  /* Pointer to the command event that was received. */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  prot_ph_stat_chg_e_type chg_type = PROT_PH_STAT_CHG_MAX;
  sd_ss_act_e_type sd_act = SD_SS_ACT_CONTINUE;
  boolean is_subsc_chg = FALSE;
  static boolean dual_standby_pref_changed = FALSE;
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stack = MM_STACK_NONE;
  mmoc_evt_status_e_type status = MMOC_EVT_STATUS_CONSUMED;

#ifdef FEATURE_MMODE_DUAL_SIM
  boolean is_priority_sub_activated = FALSE;
  boolean is_sub_on_hold_sleep = FALSE;
  sys_modem_as_id_e_type  sub_on_hold = SYS_MODEM_AS_ID_NONE;
#endif

  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* In MSIM config, mode change becomes subscription change */

  if (mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM) &&
      mmoc_dev_info->subsc_chgd.dev_prop.chg_type == MMOC_SUBSC_CHG_MODE_CHNG)
  {
    /* This might change later based on protocols decision to optimize */
    mmoc_dev_info->subsc_chgd.dev_prop.chg_type = MMOC_SUBSC_CHG_SUBSC_AVAIL;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Determine the phone status change type */

  if(mmoc_dev_info->subsc_chgd.dev_prop.chg_type == MMOC_SUBSC_CHG_SUBSC_AVAIL)
  {
    chg_type = PROT_PH_STAT_CHG_SUBSC;
  }
  else if(mmoc_dev_info->subsc_chgd.dev_prop.chg_type == MMOC_SUBSC_CHG_NAM_SEL)
  {
    chg_type = PROT_PH_STAT_CHG_NAM;
  }

  MMOC_MSG_HIGH_3("SUBSC_CHGD: PH_STAT: chg_type %d, sd_initialized %d, sd_init_called %d",
                  chg_type,
                  mmoc_dev_info->is_sd_initialized,
                  mmoc_dev_info->is_sd_init_called);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Update previous active stacks with current active stacks*/
  for(sub = SYS_MODEM_AS_ID_1; (sub < MAX_SIMS && sub < mmoc_info_ptr->nSubs); sub++)
  {
    if(mmoc_sub_info[sub]->prev_active_stacks != mmoc_sub_info[sub]->active_stacks)
    {
      MMOC_MSG_HIGH_3("PH_STAT: SUB %d, active_stacks: %d->%d",
                      sub,
                      mmoc_sub_info[sub]->prev_active_stacks,
                      mmoc_sub_info[sub]->active_stacks);
    }
    mmoc_sub_info[sub]->prev_active_stacks = mmoc_sub_info[sub]->active_stacks;
  }

  /* Store new mode pref and is_ue_mode_substate_srlte */
  for(sub = SYS_MODEM_AS_ID_1; (sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS); sub++)
  {
    if(mmoc_dev_info->subsc_chgd.sub_prop[sub] != NULL &&
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->mode_pref != mmoc_sub_info[sub]->mode_pref)
    {
      mmoc_sub_info[sub]->mode_pref = mmoc_dev_info->subsc_chgd.sub_prop[sub]->mode_pref;
    }

    if (mmoc_dev_info->subsc_chgd.sub_prop[sub] != NULL &&
        mmoc_dev_info->subsc_chgd.sub_prop[sub]->is_ue_mode_substate_srlte != mmoc_sub_info[sub]->is_srlte)
    {
      MMOC_MSG_HIGH_3("PH_STAT: SUB %d, is_srlte: %d->%d",
                  sub,
                  mmoc_sub_info[sub]->is_srlte,
                  mmoc_dev_info->subsc_chgd.sub_prop[sub]->is_ue_mode_substate_srlte);

      mmoc_sub_info[sub]->is_srlte 
        = mmoc_dev_info->subsc_chgd.sub_prop[sub]->is_ue_mode_substate_srlte;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_MMOC_LTE
  /* Update lte rrc only if task is not stopped since LTE deregisters MSGR once task stop and
     will cause crash
  */
  if (!cm_is_task_stop_sig_rcvd())
  {
    mmoc_send_lte_rrc_mode_pref_update();
  }
  else
  {
    MMOC_MSG_HIGH_0("Skip sending LTE_RRC_MODE_PREF_UPDATE_IND since task stop sig is received");
  }
#endif

#ifdef FEATURE_MMODE_DUAL_SIM


  /* Inform Subs capability change to all protocols */
  if(mmoc_subs_capability_chgd(mmoc_info_ptr))
  {
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_SUBS_CAP_CHGD_ENTER,
                             mmoc_info_ptr
                           );

    return MMOC_EVT_STATUS_NOT_CONSUMED;
  }

  /* Inform Dual Standby status to all protocols */
  if(mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM) &&
      (mmoc_dual_standby_pref_chgd(mmoc_info_ptr) ||
       mmoc_dev_info->prev_device_mode != mmoc_dev_info->device_mode) &&
      mmoc_dev_info->standby_pref != SYS_MODEM_DS_PREF_NO_STANDBY_PREF &&
      mmoc_dev_info->active_subs  != SYS_MODEM_AS_ID_NO_ACTIVE_MASK
    )
  {
    if(mmoc_dual_standby_pref_chgd(mmoc_info_ptr))
    {
      dual_standby_pref_changed = TRUE;
    }
    /* Send DS stat chgd status
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_DS_STAT_CHGD_ENTER,
                             mmoc_info_ptr
                           );
    return MMOC_EVT_STATUS_NOT_CONSUMED;
  }
  else
#endif //FEATURE_MMODE_DUAL_SIM
  {

    mmoc_dev_info->prev_standby_pref = mmoc_dev_info->standby_pref;
    mmoc_dev_info->prev_active_subs = mmoc_dev_info->active_subs;
    mmoc_dev_info->prev_device_mode = mmoc_dev_info->device_mode;
  }

#ifdef FEATURE_MMODE_DUAL_SIM
  if(cm_get_device_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE &&
      !mmoc_is_any_sxlte())
  {
    /* Updated to protocols, resetting standby_pref_changed */
    dual_standby_pref_changed = FALSE;
  }

#endif //FEATURE_MMODE_DUAL_SIM


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Intialise SD only if it is not intialised before*/

  if(!mmoc_dev_info->is_sd_initialized &&
      !mmoc_dev_info->is_sd_init_called)
  {
    sd_init2( MMOC_NV_CMD_SIG, MMOC_RPT_TIMER_SIG, mmoc_kick_dog,
              mmoc_timer_cb
            );

    mmoc_dev_info->is_sd_init_called = TRUE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check to see whether read request needs to be sent to MMGSDI
  */
  if(mmoc_send_mmgsdi_card_read_req())
  {
    /* Provisioning items need to be retrieved from MMGSDI card
    ** Move to MMOC_TRANS_STATE_MMGSDI_READ_ENTER to get those items.
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_MMGSDI_READ_ENTER,
                             mmoc_info_ptr
                           );
    return MMOC_EVT_STATUS_NOT_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send ASID/VSID mapping to MSGR */
  {
    mmoc_vfr_map_s_type *vfr_map_ind;

    vfr_map_ind = (mmoc_vfr_map_s_type *) mmoc_mem_alloc(sizeof(mmoc_vfr_map_s_type));

    if(vfr_map_ind == NULL)
    {
      sys_err_fatal_null_ptr_exception();
      return MMOC_EVT_STATUS_CONSUMED;
    }

    cm_get_vfr_map(&vfr_map_ind->vfr_map[0]);

    MMOC_MSG_HIGH_2("MMOC->MSGR: Send VFR map %d %d",
                    vfr_map_ind->vfr_map[0],
                    vfr_map_ind->vfr_map[1]);

    mmoc_msgr_send_else_err_fatal( MM_MMOC_VFR_IND,
                                   &(vfr_map_ind->msg_hdr),
                                   sizeof(mmoc_vfr_map_s_type),
                                   MSGR_NO_VARIANT);

    mmoc_mem_free(vfr_map_ind);
  }

  /* Send the phone status changed command to the next supported
  ** protocol.
  */
  if(mmoc_send_subsc_chg_ph_stat_chgd_cmd(chg_type, mmoc_info_ptr))
  {
    /* Phone status changed command sent, so wait for ack.
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_PH_STAT_CNF,
                             mmoc_info_ptr
                           );
    return MMOC_EVT_STATUS_CONSUMED;
  }


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If either CDMA or GW subscription has changed, update SD with this info */

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    /* If NAM SEL is to be called for this sub, and not called yet, send now */
    if((mmoc_dev_info->subsc_chgd.sub_prop[sub] != NULL &&
        CHECK_PROTO(mmoc_sub_info[sub]->prot_subsc_chg, PROT_SUBSC_CHG_ALL)) &&
        !(mmoc_dev_info->sd_nam_sel_mask & BM(sub)) )
    {
      mmoc_send_subsc_chgd_sd_nam_sel_cmd(mmoc_info_ptr, mmoc_sub_info[sub],sub);
      mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_SD_NAM_SEL_CNF,
                               mmoc_info_ptr
                             );
      return MMOC_EVT_STATUS_CONSUMED;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Before activating SD, clear any existing timers in MMOC
  */
  mmoc_reset_timers(mmoc_info_ptr);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Set MMoC's state info to SD initialized.
  */
  mmoc_set_sd_initialized( mmoc_info_ptr );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if we can vote off UIM.
  */
#ifdef FEATURE_UIM_RUIM
  if(mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->sub_avail)
  {
    /* Indicate to power down the RUIM */
    (void)uim_power_control(UIM_MC, FALSE);
  }
#endif /* FEATURE_UIM_RUIM */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update ps_enabled for each sub in MSIM configuration */

#ifdef FEATURE_MMODE_DUAL_SIM
  if(mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM))
  {
    mmoc_update_ps_enabled_subsc_chgd_trans(mmoc_info_ptr);

    for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
    {
      MMOC_MSG_HIGH_2("sub %d, ps_enabled %d", sub, mmoc_sub_info[sub]->ps_enabled);
    }
  }

  /* If the device is CMCC subsidized and a CMCC card is inserted, the PS sub should be
  ** started first. Otherwise, the non PS sub should be started first.
  */
  mmoc_dev_info->is_ps_priority = TRUE;

  MMOC_MSG_HIGH_1("START_DDS: %d",mmoc_dev_info->is_ps_priority);

  /* Calculate the sub on hold, which will be suspended for 2s
  ** before power up. */
  sub_on_hold = mmoc_calculate_sub_on_hold(mmoc_info_ptr);

#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Activate the protocols if in ONLINE mode */

  if(mmoc_dev_info->oprt_mode == PROT_OPRT_MODE_ONLINE)
  {
    mmoc_print_pwrup_parameters(mmoc_info_ptr);

    /* Subscription changed transaction due to subscription availability */
    if(mmoc_dev_info->subsc_chgd.dev_prop.chg_type == MMOC_SUBSC_CHG_SUBSC_AVAIL)
    {
      /* Send SD the power up indication and activate the protocols */

#ifdef FEATURE_MMODE_DUAL_SIM

      /* Both subs need to be activated, with 2s sleep in between */
      if(sub_on_hold != SYS_MODEM_AS_ID_NONE &&
          (CHECK_PROTO(mmoc_sub_info[SYS_MODEM_AS_ID_1]->prot_subsc_chg, PROT_SUBSC_CHG_ALL) ||
           (dual_standby_pref_changed && mmoc_dev_info->dual_switch_subs & BM(SYS_MODEM_AS_ID_1))) &&
          (CHECK_PROTO(mmoc_sub_info[SYS_MODEM_AS_ID_2]->prot_subsc_chg, PROT_SUBSC_CHG_ALL) ||
           (dual_standby_pref_changed && mmoc_dev_info->dual_switch_subs & BM(SYS_MODEM_AS_ID_2))))
      {
        sys_modem_as_id_e_type sub_not_on_hold = (sub_on_hold == SYS_MODEM_AS_ID_1) ?
            SYS_MODEM_AS_ID_2 : SYS_MODEM_AS_ID_1;
        boolean delay_srv_req = FALSE;

        // Power up the sub not on hold
        if(mmoc_dev_info->active_subs & BM(sub_not_on_hold))
        {
          MMOC_MSG_HIGH_1("MMOC->SD: Activating sub %d", sub_not_on_hold);
          sd_act = sd_ss_ind_user_pwr_up(FALSE, sub_not_on_hold, NULL);
        }
        else
        {
          mm_sub_stk_id_s_type mm_id;
          mm_id.asubs_id = sub_not_on_hold;
          mm_id.stk_id   = MM_STACK_0;
          sd_act = sd_ss_act_get_per_mm_id(NULL, mm_id);
        }

        mmoc_activate_prot_online_ss_main(sd_act, sub_not_on_hold, mmoc_info_ptr);

        // Wait 2s or srv 

        if(sd_act == SD_SS_ACT_ACQ_CDMA
            || sd_act == SD_SS_ACT_ACQ_GWL
            || sd_act == SD_SS_ACT_ACQ_HDR)
        {
          mmoc_hold_acq(sub_on_hold, mmoc_info_ptr);

          /* need to delay srv req for non dds sub (sub_on_hold) for srlte+x config */
          delay_srv_req = mmoc_delay_srv_req(sub_on_hold, mmoc_info_ptr);
        }

        // Power up the sub on hold
        if(mmoc_dev_info->active_subs & BM(sub_on_hold))
        {
          MMOC_MSG_HIGH_1("MMOC->SD: Activating sub %d", sub_on_hold);
          sd_act = sd_ss_ind_user_pwr_up(FALSE, sub_on_hold, NULL);
        }
        else
        {
          mm_sub_stk_id_s_type mm_id;
          mm_id.asubs_id = sub_on_hold;
          mm_id.stk_id   = MM_STACK_0;
          sd_act = sd_ss_act_get_per_mm_id(NULL, mm_id);
          delay_srv_req = FALSE;
        }

        mmoc_activate_prot_online_ss_main2(sd_act, sub_on_hold, mmoc_info_ptr,
                                             delay_srv_req);
      }

      /* Power up all active subs with a subsc_chg or need to be
      ** activated due to dual_switch */
      else
      {
        for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
        {
          if((mmoc_dev_info->active_subs & BM(sub))
              &&
              (CHECK_PROTO(mmoc_sub_info[sub]->prot_subsc_chg, PROT_SUBSC_CHG_ALL)
               || (dual_standby_pref_changed && mmoc_dev_info->dual_switch_subs & BM(sub)))
            )
          {
            if(mmoc_dev_info->active_subs & BM(sub))
            {
              MMOC_MSG_HIGH_1("MMOC->SD: Activating sub %d", sub);
              sd_act = sd_ss_ind_user_pwr_up(FALSE, sub, NULL);
            }
            else
            {
              mm_sub_stk_id_s_type mm_id;
              mm_id.asubs_id = sub;
              mm_id.stk_id   = MM_STACK_0;
              sd_act = sd_ss_act_get_per_mm_id(NULL, mm_id);
            }

            mmoc_activate_prot_online_ss_main(sd_act, sub, mmoc_info_ptr);
          }
        }
      }

#else // SSIM

      if(CHECK_PROTO(mmoc_sub_info[SYS_MODEM_AS_ID_1]->prot_subsc_chg, PROT_SUBSC_CHG_ALL))
      {
        if(mmoc_dev_info->active_subs & SYS_MODEM_AS_ID_1_MASK)
        {
          MMOC_MSG_HIGH_0("MMOC->SD: Activating primary sub");
          sd_act = sd_ss_ind_user_pwr_up( FALSE, SYS_MODEM_AS_ID_1, NULL );
        }
        else
        {
          mm_sub_stk_id_s_type mm_id;
          mm_id.asubs_id = SYS_MODEM_AS_ID_1;
          mm_id.stk_id   = MM_STACK_0;
          sd_act = sd_ss_act_get_per_mm_id(NULL, mm_id);
        }

        status = mmoc_activate_prot_online_ss_main(sd_act, SYS_MODEM_AS_ID_1, mmoc_info_ptr);
      }
#endif

      /* Send oprt online on stack1/stack2 of 3GPP2 sub */
      {
        mm_sub_stk_id_s_type mm_id;
        mm_id.asubs_id = mmoc_dev_info->sub_with_3gpp2;

        if(mmoc_is_1x_sxlte(mmoc_dev_info->sub_with_3gpp2) &&
            (mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->active_stacks & MM_STACK_2_MASK))
        {
          mm_id.stk_id = MM_STACK_2;
          sd_misc_oprt_mode_online(mm_id);
        }

#ifdef FEATURE_HDR_HYBRID
        if(mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->active_stacks & MM_STACK_1_MASK)
        {
          mm_id.stk_id = MM_STACK_1;
          sd_misc_oprt_mode_online(mm_id);
        }
#endif
      }

      if(status == MMOC_EVT_STATUS_NOT_CONSUMED)
      {
        dual_standby_pref_changed = FALSE;
        return MMOC_EVT_STATUS_NOT_CONSUMED;
      }

    }
    else
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = SYS_MODEM_AS_ID_1;
      mm_id.stk_id   = MM_STACK_0;
      sd_act = sd_ss_ind_user_nam_sel( mm_id, NULL );

      /* SD subscription init successful, activate the protocol or invoke
      ** transaction handler if this function returns EVT_STATUS_NOT_CONSUMED.
      */
      if ( mmoc_activate_prot_online(mm_id, mmoc_info_ptr)
           == MMOC_EVT_STATUS_NOT_CONSUMED)
      {
        /* Invoke the new transaction handler.
        */
        return MMOC_EVT_STATUS_NOT_CONSUMED;
      }
    }

  }
  else if ( mmoc_dev_info->oprt_mode == PROT_OPRT_MODE_OFFLINE_CDMA ||
            mmoc_dev_info->oprt_mode == PROT_OPRT_MODE_FTM
          )
  {
    /* Activate the protocol, if needed in the offline mode.
    */
    mmoc_activate_prot_offline( mmoc_info_ptr );

  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Clear the transaction.
  */
  return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );

} /* mmoc_process_subsc_chgd_ph_stat_enter() */


/*===========================================================================

FUNCTION mmoc_process_subsc_chgd_mmgsdi_read_enter

DESCRIPTION
  This is a function for handling the transaction state MMGSDI_READ_ENTER
  for the subscription changed transaction. If a MMGSDI session is used
  to read PRL, move to WAIT_MMGSDI_CNF else move to PH_STAT_ENTER.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Should wait for response for prl read.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start ph_stat_enter processing.

===========================================================================*/
static mmoc_evt_status_e_type      mmoc_process_subsc_chgd_mmgsdi_read_enter
(
  const mmoc_rpt_msg_s_type    *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  const mmoc_state_info_s_type *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_evt_status_e_type evt_status = MMOC_EVT_STATUS_CONSUMED;

  evt_status = mmocmmgsdi_process_subsc_chgd_mmgsdi_read_enter
               (rpt_ptr,
                (mmoc_state_info_s_type *) mmoc_info_ptr
               );

  return (evt_status);
} /* mmoc_process_subsc_chgd_mmgsdi_read_enter */


/*===========================================================================

FUNCTION mmoc_process_subsc_chgd_mmgsdi_read_cnf

DESCRIPTION
  This is a function for handling the transaction state MMGSDI_READ_CNF for the
  subscription changed transaction . If the report is MMOC_RPT_MMGSDI_CNF,
  move to PH_STAT_ENTER else stay in the same state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Processing finished.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start ph_stat_enter processing.

===========================================================================*/
static mmoc_evt_status_e_type         mmoc_process_subsc_chgd_mmgsdi_read_cnf
(
  const mmoc_rpt_msg_s_type     *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type        *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_evt_status_e_type evt_status = MMOC_EVT_STATUS_CONSUMED;

  evt_status = mmocmmgsdi_process_subsc_chgd_mmgsdi_read_cnf
               (rpt_ptr,
                mmoc_info_ptr
               );

  return (evt_status);
} /* mmoc_process_subsc_chgd_mmgsdi_read_cnf */



/*===========================================================================

FUNCTION mmoc_process_common_ds_stat_chgd_enter

DESCRIPTION
  This is a utility function called to send dual standby stat chgd cmd to protocols,
  if dual standby status is changed

RETURN VALUE
  TRUE  - Phone status command was sent to support mode.
  FALSE - Completed sending the phone status command to all supported modes.

===========================================================================*/
static mmoc_evt_status_e_type  mmoc_process_common_ds_stat_chgd_enter
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,

  mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check for a standby pref change */

  if((mmoc_dual_standby_pref_chgd(mmoc_info_ptr) ||
      mmoc_dev_info->prev_device_mode != mmoc_dev_info->device_mode) &&
      mmoc_dev_info->standby_pref != SYS_MODEM_DS_PREF_NO_STANDBY_PREF &&
      mmoc_dev_info->active_subs  != SYS_MODEM_AS_ID_NO_ACTIVE_MASK
    )
  {
#ifdef FEATURE_MMODE_DUAL_SIM
#if defined(MMOC_CDMA_SUPPORTED) && defined(MMOC_USES_DB_SERVICES)
    db_items_value_type  db_item;
#endif

#ifdef MMOC_HDR_SUPPORTED
    /* Inform DS stat chgd to HDR */

    if(mmoc_dev_info->prev_standby_pref != mmoc_dev_info->standby_pref)
    {

      mmoc_send_hdr_standby_pref_chgd_cmd(mmoc_next_trans_id(mmoc_info_ptr),
                                          mmoc_dev_info->standby_pref);
    }

#endif

    MMOC_MSG_HIGH_1("active_subs %d", mmoc_dev_info->active_subs);
    if (mmoc_is_standby_pref_and_active_subs_in_sync(mmoc_info_ptr))
    {
      sp_update_active_sub_info(mmoc_dev_info->active_subs);
    }

#ifdef FEATURE_MMOC_LTE
    if (!cm_is_task_stop_sig_rcvd())
    {
      mmoc_send_lte_rrc_mode_pref_update();
    }
    else
    {
      MMOC_MSG_HIGH_0("Skip sending LTE_RRC_MODE_PREF_UPDATE_IND since task stop sig is received");
    }
#endif

#if defined(MMOC_CDMA_SUPPORTED)
    /* Indicating to 1X CP only when sub1 is being activated,
    ** so that 1X CP can perform power up registration
    */

    if(!(mmoc_dev_info->prev_active_subs & BM(mmoc_dev_info->sub_with_3gpp2))
        && mmoc_dev_info->active_subs & BM(mmoc_dev_info->sub_with_3gpp2)
        && mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->active_stacks & MM_STACK_0_MASK)
    {
      mc_cdma_standby_pref_chgd_cmd();
    }
#endif

#if defined(MMOC_CDMA_SUPPORTED) && defined(MMOC_USES_DB_SERVICES)
    if (mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY
#ifdef FEATURE_MMODE_TRIPLE_SIM
        || mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY
#endif
       )
    {
      db_item.dsds_status = TRUE;
    }
    else
    {
      db_item.dsds_status = FALSE;
    }
    mcc_db_put( DB_DSDS_STATUS, &db_item, mmoc_dev_info->sub_with_3gpp2 );
#endif


    mmoc_dev_info->prev_device_mode = mmoc_dev_info->device_mode;

    cmregprx_cmd_gw_ds_stat_chgd( mmoc_dev_info->standby_pref,
                                  mmoc_next_trans_id(mmoc_info_ptr),
                                  mmoc_dev_info->active_subs,
                                  mmoc_dev_info->device_mode);

    /* DS status chgd request sent
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DS_STAT_CHGD_CNF,
                             mmoc_info_ptr
                           );

    MMOC_MSG_HIGH_4("DS pref curr: %d, prev: %d Active subs curr: %d, prev: %d",
                    mmoc_dev_info->standby_pref,
                    mmoc_dev_info->prev_standby_pref,
                    mmoc_dev_info->active_subs,
                    mmoc_dev_info->prev_active_subs);

    return MMOC_EVT_STATUS_CONSUMED;

#endif

  }
  else if (mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_NO_STANDBY_PREF
           ||  mmoc_dev_info->active_subs == SYS_MODEM_AS_ID_NO_ACTIVE_MASK )
  {
    /* Set prev_ds_pref with new ds_pref value */
    mmoc_dev_info->prev_standby_pref = mmoc_dev_info->standby_pref;
    mmoc_dev_info->prev_active_subs  = mmoc_dev_info->active_subs;
    mmoc_dev_info->prev_device_mode  = mmoc_dev_info->device_mode;
  }

  if(mmoc_dev_info->trans == MMOC_TRANS_DUAL_STANDBY_CHGD)
  {
    /* Send the CM_STANDY_PREF_CHG_OK_F report to CM.
    */
    cm_mcfg_proceed_with_refresh_type    mcfg_refresh_proceed;
    mcfg_refresh_proceed.active_subs = mmoc_dev_info->active_subs;

    MMOC_MSG_HIGH_1("MCFG_Refresh: sending from cmn_ds_chgd enter active_subs %d",mcfg_refresh_proceed.active_subs);
    mmoc_send_cm_rpt( CM_MCFG_PROCEED_WITH_REFRESH_F, (cm_sd_rpt_u_type *)&mcfg_refresh_proceed,
                      SYS_MODEM_AS_ID_1);

    /* Clear the transaction.
    */
    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_PH_STAT_ENTER,
                           mmoc_info_ptr
                         );

  return MMOC_EVT_STATUS_NOT_CONSUMED;

}

/*===========================================================================

FUNCTION mmoc_process_subsc_chgd_subs_cap_chgd_enter

DESCRIPTION
  This is a utility function called to send dual standby stat chgd cmd to protocols,
  if dual standby status is changed

RETURN VALUE
  TRUE  - Phone status command was sent to support mode.
  FALSE - Completed sending the phone status command to all supported modes.

===========================================================================*/
static mmoc_evt_status_e_type  mmoc_process_subsc_chgd_subs_cap_chgd_enter
(
  mmoc_state_info_s_type             *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;

#ifdef FEATURE_MMODE_DUAL_SIM

  uint32 sub_caps[MAX_SIMS];

  for(sub = SYS_MODEM_AS_ID_1; sub < MAX_SIMS; sub++)
  {
    sub_caps[sub] = mmoc_info_ptr->sub_prop[sub]->subs_capability;
  }

  cmregprx_cmd_gw_subs_cap_chgd( sub_caps,
                                 mmoc_next_trans_id(mmoc_info_ptr));

#ifdef MMOC_CDMA_SUPPORTED
  /* Inform NV and 1x of the new 3GPP2 subscription */
  {
    nvruim_session_enum_type nv_session = (mmoc_info_ptr->dev_prop.sub_with_3gpp2 == SYS_MODEM_AS_ID_2)?NVRUIM_1X_PROV_SEC_SESSION:NVRUIM_1X_PROV_PRI_SESSION;
    nvruim_switch_1x_session(nv_session);
  }

  {
    sys_vfr_e_type vfr_map[MAX_AS_IDS];

    cm_get_vfr_map(&vfr_map[0]);

    if(BETWEEN(mmoc_info_ptr->dev_prop.sub_with_3gpp2, SYS_MODEM_AS_ID_NONE, MAX_AS_IDS))
    {
      mcc_set_asid_vfr_info(mmoc_info_ptr->dev_prop.sub_with_3gpp2,
                            vfr_map[mmoc_info_ptr->dev_prop.sub_with_3gpp2]);
    }
  }
#endif

  /* DS status chgd request sent
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_SUBS_CAP_CHGD_CNF,
                           mmoc_info_ptr
                         );

  return MMOC_EVT_STATUS_CONSUMED;

#endif

  mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_PH_STAT_ENTER,
                           mmoc_info_ptr
                         );

  return MMOC_EVT_STATUS_NOT_CONSUMED;

}



/*===========================================================================

FUNCTION mmoc_process_subsc_chgd_subs_cap_chgd_cnf

DESCRIPTION
 This function handles the common processing  in the WAIT_DEACTD_CNF
 transaction state.

  In this state, the state handler will
  1. Validate the deactivated confirmation report received
  2. Move the trans. state to send the phone status command.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_subsc_chgd_subs_cap_chgd_cnf
(
  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  MMOC_ASSERT( rpt_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the rpt ptr is NULL
  */
  if ( rpt_ptr == NULL )
  {
    /* Not the expected report.
    */
    MMOC_ERR_0("rpt NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Process the received report.
  */
  switch (rpt_ptr->rpt.name )
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      return  mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    case MMOC_RPT_SUBS_CAP_CHGD_CNF:

      if(mmoc_info_ptr->dev_prop.trans_id != rpt_ptr->param.subs_cap_chgd_cnf.trans_id)
      {
        /* Not the expected report. Go back to waiting for cnf
        */
        MMOC_ERR_0(" Unexpected event received ");
        return MMOC_EVT_STATUS_CONSUMED;
      }

      /* Move back to DS STAT CHGD ENTER state
      */
      mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_PH_STAT_ENTER,
                               mmoc_info_ptr
                             );


      return MMOC_EVT_STATUS_NOT_CONSUMED;


    default:
      MMOC_MSG_HIGH_1( "Unexpected report received for subs_cap_chgd_cnf %d",
                       rpt_ptr->rpt.name);
      return MMOC_EVT_STATUS_CONSUMED;


  } /* switch ( rpt_ptr->rpt.name ) */

}

/*===========================================================================

FUNCTION mmoc_process_subsc_chgd_sd_nam_sel_cnf

DESCRIPTION
 This function handles the common processing  in the WAIT_SD_NAM_SEL_CNF
 transaction state.

  In this state, the state handler will
  1. Validate the NAM_SEL_CNF rpt received
  2. Move the trans. state to send the phone status command.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_subsc_chgd_sd_nam_sel_cnf
(
  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  MMOC_ASSERT( rpt_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the rpt ptr is NULL
  */
  if ( rpt_ptr == NULL )
  {
    /* Not the expected report.
    */
    MMOC_ERR_0("rpt NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Process the received report.
  */
  switch (rpt_ptr->rpt.name )
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      return  mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    case MMOC_RPT_SD_NAM_SEL_CNF:

      if(mmoc_info_ptr->dev_prop.trans_id != rpt_ptr->param.sd_nam_sel_cnf.trans_id)
      {
        /* Not the expected report. Go back to waiting for cnf
        */
        MMOC_ERR_0(" Unexpected event received ");
        return MMOC_EVT_STATUS_CONSUMED;
      }

      {
        cm_subs_chg_done_type    sub_done_payload;
        sub_done_payload.active_stk_bm = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[rpt_ptr->param.sd_nam_sel_cnf.asubs_id]->active_stacks;
        sub_done_payload.hdr.asubs_id = rpt_ptr->param.sd_nam_sel_cnf.asubs_id;
        mmoc_send_cm_rpt( CM_SUBS_CHGD_DONE, (cm_sd_rpt_u_type *)&sub_done_payload,rpt_ptr->param.sd_nam_sel_cnf.asubs_id );
      }

      /* Move back to PH STAT CHGD ENTER state
           */
      mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_PH_STAT_ENTER,
                               mmoc_info_ptr
                             );


      return MMOC_EVT_STATUS_NOT_CONSUMED;


    default:
      MMOC_MSG_HIGH_1( "Unexpected report received for sd_nam_sel_cnf %d",
                       rpt_ptr->rpt.name);
      return MMOC_EVT_STATUS_CONSUMED;


  } /* switch ( rpt_ptr->rpt.name ) */

}




/*===========================================================================

FUNCTION mmoc_process_subsc_chgd

DESCRIPTION
  This function is the transaction handler for the subscription changed
  transaction.

  The transaction state transition will be,

 NULL->DEACT_ENTER->WAIT_DEACTD_CNF->MMGSDI_READ_ENTER->MMGSDI_READ_CNF->PH_STAT_ENTER<->WAIT_PH_STAT_CNF
              |             |               ^                                 ^   |
              |_____________|_______________|                                 |   |
              |             |                                                 |   |
              |_____________|_________________________________________________|   |______>NULL

DEPENDENCIES
  None.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

SIDE EFFECTS
  None.
===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_subsc_chgd
(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_evt_status_e_type evt_status = MMOC_EVT_STATUS_CONSUMED;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Invoke the appropriate transaction state handlers.
  */
  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status = mmoc_process_subsc_chgd_null( (mmoc_cmd_msg_s_type *)cmd_ptr,
                   mmoc_info_ptr
                                               );
      break;

    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
      evt_status =  mmoc_process_subsc_chgd_deact_enter( PROT_DEACT_SUBSC_CHGD,
                    mmoc_info_ptr
                                                       );
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
      evt_status = mmoc_process_common_deactd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                 );
      break;

    case MMOC_TRANS_STATE_MMGSDI_READ_ENTER:
      evt_status = mmoc_process_subsc_chgd_mmgsdi_read_enter( rpt_ptr,
                   mmoc_info_ptr
                                                            );
      break;

    case MMOC_TRANS_STATE_MMGSDI_READ_CNF:
      evt_status = mmoc_process_subsc_chgd_mmgsdi_read_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                          );
      break;

    case MMOC_TRANS_STATE_PROT_PH_STAT_ENTER:
      evt_status = mmoc_process_subsc_chgd_ph_stat_enter( cmd_ptr,
                   mmoc_info_ptr
                                                        );
      break;

    case MMOC_TRANS_STATE_WAIT_PH_STAT_CNF:
      evt_status = mmoc_process_common_ph_stat_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                  );
      break;

    case MMOC_TRANS_STATE_HDR_DEACT_ENTER:
      evt_status = mmoc_process_common_hdr_deact_enter
                   ( PROT_DEACT_ACQ_OTHERS,
                     mmoc_info_ptr
                   );
      break;



    case MMOC_TRANS_STATE_WAIT_HDR_DEACTD_CNF:
      evt_status = mmoc_process_common_hdr_deactd_cnf ( rpt_ptr,
                   mmoc_info_ptr
                                                      );
      break;

    case MMOC_TRANS_STATE_HYBR2_DEACT_ENTER:
      evt_status = mmoc_process_common_hybr2_deact_enter
                   ( PROT_DEACT_ACQ_OTHERS,
                     mmoc_info_ptr
                   );
      break;


    case MMOC_TRANS_STATE_WAIT_HYBR2_DEACTD_CNF:
      evt_status = mmoc_process_common_hybr2_deactd_cnf ( rpt_ptr,
                   mmoc_info_ptr
                                                        );

    case MMOC_TRANS_STATE_DS_STAT_CHGD_ENTER:
      evt_status = mmoc_process_common_ds_stat_chgd_enter(cmd_ptr,
                   mmoc_info_ptr);
      break;

    case MMOC_TRANS_STATE_WAIT_DS_STAT_CHGD_CNF:
      evt_status = mmoc_process_common_ds_stat_chgd_cnf(rpt_ptr,
                   mmoc_info_ptr
                                                       );
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL:
      evt_status = mmoc_process_deactd_cnf_gwl( rpt_ptr,
                   mmoc_info_ptr
                                              );
      break;

    case MMOC_TRANS_STATE_WAIT_SUBS_CAP_CHGD_ENTER:
      evt_status = mmoc_process_subsc_chgd_subs_cap_chgd_enter( mmoc_info_ptr );
      break;

    case MMOC_TRANS_STATE_WAIT_SUBS_CAP_CHGD_CNF:
      evt_status = mmoc_process_subsc_chgd_subs_cap_chgd_cnf( rpt_ptr,
                   mmoc_info_ptr );
      break;

    case MMOC_TRANS_STATE_WAIT_SD_NAM_SEL_CNF:
      evt_status = mmoc_process_subsc_chgd_sd_nam_sel_cnf( rpt_ptr,
                   mmoc_info_ptr);
      break;

    case MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF:
    case MMOC_TRANS_STATE_MAX:
    default:
      MMOC_ERR_1("trans_state", mmoc_info_ptr->dev_prop.trans_state);
      evt_status = mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
      break;
  }


  return evt_status;

} /* mmoc_process_subsc_chgd() */



/* <EJECT> */
/*===========================================================================
=============================================================================
=============================================================================
==================== ONLINE COMMAND TRANSACTION =============================
=============================================================================
=============================================================================
===========================================================================*/


/*===========================================================================

FUNCTION mmoc_process_online_null

DESCRIPTION
  This function handles the online transaction in the NULL transaction state.

  In this state, the state handler will
  1. Call the power up SD indication if SD was initialized earlier and
     activate the protocol was indicated by SD action.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_online_null
(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;

  MMOC_ASSERT( cmd_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* The online operating mode changed command from CM should only create this
  ** transaction.
  */
  if ( cmd_ptr == NULL ||
       cmd_ptr->cmd.name != MMOC_CMD_OPRT_MODE_CHGD  ||
       cmd_ptr->param.oprt_mode_chgd.oprt_mode != SYS_OPRT_MODE_ONLINE
     )
  {
    MMOC_ERR_0("Online trans incorrectly initiated");

    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Should be in power off or online mode or LPM currently.
  */
  if ( mmoc_dev_info->oprt_mode != PROT_OPRT_MODE_NULL   &&
       mmoc_dev_info->oprt_mode != PROT_OPRT_MODE_ONLINE &&
       mmoc_dev_info->oprt_mode != PROT_OPRT_MODE_LPM &&
       mmoc_dev_info->oprt_mode != PROT_OPRT_MODE_FTM
     )
  {
    MMOC_ERR_1( "Incorrect current oprt mode %d",
                mmoc_dev_info->oprt_mode);
    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize the operating mode info.
  */
  mmoc_dev_info->oprt_mode    = PROT_OPRT_MODE_ONLINE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Deactivate any protocols that are active.
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_DEACT_ENTER,
                           mmoc_info_ptr
                         );

  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_online_null() */



/*===========================================================================

FUNCTION mmoc_process_online_ph_stat_enter

DESCRIPTION
  This function handles the offline transaction in the PH_STAT_ENTER
  transaction state.

  In this transaction state, the handler will
  1. Send the Phone status changed command to the next supported protocol
     and move to WAIT_PH_STAT_CNF.
     If the phone status changed command was sent to all the protocols,
     then SD is initialized with new subscription and activate the protocol
     if the operating mode is online.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type       mmoc_process_online_ph_stat_enter(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  prot_ph_stat_chg_e_type chg_type = PROT_PH_STAT_CHG_OPRT_MODE;
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stack = MM_STACK_NONE;
  sd_ss_act_e_type        sd_act = SD_SS_ACT_CONTINUE;

#if defined(FEATURE_MMODE_DUAL_SIM)
  sys_modem_as_id_e_type  sub_on_hold = SYS_MODEM_AS_ID_NONE;
  boolean is_priority_sub_activated = FALSE;
  sys_modem_as_id_e_type is_sub_on_hold_sleep = FALSE;
#endif

  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the phone status changed command to the next supported
  ** protocol.
  */
  if(mmoc_send_ph_stat_chgd_cmd(chg_type,
                                mmoc_info_ptr
                               )
    )
  {
    /* Phone status changed command sent, so wait for ack.
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_PH_STAT_CNF,
                             mmoc_info_ptr
                           );
    return MMOC_EVT_STATUS_CONSUMED;
  }

#ifdef FEATURE_MMODE_DUAL_SIM
  sub_on_hold = mmoc_calculate_sub_on_hold(mmoc_info_ptr);
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if SD was initialized earlier.
  */
  if(mmoc_dev_info->is_sd_initialized)
  {
    TTLBOOTUP(MMOC_ONLINE);
    mmoc_print_pwrup_parameters(mmoc_info_ptr);

#ifdef FEATURE_MMODE_DUAL_SIM
    if(sub_on_hold != SYS_MODEM_AS_ID_NONE)
    {
      for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
      {
        if(sub_on_hold != sub &&
            mmoc_dev_info->active_subs & BM(sub)
          )
        {
          sd_act = sd_ss_ind_user_pwr_up(TRUE, sub, NULL);
          mmoc_activate_prot_online_ss_main(sd_act, sub, mmoc_info_ptr);
          is_priority_sub_activated = TRUE;
        }
        else if(sub_on_hold == sub &&
                mmoc_dev_info->active_subs & BM(sub) &&
                cm_get_device_mode() != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE &&
                mmoc_dev_info->standby_pref != SYS_MODEM_DS_PREF_SINGLE_STANDBY &&
                is_priority_sub_activated)
        {
          boolean delay_srv_req = FALSE;
          
          is_sub_on_hold_sleep = TRUE;

          if(sd_act == SD_SS_ACT_ACQ_CDMA
              || sd_act == SD_SS_ACT_ACQ_GWL
              || sd_act == SD_SS_ACT_ACQ_HDR)
          {
            mmoc_hold_acq(sub_on_hold, mmoc_info_ptr);

            /* need to delay srv req for non dds sub (sub_on_hold) for srlte+x config */
            delay_srv_req = mmoc_delay_srv_req(sub_on_hold, mmoc_info_ptr);
          }

          sd_act = sd_ss_ind_user_pwr_up(TRUE, sub, NULL);
          mmoc_activate_prot_online_ss_main2(sd_act, sub_on_hold, mmoc_info_ptr,
                                               delay_srv_req);
        }
      }

      if(!is_sub_on_hold_sleep)
      {
        boolean delay_srv_req = FALSE;
        
        is_sub_on_hold_sleep = TRUE;
        if(sd_act == SD_SS_ACT_ACQ_CDMA
            || sd_act == SD_SS_ACT_ACQ_GWL
            || sd_act == SD_SS_ACT_ACQ_HDR)
        {
          mmoc_hold_acq(sub_on_hold, mmoc_info_ptr);

          /* need to delay srv req for non dds sub (sub_on_hold) for srlte+x config */
          delay_srv_req = mmoc_delay_srv_req(sub_on_hold, mmoc_info_ptr);
        }

        sd_act = sd_ss_ind_user_pwr_up(TRUE, sub_on_hold, NULL);
        mmoc_activate_prot_online_ss_main2(sd_act, sub_on_hold, mmoc_info_ptr,
                                            delay_srv_req);
      }
    }
    else
    {
      for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
      {
        if(mmoc_dev_info->active_subs & BM(sub))
        {
          sd_act = sd_ss_ind_user_pwr_up(TRUE, sub, NULL);
          if(!mmoc_skip_online_activation(mmoc_info_ptr))
          {
          mmoc_activate_prot_online_ss_main(sd_act, sub, mmoc_info_ptr);
        }
      }
    }
    }

#else //SSIM

    if(mmoc_dev_info->active_subs & BM(SYS_MODEM_AS_ID_1))
    {
      sd_act = sd_ss_ind_user_pwr_up(TRUE, SYS_MODEM_AS_ID_1, NULL);

      if(!mmoc_skip_online_activation(mmoc_info_ptr))
      {
      mmoc_activate_prot_online_ss_main(sd_act, SYS_MODEM_AS_ID_1, mmoc_info_ptr);
    }
    }
    #endif

    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = mmoc_dev_info->sub_with_3gpp2;

      if(mmoc_is_1x_sxlte(mmoc_dev_info->sub_with_3gpp2) &&
          (mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->active_stacks & MM_STACK_2_MASK))
      {
        mm_id.stk_id = MM_STACK_2;
        sd_misc_oprt_mode_online(mm_id);
      }

#ifdef FEATURE_HDR_HYBRID
      if(mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->active_stacks & MM_STACK_1_MASK)
      {
        mm_id.stk_id = MM_STACK_1;
        sd_misc_oprt_mode_online(mm_id);
      }
#endif
    }

  } /*  if ( mmoc_info_ptr->is_sd_initialized ) */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Clear the transaction.
  */
  return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );

} /* mmoc_process_online_ph_stat_enter() */


/*===========================================================================

FUNCTION mmoc_process_online

DESCRIPTION
  This function is the transaction handler for the online transaction.

  The transaction state transition will be,

     NULL -> NULL

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_online
(

  const mmoc_cmd_msg_s_type      *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_evt_status_e_type  evt_status = MMOC_EVT_STATUS_CONSUMED;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Invoke the appropriate transaction state handlers.
  */
  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status = mmoc_process_online_null( cmd_ptr,
                                             rpt_ptr,
                                             mmoc_info_ptr
                                           );
      break;

    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
      evt_status = mmoc_process_common_deact_enter( PROT_DEACT_SUBSC_CHGD,
                   mmoc_info_ptr
                                                  );
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
      evt_status = mmoc_process_common_deactd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                 );
      break;

    case MMOC_TRANS_STATE_PROT_PH_STAT_ENTER:
      evt_status = mmoc_process_online_ph_stat_enter( cmd_ptr,
                   rpt_ptr,
                   mmoc_info_ptr
                                                    );
      break;

    case MMOC_TRANS_STATE_WAIT_PH_STAT_CNF:
      evt_status = mmoc_process_common_ph_stat_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                  );
      break;


    case MMOC_TRANS_STATE_HDR_DEACT_ENTER:
      evt_status = mmoc_process_common_hdr_deact_enter
                   ( PROT_DEACT_ACQ_OTHERS,
                     mmoc_info_ptr
                   );
      break;



    case MMOC_TRANS_STATE_WAIT_HDR_DEACTD_CNF:
      evt_status = mmoc_process_common_hdr_deactd_cnf ( rpt_ptr,
                   mmoc_info_ptr
                                                      );
      break;

    case MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF:
    case MMOC_TRANS_STATE_GEN_CMD_ENTER:
    case MMOC_TRANS_STATE_MAX:
    default:
      MMOC_ERR_1("error Trans_state %d", mmoc_info_ptr->dev_prop.trans_state);
      evt_status = mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
      break;
  }


  return evt_status;

} /* mmoc_process_online() */




/*===========================================================================
=============================================================================
=============================================================================
==================== OFFLINE COMMAND TRANSACTION ============================
=============================================================================
=============================================================================
===========================================================================*/

/*===========================================================================

FUNCTION mmoc_process_offline_null

DESCRIPTION
  This function handles the offline transaction in the NULL transaction
  state.

  In this state, the state handler will
  1. Verify that offline oprt mode command invoked this transaction.
  2. Initialize the current operating mode.
  3. Move the Deact. Enter state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_offline_null
(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  MMOC_ASSERT( cmd_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* The offline operating mode changed command from CM should only create
  ** this transaction.
  */
  if(cmd_ptr == NULL ||
      cmd_ptr->cmd.name != MMOC_CMD_OPRT_MODE_CHGD ||
      (cmd_ptr->param.oprt_mode_chgd.oprt_mode != SYS_OPRT_MODE_OFFLINE &&
       cmd_ptr->param.oprt_mode_chgd.oprt_mode != SYS_OPRT_MODE_OFFLINE_CDMA &&
       cmd_ptr->param.oprt_mode_chgd.oprt_mode != SYS_OPRT_MODE_FTM)
    )
  {
    MMOC_ERR_0("error cmd");

    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize the current operating mode and phone status sent index.
  */
  mmoc_info_ptr->dev_prop.oprt_mode = mmoc_map_sys_oprt_mode_to_mmoc
                                      (
                                        cmd_ptr->param.oprt_mode_chgd.oprt_mode
                                      );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Move to DEACT_ENTER trans. state.
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_DEACT_ENTER,
                           mmoc_info_ptr
                         );

  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_offline_null() */


/*===========================================================================

FUNCTION mmoc_process_offline_deact_enter

DESCRIPTION
  This function is responsible for the sending the deactivate protocol
  request to the active protocol if present. Else, moves the transaction
  to the state which sends the Phone status change command.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_offline_deact_enter
(

  const mmoc_cmd_msg_s_type      *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  prot_deact_e_type  deact_reason = PROT_DEACT_MAX;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the deactivate request to active protocol.
  */
  switch ( mmoc_info_ptr->dev_prop.oprt_mode)
  {
    case PROT_OPRT_MODE_OFFLINE:
    case PROT_OPRT_MODE_OFFLINE_CDMA:
      deact_reason = PROT_DEACT_OFFLINE;
      break;

    case PROT_OPRT_MODE_FTM:
      deact_reason = PROT_DEACT_FTM;
      break;

    case PROT_OPRT_MODE_NULL:
    case PROT_OPRT_MODE_PWROFF:
    case PROT_OPRT_MODE_ONLINE:
    case PROT_OPRT_MODE_LPM:
    case PROT_OPRT_MODE_RESET:
    case PROT_OPRT_MODE_MAX:
    default:
      sys_err_fatal_invalid_value_exception((int)mmoc_info_ptr->dev_prop.oprt_mode);
      break;

  } /* switch( mmoc_info_ptr->curr_oprt_mode) */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Perform the common deactivated enter state handling.
  */
  return mmoc_process_common_deact_enter( deact_reason,
                                          mmoc_info_ptr
                                        );

} /* mmoc_process_offline_deact_enter() */



/*===========================================================================

FUNCTION mmoc_process_offline_ph_stat_enter

DESCRIPTION
  This function handles the offline transaction in the  PH_STAT_ENTER
  transaction state.

  In this transaction state, the handler will
  1. Send the Phone status changed command to the next supported protocol
     and move to WAIT_PH_STAT_CNF.
     If the phone status changed command was sent to all the protocols,
     SD is informed of the offline oprt mode and activate the protocol
     in offline mode if needed.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type       mmoc_process_offline_ph_stat_enter(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the phone status changed command to the next supported
  ** protocol.
  */
  if(mmoc_send_ph_stat_chgd_cmd(PROT_PH_STAT_CHG_OPRT_MODE,
                                mmoc_info_ptr
                               )
    )
  {
    /* Phone status changed command sent, so wait for ack.
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_PH_STAT_CNF,
                             mmoc_info_ptr
                           );
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If SD was previously initialized, inform SD of offline mode.
  */
  if(mmoc_info_ptr->dev_prop.is_sd_initialized)
  {
    /* Done with sending the Phone status changed commands, Call SD
    ** indication to inform of offline mode.
    */
    switch(mmoc_info_ptr->dev_prop.oprt_mode)
    {
      case PROT_OPRT_MODE_OFFLINE:
      case PROT_OPRT_MODE_OFFLINE_CDMA:
        (void) sd_ss_ind_user_offline_cdma(mmoc_info_ptr->dev_prop.sub_with_3gpp2, NULL);
        break;

      case PROT_OPRT_MODE_FTM:
        (void) sd_ss_ind_user_offline_cdma(mmoc_info_ptr->dev_prop.sub_with_3gpp2, NULL);
        break;

      case PROT_OPRT_MODE_NULL:
      case PROT_OPRT_MODE_PWROFF:
      case PROT_OPRT_MODE_ONLINE:
      case PROT_OPRT_MODE_LPM:
      case PROT_OPRT_MODE_RESET:
      case PROT_OPRT_MODE_MAX:
      default:
        sys_err_fatal_invalid_value_exception((int)mmoc_info_ptr->dev_prop.oprt_mode);
        break;

    } /* switch (mmoc_info_ptr->curr_oprt_mode) */

  } /* if ( mmoc_info_ptr->is_sd_initialized ) */


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Activate the protocol, if needed in the offline mode */
  mmoc_activate_prot_offline( mmoc_info_ptr );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mmoc_send_cm_rpt( CM_OFFLINE_OK_F, NULL, SYS_MODEM_AS_ID_1);
  mmoc_notify_offline_mode();

  MMOC_MSG_HIGH_0("OFFLINE processing complete");

  /* Clear the transaction.
  */
  return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );

} /* mmoc_process_offline_ph_stat_enter() */



/*===========================================================================

FUNCTION mmoc_process_offline

DESCRIPTION
  This function is the transaction handler for the offline transaction.

  The transaction state transition will be,

 NULL -> DEACT_ENTER -> WAIT_DEACTD_CNF -> PH_STAT_ENTER <-> WAIT_PH_STAT_CNF
              |                              ^    |
              |______________________________|    |-----> NULL

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_offline
(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_evt_status_e_type  evt_status = MMOC_EVT_STATUS_CONSUMED;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Invoke the appropriate transaction state handler.
  */
  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status =  mmoc_process_offline_null( cmd_ptr,
                    rpt_ptr,
                    mmoc_info_ptr
                                             );
      break;

    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
      evt_status =  mmoc_process_offline_deact_enter( cmd_ptr,
                    rpt_ptr,
                    mmoc_info_ptr
                                                    );
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
      evt_status =  mmoc_process_common_deactd_cnf( rpt_ptr,
                    mmoc_info_ptr
                                                  );
      break;

    case MMOC_TRANS_STATE_PROT_PH_STAT_ENTER:
      evt_status = mmoc_process_offline_ph_stat_enter( cmd_ptr,
                   rpt_ptr,
                   mmoc_info_ptr
                                                     );
      break;


    case MMOC_TRANS_STATE_WAIT_PH_STAT_CNF:
      evt_status = mmoc_process_common_ph_stat_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                  );
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL:
      evt_status =  mmoc_process_deactd_cnf_gwl( rpt_ptr,
                    mmoc_info_ptr
                                               );
      break;

    case MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF:
    case MMOC_TRANS_STATE_GEN_CMD_ENTER:
    case MMOC_TRANS_STATE_WAIT_AUTO_DEACTD_IND:
    case MMOC_TRANS_STATE_HDR_DEACT_ENTER:
    case MMOC_TRANS_STATE_WAIT_HDR_DEACTD_CNF:
    case MMOC_TRANS_STATE_MAX:
    default:
      MMOC_ERR_0("Received event in unknown state");
      evt_status = mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
      break;
  }

  return evt_status;

} /* mmoc_process_offline() */


/* <EJECT> */
/*===========================================================================
=============================================================================
=============================================================================
==================== POWER DOWN/RESET/LPM COMMAND TRANSACTION ===============
=============================================================================
=============================================================================
===========================================================================*/

/*===========================================================================

FUNCTION mmoc_process_pwr_down_null

DESCRIPTION
  This function handles the power down/reset/lpm transaction in the NULL
  transaction state.

  In this state, the state handler will
  1. Verify that offline oprt mode command invoked this transaction.
  2. Initialize the current operating mode.
  3. Move the Deact. Enter state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_pwr_down_null
(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  MMOC_ASSERT( cmd_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* The pwroff/reset/lpm operating mode changed command from CM should only
  ** create this transaction.
  */
  if ( cmd_ptr == NULL  ||
       cmd_ptr->cmd.name != MMOC_CMD_OPRT_MODE_CHGD ||
       ( cmd_ptr->param.oprt_mode_chgd.oprt_mode != SYS_OPRT_MODE_PWROFF       &&
         cmd_ptr->param.oprt_mode_chgd.oprt_mode != SYS_OPRT_MODE_RESET        &&
         cmd_ptr->param.oprt_mode_chgd.oprt_mode != SYS_OPRT_MODE_RESET_MODEM  &&
         cmd_ptr->param.oprt_mode_chgd.oprt_mode != SYS_OPRT_MODE_LPM
       )
     )
  {
    MMOC_ERR_0("Powerdown/Reset/LPM command transaction incorrectly initiated");
    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize the current operating mode and phone status sent index.
  */
  mmoc_info_ptr->dev_prop.oprt_mode = mmoc_map_sys_oprt_mode_to_mmoc
                                      (
                                        cmd_ptr->param.oprt_mode_chgd.oprt_mode
                                      );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Move to DEACT_ENTER trans. state.
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_DEACT_ENTER,
                           mmoc_info_ptr
                         );


  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_pwr_down_null() */



/*===========================================================================

FUNCTION mmoc_process_pwr_down_deact_enter

DESCRIPTION
  This function is responsible for the sending the deactivate protocol
  request to the active protocol if present. Else, moves the transaction
  to the state which sends the Phone status change command.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_pwr_down_deact_enter
(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  prot_deact_e_type  deact_reason = PROT_DEACT_MAX;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the deactivate request to active protocol.
  */
  switch(mmoc_info_ptr->dev_prop.oprt_mode)
  {
    case PROT_OPRT_MODE_PWROFF:
      deact_reason = PROT_DEACT_PWR_DOWN;
      break;

    case PROT_OPRT_MODE_RESET:
      deact_reason = PROT_DEACT_RESET;
      break;

    case PROT_OPRT_MODE_LPM:
      deact_reason = PROT_DEACT_LPM;
      break;

    case PROT_OPRT_MODE_NULL:
    case PROT_OPRT_MODE_OFFLINE:
    case PROT_OPRT_MODE_OFFLINE_CDMA:
    case PROT_OPRT_MODE_ONLINE:
    case PROT_OPRT_MODE_FTM:
    case PROT_OPRT_MODE_MAX:
    default:
      sys_err_fatal_invalid_value_exception((int)mmoc_info_ptr->dev_prop.oprt_mode);
      break;

  } /* switch( mmoc_info_ptr->curr_oprt_mode) */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Inform the system event reporting service.
  */
  event_report( EVENT_POWER_DOWN );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Perform the common deactivated enter state handling.
  */
  return mmoc_process_common_deact_enter( deact_reason,
                                          mmoc_info_ptr
                                        );

} /* mmoc_process_pwr_down_deact_enter() */



/*===========================================================================

FUNCTION mmoc_process_pwr_down_ph_stat_enter

DESCRIPTION
  This function handles the offline transaction in the  PH_STAT_ENTER
  transaction state.

  In this transaction state, the handler will
  1. Send the Phone status changed command to the next supported protocol
     and move to WAIT_PH_STAT_CNF.
     If the phone status changed command was sent to all the protocols,
     Inform the SD with the  reset/lpm/powerdown indication  and
     Shutdown the SD if it is LPM or power down.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type       mmoc_process_pwr_down_ph_stat_enter(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mm_sub_stk_id_s_type mm_id;
  mm_id.asubs_id = SYS_MODEM_AS_ID_1;
  mm_id.stk_id = MM_STACK_0;

  /* Send the phone status changed command to the next supported
  ** protocol.
  */
  if ( mmoc_send_ph_stat_chgd_cmd( PROT_PH_STAT_CHG_OPRT_MODE,
                                   mmoc_info_ptr
                                 )
     )
  {
    /* Phone status changed command sent, so wait for ack.
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_PH_STAT_CNF,
                             mmoc_info_ptr
                           );
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Call utility api to reset all active timers for the device
  */
  mmoc_reset_timers( mmoc_info_ptr);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Done with sending the Phone status changed commands, Inform SD of the
  ** operating mode change.
  */
  switch ( mmoc_info_ptr->dev_prop.oprt_mode)
  {
    case PROT_OPRT_MODE_PWROFF:

      /* Inform system report service
      */
      event_report( EVENT_POWER_DOWN );
      mmoc_update_prot_state( PROT_STATE_PWR_DOWN,
                              mm_id,
                              mmoc_info_ptr
                            );
      mmoc_update_db_dmss_state( mmoc_info_ptr );
      /* Send thePWR OFF OK report to CM.
      */
      mmoc_send_cm_rpt( CM_PWROFF_OK_F, NULL, SYS_MODEM_AS_ID_1);
      break;

    case PROT_OPRT_MODE_LPM:

      /* Inform system report service
      */
      event_report( EVENT_POWER_DOWN );
      mmoc_update_prot_state( PROT_STATE_LPM,
                              mm_id,
                              mmoc_info_ptr
                            );
      mmoc_update_db_dmss_state( mmoc_info_ptr );

      /* Send the LPM OK report to CM.
      */
      mmoc_send_cm_rpt( CM_LPM_OK_F, NULL, SYS_MODEM_AS_ID_1);
      break;

    case PROT_OPRT_MODE_RESET:

      mmoc_update_prot_state( PROT_STATE_RESET,
                              mm_id,
                              mmoc_info_ptr
                            );
      mmoc_update_db_dmss_state( mmoc_info_ptr );

      /* Send the RESET OK report to CM.
      */
      mmoc_send_cm_rpt( CM_RESET_OK_F, NULL, SYS_MODEM_AS_ID_1 );
      break;

    case PROT_OPRT_MODE_NULL:
    case PROT_OPRT_MODE_OFFLINE:
    case PROT_OPRT_MODE_OFFLINE_CDMA:
    case PROT_OPRT_MODE_ONLINE:
    case PROT_OPRT_MODE_FTM:
    case PROT_OPRT_MODE_MAX:
    default:
      sys_err_fatal_invalid_value_exception((int)mmoc_info_ptr->dev_prop.oprt_mode);
      break;

  } /* switch( mmoc_info_ptr->curr_oprt_mode) */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Inform and terminate System Determination.
  */
  if ( mmoc_info_ptr->dev_prop.is_sd_initialized &&
       mmoc_info_ptr->dev_prop.oprt_mode !=  PROT_OPRT_MODE_RESET )
  {
    mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;

    (void)sd_ss_ind_user_pwr_down( mm_id, NULL );
    (void)sd_ss_ind_term( mm_id, NULL );
    mm_id.stk_id = MM_STACK_2;
    sd_misc_oprt_mode_pwr_down(mm_id);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Clear the transaction.
  */
  return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );

} /* mmoc_process_pwr_down_ph_stat_enter() */



/*===========================================================================

FUNCTION mmoc_process_pwr_down

DESCRIPTION
  This function is the transaction handler for the power down transaction.

  The transaction state transition will be,

 NULL -> DEACT_ENTER -> WAIT_DEACTD_CNF -> PH_STAT_ENTER <-> WAIT_PH_STAT_CNF
              |                              ^    |
              |______________________________|    |-----> NULL

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_pwr_down
(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_evt_status_e_type  evt_status = MMOC_EVT_STATUS_CONSUMED;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Invoke the appropriate transaction state handler.
  */
  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status = mmoc_process_pwr_down_null( cmd_ptr,
                   rpt_ptr,
                   mmoc_info_ptr
                                             );
      break;

    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
      evt_status =  mmoc_process_pwr_down_deact_enter( cmd_ptr,
                    rpt_ptr,
                    mmoc_info_ptr
                                                     );
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
      evt_status = mmoc_process_common_deactd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                 );
      break;

    case MMOC_TRANS_STATE_PROT_PH_STAT_ENTER:
      evt_status = mmoc_process_pwr_down_ph_stat_enter( cmd_ptr,
                   rpt_ptr,
                   mmoc_info_ptr
                                                      );
      break;

    case MMOC_TRANS_STATE_WAIT_PH_STAT_CNF:
      evt_status = mmoc_process_common_ph_stat_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                  );
      break;


    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL:
      evt_status = mmoc_process_deactd_cnf_gwl( rpt_ptr,
                   mmoc_info_ptr
                                              );
      break;

    case MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF:
    case MMOC_TRANS_STATE_GEN_CMD_ENTER:
    case MMOC_TRANS_STATE_WAIT_AUTO_DEACTD_IND:
    case MMOC_TRANS_STATE_HDR_DEACT_ENTER:
    case MMOC_TRANS_STATE_WAIT_HDR_DEACTD_CNF:
    case MMOC_TRANS_STATE_MAX:
    default:
      MMOC_ERR_1("incorrect state", mmoc_info_ptr->dev_prop.trans_state);
      evt_status = mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
      break;
  }

  return evt_status;

} /* mmoc_process_pwr_down() */

/* <EJECT> */
/*===========================================================================
=============================================================================
=============================================================================
==================== GENERIC PROTOCOL COMMAND TRANSACTION ===================
=============================================================================
=============================================================================
===========================================================================*/

/*===========================================================================

FUNCTION mmoc_proc_addl_action_ue_mode_switch

DESCRIPTION
  This function is responsible for the sending the UE mode switch indication
  to CMREGPRX.

RETURN VALUE
  TRUE  - UE_MODE_SWITCH sent to REGPRX
  FALSE - UE_MODE_SWITCH not sent to REGPRX.

===========================================================================*/
static boolean                         mmoc_proc_addl_action_ue_mode_switch
(
  prot_gen_cmd_s_type             *gen_cmd_ptr,
  /* Pointer to the command event that was received.
  */

  mmoc_state_info_s_type          *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
#ifdef MMOC_GW_SUPPORTED

  mm_sub_stk_id_s_type mm_id;

  if ( gen_cmd_ptr == NULL || mmoc_info_ptr == NULL )
  {
    return FALSE;
  }

  if(mmoc_is_1x_sxlte(gen_cmd_ptr->asubs_id) &&
      gen_cmd_ptr->cmd_type      == PROT_GEN_CMD_PREF_SYS_CHGD               &&
      gen_cmd_ptr->param.pref_sys_chgd.addl_action.action & MMOC_PREF_SYS_ADDL_ACTION_UE_MODE_SWITCH &&
      (mmoc_info_ptr->dev_prop.oprt_mode != PROT_OPRT_MODE_OFFLINE_CDMA &&
       mmoc_info_ptr->dev_prop.oprt_mode != PROT_OPRT_MODE_OFFLINE))
  {
    mm_id.asubs_id = gen_cmd_ptr->asubs_id;
    mm_id.stk_id   = gen_cmd_ptr->stk_id;
    if(mm_id.asubs_id < SYS_MODEM_AS_ID_1 || mm_id.asubs_id >= MAX_SIMS ||
        mm_id.stk_id < MM_STACK_0 || mm_id.stk_id >= MAX_STACKS ||
        mmoc_info_ptr->sub_prop[mm_id.asubs_id] == NULL ||
        mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id] == NULL)
    {
      return FALSE;
    }

    /* Inform Ue mode change to 1X, if the SUb is capable of 1X */
    if (mmoc_info_ptr->dev_prop.sub_with_3gpp2 == mm_id.asubs_id)
    {
#ifdef MMOC_CDMA_SUPPORTED
      if (mmoc_info_ptr->sub_prop[mm_id.asubs_id]->ue_mode_pend_flag & BM(MM_STACK_0))
      {
        mc_cdma_ue_mode_chgd_cmd(gen_cmd_ptr->asubs_id,
                                 mmoc_next_trans_id(mmoc_info_ptr),
                                 gen_cmd_ptr->param.pref_sys_chgd.addl_action.ue_mode,
                                 gen_cmd_ptr->param.pref_sys_chgd.addl_action.is_ue_mode_substate_srlte);

        mmoc_info_ptr->sub_prop[mm_id.asubs_id]->ue_mode_pend_flag &= ~(BM(MM_STACK_0));

        mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_UE_MODE_SWITCH_CNF_1X,
                                 mmoc_info_ptr
                               );
        return TRUE;
      }
#endif

#ifdef MMOC_HDR_SUPPORTED
      if (mmoc_info_ptr->sub_prop[mm_id.asubs_id]->ue_mode_pend_flag & BM(MM_STACK_1))
      {
        mmoc_send_hdr_ue_mode_chg_req(mmoc_info_ptr);

        mmoc_info_ptr->sub_prop[mm_id.asubs_id]->ue_mode_pend_flag &= ~(BM(MM_STACK_1));

        mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_UE_MODE_SWITCH_CNF_HDR,
                                 mmoc_info_ptr
                               );
        return TRUE;
      }
#endif
    }

    /* store is_ue_mode_substate_srlte */
    mmoc_info_ptr->sub_prop[mm_id.asubs_id]->is_srlte 
      = gen_cmd_ptr->param.pref_sys_chgd.addl_action.is_ue_mode_substate_srlte;

    cmregprx_cmd_ue_mode_switch( mm_id,
                                 mmoc_next_trans_id(mmoc_info_ptr),
                                 gen_cmd_ptr->param.pref_sys_chgd.addl_action.ue_mode,
                                 gen_cmd_ptr->param.pref_sys_chgd.addl_action.is_ue_mode_substate_srlte,
                                 gen_cmd_ptr->param.pref_sys_chgd.addl_action.is_tau_reqd);


    gen_cmd_ptr->param.pref_sys_chgd.addl_action.action &= ~(MMOC_PREF_SYS_ADDL_ACTION_UE_MODE_SWITCH);

    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_UE_MODE_SWITCH,
                             mmoc_info_ptr
                           );
    return TRUE;

  }
#endif
  return FALSE;

}

/*===========================================================================

FUNCTION mmoc_process_prot_gen_cmd_null

DESCRIPTION
  This function handles the generic protocol command transaction in the NULL
  transaction state.

  In this state, the state handler will
  1. Send the generic command to  the active protocol if one is present.
     Else, if no protocol is active will call the mmoc_proc_prot_gen_cmd()
     and follow the return SD action.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_gen_cmd_null
(

  mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type           asubs_id;
  multimode_stack_e_type           stk_id;

  MMOC_ASSERT( cmd_ptr != NULL );

  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* The generic protocol command should only create this transaction.
  */
  if ( cmd_ptr == NULL ||
       cmd_ptr->cmd.name != MMOC_CMD_PROT_GEN_CMD
     )
  {
    MMOC_ERR_0("incorrect cmd");

    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }
  asubs_id = cmd_ptr->param.gen_prot_cmd.asubs_id;
  stk_id = cmd_ptr->param.gen_prot_cmd.stk_id;

  if(!MMOC_ENUM_IS_INRANGE(asubs_id, mmoc_info_ptr->nSubs) ||
      !MMOC_ENUM_IS_INRANGE(stk_id, mmoc_sub_info[asubs_id]->nStacks) ||
      !(mmoc_sub_info[asubs_id]->active_stacks & BM(stk_id)))
  {
    MMOC_MSG_HIGH_3("sub %d stk %d active stacks %d not valid for PROT_GEN_CMD", asubs_id, stk_id,
                    mmoc_sub_info[asubs_id]->active_stacks);
    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  if(cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.addl_action.action
      || mmoc_sub_info[asubs_id]->stack_prop[stk_id]->is_suspend)
  {
    MMOC_MSG_HIGH_5("PROT_GEN_CMD: sub %d stk %d addl_action: %d, suspend_reas_mask %d, is_suspend %d",
                    cmd_ptr->param.gen_prot_cmd.asubs_id,
                    cmd_ptr->param.gen_prot_cmd.stk_id,
                    cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.addl_action.action,
                    mmoc_sub_info[asubs_id]->stack_prop[stk_id]->suspend_reas_mask,
                    mmoc_sub_info[asubs_id]->stack_prop[stk_id]->is_suspend);
  }

  mmoc_update_emerg_pending(cmd_ptr, mmoc_info_ptr);

  if(cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.addl_action.action & MMOC_PREF_SYS_ADDL_ACTION_RESUME)
  {
    mmoc_sub_info[asubs_id]->stack_prop[stk_id]->suspend_reas_mask &= ~(BM(PROT_DEACT_DSDX_SUSP));
    if( mmoc_sub_info[asubs_id]->stack_prop[stk_id]->suspend_reas_mask == 0 )
    {
    mmoc_sub_info[asubs_id]->stack_prop[stk_id]->is_suspend = FALSE;
    }
    cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.addl_action.action &= ~(MMOC_PREF_SYS_ADDL_ACTION_RESUME);

    MMOC_MSG_HIGH_2("cleared: addl_action %d, suspend_reas_mask %d",
                    cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.addl_action.action,
                    mmoc_sub_info[asubs_id]->stack_prop[stk_id]->suspend_reas_mask);
  }

#ifdef FEATURE_MMODE_DUAL_SIM
  if(mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM) &&
      cmd_ptr->param.gen_prot_cmd.cmd_type == PROT_GEN_CMD_PREF_SYS_CHGD)
  {
    if(cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.domain_pref ==
        SD_SS_SRV_DOMAIN_PREF_PS_ONLY ||
        cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.domain_pref ==
        SD_SS_SRV_DOMAIN_PREF_CS_PS)
    {
      mmoc_sub_info[asubs_id]->ps_enabled = TRUE;
    }
    else
    {
      mmoc_sub_info[asubs_id]->ps_enabled = FALSE;
    }
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize the protocol state field and transaction info.
  */
  {
    sys_modem_as_id_e_type sub = cmd_ptr->param.gen_prot_cmd.asubs_id;
    sys_modem_as_id_e_type stk = cmd_ptr->param.gen_prot_cmd.stk_id;
    cmd_ptr->param.gen_prot_cmd.prot_state =
      mmoc_info_ptr->sub_prop[sub]->stack_prop[stk]->prot_state;
    mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info =
      cmd_ptr->param.gen_prot_cmd;
  }

#ifdef FEATURE_MMOC_LTE
  /* Indicate mode pref to LTE RRC only for primary stacks of sub*/
  if(cmd_ptr->param.gen_prot_cmd.cmd_type == PROT_GEN_CMD_PREF_SYS_CHGD &&
      cmd_ptr->param.gen_prot_cmd.stk_id == MM_STACK_0)
  {
    mmoc_sub_info[asubs_id]->mode_pref = cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.user_mode_pref;
    mmoc_send_lte_rrc_mode_pref_update();
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if phone is online.
  */
  if ( mmoc_dev_info->oprt_mode != PROT_OPRT_MODE_ONLINE )
  {
    /* Update pref in CM to SD even when not in ONLINE. So that latest
    ** pref from user are used before going ONLINE */
    if(cmd_ptr->param.gen_prot_cmd.cmd_type == PROT_GEN_CMD_PREF_SYS_CHGD &&
        ((cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.pref_reas ==
          SD_SS_PREF_REAS_RESTORE) ||
         (cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.addl_action.action
          & MMOC_PREF_SYS_ADDL_ACTION_UE_MODE_SWITCH) ||
         (cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.pref_reas ==
          SD_SS_PREF_REAS_USER)))
    {
      /* Inform SD protocols about new pref, even though protocols are not
      ** active. Set trans to invalid, as gen_cmd_cnf isn't needed**
      */
      mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.trans_id = PROT_TRANS_NONE;
      (void)mmoc_proc_prot_gen_cmd(FALSE, FALSE,&cmd_ptr->param.gen_prot_cmd);

      if(mmoc_proc_addl_action_ue_mode_switch(&(cmd_ptr->param.gen_prot_cmd),
                                              mmoc_info_ptr))
      {
        return MMOC_EVT_STATUS_CONSUMED;
      }
    }
    else
    {
      /* Phone not ONLINE, ignore the generic command.
      */
      MMOC_MSG_MED_0("incorrect phone state");
    }

    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Wakeup from power save */

  if( cmd_ptr->param.gen_prot_cmd.cmd_type == PROT_GEN_CMD_WAKEUP_FROM_PWR_SAVE )
  {
    MMOC_MSG_HIGH_1("gen cmd is WAKEUP_FROM_PWR_SAVE: prot_state %d",
                    mmoc_sub_info[asubs_id]->stack_prop[stk_id]->prot_state);

    if(mmoc_sub_info[asubs_id]->stack_prop[stk_id]->prot_state != PROT_STATE_PWR_SAVE &&
        mmoc_sub_info[asubs_id]->stack_prop[stk_id]->prot_state != PROT_STATE_NULL)
    {
      if(!mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM))
      {
        /* non-multisim:
        ** valid protocol states are NULL and PWR_SAVE.
        ** other protocol states are invalid because there are active protocols,
        ** or phone is not in ONLINE mode.
        */
        MMOC_MSG_HIGH_0("invalid protocol state");
        return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
      }
      else
      {
        /* Multisim: if UE is attempting acq, check the last SD action.
        ** If SD was doing 3GPP acq_db scans, process the wakeup command in SD.
        ** Else, reject the command.
        */
        sd_ss_act_s_type *ss_act;
        mm_sub_stk_id_s_type mm_id;
        sd_ss_act_e_type last_act;
        mm_id.asubs_id = asubs_id;
        mm_id.stk_id   = stk_id;
        last_act = sd_ss_act_get_per_mm_id( NULL, mm_id );

        if( last_act != SD_SS_ACT_ACQ_GWL )
        {
          MMOC_MSG_HIGH_1("last SD action %d is not ACQ_GWL", last_act);
          return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
        }

        ss_act = (sd_ss_act_s_type *) mmoc_mem_alloc( sizeof(sd_ss_act_s_type));
        if(ss_act == NULL)
        {
          MMOC_ERR_FATAL_0("Heap exhausted, mmoc_mem_alloc() returned NULL");
        }

        (void) sd_ss_act_get_per_mm_id( ss_act, mm_id );

        if( ss_act->prm.acq_gwl.scan_scope != SYS_SCAN_SCOPE_ACQ_DB )
        {
          MMOC_MSG_HIGH_1("scan scope %d is not ACQ_DB", ss_act->prm.acq_gwl.scan_scope);
          mmoc_mem_free(ss_act);
          return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
        }
        mmoc_mem_free(ss_act);
      }
    }
  }

  if(mmoc_is_1x_sxlte(asubs_id) && (stk_id == MM_STACK_0) &&
      cmd_ptr->param.gen_prot_cmd.cmd_type      == PROT_GEN_CMD_PREF_SYS_CHGD               &&
      cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.addl_action.action & MMOC_PREF_SYS_ADDL_ACTION_UE_MODE_SWITCH &&
      (mmoc_info_ptr->dev_prop.oprt_mode != PROT_OPRT_MODE_OFFLINE_CDMA &&
       mmoc_info_ptr->dev_prop.oprt_mode != PROT_OPRT_MODE_OFFLINE_AMPS &&
       mmoc_info_ptr->dev_prop.oprt_mode != PROT_OPRT_MODE_OFFLINE))

  {
#ifdef MMOC_CDMA_SUPPORTED
    mmoc_info_ptr->sub_prop[asubs_id]->ue_mode_pend_flag |= BM(MM_STACK_0);
#endif

#ifdef MMOC_HDR_SUPPORTED
    mmoc_info_ptr->sub_prop[asubs_id]->ue_mode_pend_flag |= BM(MM_STACK_1);
#endif

  }


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Move the trans. state to send the Generic command to the active
  ** protocol.
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_GEN_CMD_ENTER, mmoc_info_ptr );

  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_prot_gen_cmd_null() */



/*===========================================================================

FUNCTION mmoc_is_ps_detach_required

DESCRIPTION
  Indicates whether PS_DETACH is needed or not

RETURN VALUE
 TRUE: PS_DETCAH needed.
 FALSE: PS_DETACH not needed.

===========================================================================*/
static boolean mmoc_is_ps_detach_required (

  sys_modem_as_id_e_type         asubs_id,
  mmoc_state_info_s_type        *mmoc_info_ptr

)
{
  boolean ret = FALSE;

  if (mmoc_info_ptr == NULL)
  {
    return FALSE;
  }

  if (BM(asubs_id) & mmoc_info_ptr->dev_prop.active_subs)
  {
    ret = TRUE;
  }

  MMOC_MSG_HIGH_4("ps_detach_required %d sub %d subs_feature %d active_subs %d",
                  ret, asubs_id,
                  mmoc_info_ptr->sub_prop[asubs_id]->subs_feature,
                  mmoc_info_ptr->dev_prop.active_subs);

  return ret;

}


/*===========================================================================

FUNCTION mmoc_process_prot_gen_cmd_enter

DESCRIPTION
  This function is responsible for the sending the Generic command to the
  active protocol, if none is present calls into SD directly.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_gen_cmd_enter
(

  const mmoc_cmd_msg_s_type      *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  prot_gen_cmd_s_type             *gen_cmd_ptr    = NULL;
  prot_cmd_pref_sys_chgd_s_type   *pref_sys_chgd  = NULL;
  sys_ps_detach_e_type             ps_detach_type = SYS_PS_DETACH_TYPE_MODE_CHANGE;
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info  = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT((cmd_ptr != NULL) || (rpt_ptr != NULL));
  gen_cmd_ptr = &(mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info);

#if defined(FEATURE_LTE) || defined(FEATURE_GSM) || defined(FEATURE_WCDMA)
  pref_sys_chgd = &(gen_cmd_ptr->param.pref_sys_chgd);

  if(gen_cmd_ptr->cmd_type == PROT_GEN_CMD_PREF_SYS_CHGD &&
      pref_sys_chgd->addl_action.action & MMOC_PREF_SYS_ADDL_ACTION_PS_DETACH &&
      mmoc_dev_info->oprt_mode == PROT_OPRT_MODE_ONLINE &&
      mmoc_is_ps_detach_required(gen_cmd_ptr->asubs_id, mmoc_info_ptr))
  {
#ifdef FEATURE_LTE_TO_HDR_OH
    {
      /* Deactivate HDR tunnel. CMSDS will take care of checking
      ** if HDR tunnel mode is active before deact attempt
      */
      cmsds_deactivate_hdr_tunnel_req(mmoc_dev_info->sub_with_3gpp2);
    }
#endif

    /* Deactivate 1x stack if it is in 1x CSFB mode */
#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)) && \
       defined( FEATURE_LTE_TO_1X )
    if (cmsds_is_1xcsfb_activated(mmoc_dev_info->sub_with_3gpp2))
    {
      cmsds_deactivate_1xcsfb_protocol (mmoc_next_trans_id(mmoc_info_ptr),
                                        PROT_DEACT_ACQ_OTHERS,
                                        mmoc_dev_info->sub_with_3gpp2);
      /* Protocol deactivate request sent, wait for confirmation. */
      mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_1XCSFB_DEACT_CNF,
                               mmoc_info_ptr
                             );
      return MMOC_EVT_STATUS_CONSUMED;
    }
#endif

    /* If detach is because of explicit PS detach command from CM client by removal
    ** of PS from domain preference the detach type would be Domain Change
    */
    if(pref_sys_chgd->addl_action.action & MMOC_PREF_SYS_ADDL_ACTION_POLICY_PS_DETACH)
    {
      ps_detach_type = SYS_PS_DETACH_TYPE_POLICY_CHANGE;
      pref_sys_chgd->addl_action.action &= ~(MMOC_PREF_SYS_ADDL_ACTION_POLICY_PS_DETACH);
    }
    else if(pref_sys_chgd->domain_pref == SD_SS_SRV_DOMAIN_PREF_CS_ONLY ||
            pref_sys_chgd->domain_pref == SD_SS_SRV_DOMAIN_PREF_NONE)
    {
      ps_detach_type = SYS_PS_DETACH_TYPE_SRV_DOMAIN_CHANGE;
    }
    else
    {
      ps_detach_type = SYS_PS_DETACH_TYPE_MODE_CHANGE;
    }

    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = gen_cmd_ptr->asubs_id;
      mm_id.stk_id   = gen_cmd_ptr->stk_id;
      cmregprx_cmd_ps_detach(ps_detach_type,
                             mmoc_next_trans_id(mmoc_info_ptr),
                             mm_id
                            );
    }

    pref_sys_chgd->addl_action.action &= ~(MMOC_PREF_SYS_ADDL_ACTION_PS_DETACH);

    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_PS_DETACH_CNF,
                             mmoc_info_ptr
                           );

    return MMOC_EVT_STATUS_CONSUMED;
  }
  #ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  /*If the user has seleccted Net Sel Mode as Limited we need to trigger  STOP Mode Req with the Mode Change*/
  if(pref_sys_chgd->addl_action.action & MMOC_PREF_SYS_ADDL_ACTION_FORCE_LIMITED_SRV)
  {
    mm_sub_stk_id_s_type mm_id;
    mm_id.asubs_id = gen_cmd_ptr->asubs_id;
    mm_id.stk_id   = gen_cmd_ptr->stk_id;

	
    if(mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state == PROT_STATE_ONLINE_GWL)
    {
      mmoc_dev_info->deact_req_idx.asubs_id = mm_id.asubs_id;
      mmoc_dev_info->deact_req_idx.stk_id = mm_id.stk_id ;
	  
      cmregprx_cmd_gwl_deactivate_protocol(PROT_DEACT_SUBSC_CHGD,
                                         mmoc_next_trans_id(mmoc_info_ptr),
                                         mm_id);
	
      pref_sys_chgd->addl_action.action &= ~(MMOC_PREF_SYS_ADDL_ACTION_FORCE_LIMITED_SRV);

      mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL,
                             mmoc_info_ptr
                           );

      return MMOC_EVT_STATUS_CONSUMED;
     }
  }
  #endif
#else
  SYS_ARG_NOT_USED(pref_sys_chgd);
  SYS_ARG_NOT_USED(ps_detach_type);
#endif //FEATURE_LTE

#ifdef MMOC_GW_SUPPORTED
  if(mmoc_proc_addl_action_ue_mode_switch(gen_cmd_ptr, mmoc_info_ptr))
  {
    return MMOC_EVT_STATUS_CONSUMED;
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the generic command to the active protocol, if there is one.
  */
  if(mmoc_send_prot_gen_cmd(gen_cmd_ptr,mmoc_info_ptr))
  {
    /* Generic command sent, wait for acknowledgement.
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF,
                             mmoc_info_ptr
                           );
  }
  else
  {
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Process the generic command in MMoC itself.
    ** Set the trans_id to NONE, since no generic command ACK is required.
    */
    mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.trans_id = PROT_TRANS_NONE;

    (void) mmoc_proc_prot_gen_cmd(TRUE, TRUE, gen_cmd_ptr);

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Activate protocol only if standby pref permit its activation */

    if(mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM) &&
        !(mmoc_dev_info->active_subs & BM(mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.asubs_id)))
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.asubs_id;
      mm_id.stk_id   = mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.stk_id;

      if(mmoc_is_sxlte(mm_id.asubs_id) &&
          mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.cmd_type
          == PROT_GEN_CMD_LOCAL_ACT &&
          mm_id.stk_id == MM_STACK_2)
      {
        mmoc_sub_info[mm_id.asubs_id]->stack_prop[MM_STACK_2]->prot_state = PROT_STATE_ONLINE_GWL;
        MMOC_MSG_HIGH_0("SC_SGLTE: LOCAL_ACT, set protocol to active");
      }
      else if(mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state != PROT_STATE_NULL)
      {
        sd_ss_ind_user_prot_deactivate(mm_id.asubs_id, NULL, SD_SS_PROT_DEACT_INFO_NONE);

        mmoc_update_prot_state(PROT_STATE_PWR_SAVE,
                               mm_id,
                               mmoc_info_ptr
                              );
        sd_ss_ind_misc_pwr_save_enter(mm_id, NULL);
      }

    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* For LOCAL_ACT_IND, Hybr-2 will be activate by NAS. So we need to bring
    ** HYBR-2 active and then MMOC will send prot_gen_cmd to HYBR-2.
    */

    else if (mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.cmd_type
             == PROT_GEN_CMD_LOCAL_ACT &&
             mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.stk_id
             == MM_STACK_2 &&
             mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.asubs_id == mmoc_dev_info->sub_with_3gpp2)
    {
      mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_2]->prot_state = PROT_STATE_ONLINE_GWL;
      MMOC_MSG_HIGH_0("SC_SGLTE: LOCAL_ACT, set protocol to active");
    }
    else
    {
      /* Activate the protocol or invoke
      ** transaction handler if this function returns EVT_STATUS_NOT_CONSUMED.
      */
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.asubs_id;
      mm_id.stk_id   = mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.stk_id;

      if(mmoc_activate_prot_online(mm_id,mmoc_info_ptr)
          == MMOC_EVT_STATUS_NOT_CONSUMED
        )
      {
        /* Invoke the new transaction handler.
        */
        return MMOC_EVT_STATUS_NOT_CONSUMED;
      }
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Done with processing, clear the transaction.
    */
    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  return MMOC_EVT_STATUS_CONSUMED;

} /* mmoc_process_prot_gen_cmd_enter() */



/*===========================================================================

FUNCTION mmoc_process_prot_gen_cmd_activation_cnf

DESCRIPTION
 This function handles the processing  of the ACTIVATION_CNF rpt ( used in
 reslection from 3gpp2 to LTE) in the WAIT for ACT CNF trans state.

  In this state, the state handler will
  1. Validate the activation confirmation report received
  2. Clear the transaction

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_gen_cmd_activation_cnf
(

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  MMOC_ASSERT( rpt_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the rpt ptr is NULL
  */
  if ( rpt_ptr == NULL )
  {
    /* Not the expected report.
    */
    MMOC_ERR_0("rpt NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Process the received report.
  */
  switch(rpt_ptr->rpt.name)
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      return  mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    case MMOC_RPT_ACTIVATION_CNF:
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = rpt_ptr->param.activation_cnf.mm_id.asubs_id;
      mm_id.stk_id   = rpt_ptr->param.activation_cnf.mm_id.stk_id;

      if(mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state !=
          rpt_ptr->param.activation_cnf.prot_state)
      {
        /* Not the expected report. Go back to waiting for cnf
        */
        MMOC_ERR_2("Prot states out of sync, prot_state %d rpt prot_state %d",
                   mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state,
                   rpt_ptr->param.activation_cnf.prot_state);

        return MMOC_EVT_STATUS_CONSUMED;
      }

      /* If it is the correct trans id, clear the transaction
      */
      return mmoc_clear_transaction(NULL, mmoc_info_ptr);
    }

    default:
      MMOC_ERR_1("Unexpected report received %d",
                 rpt_ptr->rpt.name);
      return MMOC_EVT_STATUS_CONSUMED;


  } /* switch ( rpt_ptr->rpt.name ) */

} //mmoc_process_prot_gen_cmd_activation_cnf



/*===========================================================================

FUNCTION mmoc_process_prot_gen_activation_enter

DESCRIPTION
 This function moves MMOC into the wait for activation CNF state

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_gen_activation_enter
(

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if phone is online.
  */
  if(mmoc_info_ptr->dev_prop.oprt_mode != PROT_OPRT_MODE_ONLINE )
  {
    /* Phone not ONLINE, ignore the generic command.
    */
    MMOC_ERR_0("incorrect phone state");
    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Move the trans. state to wait for activation CNF
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_ACTIVATION_CNF, mmoc_info_ptr );

  return MMOC_EVT_STATUS_CONSUMED;

} //mmoc_process_prot_gen_activation_enter


static boolean mmoc_validate_gen_cmd_cnf(
  mmoc_state_info_s_type         *mmoc_info_ptr,

  mmoc_rpt_task_name_e_type prot_task
)
{
  sys_modem_as_id_e_type asubs_id = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.asubs_id;
  multimode_stack_e_type stack_id = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.stk_id;
  prot_state_e_type prot_state = PROT_STATE_NULL;

  if(!(asubs_id >= SYS_MODEM_AS_ID_1 &&
       asubs_id < MAX_SIMS &&
       stack_id >= MM_STACK_0 &&
       stack_id < MAX_STACKS))
  {
    return FALSE;
  }

  if(mmoc_info_ptr->sub_prop[asubs_id]!= NULL
    && mmoc_info_ptr->sub_prop[asubs_id]->stack_prop[stack_id] != NULL)
  {
    prot_state = mmoc_info_ptr->sub_prop[asubs_id]->stack_prop[stack_id]->prot_state;
  }

  if((prot_state == PROT_STATE_ONLINE_GWL && prot_task == MMOC_TASK_NAME_CM)
    || (prot_state == PROT_STATE_ONLINE_CDMA && prot_task == MMOC_TASK_NAME_CDMA)
    )
  {
    return FALSE;
  }

  #ifdef FEATURE_HDR
  /* HDR behaves differently to 1x & CMREGPRX that it starts sending cnf with processed as false
    ** when deactivation is in progress, so covering this case as well. Checking queue cnt to cover race-condition
    ** where hdr deact is just done before MMOC calls its API*/
  if(prot_state == PROT_STATE_ONLINE_HDR && prot_task == MMOC_TASK_NAME_HDR && 
#ifdef FEATURE_DISABLED_HABANERO
     hdrcp_is_deactivation_in_progress() == FALSE && 
#endif
    mmoc_get_task_info_ptr()->rpt_q.cnt == 0)
  {
    return FALSE;
  }
  #endif
  return TRUE;
}

/*===========================================================================

FUNCTION mmoc_process_prot_gen_cmd_cnf

DESCRIPTION
  This function handles the generic protocol command transaction in the
  WAIT_GEN_CMD_CNF transaction state.

  In this state, the state handler will
  1. Validate the Ack received. Done with the transaction if ack is correct
     and ack says that the generic command was processed. If the ack says
     that generic command was not processed, it is race condition wherein
     the MMoC recvd the deactivated indication while waiting for the ack.

  Note! Race condition, if a protocol deactivated indication is received
  while waiting for ACK, the handler will activate the AMPS protocol only
  if it is CDMA to AMPS handoff case. Else the deactivated indication
  report will be dropped.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_gen_cmd_cnf
(
  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  prot_gen_cmd_e_type     cmd_type   = PROT_GEN_CMD_MAX;
  mmoc_state_info_dev_prop_s_type  *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  mm_sub_stk_id_s_type mm_id;

  MMOC_ASSERT( rpt_ptr != NULL );

  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if a report is received.
  */
  if ( rpt_ptr == NULL )
  {
    /* Not expecting a command.
    */
    MMOC_ERR_0("Command unexpected");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Process the received report.
  */
  switch(rpt_ptr->rpt.name)
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      /* Autonomous deactivated report received.
      */
  mm_id.asubs_id = rpt_ptr->param.prot_deactd_ind.mm_id.asubs_id;
  mm_id.stk_id   = rpt_ptr->param.prot_deactd_ind.mm_id.stk_id;

  /* Check if the deactivation was from SS_HDR system selection instance.
   */
  if ( rpt_ptr->param.prot_deactd_ind.prot_state  == PROT_STATE_ONLINE_HDR &&
       mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_1]->prot_state == PROT_STATE_ONLINE_HDR)
  {
    mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
    mm_id.stk_id = MM_STACK_1;
  }

      MMOC_MSG_HIGH_5("AUTO_DEACTD_IND: Received autonomous deactivated report on sub %d stk %d, cmd sub %d stk %d, reason %d",
                         mm_id.asubs_id,
                         mm_id.stk_id,
                         mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.asubs_id,
                         mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.stk_id,
                         rpt_ptr->param.prot_deactd_ind.reason);

      if(mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.asubs_id != mm_id.asubs_id)
      {
        MMOC_MSG_HIGH_2("Buffering auto deact on different sub. Auto_deact sub:%d, curr trans sub:%d ",
                            mm_id.asubs_id,
                            mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.asubs_id );
        return mmoc_buf_auto_deact_ind_rpt(rpt_ptr,
                                            mmoc_info_ptr,
                                            rpt_ptr->param.prot_deactd_ind.mm_id);
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Process LTE to DO redirection abort
      */
      if( rpt_ptr->param.prot_deactd_ind.prot_state == PROT_STATE_ONLINE_HDR &&
          rpt_ptr->param.prot_deactd_ind.reason     == PROT_DEACT_REDIR_ABORT &&
          mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state
          != PROT_STATE_ONLINE_HDR)
      {
        MMOC_MSG_HIGH_0("Function: mmoc_process_prot_gen_cmd_cnf DO Redir aborted");

        mmoc_dev_info->is_redir_allowed = FALSE;

#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
        sd_ss_hdr_irat_fail_ind(mm_id.asubs_id);
#endif
        return MMOC_EVT_STATUS_CONSUMED;

      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef MMOC_HYBR_SUPPORTED
      /* Check if the deactivation was from SS_HDR system selection instance.
      */
      if ( rpt_ptr->param.prot_deactd_ind.prot_state == PROT_STATE_ONLINE_HDR &&
           mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state
           == PROT_STATE_ONLINE_HDR &&
           mm_id.asubs_id == mmoc_dev_info->sub_with_3gpp2 &&
           mm_id.stk_id   == MM_STACK_1 &&
           mmoc_sub_info[mm_id.asubs_id]->active_stacks & BM(mm_id.stk_id))
      {
        /* If hybrid preference is not ON then hybrid hdr instance should
        ** not be triggered to go ONLINE
        */
        if(sd_misc_is_hybr_opr(mm_id.asubs_id))
        {
          /* It is for SS_HDR instance, activate SS_HDR and return.
          */
          (void)mmoc_activate_prot_online_ss_hdr( sd_ss_hdr_act_get(mm_id.asubs_id, NULL),
                                                  mm_id.asubs_id,
                                                  mmoc_info_ptr
                                                );
        }
        else
        {
          mm_sub_stk_id_s_type mm_id;
          mm_id.asubs_id = mmoc_dev_info->sub_with_3gpp2;
          mm_id.stk_id   = MM_STACK_1;

          MMOC_ERR_0 ("trans_state WAIT_GEN_CMD_CNF, AUTO_DEACTD_IND from HYBR_HDR");

          mmoc_update_prot_state( PROT_STATE_NULL,
                                  mm_id,
                                  mmoc_info_ptr
                                );
        }

        return MMOC_EVT_STATUS_CONSUMED;
      }
#endif /* MMOC_HYBR_SUPPORTED */

      /* Do not process AUTO_DEACT_IND from 1x, if 1x protocol is still in active state
            * Ideally this should never happen
            */
#ifdef MMOC_CDMA_SUPPORTED
      if ( rpt_ptr->param.prot_deactd_ind.prot_state == PROT_STATE_ONLINE_CDMA &&
           mm_id.stk_id == MM_STACK_0 &&
           mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state == PROT_STATE_ONLINE_CDMA &&
           mc_is_cdma_active() == TRUE )
      {
        MMOC_ERR_0 ("Received AUTO_DEACT_IND from 1x but 1x is still active");
        return MMOC_EVT_STATUS_CONSUMED;
      }
#endif

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef MMOC_HYBR_GW_SUPPORTED

      /* Check if the deactivation was from SS_HDR system selection instance.
      */
      if ( rpt_ptr->param.prot_deactd_ind.prot_state == PROT_STATE_ONLINE_GWL &&
           mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state
           == PROT_STATE_ONLINE_GWL &&
           mm_id.asubs_id == mmoc_dev_info->sub_with_3gpp2 &&
           mm_id.stk_id   == MM_STACK_1 &&
           mmoc_sub_info[mm_id.asubs_id]->active_stacks & BM(mm_id.stk_id))
      {
        /* If hybrid preference is not ON then hybrid hdr instance should
        ** not be triggered to go ONLINE
        */
        if(sd_misc_is_hybr_opr(mm_id.asubs_id))
        {
          /* It is for SS_HDR instance, activate SS_HDR and return.
          */
          (void)mmoc_activate_prot_online_ss_hdr( sd_ss_hdr_act_get(mm_id.asubs_id, NULL),
                                                  mm_id.asubs_id,
                                                  mmoc_info_ptr
                                                );
        }
        else
        {
          MMOC_ERR_0("trans_state WAIT_GEN_CMD_CNF, AUTO_DEACTD_IND from HYBR_HDR");

          mmoc_update_prot_state( PROT_STATE_NULL,
                                  mm_id,
                                  mmoc_info_ptr
                                );
        }

        return MMOC_EVT_STATUS_CONSUMED;
      }
#endif //MMOC_HYBR_GW_SUPPORTED

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Check if auto deactivation from HYBR_2 stack
      */
      if(mmoc_is_1x_sxlte(rpt_ptr->param.prot_deactd_ind.mm_id.asubs_id) &&
          mm_id.asubs_id == mmoc_dev_info->sub_with_3gpp2 &&
          mm_id.stk_id == MM_STACK_2 &&
          mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state == PROT_STATE_ONLINE_GWL &&
          mmoc_sub_info[mm_id.asubs_id]->active_stacks & BM(mm_id.stk_id))
      {
        MMOC_MSG_HIGH_0("calling mmoc_activate_prot_online_ss_hybr_2");
        (void)mmoc_activate_prot_online_ss_hybr_2(sd_ss_act_get_per_mm_id(NULL, mm_id),
            mm_id.asubs_id,
            mmoc_info_ptr);
        return MMOC_EVT_STATUS_CONSUMED;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* At this point , we know that Auto deact is coming from prot on MAIN
      ** If the prot gen cmd on MAIN is of type IRAT TO GWL, allow post processing
      ** to take place, instead of immediately activating prot on MAIN.
      */
      cmd_type = mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.cmd_type;
      if(cmd_type   == PROT_GEN_CMD_IRAT_TO_GWL &&
          mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.stk_id == MM_STACK_0)
      {
        mmoc_dev_info->trans_info.gen_prot_cmd.is_activate_main = TRUE;
        mmoc_update_prot_state( PROT_STATE_NULL,
                                mm_id,
                                mmoc_info_ptr
                              );
        return MMOC_EVT_STATUS_CONSUMED;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      if(mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM))
      {
        /* For dual standby, activate MAIN only if MAIN should be active.
        */
        if(mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY ||
            mmoc_dev_info->active_subs & BM(mm_id.asubs_id))
        {
          /*  Activate main at the end of transaction.
          */
          if(sd_ss_act_get_per_mm_id( NULL, mm_id ) == SD_SS_ACT_PWR_SAVE)
          {
            return mmoc_activate_prot_online_ss_main( sd_ss_act_get_per_mm_id( NULL, mm_id ),
                   mm_id.asubs_id,
                   mmoc_info_ptr
                                                    );
          }
          else if (mm_id.asubs_id == mmoc_dev_info->sub_with_3gpp2)
          {
            mmoc_dev_info->trans_info.gen_prot_cmd.is_activate_main = TRUE;
            mmoc_update_prot_state( PROT_STATE_NULL,
                                    mm_id,
                                    mmoc_info_ptr
                                  );
            return MMOC_EVT_STATUS_CONSUMED;
          }
          else
          {
            return mmoc_activate_prot_online_ss_main( sd_ss_act_get_per_mm_id(NULL, mm_id),
                                                    mm_id.asubs_id,
                                mmoc_info_ptr
                              );
          }
        }
      }
      else
      {
        /*  Activate main at the end of transaction.
        */
        if(sd_ss_act_get_per_mm_id( NULL, mm_id ) == SD_SS_ACT_PWR_SAVE)
        {
          return mmoc_activate_prot_online_ss_main( sd_ss_act_get_per_mm_id( NULL, mm_id ),
                 mm_id.asubs_id,
                 mmoc_info_ptr
                                                  );
        }
        else
        {
          mmoc_dev_info->trans_info.gen_prot_cmd.is_activate_main = TRUE;
          mmoc_update_prot_state( PROT_STATE_NULL,
                                  mm_id,
                                  mmoc_info_ptr
                                );
          return MMOC_EVT_STATUS_CONSUMED;
        }
      }

    case MMOC_RPT_PROT_GEN_CMD_CNF:

      /* Check if it is the right acknowledgement.
      */
      if ( mmoc_dev_info->trans_id != rpt_ptr->param.gen_cmd_cnf.trans_id )
      {
        /* Not the expected transaction Id.
        */
        MMOC_MSG_LOW_0("Received incorrect trans_id for Gen cmd ack");
        return MMOC_EVT_STATUS_CONSUMED;
      }
      break;

    case MMOC_RPT_PROT_DEACTD_CNF:
    case MMOC_RPT_PH_STAT_CHGD_CNF:
    case MMOC_RPT_MAX:
    default:
      MMOC_MSG_HIGH_1(" Received unexpected report %d", rpt_ptr->rpt.name);

      return MMOC_EVT_STATUS_CONSUMED;
  } /* switch (rpt_ptr->rpt.name ) */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the generic command was processed by the protocols.
  */
  if ( !rpt_ptr->param.gen_cmd_cnf.is_cmd_processed )
  {
    /* Race condition: Protocol was deactivated just before the generic
    ** command was posted, either send generic command or process it in
    ** MMoC itself.
    */
    if(mmoc_validate_gen_cmd_cnf(mmoc_info_ptr,rpt_ptr->rpt.task_name) == FALSE)
    {
      MMOC_ERR_FATAL_1("MMOC & Protocol:%d out of sync",rpt_ptr->rpt.task_name);
    }

    mmoc_update_trans_state( MMOC_TRANS_STATE_GEN_CMD_ENTER,
                             mmoc_info_ptr
                           );
    return MMOC_EVT_STATUS_NOT_CONSUMED;

  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the protocol is deactivating.
  */

  if ( rpt_ptr->param.gen_cmd_cnf.is_prot_deactivating )
  {
    MMOC_MSG_HIGH_1("is_prot_deactivating=%d",
                    rpt_ptr->param.gen_cmd_cnf.is_prot_deactivating);

    /* Restart the sanity timer when MMOC is waiting for deactivation indication */
    mmoc_sanity_timer_reset(mmoc_info_ptr);
    mmoc_sanity_timer_set(mmoc_info_ptr, MMOC_PWRDOWN_SANITY_TIME);

    /* Wait for deactivated indication from protocol.
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_AUTO_DEACTD_IND,
                             mmoc_info_ptr
                           );
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Generic command was processed, end the transaction.
  */
  return mmoc_clear_transaction(NULL, mmoc_info_ptr);

} /* mmoc_process_prot_gen_cmd_cnf() */



/*===========================================================================

FUNCTION mmoc_process_prot_gen_cmd_auto_deactd_ind

DESCRIPTION
  This function handles the generic protocol command transaction in the
  WAIT_AUTO_DEATD_IND transaction state.

  In this state, the state handler will

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_gen_cmd_auto_deactd_ind
(
  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type           rpt_sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type           rpt_stk = MM_STACK_NONE;
  mm_sub_stk_id_s_type mm_id;

  MMOC_ASSERT( rpt_ptr != NULL );

  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if we got the autonomous deactivated report.
  */
  if (rpt_ptr == NULL)
  {
    /* Unexpected report received.
    */
    MMOC_ERR_0("unexpected report ");
    return MMOC_EVT_STATUS_CONSUMED;

  }

  rpt_sub = rpt_ptr->param.prot_deactd_ind.mm_id.asubs_id;
  rpt_stk = rpt_ptr->param.prot_deactd_ind.mm_id.stk_id;

  if (rpt_ptr->rpt.name != MMOC_RPT_PROT_AUTO_DEACTD_IND )
  {
    /* Unexpected report received.
    */
    MMOC_ERR_1("unexpected report %d", rpt_ptr->rpt.name);
    return MMOC_EVT_STATUS_CONSUMED;

  }

  MMOC_MSG_HIGH_5("AUTO_DEACTD_IND: gen_prot_cmd sub %d stk %d, prot_deactd_ind sub %d stk %d, reason=%d",
                  mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.asubs_id,
                  mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.stk_id,
                  rpt_sub,
                  rpt_stk,
                  rpt_ptr->param.prot_deactd_ind.reason);

  if(mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.asubs_id != rpt_sub)
  {
    MMOC_MSG_HIGH_2("Buffering auto deact on different sub. Auto_deact sub:%d, curr trans sub:%d ",
                    rpt_sub,mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.asubs_id );
    return  mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check for DO redirection abort. */

  if(rpt_sub == mmoc_dev_info->sub_with_3gpp2 &&
      rpt_stk == MM_STACK_1 &&
      rpt_ptr->param.prot_deactd_ind.prot_state == PROT_STATE_ONLINE_HDR &&
      rpt_ptr->param.prot_deactd_ind.reason     == PROT_DEACT_REDIR_ABORT &&
      mmoc_sub_info[rpt_sub]->stack_prop[rpt_stk]->prot_state != PROT_STATE_ONLINE_HDR)
  {

    MMOC_MSG_HIGH_0("mmoc_process_prot_gen_cmd_auto_deactd_ind DO Redir aborted");

    mmoc_dev_info->is_redir_allowed = FALSE;

#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
    sd_ss_hdr_irat_fail_ind(rpt_sub);
#endif
    return MMOC_EVT_STATUS_CONSUMED;

  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Process deactd report based on the stack. */

  switch(mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.stk_id)
  {
    case MM_STACK_1:
    {
#ifdef MMOC_HYBR_SUPPORTED
      if(rpt_ptr->param.prot_deactd_ind.prot_state == PROT_STATE_ONLINE_HDR &&
          mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_1]->prot_state
          == PROT_STATE_ONLINE_HDR)
      {
        if(sd_misc_is_hybr_opr(mmoc_dev_info->sub_with_3gpp2))
        {
          /* It is for SS_HDR instance, activate SS_HDR and return.
          */
          (void)mmoc_activate_prot_online_ss_hdr( sd_ss_hdr_act_get(mmoc_dev_info->sub_with_3gpp2, NULL),
                                                  mmoc_dev_info->sub_with_3gpp2,
                                                  mmoc_info_ptr
                                                );
        }
        else
        {
          mm_id.asubs_id = mmoc_dev_info->sub_with_3gpp2;
          mm_id.stk_id   = MM_STACK_1;

          MMOC_MSG_HIGH_0("Hybr pref is OFF, AUTO_DEACTD_IND for HYBR_HDR");

          mmoc_update_prot_state( PROT_STATE_NULL,
                                  mm_id,
                                  mmoc_info_ptr);
        }

        break;
      }
#endif /* MMOC_HYBR_SUPPORTED */

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Check if auto deactivation from HYBR_2 stack
      */
      if(rpt_sub == mmoc_dev_info->sub_with_3gpp2 &&
          rpt_stk == MM_STACK_2 &&
          mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_2]->prot_state
          == PROT_STATE_ONLINE_GWL)
      {
        mm_id.asubs_id = mmoc_dev_info->sub_with_3gpp2;
        mm_id.stk_id = MM_STACK_2;
        /* If HYBR_2 stack should be online, then activate SS_HYBR_2 again. */
        if(mmoc_dev_info->active_subs & BM(mmoc_dev_info->sub_with_3gpp2) &&
            mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->active_stacks & MM_STACK_0_MASK &&
            (mmoc_is_sxlte(mmoc_dev_info->sub_with_3gpp2) ||
             mmoc_is_1x_sxlte(mmoc_dev_info->sub_with_3gpp2))
          )
        {
          MMOC_MSG_HIGH_0("gen_cmd ss main, but recv deact_ind on hybr2");

          (void)mmoc_activate_prot_online_ss_hybr_2( sd_ss_act_get_per_mm_id(NULL,mm_id),
              mmoc_dev_info->sub_with_3gpp2,
              mmoc_info_ptr
                                                   );
        }
        else if(((mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY ||
                  mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_SINGLE_STANDBY) &&
                 mmoc_dev_info->active_subs & BM(mmoc_dev_info->sub_with_3gpp2) &&
                 mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->active_stacks & MM_STACK_2_MASK)
#ifdef FEATURE_MMODE_TRIPLE_SIM
                || mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY
#endif
                )
        {
          (void)mmoc_activate_prot_online_ss_hybr_2( sd_ss_act_get_per_mm_id(NULL,mm_id),
              mmoc_dev_info->sub_with_3gpp2,
              mmoc_info_ptr
                                                   );
        }
        else
        {

          MMOC_ERR_2 ("AUTO_DEACTD_IND from HYBR_2, DS_Pref = %d, active_subs = %d",
                      mmoc_dev_info->standby_pref,
                      mmoc_dev_info->active_subs);

          mmoc_update_prot_state( PROT_STATE_NULL,
                                  mm_id,
                                  mmoc_info_ptr
                                );
        }

        return MMOC_EVT_STATUS_CONSUMED;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Check if SS_MAIN got deactivated.
      */
      if(rpt_ptr->param.prot_deactd_ind.prot_state !=
          mmoc_sub_info[rpt_sub]->stack_prop[rpt_stk]->prot_state)
      {
        /* Bogus report.
        */
        MMOC_MSG_MED_2("Invalid report, rpt prot_state %d prot_state %d",
                       rpt_ptr->param.prot_deactd_ind.prot_state,
                       mmoc_sub_info[rpt_sub]->stack_prop[rpt_stk]->prot_state);

        return MMOC_EVT_STATUS_CONSUMED;
      }
      else
      {
        mm_id.asubs_id = rpt_sub;
        mm_id.stk_id   = rpt_stk;

        /* SS_MAIN got deactivated.
        */
        mmoc_dev_info->trans_info.gen_prot_cmd.is_activate_main = TRUE;
        mmoc_update_prot_state( PROT_STATE_NULL,
                                mm_id,
                                mmoc_info_ptr
                              );
        return MMOC_EVT_STATUS_CONSUMED;
      }
    }

    case MM_STACK_0:
    {
#ifdef MMOC_HYBR_SUPPORTED
      /* Check is the SS_HDR protocol is deactivated.
      */
      if(rpt_ptr->param.prot_deactd_ind.prot_state == PROT_STATE_ONLINE_HDR  &&
          mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_1]->prot_state
          == PROT_STATE_ONLINE_HDR )
      {
        /* It is for SS_HDR instance, activate SS_HDR and return.
        */
        (void)mmoc_activate_prot_online_ss_hdr( sd_ss_hdr_act_get(mmoc_dev_info->sub_with_3gpp2, NULL),
                                                mmoc_dev_info->sub_with_3gpp2,
                                                mmoc_info_ptr
                                              );
        return MMOC_EVT_STATUS_CONSUMED;
      }
#endif /* MMOC_HYBR_SUPPORTED */

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Check if auto deactivation from HYBR_2 stack */
      if(mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM) &&
          rpt_sub == mmoc_dev_info->sub_with_3gpp2 &&
          rpt_stk == MM_STACK_2 &&
          mmoc_sub_info[mmoc_dev_info->sub_with_3gpp2]->stack_prop[MM_STACK_2]->prot_state
          == PROT_STATE_ONLINE_GWL)
      {
        mm_id.asubs_id = rpt_sub;
        mm_id.stk_id   = rpt_stk;
        /* If HYBR_2 stack should be online, then activate SS_HYBR_2 again. */
        if(mmoc_is_sxlte(rpt_sub) &&
            mmoc_dev_info->active_subs & BM(rpt_sub) &&
            mmoc_sub_info[rpt_sub]->active_stacks & MM_STACK_0_MASK)
        {
          /* check main active for SG/SV  + G */
          (void)mmoc_activate_prot_online_ss_hybr_2( sd_ss_act_get_per_mm_id(NULL,mm_id),
              rpt_sub,
              mmoc_info_ptr
                                                   );
        }
#ifdef FEATURE_MMODE_TRIPLE_SIM
        else if(mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY ||
                ((mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY ||
                  mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_SINGLE_STANDBY) &&
                 mmoc_dev_info->active_subs & BM(rpt_sub) &&
                 mmoc_sub_info[rpt_sub]->active_stacks & MM_STACK_2_MASK))
#else
        else if(mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY ||
                (mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_SINGLE_STANDBY &&
                 mmoc_dev_info->active_subs & BM(rpt_sub) &&
                 mmoc_sub_info[rpt_sub]->active_stacks & MM_STACK_2_MASK))
#endif
        {
          (void)mmoc_activate_prot_online_ss_hybr_2( sd_ss_act_get_per_mm_id(NULL, mm_id),
              rpt_sub,
              mmoc_info_ptr
                                                   );
        }
        else
        {
          mm_id.asubs_id = rpt_sub;
          mm_id.stk_id   = rpt_stk;

          MMOC_ERR_2("AUTO_DEACTD_IND from HYBR_2, DS_Pref = %d, active_subs = %d",
                     mmoc_dev_info->standby_pref,
                     mmoc_dev_info->active_subs);

          mmoc_update_prot_state( PROT_STATE_NULL,
                                  mm_id,
                                  mmoc_info_ptr
                                );

        }

        return MMOC_EVT_STATUS_CONSUMED;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Check if auto deactivation from HYBR_2 stack
      */
      if(mmoc_is_1x_sxlte(rpt_sub))
      {
        if( rpt_stk       == MM_STACK_2 &&
            mmoc_sub_info[rpt_sub]->stack_prop[rpt_stk]->prot_state == PROT_STATE_ONLINE_GWL &&
            ((mmoc_sub_info[rpt_sub]->active_stacks & MM_STACK_0_MASK &&
              mmoc_dev_info->active_subs & BM(rpt_sub)) ||
             !mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM))
          )
        {
          MMOC_MSG_HIGH_0("gen_cmd ss main, but recv deact_ind on hybr2");
          mm_id.asubs_id = rpt_sub;
          mm_id.stk_id   = rpt_stk;
          (void)mmoc_activate_prot_online_ss_hybr_2(sd_ss_act_get_per_mm_id(NULL, mm_id),
              rpt_sub,
              mmoc_info_ptr);
          return MMOC_EVT_STATUS_CONSUMED;
        }
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Check if SS_MAIN got deactivated.
      */
      if ( rpt_ptr->param.prot_deactd_ind.prot_state !=
           mmoc_sub_info[rpt_sub]->stack_prop[rpt_stk]->prot_state )
      {
        /* Bogus report.
        */
        MMOC_MSG_MED_2("Invalid report, rpt prot_state %d prot_state %d",
                       rpt_ptr->param.prot_deactd_ind.prot_state,
                       mmoc_sub_info[rpt_sub]->stack_prop[rpt_stk]->prot_state);

        return MMOC_EVT_STATUS_CONSUMED;
      }

      /* SS_MAIN protocol got deactivated.
      */
      else if (rpt_sub == mmoc_dev_info->sub_with_3gpp2)
      {
        mm_id.asubs_id = rpt_sub;
        mm_id.stk_id   = rpt_stk;

        mmoc_dev_info->trans_info.gen_prot_cmd.is_activate_main = TRUE;
        mmoc_update_prot_state( PROT_STATE_NULL,
                                mm_id,
                                mmoc_info_ptr
                              );
      }
      else
      {
        mm_id.asubs_id = rpt_sub;
        mm_id.stk_id = rpt_stk;

        if (mmoc_activate_prot_online_ss_main(sd_ss_act_get_per_mm_id(NULL, mm_id),
                                                mm_id.asubs_id,
                                                mmoc_info_ptr) 
           == MMOC_EVT_STATUS_NOT_CONSUMED)
        {
          return MMOC_EVT_STATUS_NOT_CONSUMED;
        }
      }
    }
    break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case MM_STACK_2:
    {
      /* Check if auto deactivation from HYBR_2 stack */
      if(mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM) &&
          rpt_stk == MM_STACK_2 &&
          mmoc_sub_info[rpt_sub]->stack_prop[rpt_stk]->prot_state == PROT_STATE_ONLINE_GWL)
      {
        /* If HYBR_2 stack should be online, then activate SS_HYBR_2 again. */
        mm_id.asubs_id = rpt_sub;
        mm_id.stk_id   = rpt_stk;
        if(mmoc_is_sxlte(rpt_sub) &&
            mmoc_sub_info[rpt_sub]->active_stacks & MM_STACK_0_MASK)
        {
          /* check main active for SG/SV  + G */
          (void)mmoc_activate_prot_online_ss_hybr_2( sd_ss_act_get_per_mm_id(NULL,mm_id),
              rpt_sub,
              mmoc_info_ptr
                                                   );
        }
#ifdef FEATURE_MMODE_TRIPLE_SIM
        else if(mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY ||
                ((mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY ||
                  mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_SINGLE_STANDBY)&&
                 mmoc_sub_info[rpt_sub]->active_stacks & MM_STACK_2_MASK))
#else
        else if(mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY ||
                (mmoc_dev_info->standby_pref == SYS_MODEM_DS_PREF_SINGLE_STANDBY &&
                 mmoc_sub_info[rpt_sub]->active_stacks & MM_STACK_2_MASK))
#endif
        {
          (void)mmoc_activate_prot_online_ss_hybr_2( sd_ss_act_get_per_mm_id(NULL, mm_id),
              rpt_sub,
              mmoc_info_ptr
                                                   );
        }
        else
        {

          MMOC_ERR_2 ("AUTO_DEACTD_IND from HYBR_2, DS_Pref = %d, active_subs = %d",
                      mmoc_dev_info->standby_pref,
                      mmoc_dev_info->active_subs);

          mmoc_update_prot_state( PROT_STATE_NULL,
                                  mm_id,
                                  mmoc_info_ptr
                                );
        }
        break;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Check if auto deactivation from HYBR_2 stack
      */
      if(mmoc_is_ofmode_1x_sxlte())
      {
        if( rpt_stk == MM_STACK_2 &&
            mmoc_sub_info[rpt_sub]->stack_prop[rpt_stk]->prot_state == PROT_STATE_ONLINE_GWL)
        {
          mm_id.asubs_id = rpt_sub;
          mm_id.stk_id   = rpt_stk;
          (void)mmoc_activate_prot_online_ss_hybr_2(sd_ss_act_get_per_mm_id(NULL, mm_id),
              rpt_sub,
              mmoc_info_ptr);
          break;
        }
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef MMOC_HYBR_SUPPORTED
      /* Check is the SS_HDR protocol is deactivated.
      */
      if ( rpt_ptr->param.prot_deactd_ind.prot_state == PROT_STATE_ONLINE_HDR  &&
           mmoc_sub_info[rpt_sub]->stack_prop[MM_STACK_1]->prot_state == PROT_STATE_ONLINE_HDR )
      {
        /* It is for SS_HDR instance, activate SS_HDR and return.
        */
        (void)mmoc_activate_prot_online_ss_hdr( sd_ss_hdr_act_get(rpt_sub, NULL),
                                                rpt_sub,
                                                mmoc_info_ptr
                                              );
        return MMOC_EVT_STATUS_CONSUMED;
      }
#endif /* MMOC_HYBR_SUPPORTED */

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Check if SS_MAIN got deactivated.
      */
      if ( rpt_ptr->param.prot_deactd_ind.prot_state !=
           mmoc_sub_info[rpt_sub]->stack_prop[MM_STACK_0]->prot_state )
      {
        /* Bogus report.
        */
        MMOC_ERR_2("Invalid report, MAIN prot_state %d rpt prot_state",
                   mmoc_sub_info[rpt_sub]->stack_prop[MM_STACK_0]->prot_state,
                   rpt_ptr->param.prot_deactd_ind.prot_state);
        return MMOC_EVT_STATUS_CONSUMED;
      }
      else
      {
        /* SS_MAIN got deactivated.
        */
        mm_sub_stk_id_s_type mm_id;
        mm_id.asubs_id = rpt_sub;
        mm_id.stk_id   = MM_STACK_0;

        mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.is_activate_main = TRUE;
        mmoc_update_prot_state( PROT_STATE_NULL,
                                mm_id,
                                mmoc_info_ptr
                              );
        return MMOC_EVT_STATUS_CONSUMED;
      }
    }

    case MM_STACK_ALL:
    default:
      sys_err_fatal_invalid_value_exception((int)mmoc_dev_info->trans_info.gen_prot_cmd.cmd_info.stk_id);
      break;

  } /*  switch ( mmoc_info_ptr->trans_info.gen_prot_cmd.cmd_info.ss) */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* End transaction.
  */
  return mmoc_clear_transaction(NULL, mmoc_info_ptr);

} /* mmoc_process_prot_gen_cmd_auto_deactd_ind() */



/*===========================================================================

FUNCTION mmoc_postprocess_prot_gen_cmd

DESCRIPTION
  This function handles the generic protocol command transaction cleanup
  function.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_postprocess_prot_gen_cmd
(
  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  /* Check if MAIN has to be activated. */
  if ( mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.is_activate_main )
  {
    sys_modem_as_id_e_type sub = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.asubs_id;
    mm_sub_stk_id_s_type mm_id = {sub, MM_STACK_0};

    /* Since main is being kicked online turn the flag OFF
    */
    mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.is_activate_main = FALSE;

    /* MAIN needs to be activated.
    */
    if(sub < MAX_SIMS && 
      sub >= SYS_MODEM_AS_ID_1 && 
      mmoc_info_ptr->sub_prop[sub] &&
      mmoc_info_ptr->sub_prop[sub]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_NULL)
    {
      return mmoc_activate_prot_online_ss_main(sd_ss_act_get_per_mm_id( NULL, mm_id ),
             mm_id.asubs_id,
             mmoc_info_ptr
                                              );
    }
  }

  return MMOC_EVT_STATUS_CONSUMED;

} /* mmoc_postprocess_prot_gen_cmd() */


/*===========================================================================

FUNCTION mmoc_postprocess_prot_deact_dormant

DESCRIPTION
  This function handles the MMOC_TRANS_DEACT_FROM_DORMANT cleanup
  function.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_postprocess_prot_deact_dormant
(
  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if MAIN has to be activated.
  */
  if ( mmoc_info_ptr->dev_prop.trans_info.deact_from_dormant.is_activate_main)
  {

    /* Since main is being kicked online turn the flag OFF
    */
    mmoc_info_ptr->dev_prop.trans_info.deact_from_dormant.is_activate_main = FALSE;

    /* MAIN needs to be activated.
    */
    if(mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_NULL)
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
      mm_id.stk_id   = MM_STACK_0;

      return mmoc_activate_prot_online_ss_main(
               sd_ss_act_get_per_mm_id(NULL, mm_id),
               mmoc_info_ptr->dev_prop.sub_with_3gpp2,
               mmoc_info_ptr);
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Return event consumed.
  */
  return MMOC_EVT_STATUS_CONSUMED;

} /* mmoc_postprocess_prot_gen_cmd() */



/*===========================================================================

FUNCTION mmoc_process_prot_gen_cmd

DESCRIPTION
  This function is the transaction handler for the power down transaction.

  The transaction state transition will be,

     NULL <-> GEN_CMD_ENTER -> WAIT_GEN_CMD_CNF  -> NULL
                 ^                 |
                 |                 |
                 ------------------

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_gen_cmd
(

  mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_evt_status_e_type  evt_status = MMOC_EVT_STATUS_CONSUMED;
  prot_gen_cmd_e_type     cmd_type   = PROT_GEN_CMD_MAX;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Invoke the appropriate the transaction state handler.
  */
  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status = mmoc_process_prot_gen_cmd_null( cmd_ptr,
                   mmoc_info_ptr
                                                 );
      break;

    case MMOC_TRANS_STATE_GEN_CMD_ENTER:
      evt_status = mmoc_process_prot_gen_cmd_enter( cmd_ptr,
                   rpt_ptr,
                   mmoc_info_ptr
                                                  );
      break;

    case MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF:
      evt_status = mmoc_process_prot_gen_cmd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                );
      break;


    case MMOC_TRANS_STATE_WAIT_AUTO_DEACTD_IND:
      evt_status = mmoc_process_prot_gen_cmd_auto_deactd_ind( rpt_ptr,
                   mmoc_info_ptr
                                                            );
      break;


    case MMOC_TRANS_STATE_HDR_DEACT_ENTER:
      cmd_type = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.cmd_type;
      evt_status = mmoc_process_common_hdr_deact_enter(
                     (cmd_type == PROT_GEN_CMD_IRAT_TO_GWL ? PROT_DEACT_GWL_REDIR :
                      PROT_DEACT_ACQ_OTHERS),
                     mmoc_info_ptr
                   );
      break;



    case MMOC_TRANS_STATE_WAIT_HDR_DEACTD_CNF:
      evt_status = mmoc_process_common_hdr_deactd_cnf ( rpt_ptr,
                   mmoc_info_ptr
                                                      );
      break;


    case MMOC_TRANS_STATE_HYBR2_DEACT_ENTER:
      evt_status = mmoc_process_common_hybr2_deact_enter
                   ( PROT_DEACT_ACQ_OTHERS,
                     mmoc_info_ptr
                   );
      break;


    case MMOC_TRANS_STATE_WAIT_HYBR2_DEACTD_CNF:
      evt_status = mmoc_process_common_hybr2_deactd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                       );
      break;


    case MMOC_TRANS_STATE_GEN_CMD_ACTIVATION_ENTER:
      evt_status = mmoc_process_prot_gen_activation_enter(mmoc_info_ptr);
      break;



    case MMOC_TRANS_STATE_WAIT_ACTIVATION_CNF:
      evt_status = mmoc_process_prot_gen_cmd_activation_cnf ( rpt_ptr,
                   mmoc_info_ptr
                                                            );
      break;

    case MMOC_TRANS_STATE_WAIT_PS_DETACH_CNF:
      evt_status = mmoc_process_common_ps_detach_cnf( rpt_ptr,
                   mmoc_info_ptr );
      break;

    case MMOC_TRANS_STATE_WAIT_UE_MODE_SWITCH:
      evt_status = mmoc_process_ue_mode_switch_cnf( rpt_ptr,
                   mmoc_info_ptr );
      break;

    case MMOC_TRANS_STATE_WAIT_1XCSFB_DEACT_CNF:
      evt_status = mmoc_process_prot_gen_cmd_csfb_deact_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                           );
      break;

    case MMOC_TRANS_STATE_WAIT_UE_MODE_SWITCH_CNF_1X:
      evt_status = mmoc_process_ue_mode_switch_cnf_1x( rpt_ptr,
                   mmoc_info_ptr
                                                     );
      break;
    case MMOC_TRANS_STATE_WAIT_UE_MODE_SWITCH_CNF_HDR:
      evt_status = mmoc_process_ue_mode_switch_cnf_hdr( rpt_ptr,
                   mmoc_info_ptr
                                                      );
      break;
    #ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE	  
    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL:
	evt_status = mmoc_process_deactd_cnf_for_lmtd_net_sel_mode(rpt_ptr,mmoc_info_ptr);
	break;
    #endif
    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
    case MMOC_TRANS_STATE_WAIT_PH_STAT_CNF:
    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
    case MMOC_TRANS_STATE_PROT_PH_STAT_ENTER:
    case MMOC_TRANS_STATE_MAX:
    default:
      MMOC_ERR_0("Received event in unknown state");
      evt_status = mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
      break;
  }


  return evt_status;

} /* mmoc_process_prot_gen_cmd() */


/*===========================================================================
=============================================================================
=============================================================================
==================== AUTONOMOUS PROTOCOL DEACTIVATED TRANSACTION ============
=============================================================================
=============================================================================
===========================================================================*/



/*===========================================================================

FUNCTION mmoc_process_auto_deactd_ind_null

DESCRIPTION
  This function handles the autonomous protocol deactivated report
  transaction in the NULL transaction state.

  In this state, the state handler will
  1. based on the deactivated reason either call into SD to determine the
     next protocol to be activated or if the deactivated reason is CDMA to
     AMPS handoff, it will activate the AMPS protocol.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_auto_deactd_ind_null
(
  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mm_sub_stk_id_s_type mm_id;

  /* The deactivated report should only create this transaction.
  */
  if ( rpt_ptr == NULL ||
       rpt_ptr->rpt.name != MMOC_RPT_PROT_AUTO_DEACTD_IND
     )
  {
    MMOC_ERR_0("Autonomous deactivated transaction created incorrectly");

    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }
  mm_id.asubs_id = rpt_ptr->param.prot_deactd_ind.mm_id.asubs_id;
  mm_id.stk_id   = rpt_ptr->param.prot_deactd_ind.mm_id.stk_id;

  /* Check if the deactivation was from SS_HDR system selection instance.
   */
  if ( rpt_ptr->param.prot_deactd_ind.prot_state  == PROT_STATE_ONLINE_HDR &&
       mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_1]->prot_state == PROT_STATE_ONLINE_HDR)
  {
    mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
    mm_id.stk_id = MM_STACK_1;
  }

  MMOC_MSG_HIGH_4("auto_deactd_ind_null, sub %d stk %d, reason=%d, prot_state=%d",
                  mm_id.asubs_id,
                  mm_id.stk_id,
                  rpt_ptr->param.prot_deactd_ind.reason,
                  rpt_ptr->param.prot_deactd_ind.prot_state);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if phone is online.
  */
  if(mmoc_info_ptr->dev_prop.oprt_mode != PROT_OPRT_MODE_ONLINE )
  {
    MMOC_ERR_FATAL_0("Phone should be in online mode");
  }

  if(rpt_ptr->param.prot_deactd_ind.prot_state  == PROT_STATE_ONLINE_HDR &&
      rpt_ptr->param.prot_deactd_ind.reason      == PROT_DEACT_REDIR_ABORT &&
      mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_1]->prot_state != PROT_STATE_ONLINE_HDR)
  {
    MMOC_MSG_HIGH_0("Function mmoc_process_auto_deactd_ind_null DO Redir aborted");

    mmoc_info_ptr->dev_prop.is_redir_allowed = FALSE;

#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
    sd_ss_hdr_irat_fail_ind(mmoc_info_ptr->dev_prop.sub_with_3gpp2);
#endif

    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  /* Do not process AUTO_DEACT_IND from 1x, if 1x protocol is still in active state
            * Ideally this should never happen
            */
#ifdef MMOC_CDMA_SUPPORTED
  if ( rpt_ptr->param.prot_deactd_ind.prot_state == PROT_STATE_ONLINE_CDMA &&
       mm_id.stk_id == MM_STACK_0 &&
       mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state == PROT_STATE_ONLINE_CDMA &&
       mc_is_cdma_active() == TRUE )
  {
    MMOC_ERR_0 ("Received AUTO_DEACT_IND from 1x but 1x is still active");

    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize the trans info.
  */
  switch (rpt_ptr->param.prot_deactd_ind.reason)
  {
    case PROT_DEACT_ACQ_OTHERS:
    /* After OTASP commit, 1xCP would deactivate themselves after registering
    ** with network. DEACT_SUBSC_CHGD is used.
    */
    case PROT_DEACT_SUBSC_CHGD:

      /* If hybrid preference is NONE and autonomous deact  indication
      ** is received from protocol for HDR instance, update the state to
      ** NULL and do not try to acquire on Hybrid instance.
      */
      if(mm_id.stk_id == MM_STACK_1 && !sd_misc_is_hybr_opr(mm_id.asubs_id))
      {
        MMOC_ERR_0("Err: Unexp state, hybrid is OFF, MMOC is TRANS_STATE_NULL");

        mmoc_update_prot_state( PROT_STATE_NULL,
                                mm_id,
                                mmoc_info_ptr
                              );
        break;
      }

      /* Activate protocol on ss instance
      */
      if(mmoc_activate_prot_online(mm_id, mmoc_info_ptr) == MMOC_EVT_STATUS_NOT_CONSUMED)
      {
        /* Invoke new transaction.
        */
        return MMOC_EVT_STATUS_NOT_CONSUMED;
      }

      break;

    case PROT_DEACT_PWR_DOWN:
    case PROT_DEACT_RESET:
    case PROT_DEACT_LPM:
    case PROT_DEACT_OFFLINE:
    default:
      MMOC_ERR_1("deact reason %d", rpt_ptr->param.prot_deactd_ind.reason);
      break;
  }  /* switch (rpt_ptr->param.prot_auto_deactd_ind.reason) */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Transaction completed.
  */
  return mmoc_clear_transaction( NULL, mmoc_info_ptr );

} /* mmoc_process_auto_deactd_ind_null() */



/*===========================================================================

FUNCTION mmoc_process_auto_deactd_ind

DESCRIPTION
  This function is the transaction handler for the autonomous protocol
  deactivated indication transaction.

  The transaction state transition will be,

     NULL -> NULL

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_auto_deactd_ind
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_evt_status_e_type evt_status = MMOC_EVT_STATUS_CONSUMED;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Invoke the appropriate transaction state handler.
  */
  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status = mmoc_process_auto_deactd_ind_null( rpt_ptr,
                   mmoc_info_ptr
                                                    );
      break;

    case MMOC_TRANS_STATE_HDR_DEACT_ENTER:
      evt_status = mmoc_process_common_hdr_deact_enter( PROT_DEACT_ACQ_OTHERS,
                   mmoc_info_ptr
                                                      );
      break;

    case MMOC_TRANS_STATE_WAIT_HDR_DEACTD_CNF:
      evt_status = mmoc_process_common_hdr_deactd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                     );
      break;

    case MMOC_TRANS_STATE_HYBR2_DEACT_ENTER:
      evt_status = mmoc_process_common_hybr2_deact_enter
                   ( PROT_DEACT_ACQ_OTHERS,
                     mmoc_info_ptr
                   );
      break;


    case MMOC_TRANS_STATE_WAIT_HYBR2_DEACTD_CNF:
      evt_status = mmoc_process_common_hybr2_deactd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                       );
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
    case MMOC_TRANS_STATE_WAIT_PH_STAT_CNF:
    case MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF:
    case MMOC_TRANS_STATE_GEN_CMD_ENTER:
    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
    case MMOC_TRANS_STATE_PROT_PH_STAT_ENTER:
    case MMOC_TRANS_STATE_MAX:
    default:
      MMOC_ERR_0("Received event in unknown state");
      evt_status = mmoc_clear_transaction( NULL, mmoc_info_ptr );
      break;
  }


  return evt_status;

} /* mmoc_process_auto_deactd_ind() */


/*===========================================================================

FUNCTION mmoc_process_redir_ind_null

DESCRIPTION
  This function handles the protocol redirection report
  transaction in the NULL transaction state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_redir_ind_null
(
  const mmoc_cmd_msg_s_type      *cmd_ptr,
  /* Pointer to the command event that was received.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  MMOC_ASSERT( cmd_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* The auto-activated report should only create this transaction.
  */
  if ( cmd_ptr == NULL ||
       cmd_ptr->cmd.name != MMOC_CMD_PROT_REDIR_IND
     )
  {
    MMOC_ERR_0("Autonomous redirection transaction created incorrectly");

    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  if( !mmoc_info_ptr->dev_prop.is_redir_allowed )
  {
    /* Mark it as Allowed again */
    mmoc_info_ptr->dev_prop.is_redir_allowed = TRUE;

    MMOC_MSG_HIGH_0("IRAT: Redir has been aborted already");

    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if phone is online. If not , clear transaction
  */
  if ( mmoc_info_ptr->dev_prop.oprt_mode != PROT_OPRT_MODE_ONLINE )
  {
    MMOC_MSG_HIGH_0("IRAT:Redir ignore. Phone not online");
    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the activation was received with wrong params
  */
  if (!(cmd_ptr->param.prot_redir_ind.actd_reason  == PROT_ACT_GWL_REDIR &&
        (cmd_ptr->param.prot_redir_ind.prot_state  == PROT_STATE_ONLINE_CDMA ||
         cmd_ptr->param.prot_redir_ind.prot_state  == PROT_STATE_ONLINE_HDR))
     )
  {
    MMOC_ERR_2("IRAT: Redir wrong params, actd_reas=%d prot state=%d",
               cmd_ptr->param.prot_redir_ind.actd_reason,
               cmd_ptr->param.prot_redir_ind.prot_state);

    /* Transaction completed.
    */
    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );

  }

  /* Check if the activation was received in wrong state
  */
  if ((cmd_ptr->param.prot_redir_ind.actd_reason  == PROT_ACT_GWL_REDIR) &&
       (mmoc_info_ptr->sub_prop[cmd_ptr->param.prot_redir_ind.asubs_id] != NULL) &&
       (((mmoc_info_ptr->sub_prop[cmd_ptr->param.prot_redir_ind.asubs_id]->stack_prop[MM_STACK_0] != NULL) &&
          (cmd_ptr->param.prot_redir_ind.prot_state  == \
           mmoc_info_ptr->sub_prop[cmd_ptr->param.prot_redir_ind.asubs_id]->stack_prop[MM_STACK_0]->prot_state) )||
        ((mmoc_info_ptr->sub_prop[cmd_ptr->param.prot_redir_ind.asubs_id]->stack_prop[MM_STACK_1] != NULL) &&
         ( cmd_ptr->param.prot_redir_ind.prot_state  == \
           mmoc_info_ptr->sub_prop[cmd_ptr->param.prot_redir_ind.asubs_id]->stack_prop[MM_STACK_1]->prot_state))
       )
     )
  {
    MMOC_MSG_HIGH_2("IRAT: Redir wrong state, actd_reas=%d prot state=%d",
                     cmd_ptr->param.prot_redir_ind.actd_reason,
                     cmd_ptr->param.prot_redir_ind.prot_state);

    /* Transaction completed.
    */
    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );

  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_MSG_HIGH_2("IRAT: actd_reas=%d prot_state=%d",
                  cmd_ptr->param.prot_redir_ind.actd_reason,
                  cmd_ptr->param.prot_redir_ind.prot_state);

  /* Initialize the transaction info.
  */
  mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.cmd_info =
    cmd_ptr->param.prot_redir_ind;

  /* Move the trans. state to send the Redir Cnf command to the
  ** protocol.
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_REDIR_ENTER,
                           mmoc_info_ptr
                         );


  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_redir_ind_null() */


#if defined (FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
/*===========================================================================

FUNCTION mmoc_process_ho_ind_null

DESCRIPTION
  This function handles the protocol HO report
  transaction in the NULL transaction state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_ho_ind_null
(
  const mmoc_cmd_msg_s_type      *cmd_ptr,
  /* Pointer to the command event that was received.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  MMOC_ASSERT( cmd_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* The auto-activated report should only create this transaction.
  */
  if ( cmd_ptr == NULL ||
       cmd_ptr->cmd.name != MMOC_CMD_PROT_HO_IND
     )
  {
    MMOC_ERR_0("Autonomous HO transaction created incorrectly");

    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  if( !mmoc_info_ptr->dev_prop.is_redir_allowed )
  {
    /* Mark it as Allowed again */
    mmoc_info_ptr->dev_prop.is_redir_allowed = TRUE;

    MMOC_MSG_HIGH_0(" 1XCSFB: HO has been aborted already");

    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if phone is online.
  */
  if ( mmoc_info_ptr->dev_prop.oprt_mode != PROT_OPRT_MODE_ONLINE )
  {
    MMOC_MSG_HIGH_0("Phone should be in online mode");
    /* Transaction completed.
      */
    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the activation was received with wrong params
  */
  if (!(cmd_ptr->param.prot_ho_ind.actd_reason == PROT_ACT_LTE_HANDOVER &&
        cmd_ptr->param.prot_ho_ind.prot_state  == PROT_STATE_ONLINE_CDMA)
     )

  {
    MMOC_ERR_2("1XCSFB HO: wrong params, actd_reas=%d prot state=%d",
               cmd_ptr->param.prot_ho_ind.actd_reason,
               cmd_ptr->param.prot_ho_ind.prot_state);

    /* Transaction completed.
    */
    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_MSG_HIGH_2("1XCSFB HO: actd_reas=%d prot state=%d",
                  cmd_ptr->param.prot_ho_ind.actd_reason,
                  cmd_ptr->param.prot_ho_ind.prot_state);

  /* Initialize the transaction info.
  */
  mmoc_info_ptr->dev_prop.trans_info.ho_ind_info.cmd_info =
    cmd_ptr->param.prot_ho_ind;

  /* Move the trans. state to send the Redir Cnf command to the
  ** protocol.
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_HO_ENTER,
                           mmoc_info_ptr
                         );


  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_redir_ind_null() */

#endif /* FEATURE_LTE_TO_1X */


/*===========================================================================

FUNCTION mmoc_process_prot_redir_enter

DESCRIPTION
  This function is responsible for the sending the Redirection CNF to the
  redirected protocol.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_redir_enter
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mm_sub_stk_id_s_type mm_id;

  if( cmd_ptr == NULL )
  {
    MMOC_ERR_0("NULL ptr rcvd for cmd_ptr");
    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }
  mm_id.asubs_id = cmd_ptr->param.prot_redir_ind.asubs_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the redirection confirmation to the redirected protocol.
  */
  if( cmd_ptr->param.prot_redir_ind.prot_state == PROT_STATE_ONLINE_HDR )
  {
#ifdef MMOC_CDMA_SUPPORTED
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    if(((mmoc_info_ptr->sub_prop[mm_id.asubs_id]->sub_avail & PROT_SUBSC_MASK_3GPP2) == 0) ||
       (mmoc_info_ptr->dev_prop.sub_with_3gpp2 != mm_id.asubs_id) ||
        mcc_is_cdma_call_in_prog(mm_id.asubs_id) == TRUE  || 
        cmcall_is_there_any_cdma_call_per_sub(mm_id.asubs_id) == TRUE
      )
    {

     MMOC_MSG_HIGH_3("asubs_id=%d, subs_avail=%d, sub_with_3gpp2=%d Rejecting L2DO redirection CMD ", mm_id.asubs_id,
        mmoc_info_ptr->sub_prop[mm_id.asubs_id]->sub_avail,mmoc_info_ptr->dev_prop.sub_with_3gpp2);

#if defined(MMOC_HDR_SUPPORTED) && defined(MMOC_LTE_TO_HDR)
      /* 1x call is active, Send REDIR_CNF back to protocol with
         is_irat_ho_allowed as FALSE */

      mmoc_send_hdr_redir_protocol_cnf(
        cmd_ptr->param.prot_redir_ind.actd_reason,FALSE);
#endif

      return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
    }
#endif //    #ifdef MMOC_CDMA_SUPPORTED

    /* Get the SS instance to be used for the mode
    */
    mm_id.stk_id = sd_misc_get_ss_for_mode(mm_id.asubs_id, SYS_SYS_MODE_HDR);

    MMOC_MSG_HIGH_2("IRAT: HDR, sub %d stk %d", mm_id.asubs_id, mm_id.stk_id);

    /* STACK_ALL means hybr pref is NONE and HDR not on stack 0, LTE-HDR redir is expected to fail in SD
    */
    if(mm_id.stk_id == MM_STACK_ALL) mm_id.stk_id = MM_STACK_0;

    if(cmregprx_is_wait_srv_cnf(mm_id.asubs_id) || cmregprx_is_3gpp_inactive(mm_id.asubs_id))
    {
      /* Inform protocol that redirection is rejected
      ** and clear the transaction
      */
      MMOC_MSG_HIGH_0("Rejecting redir req based cmregprx state");
#if defined(MMOC_HDR_SUPPORTED) && defined(MMOC_LTE_TO_HDR)

      mmoc_send_hdr_redir_protocol_cnf(
            cmd_ptr->param.prot_redir_ind.actd_reason,FALSE);
#endif      
      return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
    }


    /* Store this SS info and its protocol state.
    */
    mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id.asubs_id = mm_id.asubs_id;
    mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id.stk_id   = mm_id.stk_id;
    mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.prev_prot_state =
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state;


#if defined(MMOC_HDR_SUPPORTED) && defined(MMOC_LTE_TO_HDR)

    /* Update the protocol state of corresponding MMOC stack */

    if(mm_id.stk_id == MM_STACK_0)
    {
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_dormant =
        ((sd_ss_mode_pref_e_type)((int)mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_dormant |
                                  (int)SD_SS_MODE_PREF_GWL));
    }

    mmoc_update_prot_state( PROT_STATE_ONLINE_HDR,
                            mm_id,
                            mmoc_info_ptr
                          );

    /* Send REDIR_CNF back to protocol
    */
    mmoc_send_hdr_redir_protocol_cnf(
      cmd_ptr->param.prot_redir_ind.actd_reason,
      TRUE);

#endif

    /* Move the trans. state to send the Redir Cnf command to the
    ** protocol.
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_AUTO_ACTD_IND,
                             mmoc_info_ptr
                           );
  }
  else if(cmd_ptr->param.prot_redir_ind.prot_state == PROT_STATE_ONLINE_CDMA)
  {
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    boolean invalid_stack = FALSE;

    /* Get the SS instance to be used for the mode
    */
    mm_id.stk_id = sd_misc_get_ss_for_mode(mm_id.asubs_id, SYS_SYS_MODE_CDMA);

    MMOC_MSG_HIGH_2("IRAT: CDMA, sub %d stk %d", mm_id.asubs_id, mm_id.stk_id);

    /* STACK_ALL means CDMA is not in the mode pref, LTE to CDMA redir should not happen
    */
    if(mm_id.stk_id == MM_STACK_ALL)
    {
      mm_id.stk_id = MM_STACK_0;
      invalid_stack = TRUE;
    }

    /* Store this SS info and its protocol state.
    */
    mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id.asubs_id = mm_id.asubs_id;
    mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id.stk_id   = mm_id.stk_id;
    mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.prev_prot_state =
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state;

#if defined(MMOC_CDMA_SUPPORTED) && defined(MMOC_LTE_TO_1X)

    /* Update the protocol states of relevant MMOC stacks
    */

    if(mm_id.stk_id == MM_STACK_0)
    {
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_dormant =
        ((sd_ss_mode_pref_e_type)((int)mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_dormant |
                                  (int)SD_SS_MODE_PREF_GWL));
    }

    mmoc_update_prot_state( PROT_STATE_ONLINE_CDMA,
                            mm_id,
                            mmoc_info_ptr
                          );


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    if(((mmoc_info_ptr->sub_prop[mm_id.asubs_id]->sub_avail & PROT_SUBSC_MASK_3GPP2) == 0) ||
       (mmoc_info_ptr->dev_prop.sub_with_3gpp2 != mm_id.asubs_id))
    {

      MMOC_MSG_HIGH_3("asubs_id=%d, subs_avail=%d, sub_with_3gpp2=%d Rejecting L21x redirection CMD ", mm_id.asubs_id,
                      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->sub_avail,mmoc_info_ptr->dev_prop.sub_with_3gpp2);

      return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
    }
    /* Send REDIR_CNF back to protocol
    ** Redirection should be rejected if CM is waiting for a service_cnf or the LTE stack is
    ** not active. Redirection should also be rejected if the CDMA stack is ALL which means
    ** that CDMA is not currently in the mode pref
    */
    else if(cmregprx_is_wait_srv_cnf(mm_id.asubs_id) || !(cmregprx_is_gwl_protocol_active(mm_id)) ||
       invalid_stack)
    {
      /* Inform protocol that redirection is rejected
      ** Wait for AUTO_DEACTD_IND which will clear the transaction
      */
      mc_cdma_redir_protocol_cnf(mm_id.asubs_id, cmd_ptr->param.prot_redir_ind.actd_reason, FALSE);
    }
    else
    {
      /* Inform protocol that redirection is allowed
      ** Wait for AUTO_ACTD_IND
      */
      mc_cdma_redir_protocol_cnf(mm_id.asubs_id, cmd_ptr->param.prot_redir_ind.actd_reason, TRUE);
    }
#endif
    /* Move the trans. state to wait for the response (ACTD/DEACTD) from the protocol
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_AUTO_ACTD_IND,
                             mmoc_info_ptr
                           );
  }

  return MMOC_EVT_STATUS_CONSUMED;

} /* mmoc_process_prot_redir_enter() */


#if defined (FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
/*===========================================================================

FUNCTION mmoc_process_prot_ho_enter

DESCRIPTION
  This function is responsible for the sending the HO CNF to the
  1XCP protocol.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_ho_enter
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mm_sub_stk_id_s_type mm_id;

  if( cmd_ptr == NULL )
  {
    MMOC_ERR_0("NULL ptr rcvd for cmd_ptr");
    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }
  mm_id.asubs_id = cmd_ptr->param.prot_ho_ind.asubs_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the redirection confirmation to the redirected protocol.
  */

  if(cmd_ptr->param.prot_ho_ind.prot_state == PROT_STATE_ONLINE_CDMA)
  {
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Get the SS instance to be used for the mode
    */
    mm_id.stk_id = sd_misc_get_ss_for_mode(mm_id.asubs_id, SYS_SYS_MODE_CDMA);

    MMOC_MSG_HIGH_2("HO: sub %d stk %d", mm_id.asubs_id, mm_id.stk_id);

    /* Store this SS info and its protocol state.
    */
    mmoc_info_ptr->dev_prop.trans_info.ho_ind_info.mm_id.asubs_id = mm_id.asubs_id;
    mmoc_info_ptr->dev_prop.trans_info.ho_ind_info.mm_id.stk_id   = mm_id.stk_id;
    mmoc_info_ptr->dev_prop.trans_info.ho_ind_info.prev_prot_state =
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state;

    /* Update the protocol states of relevant MMOC stacks
    */
    if(mm_id.stk_id == MM_STACK_0)
    {
      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_dormant =
        ((sd_ss_mode_pref_e_type)((int)mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_dormant |
                                  (int)SD_SS_MODE_PREF_GWL));
    }

    /* Inform CM of 1X HO
    */
    mmoc_send_cm_rpt( CM_LTE_1X_IRAT_F, NULL, mm_id.asubs_id);

    mmoc_update_prot_state( PROT_STATE_ONLINE_CDMA,
                            mm_id,
                            mmoc_info_ptr
                          );

    /* Move the trans. state to send the Redir Cnf command to the
    ** protocol.
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_DEACT_ENTER,
                             mmoc_info_ptr
                           );
    return MMOC_EVT_STATUS_NOT_CONSUMED;

  }

  return MMOC_EVT_STATUS_CONSUMED;

} /* mmoc_process_prot_ho_enter() */

#endif /* FEATURE_LTE_TO_1X */

/*===========================================================================

FUNCTION mmoc_process_redir_hold_user_act_cnf

DESCRIPTION
  This function is responsible for processing the HOLD_USER_ACT_CNF

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_redir_hold_user_act_cnf
(
  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  if( rpt_ptr == NULL )
  {
    MMOC_ERR_0("NULL ptr rcvd for cmd_ptr");
    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  if (rpt_ptr->rpt.name == MMOC_RPT_PROT_AUTO_DEACTD_IND &&
      mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.cmd_info.prot_state ==
      rpt_ptr->param.prot_deactd_ind.prot_state
     )
  {
    /* If protocol deactivated itself, then update protocol state and
    ** clear the redirection transaction
    */
    mmoc_update_prot_state(
      mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.prev_prot_state,
      mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id,
      mmoc_info_ptr);

    mmoc_update_prot_dormant_state( SD_SS_MODE_PREF_NONE,
                                    mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id,
                                    mmoc_info_ptr
                                  );

    MMOC_MSG_HIGH_2("IRAT: Protocol deactivated, reason %d prot %d",
                    rpt_ptr->param.prot_deactd_ind.reason,
                    rpt_ptr->param.prot_deactd_ind.prot_state);

    /* Transaction cleared.
    */
    return mmoc_clear_transaction(NULL, mmoc_info_ptr);
  }

  /* Process further only if it is AUTO_ACTD_IND that was awaited
  */
  if (rpt_ptr->rpt.name != MMOC_RPT_IRAT_HOLD_USER_ACT_CNF)
  {
    /* Unexpected report received.
    */
    MMOC_ERR_1("unexpected report %d", rpt_ptr->rpt.name);
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /* Process further only if it is AUTO_ACTD_IND that was awaited
  */
  if (rpt_ptr->param.lte_1x_irat_cnf.irat_ret_status != TRUE)
  {
    /* Unexpected report received.
    */
    MMOC_ERR_1("IRAT FAILURE: WAIT for AUTO DEACT IND from 1XCP %d", rpt_ptr->param.lte_1x_irat_cnf.irat_ret_status);
    return MMOC_EVT_STATUS_CONSUMED;
  }

  if ((mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id.stk_id == MM_STACK_0) &&
      (mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_ONLINE_CDMA))
  {
    mmoc_update_trans_state(MMOC_TRANS_STATE_PROT_DEACT_ENTER,
                            mmoc_info_ptr);

    return MMOC_EVT_STATUS_NOT_CONSUMED;
  }

  return mmoc_clear_transaction( NULL, mmoc_info_ptr );

}


/*===========================================================================

FUNCTION mmoc_process_prot_redir_auto_actd_ind

DESCRIPTION
  This function handles the protocol redirection command transaction in the
  WAIT_AUTO_ACTD_IND transaction state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_redir_auto_actd_ind
(
  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  MMOC_ASSERT( rpt_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if we got the autonomous activated report.
  */
  if (rpt_ptr == NULL)
  {
    /* Unexpected report received.
    */
    MMOC_ERR_0("unexpected report");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /* Process the received report.
  */
  if (rpt_ptr->rpt.name == MMOC_RPT_PROT_AUTO_DEACTD_IND)
  {
    if (mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.cmd_info.prot_state ==
        rpt_ptr->param.prot_deactd_ind.prot_state
       )
    {
      MMOC_MSG_HIGH_4("IRAT: Protocol deactivated, reason %d prot %d subs %d ss_prot_state %d",
                      rpt_ptr->param.prot_deactd_ind.reason,
                      rpt_ptr->param.prot_deactd_ind.prot_state,
                      mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id.asubs_id,
                      mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id.asubs_id]->stack_prop[mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id.stk_id]->prot_state );

      /* If redirection was being done from L on H2 to D on H1, send the IRAT
      ** failure to SD.
      */
#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
      if( rpt_ptr->param.prot_deactd_ind.prot_state  == PROT_STATE_ONLINE_HDR
          &&
          mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id.asubs_id]->stack_prop[mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id.stk_id]->prot_state  == rpt_ptr->param.prot_deactd_ind.prot_state)
      {
        sd_ss_hdr_irat_fail_ind(mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id.asubs_id);
      }
#endif
      /* If protocol deactivated itself, then update protocol state and
      ** clear the redirection transaction
      */
      mmoc_update_prot_state(
        mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.prev_prot_state,
        mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id,
        mmoc_info_ptr);

      mmoc_update_prot_dormant_state( SD_SS_MODE_PREF_NONE,
                                      mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id,
                                      mmoc_info_ptr
                                    );

      /* Transaction cleared.
      */
      return mmoc_clear_transaction(NULL, mmoc_info_ptr);
    }

    else
    {
      MMOC_MSG_HIGH_0("Received autonomous deactivated report while waiting for redir auto actd ind");
      return mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);
    }

  }

  /* Process further only if it is AUTO_ACTD_IND that was awaited
  */
  if(rpt_ptr->rpt.name != MMOC_RPT_PROT_AUTO_ACTD_IND)
  {
    /* Unexpected report received.
    */
    MMOC_ERR_1("unexpected report %d", rpt_ptr->rpt.name);
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Process auto actd report based on Redir cmd to SS instance.
  */
  switch(mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id.stk_id)
  {
    case MM_STACK_1:
#ifdef MMOC_HYBR_SUPPORTED
      /* Check if the HDR protocol is deactivated.
      */
      if(rpt_ptr->param.prot_actd_ind.prot_state == PROT_STATE_ONLINE_HDR &&
          mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_1]->prot_state == PROT_STATE_ONLINE_HDR )
      {
        /* HDR hybrid protocol got activated, deactivate GW, if necessary
        */
        MMOC_MSG_HIGH_0 ("IRAT: Hybr stack, AUTO_ACTD_IND for HDR");

        if( mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.cmd_info.actd_reason ==
            PROT_ACT_GWL_REDIR )
        {
          mmoc_update_trans_state(MMOC_TRANS_STATE_PROT_DEACT_ENTER, mmoc_info_ptr);

          return MMOC_EVT_STATUS_NOT_CONSUMED;
        }
      }
#endif /* MMOC_HYBR_SUPPORTED */
      break;

    case MM_STACK_0:

      if((rpt_ptr->param.prot_actd_ind.prot_state == PROT_STATE_ONLINE_HDR &&
          mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_ONLINE_HDR) ||
          (rpt_ptr->param.prot_actd_ind.prot_state == PROT_STATE_ONLINE_CDMA &&
           mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_ONLINE_CDMA)
        )
      {
        /* 1x/HDR protocol got activated, deactivate GWL, if necessary
        */
        MMOC_MSG_HIGH_1 ("IRAT: Main stack, AUTO_ACTD_IND for prot state=%d",
                         rpt_ptr->param.prot_actd_ind.prot_state);

        if(mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.cmd_info.actd_reason == PROT_ACT_GWL_REDIR)
        {
          /* Inform CM of 1X redirection
          */
          if(rpt_ptr->param.prot_actd_ind.prot_state == PROT_STATE_ONLINE_CDMA)
          {
            mmoc_send_cm_rpt( CM_LTE_1X_IRAT_F, NULL,mmoc_info_ptr->dev_prop.sub_with_3gpp2);
            mmoc_send_cm_rpt( CM_IRAT_HOLD_USER_ACT, NULL,mmoc_info_ptr->dev_prop.sub_with_3gpp2);
            mmoc_update_trans_state(MMOC_TRANS_STATE_WAIT_HOLD_USER_ACT_CNF,
                                    mmoc_info_ptr);
            return MMOC_EVT_STATUS_CONSUMED;
          }
          else
          {
            mmoc_update_trans_state(MMOC_TRANS_STATE_PROT_DEACT_ENTER,
                                    mmoc_info_ptr);

            return MMOC_EVT_STATUS_NOT_CONSUMED;
          }
        }
      }

      break;

    default:
      sys_err_fatal_invalid_value_exception((int)mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.mm_id.stk_id);
      break;

  } /*  switch ( mmoc_info_ptr->trans_info.redir_ind_info.cmd_info.ss) */

  MMOC_ERR_0("Erroneous condition");

  return mmoc_clear_transaction( NULL, mmoc_info_ptr );

} /* mmoc_process_prot_redir_auto_actd_ind() */


/*===========================================================================

FUNCTION mmoc_process_redir_ind_deact_enter

DESCRIPTION
  This is a function for handling the transaction state DEACT_ENTER for the
  Redir indication transaction. It sends deact request to LTE.
  Unlike mmoc_process_common_deact_enter it need not deactivate
  all active protocols

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Should wait for deactivated confirmation.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start further processing.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_redir_ind_deact_enter
(
  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  boolean                        is_deact_req_sent = FALSE;
  prot_deact_e_type              deact_reason = PROT_DEACT_GWL_REDIR;
  multimode_stack_e_type         stack        = MM_STACK_NONE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(mmoc_is_1x_sxlte(mmoc_info_ptr->dev_prop.sub_with_3gpp2) &&
      mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_2]->prot_state == PROT_STATE_ONLINE_GWL )
  {
    stack = MM_STACK_2;
    MMOC_MSG_HIGH_1("SC_SVLTE:redir deactivate stack %d", stack);
  }
  else
  {
    stack = MM_STACK_0;
  }

  if( mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.cmd_info.actd_reason ==
      PROT_ACT_GWL_REDIR )
  {
    /* Deactivate GWL protocol if it is Active or dormant
    */
    if(mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[stack]->prot_state == PROT_STATE_ONLINE_GWL)
    {
      mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
      mmoc_info_ptr->dev_prop.deact_req_idx.stk_id   = stack;

      is_deact_req_sent = mmoc_deactivate_active_protocol (
                            mmoc_info_ptr->dev_prop.deact_req_idx,
                            deact_reason,
                            mmoc_info_ptr);
    }
    else
    {
      /* Deactivate dormant LTE */
      mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
      mmoc_info_ptr->dev_prop.deact_req_idx.stk_id   = stack;

      is_deact_req_sent = mmoc_deactivate_dormant_protocol(
                            mmoc_info_ptr->dev_prop.deact_req_idx,
                            deact_reason,
                            mmoc_info_ptr);
    }

    if (is_deact_req_sent)
    {
      mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DEACTD_CNF,
                               mmoc_info_ptr
                             );

      return MMOC_EVT_STATUS_CONSUMED;
    }


#if defined(MMOC_HDR_SUPPORTED) && defined(MMOC_LTE_TO_HDR)
    if( mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.cmd_info.prot_state ==
        PROT_STATE_ONLINE_HDR )
    {
      /* Protocol is sent activation confirmation
      */
      mmoc_send_hdr_protocol_activate_cnf( PROT_ACT_GWL_REDIR );

      /* Inform CM of redirection
      */
      mmoc_send_cm_rpt( CM_LTE_DO_IRAT_F, NULL,mmoc_info_ptr->dev_prop.sub_with_3gpp2 );

      /* Clear the LTE srv status in SD/CM/clients
      */
      {
        mm_sub_stk_id_s_type mm_id;
        mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
        mm_id.stk_id = sd_misc_get_ss_for_mode(mm_id.asubs_id, SYS_SYS_MODE_LTE);
        sd_ss_ind_misc_srv_lost(mm_id);
      }
    }
#endif

#if defined(MMOC_CDMA_SUPPORTED) && defined(MMOC_LTE_TO_1X)
    if( mmoc_info_ptr->dev_prop.trans_info.redir_ind_info.cmd_info.prot_state ==
        PROT_STATE_ONLINE_CDMA )
    {
      /* Protocol is sent activation confirmation
      */
      mc_cdma_activate_protocol_cnf( mmoc_info_ptr->dev_prop.sub_with_3gpp2, PROT_ACT_GWL_REDIR );
      mmoc_send_cm_rpt( CM_IRAT_UNHOLD_USER_ACT, NULL,mmoc_info_ptr->dev_prop.sub_with_3gpp2);
    }
#endif

  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Transaction completed.
  */
  return mmoc_clear_transaction( NULL, mmoc_info_ptr );

}


#if defined (FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
/*===========================================================================

FUNCTION mmoc_process_ho_ind_deact_enter

DESCRIPTION
  This is a function for handling the transaction state DEACT_ENTER for the
  HO indication transaction. It sends deact request to LTE.
  Unlike mmoc_process_common_deact_enter it need not deactivate
  all active protocols

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Should wait for deactivated confirmation.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start further processing.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_ho_ind_deact_enter
(
  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  boolean                        is_deact_req_sent = FALSE;
  prot_deact_e_type              deact_reason = PROT_DEACT_GWL_REDIR;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(mmoc_info_ptr->dev_prop.trans_info.ho_ind_info.cmd_info.actd_reason ==
      PROT_ACT_LTE_HANDOVER)
  {
    /* Deactivate GWL protocol if it is Active or dormant
    */
    if( mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.trans_info.ho_ind_info.mm_id.asubs_id]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_ONLINE_GWL
      )
    {
      mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id = mmoc_info_ptr->dev_prop.trans_info.ho_ind_info.mm_id.asubs_id;
      mmoc_info_ptr->dev_prop.deact_req_idx.stk_id   = MM_STACK_0;

      is_deact_req_sent = mmoc_deactivate_active_protocol (
                            mmoc_info_ptr->dev_prop.deact_req_idx,
                            deact_reason,
                            mmoc_info_ptr
                          );

    }
    else if (mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.trans_info.ho_ind_info.mm_id.asubs_id]->stack_prop[MM_STACK_0]->prot_dormant != SD_SS_MODE_PREF_NONE)
    {
      /* Deactivate dormant LTE */
      mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id = mmoc_info_ptr->dev_prop.trans_info.ho_ind_info.mm_id.asubs_id;
      mmoc_info_ptr->dev_prop.deact_req_idx.stk_id   = MM_STACK_0;
      is_deact_req_sent = mmoc_deactivate_dormant_protocol(
                            mmoc_info_ptr->dev_prop.deact_req_idx,
                            deact_reason,
                            mmoc_info_ptr);
    }

    if (is_deact_req_sent)
    {
      mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DEACTD_CNF,
                               mmoc_info_ptr
                             );

      return MMOC_EVT_STATUS_CONSUMED;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Transaction completed.
  */
  return mmoc_clear_transaction( NULL, mmoc_info_ptr );

}
#endif /* FEATURE_LTE_TO_1X */


/*===========================================================================

FUNCTION mmoc_process_prot_redir_ind

DESCRIPTION
  This function is the transaction handler for the autonomous protocol
  redirection indication transaction.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_redir_ind
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_evt_status_e_type evt_status = MMOC_EVT_STATUS_CONSUMED;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Invoke the appropriate transaction state handler.
  */
  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status = mmoc_process_prot_redir_ind_null( cmd_ptr,
                   mmoc_info_ptr
                                                   );
      break;

    case MMOC_TRANS_STATE_PROT_REDIR_ENTER:
      evt_status = mmoc_process_prot_redir_enter( cmd_ptr,
                   mmoc_info_ptr
                                                );
      break;

    case MMOC_TRANS_STATE_WAIT_AUTO_ACTD_IND:
      evt_status = mmoc_process_prot_redir_auto_actd_ind( rpt_ptr,
                   mmoc_info_ptr
                                                        );
      break;

    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
      evt_status = mmoc_process_redir_ind_deact_enter( mmoc_info_ptr );
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
      evt_status = mmoc_process_common_deactd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                 );
      break;

    case MMOC_TRANS_STATE_WAIT_HOLD_USER_ACT_CNF:
      evt_status = mmoc_process_redir_hold_user_act_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                       );
      break;
    default:
      MMOC_ERR_0("Received event in unknown state");
      evt_status = mmoc_clear_transaction( NULL, mmoc_info_ptr );
      break;
  }

  return evt_status;

} /* mmoc_process_prot_redir_ind() */


#if defined (FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
/*===========================================================================

FUNCTION mmoc_process_prot_ho_ind

DESCRIPTION
  This function is the transaction handler for the autonomous protocol
  handover indication transaction.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_prot_ho_ind
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_evt_status_e_type evt_status = MMOC_EVT_STATUS_CONSUMED;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Invoke the appropriate transaction state handler.
  */
  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status = mmoc_process_prot_ho_ind_null( cmd_ptr,
                   mmoc_info_ptr
                                                );
      break;

    case MMOC_TRANS_STATE_PROT_HO_ENTER:
      evt_status = mmoc_process_prot_ho_enter( cmd_ptr,
                   mmoc_info_ptr
                                             );
      break;


    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
      evt_status = mmoc_process_ho_ind_deact_enter( mmoc_info_ptr );
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
      evt_status = mmoc_process_common_deactd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                 );
      break;

    default:
      MMOC_ERR_0("Received event in unknown state");
      evt_status = mmoc_clear_transaction( NULL, mmoc_info_ptr );
      break;
  }

  return evt_status;

} /* mmoc_process_prot_ho_ind() */

#endif /* FEATURE_LTE_TO_1X */

/*===========================================================================
=============================================================================
=============================================================================
==================== MMGSDI_INFO_IND TRANSACTION ============================
=============================================================================
=============================================================================
===========================================================================*/


/*===========================================================================

FUNCTION mmoc_process_mmgsdi_info_ind

DESCRIPTION
  This function is the transaction handler for MMGSDI INFO indication
  transaction.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_mmgsdi_info_ind
(
  const mmoc_cmd_msg_s_type      *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_evt_status_e_type evt_status = MMOC_EVT_STATUS_CONSUMED;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Invoke the appropriate transaction state handler defined for MMGSDI.
  */
  evt_status = mmocmmgsdi_process_mmgsdi_info_ind (cmd_ptr,
               rpt_ptr,
               mmoc_info_ptr);

  return evt_status;

} /* mmoc_process_mmgsdi_info_ind() */



/*===========================================================================
=============================================================================
=============================================================================
==================== POWER SAVE ENTER TRANSACTION ===========================
=============================================================================
=============================================================================
===========================================================================*/


/*===========================================================================

FUNCTION mmoc_process_pwr_save_enter_null

DESCRIPTION
  This function handles the power save transaction in the NULL transaction
  state.

  In this state, the state handler will
  1. Check if GPIO allows phone to go in power save mode, if so informs
     SD of power save enter and calls into SRCH or Sleep Controller.
     to send the phone to power save mode.
  2. If power save mode is not allowed, follows SD actions and activates the
     next protocol.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_pwr_save_enter_null
(
  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mm_sub_stk_id_s_type mm_id;
  mm_id.asubs_id = SYS_MODEM_AS_ID_1;
  mm_id.stk_id   = MM_STACK_0;

#ifndef TEST_FRAMEWORK
#ifndef FEATURE_MMOC_UNIT_TEST
#ifndef FEATURE_DISABLE_DEEP_SLEEP
  /* Change in bio(Basic IP/OP) interface.
  ** Now MMOC will use DAL interface instead of bio.
  */
  uint32           sw_switch9_state    = 0;

  if (switches_handle == NULL)
  {
    switches_dal_attach =
      DAL_DeviceAttach(DALDEVICEID_SOFTWARESWITCHES,&switches_handle);
  }

  if((switches_dal_attach == DAL_SUCCESS) && (switches_handle != NULL))
  {
    (void)DalSoftwareSwitches_GetMask(
      switches_handle,(uint32)DAL_SOFTWARE_SWITCH_9,&sw_switch9_state);
  }
#endif /* FEATURE_DISABLE_DEEP_SLEEP */
#endif /* FEATURE_MMOC_UNIT_TEST */
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Phone should be in ONLINE mode and SD action should be power save.
  */
  if ( (mmoc_info_ptr->dev_prop.oprt_mode != PROT_OPRT_MODE_ONLINE) &&
       (sd_ss_act_get_per_mm_id( NULL, mm_id ) !=  SD_SS_ACT_PWR_SAVE )
     )
  {
    MMOC_ERR_FATAL_0("Phone should be in online mode");
  }


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifndef TEST_FRAMEWORK
#ifndef FEATURE_MMOC_UNIT_TEST

  MMOC_MSG_HIGH_1("sw_switch9_state = 0x%x",sw_switch9_state);
  /* Check on GPIO to see if power save mode is allowed.
  */
  if (
#ifndef FEATURE_DISABLE_DEEP_SLEEP
    sw_switch9_state  &&
#endif
    sd_ss_ind_user_pwr_save_exit( SYS_MODEM_AS_ID_1, NULL ) != SD_SS_ACT_PWR_SAVE
  )

  {
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Power save mode is not allowed and SD can activate a new protocol.
    ** Aborted power save enter, activate the protocol or invoke
    ** transaction handler if this function returns EVT_STATUS_NOT_CONSUMED.
    */
    if ( mmoc_activate_prot_online_ss_main( sd_ss_act_get_per_mm_id( NULL, mm_id ),
                                            SYS_MODEM_AS_ID_1,
                                            mmoc_info_ptr
                                          )
         == MMOC_EVT_STATUS_NOT_CONSUMED
       )
    {
      /* Invoke the new transaction handler.
      */
      return MMOC_EVT_STATUS_NOT_CONSUMED;

    }


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


    /* Clear the transaction and return
    */
    return mmoc_clear_transaction( NULL, mmoc_info_ptr );

  } /* if (sw_switch9_state) */

#endif /* #if FEATURE_MMOC_UNIT_TEST */
#endif


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Update the MMoC's protocol state and clear the transaction.
  */
  mmoc_update_prot_state( PROT_STATE_PWR_SAVE,
                          mm_id,
                          mmoc_info_ptr
                        );

  return mmoc_clear_transaction( NULL, mmoc_info_ptr );

} /* mmoc_process_pwr_save_enter_null() */



/*===========================================================================

FUNCTION mmoc_process_pwr_save_enter

DESCRIPTION
  This function is the transaction handler for the power save enter
  transaction.

  The transaction state transition will be,

     NULL -> NULL

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_pwr_save_enter
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_evt_status_e_type evt_status = MMOC_EVT_STATUS_CONSUMED;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Invoke the current transaction state handler.
  */
  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status = mmoc_process_pwr_save_enter_null(mmoc_info_ptr);
      break;

    case MMOC_TRANS_STATE_HDR_DEACT_ENTER:
      evt_status = mmoc_process_common_hdr_deact_enter( PROT_DEACT_ACQ_OTHERS,
                   mmoc_info_ptr
                                                      );
      break;



    case MMOC_TRANS_STATE_WAIT_HDR_DEACTD_CNF:
      evt_status = mmoc_process_common_hdr_deactd_cnf ( rpt_ptr,
                   mmoc_info_ptr
                                                      );
      break;

    case MMOC_TRANS_STATE_HYBR2_DEACT_ENTER:
      evt_status = mmoc_process_common_hybr2_deact_enter( PROT_DEACT_ACQ_OTHERS,
                   mmoc_info_ptr
                                                        );
      break;


    case MMOC_TRANS_STATE_WAIT_HYBR2_DEACTD_CNF:
      evt_status = mmoc_process_common_hybr2_deactd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                       );
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
    case MMOC_TRANS_STATE_WAIT_PH_STAT_CNF:
    case MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF:
    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
    case MMOC_TRANS_STATE_PROT_PH_STAT_ENTER:
    case MMOC_TRANS_STATE_MAX:
    case MMOC_TRANS_STATE_GEN_CMD_ENTER:
    case MMOC_TRANS_STATE_WAIT_AUTO_DEACTD_IND:
    default:
      MMOC_ERR_0("Received event in unknown state");
      evt_status = mmoc_clear_transaction( NULL, mmoc_info_ptr );
      break;
  }


  return evt_status;

} /* mmoc_process_pwr_save_enter() */





/*===========================================================================
=============================================================================
=============================================================================
==================== SUSPEND SS TRANSACTION =================================
=============================================================================
=============================================================================
===========================================================================*/


/*===========================================================================

FUNCTION mmoc_process_suspend_stack_null

DESCRIPTION
  This function handles the suspend_stack transaction in the NULL
  transaction state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_suspend_stack_null
(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */


  mmoc_state_info_s_type              *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type asubs_id;
  multimode_stack_e_type stk_id;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  if( cmd_ptr == NULL )
  {
    MMOC_ERR_0("NULL ptr recvd for cmd_ptr");
    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }
  asubs_id = cmd_ptr->param.suspend_stack.mm_id.asubs_id;
  stk_id = cmd_ptr->param.suspend_stack.mm_id.stk_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(!(stk_id >= MM_STACK_0 && stk_id < MM_STACK_ALL) ||
      !(asubs_id >= SYS_MODEM_AS_ID_1 && asubs_id < MAX_SIMS))
  {
    /* Transaction created incorrectly.
    */
    MMOC_ERR_0("wrong trans");
    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Only the Suspend SS command from CM should create this transaction */

  if ( cmd_ptr->cmd.name != MMOC_CMD_SUSPEND_STACK )
  {
    /* Transaction created incorrectly.
    */
    MMOC_ERR_0("suspend stack trans incorrectly initiated");
    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If CM has already initiated SUSPEND_STACK transacation initialize
  ** MMOC state information and return. */

  if(((mmoc_sub_info[asubs_id]->stack_prop[stk_id]->is_suspend == \
      cmd_ptr->param.suspend_stack.is_suspend) &&
      (mmoc_sub_info[asubs_id]->stack_prop[stk_id]->suspend_reas_mask & \
       BM(cmd_ptr->param.suspend_stack.susp_reason))) ||
      cmd_ptr->param.suspend_stack.ignore_protocol_activate)
  {

    /* ignore_protocol_activate will inform if CM has already sent indication to
     * activate Protocol.
     */
    MMOC_MSG_HIGH_5("SUSPEND_RESUME: SUSPEND_STACK: stack already suspended with same reas \
                    sub %d stk %d is_suspend %d susp_reas %d ignore_protocol_activate %d",
                    asubs_id,
                    stk_id,
                    cmd_ptr->param.suspend_stack.is_suspend,
                    cmd_ptr->param.suspend_stack.susp_reason,
                    cmd_ptr->param.suspend_stack.ignore_protocol_activate);

    /* Inform CM of suspend immediately
    */
    {
      cm_suspend_ss_s_type    suspend_stack;
      suspend_stack.is_suspend = cmd_ptr->param.suspend_stack.is_suspend;
      suspend_stack.stack_id   = stk_id;
      mmoc_sub_info[asubs_id]->stack_prop[stk_id]->is_suspend = cmd_ptr->param.suspend_stack.is_suspend;
      mmoc_sub_info[asubs_id]->stack_prop[stk_id]->suspend_reas_mask &=
        ~(BM(mmoc_dev_info->trans_info.suspend_stack.susp_reason));
      mmoc_send_cm_rpt( CM_SUSPEND_SS_F, (cm_sd_rpt_u_type *)&suspend_stack,asubs_id );
    }
    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Copy transaction info */

  mmoc_dev_info->trans_info.suspend_stack.mm_id.asubs_id = cmd_ptr->param.suspend_stack.mm_id.asubs_id;
  mmoc_dev_info->trans_info.suspend_stack.mm_id.stk_id = cmd_ptr->param.suspend_stack.mm_id.stk_id;
  mmoc_dev_info->trans_info.suspend_stack.is_suspend = cmd_ptr->param.suspend_stack.is_suspend;
  mmoc_dev_info->trans_info.suspend_stack.susp_reason = cmd_ptr->param.suspend_stack.susp_reason;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (cmd_ptr->param.suspend_stack.is_suspend)
  {
    mmoc_update_trans_state( MMOC_TRANS_STATE_SUSPEND_STACK_ENTER,
                             mmoc_info_ptr
                           );

  }
  else
  {
    mmoc_update_trans_state( MMOC_TRANS_STATE_RESUME_STACK_ENTER,
                             mmoc_info_ptr
                           );
  }

  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_suspend_stack_null() */

/*===========================================================================

FUNCTION mmoc_process_suspend_stack_enter

DESCRIPTION
  This function handles the suspend_ss transaction in the SUSPEND_SS_ENTER
  transaction state.

  In this state, the state handler will deactivate the protocol.
  After getting confirmation, a report will be sent to CM confirming that the
  protocol has been deactivated.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_suspend_stack_enter
(

  mmoc_state_info_s_type              *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type asubs_id = mmoc_dev_info->trans_info.suspend_stack.mm_id.asubs_id;
  multimode_stack_e_type stk_id = mmoc_dev_info->trans_info.suspend_stack.mm_id.stk_id;
  prot_deact_e_type               deact_reason = PROT_DEACT_ACQ_OTHERS;
  boolean                         is_deact_req_sent = FALSE;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(asubs_id >= SYS_MODEM_AS_ID_1 && asubs_id < MAX_SIMS &&
      stk_id >= MM_STACK_0 && stk_id < MAX_STACKS)
  {
    /* Update the deact reason if prot is GWL */

    if(mmoc_sub_info[asubs_id]->stack_prop[stk_id]->prot_state == PROT_STATE_ONLINE_GWL)
    {
      deact_reason = mmoc_dev_info->trans_info.suspend_stack.susp_reason;
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    MMOC_MSG_HIGH_5("SUSPEND_RESUME: SUSPEND_STACK: enter susp_reason %d deact_reason %d sub %d stk %d, state %d",
                    mmoc_dev_info->trans_info.suspend_stack.susp_reason,
                    deact_reason,
                    mmoc_dev_info->trans_info.suspend_stack.mm_id.asubs_id,
                    mmoc_dev_info->trans_info.suspend_stack.mm_id.stk_id,
                    mmoc_sub_info[asubs_id]->stack_prop[stk_id]->prot_state);


    /* Send deact request if protocol is active
    */
    mmoc_dev_info->deact_req_idx = mmoc_dev_info->trans_info.suspend_stack.mm_id;
    is_deact_req_sent = mmoc_deactivate_active_protocol
                        ( mmoc_dev_info->deact_req_idx,
                          deact_reason,
                          mmoc_info_ptr
                        );

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Wait for confirmation if deact request has been sent */

    if (is_deact_req_sent)
    {
      mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DEACTD_CNF,
                               mmoc_info_ptr
                             );

      /* Suppress no srv or power save srv indication from SD so that the sub will show
      ** previous srv status during emergency call on the other sub
      */
      if( mmoc_dev_info->trans_info.suspend_stack.susp_reason == PROT_DEACT_DSDX_SUSP)
      {
        sd_ss_set_suppress_no_srv_due_to_suspend(asubs_id, TRUE);
      }
      
      /* Since SS has been deactivated , send indication to SD that service
      ** is lost. SD will then inform CM of srv loss
      */
      sd_ss_ind_misc_srv_lost(mmoc_dev_info->trans_info.suspend_stack.mm_id);

      return MMOC_EVT_STATUS_CONSUMED;
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Inform CM of suspend */

    {
      cm_suspend_ss_s_type    suspend_stack;
      suspend_stack.is_suspend = TRUE;
      suspend_stack.stack_id = mmoc_dev_info->trans_info.suspend_stack.mm_id.stk_id;
      mmoc_send_cm_rpt( CM_SUSPEND_SS_F, (cm_sd_rpt_u_type *)&suspend_stack,asubs_id );
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Update state in MMOC
    */
    mmoc_sub_info[asubs_id]->stack_prop[stk_id]->is_suspend = TRUE;
    mmoc_sub_info[asubs_id]->stack_prop[stk_id]->suspend_reas_mask |= 
                               BM(mmoc_dev_info->trans_info.suspend_stack.susp_reason);
    MMOC_MSG_HIGH_3("SUSPEND_RESUME: SUSPEND_STACK: suspend_reas_mask %d for sub %d stk_id %d",
                    mmoc_sub_info[asubs_id]->stack_prop[stk_id]->suspend_reas_mask,
                    asubs_id, stk_id);

  }
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Clear the transaction. */

  return mmoc_clear_transaction( NULL, mmoc_info_ptr );

} /* mmoc_process_suspend_stack_enter() */


/*===========================================================================

FUNCTION mmoc_process_resume_stack_enter

DESCRIPTION
  This function handles the suspend_ss transaction in the RESUME_SS_ENTER
  transaction state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_resume_stack_enter
(

  mmoc_state_info_s_type              *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type asubs_id = mmoc_dev_info->trans_info.suspend_stack.mm_id.asubs_id;
  multimode_stack_e_type stk_id = mmoc_dev_info->trans_info.suspend_stack.mm_id.stk_id;
  mmoc_evt_status_e_type        evt_status = MMOC_EVT_STATUS_CONSUMED;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(asubs_id >= SYS_MODEM_AS_ID_1 && asubs_id < MAX_SIMS &&
      stk_id >= MM_STACK_0 && stk_id < MAX_STACKS)
  {
    /* Update state in MMOC */

    mmoc_sub_info[asubs_id]->stack_prop[stk_id]->suspend_reas_mask &=
        ~(BM(mmoc_dev_info->trans_info.suspend_stack.susp_reason));

    MMOC_MSG_HIGH_4("SUSPEND_RESUME: RESUME_STACK: enter suspended reason %d suspend_reas_mask %d sub %d stk %d",
                    mmoc_dev_info->trans_info.suspend_stack.susp_reason,
                    mmoc_sub_info[asubs_id]->stack_prop[stk_id]->suspend_reas_mask,
                    asubs_id,
                    stk_id);

    if( mmoc_sub_info[asubs_id]->stack_prop[stk_id]->suspend_reas_mask == 0 )
    {
      mmoc_sub_info[asubs_id]->stack_prop[stk_id]->is_suspend = FALSE;

    if( mmoc_dev_info->trans_info.suspend_stack.susp_reason == PROT_DEACT_DSDX_SUSP)
    {

      sd_ss_set_suppress_no_srv_due_to_suspend(asubs_id, FALSE);

      /* SD core is not in sync with CM. Power up SD so that it is aligned*/
      if(mmoc_dev_info->active_subs & BM(asubs_id))
      {
        (void)sd_ss_ind_user_pwr_up(FALSE, asubs_id, NULL);
      }
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Activate the protocol.
    */
    evt_status = mmoc_activate_prot_online( mmoc_dev_info->trans_info.suspend_stack.mm_id,
                                            mmoc_info_ptr );

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Always inform CM of resume even without waiting for protocol to completely
    ** being brought ONLINE, because at this point, Turbo Decoder has been released
    ** and no conflict would happened.
    */
    {
      cm_suspend_ss_s_type    suspend_stack;
      suspend_stack.is_suspend = FALSE;
      suspend_stack.stack_id = mmoc_dev_info->trans_info.suspend_stack.mm_id.stk_id;
      mmoc_send_cm_rpt( CM_SUSPEND_SS_F, (cm_sd_rpt_u_type *)&suspend_stack,asubs_id );
    }

    if ( evt_status == MMOC_EVT_STATUS_NOT_CONSUMED )
    {
      /* Invoke new transaction.
      */
      return MMOC_EVT_STATUS_NOT_CONSUMED;
    }
  }
    else
    {
      cm_suspend_ss_s_type    suspend_stack;
      suspend_stack.is_suspend = TRUE;
      suspend_stack.stack_id = mmoc_dev_info->trans_info.suspend_stack.mm_id.stk_id;
      mmoc_send_cm_rpt( CM_SUSPEND_SS_F, (cm_sd_rpt_u_type *)&suspend_stack,asubs_id );
    }
  }
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Clear the transaction.
  */
  return mmoc_clear_transaction( NULL, mmoc_info_ptr );

} /* mmoc_process_resume_stack_enter() */



/*===========================================================================

FUNCTION mmoc_process_suspend_stack

DESCRIPTION
  This function is the transaction handler for the wake up from pwr save
  transaction.

  The transaction state transition will be,

     NULL -> NULL

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_suspend_stack
(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type               *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_evt_status_e_type evt_status = MMOC_EVT_STATUS_CONSUMED;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Invoke the current transaction state handler.
  */
  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status = mmoc_process_suspend_stack_null( cmd_ptr,
                   mmoc_info_ptr
                                                  );
      break;

    case MMOC_TRANS_STATE_SUSPEND_STACK_ENTER:
    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
      evt_status = mmoc_process_suspend_stack_enter(mmoc_info_ptr);
      break;


    case MMOC_TRANS_STATE_RESUME_STACK_ENTER:
      evt_status = mmoc_process_resume_stack_enter(mmoc_info_ptr);
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
      evt_status = mmoc_process_common_deactd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                 );
      break;


    case MMOC_TRANS_STATE_HDR_DEACT_ENTER:
      evt_status = mmoc_process_common_hdr_deact_enter
                   ( PROT_DEACT_ACQ_OTHERS,
                     mmoc_info_ptr
                   );
      break;


    case MMOC_TRANS_STATE_WAIT_HDR_DEACTD_CNF:
      evt_status = mmoc_process_common_hdr_deactd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                     );
      break;


    case MMOC_TRANS_STATE_HYBR2_DEACT_ENTER:
      evt_status = mmoc_process_common_hybr2_deact_enter
                   ( PROT_DEACT_ACQ_OTHERS,
                     mmoc_info_ptr
                   );
      break;


    case MMOC_TRANS_STATE_WAIT_HYBR2_DEACTD_CNF:
      evt_status = mmoc_process_common_hybr2_deactd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                       );
      break;


    case MMOC_TRANS_STATE_WAIT_PH_STAT_CNF:
    case MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF:
    case MMOC_TRANS_STATE_PROT_PH_STAT_ENTER:
    case MMOC_TRANS_STATE_MAX:
    case MMOC_TRANS_STATE_GEN_CMD_ENTER:
    case MMOC_TRANS_STATE_WAIT_AUTO_DEACTD_IND:
    default:
      MMOC_ERR_0("Received event in unknown state");
      evt_status = mmoc_clear_transaction( NULL, mmoc_info_ptr );
      break;
  }


  return evt_status;

} /* mmoc_process_suspend_stack() */

/*===========================================================================

FUNCTION mmoc_process_dual_standby_chgd

DESCRIPTION
  This function is the transaction handler for the dual standby pref changed
  transaction.

  The transaction state transition will be, standby preference is updated

 NULL -> NULL

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_dual_standby_chgd
(

  const mmoc_cmd_msg_s_type      *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_evt_status_e_type evt_status = MMOC_EVT_STATUS_CONSUMED;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Invoke the appropriate transaction state handlers.
  */
  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status = mmoc_process_dual_standby_chgd_null(cmd_ptr,
                   mmoc_info_ptr);
      break;

    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
      evt_status = mmoc_process_dual_standby_chgd_deact_enter(mmoc_info_ptr);
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
      evt_status = mmoc_process_common_deactd_cnf(rpt_ptr, mmoc_info_ptr);
      break;

    case MMOC_TRANS_STATE_DS_STAT_CHGD_ENTER:
      evt_status = mmoc_process_common_ds_stat_chgd_enter(cmd_ptr, mmoc_info_ptr);
      break;

    case MMOC_TRANS_STATE_WAIT_DS_STAT_CHGD_CNF:
      evt_status = mmoc_process_common_ds_stat_chgd_cnf(rpt_ptr,mmoc_info_ptr);
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL:
      evt_status = mmoc_process_deactd_cnf_gwl( rpt_ptr,mmoc_info_ptr);
      break;

    default:
      MMOC_ERR_1("trans_state", mmoc_info_ptr->dev_prop.trans_state);
      evt_status = mmoc_clear_transaction( NULL, mmoc_info_ptr );
      break;
  }

  return evt_status;

} /* mmoc_process_dual_standby_chgd() */


#if defined (FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
/*===========================================================================

FUNCTION mmoc_process_deactivate_1xcsfb_cmd_null

DESCRIPTION
  This function is the transaction handler for the Deactivate CSFB transaction in NULL state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_deactivate_1xcsfb_cmd_null
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */
  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_evt_status_e_type  evt_status = MMOC_EVT_STATUS_CONSUMED;
  sys_modem_as_id_e_type  asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;

  if ( (cmd_ptr == NULL) || (cmd_ptr->cmd.name != MMOC_CMD_DEACT_1XCSFB_PROT))
  {
    MMOC_ERR_0("error cmd");

    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  /* Deactivate 1x stack if it is in 1x CSFB mode */
#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)) && \
  defined( FEATURE_LTE_TO_1X )
  if (cmsds_is_1xcsfb_activated(asubs_id))
  {

    cmsds_deactivate_1xcsfb_protocol (mmoc_next_trans_id(mmoc_info_ptr),
                                      cmd_ptr->param.cdma_csfb_deact.deact_reason,
                                      asubs_id);
    /* Protocol deactivate request sent, wait for confirmation. */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_1XCSFB_DEACT_CNF,
                             mmoc_info_ptr
                           );
  }
  else
#endif
  {
    evt_status = mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  return evt_status;

}

/*===========================================================================

FUNCTION mmoc_process_deact_1xcsfb_prot_cnf

DESCRIPTION
  This function is the transaction handler for the Deactivate CSFB CNF handler

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_deactivate_1xcsfb_prot_cnf
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_evt_status_e_type  evt_status = MMOC_EVT_STATUS_CONSUMED;

  if (rpt_ptr == NULL)
  {
    MMOC_ERR_0("rpt_ptr is NULL");

    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  if(mmoc_info_ptr->dev_prop.sub_with_3gpp2 < SYS_MODEM_AS_ID_1 ||
      mmoc_info_ptr->dev_prop.sub_with_3gpp2 >= MAX_SIMS ||
      mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2] == NULL ||
      mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_0] == NULL)
  {
    return evt_status;
  }

  if (rpt_ptr->rpt.name == MMOC_RPT_PROT_AUTO_DEACTD_IND &&
      rpt_ptr->param.prot_deactd_ind.reason == PROT_DEACT_ACQ_OTHERS &&
      mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_ONLINE_GWL)
  {
    mmoc_info_ptr->dev_prop.is_buffer_auto_deact_ind = TRUE;
    MMOC_MSG_HIGH_0("Buffer AUTO_DEACT_IND");
    return evt_status;
  }

  if ((rpt_ptr->rpt.name == MMOC_RPT_1XCSFB_PROT_DEACTD_CNF) &&
      (mmoc_info_ptr->dev_prop.trans_id == rpt_ptr->param.cdma_csfb_deactd_cnf.trans_id))
  {
    if(mmoc_info_ptr->dev_prop.is_buffer_auto_deact_ind)
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
      mm_id.stk_id   = MM_STACK_0;

      mmoc_info_ptr->dev_prop.is_buffer_auto_deact_ind = FALSE;

      evt_status = mmoc_activate_prot_online( mm_id, mmoc_info_ptr );
    }

    evt_status = mmoc_clear_transaction (NULL, mmoc_info_ptr);

  }
  else
  {
    MMOC_ERR_2( "Unexpected report received %d in state %d",
                rpt_ptr->rpt.name, mmoc_info_ptr->dev_prop.trans_state);
  }

  return evt_status;

}


/*===========================================================================

FUNCTION mmoc_process_deact_1xcsfb_cmd

DESCRIPTION
  This function is the transaction handler for the Deactivate CSFB transaction

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_deactivate_1xcsfb_cmd
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  mmoc_evt_status_e_type  evt_status = MMOC_EVT_STATUS_CONSUMED;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Invoke the appropriate transaction state handler. */

  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
    {
      /* The offline operating mode changed command from CM should only create
           ** this transaction.
           */
      evt_status = mmoc_process_deactivate_1xcsfb_cmd_null(cmd_ptr, rpt_ptr, mmoc_info_ptr);
      break;
    }

    case MMOC_TRANS_STATE_WAIT_1XCSFB_DEACT_CNF:
    {
      evt_status =  mmoc_process_deactivate_1xcsfb_prot_cnf(cmd_ptr, rpt_ptr, mmoc_info_ptr);
      break;
    }

    default:
      MMOC_ERR_0("Received event in unknown state");
      evt_status = mmoc_clear_transaction( NULL, mmoc_info_ptr );
      break;

  }
  return evt_status;

}

#endif


#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*===========================================================================

FUNCTION mmoc_process_mcfg_refresh_cmd

DESCRIPTION
  This function is the transaction handler for the MCFG refresh

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_mcfg_refresh_cmd
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */
  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  uint32 prev_subs_capability[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_1;

  if(cmd_ptr == NULL)
  {
    /* Not the expected cmd.
     */
    MMOC_ERR_0("cmd NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /* Save the current maxcap */
  memset(prev_subs_capability,0,sizeof(prev_subs_capability));

  for(sub = SYS_MODEM_AS_ID_1; sub < MAX_SIMS; sub++)
  {
    if(mmoc_info_ptr->sub_prop[sub] != NULL)
    {
      prev_subs_capability[sub] = mmoc_info_ptr->sub_prop[sub]->subs_capability;
    }
  }

  MMOC_MSG_HIGH_0("MCFG_REFRESH: read policy man");
  mmoc_read_policyman_config(mmoc_info_ptr, cmd_ptr->param.mcfg_refresh.as_id, FALSE);

  /* Restore maxcap */
  for(sub = SYS_MODEM_AS_ID_1; sub < MAX_SIMS; sub++)
  {
    if(mmoc_info_ptr->sub_prop[sub] != NULL && cmd_ptr->param.mcfg_refresh.as_id == sub)
    {
      MMOC_MSG_HIGH_3("MCFG_REFRESH: Restore maxcap sub %d %d->%d",
                      sub, mmoc_info_ptr->sub_prop[sub]->subs_capability,
                      prev_subs_capability[sub]);
      mmoc_info_ptr->sub_prop[sub]->subs_capability = prev_subs_capability[sub];
    }
  }

  return mmoc_clear_transaction( NULL, mmoc_info_ptr );

}

#endif


/*===========================================================================

FUNCTION mmoc_process_deact_from_dormant_deact_cnf

DESCRIPTION
  This is a function for handling the transaction state DEACT_CNF for the
  deact from dormant transaction.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Should wait for deactivated confirmation.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start ph_stat_enter processing.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_deact_from_dormant_deact_cnf
(
  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  if ( rpt_ptr == NULL )
  {
    /* Not the expected report.
    */
    MMOC_ERR_0("rpt NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_MSG_HIGH_1("deact_cnf_gwl, rpt name %d",rpt_ptr->rpt.name);
  /* Process the received report.
  */
  switch(rpt_ptr->rpt.name)
  {
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - --*/
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      return  mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    case MMOC_RPT_PROT_DEACTD_CNF:
      if(mmoc_info_ptr->dev_prop.trans_id != rpt_ptr->param.prot_deactd_cnf.trans_id)
      {
        /* Not the expected report. Go back to waiting for cnf
        */
        MMOC_ERR_2("Unexpected event received. rpt trans_id %d trans_id %d",
                   rpt_ptr->param.prot_deactd_cnf.trans_id,
                   mmoc_info_ptr->dev_prop.trans_id);

        return MMOC_EVT_STATUS_CONSUMED;
      }

      /* If Deact_Cnf is received for a dormant protocol
      */
      if(mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_DEACT_FROM_DORMANT)
      {
        mmoc_update_prot_dormant_state( SD_SS_MODE_PREF_NONE,
                                        mmoc_info_ptr->dev_prop.deact_req_idx,
                                        mmoc_info_ptr
                                      );
        /* Expected rpt. So continue processing
        */
        mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_DEACT_ENTER,
                                 mmoc_info_ptr
                               );

      }
      return MMOC_EVT_STATUS_NOT_CONSUMED;
    default:
      MMOC_ERR_1("Unexpected report received %d",
                 rpt_ptr->rpt.name);
      return MMOC_EVT_STATUS_CONSUMED;

  } /* switch ( rpt_ptr->rpt.name ) */

} /* mmoc_process_deact_from_dormant_deact_cnf() */


/*===========================================================================

FUNCTION mmoc_process_deact_from_dormant_deact_enter

DESCRIPTION
  This is a function for handling the transaction state DEACT_ENTER for the
  deact from dormant transaction . If a protocol needs to be deactivated,
  it sets the next trans_state to DEACTD_CNF ends the transaction.
  It will deactivate all active protocols

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Should wait for deactivated confirmation.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start ph_stat_enter processing.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_deact_from_dormant_deact_enter
(
  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */

)
{
#if defined MMOC_GWL_SUPPORTED
  if(mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id < SYS_MODEM_AS_ID_1 ||
      mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id >= MAX_SIMS ||
      mmoc_info_ptr->dev_prop.deact_req_idx.stk_id < MM_STACK_0 ||
      mmoc_info_ptr->dev_prop.deact_req_idx.stk_id >= MAX_STACKS ||
      mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id] == NULL ||
      mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id]->stack_prop[mmoc_info_ptr->dev_prop.deact_req_idx.stk_id] == NULL)
  {
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /* Check whether corresponding protocols to be deactivated are in dormant state for
  ** mentioned stack.
  */

  if( mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id]->stack_prop[mmoc_info_ptr->dev_prop.deact_req_idx.stk_id]->is_gwl_deact_sent == FALSE)
  {
    cmregprx_cmd_gwl_deactivate_protocol( PROT_DEACT_FROM_DORMANT,
                                          mmoc_next_trans_id(mmoc_info_ptr),
                                          mmoc_info_ptr->dev_prop.deact_req_idx );

    mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id]->stack_prop[mmoc_info_ptr->dev_prop.deact_req_idx.stk_id]->is_gwl_deact_sent = TRUE;

    MMOC_MSG_HIGH_3("Deact GWL on sub %d stk %d prot_state %d",
                    mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id,
                    mmoc_info_ptr->dev_prop.deact_req_idx.stk_id,
                    mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id]->stack_prop[mmoc_info_ptr->dev_prop.deact_req_idx.stk_id]->prot_state);

    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL,
                             mmoc_info_ptr
                           );

    return MMOC_EVT_STATUS_CONSUMED;
  }


#endif //MMOC_GW_SUPPORTED

  /* All requested protocols deactivated, clear the transaction */

  return mmoc_clear_transaction( NULL, mmoc_info_ptr );

} /* mmoc_process_deact_from_dormant_deact_enter() */


/*===========================================================================

FUNCTION mmoc_process_deact_from_dormant_null

DESCRIPTION
  This function handles the deact from dormant transaction in the NULL
  transaction state.

  In this state, the state handler will

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_deact_from_dormant_null
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  MMOC_ASSERT( cmd_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* The deact from dormant command from CM should only create this
  ** transaction.
  */
  if ( cmd_ptr == NULL ||
       cmd_ptr->cmd.name != MMOC_CMD_DEACT_FROM_DORMANT
     )
  {
    MMOC_ERR_0("Deact from dormant incorrectly initiated");

    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if protocols to be deactivated is not NONE */

  if(cmd_ptr->param.deact_dormant.prot == SD_SS_MODE_PREF_NONE)
  {
    MMOC_ERR_0("Deact from dormant prot = NONE");

    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Set the SS for which message is received */

  mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id = cmd_ptr->param.deact_dormant.mm_id.asubs_id;
  mmoc_info_ptr->dev_prop.deact_req_idx.stk_id   = cmd_ptr->param.deact_dormant.mm_id.stk_id;

  /* Deactivate the dormant protocols, Move to DEACT_ENTER trans.
  ** state.
  */
  mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_DEACT_ENTER,
                           mmoc_info_ptr
                         );



  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmoc_process_deact_from_dormant_null() */


/*===========================================================================

FUNCTION mmoc_process_deact_from_dormant

DESCRIPTION
  This function is the transaction handler for the deact_from_dormant
  transaction.

  The transaction state transition will be,

     NULL -> NULL

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_deact_from_dormant
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_evt_status_e_type evt_status = MMOC_EVT_STATUS_CONSUMED;

  /* Invoke the current transaction state handler.
  */
  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status = mmoc_process_deact_from_dormant_null( cmd_ptr,
                   mmoc_info_ptr
                                                       );
      break;

    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
      evt_status = mmoc_process_deact_from_dormant_deact_enter (mmoc_info_ptr
                                                               );
      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF_GWL:
      evt_status = mmoc_process_deact_from_dormant_deact_cnf(rpt_ptr,
                   mmoc_info_ptr
                                                            );
      break;

    case MMOC_TRANS_STATE_HDR_DEACT_ENTER:
    case MMOC_TRANS_STATE_WAIT_HDR_DEACTD_CNF:
    case MMOC_TRANS_STATE_WAIT_PH_STAT_CNF:
    case MMOC_TRANS_STATE_WAIT_GEN_CMD_CNF:
    case MMOC_TRANS_STATE_PROT_PH_STAT_ENTER:
    case MMOC_TRANS_STATE_MAX:
    case MMOC_TRANS_STATE_GEN_CMD_ENTER:
    case MMOC_TRANS_STATE_WAIT_AUTO_DEACTD_IND:
    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
    default:
      MMOC_ERR_1(" Invalid trans_state %d", mmoc_info_ptr->dev_prop.trans_state);
      evt_status = mmoc_clear_transaction( NULL, mmoc_info_ptr );
      break;
  }

  return evt_status;

}  /* mmoc_process_deact_from_dormant */

/*===========================================================================

FUNCTION mmoc_process_suspend_resume_enter

DESCRIPTION
  This function handles suspend_resume transaction in SUSPEND_RESUME_ENTER
  transaction state.

DEPENDENCIES
  None.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

SIDE EFFECTS
  None.
===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_process_suspend_resume_enter
(

  mmoc_state_info_s_type              *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  prot_deact_e_type  deact_reason = PROT_DEACT_LOCAL_DETACH;
  boolean            is_deact_req_sent = FALSE;
  mm_sub_stk_id_s_type mm_id;
  sys_modem_as_id_e_type deact_sub = SYS_MODEM_AS_ID_1;
  multimode_stack_e_type deact_stk = MM_STACK_0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send deactivate request to all active protocols.
  */
  is_deact_req_sent = mmoc_deactivate_all_active_protocol(
                        deact_reason,
                        mmoc_info_ptr
                      );

  if (is_deact_req_sent)
  {
    /* If deactivate request is sent on a stack, update transaction state to WAIT_DEACTD_CNF
    */
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_DEACTD_CNF,
                             mmoc_info_ptr
                           );

    /** Update the corresponding stack's mmoc_susp_resume_status flag to TRUE
    */
    deact_sub = mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id;
    deact_stk = mmoc_info_ptr->dev_prop.deact_req_idx.stk_id;
    if(deact_sub >= SYS_MODEM_AS_ID_1 && deact_sub < MAX_SIMS &&
        deact_stk >= MM_STACK_0 && deact_stk < MAX_STACKS)
    {
      mmoc_susp_resume_status[deact_sub][deact_stk] = is_deact_req_sent;
    }

    /* Since SS has been deactivated , send indication to SD that service
    ** is lost. SD will then inform CM of srv loss
    */
    sd_ss_ind_misc_srv_lost(mmoc_info_ptr->dev_prop.deact_req_idx);

    return MMOC_EVT_STATUS_CONSUMED;
  }


  for(mm_id.asubs_id = 0; 
      mm_id.asubs_id < mmoc_info_ptr->nSubs && mm_id.asubs_id < MAX_SIMS; 
      mm_id.asubs_id++)
  {
    for(mm_id.stk_id = 0; 
        mm_id.stk_id < mmoc_info_ptr->sub_prop[mm_id.asubs_id]->nStacks && mm_id.stk_id < MAX_STACKS; 
        mm_id.stk_id++)
    {
      /* If all the active protocols are deactivated, start resume/activate_protocol
      ** operation on the ones that were deactivated.
      */
      if(mmoc_susp_resume_status[mm_id.asubs_id][mm_id.stk_id] == TRUE)
      {

        mmoc_activate_prot_online( mm_id, mmoc_info_ptr );

        // Reset the flag mmoc_susp_resume_status after SS is activated
        mmoc_susp_resume_status[mm_id.asubs_id][mm_id.stk_id] = FALSE;
      }
    }
  }

  /* Once all the stacks are activated, inform CM that suspend/resume
  ** transaction is completed
  */
  mmoc_send_cm_rpt( CM_SUSPEND_RESUME_F, NULL, deact_sub );

  /* Clear the transaction.
  */
  return mmoc_clear_transaction( NULL, mmoc_info_ptr );

}

/*===========================================================================

FUNCTION mmoc_process_suspend_resume_null

DESCRIPTION
  This function handles suspend_resume transaction in NULL transaction state.

DEPENDENCIES
  None.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

SIDE EFFECTS
  None.
===========================================================================*/
static  mmoc_evt_status_e_type   mmoc_process_suspend_resume_null(

  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  mmoc_state_info_s_type              *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{
  if( cmd_ptr == NULL )
  {
    MMOC_ERR_0("NULL ptr recvd for cmd_ptr");
    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  /* SUSPEND_RESUME command from CM only should create this transaction.
  */
  if ( cmd_ptr->cmd.name != MMOC_CMD_SUSPEND_RESUME_IND)
  {
    /* Transaction initiated incorrectly.
    */
    MMOC_ERR_0("SUSPEND_RESUME command transaction initiated incorrectly");
    return mmoc_clear_transaction( NULL, mmoc_info_ptr );
  }

  mmoc_update_trans_state( MMOC_TRANS_STATE_SUSPEND_RESUME_ENTER,
                           mmoc_info_ptr);

  return MMOC_EVT_STATUS_NOT_CONSUMED;

}

/*===========================================================================

FUNCTION mmoc_process_suspend_resume_ind

DESCRIPTION
  This function is the transaction handler for MMOC_CMD_SUSPEND_RESUME_IND

DEPENDENCIES
  None.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

SIDE EFFECTS
  None.
===========================================================================*/

static  mmoc_evt_status_e_type         mmoc_process_suspend_resume_ind
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type               *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */

)
{

  mmoc_evt_status_e_type evt_status = MMOC_EVT_STATUS_CONSUMED;


  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status = mmoc_process_suspend_resume_null( cmd_ptr,
                   mmoc_info_ptr
                                                   );
      break;

    case MMOC_TRANS_STATE_SUSPEND_RESUME_ENTER:
    case MMOC_TRANS_STATE_PROT_DEACT_ENTER:
      evt_status = mmoc_process_suspend_resume_enter (mmoc_info_ptr);

      break;

    case MMOC_TRANS_STATE_WAIT_DEACTD_CNF:
      evt_status = mmoc_process_common_deactd_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                 );
      break;

    default:
      MMOC_ERR_0("Received event in unknown state");
      evt_status = mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
      break;

  }

  return evt_status;
}


#ifdef MMODE_ADDITIONAL_DEBUG_INFO
/*===========================================================================

FUNCTION mmoc_dbg_buf_add_mesage

DESCRIPTION
  This function adds the transaction being processed to the debug buffer.

===========================================================================*/
void                            mmoc_dbg_buf_add_message
(
  mmoc_state_info_s_type         *mmoc_info_ptr,

  mmoc_dbg_buf_rpt_s_type        *rpt_queued_ptr,

  uint8                           rpt_idx
)
{
  int i,j;
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;

  /* Pointer to current transaction
  */
  mmoc_dbg_buf_trans_s_type *current_trans_ptr = NULL;

  int8 curr_idx = mmoc_debug_buffer.dbg_buf_idx;

  /* For commands this ptr shold be null and hence add a new transaction here
  */
  if(rpt_queued_ptr == NULL)
  {
    /* Increment debug_buffer_index and set it to 0 in case it becomes same as
    ** MMOC_MAX_DEBUG_BUFFER_SIZE as transactions are added in cyclic manner
    */
    curr_idx++;

    if(curr_idx >= MMOC_MAX_DEBUG_BUFFER_SIZE)
    {
      curr_idx = 0;
    }
    /* Assign pointer to current trans to current_trans_ptr
    */
    current_trans_ptr = ((mmoc_debug_buffer.dbg_buf)+curr_idx);
    /* Add transaction here
    */
    mmoc_dbg_buf_init_rpt_queue(curr_idx);
    current_trans_ptr->trans_id = mmoc_info_ptr->dev_prop.trans_id;
    current_trans_ptr->trans_name = mmoc_info_ptr->dev_prop.trans;

    /* Add additional data for particular transaction types */
    switch(mmoc_info_ptr->dev_prop.trans)
    {
      case MMOC_TRANS_SUBSC_CHGD:
        current_trans_ptr->addl_info[0] = mmoc_info_ptr->dev_prop.active_subs;
        current_trans_ptr->addl_info[1] = mmoc_info_ptr->dev_prop.device_mode;

        for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
        {
          current_trans_ptr->addl_info[2*(sub+1)] = mmoc_info_ptr->sub_prop[sub]->prot_subsc_chg;
          current_trans_ptr->addl_info[2*(sub+1)+1] = mmoc_info_ptr->sub_prop[sub]->sub_avail;
        }

        break;

      case MMOC_TRANS_PROT_GEN_CMD:
        current_trans_ptr->addl_info[0] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.cmd_type;
        current_trans_ptr->addl_info[1] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.asubs_id;
        current_trans_ptr->addl_info[2] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.stk_id;

        if(current_trans_ptr->addl_info[0] == PROT_GEN_CMD_PREF_SYS_CHGD)
        {
          current_trans_ptr->addl_info[3] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.param.pref_sys_chgd.pref_reas;
          current_trans_ptr->addl_info[4] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.param.pref_sys_chgd.addl_action.action;
          current_trans_ptr->addl_info[5] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.param.pref_sys_chgd.addl_action.ue_mode;
          current_trans_ptr->addl_info[6] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.param.pref_sys_chgd.addl_action.is_ue_mode_substate_srlte;
          current_trans_ptr->addl_info[7] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.param.pref_sys_chgd.domain_pref;
        }
        break;

      case MMOC_TRANS_SUSPEND_STACK:
        current_trans_ptr->addl_info[0] = mmoc_info_ptr->dev_prop.trans_info.suspend_stack.mm_id.asubs_id;
        current_trans_ptr->addl_info[1] = mmoc_info_ptr->dev_prop.trans_info.suspend_stack.mm_id.stk_id;
        current_trans_ptr->addl_info[2] = mmoc_info_ptr->dev_prop.trans_info.suspend_stack.is_suspend;
        current_trans_ptr->addl_info[3] = mmoc_info_ptr->dev_prop.trans_info.suspend_stack.ignore_protocol_activate;
        current_trans_ptr->addl_info[4] = mmoc_info_ptr->dev_prop.trans_info.suspend_stack.susp_reason;
        break;

      default:
        break;
    }

    mmoc_debug_buffer.dbg_buf_idx = curr_idx;
    return;
  }

  /* For reports this ptr shold NOT be null and hence add reports
  ** for current transaction here
  */
  if((rpt_queued_ptr != NULL && rpt_idx < MMOC_MAX_RPT_COUNT) &&
      (curr_idx >= 0) &&
      (curr_idx < MMOC_MAX_DEBUG_BUFFER_SIZE)
    )
  {
    /* Assign pointer to current trans to current_trans_ptr
    */
    current_trans_ptr = ((mmoc_debug_buffer.dbg_buf)+curr_idx);


    for(i = 0; i < (int)mmoc_info_ptr->nSubs && i < MAX_SIMS; i++)
    {
      for(j = 0; j < (int)mmoc_info_ptr->sub_prop[i]->nStacks && j < MAX_STACKS; j++)
      {
        current_trans_ptr->rpt_queue[rpt_idx].prot_state[i][j] = rpt_queued_ptr->prot_state[i][j];
      }
    }

    current_trans_ptr->rpt_queue[rpt_idx].rpt_name = rpt_queued_ptr->rpt_name;
    current_trans_ptr->rpt_queue[rpt_idx].task_name = rpt_queued_ptr->task_name;

  }

} /* mmoc_dbg_buf_add_message() */

/*===========================================================================

FUNCTION mmoc_dbg_buf_add_addl_info

DESCRIPTION
  This function adds the addl_info of the previously processed transaction
  to the debug buffer.

===========================================================================*/
void                            mmoc_dbg_buf_add_addl_info
(
  mmoc_state_info_s_type   *mmoc_info_ptr,

  mmoc_trans_e_type        curr_trans
)
{
  /* Pointer to current transaction
  */
  mmoc_dbg_buf_trans_s_type *current_trans_ptr = NULL;
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;

  int8 curr_idx = mmoc_debug_buffer.dbg_buf_idx;

  /* Assign pointer to current trans to current_trans_ptr
  */
  current_trans_ptr = ((mmoc_debug_buffer.dbg_buf)+curr_idx);
  /* Add transaction here
  */

  if(current_trans_ptr->trans_name != curr_trans)
  {
    MMOC_MSG_HIGH_2("Error in mmoc_dbg_buf_add_addl_info: current_trans_ptr->trans_name(%d) != curr_trans(%d)",
                    current_trans_ptr->trans_name,
                    curr_trans);
    return;
  }

  /* Add additional data for particular transaction types */
  switch(curr_trans)
  {
    case MMOC_TRANS_SUBSC_CHGD:
      current_trans_ptr->addl_info[0] = mmoc_info_ptr->dev_prop.active_subs;
      current_trans_ptr->addl_info[1] = mmoc_info_ptr->dev_prop.device_mode;

      for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
      {
        current_trans_ptr->addl_info[2*(sub+1)] = mmoc_info_ptr->sub_prop[sub]->prot_subsc_chg;
        current_trans_ptr->addl_info[2*(sub+1)+1] = mmoc_info_ptr->sub_prop[sub]->sub_avail;
      }

      break;

    case MMOC_TRANS_PROT_GEN_CMD:
      current_trans_ptr->addl_info[0] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.cmd_type;
      current_trans_ptr->addl_info[1] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.asubs_id;
      current_trans_ptr->addl_info[2] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.stk_id;

      if(current_trans_ptr->addl_info[0] == PROT_GEN_CMD_PREF_SYS_CHGD)
      {
        current_trans_ptr->addl_info[3] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.param.pref_sys_chgd.pref_reas;
        current_trans_ptr->addl_info[4] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.param.pref_sys_chgd.addl_action.action;
        current_trans_ptr->addl_info[5] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.param.pref_sys_chgd.addl_action.ue_mode;
        current_trans_ptr->addl_info[6] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.param.pref_sys_chgd.addl_action.is_ue_mode_substate_srlte;
        current_trans_ptr->addl_info[7] = mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.cmd_info.param.pref_sys_chgd.domain_pref;
      }
      break;

    case MMOC_TRANS_SUSPEND_STACK:
      current_trans_ptr->addl_info[0] = mmoc_info_ptr->dev_prop.trans_info.suspend_stack.mm_id.asubs_id;
      current_trans_ptr->addl_info[1] = mmoc_info_ptr->dev_prop.trans_info.suspend_stack.mm_id.stk_id;
      current_trans_ptr->addl_info[2] = mmoc_info_ptr->dev_prop.trans_info.suspend_stack.is_suspend;
      current_trans_ptr->addl_info[3] = mmoc_info_ptr->dev_prop.trans_info.suspend_stack.ignore_protocol_activate;
      current_trans_ptr->addl_info[4] = mmoc_info_ptr->dev_prop.trans_info.suspend_stack.susp_reason;
      break;

    default:
      break;
  }

} /* mmoc_dbg_buf_add_addl_info() */
#endif /* MMODE_ADDITIONAL_DEBUG_INFO */


/*===========================================================================

FUNCTION mmoc_setup_trans

DESCRIPTION
  The purpose of this function is to determine which transaction should be
  invoked in order to process the event. This will be based on the type of
  event received.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmoc_setup_trans
(
  const mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type            *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_evt_status_e_type  evt_status = MMOC_EVT_STATUS_NOT_CONSUMED;

#ifdef MMODE_ADDITIONAL_DEBUG_INFO
  mmoc_dbg_buf_rpt_s_type  rpt_to_queue;
  uint8 i,j;
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Set cleanup function to NULL.
  */
  mmoc_info_ptr->dev_prop.exit_fn_ptr = NULL;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if the event is a command or report.
  */
  if(cmd_ptr != NULL)
  {
    /* Event is a command.
    */
    switch(cmd_ptr->cmd.name)
    {
      case MMOC_CMD_OPRT_MODE_CHGD:
      {
        if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state == PROT_STATE_PWR_DOWN)
        {
          MMOC_ERR_0("Unexpected OPRT_MODE_CHGD in PROT_STATE_PWR_DOWN, skip processing");
          evt_status = MMOC_EVT_STATUS_CONSUMED;
        }
        else
        {
          switch(cmd_ptr->param.oprt_mode_chgd.oprt_mode)
          {
            case SYS_OPRT_MODE_OFFLINE:
            case SYS_OPRT_MODE_OFFLINE_CDMA:
            case SYS_OPRT_MODE_FTM:
              mmoc_update_curr_trans( MMOC_TRANS_OFFLINE,
                                      mmoc_info_ptr,
                                      cmd_ptr->param.oprt_mode_chgd.oprt_mode
                                    );
              break;

            case SYS_OPRT_MODE_RESET:
            case SYS_OPRT_MODE_RESET_MODEM:
            case SYS_OPRT_MODE_LPM:
            case SYS_OPRT_MODE_PWROFF:
              mmoc_update_curr_trans( MMOC_TRANS_PWR_DOWN,
                                      mmoc_info_ptr,
                                      cmd_ptr->param.oprt_mode_chgd.oprt_mode
                                    );
              break;

            case SYS_OPRT_MODE_ONLINE:
              mmoc_update_curr_trans( MMOC_TRANS_ONLINE,
                                      mmoc_info_ptr,
                                      cmd_ptr->param.oprt_mode_chgd.oprt_mode
                                    );
              mmoc_update_emerg_pending(cmd_ptr, mmoc_info_ptr);

              break;

            default:
              MMOC_ERR_1("Incorrect oprt_mode received %d",
                         cmd_ptr->param.oprt_mode_chgd.oprt_mode);
              evt_status = MMOC_EVT_STATUS_CONSUMED;
              break;

          } /* switch( cmd_ptr->param.oprt_mode_chgd.oprt_mode) */
        }/* else of (mmoc_info_ptr->prot_state[SD_SS_MAIN] == PROT_STATE_PWR_DOWN) */
      }
      break;

      case MMOC_CMD_SUBSCRIPTION_CHGD:
        mmoc_update_curr_trans( MMOC_TRANS_SUBSC_CHGD,
                                mmoc_info_ptr,
                                SYS_OPRT_MODE_NONE
                              );
        break;

      case MMOC_CMD_PROT_GEN_CMD:
        mmoc_update_curr_trans( MMOC_TRANS_PROT_GEN_CMD,
                                mmoc_info_ptr,
                                SYS_OPRT_MODE_NONE
                              );
        mmoc_info_ptr->dev_prop.exit_fn_ptr = (mmoc_exit_fn_f_type *)mmoc_postprocess_prot_gen_cmd;
        mmoc_info_ptr->dev_prop.trans_info.gen_prot_cmd.is_activate_main = FALSE;
        break;

      case MMOC_CMD_PROT_REDIR_IND:
        mmoc_update_curr_trans( MMOC_TRANS_PROT_REDIR_IND,
                                mmoc_info_ptr,
                                SYS_OPRT_MODE_NONE
                              );
        break;

      case MMOC_CMD_PROT_HO_IND:
#if defined (FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
        mmoc_update_curr_trans( MMOC_TRANS_PROT_HO_IND,
                                mmoc_info_ptr,
                                SYS_OPRT_MODE_NONE
                              );
#endif /* FEATURE_LTE_TO_1X */
        break;

      case MMOC_CMD_MMGSDI_INFO_IND:
        mmoc_update_curr_trans( MMOC_TRANS_MMGSDI_INFO_IND,
                                mmoc_info_ptr,
                                SYS_OPRT_MODE_NONE
                              );
        break;

      case MMOC_CMD_DUAL_STANDBY_CHGD:
        mmoc_update_curr_trans( MMOC_TRANS_DUAL_STANDBY_CHGD,
                                mmoc_info_ptr,
                                SYS_OPRT_MODE_NONE
                              );
        break;

      case MMOC_CMD_DEACT_FROM_DORMANT:
        mmoc_update_curr_trans( MMOC_TRANS_DEACT_FROM_DORMANT,
                                mmoc_info_ptr,
                                SYS_OPRT_MODE_NONE
                              );
        mmoc_info_ptr->dev_prop.exit_fn_ptr = (mmoc_exit_fn_f_type *)mmoc_postprocess_prot_deact_dormant;
        mmoc_info_ptr->dev_prop.trans_info.deact_from_dormant.is_activate_main = FALSE;
        break;

#if defined (FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
      case MMOC_CMD_DEACT_1XCSFB_PROT:
        mmoc_update_curr_trans( MMOC_TRANS_DEACT_1XCSFB_CMD,
                                mmoc_info_ptr,
                                SYS_OPRT_MODE_NONE
                              );
        break;
#endif

      case MMOC_CMD_SUSPEND_STACK:
        mmoc_update_curr_trans( MMOC_TRANS_SUSPEND_STACK,
                                mmoc_info_ptr,
                                SYS_OPRT_MODE_NONE
                              );
        break;

#ifdef FEATURE_MODEM_CONFIG_REFRESH
      case MMOC_CMD_MCFG_REFRESH:
        mmoc_update_curr_trans( MMOC_TRANS_MCFG_REFRESH,
                                mmoc_info_ptr,
                                SYS_OPRT_MODE_NONE
                              );
        break;
#endif

      case MMOC_CMD_SUSPEND_RESUME_IND:

        mmoc_update_curr_trans( MMOC_TRANS_SUSPEND_RESUME,
                                mmoc_info_ptr,
                                SYS_OPRT_MODE_NONE
                              );
        break;

      default:
        MMOC_ERR_1("Received unknown command event %d",
                   cmd_ptr->cmd.name);
        evt_status = MMOC_EVT_STATUS_CONSUMED;
        break;

    } /* switch ( cmd_ptr->cmd.name ) */

  } /* if ( cmd_ptr ) */

  else if ( rpt_ptr != NULL )
  {
    /* Event is a report.
    */
    switch(rpt_ptr->rpt.name)
    {
      case MMOC_RPT_PROT_AUTO_DEACTD_IND:
        mmoc_update_curr_trans( MMOC_TRANS_PROT_AUTO_DEACTD_IND,
                                mmoc_info_ptr,
                                mmoc_info_ptr->dev_prop.true_oprt_mode
                              );
        break;
      case MMOC_RPT_PROT_AUTO_ACTD_IND:
      case MMOC_RPT_PROT_DEACTD_CNF:
      case MMOC_RPT_PH_STAT_CHGD_CNF:
      case MMOC_RPT_PROT_GEN_CMD_CNF:
      default:
        MMOC_MSG_MED_1( "Unexpected report received %d",
                        rpt_ptr->rpt.name);
        evt_status = MMOC_EVT_STATUS_CONSUMED;
        break;

    } /* switch ( rpt_ptr->rpt.name ) */

  } /* else if ( rpt_ptr ) */
  else
  {
    sys_err_fatal_null_ptr_exception();
    return MMOC_EVT_STATUS_CONSUMED;
  } /* else */

#ifdef MMODE_ADDITIONAL_DEBUG_INFO
  curr_rpt_idx = 0;

  if (rpt_ptr == NULL)
  {
    rpt_to_queue.rpt_name = MMOC_RPT_MAX;
    rpt_to_queue.task_name = MMOC_TASK_NAME_NONE;
  }
  else
  {
    rpt_to_queue.rpt_name = rpt_ptr->rpt.name;
    rpt_to_queue.task_name = rpt_ptr->rpt.task_name;
  }

  for(i = 0; i < (int)mmoc_info_ptr->nSubs && i < MAX_SIMS; i++)
  {
    for(j = 0; j < (int)mmoc_info_ptr->sub_prop[i]->nStacks && j < MAX_STACKS; j++)
    {
      rpt_to_queue.prot_state[i][j] = mmoc_info_ptr->sub_prop[i]->stack_prop[j]->prot_state;
    }
  }

  mmoc_dbg_buf_add_message(mmoc_info_ptr,
                           NULL,
                           0);

  mmoc_dbg_buf_add_message(mmoc_info_ptr, &(rpt_to_queue), 0);
#endif

  return evt_status;

} /* mmoc_setup_trans() */




/*===========================================================================

FUNCTION mmoc_call_trans_hndlr

DESCRIPTION
  This function will process the events (either command or report) until the
  event is processed to completion or consumed. This is done by calling the
  appropriate transaction handlers based on the current transaction.

===========================================================================*/
static  void                           mmoc_call_trans_hndlr
(
  mmoc_cmd_msg_s_type            *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  boolean is_new_trans = FALSE;
  mmoc_evt_status_e_type   evt_status = MMOC_EVT_STATUS_NOT_CONSUMED;
  mmoc_trans_e_type        last_trans;
  mmoc_trans_state_e_type  last_trans_state;
  mmoc_trans_e_type        curr_trans = MMOC_TRANS_NULL;

  if( ((cmd_ptr == NULL) && (rpt_ptr == NULL)) ||
      ((cmd_ptr != NULL) && (rpt_ptr != NULL)) ||
      (mmoc_info_ptr == NULL))
  {
    MMOC_ERR_3("cmd_ptr=%d, rpt_ptr=%d, mmoc_info_ptr=%d",
               cmd_ptr, rpt_ptr, mmoc_info_ptr);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(mmoc_info_ptr->dev_prop.trans >= MMOC_TRANS_MAX)
  {
    MMOC_ERR_1("invalid trans: %d", mmoc_info_ptr->dev_prop.trans);
    return;
  }

  if(mmoc_info_ptr->dev_prop.trans_state >= MMOC_TRANS_STATE_MAX)
  {
    MMOC_ERR_1("invalid trans_state: %d", mmoc_info_ptr->dev_prop.trans_state);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifndef FEATURE_MMOC_LOW_MEM_TARGET
  /* Print debug info before calling transaction handler.
  */
  mmocdbg_print_before_trans( mmoc_info_ptr );
#endif /* FEATURE_MMOC_LOW_MEM_TARGET */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if transaction is already in progress.
  */
  if(mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_NULL)
  {
    /* No transaction in progress, start new transaction.
    */
    evt_status = mmoc_setup_trans( cmd_ptr,
                                   rpt_ptr,
                                   mmoc_info_ptr
                                 );
    is_new_trans = TRUE;
  }

  curr_trans = mmoc_info_ptr->dev_prop.trans;

  /* Call the transaction handlers until the event is consumed.
  */
  while ( evt_status != MMOC_EVT_STATUS_CONSUMED )
  {
    last_trans = mmoc_info_ptr->dev_prop.trans;
    last_trans_state = mmoc_info_ptr->dev_prop.trans_state;

    switch( mmoc_info_ptr->dev_prop.trans )
    {
      case MMOC_TRANS_PROT_GEN_CMD:
        if (cmd_ptr == NULL &&
            mmoc_info_ptr->dev_prop.trans_state == MMOC_TRANS_STATE_NULL)
        {
          MMOC_ERR_0( "cmd_ptr is NULL for GEN_CMD");
          evt_status = mmoc_clear_transaction( NULL, mmoc_info_ptr );
        }
        else
        {
          evt_status = mmoc_process_prot_gen_cmd( cmd_ptr,
                                                  rpt_ptr,
                                                  mmoc_info_ptr
                                                );
        }
        break;

      case MMOC_TRANS_SUBSC_CHGD:
        evt_status = mmoc_process_subsc_chgd( cmd_ptr,
                                              rpt_ptr,
                                              mmoc_info_ptr
                                            );
        break;

      case MMOC_TRANS_ONLINE:
        evt_status = mmoc_process_online( cmd_ptr,
                                          rpt_ptr,
                                          mmoc_info_ptr
                                        );
        break;


      case MMOC_TRANS_OFFLINE:
        evt_status = mmoc_process_offline( cmd_ptr,
                                           rpt_ptr,
                                           mmoc_info_ptr
                                         );
        break;

      case MMOC_TRANS_PWR_DOWN:
        evt_status = mmoc_process_pwr_down( cmd_ptr,
                                            rpt_ptr,
                                            mmoc_info_ptr
                                          );
        break;

      case MMOC_TRANS_PWR_SAVE_ENTER:
        evt_status = mmoc_process_pwr_save_enter( cmd_ptr,
                     rpt_ptr,
                     mmoc_info_ptr
                                                );
        break;


      case MMOC_TRANS_PROT_AUTO_DEACTD_IND:
        evt_status = mmoc_process_auto_deactd_ind( cmd_ptr,
                     rpt_ptr,
                     mmoc_info_ptr
                                                 );
        break;


      case MMOC_TRANS_PROT_REDIR_IND:
        evt_status = mmoc_process_prot_redir_ind( cmd_ptr,
                     rpt_ptr,
                     mmoc_info_ptr
                                                );
        break;

      case MMOC_TRANS_PROT_HO_IND:
#if defined (FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
        evt_status = mmoc_process_prot_ho_ind ( cmd_ptr,
                                                rpt_ptr,
                                                mmoc_info_ptr
                                              );
#endif /* FEATURE_LTE_TO_1X */
        break;

      case MMOC_TRANS_MMGSDI_INFO_IND:
        evt_status = mmoc_process_mmgsdi_info_ind( cmd_ptr,
                     rpt_ptr,
                     mmoc_info_ptr
                                                 );
        break;

      case MMOC_TRANS_DUAL_STANDBY_CHGD:
        evt_status = mmoc_process_dual_standby_chgd( cmd_ptr,
                     rpt_ptr,
                     mmoc_info_ptr
                                                   );
        break;

      case MMOC_TRANS_DEACT_FROM_DORMANT:
        evt_status = mmoc_process_deact_from_dormant( cmd_ptr,
                     rpt_ptr,
                     mmoc_info_ptr
                                                    );
        break;
#if defined (FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
      case MMOC_TRANS_DEACT_1XCSFB_CMD:
        evt_status = mmoc_process_deactivate_1xcsfb_cmd( cmd_ptr,
                     rpt_ptr,
                     mmoc_info_ptr );

        break;
#endif

      case MMOC_TRANS_SUSPEND_STACK:
        evt_status = mmoc_process_suspend_stack( cmd_ptr,
                     rpt_ptr,
                     mmoc_info_ptr
                                               );
        break;
#ifdef FEATURE_MODEM_CONFIG_REFRESH

      case MMOC_TRANS_MCFG_REFRESH:
        evt_status = mmoc_process_mcfg_refresh_cmd( cmd_ptr,
                     rpt_ptr,
                     mmoc_info_ptr
                                                  );
        break;
#endif

      case MMOC_TRANS_SUSPEND_RESUME:
        evt_status = mmoc_process_suspend_resume_ind( cmd_ptr,
                     rpt_ptr,
                     mmoc_info_ptr
                                                    );
        break;

      case MMOC_TRANS_MAX:
      case MMOC_TRANS_NULL:
      default:
        MMOC_ERR_0("Invalid transaction");
        evt_status = mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
        break;

    } /* switch ( mmoc_info_ptr->curr_trans ) */

    /* ASSERT: if the event is not consumed, at least the transaction
    ** should have been changed.
    */
    if ( evt_status == MMOC_EVT_STATUS_NOT_CONSUMED &&
         last_trans == mmoc_info_ptr->dev_prop.trans &&
         last_trans_state == mmoc_info_ptr->dev_prop.trans_state
       )
    {
      MMOC_ERR_2("Stuck in trans %d %d", last_trans, last_trans_state);

      /*lint -save -e506 -e774*/
      MMOC_ASSERT( FALSE );
      /*lint -restore*/
    }

  } /* while ( evt_status != MMOC_EVT_STATUS_CONSUMED ) */

  if(is_new_trans)
  {
    mmoc_dbg_buf_add_addl_info(mmoc_info_ptr, curr_trans);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  if(mmoc_info_ptr->dev_prop.trans >= MMOC_TRANS_MAX)
  {
    MMOC_ERR_1( "invalid curr_trans: %d", mmoc_info_ptr->dev_prop.trans);
    return;
  }

  if(mmoc_info_ptr->dev_prop.trans_state >= MMOC_TRANS_STATE_MAX)
  {
    MMOC_ERR_1( "invalid trans_state: %d", mmoc_info_ptr->dev_prop.trans_state);
    return;
  }

#ifndef FEATURE_MMOC_LOW_MEM_TARGET
  /* Print debug info after calling transaction handler.
  */
  mmocdbg_print_after_trans( mmoc_info_ptr );
#endif /* FEATURE_MMOC_LOW_MEM_TARGET */

  return;

} /* mmoc_call_trans_hndlr() */

/* <EJECT> */
/*===========================================================================
=============================================================================
=============================================================================
============================== CM -> MMOC commands ==========================
=============================================================================
=============================================================================
===========================================================================*/

/* The following section defines the API when the clients ( example CM )
** have to communicate to MMoC using the command queue.
*/

/*===========================================================================

FUNCTION mmoc_cmd_subscription_chgd3

DESCRIPTION
  This function will pack the parameters and initiate the subscription
  changed transaction. This is only for the primary SUB with only the
  primary stack active.

===========================================================================*/
void                           mmoc_cmd_subscription_chgd3
(
  mmoc_subsc_chg_e_type          chg_type,
  /* Subscription change type. */

  byte                           nam,
  /* Current NAM selection (NAM-1, NAM-2, etc.). */

  sd_ss_orig_mode_e_type         orig_mode,
  /* The origination mode (normal, OTASP, emergency, other, etc.)
  ** that is associated with the selected NAM. */

  sd_ss_mode_pref_e_type         mode_pref,
  /* The mode preference that is associated with the selected NAM. */

  sd_ss_band_pref_e_type         band_pref,
  /* Band preference that is associated with the selected NAM. */

  sys_lte_band_mask_e_type       lte_band_pref,
  /* LTE band preference that is associated with the selected NAM. */

  sd_ss_band_pref_e_type         tds_band_pref,
  /* TD-SCDMA band preference that is associated with the selected NAM. */

  sd_ss_prl_pref_e_type          prl_pref,
  /* PRL preference that is associated with the selected NAM. */

  sd_ss_roam_pref_e_type         roam_pref,
  /* Roam preference that is associated with the selected NAM. */

  sd_ss_hybr_pref_e_type         hybr_pref,
  /* New hybrid preference (none, CDMA/HDR). */

  const sys_manual_sys_info_s_type    *manual_sys_info_ptr,
  /* Pointer to struct that defines the manual system. */

  sd_ss_srv_domain_pref_e_type   srv_domain_pref,
  /* Service domain preference that is associated with the selected
  ** NAM.
  ** Applies only for GSM/WCDMA modes. */

  sd_ss_acq_order_pref_e_type    acq_order_pref,
  /* Preference for the order of acquisition ( WCDMA before GSM,
  ** GSM before WCDMA etc). */

  boolean                        is_3gpp_subsc_avail,
  /* Current GSM/WCDMA/LTE subscription availability status. */

  boolean                        is_3gpp2_subsc_avail,
  /* Current CDMA/AMPS/HDR subscription availability status. */

  prot_subsc_chg_e_type          prot_subsc_chg
  /* Enumeration of protocols that have a change in
  ** subscription available status */
)
{
  mmoc_subs_data_s_type                data;
  mmoc_state_info_s_type              *mmoc_info_ptr = mmoc_get_state_info_ptr();

  memset(&data, 0, sizeof(mmoc_subs_data_s_type));

  /* Pack subscription data */
  data.as_id                = SYS_MODEM_AS_ID_1;
  data.active_stacks        = MM_STACK_0_MASK;
  data.nv_context           = 1;
  data.is_perso_locked      = FALSE;
  data.orig_mode            = orig_mode;
  data.mode_pref            = mode_pref;
  data.band_pref            = band_pref;
  data.roam_pref            = roam_pref;
  data.lte_band_pref        = lte_band_pref;
  data.tds_band_pref        = tds_band_pref;
  data.subs_capability      = mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->subs_capability;
  data.prot_subsc_chg       = prot_subsc_chg;

  /* 3GPP data */
  if (is_3gpp_subsc_avail)
  {
    data.sub_avail = MASK_GPP;
  }
  else
  {
    data.sub_avail = 0;
  }
  if(hybr_pref != SD_SS_HYBR_PREF_NONE)
  {
    data.active_stacks |=   MM_STACK_1_MASK;
  }
  data.gpp_session_type      = MMGSDI_GW_PROV_PRI_SESSION;
  data.acq_order_pref       = acq_order_pref;
  data.srv_domain_pref      = srv_domain_pref;
  data.manual_sys_info.sys_id.id_type = SYS_SYS_ID_TYPE_UNDEFINED;
  if(manual_sys_info_ptr != NULL)
  {
    data.manual_sys_info   = *manual_sys_info_ptr;
  }

  /* 3GPP2 data */
  if (is_3gpp2_subsc_avail)
  {
    data.sub_avail |= MASK_GPP2;
  }
  data.gpp2_session_type     = MMGSDI_1X_PROV_PRI_SESSION;
  data.prl_pref             = prl_pref;
  data.hybr_pref            = hybr_pref;

  /* Send command to MMOC */
  mmoc_cmd_subscription_chgd_new(
    chg_type,
    nam,
    SYS_MODEM_AS_ID_1_MASK,
    SYS_MODEM_DS_PREF_SINGLE_STANDBY,
    cm_get_device_mode(),
    prot_subsc_chg,
    PROT_SUBSC_CHG_NONE,
    PROT_SUBSC_CHG_NONE,
    &data,
    NULL,
    NULL);

} /* mmoc_cmd_subscription_chgd() */


/*===========================================================================

FUNCTION mmoc_cmd_subscription_chgd_new

DESCRIPTION
  This function should be called whenever there is a NAM change or a
  subscription availability change (3GPP or 3GPP2) associated with any of
  the SUBs. This function will queue the subscription changed command to the
  MMOC task.

===========================================================================*/
void                           mmoc_cmd_subscription_chgd_new
(
  mmoc_subsc_chg_e_type                 chg_type,
  byte                                  nam,
  uint8                                 active_subs,
  sys_modem_dual_standby_pref_e_type    ds_pref,
  sys_modem_device_mode_e_type          device_mode,
  prot_subsc_chg_e_type                 prot_subsc_chg1,
  prot_subsc_chg_e_type                 prot_subsc_chg2,
  prot_subsc_chg_e_type                 prot_subsc_chg3,
  mmoc_subs_data_s_type                *subs1_data_ptr,
  mmoc_subs_data_s_type                *subs2_data_ptr,
  mmoc_subs_data_s_type                *subs3_data_ptr
)
{
  mmoc_cmd_msg_s_type                 *msg_ptr;
  mmoc_state_info_s_type              *mmoc_info_ptr = mmoc_get_state_info_ptr();

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                              = MMOC_CMD_SUBSCRIPTION_CHGD;
  msg_ptr->param.subsc_chgd.dev_prop.chg_type    = chg_type;
  msg_ptr->param.subsc_chgd.dev_prop.nam         = nam;
  msg_ptr->param.subsc_chgd.dev_prop.active_subs = active_subs;
  msg_ptr->param.subsc_chgd.dev_prop.device_mode = device_mode;
  msg_ptr->param.subsc_chgd.dev_prop.ds_pref     = ds_pref;
  msg_ptr->param.subsc_chgd.nSubs = 0;

  /* Fill subs info.
  */
  if(subs1_data_ptr != NULL)
  {
    msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_1] = (mmoc_subsc_chgd_sub_prop_s_type *) mmoc_mem_alloc(
          sizeof(mmoc_subsc_chgd_sub_prop_s_type));

    memscpy(msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_1],
            sizeof(*(msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_1])),
            subs1_data_ptr,
            sizeof(mmoc_subs_data_s_type));

    msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_1]->prot_subsc_chg = prot_subsc_chg1;
    msg_ptr->param.subsc_chgd.nSubs++;
  }

#ifdef FEATURE_MMODE_DUAL_SIM
  if(subs2_data_ptr != NULL)
  {
    msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_2] = (mmoc_subsc_chgd_sub_prop_s_type *) mmoc_mem_alloc(
          sizeof(mmoc_subsc_chgd_sub_prop_s_type));

    memscpy(msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_2],
            sizeof(*(msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_2])),
            subs2_data_ptr,
            sizeof(mmoc_subs_data_s_type));

    msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_2]->prot_subsc_chg = prot_subsc_chg2;
    msg_ptr->param.subsc_chgd.nSubs++;
  }
#endif

#ifdef FEATURE_MMODE_TRIPLE_SIM
  if(subs3_data_ptr != NULL)
  {
    msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_3] = (mmoc_subsc_chgd_sub_prop_s_type *) mmoc_mem_alloc(
          sizeof(mmoc_subsc_chgd_sub_prop_s_type));

    memscpy(msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_3],
            sizeof(*(msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_3])),
            subs3_data_ptr,
            sizeof(mmoc_subs_data_s_type));

    msg_ptr->param.subsc_chgd.sub_prop[SYS_MODEM_AS_ID_3]->prot_subsc_chg = prot_subsc_chg3;
    msg_ptr->param.subsc_chgd.nSubs++;
  }
#endif

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

} /* mmoc_cmd_subscription_chgd_new() */


/*===========================================================================

FUNCTION mmoc_cmd_oprt_mode_chgd

DESCRIPTION
  This function will be used to command the MMoC to do operating mode change,
  the new operating mode is specified in the oprt_mode parameter.  This
  function will queue the operating mode changed command to the MMoC task.

===========================================================================*/
void                           mmoc_cmd_oprt_mode_chgd
(

  sys_oprt_mode_e_type           oprt_mode
  /* The operating mode to be switched to.
  */
)
{
  mmoc_cmd_oprt_mode_chgd2(oprt_mode, CM_E911_PENDING_STATE_NONE);

} /* mmoc_cmd_oprt_mode_chgd() */

/*===========================================================================

FUNCTION mmoc_cmd_oprt_mode_chgd2

DESCRIPTION
  This function will be used to command the MMoC to do operating mode change,
  the new operating mode is specified in the oprt_mode parameter.  This
  function will queue the operating mode changed command to the MMoC task.

===========================================================================*/
void  mmoc_cmd_oprt_mode_chgd2(

  sys_oprt_mode_e_type           oprt_mode,
  /* The operating mode to be switched to.
  */
  cm_e911_pending_state_e_type   pending_emerg_state
  /* Indicate pending emergency state 
  */
)
{
  mmoc_cmd_msg_s_type                 *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                       = MMOC_CMD_OPRT_MODE_CHGD;
  msg_ptr->param.oprt_mode_chgd.oprt_mode = oprt_mode;
  msg_ptr->param.oprt_mode_chgd.pending_emerg_state = pending_emerg_state;
  
  MMOC_MSG_HIGH_2("CM->MMOC:OPRT_MODE_CHGD,oprt_mode %d, pending_emerg_state %d",
                   oprt_mode,pending_emerg_state);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

  return;

} /* mmoc_cmd_oprt_mode_chgd() */


/*===========================================================================

FUNCTION mmoc_cmd_pref_sys_chgd6

DESCRIPTION
  This function is used to command the MMoC to do Preferred system selection
  change. This function will queue the preferred system selection changed
  command to the MMoC Task.

===========================================================================*/
void                           mmoc_cmd_pref_sys_chgd6
(
  mm_sub_stk_id_s_type           mm_id,
  /* Indicates the SS-Instance ( SS_MAIN or SS_HDR) this
  ** pref_sys_chgd command is intended for.
  */

  sd_ss_pref_reas_e_type         pref_reas,
  /* Reason for changing the SS-Preference.
  */

  sd_ss_orig_mode_e_type         orig_mode,
  /* The origination mode (normal, OTASP, emergency, other, etc.)
  ** that is associated with the selected NAM.
  */

  sd_ss_mode_pref_e_type         mode_pref,
  /* The mode preference that is associated with the selected NAM.
  */

  sd_ss_band_pref_e_type         band_pref,
  /* Band preference that is associated with the selected NAM.
  */

  sys_lte_band_mask_e_type       lte_band_pref,
  /* LTE band preference that is associated with the selected NAM.
  */

  sd_ss_band_pref_e_type         tds_band_pref,
  /* TD-SCDMA band preference that is associated with the selected NAM.
  */

  sd_ss_prl_pref_e_type          prl_pref,
  /* PRL preference that is associated with the selected NAM.
  */

  sd_ss_roam_pref_e_type         roam_pref,
  /* Roam preference that is associated with the selected NAM.
  */

  sd_ss_hybr_pref_e_type         hybr_pref,
  /* New hybrid preference (none, CDMA/HDR).
  */

  sd_band_e_type                 otasp_band,
  /* The band-class that is associated with OTASP origination mode.
  ** Note that this parameter is ignored if orig_mode != OTASP.
  */

  sd_blksys_e_type                otasp_blksys,
  /* The PCS frequency block/Cellular system that is associated
  ** with OTASP origination mode. Note that this parameter is ignored
  ** if orig_mode != OTASP.
  */

  sd_ss_avoid_sys_e_type          avoid_type,
  /* Type of the system to be avoided, valid only
  ** if pref_reason = SD_SS_PREF_REAS_AVOID_SYS.
  */

  dword                            avoid_time,
  /* Time in seconds for which the system is to be avoided, valid
  ** only if pref_reason = SD_SS_PREF_REAS_AVOID_SYS.
  */

  const sys_manual_sys_info_s_type *manual_sys_info_ptr,
  /* Pointer to struct that defines the manual system.
  */

  sd_ss_srv_domain_pref_e_type      srv_domain_pref,
  /* Service domain preference that is associated with the selected
  ** NAM.
  ** Applies only for GSM/WCDMA modes.
  */

  sd_ss_acq_order_pref_e_type       acq_order_pref,
  /* Preference for the order of acquisition ( WCDMA before GSM,
           ** GSM before WCDMA etc).
           */

  sd_ss_pref_update_reason_e_type    pref_update_reas,
  /* Preference change reason
  */

  sys_addtl_action_s_type            *addl_action,
  /*
  To indicate sub action to be performed by MMOC during pref sys chgd cmd
  */

  sd_ss_mode_pref_e_type             user_mode_pref,
  /**< The user mode preference that is associated with the selected NAM.
  */

  const sd_rat_acq_order_s_type     *rat_acq_order_ptr,
  /**< Rat acquisition order including LTE.
  */

  uint16                             req_id,
  /**< System Selection Preferences request id.
  */

  sd_ss_pref_camp_mode_e_type  camp_only_pref,

  sys_csg_id_type                    csg_id,
  /**< CSG identifier */

  sys_radio_access_tech_e_type       csg_rat,
  /**< RAT specified for CSG */

  sys_voice_domain_pref_e_type       voice_domain_pref,
  /**< Voice domain pref */

  sd_ss_fullrat_pref_e_type          fullrat_status,
  /* Indication to SD whether ue is in full rat
   */

  uint32                             rat_capability
  /* Current rat capability
  */

)
{

  mmoc_cmd_pref_sys_chgd7( mm_id,
                           pref_reas,
                           orig_mode,
                           mode_pref,
                           band_pref,
                           lte_band_pref,
                           tds_band_pref,
                           prl_pref,
                           roam_pref,
                           hybr_pref,
                           otasp_band,
                           otasp_blksys,
                           avoid_type,
                           avoid_time,
                           manual_sys_info_ptr,
                           srv_domain_pref,
                           acq_order_pref,
                           pref_update_reas,
                           addl_action,
                           user_mode_pref,
                           rat_acq_order_ptr,
                           req_id,
                           camp_only_pref,
                           csg_id,
                           csg_rat,
                           voice_domain_pref,
                           fullrat_status,
                           rat_capability,
                           CM_E911_PENDING_STATE_OFF);
  
}

/**===========================================================================

@FUNCTION mmoc_cmd_pref_sys_chgd7

@DESCRIPTION
  This function is used to command the MMoC to do Preferred system selection
  change. This function will queue the preferred system selection changed
  command to the MMoC Task.

  lte_band_pref and CSG are supported in this function.

@DEPENDENCIES
  None.

@RETURN VALUE
  None.

@SIDE EFFECTS
  None.
===========================================================================*/
extern  void                           mmoc_cmd_pref_sys_chgd7
(
  mm_sub_stk_id_s_type           mm_id,
  /* Indicates the SS-Instance ( SS_MAIN or SS_HDR) this
  ** pref_sys_chgd command is intended for.
  */

  sd_ss_pref_reas_e_type         pref_reas,
  /* Reason for changing the SS-Preference.
  */

  sd_ss_orig_mode_e_type         orig_mode,
  /* The origination mode (normal, OTASP, emergency, other, etc.)
  ** that is associated with the selected NAM.
  */

  sd_ss_mode_pref_e_type         mode_pref,
  /* The mode preference that is associated with the selected NAM.
  */

  sd_ss_band_pref_e_type         band_pref,
  /* Band preference that is associated with the selected NAM.
  */

  sys_lte_band_mask_e_type       lte_band_pref,
  /* LTE band preference that is associated with the selected NAM.
  */

  sd_ss_band_pref_e_type         tds_band_pref,
  /* TD-SCDMA band preference that is associated with the selected NAM.
  */

  sd_ss_prl_pref_e_type          prl_pref,
  /* PRL preference that is associated with the selected NAM.
  */

  sd_ss_roam_pref_e_type         roam_pref,
  /* Roam preference that is associated with the selected NAM.
  */

  sd_ss_hybr_pref_e_type         hybr_pref,
  /* New hybrid preference (none, CDMA/HDR).
  */

  sd_band_e_type                 otasp_band,
  /* The band-class that is associated with OTASP origination mode.
  ** Note that this parameter is ignored if orig_mode != OTASP.
  */

  sd_blksys_e_type                otasp_blksys,
  /* The PCS frequency block/Cellular system that is associated
  ** with OTASP origination mode. Note that this parameter is ignored
  ** if orig_mode != OTASP.
  */

  sd_ss_avoid_sys_e_type          avoid_type,
  /* Type of the system to be avoided, valid only
  ** if pref_reason = SD_SS_PREF_REAS_AVOID_SYS.
  */

  dword                            avoid_time,
  /* Time in seconds for which the system is to be avoided, valid
  ** only if pref_reason = SD_SS_PREF_REAS_AVOID_SYS.
  */

  const sys_manual_sys_info_s_type *manual_sys_info_ptr,
  /* Pointer to struct that defines the manual system.
  */

  sd_ss_srv_domain_pref_e_type      srv_domain_pref,
  /* Service domain preference that is associated with the selected
  ** NAM.
  ** Applies only for GSM/WCDMA modes.
  */

  sd_ss_acq_order_pref_e_type       acq_order_pref,
  /* Preference for the order of acquisition ( WCDMA before GSM,
           ** GSM before WCDMA etc).
           */

  sd_ss_pref_update_reason_e_type    pref_update_reas,
  /* Preference change reason
  */

  sys_addtl_action_s_type            *addl_action,
  /*
  To indicate sub action to be performed by MMOC during pref sys chgd cmd
  */

  sd_ss_mode_pref_e_type             user_mode_pref,
  /**< The user mode preference that is associated with the selected NAM.
  */

  const sd_rat_acq_order_s_type     *rat_acq_order_ptr,
  /**< Rat acquisition order including LTE.
  */

  uint16                             req_id,
  /**< System Selection Preferences request id.
  */

  sd_ss_pref_camp_mode_e_type  camp_only_pref,

  sys_csg_id_type                    csg_id,
  /**< CSG identifier */

  sys_radio_access_tech_e_type       csg_rat,
  /**< RAT specified for CSG */

  sys_voice_domain_pref_e_type       voice_domain_pref,
  /**< Voice domain pref */

  sd_ss_fullrat_pref_e_type          fullrat_status,
    /* Indication to SD whether ue is in full rat
     */

  uint32                             rat_capability,
  /* Current rat capability
  */

  cm_e911_pending_state_e_type       pending_emerg_state
    /* Indicate pending emergency state 
              */
)
{
  mmoc_cmd_msg_s_type                 *msg_ptr;
  prot_cmd_pref_sys_chgd_s_type       pref_sys_chgd;
  uint8                               i = 0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                              = MMOC_CMD_PROT_GEN_CMD;
  msg_ptr->param.gen_prot_cmd.cmd_type           = PROT_GEN_CMD_PREF_SYS_CHGD;
  msg_ptr->param.gen_prot_cmd.asubs_id           = mm_id.asubs_id;
  msg_ptr->param.gen_prot_cmd.stk_id             = mm_id.stk_id;

  pref_sys_chgd.pref_reas                        = pref_reas;
  pref_sys_chgd.orig_mode                        = orig_mode;
  pref_sys_chgd.mode_pref                        = mode_pref;
  pref_sys_chgd.band_pref                        = band_pref;
  pref_sys_chgd.lte_band_pref                    = lte_band_pref;
  pref_sys_chgd.tds_band_pref                    = tds_band_pref;
  pref_sys_chgd.prl_pref                         = prl_pref;
  pref_sys_chgd.roam_pref                        = roam_pref;
  pref_sys_chgd.hybr_pref                        = hybr_pref;
  pref_sys_chgd.otasp_band                       = otasp_band;
  pref_sys_chgd.otasp_blksys                     = otasp_blksys;
  pref_sys_chgd.avoid_type                       = avoid_type;
  pref_sys_chgd.avoid_time                       = avoid_time;
  pref_sys_chgd.manual_sys_info.sys_id.id_type   = SYS_SYS_ID_TYPE_UNDEFINED;
  if ( manual_sys_info_ptr != NULL )
  {
    pref_sys_chgd.manual_sys_info                = *manual_sys_info_ptr;
  }
  pref_sys_chgd.domain_pref                      = srv_domain_pref;
  pref_sys_chgd.acq_order_pref                   = acq_order_pref;
  pref_sys_chgd.pref_update_reas                 = pref_update_reas;
  if (addl_action != NULL)
  {
    pref_sys_chgd.addl_action                      = *addl_action;
  }
  else
  {
    pref_sys_chgd.addl_action.action             = MMOC_PREF_SYS_ADDL_ACTION_NONE;
    pref_sys_chgd.addl_action.ue_mode            = SYS_UE_MODE_NONE;
    pref_sys_chgd.addl_action.is_ue_mode_substate_srlte = FALSE;
    pref_sys_chgd.addl_action.is_tau_reqd = FALSE;
  }
  pref_sys_chgd.user_mode_pref                   = user_mode_pref;
  pref_sys_chgd.sys_sel_pref_req_id              = req_id;
  pref_sys_chgd.camp_mode_pref                   = camp_only_pref;
  pref_sys_chgd.fullrat_status                    = fullrat_status;
  pref_sys_chgd.csg_id                           = csg_id;
  pref_sys_chgd.csg_rat                          = csg_rat;
  pref_sys_chgd.voice_domain_pref                = voice_domain_pref;
  pref_sys_chgd.rat_capability                  = rat_capability;
  pref_sys_chgd.pending_emerg_state              = pending_emerg_state;

  if(rat_acq_order_ptr != NULL)
  {
    pref_sys_chgd.rat_acq_order.version = rat_acq_order_ptr->version;
    pref_sys_chgd.rat_acq_order.num_rat = rat_acq_order_ptr->num_rat;

    for(i=0; i < SYS_MAX_ACQ_PREF_RAT_LIST_NUM; i++)
    {
      pref_sys_chgd.rat_acq_order.acq_sys_mode[i] = \
          rat_acq_order_ptr->acq_sys_mode[i];
    }
  }
  else
  {
    /* If no acquisition order, just fill up the acq_order with NONE */
    pref_sys_chgd.rat_acq_order.version = 0xFF;
    pref_sys_chgd.rat_acq_order.num_rat = 0;

    for(i=0; i < SYS_MAX_ACQ_PREF_RAT_LIST_NUM; i++)
    {
      pref_sys_chgd.rat_acq_order.acq_sys_mode[i] = SYS_SYS_MODE_NONE;
    }

  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Copy the parameters into generic command struct.
  */
  msg_ptr->param.gen_prot_cmd.param.pref_sys_chgd  = pref_sys_chgd;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* To detect duplicate pref_sys_chgd commands, set the SIG.
  */
  (void)rex_set_sigs( mmoc_tcb_ptr, MMOC_DUP_CMD_REM_SIG );

} /* mmoc_cmd_pref_sys_chgd7() */


/**===========================================================================

FUNCTION mmoc_cmd_get_networks_extn

DESCRIPTION
  The function will be used to command the MMoC to get the networks on
  GSM/WCDMA/TD-SCDMA/LTE technologies. This function will queue the Get networks
  command to the MMoC task.

===========================================================================*/
void                           mmoc_cmd_get_networks_extn
(

  sd_ss_mode_pref_e_type         mode_pref,
  /* Mode preference that is associated with the Get networks
  ** command.
  */


  sd_ss_band_pref_e_type         band_pref,
  /* Band preference that is associated with the Get networks
  ** command.
  */

  sys_lte_band_mask_e_type       lte_band_pref,
  /* Band preference that is associated with the Get networks
  ** command.
  */


  sd_ss_band_pref_e_type         tds_band_pref,
  /* TD-SCDMA band preference that is associated with the Get networks
  ** command.
  */

  mm_sub_stk_id_s_type           mm_id,
  /**< System Selection */

  sd_network_list_type_e_type    list_type,
  /**< Network List Type */

  boolean                        ignore_manual_bst,
  /**< Ignore Manual BST Band Restriction */

  boolean                        is_requeue,

  sys_periodic_frequency_scan_info_s_type  *pf_scan_info,

  sys_incremental_results_s_type   incremental_results,

  uint16                           max_search_time

)
{
  mmoc_cmd_msg_s_type                 *msg_ptr;
  prot_cmd_gw_get_net_s_type          *get_net_ptr = NULL;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                    = MMOC_CMD_PROT_GEN_CMD;
  msg_ptr->param.gen_prot_cmd.cmd_type = PROT_GEN_CMD_GET_NETWORKS_GW;
  msg_ptr->param.gen_prot_cmd.asubs_id = mm_id.asubs_id;
  msg_ptr->param.gen_prot_cmd.stk_id   = mm_id.stk_id;

  get_net_ptr = &msg_ptr->param.gen_prot_cmd.param.gw_get_net;
  get_net_ptr->mode_pref         = mode_pref;
  get_net_ptr->band_pref         = band_pref;
  get_net_ptr->lte_band_pref     = lte_band_pref;
  get_net_ptr->tds_band_pref     = tds_band_pref;
  get_net_ptr->network_list_type = list_type;
  get_net_ptr->ignore_manual_bst = ignore_manual_bst;
  get_net_ptr->is_requeue        = is_requeue;
  get_net_ptr->pf_scan_info      = pf_scan_info;
  get_net_ptr->incremental_results = incremental_results;
  get_net_ptr->max_search_time = max_search_time;

  MMOC_MSG_HIGH_4("CM->MMOC: GET_NET: sub %d stk %d ignore_mbst %d list_type %d",
                  mm_id.asubs_id,
                  mm_id.stk_id,
                  ignore_manual_bst,
                  list_type);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

}

#if defined (FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)

/*===========================================================================

FUNCTION mmoc_cmd_deactivate_1xcsfb_prot

DESCRIPTION
  The function will be used to deactivate 1xCSFB through MMoC

===========================================================================*/
void                           mmoc_cmd_deactivate_1xcsfb_prot
(
  prot_deact_e_type deact_reason
)
{
  mmoc_cmd_msg_s_type                 *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.  */

  msg_ptr->cmd.name                    = MMOC_CMD_DEACT_1XCSFB_PROT;

  msg_ptr->param.cdma_csfb_deact.deact_reason = deact_reason;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command */
  mmoc_send_cmd( msg_ptr );

  return;

} /* mmoc_cmd_deactivate_1xcsfb_prot() */

#endif

/*===========================================================================

FUNCTION mmoc_cmd_term_get_networks_gw

DESCRIPTION
  This function will be used to inform the MMoC that the user has terminated
  the Get networks request on GSM/WCDMA networks. This function will queue
  the Terminate get networks command to the MMoC task.

===========================================================================*/
void                           mmoc_cmd_term_get_networks_gw
(
  mm_sub_stk_id_s_type    mm_id
)
{
  mmoc_cmd_msg_s_type    *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                    = MMOC_CMD_PROT_GEN_CMD;
  msg_ptr->param.gen_prot_cmd.cmd_type = PROT_GEN_CMD_TERM_GET_NETWORKS_GW;
  msg_ptr->param.gen_prot_cmd.asubs_id = mm_id.asubs_id;
  msg_ptr->param.gen_prot_cmd.stk_id   = mm_id.stk_id;

  MMOC_MSG_HIGH_2("CM->MMOC: GET_NET: TERMINATE: sub %d stk %d",
                  mm_id.asubs_id,
                  mm_id.stk_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

} /* mmoc_cmd_term_get_networks_gw(SD_SS_MAIN) */


/*===========================================================================

FUNCTION mmoc_cmd_prot_resel_ind

DESCRIPTION
   This function will be used by protocol (LTE RRC) to inform MMoC that it
   wants to start reselection process.

===========================================================================*/
void                           mmoc_cmd_prot_resel_ind
(
  mm_sub_stk_id_s_type    mm_id
  /* Stack which needs to be activated
  */
)
{
#ifdef FEATURE_MMODE_LTE_RESEL
  mmoc_cmd_msg_s_type                  *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                    = MMOC_CMD_PROT_GEN_CMD;
  msg_ptr->param.gen_prot_cmd.cmd_type = PROT_GEN_CMD_IRAT_TO_GWL;
  msg_ptr->param.gen_prot_cmd.asubs_id = mm_id.asubs_id;
  msg_ptr->param.gen_prot_cmd.stk_id   = mm_id.stk_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

#endif

} /*  mmoc_cmd_prot_resel_ind() */


/*===========================================================================

FUNCTION mmoc_cmd_prot_gw_sim_state_update_ind

DESCRIPTION
   This function will be used by cmregprx to inform MMoC that it needs
   to update the sim state on T3245 timer expiry indication from NAS.

=============================================================================*/
void                           mmoc_cmd_prot_gw_sim_state_update_ind
(
  prot_cmd_gw_sim_state_e_type         sim_state,
  /* New sim state reported from REG
  */

  mm_sub_stk_id_s_type                 mm_id
  /* Stack which needs to be activated
  */
)
{
  mmoc_cmd_msg_s_type                  *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                    = MMOC_CMD_PROT_GEN_CMD;
  msg_ptr->param.gen_prot_cmd.cmd_type = PROT_GEN_CMD_GW_SIM_STATE;
  msg_ptr->param.gen_prot_cmd.asubs_id = mm_id.asubs_id;
  msg_ptr->param.gen_prot_cmd.stk_id   = mm_id.stk_id;
  msg_ptr->param.gen_prot_cmd.param.gw_sim_state = sim_state;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

} /*  mmoc_cmd_prot_gw_sim_state_update_ind() */



/*===========================================================================

FUNCTION mmoc_cmd_prot_local_deact_ind

DESCRIPTION
   This function will be used to locally deactivate the stack.

   In SGLTE, when NAS is performing reselection (L->G) or PS-Domain-Transfer
   happens, it will deactivate HYBR-2 and transfer service over to Main.

===========================================================================*/
void                           mmoc_cmd_prot_local_deact_ind
(
  mm_sub_stk_id_s_type    mm_id
  /* Stack which needs to be activated
  */
)
{
  mmoc_cmd_msg_s_type                  *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                    = MMOC_CMD_PROT_GEN_CMD;
  msg_ptr->param.gen_prot_cmd.cmd_type = PROT_GEN_CMD_LOCAL_DEACT;
  msg_ptr->param.gen_prot_cmd.asubs_id = mm_id.asubs_id;
  msg_ptr->param.gen_prot_cmd.stk_id   = mm_id.stk_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

} /*  mmoc_cmd_prot_local_deact_ind() */


/*===========================================================================

FUNCTION mmoc_cmd_prot_local_act_ind

DESCRIPTION
   This function will be used to locally activate the stack.

   In SGLTE, when NAS is performing reselection (G->L), it will activate HYBR-2 and transfer
   CS service over to hybr2.

===========================================================================*/
void                           mmoc_cmd_prot_local_act_ind
(
  mm_sub_stk_id_s_type    mm_id
  /* Stack which needs to be activated
  */
)
{
  mmoc_cmd_msg_s_type                  *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                    = MMOC_CMD_PROT_GEN_CMD;
  msg_ptr->param.gen_prot_cmd.cmd_type = PROT_GEN_CMD_LOCAL_ACT;
  msg_ptr->param.gen_prot_cmd.asubs_id = mm_id.asubs_id;
  msg_ptr->param.gen_prot_cmd.stk_id   = mm_id.stk_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

} /*  mmoc_cmd_prot_local_deact_ind() */



/*===========================================================================

FUNCTION mmoc_cmd_wakeup_from_pwr_save

DESCRIPTION
  This function will be used to command the MMoC to wakeup the phone from
  power save.

===========================================================================*/
void                           mmoc_cmd_wakeup_from_pwr_save
(

  mm_sub_stk_id_s_type    mm_id
  /* Wakeup the 'ss' System Selection instance from power save.
  */

)
{
  mmoc_cmd_msg_s_type                 *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                       = MMOC_CMD_PROT_GEN_CMD;
  msg_ptr->param.gen_prot_cmd.cmd_type    = PROT_GEN_CMD_WAKEUP_FROM_PWR_SAVE;
  msg_ptr->param.gen_prot_cmd.asubs_id    = mm_id.asubs_id;
  msg_ptr->param.gen_prot_cmd.stk_id      = mm_id.stk_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

} /* mmoc_cmd_wakeup_from_pwr_save() */


/*===========================================================================

FUNCTION  mmoc_cmd_suspend_stack_new

DESCRIPTION
  This function will be used to command the MMoC to suspend or resume
  operation on a given system selection instance when emergency call is dialed on other instance.

===========================================================================*/
void                            mmoc_cmd_suspend_stack_new
(

  mm_sub_stk_id_s_type           mm_id,
  /* System Selection instance that needs to be suspended
  */

  boolean                        is_suspend,
  /* whether to suspend or resume
  */
  boolean                        ignore_protocol_activate,
  /* Whether to activate protocol or not during resume
  */
  prot_deact_e_type              susp_reason
  /* Reason for suspension
  */

)
{
  mmoc_cmd_msg_s_type            *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                          = MMOC_CMD_SUSPEND_STACK;
  msg_ptr->param.suspend_stack.mm_id         = mm_id;
  msg_ptr->param.suspend_stack.is_suspend    = is_suspend;
  msg_ptr->param.suspend_stack.ignore_protocol_activate = ignore_protocol_activate ;
  msg_ptr->param.suspend_stack.susp_reason   = susp_reason;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_MSG_HIGH_4("SUSPEND_RESUME: sub %d stk %d, is_suspend %d, susp_reason %d",
                  mm_id.asubs_id,
                  mm_id.stk_id,
                  is_suspend,
                  susp_reason);

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

} /* mmoc_cmd_suspend_stack_new() */


/*===========================================================================

FUNCTION  mmoc_cmd_suspend_stack

DESCRIPTION
  This function will be used to command the MMoC to suspend or resume
  operation on a given system selection instance.

===========================================================================*/
void                            mmoc_cmd_suspend_stack
(

  mm_sub_stk_id_s_type           mm_id,
  /* System Selection instance that needs to be suspended
  */

  boolean                        is_suspend,
  /* whether to suspend or resume
  */
  boolean                        ignore_protocol_activate
  /* Whether to activate protocol or not during resume
  */
)
{
  mmoc_cmd_suspend_stack_new(mm_id, is_suspend, ignore_protocol_activate, PROT_DEACT_LOCAL_DETACH);
} /* mmoc_cmd_suspend_ss() */


/*===========================================================================

FUNCTION mmoc_cmd_dual_standby_chgd

DESCRIPTION
  This function will be used to command the MMoC to change dual standby
  preferences

===========================================================================*/
void                           mmoc_cmd_dual_standby_chgd
(
  sys_modem_dual_standby_pref_e_type   standby_pref,
  /* New Standby Preference */

  uint8                                active_subs,
  /* If single standby, which is the active ss */

  sys_modem_device_mode_e_type    device_mode
)
{
  mmoc_cmd_msg_s_type                 *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                         = MMOC_CMD_DUAL_STANDBY_CHGD;
  msg_ptr->param.dual_standby_chgd.standby_pref       = standby_pref;
  msg_ptr->param.dual_standby_chgd.active_subs = active_subs;
  msg_ptr->param.dual_standby_chgd.device_mode = device_mode;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

  return;

} /* mmoc_cmd_dual_standby_chgd() */

/*===========================================================================

FUNCTION mmoc_cmd_deact_from_dormant

DESCRIPTION
  This function will be used to command the MMoC to  protocols in
  given stack from its dormant state.

===========================================================================*/
void                           mmoc_cmd_deact_from_dormant
(
  mm_sub_stk_id_s_type     mm_id,
  /* Wakeup the stack instance from power save.
  */

  sd_ss_mode_pref_e_type   mode
  /* Modes which have to be deactivated from dormant state
  */
)
{
  mmoc_cmd_msg_s_type                 *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                       = MMOC_CMD_DEACT_FROM_DORMANT;
  msg_ptr->param.deact_dormant.mm_id      = mm_id;
  msg_ptr->param.deact_dormant.prot       = mode;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */

  mmoc_send_cmd( msg_ptr );

  return;

} /* mmoc_cmd_deact_from_dormant() */

/*=========================================================================

FUNCTION mmoc_cmd_suspend_resume_ind

DESCRIPTION
  This function will be used to command the MMoC to start suspend_resume
  operation on all protocols.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                           mmoc_cmd_suspend_resume_ind()

{
  mmoc_cmd_msg_s_type                 *msg_ptr;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                       = MMOC_CMD_SUSPEND_RESUME_IND;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

  return;
}


/*===========================================================================
=============================================================================
=============================================================================
============================== Protocols -> MMOC reports ====================
=============================================================================
=============================================================================
===========================================================================*/

/* The following section defines the API when the clients ( example protocols
** ) have to communicate to MMoC using the report queue.
*/


/*===========================================================================

FUNCTION mmoc_rpt_prot_deactd_cnf

DESCRIPTION
  This function will be used to confirm that the MMoC that the protocol has
  been deactivated. This function will queue the protocol deactivated report
  to the MMoC task.

  Note! This function should be called only in response to protocol
  deactivate request from MMoC.

===========================================================================*/
void                           mmoc_rpt_prot_deactd_cnf
(

  prot_trans_type                trans_id
  /* Protocols set this same as the trans_id received in their
  ** Deactivate request.
  */

)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                       = MMOC_RPT_PROT_DEACTD_CNF;
  msg_ptr->param.prot_deactd_cnf.trans_id = trans_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );

  return;

} /* mmoc_rpt_prot_deactd_cnf() */

#if defined (FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)

/*===========================================================================

FUNCTION mmoc_rpt_csfb_prot_deactd_cnf

DESCRIPTION
  This function will be used to confirm that the MMoC that the protocol has
  been deactivated. This function will queue the protocol deactivated report
  to the MMoC task.

  Note! This function should be called only in response to protocol
  deactivate request from MMoC.

===========================================================================*/
void                           mmoc_rpt_1xcsfb_prot_deactd_cnf
(

  prot_trans_type                trans_id
  /* Protocols set this same as the trans_id received in their
  ** Deactivate request.
  */

)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                       = MMOC_RPT_1XCSFB_PROT_DEACTD_CNF;
  msg_ptr->param.prot_deactd_cnf.trans_id = trans_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );

  return;

} /* mmoc_rpt_1xcsfb_prot_deactd_cnf() */

#endif

/*===========================================================================

FUNCTION mmoc_rpt_irat_hold_user_act_cnf

DESCRIPTION
  This function will be used to confirm that CM has marked to pend any user ommand rill MMOC send UNHOLD_USER_ACT

===========================================================================*/
void                           mmoc_rpt_irat_hold_user_act_cnf
(
  boolean irat_ret_status
)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                       = MMOC_RPT_IRAT_HOLD_USER_ACT_CNF;

  msg_ptr->param.lte_1x_irat_cnf.irat_ret_status = irat_ret_status;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );

  return;

} /* mmoc_rpt_1xcsfb_prot_deactd_cnf() */


/*===========================================================================

FUNCTION mmoc_rpt_prot_auto_deactd_ind

DESCRIPTION
   This function will be used to inform the MMoC that the protocol has been
  deactivated autonomously. This function will queue the deactivated
  indication report to the MMoC task.

  Note! This function should not called in response to protocol deactivate
  request from MMoC.

===========================================================================*/
void                           mmoc_rpt_prot_auto_deactd_ind
(
  prot_deact_e_type              deactd_reason,
  /* Reason that the protocol was deactivated.
  */

  prot_state_e_type              prot_state
  /* Protocol which sent the autonomous deactivation.
  */
)
{
  mm_sub_stk_id_s_type mm_id;
  mm_id.asubs_id = SYS_MODEM_AS_ID_1;
  mm_id.stk_id   = MM_STACK_0;

  /* Set ss = SD_SS_MAX. MMOC will consider the stack on which this protocol is active */
  mmoc_rpt_prot_auto_deactd_ind_ss(deactd_reason, prot_state, mm_id);

} /* mmoc_rpt_prot_auto_deactd_ind() */

/*===========================================================================

FUNCTION mmoc_rpt_prot_auto_deactd_ind_subs

DESCRIPTION
   This function will be used to inform the MMoC that the protocol has been
  deactivated autonomously. This function will queue the deactivated
  indication report to the MMoC task.

  Note! This function should not called in response to protocol deactivate
  request from MMoC.

===========================================================================*/
void                           mmoc_rpt_prot_auto_deactd_ind_subs
(
  prot_deact_e_type              deactd_reason,
  /* Reason that the protocol was deactivated.
  */

  prot_state_e_type              prot_state,
  /* Protocol which sent the autonomous deactivation.
  */

  sys_modem_as_id_e_type           asubs_id
  /* Software stack which sent the autonomous deactivation.
  */
)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;
  mm_sub_stk_id_s_type                  mm_id;

  MMOC_ASSERT( deactd_reason != PROT_DEACT_PWR_DOWN   &&
               deactd_reason != PROT_DEACT_RESET      &&
               deactd_reason != PROT_DEACT_LPM        &&
               deactd_reason != PROT_DEACT_OFFLINE    &&
               deactd_reason != PROT_DEACT_MAX
             );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mm_id.asubs_id = asubs_id;
  mm_id.stk_id = MM_STACK_0;

  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                     = MMOC_RPT_PROT_AUTO_DEACTD_IND;
  msg_ptr->param.prot_deactd_ind.reason = deactd_reason;
  msg_ptr->param.prot_deactd_ind.prot_state = prot_state;
  msg_ptr->param.prot_deactd_ind.mm_id      = mm_id;

  MMOC_MSG_HIGH_4("Sending AUTO_DEACTD_IND, sub %d stk %d, reason=%d, prot_state=%d",
                  mm_id.asubs_id,
                  mm_id.stk_id,
                  deactd_reason,
                  prot_state);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );

} /* mmoc_rpt_prot_auto_deactd_ind_ss() */


/*===========================================================================

FUNCTION mmoc_rpt_prot_auto_deactd_ind_ss

DESCRIPTION
   This function will be used to inform the MMoC that the protocol has been
  deactivated autonomously. This function will queue the deactivated
  indication report to the MMoC task.

  Note! This function should not called in response to protocol deactivate
  request from MMoC.

===========================================================================*/
void                           mmoc_rpt_prot_auto_deactd_ind_ss
(
  prot_deact_e_type              deactd_reason,
  /* Reason that the protocol was deactivated.
  */

  prot_state_e_type              prot_state,
  /* Protocol which sent the autonomous deactivation.
  */

  mm_sub_stk_id_s_type           mm_id
  /* Software stack which sent the autonomous deactivation.
  */
)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;

  MMOC_ASSERT( deactd_reason != PROT_DEACT_PWR_DOWN   &&
               deactd_reason != PROT_DEACT_RESET      &&
               deactd_reason != PROT_DEACT_LPM        &&
               deactd_reason != PROT_DEACT_OFFLINE    &&
               deactd_reason != PROT_DEACT_MAX
             );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                     = MMOC_RPT_PROT_AUTO_DEACTD_IND;
  msg_ptr->param.prot_deactd_ind.reason = deactd_reason;
  msg_ptr->param.prot_deactd_ind.prot_state = prot_state;
  msg_ptr->param.prot_deactd_ind.mm_id      = mm_id;

  MMOC_MSG_HIGH_4("Sending AUTO_DEACTD_IND, sub %d stk %d, reason=%d, prot_state=%d",
                  mm_id.asubs_id,
                  mm_id.stk_id,
                  deactd_reason,
                  prot_state);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );

} /* mmoc_rpt_prot_auto_deactd_ind_ss() */

/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_rpt_ph_stat_chgd_cnf2

DESCRIPTION
  This function should be used by the protocols to acknowledge the MMoC that
  the "phone status changed (due to either NAM change, Subscription
  availability change(GSM/WCDMA or CDMA/AMPS/HDR) or operating mode change
  request. This function will queue the phone status changed confirmation
  report to the MMoC task.

  Note! This should be called only after completing the necessary
  housekeeping actions based on the request.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                           mmoc_rpt_ph_stat_chgd_cnf2
(
  prot_trans_type                trans_id,
  /* Protocols set this same as the trans_id received in the phone
  ** status changed ack. trans_id cannot be MMOC_TRANS_AUTONOMOUS.
  */

  mmoc_mode_e_type               prot,
  /* Protocol responding to the Phone status changed command.
  */

  mmoc_ph_stat_e_type            ph_stat,
  /**< status of the command - success or error code. */

  sys_modem_as_id_e_type         as_id
)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                   = MMOC_RPT_PH_STAT_CHGD_CNF;
  msg_ptr->param.ph_stat_cnf.trans_id = trans_id;
  msg_ptr->param.ph_stat_cnf.prot     = prot;
  msg_ptr->param.ph_stat_cnf.mc_status = ph_stat;
  msg_ptr->param.ph_stat_cnf.as_id = as_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );


  return;

} /* mmoc_rpt_ph_stat_chgd_cnf2() */

/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_cmd_prot_redir_ind

DESCRIPTION
   This function will be used by protocol to inform MMoC that it wants to
   start redirection process.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                           mmoc_cmd_prot_redir_ind
(

  sys_modem_as_id_e_type         asubs_id,

  prot_act_e_type                actd_reason,
  /* Reason that the protocol was activated.
  */

  prot_state_e_type              prot_state
  /* Protocol which sent the autonomous activation for redirection.
  */

)
{
  mmoc_cmd_msg_s_type                  *msg_ptr;


  MMOC_ASSERT( actd_reason == PROT_ACT_GWL_REDIR );


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                          = MMOC_CMD_PROT_REDIR_IND;
  msg_ptr->param.prot_redir_ind.actd_reason  = actd_reason;
  msg_ptr->param.prot_redir_ind.prot_state   = prot_state;
  msg_ptr->param.prot_redir_ind.asubs_id     = asubs_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );


  /* To detect duplicate lte to DO redirection commands, set the SIG.
  */
  (void)rex_set_sigs( mmoc_tcb_ptr, MMOC_DUP_CMD_REM_SIG );

  return;


} /* mmoc_cmd_prot_redir_ind() */

#if defined (FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
/* <EJECT> */

/*===========================================================================

FUNCTION mmoc_cmd_prot_handover_ind

DESCRIPTION
   This indication is sent when TCH is acquired and HOCM is acked by n/w
   along with this, CP passes SID/NID/BAND/CHAN/MCC ,
   SID/NID received from SIB8 which we have validated / from UHDM if they are received.
   Band/Chan received from UHDM
   MCC if its included in C2K Params

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/


void                           mmoc_cmd_prot_handover_ind
(

  sys_modem_as_id_e_type         asubs_id,

  prot_act_e_type                actd_reason, //  PROT_ACT_LTE_HANDOVER
  /* Reason that the protocol was activated.
  */

  prot_state_e_type              prot_state, // PROT_STATE_ONLINE_CDMA
  /* Protocol which sent the autonomous activation for Handover.
  */
  sd_ho_sys_param_s_type  sys_param
  /* handovered system params */
)
{
  mmoc_cmd_msg_s_type                  *msg_ptr;


  MMOC_ASSERT( actd_reason == PROT_ACT_LTE_HANDOVER );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                          = MMOC_CMD_PROT_HO_IND;
  msg_ptr->param.prot_ho_ind.actd_reason  = actd_reason;
  msg_ptr->param.prot_ho_ind.prot_state   = prot_state;
  msg_ptr->param.prot_ho_ind.sys_param    = sys_param;
  msg_ptr->param.prot_ho_ind.asubs_id     = asubs_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

  return;

} /* mmoc_cmd_prot_handover_ind() */
#endif

/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_rpt_prot_auto_actd_ind

DESCRIPTION
  This function will be used to inform the MMoC that the protocol has been
  activated autonomously. This function will queue the activated
  indication report to the MMoC task.

  Note! This function should not called in response to protocol activate
  request from MMoC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                           mmoc_rpt_prot_auto_actd_ind
(

  prot_act_e_type              actd_reason,
  /* Reason that the protocol was deactivated.
  */

  prot_state_e_type              prot_state
  /* Protocol which sent the autonomous deactivation.
  */

)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;

  MMOC_ASSERT( actd_reason == PROT_ACT_GWL_REDIR );


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                     = MMOC_RPT_PROT_AUTO_ACTD_IND;
  msg_ptr->param.prot_actd_ind.reason   = actd_reason;
  msg_ptr->param.prot_actd_ind.prot_state   = prot_state;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );


  return;

} /* mmoc_rpt_prot_auto_actd_ind() */


/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_rpt_ph_stat_chgd_cnf

DESCRIPTION
  This function should be used by the protocols to acknowledge the MMoC that
  the "phone status changed (due to either NAM change, Subscription
  availability change(GSM/WCDMA or CDMA/AMPS/HDR) or operating mode change
  request. This function will queue the phone status changed confirmation
  report to the MMoC task.

  Note! This should be called only after completing the necessary
  housekeeping actions based on the request.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                           mmoc_rpt_ph_stat_chgd_cnf
(

  prot_trans_type                trans_id,
  /**< Protocols set this same as the trans_id received in the phone
  ** status changed ack. trans_id cannot be MMOC_TRANS_AUTONOMOUS.
  */

  sd_mode_e_type                 prot,
  /**< Protocol responding to the Phone status changed command. */

  mmoc_ph_stat_e_type        req_status
  /**< status of the command - success or error code. */

)


{
  mmoc_rpt_msg_s_type                  *msg_ptr;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                   = MMOC_RPT_PH_STAT_CHGD_CNF;
  msg_ptr->param.ph_stat_cnf.trans_id = trans_id;
  msg_ptr->param.ph_stat_cnf.mc_status     = req_status;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch(prot)
  {
    case SD_MODE_CDMA:
      msg_ptr->param.ph_stat_cnf.prot = MMOC_MODE_CDMA_1;
      break;

    case SD_MODE_HDR:
      msg_ptr->param.ph_stat_cnf.prot = MMOC_MODE_HDR_1;
      break;

    default:
      msg_ptr->param.ph_stat_cnf.prot = MMOC_MODE_GWL_1;
      break;
  }

  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );


  return;

} /* mmoc_rpt_ph_stat_chgd_cnf() */


/*===========================================================================

FUNCTION mmoc_rpt_subs_cap_chgd_cnf

DESCRIPTION
  This function should be used by the protocols to acknowledge the MMoC that
  the "dual standby status changed.
  This function will queue the dual standby status changed confirmation
  report to the MMoC task.

  Note! This should be called only after completing the necessary
  housekeeping actions based on the request.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                           mmoc_rpt_subs_cap_chgd_cnf
(

  prot_trans_type                trans_id
  /* Protocols set this same as the trans_id received in the phone
  ** status changed ack. trans_id cannot be MMOC_TRANS_AUTONOMOUS.
  */
)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                   = MMOC_RPT_SUBS_CAP_CHGD_CNF;
  msg_ptr->param.ph_stat_cnf.trans_id = trans_id;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );


  return;

} /* mmoc_rpt_ds_stat_chgd_cnf() */

/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_rpt_ds_stat_chgd_cnf

DESCRIPTION
  This function should be used by the protocols to acknowledge the MMoC that
  the "dual standby status changed.
  This function will queue the dual standby status changed confirmation
  report to the MMoC task.

  Note! This should be called only after completing the necessary
  housekeeping actions based on the request.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                           mmoc_rpt_ds_stat_chgd_cnf
(

  prot_trans_type                trans_id
  /* Protocols set this same as the trans_id received in the phone
  ** status changed ack. trans_id cannot be MMOC_TRANS_AUTONOMOUS.
  */
)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                   = MMOC_RPT_DS_STAT_CHGD_CNF;
  msg_ptr->param.ph_stat_cnf.trans_id = trans_id;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );


  return;

} /* mmoc_rpt_ds_stat_chgd_cnf() */


/*===========================================================================

FUNCTION mmoc_rpt_activation_cnf

DESCRIPTION
  This function should be used by the protocol ( CMREGPRX) to inform MMOC that
  activation is complete.

===========================================================================*/
void                           mmoc_rpt_activation_cnf
(
  prot_state_e_type              prot_state,
  /* Protocol sending the report
  */

  mm_sub_stk_id_s_type           mm_id
  /* Stack on which activation is occuring
  */
)
{
#ifdef FEATURE_MMODE_LTE_RESEL
  mmoc_rpt_msg_s_type                  *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                       = MMOC_RPT_ACTIVATION_CNF;
  msg_ptr->param.activation_cnf.prot_state = prot_state;
  msg_ptr->param.activation_cnf.mm_id      = mm_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );
#else
  SYS_ARG_NOT_USED(prot_state);
#endif // FEATURE_MMODE_LTE_RESEL
  return;

} /* mmoc_rpt_activation_cnf() */

/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_rpt_ps_detach_cnf

DESCRIPTION
  This function should be used by the protocols to acknowledge the MMoC that
  the "PS detach is done".
  This function will queue the ps detach confirmation
  report to the MMoC task.

  Note! This should be called only after completing the necessary
  housekeeping actions based on the request.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                           mmoc_rpt_ps_detach_cnf
(

  prot_trans_type                trans_id
  /* Protocols set this same as the trans_id received in the phone
  ** status changed ack. trans_id cannot be MMOC_TRANS_AUTONOMOUS.
  */
)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                   = MMOC_RPT_PS_DETACH_CNF;
  msg_ptr->param.ps_detach_cnf.trans_id = trans_id;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );


  return;

} /* mmoc_rpt_ps_detach_cnf() */

/*===========================================================================

FUNCTION mmoc_rpt_ue_mode_switch_cnf

DESCRIPTION
  This function should be used by the protocols to acknowledge the MMoC that
  the "PS detach is done".
  This function will queue the ps detach confirmation
  report to the MMoC task.

  Note! This should be called only after completing the necessary
  housekeeping actions based on the request.

===========================================================================*/
void                           mmoc_rpt_ue_mode_switch_cnf
(
  mm_sub_stk_id_s_type           mm_id,

  prot_trans_type                trans_id,
  /* Protocols set this same as the trans_id received in the phone
  ** status changed ack. trans_id cannot be MMOC_TRANS_AUTONOMOUS.
  */
  boolean                        is_prot_deactivated
  /* whether prot was deactivated as part of this  ue_mode_switch */
)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                     = MMOC_RPT_UE_MODE_SWITCH_CNF;
  msg_ptr->param.ue_mode_switch_cnf.trans_id = trans_id;
  msg_ptr->param.ue_mode_switch_cnf.mm_id    = mm_id;
  msg_ptr->param.ue_mode_switch_cnf.is_prot_deactivated = is_prot_deactivated;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );


  return;

} /* mmoc_rpt_ps_detach_cnf() */

/*===========================================================================

FUNCTION mmoc_1X_rpt_ue_mode_switch_cnf

DESCRIPTION
  This function should be used by the protocols to acknowledge MMoC that,
  ue mode change processing has been completed.
  This function will queue the 1X ue mode change cnf
  report to the MMoC task.

  Note! This should be called only after completing the necessary
  housekeeping actions based on the request.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                           mmoc_1x_rpt_ue_mode_switch_cnf
(
  prot_trans_type                trans_id,
  /* Protocols set this same as the trans_id received in the phone
  ** status changed ack. trans_id cannot be MMOC_TRANS_AUTONOMOUS.
  */
  sys_modem_as_id_e_type         asub_id
)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                     = MMOC_RPT_UE_MODE_SWITCH_CNF_1X;
  msg_ptr->param.ue_mode_switch_cnf_1x.trans_id = trans_id;
  msg_ptr->param.ue_mode_switch_cnf_1x.mm_id.asubs_id = asub_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );


  return;

} /* mmoc_rpt_ps_detach_cnf() */


/*===========================================================================

FUNCTION mmoc_HDR_rpt_ue_mode_switch_cnf

DESCRIPTION
  This function should be used by the protocols to acknowledge MMoC that,
  ue mode change processing has been completed.
  This function will queue the HDR ue mode change cnf
  report to the MMoC task.

  Note! This should be called only after completing the necessary
  housekeeping actions based on the request.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                           mmoc_hdr_rpt_ue_mode_switch_cnf
(
  prot_trans_type                trans_id,
  /* Protocols set this same as the trans_id received in the phone
  ** status changed ack. trans_id cannot be MMOC_TRANS_AUTONOMOUS.
  */

  sys_modem_as_id_e_type         asub_id

)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                     = MMOC_RPT_UE_MODE_SWITCH_CNF_HDR;
  msg_ptr->param.ue_mode_switch_cnf_hdr.trans_id = trans_id;
  msg_ptr->param.ue_mode_switch_cnf_hdr.mm_id.asubs_id = asub_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );


  return;

} /* mmoc_hdr_rpt_ue_mode_switch_cnf() */

/* <EJECT> */
/*===========================================================================

FUNCTION mmoc_rpt_prot_gen_cmd_cnf

DESCRIPTION
  This function is used to acknowledge the generic protocol command.

  Note!  This is a local function and should be called by
  mmoc_proc_prot_gen_cmd.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                           mmoc_rpt_prot_gen_cmd_cnf
(

  boolean                        is_cmd_processed,
  /* If the protocols received the generic command in the incorrect
  ** state, then this will be set to FALSE.
  */

  prot_trans_type                trans_id,
  /* Protocols set this same as the trans_id received in the generic
  ** command. trans_id cannot be MMOC_TRANS_AUTONOMOUS.
  */

  boolean                        is_prot_deactivating
  /* Is the protocol deactiavting because of processing the generic
  ** command. Valid when when is_cmd_processed = TRUE.
  */

)
{
  mmoc_rpt_msg_s_type                  *msg_ptr;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                           = MMOC_RPT_PROT_GEN_CMD_CNF;
  msg_ptr->param.gen_cmd_cnf.is_cmd_processed = is_cmd_processed;
  msg_ptr->param.gen_cmd_cnf.trans_id         = trans_id;
  msg_ptr->param.gen_cmd_cnf.is_prot_deactivating = is_prot_deactivating;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );


  return;

} /*  mmoc_rpt_prot_gen_cmd_cnf() */


/* <EJECT> */
/*===========================================================================
=============================================================================
=============================================================================
============================== Protocols -> MMOC functional interface =======
=============================================================================
=============================================================================
===========================================================================*/

/* The following section defines the API when the clients ( example
** protocols ) have to communicate to MMoC using the functional interface.
*/



/*===========================================================================

FUNCTION mmoc_proc_prot_gen_cmd

DESCRIPTION
  This function should be used by protocols to process the Generic command
  received from MMoC.  The Generic command could be received because of
  Reselection timeout, Get networks request, Terminate get networks or
  preferred system selection change.

  Note! This function will also send the Generic command ack to MMoC task.

RETURN VALUE
  SS-Action that the SS-Client should take upon function call return as
  follows:

    SD_SS_ACT_CONTINUE  -  Continue with current mode of operation.
    SD_SS_ACT_ACQ_CDMA  -  Acquire a CDMA system.
    SD_SS_ACT_ACQ_HDR   -  Attempt acquiring an HDR system.
    SD_SS_ACT_ACQ_GWL   -  Attempt acquiring an HDR system.
    SD_SS_ACT_MEAS_DED  -  Request a dedicated mode measurement from SRCH.
    SD_SS_ACT_MEAS_BACK -  Request a background mode measurement from SRCH.
    SD_SS_ACT_PWR_SAVE  -  Enter power save mode.

===========================================================================*/
sd_ss_act_e_type               mmoc_proc_prot_gen_cmd
(

  boolean                        is_resel_allowed,
  /* Protocol indicated if reselection is allowed at this time or not.
  ** TRUE = reselection allowed. This flag is passed on to SD20 in the
  ** call to sd_ss_ind_misc_timer() or sd_ss_ind_user_irat_to_gwl().
  ** So, this parameter is used only if the gen_ss_cmd type is Reselection
  ** timer expired or IRAT reselection to LTE. For other types it is not used.
  */

  boolean                        is_prot_active,
  /* Flag which specifies if the protocol calling this function is
  ** ACTIVE or not.
  */

  const prot_gen_cmd_s_type           *gen_cmd_ptr
  /* Pointer to generic system selection command information.
  ** Same as what was received by the protocols.
  */

)
{
  sd_ss_act_e_type    ss_act = SD_SS_ACT_CONTINUE;
  boolean             is_cmd_processed = FALSE;
  boolean             is_prot_deactivating = FALSE;
  mmoc_state_info_s_type *mmoc_info_ptr = mmoc_get_state_info_ptr();
  mmoc_state_info_dev_prop_s_type  *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  mm_sub_stk_id_s_type mm_id;
  mm_id.asubs_id = gen_cmd_ptr->asubs_id;
  mm_id.stk_id   = gen_cmd_ptr->stk_id;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(mm_id.asubs_id < SYS_MODEM_AS_ID_1 || mm_id.asubs_id >= MAX_SIMS ||
      mm_id.stk_id < MM_STACK_0 || mm_id.stk_id >= MAX_STACKS)
  {
    MMOC_MSG_HIGH_2("invalid sub %d or stack %d", mm_id.asubs_id, mm_id.stk_id);
    return ss_act;
  }

  MMOC_MSG_HIGH_6("PROT->MMOC->SD: GEN_CMD: sub %d stk %d is_resel_allowed %d is_prot_active %d trans_id %d cmd_type %d ",
                  mm_id.asubs_id,
                  mm_id.stk_id,
                  is_resel_allowed,
                  is_prot_active,
                  gen_cmd_ptr->trans_id,
                  gen_cmd_ptr->cmd_type);

  /* Check if the protocol is active or pref need to be restored at SD. In
  ** case of restore, MMOC internally calls this API to update pref to SD
  */
  if(is_prot_active || is_pref_chng_restore_req(gen_cmd_ptr))
  {
    switch(mm_id.stk_id)
    {
      case MM_STACK_0:
      {
        /* Protocol is active,Check which SD indication needs to be called.
        */
        switch(gen_cmd_ptr->cmd_type)
        {
          case PROT_GEN_CMD_SS_TIMER_EXP:
            ss_act = sd_ss_ind_misc_timer( mm_id, is_resel_allowed, NULL );
            break;

          case PROT_GEN_CMD_PREF_SYS_CHGD:
            ss_act = sd_ss_ind_user_ss_pref9
                     ( gen_cmd_ptr->asubs_id,
                       gen_cmd_ptr->param.pref_sys_chgd.pref_reas,
                       gen_cmd_ptr->param.pref_sys_chgd.orig_mode,
                       gen_cmd_ptr->param.pref_sys_chgd.mode_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.band_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.lte_band_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.tds_band_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.prl_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.roam_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.hybr_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.otasp_band,
                       gen_cmd_ptr->param.pref_sys_chgd.otasp_blksys,
                       gen_cmd_ptr->param.pref_sys_chgd.avoid_type,
                       gen_cmd_ptr->param.pref_sys_chgd.avoid_time,
                       &gen_cmd_ptr->param.pref_sys_chgd.manual_sys_info,
                       gen_cmd_ptr->param.pref_sys_chgd.domain_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.acq_order_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.pref_update_reas,
                       gen_cmd_ptr->param.pref_sys_chgd.user_mode_pref,
                       &gen_cmd_ptr->param.pref_sys_chgd.rat_acq_order,
                       gen_cmd_ptr->param.pref_sys_chgd.sys_sel_pref_req_id,
                       gen_cmd_ptr->param.pref_sys_chgd.camp_mode_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.csg_id,
                       gen_cmd_ptr->param.pref_sys_chgd.csg_rat,
                       gen_cmd_ptr->param.pref_sys_chgd.voice_domain_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.fullrat_status,
                       gen_cmd_ptr->param.pref_sys_chgd.rat_capability,
                       &gen_cmd_ptr->param.pref_sys_chgd.addl_action,
                       gen_cmd_ptr->param.pref_sys_chgd.pending_emerg_state,
                       NULL
                     );

            /* Capture the error state - should not happen
            ** Error fatal when mode pref is not sub-set of max mode cap
            */
#ifndef MMOC_DEBUG
            if(mmoc_is_sxlte(gen_cmd_ptr->asubs_id) &&
                (mmoc_dev_info->active_subs & BM(gen_cmd_ptr->asubs_id)) &&
                mmoc_dev_info->oprt_mode == PROT_OPRT_MODE_ONLINE &&
                ((uint32)gen_cmd_ptr->param.pref_sys_chgd.mode_pref &
                 ~(mmoc_sub_info[gen_cmd_ptr->asubs_id]->subs_capability)) != 0)
            {
              MMOC_ERR_FATAL_3("maxcap 0x%x and mode 0x%x out of sync for sub %d",
                               mmoc_sub_info[gen_cmd_ptr->asubs_id]->subs_capability,
                               gen_cmd_ptr->param.pref_sys_chgd.mode_pref,
                               gen_cmd_ptr->asubs_id);
            }
#endif

            break;

          case PROT_GEN_CMD_GET_NETWORKS_GW:
#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
              ss_act = sd_ss_ind_user_get_net_extn
                     ( gen_cmd_ptr->param.gw_get_net.mode_pref,
                       gen_cmd_ptr->param.gw_get_net.band_pref,
                       gen_cmd_ptr->param.gw_get_net.lte_band_pref,
                       gen_cmd_ptr->param.gw_get_net.tds_band_pref,
                       mm_id,
                       gen_cmd_ptr->param.gw_get_net.network_list_type,
                       gen_cmd_ptr->param.gw_get_net.ignore_manual_bst,
                       gen_cmd_ptr->param.gw_get_net.is_requeue,
                         NULL,
                         gen_cmd_ptr->param.gw_get_net.pf_scan_info,
                         gen_cmd_ptr->param.gw_get_net.incremental_results,
                         gen_cmd_ptr->param.gw_get_net.max_search_time
                     );
#endif
            break;

          case PROT_GEN_CMD_TERM_GET_NETWORKS_GW:
#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
            ss_act = sd_ss_ind_user_term_get_net_gw_ss
                     ( mm_id,
                       NULL
                     );
#endif
            break;

          case PROT_GEN_CMD_IRAT_TO_GWL:
#if defined FEATURE_MMODE_LTE_RESEL && defined MMOC_GWL_SUPPORTED
            ss_act = sd_ss_ind_misc_irat_to_gwl(mm_id,
                                                is_resel_allowed,
                                                NULL);
#endif
            break;

          case PROT_GEN_CMD_GW_SIM_STATE:
#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
            ss_act = sd_ss_ind_misc_gw_sim_state_update(gen_cmd_ptr->param.gw_sim_state, mm_id, NULL );
#endif
            break;

          case PROT_GEN_CMD_WAKEUP_FROM_PWR_SAVE:
            if ( ( ss_act = sd_ss_ind_user_pwr_save_exit( mm_id.asubs_id, NULL ) ) == SD_SS_ACT_PWR_SAVE )
            {
              /* Put SD back in power save mode.
              */
              ss_act = sd_ss_ind_misc_pwr_save_enter( mm_id, NULL );
            }
            break;

          default:
            MMOC_ERR_0(" Unknown generic command ");
            ss_act = SD_SS_ACT_CONTINUE;
            break;
        } /* switch (gen_cmd_ptr->cmd_type ) */

        } /*  case SD_SS_MAIN:  */
      break;

#ifdef MMOC_HYBR_1_SUPPORTED
      case MM_STACK_1:
      {
        /* Protocol is active,Check which SD indication needs to be called.
        */
        switch(gen_cmd_ptr->cmd_type)
        {
          case PROT_GEN_CMD_SS_TIMER_EXP:
            ss_act = sd_ss_ind_misc_timer( mm_id, is_resel_allowed, NULL );
            break;

          case PROT_GEN_CMD_PREF_SYS_CHGD:
            ss_act = sd_ss_ind_hybr_1_user_ss_pref4
                     ( gen_cmd_ptr->asubs_id,
                       gen_cmd_ptr->param.pref_sys_chgd.pref_reas,
                       gen_cmd_ptr->param.pref_sys_chgd.orig_mode,
                       gen_cmd_ptr->param.pref_sys_chgd.mode_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.band_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.prl_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.roam_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.hybr_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.otasp_band,
                       gen_cmd_ptr->param.pref_sys_chgd.otasp_blksys,
                       &gen_cmd_ptr->param.pref_sys_chgd.manual_sys_info,
                       gen_cmd_ptr->param.pref_sys_chgd.domain_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.acq_order_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.pref_update_reas,
                       gen_cmd_ptr->param.pref_sys_chgd.rat_capability,
                       NULL
                     );
            break;
          case PROT_GEN_CMD_GET_NETWORKS_GW:
#ifdef MMOC_HYBR_GW_SUPPORTED
            ss_act = sd_ss_ind_user_get_net_extn
                     ( gen_cmd_ptr->param.gw_get_net.mode_pref,
                       gen_cmd_ptr->param.gw_get_net.band_pref,
                       gen_cmd_ptr->param.gw_get_net.lte_band_pref,
                       gen_cmd_ptr->param.gw_get_net.tds_band_pref,
                       mm_id,
                       gen_cmd_ptr->param.gw_get_net.network_list_type,
                       gen_cmd_ptr->param.gw_get_net.ignore_manual_bst,
                       gen_cmd_ptr->param.gw_get_net.is_requeue,
                       NULL,
                       gen_cmd_ptr->param.gw_get_net.pf_scan_info,
                       gen_cmd_ptr->param.gw_get_net.incremental_results,
                       gen_cmd_ptr->param.gw_get_net.max_search_time
                       
                     );
#endif
            break;

          case PROT_GEN_CMD_TERM_GET_NETWORKS_GW:
#ifdef MMOC_HYBR_GW_SUPPORTED
            ss_act = sd_ss_ind_user_term_get_net_gw_ss
                     ( mm_id,
                       NULL
                     );
#endif
            break;

          case PROT_GEN_CMD_WAKEUP_FROM_PWR_SAVE:
            if ( ( ss_act = sd_ss_ind_hybr_1_user_pwr_save_exit( mm_id.asubs_id, NULL ) ) == SD_SS_ACT_PWR_SAVE )
            {
              /* Put SD back in power save mode.
              */
              ss_act = sd_ss_ind_misc_pwr_save_enter( mm_id, NULL );
            }
            break;

          default:
            MMOC_ERR_0("Unknown generic command");
            ss_act = SD_SS_ACT_CONTINUE;
            break;
        } /* switch (gen_cmd_ptr->cmd_type ) */

        } /* case SD_SS_HYBR_HDR: */
      break;
#endif


      case MM_STACK_2:
      {
        /* Protocol is active,Check which SD indication needs to be called.
        */
        switch(gen_cmd_ptr->cmd_type)
        {
          case PROT_GEN_CMD_SS_TIMER_EXP:
            ss_act = sd_ss_ind_misc_timer( mm_id, is_resel_allowed, NULL );
            break;

          case PROT_GEN_CMD_PREF_SYS_CHGD:
            ss_act = sd_ss_ind_hybr_2_user_ss_pref6
                     (
                       gen_cmd_ptr->param.pref_sys_chgd.pref_reas,
                       gen_cmd_ptr->param.pref_sys_chgd.orig_mode,
                       gen_cmd_ptr->param.pref_sys_chgd.mode_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.band_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.lte_band_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.tds_band_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.prl_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.roam_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.hybr_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.otasp_band,
                       gen_cmd_ptr->param.pref_sys_chgd.otasp_blksys,
                       gen_cmd_ptr->param.pref_sys_chgd.avoid_type,
                       gen_cmd_ptr->param.pref_sys_chgd.avoid_time,
                       &gen_cmd_ptr->param.pref_sys_chgd.manual_sys_info,
                       gen_cmd_ptr->param.pref_sys_chgd.domain_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.acq_order_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.pref_update_reas,
                       &gen_cmd_ptr->param.pref_sys_chgd.rat_acq_order,
                       gen_cmd_ptr->param.pref_sys_chgd.user_mode_pref,
                       gen_cmd_ptr->param.pref_sys_chgd.sys_sel_pref_req_id,
                       gen_cmd_ptr->param.pref_sys_chgd.csg_id,
                       gen_cmd_ptr->param.pref_sys_chgd.csg_rat,
                       gen_cmd_ptr->param.pref_sys_chgd.voice_domain_pref,
                       &gen_cmd_ptr->param.pref_sys_chgd.addl_action,
                       mm_id,
                       NULL
                     );
            break;

          case PROT_GEN_CMD_GET_NETWORKS_GW:
#ifdef MMOC_GW_SUPPORTED
          ss_act = sd_ss_ind_user_get_net_extn
                   ( gen_cmd_ptr->param.gw_get_net.mode_pref,
                       gen_cmd_ptr->param.gw_get_net.band_pref,
                       gen_cmd_ptr->param.gw_get_net.lte_band_pref,
                       gen_cmd_ptr->param.gw_get_net.tds_band_pref,
                       mm_id,
                       gen_cmd_ptr->param.gw_get_net.network_list_type,
                       gen_cmd_ptr->param.gw_get_net.ignore_manual_bst,
                       gen_cmd_ptr->param.gw_get_net.is_requeue,
                     NULL,
                     gen_cmd_ptr->param.gw_get_net.pf_scan_info,
                     gen_cmd_ptr->param.gw_get_net.incremental_results,
                     gen_cmd_ptr->param.gw_get_net.max_search_time
                     );
#endif
            break;

          case PROT_GEN_CMD_TERM_GET_NETWORKS_GW:
#ifdef MMOC_GW_SUPPORTED
            ss_act = sd_ss_ind_user_term_get_net_gw_ss
                     ( mm_id,
                       NULL
                     );
#endif
            break;

          case PROT_GEN_CMD_IRAT_TO_GWL:
#if defined FEATURE_MMODE_LTE_RESEL && defined MMOC_GWL_SUPPORTED
            ss_act = sd_ss_ind_misc_irat_to_gwl(mm_id,
                                                is_resel_allowed,
                                                NULL);
#endif
            break;

          /* For the case service transfer, we want to indicate to SD
          ** that NAS has deactivate the HYBR-2 stack.
          */
          case PROT_GEN_CMD_LOCAL_DEACT:
            /* This is an indication that service transfer happens on HYBR-2 to Main.
            ** Hence, ss_act return should be POWER_SAVE for HYBR-2. */
            ss_act = sd_ss_ind_misc_stack_local_deactivate(mm_id, NULL);
            break;

          /* locally activate the spefied ss.  sd will always return action continue so that
               ** No service request will be triggerred.
               */
          case PROT_GEN_CMD_LOCAL_ACT:
            ss_act = sd_ss_ind_misc_stack_local_activate(mm_id, NULL);
            break;

          case PROT_GEN_CMD_GW_SIM_STATE:
#if defined(MMOC_GW_SUPPORTED) || defined(FEATURE_MMOC_LTE)
            ss_act = sd_ss_ind_misc_gw_sim_state_update(gen_cmd_ptr->param.gw_sim_state, mm_id, NULL );
#endif
            break;

          case PROT_GEN_CMD_WAKEUP_FROM_PWR_SAVE:
            if ( ( ss_act = sd_ss_ind_hybr_2_user_pwr_save_exit( mm_id.asubs_id, NULL ) ) == SD_SS_ACT_PWR_SAVE )
            {
              /* Put SD back in power save mode.
              */
              ss_act = sd_ss_ind_misc_pwr_save_enter( mm_id, NULL );
            }
            break;

          default:
            MMOC_ERR_0(" Unknown generic command ");
            ss_act = SD_SS_ACT_CONTINUE;
            break;
        } /* switch (gen_cmd_ptr->cmd_type ) */

        } /*  case SD_SS_HYBR_2:  */
      break;


      default:
      {
        MMOC_ERR_1("Incorrect stk %d", gen_cmd_ptr->stk_id);
      }
    } /* switch ( gen_cmd_ptr->ss ) */

    /* Store command was processed.
    */
    is_cmd_processed = TRUE;

    if ((gen_cmd_ptr->stk_id >= MM_STACK_0) && (gen_cmd_ptr->stk_id < MM_STACK_ALL))
    {
      is_prot_deactivating = mmoc_is_prot_deactivating(
                               mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state,
                               ss_act);
    }
    else
    {
      MMOC_MSG_HIGH_1("Invalid stk %d recvd", gen_cmd_ptr->stk_id);
      is_prot_deactivating = FALSE;
    }
  } /* if ( is_prot_active ) */
  else
  {
    /* If this happens, MMOC thinks the protocol on gen_cmd_ptr->ss is active while
    ** the protocol is actually not. So MMOC keeps fwding the generic cmd to an inactive protocol
    */
    if ((gen_cmd_ptr->stk_id >= MM_STACK_0) && (gen_cmd_ptr->stk_id < MM_STACK_ALL))
    {
      MMOC_MSG_HIGH_3("GEN_CMD: MMOC and sub %d stk %d not in sync, mmoc prot_state=%d",
                      mm_id.asubs_id,
                      mm_id.stk_id,
                      mmoc_sub_info[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state
                     );
    }

#ifdef MMOC_HYBR_1_SUPPORTED
    /* If the mismatch is on Hybrid stack. */
    if( gen_cmd_ptr->stk_id == MM_STACK_1 )
    {
      /* Get the last SD action on the Hybrid stack. */
      ss_act = sd_ss_hdr_act_get( mm_id.asubs_id, NULL );

      /* If the action is ACQ_HDR and main stack is not currently in GWL
      ** pass it on to HDRCP so it will recover.
      */
      if( ss_act == SD_SS_ACT_ACQ_HDR &&
          mmoc_sub_info[mm_id.asubs_id]->stack_prop[MM_STACK_0]->prot_state != PROT_STATE_ONLINE_GWL )
      {
        MMOC_MSG_HIGH_1("Resending the action %d to HDRCP", ss_act);
      }
      else
      {
        MMOC_MSG_HIGH_2("SD last action = %d, Main stack state = %d",
                        ss_act,
                        mmoc_sub_info[mm_id.asubs_id]->stack_prop[MM_STACK_0]->prot_state);

        /* Reset to default.
        ** So that we do not disturb any existing functionality
        */
        ss_act = SD_SS_ACT_CONTINUE;
      }
    }
#endif /* MMOC_HYBR_1_SUPPORTED */
  }/* if ( is_prot_active ) */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the Generic command ack to MMoC if trans_id != TRANS_NONE.
  */
  if ( gen_cmd_ptr->trans_id != PROT_TRANS_NONE )
  {
    mmoc_rpt_prot_gen_cmd_cnf( is_cmd_processed,
                               gen_cmd_ptr->trans_id,
                               is_prot_deactivating
                             );
  } /*  if ( gen_cmd_ptr->trans_id != PROT_TRANS_NONE ) */


  /*if(ss_act != SD_SS_ACT_CONTINUE)
  {
    MMOC_MSG_HIGH_1("GEN_CMD: ss_act %d", ss_act);
  }*/

  return ss_act;

} /* mmoc_proc_prot_gen_cmd() */

/*===========================================================================
=============================================================================
=============================================================================
============================== MMGSDI -> MMOC functional interface ==========
=============================================================================
=============================================================================
===========================================================================*/

/* The following section defines the API when MMOC-MMGSDI has to communicate
** to MMoC using the functional interface.
*/


/*===========================================================================

FUNCTION mmoc_cmd_mmgsdi_info_ind

DESCRIPTION
   This function will be used by MMGSDI to inform MMoC about the
   MMGSDI_CARD_INSERTED_EVT. This command will be queued by MMOC when
   MMGSDI invokes the call back function.

===========================================================================*/
void                           mmoc_cmd_mmgsdi_info_ind
(

)
{
  mmoc_cmd_msg_s_type                  *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the command buffer.
  */
  msg_ptr = mmoc_get_cmd_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->cmd.name                          = MMOC_CMD_MMGSDI_INFO_IND;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the command.
  */
  mmoc_send_cmd( msg_ptr );

} /* mmoc_cmd_mmgsdi_info_ind() */



/*===========================================================================

FUNCTION mmoc_is_pref_reas_considered_non_dup_pref_sys_cmd

DESCRIPTION
  This function will look for the sd_ss_pref_reas_e_type pref_reas, which shall
  be ignored for Duplicate Pref Sys Cmds removal from the MMOC queue.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If Cmds has to be ignored for deletion.
  FASLE: Pref reason for Cmd is entitled for deletion.

SIDE EFFECTS
  None
===========================================================================*/

boolean mmoc_is_pref_reas_considered_non_dup_pref_sys_cmd( sd_ss_pref_reas_e_type pref_reas)
{
  boolean ret_val = FALSE;
  MMOC_ASSERT_ENUM_IS_INRANGE(pref_reas, SD_SS_PREF_REAS_MAX);

  switch(pref_reas)
  {
    case SD_SS_PREF_REAS_HDR_CDMA_HO:
    case SD_SS_PREF_REAS_AVOID_SYS:
    case SD_SS_PREF_REAS_ORIG_START_CS:
    case SD_SS_PREF_REAS_ORIG_START_PS:
    case SD_SS_PREF_REAS_USER_RESEL:
      ret_val = TRUE;
      break;

    default:
      ret_val = FALSE;
      break;
  }

  return ret_val;
}


/*===========================================================================

FUNCTION mmoc_remove_dup_pref_sys_cmds

DESCRIPTION
  This function will detect duplicate preferred system changed commands from
  CM and remove them from the queue. It will retain the last preferred system
  changed command.

===========================================================================*/
LOCALF void                            mmoc_remove_dup_pref_sys_cmds
(

  mm_sub_stk_id_s_type                   mm_id,
  /* Remove duplicate pref sys commands intended for ss.
  */

  const mmoc_state_info_s_type          *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  /* Pointer to MMoC's task information.
  */
  mmoc_task_info_s_type          *mmoc_task_ptr = mmoc_get_task_info_ptr();
  mmoc_cmd_msg_s_type            *next_cmd_ptr =
    (mmoc_cmd_msg_s_type *)q_check( &mmoc_task_ptr->cmd_q );
  mmoc_cmd_msg_s_type            *pref_sys_cmd_ptr = NULL;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Search for first pref_sys_chgd command in the queue.
  */
  while(next_cmd_ptr != NULL &&
        (next_cmd_ptr->cmd.name != MMOC_CMD_PROT_GEN_CMD ||
         next_cmd_ptr->param.gen_prot_cmd.cmd_type != PROT_GEN_CMD_PREF_SYS_CHGD ||
         next_cmd_ptr->param.gen_prot_cmd.asubs_id != mm_id.asubs_id ||
         next_cmd_ptr->param.gen_prot_cmd.stk_id != mm_id.stk_id))
  {
    next_cmd_ptr = (mmoc_cmd_msg_s_type *)q_next(
                     &mmoc_task_ptr->cmd_q, &next_cmd_ptr->cmd.hdr.link);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*  Check if pref_sys_chgd cmd was found in the queue.
  */
  if ( next_cmd_ptr == NULL )
  {
    /* None found.
    */
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize for duplicate pref_sys_chgd commands detection.
  */
  pref_sys_cmd_ptr = next_cmd_ptr;
  next_cmd_ptr = (mmoc_cmd_msg_s_type *)
                 q_next(&mmoc_task_ptr->cmd_q, &next_cmd_ptr->cmd.hdr.link);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Search for duplicate pref_sys_chgd command in the queue.
  */
  for ( ;
        next_cmd_ptr != NULL;
        next_cmd_ptr = (mmoc_cmd_msg_s_type *)
                       q_next(&mmoc_task_ptr->cmd_q, &next_cmd_ptr->cmd.hdr.link)
      )
  {

    if(next_cmd_ptr->cmd.name == pref_sys_cmd_ptr->cmd.name
        &&
        next_cmd_ptr->param.gen_prot_cmd.cmd_type ==
        pref_sys_cmd_ptr->param.gen_prot_cmd.cmd_type
        &&
        next_cmd_ptr->param.gen_prot_cmd.asubs_id ==
        pref_sys_cmd_ptr->param.gen_prot_cmd.asubs_id
        &&
        next_cmd_ptr->param.gen_prot_cmd.stk_id ==
        pref_sys_cmd_ptr->param.gen_prot_cmd.stk_id
        &&
        next_cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.hybr_pref ==
        pref_sys_cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.hybr_pref
        &&
        !mmoc_is_pref_reas_considered_non_dup_pref_sys_cmd(
          next_cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.pref_reas )
        &&
        !mmoc_is_pref_reas_considered_non_dup_pref_sys_cmd(
          pref_sys_cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.pref_reas )
        &&
        pref_sys_cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.addl_action.action
        == next_cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.addl_action.action
        &&
        /* Do not remove USER PREF cmds */
        !(pref_sys_cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.pref_reas == SD_SS_PREF_REAS_USER ||
          pref_sys_cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.pref_reas == SD_SS_PREF_REAS_USER_RESEL)
      )
    {
      MMOC_MSG_HIGH_0("Duplicate pref_sys_chgd");

      q_delete( &mmoc_task_ptr->cmd_q, &pref_sys_cmd_ptr->cmd.hdr.link );

      mmoc_mem_free( pref_sys_cmd_ptr );

      pref_sys_cmd_ptr = next_cmd_ptr;

    }

  }

} /* mmoc_remove_dup_pref_sys_cmds() */

/*===========================================================================

FUNCTION mmoc_remove_dup_redir_req_cmds

DESCRIPTION
  This function will detect duplicate redirection requests from
  same protocol remove them from the queue. It will retain the last command.

DEPENDENCIES
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
/*lint -esym(715,mmoc_info_ptr)*/
LOCALF void                            mmoc_remove_dup_redir_req_cmds
(

       sys_modem_as_id_e_type                    asubs_id,
          /* Remove duplicate pref sys commands intended for asubs_id.
          */

       const mmoc_state_info_s_type          *mmoc_info_ptr
          /* Pointer to MMoC's state information.
          */
)
{
  /* Pointer to MMoC's task information.
  */
  mmoc_task_info_s_type          *mmoc_task_ptr = mmoc_get_task_info_ptr();
  mmoc_cmd_msg_s_type            *next_cmd_ptr =
                   (mmoc_cmd_msg_s_type *)q_check( &mmoc_task_ptr->cmd_q );
  mmoc_cmd_msg_s_type            *redir_cmd_ptr = NULL;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Search for first redirection command in the queue.
  */
  while ( next_cmd_ptr != NULL &&
          ( next_cmd_ptr->cmd.name != MMOC_CMD_PROT_REDIR_IND ||
            next_cmd_ptr->param.prot_redir_ind.actd_reason != PROT_ACT_GWL_REDIR ||
            next_cmd_ptr->param.prot_redir_ind.prot_state != PROT_STATE_ONLINE_HDR ||
            next_cmd_ptr->param.prot_redir_ind.asubs_id != asubs_id
          )
        )
  {
    next_cmd_ptr = (mmoc_cmd_msg_s_type *)q_next(
                        &mmoc_task_ptr->cmd_q, &next_cmd_ptr->cmd.hdr.link);
  }


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*  Check if redirection cmd was found in the queue.
  */
  if ( next_cmd_ptr == NULL )
  {
    /* None found.
    */
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize for duplicate redirection commands detection.
  */
  redir_cmd_ptr = next_cmd_ptr;
  next_cmd_ptr = (mmoc_cmd_msg_s_type *)
                 q_next(&mmoc_task_ptr->cmd_q, &next_cmd_ptr->cmd.hdr.link);


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Search for duplicate redirection command in the queue.
  */
  for ( ;
        next_cmd_ptr != NULL;
        next_cmd_ptr = (mmoc_cmd_msg_s_type *)
                 q_next(&mmoc_task_ptr->cmd_q, &next_cmd_ptr->cmd.hdr.link)
      )
  {

     if ( next_cmd_ptr->cmd.name == redir_cmd_ptr->cmd.name
          &&
          next_cmd_ptr->param.prot_redir_ind.asubs_id ==
                            redir_cmd_ptr->param.prot_redir_ind.asubs_id
          &&
          next_cmd_ptr->param.prot_redir_ind.prot_state ==
                            redir_cmd_ptr->param.prot_redir_ind.prot_state
        )
      {
        MMOC_MSG_HIGH_0(" Duplicate redir_ind");

        q_delete( &mmoc_task_ptr->cmd_q, &redir_cmd_ptr->cmd.hdr.link );

        mmoc_mem_free( redir_cmd_ptr );

        redir_cmd_ptr = next_cmd_ptr;

     }

  }

} /* mmoc_remove_dup_redir_req_cmds() */


#ifdef FEATURE_MMOC_UNIT_TEST
#error code not present
#endif



/*===========================================================================

FUNCTION mmoc_process_event

DESCRIPTION
  This function will process the received event by calling the transaction
  handlers.

===========================================================================*/
void                            mmoc_process_event
(

  mmoc_state_info_s_type          *mmoc_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  mmoc_cmd_msg_s_type  *cmd_ptr = NULL;
  mmoc_rpt_msg_s_type  *rpt_ptr = NULL;

#ifdef MMODE_ADDITIONAL_DEBUG_INFO
  mmoc_dbg_buf_rpt_s_type    rpt_to_queue;
  int                    i,j;

#endif

  mm_sub_stk_id_s_type mm_id;

  /* Pointer to MMoC's task information.
  */
  mmoc_task_info_s_type          *mmoc_task_ptr = mmoc_get_task_info_ptr();


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Process the events.
  */
  while ( ( mmoc_accept_cmds(mmoc_info_ptr ) &&
            ((cmd_ptr = q_get(&mmoc_task_ptr->cmd_q)) != NULL)) ||
          ((rpt_ptr = q_get(&mmoc_task_ptr->rpt_q)) != NULL)
        )
  {
    if ( cmd_ptr != NULL )
    {
      if(cmd_ptr->cmd.name < MMOC_CMD_MAX)
      {
#ifndef FEATURE_MMOC_LOW_MEM_TARGET
        /* Invoke the transaction handler normally and dequeue the command.
        */
        mmocdbg_print_command_received ( cmd_ptr );
#endif /* FEATURE_MMOC_LOW_MEM_TARGET */
      }
      else
      {
        MMOC_ERR_1 ("invalid command: %d", cmd_ptr->cmd.name);
      }

      mmoc_call_trans_hndlr(cmd_ptr, NULL, mmoc_info_ptr );

      mmoc_mem_free( cmd_ptr );

      cmd_ptr = NULL;
    }
    else if ( rpt_ptr != NULL )
    {

      if(rpt_ptr->rpt.name < MMOC_RPT_MAX)
      {
#ifndef FEATURE_MMOC_LOW_MEM_TARGET
        mmocdbg_print_report_received ( rpt_ptr );
#endif /* FEATURE_MMOC_LOW_MEM_TARGET */
      }
      else
      {
        MMOC_ERR_1 ("invalid report: %d", rpt_ptr->rpt.name);
      }
#ifdef MMODE_ADDITIONAL_DEBUG_INFO
      for ( i = 0; i < (int)mmoc_info_ptr->nSubs && i < MAX_SIMS; i++ )
      {
        for ( j = 0; j < (int)mmoc_info_ptr->sub_prop[i]->nStacks && j < MAX_STACKS; j++ )
        {
          rpt_to_queue.prot_state[i][j] = mmoc_info_ptr->sub_prop[i]->stack_prop[j]->prot_state;
        }
      }
      /* Add the report to current transaction
      */

      if(mmoc_info_ptr->dev_prop.trans != MMOC_TRANS_NULL)
      {
        rpt_to_queue.rpt_name = rpt_ptr->rpt.name;
        rpt_to_queue.task_name = rpt_ptr->rpt.task_name;

        mmoc_dbg_buf_add_message(mmoc_info_ptr, &(rpt_to_queue), curr_rpt_idx);

        curr_rpt_idx++;
        /* if we get PROT_AUTO_DEACT_IND report as part of MMOC_TRANS_PROT_AUTO_DEACTD_IND,
        ** we should add report here as part of ongoing trans and not start new trans.
        ** Hence, we set rpt_added flag to TRUE here so it would not enter next if cond.
        ** and start new trans as a result of PROT_AUTO_DEACT_IND report.
        */

      }
#endif /* MMODE_ADDITIONAL_DEBUG_INFO */

      mmoc_call_trans_hndlr(NULL, rpt_ptr, mmoc_info_ptr );
      /* Free memory for a report only if is_resued is FALSE, its not
      ** buffered for resue
      */
      MMOC_MSG_LOW_1("rpt_ptr->rpt.is_reused:%d",rpt_ptr->rpt.is_reused);
      if(!rpt_ptr->rpt.is_reused)
      {
        mmoc_mem_free( rpt_ptr );
        rpt_ptr = NULL;
      }
    }
    /*At end of each transaction, scan buffered_auto_deact[] array. Process
    ** buffered auto deact reports for each stack one by one
    */
    mm_id = mmoc_is_auto_deact_ind_rpt_buf(mmoc_info_ptr);

    if(mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_NULL &&
        mm_id.asubs_id != SYS_MODEM_AS_ID_NO_CHANGE &&
        mm_id.asubs_id >= SYS_MODEM_AS_ID_1 && 
        mm_id.asubs_id < MAX_SIMS &&
        mm_id.stk_id != MM_STACK_ALL &&
        mm_id.stk_id >= MM_STACK_0 &&
        mm_id.stk_id < MAX_STACKS
    )
    {
      MMOC_MSG_HIGH_2("Process buffered auto_deact on sub %d stk %d",
                      mm_id.asubs_id,
                      mm_id.stk_id);

      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->buffered_auto_deact->rpt.is_reused = FALSE;
      rpt_ptr = mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->buffered_auto_deact;

      mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->buffered_auto_deact = NULL;

      if(rpt_ptr->rpt.name < MMOC_RPT_MAX)
      {
#ifndef FEATURE_MMOC_LOW_MEM_TARGET
        mmocdbg_print_report_received ( rpt_ptr );
#endif /* FEATURE_MMOC_LOW_MEM_TARGET */
      }
      else
      {
        MMOC_ERR_1 ("invalid report: %d", rpt_ptr->rpt.name);
      }

      /* If stack having buffered auto deact indication already has prot state as NULL,
      ** do not process the auto_deact report, as prot must have already been deactivated
      */
      if(mmoc_info_ptr->sub_prop[mm_id.asubs_id]->stack_prop[mm_id.stk_id]->prot_state != PROT_STATE_NULL )
      {
        mmoc_call_trans_hndlr(NULL, rpt_ptr, mmoc_info_ptr );
      }
      else
      {
        MMOC_MSG_HIGH_2("Do not process as prot_state for sub %d stk %d is NULL",
                        mm_id.asubs_id,
                        mm_id.stk_id);
      }

      MMOC_MSG_LOW_1("rpt_ptr->rpt.is_reused:%d",rpt_ptr->rpt.is_reused);
      if(!rpt_ptr->rpt.is_reused)
      {
        mmoc_mem_free( rpt_ptr );
        rpt_ptr = NULL;
      }

    }


    /* After mmoc processes each event, report back to the watchdog.  This is to avoid
    ** dog timeout when mmoc doing back to back event processing without exiting the while
    ** loop.
    */
    mmoc_kick_dog();

  } /* while (  mmoc_accept_cmds() ... ) */

  return;

} /* mmoc_process_event() */

/*****************************************************************************
******************************************************************************

  MCFG refresh related

******************************************************************************
*****************************************************************************/
#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*===========================================================================

FUNCTION mmoc_mcfg_refresh_cb

DESCRIPTION
  This is a callback function used by MCFG for NV/EFS refresh purpose
  This function puts the received refresh info into MMOC command queue

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
boolean mmoc_mcfg_refresh_cb(mcfg_refresh_info_s_type *p_info)
{
  mmoc_cmd_msg_s_type *mmoc_cmd_ptr;
  MMOC_MSG_HIGH_4("MCFG->MMOC: MCFG_REFRESH:     cb type %d slot %d sub %d ref_id %d",
                  p_info->type,p_info->slot_index,p_info->sub_index,p_info->reference_id );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(p_info->type != MCFG_REFRESH_TYPE_SUBS )
  {
    mcfg_refresh_done(p_info->reference_id);
    return FALSE;
  }

  if ((mmoc_cmd_ptr = mmoc_get_cmd_buf_else_err_fatal())!=NULL)
  {
    mmoc_cmd_ptr->cmd.name = MMOC_CMD_MCFG_REFRESH;
    mmoc_cmd_ptr->param.mcfg_refresh.as_id = (sys_modem_as_id_e_type)p_info->sub_index;
    mmoc_cmd_ptr->param.mcfg_refresh.reference_id = p_info->reference_id;
    mmoc_send_cmd( mmoc_cmd_ptr );

  }

  mcfg_refresh_done(p_info->reference_id);
  return TRUE;

}

/*===========================================================================
FUNCTION MMOC_MCFG_REFRESH_REGISTER

DESCRIPTION
This function call registers MMOC Task with MCFG for NV/EFS refresh purpose

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void mmoc_mcfg_refresh_register(void)
{
  mcfg_refresh_registration_s_type mcfg_reg;

  memset((void *)&mcfg_reg,0x00,sizeof(mcfg_refresh_registration_s_type));

  mcfg_reg.order = MCFG_REFRESH_ORDER_50;
  mcfg_reg.slot_mask = MCFG_REFRESH_INDEX_MASK_NONE;
  mcfg_reg.sub_mask = MCFG_REFRESH_INDEX_MASK_ALL;
  mcfg_reg.group = 0;
  mcfg_reg.cb = mmoc_mcfg_refresh_cb;

  if(mcfg_refresh_register(&mcfg_reg) != TRUE)
  {
    MMOC_ERR_0("MCFG Registration failed");
  }
}

#endif //FEATURE_MODEM_CONFIG_REFRESH


/*===========================================================================

FUNCTION mmoc_task

DESCRIPTION
  This function is the entrance for the MMoC task. This is an infinite loop
  outside of the state machine which handles the input signals.

===========================================================================*/
void                           mmoc_task
(
  dword                          ignored
  /* Parameter received from Rex - ignored.
  */

)
{
  int sub = 0;
  int stk = 0;
  rex_sigs_type mask = MMOC_CMD_Q_SIG       |
                       MMOC_RPT_Q_SIG       |
                       MMOC_DUP_CMD_REM_SIG |
#ifdef TEST_FRAMEWORK
                       #error code not present
#endif
                       MMOC_SANITY_TIMER_SIG;
  rex_sigs_type  sigs;

  /* Pointer to MMoC's task information.
  */
  mmoc_task_info_s_type          *mmoc_task_ptr = mmoc_get_task_info_ptr();

#ifdef TEST_FRAMEWORK
  #error code not present
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Perform MMoC task initialization.
  */
  mmoc_task_init();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the first watchdog report and set the timer for report interval
  ** and start the MMoC sanity timer.
  */
  mmoc_kick_dog();

#ifdef TEST_FRAMEWORK
  #error code not present
#endif

  rcinit_handshake_startup();

  /* MMOC initialiation after task is started */
  mmoc_init_after_task_start(&mmoc_state_info);

  /* Initialize the DB state to Powerup.
  */
  mmoc_update_db_dmss_state( &mmoc_state_info );


  /* Run time synch with other tasks is done here if needed, following APIs
  ** used: rcinit_lookup(),  rcevt_wait_name()
  */

  mcs_rcevt_signal_name(MMOC_RCEVT_READY);

  /* Enter the infinite loop and wait for events and process the received
  ** events.
  */
  for( ; ; )
  {
    sigs = mmoc_wait( mask );

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Check if dup. command removal sig is set.
    */
    if (sigs & MMOC_DUP_CMD_REM_SIG)
    {
      /* Clear the rex sigs associated with sanity timer.
      */
      (void)rex_clr_sigs( mmoc_tcb_ptr, MMOC_DUP_CMD_REM_SIG );

      /* Remove duplicate pref_sys commands for main stacks.
      */
      for(sub = 0; sub < mmoc_state_info.nSubs; sub++)
      {
        mm_sub_stk_id_s_type mm_id;
        mm_id.asubs_id = sub;
        mm_id.stk_id   = MM_STACK_0;

        mmoc_remove_dup_pref_sys_cmds( mm_id, &mmoc_state_info );
        mmoc_remove_dup_redir_req_cmds( mm_id.asubs_id, &mmoc_state_info );
      }

    } /* if ( sigs & MMOC_DUP_CMD_REM_SIG ) */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Check if it was an event.
    */
    if ( sigs & MMOC_CMD_Q_SIG || sigs & MMOC_RPT_Q_SIG )
    {
      /* Clear the rex sigs associated with cmd & rpt.
      */
      (void)rex_clr_sigs( mmoc_tcb_ptr, MMOC_CMD_Q_SIG );
      (void)rex_clr_sigs( mmoc_tcb_ptr, MMOC_RPT_Q_SIG );

      /* Process the received event( command or report ).
      */
      mmoc_process_event( &mmoc_state_info );

      /* Check if MMoC can accept commands at this time.
      */
      if ( mmoc_accept_cmds( &mmoc_state_info ) )
      {
        /* Enable REX signal mask for CMD q.
        */
        mask |= MMOC_CMD_Q_SIG;
      }
      else
      {
        /* Disable REX signal mask for CMD Q.
        */
        mask &= ~MMOC_CMD_Q_SIG;
      }
    } /* if ( sigs & MMOC_CMD_Q_SIG || sigs & MMOC_RPT_Q_SIG) */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Check if Sanity timer expired.
    */
    if (sigs & MMOC_SANITY_TIMER_SIG)
    {
      /* Clear the rex sigs associated with sanity timer.
      */
      (void)rex_clr_sigs( mmoc_tcb_ptr, MMOC_SANITY_TIMER_SIG );

      /* Sanity timer expired.
      */
      mmoc_sanity_timer_exp( &mmoc_state_info );

    } /* if ( sigs & MMOC_SANITY_TIMER_SIG ) */

    /* Check if MMGSDI request timer expired.
    ** If it is , then we have to call the callback with error status MMGSDI_SIM_BUSY
    */
    if ((sigs & MMOC_MMGSDI_ABORT_TIMER_SIG)
      && mmoc_get_mmgsdi_info_ptr ()->sim_busy_status[mmoc_state_info.dev_prop.sub_with_3gpp2] == TRUE)
    {
      mmgsdi_cnf_type *cnf = mmoc_mem_alloc(sizeof(mmgsdi_cnf_type));

      if(cnf == NULL)
      {
        sys_err_fatal_null_ptr_exception();
        return;
      }
      else
      {
        /* Clear the rex sigs associated with MMGSDI ABORT TIMER.
        */
        (void)rex_clr_sigs( mmoc_tcb_ptr, MMOC_MMGSDI_ABORT_TIMER_SIG );

        if(mmoc_state_info.dev_prop.mmoc_mmgsdi_wait_info.file_enum
            == MMGSDI_CSIM_PRL)
        {
          mmocmmgsdi_session_read_prl_cb(MMGSDI_SIM_BUSY,MMGSDI_SESSION_READ_PRL_CNF,cnf);
        }
        else if(mmoc_state_info.dev_prop.mmoc_mmgsdi_wait_info.file_enum
                != MMGSDI_NO_FILE_ENUM)
        {
          /* Clear the rex sigs associated with MMGSDI ABORT TIMER.
               */
          (void)rex_clr_sigs( mmoc_tcb_ptr, MMOC_MMGSDI_ABORT_TIMER_SIG );

          cnf->read_cnf.access.file.file_enum =
            mmoc_state_info.dev_prop.mmoc_mmgsdi_wait_info.file_enum;

          mmocmmgsdi_session_read_cb(MMGSDI_SIM_BUSY,MMGSDI_READ_CNF,cnf);
        }

        mmoc_mem_free(cnf);
      }

    } /* if ( sigs & MMOC_SANITY_TIMER_SIG ) */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef TEST_FRAMEWORK
    #error code not present
#endif /* FEATURE_MMODE_QTF */

  }  /* while ( TRUE ) */

} /* mmoc_task() */


/*===========================================================================

FUNCTION MMOC_INIT_AFTER_TASK_START

DESCRIPTION
  Initilize MMOC after task is started

===========================================================================*/
void mmoc_init_after_task_start
(
  mmoc_state_info_s_type   *mmoc_state_info_ptr
  /* Pointer to MMoC's state information.
  */
)
{
  int sub = 0;
  int stk = 0;

  /* variable used to retrieve general information of MMOC.
  */
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();

  for(sub = 0; sub < MAX_SIMS; sub++)
  {
    mmgsdi_info_ptr->read_items[sub] = MMOC_MMGSDI_MASK_NONE;
  }

  mmoc_read_policyman_config(mmoc_state_info_ptr, SYS_MODEM_AS_ID_MAX, TRUE);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mmgsdi_info_ptr->mmgsdi_client_id                          = 0;
  mmgsdi_info_ptr->mmgsdi_session_id_table[0].session_type   = MMGSDI_GW_PROV_PRI_SESSION;
  mmgsdi_info_ptr->mmgsdi_session_id_table[0].session_opened = FALSE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[0].app_info = MMGSDI_APP_NONE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[0].asubs_id = SYS_MODEM_AS_ID_1;

  mmgsdi_info_ptr->mmgsdi_session_id_table[1].session_type   = MMGSDI_1X_PROV_PRI_SESSION;
  mmgsdi_info_ptr->mmgsdi_session_id_table[1].session_opened = FALSE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[1].app_info = MMGSDI_APP_NONE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[1].asubs_id = SYS_MODEM_AS_ID_1;

  mmgsdi_info_ptr->mmgsdi_session_id_table[2].session_type   = MMGSDI_GW_PROV_SEC_SESSION;
  mmgsdi_info_ptr->mmgsdi_session_id_table[2].session_opened = FALSE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[2].app_info = MMGSDI_APP_NONE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[2].asubs_id = SYS_MODEM_AS_ID_2;

  mmgsdi_info_ptr->mmgsdi_session_id_table[3].session_type   = MMGSDI_1X_PROV_SEC_SESSION;
  mmgsdi_info_ptr->mmgsdi_session_id_table[3].session_opened = FALSE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[3].app_info = MMGSDI_APP_NONE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[3].asubs_id = SYS_MODEM_AS_ID_2;

  mmgsdi_info_ptr->mmgsdi_session_id_table[4].session_type   = MMGSDI_CARD_SESSION_SLOT_1;
  mmgsdi_info_ptr->mmgsdi_session_id_table[4].session_opened = FALSE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[4].app_info = MMGSDI_APP_NONE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[4].asubs_id = SYS_MODEM_AS_ID_1;

  mmgsdi_info_ptr->mmgsdi_session_id_table[5].session_type   = MMGSDI_CARD_SESSION_SLOT_2;
  mmgsdi_info_ptr->mmgsdi_session_id_table[5].session_opened = FALSE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[5].app_info = MMGSDI_APP_NONE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[5].asubs_id = SYS_MODEM_AS_ID_2;

#ifdef FEATURE_MMODE_TRIPLE_SIM
  mmgsdi_info_ptr->mmgsdi_session_id_table[6].session_type   = MMGSDI_GW_PROV_TER_SESSION;
  mmgsdi_info_ptr->mmgsdi_session_id_table[6].session_opened = FALSE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[6].app_info = MMGSDI_APP_NONE;
  mmgsdi_info_ptr->mmgsdi_session_id_table[6].asubs_id = SYS_MODEM_AS_ID_3;
#endif

  for(sub = 0; sub < MAX_SIMS; sub++)
  {
    mmgsdi_info_ptr->sim_busy_status[sub] = FALSE;
    mmgsdi_info_ptr->sim_available_status[sub] = FALSE;
  }
  
  /* Initialize MMGSDI */
  mmocmmgsdi_init(mmoc_state_info_ptr);

#if defined(FEATURE_QSH_DUMP) || defined(FEATURE_QSH_MDUMP)
  /* register with QSH */
  mmocdbg_qsh_init();
#endif

#ifdef FEATURE_MODEM_CONFIG_REFRESH
  mmoc_mcfg_refresh_register();
#endif

#ifdef MMOC_CDMA_SUPPORTED

#ifdef FEATURE_MMODE_DUAL_SIM
  /* Indicate capability to 1x for single SIM */
  if(mmoc_state_info_ptr->dev_prop.onebuild_feature != SYS_OVERALL_FEATURE_MODE_MULTISIM)
#endif /*FEATURE_MMODE_DUAL_SIM*/
  {
    sys_vfr_e_type vfr_map[MAX_AS_IDS];

    cm_get_vfr_map(&vfr_map[0]);

    mcc_set_asid_vfr_info(SYS_MODEM_AS_ID_1, vfr_map[SYS_MODEM_AS_ID_1]);

    /* Inform NV and 1x of the new 3GPP2 subscription */
    nvruim_switch_1x_session(NVRUIM_1X_PROV_PRI_SESSION);
  }
#endif /*MMOC_CDMA_SUPPORTED*/

  /* Initialize mmoc_susp_resume_status */
  for(sub = 0; sub < MAX_SIMS; sub++)
  {
    for(stk = 0; stk < MAX_STACKS; stk++)
    {
      mmoc_susp_resume_status[sub][stk] = FALSE;
    }
  }

  return;

}  /* mmoc_init_after_task_start() */


/*===========================================================================

FUNCTION: mmoc_update_ps_enabled_subsc_chgd_trans

DESCRIPTION
  This function updates mmoc_info_ptr with the stack that has ps.

DEPENDENCIES
  None

RETURN VALUE
 void
SIDE EFFECTS
  None
===========================================================================*/
void  mmoc_update_ps_enabled_subsc_chgd_trans(mmoc_state_info_s_type *mmoc_info_ptr)
{
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(mmoc_dev_info->subsc_chgd.sub_prop[sub] != NULL)
    {
      MMOC_MSG_HIGH_2("SUBSC_CHGD: sub %d domain_pref %d",
                      sub,
                      mmoc_dev_info->subsc_chgd.sub_prop[sub]->srv_domain_pref);

      if (mmoc_dev_info->subsc_chgd.sub_prop[sub]->srv_domain_pref == SD_SS_SRV_DOMAIN_PREF_PS_ONLY  ||
          mmoc_dev_info->subsc_chgd.sub_prop[sub]->srv_domain_pref == SD_SS_SRV_DOMAIN_PREF_CS_PS)
      {
        mmoc_sub_info[sub]->ps_enabled = TRUE;
      }
      else
      {
        mmoc_sub_info[sub]->ps_enabled = FALSE;
      }
    }
  }

} /*  mmoc_update_ps_enabled_subsc_chgd_trans() */

/*===========================================================================

FUNCTION mmoc_get_dbg_buff_ptr

DESCRIPTION
  This function will return the pointer to the MMoC's Debug Buffer.

DEPENDENCIES
  None.

RETURN VALUE
  mmoc_debug_buffer_s_type* - Pointer to the local definition of MMoC's Debug Buffer
  Note! Cannot be NULL.

SIDE EFFECTS
  None.
===========================================================================*/
mmoc_debug_buffer_s_type *mmoc_get_dbg_buff_ptr(void)
{
  return &mmoc_debug_buffer;
}

#ifdef FEATURE_MMOC_UNIT_TEST
#error code not present
#endif


/*===========================================================================

FUNCTION mmoc_check_if_only_subs_cap_chgd

DESCRIPTION
  This function will change the stop mode if there is no change in
  standby_pref and subscription availability

===========================================================================*/

void         mmoc_check_if_only_subs_cap_chgd(
  const mmoc_cmd_msg_s_type      *cmd_ptr,
  boolean                         subs_cap_changed_only[],
  mmoc_state_info_s_type         *mmoc_info_ptr)
{
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;

  if (mmoc_info_ptr->dev_prop.trans != MMOC_TRANS_SUBSC_CHGD ||
      cmd_ptr->param.subsc_chgd.dev_prop.chg_type == MMOC_SUBSC_CHG_NAM_SEL)
  {
    return;
  }

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(cmd_ptr->param.subsc_chgd.sub_prop[sub] != NULL &&
        cmd_ptr->param.subsc_chgd.sub_prop[sub]->prot_subsc_chg != PROT_SUBSC_CHG_NONE &&
        mmoc_info_ptr->sub_prop[sub]->sub_avail &&
        mmoc_info_ptr->sub_prop[sub]->sub_avail == cmd_ptr->param.subsc_chgd.sub_prop[sub]->sub_avail)
    {
      subs_cap_changed_only[sub] = TRUE;
    }
  }

}


/*===========================================================================

FUNCTION mmoc_evaluate_deact_reason


DESCRIPTION
  If there is no change in standby pref or subscription then MMOC needs to
  use another stop mode reason to prevent NAS from clearing their internal
  lists (like list of forbidden systems).


DEPENDENCIES
  None.

RETURN VALUE
  The new deact reason to send to NAS

SIDE EFFECTS
  None
===========================================================================*/
prot_deact_e_type mmoc_evaluate_deact_reason(
  mm_sub_stk_id_s_type            mm_id,
  prot_deact_e_type               deact_reason,
  mmoc_state_info_s_type          *mmoc_info_ptr)
{
#ifdef FEATURE_MMODE_DUAL_SIM
  if(mm_id.asubs_id < SYS_MODEM_AS_ID_1 ||
      mm_id.asubs_id >= MAX_SIMS ||
      mmoc_info_ptr->sub_prop[mm_id.asubs_id] == NULL ||
      mmoc_info_ptr->dev_prop.trans != MMOC_TRANS_SUBSC_CHGD ||
      deact_reason == PROT_DEACT_DUAL_SWITCH ||
      !mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM))
  {
    return deact_reason;
  }

  if (mmoc_info_ptr->dev_prop.subsc_chgd.dev_prop.chg_type == MMOC_SUBSC_CHG_MODE_CHNG)
  {
    return PROT_DEACT_CAP_CHANGE;
  }

  if(mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[mm_id.asubs_id] != NULL &&
      mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[mm_id.asubs_id]->only_subs_cap_change == TRUE)
  {
    return PROT_DEACT_CAP_CHANGE;
  }

#endif
  return deact_reason;
}

/*===========================================================================

FUNCTION mmoc_rpt_sd_nam_sel_cnf

DESCRIPTION
  This function should be used by the protocols to acknowledge the MMoC that
  the "SD NAM SEL" is done
  This function will queue the sd nam sel cnf
  report to the MMoC task.

  Note! This should be called only after completing the necessary
  housekeeping actions based on the request.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                           mmoc_rpt_sd_nam_sel_cnf
(
  void      *context
)
{

  mmoc_rpt_msg_s_type                  *msg_ptr;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get the report buffer.
  */
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                   = MMOC_RPT_SD_NAM_SEL_CNF;
  memscpy(&(msg_ptr->param.sd_nam_sel_cnf),sizeof(mmoc_rpt_subs_sd_nam_sel_cnf_s_type),
          context,sizeof(mmoc_rpt_subs_sd_nam_sel_cnf_s_type));

  /* Free context buffer */
  mmoc_mem_free(context);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );


  return;
}

/*===========================================================================

FUNCTION mmoc_is_standby_pref_and_active_subs_in_sync


DESCRIPTION
Checks if active_subs and standby_pref are in sync or not


DEPENDENCIES
  None.

RETURN VALUE
True/False

SIDE EFFECTS
  None
===========================================================================*/
boolean mmoc_is_standby_pref_and_active_subs_in_sync(mmoc_state_info_s_type         *mmoc_info_ptr)
{
  // Because of MCFG Refresh standby_pref and active_subs can go out of sync. We need to inform MCS only
  // when both are in sync
  if ((mmoc_info_ptr->dev_prop.standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY &&
       mmoc_info_ptr->dev_prop.active_subs  == (SYS_MODEM_AS_ID_1_MASK | SYS_MODEM_AS_ID_2_MASK)) ||
      (mmoc_info_ptr->dev_prop.standby_pref == SYS_MODEM_DS_PREF_SINGLE_STANDBY &&
       (mmoc_info_ptr->dev_prop.active_subs == SYS_MODEM_AS_ID_1_MASK ||
        mmoc_info_ptr->dev_prop.active_subs == SYS_MODEM_AS_ID_2_MASK)) ||
      (mmoc_info_ptr->dev_prop.standby_pref == SYS_MODEM_DS_PREF_NO_STANDBY_PREF &&
       !mmoc_info_ptr->dev_prop.active_subs))
  {
    return TRUE;
  }
  return FALSE;
}

#ifdef FEATURE_MMODE_DUAL_SIM
/*===========================================================================

FUNCTION mmoc_hold_timer_done_cb

DESCRIPTION
  The timer to hold sub acquisition expires.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void                            mmoc_hold_timer_done_cb
(
  int32                           time_ms,

  timer_cb_data_type              data
)
{
  (void) rex_set_sigs(mmoc_tcb_ptr, MMOC_HOLD_ACQ_TIMER_SIG);
} /* mmoc_timer_done_cb() */

/*===========================================================================

FUNCTION mmoc_hold_acq

DESCRIPTION
  Hold on acquisition on one sub while waiting for service on the other sub
  or timeout to proceed.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if cm indicates srv before timeout to MMOC.

SIDE EFFECTS
  None
===========================================================================*/
void  mmoc_hold_acq (
  sys_modem_as_id_e_type        asubs_id,
  mmoc_state_info_s_type        *mmoc_info_ptr
)
{
  rex_sigs_type sigs;
  cm_hold_acq_type hold_acq;
  timer_cb_data_type data;
  memset(&data, 0, sizeof(data));
  memscpy(&data, sizeof(data), &asubs_id, sizeof(asubs_id));

  MMOC_MSG_HIGH_4("HOLD_ACQ: sub %d wait for srv or timeout, trans %d, trans_state %d, active_subs %d", 
                                asubs_id, 
                                mmoc_info_ptr->dev_prop.trans,
                                mmoc_info_ptr->dev_prop.trans_state,
                                mmoc_info_ptr->dev_prop.active_subs);
  (void)rex_clr_sigs(mmoc_tcb_ptr, MMOC_PROCEED_ACQ_SIG);
  (void)rex_clr_sigs(mmoc_tcb_ptr, MMOC_HOLD_ACQ_TIMER_SIG);
  
  /* Start hold timer */
  timer_reg(&mmoc_info_ptr->dev_prop.hold_timer,
            (timer_t2_cb_type)mmoc_hold_timer_done_cb,
            (timer_cb_data_type) data,
            (timetick_type)(MMOC_HOLD_ACQ_TIME),
            0
           );

  hold_acq.online_hold_acq = 
    (mmoc_info_ptr->dev_prop.trans == MMOC_TRANS_ONLINE?TRUE:FALSE);
  hold_acq.hdr.asubs_id = asubs_id;
  mmoc_send_cm_rpt(CM_HOLD_ACQ, (cm_sd_rpt_u_type *)&hold_acq, asubs_id);

  /* Wait for time-out or successful service indication */
  sigs = mmoc_wait( MMOC_HOLD_ACQ_TIMER_SIG|MMOC_PROCEED_ACQ_SIG);

  /* clear timer and signals */
  (void) timer_clr(&mmoc_info_ptr->dev_prop.hold_timer, T_NONE);
  
  (void)rex_clr_sigs(mmoc_tcb_ptr, MMOC_PROCEED_ACQ_SIG);
  (void)rex_clr_sigs(mmoc_tcb_ptr, MMOC_HOLD_ACQ_TIMER_SIG);

  if (sigs & MMOC_PROCEED_ACQ_SIG)
  {
    MMOC_MSG_HIGH_1("PROCEED_ACQ: sub %d due to srv", asubs_id);
  }
  else
  {
    MMOC_MSG_HIGH_1("PROCEED_ACQ: sub %d due to timeout", asubs_id);
    mmoc_send_cm_rpt(CM_NO_HOLD_ACQ, NULL, asubs_id);
  }
}

/*===========================================================================

FUNCTION mmoc_set_proceed_acq_sig

DESCRIPTION
  CM indicates MMOC to proceed on the acquisition.

RETURN VALUE
  NONE

DEPENDENCIES
  None
===========================================================================*/
void mmoc_set_proceed_acq_sig(void)
{
  (void) rex_set_sigs(mmoc_tcb_ptr, MMOC_PROCEED_ACQ_SIG);
}  

/*===========================================================================

FUNCTION mmoc_delay_srv_req

DESCRIPTION
  For srlte(dds)+x configuration, if dds starts first, we want to ensure
  srv request for hybr2 on srlte dds sub is sent first before the other sub.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if mmode needs to delay srv request for the on-hold sub.

SIDE EFFECTS
  None
===========================================================================*/
boolean  mmoc_delay_srv_req (
  sys_modem_as_id_e_type        hold_sub,
  mmoc_state_info_s_type        *mmoc_info_ptr
)
{
  sys_modem_as_id_e_type srlte_sub = mmoc_get_srlte_sub(mmoc_info_ptr);

  /* FOR srlte(dds)+x config, if dds starts first, we want to ensure cmregprx sends
  ** srv request for hybr2 on srlte dds sub first before the other sub.
  */
  if (srlte_sub != SYS_MODEM_AS_ID_NONE &&
     srlte_sub != hold_sub)
  {
    return TRUE;
  }
  return FALSE;
}

#endif

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION mmoc_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for MMOC global variables

DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
void mmoc_qsh_mdump_collect()
{
  int sub,stack;
  qsh_mdump_collect_high(&mmoc_modem_heap_blocks,sizeof(uint16));
  
  qsh_mdump_collect_high(&ph_stat_cmd_tbl[0],MMOC_MODE_MAX*sizeof(mmoc_mode_info_s_type));
 
#ifdef MMODE_ADDITIONAL_DEBUG_INFO
  qsh_mdump_collect_high(&mmoc_debug_buffer,sizeof(mmoc_debug_buffer_s_type));
  qsh_mdump_collect_high(&curr_rpt_idx,sizeof(uint8));
#endif

  qsh_mdump_collect_high(&mmoc_mmgsdi_info,sizeof(mmoc_mmgsdi_info_s_type));

  qsh_mdump_collect_high(&mmoc_state_info,sizeof(mmoc_state_info_s_type));

  for (sub = SYS_MODEM_AS_ID_1; sub < MAX_SIMS; sub++)
  {
    qsh_mdump_collect_high(mmoc_state_info.sub_prop[sub],sizeof(mmoc_state_info_sub_prop_s_type));
    if (mmoc_state_info.sub_prop[sub])
    {
        for (stack = 0; stack < MAX_STACKS; stack++)
        {
          qsh_mdump_collect_high(mmoc_state_info.sub_prop[sub]->stack_prop[stack], sizeof(mmoc_state_info_stack_prop_s_type));
          if (mmoc_state_info.sub_prop[sub]->stack_prop[stack])
          {
            qsh_mdump_collect_high(mmoc_state_info.sub_prop[sub]->stack_prop[stack]->buffered_auto_deact, sizeof(mmoc_rpt_msg_s_type));
          }
        }
    }
    qsh_mdump_collect_high(&mmoc_task_info.timer[sub],sizeof(timer_type));
  }
  qsh_mdump_collect_high(&mmoc_tcb_ptr,sizeof(rex_tcb_type*));
  qsh_mdump_collect_high(&(mmoc_susp_resume_status[0][0]),MAX_SIMS*MAX_STACKS*sizeof(boolean));

#ifndef MMOC_DEBUG
  qsh_mdump_collect_high(&mmoc_dog_rpt_var,sizeof(dog_report_type));
#endif

  qsh_mdump_collect_high(&mmoc_task_info,sizeof(mmoc_task_info_s_type));
}
#endif
/*=========================================================================

FUNCTION mmoc_update_emerg_pending

DESCRIPTION
  This function will be used to set the emergency pending state.

RETURN VALUE
  None
  
===========================================================================*/
void  mmoc_update_emerg_pending(
  const mmoc_cmd_msg_s_type  *cmd_ptr,
  mmoc_state_info_s_type     *mmoc_info_ptr
)
{
  switch(cmd_ptr->cmd.name)
  {
    case MMOC_CMD_OPRT_MODE_CHGD:
      if (mmoc_info_ptr->dev_prop.pending_emerg_state 
           != cmd_ptr->param.oprt_mode_chgd.pending_emerg_state)
      {
        MMOC_MSG_HIGH_2("E911_PENDING: UPDATE: state %d->%d, upon ONLINE",
                             mmoc_info_ptr->dev_prop.pending_emerg_state,
                             cmd_ptr->param.oprt_mode_chgd.pending_emerg_state);
        mmoc_info_ptr->dev_prop.pending_emerg_state
          = cmd_ptr->param.oprt_mode_chgd.pending_emerg_state;
      }
      break;

    case MMOC_CMD_PROT_GEN_CMD:
      if (cmd_ptr->param.gen_prot_cmd.stk_id == MM_STACK_0 &&
          mmoc_info_ptr->dev_prop.pending_emerg_state
           != cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.pending_emerg_state)
      {
        MMOC_MSG_HIGH_2("E911_PENDING: UPDATE: state %d->%d, upon GEN_CMD",
           mmoc_info_ptr->dev_prop.pending_emerg_state,
           cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.pending_emerg_state);
        mmoc_info_ptr->dev_prop.pending_emerg_state
          = cmd_ptr->param.gen_prot_cmd.param.pref_sys_chgd.pending_emerg_state;
      }
      break;

    default:
      break;
  }
}

/*=========================================================================

FUNCTION mmoc_is_emerg_pending

DESCRIPTION
  This function will be used to get the emergency pending state

RETURN VALUE
  Boolean
  
===========================================================================*/
boolean  mmoc_is_emerg_pending(
  mmoc_state_info_s_type         *mmoc_info_ptr
)
{
  if (mmoc_info_ptr->dev_prop.pending_emerg_state == CM_E911_PENDING_STATE_ON)
  {
    MMOC_MSG_HIGH_0("E911_PENDING: ON");
    return TRUE;
  }
  return FALSE;
}

/*=========================================================================

FUNCTION mmoc_skip_online_activation

DESCRIPTION
  This function checks protocol activation should be skipped.
  Skip activation if online emergency call pending. The reason is that CM will emergency 
  preference following online process. No need to act online activation.

RETURN VALUE
  Boolean
  
===========================================================================*/
boolean  mmoc_skip_online_activation(
  mmoc_state_info_s_type  *mmoc_info_ptr
)
{
  return mmoc_is_emerg_pending(mmoc_info_ptr);
}
