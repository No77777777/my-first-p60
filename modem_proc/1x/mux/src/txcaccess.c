/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

          C D M A   T R A N S M I T   A C C E S S    M O D U L E

GENERAL DESCRIPTION
  Contains items associated with the access channel

EXTERNALIZED FUNCTIONS
  txc_acc_probe_resume - resumes the transmission of next Access probe
  txc_access_init - initialization section for access channel
  txc_access_att - state where access channel transmission is transmitted
  txc_hyperspace - performs jump to hyperspace for Transmit task
  txc_access_idl - Access channel idle state
  txc_flush_enc - Flush encoder with zeros at the entrance of CDMA state so
                  encoder will output expected power level for first probe
                  after power up

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

                      EDIT HISTORY FOR FILE

$Header: //components/rel/1x.mpss/8.0/mux/src/txcaccess.c#2 $ $DateTime: 2020/01/27 00:26:21 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
01/19/18   ppr     F3 Reduction changes
03/07/17   sjo     Disable logging Key info based on return value of API.
12/29/16   eye     Perform a check on the length of the Access Channel Message.
12/13/16   nsh     Fixed KW errors.
08/04/16   eye     Dont update RPC params during LTA GAP for 1x2GTA.
08/01/16   eye     Update txc_frame_offset_cmd.frame_offset in TC_F.
05/16/16   eye     Mainline FEATURE_MODEM_1X_IRAT_LTO1X for L compileout build.
04/14/16   srk     Optimized TX Trigger fixes and code cleanup.
03/15/16   eye     Update FL FO for TC_F cmd as part of WTR_HOP RIF failure.
02/05/16   eye     1x2GTA Feature changes
01/21/16   srk     Do a forced switch to pass through config on ACCESS entry.
04/21/15   srk     DRDSDS and BOLT REVISIT Feature cleanup
01/06/15   srk     Wait for TX start/stop response before sending ASDIV command
11/07/14   srk     Add a mechanism to wait for Tx trigger response from FW
                   before proceeding with abort of hyperspace jump.
10/08/14   srk     FEATURE_MODEM_1X_UTX related cleanup.
09/05/14   srk     Remove register and deregister of vstmr events in
                   ISR context.
06/12/14   vks     Change traffic isr to PCG 10 from PCG 12
                   Use common api to setup the events
05/29/14   pap     Redistributing messages.
05/19/14   bph     Make callbacks registered with TRM asynchronous
02/03/14   eye     Increasing the RTC trigger time to 1ms.
03/04/14   pap     Changes to resolve warnings by LLVM compiler.
02/13/14   dkb     Add support for optimized Tx trigger.
02/04/14   eye     Set PCG mask to 0xFFFF during access enrty & also during
                   teardown,make it independent of RC3 1/8th gating and DTX.
02/03/14   dkb     Feature guard Triton UTX code under FEATURE_MODEM_1X_UTX
12/03/12   dkb     Remove double Tx trigger in the case of a jump abort.
11/25/13   cjb     Switched logic for FEATURE_MODEM_1X_BOLT_OBSOLETE feature
11/14/13   dkb     Process muxmsgr messages in all Tx states.
11/07/13   cjb     Skip ENC_HOLD state for now to aid SOD bring-up
11/01/13   dkb     Clean up enc test waveform and reduce the frequency of
                   sending frame config to FW.
10/15/13   cjb     Removed unused IRQs and related cleanup
10/09/13   dkb     Add additional RL changes.
10/03/13   srk     Update struct member names to reflect correct meaning.
09/30/13   dkb     Initial RL changes for new FW interface.
09/10/13   dkb     Register Tx interrupts using m1x vstmr API
09/06/13   srk     Remove obsolete function references.
08/28/13   cjb     Mainline FEATURE_MODEM_1X_DIME_REVISIT.
                   Removed STX manager support.
07/25/13   eye     Switch to secure api for random number generation.
07/23/13   srk     Replace RF API calls with mux API.
07/08/13   dkb     Cleanup TX teardown
05/31/13   srk     Mainlined FEATURE_MODEM_1X_NON_SIG.
05/07/13   eye     Backing out SAR updates done for COEX.
04/25/13   eye     Updating SAAR changes which is to be replaced by COEX.
04/23/13   trc     Allow failure of enc_is95c_enable() to short-circuit the
                   transition from access to encoder hold state
03/05/13   srk     Replace existing watchdog implementation with the new
                   Dog Heartbeat Mechanism.
02/15/13   srk     Replace mcs hwio with m1x hwio
12/17/12   vks     Add support for 1x access asdiv.
11/01/12   jtm/bb  Process TXC_ACC_PROBE_F command in TXC_ACC_IDLE_S state
12/19/12   bph     Replace memcpy w/ struct assignment or memscpy
12/19/12   vks     Add support for stx manager
12/12/12   cjb     Fixed a sequencing issue for e1XCSFB. TX was getting turned
                   ON before completion of HHO due to erroneous condition check.
12/10/12   trc     Layering/Cleanup for Triton
11/08/12   vks     Clear hyperspace in progress flag after the abort
10/25/12   trc     Add txc_hyperspace_abort() hyperspace termination API
09/26/12   srk     Fix compilation issues when FEATURE_LTE_TO_1X is disabled.
09/26/12   jtm     DIME_REVISIT clean up.
09/13/12   jtm     Mainlined FEATURE_MODEM_1X_NIKEL.
08/22/12   cjb     Allowed transition to HOLD_ENC_CLK state in debug code.
08/20/12   jtm     Added debug code to skip multiple jumps to hyperspace.
08/16/12   jtm     Wrapped obsolete rex_wait during jump to hyperspace with Dime revisit.
07/11/12   srk     Changed the returned values from RF to match legacy units
                   for proper tx power logging.
06/15/12   trc     Trigger TxDAC immediately upon hyperspace abort
05/15/12   srk     Call TX wrapper function to get TX power instead of directly
                   calling RF API.
05/08/12   jtm     Optimized jump to hyperspace and treat TT_F like JUMP_F.
04/04/12   trc     Convert to m1x_time module
04/04/12   srk     Remove Redundant calls to rfm_1x_notify_tx_config.
03/11/12   trc     Add messaging around Tx timing sync internals
02/23/12   srk     Mainlined FEATURE_IS2000_SCH and FEATURE_IS2000_P2
02/22/12   jtm     Change Tx clock voting to use additional states.
02/17/12   srk     Replaced the tramp api with DAL Interrupt Controller.
02/16/12   srk     Replaced FEATURE_LTE_TO_1X by FEATURE_MODEM_1X_IRAT_LTO1X.
02/07/12   trc     Remove FCH enable in TC_F, same reason as ACCESS_F below
02/05/12   trc     Remove FCH enable in ACCESS_F, since it can interfere with
                   the Tx timing sync procedure from a preceding JUMP_F
01/31/12   srk     Feature Clean Up.
01/17/12   jtm     Revert REL_A feature clean up.
01/11/12   srk     Feature Cleanup
01/09/12   trc     Remove NikeL v1 HW support
12/13/11   srk     Feature clean up: Mainlined FEATURE_RL_JUMP_ENHANCEMENT and
                   changed all FEATURE_IS2000_REL_A_* to FEATURE_IS2000_REL_A
12/02/11   jtm     Use txc_isr_cmd() instead of DO_INT_CMD macro
12/01/11   srk     Move txc_tx_shutdown to txccommon.c.
11/22/11   srk     Feature clean up: Mainlined and Internalized Features
11/04/11   trc     Handle zeroing RL frame offset before access, NikeL cleanup
09/13/11   trc     Ignore JUMP_F for NikeL
09/02/11   sk      Rebased the code to the NikeL modem package by mainlining
                   FEATURE_MODEM_1X_NIKEL.
08/31/11   adw     Unify TT and native TX jump to hyperspace.
08/17/11   trc     Mainline RF SVDO API
08/04/11   jtm     Moved TXC_PWR_MAX definition to txccommon.h.
06/28/11   jtm     Added default definition for nikel_lc_mask_slam_workaround.
06/15/11   adw     Move to new TxAGC interface as old interface is deprecated.
06/03/11   adw     SV support.
05/26/11   trc     NikeL Tx support
05/03/11   jtm     Ensure we are in phase2 before updating long code in TT.
05/09/11   trc     NikeL support
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
03/28/11   jtm     Removed deprecated RF includes.
03/03/11   adw     Initialize encoder when entering access.
01/28/11   jtm     Additional FTM decoupling.
01/28/11   vks     Featurize RF interface with FEATURE_MODEM_1X_SUPPORTS_RF.
01/25/11   jtm     Lint Fixes.
01/21/11   vks     Use 1X specific RF SVDO feature - FEATURE_1X_RF_SVDO_API.
12/17/10   jtm     Feature cleanup.
12/15/10   vks     Temporarily featurize register accesses for Nikel.
12/09/10   jtm     Removed unnecessary include of clkregim.h
12/01/10   bb      Add support for LTE to 1X feature
11/15/10   jtm     Initial SU API split for CP.
11/05/10   jtm     Added more _i.h includes for Modem 1x SU API support.
10/30/10   jtm     Modem 1x SU API changes.
10/07/10   jtm     Replaced clk_regime*() calls to enable/disable TX with
                   enc_update_tx_clk_resources().
08/06/10   jtm     Mainlining TXC-related clocks.
06/15/10   jtm     Added RCPM event reporting.
                   Relocated RC8 specific Tx data to rc8_status substructure.
06/10/10   jtm     Eliminate dependency on log_dmss.h
06/01/10   jtm     Remove uncessary COMBINER_ENABLE_POWER_CONTROL from
                   txc_tx_disable()
05/13/10   trc     Add TX Conditioning clock support for Genesis 1.1
04/20/10   jtm     Replace rfm_tx_*() api calls and common code associated with
                   starting/stopping/enabling/disabling TX with a wrapper
                   function.
04/12/10   jtm     1x Advanced: Initialize blanking duty cycle and frame DTX
                   flag when TXC_TC_F is received.
03/03/10   vks     Include header file.
03/03/10   vks     Ensure closed loop power control is turned ON after acess
                   probes are sent.
02/04/10   jtm     Removed FEATURE_PLT.
02/01/10   jtm     Removed feature TMOBILE.
01/28/10   jtm     Corrected gain programming for R-EACH.
01/06/10   vks     Remove duplicate setting of channel gains in txc_access_isr
                   (also being set in enc_fch_data_out() which is called
                   immediately).
11/30/09   vks     Group together mdsp writes for 1xADV channel gain ratios.
11/07/09   vks     Grouped together mdsp writes for channel gains.
                   Remove tx_rate_adj variable. Use the one present in
                   channel_gains data structure (in muxmdsp.h) instead.
                   Use api provided by muxmdsp.h instead of
                   TXC_SET_<CHANNEL>_GAIN macros.
10/30/09   jtm     Moved FTM NC 1/8th rate frame support to txctraffic.c
10/28/09   jtm     Update to 1xA variable names.
10/27/09   jtm     Lint fixes.
10/16/09   jtm     FTM support for non-critical 1/8th rate frames for RC8.
09/22/09   jtm     Corrected compiler warnings.
08/22/09   jtm     Lint Fixes.
08/21/09   jtm     Lint Fixes.
08/17/09   jtm     Enable CDMA Tx clocks when TXC_REACH_F command is received.
08/11/09   jtm     Replaced deprecated ts.h api's with new time api's.
08/05/09   jtm     Lint fixes.
07/15/09   jtm     Removed include for rf1x_core_rf.h.
07/06/09   jtm     Added error checking for 1xA RC8.
06/11/09   jtm     Additional 1x Advanced RL support.
05/20/09   jtm     Added include for tx_v.h for CMI clean up.
04/21/09   jtm     Replaced assert.h with amssassert.h. Assert.h is being
                   phased out.
04/17/09   jtm     Added includes to fix compiler warnings.
04/13/09   adw     Updated rf includes to support rf mode refactoring.
04/09/09   vlc     Backed out enc_hw_reset() change as it causes MDSP failure.
04/08/09   jtm     Initial 1X Advanced feature support.
04/06/09   mca     Added RF SVDO API support
04/02/09   jtm     Removed T_IO_CARD.
04/01/09   jtm     Featurized necessary includes.
03/31/09   jtm     Eliminated implicit includes.
03/17/09   jtm     Replaced INTLOCK/INTFREE, INTLOCK_SAV/INTFREE_SAV, and
                   MSM_LOCK/MSM_UNLOCK with macros that can be mapped correctly
                   based on the compiler and controlled internally to TXC.
03/10/09   vlc     Added call to enc_hw_reset() before sending access probes.
03/03/09   jtm     Lint fixes.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
08/03/08   jtm     Removed FEATURE_IS2000_REL_D_DV and FEATURE_IS2000_REL_C_DV
                   and FEATURE_DELAYED_TX_CLOCK_SHUTDOWN code. Removed txc_flush_enc()
08/01/08   vlc     Turn off RF in Hold Enc Clock state to avoid leaving RF on
                   while tuning to a different frequency.
07/09/08   vlc     Fixed handoff failures after 1st successful handoff in VOIP
                   to 1X transfer.
07/01/08   vlc     Merged code changes from old tip archive.
05/07/08   trc     Push clock voting down into txc_access_idl_init() and
                   txc_tt_init().  Remove unsafe txc_cdma_enc_setup() from
                   TXC_CDMA_F handling in txc_access_cmd(), as it
                   will now be done safely upon TXC_CDMA_S entry.
04/17/08   trc     Ensure clocks are ON when we get a TXC_TC_F
03/31/08   mca     Changed FEATURE_CLKREGIM_RM to FEATURE_SRCH_CLKREGIM_RM
03/05/08   vlc     User clock resource manager to manage TX clocks
                   (FEATURE_CLKREGIM_RM.)
02/23/08   vlc     Added new interface to MDSP to notify MDSP of entrance and
                   exit of RL access state mainly to fine tune access probe
                   power management (FEATURE_ACCESS_PROBE_PWR_ADJ.)
04/02/07   vlc     Added new PCG define for bin0 ENC ISRs specifically for loop
                   back calls.  Moved default PCG to 1 for bin0 ENC ISRs for
                   all other calls.
02/23/07   vlc     Fixed compiler warnings/errors when SCH, Rel A, and P2
                   features are turned off.
01/25/07   vlc     Allow the processing of TXC_ACC_STOP_F in hold encoder clock
                   state.  Allow TXC to sleep when ENC clocks are turned off.
01/22/07   trc     Parameterize the PCG for bin0 and bin3 ENC ISRs
11/14/06   vlc     In function txc_access_idl_init(), make sure 1X has primary
                   chain before turning off RF (to work with SHDR.)
10/30/06   bn      Added code to support DO/UMTS to 1x handoff
10/26/06   vlc     Changed TXC time line for traffic state.  Moved 1st ENC ISR
                   to bin0, PCG1.  Moved 2nd ENC ISR to bin3, PCG2.
09/19/06   vlc     Fixed lint errors.
09/05/06   vlc     Implemented TX gain enhancements to increase digital gain
                   resolution.
08/15/06   vlc     In function txc_flush_enc(), added call to enc_fch_rc().
08/03/06   vlc     Added function txc_flush_enc().
06/09/06   trc     Remove T_MSM6700 featurization
05/16/06   vlc     Reset tcb_ptr after notifying MC of the event to prevent
                   false notification in the future.
01/17/06   rkc     Call srch_mux_set_tx_adj(0x7F), was 0, to set gain
                   adjustments after last frame has been sent (R-EACH).
11/07/05   rkc     Add call to enc_fch_enable() in txc_access_cmd() when
                   TXC_ACCESS_F is received.  This takes care of NCC to
                   OCC ECAM redirection test case.
09/27/05   rkc     Remove redundant txc_hold_enc_clk() function prototype.
09/15/05   rkc     Check boolean hold_enc_clk_on is TRUE before transitioning to
                   TXC_HOLD_ENC_CLK_S.
09/08/05   ejv     Use new srch_mux interface calls.
08/11/05   ddh     Added change to correct access proble issue for 7500
08/09/05   bn      Fixed the RA trigger logic for number of RA = 2
07/26/05   vlc     Applied code review comments for RL jump to hyperspace
                   enhancement.  Removed code to force RN value to 0 in
                   txc_reach_msg_prep().
07/25/05   rkc     Fixed lint errors.
06/06/05   vlc     In functions txc_access_att() and txc_reach_access_att(),
                   set jump source prior to transitioning to hold enc clock
                   state.
05/23/05   vlc     Changed FEATURE_IS2000_REL_D_JUMP to FEATURE_RL_JUMP_ENHANCEMENT.
                   Implemented multiple changes for feature
                   FEATURE_RL_JUMP_ENHANCEMENT:
                   Needed to NOT allow sleep while holding ENC clocks ON so 2nd
                   message on ACCESS channel can be received by Base station.
                   Need to add RN value to TXC_JUMP_F command.
                   In function txc_reach_access_att(), put back code under feature
                   FEATURE_DELAYED_TX_CLOCK_SHUTDOWN.  This code does not belong in
                   FEATURE_TX_ACCESS_POWER_FIX, which had been removed.
05/16/05   vlc     Removed feature FEATURE_TX_ACCESS_POWER_FIX which was a software
                   work around and was no longer needed for Jaguar and newer targets.
                   This feature was already turned off in Jaguar and newer targets.
05/12/05   vlc     Mainlined SRCH_HAS_TRK_LO_CNTL feature.
04/06/05   vlc     Added extra debug statements.  Moved code to set jump source
                   inside txc_hold_enc_clk_init().  Changed TXC_JUMP_CMD to
                   TXC_JUMP.
03/15/05   vlc     Fixed mismatch of {}.  Added function prototype for
                   txc_hold_enc_clk_init().
03/14/05   vlc     Fixed compiler warnings for MSM6700.
03/14/05   vlc     Added logic to eliminate Jump to Hyperspace from ACCESS state to
                   TRAFFIC state (still under feature FEATURE_IS2000_REL_D_JUMP.)
03/14/05   vlc     Merged in changes for MSM7500.  Changed T_QUASAR_BB to T_IO_CARD.
                   (dlb')
03/04/05   vlc     Added support for Fast Call Set Up - New Jump to
                   Hyperspace command feature.
                   Changed txc_access_init() to txc_access_idl_init() to clarify
                   that this function is called upon entering TXC_ACC_IDLE_S.
03/07/05   bn      Cleaned up debug F3 messages
12/14/04   ljl     Added event report for R-EACH.
10/22/04   ljl     Called enc_interrupt_slot() to set txc_reach_interrupt_slot
                   if MSM6700 is undefined.
09/29/04   sr      Included rf.h and put Rel DV include files under feature
09/28/04   sr      Merged in changes from 6550 2009 build
09/27/04   ljl     Replaced pdch with rev_pdch.
09/01/04   jrp     Added back pre-6700 interfaces.
08/20/04   vlc     Removed MCC_TXC_JUMP_DONE_SIG, no longer needed by MC task.
08/16/04   sr      Enabled DV app if F-PDCH is included in TXC_TC_F
08/16/02   ljl     Rel D reverse channel PLT.
08/10/04   vlc     Set MCC_TXC_JUMP_DONE_SIG when TXC is done with Jump to Hyperspace,
                   ready to enter traffic state.
07/22/04   bn      Replaced the call of srch_set_tx_rate_adj by rpc_set_tx_rate_adj
07/21/04   bn      For REACH, tell the MDSP to load the frame off immediately
07/20/04   bn      With the new MOD INT, no need to install the access IST
                   in the txc_access_init function
07/15/04   sr/ll   Featurized out rf driver calls under T_QUASAR_BB
07/07/04   bn      Fixed the wrong featurization in txc_access_att(). REACH should
                   be used regardless of FEATURWE_TX_ACCESS_POWER_FIX.
06/29/04   jrp     Pass-through gains, and modulator interrupt changes.
06/16/04   sr      Merged in changes from 6500 5004 build
05/21/04   jyw     introduce new RF PA backoff interface
05/18/04   jrp     Change order of ISR handler registration, and interrupt.
04/20/04   jrp     Fixed lint warnings.
03/17/04   jrp     Turn off pich and sch channels and set TX_RATE_ADJ and
                   TX_GAIN_ADJ to zero between R-EACH probes.
03/05/04   sr/bkm  Moved initializing the frame offset from MC to TXC_TC_F
02/11/04   jrp     Fixed compilation error when FEATURE_TX_ACCESS_POWER_FIX
                   is undefined.
11/05/03   sst     Changed trk_lo_adj wakeup/sleep calls to initiate change to
                   move trk_lo_adj functionality into a single module rather
                   than macro calls throughout the code base
10/30/03   jrp     Fixed compiler warnings.
09/15/03   jrp     For FEATURE_DELAYED_TX_CLOCK_SHUTDOWN, calls shutdown
                   function in task context rather than interrupt context.
09/09/03   dna     Merged porting changes for MSM6500
08/25/03   vlc     Removed shared rate_set from txc_so (txc_so_type) to avoid
                   confusion between FCH and DCCH rate sets.  Now calculate
                   rate set from RC as it is needed.
06/24/03   jrp     Changed calls to srch_set_tx_rate_adj() to call instead
                   rpc_set_tx_rate_adj() so that TX_RATE_ADJ is written in the
                   appropriate units - either 1/12 or 1/10dB.
05/23/03   jrp     Code review changes for delayed TX clock shutdown.
                   Fixed CAI_EACH_BA_?_ID and CAI_ACC_?_ID to increment at most
                   once for each access attempt.
                   Fixed ASSERT for REACH attempts based on RACH length limit.
                   Added random selection of REACH channel.
04/15/03   jrp     Support for FEATURE_DELAYED_TX_CLOCK_SHUTDOWN.
03/04/03   bkm     Added support for REV_FCH_GATING event.
02/10/03   jrp     Removed lint warnings.
02/10/03   jrp     Implemented persistence delay for REACH.  Made backoff and
                   probe backoff standard compliant.  Corrected LOG_EA_PROBE
                   to give 1-based max probes per sequence, instead of 0-based.
12/03/02   bt      rename ulpn_advance to ulpn_adjust.
11/21/02   jrp     Featurized txc_copy_reach_parameters.
11/15/02   jrp     Added support for APHO.  Added band class to LOG_EA_PROBE_C
                   packet.  Added support for REACH counters.  Made REACH TA
                   counter standard compliant.
11/01/02   jrp     Fixed lost REACH probes due to reading time earlier than
                   interrupt time, and programming the long code accordingly.
08/23/02   jrp     Changed REACH signaling logging's length calculation to
                   allow for extended message lengths.  Changed
                   txc_reach_msg_prep() to allow for extended message sizes.
08/20/02   jrp     Support for logging REACH message and probes, log
                   codes D7 and BD.
07/26/02   tc      Supported REACH 5ms and 10 ms frame.
07/16/02   bgc     Featurized txc_so.rev_fch.included in FEATURE_IS2000_P2.
07/01/02   tc      Supported REACH interference correction calculation.
06/24/02   tc      Supported REACH preamble and data transmission output power.
06/12/02   hrk     If FCH/DCCH information not included, then setting
                   txc_so.rev_fch.included / txc_so.rev_dcch.included to FALSE.
06/07/02   hrk     Added VP2 support.
05/28/02   lcc     Turns off pilot gating when getting on TC just in case it
                   was gated from last call.
05/14/02   tc      Set txc_reach_si_include flag to true.
04/10/02   tc      Supported REACH pilot reporting command TXC_REACH_PROBE_F.
04/04/02   tc      Set REACH transmitting buffer length to message length.
04/03/02   tc      Supported REACH SI bits, txc_reach_probe_prep().
04/03/02   tc      Supported REACH Ext_Msg_length and Msg_Length.
04/02/02   tc      Supported REACH 5ms and 10ms frame size information bits.
03/08/02   bn      Added logic to log interference-correction
03/04/02   bgc     Changed features to IS2000_R_SCH where appropriate.
02/28/02   hrk,fs  Replaced PILOT_FCH_9600 with PILOT_FCH
02/25/02   hjr     Fixed featurization of FEATURE_IS2000_SCH with
                   FEATURE_IS2000_DS_RS2 on.
02/22/02   tc      Supported FEATURE_IS2000_REL_A_CC.
02/11/02   tc      Supported Reverse Enhance Access Channel.
01/15/02   hrk     Changed RF_ENABLE_PWR_WAIT to RF_ENABLE_TX_PWR_WAIT when using
                   FEATURE_RF_WAIT_CHANGES.
01/15/02   hrk     Setting TX_RATE_ADJ register to 0 before access probe seq.
01/12/02   sst     Modified to use the new srch_* files that replaced srchdrv.c
                     Includes a function name change:
                       srch_get_ref_time -> srch_scomb_get_ref_time
01/03/02   bgc     Added SCH featurization to turn off for MSM6000.
11/15/01   lcc     In txc_access_cmd, added enabling of FCH when TXC_TC_F is
                   received with FCH included; otherwise FCH is disabled when
                   DCCH is disabled for 5000 and 5105.
11/02/01   lcc     In txc_access_cmd, when TXC_TC_F is received, added disabling
                   of FCH or DCCH appropriately when the channel is not included.
09/14/01   bn      Fixed P2 featurization
08/22/01   bn      Support DCCH in FTM mode
07/12/01   vlc     Changed FEATURE_RC4_SCH to FEATURE_DS_IS2000_RS2 in order
                   to eliminate FEATURE_RC4_SCH.
06/28/01   vlc     Fixed r-sch parameter verification in PLT.
06/25/01   lcc     Properly initialize txtc.cur_dsch (dedicated signalling channel)
                   on TXC_TC_F command.
06/15/01   day     Mainlined FEATURE_RLGC, FEATURE_IS95B, FEATURE_IS95B_OLPEC,
                   FEATURE_IS95B_ACC_PRB_BKOFF_DLY, FEATURE_IS2000
                   Delete FEATURE_SPECIAL_MDR
06/12/01   vlc     Fixed a typo (rev_sch_rc was inadvertently changed to
                   vrev_sch_rc).
05/23/01   fas     Add hooks for rf_pa_backoff.c
05/18/01   kk      RS2 TDSO support
05/14/01   jal     Corrected log buffer allocation in tx_log_ac_msg() to ask
                   for exactly as much space as it needs.
05/11/01   lcc     In txc_access_cmd, when TXC_TC_F is received, added turning
                   off of rev. FCH 8th rate gating when it is disabled.
04/26/01   vlc     Added SCH RC4 support.
04/20/01   sr      Merged in from MSM5100 archive
04/19/01   tc      Merged with MSM_MUX1X.01.00.01
           fc      Added support of events report: Access denied.
                   Renamed mcc_report_event_msg_txed to
                   mclog_report_event_msg_txed.
04/18/01   sr      Merged with MSM_MUX1X.01.00.00
           lcc     Added support for FEATURE_IS2000_P2
03/27/01   vlc     Merged MSM_MUX1X.00.00.17.
           fc      Added support of events report: Message transmitted.
03/14/01   tc      Loaded enc_stmr_cmd at enc_user_pn_mask and enc_user_pn_state.
03/01/01   tc      Merged MSM_MUX1X.00.00.15.
02/16/01   bgc     Changed FEATURE_FACTORY_TESTMODE to enable a runtime switch.
01/12/01   tc      Merged MSM_MUX1X.00.00.06.
01/03/01   tc      Loaded BTF value by sending MOD_STMR_CMD.
12/15/00   tc      Fixed long code state loading.
11/09/00   tc      Defined feature T_MSM_5100.
11/08/00   tc      Supported MSM5100 modulator driver interface.
12/07/00   bgc     Set sch_rate for a PLT SCH channel setup.
11/04/00   fc      Changed the RT to 0 to BKOFF + 1 for IS-95B mobile with
                   which the base station supports only one access channel.
11/10/00   jc      This file was part of txc.c. See txccommon.c for full
                   change history

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common
#include "1x_variation.h"
#include <memory.h>
#include "comdef.h"
#include "customer.h"
#include "m1x_diag.h"
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"

//mux
#include "mux_logtypes.h"
#include "muxmdsp_i.h"
#include "muxmsgr.h"
#include "rlgc.h"
#include "rlgc_i.h"
#include "txc.h"
#include "txc_v.h"
#include "txc_i.h"
#include "txcaccess.h"
#include "txccommon_i.h"
#include "txcmc.h"
#include "txcmc_i.h"
#include "txctraffic_i.h"

//drivers
#include "enc.h"
#include "enc_v.h"
#include "enc_i.h"
#include "enci.h"
#include "enci_i.h"             /* Encoder register & macro definitions */
#include "m1x_hwio_mpss.h"
#include "msm_drv.h"
#include "msm_drv_i.h"
#include "m1x_stmr_i.h"
#include "m1x_time_i.h"

//cp
#include "parm_i.h"
#include "mc_i.h"
#include "mclog.h"
#include "mclog_v.h"
#include "cai_v.h"
#include "tx.h"
#include "tx_v.h"

//srch
#include "srch_mux.h"
#include "srch_rx_div_i.h"
#include "ulpn.h"

//diag
#include "onex_non_sig.h"

//other
#include "qw.h"
#include "queue.h"
#include "rex.h"
#include "amssassert.h"
#include "err.h"
#include "crc.h"
#include "bit.h"
#include "log.h"
#include "ran.h"
#include "msg.h"
#include "event.h"
#include "secapi.h"

#ifdef FEATURE_MODEM_1X_SUPPORTS_RF
#include "rfm_1x.h"
#endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

#include "stringl.h" /* for memscpy */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* mask used to determine phase of 80 ms superframe from time reference
   read from Searcher task */
#define TXC_ACC_PHASE_80M 0xC000

/* value for masked reference time when in 1st 26.66 ms period of 80 ms
   superframe */
#define TXC_ACC_PHASE0 0x0000

/* value for masked reference time when in 2nd 26.66 ms period of 80 ms
   superframe */
#define TXC_ACC_PHASE1 0x4000

/* value for masked reference time when in 3rd 26.66 ms period of 80 ms
   superframe */
#define TXC_ACC_PHASE2 0x8000

/* Value to shift PHASE_80M masked value to get to range 0 .. 2 */
#define TXC_ACC_PHASE_80_SHIFT 14

/* maximum value for reference time read from searcher */
#define TXC_ACC_MAX_REF_TIME 0xBFFF

/* maximum  number of 1/2 chips in single PN ROLL i.e SYNC 26 */
#define TXC_ACC_MAX_SYNC26_TIME 0x3FFF

/* buffer time needed for 1x FW from dac_start to sync26 boundary
  3072 half chips means 5ms*/
#define TXC_ACC_BUFFER_TIME 3072

  /* number of 2-chip increments which must exist before the end of the
     80 ms superframe in order to be able to call enc_tx_clk_enable without
     having the 80ms superframe occur in the middle of the enc_tx_clk_enable
     call */
#define TXC_ACC_TX_ENABLE_TIME 50

static boolean txc_tt_turn_on_transmitter = FALSE;

#ifdef FEATURE_IS2000_REL_A_CC

// static boolean txc_reach_on = TRUE;
static boolean txc_reach_access;
static boolean txc_reach_si_include = TRUE;
static boolean txc_reach_first_frame_input_data;
static boolean txc_reach_program_preamble_prepare;
static boolean txc_reach_program_data_prepare;

static word txc_reach_preamble_frame_offset = 0;
static word txc_reach_data_frame_offset = 0;
static word txc_reach_frame_offset = 0;

static word txc_reach_preamble_count = 0;
static word txc_reach_data_count = 0;

// static word txc_reach_preamble_pcg_intr_cnt = 0;
// static word txc_reach_data_pcg_intr_cnt = 0;

static word txc_reach_intr_cnt = 0;

// (bkoff+rp)*(each_slot)
static word txc_reach_countdown = 0;

// (bkoff_frame_pcg/each_slot) remainder
static word txc_reach_count = 0;

// (bkoff)*(each_slot) + (rp)*(each_slot) + count
static word txc_reach_count_pcg = 0;

// ( (bkoff+rp)*(each_slot) + count )/16
static word txc_reach_count_frame = 0;

// ( (bkoff+rp)*(each_slot) + count ) mod 16
static word txc_reach_pcg_start = 0;


static word txc_reach_slot_offset = 0;

// SLOT LENGTH = 4 PCG = 5 ms
static word txc_reach_slot_length = 4;

static word txc_reach_slot_offset1 = 0;
static word txc_reach_slot_offset2 = 0;

// REACH Sub-attempt number, incremented with each access probe handoff
static byte txc_reach_sub_att_number = 0;

// PREAMBLE LENGTH = 4 PCG = 5 ms
static word txc_reach_preamble_length = 4;

// FRAME LENGTH = 20 ms
static word txc_reach_frame_length = ENC_SCH_FRAME_SIZE_20_V;

static word txc_reach_frame_size;

static byte txc_reach_rlgain_common_pilot;
static byte txc_reach_ic_thresh;
static byte txc_reach_ic_max;

static txc_each_rate_type txc_reach_rate_word;

#define TXC_REACH_FRAME_1       1;
#define TXC_REACH_FRAME_2       2;
static byte txc_reach_input_frame;

// Number of 20ms duration frame count.
static word txc_reach_20ms_frame_cnt;

// Number of psist failures for this probe
static word txc_reach_psist_delay;

// 10ms frame, 19200bps, 192 bits/frame, 24 bytes/frame
// 10ms frame, 38400bps, 384 bits/frame, 48 bytes/frame
//  5ms frame, 38400bps, 192 bits/frame, 24 bytes/frame
static byte txc_reach_frame_1_bin_0[48];
static byte txc_reach_frame_1_bin_1[48];
static byte txc_reach_frame_1_bin_2[48];
static byte txc_reach_frame_1_bin_3[48];
static byte txc_reach_frame_2_bin_0[48];
static byte txc_reach_frame_2_bin_1[48];
static byte txc_reach_frame_2_bin_2[48];
static byte txc_reach_frame_2_bin_3[48];

static qword txc_reach_current_frame_number;
static qword txc_reach_current_frame_pcg;

static qword txc_reach_next_frame_pcg;
static qword txc_reach_bkoff_frame_pcg;

static qword txc_reach_preamble_frame_pcg;
static qword txc_reach_data_frame_pcg;

static qword txc_reach_frame_pcg;

/* Used to hold channel gains calculated when TXC_REACH_F
   or TXC_REACH_PROBE_F commands are processed */
static muxmdsp_channel_gains_type txc_reach_gains;

/* for logging REACH channel info */
static mux_reach_frame_info_log_type  txc_reach_frame_info_log;

#endif // FEATURE_IS2000_REL_A_CC

byte after_fo_handoff_count_isrs = NO_OF_ISRS_AFTER_FO;

/* Specific variables for the Access Channel */
txac_type txac;

#define TXC_ACCESS_SLIP 1

mux_log_acc_info_type acc_log;

LOCAL void txc_acc_probe_suspend(txc_cmd_type * cmd_ptr);
boolean txc_suspend_after_ta = FALSE;

/* Frequency error which is used to fix non-linear access probes
 * in ideal lab conditions.  This error adds the value 3 to the LSB's
 * of the frequency tracking loop PDM. */
#define TXC_ACCESS_FREQ_ERR (3<<7)

#define TXC_ACC  88 /* Can send 88 information bits each Access Channel frame */

/* Duration of wait in TXC_HOLD_ENC_CLK_S state before timing out and turning off ENC clocks.
** Delay in units of 20ms. 100 = 2secs.
*/
#define TXC_HOLD_ENC_CLK_WAIT_DURATION (100)
typedef struct
{
  boolean txc_sleep_allowed;
  volatile word counter; /* accessed in interrupt and task context */

} txc_delay_enc_shutdown_type;
static txc_delay_enc_shutdown_type txc_delay_enc_shutdown = {TRUE, 0};
static volatile boolean txc_dbg_enable_enc_hold = TRUE;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
void txc_tx_disable( void );
void txc_acc_probe_prep( txc_cmd_type *cmd_ptr );
void txc_acc_msg_prep( txc_cmd_type *cmd_ptr );
void txc_access_isr
(
  m1x_stmr_event_type
);
void txc_hold_enc_clk_isr
(
   m1x_stmr_event_type pcg
);
void txc_hold_enc_clk_init( void );

#ifdef FEATURE_IS2000_REL_A_CC
void txc_reach_msg_prep( txc_cmd_type *cmd_ptr );
void txc_reach_isr_process_preamble( m1x_stmr_event_type );
void txc_reach_isr( m1x_stmr_event_type );
void txc_reach_probe_prep( txc_cmd_type *cmd_ptr );
#endif /* FEATURE_IS2000_REL_A_CC */

/*===========================================================================

FUNCTION TXC_TX_DISABLE

DESCRIPTION
  This function turns the power amp off and sets the channel gains to zero.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_tx_disable( void )
{
  /*-----------------------------------------------------------------------*/

  /* Make sure power amp is off */
  enc_tx_enable( FALSE );

  /* Set the channel gains to zero */
  muxmdsp_set_channel_gains_zero();

} /* txc_tx_disable */

