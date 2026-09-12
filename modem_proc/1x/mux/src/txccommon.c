/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           C D M A    T R A N S M I T   C O M M O N    M O D U L E

GENERAL DESCRIPTION
  Responsible for transmit command processing and utility routines

EXTERNALIZED FUNCTIONS
  txc_cmd - transmit task
  txc_cdma_enc_setup - sets up encoder for CDMA
  txc_cdma_enc_shutdown - shuts down encoder, turns off clock
  txc_data_pump_init - initialize data pump
  txc_flag_erasure - sets erasure flag
  txc_flag_next_init
  txc_get_fch_rate_set - Returns the rate set for FCH
  txc_get_sch_info - returns current configuration information for the SCHs
  txc_get_time_in_1_25ms_unit -  get the current time in 1.25 ms unit and
  factor in the frame offset as well
  txc_int_occured
  txc_powerup_init - Initializes timers and queues for CDMA Transmit task
  txc_register_action_time - enable MCC_TXC_FRAME_SIGs for MC's HO action
  time and for RF tune time
  txc_rpt_voc_rate - Reports the vocoder rate
  txc_set_fpc_mode - ets the value of FPC_MODE in variable txc_so
  txc_set_frame_offset
  txc_set_pch_fch_gain - Sets the gain values in txc_pch_fch_gain_tab
  txc_subtask - Transmit subtask to process CDMA
  txc_suspend_tx - enables/disables the Transmitter and enables/disables the
  transmission of Access probes
  txc_tx_shutdown - Turns off tx clock and disables TXC interrupt.  Allows MC
  to command SRCH to sleep.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

  Copyright (c) 1991 - 2020 Qualcomm Technologies, Inc.
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

$Header: //components/rel/1x.mpss/8.0/mux/src/txccommon.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
08/31/17   ppr     Sub specific 1x EFS Item support chgs
05/23/16   nsh     optimized f3 logs.
05/18/16   eye     Adding back SmartWait for 1x2GTA LTA case.
05/16/16   eye     Mainline FEATURE_MODEM_1X_IRAT_LTO1X for L compileout build.
04/28/16   eye     Decrement anyone TX frame count and report to DS.
04/21/16   srk     1x2GTA cleanup and TX power shoot fixes.
04/15/16   eye     Guarding SmartWait under TA flag to avoid LPM crash.
04/14/16   srk     Optimized TX Trigger fixes and code cleanup.
03/28/16   ab      Reset the TX stats for every enable/disable request
02/05/16   eye     1x2GTA Feature changes
01/20/16   eye     Removed TX ON check to make PA OFF when exiting TX.
12/24/15   eye     RUMI : compiler warning/error when RF undefined.
11/03/15   agh     Fixed call drop and TX disable issues for inter band CFS
09/11/15   jh      Set txc_tx.tx_on to false when disabling TX
08/19/15   srk     Remove band supported checks for ASDIV.
07/31/15   agh     WTR Hopping changes for inter band CFS and RIF HHO
07/10/15   eye     WTR Hop based HHO checks for Tx Adj retention.
05/15/15   sst     Support for Modem Power Statistics
05/11/15   jh      RXTX split - FR 21976
04/21/15   srk     Added support for band specific ASDIV enable/disable for
                   all 1x states
01/06/15   srk     Wait for TX start/stop response before sending ASDIV command
11/23/14   eye     Added support for 0x1877 Power Reporting Log Packet.
11/07/14   srk     Removed the obsolete Hyperspace abort function.
10/27/14   srk     Fix the BTF logic on UTX targets.
10/08/14   srk     FEATURE_MODEM_1X_UTX related cleanup.
09/30/14   srk     Enable/Disable TX with RF before updating TRM for ASDIV.
09/02/14   ssh     Update RF driver about the rev link config updates.
08/20/14   jh      Fixed compiler warnings for hexagon 6.4.02
07/29/14   ssh     Set CH1 params before enabling the DCCH.
07/25/14   ssh     Corrected TX shutdown sequence.
05/19/14   ab      Compiler Error Fix
06/12/14   vks     Add api to register TX (access/traffic) pcg events
06/02/14   pap     Adding diag command to refresh NV items.
05/29/14   pap     Redistributing messages.
05/19/14   bph     Make callbacks registered with TRM asynchronous
04/24/14   cjb     BOLT revisit cleanup
04/17/14   cjb     Dynamically handle RF_DEV assignment from TRM for TX case
02/27/14   bb      Changes to remove high clock vote while shuting down TX
02/25/14   pk      Only call RF Tx query APIs when Tx is on.
02/18/14   cjb     Changes to support SHDR on BOLT modem with WTR3925
01/08/14   ab      DDR power management Support / Feature Cleanup
01/04/14   eye     Set PCG mask to 0xFFFF during access enrty & also during
                   teardown,make it independent of RC3 1/8th gating and DTX.
02/03/14   dkb     Feature guard Triton UTX code under FEATURE_MODEM_1X_UTX
01/20/14   srk     ASDIV Cleanup - Remove signal and wait for ASDIV in TC exit
12/07/13   cjb     Changes to crash if RF call to sleep/wakeup TX fails
12/03/12   dkb     Remove double Tx trigger in the case of a jump abort.
11/25/13   cjb     Switched logic for FEATURE_MODEM_1X_BOLT_OBSOLETE feature.
                   Mainlined FEATURE_MODEM_1X_TRITON.
11/18/13   pap     To provide the cost of modem power for TX.
11/14/13   dkb     Process muxmsgr messages in all Tx states.
11/08/13   dkb     Featurize out call to RF API which is causing crash when
                   RF sends an unsupported message to FW.
11/07/13   srk     Do ASDIV band compatibility check for HHO.
11/05/13   cjb     Assert with the failure reason if the rf_wakeup_tx call fails
11/04/13   dkb     Clean up 1x RL and remove unsupported FW messages.
10/15/13   cjb     Removed unused IRQs and related cleanup
10/09/13   dkb     Add additional RL changes.
09/30/13   dkb     Initial RL changes for new FW interface.
09/10/13   dkb     Register Tx interrupts using m1x vstmr API
09/06/13   srk     Create new APIs to return Filtered TX power and Max TX power
08/28/13   cjb     Removed STX manager support
08/21/13   srk     Set dtx mask to 0xFFFF on TX shutdown.
07/23/13   srk     Replace RF API calls with mux API.
07/23/13   srk     Use TCB_PTR MACRO instead of rex_self() or tcb_ptr to make
                   the code compatible with RCINIT.
06/26/13   eye     Removed support for Device only DTX for RC4.
06/13/13   bb      Changes to call cmd_done after command is being
                   processed during TXC INACTIVE state
06/04/13   ab      Cleanup coex manager changes
05/14/13   eye     Unifying Device only DTX for RC3/4 and RC8.
05/07/13   eye     Backing out SAR updates done for COEX.
05/15/13   ab      1X coex manager changes
05/06/13   cjb     Resolve compiler warnings
04/25/13   eye     Updating SAAR changes which is to be replaced by COEX.
04/19/13   trc     Set ENC_SLEEP_MODE prior to disabling Tx @ RF
03/27/13   vks     Send RX power along with TX power for STX
03/05/13   srk     Replace existing watchdog implementation with the new
                   Dog Heartbeat Mechanism.
02/15/13   srk     Replace mcs hwio with m1x hwio
01/21/13   trc     Compiler warnings fix around has RF feature
01/18/13   srk     Update MCS Subscription Priority Module with the current
                   1x state.
12/19/12   vks     Add support for stx manager
12/17/12   vks     No need to wait for asdiv signal for tx start.
12/10/12   trc     Layering/Cleanup for Triton
11/30/12   vks     Add support for 1x asdiv idle mode operation.
11/19/12   vks     Moved indicating start and stop of TX mode for AsDIV
                   algorithm into txccommon
10/30/12   srk     KW warning fix.
10/25/12   trc     Trigger TxDAC immediately under all hyperspace abort cases
09/26/12   jtm     DIME_REVISIT clean up.
09/14/12   jtm     Feature cleanup.
08/21/12   srk     Create an api to return if TX ON/OFF before doing CFS.
08/20/12   srk     Ensure TX was ON prior to TX shutdown process.
08/02/12   srk     Added support for Device only DTX for 1x Advanced calls.
07/11/12   srk     Changed the returned values from RF to match legacy units
                   for proper tx power logging.
05/16/12   srk     Workaround for missed hardware assertion of the summary line
05/15/12   srk     Call TX wrapper function to get TX power instead of directly
                   calling RF API.
05/08/12   jtm     Treat a TT_F like a JUMP_F.
04/27/12   srk     Added an API to return PA state if TX is ON.
04/04/12   srk     Send channel configuration updates to RF only when there is
                   a change in channel configuration.
04/04/12   trc     Convert to m1x_time module
03/28/12   srk     Mainlined FEATURE_DS
02/24/12   srk     Mainlined FEATURE_IS2000_SCH and FEATURE_IS2000_P2
02/22/12   jtm     Change Tx clock voting to use additional states.
02/17/12   srk     Replaced the tramp api with DAL Interrupt Controller.
02/16/12   srk     Replaced FEATURE_LTE_TO_1X by FEATURE_MODEM_1X_IRAT_LTO1X.
02/13/12   srk     Moved init fucntions outside txc_powerup_init.
02/01/12   mca     Added txc_tx_is_on()
01/31/12   srk     Feature Clean Up.
12/20/11   srk     Feature clean up: Mainlined FEATURE_MODEM_1X_SUPPORTS_Q6
                   and FEATURE_MODEM_1X_SUPPORTS_RX_TX_LM
12/13/11   srk     Feature clean up: Mainlined FEATURE_RL_JUMP_ENHANCEMENT
                   and FEATURE_1X_WDOG_DYNAMIC
12/02/11   jtm     Replaced TXC_INT_CMD with txc_isr_cmd to handle command
                   exchanges between TXC task and the encoder ISR
12/01/11   srk     Move txc_tx_shutdown from txcaccess.c
11/30/11   srk     Feature clean up: Mainlined and Internalized Features
11/29/11   trc     Freeze closed loop power control when stopping Tx
09/30/11   trc     Turn on/off Tx in RF start/stop for proper HHO behavior
09/28/11   srk     Mainlined Always On features and removed Always Off Features
09/02/11   sk      Rebased the code to the NikeL modem package by mainlining
                   FEATURE_MODEM_1X_NIKEL.
08/18/11   adw     Mainline 1x message router support.
08/17/11   trc     Mainline RF SVDO API, add 1xL1<-RF BTF query support
08/05/11   vks     Fix compiler issues.
08/01/11   trc     Skip sleep/wakeup tx calls for nikel.
07/26/11   cjb     Move txc_crit_sect init/deinit to txc_task().
07/20/11   cjb     Delete txc_crit_sect while exiting txc_subtask().
07/01/11   jtm     Make txc_get_tx_device non-SVD_API friendly.
06/29/11   adw     Track RF Tx config state and prevent redundant shutdown.
06/28/11   jtm     Changes to get 1xA working on nikel.
06/23/11   vks     During shutdown, enable clocks only if required.
06/15/11   adw     Force Tx get devices to return valid config.
06/13/11   adw     Send Tx DAC stop message to FW.
06/03/11   adw     SV support.
05/27/11   trc     NikeL Tx support
05/03/11   jtm     Allow dog report struct to be accessed externally.
04/26/11   jtm     Gracefully handle a CDMA_F command when already in CDMA state.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
04/20/11   jtm     Replace clk_busy_wait() with DALSYS_BusyWait()
04/05/11   vks     Bring back checks to prevent double pumping of txc rf calls
                   for RF SVDO api.
03/28/11   jtm     Removed deprecated RF includes.
03/23/11   vks     Add support for RX/TX Link Manager feature for Nikel.
03/03/11   adw     Removed enc and txcmod initialization from task init.
02/24/11   jtm     Changed error message when receiving invalid commands
                   in inactive state to be less critical.
02/14/11   jtm     Removed extra 25ms timed wait from txc_cdma_enc_shutdown.
02/04/11   vks     Use TXC_* for txc_rfnv_backoff_chan_config_type for enum
                   declaration.
12/01/10   bb      Add support for LTE to 1X feature
01/28/11   vks     Featurize RF interface with FEATURE_MODEM_1X_SUPPORTS_RF.
01/24/11   vks     Update to new RF api for RF SVDO feature development.
01/21/11   vks     Use 1X specific RF SVDO feature - FEATURE_1X_RF_SVDO_API.
01/21/11   adw     Added initial M1X_RM support.
12/17/10   jtm     Feature cleanup.
12/17/10   bb/jtm  Allow txc_subtask to gracefully handle invalid commands
                   during inactive state
12/15/10   vks     Temporarily featurize register accesses for Nikel.
12/09/10   jtm     Mainlined FEATURE_SRCH_CLKREGIM_RM and cleaned up unneeded
                   legacy clock code.
11/01/10   jtm     Private file clean up.
10/30/10   jtm     Modem 1x SU API changes.
10/30/10   jtm     Integrate from rel/1h10.
10/28/10   jtm     Removed CLK_MS_PER_TICK
10/07/10   jtm     Replaced clk_regime*() calls to enable/disable TX with
                   enc_update_tx_clk_resources().
09/24/10   bb      Changes to remove redundant txc_tx_shutdown() call while
                   processing TXC_EXIT_F command
09/07/10   trc     Always delay by a frame+ at the end of access or traffic
                   in txc_cdma_enc_shutdown().  Do not minimize tx_adjust.
                   Ensure TX_ON goes low 20us prior to RF teardown.
08/06/10   jtm     Mainlining TXC-related clocks.
07/23/10   jtm     Added support for 1xA FFPC modes.
06/10/10   jtm     Eliminate dependency on log_dmss.h. Moved RPCTL related
                   functions to txctraffic.c.
05/10/10   jtm     Lint fixes.
04/12/10   jtm     Added RC8 MPP support.
05/13/10   trc     Add TX Conditioning clock support for Genesis 1.1
04/28/10   jtm     Removed check in txc_cmd() for qcnt > 0 to prevent false
                   err_fatals from occuring since it is expected to have multiple
                   *unique* items in the queue.
                   Disabled checking for redundant start/stop/enable TX calls.
04/20/10   jtm     Replace rfm_tx_*() api calls and common code associated with
                   starting/stopping/enabling/disabling TX with a wrapper
                   function.
04/12/10   jtm     Disable preemption in txc_cmd() to prevent a race condition.
03/30/10   jtm     Replaced FEATURE_QDSP6 with FEATURE_MODEM_1X_SUPPORTS_Q6.
02/04/10   jtm     Removed FEATURE_PLT.
02/01/10   jtm     Removed feature TMOBILE.
01/10/10   vks     Use FEATURE_QDSP6 instead of T_QDSP6.
12/07/09   vks     Initialize msgr before any other initializations in
                   txc_powerup_init.
11/30/09   vks     Group together mdsp writes for 1xADV channel gain ratios.
11/12/09   vks     Add MSGR/Q6 framework support.
11/11/09   vks     Temporarily bring back txc_sch_gain for backwards
                   compatability.
11/07/09   vks     Removed txc_<channel>_gain variables. Use channel_gains
                   (in muxmdsp.h) instead.
11/05/09   jtm     Added functions to return information about various RL
                   channels.
11/04/09   adw     Initialize the encoder as part of txc powerup init.
10/27/09   jtm     Print invalid RC in txc_get_rev_link_rate_set() instead of
                   rate set.
09/23/09   jtm     Changed macro ENC_RC_TO_RATE_SET to become a function.
09/22/09   jtm     Corrected compiler warnings.
08/21/09   jtm     Lint Fixes.
08/18/09   jtm     Corrected 1xA featurization error in txc_subtask.
08/11/09   jtm     Replaced deprecated ts.h api's with new time api's.
08/05/09   jtm     Lint fixes.
08/04/09   jtm     Replaced calls to deprecated BIO funcs with new DAL API.
07/16/09   mca     Added a task-lock check in txc_wait
07/15/09   jtm     Removed include for rf1x_core_rf.h.
07/06/09   jtm     Update to use cai_rcp_rxc_txc_type.
06/12/09   jtm     Added debug message to trap invalid rate and/or cell size
                   passed in to txc_set_pch_fch_sch_ratios()
