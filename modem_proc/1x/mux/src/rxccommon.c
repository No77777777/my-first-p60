/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

       C D M A    R E C E I V E   C O M M O N   M O D U L E

GENERAL DESCRIPTION
  Responsible for receive command processing, utility routines,
  action timer registration/deregistration.

EXTERNALIZED FUNCTIONS
  rxc_cmd - send a command to the Receive task
  rxc_flag_good_frame -  inform RXC that it's okay to start processing good
  frame during a Return-If-Fail HHO
  rxc_get_mode - returns the current Receive task primary service option
  rxc_get_pc_crc - retrieves number of messages received on the Paging
  Channel with good and bad CRCs
  rxc_get_sc_crc - retrieves number of messages received on the Sync
  Channel with good and bad CRC
  rxc_get_sch_info - returns current configuration information for the SCHs
  rxc_get_slot_status- returns whether or not we are in our paging slot,
  or in the previous or next slot
  rxc_get_state - returns the current Receive task state
  rxc_load_pc_isr -  installs the PC ISR
  rxc_markov_reset - the rxc_zero_count variable in order to zero symbol
  error rate and Markov statistics
  rxc_powerup_init - initialization of Receive task variables and
  queues
  rxc_power_comb_config - configures the power control puncturing position
  rxc_rpt_voc_rate - Reports the current frame vocoder rate
  rxc_subtask - state machine for the CDMA rx subtask
  rxc_get_chan_demod_config_param -
  rxc_get_chan_tc_demod_config_param -
  rxc_dec_version - Wrapper function for dev_version()

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

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/mux/src/rxccommon.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
04/17/18   eye     Added new crit section for audio related API's.
04/17/18   eye     Clear is_so_change flag at the end of call.
01/19/18   ppr     F3 Reduction changes
07/08/16   nsh     QSH:Add API rxc_check_rx_agc_delta().
06/15/16   eye     Created get-set API for voc_config and reset voc_config
                   after receiving STANDBY_F.
05/12/16   srk     Use locally stored/passed in RC instead of global variable
                   to avoid race condition between RX Task and TC ISR.
04/14/16   srk     Optimized TX Trigger fixes and code cleanup.
02/05/16   eye     1x2GTA Feature changes
10/02/15   srk     DRX SRCH response deadlock design change and cleanup.
09/02/15   eye     Bring HW config under SW DEMBACK check in rxc_wait.
08/26/15   bb      Changes to avoid deadlock situation with DRX feature
07/07/15   eye     Dont disable data bridge if SW DEMBACK is active.
07/07/15   eye     Set Demback ID in CSFB case, before accessing HW REG's.
06/16/15   pk      Changes to enable HW demback for CSFB traffic
06/02/15   srk     Disable SW Demback in FTM mode.
05/05/15   ab      Soft Demback changes for QTA
04/21/15   srk     DRDSDS and BOLT REVISIT Feature cleanup
03/06/15   sst     Update Paging Channel sw Beta value
02/12/15   jh      Changes for SW Demback
                   Use DEC_CLK_SYNC clock state
11/23/14   eye     Added support for 0x1877 Power Reporting Log Packet.
11/27/14   eye     Corrected the RX LEAVE ATOMIC section for get API's.
11/12/14   eye     Added API to get and set MVS variable & reset in call end.
04/02/14   cjb     Decouple clearing of DECODE_DONE ISR from FW disable by SRCH
03/21/14   vks     No need to send RXC_TC_F command to PC isr for timetransfer
03/13/14   vks     Setup traffic demod directly instead of setting it up sync/
                   paging first for RXC_TT_F
03/05/14   srk     Message macro reduction
03/03/14   srk     Dynamically specify stack size for ISRs.
01/28/14   pap     Aggregating the variables into global structures.
12/07/13   cjb     Changes to support DEMBACK_1
11/25/13   cjb     Switched logic for FEATURE_MODEM_1X_BOLT_OBSOLETE feature
10/25/13   cjb     Avoid registering for RX ISR when FW is disabled
10/11/13   vks     Inform FW about data bridge enable/disable for CH1 as well
10/10/13   cjb     DEMBACK0 vs DEMBACK1: Use DEMBACK ID from FW shared memory
10/08/13   pap     Removing warnings for LLVM.
10/09/13   vks     Clean up setting of ch1 decode.
10/08/13   vks     Update to new FW interface - fing config has the walsh code
                   and qof
09/27/13   vks     Use rxccommon api to configure ch0 demod for sync/paging
09/23/13   vks     Update code beta information for traffic (FCH)
09/11/13   vks     Use muxmdsp api to update ch0 and ch1 config
08/28/13   cjb     Mainline FEATURE_MODEM_1X_DIME
07/23/13   srk     Use TCB_PTR MACRO instead of rex_self() or tcb_ptr to make
                   the code compatible with RCINIT.
07/23/13   srk     Send ACK to MC for any outstanding commands before sending
                   system restart.
06/14/13   pap     Aggregating different global variables into structures.
05/31/13   srk     Update proper symbol counts for RC3 and RC5 calls.
03/05/13   srk     Replace existing watchdog implementation with the new
                   Dog Heartbeat Mechanism.
02/15/13   srk     Replace mcs hwio with m1x hwio
01/15/13   trc     Adapt lost decode done ISR workaround to Dime
12/06/12   vks     Protect against filling and sending ffpc log at same time
11/13/12   trc     Layering/Cleanup for Triton
10/26/12   jtm     Suspend power measurement and fade checking while a TT HHO
                   is in progress.
10/25/12   jtm     Update rxc_wait() failsafe recovery mechanism.
10/18/12   srk     Use RX debug F3s for unnecessary F3s.
07/19/12   eye     After PMRM is sent, Total and Bad frame count adjustment.
07/16/12   srk     Removed FEATURE_IS2000_REL_A_CC
06/15/12   srk     Removed FEATURE_IS2000_REL_A_CC_SMMC
04/19/12   jtm     Added sync state HW init to TT_TC_F handling.
03/28/12   srk     Mainlined FEATURE_DS
03/16/12   jtm     Return RXC_BADCMD_S for illegal commands from MC.
02/23/12   srk     Mainlined FEATURE_IS2000_SCH and FEATURE_IS2000_P2
02/17/12   srk     Replaced the tramp api with DAL Interrupt Controller.
02/16/12   srk     Replaced FEATURE_LTE_TO_1X by FEATURE_MODEM_1X_IRAT_LTO1X.
02/13/12   srk     Moved init fucntions outside rxc_powerup_init.
01/23/12   srk     Feature Cleanup
01/17/12   jtm     Revert REL_A clean up.
01/04/12   srk     Feature Cleanup.
12/20/11   srk     Feature clean up: Mainlined FEATURE_MODEM_1X_SUPPORTS_Q6
12/13/11   srk     Feature clean up: Mainlined FEATURE_1X_WDOG_DYNAMIC
12/07/11   jtm     Added RXC_CDMA_STATE to the rxc_wait failsafe handling.
                   Added err_fatal after 3 recovery attempts.
11/22/11   srk     Feature clean up: Mainlined and Internalized Features
11/08/11   srk     Added a flag to prevent clearing the failsafe timer when
                   it is not set originally.
10/26/11   vks     Remove references to obsolete sleep* api and the code
                   associated with it.
09/28/11   srk     Mainlined Always On features and removed Always Off Features
09/20/11   jtm     Added a failsafe timer to detect stalled decoder ISRs.
09/13/11   srk     Added deint_queue_init() in rxc_powerup_init() to prevent
                   memory leak.
09/02/11   srk     Rebased the code to the NikeL modem package by mainlining
                   FEATURE_MODEM_1X_NIKEL.
08/18/11   adw     Mainline 1x message router support.
08/18/11   vks     Fix klocwork errors.
07/26/11   vks     Fix compiler warnings.
07/26/11   cjb     Move rxc_crit_sect init/deinit to rxc_task().
07/20/11   cjb     Delete rxc_crit_sect while exiting rxc_subtask().
06/14/11   jtm     Save frame rate stats along with FER and SER statistics.
04/26/11   jtm     Gracefully handle a CDMA_F command when already in CDMA state.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
04/20/11   adw     Cleanup NikeL "revisit" featurization.
03/31/11   adw     Clear decoder done interrupt status on NikeL.
03/28/11   jtm     Added capability for MSO RDA test to be enabled dynamically.
03/03/11   adw     Removed deint initialization from task startup init.
02/24/11   jtm     Changed error message when receiving invalid commands
                   in inactive state to be less critical.
02/22/11   jtm     Added RXC_TC_TT_F for jumping directly to FL traffic.
01/28/11   jtm     Additional FTM decoupling.
01/25/11   jtm     Added Mux Vocoder interface module.
01/21/11   adw     Added initial M1X_RM support.
01/19/11   vks     More register featurization for nikel.
12/17/10   jtm     Feature cleanup.
12/17/10   bb      Changes to acknowledge back to MC for invalid commands
                   received during inactive state
12/16/10   jtm     Allow rxc_subtask to gracefully handle invalid commands
                   during inactive state.
12/14/10   jtm     Mainlined FEATURE_SRCH_CLKREGIM_RM and cleaned up unneeded
                   legacy clock code.
12/03/10   jtm     Removed MDR/SCCH support.
12/01/10   bb      Add support for LTE to 1X feature
11/15/10   jtm     Initial SU API split for CP.
11/05/10   jtm     Added more _i.h includes for Modem 1x SU API support.
11/01/10   jtm     Private file clean up.
10/30/10   jtm     Modem 1x SU API changes.
10/30/10   jtm     Integrate from rel/1h10.
09/15/10   vks     Add support for the new dynamic watchdog api.
08/28/10   bb/jtm  Use rx_tcb in rxc_clear_isr() instead of rex_self()
                   since this function may be called in another task's context.
08/10/10   jtm     Replaced MSG_SSID_DEFAULT with MSG_SSID_1X_MUX.
08/06/10   jtm     Replaced fwd_rc_to_rate_set with rxc_get_fwd_link_rate_set().
07/20/10   jtm/adw Disable decoder clocks in CDMA state.
06/10/10   jtm     Eliminate dependency on log_dmss.h
05/07/10   jtm     Added API to update and retrieve F-FCH SER and FER stats.
04/12/10   jtm     Disable preemption in rxc_cmd() to prevent a race condition.
04/02/10   jtm     Changed rlp_api.h to mdrrlp_api.h.
03/30/10   jtm     Replaced FEATURE_QDSP6 with FEATURE_MODEM_1X_SUPPORTS_Q6.
05/05/10   jtm     Added support for new RLP API using FEATURE_IS95_RLP_MUX_COMMON_API
03/31/10   jtm     Disable preemption in rxc_cmd() to prevent a race condition.
03/30/10   jtm     Replaced T_QDSP6 with FEATURE_MODEM_1X_SUPPORTS_Q6.
02/04/10   jtm     Removed FEATURE_PLT.
01/29/10   jtm     Removed include of snd.h. Removed feature TMOBILE.
01/10/10   vks     Use FEATURE_QDSP6 instead of T_QDSP6.
12/07/09   vks     Initialize msgr before any other initializations in
                   rxc_powerup_init.
11/12/09   vks     Add MSGR/Q6 framework support.
11/04/09   adw     Initialize the deinterleaver as part of rxc powerup init.
09/14/09   adw     Changes to support modem data header CMI refactoring.
09/12/09   trc     Push RC11 special handling down into deint_set_fch_mode()
08/22/09   jtm     Lint Fixes.
08/05/09   jtm     Lint fixes.
08/04/09   jtm     Added initilization call for new DAL API.
06/11/09   jtm     Replaced FEATURE_1X_Q6_CRIT_SECT with T_QDSP6.
06/06/09   vlc     Added support for 1x Advanced (RC11.)
05/28/09   jtm     Added call to searcher in rxc_powerup_init() to install
                   a Mux-owned MDSP_INT_0 ISR for 1x Advanced.
05/20/09   jtm     Added include for rx_v.h for CMI clean up.
04/21/09   jtm     Replaced assert.h with amssassert.h. Assert.h is being
                   phased out.
04/17/09   jtm     Added includes to fix compiler warnings.
04/13/09   adw     Replaced tdso.h with tdso_v.h include for CMI.
04/06/09   jtm     Moved FEATURE_DS_IS2000 and FEATURE_IS2000_P2 inside
                   rxc_get_sch_info(), rxc_get_fch_rate_set(), rxc_get_dcch_rate_set(),
                   and rxc_dcch_is_on() since these functions are exposed in the au/api
                   and must have a definition.
04/01/09   jtm     Include clean up.
03/30/09   jtm     Lint fixes.
03/27/09   jtm     Eliminated implicit includes.
03/26/09   jtm     Mainlined T_MSM7600.
03/26/09   jtm     Added include for modem_1x_defs.h
03/20/09   jtm     Cleaned up header file inclusions.
03/17/09   jtm     Replaced INTLOCK/INTFREE, INTLOCK_SAV/INTFREE_SAV, and
                   MSM_LOCK/MSM_UNLOCK with macros that can be mapped correctly
                   based on the compiler and controlled internally to RXC.
02/03/09   jtm     Added support for T_POSEIDON2
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
09/12/08   vlc     Removed snd.h for FEATURE_MVS_MIGRATE.
07/21/08   jtm     Removed FEATURE_RXCCHAIN_DEBUG code. Removed rxc_ser_accum().
                   Klockwork fixes and removal of obsolete code.
07/01/08   vlc     Merged code changes from old tip archive.
04/22/08   vlc     In function rxc_subtask, make sure clocks are turned on
                   when FEATURE_SRCH_CLKREGIM_RM is disabled.
04/10/08   trc     Switch clock mgmt strategy via FEATURE_SRCH_CLKREGIM_RM
03/18/08   mca     Removed sample server clock workaround for srch
03/05/08   vlc     User clock resource manager to manage RX clocks
                   (FEATURE_CLKREGIM_RM.)
05/03/07   cab     Moved to new sleep voting interface
02/23/07   vlc     Fixed compiler warnings/errors when SCH, Rel A, and P2
                   features are turned off.
01/11/07   vlc     Fix featurization compiler error when FEATURE_IS2000_REL_A
                   is turned off.
01/04/07   trc     MSM7600 support
11/14/06   vlc     In function rxc_wait(), changed lockup recovery time from 6
                   watchdog timeouts to 3 since 6 is too long to wait.
10/30/06   bn      Added code to support DO to 1x handoff
10/24/06   bn      Attempt to recover lookup in rxc_wait(),by reseting DEC/DINT HW
09/19/06   vlc     Fixed lint errors.
06/09/06   trc     Remove T_MSM6700 featurization
05/16/06   vlc     In function rxc_subtask(), check if 1X is operating on the
                   primary RF chain before disabling decoder clock regime.
05/09/06   vlc     In function rxc_subtask(), added check for RXC_CHAN_CONFIG_F
                   command.  Ignore this command but account for it as it may
                   be sent to RXC in certain HO cases.
                   This change was merged from 6500 target.
10/12/05   rkc     Add wrapper function rxc_dec_version().
09/20/05   rkc     Mainline feature FEATURE_SRCH_INACTIVE_STATE.
09/08/05   ejv     Remove inclusion of srch.h.
08/29/05   bn      Clear the all the decoder status it after installing the isr
07/25/05   rkc     Fixed lint errors.
04/27/05   vlc     Main lined code in current targets, cleaned out obsolete targets,
                   cleaned out obsolete features.
03/14/05   bn      Modified the rxc_isr_cmd not to wait when the queue is not empty
                   also modified the rxc_wait() not to use msg_fatal instead of error_fatal
02/24/05   vlc     Lock interrupt between clearing DEC_DONE_INT_STATUS and
                   installing RXC ISR.
02/23/05   vlc     Clear the DEC_DONE_INT_STATUS when setting RXC ISR.
01/26/05   vlc     Removed obsolete ds.h inclusion.
12/07/04   ejv     Added new function rxc_get_chan_tc_demod_config_param.
09/28/04   sr      Merged in changes from 6550 2009 build
08/09/04   bn/kwo  Added new interfaces to expose chan mod setup parameters
06/16/04   sr      Merged in changes from 6500 5004 build
06/14/04   sr      Added debug message in rxc_flag_good_frame()
06/09/04   jyw     Corresponding modifications for the SEARCH inactive state
05/07/04   vlc     Implemented new RLP interface for EV-DV.  The new interface
                   replaces the current interface for 1x channels as well.
                   Also, modified RLP logging extensively to work with new
                   interface.
04/28/04   jrp     Reverted change of MSM_LOCK to INTLOCK_SAV to fix compilation
                   error with new search headers.
04/20/04   jrp     Fixed lint warnings.
03/09/04   jrp     Modified rxc_wait() to ERR_FATAL after petting watchdog 11
                   times without receiving any signal for which we are
                   waiting.
01/06/04   vlc     Modified function rxc_config_demod_ch1() to use MAC engine.
10/23/03   sr/vlc  Clear decoder ISR before disabling clock regime in function
                   rxcdma_subtask().
10/22/03   sr      Fixed compilation warning errors when using RVCT compiler
10/14/03   vlc     In function rxc_config_demod_ch1(), removed call to
                   rxc_power_comb_config() which sets power control channel
                   registers.
