#ifndef SRCH_STM_H
#define SRCH_STM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                S R C H _ S T M     H E A D E R   F I L E

GENERAL DESCRIPTION
  This module contains header information for the search state machine

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2005 - 2017 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/srch/inc/srch_stm.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/06/17   eye     Move GPS ISR routine to SRCH task context
11/20/17   ab      1x2GTA : 0x18CD log packet changes.
11/20/17   ab      1x2GTA diag cmd changes
08/29/17   pk      Added TUNE_AWAY_APS_CMD
08/29/17   pk      Added RX_MOD_EXTEND_DURATION_CMD
08/03/16   pk      Added TUNE_AWAY_PROCEED_F internal cmd
05/26/16   sjo/pk  Release resources before registration
04/21/16   pk      Added IRAT_PILOT_MEAS_CFG_RSP_DEREG_CMD
03/29/16   ab      Added TUNE_AWAY_UNLOCK_CANCEL_CMD cmds
03/29/16   pk      Added TUNE_AWAY_UNLOCK_REQUIRED and
                   TUNE_AWAY_UNLOCK_IMMEDIATE cmds
03/03/16   pk      Added TUNE_AWAY_UNLOCK_TIMER_CMD
02/05/16   ab      1x2GTA Feature changes
12/01/15   jh      1x support for TRM pending response in Traffic
10/02/15   srk     DRX SRCH response deadlock design change and cleanup.
09/01/15   sst     Add method to swtich to BG_TRAFFIC during PS Calls
07/27/15   pk      Changes to ignore QTA strat cb, after wakeup cb from slpc
07/17/15   pk      QTA timeline optimization, do RF script building just after
                   QTA start call back from TRM
07/01/15   jh      WTR hopping changes
05/11/15   jh      RXTX split - FR 21976
04/08/15   srk     Add TTL control algorithm in SW.
02/20/15   ab      Enhanced Debug buffer for Idle State
12/15/14   ssh     Added L->1x measurement abort support.
10/08/14   jh      Added COEX_POWER_IND_CMD
08/08/14   as      Fix IRAT for Dual Receive
05/13/14   bph     Make callbacks registered with TRM asynchronous
01/16/14   pk      Added CD_RF_DIV_DISABLED_CMD & ACQ_ABORT_CMD
01/13/14   pap     Added OFFSET_CHG_CMD in order to sync with FW for SLEW/SLAM
                   view offset changes.
11/22/13   bph     Send PILOT MEAS CFG REQ to FW w/i correct window
10/11/13   cjb     Removed legacy SEARCH, MEAS, CLEANUP states from IRAT:1x2L
10/06/13   cjb     Switch to a common TMIED_SEARCH_MEAS API from LTE
08/19/13   cjb     Mainline FEATURE_1X_SRCH_ROT_ONLY_AFC and
                   FEATURE_1X_SRCH_AFC_SRL ( Removed VCTCXO support )
04/23/13   srk     Remove unused DIV_START_CMD
03/13/13   ab      Added WAKEUP_UPDATE_CMD for wakeup manager support
12/19/12   bb      Changes to define DIV_START_CMD
02/19/13   cjb     Changes to support LTE->1x along with new LTE CA feature.
10/16/12   bb      Changes to fix the race condition between aborting
                   system measurement and releasing resources
01/25/13   vks     Cleanup srch mdsp module
10/26/12   vks     Move cRxAntenna and afcXoMNDEnable control to FW.
10/02/12   adw/dkb Add on the fly zz2_2 support.
08/16/12   ab      Changes for Removing RX Manual Transition during Hard HO.
09/10/12   vks     Add support for rf antenna tuner feature.
07/17/12   cjb     Lock both chains on behalf of LTE before starting 1x2L meas.
04/12/12   srk     Added DIV_UPDATE_PRIORITY_CMD.
04/06/12   srk     Added internal commands for diversity.
04/05/12   vks     Add support to handle error in qpch offtl record process.
03/12/12   adw     Add online state specific RF grant and tune commands.
03/02/12   srk     Added DIV_AUTO_IS_1X_ADV_TRAFFIC_CMD for 1x adv traffic
02/23/12   adw     Move RF report timer to "common" block as also used in idle.
02/16/12   srk     Replaced FEATURE_LTE_TO_1X by FEATURE_MODEM_1X_IRAT_LTO1X.
02/01/12   mca     Added RF_REPORT_TIMER_CMD
01/24/12   sst     Implementation of RIN
01/18/12   adw     Add additional TRAM cmd for notifying of pending requests.
01/13/12   srk     Feature Cleanup: Mainlined Always ON features
11/02/11   vks     Add support for adding delay after the RF retune.
10/31/11   adw     Integrate TRAM support.
10/27/11   vks     Add support for scheduled RF disable for offline qpch.
10/24/11   jtm     Feature clean up.
10/13/11   cjb     Changes for IRAT:1x->LTE. Initial working version.
09/20/11   vks     Wait for AFLT release before proceeding with 1x rf/fw
                   release in CDMA state.
