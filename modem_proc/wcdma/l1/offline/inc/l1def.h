#ifndef L1DEF_H
#define L1DEF_H
/*===========================================================================

                    L 1   C O M M O N   D E F I N I T I O N S
DESCRIPTION
  This file contains common declarations and references for the units
  within L1.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2000 - 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/l1def.h_v   1.33   19 Jul 2002 13:57:46   djm  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/l1def.h#4 $
$DateTime: 2020/03/13 02:22:32 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/16/17   rn      FR 50745 MAAT for WCDMA
01/23/18    sk      Remove deadlock for releasing secondary antenna.
08/21/17   ss      Delay CPHY_CNF for fast LTA resume by atleast 20ms
07/25/16   am      next drx recovery for PO overrun cases when next PICH occassion is very close and sleep type is returned as awake indef 
                   from next_frame_wanted in drx_entity_awake_entry()
03/29/16   sg      Adding local cmd for handling MCPM relinquish for DCUPA deactivation 
                   to move it to WL1 task context from RF retune done
01/08/16   pr      Featurize WL1 custom msgs to use _ext diag APIs when DUAL WCDMA is enabled.
03/06/16   svh     Move ASD MDSP command unsuspend to task context.
10/16/15   rsr     Added Changes for Common Messages.
12/10/15   pv      New LOCAL CMD for WSLEEP repgrogramming from Offline to Online PICH.
11/05/15   svh     MDSP Recovery changes.
10/21/15   da      Changes to update rgs in wl1 task context
10/20/15   raj     try sche_seq lock and call ent_sche_update in task context
09/25/15   skk     Post a local cmd to change L1M state from BCH to BCH_SLEEP.
10/06/15   pvr     Mods to post local cmd to update CXM phychan DB
10/06/15   ap      Changes to add a local cmd for deactivating fingers on receing scch order to avoid race conditions
10/02/15   ac      FR 26288: DS downlink quality measurement reporting
09/13/15   ms      Move HS-CM registration to task context
09/01/15   ar      Changes to lower Q6 clock during periods of no EUL transmission
09/01/15   gj      Delay ACQ CNF fail to RRC until after search abort completes
08/07/15   pvr     Add new local cmd WRM_SEC_UNLOCK_TIMER_EXPIRED_CMD  
07/29/15   ap      Fix is to handle QTA entity wake-up in local task so that any ul tx commands in the local command queue will be checked prior to initiating FS
07/23/15   ks      Turn off QICE when W2L goes active
07/21/15   pr      Use MSG EXT only when DUAL WCDMA is enabled.
07/02/15   sks     Do not execute the guard timer expiry call back if not in PCH SLEEP. 
06/30/15   tsk     FR 26237: DSDS with Rel.8 E-FACH HSRACH + MSIM Changes.
06/30/15   ac      Enable extended msg support on BO
06/25/15   vn      Renaming SRCH START CPC DRX local command.
06/22/15   ms      Enable new event changes only for Thor 2.0
06/18/15   hdk     Removing FEATURE_WCDMA_JOLOKIA_MODEM from DSDA ASDiv changes.
06/08/15   rsr     Fixed Compilation warnings for W+W Feature.
06/03/15   svh     TA featurisation.
06/03/15   pr      W+W Phase8 Checkin.
05/07/15   skk     FR24494: BCH Inter SIB sleep initial check in.
02/27/15   ap      Initial check-in for WL1 QSH framework 
04/28/15   ac      Add 0x41D0 - DPCH demod enhancement log packet
03/26/15   sad     update to 0x41B6 log packet
04/23/15   pr      WplusW changes for Log Packets and Events
03/29/15   pr      WplusW changes for Diag F3.
02/28/12   rsr     W+W Phase 2 Checkin.
01/20/15   kcm     Call MCVS for shadow cell update in task context
12/22/14   sks     Post the fake DRX cmd via a local cmd.
12/29/14   sl      Reschedule FMO if cfg window is missed
11/13/14   psr     Removed EVENT_WCDMA_TO_WCDMA_RESELECTION_START
10/29/14   jd      Complete mode change on FW search abort
10/22/14   sr      Changes to inform CPC clients when CPC is activated
10/02/14   sd      Removed l1m_wait_for_trm_stop state and related routines
10/03/14   gsk     Resume FMO only after all clients requested for resuming the FMO.
09/23/14   pv      Support for TRM swapping of chains for call setup in DR mode.
                   Added local cmd for DRX_PREP_FOR_CALL_EST.
09/11/14   sr      Changes not to issue skipped triage as part of EUL/HS chan cfg
07/28/14   skk     Handle pended ext cmd by setting ext_cmd_sig again.
08/07/14   sr      Changes to handle freeze/unfreeze for CPC SCCH order in local command
08/04/14   sad     RxD changes for DB-MC in case of SCCH order
07/31/14   gm      Changes to reduce the text size.
07/28/14   pkg     Code Check-in for 16QAM feature.
07/26/14   rkmk    Reduce image size by using right macro depending upon number of parameters
07/23/14   abs     Mods to support 0X41B6 log packet Version-2
07/09/14   sad     Fix for incorrect enum usage
07/16/14   sr      EDRX code cleanup (reusing local command code)
07/15/14   bj      WCDMA Narrow Band Rejection for System Search.
07/11/14   gp      Added support for RF Temp Comp Split architecture
07/03/14   sks     Changes for FCDC
07/01/14   hk      Header file inclusion cleanup
06/24/14   pkg     Moving DC-HSUPA specific code under FEATURE_WCDMA_DC_HSUPA flag.
06/24/14   hdk     Featurizing DSDA ASDiv changes under FEATURE_WCDMA_JOLOKIA_MODEM
06/19/14   ash     Backed out code for 0x410E Log Pkt
06/13/14   hdk     ASDiv DSDA feature
05/23/14   rs      Local cmd for tearing down diversity
04/30/14   pr      Issue a Demod status dump every frame if TxD detection 
                   is in progress for any cell
05/21/14   jk      Moved the periodic measurements handling from srch_ops_cmd to a 
                   new local command.
05/13/14   ymu     Feature name FEATURE_WCDMA_DC_HSUPA_FRAME replacement with FEATURE_WCDMA_DC_HSUPA 
05/14/14   sks     IDLE Wakeup Optimizations and cleanup
04/27/14   jkb     Add check for Feature dual sim with feature qta  
04/16/14   bj      Remove duplicate code in split ACQ code
04/02/14   pr      Fix for KW errors.
04/11/14   yw      DCUPA: RF retune code cleanup
03/13/14   ac      Add L1_RXD_AGC_CFG_DONE_CMD
02/28/14   skk     Handle the lock_not_rel timer expiry in WL1 task context.
02/12/14   sad     DB-3C API check in
04/01/14   km      Rxd/ACQ changes
04/01/14   rs      Local cmd for requesting secondary antenna with wrm.
02/06/14   pr      Changed local cmd ASET_SRCH_DONE name to NOTIFY_WL1_DL_CARRCFG_ON_OPS_DONE_CMD 
03/27/14   rc      Updated DSR state machine to handle concurrenrency issues
01/28/14   da      Fixed Compiler Warning
01/21/14   ar      Add support for handling QICE during W2L measurements in FACH
01/22/14   gm      post a local command to handle unlock cancel event. 
01/21/14   abs     Cleaning up DSDA~CXM(FR:2334) changes
01/20/14   amj     Fixed KW error. 
07/01/14   amj     Clean up old ASET update APIs
01/07/14   as      Added command WL1TRM_TUNEAWAY_LOG_DONE_CMD.
01/06/14   sr      Suspending periodic ops before CPHY req processing
12/30/13   pr      SCCH order State machine to avoid concurrencies.
12/16/13   as      Porting G2W TA from Triton to Dime.
12/10/13   pv      Local cmds for STMR INIT and RFM PREP at wakeup.
12/04/13   ymu     Logging issue: Remove log packet submit from ISR to local cmd when deactivate DC-HSUPA
11/21/13   jd      FET 2.0 Searcher two queue approach.
11/21/13   dm      Post local command for SFN/CFN sanity check.
11/21/13   tsk     FR2573: SAR management with Integrated DSDA changes.
11/14/13   jd      Sw/Fw interface changes for searcher two queue approach.
11/13/13   hk      Removed the inclusion of UWord64
11/05/13   hk      Temporary inclusion of UWord64 typedef
07/17/13   sm      Support for rxd local command ctrl with CPC-DRX+RxDPM
10/23/13   sad     Update to inverse DL freq calculations
10/17/13   bs      Preliminary 3C Support
09/23/13   as      Porting Dual Sim from Triton to Dime.
09/18/13   vs      HS redesign.
09/12/13   jd      Searcher redesign
07/23/13   vs      Added Antenna switch cmd support
07/22/13   ymu     Remove DC_HSUPA flushing logging packet from EUL ISR to local cmd 
07/18/13   gp      Adding QXDM Event support for HSRACH Operations
06/27/13   sad     Added support for secondary Tx RL failure/Phychan estb. failure
05/21/13   sad     Added DC HSUPA code changes under FEATURE_WCDMA_DC_HSUPA 
05/20/13   sad     Support for Enhanced Layer-1 Manager
04/09/13   vs      added support for the new log pkt wcdma periodic statistics.
04/01/13   hk      Mainlined FEATURE_WCDMA_EFDPCH
03/10/13   sr      Changes to log enhanced L1 state event
02/20/13   hk      Removed FEATURE_INTER_SIB_SLEEP
01/28/13   dp      Added support for Antenna Tuner
01/21/13   stk     Support for Femto/CSG selection and reselection
08/29/12   pv      New DPC_SYNC_CMD for PICH timeline opts.
11/12/12   dm      Changes to register a callback with triage if any module 
                   wants to do finger related processing while triage is running.
08/09/12   hk      Fixed compiler error
08/09/12   hk      Streamlined FACH and DCH module cleanup procedures
08/19/12   sj      Added support for QICE receiver in EDRX mode
08/15/12   zr      Removing L1_SRCH_PERIODIC_LOG_CMD to use in search ops command
08/14/12   zr      Adding searcher periodic log local command 
08/14/12   dp      Added qxdm event for CM state changes
08/07/12   dp      W2L PSHO time improvements
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
07/05/12   pv      Offline PICH changes - new SAMPSERV_MGR_CMD.
06/21/12   sr      Added new local cmd to inform edrx inter freq when searches are done
06/08/12   gv      Feature HS RACH Initial code check in
06/07/12   vs      MBMS feature cleanup.
05/03/12   vsr     FET/DED/ED-V2 Feature checkin
04/12/12   zr      Add support for periodic TD determination.
04/09/12   dp      Serialized CM and RF configs using l1cmmgr's new NFI system
03/27/12   hk      Added support for FEATURE_WCDMA_HS_FACH_DRX
03/10/12   sv      Added new local command for TxD reschedule procedure.
02/10/12   rgn     Nikel feature cleanup
02/01/12   gnk     Nikel Feature cleanup.
01/31/12   raj     Nickel feature cleanup. Mainlined the features FEATURE_WCDMA_A2_HW_UL
01/26/11   vs      Nikel feature cleanup.
01/19/12   zr      Mainlined FEATURE_WCDMA_GENESIS_SW and FEATURE_L1_CELL_RESELECTION, 
                   FEATURE_SLEEP, and FEATURE_CELL_FACH_MEAS_OCCASION
01/17/12   vsr     Added ARD Algorithm Support
12/21/11   xl      Merge from MPSS.NI.1.1
11/23/11   rgn     Added new local command to initiate delayed triage procedure
11/14/11   ms      Moved MCPM R99_DATA client relinquish to WL1 task context
11/09/11   sb      Moved MCPM relinquish during EUL Drop from ISR to Task context
10/20/11   gnk     Removed MTD related code for NikeL
09/03/11   sv      Added local command to start CPC DRX search. 
08/29/11   kcm     Added DL_PHCH_RESUME_PHCH_OPS_AFTER_CM_SUSPEND command id
07/08/11   uk      Added L1_SRCHCR_RESUME_IDLE_SEARCH definition 
07/04/11   gnk     Mainlining FEATURE_WCDMA_UL_RACH_EVT_PWR_FIX feature
06/28/11   sp      Mainlining UL_RACH_DONE_CMD
06/09/11   hk      Delay the DC carrier config till cell_idx is available
05/11/11   gnk     Mainlining FEATURE_RF_TX_PWR_LIM,FEATURE_RF_TX_PWR_LIM_REV feat
04/28/11   sj      upmerge from offline/main
04/26/11   gnk     Removed FEATURE_WCDMA_HSUPA_IRQ_DELAY_FLOW_CONTROL feature
04/26/11   dp/uk   Add support for W2X_IDLE_CMD
04/19/11   sv      Added new local cmd to support FEATURE_WCDMA_GET_RAT_MEAS_INFO.
03/31/11   amj     Added code for CPC DRX CFG log packet.
03/11/11   dp      Ported CPC DRX changes to offline Architecture
03/12/11   sa      Fixed issues in W2L code.
01/31/11   sa\nd   Moved processing of INTER-RAT periodic measurements from 
                   ISR to task context.
01/07/11   sp      Removed Unused L1 Cmd Codes
01/06/11   vsr     Added support for QICE debug log packet
11/19/10   sp      Adding new UL_LOGGING_CMD to handle R99 UL log pkts in task context
10/19/10   ms      Restored EVENT_HSPA_PLUS_CFG support
09/15/10   ksr     Enhanced A2 logics with ERAM Readback, L1-L2 interace contents
                   dumping, Rd back profiling etc
09/02/10   gnk     Checking in the changes for UL open loop MTD feature
09/02/10   gv      Code Checkin for CPC DTX
08/23/10   uk      changed algorithm for sending LTE INIT/Deint
08/23/10   ks      Removed inclusion of mdsp header for Genesis (featurized)
08/17/10   ks      Added efdpch event logging support
06/30/10   vsr     Updated EQ logging for Dual Carrier
06/03/10   ms      Added support to log DC-HSDPA events
06/02/10   stk     Added support for W2LTE measurements and reselection under 
                   FEATURE_WCDMA_ABSOLUTE_PRIORITY & FEATURE_WCDMA_TO_LTE
04/20/10   ms      Added L1_HS_DEREGISTER_CLIENT_CMD
03/30/10   rvs     Remove FEATURE_WCDMA_CME_CONTROLLER.
03/26/10   hk      Freeze/UnFreeze CM during ASU procedure
03/08/10   rgn     Flexible carrier spacing changes 
02/10/10   ms      Added carrier id enum and max val macros
02/08/10   rgn     Syncup to mustang tip 
11/18/09   gv      Use db10 instead of db12 units for computation
11/10/09   gv      Added support for TTI RECONFIG EVENT on the offline builds.
11/09/09   rgn     Added local command to handle pending cell cfg/decfg state
11/07/09   hk      Added the support to enable DC on WPLT test mode
11/06/09   hk      Added the support to enable DC on R99
10/12/09   stk     Create a new local cmd, to call srch_clean_up done in task context
09/24/09   hk      Porting of AICH resetup feature to Offline
09/09/09   rvs     Add support for QICE F/Finv log packet.
07/15/09   rvs     Added the L1_CME_SUBMIT_QICE_LOG event and edited feature
                   flags to include FEATURE_WCDMA_QICE_CONTROLLER.
06/22/09   mr      Support for new HSPA+ events 
06/01/09   sv      Mainlined the search abort code.
05/05/09   sv      Added SRCH_ABORT_DONE_CMD to list of local cmds
04/15/09   ks      Added enum type for RxD triage local cmd
02/13/09   ks      Added enum type for triage local cmd
02/03/09   ka      Corrected code featurization (FEATURE_WCDMA_REL6_ENH_MPR)
01/20/08   ka      Added code changes for logging 4313
12/19/08   yh      Add CM-CME local cmd header
12/09/08   uk      Mainling FEATURE_WCDMA_DL_ENHANCED
11/04/08   rgn     New demod status dump designchanges
10/31/08   vsr     Added support for SCH-IC log packet
10/03/08   hk      Bringing in 7k Mailine fixes
09/29/08   mc      Added support for CME log packet
09/18/08   saik    Ensured compilation for Q6 when msmdsp.h file not present(using feature FEATURE_WFW_SW_INTERFACE)
08/27/08   vsr     Added CME capability under feature FEATURE_WCDMA_CME_CONTROLLER
08/20/08   gnk     Not using the function enc_print_rach_timing_debug
06/10/08   ms      Added PACKED_POST attribute for structures to make it
                   compatible for GNU compiler.
01/18/08   uk      Added SRCHCR_GSM_CLK_ENABLED_IN_DRX to l1_local_cmd_enum_type
01/16/08   vp      Added support for PCCPCH weak indication
01/16/08   mc      Added support for MCCH weak indication
12/26/07   rc      Added L1_INTERNAL_MEAS_EVT_6A_TO_6E_EVAL_CMD to
                   l1_local_cmd_enum_type.
10/24/07   vsr     Mainlining MBMS and Enhanced DL changes
10/15/07   gnk     Added Prach log packets & F3 msgs temporarily
09/26/07   nd      Added a new local command SRCH_FS_PROC_DONE which will be sent by the
                   ISR (fw interrupts after processing N slots of sample)
07/09/07   vb      Added rxd log submit local cmd
04/17/07   gnk     Changed enc_rach_no_ack_print_debug() to enc_print_rach_timing_debug()
                   to print timing information for all RACH procedures
03/16/07   am      4312 logpkt changes
03/07/07   nd      Added an event under FEATURE_WCDMA_L1_PSC_SCANNER
01/19/06   kps     Added TRM_ENABLE_RXD_CMD to enum l1_local_cmd_enum_type
10/27/06   kps     Added TCXO_SET_RESTRICTION_CMD to enum l1_local_cmd_enum_type
10/12/06   gv      Added structures to support the events specific to
                   HSUPA feature(reconfig_or_asu and serv_cell_change events).
10/10/06   ms      Added L1_HS_MODULE_CLEANUP_AFTER_HS_TEARDOWN_CMD to
                   enum l1_local_cmd_enum_type.
10/02/06   kps     Added TRM_STOP_CMD to enum l1_local_cmd_enum_type
09/25/06   gv      Removed the local command E_DL_DEBUG_LOGGING_CMD,as it is
                   no longer being used.
08/14/06   yiz     Mainline HSUPA code
07/06/06   kps     Added SIB_TIMER_EXPIRY_CMD to enum l1_local_cmd_enum_type
05/08/06   scm     Add updated reselection start event with 16 bits for PSC.
04/10/06   mc      Added support for EQ log packets
04/04/06   vip     Added local command for processing the hardware callbacks
                   received by RxD controller
03/21/06   vip     Added the local command for processing RxD
                    state update event
03/16/06   mc      Added support for Equalizer under the feature flag
                   FEATURE_WCDMA_EQ_CONTROLLER
08/18/05   vp      Added support for reslamming the pccpch for the TTI-1
                   hypothesis.
07/21/05   mc      Added HS_DSCH_STATUS event structure.
07/19/05   asm     Added a new command to log agc
07/08/05   mc      Added HS_SERVING_CELL_CHANGE event structure.
06/15/05   yiz     Disable TX pwr lim cmd for FEATURE_RF_TX_PWR_LIM_REV
05/05/05   gs      Added enum MDSP_CMD_LOG_SUBMIT to local command
03/07/05   rc      Added QUALMEAS_CRC_UPDATE_CMD to enum l1_local_cmd_enum_type
                   under feature FEATURE_WCDMA_QUALITY_MEAS.
01/28/05   rc      Added EVALUATE_INTERRAT_EVENTS_CMD to enum l1_local_cmd_enum_type.
11/15/04   sh      Removed SRCHCR_START_NSET_SRCH_CMD.
10/11/04   gs      Added local command codes for HSDPA
04/15/04   yiz     Rename FACH inter-RAT CR as FEATURE_CELL_FACH_MEAS_OCCASION
12/18/03   scm     Add FAILURE_REASON_NOT_APPLICABLE define for SUCCESS case.
12/10/03   yiz     Add SRCHFACH_GET_CRNTI_DONE_CMD & SRCH_FACH_MEAS_CMD for
                   FEATURE_INTER_RAT_CELL_FACH_RE_SELECTION
12/08/03   yiz     Replace last_pwr_sett with rf_tx_pwr_dbm in
                   wcdma_layer1_prach_evt_type for EVENT_WCDMA_PRACH.
11/10/03   yus     Delete FEATURE_6250_COMPILE.
10/01/03   sh      Added UL_RACH_DONE_CMD local command enum type.
07/24/03   yus     Include "msm.h" for MSM6250.
05/13/03   scm     Add event structure for new event: EVENT_WCDMA_DRX_CYCLE.
04/02/03   src     Added a new enum L1_PHYCHAN_NOOP_DONE_CMD to the L1 local
                   command type l1_local_cmd_enum_type.
03/24/03   yiz     Remove FEATURE_RACH_NO_ACK_DEBUG conditions to always enable.
03/14/03   scm     Remove failure_reason from wcdma_layer1_resume_evt_type.
03/05/03   scm     Change "num_frames" to "num_frames_suspended" to match ICD.
                   Add event structure for EVENT_WCDMA_L1_STATE.
02/28/03   scm     Putting all WCDMA L1 QXDM event structures in this file.
12/18/02   ATM     Made COMMAND enum CFA-feature friendly
10/01/02   ddh     Added SRCHCR_CHK_N_SRCH_CMD and SRCHCR_GSM_NSET_SRCH_CMDs
09/06/02   djm     ifdef FEATURE_CM_TGPS_RUNTIME_ERROR runtime error command
08/27/02   sh      Added SEQ_ALIGN_CELL_TRANS_DONE sent after sequencer
                   aligns to new cell timing during cell reselection.
07/18/02   djm     Addition of Compressed Mode TGPS RunTime Error local command
07/10/02   djm     Addition of Compressed Mode SRCH Done local command for possible use
06/24/02   sh      Added UL_RACH_NO_ACK_DEBUG_CMD for rach no-ack debug.
05/29/02   m       Added support for LOD by adding new L1 local command
05/22/02   sh/src  Undid mapping of wcdma_l1_tcb to l1_tcb as the latter is no
                   longer used.
05/16/02   yus     Mapping wcdma_l1_tcb to l1_tcb, wcdma_l1_task to l1_task.
05/18/02   sh      Added the following local commands to support cell
                   reselection: SRCH_CELL_RESEL_EVAL_CMD,
                   SRCH_NSET_SRCH_SORT_CMD, SRCH_NSET_SRCH_SORT_DONE_CMD.
05/10/02   cpe     Added SRCH_SFN_SFN_OBSERVED_DIFF_CMD and DL_MEAS_SFN_SFN_DIFF_CMD
                   to the l1 local commands
04/29/02   yus     Add #include "msmdsp.h"
04/12/02   yiz     Added UL_PHYCHAN_DPCH_LOG_DONE_CMD to
                   l1_local_cmd_enum_type to change to deferred logging.
02/26/02   sh      Added FEATURE_L1_ACT_TIME to l1_local_cmd_enum_type.
02/25/02   sh      Removed PHYCHAN_SETUP_START_CMD and added ACT_TIME_CMD to
                   l1_local_cmd_enum_type.
02/21/02   sh      Added PHYCHAN_SETUP_START_CMD and
                   UL_DL_PHYCHAN_DROP_DONE_CMD to l1_local_cmd_enum_type.
01/31/02   asr     added DRX_SLEEP_CMD to l1_local_cmd_enum_type
12/13/01   asr     added DRX_LOG_DONE_CMD to l1_local_cmd_enum_type
09/13/01   sk/yiz  Added RF_DO_TX_PWR_LIM_CMD local command definition.
08/09/01   sh/sk   Added defines for DCH and PRACH log functions.
07/15/01   sh      Changed local cmd enum names related to physical channel
                   setups.

05/21/01   sh      Removed RF_TUNE_DONE_CMD.

05/13/01   wjn     Added DL_MEAS_SFN_CFN_DIFF_CMD and
                   SRCH_SFN_CFN_OBSVED_DIFF_CMD.
                   Deleted SRCH_NBCH_SFN_INFO_CMD.

03/29/01   sh      Added local command enum DL_CELL_TRANS_DROP_DONE_CMD.

02/21/2001 mdl     removed DL_ACQ_PHYCHAN_CFG_CMD AND
                   DL_ACQ_PHYCHAN_FRM_BDRY_CMD from l1_local_cmd_enum_type

02/14/01   wjn     Added L1 local commands SRCH_CPHY_ASET_CMD and
                   PHYCHAN_ADD_DONE_CMD for channel setup handshake between
                   SRCh and DL module.
01/24/01   mdl     Added MODULE_CLEANUP_DONE_CMD enum.

01/10/2001 mdl     Added MDSP_SYNC_TIMEOUT_CMD enum

12/21/00   sh      Removed cmd_hdr_type and added q_link_type in
                   l1_local_cmd_hdr_type.

11/16/00   sh      Added DL_ACQ_PHYCHAN_CFG_CMD, DL_ACQ_PHYCHAN_FRM_BDRY_CMD,
                   RF_TUNE_DONE_CMD local commands.
                   Deleted UL_PHYCHAN_SETUP_DONE_CMD, DL_PHYCHAN_SETUP_DONE_CMD
                   local commands.

08/15/00   yus     Created file.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "customer.h"
#include "rex.h"
#include "queue.h"

#include "msm.h"

#include "log.h"
#include "l1msm.h"

#include "eul.h"
#include "wl1msimarbiter.h"
#include "wcdmadiag.h"
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* L1 local command Enumerated IDs */
/* ************************************** *
 * This list is used by logging!!!        *
 * Send updates to cfa.team               *
 * ************************************** */
