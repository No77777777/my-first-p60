#ifndef L1M_H
#define L1M_H
/*===========================================================================

                    L 1   T A S K   D E C L A R A T I O N S

DESCRIPTION
  This file contains global declarations and external references
  for the L1 Manager task.

  The L1 Manager interfaces the L1 Sequencer unit, the Device Driver layer,
  and the Processors within the L1 with the units external to the L1. It is
  the control interface between the L1 and external subsystems and provides
  a task context for the L1. The L1 Manager dispatches external and local
  commands to Processors in the L1. The L1 Manager also provides a basic state
  machine, and acts as a switcher between states of the phone.

  The L1 Manager waits on a set of signals (watchdog timer, task offline,
  task stop, command queues, etc). Whenever there is a signal the L1 Manager
  task will ascertain the source of the signal and performs appropriate
  processing.

  If there is a command on any of the command queues, the L1 Manager performs
  the following processing based on the command.

  Protocol Command: These are sub-states within a particular protocol or
  commands within a sub-state. Upon receipt of the above command, the L1
  Manager will invoke the currently active Protocol Processor to process
  the command.

  Local Command: These commands provide local state control and
  synchronization within the L1. They are also state independent commands.
  The Local Commands are sent through the Local Queue. The local state maps
  all local commands directly to protocol sub-state processing. Upon receipt
  of a Local Command, the L1M will invoke the corresponding Local Processor
  to process the command.

EXTERNALIZED FUNCTIONS

 l1_free_cmd_buf
   This function is used by L1 to free the external command buffer once L1 is
   finished processing the command. This function is executed in the context
   of the caller.

 l1_cmd_confirm
   This function sends a confirmation cmd to the upper layer.

 l1_cphy_setup_cnf_put_status
   This function is used by the L1 modules to update the l1_cphy_setup_status
   buffer in the course of performing the steps of a CPHY_SETUP_REQ.

 l1_put_local_cmd
   This function is used by the internal units of the L1 to submit local
   commands. This function puts the command onto the L1M Local Command
   Queue and sets the Local Command Queue for the L1 task.

   This function is called in the context of the L1M thread or in the
   context of an ISR within the L1.

 l1m_get_local_cmd_buf
   This function is used by the internal clients of the L1 Manager to get a
   command buffer.

 l1_get_rrc_cmd_buf
   This function gets the command buffer to send a command/confirmation
   to RRC.

INTERNALIZED FUNCTIONS

  l1_process_ext_cmd
    This function de-queues a command from the protocol command queue,
    invokes the command processor of the currently active protocol processor
    and passes the command to it.
    If there are no more protocol commands pending on the protocol command
    queue, the protocol command queue signal is cleared.
    This function is called in the context of the L1 thread.

  l1_cmd_err_ind
    This function sends a command error indication to the upper layer.

  l1_free_local_cmd_buf
    This function frees the command buffer associated with the local command.

  l1m_process_local_cmd
    This function de-queues a command from the local command queue,
    invokes the command processor of the currently active local processor
    and passes the command to it.
    If there are no more local commands pending on the local command
    queue, the local command queue signal is cleared.

    This function is called in the context of the L1M thread.

  wl1m_init
    This function does all initializations necessary for proper operation
    of the L1.

    This function should be called before the L1M can be used.

  l1m_mdsp_init_done_isr
    This ISR is called after the MDSP is initialized. The MDSP_INT4 is
    muxed with the init done and rach done interrupts. This function will
    set the MDSP_INIT_DONE_SIG signal.

  L1M Idle State Functions
  l1m_idle_init: Called when L1M transitions to Idle state.
  l1m_idle_cleanup: Called when L1M leaves Idle state.
  l1m_idle_cmd_processor: Processes external commands received by L1 in Idle
                          state.

  L1M ACQ State Functions
  l1m_acq_init: Called when L1M transitions to ACQ state.
  l1m_acq_cleanup: Called when L1M leaves ACQ state.
  l1m_acq_cmd_processor: Processes external commands received by L1 in ACQ
                         state.

  L1M BCH State Functions
  l1m_bch_init: Called when L1M transitions to BCH state.
  l1m_bch_cleanup: Called when L1M leaves BCH state.
  l1m_bch_cmd_processor: Processes external commands received by L1 in ACQ
                         state.

  L1M PCH State Functions
  l1m_pch_init: Called when L1M transitions to PCH state.
  l1m_pch_cleanup: Called when L1M leaves PCH state.
  l1m_pch_cmd_processor: Processes external commands received by L1 in PCH
                         state.

  L1M FACH State Functions
  l1m_fach_init: Called when L1M transitions to FACH state.
  l1m_fach_cleanup: Called when L1M leaves FACH state.
  l1m_fach_cmd_processor: Processes external commands received by L1 in FACH
                         state.

  L1M DCH State Functions
  l1m_dch_init: Called when L1M transitions to DCH state.
  l1m_dch_cleanup: Called when L1M leaves DCH state.
  l1m_dch_cmd_processor: Processes external commands received by L1 in DCH
                         state.

  L1M Deactive State Functions
  l1m_deact_init: Called when L1M transitions to Deactive state.
  l1m_deact_cleanup: Called when L1M leaves Deactive state.
  l1m_deact_cmd_processor: Processes external commands received by L1 in
                           Deactive state.
  
INITIALIZATION AND SEQUENCING REQUIREMENTS

  The L1_task function contains the L1 Manager task.  It must be
  specified in a call to rex_def task and started by a Main Control task.

  Copyright (c) 2000 - 2016 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/l1m.h_v   1.56   19 Jul 2002 13:56:30   djm  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1m.h#7 $ 
$DateTime: 2021/02/10 03:14:23 $ 
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/09/21   sp      Compiler upgradation changes ATHERTON 
03/12/20   vb      Replacing busywait with sleep.
12/10/19   ss      Minidump related changes
11/19/19   as      Since search cleanup is not always required during idle state initialization,
                   introduced new API to differentiate caller of l1m_idle_init and done search cleanup accordingly.
09/18/18   th      Changes to discard stale async_callback local command
04/02/18   ss      Workaround for bad DC config sent by nw protected under NV 74092
01/16/18   vn      ML Logging changes for state and status
05/15/17   kr      Send FW to STOP instead of Sleep in 1xSRLTE mode
08/21/17   ss      Delay CPHY_CNF for fast LTA resume by atleast 20ms
08/12/17   ss      Remove unused client from suspend periodic ops
08/18/17   nc      Added SFN_DECODE client to suspend periodic ops
08/16/17   ss     Changes for LTA timeline optimization
08/24/17    br      Support for directly calling a new API to read gcf NV value
12/16/16   mk      CPU and BIMC boost for W2G and W2L compressed mode only for MSM8953
07/15/16   kr      Fix issues when inINT column
                   set rf_fw_mdsp flag
                   increase threshold to trigger MIN sleep time OPTs
                   DOnt trigger MIN sleep time Opts if R99 clients are active.
07/07/16   sg      Reverting back the workaround done to delay DPCCH configuration
06/22/16   sg      Adding a flag to indicate whether DPCCH cfg will be delayed or not
06/03/16   sks     Remove usage of BMC sleep abort flag
04/20/16   skk     QSH sleep handlers.
03/04/16   skk     NASTT search power optimization changes.
03/15/16   sr      Introducing mutex lock to protect while updating LTE cap
03/16/16   sks     Force and RF and FW to sleep when modify band fails after BPLMN
03/02/16   sg      Debug code to catch cell config stuck issues.
02/11/16   ms      Doubling the global variable start_stop_op_pass
02/11/16   mk      Wl1 to Ml1 (LTE) command timeout handling support in all state:
02/10/16   skk     Use the PO overran threshold as 20 only if alt SUB is W.
01/13/15   ad      Changes to pend the RnN when IFHHO is in progress and unpend it at the 
                   end of cphy_setup
01/13/16   nga     RnN for pending LTE searches
12/22/15   ar      Add macro for checking if L1 is resuming after LTA gap
12/15/15   sl      Perform HS_RACH_MODULE cleanup ahead of EDRX de-configuration
12/11/15   sl      Back-out CR(944361) - Remove TxD detection changes only
12/03/15   sl      Debug code to capture TxD detection stuck issues
11/05/15   svh     MDSP Recovery changes.
10/22/15   da      Implemented the changes to support ACQ in DR
10/22/15   rs      WCDMA Qlink support for SC mode transition
10/21/15   da      Changes to update rgs in wl1 task context
10/20/15   raj     try sche_seq lock and call ent_sche_update in task context
10/12/15   skk     Added a timer to declare OOS in idle mode if lock is not available for 12sec.
09/25/15   skk     Post a local cmd to change L1M state from BCH to BCH_SLEEP.
09/07/15   skk     Perform schedule update on BCCH if CRC fails in PCH_SLEEP/BCH_SLEEP states.
10/09/15   pvr     Mods to post local cmd to update CXM phychan DB
10/02/15   ac      FR 26288: DS downlink quality measurement reporting
09/15/15   cc      Add variant to GL1 init
09/04/15   hdk     W+W ASDiv feature
09/01/15   ar      Changes to lower Q6 clock during periods of no EUL transmission
09/02/15    ap      Adding RACH abort to susp periodic ops
08/28/15   hdk     Update state to ASD TRM for all state transitions.
08/20/15   sr      Changes to poke holes in SIB mask to go to sleep to handle pending call setup 
08/18/15   svh     Consider MAX monitor also for thermal mitigation.
08/17/15   ap      parallelizing suspending periodic ops
08/11/15   rsr     Added Support to ensure that FW is put to Sleep, prior 
                   to Entry of Traffic State. 
05/08/15   sr      Trigger cell decfg in pending call setup mechanism when FCDC is not allowed
07/31/15   svh     Update CFCM framework for new intf.
07/27/15   gn      as_id related compilation error fixes
07/17/15   pvr     QDTA changes
07.15.15   kr      dont release lock in offline nastt if sleep abort in progress 
07/08/15   tsk     EFACH + MSIM fixes as part of blanket CR866776
06/30/15   tsk     FR 26237: DSDS with Rel.8 E-FACH HSRACH + MSIM Changes.
06/25/15   vn      Added local command type for SRCH CPC DRX.
06/20/15   ac      Remove unneeded RX_TX_SPLIT featurization for BO
06/17/15   ar      Ensure that UL periodic log status processing is only done for connected sub
06/18/15   hdk     Removing FEATURE_WCDMA_JOLOKIA_MODEM from DSDA ASDiv changes.
04/21/15   bj      Split ACQ changes for W+1x.
06/03/15   svh     TA featurisation.
06/02/15   sks     Fix a KW error.
05/20/15   pr      Replace MSG_SSID_DFLT with MSG_SSID_WCDMA_L1
05/07/15   skk     FR24494: BCH Inter SIB sleep initial check in.
05/07/15   rs      Rx-Tx Split changes
02/27/15   ap      Initial check-in for WL1 QSH framework 
03/24/15   skk     Trigger DL WEAK IND if L1 doesnt get lock for 2 sec during cell selectoin.
04/22/15   pr      W+W Phase 7 Checkin.
04/22/15   rsr     Added changes to Ensure that IRPM Enable happens 20SF after 
                   first triage cycle.
03/31/15   sr      Changes to add SCCH order module as part of DCH cleanup
03/23/15   rsr     W + W Phase 6 Checkin.
03/11/15   rsr     Phase 4 W+W Checkin.
03/10/14   pr      MCAL DL Changes for WplusW
02/20/15   rs      Added Mutex protection to avoid race condition when handling deregister-register local cmds 
                   for the same handle
02/04/15   sl      Avoid performing CM suspend operation twice in connected mode cleanup
01/30/14   pr      WL1M global reorg for WplusW
01/29/14   pr      WL1M global reorg for WplusW
01/28/15   pr      Global reorg for W + W.
01/26/15   jhl     Global re-org for W+W
01/23/15   sad     Global re-org for W+W
01/20/15   jd      Global re-org for W + W
01/19/15   psr     Global re-org for W+W
01/19/15   sks     Global re-org for W+W
01/16/15   vr      Global reorg for W + W.
01/15/15   pr      Global reorg for W + W.
01/09/15   pr      L1M code cleanup.
01/06/15   sl      Added wl1 timer for CPHY_CELL_TRANS_REQ processing
01/05/15   sl      Perform FMO cleanup before EDRX module cleanup happens
12/16/14   sr      Enhancing conn state cleanup by suspending CM first instead of supending while
                   DL cleanup is triggered
12/12/14   hdk     ASDiv test mode enhanced
12/04/14   gm      Dont allow QTA if a local command has been posted for cphy_setup_request.
11/17/14   jhl     Added new client WL1_TIMEOUT_DSR_CLIENT for DSR tune done signal
03/08/14   nd      Update 0x423A log packet for Rx Blanking
11/06/14   sl      Changes to crash s/w if SCCH order processing isn't completed in 4 frames
10/02/14   sd      Removed l1m_wait_for_trm_stop state and related routines
09/23/14   pv      Support for TRM swapping of chains for call setup in DR mode.
                   Enhanced the existing API to post a specific SIB mask internally.
09/22/14    amj     Add a Mode change request timeout client.
09/09/14    rsr     Added Changes to wait for AGC CFG done after calling RFM 
                    sleep.
08/18/14   vr      Cleanup code for HSRACH 'conventional' seamless recfg.
08/07/14   sr      Changes to handle freeze/unfreeze for CPC SCCH order in local command
08/07/14   sr/vp   Changes for enhanced local command processing
08/04/14   sad     RxD changes for DB-MC in case of SCCH order
07/31/14   gm      Changes to reduce the text size.
07/16/14   sr      EDRX code cleanup
07/15/14   ac      NV reorganization
07/11/14   gp      Added support for RF Temp Comp Split architecture
07/03/14   sks     Changes for FCDC
07/01/14   hk      Header file inclusion cleanup
06/24/14   hdk     Featurizing DSDA ASDiv changes under FEATURE_WCDMA_JOLOKIA_MODEM
06/16/14   ar      Workaround to populate BTF delay while sending mod timing command to FW
                   if not populated by RF yet.
06/13/14   hdk     ASDiv DSDA feature
06/03/14   ks      Do not init GL1 when going to state where we don't need it
05/20/14   skk     Freed a rex signal.
05/21/14   jk      Moved the periodic measurements handling from srch_ops_cmd to a 
                   new local command.
05/19/14   jd      Timeout handler if rxfe is not fired during scch activation order.
05/14/14   rsr     Added RxD timeout Functionality.
05/06/14   as      Moving WRM to WL1TRM.
04/29/14   rsr     Added CME timeout Functionality.
04/27/14   jkb     Add check for Feature dual sim with feature qta 
04/19/14   scm     Handle blocking RRC command immediately in l1_wait_for_rrc_resp().
04/16/14   bj      Remove duplicate code in split ACQ code
04/01/14   km      Rxd/ACQ changes
03/26/14   sks     Reconfig HSUPA clocks through the request_client rather than the relinquish_client.
03/11/14   ar      Create a global NV override for RxD, FET and DSR features for ES1 Build
01/27/14   mit     Added fix for KW error
01/21/14   ar      Add support for handling QICE during W2L measurements in FACH
01/21/14   abs     Cleaning up DSDA~CXM(FR:2334) changes
11/25/13   skk     Added mutex protection around setting/clearing of pich entity to sleep pending variable in ssmgrdb.
07/01/14   amj     Clean up old ASET Update code
01/07/14   as      Added WL1TRM_TUNEAWAY_LOG_SUBMIT command type.
12/16/13   as      Porting G2W TA from Triton to Dime.
11/27/13   hk      Unification of non-intra module cleanup procedures
11/21/13   dm      Post local command for SFN/CFN sanity check.
11/05/13   hk      Reduced the state cleanup timeout
10/30/13   sr      Changes to a macro to check L1M module pending bitmask 
10/23/13   sm      Local command support for RxD
10/28/13   gp      Fix for thermal mitigation stall 
10/28/13   ar      Add support for pending cleanup when FMO is in progress
10/14/13   pr/sr   Channel config optimizations
09/29/13   as      Continuing port of Dual Sim from Triton to Dime.
09/23/13   as      Porting Dual Sim from Triton to Dime.
09/12/13   jd      Searcher redesign
08/28/13   sad     Changes to state cleanup bitmask handling for DL\UL cleanup
08/14/13   sad     Changes to state cleanup bitmask handling for HS\EUL cleanup
08/02/13   pj      DOG HB changes
07/23/13   vs      Added support for antenna switch diversity feature.
07/22/13   rsr     Added CME to FACH Cleanup Module BMSK
07/15/13   abs     Moving MSGR changes outside of LTE featurization 
05/20/13   sad     Support for Enhanced Layer-1 Manager
05/10/13   mk      Defined a macro for Out of service in frames for FACH state.
05/09/13   pr      Featurized code under FEATURE_WCDMA_BOLT_RUMI_HWIO_DEMOD as part of Bolt Bringup.
04/09/13   stk     Fix for compilation issues when enabling FEATURE_WCDMA_FREE_FLOATING_TASK
03/27/13   pr      SW Changes for TxD command done from FW.
02/22/13   gsk     Externing a variable
02/11/13   gsk/sr  EDRX Full Sleep initial check in.
02/10/13   pr      Added new constants for L1-RRC Timers
01/31/13   zr      Mainlining FEATURE_WCDMA_RESCHEDULE_TXD_DET
01/28/13   dp      Added support for Antenna Tuner
12/17/12   scm     Maintain Access Stratum ID receive with all RRC commands.
08/29/12   pv      Use unused signal for MDSP AGC tune. Added params for MDSP sleep APIs.
08/01/12   pv      Added a parameter to l1m_in_drx API and replaced a signal.
07/27/12   pv      Backing out previous check-in.
07/22/12   pv      Added a parameter to l1m_in_drx API.
11/15/12   sr      Changing the CPHY_SETUP_TIMEOUT from 10 to 2 seconds
09/06/12   sr      Changing the CPHY_SETUP_TIMEOUT from 10 to 3 seconds
08/09/12   hk      Streamlined FACH and DCH module cleanup procedures
08/19/12   sj      Added support for QICE receiver in EDRX mode
08/10/12   at      Removed eram_dump_harq_id as ERAM will be dumped without using local cmd.
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
05/15/12   rgn     Added task init functions 
07/05/12   pv      Handling of sample server manager local cmd for Offline PICH.
06/19/12   vr      HSRACH code update
06/08/12   gv      Feature HS RACH Initial code check in
06/07/12   vs      MBMS feature support.
04/12/12   zr      Add support for periodic TD determination.
03/30/11   scm     Start sleep again immediately after handling new CTCH schedule.
03/27/12   hk      Added support for FEATURE_WCDMA_HS_FACH_DRX
03/10/12   sv      Added support for TxD reschedule mechanism.
02/10/12   rgn     Nikel feature cleanup
02/10/12   sj      Added an unified CME local cmd handler.
02/01/12   gnk     Nikel Feature cleanup.
01/31/12   raj     Nickel feature cleanup. Mainlined the features FEATURE_WCDMA_A2_HW_UL
01/27/12   vs      Nikel feature cleanup.
01/20/12   bs      free floating changes.
01/10/12   sp      Serialize RF RxD enable and Tx enable calls using a mutex
12/27/12   uk      Chk for GFW suspend before proceeding with state cleanup
12/02/11   stk     Added support for FEATURE_WCDMA_RXD_PCH and FEATURE_IDLEMODE_FILTER
11/21/11   uk      Added macro to check if DL module cleanup is pending 
10/21/11   mmr     Added an extern. 
09/27/11   zr      Extending l1m_module_cleanup_done_mask to 32 bits 
09/21/11   sp      Added NPA Support for Thermal Mitigation under FEATURE_WCDMA_NPA_THERMAL
08/24/11   hk      Added command id parameter while sending the error
                   indication to higher layer
08/03/11   rgn     Post a local command to deconfigure Dual carrier 
07/14/11   amj     Added new command for DRX Config done.
07/07/11   gv      Added code to support NS RGCH Rel 8 SPec CR. In this file
                   added prototype is UE rel 8 query.
05/27/11   mc      Added debug capability for bringup
04/28/11    sj      upmerge from offline/main
04/26/11   uk/mm   Support for PCH cleanup
04/26/11   dp/uk   Add support for W2X_IDLE_CMD
04/25/11   geg     Replaced seq.h with seq_v.h
04/25/11   geg     Replaced *.h with *_v.h
04/25/11   amj     Added grant monitoring enabled field to CPC DRX CFG log packet.
                   Added slot level info to drx mdsp event type.
04/08/11   rgn     Added a field to drx log struct
03/31/11   amj     Added code for CPC DRX CFG log packet.
03/11/11   dp      Ported CPC DRX changes to offline Architecture
03/12/11   sa      Fixed issues in W2L code.
02/11/11   sp      Added support for Thermal Mitigation under FEATURE_WCDMA_MTPL_THERMAL_BACKOFF
01/31/11   sa\nd   Moved processing of INTER-RAT periodic measurements from 
                   ISR to task context.
01/14/11   ks      Added RxD support for FACH state and DPCH signalling
11/19/10   sp      Added new ul_logging_cmd to handle R99 UL log pkts in task context
11/17/10   vsr     Added CME cleanup functionality
11/02/10   ksr     Moving LOG_STATUS into periodic framework
08/13/10   mc      Saving CFN of last run of processing L1 local commands
10/25/10   vsr     Added RxD cleanup functionality
10/15/10   yh      Added support for W2L Compressed mode
09/15/10   ksr     Enhanced A2 logics with ERAM Readback, L1-L2 interace contents
                   dumping, Rd back profiling etc
08/23/10   uk      changed algorithm for sending LTE INIT/Deint
08/08/10   sj      Added unified PLT support
07/15/10   vsr     Ported API l1m_is_ue_in_ftm_mode() from online branch
07/08/10   vsr     Changes to use default pci selection value from mimo param
06/14/10   stk     Added Rel8 enum support for Release Indicator NV
06/02/10   stk     Added support for W2LTE measurements and reselection under 
                   FEATURE_WCDMA_ABSOLUTE_PRIORITY & FEATURE_WCDMA_TO_LTE
02/10/09   ms      Taxis and Genesis feature validation
07/15/09   rgn     Enhanced existing s-cpich support
07/07/09   sv      Added macros for S-CPICH support.
04/29/09   mg      Add extern mcalwcdma_dsp_init
04/17/09   mg      Remove double declaration of l1m_get_state
01/30/09   mg      Readjusted the WL1 signal to not overlap with RPC signals
12/10/08   sv      Corrected define value for UE rel 7. 
12/09/08   uk      Mainling FEATURE_WCDMA_DL_ENHANCED, FEATURE_WCDMA_DRX_MANAGER
                   and FEATURE_WCDMA_OFFLINE_DEMOD_SUPPORT
11/26/08   sv      Added defines for UE release 6 and 7. 
11/04/08   rgn     New demod status dump designchanges
10/03/08   hk      Bringing in 7k Mailine fixes
01/22/08   vsr     Featurizing PCCPCH weak indication under DL enhanced
01/16/08   vp      Added support for PCCPCH weak indication
10/24/07   vsr     Mainlining MBMS and Enhanced DL changes 
09/26/07   nd      Added the corresponding struct for local command SRCH_FS_PROC_DONE 
09/24/07   rmak    Extern pass_tcxo_data_to_tcxo_mgr()
08/06/07   vsr     Included new file dlacqpullin.h
08/07/07   mg      Extern wl1_mdsp_image variable
06/15/07   kps     Support stopping and restarting the AGC for an RF TUNE.
06/08/07   rmak    Added extern for l1_sleep_abort_timer 
05/29/06   kps     Rude Timetag support
05/14/07   rmak    Changed l1_about_to_setup_pccpch_n_in_idle to l1_about_to_setup_or_drop_pccpch_in_idle
04/10/06   mg      Extern wl1_hw_mpc_result_flag variable
03/09/07   kps     Extern wl1_tcxo_freq_err_ready().
03/05/07   sk      Added code for FEATURE_UMTS_UNIFORM_OOS_HANDLING_P2 feature.
01/31/07   nd      code changes to turn on/off FEATURE_WCDMA_INTERF_MEAS_RES_OPT
                   using nv item
12/20/06   rmak    Free up signal COMB COUNT SLAMMED AFTER SLEEP SIG
10/31/06   kps     Rename some tcxo related identifiers
10/27/06   kps     Added tcxo_restriction_local_cmd_type local command 
                   under FEATURE_WCDMA_TCXOMGR30 to support TCXO Mgr 3.0
10/25/06   scm     Remove definition of wl1m_init().
10/10/06   ms      Added a member l1_hs_module_cleanup_after_hs_teardown_cmd_type 
                   to the union l1m_local_cmd_type.
09/28/06   mc      Added MCAL decoder support under feature 
                   FEATURE_MCAL_WCDMA
10/02/06   kps     Adding L1M_WAIT_FOR_TRM_STOP state and support routines.
09/22/06   mg      Add extern restart_stmr_on_start_agc_failure 
09/11/06   kps     Change MDSP_INIT_TIMEOUT_SIG to WL1_TIMEOUT_SIG to reuse it.
08/22/06   rgn     Added support for mdsp app logging 
                    logging under feature FEATURE_UMTS_MDSP_DEBUG_LOG
08/14/06   yiz     Mainline HSUPA code
07/28/06   scm     Don't featurize L1M_PCH_BPLMN definition with BPLMN #ifdef.
07/06/06   rmak    Changed l1m_get_state to return uint8 to avoid
                        exposing l1m_state_enum_struct to non-L1 code
06/20/06   bd      Renamed global vars/functions to avoid conflict with 1x names
05/02/06   mg      6280: CC slam sig not needed
03/20/06   mg      Added L1_SAMPSRV_LOG_MEAS_TIMER_SIG signal
02/20/06   gv      Added extern declaration for the global l1m_resume_cause.
12/06/05   ub      Added extern declaration for resuming_wcdma_flag
11/22/05   mg      Added extern declaration for stmr_has_passed function
11/08/05   gv/nd   Added code for WtoW BPLMN feature.
10/28/05   gs      Added support for NV based Rel5/R99 functionality
08/18/05   vp      Added support for reslamming the pccpch for the TTI-1 
                   hypothesis.
08/05/05   ub      SFN based full search timers implementation
06/20/05   mg      Move l1_about_to_setup_or_drop_pccpch_in_idle out of T_BARRED feature
06/01/05   mg      Extern l1_about_to_setup_or_drop_pccpch_in_idle
05/11/05   kps     lint fix.
05/10/05   sk      T-barred Feature: Modified periodic timer to be a timer
                   based on t-bar of barred cell. Hence added new globals.
05/05/05   gs      Added structure member mdsp_cmd_log_submit_parms to union
                   of local commands
05/04/05   sk/sh   Added T-barred Feature support
03/07/05   rc      Added trch_crc_cmd local command under feature 
                   FEATURE_WCDMA_QUALITY_MEAS for Quality Measurements support. 
02/19/05   gs      Changed type and castings related to l1m_module_cleanup_done_mask
                   to uint16 to accomodate more modules to cleanup.
02/17/05   scm     Mainline FEATURE_DUALMODE_BASELINE.
01/24/04   sh      Defined macro for checking whether field performance
                   optimizations are enabled
12/30/04   gs      Added HS_CFG module to cleanup module enum and mask
12/02/04   scm     Add some casts for lint warnings.
11/15/04   sh      Removed l1_pending_drx_cmd and l1_drx_cmd_pending.
11/02/04   scm     Add extern to wl1_send_msg_to_controller().
10/11/04   gs      Added HSDPA related local command to local command union
09/22/04   sh      Added macro L1_INIT_MODULE_CLEANUP_DONE_FOR_SIB_MOD() for
                   use during SIB mod updates.
06/01/04   scm     Add timer to push rotator value to GPS periodically.
05/10/04   gs      Added macro Added macro INIT_FACH_CLEANUP_MODULE_DONE
04/05/04   asr     New signal : L1_IN_L1M_IDLE_TOO_LONG_SIG
03/25/04   sh/src  Add extern declaration for l1m_is_mdsp_recovery_in_progress.
03/08/04   src     More error recovery support: addition of a timeout operation
                   for search abort-all, and addition of an argument to every
                   state-change init function to indicate whether it is called
                   under normal circumstances or during a state-change timeout.
03/06/04   gs      Replaced macro L1_CM_MEAS_MODULE with L1_CM_GSM_MEAS_MODULE
                   Added macro L1_CM_INTERF_MEAS_MODULE
                   Added cleanup done flag L1_CM_INTERF_MEAS_MODULE in macro
                   INIT_DCH_CLEANUP_MODULE_DONE
03/03/04   sh/src  Support for error recovery from layer-1 getting stuck while
                   processing a CPHY_SETUP_REQ.
03/01/04   src     Support for error recovery in some cases of mDSP dead/stuck.
02/23/04   sh      Added L1_STATE_CLEANUP_TIMEOUT_SIG and declared timer and
                   timeout to implement timeout mechanism for state cleanup.
02/16/04   asr     Added prototype for l1m_send_op_err_ind().
02/17/04   sh      Added L1_OUT_OF_SERVICE_TIME_PCH for 12s oos timer in DRX.
02/15/04   scm     Add ONE_SECOND definition for mDSP timeouts.
02/06/04   sh      Define L1_SLEEP_ABORT_TIMEOUT_SIG and function protos
                   l1m_sleep_abort_timeout_handler() and l1m_wake_up().
10/24/03   gs      Added enum L1_CM_MEAS_MODULE in l1_module_enum_type.
                   Updated macro INIT_DCH_CLEANUP_MODULE_DONE to include
                   L1_CM_MEAS_MODULE.
10/10/03   asr     Changed value of L1_DRX_DELAYED_TIMER_SIG to avoid conflicts.
09/24/03   asr     Added declaration of l1m_clear_drx_timer_sig().
09/03/03   asr     Added L1_DRX_DELAYED_TIMER_SIG and declared l1_drx_delayed_timer
                   for recovery from a "stuck" DRX state machine. 
07/31/03   m       Adds a signal number used in reading NV
05/28/03   scm     Change mDSP start/stop routines to return void.
05/21/02   asr     Created extern declaration of boolean l1m_just_completed_call.
05/06/03   djm     add element to compressed mode runtime error cmd struct (tgmp)
05/02/03   scm     Add routines l1m_send_start_agc_cmd_to_mdsp() and
                   l1m_send_goto_sleep_cmd_to_mdsp().
04/17/03   sh      Added signal mask and declared rex timer for out of
                   service timer in FACH.
01/30/03   gs      Added macro to define L1_CM_MODULE bit position in module
                   cleanup mask.
                   Added macro INIT_DCH_CLEANUP_MODULE_DONE for DCH cleanup.
                   This adds L1_CM_MODULE bit to existing modules.
01/29/03   scm     Add extern to l1m_clear_resuming_wcdma_flag().
01/06/03   scm     Modify l1m_suspended_cleanup comment.
11/06/02   scm     Adding L1M_DEEP_SLEEP state and support routines.
10/30/02   scm     Removing L1M_RESUMED state and related code.  Adding extern
                   to boolean l1m_resuming_wcdma().
10/11/02   djm     added meas id to TGPS runtime error local command
10/07/02   scm     SUSPEND with activation time support.
10/07/02   scm     Cleanup signal defs to avoid reuse and remove unused sigs.
10/02/02   gs      Changed the signal L1_AUTO_BLER_MEAS_TIMER_SIG value
                   from 0x00002000 to 0x00001000 as it collides with OFFLINE_SIG.
                   This was resulting in signal handling code in wcdma_l1_task
                   signal for loop not to generate code.
09/18/02   scm     Don't feature def STOPPED, SUSPENDED, and RESUMED states and
                   don't reuse signals.
09/06/02   djm     ifdef FEATURE_CM_TGPS_RUNTIME_ERROR runtime error command struct
08/30/02   scm     More work on SUSPEND/RESUME.
08/13/02   scm     Add MDSP_INIT_TIMEOUT_SIG to not wait forever on mDSP init.
08/06/02   scm     Continued work on SUSPEND/RESUME support.
07/18/02   djm     addition of cm_runtime_error_tgpsi for compressed mode runtime
                   error local command processing.
07/18/02   scm     Initial pass at SUSPEND/RESUME support, partly functional.
07/10/02   djm     include CM meas done signal always, CM measurement unit test
                   code will use without defining FEATURE_CM_SUPPORTED
06/18/02   gs      Added macro definition for CM meas done signal.
06/11/02   asr     Defined the following signals to the L1 task: SLEEP_PERIOD_COMPLETE_SIG,
                   COMB COUNT SLAMMED AFTER SLEEP SIG
05/29/02   m       Added support for LOD by introducing new types and prototypes
05/18/02   sh      Added l1_ena_cell_resel_timer to enable cell reselection
                   1 second after we enter DRX and srch_cell_resel_eval
                   local command to start cell reselection.
05/06/02   scm     Change FEATURE_WCDMA_L1_START_STOP to
                   FEATURE_CHEETAH_BASELINE, to match RRC.
05/03/02   scm     Implementing support of START/STOP primitives from RRC.
                   Code wrapped in FEATURE_WCDMA_L1_START_STOP define.
03/15/02   asr     Added l1m_is_connected() and l1m_get_state() functions
03/14/02   asr     Added l1m_in_drx(), l1m_out_of_drx() functions extern 
                   declarations
03/13/02   djm     Explicitly set L1M_INIT=0 and SRCH_MODULE=0.
                   The l1m_state_enum_type enum is used as an array index,
                   force it to minimal size 0-L1M_NUM_STATES.
                   The l1m_module_enum_type is used as a bit mask force to
                   values 0-NUM_L1_MODULES.
02/26/02   sh      Added flag FEATURE_L1_ACT_TIME to local command.
02/25/02   sh      Added seq_act_time_local_cmd_type to l1m_local_cmd_type.
02/11/02   asr     Added the state transition functions for PCH_SLEEP
02/01/02   sh      Added signal L1_RF_TEMP_COMP_TIMER_SIG for rf temperature
                   compensation for ffa build.
01/31/02   asr     Support for sleep: update to l1m_local_cmd_type, included
                   drxcmd.h; prototypes for l1m_prepare_for_sleep(),
                   l1m_prepare_for_wakeup(), l1m_woken_up(), and
                   l1m_sleep_reacq_failure().
                   Added new L1M state : L1M_PCH_SLEEP
10/19/01   sh      Added L1M_DEACTIVE state and state related functions.
10/17/01   sh      Added l1m_mdsp_init_done_isr that is called after mdsp
                   initialization.
09/14/01   gs      Added timer flag for the feature FEATURE_L1_AUTO_BLER_MEAS.
09/06/01   asr     added functions l1_wait, l1m_offine, l1m_power_down,
                   and l1_watchdog_report
08/31/01   eh      Added L1_NV_CMD_SIG.
08/31/01   sh      Added new state L1M_FS for frequency scan.
07/17/01   gs      Added the include file dltrchmeas.h
07/14/01   sh      Added SEQ_MODULE to l1_module_enum_type. Added functions
                   l1_get_rrc_cmd_buf() and wl1_put_cmd_head().
06/05/01   sh      Removed code under flag SEQ_EVENT_CHECK_ENABLED.
                   Added function l1_get_rrc_cmd_buf().
05/21/01   sh      Added RF_TUNE_DONE_SIG.
05/20/01   wjn     Added the function prototype l1m_clear_srch_done_cmd().
05/13/01   wjn     Added srch_sfn_cfn_diff_cmd_type and
                   dl_sfn_cfn_time_diff_meas_cmd_type to l1m_local_cmd_type.
                   Deleted srch_nbch_sfn_info_type from l1m_local_cmd_type.
04/19/01   sh      Added MDSP_DELAY_TIMER_SIG.
04/06/01   sh      Replaced flag REX_PLUS_PLUS with FEATURE_REX_DYNA_MEM.
03/29/01   sh      Added dl_cell_trans_drop_done_cmd_type local command.
03/09/01   sh      Changed l1_cphy_setup_cnf_ops_enum_type to
                   l1_setup_ops_enum_type.
02/27/01   sh      Reverted back to using free queues for local commands since
                   REX++ dynamic memory cannot be used in interrupts.
02/26/01   sh      rrccmd.h is now only included in wplt.h for WPLT builds.
02/23/01   sh      Added new state L1M_INIT. Wrapped free queue decls in ifndef
                   REX_PLUS_PLUS.
02/21/01   mdl     removed dl_acq_phychan_cfg_cmd_type and
                   dl_acq_phychan_frm_bdry_cmd_type from l1_local_cmd_enum_type
02/14/01   wjn     Added "srch_cphy_aset_cmd_type" and
                   "dl_cphy_aset_done_cmd_type" to "l1m_local_cmd_type" for
                   channel setup handshake between SRCH and DL module.
01/25/01   sh      Added Timer to L1M wait main loop to check for SFN/CFN
                   Event timeout. Added l1m_discard_local_cmds() function
                   and removed l1m_execute_all_local_cmd().
01/25/01   mdl     Added data structures and macros to allow L1M modules
                   to signal L1M after completion of their cleanup
                   procedures.
01/18/2001 mdl     get next L1M state as input parameter to cleanup routines
01/10/2001 mdl     Added #include "mdspsynccmd.h" to obtain definition of
                   mDSP Sync timeout command struct.
                   Added mdsp_cmd_timeout to the l1_local_cmd_type union
01/03/01   sh      Added more documentation for the L1 State Functions.
                   Changed the interface to function l1_cmd_confirm()
                   to take rrc_cmd_type instead of rrc_l1_cmd_type.
12/21/00   sh      Changes to adhere to new command queue interfaces.
                   Modifications to take care of the changes in the command
                   types resulting from deleting l1rrcctrl.h and creating
                   new interface files l1rrcif.h and l1macif.h.
12/01/2000 mdl/sh  Added all L1 signal #defines, added prototypes for
                   l1_cmd_done, l1_local_cmd_done,
                   l1_cphy_setup_cnf_put_status, and
                   l1_cphy_setup_cnf_get_status
11/08/00   sh      Moved l1_task() declaration from l1rrcctrl.h to here.
08/09/00   yus     Created file.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "customer.h"
#include "rex.h"
#include "queue.h"
#include "l1def.h"
#include "l1task_v.h"
#include "mdspsynccmd.h"
#include "srchcmd.h"

#ifdef FEATURE_WCDMA_ANT_SWITCH_SUPPORT
#include "trm.h"
#endif

#include "edlcfg.h"

#include "srchcrgsm.h"

#include "ulcmd.h"
#include "dltrchmeas.h"
#include "drxcmd.h"

#include "hscfg.h"

#ifdef FEATURE_L1_LOG_ON_DEMAND
#include "logod.h"
#endif

#include "l1qualmeas.h"

#ifdef FEATURE_WPLT
#include "wplt.h"
#else
#include "rrccmd_v.h"
#endif


#include "wl1drxentity.h"

#include "drxofflineprocmgr.h"

#include "wl1trm.h"

#include "wl1multisim.h"

#include "wl1cxm.h"

#include "lte_ml1_ext_api.h"
#include "lte_ind_msg.h"

#include "dlphch.h"
#include "dlchmgr.h"
#include "dlbcchmgr.h"
#include "seq_v.h"

#include "dlacqpullin.h"
#include "msg.h"

#include "demoddrv.h"
#include "dlcellcfg.h"
#include "DDIChipInfo.h"

#if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
#include "wsrchlte.h"
#endif

#include "wl1qicectrl.h"
#include "drxofflineprocmgr.h"
#ifdef FEATURE_WCDMA_HS_RACH
#include "wl1hsrachcntrlr.h" 
#endif /*FEATURE_WCDMA_HS_RACH*/