09/09/03   dna     Merged porting changes for MSM6500
08/08/03   vlc     Use dec_dcch_status_type instead of dec_fch_status_type for
                   rxc_dcch_status.
07/18/03   vlc     Release A changes:  Changed FCH and DCCH
                   channel set up to better indicate when decoder data is
                   available for the appropriate channel(s) for the RXC layer.
06/16/03   bn      Fixed the "out of buffer dec_free_q" problem.
04/18/03   bn      Called function mux1xdiag_init() to register all the MUX cmd to DIAG
03/20/03   bn,sr   Added logic to support simultaneously monitor BCCH and FCCCH
03/05/03   fc      Consolidated f-csh fade timer implementation.
10/15/02   dlb     Added new power control logging to combine power control
                   and frame type logging.
10/15/02   sr      Cleaned up comments in rxc_isr_cmd()
10/14/02   sr      Added MSM6100 changes
10/11/02   sr      Called rxcidle_clr_fade_timer() in rxc_exit()
08/06/02   cr      Added MSM6100 changes.
08/27/01   vlc     Changed power measurement report process for VP2.
08/13/02   sr      Added Fast RAHO support
                   Disable CH0_RC4 bit for non-RC4 configurations in
                   rxc_config_demod()
08/07/02   bn      Remove obsolete reference to event_report_data()
05/30/02   vlc     VP2 changes are now featurized under FEATURE_IS2000_REL_A_SVD
                   and FEATURE_IS2000_P2 per request of development team.
05/13/02   vlc     Added support for simultaneous DCCH & FCH channel set up
                   (feature FEATURE_IS2000_REL_A_VP2.)
02/27/02   lad     Added rxc_clear_decoder_queue() and modified rxc_isr_cmd()
                   to call this routine.
                   Merged IS2000 Rel A Common Channels:
                   Added BCCH/FCCCH support in state machine.
                   Moved unused/obsolete code under "unused" feature.
02/25/02   bn      Added support for  MUX SVD signaling
12/10/01   hrk     Renamed function rxc_flag_erasure() to rxc_flag_eib() and moved
                   the function to rxctraffic.c
11/20/01   bn      Added RDA logging logic
11/16/01   vlc     Added call to dec_clk_select () before switching rxc state.
10/30/01   bn      Added support for Release A FDCH logging
08/02/01   lcc     Moved some traffic channel related functions and data to
                   rxctraffic.c.
06/20/01   sr/tc   Fixed HHO between high RCs to low RCs in rxc_config_demod()
06/15/01   snn     Added two new functions for DCCH UI display.
06/15/01   day     Mainlined FEATURE_FFPC, FEATURE_IS95B, FEATURE_IS2000
                   Delete FEATURE_SPECIAL_MDR
06/04/01   vlc     Changed FEATURE_GPSONE to FEATURE_GPS_MODE.
05/27/01   snn     Added a new function rxc_get_fch_rate_set() for UI and
                   modified rxc_get_sch_info to return SCH RC
04/20/01   sr      Merged in from MSM5100 archive
04/19/01   tc      Merged with MSM_MUX1X.01.00.01
           bgc     Moved rxc_scr_trans_cmd to rxctraffic.c
04/18/01   sr      Merged with MSM_MUX1X.01.00.00
           lcc     Added support for FEATURE_IS2000_P2
03/27/01   vlc     Merged MSM_MUX1X.00.00.17.
           lcc     Merged in support for SO 17 (CAI_SO_VOICE_13K_IS733).
           lcc     Changes in rxc_get_sch_info for new type used for
                   rxc_stored_sch_packet.
           lcc     Optimized rxc_atmr_dispatcher.  Also added checking for phase
                   change after the ISR is done and execute the ISR for the next
                   phase if so; otherwise the ISR for the next phase will be missed.
           ych     Merged JCDMA features
03/13/01   tc      Cleaned up rxc_atmr_dispatcher..
03/02/01   tc      Merged MSM_MUX1X.00.00.15.
02/22/01   rm      Disable deinterleaver/decoder clock regime when exiting
                   FM Mode.
02/16/01   bgc     Changed FEATURE_FACTORY_TESTMODE to enable a runtime switch.
02/01/01   tc      Updated to rxc_rifhho_cfs_gps_in_progress.
01/30/01   tc      Merged GPS support.
01/18/01   ejv     Modified some combiner macros to match new names.
01/16/01   jq      Added FEATURE_IS2000_SCH ifdefs around SCH codes.
01/12/01   tc      Merged MSM_MUX1X.00.00.06.
12/22/00   hrk     Flushing FFPC log packets at end of call.
12/12/00   ejv     Added changes for CH0.
11/23/00   tc      Fixed MSM5100 PLT ftm_sc_msg, ftm_pc_msg, ftm_rxc_pc_cmd.
11/17/00   bgc     Fixed warnings with typecasts.
11/10/00   jc      repartioned rxc.c into several files from Build TR2064
10/16/00   hrk     Fixed problem ending back-to-back SCH burst.
10/16/00   hrk     Elaborated description for the overlapping SCH burst fix.
10/12/00   sr/hrk  Fixed forward overlapping burst assignment problem.
10/05/00   lcc     Moved processing of SMS traffic in rxc_pri together with OTAPA.
                   All these service options do not use data (primary or secondary)
10/04/00   jq      Added FTM FCH and SCH loopback feature
10/04/00   ak      More fixes in SCH logging.
10/03/00   sr      Excluded DTX frames from SCH bad frame stats for PMRM
09/29/00   hrk     Checking at rxc_sch.rc to determine if F-SCH is ON.
09/29/00   ejv     Added frame count while waiting for first frame in slot.
09/28/00   bgc     Added support for turbo DTX in rxc_tc_isr().
09/21/00   lcc     Combined freeing of TC resources on fade and on normal release
                   into rxc_free_tc_resources.  Also protected with MSM_LOCK
                   since it can happen both from interrupt context and task context.
                   Checking is made before proceeding to make sure that it wasn't
                   already done.  This fixes CR 14669.
09/18/00   lcc     1. Added clearing of rxc_sch_shutdown when processing a start
                      SCH action.  It is protected by INTLOCK becasue a race
                      condition can happen if a decoder ISR occurs (which checks
                      for rxc_sch_shutdown) at the same time.  Fixes CR 14711.
                   2. Added clearing of sigs in action command header in
                      rxc_process_at_cmd since no signal is expected when the
                      command is done processing.  If this is not done, spurious
                      signals may be sent.  This fixes CR 14732.
09/13/00   bgc     Turned on DTX detection for RC4 supplimental channels
                   in rxc_tc_isr().
09/12/00   sr      Changed ts_get_20ms_frame_time() to
                   ts_get_20ms_frame_offset_time() for action timing
                   Call to ts_set_frame_offset() in rxc_set_frame_offset()
09/06/00   bgc     Added FEATURE_FACTORY_TESTMODE.
08/23/00   ak      Removed sch_crc_err counts, as no longer useful.
09/05/00   rm      Added Fixes to correctly decode the last FCH frame
                   before the burst.
09/03/00   rm      Added changes to support MUX PDU type 1 for 1X SCH.
08/29/00   ak      Added missing ; to a return statement (l. 4168).
08/25/00   rc      Added changes to support NULL returns from
                   dsm_new_buffer().
08/24/00   hrk     CHanged FEATURE_CS_ACTION_TIME_TRANS to FEATURE_IS2000.
                   Removed redundant action time code.
08/22/00   rm      Fixed compiler warning.
08/18/00   rm      Free up TC resources when fade timer expires in TC state.
08/14/00   sr      Fixed SCH erasure reporting for TDSO calls
08/04/00   rm      Fixed timing issues related to SCH burst assigments.
08/02/00   sr      Added FCH statistics to power measurement report
                   Changed TDSO_BLANK to TDSO_ERASURE
07/31/00   lad     Fixed sub-record count in logging.
07/24/00   hrk     Collecting AGC values for Reverse Power Control logging.
07/24/00   ak      Changes to support some PDU's being BLANK when doing
                   RLP 3 logging.
07/20/00   ak      Removed old IS2000 mux stat structs and FEATURE_SCH_STATS.
                   Made fixes for Mux stats for SO33 and TDSO.
07/19/00   hrk     Added support for BS configurable Reverse Power Control Delay.
07/18/00   eh      Set the viterbi decoder to normal state in rx_pc_isr when
                   servicing interrupt for the first frame of paging slot.
07/14/00   ks      Setting the rxc_fade_time properly during RXC_ACC_FADE_F.
07/13/00   rm      Added PLT support for Supplemental Channel Multiframe Inter
                   leaving.
07/12/00   rm      Added suuport for 5ms frames.
07/12/00   sr      Fixed some indexing problems while building tdso frame block
                   Fixed the number of bits sent to tdso for 1x case.
07/11/00   sr      Set rxc_sup_dec_int_enable to TRUE for turbo codes
07/10/00   sr      Added STDSO support
07/06/00   ks      Added p_rev_in_use checking while setting T72M timer.
06/29/00   sr      Fixed a problem with SCH burst assignment where the last
                   SCH frame wasn't being received because the demod/deint/dec
                   was being shut down 1 frame too soon
06/26/00   kjn     Added vuid argument to rxc_dec_rate_sel.
06/22/00   hrk     Introduced function rxc_rda_correction() to improve SCH
                   DTX detection.
06/20/00   ak      Bumped up the rxc_dec_free_q count, to avoid dropped fr's
                   when timing may go bad.
06/19/00   bgc     Rearranged DTX detection to favor CRC pass over DTX.
06/16/00   lcc     1. Miscellaneous cleanup
                   2. Fixed CR 13635
                   3. Corrected a problem in comparison only the lower 2 words
                      of action time.
06/13/00   ak      Added SCH0 mux stats for parm database.
06/09/00   hrk     Added support for new MC-RXC interface.
06/09/00   bgc     Added in MSM5000_IRAM_FWD for moving functions into
                   internal RAM.
06/09/00   hrk     Added back fix for CR 13583 that got overwritten.
06/09/00   ak      Added support for F-SCH 1x.
06/08/00   hrk     Added support for Outer Loop Report
06/06/00   bgc     Added SER check for full rate for RC3 and RC4 to improve
                   full rate detection when frame bits are mostly zeros.
06/05/00   hrk     rxc_frame_type is passed as param to ffpc_update_setpt
                   instead of a boolean.
06/05/00   jq      Fixed CR 13583 by comment out the fade timer reset call in
                   rxc_pc.
                   Took out a redundant ts_get_20ms_frame_time() call in
                   rxc_pc_isr()
06/02/00   lad     Added SCH logging for frame types and FFPC.
06/02/00   ak      Added f-sch mux stats.  Also set dtx threshhold in
                   rxc_Fwd_sch_trans_cmd.
05/30/00   hrk     Moved FFPC FCH setpt updates during FCH interrupt.
05/26/00   jq      Added support for BCSMS feature (merged from PP)
                     - Added BC_DONE bit checking in idle page match
                     - Added the concept of slot_mask
                     - Allow early sleep in BC SMS slot by peeking into msg hdr
05/24/00   fc      Merged the following changes:
           ry      Added OTAPA support
05/22/00   bgc     Fixed some typos for DMSS in the DTX support.
                   Added fixed support for turbo SCH interrupts.
                   Added extra logging/msg PLT support for DTX.
                   Cleaned up and rearranged some rxc_tc_isr() code.
05/18/00   et      added support for dtx in sch
05/10/00   rm      Made Fixes to support 1x SCH.
05/10/00    jl     Added RLP statistics logging.
05/08/00   hrk     In rxc_atmr_set(), making sure that rxc_atmr_handle is
                   valid before calling atmr_set().
05/02/00   na      Fixed the SCH error message, which was printed many times
                   on getting a CRC error.
04/25/00   sr      Added support for FWD SCH transaction processing.
                   Changed bit-wise AND to a logical AND operation in rxc_tc_isr.
           hrk     Added support for SCH FFPC transaction processing.
                   Modified RXC-FFPC interface to handle SCH.
                   Updating SCH RC in SRCH for logging.
                   Changed FEATURE_IS2000_SCH to FEATURE_IS2000_F_SCH.
                   Re-arranged braces in rxc_tc_isr() for non MSM5000 case.
           dgy     Protected rxc_atmr_set () with INTLOCK, INTFREE.
04/25/00   hrk     Fixed brace mismatch in rxc_tc_isr() for non MSM5000 case.
04/24/00   hrk     done_q_ptr in action time buffer was overwritten. As a result
                   the buffer never got back into action item free Q.
04/21/00   hrk     Removed call to rxc_send_scr_action_time_rpt () for
                   RXC_HO_F and RXC_FPC_SUBCHAN_GAIN_F.
                   calling srch_update_rc () to update RC info in SRCH.
04/20/00   lad     Added subrecord count and len_per_frame to frame type log.
04/12/00   hrk     Added prototype for ffpc_log_frame() and ffpc_send_log().
                   Call to rxc_send_rlp_frames_log() was not under
                   FEATURE_RLP_LOGGING.
04/12/00   hrk     FPC_HISTORY was assigned into rxc_rpctrl_dec_history
                   instead of RPC_HISTORY.
                   Renamed global rxc_ffpc_dec_history to rxc_fpc_history,
                   rxc_rpctrl_dec_history to rxc_rpc_history.
                   Moved call to ffpc_send_log () from ffpc_ho_cmd() to
                   rxc_service_ho_cmd ().
04/11/00   lad     Added Fast Fowrward Power Control and forward frame types
                   logging.  Cleaned up RLP logging.
04/10/00   dgy     Removed FEATURE_IS2000_QPCH.
04/07/00   va      Added the processing of RXC_TUNE_RF_F (HHO fix from PP)
04/05/00   na      CR 13264: scc_crc was not being reset before call to
                   dec_read_scch_data. This was causing erasures on SCCH to
                   be detected as Full Rate frames. Now fixed.
03/31/00   na/ak   In the RX task, rxc_dsm_arr_cnt was not being checked
                   within array range. Fixed this.
03/24/00   bgc     Fixed FEATURE_RLP_LOGGING so that it can be turned off.
                   Updated rxc_fch_rda() for rate determination at RC3-RC5.
                   Added a "Skip TCI" option in PLT so that the test for good
                   frames is ignored.
03/23/00   lad     Added partial RLP frames logging under FEATURE_RLP_LOGGING.
                   Cosmetic changes and changed len in event_report_data call.
03/16/00   na      Added support for RLP3 - dsrlp_process_rx_frames() under
                   FEATURE_DS_RLP3, and support for data SCH Mux layer under
                   FEATURE_DS_IS2000.
                   Removed extraneous write to the timetest port.
03/16/00   bgc     Fixed reacquisition of Paging channel after RC4 call in
                   rxc_pc_init() with COMBINER_CLEAR_ULPN_OPTION().
02/28/00   hrk     Switched back to old RDA.
02/24/00   lad     Added support for streaming DIAG and backward compatable
                   FEATURE_DIAG_V7_LOG_IFACE.
02/22/00   bgc     Fixed some typos in the PLT code and removed some
                   unnecessary #ifdefs that made compilation fail.
02/04/00   rm      atmr_get is now called in rxc_pc_init instead
                   of rxc_tc_init. This will need to change when
                   QPCH is turned on.
02/02/00   rm      Free up resources/clean up on exit from TC state.
02/02/00   hrk     Merged following changes from GV40401:
           hrk     Merged function rxc_scc_rate_sel_rlp().
           aaj     Fixed Markov Packet anamoly in Data Services service option
           dna     Fixed CR12117 and CR12166 by moving handoff timer into RXC.
           aaj     Moved rxc_voc_xchg() from rxc_pri to the rxc_tc_isr(). This
                   was done to take care of a bug in EVRC vocoder code.
           aaj     Added FEATURE_DETECT_SUP_MUTING which detects the MDR sup
                   channel muted frames and erases those even if the CRC is ok.
           kk      Removed the panther version of the voc_config() function.
02/01/00   hrk     Added support for Implicit Action Time.
01/31/00   rm      Enable decoder clock regime on wakeup. DEC clock
                   regime is disabled when the MSM is armed to sleep.
01/25/00   hrk     Added support to process commands in RXC_TC_STATE at
                   Action Time.
01/24/00   rm      Added in support to do event sequencing in deint driver.
01/24/00   rm      Added in new interfaces to support ACTION TIME processing.
                   The changes are under FEATURE_RXC_ACTION_TIME_PROCESSING.
01/19/00   hrk     Moved FFPC logging to FFPC module. Also fixed typo in
                   defining FEATURE_FFPC.
01/07/00   bgc     Changed RX action time report to "RXC_AT_R" to make it
                   more general
01/04/00   hrk     Fixed compiler warnings. Changes for updated MC-RXC
                   interface.
12/17/99   bgc     Added CRC summary logging for verification tests.
12/16/99   rm      Implemented the new Rate Determination Algorithm
                   for FCH (RC3, RC4, RC5). This algorithm is based on
                   Daisy Chen's Simultaions.