typedef enum
{
  /* Search Dump Signal */
  SRCH_DONE_CMD               =   0x0,

  /* Search Heartbeat Signal */
  SRCH_OPS_CMD                =   0x1,

  /* Lost Search Signal */
  SRCH_LOST_SRCH_CMD          =   0x2,

  /* Local command returning the SFN-CFN Observed Time Difference */
  SRCH_SFN_CFN_OBSVED_DIFF_CMD=   0x4,

  /* Local command returning the SFN-SFN Observed Time Difference */
  SRCH_SFN_SFN_OBSERVED_DIFF_CMD=   0x5,

  /* Local command to start cell reselection evaluation */
  SRCHFACH_RESUME_FMO_CLIENT_OPS    =   0x6,

  /* Local command to indicate nset search/sort is done. */
  SRCHCR_NSET_SRCH_DONE_CMD   =   0x8,

  /* Local command to check if an inter-freq or inter-rat srch
   * should be started or if the intra-freq results should just
   * be processed after the PICH has been demodulated
   */
  SRCHCR_CHK_N_SRCH_CMD       =   0x9,

  /* command to search/sort GSM cells */
  SRCHCR_GSM_NSET_SRCH_CMD    =   0xA,

  /* Compressed Mode Measurement Done Local Command */
  CM_MEAS_DONE_CMD            =   0xB,

  /* Compressed Mode TGPS RunTime Error Command */
  CM_TGPS_RUNTIME_ERROR_CMD   =   0xC,

  /* Command to indicate next operation during Activation Time */
  ACT_TIME_CMD                =   0xD,

  /* Commands to setup UL/DL CCTrCh */
  UL_CCTRCH_SETUP_DONE_CMD    =   0xE,
  DL_CCTRCH_SETUP_DONE_CMD    =   0xF,

  /* Local Commands related to DL/UL Phy Chan drops */
  UL_DL_PHYCHAN_DROP_DONE_CMD =   0x10,
  UL_PHYCHAN_DROP_DONE_CMD    =   0x11,
  DL_PHYCHAN_DROP_DONE_CMD    =   0x12,

  /* Local Commands related to DL/UL Phy Chan configs */
  UL_PHYCHAN_CFG_DONE_CMD     =   0x13,
  DL_PHYCHAN_CFG_DONE_CMD     =   0x14,

  /* Local Commands related to DL/UL Phy Chan adds */
  UL_PHYCHAN_ADD_DONE_CMD     =   0x15,
  DL_PHYCHAN_ADD_DONE_CMD     =   0x16,

  UL_CELL_TRANS_DROP_DONE_CMD =   0x18,
  DL_CELL_TRANS_DROP_DONE_CMD =   0x19,

  /* Indicates sequencer has been aligned to new cell timing
   * during cell transition.
   */
  SEQ_ALIGN_CELL_TRANS_DONE   =   0x1A,

  /* Local command to trigger Demod to measure SFN-CFN observed time
     difference.
   */
  DL_MEAS_SFN_CFN_DIFF_CMD    =   0x1B,
  DL_MEAS_SFN_SFN_DIFF_CMD    =   0x1C,

  /* Local Command sent by modules to signal
   * end of cleanup for the module.
   */
  MODULE_CLEANUP_DONE_CMD     =   0x1D,

  RF_TUNE_DONE_CMD            =   0x1F,

  DL_PHCH_CFG_DONE_CMD        =   0x20,
  DL_ACQ_PULLIN_CMD           =   0x22,
  DL_ACQ_PULLIN_EVT_CMD       =   0x23,
  DL_ACQ_PULLIN_DONE_CMD      =   0x24,
  DL_ACQ_BCH_DECODE_DONE_CMD   =   0x25,
  /* Used to send BCCH BLER status to DL*/
  DL_PCCPCH_WEAK_IND_CMD       =   0x26,
  SRCH_SFN_WRC_DIFF_DONE_CMD   =   0x27,
  DL_BCH_CRC_STATUS_CMD       =   0x28,
  DL_TRBLK_CRC_STATUS_CMD     =   0x29,

  BCH_CFN_INVALID_CMD         =   0x2A,
  MDSP_SYNC_TIMEOUT_CMD       =   0x2B,

  UL_PHYCHAN_DPCH_LOG_DONE_CMD=   0x2C,
  UL_PRACH_LOG_DONE_CMD       =   0x2D,

  DRX_LOG_DONE_CMD            =   0x2F,
  DRX_SLEEP_CMD               =   0x30,

#ifdef FEATURE_L1_LOG_ON_DEMAND
  LOG_ON_DEMAND_CMD           =   0x31,
#endif

  UL_RACH_TIMING_DEBUG_CMD    =   0x32,

  /* The primordial no-op or unknown op */
  L1_PHYCHAN_NOOP_DONE_CMD    =   0x33,

  /* signals rach isr done for l1m to proceed with channel setup */
  UL_RACH_DONE_CMD            =   0x34,

  /* signals SRCHFACH to calc meas oksn's and start meas */
  SRCHFACH_GET_CRNTI_DONE_CMD =   0x35,
  SRCH_FACH_QICE_DROP_CMD     =   0x36,

  /* Local command to evaluate the inter-rat events */
  EVALUATE_INTERRAT_EVENTS_CMD =  0x37,

  /* Signals that HSDPA setup has completed */
  L1_HS_CHAN_CFG_DONE         =   0x38,

  /* Call functions to submit any accumulated log packets in HS */
  L1_HS_SUBMIT_LOG            =   0x39,

  QUALMEAS_CRC_UPDATE_CMD     = 0x3A,

  MDSP_CMD_LOG_SUBMIT         =   0x3B,

  /*Command to log agc*/
  L1_CMD_AGC_LOG              =   0x3C,

  DL_RXD_STATE_UPDATE_CMD     =   0x40,

  DL_RXD_HW_CB_CMD           =   0x41,
  RXD_MEAS_LOG_SUBMIT_CMD = 0x42,

  /* Signals that the SIB timer expired */
  SIB_TIMER_EXPIRY_CMD        =   0x43,

  E_DL_CHAN_CFG_DONE_CMD    =   0x44,
  E_DL_FRM_BDRY_DONE_CMD    =   0x45,
  E_DL_PHYCHAN_DONE_CMD     =   0x46,
  E_UL_PHYCHAN_DONE_CMD     =   0x47,
  E_UL_DL_SETUP_DONE_CMD    =   0x48,
  E_UL_BUILD_SS_TABLES_CMD  =   0x49,
  E_UL_LOGGING_CMD          =   0x4A,
  E_DL_LOGGING_CMD          =   0x4B,


#ifdef FEATURE_WCDMA_TRM
  /* Signals that WL1 must go to L1M_WAIT_FOR_TRM_STOP state */
  TRM_STOP_CMD                =   0x4D,
#endif

  /* Call function for module cleanup done after HS teardown in L1 task context */
  L1_HS_MODULE_CLEANUP_AFTER_HS_TEARDOWN_CMD = 0x4E,


  /* Signals that WCDMA needs to set the tcxo restriction */
  TCXO_SET_RESTRICTION_CMD       =   0x55,

  /* DRX_MANAGER_CMD for commands from other subsystems, like SRCH.
  ** DRX_ENTITY_CMD for commands for a particular channel state machine. */
  DRX_MANAGER_CMD             =   0x56,
  DRX_ENTITY_CMD              =   0x57,

#if defined(FEATURE_WCDMA_TRM)
  /* Signals that RxD can be enabled since we just got control of the
  ** secondary antenna */
  TRM_ENABLE_RXD_CMD             =   0x58,
#endif

#ifdef FEATURE_WCDMA_FS_FALSE_PEAK_ELIM_OPT
  /* used to get the mdsp result for freq scan */
  SRCH_FS_PROC_DONE              =0x5A,
#endif

  /* Removed CMDs 0x5B, 0x5C as they are not needed for 
   * offline targets. These CMDs can be reused.*/

  /* Used to evaluate events 6A to 6E in DCH state when internal meas
     are configured by n/w. Posted in wenc.c every 10msec */
  L1_INTERNAL_MEAS_EVT_6A_TO_6E_EVAL_CMD = 0x5D,

  SRCHCR_GSM_CLK_ENABLED_IN_DRX   = 0x5E,

  DL_MCAL_CFG_CMD             = 0x60,
  DL_CELL_CFG_DONE_CMD        = 0x61,
  DL_CELL_TD_DET_NOTIFY_CMD   = 0x62,

  L1_CME_CTRL_CMD             = 0x63,
  L1_CME_SUBMIT_EQ_LOG        = 0x64,
  L1_CME_SUBMIT_EQ_REG_LOG    = 0x65,
  L1_CME_SUBMIT_SCHIC_LOG     = 0x66,
  L1_CME_SUBMIT_QICE_SFC_LOG  = 0x67,
  L1_CME_SUBMIT_QICE_FFINV_LOG = 0x68,
  L1_CME_SUBMIT_QICE_DEBUG_LOG = 0x69,
  WL1_DEMODDRV_DSD_DONE_CMD   = 0x6A,
  L1_CM_CME_CMD               = 0x6B,
  /* command for logging UL TX RF log packet */
  UL_TX_RF_LOGGING_CMD        = 0x6C,
  DL_TRI_FING_AVAIL_DONE_CMD  = 0x6D,
  DL_TRI_RXD_TRANS_CMD        = 0x6E,

  /* Search Abort done Signal */
  SRCH_ABORT_DONE_CMD         = 0x6F,

  /*Command for indicating the start of a CFN to NSCH Manager*/
  DL_NSCH_CFN_UPD_CMD       =   0x70,

  /* Post a local command so that the srch_cleanup done command and cell_de_cfg is called in TASK context */
  SRCH_CLEANUP_DONE_CMD     = 0x71,
  /* local command to initiate dc drop after hs channel on dc is dropped*/
  L1_HS_DC_CHAN_DROP_DONE_CMD = 0x72,
  /*to signal dc cfg done */
  L1_DC_CFG_DONE = 0x73,
  /* to signal cleanup on hold*/
  SRCH_CLEANUP_ON_HOLD      = 0x74,

  L1_R99_RL_DROP_DONE_CMD = 0x75,

  L1_DC_START_WAIT_FOR_RF_TUNE = 0x76,

  L1_CM_FREEZE_NORMAL_FRAME_CMD = 0x77,

  L1_HS_DEREGISTER_CLIENT_CMD = 0x78,

  #if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
  SRCH_LTE_SRCH_DONE_CMD    = 0x79,
  SRCH_LTE_INT_CNF_RCVD     = 0x7A,
  #endif 
  L1_DTX_DONE_CMD             =   0x7B,

    /* Command for logging the FW DTX log packet */
  CPC_DTX_FW_LOGGING_CMD   = 0x7C,
  /*L1 local command to pre-compute the DPCCH burst
  ** burst position for cycle 1 and 2 and ETFCI selection
  ** instances MAC DTX cycle*/
  L1_CPC_DTX_BUILD_BURST_POS_TABLES_CMD =   0x7D,

  /* Creating local command to submit the configuration
  ** log packet*/
  L1_CPC_DTX_CONFIG_LOG_PKT_CMD  = 0x7E,

  /* Command for logging the UL MTD log packet */
  L1_MTD_LOGGING_CMD = 0x7F,

#if defined(FEATURE_WCDMA_L1_ZI_OPT)
  L1_EULENC_ERAM_DUMP_CMD  = 0x80,
#endif

  /* This command is used to log all R99 UL Log Packets in Task context */
  UL_LOGGING_CMD = 0x81,

  L1_INTER_RAT_PERIODIC_MEAS = 0x82,
#ifdef FEATURE_WCDMA_CM_LTE_SEARCH  
  L1_EUTRA_INTER_RAT_PERIODIC_MEAS = 0x83,  
#endif

#ifdef FEATURE_WCDMA_CPC_DRX
  SRCH_ABORT_ALL                         = 0x84,
  /*drx log process cmd*/
  L1_DRX_LOG_PROCESS_CMD                 = 0x85,
  L1_DRX_DONE_CMD                        = 0x86,
  L1_DRX_PROCESS_MDSP_EVT_LOG_CMD        = 0x87,
  L1_CPCDRX_CFG_LOG_CMD                  = 0x88,
  /* command to start CPC DRX search or update DL DRX scheduler */
  SRCH_CONFIG_CPC_DRX                     = 0x89,
#endif

  /* This command is used used to process WCDMA neighbor cell info 
   * for feature FEATURE_WCDMA_GET_RAT_MEAS_INFO
   */ 
  L1_PROCESS_CMAPI_WCDMA_SIG_INFO = 0x8A,

  /* This command is used to resume the DC carrier config
    once the cell_db_idx is available */
  L1_DC_CFG_RESUME = 0x8B,

  L1_SRCHCR_RESUME_IDLE_SEARCH = 0X8C,

  /* Command to notify L1M that EUL drop is done time to MCPM Relinquish */
  E_UL_DROP_DONE_CMD = 0x8E,

  /* Command to notify L1M that there are no R99 TrCh with Turbo coding,
     time to Relinquish R99_DATA client */
  L1_MCPM_R99_DATA_RELINQUISH_CMD = 0x8F,

  RXD_SUBMIT_ARD_LOG = 0x91,

  SRCH_RESCHEDULE_TXD_DET_CMD = 0x92,

  WL1_EDRX_WAKEUP_DONE                   = 0x93,

  WL1_EDRX_UPD_WKUP_TIME                 = 0x94,

  WL1_EDRX_LOG_SUBMIT                    = 0x95,

  WL1_EDRX_SRCH_DONE_CMD                 = 0x96,

  WL1_EDRX_CME_TRIGGER                   = 0x97,

  L1_DC_RESUME_RF_CFG_AFTER_CM           = 0x98,

  /* Debug code for Issue dbg*/
  L1_TASK_STUCK_ISSUE_DBG = 0x99,

  L1_FET_STATUS_UPDATE = 0x9A,

  L1_DEMBACK_EXT_SUBMIT_LOG = 0x9B,

  UL_PHYCHAN_HSRACH_CFG_DONE = 0x9C,

  L1_HS_RACH_LOCAL_CMD = 0x9D,

  UL_HS_RACH_MAC_TX_IND = 0x9E,

  SAMPSERV_MGR_CMD = 0x9F,

  L1_CM_FINISH_CLEANUP_CMD = 0xA0,

  DRX_DPC_SYNC_CMD = 0xA1,

  WL1_CONN_STATE_MODULE_CLEANUP_DONE_CMD = 0xA2,

  DL_TRI_CB_CMD= 0xA3,

  WL1_ATUNER_PROCESSING = 0xA4,

  L1M_SETUP_DONE_CMD = 0xA5,

  NOTIFY_WL1_DL_CARRCFG_ON_OPS_DONE_CMD = 0xA6,

  SEC_UL_PHYCHAN_DONE_CMD = 0xA7,

  SEC_E_UL_PHYCHAN_DONE_CMD = 0xA8,

  SEC_E_DL_PHYCHAN_DONE_CMD = 0xA9,

#ifdef FEATURE_WCDMA_DC_HSUPA 

  E_UL_DC_DEACT_MCPM_REQ_CMD = 0xAA,

  UL_NEW_MPR_LOGGING_CMD = 0xAB,

  UL_POW_ALLOC_LOGGING_CMD = 0xAC,

#endif /* FEATURE_WCDMA_DC_HSUPA */

  WCDMA_PERIODIC_STAT_LOG = 0xAE,
 
  SRCH_FACH_RESEL_EVAL = 0xAF,
  #ifdef FEATURE_WCDMA_DC_HSUPA 
  DC_HSUPA_ACTIVATION_LOG_CMD = 0xB0,
  #endif
  
#ifdef FEATURE_WCDMA_ANT_SWITCH_SUPPORT
  /* local command id for processing the antenna switch command */
  L1_ANT_SWITCH_CMD = 0xB1,
  L1_ANT_SWITCH_TYPE2_CMD = 0xB2,
  L1_ANT_SWITCH_EVAL_FOR_RACH_CMD = 0xB3,
  L1_ANT_SWITCH_CONFIG_LOG_PKT_CMD = 0xB4,
  L1_ANT_SWITCH_METRICS_LOG_PKT_CMD = 0xB5,
#endif

  WL1_WRM_ASYNC_CALLBACK_CMD = 0xB6,

  #ifdef FEATURE_DUAL_SIM
  WL1_DRX_WKUPMGR_CANCEL_PROCESS_CMD = 0xB7,
  #endif

  #if defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)
  WL1_QTA_TIMER_EXPIRED_CMD              = 0xB8,
  #endif /* FEATURE_DUAL_SIM */

  #ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
  WL1_MULTISIM_CXM_START_RX_FILTERING = 0xB9,
  #endif /* FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW */

  SRCH_PROC_RXD_UPD_CB_CMD = 0xBA,

  L1_RXD_CTRL_CMD = 0xBB,

  #ifdef FEATURE_WCDMA_SAR_TX_PWR_FILTER
  WL1_UL_SAR_DO_FILTERING = 0xBC,
  #endif /* FEATURE_WCDMA_SAR_TX_PWR_FILTER */

  WL1_SFN_CFN_SANITY_CHECK_CMD = 0xBD,

  #ifdef FEATURE_WCDMA_DC_HSUPA 
  DC_HSUPA_DEACTIVATION_LOG_CMD = 0xBE,
  #endif

  DRX_RFM_POWER_CLOCK_ON = 0xBF,
    
  /* Local cmd for handling RFM PREP in L1 task context at the time
  ** of wakeup from true sleep.*/
  DRX_SLEEP_RFM_PREP_START_CMD = 0xC0,

  /* This local cmd will be posted by search procedure when some 
     activity is done and needs to connect back to scheduler */
  WSRCH_ACQ_SPLIT_ACTION_DONE = 0xC1,
#ifdef FEATURE_DUAL_SIM
  WL1_SEND_FAKE_SIB_SCHED_IND = 0xC2,
#endif /*end FEATURE_DUAL_SIM */

  L1_SCCH_ORD_DONE_CMD = 0xC3,

  WL1_RESUME_CPHY_CMD = 0xC4,

  #ifdef FEATURE_QTA
  /* local command to handle unlock cancel event */
  WL1_QTA_UNLOCK_CANCEL_CMD = 0xC5,
  #endif

  L1_DSR_CONTROLLER_CMD = 0xC6,
  
  /* Local command requesting for secondary antenna */
  WRM_REQ_SEC_ANTENNA = 0xC7,

  L1_RXD_ACQ_BCH_TRIGGER_CMD = 0xC8,

  WL1_CME_SCCH_ORD_CMD = 0xC9,

  L1_PERIODIC_MEAS_CMD = 0xCA,
  
  /*TxD DSD Local CMD*/
  DL_CELL_CFG_TXD_DSD_CMD = 0xCB,

  /* local cmd to tear down diversity chain */
  DL_TEARDOWN_DIV = 0xCC,

  #ifdef FEATURE_WCDMA_ANT_SWITCH_SUPPORT
  /*DSDA Passive switch Command from TRM*/
  L1_ANT_SWITCH_TRM_PASSIVE_SWITCH_CMD = 0xCD,
  #endif 

  WRM_HANDLE_PENDING_CMD = 0xCE,

  SRCH_NOTIFY_CELL_DECFG_DONE = 0xCF,

  /* Local cmd for Exec and Abort for RF temp Comp */
  WL1_RFM_EXEC_TEMP_COMP  = 0xD0,

/*Lcoal command for NBR REJ*/
  SRCH_NB_REJ_DONE_CMD = 0xD1,

  #ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
  WL1_MULTISIM_CXM_LOG_BLANKING_INFO = 0xD2,
  #endif /* FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW */

  RXD_NOTIFY_ON_SCCH_ORD_RECEPTION = 0xD3,

  WL1DLDRXCTRL_FREEZE_UNFREEZE_FOR_SCCH_ORDER = 0xD4,

#ifdef FEATURE_DUAL_SIM
  DRX_PREP_FOR_CALL_EST = 0xD5,
#endif

  WL1DLDRXCTRL_NOTIFY_CPC_STATUS_TO_CLIENTS = 0xD6,

/* This command is used to invoke scheduler to complete mode change
     when FW aborts the on-going search */
  SRCHSCHED_COMPLETE_MODE_CHNG = 0xD7,

/* Local command to invoke FMO scheduler */
  SRCHFACH_EDRX_RESCHEDULE_FMO_SRCH = 0xD8,
  
#ifdef FEATURE_DUAL_SIM
  DRX_REQ_FAKE = 0xD9,
#endif

  DL_CELL_SHADOW_CELL_UPD_CMD = 0xDA,

  L1_DPCH_DEMOD_ENH_SUBMIT_LOG = 0xDB,

#if defined (FEATURE_WCDMA_QSH_DUMP) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC)
  WL1_PROCESS_QSH_CMD = 0xDC,
#endif /* defined (FEATURE_WCDMA_QSH_DUMP) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC) */

  #ifdef FEATURE_WCDMA_CFCM
  /* Local command to handle CFCM cb. */
  L1_HANDLE_CFCM_CB = 0xDD,
  #endif

  #ifdef FEATURE_WCDMA_EFACH_MSIM 
  WL1_UL_TX_ENABLE_AFTER_QTA = 0xDE,
  WL1_EDRX_QTA_ENTITY_SLEEP = 0xDF,
  #endif /* FEATURE_WCDMA_EFACH_MSIM */

  WSLEEP_GUARD_TIMER_EXPIRY = 0xE0,

  L1_CM_W2L_CME_CMD = 0xE1,

  WRM_SEC_UNLOCK_TIMER_EXPIRED_CMD = 0xE2,

  WL1_RESUME_ACQ_CNF_CMD = 0xE3,

  EUL_Q6_SCALING_CMD = 0xE4,

  HS_CM_INFO_ON_HS_CFG = 0xE5,

  #ifdef FEATURE_WCDMA_DL_QUAL_MEAS_REPORTING
  DL_QUAL_MEAS_POST_REPORT = 0xE6,
  #endif

  WL1_TRI_SCCH_DEACT_DUAL_BAND = 0xE7,

  #ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
  CXM_PHYCHAN_UPDATE_CMD = 0xE8,
  #endif /* FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW */
  
  WL1_SCHED_UPDATE_IND = 0xE9,

  /* Local command to handle calling drx entity schd update */
  #ifdef FEATURE_WCDMA_CPC_DRX
  DL_DRX_CTRL_ENTITY_SCHED_UPDATE = 0xEA,
  #endif /*FEATURE_WCDMA_CPC_DRX*/

/* Local command to ensure rgs updates happen in wl1 task context */
#ifdef FEATURE_TMC_TCXOMGR
  WL1_UPDATE_RGS_CMD = 0xEB,
#endif

  #ifdef FEATURE_WCDMA_MDSP_RECOVERY
  /* Local command to handle MSDP recovery */
  L1_HANDLE_MDSP_RECOVERY = 0xEC,
  #endif

  WSLEEP_IDLE_QTA_NOTIFY_LOCAL_CMD = 0xED,

 #ifdef FEATURE_WCDMA_ANT_SWITCH_SUPPORT
  L1_ANT_SWITCH_UNSUSPEND_CMD = 0xEE,
 #endif

  DRX_CALL_NEXT_DRX = 0xEF,

  L1M_SEND_CPHY_CNF_CMD = 0xF0,

  HS_DL_APS_INFO_CMD = 0xF1,

  L1_FACH_RELEASE_SECONDARY_ANT = 0xF2,

  SRCH_UPDATE_MAAT_CMD = 0xF3,

  WL1_DS_PRIO_INV_TIMER_EXPIRED_CMD = 0xF4,

  L1_LOCAL_NUM_CMD = 0xF5
} l1_local_cmd_enum_type;