08/18/11   adw     Mainline 1x message router support.
08/12/11   adw     Support second chain test [Option-2] capability on NikeL.
08/08/11   cjb     Added IRAT measurement support for NikeL.
08/03/11   adw     Remove unused Rx STM command.
07/18/11   adw/vks Modifications to doze processing required for NikeL.
07/11/11   adw     Enhanced ZZ profiling.
06/30/11   adw     Removed unused exec wakeup command.
06/20/11   vks     Initial set of changes to get RX DIV working with RX STM.
06/16/11   adw     Added commands needed to get sleep going on NikeL.
04/28/11   sst     Added 1x to LTE commands
04/12/11   vks     Added cmd for RF release done.
03/25/11   cjb     LTO1X Conn meas: FTM changes to avoid delay in RSP to DIAG.
03/16/11   sst     Add IRAT_FTM_CMD
02/22/11   sst     Update to IRAT pilot measurements
02/11/11   adw     Added cmd for reporting jump failures in lto1x.
02/08/11   adw     Misc changes to support lto1x cleanup.
02/07/11   bb      Changes to define IRAT_START_RESEL_CMD command for
                   quick LTE to 1X reselection scenarios
02/01/11   sst     Refactor of srchlte_tt_sm.* to new irat directory
12/17/10   adw     Removed unneeded time transfer command.
12/16/10   adw     Added additional commands for srchlte time transfer.
11/05/10   vks     Define new commands for FTM MDSP enable/disable.
11/03/10   bb      Changes to support eCSFB feature
10/14/10   bb      Changes to support connected mode measurements for LTE to 1X
                   IRAT feature
09/22/10   bb      Added code changes for Neighbor Set triage feature
07/15/10   adw     Moved mDSP app load processing from NA to CDMA state.
07/02/10   adw     Added command for skipping freq track done in CDMA state.
05/11/10   vks     Wait for srch4 registration to be successful before starting
                   searches.
02/18/10   sst     Added Div's DIV_AGC_CK_CMD
11/09/09   vks     Added Message Router and Q6 FW support under feature
                   FEATURE_1X_SUPPORTS_MSGR.
10/30/09   bb      Added SRCH_SCLK_COMPLETE_CMD command
10/13/09   bb      Added support for "LTE to 1X IRAT Core"
                   feature implementation
10/05/09   sst     Removed Acq's TUNE_NOW_CMD and added CK_AGC_CMD
10/02/09   sst     SVDO RF & TRMv3 development (featurized)
09/25/09   adw     Added support for dynamic memory allocation.
04/10/09   tjc     Abort OFREQ searching if we cannot register srch4
04/01/09   adw     Categorized included header files and standardized format.
03/31/09   adw     Cleaned up incs and eliminated implicit includes.
03/25/09   adw     Include modem_1x_defs.h for 1x specific modem defines.
03/09/09   vks     Added DIV_AUTO_IS_DATA_CALL_CMD and removed
                   DIV_AUTO_SCH_ENABLED_CMD for DIV_AUTO
