#ifndef DRX_H
#define DRX_H
/*===========================================================================

            L 1   D R X   R E L A T E D   D E C L A R A T I O N S

DESCRIPTION

EXTERNALIZED FUNCTIONS

drx_cmd
  This function is called by L1M when RRC commands L1 to begin DRX
  operation. This function calculates the Paging Occasion and the
  Paging Indicator (PI) value and calls the function to calculate
  the parameters that are needed to send to the mDSP to demodulate
  the PICH.

drx_rel 
  This function is called to release DRX operation (called when sccpch
  containing pich is released)
  
drx_pich_done_isr
  This ISR is called by tramp services when the MDSP_INT5 occurs.
  This function will read the piValue register from the mDSP to ascertain
  whether the PICH was successfully demodulated and if there was a page
  for the UE.
  If the UE was paged, the ISR will be de-registered.
  If the UE was not paged, the mDSP will be commanded to again demodulate
  the PICH after the next drx cycle.

drx_sleep_abort
  Aborts Sleep mode if in progress
  
drx_terminate
  This function is called when DRX mode is terminated.
  
drx_process_cmd
  This function processes the DRX sleep local command and implements 
  the DRX state machine
  
drx_sleep_period_complete
   This function is called to signal that the sleep period is complete and
   its time to wake up.

INTERNALIZED FUNCTIONS
drx_mdsp_parms
  This function calculates the parameters that are needed by the mDSP
  for PICH demodulation:
  piThreshold
  bitsPerPi
  piPnCount
  
  It also calculates the actual SFN and the slot when the mDSP command
  to demodulate the PI bits has to be sent.
  
drx_get_pi_pn_count
  This function calculates the pnCount (in units of 256 chips) in a frame
  where the UE is to begin demodulating the PICH. Range 0...143.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  PICH should be set up before DRX operation.

REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) (2002-2015) by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/drx.h_v   1.16   11 Jul 2002 21:26:56   halbhavi  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/drx.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
01/10/20   ac      Mini dump framework checkin
01/31/18   sch     Allow meas gap only if it can accommodate first search
08/31/17   am      KW errors
01/06/17   am      Vote for APS OFF if PO overrun happened in the current PICH cycle
08/15/16    pvr     Feature APS Changes for 1xSRLTE+W
01/02/17   kr      Change threshold value for MEAS GAP proceed
05/20/16   kr      Enable BPLMN from WL1 when ALT SUB  is in DATA
04/01/16   skk     Changes to handle no-lock ext cmd when meas gap is programmed.
02/15/16   skk     Dont block parallel reacq after a no lock cycle.
01/21/16   skk     Pass PI Start position to FW in a async variable in RLP cycles.
01/06/16   gm      Avoid moving to OOS if we don't get the lock for reselection.
12/10/15   nga     Adding enum type for page ind received 
10/15/15   skk     FR26881 : HRP - Half Window Reacq with PICH Decode initial check in.
10/08/15   skk     Decrease the no lock SFN increment threshold to 6 frames. 
09/29/15   pv      Use DL_MAX_NUM_FINGERS_PRI_CARRIER for logging finger statistics in DRX PICH log packet. 
09/25/15   kcm     RxD ACQ BCH handling in BCH inter sib sleep scenarios
09/21/15   sks     SSIM compilation error fixes
09/16/15   kr      timeline fixes to avoid Overran when Other sub is in data call
08/05/15   sks     Unfreeze SS before sending demod cmd if SS was frozen but PICH cfg was not sent
08/03/15   rs      Changes for SR-DR DSDS fallback mechanism 
06/23/15   skk     Mods to change DL WEAK IND for no lock timer during cell selection based on QXDM cmd. 
06/11/15   rs      Macro for updating RGS
06/10/15   kr      Reset W2G BPLMN flag at the start of sleep
06/04/15   rs      Fixed compiler warnings on Atlas
05/29/15   rs      Code changes to enable RLP for 128 DRX cycle
03/20/13   sks     RLP additions to 0x41AA.
05/19/15   sks     Changes to support PICH and measurements in L/W/G + /G.
05/16/15   sks     Change agc to s1 BPG variable from int8 to int32 to avoid overflows. 
05/07/15   skk     FR24494: BCH Inter SIB sleep initial check in.
02/27/15   ap      Initial check-in for WL1 QSH framework 
04/08/15   rs      Reduce SS capture bpgs from 22 to 19 for DS mode
02/25/15   ar      Change to extern function for posting fake drx
03/09/15   rs      Sleep timeline manager changes
02/28/12   rsr     W+W Phase 2 Checkin.
02/25/15   pv      DRX/WSLEEP cleanup for W+W
02/10/15   skk     Timeline fix for long <-> short QXT transisions during Intra f resel.
01/22/15   skk     Wakeup 6 slots early during reselections to facilitate offline PICH to online PICH transitiion timeline.
02/20/14   rs      DRX PICH version 4 log pkt changes 
01/19/15   sks     Global re-org for W+W
12/9/14    sks     Cleanup of outer loop adjustment for RxD.
12/22/14   sks     Post the fake DRX cmd via a local cmd
01/14/15    sks     Log the time of prep wakeup done at the end of the call and not at power clock on.
01/14/15   sks     Compilation fix.
11/03/14   rs      Code changes for DRDSDS fallback mechanism - part 2
06/20/14   skk     FR19218: ReacqLessPICH(RLP) porting for JO.
10/09/14   sl      Added utility API functions to access Rxd mode
09/23/14   pv      APIs for supporting TRM swapping of chains for call setup in DR mode.
08/27/14   skk     Use uint32 for drx_delayed_timer_val variable.
08/12/14   sd      Fixed DSDS Medium and Low compiler warnings in WCDMA.
08/11/14   rs      Created a macro to indicate if panic search conditions are met           
08/04/14   rs      Fixed compiler warnings
07/03/14   sks     Changes for FCDC.
06/30/14   skk     Fixed bugs in DRX PICH log packet (0x41AA)
06/24/14   sks     Initialize drx_int_cntrl_params[wsub_id].ticks_rem_minus_target to 0 inside adjust_outer_loop.
04/01/14   skk     Donot adjust outerloop if RxD is enabled while go to sleep but FOFF at wakeup.
05/20/14   skk     Making drx_delayed_timer as cb based timer from signal based.
04/02/14   skk     Bringing back drx_delayed_timer.
05/22/14   gsk     Update rxd prev mode to NO DIV in case of AGC imbalance, to avoid triage operations
                   on rxd in parallel reacq cases
05/13/14   skk     Removing all includes of "tracer_event_ids.h" and replaced with local file.
05/05/14   rs      FR-3672 CTCH log packet changes. 
03/27/14   sm      OMRD changes to make RxD trm compliant
03/27/14   sks     Add a false page indication field to 0x41AA.
01/23/14   rs      Added Support to include FW parameters for pich log pkt
01/20/14   sks     Do not setup PICH if reacq failed.
12/16/13   skk     Call RRC api can l1 go to sleep in G2W TA mode as well.
12/19/13   rs      Prioritize searches unconditionally when S criteria or reacq fails
12/16/13   as      Porting G2W TA from Triton to Dime.
11/25/13   pv       API for computing mdsp parms after we know the next paging occasion.
11/12/13   sks     Increase the sample capture size by 9 bpgs for DS mode for offline PICH.
09/17/13   jkb     Porting Dual Sim from Triton to Dime
08/14/13   rs      Increase the CTCH gap index to accomodate 512 gaps
08/11/13   gsk     Externing a fucntion
08/06/13   gsk     Code cleanup: Removing unnecessary globals from Enhanced CTCH DB
07/29/13   sks     Inclusion of QDSS profile points for timelining.
07/15/13   gsk     FR 3672: CTCH enhancement changes
07/12/13   ar      Idle RxD enhancement changes for extended 3g coverage
06/24/13   pv      Remove PICH delay timer.
02/15/13   gsk     Definition for PICH demod timer of 30ms 
02/20/13   hk      Removed FEATURE_INTER_SIB_SLEEP
02/20/13   hk      Mainlined FEATURE_WCDMA_DL_ENHANCED
02/11/13   gsk/sr  EDRX Full Sleep initial check in.
02/11/13   pv      Removed unussed API.
01/21/13   pv      Featurization to take care of Triton compile.
01/21/13   pv      DRX PICH log pkt - Offline PICH support for Dime.
14/12/12   kr      Fixing Klwrk Error.
09/23/12   pv      Added DRX PICH log pkt support.
09/11/12   abs     Ported DSDS features
08/29/12   pv      Added new enums/APIs for PICH timeline opts and prev_mode for RXD PCH.
08/27/12   pv      Externed a variable.
08/01/12   pv      New externs.
08/01/12   pv      Externed a function.
07/27/12   pv      Backing out previous checkin.
07/22/12   pv      Added externs.
07/17/12   pv      Warning fixes.
07/16/12   pv      Externs for a function and a timer.
07/13/11   pv      Offline PICH/Parallel Reacq fixes.
11/28/12   pv      Fixed compiler warnings.  
11/28/12   gsk     Fixes for compiler warnings
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
07/11/12   pv      Declared an extern. 
07/05/12   pv      Offline PICH changes.
06/08/12   gv      Feature HS RACH Initial code check in
04/23/12   gsk     Fixed compilation warnings.
02/27/12   stk     Added support for AGC settling done incase of Early RF Shutdown and GTS Failed. 
01/26/12   zr      Mainlined FEATURE_DETECT_STMR_SLP_CLK_MISMATCH, FEATURE_ENHANCED_CELL_RESELECTION, 
                   and FEATURE_WCDMA_GENESIS_SW
01/23/12   uk      Remove extern for gpio strobe init fn
01/20/12   sb      free floating changes.
01/05/12   pv      Created API for drx outer loop adjustment flag.
12/19/11   stk     Added filt_eng to the drx_ext_cntrl_params[wsub_id].drx_rxd_pch struct, to keep track of filtering-1
12/02/11   stk     Added support for FEATURE_WCDMA_RXD_PCH and FEATURE_IDLEMODE_FILTER
11/04/11   zr      Adding power profiling control functionality
09/07/11   pv      Fixed compilation warning.
09/07/11   pv      Define rex timers for drx use.
04/28/11   mmr     Added support for Feature XO.
10/19/10   pv      Added a variable for FWS suspend for Genesis.
02/10/10   ms      Included tlm.h
04/17/09   rmak    Added drx_ensure_rf_is_awake to wake up RF in W-W BPLMN under FEATURE_WCDMA_EARLY_RF_SLEEP
04/17/09   mg      Extern Add drx_sleep_timer_1_isr
03/25/09   scm     Extern boolean drx_ext_cntrl_params[wsub_id].drx_pch_delivery_enabled.
02/27/08   sv      Handle BPLMN with 320ms DRX.
10/22/08   rmak    Remove FEATURE_6275_COMPILE and use MODEM_CLK_HALT directly
10/09/08   rmak    Delete drx_terminate()
09/05/08   scm     Extern drx_ext_cntrl_params[wsub_id].drx_delay_sleep_abort, like drx_ext_cntrl_params[wsub_id].drx_sleep_aborted.
07/15/08   scm     Enable CTCH scheduling with new DL.
06/13/08   scm     Really fix compile problem without FEATURE_WCDMA_DL_ENHANCED.
06/11/08   scm     Fix compile problem without FEATURE_WCDMA_DL_ENHANCED.
06/11/08   scm     drx_send_mdsp_cmds_for_pich_setup() returns boolean now.
06/06/08   scm     Port RRC nudge feature to new DL.
05/23/08   ub      Added macro name for 80ms DRX cycle in num frames
03/19/08   rmak    Removed drx_set_sleep_period_complete_sig
                   and drx_clr_sleep_period_complete_sig
01/29/08   scm     Change API to drx_calc_pi_offset_from_pich().
12/11/07   scm     Extern drx_wake_up().
10/29/07   scm     Remove extern of "drx_just_paged".
10/05/07   scm     Remove featurization of types in drx_state_type.
08/20/07   vsr     Put new DL feature flag around drx_snapshot_sfn_bef_cell_resel()
                   Also moved in some macros.
06/13/07   scm     Extern drx_chk_l1_go_to_sleep().
05/02/07   scm     Extern drx_qxdm_dipsw_disables_sleep().
04/04/07   scm     Extern drx_int_cntrl_params[wsub_id].drx_just_paged and set_drx_state().
03/01/07   scm     Extern some functions for new DRX Manager/MBMS.
09/24/07   rmak    Added MIN_DXC_FOR_CONSEC_S2L_PULLBACKS
                   Added drx_ext_cntrl_params[wsub_id].drx_parms.num_drx_cycles_at_last_s2l_pullback
09/04/07   mg      Add extern drx_recover_stmr
07/02/07   kps     Don't reset DRX cycle count display on entering DRX mode.
04/12/07   kps     Add ability to output Paging counters.
12/08/06   rmak    Mainlining FEATURE MODEM SLEEP CONTROLLER INSTANCE 1
                   and FEATURE INTER SYSTEM TIMING MAINT THROUGH SLEEP CTLR
11/24/06   sk      Added externs for detecting SFN mismatch during cell reselection 
07/24/06   rmak    Introduced new drx_ext_cntrl_params[wsub_id].drx_parms member num_po_overruns
07/06/06   kps     Add declaration for drx_sibs_timer_expired_cmd()
05/02/06   mg      6280: No need to wait for CC slam sig
03/31/06   mg      Add num_pisum_zero and num_pisum_non_zero to drx_ext_cntrl_params[wsub_id].drx_parms
02/15/06   scm     Inter-SIB sleep support inside FEATURE_INTER_SIB_SLEEP.
02/10/06   mg      Change drx_process_command declaration for lint.
01/12/06   mg      6275: Change MODEM_CLK_STATUS_RD to MODEM_CLK_HALT
11/10/05   mg      Added externs for boolean drx_ext_cntrl_params[wsub_id].drx_nudging_rrc and drx_nudge_rrc()
                   FEATURE_DETECT_STMR_SLP_CLK_MISMATCH.
11/08/05   ub      Added extern definition for drx_ext_cntrl_params[wsub_id].drx_sleep_aborted global
11/08/05  gv/nd    Added code for WtoW BPLMN feature.
11/03/04   vp      Added declaration for drx_ok_for_concurrent_pccpch().
07/14/05   kps     make drx_ctch_expired_timer an extern so we can expire the 
                   timer when we receive the frame.
06/20/05   mg      Change drx delayed timeout form 30s to 60s
06/01/05   mg      Extern tcxo_was_shut_down and drx_ext_cntrl_params[wsub_id].stmr_ref_count
05/23/05   scm     Extern wake_up_before_pich, for only 1 CTCH reacq attempt.
05/12/05   scm     Goto sleep between CTCH occasions: FEATURE_UMTS_BMC_SLEEP_OPT.
03/21/05   scm     Modify enums in drx_state_type for BMC support.
03/15/05   scm     Add DRX_WAITING_ON_BMC_READY_FOR_WAKE_UP to drx_state_type.
02/18/05   mg      Add definiton for MSM_CLK_STATUS0
12/30/04   mg      Format the file. No code modifcations
11/03/04   asr     Renamed drx_send_mdsp_cmd() to drx_send_mdsp_cmds_for_pich_setup();
                   Eliminated the external interface drx_pich_sfn_arrived().
08/10/04   src     Added a hash-defined symbol to represent a certain DRX cycle
                   length.
07/08/04   scm     Extern drx_is_awake_for_tcxo_settling().
02/16/04   asr     Added #define for DRX_DELAYED_TIMER_VAL_MS.
10/17/03   asr     Declared drx_sleep_timer_1_clr().
09/03/03   asr     Declared new function drx_delayed()
07/30/03   asr     Declared new function drx_pich_demod_enabled_too_late()
04/01/03   scm     drx_parms_struct_type.min_paging_occasion from uint32 to uint16.
02/12/03   asr     Added act_time_256_chips to structure pich_demod_activation_time;
                   Added pich_sfn to structure pich_parms.
01/20/03   scm     Add drx_ext_cntrl_params[wsub_id].drx_state_at_suspend to remember DRX mode at SUSPEND time.
                   Also extern drx_ext_cntrl_params[wsub_id].drx_cmd_saved for RESUME.
10/21/02   asr     added num_drx_cmds, changed num_drx_cycles changed to uint32 
                   (from uint16) in drx_parms_struct_type
09/09/02   asr     Declared drx_bch_on()
07/27/02   asr     Declared drx_terminate()
07/11/02   sh      Added drx_cmd_fach() to validate drx command in fach.
06/11/02   asr     Declared several helper functions to perform rf_wakeup() 
                   in L1 task context
05/20/02   asr     Added function drx_restart() to restart the DRX state machine
                   with the previous DRX command parameters
05/18/02   sh      Added flag drx_cell_reselect_enabled for cell reselection.
05/12/02   asr     declarations of functions drx_pich_sfn_arrived() and 
                   drx_bch_setup() and drx_bch_tear_down(). New parameter 
                   in drx_ext_cntrl_params[wsub_id].drx_parms to store the 256 chip count for pich tear down
04/25/02   asr     include new parameter mdsp_sccpch_setup_cfn in 
                   drx_parms_struct_type used in "fast" sccpch setup
03/20/02   asr     Changed type of mdsp_pi_threshold from uint16 to int16
03/14/02   asr     Moved drx_state_type and drx_ext_cntrl_params[wsub_id].drx_state into this file from drx.c
02/12/02   asr     changed some function names
01/31/02   asr     support for Sleep : initial cut (function prototypes)
12/13/01   asr     logging support; also added PICh "pwr_offset" to drx_ext_cntrl_params[wsub_id].drx_parms
12/11/01   asr     "imsi" parameters in drx_ext_cntrl_params[wsub_id].drx_parms changed from uint32 to uint64
12/06/01   asr     Changes for 2001-06 compliance: adding imsi_div_8192 to drx_ext_cntrl_params[wsub_id].drx_parms
11/16/01   asr     change of variable/function names for clarity
09/20/01   asr     Modified drx_ext_cntrl_params[wsub_id].drx_parms, added prototypes for helper functions
09/04/01   sh      Created file.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "comdef.h"
#include "l1rrcif.h"
#include "drxcmd.h"
#include "tlm.h"

#include "wl1api.h"

#include "wl1drxmanager.h"
#include "rxdiv_v.h"

#include "mdspsync.h"
#include "drxofflineprocmgr.h"

#include "wsrchlog.h"

#include "wl1sleep.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
#define MAX_ALLOWED_SC_STMR_ERROR_MS (7)

/* Macro that holds the number of frames needed for pccpch setup, crc 
 * evaluation and tearing down of atleast one of the channels The frame
 * level breakup for a worst case scenario is:
 * Frame N: CPHY_SETUP for PCCPCH received
 * N+1: Setup config evt for hwch1 and hwch2
 * N+2: config evt for hwch1 and hwch2 received
 * N+3: frame boundary evt for hwch1 and hwch2 received
 * N+4: idle
 * N+5: CRC report-1 received for hwch1
 * N+6: CRC report-1 received for hwch2
 * N+7: CRC report-2 received for hwch1
 * N+8: CRC report-2 received for hwch2
 * ..
 * ..
 * N+5+DL_BCH_CONC_RPT_CNT_MAX-1: CRC report-DL_BCH_CONC_RPT_CNT_MAX received for hwch2
 * N+5+DL_BCH_CONC_RPT_CNT_MAX: CRC report-(DL_BCH_CONC_RPT_CNT_MAX+1) received for hwch1
 * Failure is detected and the hw channels are marked free
 *.N+5+DL_BCH_CONC_RPT_CNT_MAX+1:The freed hwch is available for some other purpose.
 *.Add one more frame to this to be safe.
 */