/*Enum type to indicate the carrier indices*/
typedef enum {
  DL_TRI_PRI_CARR_ID
#ifdef FEATURE_WCDMA_DC_HSDPA
  ,DL_TRI_SEC_CARR_ID
#ifdef FEATURE_WCDMA_3C_HSDPA
  ,DL_TRI_TER_CARR_ID
#endif /* FEATURE_WCDMA_3C_HSDPA */
#endif /* FEATURE_WCDMA_DC_HSDPA */
,DL_TRI_MAX_NUM_CARR
} dl_tri_carr_id_enum_type;

#if defined(FEATURE_WCDMA_3C_HSDPA)
#define WL1_MAX_NUM_CARRIER_IDX 3
#define WL1_MAX_VALID_CARRIER_IDX 2
#elif defined(FEATURE_WCDMA_DC_HSDPA)
#define WL1_MAX_NUM_CARRIER_IDX 2
#define WL1_MAX_VALID_CARRIER_IDX 1
#else
#define WL1_MAX_NUM_CARRIER_IDX 1
#define WL1_MAX_VALID_CARRIER_IDX 0
#endif

#define WL1_DL_BAND_0 0
#define WL1_DL_BAND_1 1
#define WL1_MAX_NUM_BAND 2

typedef enum
{
  WL1_PRI_UL_CARR_IDX =0
#ifdef FEATURE_WCDMA_DC_HSUPA 
  ,WL1_SEC_UL_CARR_IDX
#endif
  ,WL1_MAX_UL_CARR_IDX
}wl1_ul_carr_id_enum_type;