12/13/99   hrk     Moved FFPC functionality to a separate module. Older
                   implementation featurized under FEATURE_FFPC_OLD.
                   Added support for new MC-RXC interface.
                   Removed HHO FIX per Vanitha.
12/01/99   hrk     Changed interface to FFPC support functions to allow
                   scaled setpoint values to have 64 bits.
12/01/99   hrk     (For Vanitha) Added RXC_RELEASE_F in Traffic Channel state.
                   Added HHO FIX in RXC_RETURNED_F.
11/15/99   rm      Commented out processing for RXC_HO_F. This is going
                   to change.
11/15/99   rm      Added support to send MC report indicating that SCR has
                   has been acted on based on the ACTION TIME from MC.
11/12/99   rm      Made temp. fixes for frame offset timing issues affecting
                   deinterleaver/decoder for MSM5000. More general solution
                   to come later.
11/09/99   rm      Made Changes to support Turbo decoding of RC3, RC4 and
                   RC5 SCH.
11/04/99   bgc     Changed some calls to plt_put_log to conform to the
                   more obvious function parameters.
11/03/99   lcc     Merged from GD4021:
           kmp     Mainlained the FEATURE_ENHANCED_STANDBY_II and the
                   #ifdef FEATURE_ENHANCED_STANDBY by removing the #ifdef's
                   in the following areas:
                    - removed #ifdefs in the rxc_slot structure
                    - removed #ifdefs around MACROS RXC_MAX_INT_CNT and
                      RXC_SLOT_INT_CNT, and global var rxc_last_ser.
                    - removed #ifdefs in rxc_pc_isr(), rxc_pc_parse(),
                      rxc_pc(), rxc_exit(), and mainlined rxc_load_pc_isr().
           kmp     Merged in the new Vocoder changes required for Pink Panther
                   in rxc_tc(). Fixed a problem in which SMS service options
                   were not bringing up primary traffic in rxc_pri().
           ry      Updated to process good frame during a Return-If-Fail HHO
                   only when RXC is told to (by calling rxc_flag_good_frame);
                   otherwise, ignore all good frames
           ry      Fixed a problem in which the rxc_rif_hho_or_cfs_in_progress
                   flag is not reset properly.  Also fixed how the mobile waits
                   for two consecutive good frames before turning on the
                   transmitter in the case of a failed Return-If-Fail HHO
           rc      Changed occurence of ds_process_rlp_rx_frame() to
                   ds_process_rlp_rx_frames().
11/02/99   wjn     Added code to perform quick page match for MSM5000's 72-bit
                   decoder delay.
10/27/99   ajn     Added QPCH support
10/21/99   rm      Added new Interface RXC_SCCH_F to support MDR for MSM5000
                   and other various changes for SCCH.
10/21/99   rm      Replaced rxc_tc_isr with a MSM5000 specific rxc_tc_isr.
10/07/99   lcc     Merged from PLT 2.0:
           jcw     Added F3 message control for PLT decoder messages
           bgc     Fixed size/rate table for PLT logging and moved MSG code to
                   a section where it will see erasures.
           rm      Added SCH logging support.
           hrk     Added support for Outer Loop on F-FCH
           rm      Added SCH support. Configure deinterlevaer/decoder and
                   programming MAC Engine.
09/21/99   lcc     1. Changes to use new interface to MC.
                   2. Added MSM5000 support to service options
                   3. Optimized rate determination functions for supp.
                   3. Merged in following MSM5000 and PLT support from PLT
                      archive:
           jcw     Changes to tc_isr messages, logging improvements from bgc
           rm      Fixed RC4 descrambling.
           bgc     Modified logging to log with data or without data.
           rm      Made PLT changes  to fix jump to hyperspace timing issues.
           bgc     Fixed spacing problems and changed some RC problems in the
                   logging functions.
           rm      Added rxc_power_comb_config to configure the power control
                   puncturing based on Radio Configuration.
           bgc     Added PLT logging for decoder information.
           hrk     Added support for Fast Forward Power Control.

           rm      Added RC parameter in rxc_tci command for 95c
           rm      Made Changes for sync and paging channel frame processing
                   . The decoder delay in continous mode for MSM5000 is 72 bits.
           rm      Made changes for new deint/dec driver interfaces and
                   MSM5000 PLT support
           jcw     Added dgy's changes to fix paging channel PLT operation
           jcw     Merged in the following three PLT changes
           dgy     Supported the option so that user can decide whether to
                   wait for a PC message before proceeding.
           dgy     Added code so that RXC task continuously demodulates and
                   decodes Sync Channel frames while in SC state. Sent message
                   to PLT_MCC when RXC task receives the first message in SC
                   or PC state.
           dgy     Added support for PLT.
09/02/99   ry      Removed obsolete MSG_MEDs from rxc_tc_data()
08/26/99   aaj     Merged Panther Branch 3.1 to Mainline
08/25/99   aaj     Changed threshold for sup erasure detection for RS2 (from 77 to 50)
08/13/99   jq      Added casting for ts_get()'s parameter to clear compiler
                   warnings.
08/09/99   ry      Combined FEATURE_IS95B_HARD_HO and FEATURE_IS95B_CFS into
                   FEATURE_IS95B_MAHHO
           ry      Added check in rxc_tc_parse() to ignore any signalling
                   message that the mobile might have received right before
                   it is about to tune to the CF/TF
08/09/99   lss     Fixed a problem with the blackout counters under MDR_FER_F3MSG
08/06/99   nmn     Removed IS-96
07/29/99   hq      Changes for SMS SO6 and 14 support.
07/27/99   lss     added a feature FEATURE_MDR_FER_F3MSG for collecting MDR frame
                   error statistics and printing it on on DM F3 screen.
07/09/99   ry      Replaced txc_flag_erasure() with rxc_flag_erasure()
           ry      Updated pwr_msr_check() to suspend counting
                   rxc_pwr.num_frames during a Return-If-Fail HHO
           ry      Added rxc_start_tx()
           ry      Splited up fade_check() into fade_check_bad_frame(),
                   fade_check_good_frame(), fade_check_good_frame_hho(), and
                   fade_check_good_frame_non_hho() and added IS95B HHO support
           ry      Updated rxc_tc_data() to suspend counting
                   rxc_pwr.num_bad_frames and start counting rxc_lost_frames
                   during a Return-If-Fail HHO
           ry      Added the processing of RXC_RIF_HHO_OR_CFS_F and
                   RXC_RETURNED_F
07/06/99   aaj     Advanced freeing of decoder buffer queue element to avoid "no
                   free dec buf" problem in the traffic channel state
06/23/99   ks      resetting tx_is_disabled to false when T40M report is sent.
06/17/99   kmp     Merged in the one change listed below.
06/07/99   ks      Added T72M timer implementation during PC monitoring while
                   the Mobile is in System Access state.
05/27/99   kmp     Changed FEATURE_JSTD008 to FEATURE_TMSI.
06/08/99   aaj     Added ERR_MESSAGE if a sup channel intr occurs without sup channels
                   Also corrected few comments as suggested in the code review.
05/25/99   aaj     Fixed reverse link EIB setting for LSPD,voice and sup=0 cases
04/22/99   aaj     Undone the (cast) operator changes made for armtools 2.5
04/21/99   aaj     Replaced MSG_ERROR with MSG_HIGH in rxc_pc_isr()
04/19/99   aaj     Usage of (cast) operator in ts_get to support armtools 2.5
04/15/99   aaj     Changed rxc_tc_isr() to handle two traffic channel interrupts
                   in a MDR call. First intr is for FCC and second for SCC
04/14/99   aaj     Corrected rate determination threshold for 8th rate RS2.
04/14/99   aaj     Changed the mux table variabels from 4 byte int to 2 byte word
04/10/99   aaj     De-Featurized (mainlined) FEATURE_CLASS_1_IMSI
04/06/99   aaj     Removed functions for RXDSP clock changes.RXDSP runs at 2/3tcxo
04/06/99   aaj     Replaced b_copy_litend_to_bigend_word with optimized for MDR
                   only version b_copy_ltobw_s1d0 and b_copy_ltobw_s2d0 functions
04/01/99   aaj     Interface to new PARAM database for supplemental channel FER
03/25/99   aaj     FEATURE_MINIMIZE_ISR_LOADING added to RXC. txc_voc_xchg is
                   moved out of rxc_tc_isr to TXC task. rxc_voc_xchg is run in
                   the context of RXC task.
03/23/99   aaj     Support for mux dim and burst for data frames was added.
02/26/99   ejv     Merged FEATURE_ENHANCED_STANDBY_II changes, including fast
                   slotted page match and 2nd half frame MER issue.
02/18/99   aaj     Moved contents of mux.c into rxc.c as it contained only
                   data declarations. Also changed the mux look up tables to
                   consts to move them into ROM. That saves some RAM space.
02/03/99   aaj     Added functions to switch RXDSP clock between 2/3TCXO and chipx8.
                   Also added functions for clock change from MC for MDR calls.
02/01/99   aaj     Decoder buffer read optimization in RXC_TC_ISR. Reads only
                   the required read from decoder as agaist all rates.
01/19/99   lcc     Corrected a problem in rxc_tc_data where dec_bits may be
                   used when it is not initialized yet.
01/15/99   aaj     Merged MDR branch with Panther mainline.
                   Edit History for MDR branch.
                        01/05/99   smp  Fixed bugs in supplemental channel processing for RLP.
                        12/23/98   lcc  Fixed a problem with DPT supplemental frames
                        11/19/98   aaj  MDR RLP interface. Under #define FEATURE_IS95B_MDR.
11/20/98   ht      Add FEATURE_ENHANCED_STANDBY label for code already
                   checked in.
11/3/98    abh     Changes for frame no. correction after the frame offset
                   change. Also made some changes to remove compiler warnings.
10/22/98   pms     Backed out nearing of variables.
10/02/98   pms     Neared variables to save ROM space.
09/14/98   ajn     Valid messages before slot boundary are once again ignored.
09/14/98   gb      Allow report to mc, resulting in sleep, after any valid
                   message, since there is no use staying awake for the
                   entire slot cycle.
09/14/98   ajn     Valid messages BEFORE slot boundary should not satisfy
                   the "valid messages in first two slots" condition to go
                   to sleep.
09/14/98   cly     Pulled in new fade_check function from -r1.118 to provide
                   IS-95A 6.6.6.2.8 compliance. Also, pulled in changes from
                   -r1.118 for 14400 rate determination and more err logging.
09/14/98   ajn     Empties decoder queue after uninstalling decoder ISR, to
                   prevent a remnant message from reenabling the FADE timer
                   as the phone goes to sleep.
08/31/98   pms     Added Tan's Pascal calling convention code for ROM savings
08/20/98   ck      Using VOC_CAP_IS733 instead of VOC_CAP_ISXXXX
                   Using VOC_CAP_IS127 instead of VOC_CAP_EVRC.
08/11/98   ck      Added support for EVRC Service Option
07/28/98   lcc     Merged in ARM support.  MSM3 support is also merged in.
03/12/98   aks     Removed pre-Gemini conditionals.  Changed target-specific
                   sections into FEATURE_AGC_LOGGING.
01/27/98   ldg     Commented usages of the unusual DS_CASE macro.
01/15/98   lh      Included customer.h.
01/15/98   lh      Resend RXC report to MC if out of buffers on mcc_rx_free_q.
11/03/97   ldg     Replaced references to specific data service options with
                   a macro which represents the same information.
07/16/97   lh      Added logging out-of-buffer errors.
07/09/97   na      Fixed a RLP bug where the RLP layer was not getting informed
                   when the mux layer got an erasure. Change under ifdef DS.
06/17/97   dhh     Configured for Q (T_Q).
06/11/97   gb      UnFixed Lint catch in rate determination since this uncovered
                   several problems with the thresholds that are data dependant.
                   Fixed a paging channel data buffering problem causing occasional
                   messages to be processed very late (on re-entry to idle state).
05/29/97   na      Added correct data service option numbers - fixes service
                   option change while in traffic channel. Changes under ifdef
                   DS.
04/09/97   gb      Fixed Lint catch in 14400 rate determination where
                   if CRC pass and yamamoto good and SER below thresh
                   but another rate SER is lower we don't declare erasure.
12/23/96   rah     Added ONIST TMOBILE Support.
11/14/96   jjw     Added Rate Set 2 data service capabilities.
10/22/96   dna     Flush 8 instead of 3 half-frames on idle handoff.
10/22/96   rdh     Consolidated a few TRAMP function calls to save ROM.
09/26/96   gb      Restore TMOBILE function inadvertantly excised.
09/24/96   gb      Added more frame rate checks, reserved bits == 0, valid
                   signalling data LEN or CRC.
09/19/96   rdh     Consolidated markov statistics functions to save ROM.
09/19/96   gb      Ifdef'd vocoder logging TMOBILE to save RAM and ROM.
09/13/96   gb      Added checks for valid mux types that we use to throw
                   away some of the bogus false rate determination frames.
09/06/96   gb      Sped up erasure indicator bit timing in tc_isr.
08/05/96   gb      Fixed faulty fade check logic.
07/25/96   gb      Cleaned up decoder buffer management to avoid ERRs.
07/10/96   gb      Fixed paging channel frame time stamping to avoid going to
                   sleep too early from a page in previous slot.
                   Changed to new 14400 rate determination algorithm.
                   Fixed a number of possible buffer misuse problems which
                   would result in buffers being held too long.
06/11/96   rdh     Changed slot_status fnc to detect slot, slot-1, slot+1.
05/02/96   rdh     Changed slot_status function to detect slot and slot-1.
04/09/96   rdh     Added funct to return whether or not we're in Paging slot.
03/14/96   rdh     Priority changes now only allowed when using idle fade timer..
03/06/96   jca     Receipt of RXC_PWR_REP_DELAY command now triggers
                   PWR_REP_DELAY countdown for Power Measurement reporting.
03/04/96   rah     Updated CSS1 conditionals.
03/01/96   gb      Made vocoder pkt xchg callable by txc_traffic_isr.
02/28/96   rdh     Added mods for robust pkt/cont mode operation during Jump.
02/26/96   rdh     Fixed bad ifdef for sleep tcb for Beta2.
02/01/96   rdh     Added MC priority bump, removed ZZ ISR for sleep speedup.
01/17/96   gb      Changed ts_get_frame calls ensure correct frame timing.
11/16/95   jca     Removed INTLOCK/INTFREE pair in function rxc_cmd().
11/09/95   jjw     Completed changes to support fully compliant IS-99
11/07/95   gb      Linted and removed old unused variables and code.
11/02/95   fkm     Added calls to snd_set_voc_gain_adjust() for Vocoder Gains.
10/20/95   gb      Fixed internal mux1 array bounds problem by removing it.
10/12/95   gb      Changed markov logging formats to suit data analysis.
                   Changed msg_q size to 5 to buffer more ota msgs.
10/06/95   gb      Changed call release to switch rx to SO_NULL.
09/28/95   rah     Added MAR_RATE_SEL call. Changed rate deturmination
                   algorithm.
09/22/95   dna     Now call fade_check for 13K calls and drop if necessary.
08/23/95   ptw     Crippled vocoder logging to save a little RAM.
08/16/95   gb      Changed voc_rate_sel and mar_rx to support rate set 2.
08/14/95   gb      Added MUX2 parameter support.
08/14/95   gb      Added new Loopback_13K service option support.
08/10/95   gb      Added New Markov Service Options and made all service
                     options CAI_SO_*.
07/28/95   rah     Removed AUDIO_CONTROL_PATCH.
07/26/95   rah     Added rxc_reset_voc_init call.
07/21/95   jjw     Added local variables within '#ifdef FEATURE_DS' to support IS-99
07/20/95   rah     Moved voc_init to the tc ISR for GEMINI and ISS2 targets.
                   Removed txc task run from rxc_tc_isr for GEMINI and ISS2.