#define FRAMES_FOR_CONCURRENT_PCCPCH_SETUP 15

/* Convenience symbols to hold various DRX cycle length values in frames */
#define DRX_CYCLE_LENGTH_80MS_IN_FRAMES    8
#define DRX_CYCLE_LENGTH_320MS_IN_FRAMES   32
#define DRX_CYCLE_LENGTH_640MS_IN_FRAMES   64
#define DRX_CYCLE_LENGTH_1280MS_IN_FRAMES  128
#define DRX_CYCLE_LENGTH_2560MS_IN_FRAMES  256

/* Structure to hold all the parameters necessary for calculating
 * and saving the parameters necessary for PICH demodulation.
 */
#ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
#ifdef FEATURE_BPLMN_SEARCH_320MS
/* bplmn srch is supported for a drx cycle length of 32 frames and more */
#define MIN_DRX_CYCLE_LENGTH_FOR_WTOW_BPLMN 32
#else
/* bplmn srch is supported for a drx cycle length of 64 frames and more */
#define MIN_DRX_CYCLE_LENGTH_FOR_WTOW_BPLMN 64
#endif
#endif

/* Minimum number of consequetive DRX Cycles between short-to-long-qxt transitions */
#define MIN_DXC_FOR_CONSEC_S2L_PULLBACKS 10

/* 
 * The worst  case setup time should stay 2 frames, as CTCH wakeup time is usually 8ms.
 * But in case of turbo coded channel, setup time is incremented by 6-7 ms causing the worst case CTCH wakeup time as 1.5ms.
 * So for the enhanced CTCH algo, we decided to have 2 frames as the worst case CTCH setup time.
*/
#define DRX_CTCH_SETUP_TIME 2
#define DRX_CTCH_DROP_TIME 1
/* Taking the minimum possible CTCH periodicity as 8 and MAX possible DRX cycle length as 512
 * Array size = 512/8 = 64
 */
/* Log packet table will always have 2 extra rows than the num_ctch_oksn 
 for printing current and PICH sfn in that DRX cycle */ 
#define DRX_MAX_CTCH_BTW_PICH CTCH_LOG_TABLE_SIZE

/* Do not open a gap for meas if less than 180 ms away to PICH*/
#define MIN_TIME_MEAS_TO_PICH_US                    (180000)
/* Configurable safety check - Minimum time required to PICH when TRM grant is received
   for meas gap */
   /*Changing value to accommodate the first search in meas gap*/
   /*First one is actaul search duration, second one is time to go to sleep for PICH, and another 30 is divided as follows
     10 msec--> meas grant to pick net search time
     10 msec--> PICH timer to wakeup
     10 msec--> Buffer or if the frame increments exactly at that point of check
   */
     
#define TIME_TO_PICH_NEEDED_AT_MEAS_GAP_GRANT_MS(wsub_id)    ((srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_drx_nxt_gap_info.srch_duration *10) + (DRX_MINIMUM_UE_SLEEP_TIME_USEC_WITH_BUFF(DRX_ENTITY_PICH)/1000) + 30) 
/*10 for meas gap start to pick next, 10 for pich timer to sdt-to-ols, 10 if frame increment hust before mode duration check*/
#ifdef FEATURE_DUAL_SIM
/*This should always be even otherwise if schedule says to wakeup immediately after 
  below value, we might wakeup in the middle of TTI missing the first half of the block.
  eg: BCCH and CTCH with TTI a multiple of 2.*/