#define WL1_MAX_NUM_CARR_DC_ACTIVE 2
#define WL1_MAX_NUM_CARR_DC_INACTIVE 1


/* Header type to be included by all Local Commands */
typedef struct
{
  /* Queue Link Type */
  q_link_type             link;

  /* enumerated command value */
  l1_local_cmd_enum_type  cmd_code;

} l1_local_cmd_hdr_type;

/*Enum type to indicate the enhanced state for L1.
   DO NOT CHNAGE THE ORDER OF THIS ENUM. If you want to 
   add more fileds please add at the end and request QXDM folks
   to support the new addition*/
typedef enum {
  /*DO NOT CHNAGE THE ORDER OF THIS ENUM*/
  WCDMA_ENH_L1_IDLE,

  WCDMA_ENH_L1_CELL_PCH,

  WCDMA_ENH_L1_URA_PCH,

  WCDMA_ENH_L1_FACH,

  WCDMA_ENH_L1_ENH_FACH,

  WCDMA_ENH_L1_EDRX,

  WCDMA_ENH_L1_EDRX_FS,

  WCDMA_ENH_L1_DCH

} wcdma_enh_l1_state_enum_type;

/* Enum type to indicate the HSRACH state. L1 reports these events to QXDM
 * subsystems.
 */
typedef enum {
  /* Do not change the order of this enum */
  WCDMA_HSRACH_NO_OP,

  WCDMA_HSRACH_START,

  WCDMA_HSRACH_STOP,

  WCDMA_HSRACH_STOP_START
  
} wcdma_hsrach_state_enum_type;