06/11/09   jtm     Replaced FEATURE_1X_Q6_CRIT_SECT with T_QDSP6.
05/20/09   jtm     Added include for tx_v.h for CMI clean up.
04/17/09   jtm     Added includes to fix compiler warnings.
04/13/09   adw     Updated rf includes to support rf mode refactoring.
04/08/09   jtm     Initial 1X Advanced feature support.
04/06/09   mca     Added RF SVDO API support
04/06/09   jtm     Moved FEATURE_IS2000_P2 and FEATURE_DS_IS2000 inside
                   txc_get_dcch_rate_set(), txc_dcch_is_on(), and txc_get_sch_info
                   since these functions are exposed in the au/api and must be defined.
04/02/09   jtm     Removed T_IO_CARD.
04/01/09   jtm     Featurized necessary includes.
03/31/09   jtm     Eliminated implicit includes.
03/20/09   jtm     Cleaned up header file inclusions.
03/17/09   jtm     Replaced INTLOCK/INTFREE, INTLOCK_SAV/INTFREE_SAV, and
                   MSM_LOCK/MSM_UNLOCK with macros that can be mapped correctly
                   based on the compiler and controlled internally to TXC.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
07/21/08   jtm     Removed FEATURE_IS2000_REL_C_DV and
                   FEATURE_DELAYED_TX_CLOCK_SHUTDOWN code. Removed txc_pwr_boost
                   and txc_pwr_boost_cnt since they are unused.
06/24/08   vlc     Merged clock regime changes.
05/07/08   trc     Move initial txc_cdma_enc_setup() + clock mgmt to txc_cdma()
04/23/08   vlc     Merged the following modem clock halt code from orphan.
                   10/25/07 cr SC2X modem clock halt insure that txc doesn't
                   access un-clocked HW. FEATURE_MODEM_CLOCK_HALT.
04/03/08   trc     Guarantee Tx clocks are on in txc_cdma_enc_shutdown()
04/01/08   vlc     Added check for illegal consecutive calls to txc_cmd().
                   Removed clock resource manager code.
03/31/08   mca     Changed FEATURE_CLKREGIM_RM to FEATURE_SRCH_CLKREGIM_RM
03/05/08   vlc     User clock resource manager to manage TX clocks
                   (FEATURE_CLKREGIM_RM.)
04/03/07   vlc     Removed txc_flush_enc() call because it clobbered HDR
                   encoded data in SHDR Rel A mode.  No PA issue has been
                   reported with this code base.
04/02/07   vlc     Added new PCG define for bin0 ENC ISRs specifically for loop
                   back calls.  Moved default PCG to 1 for bin0 ENC ISRs for
                   all other calls.
10/30/06   bn      Added code to support DO/UMTS to 1x handoff
09/05/06   vlc     Implemented TX gain enhancements to increase digital gain
                   resolution.
08/03/06   vlc     Added call to txc_flush_enc() in function txc_subtask().
06/09/06   trc     Remove T_MSM6700 featurization
05/19/06   vlc     In function txc_cdma_enc_shutdown(), check if 1X is
                   operating on the primary transceiver before disabling the
                   transmit chain.
                   Change was merged from MSM6550 code base.
03/20/06   vlc     In traffic state, wait for 1 extra frame before turning off
                   PA so the last encoded frame (if any) could be transmitted.
10/17/05   rkc     Remove wrapper function txc_enc_set_frame_off().
10/12/05   rkc     Add wrapper function txc_enc_set_frame_off().
09/08/05   ejv     Use new srch_mux interface calls.
07/26/05   vlc     Applied code review comments for RL jump to hyperspace
                   enhancement.
07/25/05   rkc     Fixed lint errors.
05/23/05   vlc     Changed FEATURE_IS2000_REL_D_JUMP to FEATURE_RL_JUMP_ENHANCEMENT.
                   Added F3 debug message to function txc_cdma_enc_shutdown().
                   In function txc_subtask(), added support for Fast Call Set Up
                   - New Jump to Hyperspace command feature.
04/27/05   vlc     Main lined code in current targets, cleaned out obsolete targets,
                   cleaned out obsolete features.