#define DRX_SFN_INCREMENT_FOR_NO_LOCK 6
#endif

/* FW interfact still does not have support. Will include once FW support is ready.*/
/* Indices of FW populated params for PICH log pkt.*/
typedef enum
{
  DRX_AGC_DONE_IDX           = WFW_DEMOD_PICH_ASYNC_READ_RXAGC_DONE_IDX,
  DRX_PICH_CFG_IDX           = WFW_DEMOD_PICH_ASYNC_READ_PICH_CFG_IDX,
  DRX_OFFL_START_IDX         = WFW_DEMOD_PICH_ASYNC_READ_OFFL_START_IDX,
  DRX_SS_FRZ_TIME_IDX        = WFW_DEMOD_PICH_ASYNC_READ_SS_FRZ_TIME_IDX,
  //DRX_OFFL_TO_ONL_IDX        = WFW_DEMOD_PICH_ASYNC_READ_OFFL_TO_ONL_IDX,
  DRX_OFFL_PROC_DONE_IDX     = WFW_DEMOD_PICH_ASYNC_READ_OFFL_PROC_DONE_IDX,
  DRX_PI_DONE_IDX            = WFW_DEMOD_PICH_ASYNC_READ_PI_DONE_IDX,
  DRX_PI_BITS_START_POS_IDX  = WFW_DEMOD_PICH_ASYNC_READ_PI_BITS_START_POS_IDX,
  DRX_SCCPCH_POS_IDX         = WFW_DEMOD_PICH_ASYNC_READ_SCCPCH_POS_IDX,
  DRX_SS_WNW_END_POS_IDX     = WFW_DEMOD_PICH_ASYNC_READ_SS_WNW_END_POS_IDX,
  DRX_QXT_DONE_IDX           = WFW_DEMOD_PICH_ASYNC_READ_QXT_DONE_IDX,
  DRX_SS_UNFRZ_TIME_IDX      = WFW_DEMOD_PICH_ASYNC_READ_SS_UNFRZ_TIME_IDX,
  DRX_UNFRZ_RXAGC_DONE_IDX   = WFW_DEMOD_PICH_ASYNC_READ_UNFRZ_RXAGC_DONE_IDX,
  DRX_SCCPCH_ON_IDX          = WFW_DEMOD_PICH_ASYNC_READ_SCCPCH_ON,
  DRX_SS_WNW_START_IDX       = WFW_DEMOD_PICH_ASYNC_READ_SS_WNW_ONL_RPOC_START_IDX,
  DRX_ONL_OFFL_IDX           = WFW_DEMOD_PICH_ASYNC_READ_ON_OFF_DECISION_IDX,
  DRX_SS_WNW_REAL_START_IDX  = WFW_DEMOD_PICH_ASYNC_READ_SS_WNW_REAL_START_POS_IDX,
  DRX_PI_START_RLP           = WFW_DEMOD_PICH_ASYNC_READ_SS_WNW_RLP_POS_IDX,
  
  DRX_NUM_ELEM_FW_PICH_ASYNC_READ = WFW_DEMOD_PICH_ASYNC_READ_NUM_ELEM
} drx_fw_pich_async_read_idx_type;

typedef enum
{
   SEQUENTIAL_REACQ,
   PARALLEL_REACQ,
   REACQLESS_PICH
}drx_cycle_type;

/* FW pupulated params for PICH log pkt.*/
typedef PACKED struct PACKED_POST
{
  uint32 agc_done_time;
  uint32 pich_cfg_time;
  uint32 ss_frz_act_time;
  uint32 online_offline_decision_time;
  uint32 offl_proc_st_time;
  uint32 offl_proc_done_time;
  uint32 pi_decode_done_time;
  uint32 pi_bits_st;
  uint32 ss_window_start_pos;
  uint32 ss_window_end_pos;
  uint32 ss_window_real_start_time;
  uint32 qxt_done_time;
  uint32 pich_ack_ss_unfrz_time;
  uint32 pich_ack_unfrz_agc_tune_done_time;
  uint32 sccpch_on_time;
  uint32 sccpch_fr_start_time;
}drx_fw_pich_async_read_struct_type;
/* Fing stats stuct.*/
typedef PACKED struct PACKED_POST
{ 
  uint16 init_rssi;
  uint16 tcpfSq0;
  uint16 tcpfSq1;
} fing_stats_struct_type;

typedef PACKED struct PACKED_POST
{
   uint8 nbrs_present; /* Neighbors found in latest full srch
                          1 � intra/inter/irat Nbrs are found
                          0 � no neighbors are detected 
                        */

   uint8 rxd_enabled;  /* Is RxD enabled this DRX wakeup
                          1 � RxD enabled  0 � RxD disabled.
                        */
                        
   uint8 sttd;         /* Is STTD enabled this DRX wakeup
                          1 � STTD enabled    0- STTD disabled.
                        */
   uint8 g2w_ta_mode;  /* Did G2W TA happen this DRX wakeup
                          1 � G2W TA happened    0 � No G2W TA
                        */

   uint8 partial_pi;   /* If partial PI happened this DRX wakeup.
                          1 � Partial PI   0- No Partial PI decode.
                        */
   int8 pi_bpgs_missed; /* Num of bpgs we missed due to delay during AGC tune done.*/

   uint8 pich_tto;     /* PICH TTO passed to FW after partial PI adjustment. */

   uint8 pi_bpgs_decoded;   /* Num PI BPGs to decode after partial PI adjustment. */

   uint32 outer_loop_adjustment; /* Final ticks_rem_adj_sleep_longer_Cx8 value after all the adjustments done in a DRX cycle */

}drx_rlp_pi_info_struct_type;

typedef PACKED struct PACKED_POST
{
   uint8 num_valid_bpgs;  /*Number of valid Bpgs FW is using to compute raw pilot values for EcIo proxy.*/

   uint16 raw_pilot_eng;  /*Bit shifted FW raw pilot energy in dbx4 which is used for computing proxy EcIo.*/   

   uint16 raw_pilot_eng_valid_bpgs; /*Bit shifted FW raw pilot energy in dbx4 computed considering num valid bpgs FW used.*/   

   uint16 ref_raw_pilot_eng;  /* Bit shifted FW raw pilot energy in dbx4 which is imported from FW in a non RLP cycle.*/  

   uint16 tot_ecio;   /*Updated EcIo in a RLP cycle after picking up FW raw pilot energy.*/ 

   uint16 ref_tot_ecio;  /* EcIo of last non RLP cycle.*/

   uint16 filt_ecio;   /*  EcIo in a DRX wakeup after applying idle mode filtering. */

   uint32 strongest_fing_pos_non_rlp;  /* Strongest finger position used to bias FW raw pilot energy in non RLP cycle. */ 

   uint32 ref_cell_pos_non_rlp;  /* Reference cell position in non RLP cycle */

   uint32 ref_tot_eng;  /* Cell energy in prev non RLP cycle */

} drx_rlp_pi_proxy_ecio_info_struct_type;

/* Per finger stats info.*/
typedef PACKED struct PACKED_POST
{
  uint8 num_fings;
  fing_stats_struct_type per_fing_stats[DL_MAX_NUM_FINGERS_PRI_CARRIER]; 
} drx_fing_stats_struct_type;

/* Global struct for the PICH log pkt.*/
typedef PACKED struct PACKED_POST
{
  /* PARAMETERS*/
  /* Parallel reacq done this DRX cycle.*/
  drx_cycle_type cycle_type; 

  /* Offline PICH mode this DRX cycle.*/
  boolean offl_pich_this_cycle;  
           
  uint8 qxt;          /*Is QXT enabled this DRX wakeup.
                         1 � Long/Short QXT  0 � No QXT */
           
  int32   piSum;                /* PI SUM value.*/
  uint32  rssiSum;              /* RSSI SUM value.*/
  uint8   piVal;                /* PI Val - UE paged or not.*/  

  uint32 num_pisum_zeros;        /* No.of cumulative PI SUM 0s.*/
  uint32 num_drx_cycles;        /* No.of DRX cycles since last DRX cmd.*/
  uint32 total_drx_cycles;      /* No.of total DRX cycles since bootup.*/
  uint32 num_po_overruns;       /* No.of cumulative PO overran occasions.*/

  int32 outer_loop_freq_error; /* Outer loop frequency error.*/
  int32 tot_freq_err;          /* Total frequency error.*/ 
  int32 freq_common;           /* Freqcommon value obtained from FW.*/

  int32   slew;              /* Absolute slew.*/
  uint32  serv_cell_tot_ecio;     /* Serving cell energy in dB.*/
  uint32  serv_cell_filt_ecio;    /* Serving cell filtered energy in dB.*/  
  uint32  cell_ref_pos_at_wkup;  /* Cell ref pos at wakeup.*/
  uint32  cell_pos_bef_slp;      /* Cell ref pos at slp */
  uint8   num_false_pages;       /* Cumulative number of false pages*/
  boolean   no_rsp_to_page;        /* */

  uint16 drx_cycle_length;
  /* Offset of PI bits from the PICH frame start.
     Bits 0:18 - Offset of PICH frame.
     Bits 19:31 - Offset of PI bits within PICH frame.
  */
  uint32  pi_offset; 

  drx_fing_stats_struct_type drx_fing_stats;

  /* TIMESTAMPS */
  uint32  sleep_ending_isr_st;  /* On-line star timestamp.*/

  uint32  first_reacq_st_time;   /* 1st Reacq issuing time.*/
  uint32  first_reacq_end_time;  /* 1st Reacq end time.*/
  uint32  second_reacq_st_time;  /* 1st Reacq end time.*/
  uint32  second_reacq_end_time; /* 2nd Reacq end time.*/
  uint32  cell_cfg_st_time;      /* 2nd Reacq end time.*/
  uint32  cell_cfg_end_time;     /* Cell/fing cfg preparation start time.*/
  uint32  triage_st_time;        /* Triage start time.*/
  uint32  triage_end_time;       /* Triage start time.*/  
  uint32  rf_slp_time;           /* RF sleep time.*/    

  drx_fw_pich_async_read_struct_type fw_parms_info;

  drx_rlp_pi_info_struct_type rlp_pi_info;

  drx_rlp_pi_proxy_ecio_info_struct_type rlp_proxy_ecio_info;

} drx_pich_log_pkt_struct_type;

LOG_RECORD_DEFINE(DRX_PICH_LOG_PKT)
  /* Version number of this log packet.*/
  uint8 version;

  drx_pich_log_pkt_struct_type log_info;   
LOG_RECORD_END

#define DRX_PICH_LOG_PKT_SIZE() (sizeof(DRX_PICH_LOG_PKT_type))