#ifdef FEATURE_IS2000_REL_A_CC
/*===========================================================================

FUNCTION TXC_LOG_REACH_MSG

DESCRIPTION
  This function logs messages sent on the REACH channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  txc_log_reach_msg
(
  txc_msg_buf_type *msg_ptr   /* pointer to sent message */
)
{
  word msg_len;

  mux_msg_log_type *log_ptr;   /* pointer to buffer to log msg in */

  msg_len = msg_ptr->msg[0];
  /* Correct msg_len in case of extended message length */
  if (msg_len & 0x0080)
  {
    msg_len &= 0x007F; /* Turn off extended message indicator */
    msg_len <<= 8;
    msg_len |= msg_ptr->msg[1];
  }

  log_ptr = (mux_msg_log_type *) log_alloc_ex (LOG_REACH_SIGNALING_C, msg_len +
                                                         sizeof(log_hdr_type));
  if (log_ptr != NULL)
  {
    /* copy reach msg packet  and send out*/
    (void) memscpy( (void *)log_ptr->msg,
                    msg_len,
                    msg_ptr->msg,
                    msg_len);
    log_commit(log_ptr);
  }

} /* txc_log_reach_msg */
#endif /* FEATURE_IS2000_REL_A_CC */

/*===========================================================================

FUNCTION TXC_LOG_AC_MSG

DESCRIPTION
  This function logs messages sent on the access channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  txc_log_ac_msg
(
  txc_msg_buf_type *msg_ptr   /* pointer to sent message */
)
{
  mux_log_ac_msg_type *log_ptr;   /* pointer to buffer to log msg in */

  log_ptr = (mux_log_ac_msg_type *) log_alloc (LOG_AC_MSG_C, msg_ptr->msg[0] +
                                                         sizeof(log_hdr_type));
  if (log_ptr != NULL)
  {
    /* log vocoder packet */
    (void) memscpy( (void *)log_ptr->msg,
                    msg_ptr->msg[0],
                    msg_ptr->msg,
                    msg_ptr->msg[0]);
    log_commit(log_ptr);
  }

  /* Event report: Message transmitted */
  mclog_report_event_msg_txed(CAI_ACH, msg_ptr->msg);

} /* txc_log_ac_msg */

/*===========================================================================

FUNCTION TXC_SEC_RAN_NEXT

DESCRIPTION
  This function generates secure random numbers.

DEPENDENCIES
  None.

RETURN VALUE
  32 bit random number

SIDE EFFECTS
  None.

===========================================================================*/
uint32 txc_sec_ran_next
(
  uint32 default_ran   /* Default value to be returned if HW block fails */
)
{
  /* Variable to store more secure random number for access */
  uint32 acc_sec_ran = 0;

  /* Status of secure random number generation */
  secerrno_enum_type acc_sec_ran_status;

  acc_sec_ran_status = secapi_get_random ( SECAPI_SECURE_RANDOM,
                                           (uint8 *)&acc_sec_ran,
                                           sizeof( acc_sec_ran ) );

  if( acc_sec_ran_status != E_SUCCESS )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Secure API failed due to %d, Defaulting to %d",
      acc_sec_ran_status,
      default_ran);

    acc_sec_ran = default_ran;
  }

  return( acc_sec_ran );

} /* txc_sec_ran_next */

/*===========================================================================

FUNCTION TXC_COMPUTE_ACCESS_CHAN

DESCRIPTION
  This function generates a random access channel number and then builds
  an Access Channel long code mask based on that access channel.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_compute_access_chan( void )
{
  dword lo_mask;      /* Lower dword of Access Channel mask */
  dword hi_mask;      /* Upper dword of Access Channel mask */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* -----------------------------------------------------------------
  ** Before transmitting each access probe sequence, generate a random
  ** number, RA, from 0 to ACC_CHANs.  Use RA as the Access Channel
  ** number for all access probes in that access probe sequence.
  ** ----------------------------------------------------------------- */
  txac.ra = (byte) ran_dist( txc_sec_ran_next( 0 ), 0, txac.acc_chan+1 );

  /* ---------------------------------------------------------
  ** Build Access Channel Long Code Mask (see IS-95 6.1.3.1.8)
  ** --------------------------------------------------------- */
  lo_mask =  ((dword) (txac.ra & 0xF) << 28);
  lo_mask |= ((dword) (txac.pagech & 0x7) << 25);
  lo_mask |= ((dword) (txac.base_id & 0xFFFF) << 9);
  lo_mask |=  txac.pilot_pn & 0x000001FF;

  hi_mask  = ((dword) (txac.ra & 0x10) >> 4);
  hi_mask |= ((dword) (0x18F << 1));

  /* Save Access Channel long code mask */
  qw_set( txc_tx.buf.mask,  hi_mask,  lo_mask );

  //txc_reach_on

  /* The information related to encryption/decryption and other
   * key related information will be logged only if the below
   * API retuns TRUE. This is to avoid key information leakage
   * and other potential threats. */ 
   
  if(mmgsdi_is_test_mode_logging_enabled_for_subscription(mcc_asid) == TRUE)
  {
    M1X_MSG( MUX, LEGACY_MED,
      "RA = %d, enc_hi_mask = 0x%X, enc_lo_mask = 0x%X",
      txac.ra,
      hi_mask,
      lo_mask );
  }
  else
  {
    TX_DBG_MSG("Disabled logging of secured information");
  }

  /* For 6500 and later targets, the user PN mask can be programmed
  ** immediately after it's computed.
  */

  /* Load user PN mask  */
  enc_user_pn_mask(txc_tx.buf.mask);

} /* txc_compute_access_chan */

#ifdef FEATURE_IS2000_REL_A_CC
/*===========================================================================

FUNCTION TXC_REACH_COMPUTE_ACCESS_CHAN

DESCRIPTION
  This function generates a random access channel number and then builds
  an Access Channel long code mask based on that access channel.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_reach_compute_access_chan( void )
{
  dword lo_mask;      /* Lower dword of Access Channel mask */
  dword hi_mask;      /* Upper dword of Access Channel mask */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* -----------------------------------------------------------------
  ** Before transmitting each access probe sequence, generate a random
  ** number, RA, from 0 to ACC_CHANs.  Use RA as the Access Channel
  ** number for all access probes in that access probe sequence.
  ** ----------------------------------------------------------------- */

  txac.ra = (byte) ran_dist( txc_sec_ran_next( 0 ), 0, txac.acc_chan+1 );

  lo_mask =  ((dword) (txac.ra & 0xF) << 28);
  lo_mask |= ((dword) (txac.pagech & 0x7) << 25);
  lo_mask |= ((dword) (txac.base_id & 0xFFFF) << 9);
  lo_mask |= ((dword) txc_reach_slot_offset) & 0x000001FF;

  hi_mask  = ((dword) (txac.ra & 0x10) >> 4);
  hi_mask |= ((dword) (0x18E << 1));


  /* Save Access Channel long code mask */
  qw_set( txc_tx.buf.mask,  hi_mask,  lo_mask );


  //txc_reach_on

  /* The information related to encryption/decryption and other
   * key related information will be logged only if the below
   * API retuns TRUE. This is to avoid key information leakage
   * and other potential threats. */ 
   
  if(mmgsdi_is_test_mode_logging_enabled_for_subscription(mcc_asid) == TRUE)
  {
    M1X_MSG( MUX, LEGACY_MED,
      "RA = %d, enc_hi_mask = 0x%X, enc_lo_mask = 0x%X",
      txac.ra,
      hi_mask,
      lo_mask );
  }
  else
  {
    TX_DBG_MSG("Disabled logging of secured information");
  }

  /* For 6500 and later targets, the user PN mask can be programmed
  ** immediately after it's computed.
  */

  /* Load user PN mask  */
  enc_user_pn_mask(txc_tx.buf.mask);

} /* txc_reach_compute_access_chan */
#endif // FEATURE_IS2000_REL_A_CC

/*===========================================================================

FUNCTION TXC_ACCESS_IDL_INIT

DESCRIPTION
  Initialization section for access channel.

DEPENDENCIES
  None
RETURN VALUE
  None.
SIDE EFFECTS
  None
===========================================================================*/
void txc_access_idl_init
(
  txc_state_type      state           /* state we're coming from */
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  txac.frame.dotx = FALSE;        /* don't transmit this frame */

  /* Make Lint happy */
  MODEM_1X_UNUSED(after_fo_handoff_count_isrs);

  /* Should really initialize jump_source to default value here since even
  ** the pre jump command code has not received a command to jump at this
  ** time.  This init code is called upon reception of TXC_PC_F only (hence
  ** I changed the function name to clarify that fact.)
  */
  txc_tx.jump_source = TXC_READY_TO_JUMP;

  if(state == TXC_TT_S)
  {
    txc_tx.time_trans = TRUE;
  }
  else
  {
    txc_tx.time_trans = FALSE;
  }

  /* Init counters to default, allow sleep since RL timing is not yet required.
  */
  txc_delay_enc_shutdown.counter = 0;
  txc_delay_enc_shutdown.txc_sleep_allowed = TRUE;

  txc_suspend_after_ta = FALSE;

} /* txc_access_idl_init */


#ifdef FEATURE_IS2000_REL_A_CC
/*===========================================================================

FUNCTION  TXC_COPY_REACH_PARAMETERS

DESCRIPTION
  Copies parameters from the TXC_REACH_F or TXC_REACH_PROBE_F command into
  the txac structure.

DEPENDENCIES
  None
RETURN VALUE
  None.
SIDE EFFECTS
  Modifies global variables, including txac.
===========================================================================*/
void txc_copy_reach_parameters
(
  txc_cmd_type *cmd_ptr            /* Pointer to received command */
)
{
  int i;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  txac.bkoff = cmd_ptr->reach.bkoff;              /* Sequence backoff */
  txac.probe_bkoff = cmd_ptr->reach.probe_bkoff;  /* Probe backoff */
  txac.acc_tmo = cmd_ptr->reach.acc_tmo;          /* Ack timeout */
  txac.max_seq = cmd_ptr->reach.seq_num;          /* Max probe sequences */
  txac.req = cmd_ptr->reach.req;                  /* Request or response */

  /* -----------------------------------------
  ** Calculate initial power offset for access
  ** ----------------------------------------- */
  if ((cmd_ptr->reach.init_pwr & 0x10))  /* Sign bit (5 bit field) */
  {
    /* Negative value --> sign extend to 8 bits */
    txac.power = (byte) (cmd_ptr->reach.init_pwr | 0xE0);
  }
  else
  {
    txac.power = (cmd_ptr->reach.init_pwr & 0x1F);
  }

  /* -----------------------------------------------------------------
  ** Add in nom_pwr which is already sign extended and, if necessary,
  ** adjusted according to nom_pwr_ext.
  ** ----------------------------------------------------------------- */
  txac.power += cmd_ptr->reach.nom_pwr;

  /* Invert to turn power adjustment into tx gain adjustment */
  txac.power = -(txac.power);  //lint !e732: Loss of sign (sign maintained manually)

  /* Convert to units of 1/2 dB */
  // Value can be at most 7 bits after multiplication.
  txac.power = txac.power * 2; //lint !e734: Loss of precision (9 to 8 bits)

  txac.init_pwr = txac.power;

  /* initialize the is95a power accumulation */
  txac.is95a_pwr = txac.init_pwr;

  /* Grab power step from message in 1dB steps */
  txac.pstep = (byte) (cmd_ptr->reach.pwr_step);

  /* Convert to units of 1/2 dB */
  // pwr_step is 3 bits, so product must be no more than 4 bits.
  txac.pstep = txac.pstep * 2; //lint !e734: Loss of precision (9 to 8 bits)

  txac.nstep = cmd_ptr->reach.num_step;      /* Number of access probes */
  txac.p = cmd_ptr->reach.p;                 /* Persistence test value */
  txac.acc_chan = cmd_ptr->reach.num_each_ba;   /* Number of access channels */
  txac.pagech   = cmd_ptr->reach.fccch;       /* Current Paging Channel */
  txac.base_id  = cmd_ptr->reach.base_id;    /* Base station identification */
  txac.pilot_pn = cmd_ptr->reach.pilot_pn;   /* Pilot PN seq offset index */
  txc_tx.buf.cmd_ptr = NULL;    /* Don't save command info */

  /* -----------------------------------------------------------------
  ** Before transmitting each access probe sequence, generate a random
  ** number, RA, from 0 to ACC_CHANs.  Use RA as the Access Channel
  ** number for all access probes in that access probe sequence.
  ** ----------------------------------------------------------------- */
  txc_reach_compute_access_chan();

  txc_reach_slot_length = (word) (cmd_ptr->reach.slot + 1);

  txc_reach_slot_offset1 = (word) (cmd_ptr->reach.slot_offset1);
  txc_reach_slot_offset2 = (word) (cmd_ptr->reach.slot_offset2);

// T = N(P+B)+A = N ( P + 0 ) + 0
// T = NP = (preamble_num_frac + 1)(preamble_frac_duration + 1)
// preamble_enabled = true
// preamble_num_frac = 0
// preamble_frac_duration = 3
// preamble_off_duration = 0
// preamble_add_duration = 0
// T = (preamble_num_frac + 1)(preamble_frac_duration + 1)
// T = (0 + 1)( 3 + 1 ) = 1 * 4 = 4

  txc_reach_preamble_length =
  (word)((cmd_ptr->reach.preamble_num_frac + 1) * (cmd_ptr->reach.preamble_frac_duration + 1));


  txc_reach_rlgain_common_pilot = cmd_ptr->reach.rlgain_common_pilot;

  // IC_THRESH from the base station in units of 1/2 dB
  txc_reach_ic_thresh = cmd_ptr->reach.ic_thresh;

  // IC_MAX from the base station in units of  1/2 dB
  txc_reach_ic_max = cmd_ptr->reach.ic_max;

  // IC_THRESH in units of 1/2 dB for interference correction calculation
  // ic_thresh is 4 bit value.  Product must be no more than 5.
  txc_reach_ic_thresh = txc_reach_ic_thresh * 2;  //lint !e734: Loss of precision (9 to 8 bits)

  // IC_MAX in units of  1/2 dB for interference correction calculation
  // ic_max is 4 bit value.  Product must be no more than 5.
  txc_reach_ic_max = txc_reach_ic_max * 2; //lint !e734: Loss of precision (9 to 8 bits)


  M1X_MSG( MUX, LEGACY_MED,
    "rlgain=%x ic_thresh=%x ic_max=%x",
    txc_reach_rlgain_common_pilot,
    txc_reach_ic_thresh,
    txc_reach_ic_max);


  for( i = 0; i < ENC_SCH_RC3_16X_SIZE; i++ )
  {
    txtc.supp_frame.data[ i ] = 0x5A;
  }


  switch ( cmd_ptr->reach.rate )
  {
    case TXC_EACH_RATE_9600_20_V:
      txc_reach_frame_length = ENC_SCH_FRAME_SIZE_20_V;
      txtc.supp_rate = ENC_SCH_FULL_RATE;
      txc_reach_frame_size = 172; /* 20ms, 9600bps, 172 information bits */
      break;

    case TXC_EACH_RATE_19200_20_V:
      txc_reach_frame_length = ENC_SCH_FRAME_SIZE_20_V;
      txtc.supp_rate = ENC_SCH_2X_RATE;
      txc_reach_frame_size = 360; /* 20ms, 19200bps, 360 information bits */
      break;

    case TXC_EACH_RATE_19200_10_V:
      txc_reach_frame_length = ENC_SCH_FRAME_SIZE_10_V;
      txtc.supp_rate = ENC_SCH_FULL_RATE;
      txc_reach_frame_size = 172; /* 10ms, 19200bps, 172 information bits */
      break;

    case TXC_EACH_RATE_38400_20_V:
      txc_reach_frame_length = ENC_SCH_FRAME_SIZE_20_V;
      txtc.supp_rate = ENC_SCH_4X_RATE;
      txc_reach_frame_size = 744; /* 20ms, 38400bps, 744 information bits */
      break;

    case TXC_EACH_RATE_38400_10_V:
      txc_reach_frame_length = ENC_SCH_FRAME_SIZE_10_V;
      txtc.supp_rate = ENC_SCH_2X_RATE;
      txc_reach_frame_size = 360; /* 10ms, 38400bps, 360 information bits */
      break;

    case TXC_EACH_RATE_38400_5_V:
      txc_reach_frame_length = ENC_SCH_FRAME_SIZE_5_V;
      txtc.supp_rate = ENC_SCH_FULL_RATE;
      txc_reach_frame_size = 172; /* 5ms, 38400bps, 172 information bits */
      break;
  }

  txc_reach_rate_word = cmd_ptr->reach.rate;

  /* Get REACH data gain based on the rate word for data channel. */
  txc_reach_gains.txc_sch_gain = rpc_get_reach_data_gain( cmd_ptr->reach.rate );

  /* Get REACH pilot gain based on the rate word for data channel. */
  txc_reach_gains.txc_pch_gain = rpc_get_reach_pilot_gain( cmd_ptr->reach.rate );

  /* Get REACH pilot gain adjustment for TX_ADJ based on the pilot gain. */
  txc_reach_gains.txc_tx_rate_adj = rpc_get_pch_adj_gain(PCH_GAIN_TRUNCATE_TO_8BITS(
                                                txc_reach_gains.txc_pch_gain));

} /* txc_copy_reach_parameters */
#endif /* FEATURE_IS2000_REL_A_CC */