07/13/95   gb      Changed mux option initialization.
07/06/95   rah     Removed refs. to vcmd and modified for 13k init.
06/28/95   rdh     Move agc log back into 20ms isrs til DA group analyzes needs.
06/21/95   rdh     Fixed wakeup problem now that RXC controls chipx8.
06/19/95   rdh     Temporary mods for audio path muting during origination.
06/15/95   rdh     Moved all chipx8 bring-down code into new RXC sleep ISR.
06/14/95   gb      Added support for Rate set 2 service options.
06/13/95   rdh     Made T_SLEEP handling explicit and consistent DMSS-wide.
05/27/95   rdh     Moved AGC/power control logging over to PN roll interrupt.
05/15/95   gb      Changed constants to match new DEC declarations for gemini.
04/13/95   rdh     Call AGC/Power control logging in RX ISRs if Gemini.
03/30/95   rdh     Added srch_control_chipx8_clk call to fix runt pulse problem.
01/19/95   jca     Enabling the vocoder whenever we transition to TC state.
01/09/95   gb      Changed wake processing to delay frame count reinit.
12/23/94   jca     Now reinitalizing the vocoder on a hard handoff command.
11/29/94   gb      Changed rxc_tc_isr to signal TX for new TX frame timing.
10/27/94   rdh     Told SRCH @ bad slot rather than messing with MC transitions.
10/24/94   rdh     Additions for PN autoswitch/error correction.
10/12/94   gb      Changed voc_rx ERR_FATAL to ERR due to loopback modes.
08/09/94   jca     Added support for IS96/IS96A dual-capability vocoder.
04/25/94   gb      Parameterised all IS95 timeouts in cai_tmo.
04/04/94   gb      Set non-slotted mode when RXC gets an access_fade cmd.
03/24/94   gb      Added called interface to set isr & timing fix code.
01/25/94   jah     Go back to hunt state on wakeup from sleep
01/06/94   jah     Changed watchdog iface to use dog_report().
12/22/93   jca     Modified for IS-95 Phase 1 upgrade.
12/20/93   gb      Sleep version with frame# set in pc-isr and reset pc_rate.
08/24/93   jca     Now processing RXC_CDMA_F command in PC, TCI and TC states.
01/14/93   jai     Added hard handoff processing.
01/05/93   jai     Added data service support and fixed Markov reset problem.
10/15/92   jai     Added vocoder test.
10/08/92   jai     Added fixes for problems found in unit test.
10/06/92   jai     Fixed signal set in rxc_mc_rpt.
09/17/92   jai     Changed Vox functionality
06/24/92   jai     Started again.
03/28/91   jai     Create file.

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
#include "rxcmc_i.h"
#include "rxccommon.h"
#include "rxccommon_i.h"
#include "rxc.h"
#include "rxc_i.h"
#include "rxcidle.h"
#include "rxctraffic.h"
#include "ffpc_i.h"
#include "rxcsync.h"
#include "rxcpaging.h"
#include "mux1xdiag.h"
#include "muxmsm.h"
#include "mux_logtypes.h"
#include "muxmdsp_i.h"
#include "txccommon_i.h"
#include "txctraffic_i.h"

//diag
#include "cdma2kdiagi.h"
#include "onex_non_sig.h"

//drivers
#include "dalint_1x.h"
#include "dec5000.h"
#include "dec5000_v.h"
#include "deint_i.h"
#include "dmod_i.h"
#include "m1x_rm.h"
#include "m1x_hwio_mpss.h"
#include "msm_drv.h"
#include "msm_drv_i.h"

//cp
#include "cai_v.h"
#include "mc_i.h"
#include "mccdma_v.h"
#include "mccdma_i.h"
#include "rx.h"
#include "rx_v.h"
#include "tdso_v.h"
#ifdef FEATURE_1XCP_QSH_SUPPORT
#include "mccqsh_i.h"
#endif /* FEATURE_1XCP_QSH_SUPPORT */

//srch
#include "srch_mux.h"
#include "srchi_i.h"
#include "srchmc.h"
#include "srch_hw.h"

//other
#include "DDIInterruptController.h"
#include "rex.h"
#include "queue.h"
#include "log.h"
#include "dsm.h"
#include "msg.h"
#include "m1x_diag.h"
#include "dog.h"
#include "dog_hb_rex.h"
#include "err.h"
#include "cmd.h"
#include "qw.h"
#include "amssassert.h"
#include "mdrrlp_api.h"
#include "vstmr.h"

//feature dependent
#include "dsrlp_stats.h"
#include "muxmsgr.h"

// For demback ISR loss system restart interface
#include "mccsrch_i.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* declare a critical section that is common throughout the RX task */
rex_crit_sect_type rxc_crit_sect;

/* declare a critical section that is used for rx-audio interactions */
rex_crit_sect_type rxc_voc_crit_sect;

rxc_common_vars_type rxc_common_data =
{
  .rxc_frame_offset = 0,
  .rxc_frame_offset_countdown = 0
};

/* This structure is used to update SRCH and MUX power related stats */
rxc_tx_srch_pwr_log_type rxc_srch_mux_pwr_stats;

/* structure used to update pwr stats to diag for log packet 0x1877 */
mux_1x_pwr_info_type rxc_1x_pwr_log;

rxc_rev_sb_dtx_param_type rxc_tx_sb_dtx_param;

/*-------------------------------------------------------------------------*/
/* Buffers for RXC_DEC_FREE_Q                                              */
/*-------------------------------------------------------------------------*/
/* BN increasing the buffer to handle SIFCCCH/BCCH */
#define                  RXC_NUM_DEC_BUFS 6
rxc_dec_type rxc_dec_bufs[ RXC_NUM_DEC_BUFS ];

/*-------------------------------------------------------------------------*/
/* Buffers for RXC_ISR_FREE_Q                                              */
/*-------------------------------------------------------------------------*/
#define                  RXC_NUM_ISR_CMD_BUFS  3
rxc_isr_cmd_type rxc_isr_cmd_bufs[ RXC_NUM_ISR_CMD_BUFS ];
/*-------------------------------------------------------------------------*/


dec_fch_status_type rxc_fch_status;

#ifdef FEATURE_IS2000_REL_A_SVD
/* Used in simultaneous F-FCH and F-DCCH channel set up.
*/
dec_dcch_status_type rxc_dcch_status;
#endif /* FEATURE_IS2000_REL_A_SVD */

/* Total symbol error rates returned from decoder */
rxc_ser_type rxc_ser_total = { 0, 0, 0, 0, 0 }; /* used for logging */

byte  rxc_zero_count; /* Indicator of whether SER statistics should be set to
                        zero - anything other than 0 = reset */

/*-------------------------------------------------------------------------*/
/* Interrupt Service Routine variables                                     */
/*-------------------------------------------------------------------------*/

qword  rxc_dec_frame;
  /* Frame number */

  /* logging AGC values */
byte log_rx_agc = 0;
byte log_tx_pwr = 0;
byte log_tx_gain_adj = 0;

/*-------------------------------------------------------------------------*/
/*           Forward Traffic Channel Power Control Parameters              */
/*-------------------------------------------------------------------------*/

rxc_pwr_type rxc_pwr;

/*-------------------------------------------------------------------------*/
/* Service Option variables                                                */
/*-------------------------------------------------------------------------*/

rxc_so_type rxc_so;

#ifdef FEATURE_IS2000_REL_A_SVD

rxc_srid_ref_type  rxc_srid_ref[RXC_SR_ID_MAX]; /*service option data for reference */

#endif /* FEATURE_IS2000_REL_A_SVD */

boolean rxc_sup_dec_int_enable = FALSE;
boolean rxc_turbo_dec_enabled  = FALSE;
/* Indicates if Convolutional SCH decoder interrupt is enabled */

/* Variables for counting slots                                            */
/*-------------------------------------------------------------------------*/

rxc_slot_type rxc_slot;


word rxc_log_msg_drop = 0;     /* number of messages dropped due to no
                                 available buffer */

/* Indicator to ignore good frame while doing a Return-If-Fail HHO      */
boolean rxc_ignore_good_frame = FALSE;

tdso_frame_block_type rxc_tdso_frame_block;

rxc_fer_ser_report_type rxc_fer_ser_stats;

static boolean rxc_mvs_so_change = FALSE;

/*-------------------------------------------------------------------------*/
/* RX task DOG parameters                                                  */
/*-------------------------------------------------------------------------*/

/* dog parameters */
dog_report_type rxc_dog_rpt_id = 0;

#ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
/* Flag used to enable/disable Markov test capability. */
boolean rxc_mso_rda_test_enable;
#endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */

rex_timer_type rxc_failsafe_timer;

#define RXC_ISR_LOCKUP_MAX_COUNT              5

static boolean rxc_processing_TT_F = FALSE;

/* RXC ISR params */
static struct
{
  dalint_1x_irq_types  irq;     /* IRQ for DECODE_DONE interrupt */
  DALISR               handler; /* ISR for DECODE_DONE interrupt */
} rxc_isr_data;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

#ifdef FEATURE_1XCP_QSH_SUPPORT
/*===========================================================================

FUNCTION RXC_CHECK_RX_AGC_DELTA

DESCRIPTION
  This function checks Rx AGC delta between pri and sec channel. if diff is 
  greater than prescribed limit, it will log it.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_check_rx_agc_delta ( void )
{
  byte rxc_rx_agc_diff;
  byte rxc_pri_rx_agc       = srch_get_rx_agc();
  byte rxc_sec_rx_agc       = srch_get_rx1_agc();

  if( rxc_pri_rx_agc > rxc_sec_rx_agc )
  {
    rxc_rx_agc_diff = rxc_pri_rx_agc - rxc_sec_rx_agc;
  }
  else
  {
    rxc_rx_agc_diff = rxc_sec_rx_agc - rxc_pri_rx_agc;
  }

  if( rxc_rx_agc_diff >= RXC_RX_AGC_DIFF_ALLOWED )
  {
    QSH_LOG(QSH_CLT_ONEXL3, QSH_CAT_EVENT, QSH_MSG_TYPE_HIGH,
      "QSH:High Rx AGC delta, Rx AGC pri = %d, sec = %d, diff = %d",
      rxc_pri_rx_agc,rxc_sec_rx_agc,rxc_rx_agc_diff);
    mc_send_qsh_event(ONEXL3_QSH_EVENT_L2_HIGH_RXAGC_DELTA);
  }
}
#endif /* FEATURE_1XCP_QSH_SUPPORT */

/*===========================================================================

FUNCTION RXC_DOG_REPORT

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
void rxc_dog_report ( void )
{
  dog_hb_report(rxc_dog_rpt_id);
  (void) rex_clr_sigs( RX_TCB_PTR, RXC_DOG_RPT_SIG );

} /* rxc_dog_report */


/*===========================================================================

FUNCTION RXC_WATCHDOG_INIT

DESCRIPTION
  This function is used to initialize the RX task with the DOG subsystem

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  WDOG timer system is started for RX task
===========================================================================*/
void rxc_watchdog_init( void )
{
  /* Register with dog task */
  rxc_dog_rpt_id  = dog_hb_register_rex( RXC_DOG_RPT_SIG );

  /* Report to watchdog task. */
  rxc_dog_report();

} /* rxc_watchdog_init */


/*========================================================================

FUNCTION RXC_CLEANUP_SCH_MUX

DESCRIPTION This function cleansup the array that is used to hold
            the SCH MUX frames

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_cleanup_sch_mux( void )
{
  byte i;                                                    /* loop ctr */

  /* setup SCH MuxPDU array to hold new items */
  for( i=0; i< SCH_MAX_FWD_PDU; i++)
  {
    dsm_free_packet( &rxc_log_sch_data.rxc_dsm_arr[i]);
  }
  rxc_log_sch_data.rxc_dsm_arr_cnt = RXC_DSM_ARR_NOT_ALLOC;
} /* rxc_cleanup_sch_mux() */


/*===========================================================================

FUNCTION RXC_RPT_VOC_RATE

DESCRIPTION
  Reports the current frame vocoder rate to caller.

DEPENDENCIES
  None.

RETURN VALUE
  Current frame vocoder rate.

SIDE EFFECTS
  None.

===========================================================================*/
cai_data_rate_type rxc_rpt_voc_rate( void )
{
  return( rxc_frame_rate );

} /* rxc_rpt_voc_rate */


/*===========================================================================

FUNCTION RXC_RESET_FRAME_COUNT

DESCRIPTION
  This function resets the frame counters of valid message reporting

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  Some RXC global counters will be changed.

===========================================================================*/
void rxc_reset_frame_count( void )
{
  rxc_slot.valid_msg_mask = 0;
  rxc_slot.slot_frames = 0;
  rxc_slot.bcpg_slot_frames = 0;
  rxc_slot.bcsms_slot_frames = 0;

} /* rxc_reset_frame_count */


/*===========================================================================

FUNCTION RXC_GET_STATE

DESCRIPTION
  This function returns the current Receive task state.

DEPENDENCIES
  None.

RETURN VALUE
  Current Receive task state.

SIDE EFFECTS
  None.

===========================================================================*/
rxc_state_type rxc_get_state( void )
{
  return( rxc_common_data.rxc_state );

} /* rxc_get_state */


/*===========================================================================

FUNCTION RXC_GET_MODE

DESCRIPTION
  This function returns the current Receive task primary service option


DEPENDENCIES
  None.

RETURN VALUE
  Current Receive task mode.

SIDE EFFECTS
  None.

===========================================================================*/
word rxc_get_mode( void )
{
  return( rxc_so.pri );

} /* rxc_get_mode */


/*===========================================================================

FUNCTION RXC_SER_REPORT

DESCRIPTION
  This function returns the symbol error rate statistics.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_ser_report
(
  rxc_ser_type *ser_ptr
    /* pointer to where to put statistics */
)
{
  /* Unused parameter */
  (void) ser_ptr;

} /* rxc_ser_report */


/*===========================================================================

FUNCTION RXC_MARKOV_RESET

DESCRIPTION
  This function sets the rxc_zero_count variable in order to zero symbol
  error rate and Markov statistics.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_markov_reset( void )
{
  rxc_zero_count = TRUE;

} /* rxc_markov_reset */


/*==========================================================================

FUNCTION RXC_SC_SER_ACCUM

DESCRIPTION
  This function accumulates the status information for SYNC Channel.
  The status information includes CRC/Quality, Energy Metrics and SER

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void rxc_sc_ser_accum( void )
{
  rxc_sc_ser_total.frame_cnt++; /* Increment the sync channel frame count */
  rxc_sc_ser_total.ser += rxc_sc_status.ser;

} /* rxc_sc_ser_accum */


/*===========================================================================

FUNCTION RXC_SER_RESET

DESCRIPTION
  This function resets the symbol error rate statistics.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_ser_reset( void )
{
  rxc_ser_total.frame_cnt = 0;
  rxc_ser_total.ser1 = 0;
  rxc_ser_total.ser2 = 0;
  rxc_ser_total.ser4 = 0;
  rxc_ser_total.ser8 = 0;

} /* rxc_ser_reset */


/*===========================================================================

FUNCTION RXC_GET_SC_CRC

DESCRIPTION
  This function retrieves the number of messages received on the Sync
  Channel with good and bad CRCs.

DEPENDENCIES
  None.

RETURN VALUE
  See above.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_get_sc_crc
(
  rxc_crc_cnt_type *crc_ptr
    /* pointer to where crc counters should be placed */
)
{
  crc_ptr->good_crcs = rxc_sc_crc_good;
  crc_ptr->bad_crcs = rxc_sc_crc_bad;

} /* rxc_get_sc_crc */


/*===========================================================================

FUNCTION RXC_GET_PC_CRC

DESCRIPTION
  This function retrieves the number of messages received on the Paging
  Channel with good and bad CRCs.

DEPENDENCIES
  None.

RETURN VALUE
  See above.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_get_pc_crc
(
  rxc_crc_cnt_type *crc_ptr
    /* pointer to where crc counters should be placed */
)
{
  crc_ptr->good_crcs = rxcpaging_data.rxc_pc_crc_good;
  crc_ptr->bad_crcs = rxcpaging_data.rxc_pc_crc_bad;

} /* rxc_get_pc_crc */


/*===========================================================================

FUNCTION DUMP_DEMBACK_REGISTERS

DESCRIPTION
  This function dumps out demback HW state to help with debug

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void dump_demback_registers( void )
{

  if ( !rxc_use_sw_demback() )
  {
    /* Dump out registers again to see if the above had any effect */
    M1X_MSG( MUX, LEGACY_FATAL,
             "[DEMBACK_ID=%u] "
             "DEMBACK_MODE_SEL=0x%x "
             "SVD_MODE=0x%x "
             "DEC_DONE_INT_STATUS=0x%x "
             "DEINT_CLK_STATUS = 0x%x "
             "DEINT_TASK_STATUS = 0x%x",
             dec_demback_id,
             GET_DEMBACK_REG( DEMBACK_MODE_SEL ),
             GET_DEMBACK_REG_DW( SVD_MODE ),
             GET_DEMBACK_REG( DEC_DONE_INT_STATUS ),
             GET_DEMBACK_REG( DEINT_CLK_STATUS ),
             GET_DEMBACK_REG( DEINT_TASK_STATUS ) );
  }
} /* dump_demback_registers */