typedef struct
{
  /* DRX Cycle Length in 10 ms radio-frame units */
  uint16 drx_cycle_len;

  /* log to base 2 of the drx_cycle_len (for logging) */
  uint8 log_drx_cycle_len;

  /* number of elapsed DRX Cycles */
  uint32 num_drx_cycles;

  /* Total number of DRX Cycles. This value is only reset by diag. */
  uint32 total_drx_cycles;

  /* the next paging occasion (sfn of frame that may contain a PI for this UE) */
  uint16 next_paging_occasion;

  /* SFN of PCCPCH frame that contains the (1st of the) PI bits */
  uint16 pich_sfn; 

  /* Paging Indicator */
  uint32 PI;

  /* 10.3.6.41 Number of PI per frame - obtained from RRC during SCCPCH setup; 
  */
  uint8 pi_per_frame;

  /* IMSI div K, as per 8.3 in 25.304 (version 2000-12), used in calculating paging
     occasion, where K is the number of SCCPCHs that carry a PCH.
     Obtained from RRC during SCCPCH setup*/
  uint64 imsi_div_pch_cnt;

  /* IMSI div 8192, as per 8.3 in 25.304 (version 2001-06), used in calculating PI.
     Obtained from RRC during SCCPCH setup*/
  uint64 imsi_div_8192;

 /*
  * "imsi_div_K modulo drx_cycle_length" which is the fixed term of the 
  * paging occasion; the other term used in calculating paging occasion
  * is a variable term. Paging occasions are (fixed term) + n * drx_cycle_len
  * for n = 0,1,2,...
  * this corresponds to the case of n=0.
  */
  uint16 min_paging_occasion;

  /* CCTrCh table index of the SCCPCH that carries the paging blocks */
  uint8 sccpch_cctrch_idx;

  /* Timing Offset of the SCCPCH that is associated with this PICH */
  uint16 sccpch_offset;

  /* the offset of the PICH relative to the PCCPCH (units of 256 chips)*/
  int16 pich_offset;

  /* Threshold for detecting PI bits; required by the mdsp to provide a Y/N decision whether
     the PI bits were +1 or -1 */
  int16 mdsp_pi_threshold;

  /* Number of bits allocated to a paging indicator = 288/Np, where
   * Np is the PI per frame. Range (2, 4, 8, or 16). This parameter
   * is programmed to the mDSP.
   */
  uint8 mdsp_bits_per_pi;

  /* the time difference (in units of 256 chips) from the start of the pich frame to
     when the page indicator bits start in the pich frame
   * Range (0...143).
   */
  uint8 pi_offset_from_pich;

  /* the time difference between the start of the PI bits to the start of the
    previous PCCPCH frame. In other words, 
    the sum of ext_drxofflineprocmgr_cntrl->pi_offset_from_pich and pich_offset parameters described above, modulo 150.
    An mdsp parameter*/
  uint16 mdsp_pi_pn_count;

  /* the count, in 256 chip units, at which pich tear down should occur */
  uint16 mdsp_pich_tear_down_pi_pn_count;

  /* whether sccpch with pich was setup before drx i.e. whether to trust the values of
  bits_per_pi etc ..*/
  boolean sccpch_with_pich_setup;

  /* 25.331 10.3.6.50 : power transmitted on PICH minus power transmitted on CPICH
     in dB (range -10..5) */
  int8 pich_pwr_offset;

  uint8 mdsp_sccpch_setup_cfn;

  /* number of DRX commands received from RRC, and accepted by L1 */
  uint32 num_drx_cmds;

  /* number of pi sum zeros */
  uint32 num_zero_pisum;

  /* number of pi sum non zeros */
  uint32 num_non_zero_pisum;

  /* number of po overruns */
  uint32 num_po_overruns;

  /* Cumulative number of false pages*/
  uint32 num_false_pages;       

  /* number of drx cycles since last slp-time-pullback in a
     short-to-long qxt transition */
  int32 num_drx_cycles_at_last_s2l_pullback;

} drx_parms_struct_type;

/* This structure defines the actual SFN occasion and slot when the mDSP
 * command to demodulate the PI bits is sent. Depending on the position
 * of the PI bits to be demodulated with respect to the PCCPCH frame of the
 * Paging Occasion, the following cases for the activation times result:
 * if pi bit position < 1 slots  
 *   then sfn = (paging occasion - 1), slot 12
 * if 3 slots <= pi bit position <= 13 slots
 *   then sfn = paging occasion
 * if 13 slots < pi bit position <= 16 slots
 *   then sfn = paging occasion, slot 12
 * if 16 slots < pi bit position <= 28 slots
 *   then sfn = paging occasion + 1
 * if 28 slots < pi bit position < 30 slots
 *    then sfn = (paging occasion + 1), slot 12
 *
 * An Event is set up at this activation time and has to be 512 chips
 * before and atmost 13 slots from the PI bits.
 *
 *  To be safer (not assuming mDSP receives command from L1 within 1 slot,
 *  but relaxing that to 3 slots),
 *  use this: 
 *  if (0 slots <= pi bit position < 5 slots) 
 *                             then sfn = (paging occasion - 1; slot 10)
 *  if (5 slots <= pi bit position <= 13 slots) then 
 *                             then sfn = (paging occasion; slot 0)
 *  if (13 slots <= pi bit position < 23 slots) then 
 *                             then sfn = (paging occasion; slot 10)       
 *  if (23 slots <= pi bit position <= 28 slots) then 
 *                             then sfn = (paging occasion + 1; slot 0)
 *  if (28 slots < pi bit position < 30 slots) then 
 *                             then sfn = (paging occasion + 1; slot 10)        
 */

typedef struct
{
  /* whether we are in drx mode or not */
  boolean drx_mode;

  /* SFN of the frame where the mDSP config window begins */
  uint16 sfn;

  /* activation time of the pich demod event in units of 256 chips */
  uint16 act_time_256_chips;

} pich_demod_activation_time;

/* definition of the states of the DRX/Sleep state machine */
typedef enum 
{
  DRX_IDLE,
  DRX_CONFIGURED,
  DRX_PICH_DEMOD,
  DRX_WAITING_ON_SRCH_READY_FOR_SLEEP,
  DRX_WAITING_ON_DL_READY_FOR_SLEEP,
  DRX_WAITING_ON_RF_WARMED_UP,
  DRX_WAITING_ON_SRCH_WOKEN_UP,
  DRX_WAITING_ON_DL_WOKEN_UP,
  DRX_WAITING_ON_BMC_CTCH_ENABLED,
  DRX_WAITING_ON_BMC_CTCH_DROPPED,
  DRX_CTCH_ACTIVE,
  DRX_WAITING_ON_BPLMN_COMPLETE,
  DRX_NUM_STATES
} drx_state_type;


/*
 * Type definition for various profile points in the timeline.
 */

/* 60sec (deliberately set >> out-of-service timer which is 12 seconds 
 * DRX delayed timer duration should exceed the max SIB repetition period 
 * (which is 40.96 seconds per spec). This is to take care of inter-freq 
 * neighbors (during sib reads of inter-freq neighbors, reacq search 
 * continuously fails because RF is tuned away;  
 */
/*Timer value is decided at the time of drx_cmd based on drx_cycle_len.*/


#define DRX_PICH_TIMER_VAL_MS (30)
#define GET_DRX_STATE() drx_ext_cntrl_params[wsub_id].drx_state

#define DRX_RXD_PCH_NV_ENABLED()    (drx_ext_cntrl_params[wsub_id].drx_rxd_pch.idle_rxd_enabled == TRUE)
#define DRX_RXD_PCH_FULL_DIV_MODE() ((DRX_RXD_PCH_NV_ENABLED()) && \
                                     (drx_ext_cntrl_params[wsub_id].drx_rxd_pch.idle_rxd_curr_mode == RXD_PCH_FULL_DIV))

#ifdef FEATURE_WCDMA_REACQLESS_PICH
/* Number of offline PICH samples required in case of RLP.
** TO DO - Use 12 BPGs for Np=18 alone and for the rest, use 10 BPGs. 8 BPGs are 
** enough for an INTRA PN LPQ search of 2048 * 1 .*/
#define DRX_NUM_SAMPLES_REQUIRED_FOR_RLP 12
#endif

extern boolean wl1_use_fws_suspend;

typedef struct
{
  /* variable to tell whether idle_rxd is enabled or not via NV. */
  boolean idle_rxd_enabled;
  /* Wakeup handle/entity to tell the reason for wakeup */
  int8 wakeup_entity_for_idle_rxd;
  /* variable to store the current RXD_State after wakeup. */
  rxd_pch_mode_enum_type idle_rxd_curr_mode;
  /* variable to store the next RXD_State during sleep. */
  rxd_pch_mode_enum_type idle_rxd_next_mode;
  /* variable to store the prev RXD_State during sleep. (Used for parallel Reacq case) */
  rxd_pch_mode_enum_type idle_rxd_prev_mode;
  /* variable to store piSum value thats read from MDSP for use by Idle RxD */
  int32  piSum_for_idle_rxd;
  /* Flobal remember the page staus for this DRX cycle  */
  boolean page_received_this_drx;
  /* Variable to tell whether reacq srch was issued on diversity antenna.*/
  boolean reacq_issued_on_div_ant;
  /* Variable to store filt_eng from the best of two primary searches, to be used while calculating_next_mode() */
  uint32  filt_eng;
  /* Variable to store filt_ecio from the best of two primary searches, to be used while calculating_next_mode() */
  uint16 filt_ecio;
  /* Variable to store filt_ecio from the best of two primary searches, to be used while calculating_next_mode() */
  boolean is_div_srch_better;
  /*variable to store the filtered energy for PWC algorigthm*/
  uint32 filt_eng_pwc;
  /*variable to store the filtered energy for PWC algorigthm*/
  int16 filt_rscp_pwc;
  /* Variable to store the AGC imbalance decision made by RxD module */
  boolean rxd_out_of_lock;
}drx_rxd_pch_struct_type;

typedef struct
{
  boolean rxd_bch_active;
}drx_rxd_bch_struct_type;

typedef struct
{
  boolean parallel_reacq_next_cycle;
  boolean parallel_reacq_this_cycle;
  boolean parallel_reacq_peformed;
}drx_parallel_reacq_db_type;

#ifdef FEATURE_WCDMA_REACQLESS_PICH
typedef struct
{
/*whether RLP  conditions met for next cycle or not.*/
  boolean possible_next_cycle; 
  
/*RLP will happen this cycle */
  boolean this_cycle;  
  
  /*Variables to keep track wheter we passed S1 point and by how much*/
  boolean drx_agc_time_not_suitable_rlp;

  /*diff between agc to s1 point in bpgs.*/
  int32 drx_agc_done_to_s1_bpg;
  
  /*Saves the target ticks to PI in the current DRX cycle when RLP is enabled.*/
  uint32 wsleep_target_ticks_rem;
}drx_rlp_db_type;
#endif
                                    
typedef void (*drx_pich_entity_srch_to_sleep_cb_type)(wsub_id_e_type wsub_id);


typedef enum
{
 DRX_OPTS_INVALID_ACTION,
 DL_TRI_WORKLOOP_ACTION,
 SRCHZZ_SLEEP_OPS_ACTION
}drx_opts_svc_action_type;


typedef enum
{
  DRX_CTCH_INVALID,
  DRX_CTCH_SCHEDULE_SFN,
  DRX_CTCH_DATA_SFN,
} drx_ctch_sfn_type;