03/14/05   vlc     Fixed compiler warnings for MSM6700.
03/14/05   vlc     Merged in changes for MSM7500.  Changed T_QUASAR_BB to T_IO_CARD.
                   (dlb')
03/07/05   vlc     Added support for Fast Call Set Up - New Jump to
                   Hyperspace command feature.
                   Removed check for OK to sleep in txc_exit(), now always
                   turn off ENC clocks in txc_exit().
01/26/05   vlc     Removed obsolete ds.h inclusion.
09/28/04   sr      Merged in changes from 6550 2009 build
09/16/04   jrp     Fixed passThruEnable in support of DO.
09/01/04   jrp     Fixed featurization.
07/28/04   jrp     Renamed rpc_get_sch2pich_gain_ratio.
07/15/04   sr/ll   Featurized out rf driver calls under T_QUASAR_BB
07/15/04   jrp     Added call to muxmdsp_wr_passThruEnable in txc_subtask.
06/29/04   jrp     Added call to txcmod_int_init().
06/16/04   sr      Merged in changes from 6500 5004 build
06/14/04   sr/va   Added HHO cleanup under FEATURE_HHO_ENHANCEMENTS
06/09/04   jyw     modified the txc_subtask() to allow TXC_EXIT_F as the first
                   command issued to TXC from MC; This modification corresponds the
                   new SEARCH_INACTIVE_STATE.
05/21/04   jyw     Intorduce the new RF PA backoff interface
05/18/04   jrp     Reset SCH RC to 3 at txc_exit() for subsequent access.
05/07/04   vlc     Modified RLP logging extensively to decouple MUX processing
                   and RLP processing, moved RLP processing to RLP module.
04/26/04   vlc     Implemented new way of handling RXTX confirmation of all
                   signalling messages for voice calls/data calls/svd calls.
04/26/04   bn      Added a function to turn off the TX code in encoder shutdown function
04/20/04   jrp     Fixed lint warnings.
03/29/04   sr      Removed SAM check before turning off PA in
                   txc_cdma_enc_shutdown()
03/10/04   vlc     Added support for centralized transaction engine handling
                   under feature FEATURE_TRANS_MANAGER.
03/09/04   sr/bkm  Featurized DV code
12/16/03   aaj     Do not disable Tx if 1x does not own RF
10/30/03   jrp     Fixed compiler warnings.
09/15/03   jrp     Added check for FEATURE_DELAYED_TX_CLOCK_SHUTDOWN when
                   leaving CDMA mode.
09/09/03   dna     Merged porting changes for MSM6500
04/15/03   jrp     Support for FEATURE_DELAYED_TX_CLOCK_SHUTDOWN.
10/15/02   dlb     Added new power control logging to combine power control
                   and frame type logging.
09/24/02   jrp     Handled FPC_MODE_FFPC_DISABLED case in txc_set_fpc_mode().
08/30/02   sr      Moved the clearing of TX_ISR up to top of
                   txc_cdma_enc_shutdown() before resetting the RC to 1
08/08/02   vlc     Fixed compiler error when FEATURE_IS95B_MAHHO is turned off.
08/01/02   jrp     Removed txc_flag_eib(), txc_flag_qib(), and txc_flag_sch_eib().
                   These are now modified directly by rxctraffic.  Added featurization
                   for FPC modes 4,5,6.  Changed FPC_MODE enums to be more descriptive
                   due to SVD.
07/17/02   hrk     Support for RPC logging for simulataneous R-FCH and R-DCCH.
06/18/02   jrp     Added support for FFPC Modes 4,5,6.
05/29/02   jrp     Changed txc_suspend_tx() to call txc_acc_probe_resume() to
                   resume probing only if probing was previously suspended.
05/22/02   bn      Added new logics to support VP2 RPC
04/08/02   tc      Supported REACH 20ms frame, 38400bps pad size.
02/25/02   bn      Added support for  MUX SVD signaling
04/02/02   sr      Changed 32X gains to 8 bit values for P2 also
01/15/02   hrk     Changed RF_ENABLE_PWR_WAIT to RF_ENABLE_TX_PWR_WAIT when using
                   FEATURE_RF_WAIT_CHANGES.
12/10/01   hrk     Renamed txc_erasure_flag to txc_eib_flag. Renamed function
                   txc_flag_erasure() to txc_flag_eib().
11/09/01   hrk     Setting MOD_PCBIT_TEST_MASK=0x0000 for FPC_MODE != 3.
11/08/01   hrk     Setting MOD_PCBIT_TEST_MASK for FPC_MODE=3 for MSM5100.
10/30/01   bn      Added support for Release A RDCH logging
08/13/01   bn,hrk  Fixed :
                   1. incorrect turn around constant during R-SCH transition in RPC logs.
                   2. multiple channel adjustment gain for FCH was delayed by 1 frame in
                      RPC logs.
06/15/01   snn     Added two new functions for DCCH UI display.
06/15/01   day     Mainlined FEATURE_RLGC, FEATURE_FFPC, FEATURE_IS95B, FEATURE_IS2000
                   Delete FEATURE_SPECIAL_MDR, FEATURE_FINGER_POLL
06/04/01   vlc     Changed FEATURE_GPSONE to FEATURE_GPS_MODE.
06/01/01   snn     Added new parameter to txc_get_sch_info to return sch rc.
05/15/01   sr      Changed 32X gains to 8 bit values
05/11/01   vlc     Merged in datapump changes for PLT (ks.)
05/03/01   sr      Supported reverse link DCCH PLT.
04/20/01   sr      Merged in from MSM5100 archive
04/18/01   sr      Merged with MSM_MUX1X.01.00.00
           hrk     Default gain table, txc_pch_fch_sch_gain_tab, used by PLT/FTM
                   is based on Nominal Attribute gains specified in IS2000.
           lcc,hrk Added support for FEATURE_IS2000_P2
                   Moved txc_scr_action() and txc_scr_trans_cmd to another file.
           hrk     updated txtc.offset_pwr in Handoff scenario.
03/27/01   vlc     Merged MSM_MUX1X.00.00.17.
           hrk     updated txtc.offset_pwr in Handoff scenario.
03/23/01   sr      Added entry in gain table for 32X
03/01/01   tc      Merged MSM_MUX1X.00.00.15.
02/22/01   ak      Took rlp logging functions out of IRAM.
02/16/01   bgc     Changed FEATURE_FACTORY_TESTMODE to enable a runtime switch.
02/16/01   hrk     Flushing RPC log at the end of call.
01/31/01   tc      Merged GPS support.
01/12/01   tc      Merged MSM_MUX1X.00.00.06.
11/13/00   tc      Defined txc_pch_gain, txc_fch_gain, txc_sch_gain.
11/09/00   tc      Defined feature T_MSM_5100.
11/08/00   tc      Supported MSM5100 modulator driver interface.
01/05/01   lcc     Merged back code inside PLT and T_MOBILE that was accidentally
                   removed.
12/27/00   lcc     In txc_gating_trans_cmd, explicitly turns off rev fch 1/8th
                   rate gating when a "disable 1/8 rate gating" command is received.
11/12/00   sr      (merge) Added support for Full TDSO
11/10/00   jc      repartioned txc.c into several files from Build TR2064
10/19/00   bgc     Fixed warnings for factory test mode.
10/05/00   lcc     Removed call to mctrans_confirm_transaction during transaction
                   processing.  This function is local to the transaction engine.
10/04/00   jq      Added FTM FCH and SCH loopback feature
10/04/00   sr      Took care of programming the modulator every frame with
                   the proper gating mode and pattern information while in
                   FCH 8th rate gating mode.
09/25/00   ak      Removed call to dsrlp_init.  Redundant, since called by
                   call processing.
09/20/00   hrk     Implemented maximum power protection scheme.
09/19/00   hrk     Supported changes to Nominal Gain values for 1/8 R-FCH gating.
09/19/00   sr      Fixed problem with setting SCH gain to 0 if
                   txc_rev_fr_block.num_frames !> 1. This condition is
                   only true for 3G packet data calls.
09/15/00   ak      Some SCH Mux stats fixes.
09/14/00   sr      Supported single-sized PDUs in STDSO
09/12/00   ak      Fixed up parentheses for so 33 data Tx.
09/12/00   sr      Changed ts_get_20ms_frame_time() to
                   ts_get_20ms_frame_offset_time() for action timing
09/06/00   bgc     Added FEATURE_FACTORY_TESTMODE.
09/05/00   ttl     Added DTX duration counting.
                   Enabled the data TX on SCH when the FCH is muted for data.
09/05/00   sr      Removed unused declaration of qword current_frame_time
08/25/00   rc      Added changes to support NULL returns from
                   dsm_new_buffer().
08/24/00   hrk     Removed redundant action time code.
08/21/00   hrk     Using the rate of the frame being currently transmitted to
                   compute channel power. Earlier the rate of the frame being
                   built (i.e. next frame) was used - resulting in incorrect
                   values for R-PICH, R-FCH ... during rate transitions.
08/14/00   sr      Added support for pilot gating and FCH 1/8 rate gating in
                   PLT
08/02/00   sr      Fixed deprecated declaration of txc_cdma_enc_setup()
08/02/00   sr      Fixed typo in feature name
07/31/00   lad     Added R-SCH0 to logging, fixed sub-record count bug.
07/31/00   sr      Fixed STDSO bug re Mux PDU header not being inserted
07/31/00   sr      Marked several functions with #ifndef MSM5000_IRAM_REV
07/25/00   sr      Added FEATURE_DATA_TIMELINE_ADVANCE to move to encoder
                   interrupt to the 5ms mark for 3G packet data calls
07/24/00   hrk     Collecting AGC values for Reverse Power Control logging
                   in rxc_tc_isr().
07/24/00   ak      Added R-SCH logging.
07/20/00   ak      Removed old mux sch stats struct (replaced by parm db).
                   Also removed FEATURE_IS2000_SCH_STATS.  Fixes for mux
                   stats with SO 33 and TDSO.
07/18/00   sr      Fixed improper TDSO featurization
                   Changed tc to ret while checking for fch_gating in
                   TXC_RETURNED_F
07/17/00   sr      Added #ifndef FEATURE_PLT around FCH gating code
07/17/00   sr      Added FCH 1/8 rate gating support
07/14/00   sr      Added FCH 1/8 rate gating support
                   Added support for hard handoff preamble in PCG units
                   Turned off sio interrupts (under FEATURE_SIO_INT_LOCKOUT)
                   around txc_tx_frame_proc()
07/12/00   sr      Fixed tdso 1x SCH
07/12/00   sr      Changed argument type for txc_tdso_traffic()
07/10/00   sr      Fixed incomplete featurization of TDSO
07/10/00   sr      Added STDSO support
07/10/00   sr      Swapped arguments to txc_rlp_fch_frame_len_tbl
07/10/00   sr      Changed txc_rev_sch_trans_cmd() to set ltu_size to 0 for
                   turbo codes
                   Added boolean argument to enc_sch_data_out_cfg()
06/21/00   lcc     Increased number of action items to 10.  Change message level
                   when out of action items to error.
06/15/00   lcc     Changes to handle new HO action interface:
                   1. num_preamble now comes with unit (frames or PCGs) as well
                   2. Inclusion of nom_pwr is now optional.
                   3. Corrected a problem with quadword comparison.
06/15/00   ak      Fixes for 1x R-SCH:  zero LTUs with 1x or 2x.  Also make
                   sure to pre-pend the MM bit on 1x frames before sending
                   to sch encoder.
06/13/00   ak      Added SCH0 mux stats for parm database.
06/13/00   sr      Enable FCH after preambles are done, because in HHO case
                   txc does not get any TXC_NULLS_F to tell it that we are
                   done with preambles
06/09/00   hrk     Added support for new MC-RLGC interface.
06/02/00   ak      Added R-SCH stats.
05/25/00   sr      Fixed a FEATURE_IS2000_R_SCH bug where some essential common
                   cleanup code was being excluded.
05/24/00   fc      Merged the following changes:
           ry      Added OTAPA support
05/22/00   sr      Inserted some missing FEATURE_IS2000_R_SCH compiler flags
05/16/00   sr      Set sch_on to FALSE in TXC_RELEASE_F
05/16/00   sr      Added support for TXC_MC_TRANS_F action transaction
                   processing for R_SCH command
05/16/00   na      Added R_SCH support
05/08/00   hrk     Added support for FFPC FPC_MODE 011 (EIB 50Hz).
                   In TXC_RELEASE_F, frame offset should be reset to 0
                   only for PLT case.
                   Deleted ERR mesg in txc_scr_action if fch_incl is FALSE.
04/20/00   lad     Updated frame types and pctrl logging.
04/14/00   sr      Swapped arguments to txc_hho() to the proper order
                   Changed the treatment of implicit actions
04/12/00   hrk     Renamed rxc_rpctrl_dec_history to rxc_rpc_history.
04/11/00   lad     Added reverse power control and frame types logging.
                   Cleaned up RLP logging.
04/06/00   na      Fixed RLP logging problem for mobile transmitted frames -
                   the len table was transposed. Fix under FEATURE_RLP_LOGGING
04/05/00   sr      Added programming of power control step size in HO
                   command
                   In RC3 and 4 turned off FCH during preamble as per std.
04/04/00   hrk     Added function rpc_get_relative_gains() to support
                   Reverse Power Control Logging.
03/24/00   bgc     Added extra #ifdef for FEATURE_RLP_LOGGING so that it can
                   be turned off.
03/23/00   lad     Added partial RLP frames logging under FEATURE_RLP_LOGGING.
03/16/00   na      Added support for RLP3 on the R-FCH - dsrlp_build_tx_frames
                   under FEATURE_DS_RLP3.
03/16/00   sr      Fixed a bug with the timing of action timed commands.
                   Added programming of power control step size as part
                   of TXC_PWR_CTRL_F.
02/24/00   lad     Added support for streaming DIAG and backward compatable
                   FEATURE_DIAG_V7_LOG_IFACE.
02/24/00   hrk     Added parameter offset_pwr to txtc for IS2000
                   Added parameter rl_gain_adj for Open Loop Est for IS2000
                   Added function rpc_plt_get_fch_rate to get FCH rates in PLT
02/17/00   hrk     Fixed error in commenting.
02/14/00   hrk     Temporarily commented out TX_RATE_ADJ functionality.
02/12/00   hrk     Added RLGC support.
02/10/00   hrk     Added function to get FCH rate set.
02/08/00   hrk     Modified action time processing to use existing structure
                   definitions.
                   Added action time support for Power Control Message.
                   Added calls to RLGC functions.
02/02/00   sr      Reset frame offsets to 0 and stop tx (FEATURE_PLT only)
                   during TXC_RELEASE_F
01/31/00   sr      Added action time processing and implementation of
                   txc_ho_cmd, txc_hho_ok_cmd and txc_returned_cmd.
                   Parameters related to reverse link gain adjustment and
                   power control step size haven't yet been implemented
                   Merged in Changes from GV40401:
           ks      Changed ERR_FATAL to ERR when txc_acc_probe_suspend is
                   called while the mobile is already in SUSPEND state.
           kmp     Changed txc_acc_probe_resume() definiton and prototype
                   to be for FEATURE_IS95B, not FEATURE_IS95B_ACCESS_PROBE_HO.
01/07/00   sr      In PLT we can now specify the number of preambles to be
                   sent on the reverse traffic channel
12/22/99   hrk     Added gain table for RC3 and above. Gain values are
                   now written to HW in non-PLT code.
12/20/99   rm      Removed calls to srch_pctl_rate_set for MSM5000. The power
                   control puncturing on different RC's  for Fwd link
                   is done in RXC.
11/19/99   sr      Minimum implementation of new mc-txc interface
                   including support for new TXC_SO_F, TXC_CONNECT_SO_F
                   and TXC_RELEASE_F commands
                   Added changes to support data pump and supplemental channels
11/03/99   lcc     Merged from GD4021:
           abh     Changes for updating all the Access Params after APHO
           kmp     Fixed a problem in which SMS service options were not
                   bringing up primary traffic in txc_pri().
           fc      Added FEATURE_IS95B_ACC_PRB_BKOFF_DLY for feature reduce
                   access probe backoff delay.
           fc      Added support for feature reduce access probe backoff delay.
10/20/99   lcc     Removed some unnecessary debug messages.
10/07/99   sr      Fixed frame offset fixes
10/04/99   sr      Merged in MSM5000 support from PLT archive.  Added mux layer
                   support for MSM5000.
09/13/99   abh     Changes for TXC_ACC_SUSPEND_F message processing in
                   ACC_SETUP1_S state.
08/26/99   bgc     Added FEATURE_TX_ACCESS_POWER_FIX.  See txc_acc_freq_track()
                   for details.
08/26/99   aaj     Merged Panther Branch 3.1 to Mainline
08/21/99   ks      Changed Signal MCC_ACCPR_HO_OK_SIG to MCC_TXC_APHO_OK_SIG.
08/13/99   jq      Added casting for ts_get() parameter to clear compiler
                   warnings
08/09/99   ry      Changed FEATURE_IS95B_HARD_HO to FEATURE_IS95B_MAHHO
           ry      Changed FEATURE_IS95B_CFS to FEATURE_IS95B_MAHHO
           ry      Updated txc_send_mc_frame_sig() with the following changes:
                     o Support implicit action time
                     o Generate the second MCC_TXC_FRAME_SIG only if requested
                     o Stop the transmitter only if requested
           ry      Added the following arguments to txc_register_action_time():
                     o Explicit action time indicator
                     o Generate second MCC_TXC_FRAME_SIG indicator
                     o Stop transmitter indicator
           ry      Added txc_get_time_in_1_25ms_unit()
08/06/99   nmn     Removed IS-96
07/29/99   hq      Changes for SMS SO6 and 14 support.
07/24/99   abh     Changes for APHO because of Paging Channel Loss.
07/21/99   aaj     Fixed reverse link markov sync problem by incrementing the frame number
                   passed to tx_mar() function by 1 (txc_mar_traffic() ).
07/19/99   abh     Changes for Access Probe Handoff.
07/09/99   ry      Added txc_stop_tx() and txc_set_frame_offset()
                   Added txc_send_mc_frame_sig() and txc_register_action_time()
                   as part of HHO action time fix
                   Added txc_flag_next_int() and txc_int_occurred() to support
                   IS95B HHO and CFS
           ry      Removed all frame offset processing from txc_traffic()
06/24/99   ks      Fixed a typo in txc_suspend_tx
06/17/99   kmp     Merged in the one change listed below.
06/08/99   aaj     Fixed one problem in TXC_TRAFFIC function under feature_minimize_isr_loading
                   It would cause premature exit from txc_traffic loop resulting in
       possibly trucated long traffic channel messages
                   Also corrected few comments as suggested in the code review.
06/07/99   ks      Added code to suspend/enable the Probe Transmission, this
                   is used when T72M expires, added TXC_SUSPEND_S state.
05/10/99   kmp     Merge IS-95B changes listed below.
           kmp     Added additional diagnostic messaging for Open Loop Power
                   Estimate Correction testing in txc_access_att().
           fc      Put in changes based on input from code review.
           fc      Changed the timing for setting MCC_TXC_PROBE_SIGNAL.
           fc      Put access channel message probe signalling under
                   FEATURE_IS95B_PILOT_RPT.
           fc      Added Access Channel Message Probe signalling.
           sh      Added open loop power estimate correction feature
04/30/99   abh     Increased the buffer size for the msg buffer to allow upto 255 bytes
                   of Reverse Link Messages.
04/22/99   bgc     Added in srch_finger_lock_handler as
                   FEATURE_FINGER_POLL.
04/22/99   aaj     Undone the (cast) operator changes made for armtools 2.5
04/19/99   aaj     Usage of (cast) operator in ts_get to support armtools 2.5
04/15/99   aaj     Removed MDR only timeline for the ENC Interrupt.
04/08/99   aaj     Added FEATURE_DS around a DS_ISDATA macro used in txc_traffic_init
                   under FEATURE_MINIMIZE_ISR_LOADING
03/25/99   aaj     Made some changes in TXC processing and enc isr timeline
                     1. Two different enc intr timelines for data and voice
                     2. Moved RLP TX and frame building to TXC task
                     3. Moved rxc_voc_xchg to RXC task
                     4. Optimized/changed frame number counting in encoder ISR
                   The above mentioned changes go under FEATURE_MINIMIZE_ISR_LOADING
02/26/99   ejv     Removed srch_finger_lock_handler due to interrupt loading.
01/15/99   aaj     Merged MDR branch with Panther mainline
                   Edit History for MDR branch
                        12/09/98   smp     Include mdrrlp.h instead of rlp.h for MDR
                        12/03/98   lcc     Added support for DPT service options.
11/03/98   abh     Changes for frame no. correction after frame offset change.
10/22/98   pms     Backed out nearing of variables.
10/02/98   pms     Neared variables to save ROM space.
09/14/98   bgc,br  Added 5ms callback of srch_finger_lock_handler() to
                   monitor the state of the fingers during TX on the traffic
                   channel and turn off power control when the fingers go out
                   of lock. This solves a TX power burst problem when the
                   fingers go out of lock.
08/11/98   ck      Added support for EVRC Option
04/06/98   lcc     Added comments for r1.118 (and this one).
04/01/98   lcc     Fixed CR 4994 where we try to transmit when waiting for the
                   good frames during TCI (we shouldn't be transmitting at all).
                   See revision description for more details.
03/25/98   dna     Fixed bug where 2 reserved bits before CRC were non-zero.
03/12/98   aks     Removed pre-Gemini conditionals for Encoder (Tx) interrupt
                   offset.
01/27/98   ldg     Commented usages of the unusual DS_CASE macro.
01/26/98   fkm     FEATURE_xxx Updates (via Automatic Scripts)
11/03/97   ldg     Replaced references to specific data service options with
                   a macro which represents the same information.
06/17/97   dhh     Configured for Q (T_Q).
05/29/97   na      Added correct data service option numbers - fixes service
                   option change while in traffic channel. Changes under ifdef
                   DS.
03/02/97   ptw     Fixed mishandling of txac.log_ptr that resulted in log
                   buffers being lost or re-freed.
11/14/96   jjw     Added Rate Set 2 data service capabilities.
10/21/96   gb      Fixed repeated full rate frames at very end of call.
09/26/96   gb      Reduced multiple dim&burst requests for signalling.
09/24/96   gb      Moved frame offset pending checking to background task.
09/19/96   gb      Ifdef'd vocoder logging TMOBILE to save RAM and ROM.
09/06/96   gb      Sped up erasure indicator bit timing in tc_isr
                   Restored reverse link voice logging capability.
08/16/96   gb      Power level after hard handoff includes current gain_adj.
                   Power down before hard handoff sync to frame boundary.
08/08/96   gb      Fixed frame time calculation to account for frame offset
                   assignments/handoffs in Markov calls.
07/10/96   gb      Modified timing of data flow from voc_traffic to
                   build_frame and added lookahead to un-dim next frame if
                   we no longer need it.  Made sure that we never try to
                   build message frames when active_msg is FALSE which in a
                   weird race condition will cause the High FER powerdown
                   if lots of PMRM messages are being generated in 13K.
06/05/96   dna     Modified TXC_TC_F in traffic state to change mux option.
05/09/96   gb      Do not ask vocoder for BLANK frames if not transmitting.
04/29/96   gb      Set puncturing off after turning clocks on as a precaution.
04/09/96   gb      TX now transmits all frames in a probe, filling with 0s.
04/08/96   gb      PA now turns off nicely at end of last frame in probe.
03/18/96   rah     Added VOC_FULL_ONLY full rate voice only flag. This is
                   for hearing aid users.
03/14/96   dna     Removed sign extention of nom_pwr, now done in mccsa.c.
                   Changed txc_hho() to convert nom_pwr to proper units.
03/04/96   rah     Updated CSS1 conditionals.
03/01/96   gb      Made vocoder pkt xchg callable by rxc_tc_isr.
                   Called rxc_reset_voc_init whenever we change frame offsets.
02/07/96   gb      Fixed countdown to probe when persistence test fails.
12/14/95   gb      Tightened up tx jump to hyperspace.
11/16/95   jca     Removed INTLOCK/INTFREE pair in function txc_cmd().
11/07/95   gb      Linted and removed old unused variables and code.
10/20/95   gb      Fixed internal mux1 array bounds problem by removing it.
10/06/95   gb      Changed call release to switch tx to SO_NULL.
10/03/95   dna     Modified txc_hho for case where num_preambles = 0
09/28/95   rah     Fixed forward power control.
09/18/95   jca     Fixed Markov sync problem for Gemini target.
09/07/95   gb      Moved enc_data_out from ISR to task to lessen delay.
08/25/95   gb      Inform TX of the HHO parameters num_preamble and nom_pwr.
08/23/95   ptw     Crippled vocoder logging to save a little RAM.
08/15/95   gb      Added MUX2 signalling parameter lookups for TXC.C
08/14/95   gb      Added MUX2 parameter support.
08/14/95   gb      Added new Loopback_13K service option support.
08/10/95   gb      Added New Markov Service Options and made all service
                     options CAI_SO_*.
07/27/95   gb      Fixed frame offset Markov sync bug for 8k and 13k.
07/26/95   gb      Fixed the heinous frame offset bug in txc_access_cmd.
07/20/95   rah     Changed tx interrupt offsets. Moved the txc task run to
                   the traffic ISR for GEMINI and ISS2 targets.
07/13/95   gb      Changed mux option initialization.
07/06/95   rah     Removed refs. to T_13K.
06/19/95   gb      Fixed zero frame after transmitter turn off and on.
06/16/95   jah     Fixed transmit frame time for Frame Offset > 3.
06/14/95   gb      Added support for Rate set 2 service options.
06/07/95   ptw     Removed special boost for BTU mobiles.  Not required.
05/22/95   gb      Fixed multiple cmd_done() bug in acc_msg_prep.
05/19/95   ptw     Added support for BTU mobiles.
05/09/95   rdh     Fixed zeroing of TC power after probing up, disable Boost.
05/01/95   rdh     Fixed 1dB-1/2dB mismatch in pwr_step step size units.
03/29/95   jca     Now setting signal when transitioning to traffic state.
01/04/95   gb      Fixed frame_build Markov frame for new frame timing.
12/23/94   jca     Added nom_pwr to initial transmit power calculations.
11/29/94   gb      Changed traffic_isr for new TX frame timing.
11/22/94   gb      Rhometer test functionality changed to standard TMOBILE.
11/11/94   jca     Added code to support access probe PN randomization.
                   Modified code so that TXC signals MC task at
                   completion of access attempt.  Rewrote txc_wait().
11/01/94   gb      Sector Lock test functionality added.
10/17/94   gb      Fixed traffic channel preamble crc setting to OFF.
10/13/94   jca     Fixed txac.countdown bug and other misc bug fixes per
                   Dodge Johnson's email.
06/07/94   jca     Power step size now the same for PCN and cellular modes.
05/12/94   gb      Added support for new test commands.
05/05/94   gb      Fixed power ctl disable in tx-power-boost.
04/20/94   jca     Mods for IS-95 Phase 2 upgrade.
04/04/94   gb      Use ts_get to set tx access frame timing always.
01/06/94   jah     Changed watchdog iface to use dog_report().
12/22/93   jca     Mods for IS-95 Phase 1 upgrade.
09/22/93   jca     Now processing TXC_CDMA_F command in ACC and TC states.
06/11/93   jca     Changed code so that we now power up TX portion of RF
                   h/w, delay 5 msecs, and THEN program synths as per the
                   new RF hardware spec.
10/22/92   jai     Fixed problem with double access timeout waits.
10/20/92   jai     Made fix to access probe logging queue processing to
                   prevent losing buffers from queue.
10/15/92   jai     Changed processing when vocoder returns blank while
                   expecting a dim frame.
10/06/92   jai     Added access probe logging support.
09/21/92   jai     Added ACC_ENDPROBE_S as valid state to receive
                   TXC_TIME_STOP_F command during access state.
04/20/91   rrd     Massive overhaul and upgrade to full body
04/08/91   rrd     Created

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common
#include "1x_variation.h"
#include <memory.h>
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"

//mux
#include "ffpc_i.h"
#include "rlgc.h"
#include "rlgc_i.h"
#include "txccommon_i.h"
#include "txctraffic_i.h"
#include "txcaccess.h"
#include "muxmdsp_i.h"
#include "muxmsgr.h"
#include "txc_v.h"
#include "txc_i.h"
#include "txcmc_i.h"
#include "txcmux.h"
#include "mux_logtypes.h"
#include "muxnv_i.h"
#include "txc.h"

//drivers
#include "enc.h"
#include "enc_v.h"
#include "enc_i.h"
#include "enci.h"
#include "enci_i.h"             /* Encoder register & macro definitions */
#include "m1x_rm.h"
#include "m1x_stmr_i.h"
#include "m1x_time.h"
#include "m1x_hwio_mpss.h"

//diag
#include "cdma2kdiagi.h"

//cp
#include "cai.h"
#include "tx.h"
#include "tx_v.h"
#include "mccdma_i.h"

//srch
#include "srch_mux.h"
#include "srch_rx.h"
#include "srch_rx_div_i.h"

#ifdef FEATURE_MODEM_1X_COEXISTENCE
#include "srch_coex.h"
#endif /* FEATURE_MODEM_1X_COEXISTENCE */

//other
#include "qw.h"
#include "queue.h"
#include "rex.h"
#include "err.h"
#include "log.h"
#include "msg.h"
#include "m1x_diag.h"
#include "dog.h"
#include "dog_hb_rex.h"
#include "cmd.h"
#include "subs_prio.h"
#include "DALSys.h"
#include "rfm_device_types.h"

//feature dependent
#include "dsrlp_stats.h"

#ifdef FEATURE_MODEM_1X_SUPPORTS_RF
#include "rfm_1x.h"
#include "rfm_cdma.h"
#endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

#include "muxmsgr.h"
#include "lm_types.h"
#include "txlm_intf.h"


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* declare a critical section that is common throughout the TX task */
rex_crit_sect_type txc_crit_sect;

#ifdef FEATURE_IS2000_1X_ADV
/* Data structure to hold local copies of the 1x Advanced RCPM data */
cai_rcp_rxc_txc_type txc_rcpm_data;
#endif /* FEATURE_IS2000_1X_ADV */

/* Queue definition */
q_type txc_cmd_q;

/* Timer to wake Transmit task so that it can set its Watchdog task
   Report signal */
rex_timer_type  txc_rf_timer;

#define TXC_PWR_MIN   0x7F    /* Tx gain adjust minimum value */
#define TXC_PWR_BOOST (3*2)   /* 1/2 dB increments for boost on full rate */
#define TXC_T2P_GAIN  10      /* the T2P based on the turbo rate gains */

#define TXC_SHUTDOWN_COUNTER 40

cai_data_rate_type txc_data_rate = CAI_BLANK_RATE; /* Data rate of current frame */

#define TX_IS_OFF       0x8000 /* The OFF sentinel for QXDM parser = -32768 */

#define TX_IS_OFF_Q8    0x80   /* The OFF sentinel for QXDM parser = -128 */

#ifdef FEATURE_IS2000_REL_A_CC

byte zeros[TXC_REACH_20MS_FRAME_38400_DATA_RATE_SIZE]={
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };   /* 20ms, 38400 data rate all zeros packet */

#else

byte zeros[ENC_14400_FULL_RATE_SIZE]={
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };   /* Full rate all zeros packet */

#endif // FEATURE_IS2000_REL_A_CC


/* Curent state of TXC state machine */
txc_state_type  txc_state;

/* command block for interrupt handler commands */
txc_int_cmd_type_main txc_int_cmd;

/* hold most of the global variables for the transmit task */
txc_tx_type txc_tx;

txc_frame_offset_cmd_type_pend txc_frame_offset_cmd;

/* Offset of the transmit interrupt from the frame boundary for Traffic
** Channel.
*/
byte txc_frame_offset;

txc_so_type txc_so;

#ifdef FEATURE_IS2000_REL_A

txc_srid_ref_type  txc_srid_ref[SR_ID_MAX];

#endif /* FEATURE_IS2000_REL_A */

txc_gain_type txc_pch_fch_sch_gain_tab[ ENC_NUM_SCH_RATES ]
                                         = {
                               /* PCH     FCH     DCCH    SCH */
                                { 106<<7, 164<<7, 164<<7, 164<<7 },
                                { 106<<7, 164<<7, 164<<7, 164<<7 },
                                { 106<<7, 164<<7, 164<<7, 164<<7 },
                                { 106<<7, 164<<7, 164<<7, 164<<7 },
                                { 93<<7,  141<<7, 141<<7, 191<<7 },
                                { 88<<7,  116<<7, 116<<7, 209<<7 },
                                { 80<<7,  91<<7,  91<<7,  225<<7 },
                                { 71<<7,  65<<7,  65<<7,  236<<7 },
                                { 61<<7,  43<<7,  43<<7,  244<<7 }
                                           };

/* Gain table based on Nominal Attribute Gains in IS2000.
** Each entry is shown in 8 bit value for reference.  Since MDSP requires
** 15 bit gain programming, each entry is left shifted by 7.
** This table is used when there is no R-SCH.
*/
txc_gain_type txc_pch_fch_gain_tab[ ENC_NUM_FRAC_RATES ]
                                       = {
                                { 147<<7, 208<<7, 0, 0 }, /* FULL */
                                { 180<<7, 180<<7, 0, 0 }, /* HALF */
                                { 208<<7, 147<<7, 0, 0 }, /* QTR  */
                                { 228<<7, 114<<7, 0, 0 }  /* 8TH  */
                                         };

/* Adjusted R-PCH Gain Table when only R-FCH is active
** and R-SCH is not assigned */
int16 txc_pch_adj_gain_tab[ ENC_NUM_FRAC_RATES ] =
                                         {
                                              43,  /* FULL */
                                              26,  /* HALF */
                                              12,  /* QTR  */
                                              2    /* 8TH  */
                                         };
  int16 txc_sch_pch_adj_gain_tab[ ENC_NUM_FRAC_RATES ];

  /* Gain Table when R-FCH and R-SCH is assigned and R-SCH is active */
  txc_gain_type txc_gain_tab[ ENC_NUM_FRAC_RATES ];

  /* Gain table when R-FCH and R-SCH is assigned but R-SCH is inactive */
  txc_gain_type txc_dtx_gain_tab[ ENC_NUM_FRAC_RATES ];

  int16 txc_dtx_pch_adj_gain_tab[ ENC_NUM_FRAC_RATES ];

  txc_dcch_sch_gain_type txc_dcch_sch_gain_tab[TXC_DCCH_SCH_CONFIG_MODES];

#ifdef FEATURE_IS2000_REL_A_SVD
/* VP2 RPC new structure */
txc_fch_dcch_sch_gain_type txc_fch_dcch_sch_gain_tab[ENC_NUM_FRAC_RATES][TXC_DCCH_SCH_CONFIG_MODES];
#endif /* FEATURE_IS2000_REL_A_SVD */

#ifdef FEATURE_IS2000_1X_ADV
txc_linear_gain_ratio_type txc_fch_ack_gain_ratio_tab[ ENC_NUM_FRAC_RATES ][ TXC_NUM_ACS_TYPES ];
txc_linear_gain_ratio_type txc_fch_sch_ack_gain_ratio_tab[ ENC_NUM_FRAC_RATES ][ TXC_NUM_ACS_TYPES ];
txc_linear_gain_ratio_type txc_fch_schdtx_ack_gain_ratio_tab[ ENC_NUM_FRAC_RATES ][ TXC_NUM_ACS_TYPES ];
#endif /* FEATURE_IS2000_1X_ADV */

/* System Action Time as specified in OTA message */
word  txc_action_time;

boolean txc_action_time_chk_enabled = FALSE;
  /* When TRUE, txc_traffic() should check if the action time is up
   * while processing "TXC_INT_SIG". If the action time is up,
   * TXC will send "MCC_TXC_FRAME_SIG" to MCC
   */

boolean txc_action_time_stop_tx_enabled;
  /* When TRUE, TXC will command the transmitter to be shut down at
   * action time, i.e. while the first MCC_TXC_FRAME_SIG is generated
   */

boolean txc_next_int_occurred;

static rfm_device_enum_type onex_tx_device = RFM_INVALID_DEVICE;

#ifdef FEATURE_MODEM_1X_SUPPORTS_RF
static lm_handle_type onex_tx_lm_handle = 0;
#endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

#ifdef FEATURE_MODEM_1X_DRX
boolean rxc_fpc_mode_changed = FALSE;
#endif /* FEATURE_MODEM_1X_DRX */

/*-------------------------------------------------------------------------*/
/* Modem Power Statistics                                                  */
/*-------------------------------------------------------------------------*/
/* Bucket thresholds for binning.
   value indicates upper threshold for tx power */
int32 txc_tx_power_bin_thresh[ TXC_NUM_MODEM_POWER_BINS ] =
  { 0, 8, 15, 20, 100 };

/*-------------------------------------------------------------------------*/
/* TX task DOG parameters                                                  */
/*-------------------------------------------------------------------------*/

dog_report_type txc_dog_rpt_id = 0;

rex_timer_type txc_failsafe_timer;
rex_timer_type txc_shutdown_wait_timer;

/* This flag keeps track whether response has been sent to RXTX task for
 * TXC_MSG_F cmd or not. */
boolean txc_need_to_send_msg_rsp =  FALSE;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION TXC_DOG_REPORT

DESCRIPTION
  This function sends report to Dog heartbeat mechanism and clears the
  dog report signal.

DEPENDENCIES
  Depends on previously initialized DOG report ID.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void txc_dog_report( void )
{
  dog_hb_report(txc_dog_rpt_id);
  (void) rex_clr_sigs( TX_TCB_PTR, TXC_DOG_RPT_SIG );

} /* txc_dog_report */

/*===========================================================================

FUNCTION TXC_WATCHDOG_INIT

DESCRIPTION
  This function is used to initialize the TX task with the DOG subsystem

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  WDOG timer system is started for TX task
===========================================================================*/
void txc_watchdog_init( void )
{

  /* Register with dog task */
  txc_dog_rpt_id = dog_hb_register_rex( TXC_DOG_RPT_SIG );

  /* Report to watchdog task. */
  txc_dog_report();

} /* tx_watchdog_init */

/*===========================================================================

FUNCTION TXC_CMD_DONE

DESCRIPTION
  Dispose of a command buffer when we're done with it.  Depending of
  the contents of the buffer, this will involve placing it on a free
  queue if a free queue is designated,  and if a tcb is given,  the
  appropriate task will be notified.

DEPENDENCIES
  None.
RETURN VALUE
  Just returns the status passed to it
SIDE EFFECTS
  Could signal another task
===========================================================================*/
void txc_cmd_done
(
  txc_cmd_type    *cmd_ptr,         /* buffer to dispose of */
  txc_status_type status            /* set status of buffer */
)
{
  cmd_ptr->hdr.status = status;

  cmd_done( &cmd_ptr->hdr.cmd_hdr );

}

/*===========================================================================

FUNCTION TXC_GET_COST_OF_MODEM_POWER

DESCRIPTION
  Provides the cost of modem power in IDLE and TRAFFIC.

DEPENDENCIES
  None

RETURN VALUE
  Cost of modem power.

SIDE EFFECTS
  None
===========================================================================*/
txc_power_level txc_get_cost_of_modem_power(void)
{
  int16 txc_power = 0, rxc_power = 0;
  int32 cdma_turnaround_const = 0;

  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  rfm_device_enum_type device;
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

  /*---------------------------------------------------------------------*/

  if(txc_tx_is_on())
  {
    /* the tx power level can be found directly as it is on */
    txc_power = txc_get_filtered_tx_power_dbm10()/10; /* converted to dBm*/
  }
  else
  {
    #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
    /* find an estimate of tx power based on current rx power */
    device    = srch_rx_get_device_in_use(CLIENT_1X_RX_PRI);

    /* rx power in dBm units*/
    rxc_power = rfm_1x_get_filtered_rx_power(device)/10;
    #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

    cdma_turnaround_const = srch_rx_get_cdma_turnaround_const();

    /* find the filtered tx pilot power in dBm units */
    txc_power = cdma_turnaround_const - rxc_power;

    /*

    TxTotalPwr_linear = FiltTxPilotPwr_linear * (1 + 10^(T2P_dB/10))
    FiltTxPilotPwr_linear is FiltTxPilotPwr (in dBm) converted to linear domain;
    T2P_dB = T2P for 153.6kbps
    Convert TxTotalPwr_linear to dBm domain

    */

    txc_power = txc_power + TXC_T2P_GAIN;
  }

  /* map TxTotalPwr to cost of modem power */
  if(txc_power < 0)
  {
    return TXC_POWER_LOW;
  }
  else if(txc_power <= 10)
  {
    return TXC_POWER_MED;
  }
  else
  {
    return TXC_POWER_HIGH;
  }

}/* txc_get_cost_of_modem_power */

/*===========================================================================

FUNCTION TXC_CONFIG_MODEM_POWER_STATS

DESCRIPTION
  This function enables or disables tx duration logging

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_config_modem_power_stats
(
  boolean enable                       /* En/disable statistic collection */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   M1X_MSG( MUX, LEGACY_HIGH,
     "Modem Power Stats Log: en/dis = %d->%d",
     txc_tx.mdm_pwr_stats.enable,
     enable );

  /* Reset the stats for every enable/disable request */
  rex_enter_crit_sect( &txc_crit_sect);

  for( int i = 0; i < TXC_NUM_MODEM_POWER_BINS; i++ )
  {
    /* Clear table */
    txc_tx.mdm_pwr_stats.tx_power_bins[i] = 0;
  }

  txc_tx.mdm_pwr_stats.tx_overall         = 0;

  rex_leave_crit_sect( &txc_crit_sect );

  txc_tx.mdm_pwr_stats.enable = enable;

} /* txc_config_modem_power_stats */

/*===========================================================================

FUNCTION TXC_GET_MODEM_POWER_STATS

DESCRIPTION
  This function fills Tx duration values into the provided table.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_get_modem_power_stats
(
  uint32 tx_power_bins[]               /* Pointer to the bin array */
)
{

  int32 i = 0;
  boolean partial_frame_update = FALSE;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( tx_power_bins == NULL )
  {
    return;
  }

  if ( txc_tx.mdm_pwr_stats.enable == FALSE )
  {
     M1X_MSG( MUX, LEGACY_HIGH,
       "Modem Power Stats Log: disabled" );

     return;
  }

  rex_enter_crit_sect( &txc_crit_sect);

   M1X_MSG( MUX, LEGACY_HIGH,"Modem Power Stats, tx_overall=%d",
            txc_tx.mdm_pwr_stats.tx_overall);
  /* Cycle through bins and update appropriate one. */
  for ( i = 0; i < TXC_NUM_MODEM_POWER_BINS; i++ )
  {

    /* This is workaround code, under-report anyone 
       frame count to avoid partial frame TX counting */
    if( ( partial_frame_update == FALSE ) &&
        ( txc_tx.mdm_pwr_stats.tx_power_bins[i] != 0 ) )
    {
      txc_tx.mdm_pwr_stats.tx_power_bins[i]--;
      partial_frame_update = TRUE;
    }

    /* Convert frames to ms */
    /* Overflow may occur if delta is > 7 hrs.
       This is known limitation of API. */
    tx_power_bins[i] = ( txc_tx.mdm_pwr_stats.tx_power_bins[i] * 20 );

    M1X_MSG( MUX, LEGACY_HIGH, "Modem Power Stats, tx_power_bins[%d]=%d",
                                i, txc_tx.mdm_pwr_stats.tx_power_bins[i] );
    /* Clear table */
    txc_tx.mdm_pwr_stats.tx_power_bins[i] = 0;
    txc_tx.mdm_pwr_stats.tx_overall       = 0;
  }

  rex_leave_crit_sect( &txc_crit_sect );

} /* txc_get_modem_power_stats */