/*===========================================================================

FUNCTION RXC_WAIT

DESCRIPTION
  This function continues to reset the Watchdog signal and timer while
  waiting for any signal in a specified signal mask to be set. If this
  procedure detects that any signal in the specified signal mask is set,
  it will return the entire mask of signals set in the task'stcb.

DEPENDENCIES
  None.

RETURN VALUE
  Mask of signals returned by the rex_wait call.

SIDE EFFECTS
  None.

===========================================================================*/
rex_sigs_type rxc_wait
(
  rex_sigs_type wait_sigs
    /* signals to wait on */
)
{
  rex_sigs_type sigs;
  boolean failsafe_set = FALSE;
  uint8 recovery_count = 0;
  rxc_state_type recovery_state = RXC_CDMA_STATE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - -*/

  /* keep kicking watchdog while waiting for requested signals */
  while( ( ( sigs = rex_get_sigs( RX_TCB_PTR )) & wait_sigs ) == 0 )
  {
    if ( ( wait_sigs & RXC_ISR_CMD_SIG ) && ( sigs & RXC_TIMER_SIG ) )
    {
      /* The failsafe timer has expired!!! */
      M1X_MSG( MUX, LEGACY_FATAL,
        "ISR command failsafe timer expired!"
        " State %d, Recovery Step %d",
        rxc_common_data.rxc_state,
        recovery_count );

      /* Dump out interesting registers */
      dump_demback_registers();

      /* If we haven't hit max retries on recovery, keep trying */
      if ( recovery_count < RXC_ISR_LOCKUP_MAX_COUNT )
      {
        boolean use_sw_demback = rxc_use_sw_demback();
        recovery_state = rxc_common_data.rxc_state;

        if (rxc_processing_TT_F && rxc_common_data.rxc_state == RXC_CDMA_STATE)
        {
          recovery_state = RXC_PC_STATE;
        }

        switch (recovery_state)
        {
          case RXC_CDMA_STATE:
          case RXC_SC_STATE:
          case RXC_SC_STATE_SW:
            M1X_MSG( MUX, LEGACY_FATAL,
              "Reinstalling SC ISR" );

            dec_config_clk_resources( DEC_CLK_SYNC );
            /* Configure FW for HW/SW usage */
            muxmdsp_set_demback_mode( !use_sw_demback, FALSE );

            if( !use_sw_demback )
            {
              deint_init();
              deint_set_sc_mode();  /* Set deint/dec to sync mode */
            }

            rxc_sc_init_isr( use_sw_demback );
            break;

          case RXC_PC_STATE:
          case RXC_PC_STATE_SW:
            M1X_MSG( MUX, LEGACY_FATAL,
              "Reinstalling PC ISR" );

            dec_config_clk_resources( DEC_CLK_IDLE );

            /* Configure FW for HW/SW usage */
            muxmdsp_set_demback_mode( !use_sw_demback, TRUE );
            if( !use_sw_demback )
            {
              deint_init();
              deint_set_pc_mode ((deint_pc_rate_type)rxc_get_pc_rate());
              deint_set_dint_cfg();
            }
            rxc_config_demod_for_paging();
            rxc_set_pc_isr( use_sw_demback, rxc_get_pc_rate() );
            break;

          default:
            M1X_MSG( MUX, LEGACY_FATAL,
              "Unhandled rxc_state %d",
              recovery_state );
            break;
        }
      }
      else
      {
        /* Send a system restart to MC */
        mccsrch_rpt_type *rpt_ptr;

        if ( (rpt_ptr = (mccsrch_rpt_type  *)
        #ifdef FEATURE_1X_CP_MEM_OPT
              mc_alloc_queue_buf( MCC_SRCH_Q )) == NULL )
        #else /* !FEATURE_1X_CP_MEM_OPT*/
              q_get ( &mcc_srch_free_q )) == NULL )
        #endif /* FEATURE_1X_CP_MEM_OPT*/
        {
          /*Attempts to recover have failed. Lock up so we can dump out data */
          ONEX_ERR_FATAL("DECODER ISR LOCK UP, NO BUFFERS - SOFT RECOVERY FAILED",
                     0, 0, 0);
        }
        else /* We got a report buffer. */
        {
          /* Command to ACK to MC */
          rxc_cmd_msg_type *cmd_ptr;

          /* Send ACK to MC for any outstanding commands */
          while( (cmd_ptr = (rxc_cmd_msg_type *)
                     q_get(&rxc_common_data.rxc_cmd_q) ) != NULL )
          {
            /* DONE_S to unblock MC to process other commands */
            cmd_ptr->hdr.status = RXC_DONE_S;

            /* return item to queue if necessary */
            cmd_done( &cmd_ptr->hdr.cmd_hdr );

            (void)rex_clr_sigs( RX_TCB_PTR, RXC_CMD_Q_SIG );
          }

          M1X_MSG( MUX, LEGACY_FATAL,
             "1x MUX sending system restart to MC, lost decoder ints" );

          /* System Restart command to MC */
          rpt_ptr->hdr.rpt = SRCH_REQUEST_SYS_RESTART_R;

          /* Send SRCH report to MC */
          srch_rpt( rpt_ptr );
        }
      }

      /* Dump out registers again to see if the above had any effect */
      dump_demback_registers();

      /* increment recovery count for the next attempt */
      recovery_count++;
    }

    /* Check in with Watchdog task */
    rxc_dog_report();

    if( wait_sigs & RXC_ISR_CMD_SIG )
    {
      RX_DBG_MSG( "Setting failsafe timer" );
      (void) rex_clr_sigs ( RX_TCB_PTR, RXC_TIMER_SIG );
      (void) rex_set_timer( &rxc_failsafe_timer, 100 );
      failsafe_set = TRUE;
    }

    (void) rex_wait( wait_sigs | RXC_TIMER_SIG | RXC_DOG_RPT_SIG );
  }

  /* kick watchdog one more time on the way out */
  rxc_dog_report();

  if( failsafe_set == TRUE )
  {
    RX_DBG_MSG( "Clearing failsafe timer" );
    (void) rex_clr_timer( &rxc_failsafe_timer );
    (void) rex_clr_sigs( RX_TCB_PTR, RXC_TIMER_SIG );
  }

  if( recovery_count > 0 )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
       "Recovery Mechanism Succeeded after ISR Lockup" );

    dump_demback_registers();
  }

  return( sigs );

} /* rxc_wait */


/*===========================================================================

FUNCTION RXC_MC_RPT

DESCRIPTION
  This function generates a report to the Main Control task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_mc_rpt
(
  mccrx_rpt_type *rpt_ptr
    /* pointer to report to be sent to the Main Control task */
)
{

  rpt_ptr->hdr.rpt_hdr.done_q_ptr = &mcc_rx_free_q;
  rpt_ptr->hdr.rpt_hdr.task_ptr = NULL;

  /* Enqueue report to Main Control task */
  q_put( &mcc_rx_rpt_q, &rpt_ptr->hdr.rpt_hdr.link );
  (void) rex_set_sigs( MC_TCB_PTR, MCC_Q_SIG );

} /* rxc_mc_rpt */


/*===========================================================================

FUNCTION SEND_PWR_RPT

DESCRIPTION
  This function sends a power report to the Main Control task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void send_pwr_rpt( void )
{
  mccrx_rpt_type *rpt_ptr;
    /* Pointer to report to be sent to the Main Control task */

  boolean rxc_ta_is_enabled = rxc_is_ta_enabled();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - -*/

  if ( rxc_ta_is_enabled )
  {
    M1X_MSG( MUX, LEGACY_HIGH,"1x2GTA: Suspend PWR rpt" );
    return;
  }

  if ((rpt_ptr = (mccrx_rpt_type *)q_get( &mcc_rx_free_q )) == NULL)
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "No buffers on mcc_rx_free_q" );

    /* Reset counts */
    rxc_pwr.num_frames     = 0;
    rxc_pwr.skipped_num_frames     = 0;
    rxc_pwr.num_frames_current = 0;
    rxc_pwr.num_bad_frames = 0;
    rxc_pwr.num_bad_frames_current = 0;

    #ifdef FEATURE_IS2000_REL_A_SVD
    rxc_pwr.dcch_num_frames     = 0;
    rxc_pwr.skipped_dcch_num_frames     = 0;
    rxc_pwr.dcch_num_frames_current = 0;
    rxc_pwr.dcch_num_bad_frames = 0;
    rxc_pwr.dcch_num_bad_frames_current = 0;
    #endif /* FEATURE_IS2000_REL_A_SVD */
  }
  else
  {
    /* Generate power measurement report for MC */
    rpt_ptr->pwr.hdr.rpt = RXC_PWR_R;

    #ifdef FEATURE_IS2000_REL_A_SVD
    /* Report power measurements detected as follows:
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
      rpt_ptr->pwr.pwr_meas_frames      = rxc_pwr.num_frames;
      rxc_pwr.num_frames_current        = rxc_pwr.num_frames;
      rpt_ptr->pwr.errors_detected      = (byte)rxc_pwr.num_bad_frames;
      rxc_pwr.num_bad_frames_current      = rxc_pwr.num_bad_frames;
      rpt_ptr->pwr.dcch_pwr_meas_incl   = TRUE;
      rpt_ptr->pwr.dcch_pwr_meas_frames = rxc_pwr.dcch_num_frames;
      rxc_pwr.dcch_num_frames_current   = rxc_pwr.dcch_num_frames;
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
    /* Default to DCCH only scenario.  For this scenario, standard indicates
    ** that power measurement report represents DCCH errors and
    ** dcch_pwr_meas_incl is set to FALSE.
    */
    else if (rxc_so.for_dcch.included)
    {
      rpt_ptr->pwr.pwr_meas_frames      = rxc_pwr.dcch_num_frames;
      rxc_pwr.dcch_num_frames_current   = rxc_pwr.dcch_num_frames;
      rpt_ptr->pwr.errors_detected      = (byte)rxc_pwr.dcch_num_bad_frames;
      rxc_pwr.dcch_num_bad_frames_current = rxc_pwr.dcch_num_bad_frames;
      M1X_MSG( MUX, LEGACY_MED,
        "After PMRM Sent, Frame Count: DCCH %d, DCCH Bad Frame %d",
        rxc_pwr.dcch_num_frames,
        rxc_pwr.dcch_num_bad_frames);
    }
    #else /* FEATURE_IS2000_REL_A_SVD */
    rpt_ptr->pwr.pwr_meas_frames = rxc_pwr.num_frames;
    rxc_pwr.num_frames_current   = rxc_pwr.num_frames;
    rpt_ptr->pwr.errors_detected = (byte)rxc_pwr.num_bad_frames;
    rxc_pwr.num_bad_frames_current = rxc_pwr.num_bad_frames;
    M1X_MSG( MUX, LEGACY_MED,
        "After PMRM Sent, Frame Count: FCH %d, FCH Bad Frame %d",
        rxc_pwr.num_frames,
        rxc_pwr.num_bad_frames);
    #endif /* FEATURE_IS2000_REL_A_SVD */

    rpt_ptr->pwr.sch_pwr_meas_included = FALSE;
    rxc_mc_rpt( rpt_ptr );

    /* Turn power control reporting off until PWR_REP_DELAY countdown */
    rxc_pwr.threshold_on = FALSE;
    rxc_pwr.periodic_on = FALSE;
  }

} /* send_pwr_rpt */

/*===========================================================================

FUNCTION RXC_FLAG_GOOD_FRAME

DESCRIPTION
  This function is called by SRCH during a Return-If-Fail HHO, after
  verifying none of the HHO failure conditions is met.  This is to
  inform RXC that it's okay to start processing good frame.

DEPENDENCIES
  This function must not be called unless a return-if-fail hard handoff
  is in progress.

RETURN VALUE
  None.

SIDE EFFECTS
  RXC will start processing good frame

===========================================================================*/
void rxc_flag_good_frame( void )
{
  M1X_MSG( MUX, LEGACY_MED,
    "SRCH : don't ignore good frame %d, %d",
    rxc_hho_data.rxc_lost_frames,
    rxc_hho_data.rxc_rif_hho_timeout_frames );
  /* If the handoff has already timed out, leave this flag set to TRUE */
  if (rxc_hho_data.rxc_lost_frames < rxc_hho_data.rxc_rif_hho_timeout_frames)
  {
    rxc_ignore_good_frame = FALSE;
  }

} /* rxc_flag_good_frame */


/*===========================================================================

FUNCTION RXC_ISR_CMD

DESCRIPTION
  This procedure posts a command for the interrupt handler, continues to
  kick the watchdog while waiting for a response and returns the response.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_isr_cmd
(
  rxc_isr_cmd_type *cmd_ptr
    /* pointer to command to be posted for the interrupt handler */
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* put command on queue */
  (void) rex_clr_sigs( RX_TCB_PTR, RXC_ISR_CMD_SIG );
  q_put( &rxc_common_data.rxc_isr_q, &cmd_ptr->link );

  /* Clear buffers from decoder queue */
  rxc_clear_decoder_queue ();

  /* SRCH or MC could send MUX multiple cmds at a time, so if the queue is not
     empty the rxc task should go on and process the pending cmd in the queue
  */
  if ( rxc_common_data.rxc_cmd_q.cnt == 0 )
  {
    /* Wait for interrupt handler to process command
    ** Also wait for a command from SRCH
    */
    (void) rxc_wait( RXC_ISR_CMD_SIG | RXC_CMD_Q_SIG );

    (void) rex_clr_sigs( RX_TCB_PTR, RXC_ISR_CMD_SIG );
  }

} /* rxc_isr_cmd */

/*===========================================================================

FUNCTION RXC_SETUP_TRAFFIC_ON_TT_F

DESCRIPTION
  This function setups the deint/dec for traffic when processing RXC_TT_F.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_setup_traffic_on_tt_f( void )
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  dec_config_clk_resources( DEC_CLK_IDLE_CSFB );

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

  #ifdef FEATURE_IS2000_REL_A
  /* Always returns TRUE.  Ignoring return value. */
  (void) rxctraffic_frame_bnd_init();
  #else /* FEATURE_IS2000_REL_A */
  /* switch to Traffic Channel and use initial rate set */
  if( rxc_so.for_dcch.included )
  {
    // Note: for MSM5000, FCH and DCCH shares the same hardware
    deint_set_fch_mode( (deint_rc_type) rxc_so.for_dcch.rc );
  }
  else if( rxc_so.for_fch.included )
  {
    deint_set_fch_mode( (deint_rc_type) rxc_so.for_fch.rc );
  }

  #ifdef FEATURE_IS2000_1X_ADV
  if( rxc_so.for_fch.included )
  {
    /* Select the appropriate ISR for frame processing depending on the FCH RC. */
    rxc_tc_isr_select();
  }

  /* Check if this is a 1xA call and set clocks accordingly. */
  if( rxc_so.for_fch.rc == CAI_RC_11)
  {
    dec_config_clk_resources( DEC_CLK_VOICE_TRAFFIC );
  }
  else
  {
    dec_config_clk_resources( DEC_CLK_IDLE_CSFB );
  }
  #endif /* FEATURE_IS2000_1X_ADV */

  rxc_register_isr( (dalint_1x_isr) rxc_tc_isr, DALINT_1X_STACK_4K );

  /* Clear DEC DONE int status to make sure ISR is properly serviced. */
  SET_DEMBACK_REG( DEC_DONE_INT_STATUS, 0x7 );

  #endif /* FEATURE_IS2000_REL_A */

} /* rxc_setup_traffic_on_tt_f */

/*===========================================================================

FUNCTION RXC_CDMA

DESCRIPTION
  This function processes the state of the RXC state machine after the
  Decoder Chip has been initialized.  During this state the Receive task
  is waiting for a command to go the Sync Channel state or to Exit.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/
rxc_state_type rxc_cdma( void )
{

  rxc_state_type next_state;
    /* next state to be processed */
  rxc_cmd_msg_type *cmd_ptr;
    /* pointer to command buffer received on rxc_cmd_q */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  next_state = RXC_CDMA_STATE;

  dec_config_clk_resources( DEC_CLK_DISABLED );

  while( next_state == RXC_CDMA_STATE )
  {
    /* check for received commands */
    (void)rex_clr_sigs( RX_TCB_PTR, RXC_CMD_Q_SIG );

    if( ( cmd_ptr = (rxc_cmd_msg_type *)q_get( &rxc_common_data.rxc_cmd_q ) ) )
    {
      switch( cmd_ptr->hdr.command )
      {

        case RXC_EXIT_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_EXIT_F" );

          cmd_ptr->hdr.status = RXC_DONE_S;
          next_state          = RXC_EXIT_STATE;

          /* If enabled, send accumulated RLP Frames log. */
          dsrlp_log_rx_send_frames();
          #ifndef FEATURE_IS2000_REL_A
          /* If enabled, send accumulated frame types log. */
          rxc_frame_types_send_log();
          #else
          rxc_fdch_frame_info_send_log();
          #endif /* FEATURE_IS2000_REL_A */

          /* If enabled, send accumulated FFPC log. */
          RXC_ENTER_CRITICAL_SECTION();
          ffpc_send_log();
          RXC_LEAVE_CRITICAL_SECTION();
          /* If enabled, send accumulated RDA log. */
          rxc_rda_frame_info_send_log();
          break;
        }

        case RXC_SC_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_SC_F" );

          rxc_sc_init( RXC_CDMA_STATE);
          cmd_ptr->hdr.status = RXC_DONE_S;
          next_state          = RXC_SC_STATE;
          break;
        }

        #if defined( FEATURE_VOIP_1X_HANDOFF ) || defined( FEATURE_MODEM_1X_IRAT_LTO1X )
        case RXC_IDLE_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_IDLE_F" );

          if (cmd_ptr->idle.chan_info.chan_id == CAI_PCH)
          {
            rxc_pc_init( RXC_CDMA_STATE,
                         cmd_ptr->idle.chan_info.config.pch.rate );

            cmd_ptr->hdr.status = RXC_DONE_S;
            next_state          = RXC_PC_STATE;
            break;
          }
        }

        case RXC_TC_TT_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_TC_TT_F" );

          rxc_processing_TT_F = TRUE;

          /* Prevent RXC from fade checking during TT prior to HHO_BEGIN */
          M1X_MSG( MUX, LEGACY_MED,
            "Suspend fade checking during TT" );

          rxc_fade_data.rxc_pre_tt_ho = TRUE;

          /* Initialize RX for traffic. */
          M1X_MSG( MUX, LEGACY_MED,
            "Perform TCI init." );

          rxc_tci_init ( RXC_PC_STATE, cmd_ptr, TRUE );

          rxc_setup_traffic_on_tt_f();

          /* Transition to normal FL traffic processing */
          M1X_MSG( MUX, LEGACY_MED,
            "Transition to full traffic state." );
          rxc_tc_init( RXC_TCI_STATE );

          rxc_processing_TT_F = FALSE;

          M1X_MSG( MUX, LEGACY_MED,
            "RXC now operating in RXC_TC_STATE." );

          cmd_ptr->hdr.status = RXC_DONE_S;
          next_state          = RXC_TC_STATE;
          break;
        }
        #endif /* (FEATURE_VOIP_1X_HANDOFF || FEATURE_MODEM_1X_IRAT_LTO1X) */

        case RXC_CDMA_F:
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "Warning - RXC_CDMA_F received in RXC_CDMA_STATE" );
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
      }/* switch */

      /* Place item on queue if requested */
      cmd_done( &cmd_ptr->hdr.cmd_hdr );
    }
    else
    {
      /* wait for command or interrupt */
      (void)rxc_wait( RXC_CMD_Q_SIG );
    }
  } /* while */

  return next_state;

} /* rxc_cdma */


