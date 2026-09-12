/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         C D M A   T R A N S M I T   T R A F F I C   M O D U L E

GENERAL DESCRIPTION
  Items associated with the traffic channel

EXTERNALIZED FUNCTIONS
  txc_rlgc_trans_cmd - The RLGC transaction callback
  txc_ho_trans_cmd - The HO transaction callback
  txc_traffic_init - Initialization section for traffic channel
  txc_rev_sch_trans_cmd - The reverse SCH transaction callback
  txc_traffic_isr - Interrupt handler for the traffic channel
  txc_traffic - Process traffic channel state

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

$Header: //components/rel/1x.mpss/8.0/mux/src/txctraffic.c#2 $ $DateTime: 2020/01/27 00:26:21 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
04/17/18   eye     Clear is_so_change flag at the end of call.
04/17/18   eye     Dont reset voc params during WTR_HOP gap.
11/20/17   ab      1x2GTA : 0x18CD log packet changes.
07/22/16   eye     Dont DTX frames while CFS is in progress.
06/15/16   eye     Created get-set API for voc_config and reset voc_config
                   after receiving STANDBY_F.
05/18/16   srk     Remove unused SCR complete flag.
05/16/16   eye     Mainline FEATURE_MODEM_1X_IRAT_LTO1X for L compileout build.
04/21/16   srk     1x2GTA cleanup and TX power shoot fixes.
04/14/16   srk     Optimized TX Trigger fixes and code cleanup.
04/07/16   eye     Fixed QTA Tx power shoot up issue after tune back.
03/22/16   eye     DS logging changes for 1x2GTA.
02/25/16   srk     Make mctrans packet static.
02/25/16   srk     TX stack optimization.
02/05/16   eye     1x2GTA Feature changes
11/03/15   agh     Fixed call drop and TX disable issues for inter band CFS
09/09/15   eye     Deregister Tx TC ISR events during frame offset change.
10/02/15   srk     DRX SRCH response deadlock design change and cleanup.
09/23/15   agh     Remove all references to MVS and cleanup WON feature
08/05/15   eye     Added DRX changes for RC switch and frame off based HHO.
07/31/15   agh     WTR Hopping changes for inter band CFS and RIF HHO
07/10/15   eye     WTR Hop based HHO checks for Tx Adj retention.
07/06/15   eye     Exposed RLP reset API of MUX to DS.
05/15/15   sst     Support for Modem Power Statistics
04/19/15   eye     Handle overflowing of LMEM in CH3 for Non-UTX targets.
04/02/15   eye     Release VOC/VFR,when switch from voice to non-voice SO.
03/31/15   srk     Use subscription id in EFS operations.
02/06/15   eye     Clear RLP-MUX variables during R-SCH/Call termination.
01/20/15   bb      Changes to update subscription state to INACTIVE_STATE during
                   TRAFFIC to EXIT state
11/12/14   eye     Added API to get and set MVS variable & reset in call end.
08/21/14   eye     Added MVS_STANDBY during HHO with frame_offset change.
10/08/14   srk     Disable DTX for Half Rate Frames.
10/08/14   srk     Fix for RSCH in FTM mode.
10/08/14   ssh     Avoid RliRC update during SCR transaction/HHO.
07/29/14   ssh     Set CH1 params before enabling the DCCH.
07/11/14   cjb     NV refresh clean-up
06/26/14   pap     Changes to write default values in case of NV read fail.
06/12/14   vks     Change traffic isr to PCG 10 from PCG 12
                   Use common api to setup the events
05/29/14   pap     Redistributing messages.
05/01/14   srk     Enable DTX by default for all rates and all RCs.
01/08/14   ab      DDR power management Support / Feature Cleanup
02/04/14   pap     Aggregating the variables into global structures.
02/04/14   cjb     Temporary change to disable DTX for RC3 by default
02/03/14   dkb     Feature guard Triton UTX code under FEATURE_MODEM_1X_UTX
11/21/13   srk     Enable DTX for RC3 fractional rate frames by default.
11/14/13   dkb     Process muxmsgr messages in all Tx states.
11/13/13   dkb     Send FW signalling message before registering tx isr.
11/11/13   eye     Disable logging 0x14ED before updating frame offset to FW.
11/11/13   srk     Fix the check to enable/disable DTX.
11/09/13   bb      Changes to set correct data lengths for TDSO calls
11/06/13   srk     Disable DTX when DCCH is ON.
11/01/13   dkb     Clean up enc test waveform and reduce the frequency of
                   sending frame config to FW.
10/16/13   dkb     Add changes for R-FCH functionality without FL dependency.
10/16/13   pap     Fixing alignment for check ins.
10/11/13   eye     Removed COEX internal header file.
10/10/13   pap     Fixing alignment.
10/10/13   pap     Removing warnings for LLVM.
10/08/13   eye     Avoid COEX check for DTX decision in non DSDA mode
10/09/13   dkb     Add additional RL changes.
10/03/13   srk     Add support for Device only DTX for Full Rate frames
09/26/13   eye     T2P boost defaulted to 5dB and NV item removal.
09/30/13   dkb     Initial RL changes for new FW interface.
09/10/13   dkb     Register Tx interrupts using m1x vstmr API
09/06/13   srk     Dynamic DTX control changes and unification of T2P boost
                   for RC3 and RC8.
08/28/13   cjb     Removed STX manager support
08/21/13   srk     Update proper gain tables for RC3 DTX.
08/07/13   srk     Fixed the wrong checks in Device only DTX.
07/30/13   srk     Check with coex manager if it is safe to enable DTX.
07/24/13   srk     Disable DTX for half rate frames only for RC3.
07/23/13   srk     Remove unnecessary global variables.
06/26/13   eye     Removed support for Device only DTX for RC4.
06/26/13   eye     Corrected T2P gain logic for RC3/4 DTX.
06/25/13   eye     Moved DSDA feature check for Device Only DTX.
06/19/13   eye     Clean up Device Only DTX changes.
05/31/13   srk     Mainlined FEATURE_MODEM_1X_NON_SIG.
05/14/13   eye     Unifying Device only DTX for RC3/4 and RC8.
05/21/13   trc     Correct HHO Tx gain adjust freeze & restore sequence
05/07/13   eye     Backing out SAR updates done for COEX.
04/25/13   eye     Updating SAAR changes which is to be replaced by COEX.
04/23/13   trc     Handle enc_is95c_enable() return code
04/23/13   vks     Quick repeat the OTA twice for the eCSFB cases
04/22/13   srk     Prevent turning ON DTX before service negotiation is done.
03/11/13   trc     API cleanup for Triton/Dime, specific to R-SCH handling
03/07/13   vks     Quick repeat the OTA once for the eCSFB cases
03/05/13   srk     Remove unused signal
02/18/13   srk     Initialize the dtx frame flag properly.
02/15/13   srk     Replace mcs hwio with m1x hwio
02/13/13   trc     Correct errant ch3 encoder triggering brought in utx cleanup
01/15/13   srk     Added checks to prevent overflow while copying RLP payload
12/19/12   bph     Replace memcpy w/ struct assignment or memscpy
12/19/12   vks     Add support for stx manager
12/17/12   srk     Enable Device only DTX for RC8 by default.
12/11/12   trc     Layering/Cleanup for Triton
11/06/12   srk     Default the nominal power to 0 dB if it is not included in
                   HO message.
11/02/12   vks     Initial support for antenna selection diversity feature.
10/30/12   srk     Combine multiple debug messages into a single debug F3.
10/19/12   srk     Use proper feature guards for DTX utility function.
10/19/12   vks     Use 1x internal feature (FEATURE_MODEM_1X_VOC_4GV_WB_ON_NB)
                   for HDOn-EVRC feature (FEATURE_VOC_4GV_WB_ON_NB)
10/19/12   srk     Set RPC mode to 200 Hz if the current frame is DTX'd
10/18/12   srk     Use 1XA debug F3s instead of using diag commands.
10/18/12   srk     Use bit 15 of NV 70193 to control device only DTX behavior
                   by default for test SOs.
10/18/12   srk     Give alternate defaults for device only DTX depending on the
                   targets.
10/16/12   vks     Initial changes to support HDOn-EVRC feature
10/03/12   srk     Remove the TX power thresholds for dynamic turning ON/OFF
                   DTX and keep it ON for the entire duration of the call.
09/11/12   srk     Remove DTX frame flag and DTX t2p gain ratio boost.
08/27/12   srk     Apply T2P gain ratio boost of tx power boost.
08/23/12   srk     Stop DTX when SCH is active.
08/21/12   srk     Save TX ON/OFF state before doing CFS.
08/02/12   srk     Added Device only DTX algorithm for 1x Advanced calls.
07/12/12   jtm     Deregister and re-register the TXC_MOD_INT PCG IRQs during HHO
                   where the frame offset changes.
05/25/12   jtm     Include the current R-SCH frame when determining if the DTX
                   duration has expired and immediately begin the teardown process.
04/26/12   srk     Added proper power control step size for 1x Adv call on
                   traffic entry.
04/04/12   trc     Convert to m1x_time module
04/04/12   srk     Remove Redundant calls to rfm_1x_notify_tx_config.
03/28/12   srk     Mainlined FEATURE_DS
03/11/12   trc     Print Rx/Tx timing info every 256 Tx frames
02/24/12   srk     Mainlined FEATURE_IS2000_P2 and FEATURE_IS2000_SCH
02/23/12   jtm     Protect against invalid FCH rates.
02/15/12   srk     Definition change of PWR_CNTL_STEP of RCPM in C.S0005 V3.0
02/16/12   srk     Replaced FEATURE_LTE_TO_1X by FEATURE_MODEM_1X_IRAT_LTO1X.
02/15/12   srk     Feature Cleanup: Removed FEATURE_GPS_MODE
02/09/12   srk     Cleared HW register to prevent "Stomped on encoder data"
                   from being printed excessively during traffic.
02/08/12   srk     Mainlined FEATURE_P1_DYNAMIC_PA_SWITCH_POINT_ADJUST
01/31/12   srk     Feature Clean Up.
01/23/12   srk     Feature Cleanup
01/17/12   jtm     Revert REL_A feature clean up.
01/09/12   trc     Remove NikeL v1 HW support
01/06/12   jtm     Fix race condition between TXC task and encoder ISR when
                   release order is processed.
01/04/12   srk     Feature Cleanup.
01/03/12   jtm     Use local function ptr to save the transaction time notifier
                   function and prevent concurrency issues.
12/20/11   srk     Corrected the comments
12/01/11   jtm     Use txc_isr_cmd() instead of DO_INT_CMD macro
11/30/11   srk     Feature clean up: Mainlined and Internalized Features
11/29/11   trc     Ensure Tx is stopped before HHO transaction begins
11/04/11   trc     Add timing message when encoder is stomped and remove
                   dead code zeroing RL frame offset
09/28/11   srk     Mainlined Always On features and removed Always Off Features
09/19/11   srk     Removed Incorrect featurization in txc_set_pri_chan_config
                   txc_get_pri_so, txc_set_sec_chan_config and txc_get_sec_so
09/13/11   trc     Support PLCM workaround for NikeL
08/31/11   adw     Unify TT and native TX jump to hyperspace.
08/18/11   adw     Mainline 1x message router support.
08/17/11   trc     Mainline RF SVDO API
08/04/11   jtm     Default TT quick repeat and frame delay values to 0 (native 1x).
                   Add initial +8dB tx power boost (configurable) in TT cases.
                   Account for interfernce correction during initial Tx power calculations.
07/26/11   vks     Fix compiler warnings.
07/07/11   trc     Remove special case timing handling of loopback SO's
06/28/11   jtm     Changes to get 1xA working on nikel.
06/21/11   jtm     Klocwork fixes.
06/08/11   jtm     Added Quick Repeat capability.
06/03/11   adw     SV support.
05/25/11   trc     NikeL Tx support
05/23/11   jtm     Provided a mechanism to allow a configurable delay from TX on
                   until we send a signaling message.
05/20/11   jtm     Protect changes to SO and physical channel.
05/11/11   jtm     Do not use 200Hz RPC switching during non-blanked frames
04/28/11   jtm     Replace muxmdsp_wr_TxGainAdjStepSize with muxmdsp_set_tx_gain_adj_step_size()
04/25/11   jtm     Align RF for Tx in 1x eCSFB TT scenarios.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
04/05/11   jtm     Set RPC step size to 1dB during TCI.
03/01/11   jtm     Replace FEATURE_MODEM_1X_SUPPORTS_RLP_SU_API with
                   FEATURE_RLP_API_VERSION_1. Defined in dsrlp_api.h.
02/18/11   jtm     Removed unneeded DS header includes.
02/03/11   jtm     Wrap use of DS_LOG_RLP_INVALID in with RLP SU API support feature.
02/01/11   jtm     Replaced direct use of RLP variables with accessor functions.
01/28/11   jtm     Additional FTM decoupling.
01/28/11   jtm     Added Mux Vocoder interface module.
01/28/11   vks     Featurize RF interface with FEATURE_MODEM_1X_SUPPORTS_RF.
01/24/11   vks     Update to new RF api for RF SVDO feature development.
01/21/11   vks     Use 1X specific RF SVDO feature - FEATURE_1X_RF_SVDO_API.
01/18/11   jtm     Lint clean up.
01/13/11   jtm     Handle preamble of zero.
01/10/11   jtm     Use correct feature in txc_rlgc_trans_cmd() so RF is called
                   with the updated RPC step size value.
12/17/10   jtm     Mainlined FEATURE_MVS_MIGRATE under FEATURE_MODEM_1X_VOICE_SUPPORT.
12/15/10   jtm     Feature cleanup.
12/14/10   jtm     Removed MDR/SCCH support.
12/13/10   jtm     Feature guard FEATURE_MVS_MIGRATE with
                   FEATURE_MODEM_1X_SUPPORTS_VOICE.
12/09/10   jtm     Remove obsolete TXC_* commands.
12/01/10   bb      Add support for LTE to 1X
11/24/10   ag      Renamed pwr_cntl_step_zero_rate to pwr_cntl_step.
11/17/10   jtm     Added txc_is_loopback_call and included SO75.
11/15/10   jtm     Initial SU API split for CP.
11/05/10   jtm     Added more _i.h includes for Modem 1x SU API support.
10/30/10   jtm     Modem 1x SU API changes.
10/30/10   jtm     Integrate from rel/1h10.
08/31/10   jtm     Applied txc_frame_offset to the smart blanking dutycycle.
08/31/10   jtm     Added F3 messages to all transaction functions.
08/10/10   vks     Cleanup Genesis Revisit feature.
08/06/10   jtm     Added 1x Advanced support when REL_A is not enabled.
08/05/10   vks     Mainline FEATURE_MODEM_1X_USE_RLP_LOG_TYPES.
07/06/10   jtm     Fixed compiler warnings.
07/01/10   jtm     Only include rf_mdsp_tx_agc.h if FEATURE_1X_SUPPORTS_MSGR
                   is *not* defined.
06/29/10   jtm     Added support for new DS RLP log types.
06/24/10   jtm     Use CAI_IS_SO_VOICE to check for valid voice service options.
06/24/10   vks     During a HO, for the tx_on_off transaction, when turning ON
                   check the rpc control dip switch and take appropriate action
06/15/10   jtm     Consolidated code in txc_config_test_ack_or_nak().
                   In RC8, default to B&B for signaling only SOs.
                   Relocated is_non_critical_frame flag to txc_build_frame().
                   Added RCPM event reporting.
                   Added SO75 support. Relocated RC8 specific data to
                   the rc8_status substructure of txtc.
06/10/10   jtm     Eliminate dependency on log_dmss.h. Moved RPCTL logging
                   related functions here.
05/25/10   bb      Changes to register with TXC_SLOT0_ISR_PCG_NUM for non
                   loop back SO's during TXC SCR transaction
05/25/10   trc     Set SCRM allowed status at beginning of every call
05/19/10   vks     With the new MSGR interface, L1 needs to send FW a message
                   directly for the tx gain adj step size.
05/18/10   vks     Mainline FEATURE_MDSP_SUPPORTS_TX_AGC.
05/07/10   jtm     Zero out all channel gains during Tx stop in HHO cases.
04/20/10   jtm     Replace rfm_tx_*() api calls and common code associated with
                   starting/stopping/enabling/disabling TX with a wrapper
                   function.
04/12/10   jtm     Added RC8 MPP support.
                   Set RPC step size to RCPM value for Tx frame
                   following a DTX'd FCH frame. Corrected smart blanking to
                   not reduce the RPC mode and RPC step for any guaranteed frame.
                   Other miscellaneous 1x Advanced clean up.
04/02/10   jtm     Replaced rlp_api.h with mdrrlp_api.h.
02/04/10   jtm     Removed FEATURE_PLT.
12/16/09   jtm     Corrected txc_tx_process_fundich_data3g() to not attempt
                   to convert RC to rate set for unassigned channels.
11/30/09   vks     Group together mdsp writes for 1xADV channel gain ratios.
11/30/09   jtm     Corrected compiler warnings in 1xA.
11/24/09   jtm     Removed 1xA debug messages. Replaced by 1xA logging.
11/07/09   jtm/vks Grouped together mdsp writes for channel gains.
                   Remove tx_rate_adj variable. Use the one present in
                   channel_gains data structure (in muxmdsp.h) instead.
                   Use api provided by muxmdsp.h instead of
                   TXC_SET_<CHANNEL>_GAIN macros.
10/28/09   jtm     Modified DTXable frame logic to support new SO's.
10/16/09   jtm     FTM support for non-critical 1/8th rate frames for RC8.
10/01/09   jtm     Updated 1xA blanking to disable blanking at the mDSP for
                   the first non-blankable frame after a blanked frame.
                   Lint fixes with 1xA feature enabled.
09/24/09   jtm     Klocwork fixes.
09/23/09   jtm     Changed macro ENC_RC_TO_RATE_SET to become a function.
09/23/09   jtm     Change to allow RL channel gains to be set when using
                   FTM mode and RC8 only.
09/22/09   jtm     Corrected compiler warnings.
09/14/09   adw     Changes to support modem data header CMI refactoring.
08/25/09   jtm     Added 1xA debugging messages.
08/22/09   jtm     Lint Fixes.
08/21/09   jtm     Lint Fixes.
08/11/09   jtm     Replaced deprecated ts.h api's with new time api's and
                   removed FEATURE_1X_DISABLE_CMI.
08/11/09   jtm     Added FTM support for the 1xA RCPM.
08/05/09   jtm     Lint fixes.
08/04/09   jtm     Replaced calls to deprecated BIO funcs with new DAL API.
07/06/09   jtm     Added error checking for 1xA RC8.
06/16/09   jtm     Added 1xA feature around txc_convert_1xa_fch_blank_duty_cycle().
06/11/09   jtm     Additional 1x Advanced RL support. (FCH blanking)
06/02/09   jtm     Added include for tx_v.h due to CP CMI file split.
04/28/09   jtm     Initial 1X Advanced feature support.
04/21/09   jtm     Replaced assert.h with amssassert.h. Assert.h is being
                   phased out.
04/17/09   jtm     Added includes to fix compiler warnings.
04/13/09   adw     Replaced tdso.h with tdso_v.h include for CMI.
04/13/09   adw     Updated rf includes to support rf mode refactoring.
04/06/09   mca     Added RF SVDO API support
04/06/09   jtm     Moved FEATURE_IS2000_R_SCH inside txc_inform_sch_chng() since
                   this function is exposed in the au/api and must be defined.
04/02/09   jtm     Removed T_IO_CARD
04/01/09   jtm     Include clean up.
03/31/09   jtm     Eliminate implicit includes.
03/26/09   jtm     Featurize _v.h includes from subsystems outside of 1x.
03/20/09   jtm     Cleaned up header file inclusions.
03/17/09   jtm     Replaced INTLOCK/INTFREE, INTLOCK_SAV/INTFREE_SAV, and
                   MSM_LOCK/MSM_UNLOCK with macros that can be mapped correctly
                   based on the compiler and controlled internally to TXC.
03/03/09   jtm     Lint fixes.
01/30/09   jtm     Disable TX in action frame -20ms when performing HHO to a
                   different frame offset.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
09/12/08   vlc     Added support for FEATURE_MVS_MIGRATE.
07/21/08   jtm     Fixed Klockwork Errors. Removed FEATURE_MAX_PWR_PROTECTION_DEBUG code.
                   Removed FEATURE_IS2000_REL_D_DV and FEATURE_IS2000_REL_C_DV code.
02/08/08   vlc     Removed interim step up in power after HHO for high RCs.
10/15/07   vlc     Removed an ASSERT which caused a lint warning.
08/08/07   vlc/vm  Change the function name from rfm_set_pa_hyst_timer to
                   rfm_set_pa_access_probe_config().Also change the
                   feature name to FEATURE_PA_ACCESS_PROBE_CONFIG
08/03/07   vlc/vm  In function txc_traffic_init(), added call to function
                   rfm_set_pa_hyst_timer() to notify RF that subsequent call
                   to rfm_enable_tx_pwr() is for traffic state so RF can set
                   its timer to the appropriate value (under feature
                   FEATURE_PA_DIFF_BETWEEN_ACC_AND_TRAFFIC.)
05/04/07   vlc     Added debug information for HHO TX power settings.
04/02/07   vlc     Added new PCG define for bin0 ENC ISRs specifically for loop
                   back calls.  Moved default PCG to 1 for bin0 ENC ISRs for
                   all other calls.
02/23/07   vlc     Fixed compiler warnings/errors when SCH, Rel A, and P2
                   features are turned off.
01/22/07   trc     Parameterize the PCG for bin0 and bin3 ENC ISRs
01/09/07   vlc     Merged the following change to main line from SC2X branch.
                   Mainly, adding support for FEATURE_MDSP_SUPPORTS_TX_AGC.
           vm      Added the support for Firmware TX
12/07/06   vlc     Moved code to turn off TX for HHO action to action time
                   frame (was done in the frame previous to action time frame.)
11/22/06   vlc     Fixed RVCT compiler warnings
10/30/06   bn      Added code to support DO/UMTS to 1x handoff
10/26/06   vlc     Changed TXC time line for traffic state.  Moved 1st ENC ISR
                   to bin0, PCG1.  Moved 2nd ENC ISR to bin3, PCG2.  Removed
                   code which sends SCH frame in task context.  Added new build
                   states to handle late frames or overflow frames.
10/10/06   adk/vlc Updated the function "txc_traffic()" (case TXC_FTM_SCH_F):
                   Called "enc_sch_rc()" to correctly process SCH.
                   Called "txc_update_chan_config_to_rf()" to implement
                   "FEATURE_TX_POWER_BACK_OFF."
09/19/06   vlc     Fixed lint errors.
09/05/06   vlc     Implemented TX gain enhancements to increase digital gain
                   resolution.
07/24/06   vlc     Movd prim_num_bits to txcmux.c since it's only used there.
07/14/06   vlc     Merged the following change from MSM7500 orphan archive.
           tmr     Added parameters to ds707_scrm_max_allowable_sch_rate() call
                   to reduce RPC calls for 7500 data-on-apps.
06/09/06   trc     Remove T_MSM6700 featurization
05/03/06   vlc     In function txc_tx_on_off_trans_cmd(), swapped order of
                   programming tx gain adjust register and turning on PA to
                   eliminate spike after HHO.
                   In txc_tx_frame_proc(), move a "{" out of an #ifdef pair
                   so the brackets will now match up in the entire function.
04/17/06   vlc     Fixed sign in ASSERT statements.
03/20/06   vlc     In function txc_tx_frame_proc_send_data3g(), if a NULL pointer
                   is detected in data received from RLP layer, recover by dropping
                   the frame via an ASSERT rather than an ERR_FATAL.
                   Added important Medium level debug messages.
12/05/05   rkc     Check if rev_fr_ptr->fr_item_ptr for NULL in
                   txc_log_rlp3() and txc_log_fdch_rlp3().
09/08/05   ejv     Use new srch_mux interface calls.
08/02/05   rkc     Fixed position of closing "}" to be inside
                   #ifdef FEATURE_IS2000_R_SCH in function txc_tx_frame_proc_build3g().
07/25/05   rkc     Fixed lint errors.
05/09/95   vlc     Added check for NULL ptr from data buffers given by RLP to
                   transmit.
04/29/05   vlc     Added 4GV support under FEATURE_VOC_4GV.
04/27/05   vlc     Main lined code in current targets, cleaned out obsolete targets,
                   cleaned out obsolete features.
03/14/05   vlc     Fixed compiler warnings for MSM6700.
01/26/05   vlc     Changed #include ds.h (obsolete) to ds707.h.
01/17/05   ljl     Updated for Rel D interface changes by CP.
12/09/04   jyw     removed dec_init_td_flsb_dv if DV is already enabled
12/09/04   jyw     Refeaterization to avoid the compiling error for nonDV
12/03/04   bn      Correct name change in mDSP DV APP
12/03/04   ljl     Updated txc_switch_trans_cmd() to store target and serving
                   sector information.
11/08/04   sr      Added mDSP call to delay start of PDCCH by 1 slot in
                   txc_f_dv_params_trans_cmd()
10/20/04   ljl     Featurized the function call of rpc_program_rev_d_gain_ratios().
10/19/04   ljl     Added featurization, used the correct slot for 10ms tick,
                   and disabled reverse power control during PLT.
10/12/04   ljl     Added to call enc_write_data_done() after R-SCH data buffer has
                   been written to encoder ram for TDSO.
10/04/04   ljl     Added featurization for muxdvlog_update_gating_rate().
09/27/04   ljl     Modified to follow Rev D call flow for PLT.
09/14/04   jrp     Update calls to DV_INFO header.
09/10/04   ljl     Processed TXC_10MS_TICK_SIG.
09/08/04   sr      Disabled DV Application in the firmware when ending a call
                   as a workaround for high PER on subsequent DV calls.
09/01/04   jrp     Added parameter for DTX of R-REQCH when writing gain ratios.
09/01/04   sr      Disabled DV channels in mDSP in TXC_EXIT_F and TXC_CDMA_F
                   Added mDSP message handler for PDCH Traffic Msg
09/01/04   jrp     Added back pre-6700 interfaces.
09/01/04   sr      Added proper features around DV header files
08/30/04   jrp     Added missing call to muxmdsp_dv_ctrl_init().  Also, set both
                   serving and target parameters in f_dv_parms action.
08/25/04   jyw     Modified the rxcpdch_init interface to include the bit shift control
08/25/04   jyw     Put 2-bit shift for the DMSS dec ob control
08/25/04   jyw     Put the dec ob control for valid CRC only
08/17/04   jrp     Added programming of gain ratios in PLT mode.
08/16/04   sr      Made a new function to enable DV application. This waits for
                   an acknowledgement from mDSP services that signals DV app is
                   ready before proceeding.
                   Fixed txc_switch_trans_cmd().
08/16/04   ljl     Rel D reverse channels PLT.
08/12/04   sr      Registered action handlers for F_DV_PARAMS and SWITCH
                   actions. Set stale timeout interrupt to 8
08/02/04   jyw     Fixed the long code mask in the txc_f_dv_params_trans;
07/30/04   vlc     Added support for rel C TDSO.
07/29/04   jyw     Updated the action interface of txc_f_dv_params_trans_cmd to include
                   plt_pdch_cfg
07/28/04   jrp     Fixed compilation errors.
07/26/04   jyw     Added rxcpdch_init and rxcpdcch_init in the TXC_F_DV_PARAMS_TRANS_CMD
                   handler;
07/26/04   jyw     In TXC_F_DV_PARAMS_TRANS_CMD action handler, added function call
                   dec_init_td_flsb_dv(); to initialize the turbo decoder and
                   for link symbol buffer.
07/21/04   bn      Call encoder write done for SCH PLT
07/19/04   sr      Renamed MCTRANS_TXC_SWITCH_ACTION to MCTRANS_SWITCH_ACTION
06/29/04   jrp     Gain pass-through, and modulator interrupt changes.
06/24/04   jyw/sr  Fixed the way Devo mdsp application is enabled plus misc
                   cleanup for DV
06/16/04   sr      Merged in changes from 6500 5004 build
06/14/04   sr/va   Added HHO cleanup under FEATURE_HHO_ENHANCEMENTS
06/01/04   ljl     Added DV PLT support.
05/21/04   jyw     Introduce new RF PA backoff interface
05/12/04   vlc     Added MCTRANS_MAX_ACTION under feature FEATURE_TRANS_MANAGER.
05/07/04   vlc     Modified RLP logging extensively to decouple MUX processing
                   and RLP processing, moved RLP processing to RLP module.
                   Added MCTRANS_MAX_ACTION.
05/04/04   jyw     remove the featurization around the txc_f_dv_params_trans_cmd
                   and txc_switch_trans_cmd
05/04/04   jyw     include txc_f_dv_params_trans_cmd and txc_switch_trans_cmd
                   to FEATURE_TRANS_MANAGER
04/26/04   vlc     Implemented new way of handling RXTX confirmation of all
                   signalling messages for voice calls/data calls/svd calls.
04/26/04   sr      Set gain adjust first before turning on PA in TXC_TX_ON_F
04/20/04   jrp     Fixed lint warnings.
03/31/04   sr      Added call to enc_user_pn_mask() in TXC_RETURNED_F
03/16/04   jw      Filled the txc_lcm_trans_cmd as the action handler for the
                   NMCTRANS_LCM_ACTION in the new transaction engine;
03/16/04   jw      Added txc_lcm_trans_cmd() function for the new action
                   MCTRANS_LCM_ACTION;
03/16/04   jw      Featurized the lcm processing part in the function
                   txc_ho_trans_cmd();
03/16/04   sr      Made TEMP_HACKS_FOR_DV depend on T_MSM6700
03/10/04   vlc     Added support for centralized transaction engine handling
                   under feature FEATURE_TRANS_MANAGER.
03/05/04   sr/bkm  Moved clearing of frame offset from MC to TXC_EXIT_F and
                   TXC_CDMA_F
03/05/04   bn      For MSM6700, need to tell the SCH encoder when done
03/03/04   sr      Changes for DV plus included changes to MSMSHARED tip
02/03/04   sab     Moved call to txc_build_dcch_frame if not Release B.
02/03/04   sab     Featurized confirmation_type changes under Release B.
12/02/03   vlc     Removed txc_tx_frame_proc_build_data3g() function inside
                   #if 0.
11/21/03   jrp     Added call to muxmdsp_dv_cell_switch to write mDSP parameters.
11/04/03   sr      Explicitly compared band class returned by
                   mc_get_band_class() against CAI_BAND_CELL/PCS
10/28/03   bn      Checked the for NULL pointer from RLP, before writing to encoder
10/21/03   bn      Fixed compilation warning errors when using RVCT compiler
10/15/03   sb      If in a voice call, and not on DCCH, make sure the confirmation
                   is always delayed by a frame, to enable a rate change.
09/11/03   lcc/vlc In txc_stop_tx, only zero out all digital gains when we are turning
                   off the transmitter for HHO and not in the case of fade.  Otherwise
                   when we come out of fade and turn on the transmitter, the gains will
                   be incorrect.
09/09/03   dna     Merged porting changes for MSM6500
09/08/03   vlc     In function txc_tx_frame_proc_send_data3g(), added check for
                   NULL ptr before calling ENC_SCH_DATA_OUT macro.
09/05/03   vlc     Removed shared txc_so.rate_set from txc_log_fdch_rlp3().
08/25/03   vlc     Removed shared rate_set from txc_so (txc_so_type) to avoid
                   confusion between FCH and DCCH rate sets.  Now calculate
                   rate set from RC as it is needed.
08/06/03   sab     Use confirmation_type in payload of TXC_MSG_F to determine
                   whether TX should signal RXTX when the msg was built, or sent.
07/25/03   junz    Added FDCH RLP3 frame logging
06/27/03   bn      Corrected the RLP logging's time stamp logic when in VP2 and P3
06/24/03   jrp     Changed calls to srch_set_tx_rate_adj() to call instead
                   rpc_set_tx_rate_adj() so that TX_RATE_ADJ is written in the
                   appropriate units - either 1/12 or 1/10dB.
05/28/03   vlc     Disable SCH when getting TXC_CDMA_F or TXC_EXIT_F from MC.
05/21/03   vlc     Allow pri + sec on DCCH to teardown unsupported call scenario
                   gracefully rather than error fatal.
05/14/03   vlc     In function txc_tx_proccess_sch(), set service type to Primary
                   or Secondary for 1x SCH MUX PDU 1 and MUX PDU 2.
04/25/03   bn      Added logics to support RLP loging in VP2 and P3
04/10/03   vlc     For Rel A, moved TXC ISR to PCG 1 in traffic state.
03/28/03   vlc     Added logic to support different transmit schemes for
                   different types of RLP signalling frames in an SVD call.
03/07/03   vlc     Reverted changes for SVD done on Release 0 code which caused
                   signalling msgs to be transmitted on the incorrect fundicated
                   channel in some cases.
03/04/03   bkm     Added support for REV_FCH_GATING event.
01/31/03   bkm     Shortened R-SCH F3 debug message for readability.
01/31/03   bn      Re-did P2 featurizations fixes for JCDMA builds for sr/wn
01/30/03   bn/sy   Used the less-DSM-size-dependency logic to check for chained
                   DSM items. Also generic name for function to get DSM items
01/23/03   sr/wn   P2 featurizations fixes for JCDMA builds
01/10/03   bn      Used the new encode byte accessing function to simplify
                   the logic handling MUXPDU5 and MUXPDU3 frame processing
01/07/03   jrp     Changed sch_fr_sent initialization to FALSE to prevent
                   inaccurate error message: RLP frames not built in time.
12/05/02   sn      Fixed the compilation error when undefined feature P2
12/02/02   sy      Made changes to support E0 at the end of the turbo mux pdu5
                   payload.
11/25/02   bn      Support 32x, make sure the max number of MUXPDU is no more than 8
11/26/02   dbc     Added support for rf_notify_sch_data_rate() for dynamic PA
                   switch point adjustment according RSCH data rate. Code
                   wrapped in FEATURE_P1_DYNAMIC_PA_SWITCH_POINT_ADJUST
10/31/02   bn      Fixed the wrongly increasing RDCCH MUX counters
10/16/02   jrp     Moved pcbit data write from Tx txc_tx_frame_proc() to a new
                   function to be called from Rx context.
10/15/02   dlb     Added new power control logging to combine power control
                   and frame type logging.
10/14/02   bn      Using the correct feature R_SCH in PLT
10/14/02   ak, vlc Modified code to work with MultiMode Data client on MSM6100.
10/08/02   jrp     Added featurization to fix compiler warnings.
09/30/02   js, bn  Fixed the incorrected Reverse MUX PDU5 frame size in RLP logging
09/24/02   bn      Fixed the incorrected length of the FILL MUX PDU5
09/19/02   snn     Made changes to let DCCH handle BLANK rates and IDLE bits
                   correctly during transmission.
09/19/02   bgc     Merged feature changes for MSM6000 for IS2000_SCH and P2.
09/09/02   bn      Added logic to update RSCH-MUXPDU5 Mux-statitic counters
09/09/02   cml     Featurized definition of rlp_to_mux_table
                   Wrapped TXC_TX_POST_RLP_FCH_PROCESS with FEATURE_DS_RPL3
09/04/02   bn,hrk  In Rel.A when FEATURE_IS2000_REL_A_SVD is turned off,
                   prim_dcch_data_ptr and prim_dcch_rate were not initialized.
08/27/02   hrk     Fixed frame type log reporting in Rel.A when both R-DCCH and
                   R-FCH are assgined.
08/20/02   bn      Added new SCRM rate logic
08/20/02   dbc     Remove usage of global rf_tx_pwr_limit; instead use RF unit
                   API function rf_get_cdma_tx_pwr_limit(). When
                   FEATURE_NV_HAS_MIN_RX_RSSI is defined use rf_get_min_tx_pwr_in_dB()
                   to get the minimum TX power
08/14/02   bn      Initialize the enc_fch_gain and enc_dcch_gain correctly when
                   R-FCH or R-DCCH is not included
08/13/02   hrk     Intialized txtc.tx_rate_adj and txtc.offset_pwr in
                   txc_pilot_init() to take care of problem during handoffs
                   between high and low RCs. Fixed featurization errors.
08/08/02   vlc     Fixed compiler error when FEATURE_IS95B_MAHHO is turned off.
08/01/02   bn      Fixed compilation error when feature REL_A is off
08/01/02   jrp     Added featurization for FPC modes 4,5,6.  Changed FPC_MODE
                   enums to be more descriptive due to SVD.  Uses QIB and EIB
                   flags set by RXC without clearing.  RXC now sets and clears
                   these flags.
07/25/02   hrk,bn  Fixed featurization errors.
07/24/02   hrk     Mainlined following changes from branch off rev 1.137.1.11
           bn      Updated REV MUXPDU5 logic to support turbo code
           bn      Added support for REV MUXPDU5
           hrk     Added support for VP2.
07/15/02   bgc     Added T_MSM_5100 featurization for setting of txc_xxx_gain variables.
07/11/02   bn      Put logics to support freezing the setpoint when TX is off
07/09/02   lcc     When turning off transmitter, resets tx_rate_adjust to 0.
06/25/02   HQ      Added FEATURE_CDSMS.
06/18/02   hrk     RL_GAIN_ADJ was not factored correctly.
06/18/02   jrp     Added support for FFPC Modes 4,5,6.
06/11/02   bgc     Merged MSM6000 changes to handle rpc logging.  Added
                   featurization to turn Data off.
06/06/02   sr      Applied earlier fix for back-to-back bursts to the case of
                   data as a secondary service
06/05/02   hrk     Fixed merge error for MSM5100.
                   TX_RATE_ADJ incorrectly programmed in txc_traffic_isr().
06/04/02   lcc     1. Do not set encoder gains when transmitter is off.
                   2. When turning off transmitter, set encoder gains to 0 for
                      MSM5100.
06/03/02   lcc     Corrected featurization for P2 in txc_tx_frame_proc.
05/31/02   lcc     1. Corrected stopping of HHO preambles with DCCH.
                   2. Corrected preamble gain level for MSM5105.
05/22/02   bn      Added new logics to support VP2 RPC
05/15/02   sr      If RLP finishes building frames late, i.e. after the 15ms
                   mark, send the frames right away. This fixes problems with
                   RLP resets during back to back burst scenarios.
05/14/02   snn     Replaced enum types for BLANK rates in case of Secondary service.
05/12/02   ro      Use FEATURE_IS2000_R_SCH around sch code in txc_tx_frame_proc
05/08/02   bn      Added logic to handle out of action buffer case
04/19/02   az      Added support for new voice service option SMV (SO 56)
04/27/02   lcc     Handles the case when SO is set back to CAI_SO_NULL or removed
                   on traffic channel.  Happens when SCR contains no SO.
03/27/02   bn      Fixed Non-Rel A builds complilation error caused by secondary
                   service option and fixed a possible mis-alignment error in RDCH log
03/26/02   snn     For Non-Rel A builds, fixed use of Service type and made it
                   DSRLP_PRIMARY_SRVC always.
03/26/02   hrk     Added SVD support for Markov, SMS SOs.
03/25/02   bn      Fixed compilation errors causing by secondary service option
03/12/02   snn     Added changes for 1x SCH for Secondary service
03/06/02   sy      Made corrections for RLP logging for secondary service.
03/04/02   bn      Set the DCCH mux category of DTX frame type correctly for RS1 or RS2
03/04/02   bgc     Changed features to IS2000_R_SCH where appropriate.
02/25/02   hjr     Fixed featurization of FEATURE_IS2000_SCH with
                   FEATURE_IS2000_DS_RS2 on.
02/28/02   hrk,fs  Replaced PILOT_FCH_9600 with PILOT_FCH
02/27/02   hrk     Removed FEATURE_IS2000_REL_A_SVD around rlp_to_mux_table.
02/27/02   bn,hrk  Added SVD support.
           sy      Added RLP logging for Secondary traffic.
02/14/02   vlc     In function txc_tx_frame_proc(), removed code which
                   decrements txtc.num_preambles and exits gating mode when
                   it becomes zero.
02/13/02   bn      For MSM5100 PLT, removed writing of data to encoder in the task context
                   as it is now done in the ISR
02/11/02   hrk     For MSM5100, TX_RATE_ADJ is programmed in the last 5ms bin.
                   Setting R-FCH, R-DCCH, R-SCH digital gain to 0 during preambles.
02/01/02   snn     Changed r_sch_blank_pdu_hdr to uint32 from byte.
01/14/02   bgc     Now using correct bit shifts for TX_AGC_ADJ and tx open
                   loop.
01/22/02   bn      Added support for a counter of consecutive DTX frames
01/16/02   ak/sr   Cleared dtx_duration_included and txc_dtx_off if reverse
                   dtx duration is set to infinite. This solves the problem
                   where if the reverse dtx duration went from a finite value
                   to infinite the mobile was still following the old value.
01/15/02   hrk     Changed RF_ENABLE_PWR_WAIT to RF_ENABLE_TX_PWR_WAIT when using
                   FEATURE_RF_WAIT_CHANGES.
01/03/02   bgc     Added SCH featurization to turn off for MSM6000.
12/17/01   vlc     Removed load long code mask modification (temporary) due to
                   short test time of released builds.
12/17/01   vlc     Re-did last two revisions of this file to correct a check in
                   error (stripped version was checked in by mistake.)
12/17/01   tc      Load long code mask in txc_ho_trans_cmd if load is included.
12/13/01   vlc     In function txc_tx_frame_proc (), added check for transmitting
                   valid data frames before transmitting spare bits (TDSO support
                   only.)
12/10/01   hrk     Renamed txc_erasure_flag to txc_eib_flag. Puncturing 0x0000 if
                   a good F-FCH/F-DCCH frame is received, and 0xFFFF if a ERASURE/
                   DTX frame is received on F-FCH/F-DCCH.
12/06/01   bn      Changed the order of enable-DCCH-disable-FCH encoder to
                   disable-FCH-enable-DCCH encoder so it works in any HW platform
12/05/01   lcc     Moved encoder ISR for non-IRAM builds from bin 0 to bin 0 +
                   1 PCG to make sure that it happens after the system time frame
                   boundary.
12/02/01   ak      Fixed blanking PDU's for turbo.
11/28/01   fc      Added the support for the update of DTX duration of R-SCH
                   in progress by ESCAM containing only F-SCH assignment.
11/28/01   snn     Updated Logging RS2 data.
11/12/01   bn      Added the logic to include FCH 8th rate gating into RDCH log record
11/09/01   lcc     Removed clearing of txc_so.stored_pri when RXC_RELEASE_F is
                   received since potentially the service option can be reactivated
                   again and the call not released.
11/07/01   bn      Used MSG_HIGH to out put the expiration of SCH DTX counter
10/30/01   bn      Added support for Release A RDCH logging
10/26/01   tc      Input FCH data to modulator at ISR for MSM5100.
                   Signal RLP build frame at BIN 0 for MSM5100 non-IRAM build.
10/22/01   snn     In mixed rate sets, 1x muxPDU for SCH and FCH were made
                   different.
10/15/01   kk      Fixed R-DCCH DTX problem.
10/05/01   lcc     1. Added handling of MCTRANS_NULL_ACTION in txc_service_trans_cmd.
                   2. Added critical section protection in txc_add_action.
10/03/01   bn      Fixed RPC SCH/PICH ratio DTX all the time in STDSO or FTDSO
09/26/01   bn      Fixed the mismatched time between the RSCH rpc and frame type log
09/14/01   rama    SR_ID fix. Differentiate between SR_ID and RLP instance.
                   Changed to DSRLP_FIXED_SR_ID to DSRLP_FIXED_SR_ID_INDEX
09/14/01   bn      Fixed P2 featurization
09/11/01   bgc     Fixed problems with frame type in reverse frame logging.
                   Fixed R_SCH0 RC in reverse frame type logging.
09/05/01   lcc     Removed incorrect featurization around 1/8th rate gating.
08/28/01   lh      Back out SI bits changes for Release A Addendum 2.
08/22/01   bn      Support DCCH in FTM mode
08/14/01   sy      Added support for DCCH on RS2
08/13/01   bn,hrk  Fixed :
                   1. incorrect turn around constant during R-SCH transition in RPC logs.
                   2. multiple channel adjustment gain for FCH was delayed by 1 frame in
                      RPC logs.
08/03/01   bkm     Fixed P2 featurization in txc_tx_frame_proc_build_data3g().
07/23/01   snn     Code cleanup for allowing DCCH calls.
07/12/01   snn     Added support for Mixed Rates
07/12/01   vlc     Changed FEATURE_RC4_SCH to FEATURE_DS_IS2000_RS2 in order
                   to eliminate FEATURE_RC4_SCH.
06/28/01   vlc     Use r_sch_rc in txtc instead of rev_sch_rc in txc_so to
                   to remove duplication (PLT change).
06/21/01   lcc     In txc_scr_trans_cmd, when DCCH is not included, added disabling of
                   DCCH.
06/18/01   lcc     1. Disables frame type logging when sending preambles.
                   2. Added enabling of FCH when FCH is included in txc_scr_action
                      and txc_scr_trans_cmd.
06/15/01   day     Mainlined FEATURE_RLGC, FEATURE_FFPC, FEATURE_DATA_TIMELINE_ADVANCE,
                   FEATURE_IS2000
                   Delete FEATURE_SPECIAL_MDR, FEATURE_FINGER_POLL
06/14/01   hrk     Corrected comprison of txc_rev_fr_block.num_frames from 1 to
                   SCH_BUF_START to determine presence of R-SCH frame in P2 mode.
06/13/01   hrk     Added Max pwr protection on DCCH for TDSO.
06/13/01   sy      Added DSRLP_RATE_INACTIVE enum to support P2 Mode
                   to show which FCH/DCCH is inactive.It behaves exactly
                   like DSRLP_RATE_BLANK.
06/08/01   hrk     Updated Max pwr protection for SCH in a DCCH+SCH configuration.
06/07/01   kk      Added Mixed Rate Support to TDSO interface.
06/05/01   hrk     uncommented code to set TX_RATE_ADJ in PLT/FTM mode.
06/06/01   lcc     1. Changed the way DTX works such that pilot gating (control hold)
                      will work for MSM5100 as well.
                   2. Recovered changes for making S-TDSO work on DCCH.
06/04/01   bgc     Added #ifdefs to turn off data services.
06/04/01   vlc     Changed FEATURE_GPSONE to FEATURE_GPS_MODE.
06/01/01   jq      Wrapped some code in FEATURE_IS95B_MAHHO.
05/31/01   hrk     PCS value was used for computation of TX_GAIN_ADJ_FLOOR for
                   Cellular target.
05/30/01   kk      Fixed hardcoded RS1 number for 1x Reverse link.
05/24/01   fas     Add hooks for rf_pa_backoff.c
05/22/01   kk      Added missing EIB bit shift for RS2.
05/18/01   kk      RS2 TDSO support.
05/15/01   tc      Supported MSM5100 DCCH channel gain for P2 feature.
05/11/01   fc      Merged the following:
           lcc     Added the support of DCCH STDSO statistics logging.
05/11/01   vlc     Merged in datapump changes for PLT (ks.)
05/10/01   lcc     Corrected the problem of control hold gating rate not stored.
05/08/01   sr      Software work around for MSM5100 5ms issue
                   Turned off max power protection for baseband builds
                   Removed txc_rev_fr_block declaration in txc_tx_frame_proc()
05/03/01   tc      Supported reverse link DCCH PLT.
05/02/01   vlc     Changed FCH default tx pattern in txc_traffic_init() (for PLT
                   only.)
04/26/01   vlc     Changed name of a few constants to accomodate new SCH RC4
                   development on MSM5100.  Added SCH RC4 support.
04/24/01   jq      Fixed an error in frame logging
04/20/01   sr      Merged in from MSM5100 archive
04/19/01   tc      Fixed incorrect dsch checking in txc_log_tc_msg()
                   Fixed unnecessary check for dcch_included in txc_tc_isr()
04/19/01   tc      Merged with MSM_MUX1X.01.00.01
           lcc     Corrected re-enabling of FCH after sending PCG preambles to
                   be done only if FCH is included.
           fc      Added support of events report: Transmitter disabled and
                   Transmitter enabled.
                   Renamed mcc_report_event_msg_txed to
                   mclog_report_event_msg_txed.
           hrk     added support to disable Max Pwr protection on R-SCH from QXDM.
04/18/01   sr      Merged with MSM_MUX1X.01.00.00
           lcc,hrk Added support for FEATURE_IS2000_P2
03/27/01   vlc     Merged up to rev 1.27 of MSM archive.
03/29/01   ak      If RLP produces a blank, handle it by sending 1/8th rate
                   NULL (if no signalling).
           lcc     1. Corrected tranmission of preambles in PCG units so that it won't
                      be repeated for "n" number of frames.
                   2. Ensures transmission of HHO preambles is done before sending
                      signalling messages (HCM).
           hrk     Mods to detect baseband conditions so that max pwr protection
                   can be dynamically disabled.
           hrk     Mods to FEATURE_MAX_PWR_PROTECTION.
           lcc     Merged in support for SO 17 (CAI_SO_VOICE_13K_IS733).
           hrk     Changed prototype for txc_rpctrl_log_frame().
           hrk     Mods to FEATURE_MAX_PWR_PROTECTION.
           hrk     Reverse Power Control info is logged immediately after frame is
                   constructed.
           fc      Added support of events report: Message transmitted.
03/27/01   snn     Added void in txc_calc_r_sch_throttle() to fix compiler err.
03/23/01   sr      Added support for 32X (PLT)
03/08/01   lh      Fix for compile errors.
03/08/01   sr      Removed divide-by-2 of ok_num_mux_pdus
03/07/01   va      Added support for populating p_rev_in_use in txtc_type
                   during handoffs and HHO returns.
           lh      Added Release A SAR parameter settings.
03/02/01   tc      Merged MSM_MUX1X.00.00.15.
02/22/01   ak      RLP logging no longer in IRAM.  Separated out 3G data code
                   into separate functions to save IRAM.
02/20/01   hrk     When TX_PWR hits minimum, TX_GAIN_ADJ is not allowed to go
                   further down.
02/16/01   bgc     Changed FEATURE_FACTORY_TESTMODE to enable a runtime switch.
02/16/01   hrk     Flushing RPC log at the end of call.
02/05/01   ak      Correct blanking of rest of SDU when Turbo or Conv.  Now
                   report correct bps for FCH in RLP logging (1/2 rate, 1/4
                   rate, etc).
01/31/01   tc      Fixed FEATURE_IS2000_TDSO.
01/30/01   tc      Merged GPS support.
01/18/01   lcc     Added initialization of txc_hho_preamble_pcg_units to FALSE
                   in txc_traffic_init.
01/11/01   tc      Merged MSM_MUX1X.00.00.06.
01/08/00   ak      Fixes for R-SCH throttling, to account for asynchronous
12/20/00   tc      Fixed PLT traffic data pattern.
12/27/00   lcc     Added turning off of 1/8 rate gating on return from handoff
                   failure if gating is not enabled.
12/13/00   hrk     Enhancements for Max Power Protection.
11/27/00   tc      Fixed PLT data rate for fundamental and supplemental channel.
11/23/00   tc      Fixed MSM5100 PLT traffic data.
11/13/00   tc      Defined txc_pch_gain, txc_fch_gain, txc_sch_gain at txccommon.
11/09/00   tc      Defined feature T_MSM_5100.
11/09/00   tc      Supported MSM5100 modulator driver interface.
12/01/00   ak      New function to do R-SCH throttling calculations, to
                   help account for case where only F-SCH changes.
11/29/000  ak      Updated PARM ID to reflect changed name in CAI.h
11/22/00   sr      Fixed problem with 1x SCH in TDSO
11/15/00   bgc     Fixed merge error with duplicate code.
11/13/00   bgc     (merge) Seperated the FCH and SCH tdso blocks while
                   packing Reverse TDSO frames. Also fixed the fr_time
                   during FCH tdso frames.
11/10/00   ak      Fixed compiler bug resulting from missed change in prev.
11/07/00   ttl/ak  Changes to throttle R-SCH in data calls when assigned SCH
                   is too high for CPU.
11/12/00   sr      (merge) Added support for Full TDSO
11/12/00   bgc     (merge) Fixed warnings and errors in factory test mode
                   for RSCH.
11/12/00   sr      (merge) Fixed problem with SCH spare bits not being set
                   to 0 forSTDSO calls
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
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"

//mux
#include "rlgc.h"
#include "rlgc_i.h"
#include "txccommon_i.h"
#include "txcmux.h"
#include "txctraffic_i.h"
#include "rxctraffic.h"
#include "mux_logtypes.h"
#include "txc_v.h"
#include "txc_i.h"
#include "rxccommon.h"
#include "rxccommon_i.h"
#include "txcmc_i.h"
#include "txcso.h"
#include "muxmdsp_i.h"
#include "txcaccess.h"
#include "mux.h"
#include "muxvoc.h"
#include "muxnv_i.h"
#include "txc.h"

//drivers
#include "enc.h"
#include "enc_v.h"
#include "enc_i.h"
#include "dec5000.h"
#include "m1x_hwio_mpss.h"
#include "m1x_stmr_i.h"
#include "m1x_time_i.h"

//cp
#include "cai.h"
#include "cai_v.h"
#include "parm_i.h"
#include "mclog.h"
#include "mclog_v.h"
#include "tdso_v.h"
#include "mccdma_v.h"
#include "mccdma_i.h"
#include "mc.h"
#include "mc_v.h"
#include "mctrans_i.h"
#include "tx_v.h"
#include "mccsch_i.h"

//diag
#include "cdma2kdiagi.h"
#include "onex_non_sig.h"
#include "onex_nv_rd_wt.h"

//srch
#include "srch_mux.h"
#ifdef FEATURE_MODEM_1X_COEXISTENCE
#include "srch_coex.h"
#endif /* FEATURE_MODEM_1X_COEXISTENCE  */
#include "srchmc_v.h"
#include "srchtc_ta.h"

//other
#include "qw.h"
#include "queue.h"
#include "rex.h"
#include "amssassert.h"
#include "err.h"
#include "crc.h"
#include "bit.h"
#include "log.h"
#include "msg.h"
#include "m1x_diag.h"
#include "m1x_time_i.h"
#include "dsrlp_api.h"
#include "event.h"
#include "mdrrlp_api.h"
#include "muxmsgr.h"

#ifdef FEATURE_DATA_MM
#include "ds707_extif.h"
#endif /* FEATURE_DATA_MM */

#include "dsrlp_stats.h"

#ifdef FEATURE_MODEM_1X_SUPPORTS_RF
#include "rfm_1x.h"
#endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

#include "stringl.h" /* for memscpy */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
#define FCH_BUF       0   // Index to buffer for FCH in RLP data buffer
#define DCCH_BUF      1 // Index to buffer for DCCH in RLP data buffer
#define SCH_BUF_START 2 // Index to 1st buffer for SCH in RLP data buffer

byte *prim_data_ptr;           /* Pointer to primary data */
cai_data_rate_type prim_rate;  /* Primary data rate */
byte *prim_dcch_data_ptr = NULL;   /* Pointer to DCCH primary data */
cai_data_rate_type prim_dcch_rate; /* Primary DCCH data rate */

#ifdef FEATURE_IS2000_REL_A_SVD
byte *sec_fch_data_ptr  = NULL;    /* Pointer to FCH secondary data */
word sec_fch_num_bits = 0;         /* Num of bits in FCH secondary frame */

byte *sec_dcch_data_ptr  = NULL;   /* Pointer to DCCH secondary data */
word sec_dcch_num_bits = 0;        /* Num of bits in DCCH secondary frame */
#endif // FEATURE_IS2000_REL_A_SVD

static dsrlp_rev_frame_block_type txc_rev_fr_block;  /* Set of all RLP frames to be tx*/

#ifdef FEATURE_MODEM_1X_DEVICE_ONLY_DTX
/* Enable/Disable bits for Device only DTX  - NV 70193 */
#define DTX_DISABLED_ALL_RATES    0x0000 /* No Bits are set    */
#define DTX_ENABLED_RC8_FRAC_RATE 0x0001 /* Bit  0     */
#define DTX_ENABLED_RC3_FRAC_RATE 0x0002 /* Bit  1     */
#define DTX_ENABLED_FRAC_RATE     0x0003 /* Bits 0 and 1       */
#define DTX_ENABLED_RC8_FULL_RATE 0x0004 /* Bit  2     */
#define DTX_ENABLED_RC3_FULL_RATE 0x0008 /* Bit  3     */
#define DTX_ENABLED_FULL_RATE     0x000C /* Bits 2 and 3       */
#define DTX_ENABLED_ALL_RATES     0x000F /* Bits 0, 1, 2 and 3 */
#define DTX_ENABLED_TEST_SO       0x8000 /* Bit 15     */

/* T2P Boost for Full Rate and Fractional Rate frames for DTX (dBq3) */
#define T2P_BOOST_FRAC_RATE_DTX   40     /* 40 dBq3 == 5 dB */
#define T2P_BOOST_FULL_RATE_DTX   16     /* 16 dBq3 == 2 dB */

/*--------------------------------------------------------------------------
  Device only DTX NV controlled parameters loaded to default values if
  NV item # 70193 is not present or not set.
--------------------------------------------------------------------------*/
txctraffic_device_only_dtx_nv_params_type txc_device_dtx_nv;

/*--------------------------------------------------------------------------
  Device only DTX statically assigned parameters.
--------------------------------------------------------------------------*/
txctraffic_device_only_dtx_static_params_type txc_device_dtx_static =
{
  T2P_BOOST_FRAC_RATE_DTX,
  T2P_BOOST_FULL_RATE_DTX,
};

/* Static define Dynamic DTX control thresholds */
txctraffic_dynamic_dtx_thresh_type txc_dynamic_dtx_thresh =
{
  60,       /* Tx power offset in dBm10 from Max Tx power to turn ON DTX:
               60 dBm10 = 6 dBm */
  40,       /* Tx power offset in dBm10 from Max Tx power to turn OFF DTX:
               40 dBm10 = 4 dBm */
  3,        /* FL setpt offset in dBq3 from Max FL setpoint:
               3 dBq3 = 0.375 dB */
};
#endif /* FEATURE_MODEM_1X_DEVICE_ONLY_DTX */

/* Initialize the internal Device only DTX structure */
txctraffic_device_dtx_param_type txc_device_only_dtx_param =
{
  FALSE,                  /* Initialize DTX Frame Flag to FALSE           */
  DTX_SB_MASK_DISABLED,   /* Initialize DTX Mask to 0xFFFF                */
  0,                      /* Initialize T2P boost in dBq3 to 0            */
};

/* Final rate transmitted this frame. We save the value here for
   logging purposes since the value in txtc.rate is changed in the
   next encoder bin for building the next Tx frame */
enc_rate_type tx_frame_rate = ENC_NUM_FRAC_RATES;

#ifndef FEATURE_MODEM_1X_UTX
/* Count of no of bytes written to LMEM for this PDU */
uint32 txc_enc_pdu_byte_count = 0;
#endif /* !FEATURE_MODEM_1X_UTX */

txc_rsch_state sch_state = TXC_REV_SCH_OFF;

/* Holding buffer for TX data on the reverse FCH*/
txc_tc_frame_type       txc_ftm_rfch_data;
enc_rate_type           txc_ftm_rfch_rate;

/* Holding buffer for TX data on the reverse DCCH*/
txc_tc_frame_type       txc_ftm_rdcch_data;
enc_rate_type           txc_ftm_rdcch_rate;
boolean                 txc_ftm_dtx_dcch = FALSE;

/* Holding buffer for TX data on the reverse SCH*/
txc_tc_supp_frame_type  txc_ftm_rsch_data;
enc_sch_rate_type       txc_ftm_rsch_rate;

extern rxc_bufs_qs_vars rxc_bufs_qs_data;

/* Accumulation buffer for LOG_REV_PCTRL_C log packet */
mux_log_rev_pctrl_type                  txc_log_rpctrl_buf;

/* Sub-record pointer for LOG_REV_PCTRL_C log packet */
mux_log_rev_pctrl_subrecord_type       *txc_log_rpctrl_sr_ptr = NULL;

/* Reverse frame types log accumulation buffer */
/* Accumulation buffer */
mux_log_rev_frame_types_type            txc_log_frm_types_buf;

/* Sub-record pointer */
mux_log_rev_frame_types_subrecord_type *txc_log_frm_types_sr_ptr = NULL;

boolean txc_connect_priso = FALSE;

#ifdef FEATURE_IS2000_REL_A_SVD
boolean txc_connect_secso = FALSE;
#endif // FEATURE_IS2000_REL_A_SVD

// These are initialized in case of HHO and extra_params is TRUE
// in the HO cmd. They are only used when the HO cmd is followed
// by a HO_OK cmd
byte txc_stored_nom_pwr = 0;
byte txc_stored_num_preambles = 0;
boolean txc_hho_preamble_pcg_units = FALSE;

#ifdef FEATURE_MODEM_1X_DRX
/* This ensures DRX remains OFF during frame offset change which may
last more than 20ms and before that SRCH can trigger DRX */
boolean txc_frame_offset_change_in_progress = FALSE;
#endif /* FEATURE_MODEM_1X_DRX */

#define  MAX_R_SCH_BLANK_PDU  92  /* maximum PDU5 body size, CONV, 32X, RS1 */

/*-----------------------------------------------------------------------
  Buffer used to write zero's to the ENC when doing blank mux pdu's
  (where the first byte is 0xE0.  This is not sized to include the
  0xE0, as in turbo, you don't necessarily use it.  So this covers
  the size of the PDU, minus the first byte
-----------------------------------------------------------------------*/
LOCAL const byte r_sch_blank_buffer[MAX_R_SCH_BLANK_PDU] =
                {
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0
                };
/*------------------------------------------------------------------------
  For MSM5100 data to the encoder is written as 4 bytes at a time using
  ENC_SCH_DATA_OUT
  Since we use ENC_SCH_DATA_OUT to write the data into encoder from
  r_sch_blank_pdu_hdr, r_sch_blank_pdu_hdr should also be 4 byte aligned.
  If it is not 4 bytes long, then only byte 1 will be valid and bytes 2,
  3 & 4 are invalid. To make all the 4 bytes valid, we make it as unit32.
------------------------------------------------------------------------*/
LOCAL const byte r_sch_blank_pdu_hdr[4] = {0xE0,0,0,0};

LOCAL byte               txc_sch_spare_bits[29] ALIGN(4)=
                               {
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0
                               };

/*
** The IS2000 standard specifies that the "spare bits" remaining after
** the maximum number of PDUs for the given rate have been used up to form
** the SDU shall be fille with 0's. Since turbo coded frames do not have LTU
** CRCs, this number is a function of the supplemental channel data rate and
** the code type. Convolutional = 0, Turbo = 1.
*/
LOCAL byte               txc_num_sch_spare_bits[ENC_NUM_SCH_RATES][2] =
                               {
                                 {0, 0},          /* 1500   */
                                 {0, 0},          /* 2700   */
                                 {0, 0},          /* 4800   */
                                 {0, 0},          /* 9600   */
                                 {8, 8},          /* 19200  */
                                 {8, 40},         /* 38400  */
                                 {40, 104},       /* 76800  */
                                 {104, 232},      /* 153600 */
                                 /* Having 0's for 307.2kbps entry is right
                                  * because the array is only meant for PDU type
                                  * 3.  307.2kbps only uses PDU type 5 and uses
                                  * a separate array. */
                                  {0, 0}          /* 307.2 Kbps */
                               };

#ifdef FEATURE_IS2000_REL_A
  /* The spare bit for MUXPDU 5. This number is a function of the supplemental channel
  ** data rate and the code type. Convolutional = 0, Turbo = 1. The spare bit should be
  ** byte alignment. The only exception in this case for 1x, the spare bit for RS1 is 4
  ** and RS2 is 3, but we will around it to 8 bits
  */
  LOCAL byte               txc_muxpdu5_nsch_sbits[ENC_NUM_SCH_RATES][2] =
                                 {
                                   {0, 0},
                                   {0, 0},
                                   {0, 0},
                                   {8, 8},
                                   {0, 0},
                                   {8, 0},
                                   {8, 0},
                                   {40,0},
                                   {40,0}
                                 };

  /* The maximum frame size of MUXPDU 5, rate set 1. This number is a function of the
  ** supplemental channel data rate and the code type. Convolutional = 0, Turbo = 1.
  */
  LOCAL word               txc_muxpdu5_rs1_maxsize[ENC_NUM_SCH_RATES][2] =
                                 {
                                   {0, 0},             /* 1500   */
                                   {0, 0},             /* 2700   */
                                   {0, 0},             /* 4800   */
                                   {20, 20},           /* 9600   */
                                   {44, 44},           /* 19200  */
                                   {43, 92},           /* 38400  */
                                   {44, 188},          /* 76800  */
                                   {44,380},           /* 153600 */
                                   {92,764}            /* 307200 */
                                 };
  /* The maximum frame size of MUXPDU 5, rate set 2. This number is a function of the
  ** supplemental channel data rate and the code type. Convolutional = 0, Turbo = 1.
  */
  LOCAL word               txc_muxpdu5_rs2_maxsize[ENC_NUM_SCH_RATES][2] =
                                 {
                                   {0, 0},             /* 1800   */
                                   {0, 0},             /* 3600   */
                                   {0, 0},             /* 7200   */
                                   {32, 32},           /* 14400  */
                                   {68, 68},           /* 28800  */
                                   {67, 140},          /* 57600  */
                                   {68, 284},          /* 115200 */
                                   {68,572},           /* 230400 */
                                   {140,1148}          /* 460800 */
                                 };
  /* The maximum frame size of MUXPDU 5 for TDSO in bits, for rate set 1. This number is a function of the
  ** supplemental channel data rate and the code type. Convolutional = 0, Turbo = 1.
  */
  LOCAL word                txc_tdso_muxpdu5_rs1[(uint32) TDSO_SCH_RATE_32X+1][ 2 ] =
                                 {
                                   {166, 166},           /* 9600   */
                                   {354, 354},           /* 19200  */
                                   {346, 738},           /* 38400  */
                                   {354,1506},           /* 76800  */
                                   {354,3042},           /* 153600 */
                                   {738,6114}            /* 307200 */
                                 };

  /* The maximum frame size of MUXPDU 5 for TDSO in bits, for rate set 2. This number is a function of the
  ** supplemental channel data rate and the code type. Convolutional = 0, Turbo = 1.
  */
  LOCAL word                txc_tdso_muxpdu5_rs2[(uint32) TDSO_SCH_RATE_32X+1][ 2 ] =
                                 {
                                   {261,  261},           /* 14400  */
                                   {546,  546},           /* 28800  */
                                   {538, 1122},           /* 57600  */
                                   {546, 2274},           /* 115200 */
                                   {546, 4578},           /* 230400 */
                                   {1122,9186}            /* 460800 */
                                 };
#endif /* FEATURE_IS2000_REL_A */

int16  tx_extra_gain = 0;
boolean txc_max_pwr_limited_dtx_sch = FALSE;
boolean txc_sch_max_pwr_protection_enabled = TRUE;
word  txc_rlp_null_data = 0xFFFF;

LOCAL enc_rate_type curr_fch_frame_rate;  /*lint -e552 used for logging */
LOCAL enc_rate_type next_fch_frame_rate;
LOCAL mux_log_sch_rate_enum_type curr_sch0_frame_rate; /*lint -e552 used for logging */
LOCAL mux_log_sch_rate_enum_type next_sch0_frame_rate;
LOCAL txc_sig_frame_type next_fch_sig_type;
LOCAL txc_sig_frame_type curr_fch_sig_type; /*lint -e552 used for logging */

/* Specific variables for the Traffic Channel */
txtc_type txtc;

boolean txc_eib_flag;
#ifdef FEATURE_IS2000_REL_A
boolean txc_qib_flag;
boolean txc_sch_eib_flag;
#endif /* FEATURE_IS2000_REL_A */

/*---------------------------------------------------------------------------
  Indicates if the R-SCH should be checked for throttling.  It is a word
  instead of a boolean, because some of the R-SCH params change a little
  bit asynchronously.  By setting it to some value like 3, then in 3
  frame times, the R-SCH should hopefully be okay.  And the idea is that
  the phone won't crash due to processor overload in 3 frame times.
---------------------------------------------------------------------------*/
#define TXC_SCH_CHANGED_DELAY   2
word txc_sch_changed = 0;

typedef void (*txc_transaction_processor_type) (mctrans_action_packet_type *);
  /* function prototype for transaction callback function */

/*-------------------------------------------------------------------------
  This table gives the number of bits corresponding to each RLP rate.
  This is needed because, RLP gives the rate at which it has built a
  frame and Mux will then use that rate to lookup for bit count in this
  table.
------------------------------------------------------------------  */
const word rlp_to_mux_table[(uint32) DSRLP_RATE_ERASURE][(uint32) DSRLP_GENERIC_SRVC]
                           [(uint32) DSRLP_MUX_PDU_3] =
{
  {{171, 266},     {168, 262}},       /* Rate 1 -   full rate           */
  {{  0,   0},     {152, 242}},       /* Rate 7/8 - secondary only      */
  {{  0,   0},     {128, 208}},       /* Rate 3/4 - secondary only      */
  {{ 80, 124},     { 88, 138}},       /* Rate 1/2 - half rate           */
  {{  0,   0},     {  0, 121}},       /* Rate 7/16- secondary only      */
  {{  0,   0},     {  0, 101}},       /* Rate 3/8 - secondary only      */
  {{ 40,  54},     {  0,  67}},       /* Rate 1/4 - quarter rate        */
  {{  0,   0},     {  0,  52}},       /* Rate 3/16- secondary only      */
  {{ 16,  20},     {  0,  32}},       /* Rate 1/8 - eigth rate          */
  {{  0,   0},     {  0,  20}}        /* Rate 1/16- secondary only      */
};

#ifdef FEATURE_IS2000_REL_A_SVD
const word dsrlp_mask_table[(uint32) DSRLP_RATE_ERASURE_SCH][(uint32) DSRLP_GENERIC_SRVC]
                          [(uint32) ENC_RATE_14400+1] =
{
  /* FCH Rate 1 */
  {
    /* Primary */
    { DSRLP_RATE_1_MASK|DSRLP_RATE_1_2_MASK|DSRLP_RATE_1_4_MASK|DSRLP_RATE_1_8_MASK,
      DSRLP_RATE_1_MASK|DSRLP_RATE_1_2_MASK|DSRLP_RATE_1_4_MASK|DSRLP_RATE_1_8_MASK },

    /* Secondary */
    { DSRLP_RATE_1_MASK,
      DSRLP_RATE_1_MASK|DSRLP_RATE_7_16_MASK|DSRLP_RATE_3_16_MASK|DSRLP_RATE_1_16_MASK }
  },

  /* FCH Rate 7/8 */
  {
    /* Primary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_BLANK_MASK },

    /* Secondary */
    { DSRLP_RATE_BLANK_MASK|DSRLP_RATE_7_8_MASK,
      DSRLP_RATE_BLANK_MASK|DSRLP_RATE_7_8_MASK|DSRLP_RATE_3_8_MASK|DSRLP_RATE_1_8_MASK }
  },

  /* FCH Rate 3/4 */
  {
    /* Primary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_BLANK_MASK },

    /* Secondary */
    { DSRLP_RATE_BLANK_MASK|DSRLP_RATE_3_4_MASK,
      DSRLP_RATE_BLANK_MASK|DSRLP_RATE_3_4_MASK|DSRLP_RATE_1_4_MASK }
  },

  /* FCH Rate 1/2 */
  {
    /* Primary */
    { DSRLP_RATE_1_2_MASK|DSRLP_RATE_1_4_MASK|DSRLP_RATE_1_8_MASK,
      DSRLP_RATE_1_2_MASK|DSRLP_RATE_1_4_MASK|DSRLP_RATE_1_8_MASK },

    /* Secondary */
    { DSRLP_RATE_BLANK_MASK|DSRLP_RATE_1_2_MASK,
      DSRLP_RATE_BLANK_MASK|DSRLP_RATE_1_2_MASK }
  },

  /* FCH Rate 7/16 */
  {
    /* Primary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_BLANK_MASK },

    /* Secondary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_7_16_MASK }
  },

  /* FCH Rate 3/8 */
  {
    /* Primary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_BLANK_MASK },

    /* Secondary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_3_8_MASK }
  },

  /* FCH Rate 1/4 */
  {
    /* Primary */
    { DSRLP_RATE_1_4_MASK|DSRLP_RATE_1_8_MASK, DSRLP_RATE_1_4_MASK|DSRLP_RATE_1_8_MASK },

    /* Secondary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_1_4_MASK }
  },

  /* FCH Rate 3/16 */
  {
    /* Primary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_BLANK_MASK },

    /* Secondary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_3_16_MASK }
  },

  /* FCH Rate 1/8 */
  {
    /* Primary */
    { DSRLP_RATE_1_8_MASK, DSRLP_RATE_1_8_MASK },

    /* Secondary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_1_8_MASK }
  },

  /* FCH Rate 1/16 */
  {
    /* Primary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_BLANK_MASK },

    /* Secondary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_1_16_MASK }
  },

  /* FCH Erasure */
  {
    /* Primary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_BLANK_MASK },

    /* Secondary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_BLANK_MASK }
  },

  /* FCH Blank Rate */
  {
    /* Primary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_BLANK_MASK },

    /* Secondary */
    { DSRLP_RATE_BLANK_MASK, DSRLP_RATE_BLANK_MASK }
  }
};
#endif /* defined FEATURE_IS2000_REL_A_SVD */

/* The following states are the build states in every frame.  Frame building
** starts at the 1st ENC ISR of the frame.  Most of the lengthy build process
** for RLP service options is done in task context following the 1st ENC ISR.
** By the 2nd ISR of the same frame, if the frame is ready, it is sent in ISR
** context.  If the frame is not ready, the ISR will send a null frame on the
** FCH (if assigned) and DTX the DCCH and SCH.  In the following frame, the
** 1st ENC ISR will not start a new build frame process.  The 2nd ENC ISR will
** send the late frame (built in the previous frame.)
**
** This mechanism is used to handle situation where TXC is starved so the
** build frame process finishes late in the current frame or overflows to
** the next frame.  By not allowing late frames to propagate, TXC should
** recover after dropping 1 frame.
*/
typedef enum
{
  TXC_BUILD_FRAME_MIN = 0,       /* range checking */
  TXC_BUILD_FRAME_DONE = TXC_BUILD_FRAME_MIN,  /* default state */
  TXC_BUILD_FRAME_IN_PROGRESS,   /* frame is being built */
  TXC_BUILD_FRAME_LATE,          /* building frame takes too long */
  TXC_BUILD_FRAME_BUILT,         /* frame is built, ready to send */
  TXC_BUILD_FRAME_MAX = TXC_BUILD_FRAME_BUILT  /* range checking */

} txc_build_frame_enum_type;

LOCAL txc_build_frame_enum_type txc_build_frame_state =
                                        TXC_BUILD_FRAME_DONE;

txc_ta_type txc_ta_info;
/*===========================================================================

                 FUNCTION PROTOTYPE OF LOCAL FUNCTIONS

===========================================================================*/
void txc_register_action_handlers_notifier_interrupt_func( void );
void (*txc_action_time_notifier_func_ptr)( mctrans_server_token_type, qword );

#ifdef FEATURE_IS2000_REL_A_SVD
void txc_max_pwr_prot( void );
void txc_tx_process_sch( dsrlp_rev_frame_block_type * );
void txc_tx_post_rlp_fch_process( dsrlp_rev_frame_block_type * );
void txc_tx_post_rlp_dcch_process( dsrlp_rev_frame_block_type * );
#endif /* FEATURE_IS2000_REL_A_SVD*/

void txc_build_dcch_frame( void );
void txc_dtx_dcch_frame( void );
void txc_rpctrl_log_frame( byte );
void txc_stop_tx( boolean hho );

volatile uint8 txc_tt_tx_adj_boost = 8; /* tx gain adjust boost in dB after TT HHO */
volatile uint8 txc_sig_frame_delay_value = 0;
volatile uint8 txc_sig_frame_delay_for_tc_value = 0;
volatile uint8 txc_sig_frame_delay_for_tt_value = 0;
volatile uint8 txc_sig_quick_repeat_value = 0;
volatile uint8 txc_sig_quick_repeat_for_tc_value = 0;
volatile uint8 txc_sig_quick_repeat_for_tt_value = 2; /* 2 additional OTAs sent
                                                         for CSFB/timetransfer */

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

#ifdef FEATURE_IS2000_1X_ADV
/*===========================================================================

FUNCTION TXC_FRAME_IS_GUARANTEED

DESCRIPTION
  This function determines if the current frame (based on frame number) is
  guaranteed based on the following criteria:

      - Current RC is 8 (1x Advanced)
      - R-SCH is not assigned (No RLP control frames);
      - (FRAME_NUMBER + FRAME_OFFSET) mod BLANKING_DUTY_CYCLE != 0

  A frame *MUST* be sent when any of the above criteria is FALSE.

DEPENDENCIES
  1x should already be in the traffic state.

RETURN VALUE
  True if the frame is a guaranteed frame, False otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean txc_frame_is_guaranteed
(
  uint32 frame_num,
  cai_fch_blanking_dutycycle_type duty_cycle
)
{
  uint8 blankable_frames;

  switch( duty_cycle )
  {
    case CAI_FCH_BLANKING_DISABLED:
      /* No FCH frames are blanked */
      blankable_frames = 0;
      break;

    case CAI_FCH_N_1_OF_4_FRAMES:
      /* 3 out of every 4 frames may be blanked. */
      blankable_frames = 3;
      break;

    case CAI_FCH_N_1_OF_8_FRAMES:
      /* So 7 out of every 8 frames may be blanked. */
      blankable_frames = 7;
      break;

    default:
      /* No FCH frames are blanked */
      blankable_frames = 0;
      M1X_MSG( MUX, LEGACY_ERROR,
        "Invalid FCH blanking dutycycle, %d",
        duty_cycle);
  }

  if( ( txc_so.rev_fch.rc == CAI_RC_8 ) &&
      ( ( (frame_num + txc_frame_offset) & blankable_frames ) != 0 ) &&
      ( txtc.sch_on == FALSE ) )
  {
    return FALSE;
  }

  return TRUE;
} /* txc_frame_is_guaranteed */
#endif /* FEATURE_IS2000_1X_ADV */

#ifdef FEATURE_MODEM_1X_DEVICE_ONLY_DTX
/*===========================================================================

FUNCTION TXC_DISABLE_DTX_DYNAMIC_CONTROL

DESCRIPTION
  This function decides if the DTX should be disabled based on Tx Power and
  FL Set point thresholds.

DEPENDENCIES
  None

RETURN VALUE
  TRUE:  If DTX should be disabled due to dynamic thresholds
  FALSE: Otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean txc_disable_dtx_dynamic_control( void )
{
  /* Return if DTX should be disabled */
  boolean disable_dtx                   = FALSE;
  boolean setpoint_metric_passed        = FALSE;

  /* Make the TX Power metric "Static" for hysteresis */
  static boolean txpower_metric_passed  = FALSE;

  /* Get the Tx power metrics */
  int32  filtered_tx_power_dbm10           = txc_get_filtered_tx_power_dbm10();
  int32  tx_power_limit_dbm10              = txc_get_max_tx_agc_dbm10();

  /* Calculate the tx power thresholds */
  int32  tx_power_dtx_on_threshold_dbm10   =
    ( tx_power_limit_dbm10 - txc_dynamic_dtx_thresh.txpower_on_thresh_dbm10 );
  int32  tx_power_dtx_off_threshold_dbm10  =
    ( tx_power_limit_dbm10 - txc_dynamic_dtx_thresh.txpower_off_thresh_dbm10 );

  /* Get the setpoint metrics in dBQ3 */
  int32  current_fl_setpoint_dbq3 = ffpc_get_current_fch_setpoint_dbq3();
  int32  max_fl_setpoint_dbq3     = ffpc_get_max_fch_setpoint_dbq3();

  /* Calculate the setpoint thresholds in dBQ3 */
  int32  fl_setpoint_threshold_dbq3  =
    ( max_fl_setpoint_dbq3 - txc_dynamic_dtx_thresh.flsetpt_thresh_dbq3 );

  /* See if the Tx Power metric passes - Need to explicitly set the value
     for passing and failing cases because of hysteresis */
  if ( filtered_tx_power_dbm10 <= tx_power_dtx_on_threshold_dbm10 )
  {
    txpower_metric_passed = TRUE;
  }
  else if ( filtered_tx_power_dbm10 >= tx_power_dtx_off_threshold_dbm10 )
  {
    txpower_metric_passed = FALSE;
  }

  /* See if the FL Setpoint metric passes */
  if ( current_fl_setpoint_dbq3 <= fl_setpoint_threshold_dbq3 )
  {
    setpoint_metric_passed = TRUE;
  }

  /* If either of the Tx power metric or FL Setpoint metric doesn't pass,
     the DTX is not turned ON */
  if ( txpower_metric_passed == FALSE || setpoint_metric_passed == FALSE )
  {
    disable_dtx = TRUE;
  }

  DTX_DBG_MSG( "1x DTX: Tx Power: DTX ON Threshold = %d dBm10, "
            "DTX OFF Threshold = %d dBm10, "
            "Filtered Tx power = %d dBm10, Tx Power Metric = %d",
            tx_power_dtx_on_threshold_dbm10,
            tx_power_dtx_off_threshold_dbm10,
            filtered_tx_power_dbm10,
            txpower_metric_passed );

  DTX_DBG_MSG( "1x DTX: FL Setpoint Threshold = %d dBq3, "
            "Current FL Setpoint = %d dBq3, FL Setpoint Metric = %d",
            fl_setpoint_threshold_dbq3,
            current_fl_setpoint_dbq3,
            setpoint_metric_passed );

  return disable_dtx;

} /* txc_disable_dtx_dynamic_control */

/*===========================================================================

FUNCTION TXC_DEVICE_ONLY_DTX_DECISION

DESCRIPTION
  This function decides if the current frame should be DTX'd for transmission.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void txc_device_only_dtx_decision( boolean sb_is_on )
{
  /* Run the dynamic algorithm to enable/disable DTX */
  boolean disable_dtx_dynamic_control = txc_disable_dtx_dynamic_control();

  /* Enable DTX on R-FCH only if
     1. TX is ON and
     2. SB is OFF and
     3. R-SCH is OFF or DTX'd and
     4. DCCH is OFF and
     5. Frame is not Half Rate and
     6. Dynamic thresholds pass and
     7. No de-sense happens due to COEX.
  */
  do
  {
    /* If TX is OFF, there is no R-FCH to apply gating. So DTX is OFF */
    if ( !txc_tx.tx_on )
    {
      break;
    }

    /* If SB is ON, R-FCH gains are zero'ed. So DTX is OFF */
    if ( sb_is_on )
    {
      break;
    }

    /* When SCH is active, then R-FCH contains valid data. So DTX is OFF */
    if ( sch_state == TXC_REV_SCH_ACTIVE )
    {
      break;
    }

    /* When DCCH is ON, DTX is OFF as DTX is only applicable for R-FCH */
    if ( txc_so.rev_dcch.included )
    {
      break;
    }

    /* HALF rate frames are used for transition between FULL rate
       and 1/8th rate frames. It is also used to carry TTY packets.
       So DTX is OFF */
    if ( tx_frame_rate == ENC_HALF_RATE )
    {
      break;
    }

    /* If Dynamic thresholds don't pass, then disable DTX */
    if ( disable_dtx_dynamic_control )
    {
      break;
    }

    #ifdef FEATURE_MODEM_COEXISTENCE_FW
    /* Check Co-ex to see if DTX needs to be disabled, only in DSDA mode */
    if ( srch_coex_dsda_is_enabled() && srch_coex_disable_dtx() )
    {
      break;
    }
    #endif /* FEATURE_MODEM_COEXISTENCE_FW */

    /* Enable Device only DTX for this frame */
    txc_device_only_dtx_param.dtx_frame          = TRUE;

    /* For FULL rate frames, only gate off 25% of PCGs. This is to ensure that
       Full Rate frames are not affected and at the same time get some power
       savings by using DTX */
    if( tx_frame_rate == ENC_FULL_RATE )
    {
      /* The 1st 8 PCGs are ON PCGs. The remaining PCGs are gated at
         50% duty cycle (2 ON - 2 OFF) */
      txc_device_only_dtx_param.dtx_mask = DTX_MASK_ENABLED_FULL_RATE;

      /* Get the T2P Boost in dBq3 for Full Rate Frames */
      txc_device_only_dtx_param.dtx_t2p_boost_dbq3 =
        txc_device_dtx_static.dtx_t2p_boost_full_rate_dbq3;

      /* For Full Rate frames, 6 PC bits are valid. Hence there is no standard
         defined power control mode available for DTX'ing FULL rate frames.
         Since the PC mode is not standard, not changing the default PC mode */

      /* Valid RPC bits - 1,3,5,7,9,13 */
      txtc.rc8_status.rpc_mask = RPC_MASK_300_HZ;

      /* Valid FPC bits - 1,3,5,7,11,15 */
      txtc.rc8_status.fpc_mask = FPC_MASK_300_HZ;
    }
    else /* Fractional rate frames - Gate off 50% of PCGs */
    {
      /* The DTX mask needs to be set to follow 2 ON and 2 OFF gating pattern */
      txc_device_only_dtx_param.dtx_mask           = DTX_MASK_ENABLED;

      /* Get the T2P Boost in dBq3 for Fractional Rate Frames */
      txc_device_only_dtx_param.dtx_t2p_boost_dbq3 =
        txc_device_dtx_static.dtx_t2p_boost_frac_rate_dbq3;

      /* For RC8 operation, default Power Control (PC) mode is 400 Hz.
         All odd PCGs carry PC bits and even PCGs carry ACKs. For DTX,
         since the pilot is gated at 50% duty cycle (2 ON - 2 OFF),
         the # of odd PCGs availbale to transmit PC bits comes down
         from 8 to 4. Hence the PC mode should be set to 200 Hz */
      txtc.rc8_status.pwr_ctrl_mode = CAI_PWR_CNTL_200_HZ;

      #ifdef FEATURE_IS2000_1X_ADV
      /* Write the power control mode to be used for this frame */
      rc8_frame_config_parms.rpc_mode = txtc.rc8_status.pwr_ctrl_mode;
      #endif /* FEATURE_IS2000_1X_ADV */

      /* Valid RPC bits - 1,5,9,13 */
      txtc.rc8_status.rpc_mask = RPC_MASK_200_HZ;

      /* Valid FPC bits - 3,7,11,15 */
      txtc.rc8_status.fpc_mask = FPC_MASK_200_HZ;
    }

  } while( 0 );

  DTX_DBG_MSG( "1x DTX: RC = %d, Rate = %d, SCH = %d, DCCH = %d",
               txc_so.rev_fch.rc,
               tx_frame_rate,
               sch_state,
               txc_so.rev_dcch.included );

  DTX_DBG_MSG( "1x DTX: TX = %d, SB = %d, Dynamic Thresholds = %d, DTX = %d",
               txc_tx.tx_on,
               sb_is_on,
               !disable_dtx_dynamic_control,
               txc_device_only_dtx_param.dtx_frame );

} /* txc_device_only_dtx_decision */

/*===========================================================================

FUNCTION       TXC_DEVICE_ONLY_DTX_EFS_INIT

DESCRIPTION    This function initializes DTX parameters. NV item # 70193.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
void txc_device_only_dtx_efs_init( void )
{
  struct
  {
    uint16   dtx_enabled;
    uint16   reserved1;
    int16    reserved2;
    int16    reserved3;
    int16    reserved4;
  } efs;

  /* Load thresholds and parameters */
  if( onex_efs_read( MUX_EFS_DEVICE_ONLY_DTX_PARAMS_FILENAME,
                     &efs, sizeof( efs ) ) )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "DTX Parameters loaded from EFS" );

    /* Update Parameters */
    txc_device_dtx_nv.dtx_enabled = efs.dtx_enabled;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_HIGH,
       "No EFS File for DTX parameters. Using the defaults" );

    /* Device only DTX enabled for all rates and all RCs (Non-test SO's only) */
    txc_device_dtx_nv.dtx_enabled = DTX_ENABLED_ALL_RATES;
  }

  M1X_MSG( MUX, LEGACY_HIGH,
    "1x DTX: DTX enabled is set to (%d), "
    "T2P Boost: Fractional Rate is %d dB, Full Rate is %d dB",
    txc_device_dtx_nv.dtx_enabled,
    txc_device_dtx_static.dtx_t2p_boost_frac_rate_dbq3>>3,
    txc_device_dtx_static.dtx_t2p_boost_full_rate_dbq3>>3 );

} /* txc_device_only_dtx_efs_init */
#endif /* FEATURE_MODEM_1X_DEVICE_ONLY_DTX */

#ifdef FEATURE_IS2000_1X_ADV
/*===========================================================================

FUNCTION TXC_1XA_FRAME_PREP

DESCRIPTION
   This function is used to prepare a 1xA frame for transmission. The function
   determines if the current frame should be DTX'd due to the smart blanking
   duty cycle and also determines the RPC mode and step size to use. It also
   programs the mDSP with these values and the gain ratios needed for the
   currrent frame.

DEPENDENCIES
  1x should already be in the traffic state. It should be called from
  bin 3 of the txc_traffic_isr() prior to loading the encoder with
  bits for transmission.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void txc_1xA_frame_prep( uint32 frame_num )
{
  /* Clear the SB flag in preparation for this frame. */
  txtc.rc8_status.blank_non_crit_fch_frame = FALSE;

  /* Default the RPC mode to the value provided in the RCPM.
     Set the RPC step size */
  txtc.rc8_status.pwr_ctrl_mode = txc_rcpm_data.power_control_mode;
  txtc.rc8_status.pwr_ctrl_step_size =
    (cai_pwr_cntl_step_type) txtc.pwr_ctrl_step_size;

  if( !txc_frame_is_guaranteed( frame_num,
                                txc_rcpm_data.rev_fch_blanking_dutycycle ) )
  {
    /* This frame is NOT guaranteed for transmission. */
    if( ( txtc.rc8_status.is_non_critical_frame == TRUE ) &&
        ( txtc.rate == ENC_8TH_RATE ) )
    {
      /* The rate provided by the vocoder is a non-critical frame that
         can be blanked. Also, the final rate after muxing any
         signaling/data (txtc.rate) is still eighth rate.

         Signal the mDSP to blank this frame. Set the PC mode to 200Hz.
      */
      txtc.rc8_status.pwr_ctrl_mode = CAI_PWR_CNTL_200_HZ;
      txtc.rc8_status.blank_non_crit_fch_frame = TRUE;
    }
  }

  /* Notify mDSP whether or not this frame should be blanked. */
  rc8_frame_config_parms.blanked_frame =
                         txtc.rc8_status.blank_non_crit_fch_frame;

  /* Write the power control mode to be used for this frame */
  rc8_frame_config_parms.rpc_mode = txtc.rc8_status.pwr_ctrl_mode;

  /* Write the power control step size for this frame */
  muxmdsp_set_tx_gain_adj_step_size( txtc.rc8_status.pwr_ctrl_step_size );

  /* For RC8, the mDSP uses linear gain ratios and not digital gains.
     NOTE: DCCH is not supported. Write 0 by default.
     NOTE: There is no PICH ratio or TX rate adjust either.
  */
  rc8_frame_config_parms.dcch_linear_gain_ratio = 0;

  /* Based on the dutycycle provided by the basestation, program
     the mDSP with the correct blanking masks for the R-ACK channels
  */
  if( txc_rcpm_data.rev_fch_blanking_dutycycle == CAI_FCH_BLANKING_DISABLED )
  {
    rc8_frame_config_parms.fch_ack_mask =
                           txc_rcpm_data.for_fch_ack_mask_no_rl_blanking;
    rc8_frame_config_parms.sch_ack_mask =
                           txc_rcpm_data.for_sch_ack_mask_no_rl_blanking;
  }
  else /* blanking enabled 1-in-4 or 1-in-8 */
  {
    rc8_frame_config_parms.fch_ack_mask =
                           txc_rcpm_data.for_fch_ack_mask_rl_blanking;
    rc8_frame_config_parms.sch_ack_mask =
                           txc_rcpm_data.for_sch_ack_mask_rl_blanking;
  }
} /* txc_1xA_frame_prep */
#endif /* FEATURE_IS2000_1X_ADV */

/*===========================================================================

FUNCTION TXC_SET_REV_CHAN_GAIN_FCH_ONLY_SCH_OFF

DESCRIPTION
  Sets the channel gains for all the reverse channels for FCH only SCH OFF case

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_set_rev_chan_gain_fch_only_sch_off( void )
{
  #ifdef FEATURE_IS2000_1X_ADV
  /*  For 1x Advanced RC's we program the linear gain ratios
      and not the digital gains since the mdsp needs
      to update the gains every 1/2 pcg

      Note: There is no PICH ratio or tx rate adj programming.
            This is handled by the mdsp.

            DCCH is not supported by this feature. We will
            write a ratio of 0 to the mdsp later.
  */
  if( txc_so.rev_fch.rc == CAI_RC_8 )
  {
    uint8 acs                                        =
            TXC_GET_ACTIVE_CELL_SIZE( srch_mux_get_active_cell_size() );
    rc8_frame_config_parms.fch_linear_gain_ratio     =
            txc_fch_ack_gain_ratio_tab[ txtc.rate ][ acs ].fch_gain_ratio;
    rc8_frame_config_parms.fch_ack_linear_gain_ratio =
            txc_fch_ack_gain_ratio_tab[ txtc.rate ][ acs ].fch_ack_gain_ratio;
    rc8_frame_config_parms.sch_linear_gain_ratio     = 0;
    rc8_frame_config_parms.sch_ack_linear_gain_ratio = 0;
  }
  else
  #endif /* FEATURE_IS2000_1X_ADV */
  {
    channel_gains.txc_pch_gain    = txc_pch_fch_gain_tab[ txtc.rate ].pch_gain;
    channel_gains.txc_fch_gain    = txc_pch_fch_gain_tab[ txtc.rate ].fch_gain;
    channel_gains.txc_sch_gain    = 0;
    channel_gains.txc_dcch_gain   = 0;
    channel_gains.txc_tx_rate_adj =
            txc_pch_adj_gain_tab[ txtc.rate ] + txtc.offset_pwr;
  }
} /* txc_set_rev_chan_gain_fch_only_sch_off */

/*===========================================================================

FUNCTION TXC_SET_REV_CHAN_GAIN_FCH_ONLY_SCH_DTX

DESCRIPTION
  Sets the channel gains for all the reverse channels for FCH only SCH DTX case

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_set_rev_chan_gain_fch_only_sch_dtx( void )
{
  #ifdef FEATURE_IS2000_1X_ADV
  /*  For 1x Advanced RC's we program the linear gain ratios
      and not the digital gains since the mdsp needs
      to update the gains every 1/2 pcg

      Note: There is no PICH ratio or tx rate adj programming.
            This is handled by the mdsp.

            DCCH is not supported by this feature. We will
            write a ratio of 0 to the mdsp later.
  */
  if( txc_so.rev_fch.rc == CAI_RC_8 )
  {
    uint8 acs                                        =
      TXC_GET_ACTIVE_CELL_SIZE( srch_mux_get_active_cell_size() );
    rc8_frame_config_parms.fch_linear_gain_ratio     =
      txc_fch_schdtx_ack_gain_ratio_tab[ txtc.rate ][ acs ].fch_gain_ratio;
    rc8_frame_config_parms.fch_ack_linear_gain_ratio =
      txc_fch_schdtx_ack_gain_ratio_tab[ txtc.rate ][ acs ].fch_ack_gain_ratio;
    rc8_frame_config_parms.sch_linear_gain_ratio     =
      txc_fch_schdtx_ack_gain_ratio_tab[ txtc.rate ][ acs ].sch_gain_ratio;
    rc8_frame_config_parms.sch_ack_linear_gain_ratio =
      txc_fch_schdtx_ack_gain_ratio_tab[ txtc.rate ][ acs ].sch_ack_gain_ratio;
  }
  else
  #endif /* FEATURE_IS2000_1X_ADV */
  {
    channel_gains.txc_pch_gain    =
         txc_dtx_gain_tab[ txtc.rate ].pch_gain;
    channel_gains.txc_fch_gain    =
         txc_dtx_gain_tab[ txtc.rate ].fch_gain;
    channel_gains.txc_sch_gain    = 0;
    channel_gains.txc_dcch_gain   = 0;
    channel_gains.txc_tx_rate_adj =
         txc_dtx_pch_adj_gain_tab[ txtc.rate ] + txtc.offset_pwr;
  }

} /* txc_set_rev_chan_gain_fch_only_sch_dtx */

/*===========================================================================

FUNCTION TXC_SET_REV_CHAN_GAIN_FCH_ONLY_SCH_ON

DESCRIPTION
  Sets the channel gains for all the reverse channels for FCH only SCH ON case

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_set_rev_chan_gain_fch_only_sch_on( void )
{
  #ifdef FEATURE_IS2000_1X_ADV
  /*  For 1x Advanced RC's we program the linear gain ratios
      and not the digital gains since the mdsp needs
      to update the gains every 1/2 pcg

      Note: There is no PICH ratio or tx rate adj programming.
            This is handled by the mdsp.

            DCCH is not supported by this feature. We will
            write a ratio of 0 to the mdsp later.
  */
  if( txc_so.rev_fch.rc == CAI_RC_8 )
  {
    uint8 acs                                        =
      TXC_GET_ACTIVE_CELL_SIZE( srch_mux_get_active_cell_size() );
    rc8_frame_config_parms.fch_linear_gain_ratio     =
      txc_fch_sch_ack_gain_ratio_tab[ txtc.rate ][ acs ].fch_gain_ratio;
    rc8_frame_config_parms.fch_ack_linear_gain_ratio =
      txc_fch_sch_ack_gain_ratio_tab[ txtc.rate ][ acs ].fch_ack_gain_ratio;
    rc8_frame_config_parms.sch_linear_gain_ratio     =
      txc_fch_sch_ack_gain_ratio_tab[ txtc.rate ][ acs ].sch_gain_ratio;
    rc8_frame_config_parms.sch_ack_linear_gain_ratio =
      txc_fch_sch_ack_gain_ratio_tab[ txtc.rate ][ acs ].sch_ack_gain_ratio;
  }
  else
  #endif /* FEATURE_IS2000_1X_ADV */
  {
    channel_gains.txc_pch_gain    =
                  txc_gain_tab[ txtc.rate ].pch_gain;
    channel_gains.txc_fch_gain    =
                  txc_gain_tab[ txtc.rate ].fch_gain;
    channel_gains.txc_sch_gain    =
                  txc_gain_tab[ txtc.rate ].sch_gain;
    channel_gains.txc_dcch_gain   =
                  txc_gain_tab[ txtc.rate ].dcch_gain;
    channel_gains.txc_tx_rate_adj =
         txc_sch_pch_adj_gain_tab[ txtc.rate ] + txtc.offset_pwr;
  }

} /* txc_set_rev_chan_gain_fch_only_sch_on */

/*===========================================================================

FUNCTION TXC_LOG_TC_MSG

DESCRIPTION
  This function logs messages sent on the traffic channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void txc_log_tc_msg
(
  txc_dsch_type     dsch,     /* Signalling channel carrying the message */
  txc_msg_buf_type *msg_ptr   /* pointer to sent message */
)
{
  log_code_type log_code = dsch == TXC_REV_FCH ? LOG_R_TC_MSG_C : LOG_REV_DCCH_MSG_C;

  mux_log_r_tc_msg_type *log_ptr;

  log_ptr = (mux_log_r_tc_msg_type *)
    log_alloc( log_code, msg_ptr->msg[0]+sizeof(log_hdr_type));

  if( log_ptr != NULL )
  {
    /* copy message into buffer */
    (void) memscpy( (void *) log_ptr->msg,
                    msg_ptr->msg[0],
                    (void *) msg_ptr->msg,
                    msg_ptr->msg[0] );

    log_commit( log_ptr );
  }

  /* Event report: Message transmitted */
  if( dsch == TXC_REV_FCH )
  {
    /* R-FCH */
    mclog_report_event_msg_txed(CAI_R_FCH, msg_ptr->msg);
  }
  else
  {
    /* R-DCCH */
    mclog_report_event_msg_txed(CAI_R_DCCH, msg_ptr->msg);
  }

} /* txc_log_tc_msg */
/*===========================================================================

FUNCTION TXC_PILOT_INIT

DESCRIPTION
  Initialize reverse pilot properly based on RC.

DEPENDENCIES
  None
RETURN VALUE
  None.
SIDE EFFECTS
  None
===========================================================================*/
void txc_pilot_init( void )
{
  /*-----------------------------------------------------------------------*/

  if( txc_so.rev_dcch.included ||
      txc_so.rev_fch.rc >= CAI_RC_3 )
  {
    // JRP - need not have FCH or DCCH.  Could be just Rev C/D channels
    M1X_MSG( MUX, LEGACY_HIGH,
      "Enabling rev pilot"  );
    (void)enc_is95c_enable( TRUE ); // IS-95C
    enc_pch_enable( TRUE );

    /* Intialize offset_pwr, so that if MS switches from low RCs to high RCs
     * tx_rate_adj is properly calculated
     */
    txtc.offset_pwr = 12 * 85 / 10;  // 8.5 * 12;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "Disabling rev pilot"  );
    (void)enc_is95c_enable( FALSE ); // IS-95A
    enc_pch_enable( FALSE );

    /* Intialize tx_rate_adj and offset_pwr to 0, so that if MS switches from
     * high RCs to low RCs tx_rate_adj doesn't get set to the high RC value
     */

    /* Set channel gains to zero (tx_rate_adj gets set to zero in this
       function */
    muxmdsp_set_channel_gains_zero();
    txtc.offset_pwr  = 0;
  }
} /* txc_pilot_init */

/*===========================================================================

FUNCTION TXC_TRAFFIC_INIT

DESCRIPTION
  Initialization section for traffic channel.

DEPENDENCIES
  None
RETURN VALUE
  None.
SIDE EFFECTS
  None
===========================================================================*/
void txc_traffic_init( void )
{
  int i;

  #ifdef FEATURE_IS2000_REL_A
  int sr_index;
  #endif /* FEATURE_IS2000_REL_A */

  /*-----------------------------------------------------------------------*/

  txtc.power = txac.last_pwr;    /* set last access probe power level */

  /* Should clear state before initializing action time processing. */
  txc_register_action_handlers_notifier_interrupt_func();

  txc_build_frame_state = TXC_BUILD_FRAME_DONE; /* default setting */

  /* Convert to units of 1/2 dB and add rl_gain_adj value
  ** Note that txtc.power is sign inverted.
  */
  txtc.power -= (byte) ( 2 * txtc.rl_gain_adj );

  /* Copy zeros into frame */
  (void)memscpy( txtc.frame.data,
                 sizeof( txtc.frame.data ),
                 zeros,
                 ENC_14400_FULL_RATE_SIZE );

  muxvoc_tx_tc_init();

  /* set long code mask for traffic channel */
  txc_isr_cmd(INT_TC_F, txc_tx.mask);

  /* Set Traffic Channel offsets. */

  #ifndef FEATURE_MODEM_1X_UTX
  if( txc_so.rev_dcch.included )
  {
    muxmdsp_configure_rl_rc(txc_so.rev_dcch.rc);
  }

  if( txc_so.rev_fch.included )
  {
    /* Send RC signalling message to FW here */
    muxmdsp_configure_rl_rc(txc_so.rev_fch.rc);
  }
  #endif /* !FEATURE_MODEM_1X_UTX */

  txc_setup_pcg_events_isr( TXC_PCG_EVENT_TRAFFIC, txc_traffic_isr );

  txtc.frame.sig = NORM;          /* no data for interrupt handler */

  txc_pilot_init();

  #ifdef FEATURE_PA_ACCESS_PROBE_CONFIG
  /* Notify RF that we are entering traffic state.  Need to do this before
  ** calling rfm_enable_tx_pwr() for traffic state, so RF can set its pa
  ** hysterysis timer appropriately.  When leaving traffic state, RF will reset its
  ** timer when function rfm_tx_disable() is called so it's not necessary
  ** for TXC to call this function with a FALSE parameter.
  */
  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  rfm_1x_set_pa_access_probe_config( txc_get_tx_device(), TRUE );
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */
  #endif /* FEATURE_PA_ACCESS_PROBE_CONFIG */

  /* -------------------------------------------
  ** Power up and program the VHF TX synthesizer
  ** ------------------------------------------- */
  txc_change_rf_tx_state( TXC_RF_STATE_ENABLE );
  enc_tx_enable( TRUE );          /* make sure power amp is on */
  txc_tx.tx_on = TRUE;            /* transmitter is on */
  rxc_set_tx_in_traffic_on( TRUE );

  /* Initialize to delay X frames before transmitting
     a signaling messages after a HHO occurs. */
  txc_sig_frame_delay_value = txc_sig_frame_delay_for_tc_value;
  txtc.sig_frame_delay = 0;

  txc_sig_quick_repeat_value = txc_sig_quick_repeat_for_tc_value;
  txtc.quick_repeat_frame_cnt = 0;

  /* Events report: Transmitter enabled */
  event_report( EVENT_TRANSMITTER_ENABLED );

  txc_tx.tx_blank = TRUE;         /* expect a blank first */

  srch_mux_set_tx_adj( txtc.power );  /* set power to last access probe level */

  /* Allow SCRM at the beginning of traffic, clear any leftover inhibition */
  rpc_set_scrm_allowed( TRUE );

  /* Reset the RPC step size to the 1dB Default value. */
  #ifdef FEATURE_IS2000_1X_ADV
  if( ( txc_so.rev_fch.included == TRUE ) &&
      ( txc_so.rev_fch.rc == CAI_RC_8 ) )
  {
    txtc.pwr_ctrl_step_size = txc_rcpm_data.power_control_step;
  }
  else
  #endif /* FEATURE_IS2000_1X_ADV */
  {
    txtc.pwr_ctrl_step_size = 0;
  }

  /* write to FW the default tx adj step size to begin with.
     0 corresponds to 0x200 which is 1dB
     This API sends the FW the step size only for MSRG interface
     For legacy, this call is a NOP and the default setting takes place
     via rf call rfm_tx_enable */
  muxmdsp_set_tx_gain_adj_step_size( (cai_pwr_cntl_step_type) 0 );

  /* default to no primary or secondary service option */
  txc_set_pri_chan_config( 0, CAI_SO_NULL );
  txc_set_sec_chan_config( 0, CAI_SO_NULL );
  txc_so.stored_pri.so = CAI_SO_NULL;

  #ifdef FEATURE_IS2000_REL_A
  /* set to defautl no service option for each reference ID */
  for( sr_index = 0; sr_index < SR_ID_MAX; sr_index++ )
  {
    txc_srid_ref[sr_index].so = CAI_SO_NULL;
    txc_srid_ref[sr_index].traffic_type = CAI_NUL_TRAF;
    txc_srid_ref[sr_index].phy_channels = 0;
  }

  prim_rate          = CAI_BLANK_RATE;
  prim_data_ptr      = NULL;
  prim_dcch_rate     = CAI_NULL_RATE;
  prim_dcch_data_ptr = NULL;
  sec_fch_num_bits   = 0;
  sec_fch_data_ptr   = NULL;
  sec_dcch_num_bits  = 0;
  sec_dcch_data_ptr  = NULL;

  #ifdef FEATURE_IS2000_REL_A_SVD
  /* txc_rlp_sig_priority is used to keep track of priorities in sending RLP control
  ** frames when RLP is the secondary SO in a SVD call.  The purpose of the
  ** implementation is to improve voice quality in a SVD call.
  */
  txc_rlp_sig_priority.prev_fr_val = DSRLP_TX_NONE;
  txc_rlp_sig_priority.curr_fr_val = DSRLP_TX_NONE;
  txc_rlp_sig_priority.delay_fr_cnt = 0;
  #endif /* FEATURE_IS2000_REL_A_SVD */
  #endif /* FEATURE_IS2000_REL_A */

  txtc.preambles = TRUE;          /* send preambles when we first get here */
  /* Preambles always in frames at TCI */
  txc_hho_preamble_pcg_units = FALSE;

  // Initialize to no pilot gating
  txtc.gating_rate = CAI_GATING_RATE_NONE;

  //------------------------------------------------------
  // For FTM this parameter is initialized during TXC_TC_F
  //------------------------------------------------------
  if( !onex_nonsig_is_ftm_mode() )
  {
    txtc.num_preambles = 0xFFFF;
  }

  txtc.nulls     = FALSE;          /* not sending null data yet */
  txtc.last_rate = ENC_8TH_RATE;   /* close as we get to nothing sent yet */

  txc_log_mux_buf.count = 0;

  txc_tx.active_msg = FALSE;
  txc_tx.delay = TXC_NO_CONF;              /* no confirmation delay */

  txtc.pot_num_mux_pdus = 0;
  txtc.ok_num_mux_pdus  = 0;

  txtc.dtx_duration_included = FALSE;

  /* We are now ready to transmit */
  txtc.ready_to_transmit = TRUE;

  /* Clear the RC Update Deferred flag here. */
  txc_tx.is_rc_update_deferred = FALSE;

  if( cdma2kdiag_get_switch( CDMA2KDIAG_SW_DISABLE_RL_MAX_PWR_PROT ) )
  {
    /* If dipswitch 11 is ON, MAX PWR PROTECTION on R-SCH is disabled */
    txc_sch_max_pwr_protection_enabled = FALSE;
  }
  else
  {
    txc_sch_max_pwr_protection_enabled = TRUE;
  }

  if( onex_nonsig_is_ftm_mode() )
  {
    for( i = 0; i < ENC_14400_FULL_RATE_SIZE; i++ )
    {
      txtc.frame.data[ i ] = 0xAA;
    }
    for( i = 0; i < ENC_SCH_RC3_32X_SIZE; i++ )
    {
      txtc.supp_frame.data[ i ] = 0x5A;
    }
  }

  enc_fch_enable( FALSE );
  enc_dcch_enable( FALSE );
  if( txc_so.rev_dcch.included )
  {
    enc_dcch_data_out( zeros, TRUE, ENC_FULL_RATE );
    enc_dcch_enable( TRUE );
  }

  if( txc_so.rev_fch.included )
  {
    enc_fch_enable( TRUE );
  }

  /* Set TX pattern to transmit in all PCGs */
  enc_is95c_tx_patn( ENC_NO_GATING_PATTERN );

  // LJL
  // Acording to interface document, if in DV mode, TXC waits for Switch before
  // setting non-zero gains. Program gains including pilot to 0
  // Can this guarantee that?

} /* txc_traffic_init */

/*===========================================================================

FUNCTION TXC_TRAFFIC_SIGNEXT

DESCRIPTION
  Process traffic channel next signaling frame type.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_traffic_signext( void )
{
  /*-----------------------------------------------------------------------*/

  /* now figure out if next frame will be dim and burst or blank and burst */
  if( txc_tx.active_msg )
  {
    if( txc_tx.buf.blank )
    {
      if( txc_tx.buf.pos < txc_tx.buf.len )
      {                                      /* Not done with message? */
        txtc.frame.sig = BLANK;           /* blank and burst next */
      }
    }
    else
    {
      if( txc_tx.buf.pos < txc_tx.buf.len )
      {  /* Not done with message? */
        txtc.frame.sig = DIM;              /* dim and burst next */
      }
    }
  }
} /* txc_traffic_signext */

/*===========================================================================

FUNCTION TXC_VOC_XCHG

DESCRIPTION
  Vocoder service routine.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_voc_xchg( void )
{
  /*-----------------------------------------------------------------------*/

  /* Before transmitting, check to make sure we are ready. */
  if( !txtc.ready_to_transmit )
  {
    return;
  }

  /* In the new frame processing, TXC_PRI returns only the voice data if
     this feature is defined. If the service option is data then txc_pri
     does nothing. */

  /* get the vocoder data */
  txc_pri ();

} /* txc_voc_xchg */

#ifdef FEATURE_IS2000_REL_A
/*         Variables for RDCH Release A logging         */
static uint32 raw_rdch_log_buffer[ FRDCH_LOG_BUFFER_SIZE/4  + 1] = {0};
static log_puma_rdch_information_type * txc_rdch_log_buffer =
  (log_puma_rdch_information_type *) raw_rdch_log_buffer; //lint !e740 Unusual pointer cast
static rdch_sub_record_type *txc_rdch_sub_record_ptr = NULL;
static rev_sch_information_type *rsch_frame_info = NULL;
static int txc_rdch_log_buff_offset = 0;
static int txc_rdch_log_sub_record_offset = 0;
static word txc_rdch_curr_sch_mux_opt = 0;

/*===========================================================================

FUNCTION TXC_RDCH_FRAME_INFO_SEND_LOG

DESCRIPTION
  Terminate and send the accumulated RDCH frame information log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A RDCH frame information log may be sent.

===========================================================================*/
void txc_rdch_frame_info_send_log( void )
{
  /*-----------------------------------------------------------------------*/

  if( txc_rdch_log_buffer->hdr.len > 0 )
  {
    /* Will send log if submitted. */
    (void) log_submit( txc_rdch_log_buffer );

    /* Flag to init start of log */
    txc_rdch_log_buffer->hdr.len = 0;
  }

} /* txc_rdch_frame_info_send_log */

/*===========================================================================

FUNCTION TXC_RDCH_LOG_FRAME_INFO_INIT

DESCRIPTION
  This function initializes the RDCH frame info log subrecord, as well as the
  pointer to it and the entry for this frame, in the frame types log buffer.
  If necessary, the log is sent and a new one is generated.

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to frame log entry.

SIDE EFFECTS
  A RDCH frame info. log may be sent.

===========================================================================*/
rdch_frame_type  * txc_rdch_log_frame_info_init( void )
{
  rdch_frame_type  *rdch_frame_ptr = NULL;
  channels_info_type  rdch_chan_set_mask;

  /*-----------------------------------------------------------------------*/

  /* If logging is enabled. */
  if( log_status( LOG_RDCH_FRAME_INFO_C ) )
  {
    /* If we reached accumulation threshold, send it.
    the threshold = the buffer size - the maximum of byte per record in the
    worst case analysic */
    if( txc_rdch_log_buffer->hdr.len >= FRDCH_LOG_BUFFER_THRESHOLD )
    {
      txc_rdch_frame_info_send_log();
    }
    /* check if there is a need to re-initialize */
    if( txc_rdch_log_buffer->hdr.len == 0 )
    {
      txc_rdch_log_buffer->hdr.len = (word) FPOS( log_puma_rdch_information_type, reference_buf );
      txc_rdch_log_buffer->hdr.code = LOG_RDCH_FRAME_INFO_C;
      /* First frame, log time stamp. */
      (void) m1x_time_get( txc_rdch_log_buffer->hdr.ts );

      /*  Fill in beginning of record   */
      txc_rdch_log_buffer->frame_offset = txc_frame_offset;
      txc_rdch_log_buffer->num_sub_records = 0;

      /* reset all the control variables */
      txc_rdch_log_buff_offset = 0;
      txc_rdch_sub_record_ptr = NULL;

    }
    else
    {
      /* If a new sub-record is needed, i.e. the channel set has changed,
      ** check for the condition here.
      */
      /* clear the rdch mask */
      rdch_chan_set_mask.mask = 0;

      /* set the the mask based on the active RDCH channels */
      if( txc_so.rev_dcch.included )
      {
        rdch_chan_set_mask.individual_channels.dcch = TRUE;
      }

      if( txc_so.rev_fch.included )
      {
        rdch_chan_set_mask.individual_channels.fch = TRUE;
      }

      if( txtc.sch_on )
      {
        rdch_chan_set_mask.individual_channels.sch0 = TRUE;
      }

      /* PICH channel */
      rdch_chan_set_mask.individual_channels.rpich = txtc.gating_rate != CAI_GATING_RATE_NONE;

      /* start a new sub-record if there is a new channel been added or
      the SCH MUX option has been changed */
      if( rdch_chan_set_mask.mask != txc_rdch_sub_record_ptr->assigned_channels.mask )
      {
        /* We no longer have a valid sub-record. */
        txc_rdch_sub_record_ptr = NULL;
      }
      else
      {
        if( ( txtc.sch_on ) &&
            ( txc_rdch_curr_sch_mux_opt != txc_so.rev_sch_mux_option ) )
        {
          /* We no longer have a valid sub-record. */
          txc_rdch_sub_record_ptr = NULL;
        }
      }
    }

    /* Need to init a new subrecord? */
    if( txc_rdch_sub_record_ptr == NULL )
    {
      txc_rdch_sub_record_ptr =
            (rdch_sub_record_type *) &txc_rdch_log_buffer->reference_buf[txc_rdch_log_buff_offset];
      /* Increment sub-record count. */
      txc_rdch_log_buffer->num_sub_records++;
      /* reset the number of frame counter */
      txc_rdch_sub_record_ptr->num_frames = 0;

      /* reset the rdch log sub-record offset */
      txc_rdch_log_sub_record_offset = 0;

      /* reset the channel mask */
      txc_rdch_sub_record_ptr->assigned_channels.mask = 0;

      /********Initialize the channel assignment and RC for each RDCHs ************/

      /* check for PICH */
      txc_rdch_sub_record_ptr->assigned_channels.individual_channels.rpich
                         = txtc.gating_rate != CAI_GATING_RATE_NONE;

      /* check for FCH */
      if( txc_so.rev_fch.included )
      {
        txc_rdch_sub_record_ptr->assigned_channels.individual_channels.fch
                                   = TRUE;
        /* include FCH RC */
        txc_rdch_sub_record_ptr->reference_buf[txc_rdch_log_sub_record_offset++]
                                   = txc_so.rev_fch.rc;
        /* include GCH 8th gating */
        txc_rdch_sub_record_ptr->reference_buf[txc_rdch_log_sub_record_offset++]
                                   = txtc.fch_8th_rate_gating;
      }
      /* check for DCCH */
      if( txc_so.rev_dcch.included )
      {
        txc_rdch_sub_record_ptr->assigned_channels.individual_channels.dcch
                                   = TRUE;
        txc_rdch_sub_record_ptr->reference_buf[txc_rdch_log_sub_record_offset++]
                                   = txc_so.rev_dcch.rc;
      }
      /* check for SCH and initialize RSCH data*/
      if( txtc.sch_on )
      {
        /* set the channel bit mask */
        txc_rdch_sub_record_ptr->assigned_channels.individual_channels.sch0
                                   = TRUE;
        rsch_frame_info = (rev_sch_information_type *)
                 &txc_rdch_sub_record_ptr->reference_buf[txc_rdch_log_sub_record_offset];

        /* RSCH RC */
        rsch_frame_info->rev_sch_rc = txtc.r_sch_rc;

        /* SCH MUX OPTION */
        if( txc_so.rev_sch_mux_option != 0 )
        {
          /* RSCH0 MUX option LSB */
          rsch_frame_info->rev_sch0_mux_option_lo = txc_so.rev_sch_mux_option & 0xFF;
          /* RSCH0 MUX option MSB */
          rsch_frame_info->rev_sch0_mux_option_hi = (txc_so.rev_sch_mux_option >> 8) & 0xFF;

          /* store the MUX option for later comparasion */
          txc_rdch_curr_sch_mux_opt = txc_so.rev_sch_mux_option;
        }
        else
        {
          /* RSCH0 MUX option MSB to zero*/
          rsch_frame_info->rev_sch0_mux_option_hi = 0;
          /* RSCH0 MUX option LSB to zero*/
          rsch_frame_info->rev_sch0_mux_option_lo = 0;
        }
        /* RSCH frame length, 20 ms, 40 ms or 80 ms. For now we are only support 20 ms frame*/
        rsch_frame_info->sch_frame_length = 0;

        /* RSCH0 number of bits */
        rsch_frame_info->sch0_num_bits = (uint8) txtc.supp_rate;

        /* number of MUX PDU per SCH frame */
        rsch_frame_info->num_mux_pdu_per_sch_frame = txtc.pot_num_mux_pdus;

        /* update the sub-record offset */
        txc_rdch_log_sub_record_offset += (int) sizeof(rev_sch_information_type);
      }

      /* Update packet length */
      txc_rdch_log_buffer->hdr.len += (word)
          (FPOS(rdch_sub_record_type,reference_buf) + txc_rdch_log_sub_record_offset); //lint !e713 !e737 FPOS
      /*Update the buffer pointer offset */
      txc_rdch_log_buff_offset += (txc_rdch_log_sub_record_offset
                                    + (FPOS(rdch_sub_record_type,reference_buf)));//lint !e713 !e737 FPOS
    }
    /* Increment frame count */
    txc_rdch_sub_record_ptr->num_frames++;

    /* Initialize the RDCH frame record pointer */
    rdch_frame_ptr = (rdch_frame_type *)
                          &txc_rdch_sub_record_ptr->reference_buf[txc_rdch_log_sub_record_offset];
  }
  else
  {
    txc_rdch_log_buffer->hdr.len = 0; /* Flag that this record has stopped */
  }
  return rdch_frame_ptr;

} /* txc_rdch_log_frame_info_init */

/*=========================================================================
FUNCTION TXC_RDCH_LOG_FRAME_INFO

DESCRIPTION
  This function populates RDCH frame information logs.

DEPENDENCIE
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void txc_rdch_log_frame_info( void )
{
  int                            index;
  int                            rdch_logframe_offset   = 0;
  rdch_frame_type                *rdch_frame_record_ptr = NULL;
  r_sch_mux_pdu_information_type *rsch_mux_pdu_info_ptr = NULL;

  /*-----------------------------------------------------------------------*/

  /* Call the initialized function to init the rdch frame pointer */
  rdch_frame_record_ptr = txc_rdch_log_frame_info_init();

  if( rdch_frame_record_ptr != NULL )
  {

    /* If FCH is assigned, then including FCH format */
    if( txc_rdch_sub_record_ptr->assigned_channels.individual_channels.fch )
    {
      rdch_frame_record_ptr->reference_buf[rdch_logframe_offset++] = (byte)txtc.rev_mux;
    }
    /* If DCCH is assigned, then including DCCH format */
    if( txc_rdch_sub_record_ptr->assigned_channels.individual_channels.dcch )
    {
      /* See structure definition for explaination of reference_buf */
      rdch_frame_record_ptr->reference_buf[rdch_logframe_offset++] =
         txtc.dcch_frame.frame_cat;  //lint !e661 Possible access of out-of-bounds pointer
    }
    /* If SCH is assigned, then including SCH format */
    if( txc_rdch_sub_record_ptr->assigned_channels.individual_channels.sch0 )
    {
      boolean   sch_format_xmit = FALSE;

      #ifdef FEATURE_IS2000_REL_A
      if (((( txc_get_pri_so() == CAI_SO_PPP_PKT_DATA_3G ) ||
            ( txc_get_sec_so() == CAI_SO_PPP_PKT_DATA_3G )) &&
           ( txc_rev_fr_block.num_frames > SCH_BUF_START))
          || ((( txc_get_pri_so() == CAI_SO_TDSO ) ||
               ( txc_get_pri_so() == CAI_SO_FULL_TDSO ) ||
               ( txc_get_sec_so() == CAI_SO_TDSO ) ||
               ( txc_get_sec_so() == CAI_SO_FULL_TDSO )) &&
               ( txc_rev_fr_block.num_frames > 0))
         )
      #else
      if ((( txc_get_pri_so() == CAI_SO_PPP_PKT_DATA_3G ) &&
           ( txc_rev_fr_block.num_frames > SCH_BUF_START) )
          || ((( txc_get_pri_so() == CAI_SO_TDSO ) ||
               ( txc_get_pri_so() == CAI_SO_FULL_TDSO )) &&
               ( txc_rev_fr_block.num_frames > 0))
         )
      #endif /* FEATURE_IS2000_REL_A */
      {
        sch_format_xmit = TRUE;
      }
      /* Including the SCH frame format */
      /* See structure definition for explaination of reference_buf */
      rdch_frame_record_ptr->reference_buf[rdch_logframe_offset++]
                               = sch_format_xmit;  //lint !e661 !e662 Possible access of out-of-bounds pointer
      /* if the SCH_FORMAT indicated that this frame was transmitted,
      include NUM_MUX_PDU_PER_SCH_FRAME instance of SR_ID in the record*/
      if (sch_format_xmit)
      {
        /* Including FSCH MUX PDUs for each FSCH frame */
        for (index = 0; index < rsch_frame_info->num_mux_pdu_per_sch_frame; index++)
        {
          /* intitialize the FSCH MUX PDU pointer */
          /* See structure definition for explaination of reference_buf */
          rsch_mux_pdu_info_ptr = (r_sch_mux_pdu_information_type *)
                        &rdch_frame_record_ptr->reference_buf[rdch_logframe_offset++];   //lint !e661 !e662 Possible access of out-of-bounds pointer

          rsch_mux_pdu_info_ptr->sr_id = txc_rev_fr_block.sr_id; //lint !e661 !e662 Possible access of out-of-bounds pointer
        }
      }
    }
    /* Updated the total lenght of the record header */
    txc_rdch_log_buffer->hdr.len += (word) rdch_logframe_offset;
    /* update the offset variables */
    txc_rdch_log_sub_record_offset += rdch_logframe_offset;
    txc_rdch_log_buff_offset += rdch_logframe_offset;
  }
} /* txc_rdch_log_frame_info */

#else /* FEATURE_IS2000_REL_A */
/*===========================================================================

FUNCTION TXC_FRAME_TYPES_SEND_LOG

DESCRIPTION
  Terminate and send the accumulated reverse frame types log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A rev frame types log may be sent.

===========================================================================*/
void txc_frame_types_send_log( void )
{
  /*-----------------------------------------------------------------------*/

  if( txc_log_frm_types_buf.hdr.len != 0 )
  {
    /* Will send log if enabled. */
    log_submit(&txc_log_frm_types_buf);

    /* Flag to init start of log */
    txc_log_frm_types_buf.hdr.len = 0;
  }

} /* txc_frame_types_send_log */

/*===========================================================================

FUNCTION TXC_FRAME_TYPES_LOG_INIT

DESCRIPTION
  This function initializes the frame types log subrecord, as well as the
  pointer to it, in the frame types log buffer.  If necessary, the log is
  sent and a new one is generated.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A rev frame types log may be sent.

===========================================================================*/
static boolean txc_sch_log_frame_type_data = FALSE;

mux_log_rev_frame_types_frm_type * txc_log_frame_type_frm_init( void )
{
  mux_log_rev_frame_types_frm_type *frm_ptr = NULL;
  mux_log_ded_rev_chan_set_type chan_set_mask;

  /*-----------------------------------------------------------------------*/

  /* If logging is enabled. */
  if( log_status( LOG_REV_FRAME_TYPES_C ) )
  {
    /* If we reached accumulation threshold, send it. */
    if( txc_log_frm_types_buf.hdr.len >= MUX_LOG_REV_FRAME_TYPES_BUF_THRESHOLD )
    {
      txc_frame_types_send_log();
    }

    if( txc_log_frm_types_buf.hdr.len == 0 ) /* Need to re-init */
    {
      txc_log_frm_types_buf.hdr.len = FPOS(mux_log_rev_frame_types_type, var_len_buf);

      txc_log_frm_types_buf.hdr.code = LOG_REV_FRAME_TYPES_C;

      /* First frame, log time stamp. */
      (void) m1x_time_get(txc_log_frm_types_buf.hdr.ts);

      /*------------------------------
        Fill in beginning of record
      ------------------------------*/
      /* Frame offset in 1.25 ms units */
      txc_log_frm_types_buf.frame_offset = txc_frame_offset;
      txc_log_frm_types_buf.so           = txc_get_pri_so();

      /* FFPC Primary Channel */
      if( txc_so.rev_fch.included )
      {
        txc_log_frm_types_buf.rev_ch_rc.r_fch_rc = txc_so.rev_fch.rc;
      }
      else
      {
        txc_log_frm_types_buf.rev_ch_rc.r_fch_rc = 0;
      }

      if( txc_so.rev_dcch.included )
      {
        txc_log_frm_types_buf.rev_ch_rc.r_dcch_rc = txc_so.rev_dcch.rc;
      }
      else
      {
        txc_log_frm_types_buf.rev_ch_rc.r_dcch_rc = 0;
      }

      if( txtc.sch_on )
      {
        txc_log_frm_types_buf.rev_ch_rc.r_sch0_rc = txtc.r_sch_rc;
      }
      else
      {
        txc_log_frm_types_buf.rev_ch_rc.r_sch0_rc = 0;
      }

      txc_log_frm_types_buf.rev_ch_rc.r_sch1_rc = 0; /* not supported */

      /* We no longer have a valid sub-record. */
      txc_log_frm_types_buf.sr_cnt = 0;
      txc_log_frm_types_sr_ptr     = NULL;

    }
    else
    {
      /* If a new subrecord is needed, i.e. the channel set has changed,
      ** check for the condition here.
      */
      chan_set_mask.mask            = 0;
      chan_set_mask.chan_set.r_fch  = FALSE;
      chan_set_mask.chan_set.r_dcch = FALSE;

      if( txc_so.rev_dcch.included )
      {
        chan_set_mask.chan_set.r_dcch = TRUE;
      }

      if( txc_so.rev_fch.included )
      {
        chan_set_mask.chan_set.r_fch  = TRUE;
      }

      chan_set_mask.chan_set.r_sch0 =
               (txc_sch_log_frame_type_data) ? TRUE : FALSE;

      if( chan_set_mask.mask != txc_log_frm_types_sr_ptr->ch_set_mask.mask )
      {
        /* We no longer have a valid sub-record. */
        txc_log_frm_types_sr_ptr = NULL;
      }

      if( txtc.sch_on )
      {
        txc_log_frm_types_buf.rev_ch_rc.r_sch0_rc = txtc.r_sch_rc;
      }
    }

    /* Need to init a new subrecord? */
    if( txc_log_frm_types_sr_ptr == NULL )
    {
      /* Init sub-record pointer by finding the offset into the variable length buf.
      ** TXC_FRM_TYPES_LOG_OFFSET is calculated from the length of the log packet.
      */
      txc_log_frm_types_sr_ptr =
        (mux_log_rev_frame_types_subrecord_type *)
          &txc_log_frm_types_buf.var_len_buf.buf_ref[TXC_FRM_TYPES_LOG_OFFSET];

      /* Increment sub-record count. */
      txc_log_frm_types_buf.sr_cnt++;

      /* Init all bits in mask, and len_per_frame */
      txc_log_frm_types_sr_ptr->ch_set_mask.mask = 0;
      txc_log_frm_types_sr_ptr->len_per_frame    = 0;

      txc_log_frm_types_sr_ptr->ch_set_mask.chan_set.r_dcch = FALSE;
      txc_log_frm_types_sr_ptr->ch_set_mask.chan_set.r_fch  = FALSE;

      if( txc_so.rev_dcch.included )
      {
        txc_log_frm_types_sr_ptr->ch_set_mask.chan_set.r_dcch = TRUE;

        /* Increase length per frame value. */
        txc_log_frm_types_sr_ptr->len_per_frame +=
          sizeof(mux_log_rev_frame_types_frm_type);
      }

      if( txc_so.rev_fch.included )
      {
        txc_log_frm_types_sr_ptr->ch_set_mask.chan_set.r_fch  = TRUE;

        /* Increase length per frame value. */
        txc_log_frm_types_sr_ptr->len_per_frame +=
          sizeof(mux_log_rev_frame_types_frm_type);
      }

      if( txc_sch_log_frame_type_data )
      {
        /* R-SCH0 is active */
        txc_log_frm_types_sr_ptr->ch_set_mask.chan_set.r_sch0 = 1;

        /* Increase length per frame value. */
        txc_log_frm_types_sr_ptr->len_per_frame +=
          sizeof(mux_log_rev_frame_types_frm_type);

      }
      /* Init frame count */
      txc_log_frm_types_sr_ptr->frame_cnt = 0;

      /* Update packet length */
      txc_log_frm_types_buf.hdr.len +=
        FPOS(mux_log_rev_frame_types_subrecord_type, var_len_buf);

    }

    /* Increment frame count */
    txc_log_frm_types_sr_ptr->frame_cnt++;

    /* Init frame pointer by finding the offset into the variable length buf.
    ** TXC_FRM_TYPES_LOG_OFFSET is calculated from the length of the log.
    */
    frm_ptr = (mux_log_rev_frame_types_frm_type *)
        &txc_log_frm_types_buf.var_len_buf.buf_ref[TXC_FRM_TYPES_LOG_OFFSET];
  }
  else /* Log code not enabled */
  {
    txc_log_frm_types_buf.hdr.len = 0; /* Flag that this record has stopped */
  }
  /* reset the flag for the last TX frame of the call */
  txc_sch_log_frame_type_data = txtc.sch_on;
  return frm_ptr;

} /* txc_log_frame_type_frm_init */

/*=========================================================================
FUNCTION TXC_LOG_FRAME_TYPE

DESCRIPTION
  This function populates frame type logs.

DEPENDENCIE
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void txc_log_frame_type( void )
{
  mux_log_rev_frame_types_frm_type *frm_ptr;

  /*-----------------------------------------------------------------------*/

  frm_ptr = txc_log_frame_type_frm_init();

  if( frm_ptr != NULL )
  {
    /* Start recording per channel data */
    if( txc_log_frm_types_sr_ptr->ch_set_mask.chan_set.r_fch )
    {
      frm_ptr->r_fch.rate = curr_fch_frame_rate;
      frm_ptr->r_fch.type = curr_fch_sig_type;

      frm_ptr++; /* Increase sizeof 1 frame entry */
    }

    if( txc_log_frm_types_sr_ptr->ch_set_mask.chan_set.r_sch0 )
    {
      frm_ptr->r_sch0 = curr_sch0_frame_rate;

      frm_ptr++; /* Increase sizeof 1 frame entry */
    }

    if( txc_log_frm_types_sr_ptr->ch_set_mask.chan_set.r_dcch )
    {
      frm_ptr->r_dcch = txtc.dcch_frame.frame_cat;

      frm_ptr++; /* Increase sizeof 1 frame entry */
    }

    /* TODO: Add other channels here! */

    /* end per channel data */

    /* Update packet length */
    txc_log_frm_types_buf.hdr.len += txc_log_frm_types_sr_ptr->len_per_frame;

  } /* If logging enabled */
} /* txc_log_frame_type */

#endif /* FEATURE_IS2000_REL_A */

/*===========================================================================
FUNCTION       TXC_CALC_R_SCH_THROTTLE

DESCRIPTION    This figures out the R-SCH throttling parameters.

DEPENDENCIES   None

SIDE EFFECTS   None

RETURN VALUE   None
===========================================================================*/
void txc_calc_r_sch_throttle( void )
{
  boolean             rxc_sch_data_is_valid;
  dec_rc_type         sch_rc;
  dec_sch_rate_type   dec_sch_rate;
  boolean             double_size;

  /*-----------------------------------------------------------------------*/

  if( txtc.sch_on == TRUE )
  {
    /*-----------------------------------------------------------------------
      The number of PDU's to build is analogous to the "X".  So 1X has one
      PDU, 2X has two PDU's.  The key is that it assumes single-size PDU's.
    -----------------------------------------------------------------------*/
    txtc.pot_num_mux_pdus = 1 << ((uint16) txtc.supp_rate - (uint16) ENC_SCH_FULL_RATE);

    /* for 32X, we want to limit the number of potential PDU as 16X case */
    if( txtc.supp_rate == ENC_SCH_32X_RATE )
    {
     txtc.pot_num_mux_pdus = txtc.pot_num_mux_pdus >> 1;
    }
    #ifdef FEATURE_IS2000_REL_A
    if( ( txtc.double_sized == TRUE ) ||
        ( ( txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5 ) &&
          ( txtc.supp_rate > ENC_SCH_FULL_RATE ) ) )
    #else
    if( txtc.double_sized == TRUE )
    #endif /* FEATURE_IS2000_REL_A */
    {
      /*-----------------------------------------------------------------------
        If using double-sized PDU's, then the number of PDU's is split
        in half.
      -----------------------------------------------------------------------*/
      txtc.pot_num_mux_pdus = txtc.pot_num_mux_pdus >> 1;
    }

    /*-----------------------------------------------------------------------
      Don't throttle for TDSO.  Do this by setting the ok number of pdu's
      equal to the pot(ential) number of mux pdus.
    -----------------------------------------------------------------------*/
    if( ( txc_get_pri_so() == CAI_SO_TDSO ) ||
        ( txc_get_pri_so() == CAI_SO_FULL_TDSO ) )
    {
      txtc.ok_num_mux_pdus  = txtc.pot_num_mux_pdus;
    }
    else
    {
      rxc_sch_data_is_valid = rxc_get_sch_info(&sch_rc,
                                           &dec_sch_rate,
                                           &double_size);
      #ifdef FEATURE_DATA_MM
      txtc.ok_num_mux_pdus  = ds707_scrm_max_allowable_sch_rate(
                                                     rxc_sch_data_is_valid,
                                                     sch_rc,
                                                     dec_sch_rate,
                                                     double_size,
                                                     rxc_sup_dec_int_enable);
      #else
      txtc.ok_num_mux_pdus  = ds_scrm_max_allowable_sch_rate(
                                                     rxc_sch_data_is_valid,
                                                     sch_rc,
                                                     dec_sch_rate,
                                                     double_size,
                                                     rxc_sup_dec_int_enable);
      #endif /* FEATURE_DATA_MM */
    }

    txtc.ok_num_mux_pdus = MIN(txtc.pot_num_mux_pdus, txtc.ok_num_mux_pdus);

    if (txtc.ok_num_mux_pdus < txtc.pot_num_mux_pdus)
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "R-SCH self-throttle pdus: %d -> %d",
        txtc.pot_num_mux_pdus,
        txtc.ok_num_mux_pdus );
    }
    else
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "R-SCH not throttled pdus: %d",
        txtc.pot_num_mux_pdus );
    }
  }
  else /* no supps assigned or DTX */
  {
    txtc.pot_num_mux_pdus = 0;
    txtc.ok_num_mux_pdus  = 0;
  }
} /* txc_calc_r_sch_throttle */

/*===========================================================================
FUNCTION     TXC_LOG_RLP3

DESCRIPTION  This function logs RLP 3 frames ready for xmit.

DEPENDENCIES Assumes frames are already built.  Also assumes log mask is
             turned on.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void txc_log_rlp3
(
  dsrlp_rev_frame_block_type *rev_fr_block_ptr   /* RLP frames to be tx'ed */
)
{
  dsrlp_rev_frame_type *rev_fr_ptr;            /* Ptr to 1 RLP tx frame */
  ds_log_rlp_pkt_type  *rlp_frame_log_ptr;

  /* Ptr to log buffer */
  #ifdef FEATURE_RLP_API_VERSION_1
  ds_log_rlp_frames_type *log_tx_rlp_frm_buf = dsrlp_get_log_tx_rlp_frm_buf();
  #else
  ds_log_rlp_frames_type *log_tx_rlp_frm_buf = &dsrlp_log_tx_rlp_frm_buf;
  #endif /* FEATURE_RLP_API_VERSION_1 */

  word              loop_cntr;
  byte              i;
  word              log_sch_frame_bits;
  word              log_sch_phy_chan_rate;
  word              chan_rate;
  #ifdef FEATURE_IS2000_REL_A
  word txc_rlp_skip_log_count = 0;
  boolean           rlp_begin_new_subrecord = TRUE;
  #endif /* FEATURE_IS2000_REL_A */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( ( rev_fr_block_ptr == NULL ) ||
      ( log_tx_rlp_frm_buf == NULL ) )
  {
    /* One of our pointer is NULL. Don't log this time around. */
    M1X_MSG( MUX, LEGACY_ERROR,
      "Null RLP pointer, no log generated.");
    return;
  }

  /* Generalize for MUX_PDU != 3 or 1 */
  if( txtc.supp_rate == ENC_SCH_FULL_RATE )
  {
    if( txtc.r_sch_rc == CAI_RC_4 )
    {
      log_sch_phy_chan_rate = 144;
      #ifdef FEATURE_IS2000_REL_A
      if( txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5 )
      {
        /* (total number of bytes for MUX PDU5 + 1(mux header)) * 8 bits/byte) - 6 bits( 6 bits header) */
        log_sch_frame_bits = ((txc_muxpdu5_rs2_maxsize[txtc.supp_rate ][txtc.supp_turbo]) + 1)*8 - 6;//lint !e734 Loss of precision (assignment) (19 bits to 16 bits)
      }
      else
      #endif /* FEATURE_IS2000_REL_A */
      {
        log_sch_frame_bits = 266;
      }
    }
    else
    {
      log_sch_phy_chan_rate = 96;
      #ifdef FEATURE_IS2000_REL_A
      if( txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5 )
      {
        /* (total number of bytes for MUX PDU5 + 1(mux header)) * 8 bits/byte) - 6 bits( 6 bits header) */
        log_sch_frame_bits = ((txc_muxpdu5_rs1_maxsize[txtc.supp_rate ][txtc.supp_turbo]) + 1)*8 - 6;//lint !e734 Loss of precision (assignment) (19 bits to 16 bits)
      }
      else
      #endif /* FEATURE_IS2000_REL_A */
      {
        log_sch_frame_bits = 171;
      }
    }
  }
  else
  {
    /* since in MUX PDU5, we don't have the concept of double sided, so txtc.double_sized is
     * always set to FALSE, we only handle MUX PDU5 in the else part
     */
    if( txtc.double_sized == TRUE )
    {
      /*-------------------------------------------------------------------
        For double sized PDU a data block contains 538 bits if it is RS2
        and 346 bits if it is RS1.
       --------------------------------------------------------------------*/
      if( txtc.r_sch_rc == CAI_RC_4 )
      {
        log_sch_frame_bits = 538;
      }
      else
      {
        log_sch_frame_bits = 346;
      }
    }
    else
    {
      /*------------------------------------------------------------------
        For single sized PDU a data block contains 266 bits if it is RS2
        and 170 bits if it is RS1.
       --------------------------------------------------------------------*/
      if( txtc.r_sch_rc == CAI_RC_4 )
      {
        #ifdef FEATURE_IS2000_REL_A
        if( txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5 )
        {
          /* (total number of bytes for MUX PDU5 + 1(mux header)) * 8 bits/byte) - 6 bits( 6 bits header) */
          log_sch_frame_bits = ((txc_muxpdu5_rs2_maxsize[txtc.supp_rate ][txtc.supp_turbo]) + 1)*8 - 6;//lint !e734 Loss of precision (assignment) (19 bits to 16 bits)
        }
        else
        #endif /* FEATURE_IS2000_REL_A */
        {
          log_sch_frame_bits = 266; /* (560 - 6*2 (header) - 160) / 2  */
        }
      }
      else
      {
        #ifdef FEATURE_IS2000_REL_A
        if( txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5 )
        {
          /* (total number of bytes for MUX PDU5 + 1(mux header)) * 8 bits/byte) - 6 bits( 6 bits header) */
          log_sch_frame_bits = ((txc_muxpdu5_rs1_maxsize[txtc.supp_rate ][txtc.supp_turbo]) + 1)*8 - 6;//lint !e734 Loss of precision (assignment) (19 bits to 16 bits)
        }
        else
        #endif /* FEATURE_IS2000_REL_A */
        {
          log_sch_frame_bits = 170;
        }
      }
    }

    if( txtc.r_sch_rc == CAI_RC_4 )
    {
      log_sch_phy_chan_rate = (1 << ((uint16) txtc.supp_rate - (uint16) ENC_SCH_FULL_RATE)) * 144;
    }
    else
    {
      log_sch_phy_chan_rate = (1 << ((uint16) txtc.supp_rate - (uint16) ENC_SCH_FULL_RATE)) * 96;
    }

  }
  /*-----------------------------------------------------------------------
    The zero'th element is the FCH.  pot_num_mux_pdus is the number of
    rlp frames sent on SCH, so add 1 to include FCH.
  -----------------------------------------------------------------------*/
  loop_cntr = txtc.pot_num_mux_pdus + SCH_BUF_START;
  for( i = 0; i < loop_cntr; i++ )
  {
    #ifdef FEATURE_IS2000_REL_A
    /* in case of P3 or VP2, we should not log FCH when it carries voice only */
    if (((i == FCH_BUF) && ((!txc_so.rev_fch.included) ||
         ((!DS_ISDATA (txc_get_pri_so())) && (sec_fch_num_bits == 0))))
        || ((!txc_so.rev_dcch.included) && (i == DCCH_BUF) ) )
    {
      txc_rlp_skip_log_count = txc_rlp_skip_log_count + 1;
      continue;
    }
    #else /* FEATURE_IS2000_REL_A */
    /* in case of P3 or VP2, we should not log FCH when it carries voice only */
    if ( ((!txc_so.rev_fch.included) && (i == FCH_BUF)) ||
         ((!txc_so.rev_dcch.included) && (i == DCCH_BUF) ) )
    {
      continue;
    }
    #endif /* FEATURE_IS2000_REL_A */
    rlp_frame_log_ptr = dsrlp_log_tx_init_frame();
    if( rlp_frame_log_ptr == NULL )
    {
      break;
    }
    else
    {
      /*-------------------------------------------------------------------
        Access the frame immediately.  Use it whenever possible, to be
        generic across FCH & SCH logging.
      -------------------------------------------------------------------*/
      #ifdef FEATURE_RLP_API_VERSION_1
      ds_log_rlp_phy_channel_enum_type temp_phy_chan =
                                            dsrlp_get_rlp_logging_log_id(i);

      if( temp_phy_chan != DS_LOG_RLP_INVALID )
      {
        rlp_frame_log_ptr->mux_chan.phy_chan = ((byte)temp_phy_chan & 0x0F);
      }
      else
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "Invalid RLP log phy_chan (%d)",
          (uint32)temp_phy_chan );
        rlp_frame_log_ptr->mux_chan.phy_chan = 0;
      }
      #else
      ds_log_rlp_phy_channel_enum_type temp_phy_chan =
                                            dsrlp_log_id_table[i];

      rlp_frame_log_ptr->mux_chan.phy_chan = ((byte)temp_phy_chan & 0x0F);
      #endif /* FEATURE_RLP_API_VERSION_1 */

      rev_fr_ptr = &(rev_fr_block_ptr->rev_arr[i]);
      log_tx_rlp_frm_buf->service_ID = rev_fr_block_ptr->sr_id;
      rlp_frame_log_ptr->mux_chan.mux_pdu = (byte) rev_fr_ptr->mux_pdu_type;

      /*-------------------------------------------------------------------
        Messy part.  phy chan rate has to be calc'd separately for FCH &
        SCH.
      -------------------------------------------------------------------*/
      if( i < SCH_BUF_START )
      {
        #ifdef FEATURE_IS2000_REL_A
        if( rlp_begin_new_subrecord )
        #endif /* FEATURE_IS2000_REL_A */
        {
          if( rlp_frame_log_ptr == &log_tx_rlp_frm_buf->var_len_buf.pkt[0] )
          {
            /* This is the existing frame, not the "next" frame. */
            rlp_frame_log_ptr->time_rate.time_delta = 0;
          }
          else
          {
            rlp_frame_log_ptr->time_rate.time_delta = 1;
          }
          #ifdef FEATURE_IS2000_REL_A
          rlp_begin_new_subrecord = FALSE;
          #endif /* FEATURE_IS2000_REL_A */
        }
        #ifdef FEATURE_IS2000_REL_A
        else
        {
          /* if both DCCH and FCH are included, and when the starting the
          new log, both must have the same time as the time stamp in the header*/

          rlp_frame_log_ptr->time_rate.time_delta = 0;
        }
        #endif /* FEATURE_IS2000_REL_A */

        /*-----------------------------------------------------------------
         Get the physical channel rate for the given dsrlp rate.
        -----------------------------------------------------------------*/
        if( i == DCCH_BUF )
        {
          chan_rate = (txc_get_rev_link_rate_set( txc_so.rev_dcch.rc ) == ENC_RATE_14400) ? 144 : 96;
        }
        else
        {
          chan_rate = (txc_get_rev_link_rate_set( txc_so.rev_fch.rc ) == ENC_RATE_14400) ? 144 : 96;
        }
        if( rev_fr_block_ptr->rev_arr->srvc_type == DSRLP_PRIMARY_SRVC )
        {
          switch( rev_fr_ptr->fr_rate )
          {
             /* Full Rate */
            case DSRLP_RATE_1:
              break;

             /* Half Rate */
            case DSRLP_RATE_1_2:
              chan_rate = chan_rate >> 1;
              break;

             /* Quater Rate */
            case DSRLP_RATE_1_4:
              chan_rate = chan_rate >> 2;
              break;

               /* Eighth Rate */
            case DSRLP_RATE_1_8:
              chan_rate = chan_rate >> 3;
              break;

             /* Zero Rate */
            case DSRLP_RATE_BLANK:
              chan_rate = 0;
              break;

            default:
              M1X_MSG( MUX, LEGACY_ERROR,
                "Invalid Primary category %d",
                rev_fr_ptr->fr_rate);
              break;
          }
        }
        else if( rev_fr_block_ptr->rev_arr->srvc_type == DSRLP_SECONDARY_SRVC )
        {
          switch( rev_fr_ptr->fr_rate )
          {
            /* Full Rate */
            case DSRLP_RATE_1:
            case DSRLP_RATE_7_8:
            case DSRLP_RATE_3_4:
            case DSRLP_RATE_1_2:
              break;

             /* Half Rate */
            case DSRLP_RATE_7_16:
            case DSRLP_RATE_3_8:
            case DSRLP_RATE_1_4:
              chan_rate = chan_rate >> 1;
              break;

             /* Quarter Rate */
            case DSRLP_RATE_3_16:
            case DSRLP_RATE_1_8:
              chan_rate = chan_rate >> 2;
              break;

             /* Eighth Rate */
            case DSRLP_RATE_1_16:
              chan_rate = chan_rate >> 3;
              break;

             /* Zero Rate */
            case DSRLP_RATE_BLANK:
              chan_rate = 0;
              break;

            default:
              M1X_MSG( MUX, LEGACY_ERROR,
                "Invalid Secondary category %d",
                rev_fr_ptr->fr_rate);
              break;
          }
        }

        else
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "Invalid Service Category %d",
            rev_fr_block_ptr->rev_arr->srvc_type);
        }
        rlp_frame_log_ptr->time_rate.phy_chan_rate = chan_rate;
        rlp_frame_log_ptr->frame_len = ((rev_fr_ptr->fr_rate == DSRLP_RATE_BLANK)?
                                         0:rlp_to_mux_table[rev_fr_ptr->fr_rate]
                                         [rev_fr_block_ptr->rev_arr->srvc_type]
                                         [TXC_UPPER_BOUNDS_CHECK(rev_fr_ptr->mux_pdu_type,
                                                           DSRLP_MUX_PDU_3)]);
        rlp_frame_log_ptr->payload_len.head = ((rev_fr_ptr->fr_rate == DSRLP_RATE_BLANK)?
                                                0:(rlp_frame_log_ptr->frame_len + 7) / 8);
      }
      else
      {
        #ifdef FEATURE_IS2000_REL_A
        /*-----------------------------------------------------------------
          All else is SCH.  Delta is always same as FCH.
        -----------------------------------------------------------------*/
        /* check if there is no RLP log for FCH or DCCH */
        if( txc_rlp_skip_log_count == 2 )
        {
          if( i == ( DCCH_BUF + 1 ) )
          {
            if( rlp_frame_log_ptr == &log_tx_rlp_frm_buf->var_len_buf.pkt[0] )
            {
              /* same time as the header */
              rlp_frame_log_ptr->time_rate.time_delta    = 0;
            }
            else
            {
              /* derive the time from the previous frame */
              rlp_frame_log_ptr->time_rate.time_delta    = 1;
            }
          }
          else
          {
            /* same time as the previous SCH log */
            rlp_frame_log_ptr->time_rate.time_delta    = 0;
          }
        }
        else
        #endif /* FEATURE_IS2000_REL_A */
        {
          /* same time as the FCH or DCCH rlp log record */
          rlp_frame_log_ptr->time_rate.time_delta = 0;
        }
        rlp_frame_log_ptr->time_rate.phy_chan_rate = log_sch_phy_chan_rate;
        rlp_frame_log_ptr->frame_len               = log_sch_frame_bits;
        rlp_frame_log_ptr->payload_len.head        = DSRLP_SCH_PAYLOAD_LOG_LEN;
      }

      rlp_frame_log_ptr->payload_len.tail = 0; /*           No tail!     */

      if( ( i < rev_fr_block_ptr->num_frames )        &&
          ( rev_fr_ptr->fr_item_ptr != NULL )         &&
          ( rlp_frame_log_ptr->payload_len.head > 0 ) &&
          ( rlp_frame_log_ptr->payload_len.head <= DS_LOG_MAX_RLP_PAYLOAD_LEN ) )
      {
        log_tx_rlp_frm_buf->hdr.len += rlp_frame_log_ptr->payload_len.head;

        /* Copy frame data as head payload. */
        memscpy((void*) rlp_frame_log_ptr->payload,
                rlp_frame_log_ptr->payload_len.head,
                rev_fr_ptr->fr_item_ptr->data_ptr,
                rlp_frame_log_ptr->payload_len.head);
      }
      else
      {
        rlp_frame_log_ptr->frame_len        = 0;
        rlp_frame_log_ptr->payload_len.head = 0;
        rlp_frame_log_ptr->payload_len.tail = 0;
      }
    }
  } /* for */
} /* txc_log_rlp3 */

/*===========================================================================
FUNCTION     TXC_LOG_FDCH_RLP3

DESCRIPTION  This function logs fundicated RLP 3 frames ready for xmit.

DEPENDENCIES Assumes frames are already built.  Also assumes log mask is
             turned on.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void txc_log_fdch_rlp3
(
  dsrlp_rev_frame_block_type *rev_fr_block_ptr   /* RLP frames to be tx'ed */
)
{
  dsrlp_rev_frame_type *rev_fr_ptr;               /* Ptr to 1 RLP tx frame */
  ds_log_rlp_pkt_type *rlp_frame_log_ptr;

  /* Ptr to log buffer */
  #ifdef FEATURE_RLP_API_VERSION_1
  ds_log_rlp_frames_type *log_tx_rlp_frm_buf = dsrlp_get_log_tx_rlp_frm_buf();
  #else
  ds_log_rlp_frames_type *log_tx_rlp_frm_buf = &dsrlp_log_tx_rlp_frm_buf;
  #endif /* FEATURE_RLP_API_VERSION_1 */

  word              loop_cntr;
  byte              i;
  word              chan_rate;
  #ifdef FEATURE_IS2000_REL_A
  word txc_rlp_skip_log_count = 0;
  boolean           rlp_begin_new_subrecord = TRUE;
  #endif /* FEATURE_IS2000_REL_A */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( ( rev_fr_block_ptr == NULL ) ||
      ( log_tx_rlp_frm_buf == NULL ) )
  {
    /* One of our pointer is NULL. Don't log this time around. */
    M1X_MSG( MUX, LEGACY_ERROR,
      "Null RLP pointer, no log generated." );
    return;
  }

  /*-----------------------------------------------------------------------
    FCH only
  -----------------------------------------------------------------------*/
  loop_cntr = SCH_BUF_START;

  for( i = 0; i < loop_cntr; i++ )
  {
    #ifdef FEATURE_IS2000_REL_A
    /* in case of P3 or VP2, we should not log FCH when it carries voice only */
    if (((i == FCH_BUF) && ((!txc_so.rev_fch.included) ||
         ((!DS_ISDATA (txc_get_pri_so())) && (sec_fch_num_bits == 0))))
        || ((!txc_so.rev_dcch.included) && (i == DCCH_BUF) ) )
    {
      txc_rlp_skip_log_count = txc_rlp_skip_log_count + 1;
      continue;
    }
    #else /* FEATURE_IS2000_REL_A */
    /* in case of P3 or VP2, we should not log FCH when it carries voice only */
    if ( ((!txc_so.rev_fch.included) && (i == FCH_BUF)) ||
         ((!txc_so.rev_dcch.included) && (i == DCCH_BUF) ) )
    {
      continue;
    }
    #endif /* FEATURE_IS2000_REL_A */
    rlp_frame_log_ptr = dsrlp_log_tx_init_frame();
    if (rlp_frame_log_ptr == NULL)
    {
      break;
    }
    else
    {
      /*-------------------------------------------------------------------
        Access the frame immediately.  Use it whenever possible, to be
        generic across FCH & SCH logging.
      -------------------------------------------------------------------*/
      #ifdef FEATURE_RLP_API_VERSION_1
      ds_log_rlp_phy_channel_enum_type temp_phy_chan =
                                            dsrlp_get_rlp_logging_log_id(i);

      if( temp_phy_chan != DS_LOG_RLP_INVALID )
      {
        rlp_frame_log_ptr->mux_chan.phy_chan = ((byte)temp_phy_chan & 0x0F);
      }
      else
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "Invalid RLP log phy_chan (%d)",
          (uint32)temp_phy_chan );
        rlp_frame_log_ptr->mux_chan.phy_chan = 0;
      }
      #else
      ds_log_rlp_phy_channel_enum_type temp_phy_chan =
                                            dsrlp_log_id_table[i];

      rlp_frame_log_ptr->mux_chan.phy_chan = ((byte)temp_phy_chan & 0x0F);
      #endif /* FEATURE_RLP_API_VERSION_1 */

      rev_fr_ptr = &(rev_fr_block_ptr->rev_arr[i]);
      log_tx_rlp_frm_buf->service_ID = rev_fr_block_ptr->sr_id;
      rlp_frame_log_ptr->mux_chan.mux_pdu = (byte) rev_fr_ptr->mux_pdu_type;

      #ifdef FEATURE_IS2000_REL_A
      if( rlp_begin_new_subrecord )
      #endif /* FEATURE_IS2000_REL_A */
      {
        if (rlp_frame_log_ptr == &log_tx_rlp_frm_buf->var_len_buf.pkt[0])
        {
          /* This is the existing frame, not the "next" frame. */
          rlp_frame_log_ptr->time_rate.time_delta = 0;
        }
        else
        {
          rlp_frame_log_ptr->time_rate.time_delta = 1;
        }
        #ifdef FEATURE_IS2000_REL_A
        rlp_begin_new_subrecord = FALSE;
        #endif /* FEATURE_IS2000_REL_A */
      }
      #ifdef FEATURE_IS2000_REL_A
      else
      {
        /* if both DCCH and FCH are included, and when the starting the
        new log, both must have the same time as the time stamp in the header*/

        rlp_frame_log_ptr->time_rate.time_delta = 0;
      }
      #endif /* FEATURE_IS2000_REL_A */

      /*-----------------------------------------------------------------
       Get the physical channel rate for the given dsrlp rate.
      -----------------------------------------------------------------*/
      if( i == DCCH_BUF )
      {
        chan_rate = (txc_get_rev_link_rate_set( txc_so.rev_dcch.rc ) == ENC_RATE_14400) ? 144 : 96;
      }
      else
      {
        chan_rate = (txc_get_rev_link_rate_set( txc_so.rev_fch.rc ) == ENC_RATE_14400) ? 144 : 96;
      }

      if( rev_fr_block_ptr->rev_arr->srvc_type == DSRLP_PRIMARY_SRVC )
      {
        switch( rev_fr_ptr->fr_rate )
        {
           /* Full Rate */
          case DSRLP_RATE_1:
            break;

           /* Half Rate */
          case DSRLP_RATE_1_2:
            chan_rate = chan_rate >> 1;
            break;

           /* Quater Rate */
          case DSRLP_RATE_1_4:
            chan_rate = chan_rate >> 2;
            break;

             /* Eighth Rate */
          case DSRLP_RATE_1_8:
            chan_rate = chan_rate >> 3;
            break;

           /* Zero Rate */
          case DSRLP_RATE_BLANK:
            chan_rate = 0;
            break;

          default:
            M1X_MSG( MUX, LEGACY_ERROR,
              "Invalid Primary category %d",
              rev_fr_ptr->fr_rate );
            break;
        }
      }
      else if( rev_fr_block_ptr->rev_arr->srvc_type == DSRLP_SECONDARY_SRVC )
      {
        switch( rev_fr_ptr->fr_rate )
        {
          /* Full Rate */
          case DSRLP_RATE_1:
          case DSRLP_RATE_7_8:
          case DSRLP_RATE_3_4:
          case DSRLP_RATE_1_2:
            break;

           /* Half Rate */
          case DSRLP_RATE_7_16:
          case DSRLP_RATE_3_8:
          case DSRLP_RATE_1_4:
            chan_rate = chan_rate >> 1;
            break;

           /* Quarter Rate */
          case DSRLP_RATE_3_16:
          case DSRLP_RATE_1_8:
            chan_rate = chan_rate >> 2;
            break;

           /* Eighth Rate */
          case DSRLP_RATE_1_16:
            chan_rate = chan_rate >> 3;
            break;

           /* Zero Rate */
          case DSRLP_RATE_BLANK:
            chan_rate = 0;
            break;

          default:
            M1X_MSG( MUX, LEGACY_ERROR,
              "Invalid Secondary category %d",
              rev_fr_ptr->fr_rate );
            break;
        }
      }
      else
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "Invalid Service Category %d",
          rev_fr_block_ptr->rev_arr->srvc_type );
      }
      rlp_frame_log_ptr->time_rate.phy_chan_rate = chan_rate;
      rlp_frame_log_ptr->frame_len = ((rev_fr_ptr->fr_rate == DSRLP_RATE_BLANK)?
                                       0:rlp_to_mux_table[rev_fr_ptr->fr_rate]
                                       [rev_fr_block_ptr->rev_arr->srvc_type]
                                       [TXC_UPPER_BOUNDS_CHECK(rev_fr_ptr->mux_pdu_type,
                                                         DSRLP_MUX_PDU_3)]);

      rlp_frame_log_ptr->payload_len.head = ((rev_fr_ptr->fr_rate == DSRLP_RATE_BLANK)?
                                              0:(rlp_frame_log_ptr->frame_len + 7) / 8);

      rlp_frame_log_ptr->payload_len.tail = 0; /*           No tail!     */

      if( ( i < rev_fr_block_ptr->num_frames )        &&
          ( rev_fr_ptr->fr_item_ptr != NULL )         &&
          ( rlp_frame_log_ptr->payload_len.head > 0 ) &&
          ( rlp_frame_log_ptr->payload_len.head <= DS_LOG_MAX_RLP_PAYLOAD_LEN ) )
      {
        log_tx_rlp_frm_buf->hdr.len
                += rlp_frame_log_ptr->payload_len.head;

        /* Copy frame data as head payload. */
        memscpy( (void*) rlp_frame_log_ptr->payload,
                 rlp_frame_log_ptr->payload_len.head,
                 rev_fr_ptr->fr_item_ptr->data_ptr,
                 rlp_frame_log_ptr->payload_len.head );
      }
      else
      {
        rlp_frame_log_ptr->frame_len        = 0;
        rlp_frame_log_ptr->payload_len.head = 0;
        rlp_frame_log_ptr->payload_len.tail = 0;
      }
    }
  } /* for */
} /* txc_log_fdch_rlp3 */

/*========================================================================

FUNCTION TXC_IS_LOOPBACK_SO

DESCRIPTION This function checks the provided service option
            for the current call and returns a boolean indicating whether
            or not the SO is Loopback related.

DEPENDENCIES None

RETURN VALUE True for Loopback SOs, False otherwise.

SIDE EFFECTS None

=========================================================================*/
static boolean txc_is_loopback_so( uint16 srv_opt )
{
  boolean is_loopback = FALSE;

  /*-----------------------------------------------------------------------*/

  if( (srv_opt == CAI_SO_LOOPBACK)         ||
      (srv_opt == CAI_SO_LOOPBACK_13K)     ||
      (srv_opt == CAI_SO_LOOPBACK_SO55)    ||
      (srv_opt == CAI_SO_LOOPBACK_SO75)
    )
  {
    is_loopback = TRUE;
  }

  return (is_loopback);
} /* txc_is_loopback_so */

/*===========================================================================
FUNCTION     TXC_REV_RLP_PARAMS_RESET

DESCRIPTION  This function clears RLP-MUX params at the call end

DEPENDENCIES None

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void txc_rev_rlp_params_reset( void )
{
  /*-----------------------------------------------------------------------*/

  /* Clear all the MUX controlled params during call termination */
  txc_rev_fr_block.sr_id      = 0;
  txc_rev_fr_block.max_frames = 0;
  txc_rev_fr_block.num_frames = 0;
  txc_rev_fr_block.fr_time    = 0;

  M1X_MSG( MUX, LEGACY_HIGH,
    "Clearing RLP-MUX vars, num_frames %d",txc_rev_fr_block.num_frames );

} /* txc_rev_rlp_params_reset() */

/*===========================================================================
FUNCTION     TXC_TX_FRAME_SEND_DATA3G

DESCRIPTION  This function sends the RLP 3 frames out the SCH.

DEPENDENCIES Assumes some parameters stuff is done beforehand.  Also assumes
             the SCH is correctly config'd.  Don't call this if doing DTX.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void txc_tx_frame_proc_send_data3g
(
  dsrlp_rev_frame_block_type *rev_fr_block_ptr   /* RLP frames to be tx'ed */
)
{
  dsrlp_rev_frame_type *rev_fr_ptr;               /* Ptr to 1 RLP tx frame */

  /* Ptr to log buffer */
  #ifdef FEATURE_RLP_API_VERSION_1
  ds_log_rlp_frames_type *log_tx_rlp_frm_buf = dsrlp_get_log_tx_rlp_frm_buf();
  #else
  ds_log_rlp_frames_type *log_tx_rlp_frm_buf = &dsrlp_log_tx_rlp_frm_buf;
  #endif /* FEATURE_RLP_API_VERSION_1 */

  byte i;                                         /* loop ctr              */

  #ifndef FEATURE_MODEM_1X_UTX
  /* Total available LMEM size for CH3 output */
  uint32 txc_lmem_size = ( muxmdsp_get_enc_input_len(ENC_CHAN_3) )*4;
  #endif /* FEATURE_MODEM_1X_UTX */

  byte num_sch_spare_bits;
  word r_sch_blank_buffer_size;            /* num blank bytes out   */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( ( txtc.sch_on == TRUE ) &&
      ( rev_fr_block_ptr->num_frames > SCH_BUF_START ) )
  {
    #ifdef FEATURE_IS2000_REL_A
    if( txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5 )
    {
      switch( txtc.supp_rate )
      {
        case ENC_SCH_8TH_RATE:
        case ENC_SCH_QTR_RATE:
        case ENC_SCH_HALF_RATE:
          break;
        case ENC_SCH_FULL_RATE:
          (void) parm_inc(CAI_RSCH0_1_ID, 1);
          break;
        case ENC_SCH_2X_RATE:
          (void) parm_inc(CAI_RSCH0_2_ID, 1);
          break;
        case ENC_SCH_4X_RATE:
          (void) parm_inc(CAI_RSCH0_3_ID, 1);
          break;
        case ENC_SCH_8X_RATE:
          (void) parm_inc(CAI_RSCH0_4_ID, 1);
          break;
        case ENC_SCH_16X_RATE:
          (void) parm_inc(CAI_RSCH0_5_ID, 1);
          break;
        case ENC_SCH_32X_RATE:
          (void) parm_inc(CAI_RSCH0_6_ID, 1);
          break;
        default:
          ONEX_ERR_FATAL("Invalid rate %d",(int) txtc.supp_rate,0,0);
      }
    }
    else
    #endif /* FEATURE_IS2000_REL_A */
    {
      switch( txtc.supp_rate )
      {
        case ENC_SCH_8TH_RATE:
        case ENC_SCH_QTR_RATE:
        case ENC_SCH_HALF_RATE:
          break;
        case ENC_SCH_FULL_RATE:
          (void) parm_inc(CAI_SCH0_REV_1X_ID, 1);
          break;
        case ENC_SCH_2X_RATE:
          (void) parm_inc(CAI_SCH0_REV_2X_ID, 1);
          break;
        case ENC_SCH_4X_RATE:
          (void) parm_inc(CAI_SCH0_REV_4X_ID, 1);
          break;
        case ENC_SCH_8X_RATE:
          (void) parm_inc(CAI_SCH0_REV_8X_ID, 1);
          break;
        case ENC_SCH_16X_RATE:
          (void) parm_inc(CAI_SCH0_REV_16X_ID, 1);
          break;
        #ifdef FEATURE_IS2000_REL_A
        case ENC_SCH_32X_RATE:
          (void) parm_inc(CAI_SCH0_REV_32X_ID, 1);
          break;
        #endif /* FEATURE_IS2000_REL_A */
        default:
          ONEX_ERR_FATAL("Invalid rate %d",(int) txtc.supp_rate,0,0);
      }
    }
  }

  /*-----------------------------------------------------------------------
    Go ahead and log the data before sending it out.
  -----------------------------------------------------------------------*/
  if(log_tx_rlp_frm_buf != NULL)
  {
    if (log_status(LOG_RLP_TX_FRAMES_C))
    {
      log_tx_rlp_frm_buf->hdr.code = LOG_RLP_TX_FRAMES_C;
      txc_log_rlp3(rev_fr_block_ptr);
    }
    else if(log_status(LOG_RLP_TX_FDCH_FRAMES_C))
    {
      log_tx_rlp_frm_buf->hdr.code = LOG_RLP_TX_FDCH_FRAMES_C;
      txc_log_fdch_rlp3(rev_fr_block_ptr);
    }
  }

  /*-----------------------------------------------------------------------
    Only increment when not turbo and > 2x, as those use LTU's.  Also
    account for double/single-sized PDU's.
  -----------------------------------------------------------------------*/
  if ((!txtc.supp_turbo) &&
      (txtc.supp_rate > ENC_SCH_2X_RATE) &&
      (rev_fr_block_ptr->num_frames > SCH_BUF_START)
     )
  {
    #ifdef FEATURE_IS2000_REL_A
    if(txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5)
    {
      (void) parm_inc(CAI_RSCH0_LTU_ID, txtc.pot_num_mux_pdus);
    }
    else
    #endif /* FEATURE_IS2000_REL_A */
    {
      if ( txtc.double_sized == TRUE)
      {
        (void) parm_inc(CAI_SCH0_REV_LTU_ID, txtc.pot_num_mux_pdus);
      }
      else
      {
        (void) parm_inc(CAI_SCH0_REV_LTU_ID, txtc.pot_num_mux_pdus/2);
      }
    }
  }

  for( i=0; i < rev_fr_block_ptr->num_frames; i++)
  {
    rev_fr_ptr = &(rev_fr_block_ptr->rev_arr[i]);

    /* Assuming 1 MuxPDU fits completely in a dsm item */
    if( i >= SCH_BUF_START
        #ifndef FEATURE_MODEM_1X_UTX
        /* Check for LMEM availability to write PDU bytes */
        && ( txc_lmem_size > txc_enc_pdu_byte_count )
        #endif /* !FEATURE_MODEM_1X_UTX */
      )
    {
      if ((txtc.supp_rate == ENC_SCH_FULL_RATE)
        #ifdef FEATURE_IS2000_REL_A
        && (txtc.mux_pdu_type != CAI_MUX_PDU_TYPE_5 )
        #endif /* FEATURE_IS2000_REL_A */
         )
      {
          /*
             for 1x, need to have Mixed mode bit pre-appended
             Note that only for 1x rates we have to test for Primary and
             Secondary rates on SCH.
          */
          if (txtc.r_sch_rc == CAI_RC_4)
          {
            if (CAI_SO_PPP_PKT_DATA_3G == txc_get_pri_so())
            {
                txtc.supp_frame.data[0] = 0x00;
                b_copy( rev_fr_ptr->fr_item_ptr->data_ptr, 0,
                        txtc.supp_frame.data, 2,
                        MUX2_FULL_PRI_LEN );
            }
            else if (CAI_SO_PPP_PKT_DATA_3G == txc_get_sec_so())
            {
                /*-----------------------------------------------------------
                  For MuxPDU2 we have to add header in first 6 bits. The
                  header is comprised of:
                  EIB  |  Mixed Mode bit | Frame Mode
                   0         1                 0111
                 Hence make the first 6 bits as 0101 11xx i.e. 0x5C

                 Copy the next 262 bits of data into the transmit array
                 after 6 bits of header.

                 Since, "used field" is used later, we need to increment it
                 by 1 because adding 6 bits crosses the byte boundry.
                 Note that incrementing used field will not cross beyond the
                 DSM boundries, because max is only 33 bytes.
                -----------------------------------------------------------*/
                txtc.supp_frame.data[0] = 0x5C;
                b_copy( rev_fr_ptr->fr_item_ptr->data_ptr, 0,
                        txtc.supp_frame.data, 6,
                        MUX2_FULL_SEC_LEN );
                rev_fr_ptr->fr_item_ptr->used+=1;
            }
            else
            {
                M1X_MSG( MUX, LEGACY_ERROR,
                  "Invalid Service type for data %d",
                  txc_get_sec_so());
            }
          }
          else
          {
            if( CAI_SO_PPP_PKT_DATA_3G == txc_get_pri_so() )
            {
              txtc.supp_frame.data[0] = 0x00;
              b_copy( rev_fr_ptr->fr_item_ptr->data_ptr, 0,
                      txtc.supp_frame.data, 1,
                      MUX1_FULL_PRI_LEN );
            }
            else if( CAI_SO_PPP_PKT_DATA_3G == txc_get_sec_so() )
            {
              /*-----------------------------------------------------------
                For MuxPDU1 we have to add header in first 4 bits. The
                header is comprised of:
                 Mixed Mode bit | Traffic Type | Traffic Mode
                 1                 1             11
               Hence make the first 4 bits as 0xF0

               Copy the next 168 bits of data into the transmit array.

               Since, used field is used later, we need to increment it
               by 1 because adding 4 bits crosses the byte boundry.
              -----------------------------------------------------------*/
              txtc.supp_frame.data[0] = 0xF0;
              b_copy( rev_fr_ptr->fr_item_ptr->data_ptr, 0,
                      txtc.supp_frame.data, 4,
                      MUX1_FULL_SEC_LEN);
              rev_fr_ptr->fr_item_ptr->used+=1;
            }
            else
            {
              M1X_MSG( MUX, LEGACY_ERROR,
                 "Invalid Service type for data %d",
                 txc_get_sec_so() );
            }
          }

          enc_sch_muxpdu_data_out(
            txtc.supp_frame.data,
            rev_fr_ptr->fr_item_ptr->used
          );
      }
      else
      {
        #ifdef FEATURE_IS2000_REL_A
        if( txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5 )
        {
          if( ( rev_fr_ptr->fr_item_ptr != NULL ) &&
              ( rev_fr_ptr->fr_item_ptr->data_ptr != NULL ) )
          {
            /* if the used field is equal the max_frame_size field + 1, then
                there is only one DSM item per PDU. If there are diferent, then we
                know that there are multiple DSM items chained together*/
            if( rev_fr_ptr->fr_item_ptr->used == (rev_fr_ptr->max_frame_size + 1 ) )
            {
              enc_sch_muxpdu_data_out( rev_fr_ptr->fr_item_ptr->data_ptr,
                                       rev_fr_ptr->fr_item_ptr->used );
            }
            else  /* there could be more than 1 DSM item per DPU */
            {
              dsm_item_type *temp_current_dsm_prt;
              dsm_item_type *temp_next_dsm_prt;
              temp_current_dsm_prt = rev_fr_ptr->fr_item_ptr;

              /*--------------------------------------------------------------------------
               Add changes for adding E0 at end of the data
               Obtain Length of the RLP frame using Length Indicator.
               If (Length Indicator == 00)
                  No need to do anything.
               else if (length indicator is 01 or 10)
                  Add E0 at the end
               else
                  Should never happen. Print message and fail an ASSERT for now

               {
                   Add E0 at the end of the packet.
                   If packet does not have any more items, then grab a free SMALL DSM pkt.
                   If no free DSM packet exists, its okay, send something(its usually 0s)
               }
               Code in else path, can actually be done by existing dsm API, pushdown_tail;
               Made the length 1, though its a 4 byte word, because we are interested only
               in E0.Probably not too elegant, but saves extra definitions. If anytime in
               future you change the endianness this may be another piece to be visited
               The total data that can be sent is based on (rev_fr_ptr->max_frame_size+1).
               We add 1, because that's the header for SR_ID.
              --------------------------------------------------------------------------*/
              if( txtc.supp_turbo )
              {
                /* for turbo, there is one PDU per frame, thus use this special
                logic to insert the NULL PDU header after the last used data byte*/
                dsrlp_insert_pdu5null_hdr_turbo(temp_current_dsm_prt);
              }
              temp_next_dsm_prt = dsrlp_get_next_dsm_item_chain(temp_current_dsm_prt);
              while( temp_next_dsm_prt != NULL )
              {
                /* write the current dsm data into the encoder */
                enc_sch_muxpdu_data_out
                (
                  temp_current_dsm_prt->data_ptr,
                  temp_current_dsm_prt->used
                );

                /* update the current pointer to the next */
                temp_current_dsm_prt = temp_next_dsm_prt;
                /* get the next pointer */
                temp_next_dsm_prt = dsrlp_get_next_dsm_item_chain(temp_current_dsm_prt);
              }

              /* the last dsm item will be write into the encoder by the following logic */
              /* initialize the data pointer and the number of byte */
              if (temp_current_dsm_prt->data_ptr != NULL)
              {
                enc_sch_muxpdu_data_out
                (
                  temp_current_dsm_prt->data_ptr,
                  temp_current_dsm_prt->used
                );
              }
              else
              {
                ASSERT (temp_current_dsm_prt->data_ptr != NULL);
              }
            }
          }
          else
          {
            ASSERT (rev_fr_ptr->fr_item_ptr != NULL);
            ASSERT (rev_fr_ptr->fr_item_ptr->data_ptr != NULL);
          }
        }
        else
        #endif /* FEATURE_IS2000_REL_A */
        {
          if (
              (rev_fr_ptr->fr_item_ptr != NULL) &&
              (rev_fr_ptr->fr_item_ptr->data_ptr != NULL)
             )
          {
            enc_sch_muxpdu_data_out(
              rev_fr_ptr->fr_item_ptr->data_ptr,
              rev_fr_ptr->fr_item_ptr->used
            );
          }
          else
          {
            ASSERT (rev_fr_ptr->fr_item_ptr != NULL);
            ASSERT (rev_fr_ptr->fr_item_ptr->data_ptr != NULL);
          }
        }
      }
    }
    if( rev_fr_ptr->free_dsm_pkt == TRUE)
    {
      /* this will free all the DSM items in the chain */
      dsm_free_packet( &(rev_fr_ptr->fr_item_ptr));
    }
  } /* for loop */

 /* If there is room left, fill the R-SCH with blank PDUs */
  if( (txtc.sch_on == TRUE) &&
      (rev_fr_block_ptr->num_frames > SCH_BUF_START) &&
      (rev_fr_block_ptr->num_frames < txtc.pot_num_mux_pdus+SCH_BUF_START)
      #ifndef FEATURE_MODEM_1X_UTX
      /* Check for LMEM availability to write PDU bytes */
      && ( txc_lmem_size > txc_enc_pdu_byte_count )
      #endif /* !FEATURE_MODEM_1X_UTX */
    )
  {
    /*---------------------------------------------------------------------
      Note that the size is decremented by one, since the one-byte header
      is written separately.
    ---------------------------------------------------------------------*/
    if((txc_get_rev_link_rate_set(txtc.r_sch_rc)) == ENC_RATE_14400)
    {
#ifdef FEATURE_IS2000_REL_A
      if (txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5 )
      {
        r_sch_blank_buffer_size = txc_muxpdu5_rs2_maxsize[txtc.supp_rate ][txtc.supp_turbo];
      }
      else
      {
#endif /* FEATURE_IS2000_REL_A */
        r_sch_blank_buffer_size =( (txtc.double_sized) ? (68 - 1) : (34 - 1));
#ifdef FEATURE_IS2000_REL_A
      }
#endif /* FEATURE_IS2000_REL_A */
    }
    else if ((txc_get_rev_link_rate_set(txtc.r_sch_rc)) == ENC_RATE_9600)
    {
#ifdef FEATURE_IS2000_REL_A
      if (txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5 )
      {
        r_sch_blank_buffer_size = txc_muxpdu5_rs1_maxsize[txtc.supp_rate ][txtc.supp_turbo];
      }
      else
      {
#endif /* FEATURE_IS2000_REL_A */
        r_sch_blank_buffer_size =( (txtc.double_sized) ? (44 - 1) : (22 - 1));
#ifdef FEATURE_IS2000_REL_A
      }
#endif /* FEATURE_IS2000_REL_A */
    }
    else
    {
      /* Default to Rate Set 1 blank buffer size */
      r_sch_blank_buffer_size =( (txtc.double_sized) ? (44 - 1) : (22 - 1));

      M1X_MSG( MUX, LEGACY_ERROR,
        "Unknown Rate Set: Fix it" );
      // ASSERT(0); Use for debug
    }

    /*---------------------------------------------------------------------
      The first blank PDU has the 0x0E header.  Later PDU's may or may not
      have this header, depending on if Turbo or convolutional encoding
    ---------------------------------------------------------------------*/
#ifdef FEATURE_IS2000_REL_A
    /* for MUXPDU 5, as for the current implementation turbo will have only 1 MUX PDU,
     * thus RLP will take care all of the NULL filling. For PDU5 and turbo code,
     * the following logic should not run at all. In the future, if there are more than
     * one PDU5 per frame for turbo, then this logic is needed.
     */
    if (txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5)
    {
      /* write the NULL PDU header first */
      enc_sch_muxpdu_data_out((void *)r_sch_blank_pdu_hdr,4);
      #ifndef FEATURE_MODEM_1X_UTX
      /* Check for LMEM availability to write PDU bytes */
      if ( txc_lmem_size > txc_enc_pdu_byte_count )
      #endif /* FEATURE_MODEM_1X_UTX */
      enc_sch_muxpdu_data_out((void *)r_sch_blank_buffer,r_sch_blank_buffer_size - 3);
    }
    else
#endif /* FEATURE_IS2000_REL_A */
    {
      enc_sch_muxpdu_data_out(
        (void *)(r_sch_blank_pdu_hdr),
        4
      );
      #ifndef FEATURE_MODEM_1X_UTX
      /* Check for LMEM availability to write PDU bytes */
      if ( txc_lmem_size > txc_enc_pdu_byte_count )
      #endif /* FEATURE_MODEM_1X_UTX */
      enc_sch_muxpdu_data_out(
        (void *)r_sch_blank_buffer,
        (word)(r_sch_blank_buffer_size - 3)
      );
    }
    /*---------------------------------------------------------------------
      Have already written one blank mux pdu.  So make sure don't write
      too many more (notice "i = 1" in for... conditional).  In the for...
      loop, write the one-byte header, and then zero out rest of pdu.
    ---------------------------------------------------------------------*/
    for (i = 1;
         i < (txtc.pot_num_mux_pdus + SCH_BUF_START - rev_fr_block_ptr->num_frames);
         i++)
    {
      #ifndef FEATURE_MODEM_1X_UTX
      /* Stop writing Headers of blank PDU's if there is no LMEM left */
      if ( txc_lmem_size <= txc_enc_pdu_byte_count )
        break;
      #endif /* !FEATURE_MODEM_1X_UTX */

      if (txtc.supp_turbo == TRUE)
      {
        /*-----------------------------------------------------------------
          Only the first PDU has a 0x0E header.  Rest are all zero's
          For this current implementation, this logic only apply for PDU3
        -----------------------------------------------------------------*/
        enc_sch_muxpdu_data_out(
           (void *)r_sch_blank_buffer,
           4
         );
      }
      else
      {
        /*-----------------------------------------------------------------
          Every PDU has the 0x0E header.
        -----------------------------------------------------------------*/
        enc_sch_muxpdu_data_out(
          (void *)(r_sch_blank_pdu_hdr),
          4
        );
      }

      #ifndef FEATURE_MODEM_1X_UTX
      /* Stop writing Body of blank PDU's if there is no LMEM left */
      if ( txc_lmem_size <= txc_enc_pdu_byte_count )
        break;
      #endif /* !FEATURE_MODEM_1X_UTX */

#ifdef FEATURE_IS2000_REL_A
      if (txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5)
      {
        enc_sch_muxpdu_data_out((void *)r_sch_blank_buffer,r_sch_blank_buffer_size - 3);
      }
      else
#endif /* FEATURE_IS2000_REL_A */
      {
        enc_sch_muxpdu_data_out(
          (void *)r_sch_blank_buffer,
          (word) (r_sch_blank_buffer_size -3)
        );
      }
    } /* for */
    (void) parm_xtra_inc(CAI_XTRA_SCH0_REV_BLANK_PDU_ID,
                  (txtc.pot_num_mux_pdus + SCH_BUF_START - rev_fr_block_ptr->num_frames)
                 );
  } /* if blank frames need to be added */

  /*-----------------------------------------------------------------------
    The IS2000 standard specifies that the "spare bits", remaining after
    the maximum number of PDUs for the given rate have been used up to form
    the SDU, shall be filled with 0's. Since turbo coded frames do not have
    LTU CRCs, this number is a function of the supplemental channel data
    rate and the code type. Convolutional = 0, Turbo = 1.
  -----------------------------------------------------------------------*/
  if ( (txtc.sch_on == TRUE) && (rev_fr_block_ptr->num_frames > SCH_BUF_START)
      #ifndef FEATURE_MODEM_1X_UTX
      /* Check for LMEM availability to write spare bits */
      && ( txc_lmem_size > txc_enc_pdu_byte_count )
      #endif /* !FEATURE_MODEM_1X_UTX */
     )
  {
    #ifdef FEATURE_IS2000_REL_A
    if (txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5 )
    {
      num_sch_spare_bits = (txc_muxpdu5_nsch_sbits[ txtc.supp_rate ][ txtc.supp_turbo ])/8;
    }
    else
    #endif /* FEATURE_IS2000_REL_A */
    {
      num_sch_spare_bits = (txc_num_sch_spare_bits[ txtc.supp_rate ][ txtc.supp_turbo ])/8;
    }

    enc_sch_muxpdu_data_out(
      txc_sch_spare_bits,
      num_sch_spare_bits
    );
    #ifdef FEATURE_IS2000_REL_A
    if( ( !txtc.double_sized ) && ( txtc.mux_pdu_type != CAI_MUX_PDU_TYPE_5 ) )
    #else
    if( !txtc.double_sized )
    #endif /* FEATURE_IS2000_REL_A */
    {
      #ifndef FEATURE_MODEM_1X_UTX
      /* Check for LMEM availability to write spare bits */
      if ( txc_lmem_size > txc_enc_pdu_byte_count )
      #endif /* FEATURE_MODEM_1X_UTX */
      enc_sch_muxpdu_data_out( txc_sch_spare_bits, 2 );
    }
  }

  rev_fr_block_ptr->num_frames = 0;

  /* Send the R-SCH for this data frame */
  enc_sch_write_data_done();

} /* txc_tx_frame_proc_send_data3g */

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================
FUNCTION     TXC_TX_FRAME_PROC_BUILD_DATA3G

DESCRIPTION  This function builds the RLP 3 frames.

DEPENDENCIES None.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void txc_tx_frame_proc_build_data3g
(
  dsrlp_rev_frame_block_type *rev_fr_block_ptr   /* RLP frames to be tx'ed */
)
{
  rev_fr_block_ptr->num_frames = 0;

  /*-----------------------------------------------------------------------*/

  /*-------------------------------------------------------------------------
   filling in frame information for frame building. (FCH/DCCH).
   Ask RLP to build frames to transmit using the new RLP interfaces.
  -------------------------------------------------------------------------*/
  rev_fr_block_ptr->sr_id = DSRLP_FIXED_SR_ID_INDEX;
  rev_fr_block_ptr->max_frames = SCH_BUF_START;

  /*-------------------------------------------------------------------------
     See if throttling is required on R-SCH
  -------------------------------------------------------------------------*/
  if (txc_sch_changed > 0)
  {
    txc_sch_changed--;
    if (txc_sch_changed == 0)
    {
      txc_calc_r_sch_throttle();
    }
  }

  #ifdef FEATURE_IS2000_REL_A
  /* for MUXPDU5, for simple the implementation, in the case turbo, we use only 1 MUXPDU */
  if ((txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5) && (txtc.supp_turbo))
  {
    if (txtc.ok_num_mux_pdus > 0)
    {
      txtc.ok_num_mux_pdus = 1;
      txtc.pot_num_mux_pdus = 1;
    }
  }
  #endif /* FEATURE_IS2000_REL_A */
  /*-------------------------------------------------------------------------
    DTX expired, should not send data on SCH
  -------------------------------------------------------------------------*/
  if (txtc.txc_dtx_off)
  {
    txtc.txc_dtx_off = FALSE;
    if (txtc.sch_on)
    {
      /* turns off sch_on to prevent the data preparation on SCH */
      txtc.sch_on = FALSE;
    }
  }

  /*-------------------------------------------------------------------------
    FCH, DCCH frame information already processed.
  -------------------------------------------------------------------------*/

  /* do max power protection
  */
  txc_max_pwr_prot();

  /*-------------------------------------------------------------------------
    filling in frame information for frame building. (SCH)
  -------------------------------------------------------------------------*/

  txc_tx_process_sch (rev_fr_block_ptr);

  dsrlp_build_tx_frames(rev_fr_block_ptr);

  /*-------------------------------------------------------------------------
     DTX duration checking is activated
  -------------------------------------------------------------------------*/
  if (txtc.dtx_duration_included )
  {
    if ((txtc.pot_num_mux_pdus > 0) &&
        (rev_fr_block_ptr->num_frames <= SCH_BUF_START))
    {
      /*---------------------------------------------------------------------
        all SCH RLP frames are empty in the data block
      ---------------------------------------------------------------------*/
      if (txtc.dtx_duration > 0)
      {
        txtc.dtx_duration --;
      }

      if (txtc.dtx_duration == 0)
      {
        /*-------------------------------------------------------------------
          DTX expired, release SCH by sending SCRM to BS
          and disable the DTX duration checking.
        -------------------------------------------------------------------*/
        M1X_MSG( MUX, LEGACY_HIGH,
          "DTX expired, releasing SCH");
        mcc_set_rsch_dtx_sig();   /* Ask MC to release the SCH */
        txtc.dtx_duration_included = FALSE;
        txtc.dtx_duration_org = 0;
        txtc.txc_dtx_off = TRUE;
      }
    }
    else if (rev_fr_block_ptr->num_frames > SCH_BUF_START)
    {
      /*---------------------------------------------------------------------
        The data TX resumed before the DTX duration expired.
        Reset dtx_duration to the dtx_duration_org
      ---------------------------------------------------------------------*/
      txtc.dtx_duration = txtc.dtx_duration_org;
    }
  }

  /**********************************************************************************************/
  /* Post Processing */
  /**********************************************************************************************/

  /* process FCH */
  txc_tx_post_rlp_fch_process (rev_fr_block_ptr);

  /* process DCCH */
  txc_tx_post_rlp_dcch_process (rev_fr_block_ptr);

} /* txc_tx_frame_proc_build_data3g() */

/*===========================================================================
FUNCTION     TXC_MAX_PWR_PROT

DESCRIPTION  This function does processing for Maximum Power Protection.

DEPENDENCIES None.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void txc_max_pwr_prot( void )
{
  int16  gain_req_full_rate;

  /*-----------------------------------------------------------------------*/

  txc_max_pwr_limited_dtx_sch = FALSE;

  if( !txtc.sch_on || !txc_sch_max_pwr_protection_enabled )
  {
    return;
  }

  if( txc_so.rev_fch.included && txc_so.rev_dcch.included )
  {
    gain_req_full_rate = -txc_fch_dcch_sch_gain_tab [ENC_FULL_RATE]
                                         [DCCH_ACTIVE_SCH_ACTIVE].pch_adj_gain;
    if( tx_extra_gain <= gain_req_full_rate )
    {
      txc_max_pwr_limited_dtx_sch = TRUE;
      gain_req_full_rate = -txc_fch_dcch_sch_gain_tab [ENC_FULL_RATE]
                                         [DCCH_ACTIVE_SCH_DTX].pch_adj_gain;
    }
  }
  else if( txc_so.rev_dcch.included )
  {
    gain_req_full_rate =
          -txc_dcch_sch_gain_tab [DCCH_ACTIVE_SCH_ACTIVE].pch_adj_gain;

    if( tx_extra_gain <= gain_req_full_rate )
    {
      txc_max_pwr_limited_dtx_sch = TRUE;
    }
  }
  else if( txc_so.rev_fch.included )
  {
    gain_req_full_rate = -txc_sch_pch_adj_gain_tab [ENC_FULL_RATE];
    if( tx_extra_gain <= gain_req_full_rate )
    {
      txc_max_pwr_limited_dtx_sch = TRUE;
      gain_req_full_rate = -txc_dtx_pch_adj_gain_tab [ENC_FULL_RATE];
    }
  }
} /* txc_max_pwr_prot */

/*===========================================================================
FUNCTION     TXC_TX_PROCCESS_SCH

DESCRIPTION  This function sets up RLP information needed to build a R-SCH frame.

DEPENDENCIES None.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void txc_tx_process_sch
(
 dsrlp_rev_frame_block_type *rev_fr_block_ptr
)
{
  dsrlp_rev_frame_type *rev_fr_ptr;             /* Ptr to 1 RLP tx frame */
  int i;

  /*-----------------------------------------------------------------------*/

  enc_rate_set_type sch_rate_set = ENC_RATE_9600;

  if (txtc.sch_on && (!txc_max_pwr_limited_dtx_sch))
  {
    rev_fr_block_ptr->max_frames += txtc.ok_num_mux_pdus;
    /*---------------------------------------------------------------------
      Time to find out SCH rate sets
    ---------------------------------------------------------------------*/
    sch_rate_set = txc_get_rev_link_rate_set(txtc.r_sch_rc);

    for( i = 0; (i < txtc.pot_num_mux_pdus) &&
                (txtc.pot_num_mux_pdus <= (RLP_MAX_REV_PDU - SCH_BUF_START)); i++)
    {
        /*-------------------------------------------------------------------
        Loop through all potential frames, as this will make logging
        easier.  But only pass up to RLP the "ok" number of frames.
      -------------------------------------------------------------------*/
      rev_fr_ptr = &(rev_fr_block_ptr->rev_arr[i+SCH_BUF_START]);

      rev_fr_ptr->fr_rate_mask = DSRLP_RATE_1_MASK;

      if (sch_rate_set == ENC_RATE_9600)
      {
#ifdef FEATURE_IS2000_REL_A
        if (txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5)
        {
          rev_fr_ptr->max_frame_size  = txc_muxpdu5_rs1_maxsize[txtc.supp_rate ][txtc.supp_turbo];
        }
        else
        {
#endif /* FEATURE_IS2000_REL_A */

          rev_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_1;
          rev_fr_ptr->max_frame_size  = ( (txtc.double_sized) ? 43 : 21);

          /* Added check for MUX PDU 1 and 2 1x SCH so RLP will put RLP frame type bit
          ** in correct position, not applicable to MUX PDU 5.
          */
          if (txc_get_sec_so() == CAI_SO_PPP_PKT_DATA_3G)
          {
            rev_fr_ptr->srvc_type = DSRLP_SECONDARY_SRVC;
          }
          else
          {
            rev_fr_ptr->srvc_type = DSRLP_PRIMARY_SRVC;
          }
#ifdef FEATURE_IS2000_REL_A
        }
#endif /* #ifdef FEATURE_IS2000_REL_A */

      }
      else if(sch_rate_set == ENC_RATE_14400)
      {
#ifdef FEATURE_IS2000_REL_A
        if (txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5)
        {
          rev_fr_ptr->max_frame_size  = txc_muxpdu5_rs2_maxsize[txtc.supp_rate ][txtc.supp_turbo];
        }
        else
        {
#endif /* FEATURE_IS2000_REL_A */

          rev_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_2;
          rev_fr_ptr->max_frame_size  = ( (txtc.double_sized) ? 67 : 33);

          /* Added check for MUX PDU 1 and 2 1x SCH so RLP will put RLP frame type bit
          ** in correct position, not applicable to MUX PDU 5.
          */
          if (txc_get_sec_so() == CAI_SO_PPP_PKT_DATA_3G)
          {
            rev_fr_ptr->srvc_type = DSRLP_SECONDARY_SRVC;
          }
          else
          {
            rev_fr_ptr->srvc_type = DSRLP_PRIMARY_SRVC;
          }
#ifdef FEATURE_IS2000_REL_A
        }
#endif /* #ifdef FEATURE_IS2000_REL_A */

      }
      else
      {
         M1X_MSG( MUX, LEGACY_ERROR,
           "received wrong Rate Set information");
          // ASSERT(0); Use only for debugging
      }

       /*
       Sarithay : Do the check for 1x . Talk to Nagesh/Harsha about the functionality.
       */
#ifdef FEATURE_IS2000_REL_A
      if (txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5)
      {
        rev_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_5;
      }
      else
      {
#endif /* FEATURE_IS2000_REL_A */
        if (txtc.supp_rate == ENC_SCH_FULL_RATE)
        {
           #ifdef FEATURE_RLP_API_VERSION_1
           rev_fr_ptr->max_frame_size  =
                           dsrlp_get_frame_len(DSRLP_RATE_1,
                                               rev_fr_ptr->srvc_type,
                                               rev_fr_ptr->mux_pdu_type);
           #else
           rev_fr_ptr->max_frame_size  =
                           dsrlp_len_table[DSRLP_RATE_1]
                                          [rev_fr_ptr->srvc_type]
                                          [rev_fr_ptr->mux_pdu_type];
           #endif /* FEATURE_RLP_API_VERSION_1 */
        }
        else
        {
          rev_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_3;

          //rev_fr_ptr->srvc_type    = DSRLP_GENERIC_SRVC;

        }
#ifdef FEATURE_IS2000_REL_A
      }
#endif /* FEATURE_IS2000_REL_A */
      if (i < txtc.ok_num_mux_pdus)
      {
        rev_fr_ptr->fr_cat_mask  = DSRLP_CAT_DATA_MASK;
      }
      else
      {
       /*-----------------------------------------------------------------
         Reverse throttling taking place... don't build these
         frames.
       -----------------------------------------------------------------*/
       rev_fr_ptr->fr_cat_mask = DSRLP_CAT_BLANK_MASK;
      }
    } /* for */
  } /* sch is on */
} /* txc_tx_process_sch */

#else /* FEATURE_IS2000_REL_A_SVD */
/*===========================================================================
FUNCTION     TXC_TX_FRAME_PROC_BUILD_DATA3G

DESCRIPTION  This function builds the RLP 3 frames.

DEPENDENCIES None.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void txc_tx_frame_proc_build_data3g
(
  dsrlp_rev_frame_block_type *txc_rev_fr_block   /* RLP frames to be tx'ed */
)
{
  dsrlp_rev_frame_type *rev_fr_ptr;             /* Ptr to 1 RLP tx frame */
  byte i;                                       /* loop ctr              */
  int16  gain_req_full_rate;

  // Remember if we need to send msg on DCCH
  boolean msg_on_dcch = txc_so.rev_dcch.included && txc_tx.active_msg &&
                        (txtc.cur_dsch == TXC_REV_DCCH);
  boolean msg_on_fch = txc_so.rev_fch.included && txc_tx.active_msg &&
                        (txtc.cur_dsch == TXC_REV_FCH);
  boolean control_hold = txtc.gating_rate != CAI_GATING_RATE_NONE;

  /*
     For generality program mux_pdu to be PDU1. Note that we use only one
     variable for building mux_pdu for fch/dcch/sch, since they are used
     at mutually-xclusive times.
  */
  dsrlp_mux_pdu_enum_type mux_pdu = DSRLP_MUX_PDU_1;

  /*
   The rate sets for different channels. Note that, we need separate variables
   for each channel because, we support Mixed rate sets.
   Initialize them to RS1 as these are default values.
  */
  enc_rate_set_type fundich_rate_set = ENC_RATE_9600;
  enc_rate_set_type sch_rate_set     = ENC_RATE_9600;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  txc_rev_fr_block->num_frames = 0;

  /*-------------------------------------------------------------------------
    Read the rate sets of all the channels. We use them only if the
    channels are in use.
    if FCH is included then fundicated channel rate set is that of FCH
    else, it is for DCCH.
  -------------------------------------------------------------------------*/
  if( txc_so.rev_fch.included )
  {
    fundich_rate_set = txc_get_rev_link_rate_set(txc_so.rev_fch.rc);
  }
  else
  {
    fundich_rate_set = txc_get_rev_link_rate_set(txc_so.rev_dcch.rc);
  }

  /*-------------------------------------------------------------------------
     See if throttling is required on R-SCH
  -------------------------------------------------------------------------*/
  if (txc_sch_changed > 0)
  {
    txc_sch_changed--;
    if (txc_sch_changed == 0)
    {
      txc_calc_r_sch_throttle();
    }
  }
#ifdef FEATURE_IS2000_REL_A
  /* for MUXPDU5, for simple the implementation, in the case turbo, we use only 1 MUXPDU */
  if ((txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5) && (txtc.supp_turbo))
  {
    if (txtc.ok_num_mux_pdus > 0)
    {
      txtc.ok_num_mux_pdus = 1;
      txtc.pot_num_mux_pdus = 1;
    }
  }
#endif /* FEATURE_IS2000_REL_A */
  /*-------------------------------------------------------------------------
    DTX expired, should not send data on SCH
  -------------------------------------------------------------------------*/
  if (txtc.txc_dtx_off)
  {
    txtc.txc_dtx_off = FALSE;
    if (txtc.sch_on)
    {
      /* turns off sch_on to prevent the data preparation on SCH */
      txtc.sch_on = FALSE;
    }
  }

  /*-------------------------------------------------------------------------
   filling in frame information for frame building. (FCH/DCCH).
   Ask RLP to build frames to transmit using the new RLP interfaces.
  -------------------------------------------------------------------------*/
  /* NOTE: Although the variable below is named txc_rev_fr_blk->sr_id, it is
  actually the RLP instance (or index in rscb_array) for this service.
  Currently, only one RLP instance is supported. In the future, when multiple
  RLP instances are supported, change this to use the correct one*/
  txc_rev_fr_block->sr_id = DSRLP_FIXED_SR_ID_INDEX;
  txc_rev_fr_block->max_frames = SCH_BUF_START;
  rev_fr_ptr = &(txc_rev_fr_block->rev_arr[FCH_BUF]);

  /*-------------------------------------------------------------------------
    Now read the type of mux PDU to be used for the FCH/DCCH.
  -------------------------------------------------------------------------*/
  if(fundich_rate_set == ENC_RATE_9600)
  {
    mux_pdu = DSRLP_MUX_PDU_1;
    M1X_MSG( MUX, LEGACY_LOW,
      "Making RS1 FCH Frame");
  }
  else if (fundich_rate_set == ENC_RATE_14400)
  {
    mux_pdu = DSRLP_MUX_PDU_2;
    M1X_MSG( MUX, LEGACY_LOW,
      "Making RS2 FCH Frame");
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Unknown Rate set: Fix it");
    //ASSERT(0); Use for debugging
  }


  if ( txc_so.rev_fch.included )
  {
    txc_max_pwr_limited_dtx_sch = FALSE;

    if (txtc.sch_on && txc_sch_max_pwr_protection_enabled)
    {
      gain_req_full_rate = -txc_sch_pch_adj_gain_tab [ENC_FULL_RATE];
      if (tx_extra_gain <= gain_req_full_rate)
      {
        txc_max_pwr_limited_dtx_sch = TRUE;
        gain_req_full_rate = -txc_dtx_pch_adj_gain_tab [ENC_FULL_RATE];
      }
    }

    rev_fr_ptr->srvc_type    = DSRLP_PRIMARY_SRVC;

    /*-------------------------------------------------------------------
      We are writing FCH Frame parameters. So use the FCH rate set
      information
    -------------------------------------------------------------------*/
    if (fundich_rate_set == ENC_RATE_9600)
    {
       rev_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_1;
       M1X_MSG( MUX, LEGACY_LOW,
         "Making RS1 FCH Frame");
    }
    else if (fundich_rate_set == ENC_RATE_14400)
    {
        rev_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_2;
        M1X_MSG( MUX, LEGACY_LOW,
          "Making RS2 FCH Frame");
    }
    else
    {
       M1X_MSG( MUX, LEGACY_ERROR,
         "Unknown Rate Set TYPE. Please correct");
       // ASSERT(0); Use for debugging
    }


    rev_fr_ptr->fr_cat_mask  = DSRLP_CAT_DATA_MASK |
                               DSRLP_CAT_IDLE_MASK |
                               DSRLP_CAT_CTL_MASK;

    if (
         !msg_on_fch
       )
    {
      #ifdef FEATURE_RLP_API_VERSION_1
      rev_fr_ptr->max_frame_size = dsrlp_get_frame_len(DSRLP_RATE_1,
                                                       DSRLP_PRIMARY_SRVC,
                                                       mux_pdu);
      #else
      rev_fr_ptr->max_frame_size = dsrlp_len_table[ DSRLP_RATE_1]
                                                  [ DSRLP_PRIMARY_SRVC]
                                                  [ mux_pdu];
      #endif /* FEATURE_RLP_API_VERSION_1 */

      rev_fr_ptr->fr_rate_mask = DSRLP_RATE_1_MASK   |
                                 DSRLP_RATE_1_2_MASK |
                                 DSRLP_RATE_1_4_MASK |
                                 DSRLP_RATE_1_8_MASK;
    }
    else
    {
      rev_fr_ptr->fr_rate_mask  = DSRLP_RATE_BLANK_MASK;
      rev_fr_ptr->fr_cat_mask   = DSRLP_CAT_BLANK_MASK;
      rev_fr_ptr->fr_rate       = DSRLP_RATE_BLANK;
    }
  }
  else
  {
    rev_fr_ptr->fr_rate_mask  = DSRLP_RATE_BLANK_MASK;
    rev_fr_ptr->fr_cat_mask   = DSRLP_CAT_BLANK_MASK;
    rev_fr_ptr->fr_rate       = DSRLP_RATE_INACTIVE;
    txc_data_rate = prim_rate = CAI_BLANK_RATE;
  }

  // Now do DCCH
  rev_fr_ptr = &txc_rev_fr_block->rev_arr[DCCH_BUF];
  if ( txc_so.rev_dcch.included )
  {
    txc_max_pwr_limited_dtx_sch = FALSE;
    if (txtc.sch_on && txc_sch_max_pwr_protection_enabled)
    {
      gain_req_full_rate =
            -txc_dcch_sch_gain_tab [DCCH_ACTIVE_SCH_ACTIVE].pch_adj_gain;

      if (tx_extra_gain <= gain_req_full_rate)
      {
        txc_max_pwr_limited_dtx_sch = TRUE;
      }
    }

    if (fundich_rate_set == ENC_RATE_9600)
    {
      rev_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_1;
      M1X_MSG( MUX, LEGACY_LOW,
        "**Making RS1 DCCH Frame");
    }
    else if (fundich_rate_set == ENC_RATE_14400)
    {
      rev_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_2;
      M1X_MSG( MUX, LEGACY_LOW,
        "**Making RS2 DCCH Frame");
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "**Unknown Rate Set TYPE. Please correct");
    }

    #ifdef FEATURE_RLP_API_VERSION_1
    rev_fr_ptr->max_frame_size  = dsrlp_get_frame_len(DSRLP_RATE_1,
                                                      DSRLP_PRIMARY_SRVC,
                                                      rev_fr_ptr->mux_pdu_type);
    #else
    rev_fr_ptr->max_frame_size = dsrlp_len_table [ DSRLP_RATE_1]
                                                 [ DSRLP_PRIMARY_SRVC]
                                                 [ rev_fr_ptr->mux_pdu_type];
    #endif /* FEATURE_RLP_API_VERSION_1 */

    rev_fr_ptr->fr_rate_mask = DSRLP_RATE_1_MASK;
    rev_fr_ptr->srvc_type     = DSRLP_PRIMARY_SRVC;
    txc_data_rate = prim_rate = CAI_FULL_RATE;
    if ( msg_on_dcch || control_hold ) // No data allowed when in control hold
    {
      // Need to revisit when simultaneous FCH and DCCH are supported
      rev_fr_ptr->fr_rate_mask  = DSRLP_RATE_BLANK_MASK;
      rev_fr_ptr->fr_cat_mask   = DSRLP_CAT_BLANK_MASK;
      rev_fr_ptr->fr_rate       = DSRLP_RATE_BLANK;
      txc_data_rate = prim_rate = CAI_BLANK_RATE;
    }
    else
    {
      rev_fr_ptr->fr_cat_mask  = DSRLP_CAT_DATA_MASK |
                                 DSRLP_CAT_IDLE_MASK |
                                 DSRLP_CAT_CTL_MASK;
    }
  }
  else
  {
    rev_fr_ptr->fr_rate_mask  = DSRLP_RATE_BLANK_MASK;
    rev_fr_ptr->fr_cat_mask   = DSRLP_CAT_BLANK_MASK;
    rev_fr_ptr->fr_rate       = DSRLP_RATE_INACTIVE;
    txc_data_rate = prim_rate = CAI_BLANK_RATE;
  }

  /*-------------------------------------------------------------------------
    filling in frame information for frame building. (SCH)
  -------------------------------------------------------------------------*/
  if (txtc.sch_on && (!txc_max_pwr_limited_dtx_sch))
    {
      txc_rev_fr_block->max_frames += txtc.ok_num_mux_pdus;
      /*---------------------------------------------------------------------
        Time to find out SCH rate sets
      ---------------------------------------------------------------------*/
      sch_rate_set = txc_get_rev_link_rate_set(txtc.r_sch_rc);

      for( i = 0; i < txtc.pot_num_mux_pdus; i++)
      {
        /*-------------------------------------------------------------------
          Loop through all potential frames, as this will make logging
          easier.  But only pass up to RLP the "ok" number of frames.
        -------------------------------------------------------------------*/
        rev_fr_ptr = &(txc_rev_fr_block->rev_arr[i+SCH_BUF_START]);


        /*-------------------------------------------------------------------
          Check the rate set of the SCH (note this can be different from
          FCH). Update the muxPDU type. Note that this will be useful
          only for 1x case or MUXPDU 5 case. For Non-1x and Non-MUXPDU 5,
          we use MUXPDU3 but for 1x, we use either 1 or 2 .
        -------------------------------------------------------------------*/
        if (sch_rate_set == ENC_RATE_9600)
        {
#ifdef FEATURE_IS2000_REL_A
          if (txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5)
          {
            rev_fr_ptr->max_frame_size  = txc_muxpdu5_rs1_maxsize[txtc.supp_rate ][txtc.supp_turbo];
          }
          else
          {
#endif /* FEATURE_IS2000_REL_A */
            rev_fr_ptr->max_frame_size  = ( (txtc.double_sized) ? 43 : 21);
            mux_pdu = DSRLP_MUX_PDU_1;
            M1X_MSG( MUX, LEGACY_LOW,
              "Making RS1 SCH Frame");
#ifdef FEATURE_IS2000_REL_A
          }
#endif /* #ifdef FEATURE_IS2000_REL_A */
        }
        else if(sch_rate_set == ENC_RATE_14400)
        {
#ifdef FEATURE_IS2000_REL_A
          if (txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5)
          {
            rev_fr_ptr->max_frame_size  = txc_muxpdu5_rs2_maxsize[txtc.supp_rate ][txtc.supp_turbo];
          }
          else
          {
#endif /* FEATURE_IS2000_REL_A */
           rev_fr_ptr->max_frame_size  = ( (txtc.double_sized) ? 67 : 33);
           mux_pdu = DSRLP_MUX_PDU_2;
           M1X_MSG( MUX, LEGACY_LOW,
             "Making RS2 SCH Frame");
#ifdef FEATURE_IS2000_REL_A
          }
#endif /* #ifdef FEATURE_IS2000_REL_A */
        }
        else
        {
           M1X_MSG( MUX, LEGACY_ERROR,
             "received wrong Rate Set information");
            // ASSERT(0); Use only for debugging
        }

        rev_fr_ptr->fr_rate_mask = DSRLP_RATE_1_MASK;

        rev_fr_ptr->srvc_type    = DSRLP_GENERIC_SRVC;

#ifdef FEATURE_IS2000_REL_A
        if (txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5)
        {
          rev_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_5;
          if (i < txtc.ok_num_mux_pdus)
          {
            rev_fr_ptr->fr_cat_mask  = DSRLP_CAT_DATA_MASK;
          }
          else
          {
            /*-----------------------------------------------------------------
              Reverse throttling taking place... don't build these
              frames.
            -----------------------------------------------------------------*/
            rev_fr_ptr->fr_cat_mask = DSRLP_CAT_BLANK_MASK;
          }
        }
        else
        {
#endif /* FEATURE_IS2000_REL_A */
          /*-------------------------------------------------------------------
            Since SCH frame now, uses only PDU3, hard code it.
          -------------------------------------------------------------------*/
          rev_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_3;

          if (i < txtc.ok_num_mux_pdus)
          {
            rev_fr_ptr->fr_cat_mask  = DSRLP_CAT_DATA_MASK;
          }
          else
          {
            /*-----------------------------------------------------------------
              Reverse throttling taking place... don't build these
              frames.
            -----------------------------------------------------------------*/
            rev_fr_ptr->fr_cat_mask = DSRLP_CAT_BLANK_MASK;
          }

          if (txtc.supp_rate == ENC_SCH_FULL_RATE)
          {
            /*-----------------------------------------------------------------
              For 1x SCH, use MUX_PDU1 or MUX_PDU2, per standard
            -----------------------------------------------------------------*/
            #ifdef FEATURE_RLP_API_VERSION_1
            rev_fr_ptr->max_frame_size  =
                                dsrlp_get_frame_len(DSRLP_RATE_1,
                                                    DSRLP_PRIMARY_SRVC,
                                                    mux_pdu);
            #else
            rev_fr_ptr->max_frame_size  =
                                dsrlp_len_table[ DSRLP_RATE_1]
                                               [ DSRLP_PRIMARY_SRVC]
                                               [ mux_pdu];
            #endif /* FEATURE_RLP_API_VERSION_1 */

            rev_fr_ptr->mux_pdu_type = mux_pdu;
            rev_fr_ptr->srvc_type    = DSRLP_PRIMARY_SRVC;
          }
#ifdef FEATURE_IS2000_REL_A
        }
#endif /* FEATURE_IS2000_REL_A */
      } /* for */
  } /* sch is on */

  dsrlp_build_tx_frames(txc_rev_fr_block);

  /*-------------------------------------------------------------------------
     DTX duration checking is activated
  -------------------------------------------------------------------------*/
  if (txtc.dtx_duration_included )
  {
    if ((txtc.pot_num_mux_pdus > 0) &&
        (txc_rev_fr_block->num_frames <= SCH_BUF_START))
    {
      /*---------------------------------------------------------------------
        all SCH RLP frames are empty in the data block
      ---------------------------------------------------------------------*/
      if (txtc.dtx_duration > 0)
      {
        /* Detect the R-SCH transition from TX to DTX, then reset the
        continuous RSCH counter in the MUX statistic*/
        if (txtc.dtx_duration == txtc.dtx_duration_org)
        {
          /* second input = 0xFF will reset the corresponding parameter */
          parm_xtra_inc(CAI_XTRA_SCH0_REV_SDU_CON_DTX_ID, 0xFF);
        }
        txtc.dtx_duration --;
        /* increasing the continuous RSCH counter in the MUX statistic by 1 */
        parm_xtra_inc(CAI_XTRA_SCH0_REV_SDU_CON_DTX_ID, 1);
      }

      if (txtc.dtx_duration == 0)
      {
        /*-------------------------------------------------------------------
          DTX expired, release SCH by sending SCRM to BS
          and disable the DTX duration checking.
        -------------------------------------------------------------------*/
        M1X_MSG( MUX, LEGACY_HIGH,
          "DTX expired, DTX counter = %d frames, "
          "Releasing SCH",
          txtc.dtx_duration_org);
        mcc_set_rsch_dtx_sig();   /* Ask MC to release the SCH */
        txtc.dtx_duration_included = FALSE;
        txtc.dtx_duration_org = 0;
        txtc.txc_dtx_off = TRUE;
      }
    }
    else if (txc_rev_fr_block->num_frames > SCH_BUF_START)
    {
      /*---------------------------------------------------------------------
        The data TX resumed before the DTX duration expired.
        Reset dtx_duration to the dtx_duration_org
      ---------------------------------------------------------------------*/
      txtc.dtx_duration = txtc.dtx_duration_org;
    }
  }

  if (txc_rev_fr_block->num_frames
      && txc_so.rev_fch.included
      )
  {
    /* based on rate from RLP, set fr. rate*/
    rev_fr_ptr = &(txc_rev_fr_block->rev_arr[FCH_BUF]);
    switch ( rev_fr_ptr->fr_rate)
    {
       case DSRLP_RATE_1:
         txc_data_rate = prim_rate = CAI_FULL_RATE;
         prim_data_ptr = rev_fr_ptr->fr_item_ptr->data_ptr;
         break;

       case DSRLP_RATE_1_2:
         txc_data_rate = prim_rate = CAI_HALF_RATE;
         prim_data_ptr = rev_fr_ptr->fr_item_ptr->data_ptr;
         break;

       case DSRLP_RATE_1_4:
         txc_data_rate = prim_rate = CAI_QUARTER_RATE;
         prim_data_ptr = rev_fr_ptr->fr_item_ptr->data_ptr;
         break;

       case DSRLP_RATE_1_8:
         txc_data_rate = prim_rate = CAI_EIGHTH_RATE;
         prim_data_ptr = rev_fr_ptr->fr_item_ptr->data_ptr;
         break;

       case DSRLP_RATE_BLANK:
         if (
             msg_on_fch
             )
         {
         txc_data_rate = prim_rate = CAI_BLANK_RATE;
         }
         else
         {
           M1X_MSG( MUX, LEGACY_ERROR,
             "No Signalling, but RLP 3 Tx Blank Rate FCH" );
           txc_data_rate = prim_rate = CAI_EIGHTH_RATE;
           prim_data_ptr = (byte *) &txc_rlp_null_data;
         }
         break;

       default:
         M1X_MSG( MUX, LEGACY_ERROR,
           "Invalid Rate requested by RLP 3 TX %d",
           rev_fr_ptr->fr_rate );
         txc_data_rate = prim_rate = CAI_EIGHTH_RATE;
         prim_data_ptr = (byte *) &txc_rlp_null_data;
         break;
    }
  } /* if num_frames > 0 */
  else if ( txc_rev_fr_block->num_frames && txc_so.rev_dcch.included )
  {
    /*-----------------------------------------------------------------------
      Let's see if Data has anything to send.Use following logic to decide
      DCCH Frame_Type can be either FULL or BLANK

      Frame_Type   Idle_Frame_Bit         ACtion
      BLANK         TRUE                  Donot Send anything
      BLANK         FALSE                 Donot Send Anything
      FULL          TRUE                  Umm.. Probably good to DTX, because
                                          its anyway Idle frame. In future
                                          decide, if anything else needs to
                                          be done
      FULL          FALSE                 Send the data
    -----------------------------------------------------------------------*/
    if ( txc_rev_fr_block->rev_arr[DCCH_BUF].idle_frame )
    {
      // Force this to BLANK if we DTX.  It's used in RLP logging later.
      txc_rev_fr_block->rev_arr[DCCH_BUF].fr_rate = DSRLP_RATE_BLANK;

      prim_dcch_rate     = CAI_NULL_RATE;
      prim_dcch_data_ptr = NULL;
    }
    else
    {
//    ASSERT (txc_rev_fr_block->rev_arr[DCCH_BUF].fr_rate != DSRLP_RATE_BLANK);

      // DCCH can only be either full rate or DTX.
      if (txc_rev_fr_block->rev_arr[DCCH_BUF].fr_rate == DSRLP_RATE_BLANK)
      {
        prim_dcch_rate     = CAI_NULL_RATE;
        prim_dcch_data_ptr = NULL;
      }
      else
      {
        prim_dcch_rate     = CAI_FULL_RATE;
        prim_dcch_data_ptr = txc_rev_fr_block->rev_arr[DCCH_BUF].fr_item_ptr->data_ptr;
      }
    }
  }
  else
  {
    /*-----------------------------------------------------------------------
      Most likely here because call has ended, and RLP has been terminated
      but txc still running.  Cover this case.
    -----------------------------------------------------------------------*/
    if (msg_on_fch)
    {
      txc_data_rate = prim_rate = CAI_BLANK_RATE;
    }
    else if (msg_on_dcch)
    {
      txc_data_rate = prim_dcch_rate = CAI_NULL_RATE;
    }
    else
    {
      if (txc_so.rev_fch.included)
      {
        /* No L3/RLP msg, send 1/8 rate NULL frame on FCH
        */
        txc_data_rate = prim_rate = CAI_EIGHTH_RATE;
        prim_data_ptr = (byte *) &txc_rlp_null_data;
      }
      else
      {
        /* No L3/RLP msg, DTX DCCH
        */
        prim_dcch_rate     = CAI_NULL_RATE;
        prim_dcch_data_ptr = NULL;
      }
    }
  }
} /* txc_tx_frame_proc_build_data3g */
#endif /* FEATURE_IS2000_REL_A_SVD */

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================
FUNCTION     TXC_TX_PROCESS_FUNDICH_DATA3G

DESCRIPTION  This function populates the RLP parameters required to build a
             data frame on the fundicated channel.

DEPENDENCIES None.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void txc_tx_process_fundich_data3g
(
 word                 phy_channel,
 dsrlp_rate_enum_type max_rlp_rate,
 dsrlp_srvc_enum_type srvc_type
)
{
  dsrlp_rev_frame_type   *rev_fr_ptr;             /* Ptr to 1 RLP tx frame */
  cai_radio_config_type  chan_rc;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( ( phy_channel != MCTRANS_PHY_CHAN_FCH ) &&
      ( phy_channel != MCTRANS_PHY_CHAN_DCCH ) )
  {
    ONEX_ERR_FATAL("Illegal physical channel", 0, 0, 0);
  }

  if( phy_channel == MCTRANS_PHY_CHAN_FCH )
  {
    rev_fr_ptr = &(txc_rev_fr_block.rev_arr[FCH_BUF]);
    chan_rc = txc_so.rev_fch.rc;
  }
  else
  {
    rev_fr_ptr = &(txc_rev_fr_block.rev_arr[DCCH_BUF]);
    chan_rc = txc_so.rev_dcch.rc;
  }

  rev_fr_ptr->max_frame_rate = (uint16) max_rlp_rate;
  rev_fr_ptr->srvc_type      = srvc_type;

  if( rev_fr_ptr->max_frame_rate == (int) DSRLP_RATE_BLANK )
  {
    rev_fr_ptr->max_frame_size =  0;
    rev_fr_ptr->fr_cat_mask    = DSRLP_CAT_BLANK_MASK;
    rev_fr_ptr->data_on_ch = FALSE;
  }
  else
  {
    rev_fr_ptr->data_on_ch = TRUE;

    if( txc_get_rev_link_rate_set(chan_rc) == ENC_RATE_9600 )
    {
      rev_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_1;
    }
    else if( txc_get_rev_link_rate_set(chan_rc) == ENC_RATE_14400 )
    {
      rev_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_2;
    }

    #ifdef FEATURE_RLP_API_VERSION_1
    rev_fr_ptr->max_frame_size = dsrlp_get_frame_len(rev_fr_ptr->max_frame_rate,
                                                     rev_fr_ptr->srvc_type,
                                                     rev_fr_ptr->mux_pdu_type);
    #else
    rev_fr_ptr->max_frame_size = dsrlp_len_table[rev_fr_ptr->max_frame_rate]
                          [rev_fr_ptr->srvc_type]
                          [TXC_UPPER_BOUNDS_CHECK(rev_fr_ptr->mux_pdu_type, DSRLP_MUX_PDU_3)];
    #endif /* FEATURE_RLP_API_VERSION_1 */

    rev_fr_ptr->fr_cat_mask    = DSRLP_CAT_DATA_MASK |
                                 DSRLP_CAT_IDLE_MASK |
                                 DSRLP_CAT_CTL_MASK;
  }

  if( phy_channel == MCTRANS_PHY_CHAN_DCCH )
  {
    rev_fr_ptr->fr_rate_mask = (rev_fr_ptr->max_frame_rate == ((uint16) DSRLP_RATE_1))
                               ? DSRLP_RATE_1_MASK
                               : DSRLP_RATE_BLANK_MASK;
  }
  else
  {
    rev_fr_ptr->fr_rate_mask = (rev_fr_ptr->max_frame_rate < ((uint16) DSRLP_RATE_ERASURE_SCH))
                               ? (dsrlp_mask_table[rev_fr_ptr->max_frame_rate]
                                                 [rev_fr_ptr->srvc_type]
                                    [TXC_UPPER_BOUNDS_CHECK(rev_fr_ptr->mux_pdu_type, DSRLP_MUX_PDU_3)]) :
                                    DSRLP_RATE_ERASURE;
  }
} /* txc_tx_process_fundich_data3g */
#endif /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================
FUNCTION     TXC_TX_POST_RLP_FCH_PROCESS

DESCRIPTION  This function initializes the MUX R-FCH TX information like
             frame rate and frame buffer ptrs.

DEPENDENCIES None.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void txc_tx_post_rlp_fch_process
(
 dsrlp_rev_frame_block_type *rev_fr_block_ptr
)
{
  dsrlp_rev_frame_type *rev_fr_ptr = &(rev_fr_block_ptr->rev_arr[FCH_BUF]);

  if (rev_fr_ptr->data_on_ch)
  {
    if (rev_fr_block_ptr->num_frames > 0)
    {
      if (rev_fr_ptr->fr_rate != DSRLP_RATE_BLANK)
      {
         ASSERT (rev_fr_ptr->fr_item_ptr != NULL);
      }

      if (rev_fr_ptr->srvc_type == DSRLP_PRIMARY_SRVC)
      {
        if (rev_fr_ptr->fr_item_ptr)
        {
          /* set prim_rate, prim_data_ptr
          */
          prim_data_ptr = rev_fr_ptr->fr_item_ptr->data_ptr;
        }

        switch (rev_fr_ptr->fr_rate)
        {
          case DSRLP_RATE_1:
            txc_data_rate = prim_rate = CAI_FULL_RATE;
            break;

          case DSRLP_RATE_1_2:
            txc_data_rate = prim_rate = CAI_HALF_RATE;
            break;

          case DSRLP_RATE_1_4:
            txc_data_rate = prim_rate = CAI_QUARTER_RATE;
            break;

          case DSRLP_RATE_1_8:
            txc_data_rate = prim_rate = CAI_EIGHTH_RATE;
            break;

          case DSRLP_RATE_BLANK:
            if (rev_fr_ptr->max_frame_rate == (uint32) DSRLP_RATE_BLANK)
            {
              /* signaling msg was pending, hence RLP was BLANK'd
              */
              prim_data_ptr = NULL;
              txc_data_rate = prim_rate = CAI_BLANK_RATE;
            }
            else
            {
              /* RLP had nothing to send
              */
              M1X_MSG( MUX, LEGACY_ERROR,
                "No Signalling, but RLP 3 Tx Blank Rate FCH" );
              txc_data_rate = prim_rate = CAI_EIGHTH_RATE;
              prim_data_ptr = (byte *) &txc_rlp_null_data;
            }
            break;

         default:
            M1X_MSG( MUX, LEGACY_ERROR,
              "Invalid Rate returned by RLP 3 TX %d",
              rev_fr_ptr->fr_rate );
            txc_data_rate = prim_rate = CAI_EIGHTH_RATE;
            prim_data_ptr = (byte *) &txc_rlp_null_data;
            break;
        }
      }
#ifdef FEATURE_IS2000_REL_A_SVD
      else if (rev_fr_ptr->srvc_type == DSRLP_SECONDARY_SRVC)
      {
        /* set sec_data_rate and sec_num_bits
        */
        if ((rev_fr_ptr->fr_rate == DSRLP_RATE_BLANK) ||
            (rev_fr_ptr->fr_rate == DSRLP_RATE_INACTIVE))
        {
          sec_fch_data_ptr = NULL;
          sec_fch_num_bits = 0;
        }
        else
        {
          sec_fch_data_ptr = rev_fr_ptr->fr_item_ptr->data_ptr;
          sec_fch_num_bits = rlp_to_mux_table[ rev_fr_ptr->fr_rate ]
                [ DSRLP_SECONDARY_SRVC ]
                [TXC_UPPER_BOUNDS_CHECK(rev_fr_ptr->mux_pdu_type, DSRLP_MUX_PDU_3)];

          if ((sec_fch_num_bits == 0) || (sec_fch_data_ptr == NULL))
          {
            sec_fch_data_ptr = NULL;
            sec_fch_num_bits = 0;
            ONEX_ERR_FATAL("SVD, RLP returned MUX 0 bits",0,0,0);
          }
        }
      }
#endif /* FEATURE_IS2000_REL_A_SVD */
      else
      {
        ONEX_ERR_FATAL("%d: Unknown Traffic type on FCH",(int) rev_fr_ptr->srvc_type,0,0);
      }
    } /* if (txc_rev_fr_block->num_frames > 0) */
    else
    {
      /* RLP built nothing.
      */
      if (rev_fr_ptr->srvc_type == DSRLP_PRIMARY_SRVC)
      {
        if (rev_fr_ptr->max_frame_rate == (uint32) DSRLP_RATE_BLANK)
        {
          /* signaling msg was pending, hence RLP was BLANK'd
          */
          prim_data_ptr = NULL;
          txc_data_rate = prim_rate = CAI_BLANK_RATE;
          rev_fr_ptr->fr_rate = DSRLP_RATE_BLANK;
        }
        else
        {
          /* RLP had nothing to send
          */
          M1X_MSG( MUX, LEGACY_ERROR,
            "No Signalling, but RLP 3 Tx Blank Rate FCH" );
          txc_data_rate = prim_rate = CAI_EIGHTH_RATE;
          prim_data_ptr = (byte *) &txc_rlp_null_data;
        }
      }
#ifdef FEATURE_IS2000_REL_A_SVD
      else if (rev_fr_ptr->srvc_type == DSRLP_SECONDARY_SRVC)
      {
        sec_fch_data_ptr    = NULL;
        sec_fch_num_bits    = 0;
        rev_fr_ptr->fr_rate = DSRLP_RATE_BLANK;
      }
#endif /* FEATURE_IS2000_REL_A_SVD */
    }
  }
  else
  {
//  ASSERT((txc_rev_fr_block->num_frames == 0) &&
//         (rev_fr_ptr->fr_rate == DSRLP_RATE_BLANK));

    rev_fr_ptr->fr_rate = DSRLP_RATE_BLANK;

    if (rev_fr_ptr->srvc_type == DSRLP_PRIMARY_SRVC)
    {
      prim_data_ptr = NULL;
      prim_rate     = CAI_BLANK_RATE;
    }
#ifdef FEATURE_IS2000_REL_A_SVD
    else if (rev_fr_ptr->srvc_type == DSRLP_SECONDARY_SRVC)
    {
      sec_fch_data_ptr = NULL;
      sec_fch_num_bits = 0;
    }
#endif /* FEATURE_IS2000_REL_A_SVD */
  }
} /* txc_tx_post_rlp_fch_process */

/*===========================================================================
FUNCTION     TXC_TX_POST_RLP_DCCH_PROCESS

DESCRIPTION  This function initializes the MUX R-DCCH TX information like
             frame rate and frame buffer ptrs.

DEPENDENCIES None.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void txc_tx_post_rlp_dcch_process
(
 dsrlp_rev_frame_block_type *rev_fr_block_ptr
)
{
  dsrlp_rev_frame_type *rev_fr_ptr = &(rev_fr_block_ptr->rev_arr[DCCH_BUF]);

  if (rev_fr_ptr->data_on_ch)
  {

    if (rev_fr_block_ptr->num_frames > 0)
    {
      if (rev_fr_ptr->fr_rate != DSRLP_RATE_BLANK)
      {
         ASSERT (rev_fr_ptr->fr_item_ptr != NULL);
      }

      if (rev_fr_ptr->srvc_type == DSRLP_PRIMARY_SRVC)
      {
        if (rev_fr_ptr->idle_frame ||
            (rev_fr_ptr->fr_rate == DSRLP_RATE_BLANK))
        {
          prim_dcch_rate     = CAI_NULL_RATE;
          prim_dcch_data_ptr = NULL;

          /* Force this to BLANK if we DTX.  It's used in RLP logging later.
          */
          rev_fr_ptr->fr_rate = DSRLP_RATE_BLANK;
        }
        else if (rev_fr_ptr->fr_rate == DSRLP_RATE_1)
        {
          /* DCCH can only be either full rate or DTX.
          */
          prim_dcch_rate     = CAI_FULL_RATE;
          prim_dcch_data_ptr = rev_fr_ptr->fr_item_ptr->data_ptr;

          if (prim_dcch_data_ptr == NULL)
          {
            prim_dcch_rate = CAI_NULL_RATE;

            /* Force this to BLANK if we DTX.  It's used in RLP logging later.
            */
            rev_fr_ptr->fr_rate = DSRLP_RATE_BLANK;
            ONEX_ERR_FATAL("SVD, DCCH data ptr is NULL", 0, 0, 0);
          }
        }
        else
        {
          prim_dcch_rate      = CAI_NULL_RATE;
          prim_dcch_data_ptr  = NULL;
          rev_fr_ptr->fr_rate = DSRLP_RATE_BLANK;
          ONEX_ERR_FATAL("SVD, Invalid rate %d", (int) rev_fr_ptr->fr_rate, 0, 0);
        }
      }
#ifdef FEATURE_IS2000_REL_A_SVD
      else if (rev_fr_ptr->srvc_type == DSRLP_SECONDARY_SRVC)
      {
        if (rev_fr_ptr->idle_frame ||
            (rev_fr_ptr->fr_rate == DSRLP_RATE_BLANK))
        {
          sec_dcch_num_bits   = 0;
          sec_dcch_data_ptr   = NULL;

          /* Force this to BLANK if we DTX.  It's used in RLP logging later.
          */
          rev_fr_ptr->fr_rate = DSRLP_RATE_BLANK;
        }
        else if (rev_fr_ptr->fr_rate == DSRLP_RATE_1)
        {
          /* DCCH can only be either full rate or DTX.
          */
          sec_dcch_data_ptr = rev_fr_ptr->fr_item_ptr->data_ptr;
          sec_dcch_num_bits = rlp_to_mux_table[ rev_fr_ptr->fr_rate ]
                     [ DSRLP_SECONDARY_SRVC ]
                     [TXC_UPPER_BOUNDS_CHECK(rev_fr_ptr->mux_pdu_type, DSRLP_MUX_PDU_3)];

          if ((sec_dcch_data_ptr == NULL) || (sec_dcch_num_bits == 0))
          {
            // REMOVE AFTER DEBUG
            sec_dcch_data_ptr = NULL;
            sec_dcch_num_bits = 0;
            ONEX_ERR_FATAL("SVD, DCCH data ptr is NULL", 0, 0, 0);
          }
        }
        else
        {
          sec_dcch_data_ptr = NULL;
          sec_dcch_num_bits = 0;
          rev_fr_ptr->fr_rate = DSRLP_RATE_BLANK;
          ONEX_ERR_FATAL("SVD, Invalid rate %d", (int) rev_fr_ptr->fr_rate, 0, 0);
        }
      }
#endif /* FEATURE_IS2000_REL_A_SVD */
      else
      {
        ONEX_ERR_FATAL("%d: Unknown Traffic type on FCH",(int) rev_fr_ptr->srvc_type,0,0);
      }
    }
    else
    {
      /* rev_fr_block_ptr->num_frames == 0
      */
      if (rev_fr_ptr->srvc_type == DSRLP_PRIMARY_SRVC)
      {
        prim_dcch_rate     = CAI_NULL_RATE;
        prim_dcch_data_ptr = NULL;
      }
      #ifdef FEATURE_IS2000_REL_A_SVD
      else if (rev_fr_ptr->srvc_type == DSRLP_SECONDARY_SRVC)
      {
        sec_dcch_num_bits = 0;
        sec_dcch_data_ptr = NULL;
      }
      #endif /* FEATURE_IS2000_REL_A_SVD */

      /* Force this to BLANK if we DTX.  It's used in RLP logging later.
      */
      rev_fr_ptr->fr_rate = DSRLP_RATE_BLANK;
    }
  }
  else
  {
//  ASSERT((rev_fr_block_ptr->num_frames == 0) &&
//         (rev_fr_ptr->fr_rate == DSRLP_RATE_BLANK));

    rev_fr_ptr->fr_rate = DSRLP_RATE_BLANK;

    if( rev_fr_ptr->srvc_type == DSRLP_PRIMARY_SRVC )
    {
      prim_dcch_data_ptr = NULL;
      prim_dcch_rate     = CAI_BLANK_RATE;
    }
    #ifdef FEATURE_IS2000_REL_A_SVD
    else if( rev_fr_ptr->srvc_type == DSRLP_SECONDARY_SRVC )
    {
      sec_dcch_data_ptr = NULL;
      sec_dcch_num_bits = 0;
    }
    #endif /* FEATURE_IS2000_REL_A_SVD */
  }
} /* txc_tx_post_rlp_dcch_process */

/*===========================================================================

FUNCTION TXC_TX_FRAME_PROC

DESCRIPTION
   Note that this function is created by splitting the txc_voc_xchg into
   two parts. This part does the RLP frame building in case of data services
   service option and gets signalling from the CP. The real mulitplexing is
   done here.

   This is called from txc_traffic so as to build the TX frame in the background
   and not to load the TX ISR.

DEPENDENCIES
   This function was created on 24th March 1999. [AJ and GB]

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
/* build the frame in the background when the txc_voc_xchg signals */
void txc_tx_frame_proc( void )
{
  /* This is new frame building in the background. If feature for minimizing the
  ** ISR loading is defined then we do not call RLP in the txc_pri which executes in
  ** TXC interrupt context but is called in this function which will be called from the
  ** TXC_TRAFFIC task. Thus RLP TX runs in background reducing the ISR loading.
  */
  #ifdef FEATURE_IS2000_REL_A
  /* flush the RDCH buffer */
  static boolean flush_rdch_buffer = FALSE;
  #endif /* End of FEATURE_IS2000_REL_A */

  byte i;                                       /* loop ctr              */

  boolean so_is_voice = CAI_IS_SO_VOICE(txc_get_pri_so());

  int16  tx_open_loop=0, tx_gain_adj=0, tx_pwr_limit=0;
  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  rfm_device_enum_type tx_dev;
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Store the frame rate for logging.  curr is the one being
   * encoded now.  next is the one that will be sent in 20ms. */
  curr_sch0_frame_rate = next_sch0_frame_rate;

  TXC_ENTER_ATOMIC_SECTION();

  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  tx_dev = txc_get_tx_device();

  /* Read in 10-bit TX_OPEN_LOOP value */
  /* Read in 9-bit TX_GAIN_ADJ value, units -1/4 dB */
  tx_open_loop = rfm_1x_get_tx_open_loop( tx_dev );
  tx_gain_adj  = rfm_1x_get_tx_gain_adjust( tx_dev );
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

  TXC_LEAVE_ATOMIC_SECTION();

  /* convert open loop to -1/12 dB units */
  tx_open_loop *= -1;

  /* change TX_GAIN_ADJ value to -1/12 dB units */
  tx_gain_adj *= -3;

  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  /* find out how much extra gain is available */
  tx_pwr_limit = (int16) rfm_1x_get_tx_pwr_limit( tx_dev );
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */
  tx_extra_gain = -(511 - tx_pwr_limit) - tx_open_loop - tx_gain_adj;

  /* add the difference between turnaround constants for RC1,RC2 and RC3,RC4 -
  ** since this gets added currently to the RATE_ADJ register in MSM.
  */
  tx_extra_gain += txtc.offset_pwr;

  if( !txtc.preambles )
  {
    /* DS_ISDATA is a macro containing several IFs */
    if( DS_ISDATA( txc_get_pri_so() ) )
    {
      /* Fill up the buffer with data, returns rate */
      if( txc_tx.tx_on )
      {
        if( txc_get_pri_so() != CAI_SO_PPP_PKT_DATA_3G)
        {
          /* Can send data on the FCH. Figure out the rate. */
          if (!txc_tx.active_msg)
          {
            switch( rlp_tx_get_next_frame( &prim_data_ptr, TRUE, RLP_FULL_RATE ) )
            {
              case RLP_EIGHTH_RATE:
                txc_data_rate = prim_rate = CAI_EIGHTH_RATE;
                break;

              case RLP_HALF_RATE:
                txc_data_rate = prim_rate = CAI_HALF_RATE;
                break;

              case RLP_FULL_RATE:
                txc_data_rate = prim_rate = CAI_FULL_RATE;
                break;

              case RLP_QUARTER_RATE:
                /* Not 3G call, should only be for FCH. */
                if( txc_get_rev_link_rate_set( txc_so.rev_fch.rc ) == ENC_RATE_14400)
                {
                  txc_data_rate = prim_rate = CAI_QUARTER_RATE;
                  break;
                }
                /* else fall thru for ERR_FATAL */

              default:
                ONEX_ERR_FATAL("Invalid Rate requested by RLP TX", 0, 0, 0 );
            }
          }
          else
          {
            txc_data_rate = prim_rate = CAI_BLANK_RATE;
          }
        }/* if not SO 33 */
        else
        {
          txc_tx_frame_proc_build_data3g(&txc_rev_fr_block);
        }
      } /*txc_tx.tx_on == TRUE */
      else
      {
        txc_data_rate = prim_rate = CAI_BLANK_RATE;
      }
    }/* if DS_ISDATA */
    #ifdef FEATURE_IS2000_REL_A_SVD
    else if( txc_get_sec_so() == CAI_SO_PPP_PKT_DATA_3G )
    {
      if( txc_tx.tx_on )
      {
        /* process Secondary data
        */
        txc_tx_frame_proc_build_data3g( &txc_rev_fr_block );
      }
      else
      {
        /* txc_tx.tx_on == FALSE */
        if( txc_so.rev_dcch.included &&
            ( txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_DCCH ) )
        {
          sec_dcch_num_bits = 0;
          sec_dcch_data_ptr = NULL;
        }

        if( txc_so.rev_fch.included &&
            ( txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_FCH ))
        {
          sec_fch_num_bits = 0;
          sec_fch_data_ptr = NULL;
        }
      }
    }
    #endif /* FEATURE_IS2000_REL_A_SVD */

    //
    // TDSO SCH part
    //
    else if( ( txc_get_pri_so() == CAI_SO_TDSO ) ||
             ( txc_get_pri_so() == CAI_SO_FULL_TDSO ) )
    {
      txc_max_pwr_limited_dtx_sch = FALSE;
      if( txtc.sch_on && txc_sch_max_pwr_protection_enabled )
      {
        if( txc_so.rev_fch.included )
        {
          if( tx_extra_gain <= -txc_sch_pch_adj_gain_tab [ENC_FULL_RATE] )
          {
            txc_max_pwr_limited_dtx_sch = TRUE;
          }
        }

        // Now do DCCH
        if (txc_so.rev_dcch.included &&
           (tx_extra_gain <=
              -txc_dcch_sch_gain_tab [DCCH_ACTIVE_SCH_ACTIVE].pch_adj_gain))
        {
          txc_max_pwr_limited_dtx_sch = TRUE;
        }
      }

      if( txtc.sch_on && !txc_max_pwr_limited_dtx_sch )
      {
        if( txtc.r_sch_rc == CAI_RC_4 )
        {
           sch_tdso_frame_block.sup_fr_mux = TDSO_RATE_SET_2;
        }
        else
        {
           sch_tdso_frame_block.sup_fr_mux = TDSO_RATE_SET_1;
        }
        sch_tdso_frame_block.fr_time = qw_lo( txc_tx.frame_cnt );
        #ifdef FEATURE_IS2000_REL_A
        /* if turbo code, use only one muxpdu 5 for the whole frame */
        if( txtc.supp_turbo && ( txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5 ) )
        {
          txtc.pot_num_mux_pdus = 1;
        }
        #endif /* FEATURE_IS2000_REL_A */

        for( i = 0; i < txtc.pot_num_mux_pdus; i++ )
        {
          sch_tdso_frame_block.frame_type.rev_arr[ i ].sch_rate =
             (tdso_sch_rate_type) ((uint32) txtc.supp_rate - 3);

          sch_tdso_frame_block.frame_type.rev_arr[ i ].fr_channel =
             TDSO_SCH0;

          #ifdef FEATURE_IS2000_REL_A
          if( txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5 )
          {
            sch_tdso_frame_block.frame_type.rev_arr[ i ].mux_pdu_type = TDSO_MUX_PDU_5;
            if (sch_tdso_frame_block.sup_fr_mux == TDSO_RATE_SET_1)
            {
              sch_tdso_frame_block.frame_type.rev_arr[ i ].max_frame_size =
                txc_tdso_muxpdu5_rs1[sch_tdso_frame_block.frame_type.rev_arr[ i ].sch_rate][txtc.supp_turbo];
            }
            else
            {
              sch_tdso_frame_block.frame_type.rev_arr[ i ].max_frame_size =
                txc_tdso_muxpdu5_rs2[sch_tdso_frame_block.frame_type.rev_arr[ i ].sch_rate][txtc.supp_turbo];
            }
          }
          else
          {
            if( txtc.double_sized )
            {
              sch_tdso_frame_block.frame_type.rev_arr[ i ].fr_rate =
                                                TDSO_RATE_2; // double sized, rate set 1
            }
            else
            {
              sch_tdso_frame_block.frame_type.rev_arr[ i ].fr_rate =
                                                TDSO_RATE_1; // single sized, rate set 1
            }

            // sch_tdso_frame_block.fr_time = ???;

            sch_tdso_frame_block.frame_type.rev_arr[ i ].max_frame_size =
                    tdso_data_len[ sch_tdso_frame_block.frame_type.rev_arr[ i ].fr_rate ]
                                 [ sch_tdso_frame_block.sup_fr_mux ];

            if( txtc.supp_rate > ENC_SCH_FULL_RATE )
            {
              sch_tdso_frame_block.frame_type.rev_arr[ i ].mux_pdu_type = TDSO_MUX_PDU_3;
            }
            else
            {
              if (txtc.r_sch_rc == CAI_RC_4)
              {
                sch_tdso_frame_block.frame_type.rev_arr[ i ].mux_pdu_type =
                             TDSO_MUX_PDU_2;
              }
              else
              {
                sch_tdso_frame_block.frame_type.rev_arr[ i ].mux_pdu_type =
                              TDSO_MUX_PDU_1;
              }
            }

          }
          #else
          if( txtc.double_sized )
          {
            sch_tdso_frame_block.frame_type.rev_arr[ i ].fr_rate =
               TDSO_RATE_2; // double sized, rate set 1
          }
          else
          {
            sch_tdso_frame_block.frame_type.rev_arr[ i ].fr_rate =
               TDSO_RATE_1; // single sized, rate set 1
          }

          // sch_tdso_frame_block.fr_time = ???;

          sch_tdso_frame_block.frame_type.rev_arr[ i ].max_frame_size =
             tdso_data_len[ sch_tdso_frame_block.frame_type.rev_arr[ i ].fr_rate ]
                          [ sch_tdso_frame_block.sup_fr_mux ];

          if( txtc.supp_rate > ENC_SCH_FULL_RATE )
          {
            sch_tdso_frame_block.frame_type.rev_arr[ i ].mux_pdu_type =
               TDSO_MUX_PDU_3;
          }
          else
          {
            if (txtc.r_sch_rc == CAI_RC_4)
            {
               sch_tdso_frame_block.frame_type.rev_arr[ i ].mux_pdu_type =
                  TDSO_MUX_PDU_2;
            }
            else
            {
               sch_tdso_frame_block.frame_type.rev_arr[ i ].mux_pdu_type =
                  TDSO_MUX_PDU_1;
            }
          }
          #endif /* FEATURE_IS2000_REL_A */
          sch_tdso_frame_block.frame_type.rev_arr[ i ].srvc_type =
             TDSO_PRIMARY_SRVC;
        }
        sch_tdso_frame_block.num_frames = txtc.pot_num_mux_pdus;
        if (tdso_build_tx_frames(REV_LINK, &sch_tdso_frame_block ) != TDSO_VALID)
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "TDSO Build Failed");
        }

        txc_rev_fr_block.num_frames = sch_tdso_frame_block.num_frames;
      }
      else
      {
        sch_tdso_frame_block.num_frames = 0;
        txc_rev_fr_block.num_frames = 0;
      }
    } /* if TDSO */
  } // of if (!txtc.preambles)
  else
  {
    // Set sig to NORM for preambles
    txtc.frame.sig = NORM;
  }

  /* build the actual frame buffer to be sent */
  if( onex_nonsig_is_ftm_mode() )
  {
    txc_tx.active_msg = FALSE;
    txc_tx.delay = TXC_NO_CONF;              /* no confirmation delay */
    txc_eib_flag = FALSE;
    #ifdef FEATURE_IS2000_REL_A
    txc_sch_eib_flag = FALSE;
    txc_qib_flag = FALSE;
    #endif /* FEATURE_IS2000_REL_A */
    if( txtc.num_preambles > 0 )
    {
      txtc.preambles = TRUE;
      --txtc.num_preambles;
    }
    else
    {
      txtc.preambles = FALSE;
      txtc.rate      = txc_ftm_rfch_rate;

      #ifdef FEATURE_IS2000_1X_ADV
      /*lint -e{650} 255 is purposefully out of range of the enum */
      if( (int) txtc.rate == (int) CAI_NON_CRITICAL_RATE )
      {
        /* A FCH rate of 255 from FTM signifies a blankable or non-critical
           1/8th rate frame. Not that this is not a part of the
           enc_rate_type. This rate would be provided by the vocoder
           or Markov SO. Since this is FTM we need to swizzle things.*/
        txtc.rate = ENC_8TH_RATE;
        txtc.rc8_status.is_non_critical_frame = TRUE;
      }
      else
      {
        txtc.rc8_status.is_non_critical_frame = FALSE;
      }
      #endif /* FEATURE_IS2000_1X_ADV */

      /* Protect against any invalid rates and default to FULL */
      if( txtc.rate >= ENC_NUM_FRAC_RATES )
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "Invalid rate (%d). Default to FULL (%d)",
          txtc.rate,
          ENC_FULL_RATE );
        txtc.rate = ENC_FULL_RATE;
      }

      txtc.supp_rate = txc_ftm_rsch_rate;

      /* Copy tx_pattern or rx loopback data */
      /* Support FEATURE_FTM_DCCH */
      if( txc_so.rev_dcch.included )
      {
        txtc.rate = txc_ftm_rdcch_rate;
        (void)memscpy( txtc.frame.data,
                       sizeof(txtc.frame.data),
                       txc_ftm_rdcch_data.data,
                       sizeof(txc_ftm_rdcch_data.data) );
      }

      if( txc_so.rev_fch.included )
      {
        (void)memscpy( txtc.frame.data,
                       sizeof(txtc.frame.data),
                       txc_ftm_rfch_data.data,
                       sizeof(txc_ftm_rfch_data.data) );
      }

      (void)memscpy( txtc.supp_frame.data,
                     sizeof(txtc.supp_frame.data),
                     txc_ftm_rsch_data.data,
                     sizeof(txc_ftm_rsch_data.data) );

      /* The data rates are set in txc_traffic_init() */
      if( txc_so.rev_dcch.included ||
          txc_so.rev_fch.rc >= CAI_RC_3 )
      {
        enc_pcbit_test_data( 0xffff );
        if( txtc.sch_on )
        {
          channel_gains.txc_pch_gain =
             txc_pch_fch_sch_gain_tab[TXC_UPPER_BOUNDS_CHECK(
                                txtc.supp_rate, ENC_NUM_SCH_RATES)].pch_gain;

          {
            int16  pch_adj_gain;
            pch_adj_gain =
                rpc_get_pch_adj_gain
                (
                  PCH_GAIN_TRUNCATE_TO_8BITS(txc_pch_fch_sch_gain_tab[
                       TXC_UPPER_BOUNDS_CHECK(txtc.supp_rate, ENC_NUM_SCH_RATES)].pch_gain)
                );

            channel_gains.txc_tx_rate_adj = pch_adj_gain + txtc.offset_pwr;
          }
        }
        else
        {
          channel_gains.txc_pch_gain =
             txc_pch_fch_gain_tab[ txtc.rate ].pch_gain;

          channel_gains.txc_tx_rate_adj = txc_pch_adj_gain_tab [txtc.rate] +
                             txtc.offset_pwr;
        }
      }
      else
      {
        channel_gains.txc_pch_gain = 0;
        channel_gains.txc_tx_rate_adj = txtc.offset_pwr;
      }

      if( txc_so.rev_fch.rc >= CAI_RC_3 )
      {
        if( txtc.sch_on )
        {
           channel_gains.txc_fch_gain =
              txc_pch_fch_sch_gain_tab[TXC_UPPER_BOUNDS_CHECK(
                                txtc.supp_rate, ENC_NUM_SCH_RATES)].fch_gain;
        }
        else
        {
          channel_gains.txc_fch_gain =
             txc_pch_fch_gain_tab[ txtc.rate ].fch_gain;
        }
      }
      else
      {
        channel_gains.txc_fch_gain =  TXC_IS95A_FCH_GAIN;
      } /* end if (txc_so.rev_fch.rc >= ENC_FCH_RC_3) */

      if( txc_so.rev_dcch.included == TRUE )
      {
        txtc.dcch_frame.dtx = FALSE;
        if( txc_ftm_dtx_dcch == TRUE )
        {
          txtc.dcch_frame.dtx = TRUE;
          txc_ftm_dtx_dcch = FALSE;
        }

        if( txtc.sch_on )
        {
          if( txtc.dcch_frame.dtx )
          {
            /* set the gain to zero if DTX */
            channel_gains.txc_dcch_gain = 0;
          }
          else
          {
            channel_gains.txc_dcch_gain =
              txc_pch_fch_sch_gain_tab[TXC_UPPER_BOUNDS_CHECK(
                        txtc.supp_rate, ENC_NUM_SCH_RATES)].fch_gain;
          }
        }
        else
        {
          if( txtc.dcch_frame.dtx )
          {
            /* set the gain to zero if DTX */
            channel_gains.txc_dcch_gain = 0;
          }
          else
          {
            channel_gains.txc_dcch_gain =
              txc_pch_fch_gain_tab[ txtc.rate ].fch_gain;
          }
        }
      }

      if( txtc.sch_on )
      {
        /* R-SCH is ON and ACTIVE */
        sch_state = TXC_REV_SCH_ACTIVE;

        channel_gains.txc_sch_gain =
           txc_pch_fch_sch_gain_tab[TXC_UPPER_BOUNDS_CHECK(
                        txtc.supp_rate, ENC_NUM_SCH_RATES)].sch_gain;
      }
      else
      {
        /* R-SCH is OFF */
        sch_state = TXC_REV_SCH_OFF;

        channel_gains.txc_sch_gain = 0;
      }

      if( ( txtc.last_encoder_interrupt_bin >= TXC_TC_SECOND_ISR_PCG ) &&
          ( txtc.last_encoder_interrupt_bin <= M1X_STMR_PCG_15 ) )
      {
        muxmdsp_set_channel_gains();
      }
    }
  } /* end if(ftm_mode == FTM_MODE) */

  if( !onex_nonsig_is_ftm_mode() )
  {
    if( txc_so.rev_dcch.included )
    {
      txc_build_dcch_frame ();
    }

    if( txc_so.rev_fch.included )
    {
      txc_sig_frame_type  signext = txtc.frame.sig;

      /* fill txtc frame (fundamental) with zeros */
      (void)memscpy( txtc.frame.data,
                     sizeof(txtc.frame.data),
                     zeros,
                     ENC_14400_FULL_RATE_SIZE );

      #ifdef FEATURE_IS2000_REL_A_SVD
      /*
      ** If R-FCH is not the signaling channel,
      ** build only normal traffic frame on FCH
      ** regardless of whether a signaling msg is pending or not.
      */
      if( txtc.cur_dsch != TXC_REV_FCH )
      {
        signext = NORM;
      }
      else
      #endif /* FEATURE_IS2000_REL_A_SVD */

      /* In hard handoff case, txtc.frame.sig == NORM may be overwritten by
       * the call to txc_traffic_signext() above since HCM has been scheduled
       * and is pending. If signext is not set to NORM here when preamble
       * is in progress, then the preamble duration will exceed duration given
       * by standard.
       */
      if( txtc.preambles == TRUE )
      {
        signext = NORM;
      }

      /* build the actual frame buffer (fundamental) to be sent */
      txc_build_frame (signext);
    }
  }

  /* Store the fch frame rate (full/half/...) and signalling
   * type (dim/blank/norm) for logging.  curr is the one being
   * encoded now.  next is the one that will be sent in 20ms.
   * next_fch_sig_type is set by txc_build_frame().
   */
  curr_fch_frame_rate = next_fch_frame_rate;
  next_fch_frame_rate = txtc.rate;

  /* Log frame type if we are not sending preambles */
  if( !txtc.preambles )
  {
    #ifdef FEATURE_IS2000_REL_A
    if( log_status( LOG_RDCH_FRAME_INFO_C ) )
    {
      txc_rdch_log_frame_info();
      flush_rdch_buffer = TRUE;
    }
    else if( flush_rdch_buffer )
    {
      /* flush the buffer */
      txc_rdch_frame_info_send_log();
      flush_rdch_buffer = FALSE;
    }
    #else
    txc_log_frame_type();
    #endif /* FEATURE_IS2000_REL_A */
  }

  /* Since curr_fch_sig_type has been logged, now update it to its value
     for the next time txc_log_frame_type() will be called */
  curr_fch_sig_type = next_fch_sig_type;

  if( txc_tx.active_msg )   /* Currently working on a signalling message */
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Processing signaling message, POS = %d, LEN = %d, Delay = %d",
      txc_tx.buf.pos,
      txc_tx.buf.len,
      txtc.sig_frame_delay );

    if( txc_tx.buf.pos >= txc_tx.buf.len )   /* Done with message? */
    {
      if( txtc.quick_repeat_frame_cnt > 0 )
      {
        M1X_MSG( MUX, LEGACY_MED,
          "Performing Signaling Quick Repeat, QR Count: %d",
          txtc.quick_repeat_frame_cnt );

        txc_tx.buf.pos = 0;
        txc_traffic_signext();
        txtc.quick_repeat_frame_cnt--;

        if( txtc.quick_repeat_frame_cnt == 0 )
        {
          /* Reset the quick repeat value to the native 1x TC value. This is
             to cover the case where we came from TT and may do a normal HHO later
             on. */
          txc_sig_quick_repeat_value = txc_sig_quick_repeat_for_tc_value;
        }
      }
      else
      {
        M1X_MSG( MUX, LEGACY_MED,
          "Signaling Processing Completed" );
        txtc.frame.sig = NORM;        /* No message data this time */
        txc_tx.active_msg = FALSE;    /* No signalling message for now */

        /* Log message */
        txc_log_tc_msg( txtc.cur_dsch, &txc_tx.buf );

        /* Check if it is necessary to delay confirmation to MC by 1 frame.
        ** This delay is used to stagger back to back signalling messages to
        ** allow voice to have 1 full rate frame before another signalling
        ** message is sent.  In case of Release B, RXTX wants the confirmation
        ** when the message has been sent, not when it is built.
        */
        if( ( ( so_is_voice ) &&
              ( txtc.cur_dsch == TXC_REV_FCH ) )
             #ifdef FEATURE_IS2000_REL_B
             || ( txc_tx.confirmation_type == CONFIRM_MSG_SENT )
             #endif /* FEATURE_IS2000_REL_B */
           )
        {
          txc_tx.delay = TXC_DELAY_CONF;
        }
        else
        {
          txc_tx.delay = TXC_SEND_CONF;
        } /* end if (SO_IS_VOICE... */
      }
    }
    else /* else of if (txc_tx.buf.pos... */
    {
      /* work out whether we need a blank or dim frame next */
      txc_traffic_signext();
    }
  }
  else /* else of if (txc_tx.active_msg) */
  {
    txtc.frame.sig = NORM;        /* No message data this time */
  } /* end if (txc_tx.active_msg) */

  /* Check if we need to send signalling confirmation back to MC. */
  if( txc_tx.delay != TXC_NO_CONF )
  {
    if( txc_tx.delay == TXC_DELAY_CONF )
    {
      /* Delay confirmation till the next frame. */
      txc_tx.delay = TXC_SEND_CONF;
    }
    else
    {
      /* Send confirmation this frame. */
      txc_tx.delay = TXC_NO_CONF;
      M1X_MSG( MUX, LEGACY_HIGH,
        "Send sig msg confirm,active_msg=%d,pos=%d,len=%d",
        txc_tx.active_msg,
        txc_tx.buf.pos,
        txc_tx.buf.len );

      /* Send confirmation and dispose of command buffer. */
      txc_cmd_done( txc_tx.buf.cmd_ptr, TXC_DONE_S );

      /* Mark that cnf has been sent to RXTX task for current message and
       * there is no need to send it while exiting from traffic state */
      txc_need_to_send_msg_rsp = FALSE;
    }
  } /* end if (txc_tx.delay != TXC_NO_CONF) */

  /*
  ** Set Rate set 2 forward power control erasure reporting bit
  */
  if( txc_so.rev_fch.included       &&
      txc_so.rev_fch.rc == CAI_RC_2 &&
      txc_eib_flag )
  {
    txtc.frame.data[0] |= 0x80;
    txc_eib_flag = FALSE;
  }

  /* Send data to encoder, not generating a CRC if we are currently
  ** transmitting traffic channel preamble frames.
  */
  if( onex_nonsig_is_ftm_mode() )
  {
    if( txtc.preambles )
    {
      if( txc_so.rev_dcch.included )
      {
        enc_dcch_data_out( zeros, FALSE, ENC_FULL_RATE );
        M1X_MSG( MUX, LEGACY_HIGH,
           "Sending Preamble frame" );
      }

      if( txc_so.rev_fch.included )
      {
        enc_fch_data_out( zeros, FALSE, ENC_FULL_RATE );
        M1X_MSG( MUX, LEGACY_HIGH,
           "Sending Preamble frame" );
      }
    }
    else
    {
       if( txtc.sch_on )
       {
         /* Send the R-SCH frame in one shot here for FTM */
         enc_sch_ftm_data_out
         (
            txtc.supp_frame.data,
            TRUE,                     // generate CRC or not
            txtc.supp_rate,
            txtc.supp_turbo,          // Turbo or not
            txtc.supp_ltu_size        // LTU size; 0 means no LTU
         );
       }
    }
  }

  /* Set RL channel gains in DMSS mode, AND in FTM mode for RC8 only */
  if(!onex_nonsig_is_ftm_mode() || txc_so.rev_fch.rc == CAI_RC_8)
  {
    if ( txc_tx.tx_on )
    {
      /* Set gains properly when sending preambles for RC 3 or higher
         Note: only PiCH transmitted on preambles */
      if (txtc.preambles && (
          txc_so.rev_dcch.included ||
          (txc_so.rev_fch.rc >= CAI_RC_3)) )
      {
        #ifdef FEATURE_IS2000_1X_ADV
        /*  For 1x Advanced RC's there is no PICH ratio
            or tx rate adj programming. This is handled by the mdsp.
            Clear all ratios to 0 during preambles.
        */
        if( txc_so.rev_fch.rc == CAI_RC_8 )
        {
          rc8_frame_config_parms.fch_linear_gain_ratio     = 0;
          rc8_frame_config_parms.sch_linear_gain_ratio     = 0;
          rc8_frame_config_parms.fch_ack_linear_gain_ratio = 0;
          rc8_frame_config_parms.sch_ack_linear_gain_ratio = 0;
        }
        else
        #endif /* FEATURE_IS2000_1X_ADV */
        {
          /* During preambles set R-FCH, R-DCCH, R-SCH digital gain = 0 */
          channel_gains.txc_pch_gain    = RPC_TOT_DIG_PWR_255<<7;
          channel_gains.txc_fch_gain    = 0;
          channel_gains.txc_dcch_gain   = 0;
          channel_gains.txc_sch_gain    = 0;
          channel_gains.txc_tx_rate_adj = txtc.offset_pwr;
        }
      }
      else
      {
        if (
           txc_so.rev_dcch.included ||
           (txc_so.rev_fch.included && (txc_so.rev_fch.rc >= CAI_RC_3))
        )
        {
          if ( txtc.sch_on )
          {
            if ( (
#ifndef FEATURE_IS2000_REL_A
                  (txc_get_pri_so() == CAI_SO_PPP_PKT_DATA_3G) &&
#else
                  ((txc_get_pri_so() == CAI_SO_PPP_PKT_DATA_3G) ||
                   (txc_get_sec_so() == CAI_SO_PPP_PKT_DATA_3G)) &&
#endif /* FEATURE_IS2000_REL_A */
                  (txc_rev_fr_block.num_frames > SCH_BUF_START))
                 ||
                 (
#ifndef FEATURE_IS2000_REL_A
                  ((txc_get_pri_so() == CAI_SO_TDSO) ||
                   (txc_get_pri_so() == CAI_SO_FULL_TDSO)) &&
#else
                  ((txc_get_pri_so() == CAI_SO_TDSO) ||
                   (txc_get_sec_so() == CAI_SO_TDSO) ||
                   (txc_get_pri_so() == CAI_SO_FULL_TDSO) ||
                   (txc_get_sec_so() == CAI_SO_FULL_TDSO)) &&
#endif /* FEATURE_IS2000_REL_A */
                  (txc_rev_fr_block.num_frames > 0))
                 )
            {
              /* R-SCH is ON and ACTIVE */
              sch_state = TXC_REV_SCH_ACTIVE;
#ifdef FEATURE_IS2000_REL_A_SVD
              if (txc_so.rev_dcch.included && txc_so.rev_fch.included)
              {
                /* FCH + DCCH + SCH */
                txc_fch_dcch_sch_gain_type *gain_ptr;
                 /* FCH and SCH are active and DCCH is DTXed */
                 if (txtc.dcch_frame.dtx)
                 {
                   gain_ptr = &txc_fch_dcch_sch_gain_tab
                                           [txtc.rate][DCCH_DTX_SCH_ACTIVE];
                 }
                 else /* FCH, DCCH and SCH are active */
                 {
                   gain_ptr = &txc_fch_dcch_sch_gain_tab
                                           [txtc.rate][DCCH_ACTIVE_SCH_ACTIVE];
                 }
                 channel_gains.txc_pch_gain    = gain_ptr->gain.pch_gain;
                 channel_gains.txc_fch_gain    = gain_ptr->gain.fch_gain;
                 channel_gains.txc_dcch_gain   = gain_ptr->gain.dcch_gain;
                 channel_gains.txc_sch_gain    = gain_ptr->gain.sch_gain;
                 channel_gains.txc_tx_rate_adj = gain_ptr->pch_adj_gain +
                                                        txtc.offset_pwr;
              }
              else if (txc_so.rev_dcch.included)
#else
              if (txc_so.rev_dcch.included)
#endif /* FEATURE_IS2000_REL_A_SVD */
              {
                /* DCCH + SCH */
                txc_dcch_sch_gain_type *gain_ptr;

                if (txtc.dcch_frame.dtx)
                {
                  gain_ptr = &txc_dcch_sch_gain_tab[DCCH_DTX_SCH_ACTIVE];
                }
                else
                {
                  gain_ptr = &txc_dcch_sch_gain_tab[DCCH_ACTIVE_SCH_ACTIVE];
                }

                channel_gains.txc_pch_gain    = gain_ptr->gain.pch_gain;
                channel_gains.txc_fch_gain    = gain_ptr->gain.fch_gain;
                channel_gains.txc_dcch_gain   = gain_ptr->gain.dcch_gain;
                channel_gains.txc_sch_gain    = gain_ptr->gain.sch_gain;
                channel_gains.txc_tx_rate_adj = gain_ptr->pch_adj_gain +
                                                          txtc.offset_pwr;
              }
              else
              {
                /* FCH + SCH */
                txc_set_rev_chan_gain_fch_only_sch_on();
              }
            }
            else
            {
              /* R-SCH is DTX */
              sch_state = TXC_REV_SCH_DTX;
              #ifdef FEATURE_IS2000_REL_A_SVD
              if (txc_so.rev_dcch.included && txc_so.rev_fch.included)
              {
                /* FCH + DCCH + SCH (DTX) */
                txc_fch_dcch_sch_gain_type *gain_ptr;
                 /* FCH is active. SCH and DCCH are DTXed */
                 if (txtc.dcch_frame.dtx)
                 {
                   gain_ptr = &txc_fch_dcch_sch_gain_tab[txtc.rate][DCCH_DTX_SCH_DTX];
                 }
                 else /* FCH, DCCH are active and SCH is DTXed */
                 {
                   gain_ptr = &txc_fch_dcch_sch_gain_tab[txtc.rate][DCCH_ACTIVE_SCH_DTX];
                 }
                 channel_gains.txc_pch_gain    = gain_ptr->gain.pch_gain;
                 channel_gains.txc_fch_gain    = gain_ptr->gain.fch_gain;
                 channel_gains.txc_dcch_gain   = gain_ptr->gain.dcch_gain;
                 channel_gains.txc_sch_gain    = gain_ptr->gain.sch_gain;
                 channel_gains.txc_tx_rate_adj = gain_ptr->pch_adj_gain +
                                                           txtc.offset_pwr;
              }
              else if (txc_so.rev_dcch.included)
              #else
              if (txc_so.rev_dcch.included)
              #endif /* FEATURE_IS2000_REL_A_SVD */
              {
                /* DCCH + SCH (DTX) */
                txc_dcch_sch_gain_type *gain_ptr;

                if (txtc.dcch_frame.dtx)
                {
                  gain_ptr = &txc_dcch_sch_gain_tab[DCCH_DTX_SCH_DTX];
                }
                else
                {
                  gain_ptr = &txc_dcch_sch_gain_tab[DCCH_ACTIVE_SCH_DTX];
                }

                channel_gains.txc_pch_gain    = gain_ptr->gain.pch_gain;
                channel_gains.txc_dcch_gain   = gain_ptr->gain.dcch_gain;
                channel_gains.txc_sch_gain    = gain_ptr->gain.sch_gain;
                channel_gains.txc_fch_gain    = 0;
                channel_gains.txc_tx_rate_adj = gain_ptr->pch_adj_gain +
                                                          txtc.offset_pwr;
              }
              else
              {
                /* FCH + SCH (DTX) */
                txc_set_rev_chan_gain_fch_only_sch_dtx();
              }
            }
          }
          else
          {
            /* R-SCH is OFF */
            sch_state = TXC_REV_SCH_OFF;
            #ifdef FEATURE_IS2000_REL_A_SVD
            if (txc_so.rev_dcch.included && txc_so.rev_fch.included)
            {
              /* FCH + DCCH */
               txc_fch_dcch_sch_gain_type *gain_ptr;
               /* FCH is active, DCCH is DTXed and SCH is off*/
               if (txtc.dcch_frame.dtx)
               {
                 gain_ptr = &txc_fch_dcch_sch_gain_tab[txtc.rate][DCCH_DTX_SCH_OFF];
               }
               else /* FCH, DCCH are active and SCH is OFF*/
               {
                 gain_ptr = &txc_fch_dcch_sch_gain_tab[txtc.rate][DCCH_ACTIVE_SCH_OFF];
               }
               channel_gains.txc_pch_gain    = gain_ptr->gain.pch_gain;
               channel_gains.txc_dcch_gain   = gain_ptr->gain.dcch_gain;
               channel_gains.txc_fch_gain    = gain_ptr->gain.fch_gain;
               channel_gains.txc_sch_gain    = gain_ptr->gain.sch_gain;
               channel_gains.txc_tx_rate_adj = gain_ptr->pch_adj_gain +
                                                         txtc.offset_pwr;
            }
            else if( txc_so.rev_dcch.included )
            #else
            if (txc_so.rev_dcch.included)
            #endif /* FEATURE_IS2000_REL_A_SVD */
            {
              /* DCCH ONLY */
              txc_dcch_sch_gain_type *gain_ptr;

              if (txtc.dcch_frame.dtx)
              {
                gain_ptr = &txc_dcch_sch_gain_tab[DCCH_DTX_SCH_OFF];
              }
              else
              {
                gain_ptr = &txc_dcch_sch_gain_tab[DCCH_ACTIVE_SCH_OFF];
              }

              channel_gains.txc_pch_gain = gain_ptr->gain.pch_gain;
              channel_gains.txc_dcch_gain = gain_ptr->gain.dcch_gain;
              channel_gains.txc_sch_gain = gain_ptr->gain.sch_gain;
              channel_gains.txc_fch_gain = 0;
              channel_gains.txc_tx_rate_adj = gain_ptr->pch_adj_gain +
                                                        txtc.offset_pwr;
            }
            else
            {
              /* FCH ONLY */
              txc_set_rev_chan_gain_fch_only_sch_off();
            }
          } // else part of if ( txtc.sch_on )
        } // of if ( txc_so.rev_dcch.included || ...
      }
    } /* of if ( txc_tx.tx_on ... */

    /* Log power control values for the frame currently transmitted.
    ** This function makes use of txc_rev_fr_block.num_frames to determine
    ** if the NEXT SCH frame is DTX or not.
    */
    txc_rpctrl_log_frame (txc_rev_fr_block.num_frames);

    //
    // Must program one of MOD_MISC_CTL or TX_PATN register with the
    // correct mode or pattern depending on data rate if we are in FCH
    // 8TH rate gating mode. Hardware does not automatically do this.
    // We program both registers here just to be sure.
    //
    if( txtc.fch_8th_rate_gating )
    {
      if( txtc.rate == ENC_8TH_RATE )
      {
        enc_gating_mode( ENC_FCH_8TH_RATE_GATING_MODE );
        enc_is95c_tx_patn( ENC_FCH_8TH_RATE_GATING_PATTERN );
      }
      else
      {
        //
        // Only one of the 3 gating "modes" (PILOT_GATING, FCH_8TH_RATE and
        // HHO_PREAMBLE can be on at any given time, so it is OK to clear all
        // gating modes if we were in FCH_8TH_RATE "mode" and we are not sending
        // 8th rate frames
        //
        enc_gating_mode( ENC_NO_GATING_MODE );
        enc_is95c_tx_patn( ENC_NO_GATING_PATTERN );
      }
    }

    if ((txtc.sch_on == TRUE) &&
        (
#ifndef FEATURE_IS2000_REL_A
         ((txc_get_pri_so() == CAI_SO_PPP_PKT_DATA_3G) &&
          (txc_rev_fr_block.num_frames > SCH_BUF_START))
#else
         (((txc_get_pri_so() == CAI_SO_PPP_PKT_DATA_3G) ||
           (txc_get_sec_so() == CAI_SO_PPP_PKT_DATA_3G)) &&
          (txc_rev_fr_block.num_frames > SCH_BUF_START))
#endif /* FEATURE_IS2000_REL_A */
        ||
#ifndef FEATURE_IS2000_REL_A
         (((txc_get_pri_so() == CAI_SO_TDSO) ||
           (txc_get_pri_so() == CAI_SO_FULL_TDSO)) &&
          (txc_rev_fr_block.num_frames > 0))
#else
         (((txc_get_pri_so() == CAI_SO_TDSO) ||
           (txc_get_sec_so() == CAI_SO_TDSO) ||
           (txc_get_pri_so() == CAI_SO_FULL_TDSO) ||
           (txc_get_sec_so() == CAI_SO_FULL_TDSO)) &&
          (txc_rev_fr_block.num_frames > 0))
#endif /* FEATURE_IS2000_REL_A */
        )
    )
    {
      /* Initialize the encoder HW for the upcoming R-SCH frame */
      enc_sch_data_out_cfg
      (
        txtc.supp_rate,
        txtc.supp_turbo,
        txtc.supp_ltu_size,
        TRUE
      );

      /* Store the frame rate for logging. Next is the one that will be sent in 20ms. */
      next_sch0_frame_rate = MUX_LOG_SCH_FULL_RATE;

    }
    else
    {
      if (txtc.sch_on == TRUE)
      {
        (void) parm_xtra_inc(CAI_XTRA_SCH0_REV_SDU_DTX_ID, 1);
      }
      enc_sch_enable( FALSE);

      /* Store the frame rate for logging. Next is the one that will be sent in 20ms. */
      next_sch0_frame_rate = MUX_LOG_SCH_BLANK_FRAME;

    }

#ifndef FEATURE_IS2000_REL_A
    if((txc_get_pri_so() == CAI_SO_TDSO ) ||
            (txc_get_pri_so() == CAI_SO_FULL_TDSO ))
#else
    if((txc_get_pri_so() == CAI_SO_TDSO ) ||
            (txc_get_pri_so() == CAI_SO_FULL_TDSO ))
#endif /* FEATURE_IS2000_REL_A */
    {
      if ((txtc.sch_on == TRUE) && (txc_rev_fr_block.num_frames > 0))
      {
#ifdef FEATURE_IS2000_REL_A
        if (txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5)
        {
          switch (txtc.supp_rate)
          {
            case ENC_SCH_8TH_RATE:
            case ENC_SCH_QTR_RATE:
            case ENC_SCH_HALF_RATE:
              break;
            case ENC_SCH_FULL_RATE:
              (void) parm_inc(CAI_RSCH0_1_ID, 1);
              break;
            case ENC_SCH_2X_RATE:
              (void) parm_inc(CAI_RSCH0_2_ID, 1);
              break;
            case ENC_SCH_4X_RATE:
              (void) parm_inc(CAI_RSCH0_3_ID, 1);
              break;
            case ENC_SCH_8X_RATE:
              (void) parm_inc(CAI_RSCH0_4_ID, 1);
              break;
            case ENC_SCH_16X_RATE:
              (void) parm_inc(CAI_RSCH0_5_ID, 1);
              break;
            case ENC_SCH_32X_RATE:
              (void) parm_inc(CAI_RSCH0_6_ID, 1);
              break;
            default:
              ONEX_ERR_FATAL("Invalid SCH rate (%d)",(int) txtc.supp_rate,0,0);
          }
        }
        else
#endif /* FEATURE_IS2000_REL_A */
        {
          switch (txtc.supp_rate)
          {
            case ENC_SCH_8TH_RATE:
            case ENC_SCH_QTR_RATE:
            case ENC_SCH_HALF_RATE:
              break;
            case ENC_SCH_FULL_RATE:
              (void) parm_inc(CAI_SCH0_REV_1X_ID, 1);
              break;
            case ENC_SCH_2X_RATE:
              (void) parm_inc(CAI_SCH0_REV_2X_ID, 1);
              break;
            case ENC_SCH_4X_RATE:
              (void) parm_inc(CAI_SCH0_REV_4X_ID, 1);
              break;
            case ENC_SCH_8X_RATE:
              (void) parm_inc(CAI_SCH0_REV_8X_ID, 1);
              break;
            case ENC_SCH_16X_RATE:
              (void) parm_inc(CAI_SCH0_REV_16X_ID, 1);
              break;
#ifdef FEATURE_IS2000_REL_A
            case ENC_SCH_32X_RATE:
              (void) parm_inc(CAI_SCH0_REV_32X_ID, 1);
              break;
#endif /* FEATURE_IS2000_REL_A */
            default:
              ONEX_ERR_FATAL("Invalid SCH rate (%d)",(int) txtc.supp_rate,0,0);
          }
        }
      }

      for( i=0; i< txc_rev_fr_block.num_frames; i++)
      {
        // rev_frptr =  &sch_tdso_frame_block.frame_type.rev_arr[i].fr_ptr[ i ];
        /* Assuming 1 MuxPDU fits completely in a dsm item */
        if ((!txtc.supp_turbo) && (txtc.supp_rate > ENC_SCH_2X_RATE))
        {
          /*-----------------------------------------------------------------
            Only increment when not turbo and > 2x, as those use LTU's
          -----------------------------------------------------------------*/
#ifdef FEATURE_IS2000_REL_A
          if (txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5)
          {
            (void) parm_inc(CAI_RSCH0_LTU_ID, 1);
          }
          else
#endif /* FEATURE_IS2000_REL_A */
          {
            (void) parm_inc(CAI_SCH0_REV_LTU_ID, 1);
          }
        }
#ifdef FEATURE_IS2000_REL_A
        if ((txtc.supp_rate == ENC_SCH_FULL_RATE) && (txtc.mux_pdu_type != CAI_MUX_PDU_TYPE_5))
#else
        if (txtc.supp_rate == ENC_SCH_FULL_RATE)
#endif /* FEATURE_IS2000_REL_A */
        {
          /* for 1x, need to have Mixed mode bit pre-appended */
          txtc.supp_frame.data[0] = 0x00;
          if (txtc.r_sch_rc == CAI_RC_4)
          {

            b_copy( sch_tdso_frame_block.frame_type.rev_arr[i].fr_item_ptr, 0,
                    txtc.supp_frame.data, 2,
                    MUX2_FULL_PRI_LEN );

            enc_sch_muxpdu_data_out( txtc.supp_frame.data,
                                     ((MUX2_FULL_PRI_LEN / 8) + 1) );
          }
          else
          {
            b_copy( sch_tdso_frame_block.frame_type.rev_arr[i].fr_item_ptr, 0,
                    txtc.supp_frame.data, 1,
                    CAI_FULL_RATE_BITS );

            enc_sch_muxpdu_data_out( txtc.supp_frame.data,
                                     ((CAI_FULL_RATE_BITS/8) + 1) );
          }
        }
        else
        {
          word bits_to_copy;
          word total_byte_to_copy;

          /* Prefix the Mux PDU type 3 header */
          txtc.supp_frame.data[0] = 0x20;
#ifdef FEATURE_IS2000_REL_A
          if ((txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5) &&
              (txtc.supp_rate >= ENC_SCH_FULL_RATE))
          {
            if (txtc.r_sch_rc == CAI_RC_4)
            {
              bits_to_copy = txc_tdso_muxpdu5_rs2[
                (uint32) txtc.supp_rate - (uint32) ENC_SCH_FULL_RATE][txtc.supp_turbo];
            }
            else
            {
              bits_to_copy = txc_tdso_muxpdu5_rs1[
                (uint32) txtc.supp_rate - (uint32) ENC_SCH_FULL_RATE][txtc.supp_turbo];
            }
          }
          else
          {
            if (txtc.r_sch_rc == CAI_RC_4)
            {
                bits_to_copy = txtc.double_sized ? 538 : 265;
            }
            else
            {
                bits_to_copy = txtc.double_sized ? 346 : 170;
            }
          }
#else
          if (txtc.r_sch_rc == CAI_RC_4)
          {
              bits_to_copy = txtc.double_sized ? 538 : 265;
          }
          else
          {
              bits_to_copy = txtc.double_sized ? 346 : 170;
          }
#endif /* FEATURE_IS2000_REL_A */

          total_byte_to_copy = ((bits_to_copy+6)/8);

          b_copy( sch_tdso_frame_block.frame_type.rev_arr[i].fr_item_ptr,0,
                  txtc.supp_frame.data,6 ,bits_to_copy );

#ifdef FEATURE_IS2000_REL_A
          if ( txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5 )
          {
            enc_sch_muxpdu_data_out(txtc.supp_frame.data,total_byte_to_copy);
          }
          else
#endif /*  FEATURE_IS2000_REL_A */
          {
            enc_sch_muxpdu_data_out(
              txtc.supp_frame.data,
              total_byte_to_copy
            );
          }
        }
      } /* for loop */

      /*
      ** The IS2000 standard specifies that the "spare bits", remaining after
      ** the maximum number of PDUs for the given rate have been used up to form
      ** the SDU, shall be filled with 0's. Since turbo coded frames do not have
      ** LTU CRCs, this number is a function of the supplemental channel data
      ** rate and the code type. Convolutional = 0, Turbo = 1.
      */
      /* note that, for TDSO MUXPDU 5, there is no spare bit needs to be filled */
#ifdef FEATURE_IS2000_REL_A
      if ((txtc.sch_on == TRUE) && (txc_rev_fr_block.num_frames > 0)
         && (txtc.mux_pdu_type != CAI_MUX_PDU_TYPE_5))
#else
      if ((txtc.sch_on == TRUE) && (txc_rev_fr_block.num_frames > 0))
#endif /* FEATURE_IS2000_REL_A */
      {
        enc_sch_muxpdu_data_out(
          txc_sch_spare_bits,
          (txc_num_sch_spare_bits[ txtc.supp_rate ][ txtc.supp_turbo ])/8
        );

        /* flush with 16 bits for single sized PDUs */
        if( !txtc.double_sized )
        {
          enc_sch_muxpdu_data_out(
            txc_sch_spare_bits,
            2
          );
        }
      }

      /* Send the R-SCH frame for TDSO */
      enc_sch_write_data_done();
    } /* TDSO R-SCH handling block */
  }

  /* Channel configuration has potentially changed, we need to run the
     RF PA backoff algorithm to back off the power if necessary */
  txc_update_chan_config_to_rf( TRUE );

  /* Do an encoder status check/repair */
  enc_status_check();

} /* txc_tx_frame_proc() */

/*===========================================================================
FUNCTION     TXC_BUILD_DCCH_FRAME

DESCRIPTION  This function builds a frame to be transmitted on R-DCCH.

DEPENDENCIES None.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void txc_build_dcch_frame ( void )
{
  boolean msg_on_dcch = txc_so.rev_dcch.included &&
                      (txtc.cur_dsch == TXC_REV_DCCH) &&
                      txc_tx.active_msg;

  boolean control_hold = txtc.gating_rate != CAI_GATING_RATE_NONE;

  txtc.dcch_frame.dtx = FALSE;

  /* When sending preambles, or NULLs and no signaling message to transmit,
  ** DCCH can be DTX'd.
  */
  if ( txtc.preambles || ((txtc.nulls
       || control_hold
       ) && !msg_on_dcch) )
  {
    txtc.dcch_frame.dtx = TRUE;
    if ( txtc.preambles && txc_hho_preamble_pcg_units )
    {
      // We want num_preambles 1's as the pilot gating pattern
      enc_is95c_tx_patn ((enc_gating_pattern_type) PREAMBLES_TO_GATING_PATN( txtc.num_preambles ));
      enc_gating_mode( ENC_HHO_PREAMBLE_MODE );
    }

    if ( !txtc.preambles )
    {
      /* DTX DCCH frame
      */
      txc_dtx_dcch_frame();
    }
  }
  // See if signalling frame is requested
  else if ( msg_on_dcch )
  {
    byte tmpCat;

    /* Send a signalling frame
    */
    (void)memscpy( txtc.dcch_frame.data,
                   sizeof(txtc.dcch_frame.data),
                   zeros,
                   ENC_14400_FULL_RATE_SIZE );

    /* Build the signalling frame buffer and increment frame category counter
    */
    tmpCat = build_blank_frame(TXC_REV_DCCH);
    (void) parm_inc( tmpCat, 1 );
    txtc.dcch_frame.frame_cat = tmpCat;
  }
#ifdef FEATURE_IS2000_REL_A_SVD
  //
  // REMOVE else if{...} after DEBUG
  //
  else if ((txc_get_pri_phy_chan() & MCTRANS_PHY_CHAN_DCCH) &&
           (txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_DCCH))
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Pri + Sec not supported on DCCH");
  }
#endif // FEATURE_IS2000_REL_A_SVD
  /* no signalling frame on DCCH.  See if RLP has anything
  */
  else if (
           ((prim_dcch_rate == CAI_NULL_RATE) || (prim_dcch_rate == CAI_BLANK_RATE))
#ifdef FEATURE_IS2000_REL_A_SVD
           && (txc_get_pri_phy_chan() & MCTRANS_PHY_CHAN_DCCH)
#endif // FEATURE_IS2000_REL_A_SVD
          )
  {
    /* Force this to BLANK if DCCH is DTX'd.  Used in RLP logging later.
    */
    txc_rev_fr_block.rev_arr[DCCH_BUF].fr_rate = DSRLP_RATE_BLANK;

    /* DTX DCCH frame
    */
    txc_dtx_dcch_frame();
  }
#ifdef FEATURE_IS2000_REL_A_SVD
  else if (txc_get_pri_phy_chan() & MCTRANS_PHY_CHAN_DCCH)
#else
  else
#endif
  {
    if (prim_dcch_data_ptr == NULL)
    {
      /* already checked that prim_dcch_rate was not BLANK/NULL
      ** why is prim_dcch_ptr == NULL??
      */
      M1X_MSG( MUX, LEGACY_ERROR,
        "Prim_dcch_ptr is NULL");
      txc_dtx_dcch_frame();
    }
    else
    {

      txtc.dcch_frame.data[0] = 0;
      if ( txc_so.rev_dcch.rc == CAI_RC_3 )
      {
        /* RS 1 based
        */
        b_copy( prim_dcch_data_ptr, 0, txtc.dcch_frame.data, 1, CAI_FULL_RATE_BITS );
        (void) parm_inc(CAI_MUX1_REV_DCCH_1_ID, 1); // Increment primary traffic only counter
        txtc.dcch_frame.frame_cat = CAI_MUX1_REV_DCCH_1_ID;
      }
      else
      {
        /* Must be RS 2
        */
        b_copy( prim_dcch_data_ptr, 0, txtc.dcch_frame.data, 2, MUX2_FULL_PRI_LEN );
        (void) parm_inc(CAI_MUX2_REV_DCCH_1_ID, 1); // Increment primary traffic only counter
        txtc.dcch_frame.frame_cat = CAI_MUX2_REV_DCCH_1_ID;
      }
    }
  }
  #ifdef FEATURE_IS2000_REL_A_SVD
  /* signaling already sent. If RLP built nothing, DTX.
  */
  else if ((txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_DCCH) &&
           (sec_dcch_num_bits == 0))
  {
    if (sec_dcch_data_ptr)
    {
      /* RLP build nothing why is sec_dcch_ptr != NULL??
      */
      M1X_MSG( MUX, LEGACY_ERROR,
        "Sec_dcch_ptr is not NULL");
    }

    /* Force this to BLANK if we DTX.  It's used in RLP logging later.
    */
    txc_rev_fr_block.rev_arr[DCCH_BUF].fr_rate = DSRLP_RATE_BLANK;

    /* DTX DCCH frame
    */
    txc_dtx_dcch_frame();
  }
  else if (txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_DCCH)
  {
    if (sec_dcch_data_ptr == NULL)
    {
      /* already handled the case when sec_dcch_num_bits == 0
      ** why is sec_dcch_ptr == NULL??
      */
      M1X_MSG( MUX, LEGACY_ERROR,
        "Sec_dcch_ptr is NULL");
      txc_dtx_dcch_frame();
    }
    else
    {

      if ( txc_so.rev_dcch.rc == CAI_RC_3 )
      {
        /* RS 1 based
        */
        txtc.dcch_frame.data[0] = 0xF0; // 1111 xxxx
        b_copy (sec_dcch_data_ptr, 0, txtc.dcch_frame.data, 4, sec_dcch_num_bits);
        (void) parm_inc(CAI_MUX1_REV_DCCH_14_ID, 1); // Increment second traffic only counter
        txtc.dcch_frame.frame_cat = CAI_MUX1_REV_DCCH_14_ID;
      }
      else
      {
        /* Must be RS 2
        */
        txtc.dcch_frame.data[0] = 0x5C; // X101 11xx
        b_copy (sec_dcch_data_ptr, 0, txtc.dcch_frame.data, 6, sec_dcch_num_bits);
        (void) parm_inc(CAI_MUX2_REV_DCCH_9_ID, 1); // Increment secondary traffic only counter
        txtc.dcch_frame.frame_cat = CAI_MUX2_REV_DCCH_9_ID;
      }
    }
  }
  else
  {
    /* If DCCH is used ONLY as Signaling channel and
    ** no if in the current no signaling msg is pending, DTX DCCH frame.
    */
    txc_dtx_dcch_frame();
  }
  #endif // FEATURE_IS2000_REL_A_SVD

  /* Determine if encoder needs to be disabled to perform DTX.
  ** Normally DTX is done by setting DCCH gain to 0.
  ** Pilot gating is performed by HW only if encoder is disabled.
  */
  if ( txtc.gating_rate != CAI_GATING_RATE_NONE && txtc.dcch_frame.dtx )
  {
    enc_dcch_enable( FALSE );
  }
  else
  {
    enc_dcch_enable( TRUE );
  }
} /* txc_build_dcch_frame */

/*===========================================================================
FUNCTION     TXC_DTX_DCCH_FRAME

DESCRIPTION  This function does processing when a R-DCCH frame is to be DTX'd.

DEPENDENCIES None.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void txc_dtx_dcch_frame( void )
{
  txtc.dcch_frame.dtx = TRUE;

  if ( txc_so.rev_dcch.rc == CAI_RC_3 )
  {
    /* RS 1 */
    txtc.dcch_frame.frame_cat = CAI_MUX1_REV_DCCH_15_ID;
  }
  else
  {
    /* RS 2 */
    txtc.dcch_frame.frame_cat = CAI_MUX2_REV_DCCH_27_ID;
  }

  /* Increment DTX counter
  */
  (void) parm_inc(txtc.dcch_frame.frame_cat, 1);

} /* txc_dtx_dcch_frame */

/*===========================================================================

FUNCTION TXC_TRAFFIC_ISR

DESCRIPTION
  Interrupt handler for the traffic channel.  Merge the vocoder and
  signaling data and give them to the encoder.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
qword qw_frame_off;

void txc_traffic_isr
(
  m1x_stmr_event_type pcg
)
{
  qword cdma_time_in_20ms_w_frame_offset;
  word temp_pch_gain  = 0;
  word temp_fch_gain  = 0;
  word temp_dcch_gain = 0;
  word temp_sch_gain  = 0;

  #ifdef FEATURE_IS2000_1X_ADV
  word temp_fch_ratio     = 0,
       temp_fch_ack_ratio = 0;
  #endif /* FEATURE_IS2000_1X_ADV */

  int16 temp_pch_adj_gain = 0;
  byte null_fch_data [3];  /* Store 8th rate NULL frame for both rate sets */

  void (*local_action_time_notifier_func_ptr)(mctrans_server_token_type,qword);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the current cdma time in 20ms frame units with frame offset applied.
     This needs to be calculated each interrupt for action time processing.
     It is also used in RC8 to determine proper FCH frame blanking */
  m1x_time_get_20ms_frame_offset_time( cdma_time_in_20ms_w_frame_offset );

  txtc.last_encoder_interrupt_bin = pcg;

  if( ( txtc.last_encoder_interrupt_bin >= TXC_TC_SECOND_ISR_PCG ) &&
      ( txtc.last_encoder_interrupt_bin <= M1X_STMR_PCG_15 ) )
  {
    /* This is the 2nd interrupt in a frame.  Use this interrupt to set
    ** gain values and send data.
    */
    #ifndef FEATURE_MODEM_1X_UTX
    /* Toggle the Read buffer index */
    (void)muxmdsp_toggle_iram_index();
    #endif /* !FEATURE_MODEM_1X_UTX */

    /* Clear the Device only DTX structure before making DTX decision */
    txc_device_only_dtx_param.dtx_frame           = FALSE;
    txc_device_only_dtx_param.dtx_mask            = DTX_SB_MASK_DISABLED;
    txc_device_only_dtx_param.dtx_t2p_boost_dbq3  = 0;

    switch( txc_build_frame_state )
    {
      /* Frame building for current frame is finished or TXC is starting
      ** from default state.
      */
      case TXC_BUILD_FRAME_BUILT:
      case TXC_BUILD_FRAME_DONE:
      {
        /* Reset frame building status */
        txc_build_frame_state = TXC_BUILD_FRAME_DONE;

        /* Program digital gains with values calculated when the frame was
        ** built.
        */
        if ( ( txc_so.rev_fch.included ) && ( txc_so.rev_fch.rc < CAI_RC_3 ) )
        {
          /* FOR RC's 1 AND 2 ONLY */
          channel_gains.txc_pch_gain    = 0;
          channel_gains.txc_fch_gain    = TXC_IS95A_FCH_GAIN;
          channel_gains.txc_sch_gain    = 0;
          channel_gains.txc_dcch_gain   = 0;
          channel_gains.txc_tx_rate_adj = 0;

          muxmdsp_set_channel_gains();
        }
        else
        {
          /* FOR RC's 3, 4, AND 8 */
          /* Save the frame rate for logging. */
          tx_frame_rate = txtc.rate;

          #ifdef FEATURE_IS2000_1X_ADV
          if( txc_so.rev_fch.rc == CAI_RC_8 )
          {
            txc_1xA_frame_prep(qw_lo(cdma_time_in_20ms_w_frame_offset)+1);
          }
          else
          #endif /* FEATURE_IS2000_1X_ADV */
          {
            /* Clear the Smart Blanking flag for legacy RCs. */
            txtc.rc8_status.blank_non_crit_fch_frame = FALSE;
          }

          /* Make Device only DTX decision */
          #ifdef FEATURE_MODEM_1X_DEVICE_ONLY_DTX
          if( txc_device_only_dtx_is_enabled() )
          {
            txc_device_only_dtx_decision
                        (txtc.rc8_status.blank_non_crit_fch_frame);
          }

          /* Update the gain tables irrespective of DTX decision -
             This is mainly done to support R-FCH gating in FW */
          rpc_mean_code_chan_output_power();

          /* No need to update gain tables when R-SCH/R-DCCH is ON -
             R-FCH DTX will be disabled here */
          if ( !txc_so.rev_dcch.included )
          {
            /* Update correct gain tables for R-SCH DTX'd case */
            if ( sch_state == TXC_REV_SCH_DTX )
            {
              txc_set_rev_chan_gain_fch_only_sch_dtx();
            }
            /* Update correct gain tables for R-SCH OFF case */
            else if ( sch_state == TXC_REV_SCH_OFF )
            {
              txc_set_rev_chan_gain_fch_only_sch_off();
            }
          }
          #endif /* FEATURE_MODEM_1X_DEVICE_ONLY_DTX */

          /* Send FW the gain updates */
          #ifdef FEATURE_IS2000_1X_ADV
          if( txc_so.rev_fch.rc == CAI_RC_8 )
          {
            muxmdsp_set_rc8_frame_config_parms();
          }
          else
          #endif /* FEATURE_IS2000_1X_ADV */
          {
            muxmdsp_set_channel_gains();
          }
        }

        /* If DCCH is assigned, output current DCCH frame data to ENC. */
        if( txc_so.rev_dcch.included && !txtc.dcch_frame.dtx )
        {
          // Send data out, always generate CRC and always full rate for DCCH.
          // Note: this is called even when sending preambles, but when
          // sending preambles, DCCH is not enabled
          enc_dcch_data_out( txtc.dcch_frame.data, TRUE, ENC_FULL_RATE);
        }

        /* If FCH is assigned, output current FCH frame data to ENC. */
        if( txc_so.rev_fch.included )
        {
          enc_fch_data_out( txtc.frame.data, (boolean) !txtc.preambles, txtc.rate);
          muxvoc_print_voc_frame_info(txtc.rate, txtc.frame.data);
        }

        /* If SCH is assigned, output current SCH frame data to ENC. */
        #ifndef FEATURE_IS2000_REL_A
        if( txc_get_pri_so() == CAI_SO_PPP_PKT_DATA_3G )
        #else
        if( ( txc_get_pri_so() == CAI_SO_PPP_PKT_DATA_3G ) ||
            ( txc_get_sec_so() == CAI_SO_PPP_PKT_DATA_3G ) )
        #endif /* FEATURE_IS2000_REL_A */
        {
          txc_tx_frame_proc_send_data3g(&txc_rev_fr_block);
        }
        break;
      }

      case TXC_BUILD_FRAME_IN_PROGRESS:
      case TXC_BUILD_FRAME_LATE:
      {
        /* Frame building for current frame is not yet complete.  This is
        ** unexpected but TXC must handle this situation.  Set build frame
        ** status to LATE.
        */
        txc_build_frame_state = TXC_BUILD_FRAME_LATE;

        /* Program digital gains but do not use the gain values calculated for
        ** the current frame since the current frame is late.  Always DTX
        ** DTX DCCH and SCH by setting the channel digital gains to 0.
        */
        temp_sch_gain  = 0;
        temp_dcch_gain = 0;

        if( ( txc_so.rev_fch.included ) && ( txc_so.rev_fch.rc < CAI_RC_3 ) )
        {
          /* Set digital gains and TX rate adjust to IS95A/B configuration. */
          temp_pch_gain     = 0;
          temp_fch_gain     = TXC_IS95A_FCH_GAIN;
          temp_pch_adj_gain = 0;
        }
        else
        {
          /* Set digital gains and TX rate adjust to IS95C configuration,
          ** depending on channel assignment.
          */
          if ( txc_so.rev_fch.included )
          {
            #ifdef FEATURE_IS2000_1X_ADV

            /*  For 1x Advanced RC's set the linear gain ratios */

            if( txc_so.rev_fch.rc == CAI_RC_8 )
            {
              /* Set linear gain ratios for 1/8th rate frame */
              uint8 acs = TXC_GET_ACTIVE_CELL_SIZE( srch_mux_get_active_cell_size() );
              temp_fch_ratio = txc_fch_ack_gain_ratio_tab[ ENC_8TH_RATE ][ acs ].fch_gain_ratio;
              temp_fch_ack_ratio = txc_fch_ack_gain_ratio_tab[ ENC_8TH_RATE ][ acs ].fch_ack_gain_ratio;
            }
            else
            #endif /* FEATURE_IS2000_1X_ADV */
            {
              /* Set digital gains and TX rate adjust for 8th rate FCH frame. */
              temp_pch_gain = txc_pch_fch_gain_tab[ ENC_8TH_RATE ].pch_gain;
              temp_fch_gain = txc_pch_fch_gain_tab[ ENC_8TH_RATE ].fch_gain;
            }
          }
          else
          {
            /* FCH is not assigned and the rest of the channel configuration
            ** can be DTX'ed, so total digital gain is given to PCH.  The 15
            ** bit resolution of PCH gain requires the left shift by 7.
            */
            temp_pch_gain = RPC_TOT_DIG_PWR_255 << 7;
            temp_fch_gain = 0;
          }

          #ifdef FEATURE_IS2000_1X_ADV
          if( txc_so.rev_fch.rc != CAI_RC_8 )
          #endif /* FEATURE_IS2000_1X_ADV */
          {
            /* Calculate TX_RATE_ADJUST. */
            temp_pch_adj_gain =
              rpc_get_pch_adj_gain(PCH_GAIN_TRUNCATE_TO_8BITS(temp_pch_gain))
                + txtc.offset_pwr;
          }
        }

        #ifdef FEATURE_IS2000_1X_ADV

        /* For RC8, the mDSP uses linear gain ratios and not digital gains.
           NOTE: DCCH is not supported. Write 0 by default.
           NOTE: There is no PICH ratio or TX rate adjust either.
           NOTE: SCH is DTX'd in this case. Write 0 for the SCH and SCH-ACK ratios.
        */
        if( txc_so.rev_fch.rc == CAI_RC_8)
        {
          rc8_frame_config_parms.fch_linear_gain_ratio    = temp_fch_ratio;
          rc8_frame_config_parms.fch_ack_linear_gain_ratio= temp_fch_ack_ratio;

          /* These are DTX'd in a late frame */
          rc8_frame_config_parms.dcch_linear_gain_ratio    = 0;
          rc8_frame_config_parms.sch_linear_gain_ratio     = 0;
          rc8_frame_config_parms.sch_ack_linear_gain_ratio = 0;
          rc8_frame_config_parms.blanked_frame             = FALSE;
          rc8_frame_config_parms.rpc_mode = txtc.rc8_status.pwr_ctrl_mode;

          muxmdsp_set_rc8_frame_config_parms();
          muxmdsp_set_tx_gain_adj_step_size( txtc.rc8_status.pwr_ctrl_step_size );
        }
        else
        #endif /* FEATURE_IS2000_1X_ADV */
        {
          /* Output digital gains and TX_RATE_ADJUST. */
          channel_gains.txc_pch_gain    = temp_pch_gain;
          channel_gains.txc_fch_gain    = temp_fch_gain;
          channel_gains.txc_sch_gain    = temp_sch_gain;
          channel_gains.txc_dcch_gain   = temp_dcch_gain;
          channel_gains.txc_tx_rate_adj = temp_pch_adj_gain;

          muxmdsp_set_channel_gains();
        }

        /* If FCH is assigned, output null FCH frame data to ENC. */
        if( txc_so.rev_fch.included )
        {
          if( txc_get_rev_link_rate_set( txc_so.rev_fch.rc ) == ENC_RATE_14400 )
          {
            /* Put null primary traffic (20 bits) into an 1800 bps frame.
            ** Make sure to account for the Erasure bit and MM bit.
            */
            null_fch_data[0] = 0x3F;
            null_fch_data[1] = 0xFF;
            null_fch_data[2] = 0xFC;
          }
          else
          {
            /* Put null primary traffic (16 bits) into an 1200 bps frame.
            ** Rate set 1 does not have an Erasure bit or a MM bit.
            */
            null_fch_data[0] = 0xFF;
            null_fch_data[1] = 0xFF;
          }
          enc_fch_data_out (null_fch_data, TRUE, ENC_8TH_RATE);

        } /* end if ( txc_so.rev_fch.included ) */
        break;
      }

      default:
        /* Only get here if the state setting is out of range due to error.
        ** Reset it to the default setting.
        */
        txc_build_frame_state = TXC_BUILD_FRAME_DONE;
        break;

    } /* end switch (txc_build_frame_state) */

    /* Send RliRC info if it was deferred for SCR transactions/HHO. */
    if( txc_tx.is_rc_update_deferred )
    {
      /* Check if the REV FCH is included or not. */
      if( txc_so.rev_fch.included )
      {
        muxmdsp_configure_rl_rc( txc_so.rev_fch.rc );
      }

      /* Clear the flag here. */
      txc_tx.is_rc_update_deferred = FALSE;
    }

    #ifndef FEATURE_MODEM_1X_UTX
    /* Tell FW to commit shared memory cfg and data is ready in LMEM */
    muxmdsp_send_frame_config();
    #endif /* FEATURE_MODEM_1X_UTX */

    return;
  }

  /* Get here if this is the 1st interrupt in a frame.  This interrupt is
  ** the main TXC interrupt which kicks off frame processing among other
  ** things.
  */

  /* Every frame, give the current time to the transaction
  ** engine.  If there is an action for that frame, the
  ** transaction engine will set TXC_ACTION_TIME_SIG
  ** within the interrupt context.
  **
  ** Use function pointer to call the transaction engine.
  ** This function must execute or TXC will not process any
  ** action.
  **
  ** Also, note that TXC action time handling is being set
  ** to take place in Bin 0, PCG 1 of the frame (for Data calls).
  */
  local_action_time_notifier_func_ptr = txc_action_time_notifier_func_ptr;
  if( local_action_time_notifier_func_ptr != NULL )
  {
    /* Reset the signal. */
    (void) rex_clr_sigs (TX_TCB_PTR, TXC_ACTION_TIME_SIG );
    (*local_action_time_notifier_func_ptr)(MCTRANS_SERVER_TXC, cdma_time_in_20ms_w_frame_offset);
  }

  /* So here, check for the signal.  If it is set, call the transaction
  ** engine dispatcher to handle the action.  Action handlers have been
  ** registered to the transaction engine at power up.
  **
  ** Note that there is a plan to move the action time handling to task
  ** context in the future.
  */
  if( ( rex_get_sigs( TX_TCB_PTR ) ) & TXC_ACTION_TIME_SIG )
  {
    mctrans_process_transactions( MCTRANS_SERVER_TXC );

    /* Reset the signal. */
    (void) rex_clr_sigs( TX_TCB_PTR, TXC_ACTION_TIME_SIG );
  }

  /* Start frame processing for the current frame, only if the previous
  ** frame has been sent.  If the previous frame is late, do not build a
  ** new frame so TXC can catch up.  Late frames are the result of TXC being
  ** starved so the build frame process spans more than 1 frame.  It is
  ** desirable to skip building new frames until TXC catches up, rather than
  ** sending a continuous group of late frames which BS may not received.
  ** This is consider error recovery for TXC, not normal operation.
  */
  if (txc_build_frame_state == TXC_BUILD_FRAME_DONE)
  {
    #ifdef FEATURE_VOICE_AGENT_ARCHITECTURE
    muxvoc_get_voc_frame();
    #endif

    txc_voc_xchg();                /* Reads vocoder encoder bits */
  }

  txc_tx.int_cnt++;                   /* count tx interrupts */

  /* count "after_fo_handoff_count_isrs" number of encoder interrupt for frame
     recalulation from system time after frame offset handoff. After those isrs,
     just increment frame cnt */
  if( after_fo_handoff_count_isrs > 0 )
  {
      after_fo_handoff_count_isrs--;

      /* Derive frame from timestamp>>22 */
      (void) m1x_time_get ( txc_tx.frame_cnt );

      /* Divide by 64*1024 to truncate the low 16 bits of the time stamp,
      ** then divide the 1.25 ms units by 16 to get 20ms units.
      */
      (void) qw_div_by_power_of_2( txc_tx.frame_cnt, txc_tx.frame_cnt, 16 );
      qw_set ( qw_frame_off, 0L, txc_frame_offset);
      qw_sub( txc_tx.frame_cnt, txc_tx.frame_cnt, qw_frame_off );
      (void) qw_div_by_power_of_2( txc_tx.frame_cnt, txc_tx.frame_cnt, 4 );
  }

  /* Increment frame count for correct TX frame count */
  qw_inc( txc_tx.frame_cnt, 1L );

  /* Signal tx task to begin operation. */
  (void)rex_set_sigs ( TX_TCB_PTR, TXC_INT_SIG );

  /* For a RC3/4 hard handoff, after sending the preambles,
     we need to enable the FCH again */
  if( ( txtc.preambles && txtc.num_preambles == 0 ) &&
      ( ( txc_so.rev_dcch.included ) ||
        ( txc_so.rev_fch.rc >= CAI_RC_3 ) ) )
  {
    txtc.preambles = FALSE;
    if ( txc_so.rev_fch.included )
    {
      enc_fch_enable( TRUE );
    }
    enc_gating_mode( ENC_NO_GATING_MODE );
    enc_is95c_tx_patn( ENC_NO_GATING_PATTERN );
    M1X_MSG( MUX, LEGACY_HIGH,
      "Stop PCG preamble");
  }

  /* update modem power stats */
  txc_update_power_stats();

} /* txc_traffic_isr */

/*===========================================================================

FUNCTION TXC_STOP_TX

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_stop_tx( boolean hho )
{
  boolean ta_is_enabled = txc_is_ta_enabled();

  M1X_MSG( MUX, LEGACY_HIGH,
    "Stop TXC transmitting at %lx,hho is %d",
    txc_get_time_in_1_25ms_unit(),
    hho );

  /* Stop TX */
  txc_change_rf_tx_state( TXC_RF_STATE_STOP );

  /* Zero out all digital gains when turning transmitter off for HHO */
  if( hho )
  {
    channel_gains.txc_pch_gain = 0;
    channel_gains.txc_fch_gain = 0;
    channel_gains.txc_sch_gain = 0;
    channel_gains.txc_dcch_gain = 0;

    /* Following a HHO we want to wait this number of frames
       before sending out any signaling to allow the BS to
       acquire our RL signal. */
    txtc.sig_frame_delay = txc_sig_frame_delay_value;

    txtc.quick_repeat_frame_cnt = txc_sig_quick_repeat_value;
  }

  channel_gains.txc_tx_rate_adj = txtc.offset_pwr;

  /* Disable DTX while stopping TX */
  txc_device_only_dtx_param.dtx_frame = FALSE;
  txc_device_only_dtx_param.dtx_mask  = DTX_SB_MASK_DISABLED;

  /* Set the channel gains */
  muxmdsp_set_channel_gains();

  txc_tx.tx_on = FALSE;

  /* set up the rxc flag so that ffpc will not update the setpoint */
  rxc_set_tx_in_traffic_on( FALSE );

  /* Events report: Transmitter disabled */
  event_report( EVENT_TRANSMITTER_DISABLED );

  //FUTURE: TXC_ENTER_ISR_SECTION();
  if( txc_tx.active_msg )
  {
    /* we were in the middle of the message - send it again */
    txc_tx.buf.pos = 0;
  }
  //FUTURE: TXC_LEAVE_ISR_SECTION();

  /* If WTR Hop based inter band HHO/CFS is in progress, then dont get Tx Adj
     from new device. Use older one saved while TXC_EXIT_F is sent.
     If TA GAP is open then store Tx Adj */
  if( ta_is_enabled ||
      ( !mcc_is_hho_with_wtr_hopping_in_prog()   &&
        !mcc_is_cfs_with_wtr_hopping_in_prog() )
    )
  {
    txtc.power = srch_mux_get_tx_adj(); /* save power */
  }

  /* Print power saved in 1/2 dB units */
  M1X_MSG( MUX, LEGACY_HIGH,
     "1X TX STOP: TX ADJUST (Saved) TxAdjPwr=%d (-1/2dB), OffsetPwr=%d (1/2dB)",
     txtc.power,
     (txtc.offset_pwr / 6) );

  /* End of call, stop RLP logging */
  dsrlp_log_tx_send_frames();

} /* txc_stop_tx */

/*===========================================================================

FUNCTION TXC_RPCTRL_SEND_LOG

DESCRIPTION
  Terminate and send the accumulated reverse power control frames log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A reverse power control log may be sent.

===========================================================================*/
void txc_rpctrl_send_log( void )
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( txc_log_rpctrl_buf.hdr.len != 0 )
  {
    /* Will send log if enabled. */
    (void) log_submit( &txc_log_rpctrl_buf );

    /* Flag to init start of log */
    txc_log_rpctrl_buf.hdr.len = 0;
  }

} /* txc_rpctrl_send_log */

/*===========================================================================

FUNCTION TXC_RPCTRL_LOG_INIT

DESCRIPTION
  This function initializes the Reverse Power Control log subrecord, as well as the
  pointer to it and the frame entry, in the log buffer.  If necessary, the log is
  sent and a new one is generated.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A reverse power control log may be sent.

===========================================================================*/
mux_log_rev_pctrl_frame_type * txc_rpctrl_log_frame_init( void )
{
  mux_log_rev_pctrl_frame_type *frm_ptr = NULL;
  mux_log_ded_rev_chan_set_type chan_set_mask;
  static boolean txc_sch_log_data = FALSE;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If logging is enabled. */
  if( log_status( LOG_REV_PCTRL_C ) )
  {
    /* If we reached accumulation threshold, send it. */
    if( txc_log_rpctrl_buf.hdr.len >= MUX_LOG_REV_PCTRL_BUF_THRESHOLD )
    {
      txc_rpctrl_send_log();
    }

    if (txc_log_rpctrl_buf.hdr.len == 0) /* Need to re-init */
    {
      txc_log_rpctrl_buf.hdr.len = FPOS(mux_log_rev_pctrl_type, var_len_buf);//lint !e734 !e413 FPOS

      txc_log_rpctrl_buf.hdr.code = LOG_REV_PCTRL_C;

      /* First frame, log time stamp. */
      (void) m1x_time_get(txc_log_rpctrl_buf.hdr.ts);

      /*------------------------------
        Fill in beginning of record
      ------------------------------*/
      /* Frame offset specified by base station */
      txc_log_rpctrl_buf.frame_offset = txc_frame_offset;

      txc_log_rpctrl_buf.band_class = cdma.band_class; /* From MC */

      /* RC for each active channel */
      if ( txc_so.rev_fch.included )
      {
        txc_log_rpctrl_buf.rev_ch_rc.r_fch_rc  = txc_so.rev_fch.rc;
      }
      else
      {
        txc_log_rpctrl_buf.rev_ch_rc.r_fch_rc  = 0;
      }

      if ( txc_so.rev_dcch.included )
      {
        txc_log_rpctrl_buf.rev_ch_rc.r_dcch_rc  = txc_so.rev_dcch.rc;
      }
      else
      {
        txc_log_rpctrl_buf.rev_ch_rc.r_dcch_rc  = 0;
      }

      if ( txtc.sch_on )
      {
        txc_log_rpctrl_buf.rev_ch_rc.r_sch0_rc = txtc.r_sch_rc;
      }
      else
      {
        txc_log_rpctrl_buf.rev_ch_rc.r_sch0_rc = 0;
      }

      txc_log_rpctrl_buf.rev_ch_rc.r_sch1_rc = 0; /* Not supported! */

      /* Pilot gating rate not used until the control hold state is supported */
      txc_log_rpctrl_buf.pilot_gating_rate = 0;

      /* Power Control step size */
      txc_log_rpctrl_buf.step_size = txtc.pwr_ctrl_step_size;

      /* We no longer have a valid sub-record. */
      txc_log_rpctrl_buf.sr_cnt = 0;
      txc_log_rpctrl_sr_ptr     = NULL;

    }
    else
    {
      /* If a new subrecord is needed, i.e. the channel set has changed,
      ** check for the condition here.
      */
      chan_set_mask.mask = 0;

      chan_set_mask.chan_set.r_dcch = FALSE;
      chan_set_mask.chan_set.r_fch  = FALSE;

      if( txc_so.rev_dcch.included )
      {
        chan_set_mask.chan_set.r_dcch = TRUE;
      }

      if( txc_so.rev_fch.included )
      {
        chan_set_mask.chan_set.r_fch  = TRUE;
      }

      /* Check if SCH is active. */
      chan_set_mask.chan_set.r_sch0 = (txc_sch_log_data) ? TRUE : FALSE;

      if( chan_set_mask.mask != txc_log_rpctrl_sr_ptr->ch_set_mask.mask )
      {
        /* End this sub-record and start a new one below. */
        txc_log_rpctrl_sr_ptr = NULL;
      }
    }

    /* Need to init a new subrecord? */
    if( txc_log_rpctrl_sr_ptr == NULL )
    {
      /* Init sub-record pointer by finding the offset into the variable length buf.
      ** TXC_RPCTRL_LOG_OFFSET is calculated from the length of the log packet.
      */
      /* See structure definition for explaination of buf_ref */
      txc_log_rpctrl_sr_ptr = (mux_log_rev_pctrl_subrecord_type *)
        &txc_log_rpctrl_buf.var_len_buf.buf_ref[TXC_RPCTRL_LOG_OFFSET]; //lint !e413 FPOS inside macro

      /* Increment sub-record count. */
      txc_log_rpctrl_buf.sr_cnt++;

      /* Init all bits in mask, and len_per_frame */
      txc_log_rpctrl_sr_ptr->ch_set_mask.mask = 0;
      txc_log_rpctrl_sr_ptr->len_per_frame =
        FPOS(mux_log_rev_pctrl_frame_type, tx_ch_pwr);//lint !e734 FPOS

      txc_log_rpctrl_sr_ptr->ch_set_mask.chan_set.r_dcch = FALSE;
      txc_log_rpctrl_sr_ptr->ch_set_mask.chan_set.r_fch  = FALSE;

      if( txc_so.rev_fch.included )
      {
        txc_log_rpctrl_sr_ptr->ch_set_mask.chan_set.r_fch  = TRUE;

        /* Increase length per frame value. */
        txc_log_rpctrl_sr_ptr->len_per_frame +=
          FSIZ(mux_log_rev_pctrl_frame_type, tx_ch_pwr[0]);
      }

      if( txc_so.rev_dcch.included )
      {
        txc_log_rpctrl_sr_ptr->ch_set_mask.chan_set.r_dcch = TRUE;

        /* Increase length per frame value. */
        txc_log_rpctrl_sr_ptr->len_per_frame +=
          FSIZ(mux_log_rev_pctrl_frame_type, tx_ch_pwr[0]);
      }

      if( txc_sch_log_data )
      {
        /* R-SCH0 is active. */
        txc_log_rpctrl_sr_ptr->ch_set_mask.chan_set.r_sch0 = TRUE;

        /* Increase length per frame value. */
        txc_log_rpctrl_sr_ptr->len_per_frame +=
          FSIZ(mux_log_rev_pctrl_frame_type, tx_ch_pwr[0]);
      }

      /* Init frame count */
      txc_log_rpctrl_sr_ptr->frame_cnt = 0;

      /* Update packet length */
      txc_log_rpctrl_buf.hdr.len += FPOS(mux_log_rev_pctrl_subrecord_type, var_len_buf);//lint !e734 !e413 FPOS

    }

    /* Increment frame count */
    txc_log_rpctrl_sr_ptr->frame_cnt++;

    /* Init frame pointer by finding the offset into the variable length buf.
    ** TXC_RPCTRL_LOG_OFFSET is calculated from the length of the log packet.
    */
    frm_ptr = (mux_log_rev_pctrl_frame_type *)
      &txc_log_rpctrl_buf.var_len_buf.buf_ref[TXC_RPCTRL_LOG_OFFSET]; //lint !e413 FPOS inside macro
  }
  else
  {
    txc_log_rpctrl_buf.hdr.len = 0; /* Flag that this record has stopped */
  }
  /* reset the flag for the last TX frame of the call */
  txc_sch_log_data  = txtc.sch_on;
  return frm_ptr;

} /* txc_rpctrl_log_frame_init */

/*=========================================================================
FUNCTION TXC_RPCTRL_LOG_FRAME

DESCRIPTION
  This function populates FFPC frame logs.

DEPENDENCIE
  None.

RETURN VALUE

SIDE EFFECTS
  None.
=========================================================================*/
void txc_rpctrl_log_frame( byte num_frames )
{
  unsigned int idx; /* Setpoint array index */
  mux_log_rev_pctrl_frame_type *frm_ptr;
  txc_chan_status_type sch_chan_status  = TXC_SCH_DTX;
  txc_chan_status_type dcch_chan_status = TXC_DCCH_DTX;
  static int16 curr_frame_fch_to_pich = 0,  next_frame_fch_to_pich = 0;
  static int16 curr_frame_dcch_to_pich = 0, next_frame_dcch_to_pich = 0;
  static int16 curr_frame_sch0_to_pich = 0, next_frame_sch0_to_pich = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  frm_ptr = txc_rpctrl_log_frame_init();

  if( frm_ptr != NULL )
  {
    /* Record history */
    frm_ptr->dec_history = rxc_bufs_qs_data.rxc_rpc_history;

    /* Record AGC vals */
    frm_ptr->rx_agc = log_rx_agc;
    frm_ptr->tx_pwr = log_tx_pwr;
    frm_ptr->tx_gain_adj = log_tx_gain_adj;

    if( (
         #ifndef FEATURE_IS2000_REL_A
         ( txc_get_pri_so() == CAI_SO_PPP_PKT_DATA_3G ) &&
         #else
         (( txc_get_pri_so() == CAI_SO_PPP_PKT_DATA_3G ) ||
          ( txc_get_sec_so() == CAI_SO_PPP_PKT_DATA_3G )) &&
         #endif /* FEATURE_IS2000_REL_A */
         ( num_frames > SCH_BUF_START)
        )
        ||
        (
          #ifndef FEATURE_IS2000_REL_A
          (( txc_get_pri_so() == CAI_SO_TDSO ) ||
           ( txc_get_pri_so() == CAI_SO_FULL_TDSO )) &&
          #else
          (( txc_get_pri_so() == CAI_SO_TDSO ) ||
           ( txc_get_sec_so() == CAI_SO_TDSO ) ||
           ( txc_get_pri_so() == CAI_SO_FULL_TDSO ) ||
           ( txc_get_sec_so() == CAI_SO_FULL_TDSO )) &&
          #endif /* FEATURE_IS2000_REL_A */
          ( num_frames > 0 )
        )
      )
    {
      sch_chan_status = TXC_SCH_ACTIVE;
    }

    if( txc_so.rev_dcch.included && !txtc.dcch_frame.dtx )
    {
      dcch_chan_status = TXC_DCCH_ACTIVE;
    }

    /* note the FCH_TO_PICH, SCH0_TO_PICH ratios for
     * the frame being currently transmitted.
     */
    curr_frame_fch_to_pich  = next_frame_fch_to_pich;
    curr_frame_dcch_to_pich = next_frame_dcch_to_pich;
    curr_frame_sch0_to_pich = next_frame_sch0_to_pich;

    /* get the FCH_TO_PICH, SCH0_TO_PICH, DCCH_TO_PICH ratios for
     * the frame being built.
     */
    rpc_get_relative_gains( &next_frame_fch_to_pich,
                            &next_frame_dcch_to_pich,
                            &next_frame_sch0_to_pich,
                            txtc.rate, sch_chan_status, dcch_chan_status );

    /* Start recording per channel data */
    idx = 0; /* Initialize setpoint array index */

    if (txc_log_rpctrl_sr_ptr->ch_set_mask.chan_set.r_fch == 1)
    {
      frm_ptr->tx_ch_pwr[idx++].r_fch_pich = curr_frame_fch_to_pich;
    }

    if (txc_log_rpctrl_sr_ptr->ch_set_mask.chan_set.r_sch0 == 1)
    {
      frm_ptr->tx_ch_pwr[idx++].r_sch0_pich = curr_frame_sch0_to_pich;
    }

    if (txc_log_rpctrl_sr_ptr->ch_set_mask.chan_set.r_dcch == 1)
    {
      frm_ptr->tx_ch_pwr[idx++].r_dcch_pich = curr_frame_dcch_to_pich;
    }

    /* TODO: Add other channels here! */

    /* end per channel data */

    /* Update packet length */
    txc_log_rpctrl_buf.hdr.len += txc_log_rpctrl_sr_ptr->len_per_frame;

  } /* If logging enabled */
} /* txc_rpctrl_log_frame */

#ifdef FEATURE_MODEM_1X_DRX
/*===========================================================================

FUNCTION TXC_SET_FRAME_OFF_IN_PROGRESS

DESCRIPTION
  This function sets variable which indicates if frame offset change
  at FW level is in progress or not

DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void txc_set_frame_off_in_progress( boolean offset_change )
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  txc_frame_offset_change_in_progress = offset_change;

  M1X_MSG( MUX, LEGACY_HIGH,
    "Set Frame off change in progress %d",
    txc_frame_offset_change_in_progress );

} /* txc_set_frame_off_in_progress */

/*===========================================================================

FUNCTION TXC_GET_FRAME_OFF_IN_PROGRESS

DESCRIPTION
  This function gets variable which indicates if frame offset change
  at FW level is in progress or not.

DEPENDENCIES
  None

RETURN VALUE
  True if offset change in progress false othewise.

SIDE EFFECTS
  None

===========================================================================*/
boolean txc_get_frame_off_in_progress( void )
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  M1X_MSG( MUX, LEGACY_LOW,
    "Get Frame off change in progress %d",
    txc_frame_offset_change_in_progress );

  return txc_frame_offset_change_in_progress;

} /* txc_get_frame_off_in_progress */
#endif /* FEATURE_MODEM_1X_DRX */

/*===========================================================================

FUNCTION TXC_IS_TA_ENABLED

DESCRIPTION
  Indicates if TA GAP is open or not

DEPENDENCIES
  None

RETURN VALUE
  Is TA GAP open or not

SIDE EFFECTS
  None

===========================================================================*/
boolean txc_is_ta_enabled( void )
{
  boolean tx_gap_is_active        = FALSE;
  boolean rx_gap_is_active        = FALSE;
  static boolean ta_gap_is_active = FALSE;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  tx_gap_is_active = txc_ta_info.ta_start && ( !txc_ta_info.ta_comp );
  rx_gap_is_active = rxc_is_ta_enabled();

  if( tx_gap_is_active || rx_gap_is_active )
  {
    if( ta_gap_is_active != TRUE )
    {
      ta_gap_is_active = TRUE;

      M1X_MSG( MUX, LEGACY_HIGH,
         "1x2GTA: Tx Gap is %d, Rx Gap is %d, TA Gap is ON",
         tx_gap_is_active,
         rx_gap_is_active );
    }
  }
  else
  {
    if( ta_gap_is_active != FALSE )
    {
      ta_gap_is_active = FALSE;

      M1X_MSG( MUX, LEGACY_HIGH,
         "1x2GTA: Tx Gap is %d, Rx Gap is %d, TA Gap is OFF",
         tx_gap_is_active,
         rx_gap_is_active );
    }
  }

  return ta_gap_is_active;

} /* txc_is_ta_enabled */

/*===========================================================================

FUNCTION TXC_TRAFFIC

DESCRIPTION
  Process traffic channel state

DEPENDENCIES
  None

RETURN VALUE
  Next state.

SIDE EFFECTS
  None

===========================================================================*/
static mctrans_action_packet_type pkt; /* Needed for SCR transaction */
txc_state_type txc_traffic
(
  txc_state_type state                  /* txc_state - TXC_TRAFFIC_S */
)
{
  txc_cmd_type           *cmd_ptr;             /* command from queue */
  rex_sigs_type          sigs;                 /* task signals */
  word                   len;                  /* calculate message length */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  (void)rex_clr_sigs( TX_TCB_PTR, TXC_INT_SIG );

  /* We have an interrupt handler for the traffic channel to take care of */
  /* feeding the actual data to the encoder                              */
  /* now, act on commands on the tx command queue */
  do
  {
    sigs = rex_get_sigs( TX_TCB_PTR ); /* Get the current signals */

    /* handle interrupt  */
    if( sigs & TXC_INT_SIG )
    {
      (void)rex_clr_sigs( TX_TCB_PTR, TXC_INT_SIG );

      txc_next_int_occurred = TRUE;

      /* Start frame processing for the current frame, only if the previous
      ** frame has been sent.  If the previous frame is late, do not build a
      ** new frame so TXC can catch up.  Late frames are the result of TXC being
      ** starved so the build frame process spans more than 1 frame.  It is
      ** desirable to skip building new frames until TXC catches up, rather than
      ** sending a continuous group of late frames which BS may not received.
      ** This is consider error recovery for TXC, not normal operation.
      */
      if( txc_build_frame_state == TXC_BUILD_FRAME_DONE )
      {
        txc_build_frame_state = TXC_BUILD_FRAME_IN_PROGRESS;

        #ifdef FEATURE_IS2000_REL_A_SVD
        /* process secondary traffic type */
        if( !txtc.preambles )
        {
          txc_sec();
        }
        #endif // FEATURE_IS2000_REL_A_SVD

        /* When the conv enc ISR occurs, it sets TXC_INT_SIG signal and depending on
        ** this signal the tx frame processing starts in the background. If minimize
        ** ISR loading feature is defined then TXC_PRI which runs in ISR context does
        ** only vocoder reading and keeps the voice in a buffer. That voice is muxed
        ** with signalling and written to the convolutional encoder in this function.
        **/
        txc_tx_frame_proc();

        /* Finish frame processing for transmit task.  As the result, a RL frame is
        ** now ready for transmission.  If the ENC ISR already occured, it's too late
        ** to send data this frame.
        ** In both cases (late or normal), set the state to BUILT.
        */
        if( txc_build_frame_state == TXC_BUILD_FRAME_LATE )
        {
          M1X_MSG( MUX, LEGACY_HIGH,
            "Late frame detected at end of TX task" );
        }
        else if( txc_build_frame_state != TXC_BUILD_FRAME_IN_PROGRESS )
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "Unexpected build state=%d at end of TX task",
            txc_build_frame_state );
        }
        txc_build_frame_state = TXC_BUILD_FRAME_BUILT;
      }
      else if( txc_build_frame_state == TXC_BUILD_FRAME_BUILT )
      {
        M1X_MSG( MUX, LEGACY_HIGH,
          "Previous frame finished late, send in this frame" );
      }
      else
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "Unexpected build state=%d at start of TX task",
          txc_build_frame_state );
      }
    } /* handle interrupt */

    (void) rex_clr_sigs( TX_TCB_PTR, TXC_CMD_Q_SIG ); /* clear queued signal */

    /* get items off the command queue */
    while( ( cmd_ptr = (txc_cmd_type*) q_get( &txc_cmd_q ) ) != NULL )
    {
      /* clear the command queued signal - if there is another one we will */
      /* get it on the next interrupt */
      (void)rex_clr_sigs( TX_TCB_PTR, TXC_CMD_Q_SIG );

      /* interpret the command from the queue */
      switch( cmd_ptr->hdr.command )
      {
        case TXC_MSG_F:            /* send a traffic message */
          M1X_MSG( MUX, LEGACY_MED,
            "Got TXC_MSG_F");

          if( !txc_tx.active_msg ) /* get a free buffer */
          {
            txc_tx.buf.cmd_ptr = cmd_ptr; /* save command info */
            txc_tx.buf.blank = FALSE;  /* dim and burst */

            if( ( txc_get_pri_so() == CAI_SO_NULL ) ||
                DS_ISDATA( txc_get_pri_so() ) ) /* All data SOs */
            {
              txc_tx.buf.blank = TRUE;   /* blank and burst */
            }

            if( txtc.preambles || txtc.nulls )
            {
              /* if preambles or null, force B&B */
              txc_tx.buf.blank = TRUE;
            }

            #ifdef FEATURE_IS2000_1X_ADV
            if( ( txc_so.rev_fch.rc == CAI_RC_8 ) &&
                  CAI_IS_SO_SIGNALING( txc_get_pri_so() ) )
            {
              txc_tx.buf.blank = TRUE;   /* blank and burst */
            }
            #endif /* FEATURE_IS2000_1X_ADV */

            txc_tx.buf.pos = 0;         /* start at the beginning */
            len = cmd_ptr->msg.len;   /* get length  */

            /* Perform a sanity check on the length of the message */
            /* Not that the len is in the units of bits            */
            ASSERT(len <= (CAI_REV_TC_MSG_SIZE  - (CAI_TC_CRC_SIZE + CAI_TC_LGTH_SIZE)));

            //The old way, unchanged since IS_95A.
            txc_tx.buf.msg[ (len>>3 ) + 1 ] = 0; /* pad end with 0's */

            /* copy the actual message */
            b_copy( cmd_ptr->msg.msg_ptr, 0,       /* source */
              &txc_tx.buf.msg[1], 0, len);   /* destination,
                                                leave room for len */
            len = len+16+8;
            if( len%8 )
            {         /* pad length out to byte boundary */
              len = (len & ~7 ) + 8;
            }

            /* convert to bytes */
            txc_tx.buf.msg[0] = ( byte )( len>>3 );

            txc_tx.buf.len = len;

            b_packw(
              crc_16_calc( txc_tx.buf.msg, len-16 ),  /* calculate CRC */
              txc_tx.buf.msg, len-16, 16              /* add to message */
            );
            txtc.dim_delay = 3;
            TXC_ENTER_ISR_SECTION();
            txc_tx.active_msg = TRUE;
            txc_traffic_signext ();

            #ifdef FEATURE_IS2000_REL_B
            /* Grant the confirmation_type requested by rxtx */
            txc_tx.confirmation_type = cmd_ptr->msg.confirmation_type;
            #endif /* FEATURE_IS2000_REL_B */

            TXC_LEAVE_ISR_SECTION();

            /* Mark that cnf needs to be sent to RXTX task for current mesage.
             * If TX task does not send cnf for this message to RXTX task while
             * exiting traffic state, RXTX task may get stuck since it will
             * think that previous message transmission is still in progress.
             * This is especially required for WTR hopping scenarios where TX
             * task is de-activated while it's transmitting a message whereas
             * RXTX task is active throughout the hop procedure.
             */
            txc_need_to_send_msg_rsp = TRUE;
          }
          else
          {
            /* couldn't get a free buffer for the message! */
            txc_cmd_done( cmd_ptr, TXC_ERR_S ); /* dispose of command buffer */
            M1X_MSG( MUX, LEGACY_ERROR,
              "No tx buffers" );

            /* Mark that cnf has been sent to RXTX task for current message and
             * there is no need to send it while exiting from traffic state */
            txc_need_to_send_msg_rsp = FALSE;
          }
          break;

        case TXC_NULLS_F:          /* Transmit null traffic data */
          M1X_MSG( MUX, LEGACY_MED,
            "Got TXC_NULLS_F" );

          txtc.nulls = TRUE;
          txtc.preambles = FALSE;
          //
          // preambles done; turn FCH back on
          //
          if( txc_so.rev_fch.included )
          {
            enc_fch_enable( TRUE );
          }
          txc_cmd_done( cmd_ptr, TXC_DONE_S ); /* dispose of command buffer */
          break;

        case TXC_EXIT_F:
          M1X_MSG( MUX, LEGACY_MED,
            "Got TXC_EXIT_F" );

          /* ----------------------------------------------------
          ** Turn off the SCH again in case a SCH transaction start
          ** arrives after we process TXC_RELEASE_F.
          ** ---------------------------------------------------- */
          txtc.sch_on = FALSE;

          /* Clear MUX-RLP interface variables */
          txc_rev_rlp_params_reset();

          if( SRCH_TA_LTA == rxc_ta_info.ta_rsn )
          {
            txc_ta_info.lta_ol_tx_pwr  = muxmdsp_get_open_loop_tx_power();
            txc_ta_info.ta_start       = TRUE;
            txc_ta_info.ta_comp        = FALSE;
            txc_ta_info.lta_start_sclk = timetick_get();

            /* Time sclk logging */
            txc_ta_info.ta_start_sclk  = timetick_get();

            M1X_MSG( MUX, LEGACY_MED,
               "1x2GTA: Current time %d, Saved OL Tx power %d",
               txc_ta_info.lta_start_sclk,
               txc_ta_info.lta_ol_tx_pwr );

            /* End of call, stop RLP logging */
            dsrlp_log_tx_send_frames();
          }
          else
          {
            txc_ta_info.ta_start       = FALSE;
            txc_ta_info.ta_comp        = TRUE;
            txc_ta_info.lta_start_sclk = 0;
            txc_ta_info.lta_ol_tx_pwr  = 0;

            /* Time sclk logging */
            txc_ta_info.ta_start_sclk      = 0;
          }

          txc_ta_info.r_fch_ta_log         = FALSE;
          txc_ta_info.r_sch_ta_log         = FALSE;
          txc_ta_info.r_fch_ta_comp_sclk   = 0;
          txc_ta_info.r_sch_ta_comp_sclk   = 0;
          txc_ta_info.ta_hs_jump_complete  = FALSE;
          txc_ta_info.ta_tc_init_done      = FALSE;
          txc_ta_info.ta_rpc_skip_frames   = 3;
          txc_ta_info.lta_gap_frames       = 0;

          /* set interrupt handler to null and turn off TX clocks */
          txc_exit();

          txc_cmd_done( cmd_ptr, TXC_DONE_S ); /* dispose of command buffer */
          state = TXC_EXIT_S;
          break;

        case TXC_CDMA_F:
          M1X_MSG( MUX, LEGACY_MED,
            "Got TXC_CDMA_F" );

          /* --------------------------------------------------
          ** Transition back to CDMA init state without exiting
          ** txc_subtask()
          ** -------------------------------------------------- */

          /* ----------------------------------------------------
          ** Turn off the SCH again in case a SCH transaction start
          ** arrives after we process TXC_RELEASE_F.
          ** ---------------------------------------------------- */
          txtc.sch_on = FALSE;

          /* Clear MUX-RLP interface variables */
          txc_rev_rlp_params_reset();

          /* ----------------------------------------------------
          ** Set interrupt handler to null and turn off TX clocks
          ** ---------------------------------------------------- */
          txc_exit();

          /* -------------------------
          ** Dispose of command buffer
          ** ------------------------- */
          txc_cmd_done( cmd_ptr, TXC_DONE_S );

          /* ------------------------
          ** Switch back to CDMA mode
          ** ------------------------ */
          state = TXC_CDMA_S;
          break;

        case TXC_TC_F:
          M1X_MSG( MUX, LEGACY_MED,
            "Got TXC_TC_F" );

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
              M1X_MSG( MUX, LEGACY_ERROR,
                "Illegal channel configuration: FCH-> inc=%d, rc=%d DCCH-> rc=%d",
                cmd_ptr->tc.rev_fch.included,
                cmd_ptr->tc.rev_fch.rc,
                cmd_ptr->tc.rev_dcch.rc);
            }
          }

          txc_process_rcpm_event( txc_rcpm_data, cmd_ptr->rcp.rcp_data );

          /* Save local copies of RCPM data */
          txc_rcpm_data = cmd_ptr->tc.rcp_init;

          /* Reset the Smart Blanking flag to prevent using the wrong
             RPC mode/step initially. */
          txtc.rc8_status.blank_non_crit_fch_frame = FALSE;

          /* call into rlgc to update gains & ratios */
          rpc_rcpm_cmd();
          #endif /* FEATURE_IS2000_1X_ADV */

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
            enc_dcch_rc( (enc_fch_radio_config_type ) txc_so.rev_dcch.rc );
          }
          else
          {
            txc_so.rev_dcch.included = FALSE;
            enc_dcch_enable( FALSE );
          }

          if( cmd_ptr->tc.rev_fch.included )
          {
            txc_so.rev_fch.included = cmd_ptr->tc.rev_fch.included;

            txc_so.rev_fch.rc = cmd_ptr->tc.rev_fch.rc;
            enc_fch_rc( (enc_fch_radio_config_type) txc_so.rev_fch.rc );
          }
          else
          {
            txc_so.rev_fch.included = FALSE;
            enc_fch_enable( FALSE );
          }

          #ifdef FEATURE_IS2000_REL_A_SVD
          /* Choose either FCH or DCCH as signaling channel.
          ** FCH given priority over DCCH.
          */
          if( txc_so.rev_dcch.included &&
                   ( cmd_ptr->tc.signaling_phy_channel & MCTRANS_PHY_CHAN_DCCH ) )
          {
            txtc.dsch_map = txtc.cur_dsch = TXC_REV_DCCH;
          }

          if( txc_so.rev_fch.included )
          {
            if( cmd_ptr->tc.signaling_phy_channel & MCTRANS_PHY_CHAN_FCH )
            {
              txtc.dsch_map = txtc.cur_dsch = TXC_REV_FCH;
            }
          }
          #endif /* FEATURE_IS2000_REL_A_SVD */

          /* Ignore the rest of the cmd since we are in TC state already */
          if( cmd_ptr->tc.tcb_ptr != NULL )
          {
            /* -------------------------------------------------------
            ** Notify requesting task that transition to traffic state
            ** is complete
            ** ------------------------------------------------------- */
            (void) rex_set_sigs( cmd_ptr->tc.tcb_ptr, cmd_ptr->tc.sigs );
          }

          /* Dispose of command buffer */
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
          break;

        case TXC_FTM_SCH_F:
          M1X_MSG( MUX, LEGACY_MED,
            "Got TXC_FTM_SCH_F" );

         txtc.sch_on = cmd_ptr->ftm_sch.sch_on;
         if( cmd_ptr->ftm_sch.sch_on )
         {
           byte *tx_pat_ptr;
           int  i;

           /* Added the following lines for RC4 R-SCH support. */
           txtc.r_sch_rc = cmd_ptr->ftm_sch.radio_cfg;
           enc_sch_rc ((enc_sch_radio_config_type) txtc.r_sch_rc );

           if( ( txtc.r_sch_rc == CAI_RC_4 )          &&
               ( cmd_ptr->ftm_sch.sch_turbo == TRUE ) &&
               ( cmd_ptr->ftm_sch.sch_rate >= ENC_SCH_16X_RATE ) )
           {
              M1X_MSG( MUX, LEGACY_ERROR,
                "R-SCH configuration not supported" );
           }
           txtc.supp_rate = cmd_ptr->ftm_sch.sch_rate;
           txc_ftm_rsch_rate = cmd_ptr->ftm_sch.sch_rate;

           txtc.supp_turbo = cmd_ptr->ftm_sch.sch_turbo;
           txtc.supp_ltu_size = 0;

           txtc.sch_loopback = FALSE; /* Initially off */

           if( !txtc.sch_loopback )
           {
             tx_pat_ptr = (byte *)&cmd_ptr->ftm_sch.tx_pattern;
             for( i = 0; i < ENC_SCH_RC3_32X_SIZE; i++ )
             {
               txc_ftm_rsch_data.data[i] = tx_pat_ptr[i%4];
             }
           }
         }

         /* Dispose of command buffer */
         txc_cmd_done( cmd_ptr, TXC_DONE_S );
         break;

        case TXC_FTM_FCH_LOOPBACK:
          M1X_MSG( MUX, LEGACY_MED,
            "Got TXC_FTM_FCH_LOOPBACK" );
          txtc.fch_loopback = cmd_ptr->loopback.loopback_on;
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
          break;

        case TXC_FTM_DCCH_LOOPBACK:
          M1X_MSG( MUX, LEGACY_MED,
            "Got TXC_FTM_DCCH_LOOPBACK" );
          txtc.dcch_loopback = cmd_ptr->loopback.loopback_on;
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
          break;

        case TXC_FTM_SCH_LOOPBACK:
          M1X_MSG( MUX, LEGACY_MED,
            "Got TXC_FTM_SCH_LOOPBACK" );
          txtc.sch_loopback = cmd_ptr->loopback.loopback_on;
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
          break;

        #ifdef FEATURE_IS2000_1X_ADV
        case TXC_FTM_RCP_DATA_F:
          M1X_MSG( MUX, LEGACY_MED,
            "Got TXC_FTM_RCP_DATA" );

          txc_process_rcpm_event( txc_rcpm_data, cmd_ptr->rcp.rcp_data );

          /* Save local copies of RCPM data */
          txc_rcpm_data = cmd_ptr->rcp.rcp_data;

          /* Reset the Smart Blanking flag to prevent using the wrong
             RPC mode/step initially. */
          txtc.rc8_status.blank_non_crit_fch_frame = FALSE;

          /* call into rlgc to update gains & ratios */
          rpc_rcpm_cmd();

          txc_cmd_done( cmd_ptr, TXC_DONE_S );
          break;
        #endif /* FEATURE_IS2000_1X_ADV */

        case TXC_CONNECT_SO_F:
          M1X_MSG( MUX, LEGACY_MED,
            "Got TXC_CONNECT_SO_F" );

          #ifndef FEATURE_IS2000_REL_A
          txc_connect_priso = TRUE;

          if( txc_get_pri_so() != txc_so.stored_pri )
          {
            txc_set_pri_chan_config(0, txc_so.stored_pri);
            txtc.preambles = FALSE;
            //
            // preambles done; turn FCH back on
            //
            if( txc_so.rev_fch.included )
            {
              enc_fch_enable( TRUE );
            }
            txtc.nulls = FALSE;
          }
          #else /* FEATURE_IS2000_REL_A */
          /* Check for the primary traffic type */
          if( txc_srid_ref[cmd_ptr->conn_so.sr_id].traffic_type == CAI_PRI_TRAF )
          {
            txc_connect_priso = TRUE;
            if( ( txc_get_pri_so() != txc_srid_ref[cmd_ptr->conn_so.sr_id].so ) ||
                ( txc_get_pri_phy_chan() !=
                  txc_srid_ref[cmd_ptr->conn_so.sr_id].phy_channels ) )
            {
              txc_set_pri_chan_config( txc_srid_ref[cmd_ptr->conn_so.sr_id].phy_channels,
                                       txc_srid_ref[cmd_ptr->conn_so.sr_id].so );

              txtc.preambles = FALSE;
              //
              // preambles done; turn FCH back on
              //
              if( txc_so.rev_fch.included )
              {
                enc_fch_enable( TRUE );
              }
              txtc.nulls = FALSE;
            }
          }

          #ifdef FEATURE_IS2000_REL_A_SVD
          /* Check for the secondary traffic type */
          if( txc_srid_ref[cmd_ptr->conn_so.sr_id].traffic_type == CAI_SEC_TRAF )
          {
            txc_connect_secso = TRUE;
            if( ( txc_get_sec_so() != txc_srid_ref[cmd_ptr->conn_so.sr_id].so ) ||
                ( txc_get_sec_phy_chan() !=
                  txc_srid_ref[cmd_ptr->conn_so.sr_id].phy_channels ) )
            {
              txc_set_sec_chan_config( txc_srid_ref[cmd_ptr->conn_so.sr_id].phy_channels,
                                       txc_srid_ref[cmd_ptr->conn_so.sr_id].so );

              txtc.preambles = FALSE;
              //
              // preambles done; turn FCH back on
              //
              if( txc_so.rev_fch.included )
              {
                enc_fch_enable( TRUE );
              }
              txtc.nulls = FALSE;
            }
          }
          #endif /* FEATURE_IS2000_REL_A_SVD */
          #endif /* FEATURE_IS2000_REL_A */

          /* Dispose of command buffer */
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
          break;

        #ifdef FEATURE_IS2000_REL_A_SVD
        /* new case in SVD feature */
        case TXC_RELEASE_SO_F:
          M1X_MSG( MUX, LEGACY_MED,
            "Got TXC_RELEASE_SO_F" );

          rxc_mvs_set_is_so_change( FALSE );

          /* look into the service option reference data, check for
             release primary service */
          if( txc_srid_ref[cmd_ptr->release_so.sr_id].traffic_type == CAI_PRI_TRAF )
          {
            if( CAI_IS_SO_VOICE( txc_get_pri_so() ) )
            {
              muxvoc_disable_tx();
            }

            txc_connect_priso = FALSE;
            txc_set_pri_chan_config( 0, CAI_SO_NULL );

            TXC_ENTER_ISR_SECTION();
            prim_rate          = CAI_BLANK_RATE;
            prim_data_ptr      = NULL;
            prim_dcch_rate     = CAI_NULL_RATE;
            prim_dcch_data_ptr = NULL;
            TXC_LEAVE_ISR_SECTION();

            M1X_MSG( MUX, LEGACY_MED,
               "Releasing Primary SO" );
          }

          /* look into the service option reference data, check for
             release secondary service */
          if( txc_srid_ref[cmd_ptr->release_so.sr_id].traffic_type == CAI_SEC_TRAF )
          {
            txc_connect_secso = FALSE;
            txc_set_sec_chan_config( 0, CAI_SO_NULL );
            sec_fch_num_bits   = 0;
            sec_fch_data_ptr   = NULL;
            sec_dcch_num_bits  = 0;
            sec_dcch_data_ptr  = NULL;

            M1X_MSG( MUX, LEGACY_MED,
               "Releasing Secondary SO" );

            /* txc_rlp_sig_priority is used to keep track of priorities in sending RLP control
            ** frames when RLP is the secondary SO in a SVD call.  The purpose of the
            ** implementation is to improve voice quality in a SVD call.
            */
            txc_rlp_sig_priority.prev_fr_val = DSRLP_TX_NONE;
            txc_rlp_sig_priority.curr_fr_val = DSRLP_TX_NONE;
            txc_rlp_sig_priority.delay_fr_cnt = 0;
          }

          /* Dispose of command buffer */
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
          break;
        #endif /* FEATURE_IS2000_REL_A_SVD */

        case TXC_RELEASE_F:
          M1X_MSG( MUX, LEGACY_MED,
             "Got TXC_RELEASE_F" );

          rxc_mvs_set_is_so_change( FALSE );

          if( CAI_IS_SO_VOICE( txc_get_pri_so() ) )
          {
            muxvoc_disable_tx();
          }

          txc_connect_priso = FALSE;
          //
          // Do all the other stuff that was being done under TXC_SO_F
          // with SO_NULL
          //
          txc_set_pri_chan_config( 0, CAI_SO_NULL );

          #ifdef FEATURE_IS2000_REL_A
          TXC_ENTER_ISR_SECTION();
          prim_rate          = CAI_BLANK_RATE;
          prim_data_ptr      = NULL;
          prim_dcch_rate     = CAI_NULL_RATE;
          prim_dcch_data_ptr = NULL;
          TXC_LEAVE_ISR_SECTION();

          #ifdef FEATURE_IS2000_REL_A_SVD
          txc_connect_secso = FALSE;
          txc_set_sec_chan_config( 0, CAI_SO_NULL );

          sec_fch_num_bits   = 0;
          sec_fch_data_ptr   = NULL;
          sec_dcch_num_bits  = 0;
          sec_dcch_data_ptr  = NULL;

          /* txc_rlp_sig_priority is used to keep track of priorities in sending RLP control
          ** frames when RLP is the secondary SO in a SVD call.  The purpose of the
          ** implementation is to improve voice quality in a SVD call.
          */
          txc_rlp_sig_priority.prev_fr_val = DSRLP_TX_NONE;
          txc_rlp_sig_priority.curr_fr_val = DSRLP_TX_NONE;
          txc_rlp_sig_priority.delay_fr_cnt = 0;

          M1X_MSG( MUX, LEGACY_MED,
            "Releasing Pri & Sec SO" );
          #endif /* FEATURE_IS2000_REL_A_SVD */
          #endif /* FEATURE_IS2000_REL_A */
          txtc.preambles = FALSE;
          txtc.nulls = TRUE;
          if( onex_nonsig_is_ftm_mode() )
          {
            enc_set_frame_off( 0, 0 );
            txc_stop_tx( FALSE );  // Not HHO, set flag to false
          }

          txtc.sch_on = FALSE;

          /* Clear MUX-RLP interface variables */
          txc_rev_rlp_params_reset();

          /* Call is releasing, reset DTX duration checking */
          txtc.txc_dtx_off = FALSE;
          txtc.dtx_duration_included = FALSE;

          /* Dispose of command buffer */
          txc_cmd_done( cmd_ptr, TXC_DONE_S );

          /* End of call.  Terminate RLP frames log. */
          dsrlp_log_tx_send_frames();

          if( log_status( LOG_REV_PCTRL_C ) )
          {
            txc_rpctrl_send_log();
          }
          break;

        case TXC_HHO_SUCCESS_F:
          M1X_MSG( MUX, LEGACY_MED,
             "Got TXC_HHO_SUCCESS_F" );

          txc_hho( txc_stored_num_preambles, txc_stored_nom_pwr );

          /* Dispose of command buffer */
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
          break;

        case TXC_MVS_ENABLE_STATUS_F:
        {
          M1X_MSG( MUX, LEGACY_HIGH,
            "Got TXC_MVS_STAT_F, stat:%d",
            cmd_ptr->mvs_enable.status );

          muxvoc_enable_tx_sync_check();

          /* Dispose of command buffer */
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
        }
        break;

        case TXC_MVS_STANDBY_STATUS_F:
        {
          M1X_MSG( MUX, LEGACY_HIGH,
                   "Got TXC_MVS_STANDBY_STATUS_F, stat:%d",
                    cmd_ptr->mvs_standby.status);

          if( txc_frame_offset_cmd.frame_offset != m1x_stmr_get_frame_offset() )
          {
            /* De-register TX traffic events before updating frame offset */
            m1x_stmr_deregister_client(M1X_STMR_CLIENT_TX);

            #ifdef FEATURE_MODEM_1X_DRX
            /* Disable DRX feature @ FW before updating Frame Offsets */
            txc_set_frame_off_in_progress( TRUE );
            DALSYS_BusyWait( 15000 );
            #endif /* FEATURE_MODEM_1X_DRX */

            rxc_set_frame_offset( txc_frame_offset_cmd.frame_offset );
            enc_set_frame_off( txc_frame_offset, txc_frame_offset );

            /* Re-Enable 0x14ED logging after frame offset has updated in FW */
            rxc_update_1xA_logging_status(TRUE);
            #ifdef FEATURE_MODEM_1X_DRX
            txc_set_frame_off_in_progress( FALSE );
            #endif /* FEATURE_MODEM_1X_DRX */
          }

          /* Deregister and reregister the PCG level interrupts so that the
             new frame offset is considered. */
          txc_setup_pcg_events_isr( TXC_PCG_EVENT_TRAFFIC, txc_traffic_isr );

          rxc_mvs_set_standby_status( cmd_ptr->mvs_standby.status );
          rxc_set_voc_config_done( FALSE );

          /* Dispose of command buffer */
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
        }
        break;

        case TXC_SCR_F:
        {
          boolean rxc_ta_is_enabled = rxc_is_ta_enabled();
          boolean ta_is_enabled     = txc_is_ta_enabled();

          M1X_MSG( MUX, LEGACY_HIGH,
             "Got TXC_SCR_F" );

          pkt.action_type    = cmd_ptr->scr_params.action_type;
          pkt.action.txc_scr = cmd_ptr->scr_params.scr;

          /* Directly use the API to handle SCR action using action packet
           * received from CP as part of TXC_SCR_F cmd */
          txc_scr_trans_cmd( &pkt );

          /* Mark preambles as False. This is required if this cmd was sent
           * to TX before call was answered for MT call, for instance during
           * inter band CFS. In this case preambles doesn't get reset and UE
           * keeps on transmitting non-signalling frames leading to RXTX
           * buffer exhaustions/ ack timeouts and call drop. */
          txtc.preambles = FALSE;

          if( ta_is_enabled )
          {
            txc_ta_info.lta_gap_frames = GET_TIMETICK_MS_FROM_SCLKS
              ( ( timetick_get() ) - txc_ta_info.lta_start_sclk );

            txc_ta_info.lta_gap_frames = txc_ta_info.lta_gap_frames/20;

            M1X_MSG( MUX, LEGACY_HIGH,
              "1x2GTA: Current time %d, GAP frames %d",
               timetick_get(),
               txc_ta_info.lta_gap_frames );

            txc_ta_info.r_fch_ta_log       = TRUE;
            /* 0x18CD : Log R-SCH only its assigned */
            if( mccsch_sch_is_active(MCCSCH_REVERSE, 0) )
            {
              txc_ta_info.r_sch_ta_log     = TRUE;
            }

            /* End of call, stop RLP logging */
            dsrlp_log_tx_send_frames();

            /* Update DS layer with SR_ID and TX lost frames */
            dsrlp_update_ta_gap( DSRLP_FIXED_SR_ID_INDEX, txc_ta_info.lta_gap_frames );
          }

          if( !rxc_ta_is_enabled )
          {
            txc_ta_info.ta_start              = FALSE;
            txc_ta_info.ta_comp               = TRUE;
            txc_ta_info.ta_hs_jump_complete   = FALSE;
            txc_ta_info.ta_tc_init_done       = FALSE;

            /* Time sclk logging */
            txc_ta_info.r_fch_ta_comp_sclk = 0;
            txc_ta_info.r_sch_ta_comp_sclk = 0;
          }

          txc_ta_info.ta_rpc_skip_frames      = 3;
          txc_ta_info.lta_ol_tx_pwr           = 0;

          /* CP does a blocked wait waiting for processing of this cmd */
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
        }
        break;

        case TXC_TA_START_F:
        {
          M1X_MSG( MUX, LEGACY_HIGH,
             "1x2GTA: Got TXC_TA_START_F rsn %d",
             cmd_ptr->ta_start.ta_type );

          if( cmd_ptr->ta_start.ta_type == SRCH_TA_QTA )
          {
            txc_tx.tx_on = FALSE;

            /* set up the rxc flag so that ffpc will not update the setpoint */
            rxc_set_tx_in_traffic_on(FALSE);

            if( txc_tx.active_msg )
            {
              /* we were in the middle of the message - send it again */
              txc_tx.buf.pos = 0;
            }
          }

          /* TA sclk time logging */
          txc_ta_info.ta_start_sclk = timetick_get();
          txc_ta_info.r_fch_ta_log  = FALSE;
          txc_ta_info.r_sch_ta_log  = FALSE;

          txc_ta_info.ta_start = TRUE;
          txc_ta_info.ta_comp  = FALSE;

          /* End of call, stop RLP logging */
          dsrlp_log_tx_send_frames();

          /* CP does a blocked wait waiting for processing of this cmd */
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
        }
        break;

        case TXC_TA_COMP_F:
        {
          M1X_MSG( MUX, LEGACY_HIGH,
             "1x2GTA: Got TXC_TA_COMP_F rsn %d",
             cmd_ptr->ta_comp.ta_type );

          if( cmd_ptr->ta_comp.ta_type == SRCH_TA_QTA )
          {
            /* Start TX */
            txc_tx.tx_on = TRUE;

            /* set up the rxc flag so that ffpc will update the setpoint */
            rxc_set_tx_in_traffic_on(TRUE);

            txc_tx.tx_blank = TRUE;/* expect one blank frame */
          }

          /* Time sclk logging */
          txc_ta_info.r_fch_ta_comp_sclk = 0;
          txc_ta_info.r_sch_ta_comp_sclk = 0;

          /* No need to log R-F/SCH TA time since ENC is always enabled for QTA */
          txc_ta_info.r_fch_ta_log       = FALSE;
          txc_ta_info.r_sch_ta_log       = FALSE;

          txc_ta_info.ta_start = FALSE;
          txc_ta_info.ta_comp  = TRUE;

          /* End of call, stop RLP logging */
          dsrlp_log_tx_send_frames();

          /* CP does a blocked wait waiting for processing of this cmd */
          txc_cmd_done( cmd_ptr, TXC_DONE_S );
        }
        break;

        default:                /* bad command */
          M1X_MSG( MUX, LEGACY_ERROR,
            "Illegal TC command %d",
            cmd_ptr->hdr.command );
          /* dispose of command buffer */
          txc_cmd_done( cmd_ptr, TXC_BADCMD_S );
          break;
      }  /* switch */
    } /* Item off of queue */

    /* Handle mux msgr signal */
    if( sigs & TXC_MUXMSGR_Q_SIG )
    {
      /* process and clear the muxmsgr signal */
      muxmsgr_process_rcv_msg( TX_TASK );
    } /* Handle mux msgr signal */

    /* wait for command on the queue or an interrupt */
    if( state == TXC_TRAFFIC_S )
    {
      sigs = txc_wait( TXC_CMD_Q_SIG | TXC_INT_SIG | TXC_MUXMSGR_Q_SIG );
    }

  } while( state == TXC_TRAFFIC_S ); /* until done with this state */

  txc_clr_msgs();                  /* clear any pending messages to send */

  txc_data_rate = CAI_BLANK_RATE;

  /* Changes to update subscription state to INACTIVE during
     Traffic to EXIT states */
  if( state == TXC_EXIT_S )
  {
    txc_update_sp_state( SP_TECH_INACTIVE_STATE );
     M1X_MSG( MUX, LEGACY_HIGH,
        "Change SB state to INACTIVE" );
  }

  return state;

} /* txc_traffic */

/*===========================================================================
FUNCTION       TXC_INFORM_SCH_CHNG

DESCRIPTION    Called by data when the SCH config has changed.  Sets a flag
               which tells txc to calculate the throttling params.  Done
               this way since some vars in txc and rxc (used to calc the
               throttling numbers) don't change immediately.

DEPENDENCIES   None

SIDE EFFECTS   None

RETURN VALUE   None
===========================================================================*/
void txc_inform_sch_chng( void )
{
  /*-----------------------------------------------------------------------*/

  txc_sch_changed = TXC_SCH_CHANGED_DELAY;

} /* txc_inform_sch_chng */

/*========================================================================
FUNCTION TXC_REV_SCH_TRANS_CMD

DESCRIPTION
   The reverse SCH transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void txc_rev_sch_trans_cmd( mctrans_action_packet_type *pkt_ptr )
{
  /*-----------------------------------------------------------------------*/

   if( pkt_ptr != NULL )
   {
     M1X_MSG( MUX, LEGACY_MED,
       "Got MCTRANS_R_SCH_ACTION: mode %d, rate %d, turbo %d",
       pkt_ptr->action.txc_r_sch.mode,
       pkt_ptr->action.txc_r_sch.sch_rate+3,
       pkt_ptr->action.txc_r_sch.turbo_code);

     switch ( pkt_ptr->action.txc_r_sch.mode )
     {
       case MCTRANS_R_SCH_START:
       {
         txtc.sch_on = TRUE;
         txtc.r_sch_rc = pkt_ptr->action.txc_r_sch.rc;

         enc_sch_rc ((enc_sch_radio_config_type) txtc.r_sch_rc);
         txtc.supp_rate = (enc_sch_rate_type)
                             ( (uint32) pkt_ptr->action.txc_r_sch.sch_rate + 3 );

         txtc.supp_turbo = (pkt_ptr->action.txc_r_sch.turbo_code == CAI_TURBO_CODING);
         txtc.double_sized = pkt_ptr->action.txc_r_sch.double_size_pdu;
         if (( !txtc.supp_turbo ) && (txtc.supp_rate > ENC_SCH_2X_RATE))
         {
           /*----------------------------------------------------------------
              true for any rate greater than 2x, as an LTU is defined as
              the size of a double-size mux pdu + mux header + 16 bit
              CRC
              size = mux hdr + data payload + ltu crc
           ----------------------------------------------------------------*/
           #ifndef FEATURE_IS2000_REL_A
           if (txtc.r_sch_rc == CAI_RC_4)
           {
             txtc.supp_ltu_size = 6 + 538 + 16;
           }
           else
           {
             txtc.supp_ltu_size = 6 + 346 + 16;
           }
           #else
           txtc.supp_ltu_size = pkt_ptr->action.txc_r_sch.ltu_len;
           #endif /* FEATURE_IS2000_REL_A */
         }
         else
         {
           /*----------------------------------------------------------------
              For 1x and 2x, there is no LTU used, as per standard.
              Therefore, for our purposes, the ltu_size is zero
           ----------------------------------------------------------------*/
           txtc.supp_ltu_size = 0;
         }

         #ifdef FEATURE_IS2000_REL_A
         txtc.mux_pdu_type = pkt_ptr->action.txc_r_sch.mux_pdu_type;
         txtc.num_ltu = pkt_ptr->action.txc_r_sch.num_ltu;
         #endif /* FEATURE_IS2000_REL_A */

         /*------------------------------------------------------------------
           Set the number of PDU's to be built to the max.  DS will call
           a function so that any throttling that is required will take
           place.
         ------------------------------------------------------------------*/
         txtc.pot_num_mux_pdus = 1 << ((uint16) txtc.supp_rate - (uint16) ENC_SCH_FULL_RATE);
         /* no double side consept in for MUXPDU 5, but each LTU contains one PDU,
         ** so it seems just like double side in the case MUXPDU 3
         */
         /* for 32X, we want to limit the number of potential PDU as 16X case */
         if (txtc.supp_rate == ENC_SCH_32X_RATE)
         {
           txtc.pot_num_mux_pdus = txtc.pot_num_mux_pdus >> 1;
         }
         #ifdef FEATURE_IS2000_REL_A
         if ((txtc.double_sized == TRUE) || ((txtc.mux_pdu_type == CAI_MUX_PDU_TYPE_5)
                                             && (txtc.supp_rate > ENC_SCH_FULL_RATE)))
         #else
         if (txtc.double_sized == TRUE)
         #endif /* FEATURE_IS2000_REL_A */
         {
           txtc.pot_num_mux_pdus = txtc.pot_num_mux_pdus >> 1;
         }
         txtc.ok_num_mux_pdus  = txtc.pot_num_mux_pdus;

         /*------------------------------------------------------------------
           Get the DTX duration.
         ------------------------------------------------------------------*/
         txtc.dtx_duration_org = pkt_ptr->
                                    action.txc_r_sch.rev_sch_dtx_duration;

         /*------------------------------------------------------------------
            DTX duration is not set to infinite by the BS. Turns on the DTX
            duration checking.  A DTX duration of zero means immediately
            send the SCRM, while 1 means send the SCRM after the DTX period.
         ------------------------------------------------------------------*/
         if (txtc.dtx_duration_org != 0x0F)
         {
           txtc.dtx_duration_included = TRUE;
           txtc.dtx_duration = txtc.dtx_duration_org;
         }
         else
         {
           txtc.dtx_duration_included = FALSE;
           txtc.txc_dtx_off = FALSE;
         }

         if( txc_ta_info.r_sch_ta_log )
         {
           txc_ta_info.r_sch_ta_comp_sclk = timetick_get();
           txc_ta_info.r_sch_ta_log       = FALSE;

           /* Log to SRCH, if F-FCH & R-FCH already logged and
             if FSCH is not going to be assigned or FSCH log is already done */
           if( ( ( rxc_ta_info.f_fch_ta_log == FALSE ) && 
                 ( txc_ta_info.r_fch_ta_log == FALSE ) ) &&
               ( ( !mccsch_sch_is_active(MCCSCH_FORWARD, 0) ) ||
                 ( rxc_ta_info.f_sch_ta_log == FALSE ) ))
           {
             /* Update SRCH here about the stats */
                rxc_tx_ta_srch_log_update();
           }
         }

         M1X_MSG( MUX, LEGACY_HIGH,
           "Start RSCH rate=%d, Turbo=%d #PDUs=%d",
           txtc.supp_rate,
           txtc.supp_turbo,
           txtc.pot_num_mux_pdus);
         break;
       } // of case MCTRANS_R_SCH_START

       case MCTRANS_R_SCH_STOP:
       {
         // Shutdown SCH processing
         txtc.sch_on = FALSE;

         /* Clear MUX-RLP interface variables */
         txc_rev_rlp_params_reset();

         /* Reset DTX duration checking */
         txtc.txc_dtx_off = FALSE;
         txtc.dtx_duration_included = FALSE;
         break;
       } // of case MCTRANS_R_SCH_STOP

       case MCTRANS_R_SCH_DTX:
       {
         // If SCH processing is on, update DTX duration
         if (txtc.sch_on)
         {
           if (!txtc.dtx_duration_included)
           {
             /*-----------------------------------------------------------------
                DTX duration is not set to infinite by the BS. Turns on the DTX
                duration checking.
             ------------------------------------------------------------------*/
             if (pkt_ptr->action.txc_r_sch.rev_sch_dtx_duration != 0x0F)
             {
               txtc.dtx_duration_included = TRUE;
               txtc.dtx_duration =
                 pkt_ptr->action.txc_r_sch.rev_sch_dtx_duration;
             }
           }
           else
           {
             if (pkt_ptr->action.txc_r_sch.rev_sch_dtx_duration == 0x0F)
             {
               /*-------------------------------------------------------------
                  DTX duration is set to infinite by the BS. Turns off the DTX
                  duration checking.
               ---------------------------------------------------------------*/
               txtc.txc_dtx_off = FALSE;
               txtc.dtx_duration_included = FALSE;
             }
             else if ((txtc.dtx_duration_org - txtc.dtx_duration) >=
                      pkt_ptr->action.txc_r_sch.rev_sch_dtx_duration)
             {
               /*-------------
                  DTX expired.
               ---------------*/
               txtc.dtx_duration = 0;
             }
             else
             {
               /*--------------------
                  Update DTX duration
               ----------------------*/
               txtc.dtx_duration =
                pkt_ptr->action.txc_r_sch.rev_sch_dtx_duration -
                txtc.dtx_duration_org + txtc.dtx_duration;
             }
           } // end if (!txtc.dtx_duration_included)

           /*-----------------------
             Save the DTX duration.
           -------------------------*/
           txtc.dtx_duration_org =
             pkt_ptr->action.txc_r_sch.rev_sch_dtx_duration;
         }
         break;
       } // of case MCTRANS_R_SCH_DTX

     } //lint !e744 : No default needed for this switch
     /* Channel configuration has changed, we need to run the RF PA backoff algorithm
     to back off the power if necessary */
     txc_update_chan_config_to_rf(TRUE);
   }
   else
   {
     M1X_MSG( MUX, LEGACY_ERROR,
       "NULL MCTRANS_R_SCH_ACTION received" );
   }

} /* txc_rev_sch_trans_cmd */

/*========================================================================
FUNCTION TXC_RLGC_TRANS_CMD

DESCRIPTION
   The RLGC transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void txc_rlgc_trans_cmd( mctrans_action_packet_type *pkt_ptr )
{
  /*-----------------------------------------------------------------------*/

  if( pkt_ptr != NULL )
  {
    M1X_MSG( MUX, LEGACY_MED,
       "Got MCTRANS_RLGC_ACTION" );

    /* handle Power Control Step */
    if( pkt_ptr->action.rlgc.pwr_cntl_step.included )
    {
      txtc.pwr_ctrl_step_size = pkt_ptr->action.rlgc.pwr_cntl_step.value;

      muxmdsp_set_tx_gain_adj_step_size
        ( (cai_pwr_cntl_step_type) pkt_ptr->action.rlgc.pwr_cntl_step.value );
    }

    rpc_trans_cmd( pkt_ptr );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
       "NULL MCTRANS_RLGC_ACTION received" );
  }
} /* txc_rlgc_trans_cmd */

/*========================================================================
FUNCTION TXC_LCM_TRANS_CMD

DESCRIPTION
   The LCM transaction callback
   LCM means long code mask

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void txc_lcm_trans_cmd( mctrans_action_packet_type *pkt_ptr )
{
  /*-----------------------------------------------------------------------*/

  if( pkt_ptr != NULL )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_LCM_ACTION: mask 0x%x%x",
      pkt_ptr->action.txc_lcm.long_code_mask[1],
      pkt_ptr->action.txc_lcm.long_code_mask[0] );

    /*load the new long code mask */
    qw_equ(txc_tx.mask, pkt_ptr->action.txc_lcm.long_code_mask);
    enc_user_pn_mask(txc_tx.mask);
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_LCM_ACTION received" );
  }

} /* txc_lcm_trans_cmd */

/*========================================================================
FUNCTION TXC_HO_TRANS_CMD

DESCRIPTION
   The HO transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void txc_ho_trans_cmd( mctrans_action_packet_type *pkt_ptr )
{
  /*-----------------------------------------------------------------------*/

  if( pkt_ptr != NULL )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_HO_ACTION: is HHO %d",
      pkt_ptr->action.txc_ho.hho_ind );

    #ifdef FEATURE_IS2000_REL_A
    /* For release A, switch P_REV right here and build the next frame
       with new P_REV. */
    txtc.p_rev_in_use = pkt_ptr->action.txc_ho.p_rev_in_use;
    #endif /* FEATURE_IS2000_REL_A */

    if( pkt_ptr->action.txc_ho.hho_ind != TRUE )
    {
      if ( pkt_ptr->action.txc_ho.nom_pwr.included )
      {
        /* nom_pwr ought not to change for a soft handoff, but still
           recalculate using the "new" nom_pwr */
        byte power;

        /* ------------------------------------------------------------
        ** nom_pwr is already sign extended and, if necessary, adjusted
        ** according to nom_pwr_ext.
        ** ------------------------------------------------------------ */
        power = (byte) pkt_ptr->action.txc_ho.nom_pwr.value;

        /* Invert to turn power adjustment into tx gain adjustment
         * power is 5 bit two's complement.  After negation, it is stored
         * as two's complement, but the type is unsigned. The sign is not
         * lost because the value of this variable only uses the lower
         * five bits
         */
        power = -(power);//lint !e732 Loss of sign (assignment) (int to unsigned char)

        /* Convert to units of 1/2 dB
           power is never more than 2^5 here. After multiplication, 2^6 */
        power = power * 2;//lint !e734 Loss of precision (assignment) (9 bits to 8 bits)

        /* subtract from current gain adjust in negative 2's complement */
        txtc.power = txtc.power + power;

        /* Print nom_power and updated tx gain adjust in -1/2dB units */
        M1X_MSG( MUX, LEGACY_HIGH,
          "1X HO_TRANS:SHO (Updated) TxAdjPwr=%d,"
          "nom_pwr=%d <-- (-1/2dB)",
          txtc.power,
          power );
      }
    }
    else // This is a hard handoff
    {
      txc_hho_preamble_pcg_units = FALSE;

      /* Print frame offset changes if any */
      M1X_MSG( MUX, LEGACY_HIGH,
        "1X HO_TRANS:HHO Frame Offset %d to %d",
        txc_frame_offset,
        pkt_ptr->action.txc_ho.frame_offset);

      /* Stop RF Tx, which will happen at next frame boundary in FW,
         HHO flag set to TRUE */
      txc_stop_tx(TRUE);

      /* If the nominal power is included, then use that value for the
         stored nominal power. Default the stored nominal power to 0 dB
         otherwise */
      txc_stored_nom_pwr = pkt_ptr->action.txc_ho.nom_pwr.included ?
                           pkt_ptr->action.txc_ho.nom_pwr.value : 0;

      txc_stored_num_preambles = pkt_ptr->action.txc_ho.num_preamble.value;

       M1X_MSG( MUX, LEGACY_HIGH,
         "1X HO_TRANS:HHO (Stored) nom_pwr=%d (1/2dB), num_preambles=%d",
         (txc_stored_nom_pwr*2),
         txc_stored_num_preambles);

      if( pkt_ptr->action.txc_ho.num_preamble.unit == MCTRANS_PREAMBLE_PCG )
      {
        txc_hho_preamble_pcg_units = TRUE;
      }
    }
    /* For a return transaction, HO action can carry frame offset
       but the hho_ind value will be set to 0 */
    if( pkt_ptr->action.txc_ho.frame_offset != txc_frame_offset )
    {
      txc_frame_offset_cmd.frame_offset = pkt_ptr->action.txc_ho.frame_offset;

      /* what about the action frame? */
      /* Disable 0x14ED logging before frame offset is updated to FW */
      rxc_update_1xA_logging_status(FALSE);

      /* If its a data call no need for VFR related changes to execute,
         also if UHDM comes before call answer then set the frame offsets */
      if( ( rxc_is_data_call() ) || ( !rxc_get_mvs_status_on_tc_entry() ) )
      {
        txc_frame_offset = txc_frame_offset_cmd.frame_offset;

        /* De-register TX traffic events before updating frame offset */
        m1x_stmr_deregister_client(M1X_STMR_CLIENT_TX);

        rxc_set_frame_offset( txc_frame_offset_cmd.frame_offset );
        enc_set_frame_off( txc_frame_offset, txc_frame_offset );

        /* Re-Enable 0x14ED logging after frame offset has updated in FW */
        rxc_update_1xA_logging_status(TRUE);

        /* Deregister and reregister the PCG level interrupts so that the
           new frame offset is considered. */
        txc_setup_pcg_events_isr( TXC_PCG_EVENT_TRAFFIC, txc_traffic_isr );
      }
      else
      {
        /* Call MVS standby API before setting frame offset */
        muxvoc_mvs_standby();

        txc_frame_offset = txc_frame_offset_cmd.frame_offset;
      }
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_HO_ACTION received" );
  }

} /* txc_ho_trans_cmd */

/*========================================================================
FUNCTION TXC_SCR_TRANS_CMD

DESCRIPTION
   The SCR transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void txc_scr_trans_cmd( mctrans_action_packet_type *pkt_ptr )
{
  #ifndef FEATURE_IS2000_REL_A
  boolean so_connected;
  #endif /* !FEATURE_IS2000_REL_A */

  #ifdef FEATURE_IS2000_REL_A
  word index;
  #endif /* FEATURE_IS2000_REL_A */

  /*-----------------------------------------------------------------------*/

  if( pkt_ptr != NULL )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_SCR_ACTION" );

    #ifdef FEATURE_IS2000_1X_ADV
    /* For 1x Advanced DCCH is not supported R-FCH.
      If it is included at the same time that R-FCH is then we must
      confirm that R-FCH RC and R-DCCH are RC8.
    */
    if( pkt_ptr->action.txc_scr.dcch.included )
    {
      if( ( pkt_ptr->action.txc_scr.dcch.rc == CAI_RC_8 ) ||
          ( ( pkt_ptr->action.txc_scr.fch.included ) &&
            ( pkt_ptr->action.txc_scr.fch.rc == CAI_RC_8 ) ) )
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "Illegal channel configuration: FCH-> inc=%d, rc=%d DCCH-> rc=%d",
          pkt_ptr->action.txc_scr.fch.included,
          pkt_ptr->action.txc_scr.fch.rc,
          pkt_ptr->action.txc_scr.dcch.rc);
      }
    }
    #endif /* FEATURE_IS2000_1X_ADV */

    // Default dsch to FCH
    txtc.dsch_map = txtc.cur_dsch = TXC_REV_FCH;

    // Note the order of checking DCCH and then FCH below cannot be reversed since MSM5000/5105
    // shares the same hardware between DCCH and FCH
    if( pkt_ptr->action.txc_scr.dcch.included )
    {
      txc_so.rev_dcch.included = pkt_ptr->action.txc_scr.dcch.included;

      // Before Logical-to-physical mapping is support, just do the default
      // dsch_map specified in IS2000
      if( !pkt_ptr->action.txc_scr.fch.included )
      {
        txtc.dsch_map = txtc.cur_dsch = TXC_REV_DCCH;
      }
      txc_so.rev_dcch.rc = pkt_ptr->action.txc_scr.dcch.rc;
      enc_dcch_rc( (enc_fch_radio_config_type)txc_so.rev_dcch.rc );
      enc_dcch_enable( TRUE );
    }
    else
    {
      txc_so.rev_dcch.included = FALSE;
      enc_dcch_enable( FALSE );
    }

    if( pkt_ptr->action.txc_scr.fch.included )
    {
      txc_so.rev_fch.included = pkt_ptr->action.txc_scr.fch.included;
      txc_so.rev_fch.rc = (cai_radio_config_type) pkt_ptr->action.txc_scr.fch.rc;

      /* Avoid sending the RliRC config until the SCR transaction/HHO is
         Complete. This is needed as for frame offset change cases along with
         RC change HHO, VSTMR interrupts are deregistered. Due to this, FW gets
         the frame config commands after a couple of frames. During these
         frames, FW doesn't have consistant params. To avoid this, delaying the
         RliRC update is required. */
      txc_tx.is_rc_update_deferred = TRUE;

      enc_fch_rc( (enc_fch_radio_config_type) txc_so.rev_fch.rc );
      enc_fch_enable( TRUE );
    }
    else
    {
      txc_so.rev_fch.included = FALSE;
      enc_fch_enable (FALSE);
    }

    #ifdef FEATURE_IS2000_REL_A_SVD
    /* Choose either FCH or DCCH as signaling channel.
    ** FCH given priority over DCCH.
    */
    if( txc_so.rev_dcch.included &&
        ( pkt_ptr->action.txc_scr.signaling_phy_channel & MCTRANS_PHY_CHAN_DCCH ) )
    {
      txtc.dsch_map = txtc.cur_dsch = TXC_REV_DCCH;
    }

    if( txc_so.rev_fch.included )
    {
      if( pkt_ptr->action.txc_scr.signaling_phy_channel & MCTRANS_PHY_CHAN_FCH )
      {
        txtc.dsch_map = txtc.cur_dsch = TXC_REV_FCH;
      }
    }
    #endif /* FEATURE_IS2000_REL_A_SVD */

    if( !txc_so.rev_fch.included )
    {
      enc_fch_enable( FALSE );
    }

    #ifdef FEATURE_IS2000_REL_A
    /* copy the SCH MUX OPTION from the upper layer to the MUX for RDCH logging*/
    if( pkt_ptr->action.txc_scr.rev_sch0_mux_option.included )
    {
      txc_so.rev_sch_mux_option = pkt_ptr->action.txc_scr.rev_sch0_mux_option.value;
    }
    else
    {
      txc_so.rev_sch_mux_option = 0;
    }
    #endif /* FEATURE_IS2000_REL_A */
    // Just in case we are changing between new and old RCs, make sure rev pilot
    // is turned on/off properly
    txc_pilot_init();
    #ifndef FEATURE_IS2000_REL_A
    if( ( pkt_ptr->action.txc_scr.pri_so.included ) &&
        ( pkt_ptr->action.txc_scr.pri_so.value != txc_get_pri_so() ) )
    {
      if( txc_connect_priso )
      {
         so_connected = TRUE;
      }
      else
      {
        so_connected = FALSE;
        if( pkt_ptr->action.txc_scr.pri_so.connect_so )
        {
           txc_connect_priso = TRUE;
           so_connected = TRUE;
        }
        else
        {
           txc_so.stored_pri.so = pkt_ptr->action.txc_scr.pri_so.value;
        }
      }

      if( so_connected )
      {
        txc_so.stored_pri =
         pkt_ptr->action.txc_scr.pri_so.value;
        txc_set_pri_chan_config(0, pkt_ptr->action.txc_scr.pri_so.value);
        txtc.preambles = FALSE;
        //
        // preambles done; turn FCH back on
        //
        if( txc_so.rev_fch.included )
        {
          enc_fch_enable( TRUE );
        }
        // If connecting CAI_SO_NULL (removing SO effectively), then we should
        // just send nulls.
        txtc.nulls = (txc_get_pri_so() == CAI_SO_NULL);

        /* If new SO is LOOPBACK SO and old one is VOICE SO, then
           release the VOC and VFR resources */
        if( txc_is_loopback_so( pkt_ptr->action.txc_scr.pri_so.value ) &&
            CAI_IS_SO_VOICE( txc_get_pri_so() ) )
        {
          muxvoc_disable_tx();
        }
      }
    }
    #else
    /*check if there is new service */
    if( pkt_ptr->action.txc_scr.num_of_so_recs > 0 )
    {
      for( index = 0; index < pkt_ptr->action.txc_scr.num_of_so_recs; index++ )
      {
        /* Check for new PRIMARY service option transaction */
        if( pkt_ptr->action.txc_scr.so_cfgs[index].traffic_type == CAI_PRI_TRAF )
        {
          if( ( pkt_ptr->action.txc_scr.so_cfgs[index].so != txc_get_pri_so() ) ||
              ( txc_get_pri_phy_chan() !=
                        pkt_ptr->action.txc_scr.so_cfgs[index].phy_channel ) )
          {
            if( pkt_ptr->action.txc_scr.so_cfgs[index].connect_so )
            {
              txc_connect_priso = TRUE;
            }

            if( txc_connect_priso )
            {
              txc_set_pri_chan_config( pkt_ptr->action.txc_scr.so_cfgs[index].phy_channel,
                                       pkt_ptr->action.txc_scr.so_cfgs[index].so );

              txtc.preambles = FALSE;
              //
              // preambles done; turn FCH back on
              //
              if( txc_so.rev_fch.included )
              {
                enc_fch_enable( TRUE );
              }
              txtc.nulls = FALSE;

              /* If new SO is LOOPBACK SO and old one is VOICE SO, then
                 release the VOC and VFR resources */
              if( txc_is_loopback_so( pkt_ptr->action.txc_scr.so_cfgs[index].so ) &&
                  CAI_IS_SO_VOICE( txc_get_pri_so() ) )
              {
                muxvoc_disable_tx();
              }
            }
          }

          #ifdef FEATURE_IS2000_REL_A_SVD
          /* if PRIMARY SERVICE OPTION is the only service, then reset the secondary SO */
          if (pkt_ptr->action.txc_scr.num_of_so_recs == 1)
          {
            txc_connect_secso = FALSE;
            if (txc_get_sec_so() != CAI_SO_NULL)
            {
              M1X_MSG( MUX, LEGACY_MED,
                "Releasing Secondary SO");
            }
            txc_set_sec_chan_config(0, CAI_SO_NULL);
            sec_fch_num_bits   = 0;
            sec_fch_data_ptr   = NULL;
            sec_dcch_num_bits  = 0;
            sec_dcch_data_ptr  = NULL;
          }
          #endif /* FEATURE_IS2000_REL_A_SVD */
        } /* end of PRIMARY SERVICE OPTION */

        #ifdef FEATURE_IS2000_REL_A_SVD
        /* Check for new SECONDARY service option transaction */

        if( pkt_ptr->action.txc_scr.so_cfgs[index].traffic_type == CAI_SEC_TRAF )
        {
          if( ( pkt_ptr->action.txc_scr.so_cfgs[index].so != txc_get_sec_so() ) ||
              ( txc_get_sec_phy_chan() !=
                      pkt_ptr->action.txc_scr.so_cfgs[index].phy_channel ) )
          {
            if( pkt_ptr->action.txc_scr.so_cfgs[index].connect_so )
            {
              txc_connect_secso = TRUE;
            }

            if( txc_connect_secso )
            {
              txc_set_sec_chan_config( pkt_ptr->action.txc_scr.so_cfgs[index].phy_channel,
                                       pkt_ptr->action.txc_scr.so_cfgs[index].so );
              txtc.preambles = FALSE;
              //
              // preambles done; turn FCH back on
              //
              if( txc_so.rev_fch.included )
              {
                enc_fch_enable( TRUE );
              }
              txtc.nulls = FALSE;
            }
          }
          /* if SECONDARY SERVICE OPTION is the only service, then reset the primary SO */
          if( pkt_ptr->action.txc_scr.num_of_so_recs == 1 )
          {
            txc_connect_priso = FALSE;
            if( txc_get_pri_so() != CAI_SO_NULL )
            {
              M1X_MSG( MUX, LEGACY_MED,
                "Releasing Primary SO" );
            }
            txc_set_pri_chan_config( 0, CAI_SO_NULL );

            TXC_ENTER_ISR_SECTION();
            prim_rate          = CAI_BLANK_RATE;
            prim_data_ptr      = NULL;
            prim_dcch_rate     = CAI_NULL_RATE;
            prim_dcch_data_ptr = NULL;
            TXC_LEAVE_ISR_SECTION();
          }
        } /* end of SECONDARY SERVICE OPTION */
        #endif /* FEATURE_IS2000_REL_A_SVD */

        /* copy the tx service option reference */
        txc_srid_ref[pkt_ptr->action.txc_scr.so_cfgs[index].sr_id].so =
                                         pkt_ptr->action.txc_scr.so_cfgs[index].so;
        txc_srid_ref[pkt_ptr->action.txc_scr.so_cfgs[index].sr_id].phy_channels
                                         = pkt_ptr->action.txc_scr.so_cfgs[index].phy_channel;
        txc_srid_ref[pkt_ptr->action.txc_scr.so_cfgs[index].sr_id].traffic_type
                                         = pkt_ptr->action.txc_scr.so_cfgs[index].traffic_type;
      } /*end of FOR */
    } /* end of number of service bigger than zero */
    else
    {
      // If there is no SO to connect, then we should just send nulls.
      txtc.nulls = TRUE;
      // Release Vocoder resources if assigned
      muxvoc_disable_tx();
    }
    #endif /* FEATURE_IS2000_REL_A */

    // Channel configuration has changed, we need to run the RF PA backoff algorithm
    // to back off the power if necessary
    txc_update_chan_config_to_rf( TRUE );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_SCR_ACTION received" );
  }

} /* txc_scr_trans_cmd */

/*========================================================================
FUNCTION TXC_GATING_TRANS_CMD

DESCRIPTION
   The FCH 8TH rate gating transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void txc_gating_trans_cmd( mctrans_action_packet_type *pkt_ptr )
{
  /*-----------------------------------------------------------------------*/

  if( pkt_ptr != NULL )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_GATING_ACTION: gating enabled %d",
      pkt_ptr->action.txc_gating.rev_fch_gating_on );

    if( pkt_ptr->action.txc_gating.rev_fch_gating_on )
    {
      txtc.fch_8th_rate_gating = TRUE;
    }
    else
    {
      txtc.fch_8th_rate_gating = FALSE;
      enc_gating_mode( ENC_NO_GATING_MODE );
      enc_is95c_tx_patn( ENC_NO_GATING_PATTERN );
    }

    /* R-FCH gating event has a 1 byte boolean parameter */
    event_report_payload(EVENT_REV_FCH_GATING_IN_USE,
                         sizeof(byte),
                         (void *) &txtc.fch_8th_rate_gating);

    rpc_fch_gating_cmd (pkt_ptr->action.txc_gating.rev_fch_gating_on);
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_GATING_ACTION received" );
  }

} /* txc_gating_trans_cmd */

/*========================================================================
FUNCTION TXC_CONTROL_HOLD_TRANS_CMD

DESCRIPTION
   Processor for the control hold action.

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void txc_control_hold_trans_cmd
(
  mctrans_action_packet_type *pkt_ptr
)
{
  enc_gating_pattern_type gating_pattern;

  /*-----------------------------------------------------------------------*/

  if ( pkt_ptr != NULL )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_CTRL_HLD_GATING_ACTION: gating rate %d",
      pkt_ptr->action.txc_ctrl_hld_gating.gating_rate );

    // Default to no gating
    gating_pattern = ENC_NO_GATING_PATTERN;

    // Remember gating rate
    txtc.gating_rate = pkt_ptr->action.txc_ctrl_hld_gating.gating_rate;

    if ( txtc.gating_rate == CAI_GATING_RATE_QUARTER )
    {
      gating_pattern = ENC_QTR_RATE_GATING_PATTERN;
    }
    else if (txtc.gating_rate == CAI_GATING_RATE_HALF )
    {
      gating_pattern = ENC_HALF_RATE_GATING_PATTERN;
    }

    // Now turn on/off gating
    enc_gating_mode( ENC_PILOT_GATING_MODE );
    enc_is95c_tx_patn( gating_pattern );

    if ( txtc.gating_rate != CAI_GATING_RATE_NONE )
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "Pilot gating enabled:%d",
        txtc.gating_rate );
    }
    else
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "Pilot gating disabled" );
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_CTRL_HLD_GATING_ACTION received" );
  }
} /* txc_control_hold_trans_cmd */

/*========================================================================
FUNCTION TXC_TX_ON_OFF_TRANS_CMD

DESCRIPTION
   The TX on/off call back

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void txc_tx_on_off_trans_cmd( mctrans_action_packet_type *pkt_ptr )
{
  /*-----------------------------------------------------------------------*/

  if ( pkt_ptr != NULL )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_TX_ON_OFF_ACTION: TX On %d",
      pkt_ptr->action.txc_tx_on_off.tx_on );

    if (pkt_ptr->action.txc_tx_on_off.tx_on)
    {
      /* Set tx gain adjust power level before turning on TX in -1/2 dB */
      M1X_MSG( MUX, LEGACY_HIGH,
        "1X TX_ON:TX ADJUST (Restored) TxAdjPwr=%d (-1/2dB)",
        txtc.power);

      /* Set (potentially) new CLPC power level, RPC freeze dipswitch
         handled inside this function. */
      muxmdsp_set_tx_gain_adjust (txtc.power);

      /* Start TX */
      txc_change_rf_tx_state(TXC_RF_STATE_START);

      enc_tx_enable (TRUE);           /* make sure power amp is on */
      txc_tx.tx_on = TRUE;
      /* set up the rxc flag so that ffpc will update the setpoint */
      rxc_set_tx_in_traffic_on(TRUE);
      txtc.ready_to_transmit = TRUE;
      /* Events report: Transmitter enabled */
      event_report(EVENT_TRANSMITTER_ENABLED);

      txc_tx.tx_blank = TRUE;      /* expect one blank frame */
    }
    else
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "TX was (%d) prior to CFS, turning it off",
        txc_tx.tx_on );
      txc_tx.tx_on_before_cfs = txc_tx.tx_on;
      txc_stop_tx( FALSE );    /* This is not HHO, set flag to false */
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_TX_ON_OFF_ACTION received" );
  }
}

#ifdef FEATURE_IS2000_1X_ADV
/*========================================================================
FUNCTION TXC_RCP_CMD

DESCRIPTION
   The Radio Configuration Parameters transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void txc_rcp_cmd( mctrans_action_packet_type *pkt_ptr )
{
  /*-----------------------------------------------------------------------*/

  if ( pkt_ptr != NULL )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_RCP_ACTION" );

    txc_process_rcpm_event( txc_rcpm_data, pkt_ptr->action.txc_rcp );

    /* Save local copies of RCPM data */
    txc_rcpm_data = pkt_ptr->action.txc_rcp;

    /* Power Control step size */
    txtc.pwr_ctrl_step_size = txc_rcpm_data.power_control_step;

    /* Write the power control step size for this frame */
    muxmdsp_set_tx_gain_adj_step_size( txtc.pwr_ctrl_step_size );

    /* call into rlgc to update gains & ratios */
    rpc_rcpm_cmd();
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_RCP_ACTION received" );
  }
}
#endif /* FEATURE_IS2000_1X_ADV */

/*===========================================================================

FUNCTION TXC_HHO

DESCRIPTION
  This function is called by MC on a hard handoff boundary to set the
  number of preamble frames to be transmitted and the new nominal power.

DEPENDENCIES
 Relies on the fact that TX_OFF_TX_F has already been sent and TXC is now
 waiting for TX_ON_TX_F.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void txc_hho
(
  word num_preambles,
  word nom_pwr
)
{
  byte power;

  /*-----------------------------------------------------------------------*/

  /* ------------------------------------------------------------
  ** nom_pwr is already sign extended and, if necessary, adjusted
  ** according to nom_pwr_ext.
  ** ------------------------------------------------------------ */
  power = (byte) nom_pwr;

  /* Invert to turn power adjustment into tx gain adjustment */
  /* power is 5 bit two's complement.  After negation, it is stored
   * as two's complement, but the type is unsigned. The sign is not
   * lost because the value of this variable only uses the lower
   * five bits. */
  power = -(power); //lint !e732 Loss of sign (assignment) (int to unsigned char)

  /* Convert to units of 1/2 dB */
  /* power is never more than 2^5 here. After multiplication, 2^6 */
  power = power * 2;//lint !e734 Loss of precision (assignment) (9 bits to 8 bits)

  /* subtract from current gain adjust in negative 2's complement */
  txtc.power = txtc.power + power;
  M1X_MSG( MUX, LEGACY_HIGH,
    "1X HO SUCCESS:TX ADJUST (Updated) TxAdjPwr=%d, nom_pwr=%d <-- (-1/2dB), num_preambles=%d",
    txtc.power,
    power,
    num_preambles );

  if( num_preambles > 0 )
  {
    txtc.num_preambles = num_preambles;
    txtc.preambles = TRUE;
  }
  else
  {
    txtc.num_preambles = 0;
    txtc.preambles = FALSE;
  }

} /* txc_hho */

/*===========================================================================

FUNCTION TXC_WRITE_PCBIT_DATA

DESCRIPTION
  This function is called by RXC to write the PCBIT data.  It must be called
  from RXC context because of timing constraints.  The power control data
  must be written so that it is transmitted two frames after the frame it
  is giving quality information about.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void txc_write_pcbit_data( void )
{
  word pcbit_data;

  /*-----------------------------------------------------------------------*/

  if( (
         (txc_so.fpc_mode == FPC_MODE_PRI_EIB)
         #ifdef FEATURE_IS2000_REL_A
         || (txc_so.fpc_mode == FPC_MODE_PRI_QIB)
         || (txc_so.fpc_mode == FPC_MODE_PRI_QIB_SEC_EIB)
         || (txc_so.fpc_mode == FPC_MODE_PRI_400Hz_SEC_EIB)
         #endif /* FEATURE_IS2000_REL_A */
       ) &&
       // FPC_MODE = EIB implies FCH is active, no need to check below
       ((
         txc_so.rev_fch.included &&
         (txc_so.rev_fch.rc >= CAI_RC_3))
         || txc_so.rev_dcch.included
       )
     )
  {
     /*
     ** PC Bit was enabled in MOD_MISC_CTL when
     ** txc_so.fpc_mode was set.
     */
    pcbit_data = ffpc_calculate_pcbit_data
    (
      txc_eib_flag
      #ifdef FEATURE_IS2000_REL_A
      ,txc_sch_eib_flag
      ,txc_qib_flag
      #endif /* FEATURE_IS2000_REL_A */
    );

    enc_pcbit_test_data (pcbit_data);
  }
} /* txc_write_pcbit_data */

/*===========================================================================

FUNCTION txc_set_action_time_signal

DESCRIPTION This function sets the TXC_ACTION_TIME_SIG for TXC.  This signal
  indicates that there is an TXC action in this frame which needs to be processed.

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
void txc_set_action_time_signal( void )
{
  /*-----------------------------------------------------------------------*/

  M1X_MSG( MUX, LEGACY_HIGH,
    "TXC set action time signal");
  (void) rex_set_sigs( TX_TCB_PTR, TXC_ACTION_TIME_SIG );

} /* txc_set_action_time_signal */

/*===========================================================================

FUNCTION txc_custom_interrupt_reg_func

DESCRIPTION This function is called to obtain a pointer to MC's transaction
            manager's function which handles TXC action time.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void txc_custom_interrupt_reg_func
(
  void (* ticker_func)(mctrans_server_token_type, qword)
)
{
  /*-----------------------------------------------------------------------*/

  /* Store ptr to action time function and call it, if it's not NULL,
  ** every frame to process action time.
  */
  //MSG_HIGH_1("Before INIT txc_at_notifier_func_ptr %d",
      // txc_action_time_notifier_func_ptr);

  txc_action_time_notifier_func_ptr = ticker_func;

  //MSG_HIGH_1("After INIT txc_at_notifier_func_ptr %d",
      // txc_action_time_notifier_func_ptr);
} /* txc_custom_interrupt_reg_func */

/*lint -save -e785 OK to have too few initializers due to ifdef's */
/* Structure to support centralized transaction manager.  This struct contains
** call back functions to set action time signal and to register the timer tick
** used in processing TXC actions, as well as the functions which the MC will
** call to process TXC actions.
*/
mctrans_function_pointer_initializer_type txc_func_pointer_initializers =
{
  txc_set_action_time_signal,
  txc_custom_interrupt_reg_func,
  {
    { MCTRANS_FFPC_ACTION,            NULL},
    { MCTRANS_RLGC_ACTION,            txc_rlgc_trans_cmd},
    { MCTRANS_F_SCH_ACTION,           NULL},
    { MCTRANS_R_SCH_ACTION,           txc_rev_sch_trans_cmd},
    { MCTRANS_HO_ACTION,              txc_ho_trans_cmd},
    { MCTRANS_SCR_ACTION,             txc_scr_trans_cmd},
    { MCTRANS_CFS_NSET_ACTION,        NULL},
    { MCTRANS_CFS_CTRL_ACTION,        NULL},
    { MCTRANS_GATING_ACTION,          txc_gating_trans_cmd},
    { MCTRANS_CTRL_HLD_GATING_ACTION, txc_control_hold_trans_cmd},
    { MCTRANS_ASET_ACTION,            NULL},
    { MCTRANS_LCM_ACTION,             txc_lcm_trans_cmd},
    { MCTRANS_TX_ON_OFF_ACTION,       txc_tx_on_off_trans_cmd},
    #ifdef FEATURE_IS2000_1X_ADV
    { MCTRANS_RCP_ACTION,             txc_rcp_cmd},
    #endif /* FEATURE_IS2000_1X_ADV */
    { MCTRANS_NULL_ACTION,            NULL},
    { MCTRANS_MAX_ACTION,             NULL}
  }
};
/*lint -restore */

/*===========================================================================

FUNCTION txc_register_action_handlers_notifier_interrupt_func

DESCRIPTION This function is called at power up to register the following items
            with MC's transaction manager:
      - call back function to set TXC action time signal
      - call back function to register TXC timer tick
            - call back functions which MC will call to process TXC actions.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void txc_register_action_handlers_notifier_interrupt_func( void )
{
  /*-----------------------------------------------------------------------*/

  M1X_MSG( MUX, LEGACY_HIGH,
    "txc_register_action_handlers called");
  mctrans_register_action_notifier_handler_interrupt_reg_func(
                      MCTRANS_SERVER_TXC,
                      txc_func_pointer_initializers);
} /* txc_register_action_handlers_notifier_interrupt_func */

/*===========================================================================

FUNCTION TXC_TT_TC_INIT

DESCRIPTION
  When transition from TXC_TT_S to TXC_TRAFFIC_S, this function will be called
  to initialize the encoder to be ready for traffic.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void txc_tt_tc_init( boolean turn_on_tx )
{
  #ifdef FEATURE_IS2000_REL_A
  int sr_index;
  #endif /* FEATURE_IS2000_REL_A */

  byte    int_corr;
  boolean ta_is_enabled = txc_is_ta_enabled();

  /*-----------------------------------------------------------------------*/

  /* -------------------------------------------------------------
  ** the interference correction adjustment for the transmit power
  ** note: srch_get_pilot_ecio return pilot_ecio in 1/2db
  ** ------------------------------------------------------------- */
  int_corr = MIN( MAX( (-14 + srch_mux_get_pilot_ecio()), 0), 14 );

  if( !ta_is_enabled )
  {
    /* Set the initial tx power adjustment (stored in -1/2dB units)
       int_corr is 1/2 dB units, txc_tt_tx_adj_boost is in dB units */
    txtc.power = txc_tt_tx_adj_boost * -2;

    /* Check if add interference correction power will be greater than MAX */
    if( ( txtc.power >= 0x80 ) && ( ( txtc.power - int_corr ) < 0x80 ) )
    {
      /* Limit power */
      txtc.power = TXC_PWR_MAX;
    }
    else
    {
      /* Increase power */
      txtc.power = txtc.power - int_corr;
    }
  }
  else
  {
    /* Stop overriding OL */
    muxmdsp_update_tx_open_loop_override( 0 );
  }

  /* Print out power values and normalize to -1/2 dB units */
  M1X_MSG( MUX, LEGACY_HIGH,
    "1X TX TT:TX ADJUST (initialized) tx_adj_power=%d, int_corr=%d,"
    " boost=%d <-- (-1/2dB units)",
    txtc.power,
    -(int_corr),
    (txc_tt_tx_adj_boost * -2) );

  txc_register_action_handlers_notifier_interrupt_func();
  /* Convert to units of 1/2 dB and add rl_gain_adj value
  ** Note that txtc.power is sign inverted.
  */
  txtc.power -= (byte) (2 * txtc.rl_gain_adj);

  /* Copy zeros into frame */
  (void)memscpy( txtc.frame.data,
                 sizeof(txtc.frame.data),
                 zeros,
                 ENC_14400_FULL_RATE_SIZE );

  /* If WTR HOP based inter band HHO/CFS is in progress, 
     then dont update tx voc init here */
  if( ( !mcc_is_hho_with_wtr_hopping_in_prog() )  &&
      ( !mcc_is_cfs_with_wtr_hopping_in_prog() ) )
  {
    muxvoc_tx_tc_init();
  }

  txtc.frame.sig = NORM;          /* no data for interrupt handler */

  txc_pilot_init();

  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  #ifdef FEATURE_PA_ACCESS_PROBE_CONFIG
  /* Notify RF that we are entering traffic state.  Need to do this before
  ** calling rfm_enable_tx_pwr() for traffic state, so RF can set its pa
  ** hysterysis timer appropriately.  When leaving traffic state, RF will reset its
  ** timer when function rfm_tx_disable() is called so it's not necessary
  ** for TXC to call this function with a FALSE parameter.
  */
  #ifdef FEATURE_RF_SVDO_API
  rfm_1x_set_pa_access_probe_config( txc_get_tx_device(), TRUE );
  #else
  rfm_set_pa_access_probe_config( RFCOM_TRANSCEIVER_0, TRUE );
  #endif /* FEATURE_RF_SVDO_API */
  #endif /* FEATURE_PA_ACCESS_PROBE_CONFIG */

  #else /* !FEATURE_MODEM_1X_SUPPORTS_RF */
  M1X_MSG( MUX, LEGACY_ERROR,
    "No RF support: rfm_1x_set_pa_access_probe_config() OR "
    "rfm_set_pa_access_probe_config()" );
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

  /* -------------------------------------------
  ** Power up and program the VHF TX synthesizer
  ** ------------------------------------------- */
  txc_change_rf_tx_state( TXC_RF_STATE_ENABLE );

  if( !ta_is_enabled )
  {
    /* -------------------------------------------
    ** STOP RF to simulate a HHO scenario
    ** ------------------------------------------- */
    txc_change_rf_tx_state( TXC_RF_STATE_STOP );
  }

  /* base on the input, whether MUX should setup for a HHO or a SHO.
  For HHO, all transmitter should be off. For the SHO, the transmitter
  should be on */
  enc_tx_enable( turn_on_tx );
  txc_tx.tx_on = turn_on_tx;
  rxc_set_tx_in_traffic_on( turn_on_tx );
  txtc.ready_to_transmit = turn_on_tx;

  /* Initialize to delay X frames before transmitting
     a signaling messages after a HHO occurs. */
  txc_sig_frame_delay_value = txc_sig_frame_delay_for_tt_value;
  txtc.sig_frame_delay = 0;

  txc_sig_quick_repeat_value = txc_sig_quick_repeat_for_tt_value;
  txtc.quick_repeat_frame_cnt = 0;

  if( turn_on_tx )
  {
    /* Events report: Transmitter enabled */
    event_report( EVENT_TRANSMITTER_ENABLED );
  }

  txc_tx.tx_blank = TRUE;         /* expect a blank first */

  srch_mux_set_tx_adj( txtc.power );  /* set power to last access probe level */

  /* default to no primary or secondary service option */
  txc_so.stored_pri.so = CAI_SO_NULL;
  txc_set_pri_chan_config( 0, CAI_SO_NULL );
  txc_set_sec_chan_config( 0, CAI_SO_NULL );

  #ifdef FEATURE_IS2000_REL_A
  /* set to defautl no service option for each reference ID */
  for( sr_index = 0; sr_index < SR_ID_MAX; sr_index++ )
  {
    txc_srid_ref[sr_index].so = CAI_SO_NULL;
    txc_srid_ref[sr_index].traffic_type = CAI_NUL_TRAF;
    txc_srid_ref[sr_index].phy_channels = 0;
  }

  prim_rate          = CAI_BLANK_RATE;
  prim_data_ptr      = NULL;
  prim_dcch_rate     = CAI_NULL_RATE;
  prim_dcch_data_ptr = NULL;
  sec_fch_num_bits   = 0;
  sec_fch_data_ptr   = NULL;
  sec_dcch_num_bits  = 0;
  sec_dcch_data_ptr  = NULL;

  #ifdef FEATURE_IS2000_REL_A_SVD
  /* txc_rlp_sig_priority is used to keep track of priorities in sending RLP control
  ** frames when RLP is the secondary SO in a SVD call.  The purpose of the
  ** implementation is to improve voice quality in a SVD call.
  */
  txc_rlp_sig_priority.prev_fr_val = DSRLP_TX_NONE;
  txc_rlp_sig_priority.curr_fr_val = DSRLP_TX_NONE;
  txc_rlp_sig_priority.delay_fr_cnt = 0;
  #endif /* FEATURE_IS2000_REL_A_SVD */
  #endif /* FEATURE_IS2000_REL_A */

  txtc.preambles = TRUE;          /* send preambles when we first get here */
  /* Preambles always in frames at TCI */
  txc_hho_preamble_pcg_units = FALSE;

  // Initialize to no pilot gating
  txtc.gating_rate = CAI_GATING_RATE_NONE;

  txtc.num_preambles = 0xFFFF;
  txtc.nulls         = FALSE;          /* not sending null data yet */
  txtc.last_rate     = ENC_8TH_RATE;   /* close as we get to nothing sent yet */

  txc_log_mux_buf.count = 0;

  txc_tx.active_msg = FALSE;
  txc_tx.delay      = TXC_NO_CONF;     /* no confirmation delay */

  txtc.pot_num_mux_pdus = 0;
  txtc.ok_num_mux_pdus  = 0;

  txtc.dtx_duration_included = FALSE;

  if( cdma2kdiag_get_switch( CDMA2KDIAG_SW_DISABLE_RL_MAX_PWR_PROT ) )
  {
    /* If dipswitch 11 is ON, MAX PWR PROTECTION on R-SCH is disabled */
    txc_sch_max_pwr_protection_enabled = FALSE;
  }
  else
  {
    txc_sch_max_pwr_protection_enabled = TRUE;
  }

  /* --------------------------
  ** Go to Traffic Channel mode
  ** -------------------------- */
  enc_set_mode( ENC_TC_MODE );
  enc_user_pn_mask( txc_tx.mask );

  /* Set the counter so that TXC ISR calculates the frame number for
     first NO_OF_ISRS_AFTER_FO isrs. */
  after_fo_handoff_count_isrs = NO_OF_ISRS_AFTER_FO;

  txc_setup_pcg_events_isr( TXC_PCG_EVENT_TRAFFIC, txc_traffic_isr );

  if( txtc.tcb_ptr != NULL )
  {
    /* Notify requesting task TXC is entering traffic state */
    (void) rex_set_sigs( txtc.tcb_ptr, txtc.sigs );
    txtc.tcb_ptr = NULL;
  }

} /* txc_tt_tc_init */

#ifdef FEATURE_IS2000_1X_ADV
/*===========================================================================

FUNCTION TXC_CONFIG_TEST_ACK_OR_NAK

DESCRIPTION
  API for CP task to use when configuring ACK or NAK transmissions for
  SO74 (Markov) and SO75 (Loopback).

  Parameters:
  channel: specifies which R-ACKCH this api is meant for.
  ack_or_nak: Bit mask signifying the test configuration
              for the specified R-ACKCH

  0x0 = Normal Operation
  0z1 = Send Ack every frame
  0x2 = Send Nak every frame

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_config_test_ack_or_nak( txc_rackch_type channel, uint8 ack_or_nak )
{
  /*-----------------------------------------------------------------------*/

  if( ( channel == RACKCH1 ) &&
      ( ( ack_or_nak == 0x0 ) ||  /* TEST OFF */
        ( ack_or_nak == 0x1 ) ||  /*TX ACK*/
        ( ack_or_nak == 0x2 ) )   /*TX NAK*/
  )
  {
    /* Param 1 = Test is enabled
       Param 2 = Send ACK */
    muxmdsp_rl_ack_test_mode_config( ((ack_or_nak != 0x0) ? TRUE : FALSE ),
                                     ((ack_or_nak == 0x1) ? TRUE : FALSE));

    M1X_MSG( MUX, LEGACY_MED,
      "Configure RL ACK Test Mode - channel(%d), ack_or_nak(%x)",
      channel,
      ack_or_nak );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Invalid R-ACKCH test mode, channel(%d), ack_or_nak(%x)",
      channel,
      ack_or_nak );
  }
} /* txc_config_test_ack_or_nak */

/*===========================================================================

FUNCTION TXC_PROCESS_RCPM_EVENT

DESCRIPTION
  This function is called whenever new RCPM data is provided by Call Processing.
  If various parameters of the RCPM change, this function will generate
  an event indicating this.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_process_rcpm_event
(
  cai_rcp_rxc_txc_type old_rcpm,
  cai_rcp_rxc_txc_type new_rcpm
)
{
  uint32 temp_payload;

  /*-----------------------------------------------------------------------*/

  if( old_rcpm.for_fch_blanking_dutycycle !=
      new_rcpm.for_fch_blanking_dutycycle )
  {
    temp_payload = (uint32) new_rcpm.for_fch_blanking_dutycycle;
    event_report_payload (EVENT_1X_ADV_FL_SB_STATUS,
                          sizeof(temp_payload),
                          &temp_payload);
  }

  if(old_rcpm.rev_fch_blanking_dutycycle !=
     new_rcpm.rev_fch_blanking_dutycycle)
  {
    temp_payload = (uint32) new_rcpm.rev_fch_blanking_dutycycle;
    event_report_payload (EVENT_1X_ADV_RL_SB_STATUS,
                          sizeof(temp_payload),
                          &temp_payload);
  }

  if(old_rcpm.for_n2m_ind !=
     new_rcpm.for_n2m_ind)
  {
    temp_payload = (uint32) new_rcpm.for_n2m_ind;
    event_report_payload (EVENT_1X_ADV_N2M_CHANGE,
                          sizeof(temp_payload),
                          &temp_payload);
  }
} /* txc_process_rcpm_event */
#endif /* FEATURE_IS2000_1X_ADV */

/*===========================================================================

FUNCTION TXC_SET_PRI_CHAN_CONFIG

DESCRIPTION
  Set the currently assigned primary traffic channel, includes physical channel,
  service option, etc.

    Physical channels
    FCH  0x01
    DCCH 0x02
    SCH  0x04

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void txc_set_pri_chan_config( uint16 phy_channels, uint16 so )
{
  /*-----------------------------------------------------------------------*/

  TXC_ENTER_ISR_SECTION();
  txc_so.pri.so = so;
  #ifdef FEATURE_IS2000_REL_A
  txc_so.pri.phy_channels = phy_channels;
  #else
  MODEM_1X_UNUSED(phy_channels);
  #endif /* !FEATURE_IS2000_REL_A */
  TXC_LEAVE_ISR_SECTION();
} /* txc_set_pri_chan_config */

/*===========================================================================

FUNCTION TXC_GET_PRI_PHY_CHAN

DESCRIPTION
  Get the currently assigned primary physical channels for traffic.

  FCH  0x01
  DCCH 0x02
  SCH  0x04

DEPENDENCIES
  None

RETURN VALUE
  physical channel mask.

SIDE EFFECTS
  None.
===========================================================================*/
uint16 txc_get_pri_phy_chan( void )
{
  uint16 chan_mask;

  /*-----------------------------------------------------------------------*/

  #ifdef FEATURE_IS2000_REL_A
  TXC_ENTER_ISR_SECTION();
  chan_mask = txc_so.pri.phy_channels;
  TXC_LEAVE_ISR_SECTION();
  #else
  chan_mask = 0;
  #endif /* !FEATURE_IS2000_REL_A */

  return chan_mask;
} /* txc_get_pri_phy_chan */

/*===========================================================================

FUNCTION TXC_GET_PRI_SO

DESCRIPTION
  Get the currently assigned primary service option.

DEPENDENCIES
  None

RETURN VALUE
  primary service option

SIDE EFFECTS
  None.
===========================================================================*/
uint16 txc_get_pri_so( void )
{
  uint16 so;

  /*-----------------------------------------------------------------------*/

  TXC_ENTER_ISR_SECTION();
  so = txc_so.pri.so;
  TXC_LEAVE_ISR_SECTION();

  return so;
} /* txc_get_pri_so */

/*===========================================================================

FUNCTION TXC_SET_SEC_CHAN_CONFIG

DESCRIPTION
  Set the currently assigned secondary traffic channel, includes physical channel,
  service option, etc.

    Physical channels
    FCH  0x01
    DCCH 0x02
    SCH  0x04

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void txc_set_sec_chan_config( uint16 phy_channels, uint16 so )
{
  /*-----------------------------------------------------------------------*/

  TXC_ENTER_ISR_SECTION();
  txc_so.sec.so = so;
  #ifdef FEATURE_IS2000_REL_A
  txc_so.sec.phy_channels = phy_channels;
  #else
  MODEM_1X_UNUSED(phy_channels);
  #endif /* !FEATURE_IS2000_REL_A */
  TXC_LEAVE_ISR_SECTION();
} /* txc_set_sec_chan_config */

/*===========================================================================

FUNCTION TXC_GET_SEC_PHY_CHAN

DESCRIPTION
  Get the currently assigned secondary physical channels for traffic.

  FCH  0x01
  DCCH 0x02
  SCH  0x04

DEPENDENCIES
  None
RETURN VALUE
  physical channel mask.

SIDE EFFECTS
  None.
===========================================================================*/
uint16 txc_get_sec_phy_chan( void )
{
  uint16 chan_mask;

  /*-----------------------------------------------------------------------*/

  #ifdef FEATURE_IS2000_REL_A
  TXC_ENTER_ISR_SECTION();
  chan_mask = txc_so.sec.phy_channels;
  TXC_LEAVE_ISR_SECTION();
  #else
  chan_mask = 0;
  #endif /* !FEATURE_IS2000_REL_A */

  return chan_mask;
} /* txc_get_sec_phy_chan */

/*===========================================================================

FUNCTION TXC_GET_SEC_SO

DESCRIPTION
  Get the currently assigned secondary service option.

DEPENDENCIES
  None

RETURN VALUE
  secondary service option

SIDE EFFECTS
  None.
===========================================================================*/
uint16 txc_get_sec_so( void )
{
  uint16 so;

  /*-----------------------------------------------------------------------*/

  TXC_ENTER_ISR_SECTION();
  so = txc_so.sec.so;
  TXC_LEAVE_ISR_SECTION();
  return so;
} /* txc_get_sec_so */

/*===========================================================================

FUNCTION TXC_IS_TEST_SO

DESCRIPTION
  Returns if the current SO is a test SO or not.

DEPENDENCIES
  None

RETURN VALUE
  TRUE:  Test SOs.
  FALSE: Non-Test SOs.

SIDE EFFECTS
  None.
===========================================================================*/
boolean txc_is_test_so( void )
{
  boolean is_test_so = FALSE;

  /*-----------------------------------------------------------------------*/

  switch( txc_get_pri_so() )
  {
    /* Test service options */
    case CAI_SO_LOOPBACK:
    case CAI_SO_MARKOV:
    case CAI_SO_RS1_MARKOV:
    case CAI_SO_LOOPBACK_13K:
    case CAI_SO_MARKOV_13K:
    case CAI_SO_RS2_MARKOV:
    /* MDR Test service options */
    case CAI_SO_MDR_DPT_FRS1_RRS1:
    case CAI_SO_MDR_DPT_FRS2_RRS2:
    #ifdef FEATURE_IS2000_TDSO
    case CAI_SO_TDSO:
    case CAI_SO_FULL_TDSO:
    #endif  /* FEATURE_IS2000_TDSO */
    #ifdef FEATURE_IS2000
    case CAI_SO_MARKOV_SO54:
    case CAI_SO_LOOPBACK_SO55:
    #endif  /* FEATURE_IS2000 */
    #ifdef FEATURE_IS2000_1X_ADV
    case CAI_SO_MARKOV_SO74:
    case CAI_SO_LOOPBACK_SO75:
    #endif /* FEATURE_IS2000_1X_ADV */
      is_test_so = TRUE;
      break;

    default:
      is_test_so = FALSE;
      break;
  }

  return is_test_so;

} /* txc_is_test_so */

#ifdef FEATURE_MODEM_1X_DEVICE_ONLY_DTX
/*===========================================================================

FUNCTION TXC_DEVICE_ONLY_DTX_IS_ENABLED

DESCRIPTION
  Returns if the Device only DTX is enabled depending on the NV setting and the
  current SO. (NV# 70193)
  Bit  0 - RC8 Fractional Rate
  Bit  1 - RC3 Fractional Rate (except 1/2 rate)
  Bit  2 - RC8 Full Rate
  Bit  3 - RC8 Full Rate
  Bit 15 - Test SO

DEPENDENCIES
  None

RETURN VALUE
  TRUE : If overall device only DTX criteria is met.
  FALSE: Otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean txc_device_only_dtx_is_enabled( void )
{
  boolean dtx_is_enabled = FALSE;

  /*-----------------------------------------------------------------------*/

  do
  {
    /* While CFS is in progress, DTX is disabled */
    if( mcc_is_cfs_in_prog() )
    {
      DTX_DBG_MSG( "1x_DTX : CFS is in progress");
      break;
    }

    /* In FTM mode, DTX is disabled */
    if( onex_nonsig_is_ftm_mode() )
    {
      DTX_DBG_MSG( "1x DTX: 1x in FTM mode" );
      break;
    }

    /* If NULL SO, then service negotiation is not yet completed.
       DTX is disabled */
    if( txc_get_pri_so() == CAI_SO_NULL )
    {
      DTX_DBG_MSG( "1x DTX: SO %d is a NULL SO",
                txc_get_pri_so() );
      break;
    }

    /* If R-FCH is off, DTX is disabled */
    if( !txc_so.rev_fch.included )
    {
      DTX_DBG_MSG( "1x DTX: R-FCH is not ON" );
      break;
    }

    /* If not RC3 and RC8, DTX is disabled */
    if( ( txc_so.rev_fch.rc != CAI_RC_3 ) &&
        ( txc_so.rev_fch.rc != CAI_RC_8 ) )
    {
      DTX_DBG_MSG( "1x DTX: RC%d not supported for DTX",
                txc_so.rev_fch.rc );
      break;
    }

    /* If the frame rate is not valid, DTX is disabled */
    if( tx_frame_rate >= ENC_NUM_FRAC_RATES )
    {
      DTX_DBG_MSG( "1x DTX: Frame rate (%d) is not valid",
                tx_frame_rate );
      break;
    }

    /* If in RC8 Call and RC8 bit in NV is not set, DTX is disabled */
    if( txc_so.rev_fch.rc == CAI_RC_8 )
    {
      if( ( tx_frame_rate == ENC_FULL_RATE ) &&
          ( ( txc_device_dtx_nv.dtx_enabled & DTX_ENABLED_RC8_FULL_RATE ) == 0 ) )
      {
        DTX_DBG_MSG( "1x DTX: RC8 Full Rate NV is %d",
                  (txc_device_dtx_nv.dtx_enabled & DTX_ENABLED_RC8_FULL_RATE) );
        break;
      }

      if( ( tx_frame_rate != ENC_FULL_RATE ) &&
          ( ( txc_device_dtx_nv.dtx_enabled & DTX_ENABLED_RC8_FRAC_RATE ) == 0 ) )
      {
        DTX_DBG_MSG( "1x DTX: RC8 Fractional Rate NV is %d",
                  (txc_device_dtx_nv.dtx_enabled & DTX_ENABLED_RC8_FRAC_RATE) );
        break;
      }
    }

    /* If in RC3 Call and RC3 bit in NV is not set, DTX is disabled */
    if( txc_so.rev_fch.rc == CAI_RC_3 )
    {
      if( ( tx_frame_rate == ENC_FULL_RATE ) &&
          ( ( txc_device_dtx_nv.dtx_enabled & DTX_ENABLED_RC3_FULL_RATE ) == 0 ) )
      {
        DTX_DBG_MSG( "1x DTX: RC3 Full Rate NV is %d",
                  (txc_device_dtx_nv.dtx_enabled & DTX_ENABLED_RC3_FULL_RATE) );
        break;
      }

      if( ( tx_frame_rate != ENC_FULL_RATE ) &&
          ( ( txc_device_dtx_nv.dtx_enabled & DTX_ENABLED_RC3_FRAC_RATE ) == 0 ) )
      {
        DTX_DBG_MSG( "1x DTX: RC3 Fractional Rate NV is %d",
                  (txc_device_dtx_nv.dtx_enabled & DTX_ENABLED_RC3_FRAC_RATE) );
        break;
      }
    }

    /* If the current SO is a test SO and the test SO bit in NV is not set,
       DTX is disabled */
    if( ( txc_is_test_so() ) &&
        ( ( txc_device_dtx_nv.dtx_enabled & DTX_ENABLED_TEST_SO ) == 0 ) )
    {
      DTX_DBG_MSG( "1x DTX: Test SO NV is %d",
                (txc_device_dtx_nv.dtx_enabled & DTX_ENABLED_TEST_SO) );
      break;
    }

    /* DTX is enabled by NV and the Service Negotiation is completed */
    dtx_is_enabled = TRUE;

  } while( 0 );

  return dtx_is_enabled;

} /* txc_device_only_dtx_is_enabled */
#endif /* FEATURE_MODEM_1X_DEVICE_ONLY_DTX */


#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION mc_qsh_mdump_collect_txctraffic_data

DESCRIPTION
This function dumps the txctraffic data to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void mc_qsh_mdump_collect_txctraffic_data()
{
  qsh_mdump_collect_high(&txtc,sizeof(txtc));
  qsh_mdump_collect_high(&txc_device_dtx_static,sizeof(txc_device_dtx_static));
  qsh_mdump_collect_high(&txc_ta_info,sizeof(txc_ta_info));
}
#endif /* FEATURE_QSH_MDUMP */