/*===========================================================================

FUNCTION TXC_UPDATE_POWER_STATS

DESCRIPTION
  Update statistics Tx power level.
  The expectation is that this will be called upon each and every frame isr.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_update_power_stats( void )
{
  int32  i = 0;                        /* Loop varaible */
  int16  tx_power = -1280;             /* Tx total power */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Skip if not enabled */
  if ( txc_tx.mdm_pwr_stats.enable == FALSE )
  {
     return;
  }

  /* check if the transmitter is on */
  if ( txc_tx_is_on() )
  {
    /* get the transmit power in 1/10 dbm resolution */
    tx_power = txc_get_filtered_tx_power_dbm10();

    /* Find appropriate bin */
    for ( i = 0; i < TXC_NUM_MODEM_POWER_BINS; i++ )
    {
      if ( tx_power < txc_tx_power_bin_thresh[i] )
      {
        break;
      }
    }
    /* No bin found, set it as the Max Power */
    if ( i == TXC_NUM_MODEM_POWER_BINS )
    {
      i = TXC_NUM_MODEM_POWER_BINS - 1;
    }

    rex_enter_crit_sect( &txc_crit_sect );

    /* Bin found, increment the count */
    txc_tx.mdm_pwr_stats.tx_power_bins[i]++;
    txc_tx.mdm_pwr_stats.tx_overall++;

    rex_leave_crit_sect( &txc_crit_sect );
  }

} /* txc_update_power_stats */