/*=========================================================================**
** WCDMA L1 reports various events to the QXDM, as do lots of other        **
** subsystems.  These are high priority, small data packets used for       **
** post-processing analysis and debugging of phone behavior.  The data     **
** packet event structures used by WCDMA L1 are defined here.              **
** Event enumerated types are in event_defs.h.                             **
**                                                                         **
** These event structures are often instantiated on the stack.  Because    **
** of this, they should be relatively small.  If for some reason they      **
** need to grow a significant amount, they should be moved from stack      **
** space to heap space.                                                    **
**=========================================================================*/

/***************************
**  EVENT_WCDMA_L1_STATE  **
****************************/
typedef PACKED struct PACKED_POST
{
  uint8  l1m_state;  /* Corresponds to l1m_state_enum_type. */
}
wcdma_layer1_state_change_evt_type;

/***********************
**  EVENT_WCDMA_ASET  **
***********************/
typedef PACKED struct PACKED_POST
{
  uint8   num_cells;                /* Numer of cells in the active set.  */
  uint16  cell_psc[L1_ASET_MAX];  /* Array of primary scrambling codes. */
}
wcdma_layer1_aset_update_evt_type;

/*******************************
**  EVENT_WCDMA_PRACH  **
*******************************/
typedef PACKED struct PACKED_POST
{
  int8   rf_tx_pwr_dbm;  /* Last RF TX power for RACH power in dBm       */
  uint8  num_preambles;  /* Number of preambles transmitted.             */
  uint8  aich_status;    /* Corresponds to "l1_prach_status_enum_type".  */
}
wcdma_layer1_prach_evt_type;