/*===========================================================================

FUNCTION RXC_POWERUP_INIT

DESCRIPTION
  This function performs initialization of Receive task variables and
  queues.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Queues can be used.

===========================================================================*/
void rxc_powerup_init( void )
{
  word i;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Set frame count to 0 */
  qw_set( rxc_dec_frame, 0L, 0L );

  /* Initialize queues */
  (void)q_init( &rxc_common_data.rxc_cmd_q );
  (void)q_init( &rxc_common_data.rxc_isr_q );
  (void)q_init( &rxc_common_data.rxc_dec_q );
  (void)q_init( &rxc_common_data.rxc_dec_free_q );
  (void)q_init( &rxc_common_data.rxc_isr_free_q );
  deint_queue_init();

  /* Fill rxc_dec_free_q */
  for( i=0; i< RXC_NUM_DEC_BUFS ; i++ )
  {
    q_put( &rxc_common_data.rxc_dec_free_q,
           q_link( &rxc_dec_bufs[i], &rxc_dec_bufs[i].link ) );
  }

  /* Fill rxc_isr_free_q */
  for( i=0; i< RXC_NUM_ISR_CMD_BUFS ; i++ )
  {
    q_put( &rxc_common_data.rxc_isr_free_q,
           q_link( &rxc_isr_cmd_bufs[i], &rxc_isr_cmd_bufs[i].link ) );
  }

  /* Initialize RLP frames logging buffer */
  dsrlp_log_rx_init_frm_buf();

  /* Initialize markov log */
  rxc_bufs_qs_data.rxc_log_mar_buf.hdr.code = LOG_MAR_C;
  rxc_bufs_qs_data.rxc_log_mar_buf.hdr.len  = FPOS(mux_log_mar_type, entry);//lint !e734 FPOS
  rxc_bufs_qs_data.rxc_log_mar_buf.count    = 0;

  /* Register to DIAG all the MUX1X command handlers */
  mux1xdiag_init();

  #ifdef FEATURE_IS2000_1X_ADV
  /* Install the isr handler to service MDSP_INT_0 interrupts. */
  srch_mux_install_mdsp_isr_handler();
  #endif /*FEATURE_IS2000_1X_ADV */

  /* Perform DAL software switches initialization */
  cdma2kdiag_switch_init();

  /* Initialize RXC_ISR params */
  rxc_isr_data.irq     = DALINT_1X_MAX_IRQ;
  rxc_isr_data.handler = NULL;

} /* rxc_powerup_init */


/*===========================================================================

FUNCTION RXC_EXIT

DESCRIPTION
  This function performs processing necessary when leaving the RX CDMA
  subtask

DEPENDENCIES
  none.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_exit( void )
{
  /* set interrupt handler to null */
  rxc_clear_isr();

  /* kick watchdog one more time */
  rxc_dog_report();

  /* clear timers */
  rxcidle_clr_fade_timer();

  /* clear signals */
  (void)rex_clr_sigs( RX_TCB_PTR, 0xFFFF );

  /* Make sure page matching is completely disabled */
  rxc_slot.skipping   = FALSE;
  rxc_slot.page_match = FALSE;
  rxc_slot.blank      = FALSE;

  ffpc_disable();

  /* If FFPC logging enabled ... */
  if (log_status(LOG_FFWD_PCTRL_C))
  {
    /* flush out accumulated logs */
    RXC_ENTER_CRITICAL_SECTION();
    ffpc_send_log();
    RXC_LEAVE_CRITICAL_SECTION();
  }

} /* rxc_exit */


/*===========================================================================

FUNCTION RXC_SUBTASK

DESCRIPTION
  This function contains the state machine for the CDMA rx subtask.

DEPENDENCIES
  none.

RETURN VALUE
  None.

SIDE EFFECTS
  Receive task will start.

===========================================================================*/
void rxc_subtask( void )
{

  rxc_cmd_msg_type *cmd_ptr;
    /* pointer to command from MC */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void)rex_clr_sigs( RX_TCB_PTR, RXC_CMD_Q_SIG );

  if((cmd_ptr = (rxc_cmd_msg_type *)q_get(&rxc_common_data.rxc_cmd_q)) == NULL)
  {
    ONEX_ERR_FATAL( "RXC subtask activation error ", 0, 0, 0 );
  }
  else
  {
    /* acknowledge the command */
    cmd_ptr->hdr.status = RXC_DONE_S;
    cmd_done( &cmd_ptr->hdr.cmd_hdr );

    if( cmd_ptr->hdr.command != RXC_CDMA_F )
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Warning: Ignoring invalid command (%d) for RXC inactive state.",
        cmd_ptr->hdr.command );
    }
    else
    {
      rxc_common_data.rxc_state = RXC_CDMA_STATE;

      while( rxc_common_data.rxc_state != RXC_EXIT_STATE )
      {

        switch( rxc_common_data.rxc_state )
        {
          case RXC_CDMA_STATE:
          {
            rxc_common_data.rxc_state = rxc_cdma();  /* CDMA state */
            break;
          }

          case RXC_SC_STATE:
          {
            rxc_common_data.rxc_state = rxc_sc(); /* Process Sync Chan. data */
            break;
          }

          case RXC_PC_STATE:
          {
            /* Process Paging Chan data */
            rxc_common_data.rxc_state = rxc_pc();
            break;
          }

          case RXC_TCI_STATE:
          {
            /* Process Traffic Channel initialization */
            rxc_common_data.rxc_state = (rxc_state_type)rxc_tci();
            break;
          }

          case RXC_TC_STATE:
          {
            /* Process Traffic Channel data */
            rxc_common_data.rxc_state = (rxc_state_type)rxc_tc();
            break;
          }

          case RXC_TA_STATE:
          {
            /* Process Traffic Channel data */
            rxc_common_data.rxc_state = (rxc_state_type)rxc_ta();
            break;
          }

          default:
          {
            ONEX_ERR_FATAL( "Bad state %d", (int) rxc_common_data.rxc_state, 0, 0 );
          }

        } /* switch */

      } /* while */

      rxc_exit();  /* perform exit processing, including clearing Dec ISR */

      dec_config_clk_resources(DEC_CLK_DISABLED);
    }
  }

} /* rxc_subtask */


/*===========================================================================

FUNCTION RXC_CMD

DESCRIPTION
  This function should be called to send a command to the Receive task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Receive task will start.

===========================================================================*/
void rxc_cmd
(
  rxc_cmd_msg_type *cmd_ptr
    /* pointer to command to be enqueued */
)
{
  RXC_ENTER_ATOMIC_SECTION();
  /* Initialize link field */
  (void) q_link( cmd_ptr, &cmd_ptr->hdr.cmd_hdr.link );

  cmd_ptr->hdr.status = RXC_BUSY_S;

  /* Put item on queue for rxc_task and set signal for task */
  q_put( &rxc_common_data.rxc_cmd_q, &cmd_ptr->hdr.cmd_hdr.link );
  (void)rex_set_sigs( RX_TCB_PTR, RXC_CMD_Q_SIG );
  RXC_LEAVE_ATOMIC_SECTION();

} /* rxc_cmd */


/*===========================================================================

FUNCTION RXC_REGISTER_ISR

DESCRIPTION
  This function registers for the DEC DONE ISR.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_register_isr
(
  dalint_1x_isr         isr_handler,
  dalint_1x_stack_size  stack_size
)
{
  if( isr_handler == NULL )
  {
    ONEX_ERR_FATAL( "NULL ISR handler passed for RX interrupt", 0, 0, 0 );
    return;
  }

  if( ( rxc_isr_data.handler == NULL ) ||
      ( rxc_isr_data.handler != isr_handler ) )
  {
    /* Get the IRQ for the current DEMBACK in use */
    rxc_isr_data.irq = muxmdsp_get_demback_irq();

    /* Register for the RX interrupt */
    dalint_1x_intr_reg( DALINT_1X_RX,
                        rxc_isr_data.irq,
                        isr_handler,
                        stack_size );

    /* Remember that the ISR is registered */
    rxc_isr_data.handler = isr_handler;

    M1X_MSG( SRCH, LEGACY_LOW,
      "Registered/Updated RX ISR" );
  }
  else
  {
    M1X_MSG( SRCH, LEGACY_ERROR,
      "Duplicate registration attempt for RX ISR" );
  }

  (void) rex_clr_sigs( RX_TCB_PTR, RXC_INT_SIG );

} /* rxc_register_isr */


/*===========================================================================

FUNCTION RXC_CLEAR_ISR

DESCRIPTION
  This function de-registers from DEC DONE ISR

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_clear_isr( void )
{
  if( rxc_isr_data.handler != NULL )
  {
    /* Turn off interrupt */
    (void) dalint_1x_intr_dereg( DALINT_1X_RX, rxc_isr_data.irq );

    /* Clear IRQ and ISR as it is not registered anymore */
    rxc_isr_data.irq     = DALINT_1X_MAX_IRQ;
    rxc_isr_data.handler = NULL;

    M1X_MSG( SRCH, LEGACY_LOW,
      "Cleared RX ISR" );
  }
  else
  {
    M1X_MSG( SRCH, LEGACY_ERROR,
      "Duplicate clear attempt for RX ISR" );
  }

  (void)rex_clr_sigs( RX_TCB_PTR, RXC_INT_SIG );

} /* rxc_clear_isr */


/*===========================================================================

FUNCTION RXC_LOAD_PC_ISR

DESCRIPTION
  This externally-called function installs the PC ISR and controls
  whether or not the decoder should have its state metrics
  cleared until the first frame in the slot.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_load_pc_isr ( boolean blank_frame )
{
  /*-----------------------------------------------------------------------*/

  RXC_ENTER_ATOMIC_SECTION();
  /* Install the interrupt handler - now! */
  rxc_register_isr( (dalint_1x_isr) rxc_pc_isr, DALINT_1X_STACK_4K );

  /* Clear DEC DONE int status to make sure ISR is properly serviced */
  SET_DEMBACK_REG( DEC_CLR_INT_STATUS,
                   HWIO_FMSK( DEC_CLR_INT_STATUS, VD_CLR_STATUS ) );

  RXC_LEAVE_ATOMIC_SECTION();

  /* Remember whether we should clear path metrics or not. */
  rxc_slot.blank = blank_frame;

} /* rxc_load_pc_isr */

/*========================================================================

FUNCTION RXC_CONFIG_DEMOD_FOR_SYNC

DESCRIPTION This function setups demod channel 0 for sync

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_config_demod_for_sync( void )
{
  muxmdsp_demod_walsh_beta_type walsh_beta;
  muxmdsp_demod_ch0_type ch0;

  /*-----------------------------------------------------------------------*/

  walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_64;
  walsh_beta.swBeta          = 0x10;
  muxmdsp_update_demod_walsh_beta_cfg( MUXMDSP_DEMOD_CH0, &walsh_beta );

  ch0.combEnable = TRUE;
  ch0.rateSel    = DMOD_SPR_256;
  ch0.bpsk       = TRUE;
  ch0.syncEnable = TRUE;
  ch0.ffeBias    = MUXMDSP_FFEBIAS_WL_MASK_64;
  ch0.qpchEnable = FALSE;
  ch0.qpchOokPosition = 0;
  muxmdsp_update_demod_ch0( &ch0 );

} /* rxc_config_demod_for_sync */

/*========================================================================

FUNCTION RXC_CONFIG_DEMOD_FOR_PAGING

DESCRIPTION This function setups demod channel 0 for paging

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_config_demod_for_paging( void )
{
  muxmdsp_demod_walsh_beta_type walsh_beta;
  muxmdsp_demod_ch0_type ch0;

  /*-----------------------------------------------------------------------*/

  walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_64;
  walsh_beta.swBeta          = 0x20;
  muxmdsp_update_demod_walsh_beta_cfg( MUXMDSP_DEMOD_CH0, &walsh_beta );

  ch0.combEnable = TRUE;
  ch0.rateSel    = DMOD_SPR_64;
  ch0.bpsk       = TRUE;
  ch0.syncEnable = FALSE;
  ch0.ffeBias    = MUXMDSP_FFEBIAS_WL_MASK_64;
  ch0.qpchEnable = FALSE;
  ch0.qpchOokPosition = 0;

  muxmdsp_update_demod_ch0( &ch0 );

} /* rxc_config_demod_for_paging */

/*========================================================================

FUNCTION RXC_CONFIG_DEMOD_FOR_TRAFFIC

DESCRIPTION This function setups demod channel 0 for traffic

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_config_demod_for_traffic( deint_rc_type rc )
{
  muxmdsp_demod_walsh_beta_type walsh_beta;
  muxmdsp_demod_ch0_type ch0;
  boolean update     = TRUE;

  /*-----------------------------------------------------------------------*/

  RXC_ENTER_ATOMIC_SECTION();

  switch (rc )
  {
    case DEINT_RC1:
    case DEINT_RC2:
    {
      ch0.combEnable             = TRUE;
      ch0.rateSel                = DMOD_SPR_64;
      ch0.bpsk                   = TRUE;
      ch0.ffeBias                = MUXMDSP_FFEBIAS_WL_MASK_64;
      walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_64;
      walsh_beta.swBeta          = 0x20;
      break;
    }
    case DEINT_RC3:
    case DEINT_RC5:
    {
      ch0.combEnable             = TRUE;
      ch0.rateSel                = DMOD_SPR_64;
      ch0.bpsk                   = FALSE;
      ch0.ffeBias                = MUXMDSP_FFEBIAS_WL_MASK_64;
      walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_64;
      walsh_beta.swBeta          = 0x20;
      break;
    }
    case DEINT_RC4:
    case DEINT_RC11:
    {
      ch0.combEnable             = TRUE;
      ch0.rateSel                = DMOD_SPR_128;
      ch0.bpsk                   = FALSE;
      ch0.ffeBias                = MUXMDSP_FFEBIAS_WL_MASK_128;
      walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_128;
      walsh_beta.swBeta          = 0x17;
      break;
    }
    default:
    {
      M1X_MSG( MUX, LEGACY_FATAL,
        "Illegal RC ");
      update = FALSE;
    }

  } /* end switch */

  /* Update only for a valid RC */
  if( update )
  {
    muxmdsp_update_demod_walsh_beta_cfg( MUXMDSP_DEMOD_CH0, &walsh_beta );

    ch0.syncEnable = FALSE;
    ch0.qpchEnable = FALSE;
    ch0.qpchOokPosition = 0;
    muxmdsp_update_demod_ch0( &ch0 );

    muxmdsp_update_pwr_ctrl_rpc( rc );
  }

  RXC_LEAVE_ATOMIC_SECTION();

} /* rxc_config_demod_for_traffic */

#ifdef FEATURE_IS2000_REL_A_SVD
/*========================================================================

FUNCTION RXC_CONFIG_DEMOD_CH1

DESCRIPTION This function configures the demodulator and combiner for
            Channel 1 based on Radio Configuration

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_config_demod_ch1( deint_rc_type rc )
{
  muxmdsp_demod_walsh_beta_type walsh_beta;
  muxmdsp_demod_ch1_type ch1;

  /*-----------------------------------------------------------------------*/

  ASSERT ((rc == DEINT_RC3) || (rc == DEINT_RC4) || (rc == DEINT_RC5));

  M1X_MSG( MUX, LEGACY_HIGH,
    "Config demod for traffic Ch 1");

  RXC_ENTER_ATOMIC_SECTION();

  if( rc == DEINT_RC4 )
  {
    ch1.numSymbols = 192;
    ch1.ffeBias = MUXMDSP_FFEBIAS_WL_MASK_128;
    walsh_beta.swBeta = 0x5B;
    walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_128;
  }
  else
  {
    ch1.ffeBias = MUXMDSP_FFEBIAS_WL_MASK_128;
    ch1.numSymbols = 384;
    walsh_beta.swBeta = 0x80;
    walsh_beta.walshLengthMask = MUXMDSP_WL_MASK_64;
  }

  muxmdsp_update_demod_walsh_beta_cfg(MUXMDSP_DEMOD_CH1, &walsh_beta);

  ch1.combEnable = TRUE;
  muxmdsp_update_demod_ch1(&ch1);

  #if defined( FEATURE_MODEM_1X_SW_DEMBACK ) && !defined( FEATURE_MODEM_1X_ATLAS_REVISIT )
  /* Dont inform FW if SW DEMBACK is in use */
  if( !muxmdsp_get_is_sw_demback_active() )
  #endif /* FEATURE_MODEM_1X_SW_DEMBACK && !FEATURE_MODEM_1X_ATLAS_REVISIT */
  {
    /* FW needs to be informed about setting up data bridge for CH1 as well on
       bolt, CH1 but not TDEC */
    muxmdsp_data_brdg_enable(TRUE, FALSE);
  }

  RXC_LEAVE_ATOMIC_SECTION();

} /* rxc_config_demod_ch1 */