/*===========================================================================

FUNCTION TXC_ACCESS_CMD

DESCRIPTION
  Process commands which are common to all states of the idle state machine.

DEPENDENCIES
  None
RETURN VALUE
  Next state to be processed.
SIDE EFFECTS
  None
===========================================================================*/
txc_state_type txc_access_cmd
(
  txc_cmd_type *cmd_ptr,            /* Pointer to received command */
  txc_state_type state              /* Current state */
)
{
  txc_status_type status;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Interpret the command from the queue */
  switch( cmd_ptr->hdr.command )
  {
    case TXC_JUMP_F :
      M1X_MSG( MUX, LEGACY_MED,
        "Got TXC_JUMP_F, state=%d",
        state );

      enc_change_tx_clk_resources( ENC_TX_CLOCKS_ENABLE );

      /* Enable RF Tx */
      txc_change_rf_tx_state( TXC_RF_STATE_ENABLE );

      if( state == TXC_ACC_IDLE_S )
      {
        /* Get the PN randomization delay from Jump command.  This value is programmed
        ** during the Jump to hyperspace.
        */
        txac.rn = cmd_ptr->jump.rn;               /* PN randomization delay */
        txac.hold_enc_clk_on = cmd_ptr->jump.hold_enc_clk_on;  /* Hold encoder clock on */

        /* Set correct state and init duration of wait in TXC_HOLD_ENC_CLK_S state
        ** to default value.
        */
        txc_delay_enc_shutdown.counter = 0;
        state = TXC_ACC_JUMP_S;

        /* Jump source is set to TXC_JUMP for 2 cases:
        **   In TXC_HOLD_ENC_CLK_S state, the jump is complete.
        **   In TXC_ACC_JUMP_S state, the jump is in progress.
        */
        txc_tx.jump_source = TXC_JUMP;

        /* Set up Hold ENC clocks ISR.
        */
        txc_hold_enc_clk_init();
      }
      /* Do not change state if MS is already done with Jump.
      ** Do restart Jump timer.
      */
      else if( state == TXC_HOLD_ENC_CLK_S )
      {
        M1X_MSG( MUX, LEGACY_HIGH,
          "Rxed TXC_JUMP_F, old timer=%d",
          txc_delay_enc_shutdown.counter );

        TXC_ENTER_ISR_SECTION();
        /* Here, we are already holding the ENC clocks ON.  So just update the counter
        ** so we will extend the timer starting from this point.
        ** Int lock is required because txc_delay_enc_shutdown.counter is accessed in interrupt context.
        ** If the count down has reached the end, do not restart the count down as it's is safer
        ** not to interrupt the processing of TXC_TX_SHUTDOWN_SIG at this point.
        */
        if( txc_delay_enc_shutdown.counter > 0 )
        {
          txc_delay_enc_shutdown.counter = TXC_HOLD_ENC_CLK_WAIT_DURATION;
        }
        TXC_LEAVE_ISR_SECTION();
      }
      else if( state == TXC_ACC_JUMP_S )
      {
        M1X_MSG( MUX, LEGACY_HIGH,
           "TXC_JUMP_F ignored in state TXC_ACC_JUMP_S" );
      }
      else
      {
        M1X_MSG( MUX, LEGACY_ERROR,
           "Unexpected TXC_JUMP_F received in state %d",
           state );
      }

      status = TXC_DONE_S;
      break;

    case TXC_ACCESS_F :
      M1X_MSG( MUX, LEGACY_MED,
        "Got TXC_ACCESS_F, state=%d",
        state );

      /* Set PCG mask to 0xFFFF, to avoid FW using previous
         DTX mask and cause invalid data to be sent to BS */
      txc_device_only_dtx_param.dtx_frame = FALSE;
      txc_device_only_dtx_param.dtx_mask  = DTX_SB_MASK_DISABLED;

      /* Turn on TX clock regime here as well since TXC_JUMP_F
         may have been skipped. */
      enc_change_tx_clk_resources( ENC_TX_CLOCKS_ENABLE );

      /* We may be in TXC_ACC_JUMP_S state due to receiving the TXC_JUMP_F
      ** command from MC.  By changing the jump_source to TXC_JUMP_ACCESS
      ** here, we still transition to the correct state at the end of the jump.
      */
      txc_tx.jump_source = TXC_JUMP_ACCESS;

      ASSERT(txac.state != ACC_SUSPEND_S);

      /* Prepare to send Access Channel message */
      txc_acc_msg_prep( cmd_ptr );

      #ifdef FEATURE_IS2000_REL_A_CC
      txc_reach_access = FALSE;
      #endif // FEATURE_IS2000_REL_A_CC

      /* Initialize the stop_probes to FALSE */
      txac.stop_probes = FALSE;

      /* Enable RF Tx */
      txc_change_rf_tx_state( TXC_RF_STATE_ENABLE );

      /* Zero out RL frame offset for access */
      enc_set_frame_off( 0, 0 );
      txc_set_frame_offset( 0 );

      /* Do not change state if MS has already started Jump or is
      ** already done with Jump.
      */
      if( ( state != TXC_ACC_JUMP_S ) && ( state != TXC_HOLD_ENC_CLK_S ) )
      {
        /* Set correct state and init duration of wait in TXC_HOLD_ENC_CLK_S
        ** state to default value.
        */
        M1X_MSG( MUX, LEGACY_HIGH,
           "No preceding TXC_JUMP_F, or late TXC_ACCESS_F" );

        txc_delay_enc_shutdown.counter = 0;
        state = TXC_ACC_JUMP_S;

        /* Set up Hold ENC clocks ISR */
        txc_hold_enc_clk_init();
      }
      status = TXC_DONE_S;
      break;


    #ifdef FEATURE_IS2000_REL_A_CC
    case TXC_REACH_F :
      M1X_MSG( MUX, LEGACY_MED,
        "Got TXC_REACH_F, state=%d",
        state );

      /* Set PCG mask to 0xFFFF, to avoid FW using previous
         DTX mask and cause invalid data to be sent to BS */
      txc_device_only_dtx_param.dtx_frame = FALSE;
      txc_device_only_dtx_param.dtx_mask  = DTX_SB_MASK_DISABLED;

      /* Turn on TX clock regime here as well since TXC_JUMP_F
         may have been skipped. */
      enc_change_tx_clk_resources( ENC_TX_CLOCKS_ENABLE );

      ASSERT(txac.state != ACC_SUSPEND_S);

      /* Prepare to send Access Channel message */
      txc_reach_msg_prep( cmd_ptr );

      /* We may be in TXC_ACC_JUMP_S state due to receiving the TXC_JUMP_F
      ** command from MC.  By changing the jump_source to TXC_JUMP_ACCESS
      ** here, we still transition to the correct state at the end of the jump.
      */
      txc_tx.jump_source = TXC_JUMP_ACCESS;

      txc_reach_access = TRUE;

      // First sub-attempt
      txc_reach_sub_att_number = 0;

      /* Enhanced Access Channel Open Loop Power Offsets */
      txtc.offset_pwr = (byte) 8.5 * 12;

      /* Set channel gains to zero */
      muxmdsp_set_channel_gains_zero();

      /* Initialize the stop_probes to FALSE */
      txac.stop_probes = FALSE;

      /* Enable RF Tx */
      txc_change_rf_tx_state( TXC_RF_STATE_ENABLE );

      /* Zero out RL frame offset for access */
      enc_set_frame_off( 0, 0 );
      txc_set_frame_offset( 0 );

      /* Do not change state if MS has already started Jump or is
      ** already done with Jump.
      */
      if( ( state != TXC_ACC_JUMP_S ) && ( state != TXC_HOLD_ENC_CLK_S ) )
      {
        /* Set correct state and init duration of wait in TXC_HOLD_ENC_CLK_S
        ** state to default value.
        */
        M1X_MSG( MUX, LEGACY_HIGH,
          "No preceding TXC_JUMP_F, or late TXC_REACH_F" );

        txc_delay_enc_shutdown.counter = 0;
        state = TXC_ACC_JUMP_S;

        /* Set up Hold ENC clocks ISR */
        txc_hold_enc_clk_init();
      }
      status = TXC_DONE_S;
      break;

    case TXC_REACH_PROBE_F :
      M1X_MSG( MUX, LEGACY_MED,
        "Got TXC_REACH_PROBE_F, state=%d",
        state );

      /* This command is not valid for the TXC_HOLD_ENC_CLK_S state */
      if( state == TXC_HOLD_ENC_CLK_S )
      {
        status = TXC_BADCMD_S;
        break;
      }

      /* Check if new base_id is different from current.
       * If so, this is a access handoff.  Increment the
       * subattempt count.
       *
       *** Check this value before txac.base_id is updated ***/
      if( cmd_ptr->reach.base_id != txac.base_id )
      {
        // This is an access probe handoff.
        ++txc_reach_sub_att_number;
      }

      if( txac.state == ACC_SUSPEND_S )
      {
        txc_copy_reach_parameters( cmd_ptr );
        /* Do not reset the sequence number.  See LAC 2.1.1.2.2.2. */
        //txac.seq_num = 0;                      /* Sequence number */
        txac.num_step    = txac.nstep;           /* # of access probes remaining */
        txac.probe_count = 0;                    /* Access probe number */
        txac.first_frame = FALSE;                /* Not first frame */
        txc_acc_probe_resume();
        txac.stop_probes = FALSE;
      }

      /* Prepare Reverse Enhanced Access Channel Message. */
      txc_reach_probe_prep( cmd_ptr );

      status = TXC_DONE_S;
      break;
    #endif // FEATURE_IS2000_REL_A_CC

    case TXC_ACC_PROBE_F :
      M1X_MSG( MUX, LEGACY_MED,
        "Got TXC_ACC_PROBE_F, state=%d",
        state );

      /* This command is not valid for the TXC_HOLD_ENC_CLK_S state */
      if( state == TXC_HOLD_ENC_CLK_S )
      {
        M1X_MSG( MUX, LEGACY_MED,
           "Invalid TXC_ACC_PROBE_F in TXC_HOLD_ENC_CLK state");
        status = TXC_BADCMD_S;
        break;
      }
      /* Ignore this command if TXC is in IDLE state */
      else if( state == TXC_ACC_IDLE_S )
      {
        M1X_MSG( MUX, LEGACY_MED,
           "Ignoring TXC_ACC_PROBE_F in TXC_ACC_IDLE_S state");
        status = TXC_DONE_S;
        break;
      }

      if( txac.state == ACC_SUSPEND_S )
      {
        txac.preambles   = cmd_ptr->acc.pam_sz + 1;
        txac.slotting    = (cmd_ptr->acc.pam_sz + 1) + (3 + cmd_ptr->acc.max_cap_sz);
        txac.bkoff       = cmd_ptr->acc.bkoff;
        txac.probe_bkoff = cmd_ptr->acc.probe_bkoff;
        txac.acc_tmo     = cmd_ptr->acc.acc_tmo;
        txac.max_seq     = cmd_ptr->acc.seq_num;
        txac.num_step    = cmd_ptr->acc.num_step;
        /* -----------------------------------------
        ** Calculate initial power offset for access
        ** ----------------------------------------- */
        if( ( cmd_ptr->acc.init_pwr & 0x10 ) )  /* Sign bit (5 bit field) */
        {
          /* Negative value --> sign extend to 8 bits */
          txac.power = (byte) (cmd_ptr->acc.init_pwr | 0xE0);
        }
        else
        {
          txac.power = (cmd_ptr->acc.init_pwr & 0x1F);
        }
        /* -----------------------------------------------------------------
        ** Add in nom_pwr which is already sign extended and, if necessary,
        ** adjusted according to nom_pwr_ext.
        ** ----------------------------------------------------------------- */
        txac.power += cmd_ptr->acc.nom_pwr;

        /* Invert to turn power adjustment into tx gain adjustment */
        txac.power = -(txac.power);  //lint !e732: Loss of sign (Sign maintained manually)

        /* Convert to units of 1/2 dB */
        // Product can only be 5 bits wide
        txac.power = txac.power * 2; //lint !e734: Loss of precision (9 to 8 bits)

        txac.init_pwr = txac.power;

        /* initialize the is95a power accumulation */
        txac.is95a_pwr = txac.init_pwr;

        /* Grab power step from message in 1dB steps */
        txac.pstep = (byte) (cmd_ptr->acc.pwr_step);

        /* Convert to units of 1/2 dB */
        // Product can only be 4 bits wide
        txac.pstep = txac.pstep * 2; //lint !e734 Loss of precision (9 to 8 bits)

        /* Get the new Masking parameters from the Access Channel Message */
        txac.rn       = cmd_ptr->acc.rn;         /* PN randomization delay */
        txac.p        = cmd_ptr->acc.p;          /* Persistence test value */
        txac.acc_chan = cmd_ptr->acc.acc_chan;   /* Number of access channels */
        txac.use_chan = cmd_ptr->acc.use_chan;   /* Use specific channel */
        txac.pagech   = cmd_ptr->acc.pagech;     /* Current Paging Channel */
        txac.base_id  = cmd_ptr->acc.base_id;    /* Base station identification */
        txac.pilot_pn = cmd_ptr->acc.pilot_pn;   /* Pilot PN seq offset index */
        txc_acc_probe_resume();
        txac.stop_probes = FALSE;
      }

      /* Prepare to send Access Channel message */
      txc_acc_probe_prep( cmd_ptr );

      status = TXC_DONE_S;
      break;

    case TXC_ACC_SUSPEND_F :
      M1X_MSG( MUX, LEGACY_MED,
        "Got TXC_ACC_SUSPEND_F, state=%d",
        state );

      /* This command is not valid for the TXC_HOLD_ENC_CLK_S state */
      if( state == TXC_HOLD_ENC_CLK_S )
      {
        status = TXC_BADCMD_S;
        break;
      }
      txc_acc_probe_suspend( cmd_ptr );
      status = TXC_DONE_S;
      break;

    case TXC_TC_F:
    {
      // Default fundicated to FCH RC.  Will be updated with correct value later.
      // Put here to quiet the compiler
      cai_radio_config_type fundicated_rc = cmd_ptr->tc.rev_fch.rc;
      boolean               ta_is_enabled = txc_is_ta_enabled();

      M1X_MSG( MUX, LEGACY_MED,
        "Got TXC_TC_F, state=%d",
        state );

      if( ta_is_enabled )
      {
        txc_ta_info.ta_tc_init_done = TRUE;
        M1X_MSG( MUX, LEGACY_HIGH,
           "1x2GTA: TC init done %d",
           txc_ta_info.ta_tc_init_done );

        /* De-register TX traffic events before updating frame offset */
        m1x_stmr_deregister_client( M1X_STMR_CLIENT_TX );
      }

      M1X_MSG( MUX, LEGACY_HIGH,
         "1x2GTA: TC init done : %d",
         txc_ta_info.ta_tc_init_done );

      #ifdef FEATURE_IS2000_1X_ADV
      /* For 1x Advanced DCCH is not supported R-FCH.
        If it is included at the same time that R-FCH is then we must
        confirm that R-FCH RC and R-DCCH are RC8.
      */
      if( cmd_ptr->tc.rev_dcch.included )
      {
        if( ( cmd_ptr->tc.rev_dcch.rc == CAI_RC_8 ) ||
            ( ( cmd_ptr->tc.rev_fch.included ) &&
              ( cmd_ptr->tc.rev_fch.rc == CAI_RC_8 ) ) )
        {
          M1X_MSG( MUX, LEGACY_FATAL,
            "Illegal channel configuration: FCH-> inc=%d, rc=%d DCCH-> rc=%d",
            cmd_ptr->tc.rev_fch.included,
            cmd_ptr->tc.rev_fch.rc,
            cmd_ptr->tc.rev_dcch.rc);
        }
      }

      txc_process_rcpm_event( txc_rcpm_data, cmd_ptr->rcp.rcp_data );

      /* Save local copies of RCPM data
         We will process the data prior to entering traffic (txc_traffic_init())
      */
      txc_rcpm_data = cmd_ptr->tc.rcp_init;

      /* Reset the Smart Blanking flag to prevent using the wrong
         RPC mode/step initially. */
      txtc.rc8_status.blank_non_crit_fch_frame = FALSE;
      #endif /* FEATURE_IS2000_1X_ADV */

      /* Ensure TX clock regime is on, as it could be off if it's been
         too long since we did our last jump to hyperspace.  */
      enc_change_tx_clk_resources( ENC_TX_CLOCKS_ENABLE );

      /* Enable RF Tx */
      txc_change_rf_tx_state( TXC_RF_STATE_ENABLE );

      /* Set traffic RL frame offset */
      txc_frame_offset_cmd.frame_offset = cmd_ptr->tc.frame_offset;
      enc_set_frame_off( cmd_ptr->tc.frame_offset, cmd_ptr->tc.frame_offset );
      txc_set_frame_offset( cmd_ptr->tc.frame_offset );
      rxc_set_frame_offset( cmd_ptr->tc.frame_offset );

      /* If still in LTA on Tx side, then update follwing params to FW */
      if( ta_is_enabled )
      {
        muxmdsp_set_tx_gain_adj_step_size( txc_ta_info.tx_gain_adj_step_size );

        /* For LTA, re-register traffic ISR for new frame offset */
        txc_setup_pcg_events_isr( TXC_PCG_EVENT_TRAFFIC, txc_traffic_isr );
      }

      /*
       * Make sure that we don't try to transmit anything before the TC is
       * ready to transmit the preambles.
      */
      txtc.ready_to_transmit = FALSE;

      txc_tt_turn_on_transmitter = cmd_ptr->tc.turn_on_transmitter;

      qw_equ( txc_tx.mask, cmd_ptr->tc.mask );
        /* store initial rate set to use */

      // Default to FCH for signalling
      txtc.dsch_map = txtc.cur_dsch = TXC_REV_FCH;

      if( cmd_ptr->tc.rev_dcch.included )
      {
        txc_so.rev_dcch.included = cmd_ptr->tc.rev_dcch.included;

        // Before Logical-to-physical mapping is support, just do the default
        // dsch_map specified in IS2000
        if( !cmd_ptr->tc.rev_fch.included )
        {
          txtc.dsch_map = txtc.cur_dsch = TXC_REV_DCCH;
        }

        txc_so.rev_dcch.rc = cmd_ptr->tc.rev_dcch.rc;
        fundicated_rc = cmd_ptr->tc.rev_dcch.rc;
      }
      else
      {
        txc_so.rev_dcch.included = FALSE;
      }

      if( cmd_ptr->tc.rev_fch.included )
      {
        txc_so.rev_fch.included = cmd_ptr->tc.rev_fch.included;
        txc_so.rev_fch.rc = cmd_ptr->tc.rev_fch.rc;
        enc_fch_rc( (enc_fch_radio_config_type) txc_so.rev_fch.rc );
        // Enabling of FCH is done in txctraffic later
      }
      else
      {
        txc_so.rev_fch.included = FALSE;
      }

      #ifdef FEATURE_IS2000_REL_A_SVD
      /* Choose either FCH or DCCH as signaling channel.
      ** FCH given priority over DCCH.
      */
      if( txc_so.rev_fch.included )
      {
        if( cmd_ptr->tc.signaling_phy_channel & MCTRANS_PHY_CHAN_FCH )
        {
          txtc.dsch_map = txtc.cur_dsch = TXC_REV_FCH;
        }
      }
      else if( txc_so.rev_dcch.included &&
               ( cmd_ptr->tc.signaling_phy_channel & MCTRANS_PHY_CHAN_DCCH ) )
      {
        txtc.dsch_map = txtc.cur_dsch = TXC_REV_DCCH;
      }
      else
      {
        //
        // REMOVE AFTER DEBUG
        //
        ONEX_ERR_FATAL("Illegal signaling channel assignment", 0, 0, 0);
      }
      #endif /* FEATURE_IS2000_REL_A_SVD */

      #ifdef FEATURE_IS2000_REL_A
      txtc.p_rev_in_use = cmd_ptr->tc.p_rev_in_use;
      #endif /* FEATURE_IS2000_REL_A */

      if( txc_tx.time_trans )
      {
        txc_tx.jump_source = TXC_JUMP_TT;
      }
      else
      {
        txc_tx.jump_source = TXC_JUMP_TRAFFIC;
      }

      if( cmd_ptr->tc.tcb_ptr != NULL )
      {
        /* Save pointer to task to signal back and signal to use */
        txtc.tcb_ptr = cmd_ptr->tc.tcb_ptr;
        txtc.sigs =    cmd_ptr->tc.sigs;
      }

      if( onex_nonsig_is_ftm_mode() )
      {
        txc_ftm_rfch_rate = cmd_ptr->tc.fch_rate;

        txc_ftm_rdcch_rate = cmd_ptr->tc.dcch_rate;
        txtc.sch_on = cmd_ptr->tc.sch_on;
        if( cmd_ptr->tc.sch_on )
        {
          txtc.supp_rate = cmd_ptr->tc.sch_rate;
          txc_ftm_rsch_rate = cmd_ptr->tc.sch_rate;
          txtc.supp_turbo = cmd_ptr->tc.sch_turbo;
          txtc.supp_ltu_size = cmd_ptr->tc.sch_ltu_size;
        }

        if( txc_so.rev_dcch.included )
        {
          txtc.dcch_loopback = FALSE; /* Initially off */
          txtc.num_preambles = cmd_ptr->tc.num_preambles;
          {
            int i;
            byte *tx_pat_ptr = (byte *)&cmd_ptr->tc.tx_pattern;

            for( i = 0; i < ENC_14400_FULL_RATE_SIZE; i++ )
            {
              txc_ftm_rdcch_data.data[ i ] = tx_pat_ptr[i % 4];
            }
          }
        }

        if( txc_so.rev_fch.included )
        {
          txtc.fch_loopback = FALSE; /* Initially off */
          txtc.num_preambles = cmd_ptr->tc.num_preambles;
          {
            int i;
            byte *tx_pat_ptr = (byte *)&cmd_ptr->tc.tx_pattern;

            for( i = 0; i < ENC_14400_FULL_RATE_SIZE; i++ )
            {
              txc_ftm_rfch_data.data[ i ] = tx_pat_ptr[i % 4];
            }
          }
        }
      }

      if( !onex_nonsig_is_ftm_mode() )
      {
        /* already sign extended. */
        txtc.rl_gain_adj = (int8) cmd_ptr->tc.rl_gain_adj;
      }

      if( fundicated_rc == CAI_RC_1 ||
          fundicated_rc == CAI_RC_2 )
      {
        /* taken care through linearizer tables in RF cal */
        txtc.offset_pwr = 0;
      }
      else  // RC 3 and 4
      {
        /*-------------------------------------------------------
        ** For CELLULAR : -73 - (-81.5),
        ** For PCS      : -76 - (-84.5) = 8.5
        **------------------------------------------------------*/
        txtc.offset_pwr = (byte) 8.5 * 12;
      }

      /* Dont initialize RPC params during LTA GAP */
      if( !ta_is_enabled )
      {
        rpc_tc_cmd( cmd_ptr );

        if( cmd_ptr->tc.rev_fch_gating_on )
        {
          txtc.fch_8th_rate_gating = TRUE;
          // Note: don't need to program the encoder here yet since we
          // need to decide whether to gate or not depending on whether
          // it's a 8th rate frame when we are actually sending it.
          M1X_MSG( MUX, LEGACY_HIGH,
             "Turned ON FCH 1/8th rate gating" );
        }
        else
        {
          txtc.fch_8th_rate_gating = FALSE;
          M1X_MSG( MUX, LEGACY_HIGH,
             "FCH NOT 1/8th rate gated" );
        }

        /* R-FCH gating event has a 1 byte boolean parameter */
        event_report_payload(EVENT_REV_FCH_GATING_IN_USE,
                             sizeof(byte),
                             (void *) &txtc.fch_8th_rate_gating);
      }

      // Turn off pilot gating regardless just in case it was left gated from the
      // last call.  For 1/8 rate gating, it will be taken care of later when sending
      // frames.
      enc_gating_mode( ENC_NO_GATING_MODE );
      enc_is95c_tx_patn( ENC_NO_GATING_PATTERN );

      /* Do not change state if MS has already started Jump or is
      ** already done with Jump.
      */
      if( ( state != TXC_ACC_JUMP_S ) && ( state != TXC_HOLD_ENC_CLK_S ) )
      {
        /* Set correct state and init duration of wait in TXC_HOLD_ENC_CLK_S
        ** state to default value.
        */
        M1X_MSG( MUX, LEGACY_HIGH,
           "No preceding TXC_JUMP_F, or late TXC_TC_F" );
        txc_delay_enc_shutdown.counter = 0;
        state = TXC_ACC_JUMP_S;

        /* Set up Hold ENC clocks ISR.
        */
        txc_hold_enc_clk_init();
      }
      status = TXC_DONE_S;       /* dispose of command buffer */
      break;
    }

    /* Stop transmitting access message NOW */
    case TXC_ACC_STOP_F:
      M1X_MSG( MUX, LEGACY_MED,
        "Got TXC_ACC_STOP_F, state=%d",
        state );

      /* This command is not valid for the idle state */
      if( state == TXC_ACC_IDLE_S )
      {
        status = TXC_BADCMD_S;
      }
      else
      {
        state = TXC_ACC_IDLE_S;
        txc_tx.active_msg = FALSE;  /* Indicate no msg tx in progress */
        txac.frame.dotx   = FALSE;    /* No transmission next interrupt */

        txc_suspend_after_ta = FALSE;

        /* The only purpose of the resetting of txac.state is to satisfy */
        /* an assertion that the TXC_ACCESS_F command can not be sent by */
        /* MC->TXC when TXC is in suspended state. There is no need to   */
        /* have that assertion, other than, it helps in catching MC-TXC  */
        /* interface problems for T72-T40 timer & Access Probe Handoff   */
        /* once the interface is properly stablised, we can remove this  */
        /* statement and the assertion. However, there is no harm in     */
        /* leaving these things as it is, as they will always help in    */
        /* early bug detections.                                         */

        txac.state = ACC_SETUP1_S;

        if( cmd_ptr->stop.tcb_ptr != NULL )
        {
          /* Save pointer to task to signal back and signal to use */
          txac.tcb_ptr = cmd_ptr->stop.tcb_ptr;
          txac.sigs =    cmd_ptr->stop.sigs;
        }

        status = TXC_DONE_S;
      }
      break;

    case TXC_EXIT_F:
      M1X_MSG( MUX, LEGACY_MED,
        "Got TXC_EXIT_F, state=%d",
        state );
      /* exit cdma mode */

      /* In EXIT state, definitely allow sleep */
      txc_delay_enc_shutdown.txc_sleep_allowed = TRUE;

      txc_exit();  /* perform exit processing */
      state  = TXC_EXIT_S;
      status = TXC_DONE_S;
      break;

    case TXC_CDMA_F:
      /* --------------------------------------------------
      ** Transition back to CDMA init state without exiting
      ** txc_subtask()
      ** -------------------------------------------------- */

      /* ----------------------------------------------------
      ** Set interrupt handler to null and turn off TX clocks
      ** ---------------------------------------------------- */
      M1X_MSG( MUX, LEGACY_MED,
        "Got TXC_CDMA_F, state=%d",
        state );

      /* In CDMA state, definitely allow sleep */
      txc_delay_enc_shutdown.txc_sleep_allowed = TRUE;

      txc_exit();

      /* ------------------------
      ** Switch back to CDMA mode
      ** ------------------------ */
      state  = TXC_CDMA_S;
      status = TXC_DONE_S;
      break;

    case TXC_PC_F:
      M1X_MSG( MUX, LEGACY_MED,
        "Got TXC_PC_F, state=%d",
        state );
      /* this command is only valid for the idle state */
      if( state == TXC_ACC_IDLE_S )
      {
        status = TXC_DONE_S;
      }
      else
      {
        status = TXC_BADCMD_S;
      }
      /* either way don't change the state */
      break;

    case TXC_FRAME_OFFSET_F:
      M1X_MSG( MUX, LEGACY_MED,
        "Got TXC_FRAME_OFFSET_F, state=%d",
        state );
      /* This command is not valid for the TXC_HOLD_ENC_CLK_S state */
      if( state == TXC_HOLD_ENC_CLK_S )
      {
        status = TXC_BADCMD_S;
        break;
      }
      txc_frame_offset_cmd.frame_offset = cmd_ptr->fo.frame_offset;
      qw_equ( txc_frame_offset_cmd.action_frame, cmd_ptr->fo.action_frame );
      txc_frame_offset_cmd.pending = TRUE;

      /*
      ** Leave the offset pending but set up the frame offset anyway
      ** since it affects the DECODER for traffic channel acquisition.
      */
      txc_frame_offset = txc_frame_offset_cmd.frame_offset;
      rxc_set_frame_offset( txc_frame_offset_cmd.frame_offset );
      enc_set_frame_off( txc_frame_offset, txc_frame_offset );
      status = TXC_DONE_S;
      break;

    default:
      M1X_MSG( MUX, LEGACY_HIGH,
        "Dropped MC command %d",
        cmd_ptr->hdr.command );
      status = TXC_BADCMD_S;

      /* Don't change the state */
      break;

  }  /* switch */

  txc_cmd_done( cmd_ptr, status );   /* Dispose of command buffer */

  return state;

} /* txc_access_cmd */

/*===========================================================================

FUNCTION txc_access_or_traffic_cmd

DESCRIPTION
  Check jump source to see if it is desirable to jump to access or traffic state
  after jump to hyperspace is complete.

DEPENDENCIES
  None.
RETURN VALUE
  None.
SIDE EFFECTS
  None
===========================================================================*/

txc_state_type txc_access_or_traffic_cmd (txc_state_type state)
{

  /*-----------------------------------------------------------------------*/

  if (txc_tx.jump_source == TXC_JUMP_ACCESS)
  {
#ifdef FEATURE_IS2000_REL_A_CC
    //txc_reach_on
    if ( txc_reach_access == TRUE )
    {
      enc_set_mode( ENC_TC_MODE );  /* set up hardware for reach */

      /* Load ENC_INT_ST register with BIN 0, BIN 1, BIN 2, BIN 3
      ** with power control groups 0 and Walsh Symbols 0
      */

      txac.state = ACC_SETUP1_S;

      /* Register the R-EACH isr on pcg 8 */
      txc_setup_pcg_events_isr ( TXC_PCG_EVENT_ACCESS, txc_reach_isr );
    }
    else
#endif /* FEATURE_IS2000_REL_A_CC */
    {
      enc_set_mode( ENC_AC_MODE );  /* set up hardware for access */

      /* -----------------------------------------------------------------
      ** Before transmitting each access probe sequence, generate a random
      ** number, RA, from 0 to ACC_CHANs.  Use RA as the Access Channel
      ** number for all access probes in that access probe sequence.
      ** ----------------------------------------------------------------- */
      txc_compute_access_chan();

      /* Register the access isr on pcg 8 */
      txc_setup_pcg_events_isr( TXC_PCG_EVENT_ACCESS, txc_access_isr );
    }

    /* Send command to interrupt handler to initialize frame count
        because receive interrupt could occur at this time and we
        could get the wrong frame count */
    txc_isr_cmd(INT_FRAME_F, NULL);
    state = TXC_ACC_ATT_S;  /* ready to transmit message */
  }
  else if (txc_tx.jump_source == TXC_JUMP_TRAFFIC)
  {
    txc_traffic_init();
    state = TXC_TRAFFIC_S;  /* ready to jump to traffic */

    if (txtc.tcb_ptr != NULL)
    {
      /* Notify requesting task TXC is entering traffic state */
      (void) rex_set_sigs( txtc.tcb_ptr, txtc.sigs );
      txtc.tcb_ptr = NULL;
    }
  }
  else if (txc_tx.jump_source == TXC_JUMP_TT)
  {
    /* traffic initialization and change TXC sub-state to TXC_TRAFFIC_S */
    txc_tt_tc_init( txc_tt_turn_on_transmitter );
    state = TXC_TRAFFIC_S;  /* ready to jump to traffic */
    M1X_MSG( MUX, LEGACY_HIGH,
      "From TXC_TT_S go to TXC_TRAFFIC_S ");
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Unexpected jump source value = %u",
      txc_tx.jump_source);
  }
  M1X_MSG( MUX, LEGACY_HIGH,
    "Acc or Traf, jump_source=%d,state=%d ",
    txc_tx.jump_source,
    state);
  return (state);

} /* txc_access_or_traffic_cmd */

/*===========================================================================

FUNCTION TXC_HYPERSPACE_TX_TIME_LATCH

DESCRIPTION
  Latches the Tx timing to Rx side timing at next PN roll and configures the
  pn randomization delay.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void txc_hyperspace_tx_time_latch( void )
{
  TXC_ENTER_ATOMIC_SECTION();

  /* Make sure no baseband can escape */
  enc_tx_enable (FALSE);

  /* -----------------------------------
  ** Set PN randomization transmit delay
  ** ----------------------------------- */
  if ( ( txc_tx.jump_source == TXC_JUMP_TRAFFIC ) ||
       ( txc_tx.jump_source == TXC_JUMP_TT )
     )
  {
    /* Reset to zero if moving to Traffic Channel */
    (void) enc_pn_rand_delay( 0 );
  }
  else
  {
    #ifdef FEATURE_IS2000_REL_A_CC
    //txc_reach_on
    if( txc_reach_access == FALSE )
    #endif /* FEATURE_IS2000_REL_A_CC */
    {
      (void) enc_pn_rand_delay( txac.rn );
    }
  }

  TXC_LEAVE_ATOMIC_SECTION();
} /* txc_hyperspace_tx_time_latch */