/*****************************
**  EVENT_WCDMA_L1_SUSPEND  **
*****************************/
typedef PACKED struct PACKED_POST
{
  uint8   last_cfn;  /* CFN once WCDMA L1 is SUSPENDed. */
  uint16  last_sfn;  /* SFN once WCDMA L1 is SUSPENDed. */
}
wcdma_layer1_suspend_evt_type;

/****************************
**  EVENT_WCDMA_L1_RESUME  **
****************************/
typedef PACKED struct PACKED_POST
{
  uint8   first_cfn;             /* CFN after phychan(s) RESUME'd. */
  uint16  first_sfn;             /* SFN after phychan(s) RESUME'd. */
  uint16  num_frames_suspended;  /* Number of frames L1 SUSPENDED. */
}
wcdma_layer1_resume_evt_type;

/*****************************
**  EVENT_WCDMA_L1_STOPPED  **
*****************************/
typedef PACKED struct PACKED_POST
{
  uint16  sfn;                   /* SFN at STOP time.                      */
  uint16  num_frames_suspended;  /* Number of frames from SUSPEND to STOP. */
}
wcdma_layer1_stopped_evt_type;

#define  RESEL_TYPE_INTRA_FREQUENCY  0
#define  RESEL_TYPE_INTER_FREQUENCY  1

/**************************************************
**  EVENT_WCDMA_TO_WCDMA_RESELECTION_VER2_START  **
***************************************************
** This event is identical to the previous event, except the PSC field is
** 16 bits instead of only 8.  PSC can overflow 8 bits.  For awhile WL1 will
** generate both events, until tools and UE software are synced up.  At that
** time the old event can be removed. */
typedef PACKED struct PACKED_POST
{
  uint8   resel_type;  /* RESEL_TYPE_INTRA_FREQUENCY or          **
                       ** RESEL_TYPE_INTER_FREQUENCY.            */
  uint16  uarfcn;      /* Target UMTS channel frequency number.  */
  uint16  target_psc;  /* Target cell's primary scrambling code. */
  uint16  start_sfn;   /* SFN at reselection start time.         */
  uint32  csg_id;      /*CSG-Identity of target cell. Non-CSG value 0xFFFFFFFF*/
}
wcdma_layer1_w_to_w_resel_ver2_start_evt_type;

/*******************************************
**  EVENT_WCDMA_TO_WCDMA_RESELECTION_END  **
*******************************************/

/* Set "failure_reason" to this on SUCCESS. */
#define FAILURE_REASON_NOT_APPLICABLE  0xFF

typedef PACKED struct PACKED_POST
{
  uint16  end_sfn;         /* SFN at reselection end time.     */
  uint8   status;          /* TRUE = SUCCESS, FALSE = FAILURE. */
  uint8   failure_reason;  /* TBD.                             */
}
wcdma_layer1_w_to_w_resel_end_evt_type;

/****************************
**  EVENT_WCDMA_DRX_CYCLE  **
****************************/
typedef PACKED struct PACKED_POST
{
  int8   pich_pwr_offset;  /* Tx pwr on PICH minus pwr of primary CPICH     **
                           ** (-10 dB to +5 dB).                            */
  uint8  Np;               /* Paging indicators (18, 36, 72, 144) per PICH  **
                           ** frame.  Bits per paging indicator = 288 / Np. */
  uint8  drx_cycle_len;    /* (3 to 9), where (2 ^ drx_cycle_len) is the    **
                           ** DRX cycle length in 10 msec frames.           */
}
wcdma_layer1_drx_data_evt_type;

/***********************************
**  EVENT_HS_SERVING_CELL_CHANGE  **
***********************************/
typedef PACKED struct PACKED_POST
{
  uint16    old_uarcfn;       /*  Frequency of the previous HS serving cell */
  uint16    new_uarcfn;       /*  Frequency of the new HS serving cell */
  uint16    old_psc;          /*  PSC of the previous HS serving cell */
  uint16    new_psc;          /*  PSC of the new HS serving cell */
  boolean   hrnti_change;     /*  Flag indicating a change in H-RNTI */
  uint8     repoint_type;     /*  Type of repoint - hs_repoint_enum_type */
}
wcdma_layer1_hs_serving_cell_change_evt_type;

/***********************************
**  EVENT_HS_DSCH_STATUS  **
***********************************/
typedef PACKED struct PACKED_POST
{
  uint8     ops;              /* HS op command type: hsdpa_setup_ops_enum_type */
  uint16    psc;              /* PSC of the HS cell */
}
wcdma_layer1_hs_dsch_status_evt_type;