#ifdef FEATURE_WCDMA_CFCM
#include "cfcm.h"
#endif

#if defined (FEATURE_WCDMA_QSH_DUMP) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC)
#include "wl1_qsh_ext.h"
#endif /*defined FEATURE_WCDMA_QSH_DUMP|| defined FEATURE_WCDMA_QSH_EVENT_METRIC */

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

/* Bit 0 - Skips adding internal PCCPCH at acquisition 
   Bit 1 - Never drops PCCPCH even after N CRC errors during acquisition 
   Bit 2 - Skips GPS reporting
   Bit 3 - Enables MCPM debug API

   Bit 4 - Skips setting up CME at DCH add 
   Bit 5 - Converts crash into F3 for lost searches */

/*===========================================================================

                        FEATURE VALIDATIONS

===========================================================================*/



/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
                L1 MANAGER TASK SIGNAL DECLARATIONS
---------------------------------------------------------------------------*/
#define L1_RPT_WATCHDOG_HB_SIG             0x00000001  /* Watchdog           */
#define L1_EXT_CMD_Q_SIG                   0x00000002  /* Protocol Command Q */
#define L1_LOCAL_CMD_Q_SIG                 0x00000004  /* Local Command Q    */
#define COMMAND_DONE_SIG                   0x00000008