/*===========================================================================

FUNCTION TXC_HYPERSPACE

DESCRIPTION
  Performs jump to hyperspace for Transmit task which has not been able to
  jump since transmit clock has been turned off for power saving.

DEPENDENCIES
  Depends on the access channel mask to be used being placed in
  txc_tx.buf.mask.
RETURN VALUE
  None.
SIDE EFFECTS
  None
===========================================================================*/
txc_state_type txc_hyperspace
(
  txc_state_type state                /* txc_state - TXC_ACC_JUMP_S */
)
{
  txc_cmd_type *cmd_ptr;        /* pointer to received command */
  rex_sigs_type sigs = 0;

  /* This is used to indicate whether TXC is still waiting for TX
     Trigger response from FW */
  boolean txc_waiting_for_tx_trigger_rsp = FALSE;

  word ref_time;                /* time reference from SRCH */

  /* States of hyperspace state machine */
  typedef enum
  {
    PHASE_0_1_LATCH_TX_TIME,
    PHASE_2_PREP_ULPN_SLAM,
    PHASE_0_SLAM_COMPLETE
  } hyperspace_sub_state_type;

  hyperspace_sub_state_type sub_state;

  boolean ta_is_enabled = txc_is_ta_enabled();

  /*-------------------------------------------------------------------------*/

  /* Do not allow sleep so FL timing is not lost.
  ** Need accurate FL timing to calculate RL timing.
  */
  txc_delay_enc_shutdown.txc_sleep_allowed = FALSE;

  if( ta_is_enabled )
  {
    typedef enum
    {
      PHASE_PREP_TX_TRIGGER,
      PHASE_WAIT_ON_TRIGGER
    } hyperspace_opt_sub_state_type;

    hyperspace_opt_sub_state_type sub_state_opt = PHASE_PREP_TX_TRIGGER;
    txc_ta_info.ta_hs_jump_complete             = FALSE;

    while( state == TXC_ACC_JUMP_S )
    {
      ( void )rex_clr_sigs( TX_TCB_PTR, TXC_CMD_Q_SIG );
      if( (cmd_ptr = (txc_cmd_type*) q_get( &txc_cmd_q )) != NULL )
      {
        state = txc_access_cmd( cmd_ptr, TXC_ACC_JUMP_S );
      }

      if( ( txc_ta_info.ta_hs_jump_complete == FALSE ) &&
          ( txc_ta_info.ta_tc_init_done ) )
      {
        switch( sub_state_opt )
        {
          case PHASE_PREP_TX_TRIGGER:
          {
            M1X_MSG( MUX, LEGACY_HIGH,
              "Performing Optimized Tx timing slam prep");

            /* Do the Tx HW prefill and TxDAC trigger setup */
            enc_sync_tx_timing( FALSE );

            txc_waiting_for_tx_trigger_rsp = TRUE;

            M1X_MSG( MUX, LEGACY_HIGH,
                     "Tx timing slam prep completed,jump_source=%d",
                     txc_tx.jump_source );

            sub_state_opt = PHASE_WAIT_ON_TRIGGER;
            break;
          }

          case PHASE_WAIT_ON_TRIGGER:
          {
            M1X_MSG( MUX, LEGACY_HIGH, "Waiting for Tx trigger response." );
            if( sigs & TXC_MUXMSGR_Q_SIG )
            {
              /* Wait for trigger point on next frame boundary ( < 20 ms ) */
              if( muxmsgr_is_tx_trigger_rsp_received() )
              {
                /* Dump out Tx timing info for debug */
                muxmdsp_print_tx_timing_info( TRUE );

                txc_waiting_for_tx_trigger_rsp  = FALSE;
                txc_ta_info.ta_hs_jump_complete = TRUE;

                if( txc_tx.jump_source == TXC_READY_TO_JUMP )
                {
                  M1X_MSG( MUX, LEGACY_ERROR,
                    "Unexpected Jump to Hyperspace" );
                }
                else if( txc_tx.jump_source == TXC_JUMP )
                {
                  if( txac.hold_enc_clk_on )
                  {
                    /* Arm the timer to expire after allotted time if no command
                    ** is received from MC to transition to Traffic or
                    ** Access state. Once timer expires, turn off ENC clocks. */
                    txc_delay_enc_shutdown.counter =
                      TXC_HOLD_ENC_CLK_WAIT_DURATION;

                    /* transition to Hold ENC clocks ON state */
                    state = TXC_HOLD_ENC_CLK_S;
                  }
                  else
                  {
                    /* TXC was not told to hold the encoder clocks,
                    ** so just shut down the clocks and transition back to
                    ** idle state to save battery. */
                    state = TXC_ACC_IDLE_S; /* transition back to Access Idle */
                  }
                }
                else
                {
                  state = txc_access_or_traffic_cmd (TXC_ACC_JUMP_S);
                } /* end check for jump_source */
              }
            }
            break;
          }

          default:
          {
              ONEX_ERR_FATAL( "Invalid jump substate %d", (int) sub_state_opt, 0, 0 );
          }

        } /* switch */
      } /* end of if */

      if( state == TXC_ACC_JUMP_S )
      {
        sigs = txc_wait( TXC_CMD_Q_SIG | TXC_MUXMSGR_Q_SIG );
      }
    } /* while */
  }
  else
  {
    ( void )rex_clr_sigs( TX_TCB_PTR, TXC_PN_ROLL_SIG );

    /* Check if we can do a quick latch of TX time now rather than waiting for
       the PN roll to occur. This can save us some more time. */
    ref_time = srch_mux_scomb_get_ref_time();

    if( ( ( ref_time & TXC_ACC_PHASE_80M ) != TXC_ACC_PHASE2 )  &&
        ( ref_time < ( TXC_ACC_MAX_REF_TIME - TXC_ACC_TX_ENABLE_TIME ) ) )
    {
      M1X_MSG( MUX, LEGACY_MED,
        "Latch Tx time now!" );
      txc_hyperspace_tx_time_latch();
      sub_state = PHASE_2_PREP_ULPN_SLAM;
    }
    else
    {
      /* Couldn't latch time yet, wait for next PN roll. */
      sub_state = PHASE_0_1_LATCH_TX_TIME;
    }

    while( state == TXC_ACC_JUMP_S )
    {
      ( void )rex_clr_sigs( TX_TCB_PTR, TXC_CMD_Q_SIG );
      if( (cmd_ptr = (txc_cmd_type*) q_get( &txc_cmd_q )) != NULL )
      {
        state = txc_access_cmd( cmd_ptr, TXC_ACC_JUMP_S );
      }
      else if( sigs & TXC_PN_ROLL_SIG )
      {
        /* process the PN roll signal */
        ( void )rex_clr_sigs( TX_TCB_PTR,  TXC_PN_ROLL_SIG );

          /* get time reference from Searcher task */
        ref_time = srch_mux_scomb_get_ref_time();

        switch( sub_state )
        {
          case PHASE_0_1_LATCH_TX_TIME:
          {
            M1X_MSG( MUX, LEGACY_MED,
              "PHASE_0_1_LATCH_TX_TIME");

            /* Determine current PN phase */
          if( ( ( ref_time & TXC_ACC_PHASE_80M ) != TXC_ACC_PHASE2 )  &&
              ( ref_time < ( TXC_ACC_MAX_REF_TIME - TXC_ACC_TX_ENABLE_TIME ) ) )
            {
              txc_hyperspace_tx_time_latch();
              sub_state = PHASE_2_PREP_ULPN_SLAM;
            }
            break;
          }

          case PHASE_2_PREP_ULPN_SLAM:
          {
            M1X_MSG( MUX, LEGACY_MED,
              "PHASE_2_PREP_ULPN_SLAM");

            if( ( ref_time & TXC_ACC_PHASE_80M ) == TXC_ACC_PHASE2 )
            {
              M1X_MSG( MUX, LEGACY_HIGH,
                "Performing Tx timing slam prep");

              /* Do the Tx HW prefill and TxDAC trigger setup */
              enc_sync_tx_timing( FALSE );

              txc_waiting_for_tx_trigger_rsp = TRUE;

              M1X_MSG( MUX, LEGACY_HIGH,
                "Tx timing slam prep completed");

              sub_state = PHASE_0_SLAM_COMPLETE;
            }
            break;
          }

          case PHASE_0_SLAM_COMPLETE:
          {
            /* At this point, we should have gotten the trigger response.
               Clear this flag just in case to avoid race conditions */
            txc_waiting_for_tx_trigger_rsp = FALSE;

            /* Dump out Tx timing info for debug */
            muxmdsp_print_tx_timing_info( TRUE );

            if( ( ref_time & TXC_ACC_PHASE_80M ) != TXC_ACC_PHASE0 )
            {
              M1X_MSG( MUX, LEGACY_HIGH,
                "Phase error %d",
                ref_time );
            }
            else
            {
              M1X_MSG( MUX, LEGACY_HIGH,
                "PHASE_0_SLAM_COMPLETE,jump_source=%d",
                txc_tx.jump_source );

              if( txc_tx.jump_source == TXC_READY_TO_JUMP )
              {
                M1X_MSG( MUX, LEGACY_ERROR,
                  "Unexpected Jump to Hyperspace");
              }
              else if( txc_tx.jump_source == TXC_JUMP )
              {
                if( txac.hold_enc_clk_on )
                {
                  /* Arm the timer to expire after allotted time if no command
                  ** is received from MC to transition to Traffic or
                  ** Access state.  Once timer expires, turn off ENC clocks. */
                  txc_delay_enc_shutdown.counter =
                    TXC_HOLD_ENC_CLK_WAIT_DURATION;

                  /* transition to Hold ENC clocks ON state */
                  state = TXC_HOLD_ENC_CLK_S;
                }
                else
                {
                  /* TXC was not told to hold the encoder clocks, so just
                  ** shut down the clocks and transition back to idle state
                  ** to save battery. */

                  /* transition back to Access Idle state */
                  state = TXC_ACC_IDLE_S;
                }
              }
              else
              {
                state = txc_access_or_traffic_cmd( TXC_ACC_JUMP_S );
              } /* end check for jump_source */
            }
            break;
          }

          default:
          {
            ONEX_ERR_FATAL( "Invalid jump substate %d", (int) sub_state, 0, 0 );
          }

        } /* switch */
      }
      else if( sigs & TXC_MUXMSGR_Q_SIG )
      {
        /*
          Check for Trigger Response from FW.
          The TX Trigger function processes all Mux Msgr related signals.
          So it should handle all the cases.
        */
        if( muxmsgr_is_tx_trigger_rsp_received() )
        {
          txc_waiting_for_tx_trigger_rsp = FALSE;
        }
      }

      if( state == TXC_ACC_JUMP_S )
      {
        sigs = txc_wait( TXC_PN_ROLL_SIG | TXC_CMD_Q_SIG | TXC_MUXMSGR_Q_SIG );
      }
    } /* while */
  } /* end of !ta if */

  if( state == TXC_ACC_IDLE_S )
  {
    /* Not going to transmit the access after all */
    M1X_MSG( MUX, LEGACY_HIGH,
      "TX Hyperspace jump abort" );

    /* Just print this F3 once. No need to spam till FW sends the response */
    if( txc_waiting_for_tx_trigger_rsp )
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "Waiting for Tx trigger response before abort" );
    }

    /* Spin and Wait for Trigger Response from FW */
    while( txc_waiting_for_tx_trigger_rsp )
    {
      /* Check to see if we got response from FW */
      if( muxmsgr_is_tx_trigger_rsp_received() )
      {
        /* Set the condition to FALSE as we got the response from FW.
           This will break out of the while loop */
        txc_waiting_for_tx_trigger_rsp = FALSE;
      }
    } /* while */

    /* Turn off clocks, pa, etc. */
    txc_tx_shutdown();

    if( txac.tcb_ptr != NULL )
    {
      /* Notify requesting task that access attempt is completed */
      (void) rex_set_sigs( txac.tcb_ptr, txac.sigs );
      txac.tcb_ptr = NULL;
    }
  }

  return state;

} /* txc_hyperspace */

/*===========================================================================

FUNCTION TXC_ACCESS_ISR

DESCRIPTION
  Interrupt handler for the access channel.  Very simple, just send
  whatever it is given to send.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_access_isr
(
   m1x_stmr_event_type pcg
)
{
  /*-----------------------------------------------------------------------*/

  (void) pcg; /* unused */

  txc_tx.int_cnt++;                    /* Count tx interrupts */
  qw_inc( txc_tx.frame_cnt, 1L);       /* Count frames */

  /* Check for commands */
  if (txc_int_cmd.iscmd)
  {
    /* Make sure power amp is off */
    enc_tx_enable( FALSE );

    switch (txc_int_cmd.cmd)
    {
      case INT_TC_F:
        M1X_MSG( MUX, LEGACY_MED,
          "Got INT_TC_F");
          /* --------------------------
        ** Go to Traffic Channel mode
        ** -------------------------- */
        enc_set_mode( ENC_TC_MODE );

        /* set initial RC */
        if ( txc_so.rev_dcch.included )
        {
          enc_dcch_rc( (enc_fch_radio_config_type) txc_so.rev_dcch.rc );
        }
        if ( txc_so.rev_fch.included )
        {
          enc_fch_rc( (enc_fch_radio_config_type) txc_so.rev_fch.rc );
        }

        /* Load user PN mask */
        enc_user_pn_mask( txc_int_cmd.mask );

        /* Set the counter so that TXC ISR calculates the frame number for
           first NO_OF_ISRS_AFTER_FO isrs. */
        after_fo_handoff_count_isrs = NO_OF_ISRS_AFTER_FO;

        break;

      case INT_FRAME_F:
        M1X_MSG( MUX, LEGACY_MED,
          "Got INT_FRAME_F");
        /* ----------------------
        ** Initialize frame count
        ** ---------------------- */

        /* Derive frame from timestamp>>20 */
        m1x_time_get_20ms_frame_time ( txc_tx.frame_cnt );

        /* Increment frame count for correct TX frame count */
        qw_inc( txc_tx.frame_cnt, 1L );

        M1X_MSG( MUX, LEGACY_MED,
          "set frame_cnt+1 (%x %x)",
          qw_hi(txc_tx.frame_cnt),
          qw_lo(txc_tx.frame_cnt));

        /* We are allowed to transmit */
        txc_tx.tx_on = TRUE;
        break;

      default:
        M1X_MSG( MUX, LEGACY_ERROR,
          "Illegal command %d",
          txc_int_cmd.cmd );
    } /* switch */

    txc_int_cmd.iscmd = FALSE;     /* done with command */
    (void) rex_set_sigs( TX_TCB_PTR, TXC_INT_CMD_SIG );

  } /* end if (txc_int_cmd.iscmd) */

  else
  {
    /* We are allowed to transmit */
    txc_tx.tx_on = TRUE;

    /* See if we transmit this frame or not */
    if (txac.frame.dotx)
    {
      /* Make sure power amp is on */
      enc_tx_enable( TRUE );

      /* Set TX pattern to transmit in all PCGs */
      enc_is95c_tx_patn( ENC_NO_GATING_PATTERN );

      /* Set the channel gains */
      channel_gains.txc_fch_gain    = TXC_IS95A_FCH_GAIN;
      channel_gains.txc_pch_gain    = 0;
      channel_gains.txc_sch_gain    = 0;
      channel_gains.txc_dcch_gain   = 0;
      channel_gains.txc_tx_rate_adj = 0;

      muxmdsp_set_channel_gains();

      /* Set transmit power */
      srch_mux_set_tx_adj( (byte) txac.power );
      txac.last_pwr = txac.power;    /* Save last power used for traffic */

      #ifndef FEATURE_MODEM_1X_UTX
      (void)muxmdsp_toggle_iram_index();
      #endif /* !FEATURE_MODEM_1X_UTX */
      /* Always transmit half rate on the access channel */
      enc_fch_data_out( txac.frame.data, FALSE, ENC_HALF_RATE );

      if ((txac.state == ACC_MSG_S) && (txac.first_frame))
      {
        /* Log only first message frame */
        txac.first_frame = FALSE;

        if (acc_log.hdr.len != 0)
        {
          acc_log.hdr.code = LOG_ACC_INFO_C;
          (void) m1x_time_get(acc_log.hdr.ts);
          acc_log.probe_num = txac.probe_count + 1;
          acc_log.rx_agc = srch_mux_get_rx_agc();
          acc_log.tx_adj = (byte) txac.power;
          /* log the interference-correction */
          acc_log.int_corr = txac.int_corr;
          (void) log_submit(&acc_log );
          acc_log.hdr.len = 0;
        }
      }
    } /* if (txac.frame.dotx) */
    else /* Not transmitting this frame */
    {
      /* Make sure power amp is off */
      txc_tx_disable();
    }
  }

  #ifndef FEATURE_MODEM_1X_UTX
  /* Tell FW to commit shared memory cfg and data is ready in LMEM */
  muxmdsp_send_frame_config();
  #endif /* FEATURE_MODEM_1X_UTX */

  /* update modem power stats */
  txc_update_power_stats();

  /* Let the tx task know that we had an interrupt */

  (void) rex_set_sigs( TX_TCB_PTR, TXC_INT_SIG );

} /* txc_access_isr */


#ifdef FEATURE_IS2000_REL_A_CC
/*===========================================================================

FUNCTION TXC_REACH_ISR_PROCESS_PREAMBLE

DESCRIPTION
  Interrupt handler for the enhanced access channel for preamble processing.
  Very simple, just send whatever it is given to send.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_reach_isr_process_preamble( m1x_stmr_event_type pcg )
{

  /*-----------------------------------------------------------------------*/

  if ( txc_reach_first_frame_input_data == FALSE )
  {
    /* Transmit REACH message first frame data */
    enc_reach_data_out( txtc.supp_frame.data, TRUE, txtc.supp_rate,
             txtc.supp_turbo, txtc.supp_ltu_size, txc_reach_frame_length );

    txc_reach_first_frame_input_data = TRUE;
  }

  if ( txc_reach_program_preamble_prepare == TRUE )
  {
    if ( ( pcg >=  M1X_STMR_PCG_0 ) && ( pcg <= M1X_STMR_PCG_3 ) )
    {
      if ( txc_reach_preamble_count == 1 )
      {
        enc_set_frame_off ( 0x0, (byte)txc_reach_preamble_frame_offset );

        /* Set REACH Preamble gain to constant digital gain 255 (15 bit resolution.) */
        channel_gains.txc_pch_gain = RPC_TOT_DIG_PWR_255 << 7;

        /* Set RATE_ADJ to increase Preamble 6 dB and adjust offset power.*/
        channel_gains.txc_tx_rate_adj = txtc.offset_pwr - 72;

        muxmdsp_set_channel_gains();
        enc_pch_enable(TRUE);

        txc_reach_program_preamble_prepare = FALSE;
        txc_reach_program_data_prepare = TRUE;
        txc_reach_intr_cnt = 0;
      }
    }
    else if ( ( pcg >=  M1X_STMR_PCG_4 ) && ( pcg <= M1X_STMR_PCG_7 ) )
    {
      if ( txc_reach_preamble_count == 2 )
      {
        enc_set_frame_off ( 0x0, (byte)txc_reach_preamble_frame_offset );

        /* Set REACH Preamble gain to constant digital gain 255 (15 bit resolution.) */
        channel_gains.txc_pch_gain = RPC_TOT_DIG_PWR_255 << 7;

        /* Set RATE_ADJ to increase Preamble 6 dB and adjust offset power.*/
        channel_gains.txc_tx_rate_adj = txtc.offset_pwr - 72;

        muxmdsp_set_channel_gains();
        enc_pch_enable(TRUE);

        txc_reach_program_preamble_prepare = FALSE;
        txc_reach_program_data_prepare = TRUE;
        txc_reach_intr_cnt = 0;
      }
    }
    else if ( ( pcg >=  M1X_STMR_PCG_8 ) && ( pcg <= M1X_STMR_PCG_11 ) )
    {
      if ( txc_reach_preamble_count == 3 )
      {
        enc_set_frame_off ( 0x0, (byte)txc_reach_preamble_frame_offset );

        /* Set REACH Preamble gain to constant digital gain 255 (15 bit resolution.) */
        channel_gains.txc_pch_gain = RPC_TOT_DIG_PWR_255 << 7;

        /* Set RATE_ADJ to increase Preamble 6 dB and adjust offset power.*/
        channel_gains.txc_tx_rate_adj = txtc.offset_pwr - 72;

        muxmdsp_set_channel_gains();
        enc_pch_enable(TRUE);

        txc_reach_program_preamble_prepare = FALSE;
        txc_reach_program_data_prepare = TRUE;
        txc_reach_intr_cnt = 0;
      }
    }
  }
  else if ( txc_reach_program_data_prepare == TRUE )
  {
    if ( ++txc_reach_intr_cnt == txc_reach_data_count )
    {
      /* txc_program_data */
      enc_set_frame_off ( 0x0, (byte)txc_reach_data_frame_offset );

      channel_gains.txc_pch_gain = txc_reach_gains.txc_pch_gain;
      channel_gains.txc_sch_gain = txc_reach_gains.txc_sch_gain;

      /* Set RATE_ADJ to adjust pilot channel gain and offset power.*/
      channel_gains.txc_tx_rate_adj = txc_reach_gains.txc_tx_rate_adj + txtc.offset_pwr;

      muxmdsp_set_channel_gains();
      enc_pch_enable(TRUE);
      enc_sch_enable(TRUE);

      txc_reach_program_data_prepare = FALSE;

      txac.state = ACC_MSG_S;

      M1X_MSG( MUX, LEGACY_HIGH,
        "datoffset = %d, pch= %d, sch gain= %d",
        txc_reach_data_frame_offset,
        channel_gains.txc_pch_gain,
        channel_gains.txc_sch_gain );
    }
  }
  else if ( ( pcg >=  M1X_STMR_PCG_10 ) && ( pcg <= M1X_STMR_PCG_15 ) )
  {
    if ( txc_reach_preamble_count == 0 )
    {
      enc_set_frame_off ( 0x0, (byte)txc_reach_preamble_frame_offset );

      /* Set REACH Preamble gain to constant digital gain 255 (15 bit resolution.) */
      channel_gains.txc_pch_gain = RPC_TOT_DIG_PWR_255 << 7;

      /* Set RATE_ADJ to increase Preamble 6 dB and adjust offset power.*/
      channel_gains.txc_tx_rate_adj = txtc.offset_pwr - 72;

      muxmdsp_set_channel_gains();
      enc_pch_enable(TRUE);

      txc_reach_program_data_prepare = TRUE;
      txc_reach_intr_cnt = 0;

      M1X_MSG( MUX, LEGACY_HIGH,
        "pamoffset = %d, pch gain = %d, sch = %d",
        txc_reach_preamble_frame_offset,
        channel_gains.txc_pch_gain,
        channel_gains.txc_sch_gain );
    }
    else
    {
      txc_reach_program_preamble_prepare = TRUE;
    }
  } /* end if ( txc_reach_program_preamble_prepare == TRUE ) */
} /* txc_reach_isr_process_preamble */

/*===========================================================================

FUNCTION TXC_REACH_ISR_PROCESS_COMMANDS

DESCRIPTION
  Helper function called from the R-EACH isr to processes txc int commands.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_reach_isr_process_commands( void )
{
  /*-----------------------------------------------------------------------*/

  /* Check for commands */
  if (txc_int_cmd.iscmd)
  {
    /* Make sure power amp is off */
    enc_tx_enable( FALSE );

    switch (txc_int_cmd.cmd)
    {
      case INT_TC_F:
        M1X_MSG( MUX, LEGACY_MED,
          "Got INT_TC_F");
        /* --------------------------
        ** Go to Traffic Channel mode
        ** -------------------------- */
        enc_set_mode( ENC_TC_MODE );

        /* set initial RC */
        if ( txc_so.rev_dcch.included )
        {
          enc_dcch_rc( (enc_fch_radio_config_type) txc_so.rev_dcch.rc );
        }
        if ( txc_so.rev_fch.included )
        {
          enc_fch_rc( (enc_fch_radio_config_type) txc_so.rev_fch.rc );
        }

        /* Load user PN mask */
        enc_user_pn_mask( txc_int_cmd.mask );

        /* Set the counter so that TXC ISR calculates the frame number for
           first NO_OF_ISRS_AFTER_FO isrs. */
        after_fo_handoff_count_isrs = NO_OF_ISRS_AFTER_FO;

        M1X_MSG( MUX, LEGACY_MED,
          "traffic isr ------------------------------" );
      break;

      case INT_FRAME_F:
        M1X_MSG( MUX, LEGACY_MED,
          "Got INT_FRAME_F");
        /* ----------------------
        ** Initialize frame count
        ** ---------------------- */

        /* Derive frame from timestamp>>22 */
        m1x_time_get_20ms_frame_time ( txc_tx.frame_cnt );

        /* Increment frame count for correct TX frame count */
        qw_inc( txc_tx.frame_cnt, 1L );

        /* We are allowed to transmit */
        txc_tx.tx_on = TRUE;
      break;

      default:
        M1X_MSG( MUX, LEGACY_ERROR,
          "Illegal command %d",
          txc_int_cmd.cmd );
    } /* switch */

    txc_int_cmd.iscmd = FALSE;     /* done with command */
    (void) rex_set_sigs( TX_TCB_PTR, TXC_INT_CMD_SIG );
  } /* end if (txc_int_cmd.iscmd) */

} /* txc_reach_isr_process_commands */

/*===========================================================================

FUNCTION TXC_REACH_ISR_LOG_INFO

DESCRIPTION
  Helper function to send out R-EACH log information.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_reach_isr_log_info( void )
{
  m1x_stmr_err_type err_code;

  /*-----------------------------------------------------------------------*/

  txc_tx.int_cnt++;                    /* Count tx interrupts */

  /* We are allowed to transmit */
  txc_tx.tx_on = TRUE;

  /* See if we transmit this frame or not */
  if (txac.frame.dotx)
  {
    if ((txac.state == ACC_MSG_S) && (txac.first_frame))
    {
      /* Log only first message frame */
      txac.first_frame = FALSE;
      if (txc_reach_frame_info_log.hdr.len != 0)
      {
        /* initialize the log header: log code, time stamp*/
        txc_reach_frame_info_log.hdr.code = LOG_EA_PROBE_C;
        (void) m1x_time_get(txc_reach_frame_info_log.hdr.ts);
        /* fill out the REACH channel info structure
        ** for now, all the fields we don't know, we will set them to zero
        ** later on, we will set them to the corrested values
        */

        txc_reach_frame_info_log.rlgain_com_pilot
                                = txc_reach_rlgain_common_pilot;
        txc_reach_frame_info_log.sub_att_number
                                = txc_reach_sub_att_number + 1;
        txc_reach_frame_info_log.sequence_number
                                = txac.seq_num + 1;
        txc_reach_frame_info_log.access_prob_number
                                = txac.probe_count + 1;
        txc_reach_frame_info_log.each_num_step
                                = txac.num_step+1;
        if (txc_reach_psist_delay<=255)
        {
          txc_reach_frame_info_log.psist_delay
                                  = (uint8) txc_reach_psist_delay;
        }
        else
        {
          txc_reach_frame_info_log.psist_delay = 255;
          M1X_MSG( MUX, LEGACY_ERROR,
            "%d psist failures, logging 255",
            txc_reach_psist_delay );
        }
        txc_reach_frame_info_log.sequence_bkoff
                                = txac.bkoff;
        txc_reach_frame_info_log.probe_bkoff
                                = txac.probe_bkoff;
        txc_reach_frame_info_log.each_slot
                                = (uint8) txc_reach_slot_length - 1;
        txc_reach_frame_info_log.fccch_id
                                = txac.pagech;
        txc_reach_frame_info_log.each_id
                                = txac.ra;
        txc_reach_frame_info_log.each_slot_offset1
                                = (uint8) txc_reach_slot_offset1;
        txc_reach_frame_info_log.base_id
                                = txac.base_id;
        txc_reach_frame_info_log.each_slot_offset2
                                = (uint8) txc_reach_slot_offset2;
        txc_reach_frame_info_log.acc_rate
                                = (uint8) txc_reach_rate_word;
        txc_reach_frame_info_log.rx_agc
                                = srch_mux_get_rx_agc();
        #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
        txc_reach_frame_info_log.tx_agc
          = (byte)(txc_get_tx_agc_in_legacy_units_dbm12()>>2);
        #else /* FEATURE_MODEM_1X_SUPPORTS_RF */
        txc_reach_frame_info_log.tx_agc = 0;
        #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */
        txc_reach_frame_info_log.tx_adj
                                = (byte) txac.power;
        txc_reach_frame_info_log.int_corr
                                = txac.int_corr;
        txc_reach_frame_info_log.acc_preamble_tx_low32bit
                                = qw_lo(txc_reach_preamble_frame_pcg);
        txc_reach_frame_info_log.acc_preamble_tx_high16bit
                            = (uint16) qw_hi(txc_reach_preamble_frame_pcg);
        txc_reach_frame_info_log.pdu_length
                                = txc_tx.buf.len;
        txc_reach_frame_info_log.band_class
                                = cdma.band_class;

        M1X_MSG( MUX, LEGACY_MED,
          "--> Sending LOG_EA_PROBE_C" );

        /* call the function to send out the log */
        (void) log_submit(&txc_reach_frame_info_log );

        /* reset the length to be ready for the next log */
        txc_reach_frame_info_log.hdr.len = 0;
      }
    }
  } /* if (txac.frame.dotx) */

  /* Let the tx task know that we had an interrupt */
  (void) rex_set_sigs( TX_TCB_PTR, TXC_INT_SIG );

} /* txc_reach_isr_log_info */

/*===========================================================================

FUNCTION TXC_REACH_ISR

DESCRIPTION
  Interrupt handler for the enhanced access channel.  Very simple, just send
  whatever it is given to send.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void txc_reach_isr(
  m1x_stmr_event_type pcg
)
{

  /*-----------------------------------------------------------------------*/

  if ( txac.state == ACC_PRE_S )
  {
    /* Process preamble programming */
    txc_reach_isr_process_preamble(pcg);
  }
  else /* not in preamble programming */
  {
    txc_reach_isr_process_commands();
    txc_reach_isr_log_info();

    /* See if we transmit this frame or not */
    if (txac.frame.dotx)
    {
      /* Make sure power amp is on */
      enc_tx_enable( TRUE );

      /* Set transmit power */
      srch_mux_set_tx_adj( (byte) txac.power );
      txac.last_pwr = txac.power;    /* Save last power used for traffic */

      /* Transmit REACH on the RC3 supplemental channel */

      enc_reach_data_out( txtc.supp_frame.data, TRUE, txtc.supp_rate,
                 txtc.supp_turbo, txtc.supp_ltu_size, txc_reach_frame_length );
    } /* if (txac.frame.dotx) */
    else /* Not transmitting this frame */
    {
      txc_tx_disable();
    }
  } /*  end if ( txac.state == ACC_PRE_S) */

} /* txc_reach_isr */
#endif // FEATURE_IS2000_REL_A_CC