typedef struct
{
  /* Holds the total amount of interf search time */
  uint8 ifreq_srch_time;

  /* Holds the total amount of interf search time */
  uint8 irat_srch_time;

  /* Holds the info whether to allow searches instead of CTCH
   * Flag will be set in case of bad serving cell conditions
   */
  boolean prioritize_srchs_over_ctch;

  /* Bitmask holds the gap index where the interf/irat searches fit */
  uint32 ifreq_irat_bit_mask;
  uint32 ifreq_bit_mask;
  uint32 irat_bit_mask;

  /* Holds the index of a CTCH schedule SFN between two PICH oksns */
  uint16 ref_ctch_sfn_idx;

  /* Holds the index of next possible CTCH data schedule SFN between two PICH oksns*/
  uint16 nxt_ctch_sfn_idx;
  
  /* Holds the info of the num of CTCH SFNs and the array stores the
   * CTCH SFNs in the following order between two PICH oksns
   */
  /* Array structure have pich sfn1 ctch sfn1 ctch sfn2 ... pich sfn2 */
  uint32 ctch_sfn_arr[DRX_MAX_CTCH_BTW_PICH];
  uint32 num_ctch_sfns_between_pi_oksns ;
  
  /* Holds the schedule type(DATA/SCHEDULE) of the CTCH SFN */
  drx_ctch_sfn_type ctch_sfn_priority[DRX_MAX_CTCH_BTW_PICH];

  /* Flag used to unconditionally prioritize search over CTCH
     when panic conditions are met */

  boolean panic_srch_cond_met;

  uint16 ctch_gap_idx_cnt;

  /* Holds the SFNs and the num of SFNs that are to be notified to DL to invalididate for decoding */
  uint32 ctch_sfn_invalidate[DRX_MAX_CTCH_BTW_PICH];
  uint8 num_ctch_sfn_blown_for_srchs;

  boolean block_srch_for_ctch_decision;
  boolean srch_skipped_for_ctch;
    
} drx_enh_ctch_algo_struct_type;

/* temp struct used to store info required for CTCH log pkt */
typedef struct
{
  /* gap in frames*/
  uint16 gap_size;

  /* whether CTCH skipped*/
  boolean ctch_skipped;

  /* Ifreq srch was fit */
  boolean ifreq_fit;

  /* Irat srch was fit */
  boolean irat_fit;

}drx_ctch_sfn_info_struct_type;

typedef enum
{
  DRX_CTCH_IFREQ_IRAT_FIT,
  DRX_CTCH_IFREQ_FIT,
  DRX_CTCH_IRAT_FIT,
}drx_ctch_srch_type;

typedef enum
{
  DRX_NO_PAGE,
  DRX_FALSE_PAGE,
  DRX_TRUE_PAGE
}drx_page_ind_enum_type;


/* Database to store the variables/params for Dual Receive DS DS specific
   data.*/
#define DRX_MAX_SRCH_MEAS_FAILED_BAND_INFO_IDX (WRM_MAX_BANDS - 1)
#define DRX_MEAS_FAILED_BAND_INFO_INVALID 0xFF
#define PICH_STATS_WORDS                      32

typedef struct
{
  /* Consecutive PI SUM 0*/
  uint8 num_cons_pi_sum_zero;

  /* Consecutive reacq failures*/
  uint8 num_cons_reacq_fail;
  
  /* Consecutive good pi decodes*/
  uint8 num_cons_pi_decode_succ;

  /* Consecutive reaq passes */
  uint8 num_cons_reacq_pass;
 
  /* Last TRM params passed for reserve/request.*/
  wrm_freq_input_type trm_res_req_band_info;

  /*tracks the UE mode(SR, probe, DR)*/
  wsleep_wakeup_mode ue_mode;
  
  /* keeps track of what the current
     wakeup was for */ 
  boolean drx_current_wkup_was_pich;

  /* Keeps track of the consecutive lock fail cnt
     which is needed for transitions from SR to other modes*/
  uint16 consecutive_lock_fail_cnt;

} drx_dr_dsds_db_struct_type;

typedef struct
{
  /* data bases*/
  drx_dr_dsds_db_struct_type drx_dr_dsds_db;
/* Structure which carries the info of the enhanced CTCH algorithm */
  drx_enh_ctch_algo_struct_type *drx_ctch_enh_db_ptr;
  /* Holds all the parameters necessary for calculating parameters
     for PICH demodulation. */
  drx_parms_struct_type drx_parms;
  drx_parallel_reacq_db_type drx_parallel_reacq_db;
  drx_idle_mode_debug_nv_struct_type drx_idle_mode_debug_nv;
/* Structure for storing DRX PICH log packet info.*/
  drx_pich_log_pkt_struct_type drx_pich_log_pkt;
  /* Holds the actual SFN occasion and slot when the mDSP command
 * to demodulate the PICH has to be sent.
 */
  pich_demod_activation_time pich_page_occasion;
  /* Holds all the parameters necessary for Idle_RXD feature. */
  drx_rxd_pch_struct_type drx_rxd_pch;
  drx_rxd_bch_struct_type rxd_bch;

  #ifdef FEATURE_WCDMA_REACQLESS_PICH
  drx_rlp_db_type drx_rlp_db;
  #endif  

  drx_state_type drx_state;
/* To remember DRX state at the time of a SUSPEND, for use on RESUME. */  
  drx_state_type drx_state_at_suspend;
/* abort flags */
  boolean drx_sleep_aborted;
  boolean drx_delay_sleep_abort;  
/*timeline related */
  int32 ticks_until_pi;
  boolean drx_offline_pich_this_cycle;
  boolean po_overrun_this_cycle;

  #ifdef FEATURE_DUAL_SIM
  boolean wtow_bplmn_in_progress;
  #endif
  boolean wtog_bplmn_in_progress;

  #if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
  /* Variable to store the previous W2G BPLMN state before going to sleep */
  boolean prev_wtog_bplmn_in_progress;
  #endif  

  drx_opts_svc_action_type drx_opts_svc_action;
  /* drx command saved inside drx_cmd() for use when drx is restarted
   initializing it to an arbitary length.*/
  l1_drx_cmd_type drx_cmd_saved;
  boolean drx_enable_rgs_update;    
  rex_timer_type drx_qxt_interrupt_delayed_timer;
  drx_page_ind_enum_type drx_page_ind_received;
  boolean drx_no_rsp_to_page;
  boolean drx_needs_fast_sccpch_suspend;  
  boolean drx_pch_delivery_enabled;
  /*Used to set a timer which crashes on expiry if drx state is stuck.*/
  uint32 drx_delayed_timer_ms;  
  boolean drx_disable_auto_sccpch;
  boolean debug_wl1sleep_disable;
  #ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
  boolean l1_ff_cmd_recevd;
  #endif
  boolean dl_offline_pich_sccpch_auto_resume;
  uint32 stmr_ref_count;  
  boolean drx_finger_assgn_at_sleep_chk;
  /* variable indicates if we need to check for SFN mismatch after CR while
   handling DRX cmd */
  boolean sfn_mismatch_check_needed;
  /* Define the rex timer.*/
  rex_timer_type drx_sleep_timer_1;

  /* Sleep time,r SFN and combiner countervalue after cell reselection */
  uint32 cc_aft_CR;
  uint32 sleep_timer_aft_CR;
  uint32 sfn_aft_CR;
  /* Copies of counters, so we can simulate a "reset" of these variables
** without modifying the originals. */
  uint32 reset_rrcpg1_num_pages;
  boolean drx_nudging_rrc;
  uint32 reset_rrcpg1_num_pages_matched;

/*lock declarations */
  rex_crit_sect_type drx_evt_mgr_lock;

  boolean meas_req_and_notify_counter;

  boolean drx_alt_sub_in_idle;

  boolean ops_allowed;

  /*Timeout upon which DL WEAK IND is sent to RRC. Typically 3sec.*/
  uint16 drx_cell_selection_no_lock_timeout;

  boolean unable_to_setup_pich;

}drx_ext_cntrl_params_type;

extern drx_ext_cntrl_params_type drx_ext_cntrl_params[WCDMA_NUM_SUBS];

#define DRX_SLP_ABORT_PENDING_TO_BE_HANDLED()  (drx_ext_cntrl_params[wsub_id].drx_sleep_aborted | drx_ext_cntrl_params[wsub_id].drx_delay_sleep_abort | wl1sleep_ext_cntrl_params[wsub_id].sleep_drv_force_abort)

#define DRX_RESET_SLP_ABORT_PENDING_VARS() \
do { \
   drx_ext_cntrl_params[wsub_id].drx_sleep_aborted = FALSE; \
   drx_ext_cntrl_params[wsub_id].drx_delay_sleep_abort = FALSE; \
}while(0)
#define WSLEEP_RSSI_THRESHOLD			 0
#define WSLEEP_CPICH_ECIO_THRESHOLD 	         0
#define WSLEEP_RSCP_THRESHOLD			 0

#define DRX_CLR_RES_REQ_BAND_INFO(band_info_ptr)            (memset(band_info_ptr, 0x0, sizeof(wrm_freq_input_type)))
#define DRX_NUM_CONT_REACQ_FAILED()                         (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_reacq_fail)
#define DRX_NUM_CONT_PISUM_ZERO()                           (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_pi_sum_zero)
#define DRX_DR_TO_SR_FALLBACK_COND_MET()                    ((((DRX_NUM_CONT_PISUM_ZERO() >= DRX_NUM_CONTINUOUS_PISUM_SWITCH_THRESH) || \
                                                              (DRX_NUM_CONT_REACQ_FAILED() >= DRX_NUM_CONTINUOUS_REACQ_SWITCH_THRESH)) && \
                                                              (aset_cell_ptr->rx_agc <= WSLEEP_RSSI_THRESHOLD) \
                                                               ) ||  \
                                                              (aset_cell_ptr->filt_ecio < WSLEEP_CPICH_ECIO_THRESHOLD)|| \
                                                              (aset_cell_ptr->filt_rscp < WSLEEP_RSCP_THRESHOLD) \
                                                             )
#define DRX_CLR_CONS_REACQ_FAIL_CNT()       (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_reacq_fail = 0)
#define DRX_INC_CONS_REACQ_FAIL_CNT()       (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_reacq_fail++)
#define DRX_CLR_CONS_PI_SUM_ZERO_CNT()                      (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_pi_sum_zero = 0)
#define DRX_INC_CONS_PI_SUM_ZERO_CNT()      (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_pi_sum_zero++) 
#define DRX_NUM_CONTINUOUS_REACQ_SWITCH_THRESH              3
#define DRX_NUM_CONTINUOUS_PISUM_SWITCH_THRESH              3
#define DRX_PANIC_SRCH_COND_MET_FOR_CTCH() (drx_ext_cntrl_params[wsub_id].drx_ctch_enh_db_ptr->panic_srch_cond_met)        
#define DRX_NUM_CONT_PI_DECODE_PASS()                       (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_pi_decode_succ)  
#define DRX_NUM_CONT_REACQ_PASS()                           (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_reacq_pass)
#define DRX_CLR_CONS_REACQ_PASS_CNT()                       (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_reacq_pass = 0)
#define DRX_CLR_CONS_PI_SUM_PASS_CNT()                      (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_pi_decode_succ = 0)
#define DRX_INC_CONS_PI_SUM_PASS_CNT()                      (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_pi_decode_succ++)
#define DRX_INC_CONS_REACQ_PASS_CNT()                       (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_reacq_pass++)

#define DRX_UE_IN_PROBE_MODE()                              (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.ue_mode == WSLEEP_PROBE_MODE)
#define DRX_UE_IN_DR_MODE()                                 (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.ue_mode == WSLEEP_DR_MODE)
#define DRX_UE_IN_SR_MODE()                                 (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.ue_mode == WSLEEP_SR_MODE)

#define DRX_SET_UE_MODE(value)                              (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.ue_mode = value)   