#define L1_CPHY_ACQ_CELL_TRANS_TIMEOUT_SIG 0x00000010  
#define L1_SLEEP_ABORT_TIMEOUT_SIG         0x00000020  /* Sleep abort timeout  */
#define L1_MSGR_SIG                        0x00000040  /* Used for MSGR interactions */
#define RF_TUNE_DONE_SIG                   0x00000080

#define MDSP_DELAY_TIMER_SIG               0x00000100
#define L1_RF_TEMP_COMP_TIMER_SIG          0x00000200
#define L1_ENA_CELL_RESEL_TIMER_SIG        0x00000400
#define L1_PERIOIDC_LOG_STATUS_SIG         0x00000800

#define L1_CM_MEAS_DONE_SIG                0x00001000
/* These signals are defined in task.h     0x00002000  ** TASK_OFFLINE_SIG   **
** and are global and should not be        0x00004000  ** TASK_STOP_SIG      **
** reused by individual tasks!!!           0x00008000  ** TASK_START_SIG     */

#define SLEEP_PERIOD_COMPLETE_SIG          0x00010000  /* End of sleep       */
#define DRX_PRL_RQ_DL_WRKLP_DONE_SIG       0x00020000
#define WL1_TIMEOUT_SIG                    0x00040000  /* mDSP init, TRM req */
#define L1_AUTO_BLER_MEAS_TIMER_SIG        0x00080000

#define L1_OUT_OF_SERVICE_TIMER_SIG        0x00100000
#define L1_NV_CMD_SIG                      0x00200000
#define L1_NO_LOCK_TIMER_SIG               0x00400000
#define __RESERVED_FOR_WL1_RPC_1__         0x00800000

#define L1_STATE_CLEANUP_TIMEOUT_SIG       0x01000000
#define L1_CPHY_SETUP_TIMEOUT_SIG          0x02000000
#define __RESERVED_FOR_WL1_RPC_2__         0x04000000 
#define SRCH_ABORT_ALL_TIMEOUT_SIG         0x08000000  /* Srch abort timeout */

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)
#define WL1_QXTA_TIMER_EXPIRY              0x10000000 
#endif

#ifdef FEATURE_DUAL_SIM
#define WL1_TRM_LOCK_ACQ_SIG               0x20000000 
#define WL1_TRM_LOCK_FAILED_SIG            0x40000000 
#endif /* FEATURE_DUAL_SIM */

#define L1_MCALWCDMA_PROC_SIG              0x80000000

#define MDSP_AGC_TUNE_DONE_SIG            MDSP_DELAY_TIMER_SIG

#define WL1_CELL_DECFG_DONE_SIG           MDSP_DELAY_TIMER_SIG

#define SRCH_NBREJ_WAIT_SIG               SRCH_ABORT_ALL_TIMEOUT_SIG

#ifdef FEATURE_DUAL_SIM


extern uint32 wl1_ds_track_fw_error;
#endif



/* If the define is called from task lock then it will not print F3 */

#define WCDMA_TASKLOCK_MSG3( LVL, FMT, A, B, C ) \
        if(wl1m_ext_cntrl_params[wsub_id].wl1_print_msgs_around_tasklock)           \
        WL1_MSG3(LVL, FMT, A, B, C)

#ifdef FEATURE_DUAL_WCDMA
/* MACRO to get the SUB ID based on Client */
#define WL1M_GET_SUB_ID_FROM_WMGR_CLIENT() ((client == WMGR_CLIENT_UMTS) ? WCDMA_SUB_ID_0 : WCDMA_SUB_ID_1)
#define WL1M_GET_WMGR_CLIENT_FROM_SUB_ID() ((wsub_id == WCDMA_SUB_ID_0) ?  WMGR_CLIENT_UMTS: WMGR_CLIENT_UMTS2)
#else
#define WL1M_GET_SUB_ID_FROM_WMGR_CLIENT()  (WCDMA_SUB_ID_0)
#endif

/******************************************************************************
**                ADD NEW SIGNALS JUST ABOVE THIS COMMENT!!!                 **
** Make sure they don't match the signals described just below this comment! **
******************************************************************************/

/* These signals are also defined          0x20000000 ** VS_OP_COMPLETE_SIG  **
** in task.h and MAY be global and         0x40000000 ** FS_OP_COMPLETE_SIG  **
** should be verified to be OK to
** reuse here, if absolutely necessary.  Consider yourself warned!!!         */


#define L1_OUT_OF_SERVICE_TIME_FACH        4000        /* 4 seconds in FACH */
#define L1_OUT_OF_SERVICE_TIME_FACH_IN_FRAMES   400         /* 4 seconds in FACH */
#define L1_OUT_OF_SERVICE_TIME_PCH         12000       /* 12 seconds in PCH */

/* Timeout for state cleanup during Idle, Stop, Deactivate or Suspend cmd   */
#define L1_STATE_CLEANUP_TIMEOUT           200

/* Timeout for phychan setup request */
#define L1_CPHY_SETUP_TIMEOUT             (L1_RRC_CPHY_SETUP_TIMEOUT - L1_RRC_CPHY_SETUP_TIMEOUT_REMOVE_OVERHEAD)

/* Timeout for Cell Transititon Request */
#define L1_CPHY_ACQ_CELL_TRANS_TIMEOUT 500

/* Timeout for searcher abort command */
#define SRCH_ABORT_ALL_TIMEOUT             20 /* 20 ms should be enough */

/* Used when waiting for mDSP confirmations.  Reducing timeouts from 5 seconds
** to 1 second, but keep both around to avoid compilation problems. */
#define ONE_SECOND                         1000
#define FIVE_SECONDS                       5000

/*Maximum no. of barred cells*/
#define WL1_MAX_BARRED_CELLS 12 /*Just an arbitary no. 2 * ASET size*/

/* Buffers to be placed on the l1_local_free_q. */
#define L1_LOCAL_NUM_BUFS   60

/* Bufers to be placed on the local_wl1m_cntrl->wl1m_msgr_free_q */
#define WL1_MSGR_NUM_BUFS   20


/* These are states of operation of WCDMA L1 */
typedef enum
{
  L1M_IDLE = 0,
  L1M_FS,
  L1M_ACQ,
  L1M_BCH,
  L1M_PCH,
  L1M_FACH,
  L1M_DCH,
  L1M_DEACTIVE,
  L1M_PCH_SLEEP,
  L1M_DEEP_SLEEP,
  L1M_STOPPED,
  L1M_SUSPENDED,
  L1M_PCH_BPLMN,
  L1M_BCH_SLEEP,
  L1M_NUM_STATES
} l1m_state_enum_type;


/* These are Types of Images that can be loaded by WCDMA L1 */
typedef enum
{
  WL1_FW_INVALID_IMAGE,
  WL1_FW_PAGE_IMAGE,
  WL1_FW_TRAFFIC_IMAGE,
  WL1_FW_MEAS_IMAGE,
  WL1_FW_SLEEP_IMAGE,
  WL1_FW_UNLOAD_IMAGE,
  WL1_FW_MAX_NUM_IMAGES
} wl1_fw_image_enum_type;

/* these are the submodules of layer 1 */
typedef enum
{
  SRCH_MODULE,
  DL_MODULE,
  UL_MODULE,
  SEQ_MODULE,

  SRCHPCH_GSM_MODULE,

  L1_CM_GSM_MEAS_MODULE,
  L1_CM_INTERF_MEAS_MODULE,
  L1_LTE_MEAS_MODULE,

  L1_HS_CFG_MODULE,

  E_DL_MODULE,
  E_UL_MODULE,

  CPC_DTX_MODULE,

  L1_CME_MODULE,
  L1_RXD_MODULE,
  FET_MODULE,
  /* CM cleanup module. With this module, we will ensure that CM is completely frozen before UL, DL cleanup is triggered */
  L1_CM_MODULE,

  SCCH_ORDER_MODULE,

  /*New module to perform CPC/EDRX cleanup. Especially introduced to do HS_RACH module cleanup ahead */
  CPC_EDRX_MODULE,

  HS_RACH_MODULE,
  /* Common modules part of FACH/DCH Cleanup procedures */
  COMMON_CONN_STATE_MODULES,

  /* Make sure FMO is the first module that is suspended. 
   * In case of EDRX, if FMO isnt suspended before EDRX cleanup 
   * there is a possibility that one FMO can sneak in 
   * before updating the measurement occasion.
   */
  FMO_MODULE,

  WL1_MAX_MODULE,

  NO_CLEANUP_CMD_TO_ISSUE,

  CLEANUP_DONE

} l1_module_enum_type;

/*--------------------------------------------------------------------------
  Macros for Suspend periodic ops
---------------------------------------------------------------------------*/
typedef enum
{
  CPHY_SUSPEND_PERIODIC_OPS_CME,
  CPHY_SUSPEND_PERIODIC_OPS_TRIAGE,
  CPHY_SUSPEND_PERIODIC_OPS_IGNORE_SCCH_ORDER,
  CPHY_SUSPEND_PERIODIC_OPS_IGNORE_CPC_SCCH_ORDER, 
  CPHY_SUSPEND_PERIODIC_OPS_CM,
  CPHY_SUSPEND_PERIODIC_OPS_SEARCHER,
  CPHY_SUSPEND_PERIODIC_OPS_FMO,
  CPHY_SUSPEND_PERIODIC_OPS_WL1DLDRXCTRL,
  CPHY_SUSPEND_PERIODIC_OPS_QSET_OPS,
  CPHY_SUSPEND_PERIODIC_OPS_SEC_CARRIER,
  CPHY_SUSPEND_PERIODIC_OPS_FET_DISABLE,
  CPHY_SUSPEND_PERIODIC_OPS_RACH,
  CPHY_SUSPEND_PERIODIC_OPS_WRM,
  CPHY_SUSPEND_PERIODIC_EVENTS,
  CPHY_SUSPEND_PERIODIC_OPS_EDL_CONFIG,
  CPHY_SUSPEND_PERIODIC_OPS_UL_DPCH_CONFIG,
  CPHY_SUSPEND_PERIODIC_OPS_SFN_DECODE,
  CPHY_SUSPEND_PERIODIC_OPS_DONE,
  CPHY_SUSPEND_PERIODIC_OPS_NO_CMD_TO_ISSUE,
  CPHY_SUSPEND_PERIODIC_OPS_MAX
} l1m_suspend_client_enum_type;

/* Sets the dependency among modules while performing suspend periodic ops d*/
extern uint32 wl1m_suspend_periodic_ops_dependency_mask[CPHY_SUSPEND_PERIODIC_OPS_MAX];

/* this macro sets all the bits in the L1 module cleanup done mask */
#define INIT_CLEANUP_MODULE_DONE() \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)SRCH_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)DL_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)UL_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)SEQ_MODULE)))

#define INIT_ACQ_BCH_CLEANUP_MODULE_DONE() \
  INIT_CLEANUP_MODULE_DONE(); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_RXD_MODULE)));

/* this macro sets all the bits in the L1 module cleanup done mask for PCH state*/
#define INIT_PCH_CLEANUP_MODULE_DONE() \
  INIT_CLEANUP_MODULE_DONE(); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)SRCHPCH_GSM_MODULE)));\
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_LTE_MEAS_MODULE)));

/* this macro sets all the bits in the L1 module cleanup done mask for FACH state*/
#define INIT_FACH_CLEANUP_MODULE_DONE() \
  INIT_CLEANUP_MODULE_DONE(); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_HS_CFG_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_CM_INTERF_MEAS_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_CM_GSM_MEAS_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_LTE_MEAS_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)FMO_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_RXD_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_CME_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)HS_RACH_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)CPC_EDRX_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)COMMON_CONN_STATE_MODULES)))

#ifdef FEATURE_WCDMA_EFACH_MSIM
#define MODULE_CLEANUP_NEEDED(module)\
  (((wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask & ((uint32) (1 << (uint32)module))) >> (uint32)module))
#endif /* FEATURE_WCDMA_EFACH_MSIM */
/* this macro sets all the bits in the L1 module cleanup done mask for DCH state*/
#define INIT_DCH_CLEANUP_MODULE_DONE() \
  INIT_CLEANUP_MODULE_DONE(); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_CM_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_CM_GSM_MEAS_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_CM_INTERF_MEAS_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_LTE_MEAS_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_RXD_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_CME_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_HS_CFG_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)E_DL_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)E_UL_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)CPC_DTX_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)FET_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)SCCH_ORDER_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)CPC_EDRX_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)COMMON_CONN_STATE_MODULES)));

/* this macro clears the bit in the L1 module cleanup done mask associated
   with the passed in L1 module */
#define MODULE_CLEANUP_DONE(module) \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask &= ~((1u) << ((uint32) module))); \
  (l1msetup_ext_cntrl_params[wsub_id].l1m_ctrl_db.module_cleanup_pending_bmsk &= ~((1u) <<(uint32) module))

/* This macro sets all the bits in the L1 module cleanup done mask */
#define MODULE_SET_ALL_CLEANUP_DONE() \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask = 0)

/* this macro detects that all module cleanup is complete by checking if the
   L1 module cleanup done mask is equal to 0 */
#define L1_MODULE_CLEANUP_COMPLETE() \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask == 0) && (l1msetup_ext_cntrl_params[wsub_id].l1m_ctrl_db.module_cleanup_pending_bmsk == 0)

#define DL_MODULE_CLEANUP_PENDING() \
  ((wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask & (1 << DL_MODULE)) || \
  (l1msetup_ext_cntrl_params[wsub_id].l1m_ctrl_db.module_cleanup_pending_bmsk & (1 << DL_MODULE)))

/* Init DL and SRCH module cleanup flags (used for SIB mod) */
#define L1_INIT_MODULE_CLEANUP_DONE_FOR_SIB_MOD( )               \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask = ((uint32) (1 << (uint32)SRCH_MODULE))); \
  (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask |= ((uint32) (1 << (uint32)L1_CM_INTERF_MEAS_MODULE)));

/* This macro gives the pending mask for the given module*/
#define MODULE_CLEANUP_PENDING(module)\
    ((l1msetup_ext_cntrl_params[wsub_id].l1m_ctrl_db.module_cleanup_pending_bmsk & ((1u) << ((uint32)module))) || \
    (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask & ((1u) << ((uint32)module))))

/* This macro set the pending mask for the given module*/
#define SET_PENDING_MODULE(module)\
    l1msetup_ext_cntrl_params[wsub_id].l1m_ctrl_db.module_cleanup_pending_bmsk |= ((1u) <<(uint32)module)

#define L1_CONN_STATE_CLEANUP_IN_PROG() \
    (wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask || l1msetup_ext_cntrl_params[wsub_id].l1m_ctrl_db.module_cleanup_pending_bmsk)

#define SET_L1_IN_HHO_STATUS(hho_status)\
    l1msetup_ext_cntrl_params[wsub_id].l1m_ctrl_db.l1_in_hho = (hho_status)

#define GET_L1_IN_HHO_STATUS() l1msetup_ext_cntrl_params[wsub_id].l1m_ctrl_db.l1_in_hho

#define WL1_SLEEP_ABORT_IN_PROGRESS() wl1m_ext_cntrl_params[wsub_id].drx_sleep_abort_in_progress

#define WL1_RESUMING_AFTER_LTA_GAP(wsub_id) \
   (l1m_resuming_wcdma(wsub_id) && (wl1m_ext_cntrl_params[wsub_id].l1m_resume_cause == L1_IRAT_DUAL_SIM_TUNEAWAY))

#define SET_RF_INIT_QUEUE_STATUS(status) wl1m_ext_cntrl_params[wsub_id].rf_init_queue_status = status

#define GET_RF_INIT_QUEUE_STATUS() wl1m_ext_cntrl_params[wsub_id].rf_init_queue_status

/* Enum type to control various ES2 and future features through a global flag, 
 * Each enumeration value is a power of 2 to index a global bitmask for each feature
 */
typedef enum
{
  WL1_NV_OVERRIDE_RXD_DISABLE = 1,
  WL1_NV_OVERRIDE_FET_DISABLE = 2,
  WL1_NV_OVERRIDE_DSR_DISABLE = 4
} wl1m_feature_nv_override_enum_type;

/* Global flag to disable RxD, FET and DSR for ES1 release*/


typedef struct
{
  l1_local_cmd_hdr_type hdr;

  l1_module_enum_type module;

} module_cleanup_done_cmd_type;

typedef struct
{
  l1_local_cmd_hdr_type hdr;

  l1_module_enum_type module;

} wl1_conn_state_module_cleanup_cmd_type;

typedef struct
{
  l1_local_cmd_hdr_type hdr;

  cphy_setup_cmd_id_enum_type setup_cmd_id;

} wl1_setup_done_cmd_type;

typedef struct
{
  l1_local_cmd_hdr_type hdr;

  l1m_suspend_client_enum_type client;

} wl1_client_suspend_periodic_ops_cmd_type;

/*
 * Compressed Mode Measurement Control & Report Commands
 */
typedef enum
{
  L1CMMEAS_MEAS_DONE_MIN,
  L1CMMEAS_GSM_RSSI_SCAN,
  L1CMMEAS_GSM_BSIC_VERIFICATION,
  L1CMMEAS_MEAS_DONE_MAX
} l1cmmeas_cm_meas_done_enum_type;
#define  L1CMMEAS_MEAS_DONE_VALID(val)     \
              ( ((val) > L1CMMEAS_MEAS_DONE_MIN ) && ((val) < L1CMMEAS_MEAS_DONE_MAX) )

/*
 * Local Command Structure for Compressed Mode TGPS Run-Time Error
 */
typedef struct
{
  l1_local_cmd_hdr_type hdr;
    /* generic command header */

  l1cmmeas_cm_meas_done_enum_type  meas_complete;
    /* Type of Measurement Completed */

  uint16 event_idx;
    /* event for which reporting is needed */
} l1cmmeas_cm_meas_done_cmd_type;

/*
 * Local Command Structure for Compressed Mode TGPS Run-Time Error
 */
typedef struct
{
  l1_local_cmd_hdr_type hdr;
    /* generic command header */

  uint16  cm_runtime_error_tgpsi;
    /* TGPSI - Transmission Gap Pattern Sequence Index */
  uint16  cm_runtime_error_measid;
    /* measid - Measurement Id associated with TGPSI, if any */
  l1_cm_tg_meas_purpose_enum_type  cm_runtime_error_tgmp;
    /* TGP Measurement Purpose */
} l1cmmeas_cm_runtime_error_cmd_type;

/* Local command structure for EUL CPHY_SETUP_DONE cmd */
typedef struct 
{
  l1_local_cmd_hdr_type hdr;
  boolean status;
} eul_cphy_setup_local_cmd_type;

/* Local command structure for EUL DL COMB_CHAN_CFG cmd */
typedef struct 
{
  l1_local_cmd_hdr_type hdr;
  uint16 dummy;
} eul_dl_comb_chan_cfg_cmd_type;

/* Local command structure for EUL DL FRM_BDRY cmd */
typedef struct 
{
  l1_local_cmd_hdr_type hdr;
  uint16 dummy;
} eul_dl_frm_bdry_cmd_type;

/* Local command structure for EUL DL PHYCHAN_DONE cmd */
typedef struct 
{
  l1_local_cmd_hdr_type hdr;
  uint16 dummy;
} eul_dl_phychan_done_cmd_type;

/* Local command structure for EUL UL PHYCHAN_DONE cmd */
typedef struct 
{
  l1_local_cmd_hdr_type hdr;
  uint8 carr_idx;
  uint8 dummy;
} eul_ul_phychan_done_cmd_type;