/*===========================================================================

FUNCTION TXC_ACC_MSG_PREP

DESCRIPTION
  Process request to send an Access Channel message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  txac and txc_tx buffers are set up for message transmission.

===========================================================================*/
void txc_acc_msg_prep
(
  txc_cmd_type *cmd_ptr
    /* Pointer to received command */
)
{
  word len;           /* Length of Access Channel message */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* --------------------------------------------------------------------
  ** Number of Access Channel frames that mobile stations are to transmit
  ** in each Access Channel preamble
  ** -------------------------------------------------------------------- */
  txac.preambles = cmd_ptr->acc.pam_sz + 1;

  /* ----------------------------------------------------------------
  ** Set Access Channel slot length ((1 + PAM_SZ) + (3 + MAX_CAP_SZ))
  ** ---------------------------------------------------------------- */
  txac.slotting = (1 + cmd_ptr->acc.pam_sz) + (3 + cmd_ptr->acc.max_cap_sz);

  txac.bkoff = cmd_ptr->acc.bkoff;              /* Sequence backoff */
  txac.probe_bkoff = cmd_ptr->acc.probe_bkoff;  /* Probe backoff */
  txac.acc_tmo = cmd_ptr->acc.acc_tmo;          /* Ack timeout */
  txac.max_seq = cmd_ptr->acc.seq_num;          /* Max probe sequences */
  txac.seq_num = 0;                             /* Sequence number */
  txac.req = cmd_ptr->acc.req;                  /* Request or response */

  /* -----------------------------------------
  ** Calculate initial power offset for access
  ** ----------------------------------------- */
  if ((cmd_ptr->acc.init_pwr & 0x10))  /* Sign bit (5 bit field) */
  {
    /* Negative value --> sign extend to 8 bits */
    txac.power = (byte) (cmd_ptr->acc.init_pwr | 0xE0);
  }
  else
  {
    txac.power = (cmd_ptr->acc.init_pwr & 0x1F);
  }

  /* -----------------------------------------------------------------
  ** Add in nom_pwr which is already sign extended and, if necessary,
  ** adjusted according to nom_pwr_ext.
  ** ----------------------------------------------------------------- */
  txac.power += cmd_ptr->acc.nom_pwr;

  /* Invert to turn power adjustment into tx gain adjustment */
  txac.power = -(txac.power); //lint !e732: Loss of sign (sign maintainied manually)

  /* Convert to units of 1/2 dB */
  // init_pwr is 5 bit, nom_pwr is 4 bit.  Sum can be 6 bit.  Product (*2) can be only 7.
  txac.power = txac.power * 2; //lint !e734: Loss of precision (9 to 8 bits)

  txac.init_pwr = txac.power;

  /* initialize the is95a power accumulation */
  txac.is95a_pwr = txac.init_pwr;

  /* Grab power step from message in 1dB steps */
  txac.pstep = (byte) (cmd_ptr->acc.pwr_step);

  /* Convert to units of 1/2 dB */
  // pwr_step is 3 bit. Product (*2) can only be 4.
  txac.pstep = txac.pstep * 2; //lint !e734: Loss of precision (9 to 8 bits)

  txac.nstep = cmd_ptr->acc.num_step;      /* Number of access probes */
  txac.num_step = txac.nstep;              /* # of access probes remaining */
  txac.probe_count = 0;                    /* Access probe number */
  txac.attempt_probe_count = 0;            /* Access attempt probe number */
  txac.first_frame = FALSE;                /* Not first frame */
  txac.rn = cmd_ptr->acc.rn;               /* PN randomization delay */
  txac.p = cmd_ptr->acc.p;                 /* Persistence test value */
  txac.acc_chan = cmd_ptr->acc.acc_chan;   /* Number of access channels */
  txac.use_chan = cmd_ptr->acc.use_chan;   /* Use specific channel */
  txac.pagech   = cmd_ptr->acc.pagech;     /* Current Paging Channel */
  txac.base_id  = cmd_ptr->acc.base_id;    /* Base station identification */
  txac.pilot_pn = cmd_ptr->acc.pilot_pn;   /* Pilot PN seq offset index */
  txc_tx.buf.cmd_ptr = NULL;    /* Don't save command info */
  len = cmd_ptr->acc.len;       /* Get Access Channel message length */

  /* Perform a check on the length of the Access Channel Message */
  /* Note that len is in bits.                                   */
  ASSERT(len <= (CAI_AC_MAX_SIZE - (CAI_AC_CRC_SIZE + CAI_AC_LGTH_SIZE)));

  /* Zero the byte in which the message body ends */
  txc_tx.buf.msg[(len >> 3) + 1] = 0; /* Pad end with 0's */

  /* -----------------------------------------------------------------------
  ** If the (len mod 8) is between 3 and 7 (i.e. the message body uses more
  ** than 2 bits of its last byte) then the CRC will begin in the byte after
  ** the one in which the message body ends.  If that is the case, we need
  ** to also zero the two bits preceeding the 30 bit CRC in that byte.
  ** ----------------------------------------------------------------------- */
  txc_tx.buf.msg[(len >> 3) + 2] = 0; /* Pad end with 0's */

  /* Copy the actual message */
  b_copy( cmd_ptr->acc.msg_ptr, 0,       /* Source */
          &txc_tx.buf.msg[1], 0, len );  /* Destination, leave room for len */

  /* Add in CRC and MSG_LENGTH fields */
  len += CAI_AC_CRC_SIZE + CAI_AC_LGTH_SIZE;

  if (len % 8)
  {
    /* Pad length out to byte boundary */
    len = (len & ~7) + 8;
  }

  txc_tx.buf.msg[0] = (byte) (len >> 3);  /* Save length in bytes */

  b_packd( crc_30_calc( txc_tx.buf.msg, len-30 ), /* Calculate CRC */
           txc_tx.buf.msg, len-30, 30 );          /* Add to message */

  /*
  ** Save length in bits to actually transmit. If we make this the size
  ** of the message capsule then we will transmit all the frames of the
  ** capsule and transmit zeroes when we run out of message.
  */
  txc_tx.buf.len = (3 + cmd_ptr->acc.max_cap_sz) * TXC_ACC;

  /* -----------------------------------------------------------------
  ** Before transmitting each access probe sequence, generate a random
  ** number, RA, from 0 to ACC_CHANs.  Use RA as the Access Channel
  ** number for all access probes in that access probe sequence.
  ** ----------------------------------------------------------------- */
  txc_compute_access_chan();

  txc_tx.active_msg = TRUE;

  if (cmd_ptr->acc.tcb_ptr != NULL)
  {
    /* Save pointer to task to signal back and signal to use */
    txac.tcb_ptr = cmd_ptr->acc.tcb_ptr;
    txac.sigs = cmd_ptr->acc.sigs;
  }

} /* txc_acc_msg_prep */


#ifdef FEATURE_IS2000_REL_A_CC
//txc_reach_on

/*===========================================================================

FUNCTION TXC_REACH_MSG_PREP

DESCRIPTION
  Process request to send an Access Channel message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  txac and txc_tx buffers are set up for message transmission.

===========================================================================*/
void txc_reach_msg_prep
(
  txc_cmd_type *cmd_ptr
    /* Pointer to received command */
)
{
  word len;           /* Length of Access Channel message */

  word reach_len;     /* Length of Enhanced Access Channel message */


  int i;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  txac.bkoff = cmd_ptr->reach.bkoff;              /* Sequence backoff */
  txac.probe_bkoff = cmd_ptr->reach.probe_bkoff;  /* Probe backoff */
  txac.acc_tmo = cmd_ptr->reach.acc_tmo;          /* Ack timeout */
  txac.max_seq = cmd_ptr->reach.seq_num;          /* Max probe sequences */
  txac.seq_num = 0;                             /* Sequence number */
  txac.req = cmd_ptr->reach.req;                  /* Request or response */

  /* -----------------------------------------
  ** Calculate initial power offset for access
  ** ----------------------------------------- */
  if ((cmd_ptr->reach.init_pwr & 0x10))  /* Sign bit (5 bit field) */
  {
    /* Negative value --> sign extend to 8 bits */
    txac.power = (byte) (cmd_ptr->reach.init_pwr | 0xE0);
  }
  else
  {
    txac.power = (cmd_ptr->reach.init_pwr & 0x1F);
  }

  /* -----------------------------------------------------------------
  ** Add in nom_pwr which is already sign extended and, if necessary,
  ** adjusted according to nom_pwr_ext.
  ** ----------------------------------------------------------------- */
  txac.power += cmd_ptr->reach.nom_pwr;

  /* Invert to turn power adjustment into tx gain adjustment */
  txac.power = -(txac.power); //lint !e732: Loss of sign (sign is maintained manually)

  /* Convert to units of 1/2 dB */
  // num_pwr is 5 bit, init_pwr is 5 bits.  Sum is 6 bits, product (*2) is 7
  txac.power = txac.power * 2; //lint !e734: Loss of precision (9 to 8 bits)

  txac.init_pwr = txac.power;

  /* initialize the is95a power accumulation */
  txac.is95a_pwr = txac.init_pwr;

  /* Grab power step from message in 1dB steps */
  txac.pstep = (byte) (cmd_ptr->reach.pwr_step);

  /* Convert to units of 1/2 dB */
  // pwr_step is 3 bits.  Product (*2) is 4 bits.
  txac.pstep = txac.pstep * 2; //lint !e734: Loss of precision (9 to 8 bits)

  txac.nstep = cmd_ptr->reach.num_step;      /* Number of access probes */
  txac.num_step = txac.nstep;              /* # of access probes remaining */
  txac.probe_count = 0;                    /* Access probe number */
  txac.attempt_probe_count = 0;            /* Access attempt probe number */
  txac.first_frame = FALSE;                /* Not first frame */
  txac.p = cmd_ptr->reach.p;                 /* Persistence test value */
  txac.acc_chan = cmd_ptr->reach.num_each_ba;   /* Number of access channels */
  txac.pagech   = cmd_ptr->reach.fccch;       /* Current Paging Channel */
  txac.base_id  = cmd_ptr->reach.base_id;    /* Base station identification */
  txac.pilot_pn = cmd_ptr->reach.pilot_pn;   /* Pilot PN seq offset index */
  txc_tx.buf.cmd_ptr = NULL;    /* Don't save command info */
  len = cmd_ptr->reach.len;       /* Get Access Channel message length */

  /* Perform a check on the length of the Access Channel Message */
  /* Note that len is in bits.                                   */
  ASSERT(len <= ( (MAX_REV_MSG_BYTES*8) - (CAI_EA_CRC_SIZE + CAI_EA_LGTH_SIZE))); //lint !e506: Constant value Boolean

  // CAI_AC_MAX_SIZE = 255*8 bits
  // CAI_AC_CRC_SIZE = 30 bits
  // CAI_AC_LGTH_SIZE = 8 bits
  // For REACH, MSG_LENGTH can be 8 bits or 16 bits
  // If len <= 978 bits, EXT_MSG_LENGTH = 0, MSG_LENGTH = 7 bits
  // If len > 978 bits, EXT_MSG_LENGTH = 1, MSG_LENGTH = 15 bits
  // Need to look at the place CAI_AC_LGTH_SIZE = 8 bits
  // Need to use 8 bits or 16 bits for checking
  // Need to use 8 bits or 16 bits for calculation


  if (len > 978) /* Ext_Msg_Length is 1 bit, Msg_Length is 15 bits */
  {
    /* Zero the byte in which the message body ends */
    txc_tx.buf.msg[(len >> 3) + 2] = 0; /* Pad end with 0's */

    /* -----------------------------------------------------------------------
    ** If the (len mod 8) is between 3 and 7 (i.e. the message body uses more
    ** than 2 bits of its last byte) then the CRC will begin in the byte after
    ** the one in which the message body ends.  If that is the case, we need
    ** to also zero the two bits preceeding the 30 bit CRC in that byte.
    ** ----------------------------------------------------------------------- */
    txc_tx.buf.msg[(len >> 3) + 3] = 0; /* Pad end with 0's */

    /* Copy the actual message */
    b_copy( cmd_ptr->reach.msg_ptr, 0,       /* Source */
              &txc_tx.buf.msg[2], 0, len );  /* Destination, leave room for len */

    /* Add in CRC and MSG_LENGTH fields */
    len += CAI_EA_CRC_SIZE + CAI_EA_LGTH_SIZE;


    // CAI_AC_CRC_SIZE = 30 bits
    // CAI_AC_LGTH_SIZE = 8 bits
    // For REACH, MSG_LENGTH can be 8 bits or 16 bits
    // If len <= 978 bits, EXT_MSG_LENGTH = 0, MSG_LENGTH = 7 bits
    // If len > 978 bits, EXT_MSG_LENGTH = 1, MSG_LENGTH = 15 bits
    // Need to look at the place CAI_AC_LGTH_SIZE = 8 bits
    // Need to use 8 bits or 16 bits for checking
    // Need to use 8 bits or 16 bits for calculation


    if (len % 8)
    {
      /* Pad length out to byte boundary */
      len = (len & ~7) + 8;
    }

    reach_len = (len >> 3);

    txc_tx.buf.msg[1] = (byte) reach_len;  /* Save Msg_Length in bytes */

    /* Save Ext_Msg_Length as 1 and Msg_length in bytes */
    txc_tx.buf.msg[0] = (byte) ((reach_len | 0x8000) >> 8);

    b_packd( crc_30_calc( txc_tx.buf.msg, len-30 ), /* Calculate CRC */
             txc_tx.buf.msg, len-30, 30 );          /* Add to message */

  }
  else /* Msg_Length is 8 bits */
  {

    /* Zero the byte in which the message body ends */
    txc_tx.buf.msg[(len >> 3) + 1] = 0; /* Pad end with 0's */

    /* -----------------------------------------------------------------------
    ** If the (len mod 8) is between 3 and 7 (i.e. the message body uses more
    ** than 2 bits of its last byte) then the CRC will begin in the byte after
    ** the one in which the message body ends.  If that is the case, we need
    ** to also zero the two bits preceeding the 30 bit CRC in that byte.
    ** ----------------------------------------------------------------------- */
    txc_tx.buf.msg[(len >> 3) + 2] = 0; /* Pad end with 0's */

    /* Copy the actual message */
    b_copy( cmd_ptr->reach.msg_ptr, 0,       /* Source */
            &txc_tx.buf.msg[1], 0, len );  /* Destination, leave room for len */

    /* Add in CRC and MSG_LENGTH fields */
    len += CAI_AC_CRC_SIZE + CAI_AC_LGTH_SIZE;


    // CAI_AC_CRC_SIZE = 30 bits
    // CAI_AC_LGTH_SIZE = 8 bits
    // For REACH, MSG_LENGTH can be 8 bits or 16 bits
    // If len <= 978 bits, EXT_MSG_LENGTH = 0, MSG_LENGTH = 7 bits
    // If len > 978 bits, EXT_MSG_LENGTH = 1, MSG_LENGTH = 15 bits
    // Need to look at the place CAI_AC_LGTH_SIZE = 8 bits
    // Need to use 8 bits or 16 bits for checking
    // Need to use 8 bits or 16 bits for calculation


    if (len % 8)
    {
      /* Pad length out to byte boundary */
      len = (len & ~7) + 8;
    }

    txc_tx.buf.msg[0] = (byte) (len >> 3);  /* Save length in bytes */

    b_packd( crc_30_calc( txc_tx.buf.msg, len-30 ), /* Calculate CRC */
             txc_tx.buf.msg, len-30, 30 );          /* Add to message */

  } /*  end if (len > 978) */


  /* -----------------------------------------------------------------
  ** Before transmitting each access probe sequence, generate a random
  ** number, RA, from 0 to ACC_CHANs.  Use RA as the Access Channel
  ** number for all access probes in that access probe sequence.
  ** ----------------------------------------------------------------- */
  txc_reach_compute_access_chan();


   txc_reach_slot_length = (word) (cmd_ptr->reach.slot + 1);

   txc_reach_slot_offset1 = (word) (cmd_ptr->reach.slot_offset1);
   txc_reach_slot_offset2 = (word) (cmd_ptr->reach.slot_offset2);

   txc_reach_preamble_length =
   (word)((cmd_ptr->reach.preamble_num_frac + 1) * (cmd_ptr->reach.preamble_frac_duration + 1));


   txc_reach_rlgain_common_pilot = cmd_ptr->reach.rlgain_common_pilot;

   // IC_THRESH from the base station in units of 1/2 dB
   txc_reach_ic_thresh = cmd_ptr->reach.ic_thresh;

   // IC_MAX from the base station in units of  1/2 dB
   txc_reach_ic_max = cmd_ptr->reach.ic_max;

   // IC_THRESH in units of 1/2 dB for interference correction calculation
   // ic_thresh is 4 bit, product (*2) is 5 bit.
   txc_reach_ic_thresh = txc_reach_ic_thresh * 2;   //lint !e734: Loss of precision (9 to 8 bits)

   // IC_MAX in units of  1/2 dB for interference correction calculation
   // ic_max is 4 bit, product (*2) is 5 bit.
   txc_reach_ic_max = txc_reach_ic_max * 2; //lint !e734: Loss of precision (9 to 8 bits)


   M1X_MSG( MUX, LEGACY_HIGH,
     "rlgain=%x ic_thresh=%x ic_max=%x",
     txc_reach_rlgain_common_pilot,
     txc_reach_ic_thresh,
     txc_reach_ic_max);


  for( i = 0; i < ENC_SCH_RC3_16X_SIZE; i++ )
  {
    txtc.supp_frame.data[ i ] = 0x5A;
  }


  if ( cmd_ptr->reach.rate == TXC_EACH_RATE_9600_20_V )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "REACH rate 9600, 20ms");

    txc_reach_frame_length = ENC_SCH_FRAME_SIZE_20_V;

    txtc.supp_rate = ENC_SCH_FULL_RATE;

    txc_reach_frame_size = 172; /* 20ms, 9600bps, 172 information bits */
  }
  else if ( cmd_ptr->reach.rate == TXC_EACH_RATE_19200_20_V )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "REACH rate 19200, 20ms");

    txc_reach_frame_length = ENC_SCH_FRAME_SIZE_20_V;

    txtc.supp_rate = ENC_SCH_2X_RATE;

    txc_reach_frame_size = 360; /* 20ms, 19200bps, 360 information bits */
  }
  else if ( cmd_ptr->reach.rate == TXC_EACH_RATE_19200_10_V )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "REACH rate 19200, 10ms");

    txc_reach_frame_length = ENC_SCH_FRAME_SIZE_10_V;

    txtc.supp_rate = ENC_SCH_FULL_RATE;

    txc_reach_frame_size = 172; /* 10ms, 19200bps, 172 information bits */
  }
  else if ( cmd_ptr->reach.rate == TXC_EACH_RATE_38400_20_V )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "REACH rate 38400, 20ms");

    txc_reach_frame_length = ENC_SCH_FRAME_SIZE_20_V;

    txtc.supp_rate = ENC_SCH_4X_RATE;

    txc_reach_frame_size = 744; /* 20ms, 38400bps, 744 information bits */
  }
  else if ( cmd_ptr->reach.rate == TXC_EACH_RATE_38400_10_V )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "REACH rate 38400, 10ms");

    txc_reach_frame_length = ENC_SCH_FRAME_SIZE_10_V;

    txtc.supp_rate = ENC_SCH_2X_RATE;

    txc_reach_frame_size = 360; /* 10ms, 38400bps, 360 information bits */
  }
  else if ( cmd_ptr->reach.rate == TXC_EACH_RATE_38400_5_V )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "REACH rate 38400, 5ms");

    txc_reach_frame_length = ENC_SCH_FRAME_SIZE_5_V;

    txtc.supp_rate = ENC_SCH_FULL_RATE;

    txc_reach_frame_size = 172; /* 5ms, 38400bps, 172 information bits */
  }

  txc_reach_rate_word = cmd_ptr->reach.rate;

  /* Get REACH data gain based on the rate word for data channel. */
  txc_reach_gains.txc_sch_gain = rpc_get_reach_data_gain(
                                             cmd_ptr->reach.rate );

  /* Get REACH pilot gain based on the rate word for data channel. */
  txc_reach_gains.txc_pch_gain = rpc_get_reach_pilot_gain(
                                             cmd_ptr->reach.rate );

  /* Get REACH pilot gain adjustment for TX_ADJ based on the pilot gain. */
  txc_reach_gains.txc_tx_rate_adj = rpc_get_pch_adj_gain(PCH_GAIN_TRUNCATE_TO_8BITS
                                             (txc_reach_gains.txc_pch_gain) );

  /*
  ** Save length in bits to actually transmit. If we make this the multiple
  ** of the frame size then we will transmit all the frames of the
  ** message and transmit zeros when we run out of message.
  ** If the buf.len is not the integer number of the frame size,
  ** need to make the buf.len round up to the frame size and
  ** pad 0 bits to make the frame size.
  */
  if (txc_reach_si_include == TRUE)
  {
    /*
    ** Set transmitting buffer length to message length.
    ** txc_reach_access_att will copy SI bits of frame buffer with zeros,
    ** copy following message data bits from message buffer to frame buffer,
    ** copy remaining bits of frame buffer with zeros,
    ** transmit txc_reach_frame_size information bits.
    */

    txc_tx.buf.len = len;
  }
  else /* if (txc_reach_si_include == FALSE) */
  {
    if ( len % txc_reach_frame_size )
    {
      // We need to pad 0 bits here.

      // This set of operations will not overflow 16 bits because is divides by the
      // same amount it multiplies by, and the division occurs first.
      txc_tx.buf.len = ((len / txc_reach_frame_size) + 1) * txc_reach_frame_size; //lint !e734: Loss of precision (31 to 16 bits)
    }
    else
    {
      txc_tx.buf.len = len;
    }
  } /* end if (txc_reach_si_include == TRUE) */


  txtc.supp_turbo = FALSE;
  txtc.supp_ltu_size = 0;

  txc_tx.active_msg = TRUE;

  if (cmd_ptr->reach.tcb_ptr != NULL)
  {
    /* Save pointer to task to signal back and signal to use */
    txac.tcb_ptr = cmd_ptr->reach.tcb_ptr;
    txac.sigs = cmd_ptr->reach.sigs;
  }

} /* txc_reach_msg_prep */


//txc_reach_on

/*===========================================================================

FUNCTION TXC_REACH_PROBE_PREP

DESCRIPTION
  Process request to send an Enhanced Access Channel message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  txac and txc_tx buffers are set up for message transmission.

===========================================================================*/
void txc_reach_probe_prep
(
  txc_cmd_type *cmd_ptr
    /* Pointer to received command */
)
{
  word len;           /* Length of Access Channel message */

  word reach_len;     /* Length of Enhanced Access Channel message */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  len = cmd_ptr->reach.len;       /* Get Access Channel message length */

  /* Perform a check on the length of the Access Channel Message */
  /* Note that len is in bits.                                   */
  ASSERT(len <= ( (MAX_REV_MSG_BYTES*8) - (CAI_EA_CRC_SIZE + CAI_EA_LGTH_SIZE))); //lint !e506: Constant value Boolean

  if (len > 978) /* Ext_Msg_Length is 1 bit, Msg_Length is 15 bits */
  {
    /* Zero the byte in which the message body ends */
    txc_tx.buf.msg[(len >> 3) + 2] = 0; /* Pad end with 0's */

    /* -----------------------------------------------------------------------
    ** If the (len mod 8) is between 3 and 7 (i.e. the message body uses more
    ** than 2 bits of its last byte) then the CRC will begin in the byte after
    ** the one in which the message body ends.  If that is the case, we need
    ** to also zero the two bits preceeding the 30 bit CRC in that byte.
    ** ----------------------------------------------------------------------- */
    txc_tx.buf.msg[(len >> 3) + 3] = 0; /* Pad end with 0's */

    /* Copy the actual message */
    b_copy( cmd_ptr->reach.msg_ptr, 0,       /* Source */
              &txc_tx.buf.msg[2], 0, len );  /* Destination, leave room for len */

    /* Add in CRC and MSG_LENGTH fields */
    len += CAI_EA_CRC_SIZE + CAI_EA_LGTH_SIZE;

    if (len % 8)
    {
      /* Pad length out to byte boundary */
      len = (len & ~7) + 8;
    }

    reach_len = (len >> 3);

    txc_tx.buf.msg[1] = (byte) reach_len;  /* Save Msg_Length in bytes */

    /* Save Ext_Msg_Length as 1 and Msg_length in bytes */
    txc_tx.buf.msg[0] = (byte) ((reach_len | 0x8000) >> 8);

    b_packd( crc_30_calc( txc_tx.buf.msg, len-30 ), /* Calculate CRC */
             txc_tx.buf.msg, len-30, 30 );          /* Add to message */

  }
  else /* Msg_Length is 8 bits */
  {

    /* Zero the byte in which the message body ends */
    txc_tx.buf.msg[(len >> 3) + 1] = 0; /* Pad end with 0's */

    /* -----------------------------------------------------------------------
    ** If the (len mod 8) is between 3 and 7 (i.e. the message body uses more
    ** than 2 bits of its last byte) then the CRC will begin in the byte after
    ** the one in which the message body ends.  If that is the case, we need
    ** to also zero the two bits preceeding the 30 bit CRC in that byte.
    ** ----------------------------------------------------------------------- */
    txc_tx.buf.msg[(len >> 3) + 2] = 0; /* Pad end with 0's */

    /* Copy the actual message */
    b_copy( cmd_ptr->reach.msg_ptr, 0,       /* Source */
          &txc_tx.buf.msg[1], 0, len );  /* Destination, leave room for len */

    /* Add in CRC and MSG_LENGTH fields */
    len += CAI_AC_CRC_SIZE + CAI_AC_LGTH_SIZE;

    if (len % 8)
    {
      /* Pad length out to byte boundary */
      len = (len & ~7) + 8;
    }

    txc_tx.buf.msg[0] = (byte) (len >> 3);  /* Save length in bytes */

    b_packd( crc_30_calc( txc_tx.buf.msg, len-30 ), /* Calculate CRC */
             txc_tx.buf.msg, len-30, 30 );          /* Add to message */

  } /*  end if (len > 978) */

  /*
  ** Save length in bits to actually transmit. If we make this the multiple
  ** of the frame size then we will transmit all the frames of the
  ** message and transmit zeros when we run out of message.
  ** If the buf.len is not the integer number of the frame size,
  ** need to make the buf.len round up to the frame size and
  ** pad 0 bits to make the frame size.
  */
  if (txc_reach_si_include == TRUE)
  {
    /*
    ** Set transmitting buffer length to message length.
    ** txc_reach_access_att will copy SI bits of frame buffer with zeros,
    ** copy following message data bits from message buffer to frame buffer,
    ** copy remaining bits of frame buffer with zeros,
    ** transmit txc_reach_frame_size information bits.
    */

    txc_tx.buf.len = len;
  }
  else /* if (txc_reach_si_include == FALSE) */
  {
    if ( len % txc_reach_frame_size )
    {
      // We need to pad 0 bits here.

      // This set of operations will not overflow 16 bits because is divides by the
      // same amount it multiplies by, and the division occurs first.
      txc_tx.buf.len = ((len / txc_reach_frame_size) + 1) * txc_reach_frame_size; //lint !e734: Loss of precision (31 to 16 bits)
    }
    else
    {
      txc_tx.buf.len = len;
    }
  } /* end if (txc_reach_si_include == TRUE) */

  // We can just keep buf.len as len and do the pad remaining bits in transmittion
  // txc_tx.buf.len = len;

  txc_tx.active_msg = TRUE;

  if (cmd_ptr->reach.tcb_ptr != NULL)
  {
    /* Save pointer to task to signal back and signal to use */
    txac.tcb_ptr = cmd_ptr->reach.tcb_ptr;
    txac.sigs = cmd_ptr->reach.sigs;
  }

} /* txc_reach_probe_prep */

#endif // FEATURE_IS2000_REL_A_CC


/*===========================================================================

FUNCTION TXC_ACC_PROBE_PREP

DESCRIPTION
  Process request to send a probe of Access Channel message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  txac and txc_tx buffers are set up for message transmission.

===========================================================================*/
void txc_acc_probe_prep
(
  txc_cmd_type *cmd_ptr
    /* Pointer to received command */
)
{
  word len;           /* Length of Access Channel message */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  len = cmd_ptr->acc.len;       /* Get Access Channel message length */

  /* Perform a check on the length of the Access Channel Message */
  /* Note that len is in bits.                                   */
  ASSERT(len <= (CAI_AC_MAX_SIZE - (CAI_AC_CRC_SIZE + CAI_AC_LGTH_SIZE)));

  /* Zero the byte in which the message body ends */
  txc_tx.buf.msg[(len >> 3) + 1] = 0; /* Pad end with 0's */

  /* -----------------------------------------------------------------------
  ** If the (len mod 8) is between 3 and 7 (i.e. the message body uses more
  ** than 2 bits of its last byte) then the CRC will begin in the byte after
  ** the one in which the message body ends.  If that is the case, we need
  ** to also zero the two bits preceeding the 30 bit CRC in that byte.
  ** ----------------------------------------------------------------------- */
  txc_tx.buf.msg[(len >> 3) + 2] = 0; /* Pad end with 0's */

  /* Copy the actual message */
  b_copy( cmd_ptr->acc.msg_ptr, 0,       /* Source */
          &txc_tx.buf.msg[1], 0, len );  /* Destination, leave room for len */

  /* Add in CRC and MSG_LENGTH fields */
  len += CAI_AC_CRC_SIZE + CAI_AC_LGTH_SIZE;

  if (len % 8)
  {
    /* Pad length out to byte boundary */
    len = (len & ~7) + 8;
  }

  txc_tx.buf.msg[0] = (byte) (len >> 3);  /* Save length in bytes */

  b_packd( crc_30_calc( txc_tx.buf.msg, len-30 ), /* Calculate CRC */
           txc_tx.buf.msg, len-30, 30 );          /* Add to message */

  /*
  ** Save length in bits to actually transmit. If we make this the size
  ** of the message capsule then we will transmit all the frames of the
  ** capsule and transmit zeroes when we run out of message.
  */
  txc_tx.buf.len = (3 + cmd_ptr->acc.max_cap_sz) * TXC_ACC;

  txc_tx.active_msg = TRUE;

  if (cmd_ptr->acc.tcb_ptr != NULL)
  {
    /* Save pointer to task to signal back and signal to use */
    txac.tcb_ptr = cmd_ptr->acc.tcb_ptr;
    txac.sigs = cmd_ptr->acc.sigs;
  }
} /* txc_acc_probe_prep */