#if defined(FEATURE_WCDMA_MIMO) || defined(FEATURE_HSDPA_MAC_EHS) || \
    defined(FEATURE_WCDMA_64QAM)
/***********************************
** EVENT_HSPA_PLUS_CFG            **
***********************************/
typedef PACKED struct PACKED_POST
{
  /* MAC-hs or MAC-ehs */
  boolean mac_ehs_configured;
  /* 64 QAM configured or not */
  boolean hs_64_qam_configured;

  /* MIMO action */
  uint8 l1_mimo_action; 
  /* MIMO N_M ratio */
  uint8 mimo_n_m_ratio;
  /* Pilot configuration */
  uint8 second_cpich_pattern;
  /* S-CPICH OVSF */
  uint8 channelisation_code ;
}
wcdma_layer1_hspa_plus_cfg_evt_type;
#endif

/***********************************
** EVENT_HSPAPLUS_DC              **
***********************************/
typedef PACKED struct PACKED_POST
{
  /* Bit 0:3 Version 
     Bit 4   Mac-hs/Mac-ehs
     Bit 5:7 Reserved
   */
  uint8 version;
  /* Scrambling code of primary carrier
     Bit 0:3  SSC
     Bit 4:12 PSC
   */
  uint16 scr_code_primary_carrier;
  /* Frequency of primary carrier */
  uint16 uarfcn_primary_carrier;
  /* Call type of primary carrier
     Bit 0   64QAM status
     Bit 1   TBS table used
     Bit 2:3 MIMO action
     Bit 4   P-CPICH TD status
     Bit 5:6 HS-PDSCH TD status
     Bit 7   Reserved
   */
  uint8 call_type_primary_carrier;
  /* MIMO parameters of primary carrier
     Bit 0:3   MIMO N_M ratio
     Bit 4     MIMO Pilot configuration
     Bit 5:12  S-CPICH channelization code (ovsf code for MIMO S-CPICH)
     Bit 13:15 Reserved
   */
  uint16 mimo_info_primary_carrier;
  /* DC action */
  uint8 dc_action;
  /* Scrambling code of secondary carrier
     Bit 0:3  SSC
     Bit 4:12 PSC
   */
  uint16 scr_code_secondary_carrier;
  /* Frequency of secondary carrier */
  uint16 uarfcn_secondary_carrier;
  /* Call type of secondary carrier
     Bit 0   64QAM status
     Bit 1   TBS table used
     Bit 2:3 MIMO action
     Bit 4   P-CPICH TD status
     Bit 5:6 HS-PDSCH TD status
     Bit 7   Reserved
   */
  uint8 call_type_secondary_carrier;
  /* MIMO parameters of secondary carrier
     Bit 0:3   MIMO N_M ratio
     Bit 4     MIMO Pilot configuration
     Bit 5:12  S-CPICH channelization code (ovsf code for MIMO S-CPICH)
     Bit 13:15 Reserved
   */
  uint16 mimo_info_secondary_carrier;
}
wcdma_layer1_hspa_plus_dc_cfg_evt_type;

#ifdef FEATURE_WCDMA_DC_HSDPA
/***********************************
** EVENT_HS_SCCH_ORDER_CFG        **
***********************************/
typedef PACKED struct PACKED_POST
{

/*
Bit 0:1 1st Secondary carrier
             0 - No order
             1 - start
             2 -  stop
             3 - Continue
Bit 2:3 2nd Secondary Carrier
             0 - No order
             1 - start
             2 -  stop
             3 - Continue
Bit 4:5 3rd Secondary Carrier
             0 - No order
             1-Start
             2-Stop
             3-Continue
     Bit 6:7 DC HSUPA order
            0 - No order
            1 -  start
            2 -  stop
            3 -Continue 
     Bit 8:9 DTX order
             0 - No order
             1 -  Start
             2 -  Stop
             3 - Continue
     Bit 10:11 DRX order
             0 - No order
             1 -  Start
             2 -  Stop
             3 - Continue
     Bit 12:15 - Reserved
   */
  uint16 order;
}
wcdma_layer1_hs_scch_order_evt_type;
#endif /* FEATURE_WCDMA_DC_HSDPA */

/* Delete after MST scripts are updated */
typedef PACKED struct PACKED_POST
{

/*
Bit 0:1 1st Secondary carrier
             0 - No order
             1 - start
             2 -  stop
             3 - Continue
Bit 2:3 2nd Secondary Carrier
             0 - No order
             1 - start
             2 -  stop
             3 - Continue
Bit 4:5 3rd Secondary Carrier
             0 - No order
             1-Start
             2-Stop
             3-Continue
     Bit 6:7 DC HSUPA order
            0 - No order
            1 -  start
            2 -  stop
            3 -Continue 
     Bit 8:9 DTX order
             0 - No order
             1 -  Start
             2 -  Stop
             3 - Continue
     Bit 10:11 DRX order
             0 - No order
             1 -  Start
             2 -  Stop
             3 - Continue
     Bit 12:15 - Reserved
   */
  uint8 version;
  uint8 order;
}
wcdma_layer1_hs_scch_order_evt_type_old;

/************************************
** EVENT_EUL_SERVING_CELL_CHANGE  **
*************************************/

typedef PACKED struct PACKED_POST
{
  uint16 new_psc;
  uint8 action;
  uint8 type;
}
wcdma_layer1_eul_serv_cell_change_evt_type;

/***************************************
** EVENT_EUL_RECONFIG_OR_ASU **
****************************************/
typedef PACKED struct PACKED_POST
{
  uint8 action;
  uint8 eul_as_size;
  uint16 cell_psc[4];
}
wcdma_layer1_eul_reconfig_or_asu_evt_type;

/***************************************
** EVENT_EUL_TTI_RECONFIG **
****************************************/
typedef PACKED struct PACKED_POST
{
  uint8 new_tti;
}
wcdma_layer1_eul_tti_reconfig_evt_type;

#ifdef FEATURE_WCDMA_CPC_DTX
/***************************************
** EVENT_DTX_ACTION **
****************************************/
typedef PACKED struct PACKED_POST
{
  uint8 dtx_action;
  uint8 dtx_activation_time;
  uint8 dtx_enabling_delay;
}
wcdma_layer1_cpc_dtx_action_evt_type;
#endif

#ifdef FEATURE_WCDMA_L1_PSC_SCANNER
/* Define the Event for the PSC scanner */
typedef PACKED struct PACKED_POST
{
  uint8   scanner_state;  /* state of the psc scanner ON/OFF . */
}
wcdma_psc_scanner_state_evt_type;
#endif

/***************************************
** EVENT_EFDPCH_ACTION **
****************************************/
typedef PACKED struct PACKED_POST
{
  uint8 efdpch_action;
  uint8 efdpch_slot_format;
  uint8 efdpch_ber_target;
}
wcdma_layer1_efdpch_action_evt_type;

#ifdef FEATURE_WCDMA_HS_FACH_DRX
/***************************************
** EVENT_EDRX_ACTION **
****************************************/
typedef PACKED struct PACKED_POST
{
 uint8 action;
 uint16 dhrnti;
 uint16 drx_cycle_len_fr;
 uint16 rx_burst_fr;
 uint16 t321_drx_interruption;
}
wcdma_layer1_edrx_action_evt_type;

/***************************************
** EVENT_WCDMA_ENH_L1_STATE **
****************************************/
typedef PACKED struct PACKED_POST
{
 uint8 l1_state;
}
wcdma_enh_layer1_state_evt_type;

#endif

#ifdef FEATURE_WCDMA_HS_RACH
/***************************************
** EVENT_WCDMA_HS_RACH_OP **
****************************************/
typedef PACKED struct PACKED_POST 
{
  uint8 op;
}
wcdma_hsrach_layer1_op_evt_type;
#endif

/***************************************
** EVENT_WCDMA_CM_STATE_CHANGE **
****************************************/
typedef PACKED struct
{
  uint8 active;
}
wcdma_layer1_cm_state_change_evt_type;

/***************************************
** Carrier indices **
****************************************/
#define CARR_0   0    /* Primary carrier    */
#define CARR_1   1    /* Secondary carrier  */
#define CARR_2   2    /* Tertiary carrier   */
#define CARR_3   3    /* Quaternary carrier */
#define NUM_CARR 3    /* The number of carriers to support */
#define MAX_CARR 4    /* The maximum number of supported carriers */
#define NO_CARR  0xff /* No carrier */

/***************************************
** Carrier indices **
****************************************/
#define BAND_0   0   /* Primary Band */
#define BAND_1   1   /* Secondary Band */


/*-------------------------------------------------------------------------
           ENUMERATION FOR SEARCHER QUEUE
-------------------------------------------------------------------------*/
typedef enum
{
  SRCH_S1_Q,
  SRCH_NASTT_PN_Q,
  SRCH_NUM_Q
} srch_intraf_queue_enum_type;

#if defined(FEATURE_WCDMA_16_QAM) && defined(FEATURE_WCDMA_REL7_EDPCCH_POWER_BOOST)
/***************************************
** EVENT_WCDMA_EUL_REL7_FEAT_EVT_OP **
** bits 0  -  1 : 16QAM ACTION
                    00 => NO-OP
                    01 => START
                    10 => STOP
                    11 => CONT
** bits 2       : 16QAM STATE
                    0 => DISABLED
                    1 => ENABLED
** bits 3  -  4 : ETFCI TABLE IDX
                    00 => 0
                    01 => 1
                    10 => 2
                    11 => 3
** bits 5       : AG TABLE IDX
** bits 6       : SG TABLE IDX
** bits 7       : EXTRAPOLATION or INTERPOLATION
                    0 => EXTRAPOLATION
                    1 => INTERPOLATION
** bits 8  - 15 : ETFCI BOOST VALUE
** bits 16 - 19 : DELTA T2TP
** bits 20 - 23 : MAX SPREADING FACTOR CFGD
** bits 24      : EUL TTI
****************************************/
typedef PACKED struct PACKED_POST {
  uint32 eul_rel7_feat_evt_bmsk;
} wcdma_eul_rel7_feat_layer1_op_evt_type;
#endif /* defined(FEATURE_WCDMA_16_QAM) && defined(FEATURE_WCDMA_REL7_EDPCCH_POWER_BOOST) */