typedef enum
{
  EUL_DATA_RATE_Q6_CONTROL_ACTION_EARLY_UP,
  EUL_DATA_RATE_Q6_CONTROL_ACTION_UP,
  EUL_DATA_RATE_Q6_CONTROL_ACTION_DOWN,
  EUL_DATA_RATE_Q6_CONTROL_ACTION_HOLD,
  EUL_DATA_RATE_Q6_CONTROL_ACTION_NUM_ACTIONS
}eul_data_rate_q6_control_action_e_type;

typedef struct
{
  l1_local_cmd_hdr_type hdr;
  eul_data_rate_q6_control_action_e_type eul_q6_action;
}eul_q6_action_cmd_type;

/*Local command structure for EDL LOGGING CMD*/
typedef struct
{
  l1_local_cmd_hdr_type hdr;
  uint16 dummy;
}edl_logging_local_cmd_type;

typedef struct 
{
  l1_local_cmd_hdr_type hdr;
  eul_ul_logging_done_cmd_enum_type log_type;
} eul_logging_local_cmd_type;

#ifdef FEATURE_WCDMA_CPC_DRX
typedef struct
{
  l1_local_cmd_hdr_type hdr;
  /*buffer idx to be flushed */
  uint8 buff_idx;
}wl1_dldrx_log_process_cmd_type;

/* Local command structure for L1_DLDRX_CFG_DONE_CMD */
typedef struct
{
  l1_local_cmd_hdr_type hdr;
  /* pending config mask for start_stop cmd mask */
  uint8 pending_cfg_mask;
  uint16 activation_time_cfn;
}wl1_dldrx_drx_cfg_done_cmd_type;

/* Local command structure for L1 DLDRX CFG LOG CMD */
typedef struct
{
  l1_local_cmd_hdr_type hdr;

  /** input CPC DRX request mask */
  uint8 input_cpc_drx_req_mask;
  /** HS SCCH Reception pattern length */
  uint8 hs_scch_rec_patt_length;
  /** Flag to determine whether grant monitoring is enabled or
   *  not */
  boolean grant_monitoring_en;
  /** HS SCCH inactivity threshold - max value 512*/
  uint16 hs_scch_inact_thresh;
  /** Grant monitor inactivity threshold - max value 512 */
  uint16 grant_monitor_inact_thresh;
  /** Enabling delay - max value 128 */
  uint16 enabling_delay;
  /** DTX DRX offset - max value 159 */
  uint16 dtx_drx_offset;
  /** CFN DRX offset */
  uint16 cfn_drx_offset;
}wl1_dldrx_cfg_log_cmd_type;

typedef struct 
{
  /*Sw requested reception this slot*/
  uint8 swReqReception;
  /*F-DPCH was received this slot*/
  uint8 fdpchReception;
  /*scch was received this slot*/
  uint8 scchReception;
  /*CM was active this slot*/
  uint8 cmActive;
  /*TC warming up this slot*/
  uint8 tcWarmup;
  /*AGC warming up this slot*/
  uint8 agcWarmup;
  /*tc rake enabled this slot*/
  uint8 tcRakeEnabled;
  /*EQ enabled this slot*/
  uint8 eqEnabled;
  /*RX chain state for this slot
    0 - Rx chain was off
    1 - RX chain was turned on but waiting for warmup to complete
    2 - RX chain turned on and ready to receive data
    3 - Unused*/
  uint8 rxChainOn;
  /* Small gap in this slot */
  uint8 smallGap;
  /* Debug mode in this slot */
  uint8 debugMode;
}wl1_dl_drxctrl_slot_level_info_struct_type;

/*! @brief WCDMA CPC DRX Events subframe logging  */
typedef struct
{
  /*scch order received this subframe*/
  uint8 scchOrdReceived;
  /*scch demod enabled - whether scch demod was enabled during this subframe*/
  uint8 scchDemodEnabled;
  /*SCCH CRC state 0 - indicates failure 1 - indicates pass */
  uint8 scchCrcStatus;
  /*log record subframe number of the scch max = 1280*/
  uint16 scchSubfrNum;
  /*Current value of inactivity counter*/
  uint16 inactCntrVal;
  /* Bitmask for Rx Blanking for 3 slots of a subframe */
  uint8 multiSimGapInd;
  /* slot level info array*/
  wl1_dl_drxctrl_slot_level_info_struct_type slotLevelInfo[3];
}wl1_dl_drxctrl_subframe_info_struct_type;

typedef struct 
{
  /*header*/
  l1_local_cmd_hdr_type hdr;
  /*cfn cnt*/
  uint8 cfn_cnt;
  /*actual cfn*/
  uint8 actual_cfn;
  /*scch pattern ind*/
  boolean scch_pattern_ind;
  /*logged subframe*/
  uint16 logged_subfr;
  /* firmware information */
  wl1_dl_drxctrl_subframe_info_struct_type fw_info;
} wl1_dldrx_process_mdsp_evt_log_type;
#endif

typedef struct
{
  l1_local_cmd_hdr_type hdr;
  uint16 dummy;
} wl1_resume_idle_srch_cmd_type;

typedef struct 
{
  /*header*/
  l1_local_cmd_hdr_type hdr;
  /* DC local command */
  HS_CHANNEL_CFG_DONE_POST_CB_FUNC_TYPE *cb_func;
}hs_dc_cfg_cb_cmd_struct_type;

#ifdef FEATURE_WCDMA_HS_FACH_DRX
/* local cmd for use by wl1dldrxsubctrl for entity handling*/
typedef struct
{
  l1_local_cmd_hdr_type hdr;
  uint8 cmd_id;
  uint16 entity_type;
  boolean status;
}wl1_edrx_action_local_cmd_type;
typedef struct
{
  l1_local_cmd_hdr_type hdr;
}wl1_srchfachdrx_update_wkup_cmd_type;

typedef struct
{
  l1_local_cmd_hdr_type hdr;
  uint8 log_pkt_type;
  uint8 edrx_evt_action;
  uint16 dhrnti;
  uint16 drx_cycle_len_fr;
  uint16 rx_burst_fr;
  uint16 t321_ms;
  uint16 log_flush_idx;
  boolean drx_interruption;
}wl1_edrx_log_submit_cmd_type;

typedef struct
{
  l1_local_cmd_hdr_type hdr;
  boolean cme_action;
}wl1_edrx_cme_action_local_cmd_type;

#endif /* FEATURE_WCDMA_HS_FACH_DRX */


typedef struct
{
  l1_local_cmd_hdr_type        hdr;         /* generic command header */

  tcxomgr_restriction_type     restriction;       /* Indicates the restriction type */
  
  /* Indicates whether the restriction needs to be acknowledged or 
  ** reported to tcxomgr*/
  wl1_tcxomgr_ack_type             ack_tcxomgr; 

} tcxo_restriction_local_cmd_type;


#ifdef FEATURE_DUAL_SIM
typedef struct 
{
  /*header*/
  l1_local_cmd_hdr_type hdr;

} wl1_wkupmgr_cancel_cmd_type;
#endif

/* Local command structure for WL1_SFN_CFN_SANITY_CHECK_CMD. */
typedef struct
{
  /*header*/
  l1_local_cmd_hdr_type hdr;
  /*decoder SW DOB status FIFO index*/
  uint16 fifo_idx;
} wl1_sfn_cfn_sanity_check_cmd_type;

typedef struct
{
  /*header*/
  l1_local_cmd_hdr_type hdr;
  uint16 resource_id;
} wl1_handle_pending_cmd_type;

typedef struct
{
  /* generic command header */
  l1_local_cmd_hdr_type hdr;
  /* Activate/deactivate rxd status */
  uint8 de_activation_status;
} wl1_dl_inform_rxd_upon_scch_ord_cmd_type;

typedef struct
{
  l1_local_cmd_hdr_type hdr;
  boolean freeze;
}wl1_dl_drxctrl_freeze_unfreeze_lcmd_struct_type;

#if defined FEATURE_WCDMA_QSH_DUMP|| defined FEATURE_WCDMA_QSH_EVENT_METRIC
typedef struct
{
  l1_local_cmd_hdr_type hdr;
  qsh_client_cb_params_s wl1_qsh_lcmd_params;
}wl1_qsh_params_lcmd_struct_type;
#endif /* defined FEATURE_WCDMA_QSH_DUMP|| defined FEATURE_WCDMA_QSH_EVENT_METRIC */

typedef struct
{
  l1_local_cmd_hdr_type hdr;
  uint32 cmd_mask;
} wl1_dsr_cmd_type;

typedef struct
{
  l1_local_cmd_hdr_type hdr;
  wl1_rfm_wakeup_chain_type wkup_chain_type;
}wl1_rf_wkup_chain_cmd_type;

/*wl1m message passed with TRM passive switch command*/
typedef struct
{
  /*l1m command header*/
  l1_local_cmd_hdr_type hdr;
  trm_ant_switch_cb_info_type ant_cfg_info;
} wl1_ant_switch_trm_cmd;

#ifdef FEATURE_WCDMA_CPC_DRX
typedef struct
{
  l1_local_cmd_hdr_type hdr;

  uint8 entity_id;
  uint8 req_type;
  uint8 sleep_type;
  uint8 occasion_cfn;
  uint8 tick_cnt;
} wl1_dldrx_sched_update_lcmd_struct_type;
#endif /* FEATURE_WCDMA_CPC_DRX */

/*----------------------------------------------------*/
/*  Timeout Handler definations   */
/*----------------------------------------------------*/
#define WL1_TIMEOUT_HLDR_LOCK()      REX_ISR_LOCK(&wl1m_int_cntrl_params[wsub_id].wl1_timeout_hldr_lock)
#define WL1_TIMEOUT_HLDR_UNLOCK()  REX_ISR_UNLOCK(&wl1m_int_cntrl_params[wsub_id].wl1_timeout_hldr_lock)

typedef enum
{
  WL1_TIMEOUT_HLDR_SCCH_ORDER_CLIENT,
  WL1_TIMEOUT_HDLR_SRCH_CLIENT,
  WL1_TIMEOUT_CME_CLIENT,
  WL1_TIMEOUT_RXD_CLIENT,
  WL1_TIMEOUT_SRCH_MODE_CHANGE_REQ_CLIENT,
  WL1_TIMEOUT_HLDR_PRE_CPHY_SCCH_ORDER_CLIENT,
  WL1_TIMEOUT_DSR_CLIENT,
  WL1_CME_IRPM_ENABLE_CLIENT,
#ifdef FEATURE_WCDMA_VLC
  WL1_TIMEOUT_VLC_CLIENT,
#endif
  WL1_TIMEOUT_CELL_DECFG_DURING_SLEEP,
  WL1_ANT_SWITCH_DIV,
  WL1_TIMEOUT_HDLR_W2L_SRCH_CLIENT,
  WL1_TIMEOUT_CELL_CFG_TXD_DET,
  WL1_TIMEOUT_L1M_SEND_CPHY_CNF,
  WL1_MAX_TIMEOUT_HLDR_CLIENTS
}wl1_timeout_hldr_client_enum_type;

typedef void (*WL1_TIMEOUT_HLDR_CB_FUNC)(wsub_id_e_type wsub_id);

typedef struct
{
  uint16 timeout_value;
  boolean graceful_exit;
  tlm_tfn_struct_type start_wrc;
  WL1_TIMEOUT_HLDR_CB_FUNC client_cb;
}wl1_timeout_hldr_client_info_struct_type;

typedef struct
{
  uint32 curr_registered_client_bmask;
  wl1_timeout_hldr_client_info_struct_type client_info[WL1_MAX_TIMEOUT_HLDR_CLIENTS];
}wl1_timeout_hldr_struct_type;

#if defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH) && defined(FEATURE_WCDMA_TO_LTE_FACH)
/* Local command to change QICE (CME) status during FACH W2L measurements */
typedef struct
{
  /*Command header*/
  l1_local_cmd_hdr_type hdr;
  
  /* status of command */
  boolean status;
} srch_fach_cme_status_cmd_type;

#endif

#ifdef FEATURE_WCDMA_CFCM

#define WL1_CFCM_HLDR_LOCK()      REX_ISR_LOCK(&wl1m_int_cntrl_params[wsub_id].wl1_cfcm_hldr_lock)
#define WL1_CFCM_HLDR_UNLOCK()    REX_ISR_UNLOCK(&wl1m_int_cntrl_params[wsub_id].wl1_cfcm_hldr_lock)

/*----------------------------------------------------*/
/*  Definitions for CFCM client handling.   */
/*----------------------------------------------------*/

typedef struct
{
  cfcm_client_e client_id;
  cfcm_monitor_e        monitor_id;
  cfcm_monitor_data_s   monitor_data;
  uint32                monitors_mask;
}wl1_cfcm_cmd_type;

/* CFCM internal client callback type. */
typedef void (*WL1_CFCM_CLIENT_CB_FUNC) (wl1_cfcm_cmd_type cfcm_cmd, wsub_id_e_type wsub_id);

/* Internal CFCM clients. */
typedef enum
{
  WL1_CFCM_CLIENT_QICE,
  WL1_MAX_CFCM_CLIENTS
}wl1_cfcm_client_enum_type;

/* Internal CFCM client information type. */
typedef struct
{
  uint32 active_monitors;
  WL1_CFCM_CLIENT_CB_FUNC client_cb;
}wl1_cfcm_client_info_struct_type;

/* WL1 CFCM structure. */
typedef struct
{
  boolean registration_status;
  uint32 active_clients;
  wl1_cfcm_client_info_struct_type client_info[WL1_MAX_CFCM_CLIENTS];
}wl1_cfcm_hdlr_struct_type;

/* Local command type to handle CFCM cb. */
typedef struct
{
  l1_local_cmd_hdr_type hdr;
  wl1_cfcm_cmd_type info;
}wl1_cfcm_cb_ctrl_cmd_type;
#endif

#ifdef FEATURE_WCDMA_MDSP_RECOVERY

#define WL1_MDSP_RECOVERY_LOCK()      REX_ISR_LOCK(&wl1m_int_cntrl_params[wsub_id].wl1_mdsp_recovery_lock)
#define WL1_MDSP_RECOVERY_UNLOCK()    REX_ISR_UNLOCK(&wl1m_int_cntrl_params[wsub_id].wl1_mdsp_recovery_lock)

typedef enum
{
  WL1M_MDSP_REC_BCH        = 0,
  WL1M_MDSP_REC_PCH        = 1,
  WL1M_MDSP_REC_FACH       = 2,
  WL1M_MDSP_REC_DCH        = 3,
  WL1M_MDSP_REC_PCH_SLEEP  = 4,
  WL1M_MDSP_REC_NUM_STATES = 5
}wl1m_mdsp_recovery_stat_enum_type;

/* Structure for MDSP recovery. */
typedef struct
{
  /* MDSP recovery NV - 
   * Default value - 0x1.
   * Bit 0 - MDSP Recovery.
   * Bit 1 - EUL Recovery.
   */
  uint16 wl1m_mdsp_recovery_nv_value;
  /* Flag to ensure we do not send est ind again if one is sent as part of 
     recovery. */
  boolean wl1_mdsp_recovery_est_ind_sent;
  /* To keep track of MDSP recovery stats per state. */
  uint32 wl1_mdsp_recovery_stat_per_state[WL1M_MDSP_REC_NUM_STATES];
  /* To keep track of current MDSP recovery state. */
  mdsp_recovery_status_enum_type wl1_mdsp_recovery_status;
  /* To keep track of any IDLE_REQ from RRC. If possible use existing
     IDLE_REQ for MDSP recovery. */
  boolean wl1_mdsp_recovery_idle_in_prog;
  /* To track recovery triggered so far per state in this run. */
  uint8 wl1_num_mdsp_recovery[L1M_NUM_STATES];
  /* Timer used for triggering OOS command again after 8 frames. */
  uint8 wl1_mdsp_recovery_retry_oos_timer;
  /* Reason for MDSP recovery. */
  uint16 wl1m_mdsp_recovery_reason;
}wl1_mdsp_recovery_struct_type;
#endif

typedef struct
{
 l1_local_cmd_hdr_type local_cmd_hdr;
}wrm_ext_cmd_proc_resume_type;

/* WL1_WRM_GRANT_CALLBACK_CMD structure. */
typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  l1_local_cmd_hdr_type   hdr;

  wrm_resource_enum_type resource;

  /*Info about the command posted to TRM to
  **which the current async callback corresponds to*/
  wrm_command_enum_type posted_cmd;

  /*Callback type received for the command posted to TRM*/
  trm_async_callback_type_enum_t cb_type;

  trm_async_callback_data cb_data;

} wl1_wrm_async_callback_cmd_type;

typedef enum{
    /*Used to put RF to sleep after step1.*/
    WL1_SRCHACQ_AFTER_STEP1,

    /*In SSIM this will do power clock on so that subsequent
      ACQ cmds can be handled.
      In DSDS this will put FW to sleep, rel other resources 
      if lock fails after NASTT.*/
    WL1_SRCHACQ_NASTT_FAIL,

    /*In SSIM this will do power clock on.
      In DSDS this will wakeup FW, do rf enter mode.*/
    WL1_SRCHACQ_NASTT_DONE,

    WL1_SRCHACQ_NUM_MODES
}wl1_srchacq_res_rel_rsn_type;

#ifdef FEATURE_WCDMA_DL_QUAL_MEAS_REPORTING
/* DL_QUAL_MEAS_POST_REPORT_CMD structure */
typedef struct
{
  l1_local_cmd_hdr_type hdr;
  wcdma_qual_estimation_report_data data;
} dl_qual_meas_report_cmd_type;
#endif

/* Creates a REX timer handled in the timer context. The 'data' parameter is any
   arbitrary 30-bit value that will be passed to the callback, along with the
   sub ID. Use the macros below to split the sub ID and user data. */
#define wl1_rex_def_timer_cb(timer_ptr, cb_func, data, wsub_id) { \
                                                                  WL1_MSIM_VALIDATE_SUB_ID(); \
                                                                  TIMER_CB_CHECK_DATA(data); \
                                                                  rex_def_timer_ex(timer_ptr, cb_func, TIMER_CB_DATA_CREATE(data, wsub_id)); \
                                                                }

/* Creates a dynamic REX timer handled in the timer context. The 'data' parameter is any
   arbitrary 30-bit value that will be passed to the callback, along with the
   sub ID. Use the macros below to split the sub ID and user data. Note that
   checks are after the rex_create_timer_ex() call because we need its return
   value */
#define wl1_rex_create_dyn_timer_cb(cb_func, data, wsub_id) rex_create_timer_ex(cb_func, TIMER_CB_DATA_CREATE(data, wsub_id)); \
                                                            WL1_MSIM_VALIDATE_SUB_ID(); \
                                                            TIMER_CB_CHECK_DATA(data); \

/* Creates a timer handled in the timer context. The 'data' parameter is any
   arbitrary 30-bit value that will be passed to the callback, along with the
   sub ID. Use the macros below to split the sub ID and user data. */
#define wl1_timer_def_cb(timer_ptr, cb_func, data, wsub_id) { \
                                                              WL1_MSIM_VALIDATE_SUB_ID(); \
                                                              TIMER_CB_CHECK_DATA(data); \
                                                              timer_def(timer_ptr, NULL, NULL, NULL, cb_func, TIMER_CB_DATA_CREATE(data, wsub_id)); \
                                                            }

/* 2 bits reserved in callback data for sub ID */
#define TIMER_CB_DATA_WSUB_ID_LEN 2
#define TIMER_CB_DATA_WSUB_MASK   0x00000003
#define TIMER_CB_DATA_USER_MASK   0x3FFFFFFF

/* Create callback data value from user data and sub ID */
#define TIMER_CB_DATA_CREATE(user_data, wsub_id) ((user_data << TIMER_CB_DATA_WSUB_ID_LEN) | (wsub_id & TIMER_CB_DATA_WSUB_MASK))

/* Extract sub ID and user data from callback data passed to timer callback */
#define TIMER_CB_USER_DATA(cb_data) (((uint32)cb_data >> TIMER_CB_DATA_WSUB_ID_LEN) & TIMER_CB_DATA_USER_MASK)
#define TIMER_CB_WSUB_ID(cb_data) ((uint32)cb_data & TIMER_CB_DATA_WSUB_MASK)

/* Check for valid sub ID */
#define TIMER_CB_CHECK_SUB_ID(wsub_id) if((wsub_id < 0) || (wsub_id >= WCDMA_NUM_SUBS)) { WL1_ERR_FATAL("Invalid sub_id %d", wsub_id, 0, 0); }