/*===========================================================================

FUNCTION TXC_ACC_PROBE_SUSPEND

DESCRIPTION
  Suspends the transmission of next Access probe.
  If the TA timer is running, sets the

DEPENDENCIES
  When this function is called, it is assumed that the TXC is in
  the middle of sending Access Probes. If it is not, then the function
  does not do anything.(We do not fatal error because this condition
  is possible in a Race Condition, when TXC is done with Max. Access Probes
  and MC meanwhile sends it SUSPEND).

RETURN VALUE
  None

SIDE EFFECTS
  If the TA timer is running, some variables are set so that to send
  ACC_PROBE_HO_OK sig for MC when the TA timer expires.
  If the TA timer is not running, ACC_PROBE_HO_OK sig is set.
===========================================================================*/
LOCAL void txc_acc_probe_suspend(txc_cmd_type * cmd_ptr)
{
  /* Check the state of TXC */
  if (txc_state != TXC_ACC_ATT_S)
  {
    /* This Suspend Command has come due to a Race Condition */
    /* Ignore it.                                            */
    M1X_MSG( MUX, LEGACY_MED,
      "Ignored SUSPEND Command in Non-Access State");
  }
  else
  {
    switch (txac.state)
    {
      case ACC_SETUP1_S:
        /* Suspend command issued even before TXC could start the Acc  */
        /* Msg transmission. Suspend probing without waiting for any   */
        /* timer etc.                                                  */
        M1X_MSG( MUX, LEGACY_MED,
          "Suspend in Msg Setup");
        txac.state = ACC_SUSPEND_S;
        break;
      case ACC_BACK1_S:
      case ACC_BACK2_S:
        /* RT Timer is running. Suspend the transmission of next probe */
        /* Also command MC to initiate the Access Probe Handoff        */
        M1X_MSG( MUX, LEGACY_MED,
          "Suspend when RT running");
        txac.state = ACC_SUSPEND_S;
        break;
      case ACC_PRE_S:
      case ACC_MSG_S:
        /* Transmitting the Probe. Wait till the transmission of the probe */
        /* is over. Signal MC to initiate Handoff when TA timer expires.   */

        /* Note the intentional "missing" break. Processing in this case   */
        /* is exactly as if TA timer is running.                           */
      case ACC_ENDPROBE_S:
        /* TA Timer is running. Wait till it expires */
        M1X_MSG( MUX, LEGACY_MED,
          "Suspend when TA Timer Running");
        txc_suspend_after_ta = TRUE;
        break;
      case ACC_SETUP2_S:
      case ACC_MSG_DONE_S:
        /* This is supposed to be a transition state only!! */
        M1X_MSG( MUX, LEGACY_MED,
          "Suspend in Msg Setup");
        txac.state = ACC_SUSPEND_S;
        break;
      case ACC_SUSPEND_S:
        M1X_MSG( MUX, LEGACY_HIGH,
          "MC sent Suspend in TXC Suspend state!!");
        break;
      default:
        M1X_MSG(MUX, LEGACY_ERROR,
          "Unknown state detected");
    }
  }

  /* If TXC has already gone into Suspended state, signal MC */
  /* to initiate the Access Probe Handoff                    */
  if (txac.state == ACC_SUSPEND_S)
  {
    (void) rex_set_sigs(MC_TCB_PTR,MCC_TXC_APHO_OK_SIG);
  }
  /* Save pointer to task to signal back and signal to use */
  txac.tcb_ptr = cmd_ptr->acc.tcb_ptr;
  txac.sigs = cmd_ptr->acc.sigs;
} /* txc_acc_probe_suspend */

/*===========================================================================

FUNCTION TXC_ACC_PROBE_RESUME

DESCRIPTION
  ReSumes the transmission of next Access probe.

DEPENDENCIES
  When this function is called, it is assumed that the TXC is in
  the Access Probe Suspend State. If it is not, then the function
  performs a FATAL ERROR.

RETURN VALUE
  None

SIDE EFFECTS
  The Sequence Number etc is set as per the Access Probe Handoff Requirements.
===========================================================================*/

void txc_acc_probe_resume(void)
{
  M1X_MSG( MUX, LEGACY_MED,
    "Resume Probing");

#ifdef FEATURE_IS2000_REL_A_CC
  if ( txc_reach_access == TRUE )
  {
    /* Compute REACH long code mask in txc_reach_access_att */

    txac.state = ACC_SETUP2_S;
    txac.countdown = 0;
  }
  else
#endif // FEATURE_IS2000_REL_A_CC
  {
    txc_compute_access_chan();

    txac.state = ACC_BACK2_S; /* Fake that RT timer has been running */
    txac.countdown = 0;       /* And, it has expired just now!!      */
  }

  /* Also set the Probe numbers etc to restart from first probe */
  /* of the Access Probe Sequence.                              */
  txac.probe_count = 0;       /* Probe no. in the current Probe Sequence  */
  txac.nstep = txac.num_step; /* No. of Probes left in the Probe Sequence */
  /* As per standard, We do not need to touch the Sequence Number */

  txac.power = txac.init_pwr; /* Transmit Power of the first probe in Sequence */

  /* We do not need to set the value of txac.first_frame. It must be */
  /* Properly set at this stage.                                     */

  txac.is95a_pwr = txac.power;
}


#ifdef FEATURE_IS2000_REL_A_CC
// txc_reach_on
// begin txc_reach_access_att
/*===========================================================================

FUNCTION TXC_REACH_ACCESS_ATT

DESCRIPTION
  State where access channel transmission is transmitted.

DEPENDENCIES
  None

RETURN VALUE
  New state

SIDE EFFECTS
  None

===========================================================================*/
txc_state_type txc_reach_access_att
(
  txc_state_type state
    /* txc_state - TXC_ACCESS_S */
)
{
  txc_cmd_type  *cmd_ptr;         /* Command from queue */
  rex_sigs_type sigs;             /* Task signals */
  word          dummy;            /* Temp var */
  static word   left;             /* Count bits left */
  boolean       psist_test_passed = FALSE;
  byte          pilot_ecio;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  txac.state = ACC_SETUP1_S;      /* Set up for first probe */
  txac.probe_count = 0;           /* Initialize to first probe sequence */
  txac.attempt_probe_count = 0;   /* Access attempt probe number */

  (void)enc_is95c_enable( TRUE );
  enc_pch_enable( TRUE );
  enc_sch_enable( TRUE );
  enc_fch_enable( FALSE );
  enc_dcch_enable( FALSE );

  /* Now tell the mdsp to set the channel gains to zero */
  muxmdsp_set_channel_gains_zero();

  #ifdef FEATURE_IS2000_1X_ADV
  muxmdsp_set_rc8_frame_config_parms_zero();
  #endif /* FEATURE_IS2000_1X_ADV */

  enc_sch_rc( ENC_SCH_RC_3 );

  txc_reach_first_frame_input_data   = FALSE;
  txc_reach_program_preamble_prepare = FALSE;
  txc_reach_program_data_prepare     = FALSE;
  txc_reach_preamble_count           = 0;
  txc_reach_data_count               = 0;
  txc_reach_intr_cnt                 = 0;
  txc_reach_preamble_frame_offset    = 0;
  txc_reach_data_frame_offset        = 0;
  txc_reach_frame_offset             = 0;

  // Number of 20ms duration frame count.
  txc_reach_20ms_frame_cnt = 0;

  /* -----------------------------------------------------------------
  ** NOTE: The interrupt handler takes care of feeding the actual data
  **       to the encoder.
  ** ----------------------------------------------------------------- */
  (void) rex_clr_sigs( TX_TCB_PTR, TXC_INT_SIG );

  do
  {
    /* Wait for command on the queue or a transmit frame interrupt */
    sigs = txc_wait( TXC_CMD_Q_SIG | TXC_INT_SIG );

    if ((sigs & TXC_CMD_Q_SIG) != 0)
    {
      (void) rex_clr_sigs( TX_TCB_PTR, TXC_CMD_Q_SIG );

      if ((cmd_ptr = (txc_cmd_type*) q_get( &txc_cmd_q )) != NULL)
      {
        /* Process the received command */
        state = txc_access_cmd( cmd_ptr, TXC_ACC_ATT_S );
      }
    } /* end if ((sigs & TXC_CMD_Q_SIG) != 0) */

    /* Handle signal from interrupt handler */
    else if ((sigs & TXC_INT_SIG) != 0)
    {
      (void) rex_clr_sigs( TX_TCB_PTR, TXC_INT_SIG );

      /* Assume no transmit */
      txac.frame.dotx = FALSE;

      if (txac.state == ACC_ENDPROBE_S)
      {
        M1X_MSG( MUX, LEGACY_HIGH,
          "ACC_ATT:  END PROBE" );

        /* Turn off the pilot and REACH data signals.  Set gain adjustments to zero
         * This is done only after the last frame has been transmitted. */
        if (( txac.countdown == txac.acc_tmo - 1 )
              || ( txac.countdown == 0 ))
        {
          enc_pch_enable(FALSE);
          enc_sch_enable(FALSE);

          /* Set the channel gains to zero */
          muxmdsp_set_channel_gains_zero();

          srch_mux_set_tx_adj(0x7F);
        }

        /* -------------------------------------------------
        ** Waiting for the Ack Response Timer (TA) to expire
        ** ------------------------------------------------- */
        if (txac.countdown == 0)
        {
          txac.state = ACC_MSG_DONE_S;

          M1X_MSG( MUX, LEGACY_HIGH,
            "ACC_ATT:  MSG DONE" );

          if (txc_suspend_after_ta)
          {
            txc_suspend_after_ta = FALSE;
            txac.state = ACC_SUSPEND_S;
            /* Signal MC to initiate Access Probe Handoff */
            (void) rex_set_sigs(MC_TCB_PTR,MCC_TXC_APHO_OK_SIG);
          }
        }
        else
        {
          txac.countdown--;
        }
      } /* end if (txac.state == ACC_ENDPROBE_S) */

      if (txac.state == ACC_MSG_DONE_S)
      {
        /* Check if the Probes are suspended due to T72M timer */
        if (!txac.stop_probes)
        {
          if (txac.nstep--)
          {
            /* ------------------------------------------------------
            ** More probes remaining in current access probe sequence
            ** ------------------------------------------------------ */

            txac.power = txac.is95a_pwr;

            /* Check if next power step will be greater than MAX */
            if ((txac.power >= 0x80) && ((txac.power - txac.pstep) < 0x80))
            {
              /* Limit power */
              txac.power = TXC_PWR_MAX;
            }
            else
            {
              /* Increase power */
              txac.power -= txac.pstep;
            }

            txac.is95a_pwr = txac.power;

            /* Redo the preamble */
            txac.state = ACC_SETUP2_S;

            M1X_MSG( MUX, LEGACY_HIGH,
              "ACC_ATT:        SETUP2 " );
          }
          else
          {
            /* ------------------------------------
            ** Have completed access probe sequence
            ** ------------------------------------ */
            if (++txac.seq_num >= txac.max_seq)
            {
              /* ---------------------------------------------------
              ** Maximum access probe sequence have been transmitted
              ** in this access attempt
              ** --------------------------------------------------- */
              state = TXC_ACC_IDLE_S;
              M1X_MSG( MUX, LEGACY_HIGH,
                "ACC_ATT: probes done; going back to idle" );

              /* Set flag to indicate no msg transmission in progress */
              txc_tx.active_msg = FALSE;
            }
            else
            {
              /* -----------------------------------
              ** Start another access probe sequence
              ** ----------------------------------- */
              txac.state = ACC_SETUP1_S;

              M1X_MSG( MUX, LEGACY_HIGH,
                "ACC_ATT:  SET UP 1" );

              txac.first_frame = FALSE;     /* Not first frame */
              txac.probe_count = 0;
              txac.power = txac.init_pwr;
              txac.nstep = txac.num_step;

              /* initialize the is95a power accumulation */
              txac.is95a_pwr = txac.init_pwr;
            }
          }
        }
        else
        {
          /* Temporarily disable the transmitter and stop sending
          ** any probes until further enabled */
          txac.state = ACC_SUSPEND_S;
        }
      } /* end if (txac.state == ACC_MSG_DONE_S) */

      if (txac.state == ACC_SUSPEND_S)
      {
        /*
        ** There is no need to perform any processing in this
        ** state at this time. Below is the comment from the IS-95B
        ** development branch
        */
        /* -------------------------------------------------------
        ** The TXC is in Suspended state, Transmitter is disabled
        ** and the Probe transmission is suspended. The TXC will
        ** resume transmission of the probes once it is re-enabled
        ** ----------------------------------------------------- */
      }

      if (txac.state == ACC_SETUP1_S || txac.state == ACC_SETUP2_S)
      {
        // Report event
        mclog_report_event_msg_txed(CAI_EACH, txc_tx.buf.msg);

          // current_frame_number = ( system_time - frame_offset ) / 16;

          (void) m1x_time_get( txc_reach_current_frame_number );

          (void) qw_div_by_power_of_2( txc_reach_current_frame_number,
                                       txc_reach_current_frame_number, 16 );

          /*
          ** At this point, txc_reach_current_frame_number is a PCG
          ** count.  Since the interrupt occurs using reverse link timing
          ** and the time_get() uses forward link timing, the time_get()
          ** could return a value earlier than the bin for which the interrupt
          ** occurred.  When this happens, the frame number computed here will
          ** be incorrect, leading to an incorrect long code mask, and the probe
          ** not being seen by the BS.
          **
          ** Fix for this is to add one to the PCG count here.  The last four
          ** bits are truncated below by the >>=4 and *=16.  So, adding one will
          ** not cause a problem.  In the case where the time_get() time is read
          ** at a time earlier than the interrupt occurred, this will correct
          ** for the back to the future constant.
          */
          qw_inc( txc_reach_current_frame_number, 1L);

          qw_dec( txc_reach_current_frame_number, (dword) txc_reach_frame_offset );

          (void) qw_div_by_power_of_2( txc_reach_current_frame_number,
                                       txc_reach_current_frame_number, 4 );

          // current_frame_pcg = current_frame_number * 16 + frame_offset;

          qw_mul( txc_reach_current_frame_pcg, txc_reach_current_frame_number, 16L );

          qw_inc( txc_reach_current_frame_pcg, (dword) txc_reach_frame_offset );

          M1X_MSG( MUX, LEGACY_HIGH,
            "current pcg = %x, lo= %x",
            qw_hi(txc_reach_current_frame_pcg),
            qw_lo(txc_reach_current_frame_pcg) );


          // next_frame_pcg = current_frame_pcg + 16;

          qw_equ( txc_reach_next_frame_pcg, txc_reach_current_frame_pcg );

          qw_inc( txc_reach_next_frame_pcg, 16L );

          M1X_MSG( MUX, LEGACY_HIGH,
            "next    pcg = %x, lo= %x",
            qw_hi(txc_reach_next_frame_pcg),
            qw_lo(txc_reach_next_frame_pcg) );


          // bkoff_frame_pcg = next_frame_pcg + (bkoff + rp) * each_slot

          dummy=0;

          if (txac.state == ACC_SETUP2_S)
          {
            /* ----------------------------------------------------------------
            ** If more than one Access Channel is supported by the Base Station
            ** ---------------------------------------------------------------- */
            if (txac.acc_chan > 1)
            {
              /* -------------------------------------------------------------
              ** Compute probe backoff delay, RT, from 0 to PROBE_BKOFF slots.
              **-------------------------------------------------------------- */
              dummy = ran_dist( txc_sec_ran_next( 0 ), 0, txac.probe_bkoff+1 );
            }
            else
            {
              /* ----------------------------------------------------------
              ** Compute probe backoff delay, RT, from 0 to 1 + PROBE_BKOFF
              ** slots.
              **----------------------------------------------------------- */
              dummy = ran_dist( txc_sec_ran_next( 0 ), 0, txac.probe_bkoff+2 );
            }
          } /*  end if (txac.state == ACC_SETUP2_S) */

          /* ----------------------------------------------------------
          ** Before transmitting each access probe sequence other than
          ** the first access probe sequence, the mobile station shall
          ** generate a random number, RS, from 0 to (BKOFFs + 1).  The
          ** mobile station shall delay the transmission of the access
          ** probe sequence for RS slots.
          ** ---------------------------------------------------------- */
          if ((txac.state == ACC_SETUP1_S) && (txac.seq_num != 0))
          {
            dummy = ran_dist( txc_sec_ran_next( 0 ), 0, txac.bkoff+2 );
          }

          // slot_length is 6 bits, and dummy is less than 8. So the product is less than 16
          txc_reach_countdown = dummy * txc_reach_slot_length; //lint !e734: Loss of precision (31 to 16 bits)

          // Persistence Test
          if ((txac.req) && (txac.state == ACC_SETUP1_S))
          {
            int num_psist_failures = 0;

            psist_test_passed = FALSE;

            do {

              dword ran;
              ran = txc_sec_ran_next( 0xFFFFFFFF );
              /*
              M1X_MSG( MUX, LEGACY_ERROR,
                "Comparing %lu and %lu",
                ran,
                txac.p);
              */

              if (ran < txac.p)
              {
                psist_test_passed = TRUE;
              }
              else
              {
                ++num_psist_failures;
                ++txc_reach_psist_delay;
              }
            } while ( (psist_test_passed==FALSE) && (num_psist_failures<96) );

            // If none of the persistence tests pass, send event
            if (!psist_test_passed)
            {
              event_report(EVENT_ACCESS_DENIED);
            }

            // Following product is 7bits * 6bits <= 13bits.
            txc_reach_countdown += num_psist_failures * txc_reach_slot_length; //lint !e734: Loss of precision (31 to 16 bits)
            /*
            M1X_MSG( MUX, LEGACY_ERROR,
              "Added %d to countdown for %d PSIST failures",
              num_psist_failures*txc_reach_slot_length,
              num_psist_failures);
            */
          }

          qw_equ( txc_reach_bkoff_frame_pcg, txc_reach_next_frame_pcg );

          qw_inc( txc_reach_bkoff_frame_pcg, (dword) txc_reach_countdown );

          txc_reach_count = qw_div( txc_reach_frame_pcg,
                             txc_reach_bkoff_frame_pcg, txc_reach_slot_length );

          if ( txc_reach_count != 0 )
          {
            txc_reach_count = txc_reach_slot_length - txc_reach_count;
          }

          // preamble_frame_pcg = bkoff_frame_pcg + count;

          qw_equ( txc_reach_preamble_frame_pcg, txc_reach_bkoff_frame_pcg );

          qw_inc( txc_reach_preamble_frame_pcg, (dword) txc_reach_count );

          M1X_MSG( MUX, LEGACY_HIGH,
            "preambl pcg = %x, lo= %x",
            qw_hi(txc_reach_preamble_frame_pcg),
            qw_lo(txc_reach_preamble_frame_pcg) );


          // data_frame_pcg = preamble_frame_pcg + preamble_length;

          qw_equ( txc_reach_data_frame_pcg, txc_reach_preamble_frame_pcg );

          qw_inc( txc_reach_data_frame_pcg, (dword) txc_reach_preamble_length );

          // preamble_frame_offset = (preamble_frame_pcg) mod 16;

          txc_reach_preamble_frame_offset =
          qw_div_by_power_of_2( txc_reach_frame_pcg,
                                txc_reach_preamble_frame_pcg, 4 );

          // data_frame_offset = (data_frame_pcg) mod 16;

          txc_reach_data_frame_offset =
          qw_div_by_power_of_2( txc_reach_frame_pcg,
                                txc_reach_data_frame_pcg, 4 );

          // frame_offset = data_frame_offset;

          txc_reach_frame_offset = txc_reach_data_frame_offset;

          // slot_offset = ( preamble_frame_pcg / each_slot )  mod 512;

          (void) qw_div( txc_reach_frame_pcg,
                         txc_reach_preamble_frame_pcg, txc_reach_slot_length );

          txc_reach_slot_offset =
          qw_div_by_power_of_2( txc_reach_frame_pcg, txc_reach_frame_pcg, 9 );

          // long_code_mask = fucntion( slot_offset );


          M1X_MSG( MUX, LEGACY_HIGH,
            "slot = %d, hex = %x, data = %d",
            txc_reach_slot_offset,
            txc_reach_slot_offset,
            txc_reach_data_frame_offset );


          /* txc_compute_reach_chan( txc_reach_slot_offset ); */
          txc_reach_compute_access_chan();

          // count_frame = (preamble_frame_pcg - next_frame_pcg) / 16;
          // count_frame = ((bkoff+rp)*(each_slot) + count) /16;
          // remainder = pcg_start;

          txc_reach_count_pcg = txc_reach_countdown + txc_reach_count;

          txc_reach_count_frame = txc_reach_count_pcg / 16;

          txc_reach_pcg_start = txc_reach_count_pcg % 16;

          txac.countdown = txc_reach_count_frame;

          // preamble_cnt = (pcg_start / 4); remainder = pcg_intr_cnt;

          txc_reach_preamble_count = txc_reach_pcg_start / 4;

          // txc_reach_preamble_pcg_intr_cnt = txc_reach_pcg_start % 4;

          // data_cnt = (preamble_length / 4); remainder = pcg_intr_cnt;

          txc_reach_data_count = txc_reach_preamble_length / 4;

          // txc_reach_data_pcg_intr_cnt = txc_reach_preamble_length % 4;

          // if (count_frame != 0) go to next frame until (count_frame==0);


          // if (preamble_cnt == 0) program preamble at (count_frame==0)
          // BIN_3 interrupt; else program preamble at frame after
          // (count_frame==0) preamble_cnt interrupt;


          // if (preamble_length < 4) program data_frame_offset at preamble
          // BIN_0 interrupt; else program data_frame_offset at data_cnt
          // interrupt;

          // double check current frame pcg

          // current_frame_number = ( system_time ) 48 bits (1.25 ms) 16 bits (1/32 chip);

          (void) m1x_time_get( txc_reach_current_frame_number );

          (void) qw_div_by_power_of_2( txc_reach_current_frame_number,
                                       txc_reach_current_frame_number, 16 );

          M1X_MSG( MUX, LEGACY_HIGH,
            "current time = %x, lo= %x",
            qw_hi(txc_reach_current_frame_number),
            qw_lo(txc_reach_current_frame_number) );


        if (txac.state == ACC_SETUP1_S)
        {
          txac.state = ACC_BACK1_S;

          M1X_MSG( MUX, LEGACY_HIGH,
            "ACC_ATT: SETUP1  BACK1" );

        }
        else
        {
          txac.state = ACC_BACK2_S;

          M1X_MSG( MUX, LEGACY_HIGH,
            "ACC_ATT: SETUP2      BACK2" );

        }
      } /* end if (txac.state == ACC_SETUP1_S || ACC_SETUP2_S) */

      if (txac.state == ACC_BACK1_S || txac.state == ACC_BACK2_S)
      {
        if ( (txac.req) && (txac.state == ACC_BACK1_S) && (!psist_test_passed) && (txac.countdown == 0) )
        {
          // The persistence test failed for all the attempts.
          // Retry in the setup state again.
          txac.state = ACC_SETUP1_S;
        }
        else if (txac.countdown == 0)
        {

            /* -------------------------------------------------------------
            ** the interference correction adjustment for the transmit power
            **
            ** note: srch_get_pilot_ecio return pilot_ecio in 1/2db
            ** ------------------------------------------------------------- */
            pilot_ecio = srch_mux_get_pilot_ecio();
            /*lint -e732 */
            // txac.int_corr = MIN( MAX(-14+pilot_ecio,0), 14 );
            /*lint +e732 */

            txac.int_corr = MIN( MAX( (pilot_ecio - txc_reach_ic_thresh) ,0), txc_reach_ic_max );

            /* Check if add interference correction power will be greater than MAX */
            if ((txac.is95a_pwr >= 0x80) && ((txac.is95a_pwr - txac.int_corr) < 0x80))
            {
               /* Limit power */
               txac.power = TXC_PWR_MAX;
            }
            else
            {
               /* Increase power */
               txac.power = txac.is95a_pwr - txac.int_corr;
            }

            M1X_MSG( MUX, LEGACY_MED,
              "Seq=%d probe=%d ecio=%d, "
              "Tx adj=%d 95A=%d i-corr=%d",
              txac.seq_num+1,
              txac.probe_count+1,
              pilot_ecio/2,
              (-((signedchar)txac.power))/2,
              (-((signedchar)txac.is95a_pwr))/2,
              txac.int_corr/2);

            /* Dump out Rx/Tx timing status */
            muxmdsp_print_tx_timing_info( FALSE );

            /* Begin transmitting Access Channel preamble */
            txac.state = ACC_PRE_S;

            txc_reach_first_frame_input_data = FALSE;

            txac.frame.dotx = TRUE;

            /* Make sure power amp is on */
            enc_tx_enable( TRUE );

            /* Set transmit power */
            srch_mux_set_tx_adj( (byte) txac.power );
            txac.last_pwr = txac.power;    /* Save last power used for traffic */

            txc_tx.buf.pos   = 0;         /* Start at msg buffer position 0 */
            txac.first_frame = TRUE;

            // Input (first frame) data at interrupt during ACC_PRE_S
            // txc_tx.buf.pos needs to add TXC_ACC here

            // Number of 20ms duration frame count.
            txc_reach_20ms_frame_cnt = 1;

            /* Calculate how much of the message is left to send */
            left = txc_tx.buf.len - txc_tx.buf.pos;

            if (txc_reach_si_include == TRUE)
            {
              if (txc_reach_frame_length == ENC_SCH_FRAME_SIZE_20_V)
              {
                if (left >= (txc_reach_frame_size - 2))   /* Lots of message left */
                {
                  /* Copy SI bits of frame buffer with zeroes */
                  b_copy( zeros, 0, txtc.supp_frame.data, 0, 2 );

                  /* Copy full frame of msg data from msg buffer to frame buffer */
                  b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                          txtc.supp_frame.data, 2, (txc_reach_frame_size - 2) );
                }
                else
                {
                  /* Copy SI bits of frame buffer with zeroes */
                  b_copy( zeros, 0, txtc.supp_frame.data, 0, 2 );

                  /* Copy remaining bits of msg data from msg buffer to frame buf */
                  b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                          txtc.supp_frame.data, 2, left );

                  /* Pad remaining bits of frame buffer with zeroes */
                  b_copy( zeros, 0, txtc.supp_frame.data, (left + 2),
                                   (txc_reach_frame_size - left - 2) );
                }

                /* Advance message buffer index */
                txc_tx.buf.pos += (txc_reach_frame_size - 2);
              }
              else /* if (txc_reach_frame_length != ENC_SCH_FRAME_SIZE_20_V)*/
              {
                // 10ms frame, build frame 1, bin 0, bin 2
                //  5ms frame, build frame 1, bin 0, bin 1, bin 2, bin 3

                /* Lots of message left, build frame 1, bin 0 */
                if (left >= (txc_reach_frame_size - 2))
                {
                  /* Copy SI bits of frame buffer with zeroes */
                  b_copy( zeros, 0, txc_reach_frame_1_bin_0, 0, 2 );

                  /* Copy full frame of msg data from msg buffer to frame buffer */
                  b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                          txc_reach_frame_1_bin_0, 2, (txc_reach_frame_size - 2) );

                  /* Advance message buffer index */
                  txc_tx.buf.pos += (txc_reach_frame_size - 2);

                  /* Calculate how much of the message is left to send
                     after building frame 1, bin 0 buffer. */
                  left = txc_tx.buf.len - txc_tx.buf.pos;

                  //5ms
                  /* 5ms frame builds data for bin1, bin2, bin3 */
                  if ( txc_reach_frame_length == ENC_SCH_FRAME_SIZE_5_V )
                  {

                  /* Lots of message left, build frame 1, bin 1 */
                  if (left >= (txc_reach_frame_size - 2))
                  {
                    /* Copy SI bits of frame buffer with zeroes */
                    b_copy( zeros, 0, txc_reach_frame_1_bin_1, 0, 2 );

                    /* Copy full frame of msg data from msg buffer to
                       frame 1, bin 1 buffer. */
                    b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                            txc_reach_frame_1_bin_1, 2, (txc_reach_frame_size - 2) );

                    /* Advance message buffer index after frame 1, bin 1 */
                    txc_tx.buf.pos += (txc_reach_frame_size - 2);

                    /* Calculate how much of the message is left to send
                       after building frame 1, bin 1 buffer. */
                    left = txc_tx.buf.len - txc_tx.buf.pos;

                    /* Lots of message left, build frame 1, bin 2 */
                    if (left >= (txc_reach_frame_size - 2))
                    {
                      /* Copy SI bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_1_bin_2, 0, 2 );

                      /* Copy full frame of msg data from msg buffer to
                         frame 1, bin 2 buffer. */
                      b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                              txc_reach_frame_1_bin_2, 2, (txc_reach_frame_size - 2) );

                      /* Advance message buffer index after frame 1, bin 2*/
                      txc_tx.buf.pos += (txc_reach_frame_size - 2);

                      /* Calculate how much of the message is left to send
                         after building frame 1, bin 2 buffer. */
                      left = txc_tx.buf.len - txc_tx.buf.pos;

                      /* Lots of message left, build frame 1, bin 3 */
                      if (left >= (txc_reach_frame_size - 2))
                      {
                        /* Copy SI bits of frame buffer with zeroes */
                        b_copy( zeros, 0, txc_reach_frame_1_bin_3, 0, 2 );

                        /* Copy full frame of msg data from msg buffer to
                           frame 1, bin 3 buffer. */
                        b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                                txc_reach_frame_1_bin_3, 2, (txc_reach_frame_size - 2) );

                        /* Advance message buffer index after frame 1, bin 3 */
                        txc_tx.buf.pos += (txc_reach_frame_size - 2);

                      }
                      else /* if (one frame of message left, build frame 1, bin 3) */
                      {
                        /* Copy SI bits of frame buffer with zeroes */
                        b_copy( zeros, 0, txc_reach_frame_1_bin_3, 0, 2 );

                        /* Copy remaining bits of msg data from msg buffer to frame buf */
                        b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                                txc_reach_frame_1_bin_3, 2, left );

                        /* Pad remaining bits of frame buffer with zeroes */
                        b_copy( zeros, 0, txc_reach_frame_1_bin_3, (left + 2),
                                         (txc_reach_frame_size - left - 2) );

                        /* Advance message buffer index */
                        txc_tx.buf.pos += (txc_reach_frame_size - 2);
                      } /* endif (Lots of message left, build frame 1, bin 3) */

                    }
                    else /* if (one frame of message left, build frame 1, bin 2) */
                    {
                      /* Copy SI bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_1_bin_2, 0, 2 );

                      /* Copy remaining bits of msg data from msg buffer to frame buf */
                      b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                              txc_reach_frame_1_bin_2, 2, left );

                      /* Pad remaining bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_1_bin_2, (left + 2),
                                       (txc_reach_frame_size - left - 2) );

                      /* Advance message buffer index after frame 1, bin 2 */
                      txc_tx.buf.pos += (txc_reach_frame_size - 2);
                    } /* endif (Lots of message left, build frame 1, bin 2) */

                  }
                  else /* if (one frame of message left, build frame 1, bin 1) */
                  {
                    /* Copy SI bits of frame buffer with zeroes */
                    b_copy( zeros, 0, txc_reach_frame_1_bin_1, 0, 2 );

                    /* Copy remaining bits of msg data from msg buffer to frame buf */
                    b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                            txc_reach_frame_1_bin_1, 2, left );

                    /* Pad remaining bits of frame buffer with zeroes */
                    b_copy( zeros, 0, txc_reach_frame_1_bin_1, (left + 2),
                                     (txc_reach_frame_size - left - 2) );

                    /* Advance message buffer index after frame 1, bin 1 */
                    txc_tx.buf.pos += (txc_reach_frame_size - 2);

                  } /* endif (Lots of message left, build frame 1, bin 1) */

                  } //5ms
                  else /* if ( txc_reach_frame_length == ENC_SCH_FRAME_SIZE_10_V ) */
                  { //10ms builds data for bin2

                    /* Lots of message left, build frame 1, bin 2 */
                    if (left >= (txc_reach_frame_size - 2))
                    {
                      /* Copy SI bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_1_bin_2, 0, 2 );

                      /* Copy full frame of msg data from msg buffer to
                         frame 1, bin 2 buffer. */
                      b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                      txc_reach_frame_1_bin_2, 2, (txc_reach_frame_size - 2) );

                      /* Advance message buffer index after frame 1, bin 2*/
                      txc_tx.buf.pos += (txc_reach_frame_size - 2);

                    }
                    else /* if (one frame of message left, build frame 1, bin 2) */
                    {
                      /* Copy SI bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_1_bin_2, 0, 2 );

                      /* Copy remaining bits of msg data from msg buffer to frame buf */
                      b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                              txc_reach_frame_1_bin_2, 2, left );

                      /* Pad remaining bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_1_bin_2, (left + 2),
                              (txc_reach_frame_size - left - 2) );

                      /* Advance message buffer index after frame 1, bin 2 */
                      txc_tx.buf.pos += (txc_reach_frame_size - 2);
                    } /* endif (Lots of message left, build frame 1, bin 2) */

                    //10ms
                  } /* endif ( txc_reach_frame_length == ENC_SCH_FRAME_SIZE_5_V ) */

                }
                else /* if (one frame of message left, build frame 1, bin 0) */
                {
                  /* Copy SI bits of frame buffer with zeroes */
                  b_copy( zeros, 0, txc_reach_frame_1_bin_0, 0, 2 );

                  /* Copy remaining bits of msg data from msg buffer to frame buf */
                  b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                          txc_reach_frame_1_bin_0, 2, left );

                  /* Pad remaining bits of frame buffer with zeroes */
                  b_copy( zeros, 0, txc_reach_frame_1_bin_0, (left + 2),
                                   (txc_reach_frame_size - left - 2) );

                  /* Advance message buffer index after frame 1, bin 0 */
                  txc_tx.buf.pos += (txc_reach_frame_size - 2);

                } /* endif (lots of message left, build frame 1, bin 0) */

              } /* endif (txc_reach_frame_length == ENC_SCH_FRAME_SIZE_20_V)*/
            }
            else /* if (txc_reach_si_include == FALSE) */
            {

              if (left >= txc_reach_frame_size)   /* Lots of message left */
              {
                /* Copy full frame of msg data from msg buffer to frame buffer */
                b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                        txtc.supp_frame.data, 0, txc_reach_frame_size );
              }
              else
              {
                /* Copy remaining bits of msg data from msg buffer to frame buf */
                b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                        txtc.supp_frame.data, 0, left );

                /* Pad remaining bits of frame buffer with zeroes */
                b_copy( zeros, 0, txtc.supp_frame.data, left,
                                  (txc_reach_frame_size - left) );
              }

              /* Advance message buffer index */
              txc_tx.buf.pos += txc_reach_frame_size;

            } /* end if (txc_reach_si_include == TRUE) */
        }
        else /* Backoff timer has not expired */
        {
          txac.countdown--;
          /*
          M1X_MSG( MUX, LEGACY_ERROR,
            "Backoff Countdown %d",
            txac.countdown );
          */
        }
      } /* end if (txac.state == ACC_BACK1_S || txac.state == ACC_BACK2_S) */

      if (txac.state == ACC_MSG_S) /* Message transmit substate */
      {
        /* REACH builds the first frame of data at ACC_PRE_S and updates
        ** txc_tx.buf.pos += (txc_reach_frame_size - 2) for SI included.
        ** If the complete message fits into the first frame of data,
        ** then the buf.pos will go over the buf.len.  Therefore, we
        ** need to first check and make sure that we meet the condition of
        ** (txc_tx.buf.pos < txc_tx.buf.len), then we build the second
        ** frame of data at ACC_MSG_S.  Otherwase, we need to set ENDPROBE_S
        ** when (txc_tx.buf.pos >= txc_tx.buf.len).
        ** REACH uses CAI_REV_CSCH_MAX_MSG_BYTES (345 bytes) to allocate
        ** the message buffer.  345 bytes = 2760 bits = AC8 bits.
        ** Because word buf.len, word buf.pos are 16 bits unsigned data,
        ** therefore, they can hold the maximum REACH message length.
        ** Also, the maximum buf.pos will fit into 16 bits unsigned data,
        ** because of 2760 + 744 = 3504 = DB0 hex, where 2760 is the maximum
        ** number of message data, and 744 is the maximum information bits
        ** for 20 ms frame size, 38400 bps data rates.
        */
        if (txc_tx.buf.pos < txc_tx.buf.len)
        {
          txac.frame.dotx = TRUE; /* Enable transmit */

          // txc_tx.buf.len = ( 3 + MAX_CAP_SZ ) * ACH_FRAME_SIZE
          // txc_tx.buf.len = ( 3 + MAX_CAP_SZ ) * TXC_ACC

          // Input (first frame) data at interrupt during ACC_PRE_S
          // Input (second frame to last frame) data at interrupt during ACC_MSG_S
          // txc_tx.buf.len should substract TXC_ACC during ACC_PRE_S data input
          // txc_tx.buf.len should take one less TXC_ACC here for calculation
          // If MAX_CAP_SZ = 0, then txc_tx.buf.len = 3 * TXC_ACC
          // After ACC_PRE_S data input, txc_tx.buf.len = 2 * TXC_ACC
          // During ACC_MSG_S here, we use 2 * TXC_ACC for txc_tx.buf.len

          /* Calculate how much of the message is left to send */
          left = txc_tx.buf.len - txc_tx.buf.pos;

          // Increment number of 20ms duration frame count.
          txc_reach_20ms_frame_cnt++;

          if (txc_reach_si_include == TRUE)
          {
            if (txc_reach_frame_length == ENC_SCH_FRAME_SIZE_20_V)
            {

              if (left >= (txc_reach_frame_size - 2))   /* Lots of message left */
              {
                /* Copy SI bits of frame buffer with zeroes */
                b_copy( zeros, 0, txtc.supp_frame.data, 0, 2 );

                /* Copy full frame of msg data from msg buffer to frame buffer */
                b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                        txtc.supp_frame.data, 2, (txc_reach_frame_size - 2) );
              }
              else
              {
                /* Copy SI bits of frame buffer with zeroes */
                b_copy( zeros, 0, txtc.supp_frame.data, 0, 2 );

                /* Copy remaining bits of msg data from msg buffer to frame buf */
                b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                        txtc.supp_frame.data, 2, left );

                /* Pad remaining bits of frame buffer with zeroes */
                b_copy( zeros, 0, txtc.supp_frame.data, (left + 2),
                                 (txc_reach_frame_size - left - 2) );
              }

              /* Advance message buffer index */
              txc_tx.buf.pos += (txc_reach_frame_size - 2);

            }
            else /* if (txc_reach_frame_length != ENC_SCH_FRAME_SIZE_20_V)*/
            {
              // 10ms frame, build frame 1, bin 0, bin 2
              //  5ms frame, build frame 1, bin 0, bin 1, bin 2, bin 3

              /* Lots of message left, build frame 1, bin 0 */
              if (left >= (txc_reach_frame_size - 2))
              {
                if (txc_reach_20ms_frame_cnt % 2)
                {
                  /* Copy SI bits of frame buffer with zeroes */
                  b_copy( zeros, 0, txc_reach_frame_1_bin_0, 0, 2 );

                  /* Copy full frame of msg data from msg buffer to frame buffer */
                  b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                          txc_reach_frame_1_bin_0, 2, (txc_reach_frame_size - 2) );
                }
                else /* Build frame 2 */
                {
                  /* Copy SI bits of frame buffer with zeroes */
                  b_copy( zeros, 0, txc_reach_frame_2_bin_0, 0, 2 );

                  /* Copy full frame of msg data from msg buffer to frame buffer */
                  b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                          txc_reach_frame_2_bin_0, 2, (txc_reach_frame_size - 2) );
                }

                /* Advance message buffer index */
                txc_tx.buf.pos += (txc_reach_frame_size - 2);

                /* Calculate how much of the message is left to send
                   after building frame 1, bin 0 buffer. */
                left = txc_tx.buf.len - txc_tx.buf.pos;

                //5ms
                /* 5ms frame builds data for bin1, bin2, bin3 */
                if ( txc_reach_frame_length == ENC_SCH_FRAME_SIZE_5_V )
                {

                  /* Lots of message left, build frame 1, bin 1 */
                  if (left >= (txc_reach_frame_size - 2))
                  {
                    if (txc_reach_20ms_frame_cnt % 2)
                    {
                      /* Copy SI bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_1_bin_1, 0, 2 );

                      /* Copy full frame of msg data from msg buffer to
                         frame 1, bin 1 buffer. */
                      b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                              txc_reach_frame_1_bin_1, 2, (txc_reach_frame_size - 2) );
                    }
                    else /* Build frame 2 */
                    {
                      /* Copy SI bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_2_bin_1, 0, 2 );

                      /* Copy full frame of msg data from msg buffer to
                         frame 1, bin 1 buffer. */
                      b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                              txc_reach_frame_2_bin_1, 2, (txc_reach_frame_size - 2) );
                    }

                    /* Advance message buffer index after frame 1, bin 1 */
                    txc_tx.buf.pos += (txc_reach_frame_size - 2);

                    /* Calculate how much of the message is left to send
                       after building frame 1, bin 1 buffer. */
                    left = txc_tx.buf.len - txc_tx.buf.pos;

                    /* Lots of message left, build frame 1, bin 2 */
                    if (left >= (txc_reach_frame_size - 2))
                    {
                      if (txc_reach_20ms_frame_cnt % 2)
                      {
                        /* Copy SI bits of frame buffer with zeroes */
                        b_copy( zeros, 0, txc_reach_frame_1_bin_2, 0, 2 );

                        /* Copy full frame of msg data from msg buffer to
                           frame 1, bin 2 buffer. */
                        b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                                txc_reach_frame_1_bin_2, 2, (txc_reach_frame_size - 2) );
                      }
                      else /* Build frame 2 */
                      {
                        /* Copy SI bits of frame buffer with zeroes */
                        b_copy( zeros, 0, txc_reach_frame_2_bin_2, 0, 2 );

                        /* Copy full frame of msg data from msg buffer to
                           frame 1, bin 2 buffer. */
                        b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                                txc_reach_frame_2_bin_2, 2, (txc_reach_frame_size - 2) );
                      }

                      /* Advance message buffer index after frame 1, bin 2*/
                      txc_tx.buf.pos += (txc_reach_frame_size - 2);

                      /* Calculate how much of the message is left to send
                         after building frame 1, bin 2 buffer. */
                      left = txc_tx.buf.len - txc_tx.buf.pos;

                      /* Lots of message left, build frame 1, bin 3 */
                      if (left >= (txc_reach_frame_size - 2))
                      {
                        if (txc_reach_20ms_frame_cnt % 2)
                        {
                          /* Copy SI bits of frame buffer with zeroes */
                          b_copy( zeros, 0, txc_reach_frame_1_bin_3, 0, 2 );

                          /* Copy full frame of msg data from msg buffer to
                             frame 1, bin 3 buffer. */
                          b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                                  txc_reach_frame_1_bin_3, 2, (txc_reach_frame_size - 2) );
                        }
                        else /* Build frame 2 */
                        {
                          /* Copy SI bits of frame buffer with zeroes */
                          b_copy( zeros, 0, txc_reach_frame_2_bin_3, 0, 2 );

                          /* Copy full frame of msg data from msg buffer to
                             frame 1, bin 3 buffer. */
                          b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                                  txc_reach_frame_2_bin_3, 2, (txc_reach_frame_size - 2) );
                        }

                        /* Advance message buffer index after frame 1, bin 3 */
                        txc_tx.buf.pos += (txc_reach_frame_size - 2);

                      }
                      else /* if (one frame of message left, build frame 1, bin 3) */
                      {
                        if (txc_reach_20ms_frame_cnt % 2)
                        {
                          /* Copy SI bits of frame buffer with zeroes */
                          b_copy( zeros, 0, txc_reach_frame_1_bin_3, 0, 2 );

                          /* Copy remaining bits of msg data from msg buffer to frame buf */
                          b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                                  txc_reach_frame_1_bin_3, 2, left );

                          /* Pad remaining bits of frame buffer with zeroes */
                          b_copy( zeros, 0, txc_reach_frame_1_bin_3, (left + 2),
                                           (txc_reach_frame_size - left - 2) );
                        }
                        else /* Build frame 2 */
                        {
                          /* Copy SI bits of frame buffer with zeroes */
                          b_copy( zeros, 0, txc_reach_frame_2_bin_3, 0, 2 );

                          /* Copy remaining bits of msg data from msg buffer to frame buf */
                          b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                                  txc_reach_frame_2_bin_3, 2, left );

                            /* Pad remaining bits of frame buffer with zeroes */
                          b_copy( zeros, 0, txc_reach_frame_2_bin_3, (left + 2),
                                           (txc_reach_frame_size - left - 2) );
                        }

                        /* Advance message buffer index */
                        txc_tx.buf.pos += (txc_reach_frame_size - 2);
                      } /* endif (Lots of message left, build frame 1, bin 3) */

                    }
                    else /* if (one frame of message left, build frame 1, bin 2) */
                    {
                      if (txc_reach_20ms_frame_cnt % 2)
                      {
                        /* Copy SI bits of frame buffer with zeroes */
                        b_copy( zeros, 0, txc_reach_frame_1_bin_2, 0, 2 );

                        /* Copy remaining bits of msg data from msg buffer to frame buf */
                        b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                                txc_reach_frame_1_bin_2, 2, left );

                        /* Pad remaining bits of frame buffer with zeroes */
                        b_copy( zeros, 0, txc_reach_frame_1_bin_2, (left + 2),
                                         (txc_reach_frame_size - left - 2) );
                      }
                      else /* Build frame 2 */
                      {
                        /* Copy SI bits of frame buffer with zeroes */
                        b_copy( zeros, 0, txc_reach_frame_2_bin_2, 0, 2 );

                        /* Copy remaining bits of msg data from msg buffer to frame buf */
                        b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                              txc_reach_frame_2_bin_2, 2, left );

                        /* Pad remaining bits of frame buffer with zeroes */
                        b_copy( zeros, 0, txc_reach_frame_2_bin_2, (left + 2),
                                         (txc_reach_frame_size - left - 2) );
                      }

                      /* Advance message buffer index after frame 1, bin 2 */
                      txc_tx.buf.pos += (txc_reach_frame_size - 2);
                    } /* endif (Lots of message left, build frame 1, bin 2) */

                  }
                  else /* if (one frame of message left, build frame 1, bin 1) */
                  {
                    if (txc_reach_20ms_frame_cnt % 2)
                    {
                      /* Copy SI bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_1_bin_1, 0, 2 );

                      /* Copy remaining bits of msg data from msg buffer to frame buf */
                      b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                              txc_reach_frame_1_bin_1, 2, left );

                      /* Pad remaining bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_1_bin_1, (left + 2),
                                       (txc_reach_frame_size - left - 2) );
                    }
                    else /* Build frame 2 */
                    {
                      /* Copy SI bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_2_bin_1, 0, 2 );

                      /* Copy remaining bits of msg data from msg buffer to frame buf */
                      b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                              txc_reach_frame_2_bin_1, 2, left );

                      /* Pad remaining bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_2_bin_1, (left + 2),
                                       (txc_reach_frame_size - left - 2) );
                    }

                    /* Advance message buffer index after frame 1, bin 1 */
                    txc_tx.buf.pos += (txc_reach_frame_size - 2);

                  } /* endif (Lots of message left, build frame 1, bin 1) */

                } //5ms
                else /* if ( txc_reach_frame_length == ENC_SCH_FRAME_SIZE_10_V ) */
                { //10ms builds data for bin2

                  /* Lots of message left, build frame 1, bin 2 */
                  if (left >= (txc_reach_frame_size - 2))
                  {
                    if (txc_reach_20ms_frame_cnt % 2)
                    {
                      /* Copy SI bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_1_bin_2, 0, 2 );

                      /* Copy full frame of msg data from msg buffer to
                         frame 1, bin 2 buffer. */
                      b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                      txc_reach_frame_1_bin_2, 2, (txc_reach_frame_size - 2) );
                    }
                    else /* Build frame 2 */
                    {
                      /* Copy SI bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_2_bin_2, 0, 2 );

                      /* Copy full frame of msg data from msg buffer to
                         frame 1, bin 2 buffer. */
                      b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                      txc_reach_frame_2_bin_2, 2, (txc_reach_frame_size - 2) );
                    }

                    /* Advance message buffer index after frame 1, bin 2*/
                    txc_tx.buf.pos += (txc_reach_frame_size - 2);
                  }
                  else /* if (one frame of message left, build frame 1, bin 2) */
                  {
                    if (txc_reach_20ms_frame_cnt % 2)
                    {
                      /* Copy SI bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_1_bin_2, 0, 2 );

                      /* Copy remaining bits of msg data from msg buffer to frame buf */
                      b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                              txc_reach_frame_1_bin_2, 2, left );

                      /* Pad remaining bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_1_bin_2, (left + 2),
                             (txc_reach_frame_size - left - 2) );
                    }
                    else /* Build frame 2 */
                    {
                      /* Copy SI bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_2_bin_2, 0, 2 );

                      /* Copy remaining bits of msg data from msg buffer to frame buf */
                      b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                              txc_reach_frame_2_bin_2, 2, left );

                      /* Pad remaining bits of frame buffer with zeroes */
                      b_copy( zeros, 0, txc_reach_frame_2_bin_2, (left + 2),
                             (txc_reach_frame_size - left - 2) );
                    }

                    /* Advance message buffer index after frame 1, bin 2 */
                    txc_tx.buf.pos += (txc_reach_frame_size - 2);
                  } /* endif (Lots of message left, build frame 1, bin 2) */

                  //10ms
                } /* endif ( txc_reach_frame_length == ENC_SCH_FRAME_SIZE_5_V ) */

              }
              else /* if (one frame of message left, build frame 1, bin 0) */
              {
                if (txc_reach_20ms_frame_cnt % 2)
                {
                  /* Copy SI bits of frame buffer with zeroes */
                  b_copy( zeros, 0, txc_reach_frame_1_bin_0, 0, 2 );

                  /* Copy remaining bits of msg data from msg buffer to frame buf */
                  b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                  txc_reach_frame_1_bin_0, 2, left );

                  /* Pad remaining bits of frame buffer with zeroes */
                  b_copy( zeros, 0, txc_reach_frame_1_bin_0, (left + 2),
                                   (txc_reach_frame_size - left - 2) );
                }
                else /* Build frame 2 */
                {
                  /* Copy SI bits of frame buffer with zeroes */
                  b_copy( zeros, 0, txc_reach_frame_2_bin_0, 0, 2 );

                  /* Copy remaining bits of msg data from msg buffer to frame buf */
                  b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                          txc_reach_frame_2_bin_0, 2, left );

                  /* Pad remaining bits of frame buffer with zeroes */
                  b_copy( zeros, 0, txc_reach_frame_2_bin_0, (left + 2),
                                   (txc_reach_frame_size - left - 2) );
                }

                /* Advance message buffer index after frame 1, bin 0 */
                txc_tx.buf.pos += (txc_reach_frame_size - 2);

              } /* endif (lots of message left, build frame 1, bin 0) */

            } /* endif (txc_reach_frame_length == ENC_SCH_FRAME_SIZE_20_V)*/

          }
          else /* if (txc_reach_si_include == FALSE) */
          {

            if (left >= txc_reach_frame_size)   /* Lots of message left */
            {
              /* Copy full frame of msg data from msg buffer to frame buffer */
              b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                      txtc.supp_frame.data, 0, txc_reach_frame_size );
            }
            else
            {
              /* Copy remaining bits of msg data from msg buffer to frame buf */
              b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                      txtc.supp_frame.data, 0, left );

              // Zeros buffer has 36 bytes od zero now at txccommon
              // Need to increase zeros buffer at txccommon
              // for reach frame at 20ms, 38400bps, 744 bits

              /* Pad remaining bits of frame buffer with zeroes */
              b_copy( zeros, 0, txtc.supp_frame.data, left,
                               (txc_reach_frame_size - left) );
            }

            /* Advance message buffer index */
            txc_tx.buf.pos += txc_reach_frame_size;

          } /* end if (txc_reach_si_include == TRUE) */

          //txc_reach_on
          M1X_MSG( MUX, LEGACY_HIGH,
            "ACC_ATT: message --------------------" );

        } /* end if (txc_tx.buf.pos < txc_tx.buf.len) */


        /* Done with message? */
        if (txc_tx.buf.pos >= txc_tx.buf.len)
        {
          txac.state = ACC_ENDPROBE_S;

          ++txac.probe_count;
          switch (++txac.attempt_probe_count)
          {
            case 2:
              (void) parm_inc( CAI_EACH_BA_3_ID, 1 );
              break;
            case 3:
              (void) parm_inc( CAI_EACH_BA_4_ID, 1 );
              break;
            case 4:
              (void) parm_inc( CAI_EACH_BA_5_ID, 1 );
              break;
            case 5:
              (void) parm_inc( CAI_EACH_BA_6_ID, 1 );
              break;
            case 6:
              (void) parm_inc( CAI_EACH_BA_7_ID, 1 );
              break;
            default:
              break;
          }


          /* ------------------------------
          ** Log the Enhanced Access Channel message
          ** ------------------------------ */
          txc_log_reach_msg( &txc_tx.buf );


          if (txc_reach_frame_info_log.hdr.len == 0)
          {
            txc_reach_frame_info_log.hdr.len = sizeof(mux_reach_frame_info_log_type);
          }

          if (txc_reach_frame_info_log.hdr.len != 0)
          {
            /* initialize the REACH channel info */
            txc_reach_psist_delay = 0;
          }

          if ( P_REV_IS_4_OR_GREATER)
          {
            (void) rex_set_sigs( MC_TCB_PTR, MCC_TXC_PROBE_SIG );
            M1X_MSG( MUX, LEGACY_MED,
              "Acc Probe signal sent");
          }

          /* ----------------------------------------------------------
          ** After transmitting each access probe, the mobile station
          ** waits a specified period, TA = (1 + ACC_TMO) * 20 ms, from
          ** the end of the slot.  NOTE: Add number of frames remaining
          ** until the end of this REACH slot to TA.
          ** See LAC 2.1.1.2.2.1
          ** ---------------------------------------------------------- */
          txac.countdown = 1 + txac.acc_tmo;

        } /*  end if (txc_tx.buf.pos >= txc_tx.buf.len) */

      } /*  end if (txac.state == ACC_MSG_S) */

    } /* Handle interrupt signal */

  } while( state == TXC_ACC_ATT_S ); /* until done with this state */

  (void)enc_is95c_enable( FALSE );
  enc_pch_enable( FALSE );
  enc_sch_enable( FALSE );

  /* Now tell the mdsp to set the channel gains to zero */
  muxmdsp_set_channel_gains_zero();

  txc_reach_frame_offset = 0;
  enc_set_frame_off ( 0x0, (byte)txc_reach_frame_offset );

  txc_clr_msgs();                  /* Clear any pending messages to send */
  txc_data_rate = CAI_BLANK_RATE;  /* Initialize data rate indicator */
  txac.frame.dotx = FALSE;         /* Prevent last frame from transmitting */

  /* If the PN randomization for ACCESS state was 0, transition to
  ** TXC_HOLD_ENC_CLK_S state to save from having to Jump to Hyperspace
  ** between ACCESS and TRAFFIC states.
  ** Otherwise, turn off ENC clocks and transition to TXC_ACC_IDLE_S since
  ** the PN randomization must be programmed to 0 for TRAFFIC state.
  ** The re-programming of the PN will take place in the Jump to Hyperspace
  ** between ACCESS and TRAFFIC states.
  ** REACH processing does not include PN randomization so this check is not necessary
  ** but is there for error checking purpose.
  */
  if( (txac.rn == 0) && ( txac.hold_enc_clk_on ) )
  {
    state = TXC_HOLD_ENC_CLK_S;

    /* Arm the timer to expire after allotted time if no command is received
    ** from MC to transition to Traffic or Access state.  Once timer
    ** expires, turn off ENC clocks.
    */
    txc_delay_enc_shutdown.counter = TXC_HOLD_ENC_CLK_WAIT_DURATION;

    /* Jump source is set to TXC_JUMP for 2 cases:
    **   In TXC_HOLD_ENC_CLK_S state, the jump is complete.
    **   In TXC_ACC_JUMP_S state, the jump is in progress.
    */
    txc_tx.jump_source = TXC_JUMP;

    /* Set up Hold ENC clocks ISR.
    */
    txc_hold_enc_clk_init();
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "PN randomization value=%d for REACH",
      txac.rn );

    /* Set interrupt handler to null and turn off TX clocks */
    txc_tx_shutdown();
  }
  M1X_MSG( MUX, LEGACY_HIGH,
    "TX Reach Att done, new state=%d",
    state );

  if( txac.tcb_ptr != NULL )
  {
    /* Notify requesting task that access attempt is completed */
    (void) rex_set_sigs( txac.tcb_ptr, txac.sigs );
    txac.tcb_ptr = NULL;
  }

  return state;

} /* txc_reach_access_att */
#endif /* FEATURE_IS2000_REL_A_CC */