/*===========================================================================

FUNCTION TXC_SUSPEND_TX

DESCRIPTION
  This functions enables/Disables the Transmitter and enables/disables the
  transmission of Access probes.

DEPENDENCIES
  Depends on the current state of the TX task.

RETURN VALUE
  None.

SIDE EFFECTS
  This function is called when the T72 fade timer expires to temporarily
  enable/disable the transmission of the Access Probes.

  Note that there are two mechanisms to suspend the probing. First is by
  calling this function and second mechanism is by sending a SUSPEND message
  to TXC. The difference between these two mechanism(other than one being
  message level interface and another being message level interface), that
  the message level interface waits for TA timer to expire and then responds
  back to MC with ACCPR_HO_OK sig.  Function based interface does not do so.

===========================================================================*/
void txc_suspend_tx ( boolean cntrl_flag )
{
  boolean prev_stop_probes;

  prev_stop_probes = txac.stop_probes;
  txac.stop_probes = cntrl_flag;

  /* If the Transmission was suspended during Access attempt, set the
  ** TXAC state properly to resume the transmission of probes */

  if((!txac.stop_probes) && (prev_stop_probes) && (txc_state == TXC_ACC_ATT_S))
  {
    txc_acc_probe_resume();
  }
}


/*===========================================================================

FUNCTION TXC_RPT_VOC_RATE

DESCRIPTION
  Reports the vocoder rate to the calling routine.

DEPENDENCIES
  None.

RETURN VALUE
  Current frame data rate.

SIDE EFFECTS
  None.

===========================================================================*/
cai_data_rate_type txc_rpt_voc_rate( void )
{
  return (txc_data_rate);
}


/*===========================================================================

FUNCTION TXC_CLR_MSGS

DESCRIPTION
  Move any pending messages in the messages buffer over to the free
  queue,  effectively erasing any queued messages.

DEPENDENCIES
  None.
RETURN VALUE
  None.
SIDE EFFECTS
  None.
===========================================================================*/
void txc_clr_msgs( void )
{

  if( txc_tx.active_msg ) {               /* if we're working on a message */
    txc_tx.active_msg = FALSE;                     /* and null the pointer */
    txc_tx.delay = TXC_NO_CONF;                   /* no confirmation delay */
  }
  /* If message is still active or a reponse is still pending, send the
   * reponse to RXTX task */
  if (txc_tx.active_msg || txc_need_to_send_msg_rsp)
  {
    txc_cmd_done( txc_tx.buf.cmd_ptr, TXC_DONE_S );
    txc_need_to_send_msg_rsp = FALSE;
  }
}


/*===========================================================================

FUNCTION TXC_WAIT

DESCRIPTION
  Wait for the watchdog and for any other signals that might get set.
  Return if one of the other signals is set.  Always kick the watchdog
  whenever we wake up.

DEPENDENCIES
  None.

RETURN VALUE
  Returns the signals for the current task.

SIDE EFFECTS
  None.

===========================================================================*/
rex_sigs_type txc_wait
(
  word requested_mask
    /* Mask of signals to wait for */
)
{
  rex_sigs_type queue_mask;        /* Mask of signals indicating queue status */
  rex_sigs_type rex_signals_mask;  /* Mask of signals returned by rex_get_sigs() */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (requested_mask == 0)
  {
    /* No sigs to wait for --> Kick the watchdog and return to caller */
    txc_dog_report();
    return (0);
  }

  /* Initialize temporary mask holder for queue information */
  queue_mask = 0;

  /* --------------------------------------------------------------------
  ** Check TXC command queue: If the request mask contains a check for
  ** TXC_CMD_Q_SIG then we should check txc_cmd_q, and set in queue_mask
  ** an indication that something is waiting to be serviced on the queue.
  ** The rex signal is also cleared regardless of the queue count.  This
  ** is to cover the case where there is a signal set but nothing on the
  ** queue.
  ** -------------------------------------------------------------------- */
  if (requested_mask & TXC_CMD_Q_SIG)
  {
    (void) rex_clr_sigs( TX_TCB_PTR, TXC_CMD_Q_SIG );

    if (q_cnt( &txc_cmd_q ))
    {
      queue_mask |= TXC_CMD_Q_SIG;
    }
  }

  /* Get current state of rex signals */
  rex_signals_mask = rex_get_sigs( TX_TCB_PTR );

  /* Check for task-lock if we haven't received the signal yet */
  if ( ( ( rex_signals_mask | queue_mask ) & requested_mask ) == 0 )
  {
    if ( rex_tasks_are_locked() )
    {
      ONEX_ERR_FATAL( "rex_wait during task-locked condition in TXC", 0, 0, 0 );
    }
  }

  /* --------------------------------------------------------------------
  ** Loop until the current signals from rex (including the queue signals
  ** faked due to an item on a queue, if any) match with one or more of
  ** the requested signals.
  ** -------------------------------------------------------------------- */
  do
  {
    /* Kick the watchdog */
    txc_dog_report();

    /* Perform the 'real' wait (if needed) */
    if (((rex_signals_mask | queue_mask) & requested_mask) == 0)
    {
      rex_signals_mask = rex_wait( requested_mask | TXC_DOG_RPT_SIG );
    }

    /* ---------------------------------------------------------------
    ** Make sure that if we got a queue signal, that there is actually
    ** an item on the queue indicated
    ** --------------------------------------------------------------- */
    if (rex_signals_mask & TXC_CMD_Q_SIG)
    {
      (void) rex_clr_sigs( TX_TCB_PTR, TXC_CMD_Q_SIG );

      if (q_cnt( &txc_cmd_q ) == 0) {  /* No elements in queue ? */
        rex_signals_mask &= ~TXC_CMD_Q_SIG;  /* Turn off bit */
      }
    }

    /* Do while mask requested does not match current mask */
  } while (((rex_signals_mask | queue_mask) & requested_mask) == 0);

  /* Kick watchdog one last time on the way out */
  txc_dog_report();

  return ((rex_signals_mask | queue_mask));

} /* end txc_wait() */


/***************************************************************************/
/*                         CDMA State Procedures                           */
/***************************************************************************/

/*===========================================================================

FUNCTION TXC_CDMA_ENC_SHUTDOWN

DESCRIPTION
  Shut down the encoder / interleaver / deinterleaver & PA for CDMA

DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void txc_cdma_enc_shutdown( void )
{
  boolean ta_is_enabled = txc_is_ta_enabled();

  /* Always delay at least 1 frame before traffic/access teardown */
  M1X_MSG( MUX, LEGACY_HIGH,
    "Delay shutdown enc clocks till next frame, FW disable Tx flag" );

  /* kick the dog before using the report timer */
  txc_dog_report();

  if( txc_tx.rf_state != TXC_RF_STATE_DISABLE )
  {
    uint32 current_frame_num = 0;
    uint32 next_frame_num    = 0;
    uint32 loop_counter      = 0;
    uint32 current_pcg_num   = 0;

    /* Make sure FW disables Tx operation at next frame boundary */
    enc_tx_enable( FALSE );

    txc_tx.tx_on = FALSE;

    if( ta_is_enabled )
    {
      m1x_stmr_get_frame_and_pcg_num_with_frame_offset( &current_frame_num, &current_pcg_num );

      if( current_pcg_num > 11 ) 
      {
        do
        {
          /* Do a busy wait for 0.5ms and then check if frame count has increased */
          DALSYS_BusyWait( 500 );
          m1x_stmr_get_frame_and_pcg_num_with_frame_offset( &next_frame_num, NULL );

          loop_counter++;
          if ( loop_counter > TXC_SHUTDOWN_COUNTER )
          {
            break;
          }
        } while ( current_frame_num == next_frame_num );
      }
      else
      {
        DALSYS_BusyWait( 1500 );
      }
    }
    else
    {
      DALSYS_BusyWait( 20000 );
    }

    /* Switch back to rate set 1 for accesses */
    enc_fch_rc( ENC_FCH_RC_1 );
    enc_sch_rc( ENC_SCH_RC_3 );
    enc_rate_set( ENC_RATE_9600 );

    /* Reset Pilot gating mask before TX shutdown - FW needs this be
       set properly, No SW requirement */
    txc_device_only_dtx_param.dtx_frame = FALSE;
    txc_device_only_dtx_param.dtx_mask  = DTX_SB_MASK_DISABLED;
    muxmdsp_set_channel_gains();

    M1X_MSG( MUX, LEGACY_HIGH,
       "1X has primary chain, shutdown enc clocks");

    /* In case of WTR HOP based inter band HHO/CFS, save the Tx Adj here.
     * To be used later when Tx is turned ON */
     /* Also save Tx Adj if TA GAP is open */
    if( ta_is_enabled                         ||
        mcc_is_hho_with_wtr_hopping_in_prog() ||
        mcc_is_cfs_with_wtr_hopping_in_prog()
      )
    {
      txtc.power = srch_mux_get_tx_adj(); /* save power */
      txtc.power -= (byte) (2 * txtc.rl_gain_adj);
    }

    /* Disable TX */
    txc_change_rf_tx_state( TXC_RF_STATE_DISABLE );

    /* Disable Tx clocks */
    enc_change_tx_clk_resources( ENC_TX_CLOCKS_DISABLE );

    #ifdef FEATURE_MODEM_1X_DIMEPM_REVISIT
    /* need to go through idle first, if already enabled */
    m1x_rm_set_state( M1X_RM_TX, M1X_RM_1X_IDLE );
    m1x_rm_set_state( M1X_RM_TX, M1X_RM_1X_INACTIVE );
    #endif /* FEATURE_MODEM_1X_DIMEPM_REVISIT */
  }
  else
  {
    M1X_MSG( MUX, LEGACY_HIGH,
       "MUX does not own TX, exit without turn off TX ");
  }

  /* kick the dog before resetting the report timer */
  txc_dog_report();

} /* txc_cdma_enc_shutdown */

/*===========================================================================

FUNCTION TXC_TX_SHUTDOWN

DESCRIPTION
  Turns off tx clock and disables TXC interrupt.  Allows MC to command SRCH to
  sleep.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void txc_tx_shutdown( void )
{
  M1X_MSG( MUX, LEGACY_HIGH,
    "Shutdown TX clock" );

  txc_cdma_enc_shutdown();

  m1x_stmr_deregister_client( M1X_STMR_CLIENT_TX );

  /* Allow MC/SRCH to sleep */
  txc_set_ok_to_sleep( TRUE );

} /* txc_tx_shutdown */

/*===========================================================================

FUNCTION TXC_EXIT

DESCRIPTION
  This function performs processing necessary for exiting CDMA Transmit
  subtask.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void txc_exit ( void )
{
  txc_tx_shutdown();   /* turn off clocks, pa, etc. */

  /* clear signals */
  ( void )rex_clr_sigs( TX_TCB_PTR,  0xFFFF );

  if (log_status(LOG_REV_PCTRL_C))
  {
    txc_rpctrl_send_log();
  }

  #ifdef FEATURE_IS2000_REL_A
  if( log_status( LOG_RDCH_FRAME_INFO_C ) )
  {
    txc_rdch_frame_info_send_log();
  }
  #else
  if( log_status( LOG_REV_FRAME_TYPES_C ) )
  {
    txc_frame_types_send_log();
  }
  #endif /* FEATURE_IS2000_REL_A */
  /* Channel configuration has changed, we need to run the RF PA backoff algorithm
  to reset all the PA power*/
  txc_update_chan_config_to_rf( FALSE );

  /* Ensure this flag is set to FALSE while de-activating TX. This is required
   * to ensure synchronization in tuning to new band and activating TX for
   * WTR hopping CFS. Otherwise for CFS, RX will not wait for any good frames
   * before sending good frames report to CP and TX may get enabled before
   * tune to new band is done leading to crash */
  txc_tx.tx_on_before_cfs = FALSE;

} /* txc_exit */

/*===========================================================================

FUNCTION TXC_CDMA

DESCRIPTION
  CDMA state of tx state machine.

DEPENDENCIES
  None
RETURN VALUE
  Next state to process.
SIDE EFFECTS
  None
===========================================================================*/
txc_state_type txc_cdma
(
  txc_state_type  state                  /* current txc_state - TXC_CDMA_S */
)
{
  txc_cmd_type *cmd_ptr;              /* command from queue */
  rex_sigs_type sigs = 0;

  do
  {
    (void)rex_clr_sigs( TX_TCB_PTR, TXC_CMD_Q_SIG );  /* clear queued signal */

    /* get all items off the queue */
    while ( (cmd_ptr = (txc_cmd_type*) q_get( &txc_cmd_q )) != NULL ) {

      /* interpret the command from the queue */
      switch ( cmd_ptr->hdr.command )
      {
        case TXC_CDMA_F:
          M1X_MSG( MUX, LEGACY_HIGH,
            "Warning - TXC_CDMA_F received in TXC_CDMA_S");
          txc_cmd_done( cmd_ptr, TXC_DONE_S);
          break;

        case TXC_PC_F:            /* switch to paging/access state */
          txc_access_idl_init( state );
          state = TXC_ACC_IDLE_S;
          txc_cmd_done( cmd_ptr, TXC_DONE_S);
          break;

        case TXC_EXIT_F:
          /* exit cdma mode */
          txc_exit();  /* perform exit processing */
          state = TXC_EXIT_S;
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
          break;
        case TXC_TT_F:
          txc_tt_init();
          state = TXC_ACC_JUMP_S;
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
          break;
        default:                /* bad command */
          /* dispose of command buffer */
          txc_cmd_done( cmd_ptr, TXC_BADCMD_S );
          break;

      }  /* switch */
    } /* Items off of queue */

    /* Handle mux msgr signal */
    if ( sigs & TXC_MUXMSGR_Q_SIG )
    {
      /* process and clear the muxmsgr signal */
      muxmsgr_process_rcv_msg(TX_TASK);
    }

    /* wait for command on the queue */
    if ( state == TXC_CDMA_S ) {
       sigs = txc_wait( TXC_CMD_Q_SIG | TXC_MUXMSGR_Q_SIG );
    }

  } while( state == TXC_CDMA_S ); /* until done with this state */

  return( state );

} /* txc_cdma */


/***************************************************************************/
/*                         Switch Between States                           */
/***************************************************************************/

/*===========================================================================

FUNCTION TXC_POWERUP_INIT

DESCRIPTION
  Initializes timers and queues for CDMA Transmit task processing.

DEPENDENCIES
  None.
RETURN VALUE
  Never returns.
SIDE EFFECTS
  None.
===========================================================================*/
void txc_powerup_init( void )
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize all the msg queues and buffers */
  (void)q_init( &txc_cmd_q );

  /* Initialize RLP frames logging buffer */
  dsrlp_log_tx_init_frm_buf();

  /* Initialize txc_log_mux_buf */
  txc_log_mux_buf.count = 0;
  txc_log_mux_buf.entry.hdr.len = FSIZ(mux_log_r_mux1_type, hdr);

  /* Initialize Tx RF state */
  txc_tx.rf_state = TXC_RF_STATE_DISABLE;

  /* -----------------------------------
  ** No TXC_FRAME_OFFSET command pending
  ** ----------------------------------- */
  txc_frame_offset_cmd.pending = FALSE;

  /* Perform DAL software switches initialization */
  cdma2kdiag_switch_init();

  mux_nv_efs_init();

} /* txc_powerup_init */