/* Check for in-range data */
#define TIMER_CB_CHECK_DATA(data) if((uint32)data & ~(TIMER_CB_DATA_USER_MASK)) { WL1_ERR_FATAL("Timer callback data (0x%X) exceeds 30 bits", data, 0, 0); }


/*--------------------------------------------------------------------------
                      UNION OF ALL L1 LOCAL COMMANDS

--------------------------------------------------------------------------*/

typedef union
{
  l1_local_cmd_hdr_type        hdr;

  srch_done_cmd_type           srch_done;
  srch_ops_cmd_type            srch_ops;
  srch_periodic_meas_cmd_type periodic_meas;
  srch_interrat_periodic_meas_cmd_type    interrat_meas; 
#ifdef FEATURE_WCDMA_CM_LTE_SEARCH
  srch_eutra_periodic_meas_cmd_type    eutra_meas;
#endif
  srch_sfn_wrc_diff_cmd_done_type    srch_sfn_wrc_diff;
  srch_lost_srch_cmd_type      srch_lost_srch;
#ifdef FEATURE_WCDMA_FS_FALSE_PEAK_ELIM_OPT
  srch_fs_proc_done_cmd_type  srch_fs_proc_done;
#endif
  srch_acq_split_action_done_cmd_type srch_acq_split_action_done;
  srchcpcdrx_config_cmd_type srchcpcdrx_config;
  srchcrgsm_local_cmd_type srch_gsm_nset_cmd;

#if defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH) && defined(FEATURE_WCDMA_TO_LTE_FACH)
  /*FACH searcher to handle disable/enable of QICE for W2L*/
  srch_fach_cme_status_cmd_type srch_fach_cme_status_cmd;
#endif

  dl_phch_cfg_done_cmd_type      dl_phch_cfg_done;
  dl_acq_bch_decode_done_cmd_type    dl_acq_bch_decode_done;
  dl_bcch_weak_ind_cmd_type    dl_bcch_send_weak_ind;
  dl_acq_pullin_cmd_type       dl_acq_pullin;
  dl_acq_pullin_evt_cmd_type   dl_acq_pullin_evt;
  dl_acq_pullin_done_cmd_type  dl_acq_pullin_done;
  dl_bch_crc_status_type       dl_bch_crc_status;
  dl_trblk_crc_status_type     dl_trblk_crc_status;

  /* indicates that CRC info available for a transport channel */
  l1qualmeas_crc_update_cmd_type  trch_crc_cmd;


  /* activation time related local command */
  seq_act_time_local_cmd_type  act_time_cmd;

  /* physical channel setup related local commands */
  dl_cphy_setup_local_cmd_type dl_cphy_setup_done;
  ul_cphy_setup_local_cmd_type ul_cphy_setup_done;

  /* cell transition support commands */
  dl_cell_trans_drop_done_cmd_type  dl_cell_trans_drop_done;

  /* indicates that a mDSP command timed out */
  mdsp_sync_timeout_cmd_type   mdsp_cmd_timeout;

  /* local commands for the sleep state machine */
  drx_sleep_cmd_type           drx_sleep_cmd;

  module_cleanup_done_cmd_type module_cleanup_done;

#ifdef FEATURE_L1_LOG_ON_DEMAND
  l1_log_on_demand_type log_on_demand_cmd;
#endif

  /* local command parameters for compressed mode measurement control */
  l1cmmeas_cm_runtime_error_cmd_type  cm_runtime_error_tgpsi;

  /* Command to be sent when Compressed Mode Measurement is Complete */
  l1cmmeas_cm_meas_done_cmd_type      cm_meas_done;

  /* Local command for HSDPA channel configuration done */
  l1_hs_chan_cfg_done_cmd_type hs_chan_cfg_done;

  /* Local command for submitting accumulated log packet */
  l1_hs_log_submit_cmd_type    hs_do_log_submit;

  /* Local command to cleanup DL after HS tear down */
  l1_hs_module_cleanup_after_hs_teardown_cmd_type hs_module_cleanup_after_hs_teardown;


  eul_dl_comb_chan_cfg_cmd_type eul_dl_comb_chan_cfg;
  eul_dl_frm_bdry_cmd_type      eul_dl_frm_bdry;
  eul_dl_phychan_done_cmd_type  eul_dl_phychan_done;
  eul_ul_phychan_done_cmd_type  eul_ul_phychan_done;
  eul_cphy_setup_local_cmd_type eul_cphy_setup_done;
  edl_logging_local_cmd_type  edl_logging_cmd;
  eul_logging_local_cmd_type  eul_logging_cmd;  

  /* The structure to accomodate all R99 UL Log packets 
   * that are to be submitted in task context */
  ul_logging_local_cmd_type ul_logging_cmd;

  /* DRX_MANAGER_CMD for commands from other subsystems, like SRCH.
  ** DRX_ENTITY_CMD for commands for a particular channel state machine. */
  drx_manager_cmd_type  drx_manager_cmd;
  drx_entity_cmd_type   drx_entity_cmd;

  sampserv_mgr_cmd_type sampserv_mgr_cmd;

#ifdef FEATURE_DUAL_SIM
  wrm_ext_cmd_proc_resume_type wrm_ext_cmd_proc_resume_cmd; 
#endif

  tcxo_restriction_local_cmd_type tcxo_restriction;

  dl_mcal_cfg_cmd_type mcal_demod_cfg;
  dl_cell_cfg_post_cmd_type cell_cfg_done;
  dl_cell_cfg_td_det_notify_cmd_type td_notify;

  wl1_demoddrv_dsd_done_cmd_type dl_dsd_done_cmd;
  #if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
  wsrchlte_local_cmd_type wsrchlte_srch_done_cmd;
  wsrchlte_init_cnf_done_cmd_type wsrchlte_int_cnf_done_cmd;
  #endif

#ifdef FEATURE_DUAL_SIM
  /* Wakeup cancellation command field */
  wl1_wkupmgr_cancel_cmd_type wl1_wkupmgr_cancel_cmd;

  /* WL1TRM_TUNEAWAY_LOG_SUBMIT command type */
  wl1trm_tuneaway_log_submit_cmd_type wl1trm_tuneaway_log_submit_cmd;
#endif

  /* WL1_WRM_GRANT_CALLBACK_CMD */
  wl1_wrm_async_callback_cmd_type      async_cb;

#ifdef FEATURE_WCDMA_CPC_DRX
 /** local cmd type to process drx logging */
  wl1_dldrx_log_process_cmd_type drx_log_process_input;
 /** local command to process drx cfg done command */
  wl1_dldrx_drx_cfg_done_cmd_type drx_cfg_done;
  wl1_dldrx_process_mdsp_evt_log_type drx_process_mdsp_evt_log;
 /** local command to process drx cfg log command */
  wl1_dldrx_cfg_log_cmd_type cpcdrx_cfg_logging;
 /* Local command to handle calling drx entity sche update */
  wl1_dldrx_sched_update_lcmd_struct_type drx_sched_update_cmd;
#endif
  wl1_resume_idle_srch_cmd_type resume_idle_srch;
  /** hs dc local */
  hs_dc_cfg_cb_cmd_struct_type hs_dc_cfg_cb;
  /** cme local cmd */
  wl1_cme_ctrl_cmd_type l1_cme_ctrl_cmd;
  /** rxd local cmd */
  wl1_rxd_ctrl_cmd_type rxd_ctrl_cmd;

#ifdef FEATURE_WCDMA_HS_FACH_DRX
  /* local cmd to maintain DRX wakeup timeline for search*/
  wl1_srchfachdrx_update_wkup_cmd_type wl1_srchfachdrx_update_wkup;
  /*local cmd to submit log to QXDM in task context*/
  wl1_edrx_log_submit_cmd_type wl1_edrx_log_submit;
  /* local cmd to perform enable/disable CME */
  wl1_edrx_cme_action_local_cmd_type cme_action;
#endif /* FEATURE_WCDMA_HS_FACH_DRX */

#ifdef FEATURE_WCDMA_HS_RACH
wl1_hs_rach_local_cmd_type wl1_hs_rach_local_cmd_info;
#endif /*FEATURE_WCDMA_HS_RACH*/

  wl1_conn_state_module_cleanup_cmd_type conn_state_module_cleanup_done;
  
  wl1_client_suspend_periodic_ops_cmd_type suspend_periodic_ops_done; 
  
  wl1_atuner_local_cmd_type atuner_cmd;

  wl1_setup_done_cmd_type setup_done_cmd;

  wl1_sfn_cfn_sanity_check_cmd_type sfn_cfn_sanity_check_cmd;

  wl1_handle_pending_cmd_type wrm_pending_cmd;

#ifdef FEATURE_WCDMA_ANT_SWITCH_SUPPORT
  /*Passive switch cmd for DSDA*/
  wl1_ant_switch_trm_cmd trm_passive_ant_switch_cmd;
#endif /* FEATURE_WCDMA_ANT_SWITCH_SUPPORT */

  wl1_dl_inform_rxd_upon_scch_ord_cmd_type wl1_inform_rxd_upon_scch_ord_cmd;

  wl1_dl_drxctrl_freeze_unfreeze_lcmd_struct_type freeze_unfreeze_cmd;
  
#if defined FEATURE_WCDMA_QSH_DUMP|| defined FEATURE_WCDMA_QSH_EVENT_METRIC
  wl1_qsh_params_lcmd_struct_type wl1_qsh_params;
#endif /* defined FEATURE_WCDMA_QSH_DUMP|| defined FEATURE_WCDMA_QSH_EVENT_METRIC */

  wl1_dsr_cmd_type dsr_cmd;
  
  wl1_rf_wkup_chain_cmd_type wl1_rf_wkup_cmd;
  
  #ifdef FEATURE_WCDMA_CFCM
  wl1_cfcm_cb_ctrl_cmd_type l1_cfcm_cb_ctrl_cmd;
  #endif
  eul_q6_action_cmd_type eul_q6_action;

  #ifdef FEATURE_WCDMA_DL_QUAL_MEAS_REPORTING
  dl_qual_meas_report_cmd_type dl_qual_meas_report_cmd;
  #endif

  wl1_hs_dl_update_aps_type aps_info;

#ifdef FEATURE_WCDMA_COEXISTENCE_SW
  cxm_phychan_update_cmd_type cxm_phychan_update_cmd;
#endif
} l1m_local_cmd_type;

/*-------------------------------------------*/
/*   L1 Manager Current State                */
/*-------------------------------------------*/

#define WL1_GET_L1M_STATE() (wl1m_ext_cntrl_params[wsub_id].l1m_state)

/*L1M states in which TRUE sleep (FW/RF sleep and MCPM/SLPC programming) is possible.*/
#define WL1_L1M_SLEEP_STATES() ((WL1_GET_L1M_STATE() == L1M_PCH_SLEEP)|| (WL1_GET_L1M_STATE() == L1M_BCH_SLEEP))

/*Cell Selection can happen only in these L1M states.*/
#define WL1_L1M_IS_IN_CELL_SELECTION() ((WL1_GET_L1M_STATE() == L1M_BCH) || (WL1_GET_L1M_STATE() == L1M_BCH_SLEEP))

/*L1M state in which Discontinous Reception(DRX) for PICH can happen.*/
#define WL1_L1M_IS_IN_IDLE() ((WL1_GET_L1M_STATE() == L1M_PCH_SLEEP)|| (WL1_GET_L1M_STATE() == L1M_PCH))

#ifdef FEATURE_WCDMA_EFACH_MSIM
#define WL1_EFACH_UL_TX_PENDING_QTA() (ulcmd_ext_cntrl_params.ulcmd_ext_cntrl_params_mdump.wl1_ul_rach_trigger_during_qta)
#endif /* FEATURE_WCDMA_EFACH_MSIM */
/*-------------------------------------------*/
/*   Command Queues                          */
/*-------------------------------------------*/