02/10/09   adw     Merge from 2h08 branch, based on VU_MODEM_1X_SRCH.13.00.01
12/01/08   adw     Commented out _v.h includes from subsystems outside 1x.
11/19/08   adw     Merge from main/latest, based on VU_MODEM_1X_SRCH.12.00.24
11/19/08   adw     Merge from CMI sandbox, based on VU_MODEM_1X_SRCH.12.00.13
11/04/08   adw     Added AFC_FREQ_TRACK_OFF_RTL_CMD for RTL.
08/28/08   adw     Cleaned up featurization and seperated feature dependencies.
08/18/08   adw     Added FEATURE_SRCH_HAS_NO_TRM to remove TRM for ULC.
07/18/08   adw     Renamed AFC_TCXO_ON/OFF to AFC_FREQ_TRACK_ON/OFF.
06/23/08   aps     Added ABORT_OFREQ_CMD
06/23/08   aps     Added RF_REQ_FAIL_TIMER_CMD
06/20/08   mca     Merge from //depot
04/29/08   sst     Revert previous checkin
04/25/08   sst     Added RF_REQ_FAIL_TIMER_CMD
03/17/08   pa      Added SRCH_STM_GET_BOOLEAN to extract boolean from payload
01/17/08   sst     Add REACQ_TIMER_EXPIRED_CMD
06/01/07   pa      Added AFC_XO_CMD
05/07/07   tjc     Add REQUEST_RF_INFO_CMD
05/01/07   tjc     Add FORCE_OFF, PAUSE and RESUME commands
04/26/07   aps     Change to prevent ofreq srch from happening in ISR context
03/09/07   pa      Renamed rotator push internal commands to be consistent.
02/15/07   rkc     Added SEND_PPM_LIST_UPDATE_CMD, removed
                   ENABLE_FREQ_EST_CMD and DISABLE_FREQ_EST_CMD,
02/15/07   tjc     Add an input to allow the diversity state machine to tell
                    us that the request has failed
02/13/07   pa      Added AFC_RPUSH_SAMPLE_TIMER_CMD and AFC_RPUSH_FLAG_CMD
01/23/07   rkc     Added MDSP_ENABLED_CMD and TCXO_OFF_DONE_CMD.
01/18/07   rkc     Add CGPS commands
01/09/07   sst     Merge from 11.02.02B branch
12/14/06   pa      Removed CLOSE_LOOP_CMD and renamed START_CLOSE_LOOP_CMD
11/30/06   tjc     Added new command to enable/disable FPC for RX diversity
                    autoswitching
11/15/06   aps     Added RX_TUNE_CMD
11/08/06   tjc     Added new command to enable/disable SCH for RX diversity
                    autoswitching
10/18/06   bn      Added new commands to support TT state machine
10/13/06   trc     Changes for new STM grammar
10/11/06   tjc     Add RX diversity autoswitching
09/07/06   tjc     Update the aset only after and abort has been performed if
                   we are not at the home band/chan
08/29/06   rkc     Added LOST_RF_RESOURCE_CMD.
08/21/06   rkc     Added srch_stm_print_debug_trace() and
                   srch_stm_print_int_cmd_q()
08/07/06   rkc     Added REQUEST_SYSTEM_RESTART_CMD.  Removed bail_cmd.
08/04/06   tjc     Remove all single point waits from the traffic state
08/03/06   tjc     Moved traffic signals to internal commmands
07/19/06   trc     Decoupling STM from SRCH
07/11/06   trc     STM interface changes
07/11/06   tjc     Implemented searcher peak module
06/06/06   pa      Added internal commands to support AFC SM.
05/31/06   tjc     Added govern timer cmd
03/22/06   kwo     Added FRAME_STROBE_CMD
01/16/06   awj     Moved to srch_stm interface
11/14/05   awj     Added inactive commands
10/18/05   awj     Lint cleanup
10/04/05   pa      Added ABORT_HOME_CMD
10/03/05   pa      Added DEACTIVATE_ZZ_CMD
09/26/05   awj     Added FEATURE_SRCH_RTL
09/20/05   pa      Added system measurement commands
09/16/05   ejv     Added DIV_RELEASE_CMD.
08/31/05   ejv     Added internal commands for TC SM.
08/29/05   kwo     Merged page match commands
08/18/05   bt      Reworked the no_rf_lock case.
06/27/05   kwo     implementation, first cut
===========================================================================*/


/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

/* Other */
#include "rex.h"
#include "stm.h"


/*===========================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*-------------------------------------------------------------------------
      Variables
-------------------------------------------------------------------------*/

/* Shared internal command queue */
extern  stm_int_q_type   srch_int_cmd_q;

/*-------------------------------------------------------------------------
      Defines
-------------------------------------------------------------------------*/

/* Debug logging for all inputs */
#define SRCH_STM_DEBUG_ALL              ((uint32)~0)

/*-------------------------------------------------------------------------
      Macros
-------------------------------------------------------------------------*/

/* Extracts boolean payload if payload is available; returns FALSE otherwise */
#define SRCH_STM_GET_BOOLEAN( payload ) ( ((payload) == NULL)     ?          \
                                          FALSE                   :          \
                                          *((boolean *) (payload)) )