/*========================================================================

FUNCTION RXC_SET_DCCH_MODE

DESCRIPTION This function handles the process of setting up DCCH.

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_set_dcch_mode
(
  deint_rc_type dcch_rc,
  boolean power_control_channel
)
{
  /* Since there are 2 frames lag time between when the DCCH is set up and
  ** when data is valid, need to count down these 2 frames.  Without this
  ** mechanism, MUX may deliver stale data to its clients.  The time line is
  ** shown below:
  **    Frame 0:  Start Deint programming by calling deint_set_dcch_mode()
  **    Frame 1:  In ATMR ISR, decrement countdown counter
  **    Frame 2:  In rxc_tc_isr, if count down counter is 1, decrement it
  **              and set data_valid flag.
  ** If the current data is already valid (not new channel, may be an
  ** RC change), do not activate the count down mechanism.
  */
  if( !( rxc_so.for_dcch.data_valid ) )
  {
    rxc_so.for_dcch.data_valid_countdown = 2;
  }

  deint_set_dcch_mode( dcch_rc, power_control_channel );

} /* rxc_set_dcch_mode */

/*========================================================================

FUNCTION RXC_SET_FCH_MODE

DESCRIPTION This function handles the process of setting up FCH.

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_set_fch_mode
(
  deint_rc_type fch_rc,
  boolean       power_control_channel
)
{
  #ifdef FEATURE_IS2000_1X_ADV
  if( rxc_so.for_fch.included &&
      fch_rc == DEINT_RC11 )
  {
    /* For RC11, it is not necessary to discard the first 2 frames. FCH data is
       considered valid at the beginning */
    rxc_so.for_fch.data_valid           = TRUE;
    rxc_so.for_fch.data_valid_countdown = 0;
  }
  else
  #endif /* FEATURE_IS2000_1X_ADV */

  /* Since there are 2 frames lag time between when the FCH is set up and
  ** when data is valid, need to count down these 2 frames.  Without this
  ** mechanism, MUX may deliver stale data to its clients.  The time line is
  ** shown below:
  **    Frame 0:  Start Deint programming by calling deint_set_fch_mode()
  **    Frame 1:  In ATMR ISR, decrement countdown counter
  **    Frame 2:  In rxc_tc_isr, if count down counter is 1, decrement it
  **              and set data_valid flag.
  ** If the current data is already valid (not new channel, may be an
  ** RC change), do not activate the count down mechanism.
  */
  if( !( rxc_so.for_fch.data_valid ) )
  {
    rxc_so.for_fch.data_valid_countdown = 2;
  }

  /* Set up the deinterleaver (and decoder) for the RC and power control chan
  */
  deint_set_fch_mode( fch_rc, power_control_channel );

} /* rxc_set_fch_mode */
#endif /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================

FUNCTION RXC_CLEAR_DECODER_QUEUE

DESCRIPTION
  This function clears the RXC decoder queue by placing all items on the
  decoder free queue.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_clear_decoder_queue( void )
{
  rxc_dec_type *buf_ptr = NULL;

  while( ( buf_ptr = (rxc_dec_type *) q_get( &rxc_common_data.rxc_dec_q ) ) )
  {
    q_put (&rxc_common_data.rxc_dec_free_q, &buf_ptr->link);
  }

} /* rxc_clear_decoder_queue */


/*===========================================================================

FUNCTION RXC_CLEAR_ISR_CMD_QUEUE

DESCRIPTION
  This function clears the RXC ISR COMMAND queue by placing all items on the
  ISR COMMAND free queue.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_clear_isr_cmd_queue( void )
{
  while ( (rxc_common_data.rxc_isr_cmd_buf_ptr = (rxc_isr_cmd_type *)
                                                   q_get (&rxc_common_data.rxc_isr_q)) )
  {
    /* Free up the buffer for the ISR */
    q_put( &rxc_common_data.rxc_isr_free_q,
           &rxc_common_data.rxc_isr_cmd_buf_ptr->link );
  }

} /* rxc_clear_isr_cmd_queue */

/*========================================================================

FUNCTION RXC_GET_SCH_INFO

DESCRIPTION This function returns current configuration information for
            the SCHs. This is used by the UI in the data display.

            PARAMETERS:
            sch_rc       - gives the Radio configuration of the R-SCH
            sch_rate_ptr - points to the current SCH rate.
            double_ptr   - is TRUE when double sized PDUs are being used
                           is FALSE when single sized PDUs are being used


DEPENDENCIES None

RETURN VALUE TRUE:  If SCH are assigned.
             FALSE: If no SCH are assigned.

SIDE EFFECTS None

=========================================================================*/
boolean rxc_get_sch_info
(
  dec_rc_type* sch_rc,         /* ptr to SCH RC */
  dec_sch_rate_type* sch_rate_ptr,
  boolean* double_ptr
)
{
  /* If Supps are active, set the rate and double size PDUs */
  if ((deint_rc_type)rxc_sch.rc != DEINT_RC_INVALID)
  {
    *sch_rate_ptr = (dec_sch_rate_type)
                      rxc_log_sch_data.rxc_stored_sch_packet.sch_rate;
    *double_ptr = rxc_log_sch_data.rxc_stored_sch_packet.double_size_pdu;
    *sch_rc = (dec_rc_type) rxc_log_sch_data.rxc_stored_sch_packet.rc;
    return TRUE;
  }
  else
  {
    /* Supps are not active, so return FALSE */
    return FALSE;
  }

} /* rxc_get_sch_info */

/*========================================================================

FUNCTION RXC_GET_FWD_LINK_RATE_SET

DESCRIPTION This function returns current rate set used on the forward
            linke based on the RC provided.
            This is used by the UI in the data display.

            PARAMETERS:
            rc = the current RC of a FL traffic channel.

DEPENDENCIES None

RETURN VALUE
            RC 1, 3, 4, 11
            0 = RS1 ->  9600 bps

            RC 2, 5
            1 = RS2 -> 14400 bps

            RC invalid
            default to RS1.

SIDE EFFECTS None

=========================================================================*/
dec_rate_set_type rxc_get_fwd_link_rate_set( cai_radio_config_type rc )
{
  dec_rate_set_type rate_set = DEC_RATE_9600;

  switch( rc )
  {
    case CAI_RC_1:
    case CAI_RC_3:
    case CAI_RC_4:
    case CAI_RC_11:
      rate_set = DEC_RATE_9600;
      break;

    case CAI_RC_2:
    case CAI_RC_5:
      rate_set = DEC_RATE_14400;
      break;

    default:
      M1X_MSG( MUX, LEGACY_ERROR,
        "Unsupported FL RC (%d). Defaulting to RS1",
        rc);
      rate_set = DEC_RATE_9600;
      break;
  }

  return rate_set;

} /* rxc_get_fwd_link_rate_set */

/*========================================================================

FUNCTION RXC_GET_FCH_RATE_SET

DESCRIPTION This function returns current Rate set used by FCH.
            This is used by the UI in the data display.

            PARAMETERS:
            None

DEPENDENCIES None

RETURN VALUE

SIDE EFFECTS None

=========================================================================*/
dec_rate_set_type rxc_get_fch_rate_set( void )
{
  return rxc_get_fwd_link_rate_set( rxc_so.for_fch.rc );

} /* rxc_get_fch_rate_set */

/*========================================================================

FUNCTION RXC_GET_DCCH_RATE_SET

DESCRIPTION This function returns current Rate set used by FCH.
            This is used by the UI in the data display.

            PARAMETERS:
            None

DEPENDENCIES None

RETURN VALUE
           Rate set of the DCCH.

SIDE EFFECTS None

=========================================================================*/
dec_rate_set_type rxc_get_dcch_rate_set( void )
{
  return rxc_get_fwd_link_rate_set( rxc_so.for_dcch.rc );

} /* rxc_get_dcch_rate_set */

/*===========================================================================

FUNCTION RXC_DCCH_IS_ON

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
boolean rxc_dcch_is_on( void )
{
  if( rxc_so.for_dcch.included )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }

} /* rxc_dcch_is_on */

/*===========================================================================

FUNCTION RXC_GET_CHAN_TC_DEMOD_CONFIG_PARAM

DESCRIPTION
  This function returns the demod setup parameters for SCH.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if paramaters could be found; otherwise, FALSE

SIDE EFFECTS
  None.

===========================================================================*/
boolean rxc_get_chan_tc_demod_config_param
(
  /* pointer to channel info */
  rxc_tc_chan_info_type          *chan_info,

  /* pointer to struct to return info in */
  rxc_chan_demod_config_param_type *demod_params
)
{
  boolean               status = FALSE;
  deint_rc_type         rc;
  cai_sch_rate_type     sch_rate;       // 1X, 2X, 4X, 8X, 16X

  /* check what channel the request is for */
  switch( chan_info->chan_id )
  {
    case RXC_F_SCH:
    {
      /* Copy data to local vars */
      rc        = (deint_rc_type) chan_info->config.sch.rc;
      sch_rate  = chan_info->config.sch.sch_rate;

      #ifdef FEATURE_IS2000_1X_ADV
      ASSERT ((rc == DEINT_RC3) || (rc == DEINT_RC4) ||
              (rc == DEINT_RC5) || (rc == DEINT_RC11));
      #else
      ASSERT ((rc == DEINT_RC3) || (rc == DEINT_RC4) || (rc == DEINT_RC5));
      #endif /* FEATURE_IS2000_1X_ADV */

      if ( rc < DEINT_RC3 )
      {
        /* Return bad status */
        status = FALSE;

        M1X_MSG( MUX, LEGACY_ERROR,
          "Call with bad RC %d",
          rc);
        break;
      }

      /* Update spreading rate from table */
      demod_params->spr_rate   =
        rxc_spr_rate_tab[DEINT_SCH_RC_INDEX(rc)]
                        [RXC_UPPER_BOUNDS_CHECK(sch_rate, NUM_SPR_RATES)];
      /* Update rounding factor from table */
      demod_params->rnd_factor =
        rxc_rnd_factor_tab[DEINT_SCH_RC_INDEX(rc)]
                          [RXC_UPPER_BOUNDS_CHECK(sch_rate, NUM_RND_FACT)];

      /* Repetition factor is always 1 */
      demod_params->rep_factor = DMOD_REP_FACTOR_1;

      /* Check for bad combo of RC and rate */
      if ( ((rc == DEINT_RC3) || (rc == DEINT_RC5)) &&
           ((dec_sch_rate_type) sch_rate == DEC_SCH_RATE_32X) )
      {
        /* Return bad status */
        status = FALSE;

        M1X_MSG( MUX, LEGACY_ERROR,
          "Bad combination: RC %d and Rate %dx",
          DEINT_SCH_RC_INDEX(rc),
          1<<((int)sch_rate));
      }
      else
      {
        status = TRUE;
      }

      break;
    }

    case RXC_F_PDCCH0:
    case RXC_F_PDCCH1:
    {
      /* PDCCH parms are fixed, for now */
      demod_params->spr_rate   = MUXMDSP_WL_MASK_64;
      demod_params->rnd_factor = 3;

      status = TRUE;

      break;
    }

    case RXC_F_GCH0:
    case RXC_F_GCH1:
    {
      /* GCH parms are fixed, for now */
      demod_params->spr_rate   = SPR_256_CHIP_V;
      demod_params->rnd_factor = 3;

      status = TRUE;

      break;
    }

    /* other channels are not supported at the moment */
    default:
      status = FALSE;
  }

  M1X_MSG( MUX, LEGACY_MED,
    "chan %d, rc %d, dr %d, cr %d => spr %d, rep %d, rnd %d",
    chan_info->chan_id,
    chan_info->config.sch.rc,
    chan_info->config.sch.sch_rate,
    chan_info->config.sch.code_type,
    demod_params->spr_rate,
    demod_params->rep_factor,
    demod_params->rnd_factor);

  return status;

} /* rxc_get_chan_tc_demod_config_param */

/*===========================================================================

FUNCTION RXC_DEC_VERSION

DESCRIPTION
  Wrapper function for dev_version().
  Return the hardware version number for this circuit.

DEPENDENCIES
  None

RETURN VALUE
  Returns the version number of the circuit

SIDE EFFECTS
  None

===========================================================================*/
byte rxc_dec_version( void )
{
  return 0;

} /* rxc_dec_version */


/*===========================================================================

FUNCTION RXC_UPDATE_FER_SER_COUNTS

DESCRIPTION
  Update the FER and SER report statistics based on the fch_status and
  determined rate.

DEPENDENCIES
  RXC is in traffic and F-FCH is assigned.

RETURN VALUE
  None

SIDE EFFECTS
  FER and SER counts are incremented.

===========================================================================*/
void rxc_update_fer_ser_counts
(
  dec_fch_status_type fch_status,
  deint_rc_type       curr_rc,
  cai_data_rate_type  frame_rate
)
{
  uint16        min_ser;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Protect the counters from being accessed/cleared by multiple clients. */
  RXC_ENTER_CRITICAL_SECTION();

  /* use the rate to determine total symbols and SER */
  switch( frame_rate )
  {
    case CAI_EIGHTH_RATE:
      rxc_fer_ser_stats.fch_sym_errors += fch_status.ser[DEC_RATE_8TH-1];
      rxc_fer_ser_stats.fch_rate_cnt[1]++;
      break;

    case CAI_QUARTER_RATE:
      rxc_fer_ser_stats.fch_sym_errors += fch_status.ser[DEC_RATE_QTR-1];
      rxc_fer_ser_stats.fch_rate_cnt[2]++;
      break;

    case CAI_HALF_RATE:
      rxc_fer_ser_stats.fch_sym_errors += fch_status.ser[DEC_RATE_HALF-1];
      rxc_fer_ser_stats.fch_rate_cnt[3]++;
      break;

    case CAI_FULL_RATE:
      rxc_fer_ser_stats.fch_sym_errors += fch_status.ser[DEC_RATE_FULL-1];
      rxc_fer_ser_stats.fch_rate_cnt[4]++;
      break;

    case CAI_BLANK_RATE:
      rxc_fer_ser_stats.fch_frame_errors++;
      rxc_fer_ser_stats.fch_rate_cnt[0]++;

      /*fall through */
    default:
      min_ser = fch_status.ser[DEC_RATE_FULL-1];
      min_ser = ( (min_ser > fch_status.ser[DEC_RATE_HALF-1]) ?
                  fch_status.ser[DEC_RATE_HALF-1] : min_ser);
      min_ser = ( (min_ser > fch_status.ser[DEC_RATE_QTR-1])  ?
                  fch_status.ser[DEC_RATE_QTR-1] : min_ser);
      min_ser = ( (min_ser > fch_status.ser[DEC_RATE_8TH-1])  ?
                  fch_status.ser[DEC_RATE_8TH-1] : min_ser);
      rxc_fer_ser_stats.fch_sym_errors += min_ser;
  }

  rxc_fer_ser_stats.fch_frame_total++;

  /* NOTE: All symbol counts used for SER are scaled up after they are
     read from the decoder. This allows us to use the same total symbols/frame
     regardless of the frame rate.

     For RC3 and RC5, Symbol total is scaled up to 768. For other RCs,
     it is scaled up to 384 */
  if( curr_rc == CAI_RC_3 || curr_rc == CAI_RC_5 )
  {
    rxc_fer_ser_stats.fch_sym_total += 768;
  }
  else
  {
    rxc_fer_ser_stats.fch_sym_total += 384;
  }

  RXC_LEAVE_CRITICAL_SECTION();

} /* rxc_update_fer_ser_counts */