/*===========================================================================

FUNCTION TXC_SUBTASK

DESCRIPTION
  Transmit subtask to process CDMA.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void txc_subtask (void )
{
  txc_cmd_type *cmd_ptr;          /* pointer to command from txc_cmd_q */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  (void)rex_clr_sigs(TX_TCB_PTR, TXC_CMD_Q_SIG );

  while( (cmd_ptr = (txc_cmd_type*) q_get(&txc_cmd_q)) != NULL )
  {
    /* acknowledge the command */
    cmd_ptr->hdr.status = TXC_DONE_S;

    if( cmd_ptr->hdr.command != TXC_CDMA_F )
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Warning: Ignoring invalid command (%d) for TXC inactive state.",
        cmd_ptr->hdr.command );
      cmd_done( &cmd_ptr->hdr.cmd_hdr );
    }
    else
    {
      /* Begin CDMA subtask processing */
      txc_state = TXC_CDMA_S;           /* initialize state machine state */

      /* Before sending done_s to MC, re-read all the Tx EFS items.
       * This is required because 1x_ASID might have changed 
       * due to MCFG Refresh scenarios */
      mux_nv_efs_init();

      cmd_done( &cmd_ptr->hdr.cmd_hdr );

      /* main state machine loop */
      do
      {
        switch ( txc_state )
        {
          case TXC_CDMA_S:             /* to cdma state */
            txc_update_sp_state( SP_TECH_INACTIVE_STATE );
            txc_state = txc_cdma( txc_state );
            break;
          case TXC_ACC_IDLE_S:         /* access channel idle */
            txc_update_sp_state( SP_TECH_IDLE_STATE );
            txc_state = txc_access_idl( txc_state );
            break;
          case TXC_ACC_JUMP_S:         /* access channel jump to hyperspace */
            txc_state = txc_hyperspace( txc_state );
            break;
          case TXC_TT_S:        /* Handle the timing transfer from DO to 1X */
            txc_state = txc_tt_state( txc_state );
            break;
          case TXC_ACC_ATT_S:            /* Access attempt transmission */
            muxmdsp_configure_rl_rc( CAI_INIT_RC );

            txc_update_sp_state( SP_TECH_ACCESS_STATE );
            txc_state = txc_access_att(txc_state);

            muxmdsp_configure_rl_rc(CAI_INIT_RC);
            break;
          case TXC_TRAFFIC_S:          /* traffic channel */
            if(txc_so.rev_dcch.included)
            {
              muxmdsp_configure_rl_rc(txc_so.rev_dcch.rc);
            }
            if(txc_so.rev_fch.included)
            {
              /* Send RC signalling message to FW here */
              muxmdsp_configure_rl_rc(txc_so.rev_fch.rc);
            }

            txc_update_sp_state( SP_TECH_TRAFFIC_STATE );
            txc_state = txc_traffic(txc_state);

            muxmdsp_configure_rl_rc(CAI_INIT_RC);
            break;

          case TXC_HOLD_ENC_CLK_S: /* Leave Encoder clocks ON for now */
            txc_state = txc_hold_enc_clk (txc_state);
            break;

          default:                    /* should never get here */
            M1X_MSG( MUX, LEGACY_ERROR,
              "Illegal TX state %d",
              txc_state );
            txc_state = TXC_EXIT_S;
            break;
        } /* switch */
      } while ( txc_state != TXC_EXIT_S );  /* never exit this loop... */
    } /* if (cmd_ptr->hdr.command != TXC_CDMA_F ) */
  } /* while( (cmd_ptr = (txc_cmd_type*) q_get(&txc_cmd_q)) != NULL ) */

}  /* txc_subtask */

/***************************************************************************/
/*              Interface for the other tasks to call TX                   */
/***************************************************************************/

/*===========================================================================

FUNCTION TXC_CMD

DESCRIPTION
  This is how the outside world interfaces with the tx task.  Call this
  procedure with the address of the txc_cmd_type and the command will be
  placed on the txc_cmd_q

DEPENDENCIES
  None.
RETURN VALUE
  None.
SIDE EFFECTS
  None.
===========================================================================*/
void txc_cmd
(
  txc_cmd_type *cmd_ptr                    /* the command to queue up */
)
{
  TXC_ENTER_ATOMIC_SECTION();
  (void) q_link( cmd_ptr, &cmd_ptr->hdr.cmd_hdr.link );  /* init link */
  cmd_ptr->hdr.status = TXC_BUSY_S;        /* mark it as queued */

  q_put( &txc_cmd_q, &cmd_ptr->hdr.cmd_hdr.link ); /* and queue it */

  (void)rex_set_sigs( TX_TCB_PTR, TXC_CMD_Q_SIG ); /* signal the transmit task */
  TXC_LEAVE_ATOMIC_SECTION();
} /* txc_cmd */

/*===========================================================================

FUNCTION TXC_SET_FRAME_OFFSET

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_set_frame_offset ( byte frame_offset )
{
  /* start computing frame number from sys time in enc isr */
  after_fo_handoff_count_isrs = NO_OF_ISRS_AFTER_FO;

  txc_frame_offset = frame_offset;

} /* txc_set_frame_offset */

/*===========================================================================

FUNCTION TXC_GET_TIME_IN_1_25MS_UNIT

DESCRIPTION
  This function get the current time in 1.25 ms unit and factor in the
  frame offset as well

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
dword txc_get_time_in_1_25ms_unit( void )
{
  qword ts_time;

  (void) m1x_time_get( ts_time );
  (void) qw_div_by_power_of_2( ts_time, ts_time, 16 );
  qw_dec( ts_time, ( dword ) txc_frame_offset );
  qw_inc( ts_time, 1L );

  return( qw_lo( ts_time ) );

} /* txc_get_time_in_1_25ms_unit */

/*===========================================================================

FUNCTION TXC_FLAG_NEXT_INT

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_flag_next_int ( void )
{
  txc_next_int_occurred = FALSE;
} /* txc_flag_next_int */

/*===========================================================================

FUNCTION TXC_INT_OCCURRED

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean txc_int_occurred ( void )
{
  return( txc_next_int_occurred );
} /* txc_int_occurred */

/*===========================================================================

FUNCTION TXC_GET_REV_LINK_RATE_SET

DESCRIPTION
  Returns the rate set for a revese link channel given the current RC of
  that channel.

DEPENDENCIES
  None

RETURN VALUE
  The rate set corresponding to the provide radio configuration.

SIDE EFFECTS
  None
===========================================================================*/
enc_rate_set_type txc_get_rev_link_rate_set ( cai_radio_config_type rc )
{
  enc_rate_set_type rate_set;

  switch(rc)
  {
    case CAI_RC_1:
    case CAI_RC_3:

    #ifdef FEATURE_IS2000_1X_ADV
    case CAI_RC_8:
    #endif /* FEATURE_IS2000_1X_ADV */

      rate_set = ENC_RATE_9600;
      break;

    case CAI_RC_2:
    case CAI_RC_4:
      rate_set = ENC_RATE_14400;
      break;

    default:
      rate_set = ENC_RATE_INVALID;
      M1X_MSG( MUX, LEGACY_ERROR,
        "Invalid RL RC (%d) for Rate Set Conversion.",
        rc);
  }

  return rate_set;
} /* txc_get_rev_link_rate_set */

/*===========================================================================

FUNCTION TXC_GET_FCH_RATE_SET

DESCRIPTION
  Returns the rate set for FCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
enc_rate_set_type txc_get_fch_rate_set ( void )
{
  return txc_get_rev_link_rate_set(txc_so.rev_fch.rc);
} /* txc_get_fch_rate_set */

/*===========================================================================

FUNCTION TXC_GET_FCH_RC

DESCRIPTION
  Returns the radio configuration for FCH.

DEPENDENCIES
  None

RETURN VALUE
  The radio configuration for the current R-FCH.

SIDE EFFECTS
  None
===========================================================================*/
cai_radio_config_type txc_get_fch_rc ( void )
{
  return txc_so.rev_fch.rc;
} /* txc_get_fch_rc */

/*===========================================================================

FUNCTION TXC_GET_DCCH_RATE_SET

DESCRIPTION
  Returns the rate set for DCCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

enc_rate_set_type txc_get_dcch_rate_set ( void )
{
  return txc_get_rev_link_rate_set(txc_so.rev_dcch.rc);
} /* txc_get_dcch_rate_set */

/*===========================================================================

FUNCTION TXC_DCCH_IS_ON

DESCRIPTION
  Returns if DCCH is ON.

DEPENDENCIES
  None

RETURN VALUE
  TRUE: If DCCH is On
  FALSE: If DCCH is Off

SIDE EFFECTS
  None
===========================================================================*/
boolean txc_dcch_is_on( void )
{
  return txc_so.rev_dcch.included;
} /* txc_dcch_is_on */

/*===========================================================================

FUNCTION TXC_FCH_IS_ON

DESCRIPTION
  Returns if FCH is ON.

DEPENDENCIES
  None

RETURN VALUE
  TRUE: If FCH is On
  FALSE: If FCH is Off

SIDE EFFECTS
  None
===========================================================================*/
boolean txc_fch_is_on( void )
{
  return txc_so.rev_fch.included;
} /* txc_fch_is_on */

/*===========================================================================

FUNCTION TXC_SCH_IS_ON

DESCRIPTION
  Returns if SCH is ON.

DEPENDENCIES
  None

RETURN VALUE
  TRUE: If SCH is On
  FALSE: If SCH is Off

SIDE EFFECTS
  None
===========================================================================*/
boolean txc_sch_is_on( void )
{
  return txtc.sch_on;
} /* txc_sch_is_on */

/*===========================================================================

FUNCTION TXC_GET_REV_CHAN_GAIN

DESCRIPTION
  Returns the current gain value of the specified channel.

DEPENDENCIES
  None

RETURN VALUE
  Digital gain of the specified channel.

SIDE EFFECTS
  None
===========================================================================*/
uint16 txc_get_rev_chan_gain( txc_rl_traffic_chan_type channel )
{
  uint16 gain = 0;

  switch(channel)
  {
    case TXC_RL_PILOT:
      gain = channel_gains.txc_pch_gain;
      break;

    case TXC_RL_FCH:
      gain = channel_gains.txc_fch_gain;
      break;

    case TXC_RL_DCCH:
      gain = channel_gains.txc_dcch_gain;
      break;

    case TXC_RL_SCH:
      gain = channel_gains.txc_sch_gain;
      break;

    default:
      M1X_MSG( MUX, LEGACY_ERROR,
        "Invalid Reverse Link Channel Specified (%d)",
        channel);
  }

  return gain;
} /* txc_get_rev_chan_gain */

/*===========================================================================

FUNCTION TXC_GET_REV_SCH_RATE

DESCRIPTION
  Returns the current R-SCH rate.

DEPENDENCIES
  None

RETURN VALUE
  Rate of R-SCH.

SIDE EFFECTS
  None
===========================================================================*/
enc_sch_rate_type txc_get_rev_sch_rate( void )
{
  return txtc.supp_rate;
} /* txc_get_rev_sch_rate */

/*===========================================================================

FUNCTION TXC_SET_PCH_FCH_GAIN

DESCRIPTION
  Sets the gain values in txc_pch_fch_gain_tab for the specified rate

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void txc_set_pch_fch_gain
(
 enc_rate_type rate,
 txc_gain_type gain,
 int16         pch_adj_gain,
 txc_chan_status_type chan_status
)
{
   if (chan_status == TXC_FCH_ONLY)
   {
      txc_pch_fch_gain_tab [ rate ] = gain;
      txc_pch_adj_gain_tab [ rate ] = pch_adj_gain;
   }
   else if (chan_status == TXC_SCH_ACTIVE)
   {
      txc_gain_tab[ rate ] = gain;
      txc_sch_pch_adj_gain_tab[ rate ] = pch_adj_gain;
   }
   else if (chan_status == TXC_SCH_DTX)
   {
      txc_dtx_gain_tab[ rate ] = gain;
      txc_dtx_pch_adj_gain_tab [ rate ] = pch_adj_gain;
   }
} /* txc_set_pch_fch_gain */

#ifdef FEATURE_IS2000_1X_ADV
/*===========================================================================

FUNCTION TXC_SET_PCH_FCH_SCH_GAIN_RATIOS

DESCRIPTION
  Save the channel to pilot linear gain ratios

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_set_pch_fch_sch_ratios
(
  enc_rate_type rate,
  txc_linear_gain_ratio_type ratio,
  txc_chan_status_type chan_status,
  txc_active_cell_size_types acs
)
{
  if (chan_status == TXC_FCH_ONLY)
  {
    txc_fch_ack_gain_ratio_tab [ rate ][ acs ] = ratio;
  }
  else if ( chan_status == TXC_SCH_ACTIVE )
  {
    txc_fch_sch_ack_gain_ratio_tab [ rate ][ acs ] = ratio;
  }
  else if ( chan_status == TXC_SCH_DTX )
  {
    txc_fch_schdtx_ack_gain_ratio_tab[ rate ][ acs ] = ratio;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Invalid rate or cell size: rate=%d, acs=%d",
      rate,
      acs);
  }
} /* txc_set_pch_fch_sch_ratios */
#endif /* FEATURE_IS2000_1X_ADV */

/*===========================================================================

FUNCTION TXC_SET_FPC_MODE

DESCRIPTION
  Sets the value of FPC_MODE in txc_so.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void txc_set_fpc_mode ( fpc_mode_type fpc_mode )
{
   #ifdef FEATURE_MODEM_1X_DRX
   if( txc_so.fpc_mode != fpc_mode )
   {
      rxc_fpc_mode_changed = TRUE;

      M1X_MSG( MUX, LEGACY_HIGH,
         "Previous FPC mode = %d, Current FPC mode = %d",
         txc_so.fpc_mode, 
         fpc_mode );
   }
   #endif /* FEATURE_MODEM_1X_DRX */

   /* save current FPC_MODE value */
   txc_so.fpc_mode = fpc_mode;

   switch (txc_so.fpc_mode)
   {


     case FPC_MODE_PRI_EIB:
#ifdef FEATURE_IS2000_REL_A
     case FPC_MODE_PRI_QIB:
     case FPC_MODE_PRI_QIB_SEC_EIB:
#endif /* FEATURE_IS2000_REL_A */
       /* Setting MOD_PCBIT_TEST_MASK for MOD_PCBIT_TEST_DATA register.
       ** enable all bits for FPC_MODE_EIB.
       */
       enc_pcbit_test_mask (0xFFFF);
       break;

#ifdef FEATURE_IS2000_REL_A
     case FPC_MODE_PRI_400Hz_SEC_EIB:
       /* Setting MOD_PCBIT_TEST_MASK for MOD_PCBIT_TEST_DATA register.
       ** enable half of bits for EIB.
       */
       enc_pcbit_test_mask (0xaaaa);
       break;
#endif /* FEATURE_IS2000_REL_A */

#ifdef FEATURE_IS2000_1X_ADV
     case FPC_MODE_1XA_PRI_400Hz:
     case FPC_MODE_1XA_PRI_200Hz:
     case FPC_MODE_1XA_PRI_200Hz_SEC_200Hz:
#endif /* FEATURE_IS2000_1X_ADV */
     case FPC_MODE_PRI_800Hz:
     case FPC_MODE_PRI_400Hz_SEC_400Hz:
     case FPC_MODE_PRI_200Hz_SEC_600Hz:
     case FPC_MODE_FFPC_DISABLED:
       /* Disabling MOD_PCBIT_TEST_MASK. PC bits generated by DEMOD.
       */
       enc_pcbit_test_mask (0x0000);
       break;

     default:
       M1X_MSG( MUX, LEGACY_ERROR,
         "Unknown FPC_MODE %d",
         fpc_mode );
   }

} /* txc_set_fpc_mode */


/*===========================================================================

FUNCTION TXC_GET_SCH_INFO

DESCRIPTION This function returns current configuration information for
            the SCHs. This is used by the UI in the data display.

            PARAMETERS:
            sch_rc -       gives the Radio configuration of the R-SCH
            sch_rate_ptr - points to the current SCH rate.
            double_ptr   - is TRUE when double sized PDUs are being used
                           is FALSE when single sized PDUs are being used

            Note, the above two pointer return values are only meaningful
            when SCH is assigned

DEPENDENCIES None

RETURN VALUE TRUE:  If SCH are assigned.
             FALSE: If no SCH are assigned.

SIDE EFFECTS None

===========================================================================*/

boolean txc_get_sch_info
(
  enc_sch_radio_config_type* sch_rc,                      /* ptr to SCH RC */
  enc_sch_rate_type* sch_rate_ptr,                   /* ptr to rate of SCH */
  boolean* double_ptr                   /* Ptr to single/double sized PDUs */
)
{
  if( txtc.sch_on == TRUE)
  {
    *sch_rate_ptr = txtc.supp_rate;
    *double_ptr   = txtc.double_sized;
    *sch_rc       = (enc_sch_radio_config_type) txtc.r_sch_rc;
    return( TRUE);
  }
  else
  {
    return( FALSE);
  }
} /* txc_get_sch_info() */


