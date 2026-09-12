/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            C D M A    R E C E I V E   T R A F F I C    M O D U L E

GENERAL DESCRIPTION
  Items associated with traffic channels. Action timers and transaction
  processing

EXTERNALIZED FUNCTIONS
  rxc_frame_types_send_log -  Terminate and send the accumulated forward
  frame types log.
  rxc_tci - Traffic Channel Initialization processing state
  rxc_tc - Traffic Channel processing state
  rxc_log_mar - logs the Forward Traffic Channel Markov test data
  rxc_tc_isr - interrupt handler for the Decoder interrupt used
  when the Receive task is processing the Traffic Channel
  rxc_rlp_frame_log_init - initializes the next frame packet, as well
  as the pointer to it, in the RLP frames log buffer
  rxc_log_rlp_statistics - Logs the current values of the RLP statistics
  to the off-target diagnostic tool
  pwr_msr_check - processes the updated rxc_pwr structure to determine if a
  power measurement report should be sent to MC
  rxc_log_frame_type - populates frame type logs
  rxc_tc_parse - collects the signaling bits of the Traffic Channel
  rxc_tci_init - initializes the Traffic Channel initialization state
  fade_check - processes frame rate decision and updates the fade
  good and bad frame counts, determine if fade report should be sent to
  MC.


INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 1990 - 2020 Qualcomm Technologies, Inc.
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

$Header: //components/rel/1x.mpss/8.0/mux/src/rxctraffic.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
04/17/18   eye     Clear is_so_change flag at the end of call.
01/19/18   ppr     F3 Reduction changes
11/20/17   eye     1x2XTA during SMS/MMS when 1x is on non-DDS sub.
11/20/17   ab      1x2GTA : 0x18CD log packet changes.
08/04/16   eye     SCH logging in 0x119D and FFPC update for 1x2GTA.
07/08/16   nsh     QSH:Enable event diagnostic data to QSH.
07/01/16   nsh     Created get-set API for SO change, call mvs_standby if SO 
                   changes.
06/13/16   eye     Created get-set API for voc_config and reset voc_config
                   after receiving STANDBY_F.
05/18/16   srk     Remove unused SCR complete flag.
05/23/16   nsh     optimized f3 logs.
05/12/16   srk     Use locally stored/passed in RC instead of global variable
                   to avoid race condition between RX Task and TC ISR.
05/05/16   srk     Check for valid RC before processing rxc_tc_isr.
04/21/16   srk     1x2GTA cleanup and TX power shoot fixes.
04/14/16   srk     Optimized TX Trigger fixes and code cleanup.
03/22/16   eye     DS logging changes for 1x2GTA.
02/26/15   eye     Disable FL DRX when FPC mode is 200Hz.
02/19/16   eye     Is_TDSO_call interface between SRCH and MUX.
02/12/16   eye     1x2GTA FFPC set point changes.
02/10/16   eye     1x2GTA terminate QTA gap during unexpected exit.
02/05/16   eye     1x2GTA Feature changes
10/29/15   eye     Reset FO after RX resources are freed.
10/06/15   srk     DRX SRCH response deadlock redesign and cleanup.
09/29/15   eye     Enable DRX only after MT call has been answered.
09/21/15   agh     Remove all references to MVS and cleanup WON feature
08/27/15   jh      Fixed logging and other issues, initial idle div cleanup
08/05/15   eye     Added DRX changes for RC switch and frame off based HHO.
07/27/15   eye     Added get and set API's for DRX global variables.
06/24/15   srk     1x SW support for 1x FW DEMBACK changes in ATLAS
06/22/15   eye     Dont disable data bridge if SW DEMBACK is active.
06/16/15   eye     1x MCVS changes for TA.
06/15/15   eye     Enable HW DemBack first then send RC11 config to FW.
05/05/15   ab      Soft Demback changes for QTA
04/27/15   pap     Resolving m1x_stmr race condition to ensure critical
                   section will not lock with deregister request
04/21/15   srk     DRDSDS and BOLT REVISIT Feature cleanup
04/09/15   eye     Added missing () for is_data API call for DRX check.
03/27/15   eye     Added checks of RC11/3 and no data call for DRX enable.
04/02/15   eye     Release VOC/VFR,when swicth from voice to non-voice SO.
03/30/15   bb      Work around changes to skip SCH ON config
                   to avoid erasure for back to back grant scenarios
03/12/15   eye     Added DRX changes to avoid race conditions with SRCH.
02/27/15   eye     Added initial changes for 1x/1xA DRX feature for THOR.
01/09/15   bb      Changes to enable CDMA_FW_IND_AD1X_LOG_BUFFER_RDY logging
                   if QXDM logs are enabled
11/23/14   eye     Added support for 0x1877 Power Reporting Log Packet.
11/12/14   eye     Added API to get and set MVS variable & reset in call end.
08/27/14   eye     Added mvs_standby variables reset after real mvs_enable.
08/21/14   eye     Added MVS_STANDBY during HHO with frame_offset change.
03/21/14   vks     No need to send RXC_TC_F command to PC isr for timetransfer
03/06/14   pap     Need to release VFR resources when RXC_EXIT_F is called
                   during a voice call termination due to bad frames.
03/05/14   srk     Message macro reduction
03/03/14   srk     Dynamically specify stack size for ISRs.
01/28/14   pap     Aggregating the variables into global structures.
11/18/13   eye     Added sch0 metrics to demod stats sub packet.
12/11/13   pap     Modify the error considered fatal while doing event register
12/07/13   cjb     Changes to support DEMBACK_1
11/25/13   cjb     Switched logic for FEATURE_MODEM_1X_BOLT_OBSOLETE feature
11/11/13   eye     Externalize 0x14ED logging function.
10/28/13   srk     Remove references to obsolete 1xA enum.
10/25/13   cjb     Avoid registering for RX ISR when FW is disabled
09/17/13   eye     Added crtical section for rxc_dsm_arr handling in ISR.
10/11/13   vks     Inform FW about data bridge enable/disable for CH1 as well
10/10/13   cjb     DEMBACK0 vs DEMBACK1: Use DEMBACK ID from FW shared memory
10/09/13   vks     Changes in walsh length mask values for bolt
10/09/13   eye     Modify RX FCH and DCCH rate variables to unambigous names.
10/08/13   vks     Update to new FW interface - fing config has the walsh code
                   and qof
09/27/13   eye     Update RX params in 0x14ED log packet for legacy RC's.
10/04/13   vks     Changes to get SCH conv decode work for bolt.
10/03/13   srk     Update struct member names to reflect correct meaning.
09/27/13   vks     Use rxccommon api to configure ch0 demod for sync/paging
09/13/13   pap     Adding changes for the m1x_stmr module to be integrated.
09/11/13   vks     Inform muxmdsp and m1x stmr about frame offset
08/27/13   pap     Mainlining features FEATURE_MODEM_1X_USE_ATMR_IN_RC11.
08/28/13   cjb     Mainline FEATURE_MODEM_1X_DIME
07/23/13   srk     Replace RF API calls with mux API.
07/23/13   srk     Change the logic of dynamic allocation for log buffer to
                   avoid double freeing.
07/17/13   srk     Fix for 1 frame difference between Tx and Rx information in
                   1xA log packet (0x14EC and 0x14ED).
06/14/13   pap     Aggregating different global variables into structures.
05/31/13   srk     Mainlined FEATURE_MODEM_1X_NON_SIG.
05/14/13   eye     Unifying Device only DTX for RC3/4 and RC8.
05/09/13   cjb     Exporting MEMPOOL capture API outside of 1x-L1.
04/19/13   pap     Adding mempool capture capability for diag and fade timers.
04/17/13   vks     Revert the changes related to ignoring MCTRANS_F_SCH
04/08/13   vks     Ignore MCTRANS_F_SCH if the supplemental decoder has not
                   been enabled
04/02/13   pap     Feature cleanup
02/26/13   trc     Remove Dime TDEC reset workaround
02/15/13   srk     Replace mcs hwio with m1x hwio
02/04/13   trc     Add Dime TDEC reset workaround
01/17/13   bph     KW and compiler warning fixes
12/19/12   bph     Replace memcpy w/ struct assignment or memscpy
12/06/12   vks     Protect against filling and sending ffpc log at same time
10/22/12   jtm     Suspend power measurement and fade checking while a TT HHO
                   is in progress.
10/18/12   srk     Use 1xA debug F3s instead of global static variable.
10/03/12   srk     Log SCH and TX state along with DTX parameters.
09/26/12   jtm     DIME_REVISIT clean up.
09/15/12   ag      Changes for 1xCP memory optimization.
08/21/12   srk     After CFS, re-enable the TX only if TX was ON before CFS.
08/16/12   jtm     Fix double FCH/DCCH decoder data read.
08/13/12   eye     After PMRM is sent, Bad frame counter adjustments.
08/02/12   srk     Added support for Device only DTX for 1x Advanced calls.
07/19/12   eye     After PMRM is sent, Total and Bad frame count adjustment.
07/16/12   srk     Removed FEATURE_IS2000_REL_A_CC
07/11/12   srk     Changed the returned values from RF to match legacy units
                   for proper tx power logging.
06/28/12   srk     Mainlined FEATURE_IS2000_REL_A_CC.
06/21/12   jtm     Mainlined FEATURE_IS2000_REL_A_CC.
06/05/12   eye     Removed deregistration of FW for 1x Adv logging and add
                   checks to prevent race condition.
05/31/12   srk     Removed unnecessary F3s for optimized logging.
05/30/12   srk     Changes to remove half PCG offset in 1x advanced logs.
05/15/12   srk     Call TX wrapper function to get TX power instead of directly
                   calling RF API.
04/16/12   jtm     Added support to vote for 1xA clock resources in 1xA RCs and
                   moved QOF FCH Noise Estimation to SRCH.
04/04/12   trc     Convert to m1x_time module
03/28/12   srk     Mainlined FEATURE_DS
03/16/12   jtm     Return RXC_BADCMD_S for illegal commands from MC.
02/24/12   srk     Mainlined FEATURE_IS2000_P2 and FEATURE_IS2000_SCH
02/24/12   jtm     Vote for DATA clock resources when SO changes to DATA for
                   the duration of the call.
02/17/12   srk     Replaced the tramp api with DAL Interrupt Controller.
02/16/12   srk     Feature Cleanup: Removed FEATURE_GPS_MODE
01/31/12   srk     Feature Clean Up
01/23/12   srk     Feature Cleanup
01/17/12   jtm     Revert REL_A feature clean up.
01/06/12   srk     Added defensive checks to prevent null pointer deferencing
01/03/12   jtm     Use local function ptr to save the transaction time notifier
                   function and prevent concurrency issues.
01/03/12   srk     Enable QOF Noise Estimation Feature for legacy RCs
12/13/11   srk     Feature clean up: Mainlined FEATURE_GPSONE_OVERRIDE_RDA and
                   changed all FEATURE_IS2000_REL_A_* to FEATURE_IS2000_REL_A
12/12/11   jtm     Register for 1xA FW logging during TCI and deregister when RX
                   resources are torn down.
12/08/11   jtm     Only enable APF when in a 1xA RC traffic call.
12/01/11   srk     Added QOF FCH Noise Estimation support.
11/22/11   srk     Feature clean up: Mainlined and Internalized Features
09/28/11   srk     Mainlined Always On features and removed Always Off Features
08/18/11   adw     Mainline 1x message router support.
08/17/11   trc     Mainline RF SVDO API
08/15/11   jtm     Klocwork Fixes.
06/30/11   vks     Clean up 1xA resources before the regular cleanup.
06/28/11   vks/jtm Changes to get 1xA working on nikel.
06/17/11   jtm     Fixed syntax errors.
06/15/11   adw     Use the current RF device when querrying Rx and Tx AGC.
05/24/11   jtm     Use legacy N2m value when FL SB is disabled. Load RCPM
                   info from TCI command immediately before setting up the TC
05/18/11   vks     Replace feature MSMHW_TURBO_CODING with
                   FEATURE_MODEM_1X_SUPPORTS_TURBO_CODING
05/11/11   jtm     Removed checking for blankable SO.
04/27/11   jtm     Hook in MSGR support for 1xA Logging and use the ATMR for
                   RC11 decode done processing if FW ISR support is not avaliable.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
03/31/11   jtm     Fix for 1xA FET logging. Align Rx frame with ACK Txed.
03/31/11   adw     Clear decoder done interrupt status on NikeL.
02/25/11   jtm     Remove mvs_enable after successful RIF HHO.
02/11/11   jtm     Added RCPM FPC_MODE support for 1xA.
01/28/11   jtm     Additional FTM decoupling.
01/28/11   jtm     Added Mux Vocoder interface module.
01/28/11   vks     Featurize RF interface with FEATURE_MODEM_1X_SUPPORTS_RF.
01/24/11   vks     Update to new RF api for RF SVDO feature development.
01/21/11   vks     Use 1X specific RF SVDO feature - FEATURE_1X_RF_SVDO_API.
01/20/11   jtm     Added rxc_is_blankable_so. Mark all frames as guaranteed
                   if the SO doesn't support smart blanking.
01/19/11   vks     More register featurization for nikel.
01/18/11   jtm     Lint clean up.
01/12/11   jtm     Check for RC11 and SCH enable to avoid calling dec_read_fch_data
                   a second time during F-SCH processing.
12/28/10   ag      Decoupling of SO73 from 1xAdv.
12/17/10   jtm     Mainlined FEATURE_MVS_MIGRATE under FEATURE_MODEM_1X_VOICE_SUPPORT.
12/15/10   jtm     Feature cleanup.
12/14/10   jtm     Mainlined FEATURE_SRCH_CLKREGIM_RM and cleaned up unneeded
                   legacy clock code.
12/14/10   jtm     Removed MDR/SCCH support.
12/14/10   jtm     Featurized include of voc.h.
12/13/10   jtm     Feature guard FEATURE_MVS_MIGRATE with
                   FEATURE_MODEM_1X_SUPPORTS_VOICE.
12/09/10   jtm     Removed FEATURE_IS2000_REL_D support.
12/09/10   jtm     Remove obsolete RXC_* commands.
12/02/10   jtm     Added the capability to track the synchronization
                   of MVS and TXC and report errors.
                   Renamed rxc_is_loopback_call to rxc_is_loopback_so.
11/17/10   jtm     Added rxc_is_loopback_call and included SO75.
11/15/10   jtm     Initial SU API split for CP.
11/05/10   jtm     Added more _i.h includes for Modem 1x SU API support.
11/01/10   jtm     Private file clean up.
10/30/10   jtm     Modem 1x SU API changes.
10/30/10   jtm     Integrate from rel/1h10.
10/21/10   jtm     Enable TD_CLK at the start of a turbo F-SCH. Only disable
                   TD_CLK after the call has been completely torn down.
10/19/10   jtm     Exclude RL PCG 0 and FL PCG 15 from ACK counter incrementing.
10/08/10   jtm     Move 1xA counters here and add new MAC counters.
10/07/10   jtm     Provide correct RX frame number to 1xA FET log.
09/08/10   jtm     Store the frame number of the decoded frame for RC11 1xA logging.
09/02/10   jtm     Applied rxc_frame_offset to the smart blanking dutycycle.
09/02/10   jtm     Enable TD clocks whenever needed to write to TD registers and
                   when required for turbo coded F-SCH decoding.
08/31/10   jtm     Added F3 messages to all transaction functions.
08/27/10   jtm     Replaced MUXMDSP macros with equivalent functions.
08/26/10   jtm     Update SRCH when the current SO specifies a data call
08/25/10   jtm     Removed intlocks from within rxc_atmr_isr().
08/19/10   jtm     Decouple inhibit vocoder enable from FEATURE_MVS_MIGRATE.
08/10/10   vks     Cleanup Genesis Revisit feature.
08/10/10   jtm     Added RC11 entries to rxc_spr_rate_tab and rxc_rnd_factor_tab.
08/06/10   jtm     Replaced fwd_rc_to_rate_set with rxc_get_fwd_link_rate_set().
08/06/10   jtm     Disable 1xA Debug messages by default.
08/06/10   jtm     Added 1x Advanced support when REL_A is not enabled.
08/04/10   jtm     Notify SRCH when entering and leaving 1x Adv traffic.
07/22/10   jtm     Fixed merge error.
07/06/10   jtm     Fixed compiler warnings.
06/30/10   jtm     Do not increment N2m bad frame counter if the frame is
                   not a guaranteed frame. Calculate if the current frame is
                   guaranteed at the time the 1xA logs are filled.
06/24/10   jtm     Use CAI_IS_SO_VOICE to check for valid voice service options.
                   Do not enable the vocoder or use its APIs for unsupported
                   voice SOs.
06/21/10   jtm     Corrected FET logging. RL and FL ack masks were swapped.
                   On traffic teardown, flush the remaining contents of the
                   log if anything exists.
                   Fixed logic error in fade_check_good_frame_non_hho().
06/18/10   jtm     Modified FEATURE_MODEM_1X_INHIBIT_MVS_ENABLE logic to use a
                   seperate flag to indicate that the vocoder should not be
                   enabled until commanded to by MC. (rxc_inhibit_mvs_enable)
06/17/10   jtm     Wrap mvs inhibit and RXC_MVS_ENABLE_F logic with
                   FEATURE_MODEM_1X_INHIBIT_MVS_ENABLE.
06/15/10   jtm     Added is_guaranteed parameter to fade_check().
                   Renamed valid_frame to is_guaranteed_frame.
                   Added rxc_good_guaranteed_frame counter to count the number
                   of consecutive good guaranteed frames received which can be
                   independent of regular consecutive good frames if smart
                   blanking is enabled.
                   When in a MSO call, still read out all bits from the decoder
                   so they can be handed to mar_rx for bit error analysis even
                   when the rate decision is erasure. (FEATURE_MSO_RDA_TEST)
                   Count only guaranteed frames for power measurement reporting.
                   Cleaned up 1xA Logging. Added new 1xA Perf Log format and
                   FET Stats log. Saved additional 1xA information to log.
                   SO74 and S075 Ack/Nak test directive support added.
06/10/10   jtm     Eliminate dependency on log_dmss.h
06/10/10   jtm     Added support for RXC_MVS_ENABLE_F and added
                   the inhibit_mvs_enable flag to the tci command from CP
                   to allow CP control to delay the vocoder configuration.
05/24/10   bb      Delay Rx ATMR by 3 PCG during loopback calls
05/14/10   vks     Replace MUXMSM_AGC_CTL_MASK_DELAY macro call with function
                   muxmdsp_set_agc_ctl_mask_delay()
05/21/10   trc     Add support for MVS_MODE_4GV_NW in MVS_DTX_OFF mode
05/14/10   jtm     Disable Turbo Decoder Clocks in Voice Calls to save current.
05/10/10   jtm     Clear F-FCH SER and FER stats during traffic init.
05/05/10   jtm     Added logic to call mvs_enable() after a successful RIF HHO
                   since mvs_acquire() is delayed in CP in these scenarios.
05/05/10   jtm     Added support for new RLP API using FEATURE_IS95_RLP_MUX_COMMON_API
04/12/10   jtm     Moved valid frame determination from mDSP to SW for
                   1x Advanced. Changed 1xA Logging to use static memory.
                   Moved ACK detection debug messages out of ISR space.
                   Corrected Frame Number value used in logging.
                   Changed valid_ffpc_frame flag to valid_frame.
                   Added error messages to signal when the channel config
                   is invalid.
04/02/10   jtm     Replaced rlp_api.h with mdrrlp_api.h
02/04/10   jtm     Removed FEATURE_PLT.
01/29/10   jtm     Removed include of snd.h. Removed feature TMOBILE.
01/25/10   adw     Fixed negative indentation causing Lint warning.
12/09/09   jtm     Updated 1xA logging mechanism to align PCGs of each frame.
                   Check that Tx is enabled prior to enabling 1xA logging.
12/01/09   jtm     Added rxc_atmr_handle_is_valid().
11/24/09   jtm     Moved dtx_non_crit_fch_frame to the txtc structure.
                   Removed 1xA debug messages. Replaced by 1xA logging.
                   Added RCPM and Tx information to 1xA log.
10/28/09   jtm     Added support for MSO74 and SO73.
10/09/09   jtm     Added 1xA logging support. Added missing RC11 RDA log data
                   gathering.
10/08/09   vks     Moved mdsp read/writes in muxmsm.h to muxmdsp.h and
                   renamed these macros from muxmsm* to muxmdsp*
09/24/09   jtm     Klocwork fixes.
09/22/09   jtm     Corrected compiler warnings.
09/14/09   adw     Changes to support modem data header CMI refactoring.
08/26/09   adw     Removed deprecated ts.h header and added time_cdma.h.
08/25/09   jtm     Added 1xA debugging messages.
08/22/09   jtm     Lint Fixes.
08/21/09   jtm     Lint Fixes.
08/11/09   jtm     Replaced deprecated ts.h api's with new time api's.
08/05/09   jtm     Lint fixes.
07/29/09   jtm     Added FTM support for 1x Advanced RCPM.
07/06/09   jtm     Update to use cai_rcp_rxc_txc_type. Mainlined the use of
                   FEATURE_IS2000_1X_ADV in tables sized using the number of
                   RCs, (which now includes RC11).
06/06/09   vlc     Added support for 1x Advanced (RC11.)
05/20/09   jtm     Added include for rx_v.h for CMI clean up.
04/21/09   jtm     Replaced assert.h with amssassert.h. Assert.h is being
                   phased out.
04/17/09   jtm     Added includes to fix compiler warnings.
04/06/09   mca     Added RF SVDO API support
04/06/09   jtm     Moved FEATURE_DS_IS2000 inside rxc_init_sch_mux() since this
                   function is exposed in the au/api and must have a definition.
04/01/09   jtm     Featurized necessary includes.
03/31/09   jtm     Eliminated implicit includes.
03/20/09   jtm     Cleaned up header file inclusions.
03/17/09   jtm     Replaced INTLOCK/INTFREE, INTLOCK_SAV/INTFREE_SAV, and
                   MSM_LOCK/MSM_UNLOCK with macros that can be mapped correctly
                   based on the compiler and controlled internally to RXC.
03/03/09   jtm     Lint fixes.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
09/12/08   vlc     Added support for FEATURE_MVS_MIGRATE.
07/21/08   jtm     Fixed Klockwork Errors. Removed FEATURE_IS2000_REL_D_DV and
                   FEATURE_IS2000_REL_C_DV code. Replaced code within rxc_tci_init() with function
                   rxc_configure_ffpc_channel() since it is duplicated elsewhere.
07/01/08   vlc     Merged code changes from old tip archive.
04/10/08   trc     Switch clock mgmt strategy via FEATURE_SRCH_CLKREGIM_RM
03/05/08   vlc     User clock resource manager to manage RX clocks
                   (FEATURE_CLKREGIM_RM.)
11/29/07   vlc     Fixed lint errors.
02/23/07   vlc     Fixed compiler warnings/errors when SCH, Rel A, and P2
                   features are turned off.
11/22/06   vlc     Fixed RVCT compiler warnings.
09/19/06   vlc     Fixed lint errors.
07/13/06   vlc     In rxc_hho_begin_cmd(), reset lost frame count if the RIF
                   timer is set.
01/26/04   vlc     Added code to increment T79 timer to fade_check_fch() and
                   fade_check_dcch() under FEATURE_IS2000_REL_D.
06/13/06   vlc     In rxc_tc_isr(), added check for decoder rates.  In case
                   decoder rates are invalid, perform rate determination
                   algorithm.
06/12/06   awj     #include tramp.h; no longer included through clk.h
06/09/06   trc     Remove T_MSM6700 featurization
05/08/06   vlc     Added return if failure indicator to HHO begin action.  This
                   change is required so the return if failure timer won't
                   be activated in cases where return if failure is FALSE.
04/17/06   vlc     Added debug information in case RXC time line is extended over
                   20ms frame.
09/08/05   ejv     Use new srch_mux interface calls.
08/29/05   bn      Clear the all the decoder status it after installing the isr
07/26/05   rkc     Clear all service options upon reception of RXC_RELEASE_F.
07/25/05   rkc     Fixed lint errors.
07/18/05   vlc     Modified RXC to not allow GPS tune away if RXC is in fade
                   condition for 12 consecutive frames.  Also check TX enable
                   flag to make the check for GPS more strict, to avoid
                   possible race condition between MC, GPS Search and TXC/RXC.
05/09/05   vlc     Added check for running out of DSM buffers for rel A branch.
                   If the number of DSM buffers obtained is less than number
                   requested, return all DSM buffers and try again next frame.
04/29/05   vlc     Added 4GV support under FEATURE_VOC_4GV.
04/27/05   vlc     Main lined code in current targets, cleaned out obsolete targets,
                   cleaned out obsolete features.
04/08/05   fc/vlc  Added support for FEATURE_IS2000_REL_D TCI to IDLE state
                   transition (for DCA - Direct Channel Assignment).
03/14/05   vlc     Fixed compiler warnings for MSM6700.
02/24/05   vlc     Lock interrupt between clearing DEC_DONE_INT_STATUS and
                   installing RXC ISR.
02/23/05   vlc     Clear the DEC_DONE_INT_STATUS when setting RXC ISR.
01/26/04   vlc     Moved code to increment T79 timer to fade_check() to keep accurate
                   count of frames, rather than in fade_check_bad_frame().
01/26/05   vlc     Removed obsolete ds.h inclusion.
01/25/05   jyw     Added featurization to cover rxcpdcch_isr.
12/07/04   ejv     Added rxc_spr_rate_tab and rxc_rnd_factor_tab.
11/12/04   bn      MUX no longer need to call srch_mux_update_sch_walsh_len().
                   the new interface, SRCH has the walsh len info
11/09/04   vlc     Added support for Rel D Fast Call Set Up Bypass 2 Good Frame
                   feature.
09/28/04   sr      Merged in changes from 6550 2009 build
09/03/04   jyw     Added the relD plt support
09/01/04   jrp     Added call to muxdvlog_process_frame_boundary() for DMSS build.
08/17/04   jyw     Added the rxcpdch_plt_process_pdch in the PLT featurization
08/11/04   vlc     In TCI state, only free traffic channel resources if RXC
                   fails to go to TC state.
08/09/04   sr      Added line to rxc_config_sch_hw() to clear PDCCH bit in
                   DEM1X_CHANNEL2 register for MSM6700
08/06/04   sr/va   Set rxc_sent_bad_rpt to TRUE unconditionally in
                   rxc_hho_begin_cmd()
08/06/04   vlc     Added code to free traffic channel resources when existing
                   TCI state (similar to TC state.)
07/30/04   vlc     Added support for rel C TDSO.
07/27/04   jrp     Fixed casting problem for 7500 compiler.
06/24/04   sr      Added 2nd level interrupt support for dec_done_int in ISR
                   plus misc clean up for DV
06/24/04   sr      Put back rxc_clear_rlp_statistics()
06/16/04   sr      Merged in changes from 6500 5004 build
06/14/04   sr/va   Added HHO cleanup under FEATURE_HHO_ENHANCEMENTS
06/01/04   vlc     Added support for F-PDCH.  Changed function rxc_log_tc_msg()
                   to account for the addition of RXC_FOR_PDCH in rxc_dsch_type.
05/13/05   bn      Fixed errors in PDU5 parser functions
05/12/04   vlc     Added MCTRANS_MAX_ACTION under feature FEATURE_TRANS_MANAGER.
05/07/04   vlc     Implemented new RLP interface for EV-DV.  The new interface
                   replaces the current interface for 1x channels as well.
                   Also, modified RLP logging extensively to work with new
                   interface.
                   Added MCTRANS_MAX_ACTION.
05/06/04   ll      Fixed setpoint adjustment problem after CFS.
04/28/04   jrp     Reverted change of MSM_LOCK to INTLOCK_SAV to fix compilation
                   error with new search headers.
04/26/04   sr      Added rxcpdch initialization
03/16/04   sr      Made TEMP_HACKS_FOR_DV depend on T_MSM6700
04/27/04   vlc     Added function rxc_check_for_sch_update() to fix rate change
                   issue for bursts with 1 frame gap.  Remove similar code
                   from rxc_tc_isr().
04/20/04   jrp     Fixed lint warnings.
04/14/04   sr      Removed unused ZIF logging code
04/08/04   bn      Add SCH DTX frame counter in the F3 debug message
03/10/04   vlc     Added support for centralized transaction engine handling
                   under feature FEATURE_TRANS_MANAGER.
03/05/04   sr/bkm  Moved clearing of frame offset from MC to RXC_EXIT_F and
                   RXC_IDLE_F
03/03/04   sr      Changes for DV plus included changes to MSMSHARED tip
02/10/04   vlc     Need to check for STDSO and TDSO for F-SCH, as well as
                   3G data calls (change in function rxc_tc_isr()).
01/06/04   vlc     Removed macro DEMOD_REP_FACTOR from function
                   rxc_config_mac_for_ch1().
12/15/03   jw      In function rxc_shutdown_sch()
                   Do not disable the sch OLPC in case of 1 frame gap.
10/22/03   vlc     Fixed RVCT compiler warnings.
10/22/03   sr      Fixed compilation warning errors when using RVCT compiler
10/21/03   vlc     Re-instated the use of MUXMSM_GET_TD_STATUS_DONE macro
                   instead of accessing the register directly.
10/20/03   bn      Reset the DCCH,FCH data valid when entering TCI state
10/20/03   vlc/ejv Modified MUXMSM_GET_TD_STATUS_DONE macro call for 6500.
10/17/03   vlc     Removed superfluous F3 messages.
10/06/03   vlc     Added #include rxcbcch.h.
09/18/03   ejv     Update rxc_sw_turbo_beta for 6500.
09/10/03   dnn     Changed FEATURE_TC_FRAMES_INTEGRATION to
                   FEATURE_MSM6500_TC_FRAMES_INTEGRATION.
09/10/03   dnn     Added frame offset reprogram to FEATURE_TC_FRAMES_INTEGRATION.
09/09/03   dna     Merged porting changes for MSM6500
09/04/03   bn      Removed 16x, RC3 energy saturation workaround by putting
                   back the correct beta value.
09/04/03   bn      Corrected the beta value for CH1 MAC, since we still use
                   the old Tiger values for the COMB_RND3, we need to derive
                   the new beta value to matche it.
08/08/03   vlc     Use dec_dcch_status_type instead of dec_fch_status_type for
                   rxc_dcch_status.
08/01/03   vlc     Confirm current call has RLP service before reading SCH data.
07/29/03   ejv     Update rxc_sw_beta_sch for 6500.
07/24/03   ejv     Added call to srch_mux_update_sch_walsh_len(...).
07/23/03   vlc     When switching FPC Pri Chan, make sure to re-configure the
                   DEMOD with the appropriate RC in function
                   rxc_ffpc_trans_cmd().
07/18/03   vlc     Release A changes:  Changed FCH and DCCH
                   channel set up to better indicate when decoder data is
                   available for the appropriate channel(s) for the RXC layer.
06/27/03   bn      Corrected the Beta value for F-SCH TURBO 32, RC4
06/16/03   bn      Fixed the "out of buffer dec_free_q" problem.
06/10/03   jrp     Read FPC and RPC history from mDSP rather than hardware.
05/29/03   vlc     For SVD, created fade_check_fch() and fade_check_dcch() to
                   replace fade_check().  Reset various fade counters upon
                   primary power control channel switch.
05/28/03   vlc     Added support for FEATURE_IS2000_REL_B traffic to idle
                   transition.
05/22/03   vlc     Fixed compiler warnings for MSM6000 target.
04/18/03   bn      Removed JCDMA Features in FER so that it can apply to DIAG
03/20/03   bn,sr   Added logic to support simultaneously monitor BCCH and FCCCH
04/02/03   vlc     Added call to new deint functions deint_atmr_isr2 () and
                   deint_atmr_isr2_init () for PLT mode.
03/11/03   jrp     Corrected generation of QIB and EIB in the case of both
                   FCH and DCCH active.  Now, calculates using information
                   only from the primary channel.
02/26/03   jrp     Changed diagnostic message each SCH erasure to give only
                   summary of erasures every 100 frames.
02/25/03   vlc     Changes are for MSM6100, Release A:
                   Added new function rxc_atmr_isr2().  Added call to new deint
                   functions deint_atmr_isr2 () and deint_atmr_isr2_init ().
                   These functions support the use of a second action timer
                   ISR which is set to go off at the 4th bin in a frame to
                   accommodate SCH Turbo timing changes.
01/31/03   bkm     Adjusted msg level on "Start MC txn" F3 message
01/21/03   vlc     Changed struct pwr_on to pwr_rep_delay when processing
                   RXC_PWR_REP_DELAY_F.
                   Shortened Turbo CRC fail F3 message.
                   Put turbo SCH interrupt handling change to 3rd bin under
                   release A feature.
12/12/02   ph      rxc_tc_parse() logs signaling channel correctly now.
12/05/02   vlc     Removed MuxPD5 debug message occuring once per frame.
11/26/02   sr/vlc  For MSM6100, changed turbo SCH task ID to DEC_TURBO_SCH_TASK
                   (corresponds to 3rd bin for the dummy task.)
11/19/02   hxw     Modified gps tune-back logic to wait 4 good frames.
10/16/02   jrp     Fixed SCH EIB to write 1 in case of DTX - not just in case
                   of erasure.  Also, call to write pcbit data is made in Rx
                   context rather than sending to Tx via a static variable.
10/15/02   dlb     Added new power control logging to combine power control
                   and frame type logging.
10/14/02   bn      Using the correct feature F_SCH in PLT
10/10/02   bn      Fixed the in-corrected increasing of the DTX counter in FSCH
                   turbo PLT logging. In PLT, printout the first few bytes of FSCH
10/08/02   az      Added support for voice SO SMV
09/19/02   bgc     Merged feature changes for MSM6000 for IS2000_SCH and MDR.
09/18/02   vlc     Function rxc_init_sch_mux() now check for NULL ptr before
                   getting DSM buffers.
08/06/02   cr      Added MSM6100 changes.
08/27/01   vlc     Changed power measurement report process for VP2.
08/08/02   vlc     Fixed compiler error when FEATURE_IS95B_MAHHO is turned off.
08/02/02   vlc     Updated rxc_tc_isr() to call ffpc_update_setpt() for both
           hrk     FCH and DCCH in VP2 mode regardless of the primary power
           bn      control channel selection.
           jrp     Now keep frame status separately for FCH and DCCH in
                   rxc_tc_isr().
                   As a result of these fixes, modified DCCH frame supervision
                   code and QIB/EIB handling accordingly.
                   Also fixed featurization in rxc_tc_isr() so that SVD will work
                   even when feature P2 is not defined.
08/01/02   jrp     Added featurization for FPC modes 4,5,6.  Changed FPC_MODE enums
                   to be more descriptive due to SVD.  Now modifies TXC QIB and EIB
                   flags directly, both set and reset (used to set in RXC and reset
                   in TXC each frame).  Fixed null pointer write in rxc_tc_isr().
                   Determines QIB now in SVD section of rxc_tc_isr().
07/23/02   vlc     Modified rxc_ffpc_trans_cmd() to allow switching primary
                   power control channel when only 1 channel is active.
07/16/02   bgc     Featurized DCCH in FEATURE_IS2000_P2.  Featurized SCH in
                   FEATURE_IS2000_F_SCH.  Featurized pilot gating in
                   FEATURE_IS2000_CHS and P2.
07/11/02   bn      Added logic to support new QIB algorithm
06/18/02   jrp     Added support for FFPC Modes 4,5,6.
06/13/01   vlc     Reset fundicated channel deinterleaver state in function
                   rxc_free_tc_resources().
06/07/02   lcc     Removed unused variable in rxc_control_hold_trans_cmd.
06/04/02   vlc     Added ability to switch primary power control channel for
                   VP2.
06/04/02   bn      Modified the FDCH logging function parameters for VP2
06/03/02   bgc     Adjusted ifdef to allow one to turn off FEATURE_IS2000_P2.
06/03/02   lcc     Merged back changes in r1.92 overwritten by r1.93.
05/31/02   lcc     1. Added support for control hold gating.
                   2. Corrected programming of reverse power delay to be like
                      MSM6050.
05/30/02   vlc     VP2 changes are now featurized under FEATURE_IS2000_REL_A_SVD
                   and FEATURE_IS2000_P2 per request of development team.
                   Implemented comments from VP2 code review.
05/16/02   bgc     Featurized rxc_sch_frame_status inside FEATURE_IS2000_F_SCH.
05/13/02   vlc     Added support for simultaneous DCCH & FCH channel set up
                   (feature FEATURE_IS2000_REL_A_VP2.)
05/10/02   bn      Fixed the cause of Discrepancy in Forward Frame Type and
                   Fast Forward Power Control Packet
04/19/02   jrp     Added support RXC_GET_SETPT_F.
04/18/02   az      Added support for new voice service option SMV (SO 56)
04/03/02   bn      Added logic to flush the RDA and FDCH logs upon existing the call
03/27/02   bn      fixed a possible mis-alignment error in FDCH log
03/05/02   ems     Merged in changes for MSM6050.  Added zif logging code.
02/27/02   lad     Merged in IS2000 Release A Common Channels:
                   Replaced inline code with rxc_clear_decoder_queue().
                   Moved action timer alloc from rxcpaging to rxc_tci_init().
                   Added rxctraffic_frame_bnd_init() for timing-specific
                   config needed for starting traffic channel state.
                   Moved TCI code to rxc_tci_init().
02/27/02   vlc     Merged the following changes from SVD branch.
           lh      Added back data abort protection code when parsing MuxPDU 5.
                   secondary to primary traffic
           lh      Removed data_ptr even/odd checking because TDSO now frees
                   dsm items after processing them.
           vlc     Removed printf message in function read_mux_pdu5_sch_data().
                   Added MuxPDU 5 counter maintenance. (lh)
           bn      Added support for MUX SVD signaling
02/27/02   az      Featurized 13K voice service option.
02/04/02   bn      Featurization the PLT SCH logging for REL_A
01/28/02   sy      Changed logging to point to sr_id instead of sr_id_index.
01/22/02   vlc     Determine rate set of forward SCH in rxc_update_sch_info().
01/14/02   bgc     Added fixes for reverse power control delay problem.
01/12/02   sst     Modified to use the new srch_* files that replaced srchdrv.c
                     Includes a function name change:
                       srch_get_vsc_time -> srch_scomb_get_vsc_time
01/10/02   bn      Added support for PLT SCCH logging
01/03/02   bgc     Added SCH featurization to turn off for MSM6000.
12/10/01   hrk     Renamed rxc_erasure_flags to rxc_eib_flags. Renamed
                   rxc_fch_frame_status to rxc_fdch_frame_status.
                   Moved function rxc_flag_erasure() from rxccommon.c to this
                   module and renamed it to rxc_flag_eib().
12/10/01   hrk     Renamed RXC_FULL_FRAME to RXC_GOOD_FRAME.
12/07/01   bn      Used the correct FEATURE_DCCH_FRAME_QUALITY_SUPERVISION
                   instead of FEATURE_CHAN_SUP_TCI_INIT
11/27/01   hrk, bn Wait for spurious interrupts to get done before starting
                   FFPC logging.
11/13/01   bn      Added the enhance DCCH DTX detection and channel supervision logics
11/12/01   sr      Merged 1.58.1 branch back to trunk
11/09/01   sr      Removed call to rxc_free_tc_resources() in
                   fade_check_bad_frame() and RELEASE_F. Added call to
                   rxc_free_tc_resources() when we exit the rxc_tc() while loop
11/07/01   hrk     Setting rxc_log_ffpc_sch_data=FALSE in rxc_free_tc_resources().
10/30/01   bn      Added support for Release A FDCH logging
10/26/01   hrk/bn  Disable FFPC logging for RC less than or equal to RC2.
10/16/01   hrk     FFPC logging is done in the context of FCH decoder ISR.
                   The setpt values reported for both FCH and SCH correspond
                   the values at the beginning of the frame in which they are
                   reported.
10/05/01   lcc     Added handling of MCTRANS_NULL_ACTION in rxc_service_trans_cmd.
09/20/01   lcc     1. Removed clearing of fundicated channel frame counters when
                      PMRM is sent due to ending of F-SCH burst which shouldn't
                      have been done.
                   2. Calls rxc_init_action_item in rxc_tci_init.
09/27/01   bn      Fixed log packet exists and contains bursts of F-SCH info
                   during SCCH calls
09/27/01   bgc     Added ifdef for a warning when FEATURE_RLP_LOGGING is off.
                   Added ifdef of IS2000_SCH in rxc_atmr_isr() for SCH
                   processing.
09/21/01   lcc     Corrected an error in an "if" statement while waiting for
                   N5M frame in rxc_tci which cause erasures to be considered
                   good frames.
09/14/01   rama    SR_ID fix. Differentiate between SR_ID and RLP instance.
                   Changed ds_curr_sr_id to ds_curr_sr_id_index.
09/21/01   bgc     Added initializations for DTX filter.
                   Moved rxc_dtx_det_sch() to rxcrda.c
08/28/01   lh      Back out SI bits changes for Release A Addendum 2.
08/28/01   bn      Fixed forward frame log types packet
08/22/01   bn      Support DCCH in FTM mode
08/15/01   sr      Fixed REL_A featurization
08/13/01   sr      Removed unnecessary if() condition to check validity of RC;
                   error case is now combined with check for valid RC for DTX
                   detection.
08/13/01   ak      Do not call dec_read_sch_data when RC is invalid.
08/10/01   lh      Added MuxPDU5 support for F-SCH.
08/07/01   hrk     Added back parameter "code_type" to dec_dtx_thresh_init().
08/07/01   hrk     Parameter "code_type" removed from call to dec_dtx_thresh_init().
                   Added DTX dectection for RC5 Turbo codes.
08/02/01   lcc     1. Moved some functions from rxccommon.c which really belongs
                      here.
                   2. Modified the way F-SCH shutdown is done such that even 1-frame
                      burst will work.  Previously, 1-frame burst will result in
                      2 frames with 1 "phantom" burst.
07/26/01   ak      Moved LTU_CRC feature so LTU_ID counter inc'd correctly.
06/28/01   baon    Incorporated the features FEATURE_IS2000_REL_A/32X
06/25/01   lcc     Corrected problem with frame type logging on F-DCCH.
06/15/01   day     Mainlined FEATURE_FFPC, FEATURE_IS2000
                   Delete FEATURE_SPECIAL_MDR, FEATURE_MDR_FER_F3MSG
06/06/01   lcc     1. Allowed DTX frames to be considered as good frames when
                      waiting for N5m good frames in TCI.
                   2. Added DCCH erasure frame categories in fade_check.
06/04/01   bgc     Added #ifdefs to turn off data services.
06/04/01   vlc     Changed FEATURE_GPSONE to FEATURE_GPS_MODE.
06/01/01   hrk/snn added support for DTX detection in RC5
                   Backed off some changes on turbo.
06/01/01   jq      Wrapped some code in FEATURE_IS95B_MAHHO.
05/23/01   lad     Added work-around for 8x and 16x turbo decoder's status bit
                   not being set.
05/14/01   jal     Corrected RLP NAK statistics logging
05/11/01   jq      Modified SCH frame type logging to include the last frame.
05/03/01   hrk     moved rxc_fch_frame_status outside FEATURE_FFPC.
05/03/01   sr      Made changes in rxc_plt_fch_logging() for dcch
05/03/01   lcc     Changes to use new definition of rxc_returned_type.
04/24/01   jq      Fixed an error in frame logging
04/23/01   sr      Changes to PLT to support new interface
04/20/01   sr      Merged in from MSM5100 archive
04/19/01   tc      Fixed incorrect dsch checking in rxc_log_tc_msg()
04/19/01   tc      Merged with MSM_MUX1X.01.00.01
           fc      Renamed mcc_report_event_msg_rxed to
                   mclog_report_event_msg_rxed, mcc_report_event_timer to
                   mclog_report_event_timer and mcc_report_event_counter to
                   mclog_report_event_counter
04/18/01   sr      Merged with MSM_MUX1X.01.00.00
           bgc,lcc Moved rxc_scr_trans_cmd() to this file.
           hrk     Added FEATURE_IS2000_P2 and DCCH and DTX handling.
           ych     Added support for FEATURE_JCDMA_MONITOR
           lcc     Corrected problem with back-to-back rate-changing burst.
           hrk     Collecting Reverse Power Control logging info in the context of
                   F-FCH decoder interrupt.
           jal     Corrected a couple of mis-reinitialized fields in
                   rxc_clear_rlp_statistics()
03/27/01   vlc     Merged MSM_MUX1X.00.00.17.
           lcc     1. Merged in support for SO 17 (CAI_SO_VOICE_13K_IS733).
                   2. Moved rxc_tc_prse outside IRAM to free up some IRAM.
           lcc     1. Cleaned up the way SCH shutdown and "start" is scheduled to take
                   care of the "one frame" gap and back-to-back scenarios.
                   2. Changed rxc_stored_sch_packet to use type mctrans_rxc_f_sch_action_type.
           fc      Fixed timer ID for T5m.
           fc      Added support of events report: Message received, Timer
                   expired and Counter threshold.
           lcc     1. Moved code for shutting down SCH into
                   rxc_shutdown_sch_if_action_time.
                   2. Corrected SCH shutdown problem when new burst starts at
                   1 frame after current stop time.
                   3. Removed some unnecessary debug messages.
03/23/01   tc      Fixed FEATURE_IS2000_REL_A.
03/20/01   ych     Merged JCDMA Features.
03/15/01   ejv     Added FEATURE_IS2000_REL_A.
03/15/01   tc      Fixed GPS at rxc_tc.
03/08/01   lh      Bug fix for compile error.
03/07/01   va/lh   Added support for LAC changes in Rel A.
03/02/01   tc      Merged MSM_MUX1X.00.00.15.
02/20/01   sr      Added support for 32X
                   Merged in from common archive:
                   02/13/01   jal     Support to reset RLP statistics
02/16/01   bgc     Changed FEATURE_FACTORY_TESTMODE to enable a runtime switch.
02/15/01   tc      Fixed Forward Supplemental Channel RC4 1X.
02/13/01   jal     Support to reset RLP statistics
02/05/01   bgc     Renamed RXC_RIFHHO_CFS_GPS_F.
01/16/01   jq      Added FEATURE_IS2000_SCH ifdefs around SCH codes.
01/09/00   ak      Put LTU CRC checking under an ifdef, since CPU intensive.
12/11/00   hrk     Fixed - F-FCH CURR_SETPT_DB seemed to be starting from
                   INIT_SETPT + 0.5 dB instead of INIT_SETPT, FFPC logging showed
                   that F-SCH setpoint started at the default value instead of the
                   SCH_INIT_SETPT value specified in ESCAM.
02/01/01   tc      Updated RXC_RIF_HHO_OR_CFS_F to RXC_RIFHHO_CFS_GPS_F.
01/30/01   tc      Merged GPS support.
01/12/01   tc      Merged MSM_MUX1X.00.00.06.
12/07/00   bgc     Commented out some unused PLT code.
11/29/00   ak      Implemented LTU CRC checking.
11/17/00   bgc     Fixed some warnings with typecasts.
11/13/00   bgc     Moved PLT traffic tables to here from common.  Added
                   accessor functions for FTM loopback flags.
11/13/00   hrk     Moved updating of FFPC setpoints from task context to ISR.
11/10/00   jc      This file was part of rxc.c. See rxccommon.c for full
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
#include "ffpc_i.h"
#include "mux.h"
#include "mux_logtypes.h"
#include "muxmdsp_i.h"
#include "muxmsgr.h"
#include "muxmsm.h"
#include "muxvoc.h"
#include "rxc.h"
#include "rxc_i.h"
#include "rxccommon.h"
#include "rxccommon_i.h"
#include "rxcdemux.h"
#include "rxcidle.h"
#include "rxcmc_i.h"
#include "rxcpaging.h"
#include "rxcrda.h"
#include "rxcsync.h"
#include "rxctraffic.h"
#include "txc_v.h"
#include "txc_i.h"
#include "txccommon_i.h"
#include "txctraffic_i.h"

//drivers
#include "dalint_1x.h"
#include "dec5000.h"
#include "dec5000_v.h"
#include "deint_i.h"
#include "m1x_hwio_mpss.h"
#include "msm_drv.h"
#include "msm_drv_i.h"
#include "enc_v.h"
#include "enc_i.h"
#include "dmod_i.h"
#include "m1x_stmr_i.h"

//cp
#include "parm_i.h"
#include "mccrx.h"
#include "cai.h"
#include "cai_v.h"
#include "rx.h"
#include "rx_v.h"
#include "mclog.h"
#include "mclog_v.h"
#include "mctrans_i.h"
#include "rxtx_v.h"
#include "mccdma_v.h"
#include "mccdma_i.h"
#include "loopback.h"
#include "mar_i.h"
#ifdef FEATURE_1XCP_QSH_SUPPORT
#include "mccqsh_i.h"
#endif /* FEATURE_1XCP_QSH_SUPPORT */
#include "mccsch_i.h"

//srch
#include "srch_mux.h"
#include "srchi_i.h"
#include "srch_v.h"
#include "srchtc_ta.h"

//diag
#include "m1x_time_i.h"
#include "onex_non_sig.h"

//other
#include "amssassert.h"
#include "qw.h"
#include "rex.h"
#include "bit.h"
#include "queue.h"
#include "err.h"
#include "log.h"
#include "crc.h"
#include "cmd.h"
#include "msg.h"
#include "modem_mem.h"
#include "dsm.h"
#include "dsrlp_api.h"
#include "mdrrlp_api.h"
#include "log_dmss.h"

//feature dependent

#ifdef FEATURE_MODEM_1X_SUPPORTS_RF
#include "rfm_1x.h"
#endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

#include "diagcmd.h"
#include "diagpkt.h"
#include "diagbuf.h"
#include "dsrlp_stats.h"
#include "stringl.h" /* for memscpy */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
void rxc_prep_for_rifhho_cfs_gps
(
  boolean is_gps
);

#ifdef FEATURE_IS2000_1X_ADV
void rxc_tc_isr_select( void );
void rxc_rcp_cmd( mctrans_action_packet_type *cmd_ptr );

boolean rxc_frame_is_valid
(
  uint32                          frame_num,
  cai_radio_config_type           curr_rc,
  cai_fch_blanking_dutycycle_type duty_cycle
);

typedef struct
{
  mux_1x_adv_info_type rxc_1xA_log;
  mux_1x_adv_fet_stats_type rxc_1xA_fet_stats_log;
  uint16 rxc_1xA_log_mdsp_entry_cnt;
  boolean rxc_1xA_logging_enabled;
} rxc_1xA_log_vars;

static rxc_1xA_log_vars rxc1xA_log_data =
{
  .rxc_1xA_log_mdsp_entry_cnt = 0,
  .rxc_1xA_logging_enabled    = FALSE
};
#endif /* FEATURE_IS2000_1X_ADV */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
/* Conversion macro for time in "ms" to "sclks" */
#define GET_TIMETICK_MS_FROM_SCLK(sclk)      ((sclk)*1000/TIMETICK_NOMINAL_FREQ_HZ)

#ifdef FEATURE_MODEM_1X_DRX
/* Is service connect received to enable DRX from MUX side */
boolean rxc_serv_conn_rxed = FALSE;
/* Is DRX is already enabled from MUX side after service connect is received */
boolean rxc_mux_drx_enabled = FALSE;
/* Is DRX needs to be disabled due to RC change */
boolean rxc_disable_drx_rc_change = FALSE;
/* Is RC changed to enable/disable DRX */
boolean rxc_mux_rc_changed = FALSE;

#endif /* FEATURE_MODEM_1X_DRX */

#ifdef FEATURE_IS2000_1X_ADV
#define FACKCH_TEST_MODE_DISABLED   0x00
#define FACKCH_TEST_MODE_ACK        0x01
#define FACKCH_TEST_MODE_NAK        0x02
#define FACKCH_TEST_MODE_ENABLED    (FACKCH_TEST_MODE_ACK | FACKCH_TEST_MODE_NAK)
#endif /* FEATURE_IS2000_1X_ADV */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Power control sysmbol gating mask */
#define DEM_FULL_RATE_GATING_MASK     0x0
#define DEM_HALF_RATE_GATING_MASK     0x6
#define DEM_QUARTER_RATE_GATING_MASK  0xd

/* For loopback calls, define RXC ATMR ISR PCG offset value as 0x3
 * in order to push out the Rx ISR with such that it’s always occur after
 * Tx ISR.
*/
#define RXC_ATMR_LB_PCG_OFFSET        0x3   /* 3.75ms value */

#define RXC_TC_TMO_FRAMES (cai_tmo.t5m / 20)
  /* number of 20 ms frames in the timeout for the Traffic Channel */

/* Indicator of whether at the traffic entry MVS is already enabled or not */
boolean rxc_traffic_entry_mvs_enabled;

#define RXC_NO_RATE 65535

/* Status of MVS_STANDBY operation received from MC task */
mc_mvs_standby_enum_type mvs_standby_status;

/* Indication to MC informing MVS_STANDBY in progress or not */
boolean mvs_cdma_standby_in_progress = FALSE;

/* To log FCH rate in 0x14ED */
static cai_data_rate_type rxc_ffch_rate = CAI_BLANK_RATE;

/* F-SCH stats for 0x119D */
static rxc_sch_demod_log_type rxc_fsch_stats;

rxc_ta_type rxc_ta_info;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*         TASK VARIABLES                                                  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*-------------------------------------------------------------------------*/
/* Buffers and Queues For Logging                                          */
/*-------------------------------------------------------------------------*/
/* Current Frame # with offset */
static uint16 frame_number;

rxc_bufs_qs_vars rxc_bufs_qs_data =
{
  .rxc_fpc_history = 0,     /*rxc_fpc_history          */
  .rxc_rpc_history = 0,     /*rxc_rpc_history          */
  .rxc_log_frm_types_sr_ptr = NULL,  /**rxc_log_frm_types_sr_ptr*/
};
  #define RXC_FRM_TYPES_LOG_OFFSET \
  (rxc_bufs_qs_data.rxc_log_frm_types_buf.hdr.len - FPOS(mux_log_fwd_frame_types_type, var_len_buf))

  /* Macro for sending an accumulated MAR log */
  //lint -emacro(717,RXC_SEND_MAR_LOG) do...while(0)
  #define RXC_SEND_MAR_LOG()                                        \
   do {                                                             \
    (void) log_submit(&rxc_bufs_qs_data.rxc_log_mar_buf);                            \
    /* Re-initialize counters */                                    \
    rxc_bufs_qs_data.rxc_log_mar_buf.hdr.len = (word) FPOS(mux_log_mar_type, entry); \
    rxc_bufs_qs_data.rxc_log_mar_buf.count = 0;                                      \
   } while (0)

/*-------------------------------------------------------------------------*/
/* Variables for Fade timers                                               */
/*-------------------------------------------------------------------------*/

rxc_fade_timers_vars rxc_fade_data =
{
  .rxc_good_frames            = 0,
  .rxc_good_guaranteed_frames = 0,
  .rxc_t5m_bad_frames         = 0,
  .rxc_n2m_bad_frames         = 0,
  .rxc_ho_cnt                 = 0,
  .rxc_fade                   = FALSE,
  .rxc_sent_bad_rpt           = FALSE,
  .rxc_sent_pwron_rpt         = FALSE,
  .rxc_sent_valid_rpt         = FALSE,
  .rxc_pre_tt_ho              = FALSE
};

/*-------------------------------------------------------------------------*/
/* DEBUG VARIABLES                                                         */
/*-------------------------------------------------------------------------*/

cai_data_rate_type rxc_frame_rate = CAI_BLANK_RATE; /* Current vocoder frame rate */

/*-------------------------------------------------------------------------*/
/* Variables for Hard Handoff with Return-on-Failure                       */
/*-------------------------------------------------------------------------*/
rxc_traffic_frames_status rxc_frame_data =
{
  .rxc_fch_rate = RXC_NO_RATE,
  .rxc_sch_frame_status = RXC_NO_FRAME,
};


   boolean rxc_rifhho_cfs_gps_in_progress = FALSE;
byte rxc_cfs_tuneback_count;
#ifndef FEATURE_IS2000_REL_A
rxc_hho_vars_type rxc_hho_data =
{
  .rxc_lost_frames              = 0,
  .rxc_rif_hho_timeout_frames   = 0,
  .rxc_delay_after_hho_failure  = FALSE,
  .rxc_hard_handoff_in_progress = FALSE,
  .rxc_eib_flags                = {FALSE,FALSE},
  .rxc_use_stored_eibs          = FALSE,
  .rxc_gps_state                = RXC_GPS_STATE_NONE,
  .rxc_gps_tuneback_frames      = 0,
};
#else
rxc_hho_vars_type rxc_hho_data =
{
  .rxc_lost_frames              = 0,
  .rxc_rif_hho_timeout_frames   = 0,
  .rxc_delay_after_hho_failure  = FALSE,
  .rxc_hard_handoff_in_progress = FALSE,
  .rxc_eib_flags                = {FALSE, FALSE},
  .rxc_sch_eib_flags            = {FALSE, FALSE},
  .rxc_qib_flags                = {FALSE, FALSE},
  .rxc_use_stored_qibs          = FALSE,
  .rxc_use_stored_sch_eibs      = FALSE,
  .rxc_use_stored_eibs          = FALSE,
  .rxc_gps_state                = RXC_GPS_STATE_NONE,
  .rxc_gps_tuneback_frames      = 0
};
#endif
/*-------------------------------------------------------------------------*/
/* RXC SCH                                                                 */
/*-------------------------------------------------------------------------*/

// It takes 2 decoder ISRs to reach the time to shutdown SCH:
// first interrupt happens right after the ATMR ISR that sets
// up this counter.
static enum
{
  RXC_SCH_SHUTDOWN_INACTIVE,
  RXC_SCH_SHUTDOWN_ARMED,
  RXC_SCH_SHUTDOWN_NOW
} rxc_sch_shutdown_state = RXC_SCH_SHUTDOWN_INACTIVE;

typedef struct
{
  boolean rxc_sch_rep;         // Contain report indicator from MC
  byte rxc_sch_id;
  boolean rxc_sch_update_now;
  byte rxc_sch_update_counter;
} rxc_sch_report_static_vars;

static rxc_sch_report_static_vars rxc_sch_report_data =
{
  .rxc_sch_rep            = FALSE,
  .rxc_sch_id             = 0,
  .rxc_sch_update_now     = FALSE,
  .rxc_sch_update_counter = 0
};

/*-------------------------------------------------------------------------*/
/*  Supplemental Channel variable                                          */
dec_sch_cfg_type rxc_sch;
dec_sch_type sch_buf;

rxc_log_sch_vars rxc_log_sch_data=
{
  .rxc_log_ffpc_sch_data = FALSE,
  .rxc_dsm_arr_cnt       = RXC_DSM_ARR_NOT_ALLOC,
  .rxc_dsm_arr           = {NULL},
  .rxc_stored_sch_packet = {0}
};

#define RXC_INVALID 0

uint8 const rxc_spr_rate_tab[NUM_SPR_RATE_RC][NUM_SPR_RATES] =
{
  { /* RC3 */
    MUXMDSP_WL_MASK_64,/* 1x  */
    MUXMDSP_WL_MASK_32,/* 2x  */
    MUXMDSP_WL_MASK_16,/* 4x  */
    MUXMDSP_WL_MASK_8, /* 8x  */
    MUXMDSP_WL_MASK_4, /* 16x */
    RXC_INVALID        /* 32x */
  },
  { /* RC4 */
    MUXMDSP_WL_MASK_128,/* 1x  */
    MUXMDSP_WL_MASK_64, /* 2x  */
    MUXMDSP_WL_MASK_32, /* 4x  */
    MUXMDSP_WL_MASK_16, /* 8x  */
    MUXMDSP_WL_MASK_8,  /* 16x */
    MUXMDSP_WL_MASK_4   /* 32x */
  },
  { /* RC5 */
    MUXMDSP_WL_MASK_64,/* 1x  */
    MUXMDSP_WL_MASK_32,/* 2x  */
    MUXMDSP_WL_MASK_16,/* 4x  */
    MUXMDSP_WL_MASK_8, /* 8x  */
    MUXMDSP_WL_MASK_4, /* 16x */
    RXC_INVALID        /* 32x */
  },
  { /* RC11 */
    MUXMDSP_WL_MASK_128,/* 1x  */
    MUXMDSP_WL_MASK_64, /* 2x  */
    MUXMDSP_WL_MASK_32, /* 4x  */
    MUXMDSP_WL_MASK_16, /* 8x  */
    MUXMDSP_WL_MASK_8,  /* 16x */
    MUXMDSP_WL_MASK_4   /* 32x */
  }
};

/* Lookup table for rounding factor */
uint8 const rxc_rnd_factor_tab[NUM_RND_FACT_RC][NUM_RND_FACT] =
{
  { /* RC3 */
    3,          /* 1x  */
    2,          /* 2x  */
    2,          /* 4x  */
    1,          /* 8x  */
    1,          /* 16x */
    RXC_INVALID /* 32x */
  },
  { /* RC4 */
    3,          /* 1x  */
    3,          /* 2x  */
    2,          /* 4x  */
    2,          /* 8x  */
    1,          /* 16x */
    1           /* 32x */
  },
  { /* RC5 */
    3,          /* 1x  */
    2,          /* 2x  */
    2,          /* 4x  */
    1,          /* 8x  */
    1,          /* 16x */
    RXC_INVALID /* 32x */
  },
  { /* RC11 */
    3,          /* 1x  */
    3,          /* 2x  */
    2,          /* 4x  */
    2,          /* 8x  */
    1,          /* 16x */
    1           /* 32x */
  }
};

typedef struct
{
  uint32  good_sdus;
  uint32  incorrect_ltus;

  uint32  sdu_erasure;
  uint32  recovered_ltus;
} ltu_stats_type;

ltu_stats_type ltu_stats;

#ifdef FEATURE_IS2000_REL_A_SVD
/*-------------------------------------------------------------------------*/
/*  Simultaneous Voice and Data variable                                   */
/*-------------------------------------------------------------------------*/
/* Used in simultaneous F-FCH and F-DCCH channel set up.  Indicates which
** channel handles FFPC.
*/
cai_fpc_pri_chan_type ffpc_channel;
static word rxc_dcch_rate = RXC_NO_RATE;  /* Store the DCCH rate in rxc_tc_isr */
#endif /* FEATURE_IS2000_REL_A_SVD */

/*-------------------------------------------------------------------------*/
/*  FFPC external variables                                                */
/*-------------------------------------------------------------------------*/
extern boolean txc_eib_flag;
#ifdef FEATURE_IS2000_REL_A
extern boolean txc_qib_flag;
extern boolean txc_sch_eib_flag;
#endif /* FEATURE_IS2000_REL_A */

static uint8 one_frame_offset_index = 0;
static uint8 two_frame_offset_index = 0;

#ifndef FEATURE_IS2000_1X_ADV
rxc_traffic_status_vars rxc_traffic_status =
{
  .rxc_tx_in_traffic_on = FALSE,     /*rxc_tx_in_traffic_on*/
  .saved_dec_int_task_id = 0          /*saved_dec_int_task_id*/
};
#else
#define   RX_TX_ONE_FRAME_OFFSET_CORRECTION   2
#define   RX_TX_TWO_FRAME_OFFSET_CORRECTION   3
#define   MDSP_BUFF_SIZE                    192

rxc_traffic_status_vars_type rxc_traffic_status =
{
  .rxc_tx_in_traffic_on = FALSE,     /*rxc_tx_in_traffic_on */
  .saved_dec_int_task_id = 0,          /*saved_dec_int_task_id*/
  .rxc_rc11_n2m_val = {2,4,6,8}  /*rxc_rc11_n2m_val     */
};

/* Local structure to correct Rx Tx frame information in
   0x14EC and 0x14ED log packets */
typedef struct
{
  uint8          sb_decision[RX_TX_ONE_FRAME_OFFSET_CORRECTION];
  uint8          critical_frame[RX_TX_TWO_FRAME_OFFSET_CORRECTION];
  uint8          dtx_decision[RX_TX_ONE_FRAME_OFFSET_CORRECTION];
  uint8          sch_state[RX_TX_TWO_FRAME_OFFSET_CORRECTION];
  uint16         tx_frame_rate[RX_TX_ONE_FRAME_OFFSET_CORRECTION];
  uint32         *mdsp_buf[RX_TX_ONE_FRAME_OFFSET_CORRECTION];
} rxc_onex_adv_log_offset_type;

static rxc_onex_adv_log_offset_type log_buff;

static boolean memory_is_allocated_for_logging = FALSE;
#endif /* FEATURE_IS2000_1X_ADV */

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/*-------------------------------------------------------------------------*/
/* Functions used locally                                                  */
/*-------------------------------------------------------------------------*/
void rxc_update_sch_info( mctrans_rxc_f_sch_action_type *fwd_sch_packet );
void rxc_config_sch_hw( mctrans_rxc_f_sch_action_type *fwd_sch_packet );
void rxc_shutdown_sch( void );
void rxc_free_tc_resources (void);
void rxc_process_at_cmd (rxc_cmd_msg_type *cmd_ptr);
void rxc_ho_trans_cmd (mctrans_action_packet_type *);
void rxc_scr_trans_cmd (mctrans_action_packet_type *cmd_ptr);
void rxc_register_action_handlers_notifier_interrupt_func (void);
void (* rxc_action_time_notifier_func_ptr)(mctrans_server_token_type, qword);
void rxc_atmr_dispatcher(void);
void rxc_atmr_isr(m1x_stmr_event_type);
#ifdef FEATURE_IS2000_REL_A
void rxc_atmr_isr2(m1x_stmr_event_type);
#endif /* FEATURE_IS2000_REL_A */
void rxc_control_hold_trans_cmd( mctrans_action_packet_type *chs_packet );

/*========================================================================

FUNCTION RXC_IS_DATA_CALL

DESCRIPTION This function checks the primary and secondary service options
            for the current call and returns a boolean indicating whether
            or not the SO is Data related.

DEPENDENCIES None

RETURN VALUE True for Data SOs, False otherwise.

SIDE EFFECTS None

=========================================================================*/
boolean rxc_is_data_call( void )
{
  boolean is_data = FALSE;

  if(CAI_IS_SO_DATA(rxc_so.pri))
  {
    is_data = TRUE;
  }
  else if ( (rxc_so.pri == CAI_SO_TDSO) ||
            (rxc_so.pri == CAI_SO_FULL_TDSO) )
  {
    is_data = TRUE;
  }

  #ifdef FEATURE_IS2000_REL_A_SVD
  if(CAI_IS_SO_DATA(rxc_so.sec))
  {
    is_data = TRUE;
  }
  else if ( (rxc_so.sec == CAI_SO_TDSO) ||
            (rxc_so.sec == CAI_SO_FULL_TDSO) )
  {
    is_data = TRUE;
  }
  #endif /* FEATURE_IS2000_REL_A_SVD */

  return (is_data);
}


/*========================================================================

FUNCTION RXC_IS_LOOPBACK_SO

DESCRIPTION This function checks the provided service option
            for the current call and returns a boolean indicating whether
            or not the SO is Loopback related.

DEPENDENCIES None

RETURN VALUE True for Loopback SOs, False otherwise.

SIDE EFFECTS None

=========================================================================*/
static boolean rxc_is_loopback_so( uint16 srv_opt )
{
  boolean is_loopback = FALSE;

  if( (srv_opt == CAI_SO_LOOPBACK)         ||
      (srv_opt == CAI_SO_LOOPBACK_13K)     ||
      (srv_opt == CAI_SO_LOOPBACK_SO55)    ||
      (srv_opt == CAI_SO_LOOPBACK_SO75)
    )
  {
    is_loopback = TRUE;
  }

  return (is_loopback);
}

/*========================================================================

FUNCTION RXC_INIT_SCH_MUX

DESCRIPTION This function initializes the array that will be used to hold
            the SCH Mux frames for RLP

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_init_sch_mux( void )
{
  byte i;                                                    /* loop ctr */
  boolean rlp_buffer_error = FALSE;
  word    rlp_partial_buf_cnt = 0;

  /* setup SCH MuxPDU array to hold new items */
  for( i=0; i< SCH_MAX_FWD_PDU; i++)
  {
    if (rxc_log_sch_data.rxc_dsm_arr[i] == NULL)
    {
      rxc_log_sch_data.rxc_dsm_arr[i] = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL);

      /* If less than 8 buffers are available, treat as error case.
      */
      if (rxc_log_sch_data.rxc_dsm_arr[i] == NULL)
      {
        rlp_buffer_error = TRUE;
        break;
      }
    }
  }
  /* Error case, return the rest of the buffers.
  */
  if (rlp_buffer_error == TRUE)
  {
    for( i=0; i< SCH_MAX_FWD_PDU; i++)
    {
      if (rxc_log_sch_data.rxc_dsm_arr[i] != NULL)
      {
        dsm_free_packet( &rxc_log_sch_data.rxc_dsm_arr[i]);
        rxc_log_sch_data.rxc_dsm_arr[i] = NULL;
        rlp_partial_buf_cnt++;
      }
    }
    M1X_MSG( MUX, LEGACY_ERROR,
      "Not enough DSM items %d",
      rlp_partial_buf_cnt);

    /* Set array count to RXC_DSM_ARR_NOT_ALLOC to indicate buffers have
    ** not been assigned.
    */
    rxc_log_sch_data.rxc_dsm_arr_cnt = RXC_DSM_ARR_NOT_ALLOC;
  }
  else
  {
    rxc_log_sch_data.rxc_dsm_arr_cnt = 0;
  }
} /* rxc_init_sch_mux() */

/*===========================================================================

FUNCTION RXC_SET_FRAME_OFFSET

DESCRIPTION
  This function sets the rxc_frame_offset which is used for frame number
  timing correction computations.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Sets rxc_frame_offset. Also sets frame offset in ts.

===========================================================================*/
void rxc_set_frame_offset
(
  byte frame_offset
)
{
  rxc_common_data.rxc_frame_offset_countdown = 6;
  rxc_common_data.rxc_frame_offset = frame_offset;
  muxmdsp_update_frame_offset( frame_offset );

  /* Update m1x_time and m1x_stmr modules about the offset */
  m1x_stmr_set_frame_offset( frame_offset );
  m1x_time_set_frame_offset( frame_offset );

  rxc_ta_info.frame_offset = frame_offset;

  M1X_MSG( MUX, LEGACY_HIGH,
    "Set Forward Frame Offset to %d",
    frame_offset );

} /* rxc_set_frame_offset */

/*===========================================================================

FUNCTION SEND_FOR_SCH_FER_RPT

DESCRIPTION
  This function sends a fwd. SCH FER report to the Main Control task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void send_for_sch_fer_rpt( byte sch_id )
{
  mccrx_rpt_type *rpt_ptr;
    /* Pointer to report to be sent to the Main Control task */

  boolean rxc_ta_is_enabled = rxc_is_ta_enabled();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - -*/

  if( rxc_ta_is_enabled )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
       "1x2GTA: Suspend SCH FER rpt" );
    return;
  }

  if( ( rpt_ptr = (mccrx_rpt_type *)q_get( &mcc_rx_free_q ) ) == NULL )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "No buffers on mcc_rx_free_q" );

    /* Reset counts */
    rxc_pwr.sch_tot_frames[ sch_id ]         = 0;
    rxc_pwr.skipped_sch_tot_frames[ sch_id ] = 0;
    rxc_pwr.sch_bad_frames[ sch_id ]         = 0;
  }
  else
  {
    /* Generate power measurement report for MC */
    rpt_ptr->pwr.hdr.rpt = RXC_PWR_R;
    // set a flag to indicate that other stuff is not included
    rpt_ptr->pwr.sch_pwr_meas_included = TRUE;
    rpt_ptr->pwr.sch_id = sch_id;
    rpt_ptr->pwr.sch_pwr_meas_frames = rxc_pwr.sch_tot_frames[ sch_id ];
    rpt_ptr->pwr.sch_errors_detected = rxc_pwr.sch_bad_frames[ sch_id ];

#ifdef FEATURE_IS2000_REL_A_SVD
    /* It is mandatory to report FCH and/or DCCH at this time also.
    ** Report power measurements of the fundicated channel(s) as follows:
    **
    ** FCH & DCCH:  Report measurements for FCH and DCCH separately.  Set dcch
    **              measurement included flag to indicate reporting of both
    **              channels.
    ** FCH Only:    Report power measurement for FCH only.
    **              Indicate 1 channel reporting.
    ** DCCH Only:   Report power measurement for DCCH only.
    **              Indicate 1 channel reporting.
    */
    rpt_ptr->pwr.pwr_meas_frames      = 0;
    rpt_ptr->pwr.errors_detected      = 0;
    rpt_ptr->pwr.dcch_pwr_meas_incl   = FALSE;
    rpt_ptr->pwr.dcch_pwr_meas_frames = 0;
    rpt_ptr->pwr.dcch_errors_detected = 0;
    if ( (rxc_so.for_fch.included) && (rxc_so.for_dcch.included) )
    {
      rpt_ptr->pwr.pwr_meas_frames        = rxc_pwr.num_frames;
      rxc_pwr.num_frames_current          = rxc_pwr.num_frames;
      rpt_ptr->pwr.errors_detected        = (byte)rxc_pwr.num_bad_frames;
      rxc_pwr.num_bad_frames_current      = rxc_pwr.num_bad_frames;
      rpt_ptr->pwr.dcch_pwr_meas_incl     = TRUE;
      rpt_ptr->pwr.dcch_pwr_meas_frames   = rxc_pwr.dcch_num_frames;
      rxc_pwr.dcch_num_frames_current     = rxc_pwr.dcch_num_frames;
      rpt_ptr->pwr.dcch_errors_detected = (byte)rxc_pwr.dcch_num_bad_frames;
      rxc_pwr.dcch_num_bad_frames_current = rxc_pwr.dcch_num_bad_frames;
      M1X_MSG( MUX, LEGACY_MED,
        "After PMRM Sent, Frame Count: FCH %d, FCH Bad Frame %d, "
        "DCCH %d, DCCH Bad Frame %d",
        rxc_pwr.num_frames,
        rxc_pwr.num_bad_frames,
        rxc_pwr.dcch_num_frames,
        rxc_pwr.dcch_num_bad_frames);
    }
    else if (rxc_so.for_fch.included)
    {
      rpt_ptr->pwr.pwr_meas_frames      = rxc_pwr.num_frames;
      rxc_pwr.num_frames_current        = rxc_pwr.num_frames;
      rpt_ptr->pwr.errors_detected      = (byte)rxc_pwr.num_bad_frames;
      rxc_pwr.num_bad_frames_current    = rxc_pwr.num_bad_frames;
      M1X_MSG( MUX, LEGACY_MED,
        "After PMRM Sent, Frame Count: FCH %d, FCH Bad Frame %d",
        rxc_pwr.num_frames,
        rxc_pwr.num_bad_frames);
    }
    /* DCCH only scenario.  For this scenario, standard indicates
    ** that power measurement report represents DCCH errors and
    ** dcch_pwr_meas_incl is set to FALSE.
    */
    else if (rxc_so.for_dcch.included)
    {
      rpt_ptr->pwr.pwr_meas_frames      = rxc_pwr.dcch_num_frames;
      rxc_pwr.dcch_num_frames_current   = rxc_pwr.dcch_num_frames;
      rpt_ptr->pwr.errors_detected      = (byte)rxc_pwr.dcch_num_bad_frames;
      rxc_pwr.dcch_num_frames_current   = rxc_pwr.dcch_num_frames;
      M1X_MSG( MUX, LEGACY_MED,
        "After PMRM Sent, Frame Count: DCCH %d, DCCH Bad Frame %d",
        rxc_pwr.dcch_num_frames,
        rxc_pwr.dcch_num_bad_frames);
    }
    #else /* FEATURE_IS2000_REL_A_SVD */
      // Include FCH data as it is mandatory
    rpt_ptr->pwr.pwr_meas_frames = rxc_pwr.num_frames;
    rxc_pwr.num_frames_current   = rxc_pwr.num_frames;
    rpt_ptr->pwr.errors_detected = (byte)rxc_pwr.num_bad_frames;
    rxc_pwr.num_bad_frames_current = rxc_pwr.num_bad_frames;
    M1X_MSG( MUX, LEGACY_MED,
        "After PMRM Sent, Frame Count: FCH %d, FCH Bad Frame %d",
        rxc_pwr.num_frames,
        rxc_pwr.num_bad_frames);
    #endif /*FEATURE_IS2000_REL_A_SVD */

    rxc_mc_rpt( rpt_ptr );
  }

} /* send_for_sch_fer_rpt */

/*===========================================================================

FUNCTION rxc_send_two_valid_frame_rpt

DESCRIPTION
  This function sends the 2 valid frames Rx'ed report to the Main Control task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_send_two_valid_frame_rpt (void)
{
  mccrx_rpt_type *rpt_ptr; /* ptr to report to send to Main Control task */

  /* Event report: Counter N5m threshold reached */
  mclog_report_event_counter(CAI_COUNTER_N5M);

  /* send report of valid frame to Main Control task */
  if ( (rpt_ptr = (mccrx_rpt_type *)q_get(&mcc_rx_free_q) ) != NULL )
  {
    rxc_fade_data.rxc_sent_valid_rpt = TRUE;
    rpt_ptr->hdr.rpt = RXC_VALID_R;
    rxc_mc_rpt(rpt_ptr );
    M1X_MSG( MUX, LEGACY_HIGH,
      "Send 2 frame valid rpt");
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "No buffers on mcc_rx_free_q" );
  }
} /* end rxc_send_two_valid_frame_rpt */

/*===========================================================================

FUNCTION RXC_LOG_MAR

DESCRIPTION
  This function logs the Forward Traffic Channel Markov test data.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  rxc_log_mar
(
  byte mar,   /* expected mux1 * 16 + rate decision */
  byte ber    /* bit error rate */
)
{
  if (rxc_bufs_qs_data.rxc_log_mar_buf.count == 0)
  {
    (void) m1x_time_get(rxc_bufs_qs_data.rxc_log_mar_buf.hdr.ts);
  }

  /* Fill in log data */
  rxc_bufs_qs_data.rxc_log_mar_buf.entry[rxc_bufs_qs_data.rxc_log_mar_buf.count].mar_inx = mar;
  rxc_bufs_qs_data.rxc_log_mar_buf.entry[rxc_bufs_qs_data.rxc_log_mar_buf.count].bit_errs = ber;
  rxc_bufs_qs_data.rxc_log_mar_buf.count++;
  rxc_bufs_qs_data.rxc_log_mar_buf.hdr.len +=
    sizeof(rxc_bufs_qs_data.rxc_log_mar_buf.entry[0]);

  if (rxc_bufs_qs_data.rxc_log_mar_buf.count == MUX_LOG_MAR_CNT)
  {
    RXC_SEND_MAR_LOG();
  }
} /* rxc_log_mar */


#ifdef FEATURE_IS2000_REL_A
/*            Variables for FDCH Release A logging             */
static uint32 raw_fdch_log_buffer[ FRDCH_LOG_BUFFER_SIZE/4   +1] = {0};
static log_fdch_information_type * rxc_fdch_log_buffer =
  (log_fdch_information_type *) raw_fdch_log_buffer; //lint !e740 Unusual pointer cast
static fdch_sub_record_type *rxc_fdch_sub_record_ptr = NULL;
static fwd_sch_information_type *fsch_frame_info = NULL;
static int rxc_fdch_log_buff_offset = 0;
static int rxc_fdch_log_sub_record_offset = 0;
static word rxc_fdch_curr_sch_mux_opt = 0;

/*               FUNCTIONS TO HANDLE THE LOGGING FOR FDCH               */

/*===========================================================================

FUNCTION RXC_FDCH_FRAME_INFO_SEND_LOG

DESCRIPTION
  Terminate and send the accumulated FDCH frame information log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A FDCH frame information log may be sent.

===========================================================================*/
void rxc_fdch_frame_info_send_log(void)
{
  if (rxc_fdch_log_buffer->hdr.len > 0)
  {
    /* Will send log if submitted. */
    (void) log_submit(rxc_fdch_log_buffer);

    /* Flag to init start of log */
    rxc_fdch_log_buffer->hdr.len = 0;
  }

} /* rxc_fdch_frame_info_send_log */

/*===========================================================================

FUNCTION RXC_FDCH_LOG_FRAME_INFO_INIT

DESCRIPTION
  This function initializes the FDCH frame info log subrecord, as well as the
  pointer to it and the entry for this frame, in the frame types log buffer.
  If necessary, the log is sent and a new one is generated.

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to frame log entry.

SIDE EFFECTS
  A FDCH frame info. log may be sent.

===========================================================================*/
fdch_frame_type  * rxc_fdch_log_frame_info_init(void)
{
  fdch_frame_type  *fdch_frame_ptr = NULL;
  channels_info_type  fdch_chan_set_mask;

  /* If logging is enabled. */
  if (log_status(LOG_FDCH_FRAME_INFO_C))
  {
    /* If we reached accumulation threshold, send it.
    the threshold = the buffer size - the maximum of byte per record in the
    worst case analysic */
    if (rxc_fdch_log_buffer->hdr.len >= FRDCH_LOG_BUFFER_THRESHOLD)
    {
      rxc_fdch_frame_info_send_log();
    }
    /* check if there is a need to re-initialize */
    if (rxc_fdch_log_buffer->hdr.len == 0)
    {
      rxc_fdch_log_buffer->hdr.len = FPOS(log_fdch_information_type,reference_buf);//lint !e734 FPOS
      rxc_fdch_log_buffer->hdr.code = LOG_FDCH_FRAME_INFO_C;
      /* First frame, log time stamp. */
      (void) m1x_time_get(rxc_fdch_log_buffer->hdr.ts);

      /*  Fill in beginning of record   */
      rxc_fdch_log_buffer->frame_offset = rxc_common_data.rxc_frame_offset;
      rxc_fdch_log_buffer->num_sub_records = 0;

      /* reset all the control variables */
      rxc_fdch_log_buff_offset = 0;
      rxc_fdch_sub_record_ptr = NULL;
    }
    else
    {
      /* If a new sub-record is needed, i.e. the channel set has changed,
      ** check for the condition here.
      */
      /* clear the fdch mask */
      fdch_chan_set_mask.mask = 0;
      /* set the the mask based on the active FDCH channels */
      if (rxc_so.for_fch.included)
      {
        fdch_chan_set_mask.individual_channels.fch = TRUE;
      }
      if (rxc_so.for_dcch.included)
      {
        fdch_chan_set_mask.individual_channels.dcch = TRUE;
      }
      if (rxc_frame_data.rxc_sch_frame_status!=RXC_NO_FRAME)
      {
        fdch_chan_set_mask.individual_channels.sch0 = TRUE;
      }

      /* I don't know how to do for PICH channel, set to FALSE for now */
      fdch_chan_set_mask.individual_channels.rpich = FALSE;

      /* start a new sub-record if there is a new channel been added or the SCH
      MUX option is changing */
      if(fdch_chan_set_mask.mask != rxc_fdch_sub_record_ptr->assigned_channels.mask)
      {
        /* We no longer have a valid sub-record. */
        rxc_fdch_sub_record_ptr = NULL;
      }
      else
      {
        if ((rxc_frame_data.rxc_sch_frame_status!=RXC_NO_FRAME) &&
            (rxc_fdch_curr_sch_mux_opt != rxc_so.for_sch_mux_option))
        {
          /* We no longer have a valid sub-record. */
          rxc_fdch_sub_record_ptr = NULL;
        }
      }

    }
    /* Need to init a new subrecord? */
    if (rxc_fdch_sub_record_ptr == NULL)
    {
      rxc_fdch_sub_record_ptr =
        (fdch_sub_record_type *) &rxc_fdch_log_buffer->reference_buf[rxc_fdch_log_buff_offset];
      /* Increment sub-record count. */
      rxc_fdch_log_buffer->num_sub_records++;
      /* reset the number of frame counter */
      rxc_fdch_sub_record_ptr->num_frames = 0;

      /* reset the fdch log sub-record offset */
      rxc_fdch_log_sub_record_offset = 0;

      /* reset the channel mask */
      rxc_fdch_sub_record_ptr->assigned_channels.mask = 0;

      /********Initialize the channel assignment and RC for each FDCHs ************/

      /* check for PICH */
      /* I don't know how to do for PICH channel, set to FALSE for now */
      rxc_fdch_sub_record_ptr->assigned_channels.individual_channels.rpich = FALSE;

      /* check for FCH */
      if( rxc_so.for_fch.included )
      {
        rxc_fdch_sub_record_ptr->assigned_channels.individual_channels.fch
                                   = TRUE;
        rxc_fdch_sub_record_ptr->reference_buf[rxc_fdch_log_sub_record_offset++]
                                   = rxc_so.for_fch.rc;
      }
      /* check for DCCH */
      if( rxc_so.for_dcch.included )
      {
        rxc_fdch_sub_record_ptr->assigned_channels.individual_channels.dcch
                                   = TRUE;
        rxc_fdch_sub_record_ptr->reference_buf[rxc_fdch_log_sub_record_offset++]
                                   = rxc_so.for_dcch.rc;
      }

      /* check for SCH and initialize FSCH data*/
      if( rxc_frame_data.rxc_sch_frame_status != RXC_NO_FRAME )
      {
        /* set the channel bit mask */
        rxc_fdch_sub_record_ptr->assigned_channels.individual_channels.sch0
                                   = TRUE;
        fsch_frame_info = (fwd_sch_information_type *)
                 &rxc_fdch_sub_record_ptr->reference_buf[rxc_fdch_log_sub_record_offset];

        /* FSCH RC */
        fsch_frame_info->for_sch_rc = (uint8) rxc_sch.rc;
        /* SCH MUX OPTION */
        if( rxc_so.for_sch_mux_option != 0 )
        {
          /* FSCH0 MUX option LSB */
          fsch_frame_info->for_sch0_mux_option_lo = rxc_so.for_sch_mux_option & 0xFF;
          /* FSCH0 MUX option MSB */
          fsch_frame_info->for_sch0_mux_option_hi = (rxc_so.for_sch_mux_option >> 8) & 0xFF;

          /* store for comparation later */
          rxc_fdch_curr_sch_mux_opt = rxc_so.for_sch_mux_option;
        }
        else
        {
          /* FSCH0 MUX option MSB to zero*/
          fsch_frame_info->for_sch0_mux_option_hi = 0;
          /* FSCH0 MUX option LSB to zero*/
          fsch_frame_info->for_sch0_mux_option_lo = 0;
        }

        fsch_frame_info->sch_frame_length = 0;

        /* FSCH0 number of bits */
        fsch_frame_info->sch0_num_bits = (uint8) rxc_sch.sch_rate;

        /* number of MUX PDU per SCH frame */
        fsch_frame_info->num_mux_pdu_per_sch_frame =
          rxc_log_sch_data.rxc_dsm_arr_cnt;
        /* number of LTU per SCH frame */
        fsch_frame_info->num_ltu_per_sch_frame = rxc_sch.num_ltu;
        /* update the sub-record offset */
        rxc_fdch_log_sub_record_offset += (int) sizeof(fwd_sch_information_type);
      }

      /* Update packet length */
      rxc_fdch_log_buffer->hdr.len +=
                  (word)(FPOS(fdch_sub_record_type,reference_buf) +
                  rxc_fdch_log_sub_record_offset);

      /*Update the buffer pointer offset */
      rxc_fdch_log_buff_offset += (rxc_fdch_log_sub_record_offset +
                                    (FPOS(fdch_sub_record_type,reference_buf)));//lint !e713 !e737 FPOS
    }
    /* Increment frame count */
    rxc_fdch_sub_record_ptr->num_frames++;

    /* Initialize the FDCH frame record pointer */
    fdch_frame_ptr = (fdch_frame_type *)
                          &rxc_fdch_sub_record_ptr->reference_buf[rxc_fdch_log_sub_record_offset];

  }
  else
  {
    rxc_fdch_log_buffer->hdr.len = 0; /* Flag that this record has stopped */
  }
  return fdch_frame_ptr;
}

/*=========================================================================
FUNCTION RXC_FDCH_LOG_FRAME_INFO

DESCRIPTION
  This function populates FDCH frame information logs.

DEPENDENCIE
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
#ifdef FEATURE_IS2000_REL_A_SVD
void rxc_fdch_log_frame_info (byte fch_mux, byte dcch_mux ) /* mux type for FCH/DCCH logging */
#else
void rxc_fdch_log_frame_info (byte mux ) /* mux type for FCH/DCCH logging */
#endif /* FEATURE_IS2000_REL_A_SVD */
{
  int fdch_frame_offset = 0;
  fdch_frame_type  *fdch_frame_record_ptr = NULL;

  /* Call the initialized function to init the fdch frame pointer */
  fdch_frame_record_ptr = rxc_fdch_log_frame_info_init();

  if (fdch_frame_record_ptr != NULL)
  {
    /* If FCH is assigned, then including FCH category */
    if (rxc_fdch_sub_record_ptr->assigned_channels.individual_channels.fch)
    {
#ifdef FEATURE_IS2000_REL_A_SVD
      fdch_frame_record_ptr->reference_buf[fdch_frame_offset++] = fch_mux;
#else
      fdch_frame_record_ptr->reference_buf[fdch_frame_offset++] = mux;
#endif /*  FEATURE_IS2000_REL_A_SVD */
    }

    /* If DCCH is assigned, then including DCCH category */
    if (rxc_fdch_sub_record_ptr->assigned_channels.individual_channels.dcch)
    {
#ifdef FEATURE_IS2000_REL_A_SVD
      /* See structure definition for explaination of reference_buf */
      fdch_frame_record_ptr->reference_buf[fdch_frame_offset++] = dcch_mux; //lint !e661 Possible out-of-bounds
#else
      fdch_frame_record_ptr->reference_buf[fdch_frame_offset++] = mux;
#endif /* FEATURE_IS2000_REL_A_SVD */
    }

    /* If SCH is assigned, then including SCH category
    the possible values for SCH category are: sufficient quality, insufficient
    quality and DTX*/
    if (rxc_fdch_sub_record_ptr->assigned_channels.individual_channels.sch0)
    {

      /* Including the SCH frame category */
      /* See structure definition for explaination of reference_buf */
      fdch_frame_record_ptr->reference_buf[fdch_frame_offset++]
        = rxc_frame_data.rxc_sch_frame_status; //lint !e641 !e661 !e662 !e797 !e796 Conceivable out-of-bounds

      /* Including LTU quality information
      This field is only included if SCH is assigned and this frame is an erasure frame*/
      /* check if the FSCH category is INSUFFICIENT_QUALITY */
      if (rxc_frame_data.rxc_sch_frame_status == RXC_ERASURE_FRAME &&
                                                (rxc_sch.num_ltu != 0))
      {

        /* inluding LTU CRC bits */
        /* See structure definition for explaination of reference_buf */
        fdch_frame_record_ptr->reference_buf[fdch_frame_offset++]
          = (uint8) rxc_frame_data.rxc_sch_status.conv_sch_status.ltu_crc; //lint !e661 !e662 !e796 !e797 Conceivable out-of-bounds
      }
    }

    /* Updated the total lenght of the record header */
    rxc_fdch_log_buffer->hdr.len += (word) fdch_frame_offset;
    /* update the offset variables */
    rxc_fdch_log_sub_record_offset += fdch_frame_offset;
    rxc_fdch_log_buff_offset += fdch_frame_offset;
  }

}
#else /* FEATURE_IS2000_REL_A */
/*===========================================================================

FUNCTION RXC_FRAME_TYPES_SEND_LOG

DESCRIPTION
  Terminate and send the accumulated forward frame types log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A forward frame types log may be sent.

===========================================================================*/
void rxc_frame_types_send_log(void)
{
  if (rxc_bufs_qs_data.rxc_log_frm_types_buf.hdr.len > 0)
  {
    /* Will send log if submitted. */
    log_submit(&rxc_bufs_qs_data.rxc_log_frm_types_buf);

    /* Flag to init start of log */
    rxc_bufs_qs_data.rxc_log_frm_types_buf.hdr.len = 0;
  }

} /* rxc_frame_types_send_log */

/*===========================================================================

FUNCTION RXC_LOG_FRAME_TYPE_FRM_INIT

DESCRIPTION
  This function initializes the frame type log subrecord, as well as the
  pointer to it and the entry for this frame, in the frame types log buffer.
  If necessary, the log is sent and a new one is generated.

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to frame log entry.

SIDE EFFECTS
  A forward frame types log may be sent.

===========================================================================*/
/* indicate when the FSCH RC has been changed */
static boolean rxc_sch_rc_change = FALSE;
mux_log_fwd_frame_types_frm_type * rxc_log_frame_type_frm_init(cai_chind_type ch_ind)
{
  mux_log_fwd_frame_types_frm_type *frm_ptr = NULL;
  mux_log_ded_fwd_chan_set_type chan_set_mask;

  /* If logging is enabled. */
  if (log_status(LOG_FWD_FRAME_TYPES_C))
  {
    /* If we reached accumulation threshold or if there is a RC change,
        then send it, so that the new frame header will be created . */
    if ((rxc_bufs_qs_data.rxc_log_frm_types_buf.hdr.len >=
         MUX_LOG_FWD_FRAME_TYPES_BUF_THRESHOLD) || (rxc_sch_rc_change))
    {
      rxc_frame_types_send_log();
          rxc_sch_rc_change = FALSE;
    }

    if (rxc_bufs_qs_data.rxc_log_frm_types_buf.hdr.len == 0) /* Need to re-init */
    {
      rxc_bufs_qs_data.rxc_log_frm_types_buf.hdr.len =
        FPOS(mux_log_fwd_frame_types_type, var_len_buf);

      rxc_bufs_qs_data.rxc_log_frm_types_buf.hdr.code = LOG_FWD_FRAME_TYPES_C;

      /* First frame, log time stamp. */
      (void) m1x_time_get(rxc_bufs_qs_data.rxc_log_frm_types_buf.hdr.ts);

      /*------------------------------
        Fill in beginning of record
      ------------------------------*/
      /* Frame offset in 1.25 ms units */
      rxc_bufs_qs_data.rxc_log_frm_types_buf.frame_offset =
        rxc_common_data.rxc_frame_offset;

      /* Service Option */
      rxc_bufs_qs_data.rxc_log_frm_types_buf.so = rxc_so.pri;

      /* for now, only FCH is supported */
      rxc_bufs_qs_data.rxc_log_frm_types_buf.fwd_ch_rc.f_fch_rc =
         rxc_so.for_fch.included?rxc_so.for_fch.rc:0;
      rxc_bufs_qs_data.rxc_log_frm_types_buf.fwd_ch_rc.f_dcch_rc =
        rxc_so.for_dcch.included?rxc_so.for_dcch.rc:0;
      rxc_bufs_qs_data.rxc_log_frm_types_buf.fwd_ch_rc.f_sch0_rc =
        rxc_sch.rc;
      rxc_bufs_qs_data.rxc_log_frm_types_buf.fwd_ch_rc.f_sch1_rc = 0; /* not yet supported */

      /* We no longer have a valid sub-record. */
      rxc_bufs_qs_data.rxc_log_frm_types_buf.sr_cnt = 0;
      rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr     = NULL;

    }
    else
    {
      /* If a new subrecord is needed, i.e. the channel set has changed,
      ** check for the condition here.
      */
      chan_set_mask.mask = 0;
      /* for now only one type of channel is supported */
      chan_set_mask.chan_set.f_fch = (ch_ind&CAI_CHIND_FCH_ONLY) != FALSE;
      chan_set_mask.chan_set.f_dcch = (ch_ind&CAI_CHIND_DCCH_ONLY)!= FALSE;

      chan_set_mask.chan_set.f_sch0 =
          (rxc_frame_data.rxc_sch_frame_status!=
                                   RXC_NO_FRAME) ? TRUE : FALSE;

      if(chan_set_mask.mask !=
           rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->ch_set_mask.mask)
      {
        /* We no longer have a valid sub-record. */
        rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr = NULL;
      }
    }

    /* Need to init a new subrecord? */
    if (rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr == NULL)
    {
      /* Init sub-record pointer by finding the offset into the variable length buf.
      ** RXC_FRM_TYPES_LOG_OFFSET is calculated from the length of the log packet.
      */
      rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr = (mux_log_fwd_frame_types_subrecord_type *)
          &rxc_bufs_qs_data.rxc_log_frm_types_buf.var_len_buf.buf_ref[RXC_FRM_TYPES_LOG_OFFSET];

      /* Increment sub-record count. */
      rxc_bufs_qs_data.rxc_log_frm_types_buf.sr_cnt++;

      /* Init all bits in mask, and len_per_frame */
      rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->ch_set_mask.mask = 0;
      rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->len_per_frame    = 0;

      rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->ch_set_mask.chan_set.f_fch=
          (ch_ind&CAI_CHIND_FCH_ONLY)!=FALSE;
      rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->ch_set_mask.chan_set.f_dcch=
          (ch_ind&CAI_CHIND_DCCH_ONLY)!=FALSE;

      /* Increase length per frame value for FCH/DCCH */
      /* if FCH and DCCH are supported at the same time, need to add for DCCH here */
      rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->len_per_frame +=
        sizeof(mux_log_fwd_frame_types_frm_type);

      /* Check if SCH0 is active for this frame */
      if (rxc_frame_data.rxc_sch_frame_status!=RXC_NO_FRAME)
      {
        rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->ch_set_mask.chan_set.f_sch0
          = TRUE;

        /* Increase length per frame value for SCH0 */
        rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->len_per_frame +=
          sizeof(mux_log_fwd_frame_types_frm_type);
      }

      /* Init frame count */
      rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->frame_cnt = 0;

      /* Update packet length */
      rxc_bufs_qs_data.rxc_log_frm_types_buf.hdr.len +=
        FPOS(mux_log_fwd_frame_types_subrecord_type, var_len_buf);
    }

    /* Increment frame count */
    rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->frame_cnt++;

    /* Init frame pointer by finding the offset into the variable length buf.
    ** RXC_FRM_TYPES_LOG_OFFSET is calculated from the length of the log.
    */
    frm_ptr = (mux_log_fwd_frame_types_frm_type *)
        &rxc_bufs_qs_data.rxc_log_frm_types_buf.var_len_buf.buf_ref[RXC_FRM_TYPES_LOG_OFFSET];
  }
  else
  {
    rxc_bufs_qs_data.rxc_log_frm_types_buf.hdr.len = 0; /* Flag that this record has stopped */
  }
  /* if the FSCH RC has been changed, then set the flag so that the next frame,
  the new header frame with new the RC will be updated and sent out */
  if (rxc_bufs_qs_data.rxc_log_frm_types_buf.fwd_ch_rc.f_sch0_rc != rxc_sch.rc)
  {
    rxc_sch_rc_change = TRUE;
  }
  return frm_ptr;

} /* rxc_log_frame_type_frm_init */

/*=========================================================================
FUNCTION RXC_LOG_FRAME_TYPE

DESCRIPTION
  This function populates frame type logs.

DEPENDENCIE
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void rxc_log_frame_type
(
  int16 frame_rate,                       /* FCH  Frame rate */
  mux_log_fch_type_enum_type fch_type,    /* FCH  Frame type */
  mux_log_sch_rate_enum_type sch0_type,   /* SCH0 Frame type */
  cai_chind_type ch_ind,                  /* Indicate Active channels */
  byte mux                                /* mux type for DCCH logging */
)
{
  mux_log_fwd_frame_types_frm_type *frm_ptr;

  frm_ptr = rxc_log_frame_type_frm_init(ch_ind);

  if (frm_ptr != NULL)
  {
    /* Record data for each channel */
    if (rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->ch_set_mask.chan_set.f_fch)
    {
      frm_ptr->f_fch.rate = (byte) frame_rate;
      frm_ptr->f_fch.type = fch_type;

      frm_ptr++; /* Increase sizeof 1 frame entry */
    }

    if(rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->ch_set_mask.chan_set.f_sch0)
    {
      frm_ptr->f_sch0 = sch0_type;

      frm_ptr++; /* Increase sizeof 1 frame entry */
    }

    /* log DCCH */
    if (rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->ch_set_mask.chan_set.f_dcch)
    {
      frm_ptr->f_dcch = mux;
      frm_ptr++;
    }

    /* TODO: Add other channels here! */

    /* end per channel data */

    /* Update packet length */
    rxc_bufs_qs_data.rxc_log_frm_types_buf.hdr.len +=
      rxc_bufs_qs_data.rxc_log_frm_types_sr_ptr->len_per_frame;

  } /* If logging enabled */
} /* rxc_log_frame_type */
#endif /* FEATURE_IS2000_REL_A */

/*========================================================================
FUNCTION RXC_UPDATE_SCH_INFO

DESCRIPTION
   This function updates the struct rxc_sch with the buffered F-SCH
   parameters.

   Rate set is derived using the received rc parameter.  This is done here
   so rate set information can be retained for the processing of the final
   frame, which may occur after the rc is reset in rxc_shutdown_sch ().

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void rxc_update_sch_info (mctrans_rxc_f_sch_action_type *fwd_sch_packet)
{
  RXC_ENTER_ATOMIC_ISR_SECTION();
  rxc_sch.rc = (dec_rc_type) fwd_sch_packet->rc;

  /* Determine rate set using rc.  Do it here so rate set information can be
  ** retained for the processing of the final frame which may occur after
  ** the rc is reset in rxc_shutdown_sch ().
  */
  rxc_sch.rc = ( (cai_radio_config_type) rxc_sch.rc == CAI_INIT_RC ) ?
               CAI_RC_3 : (cai_radio_config_type) rxc_sch.rc;

  rxc_sch.sch_rs   = rxc_get_fwd_link_rate_set(rxc_sch.rc);
  rxc_sch.sch_rate = (dec_sch_rate_type)fwd_sch_packet->sch_rate;
  rxc_sch.mux_pdu_double = fwd_sch_packet->double_size_pdu;
  rxc_sch.code_type = (dec_coding_type)fwd_sch_packet->code_type;

  #ifdef FEATURE_IS2000_REL_A //32X
  rxc_sch.mux_pdu_type =
    (dec_mux_pdu_type_type)fwd_sch_packet->mux_pdu_type;
  rxc_sch.num_ltu = fwd_sch_packet->num_ltu;
  rxc_sch.ltu_len = fwd_sch_packet->ltu_len;
  #endif /* FEATURE_IS2000_REL_A */

  /* For SRCH logging */
  srch_mux_update_rc ( rxc_so.for_fch.included?
                       (deint_rc_type)rxc_so.for_fch.rc:DEINT_RC_INVALID,
                       (deint_rc_type)rxc_sch.rc,
                       DEINT_RC_INVALID,
                       rxc_so.for_dcch.included?
                       (deint_rc_type)rxc_so.for_dcch.rc:DEINT_RC_INVALID );

  /* initialize DTX filter counter to -1 */
  rxcrda_data.rxcrda_sch_dtx_fix_counter = -1;


  RXC_LEAVE_ATOMIC_ISR_SECTION();

  /* Initialize the MuxPDU dsm array, if it hasn't been initialized */
  if( rxc_log_sch_data.rxc_dsm_arr_cnt == RXC_DSM_ARR_NOT_ALLOC)
  {
    rxc_init_sch_mux();
  }

  rxc_pwr.sch_fer_rep[ fwd_sch_packet->sch_id ] = fwd_sch_packet->for_sch_fer_rep;
}

/*========================================================================
FUNCTION RXC_TO_SKIP_SCH_TRANS

DESCRIPTION
   This function checks all SCH parameters that are received against current
   SCH paramers if it is enabled. If both parameters are same it returns TRUE,
   otherwise it returns FALSE


DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
boolean rxc_to_skip_sch_trans( mctrans_rxc_f_sch_action_type *fwd_sch_packet)
{
  cai_radio_config_type  temp_rc = CAI_INIT_RC;
  boolean ret_val = FALSE;

  RXC_ENTER_ATOMIC_ISR_SECTION();

  temp_rc = ( (cai_radio_config_type) fwd_sch_packet->rc == CAI_INIT_RC ) ?
                CAI_RC_3 : (cai_radio_config_type) fwd_sch_packet->rc;

  if ( ( temp_rc == rxc_sch.rc ) &&
       ( rxc_sch.sch_rate == (dec_sch_rate_type)fwd_sch_packet->sch_rate ) &&
       ( rxc_sch.mux_pdu_double == fwd_sch_packet->double_size_pdu ) &&
       ( rxc_sch.code_type == (dec_coding_type)fwd_sch_packet->code_type ) )
  {
    rxcrda_data.rxcrda_sch_dtx_fix_counter = -1;
    rxc_pwr.sch_fer_rep[ fwd_sch_packet->sch_id ] = fwd_sch_packet->for_sch_fer_rep;
    M1X_MSG( MUX, LEGACY_HIGH,
      "Skip SCH ON Trans - it is already enabled");
    ret_val=TRUE;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "Avoid skipping SCH ON trans"
      "Old : RC %d Rate %d PduType%d CodeType %d"
      "New : RC %d Rate %d PduType%d CodeType %d",
      rxc_sch.rc,rxc_sch.sch_rate,rxc_sch.mux_pdu_double,rxc_sch.code_type,
      temp_rc,(dec_sch_rate_type)fwd_sch_packet->sch_rate,
      fwd_sch_packet->double_size_pdu,(dec_coding_type)fwd_sch_packet->code_type );
  }

 RXC_LEAVE_ATOMIC_ISR_SECTION();

 return (ret_val);

} /* rxc_to_skip_sch_trans */

/*===========================================================================

FUNCTION RXC_FLAG_EIB

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_flag_eib( boolean eib_flag )
{
  /* Save EIBS for last 2 frames.  Upon return from a failed handoff where
   * we were away for less than 12 frames, we turn the transmitter on
   * immediately and use the 'previous' EIB in the first frame we transmit
   * and the 'current' EIB in the second frame.  The eib_flag associated
   * with the first call to this function after the return is garbage because
   * it is from a frame received while tuning back.  So we only want to
   * update the previous and current EIB values after we have transmitted
   * two frames.
   */
  if ( ( ! rxc_rifhho_cfs_gps_in_progress ) &&
       ( ! rxc_hho_data.rxc_use_stored_eibs ) )
  {
    rxc_hho_data.rxc_eib_flags.previous = rxc_hho_data.rxc_eib_flags.current;
    rxc_hho_data.rxc_eib_flags.current = eib_flag;
  }

  if ( rxc_hho_data.rxc_use_stored_eibs )
  {
    /* If txc has not received an encoder interrupt since we
     * returned, then TXC has not yet used the first EIB we
     * had stored yet, so we need to wait one more frame before
     * injecting the second one.
     */
    if ( txc_int_occurred() )
    {
      rxc_hho_data.rxc_use_stored_eibs = FALSE;
    }
    else
    {
      return;
    }
  }

  txc_eib_flag = rxc_hho_data.rxc_eib_flags.current;
} /* rxc_flag_eib */

#ifdef FEATURE_IS2000_REL_A
/*===========================================================================

FUNCTION RXC_FLAG_SCH_EIB

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_flag_sch_eib( boolean eib_flag )
{
  /* Save EIBS for last 2 frames.  Upon return from a failed handoff where
   * we were away for less than 12 frames, we turn the transmitter on
   * immediately and use the 'previous' EIB in the first frame we transmit
   * and the 'current' EIB in the second frame.  The eib_flag associated
   * with the first call to this function after the return is garbage because
   * it is from a frame received while tuning back.  So we only want to
   * update the previous and current EIB values after we have transmitted
   * two frames.
   */
  if( ( ! rxc_rifhho_cfs_gps_in_progress ) &&
      ( ! rxc_hho_data.rxc_use_stored_sch_eibs ) )
  {
    rxc_hho_data.rxc_sch_eib_flags.previous =
      rxc_hho_data.rxc_sch_eib_flags.current;
    rxc_hho_data.rxc_sch_eib_flags.current = eib_flag;
  }

  if( rxc_hho_data.rxc_use_stored_sch_eibs )
  {
    /* If txc has not received an encoder interrupt since we
     * returned, then TXC has not yet used the first EIB we
     * had stored yet, so we need to wait one more frame before
     * injecting the second one.
     */
    if ( txc_int_occurred() )
    {
      rxc_hho_data.rxc_use_stored_sch_eibs = FALSE;
    }
    else
    {
      return;
    }
  }

  txc_sch_eib_flag = rxc_hho_data.rxc_sch_eib_flags.current;
} /* rxc_flag_sch_eib */

/*===========================================================================

FUNCTION RXC_FLAG_QIB

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_flag_qib( boolean qib_flag )
{
  /* Save QIBS for last 2 frames.  Upon return from a failed handoff where
   * we were away for less than 12 frames, we turn the transmitter on
   * immediately and use the 'previous' QIB in the first frame we transmit
   * and the 'current' QIB in the second frame.  The qib_flag associated
   * with the first call to this function after the return is garbage because
   * it is from a frame received while tuning back.  So we only want to
   * update the previous and current QIB values after we have transmitted
   * two frames.
   */
  if ( ( ! rxc_rifhho_cfs_gps_in_progress ) &&
       ( ! rxc_hho_data.rxc_use_stored_qibs ) )
  {
    rxc_hho_data.rxc_qib_flags.previous = rxc_hho_data.rxc_qib_flags.current;
    rxc_hho_data.rxc_qib_flags.current = qib_flag;
  }

  if ( rxc_hho_data.rxc_use_stored_qibs )
  {
    /* If txc has not received an encoder interrupt since we
     * returned, then TXC has not yet used the first QIB we
     * had stored yet, so we need to wait one more frame before
     * injecting the second one.
     */
    if ( txc_int_occurred() )
    {
      rxc_hho_data.rxc_use_stored_qibs = FALSE;
    }
    else
    {
      return;
    }
  }

  txc_qib_flag = rxc_hho_data.rxc_qib_flags.current;
} /* rxc_flag_qib */
#endif /* FEATURE_IS2000_REL_A */

#ifdef FEATURE_IS2000_1X_ADV
/*===========================================================================

FUNCTION RXC_UPDATE_1XA_COUNTERS

DESCRIPTION
  This function updates 1xA ack and early decode counters.

  FL Early Decode counters are incremented based on the received
  rate indicated by FW RDA and the PCG in which decode occured.

  FL ACK test mode counters are updated based on the expected value
  (ACK or NAK) versus the actual received value.

  The following types are used for the FL:
  CAI_FACK_EACK_RACK_ID for expected ACK received ACK
  CAI_FACK_EACK_RNAK_ID for expected ACK received NAK
  CAI_FACK_ENAK_RACK_ID for expected NAK received ACK
  CAI_FACK_ENAK_RNAK_ID for expected NAK received NAK

  RL ACK counters are updated based on whether ACK or NAK was sent
  and in which PCG the transmission occured in.

  The following types are used on the RL:
  CAI_RACK1_ACK_ID = ACK Transmitted
  CAI_RACK1_NAK_ID = NAK Transmitted

DEPENDENCIES
  * 1x should be in a RC11 call
  * ACK test mode counters require in an SO74 (Markov) or SO75 (loopback) call.
  * rxc_rc11.fackch_ack_or_nak_test_mode should have already been set appropriately.
  * rxc_rc11.status.ack_received_pc should be
    updated every frame with the PCG the ack was recieved in.
  * If 1x is transmitting, rxc_rc11.status.ack_transmitted_pcg must be updated
    prior to calling this function in order to log the last Tx'd frame.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_update_1xA_counters( void )
{
  uint16 mux_value     = 0;
  boolean ack_received = FALSE;
  boolean is_valid     = TRUE;

  /* FORWARD LINK EARLY DECODE COUNTERS */

  /* Find the initial starting point for counter IDs
     based on the decoded frame rate. */
  switch( rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_dec_rate )
  {
    case CAI_FULL_RATE:
      mux_value = CAI_PCG_0_FOR_FCH_1_ID;
      break;

    case CAI_HALF_RATE:
      mux_value = CAI_PCG_0_FOR_FCH_2_ID;
      break;

    case CAI_QUARTER_RATE:
      mux_value = CAI_PCG_0_FOR_FCH_3_ID;
      break;

    case CAI_EIGHTH_RATE:
      mux_value = CAI_PCG_0_FOR_FCH_4_ID;
      break;

    default:
      is_valid = FALSE;
      break;
  }

  if( is_valid )
  {
    /* If the rate is full-1/8th apply the proper offset to attain the
       correct counter ID to increment. */
    if( rxc_traffic_status.rxc_rc11.status.early_decode_pcg )

    if( ( rxc_traffic_status.rxc_rc11.status.early_decode_pcg > 0 ) &&
        ( rxc_traffic_status.rxc_rc11.status.early_decode_pcg < 16 ) )
    {
      mux_value += ( (CAI_PCG_1_FOR_FCH_1_ID - CAI_PCG_0_FOR_FCH_1_ID) *
                     rxc_traffic_status.rxc_rc11.status.early_decode_pcg);
      parm_inc( mux_value, 1 );
    }
  }

  /* FORWARD LINK ACK COUNTERS */

  /* Check if an ACK was received in the correct range of PCGs. Technically,
     only even PCGs are permitted, but we'll check them all here and increment
     the appropriate counter.

     NOTE: PCG 15 is not a valid PCG for receiving ACKs on the FL. Only
     even PCGs are permitted and no ACK would arrived that late in the frame.
     This PCG is used to indicate no ACK was received this frame.

     Subtract 1 from the pcg to place the mux_value with the correct range.
  */
  if( ( rxc_traffic_status.rxc_rc11.status.ack_received_pcg > 0 ) &&
      ( rxc_traffic_status.rxc_rc11.status.ack_received_pcg < 15 ) )
  {
    mux_value = CAI_ACK_RX_PCG_1_FCH_ID +
                ( rxc_traffic_status.rxc_rc11.status.ack_received_pcg - 1 );
    parm_inc( mux_value, 1 );

    /* Flag that an ACK was received. */
    ack_received = TRUE;
  }

  /* Reset valid flag. */
  is_valid = TRUE;

  switch( rxc_traffic_status.rxc_rc11.fackch_ack_or_nak_test_mode )
  {
    case FACKCH_TEST_MODE_DISABLED:
    case FACKCH_TEST_MODE_ACK:
    {
      if( ack_received )
      {
        /* Expected ACK got ACK. */
        mux_value = CAI_FACK_EACK_RACK_ID;
      }
      else
      {
        /* Expected ACK got NACK. */
        mux_value = CAI_FACK_EACK_RNAK_ID;
      }
      break;
    }

    case FACKCH_TEST_MODE_NAK:
    {
      if( ack_received )
      {
        /* Expected NAK got ACK. */
        mux_value = CAI_FACK_ENAK_RACK_ID;
      }
      else
      {
        /* Expected NAK got NAK. */
        mux_value = CAI_FACK_ENAK_RNAK_ID;
      }
      break;
    }

    default:
    {
      /* Invalid test scenario. */
      is_valid = FALSE;
    }
  }

  /* Only update the counters if the mode is valid */
  if( is_valid )
  {
    switch( rxc_so.pri )
    {
      case CAI_SO_LOOPBACK_SO75:
        loopback_log_rx( mux_value );
        break;

      case CAI_SO_MARKOV_SO74:
        mar_log_rx( mux_value );
        break;

      default:
        /* Not a valid SO for updating counters */
        break;
    }
  }

  /* REVERSE LINK ACK COUNTERS */

  /* Update RL ACK counters specific to Markov SO74 */
  if( rxc_so.pri == CAI_SO_MARKOV_SO74 )
  {
    if( rxc_traffic_status.rxc_tx_in_traffic_on )
    {
      /*Since the mobile is not permitted to Tx an ACK in PCG 0 this is
        used to indicate that No ACK was transmitted. */
      if( rxc_traffic_status.rxc_rc11.status.ack_transmitted_pcg == 0 )
      {
        /*NAK Transmitted*/
        mux_value = CAI_RACK1_NAK_ID;
      }
      else
      {
        /* ACK Transmitted*/
        mux_value = CAI_RACK1_ACK_ID;
      }

      mar_log_tx( mux_value );
    }
  }

  /* Count the total number of ACKs transmitted for all SOs.
     NOTE: PCG 0 is not a valid PCG for transmitting ACKs on the RL.
     It is used to indicate no ACK transmitted for logging so
     avoid incrementing that counter.
  */
  if( ( rxc_traffic_status.rxc_rc11.status.ack_transmitted_pcg > 0 ) &&
      ( rxc_traffic_status.rxc_rc11.status.ack_transmitted_pcg < 16 ) )
  {
    mux_value = CAI_ACK_TX_PCG_0_FCH_ID +
                  rxc_traffic_status.rxc_rc11.status.ack_transmitted_pcg;
    parm_inc( mux_value, 1 );
  }
} /* rxc_update_1xA_counters */

/*===========================================================================

FUNCTION RXC_TC_ISR_PROCESS_1XA_FRAME

DESCRIPTION
  This function a subroutine for the Decoder interrupt handler used
  when the Receive task is processing the Traffic Channel during 1x Advanced
  radio configurations.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_tc_isr_process_1xA_frame( dec_rc_type curr_rc )
{
  #ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
  boolean is_markov_so = (
    (rxc_so.pri == CAI_SO_MARKOV_SO74)  ||
    (rxc_so.pri == CAI_SO_MARKOV)       ||
    (rxc_so.pri == CAI_SO_MARKOV_13K)   ||
    (rxc_so.pri == CAI_SO_RS1_MARKOV)   ||
    (rxc_so.pri == CAI_SO_RS2_MARKOV)   ||
    (rxc_so.pri == CAI_SO_MARKOV_SO54) );
  #endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */

  /* Read FCH status from decoder output buffer for full rate and
     fractional rates */
  dec_read_fch_status( curr_rc, &rxc_fch_status );

  /* The frame information as well as frame data are stored in a temp data
     structure. When the SCH traffic ISR executes, frame information will
     be copied to the RXC decoder buffer to be de-multiplexed later in
     RXC task context.  This is similar to how legacy RCs handle decoder
     data.
  */
  rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_dec_rate =
      rxc_dec_rate_sel( (deint_rc_type) curr_rc );

  /* Read frame data.  MDSP interrupt guarantees that data is already
     stored in the decoder output buffer.  Data may become corrupted
     within the next couple of PCGs when MDSP prepares for the next
     early decoding cycle.
  */
  if ( ( rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_dec_rate !=
         CAI_BLANK_RATE )
       #ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
       || ( is_markov_so )
       #endif /* MODEM_1X_FEATURE_MSO_RDA_TEST */
  )
  {
    dec_read_fch_data( &rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_tc_buf.bits,
                       curr_rc,
                       DEC_RATE_ALL );
  }

  /* Check whether or not this frame is valid to use for FFPC setpoint
     updates and fade checking. */
  rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame =
    rxc_frame_is_valid( frame_number,
                        curr_rc,
                        rxc_traffic_status.rxc_rc11.rcp.for_fch_blanking_dutycycle );

  /* Save early decode pcg from mDSP. mDSP returns the PCG number
     prior to successful early decode, so add 1 to the value. */
  rxc_traffic_status.rxc_rc11.status.early_decode_pcg =
    muxmdsp_get_early_decode_pcg();

  rxc_traffic_status.rxc_rc11.status.ffpc_setpoint_frozen =
    muxmdsp_fch_fpc_setpoint_is_frozen();

  rxc_traffic_status.rxc_rc11.status.ack_received_pcg =
    muxmdsp_read_1xA_ack_log();

  ADV_DBG_MSG(
    "RC11 Frame Info: FRAME NUMBER=0x%x, rate=%d, guaranteed frame=%d"
    " early dec PCG=%d, Ratchet=%d, ACK Rcvd PCG=%d",
    frame_number,
    rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_dec_rate,
    rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame,
    rxc_traffic_status.rxc_rc11.status.early_decode_pcg,
    rxc_traffic_status.rxc_rc11.status.ffpc_setpoint_frozen,
    rxc_traffic_status.rxc_rc11.status.ack_received_pcg
  );

  rxc_update_1xA_counters();

} /* rxc_tc_isr_process_1xA_frame */
#endif /* FEATURE_IS2000_1X_ADV */

/*===========================================================================

FUNCTION rxc_update_sch_stats

DESCRIPTION
  This function updates various F-SCH parameters used for logging purposes.
  Such as Channel params,Frame stats,Counters,Decoder specific params etc..

DEPENDENCIES
  Need to be called after decoding is done on a SCH frame,
  with input of encoding type (CONV/TURBO)

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_update_sch_stats
(
  dec_coding_type fsch_coding_type
)
{
  /* Update the channel parameters picked from FW */
  rxc_fsch_stats.iq_acc    = muxmdsp_get_sch_iq_acc();
  rxc_fsch_stats.rt_nt_io  = muxmdsp_get_sch_rt_nt_io();
  rxc_fsch_stats.rt_ecp_io = muxmdsp_get_sch_wtd_pilot();

  /* Update Rate,Frame_Status and Coding type */
  rxc_fsch_stats.sch_rate  = rxc_sch.sch_rate;
  rxc_fsch_stats.decision  = rxc_frame_data.rxc_sch_frame_status;
  rxc_fsch_stats.code_type = fsch_coding_type;
  rxc_fsch_stats.ffpc_setpoint = (uint16)ffpc_get_sch_setpt_linQ8();

  /* Update frame counters */
  rxc_fsch_stats.total_frames++;
  if( rxc_frame_data.rxc_sch_frame_status == RXC_DTX_FRAME )
  {
    rxc_fsch_stats.dtx_frames++;
  }
  else if( rxc_frame_data.rxc_sch_frame_status == RXC_ERASURE_FRAME )
  {
    rxc_fsch_stats.erasure_frames++;
  }

  if( ( rxc_sch.sch_rate >= DEC_SCH_RATE_1X ) &&
      ( rxc_sch.sch_rate <= DEC_SCH_RATE_32X ) )
  {
    rxc_fsch_stats.rate_count[rxc_sch.sch_rate] =
    rxc_fsch_stats.rate_count[rxc_sch.sch_rate] + 1;
  }

  switch( fsch_coding_type )
  {
    case DEC_CONVOLUTIONAL:
      /* Log Conv decoder specific stats */
      rxc_fsch_stats.sch_energy                        =
        rxc_frame_data.rxc_sch_status.conv_sch_status.energy;
      rxc_fsch_stats.dec_stats.conv_sch_stats.conv_ser =
        rxc_frame_data.rxc_sch_status.conv_sch_status.ser;
      rxc_fsch_stats.dec_stats.conv_sch_stats.conv_crc =
        rxc_frame_data.rxc_sch_status.conv_sch_status.ltu_crc;
      break;

    case DEC_TURBO:
      /* Log Turbo decoder specific stats */
      rxc_fsch_stats.sch_energy                                =
        rxc_frame_data.rxc_sch_status.turbo_sch_status.energy;
      rxc_fsch_stats.dec_stats.turbo_sch_stats.turbo_crc       =
        rxc_frame_data.rxc_sch_status.turbo_sch_status.crc_pass;
      rxc_fsch_stats.dec_stats.turbo_sch_stats.turbo_iteration =
        rxc_frame_data.rxc_sch_status.turbo_sch_status.num_iteration;
      rxc_fsch_stats.dec_stats.turbo_sch_stats.turbo_min_llr   =
        rxc_frame_data.rxc_sch_status.turbo_sch_status.min_llr;
      break;

    default:
      /* Must be wrong coding type, Flash error */
      M1X_MSG( MUX, LEGACY_ERROR,
        "Wrong Coding type %d",
         fsch_coding_type );
      break;
  }
} /* rxc_update_sch_stats */

/*===========================================================================

FUNCTION rxc_get_sch_stats

DESCRIPTION
  This function updates various F-SCH parameters used for logging purposes.
  Such as Channel params,Frame stats,Counters,Decoder specific params etc..

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_get_sch_stats
(
 rxc_sch_demod_log_type *rxc_srch_fsch_stats
)
{
  uint8 i;/* Counter to loop thru rate count */

  if( ( rxc_sch.rc > DEC_RC2 ) &&
      ( rxc_frame_data.rxc_sch_frame_status != RXC_NO_FRAME ) )
  {
    rxc_srch_fsch_stats->iq_acc                       =
      rxc_fsch_stats.iq_acc;
    rxc_srch_fsch_stats->rt_nt_io                     =
      rxc_fsch_stats.rt_nt_io;
    rxc_srch_fsch_stats->rt_ecp_io                    =
      rxc_fsch_stats.rt_ecp_io;

    rxc_srch_fsch_stats->sch_rate                     =
      rxc_fsch_stats.sch_rate;
    rxc_srch_fsch_stats->decision                     =
      rxc_fsch_stats.decision;
    rxc_srch_fsch_stats->sch_energy                   =
      rxc_fsch_stats.sch_energy;
    rxc_srch_fsch_stats->code_type                    =
      rxc_fsch_stats.code_type;
    rxc_srch_fsch_stats->ffpc_setpoint                =
      rxc_fsch_stats.ffpc_setpoint;

    rxc_srch_fsch_stats->dec_stats                    =
      rxc_fsch_stats.dec_stats;

    rxc_srch_fsch_stats->total_frames                 =
      rxc_fsch_stats.total_frames;
    rxc_srch_fsch_stats->erasure_frames               =
      rxc_fsch_stats.erasure_frames;
    rxc_srch_fsch_stats->dtx_frames                   =
      rxc_fsch_stats.dtx_frames;

    for( i = 0;i < 6; i++ )
    {
      rxc_srch_fsch_stats->rate_count[i]              =
        rxc_fsch_stats.rate_count[i];
    }
  }
  else
  {
    memset(rxc_srch_fsch_stats,0,sizeof(rxc_sch_demod_log_type));
    rxc_srch_fsch_stats->decision = RXC_NO_FRAME;
  }
} /* rxc_get_sch_stats */

/*===========================================================================

FUNCTION RXC_TC_ISR

DESCRIPTION
  This function is the interrupt handler for the Decoder interrupt used
  when the Receive task is processing the Traffic Channel.
  Due to timing requirements imposed by the 13k decoder and the erasure
  indicator bit turnaround time we need to first process the decoded frame
  and decide what rate it is before building the final tx frame at this
  point in time.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_tc_isr( void )
{
  rxc_dec_type *buf_ptr;
    /* pointer to dequeued receive task buffer */

  qword tmp_qw_frame_offset;
    /* quadword to hold the rxc_frame_offset */

  qword frame_num_with_offset;
    /* quadword to hold the Frame # + offset */

  byte task_id = 0;
    /* task_id indicates if the decoder interrupt if FCH or SCH/SCCH. */

  /* frame status for FCH and DCCH
  */
  rxc_frame_type rxc_fch_frame_status  = RXC_ERASURE_FRAME;
  rxc_frame_type rxc_dcch_frame_status = RXC_ERASURE_FRAME;
  boolean log_ffpc_fch_data            = FALSE;
  boolean log_ffpc_dcch_data           = FALSE;

  #ifdef FEATURE_IS2000_REL_A
  boolean qib;
  #endif /* FEATURE_IS2000_REL_A */

  /* Static storage for DCCH channel quality indicator
   * between FCH/DCCH interrupt and SCH interrupt when
   * both are present.  This value is calculated in the
   * first interrupt, and is stored here to be used in
   * building the dec message in the second interrupt.
   */
  static rxc_dcch_quality_type  dcch_chan_quality;

  /* Variables to keep track of CRC ERR MSG */
  #define RXC_SCH_CRC_MAX_CNT 100
  static uint32 sch_crc_err = 0; // number of crc errors during last RXC_SCH_CRC_MAX_CNT framse
  static uint32 sch_crc_err_dtx_frame_cnt = 0; // number of frames since clearing sch_crc_err
  static uint32 sch_dtx_frame_cnt = 0;// number of DTX frames during last RXC_SCH_CRC_MAX_CNT framse
  boolean ltu_flag;    /* indicates if the LTU CRC passed/failed */
  word    index;       /* dummy loop counter for LTU CRCs        */
  byte    ltu_cntr;    /* number ltu's in the SDU                */
  word    mask;        /* dummy mask for LTU CRCs                */
  byte    ltu_cnt;     /* dummy counter for loops                */
  word    ltu_crc;     /* the ltu crc bits, after being shifted  */
  #ifdef FEATURE_IS2000_REL_A /* 32X */
  byte    good_ltu;    /* number of good LTUs in a frame         */
  #endif /* FEATURE_IS2000_REL_A */

  #ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
  boolean is_markov_so = (
    (rxc_so.pri == CAI_SO_MARKOV_SO74)  ||
    (rxc_so.pri == CAI_SO_MARKOV)       ||
    (rxc_so.pri == CAI_SO_MARKOV_13K)   ||
    (rxc_so.pri == CAI_SO_RS1_MARKOV)   ||
    (rxc_so.pri == CAI_SO_RS2_MARKOV)   ||
    (rxc_so.pri == CAI_SO_MARKOV_SO54) );
  #endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */

  uint8 dec_int_status = 0;

  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  /* get the RF device currently in use */
  rfm_device_enum_type   rf_device = srch_mux_get_rx_device();
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

  cai_radio_config_type curr_rc = CAI_INIT_RC;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - -*/

  curr_rc = rxc_so.for_dcch.included ?
            rxc_so.for_dcch.rc : curr_rc;

  curr_rc = rxc_so.for_fch.included ?
            rxc_so.for_fch.rc : curr_rc;

  if( CAI_INIT_RC == curr_rc )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
       "Invalid RC%d in rxc_tc_isr",
       curr_rc );

    return;
  }

  /* Get the current frame number + offset. Note that the current frame
     being processed belongs to a previous frame number since we read the
     data from the decoder after the end of the frame boundary */
  m1x_time_get_20ms_frame_offset_time( frame_num_with_offset );

  /* Subtract 1 from the frame number to align with the received frame */
  frame_number = (qw_lo(frame_num_with_offset) - 1) & 0xFFFF;

  #ifdef FEATURE_IS2000_1X_ADV
  /* FCH RC11 processing */
  if( rxc_traffic_status.rxc_rc11.tc_source_mdsp_isr == TRUE )
  {
    /* Since this is the MDSP ISR, the frame work has to be set so the rest of
       the traffic ISR processing can mimic the traditional decoder ISR.
       FCH is the only channel that uses the MDSP ISR.
    */
    task_id = DEC_FCH_TASK;
  }
  else
  #endif /* FEATURE_IS2000_1X_ADV */
  {
    dec_int_status = (uint8) GET_DEMBACK_REG( DEC_DONE_INT_STATUS );

    /* Clear DEC DONE int status to make sure ISR is properly serviced */
    SET_DEMBACK_REG( DEC_CLR_INT_STATUS,
                     HWIO_FMSK( DEC_CLR_INT_STATUS, VD_CLR_STATUS ) );

    if( !( dec_int_status & 0x2 ) )
    {
      return;
    }

    task_id = (byte) GET_DEMBACK_REG_F( DEINT_TASK_STATUS, TASK_ID );
    rxc_traffic_status.saved_dec_int_task_id = task_id; /* Save the task ID for debugging purpose */
  }

  /* Decrement tuneback count */
  if( ( ( task_id == DEC_FCH_TASK )
        #ifdef FEATURE_IS2000_REL_A_SVD
        || ( task_id == DEC_DCCH_TASK )
        #endif /* FEATURE_IS2000_REL_A_SVD */
      ) && ( rxc_cfs_tuneback_count > 0 )
    )
  {
    rxc_cfs_tuneback_count--;
  }

  if( ( task_id == DEC_FCH_TASK )
      #ifdef FEATURE_IS2000_REL_A_SVD
      || ( task_id == DEC_DCCH_TASK )
      #endif /* FEATURE_IS2000_REL_A_SVD */
    )
  {
    /* Latch the reverse power control decision history from register. */
    rxc_bufs_qs_data.rxc_rpc_history = muxmdsp_rpc_get_history();

    /* Latch the FFPC decision history from register. */
    rxc_bufs_qs_data.rxc_fpc_history = muxmdsp_fpc_get_history();

    #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
    /* Record AGC vals */
    log_rx_agc = (byte) rfm_1x_get_rx_agc (rf_device);
    log_tx_pwr = (byte)( txc_get_tx_agc_in_legacy_units_dbm12() >> 2 );
    #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */
    log_tx_gain_adj = srch_mux_get_tx_adj ();

    /* Start logging FFPC data after spurious interrupts are over */
    if( rxc_common_data.rxc_frame_offset_countdown == 0 )
    {
      if( rxc_so.for_dcch.included )
      {
        log_ffpc_dcch_data = TRUE;
      }
      if( rxc_so.for_fch.included )
      {
        log_ffpc_fch_data  = TRUE;
      }

      /* Log FFPC data only if RC > 2 */
      if( rxc_so.high_rc )
      {
        RXC_ENTER_CRITICAL_SECTION();
        ffpc_log_frame( rxc_common_data.rxc_frame_offset, rxc_so.pri,
                        rxc_bufs_qs_data.rxc_fpc_history,
                        log_ffpc_fch_data, log_ffpc_dcch_data,
                        rxc_log_sch_data.rxc_log_ffpc_sch_data );
        RXC_LEAVE_CRITICAL_SECTION();
      }
    }
  }

  if( ( ( task_id == DEC_FCH_TASK )
#ifdef FEATURE_IS2000_REL_A_SVD
       || (task_id == DEC_DCCH_TASK)
#endif /* FEATURE_IS2000_REL_A_SVD */
      ) && (rxc_sup_dec_int_enable)
     )
  {
    #ifdef FEATURE_IS2000_REL_A_SVD
    /* Since there are 2 frames lag time between when the DCCH/FCH is set up and
    ** when data is valid, need to count down these 2 frames.  Without this
    ** mechanism, MUX may deliver stale data to its clients.  The time line is
    ** shown below:
    **    Frame 0:  Start Deint programming by calling deint_set_xxch_mode()
    **    Frame 1:  In ATMR ISR, decrement countdown counter
    **    Frame 2:  In rxc_tc_isr, if count down counter is 1, decrement it
    **              and set data_valid flag.
    ** The count down is set to 2 when the channel is set up.
    ** In the next frame, it is decremented in the ATMR ISR.
    ** In the frame after that, it is decremented in the TC ISR and data valid
    ** is set to TRUE only in the TC ISR.
    ** Reason for this scheme:  in TCI state, there is no TC ISR so we must use
    ** the ATMR ISR to count down.
    ** However, in the 2nd frame, when data is valid, we must make sure data valid
    ** flag is set to TRUE to read it at the TC ISR interrupt.  ATMR interrupt happens
    ** in the next bin, when we already serviced the TC ISR.
    */
    if( ( rxc_so.for_dcch.included ) &&
        ( !( rxc_so.for_dcch.data_valid ) ) &&
        ( rxc_so.for_dcch.data_valid_countdown <= 1 ) )
    {
      rxc_so.for_dcch.data_valid_countdown = 0;
      rxc_so.for_dcch.data_valid           = TRUE;
    }

    if( ( rxc_so.for_fch.included ) &&
        ( !( rxc_so.for_fch.data_valid ) ) &&
        ( rxc_so.for_fch.data_valid_countdown <= 1 ) )
    {
      rxc_so.for_fch.data_valid_countdown = 0;
      rxc_so.for_fch.data_valid           = TRUE;
    }

    /* Get a time stamp of this decoder interrupt for debugging purpose */
    m1x_time_get_20ms_frame_offset_time( rxc_traffic_status.rxc_dec_int0_fr_time );

    /* Service DCCH channel. */
    if( rxc_so.for_dcch.included )
    {
      dec_read_dcch_status( (dec_rc_type) curr_rc, &rxc_dcch_status );
      rxc_dcch_rate = (word) rxc_dtx_det_dcch( (dec_rc_type) curr_rc ); // determine rate
      if( rxc_dcch_rate == (word) CAI_BLANK_RATE )
      {
        rxc_dcch_frame_status = RXC_ERASURE_FRAME;
        ffpc_update_setpt( rxc_dcch_frame_status, FFPC_DCCH );
      }
      else if( rxc_dcch_rate == (word) CAI_NULL_RATE )
      {
        rxc_dcch_frame_status = RXC_DTX_FRAME;
        /* no need to update FFPC setpts in case of DTX frames. */
      }
      else
      {
        rxc_dcch_frame_status = RXC_GOOD_FRAME;
        ffpc_update_setpt( rxc_dcch_frame_status, FFPC_DCCH );
      }
    }

    /* Service FCH channel. */
    if( rxc_so.for_fch.included )
    {
      #ifdef FEATURE_IS2000_1X_ADV
      /* FCH RC11 processing */
      if( rxc_traffic_status.rxc_rc11.tc_source_mdsp_isr == TRUE )
      {
        rxc_tc_isr_process_1xA_frame( (dec_rc_type) curr_rc );

        /* Store frame rate similar to legacy RCs. */
        rxc_frame_data.rxc_fch_rate =
            (word) rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_dec_rate;

        if( rxc_frame_data.rxc_fch_rate == (word) CAI_BLANK_RATE )
        {
          rxc_fch_frame_status = RXC_ERASURE_FRAME;
        }
        else
        {
          rxc_fch_frame_status = RXC_GOOD_FRAME;
        }

        /* Only update FFPC setpoint if MDSP indicates to do so. */
        if( rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame == TRUE )
        {
          ffpc_update_setpt( rxc_fch_frame_status, FFPC_FCH );
        }
      }
      else
      #endif /* FEATURE_IS2000_1X_ADV */
      {
        dec_read_fch_status( (dec_rc_type) curr_rc, &rxc_fch_status );
        rxc_frame_data.rxc_fch_rate = (word) rxc_dec_rate_sel( (deint_rc_type) curr_rc );
        if( rxc_frame_data.rxc_fch_rate == (word) CAI_BLANK_RATE )
        {
          rxc_fch_frame_status = RXC_ERASURE_FRAME;
        }
        else
        {
          rxc_fch_frame_status = RXC_GOOD_FRAME;
        }
        ffpc_update_setpt( rxc_fch_frame_status, FFPC_FCH );
      }

      rxc_ffch_rate = (cai_data_rate_type) rxc_frame_data.rxc_fch_rate;
    }
    #else /* else of FEATURE_IS2000_REL_A_SVD */

    #ifdef FEATURE_IS2000_1X_ADV
    /* FCH RC11 processing */
    if( rxc_traffic_status.rxc_rc11.tc_source_mdsp_isr == TRUE )
    {
      rxc_tc_isr_process_1xA_frame( (dec_rc_type) curr_rc );

      rxc_frame_data.rxc_fch_rate =
          (word) rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_dec_rate;

      if( rxc_frame_data.rxc_fch_rate == (word) CAI_BLANK_RATE )
      {
        rxc_fch_frame_status = RXC_ERASURE_FRAME;
      }
      else
      {
        rxc_fch_frame_status = RXC_GOOD_FRAME;
      }

      /* Only update FFPC setpoint if MDSP indicates to do so. */
      if( rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame == TRUE )
      {
        ffpc_update_setpt( rxc_fch_frame_status, FFPC_FCH );
      }
    }
    else
    #endif /* FEATURE_IS2000_1X_ADV */
    {
      // rxc_fch_status is shared between FCH and DCCH
      dec_read_fch_status( (dec_rc_type) curr_rc, &rxc_fch_status );

      rxc_frame_data.rxc_fch_rate = (word) rxc_dec_rate_sel( (deint_rc_type) curr_rc );

      if( rxc_so.for_dcch.included )
      {
        if( rxc_frame_data.rxc_fch_rate == CAI_BLANK_RATE )
        {
          rxc_dcch_frame_status = RXC_ERASURE_FRAME;
          ffpc_update_setpt( rxc_dcch_frame_status, FFPC_DCCH );
        }
        else if( rxc_frame_data.rxc_fch_rate == CAI_NULL_RATE )
        {
          rxc_dcch_frame_status = RXC_DTX_FRAME;
          /* no need to update FFPC setpts */
        }
        else
        {
          rxc_dcch_frame_status = RXC_GOOD_FRAME;
          ffpc_update_setpt( rxc_dcch_frame_status, FFPC_DCCH );
        }
      }

      if( rxc_so.for_fch.included )
      {
        if( rxc_frame_data.rxc_fch_rate == CAI_BLANK_RATE )
        {
          rxc_fch_frame_status = RXC_ERASURE_FRAME;
        }
        else
        {
          rxc_fch_frame_status = RXC_GOOD_FRAME;
        }
        ffpc_update_setpt( rxc_fch_frame_status, FFPC_FCH );
      }
    }
    #endif /* FEATURE_IS2000_REL_A_SVD */

    /* Call the function to determine DCCH channel quality */
    if( rxc_so.for_dcch.included )
    {
      dcch_chan_quality = rxc_dcch_quality_det( curr_rc, rxc_dcch_frame_status );
    }

    #ifdef FEATURE_IS2000_REL_A
    #ifdef FEATURE_IS2000_REL_A_SVD
    if( ffpc_channel == CAI_FPC_PRI_CHAN_DCCH )
    #else
    if( rxc_so.for_dcch.included )
    #endif /* FEATURE_IS2000_REL_A_SVD */
    {
      qib = ( dcch_chan_quality == RXC_POOR_CHANNEL );
    }
    else /* fch case below */
    {
      qib = ( rxc_fch_frame_status != RXC_GOOD_FRAME );
    }

    rxc_flag_qib(qib);
#endif /* FEATURE_IS2000_REL_A */

    if( (
          #ifdef FEATURE_IS2000_REL_A_SVD
          ( ffpc_channel == CAI_FPC_PRI_CHAN_DCCH) &&
          #else
          ( rxc_so.for_dcch.included) &&
          #endif /* FEATURE_IS2000_REL_A_SVD */
          ( rxc_dcch_frame_status == RXC_GOOD_FRAME)
        ) ||
        ( 
          #ifdef FEATURE_IS2000_REL_A_SVD
          ( ffpc_channel == CAI_FPC_PRI_CHAN_FCH) &&
          #else
          ( rxc_so.for_fch.included) &&
          #endif /* FEATURE_IS2000_REL_A_SVD */
          ( rxc_fch_frame_status == RXC_GOOD_FRAME)
        ) )
    {
      rxc_flag_eib( FALSE ) ;
    }
    else
    {
      /* flag to indicate EIB bit pattern to be punctured on reverse link
      ** for FCH since there will be no DTX nothing changes,
      ** for DCCH, ERASURE and DTX frames are to be treated identically.
      */
      rxc_flag_eib( TRUE ) ;
    }

    /* Return from the interrupt because sup channel interrupt will
    ** follow this */
    return;
  }

  /* Perform Normal interrupt processing */
  if( rxc_common_data.rxc_frame_offset_countdown > 0 )
  {
    /* rxc_frame_offset_countdown is used to skip over the troublesome,
    ** sporadic interrupts and correct any timing errors that may have occured.
    */

    rxc_common_data.rxc_frame_offset_countdown--;

    if( rxc_common_data.rxc_frame_offset_countdown == 0 )
    {
      /* The MSM3000 should be settled into normal 20 ms traffic frames now.
      ** Compute the correct rxc_dec_frame and then increment from there.  This
      ** is a time correction to fix any off by 1 errors on rxc_dec_frame.
      */
      (void) m1x_time_get( rxc_dec_frame );

      /* Divide by 64*1024 to truncate the low 16 bits of the time stamp,
      ** then divide the 1.25 ms units by 16 to get 20ms units.
      */
      (void) qw_div_by_power_of_2( rxc_dec_frame, rxc_dec_frame, 16 );
      qw_set( tmp_qw_frame_offset, 0L, rxc_common_data.rxc_frame_offset);
      qw_sub( rxc_dec_frame, rxc_dec_frame, tmp_qw_frame_offset );
      (void) qw_div_by_power_of_2( rxc_dec_frame, rxc_dec_frame, 4 );

      /* rxc_dec_frame holds the number of the decoded frame, which is 20ms
       * less than the current frame time. */
      qw_dec( rxc_dec_frame, 1L );
    }
    else
    {
      qw_inc( rxc_dec_frame, 1L );
    }
  }
  else
  {
    qw_inc( rxc_dec_frame, 1L );
  }

  /* Copy decoder data into buffer for task */
  if ( (buf_ptr = (rxc_dec_type *)q_get( &rxc_common_data.rxc_dec_free_q)) ==
       NULL )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "No buffers on rxc_dec_free_q" );
  }
  else
  {
    /* Determine the rate of the received forward traffic channel frame.
       Note that this rate determination is independent of decoded data and
       uses only CRC, QBITS, energy, and SER parameters */
    #ifdef FEATURE_IS2000_REL_A_SVD
    /* If rate determination has not been done, then do rate determination.
    ** This check used to be for rxc_sup_dec_int_enable only.  Checking
    ** rxc_sup_dec_int_enable only is not robust enough in case this flag
    ** is set but the FCH/DCCH interrupt which precedes the SCH interrupt
    ** in the same frame did not execute.  This anomaly is very rare but
    ** can occur due to possible INTLOCK or other SW conditions which
    ** prevent the FCH/DCCH interrupt from happening.  By the time the SCH
    ** interrupt happens, the Deint task ID will only reflects the SCH
    ** interrupt, the FCH/DCCH interrupt is completely lost.  In this
    ** bad case, even though rxc_sup_dec_int_enable is set, rate determination
    ** has not been done.
    **
    ** So it is safer to add validation for rxc_dcch_rate and rxc_fch_rate
    ** as well.  If the content of these variables is greater than the max
    ** possible valid rate (they are reset to an invalid value every frame,
    ** after they are used), then do rate determination.  If the content of
    ** these rates is valid, then use them as decoder rate for the frame and
    ** skip rate determination since it has been done earlier in the frame.
    */
    if( ( !rxc_sup_dec_int_enable ) ||
        ( ( rxc_so.for_dcch.included ) &&
          ( rxc_dcch_rate > ( (word) CAI_NULL_RATE ) ) ) ||
        ( ( rxc_so.for_fch.included ) &&
          ( rxc_frame_data.rxc_fch_rate > ( (word) CAI_NULL_RATE ) ) ) )
    {
      /* This is a FCH/DCCH Interrupt, Supplemental Channel not enabled
      ** and rate determination has not been done yet.
      */

      /* Since there are 2 frames lag time between when the DCCH/FCH is set up and
      ** when data is valid, need to count down these 2 frames.  Without this
      ** mechanism, MUX may deliver stale data to its clients.  The time line is
      ** shown below:
      **    Frame 0:  Start Deint programming by calling deint_set_xxch_mode()
      **    Frame 1:  In ATMR ISR, decrement countdown counter
      **    Frame 2:  In rxc_tc_isr, if count down counter is 1, decrement it
      **              and set data_valid flag.
      ** The count down is set to 2 when the channel is set up.
      ** In the next frame, it is decremented in the ATMR ISR.
      ** In the frame after that, it is decremented in the TC ISR and data valid
      ** is set to TRUE only in the TC ISR.
      ** Reason for this scheme:  in TCI state, there is no TC ISR so we must use
      ** the ATMR ISR to count down.
      ** However, in the 2nd frame, when data is valid, we must make sure data valid
      ** flag is set to TRUE to read it at the TC ISR interrupt.  ATMR interrupt happens
      ** in the next bin, when we already serviced the TC ISR.
      */
      if( ( rxc_so.for_dcch.included ) &&
          ( !( rxc_so.for_dcch.data_valid ) ) &&
          ( rxc_so.for_dcch.data_valid_countdown <= 1 ) )
      {
        rxc_so.for_dcch.data_valid_countdown = 0;
        rxc_so.for_dcch.data_valid           = TRUE;
      }

      if( ( rxc_so.for_fch.included ) &&
          ( !( rxc_so.for_fch.data_valid ) ) &&
          ( rxc_so.for_fch.data_valid_countdown <= 1 ) )
      {
        rxc_so.for_fch.data_valid_countdown = 0;
        rxc_so.for_fch.data_valid           = TRUE;
      }

      if( rxc_so.for_dcch.included )
      {
        /* Read status */
        dec_read_dcch_status( (dec_rc_type) curr_rc, &rxc_dcch_status );

        /* Determine rate */
        buf_ptr->dcch_dec_rate = (cai_data_rate_type) rxc_dtx_det_dcch( (dec_rc_type) curr_rc );
      }

      if( rxc_so.for_fch.included )
      {
        #ifdef FEATURE_IS2000_1X_ADV
        /* FCH RC11 processing */
        if( rxc_traffic_status.rxc_rc11.tc_source_mdsp_isr == TRUE )
        {
          rxc_tc_isr_process_1xA_frame( (dec_rc_type) curr_rc );

          /* Copy from temp buffer to tc buf to pass to RXC task. */
          buf_ptr->buf.tc_buf.bits =
            rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_tc_buf.bits;

          buf_ptr->dec_rate =
            rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_dec_rate;
        }
        else
        #endif /* FEATURE_IS2000_1X_ADV */
        {
          /* Read status */
          dec_read_fch_status( (dec_rc_type) curr_rc, &rxc_fch_status );

          /* Determine rate */
          buf_ptr->dec_rate = rxc_dec_rate_sel( (deint_rc_type) curr_rc );
        }

        rxc_ffch_rate = buf_ptr->dec_rate;
      }
    }
    else
    {
      /* Get a time stamp of this decoder interrupt for debugging purpose.
      */
      m1x_time_get_20ms_frame_offset_time( rxc_traffic_status.rxc_dec_int1_fr_time );

      /* This is the SCH interrupt after an FCH/DCCH interrupt and
      ** rate determination has already been done.
      */
      if( rxc_so.for_dcch.included )
      {
        /* Copy rate from saved value. */
        buf_ptr->dcch_dec_rate = (cai_data_rate_type) rxc_dcch_rate;
        ASSERT (buf_ptr->dcch_dec_rate <= CAI_NULL_RATE);

        /* Clear saved value after use */
        rxc_dcch_rate = RXC_NO_RATE;
      }

      if( rxc_so.for_fch.included )
      {
        /* Copy rate from saved value. */
        buf_ptr->dec_rate = (cai_data_rate_type) rxc_frame_data.rxc_fch_rate;
        ASSERT (buf_ptr->dec_rate <= CAI_NULL_RATE);

        /* Clear saved value after use */
        rxc_frame_data.rxc_fch_rate = RXC_NO_RATE;
      }
    }

    if( rxc_so.for_dcch.included )
    {
      if( buf_ptr->dcch_dec_rate == CAI_BLANK_RATE )
      {
        rxc_dcch_frame_status = RXC_ERASURE_FRAME;
      }
      else if( buf_ptr->dcch_dec_rate == CAI_NULL_RATE )
      {
        rxc_dcch_frame_status = RXC_DTX_FRAME;
      }
      else
      {
        rxc_dcch_frame_status = RXC_GOOD_FRAME;
      }
    }

    if( rxc_so.for_fch.included )
    {
      if( buf_ptr->dec_rate == CAI_BLANK_RATE )
      {
        rxc_fch_frame_status = RXC_ERASURE_FRAME;
      }
      else
      {
        rxc_fch_frame_status = RXC_GOOD_FRAME;
      }
    }

    /* Call the function to determine DCCH channel quality */
    if( rxc_so.for_dcch.included )
    {
      if( rxc_sup_dec_int_enable )
      {
        /* indication was already calculated in previous interrupt. */
        buf_ptr->dcch_chan_quality = dcch_chan_quality;
      }
      else
      {
        dcch_chan_quality = buf_ptr->dcch_chan_quality = rxc_dcch_quality_det( curr_rc,
                                                                               rxc_dcch_frame_status );
      }
    }

    /* If rxc_sup_dec_int_enable is TRUE, then this eib/qib reporting
     * has already occurred.  Skip the duplicate reporting for efficiency
     * and correctness, since rxc_flag_eib() and rxc_flag_qib() have
     * current and previous flags.
     */
    if( !rxc_sup_dec_int_enable )
    {
      if( ffpc_channel == CAI_FPC_PRI_CHAN_DCCH )
      {
        qib = ( dcch_chan_quality == RXC_POOR_CHANNEL );
      }
      else /* fch case below */
      {
        qib = ( rxc_fch_frame_status != RXC_GOOD_FRAME );
      }

      /* set status only if primary  */
      rxc_flag_qib( qib );

      if( ( ( ffpc_channel == CAI_FPC_PRI_CHAN_DCCH ) &&
            ( rxc_dcch_frame_status == RXC_GOOD_FRAME ) ) ||
          ( ( ffpc_channel == CAI_FPC_PRI_CHAN_FCH ) &&
            ( rxc_fch_frame_status == RXC_GOOD_FRAME ) ) )
      {
        rxc_flag_eib( FALSE ) ;
      }
      else
      {
        /* flag to indicate EIB bit pattern to be punctured on reverse link
        ** for FCH since there will be no DTX nothing changes,
        ** for DCCH, ERASURE and DTX frames are to be treated identically.
        */
        rxc_flag_eib( TRUE ) ;
      }
    } /* !rxc_sup_dec_int_enable */

    if( rxc_common_data.rxc_frame_offset_countdown == 0 )
    {
      /* troublesome, sporadic interrupts over, should be settled into
      ** normal 20 ms traffic frames now. Start processing FFPC.
      */

      /* SETPT already updated and set in HW if SCH is enabled.
      ** Update setpoint and write to HW here if only F-FCH/DCCH is on.
      */
      if( !rxc_sup_dec_int_enable )
      {
        /* Used in simultaneous F-FCH and F-DCCH channel set up */
        if( rxc_so.for_dcch.included )
        {
          ffpc_update_setpt( rxc_dcch_frame_status, FFPC_DCCH );
        }

        if( rxc_so.for_fch.included )
        {
          #ifdef FEATURE_IS2000_1X_ADV
          /* FCH RC11 processing */
          if( rxc_traffic_status.rxc_rc11.tc_source_mdsp_isr == TRUE )
          {
            if( rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame == TRUE )
            {
              ffpc_update_setpt( rxc_fch_frame_status, FFPC_FCH );
            }
          }
          else
          #endif /* FEATURE_IS2000_1X_ADV */
          {
            ffpc_update_setpt( rxc_fch_frame_status, FFPC_FCH );
          }
        }
      }
    }

    #ifdef FEATURE_IS2000_1X_ADV
    /* FCH RC11 processing */
    if( curr_rc == CAI_RC_11 )
    {
      if( rxc_sup_dec_int_enable )
      {
        /* If supplemental channel is assigned, decoder information is still stored
           in a temp data structure and needs to be copied to the RXC decoder buffer
           to be de-multiplexed later in RXC task context.
        */
        buf_ptr->buf.tc_buf.bits =
          rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_tc_buf.bits;
      }
    }
    else
    #endif /* FEATURE_IS2000_1X_ADV */
    {
      /* If determined rate is not erasure or blank then read the data from
      ** the decoder buffer only for the selected rate */
      if( rxc_so.for_fch.included )
      {
        if( ( ( buf_ptr->dec_rate != CAI_BLANK_RATE ) &&
              ( buf_ptr->dec_rate != CAI_NULL_RATE ) )
           #ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
           || ( is_markov_so )
           #endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */
        )
        {
          // Read in FCH data here
          dec_read_fch_data( &buf_ptr->buf.tc_buf.bits, 
                             (dec_rc_type) curr_rc,
                             DEC_RATE_ALL );
        }
      }
    }

    if( rxc_so.for_dcch.included )
    {
      if( buf_ptr->dcch_dec_rate != CAI_BLANK_RATE &&
          buf_ptr->dcch_dec_rate != CAI_NULL_RATE )
      {
        dec_read_dcch_data( &buf_ptr->buf.tc_buf.bits,
                            (dec_rc_type) curr_rc );
      }
    }
    #else /* else of FEATURE_IS2000_REL_A_SVD */
    if( !rxc_sup_dec_int_enable || ( rxc_frame_data.rxc_fch_rate == RXC_NO_RATE ) )
    {
      #ifdef FEATURE_IS2000_1X_ADV
      /* FCH RC11 processing */
      if( rxc_traffic_status.rxc_rc11.tc_source_mdsp_isr == TRUE )
      {
        rxc_tc_isr_process_1xA_frame( (dec_rc_type) curr_rc );

        /* Copy from temp buffer to tc buf to pass to RXC task. */
        buf_ptr->buf.tc_buf.bits =
          rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_tc_buf.bits;

        buf_ptr->dec_rate =
          rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_dec_rate;
      }
      else
      #endif /* FEATURE_IS2000_1X_ADV */
      {
        /* This is a FCH/DCCH Interrupt: Supplemental Channel not enabled
         * or this is the SCH interrupt and rate determination has not
         * been done yet. */
        // rxc_fch_status is shared between FCH and DCCH
        dec_read_fch_status( (dec_rc_type) curr_rc, &rxc_fch_status );

        /* Determine Rate */
        buf_ptr->dec_rate = rxc_dec_rate_sel( (deint_rc_type) curr_rc );
      }
    }
    else
    {
      /* This is the SCH interrupt after an FCH interrupt and
       * rate determination has already been done. */
      buf_ptr->dec_rate = (cai_data_rate_type) rxc_frame_data.rxc_fch_rate;
      rxc_frame_data.rxc_fch_rate = RXC_NO_RATE; /* Clear it after use */
    }

    if( rxc_so.for_dcch.included )
    {
      if( buf_ptr->dec_rate == CAI_BLANK_RATE )
      {
        rxc_dcch_frame_status = RXC_ERASURE_FRAME;
      }
      else if( buf_ptr->dec_rate == CAI_NULL_RATE )
      {
        rxc_dcch_frame_status = RXC_DTX_FRAME;
      }
      else
      {
        rxc_dcch_frame_status = RXC_GOOD_FRAME;
      }
    }

    if( rxc_so.for_fch.included )
    {
      if( buf_ptr->dec_rate == CAI_BLANK_RATE )
      {
        rxc_fch_frame_status = RXC_ERASURE_FRAME;
      }
      else
      {
        rxc_fch_frame_status = RXC_GOOD_FRAME;
      }
    }

    /* Call the function to determine DCCH channel quality */
    if( rxc_so.for_dcch.included )
    {
      if( rxc_sup_dec_int_enable )
      {
        /* indication was already calculated in previous interrupt. */
        buf_ptr->dcch_chan_quality = dcch_chan_quality;
      }
      else
      {
        dcch_chan_quality = buf_ptr->dcch_chan_quality = rxc_dcch_quality_det( curr_rc,
                                                                               rxc_dcch_frame_status );
      }
    }

    /* If rxc_sup_dec_int_enable is TRUE, then this eib/qib reporting
     * has already occurred.  Skip the duplicate reporting for efficiency
     * and correctness, since rxc_flag_eib() and rxc_flag_qib() have
     * current and previous flags.
     */
    if( !rxc_sup_dec_int_enable )
    {
      #ifdef FEATURE_IS2000_REL_A
      if( rxc_so.for_dcch.included )
      {
        qib = ( dcch_chan_quality == RXC_POOR_CHANNEL );
      }
      else /* fch case below*/
      {
        qib = ( rxc_fch_frame_status != RXC_GOOD_FRAME );
      }

      rxc_flag_qib( qib );
      #endif /* FEATURE_IS2000_REL_A */

      if( ( ( rxc_so.for_dcch.included ) &&
            ( rxc_dcch_frame_status == RXC_GOOD_FRAME ) ) ||
          ( ( rxc_so.for_fch.included ) &&
            ( rxc_fch_frame_status == RXC_GOOD_FRAME ) ) )
      {
        rxc_flag_eib( FALSE ) ;
      }
      else
      {
        /* flag to indicate EIB bit pattern to be punctured on reverse link
        ** for FCH since there will be no DTX nothing changes,
        ** for DCCH, ERASURE and DTX frames are to be treated identically.
        */
        rxc_flag_eib( TRUE ) ;
      }
    } /* !rxc_sup_dec_int_enable */

    if( rxc_common_data.rxc_frame_offset_countdown == 0 )
    {
      /* troublesome, sporadic interrupts over, should be settled into
      ** normal 20 ms traffic frames now. Start processing FFPC.
      */

      /* SETPT already updated and set in HW if SCH is enabled.
      ** Update setpoint and write to HW here if only F-FCH/DCCH is on.
      */
      if( !rxc_sup_dec_int_enable )
      {
        if( rxc_so.for_dcch.included )
        {
          ffpc_update_setpt( rxc_dcch_frame_status, FFPC_DCCH );
        }

        if( rxc_so.for_fch.included )
        {
          #ifdef FEATURE_IS2000_1X_ADV
          if( rxc_traffic_status.rxc_rc11.tc_source_mdsp_isr == TRUE )
          {
            if( rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame == TRUE )
            {
              ffpc_update_setpt( rxc_fch_frame_status, FFPC_FCH );
            }
          }
          else
          #endif /* FEATURE_IS2000_1X_ADV */
          {
            ffpc_update_setpt( rxc_fch_frame_status, FFPC_FCH );
          }
        }
      }
    }

    #ifdef FEATURE_IS2000_1X_ADV
    /* FCH RC11 processing */
    if( curr_rc == CAI_RC_11 )
    {
      if( rxc_sup_dec_int_enable )
      {
        /* If supplemental channel is assigned, decoder information is still stored
           in a temp data structure and needs to be copied to the RXC decoder buffer
           to be de-multiplexed later in RXC task context.
        */
        buf_ptr->buf.tc_buf.bits =
          rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_tc_buf.bits;
      }
    }
    else
    #endif /* FEATURE_IS2000_1X_ADV */
    {
      /* If determined rate is not erasure or blank then read the data from
      ** the decoder buffer */
      if ( ( ( buf_ptr->dec_rate != CAI_BLANK_RATE ) &&
             ( buf_ptr->dec_rate != CAI_NULL_RATE  ) )
        #ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
        || ( is_markov_so )
        #endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */
      )
      {
        /* get bits from decoder */
        if( rxc_so.for_dcch.included )
        {
          // Read in DCCH data here.  FCH and DCCH are shared
          dec_read_fch_data( &buf_ptr->buf.tc_buf.bits,
                             (dec_rc_type) curr_rc,
                             DEC_RATE_FULL );
        }
        else  // We have an else here since only one of FCH/DCCH is supported
        {
          // Read in FCH data here
          dec_read_fch_data( &buf_ptr->buf.tc_buf.bits,
                             (dec_rc_type) curr_rc,
                             DEC_RATE_ALL );
        }
      }
    }
    #endif /* FEATURE_IS2000_REL_A_SVD */

    /*
     * NOTE: rxc_update_sch_info() needs to get called before struct rxc_sch
     * is accessed.
     * rxc_sch_update_counter > 0 indicates that a new burst has started.
     * This counter will eqaul 0 when the first SCH interrupt corresponding
     * to the new burst is received, and rxc_sch needs to be updated to reflect
     * the new SCH parameter values.
     */
    #ifdef FEATURE_IS2000_REL_A
    if( ( rxc_turbo_dec_enabled && ( task_id == DEC_TURBO_SCH_TASK ) ) ||
        ( rxc_sup_dec_int_enable && ( task_id == DEC_SCCH_SCH_TASK ) ) )
    #else
    if ((rxc_turbo_dec_enabled || rxc_sup_dec_int_enable) &&
        (task_id == DEC_SCCH_SCH_TASK))
    #endif /* FEATURE_IS2000_REL_A */
    {
       if( rxc_sch_report_data.rxc_sch_update_counter > 0 )
       {
          if( --rxc_sch_report_data.rxc_sch_update_counter == 0 )
          {
            rxc_update_sch_info( &rxc_log_sch_data.rxc_stored_sch_packet );
          }
       }
    }

    if( !rxc_turbo_dec_enabled && !rxc_sup_dec_int_enable )
    {
      rxc_frame_data.rxc_sch_frame_status = RXC_NO_FRAME; // initialize to default
    }

    /* If this is a turbo SCH interrupt */
    #ifdef FEATURE_IS2000_REL_A
    if( rxc_turbo_dec_enabled && ( task_id == DEC_TURBO_SCH_TASK ) )
    #else
    if( rxc_turbo_dec_enabled && ( task_id == DEC_SCCH_SCH_TASK ) )
    #endif /* FEATURE_IS2000_REL_A */
    {
      /****************************************************************
       NOTE: TD_STATUS no longer exists on DIME. Assume that the dummy
             DEINT task that got us here occurred well after the TD
             completed decoding the frame
      ****************************************************************/
      rxc_frame_type frame_type     = RXC_ERASURE_FRAME;
      rxc_frame_type det_frame_type = RXC_ERASURE_FRAME;

      #ifdef FEATURE_IS2000_REL_A /* 32X */
      dec_read_sch_status((dec_mux_pdu_type_type) rxc_sch.mux_pdu_type,
                          (dec_rc_type)rxc_sch.rc, rxc_sch.sch_rate,
                          &rxc_frame_data.rxc_sch_status, DEC_TURBO);
      #else
      dec_read_sch_status((dec_rc_type)rxc_sch.rc, rxc_sch.sch_rate,
                          &rxc_frame_data.rxc_sch_status, DEC_TURBO);
      #endif /* FEATURE_IS2000_REL_A */ /* 32X */

      // Statistics for power measurement report
      if( rxc_pwr.sch_fer_rep[0] == TRUE )
      {
        rxc_ta_total_skip_frame_count_inc(&(rxc_pwr.sch_tot_frames[0]),
                                          &(rxc_pwr.skipped_sch_tot_frames[0]));
      }

      /* DTX only supported for RC3, RC4, RC5 and RC11 */
      #ifdef FEATURE_IS2000_1X_ADV
      if ((rxc_sch.rc==DEC_RC3) || (rxc_sch.rc==DEC_RC4) ||
          (rxc_sch.rc==DEC_RC5) || (rxc_sch.rc==DEC_RC11))
      #else
      if ((rxc_sch.rc==DEC_RC3) || (rxc_sch.rc==DEC_RC4) ||
          (rxc_sch.rc==DEC_RC5))
      #endif /* FEATURE_IS2000_1X_ADV */
      {
        det_frame_type = rxc_dtx_det_sch((dec_rc_type)rxc_sch.rc,
                           rxc_frame_data.rxc_sch_status.turbo_sch_status.crc_pass);
        frame_type = rxc_sch_dtx_filter (det_frame_type);
      }
      else
      {
        frame_type = RXC_DTX_FRAME;
        M1X_MSG( MUX, LEGACY_ERROR,
          "BAD RC in RXC_TC_ISR %d",
          rxc_sch.rc );
      }

      if( frame_type == RXC_GOOD_FRAME )
      {
        /* SCH may be assigned for a voice call.  In this case, don't
        ** read SCH data, otherwise debug printf may be printed every
        ** frame, causing confusion.
        */
        if (
            (rxc_so.pri == CAI_SO_PPP_PKT_DATA_3G) ||
            (rxc_so.sec == CAI_SO_PPP_PKT_DATA_3G) ||
            (rxc_so.pri == CAI_SO_TDSO)            ||
            (rxc_so.sec == CAI_SO_TDSO)            ||
            (rxc_so.pri == CAI_SO_FULL_TDSO)       ||
            (rxc_so.sec == CAI_SO_FULL_TDSO)
           )
        {
          RXC_ENTER_CRITICAL_SECTION();
          /* Copy SCH data into dsm_item array */
          if( rxc_log_sch_data.rxc_dsm_arr_cnt == 0)
          {
            #ifdef FEATURE_IS2000_REL_A /* 32X */
            if (rxc_sch.mux_pdu_type == DEC_MUX_PDU_TYPE_5)
            {
              // 32X turbo support here
              dec_read_mux_pdu5_sch_data(&rxc_sch,
                         &rxc_log_sch_data.rxc_dsm_arr[0],
                         &rxc_log_sch_data.rxc_dsm_arr_cnt,
                         0xFF,
                         &good_ltu
                         );
            }
            else /* MuxPDU type 1, 2, and 3, use old way */
            #endif /* FEATURE_IS2000_REL_A */
            {
              dec_read_sch_data( &rxc_sch, &rxc_log_sch_data.rxc_dsm_arr[0],
                                 &rxc_log_sch_data.rxc_dsm_arr_cnt);
            }

            for (index = 0; index < rxc_log_sch_data.rxc_dsm_arr_cnt; index++)
            {
              rxc_log_sch_data.rxc_dsm_arr[index]->app_field = TRUE;
            }
          }
          else /* dsm array has frames */
          {
            M1X_MSG( MUX, LEGACY_ERROR,
              " RX task not able to process SCH, arr_cnt=%d",
              rxc_log_sch_data.rxc_dsm_arr_cnt);
          }
          RXC_LEAVE_CRITICAL_SECTION();
        }

        #ifdef FEATURE_IS2000_REL_A /* 32X */
        if (rxc_sch.mux_pdu_type == DEC_MUX_PDU_TYPE_5)
        {
          /* MO 0xf20 (MuxPDU 5) has its own set of counters */
          switch(rxc_sch.sch_rate)
          {
            case DEC_SCH_RATE_1X:
              (void) parm_inc(CAI_FSCH0_1_ID, 1);
              break;
            case DEC_SCH_RATE_2X:
              (void) parm_inc(CAI_FSCH0_2_ID, 1);
              break;
            case DEC_SCH_RATE_4X:
              (void) parm_inc(CAI_FSCH0_3_ID, 1);
              break;
            case DEC_SCH_RATE_8X:
              (void) parm_inc(CAI_FSCH0_4_ID, 1);
              break;
            case DEC_SCH_RATE_16X:
              (void) parm_inc(CAI_FSCH0_5_ID, 1);
              break;
            case DEC_SCH_RATE_32X:
              (void) parm_inc(CAI_FSCH0_6_ID, 1);
              break;
          }
        }
        else  /* The statistics for any MO but 0xF20 */
        #endif /* FEATURE_IS2000_REL_A */
        {
        switch(rxc_sch.sch_rate)
        {
          case DEC_SCH_RATE_1X:
              (void) parm_inc(CAI_SCH0_FWD_1X_ID, 1);
            break;
          case DEC_SCH_RATE_2X:
              (void) parm_inc(CAI_SCH0_FWD_2X_ID, 1);
            break;
          case DEC_SCH_RATE_4X:
              (void) parm_inc(CAI_SCH0_FWD_4X_ID, 1);
            break;
          case DEC_SCH_RATE_8X:
              (void) parm_inc(CAI_SCH0_FWD_8X_ID, 1);
            break;
          case DEC_SCH_RATE_16X:
              (void) parm_inc(CAI_SCH0_FWD_16X_ID, 1);
              break;
            default:
              M1X_MSG( MUX, LEGACY_ERROR,
                "Invalid SCH rate %d",
                rxc_sch.sch_rate);
            break;
        }
        }

        rxc_frame_data.rxc_sch_frame_status = RXC_GOOD_FRAME;

        ffpc_update_setpt (rxc_frame_data.rxc_sch_frame_status, FFPC_SCH);
      }
      else if (frame_type == RXC_ERASURE_FRAME)
      {
        // Statistics for power measurement report
        if( rxc_pwr.sch_fer_rep[0] == TRUE )
        {
          rxc_ta_bad_frame_count_inc(&(rxc_pwr.sch_bad_frames[0]));
        }
        /* Indicate that erasure occured on SCH */
        rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_turbo_sch_erasure = TRUE;
        (void) parm_xtra_inc(CAI_XTRA_SCH0_FWD_SDU_ERR_ID, 1);

        /* Increment erasure count to be printed every RXC_SCH_CRC_MAX_CNT frames */
        sch_crc_err++;

        rxc_frame_data.rxc_sch_frame_status = RXC_ERASURE_FRAME;

        ffpc_update_setpt (rxc_frame_data.rxc_sch_frame_status, FFPC_SCH);
      }
      else
      { /* DTX */
        (void) parm_xtra_inc(CAI_XTRA_SCH0_FWD_SDU_DTX_ID, 1);
        /* do nothing for DTX, &rxc_dsm_arr array does not get filled,
        ** we treat DTX like an erasure. */
        rxc_frame_data.rxc_sch_frame_status = RXC_DTX_FRAME;
        sch_dtx_frame_cnt++;

        if (det_frame_type == RXC_ERASURE_FRAME)
        {
           M1X_MSG( MUX, LEGACY_HIGH,
             "Corrected ERASURE -> DTX frame");
        }
      }

      /* Update the F-SCH stats for logging in 0x119D log packet */
      rxc_update_sch_stats( DEC_TURBO );

      if( ++sch_crc_err_dtx_frame_cnt >= RXC_SCH_CRC_MAX_CNT )
      {
        M1X_MSG( MUX, LEGACY_HIGH,
          "Rcvd %d era and %d DTX in %d SCH frames",
          sch_crc_err,
          sch_dtx_frame_cnt,
          sch_crc_err_dtx_frame_cnt);
        sch_crc_err_dtx_frame_cnt = 0;
        sch_crc_err = 0;
        sch_dtx_frame_cnt = 0;
      }

      #ifdef FEATURE_IS2000_REL_A
      if (rxc_frame_data.rxc_sch_frame_status == RXC_GOOD_FRAME )
      {
        rxc_flag_sch_eib( FALSE );
      }
      else
      {
        rxc_flag_sch_eib( TRUE );
      }
      #endif /* FEATURE_IS2000_REL_A */

    }  /* End turbo SCH interrupt */

    /* Convolutional SCH interrupt */
    if( (task_id == DEC_SCCH_SCH_TASK) && (rxc_sup_dec_int_enable) &&
            (! rxc_turbo_dec_enabled) )
    {
      /* Based on the Radio Configuration, we check if it is a SCCH
         or SCH interrupt */
      if( rxc_so.high_rc )  /* SCH Task Interrupt */
      {
        /* Supplemental Channel Interrupt Processing */
        #ifdef FEATURE_IS2000_REL_A /* 32X */
        dec_read_sch_status((dec_mux_pdu_type_type) rxc_sch.mux_pdu_type,
                            (dec_rc_type)rxc_sch.rc, rxc_sch.sch_rate,
                            &rxc_frame_data.rxc_sch_status, DEC_CONVOLUTIONAL);
        #else
        dec_read_sch_status((dec_rc_type)rxc_sch.rc, rxc_sch.sch_rate,
                            &rxc_frame_data.rxc_sch_status, DEC_CONVOLUTIONAL);
        #endif /* FEATURE_IS2000_REL_A */ /* 32X */

        if( rxc_sch.code_type == DEC_CONVOLUTIONAL )
        {
          rxc_frame_type det_frame_type = RXC_ERASURE_FRAME;
          rxc_frame_type frame_type = RXC_ERASURE_FRAME;
          // Statistics for power measurement report
          if( rxc_pwr.sch_fer_rep[0] == TRUE )
          {
            rxc_ta_total_skip_frame_count_inc( &(rxc_pwr.sch_tot_frames[0]),
                                               &(rxc_pwr.skipped_sch_tot_frames[0]) );
          }
          /* DTX supported for RC3, RC4, RC5 and RC 11 */
          #ifdef FEATURE_IS2000_1X_ADV
          if ((rxc_sch.rc==DEC_RC3) || (rxc_sch.rc==DEC_RC4) ||
              (rxc_sch.rc==DEC_RC5) || (rxc_sch.rc==DEC_RC11))
          #else
          if ((rxc_sch.rc==DEC_RC3) || (rxc_sch.rc==DEC_RC4) || (rxc_sch.rc==DEC_RC5))
          #endif /* FEATURE_IS2000_1X_ADV */
          {
            det_frame_type = rxc_dtx_det_sch((dec_rc_type)rxc_sch.rc,
                               rxc_frame_data.rxc_sch_status.conv_sch_status.status & 0x1);
            frame_type = rxc_sch_dtx_filter (det_frame_type);
          }
          else
          {
            frame_type = RXC_DTX_FRAME;
            M1X_MSG( MUX, LEGACY_ERROR,
              "BAD RC in RXC_TC_ISR %d",
              rxc_sch.rc );
          }

          if( frame_type != RXC_DTX_FRAME )
          {
            /* SCH may be assigned for a voice call.  In this case, don't
            ** read SCH data, otherwise debug printf may be printed every
            ** frame, causing confusion.
            */
            if( ( rxc_so.pri == CAI_SO_PPP_PKT_DATA_3G ) ||
                ( rxc_so.sec == CAI_SO_PPP_PKT_DATA_3G ) ||
                ( rxc_so.pri == CAI_SO_TDSO )            ||
                ( rxc_so.sec == CAI_SO_TDSO )            ||
                ( rxc_so.pri == CAI_SO_FULL_TDSO )       ||
                ( rxc_so.sec == CAI_SO_FULL_TDSO ) )
            {
              RXC_ENTER_CRITICAL_SECTION();
              /*-------------------------------------------------------------
                Read the data from the decoder, even if SDU Erasure, as the
                LTU inner-CRC's may allow recovery of some of the LTU's.
              -------------------------------------------------------------*/
              if( rxc_log_sch_data.rxc_dsm_arr_cnt == 0 )
              {
                #ifdef FEATURE_IS2000_REL_A /* 32X */
                if( rxc_sch.mux_pdu_type == DEC_MUX_PDU_TYPE_5 )
                {
                  if( rxc_sch.num_ltu == 0 )
                  {
                    /*
                    ** No LTU case. Either 1X or 2X or turbo. Turbo is not
                    ** processed here.
                    */
                    if( frame_type == RXC_GOOD_FRAME )
                    {
                      /*
                      ** If the frame is bad, don't bother to read data,
                      ** because the MuxPDU header might be corrupted.
                      */
                      dec_read_mux_pdu5_sch_data(&rxc_sch,
                                                 &rxc_log_sch_data.rxc_dsm_arr[0],
                                                 &rxc_log_sch_data.rxc_dsm_arr_cnt,
                                                 0xFF,
                                                 &good_ltu);
                    }
                  }
                  else /* # of LTU is not 0 */
                  {
                    /* get LTU CRC status first */
                    ltu_crc = rxc_frame_data.rxc_sch_status.conv_sch_status.ltu_crc;

                    dec_read_mux_pdu5_sch_data(&rxc_sch,
                                               &rxc_log_sch_data.rxc_dsm_arr[0],
                                               &rxc_log_sch_data.rxc_dsm_arr_cnt,
                                               ltu_crc,
                                               &good_ltu);

                    /* Update Statistics */
                    (void) parm_inc(CAI_FSCH0_LTU_ID, rxc_sch.num_ltu);

                    if (frame_type == RXC_GOOD_FRAME)
                    {
                      ltu_stats.good_sdus++;
                      (void) parm_inc(CAI_FSCH0_LTUOK_ID, rxc_sch.num_ltu);

                      /* Bad LTU within good frame, likely a bad BS */
                      (void) parm_xtra_inc(CAI_XTRA_SCH0_FWD_BAD_MUXPDU5_LTUS_ID,
                                    rxc_sch.num_ltu-good_ltu);
                    }
                    else if (frame_type == RXC_ERASURE_FRAME)
                    {
                      ltu_stats.sdu_erasure++;
                      ltu_stats.recovered_ltus += good_ltu;
                      (void) parm_inc(CAI_FSCH0_LTUOK_ID, good_ltu);
                      (void) parm_xtra_inc(CAI_XTRA_SCH0_FWD_SAVED_MUXPDU5_LTUS_ID,good_ltu);
                    } /* if erasure */
                  } /* if # of LTU is 0 */
                }
                else /* the MuxPDU 1, 2, and 3 case. Do it the old way */
                #endif /* FEATURE_IS2000_REL_A */
                {
                  /* Copy SCH data into dsm_item array */
                  dec_read_sch_data( &rxc_sch,
                                     &rxc_log_sch_data.rxc_dsm_arr[0],
                                     &rxc_log_sch_data.rxc_dsm_arr_cnt);
                  /*-------------------------------------------------------------
                    Set the app fld field to TRUE, to indicate the frame is good.
                    For <= 2X, if the SDU is an erasure, this has no effect.  For
                    > 2X, then this may allow recovery of LTU's if the inner-
                    -CRC's check out.

                    account for single/double size PDU's.
                  -------------------------------------------------------------*/
                  if (rxc_sch.sch_rate <= DEC_SCH_RATE_2X)
                  {
                    if (frame_type == RXC_GOOD_FRAME)
                    {
                      ltu_flag = TRUE;
                    }
                    else
                    {
                      ltu_flag = FALSE;
                    }
                    if ( rxc_log_sch_data.rxc_dsm_arr[0] != NULL )
                    {
                      rxc_log_sch_data.rxc_dsm_arr[0]->app_field = ltu_flag;
                    }
                    if ((rxc_sch.mux_pdu_double == FALSE) &&
                        (rxc_sch.sch_rate == DEC_SCH_RATE_2X) &&
                        (rxc_log_sch_data.rxc_dsm_arr[1] != NULL ) )
                    {
                      rxc_log_sch_data.rxc_dsm_arr[1]->app_field = ltu_flag;
                    }
                  }
                  else /* > 2X */
                  {
                    /*-----------------------------------------------------------
                       quick way to figure out how many ltu's in the frame.
                       Then update the statistics counter.
                    -----------------------------------------------------------*/
                    ltu_cntr = 0x02 << ((byte) rxc_sch.sch_rate - (uint8) DEC_SCH_RATE_4X);
                    (void) parm_inc(CAI_SCH0_FWD_LTU_ID, ltu_cntr);
                    /*-----------------------------------------------------------
                      Shift down the ltu crc bits into the least sig bits
                    -----------------------------------------------------------*/
                    ltu_crc = rxc_frame_data.rxc_sch_status.conv_sch_status.ltu_crc;

                    mask = 0x80 >> (8 - ltu_cntr);
                    /*-----------------------------------------------------------
                      Figure out how many of the LTU's were actually good.  If
                      erasure, then mark the bad LTU's.
                    -----------------------------------------------------------*/
                    if (frame_type == RXC_GOOD_FRAME)
                    {
                      ltu_stats.good_sdus++;
                      (void) parm_inc(CAI_SCH0_FWD_LTUOK_ID, ltu_cntr);
                      ltu_cnt = 0;
                      for (index = 0; index < ltu_cntr; index++)
                      {
                        if ((mask & ltu_crc) == 0)
                        {
                          ltu_stats.incorrect_ltus++;
                          ltu_cnt++;
                        }
                        mask = mask >> 1;
                      }
                      (void) parm_xtra_inc(CAI_XTRA_SCH0_FWD_BAD_LTUS_ID, ltu_cnt);
                      for (index = 0;
                           ((index < rxc_log_sch_data.rxc_dsm_arr_cnt) && (index < SCH_MAX_FWD_PDU));
                           index++)
                      {
                        /*-------------------------------------------------------
                          Go ahead and mark all of these frames as good
                        -------------------------------------------------------*/
                        rxc_log_sch_data.rxc_dsm_arr[index]->app_field = TRUE;
                      } /* for */
                    } /* full frame */
                    else if (frame_type == RXC_ERASURE_FRAME)
                    {
                      ltu_stats.sdu_erasure++;
                      ltu_cnt = 0;
                      for (index = 0; (index < ltu_cntr) &&
                                      (ltu_cntr <= SCH_MAX_FWD_PDU); index++)
                      {
                        /*-------------------------------------------------------
                          Check the bits indicating if the LTU CRC passes.  If it
                          does, mark the PDU as good/bad.  Account for single &
                          double size PDU's.
                        -------------------------------------------------------*/
                        if (((mask >> index) & ltu_crc) != 0)
                        {
                          ltu_flag = TRUE;
                          ltu_stats.recovered_ltus++;
                          ltu_cnt++;
                        }
                        else
                        {
                          ltu_flag = FALSE;
                        }

                        if (rxc_sch.mux_pdu_double == TRUE)
                        {
                          if ( rxc_log_sch_data.rxc_dsm_arr[index] != NULL )
                          {
                            rxc_log_sch_data.rxc_dsm_arr[index]->app_field = ltu_flag;
                          }
                        }
                        else
                        {
                          if(((index * 2) + 1) < SCH_MAX_FWD_PDU) /* prevent buffer overrun */
                          {
                            if ( rxc_log_sch_data.rxc_dsm_arr[index*2] != NULL )
                            {
                              rxc_log_sch_data.rxc_dsm_arr[index * 2]->app_field       = ltu_flag;
                            }
                            if ( rxc_log_sch_data.rxc_dsm_arr[(index*2) + 1] != NULL )
                            {
                              rxc_log_sch_data.rxc_dsm_arr[(index * 2) + 1]->app_field = ltu_flag;
                            }
                          }
                          else
                          {
                            M1X_MSG( MUX, LEGACY_ERROR,
                              "LTU index (%d) exceeds rxc_dsm_arr[] of size %d",
                              ((index*2)+1),
                              SCH_MAX_FWD_PDU);
                            break; /* break out of for loop */
                          }
                        }
                      }/* for */

                      (void) parm_inc(CAI_SCH0_FWD_LTUOK_ID, ltu_cnt);
                      (void) parm_xtra_inc(CAI_XTRA_SCH0_FWD_SAVED_LTUS_ID, ltu_cnt);
                    } /* if erasure */
                  } /* > 2X */
                }
              }
              else
              {
                M1X_MSG( MUX, LEGACY_ERROR,
                  " RX task not able to process SCH, arr_cnt=%d",
                  rxc_log_sch_data.rxc_dsm_arr_cnt );
              }
              RXC_LEAVE_CRITICAL_SECTION();
            } /* pri or sec is 3G */
          } /* != DTX */

          if( frame_type == RXC_GOOD_FRAME )
          {
            #ifdef FEATURE_IS2000_REL_A /* 32X */
            if( rxc_sch.mux_pdu_type == DEC_MUX_PDU_TYPE_5 )
            {
              /* MO 0xf20 (MuxPDU 5) has its own set of counters */
              switch( rxc_sch.sch_rate )
              {
                case DEC_SCH_RATE_1X:
                  (void) parm_inc( CAI_FSCH0_1_ID, 1 );
                  break;
                case DEC_SCH_RATE_2X:
                  (void) parm_inc( CAI_FSCH0_2_ID, 1 );
                  break;
                case DEC_SCH_RATE_4X:
                  (void) parm_inc( CAI_FSCH0_3_ID, 1 );
                  break;
                case DEC_SCH_RATE_8X:
                  (void) parm_inc( CAI_FSCH0_4_ID, 1 );
                  break;
                case DEC_SCH_RATE_16X:
                  (void) parm_inc( CAI_FSCH0_5_ID, 1 );
                  break;
                case DEC_SCH_RATE_32X:
                  (void) parm_inc( CAI_FSCH0_6_ID, 1 );
                  break;
              }
            }
            else    /* The statistics for any MO but 0xF20 */
            #endif /* FEATURE_IS2000_REL_A */
            {
              switch( rxc_sch.sch_rate )
              {
                case DEC_SCH_RATE_1X:
                    (void) parm_inc( CAI_SCH0_FWD_1X_ID, 1 );
                  break;
                case DEC_SCH_RATE_2X:
                    (void) parm_inc( CAI_SCH0_FWD_2X_ID, 1 );
                  break;
                case DEC_SCH_RATE_4X:
                    (void) parm_inc( CAI_SCH0_FWD_4X_ID, 1 );
                  break;
                case DEC_SCH_RATE_8X:
                    (void) parm_inc( CAI_SCH0_FWD_8X_ID, 1 );
                  break;
                case DEC_SCH_RATE_16X:
                    (void) parm_inc( CAI_SCH0_FWD_16X_ID, 1 );
                    break;
                  default:
                    M1X_MSG( MUX, LEGACY_ERROR,
                      "Invalid SCH rate %d",
                      rxc_sch.sch_rate );
                  break;
              } /* switch */
            }

            rxc_frame_data.rxc_sch_frame_status = RXC_GOOD_FRAME;

            ffpc_update_setpt( rxc_frame_data.rxc_sch_frame_status, FFPC_SCH );

          } //full frame
          else if( frame_type == RXC_ERASURE_FRAME )
          {
            // Statistics for power measurement report
            if( rxc_pwr.sch_fer_rep[0] == TRUE )
            {
              rxc_ta_bad_frame_count_inc(&(rxc_pwr.sch_bad_frames[0]));
            }
            (void) parm_xtra_inc(CAI_XTRA_SCH0_FWD_SDU_ERR_ID, 1);

            /* Increment erasure count to be printed every RXC_SCH_CRC_MAX_CNT frames */
            sch_crc_err++;

            rxc_frame_data.rxc_sch_frame_status = RXC_ERASURE_FRAME;

            ffpc_update_setpt( rxc_frame_data.rxc_sch_frame_status, FFPC_SCH );
          }
          else if( frame_type == RXC_DTX_FRAME )
          {
            (void) parm_xtra_inc(CAI_XTRA_SCH0_FWD_SDU_DTX_ID, 1);
            /* do nothing for DTX, &rxc_dsm_arr array does not get filled,
            ** we treat DTX like an erasure. */
            rxc_frame_data.rxc_sch_frame_status = RXC_DTX_FRAME;
            sch_dtx_frame_cnt++;

            if( det_frame_type == RXC_ERASURE_FRAME )
            {
               M1X_MSG( MUX, LEGACY_HIGH,
                 "Corrected ERASURE -> DTX frame");
            }
          }

          /* Update the F-SCH stats for logging in 0x119D log packet */
          rxc_update_sch_stats( DEC_CONVOLUTIONAL );

          if( ++sch_crc_err_dtx_frame_cnt >= RXC_SCH_CRC_MAX_CNT )
          {
            M1X_MSG( MUX, LEGACY_HIGH,
              "Rcvd %d era and %d DTX in %d SCH frames",
              sch_crc_err,
              sch_dtx_frame_cnt,
              sch_crc_err_dtx_frame_cnt);
            sch_crc_err_dtx_frame_cnt = 0;
            sch_crc_err = 0;
            sch_dtx_frame_cnt = 0;
          }

        } //code_type==CONV

        #ifdef FEATURE_IS2000_REL_A
        if (rxc_frame_data.rxc_sch_frame_status == RXC_GOOD_FRAME)
        {
          rxc_flag_sch_eib( FALSE );
        }
        else
        {
          rxc_flag_sch_eib( TRUE );
        }
        #endif /* FEATURE_IS2000_REL_A */
      }
      else
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "No SCCH support!" );
      }
    } /* SCH Interrupt */

    //
    // RXC_SCH_SHUTDOWN
    //
    if( ( rxc_sch_shutdown_state == RXC_SCH_SHUTDOWN_NOW ) &&
        ( !rxc_is_ta_enabled() ) )
    {
      rxc_shutdown_sch();
      rxc_sch_shutdown_state = RXC_SCH_SHUTDOWN_INACTIVE;
      // Report SCH FER if needed to
      if( rxc_sch_report_data.rxc_sch_rep )
      {
        send_for_sch_fer_rpt( rxc_sch_report_data.rxc_sch_id );
        // clear tot_frames and bad_frames
        rxc_pwr.sch_tot_frames[ rxc_sch_report_data.rxc_sch_id ]         = 0;
        rxc_pwr.skipped_sch_tot_frames[ rxc_sch_report_data.rxc_sch_id ] = 0;
        rxc_pwr.sch_bad_frames[ rxc_sch_report_data.rxc_sch_id ]         = 0;

        rxc_sch_report_data.rxc_sch_id = 0;
      }
    }

    buf_ptr->state = RXC_TC_STATE;
      /* mark bits from Traffic Channel */
    qw_equ ( buf_ptr->buf.tc_buf.frame, rxc_dec_frame );

    /* Place item on rxc_dec_q */
    q_put( &rxc_common_data.rxc_dec_q, &buf_ptr->link );

    (void) rex_set_sigs( RX_TCB_PTR, RXC_INT_SIG );
  } /* buf_ptr != NULL */

  /* Write the power control data to the encoder in this context to
   * ensure that the next frame's power control bits contain information
   * about the quality of this frame. */
  txc_write_pcbit_data();

  /* Capture and update the modem power stats in traffic state */
  rxc_get_pwr_stats( curr_rc, &rxc_srch_mux_pwr_stats );
  rxc_tx_update_flush_pwr_log();

  /* perform AGC/power control logging */
  srch_mux_log_agc_pctl(); /* Log AGC and Power Control for Diag */

  /* set frame signal for rxtx */
  (void)rex_set_sigs( RXTX_TCB_PTR, RXTX_FRAME_SIG );

} /* rxc_tc_isr */

/*===========================================================================

FUNCTION RXC_LOG_TC_MSG

DESCRIPTION
  This function logs messages received on the Traffic Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void rxc_log_tc_msg
(
  rxc_dsch_type dsch,           /* Physical channel carrying the message */
  cai_fwd_tc_ext_type *msg_ptr  /* Pointer to received message */
)
{
  log_code_type log_code;
  mux_log_f_tc_msg_type  *log_ptr; /* Pointer to log buffer */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (dsch == RXC_FOR_FCH)
  {
    log_code = LOG_F_TC_MSG_C;
  }
  else if (dsch == RXC_FOR_DCCH)
  {
    log_code = LOG_FOR_DCCH_MSG_C;
  }
  else
  {
    return;
  }

  if (msg_ptr && msg_ptr->length != 0)
  {
    log_ptr = (mux_log_f_tc_msg_type *) log_alloc (log_code,
                                               sizeof(log_hdr_type) +
                                               msg_ptr->length);
  }
  else
  {
    log_ptr = NULL;
  }

  if (log_ptr == NULL)
  {
    rxc_log_msg_drop++;  /* no buffer to log message in */
  }
  else
  {
    /* copy message into buffer */
    /* msg_ptr cannot be NULL because of check for log_ptr above. */
    (void)memscpy( (byte *) log_ptr->msg,
                   msg_ptr->length,
                   (byte *) &(msg_ptr->length),
                   msg_ptr->length );//lint !e613 Possible use of null pointer (msg_ptr) in left argument to operator '->'

    log_commit(log_ptr);
  }

  /* Event report: Message received */
  if (msg_ptr)
  {
  if ( dsch == RXC_FOR_FCH )
  {
    /* F-FCH */
    mclog_report_event_msg_rxed(CAI_F_FCH, msg_ptr->body);
  }
  else
  {
    /* F-DCCH */
    mclog_report_event_msg_rxed(CAI_F_DCCH, msg_ptr->body);
  }
  }
} /* rxc_log_tc_msg */

/*===========================================================================

FUNCTION RXC_TC_PARSE

DESCRIPTION
  This function collects the signaling bits of the Traffic Channel.
  When it determines that an entire Traffic Channel message has been
  received and the message CRC checks, it will place the message on
  a queue for the Layer 2 task and signal the Layer 2 task.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean indicator of whether message data was good.

SIDE EFFECTS
  None.

===========================================================================*/

boolean rxc_tc_parse
(
  qword  frame_num,
  byte   frame[],    /* Address of 1st byte of frame. */
  word   frame_len,   /* Number of bits in frame */
  rxc_dsch_type frame_chan  /* Channel frame received on */
)
{
  word frame_inx;          /* index into traffic channel frame */
  word num_bits;           /* number of bits to copy into message buffer */
  boolean good_data = TRUE;
  /* Boolean indicator of whether message data was good. */
  boolean good_crc;        /* variable for CRC check */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_IS2000_REL_A

  /* Check if the P_REV is in transition and if the action frame has arrived */
  if( rxc_frame_data.rxc_tc_var.in_p_rev_transition )
  {
    /* If action time <= current time, switch P_REV_IN_USE */
    if( qw_cmp( rxc_frame_data.rxc_tc_var.p_rev_action_frame, frame_num ) != 1)
    {
      /* If yes, it is time to update the P_REV_IN_USE now. */
      rxc_frame_data.rxc_tc_var.p_rev_in_use =
        rxc_frame_data.rxc_tc_var.temp_p_rev_in_use;
    }
  }

#endif /* FEATURE_IS2000_REL_A */
  if ( frame[0] & 0x80 )
  {
    /* if you find a start of message bit start HUNT state regardless
        of current state (new message has been found ) */
    rxc_frame_data.rxc_tc_var.state = TC_HUNT_STATE;
  }

  frame_inx = 1;  /* Skip over 1st bit (SOM) */

  while( frame_inx <  frame_len )
  {
    switch( rxc_frame_data.rxc_tc_var.state )
    {
      case TC_HUNT_STATE:
      {
        if( frame[0] & 0x80 )
        {
          /* Start of message bit set */
          rxc_frame_data.rxc_tc_var.msg_inx = 0;
          rxc_frame_data.rxc_tc_var.msg_len = (word)(b_unpackw( frame, frame_inx, 8 ) * 8);

          // For release A, additional checking is reqired because we might not
          // support the full 32K byte message length yet. Therefore, the raw msg
          // length ( including CRC and MSG_LEN fields ) minus 16 bits MSG_LEN
          // fields (worst case scenario) and 16 bits CRC should fit in the
          // buffer with as size of CAI_FWD_TC_EXT_SIZE bytes.
          if ( (rxc_frame_data.rxc_tc_var.msg_len > 32) )
          {
            if( (rxc_frame_data.rxc_tc_var.msg_ptr == NULL ) &&
                ( (rxc_frame_data.rxc_tc_var.msg_ptr =
#ifdef FEATURE_1X_CP_MEM_OPT
                     (rxtx_rx_msg_type *)rxtx_alloc_queue_buf ( RXTX_RX_Q ) ) == NULL ) )
#else /* !FEATURE_1X_CP_MEM_OPT */
                     (rxtx_rx_msg_type *)q_get( &rxtx_rx_free_q) ) == NULL ) )
#endif /* FEATURE_1X_CP_MEM_OPT */
            {

              /* out of buffers -- wait for next start of message */
              M1X_MSG( MUX, LEGACY_ERROR,
                "No buffers on rxtx_rx_free_q" );

              rxc_frame_data.rxc_tc_var.state = TC_FLUSH_STATE;

            }
            else
            {
              b_copy ( frame, frame_inx,
                       (void *)&(rxc_frame_data.rxc_tc_var.msg_ptr->msg.tc.length),
                       rxc_frame_data.rxc_tc_var.msg_inx, 8 );    /* copy length byte */

              rxc_frame_data.rxc_tc_var.msg_inx += 8;
                /* increment index into message buffer */
              frame_inx += 8;
               /* increment index into frame */

            }

            rxc_frame_data.rxc_tc_var.state = TC_MSG_STATE; /* go to message state */
          }
          else
          { /* message length = 0 */
            rxc_frame_data.rxc_tc_var.state = TC_FLUSH_STATE;
            good_data = FALSE;
          }

        }

        else
        { /* Start of message bit not found */
          rxc_frame_data.rxc_tc_var.state = TC_FLUSH_STATE;
          good_data = FALSE;
        }

        break;
      }


      case TC_MSG_STATE:
      {
        num_bits = MIN ( (rxc_frame_data.rxc_tc_var.msg_len - rxc_frame_data.rxc_tc_var.msg_inx),
                         (frame_len - frame_inx) );
        /* calculate number of bits to copy to message buffer */
        b_copy ( frame, frame_inx, &rxc_frame_data.rxc_tc_var.msg_ptr->msg.tc.length,
                   rxc_frame_data.rxc_tc_var.msg_inx, num_bits );

        frame_inx += num_bits;
          /* increment index into frame */

        rxc_frame_data.rxc_tc_var.msg_inx += num_bits;
          /* increment index into message buffer */

        if ( rxc_frame_data.rxc_tc_var.msg_inx >= rxc_frame_data.rxc_tc_var.msg_len )
        {
          /* entire message collected - check CRC */

          /* If rxc_ignore_good_frame is set to TRUE, that means
           * we're about to tune to the TF/CF or we are already on the
           * TF/CF.  If that's the case, we should ignore any incoming
           * signalling message by failing the CRC.  This is to avoid the
           * race condition when during a CFS, the mobile might receive a
           * valid traffic channel message just before it tunes to
           * the CF and start processing the message after it has actually
           * tuned to the CF.  This will break the current software design
           * because the design assumes that the mobile is always on the
           * SF when processing TC messages.  By ignoring the message, the
           * base station is then forced to re-transmit, which might or
           * might not have an impact on performance
           */
          good_crc = (( crc_16_calc( &rxc_frame_data.rxc_tc_var.msg_ptr->msg.tc.length,
                           rxc_frame_data.rxc_tc_var.msg_len ) == CRC_16_OK ) &&
                      ( ! rxc_ignore_good_frame ) );

          if ( good_crc )
          {
            /* Set up message to be sent to Main Control task */
            if(rxc_frame_data.rxc_tc_var.msg_ptr != NULL )
            {
              rxc_frame_data.rxc_tc_var.msg_ptr->length =
                rxc_frame_data.rxc_tc_var.msg_len - CAI_TC_CRC_SIZE - CAI_TC_LGTH_SIZE;

#ifndef FEATURE_1X_CP_MEM_OPT
              rxc_frame_data.rxc_tc_var.msg_ptr->return_q_ptr = &rxtx_rx_free_q;
#endif /* !FEATURE_1X_CP_MEM_OPT */

              rxc_frame_data.rxc_tc_var.msg_ptr->chn = CAIX_TC;
              qw_equ( rxc_frame_data.rxc_tc_var.msg_ptr->msg.tc.frame_num, frame_num );

              /* log message */
              rxc_log_tc_msg( frame_chan, &rxc_frame_data.rxc_tc_var.msg_ptr->msg.tc );

              /* Put message on queue for Layer 2 task */
              q_put( &rxtx_rx_q, &rxc_frame_data.rxc_tc_var.msg_ptr->link );
            }

            /* set signal for Main Control task */
            (void)rex_set_sigs(RXTX_TCB_PTR, RXTX_RX_Q_SIG );

            rxc_frame_data.rxc_tc_var.msg_ptr = NULL;
              /* indicate no working buffer */

          }
          else
          {
            M1X_MSG( MUX, LEGACY_MED,
              "Bad TC MSG crc" );
            good_data = FALSE;

            if(rxc_frame_data.rxc_tc_var.msg_ptr != NULL )
            {
#ifdef FEATURE_1X_CP_MEM_OPT
              /* Put message on free queue */
              RXTX_FREE_QUEUE_BUF( rxc_frame_data.rxc_tc_var.msg_ptr, RXTX_RX_Q );

              /* indicate no working buffer */
#else /* !FEATURE_1X_CP_MEM_OPT */
              /* Put message on free queue */
              q_put( &rxtx_rx_free_q, &rxc_frame_data.rxc_tc_var.msg_ptr->link );

              /* indicate no working buffer */
              rxc_frame_data.rxc_tc_var.msg_ptr = NULL;
#endif /* FEATURE_1X_CP_MEM_OPT */
            }
          }

          rxc_frame_data.rxc_tc_var.state = TC_FLUSH_STATE;

        }

        break;
      }

      case TC_FLUSH_STATE:
      {
        frame_inx = frame_len; /* Indicate all bits used up. */

        rxc_frame_data.rxc_tc_var.state = TC_HUNT_STATE;

        break;
      }

      default:
      {
        ONEX_ERR_FATAL("Invaid TC parse substate",(int) rxc_frame_data.rxc_tc_var.state,0,0);
      }
    } /* switch */
  } /* while */

  return (good_data);
} /* rxc_tc_parse */

/*===========================================================================

FUNCTION PWR_MSR_CHECK

DESCRIPTION
  This procedure processes the updated rxc_pwr structure to determine if a
  power measurement report should be sent to MC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A power measurement report may be sent to MC.

===========================================================================*/
void pwr_msr_check( void )
{
  static boolean tt_msg = FALSE;

  /* Process Power measurement reports */
  if (rxc_pwr.num_delay_frames > 0)
  {
    rxc_pwr.num_delay_frames--;
  }
  else
  {
    /* If Return-If-Fail HHO or CFS is in progress, suspend counting TOT_FRAMES
     * per section 6.6.6.2.8.2 of IS-95B
     */
    if ( rxc_rifhho_cfs_gps_in_progress )
    {
      return;
    }

    /* If we're processing a time transfer HHO we should ignore all received
       frames leading up to the HHO_BEGIN action from MC. */
    if (rxc_fade_data.rxc_pre_tt_ho)
    {
      if(!tt_msg)
      {
        M1X_MSG( MUX, LEGACY_MED,
          "Processing TT, suspending power measurement reporting");
        tt_msg = TRUE;
      }

      return;
    }

    tt_msg = FALSE;

    #ifdef FEATURE_IS2000_REL_A_SVD
    /* For VP2 only, num_frames applies to FCH and dcch_num_frames
    ** applies to DCCH.  This is done for both "DCCH only" and "FCH & DCCH"
    ** scenarios.
    */
    if( rxc_so.for_fch.included )
    {
      #ifdef FEATURE_IS2000_1X_ADV
      /*For 1x Advanced RCs (RC11) check if this is a guaranteed F-FCH
        frame based on the forward blanking duty cycle. We only want to
        use guaranteed frames (indicated by valid_frame == TRUE) when
        counting total frames */
      if ( ( rxc_so.for_fch.rc == CAI_RC_11 )  &&
           ( rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame == FALSE )
         )
      {
        M1X_MSG( MUX, LEGACY_LOW,
          "Not a guaranteed frame for total frame count.");
      }
      else
      #endif /* FEATURE_IS2000_1X_ADV */
      {
        rxc_ta_total_skip_frame_count_inc( &(rxc_pwr.num_frames),
                                           &(rxc_pwr.skipped_num_frames)) ;
      }
    }
    if( rxc_so.for_dcch.included )
    {
      rxc_ta_total_skip_frame_count_inc( &(rxc_pwr.dcch_num_frames),
                                         &(rxc_pwr.skipped_dcch_num_frames) );
    }
    #else /* FEATURE_IS2000_REL_A_SVD */

    #ifdef FEATURE_IS2000_1X_ADV
    /*For 1x Advanced RCs (RC11) check if this is a guaranteed F-FCH
      frame based on the forward blanking duty cycle. We only want to
      use guaranteed frames (indicated by valid_frame == TRUE) when
      counting total frames */
    if ( ( rxc_so.for_fch.rc == CAI_RC_11 )  &&
         ( rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame == FALSE )
       )
    {
      M1X_MSG( MUX, LEGACY_LOW,
        "Not a guaranteed frame for total frame count.");
    }
    else
    #endif /* FEATURE_IS2000_1X_ADV */
    {
      rxc_pwr.num_frames++;
    }
    #endif /* FEATURE_IS2000_REL_A_SVD */

    if ((rxc_pwr.threshold_on) &&
        (
         (rxc_pwr.num_bad_frames >= rxc_pwr.pwr_thresh)
         #ifdef FEATURE_IS2000_REL_A_SVD
         || (rxc_pwr.dcch_num_bad_frames >= rxc_pwr.pwr_thresh)
         #endif /* FEATURE_IS2000_REL_A_SVD */
        )
       )
    {
      /* Send threshold power measurment report to MC */
      send_pwr_rpt();
    }
    else if (
             (rxc_pwr.num_frames == rxc_pwr.pwr_frames)
             #ifdef FEATURE_IS2000_REL_A_SVD
             || (rxc_pwr.dcch_num_frames == rxc_pwr.pwr_frames)
             #endif /* FEATURE_IS2000_REL_A_SVD */
            )
    {
      if( rxc_pwr.periodic_on )
      {
      /* Send periodic power measurment report to MC */
        send_pwr_rpt();
      }
      else
      {
        /* Reset counts but don't send a report */
        rxc_pwr.num_frames              = 0;
        rxc_pwr.skipped_num_frames      = 0;
        rxc_pwr.num_bad_frames          = 0;
        #ifdef FEATURE_IS2000_REL_A_SVD
        rxc_pwr.dcch_num_frames         = 0;
        rxc_pwr.skipped_dcch_num_frames = 0;
        rxc_pwr.dcch_num_bad_frames     = 0;
        #endif /* FEATURE_IS2000_REL_A_SVD */
      }
    }
  }
} /* pwr_msr_check */

/*===========================================================================

FUNCTION RXC_START_TX

DESCRIPTION
  This procedure restarts the transmitter by sending the RXC_TX_ON_R report
  to MC.

DEPENDENCIES
  rxc_sent_bad_rpt should be TRUE when this procedure is called.

RETURN VALUE
  None.

SIDE EFFECTS
  rxc_sent_bad_rpt is set to FALSE after the RXC_TX_ON_R report is sent


===========================================================================*/
void rxc_start_tx( void )
{
  mccrx_rpt_type *rpt_ptr;
    /* pointer to report to be sent to the Main Control task */
  M1X_MSG( MUX, LEGACY_HIGH,
    "RXC asking to turn TX on");

  /* send message to mc to restart transmissions */
  if ( (rpt_ptr = (mccrx_rpt_type *)q_get(&mcc_rx_free_q) ) != NULL )
  {
    rxc_fade_data.rxc_sent_bad_rpt = FALSE;
    rpt_ptr->hdr.rpt = RXC_TX_ON_R;
    rxc_mc_rpt(rpt_ptr );
  }
  else              /* will retry when next good frame arrives */
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "No buffers on mcc_rx_free_q" );
  }
}

/*===========================================================================

FUNCTION FADE_CHECK_GOOD_FRAME_HHO

DESCRIPTION
  This procedure is called upon the receipt of the first two good frames
  during HHO.

  If this is the first good frame after a successful handoff
  (rxc_sent_bad_rpt = TRUE), RXC shall command MC to restart the transmitter.
  In addition, if this is a successful Return-If-Fail HHO, the fade timer
  shall be reset.

  If this is the second consecutive good frame received during handoff,
  RXC shall command MC to restart forward power control and mark the end
  of the HHO.

  Note that consecutive guaranteed frames may also used for checking. In
  legacy RCs all frames are guaranteed to both counters are in sync. In
  1xA RCs (RC11) guaranteed frames follow the smart blanking duty cycle
  and may have 3 or 7 non-guaranteed frames in between that may be blank.
  This seperate counter allows the mobile to perform the correct operations
  based on the guaranteed frames as if they were truly consecutive.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void fade_check_good_frame_hho( void )
{
  mccrx_rpt_type *rpt_ptr;
    /* pointer to report to be sent to the Main Control task */

  if ( ( rxc_fade_data.rxc_good_frames < cai_tmo.n11m ) &&
       ( rxc_fade_data.rxc_good_guaranteed_frames < cai_tmo.n11m ) )
  {
    rxc_fade_data.rxc_t5m_bad_frames++; /* don't reset fade timer yet */
  }
  else
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "Rxed a good frame" );

    /* Event report: Counter N11m threshold reached */
    mclog_report_event_counter(CAI_COUNTER_N11M);

    if( rxc_fade_data.rxc_sent_bad_rpt ) /* Check if RXC_TX_ON_R has already been sent */
    {
      /* rxc_sent_bad_rpt is set to FALSE when we send RXC_TX_ON_R, so
       * by checking this flag, we avoid sending RXC_TX_ON_R more than once.
       */

      /* rxc_rifhho_cfs_gps_in_progress is set to FALSE when we return
       * from a failed hard handoff (if the GHDM allowed return-on-failure),
       * so being here while the flag is still TRUE indicates a successful
       * hard handoff (rifhho).
       */
      if ( rxc_rifhho_cfs_gps_in_progress )
      {
        /* If a Return-If-Fail HHO is successful, reset the fade timer */
        rxc_fade_data.rxc_t5m_bad_frames = 0;
        rxc_rifhho_cfs_gps_in_progress = FALSE;
      }

      /* If we return quickly from a rifhho, we send RXC_TX_ON_R immediately,
       * so rxc_sent_bad_rpt is FALSE even when the first good frame is
       * received after returning. In the case where we take a long time to
       * return, we must wait for 2 consecutive good frames, not just one.
       * So the only time we enable the transmitter here is after a successful
       * handoff.
       */
      if (!rxc_hho_data.rxc_delay_after_hho_failure)
      {
        /* We have successfully completed a handoff. Enable the transmitter. */
        rxc_start_tx();
      }
    }

    if ( (rxc_fade_data.rxc_good_frames >= cai_tmo.n3m) ||
         (rxc_fade_data.rxc_good_guaranteed_frames >= cai_tmo.n3m) )
    {
      /* At least 2 consecutive good frames */
      M1X_MSG( MUX, LEGACY_HIGH,
        "Rxed 2 consecutive good frames" );
      if( !rxc_fade_data.rxc_sent_pwron_rpt )
      {
        /* send message to mc to restart forward power control */
        if ((rpt_ptr = (mccrx_rpt_type *)q_get(&mcc_rx_free_q)) != NULL)
        {
          rxc_fade_data.rxc_sent_pwron_rpt = TRUE;
          rpt_ptr->hdr.rpt = RXC_PWR_ON_R;
          rxc_mc_rpt(rpt_ptr );
        }
        else
        {
          /* We will not re-try to start forward power control reporting. */
          M1X_MSG( MUX, LEGACY_ERROR,
            "No buffers on mcc_rx_free_q" );
        }
      }

      if (rxc_hho_data.rxc_delay_after_hho_failure)
      {
        rxc_start_tx();
        rxc_hho_data.rxc_delay_after_hho_failure = FALSE;
      }

      rxc_fade_data.rxc_fade = FALSE;  /* fade processing ended */
      rxc_hho_data.rxc_hard_handoff_in_progress = FALSE; /* done with HHO processing */
    }
  }
} /* fade_check_good_frame_hho */

/*===========================================================================

FUNCTION FADE_CHECK_GOOD_FRAME_NON_HHO

DESCRIPTION
  This procedure is called upon the receipt of a good frame during fade
  processing.  If this is the second consecutive good frame, RXC shall
  restart the transmitter, reset the fade timer, and mark the end of
  fade processing

  Note that consecutive guaranteed frames may also used for checking. In
  legacy RCs all frames are guaranteed to both counters are in sync. In
  1xA RCs (RC11) guaranteed frames follow the smart blanking duty cycle
  and may have 3 or 7 non-guaranteed frames in between that may be blank.
  This seperate counter allows the mobile to perform the correct operations
  based on the guaranteed frames as if they were truly consecutive.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void fade_check_good_frame_non_hho( void )
{
  if ( ( rxc_fade_data.rxc_good_frames < cai_tmo.n3m ) &&
       ( rxc_fade_data.rxc_good_guaranteed_frames < cai_tmo.n3m ) )
  {
    rxc_fade_data.rxc_t5m_bad_frames++; /* don't reset fade timer yet */
  }
  else /* 2 good frames after bad frames */
  {
    rxc_fade_data.rxc_t5m_bad_frames = 0; /* restart fade timer */
    rxc_fade_data.rxc_fade = FALSE;  /* fade processing ended */
    /* send message to mc to restart transmissions */
    if( rxc_fade_data.rxc_sent_bad_rpt )
    {
      rxc_start_tx();

      /* Event report: Counter N3m threshold reached */
      mclog_report_event_counter(CAI_COUNTER_N3M);
    }
  }
} /* fade_check_good_frame_non_hho */

/*===========================================================================

FUNCTION FADE_CHECK_GOOD_FRAME

DESCRIPTION
  This procedure is called upon the receipt of a good frame.  It determines
  if a HHO is in progress and calls the appropriate procedure.

   Note that consecutive guaranteed frames may also used for checking. In
  legacy RCs all frames are guaranteed to both counters are in sync. In
  1xA RCs (RC11) guaranteed frames follow the smart blanking duty cycle
  and may have 3 or 7 non-guaranteed frames in between that may be blank.
  This seperate counter allows the mobile to perform the correct operations
  based on the guaranteed frames as if they were truly consecutive.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void fade_check_good_frame
(
  boolean is_guaranteed_frame
)
{
  if(is_guaranteed_frame)
  {
    /* got a good *guaranteed* frame */
    rxc_fade_data.rxc_good_guaranteed_frames++;
  }

  /* got a good frame */
  rxc_fade_data.rxc_good_frames++;
  rxc_fade_data.rxc_n2m_bad_frames = 0;

  if ( rxc_hho_data.rxc_hard_handoff_in_progress )
  {
    fade_check_good_frame_hho();
  }
  else /* not in hard handoff */
  {
    fade_check_good_frame_non_hho();
  }
} /* fade_check_good_frame */

/* <EJECT> */
/*===========================================================================

FUNCTION FADE_CHECK_BAD_FRAME

DESCRIPTION
  This procedure is called upon the receipt of a bad frame during fade
  processing.  If this is the N2M-th (12th) consecutive bad frame, RXC
  shall shut down the transmitter

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void fade_check_bad_frame
(
  boolean is_guaranteed_frame
)
{
  mccrx_rpt_type *rpt_ptr;
    /* pointer to report to be sent to the Main Control task */
  word n2m_val = 0;
    /* n2m value (number of bad frames received on FL before TX is
       turned off.
    */

#ifdef FEATURE_IS2000_1X_ADV
  if (((deint_rc_type)rxc_so.for_fch.rc == DEINT_RC11) &&
      ( (byte) rxc_traffic_status.rxc_rc11.rcp.for_n2m_ind < RXC_MAX_N2M_SIZE) &&
      (rxc_traffic_status.rxc_rc11.rcp.for_fch_blanking_dutycycle != CAI_FCH_BLANKING_DISABLED)
     )
  {
    n2m_val = rxc_traffic_status.rxc_rc11_n2m_val[rxc_traffic_status.rxc_rc11.rcp.for_n2m_ind];
  }
  else
#endif /* FEATURE_IS2000_1X_ADV */
  {
    n2m_val = cai_tmo.n2m;
  }

  if(is_guaranteed_frame)
  {
    if( ++rxc_fade_data.rxc_n2m_bad_frames >= n2m_val )
    {
      if( rxc_traffic_status.rxc_tx_in_traffic_on )
      {
        /* Event report: Counter N2m threshold reached */
        mclog_report_event_counter(CAI_COUNTER_N2M);
      }

      /* generate transmit off report to Main Control task */
      if( !rxc_fade_data.rxc_sent_bad_rpt )
      {
        if ( (rpt_ptr = (mccrx_rpt_type *)q_get(&mcc_rx_free_q) ) != NULL )
        {
          rxc_fade_data.rxc_sent_bad_rpt = TRUE;
          rxc_fade_data.rxc_sent_pwron_rpt = FALSE;
          rpt_ptr->hdr.rpt = RXC_TX_OFF_R;
          rxc_mc_rpt(rpt_ptr );
        }
        else
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "No buffers on mcc_rx_free_q" );
        }
      }
    }
  }

  if( ++( rxc_fade_data.rxc_t5m_bad_frames ) >= RXC_TC_TMO_FRAMES )
  {
    /* Event report: Timer T5m expired */
    mclog_report_event_timer(CAI_TIMER_T5M);

    /* Log the primary chain mempool data*/
    srch_mempool_capture();

    /* generate Traffic Channel good frame time-out report*/
    if( (rpt_ptr = (mccrx_rpt_type *)q_get(&mcc_rx_free_q) ) != NULL )
    {
      /* First Free all the TC resources, since in this case we won't
         get the RELEASE_F command from MC */
      M1X_MSG( MUX, LEGACY_HIGH,
        "Sending RXC_TC_TMO_R to CP");

      rpt_ptr->hdr.rpt = RXC_TC_TMO_R;
      rxc_mc_rpt(rpt_ptr );
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "No buffers on mcc_rx_free_q" );
    }
  }
} /* fade_check_bad_frame */

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================

FUNCTION FADE_CHECK_FCH

DESCRIPTION
  This procedure processes the frame rate decision and updates the fade
  good and bad frame counts accordingly to determine if a fade report
  should be sent to MC.  This procedure is called when the primary power
  control channel is FCH.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A fade report may be sent to MC.

===========================================================================*/
void fade_check_fch
(
  byte    mux,
  boolean is_guaranteed_frame
)
{
  boolean fch_frame_check;
  boolean rxc_ta_is_enabled = rxc_is_ta_enabled();

  fch_frame_check = ( (mux == CAI_MUX1_FOR_10_ID ) ||
                      (mux == CAI_MUX1_FOR_9_ID) ||
                      (mux == CAI_MUX2_FOR_26_ID) );

  if( rxc_ta_is_enabled )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
       "1x2GTA: Skipping fade check due to FCH TA" );
    return;
  }

  if( fch_frame_check )
  {
    /* Bad frame received, broke consecutive good frame count */
    rxc_fade_data.rxc_good_frames = 0;

    if( is_guaranteed_frame )
    {
      /* Only clear the guaranteed good frame count if this bad
         frame was itself guaranteed. */
      rxc_fade_data.rxc_good_guaranteed_frames = 0;
    }

    /*
    ** If Return-If-Fail HHO or CFS is in progress, suspend the fade timer per
    ** section 6.6.6.2.8.2 of IS-95B
    */
    if( !rxc_rifhho_cfs_gps_in_progress )
    {
      fade_check_bad_frame( is_guaranteed_frame );
    }
  }
  else
  {
    /* Ignore good frames during CFS, at the beginning of a RIF HHO prior to
     * SRCH checking the failure thresholds, and during the period after
     * a RIF HHO times out, before we return to the serving frequency.
     */
    if( !rxc_ignore_good_frame )
    {
      /* good frame */
      fade_check_good_frame( is_guaranteed_frame );
    }
  }
} /* fade_check_fch */

/*===========================================================================

FUNCTION FADE_CHECK_DCCH

DESCRIPTION
  This procedure processes the frame rate decision and updates the fade
  good and bad frame counts accordingly to determine if a fade report
  should be sent to MC.  This procedure is called when the primary power
  control channel is DCCH.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A fade report may be sent to MC.

===========================================================================*/
void fade_check_dcch
(
  rxc_dcch_quality_type  dcch_chan_quality
)
{
  boolean rxc_ta_is_enabled = rxc_is_ta_enabled();

  if( rxc_ta_is_enabled )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
       "1x2GTA: Skipping fade check due to DCCH TA" );
    return;
  }

  if( dcch_chan_quality == RXC_POOR_CHANNEL )
  {
    /* Bad frame received, broke consecutive good frame count */
    rxc_fade_data.rxc_good_frames = 0;
    rxc_fade_data.rxc_good_guaranteed_frames = 0;

    /*
    ** If Return-If-Fail HHO or CFS is in progress, suspend the fade timer per
    ** section 6.6.6.2.8.2 of IS-95B
    */
    if( !rxc_rifhho_cfs_gps_in_progress )
    {
      fade_check_bad_frame( TRUE );
    }
  }
  else
  {
    /* Ignore good frames during CFS, at the beginning of a RIF HHO prior to
     * SRCH checking the failure thresholds, and during the period after
     * a RIF HHO times out, before we return to the serving frequency.
     */
    if( !rxc_ignore_good_frame )
    {
      /* good frame */
      fade_check_good_frame( TRUE );
    }
  }
} /* fade_check_dcch */
#else /* else of FEATURE_IS2000_REL_A_SVD */
/*===========================================================================

FUNCTION FADE_CHECK

DESCRIPTION
  This procedure processes the frame rate decision and updates the fade
  good and bad frame counts accordingly to determine if a fade report
  should be sent to MC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A fade report may be sent to MC.

===========================================================================*/
void fade_check
(
  byte                  mux,
  boolean               is_guaranteed, /* category of received frame */
  rxc_dcch_quality_type dcch_chan_quality
)
{
  boolean fch_frame_check;
  boolean rxc_ta_is_enabled = rxc_is_ta_enabled();

  fch_frame_check = ( (mux == CAI_MUX1_FOR_10_ID ) ||
                      (mux == CAI_MUX1_FOR_9_ID) ||
                      (mux == CAI_MUX2_FOR_26_ID) );

  if( rxc_ta_is_enabled )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
       "1x2GTA: Skipping fade check due to FCH TA" );
    return;
  }

  if( ( rxc_so.for_fch.included &&
        fch_frame_check ) ||
      ( rxc_so.for_dcch.included &&
        dcch_chan_quality == RXC_POOR_CHANNEL )
    )
  {
    /* Bad frame received, broke consecutive good frame count */
    rxc_fade_data.rxc_good_frames = 0;

    if( is_guaranteed )
    {
      /* Only clear the guaranteed good frame count if this bad
         frame was itself guaranteed. */
      rxc_fade_data.rxc_good_guaranteed_frames = 0;
    }

    /*
    ** If Return-If-Fail HHO or CFS is in progress, suspend the fade timer per
    ** section 6.6.6.2.8.2 of IS-95B
    */
    if( !rxc_rifhho_cfs_gps_in_progress )
    {
      fade_check_bad_frame( is_guaranteed );
    }
  }
  else
  {
    /* Ignore good frames during CFS, at the beginning of a RIF HHO prior to
     * SRCH checking the failure thresholds, and during the period after
     * a RIF HHO times out, before we return to the serving frequency.
     */
    if( !rxc_ignore_good_frame )
    {
      /* good frame */
      fade_check_good_frame( is_guaranteed );
    }
  }
} /* fade_check */
#endif /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================

FUNCTION RXC_TCI_INIT

DESCRIPTION
  This funtion initializes the Traffic Channel initialization state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
rxc_state_type rxc_tci_init
(
  rxc_state_type curr_state,  /* Current state of RXC state machine */
  rxc_cmd_msg_type *cmd_ptr,
  boolean skip_isr_cmd        /* Post RXC_TC_F command or not to the PC ISR */
)
{
  #ifdef FEATURE_IS2000_REL_A_SVD
  word  sr_index;
  #endif /* FEATURE_IS2000_REL_A_SVD */
  rxc_state_type next_state = curr_state;
  #ifndef FEATURE_IS2000_REL_A_SVD
  cai_radio_config_type cur_rc;
  #endif /* !FEATURE_IS2000_REL_A_SVD */

  int err = 0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ASSERT( cmd_ptr );

  switch( curr_state )
  {
    case RXC_PC_STATE:
    {
      #ifndef FEATURE_IS2000_REL_A_SVD
      // Current FCH/DCCH RC, initialize to something to silent compiler
      cur_rc = cmd_ptr->tci.for_fch.rc;
      #endif /* !FEATURE_IS2000_REL_A_SVD */

      rxc_set_frame_offset( cmd_ptr->tci.frame_offset );

      RXC_ENTER_ATOMIC_SECTION();

      /* free decoder data buffer */
      rxc_clear_decoder_queue ();

      rxc_so.enable_skip_tci = cmd_ptr->tci.enable_skip_tci;
      rxc_set_fch_loopback( FALSE );
      /* Support for FEATURE_FTM_DCCH */
      rxc_set_dcch_loopback( FALSE );
      rxc_set_sch_loopback( FALSE );

      #ifdef FEATURE_IS2000_REL_A
      rxc_frame_data.rxc_tc_var.p_rev_in_use = cmd_ptr->tci.p_rev_in_use;

      // Initialize the P_REV transistion flag to FALSE.
      rxc_frame_data.rxc_tc_var.in_p_rev_transition = FALSE;
      #endif /* FEATURE_IS2000_REL_A */

      #ifdef FEATURE_IS2000_REL_A_SVD
      (void) rxc_configure_ffpc_channel( cmd_ptr );
      #else /* FEATURE_IS2000_REL_A_SVD */
      /* DCCH configuration */
      if( ( rxc_so.for_dcch.included = cmd_ptr->tci.for_dcch.included ) != FALSE )
      {
        cur_rc = rxc_so.for_dcch.rc = cmd_ptr->tci.for_dcch.rc;
        rxc_so.high_rc = TRUE;
      }

      /* FCH configuration */
      if( ( rxc_so.for_fch.included = cmd_ptr->tci.for_fch.included ) != FALSE )
      {
        // cur_rc already holds fch rc
        rxc_so.for_fch.rc = cmd_ptr->tci.for_fch.rc;
        rxc_so.high_rc = rxc_so.for_fch.rc > CAI_RC_2;
      }

      rxc_config_demod_for_traffic( cur_rc );
      #endif /* FEATURE_IS2000_REL_A_SVD */

      /* Initialize SCH0 RC */
      rxc_sch.rc = (dec_rc_type)DEINT_RC_INVALID;

      /* For SRCH logging */
      srch_mux_update_rc(
        rxc_so.for_fch.included?
        (deint_rc_type)rxc_so.for_fch.rc:DEINT_RC_INVALID,
        (deint_rc_type)rxc_sch.rc,
        DEINT_RC_INVALID,
        rxc_so.for_dcch.included?
        (deint_rc_type)rxc_so.for_dcch.rc:DEINT_RC_INVALID );

      /* register to MC all RXC action time handlers */
      rxc_register_action_handlers_notifier_interrupt_func();

      #ifdef FEATURE_IS2000_1X_ADV
      /* Save away RXC related data from RC11 Radio Configuration Parameter
         message.  If this message has not been received by MS, CP will
         use default RC parameters and then switch to the non-default values
         after receiving an RCPM.
      */
      rxc_traffic_status.rxc_rc11.rcp = cmd_ptr->tci.rcp_init;

      /* Register for the LOG BUFFER RDY message to be able to recieve
         the fw logging ready response message */
      muxmsgr_register_msg( RX_TASK, CDMA_FW_IND_ADV1X_LOG_BUFFER_RDY );
      #endif /* FEATURE_IS2000_1X_ADV */

      /* send command to interrupt handler to go to TC mode
         For time transfer cases, since TC is setup directly and there is no
         ISR enabled currently, there is no need to post RXC_TC_F to ISR */
      if( !skip_isr_cmd )
      {
        rxc_common_data.rxc_isr_cmd_buf_ptr = (rxc_isr_cmd_type *)
                                  q_get (&rxc_common_data.rxc_isr_free_q);
        if( rxc_common_data.rxc_isr_cmd_buf_ptr != NULL )
        {
          rxc_common_data.rxc_isr_cmd_buf_ptr->cmd = RXC_TC_F;
          rxc_isr_cmd( rxc_common_data.rxc_isr_cmd_buf_ptr );
        }
        else
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "No buffers on rxc_isr_free_q" );
        }
      }

      /* Default service options to none */
      rxc_so.pri = CAI_SO_NULL;
      rxc_so.sec = CAI_SO_NULL;

      #ifdef FEATURE_IS2000_REL_A_SVD
      /* reset the service option reference */
      for( sr_index = 0; sr_index < RXC_SR_ID_MAX; sr_index++ )
      {
        rxc_srid_ref[sr_index].so           = CAI_SO_NULL;
        rxc_srid_ref[sr_index].traffic_type = CAI_NUL_TRAF;
      }
      #endif /* FEATURE_IS2000_REL_A_SVD */

      /* Initialize the vocoder interface module */
      muxvoc_rx_tc_init( cmd_ptr->tci.inhibit_voc_enable );

      /* Indicate that vocoder is not initialized */
      rxc_set_voc_config_done( FALSE );
      rxc_set_mvs_status_on_tc_entry( FALSE );

      /* Re-initialize Markov log buffer */
      rxc_bufs_qs_data.rxc_log_mar_buf.count = 0;
      rxc_bufs_qs_data.rxc_log_mar_buf.hdr.len = FPOS(mux_log_mar_type, entry);

      rxcrda_data.rxcrda_dcch_dtx_fix_counter = -1;

      rxc_fade_data.rxc_good_frames = 0;
      rxc_fade_data.rxc_good_guaranteed_frames = 0;
      rxc_fade_data.rxc_t5m_bad_frames = 0;
      rxc_fade_data.rxc_n2m_bad_frames = 0;

      /* initialize to NOT treat blank and burst to erasures */
      rxc_frame_data.rxc_erase = FALSE;

      /* initialize ser totals */
      rxc_ser_reset();

      /* initialize fer/ser stats totals */
      (void) rxc_get_fer_ser_report( NULL, TRUE );

      /* Turn power control reporting off until told otherwise */
      rxc_pwr.threshold_on = FALSE;
      rxc_pwr.periodic_on  = FALSE;

      /* Retry control flag when mcc_rx_free_q is full */
      rxc_fade_data.rxc_sent_valid_rpt = FALSE;

      /*Register action timer*/
      err = m1x_stmr_register( M1X_STMR_CLIENT_RX, M1X_STMR_PCG_4, rxc_atmr_isr );

      if( err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_REG_DUP )
      {
        ONEX_ERR_FATAL( "Unable to schedule the event with err: %d", err, 0, 0 );
      }

      memset( &ltu_stats, 0, sizeof(ltu_stats_type) );

      /* program the delay of TX generated MASK_DATA to RX PCG timing */
      muxmdsp_set_agc_ctl_mask_delay( cmd_ptr->tci.rev_pwr_cntl_delay );

      ffpc_tci_cmd( cmd_ptr );
      if( cmd_ptr->tci.for_dcch.included )
      {
        /* call the function to initialize DCCH quality threshole value */
        rxcrda_dcch_init();
      }

      cmd_ptr->hdr.status = RXC_DONE_S;
      next_state          = RXC_TCI_STATE;

      #ifndef FEATURE_IS2000_REL_A_SVD
      if( hw_version() >= MSM_50_A0 )
      {
        if( cmd_ptr->tci.for_dcch.included && cmd_ptr->tci.for_fch.included )
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "FCH and DCCH not allowed on MSM5000" );
          cmd_ptr->hdr.status = RXC_BADPARM_S;
        }
      }
      #endif /* !FEATURE_IS2000_REL_A_SVD */

      RXC_LEAVE_ATOMIC_SECTION();
      rxc_pwr.sch_tot_frames[0]         = 0;
      rxc_pwr.skipped_sch_tot_frames[0] = 0;
      rxc_pwr.sch_bad_frames[0]         = 0;
      rxc_pwr.sch_fer_rep[0]            = FALSE;
      rxc_pwr.sch_tot_frames[1]         = 0;
      rxc_pwr.skipped_sch_tot_frames[1] = 0;
      rxc_pwr.sch_bad_frames[1]         = 0;
      rxc_pwr.sch_fer_rep[1]            = FALSE;

      #ifdef FEATURE_IS2000_1X_ADV
      /* For 1x Advanced DCCH is not supported R-FCH.
        If it is included at the same time that R-FCH is then we must
        confirm that R-FCH RC and R-DCCH are RC11.
      */
      if( cmd_ptr->tci.for_dcch.included )
      {
        if( ( cmd_ptr->tci.for_dcch.rc == CAI_RC_11 ) ||
            ( ( cmd_ptr->tci.for_fch.included ) &&
              ( cmd_ptr->tci.for_fch.rc == CAI_RC_11 ) ) )
        {
          M1X_MSG( MUX, LEGACY_FATAL,
            "Illegal channel configuration: FCH-> inc=%d, rc=%d DCCH-> rc=%d",
            cmd_ptr->tci.for_fch.included,
            cmd_ptr->tci.for_fch.rc,
            cmd_ptr->tci.for_dcch.rc );
        }
      }

      if( ( cmd_ptr->tci.for_fch.included ) &&
          ( cmd_ptr->tci.for_fch.rc == CAI_RC_11 ) )
      {
        /* Notify SRCH that we are entering 1x Adv traffic */
        srch_mux_notify_1x_adv_traffic( TRUE );
      }
      #endif /* FEATURE_IS2000_1X_ADV */

      break;
    }

    default:
    {
      ONEX_ERR_FATAL( "Bad state %d", (int) curr_state, 0, 0 );
    }
  } /* switch */

  return next_state;

} /* rxc_tci_init */

/*===========================================================================

FUNCTION RXC_TC_INIT

DESCRIPTION
  This funtion initializes the Traffic Channel processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_tc_init
(
  rxc_state_type curr_state   /* Current state of RXC state machine */
)
{
  uint32 i = 0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch( curr_state )
  {
    case RXC_TCI_STATE:
    {
      /* interrupt handler is already in Traffic Channel mode */
      /* set up variable for Traffic Channel message parsing state machine */
      rxc_frame_data.rxc_tc_var.state           = TC_HUNT_STATE;
      rxc_frame_data.rxc_tc_var.msg_ptr         = NULL;

      /* Initialize frame counts for reception watchdog */
      rxc_fade_data.rxc_t5m_bad_frames          = 0;
      rxc_fade_data.rxc_n2m_bad_frames          = 0;

      rxc_fade_data.rxc_good_frames             = 0;
      rxc_fade_data.rxc_good_guaranteed_frames  = 0;

      rxc_fade_data.rxc_fade                    = FALSE;
      rxc_fade_data.rxc_sent_bad_rpt            = FALSE;
      rxc_fade_data.rxc_sent_pwron_rpt          = FALSE;      /* used in HHO */

      rxcrda_data.rxcrda_dcch_dtx_fix_counter   = -1;

      rxc_hho_data.rxc_eib_flags.previous       = FALSE;
      rxc_hho_data.rxc_eib_flags.current        = FALSE;
      rxc_rifhho_cfs_gps_in_progress            = FALSE;
      rxc_cfs_tuneback_count                    = 0;

      rxc_hho_data.rxc_delay_after_hho_failure  = FALSE;
      rxc_ignore_good_frame                     = FALSE;

      rxc_hho_data.rxc_gps_state                = RXC_GPS_STATE_NONE;

      /* No hard handoff in progress */
      rxc_hho_data.rxc_hard_handoff_in_progress = FALSE;


      rxc_init_averaged_fer(); /* Initialize averaged FER counters */

      /* Initialize channel information used in RLP 3 payload logging. */
      memset( &rxc_bufs_qs_data.rxc_rlp_logging_chan_info, 0,
              sizeof(rxc_bufs_qs_data.rxc_rlp_logging_chan_info) );

      /* For 0x1877 logging, RX/TX logging adjustments init */
      for( i = 0; i < LOGGING_FRAME_OFFSET_SB_DTX; i++ )
      {
        rxc_tx_sb_dtx_param.sb_decision[i] = 0;
        rxc_tx_sb_dtx_param.dtx_mask[i]    = 0xFFFF;
      }

      /* Clear the contents of logging buffer structure on traffic entry */
      /* Zero initialize the members of the structure */
      for ( i = 0; i < RX_TX_ONE_FRAME_OFFSET_CORRECTION; i++ )
      {
        log_buff.sb_decision[i]   = 0;
        log_buff.dtx_decision[i]  = 0;
        log_buff.tx_frame_rate[i] = 0;

        if ( log_buff.mdsp_buf[i] == NULL )
        {
          log_buff.mdsp_buf[i] =
            modem_mem_calloc( MDSP_BUFF_SIZE, sizeof(uint32),
                              MODEM_MEM_CLIENT_1X );
          if ( log_buff.mdsp_buf[i] == NULL )
          {
            M1X_MSG( MUX, LEGACY_HIGH,
                 "Unable to allocate memory - skip 0x14ED logging" );
            break;
          }
        }
      }

      /* Zero initialize the members of the structure */
      for( i = 0; i < RX_TX_TWO_FRAME_OFFSET_CORRECTION; i++ )
      {
        log_buff.critical_frame[i]    = 0;
        log_buff.sch_state[i]         = 0;
      }
      memory_is_allocated_for_logging = TRUE;
      break;
    }

    default:
    {
      ONEX_ERR_FATAL( "Bad state %d", (int) curr_state, 0, 0 );
    }
  }/* switch */

} /* rxc_tc_init */

#ifdef FEATURE_IS2000_REL_A
/*===========================================================================

FUNCTION RXCTRAFFIC_FRAME_BND_INIT

DESCRIPTION
  This funtion initializes the Traffic Channel processing state at the
  frame boundary.  Note this is called in ISR context.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxctraffic_frame_bnd_init( void )
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* switch to Traffic Channel and use initial rate set */
  #ifdef FEATURE_IS2000_REL_A_SVD
  if( rxc_so.for_dcch.included )
  {
    rxc_set_dcch_mode( (deint_rc_type) rxc_so.for_dcch.rc,
                       ( ffpc_channel == CAI_FPC_PRI_CHAN_DCCH ) );
  }
  if( rxc_so.for_fch.included )
  {
    rxc_set_fch_mode( (deint_rc_type) rxc_so.for_fch.rc,
                      ( ffpc_channel == CAI_FPC_PRI_CHAN_FCH ) );
  }
  #else
  if( rxc_so.for_dcch.included )
  {
    // Note: for MSM5000, FCH and DCCH shares the same hardware
    deint_set_fch_mode( rxc_so.for_dcch.rc );
  }
  if( rxc_so.for_fch.included )
  {
    deint_set_fch_mode( rxc_so.for_fch.rc );
  }
  #endif /* FEATURE_IS2000_REL_A_SVD */

  /* get frame number and subtract 1 since the decoder output buffer will
   * contain curr_frame_time-1 frame data in packet mode. */
  m1x_time_get_20ms_frame_time( rxc_dec_frame );
  qw_dec( rxc_dec_frame, 1L );

  #ifdef FEATURE_IS2000_1X_ADV
  /* Check if this is a 1xA call and set clocks accordingly. */
  if( rxc_so.for_fch.rc == CAI_RC_11 )
  {
    dec_config_clk_resources( DEC_CLK_VOICE_TRAFFIC );
  }
  else
  {
    dec_config_clk_resources( DEC_CLK_IDLE_CSFB );
  }
  #endif /* FEATURE_IS2000_1X_ADV */

  #ifdef FEATURE_IS2000_1X_ADV
  if( rxc_so.for_fch.included )
  {
    /* Select the appropriate ISR for frame processing depending on the FCH RC. */
    rxc_tc_isr_select();
  }
  #endif /* FEATURE_IS2000_1X_ADV */

  RXC_ENTER_ATOMIC_SECTION();
  /* For RC11, setting the decoder ISR here prepares for SCH decoder interrupt if
     SCH is assigned.  Since FCH deinterleaver task list is not set, this operation
     has no impact on FCH.
  */
  rxc_register_isr( (dalint_1x_isr) rxc_tc_isr, DALINT_1X_STACK_4K );

  /* Clear DEC DONE int status to make sure ISR is properly serviced. */
  SET_DEMBACK_REG( DEC_CLR_INT_STATUS,
                   HWIO_FMSK( DEC_CLR_INT_STATUS, VD_CLR_STATUS ) );

  RXC_LEAVE_ATOMIC_SECTION();

} /* rxctraffic_frame_bnd_init */
#endif /* FEATURE_IS2000_REL_A */

#ifdef FEATURE_MODEM_1X_DRX

/*===========================================================================
FUNCTION       RXC_DRX_DISABLE_FPC_MODE

DESCRIPTION    This function checks if DRX feature needs to be enabled
               or disabled based on FPC mode in use.

DEPENDENCIES   None

RETURN VALUE   TRUE  : DRX disable
               FALSE : DRX can be enabled.

SIDE EFFECTS   None

===========================================================================*/
static boolean rxc_drx_disable_fpc_mode
(
  fpc_mode_type fpc_mode
)
{
  boolean drx_fpc_disable = FALSE;
  rxc_fpc_mode_changed = FALSE;

  if( ( fpc_mode == FPC_MODE_PRI_200Hz_SEC_600Hz ) ||
      ( fpc_mode == FPC_MODE_1XA_PRI_200Hz ) ||
      ( fpc_mode == FPC_MODE_1XA_PRI_200Hz_SEC_200Hz ) )
  {
    drx_fpc_disable = TRUE;

    M1X_MSG( MUX, LEGACY_LOW,
      "Dont enable DRX FPC mode %d",
      fpc_mode );
  }

  return( drx_fpc_disable );

} /* rxc_drx_disable_fpc_mode */

/*===========================================================================

FUNCTION RXC_MUX_DRX_ENABLE

DESCRIPTION
  This function is called to enable or disable DRX.
  This function sends enable_drx/disable_drx messages to FW.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void rxc_mux_drx_enable
(
  boolean mux_drx_enable
)
{
  static boolean drx_enabled          = FALSE;
  boolean        drx_state_is_updated = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  RXC_ENTER_ISR_SECTION();
  /* Enable DRX if its already disabled and Service Connect is received and
     SRCH is capable of DRXing (means RxDIV is disabled). 1x FL DRX is
     applicable only for RC3 or RC11 and non-data(voice) calls */
  if( ( mux_drx_enable )                     &&
      ( !drx_enabled )                       &&
      ( rxc_serv_conn_rxed )                 &&
      ( ( rxc_so.for_fch.rc == CAI_RC_3 ) ||
        ( rxc_so.for_fch.rc == CAI_RC_11 ) ) &&
      ( !rxc_is_data_call() )                &&
      ( !txc_get_frame_off_in_progress() )   &&
      ( rxc_get_mvs_status_on_tc_entry() )   &&
      ( !rxc_drx_disable_fpc_mode( txc_so.fpc_mode ) ) )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "DRX: Enable = %d, Already Enabled = %d, Service Connection = %d",
      mux_drx_enable, 
      drx_enabled, 
      rxc_serv_conn_rxed );

    drx_enabled          = TRUE;
    drx_state_is_updated = TRUE;

    /* Send DRX enable msg to FW and wait for response */
    muxmdsp_update_drx_config( TRUE );

    /* Stop srch if active */
    srch_mux_change_drx_state( TRUE );
  }
  else if( !mux_drx_enable && drx_enabled )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "DRX: Enable = %d, Already Enabled = %d, Service Connection = %d",
      mux_drx_enable, 
      drx_enabled, 
      rxc_serv_conn_rxed );

    drx_enabled          = FALSE;
    drx_state_is_updated = TRUE;

    /* Send DRX disable msg to FW and wait for response */
    muxmdsp_update_drx_config( FALSE );

    /* Stop srch if active */
    srch_mux_change_drx_state( FALSE );
  }
  RXC_LEAVE_ISR_SECTION();

  if( drx_state_is_updated )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
       "DRX: Waiting for SRCH Abort response" );

    if( SRCH_TCB_PTR != rex_self() )
    {
      /* Wait for SRCH ABORT to be completed */
      (void) rex_clr_sigs( RX_TCB_PTR, RXC_SRCH_ABORT_SIG );
      rxc_wait( RXC_SRCH_ABORT_SIG );
      (void) rex_clr_sigs( RX_TCB_PTR, RXC_SRCH_ABORT_SIG );
    }

    M1X_MSG( MUX, LEGACY_HIGH,
       "DRX: Received SRCH Abort response" );
  }

} /* rxc_mux_drx_enable */
#endif /* FEATURE_MODEM_1X_DRX */

/*===========================================================================

FUNCTION RXC_TCI

DESCRIPTION
  Traffic Channel Initialization processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/
rxc_state_type rxc_tci( void )
{
  rxc_state_type next_state;
    /* next state to be processed */
  rxc_cmd_msg_type *cmd_ptr;
    /* pointer to command buffer received on rxc_cmd_q */
  rxc_dec_type *buf_ptr;
  cai_data_rate_type dec_rate;  /* rate returned by rxc_dec_rate_sel */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - -*/

  /* Clear F-SCH stats for 0x119D while entering traffic */
  memset(&rxc_fsch_stats,0,sizeof(rxc_fsch_stats));

  next_state = RXC_TCI_STATE;
  while( next_state == RXC_TCI_STATE )
  {
    (void) rex_clr_sigs( RX_TCB_PTR, RXC_INT_SIG );

    if( ( buf_ptr = (rxc_dec_type *)q_get( &rxc_common_data.rxc_dec_q ) ) )
    {
      if( buf_ptr->state == RXC_TC_STATE )
      {
        /* Variable holds result from test for a bad frame */
        boolean bad_frame_test;
        /* the dec_rate had been computed at the rxc_tc_isr() level */
        dec_rate = buf_ptr->dec_rate;
        bad_frame_test = ( dec_rate == CAI_BLANK_RATE );

        /* bad_frame_test is (rate == ERASURE) for FCH or
           (dcch_quality == POOR) for DCCH */
        bad_frame_test = ( rxc_so.for_fch.included && bad_frame_test ) ||
                         ( rxc_so.for_dcch.included &&
                         ( buf_ptr->dcch_chan_quality == RXC_POOR_CHANNEL ) );

        if( bad_frame_test &&
            ( ( !onex_nonsig_is_ftm_mode() ) || !rxc_so.enable_skip_tci ) )
        {
          /* category 9 or 10 frame */
          rxc_fade_data.rxc_t5m_bad_frames = 0;
          rxc_fade_data.rxc_n2m_bad_frames = 0;
          rxc_fade_data.rxc_good_frames    = 0;

          #ifdef FEATURE_IS2000_1X_ADV
          if( ( rxc_so.for_fch.rc == CAI_RC_11 ) &&
              ( rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame ) )
          #endif /* FEATURE_IS2000_1X_ADV */
          {
            rxc_fade_data.rxc_good_guaranteed_frames = 0;
          }

          /* clear the valid rpt flag, get ready for the next rpt */
          rxc_fade_data.rxc_sent_valid_rpt = FALSE;
        }
        else
        {
          #ifdef FEATURE_IS2000_1X_ADV
          if( ( rxc_so.for_fch.rc == CAI_RC_11 ) &&
              ( rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame ) )
          #endif /* FEATURE_IS2000_1X_ADV */
          {
            /* In legacy RCs all frames are "guaranteed" but in RC11 (1xAdv)
               if smart blanking is enabled guaranteed frames depend on the
               duty cycle. In that case we either want 2 consecutive good
               frames OR two consecutive guaranteed frames to meet the
               standard.
            */
            rxc_fade_data.rxc_good_guaranteed_frames++;
          }

          if (((++rxc_fade_data.rxc_good_frames >= cai_tmo.n5m) ||
               ( rxc_fade_data.rxc_good_guaranteed_frames >= cai_tmo.n5m ) ||
               (onex_nonsig_is_ftm_mode() && rxc_so.enable_skip_tci)) &&
              (!rxc_fade_data.rxc_sent_valid_rpt))
          {
            M1X_MSG( MUX, LEGACY_MED,
              "Send 2 valid frame rpt from rxctci()" );

            rxc_send_two_valid_frame_rpt ();
          }
        }
      }
      q_put( &rxc_common_data.rxc_dec_free_q, &buf_ptr->link );
    }
    else
    {
      /* check for received commands */
      (void) rex_clr_sigs( rex_self(), RXC_CMD_Q_SIG );

      if( ( cmd_ptr = (rxc_cmd_msg_type *)q_get( &rxc_common_data.rxc_cmd_q ) ) )
      {
        switch( cmd_ptr->hdr.command )
        {
          case RXC_CDMA_F:
            M1X_MSG( MUX, LEGACY_MED,
              "Got RXC_CDMA_F" );

            /* --------------------------------------------------
            ** Transition back to CDMA init state without exiting
            ** rxc_subtask()
            ** -------------------------------------------------- */

            /* --------------------------------------
            ** Clean up before switching to CDMA mode
            ** -------------------------------------- */
            rxc_exit();

            rxc_set_frame_offset(0);

            cmd_ptr->hdr.status = RXC_DONE_S;
            next_state          = RXC_CDMA_STATE;
            break;

          case RXC_SC_F:
          {
            M1X_MSG( MUX, LEGACY_MED,
              "Got RXC_SC_F" );

            /* initialize Sync channel state */
            rxc_sc_init( RXC_PC_STATE );
            cmd_ptr->hdr.status = RXC_DONE_S;
            next_state          = RXC_SC_STATE;
            break;
          }/* RXC_SC_F */

          case RXC_EXIT_F:
          {
            M1X_MSG( MUX, LEGACY_MED,
              "Got RXC_EXIT_F" );

            cmd_ptr->hdr.status = RXC_DONE_S;
            next_state          = RXC_EXIT_STATE;
            break;
          }

          case RXC_TC_F:
          {
            M1X_MSG( MUX, LEGACY_MED,
              "Got RXC_TC_F" );

            /* make sure all items are placed back on the rxtx_rx_free_q */
            if( rxc_frame_data.rxc_tc_var.msg_ptr )
            {
              #ifdef FEATURE_1X_CP_MEM_OPT
              RXTX_FREE_QUEUE_BUF( rxc_frame_data.rxc_tc_var.msg_ptr, RXTX_RX_Q );
              #else /* !FEATURE_1X_CP_MEM_OPT */
              q_put(&rxtx_rx_free_q, &rxc_frame_data.rxc_tc_var.msg_ptr->link );
              rxc_frame_data.rxc_tc_var.msg_ptr = NULL;
              #endif /* FEATURE_1X_CP_MEM_OPT */
            }

            /* initialize for traffic channel */
            rxc_tc_init( RXC_TCI_STATE );
            cmd_ptr->hdr.status = RXC_DONE_S;
            next_state          = RXC_TC_STATE;
            break;
          }/* RXC_TC_F */

          case RXC_TCI_F:
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_TCI_F");

          if( onex_nonsig_is_ftm_mode() )
          {
            cmd_ptr->hdr.status = RXC_DONE_S;
            break;
          }
          /* For DMSS, fall through */

          default:
          {
            M1X_MSG( MUX, LEGACY_ERROR,
              "Illegal RX command %d",
              cmd_ptr->hdr.command );
            cmd_ptr->hdr.status = RXC_BADCMD_S;
          }
        } /* switch */

        /* return item to queue if requested */
        cmd_done( &cmd_ptr->hdr.cmd_hdr );

      }
      else
      {
        /* Wait for a command */
        (void) rxc_wait( RXC_CMD_Q_SIG | RXC_INT_SIG );
      }
    }

  } /* while */

  if( next_state != RXC_TC_STATE )
  {
    /* Traffic Channel exit clean-up */
    rxc_free_tc_resources();

    /* Reset active service options to none active */
    rxc_so.pri = CAI_SO_NULL;
    rxc_so.sec = CAI_SO_NULL;

    /* Reset RCs to Init RC */
    rxc_so.for_fch.rc  = CAI_INIT_RC;
    rxc_so.for_dcch.rc = CAI_INIT_RC;
  }

  return next_state;

} /* rxc_tci */

/*===========================================================================

FUNCTION RXC_TA_RIF_LOST_FRAMES_UPDATE

DESCRIPTION
  Adjust lost frames for RIF during TA GAP

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
static void rxc_ta_rif_lost_frames_update( word skipped_frames )
{
  if( rxc_rifhho_cfs_gps_in_progress )
  {
    /* Update lost frames with skipped frame count in case of RIF HHO*/
    rxc_hho_data.rxc_lost_frames += skipped_frames;

    /* If lost frame count exceeds time out limit, then update time out limit to next frame to send
    RXC_HHO_TIMEOUT_R report to 1x CP*/
    if (rxc_hho_data.rxc_lost_frames >= rxc_hho_data.rxc_rif_hho_timeout_frames )
    {
      rxc_hho_data.rxc_rif_hho_timeout_frames = rxc_hho_data.rxc_lost_frames+1;
    }
  }

} /* rxc_ta_rif_lost_frames_update */

/*===========================================================================

FUNCTION RXC_TA_FADE_TIMER_CB

DESCRIPTION
  Callback to handle T5m timer expiry during LTA GAP

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void rxc_ta_fade_timer_cb( uint32 timer_id )
{
  MODEM_1X_UNUSED( timer_id );

  mccrx_rpt_type *rpt_ptr;

  /* Event report: Timer T5m expired */
  mclog_report_event_timer( CAI_TIMER_T5M );

  /* Log the primary chain mempool data*/
  srch_mempool_capture();

  /* generate Traffic Channel good frame time-out report*/
  if( ( rpt_ptr = (mccrx_rpt_type *)q_get( &mcc_rx_free_q ) ) != NULL )
  {
    /* First Free all the TC resources, since in this case we won't
       get the RELEASE_F command from MC */
    M1X_MSG( MUX, LEGACY_HIGH,
      "1x2GTA: Sending RXC_TC_TMO_R to CP" );

    rpt_ptr->hdr.rpt = RXC_TC_TMO_R;
    rxc_mc_rpt(rpt_ptr );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "No buffers on mcc_rx_free_q" );
  }

}/* rxc_ta_fade_timer_cb */

/*===========================================================================

FUNCTION RXC_TC

DESCRIPTION
  Traffic Channel processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/
rxc_state_type rxc_tc( void )
{
  rxc_state_type next_state;
    /* Next state to be processed */
  rxc_cmd_msg_type *cmd_ptr;
    /* Pointer to command buffer received on rxc_cmd_q */
  rex_sigs_type sigs = 0;
  #ifdef FEATURE_IS2000_REL_B
  boolean free_tc_resources = FALSE;
    /* Indicate if traffic channel resources have been freed */
  #endif /* FEATURE_IS2000_REL_B */
  int err = 0;

  boolean rxc_mvs_standby_progress_status         = FALSE;
  mc_mvs_standby_enum_type rxc_mvs_standby_status = MVS_STANDBY_FAIL;
  boolean rxc_mvs_tc_entry_enabled                = FALSE;
  boolean rxc_mvs_voc_config_done                 = FALSE;
  boolean rxc_mvs_is_so_change_status             = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  next_state = RXC_TC_STATE;

  while( next_state == RXC_TC_STATE )
  {
    if( sigs & RXC_INT_SIG )
    {
      /* After returning from GPS, deinterleaver still
      ** contains up to two legacy frames in its double
      ** buffer. Wait 2 frames before starting normal
      ** RDA operation.
      */
      if ( rxc_hho_data.rxc_gps_state == RXC_GPS_STATE_RETURNED )
      {
        if ( rxc_hho_data.rxc_gps_tuneback_frames++ >= 2 )
        {
          rxc_hho_data.rxc_gps_state = RXC_GPS_STATE_NONE;
          M1X_MSG( MUX, LEGACY_MED,
            "No longer ignoring frames" );
        }
      }

      /* Copy the global shared var's here in local var's */
      rxc_mvs_standby_progress_status = rxc_mvs_get_standby_progress_status();
      rxc_mvs_standby_status          = rxc_mvs_get_standby_status();
      rxc_mvs_tc_entry_enabled        = rxc_get_mvs_status_on_tc_entry();
      rxc_mvs_voc_config_done         = rxc_get_voc_config_done();
      rxc_mvs_is_so_change_status     = rxc_mvs_get_is_so_change();

      /* Check for Rx AGC delta */
      #if defined(FEATURE_1XCP_QSH_SUPPORT) &&  \
          defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH)
      if( srch_mux_is_rx_div_enabled() )
      {
        rxc_check_rx_agc_delta();
      }
      #endif /* FEATURE_1XCP_QSH_SUPPORT && 
                FEATURE_QSH_EVENT_NOTIFY_TO_QSH */

      #ifdef FEATURE_IS2000_1X_ADV
      /* Changes to enable logging with FW only if log packets
         are really enabled */
      if ( log_status( LOG_1X_ADV_L1_PERF_C   ) ||
           log_status( LOG_1X_ADV_FET_STATS_C ) )
      {
        rxc_update_1xA_logging_status( TRUE );
      }
      #endif /* FEATURE_IS2000_1X_ADV */

      /* Process Interrupt signal */
      rxc_tc_data();

      /* -------------------------------------------------------------
      ** If the primary service option is voice, configure the vocoder
      ** for voice option
      ** 
      ** If this is the first time vocoder initialization then 
      ** tc_entry_enabled and is_so_change both would be FALSE.
      ** 
      ** If vocoder is re initialized because of SO change then
      ** is_so_change would be TRUE and tc_entry_enabled would be FALSE.
      **
      ** If vocoder is re initialized because of HHO then is_so_change
      ** would be FALSE and tc_entry_enabled would be TRUE.
      ** ------------------------------------------------------------- */
      if ( ( rxc_mvs_voc_config_done == FALSE ) &&
           ( CAI_IS_SO_VOICE( rxc_so.pri ) ) )
      {
        if( ( ( !rxc_mvs_tc_entry_enabled ) && 
              ( rxc_mvs_is_so_change_status == FALSE ) ) ||
            ( ( rxc_mvs_standby_progress_status == TRUE ) &&
              ( rxc_mvs_standby_status == MVS_STANDBY_SUCCESS ) ) )
        {
          M1X_MSG( MUX, LEGACY_MED,
                   "STANDBY_PROGRESS: %d, STANDBY_STATUS %d, TC_ENTRY_STATUS %d"
                   " VOC_CONFIG %d SO CHANGE %d",
                    rxc_mvs_standby_progress_status,rxc_mvs_standby_status,
                    rxc_mvs_tc_entry_enabled,rxc_mvs_voc_config_done,
                    rxc_mvs_is_so_change_status );

          rxc_set_voc_config_done( muxvoc_config_vocoder(rxc_so.pri) );

          if( rxc_mvs_is_so_change_status == TRUE )
          {
            rxc_mvs_set_is_so_change(FALSE);
          }
        }
      }

      /* Count down handoff fade timer */
      if( rxc_fade_data.rxc_ho_cnt > 0 )
      {
        rxc_fade_data.rxc_ho_cnt--;
      }
    }

    /* Check for received MSGR commands and process them. */
    if( sigs & RXC_MUXMSGR_Q_SIG )
    {
      (void) rex_clr_sigs( rex_self(), RXC_MUXMSGR_Q_SIG );
      muxmsgr_process_rcv_msg( RX_TASK );
    }

    #ifdef FEATURE_MODEM_1X_DRX
    if( ( rxc_mux_rc_changed && rxc_disable_drx_rc_change ) ||
        ( txc_get_frame_off_in_progress() ) ||
        ( rxc_fpc_mode_changed && rxc_drx_disable_fpc_mode( txc_so.fpc_mode ) ) )
    {
      M1X_MSG( MUX,LEGACY_HIGH,
        "DRX: RC changed or FO in progress or FPC 200Hz, disable DrX" );

      rxc_mux_drx_enable( FALSE );
      rxc_mux_rc_changed        = FALSE;
      rxc_disable_drx_rc_change = FALSE;
    }
    #endif /* FEATURE_MODEM_1X_DRX */

    /* Check for received commands */
    (void) rex_clr_sigs( rex_self(), RXC_CMD_Q_SIG );
    if( ( cmd_ptr = (rxc_cmd_msg_type *)q_get( &rxc_common_data.rxc_cmd_q ) ) )
    {
      switch( cmd_ptr->hdr.command )
      {
        case RXC_CDMA_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_CDMA_F" );

          /* --------------------------------------------------
          ** Transition back to CDMA init state without exiting
          ** rxc_subtask()
          ** -------------------------------------------------- */

          /* -------------------------------------
          ** Log last frame rate entry if required
          ** ------------------------------------- */
          if( rxc_bufs_qs_data.rxc_log_mar_buf.count )
          {
            RXC_SEND_MAR_LOG();
          }

          if( rxc_frame_data.rxc_tc_var.msg_ptr )
          {
            #ifdef FEATURE_1X_CP_MEM_OPT
            RXTX_FREE_QUEUE_BUF( rxc_frame_data.rxc_tc_var.msg_ptr, RXTX_RX_Q );
            #else /* !FEATURE_1X_CP_MEM_OPT */
            q_put( &rxtx_rx_free_q, &rxc_frame_data.rxc_tc_var.msg_ptr->link );
            rxc_frame_data.rxc_tc_var.msg_ptr = NULL;
            #endif /* FEATURE_1X_CP_MEM_OPT */
          }

          /* --------------------------------------
          ** Clean up before switching to CDMA mode
          ** -------------------------------------- */
          rxc_exit();

          rxc_set_frame_offset( 0 );

          cmd_ptr->hdr.status = RXC_DONE_S;
          next_state          = RXC_CDMA_STATE;
          break;
        }

        case RXC_SC_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_SC_F" );

          /* --------------------------------------------------
          ** Transition back to SYNC_ACQ state without exiting
          ** rxc_subtask()
          ** -------------------------------------------------- */

          /* -------------------------------------
          ** Log last frame rate entry if required
          ** ------------------------------------- */
          if( rxc_bufs_qs_data.rxc_log_mar_buf.count )
          {
            RXC_SEND_MAR_LOG();
          }

          if( rxc_frame_data.rxc_tc_var.msg_ptr )
          {
            #ifdef FEATURE_1X_CP_MEM_OPT
            RXTX_FREE_QUEUE_BUF( rxc_frame_data.rxc_tc_var.msg_ptr , RXTX_RX_Q);
            #else /* !FEATURE_1X_CP_MEM_OPT */
            q_put( &rxtx_rx_free_q, &rxc_frame_data.rxc_tc_var.msg_ptr->link );
            rxc_frame_data.rxc_tc_var.msg_ptr = NULL;
            #endif /* FEATURE_1X_CP_MEM_OPT */
          }

          /* --------------------------------------
          ** Clean up before switching to SYNC mode
          ** -------------------------------------- */
          rxc_exit();

          cmd_ptr->hdr.status = RXC_DONE_S;

          rxc_sc_init( RXC_TC_STATE );

          next_state = RXC_SC_STATE;
          break;
        }/* RXC_SC_F */

        case RXC_EXIT_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_EXIT_F" );

          #ifdef FEATURE_MODEM_1X_DRX
          /* Disable DRX feature on call termination */
          rxc_serv_conn_rxed  = FALSE;
          rxc_mux_drx_enabled = FALSE;
          rxc_mux_drx_enable( FALSE );
          #endif /* FEATURE_MODEM_1X_DRX */

          /* Log last frame rate entry if required */
          if (rxc_bufs_qs_data.rxc_log_mar_buf.count != 0)
          {
            RXC_SEND_MAR_LOG();
          }

          if( rxc_frame_data.rxc_tc_var.msg_ptr )
          {
            #ifdef FEATURE_1X_CP_MEM_OPT
            RXTX_FREE_QUEUE_BUF( rxc_frame_data.rxc_tc_var.msg_ptr, RXTX_RX_Q );
            #else /* !FEATURE_1X_CP_MEM_OPT */
            q_put(&rxtx_rx_free_q, &rxc_frame_data.rxc_tc_var.msg_ptr->link );
            rxc_frame_data.rxc_tc_var.msg_ptr = NULL;
            #endif /* FEATURE_1X_CP_MEM_OPT */
          }

          muxvoc_disable_rx();

          #ifdef FEATURE_MODEM_1X_TA_MCVS
          /* Update SRCH : DATA SO is terminated */
          srch_mux_set_data_so( FALSE );
          #endif /* FEATURE_MODEM_1X_TA_MCVS */

          /* Time sclk logging */
          rxc_ta_info.ta_start_sclk      = 0;
          rxc_ta_info.f_fch_ta_comp_sclk = 0;
          rxc_ta_info.f_sch_ta_comp_sclk = 0;
          rxc_ta_info.f_fch_ta_log       = FALSE;
          rxc_ta_info.f_sch_ta_log       = FALSE;

          /* Close the QTA GAP here, unexpected QTA stop */
          rxc_ta_info.ta_rsn   = SRCH_TA_NONE;
          rxc_ta_info.ta_start = FALSE;
          rxc_ta_info.ta_comp  = TRUE;
          cmd_ptr->hdr.status = RXC_DONE_S;
          next_state          = RXC_EXIT_STATE;
          break;
        }

        case RXC_PWR_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_PWR_F" );

          /* Reset power control parameters */
          rxc_pwr.threshold_on = cmd_ptr->pwr.pwr_thresh_enable;
          rxc_pwr.periodic_on  = cmd_ptr->pwr.pwr_period_enable;

          if( rxc_pwr.threshold_on || rxc_pwr.periodic_on )
          {
            rxc_pwr.pwr_thresh                = cmd_ptr->pwr.pwr_rep_thresh;
            rxc_pwr.pwr_frames                = cmd_ptr->pwr.pwr_rep_frames;
            rxc_pwr.delay_frames              = cmd_ptr->pwr.pwr_rep_delay;
            rxc_pwr.num_bad_frames            = 0;
            rxc_pwr.num_frames                = 0;
            rxc_pwr.skipped_num_frames        = 0;
            #ifdef FEATURE_IS2000_REL_A_SVD
            rxc_pwr.dcch_num_frames           = 0;
            rxc_pwr.skipped_dcch_num_frames   = 0;
            rxc_pwr.dcch_num_bad_frames = 0;
            #endif /* FEATURE_IS2000_REL_A_SVD */
            rxc_pwr.num_delay_frames          = 0;
            rxc_pwr.sch_tot_frames[0]         = 0;
            rxc_pwr.skipped_sch_tot_frames[0] = 0;
            rxc_pwr.sch_bad_frames[0]         = 0;
            rxc_pwr.sch_fer_rep[0]            = FALSE;
            rxc_pwr.sch_tot_frames[1]         = 0;
            rxc_pwr.skipped_sch_tot_frames[1] = 0;
            rxc_pwr.sch_bad_frames[1]         = 0;
            rxc_pwr.sch_fer_rep[1]            = FALSE;
          }
          cmd_ptr->pwr.hdr.status = RXC_DONE_S;
          break;
        } /* RXC_PWR_F */

        case RXC_ERASE_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_ERASE_F" );

          /* treat blank and burst as erasures */
          rxc_frame_data.rxc_erase  = TRUE;
          cmd_ptr->erase.hdr.status = RXC_DONE_S;
          break;
        }
        /* Processing of Supplemental Channel (SCH IS95C) update command */
        case RXC_SCH_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_SCH_F" );

          dec_config_clk_resources( DEC_CLK_DATA_TRAFFIC );

          rxc_sch.rc                             = (dec_rc_type) cmd_ptr->sch.rc;
          rxc_sch.sch_rate                       = cmd_ptr->sch.sch_rate;
          rxc_sch.mux_pdu_double                 = cmd_ptr->sch.double_size_pdu;
          rxc_sch.code_type                      = cmd_ptr->sch.code_type;
          rxc_frame_data.rxc_tc_var.sch_loopback = FALSE; /* Initialy off */
          RXC_ENTER_ATOMIC_SECTION();
          /* initialize the dtx threshold value */
          dec_dtx_thresh_init( rxc_sch.rc, rxc_sch.code_type, rxc_sch.sch_rate );

          rxc_set_mac_rnd( (deint_rc_type) rxc_sch.rc, rxc_sch.sch_rate,
                           rxc_sch.code_type );

          #ifdef FEATURE_IS2000_REL_A //32X
          deint_set_sch_mode( rxc_sch.num_ltu, rxc_sch.ltu_len, rxc_sch.mux_pdu_type,
                              (deint_rc_type) rxc_sch.rc, rxc_sch.code_type,
                              rxc_sch.sch_rate);
          /* Initialize variables used in 2nd ATMR ISR */
          deint_atmr_isr2_init();

          /* Set 2nd ATMR ISR to go off at the 4th bin */
          err = m1x_stmr_register( M1X_STMR_CLIENT_RX, M1X_STMR_PCG_12, rxc_atmr_isr2 );

          if( err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_REG_DUP )
          {
            ONEX_ERR_FATAL( "Unable to schedule the event with err: %d", err, 0, 0 );
          }
          #else
          deint_set_sch_mode( (deint_rc_type) rxc_sch.rc, rxc_sch.code_type,
                              rxc_sch.sch_rate);
          #endif /* FEATURE_IS2000_REL_A */

          if( rxc_sch.code_type == DEC_CONVOLUTIONAL )
          {
            rxc_sup_dec_int_enable = TRUE;
            rxc_turbo_dec_enabled  = FALSE;
          }
          else
          {
            /* Decoding = TURBO */
            rxc_sup_dec_int_enable = TRUE;
            rxc_turbo_dec_enabled  = TRUE;
          }
          RXC_LEAVE_ATOMIC_SECTION();

          /* Initialize the MuxPDU dsm array, if it hasn't been initialized */
          if( rxc_log_sch_data.rxc_dsm_arr_cnt == RXC_DSM_ARR_NOT_ALLOC )
          {
            rxc_init_sch_mux();
          }

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_RESET_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_RESET_F" );

          /* reset the fade timer */
          rxc_fade_data.rxc_good_frames            = 0;
          rxc_fade_data.rxc_good_guaranteed_frames = 0;
          rxc_fade_data.rxc_t5m_bad_frames         = 0;
          rxc_fade_data.rxc_n2m_bad_frames         = 0;

          /* the following two lines are good to have */
          rxc_fade_data.rxc_fade         = FALSE;
          rxc_fade_data.rxc_sent_bad_rpt = FALSE;

          rxcrda_data.rxcrda_dcch_dtx_fix_counter = -1;
          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_PWR_ON_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_PWR_ON_F" );

          /* Restart power control reporting */
          rxc_pwr.threshold_on = cmd_ptr->pwr_on.pwr_thresh_enable;
          rxc_pwr.periodic_on  = cmd_ptr->pwr_on.pwr_period_enable;

          rxc_pwr.num_frames                = 0;
          rxc_pwr.skipped_num_frames        = 0;
          rxc_pwr.num_bad_frames            = 0;
          #ifdef FEATURE_IS2000_REL_A_SVD
          rxc_pwr.dcch_num_frames           = 0;
          rxc_pwr.skipped_dcch_num_frames   = 0;
          rxc_pwr.dcch_num_bad_frames       = 0;
          #endif /* FEATURE_IS2000_REL_A_SVD */
          rxc_pwr.sch_tot_frames[0]         = 0;
          rxc_pwr.skipped_sch_tot_frames[0] = 0;
          rxc_pwr.sch_bad_frames[0]         = 0;
          rxc_pwr.sch_fer_rep[0]            = FALSE;
          rxc_pwr.sch_tot_frames[1]         = 0;
          rxc_pwr.skipped_sch_tot_frames[1] = 0;
          rxc_pwr.sch_bad_frames[1]         = 0;
          rxc_pwr.sch_fer_rep[1]            = FALSE;

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_PWR_RESET_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_PWR_RESET_F" );

          /* Initialize power control as specified in IS-95 6.6.4.1.1.1 */
          rxc_pwr.num_frames              = 0;
          rxc_pwr.skipped_num_frames      = 0;
          rxc_pwr.num_bad_frames          = 0;
          #ifdef FEATURE_IS2000_REL_A_SVD
          rxc_pwr.dcch_num_frames         = 0;
          rxc_pwr.skipped_dcch_num_frames = 0;
          rxc_pwr.dcch_num_bad_frames     = 0;
          #endif /* FEATURE_IS2000_REL_A_SVD */

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        #ifdef FEATURE_IS2000_REL_A_SVD
        case RXC_RELEASE_SO_F:       /* rxc release service option */
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_RELEASE_SO_F" );

          rxc_mvs_set_is_so_change( FALSE );

          /* get the service option data from the service option ID */
          /* check if release request for primary service */
          if( rxc_srid_ref[cmd_ptr->release_so.sr_id].traffic_type == CAI_PRI_TRAF )
          {
            muxvoc_disable_rx();

            #ifdef FEATURE_MODEM_1X_TA_MCVS
            /* Update SRCH : DATA SO is terminated */
            srch_mux_set_data_so( FALSE );
            #endif /* FEATURE_MODEM_1X_TA_MCVS */

            rxc_so.pri = CAI_SO_NULL;
          }
          /* check if release request for secondary service */
          if( rxc_srid_ref[cmd_ptr->release_so.sr_id].traffic_type == CAI_SEC_TRAF )
          {
            rxc_so.sec = CAI_SO_NULL;
          }

          #ifdef FEATURE_MODEM_1X_DRX
          /* Disable DRX feature on call termination */
          rxc_serv_conn_rxed  = FALSE;
          rxc_mux_drx_enabled = FALSE;
          rxc_mux_drx_enable( FALSE );
          #endif /* FEATURE_MODEM_1X_DRX */

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }
        #endif /* FEATURE_IS2000_REL_A_SVD */

        case RXC_RELEASE_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_RELEASE_F" );

          rxc_mvs_set_is_so_change( FALSE );

          muxvoc_disable_rx();

          #ifdef FEATURE_MODEM_1X_TA_MCVS
          /* Update SRCH : DATA SO is terminated */
          srch_mux_set_data_so( FALSE );
          #endif /* FEATURE_MODEM_1X_TA_MCVS */

          /* Release all services */
          rxc_so.pri = CAI_SO_NULL;
          rxc_so.sec = CAI_SO_NULL;

          #ifdef FEATURE_MODEM_1X_DRX
          /* Disable DRX feature on call termination */
          rxc_serv_conn_rxed  = FALSE;
          rxc_mux_drx_enabled = FALSE;
          rxc_mux_drx_enable( FALSE );
          #endif /* FEATURE_MODEM_1X_DRX */

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_PWR_REP_DELAY_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_PWR_REP_DELAY_F" );

          /* --------------------------------------------------------------
          ** After sending a Power Measurement Report Message, the mobile
          ** station shall set TOT_FRAMES and BAD_FRAMES to zero and shall
          ** not increment either counter for a period of PWR_REP_DELAY * 4
          ** frames following the first transmission of the message.
          ** -------------------------------------------------------------- */
          rxc_pwr.num_frames     = (rxc_pwr.num_frames >= rxc_pwr.num_frames_current)?
                                   (rxc_pwr.num_frames - rxc_pwr.num_frames_current): 0;
          rxc_pwr.num_bad_frames = (rxc_pwr.num_bad_frames >= rxc_pwr.num_bad_frames_current)?
                                   (rxc_pwr.num_bad_frames - rxc_pwr.num_bad_frames_current) : 0;
          M1X_MSG( MUX, LEGACY_MED,
            "After PMRM Sent,FCH Frame Count %d, Bad Count %d",
            rxc_pwr.num_frames,
            rxc_pwr.num_bad_frames);
          #ifdef FEATURE_IS2000_REL_A_SVD
          rxc_pwr.dcch_num_frames     = (rxc_pwr.dcch_num_frames >= rxc_pwr.dcch_num_frames_current)?
                                        (rxc_pwr.dcch_num_frames - rxc_pwr.dcch_num_frames_current): 0;
          rxc_pwr.dcch_num_bad_frames = (rxc_pwr.dcch_num_bad_frames >= rxc_pwr.dcch_num_bad_frames_current)?
                                        (rxc_pwr.dcch_num_bad_frames - rxc_pwr.dcch_num_bad_frames_current) : 0;
          M1X_MSG( MUX, LEGACY_MED,
            "After PMRM Sent,DCCH Frame Count %d, Bad Count %d",
            rxc_pwr.dcch_num_frames,
            rxc_pwr.dcch_num_bad_frames);
          #endif /* FEATURE_IS2000_REL_A_SVD */
          rxc_pwr.num_delay_frames = (word)(rxc_pwr.delay_frames * 4);

          /* Restart power control reporting */
          rxc_pwr.threshold_on = cmd_ptr->pwr_rep_delay.pwr_thresh_enable;
          rxc_pwr.periodic_on  = cmd_ptr->pwr_rep_delay.pwr_period_enable;

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_TC_F:
        {
          /* We have been told to go to traffic channel state but we are
             already there.  Just ignore the command */
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_TC_F" );

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_OLR_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_OLR_F" );

          ffpc_send_olrm ();

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_FTM_FCH_LOOPBACK:
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_FTM_FCH_LOOPBACK" );
          rxc_frame_data.rxc_tc_var.fch_loopback = cmd_ptr->loopback.loopback_on;
          break;
                /* Support FEATURE_FTM_DCCH */
        case RXC_FTM_DCCH_LOOPBACK:
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_FTM_DCCH_LOOPBACK" );
          rxc_frame_data.rxc_tc_var.dcch_loopback = cmd_ptr->loopback.loopback_on;
          break;
                /* End of FEATURE_FTM_DCCH */
        case RXC_FTM_SCH_LOOPBACK:
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_FTM_SCH_LOOPBACK" );
          rxc_frame_data.rxc_tc_var.sch_loopback = cmd_ptr->loopback.loopback_on;
          break;

        #ifdef FEATURE_IS2000_1X_ADV
        case RXC_FTM_RCP_DATA_F:
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_FTM_RCP_DATA" );

          /* Save RCP data to local data structure */
          rxc_traffic_status.rxc_rc11.rcp = cmd_ptr->rcp.rcp_data;

          /* Write RC11 RPC parameters to MDSP. */
          muxmdsp_update_rc11_params( &rxc_traffic_status.rxc_rc11.rcp );

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        #endif /* FEATURE_IS2000_1X_ADV */

        #ifdef FEATURE_IS2000_REL_A
        case RXC_GET_SETPT_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_GET_SETPT_F" );
          ffpc_get_setpt
          (
            &(cmd_ptr->setpt.fch_incl),
            &(cmd_ptr->setpt.fpc_fch_curr_setpt),
            &(cmd_ptr->setpt.dcch_incl),
            &(cmd_ptr->setpt.fpc_dcch_curr_setpt),
            &(cmd_ptr->setpt.num_sup),
            &(cmd_ptr->setpt.sch_setpt_info[0].sch_id),
            &(cmd_ptr->setpt.sch_setpt_info[0].fpc_sch_curr_setpt)
          );
          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }
        #endif //FEATURE_IS2000_REL_A

        #ifdef FEATURE_IS2000_REL_B
        case RXC_IDLE_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_IDLE_F" );

          /* Transition back to Idle state without going to Sync state
          ** first.
          */

          /* Log last frame rate entry if required (for Markov.)
          */
          if( rxc_bufs_qs_data.rxc_log_mar_buf.count )
          {
            RXC_SEND_MAR_LOG();
          }

          /* Make sure all items are placed back on the rxtx_rx_free_q
          */
          if( rxc_frame_data.rxc_tc_var.msg_ptr )
          {
            #ifdef FEATURE_1X_CP_MEM_OPT
            RXTX_FREE_QUEUE_BUF( rxc_frame_data.rxc_tc_var.msg_ptr, RXTX_RX_Q );
            #else /* !FEATURE_1X_CP_MEM_OPT */
            q_put( &rxtx_rx_free_q, &rxc_frame_data.rxc_tc_var.msg_ptr->link );
            rxc_frame_data.rxc_tc_var.msg_ptr = NULL;
            #endif /* FEATURE_1X_CP_MEM_OPT */
          }

          /* Clean up before switching to Idle state.
          */
          rxc_exit();

          /* Call rxc_free_tc_resources() here instead of outside
          ** switch statement.  In case Mobile is configured to go to
          ** BCCH state, do not call rxc_free_tc_resources() after
          ** ATMR is set up for BCCH since rxc_free_tc_resources()
          ** will free up the ATMR.
          */
          rxc_free_tc_resources();
          free_tc_resources = TRUE;

          /* Set FO to 0 after freeing RX resoucres */
          rxc_set_frame_offset(0);

          /* Initialize for Idle state.  With new common channel support,
          ** Mobile will transition to either Paging channel or
          ** BCCH/F-CCCH.
          */
          rxcidle_init();

          #ifdef FEATURE_MODEM_1X_SW_DEMBACK
          if ( rxc_use_sw_demback() == TRUE )
          {
            /* 1X is transitioning from traffic to IDLE
               so a switch to SW demback is needed here */
            muxmdsp_set_demback_mode( FALSE, TRUE );

            /* Set the demback ID */
            if( muxmdsp_fw_smem_addr )
            {
              dec_set_demback_id( muxmdsp_get_demback_id_index() );
            }
            else
            {
              M1X_MSG ( MUX, LEGACY_ERROR, "demback id not set" );
            }
          }
          #endif /* FEATURE_MODEM_1X_SW_DEMBACK */

          if( cmd_ptr->idle.chan_info.chan_id == CAI_PCH )
          {
            rxc_pc_init( RXC_SC_STATE, cmd_ptr->idle.chan_info.config.pch.rate );

            next_state = RXC_PC_STATE;
          }

          cmd_ptr->hdr.status = RXC_DONE_S;

          if( next_state == RXC_TC_STATE )
          {
            M1X_MSG( MUX, LEGACY_ERROR,
              "Failed Traffic to Idle transition" );
          }
          break;
        }
        #endif /* FEATURE_IS2000_REL_B */

        case RXC_MVS_ENABLE_STATUS_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_MVS_STAT_F, stat:%d",
            cmd_ptr->mvs_enable.status );
          muxvoc_enable_rx_sync_check();
          cmd_ptr->hdr.status = RXC_DONE_S;
        }
        break;

        case RXC_VOC_ENABLE_F:
        {
          /* MC has indicated that the vocoder needs to be
             configured (or reconfigured). Reset the config
             done flag and inhibit flags.
          */
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_VOC_ENABLE_F" );
          muxvoc_clear_inhibit_voc_enable_flag();
          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_TA_START_F:
        {
          M1X_MSG( MUX, LEGACY_HIGH,
            "1x2GTA: Got RXC_TA_START_F, reason %d",
            cmd_ptr->ta_start.ta_type );

          M1X_MSG( MUX, LEGACY_HIGH,
             "1x2GTA: TMO %d, T5m %d, Rem %d",
             RXC_TC_TMO_FRAMES,
             rxc_fade_data.rxc_t5m_bad_frames,
             ( RXC_TC_TMO_FRAMES - rxc_fade_data.rxc_t5m_bad_frames )*20);

          rxc_ta_info.ta_rsn = cmd_ptr->ta_comp.ta_type;

          /* Disable 0x14ED packet logging before TA starts */
          rxc_update_1xA_logging_status(FALSE);

          /*Reset the TA skipped frames during TA START */
          rxc_pwr.skipped_num_frames        = 0;
          rxc_pwr.skipped_dcch_num_frames   = 0;
          rxc_pwr.skipped_sch_tot_frames[0] = 0;

          /* If any logging pending from previous LTA/QTA, log it here */
          if( rxc_ta_info.f_fch_ta_log || rxc_ta_info.f_sch_ta_log ||
              txc_ta_info.r_fch_ta_log || txc_ta_info.r_sch_ta_log )
          {
            /* Update SRCH here about the stats */
            rxc_tx_ta_srch_log_update();
          }

          /* TA sclk time logging */
          rxc_ta_info.ta_start_sclk = timetick_get();
          rxc_ta_info.f_fch_ta_log  = FALSE;
          rxc_ta_info.f_sch_ta_log  = FALSE;

          if( cmd_ptr->ta_start.ta_type == SRCH_TA_LTA )
          {
            timer_def(&(rxc_ta_info.ta_fade_timer), NULL, RX_TCB_PTR,
                    0, rxc_ta_fade_timer_cb, 0);

            timer_set(&(rxc_ta_info.ta_fade_timer),
            ( RXC_TC_TMO_FRAMES - rxc_fade_data.rxc_t5m_bad_frames )*20,0,T_MSEC);

            M1X_MSG( MUX, LEGACY_HIGH,
              "1x2GTA: Current time %d",
              timetick_get() );

            rxc_ta_info.lta_start_sclk = timetick_get();

            next_state = RXC_TA_STATE;
          }

          rxc_ta_info.ta_start = TRUE;
          rxc_ta_info.ta_comp  = FALSE;

          /* If enabled, send accumulated RLP Frames log. */
          dsrlp_log_rx_send_frames();

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_TA_COMP_F:
        {
          M1X_MSG( MUX, LEGACY_HIGH,
            "1x2GTA: Got RXC_TA_COMP_F, reason %d",
            cmd_ptr->ta_comp.ta_type );

          rxc_ta_info.ta_rsn = SRCH_TA_NONE;

          /* Re-enable 0x14ED packet logging after TA finishes */
          rxc_update_1xA_logging_status(TRUE);

          /* Time sclk logging */
          rxc_ta_info.f_fch_ta_comp_sclk = 0;
          rxc_ta_info.f_sch_ta_comp_sclk = 0;

          /* No need to log F-F/SCH TA time since DEC is always enabled for QTA */
          rxc_ta_info.f_fch_ta_log       = FALSE;
          rxc_ta_info.f_sch_ta_log       = FALSE;

          rxc_ta_info.ta_start = FALSE;
          rxc_ta_info.ta_comp  = TRUE;

          /* Update T5m timer, in case TA GAP opened in Erasures */
          rxc_fade_data.rxc_t5m_bad_frames += rxc_pwr.skipped_num_frames;

          /* Update RIF timeout related counts */
          rxc_ta_rif_lost_frames_update( rxc_pwr.skipped_num_frames );

          /* Update FCH,DCCH and SCH frame count for PMRM */
          rxc_ta_fer_update( TRUE, 0 );

          /* If enabled, send accumulated RLP Frames log. */
          dsrlp_log_rx_send_frames();

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        default:
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "Illegal RX command %d",
            cmd_ptr->hdr.command );
          cmd_ptr->hdr.status = RXC_BADCMD_S;
        }
      } /* switch */

      /* return item to queue if requested */
      cmd_done( &cmd_ptr->hdr.cmd_hdr );

      (void) rex_clr_sigs( rex_self(), RXC_CMD_Q_SIG );
    }
    else
    {
      sigs = RXC_CMD_Q_SIG | RXC_INT_SIG | RXC_MUXMSGR_Q_SIG;

      /* Wait for a command or an interrupt */
      sigs = rxc_wait( sigs );
    }
  } /* while */

  #ifdef FEATURE_IS2000_REL_B
  if( free_tc_resources == FALSE )
  #endif /* FEATURE_IS2000_REL_B */
  {
    /* If haven't done it above, do traffic Channel exit clean-up now */
    rxc_free_tc_resources();
  }

  if( !rxc_is_ta_enabled() )
  {
    /* Reset active service options to none active */
    rxc_so.pri = CAI_SO_NULL;
    rxc_so.sec = CAI_SO_NULL;

    /* Reset RCs to Init RC */
    rxc_so.for_fch.rc  = CAI_INIT_RC;
    rxc_so.for_dcch.rc = CAI_INIT_RC;
  }

  return next_state;

} /* rxc_tc */

/*===========================================================================

FUNCTION RXC_TA_TOTAL_SKIP_FRAME_COUNT_INC

DESCRIPTION
  Increment total FRAME COUNT for FCH/DCCH/SCH only outside TA GAP
  Increment skipped FRAME COUNT for FCH/DCCH/SCH during TA GAP

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void rxc_ta_total_skip_frame_count_inc( word* total_frame_count, word* skip_frame_count )
{
  boolean rxc_ta_is_enabled = rxc_is_ta_enabled();

  if( rxc_ta_is_enabled )
  {
    (*skip_frame_count)++;
    M1X_MSG( MUX, LEGACY_HIGH,
       "1x2GTA: Suspend FCH/DCCH/SCH total frame count" );
  }
  else
  {
    (*total_frame_count)++;
  }
}  /* rxc_ta_total_skip_frame_count_inc */

/*===========================================================================

FUNCTION RXC_TA_BAD_FRAME_COUNT_INC

DESCRIPTION
  Increment BAD FRAME COUNT for FCH/DCCH/SCH only outside TA GAP

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void rxc_ta_bad_frame_count_inc( word* bad_frame_count )
{
  boolean rxc_ta_is_enabled = rxc_is_ta_enabled();

  if( rxc_ta_is_enabled )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
       "1x2GTA: Suspend bad frame count" );
  }
  else
  {
    (*bad_frame_count)++;
  }
}  /* rxc_ta_bad_frame_count_inc */

/*===========================================================================

FUNCTION RXC_TA_FER_UPDATE

DESCRIPTION
  Update FCH,DCCH,SCH frame counts after TA GAP

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void rxc_ta_fer_update( boolean sch, uint8 sch_id )
{
  int32 ratio_fr, ratio_dcch;
  ratio_fr   = (rxc_pwr.num_bad_frames*100)      / rxc_pwr.num_frames;
  ratio_dcch = (rxc_pwr.dcch_num_bad_frames*100) / rxc_pwr.dcch_num_frames;

  M1X_MSG( MUX, LEGACY_ERROR,
    "1x2GTA: Calc skipped frames num %d+%d=%d, dcch %d+%d=%d",
    rxc_pwr.num_frames,
    rxc_pwr.skipped_num_frames,
    rxc_pwr.num_frames + rxc_pwr.skipped_num_frames,
    rxc_pwr.dcch_num_frames,
    rxc_pwr.skipped_dcch_num_frames,
    rxc_pwr.dcch_num_frames + rxc_pwr.skipped_dcch_num_frames);
  M1X_MSG( MUX, LEGACY_ERROR,
    "1x2GTA: Calc bad frames ratio %d/100 %d->%d, dcch ratio %d/100 %d->%d",
    ratio_fr,
    rxc_pwr.num_bad_frames,
    (rxc_pwr.num_frames * ratio_fr ) / 100,
    ratio_dcch,
    rxc_pwr.dcch_num_bad_frames,
    (rxc_pwr.dcch_num_frames * ratio_dcch ) / 100);

  rxc_pwr.num_frames           += rxc_pwr.skipped_num_frames;
  rxc_pwr.num_bad_frames        = (rxc_pwr.num_frames * ratio_fr ) / 100;
  rxc_pwr.dcch_num_frames      += rxc_pwr.skipped_dcch_num_frames;
  rxc_pwr.dcch_num_bad_frames   = (rxc_pwr.dcch_num_frames * ratio_dcch ) / 100;

  if( sch == TRUE )
  {
    int32 ratio_sch;

    ratio_sch   = (rxc_pwr.sch_bad_frames[ sch_id ]*100) / rxc_pwr.sch_tot_frames[ sch_id ];

    M1X_MSG( MUX, LEGACY_ERROR,
      "1x2GTA: Calc skipped frames SCH[%d] %d+%d=%d",
      sch_id,
      rxc_pwr.sch_tot_frames[ sch_id ],
      rxc_pwr.skipped_sch_tot_frames[ sch_id ],
      rxc_pwr.sch_tot_frames[ sch_id ] + rxc_pwr.skipped_sch_tot_frames[ sch_id ]);
    M1X_MSG( MUX, LEGACY_ERROR,
      "1x2GTA: Calc bad SCH[%d] ratio %d/100 %d->%d",
      sch_id,
      ratio_sch,
      rxc_pwr.sch_bad_frames[ sch_id ],
      (rxc_pwr.sch_tot_frames[ sch_id ] * ratio_sch ) / 100 );

    rxc_pwr.sch_tot_frames[ sch_id ]  += rxc_pwr.skipped_sch_tot_frames[ sch_id ];
    rxc_pwr.sch_bad_frames[ sch_id ]   = (rxc_pwr.sch_tot_frames[ sch_id ] * ratio_sch ) / 100;

    M1X_MSG( MUX, LEGACY_ERROR,
      "1x2G: Adding skipped SCH[%d] num %d+%d=%d",
      sch_id,
      rxc_pwr.sch_tot_frames[ sch_id ],
      rxc_pwr.skipped_sch_tot_frames[ sch_id ],
      rxc_pwr.sch_tot_frames[ sch_id ] + rxc_pwr.skipped_sch_tot_frames[ sch_id ] );
  }

}/* rxc_ta_fer_update */

/*===========================================================================

FUNCTION RXC_IS_TA_ENABLED

DESCRIPTION
  Returns status of TA GAP

DEPENDENCIES
  None.

RETURN VALUE
  TRUE  : If 1x Rx is in TA GAP
  FALSE : If 1x Rx is not in TA GAP

SIDE EFFECTS
  None

===========================================================================*/
boolean rxc_is_ta_enabled( void )
{
  static boolean rx_ta_gap_is_active = FALSE;

  if( rxc_ta_info.ta_start && !rxc_ta_info.ta_comp )
  {
    if( rx_ta_gap_is_active != TRUE )
    {
      rx_ta_gap_is_active = TRUE;
      M1X_MSG( MUX, LEGACY_HIGH,
         "1x2GTA: Rx GAP is ON" );
    }
  }
  else
  {
    if( rx_ta_gap_is_active != FALSE )
    {
      rx_ta_gap_is_active = FALSE;
      M1X_MSG( MUX, LEGACY_HIGH,
         "1x2GTA: Rx GAP is OFF" );
    }
  }

  return rx_ta_gap_is_active;

}/* rxc_is_ta_enabled */

/*===========================================================================

FUNCTION RXC_TA_FADE_CHECK

DESCRIPTION
  Sets or resets fade flag depending upon TA GAP.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void rxc_ta_fade_check( void )
{
  boolean rxc_ta_is_enabled = rxc_is_ta_enabled();

  if( rxc_ta_is_enabled )
  {
    rxc_fade_data.rxc_fade = FALSE;
    M1X_MSG( MUX, LEGACY_HIGH,
       "1x2GTA: Re-enable fade timer prevented" );
  }
  else
  {
    rxc_fade_data.rxc_fade = TRUE;
  }
} /* rxc_ta_fade_check */

/*===========================================================================

FUNCTION RXC_TA_FFPC_SET_UP

DESCRIPTION
  Set UP FFPC after returning from LTA

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void rxc_ta_ffpc_set_up( void )
{
  /* DCCH configuration */
  if( rxc_so.for_dcch.included )
  {
    if( ffpc_channel == CAI_FPC_PRI_CHAN_DCCH )
    {
      rxc_config_demod_for_traffic( (deint_rc_type) rxc_so.for_dcch.rc );
      rxc_set_dcch_mode( (deint_rc_type) rxc_so.for_dcch.rc, TRUE );
      M1X_MSG( MUX, LEGACY_HIGH,
         "Config demod for DCCH" );
    }
    else
    {
      rxc_config_demod_ch1( (deint_rc_type) rxc_so.for_dcch.rc );
      rxc_set_dcch_mode( (deint_rc_type) rxc_so.for_dcch.rc, FALSE );
      M1X_MSG( MUX, LEGACY_HIGH,
         "Config demod Ch1 for DCCH" );
    }
  }

  /* FCH configuration */
  if( rxc_so.for_fch.included )
  {
    if( ffpc_channel == CAI_FPC_PRI_CHAN_FCH )
    {
      rxc_config_demod_for_traffic( (deint_rc_type) rxc_so.for_fch.rc );
      rxc_set_fch_mode( (deint_rc_type) rxc_so.for_fch.rc, TRUE );
      M1X_MSG( MUX, LEGACY_HIGH,
         "Config demod for FCH" );
    }
    else
    {
      rxc_config_demod_ch1( (deint_rc_type) rxc_so.for_fch.rc );
      rxc_set_fch_mode( (deint_rc_type) rxc_so.for_fch.rc, FALSE );
      M1X_MSG( MUX, LEGACY_HIGH,
         "Config demod Ch1 for FCH" );
    }
  }

  ffpc_update_fpc_mode( FALSE, ffpc_info.fpc_mode );

  ffpc_program_hw ();

  ffpc_config_fch_ratchet_setpt_freeze( ffpc_fch_params.olpc_enabled );

} /* rxc_ta_ffpc_set_up */

/*===========================================================================

FUNCTION RXC_TA_DEC_SET_UP

DESCRIPTION
  Set UP DEC after returning from LTA

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void rxc_ta_dec_set_up( void )
{
  /* free decoder data buffer */
  rxc_clear_decoder_queue ();

  /* Update the Demback ID */
  if( muxmdsp_fw_smem_addr )
  {
    dec_set_demback_id( muxmdsp_get_demback_id_index() );
  }
  else
  {
    M1X_MSG ( MUX, LEGACY_FATAL, "demback id not set" );
  }

  /* Initialize the CDMA deinterleaver post sleep.
     Decoder will also be initialized. */
  deint_init();

  /* switch to Traffic Channel and use initial rate set */
  #ifdef FEATURE_IS2000_REL_A_SVD
  if( rxc_so.for_dcch.included )
  {
    rxc_set_dcch_mode( (deint_rc_type) rxc_so.for_dcch.rc,
                       ( ffpc_channel == CAI_FPC_PRI_CHAN_DCCH ) );
  }

  if( rxc_so.for_fch.included )
  {
    rxc_set_fch_mode( (deint_rc_type) rxc_so.for_fch.rc,
                      ( ffpc_channel == CAI_FPC_PRI_CHAN_FCH ) );
  }
  #else
  if( rxc_so.for_dcch.included )
  {
    // Note: for MSM5000, FCH and DCCH shares the same hardware
    deint_set_fch_mode( rxc_so.for_dcch.rc );
  }

  if (rxc_so.for_fch.included)
  {
    deint_set_fch_mode( rxc_so.for_fch.rc );
  }
  #endif /* FEATURE_IS2000_REL_A_SVD */

  /* get frame number and subtract 1 since the decoder output buffer will
   * contain curr_frame_time-1 frame data in packet mode. */
  m1x_time_get_20ms_frame_time( rxc_dec_frame );
  qw_dec( rxc_dec_frame, 1L );

  #ifdef FEATURE_IS2000_1X_ADV
  if( rxc_so.for_fch.included )
  {
    /* Select the appropriate ISR for frame processing depending on the FCH RC. */
    rxc_tc_isr_select();
  }
  #endif /* FEATURE_IS2000_1X_ADV */

  RXC_ENTER_ATOMIC_SECTION();
  /* For RC11, setting the decoder ISR here prepares for SCH decoder interrupt if
     SCH is assigned.  Since FCH deinterleaver task list is not set, this operation
     has no impact on FCH.
  */
  rxc_register_isr( (dalint_1x_isr) rxc_tc_isr, DALINT_1X_STACK_4K );

  /* Clear DEC DONE int status to make sure ISR is properly serviced. */
  SET_DEMBACK_REG( DEC_CLR_INT_STATUS,
                   HWIO_FMSK( DEC_CLR_INT_STATUS, VD_CLR_STATUS ) );

  RXC_LEAVE_ATOMIC_SECTION();

} /* rxc_ta_dec_set_up */

/*===========================================================================

FUNCTION RXC_TA_LOG_SET_UP

DESCRIPTION
  Set UP logging after returning from LTA

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void rxc_ta_log_set_up( void )
{
  uint32 i = 0;

  rxc_init_averaged_fer(); /* Initialize averaged FER counters */

  /* Initialize channel information used in RLP 3 payload logging. */
  memset (&rxc_bufs_qs_data.rxc_rlp_logging_chan_info, 0,
           sizeof(rxc_bufs_qs_data.rxc_rlp_logging_chan_info));

  /* For 0x1877 logging, RX/TX logging adjustments init */
  for( i = 0; i < LOGGING_FRAME_OFFSET_SB_DTX; i++ )
  {
    rxc_tx_sb_dtx_param.sb_decision[i] = 0;
    rxc_tx_sb_dtx_param.dtx_mask[i]    = 0xFFFF;
  }

  /* Clear the contents of logging buffer structure on traffic entry */
  /* Zero initialize the members of the structure */
  for ( i = 0; i < RX_TX_ONE_FRAME_OFFSET_CORRECTION; i++ )
  {
    log_buff.sb_decision[i]      = 0;
    log_buff.dtx_decision[i]     = 0;
    log_buff.tx_frame_rate[i]    = 0;

    if ( log_buff.mdsp_buf[i] == NULL )
    {
      log_buff.mdsp_buf[i]       =
        modem_mem_calloc( MDSP_BUFF_SIZE, sizeof(uint32),
        MODEM_MEM_CLIENT_1X );
      if ( log_buff.mdsp_buf[i] == NULL )
      {
        M1X_MSG( MUX, LEGACY_HIGH,
          "Unable to allocate memory - skip 0x14ED logging" );
        break;
      }
    }
  }

   /* Zero initialize the members of the structure */
   for ( i = 0; i < RX_TX_TWO_FRAME_OFFSET_CORRECTION; i++ )
   {
     log_buff.critical_frame[i]   = 0;
     log_buff.sch_state[i]        = 0;
   }

   memory_is_allocated_for_logging = TRUE;

   /* Enable 1xA logging */
   rxc_update_1xA_logging_status( TRUE );

} /* rxc_ta_log_set_up */

/*===========================================================================

FUNCTION RXC_TA

DESCRIPTION
  Long Tune Away during 1x2GTA processing state

DEPENDENCIES
  None.

RETURN VALUE
  Current RXC state.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/
rxc_state_type rxc_ta( void )
{
  rxc_state_type next_state;
    /* Next state to be processed */
  rxc_cmd_msg_type *cmd_ptr;
    /* Pointer to command buffer received on rxc_cmd_q */
  word skip_frames;
    /* Frames missed during LTA GAP */
  int err = 0;
    /* Error code returned for VSTMR */

  next_state = RXC_TA_STATE;

  while( next_state == RXC_TA_STATE )
  {
    (void) rex_clr_sigs( rex_self(), RXC_CMD_Q_SIG );
    if ((cmd_ptr = (rxc_cmd_msg_type *)q_get( &rxc_common_data.rxc_cmd_q )) !=
        NULL)
    {
      switch( cmd_ptr->hdr.command )
      {
        case RXC_EXIT_F:
        {
          M1X_MSG( MUX, LEGACY_HIGH,
            "Got RXC_EXIT_F" );

          #ifdef FEATURE_MODEM_1X_DRX
          /* Disable DRX feature on call termination */
          rxc_serv_conn_rxed    = FALSE;
          rxc_mux_drx_enabled   = FALSE;
          rxc_mux_drx_enable( FALSE );
          #endif /* FEATURE_MODEM_1X_DRX */

          /* Log last frame rate entry if required */
          if( rxc_bufs_qs_data.rxc_log_mar_buf.count != 0 )
          {
            RXC_SEND_MAR_LOG();
          }

          if( rxc_frame_data.rxc_tc_var.msg_ptr != NULL )
          {
            #ifdef FEATURE_1X_CP_MEM_OPT
            RXTX_FREE_QUEUE_BUF( rxc_frame_data.rxc_tc_var.msg_ptr, RXTX_RX_Q );
            #else /* !FEATURE_1X_CP_MEM_OPT */
            q_put(&rxtx_rx_free_q, &rxc_frame_data.rxc_tc_var.msg_ptr->link );
            rxc_frame_data.rxc_tc_var.msg_ptr = NULL;
            #endif /* FEATURE_1X_CP_MEM_OPT */
          }

          muxvoc_disable_rx();

          #ifdef FEATURE_MODEM_1X_TA_MCVS
          /* Update SRCH : DATA SO is terminated */
          srch_mux_set_data_so( FALSE );
          #endif /* FEATURE_MODEM_1X_TA_MCVS */

          /* clear the timer and undef it */
         (void) timer_clr( &( rxc_ta_info.ta_fade_timer ), T_NONE );
         (void) timer_undef( &( rxc_ta_info.ta_fade_timer ) );

          /* Time sclk logging */
          rxc_ta_info.ta_start_sclk      = 0;
          rxc_ta_info.f_fch_ta_comp_sclk = 0;
          rxc_ta_info.f_sch_ta_comp_sclk = 0;
          rxc_ta_info.f_fch_ta_log       = FALSE;
          rxc_ta_info.f_sch_ta_log       = FALSE;

          /* Close the LTA GAP here */
          rxc_ta_info.ta_rsn              = SRCH_TA_NONE;
          rxc_ta_info.ta_start            = FALSE;
          rxc_ta_info.ta_comp             = TRUE;
          txc_ta_info.ta_start = FALSE;
          txc_ta_info.ta_comp  = TRUE;
          txc_ta_info.ta_hs_jump_complete   = FALSE;
          txc_ta_info.ta_tc_init_done       = FALSE;
          txc_ta_info.ta_rpc_skip_frames    =  3;
          txc_ta_info.lta_gap_frames        = 0;
          txc_ta_info.lta_start_sclk        = 0;
          txc_ta_info.lta_ol_tx_pwr         = 0;

          /* If enabled, send accumulated RLP Frames log. */
          dsrlp_log_rx_send_frames();

          cmd_ptr->hdr.status = RXC_DONE_S;
          next_state = RXC_EXIT_STATE;
          break;
        }

        case RXC_TA_COMP_F:
        {
          M1X_MSG( MUX, LEGACY_HIGH,
            "1x2GTA: Got RXC_TA_COMP_F, reason %d",
            cmd_ptr->ta_comp.ta_type );

          rxc_ta_info.ta_rsn = SRCH_TA_NONE;

          rxc_ta_info.lta_gap_ms = GET_TIMETICK_MS_FROM_SCLKS
          ( ( timetick_get() ) - rxc_ta_info.lta_start_sclk );

          skip_frames = ((rxc_ta_info.lta_gap_ms)/20);

          M1X_MSG( MUX, LEGACY_HIGH,
             "1x2GTA: T5m %d, SCLK %d, Last start %d, Skip %d",
             rxc_fade_data.rxc_t5m_bad_frames,
             timetick_get(),
             rxc_ta_info.lta_start_sclk,
             skip_frames );

         /* clear the timer and undef it */
         (void) timer_clr( &( rxc_ta_info.ta_fade_timer ), T_NONE );
         (void) timer_undef( &( rxc_ta_info.ta_fade_timer ) );

          /* Vote for data traffic here  */
          dec_config_clk_resources( DEC_CLK_DATA_TRAFFIC );

          /* Restore lost TC context here */
          ffpc_fch_params.olpc_enabled  = rxc_ta_info.fch_olpc_enabled;
          ffpc_fch_params.incl_status   = rxc_ta_info.fch_incl_status;

          rxc_set_frame_offset( rxc_ta_info.frame_offset );
          muxmdsp_set_agc_ctl_mask_delay( rxc_ta_info.rev_pwr_ctrl_delay );

         /* register to MC all RXC action time handlers */
          rxc_register_action_handlers_notifier_interrupt_func();
          muxmsgr_register_msg( RX_TASK, CDMA_FW_IND_ADV1X_LOG_BUFFER_RDY );

         /*Register action timer*/
         err = m1x_stmr_register( M1X_STMR_CLIENT_RX, M1X_STMR_PCG_4, rxc_atmr_isr );

         if( err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_REG_DUP )
         {
           ONEX_ERR_FATAL( "Unable to schedule the event with err: %d", err, 0, 0 );
         }

          /* Dont configure VOC for data call */
          muxvoc_rx_tc_init( TRUE );

          /* Set up FFPC */
          rxc_ta_ffpc_set_up();

          /* set up decoder here */
          rxc_ta_dec_set_up();

          /* Set up logging after LTA */
          rxc_ta_log_set_up();

          rxcrda_data.rxcrda_sch_dtx_fix_counter = -1;

          /* Initialize SCH0 RC */
          rxc_sch.rc = (dec_rc_type) DEINT_RC_INVALID;

          /* Update SRCH about data , 1xA, RC's etc */
          #ifdef FEATURE_MODEM_1X_TA_MCVS
          srch_mux_set_data_so( rxc_is_data_call() );
          #endif /* FEATURE_MODEM_1X_TA_MCVS */

          #ifdef FEATURE_IS2000_1X_ADV
          if( rxc_so.for_fch.rc == CAI_RC_11 )
          {
            srch_mux_notify_1x_adv_traffic( TRUE );
          }
          #endif /* FEATURE_IS2000_1X_ADV */

          srch_mux_update_rc( (deint_rc_type) rxc_so.for_fch.rc,
                              (deint_rc_type) rxc_sch.rc,
                              DEINT_RC_INVALID,
                              DEINT_RC_INVALID );

          /* set up the rxc flag so that ffpc will not update the setpoint */
          rxc_set_tx_in_traffic_on( FALSE );

          /* Restore old PMRM frame stats before FER update */
          rxc_fade_data.rxc_t5m_bad_frames += skip_frames;
          rxc_pwr.skipped_num_frames        = skip_frames;
          rxc_pwr.skipped_dcch_num_frames   = skip_frames;
          rxc_pwr.skipped_sch_tot_frames[0] = skip_frames;

          /* Update RIF timeout related counts */
          rxc_ta_rif_lost_frames_update( rxc_pwr.skipped_num_frames );

          /* Update FCH,DCCH and SCH frame count for PMRM */
          rxc_ta_fer_update( TRUE, 0 );

          /* Time sclk logging */
          rxc_ta_info.f_fch_ta_comp_sclk = 0;
          rxc_ta_info.f_sch_ta_comp_sclk = 0;
          rxc_ta_info.f_fch_ta_log       = TRUE;
          /* 0x18CD : Log F-SCH only if its assigned */
          if( mccsch_sch_is_active(MCCSCH_FORWARD, 0) )
          {
            rxc_ta_info.f_sch_ta_log     = TRUE;
          }

          rxc_ta_info.ta_start = FALSE;
          rxc_ta_info.ta_comp  = TRUE;

          /* If enabled, send accumulated RLP Frames log. */
          dsrlp_log_rx_send_frames();

          cmd_ptr->hdr.status = RXC_DONE_S;
          next_state          = RXC_TC_STATE;
          break;
        }

         default:
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "Illegal RX command %d",
            cmd_ptr->hdr.command );
          cmd_ptr->hdr.status = RXC_BADCMD_S;
        }
      }

      /* return item to queue if requested */
      cmd_done( &cmd_ptr->hdr.cmd_hdr );
      (void) rex_clr_sigs( rex_self(), RXC_CMD_Q_SIG );
    }
    else
    {
      (void) rxc_wait( RXC_CMD_Q_SIG );
    }
  }

  return next_state;

}/* rxc_ta */

/*===========================================================================

FUNCTION RXC_TX_TA_SRCH_LOG_UPDATE

DESCRIPTION
  Thi API updates TA durations to SRCH module

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void rxc_tx_ta_srch_log_update( void )
{
  tc_ta_log_type ta_stats_log;
  uint16  sclk2ms,ms2frame;
  uint16  f_fch_ta_frames = 0,f_sch_ta_frames = 0,r_fch_ta_frames = 0,r_sch_ta_frames = 0;

  ta_stats_log.rx_stats.ta_start_sclk      = rxc_ta_info.ta_start_sclk;
  ta_stats_log.rx_stats.f_fch_ta_comp_sclk = rxc_ta_info.f_fch_ta_comp_sclk;
  ta_stats_log.rx_stats.f_sch_ta_comp_sclk = rxc_ta_info.f_sch_ta_comp_sclk;

  ta_stats_log.tx_stats.ta_start_sclk      = txc_ta_info.ta_start_sclk;
  ta_stats_log.tx_stats.r_fch_ta_comp_sclk = txc_ta_info.r_fch_ta_comp_sclk;
  ta_stats_log.tx_stats.r_sch_ta_comp_sclk = txc_ta_info.r_sch_ta_comp_sclk;

  if( rxc_ta_info.f_fch_ta_comp_sclk != 0 )
  {
    sclk2ms  = GET_TIMETICK_MS_FROM_SCLK(
               (rxc_ta_info.f_fch_ta_comp_sclk - rxc_ta_info.ta_start_sclk) );
    ms2frame = ( sclk2ms / 20 );
    f_fch_ta_frames = ms2frame + ( ( sclk2ms % 20 ) ? 1 : 0 );
  }

  if( rxc_ta_info.f_sch_ta_comp_sclk != 0 )
  {
    sclk2ms  = GET_TIMETICK_MS_FROM_SCLK(
               (rxc_ta_info.f_sch_ta_comp_sclk - rxc_ta_info.ta_start_sclk) );
    ms2frame = ( sclk2ms / 20 );
    f_sch_ta_frames = ms2frame + ( ( sclk2ms % 20 ) ? 1 : 0 );
  }

  if( txc_ta_info.r_fch_ta_comp_sclk != 0 )
  {
    sclk2ms  = GET_TIMETICK_MS_FROM_SCLK(
               (txc_ta_info.r_fch_ta_comp_sclk - txc_ta_info.ta_start_sclk) );
    ms2frame = ( sclk2ms / 20 );
    r_fch_ta_frames = ms2frame + ( ( sclk2ms % 20 ) ? 1 : 0 );
  }

  if( txc_ta_info.r_sch_ta_comp_sclk != 0 )
  {
    sclk2ms  = GET_TIMETICK_MS_FROM_SCLK(
               (txc_ta_info.r_sch_ta_comp_sclk - txc_ta_info.ta_start_sclk) );
    ms2frame = ( sclk2ms / 20 );
    r_sch_ta_frames = ms2frame + ( ( sclk2ms % 20 ) ? 1 : 0 );
  }

  M1X_MSG( MUX, LEGACY_HIGH,
    "TA lost frame counts: F-FCH %d F-SCH %d R-FCH %d R-SCH %d", 
     f_fch_ta_frames,f_sch_ta_frames,r_fch_ta_frames,r_sch_ta_frames );

  srchtc_ta_log_params(&ta_stats_log);

} /* rxc_tx_ta_srch_log_update */

byte rxc_sw_beta_sch_conv[NUM_SCH_RC][6] = {
  { /* RC3 */
    0x80, /* 1x,  Walsh length 64 */
    0x5b, /* 2x,  Walsh length 32 */
    0x80, /* 4x,  Walsh length 16 */
    0x5b, /* 8x,  Walsh length  8 */
    0x80, /* 16x, Walsh length  4 */
    0x00  /* unused for RC3 */
  },
  { /* RC4 */
    0x5b, /* 1x,  Walsh length 128 */
    0x80, /* 2x,  Walsh length  64 */
    0x5b, /* 4x,  Walsh length  32 */
    0x80, /* 8x,  Walsh length  16 */
    0x5b, /* 16x, Walsh length   8 */
    0x80  /* 32x, Walsh length   4 */
  },
  { /* RC5 */
    0x80, /* 1x,  Walsh length 64 */
    0x5b, /* 2x,  Walsh length 32 */
    0x80, /* 4x,  Walsh length 16 */
    0x5b, /* 8x,  Walsh length  8 */
    0x80, /* 16x, Walsh length  4 */
    0x00  /* unused for RC5 */
  },
  { /* RC11 */
    0x5b, /* 1x,  Walsh length 128 */
    0x80, /* 2x,  Walsh length  64 */
    0x5b, /* 4x,  Walsh length  32 */
    0x80, /* 8x,  Walsh length  16 */
    0x5b, /* 16x, Walsh length   8 */
    0x80  /* 32x, Walsh length   4 */
  }
};

byte rxc_sw_beta_sch_turbo[NUM_SCH_RC][5] = {
  { /* RC3 */
    0x48, /* 2x */
    0x66, /* 4x */
    0x48, /* 8x */
    0x66, /* 16x */
    0x00  /* unused for RC3 */
  },
  { /* RC4 */
    0x90, /* 2x */
    0x66, /* 4x */
    0x90, /* 8x */
    0x66, /* 16x */
    0x90  /* 32x */
  },
  { /* RC5 */
    0x58, /* 2x */
    0x7c, /* 4x */
    0x58, /* 8x */
    0x7c, /* 16x */
    0x00  /* unused for RC5 */
   },
  { /* RC11 */
    0x90, /* 2x */
    0x66, /* 4x */
    0x90, /* 8x */
    0x66, /* 16x */
    0x90  /* 32x */
  }
};

/*========================================================================

FUNCTION RXC_SET_MAC_RND

DESCRIPTION This function sets the MAC Engine Rounding factors based on the
             spreading Rate.

DEPENDENCIES MAC Engine should be enabled.

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_set_mac_rnd
(
  deint_rc_type rc,
  dec_sch_rate_type sch_rate,
  dec_coding_type code_type
)
{
  muxmdsp_demod_walsh_beta_type walsh_beta;
  muxmdsp_demod_ch2_type ch2;

  /*-----------------------------------------------------------------------*/

  RXC_ENTER_ATOMIC_SECTION();

  if( code_type == DEC_CONVOLUTIONAL )
  {
    /* code_type = CONVOLUTIONAL */
    walsh_beta.swBeta = rxc_sw_beta_sch_conv[DEINT_SCH_RC_INDEX(rc)][sch_rate];
  }
  else
  {
    /* code_type = TURBO */
    walsh_beta.swBeta = rxc_sw_beta_sch_turbo[DEINT_SCH_RC_INDEX(rc)]
                    [((((uint32) sch_rate - 1) > 0) ? ((uint32) sch_rate - 1) : 0) ];
  }

  switch( rc )
  {
    case DEINT_RC3:
    case DEINT_RC5:
    {
      switch( sch_rate )
      {
        case DEC_SCH_RATE_1X:
        {
          ch2.softdecAccumRnd = 0;
          ch2.numSymbols = 384;
          walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_64;
          break;
        }
        case DEC_SCH_RATE_2X:
        {
          ch2.softdecAccumRnd = 1;
          ch2.numSymbols = 384 *2;
          walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_32;
          break;
        }
        case DEC_SCH_RATE_4X:
        {
          ch2.softdecAccumRnd = 1;
          ch2.numSymbols = 384*4;
          walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_16;
          break;
        }
        case DEC_SCH_RATE_8X:
        {
          ch2.softdecAccumRnd = 2;
          ch2.numSymbols = 384*8;
          walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_8;
          break;
        }
        case DEC_SCH_RATE_16X:
        {
          ch2.softdecAccumRnd = 2;
          ch2.numSymbols = 384*16;
          walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_4;
          break;
        }
        default:
          M1X_MSG( MUX, LEGACY_FATAL,
            "Invalid SCH RATE %d, default 1x rate",
            sch_rate );
          ch2.softdecAccumRnd = 0;
          ch2.numSymbols = 384;
          walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_64;
          break;
      }
      break;
    }

    case DEINT_RC4:
    #ifdef FEATURE_IS2000_1X_ADV
    case DEINT_RC11:
    #endif /* FEATURE_IS2000_1X_ADV */
    {
      switch( sch_rate )
      {
        case DEC_SCH_RATE_1X:
        {
          ch2.softdecAccumRnd = 0;
          ch2.numSymbols = 192;
          walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_128;
          break;
        }
        case DEC_SCH_RATE_2X:
        {
          ch2.softdecAccumRnd = 0;
          ch2.numSymbols = 192*2;
          walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_64;
          break;
        }
        case DEC_SCH_RATE_4X:
        {
          ch2.softdecAccumRnd = 1;
          ch2.numSymbols = 192*4;
          walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_32;
          break;
        }
        case DEC_SCH_RATE_8X:
        {
          ch2.softdecAccumRnd = 1;
          ch2.numSymbols = 192*8;
          walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_16;
          break;
        }
        case DEC_SCH_RATE_16X:
        {
          ch2.softdecAccumRnd = 2;
          ch2.numSymbols = 192*16;
          walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_8;
          break;
        }
        case DEC_SCH_RATE_32X:
        {
          ch2.softdecAccumRnd = 2;
          ch2.numSymbols = 192*32;
          walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_4;
          break;
        }

        default:
          M1X_MSG( MUX, LEGACY_FATAL,
            "Invalid SCH Rate %d, default 1x",
            sch_rate );
          ch2.softdecAccumRnd = 0;
          ch2.numSymbols = 192;
          walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_128;
          break;
      }
      break;
    }

    default:
      M1X_MSG( MUX, LEGACY_FATAL,
        "Invalid RC %d, default RC3, 1x rate ",
        rc );
      ch2.softdecAccumRnd = 0;
      ch2.numSymbols = 384;
      walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_64;
      break;
  }

  muxmdsp_update_demod_walsh_beta_cfg( MUXMDSP_DEMOD_CH2, &walsh_beta );

  ch2.combEnable       = TRUE;
  ch2.softdecRnd       = code_type;
  ch2.ffeBias          = rxc_rnd_factor_tab[DEINT_SCH_RC_INDEX(rc)]
                          [RXC_UPPER_BOUNDS_CHECK(sch_rate, NUM_RND_FACT)];
  ch2.fpcSchIqAccScale = ffpc_get_iqacc_scale();
  muxmdsp_update_demod_ch2( &ch2 );

  RXC_LEAVE_ATOMIC_SECTION();

} /* rxc_set_mac_rnd */

/*========================================================================

FUNCTION RXC_DISABLE_CH2

DESCRIPTION This function disables ch2

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_disable_ch2( void )
{
  muxmdsp_demod_walsh_beta_type walsh_beta = { 0 };
  muxmdsp_demod_ch2_type        ch2        = { { 0 } };

  /*-----------------------------------------------------------------------*/

  muxmdsp_update_demod_walsh_beta_cfg( MUXMDSP_DEMOD_CH2, &walsh_beta );
  muxmdsp_update_demod_ch2( &ch2 );

} /* rxc_disable_ch2 */

/*========================================================================

FUNCTION RXC_DISABLE_CH1

DESCRIPTION This function disables ch1

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_disable_ch1( void )
{
  muxmdsp_demod_walsh_beta_type walsh_beta = { 0 };
  muxmdsp_demod_ch1_type        ch1        = { { 0 } };

  /*-----------------------------------------------------------------------*/

  muxmdsp_update_demod_walsh_beta_cfg( MUXMDSP_DEMOD_CH1, &walsh_beta );
  muxmdsp_update_demod_ch1( &ch1 );

  /* Do we need to check if SCH is enabled when shutting down data bridge */

  /* FW needs to be informed about clearing data bridge for CH1 as well on
     bolt */
  #if defined( FEATURE_MODEM_1X_SW_DEMBACK ) && !defined( FEATURE_MODEM_1X_ATLAS_REVISIT )
  /* Dont inform FW if SW DEMBACK is in use */
  if( !muxmdsp_get_is_sw_demback_active() )
  #endif /* FEATURE_MODEM_1X_SW_DEMBACK && !FEATURE_MODEM_1X_ATLAS_REVISIT */
  {
    muxmdsp_data_brdg_enable( FALSE, FALSE );
  }

} /* rxc_disable_ch1 */

/*========================================================================

FUNCTION RXC_DISABLE_CH0

DESCRIPTION This function disables ch0

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_disable_ch0( void )
{
  muxmdsp_demod_walsh_beta_type walsh_beta = { 0 };
  muxmdsp_demod_ch0_type        ch0        = { { 0 } };

  /*-----------------------------------------------------------------------*/

  muxmdsp_update_demod_walsh_beta_cfg( MUXMDSP_DEMOD_CH0, &walsh_beta );
  muxmdsp_update_demod_ch0( &ch0 );

} /* rxc_disable_ch0 */

#ifdef FEATURE_IS2000_REL_A_SVD
/*=========================================================================
FUNCTION rxc_delete_fch_dcch_task_list

DESCRIPTION
    This function deletes the FCH and DCCH tasks from the deinterleaver task
    RAM and clears data_valid flag for both channels.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void rxc_delete_fch_dcch_task_list( void )
{
  deint_delete_fch_dcch_task_list();

  rxc_so.for_dcch.data_valid_countdown = 0;
  rxc_so.for_dcch.data_valid = FALSE;

  rxc_so.for_fch.data_valid_countdown = 0;
  rxc_so.for_fch.data_valid = FALSE;

} /* end rxc_delete_fch_dcch_task_list */

/*=========================================================================
FUNCTION FFPC_TRANS_CMD

DESCRIPTION
  This function handles MCTRANS_FFPC_ACTION command fro MC.

DEPENDENCIE
  None.

RETURN VALUE

SIDE EFFECTS
  None.
=========================================================================*/
void rxc_ffpc_trans_cmd
(
  mctrans_action_packet_type *cmd_ptr
)
{
  mctrans_ffpc_action_type *ffpc_ptr;

  if( cmd_ptr )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_FFPC_ACTION" );

    ffpc_ptr = &cmd_ptr->action.ffpc;

    /* Check for change in primary power control channel */
    if( ffpc_ptr->fpc_pri_chan.included )
    {
      if( ffpc_channel != ffpc_ptr->fpc_pri_chan.value )
      {
        /* Initialize frame counts for reception watchdog */
       rxc_fade_data.rxc_t5m_bad_frames = 0;
       rxc_fade_data.rxc_n2m_bad_frames = 0;
       rxc_fade_data.rxc_good_frames = 0;
       rxc_fade_data.rxc_good_guaranteed_frames = 0;
       rxc_fade_data.rxc_fade = FALSE;
       rxc_fade_data.rxc_sent_bad_rpt = FALSE;
       rxc_fade_data.rxc_sent_pwron_rpt = FALSE;      /* used in hard handoff */

        /* Delete the current task list then set up the new task
        ** list with the appropriate channels.
        */
        if( ( rxc_so.for_fch.included == TRUE ) &&
            ( rxc_so.for_dcch.included == TRUE ) )
        {
          rxc_delete_fch_dcch_task_list ();
          ffpc_channel   = (cai_fpc_pri_chan_type) ffpc_ptr->fpc_pri_chan.value;
          if( ffpc_channel == CAI_FPC_PRI_CHAN_FCH )
          {
            M1X_MSG( MUX, LEGACY_HIGH,
               "Switch FFPC ch to FCH" );
            rxc_config_demod_for_traffic( (deint_rc_type) rxc_so.for_fch.rc );
            rxc_config_demod_ch1( (deint_rc_type) rxc_so.for_dcch.rc );
          }
          else
          {
            M1X_MSG( MUX, LEGACY_HIGH,
               "Switch FFPC ch to DCCH");
            rxc_config_demod_ch1 ( (deint_rc_type)rxc_so.for_fch.rc );
            rxc_config_demod_for_traffic ( (deint_rc_type)rxc_so.for_dcch.rc );
          }
          rxc_set_dcch_mode( (deint_rc_type) rxc_so.for_dcch.rc,
                             ( ffpc_channel == CAI_FPC_PRI_CHAN_DCCH ) );
          rxc_set_fch_mode( (deint_rc_type)rxc_so.for_fch.rc,
                            ( ffpc_channel == CAI_FPC_PRI_CHAN_FCH ) );
        }
        else if( ( rxc_so.for_fch.included == TRUE ) &&
                 ( rxc_so.for_dcch.included == FALSE ) &&
                 ( ffpc_ptr->fpc_pri_chan.value == CAI_FPC_PRI_CHAN_FCH ) )
       {
          rxc_delete_fch_dcch_task_list();
          ffpc_channel = CAI_FPC_PRI_CHAN_FCH;
          rxc_config_demod_for_traffic ( (deint_rc_type)rxc_so.for_fch.rc );
          rxc_set_fch_mode( (deint_rc_type)rxc_so.for_fch.rc, TRUE );
          M1X_MSG( MUX, LEGACY_HIGH,
             "Switch FFPC ch to FCH,no DCCH");
        }
        else if( ( rxc_so.for_fch.included == FALSE ) &&
                 ( rxc_so.for_dcch.included == TRUE ) &&
                 ( ffpc_ptr->fpc_pri_chan.value == CAI_FPC_PRI_CHAN_DCCH ) )
        {
          rxc_delete_fch_dcch_task_list();
          ffpc_channel = CAI_FPC_PRI_CHAN_DCCH;
          rxc_config_demod_for_traffic( (deint_rc_type)rxc_so.for_dcch.rc );
          rxc_set_dcch_mode( (deint_rc_type)rxc_so.for_dcch.rc, TRUE );
          M1X_MSG( MUX, LEGACY_HIGH,
             "Switch FFPC ch to DCCH,no FCH" );
        }
        else
        {
          M1X_MSG( MUX, LEGACY_ERROR,
             "Unexpected FFPC ch switch %d,%d,%d",
             ffpc_ptr->fpc_pri_chan.value,
             rxc_so.for_fch.included,
             rxc_so.for_dcch.included );
        }
      }
    }

    /* Call ffpc_trans_cmd() to process FFPC transaction */
    ffpc_trans_cmd( cmd_ptr );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_FFPC_ACTION received" );
  }
} /* rxc_ffpc_trans_cmd */
#endif /* FEATURE_IS2000_REL_A_SVD */

/*========================================================================
FUNCTION RXC_CONFIG_SCH_HW

DESCRIPTION
   This function sets up hardware for F-SCH processing.

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void rxc_config_sch_hw( mctrans_rxc_f_sch_action_type *fwd_sch_packet )
{
  m1x_stmr_err_type err = 0;

  /*-------------------------------------------------------------------*/

  RXC_ENTER_ATOMIC_SECTION();

  rxc_set_mac_rnd
  (
    (deint_rc_type) ((dec_rc_type) fwd_sch_packet->rc),
    (dec_sch_rate_type)fwd_sch_packet->sch_rate,
    (dec_coding_type)fwd_sch_packet->code_type
  );
  dec_dtx_thresh_init
  (
    (dec_rc_type) fwd_sch_packet->rc,
    (dec_coding_type)fwd_sch_packet->code_type,
    (dec_sch_rate_type)fwd_sch_packet->sch_rate
  );

  #ifdef FEATURE_IS2000_REL_A //32X
  deint_set_sch_mode
  (
    fwd_sch_packet->num_ltu,
    fwd_sch_packet->ltu_len,
    (dec_mux_pdu_type_type) fwd_sch_packet->mux_pdu_type,
    (deint_rc_type) ((dec_rc_type) fwd_sch_packet->rc),
    (dec_coding_type)fwd_sch_packet->code_type,
    (dec_sch_rate_type)fwd_sch_packet->sch_rate
  );
  /* Initialize variables used in 2nd ATMR ISR */
  deint_atmr_isr2_init();

  /* Set 2nd ATMR ISR to go off at the 4th bin */
  err = m1x_stmr_register(M1X_STMR_CLIENT_RX, M1X_STMR_PCG_12,rxc_atmr_isr2);
  if(err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_REG_DUP)
  {
    ONEX_ERR_FATAL("Unable to schedule the event with err: %d", err, 0, 0);
  }
  #else
  deint_set_sch_mode
  (
    (deint_rc_type) ((dec_rc_type) fwd_sch_packet->rc),
    (dec_coding_type)fwd_sch_packet->code_type,
    (dec_sch_rate_type)fwd_sch_packet->sch_rate
  );
  #endif /* FEATURE_IS2000_REL_A */

  RXC_LEAVE_ATOMIC_SECTION();

} /* rxc_config_sch_hw */

/*========================================================================
FUNCTION RXC_FWD_SCH_TRANS_CMD

DESCRIPTION
   The fwd. SCH transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void rxc_fwd_sch_trans_cmd( mctrans_action_packet_type *fwd_sch_packet )
{
  if( fwd_sch_packet != NULL )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_F_SCH_ACTION: start %d, rate %d, turbo %d",
      fwd_sch_packet->action.rxc_f_sch.start,
      fwd_sch_packet->action.rxc_f_sch.sch_rate,
      fwd_sch_packet->action.rxc_f_sch.code_type );

    if( fwd_sch_packet->action.rxc_f_sch.start )
    {
      rxc_log_sch_data.rxc_stored_sch_packet = fwd_sch_packet->action.rxc_f_sch;
      if( (deint_rc_type) rxc_sch.rc != DEINT_RC_INVALID )
      // This is one of the 3 scenarios:
      // 1. Overlapping
      // 2  back-to-back
      // 3. Two assignments with 1 frame gap in between
      {
        /* For overlapping SCH assignments defer updating of
         * struct rxc_sch until the last frame of the previous burst
         * is processed by RLP. RLP will start processing the first
         * frame of the new burst 3 SCH interrupts from now.
         *
         * It is assumed that FCH interrupt occurs before SYS_TIME_INT2.
         * SYS_TIME_INT2 is currently programmed to trigger 5ms after the
         * start of a 20ms frame. Also SYS_TIME_INT2 is assumed to happen
         * before SCH interrupt.
         * When this no longer holds the counter value should be adjusted.
         */

        if ( rxc_sch_shutdown_state == RXC_SCH_SHUTDOWN_NOW )
        {
          // This is scenario 3:
          // There is 1 frame gap between bursts.  In that case, we are shutting
          // down SCH and starting in the same frame.  Unfortunately, ATMR ISR
          // happens before the decoder ISR, so we need to defer the F-SCH start
          // until after the shutdown (inside decoder ISR).
          rxc_sch_report_data.rxc_sch_update_now = TRUE;
          rxc_sch_report_data.rxc_sch_update_counter = 0;
          rxc_log_sch_data.rxc_log_ffpc_sch_data = TRUE;
        }
        else
        {
          if ( rxc_sch_shutdown_state == RXC_SCH_SHUTDOWN_ARMED )
          {
            // This is scenario 2:
            // This happens when stop and start time are the same.  The "stop" was just
            // set up and then the "start" is now schedule in the same ATMR interrupt.
            // In this case, we need to cancel the "stop".
            // We also need to do whatever is needed for scenario 1 below.
            rxc_sch_shutdown_state = RXC_SCH_SHUTDOWN_INACTIVE;
            rxc_log_sch_data.rxc_log_ffpc_sch_data = TRUE;
          }
          // This is scenario 1: overlapping bursts
          // Start sch_update counter for countdown to update SCH...
          rxc_sch_report_data.rxc_sch_update_counter = 3;

          /*On BOLT & derivative targets, Turbo/Conv decoder config is
            instantaneous by writing directly to HW contrary to DIME
            based targets where config is done through FW messaging */
          /*On Back to Back SCH assignments, above limitation results in
            single frame SCH erasure */
          /* So,On BOLT based targets if there is no change in SCH config
             then SKIP the HW config part */
          if ( !rxc_to_skip_sch_trans (  &fwd_sch_packet->action.rxc_f_sch ) )
          {
            //... and config HW for SCH
            rxc_config_sch_hw( &fwd_sch_packet->action.rxc_f_sch );
          }
        }
      }
      else
      {
        /* New SCH assignment - not an overlapping burst. Update SCH info */
        rxc_sch_report_data.rxc_sch_update_counter = 0;
        rxc_log_sch_data.rxc_log_ffpc_sch_data = TRUE;
        rxc_update_sch_info( &fwd_sch_packet->action.rxc_f_sch );
        rxc_config_sch_hw( &fwd_sch_packet->action.rxc_f_sch );
      }

      /* Log LTA F-SCH time here */
      if( rxc_ta_info.f_sch_ta_log )
      {
        rxc_ta_info.f_sch_ta_comp_sclk = timetick_get();
        rxc_ta_info.f_sch_ta_log       = FALSE;

           /* Log to SRCH, if F-FCH & R-FCH already logged and
             if RSCH is not going to be assigned or RSCH log is already done */
        if( ( ( rxc_ta_info.f_fch_ta_log == FALSE ) && 
              ( txc_ta_info.r_fch_ta_log == FALSE ) ) &&
            ( ( !mccsch_sch_is_active(MCCSCH_REVERSE, 0) ) ||
              ( txc_ta_info.r_sch_ta_log == FALSE ) ) )
        {
          /* Update SRCH here about the stats */
          rxc_tx_ta_srch_log_update();
        }
      }
    }
    else
    {
      //
      // RXC_SCH_SHUTDOWN
      //
      // Shutdown SCH, but after the current frame is decoded
      rxc_sch_shutdown_state = RXC_SCH_SHUTDOWN_ARMED;
      rxc_log_sch_data.rxc_log_ffpc_sch_data = FALSE;
      if( fwd_sch_packet->action.rxc_f_sch.for_sch_fer_rep )
      {
         rxc_sch_report_data.rxc_sch_rep = TRUE;
         rxc_sch_report_data.rxc_sch_id = fwd_sch_packet->action.rxc_f_sch.sch_id;
      }
      else
      {
         rxc_sch_report_data.rxc_sch_rep = FALSE;
      }
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL FFPC action packet received" );
  }

} /* rxc_fwd_sch_trans_cmd */

/*========================================================================
FUNCTION RXC_CONTROL_HOLD_TRANS_CMD

DESCRIPTION
   The control hold transaction callback.

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void rxc_control_hold_trans_cmd( mctrans_action_packet_type *chs_packet )
{
  uint8 dem_gating_mask = DEM_FULL_RATE_GATING_MASK; // Default no gating

  if ( chs_packet != NULL )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_CTRL_HLD_GATING_ACTION: rate %d, delay %d",
      chs_packet->action.rxc_ctrl_hld_gating.gating_rate,
      chs_packet->action.rxc_ctrl_hld_gating.rev_pwr_control_delay );

    // Set reverse power control delay
    muxmdsp_set_agc_ctl_mask_delay(
              chs_packet->action.rxc_ctrl_hld_gating.rev_pwr_control_delay );

    // Now set gating mask
    if ( chs_packet->action.rxc_ctrl_hld_gating.gating_rate == CAI_GATING_RATE_QUARTER )
    {
      dem_gating_mask = DEM_QUARTER_RATE_GATING_MASK;
    }
    else if (chs_packet->action.rxc_ctrl_hld_gating.gating_rate == CAI_GATING_RATE_HALF )
    {
      dem_gating_mask = DEM_HALF_RATE_GATING_MASK;
    }

    MODEM_1X_UNUSED( dem_gating_mask );

    /*
       Notify RDA of the gating rate.  This affects energy accumulation for the PCGs on
       F-power control subchannel.
    */
    rxcrda_update_gating_rate( chs_packet->action.rxc_ctrl_hld_gating.gating_rate );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_CTRL_HLD_GATING_ACTION received" );
  }
} /* rxc_control_hold_trans_cmd */

/*========================================================================
FUNCTION RXC_HO_TRANS_CMD

DESCRIPTION
  Process Handoff transaction request from MC.

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void rxc_ho_trans_cmd
(
  mctrans_action_packet_type *cmd_ptr
)
{
  #ifdef FEATURE_IS2000_REL_A
  qword current_frame_time = {0};
  #endif /* FEATURE_IS2000_REL_A */

  mctrans_rxc_ho_action_type *ho = &cmd_ptr->action.rxc_ho;

  if( cmd_ptr != NULL )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_HO_ACTION: HHO %d, RIF %d, reset_fpc %d",
      ho->hho_ind,
      ho->rif_ind,
      ho->reset_fpc);

    #ifdef FEATURE_IS2000_REL_A
    /* find current time */
    m1x_time_get_20ms_frame_offset_time (current_frame_time);
    qw_inc (current_frame_time, 1L);
    qw_equ(rxc_frame_data.rxc_tc_var.p_rev_action_frame, current_frame_time);
    #endif /* FEATURE_IS2000_REL_A */

    /* rxc_ignore_good_frame is set to TRUE here to ignore any good
     * frames that are received during CFS or before verifying all the
     * handoff failure conditions for RIF HHO.  For RIF HHO, after
     * we've verified that none of the handoff failure conditions are
     * met, this flag will be set to FALSE (by SRCH) so that we can
     * start looking for that one good frame to declare a successful HHO.
     */
    if( ho->hho_ind && ho->rif_ind )
    {
      /* Prepare for RIFHHO/CFS/GPS , FALSE inidicates this is not for GPS */
      rxc_prep_for_rifhho_cfs_gps( FALSE );
    }

    if( !ho->rif_ind )
    {
      /* Soft Handoff or Hard Handoff with No Return-If-Failure */
      if( ho->reset_fpc )
      {
        rxc_pwr.num_frames                = 0;
        rxc_pwr.skipped_num_frames        = 0;
        rxc_pwr.num_bad_frames            = 0;
        #ifdef FEATURE_IS2000_REL_A_SVD
        rxc_pwr.dcch_num_frames           = 0;
        rxc_pwr.skipped_dcch_num_frames   = 0;
        rxc_pwr.dcch_num_bad_frames       = 0;
        #endif /* FEATURE_IS2000_REL_A_SVD */
        rxc_pwr.sch_tot_frames[0]         = 0;
        rxc_pwr.skipped_sch_tot_frames[0] = 0;
        rxc_pwr.sch_bad_frames[0]         = 0;
        rxc_pwr.sch_fer_rep[0]            = FALSE;
        rxc_pwr.sch_tot_frames[1]         = 0;
        rxc_pwr.skipped_sch_tot_frames[1] = 0;
        rxc_pwr.sch_bad_frames[1]         = 0;
        rxc_pwr.sch_fer_rep[1]            = FALSE;
      }
    }

     /* program the delay of TX generated MASK_DATA to RX PCG timing */
    if( ho->rev_pwr_cntl_delay.included )
    {
      muxmdsp_set_agc_ctl_mask_delay( ho->rev_pwr_cntl_delay.value );
    }

    if( ho->hho_ind )
    {
       /* Since we had a hard handoff, terminate FFPC log */
       RXC_ENTER_CRITICAL_SECTION();
       ffpc_send_log ();
       RXC_LEAVE_CRITICAL_SECTION();
    }

     /* If enabled, terminate the accumulated frame types log. */
    dsrlp_log_rx_send_frames();

    #ifdef FEATURE_IS2000_REL_A
    if( rxc_frame_data.rxc_tc_var.p_rev_in_use != ho->p_rev_in_use )
    {
      // At the action frame, RXC should use the new P_REV to
      // assemble signaling messages. But now it is not the
      // right time. Considering there may be one, or more
      // decoder interrupts depending on SCH is enabled
      // (maybe some other channels in the future), it is easier
      // to allow RXC to check a flag before building a frame.
      // If the flag is set, the RXC should change the P_REV_IN_USE
      // when the action frame is arrived.

      rxc_frame_data.rxc_tc_var.in_p_rev_transition = TRUE;

      // rxc_tc_var.p_rev_action_frame is already set before RXC calls
      // mctrans_process_action, which in turn calls this function.

      rxc_frame_data.rxc_tc_var.temp_p_rev_in_use = ho->p_rev_in_use;

    }
    #endif /* FEATURE_IS2000_REL_A */
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_HO_ACTION received" );
  }

} /* rxc_ho_trans_cmd */

/*===========================================================================

FUNCTION RXC_FREE_TC_RESOURCES

DESCRIPTION
  This function resets the MAC Engine and resets the deinteleaver/decoder.
  Also frees up the action timer resource and resets the sch mux stats.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_free_tc_resources( void )
{
  uint32 i = 0;
  int err  = 0;
  boolean rxc_ta_is_enabled = rxc_is_ta_enabled();

  /*-----------------------------------------------------------------*/

  #ifdef FEATURE_IS2000_1X_ADV
  /* Clear ATMR decoder interupt. */
  err = m1x_stmr_deregister( M1X_STMR_CLIENT_RX, M1X_STMR_PCG_5 );
  if( err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_UNREG_EVENT )
  {
    ONEX_ERR_FATAL( "Unexpected m1x_stmr driver behaviour with error: %d", err, 0, 0 );
  }

  RXC_ENTER_ATOMIC_SECTION();
  /* Turn off RC11 processing in the MDSP. */
  muxmdsp_set_rc11_mode( FALSE );

  /* Clear source MDSP ISR to be safe. */
  rxc_traffic_status.rxc_rc11.tc_source_mdsp_isr = FALSE;
  RXC_LEAVE_ATOMIC_SECTION();

  /* Notify SRCH that we have left 1x Adv traffic */
  srch_mux_notify_1x_adv_traffic( FALSE );

  /*Disable mDSP 1xA Logging and clean up buffer.
    This also disables the 1xA logging interrupt if it was on previously.*/
  rxc_update_1xA_logging_status( FALSE );

  /* Free the dynamically allocated mdsp buffer memory */
  for( i = 0; i < RX_TX_ONE_FRAME_OFFSET_CORRECTION; i++ )
  {
    if( log_buff.mdsp_buf[i] != NULL )
    {
      modem_mem_free( log_buff.mdsp_buf[i], MODEM_MEM_CLIENT_1X );
      log_buff.mdsp_buf[i] = NULL;
    }
  }
  memory_is_allocated_for_logging = FALSE;
  #endif /* FEATURE_IS2000_1X_ADV */

  /* deregister the RX client and all associated events */
  m1x_stmr_deregister_client( M1X_STMR_CLIENT_RX );

  RXC_ENTER_ISR_SECTION();

  // RXC_SCH_SHUTDOWN
  rxc_sch_shutdown_state = RXC_SCH_SHUTDOWN_INACTIVE;
  rxc_log_sch_data.rxc_log_ffpc_sch_data = FALSE;
  rxc_sch_report_data.rxc_sch_rep = FALSE;
  rxc_sch_report_data.rxc_sch_id = 0;

  if( onex_nonsig_is_ftm_mode() )
  {
    enc_set_frame_off( 0x00, 0x00 );
  }

  dsrlp_log_rlp_statistics( DSRLP_FIXED_SR_ID_INDEX, TRUE );

  rxc_set_voc_config_done( FALSE );
  rxc_sup_dec_int_enable             = FALSE;
  rxc_turbo_dec_enabled              = FALSE;
  rxc_set_mvs_status_on_tc_entry( FALSE );
  rxc_disable_ch0();
  rxc_disable_ch1();
  rxc_disable_ch2();

  deint_delete_supplemental_task_list();

  if( !rxc_ta_is_enabled )
  {
    /* Clean UP SCH buffers only outside LTA GAP */
    rxc_cleanup_sch_mux();
  }
  else
  {
    /* Take back up of OLPC params for LTA case for later resumption */
    rxc_ta_info.fch_olpc_enabled  = ffpc_fch_params.olpc_enabled;
  }

  RXC_LEAVE_ISR_SECTION();
  ffpc_disable ();

  /* Clearing Standby related variables on call end */
  rxc_mvs_set_standby_progress_status( FALSE );
  rxc_mvs_set_standby_status( MVS_STANDBY_FAIL );
  rxc_mvs_set_is_so_change( FALSE );

  #ifndef FEATURE_IS2000_REL_A
  /* flush out leftover log records. Note that this is done inspite of the
  ** fact that FORWARD FRAME type and FFPC logging may be turned off at this
  ** time. If the logs are not flushed, the logging info from this call will
  ** appear in the logs of the subsequent call.
  */
  rxc_frame_types_send_log();
  #else
  rxc_fdch_frame_info_send_log();
  #endif /* FEATURE_IS2000_REL_A */

  RXC_ENTER_CRITICAL_SECTION();
  ffpc_send_log();
  RXC_LEAVE_CRITICAL_SECTION();

  /* If enabled, send accumulated RDA log. */
  rxc_rda_frame_info_send_log();

  /* Revert gating mask to no gating */
  rxcrda_update_gating_rate( CAI_GATING_RATE_NONE );

  #ifdef FEATURE_IS2000_REL_A_SVD
  /* Delete the current task list, reset fundicated channel deinterleaver
  ** state.
  */
  rxc_delete_fch_dcch_task_list();
  #endif /* FEATURE_IS2000_REL_A_SVD */

  dec_config_clk_resources( DEC_CLK_IDLE );

} /* rxc_free_tc_resources */

/*===========================================================================

FUNCTION RXC_FCH_LOOPBACK_QUERY

DESCRIPTION
  This procedure returns whether FCH loopback is enabled for Factory
  Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean, true if loobpack is on.

SIDE EFFECTS
  None.

===========================================================================*/
boolean rxc_fch_loopback_query( void )
{
  return rxc_frame_data.rxc_tc_var.fch_loopback;
}

/* Support FEATURE_FTM_DCCH */
/*===========================================================================

FUNCTION RXC_DCCH_LOOPBACK_QUERY

DESCRIPTION
  This procedure returns whether DCCH loopback is enabled for Factory
  Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean, true if loobpack is on.

SIDE EFFECTS
  None.

===========================================================================*/
boolean rxc_dcch_loopback_query( void )
{
  return rxc_frame_data.rxc_tc_var.dcch_loopback;
}
/* End of FEATURE_FTM_DCCH */

/*===========================================================================

FUNCTION RXC_SCH_LOOPBACK_QUERY

DESCRIPTION
  This procedure returns whether SCH loopback is enabled for Factory
  Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean, true if loobpack is on.

SIDE EFFECTS
  None.

===========================================================================*/
boolean rxc_sch_loopback_query( void )
{
  return rxc_frame_data.rxc_tc_var.sch_loopback;
}

/*===========================================================================

FUNCTION RXC_SET_FCH_LOOPBACK

DESCRIPTION
  This procedure sets FCH loopback for Factory Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Loopback mode is set in rxctraffic.

===========================================================================*/
void rxc_set_fch_loopback( boolean set )
{
  rxc_frame_data.rxc_tc_var.fch_loopback = set;
}

/* Support FEATURE_FTM_DCCH */
/*===========================================================================

FUNCTION RXC_SET_DCCH_LOOPBACK

DESCRIPTION
  This procedure sets DCCH loopback for Factory Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Loopback mode is set in rxctraffic.

===========================================================================*/
void rxc_set_dcch_loopback( boolean set )
{
  rxc_frame_data.rxc_tc_var.dcch_loopback = set;
}
/* End of FEATURE_FTM_DCCH */

/*===========================================================================

FUNCTION RXC_SET_SCH_LOOPBACK

DESCRIPTION
  This procedure sets SCH loopback for Factory Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Loopback mode is set in rxctraffic.

===========================================================================*/
void rxc_set_sch_loopback( boolean set )
{
  rxc_frame_data.rxc_tc_var.sch_loopback = set;
}

/*========================================================================
FUNCTION RXC_SCR_TRANS_CMD

DESCRIPTION
  Process SCR transaction request from MC.

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
void rxc_scr_trans_cmd
(
  mctrans_action_packet_type *cmd_ptr
)
{
  mctrans_rxc_scr_action_type *scr = &cmd_ptr->action.rxc_scr;
  #ifdef FEATURE_IS2000_REL_A
  word so_index;
  #endif /* FEATURE_IS2000_REL_A */
  #ifdef FEATURE_IS2000_1X_ADV
  deint_rc_type prev_for_fch_rc;
  #endif /* FEATURE_IS2000_1X_ADV */
  int err = 0;

  /*------------------------------------------------------------------*/

  if( cmd_ptr )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_SCR_ACTION" );

    #ifdef FEATURE_IS2000_1X_ADV
    /* Save previous RC to detect change. */
    prev_for_fch_rc = (deint_rc_type) rxc_so.for_fch.rc;
    #endif /* FEATURE_IS2000_1X_ADV */

    if( !scr->dcch.included && !scr->fch.included )
    {  /* Neither is included, turn it off */
      rxc_free_tc_resources();
    }

    #ifdef FEATURE_IS2000_1X_ADV
    /* For 1x Advanced DCCH is not supported R-FCH. If it is
       included at the same time that R-FCH is then we must
       confirm that R-FCH RC and R-DCCH are RC11. */
    if( scr->dcch.included )
    {
      if( ( scr->dcch.rc == CAI_RC_11 ) ||
          ( ( scr->fch.included ) &&
            ( scr->fch.rc == CAI_RC_11 ) ) )
      {
        M1X_MSG( MUX, LEGACY_FATAL,
          "Illegal channel configuration: FCH-> inc=%d, rc=%d DCCH-> rc=%d",
          scr->fch.included,
          scr->fch.rc,
          scr->dcch.rc );
      }
    }
    #endif /* FEATURE_IS2000_1X_ADV */

    if( scr->dcch.included )
    {
      // If DCCH is newly added or changing RC
      if( !rxc_so.for_dcch.included ||
          scr->dcch.rc != rxc_so.for_dcch.rc )
      {
        rxc_so.for_dcch.included = TRUE;
        rxc_so.for_dcch.rc = scr->dcch.rc;
        #ifdef FEATURE_IS2000_REL_A_SVD
        if( ffpc_channel == CAI_FPC_PRI_CHAN_DCCH )
        {
          rxc_config_demod_for_traffic ( (deint_rc_type)rxc_so.for_dcch.rc );
          rxc_set_dcch_mode( (deint_rc_type)rxc_so.for_dcch.rc, TRUE);
        }
        else
        {
          rxc_config_demod_ch1 ( (deint_rc_type)rxc_so.for_dcch.rc );
          rxc_set_dcch_mode( (deint_rc_type)rxc_so.for_dcch.rc, FALSE );
        }
        #else
        rxc_config_demod_for_traffic ( (deint_rc_type)rxc_so.for_dcch.rc );
        deint_set_fch_mode ( (deint_rc_type)rxc_so.for_dcch.rc );

        #endif /* FEATURE_IS2000_REL_A_SVD */
        /* For SRCH logging */
        // Note: we know that FCH is not included since DCCH included
        // We assume only 1 of FCH/DCCH supported here
        srch_mux_update_rc( DEINT_RC_INVALID,
                            (deint_rc_type)rxc_sch.rc,
                            DEINT_RC_INVALID,
                            (deint_rc_type)rxc_so.for_dcch.rc );
      }
      rxc_so.rate_set = rxc_get_fwd_link_rate_set( rxc_so.for_dcch.rc );
      rxc_so.high_rc = TRUE;
    }
    else
    {
      rxc_so.for_dcch.included = FALSE;
      #ifdef FEATURE_IS2000_REL_A_SVD
      /* Disables the processing of the DCCH */
      rxc_so.for_dcch.data_valid = FALSE;
      rxc_so.for_dcch.data_valid_countdown = 0;
      #endif /* FEATURE_IS2000_REL_A_SVD */
    }

    #ifdef FEATURE_IS2000_REL_A
    /* Copy the SCH MUX OPTION from the upper layer to the MUX for
       FDCH logging */
    if( scr->for_sch0_mux_option.included )
    {
      rxc_so.for_sch_mux_option = scr->for_sch0_mux_option.value;
    }
    else
    {
      rxc_so.for_sch_mux_option = 0;
    }
    #endif /* FEATURE_IS2000_REL_A */

    if( scr->fch.included )
    {
      // If FCH is newly added or changing RC
      if( !rxc_so.for_fch.included ||
          scr->fch.rc != rxc_so.for_fch.rc )
      {
        #ifdef FEATURE_MODEM_1X_DRX
        /* If RC has changed then disable DRX */
        if( scr->fch.rc != rxc_so.for_fch.rc )
        {
          rxc_mux_rc_changed = TRUE;
          rxc_disable_drx_rc_change =
          ( ( scr->fch.rc == CAI_RC_3 ) || ( scr->fch.rc == CAI_RC_11 ) )
          ? FALSE : TRUE;

          M1X_MSG( MUX,LEGACY_HIGH,
          "DRX: RC has Changed from %d to %d, Disable DrX = %d",
          rxc_so.for_fch.rc,
          scr->fch.rc,
          rxc_disable_drx_rc_change );
        }
        #endif /* FEATURE_MODEM_1X_DRX */

        rxc_so.for_fch.included = TRUE;
        rxc_so.for_fch.rc = scr->fch.rc;
        #ifdef FEATURE_IS2000_REL_A_SVD
        if( ffpc_channel == CAI_FPC_PRI_CHAN_FCH )
        {
          rxc_config_demod_for_traffic( (deint_rc_type) rxc_so.for_fch.rc );
          rxc_set_fch_mode( (deint_rc_type) rxc_so.for_fch.rc, TRUE );
        }
        else
        {
          rxc_config_demod_ch1( (deint_rc_type) rxc_so.for_fch.rc );
          rxc_set_fch_mode( (deint_rc_type) rxc_so.for_fch.rc, FALSE );
        }
        #else
        rxc_config_demod_for_traffic( (deint_rc_type) scr->fch.rc );
        deint_set_fch_mode( (deint_rc_type) scr->fch.rc );
        #endif /* FEATURE_IS2000_REL_A_SVD */

        /* For SRCH logging */
        // Note: we know that DCCH is not included since FCH included
        // We assume only 1 of FCH/DCCH supported here
        srch_mux_update_rc( (deint_rc_type)scr->fch.rc,
                            (deint_rc_type)rxc_sch.rc,
                            DEINT_RC_INVALID,
                            DEINT_RC_INVALID );

        #ifdef FEATURE_IS2000_1X_ADV
        if( scr->fch.rc == CAI_RC_11 )
        {
          /* Notify SRCH that we are entering 1x Adv traffic */
          srch_mux_notify_1x_adv_traffic( TRUE );
        }
        #endif /* FEATURE_IS2000_1X_ADV */
      }
      rxc_so.rate_set = rxc_get_fwd_link_rate_set( scr->fch.rc );
      rxc_so.high_rc = rxc_so.for_fch.rc > CAI_RC_2;

      #ifdef FEATURE_MODEM_1X_DRX
      /* Enable DRX feature */
      rxc_serv_conn_rxed = TRUE;
      #endif /* FEATURE_MODEM_1X_DRX */
    }
    else
    {
      rxc_so.for_fch.included = FALSE;

      #ifdef FEATURE_IS2000_1X_ADV
      /* Notify SRCH that we have left 1x Adv traffic */
      srch_mux_notify_1x_adv_traffic( FALSE );
      #endif /* FEATURE_IS2000_1X_ADV */

      #ifdef FEATURE_IS2000_REL_A_SVD
      /* Disables the processing of the FCH.
      */
      rxc_so.for_fch.data_valid = FALSE;
      rxc_so.for_fch.data_valid_countdown = 0;
      #endif /* FEATURE_IS2000_REL_A_SVD */
    }

    #ifndef FEATURE_IS2000_REL_A
    if( ( scr->pri_so.included ) && ( scr->pri_so.value != rxc_so.pri ) )
    {
      if( rxc_is_loopback_so( scr->pri_so.value ) == TRUE )
      {
        /* If new SO is LOOPBACK SO and old one is VOICE SO, then
           release the VOC and VFR resources */
        if( CAI_IS_SO_VOICE( rxc_so.pri ) )
        {
          muxvoc_disable_rx();
        }

        /* For loop back calls, delaying RXC ATMR by 3
         * PCGs to ensuringg the sequencing and allowing
         * Tx to have first before Rx ATMR fires.
         */

        /* de-register from old PCG value */
        err = m1x_stmr_deregister( M1X_STMR_CLIENT_RX,M1X_STMR_PCG_4 );
        if( err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_UNREG_EVENT )
        {
          ONEX_ERR_FATAL( "Unexpected m1x_stmr driver behaviour with error: %d",
                     err, 0, 0 );
        }

        /* register with new PCG value */
        err = m1x_stmr_register( M1X_STMR_CLIENT_RX, M1X_STMR_PCG_7, rxc_atmr_isr );
        if( err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_REG_DUP )
        {
          ONEX_ERR_FATAL( "Unable to schedule the event with err: %d", err, 0, 0 );
        }
      }
      else if( rxc_is_loopback_so(rxc_so.pri) == TRUE )
      {
        /* previous SO is loopback one, so register RXC ATMR
         * with non loopback SO PCG value
         */

        /* de-register from old PCG value */
        err = m1x_stmr_deregister( M1X_STMR_CLIENT_RX,M1X_STMR_PCG_7 );
        if( err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_UNREG_EVENT )
        {
          ONEX_ERR_FATAL( "Unexpected m1x_stmr driver behaviour with error: %d", err, 0, 0 );
        }

        /* register with new PCG value */
        err = m1x_stmr_register( M1X_STMR_CLIENT_RX, M1X_STMR_PCG_4, rxc_atmr_isr );
        if( err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_REG_DUP )
        {
          ONEX_ERR_FATAL( "Unable to schedule the event with err: %d", err, 0, 0 );
        }
      }

      /* For voice service options, indicate that voice option is
         not configured */
      if( CAI_IS_SO_VOICE( scr->pri_so.value ) )
      {
        /* Service Option changed, call mvs_standby */
        if( rxc_get_voc_config_done() )
        {
          muxvoc_mvs_standby();
          rxc_mvs_set_is_so_change( TRUE );
        }

        rxc_set_voc_config_done( FALSE );
        rxc_set_mvs_status_on_tc_entry( FALSE );
      }
      else if( scr->pri_so.value == CAI_SO_NULL )
      {
        /* If new SO is NULL SO and old one is VOICE SO, then
           release the VOC and VFR resources */
        if( CAI_IS_SO_VOICE( rxc_so.pri ) )
        {
          muxvoc_disable_rx();
        }
        rxc_set_voc_config_done( FALSE );
        rxc_set_mvs_status_on_tc_entry( FALSE );

        if( rxc_sup_dec_int_enable )
        {
          /* Delete SCH task from the deinterleaver Task RAM. */
          rxc_sup_dec_int_enable = FALSE;
          rxc_turbo_dec_enabled  = FALSE;
          deint_delete_supplemental_task_list();
        }
      }
      rxc_so.pri = scr->pri_so.value;
    }
    #else /* FEATURE_IS2000_REL_A */
    /* ckech if there is new service option record */
    if( scr->num_of_so_recs > 0 )
    {
      /* search through all the new service option records */
      for( so_index = 0; so_index < scr->num_of_so_recs; so_index++ )
      {
        /* check if this the primary service option */
        if( scr->so_cfgs[so_index].traffic_type == CAI_PRI_TRAF )
        {
          if( scr->so_cfgs[so_index].so != rxc_so.pri )
          {

            if( rxc_is_loopback_so( scr->so_cfgs[so_index].so ) == TRUE )
            {
              /* If new SO is LOOPBACK SO and old one is VOICE SO, then
                 release the VOC and VFR resources */
              if( CAI_IS_SO_VOICE( rxc_so.pri ) )
              {
                muxvoc_disable_rx();
              }
              /* For loop back calls, delaying RXC ATMR by 3
               * PCGs to ensuring the sequencing and allowing
               * Tx to have first before Rx ATMR fires.
               */

               /* de register from old PCG value */
              err = m1x_stmr_deregister( M1X_STMR_CLIENT_RX,M1X_STMR_PCG_4 );
              if( err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_UNREG_EVENT )
              {
                ONEX_ERR_FATAL( "Unexpected m1x_stmr driver behaviour with error: %d",
                           err, 0, 0 );
              }

               /* register with new PCG value */
               err = m1x_stmr_register( M1X_STMR_CLIENT_RX,M1X_STMR_PCG_7,
                                        rxc_atmr_isr );
               if( err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_REG_DUP )
               {
                 ONEX_ERR_FATAL( "Unable to schedule the event with err: %d", err, 0, 0 );
               }

            }
            else if( rxc_is_loopback_so( rxc_so.pri ) == TRUE )
            {
              /* previous SO is loopback one, so register RXC ATMR
               * with non loopback SO PCG value
               */

              /* de register from old PCG value */
              err = m1x_stmr_deregister( M1X_STMR_CLIENT_RX, M1X_STMR_PCG_7 );
              if( err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_UNREG_EVENT )
              {
                ONEX_ERR_FATAL( "Unexpected m1x_stmr driver behaviour with error: %d",
                           err, 0, 0 );
              }

              /* register with new PCG value */
              err = m1x_stmr_register( M1X_STMR_CLIENT_RX,M1X_STMR_PCG_4,
                                       rxc_atmr_isr );
              if( err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_REG_DUP )
              {
                ONEX_ERR_FATAL( "Unable to schedule the event with err: %d", err, 0, 0 );
              }
            }

            /* For voice service options, indicate that voice option is
               not configured */
            if( CAI_IS_SO_VOICE( scr->so_cfgs[so_index].so ) )
            {
              /* Service Option changed, call mvs_standby */
              if( rxc_get_voc_config_done() )
              {
                muxvoc_mvs_standby();
                rxc_mvs_set_is_so_change( TRUE );
              }

              rxc_set_voc_config_done( FALSE );
              rxc_set_mvs_status_on_tc_entry( FALSE );
            }
            else if( scr->so_cfgs[so_index].so == CAI_SO_NULL )
            {
              /* If new SO is NULL SO and old one is VOICE SO, then
                 release the VOC and VFR resources */
              if( CAI_IS_SO_VOICE( rxc_so.pri ) )
              {
                muxvoc_disable_rx();
              }
              rxc_set_voc_config_done( FALSE );
              rxc_set_mvs_status_on_tc_entry( FALSE );
              if( rxc_sup_dec_int_enable )
              {
                /* Delete SCH task from the deinterleaver Task RAM. */
                rxc_sup_dec_int_enable = FALSE;
                rxc_turbo_dec_enabled  = FALSE;
                deint_delete_supplemental_task_list();
              }
            }
            rxc_so.pri = scr->so_cfgs[so_index].so;
          }

          #ifdef FEATURE_IS2000_REL_A_SVD
          /* if PRIMARY SERVICE is the only service, reset the secondary SO */
          if( scr->num_of_so_recs == 1 )
          {
            if( rxc_so.sec != CAI_SO_NULL )
            {
              M1X_MSG( MUX, LEGACY_MED,
                "Releasing Secondary SO" );
            }
            rxc_so.sec = CAI_SO_NULL;
          }
          #endif /* FEATURE_IS2000_REL_A_SVD */
        }

        #ifdef FEATURE_IS2000_REL_A_SVD
        /* check if this the secondary service option */
        if( scr->so_cfgs[so_index].traffic_type == CAI_SEC_TRAF )
        {
          if( scr->so_cfgs[so_index].so != rxc_so.sec )
          {
            rxc_so.sec = scr->so_cfgs[so_index].so;

            /* For voice service options, indicate that voice option is
               not configured */
            if( CAI_IS_SO_VOICE( scr->so_cfgs[so_index].so ) )
            {
              /* Service Option changed, call mvs_standby */
              if( rxc_get_voc_config_done() )
              {
                muxvoc_mvs_standby();
                rxc_mvs_set_is_so_change( TRUE );
              }

              rxc_set_voc_config_done( FALSE );
              rxc_set_mvs_status_on_tc_entry( FALSE );
            }
            else if( scr->so_cfgs[so_index].so == CAI_SO_NULL )
            {
              rxc_set_voc_config_done( FALSE );
              rxc_set_mvs_status_on_tc_entry( FALSE );
              if( rxc_sup_dec_int_enable )
              {
                /* Delete SCH task from the deinterleaver Task RAM. */
                rxc_sup_dec_int_enable = FALSE;
                rxc_turbo_dec_enabled  = FALSE;
                deint_delete_supplemental_task_list();
              }
            }
          }

          /* if SECONDARY SERVICE is the only service, reset the primary SO */
          if( scr->num_of_so_recs == 1 )
          {
            if( rxc_so.pri != CAI_SO_NULL )
            {
              M1X_MSG( MUX, LEGACY_MED,
                "Releasing Primary SO" );
            }
            rxc_so.pri = CAI_SO_NULL;
          }
        }
        /* store the service option for later reference */
        rxc_srid_ref[scr->so_cfgs[so_index].sr_id].so = scr->so_cfgs[so_index].so;
        rxc_srid_ref[scr->so_cfgs[so_index].sr_id].traffic_type = scr->so_cfgs[so_index].traffic_type;
        #endif /* FEATURE_IS2000_REL_A_SVD */
      } /* for */
    } /* end of if service option record */
    #endif /* FEATURE_IS2000_REL_A */

    /* program the delay of TX generated MASK_DATA to RX PCG timing */
    if( scr->rev_pwr_cntl_delay.included )
    {
      muxmdsp_set_agc_ctl_mask_delay( scr->rev_pwr_cntl_delay.value );
    }
    #ifdef FEATURE_IS2000_1X_ADV
    if( ( rxc_so.for_fch.included == TRUE ) &&
        ( prev_for_fch_rc != (deint_rc_type) rxc_so.for_fch.rc ) )
    {
      rxc_tc_isr_select();
    }
    #endif /* FEATURE_IS2000_1X_ADV */

    #ifdef FEATURE_MODEM_1X_TA_MCVS
    /* Update SRCH : DATA SO is terminated */
    srch_mux_set_data_so( rxc_is_data_call() );
    #endif /* FEATURE_MODEM_1X_TA_MCVS */

    /* Report to SRCH that the SO is a Data SO/TDSO/SMS and DDS sub or not  */
    srch_mux_so_dds_info.is_non_dds_sub      = scr->is_1x_on_non_dds_sub;
    srch_mux_so_dds_info.is_data_call        = rxc_is_data_call();
    srch_mux_so_dds_info.is_tdso_call        =
    ( ( rxc_so.pri == CAI_SO_TDSO ) || ( rxc_so.pri == CAI_SO_FULL_TDSO ) )
    ? TRUE :FALSE;
    srch_mux_so_dds_info.is_sms_call         =
    ( ( rxc_so.pri == CAI_SO_SMS ) || ( rxc_so.pri == CAI_SO_RS2_SMS ) )
    ? TRUE :FALSE;

    srch_mux_report_data_call( srch_mux_so_dds_info );

    /* If this is a DATA service option, vote for DATA clock resources,
       else check if this is a 1xA call and set clocks accordingly. */
    if( rxc_is_data_call() == TRUE )
    {
      dec_config_clk_resources( DEC_CLK_DATA_TRAFFIC );
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_SCR_ACTION received" );
  }
} /* rxc_scr_trans_cmd */

/*===========================================================================

FUNCTION RXC_SHUTDOWN_SCH

DESCRIPTION
  This procedure shuts down F-SCH.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_shutdown_sch( void )
{
  int err = 0;

  /*--------------------------------------------------------------------*/

  M1X_MSG( MUX, LEGACY_MED,
    "SCH shutdown" );

  #ifdef FEATURE_IS2000_REL_A
  /* De-install 2nd ISR */
  err = m1x_stmr_deregister( M1X_STMR_CLIENT_RX, M1X_STMR_PCG_12 );
  if( err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_UNREG_EVENT )
  {
    ONEX_ERR_FATAL( "Unexpected m1x_stmr driver behaviour with error: %d", err, 0, 0 );
  }
  #endif /* FEATURE_IS2000_REL_A */

  RXC_ENTER_ATOMIC_SECTION();

  rxc_disable_ch2();

  rxc_sup_dec_int_enable = FALSE;
  rxc_turbo_dec_enabled  = FALSE;
  deint_delete_supplemental_task_list();

  if( !rxc_sch_report_data.rxc_sch_update_now )
  {
    /* disable F-SCH Outer Loop in SW */
    ffpc_disable_chan_olpc( FFPC_SCH );
  }
  else
  {
    /*
     OLPC was updated by the previous ATMR_ISR (action:MCTRANS_FFPC_ACTION)
     if a sch traffic is comming at 1 grame gap distance
     sch channel olpc should not be touched at SCH_ISR in this 1frame gap scenario
     otherwise the olpc would be disable during the next frame
    */
     M1X_MSG( MUX, LEGACY_HIGH,
       "SCH 1-frame-gap; do not touch OLPC" );
  }

  /* Initialize SCH0 RC */
  rxc_sch.rc = (dec_rc_type) DEINT_RC_INVALID;

  RXC_LEAVE_ATOMIC_SECTION();

  /* For SRCH logging */
  srch_mux_update_rc( rxc_so.for_fch.included?
                      (deint_rc_type) rxc_so.for_fch.rc : DEINT_RC_INVALID,
                      (deint_rc_type) rxc_sch.rc,
                      DEINT_RC_INVALID,
                      rxc_so.for_dcch.included?
                      (deint_rc_type) rxc_so.for_dcch.rc : DEINT_RC_INVALID );

} /* rxc_shutdown_sch */

/*=============================================================================
FUNCTION RXC_ATMR_ISR

DESCRIPTION
  This is the Action Timer System Interrupt. In this, we check the Action Item
  Queue to service the commands that need to be serviced before the next frame
  time.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void rxc_atmr_isr( m1x_stmr_event_type events )
{
  /* Check the Action Item Queue to find out if there
     are any Commands pending that need to be serviced
     before the next frame. */
  qword current_frame_time;
  void (* local_action_time_notifier_func_ptr)( mctrans_server_token_type, qword );

  /*------------------------------------------------------------------*/

  MODEM_1X_UNUSED( events ); /*avoid warning*/

  /* Check if there any read parameters that need to be programmed */
  deint_process_sequencing_cmd();

  #ifdef FEATURE_IS2000_REL_A_SVD
  /* Since there are 2 frames lag time between when the DCCH/FCH is set up and
  ** when data is valid, need to count down these 2 frames.  Without this
  ** mechanism, MUX may deliver stale data to its clients.  The time line is
  ** shown below:
  **    Frame 0:  Start Deint programming by calling deint_set_xxch_mode()
  **    Frame 1:  In ATMR ISR, decrement countdown counter
  **    Frame 2:  In rxc_tc_isr, if count down counter is 1, decrement it
  **              and set data_valid flag.
  ** The count down is set to 2 when the channel is set up.
  ** In the next frame, it is decremented in the ATMR ISR.
  ** In the frame after that, it is decremented in the TC ISR and data valid
  ** is set to TRUE only in the TC ISR.
  ** Reason for this scheme:  in TCI state, there is no TC ISR so we must use
  ** the ATMR ISR to count down.
  ** However, in the 2nd frame, when data is valid, we must make sure data valid
  ** flag is set to TRUE to read it at the TC ISR interrupt.  ATMR interrupt happens
  ** in the next bin, when we already serviced the TC ISR.
  */
  if( ( rxc_so.for_dcch.included )        &&
      ( !( rxc_so.for_dcch.data_valid ) ) &&
      ( rxc_so.for_dcch.data_valid_countdown > 0 ) )
  {
    rxc_so.for_dcch.data_valid_countdown--;
  }

  if( ( rxc_so.for_fch.included )       &&
      ( !(rxc_so.for_fch.data_valid ) ) &&
      ( rxc_so.for_fch.data_valid_countdown > 0 ) )
  {
    rxc_so.for_fch.data_valid_countdown--;
  }
  #endif /* FEATURE_IS2000_REL_A_SVD */

  // If SCH shutdown is pending, need to update shutdown state
  if ( rxc_sch_shutdown_state == RXC_SCH_SHUTDOWN_ARMED )
  {
    rxc_sch_shutdown_state = RXC_SCH_SHUTDOWN_NOW;
  }

  /* Need to read the 64 bit system timestamp in units of 20
  ** millisecond frame time.  Every frame, give this time to
  ** the transaction engine.  If there is an action for that
  ** frame, the transaction engine will set RXC_ACTION_TIME_SIG
  ** within the interrupt context.
  */
  m1x_time_get_20ms_frame_offset_time( current_frame_time );

  /* Use function pointer to call the transaction engine.
  ** This function must execute or RXC will not process any
  ** action.
  */
  local_action_time_notifier_func_ptr = rxc_action_time_notifier_func_ptr;
  if( local_action_time_notifier_func_ptr != NULL )
  {
    /* Reset the signal. */
    (void) rex_clr_sigs( RX_TCB_PTR, RXC_ACTION_TIME_SIG );
    (*local_action_time_notifier_func_ptr)(MCTRANS_SERVER_RXC, current_frame_time);
  }

  /* So here, check for the signal.  If it is set, call the transaction
  ** engine dispatcher to handle the action.  Action handlers have been
  ** registered to the transaction engine.
  **
  ** Note that there is a plan to move the action time handling to task
  ** context in the future.
  */
  if( ( rex_get_sigs( RX_TCB_PTR ) ) & RXC_ACTION_TIME_SIG )
  {
    mctrans_process_transactions( MCTRANS_SERVER_RXC );

    /* Reset the signal. */
    (void) rex_clr_sigs( RX_TCB_PTR, RXC_ACTION_TIME_SIG );
  }

} /* rxc_atmr_isr */

/*=============================================================================
FUNCTION RXC_SET_TX_IN_TRAFFIC_ON

DESCRIPTION
  This function is called to set/reset the rxc_tx_in_traffic_on flag if TX is turned
  on/off

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void rxc_set_tx_in_traffic_on( boolean tx_status )
{
  rxc_traffic_status.rxc_tx_in_traffic_on = tx_status;

} /* rxc_set_tx_in_traffic_on */

#ifdef FEATURE_IS2000_REL_A
/*=============================================================================
FUNCTION rxc_atmr_isr2

DESCRIPTION
  This function is invoked once a frame, at the 4th bin.  This function calls
  Deint function deint_atmr_isr2() to support changes for 6100 release A Turbo
  SCH.  These changes are the result of moving the Turbo SCH dummy task to the
  3rd bin to give the Decoder more time to decode 32X turbo data rate.

DEPENDENCIES
  rxc_atmr_* functions were called in the correct order to handle ATMR ISRs.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void rxc_atmr_isr2( m1x_stmr_event_type events )
{
  /*------------------------------------------------------------------------*/

  MODEM_1X_UNUSED(events); /* avoid compile time warning*/

  deint_atmr_isr2();
} /* rxc_atmr_isr2 */
#endif /* FEATURE_IS2000_REL_A */

/*=========================================================================
FUNCTION RXC_PREP_FOR_RIFHHO_CFS_GPS

DESCRIPTION
  This function prepares for RIFHHO, CFS and GPS operations

DEPENDENCIE
  None.

RETURN VALUE

SIDE EFFECTS
  None.
=========================================================================*/
void rxc_prep_for_rifhho_cfs_gps
(
  boolean is_gps
)
{
  M1X_MSG( MUX, LEGACY_MED,
    "Prep for RIFHHO_CFS_GPS" );

  /* -----------------------------------------------------
  ** Perform Return-If-Fail Hard Handoff or CFS processing
  ** ----------------------------------------------------- */
  rxc_rifhho_cfs_gps_in_progress       = TRUE;
  rxc_hho_data.rxc_lost_frames         = 0;
  rxc_hho_data.rxc_use_stored_eibs     = FALSE;
  #ifdef FEATURE_IS2000_REL_A
  rxc_hho_data.rxc_use_stored_qibs     = FALSE;
  rxc_hho_data.rxc_use_stored_sch_eibs = FALSE;
  #endif /* FEATURE_IS2000_REL_A */

  /* Indicate that a bad frames report has been sent to the Main
     Control task in order to prevent RXC from sending one */
  rxc_fade_data.rxc_sent_bad_rpt = TRUE;

  /* To avoid getting valid signaling after tuning away,
  ** rxc_ignore_good_frame must be set to TRUE before we tune.
  ** For RIF HHO and CFS, RXC_TUNE_RF_F is sent before tuning
  ** to set rxc_ignore_good_frame, and then RXC_RIFHHO_CFS_GPS_F
  ** is sent after we tune.  For GPS we send RXC_RIFHHO_CFS_GPS_F
  ** before we tune and do not send the RXC_TUNE_RF_F at all, so
  ** we need to set rxc_ignore_good_frame to TRUE here for GPS.
  */
  rxc_ignore_good_frame = TRUE;
  rxc_hho_data.rxc_rif_hho_timeout_frames = 0;

  if (is_gps)
  {
    /*
    ** GPS is REALLY started:
    ** Remember we are in GPS.
    */
    M1X_MSG( MUX, LEGACY_MED,
      "Rxed RXC_START_GPS_F");
    rxc_hho_data.rxc_gps_state = RXC_GPS_STATE_STARTED;
  }
} /* rxc_prep_for_rifhho_cfs_gps */

/*=========================================================================
FUNCTION RXC_CFS_GPS_PREP_CMD

DESCRIPTION
  This function handles MCTRANS_CFS_GPS_PREP__ACTION command fro MC.

DEPENDENCIE
  None.

RETURN VALUE

SIDE EFFECTS
  None.
=========================================================================*/
void rxc_cfs_gps_prep_cmd
(
  mctrans_action_packet_type *cmd_ptr
)
{
  if(cmd_ptr != NULL)
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_CFS_GPS_PREP_ACTION: is_gps %d",
      cmd_ptr->action.cfs_gps_prep.is_gps);

    rxc_prep_for_rifhho_cfs_gps(cmd_ptr->action.cfs_gps_prep.is_gps);
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_CFS_GPS_PREP_ACTION received" );
  }
} /* rxc_cfs_gps_prep_cmd */

/*=========================================================================
FUNCTION RXC_HHO_BEGIN_CMD

DESCRIPTION
  This function handles MCTRANS_HHO_BEGIN_ACTION command fro MC.

DEPENDENCIE
  None.

RETURN VALUE

SIDE EFFECTS
  None.
=========================================================================*/
void rxc_hho_begin_cmd
(
  mctrans_action_packet_type *cmd_ptr
)
{
  /* pointer to report to send to Main Control task */
  mccrx_rpt_type *rpt_ptr;

  if(cmd_ptr != NULL)
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_HHO_BEGIN_ACTION: rif %d, timeout %d",
      cmd_ptr->action.rxc_hho_begin.rif_ind,
      cmd_ptr->action.rxc_hho_begin.hho_timeout);

    if (cmd_ptr->action.rxc_hho_begin.rif_ind == TRUE)
    {
      /* Convert the 80ms timeout to a 20ms frame count */
      rxc_hho_data.rxc_rif_hho_timeout_frames =
        cmd_ptr->action.rxc_hho_begin.hho_timeout * 4;

      /* We started counting lost frame since receiving the HO action, which may
      ** preceed the HHO begin action.  So the lost frame count may be non zero
      ** at this time.  Since we just got the RIF timer, reset the lost frame
      ** count so the RIF timer will not expire earlier than expected.
      */
      rxc_hho_data.rxc_lost_frames = 0;

      /* We use zero as an indication that there is no timeout.
       * If the base station was insane enough to set the timeout
       * to zero, send the timeout report immediately.
       */
      if (rxc_hho_data.rxc_rif_hho_timeout_frames == 0)
      {
        /* send report of handoff timeout to Main Control task */
        if ( (rpt_ptr = (mccrx_rpt_type *)q_get(&mcc_rx_free_q) ) != NULL )
        {
          rpt_ptr->hdr.rpt = RXC_HHO_TIMEOUT_R;
          rxc_mc_rpt(rpt_ptr );
        }
        else
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "No buffers on mcc_rx_free_q" );
          rxc_hho_data.rxc_rif_hho_timeout_frames++; /* Try again next frame */
        }
      }
    }
    else
    {
      rxc_hho_data.rxc_rif_hho_timeout_frames = 0;
    }

    /* -------------------------------------------
     ** Perform CDMA to CDMA Hard Handoff processing
     ** ------------------------------------------- */
    rxc_hho_data.rxc_hard_handoff_in_progress = TRUE;


    /* Indicate that a bad frames report has been sent to the Main
       Control task in order to prevent RXC from sending one */
    rxc_fade_data.rxc_sent_bad_rpt = TRUE;

    if ( ! rxc_rifhho_cfs_gps_in_progress )
    {
      /* Reset the fade timer */
      rxc_fade_data.rxc_good_frames = 0;
      rxc_fade_data.rxc_good_guaranteed_frames = 0;
      rxc_fade_data.rxc_t5m_bad_frames = 0;
    }

    /* Indicate that fade processing is in progress */
    rxc_ta_fade_check();

    /* Wait 1 frame before fade timer processing */
    rxc_fade_data.rxc_ho_cnt = 1;

    /* Turn off power control reporting */
    rxc_pwr.threshold_on = FALSE;
    rxc_pwr.periodic_on  = FALSE;

    /* Indicate that RXC_PWR_ON_R hasn't been sent */
    rxc_fade_data.rxc_sent_pwron_rpt = FALSE;

  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_HHO_BEGIN_ACTION received" );
  }
} /* rxc_hho_begin_cmd */

/*=========================================================================
FUNCTION RXC_RETURN_CMD

DESCRIPTION
  This function handles MCTRANS_RETURN_ACTION command from MC.

DEPENDENCIE
  None.

RETURN VALUE

SIDE EFFECTS
  None.
=========================================================================*/
void rxc_return_cmd
(
  mctrans_action_packet_type *unused_cmd_ptr
)
{
  #ifdef FEATURE_IS2000_1X_ADV
  word n2m_val = 0;
    /* n2m value (number of bad frames on the FL before TX is turned off */
  #endif /* FEATURE_IS2000_1X_ADV */

  M1X_MSG( MUX, LEGACY_MED,
    "Got MCTRANS_RETURN_ACTION: LOST_FRAME %d, rxc_fade: %d",
    rxc_hho_data.rxc_lost_frames,
    rxc_fade_data.rxc_fade );

  /*
   * Mark the end of the Return-If-Fail HHO or CFS.  This will
   * re-enable the fade timer and resume counting TOT_FRAMES and
   * BAD_FRAMES.  Note that fade_check still needs to be TRUE at
   * this point
   */
  rxc_rifhho_cfs_gps_in_progress = FALSE;

  /* When returning to the SF, set rxc_ignore_good_frame to FALSE
   * so that we can immediately start counting good frames to restart
   * the transmitter
   */
  rxc_ignore_good_frame = FALSE;

  /* Remember that we do not need to send a TX off rpt to MC */
  rxc_fade_data.rxc_sent_bad_rpt = TRUE;
  /* Indicate that fade processing is in progress */
  rxc_ta_fade_check();

  if (rxc_hho_data.rxc_gps_state == RXC_GPS_STATE_STARTED)
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "GPS returned.");
    rxc_hho_data.rxc_gps_tuneback_frames = 0; /* ignore first 2 frames after tuneback */
    rxc_hho_data.rxc_gps_state = RXC_GPS_STATE_RETURNED;
  }
  else
  /*
  ** Need to wait N3M (2) consecutive good frames before re-enabling
  ** the transmitter if the interval between the time that the mobile
  ** station disables its transmitter and the time that it resumes
  ** using the SF >= N2M (12) frames per section 6.6.6.2.8.2.1 of
  ** IS-95B.
  */
#ifdef FEATURE_IS2000_1X_ADV
  /* For RC11, use the N2M value specified in the Radio Configuration
     parameter message rather than the default N2M value used in legacy
     RCs.
  */
  if (
      (rxc_so.for_fch.included) &&
      ((deint_rc_type)rxc_so.for_fch.rc == DEINT_RC11) &&
      ( (byte) rxc_traffic_status.rxc_rc11.rcp.for_n2m_ind < RXC_MAX_N2M_SIZE) &&
      (rxc_traffic_status.rxc_rc11.rcp.for_fch_blanking_dutycycle !=
        CAI_FCH_BLANKING_DISABLED)
     )
  {
    n2m_val = rxc_traffic_status.rxc_rc11_n2m_val[rxc_traffic_status.rxc_rc11.rcp.for_n2m_ind];
  }
  else
  {
    n2m_val = cai_tmo.n2m;
  }

  /* If # of lost frames is greater than N2M counter then wait for
     2 good frames before tuening ON TX */
  if (rxc_hho_data.rxc_lost_frames >= n2m_val)
#else
  if( rxc_hho_data.rxc_lost_frames >= cai_tmo.n2m )
#endif /* FEATURE_IS2000_1X_ADV */
  {
    /* No need to use stored EIB's because we will receive 2 frames
     * before transmitting, so their EIB's will be used automatically.
     */
    if (rxc_hho_data.rxc_hard_handoff_in_progress)
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "Delay after HHO failure");
      rxc_hho_data.rxc_delay_after_hho_failure = TRUE;
    }
  }
  /* Enable TX only if TX was ON before doing CFS */
  else if( txc_tx_was_on_before_cfs() )
  {
    /* Restore EIBS if Rate Set 2 */
    if ( rxc_so.rate_set == DEC_RATE_14400 )
    {
      txc_flag_next_int();
      txc_eib_flag = rxc_hho_data.rxc_eib_flags.previous;
      rxc_hho_data.rxc_use_stored_eibs = TRUE;
#ifdef FEATURE_IS2000_REL_A
      txc_qib_flag = rxc_hho_data.rxc_qib_flags.previous;
      txc_sch_eib_flag = rxc_hho_data.rxc_sch_eib_flags.previous;
      rxc_hho_data.rxc_use_stored_qibs = TRUE;
      rxc_hho_data.rxc_use_stored_sch_eibs = TRUE;
#endif /* FEATURE_IS2000_REL_A */
    }

    rxc_start_tx();
  }
} /* rxc_return_cmd */

/*===========================================================================

FUNCTION rxc_set_action_time_signal

DESCRIPTION This function sets the RXC_ACTION_TIME_SIG for RXC.  This signal
  indicates that there is an RXC action in this frame which needs to be processed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_set_action_time_signal( void )
{
  M1X_MSG( MUX, LEGACY_MED,
    "RXC set action time signal");
  (void)rex_set_sigs ( RX_TCB_PTR, RXC_ACTION_TIME_SIG );
} /* rxc_set_action_time_signal */

/*===========================================================================

FUNCTION rxc_custom_interrupt_reg_func

DESCRIPTION This function is called to obtain a pointer to MC's transaction
            manager's function which handles action time.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_custom_interrupt_reg_func(
  void (* ticker_func)(mctrans_server_token_type, qword)
)
{
  /* Store ptr to action time function and call it, if it's not NULL,
  ** every frame to process action time.
  */
  //MSG_HIGH_1("Before INIT rxc_at_notifier_func_ptr %d",
      // rxc_action_time_notifier_func_ptr);

  rxc_action_time_notifier_func_ptr = ticker_func;

  //MSG_HIGH_1("After INIT rxc_at_notifier_func_ptr %d",
      // rxc_action_time_notifier_func_ptr);
} /* end rxc_custom_interrupt_reg_func */

/*lint -save -e785 Too few initializers for aggregate -- Difficult to keep
 * track of due to ifdefs, probably OK */
/* Structure to support centralized transaction manager.  This struct contains
** call back functions to set action time signal and to register the timer tick
** used in processing RXC actions, as well as the functions which the MC will
** call to process RXC actions.
*/
mctrans_function_pointer_initializer_type rxc_func_pointer_initializers =
{
  rxc_set_action_time_signal,
  rxc_custom_interrupt_reg_func,
  {
#ifdef FEATURE_IS2000_REL_A_SVD
    /* For VP2 mode, need to check if the primary power control
    ** has changed and program the deinterleaver if it has.  This is
    ** separate from the main FFPC action processing.  Function
    ** rxc_ffpc_trans_cmd() calls ffpc_trans_cmd() when it's done
    ** with the power control channel switch.
    */
    { MCTRANS_FFPC_ACTION,            rxc_ffpc_trans_cmd},
#else
    { MCTRANS_FFPC_ACTION,            ffpc_trans_cmd},
#endif /* FEATURE_IS2000_REL_A_SVD */
    { MCTRANS_RLGC_ACTION,            NULL},
    { MCTRANS_F_SCH_ACTION,           rxc_fwd_sch_trans_cmd},
    { MCTRANS_R_SCH_ACTION,           NULL},
    { MCTRANS_HO_ACTION,              rxc_ho_trans_cmd},
    { MCTRANS_SCR_ACTION,             rxc_scr_trans_cmd},
    { MCTRANS_CFS_NSET_ACTION,        NULL},
    { MCTRANS_CFS_CTRL_ACTION,        NULL},
    { MCTRANS_GATING_ACTION,          NULL},
    { MCTRANS_CTRL_HLD_GATING_ACTION, rxc_control_hold_trans_cmd},
    { MCTRANS_ASET_ACTION,            NULL},
    { MCTRANS_HHO_BEGIN_ACTION,       rxc_hho_begin_cmd},
    { MCTRANS_RETURN_ACTION,          rxc_return_cmd},
    { MCTRANS_CFS_GPS_PREP_ACTION,    rxc_cfs_gps_prep_cmd},
#ifdef FEATURE_IS2000_1X_ADV
    { MCTRANS_RCP_ACTION,             rxc_rcp_cmd},
#endif /* FEATURE_IS2000_1X_ADV */

    { MCTRANS_NULL_ACTION,            NULL},
    { MCTRANS_MAX_ACTION,             NULL}
  }
};
/*lint -restore */

/*===========================================================================

FUNCTION rxc_register_action_handlers_notifier_interrupt_func

DESCRIPTION This function is called at power up to register the following items
            with MC's transaction manager:
      - call back function to set RXC action time signal
      - call back function to register RXC timer tick
            - call back functions which MC will call to process RXC actions.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_register_action_handlers_notifier_interrupt_func (void)
{
  M1X_MSG( MUX, LEGACY_MED,
    "rxc_register_action_handlers called");
  mctrans_register_action_notifier_handler_interrupt_reg_func(
                      MCTRANS_SERVER_RXC,
                      rxc_func_pointer_initializers);
}

/*===========================================================================

FUNCTION rxc_check_for_sch_update

DESCRIPTION
  This function checks for SCH updates in case of 1 frame gap.  This function
  is called AFTER the current frame has been demuxed and is ready for RLP,
  and BEFORE RLP is called.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_check_for_sch_update (void)
{
  /* If there is a deferred SCH update pending, do it now
  ** This covers the 1-frame gap scenario
  **
  ** This code was moved here from its original location
  ** (towards the end of rxc_tc_isr().)  This code needed to be moved
  ** because the previous location causes the current frame
  ** to be demuxed with new SCH parameters meant for future frames.
  ** (The current frame is always demuxed AFTER rxc_tc_isr().  The critical
  ** problem which showed up with the old time line is the last frame
  ** of the previous burst is bad when the rate is changed and
  ** the gap between the 2 bursts is 1 frame, specifically for MUX
  ** PDU 5.  There may not be NAKs because the frame sequence numbers
  ** still match, the problem shows up in bad PPP errors, as a portion
  ** of the data payload may have been added (garbage) or deleted
  ** depending on the rate change (from lower to higer or from higher to
  ** lower.)
  **
  ** A good time to update SCH info is after the frame data has been
  ** demuxed but before the frame data is given to RLP.  Since RLP
  ** runs in RXC context, we don't want to do the update after giving
  ** data to RLP, since RLP may run across frame boundary, and we must
  ** update the SCH within the correct frame to be aligned with burst
  ** assignments.
  */
  if ( rxc_sch_report_data.rxc_sch_update_now )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "Update SCH in RXC task context");
    rxc_update_sch_info( &rxc_log_sch_data.rxc_stored_sch_packet );
    rxc_config_sch_hw( &rxc_log_sch_data.rxc_stored_sch_packet );
    rxc_sch_report_data.rxc_sch_update_now = FALSE;
  }

} /* end rxc_check_for_sch_update */

/*===========================================================================

FUNCTION RXC_CLEAR_RLP_STATISTICS

DESCRIPTION
  Clears the current values of the RLP statistics on command from the
  off-target diagnostic tool.

  NOTE that this is a wrapper function only.  The real processing has been
  moved to DSRLP module.

  Input Parameters:
    *req_ptr: Pointer to DIAG request packet
    pkt_len: Length of DIAG request packet

DEPENDENCIES
  Logging mask, rxc_rlp_stats_interval module global

RETURN VALUE
  Pointer to DIAG response packet

SIDE EFFECTS
  None
===========================================================================*/
diag_rsp_type * rxc_clear_rlp_statistics(
  DIAG_RLP_STAT_RESET_F_req_type *req_ptr,
  word pkt_len
)
{

  DIAG_RLP_STAT_RESET_F_rsp_type *rsp_ptr;
  rsp_ptr = (DIAG_RLP_STAT_RESET_F_rsp_type *) dsrlp_clear_rlp_statistics (req_ptr, pkt_len);
  return((diag_rsp_type *) rsp_ptr );

}

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================

FUNCTION RXC_CONFIGURE_FFPC_CHANNEL

DESCRIPTION
  Used in simultaneous F-FCH and F-DCCH channel set up.  Indicates
  which channel handles FFPC. Also resets the data valid flag and
  counters for FCH and DCCH in VP2

DEPENDENCIES


RETURN VALUE
  Current Radio Configuration

SIDE EFFECTS
  None
===========================================================================*/
cai_radio_config_type rxc_configure_ffpc_channel
(
  rxc_cmd_msg_type *cmd_ptr
)
{
  cai_radio_config_type cur_rc = 0;

  /* need to reset the data valid flag and counters for FCH and DCCH in VP2 */
  rxc_so.for_dcch.included = cmd_ptr->tci.for_dcch.included;
  rxc_so.for_dcch.data_valid = FALSE;
  rxc_so.for_dcch.data_valid_countdown = 0;
  rxc_so.for_fch.included = cmd_ptr->tci.for_fch.included;
  rxc_so.for_fch.data_valid = FALSE;
  rxc_so.for_fch.data_valid_countdown = 0;

  if( cmd_ptr->tci.fpc.included )
  {
    if( ( cmd_ptr->tci.fpc.fpc_pri_chan == CAI_FPC_PRI_CHAN_DCCH ) ||
        ( cmd_ptr->tci.fpc.fpc_pri_chan == CAI_FPC_PRI_CHAN_FCH  ) )
    {
      ffpc_channel = cmd_ptr->tci.fpc.fpc_pri_chan;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Bad ffpc_ch val %d",
        cmd_ptr->tci.fpc.fpc_pri_chan );
      ffpc_channel = CAI_FPC_PRI_CHAN_FCH;   /* Use FCH as default */
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "FPC pri cntrl ch not avail" );
    ffpc_channel = CAI_FPC_PRI_CHAN_FCH;     /* Use FCH as default */
  }

  /* DCCH configuration */
  if( rxc_so.for_dcch.included )
  {
    cur_rc = rxc_so.for_dcch.rc = cmd_ptr->tci.for_dcch.rc;
    rxc_so.high_rc = TRUE;
    if( ffpc_channel == CAI_FPC_PRI_CHAN_DCCH )
    {
      rxc_config_demod_for_traffic( (deint_rc_type) cur_rc );
      M1X_MSG( MUX, LEGACY_HIGH,
         "Config demod for DCCH" );
    }
    else
    {
      rxc_config_demod_ch1( (deint_rc_type) cur_rc );
      M1X_MSG( MUX, LEGACY_HIGH,
         "Config demod Ch1 for DCCH" );
    }
  }

  /* FCH configuration */
  if( rxc_so.for_fch.included )
  {
    cur_rc = rxc_so.for_fch.rc = cmd_ptr->tci.for_fch.rc;
    rxc_so.high_rc = cur_rc > CAI_RC_2;
    if( ffpc_channel == CAI_FPC_PRI_CHAN_FCH )
    {
      rxc_config_demod_for_traffic( (deint_rc_type) cur_rc );
      M1X_MSG( MUX, LEGACY_HIGH,
         "Config demod for FCH" );
    }
    else
    {
      rxc_config_demod_ch1( (deint_rc_type) cur_rc );
      M1X_MSG( MUX, LEGACY_HIGH,
         "Config demod Ch1 for FCH" );
    }
  }

  return cur_rc;

} /* rxc_configure_ffpc_channel() */
#endif /* FEATURE_IS2000_REL_A_SVD */

#ifdef FEATURE_IS2000_1X_ADV
/*===========================================================================

FUNCTION RXC_TC_MDSP_ISR

  This function is a call back function which processes MDSP interrupts for
  RC11 processing in traffic state.  A flag is set before calling rxc_tc_isr()
  to indicate that it's an MDSP interrupt vs. the legacy decoder interrupt.

DEPENDENCIES
  MDSP must be running.

RETURN VALUE
  None.

SIDE EFFECTS
  Either MDSP ISR or DEC ISR is used to drive RXC frame processing.  Both ISRs
  executes around PCG 0 of the frame.

===========================================================================*/
void rxc_tc_mdsp_isr( m1x_stmr_event_type events )
{
  /*-------------------------------------------------------------------*/

  MODEM_1X_UNUSED( events ); /*avoid compile warning*/

  /* Indicate that the MDSP ISR is running. */
  rxc_traffic_status.rxc_rc11.tc_source_mdsp_isr = TRUE;

  /* Call legacy traffic ISR function for additional processing. */
  rxc_tc_isr();

  /* Clear immediately after returning from traffic ISR function to avoid
     interfering with SCH decoder ISR in the same frame (if SCH is assigned.)
  */
  rxc_traffic_status.rxc_rc11.tc_source_mdsp_isr = FALSE;

} /* end rxc_tc_mdsp_isr() */

/*===========================================================================

FUNCTION RXC_1X_ADV_PARSE_MDSP_LOG

DESCRIPTION
  Scan through the current mdsp_buff for any information required for logging.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void rxc_1x_adv_parse_mdsp_log( void )
{
  uint16 entry_count;

  /*Since the mobile is not permitted to Tx an ACK in PCG 0 this is
    used to indicate that No ACK was transmitted. */
  uint8 pcg = 0;

 /* Scan for which PCG (if any) an ACK was sent on R-ACKCH0. */
  for(entry_count = 0; ( (entry_count < rxc1xA_log_data.rxc_1xA_log_mdsp_entry_cnt) &&
                         (entry_count < ARR_SIZE(rxc1xA_log_data.rxc_1xA_log.mdsp_buff)));
                         entry_count += 6)
  {
    /*R-ACKCH0 gain is saved in the lower 16bits of 32bit entry 1 (counting from 0)
      of each half PCG group. (6 32bit entries per group). Any value other than
      0 indicates the ACK was transmitted. Save the PCG where this is true. */
    if(rxc1xA_log_data.rxc_1xA_log.mdsp_buff[entry_count + 1] & 0xFFFF)
    {
      /*The PCG value is bits [12:9] of 32bit entry 0. */
      pcg = (uint8)( ( (rxc1xA_log_data.rxc_1xA_log.mdsp_buff[entry_count] ) >>
                          9 ) & 0xF );
      break;
    }
  }

  rxc_traffic_status.rxc_rc11.status.ack_transmitted_pcg = pcg;
} /* rxc_1x_adv_parse_mdsp_log */

/*===========================================================================

FUNCTION RXC_1X_ADV_INFO_FLUSH_LOG

DESCRIPTION
  Terminate and flush the accumulated 1x Advanced information log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A 1x Advanced information log may be flushed. Sub packet count is cleared to
  begin accumulating again should logging be enabled.

===========================================================================*/
void rxc_1x_adv_info_flush_log( void )
{
  if( log_status( LOG_1X_ADV_L1_PERF_C ) )
  {
    /* Get the current RX Frame Number + offset. */
    rxc1xA_log_data.rxc_1xA_log.frame_num_with_offset = frame_number;

    /* RX Smart Blanking Parameters. */
    rxc1xA_log_data.rxc_1xA_log.smart_blanking |= (
       ( ( (uint8) rxc_traffic_status.rxc_rc11.rcp.for_fch_blanking_dutycycle & 0x3 ) << 4 ) |
       ( ( (uint8) rxc_traffic_status.rxc_rc11.rcp.rev_fch_blanking_dutycycle & 0x3 ) << 2 ) |
       ( ( (uint8) rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame & 0x1 ) << 1 ) );

    /* RX Power Control Parameters. */
    rxc1xA_log_data.rxc_1xA_log.power_control |= (
       ( ( (uint8) rxc_traffic_status.rxc_rc11.rcp.power_control_mode & 0x1 ) << 7 ) |
       ( ( (uint8) rxc_traffic_status.rxc_rc11.rcp.power_control_step & 0x7 ) << 4 ) );

    /* RX Frame Data. */
    rxc1xA_log_data.rxc_1xA_log.frame_data |= (
       ( ( ( (uint16) rxc_sch.rc != (uint16) DEINT_RC_INVALID ) & 0x1 ) << 13 ) |
       ( ( (uint16) rxc_traffic_status.rxc_rc11.rcp.for_n2m_ind & 0x3 ) << 11) |
       ( ( (uint16) rxc_traffic_status.rxc_rc11.status.early_decode_pcg & 0xF ) << 7) |
       ( ( (uint16) rxc_ffch_rate & 0x7 ) << 4) |
       (   (uint16) rxc_traffic_status.rxc_rc11.status.ffpc_setpoint_frozen & 0x1 ) );
  }

  if( rxc1xA_log_data.rxc_1xA_log.hdr.len > 0 )
  {
    /* Send command to diag to send the packet */
    log_submit( &rxc1xA_log_data.rxc_1xA_log );
    log_set_length( &rxc1xA_log_data.rxc_1xA_log, 0 );
  }
  rxc1xA_log_data.rxc_1xA_log_mdsp_entry_cnt = 0;

} /* rxc_1x_adv_info_flush_log */

/*===========================================================================

FUNCTION RXC_1X_ADV_FET_STATS_FLUSH_LOG

DESCRIPTION
  Terminate and flush the accumulated 1x Advanced FET Stats Log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A 1x Advanced FET Stats log may be flushed.

===========================================================================*/
void rxc_1x_adv_fet_stats_flush_log( void )
{
  if( rxc1xA_log_data.rxc_1xA_fet_stats_log.hdr.len > 0 )
  {
    /* Send command to diag to send the packet */
    log_submit( (log_type*)&rxc1xA_log_data.rxc_1xA_fet_stats_log );
    log_set_length( &rxc1xA_log_data.rxc_1xA_fet_stats_log, 0 );
  }
} /* rxc_1x_adv_fet_stats_flush_log */

/*===========================================================================

FUNCTION RXC_MDSP_1XA_LOGGING_ISR

DESCRIPTION:
  This function is a call back function which processes MDSP interrupts for
  1x Advanced logging in traffic state. This ISR fires every half frame.

DEPENDENCIES
  MDSP must be running and the ISR must have been previously registered.
  E and I flags (bits 15 and 0) of mDSP variable CDMA_rliLogConfig must be
  enabled (set to 1).

RETURN VALUE
  None.

SIDE EFFECTS
  1x Advanced data is read from the MDSP and stored for logging purposes.

===========================================================================*/
void rxc_mdsp_1xa_logging_isr( int16 mdsp_buffer_index )
{
  uint16                                rf_band;
  uint16                                rf_chan;
  uint32                                  i = 0;
  uint32                            rsch_on = 0;

  const cai_data_rate_type enc_to_cai_rate [4] =
    {  CAI_FULL_RATE,     CAI_HALF_RATE,
       CAI_QUARTER_RATE,  CAI_EIGHTH_RATE
    };

  /*Logging proceeds only if its enabled and RX is in traffic state*/
  if( ( RXC_TC_STATE == rxc_common_data.rxc_state ) &&
      ( rxc1xA_log_data.rxc_1xA_logging_enabled )   &&
      ( memory_is_allocated_for_logging ) )
  {
    /* Initialize the log buffer and align with the start of a frame. */
    if( rxc1xA_log_data.rxc_1xA_log_mdsp_entry_cnt == 0 )
    {
      /*clear mdsp log*/
      memset(rxc1xA_log_data.rxc_1xA_log.mdsp_buff, 0,
               sizeof(rxc1xA_log_data.rxc_1xA_log.mdsp_buff));

      /* Fill HDR information. */
      if( log_status( LOG_1X_ADV_L1_PERF_C ) )
      {
        log_set_code( &rxc1xA_log_data.rxc_1xA_log, LOG_1X_ADV_L1_PERF_C );
        log_set_length( &rxc1xA_log_data.rxc_1xA_log, sizeof( mux_1x_adv_info_type ) );
        log_set_timestamp( &rxc1xA_log_data.rxc_1xA_log );
        rxc1xA_log_data.rxc_1xA_log_mdsp_entry_cnt = 0;

        /* Set current log structure version. */
        rxc1xA_log_data.rxc_1xA_log.version = MUX_1XA_LOG_VERSION;

        /* Current Band and Channel */
        (void) srch_mux_get_band_chan( &rf_band, &rf_chan );
        rxc1xA_log_data.rxc_1xA_log.band_class = (uint8)  rf_band;
        rxc1xA_log_data.rxc_1xA_log.channel    = (uint16) rf_chan;

        /* ACS 1 & 2+ Reverse Gain Adjusts. */
        rxc1xA_log_data.rxc_1xA_log.rev_gain_adj_acs1     =
                rxc_traffic_status.rxc_rc11.rcp.rev_gain_adj_acs1;

        rxc1xA_log_data.rxc_1xA_log.rev_gain_adj_acs2plus =
                rxc_traffic_status.rxc_rc11.rcp.rev_gain_adj_acs2plus;

        /* Store the current frame's Tx information in a local circular buffer
           to align with Rx information */
        log_buff.sb_decision[one_frame_offset_index]     =
          (uint8) txtc.rc8_status.blank_non_crit_fch_frame;
        log_buff.critical_frame[two_frame_offset_index]  =
          (uint8) !txtc.rc8_status.is_non_critical_frame;
        log_buff.dtx_decision[one_frame_offset_index]    =
          (uint8) txc_device_only_dtx_param.dtx_frame;
        log_buff.sch_state[two_frame_offset_index]       =
          (uint8) sch_state;

        if( tx_frame_rate < ENC_NUM_FRAC_RATES )
        {
          log_buff.tx_frame_rate[one_frame_offset_index] =
            enc_to_cai_rate[tx_frame_rate];
        }

        /* Update the circular buffer index to point to the correct Tx frame
           corresposding to the Rx Frame */
        one_frame_offset_index = (one_frame_offset_index + 1) %
                                   RX_TX_ONE_FRAME_OFFSET_CORRECTION;
        two_frame_offset_index = (two_frame_offset_index + 1) %
                                   RX_TX_TWO_FRAME_OFFSET_CORRECTION;

        /* TX Smart Blanking Parameters. */
        rxc1xA_log_data.rxc_1xA_log.smart_blanking =
          ( log_buff.sb_decision[one_frame_offset_index] & 0x1 );

        /* RL Device Only DTX Parameters */
        rxc1xA_log_data.rxc_1xA_log.device_only_dtx_params  =
           #ifdef FEATURE_MODEM_1X_DEVICE_ONLY_DTX
           ( log_buff.dtx_decision[one_frame_offset_index] & 0x0001 ) |
           #endif /* FEATURE_MODEM_1X_DEVICE_ONLY_DTX */
           (( log_buff.critical_frame[two_frame_offset_index] & 0x0001) << 1 ) |
           ( ( rxc_traffic_status.rxc_tx_in_traffic_on & 0x01 ) << 2 ) |
           ((log_buff.sch_state[two_frame_offset_index] & 0x0003) << 3);

        /* Power Control Parameters. */
        rxc1xA_log_data.rxc_1xA_log.power_control = (
           ( ( (uint8) txtc.rc8_status.pwr_ctrl_mode & 0x1 ) << 3 ) |
           (   (uint8) txtc.rc8_status.pwr_ctrl_step_size & 0x7 ) );

        /* See if RSCH is on/off depending on RSCH state */
        rsch_on = ( ( log_buff.sch_state[two_frame_offset_index] & 0x3 ) > 0 )?
                  TRUE : FALSE;

        /* TX Frame Data. */
        rxc1xA_log_data.rxc_1xA_log.frame_data = (
           ( ( (uint16) txc_tx.tx_on & 0x1 ) << 15 ) |
           ( rsch_on << 14 )  |
           ( ( log_buff.tx_frame_rate[one_frame_offset_index] & 0x7 ) << 1) );

        /* ACK Masks. */
        rxc1xA_log_data.rxc_1xA_log.ack_masks[5] =
          rxc_traffic_status.rxc_rc11.rcp.for_fch_ack_mask_rl_blanking;
        rxc1xA_log_data.rxc_1xA_log.ack_masks[4] =
          rxc_traffic_status.rxc_rc11.rcp.for_fch_ack_mask_no_rl_blanking;
        rxc1xA_log_data.rxc_1xA_log.ack_masks[3] =
          rxc_traffic_status.rxc_rc11.rcp.rev_fch_ack_mask;
        rxc1xA_log_data.rxc_1xA_log.ack_masks[2] =
          rxc_traffic_status.rxc_rc11.rcp.for_sch_ack_mask_rl_blanking;
        rxc1xA_log_data.rxc_1xA_log.ack_masks[1] =
          rxc_traffic_status.rxc_rc11.rcp.for_sch_ack_mask_no_rl_blanking;
        rxc1xA_log_data.rxc_1xA_log.ack_masks[0] =
          rxc_traffic_status.rxc_rc11.rcp.rev_sch_ack_mask;

        /* Parameters Added in Log Version 0x01 */
        rxc1xA_log_data.rxc_1xA_log.ffpc_data = (
           ( ( (uint16) txc_so.fpc_mode & 0xF ) << 4 )  |
           ( ( (uint16) rxc_traffic_status.rxc_rc11.rcp.fpc_mode.value & 0xF )) );

        /* Update the circular buffer index to copy the MDSP Buffer from FW
           to the correct log buffer index. */
        one_frame_offset_index = (one_frame_offset_index + 1) %
                                 RX_TX_ONE_FRAME_OFFSET_CORRECTION;
      }
    }

    /* Copy mdsp log data into the log sub packet */
    muxmdsp_read_1xa_log_buffer( &rxc1xA_log_data.rxc_1xA_log_mdsp_entry_cnt,
                                 ARR_SIZE(rxc1xA_log_data.rxc_1xA_log.mdsp_buff),
                                 log_buff.mdsp_buf[one_frame_offset_index],
                                 (uint8) mdsp_buffer_index );

    /* If the log has been filled, flush it */
    if( rxc1xA_log_data.rxc_1xA_log_mdsp_entry_cnt >=
        ARR_SIZE( rxc1xA_log_data.rxc_1xA_log.mdsp_buff ) )
    {
      /* Update the circular buffer index to copy the correct frame
         information to the MDSP Buffer */
      one_frame_offset_index = (one_frame_offset_index + 1) %
                               RX_TX_ONE_FRAME_OFFSET_CORRECTION;

      /* Delay the MDSP buffer by a frame to align Rx and Tx information */
      for( i = 0; i < ARR_SIZE( rxc1xA_log_data.rxc_1xA_log.mdsp_buff ); i++ )
      {
        rxc1xA_log_data.rxc_1xA_log.mdsp_buff[i] =
                                         log_buff.mdsp_buf[one_frame_offset_index][i];
      }

      /*scan the log for information to capture before we flush it.*/
      rxc_1x_adv_parse_mdsp_log();

      if( log_status( LOG_1X_ADV_FET_STATS_C ) )
      {
        rxc_log_1xa_fet_stats();
      }

      rxc_1x_adv_info_flush_log();
    }
  }
} /* rxc_mdsp_1xa_logging_isr */

/*===========================================================================

FUNCTION RXC_LOG_1XA_FET_STATS

DESCRIPTION:
  When LOG_1X_ADV_FET_STATS_C is enabled, log the current frame early termination
  statistics and report them.

DEPENDENCIES
  Currently requires LOG_1X_ADV_L1_PERF_C to be enabled since information
  is extracted from the mdsp_buf used by that log.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_log_1xa_fet_stats(void)
{
  static uint8 fet_stats_frame_cnt = 0;
  boolean is_guaranteed;

  /* Initialize the log buffer and align with the start of a frame. */
  if( rxc1xA_log_data.rxc_1xA_fet_stats_log.hdr.len == 0 )
  {
    /* Clear the log */
    memset(rxc1xA_log_data.rxc_1xA_fet_stats_log.fwd_link_ack_stats, 0,
           sizeof(rxc1xA_log_data.rxc_1xA_fet_stats_log.fwd_link_ack_stats));
    memset(rxc1xA_log_data.rxc_1xA_fet_stats_log.rev_link_ack_stats, 0,
           sizeof(rxc1xA_log_data.rxc_1xA_fet_stats_log.rev_link_ack_stats));
    fet_stats_frame_cnt = 0;

    /* Fill HDR information. */
    log_set_code(&rxc1xA_log_data.rxc_1xA_fet_stats_log, LOG_1X_ADV_FET_STATS_C);
    log_set_length(&rxc1xA_log_data.rxc_1xA_fet_stats_log, sizeof(mux_1x_adv_fet_stats_type));
    log_set_timestamp(&rxc1xA_log_data.rxc_1xA_fet_stats_log);

    /* Set current log structure version. */
    rxc1xA_log_data.rxc_1xA_fet_stats_log.version = MUX_1XA_FET_STATS_VERSION;

    /* Save ACK Masks */
    if( txc_rcpm_data.rev_fch_blanking_dutycycle == CAI_FCH_BLANKING_DISABLED )
    {
      rxc1xA_log_data.rxc_1xA_fet_stats_log.rev_link_ack_mask =
                             txc_rcpm_data.for_fch_ack_mask_no_rl_blanking;
    }
    else /* blanking enabled 1-in-4 or 1-in-8 */
    {
      rxc1xA_log_data.rxc_1xA_fet_stats_log.rev_link_ack_mask =
                             txc_rcpm_data.for_fch_ack_mask_rl_blanking;
    }

    rxc1xA_log_data.rxc_1xA_fet_stats_log.fwd_link_ack_mask =
                          (rxc_traffic_status.rxc_rc11.rcp.rev_fch_ack_mask & 0xAAAA);
  }

  /* Save current frame statistics */
  rxc1xA_log_data.rxc_1xA_fet_stats_log.fwd_link_ack_stats[fet_stats_frame_cnt] = (
     ( ( (uint8) rxc_traffic_status.rxc_rc11.status.ack_received_pcg & 0xF ) << 4 ) |
    ( ( (uint8) log_buff.tx_frame_rate[one_frame_offset_index] & 0x7 ) << 1 ) |
    (   (uint8) log_buff.sb_decision[one_frame_offset_index] & 0x1 ) );

  is_guaranteed = rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame;

  rxc1xA_log_data.rxc_1xA_fet_stats_log.rev_link_ack_stats[fet_stats_frame_cnt] = (
     ( ( (uint8) rxc_traffic_status.rxc_rc11.status.ack_transmitted_pcg & 0xF ) << 4 ) |
     ( ( (uint8) rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_dec_rate & 0x7 ) << 1 ) |
    (   (uint8) is_guaranteed & 0x1 ) );

  fet_stats_frame_cnt++;

  /* If the log has been filled, flush it */
  if(fet_stats_frame_cnt >=
     ARR_SIZE(rxc1xA_log_data.rxc_1xA_fet_stats_log.fwd_link_ack_stats))
  {
    rxc_1x_adv_fet_stats_flush_log();
  }
} /* end rxc_log_1xa_fet_stats() */

/*=========================================================================
FUNCTION  RXC_UPDATE_1XA_LOGGING_STATUS

DESCRIPTION
  When enabled == true, this function will configure the mDSP to begin
  collecting data for 1xA calls if logging is permitted.
  It will also register the callback that the mDSP will use to provide
  the data to Mux.

  When enabled == false, this function will deregister the interrupt
  callback and disable logging at the mDSP.

  Redundant calls are ignored.

DEPENDENCIES
  RL must be in traffic.
  F-FCH RC must be RC11.

RETURN VALUE
  None.

SIDE EFFECTS
  The 1xA logging mechanism may be enabled or disabled.

=========================================================================*/
void rxc_update_1xA_logging_status (boolean enable)
{
  boolean logging_is_allowed;

  /* In order for the correct half-pcg alignment in the
     mDSP log to occur, RXC must delay enabling the 1xA log
     until the transmitter is enabled (CDMA1X_cTxEnable).
     This is indicated by the rxc_tx_in_traffic_on flag, which
     is set to TRUE after TXC enables the transmitter.

     1xA Logging is required in the background even if the log mask
     is not enabled in order to update counters used during these calls.
     For these cases the function responsible for stuffing the log data
     will only do the bare minimum if the log mask is not enabled and will
     not submit the log packet.
  */
  logging_is_allowed = ( (enable) && (rxc_traffic_status.rxc_tx_in_traffic_on) );

  if ((rxc1xA_log_data.rxc_1xA_logging_enabled) && (!logging_is_allowed) )
  {
    /*De-register the MDSP interrupt and disable logging.*/
    muxmdsp_enable_1xA_logging (FALSE);
    rxc_1x_adv_info_flush_log();
    rxc1xA_log_data.rxc_1xA_logging_enabled = FALSE;
  }
  else if ((!rxc1xA_log_data.rxc_1xA_logging_enabled) && (logging_is_allowed))
  {
    /* Register MDSP interrupt call back function for 1x Advanced
       logging. Clear the buffer queue between the isr and task.
       Enable the mDSP to begin logging.
    */
    muxmdsp_enable_1xA_logging (TRUE);
    rxc_1x_adv_info_flush_log();
    rxc1xA_log_data.rxc_1xA_logging_enabled = TRUE;
  }

  if( !log_status(LOG_1X_ADV_FET_STATS_C) || (!logging_is_allowed) )
  {
    rxc_1x_adv_fet_stats_flush_log();
  }

} /* rxc_update_1xA_logging_status */

/*===========================================================================

FUNCTION RXC_TC_ISR_SELECT

DESCRIPTION
  This function selects the appropriate RXC 20ms frame processing ISR.
  RC 11 uses the MDSP ISR for this purpose while other RCs use the traditional
  Decoder ISR. RC 11 is different because it involves early frame determination
  and MDSP performs rate determination algorithm rather than SW.

  For RC11, this function also sets the MDSP to the right state to process RC11.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Either ATMR ISR or DEC ISR is used to drive RXC frame processing.  Both ISRs
  execute around PCG 0 of the frame.

===========================================================================*/
void rxc_tc_isr_select( void )
{
  int err = 0;

  /*--------------------------------------------------------------------*/

  if( (deint_rc_type) rxc_so.for_fch.rc == DEINT_RC11 )
  {
    /* Clear FCH deinterleaver task list since MDSP will program it. */
    #ifdef FEATURE_IS2000_REL_A_SVD
    rxc_delete_fch_dcch_task_list ();
    #endif /* FEATURE_IS2000_REL_A_SVD */

    /* Write RC11 RPC parameters to MDSP. */
    muxmdsp_update_rc11_params( &rxc_traffic_status.rxc_rc11.rcp );

    /* Initialize decoder rate used for RC11.*/
    rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_dec_rate = CAI_BLANK_RATE;

    RXC_ENTER_ATOMIC_SECTION();
    /* The default for RXC traffic ISR source is the legacy decoder ISR.  Since
       the same interrupt function is used to service both the FCH and the SCH,
       the ISR source is only set to TRUE for the duration of the MDSP ISR.
    */
    rxc_traffic_status.rxc_rc11.tc_source_mdsp_isr = FALSE;

    /* Simulate a decoder done interrupt using the ATMR in PCG 1 of the frame.
       Use this ISR to handle reading the frame from HW, grabbing
       decoder metrics, and reading 1xA FW data from shared memory. */
    err = m1x_stmr_register(M1X_STMR_CLIENT_RX, M1X_STMR_PCG_1, rxc_tc_mdsp_isr);
    if(err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_REG_DUP)
    {
      ONEX_ERR_FATAL("Unable to schedule the event with err: %d", err, 0, 0);
    }

    /* Prepare MDSP for RC11 processing. */
    muxmdsp_set_rc11_mode( TRUE );

    /* Enable APF */
    muxmdsp_enable_apf( TRUE );

    RXC_LEAVE_ATOMIC_SECTION();
  }
  else
  {
    /* Clear ATMR decoder interupt. */
    err = m1x_stmr_deregister(M1X_STMR_CLIENT_RX,M1X_STMR_PCG_1);
    if(err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_UNREG_EVENT)
    {
      ONEX_ERR_FATAL("Unexpected m1x_stmr driver behaviour with error: %d", err, 0, 0);
    }

    RXC_ENTER_ATOMIC_SECTION();

    /* The default for RXC traffic ISR source is the legacy decoder ISR.  Since
       the same interrupt function is used to service both the FCH and the SCH,
       the ISR source is only set to TRUE for the duration of the MDSP ISR.
    */
    rxc_traffic_status.rxc_rc11.tc_source_mdsp_isr = FALSE;

    /* Turn off RC11 processing in the MDSP. */
    muxmdsp_set_rc11_mode( FALSE );

    /* Disable APF */
    muxmdsp_enable_apf( FALSE );

    RXC_LEAVE_ATOMIC_SECTION();
  }

} /* rxc_tc_isr_select */

/*=========================================================================
FUNCTION RXC_RCP_CMD

DESCRIPTION
  This function handles MCTRANS_RCP_ACTION command from CP.  CP sends this
  action to provide RXC with information from RCPM (RC parameter message.)
  CP uses default RC parameters prior to receiving RCPM, then switch to the
  non-default RC parameters when an RCPM is received.

DEPENDENCIE
  MDSP must be running.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void rxc_rcp_cmd
(
  mctrans_action_packet_type *cmd_ptr
)
{
  if( cmd_ptr != NULL )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Got MCTRANS_RCP_ACTION" );

    rxc_traffic_status.rxc_rc11.rcp = cmd_ptr->action.rxc_rcp;

    /* Write RC11 RPC parameters to MDSP. */
    muxmdsp_update_rc11_params (&rxc_traffic_status.rxc_rc11.rcp);

    if( cmd_ptr->action.rxc_rcp.fpc_mode.incl )
    {
      /* Update FFPC mode, etc. */
      ffpc_rcpm_trans_cmd( cmd_ptr );
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL MCTRANS_RCP_ACTION received" );
  }
} /* rxc_rcp_cmd */

/*===========================================================================

FUNCTION RXC_FRAME_IS_VALID

DESCRIPTION
  This function determines if the currently received frame
  (based on frame number) is a valid frame for use in FFPC setpoint updates
  and other calculations.

  Criteria used:

      - Current RC is 11 (1x Advanced)
      - (FRAME_NUMBER + FRAME_OFFSET) mod BLANKING_DUTY_CYCLE != 0

DEPENDENCIES
  1x should already be in the traffic state.

RETURN VALUE
  True if the frame is a valid frame, False otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean rxc_frame_is_valid
(
  uint32                          frame_num,
  cai_radio_config_type           curr_rc,
  cai_fch_blanking_dutycycle_type duty_cycle
)
{
  uint8 blankable_frames = 0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

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
      break;
  }

  /*  Mark all frames as *not* guaranteed if:
      - if the SO supports smart blanking.
      - if the current RC supports smart blanking (i.e RC11)
      - if the smart blanking duty cycle is not disabled
      - if the current frame should be blanked based on frame number + frame offset.
  */
  if( ( curr_rc == CAI_RC_11 ) &&
      ( ( ( frame_num + rxc_common_data.rxc_frame_offset ) & blankable_frames ) != 0 ) )
  {
    return FALSE;
  }

  /* Frame *IS* guaranteed. */
  return TRUE;

} /* rxc_frame_is_valid */

/*===========================================================================

FUNCTION RXC_CONFIG_TEST_ACK_OR_NAK

DESCRIPTION
  API for CP task to use when configuring the test directive for ACK or NAK
  reception for SO74 (Markov) and SO75 (Loopback). This function
  copies the provided paramter for use later during counter updates
  after determining if it is within the valid range.

  Parameters:
  ack_or_nak: Bit mask signifying the test configuration

  0x0 = Normal Operation
  0z1 = Ack expected
  0x2 = Nak expected

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void rxc_config_test_ack_or_nak( uint8 ack_or_nak )
{
  rxc_traffic_status.rxc_rc11.fackch_ack_or_nak_test_mode = 0;

  if( (ack_or_nak == 0x0) || /* TEST OFF */
      (ack_or_nak == 0x1) || /*EXPECT ACK*/
      (ack_or_nak == 0x2) )  /*EXPECT NAK*/
  {
    rxc_traffic_status.rxc_rc11.fackch_ack_or_nak_test_mode = ack_or_nak;
  }

  M1X_MSG( MUX, LEGACY_MED,
      "FL ACKCH Test Mode - ack_or_nak(%x)",
      ack_or_nak);
}
#endif /* FEATURE_IS2000_1X_ADV */


#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION mc_qsh_mdump_collect_rxctraffic_data

DESCRIPTION
This function dumps the rxctraffic data to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void mc_qsh_mdump_collect_rxctraffic_data()
{
  qsh_mdump_collect_high(&rxc_ta_info,sizeof(rxc_ta_info));
  qsh_mdump_collect_high(&rxc_traffic_status,sizeof(rxc_traffic_status));
}
#endif /* FEATURE_QSH_MDUMP */