/*===========================================================================

FUNCTION RXC_GET_FER_SER_REPORT

DESCRIPTION
  Cumulative Symbol Error retrieval API.

DEPENDENCIES
  None

RETURN VALUE
  param[out] client_data       Client held data structure to be populated with
                               frame and symbol errors and total counts
  param[in]  reset_statistics  Pass TRUE to reset symbol errors and total to
                               zero after returning current values

  retval 0 indicates success
  retval non-zero indicates failure

SIDE EFFECTS
  None
===========================================================================*/
int rxc_get_fer_ser_report
(
  rxc_fer_ser_report_type *client_data,
  boolean reset_stats
)
{
  uint8 i;

  /*-----------------------------------------------------------------------*/

  /* Protect the counters from being accessed/cleared by multiple clients. */
  RXC_ENTER_CRITICAL_SECTION();

  /* copy current counts to returned structure */
  if(client_data != NULL)
  {
    client_data->fch_frame_errors = rxc_fer_ser_stats.fch_frame_errors;
    client_data->fch_frame_total  = rxc_fer_ser_stats.fch_frame_total;
    client_data->fch_sym_errors   = rxc_fer_ser_stats.fch_sym_errors;
    client_data->fch_sym_total    = rxc_fer_ser_stats.fch_sym_total;

    for(i = 0; i < 5; i++)
    {
      client_data->fch_rate_cnt[i] = rxc_fer_ser_stats.fch_rate_cnt[i];
    }
  }

  /* if requested to reset, zero out the local counts. */
  if(reset_stats)
  {
    memset(&rxc_fer_ser_stats, 0, sizeof(rxc_fer_ser_report_type));
  }

  RXC_LEAVE_CRITICAL_SECTION();

  return 0; /* success */

} /* rxc_get_fer_ser_report */


/*===========================================================================

FUNCTION RXC_IS_MSO_RDA_TEST_ENABLED

DESCRIPTION
  Return the value of the Markov Service Option test flag. This flag indicates
  if the extended test capabilities for this service option are enabled.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if test feature is enabled, FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean rxc_is_mso_rda_test_enabled( void )
{
  #ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
  return rxc_mso_rda_test_enable;
  #else
  return FALSE;
  #endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */

} /* rxc_is_mso_rda_test_enabled */

/*===========================================================================

FUNCTION rxc_get_pwr_stats

DESCRIPTION
  This function updates various power related parameters used for logging purposes.
  Such as Tx pwr, SB, FET, DRX, DTX etc

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_get_pwr_stats
(
  cai_radio_config_type    curr_rc,
  rxc_tx_srch_pwr_log_type *rxc_tx_srch_pwr_stats
)
{
  static uint8  offset_index = 0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( rxc_tx_srch_pwr_stats != NULL )
  {
    /* Initialize struct members with zero values */
    memset( rxc_tx_srch_pwr_stats, 0, sizeof(rxc_tx_srch_pwr_log_type) );

    rxc_tx_srch_pwr_stats->ustmr_ref   = VSTMR_XO_READ_FULL();
    rxc_tx_srch_pwr_stats->num_fing    = srch_mux_get_fing_count();
    rxc_tx_srch_pwr_stats->aset_size   = srch_mux_get_aset_size();
    rxc_tx_srch_pwr_stats->rxd_enabled = (srch_mux_is_rx_div_enabled()) ? 1: 0;
    rxc_tx_srch_pwr_stats->tx_pwr      = txc_get_tx_agc_dbm2();

    rxc_tx_sb_dtx_param.sb_decision[offset_index]  =
      ( txtc.rc8_status.blank_non_crit_fch_frame ? 1 : 0 );
    rxc_tx_sb_dtx_param.dtx_mask[offset_index]     =
       txc_device_only_dtx_param.dtx_mask;

    offset_index = ( offset_index + 1 ) % LOGGING_FRAME_OFFSET_SB_DTX ;

    if( rxc_common_data.rxc_state == RXC_TC_STATE )
    {
      rxc_tx_srch_pwr_stats->num_drx_pcg    = 0;
      #ifdef FEATURE_MODEM_1X_DEVICE_ONLY_DTX
      if( rxc_tx_sb_dtx_param.dtx_mask[offset_index] == 0xFFFF )
      {
        rxc_tx_srch_pwr_stats->dtx_percentage = 0;
      }
      else if( rxc_tx_sb_dtx_param.dtx_mask[offset_index] == 0xFF33 )
      {
        rxc_tx_srch_pwr_stats->dtx_percentage = 1;
      }
      else if( rxc_tx_sb_dtx_param.dtx_mask[offset_index] == 0x3333 )
      {
        rxc_tx_srch_pwr_stats->dtx_percentage = 2;
      }
      #endif /* FEATURE_MODEM_1X_DEVICE_ONLY_DTX */

      #ifdef FEATURE_IS2000_1X_ADV
      if( curr_rc == CAI_RC_11 )
      {
        rxc_tx_srch_pwr_stats->fl_frame_smartblanked =
          ( ( rxc_traffic_status.rxc_rc11.mdsp_dec_save_info.mdsp_dec_rate == \
              CAI_BLANK_RATE ) ? 1 : 0 );

        rxc_tx_srch_pwr_stats->rl_frame_smartblanked =
            rxc_tx_sb_dtx_param.sb_decision[offset_index];

          rxc_tx_srch_pwr_stats->rl_fet_num_pcg = 0;
        if( ( rxc_traffic_status.rxc_rc11.status.ack_received_pcg > 0  ) &&
            ( rxc_traffic_status.rxc_rc11.status.ack_received_pcg < 15 ) )
        {
            rxc_tx_srch_pwr_stats->rl_fet_num_pcg =
            15 - rxc_traffic_status.rxc_rc11.status.ack_received_pcg;
        }

          rxc_tx_srch_pwr_stats->fl_fet_num_pcg = 0;
        if( rxc_traffic_status.rxc_rc11.status.ack_transmitted_pcg != 0 )
        {
            rxc_tx_srch_pwr_stats->fl_fet_num_pcg =
            15 - rxc_traffic_status.rxc_rc11.status.ack_transmitted_pcg;
        }
      }
      else
      {
        rxc_tx_srch_pwr_stats->fl_frame_smartblanked = 0;
        rxc_tx_srch_pwr_stats->rl_frame_smartblanked = 0;
        rxc_tx_srch_pwr_stats->fl_fet_num_pcg        = 0;
        rxc_tx_srch_pwr_stats->rl_fet_num_pcg        = 0;
      }
      #endif /* FEATURE_IS2000_1X_ADV */
    }
    else
    {
      rxc_tx_srch_pwr_stats->num_drx_pcg           = 0;
      rxc_tx_srch_pwr_stats->dtx_percentage        = 0;
      rxc_tx_srch_pwr_stats->fl_frame_smartblanked = 0;
      rxc_tx_srch_pwr_stats->rl_frame_smartblanked = 0;
      rxc_tx_srch_pwr_stats->fl_fet_num_pcg        = 0;
      rxc_tx_srch_pwr_stats->rl_fet_num_pcg        = 0;
    }
  }
} /* rxc_get_pwr_stats */

/*===========================================================================

FUNCTION rxc_tx_update_flush_pwr_log

DESCRIPTION
  This function updates and flushes pwr log param for 0x1877 every 20ms

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_tx_update_flush_pwr_log( void )
{
  if( log_status( LOG_1X_PHYSICAL_LAYER_POWER_INFO_C ) )
  {
    log_set_code( &rxc_1x_pwr_log, LOG_1X_PHYSICAL_LAYER_POWER_INFO_C );
    log_set_length( &rxc_1x_pwr_log, sizeof(mux_1x_pwr_info_type) );
    log_set_timestamp( &rxc_1x_pwr_log );

    rxc_1x_pwr_log.log_pkt_version     = MUX_1X_PWR_LOG_VERSION;
    rxc_1x_pwr_log.ustmr               = rxc_srch_mux_pwr_stats.ustmr_ref;
    rxc_1x_pwr_log.number_of_fingers   = rxc_srch_mux_pwr_stats.num_fing;
    rxc_1x_pwr_log.aset_size           = rxc_srch_mux_pwr_stats.aset_size;
    rxc_1x_pwr_log.rxd                 = rxc_srch_mux_pwr_stats.rxd_enabled;
    rxc_1x_pwr_log.txpwr               = rxc_srch_mux_pwr_stats.tx_pwr;
    rxc_1x_pwr_log.num_drx             = rxc_srch_mux_pwr_stats.num_drx_pcg;
    rxc_1x_pwr_log.dtx_gating          = rxc_srch_mux_pwr_stats.dtx_percentage;
    #ifdef FEATURE_IS2000_1X_ADV
    rxc_1x_pwr_log.fl_smartblanked     =
      rxc_srch_mux_pwr_stats.fl_frame_smartblanked;
    rxc_1x_pwr_log.rl_smartblanked     =
      rxc_srch_mux_pwr_stats.rl_frame_smartblanked;
    rxc_1x_pwr_log.fl_early_terminated = rxc_srch_mux_pwr_stats.fl_fet_num_pcg;
    rxc_1x_pwr_log.rl_early_terminated = rxc_srch_mux_pwr_stats.rl_fet_num_pcg;
    #endif /* FEATURE_IS2000_1X_ADV */
  }

  if( rxc_1x_pwr_log.hdr.len > 0 )
  {
    log_submit( &rxc_1x_pwr_log );
    log_set_length( &rxc_1x_pwr_log, 0 );
  }
} /* rxc_tx_update_flush_pwr_log */

/*===========================================================================

FUNCTION RXC_MVS_SET_STANDBY_PROGRESS_STATUS

DESCRIPTION
  This function sets the mvs_cdma_standby_in_progress variable with desired
  value ( TRUE or FALSE )

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_mvs_set_standby_progress_status
(
  boolean standby_progress_status
)
{
  rex_enter_crit_sect( &rxc_voc_crit_sect );
  mvs_cdma_standby_in_progress = standby_progress_status;
  rex_leave_crit_sect( &rxc_voc_crit_sect );

  M1X_MSG( MUX, LEGACY_MED,
    "SET MVS_STANDBY_PROGRESS_STATUS %d",
     mvs_cdma_standby_in_progress );

}  /* rxc_mvs_set_standby_progress_status */

/*===========================================================================

FUNCTION RXC_MVS_GET_STANDBY_PROGRESS_STATUS

DESCRIPTION
  This function gets the value mvs_cdma_standby_in_progress variable

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
boolean rxc_mvs_get_standby_progress_status( void )
{
  boolean standby_progress_status;

  rex_enter_crit_sect( &rxc_voc_crit_sect );
  standby_progress_status = mvs_cdma_standby_in_progress;
  rex_leave_crit_sect( &rxc_voc_crit_sect );

  RX_DBG_MSG( "GET MVS_STANDBY_PROGRESS_STATUS %d",
     standby_progress_status );

  return standby_progress_status;
}  /* rxc_mvs_get_standby_progress_status */

/*===========================================================================

FUNCTION RXC_MVS_SET_STANDBY_STATUS

DESCRIPTION
  This function sets the mvs_standby_status variable with desired
  value ( MVS_STANDBY_SUCCESS or MVS_STANDBY_FAIL )

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_mvs_set_standby_status
(
  mc_mvs_standby_enum_type standby_status
)
{
  rex_enter_crit_sect( &rxc_voc_crit_sect );
  mvs_standby_status = standby_status;
  rex_leave_crit_sect( &rxc_voc_crit_sect );

  M1X_MSG( MUX, LEGACY_MED,
    "SET MVS_STANDBY_STATUS %d",
     mvs_standby_status );

}  /* rxc_mvs_set_standby_status */

/*===========================================================================

FUNCTION RXC_MVS_GET_STANDBY_STATUS

DESCRIPTION
  This function gets the value mvs_standby_status variable

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
mc_mvs_standby_enum_type rxc_mvs_get_standby_status( void )
{
  mc_mvs_standby_enum_type standby_status;

  rex_enter_crit_sect( &rxc_voc_crit_sect );
  standby_status = mvs_standby_status;
  rex_leave_crit_sect( &rxc_voc_crit_sect );

  RX_DBG_MSG( "GET MVS_STANDBY_STATUS %d",
    standby_status );

  return standby_status;

}  /* rxc_mvs_get_standby_status */

/*===========================================================================

FUNCTION RXC_SET_MVS_STATUS_ON_TC_ENTRY

DESCRIPTION
  This function sets the rxc_traffic_entry_mvs_enabled variable with desired
  value ( TRUE or FALSE ).To know if MVS is enabled already on TC entry or not.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_set_mvs_status_on_tc_entry
(
  boolean mvs_status
)
{
  rex_enter_crit_sect( &rxc_voc_crit_sect );
  rxc_traffic_entry_mvs_enabled = mvs_status;
  rex_leave_crit_sect( &rxc_voc_crit_sect );

  M1X_MSG( MUX, LEGACY_MED,
    "SET MVS STATUS ON TC ENTRY %d",
     rxc_traffic_entry_mvs_enabled );

}  /* rxc_mvs_set_standby_status */

/*===========================================================================

FUNCTION RXC_GET_MVS_STATUS_ON_TC_ENTRY

DESCRIPTION
  This function gets the value rxc_traffic_entry_mvs_enabled variable

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
boolean rxc_get_mvs_status_on_tc_entry( void )
{
  boolean mvs_status_tc_entry;

  rex_enter_crit_sect( &rxc_voc_crit_sect );
  mvs_status_tc_entry = rxc_traffic_entry_mvs_enabled;
  rex_leave_crit_sect( &rxc_voc_crit_sect );

  RX_DBG_MSG( "GET MVS STATUS ON TC ENTRY %d",
    mvs_status_tc_entry );

  return mvs_status_tc_entry;
}  /* rxc_get_mvs_status_on_tc_entry */

/*===========================================================================

FUNCTION RXC_SET_VOC_CONFIG_DONE

DESCRIPTION
  This function sets the rxc_frame_data.rxc_voc_config_done variable with desired
  value ( TRUE or FALSE ).To know if vocoder is already configured or not..

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_set_voc_config_done
(
  boolean config_done
)
{
  rex_enter_crit_sect( &rxc_voc_crit_sect );
  rxc_frame_data.rxc_voc_config_done = config_done;
  rex_leave_crit_sect( &rxc_voc_crit_sect );

  M1X_MSG( MUX, LEGACY_MED,
    "SET VOC CONFIG DONE %d",rxc_frame_data.rxc_voc_config_done );
}  /* rxc_set_voc_config_done */

/*===========================================================================

FUNCTION RXC_GET_VOC_CONFIG_DONE

DESCRIPTION
  This function gets the value rxc_frame_data.rxc_voc_config_done variable

DEPENDENCIES
  None

RETURN VALUE
  rxc_frame_data.rxc_voc_config_done

SIDE EFFECTS
  None.

===========================================================================*/
boolean rxc_get_voc_config_done( void )
{
  boolean voc_config_done;

  rex_enter_crit_sect( &rxc_voc_crit_sect );
  voc_config_done = rxc_frame_data.rxc_voc_config_done;
  rex_leave_crit_sect( &rxc_voc_crit_sect );

  RX_DBG_MSG( "GET VOC CONFIG_DONE %d",
       voc_config_done );

  return( voc_config_done );
}  /* rxc_get_voc_config_done */

/*========================================================================

FUNCTION RXC_MVS_GET_IS_SO_CHANGE

DESCRIPTION This function gets the value of rxc_mvs_so_change variable. To
            know if there is a SO change or not..

DEPENDENCIES None

RETURN VALUE True if SO change, False if no SO change.

SIDE EFFECTS None

=========================================================================*/

boolean rxc_mvs_get_is_so_change( void )
{
  boolean so_change = rxc_mvs_so_change;

  RX_DBG_MSG( "GET SO CHANGE %d",
       so_change );

  return so_change;
} /* rxc_mvs_get_is_so_change */

/*========================================================================

FUNCTION RXC_MVS_SET_IS_SO_CHANGE

DESCRIPTION This function sets the value of rxc_mvs_so_change variable with
            desired value ( TRUE or FALSE).

DEPENDENCIES None

RETURN VALUE None.

SIDE EFFECTS None

=========================================================================*/

void rxc_mvs_set_is_so_change( boolean so_change )
{
  rxc_mvs_so_change = so_change;

  M1X_MSG( MUX, LEGACY_MED,
  "SET SO CHANGE %d", rxc_mvs_so_change );
} /* rxc_mvs_set_is_so_change */

/*===========================================================================

FUNCTION RXC_USE_SW_DEMBACK

DESCRIPTION
  This function returns TRUE if the HW demback is used by another RAT,
  thereby needing to use the SW implementation.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - When SW demback needs to be used.
  FALSE - When HW demback is available

SIDE EFFECTS
  None.

===========================================================================*/
boolean rxc_use_sw_demback( void )
{
  boolean rxc_sw_demback_enable = FALSE;

  /* TODO: Add more conditions and check, with the current changes, SW demback
     will always get enabled. This functionis never called for or from Traffic
     State so a specific check for traffic is not needed */
  #ifdef FEATURE_MODEM_1X_SW_DEMBACK
  if( !onex_nonsig_is_ftm_mode() )
  {
    rxc_sw_demback_enable = TRUE;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Use HW Demback in NS FTM mode" );
  }
  #endif /* FEATURE_MODEM_1X_SW_DEMBACK */

  return rxc_sw_demback_enable;
} /* rxc_use_sw_demback */


#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION mc_qsh_mdump_collect_rxccommon_data

DESCRIPTION
This function dumps the rxccommon data to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void mc_qsh_mdump_collect_rxccommon_data()
{
  qsh_mdump_collect_high(&rxc_common_data,sizeof(rxc_common_data));
  qsh_mdump_collect_high(&rxc_tdso_frame_block,sizeof(rxc_tdso_frame_block));
}
#endif /* FEATURE_QSH_MDUMP */