/*===========================================================================

FUNCTION TXC_ACCESS_ATT

DESCRIPTION
  State where access channel transmission is transmitted.

DEPENDENCIES
  None

RETURN VALUE
  New state

SIDE EFFECTS
  None

===========================================================================*/

txc_state_type txc_access_att
(
  txc_state_type state
    /* txc_state - TXC_ACCESS_S */
)
{
  txc_cmd_type  *cmd_ptr;         /* Command from queue */
  rex_sigs_type sigs;             /* Task signals */
  word          dummy;            /* Temp var */
  static word   preamble_cnt = 0; /* Preamble count */
  static word   left;             /* Count bits left */
  qword         slots;            /* Number of slots from time 0, unused */
  byte          pilot_ecio;
  int           pre_is95a_status, post_is95a_status;
    /* Status of encoder switch to/from 95A mode around access */
#ifdef FEATURE_MODEM_1X_SRCH_ASD
  static uint8 seq_num = 0;
#endif /* FEATURE_MODEM_1X_SRCH_ASD */

#ifdef FEATURE_IS2000_REL_A_CC
// txc_reach_on
  if ( txc_reach_access == TRUE )
  {
    state = txc_reach_access_att(state);

    return (state);
  }
#endif // FEATURE_IS2000_REL_A_CC

  txac.state = ACC_SETUP1_S;      /* Set up for first probe */
  txac.probe_count = 0;           /* Initialize to first probe sequence */
  txac.attempt_probe_count = 0;   /* Access attempt probe number */

  /* -----------------------------------------------------------------
  ** NOTE: The interrupt handler takes care of feeding the actual data
  **       to the encoder.
  ** ----------------------------------------------------------------- */

  (void) rex_clr_sigs( TX_TCB_PTR, TXC_INT_SIG );

  /* Set up the encoder for IS95A mode, and save the status */
  pre_is95a_status = enc_is95c_enable( FALSE );

  do
  {
    /* Wait for command on the queue or a transmit frame interrupt */
    sigs = txc_wait( TXC_CMD_Q_SIG | TXC_INT_SIG | TXC_MUXMSGR_Q_SIG );

    if ((sigs & TXC_CMD_Q_SIG) != 0)
    {
      (void) rex_clr_sigs( TX_TCB_PTR, TXC_CMD_Q_SIG );

      if ((cmd_ptr = (txc_cmd_type*) q_get( &txc_cmd_q )) != NULL)
      {
        /* Process the received command */
        state = txc_access_cmd( cmd_ptr, TXC_ACC_ATT_S );
      }
    } /* end if ((sigs & TXC_CMD_Q_SIG) != 0) */

    /* Handle signal from interrupt handler */
    else if ((sigs & TXC_INT_SIG) != 0)
    {
      (void) rex_clr_sigs( TX_TCB_PTR, TXC_INT_SIG );

      /* Assume no transmit */
      txac.frame.dotx = FALSE;

      if (txac.state == ACC_ENDPROBE_S)
      {
        /* -------------------------------------------------
        ** Waiting for the Ack Response Timer (TA) to expire
        ** ------------------------------------------------- */
        if (txac.countdown == 0)
        {
          txac.state = ACC_MSG_DONE_S;

          if (txc_suspend_after_ta)
          {
            txc_suspend_after_ta = FALSE;
            txac.state = ACC_SUSPEND_S;
            /* Signal MC to initiate Access Probe Handoff */
            (void) rex_set_sigs(MC_TCB_PTR,MCC_TXC_APHO_OK_SIG);
          }
        }
        else
        {
          txac.countdown--;
        }
      } /* end if (txac.state == ACC_ENDPROBE_S) */

      if (txac.state == ACC_MSG_DONE_S)
      {
        /* Check if the Probes are suspended due to T72M timer */
        if (!txac.stop_probes)
        {
          if (txac.nstep--)
          {
            /* ------------------------------------------------------
            ** More probes remaining in current access probe sequence
            ** ------------------------------------------------------ */

            txac.power = txac.is95a_pwr;

            /* Check if next power step will be greater than MAX */
            if ((txac.power >= 0x80) && ((txac.power - txac.pstep) < 0x80))
            {
              /* Limit power */
              txac.power = TXC_PWR_MAX;
            }
            else
            {
              /* Increase power */
              txac.power -= txac.pstep;
            }

            txac.is95a_pwr = txac.power;

            /* -------------------------------------------------------------
            ** If more than one Access Channel supported by the Base Station
            ** ------------------------------------------------------------- */
            if (txac.acc_chan > 0)  // BNFIX
            {
              /* ----------------------------------------------------------
              ** Refer to IS-95-B, the mobile can choose the Access Channel
              ** before each probe.
              ** ---------------------------------------------------------- */
              txc_compute_access_chan();

            }

            /* Redo the preamble */
            txac.state = ACC_SETUP2_S;
          }

          else
          {
            /* ------------------------------------
            ** Have completed access probe sequence
            ** ------------------------------------ */

            if (++txac.seq_num >= txac.max_seq)
            {
              /* ---------------------------------------------------
              ** Maximum access probe sequence have been transmitted
              ** in this access attempt
              ** --------------------------------------------------- */
              state = TXC_ACC_IDLE_S;
              M1X_MSG( MUX, LEGACY_HIGH,
                "ACC_ATT: probes done; going back to idle" );

              /* Set flag to indicate no msg transmission in progress */
              txc_tx.active_msg = FALSE;
            }

            else
            {
              /* ---------------------------------------------------------
              ** This is the last access probe in this access probe
              ** sequence.  Compute the next random access channel, RA,
              ** and load the new Access Channel long code mask into the
              ** encoder now so that we meet the minimum time required
              ** for the new PN mask to take effect before the next access
              ** probe sequence.
              ** --------------------------------------------------------- */
              txc_compute_access_chan();

              /* -----------------------------------
              ** Start another access probe sequence
              ** ----------------------------------- */
              txac.state = ACC_SETUP1_S;
              txac.first_frame = FALSE;     /* Not first frame */
              txac.probe_count = 0;
              txac.power = txac.init_pwr;
              txac.nstep = txac.num_step;

              /* initialize the is95a power accumulation */
              txac.is95a_pwr = txac.init_pwr;
            }
          }
        }
        else
        {
          /* Temporarily disable the transmitter and stop sending
          ** any probes until further enabled */
          txac.state = ACC_SUSPEND_S;
        }
      } /* end if (txac.state == ACC_MSG_DONE_S) */

      if (txac.state == ACC_SUSPEND_S)
      {
        /*
        ** There is no need to perform any processing in this
        ** state at this time. Below is the comment from the IS-95B
        ** development branch
        */
        /* -------------------------------------------------------
        ** The TXC is in Suspended state, Transmitter is disabled
        ** and the Probe transmission is suspended. The TXC will
        ** resume transmission of the probes once it is re-enabled
        ** ----------------------------------------------------- */
      }

      if (txac.state == ACC_SETUP1_S || txac.state == ACC_SETUP2_S)
      {
        /* ------------------------------
        ** Log the Access Channel message
        ** ------------------------------ */
        txc_log_ac_msg( &txc_tx.buf );

        if (acc_log.hdr.len == 0)
        {
          acc_log.hdr.len = sizeof(mux_log_acc_info_type);
        }

        if (acc_log.hdr.len != 0)
        {
          acc_log.seq_num = txac.seq_num + 1;
          acc_log.rt      = 0;
          acc_log.rs      = 0;
          acc_log.psist   = 0;
          acc_log.ra      = txac.ra;
          acc_log.rn      = (byte) txac.rn;
        }

        /* Initialize preamble counter */
        preamble_cnt = 0;

        /* Set slots = current frame number */
        qw_equ( slots, txc_tx.frame_cnt );
        /* Test next frame */
        qw_inc( slots, TXC_ACCESS_SLIP );


        /* Get number of frames into this Access Channel slot */
        txac.countdown = qw_div( slots, slots, txac.slotting );

        if (txac.countdown != 0)
        {
          /* Make it a countdown to the next slot, not a countup */
          txac.countdown = txac.slotting - txac.countdown;
        }

        if (txac.state == ACC_SETUP2_S)
        {
          /* ----------------------------------------------------------------
          ** If more than one Access Channel is supported by the Base Station
          ** ---------------------------------------------------------------- */
          if (txac.acc_chan > 1)
          {
            /* -------------------------------------------------------------
            ** Compute probe backoff delay, RT, from 0 to PROBE_BKOFF slots.
            **-------------------------------------------------------------- */
            dummy = ran_dist( txc_sec_ran_next( 0 ), 0, txac.probe_bkoff+1 );
          }
          else
          {
            /* ----------------------------------------------------------
            ** Compute probe backoff delay, RT, from 0 to 1 + PROBE_BKOFF
            ** slots.
            **----------------------------------------------------------- */
            dummy = ran_dist( txc_sec_ran_next( 0 ), 0, txac.probe_bkoff+2 );
          }

          M1X_MSG( MUX, LEGACY_MED,
            "# Acc Chan = %d, RT = %d",
            txac.acc_chan,
            dummy);
          /* -------------------------------------
          ** Add probe backoff delay to countdown.
          ** ------------------------------------- */
          // dummy is 5 bits, slotting is 4 bits.  Product is 9 bits.
          txac.countdown += dummy * txac.slotting; //lint !e734: Loss of precision (31 to 16 bits)

          acc_log.rt = (byte)dummy;
        }

        #ifdef FEATURE_MODEM_1X_SRCH_ASD
        if( txac.seq_num == 0 )
        {
          srch_rx_div_asd_access_forced_switch_event();
        }

        if( ( txac.seq_num != 0 ) && ( seq_num != txac.seq_num ) )
        {
          (void) rex_clr_sigs ( TX_TCB_PTR, TXC_ASDIV_WAIT_SIG );
          srch_rx_div_asd_access_event();
          txc_wait( TXC_ASDIV_WAIT_SIG );
          (void) rex_clr_sigs ( TX_TCB_PTR, TXC_ASDIV_WAIT_SIG );
        }
        seq_num = txac.seq_num;
        #endif /* FEATURE_MODEM_1X_SRCH_ASD */

        /* ----------------------------------------------------------
        ** Before transmitting each access probe sequence other than
        ** the first access probe sequence, the mobile station shall
        ** generate a random number, RS, from 0 to (BKOFFs + 1).  The
        ** mobile station shall delay the transmission of the access
        ** probe sequence for RS slots.
        ** ---------------------------------------------------------- */
        if ((txac.state == ACC_SETUP1_S) && (txac.seq_num != 0))
        {
          dummy = ran_dist( txc_sec_ran_next( 0 ), 0, txac.bkoff+2 );
          // dummy is 5 bits, slotting is 4 bits.  Product is 9 bits.
          txac.countdown += dummy * txac.slotting; //lint !e734: Loss of precision (31 to 16 bits)

          acc_log.rs = (byte) dummy;
        }

        if (txac.state == ACC_SETUP1_S)
        {
          txac.state = ACC_BACK1_S;
        }
        else
        {
          txac.state = ACC_BACK2_S;
        }
      } /* end if (txac.state == ACC_SETUP1_S || ACC_SETUP2_S) */

      if (txac.state == ACC_BACK1_S || txac.state == ACC_BACK2_S)
      {
        if (txac.countdown == 0)
        {
          /* --------------------------------------------------------------
          ** Backoff timer has expired --> If the access attempt is an
          ** Access Channel request, then before transmitting the first
          ** access probe in the access probe sequence, the mobile station
          ** shall perform a persistence test for each Access Channel slot.
          ** -------------------------------------------------------------- */
          if ((txac.req) && (txac.state == ACC_BACK1_S))
          {
              acc_log.psist++;
          }

          if ((txac.req) && (txac.state == ACC_BACK1_S) &&
              ( txc_sec_ran_next( 0 ) > txac.p))
          {
            /* Persistence test failed - try again next slot */
            txac.countdown += (txac.slotting-1);

            /* Events report: Access Denied */
            event_report(EVENT_ACCESS_DENIED);
          }
          else
          {

            /* -------------------------------------------------------------
            ** the interference correction adjustment for the transmit power
            **
            ** note: srch_get_pilot_ecio return pilot_ecio in 1/2db
            ** ------------------------------------------------------------- */
            pilot_ecio = srch_mux_get_pilot_ecio();
            /*lint -e732 */
            txac.int_corr = MIN( MAX(-14+pilot_ecio,0), 14 );
            /*lint +e732 */

            /* Check if add interference correction power will be greater than MAX */
            if ((txac.is95a_pwr >= 0x80) && ((txac.is95a_pwr - txac.int_corr) < 0x80))
            {
               /* Limit power */
               txac.power = TXC_PWR_MAX;
            }
            else
            {
               /* Increase power */
               txac.power = txac.is95a_pwr - txac.int_corr;
            }

            M1X_MSG( MUX, LEGACY_MED,
              "Seq=%d probe=%d ecio=%d, "
              "Tx adj=%d 95A=%d i-corr=%d",
              txac.seq_num+1,
              txac.probe_count+1,
              pilot_ecio/2,
              (-((signed char)txac.power))/2,
              (-((signed char)txac.is95a_pwr))/2,
              txac.int_corr/2);

            /* Dump out Rx/Tx timing status */
            muxmdsp_print_tx_timing_info( FALSE );

            /* Begin transmitting Access Channel preamble */
            txac.state = ACC_PRE_S;
            preamble_cnt = 0;
          }
        }
        else /* Backoff timer has not expired */
        {
          txac.countdown--;
        }
      } /* end if (txac.state == ACC_BACK1_S || txac.state == ACC_BACK2_S) */

      if (txac.state == ACC_PRE_S)  /* Preamble transmit substate */
      {
        if (preamble_cnt < txac.preambles)
        {
          preamble_cnt++;
          txac.frame.dotx = TRUE; /* Enable transmit */
        }
        else
        {
          /* -------------------------------------------------------
          ** Preamble transmitted -- transmit Access Channel message
          ** capsule immediately following the preamble
          ** ------------------------------------------------------- */
          txac.state       = ACC_MSG_S; /* Message transmit substate */
          txc_tx.buf.pos   = 0;         /* Start at msg buffer position 0 */
          txac.first_frame = TRUE;
        }
      } /* end if (txac.state == ACC_PRE_S) */

      if (txac.state == ACC_MSG_S) /* Message transmit substate */
      {
        txac.frame.dotx = TRUE; /* Enable transmit */

        /* Calculate how much of the message is left to send */
        left = txc_tx.buf.len - txc_tx.buf.pos;

        if (left >= TXC_ACC)   /* Lots of message left */
        {
          /* Copy full frame of msg data from msg buffer to frame buffer */
          b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                  txac.frame.data, 0, TXC_ACC );
        }
        else
        {
          /* Copy remaining bits of msg data from msg buffer to frame buf */
          b_copy( txc_tx.buf.msg, txc_tx.buf.pos,
                  txac.frame.data, 0, left );

          /* Pad remaining bits of frame buffer with zeroes */
          b_copy( zeros, 0, txac.frame.data, left, (TXC_ACC - left) );
        }

        /* Advance message buffer index */
        txc_tx.buf.pos += TXC_ACC;

        /* Done with message? */
        if (txc_tx.buf.pos >= txc_tx.buf.len)
        {
          txac.state = ACC_ENDPROBE_S;

          ++txac.probe_count;
          switch (++txac.attempt_probe_count)
          {
            case 2:
              (void) parm_inc( CAI_ACC_3_ID, 1 );
              break;
            case 3:
              (void) parm_inc( CAI_ACC_4_ID, 1 );
              break;
            case 4:
              (void) parm_inc( CAI_ACC_5_ID, 1 );
              break;
            case 5:
              (void) parm_inc( CAI_ACC_6_ID, 1 );
              break;
            case 6:
              (void) parm_inc( CAI_ACC_7_ID, 1 );
              break;
            default:
              break;
          }

          if ( P_REV_IS_4_OR_GREATER)
          {
            (void) rex_set_sigs( MC_TCB_PTR, MCC_TXC_PROBE_SIG );
            M1X_MSG( MUX, LEGACY_MED,
              "Acc Probe signal sent");
          }

          /* ----------------------------------------------------------
          ** After transmitting each access probe, the mobile station
          ** waits a specified period, TA = (2 + ACC_TMO) * 80 ms, from
          ** the end of the slot.  NOTE: Add number of frames remaining
          ** until the end of this Access Channel slot to TA.
          ** ---------------------------------------------------------- */
          txac.countdown = ((txac.slotting - txac.preambles) -
                            ((txc_tx.buf.len + TXC_ACC) / TXC_ACC)) +
                           ((2 + txac.acc_tmo) * 4);

          /* ---------------------------------------------------------
          ** In the above calculation, if txc_tx.buf.len is an integer
          ** multiple of TXC_ACC, the countdown will be one frame too
          ** small.
          ** --------------------------------------------------------- */
          if ((txc_tx.buf.len % TXC_ACC) == 0)
          {
            txac.countdown++;
          }
        }
      } /* end if (txac.state == ACC_MSG_S) */

      else if (txac.frame.dotx)
      {
        /* Send frame of all zeroes */
        b_copy( zeros, 0, txac.frame.data, 0, TXC_ACC );
      }
    } /* Handle interrupt signal */

    /* Handle mux msgr signal */
    else if ( sigs & TXC_MUXMSGR_Q_SIG )
    {
      /* process and clear the muxmsgr signal */
      muxmsgr_process_rcv_msg(TX_TASK);
    } /* Handle mux msgr signal */

  } while (state == TXC_ACC_ATT_S); /* until done with this state */

  txc_clr_msgs();                  /* Clear any pending messages to send */
  txc_data_rate = CAI_BLANK_RATE;  /* Initialize data rate indicator */
  txac.frame.dotx = FALSE;         /* Prevent last frame from transmitting */

  /* Set up the encoder for (future) IS2000 mode, and save the status */
  post_is95a_status = enc_is95c_enable( TRUE );

  /* If the PN randomization for ACCESS state was 0, transition to
  ** TXC_HOLD_ENC_CLK_S state to save from having to Jump to Hyperspace
  ** between ACCESS and TRAFFIC states.
  ** Also ensure that the switch to/from IS95A was reliable, otherwise we'll
  ** want to set up Tx from scratch again for traffic (no enc hold state).
  ** Otherwise, turn off ENC clocks and transition to TXC_ACC_IDLE_S since
  ** the PN randomization must be programmed to 0 for TRAFFIC state.
  ** The re-programming of the PN will take place in the Jump to Hyperspace
  ** between ACCESS and TRAFFIC states.
  ** REACH processing does not include PN randomization.
  */
  if ( ( txac.rn == 0 ) &&
       ( txac.hold_enc_clk_on ) &&
       ( pre_is95a_status == 0 ) &&
       ( txc_dbg_enable_enc_hold ) &&
       ( post_is95a_status == 0 ) )
  {
    state = TXC_HOLD_ENC_CLK_S;

    /* Arm the timer to expire after allotted time if no command is received
    ** from MC to transition to Traffic or Access state.  Once timer
    ** expires, turn off ENC clocks.
    */
    txc_delay_enc_shutdown.counter = TXC_HOLD_ENC_CLK_WAIT_DURATION;

    /* Jump source is set to TXC_JUMP for 2 cases:
    **   In TXC_HOLD_ENC_CLK_S state, the jump is complete.
    **   In TXC_ACC_JUMP_S state, the jump is in progress.
    */
    txc_tx.jump_source = TXC_JUMP;

    /* Set up Hold ENC clocks ISR.
    */
    txc_hold_enc_clk_init();
  }
  else
  {
    /* Give some indication of why Tx is shutting down after access */
    M1X_MSG( MUX, LEGACY_HIGH,
      "Access complete: PN ran=%u, Enc hold=%u, Pre=%d, Post=%d",
      txac.rn,
      txac.hold_enc_clk_on,
      pre_is95a_status,
      post_is95a_status );

    /* Set interrupt handler to null and turn off TX clocks */
    txc_tx_shutdown();
  }
  M1X_MSG( MUX, LEGACY_HIGH,
    "TX Acc Att stopped, new state=%d",
    state);

  if (txac.tcb_ptr != NULL)
  {
    /* Notify requesting task that access attempt is completed */
    (void) rex_set_sigs( txac.tcb_ptr, txac.sigs );
    txac.tcb_ptr = NULL;
  }

  return (state);

} /* txc_access_att */