#define DRX_INC_MEAS_REQ_AND_NOTIFY_COUNTER()               (drx_ext_cntrl_params[wsub_id].meas_req_and_notify_counter ++)
#define DRX_RESET_MEAS_REQ_AND_NOTIFY_COUNTER()             (drx_ext_cntrl_params[wsub_id].meas_req_and_notify_counter = 0)
#define DRX_GET_MEAS_REQ_AND_NOTIFY_COUNTER()               (drx_ext_cntrl_params[wsub_id].meas_req_and_notify_counter)
#define DRX_SET_CURRENT_WAKEUP_AS_PICH(value)               (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.drx_current_wkup_was_pich = value)
#define DRX_CURRENT_WAKEUP_WAS_PICH()                       (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.drx_current_wkup_was_pich) 
#define DRX_GET_CONS_LOCK_FAIL_CNT()                        (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.consecutive_lock_fail_cnt)
#define DRX_INC_CONS_LOCK_FAIL_CNT()                        (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.consecutive_lock_fail_cnt++)
#define DRX_CLR_CONS_LOCK_FAIL_CNT()                        (drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.consecutive_lock_fail_cnt = 0)

/* condition 1: REACQ FAIL + LOCK FAIL */
#define DRX_SR_DR_TRANS_COND_1()      (DRX_NUM_CONT_REACQ_FAILED() + DRX_GET_CONS_LOCK_FAIL_CNT())

/* condition 2: REACQ PASS + LOCK FAIL */
#define DRX_SR_DR_TRANS_COND_2()      (DRX_NUM_CONT_REACQ_PASS() + DRX_GET_CONS_LOCK_FAIL_CNT())

/* condition 3: PISUM = 0 + LOCK FAIL */
#define DRX_SR_DR_TRANS_COND_3()      (DRX_NUM_CONT_PISUM_ZERO() + DRX_GET_CONS_LOCK_FAIL_CNT())

/* condition 4: PISUM != 0 + LOCK FAIL */
#define DRX_SR_DR_TRANS_COND_4()      (DRX_NUM_CONT_PI_DECODE_PASS()+ DRX_GET_CONS_LOCK_FAIL_CNT())

#define DRX_CHK_STATE(drx_state_chk) (drx_ext_cntrl_params[wsub_id].drx_state == drx_state_chk)
#define DRX_ALT_SUB_IN_IDLE(val)     (drx_ext_cntrl_params[wsub_id].drx_alt_sub_in_idle = val)
#define DRX_IS_ALT_SUB_IN_IDLE()     (drx_ext_cntrl_params[wsub_id].drx_alt_sub_in_idle) 

extern void drx_opts_svc_handler( wsub_id_e_type wsub_id);

#define DRX_CLR_PASS_FAIL_COUNTERS()   \
do{\
    drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_reacq_fail = 0;   \
    drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_pi_sum_zero = 0;  \
    drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_reacq_pass = 0;   \
    drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.num_cons_pi_decode_succ = 0;\
    drx_ext_cntrl_params[wsub_id].drx_dr_dsds_db.consecutive_lock_fail_cnt = 0;\
}while(0)

#define DRX_SET_RGS_UPDATE_STATUS(value) (drx_ext_cntrl_params[wsub_id].drx_enable_rgs_update = value)
#define DRX_GET_RGS_UPDATE_STATUS()      (drx_ext_cntrl_params[wsub_id].drx_enable_rgs_update)

#define DRX_IGNORE_BPLMN_SLP_ABORT_IND() ((ext_wl1m_cntrl->l1m_state != L1M_PCH_SLEEP) || \
                                          ((WL1_GET_L1M_STATE() == L1M_PCH_SLEEP) && !drx_entity_is_po_far_away(DRX_SLEEP_THRESHOLD_FOR_BPLMN_SLEEP_ABORT_IND, wsub_id)) || \
                                          WL1SLEEP_IS_MEAS_GAP_ALLOWED()     \
                                         )

/* Min value for NBCH lock time out will be 640ms - 180ms = 460ms 
   for the rest of the drx cycle length values it will be drx cycle length - 180ms.*/
extern const uint16 drx_lock_timeout_for_nbch[DRX_CYCLE_MAX + 1];
/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION     DRX_ENSURE_RF_IS_AWAKE

DESCRIPTION
  This function is called to ensure RF is awake
  
PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If RF is not awake, DRX will wake up RF