/*-------------------------------------------------------------------------
      Internal Command IDs
-------------------------------------------------------------------------*/

typedef enum
{
  INTERNAL_CMD_BLOCK_START = 100,

  /* Common Internal Commands */
  SRCH_START_CMD,
  SRCH_STOP_CMD,
  SRCH_OFFLINE_CMD,
  SRCH_DUMP_CMD,
  SRCH_LOST_DUMP_CMD,
  ROLL_CMD,
  SLEW_ROLL_RSP_CMD,
  SRCH_TL_TIMER_CMD,
  SRCH_OFREQ_UPDATE_TIMER_CMD,
  RSSI_TIMER_CMD,
  TUNE_PREP_CMD,
  TUNE_DONE_CMD,
  RX_TUNE_CMD,
  SYS_MEAS_CMD,
  AFC_RPT_TIMER_CMD,
  FRAME_STROBE_CMD,
  SYNTH_FRAME_STROBE_CMD,
  GOVERN_TIMER_CMD,
  SRCH_PEAK_CMD,
  REQUEST_SYSTEM_RESTART_CMD,
  LOST_RF_RESOURCE_CMD,
  MDSP_ENABLED_CMD,
  RX_TUNE_COMP_DELAY_CMD,
  FREQ_TRACK_OFF_DONE_CMD,
  GO_TO_SLEEP_CMD,
  GO_TO_DOZE_CMD,
  RF_REPORT_TIMER_CMD,
  TX_RESOURCE_REQUIRED_CMD,

  /* Idle State Internal Commands */
  ABORT_CMD,
  START_SLEEP_CMD,
  REACQ_CMD,
  QPCH_REACQ_CMD,
  QPCH_REACQ_DONE_CMD,
  OFREQ_CMD,
  PRIO_CMD,
  WAKEUP_DONE_CMD,
  WAKEUP_NOW_CMD,
  REACQ_DONE_CMD,
  TL_DONE_CMD,
  QPCH_DEMOD_DONE_CMD,
  WAKEUP_CMD,
  CX8_ON_CMD,
  FING_SLEW_DONE_CMD,
  PAGE_MATCH_CMD,
  ADJUST_TIMING_CMD,
  FLUSH_FILT_CMD,
  FAKE_REACQ_CMD,
  FAKE_QPCH_REACQ_CMD,
  QPCH_CONTINUE_CMD,
  SNOOZE_CMD,
  RF_TUNE_TIMER_CMD,
  OFREQ_DONE_SLEEP_CMD,
  OFREQ_HANDING_OFF_CMD,
  OFREQ_HANDOFF_CMD,
  OFREQ_HANDOFF_DONE_CMD,
  NSET_UPDATE_CMD,
  XFER_FROM_QPCH_CMD,
  CONFIG_RXC_CMD,
  LOG_STATS_CMD,
  RECORD_DONE_CMD,
  FREQ_TRACK_DONE_CMD,
  DEMOD_DONE_CMD,
  NO_RF_LOCK_CMD,
  ORIG_PENDING_CMD,
  REBUILD_LISTS_CMD,
  HO_REQUEST_CMD,
  SCAN_ALL_CMD,
  SCAN_ALL_RPT_CMD,
  ONLINE_CMD,
  BC_INFO_CMD,
  CDMA_CMD,
  CDMA_RSP_CMD,
  IDLE_CMD,
  IDLE_RSP_CMD,
  TIMED_CMD,
  OFREQ_SUSPEND_CMD,
  RESELECTION_CHECK_CMD,
  OCONFIG_NBR_FOUND_CMD,
  SLEEP_OK_CMD,
  SYS_MEAS_TIMER_CMD,
  ABORT_COMPLETE_CMD,
  RF_GRANTED_CMD,
  RF_PREP_CMD,
  DOZE_CMD,
  SLEEP_TIMER_EXPIRED_CMD,
  TILL_REACQ_TIMER_EXPIRED_CMD,
  DEACTIVATE_ZZ_CMD,
  ABORT_HOME_CMD,
  DELAYED_ASET_CMD,
  ABORT_OFREQ_CMD,
  SCHED_SRCH4_REG_CMD,
  ONLINE_RF_GRANTED_CMD,
  ONLINE_TUNE_DONE_CMD,
  IDLE_DIV_REQUEST_CMD,
  WAKEUP_UPDATE_CMD,
  QTA_BUILD_RF_SCRIPT_CMD,
  QTA_EXIT_CMD,
  RF_TUNE_DENIED_CMD,

  /* Inactive State Internal Commands */
  ENTER_AMPS_CMD,
  ENTER_CDMA_CMD,
  #ifdef FEATURE_MODEM_1X_IRAT_LTO1X
  SRCHLTE_TT_F,
  IRAT_TT_F,
  #endif  /* FEATURE_MODEM_1X_IRAT_LTO1X */

  /* CDMA State Internal Commands */
  CD_RF_LOCK_GRANTED_CMD,
  MDSP_START_CMD,
  SRCH4_MDSP_REG_CMD,
  SRCH_SCLK_COMPLETE_CMD,
  WAIT_DONE_CMD,
  FIRST_SYS_MEAS_CMD,
  NEXT_SYS_MEAS_CMD,
  RF_REQ_FAIL_TIMER_CMD,
  SKIP_FREQ_DONE_CMD,
  AFLT_RELEASE_DONE_CMD,
  RF_RELEASE_DONE_CMD,
  SYS_MEAS_ABORT_COMPLETE_CMD,
  #ifdef FEATURE_MODEM_1X_BATCH_ACQ
  CD_RF_DIV_DISABLED_CMD,
  #endif  /* FEATURE_MODEM_1X_BATCH_ACQ */

  /* ACQ State Internal Commands */
  CK_AGC_CMD,
  MODE_DET_CMD,
  RSSI_CK_TIMER_CMD,
  FREQERR_CK_TIMER_CMD,
  VERIFY_TO_TIMER_CMD,
  PULLIN_TO_TIMER_CMD,
  ACQ_TO_TIMER_CMD,
  SRCH4_MDSP_APP_EN_CMD,
  SRCH4_MDSP_APP_DIS_CMD,
  AFC_ACQ_SUCCESS_CMD,
  AFC_START_VCO_PULLIN_CMD,
  #ifdef FEATURE_MODEM_1X_BATCH_ACQ
  ACQ_ABORT_CMD,
  #endif  /* FEATURE_MODEM_1X_BATCH_ACQ */

  /* Common State Machine Internal Commands */
  TCXOMGR_CMD,
  RAND_BITS_CMD,
  MSGR_SRCH4_MSG_CMD,
  MSGR_1XDEMOD_MSG_CMD,
  COEX_POWER_IND_CMD,
  #ifdef FEATURE_MODEM_1X_SRCH_ASD
  SRCH_ASD_ANTENNA_SWITCH_CMD,
  SRCH_ASD_RF_SET_ANT_CMD,
  SRCH_ASD_CLEAR_TRAFFIC_DATA_CMD,
  #endif /* FEATURE_MODEM_1X_SRCH_ASD */

 /* Diversity State Machine Internal Commands */
  DIV_REQUEST_CMD,
  DIV_PREP_CMD,
  DIV_TUNE_CMD,
  DIV_INIT_CMD,
  DIV_READY_CMD,
  DIV_PAUSE_CMD,
  DIV_STOP_CMD,
  DIV_RELEASE_CMD,
  DIV_AGC_ECIO_CK_CMD,

  /* Traffic GPS State Machine Internal Commands */
  TCG_VISIT_REQUEST_CMD,
  TCG_RETRY_TIMER_CMD,
  TCG_MC_RESPONSE_CMD,
  TCG_CANCEL_VISIT_REQUEST_CMD,
  TCG_VISIT_DONE_PRE_TUNE_CMD,
  TCG_VISIT_DONE_POST_TUNE_CMD,

  /* AFC State Machine Internal Commands */
  AFC_START_ACQ_CMD,
  AFC_PULL_IN_DONE_CMD,
  AFC_PULL_IN_FAIL_CMD,
  AFC_NO_CONTROL_CMD,
  AFC_FAST_CMD,
  AFC_SLOW_CMD,
  AFC_SRL_CMD,
  AFC_XO_CMD,
  AFC_LOG_TIMER_CMD,
  AFC_ACCESS_CMD,
  AFC_ACCESS_EXIT_CMD,
  AFC_TRAFFIC_CMD,
  AFC_CDMA_CMD,
  AFC_FREQ_TRACK_OFF_CMD,
  AFC_FREQ_TRACK_OFF_RTL_CMD,
  AFC_FREQ_TRACK_ON_CMD,
  AFC_TC_EXIT_CMD,
  AFC_OFF_CMD,
  AFC_SANITY_TIMER_CMD,
  AFC_ROT_PUSH_SAMPLE_TIMER_CMD,
  AFC_ROT_PUSH_FLAG_CMD,

  /* Traffic State Internal Commands */
  ACTION_TIME_CMD,
  DO_SRCH_CMD,
  REF_TIMER_CMD,
  CTR_TIMER_CMD,
  PPSMM_TIMER_CMD,
  ASET_UPDATE_PRE_TUNE_CMD,
  ASET_UPDATE_POST_TUNE_CMD,
  CFS_ACTION_CMD,
  ENTRY_TUNE_CMD,
  IDLE_ENTRY_DIV_DISABLED_CMD,
  IDLE_ENTRY_COMPLETE_CMD,
  DATA_CALL_CHG_CMD,
  #ifdef FEATURE_MODEM_1X_DRX
  DRX_STATE_CHANGE_CMD,
  #endif /* FEATURE_MODEM_1X_DRX */

  /* Internal WTR Hopping commands in traffic State */
  WTR_HOP_PRE_TUNE_CMD,
  WTR_HOP_POST_TUNE_CMD,

  /* Internal cmd to trigger the TuneAway */
  TUNE_AWAY_UNLOCK_BY_CMD,
  TUNE_AWAY_UNLOCK_REQUIRED_CMD,
  TUNE_AWAY_UNLOCK_IMMEDIATE_CMD,
  TUNE_AWAY_UNLOCK_CANCEL_CMD,
  TUNE_AWAY_UNLOCK_TIMER_CMD,
  TUNE_AWAY_DIV_DISABLED_CMD,
  TUNE_AWAY_RF_CHAIN_GRANTED,
  TUNE_AWAY_EXIT_CMD,
  TUNE_AWAY_PROCEED_F,
  TUNE_AWAY_LOG_CMD,
  TUNE_AWAY_APS_CMD,  

  #ifdef FEATURE_1X_TUNEAWAY_TEST
  TUNE_AWAY_TEST_CMD,
  #endif /* FEATURE_1X_TUNEAWAY_TEST */

  /* Neighbor Set Traige internal commands */
  NS_TRIAGE_TIMER_CMD,
  NS_CTR_TIMER_CMD,

  /* System measurement Internal commands */
  RF_LOCKED_CMD,
  REGISTER_DONE_CMD,
  AGC_TIMER_CMD,
  MEAS_NEXT_CMD,
  MEAS_DONE_CMD,

  #ifdef FEATURE_MODEM_1X_IRAT_LTO1X
  /* IRAT related internal commands */
  IRAT_START_CDMA_TT_F,
  IRAT_PILOT_MEAS_CMD,
  IRAT_PILOT_MEAS_ABORT_CMD,
  IRAT_PILOT_MEAS_CFG_TIMER_CMD,
  IRAT_PILOT_MEAS_CFG_RSP_CMD,
  IRAT_PILOT_MEAS_CFG_RSP_DEREG_CMD,
  IRAT_PILOT_MEAS_STOP_CMD,
  IRAT_RESEL_RF_LOCKED_CMD,
  IRAT_FIND_SYNC80_CMD,
  IRAT_FING_LOCKED_CMD,
  IRAT_FING_LOCK_FAILED_CMD,
  IRAT_JUMP_FAILED_CMD,
  IRAT_TRANS_TO_TRAFFIC_CMD,
  IRAT_TRANS_TO_IDLE_CMD,
  IRAT_FTM_INIT_CMD,
  IRAT_FTM_MEAS_CMD,
  #endif /* FEATURE_MODEM_1X_IRAT_LTO1X */

  #ifdef FEATURE_1X_TO_LTE
  IRAT_1XTOL_START_CMD,
  IRAT_1XTOL_INIT_CNF_CMD,
  IRAT_1XTOL_SRCH_MEAS_REQ_CMD,
  IRAT_1XTOL_SRCH_MEAS_CNF_CMD,
  IRAT_1XTOL_SRCH_CNF_CMD,
  IRAT_1XTOL_MEAS_CNF_CMD,
  IRAT_1XTOL_CLEANUP_CNF_CMD,
  IRAT_1XTOL_DEINIT_REQ_CMD,
  IRAT_1XTOL_DEINIT_CNF_CMD,
  IRAT_1XTOL_RESEL_CHECK_CMD,
  IRAT_1XTOL_ABORT_REQ_CMD,
  IRAT_1XTOL_ABORT_CNF_CMD,
  #endif /* FEATURE_1X_TO_LTE */

  /* this is the DO timer transfer internal commands */
  TT_READY_CMD,
  TT_SLAMCOMB_CMD,

  /* RX diversity Autoswitching Internal Commands*/
  DIV_AUTO_ENABLE_CMD,
  DIV_AUTO_DISABLE_CMD,
  DIV_AUTO_PAUSE_CMD,
  DIV_AUTO_RESUME_CMD,
  DIV_AUTO_FORCE_OFF_CMD,
  DIV_AUTO_IS_DATA_CALL_CMD,
  DIV_AUTO_IS_1X_ADV_TRAFFIC_CMD,
  DIV_AUTO_FPC_ENABLED_CMD,
  DIV_AUTO_AUTOSWITCH_CMD,

  /* CGPS Commands */
  REQUEST_1X_PPM_OP_CMD,
  RELEASE_1X_PPM_OP_CMD,
  REQUEST_1X_SYNC80_DATA_CMD,
  GET_SRCH_WINDOW_CENTER_CMD,
  GET_REF_PN_CMD,
  GET_ACT_CAND_SETS_CMD,
  ENABLE_TTD_RECORDING_CMD,
  DISABLE_TTD_RECORDING_CMD,
  REQUEST_SYSTIME_UNC_CMD,
  REQUEST_TTD_CMD,
  CANCEL_TTD_REQUEST_CMD,
  SEND_PPM_LIST_UPDATE_CMD,
  REQUEST_RF_INFO_CMD,
  GPS_ISR_CMD,

  /* RX Internal Commands */
  RX_REQ_AND_NOTIFY_CMD,
  RX_REQ_ENABLE_DIV_CMD,
  RX_REQ_DISABLE_DIV_CMD,
  RX_RESERVE_AT_CMD,
  RX_RELEASE_CMD,
  RX_DISABLE_CMD,
  RX_SCHEDULED_DISABLE_CMD,
  RX_ANTENNA_TUNER_CMD,
  RX_MOD_REASON_CMD,
  RX_MOD_CRITERIA_CMD,
  RX_MOD_DURATION_CMD,
  RX_MOD_EXTEND_DURATION_CMD,
  RX_MOD_REAS_DUR_CMD,
  RX_MOD_IRAT_REAS_DUR_CMD,
  RX_EXCHANGE_DEVICES_CMD,
  RX_EXCHANGE_IRAT_CMD,
  RX_TRM_CH_CONFLICT_CMD,
  RX_TRM_CH_PENDING_CMD,
  RX_TRM_CH_ENABLE_CMD,
  RX_TRM_CH_DISABLE_CMD,
  RX_TRM_CH_GRANTED_CMD,
  RX_TRM_CH_MODIFIED_CMD,
  RX_TRM_CH_SHARING_CMD,
  RX_TRM_TIMEOUT_CMD,
  RX_HOP_DEVICES_CMD,
  TX_TUNE_CMD,
  TX_WAKEUP_CMD,
  TX_SLEEP_CMD,

  /* RX Internal RF Response Commands */
  RX_RF_RSP_WUP_PREP_COMP_CMD,
  RX_RF_RSP_WUP_COMP_CMD,
  RX_RF_RSP_SLP_COMP_CMD,
  RX_RF_RSP_TUNE_COMP_CMD,
  RX_RF_ENABLE_DIV_COMP_CMD,
  RX_RF_DISABLE_DIV_COMP_CMD,
  RX_RF_DISABLE_COMP_CMD,
  RX_RF_RSP_SCHED_SLP_ERROR_CMD,

  /* RX Response Commands */
  RX_CH_DENIED_CMD,
  RX_CH_GRANTED_CMD,
  RX_RF_DISABLED_CMD,
  RX_RF_DISABLED_ERR_CMD,
  RX_TUNE_COMP_CMD,
  RX_MOD_GRANTED_CMD,
  RX_MOD_DENIED_CMD,
  RX_CH_ENABLE_DIV_COMP_DONE_CMD,
  RX_CH_DISABLE_DIV_COMP_DONE_CMD,
  RX_EXCHANGE_DEVICES_COMP_CMD,
  RX_EXCHANGE_IRAT_COMP_CMD,

  /* TRAM Commands */
  TRAM_REQUEST_CMD,
  TRAM_TUNE_CMD,
  TRAM_MODIFY_CMD,
  TRAM_DISABLE_CMD,
  TRAM_RELEASE_CMD,
  TRAM_PENDING_CMD,
  TRAM_DISPATCH_CMD,

  LAST_INT_CMD,
} int_cmd_code_type;