/*===========================================================================

FUNCTION TXC_ACCESS_IDL

DESCRIPTION
  Access channel idle state.  Waits for command to transmit an access
  attempt or leave the access state.

DEPENDENCIES
  None

RETURNVALUE
  New state

SIDE EFFECTS
  None

===========================================================================*/
txc_state_type txc_access_idl
(
  txc_state_type state
    /* txc_state - TXC_ACC_IDLE_S */
)
{
  txc_cmd_type *cmd_ptr;
    /* Command from queue */
  rex_sigs_type sigs = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
    (void) rex_clr_sigs( TX_TCB_PTR, TXC_CMD_Q_SIG );

    /* Get items off the queue */
    if ((cmd_ptr = (txc_cmd_type*) q_get( &txc_cmd_q )) != NULL)
    {
      /* Process messages common to all idle substates */
      state = txc_access_cmd( cmd_ptr, TXC_ACC_IDLE_S );
    }
    else if ( sigs & TXC_MUXMSGR_Q_SIG )
    {
      /* process and clear the muxmsgr signal */
      muxmsgr_process_rcv_msg(TX_TASK);
    }
    else
    {
      sigs = txc_wait( TXC_CMD_Q_SIG | TXC_MUXMSGR_Q_SIG );
    }
  } while (state == TXC_ACC_IDLE_S); /* Until done with this state */

  return (state);

} /* txc_access_idl */

/*===========================================================================

FUNCTION TXC_OK_TO_SLEEP

DESCRIPTION
  This function returns true if MC can begin sleep.  This mechanism is in
  place to delay shutdown for a period after an access attempt to prevent
  the need to jump to hyperspace for another attempt that occurs soon
  thereafter.

DEPENDENCIES

RETURN VALUE
  True if it is okay to sleep.  False if sleep is to be delayed.

SIDE EFFECTS
  None.
===========================================================================*/
boolean txc_ok_to_sleep( void )
{
  return txc_delay_enc_shutdown.txc_sleep_allowed;
}

/*===========================================================================

FUNCTION TXC_SET_OK_TO_SLEEP

DESCRIPTION
  This function sets the txc_sleep_allowed parameter of txc_delay_enc_shutdown.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void txc_set_ok_to_sleep( boolean ok_to_sleep )
{
  txc_delay_enc_shutdown.txc_sleep_allowed = ok_to_sleep;
}

/*===========================================================================

FUNCTION txc_hold_enc_clk_init

DESCRIPTION
  Initialization section used in TXC_HOLD_ENC_CLK_S and TXC_ACC_JUMP_S states.
  In both states, ISR needs to be txc_hold_enc_clk_isr.
  In both states, jump source needs to be set to TXC_JUMP.

DEPENDENCIES
  None
RETURN VALUE
  None.
SIDE EFFECTS
  None
===========================================================================*/
void txc_hold_enc_clk_init (void)
{
  /*-----------------------------------------------------------------------*/

  M1X_MSG( MUX, LEGACY_MED,
    "Init Hold ENC Clocks, TX disable");

  /* Reset shut down signal, do not allow MC/SRCH to sleep.
  */
  (void) rex_clr_sigs (TX_TCB_PTR, TXC_TX_SHUTDOWN_SIG);
  txc_delay_enc_shutdown.txc_sleep_allowed = FALSE;

  /* Timer count down is done in interrupt context.  Actual shut down
  ** is done in task context.  Set up interrupt function here.
  */
  txc_setup_pcg_events_isr( TXC_PCG_EVENT_HOLD_ENC_CLK, txc_hold_enc_clk_isr );

} /* txc_hold_enc_clk_init */

/*===========================================================================

FUNCTION txc_hold_enc_clk

DESCRIPTION
  In this state, hold the encoder clocks ON for a specified time while waiting
  for the appropriate command(s) from MC to transition to the next state.
  If timer expires, turn off ENC clocks and transition back to Access Idle state.

DEPENDENCIES
  None

RETURNVALUE
  New state

SIDE EFFECTS
  None

===========================================================================*/
txc_state_type txc_hold_enc_clk
(
  txc_state_type state
    /* txc_state - TXC_HOLD_ENC_CLK_S */
)
{
  txc_cmd_type  *cmd_ptr;         /* Command from queue */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
    (void) rex_clr_sigs( TX_TCB_PTR, TXC_CMD_Q_SIG );

    /* Get items off the queue */
    if ((cmd_ptr = (txc_cmd_type*) q_get( &txc_cmd_q )) != NULL)
    {
      /* Process messages common to all idle substates */
      state = txc_access_cmd (cmd_ptr, TXC_HOLD_ENC_CLK_S);

      /* Perform state transition upon reception of access (and reach) or
      ** traffic command from MC.
      */
      switch (txc_tx.jump_source)
      {
        case TXC_JUMP:
          break;

        case TXC_JUMP_ACCESS:
        case TXC_JUMP_TRAFFIC:
        case TXC_JUMP_TT:
          state = txc_access_or_traffic_cmd (TXC_HOLD_ENC_CLK_S);
          break;

        case TXC_READY_TO_JUMP:
        default:
          M1X_MSG( MUX, LEGACY_ERROR,
            "Unexpected source in jump done state");
          break;
      } /* end switch */

    }
    else
    {
      rex_sigs_type sigs;
      sigs = txc_wait (TXC_CMD_Q_SIG | TXC_TX_SHUTDOWN_SIG | TXC_MUXMSGR_Q_SIG);
      if (sigs & TXC_TX_SHUTDOWN_SIG)
      {
        (void) rex_clr_sigs (TX_TCB_PTR, TXC_TX_SHUTDOWN_SIG);
        state = TXC_ACC_IDLE_S;
      }

      /* Handle mux msgr signal */
      if ( sigs & TXC_MUXMSGR_Q_SIG )
      {
        /* process and clear the muxmsgr signal */
        muxmsgr_process_rcv_msg(TX_TASK);
      } /* Handle mux msgr signal */
    }

  } while( state == TXC_HOLD_ENC_CLK_S ); /* Until done with this state */

  if( state == TXC_ACC_IDLE_S )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "Return to Idle from Hold Enc Clk state" );

    txc_tx_shutdown();
    if( txac.tcb_ptr != NULL )
    {
      /* Notify requesting task that access attempt is completed */
      (void) rex_set_sigs( txac.tcb_ptr, txac.sigs );
      txac.tcb_ptr = NULL;
    }
  }

  return state;

} /* txc_hold_enc_clk */

/*===========================================================================

FUNCTION txc_hold_enc_clk_isr

DESCRIPTION
  Interrupt handler for the TXC_HOLD_ENC_CLK_S state.  Also used when jumping
  to hyperspace in TXC_ACC_JUMP_S.  Transition to traffic state is done here.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_hold_enc_clk_isr
(
   m1x_stmr_event_type pcg
)
{
  /*-----------------------------------------------------------------------*/

  (void) pcg; /* unused */

  /* Check for commands */
  if (txc_int_cmd.iscmd)
  {
    /* Make sure power amp is off */
    enc_tx_enable( FALSE );

    switch (txc_int_cmd.cmd)
    {
      case INT_TC_F:
        M1X_MSG( MUX, LEGACY_MED,
          "Got INT_TC_F");
        /* --------------------------
        ** Go to Traffic Channel mode
        ** -------------------------- */
        enc_set_mode( ENC_TC_MODE );

        /* set initial RC */
        if ( txc_so.rev_dcch.included )
        {
          enc_dcch_rc( (enc_fch_radio_config_type) txc_so.rev_dcch.rc );
        }
        if ( txc_so.rev_fch.included )
        {
          enc_fch_rc( (enc_fch_radio_config_type) txc_so.rev_fch.rc );
        }

        /* Load user PN mask */
        enc_user_pn_mask( txc_int_cmd.mask );

        /* Set the counter so that TXC ISR calculates the frame number for
           first NO_OF_ISRS_AFTER_FO isrs. */
        after_fo_handoff_count_isrs = NO_OF_ISRS_AFTER_FO;

        break;

      default:
        M1X_MSG( MUX, LEGACY_ERROR,
          "Illegal command %d",
          txc_int_cmd.cmd );
    } /* switch */

    txc_int_cmd.iscmd = FALSE;     /* done with command */
    (void) rex_set_sigs( TX_TCB_PTR, TXC_INT_CMD_SIG );

  } /* end if (txc_int_cmd.iscmd) */

  /* Count down Jump done timer.  Signal Transmit task when timer expires.
  */
  //FUTURE: TXC_ENTER_ISR_SECTION();
  if (txc_delay_enc_shutdown.counter > 0)
  {
    txc_delay_enc_shutdown.counter--;
    if (txc_delay_enc_shutdown.counter == 0)
    {
      /* Let the TXC task know that the Jump done timer has expired.
      */
      (void) rex_set_sigs (TX_TCB_PTR, TXC_TX_SHUTDOWN_SIG);
    }
  }
  //FUTURE: TXC_LEAVE_ISR_SECTION();

} /* end txc_hold_enc_clk_isr */

/*===========================================================================

FUNCTION TXC_TT_INIT

DESCRIPTION
  Initialize TXC to go in Timing Transfer State.

DEPENDENCIES

RETURN VALUE
  None.
SIDE EFFECTS
  None
===========================================================================*/
void txc_tt_init(void)
{

  enc_change_tx_clk_resources(ENC_TX_CLOCKS_ENABLE);

  /* Critical section, lock the INT, turn on the ENC clock, disable TX and
  reset the PN delay. In this case, the BTF will be load in the RL timing
  */
  TXC_ENTER_ATOMIC_SECTION();
  enc_tx_enable (FALSE);      /* set puncturing off now that clks on */

  /* this function call will load the enc_btf_chipx8 into the TX time.
  Make sure TX time offsets by a enc_btf_chipx8 in respect to the RX time.
  The new hardware allows us to load in the BTF value at any RX 26.6 ms
  boundary. The load value will be computed as :
  ((phase + 1) % 3)* (2^15)*8 + enc_btf_chipx8 + 2, where phase from 0 to 2,
  but the HW is smart enough, it knows what is the current phase, so it will
  take care the ((phase + 1) % 3)* (2^15)*8 part. The SW only needs
  to load in enc_btf_chipx8 + 2 */
  (void) enc_pn_rand_delay( 0 );
  TXC_LEAVE_ATOMIC_SECTION();

  /* In the normal call, the power level of the last access probe will
  be use for the start power level of the traffic. Since in this case,
  we skip the access probe so we use purely open-loop, no adjust or
  anything*/
  txac.last_pwr = 0;

  txc_access_idl_init( TXC_TT_S );

  /* IS95-A mode */
  (void)enc_is95c_enable( FALSE );

  /* the Pilot Channel enable bit must be cleared */
  enc_pch_enable( FALSE );

  /* the Fundamental Channel enable bit must be cleared */
  enc_fch_enable( FALSE );

  /* the Supplemental Channel enable bit must be cleared */
  enc_sch_enable( FALSE );

  /* Dedicated Control Channel enable bit must be cleared */
  enc_dcch_enable( FALSE );

  /* Now tell the mdsp to set the channel gains to zero */
  muxmdsp_set_channel_gains_zero();

  #ifdef FEATURE_IS2000_1X_ADV
  muxmdsp_set_rc8_frame_config_parms_zero();
  #endif /* FEATURE_IS2000_1X_ADV */

  /* Enable RF Tx */
  txc_change_rf_tx_state(TXC_RF_STATE_ENABLE);

  /* Get the PN randomization delay from Jump command.  This value is programmed
  ** during the Jump to hyperspace.
  */
  txac.rn = 0;                  /* PN randomization delay */
  txac.hold_enc_clk_on = TRUE;  /* Hold encoder clock on */

  /* Set correct state and init duration of wait in TXC_HOLD_ENC_CLK_S state
  ** to default value.
  */
  txc_delay_enc_shutdown.counter = 0;

  /* Jump source is set to TXC_JUMP for 2 cases:
  **   In TXC_HOLD_ENC_CLK_S state, the jump is complete.
  **   In TXC_ACC_JUMP_S state, the jump is in progress.
  */
  txc_tx.jump_source = TXC_JUMP;

  /* Set up Hold ENC clocks ISR.
  */
  txc_hold_enc_clk_init();

}

/*===========================================================================

FUNCTION TXC_TT_STATE

DESCRIPTION
 During the timing transfer from DO to 1x, after the FL timing is established
 This function will call to setup the RL timing which involving the loading
 ULPN state process the TXC_TC_F command.

DEPENDENCIES

RETURN VALUE
  None.
SIDE EFFECTS
  None
===========================================================================*/
txc_state_type txc_tt_state
(
  txc_state_type state
)
{
  txc_cmd_type *cmd_ptr;
    /* pointer to received command */

  rex_sigs_type sigs = 0;

  /* kick the dog */
  txc_dog_report();

  while( state == TXC_TT_S )
  {
    /* just wait for CP to command to go to traffic */
    if (sigs & TXC_CMD_Q_SIG)
    {
      ( void )rex_clr_sigs( TX_TCB_PTR, TXC_CMD_Q_SIG );

      if( (cmd_ptr = (txc_cmd_type*) q_get( &txc_cmd_q )) != NULL )
      {
        if(cmd_ptr->hdr.command == TXC_TC_F)
        {
          M1X_MSG( MUX, LEGACY_HIGH,
            "Got TXC_TC_F cmd in TXC_TT_S state ");
          txc_tt_turn_on_transmitter = cmd_ptr->tc.turn_on_transmitter;
          state = txc_access_cmd(cmd_ptr,TXC_TT_S);
          M1X_MSG( MUX, LEGACY_HIGH,
            "Done Processing TXC_TC_F, go to TX_TT_TC_INIT State");
        }
        else if (cmd_ptr->hdr.command == TXC_EXIT_F)
        {
          M1X_MSG( MUX, LEGACY_HIGH,
            "Got TXC_EXIT_F cmd in TXC_TT_S State!");
          txc_exit();  /* perform exit processing */
          state = TXC_EXIT_S;
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
        }
        else
        {
          txc_cmd_done( cmd_ptr, TXC_BADCMD_S );
          M1X_MSG( MUX, LEGACY_ERROR,
            "TX_TT_WAIT_FOR_TC State, Expect TXC_TC_F!, receive cmd %d",
            cmd_ptr->hdr.command);
        }
      }
    }
    else if ( sigs & TXC_MUXMSGR_Q_SIG )
    {
      /* process and clear the muxmsgr signal */
      muxmsgr_process_rcv_msg(TX_TASK);
    } /* Handle mux msgr signal */
    else
    {
      sigs = txc_wait( TXC_CMD_Q_SIG | TXC_MUXMSGR_Q_SIG );
    }
  } /* while */

  return (state);

} /* txc_tt_state */