===========================================================================*/
extern void drx_ensure_rf_is_awake( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_CMD

DESCRIPTION
  This function is called by L1M when RRC commands L1 to begin DRX
  operation. This function calculates the Paging Occasion and the
  Paging Indicator (PI) value and calls the function to calculate
  the parameters that are needed to send to the mDSP to demodulate
  the PICH.

DEPENDENCIES
  None

RETURN VALUE
  True if all went well; False otherwise.

SIDE EFFECTS
  The global variable drx_ext_cntrl_params[wsub_id].drx_parms is updated with the computed values.

===========================================================================*/
extern boolean drx_cmd( l1_drx_cmd_type* cmd , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_CMD_FACH

DESCRIPTION
  This function validates the DRX command received from RRC during FACH
  and then saves the DRX Cycle Length.

DEPENDENCIES
  None

RETURN VALUE
   BOOLEAN, true if validation succeeds, false otherwise.

SIDE EFFECTS
  The global variable drx_ext_cntrl_params[wsub_id].drx_parms.drx_cycle_len is updated.

===========================================================================*/
extern boolean drx_cmd_fach( l1_drx_cmd_type* cmd );

/*===========================================================================

FUNCTION     DRX_RESTART

DESCRIPTION
  Restarts the DRX state machine with previous DRX command parameters.

DEPENDENCIES
  None

RETURN VALUE
   None

SIDE EFFECTS


===========================================================================*/
extern void drx_restart( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_REL

DESCRIPTION
  This function is called to stop drx operation

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_rel( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       DRX_CALC_PI_OFFSET_FROM_PICH

DESCRIPTION
  This function calculates the pnCount (in units of 256 chips) in a frame
  where the UE is to begin demodulating the PICH.  Range 0...143.
  It calculates this PI position for the SFN passed in by the caller,
  and returns the result to the caller.

DEPENDENCIES
  None

RETURN VALUE
  uint8 - Calculated PI PN position (Range 0 - 143).

SIDE EFFECTS
  None

===========================================================================*/
extern uint8 drx_calc_pi_offset_from_pich(uint16 pag_occ, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       DRX_CALC_NEXT_PAGING_OCCASION

DESCRIPTION
  This function calculates the next paging occasion. The value calculated is 
  set in drx_ext_cntrl_params[wsub_id].drx_parms.next_paging_occasion

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void drx_calc_next_paging_occasion(boolean is_wakeup_cancel, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       DRX_COMPUTE_MDSP_PARMS

DESCRIPTION
  This function calculates the parameters that are needed by the mDSP
  for PICH demodulation:
  piThreshold
  bitsPerPi
  piPnCount
  
  It also calculates the actual SFN and the slot when the mDSP command
  to demodulate the PI bits has to be sent.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The global variable drx_ext_cntrl_params[wsub_id].drx_parms is updated with the calculated values.

===========================================================================*/
extern boolean drx_compute_mdsp_parms(boolean is_wakeup_cancel, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       DRX_PICH_DONE_ISR

DESCRIPTION
  This ISR is called by tramp services when the MDSP_INT5 occurs.
  This function will read the piValue register from the mDSP to ascertain
  whether the PICH was successfully demodulated and if there was a page
  for the UE.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If the UE was paged, the ISR will be de-registered.
  If the UE was not pages, the mDSP will be commanded to again demodulate
  the PICH after the next drx cycle.

===========================================================================*/
extern void drx_pich_done_isr( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       DRX_SEND_MDSP_CMDS_FOR_PICH_SETUP

DESCRIPTION
  This function causes the required mDSP commands to set up the demodulation
  of PICH.  It is possible for this function to get called with PICH suspended
  or even dropped (not registered with DRX Manager).  When that happens, don't
  demodulate PICH.  This should be pretty rare, but can happen.

DEPENDENCIES
  None

RETURN VALUE
  TRUE - PICH demod commands sent to mDSP.
  FALSE - Commands NOT send to mDSP. No PICH at this time.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean drx_send_mdsp_cmds_for_pich_setup( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       DRX_LOG_INFO

DESCRIPTION
  This function logs the drx info collected during the last PICH done
  mdsp interrupt. This function is called in task context.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void drx_log_info( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_WAKE_UP

DESCRIPTION
  This function is called to prepare L1 for wakeup

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_wake_up( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       DRX_SLEEP_ABORT
DESCRIPTION
  This function initiates the abort of sleep mode if already in progress

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void drx_sleep_abort_for_sub(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_SLEEP_ABORT_SUB0

DESCRIPTION
  This function is called by QCHAT; so it calls the master function with default SUB

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_sleep_abort(void);

/*===========================================================================
FUNCTION     DRX_IS_AWAKE_FOR_TCXO_SETTLING

DESCRIPTION
  This function returns whether or not we are intentionally staying awake
  during DRX cycles to allow VCTCXO to settle.  This gives the mDSP a chance
  to adjust trk_lo_adj based upon downlink pilot and drive the VCTCXO
  frequency to a more accurate value.  We do this for approximately 10
  seconds after power up.  This is done to help GSM, in the event that it
  doesn't have rotator support.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE  - We are allowing VCTCXO to track.
  FALSE - Tracking is complete.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean drx_is_awake_for_tcxo_settling( wsub_id_e_type wsub_id);

#ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
/*===========================================================================

FUNCTION     DRX_CHK_L1_GO_TO_SLEEP

DESCRIPTION
  This function calls the RRC query function and checks whether a WTOW BPLMN
  scan needs to be intiated.  It also checks whether a WTOG BPLMN search is
  active and monitors the GSM CLK. If the GSM clock is enabled and there are
  no GSM neighbors and the WTOG BPLMN search is not in progress then turn off
  the GSM clock.  The RRC query function returns a FALSE if a WTOW BPLMN
  search can be performed and returns TRUE if no search can be performed or a
  WTOG search can be performed.
  
DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE:  Either no WTOW search or WTOG BPLMN search.
          \ FALSE: WTOW BPLMN search can be started.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean drx_chk_l1_go_to_sleep(uint32 ms_to_sleep, wsub_id_e_type wsub_id);
#endif


#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
/*===========================================================================
FUNCTION     l1_rrc_can_l1_go_to_sleep

DESCRIPTION       This function is called by L1 in every DRX cycle to determine
                  if L1 should go to sleep or not. This function call was 
                  introduced due to WTOW and WTOG BPLMN search. If WTOW search needs to be
                  done during the DRX sleep time, then L1 should NOT go to sleep. Else
                  if WTOG search needs to be done then L1 should go to sleep.

DEPENDENCIES      NONE

RETURN VALUE      
                  TRUE:  L1 can go to sleep.
                  FALSE: Otherwise
                                  
SIDE EFFECTS
===========================================================================*/

boolean l1_rrc_can_l1_go_to_sleep(  uint32 timer_val,
  boolean *wtog_bplmn_in_progress
#ifdef FEATURE_DUAL_SIM_BPLMN
  ,boolean *wtow_bplmn_in_progress
#endif
  ,wsub_id_e_type wsub_id
);


/*===========================================================================

FUNCTION l1_rrcpg1_modify_sibs_for_sfn_update

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

boolean l1_rrcpg1_modify_sibs_for_sfn_update
( wsub_id_e_type wsub_id);
#endif/*FEATURE_WCDMA_FREE_FLOATING_TASK*/

/*===========================================================================

FUNCTION     DRX_PROCESS_COMMAND

DESCRIPTION
  This function implements the state machine of DRX/Sleep. It processes the
  local commands of type DRX_SLEEP_CMD.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_process_command(const drx_sleep_cmd_type * cmd, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION     DRX_PICH_DEMOD_EVENT_ENABLED_TOO_LATE

DESCRIPTION
  This function is called when it is detected that the pich demod
  event wasn't enabled in time. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_pich_demod_enabled_too_late( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_BCH_SETUP

DESCRIPTION
  This function is called when a BCH is setup. It sets a flag in the drx module.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_bch_setup( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_BCH_TEAR_DOWN

DESCRIPTION
  This function is called when BCH is torn down. It resets a flag in the drx module.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_bch_tear_down( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_SLEEP_PERIOD_COMPLETE

DESCRIPTION
  This function is called when the sleep period is over. It calls 
  a function in the sleep module to wake up.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_sleep_period_complete( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_SET/CLR_SLEEP_PERIOD_COMPLETE_SIG, 
             DRX_SET/CLR_COMBINER_COUNT_SLAMMED_SIG
             DRX_WAIT_FOR_COMB_CNT_SLAM
             DRX_DEFINE_WAKE_UP_TIMER

DESCRIPTION
  These functions are called to send a specific signal to L1 (drx is merely 
  acting like a conduit between sleep and L1 here).
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_define_wake_up_timer(rex_timer_type * ptimer, wsub_id_e_type wsub_id);

extern void drx_nudge_rrc( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_BCH_ON

DESCRIPTION
  This function returns true if BCH was setup and FALSE otherwise.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern boolean drx_bch_on( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_DELAYED

DESCRIPTION
  This function handles the recovery from DRX getting stuck for an extended period.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_delayed_cb(timer_cb_data_type cb_data);

/*===========================================================================

FUNCTION     DRX_SLEEP_TIMER_1_CLR

DESCRIPTION
  This function is called to clear timer 1 service provided by the sleep controller.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_sleep_timer_1_clr( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       DRX_OK_FOR_CONCURRENT_PCCPCH

DESCRIPTION
  This function determines if the DRX module is OK with DL going for a
  concurrent pccpch setup.

DEPENDENCIES
  None

RETURN VALUE
  TRUE, if DRX is OK for a concurrent pccpch setup. Else, False.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean drx_ok_for_concurrent_pccpch( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     GET_SLEEP_MS

DESCRIPTION
  This function is called to compute the number of milliseconds for which 
  to sleep
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern uint32 get_sleep_ms( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     NEXT_DRX

DESCRIPTION
  This function is called to kick in the next DRX cycle

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void next_drx(boolean reacq_failed, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SET_DRX_STATE

DESCRIPTION
  This function is called to change the state of the DRX module.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void set_drx_state(drx_state_type newState, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_QXDM_DIPSW_DISABLES_SLEEP

DESCRIPTION
  This function checks the QXDM DIP switch settings to see if the user has
  disabled sleep mode operation.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE if sleep is disabled via QXDM DIP switch setting.  FALSE otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean drx_qxdm_dipsw_disables_sleep( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_SNAPSHOT_SFN_BEF_CELL_RESEL

DESCRIPTION
This function is called after a cell reselection is triggered and NBCH is
setup and SFN-SFN diff, Noff computation for neighbor cell are done. This
function takes a snapshot of neighbor cell's timing (SFN,CC) and sleep clock
snapshot. This is used for comparison after cell reselection to detect any
timing errors during cell reseleciton. Timing is computed w.r.t target cell

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
extern void drx_snapshot_sfn_bef_cell_resel
(
  tlm_cell_info_struct_type neighbor_cell_info
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_SNAPSHOT_SFN_AFTER_CELL_RESEL

DESCRIPTION
This function is called after a cell reselection has completed, and we have
aligned our timing to the new cell.  It snapshots the SFN/chipx1 of the
new serving cell, and also snapshots the sleep clock.  Once we enter DRX
mode on the new cell, we compare reselection durations measured by STMR and
by sleep clock.  If they are too far off, our timing may be off, so we
nudge RRC to recover.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
extern void drx_snapshot_sfn_after_cell_resel
(
  tlm_cell_info_struct_type serving_cell_info
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_NUDGE_SUPPORT_AFTER_PCCPCH_SETUP

DESCRIPTION
This function gets called when both PCCPCH_S and PCCPCH_N setup completes,
and CRC's are either passing or failing.  There is some processing that can
happen regarding RRC nudge at this time.

If it's PCCPCH_N, and we're reselecting, now is a good time to snapshot
neighbor timing, so we have a reference after the reselection completes.

If it's PCCPCH_S, because we nudged RRC earlier, we didn't send SETUP_CNF
before, so let's send it now.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void drx_nudge_support_after_pccpch_setup
(
  tlm_cell_info_struct_type cell_info
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_CHECK_FOR_RRC_NUDGE

DESCRIPTION
This function gets called at sleep time.  We check if we want to nudge RRC.
Nudging RRC means telling RRC to bring up PCCPCH_S, so we can reestablish
timing of the network.  We do this when UE timing might be off, like when
the duration of a cell reselection seems questionable.  We also do it every
DRX_NUDGE_RRC_TO_READ_MIB_SEC seconds, just to make sure our timing is OK.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
We might nudge RRC to bring up PCCPCH_S.
===========================================================================*/
extern void drx_check_for_rrc_nudge( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
extern void l1_rrc_cphy_nudge_for_sfn_miss_match_rsp_handler
(
boolean status
, wsub_id_e_type wsub_id);
#endif /*FEATURE_WCDMA_FREE_FLOATING_TASK*/


/*===========================================================================

FUNCTION       DRX_RECOVER_STMR

DESCRIPTION
  This function resets and takes the UE oos if STMR was stuck.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void drx_recover_stmr( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       drx_is_ue_just_paged

DESCRIPTION
  This function returns the status of the UE page for this page cycle.

DEPENDENCIES
  None

RETURN VALUE
  TRUE - if UE got paged. 
  FALSE- if UE is not paged.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean drx_is_ue_just_paged( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       DRX_SLEEP_TIMER_1_ISR
DESCRIPTION
  This function is the isr for the sleep timer 1 done interrupt.
  If this fires, it implies that RRC hasn't responded to a PICH-page.
  If in the right state, it posts a local command that kicks sleep back in.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void drx_sleep_timer_1_cb(timer_cb_data_type cb_data);

/*===========================================================================
FUNCTION   DRX_RXD_PCH_SET_ENABLE 

DESCRIPTION
  This function sets the value of drx_ext_cntrl_params[wsub_id].drx_rxd_pch.idle_rxd_enabled

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void drx_rxd_pch_set_enable(boolean state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION   DRX_RXD_PCH_GET_NEXT_MODE 

DESCRIPTION
  This function returns the next mode in RXD IDLE.

DEPENDENCIES
  None

RETURN VALUE
  RXD_PCH_NO_DIV (or) RXD_PCH_FAST_DIV (or) RXD_PCH_FULL_DIV

SIDE EFFECTS
  None

===========================================================================*/
rxd_pch_mode_enum_type drx_rxd_pch_get_next_mode( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION   DRX_RXD_PCH_RESET_DB 

DESCRIPTION
  This function clears the entire DRX_RXD_PCH structure.
  This function is called upon receiving DRX_CMD and also upon every wakeup

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void drx_rxd_pch_reset_db(boolean clear_all, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_PRLL_REACQ_CURR_CYCLE

DESCRIPTION
  This function returns if we are in parallel reacq this cycle.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern boolean drx_prll_reacq_curr_cycle( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     L1M_MDSP_AGC_TUNE_DONE

DESCRIPTION
  Handler for the AGC tuning done interrupt from MDSP.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void l1m_mdsp_agc_tune_done( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_PICH_INTERRUPT_DELAYED

DESCRIPTION
   CB function for pich demod delayed timer

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void drx_pich_interrupt_delayed(uint32 unused);

extern void drx_offline_pich_adjust_outer_loop(void);

extern int32 pich_stats_adjust_drx_ticks( wsub_id_e_type wsub_id);

extern void drx_calc_pi_bit_time_for_offline_pich( wsub_id_e_type wsub_id);

extern void drx_pich_phychan_setup_ops(drx_pich_processing_mode_type mode, wsub_id_e_type wsub_id);

extern void drx_offline_pich_setup( wsub_id_e_type wsub_id);

extern void drx_ss_mgr_event_notify(drx_ssmgr_notify_evt_type drx_ss_mgr_notification_event,drx_notification_post_cb_type post_cb, wsub_id_e_type wsub_id);

extern boolean drx_get_fast_sccpch_suspend( wsub_id_e_type wsub_id);

extern void drx_set_fast_sccpch_suspend(boolean config, wsub_id_e_type wsub_id);

extern void drx_sccpch_start_event_done_handler( wsub_id_e_type wsub_id);

extern void drx_rf_wakeup_with_agc_done(boolean wait_for_agc_done, wsub_id_e_type wsub_id);

extern void drx_qxt_interrupt_delayed(timer_cb_data_type cb_data);

extern boolean drx_set_pich_entity_srch_to_sleep_pending(drx_pich_entity_srch_to_sleep_cb_type cb, wsub_id_e_type wsub_id);

extern uint32 drx_get_pending_notf_bmsk( wsub_id_e_type wsub_id);

extern boolean drx_prll_reacq_this_cycle( wsub_id_e_type wsub_id);

extern void drx_pich_opts_init( wsub_id_e_type wsub_id);

extern boolean drx_chk_srch_to_slp_status( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_GET_TIMESTAMP_FOR_LOG_PKT

DESCRIPTION
  This function computes the timestamp in the form 
  Bits 0:18 subframe cx8, bits 19:30: frame

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern uint32 drx_get_timestamp_for_log_pkt( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_PICH_LOG_INIT

DESCRIPTION
  This function initializes the content of the PICH log pkt.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void drx_pich_log_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_PICH_LOG_FREQ_ERR

DESCRIPTION
  This function logs the frequency error for PICH log pkt.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void drx_pich_log_freq_err( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_PICH_LOG_PKT_SUBMIT

DESCRIPTION
  This function submits the PICH log pkt if it can allocate a buffer.

DEPENDENCIES
  Prints only after atleast 1 DRX cycle with true sleep happens.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void drx_pich_log_pkt_submit( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_PICH_LOG_PKT_READ_FW_PARMS

DESCRIPTION
  This function reads the params populated by FW for the PICH log pkt.

DEPENDENCIES
  FW should have populated the correct values.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void drx_pich_log_pkt_read_fw_params( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_PICH_LOG_FING_STATS

DESCRIPTION
  This function logs the fing stats to be printed in the PICH log pkt.
  1. Fing RSSI
  2. TCPFSQ0 value
  3. TCPFSQ2 value/

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void drx_pich_log_fing_stats( wsub_id_e_type wsub_id);
extern void drx_ss_mgr_event_done_notify_local_cmd_handler(drx_ssmgr_notify_evt_type drx_ss_mgr_notification_event, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_RF_WARMED_UP

DESCRIPTION
  This function is used as a call back function thats called when RF has warmed up,
  after a sleep period has expired. It posts a DRX local command.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_rf_warmed_up( wsub_id_e_type wsub_id);

uint8 drx_check_gaps_between_ctchs( wsub_id_e_type wsub_id);
void drx_block_srch_for_ctch(uint32 t_irat, uint32 t_ifreq, wsub_id_e_type wsub_id);
void drx_compute_srches_fits_in_gaps(boolean skip_ctch_sfns, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_INIT_ENH_CTCH_ALGO_INFO

DESCRIPTION
 This func resets all the CTCH info calcualted for the previous PICH wakeup.
 This func should be called when CTCH algo needs to run for searches.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE 
===========================================================================*/
extern void drx_init_enh_ctch_algo_info( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_QUERY_BLOCK_SRCH_FOR_CTCH_DECISION

DESCRIPTION
 This func returns if searches are to be blocked for CTCH.
 If CTCH entity is awake, searches should be blocked.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE 
TRUE -- Allow searches 
FALSE -- Block searches 

===========================================================================*/
boolean drx_allow_srch_with_active_ctch_schd( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_QUERY_TO_EVAL_ENH_CTCH_ALGO

DESCRIPTION
 This func returns the decision to evluate the enhanced CTCH algo.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE 
TRUE -- Evaluate the algo 
FALSE -- Do not evaluate the algo

===========================================================================*/
boolean drx_query_to_eval_enh_ctch_algo(void);

/*===========================================================================
FUNCTION     DRX_PRIORITIZE_SRCH_OVER_CTCH

DESCRIPTION
 The function sets the flag to drop CTCH for searches. Flag is usually set in
 case of serving cell ecio lesser than the threshold/S-criteria failure/Serving cell barred

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE 
 None
===========================================================================*/
void drx_prioritize_srch_over_ctch( wsub_id_e_type wsub_id);
extern boolean drx_query_block_srch_for_ctch_decision( wsub_id_e_type wsub_id);

#ifdef FEATURE_DUAL_SIM
/*===========================================================================
FUNCTION     DRX_GET_NULL_DRX_CYCLE

DESCRIPTION
  This function returns drx_int_cntrl_params[wsub_id].drx_is_null_drx_cycle varaiable.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern boolean drx_get_null_drx_cycle( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_SET_NULL_DRX_CYCLE

DESCRIPTION
  This function sets drx_int_cntrl_params[wsub_id].drx_is_null_drx_cycle varaiable to the 
  value of the argument passed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_set_null_drx_cycle(boolean value, wsub_id_e_type wsub_id);
#endif

/*===========================================================================

FUNCTION     DRX_CALC_MDSP_PARMS_WITH_NEXT_PO

DESCRIPTION
  This function calculates all MDSP parms for the next paging occasion after we know the
  next paging occasion.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_calc_mdsp_parms_with_next_po( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_QUERY_BLOCK_SRCH_FOR_CTCH_DECISION

DESCRIPTION
  This function is called before Ifreq or Irat search is issued. 

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE 
 TRUE - block search
 FASLE - don't block search
===========================================================================*/
extern boolean drx_query_block_srch_for_ctch_decision( wsub_id_e_type wsub_id);

/*=========================================================================== 
FUNCTION DRX_CALCULATE_GAPS_BETWEEN_CTCH 
 
DESCRIPTION 
This function calculates the gap in frames between CTCH sfn's. It considers 
the setup and drop time for CTCH while calculating the gaps 
 
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE 
 returns the Gap in frames
 
===========================================================================*/
uint16 drx_calculate_gaps_between_ctch(boolean loop_selector, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_CALL_CAN_L1_GO_TO_SLEEP

DESCRIPTION
  This function will be called while L1 is going to sleep, This
  calls RRC's api so that they can start/abort BPLMN if required.

DEPENDENCIES
  None

RETURN VALUE
  Boolean can_l1_sleep  =  TRUE  - L1 can go to sleep
                           FALSE - L1 shouldnt go to sleep but start BPLMN

PARAMETERS
  ms_to_sleep - Time L1 has to sleep.
   
SIDE EFFECTS

===========================================================================*/
extern boolean drx_call_can_l1_go_to_sleep(uint32 ms_to_sleep, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION DRX_READ_IDLE_MODE_DEBUG_NV

DESCRIPTION
      NV used for enable/disable debugging in idle mode.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
extern void drx_read_idle_mode_debug_nv( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_UPDATE_DRX_DELAYED_TIMER_VAL

DESCRIPTION
  This function is called whenever we get a drx_cmd to update the
  value of drx_delayed debug timer.
 
PARAMETERS 
  uint16 drx_cycle_len      - DRX cycle length.
  boolean wake_up_for_ctch  - TRUE : non PICH entitiy is waking up next.
                              FASLE: PICH entity will wakeup next.
  int32 sleep_ms            - Is valid only if wake_up_for_ctch is TRUE.
                              provides sleep time for non PICH entity.
RETURN VALUE
  None
 
SIDE EFFECTS
  Global variable drx_ext_cntrl_params[wsub_id].drx_delayed_timer_ms is updated with computed values.

===========================================================================*/
extern void drx_update_drx_delayed_timer_val(uint16 drx_cycle_len,boolean wake_up_for_ctch,int32 sleep_ms, wsub_id_e_type wsub_id);

/*=========================================================================== 
FUNCTION DRX_PREP_FOR_CALL_EST
 
DESCRIPTION 
  This function informs DRX module to prepare for call establishment.
 
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE 
  None.
 
===========================================================================*/
extern void drx_prep_for_call_est( wsub_id_e_type wsub_id);

/*=========================================================================== 
FUNCTION DRX_HANDLE_PREP_FOR_LOCK_REL_CALL_EST
 
DESCRIPTION 
  This function prepares the DRX module to prepare for all its activities
  to quickly release the lock before call setup.
 
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE 
  None.
 
===========================================================================*/
extern void drx_handle_prep_for_lock_rel_call_est( wsub_id_e_type wsub_id);

/*=========================================================================== 
FUNCTION DRX_CHECK_WAITING_ON_CALL_EST
 
DESCRIPTION 
  This DRX function checks if we are waiting on a WRM PENDING CMD from RRC on 
  call establishment.
 
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE 
  None.
 
===========================================================================*/
extern boolean drx_check_waiting_on_call_est( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION DRX_GET_CURR_RXD_MODE

DESCRIPTION
    This function returns current RXD_State after wakeup.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
rxd_pch_mode_enum_type drx_get_curr_rxd_mode( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION DRX_GET_PREV_RXD_MODE

DESCRIPTION
  This function returns prev RXD_State during sleep. (Used for parallel Reacq case) 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
rxd_pch_mode_enum_type drx_get_prev_rxd_mode( wsub_id_e_type wsub_id);

#ifdef FEATURE_DUAL_SIM
/*===========================================================================
FUNCTION    drx_trigger_int_drx_cmd

DESCRIPTION
  Checks if L1 can post a DRX to itself. 
  It happens only when we are in PCH but have a pending req and notify from RRC even before a DRX cmd is received.

DEPENDENCIES
  None.

PARAMETERS
  none

RETURN VALUE
  none
===========================================================================*/
void drx_trigger_int_drx_cmd( wsub_id_e_type wsub_id);
#endif


#ifdef FEATURE_WCDMA_QSH_DUMP
extern void wl1_drx_sleep_dump_cb(void* addr_ptr);
#endif

#ifdef FEATURE_WCDMA_REACQLESS_PICH
/*===========================================================================
FUNCTION drx_configure_num_pi_bits_in_rlp

DESCRIPTION
    This function configures the number of PI bits that need to be decoded. This might be 
    different from the actual PI bits transmitted since we want to do partial decoding whenever
    we do not cross the (num_pi_bits/2) at the time of AGC tune done in case of Offline PICH.
  
DEPENDENCIES
   This will be done when FEATURE_WCDMA_REACQ_LESS_PICH is defined.

RETURN VALUE
  None

SIDE EFFECTS
  PI decoding is done on less than the number of PI bits transmitted by the network.
===========================================================================*/
extern void drx_configure_num_pi_bits_in_rlp(uint8 *bits_per_pi_ind,uint16 *trans_time_offset, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION drx_update_num_pi_bits_in_rlp

DESCRIPTION
    This function updates the bits per PI for an Reacqless PICH cycle in the pointer 
    that is passed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void drx_update_num_pi_bits_in_rlp(uint8 *bits_per_pi_ind, wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION     DRX_GET_PARMS_FOR_PICH_LOG_PKT

DESCRIPTION
  API used to get FW, freq err and fing info to be populated for
  PICH log pkt later

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None
===========================================================================*/
extern void drx_get_parms_for_pich_log_pkt(wsub_id_e_type wsub_id);

/*===================================================================================
FUNCTION     DRX_PROCESS_OPS_AT_SLEEP_FAILURE

DESCRIPTION
  This function takes care of all resetting of variables/timers/states for handling sleep failure
  case.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
======================================================================================*/
extern void drx_process_ops_at_sleep_failure(wsub_id_e_type wsub_id);

#ifdef FEATURE_DUAL_SIM
/*===========================================================================
FUNCTION    drx_post_fake_drx

DESCRIPTION
  When an activity with TRM_ACCESS_URGENT requiring a device switch is sent by RRC 
  in PCH before DRX cmd, L1 has to post a DRX to itself first.This enable the release
  of the current device so that the pending req and notify from RRC can be handled.

DEPENDENCIES
  None.

PARAMETERS
  none

RETURN VALUE
  none
===========================================================================*/
extern void drx_post_fake_drx( wsub_id_e_type wsub_id);
#endif

extern void drx_int_cntrl_params_init( wsub_id_e_type wsub_id);
extern void drx_ext_cntrl_params_init( wsub_id_e_type wsub_id);
extern void drx_bch_init(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     PICH_STATS_INIT

DESCRIPTION
  This function is called to initialize/reset all the variables/data-structures
  needed for PICH STATS Buffering. If this function is used to reset only
  PICH_STATS Buffering related data and not local_drx_cntrl->outer_loop_adjust_target_by,
  use it with a FALSE parameter
  
PARAMETERS
  boolean init_outer_loop - TRUE if local_drx_cntrl->outer_loop_adjust_target_by
                            needs to be reset to 0

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void pich_stats_init(boolean init_outer_loop, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_RXD_PCH_UPDATE_WAKEUP_ENTITY

DESCRIPTION
  This function stores the wakeup entity in DRX RXD PCH DB.

DEPENDENCIES
  None.

PARAMETERS
  Handle waking up - drx_entity_type.

RETURN VALUE
  None.
===========================================================================*/
void drx_rxd_pch_update_wakeup_entity(drx_entity_type handle_waking_up,wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_OPS_ALLOWED

DESCRIPTION
  This function calls the MSIM arbiter function that will either allow/prevent
  offline PICH/FCDC based on the alt tech's FW state. It then stores this info.  

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None.
===========================================================================*/
extern boolean drx_ops_allowed(wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION DRX_ONE_TIME_INIT

DESCRIPTION
  One time init which will be done only at stack initialization.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void drx_one_time_init(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION DRX_UPDATE_DEBUG_INFO_FROM_QXDM_CMD

DESCRIPTION
  This function updates the debug info in to drx variables
  which will be used in some particular scenarios.
 
  For eg:
    no_lock_timeout_ms is used to change the DL WEAK IND timeout during cell selection.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void drx_update_debug_info_from_qxdm_cmd(uint8 no_lock_timeout_frames, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_POST_LOCAL_CMD

DESCRIPTION
  This function is called to post a local command of type "DRX_SLEEP_CMD"
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void drx_post_local_cmd
(  
  drx_sleep_cmd_sender_enum sender,
  boolean success, 
  wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION wl1_qsh_mdump_collect_drx_params

DESCRIPTION
  Wrapper

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_drx_params(wsub_id_e_type wsub_id);
#endif
#endif /* DRX_H */