/*===========================================================================

                    FUNCTION DECLARATIONS FOR MODULE

===========================================================================*/

/*===========================================================================

FUNCTION       SRCH_STM_INIT

DESCRIPTION    This function initializes the search STM module.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
void srch_stm_init( void );

/*===========================================================================

FUNCTION       SRCH_STM_DEINIT

DESCRIPTION    This function deinitializes the search STM module.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
void srch_stm_deinit( void );


#ifdef SRCH_IDLE_DEBUG_BUFFER
/*===========================================================================

FUNCTION       SRCH_STM_IDLE_INIT

DESCRIPTION    This function initializes the search IDLE STM module.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_stm_idle_init( void );

/*===========================================================================

FUNCTION       SRCH_STM_IDLE_DEINIT

DESCRIPTION    This function deinitializes the search IDLE STM module.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_stm_idle_deinit( void );
#endif /*  SRCH_IDLE_DEBUG_BUFFER  */

/*===========================================================================

FUNCTION SRCH_STM_GET_MSG_BUF

DESCRIPTION    This function allocates a message buffer including header
               information for the state machine driver.

DEPENDENCIES   None

RETURN VALUE   Pointer to the payload field - header data will be in front,
               and clobbering it will have unsavory effects.

SIDE EFFECTS   None

===========================================================================*/
extern void *srch_stm_alloc_msg_buf
(
 stm_state_machine_type *sm,       /* state machine command is for */
 stm_group_type         *group,    /* group that the state machine is in */
 int_cmd_code_type       id,       /* internal command ID */
 uint32                  size      /* size of internal command payload */
);