/*===========================================================================

FUNCTION     TXC_UPDATE_CHAN_CONFIG_TO_RF

DESCRIPTION  This function is called by MUX to update the actual channel config
             to RF only if the channel configuration has changed.

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

===========================================================================*/
void txc_update_chan_config_to_rf ( boolean  valid_config )
{
  static txc_rfm_1x_txc_chan_config_type prev_chan_config;
  txc_rfm_1x_txc_chan_config_type        updated_chan_config = { FALSE };

  if ( valid_config )
  {
    /* Intialize the chan config based on the MUX current setup */
    updated_chan_config.fch_on    = txc_so.rev_fch.included;
    updated_chan_config.fch_rc    = txc_so.rev_fch.rc;
    updated_chan_config.dcch_on   = txc_so.rev_dcch.included;
    updated_chan_config.dcch_rc   = txc_so.rev_dcch.rc;

    if ( txtc.sch_on )
    {
      int16 sch2pch;

      /* Update the SCH config */
      updated_chan_config.sch_on    = TRUE;
      updated_chan_config.sch_rc    = txtc.r_sch_rc;
      updated_chan_config.sch_rate  = txtc.supp_rate;

      /* Get the SCH-TO-PCH ratio */
      if ( txtc.supp_turbo )
      {
        sch2pch = rpc_get_sch2pich_gain_ratio ( txtc.supp_rate, CAI_TURBO_CODING );
      }
      else
      {
        sch2pch = rpc_get_sch2pich_gain_ratio ( txtc.supp_rate, CAI_CONVOLUTIONAL_CODING );
      }

      /* Check of the SCH to PCH ratio is bigger than 6 dB*/
      if (sch2pch > 48) /* SCH2PCH in the unit of 1/8 Db, so 6*8 = 48 */
      {
        updated_chan_config.sch_to_pch_over_6dB = TRUE;
      }
    }
  }

  if( memcmp( &prev_chan_config,
              &updated_chan_config,
              sizeof(txc_rfm_1x_txc_chan_config_type) ) != 0 )
  {
    #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
    rfm_1x_notify_tx_config( txc_get_tx_device(), FALSE, &updated_chan_config );
    #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */
  }

  prev_chan_config = updated_chan_config;

} /* txc_update_chan_config_to_rf */

#ifdef FEATURE_MODEM_1X_SUPPORTS_RF
/*===========================================================================

FUNCTION      TXC_SET_TX_DEVICE

DESCRIPTION   Sets the RF Tx device in use.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
static void txc_set_tx_device( rfm_device_enum_type tx_device )
{
  M1X_MSG( MUX, LEGACY_MED,
    "Tx device: %d",
    tx_device );

  /* Store the Tx device currently in use */
  onex_tx_device = tx_device;

} /* txc_set_tx_device */
#endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

/*===========================================================================

FUNCTION      TXC_GET_TX_DEVICE

DESCRIPTION   Returns the RF Tx device in use.

DEPENDENCIES  None

RETURN VALUE  The RF Tx device.

SIDE EFFECTS  None

===========================================================================*/
rfm_device_enum_type txc_get_tx_device( void )
{
  /* return the Tx device currently in use */
  return ((rfm_device_enum_type)onex_tx_device);
} /* txc_get_tx_device */

/*===========================================================================

FUNCTION TXC_GET_PA_STATE

DESCRIPTION
  Returns PA state if TX is ON.

DEPENDENCIES
  None.

RETURN VALUE
  txc_pa_gain_state_enum_type

SIDE EFFECTS
  None
===========================================================================*/
txc_pa_gain_state_enum_type txc_get_pa_state( void )
{
  txc_pa_gain_state_enum_type pa_state = TXC_PA_GAIN_STATE_TX_OFF;

  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  rfm_1x_tx_agc_state_param_type tx_state_params;
  rfm_cdma_api_status_type rf_api_status;

  if( txc_tx.tx_on == TRUE )
  {
    rf_api_status = rfm_1x_get_tx_agc_state_parameter( txc_get_tx_device(),
                                                       &tx_state_params );
    if( rf_api_status == RFM_CDMA_HEALTHY_STATE )
    {
      switch( tx_state_params.pa_state )
      {
        case RFM_PA_GAIN_STATE_0:
          pa_state = TXC_PA_GAIN_STATE_0;
          break;

        case RFM_PA_GAIN_STATE_1:
          pa_state = TXC_PA_GAIN_STATE_1;
          break;

        case RFM_PA_GAIN_STATE_2:
          pa_state = TXC_PA_GAIN_STATE_2;
          break;

        case RFM_PA_GAIN_STATE_3:
          pa_state = TXC_PA_GAIN_STATE_3;
          break;

        default:
          pa_state = TXC_PA_GAIN_STATE_TX_OFF;
          M1X_MSG( MUX, LEGACY_ERROR,
            "RF API returned undefined PA Gain State(%d)",
            tx_state_params.pa_state );
          break;
      }
    }
    else
    {
        M1X_MSG( MUX, LEGACY_ERROR,
          "RF API returned Error(%d)",
          rf_api_status );
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_MED,
      "TX is OFF" );
  }
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

  M1X_MSG( MUX, LEGACY_MED,
    "Returning pa state(%d)",
    pa_state );

  return( pa_state );

} /* txc_get_pa_state */

/*===========================================================================

FUNCTION TXC_GET_FILTERED_TX_POWER_DBM10

DESCRIPTION
  Returns Filtered TX Power in dBm10 if TX is ON for the given RF device.

DEPENDENCIES
  None.

RETURN VALUE
  Filtered Tx power value in dBm10

SIDE EFFECTS
  None
===========================================================================*/
int16 txc_get_filtered_tx_power_dbm10( void )
{
  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  return( ( txc_tx.tx_on ) ?
    rfm_1x_get_filtered_tx_power( txc_get_tx_device() ) : -600 );
  #else
  return( -600 );
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */
} /* txc_get_filtered_tx_power_dbm10 */

/*===========================================================================

FUNCTION TXC_GET_TX_AGC_DBM10

DESCRIPTION
  Returns TX AGC in 1/10th dBm if TX is ON for the given RF device.

DEPENDENCIES
  None.

RETURN VALUE
  Tx ON  : Total 1x transmit power in 1/10th dBm and 0x8000 on failure
  Tx OFF : 0x8000 = -32768

SIDE EFFECTS
  None
===========================================================================*/
int16 txc_get_tx_agc_dbm10( void )
{
  int16 tx_agc_dbm10 = TX_IS_OFF;

  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  if( txc_tx.tx_on == TRUE )
  {
    tx_agc_dbm10 = rfm_1x_get_tx_agc( txc_get_tx_device() );
    if( tx_agc_dbm10 == (int16)0x8000 )
    {
      M1X_MSG( MUX, LEGACY_MED,
        "RF returned ERROR, %d",
        tx_agc_dbm10 );
    }
  }
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

  return( tx_agc_dbm10 );

} /* txc_get_tx_agc_dbm10 */

/*===========================================================================

FUNCTION TXC_GET_TX_AGC_IN_LEGACY_UNITS_DBM12

DESCRIPTION
  Converts TX AGC to 1/12th dBm to match legacy units.

DEPENDENCIES
  None.

RETURN VALUE
  Tx AGC in 1/12th dBm

SIDE EFFECTS
  None
===========================================================================*/
int16 txc_get_tx_agc_in_legacy_units_dbm12( void )
{
  int16 legacy_value_dbm12;
  /* Convert new tx power units(1/10th dBm) to old tx power units(1/12th dBm)

  We know:
          1. dBm = -52.25 + ( old_txagc_units / 12 )
          2. dBm = new_txagc_units / 10

  Setting these equal gives:
          -52.25 + ( old_txagc_units / 12 ) = new_txagc_units / 10

  Solving for old_txagc_units in terms of new_txagc_units, using purely
  integer math, gives:
          old_txagc_units = ( ( new_txagc_units * 6 ) + 3135 ) / 5
  */
  if( txc_get_tx_agc_dbm10() <= -523 )
  {
    legacy_value_dbm12 = 0;
  }
  else
  {
    legacy_value_dbm12 = ((txc_get_tx_agc_dbm10()*6)+3135)/5;
  }

  return( legacy_value_dbm12 );

} /* txc_get_tx_agc_in_legacy_units_dbm12 */

/*===========================================================================

FUNCTION TXC_GET_MAX_TX_AGC_DBM12

DESCRIPTION
  Returns MAX TX AGC in dBm12 for the given RF device.

DEPENDENCIES
  None.

RETURN VALUE
  Max 1x transmit power in dBm12 and 0 dBm12 (-52.33 dBm) on failure

SIDE EFFECTS
  None
===========================================================================*/
int16 txc_get_max_tx_agc_dbm12( void )
{
  int16 max_tx_agc_dbm12 = 0; /* -52.33 dBm */

  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  if( txc_tx.tx_on )
  {
  max_tx_agc_dbm12 = rfm_1x_get_tx_pwr_limit( txc_get_tx_device() );
  }
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

  return( max_tx_agc_dbm12 );

} /* txc_get_max_tx_agc_dbm12 */

/*===========================================================================

FUNCTION TXC_GET_MAX_TX_AGC_DBM10

DESCRIPTION
  Returns MAX TX AGC in dBm10 for the given RF device.

DEPENDENCIES
  None.

RETURN VALUE
  Max 1x transmit power in dBm10

SIDE EFFECTS
  None
===========================================================================*/
int16 txc_get_max_tx_agc_dbm10( void )
{
  /* Convert max tx power (dBm12) to dBm10

        max_tx_agc_in_dbm = ( max_tx_agc_dbm12 / 12 ) - 52.33
        max_tx_agc_in_dbm = ( max_tx_agc_dbm12 - 628 ) / 12

     We know,
        max_tx_agc_in_dbm10 = max_tx_agc_in_dbm * 10

     Hence
        max_tx_agc_in_dbm10 = ( ( max_tx_agc_dbm12 - 628 ) / 12 ) * 10
  */
  return( ( ( txc_get_max_tx_agc_dbm12() - 628 ) / 12 ) * 10 );

} /* txc_get_max_tx_agc_dbm10 */

/*===========================================================================

FUNCTION TXC_GET_TX_AGC_DBM2

DESCRIPTION
  Returns TX AGC in 1/2th dBm if TX is ON for the given RF device.

DEPENDENCIES
  None.

RETURN VALUE
  Tx ON  : Total 1x transmit power in 1/2th dBm and 0x80 on failure
  Tx OFF : 0x80 = -128

SIDE EFFECTS
  None
===========================================================================*/
int8 txc_get_tx_agc_dbm2( void )
{
  int16 tx_agc_dbm10 ;
  int8  tx_agc_dbm2 = TX_IS_OFF_Q8;

  tx_agc_dbm10 = txc_get_tx_agc_dbm10();

  if( tx_agc_dbm10 != (int16)0x8000 )
  {
    if( tx_agc_dbm10 < 0 )
    {
      tx_agc_dbm2 = (int8)( ( ( tx_agc_dbm10*2 ) - 5 ) / 10 );
    }
    else
    {
      tx_agc_dbm2 = (int8)( ( ( tx_agc_dbm10*2 ) + 5 ) / 10 );
    }
  }

  return( tx_agc_dbm2 );
}

#ifdef FEATURE_MODEM_1X_SUPPORTS_RF
/*===========================================================================

FUNCTION      TXC_GET_TXLM_CLIENT

DESCRIPTION   This function translates the Tx device into the proper
               TxLM (TX Layer Manager) chain enum type.

DEPENDENCIES  None

RETURN VALUE  TxLM chain enum

SIDE EFFECTS  None

===========================================================================*/
static txlm_chain_type txc_get_txlm_client
(
  rfm_device_enum_type       tx_device       /* RF Tx device type */
)
{
  txlm_chain_type chain = TXLM_CHAIN_0;

  /* NOTE: This is a legacy param passed to TXLM allocate/deallocate APIs.
     RF doesn't use this at all. Hence, passing a dummy value for now. */
  chain = (txlm_chain_type) tx_device;

  return( chain );

} /* rx_get_rxlm_client */

#else /* !FEATURE_MODEM_1X_SUPPORTS_RF */
/*===========================================================================

FUNCTION      TXC_SEND_FW_TX_STOP

DESCRIPTION   When RF support is not available (such as on RUMI) use this
              function to issue a TX_STOP command to FW in place of RF doing so.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
void txc_send_fw_tx_stop( void )
{
  cfw_tx_stop_msg_t tx_stop;

  tx_stop.tx_chain = 0;
  tx_stop.txlm_buffer_idx = 0;
  tx_stop.rf_script_idx = -1;

  M1X_MSG( MUX, LEGACY_HIGH,
    "RUMI: Sending fake CDMA_FW_TX_STOP_CMD");

  /* Register for a response */
  muxmsgr_register_msg( TX_TASK, CDMA_FW_TX_STOP_RSP );

  muxmsgr_set_stop_rsp_flag();

  muxmsgr_send_msg( TX_TASK, &tx_stop.hdr,
                    CDMA_FW_TX_STOP_CMD,
                    sizeof( tx_stop ) );

  M1X_MSG( MUX, LEGACY_HIGH,
    "RUMI: Waiting for CDMA_FW_TX_STOP_RSP");

  muxmsgr_wait_on_stop_rsp();

  /* Deregister from the message */
  muxmsgr_deregister_msg( TX_TASK, CDMA_FW_TX_STOP_RSP );

} /* txc_send_fw_tx_stop */

/*===========================================================================

FUNCTION      TXC_SEND_FW_TX_START

DESCRIPTION   When RF support is not available (such as on RUMI) use this
              function to issue a TX_START command to FW in place of RF doing so.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
void txc_send_fw_tx_start( void )
{
  cfw_tx_start_msg_t        tx_start = { { 0 } };

  tx_start.rf_script_idx = -1;

  M1X_MSG( MUX, LEGACY_HIGH,
    "RUMI: Sending fake CDMA_FW_TX_START_CMD");

  #ifdef FEATURE_MODEM_1X_UTX
  /* Since RF support is not available, manually set TX_MODE_COMMON */
  HWIO_OUT( TX_MODE_COMMON, 0x7C );
  #endif /* FEATURE_MODEM_1X_UTX */

  /* Register for a response */
  muxmsgr_register_msg( TX_TASK, CDMA_FW_TX_START_RSP );

  muxmsgr_set_start_rsp_flag();

  muxmsgr_send_msg( TX_TASK, &tx_start.hdr,
                    CDMA_FW_TX_START_CMD,
                    sizeof( tx_start ) );

  M1X_MSG( MUX, LEGACY_HIGH,
    "RUMI: Waiting for CDMA_FW_TX_START_RSP");

  muxmsgr_wait_on_start_rsp();

  /* Deregister from the message */
  muxmsgr_deregister_msg( TX_TASK, CDMA_FW_TX_START_RSP );

} /* txc_send_fw_tx_start */
#endif /* !FEATURE_MODEM_1X_SUPPORTS_RF */