extern void wrm_async_callback_cmd_handler(wl1_wrm_async_callback_cmd_type *async_cb, wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
/*
 * L1 Free floating Command queue.
 * Free floating RRC commands are placed on this queue.
 */


/*===========================================================================

FUNCTION     L1M_RRC_BLOCKING_CMD_PROCESSOR

DESCRIPTION
  This functions processes the Free floating commands received by L1.
   After the command is processed, a CNF to RRC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void l1m_rrc_blocking_cmd_processor(l1_ext_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION    WL1M_TIMEOUT_HANDLER_CHECK_CLIENT_REGISTRATION_STATUS

DESCRIPTION
  This functions processes the Free floating commands received by L1.
   After the command is processed, a CNF to RRC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern boolean wl1m_timeout_handler_check_client_registration_status(wl1_timeout_hldr_client_enum_type client, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION l1_clr_ff_sigs

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

extern void l1_clr_ff_sigs( wsub_id_e_type wsub_id);

extern void l1m_process_rrc_blocking_cmd( wsub_id_e_type wsub_id);

extern void l1_clr_ext_cmd_sig( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WL1_SEND_FF_MSG_TO_CONTROLLER

DESCRIPTION
  This function sends a Free floating message to RRC.
  
DEPENDENCIES
  None.

PARAMETERS
  rrc_cmd_type *cmd_ptr - Pointer to command structure to send to either
                          RRC or FTM (Factory Test Mode).
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void wl1_send_ff_msg_to_controller( rrc_cmd_type *cmd_buf, wsub_id_e_type wsub_id  );
#endif
/*
 * L1 Manager Protocol Command queue.
 * Protocol related commands are placed on this queue.
 */


/*
 * L1 Manager Local Command queue.
 * Local Commands are placed on this queue.
 */


/*
 * L1 Local Free Queue. The Protocol/Local State Processor
 * is responsible in filling up this queue with queue items.
 * The L1 Manager will use this free queue to get a command buffer
 * for a local command and passes the buffer to the client.
 */


/*----------------------------------------------------*/
/*  Last occurence of WL1 processing local commands   */
/*----------------------------------------------------*/



/*
 * The following structure of function pointers are used to define
 * a Protocol Processor. This structure encapsulates all the functions
 * necessary for operation on the Protocol Command.
 * For a particular protocol, implementation should be provided
 * for these functions, and an instance of this type should be
 * defined. This instance will be used by the L1 Manager whenever there
 * is a switch to the Protocol.
 */
typedef struct
{
  /*
   * Pointer to a function that initializes a Protocol Processor.
   * This is called from the L1M Main Loop when a Processor is
   * installed. This function takes a pointer to the Protocol
   * Command Queue, a pointer the Protocol Free Queue, pointer to
   * the Local Command Queue and a pointer to the Local Free Queue.
   * The Processor will initialize the free queues with the desired
   * number of free queue items whose types are known to the Processor
   * and save the pointers to the command queues and the free queues
   * for it's use.
   */
  void (*init)(boolean normal, wsub_id_e_type wsub_id);

  /*
   * Pointer to a function that de-initializes a Processor.
   * This is called from the L1M Main Loop when the currently active
   * Processor is de-installed. This function will de-queue all the
   * command and free queues.
   */
  void (*cleanup)(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

  /*
   * This is the state machine of the Processor called by the L1M
   * processing loop when there is a command on the Protocol Command
   * Queue. This function takes a pointer to the Protocol Command and
   * then acts on this Protocol Command. It may call the cleanup procedure
   * of the Local Processor, and may initialize the Local Command Pointer
   * to point to a new Local Processor. It should initialize the new
   * Local Processor, which will fill up the local free queue. The function
   * is responsible in returning the command pointer to the free queue
   * once the command is completed.
   */
  void (*cmd_processor)(l1_ext_cmd_type* cmd_item_ptr, wsub_id_e_type wsub_id);
} l1_ext_processor_type;

typedef struct
{
  uint32 wl1m_nv_override;
  /*
* L1 Local Free Queue. The Protocol/Local Processor
* is responsible in filling up this queue with queue items.
* The L1 Manager will use this free queue to get a command buffer
* for a local command and passes the buffer to the client.
*/
  q_type l1_local_free_q;

  #ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
/* * L1 Protocol Command queue. The upper layers send protocol
* sub-state commands via this command queue. */
  q_type l1_ff_cmd_q;
  #endif

}wl1m_ext_cntrl_params_mdump_type;

typedef struct
{
  /* Critical section mutex for WCDMA */
  rex_crit_sect_type wcdma_crit_sect;

  /* Total expiry happened since we last accounted for in the barred cell list*/
  rex_timer_cnt_type l1m_tbarred_elapsed_count;
  /* Timer to enable cell reselection evaluation after cell camp */
  rex_timer_type l1_ena_cell_resel_timer;
  rex_timer_cnt_type l1_ena_cell_resel_time_ms;
/*
 * L1 Local Free Queue. The Protocol/Local Processor
 * is responsible in filling up this queue with queue items.
 * The L1 Manager will use this free queue to get a command buffer
 * for a local command and passes the buffer to the client.
 */

  #ifdef FEATURE_WCDMA_SAR_TX_PWR_FILTER
  /* SAR Tx power filtering timer */
  rex_timer_type wl1ulsar_tx_power_timer;
  #endif
  /* flag to indicate that we're about to set up or drop PCCPCH S or N in idle mode
   Used in Idle mode/sleep abort processing. */
  boolean l1_about_to_setup_or_drop_pccpch_in_idle;
  /*Index into l1m_sorted_tbar_list*/
  uint16 l1m_tbar_index;
#ifdef FEATURE_WCDMA_PLT_MODE
  #error code not present
#endif
  #ifdef FEATURE_L1_LOCAL_CMD_DEBUG_TRACE
/* Trace buffer for local cmd */
  uint8 l1_lcmd_trace_cmd_progress_cfn[MAX_CFN_COUNT];
  /* current local commands */
  uint8 l1_lcmd_trace_cur_cmd_id;
  #endif
  /* This Flag will be used to idenitfy the resume cause. This flag was introduced
** to selectively enable or disable the GSM clock from WL1.
*/
  l1_suspend_resume_cause_enum_type l1m_resume_cause;

  /* In case of fast LTA resume, DPCH/UL gets completed pre-maturely in s/w to allow HS/EUL/EDL to start with
  There is high chances of getting next CPHY from RRC before EUL/EDL configuration gets completed in FW
  which might allow s/w to send reconfiguration before even FW could complete with the actual config
  Delay CPHY_CNF to RRC so that the subsequent reconfig gets delayed */
  boolean delay_cphy_cnf_to_rrc;
  #ifdef FEATURE_DUAL_SIM
  boolean wl1_ignore_wrm_ext_cmd_proc_resume_cmd;
  /* Used when waiting for confirmations from WRM */
  rex_timer_type wrm_timeout_timer;
  rex_crit_sect_type wl1_send_fake_sib_mask_crit_sect;
  boolean wl1_bcch_dec_done;
  boolean drx_aborted_during_dsds_bplmn;
  /* This flag tracks WL1 wake up cancel initiation. We can avoid
   multiple cancelations. */
  rex_crit_sect_type wkupmgr_collision_detect_crit_sect;
  #endif
  /* RRC fills in as_id with every external command to WL1.  But other subsystems,
  ** like GPS, BMC, etc., won't.  Store value from RRC here, for use elsewhere
  ** in WL1. */
  sys_modem_as_id_e_type wl1_sys_modem_as_id;
  /* Timer to countdown the time required for searcher abort */
  rex_timer_type srch_abort_all_timer;
  /*Used to force show SC capability in DC scenarion when network gives bad configuration*/
  rex_timer_type l1_show_SC_capability;

  /* Timer to countdown the 2 second DRX sleep abort timeout */
  rex_timer_type l1_sleep_abort_timer;
    rex_timer_type l1_drx_delayed_timer;
  /* Timer to countdown the time required for phychan setup */
  rex_timer_type l1_cphy_setup_timer;
  /* Timer to countdown the L1 state cleanup */
  rex_timer_type l1_state_cleanup_timer;
  #ifdef FEATURE_TMC_TCXOMGR
  rex_crit_sect_type wl1_tcxo_rpush_crit_sect;
  #endif
  uint32 bringup_debug;
  /* during handling of error scenarios in start_agc repsonse from mdsp,
this variable stores whether stmr needs to be restarted */
  boolean restart_stmr_on_start_agc_failure;
  boolean wl1_print_msgs_around_tasklock;
  boolean drx_sleep_abort_in_progress;
  /*  Global variable for the wcdma_query_tx_power_dbm_swap related timer */
  rex_timer_type wcdma_query_tx_power_timer;
  /* T-barred timer. */
  rex_timer_type wl1_tbarred_timer;
  /* Mutex to serialize rx and tx enable */
  rex_crit_sect_type wl1_rx_and_tx_enable_mutex;

  wl1m_ext_cntrl_params_mdump_type wl1m_ext_cntrl_params_mdump;
  
/*This bitmask is used to determine when all of the L1 modules have completed
their cleanup procedures when making state transitions.*/
  uint32 l1m_module_cleanup_done_mask;
 /* L1 Protocol Command queue. The upper layers send protocol
sub-state commands via this command queue. */
  q_type l1_ext_cmd_q;
  /* RRC can ask for different types of mDSP images to be loaded when it
** activates WL1.  WL1 then asks mDSP Services for the proper image(s) when
** starting up.  WL1 also uses this info when shutting down the mDSP. */
  wl1_mdsp_image_enum_type wl1_mdsp_image;
  #ifdef FEATURE_WCDMA_TCXO_RESOURCE_SUPPORT
  /* Timer used in FACH and DCH to notify VCTCXO SRM and TCXO Mgr 3.0 clients that the
** frequency error (i.e. rotator) is accurate and ready to be read. */
  rex_timer_type wl1_tcxo_freq_err_timer;
  #endif
  /* Variable to see if RF Temp Comp Part1 had to be delayed */
  boolean wl1_delay_rf_temp_comp;
  #if defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)
  /* This flag is set to trigger temp comp after QTA gap.*/
  boolean wl1_delay_temp_comp_for_qta;
  #endif
  /*
 * L1 Local Command queue. The upper layers send local
 * sub-state commands via this command queue.
 */
  q_type l1_local_cmd_q;
  rex_crit_sect_type wl1_page_opt_crit_sect;
  /* Used when waiting for confirmations from mDSP and TRM */
  rex_timer_type wl1_timeout_timer;
  /* Restoring compressed mode gaps after a RESUME is handled a little differently
** from setting them up normally because we keep some state info around from the
** time of the SUSPEND.  Here's a flag to know if we're doing a RESUME or not. */
  boolean l1m_resuming_wcdma_flag;
  /* Keeps track of the current state of the L1 when W gets suspended for LTA. 
      Used especially to know if W is resuming back to FACH/DCH*/
  l1m_state_enum_type l1m_resume_state;
  /* Keeps track of the current state of the L1 State Machine. */
  l1m_state_enum_type l1m_state;
  
  /*set to FALSE when forcing SC capability to nw in case of bad config*/
  boolean l1_is_ue_dc_capable;
  wl1_force_sc_capability_struct_type wl1_force_sc_capability_nv;
/*
 * This variable saves the last CFN at which WL1 task 
 *  got a chance to run and process local commands.
 */
  uint16 l1_local_cmd_last_cfn;
  /* Timer to countdown the time required for cell transition request */
  rex_timer_type l1_cphy_acq_cell_trans_timer;
  /* This variable saves DCH cleanup next state, in case of HSDPA/HSUPA channel
   cleanup is required. DL DPCH channel is torn down after HSDPA/HSUPA channel
   is torn down. DPCH channel teardown need next L1M state */
  l1m_state_enum_type l1m_conn_state_cleanup_next_state;
  #ifdef FEATURE_RUDE_TIMETAG
  #error code not present
#endif
  /* Out of service timer */
  rex_timer_type l1_out_of_service_timer;
  /* set to true when DCH is being torn down */
  boolean l1m_just_completed_call;
  rex_crit_sect_type wl1_local_cmd_handle_crit_sect;
  /*Sorted list of tbarred timers to be run*/
  rex_timer_cnt_type l1m_sorted_tbar_list[WL1_MAX_BARRED_CELLS];
  /* Store Current FW APP */
  wl1_fw_app_type fw_app_status;
  /* Variable to track FW START/STOP cmd reception by FW */
  volatile boolean start_stop_op_pass;
    /*To get the CHIP ID*/
  DalChipInfoIdType chip_id;
      /*! @brief Test START request */
  msgr_client_t wl1_msgr_client;
  msgr_id_t wl1_msgr_rex_q_id;

  /* Used for profiling LTA TL operations*/
  tlm_tfn_struct_type time_profile_end_frc;
  tlm_tfn_struct_type time_profile_start_frc;
  double time_profile_min_suspend_time;
  double time_profile_max_suspend_time;
  double time_profile_avg_suspend_time;
  double time_profile_min_resume_time;
  double time_profile_max_resume_time;
  double time_profile_avg_resume_time;
  /* Used to check whether the RF init is queued in DPC task or not    */
  boolean rf_init_queue_status;
  boolean wl1_1xsrlte_mode_on_other_sub;
}wl1m_ext_cntrl_params_type;

extern wl1m_ext_cntrl_params_type wl1m_ext_cntrl_params[WCDMA_NUM_SUBS];


typedef struct
{
  boolean wl1_mutex_initialized;
  /* Lock for timeout handler */
  rex_crit_sect_type wl1_timeout_hldr_lock;
  /*----------------------------------------------------*/
  wl1_timeout_hldr_struct_type wl1_timeout_handler;
 #ifdef FEATURE_WCDMA_CFCM
  /* Lock for CFCM handler. */
  rex_crit_sect_type wl1_cfcm_hldr_lock;
  /* CFCM handler. */
  wl1_cfcm_hdlr_struct_type wl1_cfcm_handler;
 #endif

 #ifdef FEATURE_WCDMA_MDSP_RECOVERY
  /* Lock for MDSP recovery. */
  rex_crit_sect_type wl1_mdsp_recovery_lock;
  /* MDSP recovery handler. */
  wl1_mdsp_recovery_struct_type wl1_mdsp_recovery_handler;
 #endif
  l1_cmd_enum_type debug_command_from_rrc;
/*WL1 MSGR Command queue. WL1 when registered with
 Message Router as a client can receive messages from any
 external entity/sender via this command queue.*/
  q_type wl1m_msgr_cmd_q;

  #ifdef FEATURE_WCDMA_L1_WDOG_DYNAMIC
  dog_report_type wcdma_l1_dog_rpt_var;
  #endif
  /* RF Driver Temperature Compenstation Timer */
  rex_timer_type l1_rf_temp_comp_timer;
  #ifndef FEATURE_WCDMA_MCPM_DRIVER
  boolean wl1_inc_q6_fw_spd_at_pch_init;
  #endif
  #ifdef FEATURE_TMC_TCXOMGR
  rex_timer_type wl1_update_rgs_timer;
  #endif
  boolean high_low;
/* WL1 MSGR Free Queue. Upon processing the message from
 the command queue it is the responsibility of the receiver to
 free up the queue. */
  q_type wl1m_msgr_free_q;
  /* 
 * Mutex definitions for extenal and local cmd queues and
 * signal management
 */
  rex_crit_sect_type wl1_ext_cmd_handle_crit_sect;
  /* indicate if interf meas result optimization is enabled or not */
  boolean wl1_nv_interf_meas_res_opt_ind;
  boolean wl1m_wfw_intf_ready;
  /* indicate supported release version type */
  l1_nv_rel_ind_enum_type wl1_nv_rel_ind;
  /* Debug stuff for use only during debug sessions.  Delete soon. */
  boolean f_send_debug_primitive;
  boolean f_mdsp_and_rf_awake;
  l1_stop_cause_enum_type debug_stop_type;
  #ifdef FEATURE_L1_LOCAL_CMD_DEBUG_TRACE
  /* Trace buffer for local cmd */
  uint8 l1_lcmd_trace_cmd_start_cfn[MAX_CFN_COUNT];
  uint8 l1_lcmd_trace_cmd_stop_cfn[MAX_CFN_COUNT];
  #endif
  #ifdef FEATURE_RRC_L1_ERROR_RECOVERY_P1
  boolean l1m_mdsp_recovery_in_progress;
  #endif
  wl1_msgr_cmd_type wl1_msgr_bufs[WL1_MSGR_NUM_BUFS];

  #ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
  /* flag to indicate that its blocking command or external command 
** 0 indicates external command
** 1 indicates blocking command */
  boolean l1_ff_cmd_recevd;
  #endif
  #ifdef FEATURE_FTM_WCDMA_BER
  /* Used to remember the mode of the phone when an external command is
** received.  Indicates whether command came from RRC or FTM. */
  boolean wl1_ue_in_ftm_mode;
  #endif
  l1m_local_cmd_type l1_local_bufs[L1_LOCAL_NUM_BUFS];
  rex_timer_type l1_periodic_log_status_timer;
  /* status from mdsp to the goto_sleep, start_agc and stop_agc commands */
  uint16 mdsp_agc_cmd_status;
  l1m_local_cmd_type *l1m_current_local_cmd;
  #ifdef FEATURE_WCDMA_L1_ZI_OPT
  /* Flag to indicate allocation state of Dynamic buffers */
  boolean wl1_dynamic_buffers_allocated;
  #endif
  /* Flag to check if UE is in SGLTE mode, don't send GL1 init as part of W init if this is TRUE */
  boolean wl1m_is_ue_in_sglte_mode;
  #ifdef FEATURE_RRC_L1_ERROR_RECOVERY_P1
  boolean l1m_allow_to_send_op_err_ind;
  #endif
  /* Store Currently Loaded Image in FW */
  wl1_fw_image_enum_type current_fw_image;
}wl1m_int_cntrl_params_type;

/* Mutex for all one time init */
rex_crit_sect_type wl1_one_time_init_crit_sect;
#define WL1_ONE_TIME_INIT_INTLOCK()  REX_ISR_LOCK(&wl1_one_time_init_crit_sect)
#define WL1_ONE_TIME_INIT_INTFREE()  REX_ISR_UNLOCK(&wl1_one_time_init_crit_sect)

/******************************************************************************
*******************************************************************************
                      EXTERNAL VARIABLE DECLARATIONS
*******************************************************************************
******************************************************************************/

/*--------------------------------------------------------------------------
                 EXTERN VARIABLE:  L1_EXT_STATE_PROCESSOR
--------------------------------------------------------------------------*/
extern const l1_ext_processor_type* l1m_ext_state_processor[L1M_NUM_STATES + 1];

#ifdef FEATURE_WCDMA_TCXO_RESOURCE_SUPPORT
/* Timer used in FACH and DCH to notify VCTCXO SRM and TCXO Mgr 3.0 clients 
** that the frequency error (i.e. rotator) is accurate and ready to be read.
** This happens once a second. */
#define WL1_TCXO_FREQ_ERR_READY_RATE      1000
/* Timeout for Timer used in FACH, BCH and DCH to push the frequency error if we didnt
** send any frequency error samples, because no samples have been collected yet. 
** Meant to be shorter than WL1_TCXO_FREQ_ERR_READY_RATE. */
#define WL1_TCXO_INITIAL_FREQ_ERR_READY   100

#endif


#ifdef FEATURE_DUAL_SIM
#define WL1_SEND_FAKE_SIB_MASK_LOCK()  REX_ISR_LOCK(&wl1m_ext_cntrl_params[wsub_id].wl1_send_fake_sib_mask_crit_sect)
#define WL1_SEND_FAKE_SIB_MASK_UNLOCK()  REX_ISR_UNLOCK(&wl1m_ext_cntrl_params[wsub_id].wl1_send_fake_sib_mask_crit_sect)
#endif


#define WL1_RX_AND_TX_ENABLE_LOCK()   REX_ISR_LOCK(&wl1m_ext_cntrl_params[wsub_id].wl1_rx_and_tx_enable_mutex)
#define WL1_RX_AND_TX_ENABLE_UNLOCK() REX_ISR_UNLOCK(&wl1m_ext_cntrl_params[wsub_id].wl1_rx_and_tx_enable_mutex)

#define WL1_UE_IN_PLT_MODE() (wl1m_ext_cntrl_params[WL1_DEFAULT_SUB_ID].wl1_ue_in_plt_mode)

/*===========================================================================
FUNCTION     PASS_TCXO_DATA_TO_TCXO_MGR

DESCRIPTION
  This function is called when WCDMA mode is deactivated.  It grabs the
  VCTCXO trk_lo_adj and rotator values and sends them to the TCXO MGR.  It
  may keep the values around or store them in flash or whatever it
  sees fit.  Another RAT, like GSM for example, may then query for these
  values upon initial acquisition to start with a more accurate VCTCXO
  error adjustment instead of a default mid-point assumption.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void pass_tcxo_data_to_tcxo_mgr( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_TCXO_FREQ_ERR_READY

DESCRIPTION
  This function is called periodically in FACH and DCH to notify VCTCXO SRM
  (Shared Resource Manager) and TCXO Mgr 3.0 clients (like GPS) that the 
  frequency error (i.e. rotator) value is accurate and may be read.  
  The client(s) may then retrieve the frequency error from SRM.

  Only do notification if we have at least one finger in lock.  Otherwise
  assume our frequency error is not accurate because we have no fingers
  tracking the downlink.

DEPENDENCIES
  None.

PARAMETERS
  uint32 unused - Not used.  Included to make REX happy.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_tcxo_freq_err_ready(timer_cb_data_type cb_data);

/*===========================================================================
FUNCTION     L1M_QUERY_REL5_SUPPORTED

DESCRIPTION
  This function return boolean based on support of release 5 enabled or not

DEPENDENCIES
  None

RETURN VALUE
  TRUE or FALSE

SIDE EFFECTS
  None
===========================================================================*/
extern boolean l1m_query_rel5_supported( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_REL8
/*===========================================================================
FUNCTION     L1M_QUERY_REL8_SUPPORTED

DESCRIPTION
  This function return boolean based on support of release 8 enabled or not

DEPENDENCIES
  None

RETURN VALUE
  TRUE or FALSE 

SIDE EFFECTS
  None
===========================================================================*/
extern boolean l1m_query_rel8_supported( wsub_id_e_type wsub_id);

#endif /*FEATURE_WCDMA_REL8*/

/*===========================================================================

FUNCTION L1M_PROCESS_EXT_CMD

DESCRIPTION
  This function de-queues a command from the protocol command queue,
  invokes the command processor of the currently active protocol processor
  and passes the command to it.

  If there are no more protocol commands pending on the protocol command
  queue, the protocol command queue signal is cleared.

  This function is called in the context of the L1 thread.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The Protocol Command Queue signal is cleared if there are no
  protocol commands on the Protocol Command Queue.

===========================================================================*/
void l1m_process_ext_cmd( boolean req_for_lock, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1_FREE_CMD_BUF

DESCRIPTION
  This function is used by L1 to free the external command buffer once L1 is
  finished processing the command. This function is executed in the context
  of the caller.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void l1_free_cmd_buf
(
  /* Command buffer to be freed */
  l1_ext_cmd_type* cmd_ptr
);

/*===========================================================================

FUNCTION L1M_GET_LOCAL_CMD_BUF

DESCRIPTION
  This function is used by the internal clients of the L1 Manager to get a
  free command buffer.

DEPENDENCIES
  None

RETURN VALUE
  Pointer to the free buffer.

SIDE EFFECTS
  None.

===========================================================================*/
extern l1m_local_cmd_type* l1m_get_local_cmd_buf( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1M_PUT_QUICK_LOCAL_CMD

DESCRIPTION
  This function provides advantage of processing l1m local commands quickly.
  If we are already in WL1 task context during execution of l1m local command, 
  then we process the local command along with ongoing task. Else, we add the 
  command to the wl1m_ext_cntrl_params[wsub_id].l1_local_cmd_q queue and set signal using l1m_put_local_cmd.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void l1m_put_quick_local_cmd(
  l1m_local_cmd_type* cmd_ptr,
  boolean run_in_l1_task
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1M_PUT_LOCAL_CMD

DESCRIPTION
  This function is used by the internal units of the L1 to submit local
  commands. This function puts the command onto the L1M Local Command
  Queue and sets the Local Command Queue for the L1 task.

  This function is called in the context of the L1M thread or in the
  context of an ISR within the L1.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The Local Command buffer is put on the Local Command Queue and the
  Local Command Queue signal is set.

===========================================================================*/
extern void l1m_put_local_cmd
(
  l1m_local_cmd_type* cmd_ptr
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1_FREE_LOCAL_CMD_BUF

DESCRIPTION
  This function frees the command buffer associated with the local command.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void l1m_free_local_cmd_buf
(
  /* Command buffer to be freed */
  l1m_local_cmd_type* cmd_ptr
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1_GET_RRC_CMD_BUF

DESCRIPTION
  This function gets the command buffer to send a command/confirmation
  to RRC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern rrc_cmd_type* l1_get_rrc_cmd_buf(void);

/*===========================================================================

FUNCTION     WL1_SEND_MSG_TO_CONTROLLER_SUB0

DESCRIPTION
  This function sends a message to the software module that is currently
  controlling WL1.  During normal operation, this is RRC.  During Factory
  Test Mode, this is FTM.  The messages are generally confirmations to
  commands or indications, like measurement data available or out of sync.

DEPENDENCIES
  None.

PARAMETERS
  rrc_cmd_type *cmd_ptr - Pointer to command structure to send to either
                          RRC or FTM (Factory Test Mode).
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void wl1_send_msg_to_controller(rrc_cmd_type *cmd_ptr);

/*===========================================================================

FUNCTION     WL1_SEND_MSG_TO_CONTROLLER

DESCRIPTION
  This function sends a message to the software module that is currently
  controlling WL1.  During normal operation, this is RRC.  During Factory
  Test Mode, this is FTM.  The messages are generally confirmations to
  commands or indications, like measurement data available or out of sync.

DEPENDENCIES
  None.

PARAMETERS
  rrc_cmd_type *cmd_ptr - Pointer to command structure to send to either
                          RRC or FTM (Factory Test Mode).
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void wl1_send_msg_to_controller_for_sub(rrc_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1M_GET_CURRENT_LOCAL_CMD

DESCRIPTION
  This function returns the current local command being processed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern l1m_local_cmd_type* l1m_get_current_local_cmd( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION l1M_PROCESS_LOCAL_CMD

DESCRIPTION
  This function de-queues a command from the local command queue,
  invokes the command processor of the currently active local processor
  and passes the command to it.

  If there are no more local commands pending on the local command
  queue, the local command queue signal is cleared.

  This function is called in the context of the L1M thread.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The Local Command Queue signal is cleared if there are no
  local commands on the Local Command Queue.

===========================================================================*/
void l1m_process_local_cmd( l1m_local_cmd_type *cmd_ptr, boolean force_l1m_task_context, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION    L1M_DISCARD_LOCAL_CMDS

DESCRIPTION
  This function throws away all pending local commands, conditionally.  It
  may or may not discard the command depending upon the current wl1m_ext_cntrl_params[wsub_id].l1m_state
  and what the command actually is.  Some commands we don't want to lose.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1m_discard_local_cmds( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     L1M_IS_CPHY_SETUP_WITH_ACT_TIME_POSTED
    
DESCRIPTION
  This function checks whether a local command has been posted or not for cphy_setup_request
  in the wl1m_ext_cntrl_params[wsub_id].l1_local_cmd_q.

DEPENDENCIES
  None

RETURN VALUE
  Boolean

SIDE EFFECTS
  None
===========================================================================*/
extern boolean l1m_is_cphy_setup_with_act_time_posted( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1_CMD_CONFIRM

DESCRIPTION
  This function sends a confirmation cmd to the upper layer.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void l1_cmd_confirm( rrc_cmd_type* cmd, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1_CMD_ERR_IND

FILE NAME       l1m.c

DESCRIPTION
  This function sends a command error indication to the upper layer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_cmd_err_ind( l1_error_enum_type error, uint8 err_type, l1_cmd_enum_type cmd_id, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1_CPHY_SETUP_CNF_PUT_STATUS

FILE NAME       l1m.c

DESCRIPTION     This function is used by the L1 modules to update the
                l1_cphy_setup_status buffer in the course of performing
                the steps of a CPHY_SETUP_REQ.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    The l1_cphy_status buffer is updated accordingly
===========================================================================*/
void l1_cphy_setup_cnf_put_status
(
  /* this is the type of status to be updated, UL/DL Phychan/TrCH */
  l1_cphy_setup_cnf_stat_type_enum_type stat_type,

  /* this is the channel being referred to cast to a uint8.  The cast allows
     all the various channels to be referenced even though they are of
     differing enum types */
  uint8 ch,

  /* the operation that was performed on the given channel */
  l1_setup_ops_enum_type op,

  /* TRUE if the operation passed */
  boolean pass
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTIONS L1M IDLE STATE FUNCTIONS

DESCRIPTION
  l1m_idle_init
    This fuction does idle state initialization with serach db cleanup
    set to true since serach db cleanup is mandatory only during IDLE state transition

  l1m_idle_init_ds
    This function does all initializations necessary for proper operation
    of the l1 in the IDLE state. This function is called when L1M transitions
    to the L1M_IDLE state.
    All L1 modules perform Idle state initialization procedures.

  l1m_idle_cleanup
    This function is called when L1M is transitioning to another state from
    the L1M_IDLE state. This function calls the idle state cleanup functions
    of the L1 modules and executes all the local commands pending in the
    local queue.

  l1m_idle_cmd_processor
    This function handles all external commands while in the L1M_IDLE state.

===========================================================================*/
void l1m_idle_init( boolean normal , wsub_id_e_type wsub_id);
void l1m_idle_init_ds( boolean normal , boolean skip_search_cleanup ,wsub_id_e_type wsub_id);
void l1m_idle_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_idle_cmd_processor( l1_ext_cmd_type* cmd_ptr , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1M ACQ StATE FUNCTIONS

DESCRIPTION

  l1m_acq_init
    This function does all initializations necessary for proper operation
    of the l1 in the ACQ state.
    All L1 modules perform ACQ state initialization procedures.

  l1m_acq_cleanup
    This function is called when L1M is transitioning to another state from
    the L1M_ACQ state. This function calls the ACQ state cleanup functions
    of the L1 modules and executes all the local commands pending in the
    local queue.

  l1m_acq_cmd_processor
    This function handles all external commands while in the L1M_ACQ state.

===========================================================================*/
void l1m_acq_init( boolean normal , wsub_id_e_type wsub_id);
void l1m_acq_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_acq_cmd_processor( l1_ext_cmd_type* cmd_ptr , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1M FREQ SCAN StATE FUNCTIONS

DESCRIPTION

  l1m_fs_init
    This function does all initializations necessary for proper operation
    of the l1 in the FS state.
    All L1 modules perform FS state initialization procedures.

  l1m_fs_cleanup
    This function is called when L1M is transitioning to another state from
    the L1M_FS state. This function calls the FS state cleanup functions
    of the L1 modules and executes all the local commands pending in the
    local queue.

  l1m_fs_cmd_processor
    This function handles all external commands while in the L1M_FS state.

===========================================================================*/
void l1m_fs_init( boolean normal , wsub_id_e_type wsub_id);
void l1m_fs_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_fs_cmd_processor( l1_ext_cmd_type* cmd_ptr , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTIONS L1M BCH STATE FUNCTIONS

DESCRIPTION
  l1m_bch_init
    This function does all initializations necessary for proper operation
    of the l1 in the BCH state.
    All L1 modules perform BCH state initialization procedures.

  l1m_bch_cleanup
    This function is called when L1M is transitioning to another state from
    the L1M_BCH state. This function calls the BCH state cleanup functions
    of the L1 modules and executes all the local commands pending in the
    local queue.

  l1m_bch_cmd_processor
    This function handles all external commands while in the L1M_BCH state.

===========================================================================*/
void l1m_bch_init( boolean normal , wsub_id_e_type wsub_id);
void l1m_bch_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_bch_cmd_processor(l1_ext_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1M PCH STATE FUNCTIONS

DESCRIPTION
  l1m_pch_init
    This function does all initializations necessary for proper operation
    of the l1 in the PCH state.
    All L1 modules perform PCH state initialization procedures.

  l1m_pch_cleanup
    This function is called when L1M is transitioning to another state from
    the L1M_PCH state. This function calls the PCH state cleanup functions
    of the L1 modules and executes all the local commands pending in the
    local queue.

  l1m_pch_cmd_processor
    This function handles all external commands while in the L1M_PCH state.

===========================================================================*/
void l1m_pch_init( boolean normal , wsub_id_e_type wsub_id);
void l1m_pch_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_pch_cmd_processor(l1_ext_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTIONS L1M FACH STATE FUNCTIONS

DESCRIPTION
  l1m_fach_init
    This function does all initializations necessary for proper operation
    of the l1 in the FACH state.
    All L1 modules perform FACH state initialization procedures.

  l1m_fach_cleanup
    This function is called when L1M is transitioning to another state from
    the L1M_FACH state. This function calls the FACH state cleanup functions
    of the L1 modules and executes all the local commands pending in the
    local queue.

  l1m_fach_cmd_processor
    This function handles all external commands while in the L1M_FACH state.

===========================================================================*/
void l1m_fach_init( boolean normal , wsub_id_e_type wsub_id);
void l1m_fach_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_fach_cmd_processor(l1_ext_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTIONS L1M DCH STATE FUNCTIONS

DESCRIPTION
  l1m_dch_init
    This function does all initializations necessary for proper operation
    of the l1 in the DCH state.
    All L1 modules perform DCH state initialization procedures.

  l1m_dch_cleanup
    This function is called when L1M is transitioning to another state from
    the L1M_DCH state. This function calls the DCH state cleanup functions
    of the L1 modules and executes all the local commands pending in the
    local queue.

  l1m_dch_cmd_processor
    This function handles all external commands while in the L1M_DCH state.

===========================================================================*/
void l1m_dch_init( boolean normal , wsub_id_e_type wsub_id);
void l1m_dch_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_dch_cmd_processor(l1_ext_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTIONS L1M DEACTIVE STATE FUNCTIONS

DESCRIPTION
  l1m_deact_init

  l1m_deact_cleanup

  l1m_deact_cmd_processor

===========================================================================*/
void l1m_deact_init( boolean normal , wsub_id_e_type wsub_id);
void l1m_deact_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_deact_cmd_processor(l1_ext_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTIONS L1M PCH SLEEP STATE FUNCTIONS

DESCRIPTION
  l1m_pch_sleep_init

  l1m_pch_sleep_cleanup

  l1m_pch_sleep_cmd_processor

===========================================================================*/
void l1m_pch_sleep_init( boolean normal , wsub_id_e_type wsub_id);
void l1m_pch_sleep_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_pch_sleep_cmd_processor(l1_ext_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);

#ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
/*===========================================================================

FUNCTIONS L1M PCH BPLMN STATE FUNCTIONS

DESCRIPTION
  l1m_pch_bplmn_init

  l1m_pch_bplmn_cleanup

  l1m_pch_bplmn_cmd_processor

===========================================================================*/
void l1m_pch_bplmn_init( boolean normal, wsub_id_e_type wsub_id );
void l1m_pch_bplmn_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_pch_bplmn_cmd_processor(l1_ext_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);
#endif
/*===========================================================================

FUNCTIONS     L1M DEEP SLEEP STATE FUNCTIONS

DESCRIPTION
  l1m_deep_sleep_init
    Supports transition into L1M_DEEP_SLEEP state, caused by reception of
    CPHY_STOP_WCDMA_MODE_REQ primitive from RRC with L1_DEEP_SLEEP cause.

  l1m_deep_sleep_cleanup
    Supports transition out of L1M_DEEP_SLEEP state to either L1M_IDLE or
    L1M_DEACTIVE.  Actually does nothing, for there is nothing to do.

  l1m_deep_sleep_cmd_processor
    This function handles all external commands while in the L1M_DEEP_SLEEP state.

===========================================================================*/
void l1m_deep_sleep_init( boolean normal , wsub_id_e_type wsub_id);
void l1m_deep_sleep_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_deep_sleep_cmd_processor(l1_ext_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTIONS     L1M STOPPED STATE FUNCTIONS

DESCRIPTION
  l1m_stopped_init
    Supports transition into L1M_STOPPED state, caused by reception of
    CPHY_STOP_WCDMA_MODE_REQ primitive from RRC with L1_RAT_CHANGE cause.

  l1m_stopped_cleanup
    Supports transition out of L1M_STOPPED state to either L1M_IDLE or
    L1M_DEACTIVE.  Actually does nothing, for there is nothing to do.

  l1m_stopped_cmd_processor
    This function handles all external commands while in the L1M_STOPPED state.

===========================================================================*/
void l1m_stopped_init( boolean normal , wsub_id_e_type wsub_id);
void l1m_stopped_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_stopped_cmd_processor(l1_ext_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTIONS     L1M SUSPENDED STATE FUNCTIONS

DESCRIPTION
  l1m_suspended_init
    Supports transition into L1M_SUSPENDED state, caused by reception of
    CPHY_SUSPEND_WCDMA_MODE_REQ primitive from RRC.

  l1m_suspended_cleanup
    Supports transition out of L1M_SUSPENDED state.  There are 2 possibilities
    here.  We are leaving L1M_SUSPENDED because we got a RESUME.  Or we are
    leaving because we're getting shut down.  In the shut down scenario, call
    the cleanup routine of the state that got SUSPENDED to do final cleaning
    up.  Currently the only state requiring this is L1M_DCH.  In the RESUME case,
    don't do that because we're going to be heading back into that state and
    resuming where we left off.

  l1m_suspended_cmd_processor
    This function handles all external commands while in the L1M_SUSPENDED state.
    The only valid commands are CPHY_RESUME_WCDMA_MODE_REQ,
    CPHY_STOP_WCDMA_MODE_REQ, and CPHY_DEACTIVATE_REQ.
===========================================================================*/
void l1m_suspended_init( boolean normal , wsub_id_e_type wsub_id);
void l1m_suspended_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_suspended_cmd_processor(l1_ext_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTIONS L1M BCH SLEEP STATE FUNCTIONS

DESCRIPTION
  l1m_bch_sleep_init
    This should never get called as we directly assign l1m_state to this state.
  l1m_bch_sleep_cleanup
    This should never get called as we move directly out of this state to L1M_BCH state.
  l1m_bch_sleep_cmd_processor
    This function handles all external commands while in the L1M_BCH_SLEEP state.

===========================================================================*/
void l1m_bch_sleep_init( boolean normal , wsub_id_e_type wsub_id);
void l1m_bch_sleep_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);
void l1m_bch_sleep_cmd_processor(l1_ext_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        l1m_send_start_agc_cmd_to_mdsp

DESCRIPTION
  This function calls mdsp_sync_send_start_agc_cmd() to send the
  MDSP_CMD_START_AGC command to mDSP firmware.  It will wait up to five
  seconds for a confirmation.  It then waits 2 msec to allow the AGC
  to settle.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_send_start_agc_cmd_to_mdsp(void);

/*===========================================================================
FUNCTION        L1M_SEND_STOP_AGC_CMD_TO_MDSP

DESCRIPTION
  This function calls mdsp_sync_send_agc_cmd() to stop the AGC.
  This is done by sending the MDSP_CMD_START_AGC command with the 
  appropriate parameters to stop the AGC on the primary antenna.
  It will wait up to one second for a confirmation.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_send_stop_agc_cmd_to_mdsp( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        l1m_send_goto_sleep_cmd_to_mdsp

DESCRIPTION
  This function calls mdsp_sync_send_goto_sleep_cmd() to send the
  MDSP_CMD_GOTO_SLEEP command to mDSP firmware.  It will wait up to five
  seconds for a confirmation.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_send_goto_sleep_cmd_to_mdsp(boolean opts_enabled, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1M_CLEAR_SRCH_DONE_CMD

DESCRIPTION
  This function deletes (clears) ALL "SRCH_DONE_CMD" from the L1 local queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void l1m_clear_srch_done_cmd ( wsub_id_e_type wsub_id);
extern void l1m_clear_srch_ops_cmd( wsub_id_e_type wsub_id);
extern void l1m_clear_cmd(l1_local_cmd_enum_type cmd, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1M_POWER_DOWN

DESCRIPTION
  This function is called when L1 receives the TASK_STOP_SIG signal, so that
  the L1 task can prepare for the phone to be powered down.  There's nothing
  to do, so just dump a debug message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void l1m_power_down ( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1M_OFFLINE

DESCRIPTION
  This function is called when L1 receives the TASK_OFFLINE_SIG signal.
  There's nothing to do, so just dump a debug message.  In offline mode,
  L1 shall only handle service programming/diagnostics/test commands.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void l1m_offline ( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1_WAIT

DESCRIPTION
  This function is L1's interface to rex_wait(). It handles reporting to the
  watchdog task in case the watchdog signal arrives before the expected signal.
  
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void l1_wait (rex_sigs_type mask, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1_REPORT_WATCHDOG

DESCRIPTION
  This function reports to the watchdog task and restarts the timer for the next report.
  
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void l1_report_watchdog ( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     L1M_MDSP_INIT_DONE_ISR

DESCRIPTION
  This ISR is called after the MDSP is initialized. The MDSP_INT4 is
  muxed with the init done and rach done interrupts. This function will
  set the MDSP_INIT_DONE_SIG signal.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  MDSP_INIT_DONE_SIG signal is set and this ISR is de-registered.

===========================================================================*/
void l1m_mdsp_init_done_isr( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     L1M_WAKE_UP

DESCRIPTION
  This helper function is called from other functions to make L1 ready to process
  external commands (if any), and enable the RF TX chain.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void l1m_wake_up(boolean bplmn_progress, wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_HS_FACH_DRX 
/*===========================================================================

FUNCTION     L1M_WAKE_UP

DESCRIPTION
  This helper function is called from other functions to make L1 ready to process
  external commands (if any), and enable the RF TX chain.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void l1m_wake_up_for_edrx( wsub_id_e_type wsub_id);

#endif /* FEATURE_WCDMA_HS_FACH_DRX */

/*===========================================================================

FUNCTION     L1M_PREPARE_FOR_SLEEP

DESCRIPTION
  This function is called to indicate to L1 that a sleep cycle will shortly begin.
  It sends the go_to_sleep command to the mDSP, shuts off the mDSP clock, prepares the
  Sequencer for sleep, and changes state to PCH_SLEEP.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void l1m_prepare_for_sleep( boolean wait_for_fw_slp, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     L1M_WOKEN_UP

DESCRIPTION
  This function is called to indicate that L1 is ready for business after waking up 
  : reacquisition search has succeeded and DL channels are set up.
  It changes state back to L1M_PCH (state before sleeping), 
  sends L1 task the L1_EXT_CMD_Q_SIG signal to start processing external commands 
  (which were on hold during sleep). 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void l1m_woken_up( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     L1M_SLEEP_REACQ_FAILURE

DESCRIPTION
  This function is called to indicate to L1M that the searcher hasn't been able to 
  reacquire after waking up. Change state back to L1M_PCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void l1m_sleep_reacq_failure( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     L1M_IN_DRX

DESCRIPTION
  This function is called to indicate to L1M that L1 is in DRX or sleep mode.
  wl1m_ext_cntrl_params[wsub_id].l1m_state is changed to L1M_PCH_SLEEP

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void l1m_in_drx(boolean ss_mgr_notify, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     L1M_OUT_OF_DRX

DESCRIPTION
  This function is called to confirm that L1M is back in PCH state (from PCH_SLEEP)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void l1m_out_of_drx( wsub_id_e_type wsub_id);

#ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
/*===========================================================================

FUNCTION     L1M_IN_BPLMN

DESCRIPTION
  This function is called to indicate to L1M that L1 is in BPLMN srch mode.
  wl1m_ext_cntrl_params[wsub_id].l1m_state is changed to L1M_PCH_BPLMN.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void l1m_in_bplmn( wsub_id_e_type wsub_id);
#endif

/*===========================================================================

FUNCTION      l1m_resuming_wcdma

DESCRIPTION
  This function returns whether or not we are in the process of resuming
  WCDMA after inter-RAT handover or cell reselection failure.

DEPENDENCIES
  Whether or not we've recently received CPHY_RESUME_WCDMA_MODE_REQ
  from RRC.

RETURN VALUE
  boolean:
    TRUE  - In the process of resuming WCDMA.
    FALSE - Otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean l1m_resuming_wcdma( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      l1m_clear_resuming_wcdma_flag

DESCRIPTION
  This function resets the "l1m_resuming_wcdma_flag", called when we are
  done resuming.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_clear_resuming_wcdma_flag( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      l1m_clear_resume_state

DESCRIPTION
  This function resets the "l1m_resume_state", called when we are
  done resuming.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_clear_resume_state(wsub_id_e_type wsub_id);

#ifdef FEATURE_RRC_L1_ERROR_RECOVERY_P1
/*===========================================================================

FUNCTION      l1m_allow_sending_op_err_ind

DESCRIPTION
  This function sets the "l1m_allow_to_send_op_err_ind" flag. When set, this
  flag allows an operational error indiaction to be sent to the RRC, so that
  the latter can initiate some error recovery, e.g., through STOP/START.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_allow_sending_op_err_ind ( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      l1m_disallow_sending_op_err_ind

DESCRIPTION
  This function resets the "l1m_allow_to_send_op_err_ind" flag. When reset, the
  flag disallows operational error indiaction from being sent to the RRC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_disallow_sending_op_err_ind ( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      l1m_prepare_for_mdsp_recovery

DESCRIPTION
  This function sets the "l1m_mdsp_recovery_in_progress" flag. When this flag
  is set, STOP requests are handled by carefully avoiding all interactions with
  the mDSP.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_prepare_for_mdsp_recovery ( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      l1m_complete_mdsp_recovery

DESCRIPTION
  This function resets the "l1m_mdsp_recovery_in_progress" flag. When this flag
  is reset, STOP requests are handled as usual with all its bells and whistles.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_complete_mdsp_recovery ( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      L1M_IS_MDSP_RECOVERY_IN_PROGRESS

DESCRIPTION
  This function returns a boolean specifying whether a mDSP error
  recovery is in progress or not.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean, TRUE if mDSP error recovery is in progress, FALSE otherwise

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean l1m_is_mdsp_recovery_in_progress( wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_L1_LOG_ON_DEMAND
/*===========================================================================

FUNCTION      process_log_on_demand

DESCRIPTION
  This function routes log on demand requests to various handlers
  using the log code as the key

DEPENDENCIES
  None

RETURN VALUE
   wl1m_ext_cntrl_params[wsub_id].l1m_state

SIDE EFFECTS

===========================================================================*/
extern void process_log_on_demand(log_code_type log_code, wsub_id_e_type wsub_id);
#endif
/*===========================================================================

FUNCTION      l1m_send_op_err_ind

DESCRIPTION
    This function generates the RRC_CPHY_OPERATIONAL_ERROR_IND to RRC.

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void l1m_send_op_err_ind(l1_op_err_source_type source, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      stmr_has_passed

DESCRIPTION
    Returns true if b has passed c with a as reference

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean stmr_has_passed(uint32 a, uint32 b, uint32 c);


/*===========================================================================
FUNCTION     L1M_QUERY_INTERF_MEAS_RES_OPT_SUPPORTED

DESCRIPTION
  This function return boolean based on support of inter freq meas result optimization

DEPENDENCIES
  None

RETURN VALUE
  TRUE or FALSE

SIDE EFFECTS
  None
===========================================================================*/
extern boolean l1m_query_interf_meas_res_opt_supported( wsub_id_e_type wsub_id);


#ifdef FEATURE_FTM_WCDMA_BER
/*===========================================================================
FUNCTION     l1m_is_ue_in_ftm_mode

DESCRIPTION
  Return Whether UE is in FTM mode.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean l1m_is_ue_in_ftm_mode( wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_DUAL_SIM
/*===========================================================================
FUNCTION        WL1_PROGRAM_PLL_AGC_SETTLING_TIME

DESCRIPTION
  Make sure the PLL settling time is set properly in WFW.  There were times
  in the past that during W acq we were getting bad AGC because FW was
  receiving a bad value. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_program_pll_agc_settling_time( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_DS_REQUEST_RESOURCES

DESCRIPTION
  This function is called when WL1 is granted a  TRM lock
  This function requests hardware resources to continue with WCDMA operation.
  TCXO,RF,CLOCKS.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/
#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
extern void wl1_ds_request_resources(rfm_path_type rfm_path, wsub_id_e_type wsub_id);
#else
extern void wl1_ds_request_resources(rfm_device_enum_type rfm_device, wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION        WL1_DS_RELEASE_RESOURCES

DESCRIPTION
  This function is called when WL1 is about to release TRM lock
  This function releases hardware resources used by WCDMA L1 so that they
  may be used by another RAT.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Frees up hardware resources used by WCDMA L1.
===========================================================================*/
extern void wl1_ds_release_resources( boolean force_rf_fw_sleep, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_DS_HDLE_RESOURCES_SPLIT_ACQ

DESCRIPTION
  This function will be called during various steps in split acq. It will
  take appropriate action based on the res_rel_rsn (resource release reason)
  passed to it.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Frees up hardware resources used by WCDMA L1 excluding clocks
===========================================================================*/
extern void wl1_ds_hdle_resources_split_acq(wl1_srchacq_res_rel_rsn_type res_rel_rsn, wsub_id_e_type wsub_id);

#endif /*FEATURE_DUAL_SIM*/

#if defined(FEATURE_WCDMA_L1_ZI_OPT)

/*===========================================================================
FUNCTION  EULENC_ERAM_DUMP_GENERATE_LOCAL_CMD

DESCRIPTION
  Generates local command to log the HARQ data in Task context
  
DEPENDENCIES  None
RETURN VALUE  None
SIDE EFFECTS  None
===========================================================================*/
extern void eulenc_eram_dump_generate_local_cmd(uint8 harq_id);
#endif

/*===========================================================================
FUNCTION        WL1_CONN_STATE_CLEANUP_IN_ORDER

DESCRIPTION     This is the connected state cleanup routine which will take care
                of cleaning up the DCH state specific modules in sequence

DEPENDENCIES    The global wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask controls the modules that need cleanup.
Before wl1_conn_state_cleanup_in_order() is called for 1st time, wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask
must be set to the list of modules that need cleanup. If a module posts a cleanup done command,
the specific modules must be cleared from wl1m_ext_cntrl_params[wsub_id].l1m_module_cleanup_done_mask. Else, this function
will keep executing in loop(as the same module cleanup will be attempted in every run).

This function will get triggered from the cleanup done local command processor, which will
have the module enum as input corresponding to module whose cleanup is done.

RETURN VALUE    None

SIDE EFFECTS
==========================================================================*/
extern void wl1_conn_state_cleanup_in_order(l1_module_enum_type module, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        wl1_mdsp_rf_awake

DESCRIPTION  This API returns if FW/RF is put to sleep.

DEPENDENCIES    None

RETURN VALUE    
       None
 
SIDE EFFECTS    
       None
===========================================================================*/
extern boolean wl1_mdsp_rf_awake( wsub_id_e_type wsub_id);

#ifdef FEATURE_DUAL_SIM
extern rrc_cmd_type *cnf_rpt;
#endif
/*===========================================================================

FUNCTION     wl1_is_gl1_init_needed

DESCRIPTION
  This function return boolean based on whether GL1 initialization should happen

DEPENDENCIES
  None

RETURN VALUE
  TRUE or FALSE

SIDE EFFECTS
  None
===========================================================================*/
extern boolean wl1_is_gl1_init_needed( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WL1_FMO_SUSPEND_DONE_CB

DESCRIPTION
  This helper function is passed as callback to FMO Suspend functionality from
  Connected/Connecting cleanup routine.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void wl1_fmo_suspend_done_cb( wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION L1M_SEND_MODULE_CLEANUP_DONE_CMD

DESCRIPTION
  This function is used to post module cleanup done to l1m along with the module id that
  has been completed

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
extern void l1m_send_module_cleanup_done_cmd(l1_module_enum_type module, wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION L1M_SEND_CM_MODULE_CLEANUP_DONE

DESCRIPTION
  This function is a callback which will be called after CM is completely frozen and is used to post 
  CM module cleanup done to l1m

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
extern void l1_send_cm_module_cleanup_done(void* done, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1M_TIMEOUT_HLDR_INIT

DESCRIPTION   This function initializes Timeout handler

DEPENDENCIES    None

RETURN VALUE    
 
SIDE EFFECTS    

===========================================================================*/
 extern void wl1m_timeout_hldr_init( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1M_TIMEOUT_HLDR_REGISTER_CLIENT

DESCRIPTION   This function registers client with timeout handler.
                       Client: Client which wants to register
                       timeout_frames: Client needs to specify timeout frames. Once the timeout frames is reached,
                                                Timeout handler will either crash or call the callback if graceful_exit is set by the clent
                       graceful_exit: If client sets this flag, Clent also needs to register a callback so that when the timeout happens,
                                            the callback will be called
                       func: This is the callback function that client needs to register if graceful_exit is set.

DEPENDENCIES    None

RETURN VALUE    
 
SIDE EFFECTS    

===========================================================================*/
 extern void wl1m_timeout_hldr_register_client(
           wl1_timeout_hldr_client_enum_type client,
           uint16 timeout_frames,
           boolean graceful_exit,
           WL1_TIMEOUT_HLDR_CB_FUNC func, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1M_TIMEOUT_HLDR_DEREGISTER_CLIENT

DESCRIPTION   This function takes care of deregistering the client with Timeout handler.

DEPENDENCIES    None

RETURN VALUE    
 
SIDE EFFECTS    

===========================================================================*/
extern void wl1m_timeout_hldr_deregister_client(wl1_timeout_hldr_client_enum_type client, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1M_EXECUTE_TIMEOUT_HLDR

DESCRIPTION   This function is called from dl maint evt handler every frame. This function take a current snapshot
                       of the frc and checks if timeout value of the client registered is reached and performs the necessary
                       action.

DEPENDENCIES    None

RETURN VALUE    
 
SIDE EFFECTS    

===========================================================================*/
extern void wl1m_execute_timeout_hldr( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WL1_GET_CURRENT_FW_IMAGE

DESCRIPTION
 This function is to get the current loaded FW Image.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern wl1_fw_image_enum_type wl1_get_current_fw_image(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WL1M_FW_IMAGE_CONTROLLER

DESCRIPTION
 This function is called when L1 needs to load the different FW Images based 
 on current traffic state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern boolean wl1m_fw_image_controller(const wl1_fw_image_enum_type fw_image, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_rfm_temp_comp_part1_cb()

DESCRIPTION
This function is called when RF Temp Comp Part1 is completed. This function will further call
exec_temp_comp or abort_temp_comp based on some conditions

DEPENDENCIES
  RF Temp Comp Part1 completion will trigger this

RETURN VALUE
  None
 
SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_temp_comp_part1_cb(rfm_cb_event_enum_type abort_prep_temp_comp, void* unused);

#ifdef FEATURE_DUAL_SIM
/*=========================================================================
FUNCTION    WL1_CLEAR_F_MDSP_AND_RF_AWAKE_FLAG

DESCRIPTION
    In go to sleep fail cases we might not have go_to_sleep and ds_release_resources 
    in DSDS mode so wl1m_int_cntrl_params[wsub_id].f_mdsp_and_rf_awake flag will not be cleared and FW and RF wont be
    woken up from drx_ensure_rf_awake. This is used to clear wl1m_int_cntrl_params[wsub_id].f_mdsp_and_rf_awake flag.

PARAMETERS
 
DEPENDENCIES
  None

RETURN VALUE
  None
=========================================================================*/
extern void wl1_clear_f_mdsp_and_rf_awake_flag( wsub_id_e_type wsub_id);
extern void wl1_set_f_mdsp_and_rf_awake_flag( wsub_id_e_type wsub_id);

#endif /* DUAL_SIM */
extern void wl1sleep_drx_init_all_modules_vars(void);

#if defined (FEATURE_WCDMA_PLT) || defined (FEATURE_WCDMA_PLT_MODE) 
#error code not present
#endif

/*===========================================================================
FUNCTION  WL1_CONN_STATE_INIT

DESCRIPTION
  This function is called when entering connected state (FACH/DCH).
  It won't be called during FACH to DCH transition

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_conn_state_init(wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     WL1_INITIALIZE_MUTEXES_CONN_STATE_MUTEXES

DESCRIPTION  This function is placeholder for all global connected mode mutex initializations which are not doubled
  
DEPENDENCIES
 

PARAMETERS
 

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_initialize_conn_state_mutexes(void);

/*===========================================================================
FUNCTION  WL1M_FW_APP_SPECIFIC_INIT

DESCRIPTION
 This function is called when L1 needs to load the different FW Images based 
 on current traffic state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1m_app_specific_init(void);

/*===========================================================================
FUNCTION  WL1_CONN_STATE_DEINIT

DESCRIPTION
  This function is called when Exiting connected state (FACH/DCH).
  It won't be called during FACH to DCH transition

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_conn_state_deinit(wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_RX_TX_SPLIT
/*===========================================================================
FUNCTION  wl1_allocate_rxlm_buffers_at_wakeup

DESCRIPTION
  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_allocate_rxlm_buffers_at_wakeup(wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_RX_TX_SPLIT */

/*===========================================================================
FUNCTION  WL1_INIT

DESCRIPTION
  This function is called to initialize WL1M.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1m_init(boolean normal, wl1_mdsp_image_enum_type mdsp_image, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  wl1_allocate_rxlm_buffers_at_wakeup

DESCRIPTION
  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_allocate_rxlm_buffers_at_wakeup(wsub_id_e_type wsub_id);

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

FUNCTION L1M_PERIODIC_LOG_STATUS_SIG_HANDLER

DESCRIPTION
  This function is called periodically for every L1_PERIOIDC_LOG_STATUS_PERIOD ms
  R99 log packets status is updated into R99 log status data structure
  If EUL is active, EUL log packets status is updated into EUL log status 
  data structure.

  Timer is restarted for the next periodic update

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void l1m_periodic_log_status_sig_handler( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1M_PERIODIC_LOG_STATUS_CLEANUP_HANDLER

DESCRIPTION
  This function is called at UL Module Connected state de-init to stop the UL and
  EUL log status reporting.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void l1m_periodic_log_status_cleanup_handler( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_CFCM
/*=========================================================================
FUNCTION    WL1_CFCM_UPDATE_CLIENT

DESCRIPTION
 Update internal CFCM client info.

PARAMETERS
 Internal client index and client info.
 Pass null cb to deregister internal client.
 Current W SUB id.

DEPENDENCIES
 None.

RETURN VALUE
 None.
=========================================================================*/
extern void wl1_cfcm_update_client(wl1_cfcm_client_enum_type client, wl1_cfcm_client_info_struct_type client_info, wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1_CFCM_CB_HANDLER

DESCRIPTION
 Handle CFCM callback in task context by calling affected internal clients.

PARAMETERS
 CFCM cmd stored.
 Current W SUB id.

DEPENDENCIES
 None.

RETURN VALUE
 None.
=========================================================================*/
extern void wl1_cfcm_cb_handler(wl1_cfcm_cb_ctrl_cmd_type *cmd, wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1_CFCM_REGISTER_AND_UPDATE

DESCRIPTION
 Register and update WL1 client with CFCM.

PARAMETERS
 Current W SUB id.

DEPENDENCIES
 None.

RETURN VALUE
 None.
=========================================================================*/
void wl1_cfcm_register_and_update(wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1_IS_CFCM_INT_CLIENT_REGISTERED

DESCRIPTION
 Check if internal CFCM client is registered.

PARAMETERS
 Client idx.
 Current W SUB id.

DEPENDENCIES
 None.

RETURN VALUE
 True if client is registered.
 False if client is not registered or if invalid client is passed.
=========================================================================*/
extern boolean wl1_is_cfcm_int_client_registered(wl1_cfcm_client_enum_type client, wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1_CFCM_CB

DESCRIPTION
 CB called by CFCM when there is a monitored event.

PARAMETERS
 CFCM cmd.

DEPENDENCIES
 None.

RETURN VALUE
 None.
=========================================================================*/
extern void wl1_cfcm_cb(cfcm_cmd_type_s *cfcm_cmd);
#endif

/*=========================================================================
FUNCTION    wl1m_is_local_command_pending

DESCRIPTION
 This function checks if the requested local command is pending in local command QUEUE

PARAMETERS
 i/p: Local command id

DEPENDENCIES
 None.

RETURN VALUE
 TRUE if the local command is pending in the queue
 FALSE otherwise
=========================================================================*/
boolean wl1m_is_local_command_pending(l1_local_cmd_enum_type local_cmd_id, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     eulsg_data_rate_based_q6_control

DESCRIPTION  This function checks the Tx power, MAC buffer and Max allowed 
             ETFCI to determine whether or not Q6 needs to be bumped up 

DEPENDENCIES  NONE

RETURN VALUE  None

SIDE EFFECTS  NONE
===========================================================================*/
extern void eulsg_data_rate_based_q6_control(eul_data_rate_q6_control_action_e_type action, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     eul_send_q6_control_command

DESCRIPTION  This function performs the Q6 clock change

DEPENDENCIES  NONE

RETURN VALUE  None

SIDE EFFECTS  NONE
===========================================================================*/
extern void eul_send_q6_control_command(
eul_data_rate_q6_control_action_e_type action, 
wsub_id_e_type wsub_id);
/*=========================================================================
FUNCTION    WL1M_POST_LOCAL_CMD_FOR_SCHEDULE_UDPATE

DESCRIPTION
 This function posts a local cmd to handle BCH -> BCH_SLEEP 
 state transition and also does a schedule update fro BCCH entity. 

PARAMETERS

DEPENDENCIES
 None.

RETURN VALUE
None.
=========================================================================*/
extern void wl1m_post_local_cmd_for_schedule_update(wsub_id_e_type wsub_id);

#ifdef FEATURE_TMC_TCXOMGR
/*===========================================================================
FUNCTION     WL1_UPDATE_RGS_IN_TCXO_MGR_TIMER_EXPIRY_CB

DESCRIPTION
  This callback function will be called at the timer expiry (10 ms) to send trk_lo
  to TCXO MGR, so it can maintain a trk_lo vs. temperature table.
 
  It will in-turn post a local command, so that the whole handling happens in
  wl1 task context

DEPENDENCIES
  None.

PARAMETERS
  uint32 unused - Not used.  Included to make REX happy.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_update_rgs_in_tcxo_mgr_timer_expiry_cb(timer_cb_data_type cb_data);

/*===========================================================================
FUNCTION     WL1_UPDATE_RGS_IN_TCXO_MGR

DESCRIPTION
  This function is called very slowly (currently every 31 seconds) while
  WCDMA is the active RAT.  Its purpose is to update the trk_lo vs.
  temperature table stored by the TCXO Manager.  In states where we don't
  have service, it doesn't log trk_lo, because we're not tracking the
  downlink and don't have a frequency reference.

  In cases where we temporarily lose lock or fail reacq, the function still
  logs trk_lo.  trk_lo will still be at the last known good value when we had
  lock or successfully reacquired.  So it should still be pretty accurate.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void wl1_update_rgs_in_tcxo_mgr(wsub_id_e_type wsub_id);
#endif /* FEATURE_TMC_TCXOMGR */

#ifdef FEATURE_WCDMA_ATLAS_MODEM
/*=========================================================================
FUNCTION    WL1_SS_FREEZE_UNFREEZE_FOR_QLINK_MODE_TRANSITION

DESCRIPTION
    This function is used to do a dummy sample server freeze and unfreeze
    operation to enable FW to transition between Qlink SC modes (8SC or 16SC).
    Freeze and unfreeze ops are performed without putting RF to LPM or bringing
    it back from LPM.

PARAMETERS
    SUB_ID

DEPENDENCIES
    None.

RETURN VALUE
    None
=========================================================================*/
extern void wl1_ss_freeze_unfreeze_for_qlink_mode_transition(wsub_id_e_type wsub_id);
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/
/*===========================================================================
FUNCTION        wl1_init_task

DESCRIPTION  Initialises timers/task and queue structures   

DEPENDENCIES    None

RETURN VALUE    
       None
 
SIDE EFFECTS    
       None
===========================================================================*/
extern void wl1_init_task(void);

extern void wl1_fw_unload_rf_sleep(uint32 wsub_id);


#ifdef FEATURE_WCDMA_MDSP_RECOVERY
/*=========================================================================
FUNCTION    WL1M_GET_MDSP_RECOVERY_STATE

DESCRIPTION
 Returns current MDSP recovery status.

PARAMETERS
 Sub-id.

DEPENDENCIES
 None.

RETURN VALUE
 mdsp_recovery_status.
=========================================================================*/
extern mdsp_recovery_status_enum_type wl1m_get_mdsp_recovery_state(wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1M_SET_MDSP_RECOVERY_STATE

DESCRIPTION
 Sets MDSP recovery status.

PARAMETERS
 New state.
 Sub-id.

DEPENDENCIES
  None.

RETURN VALUE
  None.
=========================================================================*/
extern void wl1m_set_mdsp_recovery_state(mdsp_recovery_status_enum_type set_state, wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1M_MDSP_RECOVERY_INTERRUPT_HANDLER

DESCRIPTION
 Interrupt handler for MDSP recovery trigger.

PARAMETERS
  Sub-id.

DEPENDENCIES
 None.

RETURN VALUE
 None.
=========================================================================*/
extern void wl1m_mdsp_recovery_interrupt_handler(wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1M_HANDLE_MDSP_RECOVERY

DESCRIPTION
 Start MDSP recovery process. Sends command to RRC depending on current 
 state.

PARAMETERS
 Sub-id.

DEPENDENCIES
 None.

RETURN VALUE
 None.
=========================================================================*/
extern void wl1m_handle_mdsp_recovery(wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1M_RECOVER_FW

DESCRIPTION
 Handle WFW recovery. Calls for WFW stop, FW APPs suspend, FW APPs resume, 
 and WFW start.

PARAMETERS
 Sub-id.

DEPENDENCIES
 None.

RETURN VALUE
 None.
=========================================================================*/
extern void wl1m_recover_fw(wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1M_HANDLE_EXT_CMD_FOR_RECOVERY

DESCRIPTION
 Keeps track of IDLE REQs.

PARAMETERS
 External command pointer.
 Sub-id.

DEPENDENCIES
 None.

RETURN VALUE
 None.
=========================================================================*/
extern void wl1m_handle_ext_cmd_for_recovery(l1_ext_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1M_MDSP_RECOVERY_NV_READ

DESCRIPTION
 Read MDSP Recovery NV.

PARAMETERS
 Sub-id. None.

DEPENDENCIES
 None.

RETURN VALUE
 None.
=========================================================================*/
void wl1m_mdsp_recovery_nv_read(wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1M_MDSP_RECOVERY_INIT

DESCRIPTION
 Initialise MDSP recovery stuff.

PARAMETERS
 Sub-id.

DEPENDENCIES
 None.

RETURN VALUE
 None.
=========================================================================*/
void wl1m_mdsp_recovery_init(wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1_IS_MDSP_RECOVERY_EST_IND_SENT

DESCRIPTION
 Check if est ind is sent as part of recovery.

PARAMETERS
 Sub-id.

DEPENDENCIES
 None.

RETURN VALUE
 None.
=========================================================================*/
extern boolean wl1_is_mdsp_recovery_est_ind_sent(wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_MDSP_RECOVERY */

/*=========================================================================
FUNCTION    WL1_IS_L1_ACTIVE

DESCRIPTION
 Get whether L1 stack is active.

PARAMETERS
 None.

DEPENDENCIES
 None.

RETURN VALUE
 TRUE  - L1 is not in deep sleep, stopped or deact states.
 FALSE - L1 is in any other state.
=========================================================================*/
extern boolean wl1_is_l1_active(wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1M_LTA_TL_PROFILING_INIT

DESCRIPTION
This function would be an one time init at the very first time W stack comes-up.
It sets the variables used for profiling LTA timeline to initial values

PARAMETERS
 None.

DEPENDENCIES
 None.

RETURN VALUE
 None
=========================================================================*/
extern void wl1m_lta_tl_profiling_init(void);

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

/*===========================================================================

FUNCTION    L1M_DISCARD_SPECIFIC_LOCAL_CMD

DESCRIPTION
   This function deletes specific command from the local command queue

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1m_discard_specific_local_cmd( l1_local_cmd_enum_type cmd, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION perf_busyWait_sleep_timer

DESCRIPTION
  This function will sleep for 100x uS < pescribed amount of time and do busylooping for remaining time. 

DEPENDENCIES
  NONE

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/


void perf_busyWait_sleep_timer(uint32 pause_time_us); 

#endif /* L1M_H */