#endif /* L1DEF_H */

/****************************************************************************
** WL1_MSGx - Macros only take non-string parameters (like %d, not %s).
**
** Avoiding string support (%s) here because of excessive CPU overhead.
**
** Options for LEVEL:
** - LOW, MED, HIGH, ERROR, FATAL
**
** Example:
** - WL1_MSG2(ERROR, "delta_vco_temp (%d) > 0.5ppm, bounded to %d",
**            ABS(delta_vco_temp), tcxomgr.vco_temp_update_thresh);
**************************************************************************/
#ifdef FEATURE_DUAL_WCDMA
#define WL1_MSG0(LEVEL, FMT) \
      MSG_EXT(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT)

#define WL1_MSG1(LEVEL, FMT, A) \
       MSG_1_EXT(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A)

#define WL1_MSG2(LEVEL, FMT, A, B) \
      MSG_2_EXT(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B)

#define WL1_MSG3(LEVEL, FMT, A, B, C) \
      MSG_3_EXT(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C)

#define WL1_MSG4(LEVEL, FMT, A, B, C, D) \
      MSG_4_EXT(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C, D)

#define WL1_MSG5(LEVEL, FMT, A, B, C, D, E) \
      MSG_5_EXT(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C, D, E)

#define WL1_MSG6(LEVEL, FMT, A, B, C, D, E, F) \
      MSG_6_EXT(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C, D, E, F)

#define WL1_MSG7(LEVEL, FMT, A, B, C, D, E, F,G) \
      MSG_7_EXT(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C, D, E, F,G)

#define WL1_MSG8(LEVEL, FMT, A, B, C, D, E, F,G,H) \
      MSG_8_EXT(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C, D, E, F,G,H)

#define WL1_MSG9(LEVEL, FMT, A, B, C, D, E, F, G, H, I) \
      MSG_9_EXT(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C, D, E, F, G, H, I)

#define WL1_CUSTOM_MSG0(CUSTOM_LEVEL, FMT) \
do{  \
      MSG_EXT(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT);    \
  }while(0)

#define WL1_CUSTOM_MSG1(CUSTOM_LEVEL, FMT, A) \
do{  \
       MSG_1_EXT(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A);    \
  }while(0)

#define WL1_CUSTOM_MSG2(CUSTOM_LEVEL, FMT, A, B) \
do{  \
       MSG_2_EXT(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B);    \
  }while(0)

#define WL1_CUSTOM_MSG3(CUSTOM_LEVEL, FMT, A, B, C) \
do{  \
       MSG_3_EXT(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C);    \
  }while(0)

#define WL1_CUSTOM_MSG4(CUSTOM_LEVEL, FMT, A, B, C, D) \
do{  \
       MSG_4_EXT(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C, D);    \
  }while(0)

#define WL1_CUSTOM_MSG5(CUSTOM_LEVEL, FMT, A, B, C, D, E) \
do{  \
       MSG_5_EXT(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C, D, E);    \
  }while(0)

#define WL1_CUSTOM_MSG6(CUSTOM_LEVEL, FMT, A, B, C, D, E, F) \
do{  \
       MSG_6_EXT(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C, D, E, F);    \
  }while(0)

#define WL1_CUSTOM_MSG7(CUSTOM_LEVEL, FMT, A, B, C, D, E, F, G) \
do{  \
       MSG_7_EXT(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C, D, E, F, G);    \
  }while(0)

#define WL1_CUSTOM_MSG8(CUSTOM_LEVEL, FMT, A, B, C, D, E, F,G,H) \
do{  \
       MSG_8_EXT(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C, D, E, F, G, H);    \
  }while(0)

#define WL1_CUSTOM_MSG9(CUSTOM_LEVEL, FMT, A, B, C, D, E, F, G, H, I) \
do{  \
      MSG_9_EXT(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, 1, &wl1_diag_sub_id[wsub_id], FMT, A, B, C, D, E, F, G, H, I);    \
  }while(0);

#define WL1_EVENT_REPORT_PAYLOAD(event_id, length, payload) \
      event_report_payload_ext(event_id, length, payload, 1, &wl1_diag_sub_id[wsub_id]);

#else

#define WL1_MSG0(LEVEL, FMT) \
        MSG(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT)

#define WL1_MSG1(LEVEL, FMT, A) \
        MSG_1(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A)

#define WL1_MSG2(LEVEL, FMT, A, B) \
        MSG_2(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B)

#define WL1_MSG3(LEVEL, FMT, A, B, C) \
        MSG_3(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C)

#define WL1_MSG4(LEVEL, FMT, A, B, C, D) \
        MSG_4(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C, D)

#define WL1_MSG5(LEVEL, FMT, A, B, C, D, E) \
        MSG_5(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C, D, E)

#define WL1_MSG6(LEVEL, FMT, A, B, C, D, E, F) \
        MSG_6(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C, D, E, F)

#define WL1_MSG7(LEVEL, FMT, A, B, C, D, E, F,G) \
        MSG_7(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C, D, E, F,G)

#define WL1_MSG8(LEVEL, FMT, A, B, C, D, E, F,G,H) \
        MSG_8(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C, D, E, F,G,H)

#define WL1_MSG9(LEVEL, FMT, A, B, C, D, E, F, G, H, I) \
        MSG_9(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C, D, E, F, G, H, I)

#define WL1_CUSTOM_MSG0(CUSTOM_LEVEL, FMT) \
        MSG(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, FMT)

#define WL1_CUSTOM_MSG1(CUSTOM_LEVEL, FMT, A) \
        MSG_1(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, FMT, A)

#define WL1_CUSTOM_MSG2(CUSTOM_LEVEL, FMT, A, B) \
        MSG_2(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, FMT, A, B)

#define WL1_CUSTOM_MSG3(CUSTOM_LEVEL, FMT, A, B, C) \
        MSG_3(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, FMT, A, B, C)

#define WL1_CUSTOM_MSG4(CUSTOM_LEVEL, FMT, A, B, C, D) \
        MSG_4(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, FMT, A, B, C, D)

#define WL1_CUSTOM_MSG5(CUSTOM_LEVEL, FMT, A, B, C, D, E) \
        MSG_5(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, FMT, A, B, C, D, E)

#define WL1_CUSTOM_MSG6(CUSTOM_LEVEL, FMT, A, B, C, D, E, F) \
        MSG_6(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, FMT, A, B, C, D, E, F)

#define WL1_CUSTOM_MSG7(CUSTOM_LEVEL, FMT, A, B, C, D, E, F, G) \
        MSG_7(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, FMT, A, B, C, D, E, F,G)

#define WL1_CUSTOM_MSG8(CUSTOM_LEVEL, FMT, A, B, C, D, E, F,G,H) \
        MSG_8(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, FMT, A, B, C, D, E, F,G,H)

#define WL1_CUSTOM_MSG9(CUSTOM_LEVEL, FMT, A, B, C, D, E, F, G, H, I) \
        MSG_9(MSG_SSID_WCDMA_L1, CUSTOM_LEVEL, FMT, A, B, C, D, E, F, G, H, I)

#define WL1_EVENT_REPORT_PAYLOAD(event_id, length, payload) event_report_payload(event_id, length, payload);

#endif /*FEATURE_DUAL_WCDMA*/


#define WL1_CMN_MSG0(LEVEL, FMT) \
        MSG(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT)

#define WL1_CMN_MSG1(LEVEL, FMT, A) \
        MSG_1(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A)

#define WL1_CMN_MSG2(LEVEL, FMT, A, B) \
        MSG_2(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B)

#define WL1_CMN_MSG3(LEVEL, FMT, A, B, C) \
        MSG_3(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C)

#define WL1_CMN_MSG4(LEVEL, FMT, A, B, C, D) \
        MSG_4(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C, D)

#define WL1_CMN_MSG5(LEVEL, FMT, A, B, C, D, E) \
        MSG_5(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C, D, E)

#define WL1_CMN_MSG6(LEVEL, FMT, A, B, C, D, E, F) \
        MSG_6(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C, D, E, F)

#define WL1_CMN_MSG7(LEVEL, FMT, A, B, C, D, E, F,G) \
        MSG_7(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C, D, E, F,G)

#define WL1_CMN_MSG8(LEVEL, FMT, A, B, C, D, E, F,G,H) \
        MSG_8(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C, D, E, F,G,H)

#define WL1_CMN_MSG9(LEVEL, FMT, A, B, C, D, E, F, G, H, I) \
        MSG_9(MSG_SSID_WCDMA_L1, MSG_LEGACY_ ## LEVEL, FMT, A, B, C, D, E, F, G, H, I)

/*#ifdef FEATURE_QSH_MDUMP
#define QSH_WL1_MDUMP_SET \
do {\
qsh_mdump_set_crash_client(QSH_CLT_WL1,QSH_CLT_WL1); \
qsh_mdump_set_crash_subs_id(QSH_CLT_WL1,SYS_MODEM_AS_ID_1); \
qsh_mdump_set_crash_rat(QSH_CLT_WL1,SYS_SYS_MODE_WCDMA);\
qsh_mdump_set_crash_category(QSH_CLT_WL1,QSH_MDUMP_CRASH_CAT_UNKNOWN);\
}while(0)
#else
#define QSH_WL1_MDUMP_SET \
do {\
}while(0)
#endif

#define WL1_ERR_FATAL(FMT, A, B, C) \
        do {\
        QSH_WL1_MDUMP_SET;\
        WL1_ERR_FATAL(FMT, A, B, C);\
        }while(0)
*/

#define WL1_ERR_FATAL(FMT, A, B, C) ERR_FATAL(FMT, A, B, C)