/*===========================================================================

FUNCTION      TXC_CHANGE_RF_TX_STATE

DESCRIPTION   This function handles changing the transmitter state
              (enabled/disabled/started/stopped) using RF APIs and safeguards
              against calling these RF APIs redundantly.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS

              is_enabled  is_stopped  requested state      |   result
              ------------------------------------------------------------
                  0           0      TXC_RF_STATE_ENABLE   |   TX Enabled
                  0           0      TXC_RF_STATE_DISABLE  |   No Change
                  0           0      TXC_RF_STATE_START    |   No Change
                  0           0      TXC_RF_STATE_STOP     |   No Change
                                                           |
                  0           1      TXC_RF_STATE_ENABLE   |   INVALID
                  0           1      TXC_RF_STATE_DISABLE  |   INVALID
                  0           1      TXC_RF_STATE_START    |   INVALID
                  0           1      TXC_RF_STATE_STOP     |   INVALID
                                                           |
                  1           0      TXC_RF_STATE_ENABLE   |   No Change
                  1           0      TXC_RF_STATE_DISABLE  |   TX Disabled
                  1           0      TXC_RF_STATE_START    |   No Change
                  1           0      TXC_RF_STATE_STOP     |   TX Stopped
                                                           |
                  1           1      TXC_RF_STATE_ENABLE   |   No Change
                  1           1      TXC_RF_STATE_DISABLE  |   TX Disabled
                  1           1      TXC_RF_STATE_START    |   TX Started
                  1           1      TXC_RF_STATE_STOP     |   No Change


              NOTE: is_stopped will be cleared by default whenever RF TX
                    transitions to enabled or disabled states.

              -------------------------------------------------------------

===========================================================================*/
void txc_change_rf_tx_state( txc_rf_state_type state )
{
  static boolean is_enabled = FALSE;
  static boolean is_stopped = FALSE;

  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  rfm_wait_time_t rf_ret_val = RFM_CDMA_ERROR_FAILURE;
  lm_status_type  lm_status  = LM_BUFFER_NOT_ALLOCATED;
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

  switch(state)
  {
    case TXC_RF_STATE_DISABLE:

      if(is_enabled)
      {
        /* Tear down the encoder/modulator/DAC prior to disabling RF */
        enc_set_mode( ENC_SLEEP_MODE );

        #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
        M1X_MSG( MUX, LEGACY_MED,
          "Calling RF_API - rfm_1x_sleep_tx( %u )",
          txc_get_tx_device() );

        /* Disable RF */
        /* notify the TX SM that tx is going to sleep */
        srch_rx_tx_sleep();
        rf_ret_val = rfm_1x_sleep_tx( txc_get_tx_device(),
                                      onex_tx_lm_handle, NULL, NULL );
        if( rf_ret_val < RFM_CDMA_HEALTHY_STATE )
        {
          ONEX_ERR_FATAL( "RF sleep TX failed. ret_value=%d", rf_ret_val, 0, 0 );
        }
        else if ( rf_ret_val > RFM_CDMA_HEALTHY_STATE )
        {
          /*
          This case is not currently handled. The RF code only returns 0 or
          negative value for the sleep_tx function and the rest of this code
          is dependant on sleep_tx being done. The callback is not used
          (passed as NULL) as RF returns the callback immediately and not
          asynchronously. So this could lead to race conditions in cases
          where sleep_tx takes some time to actually disable TX.
          */
        }

        txc_tx.tx_on = FALSE;

        M1X_MSG(MUX, LEGACY_MED,
          "txlm_deallocate_buffer");
        lm_status = txlm_deallocate_buffer( onex_tx_lm_handle );

        if ( lm_status != LM_SUCCESS )
        {
          /* Handle error - Print out error message */
          M1X_MSG( MUX, LEGACY_ERROR,
            "TXLM buffer deallocate error %u for 1X tech",
            (uint32)lm_status);
        }

        /* Clear the variable for Tx device currently in use */
        txc_set_tx_device( RFM_INVALID_DEVICE );

        /* TxLm has been deallocated, update the API */
        srch_mux_set_txlm_handle( onex_tx_lm_handle );

        #else /* !FEATURE_MODEM_1X_SUPPORTS_RF */
        txc_send_fw_tx_stop();
        #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

        #ifdef FEATURE_MODEM_1X_COEXISTENCE
        /* shutting down TX, update the coex manger about it */
        srch_coex_send_frequency_indication( COEX_RX_SRCH_TRAFFIC ,
                                             COEX_TX_ACTIVITY_NONE );
        #endif /* FEATURE_MODEM_1X_COEXISTENCE */

        #ifdef FEATURE_MODEM_1X_SRCH_ASD
        (void) rex_clr_sigs ( TX_TCB_PTR, TXC_ASDIV_WAIT_SIG );
        /* Inform ASD about stopping of TX */
        srch_rx_div_asd_stop_tx_mode();
        txc_wait( TXC_ASDIV_WAIT_SIG );
        (void) rex_clr_sigs ( TX_TCB_PTR, TXC_ASDIV_WAIT_SIG );
        #endif /* FEATURE_MODEM_1X_SRCH_ASD */

        txc_tx.rf_state = TXC_RF_STATE_DISABLE;
        is_enabled = FALSE;
        is_stopped = FALSE;
        M1X_MSG( MUX, LEGACY_MED,
          "Disable RF TX");
      }
      break;

    case TXC_RF_STATE_ENABLE:

      if(!is_enabled)
      {
        #ifdef FEATURE_MODEM_1X_SRCH_ASD
        (void) rex_clr_sigs ( TX_TCB_PTR, TXC_ASDIV_WAIT_SIG );
        /* Inform ASD about starting of TX */
        srch_rx_div_asd_start_tx_mode();
        txc_wait( TXC_ASDIV_WAIT_SIG );
        (void) rex_clr_sigs ( TX_TCB_PTR, TXC_ASDIV_WAIT_SIG );
        #endif /* FEATURE_MODEM_1X_SRCH_ASD */

        #ifdef FEATURE_MODEM_1X_COEXISTENCE
        /* Turning ON TX, update the coex manger about it */
        srch_coex_send_frequency_indication( COEX_RX_SRCH_TRAFFIC ,
                                             COEX_TX_FRAME_TRANSMIT );
        #endif /* FEATURE_MODEM_1X_COEXISTENCE */

        #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
        rfm_device_enum_type rx_dev;  /* rx rf dev */

        /* Get and store the Tx device currently in use */
        txc_set_tx_device( srch_mux_get_tx_device() );

        M1X_MSG( MUX, LEGACY_MED,
          "txlm_allocate_buffer");

        lm_status = txlm_allocate_buffer(
                                  txc_get_txlm_client( txc_get_tx_device() ),
                                  LM_1X,
                                  &(onex_tx_lm_handle) );

        if ( lm_status != LM_SUCCESS )
        {
          /* Error fatal if error - can't gracefully recover from
             this situation */
          ONEX_ERR_FATAL("TXLM buffer allocate error %d for 1X tech",
                                                (uint32)lm_status, 0, 0);
        }

        rx_dev = srch_mux_get_rx_device(); /* get rx device */

        M1X_MSG( MUX, LEGACY_MED,
          "Calling RF_API - rfm_1x_wakeup_tx_v2( TX %u, RX %u )",
          txc_get_tx_device(), rx_dev );

        /* Enable TX */
        rf_ret_val = rfm_1x_wakeup_tx_v2( txc_get_tx_device(),
                                          rx_dev, onex_tx_lm_handle, NULL, NULL );
        if( rf_ret_val < RFM_CDMA_HEALTHY_STATE )
        {
          ONEX_ERR_FATAL( "RF wakeup TX failed. ret_value=%d", rf_ret_val, 0, 0 );
        }
        else if ( rf_ret_val > RFM_CDMA_HEALTHY_STATE )
        {
          /*
          This case is not currently handled. The RF code only returns 0 or
          negative value for the wakeup_tx function and the rest of this code
          is dependant on wakeup_tx being done. The callback is not used
          (passed as NULL) as RF returns the callback immediately and not
          asynchronously. So this could lead to race conditions in cases
          where wakeup_tx takes some time to actually enable TX.
          */
        }

        /* TxLm has been allocated, update the API */
        srch_mux_set_txlm_handle( onex_tx_lm_handle );

        /* notify the TX SM that tx is awake */
        srch_rx_tx_wakeup();
        #else
        /* RUMI: Send TX Start CMD on behalf of RF */
        txc_send_fw_tx_start();
        #endif /* FEATURE_MODEM_1X_SUPPORTS_RF*/

        M1X_MSG( MUX, LEGACY_MED,
          "Enable RF TX - is_enabled(%d), is_stopped(%d)",
          is_enabled,
          is_stopped);

        txc_tx.rf_state = TXC_RF_STATE_ENABLE;
        is_enabled = TRUE;
        is_stopped = FALSE;
      }
      break;

    case TXC_RF_STATE_STOP:

      if(is_enabled && !is_stopped)
      {
        M1X_MSG( MUX, LEGACY_MED,
          "Stop RF TX - is_enabled(%d), is_stopped(%d)",
          is_enabled,
          is_stopped);

        #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
        /* Freeze Tx closed loop power immediately when stopping Tx.  It
           will automatically be enabled by MUX when Tx is restarted after
           successful HHO */
        rfm_1x_set_tx_closed_loop_power( txc_get_tx_device(),
                                         RFM_1X_TX_CL_FREEZE,
                                         0 );
        #else /* !FEATURE_MODEM_1X_SUPPORTS_RF */
        M1X_MSG( MUX, LEGACY_ERROR,
          "No RF support: rfm_1x_set_tx_closed_loop_power()" );
        #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

        /* Note that for NikeL, this actually takes effect at the next
           frame boundary (frame_valid = FALSE).  If something more immediate
           is needed, or RPC needs to be stopped immediately, then a revised
           interface w/ FW might be necessary. */
        enc_tx_enable(FALSE);

        txc_tx.rf_state = TXC_RF_STATE_STOP;
        is_stopped = TRUE;
      }
      break;

    case TXC_RF_STATE_START:

      if(is_enabled && is_stopped)
      {
        M1X_MSG( MUX, LEGACY_MED,
          "Start RF TX - is_enabled(%d), is_stopped(%d)",
          is_enabled,
          is_stopped);

        /* Note that for NikeL, this actually takes effect at the next
           frame boundary (frame_valid = FALSE).  If something more immediate
           is needed, or RPC needs to be stopped immediately, then a revised
           interface w/ FW might be necessary. */
        enc_tx_enable(TRUE);

        txc_tx.rf_state = TXC_RF_STATE_START;
        is_stopped = FALSE;
      }
      break;

    default:
      M1X_MSG( MUX, LEGACY_ERROR,
        "Invalid RF TX state (%d) - is_enabled(%d), is_stopped(%d)",
        state,
        is_enabled,
        is_stopped);
  }
}

/*===========================================================================

FUNCTION      TXC_ISR_CMD

DESCRIPTION
  This procedure posts a command for the interrupt handler and waits for the
  signal from the interrupt handler indicating that the command was processed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void txc_isr_cmd( txc_int_cmd_type command, qword lc_mask )
{
  rex_sigs_type sigs = 0;
  uint32 recovery_count = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - -*/

  M1X_MSG( MUX, LEGACY_HIGH,
    "Sending TXC ISR CMD %d",
    command);

  memset(&txc_int_cmd, 0, sizeof(txc_int_cmd_type_main));

  /* Set the command */
  txc_int_cmd.cmd = command;

  /* Copy the parameters for the ISR command. */
  if(lc_mask)
  {
    qw_equ(txc_int_cmd.mask, lc_mask);
  }

  /* Clear pending CMD signals from the ISR */
  (void)rex_clr_sigs( TX_TCB_PTR, TXC_INT_CMD_SIG );

  /* Signal that the command is ready and available for the ISR to process. */
  txc_int_cmd.iscmd = TRUE;

  /* keep kicking watchdog while waiting for requested signals
     If the failsafe timer expires while we're waiting, attempt to
     repair the damage and try to wait on the signals again.
  */
  do
  {
    /* Configure the failsafe timer to signal us if we get stuck
       waiting for the ISR to process the command. */
    M1X_MSG( MUX, LEGACY_MED,
      "Setting failsafe timer");
    (void) rex_clr_sigs ( TX_TCB_PTR, TXC_TIMER_SIG );
    (void) rex_set_timer( &txc_failsafe_timer, 100 );

    /* Check in with Watchdog task. */
    txc_dog_report();

    /* Dump out timing info pre-wait */
    muxmdsp_print_tx_timing_info( FALSE );

    /* Wait on signals */
    sigs = rex_wait( TXC_INT_CMD_SIG | TXC_TIMER_SIG | TXC_DOG_RPT_SIG );

    /* Dump out timing info post-wait */
    muxmdsp_print_tx_timing_info( FALSE );

    /* Check the signals returned from the rex_wait. */
    if ( sigs & TXC_DOG_RPT_SIG )
    {
      /*-------------------------------*/
      /*      Kick watchdog            */
      /*-------------------------------*/
      txc_dog_report();
    }

    if ( sigs & TXC_TIMER_SIG )
    {
      if(++recovery_count >= 3)
      {
        ONEX_ERR_FATAL("Recovery Mechanism Failed after ISR Lockup", 0, 0, 0);
      }

      /* No known recovery mechanism yet for Dime and beyond. */

    }
    else if ( sigs & TXC_INT_CMD_SIG )
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "Completed TXC ISR CMD %d",
        command);
    }
    else if ( !(sigs & TXC_DOG_RPT_SIG) )
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Unexpected signal from rex_wait, %d",
        sigs);
    }

  }  while ( ( sigs & TXC_INT_CMD_SIG ) == 0 );

  /* kick watchdog one more time on the way out */
  txc_dog_report();

  /* Clear signals and failsafe timer */
  M1X_MSG( MUX, LEGACY_MED,
    "Clearing failsafe timer");
  (void)rex_clr_timer(&txc_failsafe_timer);
  (void)rex_clr_sigs( TX_TCB_PTR, TXC_INT_CMD_SIG | TXC_TIMER_SIG );

  if(recovery_count > 0)
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "Recovery Mechanism Succeeded after ISR Lockup");
  }
}

/*===========================================================================

FUNCTION TXC_TX_IS_ON

DESCRIPTION
  Returns if TX is ON.

DEPENDENCIES
  None

RETURN VALUE
  TRUE: If TX is On
  FALSE: If TX is Off

SIDE EFFECTS
  None
===========================================================================*/
boolean txc_tx_is_on( void )
{
  return txc_tx.tx_on;
} /* txc_tx_is_on */

/*===========================================================================

FUNCTION TXC_IS_CURR_STATE_EXIT

DESCRIPTION
  This API is used by other layers (CP) to check if TX is in exit state.

DEPENDENCIES
  None

RETURN VALUE
  True if current state is EXIT

SIDE EFFECTS
  None
===========================================================================*/
boolean txc_is_curr_state_exit( void )
{
  return (txc_state == TXC_EXIT_S);
} /* txc_is_curr_state_exit */

/*===========================================================================

FUNCTION TXC_TX_WAS_ON_BEFORE_CFS

DESCRIPTION
  Returns if TX was ON prior to doing CFS.

DEPENDENCIES
  None

RETURN VALUE
  TRUE:  If TX was ON prior to doing CFS.
  FALSE: Otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean txc_tx_was_on_before_cfs( void )
{
  return txc_tx.tx_on_before_cfs;

} /* txc_tx_was_on_before_cfs */

/*===========================================================================

FUNCTION TXC_SETUP_PCG_EVENTS_ISR

DESCRIPTION
  This function sets up the m1x stmr events for the specified condition.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_setup_pcg_events_isr
(
  txc_pcg_event_enum_type event_type,
  void (*pcg_event_isr)(m1x_stmr_event_type)
)
{
  m1x_stmr_err_type err_code = 100;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - -*/

  if (pcg_event_isr != NULL)
  {

    /* Deregister any outstanding events */
    m1x_stmr_deregister_client(M1X_STMR_CLIENT_TX);

    switch (event_type)
    {
       case TXC_PCG_EVENT_TRAFFIC:
        /* Traffic requires PCG 1 and PCG 10 */
        err_code = m1x_stmr_register(M1X_STMR_CLIENT_TX,
                                     TXC_TC_FIRST_ISR_PCG,
                                     pcg_event_isr);

        err_code |= m1x_stmr_register(M1X_STMR_CLIENT_TX,
                                      TXC_TC_SECOND_ISR_PCG,
                                      pcg_event_isr);
        break;

       case TXC_PCG_EVENT_ACCESS:
        /* Access requires PCG 8 */
        err_code = m1x_stmr_register(M1X_STMR_CLIENT_TX,
                                     TXC_ACCESS_ISR_PCG,
                                     pcg_event_isr);
        break;

       case TXC_PCG_EVENT_HOLD_ENC_CLK:
        /* Enc hold requires PCG 1 */
        err_code = m1x_stmr_register(M1X_STMR_CLIENT_TX,
                                     TXC_HOLD_ENC_CLK_ISR_PCG,
                                     pcg_event_isr);
        break;

       default:
        err_code = 101;
    }
  }

  if ( err_code != M1X_STMR_SUCCESS )
  {
    /* err_code = 100 is NULL function pointer
                  101 is invalid event type
                  other - returned by m1x_stmr */
    ONEX_ERR_FATAL("Failed to register isr: err - %d, event_type %d",
              err_code, event_type, 0);
  }

} /* txc_setup_pcg_events_isr */

/*-------------------------------------------------------------------------*/
/*         API to interact with MCS Susbcription Priority Module           */
/*-------------------------------------------------------------------------*/

/*===========================================================================

FUNCTION TXC_UPDATE_SP_STATE

DESCRIPTION
  Updates the MCS Subscription Priority Module with the current 1x State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_update_sp_state( sp_tech_states_enum_t state )
{
  static sp_tech_subscription_info current_1x_state = {
                                                        SP_TECH_NO_STATE,
                                                        SYS_SYS_MODE_CDMA
                                                      };

  if ( state != current_1x_state.state )
  {
    current_1x_state.state = state;
    if ( !sp_tech_subscription_state_info_update( current_1x_state ) )
    {
      TX_DBG_MSG( "1x state(%d) update to SP Module failed",
               state );
    }
  }

} /* txc_update_sp_state */


#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION mc_qsh_mdump_collect_txccommon_data

DESCRIPTION
This function dumps the txccommon data to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void mc_qsh_mdump_collect_txccommon_data()
{
  qsh_mdump_collect_high(&txc_state,sizeof(txc_state));
  qsh_mdump_collect_high(&txc_tx,sizeof(txc_tx));
}
#endif /* FEATURE_QSH_MDUMP */