/*===========================================================================

FUNCTION SRCH_STM_DEBUG_HOOK

DESCRIPTION    This function is called every time an input is processed.
               The debug_info structure pointer passed-in provides

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_stm_debug_hook(user_debug_info_type *debug_info);

/*===========================================================================

FUNCTION SRCH_STM_GET_MSG_BITMASK

DESCRIPTION    This function returns a 32 bit bitmask with a 1 in the
               position of the index of the passed-in message.

DEPENDENCIES   None

RETURN VALUE   If the passed-in message resides between index 0 and 31,
               a bitmask equivalent to (1 << index) will be returned.  Upon
               error or index out of range, the return value will be 0.

SIDE EFFECTS   None

===========================================================================*/
extern uint32 srch_stm_get_msg_bitmask
(
  stm_state_machine_type const *sm,
  stm_input_type                message_id
);

/*===========================================================================

FUNCTION       SRCH_STM_EXT_CMD_SIG_MAPPER

DESCRIPTION    This function maps signals to state machine inputs

DEPENDENCIES   None

RETURN VALUE   TRUE if mapped, FALSE otherwise

SIDE EFFECTS   sig_cmd structure appropriately populated upon success

===========================================================================*/
extern boolean srch_stm_ext_cmd_sig_mapper
(
  rex_tcb_type *tcb,          /* tcb of the process with the signal */
  rex_sigs_type sig,          /* signal which is set */
  stm_state_machine_type *sm, /* ptr to the sm which will handle the ext cmd */
  stm_sig_cmd_type  *sig_cmd  /* ptr to the sig_cmd structure to populate */
);

/*===========================================================================

FUNCTION       SRCH_STM_EXT_CMD_ERROR_HANDLER

DESCRIPTION    This function handles the case where an external command
               wasn't processed in the state machine that received it.
               As a recourse, we'll simply reply back to the caller with
               a SRCH_ERR_S to unblock them, since they're probably waiting
               on an ACK to the original command.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_stm_ext_cmd_error_handler
(
  stm_sig_cmd_type *sig_cmd  /* unprocessed sig_cmd structure ptr */
);

/*===========================================================================

FUNCTION SRCH_STM_PRINT_DEBUG_TRACE

DESCRIPTION    This function prints the diag messages of the entire
               stm_debug_trace structure

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_stm_print_debug_trace( void );

/*===========================================================================

FUNCTION SRCH_STM_PRINT_INT_CMD_Q

DESCRIPTION    This function prints the diag messages of the internal command
               queue entries from the read index to the write index.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_stm_print_int_cmd_q( void );
#endif /* SRCH_STM_H */
